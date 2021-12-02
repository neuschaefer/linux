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
 * $RCSfile: x_cl.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 41ca7918d43fabd0a4e138ad36c77d7d $ Asa
 *
 * Description: 
 *         This file contains the interface generic APIs of GCL layer. 
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "handle/handle.h"
#include "handle/x_handle_grp.h"
#include "os/inc/x_os.h"
#include "util/x_lnk_list.h"
#include "cl/x_cl.h"
#include "cl/_cl.h"
#include "cl/u_cl.h"
#include "cl/cl_cli.h"
#include "inc/x_sys_name.h"
#include "dbg/x_dbg.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MAX_ENG_NAME_LEN 16

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static SLIST_T(_CL_ENG_REG_T) t_eng_reg_list = {NULL};
static BOOL gb_cl_init = FALSE;
static HANDLE_T       h_cl_semaphore = NULL_HANDLE;

#define DBG_ABRT_CANNOT_LOCK_CM                ((UINT32) 0x00000003)
#define DBG_ABRT_CANNOT_UNLOCK_CM              ((UINT32) 0x00000004)

#define ABORT(_cat, _val) DBG_ABORT(DBG_MOD_CL | (_cat) | (_val))


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  _cl_free_handle
 *
 * Description: callback function for free handle
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE    success. 
 *          FALSE   fail.
 ----------------------------------------------------------------------------*/
static BOOL _cl_free_handle (
    HANDLE_T        h_handle,
    HANDLE_TYPE_T   e_type,
    VOID*           pv_obj,
    VOID*           pv_tag,
    BOOL            b_req_handle)
{
    DBG_API(("[CL] free_handle: h_handle:%x \t e_type:%x \t pv_obj:%x \t pv_tag:%x \t b_req_handle:%x\n\r"
        , h_handle, e_type, pv_obj, pv_tag, b_req_handle));
    
    if (pv_obj)
    {
        x_mem_free(pv_obj);
        pv_obj = NULL;
        return TRUE;
    }

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name:  x_cl_open_eng
 *
 * Description: Open an engine for compression/decompression operation.
 *
 * Inputs:  ps_name     identifies sixteen characters engine name, and the character is case sensitive.
 *          pf_nfy      specify the notify callback function. 
 *          pv_tag      the argument prepared by the user, and it will send back to user in (*x_cl_nfy_fct).
 *
 * Outputs: ph_engine   The handle of the engine which want to be opened.
 *
 * Returns: CLR_OK                    The routine was successful and the operation has been completed.
 *          CLR_INV_ARG               The argument ps_name or ph_engine is NULL.
 *          CLR_ENG_NOT_FOUND         Cannot find the engine, which you want to open it.
 *          CLR_OUT_OF_MEMORY         There is insufficient memory available to allocate structure.
 *          CLR_OUT_OF_HANDLE         No more handles are available.
 ----------------------------------------------------------------------------*/
extern INT32 x_cl_open_eng (
             const CHAR*    ps_name,  
             x_cl_nfy_fct   pf_nfy,   
             VOID*          pv_tag,
             HANDLE_T*      ph_engine    
             )
{
    CL_ENG_REG_T* pt_eng_reg;
    BOOL b_found = FALSE;
    CL_ENG_OBJ_T* pt_eng_obj;
    HANDLE_T    h_eng;

    DBG_API(("[CL] open_eng: ps_name:%s \t pf_nfy:%x \t pv_tag:%x \t ph_engine:%x \n\r"
        , ps_name, pf_nfy, pv_tag, ph_engine));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (ps_name == NULL) ||
        (ph_engine == NULL) )
    {
        return CLR_INV_ARG;
    }
    
    /************************************************************************/
    /* Perform open engine                                                  */
    /************************************************************************/

	cl_lock();
	
    /* does the engine exist? */
    SLIST_FOR_EACH(pt_eng_reg, &t_eng_reg_list, t_link)
    {
        if (x_strncmp(pt_eng_reg->ac_eng_name, ps_name, MAX_ENG_NAME_LEN) == 0)
        {
           b_found = TRUE;
           break;
        }
    }
    
    if (b_found == FALSE)
    {
    	cl_unlock();
        return CLR_ENG_NOT_FOUND;
    }
    
    /* create a engine object*/
    /* malloc node */
    pt_eng_obj = x_mem_alloc(sizeof(CL_ENG_OBJ_T));
    if (pt_eng_obj == NULL)
    {
    	cl_unlock();
        return CLR_OUT_OF_MEMORY;
    }
    x_memset(pt_eng_obj, 0, sizeof(CL_ENG_OBJ_T));

    /* allocate handle*/
    if (HR_OK != handle_alloc(HT_CL_ENG_TYPE, pt_eng_obj, NULL, _cl_free_handle, &h_eng))
    {
        x_mem_free(pt_eng_obj);
    	cl_unlock();
        return CLR_OUT_OF_HANDLE;
    }

    /* fill data */
    pt_eng_obj->pt_eng_reg = pt_eng_reg;
    pt_eng_obj->pf_nfy = pf_nfy;
    pt_eng_obj->pv_tag = pv_tag;
#ifdef CL_SUPPORT_ARCHIVE
    SLIST_INIT(&(pt_eng_obj->t_child_archive_list)); 
    pt_eng_obj->h_linklist_op_lock = NULL_HANDLE;
    if (x_strcmp(SN_CL_ZIP, ps_name) == 0)
    {
        /* create semaphore */
        if (OSR_OK != x_sema_create( &pt_eng_obj->h_linklist_op_lock,
                                 X_SEMA_TYPE_BINARY,
                                 X_SEMA_STATE_UNLOCK ))
        {
    		cl_unlock();
            return CLR_OUT_OF_HANDLE;
        }
    }
#endif
    *ph_engine = h_eng;
    cl_unlock();
    return CLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_cl_close
 *
 * Description: Close the entry, archive or engine after no more using
 *
 * Inputs:  h_handle    the handle of a object which you already opened.
 *
 * Outputs: -
 *
 * Returns: CLR_OK                 The routine was successful and the operation has been completed.
 *          CLR_INV_ARG            The argument h_handle is a NULL_HANDLE.
 *          CLR_INV_HANDLE         The handle is not valid any more. 
 *          CLR_FCT_NOT_SUPPORT    The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 x_cl_close (
             HANDLE_T           h_handle 
             )
{
    HANDLE_TYPE_T e_type;
    VOID* pv_obj;
    CL_ENG_OBJ_T* pt_eng_obj; 
#ifdef CL_SUPPORT_ARCHIVE
    CL_ARCHIVE_OBJ_T *pt_arc_obj;
    CL_ENTRY_OBJ_T* pt_ety_obj;
    cl_eng_close_fct pf_eng_close_fct;
#endif
    x_cl_nfy_fct pf_nfy_fct;
    INT32 i4_res = CLR_INV_ARG ;
    
    DBG_API(("[CL] close: h_handle:%x \n\r"
        , h_handle));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/

	
    cl_lock();
    if (HR_OK != handle_get_type_obj(h_handle, &e_type, (VOID**)&pv_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {
        if (pv_obj == NULL)
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }
    }
    /************************************************************************/
    /* Perform open engine                                                  */
    /************************************************************************/
    
    switch (e_type)
    {
    case HT_CL_ENG_TYPE:/* close engine */
        pt_eng_obj = (CL_ENG_OBJ_T*)pv_obj;
#ifdef CL_SUPPORT_ARCHIVE
        /* close child archive */
        while((pt_arc_obj = SLIST_FIRST(&pt_eng_obj->t_child_archive_list)) != NULL)
        {	
            /* execute archive's close  */
            pf_eng_close_fct = pt_arc_obj->pt_eng_reg->t_eng_fct_tbl.pf_close;
            if (pf_eng_close_fct == NULL)
            {
    			cl_unlock();
                return CLR_FCT_NOT_SUPPORT;
            }
            i4_res = pf_eng_close_fct(pt_arc_obj->h_self);
            if (i4_res != CLR_OK)
            {
    			cl_unlock();
                return i4_res;
            }
            
            /* notify child */
            pf_nfy_fct = pt_arc_obj->pf_nfy;
            if (pf_nfy_fct)
            {
                pf_nfy_fct(pt_arc_obj->h_self, pt_arc_obj->pv_tag, CL_NFY_HANDLE_CLOSED_TYPE);
            }
            handle_free(pt_arc_obj->h_self,TRUE);
        }
#endif
        /* free engine handle */
        pf_nfy_fct = pt_eng_obj->pf_nfy;
        if (pf_nfy_fct)
        {
            pf_nfy_fct(h_handle, pt_eng_obj->pv_tag, CL_NFY_HANDLE_CLOSED_TYPE);
        }
#ifdef CL_SUPPORT_ARCHIVE
        if (pt_eng_obj->h_linklist_op_lock != NULL_HANDLE)
        {
            /* delete semaphore */
            if (x_sema_delete (pt_eng_obj->h_linklist_op_lock) != OSR_OK)
            {
    			cl_unlock();
                return CLR_INV_HANDLE;   
            }
        }
#endif
        handle_free(h_handle,TRUE);
        i4_res = CLR_OK;
        break;
#ifdef CL_SUPPORT_ARCHIVE
    case HT_CL_ARC_TYPE:/* close archive */
        pt_arc_obj = (CL_ARCHIVE_OBJ_T*)pv_obj;
        /* execute archive's close  */
        pf_eng_close_fct = pt_arc_obj->pt_eng_reg->t_eng_fct_tbl.pf_close;
        if (pf_eng_close_fct == NULL)
        {
    		cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
        i4_res = pf_eng_close_fct(h_handle);
        break;
        
    case HT_CL_ETY_TYPE:/* close entry */
        pt_ety_obj = (CL_ENTRY_OBJ_T*)pv_obj;
        /* execute entry's close  */
        pf_eng_close_fct = pt_ety_obj->pt_eng_reg->t_eng_fct_tbl.pf_close;
        if (pf_eng_close_fct == NULL)
        {
    		cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
        i4_res = pf_eng_close_fct(h_handle);
        break;
#endif
    case HT_CL_REPEATED_TYPE: /* repeated handle*/   
        handle_free(h_handle,TRUE);
        break;
    default:
		
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
	
    cl_unlock();
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name:  x_cl_get_compress_len
 *
 * Description: This routine gets the length of compressed buffer after actually performs data compression from source buffer. 
 *
 * Inputs:  h_engine          identifies the handle of the engine which performs compression operation 
 *          pv_src_buf        point to the source buffer, and data in this buffer will be compressed. 
 *          z_src_len         specify the length of the source buffer. 
 *          ui4_param_type    specify type of compression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param          specify compression paramters, set NULL is using default
 *
 * Outputs: pz_dest_len contains the compressed length after source  buffer is compressed.
 *
 * Returns: CLR_OK                The routine was successful and the operation has been completed.
 *          CLR_INV_ARG           The argument pv_src_buf or pz_dest_len contains a NULL pointer.
 *          CLR_INV_HANDLE        The handle is not valid any more.
 *          CLR_OUT_OF_MEMORY     The system runs out of memory.
 *          CLR_COMPRESS_ERROR    The routine occurred error during compression algorithm process, such as other third-party error code or the symbol not found in codebook.
 *          CLR_FCT_NOT_SUPPORT   The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 x_cl_get_compress_len (
             HANDLE_T               h_engine,  
             const VOID*            pv_src_buf,
             SIZE_T                 z_src_len, 
             UINT32                 ui4_param_type,
             VOID*                  pv_param, 
             SIZE_T*                pz_dest_len
             )
{
    CL_ENG_OBJ_T* pt_eng_obj; 
    cl_eng_get_fct pf_get_fct;
    HANDLE_TYPE_T e_type;
    INT32 i4_res;

    DBG_API(("[CL] get_compress_len: h_engine:%x \t pv_src_buf:%x \t z_src_len:%x \t pz_dest_len:%x \n\r"
        , h_engine, pv_src_buf, z_src_len, pz_dest_len));  
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_src_buf == NULL) ||
        (pz_dest_len == NULL) )
    {
        return CLR_INV_ARG;
    }
    if ((ui4_param_type != CL_NULL_PARAM_TYPE) 
        && (pv_param == NULL))
    {
        return CLR_INV_ARG;
    }

	
    cl_lock();
    /************************************************************************/
    /* Perform  decompression                                               */
    /************************************************************************/
    if (HR_OK != handle_get_type_obj(h_engine, &e_type, (VOID**)&pt_eng_obj))
    {	
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {
        if ( (e_type != HT_CL_ENG_TYPE) ||
            (pt_eng_obj == NULL) )
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_eng_obj->pt_eng_reg == NULL)
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }

        pf_get_fct = pt_eng_obj->pt_eng_reg->t_eng_fct_tbl.pf_get;
        if (pf_get_fct == NULL)
        {
    		cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    
    /* execute engine function */
    i4_res = pf_get_fct(CL_GET_COMPRESS_LEN_TYPE, (VOID*)pv_src_buf, z_src_len, ui4_param_type, pv_param, 0, 0, pz_dest_len);
	
    cl_unlock();
	return i4_res;
}
/*-----------------------------------------------------------------------------
 * Name:  x_cl_compress
 *
 * Description: This routine compresses data from source buffer to destination buffer.
 *
 * Inputs:  h_engine          identifies the handle of the engine, which performs compression operation.
 *          pv_src_buf        point to the source buffer, which contains source data waiting for compression.
 *          z_src_len         specify the length of the source buffer.
 *          ui4_param_type    specify type of compression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param          specify compression paramters, set NULL is using default
 *          pz_dest_len       specify the length of the destination buffer.
 *
 * Outputs: pv_dest_buf     point to the destination buffer.
 *          pz_dest_len     specify the really compression length of destination buffer.
 *
 * Returns: CLR_OK                    The routine was successful and the operation has been completed.
 *          CLR_INV_ARG               The argument pv_src_buf, pv_dest_buf or pz_dest_len contains a NULL pointer.
 *          CLR_INV_HANDLE            The handle is not valid any more.
 *          CLR_OUT_OF_MEMORY         The system runs out of memory.
 *          CLR_COMPRESS_ERROR        The routine occurred error during compression algorithm process, such as other third-party error code or the symbol not found in codebook.
 *          CLR_FCT_NOT_SUPPORT       The requested function was not supported. 
 *          CLR_INSUFFICIENT_BUFFER   The destination buffer is insufficient for data compression of source buffer.
 ----------------------------------------------------------------------------*/
extern INT32 x_cl_compress (
             HANDLE_T               h_engine,   
             const VOID*            pv_src_buf, 
             SIZE_T                 z_src_len,
             UINT32                 ui4_param_type,
             VOID*                  pv_param,
             VOID*                  pv_dest_buf,
             SIZE_T*                pz_dest_len 
             )
{
    CL_ENG_OBJ_T* pt_eng_obj; 
    cl_eng_compress_fct pf_compress_fct;
    HANDLE_TYPE_T e_type;
    INT32 i4_res;
    
    DBG_API(("[CL] compress: h_engine:%x \t pv_src_buf:%x \t z_src_len:%x \t pv_dest_buf:%x \t pz_dest_len:%x \n\r"
        , h_engine, pv_src_buf, z_src_len, pv_dest_buf, pz_dest_len));  
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_src_buf == NULL) ||
        (pv_dest_buf == NULL) ||
        (pz_dest_len == NULL) )
    {
        return CLR_INV_ARG;
    }
    if ((ui4_param_type != CL_NULL_PARAM_TYPE) 
        && (pv_param == NULL))
    {
        return CLR_INV_ARG;
    }

	cl_lock();
    /************************************************************************/
    /* Perform  compression                                                 */
    /************************************************************************/
    if (HR_OK != handle_get_type_obj(h_engine, &e_type, (VOID**)&pt_eng_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {
        if ( (e_type != HT_CL_ENG_TYPE) ||
            (pt_eng_obj == NULL) )
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_eng_obj->pt_eng_reg == NULL)
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }

        pf_compress_fct = pt_eng_obj->pt_eng_reg->t_eng_fct_tbl.pf_compress;
        if (pf_compress_fct == NULL)
        {
    		cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    
    /* execute engine function */
    i4_res = pf_compress_fct(CL_COMPRESS_TYPE, (VOID*)pv_src_buf, z_src_len, ui4_param_type, pv_param, pv_dest_buf, pz_dest_len);
	cl_unlock();
	return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name:  x_cl_get_decompress_len
 *
 * Description: Get the length of the decompressed buffer after really decompress data from source buffer.
 *
 * Inputs:  h_engine            identifies the handle of the engine which performs compression operation 
 *          pv_src_buf          point to the source buffer, which contains source data waiting for decompression. 
 *          z_src_len           specify the length of the source buffer.
 *          ui4_param_type      specify type of decompression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify decompression paramters, set NULL is using default
 *
 * Outputs: pz_dest_len     contains the compressed length after source  buffer is compressed.
 *
 * Returns: CLR_OK                  The routine was successful and the operation has been completed.
 *          CLR_INV_ARG             The argument pv_src_buf or pz_dest_len contains a NULL pointer.
 *          CLR_INV_HANDLE          The handle is not valid any more.
 *          CLR_OUT_OF_MEMORY       The system runs out of memory.
 *          CLR_FCT_NOT_SUPPORT     The requested function was not supported.
 *          CLR_DECOMPRESS_ERROR    The routine occurred error during decompression algorithm process, such as the bit stream is damage.
 ----------------------------------------------------------------------------*/
extern INT32 x_cl_get_decompress_len (
             HANDLE_T                 h_engine,   
             const VOID*              pv_src_buf, 
             SIZE_T                   z_src_len,  
             UINT32                   ui4_param_type,
             VOID*                    pv_param, 
             SIZE_T*                  pz_dest_len 
             )
{
    CL_ENG_OBJ_T* pt_eng_obj; 
    cl_eng_get_fct pf_get_fct;
    HANDLE_TYPE_T e_type;
    INT32 i4_res;

    DBG_API(("[CL] get_decompress_len: h_engine:%x \t pv_src_buf:%x \t z_src_len:%x \t pz_dest_len:%x \n\r"
        , h_engine, pv_src_buf, z_src_len, pz_dest_len));     
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_src_buf == NULL) ||
        (pz_dest_len == NULL) )
    {
        return CLR_INV_ARG;
    }
    if ((ui4_param_type != CL_NULL_PARAM_TYPE) 
        && (pv_param == NULL))
    {
        return CLR_INV_ARG;
    }

	
    cl_lock();
		
    /************************************************************************/
    /* Perform  decompression                                               */
    /************************************************************************/
    if (HR_OK != handle_get_type_obj(h_engine, &e_type, (VOID**)&pt_eng_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {
        if ( (e_type != HT_CL_ENG_TYPE) ||
             (pt_eng_obj == NULL) )
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_eng_obj->pt_eng_reg == NULL)
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }

        pf_get_fct = pt_eng_obj->pt_eng_reg->t_eng_fct_tbl.pf_get;
        if (pf_get_fct == NULL)
        {
    		cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    
    /* execute engine function */
    i4_res = pf_get_fct(CL_GET_DECOMPRESS_LEN_TYPE, (VOID*)pv_src_buf, z_src_len, ui4_param_type, pv_param, 0, 0, pz_dest_len);
    
	cl_unlock();
	return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name:  x_cl_decompress
 *
 * Description: Decompress data from source buffer to destination buffer.
 *
 * Inputs:  h_engine            identifies the handle of the engine which performs compression operation 
 *          pv_src_buf          point to the source buffer. 
 *          z_src_len           specify the length of the source buffer. 
 *          z_skip_len          indicate the number of bytes to skip from the beginning of decompression output.
 *          ui4_param_type      specify type of decompression paramters, set CL_NULL_PARAM_TYPE is using default
 *          pv_param            specify decompression paramters, set NULL is using default
 *          pz_dest_len         specify the length of destination buffer.
 *          ph_prev_state       set NULL if one shot;specify previous decompress state
 *
 * Outputs: pv_dest_buf     point to the destination buffer. 
 *          pz_dest_len     specify the really decompression length of destination buffer.
 *          ph_prev_state   next time decompress state
 *
 * Returns: CLR_OK                   The routine was successful and the operation has been completed.
 *          CLR_INV_ARG              The argument pv_src_buf, pv_dest_buf or pz_dest_len contains a NULL pointer.
 *          CLR_INV_HANDLE           The handle is not valid any more.
 *          CLR_OUT_OF_MEMORY        The system runs out of memory.
 *          CLR_FCT_NOT_SUPPORT      The requested function was not supported.
 *          CLR_DECOMPRESS_ERROR     The routine occurred error during decompression algorithm process, such as the bit stream is damage.
 ----------------------------------------------------------------------------*/

extern INT32 x_cl_decompress (
             HANDLE_T                 h_engine, 
             const VOID*              pv_src_buf, 
             SIZE_T                   z_src_len,  
             SIZE_T                   z_skip_len,
             UINT32                   ui4_param_type, 
             VOID*                    pv_param, 
             VOID*                    pv_dest_buf, 
             SIZE_T*                  pz_dest_len,
             HANDLE_T*                ph_prev_state
             )
{
    CL_ENG_OBJ_T* pt_eng_obj; 
    cl_eng_decompress_fct pf_decompress_fct;
    HANDLE_TYPE_T e_type;
    INT32 i4_res;

    DBG_API(("[CL] decompress: h_engine:%x \t pv_src_buf:%x \t z_src_len:%x \t z_skip_len:%x \t pv_dest_buf:%x \t pz_dest_len:%x \n\r"
        , h_engine, pv_src_buf, z_src_len, z_skip_len, pv_dest_buf, pz_dest_len)); 
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_src_buf == NULL) ||
        (pv_dest_buf == NULL) ||
        (pz_dest_len == NULL) )
    {
        return CLR_INV_ARG;
    }
    if ((ui4_param_type != CL_NULL_PARAM_TYPE) 
        && (pv_param == NULL))
    {
        return CLR_INV_ARG;
    }

	cl_lock();
    /************************************************************************/
    /* Perform  decompression                                               */
    /************************************************************************/
    if (HR_OK != handle_get_type_obj(h_engine, &e_type, (VOID**)&pt_eng_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {
        if ( (e_type != HT_CL_ENG_TYPE) ||
            (pt_eng_obj == NULL) )
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_eng_obj->pt_eng_reg == NULL)
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }

        pf_decompress_fct = pt_eng_obj->pt_eng_reg->t_eng_fct_tbl.pf_decompress;
        if (pf_decompress_fct == NULL)
        {
    		cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    
    /* execute engine function */
    i4_res = pf_decompress_fct(CL_DECOMPRESS_TYPE, (VOID*)pv_src_buf, z_src_len, ui4_param_type, pv_param, pv_dest_buf, z_skip_len, pz_dest_len, ph_prev_state);
    
	cl_unlock();
	return i4_res;
}

extern INT32 x_cl_decompress_ex(
             HANDLE_T                 h_engine, 
             const VOID*              pv_src_buf, 
             SIZE_T                   z_src_len,  
             SIZE_T                   z_skip_len,
             UINT32                   ui4_param_type, 
             VOID*                    pv_param, 
             VOID*                    pv_dest_buf, 
             SIZE_T*                  pz_dest_len,
             HANDLE_T*                ph_prev_state
             )
{
    CL_ENG_OBJ_T* pt_eng_obj; 
    cl_eng_decompress_fct pf_decompress_fct;
    HANDLE_TYPE_T e_type;
    INT32 i4_res;

    DBG_API(("[CL] decompress: h_engine:%x \t pv_src_buf:%x \t z_src_len:%x \t z_skip_len:%x \t pv_dest_buf:%x \t pz_dest_len:%x \n\r"
        , h_engine, pv_src_buf, z_src_len, z_skip_len, pv_dest_buf, pz_dest_len)); 
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_src_buf == NULL) ||
        (pv_dest_buf == NULL) ||
        (pz_dest_len == NULL) )
    {
        return CLR_INV_ARG;
    }
    if ((ui4_param_type != CL_NULL_PARAM_TYPE) 
        && (pv_param == NULL))
    {
        return CLR_INV_ARG;
    }

	
    cl_lock();
    /************************************************************************/
    /* Perform  decompression                                               */
    /************************************************************************/
    if (HR_OK != handle_get_type_obj(h_engine, &e_type, (VOID**)&pt_eng_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {
        if ( (e_type != HT_CL_ENG_TYPE) ||
            (pt_eng_obj == NULL) )
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_eng_obj->pt_eng_reg == NULL)
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }

        pf_decompress_fct = pt_eng_obj->pt_eng_reg->t_eng_fct_tbl.pf_decompress;
        if (pf_decompress_fct == NULL)
        {
    		cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    
    /* execute engine function */
    i4_res = pf_decompress_fct(CL_DECOMPRESS_TYPE_EX, (VOID*)pv_src_buf, z_src_len, ui4_param_type, pv_param, pv_dest_buf, z_skip_len, pz_dest_len, ph_prev_state);

	cl_unlock();
	return i4_res;
}


/*-----------------------------------------------------------------------------
 * Name:  x_cl_open_archive
 *
 * Description: This routine creates and opens an archive for later entry operation.
 *
 * Inputs:  h_engine    specified the handle of an engine.
 *          ps_name     specified the path name string of the archive. 
 *          ui1_mode    specified the access permit mode, and only support CL_READ_MODE.
 *          pf_nfy      specify the notify callback function. When ph_archive is not valid any more, the callback function pf_nfy will be called. 
 *          pv_tag      the argument prepared by the user, and it will send back to user in (*x_cl_nfy_fct). 
 *
 * Outputs: ph_archive  retrieve the archive handle.
 *
 * Returns: CLR_OK                     The routine was successful and the operation has been completed.
 *          CLR_INV_ARG                The argument ps_path or ppv_harchive contains a NULL pointer or ui1_mode is outside of definition.
 *          CLR_INV_HANDLE             The handle is invalid.
 *          CLR_OUT_OF_MEMORY          The system runs out of memory.
 *          CLR_BAD_ARCHIVE            The archive is corrupted.
 *          CLR_OUT_OF_HANDLE          No more handles are available.
 *          CLR_FCT_NOT_SUPPORT        The requested function was not supported.
 *          CLR_FILE_OR_DIR_MISSING    The input arguments of the file or directory name are invalid.
 ----------------------------------------------------------------------------*/
extern INT32 x_cl_open_archive (
             HANDLE_T       h_engine,  
             const CHAR*    ps_name,   
             UINT8          ui1_mode,  
             x_cl_nfy_fct   pf_nfy,    
             VOID*          pv_tag,
             HANDLE_T*      ph_archive     
             )
{
#ifdef CL_SUPPORT_ARCHIVE
    CL_ENG_OBJ_T* pt_eng_obj; 
    cl_eng_open_fct pf_eng_open_fct;
    HANDLE_TYPE_T e_type;
    INT32 i4_res;

    DBG_API(("[CL] open_archive: h_engine:%x \t ps_name:%s \t ui1_mode:%x \t pf_nfy:%x \t pv_tag:%x \t ph_archive:%x \n\r"
        , h_engine, ps_name, ui1_mode, pf_nfy, pv_tag, ph_archive)); 
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/

    if ( (ps_name == NULL) ||
        ( ph_archive == NULL) )
    {
        return CLR_INV_ARG;
    }

    if (ui1_mode&0xE)
    {
        return CLR_FCT_NOT_SUPPORT;
    }
    /************************************************************************/
    /* Perform open an archive                                              */
    /************************************************************************/
    
    cl_lock();
	
    if (HR_OK != handle_get_type_obj(h_engine, &e_type, (VOID**)&pt_eng_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {
        if ( (e_type != HT_CL_ENG_TYPE) ||
            (pt_eng_obj == NULL) )
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_eng_obj->pt_eng_reg == NULL)
        {
    		cl_unlock();
            return CLR_INV_HANDLE;
        }

        pf_eng_open_fct = pt_eng_obj->pt_eng_reg->t_eng_fct_tbl.pf_open;
        if (pf_eng_open_fct == NULL)
        {
    		cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    
    /* execute engine's open archive */
    i4_res = pf_eng_open_fct(CL_OPEN_ARCHIVE_TYPE, h_engine, (VOID*)ps_name, ui1_mode, ph_archive);

	cl_unlock();
    return i4_res;
#else
    return CLR_FCT_NOT_SUPPORT;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  x_cl_open_entry_by_name
 *
 * Description: This routine creates or opens an entry, which has the path name string ps_name in the archive.
 *
 * Inputs:  h_archive   the handle of the archive. 
 *          ps_name     specified the path name string of the entry which will be opened. 
 *          pf_nfy      specify the notify callback function. When ph_entry is not valid any more, the callback function pf_nfy will be called. 
 *          pv_tag      the argument prepared by the user, and it will send back to user in (*x_cl_nfy_fct). 
 *
 * Outputs: ph_entry    retrieve the handle of the entry.
 *
 * Returns: CLR_OK                       The routine was successful and the operation has been completed.
 *          CLR_INV_ARG                  The argument pv_harchive, ps_name and ppv_hentry contain a NULL pointer.
 *          CLR_OUT_OF_MEMORY            The system runs out of memory.
 *          CLR_INV_HANDLE               The handle is not valid any more.
 *          CLR_BAD_ARCHIVE              The archive is corrupted.
 *          CLR_FILE_OR_DIR_MISSING      The input arguments of the file or directory name are invalid.
 *          CLR_OUT_OF_HANDLE            No more handles are available.
 *          CLR_FCT_NOT_SUPPORT          The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 x_cl_open_entry_by_name (
             HANDLE_T       h_archive, 
             const CHAR*    ps_name,   
             x_cl_nfy_fct   pf_nfy,    
             VOID*          pv_tag,
             HANDLE_T*      ph_entry      
             )
{
#ifdef CL_SUPPORT_ARCHIVE
    CL_ARCHIVE_OBJ_T* pt_arc_obj;
    cl_eng_open_fct pf_eng_open_fct;
    HANDLE_TYPE_T e_type;
    INT32 i4_res;

    DBG_API(("[CL] open_entry_by_name: h_archive:%x \t ps_name:%s \t pf_nfy:%x \t pv_tag:%x \t ph_entry:%x \n\r"
        , h_archive, ps_name, pf_nfy, pv_tag, ph_entry)); 
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (ps_name == NULL) ||
        ( ph_entry == NULL) )
    {
        return CLR_INV_ARG;
    }    
    /************************************************************************/
    /* Perform open entry                                                   */
    /************************************************************************/
    cl_lock();
    if (HR_OK != handle_get_type_obj(h_archive, &e_type, (VOID**)&pt_arc_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {
        if ( (e_type != HT_CL_ARC_TYPE) ||
            (pt_arc_obj == NULL))
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_arc_obj->pt_eng_reg == NULL)
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        pf_eng_open_fct = pt_arc_obj->pt_eng_reg->t_eng_fct_tbl.pf_open;
        if (pf_eng_open_fct == NULL)
        {
        	cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    
    /* execute engine's open entry */
    i4_res = pf_eng_open_fct(CL_OPEN_ENTRY_BY_NAME_TYPE, h_archive, (VOID*)ps_name, 0, ph_entry);
	cl_unlock();
    return i4_res;
#else
    return CLR_FCT_NOT_SUPPORT;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  x_cl_open_entry_by_index
 *
 * Description: This routine opens an entry in the archive by index of entry numbers.
 *
 * Inputs:  h_archive       the handle of the archive. 
 *          ui4_entry_index specified the index of the entry which will be opened, and it is 0 base.
 *          pf_nfy          specify the notify callback function. When ph_entry is not valid any more, the callback function pf_nfy will be called.
 *          pv_tag          the argument prepared by the user, and it will send back to user in (*x_cl_nfy_fct).
 *
 * Outputs: ph_entry    retrieve the handle of the entry. 
 *
 * Returns: CLR_OK                The routine was successful and the operation has been completed.
 *          CLR_INV_ARG           The argument pv_harchive and ppv_hentry contain a NULL pointer or ui4_entry_index is out of entry number range.
 *          CLR_OUT_OF_MEMORY     The system runs out of memory.
 *          CLR_BAD_ARCHIVE       The archive is corrupted.
 *          CLR_INV_HANDLE        The handle is not valid any more.
 *          CLR_OUT_OF_HANDLE     No more handles are available.
 *          CLR_FCT_NOT_SUPPORT   The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 x_cl_open_entry_by_index (
             HANDLE_T       h_archive,       
             UINT32         ui4_entry_index, 
             x_cl_nfy_fct   pf_nfy,          
             VOID*          pv_tag,
             HANDLE_T*      ph_entry           
             )
{
#ifdef CL_SUPPORT_ARCHIVE
    CL_ARCHIVE_OBJ_T* pt_arc_obj;
    cl_eng_open_fct pf_eng_open_fct;
    HANDLE_TYPE_T e_type;
    INT32 i4_res;

    DBG_API(("[CL] open_entry_by_index: h_archive:%x \t ui4_entry_index:%x \t pf_nfy:%x \t pv_tag:%x \t ph_entry:%x \n\r"
        , h_archive, ui4_entry_index, pf_nfy, pv_tag, ph_entry)); 
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (ph_entry == NULL)
    {
        return CLR_INV_ARG;
    }    
    /************************************************************************/
    /* Perform open entry                                                   */
    /************************************************************************/
	cl_lock();
	
    if (HR_OK != handle_get_type_obj(h_archive, &e_type, (VOID**)&pt_arc_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {    
        if ( (e_type != HT_CL_ARC_TYPE) ||
            (pt_arc_obj == NULL) )
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_arc_obj->pt_eng_reg == NULL)
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }
        
        pf_eng_open_fct = pt_arc_obj->pt_eng_reg->t_eng_fct_tbl.pf_open;
        if (pf_eng_open_fct == NULL)
        {
        	cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    /* execute engine's open entry */
    i4_res = pf_eng_open_fct(CL_OPEN_ENTRY_BY_INDEX_TYPE, h_archive, (VOID*)&ui4_entry_index, 0, ph_entry);
    cl_unlock();
	return i4_res;
#else
    return CLR_FCT_NOT_SUPPORT;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  x_cl_decompress_entry
 *
 * Description: This Routine will read and decompress data from the current entry file to the destination buffer, after open an entry in the archive.
 *
 * Inputs:  h_entry         the handle of the entry.
 *          z_skip_len      indicate the number of Bytes which shall be skipped before data shall be actually written to the destination buffer.
 *          pz_dest_len     specified the length of the destination buffer.
 *
 * Outputs:  pv_dest      the destination buffer that contains reading data.
 *           pz_dest_len  specify the really decompression length of destination buffer.
 *
 * Returns: CLR_OK                    The routine was successful and the operation has been completed.
 *          CLR_INV_ARG               The argument pv_dest or pz_dest_len contain a NULL pointer.
 *          CLR_INV_HANDLE            The handle is not valid any more.
 *          CLR_DECOMPRESS_ERROR      The routine occurred error during decompression algorithm process, such as the bit stream is damage.
 *          CLR_INSUFFICIENT_BUFFER   The destination buffer is insufficient for data decompression.
 *          CLR_OUT_OF_MEMORY         The system runs out of memory.
 *          CLR_FCT_NOT_SUPPORT       The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 x_cl_decompress_entry (
             HANDLE_T       h_entry,        
             SIZE_T         z_skip_len, 
             VOID*          pv_dest,
             SIZE_T*        pz_dest_len
             )
{
#ifdef CL_SUPPORT_ARCHIVE
    CL_ENTRY_OBJ_T* pt_entry_obj; 
    cl_eng_decompress_fct pf_decompress_fct;
    HANDLE_TYPE_T e_type;
    INT32 i4_res;

    DBG_API(("[CL] decompress_entry: h_entry:%x \t z_skip_len:%x \t pv_dest:%x \t pz_dest_len:%x \n\r"
        , h_entry, z_skip_len, pv_dest, pz_dest_len)); 
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pv_dest == NULL) ||
        (pz_dest_len == NULL) )
    {
        return CLR_INV_ARG;
    }

	cl_lock();
    /************************************************************************/
    /* Perform decompress entry                                             */
    /************************************************************************/
    if (HR_OK != handle_get_type_obj(h_entry, &e_type, (VOID**)&pt_entry_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {
        if ( (e_type != HT_CL_ETY_TYPE) ||
            (pt_entry_obj == NULL) )
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_entry_obj->pt_eng_reg == NULL)
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        pf_decompress_fct = pt_entry_obj->pt_eng_reg->t_eng_fct_tbl.pf_decompress;
        if (pf_decompress_fct == NULL)
        {
        	cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    /* execute engine's open entry */
    i4_res = pf_decompress_fct(CL_DECOMPRESS_ENTRY_TYPE, (VOID*)(&h_entry), 0, CL_NULL_PARAM_TYPE, NULL, pv_dest, z_skip_len, pz_dest_len, NULL);
	cl_unlock();
	return i4_res;
#else
    return CLR_FCT_NOT_SUPPORT;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  x_cl_get_decompress_entry_len
 *
 * Description: This Routine will get the length of the current entry file except z_skip_len.
 *
 * Inputs:  h_entry       the handle of the entry.
 *
 * Outputs: pz_dest_len    specify the actual length, which is needed to decompress the entry data.
 *
 * Returns: CLR_OK                The routine was successful and the operation has been completed.
 *          CLR_INV_ARG           The argument pz_dest_len contain a NULL pointer.
 *          CLR_INV_HANDLE        The handle is not valid any more.
 *          CLR_FCT_NOT_SUPPORT   The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 x_cl_get_decompress_entry_len (
             HANDLE_T       h_entry,    
             SIZE_T*        pz_dest_len 
             )
{
#ifdef CL_SUPPORT_ARCHIVE
    CL_ENTRY_OBJ_T* pt_entry_obj; 
    cl_eng_get_fct pf_get_fct;
    HANDLE_TYPE_T e_type;
    INT32 i4_res;

    DBG_API(("[CL] get_decompress_entry_len: h_entry:%x \t pz_dest_len:%x \n\r"
        , h_entry, pz_dest_len)); 
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pz_dest_len == NULL)
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform get decompress entry length                                  */
    /************************************************************************/

	cl_lock();
    if (HR_OK != handle_get_type_obj(h_entry, &e_type, (VOID**)&pt_entry_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {    
        if ( (e_type != HT_CL_ETY_TYPE) ||
            (pt_entry_obj == NULL) )
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_entry_obj->pt_eng_reg == NULL)
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        pf_get_fct = pt_entry_obj->pt_eng_reg->t_eng_fct_tbl.pf_get;
        if (pf_get_fct == NULL)
        {
        	cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    
    /* execute engine function */
    i4_res = pf_get_fct(CL_GET_DECOMPRESS_ENTRY_LEN_TYPE,(VOID*)(&h_entry), 0, CL_NULL_PARAM_TYPE, NULL, 0, 0, pz_dest_len);
	cl_unlock();
	return i4_res;
#else
    return CLR_FCT_NOT_SUPPORT;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  x_cl_query_info
 *
 * Description: This API will query information of the archive.
 *
 * Inputs:  h_archive        the handle of the archive.
 *          e_type           specified the type of the query operation.
 *          pv_filter        specified the querying filter to query the archive.
 *          pz_query_buf_len specified the length of the query buffer.
 *
 * Outputs: pv_query_buf        retrieve the query information in the buffer.
 *          pz_query_buf_len    specified the actual size of query information needed.
 *
 * Returns: CLR_OK                    The routine was successful and the operation has been completed.
 *          CLR_INV_ARG               The argument pv_query_buf, pv_filter or pz_query_buf_len contain a NULL pointer or e_type is out of range.
 *          CLR_INV_HANDLE            The handle is not valid any more
 *          CLR_INSUFFICIENT_BUFFER   The destination buffer is insufficient for data querying.
 *          CLR_FCT_NOT_SUPPORT       The requested function was not supported.
 ----------------------------------------------------------------------------*/
extern INT32 x_cl_query_info (
             HANDLE_T               h_archive,       
             CL_QUERY_INFO_TYPE_T   e_type,          
             VOID*                  pv_filter,       
             VOID*                  pv_query_buf,    
             SIZE_T*                pz_query_buf_len 
             )
{
#ifdef CL_SUPPORT_ARCHIVE
    CL_ARCHIVE_OBJ_T* pt_archive_obj; 
    cl_eng_get_fct pf_get_fct;
    HANDLE_TYPE_T e_handle_type;
    INT32 i4_res;

    DBG_API(("[CL] query_info: h_archive:%x \t e_type:%x \t pv_filter:%x \t pv_query_buf:%x \t pz_query_buf_len:%x\n\r"
        , h_archive, e_type, pv_filter, pv_query_buf, pz_query_buf_len)); 
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (pz_query_buf_len == NULL) ||
        (pv_query_buf == NULL) )
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform get query info length                                        */
    /************************************************************************/
	cl_lock();
    if (HR_OK != handle_get_type_obj(h_archive, &e_handle_type, (VOID**)&pt_archive_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {
        if ( (e_handle_type != HT_CL_ARC_TYPE)||
            (pt_archive_obj == NULL) )
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_archive_obj->pt_eng_reg == NULL)
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        pf_get_fct = pt_archive_obj->pt_eng_reg->t_eng_fct_tbl.pf_get;
        if (pf_get_fct == NULL)
        {
        	cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    
    /* execute engine function */
    i4_res = pf_get_fct(CL_QUERY_INFO_TYPE, (VOID*)(&h_archive), e_type, CL_NULL_PARAM_TYPE, NULL, pv_filter, pv_query_buf, pz_query_buf_len);
	cl_unlock();
	return i4_res;
#else
    return CLR_FCT_NOT_SUPPORT;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  x_cl_get_query_info_len
 *
 * Description: This routine will get the length of query information of the archive
 *
 * Inputs:  h_archive    the handle of the archive.
 *          e_type       specified the type of the query operation.
 *          pv_filter    specified the querying filter to query the archive.
 *
 * Outputs: pz_query_buf_len    specified the actual size of query information needed.
 *
 * Returns: CLR_OK                The routine was successful and the operation has been completed.
 *          CLR_INV_ARG           The argument pv_filter or pz_query_buf_len contain a NULL pointer or e_type is out of range.
 *          CLR_INV_HANDLE        The handle is not valid any more.
 *          CLR_FCT_NOT_SUPPORT   The requested function was not supported.
 ----------------------------------------------------------------------------*/

extern INT32 x_cl_get_query_info_len (
             HANDLE_T               h_archive,       
             CL_QUERY_INFO_TYPE_T   e_type,          
             VOID*                  pv_filter,       
             SIZE_T*                pz_query_buf_len 
             )
{
#ifdef CL_SUPPORT_ARCHIVE
    CL_ARCHIVE_OBJ_T* pt_archive_obj; 
    cl_eng_get_fct pf_get_fct;
    HANDLE_TYPE_T e_handle_type;
    INT32 i4_res;

    DBG_API(("[CL] get_query_info_len: h_archive:%x \t e_type:%x \t pv_filter:%x \t pz_query_buf_len:%x\n\r"
        , h_archive, e_type, pv_filter, pz_query_buf_len));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( pz_query_buf_len == NULL ) 
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform get query info length                                        */
    /************************************************************************/
	cl_lock();
    if (HR_OK != handle_get_type_obj(h_archive, &e_handle_type, (VOID**)&pt_archive_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {    
        if ( (e_handle_type != HT_CL_ARC_TYPE)||
            (pt_archive_obj == NULL) )
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_archive_obj->pt_eng_reg == NULL)
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        pf_get_fct = pt_archive_obj->pt_eng_reg->t_eng_fct_tbl.pf_get;
        if (pf_get_fct == NULL)
        {
        	cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    
    /* execute engine function */
    i4_res = pf_get_fct(CL_GET_QUERY_INFO_LEN_TYPE,  &h_archive, e_type, CL_NULL_PARAM_TYPE, NULL, pv_filter, 0, pz_query_buf_len);
	cl_unlock();
	return i4_res;
#else
    return CLR_FCT_NOT_SUPPORT;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  x_cl_get_num_entries
 *
 * Description: This API will query information of the archive.
 *
 * Inputs:  h_archive          the handle of the archive.
 *
 * Outputs: pui4_num_entris    retrieve the number of entries
 *
 * Returns: CLR_OK                The routine was successful and the operation has been completed.
 *            CLR_INV_ARG         The argument pui4_num_entries contains a NULL pointer.            
 *            CLR_INV_HANDLE      The handle h_archive is not valid any more.            
 *            CLR_FCT_NOT_SUPPORT The requested function was not supported.
 ----------------------------------------------------------------------------*/

extern INT32 x_cl_get_num_entries (
             HANDLE_T   h_archive,       
             UINT32*    pui4_num_entris 
             )
{
#ifdef CL_SUPPORT_ARCHIVE
    CL_ARCHIVE_OBJ_T* pt_archive_obj; 
    cl_eng_get_fct pf_get_fct;
    HANDLE_TYPE_T e_type;
    INT32 i4_res;

    DBG_API(("[CL] get_num_entries: h_archive:%x \t pui4_num_entris:%x \n\r"
        , h_archive, pui4_num_entris));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pui4_num_entris == NULL)
    {
        return CLR_INV_ARG;
    }
    /************************************************************************/
    /* Perform get number of entry                                          */
    /************************************************************************/
	cl_lock();
    if (HR_OK != handle_get_type_obj(h_archive, &e_type, (VOID**)&pt_archive_obj))
    {
    	cl_unlock();
        return CLR_INV_HANDLE;
    }
    else
    {    
        if ( (e_type != HT_CL_ARC_TYPE) || 
            (pt_archive_obj == NULL) )
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        if (pt_archive_obj->pt_eng_reg == NULL)
        {
        	cl_unlock();
            return CLR_INV_HANDLE;
        }

        pf_get_fct = pt_archive_obj->pt_eng_reg->t_eng_fct_tbl.pf_get;
        if (pf_get_fct == NULL)
        {
        	cl_unlock();
            return CLR_FCT_NOT_SUPPORT;
        }
    }
    
    /* execute engine function */
    i4_res = pf_get_fct(CL_GET_NUM_ENTRIES_TYPE, &h_archive, 0, CL_NULL_PARAM_TYPE, NULL, 0, 0, (SIZE_T*)pui4_num_entris);
	cl_unlock();
	return i4_res;
#else
    return CLR_FCT_NOT_SUPPORT;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: _handle_autofree
 *
 * Description: <function description>
 *
 * Inputs:  h_handle References.
 *          e_type   References.
 *
 * Outputs: - 
 *
 * Returns: HR_INV_HANDLE_TYPE  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _handle_autofree (
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type)
{
    
    if (e_type < HT_GROUP_CL || e_type >= HT_CL_END_TYPE)
    {
        return HR_INV_HANDLE_TYPE;
    }
    
    return (x_cl_close(h_handle) == CLR_OK ? HR_OK : HR_INV_HANDLE);
}

/*-----------------------------------------------------------------------------
 * Name:  cl_register_eng
 *
 * Description: This API registers an engine to generic compression library layer.
 *
 * Inputs:  ps_name         it specifies sixteen characters of a engine name, and the character is case sensitive. 
 *          pt_fct_tbl      the function table of the engine. 
 *
 * Outputs: -
 *
 * Returns: CLR_OK                The engine registration is successful. 
 *          CLR_OUT_OF_MEMORY     There is insufficient memory available to add a link list of CL_ENG_FCT_T.
 *          CLR_NAME_EXIST        The name of the engine already registered in GCL layer.
 *          CLR_INV_ARG           The argument ps_name or pt_fct_tbl contains a NULL pointer.
 *          CLR_CLI_ERROR         initial CLI function for compression library is failed.
 *          CLR_AUTOFREE_ERROR    Add auto free function to AEE is failed.
 ----------------------------------------------------------------------------*/
extern INT32 cl_register_eng (
        const CHAR*                ps_name,        
        const CL_ENG_FCT_TBL_T*    pt_fct_tbl  
        )
{
    CL_ENG_REG_T* pt_eng_reg;
    INT32 i4_i;
    static handle_autofree_fct apf_autofree_fcts[HT_CL_END_TYPE - HT_GROUP_CL];
    
    DBG_API(("[CL] register_eng: ps_name:%s \t pt_fct_tbl:%x \n\r"
        , ps_name, pt_fct_tbl));   
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (ps_name == NULL) ||
        (pt_fct_tbl == NULL) )
    {
        return CLR_INV_ARG;
    }

    /************************************************************************/
    /* Perform register engine                                              */
    /************************************************************************/

    /* check if the engine is exist?*/
    SLIST_FOR_EACH(pt_eng_reg, &t_eng_reg_list, t_link)
    {
        if (x_strncmp(pt_eng_reg->ac_eng_name, ps_name, MAX_ENG_NAME_LEN) == 0)
        {
            return CLR_NAME_EXIST;
        }
    }
    
    /* malloc node */
    pt_eng_reg = x_mem_alloc(sizeof(CL_ENG_REG_T));
    if (pt_eng_reg == NULL)
    {
        return CLR_OUT_OF_MEMORY;
    }

    x_memset(pt_eng_reg, 0, sizeof(CL_ENG_REG_T));

    /* fill data */
    x_strncpy(pt_eng_reg->ac_eng_name, ps_name, MAX_ENG_NAME_LEN);
    pt_eng_reg->t_eng_fct_tbl.pf_close = pt_fct_tbl->pf_close;
    pt_eng_reg->t_eng_fct_tbl.pf_compress = pt_fct_tbl->pf_compress;
    pt_eng_reg->t_eng_fct_tbl.pf_decompress = pt_fct_tbl->pf_decompress;
    pt_eng_reg->t_eng_fct_tbl.pf_get = pt_fct_tbl->pf_get;
    pt_eng_reg->t_eng_fct_tbl.pf_open = pt_fct_tbl->pf_open;

    /* add new node */
    SLIST_INSERT_HEAD(pt_eng_reg, &t_eng_reg_list, t_link);

    if (gb_cl_init == FALSE)
    {

#ifdef CLI_LVL_ALL
        /* Initialize compression library CLI component */
        if (cl_cli_init() != CLR_OK)
        {
            return CLR_CLI_ERROR;
        } 
#endif

        for (i4_i = 0; i4_i < HT_CL_END_TYPE - HT_GROUP_CL; i4_i++)
        {
            apf_autofree_fcts[i4_i] = _handle_autofree;
        }
            
        if (handle_set_autofree_tbl(HT_GROUP_CL, apf_autofree_fcts) != HR_OK)
        {
            return CLR_AUTOFREE_ERROR;
        }

		x_sema_create(&h_cl_semaphore,X_SEMA_TYPE_BINARY,X_SEMA_STATE_UNLOCK);
		if (h_cl_semaphore == NULL_HANDLE)
		{
			return CLR_AUTOFREE_ERROR;
		}
		
        gb_cl_init = TRUE;
        
    }

    return CLR_OK;

}

extern VOID cl_lock(VOID)
{
	INT32 i4_res;
	   
   i4_res = x_sema_lock(h_cl_semaphore,
						X_SEMA_OPTION_WAIT);

   if (i4_res != OSR_OK)
   {
	   ABORT(DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_CM);
   }

}

extern VOID cl_unlock(VOID)
{
	INT32 i4_res;
	
	i4_res = x_sema_unlock(h_cl_semaphore);
	
	if (i4_res != OSR_OK)
	{
		ABORT(DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_CM);
	}
}

