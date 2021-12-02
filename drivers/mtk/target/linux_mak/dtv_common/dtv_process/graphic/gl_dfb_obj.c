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
#include "inc/u_common.h"
#include "graphic/u_gl.h"
#include "graphic/gl.h"
#include "graphic/gl_dfb.h"

#include "handle/u_handle.h"
#include "stdio.h"

#if 0
#include <directfb.h>
#endif

#include "pthread.h"

HANDLE_T gl_dfb_new_handle(
        HANDLE_TYPE_T    e_type,
        VOID *           pv_obj,
        VOID *           pv_tag,
        handle_free_fct  pf_free)
{
    HANDLE_T h_obj;
    if(handle_alloc(e_type, pv_obj, pv_tag, pf_free, &h_obj) == HR_OK)
    {
        GL_DFB_TRACE("[GL_DFB_OBJ]create handle sucessful%x type:%d\n", h_obj, e_type);
        if(HR_OK != handle_link(&(((GL_DFB_OBJ_T *)pv_obj)->t_link), h_obj))
        {
		    GL_DFB_TRACE("[GL_DFB_OBJ]Link handle failed%x\n", h_obj);
        }
        return h_obj;
    }
    else
    {		
        GL_DFB_TRACE("[GL_DFB_OBJ]Create handle failed . %d\n", e_type);
        return NULL_HANDLE;
    }
}

HANDLE_T gl_dfb_duplicate_handle(
    HANDLE_T        h_obj, 
    HANDLE_TYPE_T   e_type, 
    handle_free_fct pf_free)
{
    VOID*           pv_obj;
    
    VOID*           pv_tag;
    HANDLE_T        h_dup;
    
    if (HR_OK != x_handle_get_tag(h_obj, &pv_tag))
    {
	    GL_DFB_TRACE("[GL_DFB_OBJ]Dunplicate fail handle%x\n", h_obj);
        return NULL_HANDLE;
    }
    
    if (HR_OK != handle_get_obj(h_obj, &pv_obj))
    {
        GL_DFB_TRACE("[GL_DFB_OBJ]Dunplicate fail handle%x\n", h_obj);
        return NULL_HANDLE;
    }

    if (HR_OK != handle_alloc(e_type, pv_obj, pv_tag, pf_free, &h_dup))
    {
        GL_DFB_TRACE("[GL_DFB_OBJ]Dunplicate fail handle%x\n", h_obj);
        return NULL_HANDLE;
    }

    gl_dfb_obj_lock(pv_obj);
    if(HR_OK != handle_link(&(((GL_DFB_OBJ_T *)pv_obj)->t_link), h_dup))
    {
        GL_DFB_TRACE("[GL_DFB_OBJ]Dunplicate fail handle%x\n", h_obj);
        gl_dfb_obj_unlock(pv_obj);
        return NULL_HANDLE;
    }
    
    gl_dfb_obj_unlock(pv_obj);
	
    return (h_dup);


}


BOOL gl_dfb_free_handle(HANDLE_T h_obj)
{    
    return (handle_free(h_obj, TRUE) == HR_OK);
}

VOID * gl_dfb_handle_get_obj(HANDLE_TYPE_T e_type, HANDLE_T h_obj)
{
    GL_DFB_OBJ_T * p_obj;
    if(h_obj == NULL_HANDLE)
	{
        /*NULL handle could be a NULL object*/
        return NULL;
	}
    if(handle_get_obj(h_obj, (VOID **)(&p_obj)) == HR_OK)
    {
        return p_obj;
    }
    else
    {
        GL_DFB_TRACE("[GL_DFB_OBJ]Handle get obj failed . %x\n", h_obj);
        return NULL;
    }
}


/*
 *
 * GL DFB Object super class implemetation.
 * The objects are inherited from this class.
 *
 * */

void gl_dfb_obj_init(HANDLE_TYPE_T e_type, void * p_obj)
{
    int ret;
    pthread_mutexattr_t  mtx_attr;
    static UINT32 ui4_id = 0;

    if((ret = pthread_mutexattr_init(&mtx_attr)) != 0)
    {
        GL_DFB_TRACE("[GL_DFB_OBJ]Error for pthread init mtx attr %d\n", ret);
    }
    pthread_mutexattr_settype (&mtx_attr, PTHREAD_MUTEX_RECURSIVE_NP);

    if((ret = pthread_mutex_init(&((GL_DFB_OBJ_T*)p_obj)->mtx, &mtx_attr)) != 0)
    {
        GL_DFB_TRACE("[GL_DFB_OBJ]Error for pthread mtx init %d\n", ret);
    }
    
    ((GL_DFB_OBJ_T*)p_obj)->e_type = e_type;
    handle_link_init(&(((GL_DFB_OBJ_T*)p_obj)->t_link));
    
    gl_dfb_global_lock();
    ((GL_DFB_OBJ_T*)p_obj)->ui4_id = ui4_id ++;
    gl_dfb_global_unlock();
    
    
}

void gl_dfb_obj_uninit(void * p_obj)
{
    pthread_mutex_destroy(&((GL_DFB_OBJ_T*)p_obj)->mtx);
}

void gl_dfb_obj_lock(VOID * p_obj)
{
    int ret;

    if(p_obj == NULL)
    {
        return;
    }
    if((ret = pthread_mutex_lock(&((GL_DFB_OBJ_T*)p_obj)->mtx)) != 0)
    {
        GL_DFB_TRACE("[GL_DFB_OBJ]Error for obj lock:%d\n", ret);
    }  

}

BOOL gl_dfb_obj_try_lock(VOID * p_obj)
{
    if(p_obj == NULL)
    {
        return TRUE;
    }
    return (pthread_mutex_trylock(&((GL_DFB_OBJ_T*)p_obj)->mtx) == 0);
   
}

static INT32 _compare_obj(GL_DFB_OBJ_T * p_obj1, GL_DFB_OBJ_T * p_obj2)
{
    /*NULL obj could be sorted as a lowest*/
    if(p_obj1 == NULL || p_obj2 == NULL)
    {
        if((INT32)p_obj1 < (INT32)p_obj2)
        {
            return -1;
        }
        else if((INT32)p_obj1 > (INT32)p_obj2)
        {
            return 1;
        }
        else
        {
            return 0;
        }

    }

    if(p_obj1->e_type < p_obj2->e_type)
    {
        return 1;
    }
    else if(p_obj1->e_type > p_obj2->e_type)
    {
        return -1;
    }
    /*type ==*/
    if(p_obj1->ui4_id < p_obj2->ui4_id)
    {
        return 1;
    }
    else if(p_obj1->ui4_id > p_obj2->ui4_id)
    {
        return -1;
    }
    /*??? Reach here?*/
    return 0;
}

static void _sort2(VOID * ap_obj[], VOID* ap_sorted[])
{
    if(_compare_obj(ap_obj[0], ap_obj[1]) < 0)
    {
        ap_sorted[0] = ap_obj[1];
        ap_sorted[1] = ap_obj[0];     
    }
    else
    {
        GL_MEMCPY(ap_sorted, ap_obj, sizeof(GL_DFB_OBJ_T*) * 2);
    }
}
static void _sort3(VOID * ap_obj[], VOID * ap_sorted[])
{
    if(_compare_obj(ap_obj[0], ap_obj[1]) < 0)
    {
        if(_compare_obj(ap_obj[1], ap_obj[2]) < 0)
        {
                ap_sorted[0] = ap_obj[2];
                ap_sorted[1] = ap_obj[1];
                ap_sorted[2] = ap_obj[0];   
        }
        else
        {
            if(_compare_obj(ap_obj[0], ap_obj[2]) < 0)
            {
                ap_sorted[0] = ap_obj[1];
                ap_sorted[1] = ap_obj[2];
                ap_sorted[2] = ap_obj[0];
            }
            else
            {
                ap_sorted[0] = ap_obj[1];
                ap_sorted[1] = ap_obj[0];
                ap_sorted[2] = ap_obj[2];            
            }
        }
    }
    else
    {
        if(_compare_obj(ap_obj[1], ap_obj[2]) > 0)
        {
            GL_MEMCPY(ap_sorted, ap_obj, sizeof(GL_DFB_OBJ_T * ) * 3);                
        }
        else
        {
            if(_compare_obj(ap_obj[0], ap_obj[2]) < 0)
            {
                ap_sorted[0] = ap_obj[2];
                ap_sorted[1] = ap_obj[0];
                ap_sorted[2] = ap_obj[1];
            }
            else
            {
                ap_sorted[0] = ap_obj[0];
                ap_sorted[1] = ap_obj[2];
                ap_sorted[2] = ap_obj[1];            
            }
        }    
    }
}

void gl_dfb_mul_objs_lock(VOID ** pp_obj, UINT32 ui4_count)
{
    void * apv_sorted[3];
    switch(ui4_count)
    {
        case 1:
            gl_dfb_obj_lock(pp_obj[0]);
            break;
        case 2:
            _sort2(pp_obj, apv_sorted);
            gl_dfb_obj_lock(apv_sorted[0]);
            gl_dfb_obj_lock(apv_sorted[1]);
            break;
        case 3:
            _sort3(pp_obj, apv_sorted);
            gl_dfb_obj_lock(apv_sorted[0]);
            gl_dfb_obj_lock(apv_sorted[1]);
            gl_dfb_obj_lock(apv_sorted[2]);            
            break;
        default:
            GL_DFB_TRACE("Warning: more than 3 objs mul locked-Failed\n");
            return;
    }
   
}

void gl_dfb_obj_unlock(VOID * p_obj)
{
    if(p_obj == NULL)
    {
        return;
    }
    pthread_mutex_unlock(&((GL_DFB_OBJ_T*)p_obj)->mtx);
   
}

void gl_dfb_mul_objs_unlock(VOID ** pp_obj, UINT32 ui4_count)
{
    UINT32 i;
    for(i = 0 ;i < ui4_count; i ++)
    {   
        gl_dfb_obj_unlock(pp_obj[i]);
    }    
}



/*
 * Get object in the module lock protection and make the object locked.
 * 
 * */
void * gl_dfb_handle_lock_obj(HANDLE_TYPE_T e_type, HANDLE_T h_obj, INT32 * p_err)
{
    GL_DFB_OBJ_T * p_obj;
    
    *p_err = GLR_OK;
    
    p_obj = gl_dfb_handle_get_obj(e_type, h_obj);
    gl_dfb_obj_lock(p_obj);
    
    return p_obj;
}

void gl_dfb_handle_unlock_obj(void * p_obj)
{
    gl_dfb_obj_unlock(p_obj);
}


INT32 gl_dfb_mul_handles_lock_objs(HANDLE_TYPE_T ae_type[], HANDLE_T ah_obj[], UINT32 ui_count, void * apv_obj[])
{
    UINT32 i;
    
    for(i = 0 ;i < ui_count; i ++)
    {    
        apv_obj[i] = gl_dfb_handle_get_obj(ae_type[i], ah_obj[i]);
    }    
    
    gl_dfb_mul_objs_lock(apv_obj, ui_count);

    return GLR_OK;
}

void gl_dfb_mul_handles_unlock_objs( void * pv_obj[], UINT32 ui_count)
{
    gl_dfb_mul_objs_unlock(pv_obj, ui_count);

}


