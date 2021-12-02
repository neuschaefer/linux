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
/* @file c_fm_dlna.c 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *             This file implements public API of dlnafs.
 */
/*----------------------------------------------------------------------------*/

#include "c_fm_dlnafs.h"
#include "file_mngr/fm_ufs.h"
#include "x_fm_dlnafs.h"

extern INT32  x_fm_dlna_set_browse_recursive(CHAR* ps_filename, BOOL b_recursive);

extern INT32  x_fm_dlna_set_browse_file_ext(CHAR * ps_filename, CHAR * ps_ext_name);

extern INT32  x_fm_dlna_set_content_filter(CHAR * ps_filename, DLNA_FILE_CONTENT_TYPE_T e_browse_type);

extern INT32  x_fm_dlna_orderby(CHAR * ps_filename, DLNA_ORDERBY_TYPE_T e_orderby, BOOL b_override);

extern INT32 x_dlna_dmp_fm_get_media_info(CHAR *ps_filename, DLNA_MEDIA_INFO_T *pt_info);

extern BOOL x_fm_dlna_is_pause_supported(CHAR *ps_media_name);

extern BOOL x_fm_dlna_is_device_alive(VOID);

extern BOOL x_fm_dlna_is_seek_supported(CHAR * ps_media_name);

#ifdef DLNAFS_ENCODE_TITLE 
extern INT32 x_fm_dlna_parse_display_title(CHAR *ps_encoding_name, CHAR *ps_out_name, UINT32 *pui4_out_len);
#endif

INT32 c_fm_dlna_set_browse_recursive(CHAR* ps_filename, BOOL b_recursive)
{
#ifdef DLNA_SUPPORT
    return x_fm_dlna_set_browse_recursive(ps_filename, b_recursive);
#else
    return DLNAR_DMS_CANNOT_SUPPORT;
#endif
}
    
INT32 c_fm_dlna_set_browse_file_ext(CHAR * ps_filename, CHAR * ps_ext_name)
{
#ifdef DLNA_SUPPORT
    return x_fm_dlna_set_browse_file_ext(ps_filename, ps_ext_name);
#else
    return DLNAR_DMS_CANNOT_SUPPORT;
#endif
}


INT32 c_fm_dlna_set_content_filter(CHAR * ps_filename, DLNA_FILE_CONTENT_TYPE_T e_browse_type)
{
#ifdef DLNA_SUPPORT
    return x_fm_dlna_set_content_filter(ps_filename, e_browse_type);
#else
    return DLNAR_DMS_CANNOT_SUPPORT;
#endif
}


INT32 c_fm_dlna_orderby(CHAR * ps_filename, DLNA_ORDERBY_TYPE_T e_orderby, BOOL b_override)
{
#ifdef DLNA_SUPPORT
    return x_fm_dlna_orderby(ps_filename, e_orderby, b_override);
#else
    return DLNAR_DMS_CANNOT_SUPPORT;
#endif
}

INT32 c_dlna_dmp_fm_get_media_info(CHAR *ps_filename, DLNA_MEDIA_INFO_T *pt_info)
{
#ifdef DLNA_SUPPORT
    return x_dlna_dmp_fm_get_media_info(ps_filename, pt_info);
#else
    return DLNAR_DMS_CANNOT_SUPPORT;
#endif
}

BOOL c_fm_dlna_is_folder_sorting_supported(CHAR * ps_filename)
{
#ifdef DLNA_SUPPORT
    return x_fm_dlna_is_folder_sorting_supported(ps_filename);
#else
    return FALSE;
#endif
}


BOOL c_fm_dlna_is_folder_changed(CHAR * ps_dir_name)
{
#ifdef DLNA_SUPPORT
    return x_fm_dlna_is_folder_changed(ps_dir_name);
#else
    return FALSE;
#endif
}

BOOL c_fm_dlna_is_pause_supported(CHAR * ps_media_name)
{
#ifdef DLNA_SUPPORT
    return x_fm_dlna_is_pause_supported(ps_media_name);
#else
    return TRUE;
#endif
}

BOOL c_fm_dlna_is_device_alive(VOID)
{
#ifdef DLNA_SUPPORT
    return x_fm_dlna_is_device_alive();
#else
    return FALSE;
#endif
}

BOOL c_fm_dlna_is_seek_supported(CHAR * ps_media_name)
{
#ifdef DLNA_SUPPORT
    return x_fm_dlna_is_seek_supported(ps_media_name);
#else
    return FALSE;
#endif
}

INT32 c_fm_dlna_parse_display_title(CHAR *ps_encoding_name, CHAR *ps_out_name, UINT32 *pui4_out_len)
{
#ifdef DLNAFS_ENCODE_TITLE 
	return 	x_fm_dlna_parse_display_title(ps_encoding_name,ps_out_name,pui4_out_len);
#else
	return -1;  
#endif
}
