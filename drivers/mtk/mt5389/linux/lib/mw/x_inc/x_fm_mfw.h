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
/*----------------------------------------------------------------------------*

 * SWAuthor: Chun Zhou 
 *
 * Description: 
 *         This header file contains Multi-File-Wrapper exported functions.
 *---------------------------------------------------------------------------*/

#ifndef _X_FM_MFW_H_
#define _X_FM_MFW_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_fm.h"
#include "u_fm_mfw.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define IN
#define OUT
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_init
 *
 * Description: Init Multi File Wrapper
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_init(VOID);


/*-----------------------------------------------------------------------------
  * Name: x_fm_mfw_analyse
  *
  * Description: Analyse the device to get the available space and the continuous
  *              space. This is a asynchronous a function, and wrapper will inform
  *              the caller of the analysing progress. When the analysing is done,
  *              inform caller of the result. After that, wrapper will free the handle
  *              itself.
  * Input:  pt_param
  *             Parameters for analysing.
  *
  * Output: h_req
  *             Handle of the analysing request.
  *
  * Return: FMR_OK
  *                 Success!
  *         FMR_ARG
  *                 pt_parm is NULL or ph_req is NULL or pt_parm has invalid member
  *                 varialbes.
  *         FMR_CORE
  *                 Internal error.
  *
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_analyse(
    IN  const FM_MFW_ANLS_PARAM_T* pt_param,
    OUT HANDLE_T*                  ph_req);

/*-----------------------------------------------------------------------------
  * Name: x_fm_mfw_create
  *
  * Description: Create a wrapper file according to the configs. This is a asynchronous
  *              a function, and it will analyse the device to get the available space 
  *              and the continuous space first, and then do the creating things. Wrapper
  *              will inform the caller of the analysing progress in phase1 and creating
  *              progress in phase2. When the creating is done, inform caller of the 
  *              result. After that, wrapper will free the handle itself.
  * 
  * Input:  pt_cfg
  *             Configs for creating.
  *
  * Output: h_req
  *             Handle of the creating request.
  *
  * Return: FMR_OK
  *                 Success!
  *         FMR_ARG
  *                 pt_cfg is NULL or ph_req is NULL or pt_cfg has invalid member
  *                 varialbes.
  *         FMR_CORE
  *                 Internal error.
  *
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_create(
    IN  const FM_MFW_VF_CFG_T*     pt_cfg,
    OUT HANDLE_T*                  ph_req);


/*-----------------------------------------------------------------------------
  * Name: x_fm_mfw_delete
  *
  * Description: Delete a wrapper file.
  * 
  * Input: h_dir
  *             Handle of a directory label.
  *        ps_path
  *             Path name of the wrapper file to be deleted.
  *
  * Output: -
  *
  * Return: FMR_OK
  *                 Success!
  *         FMR_ARG
  *                 ps_path is NULL.
  *         FMR_CORE
  *                 Internal error.
  *
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_delete(
    IN  HANDLE_T                   h_dir,
    OUT const CHAR*                ps_path);


/*-----------------------------------------------------------------------------
  * Name: x_fm_mfw_check
  *
  * Description: Check if the file is a wrapper file. If it is a wrapper file, 
  *              then check if all sub files in the wrapper are OK.
  * 
  * Input:  h_dir
  *             Handle of a directory label.
  *         ps_path
  *             Path name of the wrapper file to be deleted.
  *
  * Output: pb_mfw
  *             A wrapper file or not.
  *         pb_ok
  *             All sub files in the wrapper are OK or not.
  *
  * Return: FMR_OK
  *                 Success!
  *         FMR_ARG
  *                 ps_path is NULL.
  *         FMR_CORE
  *                 Internal error.
  *
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_check(
    IN  HANDLE_T                   h_dir,
    IN  const CHAR*                ps_path,
    OUT BOOL*                      pb_mfw,
    OUT BOOL*                      pb_ok);

/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_open
 *
 * Description: Create a multi-file wrapper, and return a mfw handle.
 *
 * Inputs:  ps_path
 *              Head file path.
 *
 * Outputs: ph_file
 *              Handle of the created multi-file wrapper
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              a_ps_path is NULL or any element in the array is NULL or
 *              ui1_ent_num is 0 or ph_file is NULL.
 *          FMR_NO_ENTRY
 *              Any element file in the array is not found.
 *          FMR_IS_DIR
 *              Any element file in the array is a directory.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_open (
    IN  HANDLE_T                   h_dir,
    IN  const CHAR*                ps_path,
    IN  UINT32                     ui4_flags,
    IN  UINT32                     ui4_mode,
    IN  BOOL                       b_ext_buf,
    OUT HANDLE_T*                  ph_file); 

/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_close
 *
 * Description: Delete a multi-file wrapper.
 *
 * Inputs:  h_file
 *              Handle of the created multi-file wrapper
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_HANDLE
 *              h_file is invalid.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_close(
    IN  HANDLE_T    h_file);


/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_lseek
 *
 * Description: Change the current postion of the opened multi-file wrapper.
 *
 * Inputs:  h_file
 *              Handle of an opened multi-file wrapper.
 *          i8_offset
 *              Offset from ui4_whence in bytes. Positive number means move
 *              current position backward while negative number means move
 *              forward.
 *          ui1_whence
 *              FM_SEEK_BGN tells File Manager to move current position to
 *              i8_offset from the begging of the wrapper. if i8_offset is
 *              negative, current position is set to 0, that is the begging
 *              of wrapper.
 *              FM_SEEK_CUR tells File Manager to move current position to
 *              i8_offset from current position.
 *              FM_SEEK_END tells File Manager to move current position to
 *              i8_offset from the end of the wrapper. If i8_offset is a
 *              positive number, current position is set to the size of
 *              the file, that is the end of wrapper.
 *
 * Outputs: pui8_cur_pos
 *              Current position.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pui8_cur_pos is NULL.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              h_file is being closed or ui1_whence is not defined.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_lseek(
    IN  HANDLE_T        h_file,
    IN  INT64           i8_offset,
    IN  UINT8           ui1_whence,
    OUT UINT64*         pui8_cur_pos);
    
/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_read
 *
 * Description: Read data from wrapper to buffer.
 *
 * Inputs:  h_file
 *              Handle of an opened wrapper.
 *          pv_data
 *              Buffer to hold read data.
 *          ui4_count
 *              Size of pv_data in bytes.
 *
 * Outputs: pui4_read
 *              Actual read bytes.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL or pui4_read is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a multi-file wrapper or it is being
 *              closed.
 *          FMR_EOF
 *              Reaches EOF.
 *          FMR_DEVICE_ERROR
 *              Device read error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_read(
    IN  HANDLE_T        h_file,
    IN  VOID*           pv_data,
    IN  UINT32          ui4_count,
    OUT UINT32*         pui4_read);

/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_read_async
 *
 * Description: Asynchronously read data from the opened wrapper.
 *
 * Inputs:  h_file
 *              Handle of the opened wrapper.
 *          pv_data
 *              Reference to a buffer to hold the read data.
 *          ui4_count
 *              Number of bytes to read.
 *          ui1_pri
 *              I/O priority.
 *          pf_nfy_fct
 *              Callback function while read is done.
 *          pv_tag
 *              Tag value, which will be returned in the callback.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a multi-file wrapper or it is being
 *              closed, driver not ready, etc.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_EOF
 *              Reaches end of file.
 *          FMR_ALIGNMENT
 *              pv_data alignment error.
 *          FMR_DEVICE_ERROR
 *              Device read error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_read_async(
    IN  HANDLE_T        h_file,
    IN  VOID            *pv_data,
    IN  UINT32          ui4_count,
    IN  UINT8           ui1_pri,
    IN  x_fm_async_fct  pf_nfy_fct,
    IN  VOID*           pv_tag,
    OUT HANDLE_T*       ph_req);

/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_write
 *
 * Description: Write data from buffer to wrapper.
 *
 * Inputs:  h_file
 *              Handle of an opened wrapper.
 *          pv_data
 *              Buffer to write.
 *          ui4_count
 *              Number of bytes to write.
 *
 * Outputs: pui4_written
 *              Actual written bytes.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL or pui4_written is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a multi-file wrapper or it is being
 *              closed.
 *          FMR_EOF
 *              Reaches EOF.
 *          FMR_DEVICE_ERROR
 *              Device write error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_write(
    IN  HANDLE_T        h_file,
    IN  const VOID*     pv_data,
    IN  UINT32          ui4_count,
    OUT UINT32*         pui4_written);

/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_write_async
 *
 * Description: Asynchronously write data to the opened wrapper.
 *              In nucleus, async write is done with another file handle.
 *              So sub file position can not be changed. Because of this 
 *              limitation, before calling x_fm_mfw_write_async(), user should
 *              call x_fm_mfw_lseek(h_file,0,FM_SEEK_CUR, &pos) to set correct
 *              position of the sub file.
 *
 * Inputs:  h_file
 *              Handle of the opened wrapper.
 *          pv_data
 *              Buffer to write.
 *          ui4_count
 *              Number of bytes to write.
 *          ui1_pri
 *              I/O priority.
 *          pf_nfy_fct
 *              Callback function while write is done.
 *          pv_tag
 *              Tag value, which will be returned in the callback.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a multi-file wrapper or it is being
 *              closed, driver not ready, etc.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_EOF
 *              Reaches end of file.
 *          FMR_ALIGNMENT
 *              pv_data alignment error.
 *          FMR_DEVICE_ERROR
 *              Device write error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_write_async(
    IN  HANDLE_T        h_file,
    IN  const VOID*     pv_data,
    IN  UINT32          ui4_count,
    IN  UINT8           ui1_pri,
    IN  x_fm_async_fct  pf_nfy_fct,
    IN  VOID*           pv_tag,
    OUT HANDLE_T*       ph_req);
    
/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_feof
 *
 * Description: Tell if current position reaches the end of wrapper.
 *
 * Inputs:  h_file
 *              Handle of an opened wrapper.
 *
 * Outputs: pb_eof
 *              TRUE if it is at EOF, otherwise FALSE is returned.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pb_eof is NULL.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              file is beging closed.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_feof(
    IN  HANDLE_T        h_file,
    OUT BOOL            *pb_eof);

/*-----------------------------------------------------------------------------
  * Name: x_fm_mfw_abort_async
  *
  * Description: Abort async-ed request
  *
  * Input: h_req
  *             Handle of the async-ed request.
  *
  * Returns:FMR_OK
  *             Success!
  *         FMR_INVAL
  *             No such request in queue.
  *         FMR_CANCELING
  *             Caller will get a e_cond in callback function.
  *
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_abort_async(
    IN  HANDLE_T h_req);


/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_get_info_by_name
 *
 * Description: Get information about a wrapper.
 *
 * Inputs:  h_dir
 *              Handle of a directory where the ps_path will be created.
 *              FM_ROOT_HANDLE can be used to indicate the root directory.
 *              NULL_HANDLE is equivalent to FM_ROOT_HANDLE.
 *          ps_path
 *              Path name of the entry to be created. It can be a single
 *              file name or a path. Note that if the first character of
 *              ps_path is '/', then h_dir will be ignored and ps_path is
 *              regarded as absolute path. This argument cannot be NULL.
 *
 * Outputs: pt_info
 *              Reference to the returned information about the wrapper.  
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path or pt_info is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_get_info_by_name(
    HANDLE_T        h_dir,
    const CHAR*     ps_path,
    FM_FILE_INFO_T* pt_info);


/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_get_info_by_handle
 *
 * Description: Get information about an opened wrapper.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *
 * Outputs: pt_info
 *              Reference to the returned information about the wrapper.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              h_file is NULL_HANDLE or pt_info is NULL.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_get_info_by_handle(
    HANDLE_T        h_file,
    FM_FILE_INFO_T* pt_info);


/*-----------------------------------------------------------------------------
 * Name: x_fm_mfw_get_fs_info
 *
 * Description: Get a mounted file system information.
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *          ps_path
 *              A file/directory path name in the file system.
 *
 * Outputs: pt_fs_info
 *              Reference to file system information object.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path or pt_fs_info is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_NO_ENTRY
 *              The directory component in ps_path or ps_path does not exist.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NOT_DIR
 *              The directory component in ps_path is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mfw_get_fs_info(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_FS_INFO_T    *pt_fs_info);



#endif /* _X_FM_MFW_H */

