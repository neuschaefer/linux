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
/*! @file u_dsmcc_oc.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains DSM-CC OC specific definitions, which
 *         are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_TBL_MNGR_DSMCC_OC DSM-CC Object Carousel
 *  @ingroup groupMW_TBL_MNGR
 *  @brief Provides API to retrieve data from DSM-CC Object Carousel
 *  %This module allows an application to retrieve data from DSM-CC Object Carousel
 *  @see groupMW_TBL_MNGR_DSMCC_ENG, groupMW_TBL_MNGR_DSMCC_DB
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_DSMCC_OC_H_
#define _U_DSMCC_OC_H_

/*---------------------------------------------------------------------------
                    include files
---------------------------------------------------------------------------*/

#include "u_tm.h"

/*---------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 --------------------------------------------------------------------------*/

/* Default DSMCC OC engine name. */
#define DSMCC_OC_DEFAULT_NAME  "dsmcc_oc_engine"    /**< Default name of DSM-CC Obeject Carousel engine name*/

/* DSMCC OC handle types. */
#define TMT_DSMCC_OC_SRG       (TMT_DSMCC_OC + ((HANDLE_TYPE_T)  0))    /**< DSM-CC Object Carousel Service Gateway handle type */
#define TMT_DSMCC_OC_DIR       (TMT_DSMCC_OC + ((HANDLE_TYPE_T)  1))    /**< DSM-CC Object Carousel Directory handle type */
#define TMT_DSMCC_OC_FILE      (TMT_DSMCC_OC + ((HANDLE_TYPE_T)  2))    /**< DSM-CC Object Carousel File handle type */
#define TMT_DSMCC_OC_ASYNC_RD  (TMT_DSMCC_OC + ((HANDLE_TYPE_T)  3))    /**< DSM-CC Object Carousel asynchronous read handle type */
#define TMT_DSMCC_OC_LOAD_BY_PATH (TMT_DSMCC_OC + ((HANDLE_TYPE_T)  4))    /**< DSM-CC Object Carousel asynchronous read handle type */

/* DSMCC OC get types. */
#define DSMCC_OC_GET_TYPE_NUM_SUBENTRIES  (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 0))    /**< DSM-CC Object Carousel number of subentries get type */
#define DSMCC_OC_GET_TYPE_SUBENTRY_INFO   (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 1))    /**< DSM-CC Object Carousel subentry information get type */
#define DSMCC_OC_GET_TYPE_FILE_INFO       (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 2))    /**< DSM-CC Object Carousel file information get type */
#define DSMCC_OC_GET_TYPE_FILE_RD         (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 3))    /**< DSM-CC Object Carousel read file get type */

/* DSMCC OC load types. */
#define DSMCC_OC_LOAD_TYPE_PID_TS_DESCR  ((TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 0)) | TM_LOAD_TYPE_SINGLE_SRC)    /**< Load type by PID and TS descriptor */
#define DSMCC_OC_LOAD_TYPE_PID_TS_ID     ((TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 1)) | TM_LOAD_TYPE_SINGLE_SRC)    /**< Load type by PID and TS ID */
#define DSMCC_OC_LOAD_TYPE_NAME          ((TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 2)) | TM_LOAD_TYPE_SINGLE_SRC)    /**< Load type by name */
#define DSMCC_OC_LOAD_TYPE_ASYNC_RD      ((TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 3)) | TM_LOAD_TYPE_SINGLE_SRC)    /**< Load type for asynchronous read */
#define DSMCC_OC_LOAD_N_READ_TYPE_NAME   ((TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 4)) | TM_LOAD_TYPE_SINGLE_SRC)    /**< Load & Read type by name */

/* DSMCC OC parse types. */
#define DSMCC_OC_PARSE_TYPE_SRG_PVT_DATA  (TM_PARSE_TYPE_LAST_ENTRY + ((TM_PARSE_TYPE_T) 0))    /**< Parse type for service gateway private data */

/* User Info descrptor tag */
/* DSMCC Module Info */
#define DSMCC_OC_MODULE_USER_INFO_COMPRESSED_MODULE_TAG    (0x09)
#define DSMCC_OC_MODULE_USER_INFO_CACHING_PRIORITY_TAG     (0x71)

/* For DSMCC_OC_MODULE_USER_INFO_COMPRESSED_MODULE_TAG */
#define DSMCC_OC_COMPRESSED_MODULE_COMPRESS_METHOD         (0x08)
#define DSMCC_OC_MODULE_ZLIB_COMPRESSION(compression_method)  ((compression_method & 0xF) ==  0x8)

/* For DSMCC_OC_MODULE_USER_INFO_CACHING_PRIORITY_TAG */
#define DSMCC_OC_CACHING_PRIORITY_DEFAULT_PRIORITY_VALUE        (0x80)
#define DSMCC_OC_CACHING_PRIORITY_TRANSPARENT_CACHING           (0x01)
#define DSMCC_OC_CACHING_PRIORITY_SEMI_TRANSPARENT_CACHING      (0x02)
#define DSMCC_OC_CACHING_PRIORITY_STATIC_CACHING                (0x03)

#define DSMCC_OC_SEMI_TRANSPARENT_CACHING_TIMEOUT               (3000) /* 30 seconds = 2000 sys ticks in linux */
#define DSMCC_OC_TRANSPARENT_CACHING_TIMEOUT                    (50)   /* 0.5 second = 50 sys ticks in linux*/

/* Structure used for DSMCC_OC_PARSE_TYPE_SRG_PVT_DATA. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_OBJ_DATA_T
 *  @brief  Structure used for DSMCC_OC_PARSE_TYPE_SRG_PVT_DATA.
 *  @code
 *  typedef struct _DSMCC_OC_OBJ_DATA_T
 *  {
 *      const UINT8*  pui1_obj_data;           
 *      SIZE_T        z_obj_data_len;          
 *  }   DSMCC_OC_OBJ_DATA_T;
 *  @endcode
 *  @li@c  pui1_obj_data  - Service Gateway private data.
 *  @li@c  z_obj_data_len - Length of Service Gateway private data.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_OBJ_DATA_T
{
    const UINT8*  pui1_obj_data;           /* References the data to be parsed. */ 

    SIZE_T        z_obj_data_len;          /* Contains the length of the data. */
}   DSMCC_OC_OBJ_DATA_T;


/* Structure used for DSMCC_OC_LOAD_TYPE_PID_TS_DESCR. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_PID_TS_DESCR_T
 *  @brief  Structure used for DSMCC_OC_LOAD_TYPE_PID_TS_DESCR.
 *  @code
 *  typedef struct _DSMCC_OC_PID_TS_DECSR_T
 *  {
 *      TS_DESCR_T  t_ts_descr;                
 *      HANDLE_T  h_root;                      
 *      MPEG_2_PID_T  t_pid;                   
 *  }   DSMCC_OC_PID_TS_DESCR_T;
 *  @endcode
 *  @li@c  t_ts_descr - TS descriptor structure.
 *  @li@c  h_root     - Handle of root.
 *  @li@c  t_pid      - PID.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_PID_TS_DECSR_T
{
    TS_DESCR_T  t_ts_descr;                /* TS descriptor. */

    HANDLE_T  h_root;                      /* Root handle. */

    MPEG_2_PID_T  t_pid;                   /* PID value. */
}   DSMCC_OC_PID_TS_DESCR_T;

/* Structure used for DSMCC_OC_LOAD_TYPE_PID_TS_ID. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_PID_TS_ID_T
 *  @brief  Structure used for DSMCC_OC_LOAD_TYPE_PID_TS_ID.
 *  @code
 *  typedef struct _DSMCC_OC_PID_TS_ID_T
 *  {
 *      MPEG_2_PID_T  t_pid;                   
 *      HANDLE_T  h_root;                      
 *      UINT16  ui2_ts_id;                     
 *  }   DSMCC_OC_PID_TS_ID_T;
 *  @endcode
 *  @li@c  t_pid     - PID
 *  @li@c  h_root    - Handle of root.
 *  @li@c  ui2_ts_id - TS ID.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_PID_TS_ID_T
{
    MPEG_2_PID_T  t_pid;                   /* PID value. */

    HANDLE_T  h_root;                      /* Root handle. */

    UINT16  ui2_ts_id;                     /* TS id. */
}   DSMCC_OC_PID_TS_ID_T;

/* Structure used for DSMCC_OC_LOAD_TYPE_NAME for a directory. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_DIR_NAME_T
 *  @brief  Structure used for DSMCC_OC_LOAD_TYPE_NAME for a directory.
 *  @code
 *  typedef struct _DSMCC_OC_DIR_NAME_T
 *  {
 *      const CHAR*  ps_name;                  
 *  }   DSMCC_OC_DIR_NAME_T;
 *  @endcode
 *  @li@c  ps_name - Directory name.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_DIR_NAME_T
{
    const CHAR*  ps_name;                  /* Directory name. */
}   DSMCC_OC_DIR_NAME_T;

/* Structure used for DSMCC_OC_LOAD_TYPE_NAME for a file. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_FILE_NAME_T
 *  @brief  Structure used for DSMCC_OC_LOAD_TYPE_NAME for a file.
 *  @code
 *  typedef struct _DSMCC_OC_FILE_NAME_T
 *  {
 *      const CHAR*  ps_name;                  
 *      BOOL  b_file_cache;                    
 *  }   DSMCC_OC_FILE_NAME_T;
 *  @endcode
 *  @li@c  ps_name      - File name.
 *  @li@c  b_file_cache - Indicates if the whole file is cached into memory.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_FILE_NAME_T
{
    const CHAR*  ps_name;                  /* File name. */

    BOOL  b_file_cache;                    /* If set to TRUE will load the whole file into memory. */
}   DSMCC_OC_FILE_NAME_T;


/*------------------------------------------------------------------*/
/*! @enum  DSMCC_OC_OBJ_TYPE
 *  @brief Indicates the object type.
 *  @code
 *  typedef enum
 *  {
 *      DSMCC_OC_TYPE_DIR = 0,
 *      DSMCC_OC_TYPE_FILE
 *  }   DSMCC_OC_OBJ_TYPE;
 *  @endcode
 *  @li@c  DSMCC_OC_TYPE_DIR  - Object type is a directory.
 *  @li@c  DSMCC_OC_TYPE_FILE - Object type is a file.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DSMCC_OC_TYPE_DIR = 0,
    DSMCC_OC_TYPE_FILE
}   DSMCC_OC_OBJ_TYPE;


typedef struct _DSMCC_OC_LOAD_FILE_BY_PATH
{
    /* to be filled by client */
    VOID*         pv_client_nfy_tag;  /* org client request tag */
    x_tm_nfy_fct  pf_client_nfy;      /* org client request nfy */
    HANDLE_T*     ph_client_file;     /* org client request handle */
    BOOL          b_file_cache;       /* org client file cache request */

    /* to be filled by DSMCC */
    CHAR*              ps_path;
    HANDLE_T           h_current_handle;
    DSMCC_OC_OBJ_TYPE  e_current_handle_type;
}   DSMCC_OC_LOAD_FILE_BY_PATH;


/* Structure used for DSMCC_OC_LOAD_TYPE_NAME for a file. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_LOAD_N_READ_FILE_NAME_T
 *  @brief  Structure used for DSMCC_OC_LOAD_TYPE_NAME for a file.
 *  @code
 *  typedef struct _DSMCC_OC_FILE_NAME_T
 *  {
 *      const CHAR*  ps_name;                  
 *      BOOL  b_file_cache;                    
 *      UINT64  ui8_offs;
 *      UINT8*  pui1_buff;
 *      SIZE_T  z_buff_len;
 *  }   DSMCC_OC_FILE_NAME_T;
 *  @endcode
 *  @li@c  ps_name      - File name.
 *  @li@c  b_file_cache - Indicates if the whole file is cached into memory.
 *  @li@c  ui8_offs     - Offset within the file.
 *  @li@c  pui1_buff    - Buffer for reading the file.
 *  @li@c  z_buff_len   - Buffer length.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_LOAD_N_READ_FILE_NAME_T
{
    const CHAR*  ps_name;                  /* File name. */

    BOOL  b_file_cache;                    /* If set to TRUE will load the whole file into memory. */

    UINT64  ui8_offs;

    UINT8*  pui1_buff;

    SIZE_T  z_buff_len;
}   DSMCC_OC_LOAD_N_READ_FILE_NAME_T;


/* Structure associated with DSMCC_OC_LOAD_TYPE_ASYNC_RD */
/* Asynchronous read from a file from a specific offset. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_ASYNC_RD_INFO_T
 *  @brief  Structure associated with DSMCC_OC_LOAD_TYPE_ASYNC_RD. Asynchronous read from a file from a specific offset.
 *  @code
 *  typedef struct _DSMCC_OC_ASYNC_RD_INFO_T
 *  {
 *      UINT64  ui8_offs;
 *      UINT8*  pui1_buff;
 *      SIZE_T  z_buff_len;
 *  }   DSMCC_OC_ASYNC_RD_INFO_T;
 *  @endcode
 *  @li@c  ui8_offs   - Offset within the file.
 *  @li@c  pui1_buff  - Buffer for reading the file.
 *  @li@c  z_buff_len - Buffer length.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_ASYNC_RD_INFO_T
{
    UINT64  ui8_offs;

    UINT8*  pui1_buff;

    SIZE_T  z_buff_len;
}   DSMCC_OC_ASYNC_RD_INFO_T;


/* DSM::Tap with optional selector. */
#define DSMCC_OC_SELECTOR_TYPE_UNKNOWN  ((UINT16) 0x0000)    /**< Selector type values as defined by ISO 13818-6 and ATSC A-90. Value ．0・is reserved by ISO. */
#define DSMCC_OC_SELECTOR_TYPE_MSG      ((UINT16) 0x0001)    /**< Selector type values as defined by ISO 13818-6 and ATSC A-90. Value ．1・ specifies a MESSAGE type selector. */

/* Selector structure for DSMCC_OC_SELECTOR_TYPE_MSG. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_SELECTOR_MSG_T
 *  @brief  Selector structure for DSMCC_OC_SELECTOR_TYPE_MSG.
 *  @code
 *  typedef struct _DSMCC_OC_SELECTOR_MSG_T
 *  {
 *      UINT32  ui4_trans_act_id;
 *      UINT32  ui4_timeout;
 *  }   DSMCC_OC_SELECTOR_MSG_T;
 *  @endcode
 *  @li@c  ui4_trans_act_id - Transaction ID.
 *  @li@c  ui4_timeout      - Timeout length.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_SELECTOR_MSG_T
{
    UINT32  ui4_trans_act_id;
    UINT32  ui4_timeout;
}   DSMCC_OC_SELECTOR_MSG_T;

/* Optional selector attached to a DSM::Tap object. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_SELECTOR_T
 *  @brief  Optional selector attached to a DSM::Tap object.
 *  @code
 *  typedef struct _DSMCC_OC_SELECTOR_T
 *  {
 *      UINT16  ui2_type;
 *      union
 *      {
 *          DSMCC_OC_SELECTOR_MSG_T  t_msg;
 *      }   u_selector;
 *  }   DSMCC_OC_SELECTOR_T;
 *  @endcode
 *  @li@c  ui2_type - Selector type.
 *  @li@c  t_msg    - Selector message.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_SELECTOR_T
{
    UINT16  ui2_type;

    union
    {
        DSMCC_OC_SELECTOR_MSG_T  t_msg;
    }   u_selector;

}   DSMCC_OC_SELECTOR_T;

#define DSMCC_OC_TAP_SIZE                        ((UINT32) 0x7)       /**< Size of each DSM Tap */

/* TAP use definitions */
#define DSMCC_OC_TAP_USE_UNKNOWN                 ((UINT16) 0x0000)    /**< DSM Tap usage unknown */
#define DSMCC_OC_TAP_USE_BIOP_DELIVERY_PARA_USE  ((UINT16) 0x0016)    /**< DSM Tap use type: module delivery type */
#define DSMCC_OC_TAP_USE_BIOP_OBJECT_USE         ((UINT16) 0x0017)    /**< BIOP(Broadcast Inter-ORB Protocol) objects in modules */

/* DSM::Tap. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_DSM_TAP_T
 *  @brief  A structure containing the data of a single DSM-CC Tap.
 *  @code
 *  typedef struct _DSMCC_OC_DSM_TAP_T
 *  {
 *      DSMCC_OC_SELECTOR_T  t_selector;    
 *      UINT16  ui2_tap_id;                 
 *      UINT16  ui2_tap_use;                
 *      UINT16  ui2_assoc_tag;              
 *  }   DSMCC_OC_DSM_TAP_T;
 *  @endcode
 *  @li@c  t_selector    - Selector attached to a DSM::Tap object.
 *  @li@c  ui2_tap_id    - ID of the tap.
 *  @li@c  ui2_tap_use   - Tap use type
 *  @li@c  ui2_assoc_tag - Association tag of the tap.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_DSM_TAP_T
{
    DSMCC_OC_SELECTOR_T  t_selector;    /* Contains the DSM::Tap selector type */

    UINT16  ui2_tap_id;                 /* Tap id as specified by standards ISO 13818-6 and ATSC A-90 */
    UINT16  ui2_tap_use;                /* Tap usage as specified by standards ISO 13818-6 and ATSC A-90 */
    UINT16  ui2_assoc_tag;              /* Association tag as specified by standards ISO 13818-6 and ATSC A-90. */
}   DSMCC_OC_DSM_TAP_T;


/* Structure notified with TM_COND_DSM_TAP_XLAT. */
/* Request client to translate a DSM::Tap.       */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_DSM_TAP_XLAT_T
 *  @brief  Used to translate a DSM-CC Tap into a transport stream description. This translation must be done outside of the DSMCC engine.
 *  @code
 *  typedef struct _DSMCC_OC_DSM_TAP_XLAT_T
 *  {
 *      DSMCC_OC_DSM_TAP_T  t_dsm_tap;      
 *      TS_DESCR_T  t_dsi_ts_descr;         
 *      TS_DESCR_T  t_xlat_ts_descr;        
 *      MPEG_2_PID_T  t_dsi_pid;            
 *      MPEG_2_PID_T  t_xlat_pid;           
 *  }   DSMCC_OC_DSM_TAP_XLAT_T;
 *  @endcode
 *  @li@c  t_dsm_tap       - DSM::Tap description
 *  @li@c  t_dsi_ts_descr  - DSI's TS description.
 *  @li@c  t_xlat_ts_descr - TS description based on DSM-CC Tap description. Basically, this is the result of the translation DSMCC Tap to TS.
 *  @li@c  t_dsi_pid       - DSI's PID value.
 *  @li@c  t_xlat_pid      - PID value which is associated with the DSM-CC Tap.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_DSM_TAP_XLAT_T
{
    DSMCC_OC_DSM_TAP_T  t_dsm_tap;      /* DSM::Tap description */

    TS_DESCR_T  t_dsi_ts_descr;         /* DSI's TS description. */
    TS_DESCR_T  t_xlat_ts_descr;        /* DS::Tap --> TS descriptor translation. */

    MPEG_2_PID_T  t_dsi_pid;            /* DSI's PID value. */
    MPEG_2_PID_T  t_xlat_pid;           /* DS::Tap --> PID translation. */
}   DSMCC_OC_DSM_TAP_XLAT_T;

/* Structure notified with TM_COND_LOAD_PROGRESS. */
/* Notify a client with the load progress.        */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_LOAD_PROGRESS_T
 *  @brief  Structure notified with TM_COND_LOAD_PROGRESS. Notify a client with the load progress.
 *  @code
 *  typedef struct _DSMCC_OC_LOAD_PROGRESS_T
 *  {
 *      UINT32  ui4_curr;
 *      UINT32  ui4_total;
 *  }   DSMCC_OC_LOAD_PROGRESS_T;
 *  @endcode
 *  @li@c  ui4_curr  - Current load progress.
 *  @li@c  ui4_total - Total number of bytes to load.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_LOAD_PROGRESS_T
{
    UINT32  ui4_curr;
    UINT32  ui4_total;
}   DSMCC_OC_LOAD_PROGRESS_T;


/* Structure associated with DSMCC_OC_GET_TYPE_SUBENTRY_INFO */
/* Returns DSMCC-OC specific object info.                    */
#define DSMCC_OC_NAME_LEN_MAX  256    /**< DSM-CC Object Carousel maximum name length */


/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_SUBENTRY_INFO_T
 *  @brief  Subentry information.
 *  @code
 *  typedef struct _DSMCC_OC_SUBENTRY_INFO_T
 *  {
 *      UINT64  ui8_size;
 *      UINT32  ui4_obj_key;
 *      UINT16  ui2_assoc_tag;
 *      CHAR  s_name [DSMCC_OC_NAME_LEN_MAX];
 *      DSMCC_OC_OBJ_TYPE  e_type;
 *  }   DSMCC_OC_SUBENTRY_INFO_T;
 *  @endcode
 *  @li@c  ui8_size                       - Size of the file.
 *  @li@c  ui4_obj_key                    - Object key associated with a directory entry.
 *  @li@c  ui2_assoc_tag                  - Association tag which may also be used to select a PMT.
 *  @li@c  s_name [DSMCC_OC_NAME_LEN_MAX] - Subentry name.
 *  @li@c  e_type                         - Sunentry pbject type.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_SUBENTRY_INFO_T
{
    UINT64  ui8_size;

    UINT32  ui4_obj_key;

    UINT16  ui2_assoc_tag;

    CHAR  s_name [DSMCC_OC_NAME_LEN_MAX];

    DSMCC_OC_OBJ_TYPE  e_type;

    /* From caching priority descriptor */
    UINT8   ui1_caching_priority;                           /* Caching priority */
    UINT8   ui1_transparency_level;                         /* Transparency Level */
}   DSMCC_OC_SUBENTRY_INFO_T;


/* Structure associated with DSMCC_OC_GET_TYPE_FILE_RD */
/* Reads from a file from a specific offset.           */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_FILE_RD_INFO_T
 *  @brief  Structure associated with DSMCC_OC_GET_TYPE_FILE_RD. Reads from a file from a specific offset.
 *  @code
 *  typedef struct _DSMCC_OC_FILE_RD_INFO_T
 *  {
 *      UINT64  ui8_offs;
 *      UINT32  ui4_timeout;
 *  }   DSMCC_OC_FILE_RD_INFO_T;
 *  @endcode
 *  @li@c  ui8_offs    - Offset within the file.
 *  @li@c  ui4_timeout - Timeout length.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_FILE_RD_INFO_T
{
    UINT64  ui8_offs;

    UINT32  ui4_timeout;
}   DSMCC_OC_FILE_RD_INFO_T;


/* Structure associated with DSMCC_OC_GET_TYPE_FILE_INFO */
/* Retrieves file specific information.                  */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_OC_FILE_INFO_T
 *  @brief  Structure associated with DSMCC_OC_GET_TYPE_FILE_INFO. Retrieves file specific information.Retrieves file specific information.
 *  @code
 *  typedef struct _DSMCC_OC_FILE_INFO_T
 *  {
 *      UINT64  ui8_size;
 *      UINT32  ui4_obj_key;
 *      UINT16  ui2_assoc_tag;
 *      CHAR  s_name [DSMCC_OC_NAME_LEN_MAX];
 *  }   DSMCC_OC_FILE_INFO_T;
 *  @endcode
 *  @li@c  ui8_size                       - Size of the file.
 *  @li@c  ui4_obj_key                    - Object key associated with a file entry.
 *  @li@c  ui2_assoc_tag                  - Association tag which may also be used to select a PMT.
 *  @li@c  s_name [DSMCC_OC_NAME_LEN_MAX] - Name of the file.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_OC_FILE_INFO_T
{
    UINT64  ui8_size;

    UINT32  ui4_obj_key;

    UINT16  ui2_assoc_tag;

    CHAR  s_name [DSMCC_OC_NAME_LEN_MAX];

    /* From caching priority descriptor */
    UINT8  ui1_caching_priority;    /* Caching priority */
    UINT8  ui1_transparency_level;  /* Transparency level */
}   DSMCC_OC_FILE_INFO_T;



#endif /* _U_DSMCC_OC_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                 /* end of groupMW_TBL_MNGR_DSMCC_OC */
/*----------------------------------------------------------------------------*/



