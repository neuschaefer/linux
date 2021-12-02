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
/*------------------------------------------------------------------------
                    include files
 -----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef ANDROID
#include <sys/un.h>
#include <sys/socket.h>
#else
#include <sys/socket.h>
#include <sys/un.h>
#endif

#include <errno.h>
#include <time.h>
#include <sys/timeb.h>
#include <errno.h>

#include HEADER_COMMON_H


#include "ipc_common/_ipc_packer.h"
#include "_rpc_ipc_util.h"
#include "_ipc.h"

#define MAX_PATH 255
#define MAX_SV_NAME_SIZE 127

/*------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 -----------------------------------------------------------------------*/
/* Note: unix socket structure:
typedef unsigned char sa_family_t;

#define    _SOCKADDR_COMMON(sa_prefix)    \
   unsigned char sa_prefix##len;     \
   sa_family_t sa_prefix##family

Structure describing the address of an AF_LOCAL (aka AF_UNIX) socket.  
struct sockaddr_un
  {
    __SOCKADDR_COMMON (sun_);
    char sun_path[108];   // Path name.  
  };
*/
#define IPC_SUN_PATH_LEN(ptr) (sizeof((ptr)->sun_path))


#define SOCK_CHECK(stmt, i4_ret, check_exp)\
do\
{\
    i4_ret = (stmt);\
    if(!(check_exp))\
    {\
        IPC_ERR(" %s = %d, errno:%d\n", #stmt, i4_ret, errno);\
    }\
}while(0)


/*  Message:
 *
 *  +-----------+  <--- size of message
 *  | DATA size |
 *  +-----------+
 *  | Type      |
 *  +-----------+
 *  |           |
 *  | MESSAGE   |
 *  +-----------+
 *
 *  MESSAE: OP: Packed argument data;
 */

#define MAX_MSG_SIZE 8192

typedef struct IPC_CFG_T_
{
    CHAR ps_sock_path[MAX_PATH + 1];
    BOOL b_log;
    
}IPC_CFG_T;

typedef struct IPC_STACK_ITEM_T_
{
    BOOL  b_local;
    const CHAR * ps_op;
    struct IPC_STACK_ITEM_T_ * pt_next;
    struct IPC_STACK_ITEM_T_ * pt_prev;
}IPC_STACK_ITEM_T;


typedef struct _IPC_MSG_BASE_T_
{
    SIZE_T      z_size;
    UINT32      ui4_type;
    
}IPC_MSG_BASE_T;

typedef struct IPC_SESS_T_
{
    
    IPC_ID_T t_id;
    
    ipc_close_fct            pf_close;
    ipc_proc_msg_fct         pf_proc_msg;

    union {
        ipc_proc_rpc_op_fct  pf_proc_op;
        ipc_proc_rpc_cb_fct  pf_proc_cb;
    }u;

    /* Socket */
    int      i4_it_sock;
    int      i4_ct_sock;
    /* 
     * Semaphore for protect message body, it's length and type 
     * sending in different threads
     */
    VOID *   pv_send_sem;

    /*
     *  Synchronize multiple threads except IPC Thread
     *  accessing same IPC ID.
     */
    VOID *   pv_sync_mtx;
    
    /* Table management, protected by pv_tbl_lock */
    struct  IPC_SESS_T_ * pt_next;
    
    /*Referenced count */
    UINT32  ui4_refc;

    /* The ipc thread of this session */
    VOID *  pv_ipc_thd;

    /* THis is for User store private data */
    VOID *  pv_tag;

    INT32   i4_depth_in_ipc_thread;
    INT32   i4_depth_in_ctx_thread;

    INT32           i4_pending_in_ctx_thread;
    const CHAR *    ps_cur_op_in_ctx_thread;
    INT32           i4_pending_in_ipc_thread;
    const CHAR *    ps_cur_op_in_ipc_thread;

    IPC_STACK_ITEM_T * pt_stack_top_in_ctx_thread;
    IPC_STACK_ITEM_T * pt_stack_top_in_ipc_thread;
}IPC_SESS_T;

/*------------------------------------------------------------------------
                    functions declarations
 -----------------------------------------------------------------------*/
static VOID _set_cur_id(IPC_ID_T t_id);

static IPC_ID_T _get_cur_id();

static INT32 _init_sess_tbl(UINT32 ui4_num);

static IPC_SESS_T * _get_sess(IPC_ID_T t_id, BOOL b_add_ref);

static IPC_ID_T _alloc_id(
    VOID *               pv_tag,
    ipc_close_fct        pf_close,
    ipc_proc_msg_fct     pf_proc_msg,
    ipc_proc_rpc_op_fct  pf_proc_rpc_op,
    ipc_proc_rpc_cb_fct  pf_proc_rpc_cb,
    int                  i4_it_sock,
    int                  i4_ct_sock);

static BOOL _self_is_ipc_thd(IPC_SESS_T * pt_sess);

static INT32 _send_msg(
    IPC_SESS_T * pt_sess,
    UINT32 ui4_msg_type,
    VOID * pv_msg,
    SIZE_T z_size);

static INT32 _recv_msg(
    IPC_SESS_T *    pt_sess,
    UINT32 *        pui4_msg_type,
    VOID *          pv_msg,
    SIZE_T *        pz_size,
    UINT32          ui4_timeout);

static INT32 _proc_ret(
    IPC_SESS_T *    pt_sess, 
    VOID *          pv_msg,
    SIZE_T          z_msg_size, 
    ARG_DESC_T *    pt_return);

static INT32 _proc_cb(
    IPC_SESS_T *    pt_sess, 
    VOID *          pv_msg, 
    size_t          z_msg_size);

static INT32 _proc_op(
    IPC_SESS_T *    pt_sess, 
    VOID *          pv_msg, 
    size_t          z_msg_size);

static void  _ipc_thd(VOID * pv_arg);

static INT32 _start_ipc_thd(IPC_ID_T t_id);

static VOID _ipc_listen_thd(VOID * pv_arg);

static INT32 _proc_loop(
    IPC_SESS_T *    pt_sess,
/*    VOID *          pv_msg,*/
    ARG_DESC_T *    pt_return,
    UINT32          ui4_timeout);

/*------------------------------------------------------------------------
                    data declarations
 -----------------------------------------------------------------------*/
/* Static APIs and variables for sess table.
 * IPC id is the index of table.
 */
static IPC_SESS_T * pt_free;
static IPC_SESS_T * pt_sess_tbl;
static SIZE_T  ui4_max_sess_num = 0;

static VOID * pv_tbl_lock;
static VOID * pv_key_sess = NULL;
static VOID * pv_key_client_auto_close;
                   
static IPC_CFG_T t_cfg;                   

BOOL   g_b_log;
char * g_ps_exe_name = "Not Specified";

static SIZE_T  z_used_id = 0;


/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Push the op/cb into RPC calling stack. 
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static IPC_STACK_ITEM_T * _stack_push_op(
    IPC_STACK_ITEM_T * pt_cur_top,
    const CHAR * ps_op,
    BOOL b_local)
{
    IPC_STACK_ITEM_T * pt_top;

    pt_top = RPC_MALLOC(sizeof(IPC_STACK_ITEM_T));
    IPC_ASSERT(pt_top != NULL);
    
    pt_top->pt_next = pt_cur_top;
    pt_top->ps_op   = ps_op;
    pt_top->b_local = b_local;
    pt_top->pt_next = pt_top->pt_prev = NULL;
    
    if(pt_cur_top != NULL)
    {
        pt_cur_top->pt_next = pt_top;
        pt_top->pt_prev     = pt_cur_top;
    }
    return pt_top;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Pop the op/cb from RPC calling stack. 
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static IPC_STACK_ITEM_T * _stack_pop_op(
    IPC_STACK_ITEM_T * pt_cur_top)
{
    IPC_STACK_ITEM_T * pt_top = NULL;

    if(pt_cur_top != NULL)
    {
        pt_top = pt_cur_top->pt_prev;
        RPC_FREE(pt_cur_top);
    }
    
    return pt_top;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Destroy RPC calling stack. 
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _stack_destroy(IPC_STACK_ITEM_T * pt_cur_top)
{
    IPC_STACK_ITEM_T * pt_top = NULL;
    while(pt_cur_top != NULL)
    {
        pt_top = pt_cur_top->pt_prev;
        RPC_FREE(pt_cur_top);  
        pt_cur_top = pt_top;
    }    
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Trace RPC calling stack. 
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _trace_stack(IPC_STACK_ITEM_T * pt_cur_top)
{
    while(pt_cur_top != NULL)
    {
        IPC_LOG("  %s\n", pt_cur_top->ps_op);
        pt_cur_top = pt_cur_top->pt_prev;
    }
}

/* 
 * FIXME : 
 * 
 */
 /*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Set IPC id as "current" id in current thread. IPC thread will
 * set current id to what it is processing for. Client may use this as "default
 * id". FIXME: If client has several links, which one should be the default one is not
 * decided. (Currently is the last one).
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _set_cur_id(IPC_ID_T t_id)
{
    rpcu_os_tls_set(pv_key_sess, (VOID *)t_id);
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Auto close the client if client dose not closed when client 
 * thread exits.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _ipc_client_auto_close(VOID * pv_key)
{
    IPC_ID_T t_id = (IPC_ID_T)pv_key;    
    ipc_close_client(t_id);
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Get current id
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static IPC_ID_T _get_cur_id()
{
    IPC_ID_T t_id;

    if (pv_key_sess == NULL)
    {
        return IPCR_NOT_OPENED;
    }
    
    t_id = (IPC_ID_T)rpcu_os_tls_get(pv_key_sess);
    
    if(t_id == 0)
    {        
        return IPCR_NOT_OPENED;
    }
    return t_id;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Init session table with a maximal size and some resources . 
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 
_init_sess_tbl(UINT32 ui4_num)
{

    UINT32 ui4_i;
    
    rpcu_os_mutex_create(&pv_tbl_lock);    

    ui4_max_sess_num = ui4_num;
    
    pt_sess_tbl = RPC_MALLOC(sizeof(IPC_SESS_T) * ui4_num);
    if(pt_sess_tbl == NULL)
    {
        return IPCR_OUTOFMEMORY;
    }    
    memset(pt_sess_tbl, 0, sizeof(IPC_SESS_T) * ui4_num);   
    
    /* Default ID 0 is used for special purpose.  */
    for(ui4_i = 1; ui4_i < ui4_num; ui4_i ++)
    {
        pt_sess_tbl[ui4_i].t_id     = ui4_i;
        pt_sess_tbl[ui4_i].ui4_refc = 0;
        if(ui4_i < ui4_num - 1)
        {
            pt_sess_tbl[ui4_i].pt_next = &pt_sess_tbl[ui4_i + 1];
        }
        else
        {
            pt_sess_tbl[ui4_i].pt_next = NULL;
        }
    }

    pt_free = &pt_sess_tbl[1];

    return IPCR_OK;
}   


/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Allocate a new id from session table. If there is no free 
 * one, this will failed with return value 0.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/

static IPC_ID_T _alloc_id(
    VOID *               pv_tag,
    ipc_close_fct        pf_close,
    ipc_proc_msg_fct     pf_proc_msg,
    ipc_proc_rpc_op_fct  pf_proc_rpc_op,
    ipc_proc_rpc_cb_fct  pf_proc_rpc_cb,
    int                  i4_it_sock,
    int                  i4_ct_sock)
{
    IPC_SESS_T * pt_sess;

    rpcu_os_mutex_lock(pv_tbl_lock);
    
    if(pt_free != NULL)
    {
        pt_sess             = pt_free;
        pt_free             = pt_free->pt_next;        
        pt_sess->ui4_refc   = 1;
        rpcu_os_mutex_unlock(pv_tbl_lock);
    
        pt_sess->pf_close        = pf_close;
        pt_sess->pf_proc_msg     = pf_proc_msg;
        /*FIX here, for cb and op */
        if(pf_proc_rpc_op != NULL)
        {
            pt_sess->u.pf_proc_op   = pf_proc_rpc_op;
        }
        else if(pf_proc_rpc_cb != NULL)
        {
            pt_sess->u.pf_proc_cb   = pf_proc_rpc_cb;
        }
        rpcu_os_mutex_create(&pt_sess->pv_sync_mtx);
       
        pt_sess->i4_it_sock         = i4_it_sock;
        pt_sess->i4_ct_sock         = i4_ct_sock;
        pt_sess->pt_next            = NULL;       
        
        pt_sess->pv_tag             = pv_tag;       
        
        z_used_id ++;
        
        IPC_INFO("New IPC ID (Count %d)\n", z_used_id);
        
        pt_sess->pt_stack_top_in_ctx_thread = NULL;
        pt_sess->pt_stack_top_in_ipc_thread = NULL;
    
    }
    else
    {
        rpcu_os_mutex_unlock(pv_tbl_lock);
        /* Or resize */
        return 0;
    }
    return pt_sess->t_id;
}


/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Get the session by id and it may inc reference count.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static IPC_SESS_T * _get_sess(IPC_ID_T t_id, BOOL b_add_ref)
{
    IPC_SESS_T * pt_sess;
    
    if(t_id == IPC_DEFAULT_ID)
    {
        t_id = _get_cur_id();
    }
    
    if(t_id < 0 || t_id > ui4_max_sess_num)
    {           
        return NULL;    
    }
    
    rpcu_os_mutex_lock(pv_tbl_lock);
    
    pt_sess = &pt_sess_tbl[(INT32)t_id];

    /* Get session which is freed */
    if(pt_sess->ui4_refc == 0)
    {
        rpcu_os_mutex_unlock(pv_tbl_lock);                
        return NULL;
    }
    if(b_add_ref == TRUE)
    {
        pt_sess->ui4_refc ++;
    }
    rpcu_os_mutex_unlock(pv_tbl_lock);
    return pt_sess;
}         

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Unref a session. This will descrease the reference count.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
#if 0
static VOID _unref_sess(IPC_SESS_T * pt_sess)
{    
    IPC_ASSERT(pt_sess != NULL);
    
    rpcu_os_mutex_lock(pv_tbl_lock);
    
    pt_sess->ui4_refc --;

    if(pt_sess->ui4_refc == 0)
    {
        IPC_TL_LOG("Unref the sess to 0, RPC_FREE id: %d\n", (int)pt_sess->t_id);
        /* Clean the resource */

        rpcu_os_mutex_delete(pt_sess->pv_sync_mtx);

        _stack_destroy(pt_sess->pt_stack_top_in_ctx_thread);
        _stack_destroy(pt_sess->pt_stack_top_in_ipc_thread);
        
        /* Add to free list */
        pt_sess->pt_next = pt_free;
        pt_free          = pt_sess;

        z_used_id --;
        IPC_INFO("Unref IPC ID (Count %d)\n", z_used_id);
        
    }
    
    rpcu_os_mutex_unlock(pv_tbl_lock);
}
#endif
static VOID _unref_sess( IPC_SESS_T * pt_sess )
{    
    IPC_ASSERT( pt_sess != NULL );
    
    if ( NULL == pt_sess ) 
    {
        IPC_INFO( "pt_sess is NULL\n" );
        return;
    }
    
    rpcu_os_mutex_lock( pv_tbl_lock );
    
    pt_sess->ui4_refc --;

    if ( pt_sess->ui4_refc == 0 )
    {
        pt_sess->pv_ipc_thd = 0;
		
        if ( pt_sess->i4_ct_sock >= 0 ) 
        {
	        IPC_INFO( "Close(pt_sess->i4_ct_sock: %d)\n", pt_sess->i4_ct_sock );   
	        close(pt_sess->i4_ct_sock);
	        pt_sess->i4_ct_sock = -1;
        }
		
        if ( pt_sess->i4_it_sock >= 0 ) 
        {
            IPC_INFO( "Close(pt_sess->i4_it_sock: %d)\n", pt_sess->i4_it_sock );
		    close( pt_sess->i4_it_sock );
		    pt_sess->i4_it_sock = -1;
        }

        rpcu_os_mutex_delete( pt_sess->pv_sync_mtx );

        _stack_destroy( pt_sess->pt_stack_top_in_ctx_thread );
        _stack_destroy( pt_sess->pt_stack_top_in_ipc_thread );
        
        /* Add to free list */
        pt_sess->pt_next = pt_free;
        pt_free          = pt_sess;

        z_used_id --;
        IPC_INFO( "Unref IPC ID (Count %d)\n", z_used_id );
    }
    
    rpcu_os_mutex_unlock( pv_tbl_lock );
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Set context thread socket to the IPC id if it is prepared.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _set_ct_sock(
    IPC_ID_T             t_id,
    int                  i4_ct_sock)
{
    IPC_SESS_T * pt_sess = _get_sess(t_id, TRUE);

    /*TODO Check ID here !!*/
    if(pt_sess == NULL)
    {
        return IPCR_INV_IPC_ID;
    }
    pt_sess->i4_ct_sock = i4_ct_sock;
    _unref_sess(pt_sess);

    return IPCR_OK;
}


/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Check current thread is the IPC thread for a session. 
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static BOOL _self_is_ipc_thd(IPC_SESS_T * pt_sess)
{
    VOID * pv_thd;

    pv_thd = rpcu_os_thread_self();

    if(pv_thd == pt_sess->pv_ipc_thd)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Push op into IPC calling stack.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _push_op(IPC_SESS_T * pt_sess, const CHAR * ps_op, BOOL b_local)
{
    if(_self_is_ipc_thd(pt_sess) == FALSE)
    {
        pt_sess->pt_stack_top_in_ctx_thread = 
            _stack_push_op(pt_sess->pt_stack_top_in_ctx_thread, ps_op, b_local);
    }
    else
    {
        pt_sess->pt_stack_top_in_ipc_thread = 
            _stack_push_op(pt_sess->pt_stack_top_in_ipc_thread, ps_op, b_local);
    }
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Pop op from IPC calling stack.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _pop(IPC_SESS_T * pt_sess)
{
    if(_self_is_ipc_thd(pt_sess) == FALSE)
    {
        pt_sess->pt_stack_top_in_ctx_thread = 
            _stack_pop_op(pt_sess->pt_stack_top_in_ctx_thread);
    }
    else
    {
        pt_sess->pt_stack_top_in_ipc_thread = 
            _stack_pop_op(pt_sess->pt_stack_top_in_ipc_thread);
    }
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Send message to the other side. If the current thread is IPC
 * thread this will use IPC thread socket. If the current thread is context 
 * thread this will use context thread socket of this session.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _send_msg(
    IPC_SESS_T * pt_sess,
    UINT32 ui4_msg_type,
    VOID * pv_msg,
    SIZE_T z_size)
{
    IPC_MSG_BASE_T  t_msg;
    INT32           i4_sock;
    int             i4_ret;
    if(pt_sess != NULL)
    {
        t_msg.z_size    = z_size;
        t_msg.ui4_type  = ui4_msg_type;
        
        if(_self_is_ipc_thd(pt_sess) == TRUE)
        {
            i4_sock = pt_sess->i4_it_sock;
        }
        else
        {
            i4_sock = pt_sess->i4_ct_sock;
        }
        
        SOCK_CHECK(send(i4_sock, &t_msg, sizeof(IPC_MSG_BASE_T), 0), i4_ret, i4_ret > 0);
        SOCK_CHECK(send(i4_sock, pv_msg, z_size, 0), i4_ret, i4_ret > 0);
        
        IPC_TL_LOG("Sent Msg type:%u size:%u\n", (unsigned)t_msg.ui4_type , t_msg.z_size);

        return IPCR_OK;
    }
            
    return IPCR_NOT_OPENED;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Receive message from the other side. If the current thread is IPC
 * thread this will use IPC thread socket. If the current thread is context 
 * thread this will use context thread socket of this session.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _recv_msg(
    IPC_SESS_T *    pt_sess,
    UINT32 *        pui4_msg_type,
    VOID *          pv_msg,
    SIZE_T *        pz_size,
    UINT32          ui4_timeout)
{
    IPC_MSG_BASE_T  t_msg_base;
    int             i4_ret;
    INT32           i4_sock;

    if(_self_is_ipc_thd(pt_sess) == TRUE)
    {       
        i4_sock = pt_sess->i4_it_sock;
    }
    else
    {
        i4_sock = pt_sess->i4_ct_sock;
    }

    SOCK_CHECK(recv(i4_sock, &t_msg_base, sizeof(IPC_MSG_BASE_T), 0), i4_ret, i4_ret > 0);
    if(i4_ret <= 0)
    {
        return IPCR_CNN_FAIL;
    }
    IPC_TL_LOG("Recved Msg base in IT, type:%u size:%u\n", (unsigned)t_msg_base.ui4_type , t_msg_base.z_size);
    if(t_msg_base.z_size > MAX_MSG_SIZE)
    {
        return IPCR_RCVD_ERR;
    }
    if(t_msg_base.z_size > 0)
    {
        SOCK_CHECK(recv(i4_sock, pv_msg, t_msg_base.z_size, 0), i4_ret, i4_ret > 0);
        if(i4_ret <= 0)
        {
            return IPCR_CNN_FAIL;
        }            
    }
    *pz_size        = t_msg_base.z_size;
    *pui4_msg_type  = t_msg_base.ui4_type;

    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Process return message. Memory will be updated.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _proc_ret(
    IPC_SESS_T *    pt_sess, 
    VOID *          pv_msg,
    SIZE_T          z_msg_size, 
    ARG_DESC_T *    pt_return)
{
    IPC_TL_LOG(" Proc ret \n");
    ipc_packer_writeback(pv_msg, z_msg_size, pt_return);
    return IPCR_OK;

}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Process callback message. This will call callback handler 
 * callback and send return message to the other side.This may enter 
 * recursive calling with ipc_do_op/ipc_do_cb  from hander.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _proc_cb(
    IPC_SESS_T *    pt_sess, 
    VOID *          pv_msg, 
    size_t          z_msg_size)
{
    CHAR *      ps_cb_type;
    VOID *      pv_cb_addr;
    UINT32      ui4_num_args_cb;
    ARG_DESC_T* pt_args_cb;
    ARG_DESC_T  t_return_cb;
    INT32       i4_ret;
    VOID *      pv_repack_msg;
    
    IPC_TL_LOG(" Proc CB \n");

    if((i4_ret = ipc_packer_translate(pv_msg,
                                    z_msg_size,
                                    &ps_cb_type,
                                    &pv_cb_addr,
                                    &pt_args_cb,
                                    &ui4_num_args_cb)) != IPCR_OK)
    {
        return i4_ret;
    }

    IPC_TL_LOG(" Call CB hdlr \n");
    _push_op(pt_sess, ps_cb_type, TRUE);
    if((i4_ret = pt_sess->u.pf_proc_cb(pt_sess->t_id,
                                       ps_cb_type,
                                       pv_cb_addr,
                                       ui4_num_args_cb,
                                       pt_args_cb,
                                       &t_return_cb)) != IPCR_OK)
    {
        return i4_ret;
    }

    _pop(pt_sess);

    pv_repack_msg = malloc(MAX_MSG_SIZE);
    if(pv_repack_msg == NULL)
    {        
        return IPCR_OUTOFMEMORY;
    }
    
    if((i4_ret = ipc_packer_pack_ret(pv_msg, 
                                     &t_return_cb, 
                                     pv_repack_msg, 
                                     MAX_MSG_SIZE, 
                                     &z_msg_size)) != IPCR_OK)
    {
        free(pv_repack_msg);
        return i4_ret;
    }   

    IPC_TL_LOG(" Send CB RET \n");
    if((i4_ret = _send_msg(pt_sess, IPCM_TYPE_RET, pv_repack_msg, z_msg_size)) != IPCR_OK)
    {
        free(pv_repack_msg);
        return i4_ret;
    }
    free(pv_repack_msg);
    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Process op message. This will call op handler and send return
 * message to the other side.
 * This may enter recursive calling with ipc_do_op/ipc_do_cb from hander.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _proc_op(
    IPC_SESS_T *    pt_sess, 
    VOID *          pv_msg, 
    size_t          z_msg_size)
{
    CHAR *      ps_op;
    VOID *      pv_cb_addr;
    UINT32      ui4_num_args;
    ARG_DESC_T* pt_args;
    ARG_DESC_T  t_return;
    INT32       i4_ret;
    VOID *      pv_repack_msg;
    IPC_TL_LOG(" Proc OP \n");

    if((i4_ret = ipc_packer_translate(pv_msg,
                                z_msg_size,
                                &ps_op,
                                &pv_cb_addr,
                                &pt_args,
                                &ui4_num_args)) != IPCR_OK)
    {
        return i4_ret;
    }
    
    IPC_TL_LOG(" Call OP hdlr \n");

    _push_op(pt_sess, ps_op, TRUE);
    
    if((i4_ret = pt_sess->u.pf_proc_op(pt_sess->t_id,
                                   ps_op,
                                   ui4_num_args,
                                   pt_args,
                                   &t_return)) != IPCR_OK)
    {
        return i4_ret;
    }
    _pop(pt_sess);
    
    pv_repack_msg = malloc(MAX_MSG_SIZE);
    if(pv_repack_msg == NULL)
    {        
        return IPCR_OUTOFMEMORY;
    }
    
    if((i4_ret = ipc_packer_pack_ret(pv_msg, 
                                     &t_return, 
                                     pv_repack_msg, 
                                     MAX_MSG_SIZE, 
                                     &z_msg_size)) != IPCR_OK)
    {
        free(pv_repack_msg);
        return i4_ret;
    }
    
    IPC_TL_LOG(" Send OP RET \n");
    
    if((i4_ret = _send_msg(pt_sess, IPCM_TYPE_RET, pv_repack_msg, z_msg_size)) != IPCR_OK)
    {
        free(pv_repack_msg);
        return i4_ret;
    }
    free(pv_repack_msg);
    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Process message from other side. This is root entry of the
 * message processing. It will redispatch message by message type. This may
 * enter recursive calling with ipc_do_op/ipc_do_cb in _proc_op/_proc_cb.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _proc_msg(
    IPC_SESS_T *    pt_sess,  
    VOID *          pv_msg,
    ARG_DESC_T *    pt_return,
    UINT32          ui4_timeout,
    BOOL *          pb_returned)
{    
    UINT32          ui4_msg_type;
    SIZE_T          z_msg_size;
    INT32           i4_ret;
    
    *pb_returned = FALSE;    
    
    if ((i4_ret = _recv_msg(pt_sess, &ui4_msg_type, pv_msg, &z_msg_size, ui4_timeout)) 
                 == IPCR_OK)
    {        
        switch (ui4_msg_type & IPCM_MASK_TYPE)
        {
            case IPCM_TYPE_OP:                
                _proc_op(pt_sess, pv_msg, z_msg_size);                
            break;
            case IPCM_TYPE_CB:
                _proc_cb(pt_sess, pv_msg, z_msg_size);
            break;
            case IPCM_TYPE_RET:                
                _proc_ret(pt_sess, pv_msg, z_msg_size, pt_return);            
                *pb_returned = TRUE;
            break;
            case IPCM_TYPE_ERROR:   
                return IPCR_RCVD_ERR;
            break;
            default:            
                /*TODO: ASSERT or call user's message proc function */
            break;
        }
    }
    return i4_ret;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: IPC thread main function. This will in a message processing
 * loop if the connection is not destroyed.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static void  _ipc_thd(VOID * pv_arg)
{
    VOID *          pv_msg;
    IPC_SESS_T *    pt_sess;
    IPC_ID_T        t_id;
    INT32           i4_ret;
    BOOL            b_returned;

    t_id    = *(IPC_ID_T *)pv_arg;

    /* There is a ref when calling alloc_id before IPC thread 
     * starts, so we don't add new ref.
     */    
    pt_sess = _get_sess(t_id, FALSE);
    IPC_ASSERT(pt_sess != NULL);
    if ( !pt_sess )
    {
        IPC_ERR(" IPC thread started failed! pt_sess is null!\n");
        return;
    }
    
    /* Update IPC thread handle */
    pt_sess->pv_ipc_thd = rpcu_os_thread_self();

    /* This thread's default ID is t_id */
    _set_cur_id(t_id);

    IPC_TL_LOG(" IPC thread started\n");

    pv_msg = RPC_MALLOC(MAX_MSG_SIZE);
    if(pv_msg != NULL)
    {    
        do
        {   
            i4_ret = _proc_msg(pt_sess, pv_msg, NULL, 0xffffffff, &b_returned);

            /* This should not happen, ignore or break?*/
            /*if(i4_ret == IPCR_OK && b_returned == TRUE)
            {            
                continue;
            }*/
            
        } while(i4_ret == IPCR_OK);    
     
        RPC_FREE(pv_msg);        

        IPC_TL_LOG("IPC Thread (id:%d) is being finished:\n", (int)t_id);    
    }        

    if(pt_sess->pf_close != NULL)
    {
        pt_sess->pf_close(pt_sess->pv_tag);
    }
    
    _unref_sess(pt_sess);
    
    IPC_TL_LOG("IPC Thread (id:%d) finshed:\n", (int)t_id);    
    
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Start IPC thread.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _start_ipc_thd(IPC_ID_T t_id)
{
    INT32 i4_ret;

    IPC_TL_LOG(" Start IPC thread\n");

    if((i4_ret = rpcu_os_create_thread(_ipc_thd, (VOID *)&t_id, sizeof(IPC_ID_T), 120, 1024 * 8)) != IPCR_OK)
    {
        IPC_ERR(" START IPC THREAD FAILED\n");       
    }

    return i4_ret;
}

/* CMMD > 0 used to transfor the server id as the seria number. */
#define IPC_ACK_CMD_REQ   ((INT32)0)
#define IPC_ACK_CMD_SUCC  ((INT32)-1)
#define IPC_ACK_CMD_FAIL  ((INT32)-2)

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: IPC listen thread main function. This will wait for connections
 * and confirm ACK for dual-link. It will allocate new IPC id for new connection.
 * This will exit if the server is closed.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _ipc_listen_thd(VOID * pv_arg)
{
    IPC_ID_T    t_id;
    IPC_ID_T    t_new_id;
    IPC_SESS_T* pt_sess;

    INT32       i4_new_sock;
    INT32       i4_ret;
    UINT32      i4_cmd;
    
    t_id = *(IPC_ID_T *)pv_arg;

    /* A ref count is reserved by alloc_id */
    pt_sess = _get_sess(t_id, FALSE);
    IPC_ASSERT(pt_sess != NULL);
    if ( !pt_sess )
    {
        IPC_ERR("_get_sess get null sess!\n");
        return;
    }
    
    while(1)
    {
        i4_new_sock = accept(pt_sess->i4_it_sock, NULL, NULL);

        IPC_TL_LOG("Accepted a sock, %d\n", (int)i4_new_sock);
        if(i4_new_sock < 0)
        {
            break;
        }
        
        /* FIX ME, TMP */
        if(recv(i4_new_sock, &i4_cmd, sizeof(INT32), 0) == sizeof(INT32))
        {
            if(i4_cmd == IPC_ACK_CMD_REQ)
            {
                t_new_id = _alloc_id(pt_sess->pv_tag,
                                     pt_sess->pf_close,
                                     pt_sess->pf_proc_msg,
                                     pt_sess->u.pf_proc_op,
                                     NULL,
                                     i4_new_sock,
                                     0);
                if(t_new_id == 0)
                {
                    IPC_ERR("Cannot alloc new IPC id for connection\n");
                    close(i4_new_sock);
                    continue;
                }
                if((i4_ret = _start_ipc_thd(t_new_id)) != IPCR_OK)
                {
                    IPC_ERR("Cannot start thread new IPC id for connection\n");
                    close(i4_new_sock);
                    continue;
                }
                send(i4_new_sock, &t_new_id, sizeof(INT32), 0);
            }
            else
            {
                if(_set_ct_sock((IPC_ID_T)i4_cmd, i4_new_sock) == IPCR_OK)
                {
                    i4_cmd = IPC_ACK_CMD_SUCC;
                    send(i4_new_sock, &i4_cmd, sizeof(INT32), 0);
                }
                else
                {
                    i4_cmd = IPC_ACK_CMD_FAIL;  
                    send(i4_new_sock, &i4_cmd, sizeof(INT32), 0);
                    IPC_ERR("Cannot finish ACK for connection\n");
                    close(i4_new_sock);
                    continue;
                }
            }
        }
        else
        { 
            IPC_ERR("ACK Error\n");
            close(i4_new_sock);
            continue;
        }
            
    }
    IPC_TL_LOG("Listen thread exists\n");

    _unref_sess(pt_sess);
    
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Process message in a loop for repeated CB messages. 
 * This will return if a return message is processed.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _proc_loop(
    IPC_SESS_T *    pt_sess,
    ARG_DESC_T *    pt_return,
    UINT32          ui4_timeout)
{
    INT32   i4_ret = IPCR_OK;


    BOOL    b_returned;
    VOID *  pv_msg;
    
    pv_msg = malloc(MAX_MSG_SIZE);
    if(pv_msg == NULL)
    {
        return IPCR_OUTOFMEMORY;
    }
    
    do
    {
        i4_ret = _proc_msg(pt_sess, pv_msg, pt_return, 0xffffffff, &b_returned);
        if(i4_ret == IPCR_OK && b_returned == TRUE)
        {
            break;
        }        
    }while(i4_ret == IPCR_OK);
    
    free(pv_msg);
    /*Fix here, i4_ret*/
    return i4_ret;
}


/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Enter a session. Context threads to call ipc_do_... must be 
 * synchronized. This entering/leaving is recursive by mutex. if current thread is 
 * IPC thread, currently nothing is done.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _enter_sess(IPC_SESS_T * pt_sess, const CHAR * ps_op, UINT32 ui4_timeout)
{
    INT32 i4_ret; 
       
    if(_self_is_ipc_thd(pt_sess) == FALSE)
    {
        i4_ret = rpcu_os_mutex_lock_timeout(pt_sess->pv_sync_mtx, ui4_timeout);
        if(i4_ret == IPCR_TIMEOUT)
        {
            return IPCR_TIMEOUT;
        }
    }
    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Leave a session. Context threads to call ipc_do_... must be 
 * synchronized. This entering/leaving is recursive by mutex. if current thread is 
 * IPC thread, currently nothing is done.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _leave_sess(IPC_SESS_T * pt_sess)
{
    INT32 i4_ret; 
    if(_self_is_ipc_thd(pt_sess) == FALSE)
    {        
        i4_ret = rpcu_os_mutex_unlock(pt_sess->pv_sync_mtx);
        IPC_ASSERT(i4_ret == IPCR_OK);        
    }    
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Trace IPC stack.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
VOID ipc_trace_stack()
{
    UINT32 ui4_i;
   
    rpcu_os_mutex_lock(pv_tbl_lock);
    for(ui4_i = 0; ui4_i < ui4_max_sess_num; ui4_i ++)
    {
        if(pt_sess_tbl[ui4_i].ui4_refc != 0)
        {
           
            if(pt_sess_tbl[ui4_i].pt_stack_top_in_ctx_thread != NULL)
            {
                IPC_LOG("<%s> CT Stack: \n", g_ps_exe_name);
                _trace_stack(pt_sess_tbl[ui4_i].pt_stack_top_in_ctx_thread);
            }
            if(pt_sess_tbl[ui4_i].pt_stack_top_in_ipc_thread != NULL)
            {
                IPC_LOG("<%s> IT Stack: \n", g_ps_exe_name);         
                _trace_stack(pt_sess_tbl[ui4_i].pt_stack_top_in_ipc_thread);
            }
        }
    }
    rpcu_os_mutex_unlock(pv_tbl_lock);
    
    
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: IPC monitor thread main function.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _monitor_main(VOID * pv_arg)
{   

    while(1)
    {
        sleep(5);
        ipc_trace_stack();
        IPC_LOG("\n");
    }        
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Start IPC monitor thread.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _monitor_start()
{
    INT32 i4_ret;

    IPC_INFO(" Start IPC Monitor\n");

    if((i4_ret = rpcu_os_create_thread(
        _monitor_main, 
        NULL, 0, 120, 1024 * 8)) != IPCR_OK)
    {
        IPC_ERR(" START IPC MONITOR THREAD FAILED\n");       
    }
    

}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Set client close func and tag.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 ipc_set_close_fct(IPC_ID_T t_ipc_id, void * pv_tag, ipc_close_fct pf_close)
     /*Need chain? ..., void ** ppv_tag, ipc_close_fct * ppf_close*/
{
    IPC_SESS_T *    pt_sess = NULL;
    /*INT32           i4_ret  = IPCR_OK;*/

    pt_sess = _get_sess(t_ipc_id, TRUE);

    if(pt_sess != NULL)
    {
        pt_sess->pf_close = pf_close;
        pt_sess->pv_tag   = pv_tag;
        
        return IPCR_OK;
    }
    else
    {
        return IPCR_NOT_OPENED;
    }
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: This API is called by a client and requests the server to 
 *              perform an operation as specified in argument 'ps_op'. 
 *              This function is a synchronous operation and will only return 
 *              once the server has completed the requested operation.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 ipc_do_op (
    IPC_ID_T                  t_ipc_id,
    const CHAR*               ps_op,
    UINT32                    ui4_num_args,
    ARG_DESC_T*               pt_args,
    ARG_DESC_T*               pt_return,
    UINT32                    ui4_timeout,
    ipc_get_rpc_desc_fct      pf_get_rpc_desc,
    VOID*                     pv_tag)
{
    VOID *          pv_msg = NULL;
    SIZE_T          z_msg_size;
    IPC_SESS_T *    pt_sess = NULL;
    INT32           i4_ret = IPCR_OK;

    if(   ps_op     == NULL 
       || pt_return == NULL
       || (pt_args  == NULL && ui4_num_args != 0))
    {
        IPC_ERR("Invalid arg "
            "t_ipc_id:%d, "
            "ps_op:%s, "
            "pt_args:0x%x, ui4_num_args: %d,"
            "pt_return:0x%x\n",
            t_ipc_id,
            ps_op,
            pt_args, ui4_num_args,
            pt_return);
            
        return IPCR_INV_ARGS;
    }

    pt_sess = _get_sess(t_ipc_id, TRUE);

    if(pt_sess != NULL)
    {
        IPC_TL_LOG(" Start ipc_do_op in \n");
        pv_msg = RPC_MALLOC(MAX_MSG_SIZE);

        if(pv_msg != NULL)
        {   
            if((i4_ret = ipc_packer_pack(pv_msg,
                                  MAX_MSG_SIZE,
                                  ps_op,
                                  NULL,
                                  ui4_num_args,
                                  pt_args,
                                  pf_get_rpc_desc,
                                  pv_tag,
                                  &z_msg_size)) == IPCR_OK)
            {                
                if((i4_ret = _enter_sess(pt_sess, ps_op, ui4_timeout)) == IPCR_OK)
                {    
                    if((i4_ret = _send_msg(pt_sess, IPCM_TYPE_OP, pv_msg, z_msg_size)) == IPCR_OK)
                    {
                        _push_op(pt_sess, ps_op, FALSE);
                        
                        i4_ret = _proc_loop(pt_sess, pt_return, ui4_timeout);
                        
                        _pop(pt_sess);
                    }                    
                    _leave_sess(pt_sess);
                }                
            }
        }
        else
        {
            i4_ret = IPCR_OUTOFMEMORY;
        }
    }
    else
    {
        i4_ret = IPCR_NOT_OPENED;
    }

    if(pt_sess != NULL)
    {
        _unref_sess(pt_sess);
    }
    if(pv_msg != NULL)
    {
        RPC_FREE(pv_msg);
    }

    if(i4_ret != IPCR_OK)
    {
        IPC_ERR("ipc_do_op %s failed ret:%d\n", ps_op, i4_ret);
    }
    return i4_ret;         

}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Do callback to client. This function is a synchronous 
 *              operation and will only return once the client has completed 
 *              the requested asynchronous callback operation.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 ipc_do_cb(
    IPC_ID_T                  t_ipc_id,
    const CHAR*               ps_cb_type,
    VOID*                     pv_cb_addr,
    UINT32                    ui4_num_args,
    ARG_DESC_T*               pt_args,
    ARG_DESC_T*               pt_return,
    UINT32                    ui4_timeout,
    ipc_get_rpc_desc_fct      pf_get_rpc_desc,
    VOID*                     pv_tag)
{
    VOID *          pv_msg  = NULL;
    SIZE_T          z_msg_size;
    IPC_SESS_T *    pt_sess = NULL;
    INT32           i4_ret  = IPCR_OK;

    if(   ps_cb_type    == NULL 
       || pt_return     == NULL
       || (pt_args      == NULL && ui4_num_args != 0)
       || t_ipc_id      < 0
       || pv_cb_addr    == NULL)
    {
        IPC_ERR("Invalid arg "
                "t_ipc_id:%d, "
                "pv_cb_addr:0x%x, "
                "ps_cb_type:%s, "
                "pt_args:0x%x, ui4_num_args: %d,"
                "pt_return:0x%x\n",
                t_ipc_id,
                pv_cb_addr,
                ps_cb_type,
                pt_args, ui4_num_args,
                pt_return);
                
        return IPCR_INV_ARGS;
    }
    /* FIX Me, should I do this here or return error? 
     * Do it simple.
     */    
    if(pv_cb_addr == NULL)
    {
        return IPCR_OK;
    }
    
    pt_sess = _get_sess(t_ipc_id, TRUE);

    if(pt_sess != NULL)
    {
        IPC_TL_LOG(" Start ipc_do_op \n");
        pv_msg = RPC_MALLOC(MAX_MSG_SIZE);

        if(pv_msg != NULL)
        {   
            if((i4_ret = ipc_packer_pack(pv_msg,
                                  MAX_MSG_SIZE,
                                  ps_cb_type,
                                  pv_cb_addr,
                                  ui4_num_args,
                                  pt_args,
                                  pf_get_rpc_desc,
                                  pv_tag,
                                  &z_msg_size)) == IPCR_OK)
            {                
                if((i4_ret = _enter_sess(pt_sess, ps_cb_type, ui4_timeout)) == IPCR_OK)
                {
                    if((i4_ret = _send_msg(pt_sess, IPCM_TYPE_CB, pv_msg, z_msg_size)) == IPCR_OK)
                    {
                        _push_op(pt_sess, ps_cb_type, FALSE);
                        
                        i4_ret = _proc_loop(pt_sess, pt_return, ui4_timeout);
                        
                        _pop(pt_sess);
                    }
                    _leave_sess(pt_sess);
                }                
            }
        }
        else
        {
            i4_ret = IPCR_OUTOFMEMORY;
        }
    }
    else
    {
        i4_ret = IPCR_NOT_OPENED;
    }

    if(pt_sess != NULL)
    {
        _unref_sess(pt_sess);
    }
    if(pv_msg != NULL)
    {
        RPC_FREE(pv_msg);
    }   
    
    if(i4_ret != IPCR_OK)
    {
        IPC_ERR("ipc_do_cb %s failed ret:%d\n", ps_cb_type, i4_ret);
    }
    
    return i4_ret;        
}


/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: This API is called by a server and opens the server's communication port.
 * This will create listening socket and start listening thread.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
IPC_ID_T ipc_open_server (
    const CHAR*          ps_server,
    VOID *               pv_tag,
    ipc_close_fct        pf_close,
    ipc_proc_msg_fct     pf_proc_msg,
    ipc_proc_rpc_op_fct  pf_proc_rpc_op)
{

    IPC_ID_T            t_id;
    INT32               i4_ret;
    struct sockaddr_un  t_serv_addr;
    int                 i4_sock;

    if(ps_server == NULL || pf_proc_rpc_op == NULL)
    {
        return IPCR_INV_ARGS;
    }

    i4_sock  = socket(PF_UNIX, SOCK_STREAM, 0);
    if(i4_sock < 0)
    {
       IPC_ERR(" SOCK create errno:%d\n", (int)errno);
       return IPCR_CNN_FAIL;
       
    }
    
    if(strlen(t_cfg.ps_sock_path) + strlen(ps_server) >= sizeof(t_serv_addr.sun_path) - 1)
    {
        close(i4_sock);
        IPC_ERR("Too long IPC server name or path name\n");
        return IPCR_INV_SERVER;
    }
    
    t_serv_addr.sun_family = AF_UNIX;
    snprintf(t_serv_addr.sun_path, IPC_SUN_PATH_LEN(&t_serv_addr), "%s/%s", t_cfg.ps_sock_path, ps_server);
    
    mkdir(t_cfg.ps_sock_path, 0777);
    unlink(t_serv_addr.sun_path);
    
    if(bind(i4_sock, (const void *)&t_serv_addr, sizeof(struct sockaddr_un)) == -1)
    {
        close(i4_sock);
        IPC_ERR(" BIND ERRRRR, SOCK [%d] errno:%d\n", (int)i4_sock, (int)errno);
        return IPCR_CNN_FAIL;
    }

    /*FIXME*/
    listen(i4_sock, 256);
    
    /* Here there will be a ref count to sess and unrefed in ipc thread */
    t_id = _alloc_id(pv_tag, pf_close, pf_proc_msg, pf_proc_rpc_op, NULL, i4_sock, 0);
    if(t_id != (IPC_ID_T)0)
    {
        _set_cur_id(t_id);
        i4_ret = rpcu_os_create_thread(_ipc_listen_thd, (VOID *)&t_id, sizeof(IPC_ID_T), 120, 4096);
        if(i4_ret != IPCR_OK)
        {
            _unref_sess(_get_sess(t_id, FALSE));
            return i4_ret;
        }
    }
    else
    {        
        close(i4_sock);
        IPC_ERR(" Allocate ID failed\n");
    }
    return t_id;

}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: This will close server. Close all the linkage.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 ipc_close_server (IPC_ID_T  t_ipc_id)
{
    /* TODO: Close all the connections ? */
    IPC_SESS_T * pt_sess;

    if(t_ipc_id == RPC_DEFAULT_ID)
    {
        return IPCR_INV_ARGS;
    }
    
    pt_sess = _get_sess(t_ipc_id, TRUE);
    if(pt_sess != NULL)
    {
        shutdown(pt_sess->i4_it_sock, SHUT_RDWR);
        shutdown(pt_sess->i4_ct_sock, SHUT_RDWR);
        _unref_sess(pt_sess);           
        return IPCR_OK;
    }
    else
    {
        return IPCR_NOT_OPENED;
    }

}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: This API is called by a client and opens a communication link 
 *              to the server. One client may only open one communication link 
 *              with one server. However, one client may open multiple 
 *              communication links with different servers.
 *              This will connect to server by dual-connection. And it will send
 *              and reseive ACK messages to build dual connections. 
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
IPC_ID_T ipc_open_client (
    const CHAR*          ps_server,
    VOID *               pv_tag,
    ipc_close_fct        pf_close,
    ipc_proc_msg_fct     pf_proc_msg,
    ipc_proc_rpc_cb_fct  pf_proc_rpc_cb)
{
    struct sockaddr_un  t_serv_addr;
    int                 i4_it_sock  = 0;
    int                 i4_ct_sock  = 0;
    int                 i4_ret      = IPCR_OK;
    IPC_ID_T            t_id        = 0;
    INT32               i4_cmd      = 0;

    if(ps_server == NULL || pf_proc_rpc_cb == NULL)
    {
        return IPCR_INV_ARGS;
    }
    
    /* FixMe , how to find the size limit of sun_path */
    if(strlen(t_cfg.ps_sock_path) + strlen(ps_server) >= sizeof(t_serv_addr.sun_path) - 1)
    {        
        IPC_ERR("Too long IPC server name or path name\n");
        return IPCR_INV_SERVER;
    }
    
    t_serv_addr.sun_family = AF_UNIX;
    snprintf(t_serv_addr.sun_path, IPC_SUN_PATH_LEN(&t_serv_addr),  "%s/%s", t_cfg.ps_sock_path, ps_server);  
    
    i4_ct_sock  = socket(PF_UNIX, SOCK_STREAM, 0);
    if(i4_ct_sock < 0)
    {
       IPC_ERR(" SOCK create errno:%d\n", (int)errno);
       i4_ret = IPCR_CNN_FAIL;       
    }

    if(i4_ret == IPCR_OK)
    {
        i4_it_sock  = socket(PF_UNIX, SOCK_STREAM, 0);
        if(i4_it_sock < 0)
        {
           IPC_ERR(" SOCK create errno:%d\n", (int)errno);       
           i4_ret = IPCR_CNN_FAIL;
        }   
    }
    
    if(i4_ret == IPCR_OK)
    {
        i4_ret = connect(i4_ct_sock, (const void *)&t_serv_addr, sizeof(struct sockaddr_un));
        if(i4_ret < 0)
        {
            IPC_ERR(" SOCK Connection fail errno:%d\n", (int)errno);       
            i4_ret = IPCR_CNN_FAIL;
        }     
    }

    if(i4_ret == IPCR_OK)
    {
        i4_cmd = IPC_ACK_CMD_REQ;
        if(send(i4_ct_sock, &i4_cmd, sizeof(INT32), 0) <= 0)
        {
            IPC_ERR(" ACK Fail (%d)\n", (int)i4_cmd);    
            i4_ret = IPCR_CNN_FAIL;            
        }
    }
    
    if(i4_ret == IPCR_OK)
    {    
        if(recv(i4_ct_sock, &i4_cmd, sizeof(INT32), 0) != sizeof(INT32))
        {
            IPC_ERR(" ACK Fail (%d)\n", (int)i4_cmd);    
            i4_ret = IPCR_CNN_FAIL;            
        }
    } 

    if(i4_ret == IPCR_OK)
    {   
        if(connect(i4_it_sock, (const void *)&t_serv_addr, sizeof(struct sockaddr_un)) < 0)
        {
            IPC_ERR(" SOCK Connection fail errno:%d\n", (int)errno);               
            i4_ret = IPCR_CNN_FAIL;
        }
    }

    if(i4_ret == IPCR_OK)
    {  
        if(send(i4_it_sock, &i4_cmd, sizeof(INT32), 0) != sizeof(INT32))
        {
            IPC_ERR(" ACK Fail (%d)\n", (int)i4_cmd);    
            i4_ret = IPCR_CNN_FAIL;            
        }
    }

    if(i4_ret == IPCR_OK)
    {  
        if(     recv(i4_it_sock, &i4_cmd, sizeof(INT32), 0) != sizeof(INT32) 
            /*||  i4_cmd != IPC_ACK_CMD_SUCC*/)
        {
            IPC_ERR(" ACK Fail (%d)\n", (int)i4_cmd);    
            i4_ret = IPCR_CNN_FAIL;
        }
    }
    
    if(i4_ret == IPCR_OK)
    {  
        /* Here there will be a ref count to sess and unrefed in ipc thread */
        if((t_id = _alloc_id(pv_tag, 
                             pf_close, 
                             pf_proc_msg, 
                             NULL, 
                             pf_proc_rpc_cb, 
                             i4_it_sock, 
                             i4_ct_sock)) != (IPC_ID_T)0)
        {
            _set_cur_id(t_id);
            /* Use this to auto close the session if current thread exits */
            rpcu_os_tls_set(pv_key_client_auto_close, (VOID *)t_id);
            
            i4_ret = _start_ipc_thd(t_id);
        }
        else
        {
            i4_ret = IPCR_OUTOFMEMORY;
            IPC_ERR(" Allocate Client ID failed\n");            
        }
    }

    if(i4_ret == IPCR_OK)
    {
        return t_id;    
    }
    else
    {   
        /* Error handling */
        if(i4_it_sock != 0)
        {
            close(i4_it_sock);
        }
        if(i4_ct_sock != 0)
        {
            close(i4_ct_sock);
        }
        if(t_id != 0)
        {
            _unref_sess(_get_sess(t_id, FALSE));
        }
        return i4_ret;
    }
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: This API is called by a server and opens the server's communication port.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 ipc_close_client (IPC_ID_T  t_ipc_id)
{
    /* FIXME */
    IPC_SESS_T * pt_sess;
    
    pt_sess = _get_sess(t_ipc_id, TRUE);
    
    if(pt_sess != NULL)
    {
        /* 
         * This will awake the IPC thread, and then 
         * the IPC thread will do some cleaning work.         * 
         */
        shutdown(pt_sess->i4_it_sock, SHUT_RDWR);
        shutdown(pt_sess->i4_ct_sock, SHUT_RDWR);
        _unref_sess(pt_sess);
        return IPCR_OK;
    }
    else
    {
        return IPCR_NOT_OPENED;
    }
}


BOOL ipc_log_on()
{
    BOOL b_old = g_b_log;
    g_b_log = TRUE;    
    return b_old;
}

VOID ipc_log_reset(BOOL b_old)
{
    g_b_log = b_old;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Init ipc and read configuration file.Init the session table.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 ipc_init(UINT32 ui4_max_id, const OS_FNCT_T*  pt_os_fnct)
{
    INT32  i4_ret;

    FILE * pt_f;
    CHAR * ps_line = NULL;
    SIZE_T z_len;
    CHAR * pc_c;
    CHAR   ps_key[32];
    CHAR   ps_val[256];

    pt_f = fopen("/proc/self/cmdline", "rt");
    
    if(pt_f != NULL)
    {
        if(getline(&ps_line, &z_len, pt_f) > 0)
        {
            g_ps_exe_name = RPC_MALLOC(z_len + 1);
            if(g_ps_exe_name != NULL)
            {
                strncpy(g_ps_exe_name, ps_line, z_len);
            }
            else
            {
                g_ps_exe_name = "Too long";
            }
        }
        fclose(pt_f);
    }    

    strncpy(t_cfg.ps_sock_path, "/tmp", 5);
    g_b_log = FALSE;
    
    pt_f = fopen("mtk_ipc.cfg", "rt");

    
    if(pt_f != NULL)
    {
        while(getline(&ps_line, &z_len, pt_f) > 0)
        {
            for(pc_c = ps_line; (*pc_c != '\0') && (*pc_c ==' ' || *pc_c == '\t'); pc_c ++)
            {
            }
            if(*pc_c == '#')
            {
                continue;
            }
            if(sscanf(ps_line, "%31s %255s\n", ps_key, ps_val) == 2)
            {
                if(strncmp(ps_key, "sock_path", 9) == 0)
                {
                    strncpy(t_cfg.ps_sock_path, ps_val, ( strlen(ps_val)>MAX_PATH)?MAX_PATH:strlen(ps_val) );
                }      
                else if(strncmp(ps_key, "log", 3) == 0)
                {
                    if(strncmp(ps_val, "on", 2) == 0)
                    {
                        g_b_log = TRUE;           
                    }
                    else
                    {
                        g_b_log = FALSE;
                    }
                }
                else if(strncmp(ps_key, "monitor", 7) == 0)
                {
                    if(strncmp(ps_val, "on", 2) == 0)
                    {
                        _monitor_start();    
                    }               
                }
                IPC_LOG("Read cfg %s, V %s\n", ps_key, ps_val);
             }
        }
        fclose(pt_f);
    }
    
    
    rpcu_os_init(pt_os_fnct);
    
    if((pv_key_sess = rpcu_os_new_tls_key(NULL)) == NULL)
    {
        return IPCR_OSERR;
    }
    
    if((pv_key_client_auto_close = rpcu_os_new_tls_key(_ipc_client_auto_close)) == NULL)
    {
        IPC_ERR("error for create\n");
        return IPCR_OSERR;
    }
    
    if((i4_ret = _init_sess_tbl(ui4_max_id)) != IPCR_OK)
    {
        rpcu_os_delete_tls_key(pv_key_sess);    
    }  

    
    return i4_ret;
    
}


