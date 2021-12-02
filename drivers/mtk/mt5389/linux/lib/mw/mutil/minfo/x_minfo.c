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
 * $RCSfile: x_minfo.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description:
 *         This file contains all the media info interface APIs
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
#include "mutil/mfmtrecg/x_mfmtrecg.h"
#include "mutil/minfo/x_minfo.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/minfo_dbg.h"
#include "mutil/minfo/minfo_cli.h"
#include "mutil/mm_util_input.h"
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
#include "x_minfo.h"
#include "minfo.h"
#include "minfo_dbg.h"
#include "minfo_cli.h"
#include "x_dbg.h"
#include "u_dbg.h"
#include "../mm_util.h"
#include "../mm_util_input.h"

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
static BOOL _minfo_free_handle (
    HANDLE_T        h_handle,
    HANDLE_TYPE_T   e_type,
    VOID*           pv_obj,
    VOID*           pv_tag,
    BOOL            b_req_handle)
{
    MINFO_OBJ_T*                    pt_obj = NULL;

    if (pv_obj)
    {
         if ((e_type == HT_MINFO_FILE_TYPE)
                || (e_type == HT_MINFO_MEM_TYPE))
        {
            pt_obj = (MINFO_OBJ_T*)pv_obj;
            if (e_type == HT_MINFO_FILE_TYPE)
            {
                if (pt_obj->pv_buff_or_file_path_name)
                {
                    _mm_util_mem_free(pt_obj->pv_buff_or_file_path_name);
                    pt_obj->pv_buff_or_file_path_name = NULL;
                }
            }
            _mm_util_mem_free(pv_obj);
            pv_obj = NULL;
            return TRUE;
        }
    }
    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name:  x_minfo_fopen
 *
 * Description: This API opens a media file to get media information.
 *
 * Inputs:  ps_path_name    specify the path string of file which will be opened in minfo.
 *          pf_nfy          specify the function point of notification function.
 *          pv_tag          specify the tag which will be send back to AP when minfo notification.
 *          pt_format       if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file.
 *
 * Outputs: pt_format       If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP.
 *          ph_minfo        The parameter will retrieve a media info handle.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_minfo_fopen (
                        CHAR*               ps_path_name,
                        x_minfo_nfy_fct     pf_nfy,
                        VOID*               pv_tag,
                        MEDIA_FORMAT_T*     pt_format,
                        HANDLE_T*           ph_minfo)
{

#ifndef __NO_FM__

    INT32                           i4_ret = MINFOR_INV_ARG;
    MEDIA_FORMAT_T                  t_media_format;
    MINFO_OBJ_T*                    pt_obj;
    SIZE_T                          z_file_path_name_length;
    handler_file_open_fct           pf_file_open;
    MINFO_NOTIFY_ASYNC_EXEC_MSG_T   t_notify_msg;
    FM_FILE_INFO_T                  t_fm_info;

    DBG_API(("[minfo] x_minfo_fopen: ps_path_name:%s \t pf_nfy:%x \t pv_tag:%x \t pt_format:%x \t ph_minfo:%x\n\r"
        , ps_path_name, pf_nfy, pv_tag, pt_format, ph_minfo));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((ps_path_name == NULL)
       || (pt_format == NULL)
       || (ph_minfo == NULL))
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* make sure file format */
    if (pt_format->e_media_type == MEDIA_TYPE_UNKNOWN)
    {
        t_media_format.b_fav_recg_only = FALSE;
        i4_ret = x_mfmtrecg_get_file_fmt(ps_path_name, &t_media_format);
        if ((t_media_format.e_media_type == MEDIA_TYPE_UNKNOWN)|| (i4_ret != MFMTRECGR_OK ))
        {
            return MINFOR_FMT_NOT_SUPPORT;
        }
    }
    else
    {
        t_media_format = *pt_format;
    }
    /* allocate elm_obj memory */
    pt_obj = _mm_util_mem_alloc(sizeof(MINFO_OBJ_T));
    if (pt_obj == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_obj, 0, sizeof(MINFO_OBJ_T));

    z_file_path_name_length = x_strlen(ps_path_name) + 1;/* NULL terminator*/
    pt_obj->pv_buff_or_file_path_name = _mm_util_mem_alloc(z_file_path_name_length);
    if ( pt_obj->pv_buff_or_file_path_name == NULL)
    {
        _mm_util_mem_free(pt_obj);
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_obj->pv_buff_or_file_path_name, 0, z_file_path_name_length);
    if (HR_OK != handle_alloc(HT_MINFO_FILE_TYPE, pt_obj, NULL, _minfo_free_handle, ph_minfo))
    {

        _mm_util_mem_free(pt_obj);
        return MINFOR_OUT_OF_HANDLE;
    }

    /* fill data */
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path_name, FM_READ_ONLY, 0777, FALSE, &pt_obj->h_file);
    if (i4_ret != FMR_OK)
    {
        return MINFOR_FILE_ERR;
    }
    x_fm_get_info_by_handle(pt_obj->h_file, &t_fm_info);
    x_strcpy(pt_obj->pv_buff_or_file_path_name, ps_path_name);
    pt_obj->t_media_format = t_media_format;
    pt_obj->h_minfo = *ph_minfo;
    pt_obj->z_buff_leng = (UINT32)t_fm_info.ui8_size;
    pt_obj->pf_nfy = pf_nfy;
    pt_obj->pv_tag = pv_tag;
    pt_obj->b_abort_flag = FALSE;
    pt_obj->b_parsed_id3 = FALSE;
    pt_obj->t_input_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_obj->t_input_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_obj->t_input_fct_tbl.pf_input2 = mm_util_finput2bytes_l_fct;
    pt_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_l_fct;
    pt_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_l_fct;
    pt_obj->t_input_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_obj->t_input_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;

    if (pt_obj->h_sync_lock == NULL_HANDLE)
    {
        /* create semaphore */
        i4_ret = x_sema_create( &pt_obj->h_sync_lock,
                             X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_LOCK );

        if ( i4_ret != OSR_OK || pt_obj->h_sync_lock == NULL_HANDLE )
        {
            x_fm_close(pt_obj->h_file);
            return MINFOR_INTERNAL_ERR;
        }
    }
    /* select handler */
    i4_ret = minfo_match_handler(&t_media_format, &pt_obj->t_handler_fct_tbl);
    if (i4_ret != MINFOR_OK)
    {
        x_fm_close(pt_obj->h_file);
        return MINFOR_FMT_NOT_SUPPORT;
    }
    pf_file_open = pt_obj->t_handler_fct_tbl.pf_fopen;
    if (pf_file_open)
    {
        if (pf_file_open(ps_path_name, (VOID*)pt_obj) != MINFOR_OK )
        {
            x_fm_close(pt_obj->h_file);
            return MINFOR_FCT_NOT_SUPPORT;
        }
    }
    else
    {
        x_fm_close(pt_obj->h_file);
        return MINFOR_FCT_NOT_SUPPORT;
    }

    t_notify_msg.h_minfo = pt_obj->h_minfo;
    t_notify_msg.e_msg_type = MINFO_MSG_ASYNC_EXEC_TYPE;
    t_notify_msg.u.t_async_exec_msg.b_start_parsing = TRUE;
    return minfo_send_notify_msg (&t_notify_msg);
#else
    return 0;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  x_minfo_mopen
 *
 * Description: This API opens a media file to get media information.
 *
 * Inputs:  pv_mem_data     specify the memory address of media data buffer.
 *          z_mem_leng      specify the memory length of media data buffer.
 *          pf_nfy          specify the function point of notification function.
 *          pv_tag          specify the tag which will be send back to AP when minfo notification.
 *          pt_format       if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file.
 *
 * Outputs: pt_format       If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP.
 *          ph_minfo        The parameter will retrieve a media info handle.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_minfo_mopen (
                        VOID*               pv_mem_data,
                        SIZE_T              z_mem_leng,
                        x_minfo_nfy_fct     pf_nfy,
                        VOID*               pv_tag,
                        MEDIA_FORMAT_T*     pt_format,
                        HANDLE_T*           ph_minfo)
{
    INT32                           i4_ret = MINFOR_INV_ARG;
    MEDIA_FORMAT_T                  t_media_format;
    MINFO_OBJ_T*                    pt_obj;
    handler_mem_open_fct            pf_mem_open;
    MINFO_NOTIFY_ASYNC_EXEC_MSG_T   t_notify_msg;


    DBG_API(("[minfo] x_minfo_mopen: pv_mem_data:%x \t z_mem_leng:%x \t pf_nfy:%x \t pv_tag:%x \t pt_format:%x \t ph_minfo:%x\n\r"
        , pv_mem_data, z_mem_leng, pf_nfy, pv_tag, pt_format, ph_minfo));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL)
       || (ph_minfo == NULL))
    {
        return MINFOR_INV_ARG;
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
            return MINFOR_FMT_NOT_SUPPORT;
        }
    }
    else
    {
        t_media_format = *pt_format;
    }

    /* allocate obj memory */
    pt_obj = _mm_util_mem_alloc(sizeof(MINFO_OBJ_T));
    if (pt_obj == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_obj, 0, sizeof(MINFO_OBJ_T));

    if (HR_OK != handle_alloc(HT_MINFO_MEM_TYPE, pt_obj, NULL, _minfo_free_handle, ph_minfo))
    {
        _mm_util_mem_free(pt_obj);
        return MINFOR_OUT_OF_HANDLE;
    }

    /* fill data */
    pt_obj->t_media_format = t_media_format;
    pt_obj->h_minfo = *ph_minfo;
    pt_obj->pv_buff_or_file_path_name = pv_mem_data;
    pt_obj->z_buff_leng = z_mem_leng;
    pt_obj->pf_nfy = pf_nfy;
    pt_obj->pv_tag = pv_tag;
    pt_obj->h_src_feeder = NULL_HANDLE;
    pt_obj->b_abort_flag = FALSE;
    pt_obj->b_parsed_id3 = FALSE;
#ifdef ENABLE_MMS_SUPPORT
    pt_obj->b_only_has_hdr = t_media_format.b_only_has_hdr;
#endif

    if(z_mem_leng != 0)
    {
        pt_obj->t_input_fct_tbl.pf_copybytes = mm_util_mcopybytes_fct;
        pt_obj->t_input_fct_tbl.pf_input1 = mm_util_minput1bytes_fct;
        pt_obj->t_input_fct_tbl.pf_input2 = mm_util_minput2bytes_l_fct;
        pt_obj->t_input_fct_tbl.pf_input3 = mm_util_minput3bytes_l_fct;
        pt_obj->t_input_fct_tbl.pf_input4 = mm_util_minput4bytes_l_fct;
        pt_obj->t_input_fct_tbl.pf_set_pos = mm_util_msetpos_fct;
        pt_obj->t_input_fct_tbl.pf_get_pos = mm_util_mgetpos_fct;
    }
    else
    {
        MEDIA_FEEDER_T* pt_src_feeder;
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        pt_obj->h_src_feeder = pt_src_feeder->h_feeder;
        pt_obj->t_input_fct_tbl.pf_copybytes =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes;
        pt_obj->t_input_fct_tbl.pf_input1 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input1;
        pt_obj->t_input_fct_tbl.pf_input2 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input2;
        pt_obj->t_input_fct_tbl.pf_input3 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input3;
        pt_obj->t_input_fct_tbl.pf_input4 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input4;
        pt_obj->t_input_fct_tbl.pf_set_pos =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos;
        pt_obj->t_input_fct_tbl.pf_get_pos =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos;
        if(pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos &&
           pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos)
        {
            i4_ret = pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos
                (pt_src_feeder, 0, FM_SEEK_END);
            if (i4_ret != MINFOR_OK)
            {
                DBG_ERROR(("ERROR: seek to file end fail. \r\n"));
                handle_free(*ph_minfo, TRUE);
                *ph_minfo = NULL_HANDLE;
                return i4_ret;
            }
            pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos
                (pt_src_feeder, &pt_obj->z_buff_leng);     
        }
    }

    if (pt_obj->h_sync_lock == NULL_HANDLE)
    {
        /* create semaphore */
        i4_ret = x_sema_create( &pt_obj->h_sync_lock,
                             X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_LOCK );

        if ( i4_ret != OSR_OK || pt_obj->h_sync_lock == NULL_HANDLE )
        {
            handle_free(*ph_minfo, TRUE);
            *ph_minfo = NULL_HANDLE;
            return MINFOR_INTERNAL_ERR;
        }
    }

    /* select handler */
    i4_ret = minfo_match_handler(&t_media_format, &pt_obj->t_handler_fct_tbl);
    if (i4_ret != MINFOR_OK)
    {
        x_sema_delete(pt_obj->h_sync_lock);
        handle_free(*ph_minfo, TRUE);
        *ph_minfo = NULL_HANDLE;
        return MINFOR_FMT_NOT_SUPPORT;
    }
    pf_mem_open = pt_obj->t_handler_fct_tbl.pf_mopen;
    if (pf_mem_open)
    {
        if (pf_mem_open(pv_mem_data, z_mem_leng, (VOID*)pt_obj) != MINFOR_OK)
        {
            x_sema_delete(pt_obj->h_sync_lock);
            handle_free(*ph_minfo, TRUE);
            *ph_minfo = NULL_HANDLE;
            return MINFOR_FCT_NOT_SUPPORT;
        }
    }
    else
    {
        x_sema_delete(pt_obj->h_sync_lock);
        handle_free(*ph_minfo, TRUE);
        *ph_minfo = NULL_HANDLE;
        return MINFOR_FCT_NOT_SUPPORT;
    }

    t_notify_msg.h_minfo = pt_obj->h_minfo;
    t_notify_msg.e_msg_type = MINFO_MSG_ASYNC_EXEC_TYPE;
    t_notify_msg.u.t_async_exec_msg.b_start_parsing = TRUE;
    return minfo_send_notify_msg (&t_notify_msg);

}

/*-----------------------------------------------------------------------------
 * Name:  x_minfo_fopen_sync
 *
 * Description: This API opens a media file to get media information -- sync version.
 *
 * Inputs:  ps_path_name    specify the path string of file which will be opened in minfo.
 *          pf_nfy          specify the function point of notification function.
 *          pv_tag          specify the tag which will be send back to AP when minfo notification.
 *          pt_format       if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file.
 *
 * Outputs: pt_format       If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP.
 *          ph_minfo        The parameter will retrieve a media info handle.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_minfo_fopen_sync (
    CHAR*                            ps_path_name,
    MEDIA_FORMAT_T*                  pt_format,
    HANDLE_T*                        ph_minfo
)
{
#ifndef __NO_FM__
    INT32                            i4_ret = MINFOR_INV_ARG;
    MEDIA_FORMAT_T                   t_media_format;
    MINFO_OBJ_T*                     pt_obj;
    SIZE_T                           z_file_path_name_length;
    handler_file_open_fct            pf_file_open;
    handler_parse_fct                pf_file_parse;
    FM_FILE_INFO_T                   t_fm_info;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((ps_path_name == NULL) || (pt_format == NULL) || (ph_minfo == NULL))
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /* make sure file format */
    if (pt_format->e_media_type == MEDIA_TYPE_UNKNOWN)
    {
        t_media_format.b_fav_recg_only = pt_format->b_fav_recg_only;
        i4_ret = x_mfmtrecg_get_file_fmt(ps_path_name, &t_media_format);
        if ((t_media_format.e_media_type == MEDIA_TYPE_UNKNOWN)|| (i4_ret != MFMTRECGR_OK ))
        {
            return MINFOR_FMT_NOT_SUPPORT;
        }
    }
    else
    {
        t_media_format = *pt_format;
    }
    /* allocate elm_obj memory */
    pt_obj = _mm_util_mem_alloc(sizeof(MINFO_OBJ_T));
    if (pt_obj == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_obj, 0, sizeof(MINFO_OBJ_T));

    z_file_path_name_length = x_strlen(ps_path_name) + 1;/* NULL terminator*/
    pt_obj->pv_buff_or_file_path_name = _mm_util_mem_alloc(z_file_path_name_length);
    if ( pt_obj->pv_buff_or_file_path_name == NULL)
    {
        _mm_util_mem_free(pt_obj);
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_obj->pv_buff_or_file_path_name, 0, z_file_path_name_length);
    if (HR_OK != handle_alloc(HT_MINFO_FILE_TYPE, pt_obj, NULL, _minfo_free_handle, ph_minfo))
    {
        _mm_util_mem_free(pt_obj);
        return MINFOR_OUT_OF_HANDLE;
    }

    /* fill data */
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path_name, FM_READ_ONLY, 0777, FALSE, &pt_obj->h_file);
    if (i4_ret != FMR_OK)
    {
        return MINFOR_FILE_ERR;
    }
    x_fm_get_info_by_handle(pt_obj->h_file, &t_fm_info);
    x_strcpy(pt_obj->pv_buff_or_file_path_name, ps_path_name);
    pt_obj->t_media_format = t_media_format;
    pt_obj->h_minfo = *ph_minfo;
    pt_obj->z_buff_leng = (UINT32)t_fm_info.ui8_size;
    pt_obj->t_input_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_obj->t_input_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_obj->t_input_fct_tbl.pf_input2 = mm_util_finput2bytes_l_fct;
    pt_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_l_fct;
    pt_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_l_fct;
    pt_obj->t_input_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_obj->t_input_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;
    pt_obj->b_abort_flag = FALSE;

    if (pt_obj->h_sync_lock == NULL_HANDLE)
    {
        /* create semaphore */
        i4_ret = x_sema_create( &pt_obj->h_sync_lock,
                             X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_LOCK );

        if ( i4_ret != OSR_OK || pt_obj->h_sync_lock == NULL_HANDLE )
        {
            x_fm_close(pt_obj->h_file);
            return MINFOR_INTERNAL_ERR;
        }
    }

    /* select handler */
    i4_ret = minfo_match_handler(&t_media_format, &pt_obj->t_handler_fct_tbl);
    if (i4_ret != MINFOR_OK)
    {
        x_fm_close(pt_obj->h_file);
        return MINFOR_FMT_NOT_SUPPORT;
    }
    pf_file_open = pt_obj->t_handler_fct_tbl.pf_fopen;
    if (pf_file_open)
    {
        if (pf_file_open(ps_path_name, (VOID*)pt_obj) != MINFOR_OK )
        {
            x_fm_close(pt_obj->h_file);
            return MINFOR_FCT_NOT_SUPPORT;
        }
    }
    else
    {
        x_fm_close(pt_obj->h_file);
        return MINFOR_FCT_NOT_SUPPORT;
    }
    pf_file_parse = pt_obj->t_handler_fct_tbl.pf_parse;
    if (pf_file_parse)
    {
        if(t_media_format.e_media_type == MEDIA_TYPE_AUDIO_ES && 
           t_media_format.t_media_subtype.u.e_aud_subtype != MEDIA_AUD_SUBTYPE_MP3 &&
           t_media_format.t_media_subtype.u.e_aud_subtype != MEDIA_AUD_SUBTYPE_WMA)
        {
            /*Don't parse audio es because no ID3 info in audio es*/
            x_fm_close(pt_obj->h_file);
            pt_obj->h_file = NULL_HANDLE;
            return MINFOR_OK;
        }
        else if (pf_file_parse((VOID*)pt_obj) != MINFOR_OK)
        {
            x_fm_close(pt_obj->h_file);
            pt_obj->h_file = NULL_HANDLE;
            return MINFOR_FCT_NOT_SUPPORT;
        }
        pt_obj->b_parsing_successful = TRUE;        
        
    }
    else
    {
        x_fm_close(pt_obj->h_file);
        pt_obj->h_file = NULL_HANDLE;
        return MINFOR_FCT_NOT_SUPPORT;
    }

    x_fm_close(pt_obj->h_file);
    pt_obj->h_file = NULL_HANDLE;
    return MINFOR_OK;
#else
    return 0;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  x_minfo_mopen_sync
 *
 * Description: This API opens a media file to get media information -- sync version.
 *
 * Inputs:  pv_mem_data     specify the memory address of media data buffer.
 *          z_mem_leng      specify the memory length of media data buffer.
 *          pt_format       if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file.
 *
 * Outputs: pt_format       If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP.
 *          ph_minfo        The parameter will retrieve a media info handle.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_minfo_mopen_sync(
    VOID*                            pv_mem_data,
    SIZE_T                           z_mem_leng,
    MEDIA_FORMAT_T*                  pt_format,
    HANDLE_T*                        ph_minfo
)
{
    INT32                            i4_ret = MINFOR_INV_ARG;
    MEDIA_FORMAT_T                   t_media_format;
    MINFO_OBJ_T*                     pt_obj;
    handler_mem_open_fct             pf_mem_open;
    handler_parse_fct                pf_file_parse;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL) || (ph_minfo == NULL))
    {
        return MINFOR_INV_ARG;
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
            return MINFOR_FMT_NOT_SUPPORT;
        }
    }
    else
    {
        t_media_format = *pt_format;
    }

    /* allocate obj memory */
    pt_obj = _mm_util_mem_alloc(sizeof(MINFO_OBJ_T));
    if (pt_obj == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }
    x_memset(pt_obj, 0, sizeof(MINFO_OBJ_T));

    if (HR_OK != handle_alloc(HT_MINFO_MEM_TYPE, pt_obj, NULL, _minfo_free_handle, ph_minfo))
    {
        _mm_util_mem_free(pt_obj);
        return MINFOR_OUT_OF_HANDLE;
    }

    /* fill data */
    pt_obj->t_media_format = t_media_format;
    pt_obj->h_minfo = *ph_minfo;
    pt_obj->pv_buff_or_file_path_name = pv_mem_data;
    pt_obj->z_buff_leng = z_mem_leng;
    pt_obj->h_src_feeder = NULL_HANDLE;
    pt_obj->b_abort_flag = FALSE;

    if(z_mem_leng != 0)
    {
        pt_obj->t_input_fct_tbl.pf_copybytes = mm_util_mcopybytes_fct;
        pt_obj->t_input_fct_tbl.pf_input1 = mm_util_minput1bytes_fct;
        pt_obj->t_input_fct_tbl.pf_input2 = mm_util_minput2bytes_l_fct;
        pt_obj->t_input_fct_tbl.pf_input3 = mm_util_minput3bytes_l_fct;
        pt_obj->t_input_fct_tbl.pf_input4 = mm_util_minput4bytes_l_fct;
        pt_obj->t_input_fct_tbl.pf_set_pos = mm_util_msetpos_fct;
        pt_obj->t_input_fct_tbl.pf_get_pos = mm_util_mgetpos_fct;
    }
    else
    {
        MEDIA_FEEDER_T* pt_src_feeder;
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        pt_obj->h_src_feeder = pt_src_feeder->h_feeder;
        pt_obj->t_input_fct_tbl.pf_copybytes =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes;
        pt_obj->t_input_fct_tbl.pf_input1 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input1;
        pt_obj->t_input_fct_tbl.pf_input2 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input2;
        pt_obj->t_input_fct_tbl.pf_input3 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input3;
        pt_obj->t_input_fct_tbl.pf_input4 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input4;
        pt_obj->t_input_fct_tbl.pf_set_pos =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos;
        pt_obj->t_input_fct_tbl.pf_get_pos =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos;
    }

    if (pt_obj->h_sync_lock == NULL_HANDLE)
    {
        /* create semaphore */
        i4_ret = x_sema_create( &pt_obj->h_sync_lock,
                             X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_LOCK );

        if ( i4_ret != OSR_OK || pt_obj->h_sync_lock == NULL_HANDLE )
        {
            return MINFOR_INTERNAL_ERR;
        }
    }

    /* select handler */
    i4_ret = minfo_match_handler(&t_media_format, &pt_obj->t_handler_fct_tbl);
    if (i4_ret != MINFOR_OK)
    {
        return MINFOR_FMT_NOT_SUPPORT;
    }
    pf_mem_open = pt_obj->t_handler_fct_tbl.pf_mopen;
    if (pf_mem_open)
    {
        if (pf_mem_open(pv_mem_data, z_mem_leng, (VOID*)pt_obj) != MINFOR_OK)
        {
            return MINFOR_FCT_NOT_SUPPORT;
        }
    }
    else
    {
        return MINFOR_FCT_NOT_SUPPORT;
    }

    pf_file_parse = pt_obj->t_handler_fct_tbl.pf_parse;
    if (pf_file_parse)
    {
        if (pf_file_parse((VOID*)pt_obj) != MINFOR_OK)
        {
            return MINFOR_FCT_NOT_SUPPORT;
        }
    }
    else
    {
        return MINFOR_FCT_NOT_SUPPORT;
    }

    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  x_minfo_close
 *
 * Description: This API closes the media info handle.
 *
 * Inputs:  h_minfo         specify the media info handle which will be closed.
 *
 * Outputs:
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_minfo_close (HANDLE_T h_minfo)
{
    INT32                           i4_ret;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    MINFO_OBJ_T*                    pt_obj;
    MINFO_NOTIFY_ASYNC_EXEC_MSG_T   t_notify_msg;

    DBG_API(("[minfo] x_minfo_close: h_minfo:%x \n\r"
        , h_minfo));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (h_minfo == NULL_HANDLE)
    {
        return MINFOR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_minfo, &e_handle_type, &pv_obj);
    if ((i4_ret != HR_OK)
        || ((e_handle_type != HT_MINFO_FILE_TYPE) && (e_handle_type != HT_MINFO_MEM_TYPE))
        || (pv_obj == NULL))
    {
        return MINFOR_INV_HANDLE;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/

    pt_obj = (MINFO_OBJ_T*)pv_obj;
    pt_obj->b_abort_flag = TRUE;
    if(pt_obj->b_parsing_done)
    {
        if (pt_obj->t_handler_fct_tbl.pf_close)
        {
            pt_obj->t_handler_fct_tbl.pf_close((VOID*)pt_obj);
        }
        pt_obj->t_handler_fct_tbl.pf_close = NULL;
    }
    t_notify_msg.h_minfo = pt_obj->h_minfo;
    t_notify_msg.e_msg_type = MINFO_MSG_NOTIFY_TYPE;
    t_notify_msg.u.t_nfy_msg.e_event = MINFO_NFY_EVENT_HANDLE_CLOSED;
    t_notify_msg.u.t_nfy_msg.pv_data1 = NULL;
    t_notify_msg.u.t_nfy_msg.pv_data2 = NULL;
    i4_ret = minfo_send_notify_msg (&t_notify_msg);
    x_sema_lock(pt_obj->h_sync_lock, X_SEMA_OPTION_WAIT);
    if (pt_obj->h_sync_lock)
    {
        x_sema_delete(pt_obj->h_sync_lock);
        pt_obj->h_sync_lock = NULL_HANDLE;
    }
    handle_free(pt_obj->h_minfo, TRUE); 
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  x_minfo_get_info
 *
 * Description: This API retrieves the information of media info handle. It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  h_minfo      specify the media info handle which will be retrieved information.
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_minfo_get_info (
                        HANDLE_T            h_minfo,
                        MINFO_INFO_TYPE_T   e_info_type,
                        MINFO_REC_IDX_T     ui2_index,
                        VOID*               pv_buf,
                        SIZE_T*             pz_buf_leng)
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj = NULL;
    MINFO_OBJ_T*                pt_obj;
    handler_get_info_fct        pf_get_info = NULL;
#ifdef MINFO_GET_RAW_DATA_SUPPORT     
    MINFO_TYPE_RAW_DATA_T*      pt_minfo_raw_data = (MINFO_TYPE_RAW_DATA_T*)pv_buf; 
    UINT32                      ui4_read_size = 0;
#endif

    DBG_API(("[minfo] x_minfo_get_info: h_minfo:%x \t e_info_type:%x \t pv_buf:%x \t pz_buf_leng:%x \n\r"
        , h_minfo, e_info_type, pv_buf, pz_buf_leng));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pz_buf_leng == NULL)
    {
        return MINFOR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_minfo, &e_handle_type, &pv_obj);
    if ((i4_ret != HR_OK)
        || ((e_handle_type != HT_MINFO_FILE_TYPE) && (e_handle_type != HT_MINFO_MEM_TYPE))
        || (pv_obj == NULL))
    {
        return MINFOR_INV_HANDLE;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_obj = (MINFO_OBJ_T*)pv_obj;
    
#ifdef MINFO_GET_RAW_DATA_SUPPORT    
    if (e_info_type == MINFO_INFO_TYPE_RAW_DATA)
    {
        i4_ret = pt_obj->t_input_fct_tbl.pf_set_pos(pt_obj , pt_minfo_raw_data->ui8_seek_pos, FM_SEEK_BGN);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }
        
        i4_ret = pt_obj->t_input_fct_tbl.pf_copybytes(pt_obj, pt_minfo_raw_data->pv_get_info, 
                             pt_minfo_raw_data->z_read_data_size, 
                             pt_minfo_raw_data->z_read_data_size, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MINFOR_INTERNAL_ERR;
        }

        return MINFOR_OK;
    }
    else
#endif
    {
        pf_get_info = pt_obj->t_handler_fct_tbl.pf_get_info;
        if (pf_get_info)
        {
            return pf_get_info(pv_obj, e_info_type,ui2_index, pv_buf, pz_buf_leng);
        }
    }

    return MINFOR_FCT_NOT_SUPPORT;
}

#if 1/*MKV DivXPlus*/
INT32 x_minfo_set_info (
                        HANDLE_T            h_minfo,
                        MINFO_INFO_TYPE_T   e_info_type,
                        MINFO_REC_IDX_T     ui2_index,
                        VOID*               pv_buf,
                        SIZE_T*             pz_buf_leng)
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj = NULL;
    MINFO_OBJ_T*                pt_obj;
    handler_set_info_fct        pf_set_info = NULL;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pz_buf_leng == NULL)
    {
        return MINFOR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_minfo, &e_handle_type, &pv_obj);
    if ((i4_ret != HR_OK)
        || ((e_handle_type != HT_MINFO_FILE_TYPE) && (e_handle_type != HT_MINFO_MEM_TYPE))
        || (pv_obj == NULL))
    {
        return MINFOR_INV_HANDLE;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_obj = (MINFO_OBJ_T*)pv_obj;
    pf_set_info = pt_obj->t_handler_fct_tbl.pf_set_info;
    if (pf_set_info)
    {
        return pf_set_info(pv_obj, e_info_type,ui2_index, pv_buf, pz_buf_leng);
    }

    return MINFOR_FCT_NOT_SUPPORT;
}
#endif
/*-----------------------------------------------------------------------------
 * Name:  x_minfo_get_nums_recs
 *
 * Description:
 *
 * Inputs:  h_minfo         specify the media info handle which will be retrieved information.
 *               e_info_type  specify the information type of the media.
 *
 * Outputs: pui4_num_recs    The parameter will retrieve the record nums of specific MEDIA INFO TYPE
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 x_minfo_get_num_recs(
                           HANDLE_T           h_minfo,
                           MINFO_INFO_TYPE_T  e_info_type,
                   UINT16*                  pui2_num_recs)

{
    INT32                       i4_ret;
    HANDLE_TYPE_T       e_handle_type;
    VOID*                       pv_obj = NULL;
    MINFO_OBJ_T*                 pt_obj;
    handler_get_nums_recs_fct pf_get_rec_nums = NULL;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pui2_num_recs == NULL)
    {
        return MINFOR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_minfo, &e_handle_type, &pv_obj);
    if ((i4_ret != HR_OK)
        || ((e_handle_type != HT_MINFO_FILE_TYPE) && (e_handle_type != HT_MINFO_MEM_TYPE))
        || (pv_obj == NULL))
    {
        return MINFOR_INV_HANDLE;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_obj = (MINFO_OBJ_T*)pv_obj;
    pf_get_rec_nums = pt_obj->t_handler_fct_tbl.pf_get_rec_nums;
    if (pf_get_rec_nums)
    {
        return pf_get_rec_nums(pv_obj, e_info_type,pui2_num_recs);
    }
    return MINFOR_FCT_NOT_SUPPORT;
}
