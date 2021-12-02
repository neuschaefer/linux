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
 * $RCSfile: handle_if.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all handle related API's which need to dispatch to
 *         Linux kernel to deal with.
 *---------------------------------------------------------------------------*/
 /*-----------------------------------------------------------------------------
                    include files
  ----------------------------------------------------------------------------*/
 
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/x_handle_grp.h"
#include "handle/handle_if.h"
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include "include/ioctl_inc/ioctl_param.h"

#ifdef RPC_ENABLE
#include "rpc/ri_common.h"
#endif


#ifdef x_dbg_stmt
#undef x_dbg_stmt
#endif
#ifdef HANDLE_DEBUG
#define x_dbg_stmt printf
#else
#define x_dbg_stmt
#endif


static INT32 i4_fd;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
#if (HANDLE_LIB_ID == HANDLE_LIB_ID_KERNEL)

#else

INT32 handle_if_init (VOID)
{
    if (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("\nDTV_SVC vvvvvv __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
        
        i4_fd = open("/dev/rmmgr", O_RDWR);
        if (i4_fd == -1)
        {
            return HR_NOT_ALLOWED;
        }

        return HR_OK;
    }
    else if (HANDLE_LIB_ID > HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("\nDTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
        
        /* do nothing here */
        

        return HR_OK;
    }
}

#ifdef RPC_ENABLE
static INT32 _handle_if_get_info_rpc(HANDLE_T        h_handle,
                                     HANDLE_TYPE_T*  pe_type,
                                     VOID**          ppv_tag)
{
    RPC_DECL(3, INT32);
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_handle);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pe_type);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ppv_tag);
    RPC_DO_OP("x_handle_get_info");
    RPC_RETURN(ARG_TYPE_INT32, HR_INV_ARG);
}

static INT32 _handle_if_get_tag_rpc(HANDLE_T  h_handle,  VOID**  ppv_tag)
{
    RPC_DECL(2, INT32);
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_handle);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ppv_tag);
    RPC_DO_OP("x_handle_get_tag");
    RPC_RETURN(ARG_TYPE_INT32, HR_INV_ARG);
}

static INT32 _handle_if_get_type_rpc(HANDLE_T  h_handle, HANDLE_TYPE_T*  pe_type)
{
    RPC_DECL(2, INT32);
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_handle);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pe_type);
    RPC_DO_OP("x_handle_get_type");
    RPC_RETURN(ARG_TYPE_INT32, HR_INV_ARG);
}

static INT32 _handle_if_set_tag_rpc(HANDLE_T  h_handle, VOID * pv_tag)
{
    RPC_DECL(2, INT32);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_handle);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_tag);
    RPC_DO_OP("x_handle_set_tag");
    RPC_RETURN(ARG_TYPE_INT32, HR_INV_ARG);
}

static BOOL _handle_if_valid_rpc(HANDLE_T  h_handle)
{
    RPC_DECL(1, BOOL);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_handle);
    RPC_DO_OP("x_handle_valid");
    RPC_RETURN(ARG_TYPE_BOOL, FALSE);
}

static VOID _handle_if_set_aux_next_wrapper (VOID*     pv_obj,
                                         HANDLE_T  h_handle,
                                         HANDLE_T  h_next)
{
    RPC_DECL_VOID(3);
    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_obj);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_handle);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_next);      
    
    RPC_DO_CB(0, "handle_if_set_aux_next", (VOID*)((UINT32)1)/* not NONE */);
    
    RPC_RETURN_VOID;
}

static VOID _handle_if_set_aux_next_rpc (VOID*     pv_obj,
                                         HANDLE_T  h_handle,
                                         HANDLE_T  h_next)
{
    RPC_DECL_VOID(3);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_obj);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_handle);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_next);
    RPC_DO_OP("handle_set_aux_next");
    RPC_RETURN_VOID;
}

static VOID _handle_if_set_aux_prev_wrapper (VOID*     pv_obj,
                                         HANDLE_T  h_handle,
                                         HANDLE_T  h_next)
{
    RPC_DECL_VOID(3);
    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_obj);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_handle);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_next);      
    
    RPC_DO_CB(0, "handle_if_set_aux_prev", (VOID*)((UINT32)1)/* not NONE */);
    
    RPC_RETURN_VOID;
}

static VOID _handle_if_set_aux_prev_rpc (VOID*     pv_obj,
                                         HANDLE_T  h_handle,
                                         HANDLE_T  h_prev)
{
    RPC_DECL_VOID(3);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_obj);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_handle);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_prev);
    RPC_DO_OP("handle_set_aux_prev");
    RPC_RETURN_VOID;
}

static HANDLE_T _handle_if_get_aux_next_wrapper (VOID*     pv_obj,
                                             HANDLE_T  h_handle)
{
    RPC_DECL(2, UINT32);
    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_obj);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_handle);    
    
    RPC_DO_CB(0, "handle_if_get_aux_next", (VOID*)((UINT32)1)/* not NONE */);
    
    RPC_RETURN(ARG_TYPE_UINT32, NULL_HANDLE);
}

static HANDLE_T _handle_if_get_aux_next_rpc (VOID*     pv_obj,
                                             HANDLE_T  h_handle)
{
    RPC_DECL(2, UINT32);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_obj);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_handle);
    RPC_DO_OP("handle_get_aux_next");
    RPC_RETURN(ARG_TYPE_UINT32, NULL_HANDLE);
}

static HANDLE_T _handle_if_get_aux_link_head_wrapper (HANDLE_T  h_handle)
{
    RPC_DECL(1, UINT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_handle);    
    
    RPC_DO_CB(0, "handle_if_get_aux_link_head", (VOID*)((UINT32)1)/* not NONE */);
    
    RPC_RETURN(ARG_TYPE_UINT32, NULL_HANDLE);
}

static HANDLE_T _handle_if_get_aux_link_head_rpc (HANDLE_T  h_handle)
{
    RPC_DECL(1, UINT32);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_handle);
    RPC_DO_OP("handle_get_aux_link_head");
    RPC_RETURN(ARG_TYPE_UINT32, NULL_HANDLE);
}

static HANDLE_T _handle_if_get_aux_prev_wrapper (VOID*     pv_obj,
                                             HANDLE_T  h_handle)
{
    RPC_DECL(2, UINT32);
    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_obj);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_handle);    
    
    RPC_DO_CB(0, "handle_if_get_aux_prev", (VOID*)((UINT32)1)/* not NONE */);
    
    RPC_RETURN(ARG_TYPE_UINT32, NULL_HANDLE);
}

static HANDLE_T _handle_if_get_aux_prev_rpc (VOID*     pv_obj,
                                             HANDLE_T  h_handle)
{
    RPC_DECL(2, UINT32);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_obj);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_handle);
    RPC_DO_OP("handle_get_aux_prev");
    RPC_RETURN(ARG_TYPE_UINT32, NULL_HANDLE);
}

static VOID _handle_if_autofree_rpc (HANDLE_T  h_handle)
{
    RPC_DECL_VOID(1);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_handle);
    RPC_DO_OP("handle_autofree");
    RPC_RETURN_VOID;
}

#endif

INT32 handle_if_get_info (HANDLE_T        h_handle,
                          HANDLE_TYPE_T*  pe_type,
                          VOID**          ppv_tag)
{
    if (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)
    {
        handle_kernel_filter t_filter;

        /*x_dbg_stmt("DTV_SVC vvvvvv __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
        t_filter.h_handle = h_handle;
        if(ioctl(i4_fd, IOCTL_HANDLE_GET_INFO, &t_filter) < 0)
        {
            return HR_INV_HANDLE;
        }   

        *pe_type = t_filter.e_type;
        *ppv_tag = (VOID*)t_filter.pv_tag;

        return HR_OK;
    }
    else if (HANDLE_LIB_ID > HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
#ifdef RPC_ENABLE
        return _handle_if_get_info_rpc(h_handle, pe_type, ppv_tag);
#else
        return HR_INV_HANDLE;
#endif
    }
}

INT32 handle_if_get_tag (HANDLE_T  h_handle,
                         VOID**    ppv_tag)
{
    if (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)
    {
        handle_kernel_filter t_filter;

        /*x_dbg_stmt("DTV_SVC vvvvvv __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
        t_filter.h_handle = h_handle;
        if(ioctl(i4_fd, IOCTL_HANDLE_GET_TAG, &t_filter) < 0)
        {
            return HR_INV_HANDLE;
        }

        *ppv_tag = (VOID*)t_filter.pv_tag;

        return HR_OK;
    }
    else if (HANDLE_LIB_ID > HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
#ifdef RPC_ENABLE
        return _handle_if_get_tag_rpc(h_handle, ppv_tag);
#else
        return HR_INV_HANDLE;
#endif
    }
}

INT32 handle_if_get_type (HANDLE_T        h_handle,
                          HANDLE_TYPE_T*  pe_type)
{
    if (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)
    {
        handle_kernel_filter t_filter;

        /*x_dbg_stmt("DTV_SVC vvvvvv __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
        t_filter.h_handle = h_handle;
        if(ioctl(i4_fd, IOCTL_HANDLE_GET_TYPE, &t_filter) < 0)
        {
            return HR_INV_HANDLE;
        }   

        *pe_type = t_filter.e_type;

        return HR_OK;
    }
    else if (HANDLE_LIB_ID > HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/

#ifdef RPC_ENABLE
        return _handle_if_get_type_rpc(h_handle, pe_type);
#else
        return HR_INV_HANDLE;
#endif
    }
}

INT32 handle_if_set_tag (HANDLE_T  h_handle,
                         VOID*     pv_tag)
{
    if (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)
    {
        handle_kernel_filter t_filter;

        /*x_dbg_stmt("DTV_SVC vvvvvv __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
        t_filter.h_handle = h_handle;
        t_filter.pv_tag = (UINT32)pv_tag;
        if(ioctl(i4_fd, IOCTL_HANDLE_SET_TAG, &t_filter) < 0)
        {
            return HR_INV_HANDLE;
        }

        return HR_OK;
    }
    else if (HANDLE_LIB_ID > HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
#ifdef RPC_ENABLE
        return _handle_if_set_tag_rpc(h_handle, pv_tag);
#else
        return HR_INV_HANDLE;
#endif
    }
}

BOOL handle_if_valid (HANDLE_T  h_handle)
{
    if (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)
    {
        handle_kernel_filter t_filter;
        
        /*x_dbg_stmt("DTV_SVC vvvvvv __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
        t_filter.h_handle = h_handle;
        if(ioctl(i4_fd, IOCTL_HANDLE_VALID, &t_filter) < 0)
        {
            return FALSE;
        }
        
        return t_filter.b_valid;
    }
    else if (HANDLE_LIB_ID > HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
        
        if(h_handle == NULL_HANDLE)
        {
            return FALSE;
        }
        
#ifdef RPC_ENABLE
        return _handle_if_valid_rpc(h_handle);
#else
        return FALSE;
#endif
    }
}

VOID handle_if_set_aux_next (VOID*     pv_obj,
                             HANDLE_T  h_handle,
                             HANDLE_T  h_next)
{
    if (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)
    {
        /* TODO : callback to DTV-APP */
        /*x_dbg_stmt("DTV_SVC to app __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
        
#ifdef RPC_ENABLE
        _handle_if_set_aux_next_wrapper(pv_obj, h_handle, h_next);
#endif
    }
    else if (HANDLE_LIB_ID > HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
#ifdef RPC_ENABLE
        _handle_if_set_aux_next_rpc(pv_obj, h_handle, h_next);
#endif
    }
}

VOID handle_if_set_aux_prev (VOID*     pv_obj,
                             HANDLE_T  h_handle,
                             HANDLE_T  h_prev)
{
    if (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)
    {
        /* TODO : callback to DTV-APP */
        /*x_dbg_stmt("DTV_SVC to app __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
        
#ifdef RPC_ENABLE
        _handle_if_set_aux_prev_wrapper(pv_obj, h_handle, h_prev);
#endif
    }
    else if (HANDLE_LIB_ID > HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
#ifdef RPC_ENABLE
        _handle_if_set_aux_prev_rpc(pv_obj, h_handle, h_prev);
#endif
    }
}

HANDLE_T handle_if_get_aux_next (VOID*     pv_obj,
                                 HANDLE_T  h_handle)
{
    if (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)
    {
        /* TODO : callback to DTV-APP */
        /*x_dbg_stmt("DTV_SVC to app __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
        
#ifdef RPC_ENABLE
        return _handle_if_get_aux_next_wrapper(pv_obj, h_handle);
#endif
    }
    else if (HANDLE_LIB_ID > HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
#ifdef RPC_ENABLE
        return _handle_if_get_aux_next_rpc(pv_obj, h_handle);
#endif
    }
    
    return NULL_HANDLE;
}

HANDLE_T handle_if_get_aux_prev (VOID*     pv_obj,
                                 HANDLE_T  h_handle)
{
    if (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)
    {
        /* TODO : callback to DTV-APP */
        /*x_dbg_stmt("DTV_SVC to app __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
        
#ifdef RPC_ENABLE
        return _handle_if_get_aux_prev_wrapper(pv_obj, h_handle);
#endif
    }
    else if (HANDLE_LIB_ID > HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
#ifdef RPC_ENABLE
        return _handle_if_get_aux_prev_rpc(pv_obj, h_handle);
#endif
    }
    
    return NULL_HANDLE;
}

HANDLE_T handle_if_get_aux_link_head (HANDLE_T  h_handle)
{
    if (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)
    {
        /* TODO : callback to DTV-APP */
        /*x_dbg_stmt("DTV_SVC to app __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
        
#ifdef RPC_ENABLE
        return _handle_if_get_aux_link_head_wrapper(h_handle);
#endif
    }
    else if (HANDLE_LIB_ID > HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
#ifdef RPC_ENABLE
        return _handle_if_get_aux_link_head_rpc(h_handle);
#endif
    }
    
    return NULL_HANDLE;
}

VOID handle_if_autofree (HANDLE_T  h_handle)
{
    if (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)
    {
        /* TODO: how to call kernel? */
        /*x_dbg_stmt("DTV_SVC todo vvvvvv __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
    }
    else if (HANDLE_LIB_ID > HANDLE_LIB_ID_DTV_SVC)
    {
        /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
#ifdef RPC_ENABLE
        _handle_if_autofree_rpc(h_handle);
#endif
    }
}
    
#endif


#if (HANDLE_LIB_ID == HANDLE_LIB_ID_KERNEL) || (HANDLE_LIB_ID == HANDLE_LIB_ID_DTV_SVC)

#else

static VOID _handle_if_set_aux_link_head_rpc (HANDLE_T  h_handle,
                                              HANDLE_T  h_aux_link_head)
{
    RPC_DECL_VOID(2);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_handle);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_aux_link_head);
    RPC_DO_OP("handle_set_aux_link_head");
    RPC_RETURN_VOID;
}

VOID handle_if_set_aux_link_head (HANDLE_T  h_handle,
                                  HANDLE_T  h_aux_link_head)
{
    /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
    _handle_if_set_aux_link_head_rpc(h_handle, h_aux_link_head);
}

static INT32 _handle_if_alloc_aux_link_head_rpc (VOID*                pv_obj,
                                                 handle_aux_free_fct  pf_aux_free,
                                                 HANDLE_T*            ph_aux_head)
{
    RPC_DECL(3, INT32);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_obj);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_aux_free);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_aux_head);
    RPC_DO_OP("handle_alloc_aux_link_head");
    RPC_RETURN(ARG_TYPE_INT32, HR_INV_ARG);
}

INT32 handle_if_alloc_aux_link_head (VOID*                pv_obj,
                                     handle_aux_free_fct  pf_aux_free,
                                     HANDLE_T*            ph_aux_head)
{
    /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
    return _handle_if_alloc_aux_link_head_rpc(pv_obj, pf_aux_free, ph_aux_head);
}

static VOID _handle_if_inc_aux_link_cnt_rpc (HANDLE_T  h_aux_link_head)
{
    RPC_DECL_VOID(1);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_aux_link_head);
    RPC_DO_OP("handle_inc_aux_link_cnt");
    RPC_RETURN_VOID;
}

VOID handle_if_inc_aux_link_cnt (HANDLE_T  h_aux_link_head)
{
    /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
    _handle_if_inc_aux_link_cnt_rpc(h_aux_link_head);
}

static VOID _handle_if_dec_aux_link_cnt_rpc (HANDLE_T  h_aux_link_head)
{
    RPC_DECL_VOID(1);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_aux_link_head);
    RPC_DO_OP("handle_dec_aux_link_cnt");
    RPC_RETURN_VOID;
}

VOID handle_if_dec_aux_link_cnt (HANDLE_T  h_aux_link_head)
{
    /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
    _handle_if_dec_aux_link_cnt_rpc(h_aux_link_head);
}

static UINT16 _handle_if_get_aux_link_cnt_rpc (HANDLE_T  h_aux_link_head)
{
    RPC_DECL(1, UINT16);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_aux_link_head);
    RPC_DO_OP("handle_get_aux_link_cnt");
    RPC_RETURN(ARG_TYPE_UINT16, HR_INV_ARG);
}

UINT16 handle_if_get_aux_link_cnt (HANDLE_T  h_aux_link_head)
{
    /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
    return _handle_if_get_aux_link_cnt_rpc(h_aux_link_head);
}

static VOID _handle_if_aux_link_head_free_rpc (HANDLE_T  h_aux_link_head)
{
    RPC_DECL_VOID(1);
    RPC_ARG_INP(ARG_TYPE_UINT32, h_aux_link_head);
    RPC_DO_OP("handle_aux_link_head_free");
    RPC_RETURN_VOID;
}

VOID handle_if_aux_link_head_free (HANDLE_T  h_aux_link_head)
{
    /*x_dbg_stmt("DTV_APP ^^^^^^ __line:%d, __func:%s\n", __LINE__, __FUNCTION__);*/
    
    _handle_if_aux_link_head_free_rpc(h_aux_link_head);
}

#endif

