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
/*! @file u_dsmcc_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains DSM-CC engine specific definitions, which
 *         are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_TBL_MNGR_DSMCC_ENG DSM-CC Engine
 *  @ingroup groupMW_TBL_MNGR
 *  @brief Provides DSM-CC related functions
 *  %This module allows an application to retrieve data from DSM-CC carousels
 *  @see groupMW_TBL_MNGR_DSMCC_DB, groupMW_TBL_MNGR_DSMCC_OC
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_DSMCC_ENG_H_
#define _U_DSMCC_ENG_H_

/*---------------------------------------------------------------------------
                    include files
---------------------------------------------------------------------------*/

#include "u_handle_grp.h"
#include "u_common.h"
#include "u_tm.h"

/*---------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 --------------------------------------------------------------------------*/

/* Default DSMCC engine name. */
#define DSMCC_DEFAULT_NAME  "dsmcc_engine"    /**< Default DSM-CC engine name */

/* invalid PTS */
#define DSMCC_INVALID_PTS ((PTS_T) (0xffffffffffffffffLL))    /**< DSM-CC invalid TPS define */

/* DSMCC engine handle types. */
#define TMT_DSMCC_DSI      (TMT_DSMCC + ((HANDLE_TYPE_T)  0))    /**< DSM-CC DSI handle type       */
#define TMT_DSMCC_DII      (TMT_DSMCC + ((HANDLE_TYPE_T)  1))    /**< DSM-CC DII handle type       */
#define TMT_DSMCC_DDB      (TMT_DSMCC + ((HANDLE_TYPE_T)  2))    /**< DSM-CC DDB handle type       */
#define TMT_DSMCC_TOP_MON  (TMT_DSMCC + ((HANDLE_TYPE_T)  3))    /**< DSM-CC Monitoring handle type*/

/* DSMCC engine get types. */
#define DSMCC_GET_TYPE_DOWNLOAD_LEVEL   (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 0))    /**< To get download level         */
#define DSMCC_GET_TYPE_DSI_INFO         (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 1))    /**< To get DSI information        */
#define DSMCC_GET_TYPE_DII_INFO         (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 2))    /**< To get DII information        */
#define DSMCC_GET_TYPE_DII_MODULE_INFO  (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 3))    /**< To get DII module information */
#define DSMCC_GET_TYPE_DDB_INFO         (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 4))    /**< To get DDB information        */

/* DSMCC engine load types. */
#define DSMCC_LOAD_TYPE_PID_TS_DESCR_IDS   (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 0))    /**< Load by PID, TS descriptor and some other ID */
#define DSMCC_LOAD_TYPE_PID_TS_ID_IDS     ((TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 1)) | TM_LOAD_TYPE_SINGLE_SRC)    /**< Load by PID, TS ID and some other ID */
#define DSMCC_LOAD_TYPE_DII_IDS            (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 2))    /**< Load DDB based on a DII information */

/* DSMCC engine parse types. */
#define DSMCC_PARSE_TYPE_DSI_PVT_DATA     (TM_PARSE_TYPE_LAST_ENTRY + ((TM_PARSE_TYPE_T) 0))    /**< Parse type for DSI private data */
#define DSMCC_PARSE_TYPE_DII_COMP_DESCR   (TM_PARSE_TYPE_LAST_ENTRY + ((TM_PARSE_TYPE_T) 1))    /**< Parse type for DII compatibility descriptor */
#define DSMCC_PARSE_TYPE_DII_MODULE_INFO  (TM_PARSE_TYPE_LAST_ENTRY + ((TM_PARSE_TYPE_T) 2))    /**< Parse type for DII module information */
#define DSMCC_PARSE_TYPE_DII_PVT_DATA     (TM_PARSE_TYPE_LAST_ENTRY + ((TM_PARSE_TYPE_T) 3))    /**< Parse type for DII private data */
#define DSMCC_PARSE_TYPE_DDB_BLOCK_DATA   (TM_PARSE_TYPE_LAST_ENTRY + ((TM_PARSE_TYPE_T) 4))    /**< Parse type for DDB block data */


/* DSM-CC engine configuration structure */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_CONFIG_T
 *  @brief  DSM-CC engine configuration structure
 *  @code
 *  typedef struct _DSMCC_CONFIG_T
 *  {
 *      UINT8 ui1_num_dsi_filters;
 *      UINT8 ui1_num_dii_filters;
 *      UINT8 ui1_num_ddb_filters;
 *      UINT8 ui1_num_ddl_filters;
 *      SIZE_T  z_dsi_buff_size;
 *      SIZE_T  z_dii_buff_size;
 *      SIZE_T  z_ddb_buff_size;
 *      SIZE_T  z_ddl_buff_size;
 *      UINT16  ui2_num_dsi_tbl_objs;
 *      UINT16  ui2_num_dii_tbl_objs;
 *      UINT16  ui2_num_ddb_tbl_objs;
 *      UINT16  ui2_num_ddl_tbl_objs;
 *      UINT16  ui2_num_mon_tbl_objs;  
 *  }   DSMCC_CONFIG_T;
 *  @endcode
 *  @li@c  ui1_num_dsi_filters  - Number of DSI filters.
 *  @li@c  ui1_num_dii_filters  - Number of DII filters.
 *  @li@c  ui1_num_ddb_filters  - Number of DDB filters.
 *  @li@c  ui1_num_ddl_filters  - Number of download filters.
 *  @li@c  z_dsi_buff_size      - DSI buffer size.
 *  @li@c  z_dii_buff_size      - DII buffer size.
 *  @li@c  z_ddb_buff_size      - DDB buffer size.
 *  @li@c  z_ddl_buff_size      - Download buffer size.
 *  @li@c  ui2_num_dsi_tbl_objs - Number of DSI table objects.
 *  @li@c  ui2_num_dii_tbl_objs - Number of DII table objects.
 *  @li@c  ui2_num_ddb_tbl_objs - Number of DDB table objects.
 *  @li@c  ui2_num_ddl_tbl_objs - Number of download table objects.
 *  @li@c  ui2_num_mon_tbl_objs - Number of monitoring table objects.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_CONFIG_T
{
    UINT8 ui1_num_dsi_filters;
    UINT8 ui1_num_dii_filters;
    UINT8 ui1_num_ddb_filters;
    UINT8 ui1_num_ddl_filters;

    SIZE_T  z_dsi_buff_size;
    SIZE_T  z_dii_buff_size;
    SIZE_T  z_ddb_buff_size;
    SIZE_T  z_ddl_buff_size;

    UINT16  ui2_num_dsi_tbl_objs;
    UINT16  ui2_num_dii_tbl_objs;
    UINT16  ui2_num_ddb_tbl_objs;
    UINT16  ui2_num_ddl_tbl_objs;
    UINT16  ui2_num_mon_tbl_objs;  /* dsi or top-level dii */
}   DSMCC_CONFIG_T;

/* Definition of the DSMCC server id. */
#define DSMCC_SERVER_ID_MAX_LEN  20    /**< Maximum DSM-CC server ID length */

typedef UINT8 DSMCC_SERVER_ID_T [DSMCC_SERVER_ID_MAX_LEN];    /**< DSM-CC server ID */

/* Transaction ID fields mask */
#define DSMCC_TRANS_ID_UPDATE_FLAG_MASK       (0x00000001) /* 0x00000001 = 0000 0000 0000 0000 0000 0000 0000 0001 */
#define DSMCC_TRANS_ID_IDENTIFICATION_MASK    (0x0000FFFE) /* 0x0000FFFE = 0000 0000 0000 0000 1111 1111 1111 1110 */
#define DSMCC_TRANS_ID_VERSION_MASK           (0x3FFF0000) /* 0x3FFF0000 = 0011 1111 1111 1111 0000 0000 0000 0000 */
#define DSMCC_TRANS_ID_ORIGINATOR_MASK        (0xC0000000) /* 0xC0000000 = 1100 0000 0000 0000 0000 0000 0000 0000 */

#define DSMCC_TRANS_ID_EXTRACT_ID_FIELD_MASK(trans_id)    (trans_id & DSMCC_TRANS_ID_IDENTIFICATION_MASK)
#define DSMCC_TRANS_ID_EXTRACT_VER_FIELD_MASK(trans_id)   (trans_id & DSMCC_TRANS_ID_VERSION_MASK)

/* Structure associated with DSMCC_GET_TYPE_DSI_INFO */
/* Returns the common data of a DSI table.           */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_DSI_INFO_T
 *  @brief  Structure associated with DSMCC_GET_TYPE_DSI_INFO. Returns the common data of a DSI table.
 *  @code
 *  typedef struct _DSMCC_DSI_INFO_T
 *  {
 *      UINT32  ui4_transaction_id;
 *      DSMCC_SERVER_ID_T  t_server_id;
 *  }   DSMCC_DSI_INFO_T;
 *  @endcode
 *  @li@c  ui4_transaction_id - Transaction ID
 *  @li@c  t_server_id        - Server ID
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_DSI_INFO_T
{
    UINT32  ui4_transaction_id;

    DSMCC_SERVER_ID_T  t_server_id;
}   DSMCC_DSI_INFO_T;


/* Structure associated with DSMCC_GET_TYPE_DII_INFO */
/* Returns the common data of a DII table.           */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_DII_INFO_T
 *  @brief  Structure associated with DSMCC_GET_TYPE_DII_INFO. Returns the common data of a DII table.
 *  @code
 *  typedef struct _DSMCC_DII_INFO_T
 *  {
 *      UINT32  ui4_download_id;
 *      UINT32  ui4_transaction_id;
 *      UINT32  ui4_download_window;
 *      UINT32  ui4_download_scenario;
 *      UINT16  ui2_block_size;
 *      UINT16  ui2_num_modules;
 *      UINT8  ui1_window_size;
 *      UINT8  ui1_ack_period;
 *  }   DSMCC_DII_INFO_T;
 *  @endcode
 *  @li@c  ui4_download_id       - Download ID
 *  @li@c  ui4_transaction_id    - Transaction ID
 *  @li@c  ui4_download_window   - Download window
 *  @li@c  ui4_download_scenario - Download scenario
 *  @li@c  ui2_block_size        - Block size
 *  @li@c  ui2_num_modules       - Number of modules
 *  @li@c  ui1_window_size       - Window size
 *  @li@c  ui1_ack_period        - Ack period
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_DII_INFO_T
{
    UINT32  ui4_download_id;
    UINT32  ui4_transaction_id;
    UINT32  ui4_download_window;
    UINT32  ui4_download_scenario;
    UINT32  ui4_version;

    UINT16  ui2_block_size;
    UINT16  ui2_num_modules;

    UINT8  ui1_window_size;
    UINT8  ui1_ack_period;
}   DSMCC_DII_INFO_T;

/* Structure associated with DSMCC_GET_TYPE_DII_MODULE_INFO */
/* Returns the common module data of a DII table.           */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_DII_MODULE_INFO_T
 *  @brief  Structure associated with DSMCC_GET_TYPE_DII_MODULE_INFO. Returns the common module data of a DII table.
 *  @code
 *  typedef struct _DSMCC_DII_MODULE_INFO_T
 *  {
 *      UINT32  ui4_module_size;
 *      UINT16  ui2_module_id;
 *      UINT8  ui1_module_version;
 *  }   DSMCC_DII_MODULE_INFO_T;
 *  @endcode
 *  @li@c  ui4_module_size    - DII module size.
 *  @li@c  ui2_module_id      - DII module ID.
 *  @li@c  ui1_module_version - DII module version.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_DII_MODULE_INFO_T
{
    UINT32  ui4_module_size;

    UINT16  ui2_module_id;

    UINT8  ui1_module_version;

    /* Compressed module descriptor */
    UINT8  ui1_compression_method;
    UINT32 ui4_original_size;

    /* Caching priority descriptor */
    UINT8  ui1_caching_priority;
    UINT8  ui1_transparency_level;
}   DSMCC_DII_MODULE_INFO_T;

/* Structure associated with DSMCC_GET_TYPE_DDB_INFO */
/* Returns the common data of a DDB table.           */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_DDB_INFO_T
 *  @brief  Structure associated with DSMCC_GET_TYPE_DDB_INFO. Returns the common data of a DDB table.
 *  @code
 *  typedef struct _DSMCC_DDB_INFO_T
 *  {
 *      UINT32  ui4_download_id;
 *      UINT16  ui2_module_id;
 *      UINT16  ui2_block_num;
 *      UINT8   ui1_version;
 *  }   DSMCC_DDB_INFO_T;
 *  @endcode
 *  @li@c  ui4_download_id - DDB Download ID
 *  @li@c  ui2_module_id   - DDB Module ID
 *  @li@c  ui2_block_num   - DDB Block number.
 *  @li@c  ui1_version     - DDB Version.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_DDB_INFO_T
{
    UINT32  ui4_download_id;

    UINT16  ui2_module_id;
    UINT16  ui2_block_num;

    UINT8   ui1_version;
}   DSMCC_DDB_INFO_T;


/* Structure used to load specific DSI, DII and / or DDB tables. */
#define DSMCC_IDS_FLAG_DOWNLOAD_ID_VALID     ((UINT8) 0x01)    /**< Indicates that Download ID flag is valid. */
#define DSMCC_IDS_FLAG_TRANSACTION_ID_VALID  ((UINT8) 0x02)    /**< Indicates that Transaction ID is valid. */
#define DSMCC_IDS_FLAG_MODULE_ID_VALID       ((UINT8) 0x04)    /**< Indicates that Module ID is valid. */
#define DSMCC_IDS_FLAG_BLOCK_NUM_VALID       ((UINT8) 0x08)    /**< Indicates that Block number is valid. */
#define DSMCC_IDS_FLAG_VERSION_VALID         ((UINT8) 0x10)    /**< Indicates that Version is valid. */

/*------------------------------------------------------------------*/
/*! @struct DSMCC_IDS_T
 *  @brief  Structure to contain various ID's, and flags to indicate which ID's are valid.
 *  @code
 *  typedef struct _DSMCC_IDS_T
 *  {
 *      UINT32  ui4_download_id;
 *      UINT32  ui4_transaction_id;
 *      UINT16  ui2_module_id;
 *      UINT16  ui2_first_block_num;
 *      UINT16  ui2_last_block_num;
 *      UINT8  ui1_version;
 *      UINT8  ui1_flags;
 *  }   DSMCC_IDS_T;
 *  @endcode
 *  @li@c  ui4_download_id     - Download ID.
 *  @li@c  ui4_transaction_id  - Transaction ID.
 *  @li@c  ui2_module_id       - Module ID.
 *  @li@c  ui2_first_block_num - First block number.
 *  @li@c  ui2_last_block_num  - Last block number.
 *  @li@c  ui1_version         - Version.
 *  @li@c  ui1_flags           - Flags to indicate which ID's are valid.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_IDS_T
{
    UINT32  ui4_download_id;
    UINT32  ui4_transaction_id;

    UINT16  ui2_module_id;
    UINT16  ui2_first_block_num;
    UINT16  ui2_last_block_num;

    UINT8  ui1_version;
    UINT8  ui1_flags;
}   DSMCC_IDS_T;


/* Structure associated with DSMCC_LOAD_TYPE_PID_TS_DESCR_IDS and */
/* will load specific DSI, DII and / or DDB table based on DSMCC  */
/* id's, PID value and TS id.                                     */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_PID_TS_DESCR_IDS_T
 *  @brief  Structure associated with DSMCC_LOAD_TYPE_PID_TS_DESCR_IDS 
 *          and will load specific DSI, DII and / or DDB table based on 
 *          DSMCC id's, PID value and TS id.
 *  @code
 *  typedef struct _DSMCC_PID_TS_DESCR_IDS_T
 *  {
 *      DSMCC_IDS_T  t_dsmcc_ids;
 *      TS_DESCR_T    t_ts_descr;
 *      MPEG_2_PID_T  t_pid;
 *  }   DSMCC_PID_TS_DESCR_IDS_T;
 *  @endcode
 *  @li@c  t_dsmcc_ids - DSM-CC ID's
 *  @li@c  t_ts_descr  - TS descriptor
 *  @li@c  t_pid       - PID
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_PID_TS_DESCR_IDS_T
{
    DSMCC_IDS_T  t_dsmcc_ids;

    TS_DESCR_T    t_ts_descr;

    MPEG_2_PID_T  t_pid;
}   DSMCC_PID_TS_DESCR_IDS_T;

/* Structure associated with DSMCC_LOAD_TYPE_PID_TS_ID_IDS and */
/* will load specific DSI, DII and / or DDB table based on     */
/* DSMCC id's, PID value and TS id.                            */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_PID_TS_ID_IDS_T
 *  @brief  Structure associated with DSMCC_LOAD_TYPE_PID_TS_ID_IDS 
 *          and will load specific DSI, DII and / or DDB table based on 
 *          DSMCC id's, PID value and TS id.
 *  @code
 *  typedef struct _DSMCC_PID_TS_ID_IDS_T
 *  {
 *      DSMCC_IDS_T  t_dsmcc_ids;
 *      MPEG_2_PID_T  t_pid;
 *      UINT16  ui2_ts_id;
 *  }   DSMCC_PID_TS_ID_IDS_T;
 *  @endcode
 *  @li@c  t_dsmcc_ids - DSM-CC ID's
 *  @li@c  t_pid       - PID
 *  @li@c  ui2_ts_id   - TS ID
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_PID_TS_ID_IDS_T
{
    DSMCC_IDS_T  t_dsmcc_ids;

    MPEG_2_PID_T  t_pid;

    UINT16  ui2_ts_id;
}   DSMCC_PID_TS_ID_IDS_T;


/* Structure associated with TM_LOAD_TYPE_TS_DESCR. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_PID_TS_DESCR_T
 *  @brief  Structure associated with TM_LOAD_TYPE_TS_DESCR.
 *  @code
 *  typedef struct _DSMCC_PID_TS_DESCR_T
 *  {
 *      TS_DESCR_T  t_ts_descr;
 *      MPEG_2_PID_T  t_pid;
 *  }   DSMCC_PID_TS_DESCR_T;
 *  @endcode
 *  @li@c  t_ts_descr - TS descriptor.
 *  @li@c  t_pid      - PID.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_PID_TS_DESCR_T
{
    TS_DESCR_T  t_ts_descr;

    MPEG_2_PID_T  t_pid;
}   DSMCC_PID_TS_DESCR_T;

/* Structure associated with TM_LOAD_TYPE_TS_ID. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_PID_TS_ID_T
 *  @brief  Structure associated with TM_LOAD_TYPE_TS_ID.
 *  @code
 *  typedef struct _DSMCC_PID_TS_ID_T
 *  {
 *      MPEG_2_PID_T  t_pid;
 *      UINT16  ui2_ts_id;
 *  }   DSMCC_PID_TS_ID_T;
 *  @endcode
 *  @li@c  t_pid     - PID.
 *  @li@c  ui2_ts_id - TS ID.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_PID_TS_ID_T
{
    MPEG_2_PID_T  t_pid;

    UINT16  ui2_ts_id;
}   DSMCC_PID_TS_ID_T;



/*------------------------------------------------------------------*/
/*! @enum  DSMCC_DOWNLOAD_TYPE_T
 *  @brief Download level
 *  @code
 *  typedef enum {invalid_level, one_level, two_level} DSMCC_DOWNLOAD_TYPE_T;
 *  @endcode
 */
/*------------------------------------------------------------------*/
typedef enum {invalid_level, one_level, two_level} DSMCC_DOWNLOAD_TYPE_T;


#endif /* _U_DSMCC_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                /* end of groupMW_TBL_MNGR_DSMCC_ENG */
/*----------------------------------------------------------------------------*/


