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
 * $RCSfile: x_midxbuld.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media index builder interface APIs  
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif

#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/mm_util_memory.h"
#include "mutil/mfmtrecg/x_mfmtrecg.h"
#include "mutil/midxbuld/x_midxbuld.h"
#include "mutil/midxbuld/midxbuld.h"
#include "mutil/midxbuld/midxbuld_cli.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "x_mm_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../mfmtrecg/x_mfmtrecg.h"
#include "x_midxbuld.h"
#include "midxbuld.h"
#include "midxbuld_cli.h"
#include "x_dbg.h"
#include "u_dbg.h"
#include "../mm_util.h"
#include "../mm_util_memory.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
 
static BOOL _midxbuld_free_handle (
                        HANDLE_T                 h_handle,
                        HANDLE_TYPE_T            e_type,
                        VOID*                    pv_obj,
                        VOID*                    pv_tag,
                        BOOL                     b_req_handle)
{
    MIDXBULD_OBJ_T*     pt_obj = NULL;

    if (pv_obj)
    {
         if ((e_type == HT_MIDXBULD_FILE_TYPE)
                || (e_type == HT_MIDXBULD_MEM_TYPE))
        {
            pt_obj = (MIDXBULD_OBJ_T*)pv_obj;
            if (e_type == HT_MIDXBULD_FILE_TYPE)
            {
                if (pt_obj->h_file)
                {
#ifndef __NO_FM__                   
                    x_fm_close(pt_obj->h_file);
#endif
                    pt_obj->h_file = NULL_HANDLE;
                }
                if (pt_obj->pv_buff_or_file_path_name)
                {
                    _mm_util_mem_free_with_tag(pt_obj->pv_buff_or_file_path_name, MM_MEM_MIDX_TAG);
                    pt_obj->pv_buff_or_file_path_name = NULL;
                }
            }
            _mm_util_mem_free_with_tag(pv_obj, MM_MEM_MIDX_TAG);
            pv_obj = NULL;
            return TRUE;
        }
    }
    return FALSE;
} 
/*-----------------------------------------------------------------------------
 * Name:  x_midxbuld_fopen
 *
 * Description: This API opens a media file to do media indexing.
 *
 * Inputs:  ps_path_name    specify the path string of file which will be opened in minfo.
 *          pf_nfy          specify the function point of notification function.
 *          pv_tag          specify the tag which will be send back to AP when minfo notification.
 *          pt_format       if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file.
 *
 * Outputs: pt_format       If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP.
 *          ph_midxbuld        The parameter will retrieve a media index builder handle.
 *
 * Returns: MIDXBULDR    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/                       
INT32 x_midxbuld_fopen (
                        CHAR*                    ps_path_name,
                        x_midxbuld_nfy_fct       pf_nfy,      
                        VOID*                    pv_tag,     
                        MEDIA_FORMAT_T*          pt_format,
                        MIDXBULD_SOURCE_INFO_T*  pt_src_info,
                        MIDXBULD_KEY_INFO_T*     pt_key_info,
                        HANDLE_T*                ph_midxbuld)
{ 
    INT32                   i4_ret = MIDXBULDR_INV_ARG;
    MEDIA_FORMAT_T          t_media_format;
    MIDXBULD_OBJ_T*         pt_obj;
    SIZE_T                  z_file_path_name_length;
    handler_file_open_fct   pf_file_open;
    
    DBG_API(("[midxbuld] x_midxbuld_fopen: ps_path_name:%s \t pf_nfy:%x \t pv_tag:%x \t pt_format:%x \t ph_midxbuld:%x\n\r"
        , ps_path_name, pf_nfy, pv_tag, pt_format, ph_midxbuld));  
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((ps_path_name == NULL)
       || (pt_format == NULL)
       || (ph_midxbuld == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* make sure file format */
    if ((pt_format->e_media_type == MEDIA_TYPE_UNKNOWN)
        || (pt_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_UNKNOWN))
    {
        pt_format->b_fav_recg_only = FALSE;
        i4_ret = x_mfmtrecg_get_file_fmt(ps_path_name, &t_media_format);
        if ((t_media_format.e_media_type == MEDIA_TYPE_UNKNOWN)
        || (t_media_format.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_UNKNOWN)
        || (i4_ret != MFMTRECGR_OK ))        
        {
            return MIDXBULDR_FMT_NOT_SUPPORT;
        }
    }
    else
    {
        t_media_format = *pt_format;
    }

    /* allocate elm_obj memory */
    pt_obj = _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_obj == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_obj, 0, sizeof(MIDXBULD_OBJ_T));

    z_file_path_name_length = x_strlen(ps_path_name) + 1;/* NULL terminator*/
    pt_obj->pv_buff_or_file_path_name = _mm_util_mem_alloc_with_tag(z_file_path_name_length, MM_MEM_MIDX_TAG);
    if ( pt_obj->pv_buff_or_file_path_name == NULL)
    {
        _mm_util_mem_free_with_tag(pt_obj, MM_MEM_MIDX_TAG);
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_obj->pv_buff_or_file_path_name, 0, z_file_path_name_length);
    if (HR_OK != handle_alloc(HT_MIDXBULD_FILE_TYPE, pt_obj, NULL, _midxbuld_free_handle, ph_midxbuld))
    {
        _mm_util_mem_free_with_tag(pt_obj, MM_MEM_MIDX_TAG);
        return MIDXBULDR_OUT_OF_HANDLE;
    }   

    /* fill data */
    x_strcpy(pt_obj->pv_buff_or_file_path_name, ps_path_name);
    pt_obj->t_media_format = t_media_format;
    pt_obj->t_src_info = *pt_src_info;
    pt_obj->h_midxbuld = *ph_midxbuld;
    pt_obj->pf_nfy = pf_nfy;
    pt_obj->pv_tag = pv_tag;

    if (pt_obj->h_sync_lock == NULL_HANDLE)
    {
        /* create semaphore */   
        i4_ret = x_sema_create( &pt_obj->h_sync_lock,
                             X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_LOCK );
                             
        if ( i4_ret != OSR_OK || pt_obj->h_sync_lock == NULL_HANDLE )
        {
            return MIDXBULDR_INTERNAL_ERR;
        }        
    }
    
    /* select handler */
    i4_ret = midxbuld_match_handler(&t_media_format, &pt_obj->t_handler_fct_tbl);
    if (i4_ret != MIDXBULDR_OK)
    {
        return MIDXBULDR_FMT_NOT_SUPPORT;
    }
    
    pf_file_open = pt_obj->t_handler_fct_tbl.pf_fopen;
    if (pf_file_open)
    {
        if (pf_file_open(ps_path_name, pt_key_info, (VOID*)pt_obj) != MIDXBULDR_OK )
        {
            return MIDXBULDR_FCT_NOT_SUPPORT;
        }

        /* start key frame table index building */
        return (x_midxbuld_start_indexing(*ph_midxbuld));
    }
    else
    {
        return MIDXBULDR_FCT_NOT_SUPPORT;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  x_midxbuld_mopen
 *
 * Description: This API opens a media memory buffer to do media indexing.
 *
 * Inputs:  pv_mem_data     specify the memory address of media data buffer.
 *          z_mem_leng      specify the memory length of media data buffer.
 *          pf_nfy          specify the function point of notification function.
 *          pv_tag          specify the tag which will be send back to AP when minfo notification.
 *          pt_format       if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file.
 *
 * Outputs: pt_format       If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP.
 *          ph_midxbuld     The parameter will retrieve a media index builder handle.
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/                       
INT32 x_midxbuld_mopen (
                        VOID*                    pv_mem_data,   
                        SIZE_T                   z_mem_leng,   
                        x_midxbuld_nfy_fct       pf_nfy,
                        VOID*                    pv_tag,  
                        MEDIA_FORMAT_T*          pt_format,
                        MIDXBULD_SOURCE_INFO_T*  pt_src_info,
                        MIDXBULD_KEY_INFO_T*     pt_key_info,
                        HANDLE_T*                ph_midxbuld)
{
    INT32                   i4_ret = MIDXBULDR_INV_ARG;
    MEDIA_FORMAT_T          t_media_format;
    MIDXBULD_OBJ_T*         pt_obj;
    handler_mem_open_fct    pf_mem_open;
        
    DBG_API(("[MIDXBULD] x_midxbuld_mopen: pv_mem_data:%x \t z_mem_leng:%x \t pf_nfy:%x \t pv_tag:%x \t pt_format:%x \t ph_midxbuld:%x\n\r"
        , pv_mem_data, z_mem_leng, pf_nfy, pv_tag, pt_format, ph_midxbuld));  
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL)
       || (ph_midxbuld == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* make sure file format */
    if ((pt_format->e_media_type == MEDIA_TYPE_UNKNOWN)
        || (pt_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_UNKNOWN))
    {
        i4_ret = x_mfmtrecg_get_mem_fmt(pv_mem_data, z_mem_leng, &t_media_format);
        if ((t_media_format.e_media_type == MEDIA_TYPE_UNKNOWN)
        || (t_media_format.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_UNKNOWN)
        || (i4_ret != MFMTRECGR_OK ))        
        {
            return MIDXBULDR_FMT_NOT_SUPPORT;
        }
    }
    else
    {
        t_media_format = *pt_format;
    }

    /* allocate obj memory */
    pt_obj = _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_obj == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_obj, 0, sizeof(MIDXBULD_OBJ_T));
    
    if (HR_OK != handle_alloc(HT_MIDXBULD_MEM_TYPE, pt_obj, NULL, _midxbuld_free_handle, ph_midxbuld))
    {
        _mm_util_mem_free_with_tag(pt_obj, MM_MEM_MIDX_TAG);
        return MIDXBULDR_OUT_OF_HANDLE;
    }   
        
    /* fill data */
    if (z_mem_leng == 0)
    {
        MEDIA_FEEDER_T* pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;

        pt_obj->h_src_feeder = pt_src_feeder->h_feeder;
    }
    else
    {
        pt_obj->h_src_feeder = NULL_HANDLE;
    }
    pt_obj->t_media_format = t_media_format;
    pt_obj->t_src_info = *pt_src_info;
    pt_obj->h_midxbuld = *ph_midxbuld;
    pt_obj->pv_buff_or_file_path_name = pv_mem_data;
    pt_obj->z_buff_leng = z_mem_leng;
    pt_obj->pf_nfy = pf_nfy;
    pt_obj->pv_tag = pv_tag;
    
    if (pt_obj->h_sync_lock == NULL_HANDLE)
    {
        /* create semaphore */   
        i4_ret = x_sema_create( &pt_obj->h_sync_lock,
                             X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_LOCK );
                             
        if ( i4_ret != OSR_OK || pt_obj->h_sync_lock == NULL_HANDLE )
        {
            return MIDXBULDR_INTERNAL_ERR;
        }        
    }
    
    /* select handler */
    i4_ret = midxbuld_match_handler(&t_media_format, &pt_obj->t_handler_fct_tbl);
    if (i4_ret != MIDXBULDR_OK)
    {
        return MIDXBULDR_FMT_NOT_SUPPORT;
    }
    pf_mem_open = pt_obj->t_handler_fct_tbl.pf_mopen;
    if (pf_mem_open)
    {
        if (pf_mem_open(pv_mem_data, z_mem_leng, pt_key_info, (VOID*)pt_obj) != MIDXBULDR_OK)
        {
            return MIDXBULDR_FCT_NOT_SUPPORT;
        }

        /* start key frame table index building */
        return (x_midxbuld_start_indexing(*ph_midxbuld));
    }
    else
    {
        return MIDXBULDR_FCT_NOT_SUPPORT;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  x_midxbuld_close
 *
 * Description: TThis API closes the media index builder handle.
 *
 * Inputs:  h_midxbuld         specify the media index builder handle which will be closed.
 *
 * Outputs: 
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_midxbuld_close (HANDLE_T h_midxbuld)
{
    INT32                               i4_ret;
    HANDLE_TYPE_T                       e_handle_type;
    VOID*                               pv_obj = NULL;
    MIDXBULD_OBJ_T*                     pt_obj;
    MIDXBULD_NOTIFY_ASYNC_EXEC_MSG_T    t_notify_msg;
        
    DBG_API(("[MIDXBULD] x_midxbuld_close: h_midxbuld:%x \n\r"
        , h_midxbuld));  
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (h_midxbuld == NULL_HANDLE)
    {
        return MIDXBULDR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_midxbuld, &e_handle_type, &pv_obj);
    if ((i4_ret != HR_OK) 
        || ((e_handle_type != HT_MIDXBULD_FILE_TYPE) && (e_handle_type != HT_MIDXBULD_MEM_TYPE))
        || (pv_obj == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/

    pt_obj = (MIDXBULD_OBJ_T*)pv_obj;
    /*stop indexing*/
    pt_obj->b_abort_flag = TRUE;
    /*send close msg*/
    t_notify_msg.h_midxbuld = pt_obj->h_midxbuld;
    t_notify_msg.e_msg_type = MIDXBULD_MSG_ASYNC_EXEC_TYPE;
    t_notify_msg.u.t_async_exec_msg.e_async_exec = MIDXBULD_ASYNC_EXEC_CLOSE;
    i4_ret = midxbuld_send_notify_msg (&t_notify_msg);
    x_sema_lock(pt_obj->h_sync_lock, X_SEMA_OPTION_WAIT);
    if (pt_obj->h_sync_lock)
    {
        x_sema_delete(pt_obj->h_sync_lock);
        pt_obj->h_sync_lock = NULL_HANDLE;
    }
    handle_free(pt_obj->h_midxbuld, TRUE); 
    return i4_ret;

}

/*-----------------------------------------------------------------------------
 * Name:  x_midxbuld_start_indexing
 *
 * Description: specify the media index builder handle which will start indexing
 *
 * Inputs:  h_midxbuld      specify the media info handle which will be retrieved information.
 *          
 *
 * Outputs: 
 *
 * Returns: MIDXBULDR    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_midxbuld_start_indexing (HANDLE_T h_midxbuld)   
{
    INT32                               i4_ret;
    HANDLE_TYPE_T                       e_handle_type;
    VOID*                               pv_obj = NULL;
    MIDXBULD_OBJ_T*                     pt_obj;
    MIDXBULD_NOTIFY_ASYNC_EXEC_MSG_T    t_notify_msg;
        
    DBG_API(("[MIDXBULD] x_midxbuld_start_indexing: h_midxbuld:%x \n\r"
        , h_midxbuld));  
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (h_midxbuld == NULL_HANDLE)
    {
        return MIDXBULDR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_midxbuld, &e_handle_type, &pv_obj);
    if ((i4_ret != HR_OK) 
        || ((e_handle_type != HT_MIDXBULD_FILE_TYPE) && (e_handle_type != HT_MIDXBULD_MEM_TYPE))
        || (pv_obj == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    
    pt_obj = (MIDXBULD_OBJ_T*)pv_obj;
    pt_obj->b_abort_flag = FALSE;
    t_notify_msg.h_midxbuld = pt_obj->h_midxbuld;
    t_notify_msg.e_msg_type = MIDXBULD_MSG_ASYNC_EXEC_TYPE;
    t_notify_msg.u.t_async_exec_msg.e_async_exec = MIDXBULD_ASYNC_EXEC_START_INDEXING;
    return midxbuld_send_notify_msg (&t_notify_msg);

}

/*-----------------------------------------------------------------------------
 * Name:  x_midxbuld_stop_indexing
 *
 * Description: specify the media index builder handle which will stop indexing
 *
 * Inputs:  h_midxbuld      specify the media info handle which will be retrieved information.
 *          
 *
 * Outputs: 
 *
 * Returns: MIDXBULDR    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_midxbuld_stop_indexing (HANDLE_T h_midxbuld)   
{
    INT32                               i4_ret;
    HANDLE_TYPE_T                       e_handle_type;
    VOID*                               pv_obj = NULL;
    MIDXBULD_OBJ_T*                     pt_obj;
        
    DBG_API(("[MIDXBULD] x_midxbuld_stop_indexing: h_midxbuld:%x \n\r"
        , h_midxbuld));  
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (h_midxbuld == NULL_HANDLE)
    {
        return MIDXBULDR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_midxbuld, &e_handle_type, &pv_obj);
    if ((i4_ret != HR_OK) 
        || ((e_handle_type != HT_MIDXBULD_FILE_TYPE) && (e_handle_type != HT_MIDXBULD_MEM_TYPE))
        || (pv_obj == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_obj = (MIDXBULD_OBJ_T*)pv_obj;
    pt_obj->b_abort_flag = TRUE;
    
    return MIDXBULDR_OK;

}

/*-----------------------------------------------------------------------------
 * Name:  x_midxbuld_get_info
 *
 * Description: This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  h_midxbuld   specify the media idx buld handle which will be retrieved information.
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_midxbuld_get_info (
                        HANDLE_T                 h_midxbuld, 
                        MIDXBULD_GET_INFO_TYPE_T e_info_type, 
                        VOID*                    pv_buf,     
                        SIZE_T*                  pz_buf_leng)
{
    INT32                               i4_ret;
    HANDLE_TYPE_T                       e_handle_type;
    VOID*                               pv_obj = NULL;
    MIDXBULD_OBJ_T*                     pt_obj;
    handler_get_info_fct                pf_get_info;
    
    DBG_API(("[midxbuld] x_midxbuld_get_info: h_midxbuld:%d \t e_info_type:%x \t pv_buf:%x \t pz_buf_leng:%x\n\r"
        , h_midxbuld, e_info_type, pv_buf, pz_buf_leng));  
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (h_midxbuld == NULL_HANDLE)
    {
        return MIDXBULDR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_midxbuld, &e_handle_type, &pv_obj);
    if ((i4_ret != HR_OK) 
        || ((e_handle_type != HT_MIDXBULD_FILE_TYPE) && (e_handle_type != HT_MIDXBULD_MEM_TYPE))
        || (pv_obj == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_obj = (MIDXBULD_OBJ_T*)pv_obj;
    
    pf_get_info = pt_obj->t_handler_fct_tbl.pf_get_info;
    if (pf_get_info)
    {
        return pf_get_info(pv_obj, e_info_type, pv_buf, pz_buf_leng);
    }

    return MIDXBULDR_FCT_NOT_SUPPORT;
    
}

/*-----------------------------------------------------------------------------
 * Name:  x_midxbuld_set_info
 *
 * Description: This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  h_midxbuld   specify the media idx buld handle which will be retrieved information.
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_midxbuld_set_info (
                        HANDLE_T                 h_midxbuld, 
                        MIDXBULD_SET_INFO_TYPE_T e_info_type, 
                        VOID*                    pv_buf,     
                        SIZE_T*                  pz_buf_leng)
{
    INT32                               i4_ret;
    HANDLE_TYPE_T                       e_handle_type;
    VOID*                               pv_obj = NULL;
    MIDXBULD_OBJ_T*                     pt_obj;
    handler_set_info_fct                pf_set_info;
    
    DBG_API(("[midxbuld] x_midxbuld_set_info: h_midxbuld:%d \t e_info_type:%x \t pv_buf:%x \t pz_buf_leng:%x\n\r"
        , h_midxbuld, e_info_type, pv_buf, pz_buf_leng));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (h_midxbuld == NULL_HANDLE)
    {
        return MIDXBULDR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_midxbuld, &e_handle_type, &pv_obj);
    if ((i4_ret != HR_OK) 
        || ((e_handle_type != HT_MIDXBULD_FILE_TYPE) && (e_handle_type != HT_MIDXBULD_MEM_TYPE))
        || (pv_obj == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_obj = (MIDXBULD_OBJ_T*)pv_obj;
    
    pf_set_info = pt_obj->t_handler_fct_tbl.pf_set_info;
    if (pf_set_info)
    {
        return pf_set_info(pv_obj, e_info_type, pv_buf, pz_buf_leng);
    }

    return MIDXBULDR_FCT_NOT_SUPPORT;
    
}

