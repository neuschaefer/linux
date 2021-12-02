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
 * $RCSfile:$
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor: Jun Zuo  $
 *
 * Description: 
 *    This  file contains implementation of the SATL's application API's. 
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "satl/x_satl.h"
#include "handle/handle.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

/*-------------------------------------------------------------------------
                    functions implementations
 --------------------------------------------------------------------------*/
static INT32 _hndlr_x_satl_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_VOID;
    
      ((x_satl_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
          (SATL_COND_T)pt_args[1].u.ui4_arg,
                      pt_args[2].u.ui4_arg,
                      pt_args[3].u.pv_arg,
                      pt_args[4].u.ui4_arg);
                      
    return RPCR_OK;
}

static INT32 _x_satl_create
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
    RPC_DECL(7, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_satl_name));
    
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_satl_name);
    RPC_ARG_INP(ARG_TYPE_UINT16,      ui2_satl_id);
    RPC_ARG_INP(ARG_TYPE_UINT8,     e_mem_policy);
    RPC_ARG_INP(ARG_TYPE_UINT16,  ui2_max_nb_rec);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,        pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,        pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,      ph_satl);

    RPC_DO_OP("x_satl_create");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

static INT32 _x_satl_fs_load
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
    RPC_DECL(7, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_satl_path));
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_satl_name));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_satl_path);
    RPC_ARG_INP(ARG_TYPE_UINT16,   ui2_satl_id);
    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_satl_name);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,     pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,     pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,   ph_satl);

    RPC_DO_OP("x_satl_fs_load");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

static INT32 _x_satl_open
(
    UINT16              ui2_satl_id,
    VOID*               pv_tag,
    x_satl_nfy_fct      pf_nfy,
    HANDLE_T*           ph_satl
)
{
    RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_satl_id);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_satl);

    RPC_DO_OP("x_satl_open");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

static INT32 _x_satl_open_by_name
(
    const CHAR*         ps_satl_name,
    VOID*               pv_tag,
    x_satl_nfy_fct      pf_nfy,
    HANDLE_T*           ph_satl
)
{
    RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_satl_name);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_satl);

    RPC_DO_OP("x_satl_open_by_name");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_satl_create
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
 *   SATLR_AEE_OUT_OF_RESOURCES Can not allocate AEE resources.
 *   SATLR_AEE_NO_RIGHTS No right for the AEE resources. 
 ----------------------------------------------------------------------------*/
INT32 c_satl_create
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
    INT32     i4_rc;

    i4_rc = _x_satl_create(ps_satl_name, ui2_satl_id, e_mem_policy,
                          ui2_max_nb_rec, pv_tag, pf_nfy, ph_satl);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_satl_delete
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
INT32 c_satl_delete
(
    HANDLE_T    	h_satl
)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_satl);

    RPC_DO_OP("x_satl_delete");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_clean
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
INT32 c_satl_clean
(
    HANDLE_T    	h_satl
)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_satl);

    RPC_DO_OP("x_satl_clean");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_set_src_name
 *
 * Description: This function sets the tuner source name for all the 
 *              satellite records in this SATL.    
 *
 * Inputs: 
 *   h_satl       Handle to the SATL object.
 *   ps_src_name  Pointer to the tuner source name.  This API will make 
 *                a copy of this source name and store it internally.  
 *                The length of the 'source name' is limited to 
 *                (MAX_SATL_SRC_NAME_LEN+1) bytes.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          Tuner source name is successfully stored.
 *   SATLR_INV_HANDLE  Bad SATL handle.
 *   SATLR_INV_ARG     'ps_src_name' is null. 
 -----------------------------------------------------------------------*/
INT32 c_satl_set_src_name
(
    HANDLE_T            h_satl,
    const CHAR*         pt_src_name
)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_satl);
    RPC_ARG_INP(ARG_TYPE_REF_STR, pt_src_name);

    RPC_DO_OP("x_satl_set_src_name");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_add_rec
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
INT32 c_satl_add_rec
(
    HANDLE_T            h_satl,
    const SATL_REC_T*   pt_satl_rec
)
{
    RPC_DECL(2, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_satl_rec, RPC_DESC_SATL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_satl);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_satl_rec);

    RPC_DO_OP("x_satl_add_rec");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_del_rec
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
INT32 c_satl_del_rec
(
    HANDLE_T            h_satl,
    UINT16              ui2_satl_rec_id
)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,          h_satl);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_satl_rec_id);

    RPC_DO_OP("x_satl_del_rec");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_update_rec
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
INT32 c_satl_update_rec
(
    HANDLE_T            h_satl,
    const SATL_REC_T*   pt_satl_rec,
    BOOL                b_must_exist	
)
{
    RPC_DECL(3, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_satl_rec, RPC_DESC_SATL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_satl);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_satl_rec);
    RPC_ARG_INP(ARG_TYPE_BOOL,   b_must_exist);

    RPC_DO_OP("x_satl_update_rec");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_qry_rec_id
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
INT32 c_satl_qry_rec_id
(
    HANDLE_T            h_satl,
    SATL_REC_ID_QRY_T   e_satl_qry_option,
    UINT16*             pui2_rec_id	
)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_satl);
    RPC_ARG_INP(ARG_TYPE_UINT8, e_satl_qry_option);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_rec_id);

    RPC_DO_OP("x_satl_qry_rec_id");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_lock
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
INT32 c_satl_lock
(
    HANDLE_T            h_satl
)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_satl);

    RPC_DO_OP("x_satl_lock");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_unlock
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
INT32 c_satl_unlock
(
    HANDLE_T            h_satl
)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_satl);

    RPC_DO_OP("x_satl_unlock");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_read_lock
 *
 * Description: This function acquires the read-lock on the SATL object.
 *    The function blocks until the lock is acquired.
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
INT32 c_satl_read_lock
(
    HANDLE_T            h_satl
)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_satl);

    RPC_DO_OP("x_satl_read_lock");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_read_unlock
 *
 * Description: This function releases the read-lock on the SATL database. 
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
INT32 c_satl_read_unlock
(
    HANDLE_T            h_satl
)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_satl);

    RPC_DO_OP("x_satl_read_unlock");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_fs_load
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
 *   SATLR_AEE_OUT_OF_RESOURCES Can not allocate AEE resources.
 *   SATLR_AEE_NO_RIGHTS No right for the AEE resources. 
 -----------------------------------------------------------------------*/
INT32 c_satl_fs_load
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
    INT32     i4_rc;

    i4_rc = _x_satl_fs_load(h_dir, ps_satl_path, ui2_satl_id,
                          ps_satl_name, pv_tag, pf_nfy, ph_satl);
            
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_satl_fs_store
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
INT32 c_satl_fs_store
(
    HANDLE_T            h_dir,
    const CHAR*         ps_satl_path,
    UINT16              ui2_satl_id
)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_satl_path);
    RPC_ARG_INP(ARG_TYPE_UINT16,   ui2_satl_id);

    RPC_DO_OP("x_satl_fs_store");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_fs_del
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
INT32 c_satl_fs_del
(
    HANDLE_T            h_dir,
    const CHAR*         ps_satl_path,
    UINT16              ui2_satl_id
)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR, ps_satl_path);
    RPC_ARG_INP(ARG_TYPE_UINT16,   ui2_satl_id);

    RPC_DO_OP("x_satl_fs_del");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_fs_qry
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
INT32 c_satl_fs_qry
(
    HANDLE_T            h_dir,
    const CHAR*         ps_satl_path,
    UINT16              ui2_idx,
    UINT16*             pui2_satl_id,
    CHAR*               ps_satl_name
)
{
    RPC_DECL(5, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_satl_path);
    RPC_ARG_INP(ARG_TYPE_UINT16,         ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_satl_id);
    RPC_ARG_OUT(ARG_TYPE_REF_STR,    ps_satl_name);

    RPC_DO_OP("x_satl_fs_qry");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_fs_sync
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
INT32 c_satl_fs_sync
(
    HANDLE_T		h_satl,
    HANDLE_T        h_dir,
    const CHAR*	    ps_satl_path,
    UINT16		    ui2_satl_id
)
{
    RPC_DECL(4, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_satl);
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_satl_path);
    RPC_ARG_INP(ARG_TYPE_UINT16,      ui2_satl_id);

    RPC_DO_OP("x_satl_fs_sync");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
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
 *   SATLR_OK                   SATL handle is returned. 
 *   SATLR_OUT_OF_HANDLE        No more handle available.
 *   SATLR_OUT_OF_MEM           Not enough memory to create the SATL control 
 *                              structure.
 *   SATLR_NOT_FOUND            The specified SATL database object id (e.g. 
 *                              'ui2_satl_id') is not found.
 *   SATLR_INV_ARG              'ph_satl' is NULL.
 *   SATLR_AEE_OUT_OF_RESOURCES Can not allocate AEE resources.
 *   SATLR_AEE_NO_RIGHTS        No right for the AEE resources.  
 -----------------------------------------------------------------------*/
INT32 c_satl_open
(
    UINT16              ui2_satl_id,
    VOID*               pv_tag,
    x_satl_nfy_fct      pf_nfy,
    HANDLE_T*           ph_satl
)
{
    INT32     i4_rc;

    i4_rc = _x_satl_open(ui2_satl_id, pv_tag, pf_nfy, ph_satl);
    
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_satl_open_by_name
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
 *   SATLR_OK                   SATL handle is returned. 
 *   SATLR_OUT_OF_HANDLE        No more handle available.
 *   SATLR_OUT_OF_MEM           Not enough memory to create the SATL
 *                              handle structure.
 *   SATLR_NOT_FOUND            The specified SATL database object name (e.g., 
 *                              ps_satl_name) is not found.
 *   SATLR_INV_ARG              'ph_satl' or 'ps_satl_name' is NULL.
 *   SATLR_AEE_OUT_OF_RESOURCES Can not allocate AEE resources.
 *   SATLR_AEE_NO_RIGHTS        No right for the AEE resources.     
 -----------------------------------------------------------------------*/
INT32 c_satl_open_by_name
(
    const CHAR*         ps_satl_name,
    VOID*               pv_tag,
    x_satl_nfy_fct      pf_nfy,
    HANDLE_T*		    ph_satl
)
{
    INT32     i4_rc;

    i4_rc = _x_satl_open_by_name(ps_satl_name, pv_tag, pf_nfy, ph_satl);

    return i4_rc;    
}

/*----------------------------------------------------------------------
 * Name: c_satl_close
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
INT32 c_satl_close
(
    HANDLE_T            h_satl
)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_satl);

    RPC_DO_OP("x_satl_close");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_get_handle_info
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
INT32 c_satl_get_handle_info
(
    HANDLE_T            h_satl,
    UINT16*             pui2_satl_id,
    CHAR*               ps_satl_name
)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_satl);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_satl_id);
    RPC_ARG_OUT(ARG_TYPE_REF_STR,    ps_satl_name);

    RPC_DO_OP("x_satl_get_handle_info");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_get_rec
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
 *   pt_satl_rec  Pointer to a service record structure.   This structure must 
 *                be allocated by the caller.  On return, it is filled with 
 *                service record information.
 *   pui4_ver_id  Pointer to a variable holding version id of the database 
 *                object.   Note: version id of the SATL database object is 
 *                incremented each time when a modification takes place.
 * Returns: 
 *   SATLR_OK   Data from the specified record is returned. 
 *   SATLR_INV_HANDLE   Bad handle.
 *   SATLR_REC_NOT_FOUND No record matching the search parameter 
 *                      ('ui2_satl_rec_id') is found.
 *   SATLR_INV_ARG      'pui4_ver_id' or 'pt_satl_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_satl_get_rec
(
    HANDLE_T            h_satl,
    UINT16              ui2_satl_rec_id,
    SATL_REC_T*         pt_satl_rec,
    UINT32*             pui4_ver_id
)
{
    RPC_DECL(4, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_satl_rec, RPC_DESC_SATL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_satl);
    RPC_ARG_INP(ARG_TYPE_UINT16,  ui2_satl_rec_id);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pt_satl_rec);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_ver_id);

    RPC_DO_OP("x_satl_get_rec");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_get_src_name
 *
 * Description: This API returns the source name associated with all 
 *              the satellite records for this SATL.
 *
 * Inputs: 
 *   h_satl       Handle to the SATL object.
 *
 * Outputs: 
 *   ps_src_name  Pointer to a char buffer (with size at least 
 *                MAX_SATL_NAME_LEN+1 bytes).  On return, this buffer 
 *                will be filled with SATL source name.
 * Returns: 
 *   SATLR_OK          SATL source name is returned. 
 *   SATLR_INV_HANDLE  Bad 'h_satl' handle.
 *   SATLR_INV_ARG     'pt_src_name' is null. 
 -----------------------------------------------------------------------*/
INT32 c_satl_get_src_name
(
    HANDLE_T            h_satl,
    CHAR*               ps_src_name 
)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_satl);
    RPC_ARG_OUT(ARG_TYPE_REF_STR,    ps_src_name);

    RPC_DO_OP("x_satl_get_src_name");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_get_num_recs
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
INT32 c_satl_get_num_recs
(
    HANDLE_T            h_satl,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,             h_satl);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_num_recs);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,   pui4_ver_id);

    RPC_DO_OP("x_satl_get_num_recs");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_rec_init
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
INT32 c_satl_rec_init
(
    SATL_REC_T*		pt_satl_rec
)
{
    RPC_DECL(1, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_satl_rec, RPC_DESC_SATL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_REF_DESC,    pt_satl_rec);

    RPC_DO_OP("x_satl_rec_init");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}


/*----------------------------------------------------------------------
 * Name: c_satl_set_cursor
 *
 * Description: This API specifies a entry position in SATL records, 
 *   which will read when API c_satl_read_cursor() is called.
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
INT32 c_satl_set_cursor
(
    HANDLE_T            h_satl,
    SATL_ITERATE_T      e_pos
)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_satl);
    RPC_ARG_INP(ARG_TYPE_UINT8,  e_pos);

    RPC_DO_OP("x_satl_set_cursor");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_read_cursor
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
INT32 c_satl_read_cursor
(
    HANDLE_T            h_satl,
    SATL_REC_T*         pt_satl_rec,
    UINT32*             pui4_ver_id
)
{
    RPC_DECL(3, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_satl_rec, RPC_DESC_SATL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_satl);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pt_satl_rec);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_ver_id);

    RPC_DO_OP("x_satl_read_cursor");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

/*----------------------------------------------------------------------
 * Name: c_satl_sort_rec
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
INT32 c_satl_sort_rec
(
    HANDLE_T			h_satl,
    UINT32*             pui4_ver_id 
)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,           h_satl);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_ver_id);

    RPC_DO_OP("x_satl_sort_rec");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_INV_ARG);
}

INT32 c_satl_init_mem_dump(SATL_MEM_DUMP_T * pt_mem_dump)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_mem_dump,  RPC_DESC_SATL_MEM_DUMP_T, NULL));

    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_mem_dump);

    RPC_DO_OP("x_satl_init_mem_dump");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_FAIL);
}

INT32 c_satl_free_mem_dump(SATL_MEM_DUMP_T * pt_mem_dump)
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_mem_dump,  RPC_DESC_SATL_MEM_DUMP_T, NULL));

    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_mem_dump);

    RPC_DO_OP("x_satl_free_mem_dump");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_FAIL);
}

INT32 c_satl_dump_to_mem(HANDLE_T h_satl,
                         SATL_MEM_DUMP_T * pt_mem_dump)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_mem_dump,  RPC_DESC_SATL_MEM_DUMP_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_satl);
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_mem_dump);

    RPC_DO_OP("x_satl_dump_to_mem");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_FAIL);
}

INT32 c_satl_load_from_mem(HANDLE_T h_satl,
                           SATL_MEM_DUMP_T * pt_mem_dump)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_mem_dump,  RPC_DESC_SATL_MEM_DUMP_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_satl);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_mem_dump);

    RPC_DO_OP("x_satl_load_from_mem");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_FAIL);
}

INT32 c_satl_get_rec_by_rec_idx(HANDLE_T h_satl,
                                UINT16 ui2_idx,
                                SATL_REC_T * pt_satl_rec,
                                UINT32 * pui4_ver_id)
{
    RPC_DECL(4, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_satl_rec, RPC_DESC_SATL_REC_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32, h_satl);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_idx);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_satl_rec);
    RPC_ARG_IO(ARG_TYPE_REF_UINT32, pui4_ver_id);

    RPC_DO_OP("x_satl_get_rec_by_rec_idx");

    RPC_RETURN(ARG_TYPE_INT32, SATLR_FAIL);
}

INT32 c_rpc_reg_satl_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_satl_nfy_fct);   

    return RPCR_OK;
}

