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
 * $RCSfile: _sm_utils.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/14 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: f8ae3234bfa666bf9e029ab3baeae46e $
 *
 * Description: 
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "dbg/dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "handle/handle.h"
#include "os/inc/x_os.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/_sm_utils.h"
#include "time_msrt/def_time_msrt_level.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#ifdef DEBUG
#define SM_DBG_RECORD_MAX_NUM    32
#define SM_DBG_RECORD_MAX_LEN    128
#define MAX_SM_DBG_BUFF_LEN     1024

static UINT32   gui4_module_mask = SM_DBG_MOD_ALL;
static UINT16   gui2_trace_lvl = DBG_INIT_LEVEL_MW_STRM_MNGR;

static HANDLE_T h_dbg_lock = NULL_HANDLE;
static UINT8    gui1_curr_record = 0;
static CHAR     as_sm_dbg_records[SM_DBG_RECORD_MAX_NUM][SM_DBG_RECORD_MAX_LEN] =
{ 
    "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",
    "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",
    "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",
    "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0"
};

static CHAR s_sm_dbg_buff[MAX_SM_DBG_BUFF_LEN];

#endif /* DEBUG */

#ifdef SM_ASH_NO_SIG_AUTO_MUTE
static INT8 i1_lost_rate_up_bound = 30;
static INT8 i1_lost_rate_low_bound = 20;
static INT8 i1_error_rate_bound = 5;
static INT8 i1_react_freq = 2;
#endif
#ifdef TIME_MEASUREMENT
static UINT16 ui2_sm_tms_level = TMS_INIT_LEVEL_MW_STRM_MNGR;
#endif /* TIME_MEASUREMENT */

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

static BOOL drv_clone_hdl_free( HANDLE_T       h_handle,
                                HANDLE_TYPE_T  e_type,
                                VOID*          pv_obj,
                                VOID*          pv_tag,
                                BOOL           b_req_handle )
{
    if ( b_req_handle == TRUE || e_type != SM_HT_DRV_HDL_CLONE ||
         !pv_obj )
    {
        return FALSE;    
    }
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
UINT32 sm_get_dbg_module_mask( VOID )
{
#ifdef DEBUG
    return gui4_module_mask;
#else
    return 0;
#endif
}

VOID sm_set_dbg_module_mask( UINT32 ui4_module_mask )
{
#ifdef DEBUG
    gui4_module_mask = ui4_module_mask;
#endif
}

VOID* sm_get_sess_obj( HANDLE_T h_sess )
{
    INT32                   i4_ret;
    HANDLE_TYPE_T           e_hdl_type;
    SM_SESSION_T*           pt_sess_obj = NULL;
    
    i4_ret = handle_get_type_obj( h_sess,
                                  &e_hdl_type,
                                  (VOID**)&pt_sess_obj);    
    if ( i4_ret != HR_OK || e_hdl_type != SM_HT_SESSION )
    {
        return NULL;
    }
    if ( !pt_sess_obj )
    {
        SM_ABORT( SM_DBG_ABRT_INV_SESS_OBJ );
    }
    
    return pt_sess_obj;
}

VOID* sm_get_sess_hdlr_obj( HANDLE_T h_sess, VOID** ppv_sess_obj )
{
    INT32                   i4_ret;
    HANDLE_TYPE_T           e_hdl_type;
    SM_SESSION_T*           pt_sess_obj = NULL;
    SM_SESS_HANDLER_T*      pt_sess_hdlr_obj = NULL;

    pt_sess_obj = sm_get_sess_obj( h_sess );
    if ( !pt_sess_obj )
    {
        return NULL;    
    }
    if ( ppv_sess_obj )
    {
        *ppv_sess_obj = pt_sess_obj;
    }
        
    i4_ret = handle_get_type_obj( pt_sess_obj->h_sess_hdlr,
                                  &e_hdl_type,
                                  (VOID**)&pt_sess_hdlr_obj);    
    if ( i4_ret != HR_OK || e_hdl_type != SM_HT_SESS_HANDLER )
    {
        return NULL;
    }
    if ( !pt_sess_hdlr_obj )
    {
        SM_ABORT( SM_DBG_ABRT_INV_HANDLER_OBJ );
    }
    
    return pt_sess_hdlr_obj;
}

VOID* sm_get_strm_obj( HANDLE_T h_stream )
{
    INT32              i4_ret;
    HANDLE_TYPE_T      e_hdl_type;
    SM_STREAM_T*       pt_strm_base = NULL;

    i4_ret = handle_get_type_obj( h_stream,
                                  &e_hdl_type,
                                  (VOID**)&pt_strm_base);    
    if ( i4_ret != HR_OK || e_hdl_type != SM_HT_STREAM )
    {
        return NULL;
    }
    if ( !pt_strm_base )
    {
        SM_ABORT( SM_DBG_ABRT_INV_STRM_OBJ );
    }

    return pt_strm_base;
}

VOID* sm_get_strm_hdlr_obj( HANDLE_T h_stream, VOID** ppv_strm_obj )
{
    INT32                   i4_ret;
    HANDLE_TYPE_T           e_hdl_type;
    SM_STREAM_T*            pt_strm_base = NULL;
    SM_HANDLER_T*           pt_hdlr_obj = NULL;

    pt_strm_base = sm_get_strm_obj( h_stream );
    if ( !pt_strm_base )
    {
        return NULL;    
    }
    if ( ppv_strm_obj )
    {
        *ppv_strm_obj = pt_strm_base;
    }
    
    i4_ret = handle_get_type_obj( pt_strm_base->h_handler,
                                  &e_hdl_type,
                                  (VOID**)&pt_hdlr_obj);    
    if ( i4_ret != HR_OK || e_hdl_type != SM_HT_HANDLER )
    {
        return NULL;
    }
    if ( !pt_hdlr_obj )
    {
        SM_ABORT( SM_DBG_ABRT_INV_HANDLER_OBJ );
    }
    
    return pt_hdlr_obj;
}

INT32 sm_lock( HANDLE_T h_lock )
{
    INT32   i4_ret;
    
    i4_ret = x_sema_lock( h_lock, X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
       return SMR_INTERNAL_ERROR;
    }
    
    return SMR_OK;
}

VOID sm_unlock( HANDLE_T h_lock )
{
    INT32   i4_ret;
    
    i4_ret = x_sema_unlock( h_lock );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE );
    }
}

RM_COND_T sm_rm_cond_filter( RM_COND_T e_cond, COND_GRP_T  e_cond_grp )
{
    return rm_cond_chg( e_cond, 0, e_cond_grp);
}

HANDLE_T sm_drv_hdl_clone( HANDLE_T h_driver )
{
    INT32      i4_ret;
    HANDLE_T   h_clone;
    
    i4_ret = handle_alloc( SM_HT_DRV_HDL_CLONE,
                           (VOID*) h_driver,
                           (VOID*) NULL,
                           (handle_free_fct) drv_clone_hdl_free,
                           (HANDLE_T*) &h_clone);    
    if ( i4_ret != HR_OK )
    {
        return NULL_HANDLE;    
    }
    
    return h_clone;
}

HANDLE_T sm_drv_hdl_clone_get_hdl( HANDLE_T h_clone )
{
    INT32         i4_ret;
    HANDLE_T      h_real;
    HANDLE_TYPE_T t_hdl_type;
                    
    i4_ret = handle_get_type_obj( h_clone,
                                  (HANDLE_TYPE_T*)&t_hdl_type,
                                  (VOID**)&h_real );
    if ( i4_ret != HR_OK || t_hdl_type != SM_HT_DRV_HDL_CLONE )
    {
        return NULL_HANDLE;
    }
    
    return h_real;
}

VOID sm_dbg_dump_bin( const UINT8* pui1_buff, SIZE_T z_size )
{
#ifdef DEBUG
    UINT32 ui4_i = 0;
    
    for ( ui4_i = 0; ui4_i < z_size; ++ui4_i )
    {
        x_dbg_stmt( "%.2X ", pui1_buff[ui4_i] );
    }
#endif
}

INT32 sm_dbg_msg ( 
    const CHAR*  ps_format, ...)
{
    INT32   i4_return = DBGR_OK;

#ifdef DEBUG
    VA_LIST arg_list;

    i4_return = DBGR_INV_ARG;

    if (ps_format != NULL)
    {
        i4_return = DBGR_OK;

        /* Bug-26  01/05/2005  ffr. Don't use the old API "x_va_start". Use the */
        /* new macro VA_START.                                                  */
        VA_START (arg_list, ps_format);

        sm_dbg_msg_va( ps_format, arg_list);

        VA_END (arg_list);
    }
#else
    i4_return = DBGR_NOT_ENABLED;
#endif

    return (i4_return);
}

INT32 sm_dbg_msg_va ( 
    const CHAR*  ps_format, 
    VA_LIST      arg_list )
{
    INT32   i4_ret = SMR_OK;

#ifdef DEBUG

    i4_ret = SMR_INV_ARG;

    if (ps_format != NULL)
    {
        i4_ret = DBGR_OK;

        /* Wait for other debug statement to be finished. */
        SM_LOCK( h_dbg_lock );

        /* Create debug statement string and send it to output device. */
        x_vsprintf (&(s_sm_dbg_buff [0]), ps_format, arg_list);

        /* If the debug statement string exceeds the maximum string */
        /* buffer ength then abort because there has most likely   */
        /* memory trashing taken place. Very, very bad.            */
        if (x_strlen ((const CHAR*) &(s_sm_dbg_buff [0])) > (MAX_SM_DBG_BUFF_LEN - 1))
        {
            SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
        }
        
        /* record the message */
        if ( x_strlen( s_sm_dbg_buff ) >= SM_DBG_RECORD_MAX_LEN )
        {
            x_memcpy( as_sm_dbg_records[gui1_curr_record], 
                      s_sm_dbg_buff, 
                      (SM_DBG_RECORD_MAX_LEN - 1) );
            as_sm_dbg_records[gui1_curr_record][(SM_DBG_RECORD_MAX_LEN - 1)] = '\0';
        }
        else
        {
            x_strcpy( as_sm_dbg_records[gui1_curr_record], s_sm_dbg_buff );
        }
        gui1_curr_record = (gui1_curr_record + 1) % SM_DBG_RECORD_MAX_NUM;
        
        SM_UNLOCK( h_dbg_lock );
    }
#else
    i4_ret = SMR_OK;
#endif

    return (i4_ret);
}

INT32 sm_dbg_init( VOID )
{
#ifdef DEBUG
    INT32 i4_ret;
    
    i4_ret = x_sema_create( &h_dbg_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK || h_dbg_lock==NULL_HANDLE )
    {
        return SMR_INSUFFICIENT_MEMORY;   
    }
#endif /* DEBUG */
    
    return SMR_OK;
}

VOID sm_dbg_dump_records( VOID )
{
#ifdef DEBUG
    INT32   i4_i = 0;
    
    for( i4_i = 0; i4_i < SM_DBG_RECORD_MAX_NUM; ++i4_i )
    {
        x_dbg_stmt( "[%.2d]%s", i4_i,
                    as_sm_dbg_records[((i4_i+gui1_curr_record)%SM_DBG_RECORD_MAX_NUM)] );
    }
#endif /* DEBUG */
}

VOID sm_dbg_set_trace_level( UINT16 ui2_trace_lvl )
{
#ifdef DEBUG
    SM_LOCK( h_dbg_lock );
    
    gui2_trace_lvl = ui2_trace_lvl;
    
    SM_UNLOCK( h_dbg_lock );
#endif /* DEBUG */
}

VOID sm_dbg_set_module_mask( UINT32 ui4_module_mask )
{
#ifdef DEBUG
    SM_LOCK( h_dbg_lock );
    
    gui4_module_mask = ui4_module_mask;
    
    SM_UNLOCK( h_dbg_lock );
#endif /* DEBUG */
}

UINT16 sm_get_trace_level( VOID )
{
#ifdef DEBUG
    return gui2_trace_lvl;
#else
    return DBG_LEVEL_NONE;
#endif /* DEBUG */
}

VOID sm_dbg_get_trace_info( 
    UINT16* pui1_trace_lvl,
    UINT32* pui4_module_mask )
{
#ifdef DEBUG
    if ( pui1_trace_lvl )
    {
        *pui1_trace_lvl = gui2_trace_lvl;
    }
    if ( pui4_module_mask )
    {
        *pui4_module_mask = gui4_module_mask;
    }
#else
    if ( pui1_trace_lvl )
    {
        *pui1_trace_lvl = DBG_LEVEL_NONE;
    }
    if ( pui4_module_mask )
    {
        *pui4_module_mask = SM_DBG_MOD_NONE;
    }
#endif /* DEBUG */
}

const CHAR* sm_strm_cond_2_str( SM_COND_T             e_cond )
{
    static const CHAR*   as_strm_cond[] = 
    {
        "SM_COND_IGNORE",
        "SM_COND_OPENING",
        "SM_COND_OPENED",
        "SM_COND_STARTING",
        "SM_COND_STARTED",
        "SM_COND_STOPPING",
        "SM_COND_CLOSING",
        "SM_COND_CLOSED",
        "SM_COND_ERROR"
    };
    
    return as_strm_cond[(UINT32)e_cond];
}

const CHAR* sm_sess_cond_2_str( SM_COND_T             e_cond )
{
    static const CHAR*   as_sess_cond[] = 
    {
        "SM_SESS_COND_IGNORE",
        "SM_SESS_COND_OPENING",
        "SM_SESS_COND_OPENED",
        "SM_SESS_COND_STARTING",
        "SM_SESS_COND_STARTED",
        "SM_SESS_COND_STOPPING",
        "SM_SESS_COND_CLOSING",
        "SM_SESS_COND_CLOSED",
        "SM_SESS_COND_ERROR"
    };
    
    return as_sess_cond[(UINT32)e_cond];
}


#ifdef TIME_MEASUREMENT
UINT16 sm_get_tms_level( VOID )
{
    return ui2_sm_tms_level;
}

VOID sm_set_tms_level( UINT16 ui2_tms_level )
{
    ui2_sm_tms_level = ui2_tms_level;
}

#endif /* TIME_MEASUREMENT */
#ifdef SM_ASH_NO_SIG_AUTO_MUTE
VOID sm_set_as_force_mute_parm (INT8 i1_lost_up_bound , 
                    INT8 i1_lost_low_bound,
                    INT8 i1_error_bound,
                    INT8 i1_freq)
{

    i1_lost_rate_up_bound   =  i1_lost_up_bound;
    i1_lost_rate_low_bound =  i1_lost_low_bound;
    i1_error_rate_bound = i1_error_bound;
    i1_react_freq = i1_freq;
    return; 
}

VOID sm_get_as_force_mute_parm (INT8* i1_lost_up_bound ,     
                    INT8* i1_lost_low_bound,
                    INT8* i1_error_bound,
                    INT8* i1_freq)
{
    *i1_lost_up_bound =  i1_lost_rate_up_bound;    
    *i1_lost_low_bound = i1_lost_rate_low_bound;
    *i1_error_bound = i1_error_rate_bound;
    *i1_freq = i1_react_freq;
    return;
}
#endif

