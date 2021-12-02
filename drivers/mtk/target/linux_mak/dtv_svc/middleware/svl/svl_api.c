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
 * $RCSfile: svl_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains implementation of the exported Service List (SVL)
 *    library API's.
 *   
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "cdb/cdb_api.h"
#include "svl/svl.h"
#include "svl/_svl.h"
#include "svl/svl_rec.h"
#include "svl/svl_rec_id.h"
#include "svl/svl_fs.h"
#include "svl/svl_dbg.h"

//#define ENABLE_SVL_DUMP
//#define ENABLE_SVL_API_LOG
static BOOL s_b_svl_api_log = FALSE;
static BOOL s_b_svl_rec_dump = FALSE;

#define SVL_API_LOG if(s_b_svl_api_log) x_dbg_stmt


/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: x_svl_create
 *
 * Description: Create a SVL object and return a handle to this SVL object.
 *
 * Inputs: 
 *   ps_svl_name   Name of the SVL object. 
 *   ui2_svl_id    Unique id for the SVL object.  
 *                 The limit for SVL ID is between 1 and 65535.
 *   e_mem_policy  Memory policy: SVL_MEM_STATIC or SVL_MEM_DYNAMIC
 *   ui2_max_nb_rec Specifies the maximum number of SVL record (for
 *                 SVL_MEM_STATIC), or it specifies number of
 *                 SVL records per memory chunk.
 *   pv_tag        Pointer to a private tag value provided by the 
 *                 caller.  This tag value is passed to the 
 *                 caller's notification function.
 *   pf_nfy        Notification function provided by the caller.  
 *                 This function is called to notify the owner of 
 *                 this SVL handle when the SVL object has been 
 *                 modified.  If NULL, then no notification function 
 *                 will be called.
 *
 * Outputs:
 *   ph_svl        Handle to the SVL object.
 *
 * Returns: 
 *   SVLR_OK           SVL object is created successfully.
 *   SVLR_INV_ARG      'ui2_svl_id' is 0, or ph_svl is NULL, or ps_svl_name
 *                     is 0..
 *   SVLR_OUT_OF_HANDLE Cannot get a handle.
 *   SVLR_FAIL         The specified 'ps_svl_name' or 'ui2_svl_id' value is not *                     unique (e.g., it is currently been used), or limit of
 *                     the number of SVL objects that can be created is
 *                     reached.
 *   SVLR_OUT_OF_MEM   Not enough memory to create SVL object.
 *   SVLR_INV_ARG      'ps_svl_name' or 'ph_svl' is NULL.
 * 
 ----------------------------------------------------------------------------*/
INT32 x_svl_create
(
    const CHAR*      ps_svl_name,
    UINT16           ui2_svl_id,
    SVL_MEM_POLICY_T e_mem_policy,
    UINT16           ui2_max_nb_rec,
    VOID*            pv_tag,
    x_svl_nfy_fct    pf_nfy,
    HANDLE_T*        ph_svl
)
{
    INT32  i4_rc=SVLR_FAIL;
    CHAR   s_svl_name[MAX_SVL_NAME_LEN+1];
    
    if ( ui2_svl_id != 0            &&
         ps_svl_name != NULL        &&
         x_strlen(ps_svl_name) > 0  &&
         ph_svl != NULL )
    {
        x_strncpy(s_svl_name,ps_svl_name,MAX_SVL_NAME_LEN);
        s_svl_name[MAX_SVL_NAME_LEN]='\0';
            
        svl_sema_lock();
        /* check if the input SVL name and id has not been used. */
        /* Add the name and id to the internal list. */
        if ( svl_check_id_name(ui2_svl_id,s_svl_name) == TRUE &&
             svl_add_id_name(ui2_svl_id,s_svl_name) == SVLR_OK
            )
        {
            *ph_svl=NULL_HANDLE;
            i4_rc=svl_create_object(s_svl_name,ui2_svl_id,
                                    e_mem_policy,ui2_max_nb_rec,
                                    pv_tag,pf_nfy,ph_svl);
            if ( i4_rc != SVLR_OK )
            {
                /* Remove the id name pair. */
                svl_del_id(ui2_svl_id);
            }
        }
        svl_sema_unlock();
    }
    else
    {
        i4_rc=SVLR_INV_ARG;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} create svl [name: %s svl_id: %d handle: %d] \n", ps_svl_name, ui2_svl_id, *ph_svl));
    }
    else
    {
        DBG_ERROR(("{SVL} create [name: %s svl_id: %d] failed.\n", ps_svl_name, ui2_svl_id));
    }

    SVL_API_LOG("{svl}%s(%s,%d,R 0x%x)=%d\n",__FUNCTION__,ps_svl_name,ui2_svl_id,((ph_svl==0)?NULL_HANDLE:(*ph_svl)),i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_delete
 *
 * Description: This API deletes the specified SVL object associated 
 *              with 'h_svl' handle.    Other clients with a handle to 
 *              the deleted SVL object will receive a notification of 
 *              the delete event.   
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Specified SVL object is deleted.
 *   SVLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 x_svl_delete
(
    HANDLE_T    	h_svl
)
{
    INT32           i4_rc;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        svl_sema_lock();
        /* remove svl id/name pairs. */
        svl_del_id(pt_svl->ui2_svl_id);
        svl_sema_unlock();
        
        i4_rc=svl_delete_object(pt_svl);
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} delete handle [%d] \n",h_svl));
    }
    else
    {
        DBG_ERROR(("{SVL} delete handle [%d] failed.\n",h_svl));
    }

    SVL_API_LOG("{svl}%s(0x%x)=%d\n",__FUNCTION__,h_svl,i4_rc);
    return i4_rc;      
}

/*----------------------------------------------------------------------
 * Name: x_svl_clean
 *
 * Description: This API removes all the service records from the 
 *              specified SVL object pointed to by the 'h_svl' handle.    
 *              All other handles to this SVL object will receive a 
 *              notification.   
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           All records are successfully removed. 
 *   SVLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 x_svl_clean
(
    HANDLE_T    	h_svl
)
{
    INT32           i4_rc;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        i4_rc=svl_clean_object(pt_svl);
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} clean handle [%d] \n",h_svl));
    }
    else
    {
        DBG_ERROR(("{SVL} clean handle [%d] failed.\n",h_svl));
    }

    SVL_API_LOG("{svl}%s(0x%x)=%d\n",__FUNCTION__,h_svl,i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_add_rec
 *
 * Description: This function adds a new service record to the SVL 
 *              database object.  The SVL library will make copy of 
 *              the record content from the address location pointed to 
 *              by the 'pt_svl_rec'.  The 'ui2_svl_rec_id' field in 
 *              the 'pt_svl_rec' structure specifies the key field of the 
 *              service record to be added to the database.  
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *   pt_svl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Record is successfully added. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *   SVLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   SVLR_INV_ARG      'pt_svl_rec' is null or the SVL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 x_svl_add_rec
(
    HANDLE_T           h_svl,
    const SVL_REC_T*   pt_svl_rec
)
{
    INT32           i4_rc;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pt_svl_rec                 != NULL &&
             pt_svl_rec->ui2_svl_rec_id != SVL_NULL_REC_ID )
        {
            i4_rc=svl_write_rec_to_object(pt_svl,
                                          pt_svl_rec,
                                          (UINT8)SVL_APPEND_RECORD);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    SVL_API_LOG("{svl}%s(0x%x)=%d\n",__FUNCTION__,h_svl,i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_del_rec
 *
 * Description: This function deletes a specified service record from 
 *              the SVL.  The record to be deleted is specified by the 
 *              service record id.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_svl_rec_id  Specifies id the service record to be deleted. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK            Specified record is deleted. 
 *   SVLR_INV_HANDLE    Bad SVL handle.
 *   SVLR_INV_ARG       ui2_svl_rec_id is a NULL record id.
 *   SVLR_REC_NOT_FOUND The specified record to be deleted is not in 
 *                      the SVL object.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_del_rec
(
    HANDLE_T           h_svl,
    UINT16             ui2_svl_rec_id
)
{
    INT32           i4_rc;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( ui2_svl_rec_id != SVL_NULL_REC_ID )
        {
            i4_rc=svl_del_rec_from_object(pt_svl, ui2_svl_rec_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    SVL_API_LOG("{svl}%s(0x%x,%d)=%d\n",__FUNCTION__,h_svl,ui2_svl_rec_id,i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_update_rec
 *
 * Description: This function updates a service record to the SVL object.   
 *              If the key field value (e.g., 'ui2_svl_rec_id' field in 
 *              the 'pt_svl_rec' structure) of the input service record 
 *              matches an existing record in the SVL object, then the 
 *              existing record is over-wrote with the new information,  
 *              otherwise a new record is created in the SVL database. 
 * 
 *              If the 'b_must_exist' flag is TRUE, then there must be an 
 *              existing record with the same key field value, else a status 
 *              of 'SVLR_REC_NOT_FOUND' is returned to the caller.  
 *              If 'b_must_exist' flag is FALSE, then a new service record 
 *              is created if no existing record matching the key field value 
 *              is found in the SVL database.
 *
 *              Call to this function must be protected, e.g., the SVL 
 *              database object needs to be locked before the call to this 
 *              function and unlock after returning from call to this function.
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *   pt_svl_rec Pointer to service record structure. 
 *   b_must_exist Boolean flag specifying if the service record to be 
 *                updated should exist in the SVL database
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Record is successfully added/updated.
 *   SVLR_INV_HANDLE   Bad SVL handle. 
 *   SVLR_OUT_OF_MEM   Not enough memory to create a new SVL record.
 *   SVLR_REC_NOT_FOUND The specified record is not found 
 *                      ('b_must_exist' == TRUE)
 *   SVLR_INV_ARG      'pt_svl_rce' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_update_rec
(
    HANDLE_T           h_svl,
    const SVL_REC_T*   pt_svl_rec,
    BOOL               b_must_exist	
)
{
    INT32           i4_rc;
    SVL_OBJ_T*      pt_svl;
    HANDLE_TYPE_T   pe_type;
    UINT8           ui1_write_mode;
    SVL_REC_T       t_tmp_svl_rec;
    UINT32          ui4_ver_id = 0;
    INT32           i4_persist_data_changed = 1 ;

    /* For DVB record, we will check if the data to be updated
       are persistent data.
    */
    if ( (pt_svl_rec != NULL) &&
         (
             (pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type == BRDCST_TYPE_DVB ) ||
             (pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type == BRDCST_TYPE_ANALOG ) ||
             (pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type == BRDCST_TYPE_DTMB )
         )
        )
    {
        /* get the existing SVL record. */
        if ( x_svl_get_rec(h_svl,  pt_svl_rec->ui2_svl_rec_id,
                           &t_tmp_svl_rec, &ui4_ver_id)
             == SVLR_OK )
        {
            /* Evaluate the update. */
            i4_persist_data_changed =
                svl_is_updated_flds_persistent_data(
                    &t_tmp_svl_rec, pt_svl_rec);

            /* This is fix (CR00132122) to improve the speed for
               updating record.
               
               Since we know that this record (e.g, DVB record)
               exist, we will change the 'b_must_exist' to TRUE
               regardless of what user specified.  The 'update'
               operation to an existing record is much faster.
            */
            b_must_exist = TRUE;
            
        }
    }
        

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pt_svl_rec != NULL                             &&
             pt_svl_rec->ui2_svl_rec_id != SVL_NULL_REC_ID
           )
        {
            if ( b_must_exist == TRUE )
            {
                ui1_write_mode=SVL_MODIFY_RECORD;
            }
            else
            {
                ui1_write_mode=SVL_UPDATE_RECORD;
            }
            
            i4_rc=svl_write_rec_to_object(pt_svl,
                                          pt_svl_rec,
                                          ui1_write_mode);            
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    /* If update is successful, then we check if persistent data flag
       has been set for the update operation.  If set to 0 (e.g, no
       persistent data is modified), then reset the version id to prevent
       file system write when SVL data is save to file.
    */
    if ( i4_rc == SVLR_OK )
    {
        if ( i4_persist_data_changed == 0 )
        {
            /* reset the CDB version id. */
            /*
            x_dbg_stmt("Persistent data is not changed. reset version id to: %d\n",
                       ui4_ver_id);
            */
            
            if ( db_set_data_version_id(pt_svl->h_cdb, ui4_ver_id) != DBR_OK )
            {
                DBG_ERROR(("{SVL} Error: can not reset version id.\n"));
            }
        }
    }

    SVL_API_LOG("{svl}%s(0x%x)=%d\n",__FUNCTION__,h_svl,i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_qry_rec_id
 *
 * Description: This API queries the range (FIRST, LAST, or NEXT) of 
 *              valid service record id values used in the SVL object.  
 *   
 * Inputs: 
 *   h_svl             Handle to the SVL object.
 *   e_svl_qry_option  Flag to specifies the record id value to be returned.  
 *                     The values are:
 *                     SVL_FIRST_REC_ID,  SVL_LAST_REC_ID, SVL_NEXT_REC_ID.
 *
 * Outputs: None
 *   pui2_rec_id       Contain the record id value.	
 *
 * Returns: 
 *   SVLR_OK           SVL record id return.
 *   SVLR_INV_HANDLE   Bad SVL handle. 
 *   SVLR_INV_ARG      'pui2_rec_id' is NULL.

 -----------------------------------------------------------------------*/

INT32 x_svl_qry_rec_id
(
    HANDLE_T           h_svl,
    SVL_REC_ID_QRY_T   e_svl_qry_option,
    UINT16*            pui2_rec_id	
)
{
    INT32           i4_rc;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pui2_rec_id != NULL )
        {
            switch (e_svl_qry_option)
            {
                case SVL_FIRST_REC_ID:
                {
                    if ( svl_get_smallest_rec_id(pt_svl, pui2_rec_id) == TRUE )
                    {
                        i4_rc=SVLR_OK;
                    }
                    else
                    {
                        i4_rc=SVLR_REC_NOT_FOUND ;
                    }
                }
                break;

                case SVL_LAST_REC_ID:
                {
                    if ( svl_get_largest_rec_id(pt_svl, pui2_rec_id) == TRUE )
                    {
                        i4_rc=SVLR_OK;
                    }
                    else
                    {
                        i4_rc=SVLR_REC_NOT_FOUND ;
                    }
                }
                break;

                case SVL_NEXT_REC_ID:
                {
                    svl_get_next_rec_id(pt_svl, pui2_rec_id);
                    i4_rc=SVLR_OK;
                }
                break;

                default:
                {
                    *pui2_rec_id=SVL_NULL_REC_ID;
                    i4_rc=SVLR_INV_ARG;
                }
            }
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_lock
 *
 * Description: This function locks the SVL object.   The function blocks 
 *              until the lock is acquired.
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL is locked. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *
 -----------------------------------------------------------------------*/
INT32 x_svl_lock
(
    HANDLE_T           h_svl
) 
{
    INT32           i4_rc;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        i4_rc=svl_lock_object(pt_svl);
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} lock handle [%d] \n",h_svl));
    }
    else
    {
        DBG_ERROR(("{SVL} lock handle [%d] failed.\n",h_svl));
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_unlock
 *
 * Description: This function unlocks the SVL database. 
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK          SVL database is unlocked. 
 *   SVLR_INV_HANDLE  Bad SVL handle.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_unlock
(
    HANDLE_T           h_svl
) 
{ 
    INT32           i4_rc;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        i4_rc=svl_unlock_object(pt_svl);
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} unlock handle [%d] \n",h_svl));
    }
    else
    {
        DBG_ERROR(("{SVL} unlock handle [%d] failed.\n",h_svl));
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_read_lock
 *
 * Description: This function locks the SVL object for read operation.
 *   The function blocks (if write lock is active) until the read lock
 *   is acquired.
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL is locked. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *
 -----------------------------------------------------------------------*/
INT32 x_svl_read_lock
(
    HANDLE_T           h_svl
)
{
    INT32           i4_rc;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        i4_rc=svl_lock_object_for_read(pt_svl);
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} read-lock handle [%d] \n",h_svl));
    }
    else
    {
        DBG_ERROR(("{SVL} read-lock handle [%d] failed.\n",h_svl));
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_read_unlock
 *
 * Description: This function release the read-lock on SVL database. 
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK          SVL database is unlocked. 
 *   SVLR_INV_HANDLE  Bad SVL handle.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_read_unlock
(
    HANDLE_T           h_svl
)
{
    INT32           i4_rc;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        i4_rc=svl_unlock_object_for_read(pt_svl);
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} unlock read-handle [%d] \n",h_svl));
    }
    else
    {
        DBG_ERROR(("{SVL} unlock read-handle [%d] failed.\n",h_svl));
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_fs_load
 *
 * Description: This API loads a specified SVL object from file system 
 *              into memory, and returns a handle to the newly created 
 *              SVL object.
 *
 * Inputs: 
 *  h_dir          The handle to the directory.  The 'ps_svl_path'
 *                 will be relative to the specified 'h_dir'.
 *
 *  ps_svl_path    Specifies the directory path of the SVL object in the 
 *                 file system. See File Path Specification, 10.1, for the 
 *                 types of file path that can be specified.
 *
 *  ui2_svl_id     Specifies the SVL id to load from the file system.  
 *                 The limit for SVL ID is between 1 and 65535.
 *
 *  ps_svl_name    Specifies the name for the SVL object.  If null string 
 *                 is specified, then the default name stored in the file 
 *                 system is used to create the SVL object.
 *
 *  pv_tag         Pointer to a private tag value provided by the caller.  
 *                 This tag value is passed to the caller's notification 
 *                 function.
 *
 *  pf_nfy         Notification function provided by the caller.  This 
 *                 function is called to notify the owner of this SVL handle 
 *                 when the SVL object has been modified.  If NULL then no 
 *                 notification function will be called.
 *
 * Outputs: 
 *  ph_svl         On return, contain the handle to the SVL object created 
 *                 from file system.
 * Returns: 
 *   SVLR_OK             SVL object is created. 
 *
 *   SVLR_NOT_FOUND      'ui2_svl_id' is not found or 'ps_svl_path' 
 *                       does not exist.
 *
 *   SVLR_OUT_OF_HANDLE  Cannot get a handle.
 *
 *   SVLR_OUT_OF_MEM     Not enough memory to create SVL object.
 *
 *   SVLR_FAIL           The specified 'ps_svl_name' or 'ui2_svl_id' value is 
 *                       not unique.
 *
 *   SVLR_INV_ARG        'ps_svl_name' or 'ph_svl' or 'ps_svl_path' is NULL.
 *
 *   SVLR_INV_FILE_PATH  Specified file path is not a directory or a RAW 
 *                       character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_fs_load
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id,
    const CHAR*	    ps_svl_name,
    VOID*			pv_tag,
    x_svl_nfy_fct	pf_nfy,
    HANDLE_T*		ph_svl	
)
{
    INT32           i4_rc;
    CHAR            s_svl_name[MAX_SVL_NAME_LEN+1];
    
    SVL_API_LOG("{svl}%s(%s,%d,%s) begin\n",__FUNCTION__,ps_svl_path,ui2_svl_id,ps_svl_name);
    if ( ps_svl_path != NULL     &&
         ph_svl      != NULL     &&
         ui2_svl_id  != 0   
        )
    {
        *ph_svl=NULL_HANDLE;
        if ( ps_svl_name != NULL )
        {
            x_strncpy(s_svl_name,ps_svl_name,MAX_SVL_NAME_LEN);
            s_svl_name[MAX_SVL_NAME_LEN]='\0';
            ps_svl_name=s_svl_name;
        }

        DBG_API(("{SVL} loading SVL data.\n"));
        
        i4_rc=svl_fs_load_object(
            h_dir,
            ps_svl_path,
            ui2_svl_id,
            ps_svl_name,
            pv_tag,
            pf_nfy,
            ph_svl);

        DBG_API(("{SVL} done loading SVL data [ret code: %d] \n",i4_rc));
    }
    else
    {
        i4_rc = SVLR_INV_ARG;
    }

    if(s_b_svl_rec_dump)
    {
        svl_dump_rec(ui2_svl_id,BRDCST_TYPE_UNKNOWN);
    }

    SVL_API_LOG("{svl}%s(%s,%d,%s,R 0x%x)=%d end\n",__FUNCTION__,ps_svl_path,ui2_svl_id,ps_svl_name,((ph_svl==0)?NULL_HANDLE:(*ph_svl)),i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_fs_store
 *
 * Description: This API stores the specified SVL object to the 
 *              file system.
 *
 * Inputs:
 *   h_dir         The handle to the directory.  The 'ps_svl_path'
 *                 will be relative to the specified 'h_dir'.
 *
 *   ps_svl_path   Specifies the filename path to store the SVL 
 *                 object in the file system.  
 *
 *  ui2_svl_id     Specifies the SVL object to be stored to the file system.  
 *                 The limit for SVL ID is between 1 and 65535.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK             SVL object is stored to the file system. 
 *
 *   SVLR_NOT_FOUND      The SVL object specified by the 'ui2_svl_id' 
 *                       does not exist.
 *
 *   SVLR_INV_ARG        ps_svl_path' is NULL.
 *
 *   SVLR_INV_FILE_PATH  Specified file path is not a directory or 
 *                       a RAW character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_fs_store
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id	
)
{
    INT32       i4_rc;

    SVL_API_LOG("{svl}%s(%s,%d) begin\n",__FUNCTION__,ps_svl_path,ui2_svl_id);
    
    if ( ps_svl_path != NULL &&
         ui2_svl_id  != 0
        )
    {
        DBG_API(("{SVL} saving SVL data.\n"));
        
        i4_rc=svl_fs_store_object(h_dir,ps_svl_path,ui2_svl_id);

        DBG_API(("{SVL} done saving SVL data [ret code: %d]\n",i4_rc));
    }
    else
    {
        i4_rc = SVLR_INV_ARG;
    }

    if(s_b_svl_rec_dump)
    {
        svl_dump_rec(ui2_svl_id,BRDCST_TYPE_UNKNOWN);
    }

    SVL_API_LOG("{svl}%s(%s,%d)=%d end\n",__FUNCTION__,ps_svl_path,ui2_svl_id,i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_fs_del
 *
 * Description: This API deletes a specified SVL object on the file system.
 *
 * Inputs:
 *   h_dir          The handle to the directory.  The 'ps_svl_path'
 *                  will be relative to the specified 'h_dir'.
 *
 *   ps_svl_path    Specifies the filename path of the SVL object in 
 *                  the file system. 
 *
 *   ui2_svl_id     Specifies the SVL object to be deleted from the 
 *                  file system.  The limit for SVL ID is between 1 
 *                  and 65535.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK             SVL object is deleted from the file system. 
 *
 *   SVLR_NOT_FOUND      'ui2_svl_id' is not found.
 *
 *   SVLR_INV_ARG        'ps_svl_path' is NULL.
 *
 *   SVLR_INV_FILE_PATH  Specified file path, 'ps_svl_path', does not exist, or
 *                       it is not a directory or a RAW character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_fs_del
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id
)
{
    INT32           i4_rc;
    
    if ( ps_svl_path != NULL &&
         ui2_svl_id  != 0
        )
    {   
        i4_rc=svl_fs_del_object(h_dir,ps_svl_path, ui2_svl_id);
    }
    else
    {
        i4_rc = SVLR_INV_ARG;
    }

    SVL_API_LOG("{svl}%s(%s,%d)=%d\n",__FUNCTION__,ps_svl_path,ui2_svl_id,i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_fs_qry
 *
 * Description: This API queries the SVL objects stored in file system.
 *              It return the SVL object name and id value.
 * Inputs:
 *   h_dir         The handle to the directory.  The 'ps_svl_path'
 *                 will be relative to the specified 'h_dir'.
 *
 *   ps_svl_path   Specifies the directory path where SVL objects 
 *                 are stored.  
 *
 *   ui2_idx       Index value to get the next SVL object.  
 *                 0 means the first SVL, 1 is the second SVL, etc.
 *
 * Outputs: 
 *   pui2_svl_id   Pointer to a UINT16 variable. On return, it contains 
 *                 SVL ID.
 *
 *   ps_svl_name   Pointer to a string buffer (minimum 17 bytes) .  
 *                 On return, it contains the default SVL name stored on 
 *                 the file system. 
 *
 * Returns: 
 *   SVLR_OK              SVL object found. 
 *
 *   SVLR_NOT_FOUND       No SVL object found.
 *
 *   SVLR_INV_ARG         'ps_svl_name' or 'pui2_svl_id' is NULL.
 *
 *   SVLR_INV_FILE_PATH   Specified file path is not a directory or a 
 *                        RAW character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_fs_qry
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_idx,
    UINT16*	   	    pui2_svl_id,
    CHAR*           ps_svl_name	
)
{
    INT32           i4_rc;
    
    if ( ps_svl_path != NULL     &&
         pui2_svl_id != NULL     &&
         ps_svl_name != NULL
        )
    {
        *pui2_svl_id = SVL_NULL_ID;
        ps_svl_name[0] = '\0';
        
        i4_rc=svl_qry_persistent_objects(
            h_dir,
            ps_svl_path,
            ui2_idx,
            pui2_svl_id,
            ps_svl_name);
    }
    else
    {
        i4_rc = SVLR_INV_ARG;
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: x_svl_fs_sync
 *
 * Description: This API sync a specified SVL handle with SVL records
 *              from persistent file system.  The existing SVL records
 *              are replaced with new records read from file. 
 *
 * Inputs:
 *
 *   h_svl          Specify the handle to the SVL object.
 *
 *   h_dir          The handle to the directory.  The 'ps_svl_path'
 *                  will be relative to the specified 'h_dir'.
 *
 *   ps_svl_path    Specifies the directory path of the SVL object in the 
 *                  file system. See File Path Specification, 10.1, for the 
 *                  types of file path that can be specified.
 *
 *   ui2_svl_id     Specifies the SVL id to load from the file system.  
 *                  The limit for SVL ID is between 1 and 65535.
 *
 * Outputs: 
 *
 *   None.
 *                 
 * Returns: 
 *   SVLR_OK             SVL records are load from the persistent file
 *                       into the memory of SVL object.
 *
 *   SVLR_NOT_FOUND      'ui2_svl_id' is not found or 'ps_svl_path' 
 *                       does not exist.
 *
 *   SVLR_FAIL           internal error
 *
 *   SVLR_INV_ARG        'ps_svl_path' is NULL, or 'h_svl' is NULL_HANDLE,
 *                       or ui2_svl_id is 0.
 *
 *   SVLR_INV_FILE_PATH  Specified file path is not a directory or a RAW 
 *                       character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_fs_sync
(
    HANDLE_T		h_svl,	
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id
)
{
    INT32           i4_rc;

    SVL_API_LOG("{svl}%s(0x%x,%s,%d) begin\n",__FUNCTION__,h_svl,ps_svl_path,ui2_svl_id);
    
    if ( ps_svl_path != NULL            &&
         h_svl       != NULL_HANDLE     &&
         ui2_svl_id  != 0   
        )
    {
        i4_rc=svl_fs_sync_object(h_svl,
                                 h_dir,
                                 ps_svl_path,
                                 ui2_svl_id);
    }
    else
    {
        i4_rc = SVLR_INV_ARG;
    }
    
    if(s_b_svl_rec_dump)
    {
        svl_dump_rec(ui2_svl_id,BRDCST_TYPE_UNKNOWN);
    }

    SVL_API_LOG("{svl}%s(0x%x,%s,%d)=%d end\n",__FUNCTION__,h_svl,ps_svl_path,ui2_svl_id,i4_rc);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_open
 *
 * Description: This function create a handle to the specified SVL database 
 *              object by its ID value.    
 *
 * Inputs: 
 *   ui2_svl_id:  Specifies the SVL object identifier value.  
 *                The limit for SVL ID is between 1 and 65535.
 *   pv_tag:      A private tag value associated with the calling client.
 *   pf_nfy:      Pointer to the caller provided notification function.  
 *                The notification function will be called in the context of 
 *                the thread that modifies (update, add, delete) the SVL 
 *                database.  If NULL, then no notification function will be 
 *                called.
 *
 * Outputs:
 *   ph_svl:      Handle to the SVL object.
 *
 * Returns: 
 *   SVLR_OK               SVL handle is returned. 
 *   SVLR_OUT_OF_HANDLE    No more handle available.
 *   SVLR_OUT_OF_MEM       Not enough memory to create the SVL control 
 *                         structure.
 *   SVLR_NOT_FOUND        The specified SVL database object id (e.g. 
 *                         'ui2_svl_id') is not found.
 *   SVLR_INV_ARG          'ph_svl' is NULL.
 *   
 -----------------------------------------------------------------------*/
extern INT32 x_svl_open
(
    UINT16		    ui2_svl_id,
    VOID*		    pv_tag,
    x_svl_nfy_fct   pf_nfy,
    HANDLE_T*		ph_svl
) 
{ 
    INT32           i4_rc;
    CHAR            ps_svl_name[MAX_SVL_NAME_LEN+1];

    if ( ph_svl      != NULL &&
         ui2_svl_id  != 0
        )
    {
        *ph_svl=NULL_HANDLE;
        svl_sema_lock();
        /* Find the id/name */
        if ( svl_get_name_by_id(ui2_svl_id, ps_svl_name) == TRUE )
        {
            i4_rc=svl_open_object(ui2_svl_id,
                                  ps_svl_name,
                                  pv_tag,
                                  pf_nfy,
                                  ph_svl);
        }
        else
        {
            i4_rc=SVLR_NOT_FOUND;
        }
        svl_sema_unlock();
    }
    else
    {
        i4_rc = SVLR_INV_ARG;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} open SVL [svl_id: %d], handle [%d] \n",ui2_svl_id,*ph_svl));
    }
    else
    {
        DBG_ERROR(("{SVL} open SVL [svl_id: %d] failed.\n",ui2_svl_id));
    }

    SVL_API_LOG("{svl}%s(%d,R 0x%x)=%d\n",__FUNCTION__,ui2_svl_id,((ph_svl==0)?NULL_HANDLE:(*ph_svl)),i4_rc);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_open_by_name
 *
 * Description: This function creates a handle to the specified SVL database 
 *              object by its name.    
 *
 * Inputs: 
 *   ps_svl_name    Specifies the SVL object name (16 char max).
 *   pv_tag         A private tag value associated with the calling client.
 *   pf_nfy         Pointer to a caller provided notification function.  
 *                  The notification function is called in the context of 
 *                  the thread that modifies (update, add, delete) the SVL 
 *                  database.  If NULL, then no notification function will 
 *                  be called.
 *
 * Outputs:
 *   ph_svl:        Handle to the SVL object.
 *
 * Returns: 
 *   SVLR_OK             SVL handle is returned. 
 *   SVLR_OUT_OF_HANDLE  No more handle available.
 *   SVLR_OUT_OF_MEM     Not enough memory to create the SVL handle structure.
 *   SVLR_NOT_FOUND      The specified SVL database object name (e.g., 
 *                       ps_svl_name)  is not found.
 *   SVLR_INV_ARG        'ph_svl' or 'ps_svl_name' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_open_by_name
(
    const CHAR*     ps_svl_name,
    VOID*		    pv_tag,
    x_svl_nfy_fct	pf_nfy,
    HANDLE_T*		ph_svl	
) 
{
    INT32           i4_rc;
    UINT16          ui2_svl_id;
    CHAR            s_svl_name[MAX_SVL_NAME_LEN+1];
    
    if ( ph_svl  != NULL && ps_svl_name != NULL )
    {
        *ph_svl=NULL_HANDLE;

        x_strncpy(s_svl_name,ps_svl_name,MAX_SVL_NAME_LEN);
        s_svl_name[MAX_SVL_NAME_LEN]='\0';
            
        svl_sema_lock();
        /* Find the id/name */

        
        if ( svl_get_id_by_name(s_svl_name, &ui2_svl_id) == TRUE )
        {
            i4_rc=svl_open_object(ui2_svl_id,
                                  s_svl_name,
                                  pv_tag,
                                  pf_nfy,
                                  ph_svl);
            svl_sema_unlock();
        }
        else
        {
            svl_sema_unlock();
            i4_rc=SVLR_NOT_FOUND;
        }
    }
    else
    {
        i4_rc = SVLR_INV_ARG;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} open SVL [name: %s], handle [%d] \n",ps_svl_name, *ph_svl));
    }
    else
    {
        DBG_ERROR(("{SVL} open SVL [name: %s] failed.\n",ps_svl_name));
    }

    SVL_API_LOG("{svl}%s(%s,R 0x%x)=%d\n",__FUNCTION__,ps_svl_name,((ph_svl==0)?NULL_HANDLE:(*ph_svl)),i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_close
 *
 * Description: This function frees the specified handle to the SVL 
 *              database object.    
 *
 * Inputs: 
 *   h_svl	Handle to SVL database object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL handle is freed. 
 *   SVLR_INV_HANDLE   Bad handle.
 *   
 -----------------------------------------------------------------------*/
extern INT32 x_svl_close
(
    HANDLE_T                  h_svl
) 
{ 
    INT32           i4_rc;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        handle_free(h_svl,TRUE);
        i4_rc=SVLR_OK;
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} close handle [%d] \n",h_svl));
    }
    else
    {
        DBG_ERROR(("{SVL} close handle [%d] failed.\n",h_svl));
    }

    SVL_API_LOG("{svl}%s(0x%x)=%d\n",__FUNCTION__,h_svl,i4_rc);
    return i4_rc;      
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_handle_info
 *
 * Description: This function gets the SVL ID and SVL name for the 
 *              specified handle.    
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object.
 *
 * Outputs: 
 *   pui2_svl_id  Pointer to SVL ID variable.  On return it contains 
 *                the SVL ID value for this handle.
 *   ps_svl_name  Pointer to a char array variable (the size of this char 
 *                array should be at least MAX_SVL_NAME_LEN+1).  On return, 
 *                this array contains the name of the SVL object.
 * Returns: 
 *   SVLR_OK           Information about this handle (svl id and svl name)
 *                     is returned.
 *   SVLR_INV_HANDLE   Bad handle.
 *   SVLR_INV_ARG      'pui2_svl_id' or 'ps_svl_name' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_get_handle_info
(
    HANDLE_T		h_svl,
    UINT16*			pui2_svl_id,
    CHAR*			ps_svl_name
) 
{ 
    INT32           i4_rc;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pui2_svl_id != NULL && ps_svl_name != NULL )
        {
            *pui2_svl_id = pt_svl->ui2_svl_id;
            x_strcpy(ps_svl_name, pt_svl->ps_svl_name);
            i4_rc=SVLR_OK;
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;     
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_rec
 *
 * Description: This API gets a specific record stored in the SVL database 
 *              object.    The search parameter is specified by the 
 *              'ui2_svl_rec_id' value.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_svl_rec_id  Specifies the service record id.
 *
 * Outputs: 
 *   pt_svl_rec Pointer to a service record structure.   This structure must 
 *              be allocated by the caller.  On return, it is filled with 
 *              service record information.
 *   pui4_ver_id Pointer to a variable holding version id of the database 
 *              object.   Note: version id of the SVL database object is 
 *              incremented each time when a modification takes place.
 * Returns: 
 *   SVLR_OK    Data from the specified record is returned. 
 *   SVLR_INV_HANDLE    Bad handle.
 *   SVLR_REC_NOT_FOUND No record matching the search parameter 
 *                      ('ui2_svl_rec_id')  is found.
 *    SVLR_INV_ARG       'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_get_rec
(
    HANDLE_T            h_svl,
    UINT16			    ui2_svl_rec_id,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id
)
{
    INT32               i4_rc;
    SVL_OBJ_T*          pt_svl;
    HANDLE_TYPE_T       pe_type;
    
    UINT8               ui1_key_count;
    DB_FIELD_T          aui4_keyfld_type[1];
    const VOID*         apv_keyfld_addr[1];
    
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pui4_ver_id    != NULL &&
             pt_svl_rec     != NULL &&
             ui2_svl_rec_id != SVL_NULL_REC_ID   )
        {
            /* Set up arguments to read record by record_id value. */
            ui1_key_count = 1;
            aui4_keyfld_type[0]      = (DB_FIELD_T) SVL_CDB_TYPE_REC_ID ;
            apv_keyfld_addr[0]       = &ui2_svl_rec_id;   
            
            i4_rc=svl_get_rec_by_flds(pt_svl,
                                      ui1_key_count,
                                      aui4_keyfld_type,
                                      apv_keyfld_addr,
                                      NULL,
                                      pt_svl_rec,
                                      0,
                                      pui4_ver_id);             
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: cmp_fct_src_id
 *
 * Description: This function interpret the union field in SVL_REC_T
 *              structure. It compares the source id value specifies
 *              by the caller with the source id value store in the
 *              SVL database object.  If no source id value
 *              match, the it returns 0, else it returns 1.
 *
 * Inputs: 
 *   pv_rec_fld_A: address containing the field value specified by the caller.
 *
 *   pv_rec_fld_B: address to the field record (note: this address is not
 *                 guarantee to be aligned on word boundary.
 *                  
 *   e_union_tag:  Union tag value of the record field to be compared.
 *                
 *   ui4_size:     Size of the field
 *                
 *   e_opcode: call for comparing the user specified fiedl value and field
 *             stored in SVL record.
 *         
 *   pv_tag:   tag value provided by the caller.
 *
 * Outputs: None
 *   
 * Returns: 
 *   0            source id value is matched.
 *   1            source id value is not matched.
 *   
 -----------------------------------------------------------------------*/

static INT32 cmp_fct_src_id
(
    const VOID*        pv_rec_fld_A, 
    const VOID*        pv_rec_fld_B,
    DB_UNION_TAG_T     e_union_tag,
    UINT32             ui4_size, 
    DB_CMP_OPCODE_T    e_opcode,
    VOID*              pv_tag
)
{
    INT32        i4_rc;
    UINT16       ui2_src_id_to_find;
    UINT16       ui2_src_id_in_rec;

    /*
      if return value is 0, then the record matches with the
      search parameters.  Set the return value to negative
      match first.
    */

    /*
       No match.
    */
    i4_rc = 1;

    /*
      Compare for source id in this record field only if the
      union tag field associated with this record field
      is ATSC or SCTE.
      
      Note: the union tag field contain the broadcast type
      information.
    */
    if ( e_union_tag == BRDCST_TYPE_ATSC ||
         e_union_tag == BRDCST_TYPE_SCTE ||
         e_union_tag == BRDCST_TYPE_ANALOG
        )
    {
        if ( e_union_tag == BRDCST_TYPE_ANALOG )
        {
            UINT8*   pui1_src_B;
            BOOL     b_src_valid_flag;
            
            pui1_src_B  = (UINT8*) pv_rec_fld_B ;

            /*
              Need to skip the first 4 byte in the Analog union structure,
              because the first 4 bytes is stream component field.
            */
            pui1_src_B += 4;
            
            /*
               Note; the address to pv_rec_fld_A and pv_rec_fld_B is not
               guarantee to be word aligned, therefore we need to copy
               the content to local variables before making comparison.
        
               copy the contents of the data address to local variables.
            */
            x_memcpy(&ui2_src_id_to_find, pv_rec_fld_A, 2);
            x_memcpy(&ui2_src_id_in_rec,  pui1_src_B, 2);

            pui1_src_B += 2;

            /*
              Get the source id validity flag.
            */
            x_memcpy(&b_src_valid_flag, pui1_src_B, 1);
            
            if ( b_src_valid_flag != TRUE )
            {
                /*
                  No valid source id contain in this record,
                  so return no match.
                */
                return i4_rc;
            }   
        }
        else
        {
            /*
               Note; the address to pv_rec_fld_A and pv_rec_fld_B is not
               guarantee to be word aligned, therefore we need to copy
               the content to local variables before making comparison.
        
               copy the contents of the data address to local variables.
            */
            x_memcpy(&ui2_src_id_to_find, pv_rec_fld_A, 2);
            x_memcpy(&ui2_src_id_in_rec,  pv_rec_fld_B, 2);
        }
        
        
        if ( e_opcode == DB_OPS_EQUAL )
        {
            if ( ui2_src_id_to_find == ui2_src_id_in_rec )
            {
                i4_rc = 0;
            }
        }
    }
    else
    {
       /*
		 The union tag value of the key and record field
         don't match.
       */
       i4_rc = 1;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_rec_by_rc_key
 *
 * Description: This API returns service record entry with a specified 
 *              remote_control_key_id. Only used for ISDB protocol
 *
 * Inputs: 
 *   h_svl          Handle to the SVL object.
 *   ui1_rc_key_id  Specifies the remote_control_key_id
 *
 * Outputs: None
 *   pt_svl_rec	  Pointer to a SVL record structure.
 *
 * Input/Output arguments
 *   pui4_ver_id  Pointer to a variable holding version id of the SVL 
 *                database object.  On entry, this variable contains 
 *                the version id of the database that the caller expects.  
 *                On the return, this variable contains the current version 
 *                id of the database.   If the expected version id and 
 *                current database version id do not match, an error status 
 *                code is returned.  If 'pui4_ver_id' is set to 
 *                'SVL_NULL_VER_ID' value, then no checking is done and 
 *                the latest version id is returned the application. 
 * Returns: 
 *   SVLR_OK             A SVL_REC_T structure is returned. 
 *   SVLR_INV_HANDLE     Bad 'h_svl' handle.
 *   SVLR_REC_NOT_FOUND  Specified record is not found.
 *   SVLR_INVALID_ARG    'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *   SVLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id' 
 *                       pass into the API and the internal database version 
 *                       id is not the same.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_get_rec_by_rc_key
(
    HANDLE_T           h_svl,
    UINT8              ui1_rc_key,
    SVL_REC_T*         pt_svl_rec,
    UINT32*            pui4_ver_id
)
{
    UINT16          ui2_idx;
    SVL_REC_T       t_svl_rec;
    UINT16          ui2_svl_rec_count = 0;

    if (ui1_rc_key < 1 || ui1_rc_key > 12)
    {
        return SVLR_FAIL;
    }
    
    if (0 != x_svl_get_num_rec_by_brdcst_type(h_svl,
                    BRDCST_TYPE_ISDB,
                    0xFFFFFFFF,
                    &ui2_svl_rec_count,
                    pui4_ver_id))
    {
        return SVLR_NOT_FOUND;
    }
    
    if (0 == ui2_svl_rec_count)
    {
        return SVLR_NOT_FOUND;    
    }
    
    for (ui2_idx=0; ui2_idx<ui2_svl_rec_count; ui2_idx++)
    {
        if (SVLR_OK != x_svl_get_rec_by_brdcst_type(h_svl,
                    BRDCST_TYPE_ISDB,
                    0xFFFFFFFF,
                    ui2_idx,
                    &t_svl_rec,
                    pui4_ver_id))
        {
            return SVLR_NOT_FOUND;
        }
        if (t_svl_rec.u_data.t_isdb.ui1_reg_key == ui1_rc_key)
        {
            *pt_svl_rec = t_svl_rec;
            return SVLR_OK;
        }
    }
    
    return SVLR_FAIL;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_rec_by_src_id
 *
 * Description: This API returns service record entry with a specified 
 *              source id value and network mask parameter from the SVL 
 *              database object.  If there are multiple service records 
 *              matching the specified channel number, the caller can use 
 *              'ui2_idx' to iterate over service records with the same 
 *              channel number.    
 *
 * Inputs: 
 *   h_svl            Handle to the SVL object.
 *   ui2_source_id  Specifies the source id of the service records 
 *                  to be returned.
 *   ui4_nw_mask  32-bits mask specifying the network(s) of the service 
 *                record to be returned.  
 *   ui2_idx      Index for iterating the SVL for service records matching 
 *                the specified channel number.  0 means the first record
 *                found in SVL list that matches the specified channel 
 *                number and network mask parameter, and 1 is the next record
 *                found, and so on.
 *
 * Outputs: None
 *   pt_svl_rec	  Pointer to a SVL record structure.
 *
 * Input/Output arguments
 *   pui4_ver_id  Pointer to a variable holding version id of the SVL 
 *                database object.  On entry, this variable contains 
 *                the version id of the database that the caller expects.  
 *                On the return, this variable contains the current version 
 *                id of the database.   If the expected version id and 
 *                current database version id do not match, an error status 
 *                code is returned.  If 'pui4_ver_id' is set to 
 *                'SVL_NULL_VER_ID' value, then no checking is done and 
 *                the latest version id is returned the application. 
 * Returns: 
 *   SVLR_OK             A SVL_REC_T structure is returned. 
 *   SVLR_INV_HANDLE     Bad 'h_svl' handle.
 *   SVLR_REC_NOT_FOUND  Specified record is not found.
 *   SVLR_INVALID_ARG    'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *   SVLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id' 
 *                       pass into the API and the internal database version 
 *                       id is not the same.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_get_rec_by_src_id
(
    HANDLE_T           h_svl,
    UINT16             ui2_source_id,
    UINT32		       ui4_nw_mask,
    UINT16		       ui2_idx,
    SVL_REC_T*         pt_svl_rec,
    UINT32*            pui4_ver_id
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    
    UINT8               ui1_key_count;
    DB_FIELD_T          aui4_keyfld_type[2];
    const VOID*         apv_keyfld_addr[2];
    DB_SEARCH_T         ae_cmp_param[2];
    
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_svl_rec     != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            ui1_key_count = 2;

            aui4_keyfld_type[0]       = (DB_FIELD_T) SVL_CDB_TYPE_DATA ;
            apv_keyfld_addr[0]        = &ui2_source_id;
            ae_cmp_param[0].e_opcode  = DB_OPS_EQUAL;
            ae_cmp_param[0].e_tag_val = BRDCST_TYPE_ATSC | BRDCST_TYPE_SCTE
                | BRDCST_TYPE_ANALOG ;
            ae_cmp_param[0].pf_fct    = cmp_fct_src_id;
            

            aui4_keyfld_type[1]      = (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID ;
            apv_keyfld_addr[1]       = &ui4_nw_mask;
            ae_cmp_param[1].e_opcode = DB_OPS_BITWISE_AND;
            
            i4_rc=svl_get_rec_by_flds(pt_svl,
                                      ui1_key_count,
                                      aui4_keyfld_type,
                                      apv_keyfld_addr,
                                      ae_cmp_param,
                                      pt_svl_rec,
                                      ui2_idx,
                                      pui4_ver_id);            
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_rec_by_channel
 *
 * Description: This API returns service record entry with a specified 
 *              channel number and network mask parameter from the SVL 
 *              database object.  If there are multiple service records 
 *              matching the specified channel number, the caller can use 
 *              'ui2_idx' to iterate over service records with the same 
 *              channel number.    
 *
 * Inputs: 
 *   h_svl            Handle to the SVL object.
 *   ui2_channel_num  Specifies the channel number of the service records 
 *                    to be returned.
 *   ui4_nw_mask  32-bits mask specifying the network(s) of the service 
 *                record to be returned.  
 *   ui2_idx      Index for iterating the SVL for service records matching 
 *                the specified channel number.  0 means the first record
 *                found in SVL list that matches the specified channel 
 *                number and network mask parameter, and 1 is the next record
 *                found, and so on.
 *
 * Outputs: None
 *   pt_svl_rec	  Pointer to a SVL record structure.
 *
 * Input/Output arguments
 *   pui4_ver_id  Pointer to a variable holding version id of the SVL 
 *                database object.  On entry, this variable contains 
 *                the version id of the database that the caller expects.  
 *                On the return, this variable contains the current version 
 *                id of the database.   If the expected version id and 
 *                current database version id do not match, an error status 
 *                code is returned.  If 'pui4_ver_id' is set to 
 *                'SVL_NULL_VER_ID' value, then no checking is done and 
 *                the latest version id is returned the application. 
 * Returns: 
 *   SVLR_OK             A SVL_REC_T structure is returned. 
 *   SVLR_INV_HANDLE     Bad 'h_svl' handle.
 *   SVLR_REC_NOT_FOUND  Specified record is not found.
 *   SVLR_INVALID_ARG    'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *   SVLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id' 
 *                       pass into the API and the internal database version 
 *                       id is not the same.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_get_rec_by_channel
(
    HANDLE_T			h_svl,
    UINT32			    ui4_channel_num,
    UINT32			    ui4_nw_mask,
    UINT16			    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    
    UINT8               ui1_key_count;
    DB_FIELD_T          aui4_keyfld_type[2];
    const VOID*         apv_keyfld_addr[2];
    DB_SEARCH_T         ae_cmp_param[2];
    
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_svl_rec     != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            ui1_key_count = 2;

            aui4_keyfld_type[0]      = (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID ;
            apv_keyfld_addr[0]       = &ui4_channel_num;
            ae_cmp_param[0].e_opcode = DB_OPS_EQUAL;

            aui4_keyfld_type[1]      = (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID ;
            apv_keyfld_addr[1]       = &ui4_nw_mask;
            ae_cmp_param[1].e_opcode = DB_OPS_BITWISE_AND;
            
            i4_rc=svl_get_rec_by_flds(pt_svl,
                                      ui1_key_count,
                                      aui4_keyfld_type,
                                      apv_keyfld_addr,
                                      ae_cmp_param,
                                      pt_svl_rec,
                                      ui2_idx,
                                      pui4_ver_id);            
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_rec_by_brdcst_type
 *
 * Description: This function returns service record with a specified
 *              broadcast type (e.g., BRDCST_TYPE_ANALOG, BRDCST_TYPE_ATSC,
 *              or BRDCST_TYPE_DVB) and network.  If there are multiple
 *              service records matching the specified broadcast type
 *              (see 9.3.10, x_svl_get_num_rec_by_brdcst_type), the client
 *              can use ui2_idx to iterate over service records with the same
 *              broadcast type.
 * 
 *              If the specified broadcast type is BRDCST_TYPE_UNKNOWN, then
 *              all the service records for the specified networks are matched.
 *
 *              Note: the returned records will be sorted by channel number
 *              (ascending order).  For ATSC broadcast type, the returned
 *              records will be sorted by major number and minor number, in
 *              that order (see section 4.2.1 on specifying the channel number
 *              field in the service list record for ATSC broadcast).
 *
 *              If another caller locks (e.g., to perform write operation)
 *              the SVL object reference by the 'h_svl' handle, then this
 *              function will block until the other caller has unlock the
 *              SVL object.
 *
 * Inputs: 
 *   h_svl	        Handle to the SVL object.
 *   e_brdcst_type	Specifies the broadcast type (ANALOG, DVB, or ATSC) of
 *                  the service records to be returned.
 *   ui4_nw_mask    32-bits mask specifying the network(s) of the service
 *                  record to be returned.
 *                  The 'ui4_nw_mask' value is compared with the service
 *                  record's network value using bit-wise AND operation.
 *                  The comparison is considered a positive match if the
 *                  result is greater than 0.  For example, if SATELLITE,
 *                  CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                  0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04 will
 *                  match service records with internal network mask values
 *                  of 0x04, 0x06, and 0x07.    
 *                  
 *  ui2_idx         Index for iterating the SVL for service records matching
 *                  the specified broadcast type.  0 means the first record
 *                  found that matches the specified broadcast type and
 *                  network mask parameter, and 1 is the next record found,
 *                  and so on.	
 *
 * Outputs: 
 *  pt_svl_rec	    Pointer to a SVL record structure.
 *
 * Input/Output
 *  pui4_ver_id     Pointer to a variable holding version id of the SVL
 *                  database object.  On entry, this variable contains the
 *                  version id of the database that the caller expects.
 *                  On the return, this variable contains the current version
 *                  id of the database.   If the expected version id and
 *                  current database version id do not match, an error status
 *                  code is returned.  If 'pui4_ver_id' is set to
 *                  'SVL_NULL_VER_ID' value, then no checking is done and
 *                  the latest version id is returned the application.
 *
 * Returns: 
 *   SVLR_OK             A SVL_REC_T structure is returned. 
 *
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *
 *   SVLR_REC_NOT_FOUND  Specified record is not found.
 *
 *   SVLR_INV_ARG        'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *
 *   SVLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id'
 *                       pass into the API and the internal database version
 *                       id is not the same.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_get_rec_by_brdcst_type
(
    HANDLE_T			h_svl,
    BRDCST_TYPE_T		e_brdcst_type,
    UINT32			    ui4_nw_mask,
    UINT16			    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    SVL_SEARCH_PARM_T   t_parm;
 
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_svl_rec     != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            
            t_parm.ui4_fld_type = (DB_FIELD_T) SVL_CDB_TYPE_BRDCST_T ;
            t_parm.u.t_brdcst.e_brdcst_type   = e_brdcst_type;
            t_parm.u.t_brdcst.ui4_nw_mask     = ui4_nw_mask;
            t_parm.u.t_brdcst.ui2_idx         = ui2_idx;
            
            i4_rc=svl_read_rec_from_sort_list(pt_svl,
                                              &t_parm,
                                              ui2_idx,
                                              pt_svl_rec,
                                              pui4_ver_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    
    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} get_rec_by_brdcst_type [type: %d] [nw_mask: 0x%x] [idx: %d] [ver_id: %d] [svl_rec_id: %d] \n",e_brdcst_type, ui4_nw_mask, ui2_idx, *pui4_ver_id, pt_svl_rec->ui2_svl_rec_id));
    }
    else
    {
        DBG_ERROR(("{SVL} get_rec_by_brdcst_type [type: %d] failed.\n",e_brdcst_type));
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_rec_by_ts
 *
 * Description: This function searches the SVL and returns service record
 *              with a specified Transport Stream List ID, Transport Stream
 *              record ID (e.g., TSL ID and TSL record ID) and network mask
 *              parameter.  If there are multiple service records matching
 *              the specified TSL ID, TSL record ID, and network mask, the
 *              caller can use ui2_idx to iterate over service records with
 *              the positive match.
 * 
 *              Note: the combination of TSL ID and TSL record ID uniquely
 *              identifies a transport stream record within the Middleware.
 *
 *              If another caller locks (e.g., to perform write operation)
 *              the SVL object reference by the 'h_svl' handle, then this
 *              function will block until the other caller has unlock the
 *              SVL object.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_tsl_id	     Specifies the Transport Stream List ID value.
 *   ui2_tsl_rec_id  Specifies the Transport Stream record ID value. 
 *   ui4_nw_mask     32-bits mask specifying the network(s) of the service
 *                   record to be returned.  
 *                   The 'ui4_nw_mask' value is compared with the service
 *                   record's network value using bit-wise AND operation.
 *                   The comparison is considered a positive match if the
 *                   result is greater than 0.  For example, if SATELLITE,
 *                   CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                   0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04
 *                   will match service records with internal network mask
 *                   values of 0x04, 0x06, and 0x07.    
 *                   
 *   ui2_idx         Index for iterating the SVL for service records matching
 *                   the specified broadcast type.  0 means the first record
 *                   found that matches the specified broadcast type and
 *                   network mask parameter, and 1 is the next record found,
 *                   and so on.	  
 *
 * Outputs: 
 *   pt_svl_rec	     Pointer to a SVL record structure.
 *
 * Input/Output
 *   pui4_ver_id     Pointer to a variable holding version id of the SVL
 *                   database object.  On entry, this variable contains the
 *                   version id of the database that the caller expects.
 *                   On the return, this variable contains the current
 *                   version id of the database.   If the expected version id
 *                   and current database version id do not match, an error
 *                   status code is returned.  If 'pui4_ver_id' is set to
 *                   'SVL_NULL_VER_ID' value, then no checking is done and
 *                   the latest version id is returned the application.
 * Returns: 
 *   SVLR_OK             A SVL_REC_T structure is returned. 
 *
 *   SVLR_INV_HANDLE     Bad 'h_svl' handle.
 *
 *   SVLR_REC_NOT_FOUND  Specified record is not found.
 *
 *   SVLR_INV_ARG        'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *
 *   SVLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id'
 *                       pass into the API and the internal database version
 *                       id is not the same.
 -----------------------------------------------------------------------*/
INT32 x_svl_get_rec_by_ts
(
    HANDLE_T			h_svl,
    UINT16			    ui2_tsl_id,
    UINT16			    ui2_tsl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16		  	    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    SVL_SEARCH_PARM_T   t_parm;
 
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_svl_rec     != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            t_parm.ui4_fld_type = (DB_FIELD_T) SVL_CDB_TYPE_TSL_ID ;
            t_parm.u.t_ts.ui2_tsl_id      = ui2_tsl_id;
            t_parm.u.t_ts.ui2_tsl_rec_id  = ui2_tsl_rec_id;
            t_parm.u.t_ts.ui4_nw_mask     = ui4_nw_mask;
            t_parm.u.t_ts.ui2_idx         = ui2_idx;
            
            i4_rc=svl_read_rec_from_sort_list(pt_svl,
                                              &t_parm,
                                              ui2_idx,
                                              pt_svl_rec,
                                              pui4_ver_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_rec_by_nw
 *
 * Description: This function searches the SVL and returns service record
 *              with a specified Network List ID, Network record ID (e.g.,
 *              NWL ID and NWL record ID) and network mask parameter.  If
 *              there are multiple service records matching the specified
 *              NWL ID, NWL record ID, and network mask, the caller can use
 *              ui2_idx to iterate over service records with the positive
 *              match.
 * 
 *              Note: the combination of NWL ID and NWL record ID uniquely
 *              identifies a network record within the Middleware.
 *
 *              If another caller locks (e.g., to perform write operation)
 *              the SVL object reference by the 'h_svl' handle, then this
 *              function will block until the other caller has unlock the
 *              SVL object.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_nwl_id	     Specifies the Network List ID value.
 *   ui2_nwl_rec_id  Specifies the Network record ID value. 
 *   ui4_nw_mask     32-bits mask specifying the network(s) of the service
 *                   record to be returned.  
 *                   The 'ui4_nw_mask' value is compared with the service
 *                   record's network value using bit-wise AND operation.
 *                   The comparison is considered a positive match if the
 *                   result is greater than 0.  For example, if SATELLITE,
 *                   CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                   0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04
 *                   will match service records with internal network mask
 *                   values of 0x04, 0x06, and 0x07.    
 *                   
 *   ui2_idx         Index for iterating the SVL for service records matching
 *                   the specified broadcast type.  0 means the first record
 *                   found that matches the specified broadcast type and
 *                   network mask parameter, and 1 is the next record found,
 *                   and so on.	  
 *
 * Outputs: 
 *   pt_svl_rec	     Pointer to a SVL record structure.
 *
 * Input/Output
 *   pui4_ver_id     Pointer to a variable holding version id of the SVL
 *                   database object.  On entry, this variable contains the
 *                   version id of the database that the caller expects.
 *                   On the return, this variable contains the current
 *                   version id of the database.   If the expected version id
 *                   and current database version id do not match, an error
 *                   status code is returned.  If 'pui4_ver_id' is set to
 *                   'SVL_NULL_VER_ID' value, then no checking is done and
 *                   the latest version id is returned the application.
 * Returns: 
 *   SVLR_OK             A SVL_REC_T structure is returned. 
 *
 *   SVLR_INV_HANDLE     Bad 'h_svl' handle.
 *
 *   SVLR_REC_NOT_FOUND  Specified record is not found.
 *
 *   SVLR_INV_ARG        'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *
 *   SVLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id'
 *                       pass into the API and the internal database version
 *                       id is not the same.
 -----------------------------------------------------------------------*/
INT32 x_svl_get_rec_by_nw
(
    HANDLE_T			h_svl,
    UINT16			    ui2_nwl_id,
    UINT16			    ui2_nwl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16		  	    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    SVL_SEARCH_PARM_T   t_parm;
 
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_svl_rec     != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            t_parm.ui4_fld_type = (DB_FIELD_T) SVL_CDB_TYPE_NWL_ID ;
            t_parm.u.t_nw.ui2_nwl_id      = ui2_nwl_id;
            t_parm.u.t_nw.ui2_nwl_rec_id  = ui2_nwl_rec_id;
            t_parm.u.t_nw.ui4_nw_mask     = ui4_nw_mask;
            t_parm.u.t_nw.ui2_idx         = ui2_idx;
            
            i4_rc=svl_read_rec_from_sort_list(pt_svl,
                                              &t_parm,
                                              ui2_idx,
                                              pt_svl_rec,
                                              pui4_ver_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_rec_by_sat
 *
 * Description: This function searches the SVL and returns service record
 *              with a specified Satellite List ID, Satellite record ID
 *              (e.g., SATL ID and SATL record ID) and network mask
 *              parameter.  If there are multiple service records matching
 *              the specified SATL ID, SATL record ID, and network mask, the
 *              caller can use ui2_idx to iterate over service records with
 *              the positive match.
 * 
 *              Note: the combination of SATL ID and SATL record ID uniquely
 *              identifies a transport stream record within the Middleware.
 *
 *              If another caller locks (e.g., to perform write operation)
 *              the SVL object reference by the 'h_svl' handle, then this
 *              function will block until the other caller has unlock the
 *              SVL object.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_satl_id     Specifies the Satellite List ID value.
 *   ui2_satl_rec_id Specifies the Satellite record ID value. 
 *   ui4_nw_mask     32-bits mask specifying the network(s) of the service
 *                   record to be returned.  
 *                   The 'ui4_nw_mask' value is compared with the service
 *                   record's network value using bit-wise AND operation.
 *                   The comparison is considered a positive match if the
 *                   result is greater than 0.  For example, if SATELLITE,
 *                   CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                   0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04
 *                   will match service records with internal network mask
 *                   values of 0x04, 0x06, and 0x07.    
 *                   
 *   ui2_idx         Index for iterating the SVL for service records matching
 *                   the specified broadcast type.  0 means the first record
 *                   found that matches the specified broadcast type and
 *                   network mask parameter, and 1 is the next record found,
 *                   and so on.	  
 *
 * Outputs: 
 *   pt_svl_rec	     Pointer to a SVL record structure.
 *
 * Input/Output
 *   pui4_ver_id     Pointer to a variable holding version id of the SVL
 *                   database object.  On entry, this variable contains the
 *                   version id of the database that the caller expects.
 *                   On the return, this variable contains the current
 *                   version id of the database.   If the expected version id
 *                   and current database version id do not match, an error
 *                   status code is returned.  If 'pui4_ver_id' is set to
 *                   'SVL_NULL_VER_ID' value, then no checking is done and
 *                   the latest version id is returned the application.
 * Returns: 
 *   SVLR_OK             A SVL_REC_T structure is returned. 
 *
 *   SVLR_INV_HANDLE     Bad 'h_svl' handle.
 *
 *   SVLR_REC_NOT_FOUND  Specified record is not found.
 *
 *   SVLR_INV_ARG        'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *
 *   SVLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id'
 *                       pass into the API and the internal database version
 *                       id is not the same.
 -----------------------------------------------------------------------*/
INT32 x_svl_get_rec_by_sat
(
    HANDLE_T			h_svl,
    UINT16			    ui2_satl_id,
    UINT16			    ui2_satl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16		  	    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    SVL_SEARCH_PARM_T   t_parm;
 
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_svl_rec     != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            t_parm.ui4_fld_type = (DB_FIELD_T) SVL_CDB_TYPE_SATL_ID ;
            t_parm.u.t_sat.ui2_satl_id     = ui2_satl_id;
            t_parm.u.t_sat.ui2_satl_rec_id = ui2_satl_rec_id;
            t_parm.u.t_sat.ui4_nw_mask     = ui4_nw_mask;
            t_parm.u.t_sat.ui2_idx         = ui2_idx;
            
            i4_rc=svl_read_rec_from_sort_list(pt_svl,
                                              &t_parm,
                                              ui2_idx,
                                              pt_svl_rec,
                                              pui4_ver_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_num_rec_by_channel
 *
 * Description: Return the number of SVL records matching the channel
 *              and network mask values.
 *
 * Inputs: 
 *   h_svl	          Handle to the SVL object.
 *
 *   ui4_channel_num  Specifies the channel number.
 *
 *   ui4_nw_mask      32-bits mask specifying the network(s) of the
 *                    service record to be counted.  
 *                    The 'ui4_nw_mask' value is compared with the service
 *                    record's network value using bit-wise AND operation.
 *                    The comparison is considered a positive match if the
 *                    result is greater than 0.  For example, if SATELLITE,
 *                    CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                    0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04
 *                    will match service records with internal network mask
 *                    values of 0x04, 0x06, and 0x07.    
 *
 * Outputs: 
 *   pui2_num_recs:    number of records.
 *   pui4_ver_id:      version id.
 *
 * Returns: 
 *   SVLR_OK           Number of records found. 
 *
 *   SVLR_INV_HANDLE   Bad 'h_svl' handle.
 *
 *   SVLR_INV_ARG      'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_get_num_rec_by_channel
(
    HANDLE_T			h_svl,
    UINT32			    ui4_channel_num,
    UINT32			    ui4_nw_mask,
    UINT16*			    pui2_num_recs,
    UINT32*			    pui4_ver_id
)
{
    INT32   i4_rc;
    UINT8           ui1_key_count;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;
    DB_FIELD_T      aui4_keyfld_type[2];
    const VOID*     apv_keyfld_addr[2];
    DB_SEARCH_T     ae_cmp_param[2];
 
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pui2_num_recs  != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            ui1_key_count = 2;

            aui4_keyfld_type[0]      = (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID ;
            apv_keyfld_addr[0]       = &ui4_channel_num;
            ae_cmp_param[0].e_opcode = DB_OPS_EQUAL;

            aui4_keyfld_type[1]      = (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID ;
            apv_keyfld_addr[1]       = &ui4_nw_mask;
            ae_cmp_param[1].e_opcode = DB_OPS_BITWISE_AND;
            
            i4_rc=svl_get_num_rec_by_flds(pt_svl,
                                          ui1_key_count,
                                          aui4_keyfld_type,
                                          apv_keyfld_addr,
                                          ae_cmp_param,
                                          pui2_num_recs,
                                          pui4_ver_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_num_rec_by_brdcst_type
 *
 * Description: Return the number of SVL records matching the broadcast
 *              type ((e.g., BRDCST_TYPE_ANALOG, BRDCST_TYPE_ATSC, or
 *              BRDCST_TYPE_DVB) and network mask values.
 *
 *              If specified broadcast type is BRDCST_TYPE_UNKNOWN, then it
 *              matches any broadcast types.
 *
 *              To get the number of all the records in the SVL, caller can
 *              specifies BRDCST_TYPE_UNKNOWN and 'network mask' parameter
 *              of 0xFFFF.
 * Inputs: 
 *   h_svl	        Handle to the SVL object.
 *   e_brdcstl_type	Specifies the broadcast type (e.g., BRDCST_TYPE_ANALOG,
 *                  BRDCST_TYPE_ATSC, BRDCST_TYPE_DVB, or BRDCST_TYPE_UNKNOWN)
 *                  of the service record to be counted.
 *
 *   ui4_nw_mask    32-bits mask specifying the network(s) of the service
 *                  record to be counted.  
 *                  The 'ui4_nw_mask' value is compared with the service
 *                  record's network value using bit-wise AND operation.
 *                  The comparison is considered a positive match if the
 *                  result is greater than 0.  For example, if SATELLITE,
 *                  CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                  0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04 will
 *                  match service records with internal network mask values
 *                  of 0x04, 0x06, and 0x07.
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records matching
 *                  the specified stream type in the SVL. 	
 *
 *   pui4_ver_id    Pointer to a variable holding version id of the SVL
 *                  database object.
 * Returns: 
 *   SVLR_OK          Number of records found is returned.
 *   SVLR_INV_HANDLE  Bad 'h_svl' handle.
 *   SVLR_INV_ARG     'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_svl_get_num_rec_by_brdcst_type
(
    HANDLE_T			h_svl,
    BRDCST_TYPE_T		e_brdcst_type,
    UINT32			    ui4_nw_mask,
    UINT16*			    pui2_num_recs,
    UINT32*			    pui4_ver_id
)
{
    INT32   i4_rc;
    UINT8           ui1_key_count;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;
    DB_FIELD_T      aui4_keyfld_type[2];
    const VOID*     apv_keyfld_addr[2];
    DB_SEARCH_T     ae_cmp_param[2];

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pui2_num_recs  != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.  If the BRDCST type is unknown, then
               only the network mask field is evaluated.
            */
			   if ( e_brdcst_type == BRDCST_TYPE_UNKNOWN )
            {
                ui1_key_count = 1;
                aui4_keyfld_type[0]      = (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID ;
                apv_keyfld_addr[0]       = &ui4_nw_mask;
                ae_cmp_param[0].e_opcode = DB_OPS_BITWISE_AND;
				}
            else
				{
                ui1_key_count = 2;
				    aui4_keyfld_type[0]      = (DB_FIELD_T) SVL_CDB_TYPE_BRDCST_T ;
                apv_keyfld_addr[0]       = &e_brdcst_type;
                ae_cmp_param[0].e_opcode = DB_OPS_EQUAL;

                aui4_keyfld_type[1]      = (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID ;
                apv_keyfld_addr[1]       = &ui4_nw_mask;
                ae_cmp_param[1].e_opcode = DB_OPS_BITWISE_AND;
				}
            
            i4_rc=svl_get_num_rec_by_flds(pt_svl,
                                          ui1_key_count,
                                          aui4_keyfld_type,
                                          apv_keyfld_addr,
                                          ae_cmp_param,
                                          pui2_num_recs,
                                          pui4_ver_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_num_rec_by_ts
 *
 * Description: This function returns the number of records for the
 *              specified Transport Stream List ID, Transport Stream record
 *              ID (e.g., TSL ID and TSL record ID), and network mask
 *              parameter.
 *              Note: the combination of TSL ID and TSL record ID uniquely
 *              identifies a transport stream record within the Middleware.
 *
 * Inputs: 
 *   h_svl	    Handle to the SVL object.
 *
 *   ui2_tsl_id Specifies the Transport Stream List ID value.
 *   ui2_tsl_rec_id	Specifies the Transport Stream record ID value.  
 *
 *   ui4_nw_mask    32-bits mask specifying the network(s) of the service
 *                  record to be counted.  
 *                  The 'ui4_nw_mask' value is compared with the service
 *                  record's network value using bit-wise AND operation.
 *                  The comparison is considered a positive match if the
 *                  result is greater than 0.  For example, if SATELLITE,
 *                  CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                  0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04 will
 *                  match service records with internal network mask values
 *                  of 0x04, 0x06, and 0x07.    
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records
 *                  matching the specified stream type in the SVL. 	
 *   pui4_ver_id    Pointer to a variable holding version id of the SVL
 *                  database object. 	
 * Returns: 
 *   SVLR_OK         Number of records found.
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_get_num_rec_by_ts
(
    HANDLE_T			h_svl,
    UINT16              ui2_tsl_id,
    UINT16              ui2_tsl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
)
{
    INT32   i4_rc;
    UINT8           ui1_key_count;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;
    DB_FIELD_T      aui4_keyfld_type[3];
    const VOID*     apv_keyfld_addr[3];
    DB_SEARCH_T     ae_cmp_param[3];
 
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pui2_num_recs  != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            ui1_key_count = 3;

            aui4_keyfld_type[0]      = (DB_FIELD_T) SVL_CDB_TYPE_TSL_ID ;
            apv_keyfld_addr[0]       = &ui2_tsl_id;
            ae_cmp_param[0].e_opcode = DB_OPS_EQUAL;

            aui4_keyfld_type[1]      = (DB_FIELD_T) SVL_CDB_TYPE_TSL_REC_ID ;
            apv_keyfld_addr[1]       = &ui2_tsl_rec_id;
            ae_cmp_param[1].e_opcode = DB_OPS_EQUAL;

            aui4_keyfld_type[2]      = (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID ;
            apv_keyfld_addr[2]       = &ui4_nw_mask;
            ae_cmp_param[2].e_opcode = DB_OPS_BITWISE_AND;
            
            i4_rc=svl_get_num_rec_by_flds(pt_svl,
                                          ui1_key_count,
                                          aui4_keyfld_type,
                                          apv_keyfld_addr,
                                          ae_cmp_param,
                                          pui2_num_recs,
                                          pui4_ver_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_num_rec_by_nw
 *
 * Description: This function returns the number of records for the
 *              specified Network List ID, Network record ID (e.g., NWL ID
 *              and NWL record ID), and network mask parameter.
 *              Note: the combination of NWL ID and NWL record ID uniquely
 *              identifies a transport stream record within the Middleware.
 *
 * Inputs: 
 *   h_svl	    Handle to the SVL object.
 *
 *   ui2_nwl_id Specifies the Network List ID value.
 *   ui2_nwl_rec_id	Specifies the Network record ID value.  
 *
 *   ui4_nw_mask    32-bits mask specifying the network(s) of the service
 *                  record to be counted.  
 *                  The 'ui4_nw_mask' value is compared with the service
 *                  record's network value using bit-wise AND operation.
 *                  The comparison is considered a positive match if the
 *                  result is greater than 0.  For example, if SATELLITE,
 *                  CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                  0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04 will
 *                  match service records with internal network mask values
 *                  of 0x04, 0x06, and 0x07.    
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records
 *                  matching the specified stream type in the SVL. 	
 *   pui4_ver_id    Pointer to a variable holding version id of the SVL
 *                  database object. 	
 * Returns: 
 *   SVLR_OK         Number of records found.
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_get_num_rec_by_nw
(
    HANDLE_T			h_svl,
    UINT16              ui2_nwl_id,
    UINT16              ui2_nwl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
)
{
    INT32   i4_rc;
    UINT8           ui1_key_count;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;
    DB_FIELD_T      aui4_keyfld_type[3];
    const VOID*     apv_keyfld_addr[3];
    DB_SEARCH_T     ae_cmp_param[3];
 
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pui2_num_recs  != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            ui1_key_count = 3;

            aui4_keyfld_type[0]      = (DB_FIELD_T) SVL_CDB_TYPE_NWL_ID ;
            apv_keyfld_addr[0]       = &ui2_nwl_id;
            ae_cmp_param[0].e_opcode = DB_OPS_EQUAL;

            aui4_keyfld_type[1]      = (DB_FIELD_T) SVL_CDB_TYPE_NWL_REC_ID ;
            apv_keyfld_addr[1]       = &ui2_nwl_rec_id;
            ae_cmp_param[1].e_opcode = DB_OPS_EQUAL;

            aui4_keyfld_type[2]      = (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID ;
            apv_keyfld_addr[2]       = &ui4_nw_mask;
            ae_cmp_param[2].e_opcode = DB_OPS_BITWISE_AND;
            
            i4_rc=svl_get_num_rec_by_flds(pt_svl,
                                          ui1_key_count,
                                          aui4_keyfld_type,
                                          apv_keyfld_addr,
                                          ae_cmp_param,
                                          pui2_num_recs,
                                          pui4_ver_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_num_rec_by_sat
 *
 * Description: This function returns the number of records for the
 *              specified Satellite List ID, Satellite record ID (e.g., SATL
 *              ID and SATL record ID), and network mask parameter.
 *              Note: the combination of SATL ID and SATL record ID uniquely
 *              identifies a transport stream record within the Middleware.
 *
 * Inputs: 
 *   h_svl	    Handle to the SVL object.
 *
 *   ui2_satl_id        Specifies the Satellite List ID value.
 *   ui2_satl_rec_id	Specifies the Satellite record ID value.  
 *
 *   ui4_nw_mask    32-bits mask specifying the network(s) of the service
 *                  record to be counted.  
 *                  The 'ui4_nw_mask' value is compared with the service
 *                  record's network value using bit-wise AND operation.
 *                  The comparison is considered a positive match if the
 *                  result is greater than 0.  For example, if SATELLITE,
 *                  CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                  0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04 will
 *                  match service records with internal network mask values
 *                  of 0x04, 0x06, and 0x07.    
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records
 *                  matching the specified stream type in the SVL. 	
 *   pui4_ver_id    Pointer to a variable holding version id of the SVL
 *                  database object. 	
 * Returns: 
 *   SVLR_OK         Number of records found.
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_get_num_rec_by_sat
(
    HANDLE_T			h_svl,
    UINT16              ui2_satl_id,
    UINT16              ui2_satl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
)
{
    INT32   i4_rc;
    UINT8           ui1_key_count;
    SVL_OBJ_T*  	pt_svl;
    HANDLE_TYPE_T   pe_type;
    DB_FIELD_T      aui4_keyfld_type[3];
    const VOID*     apv_keyfld_addr[3];
    DB_SEARCH_T     ae_cmp_param[3];
 
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pui2_num_recs  != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            ui1_key_count = 3;

            aui4_keyfld_type[0]      = (DB_FIELD_T) SVL_CDB_TYPE_SATL_ID ;
            apv_keyfld_addr[0]       = &ui2_satl_id;
            ae_cmp_param[0].e_opcode = DB_OPS_EQUAL;

            aui4_keyfld_type[1]      = (DB_FIELD_T) SVL_CDB_TYPE_SATL_REC_ID ;
            apv_keyfld_addr[1]       = &ui2_satl_rec_id;
            ae_cmp_param[1].e_opcode = DB_OPS_EQUAL;

            aui4_keyfld_type[2]      = (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID ;
            apv_keyfld_addr[2]       = &ui4_nw_mask;
            ae_cmp_param[2].e_opcode = DB_OPS_BITWISE_AND;
            
            i4_rc=svl_get_num_rec_by_flds(pt_svl,
                                          ui1_key_count,
                                          aui4_keyfld_type,
                                          apv_keyfld_addr,
                                          ae_cmp_param,
                                          pui2_num_recs,
                                          pui4_ver_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: svl_get_total_num_recs
 *
 * Description: this function returns total records number from all svl ;
 *  
 * Inputs: 
 *    void
 *
 * Outputs: 
 *      
 
 * Returns: 
 *   total number            
 *   
 -----------------------------------------------------------------------*/

static UINT32 svl_get_total_num_recs(VOID)
{
    UINT16  ui2_idx;
    UINT16  ui2_svl_num;

    UINT16  ui2_svl_id;
    CHAR    s_svl_name[MAX_SVL_NAME_LEN + 1];
    HANDLE_T    h_svl;
    SVL_OBJ_T*  pt_svl;
    HANDLE_TYPE_T   pe_type;
    DB_INFO_T   t_db_info;
    UINT32      ui4_total_num = 0;
    
    ui2_svl_num = svl_get_max_obj_num();
    for(ui2_idx=0; ui2_idx < ui2_svl_num; ui2_idx++)
    {
        if ( svl_get_id_and_name_by_idx(ui2_idx, s_svl_name, &ui2_svl_id)== SVLR_OK )
        {
            if ( ui2_svl_id != SVL_NULL_ID )
            {
                h_svl = NULL_HANDLE;
                if(svl_open_object(ui2_svl_id,(const CHAR*)s_svl_name,NULL,NULL,&h_svl) == SVLR_OK)
                {
                    if ( (handle_get_type_obj(h_svl, &pe_type,((VOID**) &pt_svl)) == HR_OK) &&
                     (pe_type == SVLT_SERVICE_LIST)                                     &&
                     (pt_svl  != NULL)
                    )
                    {
                        if ( db_get_info(pt_svl->h_cdb, &t_db_info) == DBR_OK )
                        {
                            ui4_total_num += t_db_info.ui4_record_count;
                        }
                    }
                    handle_free(h_svl,TRUE);
                }
                
            }
        }
        
    }
    
    return ui4_total_num;
}
/*----------------------------------------------------------------------
 * Name: x_svl_get_total_num_recs
 *
 * Description: 
 *              If h_svl is NULL , this API returns total records number from all svl ;
 *              if h_svl is not NULL , this API returns total records number from the svl specified by h_svl.
 * Inputs: 
 *    h_svl    handle to svl object;
 *
 * Outputs: 
 *      
 
 * Returns: 
 *   total number            
 *   
 -----------------------------------------------------------------------*/


UINT32 x_svl_get_total_num_recs(HANDLE_T    h_svl)
{
    SVL_OBJ_T*  pt_svl;
    HANDLE_TYPE_T   pe_type;
    DB_INFO_T   t_db_info;
    
    if( NULL_HANDLE == h_svl)
    {
        return svl_get_total_num_recs();
    }



    if ( (handle_get_type_obj(h_svl, &pe_type,((VOID**) &pt_svl)) == HR_OK) &&
        (pe_type == SVLT_SERVICE_LIST)                                     &&
        (pt_svl  != NULL))
    {
        if ( db_get_info(pt_svl->h_cdb, &t_db_info) == DBR_OK )
        {
            return t_db_info.ui4_record_count;
        }
    }

    return 0;
}



/*----------------------------------------------------------------------
 * Name: x_svl_iterate_rec
 *
 * Description: This API iterates over service records in the SVL list.  
 *              It returns the next service record from a specified the 
 *              channel number of the current service record location.  
 *              The 'e_iter_dir' argument specifies direction of iteration, 
 *              e.g., the next record's channel numeric value is plus or 
 *              minus from the current record's channel numeric value.  
 *              Service records that do not match the network mask 
 *              specification will be skipped during the iteration.  
 *
 *              If the current record is at the end or beginning of the 
 *              service list, then 'CHANNEL_PLUS' or 'CHANNEL_MINUS' will 
 *              return a status to indicate that end-point is reached. It is 
 *              up the caller to decide if next service record should wrap 
 *              around to the beginning service record or to the end of the 
 *              service record.
 *
 *              Since the service record in the SVL is sorted by 'channel
 *              number', the iteration can be thought of as getting the next 
 *              or the previous channel.  For ATSC broadcast type, this 
 *              implies the returned records will be sorted by major number 
 *              and minor number, in that order.
 *
 *              Specifying 'CHANNEL_SMALLEST' returns the record with the 
 *              smallest channel number (regardless of the current service 
 *              record position), and similarly specifying 'CHANNEL_LARGEST' 
 *              returns the record with the largest channel number.
 *
 * Inputs: 
 *    h_svl     Handle to the SVL object.
 *    ui4_current_channel   Specifies the channel number of the current 
 *                          service record to begin for the search for 
 *                          the next service record.
 *    ui4_nw_mask   32-bits mask specifying the network(s) of the service 
 *                  record to be returned.  
 *
 *    e_iter_dir    Specifies the direction of the iteration.  The values are:
 *
 *                        SVL_CHANNEL_PLUS	
 *                        SVL_CHANNEL_MINUS
 *                        SVL_CHANNEL_SMALLEST
 *                        SVL_CHANNEL_LARGEST
 *
 *                  If SVL_CHANNEL_SMALLEST or SVL_CHANNEL_LARGEST is 
 *                  specified, then the record with the smallest (numerically)
 *                  channel value is returned, and vice versa.  If the 
 *                  iteration has reach an end point, i.e., the current record
 *                  is point to the first or last service record, then the 
 *                  current service record is returned.
 *
 * Outputs: 
 *    pt_svl_rec    Pointer to a SVL record structure.
 *
 * In/Output:
 *    pui4_ver_id   Pointer to a variable holding version id of the SVL
 *                  database object.  On entry, this variable contains the
 *                  version id of the database that the caller expects.  
 *                  On the return, this variable contains the current version 
 *                  id of the database.   If the expected version id and 
 *                  current database version id do not match, an error status 
 *                  code is returned.  If 'pui4_ver_id' is set to 
 *                  'SVL_NULL_VER_ID' value, then no checking is done and the 
 *                  latest version id is returned the application.
 * Returns: 
 *   SVLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_iterate_rec
(
    HANDLE_T			h_svl,
    UINT32              ui4_current_channel,
    UINT32              ui4_nw_mask,
    SVL_ITERATE_T		e_iter_dir,  
    SVL_REC_T*			pt_svl_rec,
    UINT32*             pui4_ver_id 
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    SVL_SEARCH_PARM_T   t_parm;
 
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_svl_rec     != NULL
            )
        {
            /* Set up the search parameter and address, we set the search
               field type to channel type, as we want to use the channel
               number to position of the start of the iteration.
            */
            t_parm.ui4_fld_type = (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID ;
            t_parm.u.t_chan.ui4_channel_num = ui4_current_channel;
            t_parm.u.t_chan.ui4_nw_mask     = ui4_nw_mask;
            t_parm.u.t_chan.ui2_idx         = 0;
            
            i4_rc=svl_iterate_from_sorted_list(pt_svl,
                                               &t_parm,
                                               e_iter_dir,
                                               pt_svl_rec,
                                               pui4_ver_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} iterate [handle: %d] [value: %d] [nw_mask: 0x%x] [direction: %d] [ver_id: %d] [svl_rec_id: %d]\n",h_svl, ui4_current_channel, ui4_nw_mask, e_iter_dir, *pui4_ver_id, pt_svl_rec->ui2_svl_rec_id));
    }
    else
    {
        DBG_ERROR(("{SVL} iterate [handle %d] [value: %d] [direction: %d] failed.\n",h_svl, ui4_current_channel, e_iter_dir));
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_iterate_by_nw
 *
 * Description: This API iterates over service records in the SVL list with
 *              reference to the given NWL record.  It returns the next
 *              matching service record from a specified the channel number
 *              of the current service record location.  
 *              The 'e_iter_dir' argument specifies direction of iteration, 
 *              e.g., the next record's channel numeric value is plus or 
 *              minus from the current record's channel numeric value.  
 *              Service records that do not match the network mask and NWL
 *              ID / NWL record ID specified will be skipped during the
 *              iteration.  
 *
 *              If the current record is at the end or beginning of the 
 *              service list, then 'CHANNEL_PLUS' or 'CHANNEL_MINUS' will 
 *              return a status to indicate that end-point is reached. It is 
 *              up the caller to decide if next service record should wrap 
 *              around to the beginning service record or to the end of the 
 *              service record.
 *
 *              Since the service record in the SVL is sorted by 'channel
 *              number', the iteration can be thought of as getting the next 
 *              or the previous channel in the specified network.
 *
 *              Specifying 'CHANNEL_SMALLEST' returns the record with the 
 *              smallest channel number (regardless of the current service 
 *              record position), and similarly specifying 'CHANNEL_LARGEST' 
 *              returns the record with the largest channel number.
 *
 * Inputs: 
 *    h_svl     Handle to the SVL object.
 *    ui4_current_channel   Specifies the channel number of the current 
 *                          service record to begin for the search for 
 *                          the next service record.
 *    ui4_nw_mask   32-bits mask specifying the network(s) of the service 
 *                  record to be returned.  
 *    ui2_nwl_id        Specifies the Network List ID value.
 *    ui2_nwl_rec_id	Specifies the Network record ID value.  
 *    e_iter_dir    Specifies the direction of the iteration.  The values are:
 *
 *                        SVL_CHANNEL_PLUS	
 *                        SVL_CHANNEL_MINUS
 *                        SVL_CHANNEL_SMALLEST
 *                        SVL_CHANNEL_LARGEST
 *
 *                  If SVL_CHANNEL_SMALLEST or SVL_CHANNEL_LARGEST is 
 *                  specified, then the record with the smallest (numerically)
 *                  channel value is returned, and vice versa.  If the 
 *                  iteration has reach an end point, i.e., the current record
 *                  is point to the first or last service record, then the 
 *                  current service record is returned.
 *
 * Outputs: 
 *    pt_svl_rec    Pointer to a SVL record structure.
 *
 * In/Output:
 *    pui4_ver_id   Pointer to a variable holding version id of the SVL
 *                  database object.  On entry, this variable contains the
 *                  version id of the database that the caller expects.  
 *                  On the return, this variable contains the current version 
 *                  id of the database.   If the expected version id and 
 *                  current database version id do not match, an error status 
 *                  code is returned.  If 'pui4_ver_id' is set to 
 *                  'SVL_NULL_VER_ID' value, then no checking is done and the 
 *                  latest version id is returned the application.
 * Returns: 
 *   SVLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_iterate_by_nw
(
    HANDLE_T			h_svl,
    UINT32              ui4_current_channel,
    UINT32              ui4_nw_mask,
    UINT16              ui2_nwl_id,
    UINT16              ui2_nwl_rec_id,
    SVL_ITERATE_T		e_iter_dir,  
    SVL_REC_T*			pt_svl_rec,
    UINT32*             pui4_ver_id 
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    SVL_SEARCH_PARM_T   t_parm;
 
    if (ui2_nwl_id == SVL_NULL_ID && ui2_nwl_rec_id == SVL_NULL_REC_ID)
    {
        return x_svl_iterate_rec(h_svl, ui4_current_channel, ui4_nw_mask,
                                 e_iter_dir, pt_svl_rec, pui4_ver_id);
    }

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_svl_rec     != NULL
            )
        {
            /* Set up the search parameter and address, we set the search
               field type to channel type, as we want to use the channel
               number to position of the start of the iteration.
            */
            t_parm.ui4_fld_type = (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID ;
            t_parm.u.t_chan.ui4_channel_num = ui4_current_channel;
            t_parm.u.t_chan.ui4_nw_mask     = ui4_nw_mask;
            t_parm.u.t_chan.ui2_idx         = 0;
            
            for (i4_rc=svl_iterate_from_sorted_list(pt_svl,
                                                    &t_parm,
                                                    e_iter_dir,
                                                    pt_svl_rec,
                                                    pui4_ver_id);
                 i4_rc == SVLR_OK &&
                 (pt_svl_rec->uheader.t_rec_hdr.ui2_nwl_id != ui2_nwl_id ||
                  pt_svl_rec->uheader.t_rec_hdr.ui2_nwl_rec_id != ui2_nwl_rec_id);

                 i4_rc=svl_iterate_from_sorted_list(pt_svl,
                                                    &t_parm,
                                                    e_iter_dir,
                                                    pt_svl_rec,
                                                    pui4_ver_id))
            {
                t_parm.u.t_chan.ui4_channel_num = pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id;

                if (e_iter_dir == SVL_CHANNEL_SMALLEST)
                {
                    e_iter_dir = SVL_CHANNEL_PLUS;
                }
                else if (e_iter_dir == SVL_CHANNEL_LARGEST)
                {
                    e_iter_dir = SVL_CHANNEL_MINUS;
                }
            }
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} iterate by nw [handle: %d] [value: %d] [nw_mask: 0x%x] [direction: %d] [ver_id: %d] [svl_rec_id: %d]\n",h_svl, ui4_current_channel, ui4_nw_mask, e_iter_dir, *pui4_ver_id, pt_svl_rec->ui2_svl_rec_id));
    }
    else
    {
        DBG_ERROR(("{SVL} iterate by nw [handle %d] [value: %d] [direction: %d] failed.\n",h_svl, ui4_current_channel, e_iter_dir));
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_iterate_by_sat
 *
 * Description: This API iterates over service records in the SVL list with
 *              reference to the given SATL record.  It returns the next
 *              matching service record from a specified the channel number
 *              of the current service record location.  
 *              The 'e_iter_dir' argument specifies direction of iteration, 
 *              e.g., the next record's channel numeric value is plus or 
 *              minus from the current record's channel numeric value.  
 *              Service records that do not match the network mask and SATL
 *              ID / SATL record ID specified will be skipped during the
 *              iteration.  
 *
 *              If the current record is at the end or beginning of the 
 *              service list, then 'CHANNEL_PLUS' or 'CHANNEL_MINUS' will 
 *              return a status to indicate that end-point is reached. It is 
 *              up the caller to decide if next service record should wrap 
 *              around to the beginning service record or to the end of the 
 *              service record.
 *
 *              Since the service record in the SVL is sorted by 'channel
 *              number', the iteration can be thought of as getting the next 
 *              or the previous channel in the specified satellite.
 *
 *              Specifying 'CHANNEL_SMALLEST' returns the record with the 
 *              smallest channel number (regardless of the current service 
 *              record position), and similarly specifying 'CHANNEL_LARGEST' 
 *              returns the record with the largest channel number.
 *
 * Inputs: 
 *    h_svl     Handle to the SVL object.
 *    ui4_current_channel   Specifies the channel number of the current 
 *                          service record to begin for the search for 
 *                          the next service record.
 *    ui4_nw_mask   32-bits mask specifying the network(s) of the service 
 *                  record to be returned.  
 *    ui2_satl_id       Specifies the Satellite List ID value.
 *    ui2_satl_rec_id	Specifies the Satellite record ID value.  
 *    e_iter_dir    Specifies the direction of the iteration.  The values are:
 *
 *                        SVL_CHANNEL_PLUS	
 *                        SVL_CHANNEL_MINUS
 *                        SVL_CHANNEL_SMALLEST
 *                        SVL_CHANNEL_LARGEST
 *
 *                  If SVL_CHANNEL_SMALLEST or SVL_CHANNEL_LARGEST is 
 *                  specified, then the record with the smallest (numerically)
 *                  channel value is returned, and vice versa.  If the 
 *                  iteration has reach an end point, i.e., the current record
 *                  is point to the first or last service record, then the 
 *                  current service record is returned.
 *
 * Outputs: 
 *    pt_svl_rec    Pointer to a SVL record structure.
 *
 * In/Output:
 *    pui4_ver_id   Pointer to a variable holding version id of the SVL
 *                  database object.  On entry, this variable contains the
 *                  version id of the database that the caller expects.  
 *                  On the return, this variable contains the current version 
 *                  id of the database.   If the expected version id and 
 *                  current database version id do not match, an error status 
 *                  code is returned.  If 'pui4_ver_id' is set to 
 *                  'SVL_NULL_VER_ID' value, then no checking is done and the 
 *                  latest version id is returned the application.
 * Returns: 
 *   SVLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_iterate_by_sat
(
    HANDLE_T			h_svl,
    UINT32              ui4_current_channel,
    UINT32              ui4_nw_mask,
    UINT16              ui2_satl_id,
    UINT16              ui2_satl_rec_id,
    SVL_ITERATE_T		e_iter_dir,  
    SVL_REC_T*			pt_svl_rec,
    UINT32*             pui4_ver_id 
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    SVL_SEARCH_PARM_T   t_parm;
 
    if (ui2_satl_id == SVL_NULL_ID && ui2_satl_rec_id == SVL_NULL_REC_ID)
    {
        return x_svl_iterate_rec(h_svl, ui4_current_channel, ui4_nw_mask,
                                 e_iter_dir, pt_svl_rec, pui4_ver_id);
    }

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_svl_rec     != NULL
            )
        {
            /* Set up the search parameter and address, we set the search
               field type to channel type, as we want to use the channel
               number to position of the start of the iteration.
            */
            t_parm.ui4_fld_type = (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID ;
            t_parm.u.t_chan.ui4_channel_num = ui4_current_channel;
            t_parm.u.t_chan.ui4_nw_mask     = ui4_nw_mask;
            t_parm.u.t_chan.ui2_idx         = 0;
            
            for (i4_rc=svl_iterate_from_sorted_list(pt_svl,
                                                    &t_parm,
                                                    e_iter_dir,
                                                    pt_svl_rec,
                                                    pui4_ver_id);
                 i4_rc == SVLR_OK &&
                 (pt_svl_rec->uheader.t_rec_hdr.ui2_satl_id != ui2_satl_id ||
                  pt_svl_rec->uheader.t_rec_hdr.ui2_satl_rec_id != ui2_satl_rec_id);

                 i4_rc=svl_iterate_from_sorted_list(pt_svl,
                                                    &t_parm,
                                                    e_iter_dir,
                                                    pt_svl_rec,
                                                    pui4_ver_id))
            {
                t_parm.u.t_chan.ui4_channel_num = pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id;

                if (e_iter_dir == SVL_CHANNEL_SMALLEST)
                {
                    e_iter_dir = SVL_CHANNEL_PLUS;
                }
                else if (e_iter_dir == SVL_CHANNEL_LARGEST)
                {
                    e_iter_dir = SVL_CHANNEL_MINUS;
                }
            }
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} iterate by sat [handle: %d] [value: %d] [nw_mask: 0x%x] [direction: %d] [ver_id: %d] [svl_rec_id: %d]\n",h_svl, ui4_current_channel, ui4_nw_mask, e_iter_dir, *pui4_ver_id, pt_svl_rec->ui2_svl_rec_id));
    }
    else
    {
        DBG_ERROR(("{SVL} iterate by sat [handle %d] [value: %d] [direction: %d] failed.\n",h_svl, ui4_current_channel, e_iter_dir));
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_sort_rec
 *
 * Description: This API applies the sort operation (using the
 *              per-handle compare fucntion, or the global compare
 *              function, or default compare function, to the SVL
 *              records.
 *             
 * Inputs: 
 *    h_svl     Handle to the SVL object.
 *
 * Outputs: 
 *    pui4_ver_id   Contains the version id of the SVL database object.   
 *                  
 * Returns: 
 *   SVLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_sort_rec
(
    HANDLE_T			h_svl,
    UINT32*             pui4_ver_id 
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
 
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pui4_ver_id  != NULL )
        {
            i4_rc=svl_sort_rec_by_channel(pt_svl, TRUE, pui4_ver_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_get_ver_id
 *
 * Description: This function returns the current version id of the
 *              SVL database object.
 *
 * Inputs: 
 *   h_svl        Handle to the SVL object.
 *
 * Outputs:
 *   pui4_ver_id  Pointer to a variable holding version id of the SVL
 *                database object.
 * Returns: 
 *   SVLR_OK         Version id is returned. 
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pui4_ver_id' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_svl_get_ver_id
(
    HANDLE_T			h_svl,
    UINT32*			    pui4_ver_id
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
 
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pui4_ver_id != NULL )
        {   
            i4_rc=svl_get_db_version_id(pt_svl, pui4_ver_id);               
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_rec_init
 *
 * Description: This API pre-sets the SVL_REC_T structure to a 
 * pre-defined invalid value (e.g., 0xFF).   The pre-defined invalid 
 * value can be used to detect changes in the SVL_REC_T structure for 
 * backward compatibility in the future.
 *
 * For example, if a new field is added to the SVL record structure.  
 * At the runtime, if the new field in the SVL_REC_T structure passed 
 * into SVL library contain 0xFF, then we know the caller has not been 
 * re-compile with the new SVL module.   
 *
 * Inputs: 
 *   Pointer to a SVL record structure. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL library is initialized successfully.
 *   SVLR_INV_ARG      'pt_svl_rec' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_svl_rec_init
(
    SVL_REC_T*			pt_svl_rec 
)
{
    if ( pt_svl_rec  != NULL  )
    {
        x_memset(pt_svl_rec,0,sizeof(SVL_REC_T));
    }
    else
    {
        return SVLR_INV_ARG;
    }
    return SVLR_OK;    
}

/*------------------------------------------------------------------
 * Name:  svl_2_cdb_sort_cmp_fct_wrapper
 *
 * Description: Compare function registered with CDB database object.
 *              When this function is called by CDB internally to
 *              perform sorting operation, it will then invoke the
 *              SVL sort compare function.
 *
 * Inputs: 
 *   pv_elem_a  CDB record 1 for comparison.
 *
 *   pv_elem_b  CDB record 2 for comparison.
 *
 *   pv_data    Referece to the SVL private structure containing
 *              the actual compare function.
 *
 * Outputs:
 *   None.
 *
 * Returns: 
 *   SVLR_OK         Version id is returned. 
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pf_sort_cmp' or 'pf_search_cmp' is a NULL pointer.
 *
 *
--------------------------------------------------------------------*/
INT32  svl_2_cdb_sort_cmp_fct_wrapper
(
    const VOID*  pv_elem_a,
    const VOID*  pv_elem_b,
    VOID*        pv_data
)
{
    SVL_REC_T         t_svl_rec_1;
    SVL_REC_T         t_svl_rec_2;
    UINT8*            pui1_a;
    UINT8*            pui1_b;
    UINT8             ui1_fld_count;
    DB_FIELD_INFO_T*  pt_rec_fields;
    DB_FIELD_T*       pui4_recfld_type;
    VOID*             apv_recfld_addr[SVL_NUM_OF_REC_FIELDS];
    INT32             i4_union_tag;
    INT32             i4_rc;

    pui1_a = *( (UINT8**) pv_elem_a ) ;
    pui1_b = *( (UINT8**) pv_elem_b );
    
    /* Specifies the fields to retrieve the record data. For
       SVL record, we extract all fields of the data.
    */
    svl_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);
    
    /*
      call the SVL sort compare function.
    */
    if ( pv_data != 0 )
    {
        SVL_CMP_FCT_INFO_T*    pt_cmp_fct_tag = NULL ;

        pt_cmp_fct_tag = (SVL_CMP_FCT_INFO_T*) pv_data;

        /* Set up the addresses to copy the record data to. */
        svl_rec_fld_to_addr(&t_svl_rec_1,
                            (const VOID**)apv_recfld_addr,
                            NULL,
                            0,
                            &i4_union_tag);
        /*
          convert the byte data to SVL record.
        */
        db_parse_rec_data(pt_cmp_fct_tag->pt_cdb_database,
                          (UINT8*) pui1_a,
                          ui1_fld_count,
                          pui4_recfld_type,
                          apv_recfld_addr);

        /* Set up the addresses to copy the second record data. */
        svl_rec_fld_to_addr(&t_svl_rec_2,
                            (const VOID**)apv_recfld_addr,
                            NULL,
                            0,
                            &i4_union_tag);
        /*
          convert the byte data to SVL record.
        */
        db_parse_rec_data(pt_cmp_fct_tag->pt_cdb_database,
                          (UINT8*) pui1_b,
                          ui1_fld_count,
                          pui4_recfld_type,
                          apv_recfld_addr);
#if 0
        x_dbg_stmt("{SVL} sort cmp called.\n");
        x_dbg_stmt("      record 1: \n");
        svl_print_rec(&t_svl_rec_1, 0);
        x_dbg_stmt("      record 2: \n");
        svl_print_rec(&t_svl_rec_2, 0);
#endif        

        i4_rc = pt_cmp_fct_tag->pf_sort_cmp(&t_svl_rec_1, &t_svl_rec_2,       
                                            (pt_cmp_fct_tag->pv_tag));
    }
    else
    {
        i4_rc = 0;
    }
    return i4_rc;
}


INT32  svl_2_cdb_search_cmp_fct_wrapper
(
    const VOID*  pv_elem_a,
    const VOID*  pv_data_to_compare,
    VOID*        pv_tag_data
)
{
    SVL_REC_T         t_svl_rec_1;
    UINT8*            pui1_a;
    UINT8             ui1_fld_count;
    DB_FIELD_INFO_T*  pt_rec_fields;
    DB_FIELD_T*       pui4_recfld_type;
    VOID*             apv_recfld_addr[SVL_NUM_OF_REC_FIELDS];
    INT32             i4_union_tag;
    INT32             i4_rc;

    pui1_a = *( (UINT8**) pv_elem_a ) ;

    /*
       Specifies the fields to retrieve the record data. For
       SVL record, we extract all fields of the data.
    */
    svl_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);
    
    if ( pv_tag_data != NULL )
    {
        SVL_CMP_FCT_INFO_T*    pt_cmp_fct_tag = NULL ;

        pt_cmp_fct_tag = (SVL_CMP_FCT_INFO_T*) pv_tag_data;

        /* Set up the addresses to copy the record data to. */
        svl_rec_fld_to_addr(&t_svl_rec_1,
                            (const VOID**)apv_recfld_addr,
                            NULL,
                            0,
                            &i4_union_tag);
        /*
          convert the byte data to SVL record.
        */
        db_parse_rec_data(pt_cmp_fct_tag->pt_cdb_database,
                          (UINT8*) pui1_a,
                          ui1_fld_count,
                          pui4_recfld_type,
                          apv_recfld_addr);
#if 0
        x_dbg_stmt("{SVL] search cmp called.\n");
        svl_print_rec(&t_svl_rec_1, 0);
#endif        

        i4_rc = pt_cmp_fct_tag->pf_search_cmp(&t_svl_rec_1,
                                              pv_data_to_compare,
                                              (pt_cmp_fct_tag->pv_tag));
    }
    else
    {
        i4_rc = 0 ;
    }
    
    return  i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_reg_cmp_fcts
 *
 * Description: This function register compare functions for
 *              sorting and for searching operation.
 *
 *              If global flag is TRUE, then this compare functions
 *              apply to all SVL handle points to the same CDB database
 *              object.
 *
 *              Otherwise, the compare function are SVL handle specific.
 *
 * Inputs: 
 *   h_svl        Handle to the SVL object.
 *
 *   b_global     TRUE:  compare functions are global scope.
 *                FALSE: compare functions are SVL handle specific.
 *
 *   pf_sort_cmp  Compare function used for sorting.
 *
 *   pv_sort_info
 *
 *   pf_search_cmp Compare function used for searching.
 *
 * Outputs:
 *   None.
 *
 * Returns: 
 *   SVLR_OK         Version id is returned. 
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pf_sort_cmp' or 'pf_search_cmp' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_svl_reg_cmp_fcts
(
    HANDLE_T               h_svl,
    BOOL                   b_global,
    VOID*                  pv_tag,   
    x_svl_sort_cmp         pf_sort_cmp,
    x_svl_search_cmp       pf_search_cmp
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    UINT32           ui4_ver_id=0;
    
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        /*
          Both function must be provided or else both functions must be
          NULL.  If both are NULL, it means un-registering the SVL Handle
          specific compare functions.
        */
        if ( (pf_sort_cmp != NULL && pf_search_cmp != NULL) ||
             (pf_sort_cmp == NULL && pf_search_cmp == NULL )
           )
        {
            if ( b_global == TRUE )
            {
                /*
                  Because this is global functions and its associated tags
                  are attached to CDB data object, if the this SVL handle
                  that register the global sort/search
                  compare function is invalidate/freed, we still need to
                  keep the memory points to by the tag.

                  Therefore the current solution is to dynamic allocate the
                  memory region for the registered tag, and when another
                  registeration is called, the old tag will be freed by
                  the CDB data object. 
                */
                SVL_CMP_FCT_INFO_T*    pt_cmp_fct_tag = NULL ;

                /*
                  Note: this function should not delete this data, it will
                  be deleted by the CDB data object when it is no longer
                  needed.
                */
                pt_cmp_fct_tag = (SVL_CMP_FCT_INFO_T*)
                    x_mem_alloc(sizeof(SVL_CMP_FCT_INFO_T));

                if ( pt_cmp_fct_tag != NULL )
                {
                    pt_cmp_fct_tag->pf_sort_cmp   = pf_sort_cmp;
                    pt_cmp_fct_tag->pf_search_cmp = pf_search_cmp;
                    pt_cmp_fct_tag->pv_tag        = pv_tag;
                    
                    i4_rc = db_get_database_obj(pt_svl->h_cdb,
                                                &(pt_cmp_fct_tag->pt_cdb_database));
                    if ( i4_rc == DBR_OK )
                    {
                        i4_rc = db_reg_cmp_fcts(pt_svl->h_cdb,
                                                svl_2_cdb_sort_cmp_fct_wrapper,
                                                svl_2_cdb_search_cmp_fct_wrapper,
                                                (VOID*) pt_cmp_fct_tag);
                    }
                    else
                    {
                        x_mem_free(pt_cmp_fct_tag);
                    }

                    i4_rc = svl_cdb_to_svl_err_code(i4_rc);
                }
                else
                {
                    i4_rc = SVLR_OUT_OF_MEM ;
                }
            }
            else
            {
                /*
                  Attached the compare functions with SVL object.
                */
                pt_svl->t_cmp_fct_info.pf_sort_cmp   = pf_sort_cmp;
                pt_svl->t_cmp_fct_info.pf_search_cmp = pf_search_cmp;
                pt_svl->t_cmp_fct_info.pv_tag        = pv_tag;
                
                i4_rc = db_get_database_obj(pt_svl->h_cdb,
                                            &(pt_svl->t_cmp_fct_info.pt_cdb_database));
                if ( i4_rc == DBR_OK )
                {
                    i4_rc = SVLR_OK;
                }
                else
                {
                    i4_rc = SVLR_FAIL;
                    pt_svl->t_cmp_fct_info.pf_sort_cmp   = NULL;
                    pt_svl->t_cmp_fct_info.pf_search_cmp = NULL;
                    pt_svl->t_cmp_fct_info.pv_tag    = NULL;
                }
                svl_sort_rec_by_channel(pt_svl, TRUE, &ui4_ver_id);
            }     
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} reg cmp fct [handle: %d] [flag: %d] [tag: 0x%x] [sort_fct: 0x%x] [search_fct: 0x%x]\n",h_svl, b_global, pv_tag, pf_sort_cmp, pf_search_cmp));
    }
    else
    {
        DBG_ERROR(("{SVL} reg cmp fct [handle %d] failed.\n",h_svl));
    }

    SVL_API_LOG("{svl}%s(0x%x,%d)=%d\n",__FUNCTION__,h_svl,b_global,i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_unreg_cmp_fcts
 *
 * Description: This function un-register compare functions for
 *              sorting and for searching operation.
 *
 *              If global flag is TRUE, then this compare functions
 *              apply to all SVL handle points to the same CDB database
 *              object.
 *
 *              Otherwise, the compare function are SVL handle specific.
 *
 * Inputs: 
 *   h_svl        Handle to the SVL object.
 *
 *   b_global     TRUE:  compare functions are global scope.
 *                FALSE: compare functions are SVL handle specific.
 *
 * Outputs:
 *   None.
 *
 * Returns: 
 *   SVLR_OK         Version id is returned. 
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pf_sort_cmp' or 'pf_search_cmp' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_svl_unreg_cmp_fcts
(
    HANDLE_T               h_svl,
    BOOL                   b_global
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    
    if ( (handle_get_type_obj(h_svl, &pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                     &&
         (pt_svl  != NULL)
        )
    {
        if ( b_global == TRUE )
        {
            i4_rc = db_unreg_cmp_fcts(pt_svl->h_cdb);

            i4_rc = svl_cdb_to_svl_err_code(i4_rc);                
        }
        else
        {
            /*
              Un-attached the compare functions with SVL object.
            */
            pt_svl->t_cmp_fct_info.pf_sort_cmp   = NULL;
            pt_svl->t_cmp_fct_info.pf_search_cmp = NULL;
            pt_svl->t_cmp_fct_info.pv_tag    = NULL;
            i4_rc = SVLR_OK;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    if ( i4_rc == SVLR_OK )
    {
        DBG_API(("{SVL} Un-reg cmp fct [handle: %d] [flag: %d] \n",h_svl, b_global));
    }
    else
    {
        DBG_ERROR(("{SVL} Un-reg cmp fct [handle %d] failed.\n",h_svl));
    }

    SVL_API_LOG("{svl}%s(0x%x,%d)=%d\n",__FUNCTION__,h_svl,b_global,i4_rc);
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: x_svl_get_info
 *
 * Description: 
 *   Get info (number of records, size of record, etc) for this SVL handle.
 *
 *
 * Inputs: 
 *   h_svl        Handle to the SVL object.
 *
 * Outputs:
 *   None.
 *
 * Returns: 
 *   SVLR_OK         Version id is returned. 
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pf_sort_cmp' or 'pf_search_cmp' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_svl_get_info
(
    HANDLE_T               h_svl
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    DB_INFO_T           t_db_info;
    UINT32              ui4_persistent_size;
    UINT32              ui4_ver_id;
    
    if ( (handle_get_type_obj(h_svl, &pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                     &&
         (pt_svl  != NULL)
        )
    {
        x_dbg_stmt("<SVL ID:  %d  SVL_NAME: %s>\n",
                   pt_svl->ui2_svl_id, pt_svl->ps_svl_name);
        
        if ( db_get_info(pt_svl->h_cdb, &t_db_info) == DBR_OK )
        {
            x_dbg_stmt("number of SVL records:   %d \n",
                       t_db_info.ui4_record_count);
            
            x_dbg_stmt("Heap memory size of each record:     %d (bytes)\n",
                       t_db_info.ui2_record_width);

            x_dbg_stmt("Heap memory usage for string buffer:  allocated: %d (bytes) used: %d (bytes)\n",
                       t_db_info.ui4_str_buf_allocated,
                       t_db_info.ui4_str_buf_used);

            x_dbg_stmt("Heap memory usage for string obj array: allocated: %d (bytes) used: %d (bytes)\n",
                       t_db_info.ui2_nb_str_obj_allocated*sizeof(UINT16),
                       t_db_info.ui2_nb_str_obj_used*sizeof(UINT16));
            
            x_dbg_stmt("SVL object filesize [header+metadata+data]:  %d (bytes)\n",
                       svl_persistent_size(pt_svl));

            x_dbg_stmt("Current SVL file version: %d\n", SVL_FILE_VER);
            
            /* Size of CDB object. */
            i4_rc=db_get_persistent_size(pt_svl->h_cdb,
                                         (db_endian_conv_fct)svl_data_conv_to_big_end,
                                         &ui4_persistent_size,
                                         &ui4_ver_id);
            if ( i4_rc == DBR_OK )
            {
                x_dbg_stmt("[with CDB object using: [%d bytes]\n", ui4_persistent_size);
            }
            
            i4_rc = SVLR_OK;
        }
        else
        {
            x_dbg_stmt("Error: can get info on this SVL handle.\n");
            i4_rc = SVLR_FAIL;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: x_svl_get_client_data
 *
 * Description: This API gets customer's private data associated with
 *    the specified SVL record (e.g.,'ui2_svl_rec_id')
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_svl_rec_id  Specifies the service record id.
 *
 * Outputs: 
 *   pt_client_data  Pointer to a structure containing customer data
 *                   If the buffer is too small, only data up to the
 *                   len of 'pt_client_data->ui1_buf_len' is copied out.
 *                   The paramter 'pt_client_data->ui1_data_len' will be
 *                   set to indicate the total len of customer data in
 *                   the internal SVL record. 
 *
 *   pui4_ver_id     Pointer to a variable holding version id of the
 *                   database object.   Note: version id of the SVL
 *                   database object is incremented each time when a
 *                   modification takes place.
 * Returns: 
 *   SVLR_OK    Customer's private data from the specified record is returned. 
 *   SVLR_INV_HANDLE    Bad handle.
 *   SVLR_REC_NOT_FOUND No record matching the search parameter 
 *                      ('ui2_svl_rec_id')  is found.
 *   SVLR_INV_ARG       'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_get_client_data
(
    HANDLE_T               h_svl,
    UINT16                 ui2_svl_rec_id,
    SVL_CLIENT_DATA_T*     pt_client_data,
    UINT32*                pui4_ver_id
)
{
    INT32                  i4_rc;
    SVL_OBJ_T*             pt_svl;
    HANDLE_TYPE_T          pe_type;
    
    UINT8                  ui1_key_count;
    DB_FIELD_T             aui4_keyfld_type[1];
    const VOID*            apv_keyfld_addr[1];
    
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pui4_ver_id    != NULL &&
             pt_client_data != NULL &&
             ui2_svl_rec_id != SVL_NULL_REC_ID   )
        {
            /* Set up arguments to read record by record_id value. */
            ui1_key_count = 1;
            aui4_keyfld_type[0]      = (DB_FIELD_T) SVL_CDB_TYPE_REC_ID ;
            apv_keyfld_addr[0]       = &ui2_svl_rec_id;
            
            i4_rc = svl_get_client_private_data(pt_svl,
                                                ui1_key_count,
                                                aui4_keyfld_type,
                                                apv_keyfld_addr,
                                                NULL,
                                                pt_client_data,
                                                0,
                                                pui4_ver_id);           
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_set_client_data
 *
 * Description: This API copies customer's private data associated with
 *    the specified SVL record (e.g.,'ui2_svl_rec_id') to SVL database.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_svl_rec_id  Specifies the service record id.
 *   pt_client_data  Pointer to a structure containing customer data
 *                   The paramter 'pt_client_data->ui1_data_len' indicates
 *                   the total len of customer data in the 'pt_client_data->
 *                   pui1_data_buf' buffer.
 *
 * Outputs: 
 *   None
 *
 * Returns: 
 *   SVLR_OK            Customer's private data is copied into SVL database. 
 *   SVLR_INV_HANDLE    Bad handle.
 *   SVLR_REC_NOT_FOUND No record matching the search parameter 
 *                      ('ui2_svl_rec_id')  is found.
 *   SVLR_INV_ARG       'pt_client_data' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_set_client_data
(
    HANDLE_T               h_svl,
    UINT16                 ui2_svl_rec_id,
    SVL_CLIENT_DATA_T*     pt_client_data
)
{
    INT32           i4_rc;
    SVL_OBJ_T*      pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pt_client_data != NULL )
        {          
            i4_rc=svl_set_client_private_data(pt_svl,
                                              ui2_svl_rec_id,
                                              pt_client_data);          
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    SVL_API_LOG("{svl}%s(0x%x,%d)=%d\n",__FUNCTION__,h_svl,ui2_svl_rec_id,i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_iterate_rec_by_custom_ord
 *
 * Description: This API iterates over service records in the SVL list.  
 *              It returns the next/prev service record from a given
 *              service record location specified by the pv_data.
 *              
 *              The 'e_iter_dir' argument specifies direction of iteration, 
 *              e.g., the next record or prev record from the current
 *              record location identified by the 'pv_data'
 *
 *              Service records that do not match the network mask 
 *              specification will be skipped during the iteration.  
 *
 *              If the current record is at the end or beginning of the 
 *              sorted service list, then 'SVL_NEXT_REC' or 'SVL_PREV_REC'  
 *              returns a status to indicate that end-point is reached. It is 
 *              up the caller to decide if next service record should wrap 
 *              around to the beginning service record or to the end of the 
 *              service record.
 *
 *              Since the service record in the SVL is sorted by the order
 *              specified in the client provided compare function and search
 *              compare function, the iteration can be thought of as getting
 *              the next or the previous record in a customized ordered list.
 *
 *              Specifying 'SVL_FIRST_REC' returns the first record in 
 *              sorted list (regardless of the current service 
 *              record position), and similarly specifying 'SVL_LAST_REC' 
 *              returns the last record in the customized sorted list.
 *
 * Inputs: 
 *    h_svl     Handle to the SVL object.
 *    pv_data   Specifies the 'pv_data' of the current service record
 *              to begin for the search for the next service record.
 *
 *    ui4_nw_mask   32-bits mask specifying the network(s) of the service 
 *                  record to be returned.  
 *
 *    e_iter_dir    Specifies the direction of the iteration.  The values are:
 *
 *                        SVL_NEXT_REC	
 *                        SVL_PREV_REC
 *                        SVL_FIRST_REC
 *                        SVL_LAST_REC
 *
 *                  If SVL_FIRST_REC or SVL_LAST_REC is specified, then
 *                  the first record in the sorted list is returned.  If the 
 *                  iteration has reach an end point, i.e., the current record
 *                  is point to the first or last service record, then the 
 *                  current service record is returned.
 *
 * Outputs: 
 *    pt_svl_rec    Pointer to a SVL record structure.
 *
 * In/Output:
 *    pui4_ver_id   Pointer to a variable holding version id of the SVL
 *                  database object.  On entry, this variable contains the
 *                  version id of the database that the caller expects.  
 *                  On the return, this variable contains the current version 
 *                  id of the database.   If the expected version id and 
 *                  current database version id do not match, an error status 
 *                  code is returned.  If 'pui4_ver_id' is set to 
 *                  'SVL_NULL_VER_ID' value, then no checking is done and the 
 *                  latest version id is returned the application.
 * Returns: 
 *   SVLR_OK           
 *   
 ---------------------------------------------------------------------*/
INT32 x_svl_iterate_rec_by_custom_ord
(
    HANDLE_T			h_svl,
    VOID*               pv_data, 
    UINT32              ui4_nw_mask,
    SVL_ITERATE_T		e_iter_dir,  /* first, last, next or prev rec */
    SVL_REC_T*			pt_svl_rec,
    UINT32*             pui4_ver_id 
)
{
    INT32               i4_rc;
    SVL_OBJ_T*  	    pt_svl;
    HANDLE_TYPE_T       pe_type;
    SVL_SEARCH_PARM_T   t_parm;
 
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_svl_rec     != NULL
            )
        {
            /* Set up the search parameter, although this is cutomized
               serarch, we will set the search field type to 'channel type',
               as we want to use the structure 't_parm' to pass the netmask
               value for comparison inside the subroutine.
            */
            t_parm.ui4_fld_type             = (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID ;
            t_parm.pv_data                  = pv_data;
            t_parm.u.t_chan.ui4_nw_mask     = ui4_nw_mask;
            t_parm.u.t_chan.ui2_idx         = 0;
            
            
            i4_rc=svl_iterate_from_customized_sorted_list(pt_svl,
                                                          &t_parm,
                                                          e_iter_dir,
                                                          pt_svl_rec,
                                                          pui4_ver_id);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_init_mem_dump
 *
 * Description: This API initializes the memory dump structure, which 
 *              is used for x_svl_dump_to_mem.
 *
 * Inputs: 
 *   None
 *
 * Outputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Returns: 
 *   SVLR_OK            SVL database successfully dumped.
 *   SVLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_init_mem_dump
(
    SVL_MEM_DUMP_T*        pt_mem_dump
)
{
    INT32           i4_rc;

    if ( pt_mem_dump != NULL )
    {          
        x_memset(pt_mem_dump, 0, sizeof(SVL_MEM_DUMP_T));
        i4_rc=SVLR_OK;
    }
    else
    {
        i4_rc=SVLR_INV_ARG;
    }

	SVL_API_LOG("{svl}%s(0x%x)=%d\n",__FUNCTION__,pt_mem_dump,i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_free_mem_dump
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
 *   SVLR_OK            SVL database successfully dumped.
 *   SVLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_free_mem_dump
(
    SVL_MEM_DUMP_T*        pt_mem_dump
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
        i4_rc=SVLR_OK;
    }
    else
    {
        i4_rc=SVLR_INV_ARG;
    }

	SVL_API_LOG("{svl}%s(0x%x)=%d\n",__FUNCTION__,pt_mem_dump,i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_dump_to_mem
 *
 * Description: This API dumps the specified SVL database into the specified
 *              structure, which could be later used for x_svl_load_from_mem.
 *              The memory dump structure should be initialized before dumping.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *
 * Outputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Returns: 
 *   SVLR_OK            SVL database successfully dumped.
 *   SVLR_INV_HANDLE    Bad handle.
 *   SVLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_dump_to_mem
(
    HANDLE_T               h_svl,
    SVL_MEM_DUMP_T*        pt_mem_dump
)
{
    INT32           i4_rc;
    SVL_OBJ_T*      pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pt_mem_dump != NULL )
        {
            pt_mem_dump->ui2_svl_id = pt_svl->ui2_svl_id;
            x_strcpy(pt_mem_dump->ps_svl_name, pt_svl->ps_svl_name);
            i4_rc = db_dump_to_mem(pt_svl->h_cdb,
                                   TRUE,
                                   &(pt_mem_dump->pv_data),
                                   &(pt_mem_dump->ui4_data_len));
            i4_rc = svl_cdb_to_svl_err_code(i4_rc);
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

	SVL_API_LOG("{svl}%s(0x%x, 0x%x)=%d\n",__FUNCTION__,h_svl,pt_mem_dump,i4_rc);
	
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_load_from_mem
 *
 * Description: This API loads the SVL database from the specified memory
 *              dump structure, whose data comes from a previous call to
 *              x_svl_load_from_mem.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Outputs: 
 *
 * Returns: 
 *   SVLR_OK            SVL database successfully dumped.
 *   SVLR_INV_HANDLE    Bad handle.
 *   SVLR_INV_ARG       'pt_mem_dump' is NULL or SVL metadata not match.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_load_from_mem
(
    HANDLE_T               h_svl,
    SVL_MEM_DUMP_T*        pt_mem_dump
)
{
    INT32           i4_rc;
    SVL_OBJ_T*      pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pt_mem_dump != NULL )
        {
            if (pt_mem_dump->ui2_svl_id == pt_svl->ui2_svl_id &&
                x_strcmp(pt_mem_dump->ps_svl_name, pt_svl->ps_svl_name) == 0)
            {
                i4_rc = db_load_from_mem(pt_svl->h_cdb,
                                         pt_mem_dump->pv_data,
                                         pt_mem_dump->ui4_data_len);
                i4_rc = svl_cdb_to_svl_err_code(i4_rc);
            }
            else
            {
                i4_rc = SVLR_INV_ARG;
            }
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }

    SVL_API_LOG("{svl}%s(0x%x)=%d\n",__FUNCTION__,h_svl,i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_fs_store_given_file_handle
 *
 * Description: This API stores the specified SVL object to the 
 *              file system.
 *
 * Inputs:
 *   h_dir         The handle to the file.
 *
 *  ui2_svl_id     Specifies the SVL object to be stored to the file system.  
 *                 The limit for SVL ID is between 1 and 65535.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK             SVL object is stored to the file system. 
 *
 *   SVLR_NOT_FOUND      The SVL object specified by the 'ui2_svl_id' 
 *                       does not exist.
 *
 *   SVLR_INV_ARG        ps_svl_path' is NULL.
 *
 *   SVLR_INV_FILE_PATH  Specified file path is not a directory or 
 *                       a RAW character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_fs_store_given_file_handle
(
    HANDLE_T        h_file,
    UINT16          ui2_svl_id  
)
{
    INT32       i4_rc;

    SVL_API_LOG("{svl}%s(file:0x%x,%d) begin\n",__FUNCTION__,h_file,ui2_svl_id);
    
    if ( ui2_svl_id  != 0 )
    {
        DBG_API(("{SVL} saving SVL data.\n"));
        
        i4_rc=svl_fs_store_object_given_file_handle(h_file,ui2_svl_id);

        DBG_API(("{SVL} done saving SVL data [ret code: %d]\n",i4_rc));
    }
    else
    {
        i4_rc = SVLR_INV_ARG;
    }

    if(s_b_svl_rec_dump)
    {
        svl_dump_rec(ui2_svl_id,BRDCST_TYPE_UNKNOWN);
    }


    SVL_API_LOG("{svl}%s(file:0x%x,%d)=%d end\n",__FUNCTION__,h_file,ui2_svl_id,i4_rc);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_svl_fs_sync_given_file_handle
 *
 * Description: This API sync a specified SVL handle with SVL records
 *              from persistent file system.  The existing SVL records
 *              are replaced with new records read from file. 
 *
 * Inputs:
 *
 *   h_svl          Specify the handle to the SVL object.
 *
 *   h_file         The handle to the file.
 *
 *   ui2_svl_id     Specifies the SVL id to load from the file system.  
 *                  The limit for SVL ID is between 1 and 65535.
 *
 * Outputs: 
 *
 *   None.
 *                 
 * Returns: 
 *   SVLR_OK             SVL records are load from the persistent file
 *                       into the memory of SVL object.
 *
 *   SVLR_NOT_FOUND      'ui2_svl_id' is not found or 'ps_svl_path' 
 *                       does not exist.
 *
 *   SVLR_FAIL           internal error
 *
 *   SVLR_INV_ARG        'ps_svl_path' is NULL, or 'h_svl' is NULL_HANDLE,
 *                       or ui2_svl_id is 0.
 *
 *   SVLR_INV_FILE_PATH  Specified file path is not a directory or a RAW 
 *                       character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_svl_fs_sync_given_file_handle
(
    HANDLE_T		h_svl,	
    HANDLE_T        h_file,
    UINT16		    ui2_svl_id
)
{
    INT32           i4_rc;

    SVL_API_LOG("{svl}%s(0x%x, file:0x%x,%d) begin\n",__FUNCTION__,h_svl,h_file,ui2_svl_id);
    
    if ( h_svl       != NULL_HANDLE     &&
         ui2_svl_id  != 0   
        )
    {
        i4_rc=svl_fs_sync_object_given_file_handle(h_svl,
                                                   h_file,
                                                   ui2_svl_id);
    }
    else
    {
        i4_rc = SVLR_INV_ARG;
    }

    if(s_b_svl_rec_dump)
    {
    	svl_dump_rec(ui2_svl_id,BRDCST_TYPE_UNKNOWN);
    }


    SVL_API_LOG("{svl}%s(0x%x, file:0x%x,%d)=%d end\n",__FUNCTION__,h_svl,h_file,ui2_svl_id,i4_rc);
    return i4_rc;
}


INT32 x_svl_get_multi_lang
(
    HANDLE_T               h_svl,
    UINT16                 ui2_svl_rec_id,
    SVL_MULTI_LANG_T *     pt_multi_lang,
    UINT32*                pui4_ver_id
)
{
    INT32                  i4_rc;
    SVL_OBJ_T*             pt_svl;
    HANDLE_TYPE_T          pe_type;
    
    UINT8                  ui1_key_count;
    DB_FIELD_T             aui4_keyfld_type[1];
    const VOID*            apv_keyfld_addr[1];
    
    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pui4_ver_id    != NULL &&
             pt_multi_lang != NULL &&
             ui2_svl_rec_id != SVL_NULL_REC_ID   )
        {
            /* Set up arguments to read record by record_id value. */
            ui1_key_count = 1;
            aui4_keyfld_type[0]      = (DB_FIELD_T) SVL_CDB_TYPE_REC_ID ;
            apv_keyfld_addr[0]       = &ui2_svl_rec_id;
            
            i4_rc = svl_get_multi_lang(pt_svl,
                                                ui1_key_count,
                                                aui4_keyfld_type,
                                                apv_keyfld_addr,
                                                NULL,
                                                pt_multi_lang,
                                                0,
                                                pui4_ver_id);           
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
        i4_rc = SVLR_INV_HANDLE;
    }
    return i4_rc;
}

INT32 x_svl_set_multi_lang
(
    HANDLE_T               h_svl,
    UINT16                 ui2_svl_rec_id,
    SVL_MULTI_LANG_T *     pt_multi_lang
)
{
    INT32           i4_rc;
    SVL_OBJ_T*      pt_svl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_svl,&pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                    &&
         (pt_svl  != NULL)
        )
    {
        if ( pt_multi_lang != NULL )
        {          
            i4_rc=svl_set_multi_lang(pt_svl,
                                              ui2_svl_rec_id,
                                              pt_multi_lang);          
        }
        else
        {
            i4_rc=SVLR_INV_ARG;
        }
    }
    else
    {
        i4_rc = SVLR_INV_HANDLE;
    }

    SVL_API_LOG("{svl}%s(0x%x,%d)=%d\n",__FUNCTION__,h_svl,ui2_svl_rec_id,i4_rc);
    return i4_rc;
}



void svl_enable_api_log(BOOL b_enable)
{
	s_b_svl_api_log = b_enable;
}

void svl_enable_rec_dmp(BOOL b_enable)
{
	s_b_svl_rec_dump = b_enable;
}



