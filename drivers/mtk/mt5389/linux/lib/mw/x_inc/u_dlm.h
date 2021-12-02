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
/*! @file u_dlm.h 
 *  $RCSfile: $
 *  $Revision: #2 $
 *  $Date: 2012/09/29 $
 *  $Author: jiangbo.wang $
 *  
 *  @par Description:
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_DLM Download Lib
 *
 *  @ingroup groupMW
 *  @brief Provides constant,type definitions and function of the Download Lib
*   .. Download Lib is a middleware component that is responsible for Download image to system
 *
 *  %MW Application API would be used by TV Application, lot of these APIs 
would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef U_DLM_H
#define U_DLM_H

#ifdef NO_MIDDLEWARE 
#ifdef CC_UBOOT
    #include <linux/string.h>
#else
#ifndef LOADER_IMGHDR_H
#ifndef CC_MTK_LOADER
    #include <stdio.h>
    #include <stdlib.h>
#endif /* CC_MTK_LOADER */
    #include <string.h>
#endif /* LOADER_IMGHDR_H */
#endif /* CC_UBOOT */
#ifdef CC_MTK_LOADER
    #include "u_os.h"
    #define x_dbg_stmt     Printf    /**<        */
#else /* CC_MTK_LOADER */
/* Middleware C library */

    void *malloc(size_t size);
    void free(void *ptr);
    #define x_memcpy       memcpy    /**<Memory copy function.        */
    #define x_memcmp       memcmp    /**<Memory compare function.        */
    #define x_memset       memset    /**<Memory set function.        */
    #define x_mem_alloc    malloc    /**<Memory alloc function.        */
    #define x_mem_free     free    /**<Memory free function.        */
    #define x_dbg_stmt     printf    /**<Printf.        */
    #define x_strcpy       strcpy    /**<String copy function.        */
    #define x_strlen       strlen    /**<String length function.       */
    #define x_strcmp       strcmp    /**<String compare function.        */
    #define x_strcat       strcat    /**<Cascade string function.        */
#ifndef NO_TYPE_DEF
/* Middleware Type define */
    #define UINT32         unsigned long    /**<Middleware UINT32 define.        */
    #define UINT16         unsigned short    /**<Middleware UINT16 define.        */
    #define INT32          signed long    /**<Middleware INT32 define.        */
#endif /* NO_TYPE_DEF */
    #define UCHAR          unsigned char    /**<Middleware UCHAR define.        */
    #define CHAR           signed char    /**<Middleware CHAR define.        */
    #define VOID           void    /**<Middleware VOID define.        */
#endif /* CC_MTK_LOADER */
#else
#include "u_os.h"
#include "u_dbg.h"
#endif

#define X_INTERNAL_BUFFER_SIZE  4096    /**<Minum size is hdr size.        */

#define DEBUG_DLM_TRACE 1    /**<Debug log open.        */

#if DEBUG_DLM_TRACE
    #define DLM_TRACE(x)  {x_dbg_stmt x;}    /**<Debug log.        */
#else
    #define DLM_TRACE(x)    /**<Debug log.         */
#endif

#define kValidateID             (UCHAR) (0x10)    /**<Flag check ID.        */
#define kValidateVersion        (UCHAR) (0x20)    /**<Flag check version.        */
#define kValidateModelName      (UCHAR) (0x40)    /**<Flag model name.        */

#define DLM_ENCRYPT_LENGTH      2048
/*------------------------------------------------------------------*/
/*! @enum DLM_ACCESS_T
 *  @brief This structure enumerates access type of file operation.
 *  @code
 *  typedef enum 
 *  {
 *      DLM_READ = 0,
 *      DLM_READ_INCONSISTENCY,
 *      DLM_WRITE,
 *      DLM_WRITE_INCONSISTENCY
 *  }DLM_ACCESS_T;
 *  @endcode
 *  @li@c  DLM_READ                                          - Read access.
 *  @li@c  DLM_READ_INCONSISTENCY                            - Inconsistency read access.
 *  @li@c  DLM_WRITE                                         - Write access.
 *  @li@c  DLM_WRITE_INCONSISTENCY                           - Inconsistency write access.
 */
/*------------------------------------------------------------------*/
typedef enum 
{
    DLM_READ = 0,
    DLM_READ_INCONSISTENCY,
    DLM_WRITE,
    DLM_WRITE_INCONSISTENCY
}DLM_ACCESS_T;

/*------------------------------------------------------------------*/
/*! @enum DLM_RTN_T
 *  @brief This structure enumerates error code.
 *  @code
 *  typedef enum 
 *  {
 *      DLM_OK = 0,
 *      DLM_ALLOCATE_MEMORY_FAILED = -1,
 *      DLM_DUPLICATE_TAG = -2,
 *      DLM_DUPLICATE_API = -3,
 *      DLM_ID_NOT_MATCH = -4,
 *      DLM_MODEL_NAME_NOT_MATCH = -5,
 *      DLM_DIGEST_NOT_MATCH =  -6,
 *      DLM_VERSION_NOT_MATCH = -7,
 *      DLM_HDR_MAGIC_NOT_MATCH = -8,
 *      DLM_NO_HELPER_REGISTERED = -9,
 *      DLM_PARSE_HELPER_FAILED = -10,
 *      DLM_PARSE_HELPER_IGNORED = -11,
 *      DLM_OPEN_FILE_FAILED = -12,
 *      DLM_DEL_FILE_FAILED = -13,
 *      DLM_RENAME_FILE_FAILED = -14,
 *      DLM_READ_FILE_FAILED = -15,
 *      DLM_WRITE_DATA_FAILED= -16,
 *      DLM_UPLOAD_FAILED = -17,
 *      DLM_UNKONW_TAG = -18,
 *      DLM_OTHERS = -100,
 *      DLM_UI_PERCENTAGE_STATUS
 *  } DLM_RTN_T;
 *  @endcode
 *  @li@c  DLM_OK                                            - Ok.
 *  @li@c  DLM_ALLOCATE_MEMORY_FAILED                        - Out of memory.
 *  @li@c  DLM_DUPLICATE_TAG                                 - Duplicate tag.
 *  @li@c  DLM_DUPLICATE_API                                 - Duplicate API.
 *  @li@c  DLM_ID_NOT_MATCH                                  - ID not match.
 *  @li@c  DLM_MODEL_NAME_NOT_MATCH                          - Model name not match.
 *  @li@c  DLM_DIGEST_NOT_MATCH                              - Digest not match.
 *  @li@c  DLM_VERSION_NOT_MATCH                             - Version not match.
 *  @li@c  DLM_HDR_MAGIC_NOT_MATCH                           - Header magic not match.
 *  @li@c  DLM_NO_HELPER_REGISTERED                          - Not helper registered.
 *  @li@c  DLM_PARSE_HELPER_FAILED                           - Parse helper failed.
 *  @li@c  DLM_PARSE_HELPER_IGNORED                          - Parse helper ignored.
 *  @li@c  DLM_OPEN_FILE_FAILED                              - Open file failed.
 *  @li@c  DLM_DEL_FILE_FAILED                               - Delete file failed.
 *  @li@c  DLM_RENAME_FILE_FAILED                            - Rename file failed.
 *  @li@c  DLM_READ_FILE_FAILED                              - Read file failed.
 *  @li@c  DLM_WRITE_DATA_FAILED                             - Write data failed.
 *  @li@c  DLM_UPLOAD_FAILED                                 - Upload failed.
 *  @li@c  DLM_UNKONW_TAG                                    - Unkown tag.
 *  @li@c  DLM_OTHERS                                        - Others.
 *  @li@c  DLM_UI_PERCENTAGE_STATUS                          - Status of UI percentage.
 */
/*------------------------------------------------------------------*/
typedef enum 
{
    DLM_OK = 0,

    /* memory failed */
    DLM_ALLOCATE_MEMORY_FAILED = -1,

    /* append data callback */
    DLM_DUPLICATE_TAG = -2,
    DLM_DUPLICATE_API = -3,

    /* possible return code from code file header check process */
    DLM_ID_NOT_MATCH = -4,
    DLM_MODEL_NAME_NOT_MATCH = -5,
    DLM_DIGEST_NOT_MATCH =  -6,
    DLM_VERSION_NOT_MATCH = -7,
    DLM_HDR_MAGIC_NOT_MATCH = -8,

    /* call helper to parse each append data */
    DLM_NO_HELPER_REGISTERED = -9,
    DLM_PARSE_HELPER_FAILED = -10,
    DLM_PARSE_HELPER_IGNORED = -11,

    /* file operation failed */
    DLM_OPEN_FILE_FAILED = -12,
    DLM_DEL_FILE_FAILED = -13,
    DLM_RENAME_FILE_FAILED = -14,
    DLM_READ_FILE_FAILED = -15,
    DLM_WRITE_DATA_FAILED= -16,
    DLM_UPLOAD_FAILED = -17,

    DLM_UNKONW_TAG = -18,
    DLM_DIVISION_SIZE_ERROR = -19,
    DLM_ARGV_INVALID = -20,
    DLM_OTHERS = -100,


    /* status report */
    DLM_UI_PERCENTAGE_STATUS
} DLM_RTN_T;

/*------------------------------------------------------------------*/
/*! @brief  header parse function,define by user.
 *  @param [in] s_filename Specified the download file.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*dlm_hdr_parse_fn)           (CHAR *s_filename);
/*------------------------------------------------------------------*/
/*! @brief Append data parse fuction,define by user.
 *  @param [in] e_flag Access of file operation.
 *  @param [in] *pi4_len Length of the buffer.
 *  @param [in][out] pauc_buffer Buffer for store append data. alloc by user.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*dlm_append_data_fn)         (DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32 *pi4_len);
/*------------------------------------------------------------------*/
/*! @brief Append data parse fuction2,define by user.
 *  @param [in] s_tag_name Specified the tag name for parse.
 *  @param [in] e_flag Access of file operation.
 *  @param [in] *pi4_len Length of the buffer.
 *  @param [in][out] pauc_buffer Buffer for store append data. alloc by user.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*dlm_append_data_fn2)         (UCHAR* s_tag_name, DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32 *pi4_len);
/*------------------------------------------------------------------*/
/*! @brief Notify function for percent of progress.
 *  @param [out] i4_percentage Percent of progress.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*dlm_progress_notify_fn)     (INT32 i4_percentage);
/*------------------------------------------------------------------*/
/*! @brief Version check funtion,define by user.
 *  @param [in] ps_version Specified the version to be check.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*dlm_ver_chk_fn)             (CHAR *ps_version);

/*------------------------------------------------------------------*/
/*! @struct DLM_APPEND_BLOCK_T
 *  @brief Struct of append data.
 *  @code
 *  typedef struct _DLM_PARSE_BLOCK_S
 *  {
 *      UCHAR                      uc_type[4];
 *      UCHAR                     *pauc_buffer;
 *      UINT32                     ui4_buf_len;
 *      dlm_append_data_fn         pf_callback;
 *      dlm_append_data_fn2		pf_callback2;
 *      struct _DLM_PARSE_BLOCK_S *pt_next;
 *  }DLM_APPEND_BLOCK_T;
 *  @endcode
 *  @li@c  uc_type[4]                                        - Tag of append data.
 *  @li@c  *pauc_buffer                                      - Buffer for append data.
 *  @li@c  ui4_buf_len                                       - Buffer length.
 *  @li@c  pf_callback                                       - Callback function registered from caller.
 *  @li@c  pf_callback2                                      - Callback function2 registered from caller.
 *  @li@c  *pt_next                                          - Point to next append data.
 */
/*------------------------------------------------------------------*/
typedef struct _DLM_PARSE_BLOCK_S
{
    /*UCHAR                      uc_type;*/
    UCHAR                      uc_type[4];
    UCHAR                     *pauc_buffer;
    UINT32                     ui4_buf_len;
    dlm_append_data_fn         pf_callback;
    dlm_append_data_fn2		pf_callback2;
    struct _DLM_PARSE_BLOCK_S *pt_next;
}DLM_APPEND_BLOCK_T;

/*------------------------------------------------------------------*/
/*! @struct Append_DATA_TLV_T
 *  @brief Append data tables.
 *  @code
 *  typedef struct _APPEND_DATA_TLV_S
 *  {
 *      UCHAR  uc_type[4];
 *      UCHAR  uc_privacy[4];
 *      UCHAR  uc_length[4];
 *  } Append_DATA_TLV_T;
 *  @endcode
 *  @li@c  uc_type[4]                                        - Tag of append data.
 *  @li@c  uc_privacy[4]                                     - Control of append data.
 *  @li@c  uc_length[4]                                      - Lendth of append data.
 */
/*------------------------------------------------------------------*/
typedef struct _APPEND_DATA_TLV_S
{
    UCHAR  uc_type[4];
    UCHAR  uc_privacy[4];
    UCHAR  uc_length[4];
#if (defined(DLM_USE_AES_MODE) || ((defined(LINUX_TURNKEY_SOLUTION)) && (!defined(CC_MTK_LOADER))))
    UCHAR  uc_magic[16];
    UCHAR  uc_digest[32];
 #endif
} Append_DATA_TLV_T;

/*------------------------------------------------------------------*/
/*! @struct DEFAULT_CODE_FILE_HDR_T
 *  @brief File header for download.
 *  @code
 *  typedef struct DEFAULT_CODE_FILE_HDR_S
 *  {
 *      UCHAR   uac_id[4];           
 *      CHAR    s_magic[8];          
 *      CHAR    s_version[64];
 *      UINT32  ui4_time;
 *      UINT32  ui4_total_size;
 *      UCHAR   uac_control[4];
 *      UCHAR   s_model[32];
 *      UCHAR   uac_signature[32];
 *  } DEFAULT_CODE_FILE_HDR_T;
 *  @endcode
 *  @li@c  uac_id[4]                                         - Default key to encrypt/decrypt data. 
 *  @li@c  s_magic[8]                                        - Unique field for firmware (quick check item).
 *  @li@c  s_version[64]                                     - Field for firmware version.
 *  @li@c  ui4_time                                          - Build time.
 *  @li@c  ui4_total_size                                    - Filesize.
 *  @li@c  uac_control[4]                                    - Logic control flag.
 *  @li@c  s_model[32]                                       - Module name.
 *  @li@c  uac_signature[32]                                 - File signature.
 */
/*------------------------------------------------------------------*/
typedef struct DEFAULT_CODE_FILE_HDR_S
{
    UCHAR   uac_id[4];           /* default key to encrypt/decrypt data */
    CHAR    s_magic[8];          /* unique field for firmware (quick check item) */
#if (defined(DLM_USE_AES_MODE) || ((defined(LINUX_TURNKEY_SOLUTION)) && (!defined(CC_MTK_LOADER))))
    CHAR    s_version[56];
#else
    CHAR    s_version[64];
#endif
    UINT32  ui4_time;
    UINT32  ui4_total_size;
    UCHAR   uac_control[4];
    UCHAR   s_model[32];
    UCHAR   uac_signature[32];
} DEFAULT_CODE_FILE_HDR_T;

/*------------------------------------------------------------------*/
/*! @struct Append_T
 *  @brief Append data block common header.
 *  @code
 *  typedef struct append_s
 *  {
 *      UCHAR    uc_type[4];
 *      UCHAR    uac_privacy[4];
 *      UINT32   ui4_length;
 *      CHAR    *ps_filename;
 *      struct append_s *pt_next;
 *  } Append_T;
 *  @endcode
 *  @li@c  uc_type[4]                                        - Append tag.
 *  @li@c  uac_privacy[4]                                    - Control of append data.
 *  @li@c  ui4_length                                        - Append data length.
 *  @li@c  *ps_filename                                      - Append filename.
 *  @li@c  *pt_next                                          - Point to next append data.
 */
/*------------------------------------------------------------------*/
typedef struct append_s
{
    UCHAR    uc_type[4];
    UCHAR    uac_privacy[4];
    UINT32   ui4_length;
    CHAR    *ps_filename;

    struct append_s *pt_next;
} Append_T;

/*------------------------------------------------------------------*/
/*! @enum DLM_OPEN_MODE_T
 *  @brief  I/O releate define
 *  @code
 *  typedef enum 
 *  {
 *      kDlm_Read = 0,
 *      kDlm_Write,
 *      kDlm_Append
 *  } DLM_OPEN_MODE_T;
 *  @endcode
 *  @li@c  kDlm_Read                                         - Read access.
 *  @li@c  kDlm_Write                                        - Write access.
 *  @li@c  kDlm_Append                                       - Append access.
 */
/*------------------------------------------------------------------*/
typedef enum 
{
    kDlm_Read = 0,
    kDlm_Write,
    kDlm_Append
} DLM_OPEN_MODE_T;

/*------------------------------------------------------------------*/
/*! @brief I/O open file function.
 *  @param [in] ps_filename Specified the file to be open.
 *  @param [in] e_mode Open mode for the specify file.
 */
/*------------------------------------------------------------------*/
typedef INT32  (*dlm_open_fn)       (CHAR *ps_filename, DLM_OPEN_MODE_T e_mode);
/*------------------------------------------------------------------*/
/*! @brief I/O close file function.
 *  @param [in] i4_fd File handle, point to the file which want to be close.
 */
/*------------------------------------------------------------------*/
typedef INT32  (*dlm_close_fn)      (INT32 i4_fd);
/*------------------------------------------------------------------*/
/*! @brief  I/O read file function.
 *  @param [in] i4_fd File handle, point to the file. 
 *  @param [out] pc_buf Buffer for store data.
 *  @param [in] ui4_buf_size Read count.
 */
/*------------------------------------------------------------------*/
typedef INT32  (*dlm_read_fn)       (INT32 i4_fd, CHAR *pc_buf, UINT32 ui4_buf_size);
/*------------------------------------------------------------------*/
/*! @brief I/O write file function.
 *  @param [in] i4_fd File handle, point to the file .
 *  @param [in] pc_buffer Buffer for store data.
 *  @param [in] ui4_buf_size Size for write.
 */
/*------------------------------------------------------------------*/
typedef INT32  (*dlm_fwrite_fn)     (INT32 i4_fd, CHAR *pc_buffer, UINT32 ui4_buf_size);
/*------------------------------------------------------------------*/
/*! @brief I/O eof function.
 *  @param [in] i4_fd File handle, point to the file.
 */
/*------------------------------------------------------------------*/
typedef INT32  (*dlm_eof_fn)        (INT32 i4_fd);
/*------------------------------------------------------------------*/
/*! @brief Calc file length function.
 *  @param [in] Filename specify the file.
 */
/*------------------------------------------------------------------*/
typedef UINT32 (*dlm_file_size_fn)  (CHAR *filename);
/*------------------------------------------------------------------*/
/*! @brief  I/O seek funtion.
 *  @param [in] i4_fd File handle, point to the file.
 *  @param [in] i4_offset Seek i4_offset count.
 */
/*------------------------------------------------------------------*/
typedef INT32  (*dlm_seek_fn)       (INT32 i4_fd, UINT32 i4_offset);

/*------------------------------------------------------------------*/
/*! @struct DLM_IO_T
 *  @brief  I/O fuction.
 *  @code
 *  typedef struct DLM_IO_S
 *  {
 *      dlm_open_fn         open_source;
 *      dlm_close_fn        close_source;
 *      dlm_read_fn         read_data;
 *      dlm_fwrite_fn       write_data;
 *      dlm_eof_fn          eof_data;
 *      dlm_file_size_fn    data_size;
 *      dlm_seek_fn         seek_source;
 *  }DLM_IO_T;
 *  @endcode
 *  @li@c  open_source                                       - Open file function.
 *  @li@c  close_source                                      - Close file function.
 *  @li@c  read_data                                         - Read file function.
 *  @li@c  write_data                                        - Write data function.
 *  @li@c  eof_data                                          - Eof data function.
 *  @li@c  data_size                                         - Get file size function.
 *  @li@c  seek_source                                       - Seek file function.
 */
/*------------------------------------------------------------------*/
typedef struct DLM_IO_S
{
    dlm_open_fn         open_source;
    dlm_close_fn        close_source;
    dlm_read_fn         read_data;
    dlm_fwrite_fn       write_data;
    dlm_eof_fn          eof_data;
    dlm_file_size_fn    data_size;
    dlm_seek_fn         seek_source;
}DLM_IO_T;

#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_DLM*/
/*----------------------------------------------------------------------------*/

