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
 * $RCSfile: sm_obsvr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "dbg/dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/_sm_utils.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

typedef struct _SM_OBSVR_T
{
    SM_OBSVR_FILTER_T             t_fltr;
    x_sm_obsvr_sess_nfy_fct       pf_sess_nfy;
    VOID*                         pv_sess_nfy_tag;
    x_sm_obsvr_strm_nfy_fct       pf_strm_nfy;
    VOID*                         pv_strm_nfy_tag;
    HANDLE_T                      h_lock;
} SM_OBSVR_T;

typedef struct _SM_OBSVR_PARSE_DATA_T
{
    BOOL              b_is_sess;
    VOID*             pv_arg;
    HANDLE_T          h_sm_obj;
    SM_COND_T         e_cond;
    VOID*             pv_tag;
    SM_EVN_TYPE_T     e_event;
    UINT32            ui4_data;
} SM_OBSVR_PARSE_DATA_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

BOOL            b_obsvr_init = FALSE;
HANDLE_LINK_T   t_obsvr_link;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

static BOOL _sm_obsvr_free_fct( HANDLE_T       h_handle,
                                HANDLE_TYPE_T  e_type,
                                VOID*          pv_obj,
                                VOID*          pv_tag,
                                BOOL           b_req_handle )
{
    SM_OBSVR_T*     pt_obsvr = (SM_OBSVR_T*)pv_obj;
    HANDLE_T        h_lock;

    SM_ASSERT(pt_obsvr);

    if ( b_req_handle )
    {
        return FALSE;
    }

    h_lock = pt_obsvr->h_lock;
    pt_obsvr->h_lock = NULL_HANDLE;

    x_sema_delete( h_lock );
    x_mem_free( pt_obsvr );
    x_mem_free( pt_obsvr->t_fltr.pui4_event_fltr );

    return TRUE;
}

static BOOL sm_obsvr_event_filter(
    SM_OBSVR_T*               pt_obsvr,
    UINT32*                   pui4_event_fltr,
    UINT32                    ui4_event )
{
    INT32     i4_i = 0;

    if ( !pui4_event_fltr )
    {
        return FALSE;
    }

    while ( pui4_event_fltr[i4_i++] != SM_EVN_IGNORE )
    {
        if ( ui4_event == pui4_event_fltr[i4_i] )
        {
            return TRUE;
        }
    }

    return FALSE;
}

static HP_TYPE_T sm_obsvr_parse_fct(
    UINT16         ui2_count,
    UINT16         ui2_max_count,
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    VOID*          pv_parse_data)
{
    SM_OBSVR_T*               pt_obsvr = (SM_OBSVR_T*)pv_obj;
    SM_OBSVR_PARSE_DATA_T*    pt_parse_data = (SM_OBSVR_PARSE_DATA_T*)pv_parse_data;
    BOOL                      b_should_nfy = FALSE;
    SM_OBSVR_T                t_obsvr;

    SM_ASSERT( pt_obsvr );
    SM_ASSERT( pt_parse_data );

    SM_LOCK_RETURN_ON_FAIL( pt_obsvr->h_lock, HP_NEXT );

    t_obsvr = *pt_obsvr;

    SM_UNLOCK( pt_obsvr->h_lock );

    if ( pt_parse_data->b_is_sess )
    {
        if ( t_obsvr.t_fltr.ui4_obj_type_fltr != SM_OBSVR_OBJ_TYPE_FILTER_ANY &&
             !(t_obsvr.t_fltr.ui4_obj_type_fltr & SM_OBSVR_OBJ_TYPE_FILTER_SESS) )
        {
            return HP_NEXT;
        }

        if ( t_obsvr.t_fltr.ui4_sess_type_fltr != SM_OBSVR_SESS_TYPE_FILTER_ANY &&
             !(t_obsvr.t_fltr.ui4_sess_type_fltr &
               MAKE_BIT_MASK_32((SM_SESS_TYPE_T)pt_parse_data->pv_arg)) )
        {
            return HP_NEXT;
        }

        if ( t_obsvr.t_fltr.ui4_sess_state_fltr == SM_OBSVR_STATE_FLTER_ANY ||
             t_obsvr.t_fltr.ui4_sess_state_fltr & MAKE_BIT_MASK_32(pt_parse_data->e_cond) )
        {
            b_should_nfy = TRUE;
        }

        if ( ( t_obsvr.t_fltr.pui4_event_fltr &&
               t_obsvr.t_fltr.pui4_event_fltr[0] == SM_OBSVR_EVN_FLTER_ANY ) ||
             sm_obsvr_event_filter(
                &t_obsvr, t_obsvr.t_fltr.pui4_event_fltr, pt_parse_data->e_event ) )
        {
            b_should_nfy = TRUE;
        }

        if ( b_should_nfy && t_obsvr.pf_sess_nfy )
        {
            t_obsvr.pf_sess_nfy(
                h_handle,
                (SM_SESS_TYPE_T)pt_parse_data->pv_arg,
                pt_parse_data->h_sm_obj,
                pt_parse_data->e_cond,
                t_obsvr.pv_sess_nfy_tag,
                pt_parse_data->e_event,
                pt_parse_data->ui4_data );
        }
    }
    else /* is stream */
    {
        if ( t_obsvr.t_fltr.ui4_obj_type_fltr != SM_OBSVR_OBJ_TYPE_FILTER_ANY &&
             !(t_obsvr.t_fltr.ui4_obj_type_fltr & SM_OBSVR_OBJ_TYPE_FILTER_STRM) )
        {
            return HP_NEXT;
        }

        if ( t_obsvr.t_fltr.ui4_strm_type_fltr != SM_OBSVR_STRM_TYPE_FILTER_ANY &&
             !(t_obsvr.t_fltr.ui4_strm_type_fltr &
               MAKE_BIT_MASK_32((STREAM_TYPE_T)(UINT32)pt_parse_data->pv_arg)) )
        {
            return HP_NEXT;
        }

        if ( t_obsvr.t_fltr.ui4_strm_state_fltr == SM_OBSVR_STATE_FLTER_ANY ||
             t_obsvr.t_fltr.ui4_strm_state_fltr & MAKE_BIT_MASK_32(pt_parse_data->e_cond) )
        {
            b_should_nfy = TRUE;
        }

        if ( ( t_obsvr.t_fltr.pui4_event_fltr &&
               t_obsvr.t_fltr.pui4_event_fltr[0] == SM_OBSVR_EVN_FLTER_ANY ) ||
             sm_obsvr_event_filter(
                &t_obsvr, t_obsvr.t_fltr.pui4_event_fltr, pt_parse_data->e_event ) )
        {
            b_should_nfy = TRUE;
        }

        if ( b_should_nfy && t_obsvr.pf_strm_nfy )
        {
            t_obsvr.pf_strm_nfy(
                h_handle,
                (STREAM_TYPE_T)(UINT32)pt_parse_data->pv_arg,
                pt_parse_data->h_sm_obj,
                pt_parse_data->e_cond,
                t_obsvr.pv_strm_nfy_tag,
                pt_parse_data->e_event,
                pt_parse_data->ui4_data );
        }
    }

    return HP_NEXT;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
VOID sm_obsvr_notify(
    HANDLE_T          h_sm_obj,
    SM_COND_T         e_cond,
    VOID*             pv_tag,
    SM_EVN_TYPE_T     e_event,
    UINT32            ui4_data )
{
    HANDLE_TYPE_T            e_hdl_type;
    VOID*                    pv_obj = NULL;
    SM_OBSVR_PARSE_DATA_T    t_parse_data;
    INT32                    i4_ret;

    i4_ret = handle_get_type_obj( h_sm_obj,
                                  &e_hdl_type,
                                  &pv_obj );
    if ( i4_ret != HR_OK || !pv_obj ||
         (e_hdl_type!=SM_HT_SESSION && e_hdl_type!=SM_HT_STREAM) )
    {
        return;
    }

    if ( e_hdl_type == SM_HT_SESSION )
    {
        SM_SESSION_T*    pt_sess_obj = pv_obj;
        SM_SESS_TYPE_T   e_sess_type;

        SM_LOCK_RETURN_ON_FAIL( pt_sess_obj->h_lock, );

        e_sess_type = pt_sess_obj->e_sess_type;

        SM_UNLOCK( pt_sess_obj->h_lock );

        t_parse_data.b_is_sess = TRUE;
        t_parse_data.pv_arg = (VOID*)e_sess_type;
        t_parse_data.h_sm_obj = h_sm_obj;
        t_parse_data.e_cond = e_cond;
        t_parse_data.pv_tag = pv_tag;
        t_parse_data.e_event = e_event;
        t_parse_data.ui4_data = ui4_data;
    }
    else if ( e_hdl_type == SM_HT_STREAM )
    {
        SM_STREAM_T*     pt_strm_obj = pv_obj;
        STREAM_TYPE_T    e_strm_type;

        SM_LOCK_RETURN_ON_FAIL( pt_strm_obj->h_lock, );

        e_strm_type = pt_strm_obj->t_strm_comp_id.e_type;

        SM_UNLOCK( pt_strm_obj->h_lock );

        t_parse_data.b_is_sess = FALSE;
        t_parse_data.pv_arg = (VOID*)e_strm_type;
        t_parse_data.h_sm_obj = h_sm_obj;
        t_parse_data.e_cond = e_cond;
        t_parse_data.pv_tag = pv_tag;
        t_parse_data.e_event = e_event;
        t_parse_data.ui4_data = ui4_data;
    }
    else
    {
        return;
    }

    handle_parse( &t_obsvr_link,
                  sm_obsvr_parse_fct,
                  (VOID*) &t_parse_data );
}

INT32 x_sm_register_obsvr(
    const SM_OBSVR_FILTER_T*      pt_fltr,
    x_sm_obsvr_sess_nfy_fct       pf_sess_nfy,
    VOID*                         pv_sess_nfy_tag,
    x_sm_obsvr_strm_nfy_fct       pf_strm_nfy,
    VOID*                         pv_strm_nfy_tag,
    HANDLE_T*                     ph_obsvr )
{
    SM_OBSVR_T*     pt_obsvr = NULL;
    INT32           i4_i = 0, i4_ret;
    UINT32*         pui4_event_fltr = NULL;

    if ( !b_obsvr_init )
    {
        handle_link_init( &t_obsvr_link );
    }

    if ( !pt_fltr || !ph_obsvr )
    {
        return SMR_INV_ARG;
    }

    pt_obsvr = (SM_OBSVR_T*) x_mem_alloc( sizeof(SM_OBSVR_T) );
    if ( !pt_obsvr )
    {
        return SMR_INSUFFICIENT_MEMORY;
    }

    for ( i4_i = 0;
          ( pt_fltr->pui4_event_fltr &&
            pt_fltr->pui4_event_fltr[i4_i]!=SM_EVN_IGNORE );
          i4_i++ )
    {
    }
    if ( i4_i > 1024 )
    {
        x_mem_free( pt_obsvr );
        return SMR_INSUFFICIENT_MEMORY;
    }

    if ( i4_i > 0 )
    {
        pui4_event_fltr = (UINT32*)x_mem_alloc( (UINT32)((i4_i+1)*sizeof(UINT32)) );
        if ( !pui4_event_fltr )
        {
            x_mem_free( pt_obsvr );
            return SMR_INSUFFICIENT_MEMORY;
        }

        x_memcpy( pui4_event_fltr,
                  pt_fltr->pui4_event_fltr,
                  (UINT32)(((i4_i+1)*sizeof(UINT32))) );
    }

    i4_ret = x_sema_create( &pt_obsvr->h_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK || pt_obsvr->h_lock == NULL_HANDLE )
    {
        x_mem_free( pt_obsvr );
        x_mem_free( pui4_event_fltr );
        return SMR_OUT_OF_HANDLES;
    }

    i4_ret = handle_alloc( SM_HT_OBSVR,
                       pt_obsvr,
                       (VOID*)NULL,
                       &_sm_obsvr_free_fct,
                       ph_obsvr );
    if ( i4_ret != HR_OK || *ph_obsvr==NULL_HANDLE )
    {
        x_mem_free( pt_obsvr );
        x_mem_free( pui4_event_fltr );
        x_sema_delete( pt_obsvr->h_lock );
        return SMR_OUT_OF_HANDLES;
    }

    pt_obsvr->t_fltr = *pt_fltr;
    pt_obsvr->t_fltr.pui4_event_fltr = pui4_event_fltr;
    pt_obsvr->pf_sess_nfy = pf_sess_nfy;
    pt_obsvr->pv_sess_nfy_tag = pv_sess_nfy_tag;
    pt_obsvr->pf_strm_nfy = pf_strm_nfy;
    pt_obsvr->pv_strm_nfy_tag = pv_strm_nfy_tag;

    i4_ret = handle_link( &t_obsvr_link, *ph_obsvr );
    if ( i4_ret != HR_OK )
    {
        handle_free( *ph_obsvr, FALSE );
    }

    return SMR_OK;
}

INT32 x_sm_unregister_obsvr( HANDLE_T       h_obsvr )
{
    if ( !b_obsvr_init )
    {
        handle_link_init( &t_obsvr_link );
    }

    handle_delink( &t_obsvr_link, h_obsvr );
    handle_free( h_obsvr, FALSE );

    return SMR_OK;
}
