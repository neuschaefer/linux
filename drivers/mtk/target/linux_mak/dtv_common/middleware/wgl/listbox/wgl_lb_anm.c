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
 * $RCSfile: wgl_lb_anm.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "wgl/listbox/wgl_lb_anm.h"
#include "wgl/listbox/wgl_lb_util.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/frlm/wgl_msfs.h"
#include "os/inc/x_os.h"
#include "unicode/x_uc_str.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define TRQ_INIT_NUM        8
#define TRQ_EXPAND_NUM      8
#define TWL_INIT_NUM        8
#define TWL_EXPAND_NUM      8
#define TRP_INIT_NUM        8
#define TRP_EXPAND_NUM      8
#define HETSM_INIT_NUM      4
#define HETSM_EXPAND_NUM    4

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/

/*****************************************************************************
 *****************************************************************************
        common animation tools
 *****************************************************************************
 *****************************************************************************/


/*****************************************************************************
            TRQ - Timer Request Queue 
 *****************************************************************************/

INT32 anm_trq_init(
    OUT     ANM_TRQ_CTX_T*              pt_ctx,
    IN      SIZE_T                      z_max_data_size)
{
    INT32       i4_ret;
    
    /* init NPM */
    CATCH_FAIL( wgl_npm_init( 
        & pt_ctx->t_npm,  
        sizeof ( TRQ_NODE_T ) + z_max_data_size,
        TRQ_INIT_NUM,
        TRQ_EXPAND_NUM ) );
    
    /* init context */
    pt_ctx->ui2_sn       = 0;
    pt_ctx->ui2_num_node = 0;
    DLIST_INIT( & pt_ctx->t_dlst_nodes );
    
    return WGLR_OK;
}        

INT32 anm_trq_deinit(
    IN      ANM_TRQ_CTX_T*              pt_ctx)
{
    TRQ_NODE_T          *pt_node;
    INT32               i4_ret;
    
    /* free all nodes */
    pt_node = DLIST_HEAD( & pt_ctx->t_dlst_nodes );
    while ( pt_node )
    {
        DLIST_REMOVE( pt_node, & pt_ctx->t_dlst_nodes, t_link );
        pt_ctx->ui2_num_node -- ;
        INFORM_ERR( wgl_npm_free( & pt_ctx->t_npm, (VOID *) pt_node ) );
        pt_node = DLIST_HEAD( & pt_ctx->t_dlst_nodes );
    }
    
    /* deinit NPM */
    INFORM_ERR( wgl_npm_deinit ( & pt_ctx->t_npm ) );
    
    /* clear the context */
    x_memset( pt_ctx, 0, sizeof( ANM_TRQ_CTX_T ) );
    
    return WGLR_OK;
}        

INT32 anm_trq_put(
    IN OUT  ANM_TRQ_CTX_T*              pt_ctx,
    IN      VOID*                       pv_data,
    IN      SIZE_T                      z_size,
    OUT     UINT16*                     pui2_id_out)
{
    INT32           i4_ret;
    SIZE_T          z_node;
    TRQ_NODE_T*     pt_node_new; 
    
    z_node = sizeof(TRQ_NODE_T) + z_size;
    
    CHECK_VALID( z_node <= wgl_npm_get_node_size( &pt_ctx->t_npm ), WGLR_INV_ARG );
    CHECK_VALID( pui2_id_out != NULL, WGLR_INV_ARG );
    
    /* allocate a new node */
    CATCH_FAIL( wgl_npm_alloc( &pt_ctx->t_npm, (VOID **) &pt_node_new ) );
    
    /* init node */
    pt_node_new->ui2_id = pt_ctx->ui2_sn ++ ;
    pt_node_new->z_size = z_size;
    
    /* store the node data at the tail of TRQ_NODE_T */
    x_memcpy((VOID *) (pt_node_new + 1), pv_data, z_size);
    
    /* append this node at the tail of node list */
    DLIST_INSERT_TAIL( pt_node_new, &pt_ctx->t_dlst_nodes, t_link);
    
    /* increase the node number */
    pt_ctx->ui2_num_node ++ ;
    
    return WGLR_OK;
}        

/*-----------------------------------------------------------------------------
 * Name: anm_trq_get
 *
 * Description: This API retrieves (pops) a timer request from queue.
 *              Note that this API should not be called when the queue is empty.
 *
 * Inputs:  pt_ctx      Contains the TRQ context.
 *          z_buf_size  Specifies the buffer size.
 *
 * Outputs: pt_ctx      Contains the changed TRQ context.
 *          pv_data     Contains the client-defined timer request info.
 *          pui2_id     Contains an unique id generated for identifying this timer request.
 *
 * Returns: WGLR_OK         Routine successful.
 *          WGLR_INV_STATE  The queue is empty.
 *          WGLR_INV_ARG    Invalid arguments.
 *          < 0             Some other errors.
 ----------------------------------------------------------------------------*/
INT32 anm_trq_get(
    IN OUT  ANM_TRQ_CTX_T*              pt_ctx,
    IN      SIZE_T                      z_buf_size,
    OUT     VOID*                       pv_data,
    OUT     UINT16*                     pui2_id)
{
    INT32           i4_ret;
    TRQ_NODE_T*     pt_node; 
    
    /* check state */
    CHECK_VALID( pt_ctx->ui2_num_node > 0, WGLR_INV_STATE );
    CHECK_VALID( pv_data != NULL  &&
                 pui2_id != NULL  , WGLR_INV_ARG );
    
    /* retrieve a node at the head */
    pt_node = DLIST_HEAD( &pt_ctx->t_dlst_nodes );

    /* check output buffer size */    
    CHECK_VALID( z_buf_size >= pt_node->z_size, WGLR_BUFFER_NOT_ENOUGH );

    /* copy data out */
    x_memcpy( pv_data, (VOID *) (pt_node + 1), pt_node->z_size );
    *pui2_id = pt_node->ui2_id;

    /* remove from node list */
    DLIST_REMOVE( pt_node, & pt_ctx->t_dlst_nodes, t_link );
    
    /* free this node */
    INFORM_ERR( wgl_npm_free( &pt_ctx->t_npm, (VOID *) pt_node ) );

    /* decrease the node number */
    pt_ctx->ui2_num_node -- ;
    
    return WGLR_OK;
}        

BOOL anm_trq_check_empty(
    IN      ANM_TRQ_CTX_T*              pt_ctx)
{
    return DLIST_IS_EMPTY( &pt_ctx->t_dlst_nodes ) ? TRUE : FALSE ;
}        


/*****************************************************************************
            TWL - Timer Wait List 
 *****************************************************************************/
INT32 anm_twl_init( 
    OUT     ANM_TWL_CTX_T*              pt_ctx,
    IN      SIZE_T                      z_max_data_size)
{
    INT32       i4_ret;
    
    /* init NPM */
    CATCH_FAIL( wgl_npm_init( 
        & pt_ctx->t_npm, 
        sizeof ( TWL_NODE_T ) + z_max_data_size,
        TWL_INIT_NUM,
        TWL_EXPAND_NUM ) );
    
    /* init context */
    pt_ctx->ui2_num_node = 0;
    DLIST_INIT( & pt_ctx->t_dlst_nodes );
    
    return WGLR_OK;
}        

INT32 anm_twl_deinit(
    IN      ANM_TWL_CTX_T*              pt_ctx)
{
    INT32           i4_ret;
    TWL_NODE_T      *pt_node;
    
    
    /* free TWL nodes */
    pt_node = DLIST_HEAD( & pt_ctx->t_dlst_nodes );
    while ( pt_node )
    {
        DLIST_REMOVE( pt_node, & pt_ctx->t_dlst_nodes, t_link );
        
        pt_ctx->ui2_num_node -- ;
        
        INFORM_ERR( wgl_npm_free( & pt_ctx->t_npm, (VOID *) pt_node ) );
        
        pt_node = DLIST_HEAD( & pt_ctx->t_dlst_nodes );
    }
    
    /* deinit NPM */
    INFORM_ERR( wgl_npm_deinit ( & pt_ctx->t_npm ) );
    
    /* clear the context */
    x_memset( pt_ctx, 0, sizeof ( ANM_TWL_CTX_T ) );
    
    return WGLR_OK;
}        

INT32 anm_twl_append(
    IN OUT  ANM_TWL_CTX_T*              pt_ctx,
    IN      UINT16                      ui2_id,
    IN      VOID*                       pv_data,
    IN      SIZE_T                      z_size)
{
    INT32                   i4_ret;
    TWL_NODE_T*             pt_node_new;
    SIZE_T                  z_node;
    
    /* check parameters */
    z_node = sizeof(DLIST_TWL_NODE_T) + z_size;
    CHECK_VALID( z_node <= wgl_npm_get_node_size( &pt_ctx->t_npm ), WGLR_INV_ARG );
    
    /* allocate a new node */
    CATCH_FAIL( wgl_npm_alloc( &pt_ctx->t_npm, (VOID **) & pt_node_new ) );
    
    /* init node */
    pt_node_new->ui2_id = ui2_id;
    pt_node_new->z_size = z_size;
    
    /* store the node data at the tail of TWL_NODE_T */
    x_memcpy( (VOID *)(pt_node_new + 1), pv_data, z_size);
    
    /* append the node at the end of node list */
    DLIST_INSERT_TAIL( pt_node_new, & pt_ctx->t_dlst_nodes, t_link );
    
    /* increase the node number */
    pt_ctx->ui2_num_node ++ ;
    
    return WGLR_OK;
}        

INT32 anm_twl_get(
    IN OUT  ANM_TWL_CTX_T*              pt_ctx,
    IN      UINT16                      ui2_id,
    IN      SIZE_T                      z_buf_size,
    OUT     VOID*                       pv_data)
{
    INT32                   i4_ret;
    TWL_NODE_T*             pt_node;
    BOOL                    b_found;
    
    /* check parameters */
    CHECK_VALID( pv_data != NULL, WGLR_INV_ARG );
    
    /* search the node with ui2_id */
    b_found = FALSE;
    DLIST_FOR_EACH( pt_node, &pt_ctx->t_dlst_nodes, t_link )
    {
        if ( pt_node->ui2_id == ui2_id )
        {
            b_found = TRUE;
            break;
        }
    }
    CHECK_VALID( b_found, WGLR_INV_ARG );
    
    /* check buffer size */
    CHECK_VALID( z_buf_size >= pt_node->z_size, WGLR_BUFFER_NOT_ENOUGH );
    
    /* copy data */
    x_memcpy( pv_data, (VOID *) (pt_node + 1), pt_node->z_size );
    
    /* remove the node from the list */
    DLIST_REMOVE( pt_node, &pt_ctx->t_dlst_nodes, t_link );
    
    /* free this node */
    INFORM_ERR( wgl_npm_free( &pt_ctx->t_npm, (VOID *) pt_node ) );
    
    /* decrease node number */
    pt_ctx->ui2_num_node -- ;
    
    return WGLR_OK;
}        

INT32 anm_twl_del_by_parse(
    IN OUT  ANM_TWL_CTX_T*              pt_ctx,
    IN      anm_twl_del_parse_fct       pf_parse,
    IN      VOID*                       pv_tag)
{
    INT32               i4_ret;
    TWL_NODE_T          *pt_node, *pt_node_next;
    
    pt_node = DLIST_HEAD( & pt_ctx->t_dlst_nodes );
    
    while ( pt_node )
    {
        pt_node_next = DLIST_NEXT( pt_node, t_link );
        
        if ( pf_parse( pt_node->ui2_id, (VOID *) (pt_node + 1), pv_tag ) )
        {
            /* remove from list */
            DLIST_REMOVE( pt_node, & pt_ctx->t_dlst_nodes, t_link );
    
            /* free this node */
            INFORM_ERR( wgl_npm_free( & pt_ctx->t_npm, (VOID *) pt_node ) );
            
            /* decrease node number */
            pt_ctx->ui2_num_node -- ;
        }            
        
        pt_node = pt_node_next;
    }         
    
    return WGLR_OK;
}        


/*****************************************************************************
            TRP - Timer Resource Pool
 *****************************************************************************/
INT32 anm_trp_init(
    IN      HANDLE_T                    h_widget,
    OUT     ANM_TRP_CTX_T*              pt_ctx)
{
    INT32       i4_ret;
    
    /* init NPM */
    CATCH_FAIL( wgl_npm_init(
        & pt_ctx->t_npm,
        sizeof( TRP_NODE_T ),
        TRP_INIT_NUM,
        TRP_EXPAND_NUM ) );
    
    /* init context */
    pt_ctx->h_widget = h_widget;
    DLIST_INIT( & pt_ctx->t_dlst_nodes );
    
    return WGLR_OK;
}        

INT32 anm_trp_deinit(
    IN      ANM_TRP_CTX_T*              pt_ctx)
{
    TRP_NODE_T          *pt_node;
    INT32               i4_ret;
    
    /* free all nodes */
    pt_node = DLIST_HEAD( & pt_ctx->t_dlst_nodes );
    while ( pt_node )
    {
        DLIST_REMOVE( pt_node, & pt_ctx->t_dlst_nodes, t_link );
        
        INFORM_ERR( x_wgl_delete_timer( pt_node->h_timer ) );
        
        INFORM_ERR( wgl_npm_free( & pt_ctx->t_npm, (VOID *) pt_node ) );
        
        pt_node = DLIST_HEAD( & pt_ctx->t_dlst_nodes );
    }
    
    /* deinit NPM */
    INFORM_ERR( wgl_npm_deinit( & pt_ctx->t_npm ) );
    
    /* clear context */
    x_memset( pt_ctx, 0, sizeof( ANM_TRP_CTX_T ) );
    
    return WGLR_OK;
}        

INT32 anm_trp_alloc(
    IN OUT  ANM_TRP_CTX_T*              pt_ctx,
    IN      WGL_TIMER_SYNC_MODE_T       e_mode,
    OUT     HANDLE_T*                   ph_timer)
{
    INT32               i4_ret, i4_ret_fct;
    TRP_NODE_T          *pt_node;
    
    *ph_timer = NULL_HANDLE;
    
    /* search for a free node */
    DLIST_FOR_EACH( pt_node, &pt_ctx->t_dlst_nodes, t_link )
    {
        if ( pt_node->e_status == NODE_ST_FREE &&
             pt_node->e_mode == e_mode )
        {
            /* found a Timer Resource ! */
            pt_node->e_status = NODE_ST_IN_USE;
            *ph_timer         = pt_node->h_timer;
            
            return WGLR_OK;
        }
    }
    
    /* allocate a new one if not found */
    CATCH_FAIL ( wgl_npm_alloc( & pt_ctx->t_npm, (VOID **) &pt_node ) );
    
    /* create a timer */
    i4_ret_fct = x_wgl_create_timer( pt_ctx->h_widget, e_mode, &pt_node->h_timer);
    if ( WGL_IS_FAIL( i4_ret_fct ) )
    {
        INFORM_ERR( i4_ret_fct );
        INFORM_ERR( wgl_npm_free( &pt_ctx->t_npm, (VOID *) pt_node ) ) ;
        return i4_ret_fct;
    }
    
    /* init the new node */
    pt_node->e_status = NODE_ST_IN_USE;
    pt_node->e_mode   = e_mode;
    
    /* append the node into the node list */
    DLIST_INSERT_TAIL( pt_node, &pt_ctx->t_dlst_nodes, t_link );

    /* return the timer */
    *ph_timer = pt_node->h_timer;
    
    return WGLR_OK;
}        
    
INT32 anm_trp_free(
    IN OUT  ANM_TRP_CTX_T*              pt_ctx,
    IN      HANDLE_T                    h_timer)
{
    INT32               i4_ret;
    TRP_NODE_T          *pt_node;
    
    /* search for the host node of h_timer */
    DLIST_FOR_EACH( pt_node, &pt_ctx->t_dlst_nodes, t_link )
    {
        if ( pt_node->h_timer == h_timer )
        {
            CHECK_VALID( pt_node->e_status == NODE_ST_IN_USE, WGLR_INV_STATE );
            
            /* free this node */
            pt_node->e_status = NODE_ST_FREE;
            
            return WGLR_OK;
        }
    }
    
    INFORM_ERR( WGLR_INV_ARG );
    return WGLR_INV_ARG;
}        

/*****************************************************************************
            TSA - Text Scroll Agent
 *****************************************************************************/
static INT32 _tsa_get_text_char_based(
    IN      ANM_TSA_CTX_T*              pt_ctx,
    OUT     UINT16*                     pui2_start,
    OUT     UINT16*                     pui2_len_eff,
    OUT     INT32*                      pi4_left_offset,
    OUT     BOOL*                       pb_end_scrl,
    OUT     BOOL*                       pb_req_timer,
    OUT     BOOL*                       pb_disp_normal,
    OUT     UINT32*                     pui4_ms_timer);

static INT32 _tsa_restart_scrl_cycle(
    IN OUT  ANM_TSA_CTX_T*              pt_ctx,
    OUT     BOOL*                       pb_req_timer);

 
INT32 anm_tsa_init(
    OUT     ANM_TSA_CTX_T*              pt_ctx,
    OUT     BOOL*                       pb_req_timer,
    OUT     UINT32*                     pui4_ms_timer,
    IN      ANM_TSA_INIT_T*             pt_init)
{
    INT32   i4_ret;
    UINT32  ui4_tk_now;
    UINT32  ui4_ms_tk;
    
    *pb_req_timer = FALSE;
    
    /* check parameters */
    CHECK_VALID( 
        ! ( pt_init->t_info.ui4_style & ( ~( ( TSA_STL_LAST_STYLE << 1 ) - 1) ) ), WGLR_INV_ARG );
    CHECK_VALID( 
        pt_init->t_info.pw2s_text                                              &&
        pt_init->t_info.ui2_len_text > 0                                       &&
        pt_init->t_info.i4_w_bound                                             &&
        pt_init->t_info.ui4_tk_delay > 0                                       &&
        
        (  
           ( HAS_FLAG( pt_init->t_info.ui4_style, TSA_STL_REPEAT ) && 
             pt_init->t_info.ui4_scrl_count == 0 )                        ||
           ( !HAS_FLAG( pt_init->t_info.ui4_style, TSA_STL_REPEAT ) ) 
        )                                                                      &&
        
        (  
           ( HAS_FLAG( pt_init->t_info.ui4_style, TSA_STL_PIXEL_BASED ) &&
             pt_init->t_info.i4_step > 0 )                                  ||
           ( !HAS_FLAG( pt_init->t_info.ui4_style, TSA_STL_PIXEL_BASED ) &&
             pt_init->t_info.i4_step == 0 ) 
        )                                                                      &&
        
        pt_init->pw2s_buf                                                      &&
        pt_init->ui2_len_buf > 0                                               &&
        ( pt_init->t_info.ui2_len_text + (UINT16)1) <= pt_init->ui2_len_buf
        
        , WGLR_INV_ARG );
                 
    
    /* init attributes */
    pt_ctx->ui4_style      = pt_init->t_info.ui4_style;
    pt_ctx->h_font         = pt_init->t_info.h_font;
    pt_ctx->i4_w_bound     = pt_init->t_info.i4_w_bound;
    pt_ctx->ui4_tk_delay   = pt_init->t_info.ui4_tk_delay;
    pt_ctx->ui4_tk_start_delay 
                           = pt_init->t_info.ui4_tk_start_delay;
    pt_ctx->ui4_tk_start_scrl
                           = 0;
    pt_ctx->ui4_scrl_count = pt_init->t_info.ui4_scrl_count;
    pt_ctx->i4_step        = pt_init->t_info.i4_step;
    pt_ctx->ui1_txt_dir    = pt_init->t_info.ui1_txt_dir;
    pt_ctx->pw2s_buf       = pt_init->pw2s_buf;
    pt_ctx->ui2_len_buf    = pt_init->ui2_len_buf;
    
    pt_ctx->ui2_len_text   = pt_init->t_info.ui2_len_text;
    pt_ctx->b_no_store_text = pt_init->t_info.b_no_store_text;
    lb_util_w2s_strncpy_wo_ch (pt_ctx->pw2s_buf, pt_init->t_info.pw2s_text, pt_init->t_info.ui2_len_text, WGL_UTF16_SOFT_HYPHEN);


    /* init session context */
    if ( ( ! HAS_FLAG( pt_ctx->ui4_style, TSA_STL_REPEAT ) ) && 
           pt_ctx->ui4_scrl_count == 0 )
    {
        pt_ctx->b_end_scrl = TRUE;
    }
    else
    {
        pt_ctx->b_end_scrl = FALSE;
    }
    /* Here we add the user specified delay ticks to delay a certain
       time when start scroll the text */
    ui4_tk_now = x_os_get_sys_tick();
    ui4_ms_tk = x_os_get_sys_tick_period();
    if (pt_ctx->ui4_tk_start_delay > 0)
    {
        pt_ctx->ui4_tk_last_scrl  = ui4_tk_now + pt_ctx->ui4_tk_start_delay;
        pt_ctx->ui4_tk_start_scrl = pt_ctx->ui4_tk_last_scrl + pt_ctx->ui4_tk_delay;
        *pui4_ms_timer            = (pt_ctx->ui4_tk_start_delay + pt_ctx->ui4_tk_delay) * ui4_ms_tk;
        *pb_req_timer             = TRUE;
    }
    else
    {
        pt_ctx->ui4_tk_start_scrl = ui4_tk_now;
        pt_ctx->ui4_tk_last_scrl  = ui4_tk_now;
        *pui4_ms_timer            = pt_ctx->ui4_tk_delay * ui4_ms_tk;
    }

    pt_ctx->ui4_count_scrled = 0;
    
    /* init scroll cycle */
    CATCH_FAIL( _tsa_restart_scrl_cycle( pt_ctx, pb_req_timer ) );
    
    return WGLR_OK;
    
}        

INT32 anm_tsa_deinit(
    IN      ANM_TSA_CTX_T*              pt_ctx)
{
    /* clear context */
    x_memset( pt_ctx, 0, sizeof( ANM_TSA_CTX_T ) );
    
    return WGLR_OK;
}        

BOOL anm_tsa_check_info_changed(
    IN      ANM_TSA_CTX_T*              pt_ctx,
    IN      ANM_TSA_INFO_T*             pt_info)
{
    BOOL                                b_change;
    UTF16_T*							pw2s_tmp_buf;
    
	pw2s_tmp_buf = NULL;
	
    if(pt_ctx == NULL || pt_info == NULL )
    {
        return FALSE;   
    }
    
   if(0!= pt_info->ui2_len_text) 
    {
    	pw2s_tmp_buf = WGL_MEM_ALLOC(sizeof(UTF16_T) * 
    								(pt_info->ui2_len_text+1));
		if(NULL != pw2s_tmp_buf)
		{
			x_memset( pw2s_tmp_buf, 0,(sizeof(UTF16_T) * 
										(pt_info->ui2_len_text+1)));
			lb_util_w2s_strncpy_wo_ch (pw2s_tmp_buf, pt_info->pw2s_text, 
			x_uc_w2s_strlen(pt_info->pw2s_text), WGL_UTF16_SOFT_HYPHEN);
		}
		else
		{
			 return FALSE;	
		}
	}
    if(pt_info->b_no_store_text == FALSE && 0!= pt_info->ui2_len_text)    
    {
        b_change = ( ( pt_info->ui4_style      == pt_ctx->ui4_style )       &&
             ( pt_info->h_font         == pt_ctx->h_font )          &&
             ( pt_info->i4_w_bound     == pt_ctx->i4_w_bound )      &&
             ( pt_info->ui4_tk_delay   == pt_ctx->ui4_tk_delay )    && 
             ( pt_info->ui4_scrl_count == pt_ctx->ui4_scrl_count )  &&
             ( pt_info->i4_step        == pt_ctx->i4_step )         &&
             ( pt_info->ui2_len_text   == pt_ctx->ui2_len_text )    &&
             ( pt_info->ui1_txt_dir    == pt_ctx->ui1_txt_dir  )    &&
             ! x_uc_w2s_strncmp( pw2s_tmp_buf, pt_ctx->pw2s_buf, (UINT32)
pt_info->ui2_len_text)) 
             
           ? FALSE : TRUE;
	}
    else
    {
        b_change = ( ( pt_info->ui4_style      == pt_ctx->ui4_style )       &&
                 ( pt_info->h_font         == pt_ctx->h_font )          &&
                 ( pt_info->i4_w_bound     == pt_ctx->i4_w_bound )      &&
                 ( pt_info->ui4_tk_delay   == pt_ctx->ui4_tk_delay )    && 
                 ( pt_info->ui4_scrl_count == pt_ctx->ui4_scrl_count )  &&
                 ( pt_info->i4_step        == pt_ctx->i4_step )         &&
                 ( pt_info->ui2_len_text   == pt_ctx->ui2_len_text )    &&
                 ( pt_info->ui1_txt_dir    == pt_ctx->ui1_txt_dir  ) ) 
                 
               ? FALSE : TRUE;
    }
    if(NULL != pw2s_tmp_buf)
    {
		WGL_MEM_FREE(pw2s_tmp_buf);
	}
    return b_change;
}

INT32 anm_tsa_update_info(
    IN OUT  ANM_TSA_CTX_T*              pt_ctx,
    IN      ANM_TSA_INFO_T*             pt_info,
    OUT     BOOL*                       pb_req_timer,
    OUT     UINT32*                     pui4_ms_timer)
{
    BOOL        b_need_reset;
    INT32       i4_ret;
    UINT32      ui4_ms_tk;
    
    *pb_req_timer = FALSE;
    

    if(pt_info->b_no_store_text == FALSE)
    {
    b_need_reset = 
       ( ( pt_info->ui4_style    == pt_ctx->ui4_style )         &&
         ( pt_info->h_font       == pt_ctx->h_font )            &&
         ( pt_info->i4_w_bound   == pt_ctx->i4_w_bound )        &&        
         ( pt_info->ui2_len_text == pt_ctx->ui2_len_text )      &&
         ( pt_info->ui1_txt_dir  == pt_ctx->ui1_txt_dir  )      &&
         ! x_uc_w2s_strncmp( pt_info->pw2s_text, pt_ctx->pw2s_buf, (UINT32) pt_info->ui2_len_text ) ) 
     ? FALSE : TRUE;
    }
    else
    {
        b_need_reset = 
           ( ( pt_info->ui4_style    == pt_ctx->ui4_style )         &&
             ( pt_info->h_font       == pt_ctx->h_font )            &&
             ( pt_info->i4_w_bound   == pt_ctx->i4_w_bound )        &&        
             ( pt_info->ui2_len_text == pt_ctx->ui2_len_text )      &&
             ( pt_info->ui1_txt_dir  == pt_ctx->ui1_txt_dir  )  ) 
         ? FALSE : TRUE;
    }

    if ( b_need_reset )
    {
        CHECK_VALID( pt_info->ui2_len_text < pt_ctx->ui2_len_buf, WGLR_INV_ARG );
        
        pt_ctx->ui4_style    = pt_info->ui4_style;
        pt_ctx->h_font       = pt_info->h_font;
        pt_ctx->i4_w_bound   = pt_info->i4_w_bound;
        pt_ctx->ui2_len_text = pt_info->ui2_len_text;
        pt_ctx->ui1_txt_dir  = pt_info->ui1_txt_dir;
        lb_util_w2s_strncpy_wo_ch (pt_ctx->pw2s_buf, pt_info->pw2s_text, pt_info->ui2_len_text, WGL_UTF16_SOFT_HYPHEN);
    }
    
    pt_ctx->ui4_tk_delay   = pt_info->ui4_tk_delay;
    pt_ctx->ui4_scrl_count = pt_info->ui4_scrl_count;
    pt_ctx->i4_step        = pt_info->i4_step;
    
    if ( b_need_reset )
    {
        pt_ctx->b_end_scrl       = TRUE;
        CATCH_FAIL( _tsa_restart_scrl_cycle( pt_ctx, pb_req_timer ) );
        
        /* reset scrl session */
        pt_ctx->b_end_scrl       = FALSE;
        pt_ctx->ui4_count_scrled = 0;
        pt_ctx->ui4_tk_last_scrl = x_os_get_sys_tick();
    }
    
    ui4_ms_tk = x_os_get_sys_tick_period();
    *pui4_ms_timer = pt_ctx->ui4_tk_delay * ui4_ms_tk;
    
    return WGLR_OK;
}            

INT32 anm_tsa_get_text(
    IN      ANM_TSA_CTX_T*              pt_ctx,
    IN      BOOL                        b_read_only,
    OUT     BOOL*                       pb_disp_normal,
    OUT     UINT16*                     pui2_start,
    OUT     UINT16*                     pui2_len_eff,
    OUT     INT32*                      pi4_left_offset,
    OUT     BOOL*                       pb_end_scrl,
    OUT     BOOL*                       pb_req_timer,
    OUT     UINT32*                     pui4_ms_timer)
{
    INT32       i4_ret;

    CHECK_VALID((pt_ctx != NULL), WGLR_INV_ARG);
    
    /* quick return if end scroll */
    if ( pt_ctx->b_end_scrl )
    {
        *pui2_start      = pt_ctx->ui2_start;
        *pui2_len_eff    = pt_ctx->ui2_len_eff;
        *pi4_left_offset = pt_ctx->i4_left_offset;
        *pb_end_scrl     = TRUE;
        *pb_req_timer    = FALSE;
        *pb_disp_normal  = TRUE;
        
        return WGLR_OK;
    }
    else if ( b_read_only )
    {
        *pui2_start      = pt_ctx->ui2_start;
        *pui2_len_eff    = pt_ctx->ui2_len_eff;
        *pi4_left_offset = pt_ctx->i4_left_offset;
        *pb_end_scrl     = pt_ctx->b_end_scrl;
        *pb_req_timer    = FALSE;
        *pb_disp_normal  = TRUE;
        
        return WGLR_OK;
    }
    
    /* switch scrolling mode */
    if( HAS_FLAG( pt_ctx->ui4_style, TSA_STL_PIXEL_BASED ) )
    {
        /* NOT SUPORTED yet, use text based instead */
        CATCH_FAIL( _tsa_get_text_char_based(
            pt_ctx,
            pui2_start,
            pui2_len_eff,
            pi4_left_offset,
            pb_end_scrl,
            pb_req_timer,
            pb_disp_normal,
            pui4_ms_timer) );
    }
    else
    {
        CATCH_FAIL( _tsa_get_text_char_based(
            pt_ctx,
            pui2_start,
            pui2_len_eff,
            pi4_left_offset,
            pb_end_scrl,
            pb_req_timer,
            pb_disp_normal,
            pui4_ms_timer) );
    }        
    
    return WGLR_OK;
}    

static INT32 _tsa_get_text_char_based(
    IN      ANM_TSA_CTX_T*              pt_ctx,
    OUT     UINT16*                     pui2_start,
    OUT     UINT16*                     pui2_len_eff,
    OUT     INT32*                      pi4_left_offset,
    OUT     BOOL*                       pb_end_scrl,
    OUT     BOOL*                       pb_req_timer,
    OUT     BOOL*                       pb_disp_normal,
    OUT     UINT32*                     pui4_ms_timer)
{
    UINT32          ui4_tk_now;
    UINT32          ui4_ms_tk;
    UINT16          ui2_start_new;
    UINT16          ui2_len_eff_new;
    INT32           i4_left_offset_new;
    INT32           i4_ret;

    *pui2_start        = pt_ctx->ui2_start;
    *pui2_len_eff      = pt_ctx->ui2_len_eff;
    *pi4_left_offset   = pt_ctx->i4_left_offset;
    *pb_end_scrl       = FALSE;
    *pb_req_timer      = FALSE;
    *pb_disp_normal    = FALSE;
    
    ui2_start_new      = pt_ctx->ui2_start;
    ui2_len_eff_new    = pt_ctx->ui2_len_eff;
    i4_left_offset_new = pt_ctx->i4_left_offset;
    
    ui4_ms_tk  = x_os_get_sys_tick_period();
    ui4_tk_now = x_os_get_sys_tick();
    
    /* Delay when start scroll */ 
    if (ui4_tk_now < pt_ctx->ui4_tk_start_scrl)
    {
        *pb_disp_normal = TRUE;
        *pb_req_timer   = FALSE;
        if ( HAS_FLAG ( pt_ctx->ui4_style, TSA_STL_FROM_FIRST_CHAR ) &&
             pt_ctx->ui2_len_eff == 1)
        {
            /* In this case, if we don't reset the ui2_len_eff to 0,
               it will display two chars at first after the delay */
            pt_ctx->ui2_len_eff = 0;
        }

        return WGLR_OK;
    }

    if (ui4_tk_now >= pt_ctx->ui4_tk_last_scrl + pt_ctx->ui4_tk_delay)
    {
        if ( pt_ctx->ui1_txt_dir == WGL_TEXT_DIR_LEFT_TO_RIGHT )
        {
            if ( pt_ctx->i4_left_offset > 0 )
            {
                /* starting phase */
                /* append a character at the end, decrease i4_left_offset */
                INT32   i4_w_new;
                
                CHECK_VALID( pt_ctx->ui2_start + pt_ctx->ui2_len_eff < pt_ctx->ui2_len_text, WGLR_INTERNAL_ERROR );
                
                CATCH_FAIL( wgl_msfs_get_chars_width(
                    pt_ctx->h_font,
                    pt_ctx->pw2s_buf + pt_ctx->ui2_start,
                    pt_ctx->ui2_len_eff + (UINT16)1,
                    NULL,
                    0,
                    0,
                    & i4_w_new ) );
                
                if ( i4_w_new > pt_ctx->i4_w_bound )
                {
                    ui2_start_new ++ ;
                    
                    /* transit into normal phase */
                    i4_left_offset_new  = 0;
                }
                else
                {
                    ui2_len_eff_new ++;
                    
                    i4_left_offset_new = pt_ctx->i4_w_bound - i4_w_new;
                } 
            }
            else
            {
                BOOL        b_end_scrl_cycle;
                
                /* normal phase */
                
                /* remove a character at head, and try to append as many character at the end as possible */
                /* if no character can be removed, end of scroll cycle is found */
                
                /* assume not end of scrl cycle */
                b_end_scrl_cycle = FALSE;
                
                /* try to remove a character at the head */
                if ( pt_ctx->ui2_start + (UINT16)1 < pt_ctx->ui2_len_text )
                {
                    if ( pt_ctx->ui2_start + pt_ctx->ui2_len_eff < pt_ctx->ui2_len_text )
                    {
                        UTF16_T         *pw2s_temp;
                        UINT32          ui4_num_ch;
                        INT32           i4_w_temp;
                        UINT16          ui2_num_ch_avail;
                        
                        ui2_num_ch_avail = pt_ctx->ui2_len_text - ( pt_ctx->ui2_start + (UINT16)1 );
                        
                        /* find the lonest text that can fit in i4_w_bound */
                        CATCH_FAIL( wgl_get_max_char_in_box(
                            pt_ctx->h_font,
                            pt_ctx->pw2s_buf + ( pt_ctx->ui2_start + 1 ),
                            (INT32) ui2_num_ch_avail,
                            FALSE,
                            pt_ctx->i4_w_bound,
                            NULL,
                            & pw2s_temp,
                            & ui4_num_ch,
                            & i4_w_temp) );
                        
                        /* remove a character at the head */
                        ui2_start_new ++ ;
                        
                        if ( ( (UINT16) ui4_num_ch ) < ui2_num_ch_avail )
                        {
                            /* record the new text length */
                            if ( ui4_num_ch > 0 )
                            {
                                /* i4_w_bound is large enough to fit at least one character */
                                ui2_len_eff_new = (UINT16) ui4_num_ch;
                            }
                            else
                            {
                                /* i4_w_bound is too small, force to include a character anyway */
                                ui2_len_eff_new = 1;
                            }
                        }
                        else
                        {
                            /* update ui2_len_eff */
                            ui2_len_eff_new = ui2_num_ch_avail;
                            
                            /* end of text is encountered */
                            if ( HAS_FLAG ( pt_ctx->ui4_style, TSA_STL_TILL_LAST_CHAR ) )
                            {
                                /* continue scrolling till last char */
                                /* do nothing */
                            }
                            else
                            {
                                /* allow the last move for the last char */
                                /* do nothing */
                            }
                        }
                    }
                    else
                    {
                        
                        if ( HAS_FLAG ( pt_ctx->ui4_style, TSA_STL_TILL_LAST_CHAR ) )
                        {
                            /* continue scrolling till last char */
                            
                            /* remove a character at the head */
                            ui2_start_new ++ ;
                            ui2_len_eff_new -- ;
                        }
                        else
                        {
                            b_end_scrl_cycle = TRUE;
                        } 
                    } 
                }
                else
                {
                    /* end of this scroll cycle */
                    b_end_scrl_cycle = TRUE;
                }
                
                /* check if end of scroll cycle */
                if ( b_end_scrl_cycle )
                {
                    /* increase ui4_count_scrled */
                    pt_ctx->ui4_count_scrled ++ ;
                    
                    /* check if end of scroll */
                    if ( ( HAS_FLAG( pt_ctx->ui4_style, TSA_STL_REPEAT ) )   || 
                         ( pt_ctx->ui4_count_scrled < pt_ctx->ui4_scrl_count ) )
                    {
                        BOOL    b_temp;
                        
                        /* restart scroll cycle */
                        INFORM_ERR( _tsa_restart_scrl_cycle( pt_ctx, & b_temp ) );
                        
                        ui2_start_new      = pt_ctx->ui2_start;
                        ui2_len_eff_new    = pt_ctx->ui2_len_eff;
                        i4_left_offset_new = pt_ctx->i4_left_offset;
                    }
                    else
                    {
                        /* inform end of scrl */
                        pt_ctx->b_end_scrl = TRUE;
                        *pb_end_scrl       = TRUE;
                    } 
                }
            }
        }
        /*Text dirction:Right->Left, for Arab text*/
        else
        {
            BOOL            b_end_scrl_cycle;
            UTF16_T         *pw2s_temp;
            UINT32          ui4_num_ch;
            INT32           i4_w_temp;
            UINT16          ui2_num_ch_avail;

            /* find the lonest text that can fit in i4_w_bound */
            CATCH_FAIL( wgl_get_max_char_in_box_for_arab(
                (INT32)pt_ctx->ui2_len_text,
                pt_ctx->h_font,
                &pt_ctx->pw2s_buf[pt_ctx->ui2_len_text - 1],
                (INT32) pt_ctx->ui2_len_text,
                FALSE,
                pt_ctx->i4_w_bound,
                NULL,
                NULL,
                0,
                0,
                TRUE,
                & pw2s_temp,
                & ui4_num_ch,
                & i4_w_temp) );

            if ( (UINT32)(pt_ctx->ui2_len_text - pt_ctx->ui2_start) < ui4_num_ch )
            {
                CHECK_VALID( (pt_ctx->ui2_start + pt_ctx->ui2_len_eff - (UINT16)1) < pt_ctx->ui2_len_text, WGLR_INTERNAL_ERROR );
                
                ui2_start_new--;
                ui2_len_eff_new++;
            }
            else
            {     
                /* normal phase */
                /* assume not end of scrl cycle */
                b_end_scrl_cycle = FALSE;
                      
                /* try to remove a character at the head */
                if ( pt_ctx->ui2_start > 0 )
                {
                    if ( (pt_ctx->ui2_start + ui2_len_eff_new - (INT16)1) < pt_ctx->ui2_len_text )
                    {   
                        UINT16  ui2_end;
                        
                        ui2_start_new --;
                        ui2_end = ui2_start_new + ui2_len_eff_new - (UINT16)1;
                        ui2_num_ch_avail = ui2_end + (UINT16)1;
                        
                        /* find the lonest text that can fit in i4_w_bound */
                        CATCH_FAIL( wgl_get_max_char_in_box_for_arab(
                            (INT32)ui2_num_ch_avail,
                            pt_ctx->h_font,
                            pt_ctx->pw2s_buf + ui2_end,
                            (INT32) ui2_num_ch_avail,
                            FALSE,
                            pt_ctx->i4_w_bound,
                            NULL,
                            NULL,
                            0,
                            0,
                            TRUE,
                            & pw2s_temp,
                            & ui4_num_ch,
                            & i4_w_temp) );
                        
                        /* remove a character at the head */
                        ui2_start_new = ui2_end - (UINT16)ui4_num_ch + (UINT16)1;
                        i4_left_offset_new = pt_ctx->i4_w_bound - i4_w_temp;
                        
                        if ( ( (UINT16) ui4_num_ch ) < ui2_num_ch_avail )
                        {
                            /* record the new text length */
                            if ( ui4_num_ch > 0 )
                            {
                                /* i4_w_bound is large enough to fit at least one character */
                                ui2_len_eff_new = (UINT16) ui4_num_ch;
                            }
                            else
                            {
                                /* i4_w_bound is too small, force to include a character anyway */
                                ui2_len_eff_new = 1;
                            }
                        }
                    }
                }
                else
                {
                    if ( HAS_FLAG ( pt_ctx->ui4_style, TSA_STL_TILL_LAST_CHAR ) )
                    {
                        if (ui2_len_eff_new > 0)
                        {
                            ui2_len_eff_new--;
                            
                            CATCH_FAIL( wgl_msfs_get_chars_width(
                                    pt_ctx->h_font,
                                    pt_ctx->pw2s_buf,
                                    ui2_len_eff_new,
                                    NULL,
                                    0,
                                    0,
                                    & i4_w_temp ) );
                            i4_left_offset_new = pt_ctx->i4_w_bound - i4_w_temp;
                        } 
                        else
                        {
                            /* end of this scroll cycle */
                            b_end_scrl_cycle = TRUE;
                        }
                    }
                    else
                    {
                        /* end of this scroll cycle */
                        b_end_scrl_cycle = TRUE;
                    }
                }
                
                /* check if end of scroll cycle */
                if ( b_end_scrl_cycle )
                {
                    /* increase ui4_count_scrled */
                    pt_ctx->ui4_count_scrled ++ ;
                    
                    /* check if end of scroll */
                    if ( ( HAS_FLAG( pt_ctx->ui4_style, TSA_STL_REPEAT ) )   || 
                         ( pt_ctx->ui4_count_scrled < pt_ctx->ui4_scrl_count ) )
                    {
                        BOOL    b_temp;
                        
                        /* restart scroll cycle */
                        INFORM_ERR( _tsa_restart_scrl_cycle( pt_ctx, & b_temp ) );
                        
                        ui2_start_new      = pt_ctx->ui2_start;
                        ui2_len_eff_new    = pt_ctx->ui2_len_eff;
                        i4_left_offset_new = pt_ctx->i4_left_offset;
                    }
                    else
                    {
                        /* inform end of scrl */
                        pt_ctx->b_end_scrl = TRUE;
                        *pb_end_scrl       = TRUE;
                    }
                }
            }
            
        }
        
        /* update context */
        pt_ctx->ui2_start        = ui2_start_new ;
        pt_ctx->ui2_len_eff      = ui2_len_eff_new ;
        pt_ctx->i4_left_offset   = i4_left_offset_new ;
        
        /* record last scrl time */
        pt_ctx->ui4_tk_last_scrl = ui4_tk_now;

        /* prepare return values */
        *pui2_start              = pt_ctx->ui2_start;
        *pui2_len_eff            = pt_ctx->ui2_len_eff;
        *pi4_left_offset         = pt_ctx->i4_left_offset;
        
        /* should request a timer in this case */
        *pb_req_timer            = TRUE;
        *pb_disp_normal          = FALSE;
        *pui4_ms_timer           = pt_ctx->ui4_tk_delay * ui4_ms_tk;
    }
    
    return WGLR_OK;
}        

static INT32 _tsa_restart_scrl_cycle(
    IN OUT  ANM_TSA_CTX_T*              pt_ctx,
    OUT     BOOL*                       pb_req_timer)
{
    BOOL        b_pixel_based;
    INT32       i4_ret;
    INT32       i4_w_text, i4_h_temp;

    *pb_req_timer = FALSE;
    
    /* pre-init context */
    pt_ctx->ui2_start        = 0;
    pt_ctx->ui2_len_eff      = pt_ctx->ui2_len_text;
    pt_ctx->i4_left_offset   = 0;
    
    /* check if need scroll */
    i4_ret = x_fe_get_string_size( 
        pt_ctx->h_font, 
        pt_ctx->pw2s_buf, 
        (INT32) pt_ctx->ui2_len_text,
        &i4_w_text,
        &i4_h_temp );

    if ( !( i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE ) )
    {
        INFORM_ERR( i4_ret );
        return i4_ret;
    }
    
    /* don't need to scroll */
    if ( i4_w_text <= pt_ctx->i4_w_bound )
    {
        return WGLR_OK;
    }
    
    if ( HAS_FLAG( pt_ctx->ui4_style, TSA_STL_PIXEL_BASED ) )
    {
        /* NOT SUPPORTED now, use char-based mode instead */
        b_pixel_based = FALSE;
    }
    else
    {
        b_pixel_based = FALSE;
    }

    if ( b_pixel_based )
    {
        /* NOT SUPPORTED now ! */
        return WGLR_NOT_SUPPORT_FORMAT;
    }
    else
    {    
        /* char-based mode */
        if ( pt_ctx->ui1_txt_dir == WGL_TEXT_DIR_LEFT_TO_RIGHT)
        {
            if ( HAS_FLAG ( pt_ctx->ui4_style, TSA_STL_FROM_FIRST_CHAR ) )
            {
                INT32   i4_w_first_char;
                
                i4_ret = x_fe_get_string_size( 
                    pt_ctx->h_font, 
                    pt_ctx->pw2s_buf, 
                    1,
                    &i4_w_first_char,
                    &i4_h_temp );
            
                if ( !( i4_ret == FER_OK || i4_ret == FER_NO_CHAR_CODE ) )
                {
                    INFORM_ERR( i4_ret );
                    return i4_ret;
                }
                
                pt_ctx->ui2_start        = 0;
                pt_ctx->ui2_len_eff      = 1;
                pt_ctx->i4_left_offset   = pt_ctx->i4_w_bound - i4_w_first_char;
            }
            else
            {
                UTF16_T     *pw2s_temp;
                UINT32      ui4_max_num_ch;
                INT32       i4_max_w;
                
                CATCH_FAIL( wgl_get_max_char_in_box(
                    pt_ctx->h_font,
                    pt_ctx->pw2s_buf,
                    (INT32) pt_ctx->ui2_len_text,
                    FALSE,
                    pt_ctx->i4_w_bound,
                    NULL,
                    & pw2s_temp,
                    & ui4_max_num_ch,
                    & i4_max_w ) );
                    
                pt_ctx->ui2_start      = 0;
                pt_ctx->ui2_len_eff    = (UINT16) ui4_max_num_ch;
                pt_ctx->i4_left_offset = 0;
            }
        }
        else
        {
            if ( HAS_FLAG ( pt_ctx->ui4_style, TSA_STL_FROM_FIRST_CHAR ) )
            {
                pt_ctx->ui2_start      = pt_ctx->ui2_len_text - (UINT16)1;
                pt_ctx->ui2_len_eff    = 1;
                pt_ctx->i4_left_offset = 0;
            }
            else
            {
                UTF16_T     *pw2s_temp;
                UINT32      ui4_max_num_ch;
                INT32       i4_max_w;
                
                CATCH_FAIL( wgl_get_max_char_in_box_for_arab(
                    (INT32) pt_ctx->ui2_len_text,
                    pt_ctx->h_font,
                    &pt_ctx->pw2s_buf[pt_ctx->ui2_len_text - 1],
                    (INT32) pt_ctx->ui2_len_text,
                    FALSE,
                    pt_ctx->i4_w_bound,
                    NULL,
                    NULL,
                    0,
                    0,
                    TRUE,
                    & pw2s_temp,
                    & ui4_max_num_ch,
                    & i4_max_w ) );
                    
                pt_ctx->ui2_start      = pt_ctx->ui2_len_text - (UINT16) ui4_max_num_ch;
                pt_ctx->ui2_len_eff    = (UINT16) ui4_max_num_ch;
                pt_ctx->i4_left_offset = pt_ctx->i4_w_bound - i4_max_w; 
            }
            
        }
        
    }
    
    /* should request a timer */
    *pb_req_timer = TRUE;
    
    return WGLR_OK;
}


/*****************************************************************************
 *****************************************************************************
        listbox specific animation tools
 *****************************************************************************
 *****************************************************************************/

/*****************************************************************************
            HETSM - Highlight Element Text Scroll Manager
 *****************************************************************************/
static INT32 _hetsm_clear_all(
    IN OUT  LB_HETSM_CTX_T*             pt_ctx);

#define HETSM_IN_VALID_STATE( _pt_ctx_hetsm )       \
    ( pt_ctx->ui2_tsa_text_buf_len > 0 )
 
INT32 lb_hetsm_init(
    OUT     LB_HETSM_CTX_T*             pt_ctx,
    IN      UINT16                      ui2_tsa_text_buf_len)
{
    INT32   i4_ret;
    
    if ( ui2_tsa_text_buf_len > 0 )
    {
        /* init NPM */
        CATCH_FAIL( wgl_npm_init( 
            & pt_ctx->t_npm,
            sizeof ( HETSM_NODE_T ) + ( sizeof (UTF16_T) * ui2_tsa_text_buf_len ),
            HETSM_INIT_NUM,
            HETSM_EXPAND_NUM ) );
        
        /* init context */
        pt_ctx->ui2_tsa_text_buf_len = ui2_tsa_text_buf_len;
        pt_ctx->pv_marker            = NULL;
        DLIST_INIT( & pt_ctx->t_dlst_nodes );
    }
    else
    {
        x_memset( pt_ctx, 0, sizeof ( LB_HETSM_CTX_T ) );
        pt_ctx->ui2_tsa_text_buf_len = 0;
    }
    
    return WGLR_OK;
}        

INT32 lb_hetsm_deinit(
    IN      LB_HETSM_CTX_T*             pt_ctx)
{
    INT32           i4_ret;
    
    if ( HETSM_IN_VALID_STATE ( pt_ctx ) ) 
    {
        /* free all nodes */
        INFORM_ERR( _hetsm_clear_all ( pt_ctx ) );
        
        /* deinit NPM */
        INFORM_ERR( wgl_npm_deinit ( & pt_ctx->t_npm ) );
    }
    
    /* clear context */
    x_memset( pt_ctx, 0, sizeof( LB_HETSM_CTX_T ) );
    
    return WGLR_OK;
}    

BOOL lb_hetsm_check_marker(
    IN      LB_HETSM_CTX_T*             pt_ctx,
    IN      VOID*                       pv_marker)
{
    
    /* check state */
    CHECK_VALID( HETSM_IN_VALID_STATE ( pt_ctx ), FALSE );
    
    return ( pv_marker == pt_ctx->pv_marker ) ? TRUE : FALSE;
}        

INT32 lb_hetsm_reset(
    IN OUT  LB_HETSM_CTX_T*             pt_ctx,
    IN      VOID*                       pv_marker)
{
    INT32           i4_ret;
    
    /* check state */
    CHECK_VALID( HETSM_IN_VALID_STATE ( pt_ctx ), WGLR_INV_STATE );
    
    /* free all TSA's */
    INFORM_ERR( _hetsm_clear_all ( pt_ctx ) );
    
    /* update the marker */
    pt_ctx->pv_marker = pv_marker;
    
    return WGLR_OK;
}        
    
INT32 lb_hetsm_query_tsa(
    IN      LB_HETSM_CTX_T*             pt_ctx,
    IN      VOID*                       pv_marker,
    IN      UINT8                       ui1_col,
    OUT     ANM_TSA_CTX_T**             ppt_ctx_tsa)
{
    HETSM_NODE_T    *pt_node;
    BOOL            b_found;
    
    *ppt_ctx_tsa    = NULL;
    
    /* check state */
    CHECK_VALID( HETSM_IN_VALID_STATE ( pt_ctx ), WGLR_INV_STATE );
    
    CHECK_VALID( lb_hetsm_check_marker( pt_ctx, pv_marker ), WGLR_INV_ARG );
    
    b_found = FALSE;
    DLIST_FOR_EACH( pt_node, & pt_ctx->t_dlst_nodes, t_link )
    {
        if ( ui1_col == pt_node->ui1_col )
        {
            b_found = TRUE;
            break;
        }
    }
    
    if ( b_found )
    {
        *ppt_ctx_tsa = & pt_node->t_ctx_tsa;
    }
    else
    {
        *ppt_ctx_tsa = NULL;
    }
    
    return WGLR_OK;
}        

INT32 lb_hetsm_create_tsa(
    IN OUT  LB_HETSM_CTX_T*             pt_ctx,
    IN      UINT8                       ui1_col,
    IN      ANM_TSA_INFO_T*             pt_init_info,
    OUT     ANM_TSA_CTX_T**             ppt_ctx_tsa,
    OUT     BOOL*                       pb_req_timer,
    OUT     UINT32*                     pui4_ms_timer)
{
    INT32               i4_ret, i4_ret_fct;
    HETSM_NODE_T        *pt_node_new;
    ANM_TSA_CTX_T       *pt_ctx_tsa_temp;
    ANM_TSA_INIT_T      t_tsa_init;
    
    *ppt_ctx_tsa  = NULL;
    *pb_req_timer = FALSE;
    
    /* check state */
    CHECK_VALID( HETSM_IN_VALID_STATE ( pt_ctx ), WGLR_INV_STATE );
    
    /* check if the TSA already exist */
    CATCH_FAIL( lb_hetsm_query_tsa( pt_ctx, pt_ctx->pv_marker, ui1_col, & pt_ctx_tsa_temp ) );
    CHECK_VALID( pt_ctx_tsa_temp == NULL, WGLR_INV_ARG );
    
    /* allocate a new node */
    CATCH_FAIL( wgl_npm_alloc( &pt_ctx->t_npm, (VOID **) & pt_node_new ) );
    
    /* init the node */
    pt_node_new->ui1_col = ui1_col;
    
    /* init tsa context */
    x_memcpy( & t_tsa_init.t_info, pt_init_info, sizeof ( ANM_TSA_INFO_T ) );
    t_tsa_init.pw2s_buf    = pt_node_new->w2s_buf;
    t_tsa_init.ui2_len_buf = pt_ctx->ui2_tsa_text_buf_len;
    
    i4_ret_fct = anm_tsa_init( &pt_node_new->t_ctx_tsa, pb_req_timer, pui4_ms_timer, &t_tsa_init );
    if ( WGL_IS_FAIL( i4_ret_fct ) )
    {
        INFORM_ERR( i4_ret_fct );
        INFORM_ERR( wgl_npm_free ( & pt_ctx->t_npm, (VOID *) pt_node_new ) );
        return i4_ret_fct;
    }
    
    /* link the node */
    DLIST_INSERT_TAIL( pt_node_new, &pt_ctx->t_dlst_nodes, t_link );
    
    *ppt_ctx_tsa = & pt_node_new->t_ctx_tsa;
    
    return WGLR_OK;
}        

INT32 lb_hetsm_free_tsa(
    IN OUT  LB_HETSM_CTX_T*             pt_ctx,
    IN      UINT8                       ui1_col)
{
    INT32           i4_ret;
    HETSM_NODE_T    *pt_node;
    BOOL            b_found;
    
    /* check state */
    CHECK_VALID( HETSM_IN_VALID_STATE ( pt_ctx ), WGLR_INV_STATE );
    
    /* find the target node */
    b_found = FALSE;
    pt_node = NULL;
    DLIST_FOR_EACH( pt_node, & pt_ctx->t_dlst_nodes, t_link )
    {
        if ( ui1_col == pt_node->ui1_col )
        {
            b_found = TRUE;
            break;
        }
    }
    
    CHECK_VALID( b_found, WGLR_INV_ARG );

    /* remove the node from list */
    DLIST_REMOVE( pt_node, & pt_ctx->t_dlst_nodes, t_link );

    /* deinit the TSA context */
    INFORM_ERR( anm_tsa_deinit( & pt_node->t_ctx_tsa ) );

    /* free the node */
    CATCH_FAIL( wgl_npm_free( & pt_ctx->t_npm, (VOID *) pt_node ) );
    
    return WGLR_OK;
}        

static INT32 _hetsm_clear_all(
    IN OUT  LB_HETSM_CTX_T*             pt_ctx)
{
    HETSM_NODE_T    *pt_node;
    INT32           i4_ret;
    
    pt_node = DLIST_HEAD( & pt_ctx->t_dlst_nodes );
    while ( pt_node )
    {
         /* remove this node */
         DLIST_REMOVE( pt_node, & pt_ctx->t_dlst_nodes, t_link );
         
         /* deinit TSA context */
         INFORM_ERR( anm_tsa_deinit( & pt_node->t_ctx_tsa ) );
         
         /* free node */
         INFORM_ERR( wgl_npm_free( & pt_ctx->t_npm, (VOID *) pt_node ) );
         
         pt_node = DLIST_HEAD( & pt_ctx->t_dlst_nodes );
    }
     
     return WGLR_OK;   
}    


/*****************************************************************************
            list box specific functions
 *****************************************************************************/
BOOL lb_tool_twl_gc_fct(
    UINT16                  ui2_id,
    VOID*                   pv_data,
    VOID*                   pv_tag)
{
    LB_TR_T*        pt_tr     = (LB_TR_T *) pv_data;
    UINT32          ui4_ms_tk = x_os_get_sys_tick_period();
    
    if ( (pt_tr->ui4_tk_req * ui4_ms_tk + pt_tr->ui4_ms_delay + LB_TR_MAX_EXPIRED_TIME) <
         ( x_os_get_sys_tick() * ui4_ms_tk ) )
    {
        return TRUE;
    }
    
    return FALSE;
}        

