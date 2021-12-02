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
/*! @file c_dlm.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_DLM
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


#ifndef C_DLM_H
#define C_DLM_H

#include "u_dlm.h"

/*------------------------------------------------------------------*/
/*! @brief  c_dlm_reg_hdr_callback
 *              User may replace our default file header, in case they need 
 *              use their API to check file header, hence this API provide user 
 *              to register their check API.
 *  @param [in] fn               User file header check API.
 *  @param [in] ui4_hdr_len   User define file header size.
 *  @return Return the error code.
 *  @retval DLM_OK Register their check API OK.
 */
/*------------------------------------------------------------------*/
INT32     c_dlm_reg_hdr_callback(dlm_hdr_parse_fn fn, UINT32 ui4_hdr_len);

/*------------------------------------------------------------------*/
/*! @brief  c_dlm_reg_append_callback
 *              The codefile from contain many user define data, they may firmware
 *              or other internal data, when donwload process is running it will 
 *              dispath user defined data to each user, hence, user "MUST" provide
 *              their own API to process this data.
 *  @param [in] pc_type       4 bytes tag, user defined, this tag "MUST" same as build code file.
 *  @param [in] ui4_buf_size  Buffer size for this data.
 *  @param [in] fn                User defined callback.
 *  @return Return the error code.
 *  @retval DLM_OK                       If there did not exsit any failed.
 *  @retval DLM_DUPLICATE_TAG     The tag already in use.
 *  @retval DLM_DUPLICATE_API      The API already in use.
 */
/*------------------------------------------------------------------*/
INT32     c_dlm_reg_append_callback(UCHAR *pc_type, UINT32 ui4_buf_size, dlm_append_data_fn fn);

/*------------------------------------------------------------------*/
/*! @brief  c_dlm_reg_append_callback2
 *              The codefile from contain many user define data, they may firmware
 *              or other internal data, when donwload process is running it will 
 *              dispath user defined data to each user, hence, user "MUST" provide
 *              their own API to process this data.
 *  @param [in] pc_type          4 bytes tag, user defined, this tag "MUST" same as build code file.
 *  @param [in] ui4_buf_size     Buffer size for this data.
 *  @param [in] fn                  User defined callback.
 *  @return Return the error code.
 *  @retval DLM_OK                       If there did not exsit any failed.
 *  @retval DLM_DUPLICATE_TAG     The tag already in use.
 *  @retval DLM_DUPLICATE_API      The API already in use.

 */
/*------------------------------------------------------------------*/
INT32 c_dlm_reg_append_callback2(UCHAR *pc_type, UINT32 ui4_buf_size, dlm_append_data_fn2 fn);

/*------------------------------------------------------------------*/
/*! @brief  c_dlm_unreg_append_callback
 *              This API is use to remove installed append data call back. 
 *  @param [in] puac_tag Call back tag.
 *  @return Return the error code.
 *  @retval DLM_OK                               Uninstall ok.
 *  @retval DLM_NO_HELPER_REGISTERED  No such tag exist.
 */
/*------------------------------------------------------------------*/
INT32     c_dlm_unreg_append_callback(UCHAR *puac_tag);

/*------------------------------------------------------------------*/
/*! @brief  c_dlm_register_progress_notify
 *              User may register this api to monitor current tag download or upload progress.
 *  @param [in] fn   User progress notify function.
 *  @return Return the error code.
 *  @retval DLM_OK uninstall ok.
 */
/*------------------------------------------------------------------*/
INT32     c_dlm_register_progress_notify(dlm_progress_notify_fn fn);

/*------------------------------------------------------------------*/
/*! @brief  c_dlm_register_tag_progress_notify
 *              User may register this api to monitor current tag download or upload progress.
 *  @param [in] fn  user progress notify function.
 *  @return Return the error code.
 *  @retval DLM_OK
 */
/*------------------------------------------------------------------*/

INT32     c_dlm_register_tag_progress_notify(dlm_progress_notify_fn fn);

/*------------------------------------------------------------------*/
/*! @brief  c_dlm_integrity_check
 *              This API will check file integrity as user defined.
 *  @param [in] ps_filename            Filename.
 *  @param [in] pt_user_io              IO function.
 *  @return Return the error code.
 *  @retval DLM_OK check ok,others failed.
 */
/*------------------------------------------------------------------*/
INT32     c_dlm_integrity_check(CHAR *ps_filename, DLM_IO_T *pt_user_io);

/*------------------------------------------------------------------*/
/*! @brief   c_dlm_download
 *               User can use this API to download codefile from USB flash.
 *  @param [in] filename            Code file name.
 *  @param [in] pt_user_io         IO function.
 *  @param [in] i4_check           File Header Check flag.
 *  @return Return the error code.
 *  @retval DLM_OK  download finished, others    download failed.
 */
/*------------------------------------------------------------------*/
INT32     c_dlm_download(CHAR *filename, DLM_IO_T *pt_user_io, INT32 i4_check);
/*------------------------------------------------------------------*/
/*! @brief   c_dlm_upload
 *              Upload device internal data to USB flash for future use.
 *              During upload process, this API make proper header for internal.
 *              data and add digest to header to provide codefile intergrity.
 *              
 *  @param [in] pac_tag          Which data to upload.
 *  @param [in] pt_user_io       User define I/O callback.
 *  @param [in] ps_path          Output file and path. 
 *  @return Return the error code.
 *  @retval  DLM_OK                                      Upload data done.
 *  @retval  DLM_ALLOCATE_MEMORY_FAILED    Allocate memory failed.
 *  @retval  DLM_OPEN_FILE_FAILED                Open file failed.
 *  @retval  DLM_WRITE_DATA_FAILED             Write data failed.
 *  @retval  DLM_NO_HELPER_REGISTERED         No such data exist.
 */
/*------------------------------------------------------------------*/
INT32     c_dlm_upload(CHAR *pac_tag, DLM_IO_T *pt_io, CHAR* ps_path);


/*------------------------------------------------------------------*/
/*! @brief c_dlm_get_download_hdr
 *             User can use this API to get the header of the file.
 *  @return Return header of download file.
 *  @retval Point of the header.
 */
/*------------------------------------------------------------------*/
CHAR     *c_dlm_get_download_hdr(VOID);


/*------------------------------------------------------------------*/
/*! @brief   c_dlm_set_ignore_check_item
 *               This API is use to disable some integrity check item
 *  @param [in] uc_flag   Check flag.
 *  @return Return the error code.
 *  @retval DLM_OK  Check ok.
                others    check failed
 */
/*------------------------------------------------------------------*/
INT32     c_dlm_set_ignore_check_item(UCHAR uc_flag);

/*------------------------------------------------------------------*/
/*! @brief      c_dlm_get_codefile_tag_list
 *                  User can call this api to get the tag list of append data for future process.
 *  @param [in] ps_filepath  Codefile name.
 *  @return Return the append data list.
 *  @retval  Append_T*  If NULL,   codefile doesn't exist any tag,others, Tag list.
 */
/*------------------------------------------------------------------*/
Append_T *c_dlm_get_codefile_tag_list(CHAR *ps_filepath);


/*------------------------------------------------------------------*/
/*! @brief  c_dlm_download_by_tag
 *              This API is use to download specify append data.
 *  @param [in] ps_file         Codefile name.
 *  @param [in] puc_tag      User defined tag.
 *  @param [in] pt_user_io   User specify I/O.
 *  @return Return the error code.
 *  @retval DLM_OK     download ok.
 *             others       download failed.
 */
/*------------------------------------------------------------------*/
INT32 c_dlm_download_by_tag(CHAR *ps_file, UCHAR *puc_tag, DLM_IO_T *pt_user_io);
/*------------------------------------------------------------------*/
/*! @brief  c_dlm_download_by_division
 *             This API is use to download specify append data.
 *  @param [in] ui4_len       Length of puc_buffer.
 *  @param [in] puc_buffer  Append data.
 *  @param [in] puc_tag     User defined tag.
 *  @return Return the error code.
 *  @retval DLM_OK     Download ok.
 *             others       Download failed.
 */
/*------------------------------------------------------------------*/

INT32 c_dlm_download_by_division(UINT32 ui4_len, UCHAR*  puc_buffer, UCHAR* puc_tag);


/*------------------------------------------------------------------*/
/*! @brief  c_dlm_tag_test
 *              This API is use to download specify append data.
 *  @param [in] ps_file           Codefile name.
 *  @param [in] puac_tag       User specify tag name.
 *  @param [in] pac_buffer     The buffer to store downloaded data.
 *  @param [in] i4_buf_size     Pac_buffer size.
 *  @return Return the error code.
 *  @retval DLM_OK     Download OK.
 *  @retval others       Download failed.
 */
/*------------------------------------------------------------------*/
INT32 c_dlm_tag_test(CHAR *ps_file, UCHAR *puac_tag, DLM_IO_T *pt_user_io, CHAR *pac_buffer, INT32 i4_buf_size);

/*------------------------------------------------------------------*/
/*! @brief   c_dlm_reg_version_compare_callback
 *               Register user defined version compare API.
 *  @param [in] p_fn   Compare API.
 *  @retval     VOID
 */
/*------------------------------------------------------------------*/
VOID  c_dlm_reg_version_compare_callback(dlm_ver_chk_fn p_fn);

#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_DLM*/
/*----------------------------------------------------------------------------*/


