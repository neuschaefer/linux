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
 * $RCSfile: c_dlm_api.c,v $
 * $Revision: #2 $
 * $Date: 2012/06/01 $
 * $Author: muran.xiang $
 * $CCRevision:  $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *
 * Description:
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/u_common.h"
#include "x_dlm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: c_dlm_reg_hdr_callback
 *
 * Description: User may replace our default file header, in case they need 
 *              use their API to check file header, hence this API provide user 
 *              to register their check API.
 *
 * Inputs:  
 *              fn, user file header check API
 *              ui4_hdr_len, user define file header size
 * Outputs:     
 * Returns:     DLM_OK 
 ----------------------------------------------------------------------------*/
INT32  c_dlm_reg_hdr_callback(dlm_hdr_parse_fn fn, UINT32 ui4_hdr_len)
{
    return x_dlm_reg_hdr_callback(fn, ui4_hdr_len);
}

/*-----------------------------------------------------------------------------
 * Name: c_dlm_reg_append_callback
 *
 * Description: The codefile from contain many user define data, they may firmware
 *              or other internal data, when donwload process is running it will 
 *              dispath user defined data to each user, hence, user "MUST" provide
 *              their own API to process this data.
 *
 * Inputs:  pc_type, 4 bytes tag, user defined, this tag "MUST" same as build code 
 *                   file
 *          ui4_buf_size, buffer size for this data
 *          fn,  user defined callback
 *               
 * Outputs: 
 * Returns: DLM_OK, if there did not exsit any failed
 *          DLM_DUPLICATE_TAG, the tag already in use
 *          DLM_DUPLICATE_API, the API already in use
 ----------------------------------------------------------------------------*/
INT32  c_dlm_reg_append_callback(UCHAR *pc_type, UINT32 ui4_buf_size, dlm_append_data_fn fn)
{
    return x_dlm_reg_append_callback(pc_type, ui4_buf_size, fn);
}

/*-----------------------------------------------------------------------------
 * Name: c_dlm_reg_append_callback
 *
 * Description: The codefile from contain many user define data, they may firmware
 *              or other internal data, when donwload process is running it will 
 *              dispath user defined data to each user, hence, user "MUST" provide
 *              their own API to process this data.
 *
 * Inputs:  pc_type, 4 bytes tag, user defined, this tag "MUST" same as build code 
 *                   file
 *          ui4_buf_size, buffer size for this data
 *          fn,  user defined callback
 *               
 * Outputs: 
 * Returns: DLM_OK, if there did not exsit any failed
 *          DLM_DUPLICATE_TAG, the tag already in use
 *          DLM_DUPLICATE_API, the API already in use
 ----------------------------------------------------------------------------*/

INT32 c_dlm_reg_append_callback2(UCHAR *pc_type, UINT32 ui4_buf_size, dlm_append_data_fn2 fn)
{
    return  x_dlm_reg_append_callback2(pc_type, ui4_buf_size, fn);
}

/*-----------------------------------------------------------------------------
 * Name: c_dlm_integrity_check
 *
 * Description: This API will check file integrity as user defined
 *
 * Inputs:  filename
 * Outputs: no
 * Returns: DLM_OK check ok
 *          others failed
 ----------------------------------------------------------------------------*/
INT32  c_dlm_integrity_check(CHAR *ps_filename, DLM_IO_T *pt_user_io)
{
    return x_dlm_integrity_check(ps_filename, pt_user_io);
}

/*-----------------------------------------------------------------------------
 * Name: c_dlm_download
 *
 * Description: User can use this API to download codefile from USB flash
 *
 * Inputs:  filename, code file name
 * Outputs: NA
 * Returns: DLM_OK, download finished
 ----------------------------------------------------------------------------*/
INT32  c_dlm_download(CHAR *filename, DLM_IO_T *pt_user_io, INT32 i4_check)
{
    return x_dlm_download(filename, pt_user_io, i4_check);
}

/*-----------------------------------------------------------------------------
 * Name: c_dlm_upload
 *
 * Description: Upload device internal data to USB flash for future use.
 *              During upload process, this API make proper header for internal
 *              data and add digest to header to provide codefile intergrity
 *              
 * Inputs:  
 *              pac_tag, which data to upload
 *              pt_user_io, user define I/O callback
 *              ps_path, output file and path  
 * Outputs: No
 * Returns: 
 *          DLM_OK                     : upload data done
 *          DLM_ALLOCATE_MEMORY_FAILED : Allocate memory failed
 *          DLM_OPEN_FILE_FAILED, open file failed
 *          DLM_WRITE_DATA_FAILED, write data failed
 *          DLM_NO_HELPER_REGISTERED, no such data exist
 ----------------------------------------------------------------------------*/
#ifndef CC_LOADER_SHRINK_DLM_API
INT32  c_dlm_upload(CHAR *pac_tag, DLM_IO_T *pt_user_io, CHAR *ps_path)
{
    return x_dlm_upload(pac_tag, pt_user_io, ps_path);
}
#endif
/*-----------------------------------------------------------------------------
 * Name: c_dlm_register_progress_notify
 *
 * Description: user may register this api to monitor current tag download or upload progress
 *
 * Inputs:  
 *              fn, user progress notify function
 * Outputs:     
 * Returns:     DLM_OK 
 ----------------------------------------------------------------------------*/
INT32 c_dlm_register_progress_notify(dlm_progress_notify_fn fn)
{
    return x_dlm_register_progress_notify(fn);
}

CHAR   *c_dlm_get_download_hdr(VOID)
{
    return x_dlm_get_download_hdr();
}

/*-----------------------------------------------------------------------------
 * Name: c_dlm_unreg_append_callback
 *
 * Description: This API is use to remove installed append data call back 
 *
 * Inputs:  call back tag
 * Outputs: NA
 * Returns: DLM_OK, uninstall ok
 *          DLM_NO_HELPER_REGISTERED, no such tag exist
 ----------------------------------------------------------------------------*/
INT32 c_dlm_unreg_append_callback(UCHAR *puac_tag)
{
    return x_dlm_unreg_append_callback(puac_tag);
}

/*-----------------------------------------------------------------------------
 * Name: c_dlm_set_ignore_check_item
 *
 * Description: This API is use to disable some integrity check item
 *
 * Inputs:  check flag
 * Outputs: NA
 * Returns: DLM_OK, 
 ----------------------------------------------------------------------------*/
INT32 c_dlm_set_ignore_check_item(UCHAR uc_flag)
{
    return x_dlm_set_ignore_check_item(uc_flag);
}

Append_T *c_dlm_get_codefile_tag_list(CHAR *ps_filepath)
{
    return x_dlm_get_codefile_tag_list(ps_filepath);
}

/*-----------------------------------------------------------------------------
 * Name: c_dlm_register_tag_progress_notify
 *
 * Description: user may register this api to monitor current tag download or upload progress
 *
 * Inputs:  
 *              fn, user progress notify function
 * Outputs:     
 * Returns:     DLM_OK 
 ----------------------------------------------------------------------------*/
INT32 c_dlm_register_tag_progress_notify(dlm_progress_notify_fn fn)
{
    return x_dlm_register_tag_progress_notify(fn);
}

/*-----------------------------------------------------------------------------
 * Name: x_dlm_download_by_tag
 *
 * Description: This API is use to download specify append data
 *
 * Inputs:  ps_file,  codefile name
 *          puc_tag,   user defined tag
 *          pt_user_io, user specify I/O
 * Outputs: NA
 * Returns: DLM_OK, 
 * 
 * Note: 
 *    Before this function is called, the codefile need pass integrity check
 ----------------------------------------------------------------------------*/
INT32 c_dlm_download_by_tag(CHAR *ps_file, UCHAR *puc_tag, DLM_IO_T *pt_user_io)
{
    return  x_dlm_download_by_tag(ps_file, puc_tag, pt_user_io);
}
/*-----------------------------------------------------------------------------
 * Name: c_dlm_download_by_division
 *
 * Description: This API is use to download specify append data
 *
 * Inputs:  ui4_len,  length of puc_buffer
 *          puc_buffer append data
 *          puc_tag user defined tag
 * Outputs: NA
 * Returns: DLM_OK, 
 * 
 * Note: 
 *  
 ----------------------------------------------------------------------------*/

INT32 c_dlm_download_by_division(UINT32 ui4_len, UCHAR*  puc_buffer, UCHAR* puc_tag)
{
    return x_dlm_download_by_division(ui4_len, puc_buffer, puc_tag);
}

/*-----------------------------------------------------------------------------
 * Name: x_dlm_tag_test
 *
 * Description: This API is use to download specify append data
 *
 * Inputs:  ps_file,  codefile name
 *          puac_tag, user specify tag name
 *          pac_buffer, the buffer to store downloaded data
 *          i4_buf_size, pac_buffer size
 * Outputs: NA
 * Returns: DLM_OK, 
 * 
 * Note, 
 *      Due to crypher tool limit, the i4_buf_size must to be divisible by 4
 ----------------------------------------------------------------------------*/
INT32 c_dlm_tag_test(CHAR *ps_file, UCHAR *puac_tag, DLM_IO_T *pt_user_io, CHAR *pac_buffer, INT32 i4_buf_size)
{
    return x_dlm_tag_test(ps_file, puac_tag, pt_user_io, pac_buffer, i4_buf_size);
}

/**
 * c_dlm_reg_version_compare_callback
 * @param p_fn
 * 
 * @return VOID
 */
VOID c_dlm_reg_version_compare_callback(dlm_ver_chk_fn p_fn)
{
    x_dlm_reg_version_compare_callback(p_fn);
}
