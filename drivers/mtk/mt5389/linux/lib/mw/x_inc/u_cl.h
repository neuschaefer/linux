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
/*! @file u_cl.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file exports all compression library specific definitions and structures.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_CL  Compress Lib
 *
 *  @ingroup groupMW
 *  @brief Provides constant,type definitions and function of the Compress Lib. Compress Lib is a middleware component that is responsible for Compress and Uncompress.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_CL_H_
#define _U_CL_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @enum CL_NFY_EVENT_TYPE_T
 *  @brief Notification type for "open" callback function,such as "open engine",
              "open archive" and "open entry". 
 *  @code
 *  typedef enum
 *  {
 *      CL_NFY_HANDLE_CLOSED_TYPE
 *  } CL_NFY_EVENT_TYPE_T;
 *  @endcode
 *  @li@c  CL_NFY_HANDLE_CLOSED_TYPE                         - Call back type: handle closed.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CL_NFY_HANDLE_CLOSED_TYPE
} CL_NFY_EVENT_TYPE_T;
/*------------------------------------------------------------------*/
/*! @enum CL_QUERY_INFO_TYPE_T
 *  @brief Type for query information of a archive. 
 *  @code
 *  typedef enum
 *  {
 *      CL_QUERY_GLOBAL_COMMENT_TYPE,
 *      CL_QUERY_ENTRY_INFO_TYPE,
 *      CL_QUERY_ENTRY_NAME_TYPE,
 *      CL_QUERY_ENTRY_COMMENT_TYPE
 *  }   CL_QUERY_INFO_TYPE_T;
 *  @endcode
 *  @li@c  CL_QUERY_GLOBAL_COMMENT_TYPE                      - Query global commet information.
 *  @li@c  CL_QUERY_ENTRY_INFO_TYPE                          - Query entry information.
 *  @li@c  CL_QUERY_ENTRY_NAME_TYPE                          - Query entry name.
 *  @li@c  CL_QUERY_ENTRY_COMMENT_TYPE                       - Query entry comment information.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CL_QUERY_GLOBAL_COMMENT_TYPE,
    CL_QUERY_ENTRY_INFO_TYPE,
    CL_QUERY_ENTRY_NAME_TYPE,
    CL_QUERY_ENTRY_COMMENT_TYPE
}   CL_QUERY_INFO_TYPE_T;

#ifdef CL_SUPPORT_ARCHIVE
/*------------------------------------------------------------------*/
/*! @struct CL_DATE_TIME_T
 *  @brief  Date/time Infomation.
 *  @code
 *  typedef struct _CL_DATE_TIME_T{
 *      UINT16 ui2_sec;            
 *      UINT16 ui2_min;            
 *      UINT16 ui2_hour;           
 *      UINT16 ui2_mday;           
 *      UINT16 ui2_mon;            
 *      UINT16 ui2_year;           
 *  } CL_DATE_TIME_T;
 *  @endcode
 *  @li@c  ui2_sec                                           - Seconds after the minute - [0,59].
 *  @li@c  ui2_min                                           - Minutes after the hour - [0,59].
 *  @li@c  ui2_hour                                          - Hours since midnight - [0,23].
 *  @li@c  ui2_mday                                          - Day of the month - [1,31].
 *  @li@c  ui2_mon                                           - Months since January - [0,11].
 *  @li@c  ui2_year                                          - Years - [1980..2044].
 */
/*------------------------------------------------------------------*/
typedef struct _CL_DATE_TIME_T{
    UINT16 ui2_sec;            /* seconds after the minute - [0,59] */
    UINT16 ui2_min;            /* minutes after the hour - [0,59] */
    UINT16 ui2_hour;           /* hours since midnight - [0,23] */
    UINT16 ui2_mday;           /* day of the month - [1,31] */
    UINT16 ui2_mon;            /* months since January - [0,11] */
    UINT16 ui2_year;           /* years - [1980..2044] */
} CL_DATE_TIME_T;

/*------------------------------------------------------------------*/
/*! @struct CL_ENTRY_INFO_T
 *  @brief Entry information.
 *  @code
 *  typedef struct _CL_ENTRY_INFO_T
 *  {
 *      CL_DATE_TIME_T  t_date_time;
 *      UINT32          ui4_compressed_size;      
 *      UINT32          ui4_decompressed_size;    
 *      UINT16          ui2_compression_method;   
 *  } CL_ENTRY_INFO_T;
 *  @endcode
 *  @li@c  t_date_time                                       - Compressed Data\Time.
 *  @li@c  ui4_compressed_size                               - Compressed size                 4 bytes. 
 *  @li@c  ui4_decompressed_size                             - Uncompressed size               4 bytes.
 *  @li@c  ui2_compression_method                            -Compression method              2 bytes. 
 */
/*------------------------------------------------------------------*/
typedef struct _CL_ENTRY_INFO_T
{
    CL_DATE_TIME_T  t_date_time;
    UINT32          ui4_compressed_size;      /* compressed size  4 bytes */
    UINT32          ui4_decompressed_size;    /* uncompressed size  4 bytes */
    UINT16          ui2_compression_method;   /* compression method  2 bytes */ 
} CL_ENTRY_INFO_T;
#endif
#define CL_NULL_PARAM_TYPE    0x0    /**< Null param.       */

#ifdef CL_SUPPORT_ARCHIVE
#define CL_READ_MODE ((UINT8) 0x01)    /**< Read mode.       */
#define CL_WRITE_MODE ((UINT8) 0x02)    /**< Write mode.       */
#define CL_CREATE_MODE ((UINT8) 0x04)    /**< Create mode.       */
#define CL_APPEND_MODE ((UINT8) 0x08)    /**< Append mode.       */
#endif
/************************************************************************/
/* Error code                                                           */
/************************************************************************/
#define CLR_OK                        ((INT32)    0)    /**< Ok.       */
#define CLR_INSUFFICIENT_BUFFER       ((INT32)   -1)    /**< Insuffucuent buffer.       */
#define CLR_COMPRESS_ERROR            ((INT32)   -2)    /**< Compress error.       */
#define CLR_DECOMPRESS_ERROR          ((INT32)   -3)    /**< Decompress error.       */
#define CLR_FILE_OR_DIR_MISSING       ((INT32)   -4)    /**< File or directory missing.       */
#define CLR_INV_ARG                   ((INT32)   -5)    /**< Argument invalid.       */
#define CLR_BAD_ARCHIVE               ((INT32)   -6)    /**< Bad archive.       */
#define CLR_INV_HANDLE                ((INT32)   -7)    /**< Handle invalid.       */
#define CLR_OUT_OF_HANDLE             ((INT32)   -8)    /**< Out of handle.       */
#define CLR_ENG_NOT_FOUND             ((INT32)   -9)    /**< Not support this compress type.       */
#define CLR_OUT_OF_MEMORY             ((INT32)   -10)    /**< Out of memory.       */
#define CLR_FCT_NOT_SUPPORT           ((INT32)   -11)    /**< Function not support.       */
#define CLR_NAME_EXIST                ((INT32)   -12)    /**< File name is already exist.       */
#define CLR_CLI_ERROR                 ((INT32)   -13)    /**< Cli cmd error.       */
#define CLR_AUTOFREE_ERROR            ((INT32)   -14)    /**< Free resource error.       */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#endif /* _U_CL_H_ */


/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_CL*/
/*----------------------------------------------------------------------------*/
