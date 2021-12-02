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
 * $RCSfile: _sm_hc_tbl.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/9 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: 54ca8b28113974ac8d2d5d1d70ba3db4 $
 *
 * Description:
 *         This file implements the Stream Handler Control Table.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "dbg/dbg.h"

#include "strm_mngr/x_sm.h"
#include "strm_mngr/_sm_utils.h"
#include "hash/hash_api.h"

#include "_sm_hc_tbl.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SM_HC_SESS_HDLR_CTRL_TBL_SIZE     32

#define MAX( a, b)   ( (a>b)? a: b )
#define MIN( a, b)   ( (a<b)? a: b )

typedef enum
{
    SM_PARSE_HDLR_TYPE_FIND,
    SM_PARSE_HDLR_TYPE_LOOK_FOR_CONFLICT
} HC_TBL_STRM_HDLR_PARSE_TYPE_T;

typedef struct _HC_TBL_STRM_HDLR_PARSE_INFO_T
{
    CHAR                           s_sess_hdlr_name[SM_HDLR_NAME_MAX_SIZE+1];
    HC_TBL_STRM_HDLR_PARSE_TYPE_T  e_parse_type;
    union {
        STREAM_TYPE_T              e_strm_type;
        UINT32                     ui4_strm_type_mask;
    } smph_u;
#define smph_strm_type             smph_u.e_strm_type
#define smph_strm_type_mask        smph_u.ui4_strm_type_mask

    HANDLE_T                      h_hdlr;           /* OUT */
} HC_TBL_STRM_HDLR_PARSE_INFO_T;

typedef struct
{
    sm_hc_tbl_sess_hdlr_parse_fct   pf_parse_fct;
    VOID*                           pv_parse_tag;
} HC_TBL_SESS_HDLR_PARSE_INFO_T;


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

HANDLE_T                 gh_sess_hdlr_ctrl_tbl = NULL_HANDLE;
HANDLE_LINK_T            gt_hdlr_ctrl_tbl;
HANDLE_T                 gh_hdlr_ctrl_tbl_lock;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    Internal functions implementation
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: hc_tbl_sess_hdlr_handle_free_fct
 *
 * Description: This function frees the session handler object.
 *
 * Inputs:  h_handle      Contains the handle to the session handler.
 *          e_type        Contains the hash handle type.
 *          pv_obj        References the session handler object structure.
 *          pv_tag        Ignored.
 *          b_req_handle  Is set to TRUE if the handle free was initiated with
 *                        this handle else FALSE.
 *
 * Outputs: -
 *
 * Returns: TRUE   Success.
 *          FALSE  Fail.
 *
 ----------------------------------------------------------------------------*/
BOOL hc_tbl_sess_hdlr_handle_free_fct( HANDLE_T       h_handle,
                                       HANDLE_TYPE_T  e_type,
                                       VOID*          pv_obj,
                                       VOID*          pv_tag,
                                       BOOL           b_req_handle )
{
    SM_SESS_HANDLER_T*      pt_hdlr_obj = (SM_SESS_HANDLER_T*)pv_obj;

    if ( !pt_hdlr_obj || (e_type != SM_HT_HANDLER) )
    {
        return FALSE;
    }

    if ( b_req_handle )
    {
        /* remove from hash */
    }

    if ( !handle_link_is_empty( &pt_hdlr_obj->t_link ) )
    {
        return FALSE;
    }

    x_mem_free( pt_hdlr_obj );

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: hc_tbl_strm_hdlr_handle_free_fct
 *
 * Description: <function description>
 *
 * Inputs:  h_handle     References.
 *          e_type       References.
 *          pv_obj       References.
 *          pv_tag       References.
 *          b_req_handle References.
 *
 * Outputs: -
 *
 * Returns: TRUE   Success.
 *          FALSE  Fail.
 *
 ----------------------------------------------------------------------------*/
BOOL hc_tbl_strm_hdlr_handle_free_fct( HANDLE_T       h_handle,
                                      HANDLE_TYPE_T  e_type,
                                      VOID*          pv_obj,
                                      VOID*          pv_tag,
                                      BOOL           b_req_handle )
{
    SM_HANDLER_T*   pt_hdlr_obj = (SM_HANDLER_T*)pv_obj;

    if ( !pt_hdlr_obj || (e_type != SM_HT_HANDLER) )
    {
        return FALSE;
    }

    if ( handle_delink( &gt_hdlr_ctrl_tbl, h_handle ) != HR_OK )
    {
        return FALSE;
    }

    /* check if the stream handle link list inside the handler is empty */
    if ( !handle_link_is_empty( &pt_hdlr_obj->t_link ) )
    {
        return FALSE;
    }

    x_mem_free( pt_hdlr_obj );

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: hc_tbl_sess_hdlr_hash_free_fct
 *
 * Description: This function frees the session handler control table.
 *
 * Inputs:  pv_obj References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID hc_tbl_sess_hdlr_hash_free_fct( VOID* pv_obj )
{
    HANDLE_T             h_sess_hdlr = (HANDLE_T)pv_obj;
    INT32                i4_ret;

    i4_ret = handle_free( h_sess_hdlr, FALSE );
    if ( i4_ret != HR_OK )
    {
        return;
    }

    return;
}

/*-----------------------------------------------------------------------------
 * Name: hc_tbl_sess_hdlr_parse_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_hash      References.
 *          ui4_counter  References.
 *          pv_obj       References.
 *          pv_parse_tag References.
 *
 * Outputs: -
 *
 * Returns: HASH_OK           Success.
 *          HASH_PARSE_BREAK  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 hc_tbl_sess_hdlr_parse_fct( HANDLE_T          pt_hash,
                                  UINT32            ui4_counter,
                                  VOID*             pv_obj,
                                  VOID*             pv_parse_tag )
{
    INT32                            i4_ret;
    BOOL                             b_ret;
    HC_TBL_SESS_HDLR_PARSE_INFO_T*   pt_parse_info = NULL;
    HANDLE_T                         h_sess_hdlr = (HANDLE_T)pv_obj;
    HANDLE_TYPE_T                    t_hdl_type;
    SM_SESS_HANDLER_T*               pt_sess_hdlr_obj = NULL;

    if ( !pv_obj || !pv_parse_tag )
    {
        return HASH_PARSE_BREAK;
    }

    pt_parse_info = (HC_TBL_SESS_HDLR_PARSE_INFO_T*)pv_parse_tag;

    i4_ret = handle_get_type_obj( h_sess_hdlr,
                                  &t_hdl_type,
                                  (VOID**)&pt_sess_hdlr_obj );
    if ( i4_ret != HR_OK || t_hdl_type != SM_HT_SESS_HANDLER )
    {
        return HASH_PARSE_BREAK;
    }

    b_ret = pt_parse_info->pf_parse_fct( ui4_counter,
                                         pt_sess_hdlr_obj,
                                         pt_parse_info->pv_parse_tag );
    if ( !b_ret )
    {
        return HASH_PARSE_BREAK;
    }

    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: find_hdlr_parse_fct
 *
 * Description: <function description>
 *
 * Inputs:  ui2_count     References.
 *          ui2_max_count References.
 *          h_handle      References.
 *          e_type        References.
 *          pv_obj        References.
 *          pv_tag        References.
 *          pv_parse_data References.
 *
 * Outputs: -
 *
 * Returns: HP_BREAK  Break parsing.
 *          HP_NEXT   Next element.
 *
 ----------------------------------------------------------------------------*/
HP_TYPE_T find_hdlr_parse_fct( UINT16         ui2_count,
                               UINT16         ui2_max_count,
                               HANDLE_T       h_handle,
                               HANDLE_TYPE_T  e_type,
                               VOID*          pv_obj,
                               VOID*          pv_tag,
                               VOID*          pv_parse_data)
{
    SM_HANDLER_T*         pt_hdlr_obj = (SM_HANDLER_T*)pv_obj;
    HC_TBL_STRM_HDLR_PARSE_INFO_T* pt_parse_info = (HC_TBL_STRM_HDLR_PARSE_INFO_T*)pv_parse_data;

    if ( !pt_hdlr_obj || !pt_parse_info )
    {
        return HP_BREAK;
    }

    if ( x_strcmp( pt_hdlr_obj->s_sess_hdlr_name,
                   pt_parse_info->s_sess_hdlr_name ) != 0 )
    {
        return HP_NEXT;
    }

    if ( pt_parse_info->e_parse_type == SM_PARSE_HDLR_TYPE_FIND )
    {
        UINT32 ui4_strm_type_mask = (UINT32)( 1 << (UINT32)pt_parse_info->smph_strm_type );

        if ( ( pt_hdlr_obj->t_hdlr_desc.ui4_strm_type_mask &
               ui4_strm_type_mask )  )
        {
            /* Handler found */
            pt_parse_info->h_hdlr = h_handle;
            return HP_BREAK;
        }
    }
    else if ( pt_parse_info->e_parse_type ==
              SM_PARSE_HDLR_TYPE_LOOK_FOR_CONFLICT )
    {
        if ( (pt_hdlr_obj->t_hdlr_desc.ui4_strm_type_mask &
              pt_parse_info->smph_strm_type_mask)  )
        {
            /* Handler conflict found */
            pt_parse_info->h_hdlr = h_handle;
            return HP_BREAK;
        }
    }
    else
    {
        /* error */
        return HP_BREAK;
    }

    /* not found, continue */
    return HP_NEXT;
}


/*-----------------------------------------------------------------------------
                    Exported functions implementation
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_init
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SMR_OK                   Success.
 *          SMR_INTERNAL_ERROR       Fail.
 *          SMR_INSUFFICIENT_MEMORY  Fail.
 *          SMR_ALREADY_INIT         Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_hc_tbl_init( VOID )
{
    INT32 i4_ret;

    if ( gh_sess_hdlr_ctrl_tbl )
    {
        return SMR_ALREADY_INIT;
    }

    if ( handle_link_init( &gt_hdlr_ctrl_tbl ) != SMR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    i4_ret = hash_create( SM_HC_SESS_HDLR_CTRL_TBL_SIZE,
                       &gh_sess_hdlr_ctrl_tbl );
    if ( i4_ret != HASH_OK || gh_sess_hdlr_ctrl_tbl == NULL_HANDLE )
    {
        return SMR_INSUFFICIENT_MEMORY;
    }

    i4_ret = x_sema_create( &gh_hdlr_ctrl_tbl_lock,
                         X_SEMA_TYPE_MUTEX,
                         X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK || gh_hdlr_ctrl_tbl_lock == NULL_HANDLE )
    {
        hash_delete( gh_sess_hdlr_ctrl_tbl, NULL );
        gh_sess_hdlr_ctrl_tbl = NULL_HANDLE;
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_clean_up
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_hc_tbl_clean_up( VOID )
{
    INT32   i4_ret;

    if ( !gh_sess_hdlr_ctrl_tbl )
    {
        return SMR_OK;
    }

    i4_ret = handle_free_all( &gt_hdlr_ctrl_tbl );
    if ( i4_ret != HR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    hash_delete( gh_sess_hdlr_ctrl_tbl,
                 &hc_tbl_sess_hdlr_hash_free_fct );

    gh_sess_hdlr_ctrl_tbl = NULL_HANDLE;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_lock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_hc_tbl_lock( VOID )
{
    INT32 i4_ret;

    i4_ret = x_sema_lock( gh_hdlr_ctrl_tbl_lock,
                       X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_LOCK_SEMAPHORE );
    }
}

/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_unlock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_hc_tbl_unlock( VOID )
{
    INT32 i4_ret;

    i4_ret = x_sema_unlock( gh_hdlr_ctrl_tbl_lock );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE );
    }
}

/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_parse
 *
 * Description: <function description>
 *
 * Inputs:  pf_parse_fct References.
 *          pv_tag       References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_hc_tbl_parse( handle_parse_fct  pf_parse_fct, VOID* pv_tag )
{
    INT32 i4_ret;

    i4_ret = handle_parse( &gt_hdlr_ctrl_tbl,
                        pf_parse_fct,
                        (VOID*)pv_tag );
    if ( i4_ret!=HR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}

/* Return handle of the handler that control $e_strm_type */
/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_find_hdlr
 *
 * Description: <function description>
 *
 * Inputs:  s_sess_hdlr_name References.
 *          e_strm_type      References.
 *
 * Outputs: -
 *
 * Returns: handle       Success.
 *          NULL_HANDLE  Fail.
 *
 ----------------------------------------------------------------------------*/
HANDLE_T sm_hc_tbl_find_hdlr( const CHAR*        s_sess_hdlr_name,
                              STREAM_TYPE_T      e_strm_type )
{
    INT32                 i4_ret;
    HC_TBL_STRM_HDLR_PARSE_INFO_T  t_parse_info;

    if ( e_strm_type == ST_UNKNOWN )
    {
        return NULL_HANDLE;
    }

    t_parse_info.e_parse_type = SM_PARSE_HDLR_TYPE_FIND;
    x_strcpy( t_parse_info.s_sess_hdlr_name, s_sess_hdlr_name);
    t_parse_info.smph_strm_type = e_strm_type;
    t_parse_info.h_hdlr = NULL_HANDLE;

    sm_hc_tbl_lock();

    i4_ret = handle_parse( &gt_hdlr_ctrl_tbl,
                        &find_hdlr_parse_fct,
                        (VOID*)&t_parse_info );

    sm_hc_tbl_unlock();
    if ( i4_ret!=HR_OK )
    {
        return NULL_HANDLE;
    }

    return t_parse_info.h_hdlr;
}

/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_find_sess_hdlr
 *
 * Description: <function description>
 *
 * Inputs:  s_sess_hdlr_name References.
 *
 * Outputs: -
 *
 * Returns: handle       Success.
 *          NULL_HANDLE  Fail.
 *
 ----------------------------------------------------------------------------*/
HANDLE_T sm_hc_tbl_find_sess_hdlr( const CHAR*   s_sess_hdlr_name )
{
    INT32     i4_ret;
    HANDLE_T  h_sess_hdlr;

    if ( !s_sess_hdlr_name )
    {
        return NULL_HANDLE;
    }

    sm_hc_tbl_lock();

    i4_ret = hash_get_obj( gh_sess_hdlr_ctrl_tbl,
                           (INT8*)s_sess_hdlr_name,
                           (x_strlen(s_sess_hdlr_name) + 1),
                           (VOID**)&h_sess_hdlr );

    sm_hc_tbl_unlock();

    if ( i4_ret != HASH_OK )
    {
        return NULL_HANDLE;
    }

    return h_sess_hdlr;
}

/* Find if the stream types in $ui4_strm_type_mask is already registered by an
   conflict handler. */
/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_is_conflict
 *
 * Description: <function description>
 *
 * Inputs:  s_sess_hdlr_name   References.
 *          ui4_strm_type_mask References.
 *
 * Outputs: -
 *
 * Returns: TRUE   Success.
 *          FALSE  Fail.
 *
 ----------------------------------------------------------------------------*/
BOOL sm_hc_tbl_is_conflict( const CHAR*          s_sess_hdlr_name,
                            UINT32               ui4_strm_type_mask )
{
    INT32                 i4_ret;
    HC_TBL_STRM_HDLR_PARSE_INFO_T  t_parse_info;

    t_parse_info.e_parse_type = SM_PARSE_HDLR_TYPE_LOOK_FOR_CONFLICT;
    x_strcpy( t_parse_info.s_sess_hdlr_name, s_sess_hdlr_name );
    t_parse_info.smph_strm_type_mask = ui4_strm_type_mask;
    t_parse_info.h_hdlr = NULL_HANDLE;

    sm_hc_tbl_lock();

    i4_ret = handle_parse( &gt_hdlr_ctrl_tbl,
                           &find_hdlr_parse_fct,
                           (VOID*)&t_parse_info );

    sm_hc_tbl_unlock();
    if ( i4_ret!=HR_OK )
    {
        return FALSE;
    }

    return ((t_parse_info.h_hdlr != NULL_HANDLE)? TRUE: FALSE);
}

/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_reg_strm_hdlr
 *
 * Description: <function description>
 *
 * Inputs:  pt_strm_hdlr_desc    References.
 *          ps_sess_hdlr_name    References.
 *          pt_fct_tbl           References.
 *          b_run_task_as_thread References.
 *          ph_hdlr              References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK                       Success.
 *          SMR_INSUFFICIENT_MEMORY      Fail.
 *          SMR_INTERNAL_ERROR           Fail.
 *          SMR_OUT_OF_HANDLES           Fail.
 *          SMR_HDLR_ALREADY_REGISTERED  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_hc_tbl_reg_strm_hdlr( const SM_HDLR_DESC_T*    pt_strm_hdlr_desc,
                               const CHAR*              ps_sess_hdlr_name,
                               const SM_HDLR_FCT_TBL_T* pt_fct_tbl,
                               BOOL                     b_run_task_as_thread,
                               HANDLE_T*                ph_hdlr )
{
    INT32           i4_ret;
    SIZE_T          z_name_size;
    SM_HANDLER_T*   pt_hdlr_obj;

    if ( sm_hc_tbl_is_conflict( ps_sess_hdlr_name,
                                pt_strm_hdlr_desc->ui4_strm_type_mask) )
    {
        return SMR_HDLR_ALREADY_REGISTERED;
    }

    /* create handler object*/
    pt_hdlr_obj = (SM_HANDLER_T*)x_mem_alloc( sizeof(SM_HANDLER_T) );
    if ( !pt_hdlr_obj )
    {
        return SMR_INSUFFICIENT_MEMORY;
    }

    pt_hdlr_obj->t_hdlr_desc = *pt_strm_hdlr_desc;
    pt_hdlr_obj->t_fct_tbl = *pt_fct_tbl;
    pt_hdlr_obj->b_run_task_as_thread = b_run_task_as_thread;
    pt_hdlr_obj->h_hdlr_thread = NULL_HANDLE;
    if ( handle_link_init( &pt_hdlr_obj->t_link ) != SMR_OK )
    {
        x_mem_free( pt_hdlr_obj );
        return SMR_INTERNAL_ERROR;
    }

    /* copy session handler name string */
    z_name_size = MIN( x_strlen(ps_sess_hdlr_name), (SM_HDLR_NAME_MAX_SIZE) );
    x_memcpy( pt_hdlr_obj->s_sess_hdlr_name, ps_sess_hdlr_name, z_name_size );
    pt_hdlr_obj->s_sess_hdlr_name[z_name_size] = '\0';

    /* create handle */
    i4_ret = handle_alloc( SM_HT_HANDLER,
                           pt_hdlr_obj,
                           NULL,
                           &hc_tbl_strm_hdlr_handle_free_fct,
                           ph_hdlr );
    if ( i4_ret != HR_OK )
    {
        x_mem_free( pt_hdlr_obj );
        return SMR_OUT_OF_HANDLES;
    }

    i4_ret = handle_link( &gt_hdlr_ctrl_tbl, (*ph_hdlr) );
    if ( i4_ret != HR_OK )
    {
        handle_free( (*ph_hdlr), FALSE );
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_reg_sess_hdlr
 *
 * Description: <function description>
 *
 * Inputs:  pt_hdlr_desc         References.
 *          pt_fct_tbl           References.
 *          b_run_task_as_thread References.
 *          ph_sess_hdlr         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK                       Success.
 *          SMR_INSUFFICIENT_MEMORY      Fail.
 *          SMR_INTERNAL_ERROR           Fail.
 *          SMR_OUT_OF_HANDLES           Fail.
 *          SMR_HDLR_ALREADY_REGISTERED  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_hc_tbl_reg_sess_hdlr( const SM_HDLR_DESC_T*         pt_hdlr_desc,
                               const SM_SESS_HDLR_FCT_TBL_T* pt_fct_tbl,
                               BOOL                          b_run_task_as_thread,
                               HANDLE_T*                     ph_sess_hdlr )
{
    INT32               i4_ret;
    SIZE_T              z_name_size;
    SM_SESS_HANDLER_T*  pt_sess_hdlr_obj = NULL;
    HANDLE_T            h_out;

    z_name_size = MIN( x_strlen(pt_hdlr_desc->s_hdlr_name),
                       (SM_HDLR_NAME_MAX_SIZE) );

    /* check if conflict exist */
    i4_ret = hash_get_obj( gh_sess_hdlr_ctrl_tbl,
                           (INT8*)pt_hdlr_desc->s_hdlr_name,
                           (z_name_size + 1),
                           (VOID**)&h_out );
    if ( i4_ret != HASH_NOT_EXIST )
    {
        return SMR_HDLR_ALREADY_REGISTERED;
    }

    /* create session handler object */
    pt_sess_hdlr_obj = (SM_SESS_HANDLER_T*)
        x_mem_alloc( sizeof(SM_SESS_HANDLER_T) );
    if ( !pt_sess_hdlr_obj )
    {
        return SMR_INSUFFICIENT_MEMORY;
    }

    pt_sess_hdlr_obj->t_fct_tbl = *pt_fct_tbl;
    pt_sess_hdlr_obj->b_run_task_as_thread = b_run_task_as_thread;
    pt_sess_hdlr_obj->h_hdlr_thread = NULL_HANDLE;
    if ( handle_link_init( &pt_sess_hdlr_obj->t_link ) != SMR_OK )
    {
        x_mem_free( pt_sess_hdlr_obj );
        return SMR_INTERNAL_ERROR;
    }

    /* copy handler descriptor */
    pt_sess_hdlr_obj->t_hdlr_desc = *pt_hdlr_desc;

    /* create handle */
    i4_ret = handle_alloc( SM_HT_SESS_HANDLER,
                           pt_sess_hdlr_obj,
                           NULL,
                           &hc_tbl_sess_hdlr_handle_free_fct,
                           ph_sess_hdlr );
    if ( i4_ret != HR_OK || !ph_sess_hdlr )
    {
        x_mem_free( pt_sess_hdlr_obj );
        return SMR_OUT_OF_HANDLES;
    }

    i4_ret = hash_add_obj( gh_sess_hdlr_ctrl_tbl,
                           (INT8*)pt_hdlr_desc->s_hdlr_name,
                           z_name_size + 1,
                           (VOID*)(*ph_sess_hdlr),
                           (VOID**)&h_out );    /* OUT */
    if ( i4_ret != HASH_OK )
    {
        handle_free( *ph_sess_hdlr, FALSE );
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_unreg_hdlr
 *
 * Description: <function description>
 *
 * Inputs:  h_hdlr References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 sm_hc_tbl_unreg_hdlr( HANDLE_T             h_hdlr )
{
    return handle_free( h_hdlr, TRUE );
}

/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_sess_hdlr_parse
 *
 * Description: <function description>
 *
 * Inputs:  pf_parse_fct References.
 *          pv_parse_tag References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_hc_tbl_sess_hdlr_parse( sm_hc_tbl_sess_hdlr_parse_fct  pf_parse_fct,
                                 VOID*                          pv_parse_tag )
{
    INT32                           i4_ret;
    HC_TBL_SESS_HDLR_PARSE_INFO_T   t_parse_info;

    t_parse_info.pf_parse_fct = pf_parse_fct;
    t_parse_info.pv_parse_tag = pv_parse_tag;

    i4_ret = hash_parse( gh_sess_hdlr_ctrl_tbl,
                         &hc_tbl_sess_hdlr_parse_fct,
                         &t_parse_info );
    if ( i4_ret != HASH_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_hc_tbl_sess_hdlr_get_size
 *
 * Description: <function description>
 *
 * Inputs:  pz_size References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_hc_tbl_sess_hdlr_get_size( SIZE_T* pz_size )
{
    INT32 i4_ret;

    i4_ret = hash_get_size( gh_sess_hdlr_ctrl_tbl,
                            pz_size );      /* OUT */
    if ( i4_ret != HASH_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}
