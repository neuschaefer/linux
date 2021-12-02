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
/*----------------------------------------------------------------------------*/
/*! @file c_minfo.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file exports all media info API and constant for
 *         other module using.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_MUTIL_MINFO Multi Media Information
 *
 *  @ingroup groupMW
 *  @brief Provides external function TV Application
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _C_MINFO_H_
#define _C_MINFO_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "u_mfmtrecg.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/* This API opens a media file to get media information. */
/*------------------------------------------------------------------*/
/*! @brief          This API opens a media file to get media information.
 *  @param [in]     ps_path_name    specify the path string of file which will be opened in minfo.
 *  @param [in]     pf_nfy          specify the function point of notification function.
 *  @param [in]     pv_tag          specify the tag which will be send back to AP when minfo notification.
 *  @param [in]     pt_format       if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file.
 *  @param [out]    pt_format       If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP.
 *  @param [out]    ph_minfo        The parameter will retrieve a media info handle.
 *  @note
 *  @see
 *  @return 
 *  @retval      -MINFOR_OK               Success. The routine was successful and the operation has been completed.
 *  @retval      -MFMTRECGR_INV_ARG       input arg invalid
 *  @retval      -MINFOR_OUT_OF_MEMORY    mem not enough
 *  @retval      -MINFOR_FMT_NOT_SUPPORT  input pt_format not support
 *  @retval      -MINFOR_OUT_OF_HANDLE    out of handle
 *  @retval      -MINFOR_FILE_ERR         open file error
 *  @retval      -MINFOR_INTERNAL_ERR     internal error
 *  @retval      -MINFOR_FCT_NOT_SUPPORT  open function failed
 */
/*------------------------------------------------------------------*/
extern INT32 c_minfo_fopen (
                        CHAR*               ps_path_name,   /* (I) specify the path string of file which will be opened in minfo. */
                        x_minfo_nfy_fct     pf_nfy,         /* (I) specify the function point of notification function. */
                        VOID*               pv_tag,         /* (I) specify the tag which will be send back to AP when minfo notification. */
                        MEDIA_FORMAT_T*     pt_format,      /* (I/O) if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file./If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP. */
                        HANDLE_T*           ph_minfo);      /* (O) The parameter will retrieve a media info handle. */


/* This API opens a media file to get media information. */
/*------------------------------------------------------------------*/
/*! @brief          This API opens a mem to get media information.
 *  @param [in]     pv_mem_data     specify the memory address of media data buffer.
 *  @param [in]     z_mem_leng      specify the memory length of media data buffer.
 *  @param [in]     pv_tag          specify the tag which will be send back to AP when minfo notification.
 *  @param [in]     pt_format       if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file.
 *  @param [out]    pt_format       If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP.
 *  @param [out]    ph_minfo        The parameter will retrieve a media info handle.
 *  @note
 *  @see
 *  @return 
 *  @retval      -MINFOR_OK               Success. The routine was successful and the operation has been completed.
 *  @retval      -MFMTRECGR_INV_ARG       input arg invalid
 *  @retval      -MINFOR_OUT_OF_MEMORY    mem not enough
 *  @retval      -MINFOR_FMT_NOT_SUPPORT  input pt_format not support
 *  @retval      -MINFOR_OUT_OF_HANDLE    out of handle
 *  @retval      -MINFOR_FILE_ERR         open file error
 *  @retval      -MINFOR_INTERNAL_ERR     internal error
 *  @retval      -MINFOR_FCT_NOT_SUPPORT  open function failed
 */
/*------------------------------------------------------------------*/
extern INT32 c_minfo_mopen (
                        VOID*               pv_mem_data,    /* (I) specify the memory address of media data buffer. */
                        SIZE_T              z_mem_leng,     /* (I) specify the memory length of media data buffer. */
                        x_minfo_nfy_fct     pf_nfy,         /* (I) specify the function point of notification function. */
                        VOID*               pv_tag,         /* (I) specify the tag which will be send back to AP when minfo notification. */
                        MEDIA_FORMAT_T*     pt_format,      /* (I/O) if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file./If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP. */
                        HANDLE_T*           ph_minfo);      /* (O) The parameter will retrieve a media info handle. */

/* This API opens a media file to get media information -- sync version. */
/*------------------------------------------------------------------*/
/*! @brief          This API opens a media file to get media information -- sync version.
 *  @param [in]     ps_path_name    specify the path string of file which will be opened in minfo.
 *  @param [in]     pt_format       if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file.
 *  @param [out]    pt_format       If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP.
 *  @param [out]     ph_minfo        The parameter will retrieve a media info handle.
 *  @note
 *  @see
 *  @return 
 *  @retval      -MINFOR_OK               Success. The routine was successful and the operation has been completed.
 *  @retval      -MFMTRECGR_INV_ARG       input arg invalid
 *  @retval      -MINFOR_OUT_OF_MEMORY    mem not enough
 *  @retval      -MINFOR_FMT_NOT_SUPPORT  input pt_format not support
 *  @retval      -MINFOR_OUT_OF_HANDLE    out of handle
 *  @retval      -MINFOR_FILE_ERR         open file error
 *  @retval      -MINFOR_INTERNAL_ERR     internal error
 *  @retval      -MINFOR_FCT_NOT_SUPPORT  open function failed
 */
/*------------------------------------------------------------------*/
extern INT32 c_minfo_fopen_sync (
                        CHAR*               ps_path_name,   /* (I) specify the path string of file which will be opened in minfo. */
                        MEDIA_FORMAT_T*     pt_format,      /* (I/O) if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file./If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP. */
                        HANDLE_T*           ph_minfo);      /* (O) The parameter will retrieve a media info handle. */

/* This API opens a media file to get media information -- sync version. */
/*------------------------------------------------------------------*/
/*! @brief          This API opens a media file to get media information -- sync version.
 *  @param [in]     pv_mem_data     specify the memory address of media data buffer.
 *  @param [in]     z_mem_leng      specify the memory length of media data buffer.
 *  @param [out]    pt_format       If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP.
 *  @param [out]    ph_minfo        The parameter will retrieve a media info handle.
 *  @note
 *  @see
 *  @return 
 *  @retval      -MINFOR_OK               Success. The routine was successful and the operation has been completed.
 *  @retval      -MFMTRECGR_INV_ARG       input arg invalid
 *  @retval      -MINFOR_OUT_OF_MEMORY    mem not enough
 *  @retval      -MINFOR_FMT_NOT_SUPPORT  input pt_format not support
 *  @retval      -MINFOR_OUT_OF_HANDLE    out of handle
 *  @retval      -MINFOR_FILE_ERR         open file error
 *  @retval      -MINFOR_INTERNAL_ERR     internal error
 *  @retval      -MINFOR_FCT_NOT_SUPPORT  open function failed
 */
/*------------------------------------------------------------------*/
extern INT32 c_minfo_mopen_sync (
                        VOID*               pv_mem_data,    /* (I) specify the memory address of media data buffer. */
                        SIZE_T              z_mem_leng,     /* (I) specify the memory length of media data buffer. */
                        MEDIA_FORMAT_T*     pt_format,      /* (I/O) if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file./If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP. */
                        HANDLE_T*           ph_minfo);      /* (O) The parameter will retrieve a media info handle. */

/* This API closes the media info handle. */
/*------------------------------------------------------------------*/
/*! @brief          This API closes the media info handle.
 *  @param [in]     h_minfo         specify the media info handle which will be closed.
 *  @param [out]    
 *  @note
 *  @see
 *  @return 
 *  @retval      -  MINFOR_OK               Success. The routine was successful and the operation has been completed.
 *  @retval      -  MFMTRECGR_INV_ARG       Input arg invalid
 */
/*------------------------------------------------------------------*/
extern INT32 c_minfo_close (
                        HANDLE_T            h_minfo);       /* (I) specify the media info handle which will be closed. */

/* This API retrieves the information of media info handle. It will return MINFO_NOT_READY, before parse media complete. */
/*------------------------------------------------------------------*/
/*! @brief          This API retrieves the information of media info handle. It will return MINFO_NOT_READY, before parse media complete.
 *  @param [in]     h_minfo      specify the media info handle which will be retrieved information.
 *  @param [in]     e_info_type  specify the information type of the media.
 *  @param [in]     pv_buf       specify the memory buffer which will contain the media information.
 *  @param [in]     pz_buf_leng  specify the length of the buffer.
 *  @param [out]    pv_buf       specify the memory buffer which will contain the media information
 *  @param [out]    pz_buf_leng  If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length.
 *  @note
 *  @see
 *  @return 
 *  @retval      -MINFOR_OK               Success. The routine was successful and the operation has been completed.
 *  @retval      -MFMTRECGR_INV_ARG       input arg invalid
 *  @retval      -MINFOR_INV_HANDLE       invalid handle
 *  @retval      -MINFOR_FCT_NOT_SUPPORT  open function failed
 */
/*------------------------------------------------------------------*/
extern INT32 c_minfo_get_info (
                        HANDLE_T            h_minfo,            /* (I) specify the media info handle which will be retrieved information. */
                        MINFO_INFO_TYPE_T   e_info_type,        /* (I) specify the information type of the media. */
                        MINFO_REC_IDX_T     ui2_index,          /* (I) specify the index of record which will be retrieved in media info.*/
                        VOID*               pv_buf,             /* (I/O) specify the memory buffer which will contain the media information.
                                                                   /specify the memory buffer which will contain the media information , i.e. MINFO_REC_IDX_T*/
                        SIZE_T*             pz_buf_leng);       /* (I/O) specify the length of the buffer./If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length. */



/* This API closes the media info handle. */
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]     h_minfo         specify the media info handle which will be retrieved information.
 *  @param [in]     e_info_type  specify the information type of the media.
 *  @param [out]    pui4_num_recs    The parameter will retrieve the record nums of specific MEDIA INFO TYPE
 *  @note
 *  @see
 *  @return 
 *  @retval      -MINFOR_OK               Success. The routine was successful and the operation has been completed.
 *  @retval      -MFMTRECGR_INV_ARG       input arg invalid
 *  @retval      -MINFOR_INV_HANDLE       invalid handle
 *  @retval      -MINFOR_FCT_NOT_SUPPORT  open function failed
 */
/*------------------------------------------------------------------*/
extern INT32 c_minfo_get_num_recs(
                        HANDLE_T            h_minfo,            /* (I) specify the media info handle which will be closed. */
                        MINFO_INFO_TYPE_T   e_info_type,        /* (I) specify the information type of the media. */
                        UINT16*             pui4_num_recs);     /* (O) The parameter will retrieve the record nums of specific MEDIA INFO TYPE */

#ifdef __cplusplus
}
#endif

#endif /* _C_MINFO_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                              /* end of groupMW_MUTIL_MINFO*/
/*----------------------------------------------------------------------------*/

