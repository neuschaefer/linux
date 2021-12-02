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
/*! @file u_txt.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file exports all txt library specific definitions and structures.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_TXT Txt Browser
 *
 *  @ingroup groupMW
 *  @brief Provides constant,type definitions and function of the  Txt Lib
 *   ..  Txt Lib is a middleware component that is responsible for Txt decode
 *
 *  %MW Application API would be used by TV Application, lot of these APIs 
 *  would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_TXT_H_
#define _U_TXT_H_

#include "u_handle.h"
#include "u_lnk_list.h"

#define HT_TXT_TYPE         (HT_GROUP_IMG + 32)    /**<Txt handle        */

#define TXT_TYPE_DATA_NONE      ((UINT32)   0x00)    /**<  data none      */
#define TXT_TYPE_DATA_TAG       ((UINT32)   0x01)    /**<  tag data     */


typedef enum
{
    TXT_ASYNC_COND_SCANING = 0x0,
    TXT_ASYNC_COND_SCAN_FINISH,
    TXT_ASYNC_COND_SCAN_ABORT,
    TXT_ASYNC_COND_GET_DATA,
    TXT_ASYNC_COND_GET_DATA_FAIL,
    TXT_ASYNC_COND_MAX
} TXT_ASYNC_COND_T;

typedef VOID (*c_txt_nfy_fct) 
(                           HANDLE_T                h_handle,  /* (I) Specifies which handle will be notified. */
                            const VOID*             pv_tag,    /* (I) The client data specified by the client when callback has been called, the callback function will send it again.. */
                            TXT_ASYNC_COND_T        e_event,   /* (I) it identifies the event type of notification. */
                            const VOID*             pv_data
 );  /* (I) specify the notification data which is followed with the notification event. */


/*------------------------------------------------------------------*/
/*! @enum TXT_INTRA_TYPE_T
 *  @brief This structure enumerates all intra-Txt types.
 *  @code
 *  typedef enum
 *  {
 *      INTRA_TXT_THUMBNAIL_TYPE,
 *      INTRA_TXT_PRIMARY_TXT_TYPE
 *  } TXT_INTRA_TYPE_T;
 *  @endcode
 *  @li@c  INTRA_TXT_THUMBNAIL_TYPE                          - thumbnail type
 *  @li@c  INTRA_TXT_PRIMARY_TXT_TYPE                        - primary type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    INTRA_TXT_THUMBNAIL_TYPE,
    INTRA_TXT_PRIMARY_TYPE
} TXT_INTRA_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum TXT_STOP_OP_TYPE_T
 *  @brief This structure enumerates all stop operation types.
 *  @code
 *  typedef enum
 *  {
 *      TXT_STOP_PRIMARY_OP_TYPE,
 *      TXT_STOP_THUMBNAIL_OP_TYPE
 *  } TXT_STOP_OP_TYPE_T;
 *  @endcode
 *  @li@c  TXT_STOP_PRIMARY_OP_TYPE                          - stop thumbnail type
 *  @li@c  TXT_STOP_THUMBNAIL_OP_TYPE                        - stop primary type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    TXT_STOP_PRIMARY_OP_TYPE,
    TXT_STOP_THUMBNAIL_OP_TYPE
} TXT_STOP_OP_TYPE_T;

typedef enum
{
    TXT_STAT_INITIAL, 
    TXT_STAT_SCANING, 
    TXT_STAT_SCANED, 
    TXT_STAT_SCAN_ABORT,
    TXT_STAT_STOP,
    TXT_STAT_GET_DATA
} TXT_SCAN_STATUS_T;

typedef enum _TXT_CODE_TYPE_T
{
    TXT_ECODE_NONE = 0x0,                  // Txt code type
    /* 1. World  coding */
    TXT_ECODE_UTF8,
    TXT_ECODE_UTF8BE,
    TXT_ECODE_UNICODE,                     // UTF16
    TXT_ECODE_UNICODEBE,
    TXT_ECODE_UTF32,
    TXT_ECODE_UTF32BE,
    /* 2. Basic coding */
    TXT_ECODE_ASCII    = 0x7,              // USA character
    /* 3.1 Local coding 1 or 2 byte */
    TXT_ECODE_GB,                          // GB2312 simple chinese
    TXT_ECODE_BIG5,                        // BIG5 tradinal chinese
    TXT_ECODE_KSC,                         // Korean
    TXT_ECODE_JIS,                         // Japanese
    /* 3.2 Local coding 1 byte */
    TXT_ECODE_PL,                          // CP1250
    TXT_ECODE_RU,                          // CP1251
    TXT_ECODE_EN,                          // CP1252 ASCII[0X00-0XFF]
    TXT_ECODE_EL,                          // CP1253
    TXT_ECODE_TR,                          // CP1254
    TXT_ECODE_AR,                          // CP1256
    TXT_ECODE_HEBR,                        // CP1255
    TXT_ECODE_MAX
}TXT_ECODE_TYPE_T;


typedef enum _TXT_SET_ATTR_TYPE_T
{
    /*   none */
    TXT_SET_NONE= 0x0,   
    /*  1. text code */
    TXT_SET_ENCODE,                        
    /* 2.  text font */
    TXT_SET_WGL_HANDLE,
    /* 3. 1 is render base on line, default is base on page*/
    TXT_SET_RENDER_TYPE,
    /* 4. Set Scan Font Info*/
    TXT_SET_SCAN_FONT_INFO,
    /*  invalid  */
    TXT_SET_MAX
} TXT_SET_ATTR_TYPE_T;


typedef struct _TXT_SET_CMD_T
{
    TXT_SET_ATTR_TYPE_T e_set_attr_type;
    VOID* pv_param;
} TXT_SET_CMD_T;




/*------------------------------------------------------------------*/
/*! @enum TXT_SRC_TYPE_T
 *  @brief This structure enumerates all Txt source types.
 *  @code
 *  typedef enum
 *  {
 *      TXT_FILE_SRC_TYPE,
 *      TXT_MEMORY_SRC_TYPE,
 *      TXT_DLNA_SRC_TYPE
 *  } TXT_SRC_TYPE_T;
 *  @endcode
 *  @li@c  TXT_FILE_SRC_TYPE                                 - txt from file
 *  @li@c  TXT_MEMORY_SRC_TYPE                               - txt from memory
 *  @li@c  TXT_DLNA_SRC_TYPE                                 - txt from dlna
 */
/*------------------------------------------------------------------*/
typedef enum
{
    TXT_FILE_SRC_TYPE,
    TXT_MEMORY_SRC_TYPE,
    TXT_DLNA_SRC_TYPE
} TXT_SRC_TYPE_T;

#define MAX_PAGE_LINE 50

typedef struct _TXT_LINE_INFO_T
{
    DLIST_ENTRY_T(_TXT_LINE_INFO_T) t_line_lnk;
    UINT32                          ui4_offset_to_page_start;
    UINT16                          ui2_length;
    UINT32                          ui4_line_idx;
    #if 0
    UINT32                          ui4_logic_idx;
    BOOL                            b_logic_line;
    #endif
}TXT_LINE_INFO_T;

typedef struct _TXT_PAGE_INFO_T
{ 
    DLIST_ENTRY_T(_TXT_PAGE_INFO_T) t_page_lnk;
    DLIST_T(_TXT_LINE_INFO_T)        t_line_list;
    UINT32         ui4_offset_to_file_start;
    UINT16         ui2_page_lenth;
    UINT16         ui2_page_idx;
    UINT16         ui2_line_num;   
}TXT_PAGE_INFO_T;

typedef struct _TXT_DISPLAY_LINE_INFO_T
{
    DLIST_ENTRY_T(_TXT_DISPLAY_LINE_INFO_T) t_dis_line_lnk;
    UTF16_T*       w2s_string_start;
    UINT16         ui2_length;
    UINT32         ui4_line_idx;
    #if 0
    UINT32         ui4_logic_idx;
    BOOL           b_logic_line;
    #endif
}
TXT_DISPLAY_LINE_INFO_T;

typedef struct _TXT_DISPLAY_PAGE_INFO_T
{
    DLIST_ENTRY_T(_TXT_DISPLAY_PAGE_INFO_T) t_dis_page_lnk;
    DLIST_T(_TXT_DISPLAY_LINE_INFO_T) t_dis_line_list;
    UTF16_T*                w2s_string;
    UINT16                  ui2_page_idx;
    UINT16                  ui2_line_num;
}
TXT_DISPLAY_PAGE_INFO_T;

typedef struct
{
    INT32               i4_ret;
    VOID                *pv_tag;
} TXT_NFY_LOAD_WITH_TAG_T;



#define TXT_READ_FILE_MODE      ((UINT8) 0x01)    /**<  read file mode      */
#define TXT_RW_FILE_MODE        ((UINT8) 0x02)    /**<  read and write mode      */





/************************************************************************/
/* Error code                                                           */
/************************************************************************/
#define TXTR_MAX_SUPPORT            ((INT32)     4)    /**< Txt MAX Support       */
#define TXTR_NOT_ENOUGH_DATA        ((INT32)     3)    /**< Txt no data       */
#define TXTR_ALREADY_STOP           ((INT32)     2)    /**< Txt stop       */
#define TXTR_NO_EXIF_DATA           ((INT32)     1)    /**< no exif information       */
#define TXTR_OK                     ((INT32)     0)    /**< Txt ok       */
#define TXTR_MSG_Q_ERR              ((INT32)    -1)    /**< insufficient buffer       */
#define TXTR_FILE_OR_DIR_MISSING    ((INT32)    -2)    /**< file or directory is missing       */
#define TXTR_INV_HANDLE             ((INT32)    -3)    /**< invalid handle       */
#define TXTR_OUT_OF_HANDLE          ((INT32)    -4)    /**< out of handle       */
#define TXTR_OUT_OF_MEMORY          ((INT32)    -5)    /**< out of memory       */
#define TXTR_FCT_NOT_SUPPORT        ((INT32)    -6)    /**< function is not support       */
#define TXTR_DATA_NOT_INIT          ((INT32)    -7)    /**< name exist       */
#define TXTR_INV_ARG                ((INT32)    -8)    /**< invalid argument       */
#define TXTR_CMD_INV                ((INT32)    -9)    /**< Txt engine not found      */
#define TXTR_TXT_NOT_SUPPORT        ((INT32)   -10)    /**< Txt type not support       */
#define TXTR_INTERNAL_ERR           ((INT32)   -11)    /**< internal error       */
#define TXTR_REENTRY_LOADING        ((INT32)   -12)    /**< retry loading       */
#define TXTR_ABORT_SCAN             ((INT32)   -13)    /**< abort loading       */
#define TXTR_SCAN_FAIL              ((INT32)   -14)    /**< decode fail       */
#define TXTR_INITIAL_ERR            ((INT32)   -15)    /**< inital error       */
#define TXTR_FONT_NOT_INIT          ((INT32)   -16)    /**< message queue error       */
#define TXTR_FILE_ERR               ((INT32)   -17)    /**< file opration error       */
#define TXTR_CLI_ERROR              ((INT32)   -18)    /**< cli error       */
#define TXTR_AUTOFREE_ERROR         ((INT32)   -19)    /**< free resource error      */
#define TXTR_IO_ERROR               ((INT32)   -20)    /**< IO error       */
#define TXTR_DISPLAY_AREA_NOT_INIT  ((INT32)   -21)    /**< driver busy       */
#define TXTR_TXT_CRASHED            ((INT32)   -22)    /**< Txt crashed       */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#endif /* _U_TXT_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_TXT*/
/*----------------------------------------------------------------------------*/

