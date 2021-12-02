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
#include "stdio.h"

#include "inc/u_common.h"
#include "os/inc/x_os.h"
#include "rrctx/rrctx_rrt_eng/u_rrctx_rrt_eng.h"
#include "evctx/u_evctx.h"
#include "handle/u_handle.h"
#include "handle/x_handle.h"
#include "handle/handle.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

#define RET_ON_FAIL(stmt) \
do {\
    INT32 __i4_ret;\
    __i4_ret = (stmt);\
    if(__i4_ret != RPCR_OK)\
    {   RPC_ERROR("RPC Failure %s, #%d\n", __FUNCTION__, __LINE__);\
        return __i4_ret;\
    }\
}while(0)

extern INT32 handle_set_free_fct (HANDLE_T          h_handle,
                           VOID*             pv_pvt,                           
                           handle_free_fct   pf_new_free,
                           VOID **           ppv_old_pvt,
                           handle_free_fct*  ppf_old_free);

static INT32 _thread_create(
    os_main_fct  pf_main,
    SIZE_T       z_stack_size,
    UINT8        ui1_priority,
    SIZE_T       z_arg_size,
    VOID*        pv_arg)
{
    HANDLE_T        h_th;
    CHAR            ps_name[17];
    static INT32    i4_seria = 0;
    
    snprintf(ps_name, 17, "RPC_%d", (int)(i4_seria ++));
    
    if(x_thread_create(
                &h_th,     
                ps_name,
                z_stack_size,
                ui1_priority,
                pf_main,   
                z_arg_size,
                pv_arg) != OSR_OK)
    {
        return RPCR_OSERR;
    }

    return RPCR_OK;

}

static BOOL _wrapper_free_cb (HANDLE_T       h_handle,
                         HANDLE_TYPE_T  e_type,
                         VOID*          pv_obj,
                         VOID*          pv_tag,
                         BOOL           b_req_handle)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    BOOL b_return;
 
    pt_nfy_tag = (RPC_CB_NFY_TAG_T *) pv_obj;
    
    if (pt_nfy_tag->pf_old_free (h_handle, 
                                 e_type, 
                                 pt_nfy_tag->pv_old_pvt, 
                                 pv_tag, 
                                 b_req_handle))
    {        
        ri_free_cb_tag (pt_nfy_tag); 
        b_return = TRUE;
    }
    else
    {
        ri_free_cb_tag (pt_nfy_tag); 
        b_return = FALSE;
    }
 
    return (b_return);
}

RPC_CB_NFY_TAG_T * ri_create_cb_tag(
    RPC_ID_T t_rpc_id, 
    VOID *   apv_cb_addr_ex[],    
    SIZE_T   z_num,
    VOID *   pv_tag)
{
    RPC_CB_NFY_TAG_T *   pt_nfy_tag;
    
    pt_nfy_tag = (RPC_CB_NFY_TAG_T *)malloc(sizeof(RPC_CB_NFY_TAG_T));
    if(pt_nfy_tag == NULL)
    {
        return NULL;
    }
    memset(pt_nfy_tag, 0, sizeof(RPC_CB_NFY_TAG_T));
    pt_nfy_tag->t_id        = t_rpc_id;
    pt_nfy_tag->pv_tag      = pv_tag;
    
    if(z_num == 1)
    {
        pt_nfy_tag->pv_cb_addr  = apv_cb_addr_ex[0];                          
    }
    else
    {
        SIZE_T z_i;
        for(z_i = 0; z_i < z_num; z_i ++)
        {
            pt_nfy_tag->apv_cb_addr_ex[z_i] = apv_cb_addr_ex[z_i];                          
        }
    }

    pt_nfy_tag->pf_cb_tag_free = NULL;
    return pt_nfy_tag;
}

RPC_CB_NFY_TAG_T * ri_create_cb_tag_with_free(
    RPC_ID_T            t_rpc_id, 
    VOID *              apv_cb_addr_ex[],    
    SIZE_T              z_num,
    VOID *              pv_tag,
    cb_tag_free_fct     pf_cb_tag_free)
{
    RPC_CB_NFY_TAG_T *   pt_nfy_tag;
    
    pt_nfy_tag = ri_create_cb_tag(t_rpc_id, apv_cb_addr_ex, z_num, pv_tag);
    pt_nfy_tag->pf_cb_tag_free = pf_cb_tag_free;

    return pt_nfy_tag;
}


VOID ri_free_cb_tag(RPC_CB_NFY_TAG_T *   pt_nfy_tag)
{
    /*RPC_LOG(" Free tag 0x%x (id 0x%x)\n",
            pt_nfy_tag,(unsigned int) pt_nfy_tag->t_id);*/
            
    if (pt_nfy_tag->pf_cb_tag_free)
    {
        pt_nfy_tag->pf_cb_tag_free(pt_nfy_tag);
    }
    free(pt_nfy_tag);
}

VOID ri_bind_cb_tag(HANDLE_T h_obj, RPC_CB_NFY_TAG_T * pt_nfy_tag)
{   
    if(pt_nfy_tag != NULL)
    {
        handle_set_free_fct(h_obj,                                          
                        pt_nfy_tag,                                     
                        _wrapper_free_cb,                                    
                        &pt_nfy_tag->pv_old_pvt,                        
                        &pt_nfy_tag->pf_old_free);     
    }
}

INT32 c_rpc_init()
{
    OS_FNCT_T t_of = {_thread_create};
    rpc_init(&t_of);
    return RPCR_OK;
}


RPC_ID_T c_rpc_start_client()
{
    return rpc_open_client("DTVService");
}


INT32 ri_rrctx_rating_list_add_desc(void * pt_rating_lst)
{    
    EVCTX_RATING_LIST_T * pt_list;
    
    for(pt_list = pt_rating_lst; pt_list != NULL; pt_list = pt_list->pt_next)
    {
        RET_ON_FAIL(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_list->ps_rating_descrption));   	    
		 
        if(pt_list->ui2_rated_dimensions == 0 || pt_list->pt_ratings == NULL)
        {
            /*Error here?*/
        }
        else
        {
            RET_ON_FAIL(RPC_ADD_REF_BUFF(
                RPC_DEFAULT_ID, 
                pt_list->pt_ratings, 
                sizeof(EVCTX_RATING_T) * pt_list->ui2_rated_dimensions));
        }

        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_list, RPC_DESC_EVCTX_RATING_LIST_T, NULL));    
    }
    return RPCR_OK;
}



