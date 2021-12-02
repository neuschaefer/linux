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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/un.h>
#include <errno.h>

#include HEADER_COMMON_H

#include "_ipc.h"

#include "_rpc_ipc_util.h"
#include "rpc.h"

#define RPC_TRACE_HANDLER_TABLE



typedef struct _HASH_NODE_T
{
    /* Do we really need to copy the key?  
     * Or just assume that all the key is from
     * constant string.
     */
     
    CHAR * ps_key;
    VOID * pv_val;
    struct _HASH_NODE_T * pt_next;
    struct _HASH_NODE_T * pt_prev;
}HASH_NODE_T;

static UINT32           ui4_tbl_len_order;
static HASH_NODE_T **   ppt_tbl = NULL;


#ifdef RPC_TRACE_HANDLER_TABLE
static SIZE_T   *       pz_list_len = NULL;
static SIZE_T           z_max_list_len = 0;
static SIZE_T           z_total = 0;
#endif

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Hash function for op string. Simply use order to void % as
 * the barral size.
 * 
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static UINT32 _hash_func(const CHAR * ps_str, UINT32 ui4_order)
{
    UINT32 ui4_h;
    const CHAR * pc_c;
    
    ui4_h = 0;
    for(pc_c = ps_str; *pc_c != '\0'; pc_c ++)
    {
        ui4_h = ui4_h * 31 + *pc_c;
    }
    return ui4_h & ((1U << ui4_order) - 1);
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Init hash table for op/cb
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _init_hash(UINT32 ui4_order)
{
    UINT32 ui4_num = 1U << ui4_order;
    
    ui4_tbl_len_order = ui4_order;
    
    ppt_tbl       = RPC_CALLOC(ui4_num, sizeof(HASH_NODE_T *));
    
#ifdef RPC_TRACE_HANDLER_TABLE
    pz_list_len   = RPC_CALLOC(ui4_num, sizeof(SIZE_T));
#endif
    return RPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Add key to hash table.
 * This assume that the key is not in table
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _hash_add(const CHAR * ps_key, VOID * pv_val)
{
    UINT32 ui4_idx = _hash_func(ps_key, ui4_tbl_len_order);
    HASH_NODE_T * pt_node;
    
    if(pv_val == NULL)
    {
        return RPCR_INV_ARGS;
    }
    pt_node = RPC_MALLOC(sizeof(HASH_NODE_T));
    if(pt_node == NULL)
    {
        return RPCR_OUTOFMEMORY;
    }
    memset(pt_node, 0, sizeof(HASH_NODE_T));   
    pt_node->pv_val = pv_val;
    /* FIXME: Copy or assign */
    /* OPs should be constant string, ?? */
    pt_node->ps_key = (CHAR *)ps_key;        

    if(ppt_tbl[ui4_idx] == NULL)
    {
        ppt_tbl[ui4_idx] = pt_node;
    }
    else
    {
        pt_node->pt_next = ppt_tbl[ui4_idx];
        ppt_tbl[ui4_idx]->pt_prev = pt_node;
        ppt_tbl[ui4_idx] = pt_node;  
    }
    
#ifdef RPC_TRACE_HANDLER_TABLE
    z_total ++;    
    pz_list_len[ui4_idx] ++;
    if(pz_list_len[ui4_idx] > z_max_list_len)
    {
        z_max_list_len = pz_list_len[ui4_idx];
        IPC_TL_LOG("Handler table max list len:%u, total:%u\n", 
                z_max_list_len,
                z_total);
    }
#endif

    return RPCR_OK;
}
/*  */

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Get value of key from hash table.
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _hash_get(const CHAR * ps_key, VOID ** ppv_val)
{
    UINT32 ui4_idx = _hash_func(ps_key, ui4_tbl_len_order);
    HASH_NODE_T * pt_node;
    
    if(ppt_tbl[ui4_idx] == NULL)
    {
        return RPCR_NOT_FOUND;
    }
    else
    {
        for(pt_node = ppt_tbl[ui4_idx]; pt_node; pt_node = pt_node->pt_next)
        {
            if(strcmp(pt_node->ps_key, ps_key) == 0)
            {
                if(ppv_val)
                {
                    *ppv_val = pt_node->pv_val;
                }
                return RPCR_OK;
            }
        }
    }
    return RPCR_NOT_FOUND;
}

/* TODO: Implement the hash_remove, .... */


typedef enum RPC_HANDLER_TYPE_E_
{
    RPC_HANDLER_TYPE_OP = 0,
    RPC_HANDLER_TYPE_CB
}RPC_HANDLER_TYPE_E;

typedef struct RPC_HANDLER_T_
{
    RPC_HANDLER_TYPE_E e_type;
    union
    {
        rpc_op_hndlr_fct pf_op_hndlr;
        rpc_cb_hndlr_fct pf_cb_hndlr;
    } u;
}RPC_HANDLER_T;

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Init rpc handler component. 
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpc_handler_init()
{
    _init_hash(10);
    return RPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Register OP handler. If the OP is already in the table, it 
 * will return RPCR_EXIST.
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpc_reg_op_hndlr(    
    const CHAR *         ps_op,    
    rpc_op_hndlr_fct     pf_op_hndlr)
{

    RPC_HANDLER_T * pt_handler;

    if(ps_op == NULL || pf_op_hndlr == NULL)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_handler = RPC_MALLOC(sizeof(RPC_HANDLER_T));
    if(pt_handler == NULL)
    {
        return RPCR_OUTOFMEMORY;
    }
    pt_handler->e_type = RPC_HANDLER_TYPE_OP;
    pt_handler->u.pf_op_hndlr = pf_op_hndlr;
    
    if(_hash_get(ps_op, NULL) == IPCR_OK)
    {
        RPC_FREE(pt_handler);
        IPC_TL_LOG(" Try to register duplicate handler, %s\n", ps_op);
        return RPCR_EXIST;
    }
    return _hash_add(ps_op, pt_handler);
       
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Register CB handler. If the CB is already in the table, it 
 * will return RPCR_EXIST.
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpc_reg_cb_hndlr(    
    const CHAR *         ps_cb_type,    
    rpc_cb_hndlr_fct     pf_cb_hndlr)
{
    RPC_HANDLER_T * pt_handler;

    if(ps_cb_type == NULL || pf_cb_hndlr == NULL)
    {
        return RPCR_INV_ARGS;
    }

    pt_handler = RPC_MALLOC(sizeof(RPC_HANDLER_T));
    if(pt_handler == NULL)
    {
        return RPCR_OUTOFMEMORY;
    }
    pt_handler->e_type = RPC_HANDLER_TYPE_CB;
    pt_handler->u.pf_cb_hndlr = pf_cb_hndlr;
    
    if(_hash_get(ps_cb_type, NULL) == IPCR_OK)
    {
        RPC_FREE(pt_handler);
        IPC_TL_LOG(" Try to register duplicate handler, %s\n", ps_cb_type);
        return RPCR_EXIST;
    }
    return _hash_add(ps_cb_type, pt_handler);

}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Get OP handler by the OP string from table.
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpc_get_op_hndlr(
    const CHAR *       ps_op,
    rpc_op_hndlr_fct * ppf_handler)
{
    INT32 i4_ret;
    RPC_HANDLER_T * pt_handler; 
    
    i4_ret = _hash_get(ps_op, (VOID **)(VOID *)&pt_handler);
    
    if(i4_ret == RPCR_OK)
    {
        *ppf_handler = pt_handler->u.pf_op_hndlr;
        return RPCR_OK;
    }
    else
    {
        IPC_TL_LOG("Not found op handler %s\n", ps_op);
        return RPCR_NOT_FOUND;
    }
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Get CB handler by the CB string from table.
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpc_get_cb_hndlr(
    const CHAR *       ps_cb_type,
    rpc_cb_hndlr_fct * ppf_handler)
{
    INT32 i4_ret;
    RPC_HANDLER_T * pt_handler;

    i4_ret = _hash_get(ps_cb_type, (VOID **)(VOID *)&pt_handler);
    
    if(i4_ret == RPCR_OK)
    {
        *ppf_handler = pt_handler->u.pf_cb_hndlr;
        return RPCR_OK;
    }
    else
    {
        IPC_TL_LOG("Not found cb handler %s\n", ps_cb_type);
        return RPCR_NOT_FOUND;
    }
    
}

/*TODO: Unreg*/

