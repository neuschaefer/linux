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
/*-----------------------------------------------------------------------------*/
/*! @file u_tm.h 
 * $RCSfile: u_tm.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 *  @par Description:
 *         This header file contains Table Manager specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _U_TM_H_
#define _U_TM_H_

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_TBL_MNGR Table Manager
 *
 *  @ingroup groupMW
 *  @brief Table Manager is used to get Service Information from stream.
 *
 *  %Table Manager is designed for user to retrive and monitor service information
 *   that tranmitted via transport stream.
 *
 *   Table Manager is consitant of a framework, and a set of difference sub 
 *   engine. Those engines are designed for retrieve and monitor different 
 *   service informaton table for differenct DTV standard. E.g. there would be 
 *   a DVB-SI engine, which is used to retrieve and monitor the table SDT, NIT 
 *   that defined in DVB standard. And there also be a ATSC-SI engine, which is 
 *   used to retrieve and monitor the table MGT, VCT that defined in ATSC standard.
 *
 *  @see groupMW_TBL_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_handle.h"
#include "u_common.h"
#include "u_sys_name.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Max number of characters in a Table Manager name */
/* (excluding the zero terminating character).      */
#define TM_NAME_LEN  SYS_NAME_LEN

/* Table Manager handles. */
#define TMT_UNKNOWN     (HT_GROUP_TM + ((HANDLE_TYPE_T)  0))
#define TMT_TBL_ENGINE  (HT_GROUP_TM + ((HANDLE_TYPE_T)  1))
#define TMT_ROOT        (HT_GROUP_TM + ((HANDLE_TYPE_T)  2))
#define TMT_LAST_ENTRY  (HT_GROUP_TM + ((HANDLE_TYPE_T)  8))

#define TMT_PSI       (TMT_LAST_ENTRY + ((HANDLE_TYPE_T)  0))
#define TMT_DVB       (TMT_LAST_ENTRY + ((HANDLE_TYPE_T)  6))
#define TMT_ATSC      (TMT_LAST_ENTRY + ((HANDLE_TYPE_T) 25))
#define TMT_SECM      (TMT_LAST_ENTRY + ((HANDLE_TYPE_T) 45))
#define TMT_DSMCC     (TMT_LAST_ENTRY + ((HANDLE_TYPE_T) 53))
#define TMT_SCTE      (TMT_LAST_ENTRY + ((HANDLE_TYPE_T) 60))
#define TMT_SECT      (TMT_LAST_ENTRY + ((HANDLE_TYPE_T) 70))
#define TMT_MHP       (TMT_LAST_ENTRY + ((HANDLE_TYPE_T) 75))
#define TMT_DSMCC_OC  (TMT_LAST_ENTRY + ((HANDLE_TYPE_T) 92))

#define TMT_ISDB   (TMT_LAST_ENTRY + ((HANDLE_TYPE_T) 85))

/* Some type definitions. */
typedef UINT32 TM_GET_TYPE_T;
typedef UINT32 TM_LOAD_TYPE_T;
typedef UINT32 TM_MON_TYPE_T;
typedef UINT32 TM_PARSE_TYPE_T;
typedef UINT32 TM_SET_TYPE_T;
typedef UINT32 TM_STORE_TYPE_T;

typedef UINT8 TM_SRC_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum    NFY_RET_T
 *  @brief   Type of notify return code
 
 *  @code
 * typedef enum
 * {
 *     NFY_RET_INVALID = -1,
 *     NFY_RET_PROCESSED,
 *     NFY_RET_RETRY,
 * 
 *     NFY_RET_DSM_TAP_XLAT_SYNC_SAME_TS_PID = 32,
 *     NFY_RET_DSM_TAP_XLAT_SYNC_TS_PID_SET
 * 
 * }   NFY_RET_T;
 *  @endcode

 *  @li@c   NFY_RET_INVALID                               - invalid         
 *  @li@c   NFY_RET_PROCESSED                             - processed       
 *  @li@c   NFY_RET_RETRY                                 - retry           
 *  @li@c   NFY_RET_DSM_TAP_XLAT_SYNC_SAME_TS_PID         - sync same ts pid
 *  @li@c   NFY_RET_DSM_TAP_XLAT_SYNC_TS_PID_SET          - sync ts pid set 
 */
/*------------------------------------------------------------------*/
/* Return values of (*x_tm_nfy_fct). */
typedef enum
{
    NFY_RET_INVALID = -1,
    NFY_RET_PROCESSED,
    NFY_RET_RETRY,

    NFY_RET_DSM_TAP_XLAT_SYNC_SAME_TS_PID = 32,
    NFY_RET_DSM_TAP_XLAT_SYNC_TS_PID_SET

}   NFY_RET_T;

/*------------------------------------------------------------------*/
/*! @enum    PARSE_RET_T
 *  @brief   Type of parse function return code
 
 *  @code
 * typedef enum
 * {
 *     PARSE_RET_ERROR = -1,
 *     PARSE_RET_END,
 *     PARSE_RET_NEXT,
 *     PARSE_RET_RESTART
 * }   PARSE_RET_T;
 *  @endcode

 *  @li@c   PARSE_RET_ERROR                             - error  
 *  @li@c   PARSE_RET_END                               - end    
 *  @li@c   PARSE_RET_NEXT                              - next   
 *  @li@c   PARSE_RET_RESTART                           - restart
 */
/*------------------------------------------------------------------*/
/* Return values of (*x_tm_parse_fct). */
typedef enum
{
    PARSE_RET_ERROR = -1,
    PARSE_RET_END,
    PARSE_RET_NEXT,
    PARSE_RET_RESTART
}   PARSE_RET_T;


/*------------------------------------------------------------------*/
/*! @enum    PARSE_RET_T
 *  @brief   Type of parse function return code
 
 *  @code
 * typedef enum
 * {
 *     TM_COND_UNAVAILABLE = 0,
 *     TM_COND_TRANSITION,
 *     TM_COND_AVAILABLE,
 *     TM_COND_UPDATE,
 *     TM_COND_SECTION,
 *     TM_COND_SRC_CHANGED,
 *     TM_COND_VER_CHANGED,
 * 
 *     TM_COND_DSM_TAP_XLAT = 32,
 *     TM_COND_LOAD_PROGRESS
 * }   TM_COND_T;
 *  @endcode

 *  @li@c   TM_COND_UNAVAILABLE                         - UNAVAILABLE  
 *  @li@c   TM_COND_TRANSITION                          - TRANSITION   
 *  @li@c   TM_COND_AVAILABLE                           - AVAILABLE    
 *  @li@c   TM_COND_UPDATE                              - UPDATE       
 *  @li@c   TM_COND_SECTION                             - SECTION      
 *  @li@c   TM_COND_SRC_CHANGED                         - SRC_CHANGED  
 *  @li@c   TM_COND_VER_CHANGED                         - VER_CHANGED  
 *  @li@c   TM_COND_DSM_TAP_XLAT                        - DSM_TAP_XLAT 
 *  @li@c   TM_COND_LOAD_PROGRESS                       - LOAD_PROGRESS
 */
/*------------------------------------------------------------------*/
/* Notify conditions. Note that this enumeration may only contain */
/* entries with a value '0' to '63' (inclusive). All other values */
/* are reserved for internal notify conditions.                   */
typedef enum
{
    TM_COND_UNAVAILABLE = 0,
    TM_COND_TRANSITION,
    TM_COND_AVAILABLE,
    TM_COND_UPDATE,
    TM_COND_SECTION,
    TM_COND_SRC_CHANGED,
    TM_COND_VER_CHANGED,

    TM_COND_DSM_TAP_XLAT = 32,    /* Only used by the DSMCC-OC */
    TM_COND_LOAD_PROGRESS,        /* Only used by DSMCC-OC and DSMCC-DB. */

    TM_COND_SRC_DISC = 64,
    TM_COND_SRC_CONN = 65

}   TM_COND_T;

/* Generic source types. Values from */
/* 240..256 are resevrved.           */
#define TM_SRC_TYPE_SAME_AS_ROOT    ((TM_SRC_TYPE_T)  0)
#define TM_SRC_TYPE_SAME_AS_PARENT  ((TM_SRC_TYPE_T)  1)
#define TM_SRC_TYPE_CONN_HANDLE     ((TM_SRC_TYPE_T)  2)
#define TM_SRC_TYPE_MPEG_2_BRDCST   ((TM_SRC_TYPE_T)  3)
#define TM_SRC_TYPE_MPEG_2_OOB      ((TM_SRC_TYPE_T)  4)
#define TM_SRC_TYPE_FILE_NAME       ((TM_SRC_TYPE_T)  5)
#define TM_SRC_TYPE_CLIENT          ((TM_SRC_TYPE_T)  6)


/* Get types must only use the first 24 bits. The upper */
/* 8 bits are reserved for flags and other purposes.    */
/* Generic get operations. */
#define TM_GET_TYPE_IGNORE                        ((TM_GET_TYPE_T)   0)
#define TM_GET_TYPE_UPDATE_TIME                   ((TM_GET_TYPE_T)   1)
#define TM_GET_TYPE_NUM_MON_ENTRIES               ((TM_GET_TYPE_T)   2)
#define TM_GET_TYPE_MON_ENTRY                     ((TM_GET_TYPE_T)   3)
#define TM_GET_TYPE_TBL_ID                        ((TM_GET_TYPE_T)  32)
#define TM_GET_TYPE_TBL_EXT_ID                    ((TM_GET_TYPE_T)  33)
#define TM_GET_TYPE_VERSION                       ((TM_GET_TYPE_T)  34)
#define TM_GET_TYPE_NUM_MAIN_BODY_DESCRS          ((TM_GET_TYPE_T)  35)
#define TM_GET_TYPE_LEN_MAIN_BODY_DESCRS          ((TM_GET_TYPE_T)  36)
#define TM_GET_TYPE_MAIN_BODY_DESCRS              ((TM_GET_TYPE_T)  37)
#define TM_GET_TYPE_NUM_FIRST_LOOP_ENTRIES        ((TM_GET_TYPE_T)  38)
#define TM_GET_TYPE_NUM_FIRST_LOOP_DESCRS         ((TM_GET_TYPE_T)  39)
#define TM_GET_TYPE_LEN_FIRST_LOOP_DESCRS         ((TM_GET_TYPE_T)  40)
#define TM_GET_TYPE_FIRST_LOOP_DESCRS             ((TM_GET_TYPE_T)  41)
#define TM_GET_TYPE_NUM_SECOND_LOOP_ENTRIES       ((TM_GET_TYPE_T)  42)
#define TM_GET_TYPE_NUM_SECOND_LOOP_DESCRS        ((TM_GET_TYPE_T)  43)
#define TM_GET_TYPE_LEN_SECOND_LOOP_DESCRS        ((TM_GET_TYPE_T)  44)
#define TM_GET_TYPE_SECOND_LOOP_DESCRS            ((TM_GET_TYPE_T)  45)
#define TM_GET_TYPE_RAW_SECT                      ((TM_GET_TYPE_T)  46)
#define TM_GET_TYPE_PRIORITY                      ((TM_GET_TYPE_T)  47)
#define TM_GET_TYPE_NUM_SECOND_LOOP_MULTI_DESCRS  ((TM_GET_TYPE_T)  48)
#define TM_GET_TYPE_LEN_SECOND_LOOP_MULTI_DESCRS  ((TM_GET_TYPE_T)  49)
#define TM_GET_TYPE_SECOND_LOOP_MULTI_DESCRS      ((TM_GET_TYPE_T)  50)
#define TM_GET_TYPE_LAST_ENTRY                    ((TM_GET_TYPE_T) 256)

#define TM_GET_TYPE_ENG_ALLOC_HANDLE         ((TM_GET_TYPE_T)   0x80000000)

#define TM_GET_TYPE_MASK                     ((TM_GET_TYPE_T)   0x00ffffff)

/* Load types must only use the first 24 bits. The upper */
/* 8 bits are reserved for flags and other purposes.     */
/* Generic load operations. */
#define TM_LOAD_TYPE_IGNORE                  (((TM_LOAD_TYPE_T)          0) | TM_LOAD_TYPE_SINGLE_SRC)
#define TM_LOAD_TYPE_IDX                     ((TM_LOAD_TYPE_T)           1)
#define TM_LOAD_TYPE_TS_DESCR                ((TM_LOAD_TYPE_T)           2)
#define TM_LOAD_TYPE_TS_ID                   (((TM_LOAD_TYPE_T)          3) | TM_LOAD_TYPE_SINGLE_SRC)
#define TM_LOAD_TYPE_USE_PARENT_INFO         ((TM_LOAD_TYPE_T)           4)
#define TM_LOAD_TYPE_NO_VER_CHECK            ((TM_LOAD_TYPE_T)           5)

#define TM_LOAD_TYPE_LAST_ENTRY              ((TM_LOAD_TYPE_T)         256)

#define TM_LOAD_TYPE_SINGLE_SRC              ((TM_LOAD_TYPE_T)  0x80000000)
#define TM_LOAD_TYPE_FEED_SECTION            ((TM_LOAD_TYPE_T)  0x40000000)

#define TM_LOAD_TYPE_MASK                    ((TM_LOAD_TYPE_T)  0x00ffffff)

/* Monitor types must only use the first 24 bits. The upper */
/* 8 bits are reserved for flags and other purposes.        */
/* Generic monitor operations. */
#define TM_MON_TYPE_IGNORE                   (((TM_MON_TYPE_T)           0) | TM_MON_TYPE_SINGLE_SRC)
#define TM_MON_TYPE_LAST_ENTRY               ((TM_MON_TYPE_T)          256)

#define TM_MON_TYPE_SINGLE_SRC               ((TM_MON_TYPE_T)   0x80000000)

#define TM_MON_TYPE_MASK                     ((TM_MON_TYPE_T)   0x00ffffff)

/* Parse types must only use the first 24 bits. The upper */
/* 8 bits are reserved for flags and other purposes.      */
/* Generic parse operations. */
#define TM_PARSE_TYPE_IGNORE                        ((TM_PARSE_TYPE_T)   0)
#define TM_PARSE_TYPE_MAIN_BODY_ALL_DESCRS          ((TM_PARSE_TYPE_T)   1)
#define TM_PARSE_TYPE_MAIN_BODY_DESCRS              ((TM_PARSE_TYPE_T)   2)
#define TM_PARSE_TYPE_FIRST_LOOP_ALL_DESCRS         ((TM_PARSE_TYPE_T)   3)
#define TM_PARSE_TYPE_FIRST_LOOP_DESCRS             ((TM_PARSE_TYPE_T)   4)
#define TM_PARSE_TYPE_SECOND_LOOP_ALL_DESCRS        ((TM_PARSE_TYPE_T)   5)
#define TM_PARSE_TYPE_SECOND_LOOP_DESCRS            ((TM_PARSE_TYPE_T)   6)
#define TM_PARSE_TYPE_MAIN_BODY_ALL_DATA            ((TM_PARSE_TYPE_T)   7)
#define TM_PARSE_TYPE_FIRST_LOOP_ALL_DATA           ((TM_PARSE_TYPE_T)   8)
#define TM_PARSE_TYPE_SECOND_LOOP_ALL_MULTI_DESCRS  ((TM_PARSE_TYPE_T)   9)
#define TM_PARSE_TYPE_SECOND_LOOP_MULTI_DESCRS      ((TM_PARSE_TYPE_T)  10)
#define TM_PARSE_TYPE_LAST_ENTRY                    ((TM_PARSE_TYPE_T) 256)

#define TM_PARSE_TYPE_MASK                   ((TM_PARSE_TYPE_T) 0x00ffffff)

#define TM_SET_TYPE_TBL_ENTRY                ((TM_SET_TYPE_T)            0)
#define TM_SET_TYPE_AVAILABLE                ((TM_SET_TYPE_T)            1)
#define TM_SET_TYPE_MAIN_DESCR               ((TM_SET_TYPE_T)            2)
#define TM_SET_TYPE_VERSION_NUM              ((TM_SET_TYPE_T)            3)
#define TM_SET_TYPE_PRIORITY                 ((TM_SET_TYPE_T)            4)
#define TM_SET_TYPE_SEC_MASK_CLR             ((TM_SET_TYPE_T)            5) 
#define TM_SET_TYPE_LAST_ENTRY               ((TM_SET_TYPE_T)          256)

#define TM_SET_TYPE_MASK                     ((TM_SET_TYPE_T)   0x00ffffff)

/* Store types must only use the first 24 bits. The upper */
/* 8 bits are reserved for flags and other purposes.      */
/* Generic store operations. */
#define TM_STORE_TYPE_IGNORE                 ((TM_STORE_TYPE_T)          0)
#define TM_STORE_TYPE_FILE_NAME              ((TM_STORE_TYPE_T)          1)
#define TM_STORE_TYPE_LAST_ENTRY             ((TM_STORE_TYPE_T)        256)

#define TM_STORE_TYPE_MASK                   ((TM_STORE_TYPE_T) 0x00ffffff)

/* Unavailable reasons. */
#define TM_UNAVAIL_REASON_AS_REQUESTED           ((UINT32)  0)
#define TM_UNAVAIL_REASON_ROOT_CLOSE             ((UINT32)  1)
#define TM_UNAVAIL_REASON_PARENT_LOST            ((UINT32)  2)
#define TM_UNAVAIL_REASON_OUT_OF_RESOURCES       ((UINT32)  3)
#define TM_UNAVAIL_REASON_OUTSIDE_TIME_BRACKETS  ((UINT32)  4)
#define TM_UNAVAIL_REASON_OP_COMPLETED           ((UINT32)  5)

/*------------------------------------------------------------------*/
/*! @struct TM_SECOND_LOOP_ALL_DESCRS_T
 *  @brief  tm 2nd loop description structure
 
 *  @code
 * typedef struct _TM_SECOND_LOOP_ALL_DESCRS_T
 * {
 *     UINT16  ui2_loop_one_idx;
 *     UINT16  ui2_loop_two_idx;
 * }   TM_SECOND_LOOP_ALL_DESCRS_T;
 *  @endcode
 
 *  @li@c    ui2_loop_one_idx            - loop one idx
 *  @li@c    ui2_loop_two_idx            - loop two idx
 */
/*------------------------------------------------------------------*/
/* Allows parsing of second loop descriptors. */
typedef struct _TM_SECOND_LOOP_ALL_DESCRS_T
{
    UINT16  ui2_loop_one_idx;
    UINT16  ui2_loop_two_idx;
}   TM_SECOND_LOOP_ALL_DESCRS_T;

/*------------------------------------------------------------------*/
/*! @struct TM_SECOND_LOOP_DESCR_SEL_T
 *  @brief  tm 2nd loop description select structure
 
 *  @code
 * typedef struct _TM_SECOND_LOOP_DESCR_SEL_T
 * {
 *     UINT16  ui2_loop_one_idx;
 *     UINT16  ui2_loop_two_idx;
 *     UINT8*  pui1_descr_tag_list;
 *     SIZE_T  z_num_descr_tags;
 * }   TM_SECOND_LOOP_DESCR_SEL_T;
 *  @endcode
 
 *  @li@c    ui2_loop_one_idx            - loop one idx
 *  @li@c    ui2_loop_two_idx            - loop two idx
 *  @li@c    pui1_descr_tag_list         - pointer of tag list
 *  @li@c    z_num_descr_tags            - number of tag descriptior
 */
/*------------------------------------------------------------------*/
typedef struct _TM_SECOND_LOOP_DESCR_SEL_T
{
    UINT16  ui2_loop_one_idx;
    UINT16  ui2_loop_two_idx;
    UINT8*  pui1_descr_tag_list;
    SIZE_T  z_num_descr_tags;
}   TM_SECOND_LOOP_DESCR_SEL_T;

/*------------------------------------------------------------------*/
/*! @struct TM_SECOND_LOOP_ALL_MULTI_DESCRS_T
 *  @brief  tm 2nd loop multi-description structure
 
 *  @code
 * typedef struct _TM_SECOND_LOOP_ALL_MULTI_DESCRS_T
 * {
 *     UINT16  ui2_loop_one_idx;
 *     UINT16  ui2_loop_two_idx;
 *     UINT16  ui2_loop_two_set_idx;
 * }   TM_SECOND_LOOP_ALL_MULTI_DESCRS_T;
 *  @endcode
 
 *  @li@c    ui2_loop_one_idx            - loop one idx
 *  @li@c    ui2_loop_two_idx            - loop two idx
 *  @li@c    ui2_loop_two_set_idx        - loop two select idx
 */
/*------------------------------------------------------------------*/
typedef struct _TM_SECOND_LOOP_ALL_MULTI_DESCRS_T
{
    UINT16  ui2_loop_one_idx;
    UINT16  ui2_loop_two_idx;
    UINT16  ui2_loop_two_set_idx;
}   TM_SECOND_LOOP_ALL_MULTI_DESCRS_T;

/*------------------------------------------------------------------*/
/*! @struct TM_SECOND_LOOP_MULTI_DESCR_SEL_T
 *  @brief  tm 2nd loop multi-description select structure
 
 *  @code
 * typedef struct _TM_SECOND_LOOP_MULTI_DESCR_SEL_T
 * {
 *     UINT16  ui2_loop_one_idx;
 *     UINT16  ui2_loop_two_idx;
 *     UINT16  ui2_loop_two_set_idx;
 *     UINT8*  pui1_descr_tag_list;
 *     SIZE_T  z_num_descr_tags;
 * }   TM_SECOND_LOOP_MULTI_DESCR_SEL_T;
 *  @endcode
 
 *  @li@c    ui2_loop_one_idx            - loop one idx
 *  @li@c    ui2_loop_two_idx            - loop two idx
 *  @li@c    ui2_loop_two_set_idx        - loop two select idx
 *  @li@c    pui1_descr_tag_list         - pointer of descrition tag list
 *  @li@c    z_num_descr_tags            - number of descrition tag list
 */
/*------------------------------------------------------------------*/
typedef struct _TM_SECOND_LOOP_MULTI_DESCR_SEL_T
{
    UINT16  ui2_loop_one_idx;
    UINT16  ui2_loop_two_idx;
    UINT16  ui2_loop_two_set_idx;
    UINT8*  pui1_descr_tag_list;
    SIZE_T  z_num_descr_tags;
}   TM_SECOND_LOOP_MULTI_DESCR_SEL_T;

/*------------------------------------------------------------------*/
/*! @struct TM_FIRST_LOOP_DESCR_SEL_T
 *  @brief  tm 1st loop description select structure
 
 *  @code
 * typedef struct _TM_FIRST_LOOP_DESCR_SEL_T
 * {
 *     UINT16  ui2_loop_idx;
 * 
 *     UINT8*  pui1_descr_tag_list;
 * 
 *     SIZE_T  z_num_descr_tags;
}   TM_FIRST_LOOP_DESCR_SEL_T;
 *  @endcode
 
 *  @li@c    ui2_loop_idx                - loop index
 *  @li@c    pui1_descr_tag_list         - pointer of descrition tag list
 *  @li@c    z_num_descr_tags            - number of descrition tag list
 */
/*------------------------------------------------------------------*/

/* Allows parsing of first loop descriptors. */
typedef struct _TM_FIRST_LOOP_DESCR_SEL_T
{
    UINT16  ui2_loop_idx;

    UINT8*  pui1_descr_tag_list;

    SIZE_T  z_num_descr_tags;
}   TM_FIRST_LOOP_DESCR_SEL_T;

/*------------------------------------------------------------------*/
/*! @struct TM_MAIN_BODY_DESCR_SEL_T
 *  @brief  tm main loop description select structure
 
 *  @code
 * typedef struct _TM_MAIN_BODY_DESCR_SEL_T
 * {
 *     UINT8*  pui1_descr_tag_list;
 * 
 *     SIZE_T  z_num_descr_tags;
 * }   TM_MAIN_BODY_DESCR_SEL_T;
 *  @endcode
 
 *  @li@c    pui1_descr_tag_list         - pointer of descrition tag list
 *  @li@c    z_num_descr_tags            - number of descrition tag list
 */
/*------------------------------------------------------------------*/
/* Allows parsing of main body descriptors. */
typedef struct _TM_MAIN_BODY_DESCR_SEL_T
{
    UINT8*  pui1_descr_tag_list;

    SIZE_T  z_num_descr_tags;
}   TM_MAIN_BODY_DESCR_SEL_T;

/*------------------------------------------------------------------*/
/*! @struct TM_UPDATE_TIME_T
 *  @brief  tm update time structure
 
 *  @code
 * typedef struct _TM_UPDATE_TIME_T
 * {
 *     TIME_T  t_store_time;
 *     TIME_T  t_update_time;
 * }   TM_UPDATE_TIME_T;
 *  @endcode
 
 *  @li@c    t_store_time            - store time
 *  @li@c    t_update_time           - update time
 */
/*------------------------------------------------------------------*/
/* Update time. */
typedef struct _TM_UPDATE_TIME_T
{
    TIME_T  t_store_time;
    TIME_T  t_update_time;
}   TM_UPDATE_TIME_T;

/*------------------------------------------------------------------*/
/*! @struct TM_BUFFER_T
 *  @brief  tm buffer structure
 
 *  @code
 * typedef struct
 * {
 *     UINT8* pui1_buffer;
 *     SIZE_T z_buffer_len;
 * } TM_BUFFER_T;
 *  @endcode
 
 *  @li@c    pui1_buffer            - pointer of buffer
 *  @li@c    z_buffer_len           - buffer length
 */
/*------------------------------------------------------------------*/
/* buffer */
typedef struct
{
    UINT8* pui1_buffer;
    SIZE_T z_buffer_len;
} TM_BUFFER_T;


/*----------------------------------------------------------------------------*/
/*! @brief  tm notify function

 *  @param  [in] h_obj            - handle of tm objst
 *  @param  [in] e_obj_type       - tm object type
 *  @param  [in] e_nfy_cond       - notify condition
 *  @param  [in] pv_nfy_tag       - notify tag
 *  @param  [in] ui4_data         - notify data
 *  
 *  @note   

 *  @see
 *  @return		NFY_RET_T
 *  @retval     NFY_RET_INVALID                           - invalid
 *  @retval     NFY_RET_PROCESSED                         - processed
 *  @retval     NFY_RET_RETRY                             - retry
 *  @retval     NFY_RET_DSM_TAP_XLAT_SYNC_SAME_TS_PID     - sync same ts pid
 *  @retval     NFY_RET_DSM_TAP_XLAT_SYNC_TS_PID_SET      - sync ts pid set
 */
/*----------------------------------------------------------------------------*/
/* Notify function. */
typedef NFY_RET_T (*x_tm_nfy_fct) (HANDLE_T       h_obj,
                                   HANDLE_TYPE_T  e_obj_type,
                                   TM_COND_T      e_nfy_cond,
                                   VOID*          pv_nfy_tag,
                                   UINT32         ui4_data);
/*----------------------------------------------------------------------------*/
/*! @brief  tm parse function

 *  @param  [in] h_obj            - handle of tm objst
 *  @param  [in] pui1_obj_data    - pointer of tm object
 *  @param  [in] z_obj_data_len   - tm object data length
 *  @param  [in] ui2_count        - object count
 *  @param  [in] pv_data          - pointer of data
 *  @param  [in] z_data_len       - data length
 *  
 *  @note   

 *  @see
 *  @return		PARSE_RET_T
 *  @retval     PARSE_RET_ERROR                         - error
 *  @retval     PARSE_RET_END                           - end
 *  @retval     PARSE_RET_NEXT                          - next
 *  @retval     PARSE_RET_RESTART                       - restart
 */
/*----------------------------------------------------------------------------*/
/* Parsing callback function. */
typedef PARSE_RET_T (*x_tm_parse_fct) (HANDLE_T      h_obj,
                                       const UINT8*  pui1_obj_data,
                                       SIZE_T        z_obj_data_len,
                                       UINT16        ui2_count,
                                       VOID*         pv_data,
                                       SIZE_T        z_data_len);

/* Return values. */
#define TMR_OK                    ((INT32)   0)
#define TMR_NOT_INIT              ((INT32)  -1)
#define TMR_ALREADY_INIT          ((INT32)  -2)
#define TMR_INV_ARG               ((INT32)  -3)
#define TMR_INV_HANDLE            ((INT32)  -4)
#define TMR_INV_HANDLE_TYPE       ((INT32)  -5)
#define TMR_OUT_OF_HANDLES        ((INT32)  -6)
#define TMR_INV_NAME              ((INT32)  -7)
#define TMR_DUPLICATE_NAME        ((INT32)  -8)
#define TMR_UNKNOWN_NAME          ((INT32)  -9)
#define TMR_OUT_OF_RESOURCES      ((INT32) -10)
#define TMR_INV_OBJ_TYPE          ((INT32) -11)
#define TMR_OBJ_DEL_NOT_ALLOWED   ((INT32) -12)
#define TMR_NO_SUPPORT            ((INT32) -13)
#define TMR_INV_HANDLE_STATE      ((INT32) -14)
#define TMR_NOT_ENOUGH_SPACE      ((INT32) -15)
#define TMR_INV_GET_TYPE          ((INT32) -16)
#define TMR_INV_GET_INFO          ((INT32) -17)
#define TMR_INV_LOAD_TYPE         ((INT32) -18)
#define TMR_INV_LOAD_INFO         ((INT32) -19)
#define TMR_INV_FLAG_SETTING      ((INT32) -20)
#define TMR_INV_MON_TYPE          ((INT32) -21)
#define TMR_INV_MON_INFO          ((INT32) -22)
#define TMR_INV_PARSE_TYPE        ((INT32) -23)
#define TMR_INV_PARSE_INFO        ((INT32) -24)
#define TMR_PARSE_FCT_FAILURE     ((INT32) -25)
#define TMR_INV_STORE_TYPE        ((INT32) -26)
#define TMR_INV_STORE_INFO        ((INT32) -27)
#define TMR_INV_SRC_TYPE          ((INT32) -28)
#define TMR_INV_SRC_INFO          ((INT32) -29)
#define TMR_SINGLE_SRC_REQUIRED   ((INT32) -30)
#define TMR_INV_ENG_HANDLE        ((INT32) -31)
#define TMR_SRC_PARTIAL_OVERLAP   ((INT32) -32)
#define TMR_AEE_OUT_OF_RESOURCES  ((INT32) -33)
#define TMR_AEE_NO_RIGHTS         ((INT32) -34)
#define TMR_OUTSIDE_TIME_BRACKETS ((INT32) -35)
#define TMR_INV_SET_TYPE          ((INT32) -36)
#define TMR_INV_SET_INFO          ((INT32) -37)
#define TMR_TIMEOUT               ((INT32) -38)
#define TMR_UNAVAILABLE           ((INT32) -39)
#define TMR_DECOMPRESS_ERROR      ((INT32) -40)

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_TBL_MNGR            */
/*----------------------------------------------------------------------------*/



#endif /* _U_TM_H */
