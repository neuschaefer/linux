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
 * $RCSfile: satl_init.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements init service list (SATL) database function.
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
#include "satl/satl.h"
#include "satl/_satl.h"
#include "satl/satl_cli.h"

/* Internal static variable. */
static UINT16          ui2_max_satl_obj=0;

/* Internal array for the SATL name-id pairs. */
static SATL_ID_NAME_T* pt_id_name_ary=NULL;

/* Internal global lock. */
static HANDLE_T        h_satl_lock;


/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/
static INT32 satl_handle_autofree
(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type
)
{
    return handle_free(h_handle, FALSE);
}

static handle_autofree_fct   apf_autofree_fct[]=
{
     satl_handle_autofree   
};

/*-----------------------------------------------------------------------------
 * Name: satl_init
 *
 * Description: This function initializes the SATL library. 
 *
 * Inputs: 
 *    ui2_max_num_satl_obj: Maximum number of SATL object that can be created.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          SATL library is initialized successfully.
 *   SATLR_INV_ARG     'ui2_max_num_satl_obj' is 0.
 *
 ----------------------------------------------------------------------------*/
INT32 satl_init
(
    UINT16           ui2_max_num_satl_obj
) 
{
    if ( ui2_max_num_satl_obj == 0 )
    {
        ui2_max_num_satl_obj = 8;
    }
    
    if ( sizeof(SATL_REC_T) > SATL_REC_SIZE )
    {
         x_dbg_stmt("SATL record size in memory: %d  pre-set SATL_REC_T size: %d \n",
                   sizeof(SATL_REC_T), SATL_REC_SIZE);
         
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_REC_SIZE_TOO_LARGE);
    }

    if ( ui2_max_num_satl_obj > 0 )
    {
        pt_id_name_ary=(SATL_ID_NAME_T*)x_mem_calloc (ui2_max_num_satl_obj,
                                                     sizeof(SATL_ID_NAME_T));
        if ( pt_id_name_ary != NULL )
        {
            ui2_max_satl_obj=ui2_max_num_satl_obj;

            if (x_sema_create( &(h_satl_lock), X_SEMA_TYPE_MUTEX,
                               X_SEMA_STATE_UNLOCK) != OSR_OK)
            {
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
            }

            /* register autofree function for the SATL handle. */
            if ( handle_set_autofree_tbl(HT_GROUP_SATL,
                                         apf_autofree_fct)
                 != HR_OK )
            {
                ABORT(DBG_CAT_INV_OP,DBG_ABRT_CANNOT_SET_AUTOFREE);
            }
                
#ifdef CLI_LVL_ALL
            /*
              Initialize CLI support
            */
            satl_cli_init();
#endif                   
        }
        else
        {
            ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEMORY);
        }
    }
    else
    {
        return SATLR_INV_ARG ;
    }
    return SATLR_OK;
}

/**********************************************************************
   Following are private functions only known to the SATL library.
   The private functions are for adding new SATL id/name, deleting
   SATL id/name, from the internal SATL id/name arrays.  They are
   in this file because these private functions need to access
   the internal SATL id/name arrays

***********************************************************************/

/*-----------------------------------------------------------------------------
 * Name: satl_add_id_name
 *
 * Description: This function add new SATL ID/NAME entry to the internal
 *              SATL library. 
 *
 * Inputs: 
 *    ui2_satl_id: SATL ID.
 *    s_satl_name: SATL Name.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK      SATL ID/Name is added.
 *   SATLR_OUT_OF_RES Can not add the new SATL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 satl_add_id_name(UINT16 ui2_satl_id, const CHAR* s_satl_name)
{
    UINT16 ui2_i;
    
    for ( ui2_i = 0 ; ui2_i < ui2_max_satl_obj ; ui2_i++ )
    {
        /* Look for an empty slot. */
        if ( pt_id_name_ary[ui2_i].ui2_satl_id == SATL_NULL_ID )
        {
            pt_id_name_ary[ui2_i].ui2_satl_id=ui2_satl_id;
            x_strcpy(pt_id_name_ary[ui2_i].s_satl_name,s_satl_name);
            return SATLR_OK;
        }
    }    
    return SATLR_FAIL;
}

/*-----------------------------------------------------------------------------
 * Name: satl_check_id_name
 *
 * Description: This function check the SATL ID/NAME entry. 
 *
 * Inputs: 
 *    ui2_satl_id: SATL ID.
 *    s_satl_name: SATL Name.
 *
 * Outputs: None
 *
 * Returns: 
 *   TRUE      SATL ID/Name is not currently in the database..
 *   FALSE     Invalid SATL ID/Name.
 ----------------------------------------------------------------------------*/
BOOL satl_check_id_name(UINT16 ui2_satl_id, const CHAR* s_satl_name)
{
    UINT16 ui2_i;
    for ( ui2_i = 0 ; ui2_i < ui2_max_satl_obj ; ui2_i++ )
    {
        if ( pt_id_name_ary[ui2_i].ui2_satl_id != SATL_NULL_ID )
        {
            if ( pt_id_name_ary[ui2_i].ui2_satl_id == ui2_satl_id )
            {
                return FALSE;
            }
            if ( x_strcmp(pt_id_name_ary[ui2_i].s_satl_name,s_satl_name) == 0 )
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: satl_del_id
 *
 * Description: This function deletes SATL ID/NAME entry from the internal
 *              SATL library. 
 *
 * Inputs: 
 *    ui2_satl_id: SATL ID.
 *    s_satl_name: SATL Name.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK      SATL entry is deleted.
 *
 ----------------------------------------------------------------------------*/
INT32 satl_del_id(UINT16 ui2_satl_id)
{
    UINT16 ui2_i;
    INT32 i4_rc;

    i4_rc = SATLR_NOT_FOUND;
    if ( ui2_satl_id != SATL_NULL_ID )
    {
        for ( ui2_i = 0 ; ui2_i < ui2_max_satl_obj ; ui2_i++ )
        {
            /* Look for SATL ID. */
            if ( pt_id_name_ary[ui2_i].ui2_satl_id == ui2_satl_id )
            {
                pt_id_name_ary[ui2_i].ui2_satl_id = SATL_NULL_ID;
                pt_id_name_ary[ui2_i].s_satl_name[0]='\0';
                i4_rc = SATLR_OK;
                break;
            }
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_get_name_by_id
 *
 * Description: This function search for specific SATL ID and return the SATL
 *              NAME.
 *
 * Inputs: 
 *    ui2_satl_id: SATL ID.
 *
 * Outputs:
 *    s_satl_name: SATL Name.
 *
 * Returns: 
 *   TRUE       SATL ID is found.
 *   FALSE      SATL ID is not found.
 ----------------------------------------------------------------------------*/
BOOL satl_get_name_by_id(UINT16 ui2_satl_id, CHAR* s_satl_name)
{
    UINT16 ui2_i;
    for ( ui2_i = 0 ; ui2_i < ui2_max_satl_obj ; ui2_i++ )
    {
        if ( ui2_satl_id == pt_id_name_ary[ui2_i].ui2_satl_id )
        {
            x_strcpy(s_satl_name,pt_id_name_ary[ui2_i].s_satl_name);
            return TRUE;
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: satl_get_id_by_name
 *
 * Description: This function search for specific SATL Name and return the SATL
 *              ID.
 *
 * Inputs: 
 *    s_satl_name: SATL Name.
 *
 * Outputs:
 *    ui2_satl_id: SATL ID.
 *
 * Returns: 
 *   TRUE          SATL Name is found.
 *   FALSE         SATL Name is not found.
 ----------------------------------------------------------------------------*/
BOOL satl_get_id_by_name(const CHAR* s_satl_name, UINT16* pui2_satl_id)
{
    UINT16 ui2_i;
    for ( ui2_i = 0 ; ui2_i < ui2_max_satl_obj ; ui2_i++ )
    {
        if ( x_strcmp(pt_id_name_ary[ui2_i].s_satl_name,s_satl_name) == 0 )
        {
            *pui2_satl_id = pt_id_name_ary[ui2_i].ui2_satl_id;
            return TRUE;
        }
    }
    *pui2_satl_id=SATL_NULL_ID;
    return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name: satl_get_id_and_name_by_idx
 *
 * Description: This function get a SATL ID and its Name.
 *
 * Inputs: 
 *    s_satl_name: SATL Name.
 *
 * Outputs:
 *    ui2_satl_id: SATL ID.
 *
 * Returns: 
 *   TRUE          SATL ID and Name is found.
 *   FALSE         SATL Name is not found.
 ----------------------------------------------------------------------------*/
INT32 satl_get_id_and_name_by_idx
(
    UINT16  ui2_i,
    CHAR*   ps_satl_name,
    UINT16* pui2_satl_id
)
{
    INT32   i4_rc ;
    
    if ( pt_id_name_ary[ui2_i].ui2_satl_id != SATL_NULL_ID )
    {
        *pui2_satl_id = pt_id_name_ary[ui2_i].ui2_satl_id;
        x_strncpy(ps_satl_name, pt_id_name_ary[ui2_i].s_satl_name, MAX_SATL_NAME_LEN+1);
        i4_rc = SATLR_OK;
    }
    else
    {
        i4_rc = SATLR_FAIL;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_sema_lock
 *
 * Description: This function get the sema for the SATL library. 
 *
 * Inputs: 
 *    None.
 *
 * Outputs:
 *    None.
 *
 * Returns: 
 *   TRUE       SATL ID is found.
 *   FALSE      SATL ID is not found.
 ----------------------------------------------------------------------------*/
VOID satl_sema_lock(VOID)
{
    if ( x_sema_lock( h_satl_lock, X_SEMA_OPTION_WAIT) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: satl_sema_unlock
 *
 * Description: This function release the sema for the SATL library. 
 *
 * Inputs: 
 *    None.
 *
 * Outputs:
 *    None.
 *
 * Returns: 
 *   TRUE       SATL ID is found.
 *   FALSE      SATL ID is not found.
 ----------------------------------------------------------------------------*/
VOID satl_sema_unlock(VOID)
{
    if ( x_sema_unlock( h_satl_lock) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE);
    }
}

