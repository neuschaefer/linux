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
 * $RCSfile: lol_init.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements initialization function of Large Object list (LOL)
 *    module.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "os/inc/x_os.h"
#include "cdb/cdb_api.h"
#include "lol/lol.h"
#include "lol/_lol.h"
#include "lol/lol_rec.h"
#include "lol/lol_cli.h"

/* Internal static variable. */
static UINT16          _s_ui2_max_lol_obj = 0;

/* Internal array for the LOL name-id pairs. */
static LOL_ID_NAME_T*  pt_id_name_ary=NULL;

/* Internal global lock. */
static HANDLE_T        h_lol_lock;


/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/
static INT32 lol_handle_autofree
(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type
)
{
    return handle_free(h_handle, FALSE);
}

static handle_autofree_fct   apf_autofree_fct[]=
{
     lol_handle_autofree    
};

/*-----------------------------------------------------------------------------
 * Name: lol_init
 *
 * Description: This function initializes the LOL library. 
 *
 * Inputs: 
 *    ui2_max_num_lol_obj:  Maximum number of LOL objects that can be created.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           LOL module is initialized successfully.
 *   LOLR_INV_ARG      'ui2_max_num_lol_obj' is 0.
 *
 ----------------------------------------------------------------------------*/
INT32 lol_init 
(
    UINT16           ui2_max_num_lol_obj
)
{
    if ( ui2_max_num_lol_obj == 0 )
    {
        ui2_max_num_lol_obj = 8;
    }
    
    /*
      Check the size of the LOL record structure, make sure it is not
      larger than the pre-set size.
    */
    if ( sizeof(LOL_REC_T) <= LOL_REC_SIZE )
    {        
        if ( ui2_max_num_lol_obj > 0 )
        {
            /* allocate array for storing lol_id and name pair. */
            pt_id_name_ary=(LOL_ID_NAME_T*)x_mem_calloc (ui2_max_num_lol_obj,
                                         sizeof(LOL_ID_NAME_T));
            if ( pt_id_name_ary != NULL )
            {
                _s_ui2_max_lol_obj = ui2_max_num_lol_obj;

                if (x_sema_create( &(h_lol_lock), X_SEMA_TYPE_MUTEX,
                                   X_SEMA_STATE_UNLOCK) != OSR_OK)
                {
                    ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
                }
                
                /* register autofree function for the SVL handle. */
                if ( handle_set_autofree_tbl(HT_GROUP_LOL,
                                             apf_autofree_fct)
                     != HR_OK )
                {
                    ABORT(DBG_CAT_INV_OP,DBG_ABRT_CANNOT_SET_AUTOFREE);
                }
                
#if defined(CLI_LVL_ALL) || defined(CLI_STRESS_MODE)
                /*
                  Initialize CLI support
                */
                lol_cli_init();
#endif   
            }
            else
            {
                ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEMORY);
            }
        }
        else
        {
            return LOLR_INV_ARG ;
        }
    }
    else
    {
         x_dbg_stmt("LOL record size in memory: %d  pre-set LOL_REC_T size: %d \n",
                   sizeof(LOL_REC_T), LOL_REC_SIZE);
         
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_REC_SIZE_TOO_LARGE);
    }
    
    return LOLR_OK;
}

/**********************************************************************
   Following are private functions only known to the LOL library.
   The private functions are for adding new LOL id/name pair, deleting
   LOL id/name pair, from the internal LOL id/name array.  They are
   in this file because these private functions need to access
   the internal LOL id/name arrays

***********************************************************************/

/*-----------------------------------------------------------------------------
 * Name: lol_add_id_name
 *
 * Description: This function add new SVL ID/NAME pair entry to the internal
 *              LOL module. 
 *
 * Inputs: 
 *    ui2_lol_id:  LOL ID.
 *    s_lol_name:  LOL Name.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK       LOL ID/Name is added.
 *   LOLR_OUT_OF_RES  Can not add the new LOL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 lol_add_id_name(UINT16 ui2_lol_id, const CHAR* s_lol_name)
{
    INT16 ui2_i;

    if ( ui2_lol_id == LOL_NULL_ID  ||  s_lol_name == NULL )
    {
        return LOLR_INV_ARG;
    }
    
    for ( ui2_i = 0 ; ((UINT16)ui2_i) < _s_ui2_max_lol_obj ; ui2_i++ )
    {
        /* Look for an empty slot. */
        if ( pt_id_name_ary[ui2_i].ui2_lol_id == LOL_NULL_ID )
        {
            pt_id_name_ary[ui2_i].ui2_lol_id=ui2_lol_id;
            x_strcpy(pt_id_name_ary[ui2_i].s_lol_name,s_lol_name);
            return LOLR_OK;
        }
    }
    return LOLR_OUT_OF_SPACE;
}

/*-----------------------------------------------------------------------------
 * Name: lol_check_id_name
 *
 * Description: This function check the LOL ID/NAME entry. 
 *
 * Inputs: 
 *    ui2_lol_id:  lol ID.
 *    s_lol_name:  lol Name.
 *
 * Outputs: None
 *
 * Returns: 
 *   TRUE      lol id/name  is not currently in the database.
 *   FALSE     Invalid lol id/name (id or name already existed).
 ----------------------------------------------------------------------------*/
BOOL lol_check_id_name(UINT16 ui2_lol_id, const CHAR* s_lol_name)
{
    INT16 ui2_i;
    
    for ( ui2_i = 0 ; ((UINT16)ui2_i) < _s_ui2_max_lol_obj ; ui2_i++ )
    {
        if ( pt_id_name_ary[ui2_i].ui2_lol_id != LOL_NULL_ID )
        {
            if ( pt_id_name_ary[ui2_i].ui2_lol_id == ui2_lol_id )
            {
                return FALSE;
            }
            if ( x_strcmp(pt_id_name_ary[ui2_i].s_lol_name,s_lol_name) == 0 )
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: lol_del_id
 *
 * Description: This function deletes LOL ID/NAME entry from the internal
 *              LOL module. 
 *
 * Inputs: 
 *    ui2_lol_id:  LOL ID.
 *    s_lol_name:  LOL Name.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK          LOL entry is deleted.
 *   LOLR_NOT_FOUND   Specified LOL entry not found.
 ----------------------------------------------------------------------------*/
INT32 lol_del_id(UINT16 ui2_lol_id)
{
    INT16 ui2_i;
    INT32 i4_rc;

    i4_rc = LOLR_NOT_FOUND;
    if ( ui2_lol_id != LOL_NULL_ID )
    {
        for ( ui2_i = 0 ; ((UINT16)ui2_i) < _s_ui2_max_lol_obj ; ui2_i++ )
        {
            /* Look for LOL ID. */
            if ( pt_id_name_ary[ui2_i].ui2_lol_id == ui2_lol_id )
            {
                pt_id_name_ary[ui2_i].ui2_lol_id = LOL_NULL_ID;
                pt_id_name_ary[ui2_i].s_lol_name[0]='\0';
                i4_rc = LOLR_OK;
                break;
            }
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: lol_get_name_by_id
 *
 * Description: This function search for specific LOL ID and return the LOL
 *              NAME.
 *
 * Inputs: 
 *    ui2_lol_id:  LOL ID.
 *
 * Outputs:
 *    s_lol_name:  LOL Name (must be at least the size of MAX_LOL_NAME_LEN).
 *
 * Returns: 
 *   TRUE       specified lol_id is found.
 *   FALSE      specified lol id is not found.
 ----------------------------------------------------------------------------*/
BOOL lol_get_name_by_id(UINT16 ui2_lol_id, CHAR* s_lol_name)
{
    INT16 ui2_i;

    /* 0 is not a valid value. */
    if ( ui2_lol_id == LOL_NULL_ID )
    {
        return FALSE;
    }
    
    for ( ui2_i = 0 ; ((UINT16)ui2_i) < _s_ui2_max_lol_obj ; ui2_i++ )
    {
        if ( ui2_lol_id == pt_id_name_ary[ui2_i].ui2_lol_id )
        {
            x_strcpy(s_lol_name,pt_id_name_ary[ui2_i].s_lol_name);
            return TRUE;
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: lol_get_id_by_name
 *
 * Description: This function search for specific LOL Name and return the LOL
 *              ID.
 *
 * Inputs: 
 *    s_lol_name:  LOL Name.
 *
 * Outputs:
 *    ui2_lol_id:  LOL ID.
 *
 * Returns: 
 *   TRUE          LOL Name is found.
 *   FALSE         LOL Name is not found.
 ----------------------------------------------------------------------------*/
BOOL lol_get_id_by_name(const CHAR* s_lol_name, UINT16* pui2_lol_id)
{
    INT16 ui2_i;
    
    *pui2_lol_id=LOL_NULL_ID;

    if ( s_lol_name == NULL )
    {
        return FALSE;
    }
    
    for ( ui2_i = 0 ; ((UINT16)ui2_i) < _s_ui2_max_lol_obj ; ui2_i++ )
    {
        if ( x_strcmp(pt_id_name_ary[ui2_i].s_lol_name,s_lol_name) == 0 )
        {
            *pui2_lol_id = pt_id_name_ary[ui2_i].ui2_lol_id;
            return TRUE;
        }
    }
    
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: lol_get_id_and_name_by_idx
 *
 * Description: This function get a LOL ID and its Name.
 *
 * Inputs: 
 *    s_lol_name:  LOL Name.
 *
 * Outputs:
 *    ps_lol_name:  LOL name. The caller must provides a buffer of at least
 *                  MAX_LOL_NAME_LEN size.
 *
 *    pui2_lol_id:  LOL ID.
 *
 * Returns: 
 *   TRUE          LOL ID and Name is found.
 *   FALSE         LOL Name is not found.
 ----------------------------------------------------------------------------*/
INT32 lol_get_id_and_name_by_idx
(
    UINT16  ui2_i,
    CHAR*   ps_lol_name,
    UINT16* pui2_lol_id
)
{
    INT32   i4_rc ;
    
    if ( pt_id_name_ary[ui2_i].ui2_lol_id != LOL_NULL_ID )
    {
        *pui2_lol_id = pt_id_name_ary[ui2_i].ui2_lol_id;
        x_strncpy(ps_lol_name, pt_id_name_ary[ui2_i].s_lol_name, MAX_LOL_NAME_LEN-1);
        ps_lol_name[MAX_LOL_NAME_LEN-1]='\0';
        i4_rc = LOLR_OK;
    }
    else
    {
        i4_rc = LOLR_FAIL;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: lol_sema_lock
 *
 * Description: This function get the sema for the LOL library. 
 *
 * Inputs: 
 *    None.
 *
 * Outputs:
 *    None.
 *
 * Returns: 
 *   None
 *   
 ----------------------------------------------------------------------------*/
VOID lol_sema_lock(VOID)
{
    if ( x_sema_lock( h_lol_lock, X_SEMA_OPTION_WAIT) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: lol_sema_unlock
 *
 * Description: This function release the sema for the LOL library. 
 *
 * Inputs: 
 *    None.
 *
 * Outputs:
 *    None.
 *
 * Returns: 
 *   None
 *   
 ----------------------------------------------------------------------------*/
VOID lol_sema_unlock(VOID)
{
    if ( x_sema_unlock( h_lol_lock) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE);
    }
}

