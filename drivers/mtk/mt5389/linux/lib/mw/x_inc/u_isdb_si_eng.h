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
/*! @file u_isdb_si_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains ARIB-SI engine specific definitions,
 *         which are exported.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_ISDB_SI_H_
#define _U_ISDB_SI_H_    

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle_grp.h"
#include "u_tm.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_TBL_MNGR_ISDB_SI_ENG ISDB SI Engine
 *
 *  @ingroup groupMW_TBL_MNGR
 *  @brief Load and get ISDB table information for Application
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could load or get
 *   the detail information of ISDB table
 *   
 *  @see groupMW_TBL_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define ISDB_SI_DEFAULT_NAME        "isdb_si_engine"    /**< Define the default ISDB si engine name.*/


/* ARIB-SI engine handle types. */
#define TMT_ISDB_BAT                (TMT_ISDB + ((HANDLE_TYPE_T)  0))    /**<  The handle to load BAT table type */
#define TMT_ISDB_NIT                (TMT_ISDB + ((HANDLE_TYPE_T)  1))    /**<  The handle to load NIT table type */
#define TMT_ISDB_SDT                (TMT_ISDB + ((HANDLE_TYPE_T)  2))    /**<  The handle to load SDT table type */
#define TMT_ISDB_HEIT_PF            (TMT_ISDB + ((HANDLE_TYPE_T)  3))    /**<  The handle to load High EIT present and following table type */
#define TMT_ISDB_HEIT_S             (TMT_ISDB + ((HANDLE_TYPE_T)  4))    /**<  The handle to load High EIT schedule table type */
#define TMT_ISDB_TDT                (TMT_ISDB + ((HANDLE_TYPE_T)  5))    /**<  The handle to load TDT table type */
#define TMT_ISDB_TOT                (TMT_ISDB + ((HANDLE_TYPE_T)  6))    /**<  The handle to load TOT table type */
#define TMT_ISDB_RST                (TMT_ISDB + ((HANDLE_TYPE_T)  7))    /**<  The handle to load RST table type */

#define TMT_ISDB_BAT_MON            (TMT_ISDB + ((HANDLE_TYPE_T)  8))    /**<  The handle to monitor BAT table type */
#define TMT_ISDB_NIT_ACTUAL_MON     (TMT_ISDB + ((HANDLE_TYPE_T)  9))    /**<  The handle to monitor actual NIT table type */
#define TMT_ISDB_NIT_OTHER_MON      (TMT_ISDB + ((HANDLE_TYPE_T) 10))    /**<  The handle to monitor other NIT table type */
#define TMT_ISDB_SDT_ACTUAL_MON     (TMT_ISDB + ((HANDLE_TYPE_T) 11))    /**<  The handle to monitor actual SDT table type */
#define TMT_ISDB_SDT_OTHER_MON      (TMT_ISDB + ((HANDLE_TYPE_T) 12))    /**<  The handle to monitor other SDT table type */

#define TMT_ISDB_TXT                (TMT_ISDB + ((HANDLE_TYPE_T) 13))    /**<  The handle to load text or name type */

#define TMT_ISDB_PCAT               (TMT_ISDB + ((HANDLE_TYPE_T)  14))    /**<  The handle to load PCAT table type */
#define TMT_ISDB_BIT                (TMT_ISDB + ((HANDLE_TYPE_T)  15))    /**<  The handle to load BIT table type */
#define TMT_ISDB_NBIT               (TMT_ISDB + ((HANDLE_TYPE_T)  16))    /**<  The handle to load NBIT table type */
#define TMT_ISDB_LDT                (TMT_ISDB + ((HANDLE_TYPE_T)  17))    /**<  The handle to load LDT table type */
#define TMT_ISDB_LEIT               (TMT_ISDB + ((HANDLE_TYPE_T)  18))    /**<  The handle to load lower EIT table type */
#define TMT_ISDB_MEIT               (TMT_ISDB + ((HANDLE_TYPE_T)  19))    /**<  The handle to load middleware EIT table type */

#define TMT_ISDB_LIT                (TMT_ISDB + ((HANDLE_TYPE_T)  20))    /**<  The handle to load LIT table type */
#define TMT_ISDB_ERT                (TMT_ISDB + ((HANDLE_TYPE_T)  21))    /**<  The handle to load ERT table type */
#define TMT_ISDB_ITT                (TMT_ISDB + ((HANDLE_TYPE_T)  22))    /**<  The handle to load ITT table type */

#define TMT_ISDB_CDT                (TMT_ISDB + ((HANDLE_TYPE_T)  23))    /**<  The handle to load CDT table type */
#define TMT_ISDB_LSDTT              (TMT_ISDB + ((HANDLE_TYPE_T)  24))    /**<  The handle to load lower SDTT table type */
#define TMT_ISDB_HSDTT              (TMT_ISDB + ((HANDLE_TYPE_T)  25))    /**<  The handle to load high SDTT table type */

#define TMT_ISDB_TXT_STATE          (TMT_ISDB + ((HANDLE_TYPE_T) 26))/*bliu add */    /**<   The handle to load text or name state type */

#define TMT_ISDB_LAST_ENTRY         (TMT_ISDB + ((HANDLE_TYPE_T) 27))    /**<  The last handle ISDB table type      */


/* ARIB-SI engine get operations. */
#define ISDB_SI_GET_TYPE_TS_IDS                  \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 0))    /**<   The operation to get transport stream ID type*/

#define ISDB_SI_GET_TYPE_TS_ENTRY                \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 1))    /**<  The operation to get transport stream entry type*/

#define ISDB_SI_GET_TYPE_SVC_IDS                 \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 2))    /**<  The operation to get service ID type*/

#define ISDB_SI_GET_TYPE_SVC_ENTRY               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 3))    /**<  The operation to get service entry type */

#define ISDB_SI_GET_TYPE_EVT_ENTRY               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 4))    /**<  The operation to get event entry type */

#define ISDB_SI_GET_TYPE_ERS_ENTRY               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 5))    /**<  The operation to get ERS type*/

#define ISDB_SI_GET_TYPE_SCHED_END_TIME          \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 6))    /**<  The operation to get EIT schedule end time type */

#define ISDB_SI_GET_TYPE_TXT_DESCR               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 7))    /**<  The operation to get text description type*/

#define ISDB_SI_GET_TYPE_TXT                     \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 8))    /**<  The operation to get text type */

#define ISDB_SI_GET_TYPE_TXT_LEN                 \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 9))    /**<   The operation to get the length of text type*/

#define ISDB_SI_GET_TYPE_SHORT_TXT               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 10))    /**<  The operation to get short text type */

#define ISDB_SI_GET_TYPE_SHORT_TXT_LEN           \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 11))    /**<  The operation to get the length of  short text type */

#define ISDB_SI_GET_TYPE_TXT_STYLE               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 12))    /**<  The operation to get the style of text type */

#define ISDB_SI_GET_TYPE_TXT_STYLE_SIZE          \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 13))    /**<  The operation to get the size of text style type*/

#define ISDB_SI_GET_TYPE_BRDCSTER_ENTRY          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  14))    /**< The operation to get broadcaster entry type */

#define ISDB_SI_GET_TYPE_LDESC_ENTRY          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  15))    /**<  The operation to get the desc of LDT table type */

#define ISDB_SI_GET_TYPE_NBINFO_ENTRY          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  16))    /**<  The operation to get NBIT informatin type */

#define ISDB_SI_GET_TYPE_NBINFO_ENTRY_KEY          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  17))    /**<  The operation to get the key of NBIT information type */

#define ISDB_SI_GET_TYPE_PCONT_ENTRY          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  16))    /**<  The operation to get content of PCAT table type*/

#define ISDB_SI_GET_TYPE_PCONT_ENTRY_SDESC          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  17))    /**<  The operation to get the content of PCAT entry type */
    
#define ISDB_SI_GET_TYPE_ARIB_EVT_ENTRY          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  18))    /**<  The operation to get the ISDB event entry type*/



#define ISDB_SI_GET_TYPE_ERNODE_ENTRY          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  19))    /**<  The operation to get the node of ERT table type*/

#define ISDB_SI_GET_TYPE_LEVT_ID          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  20))    /**<  The operation to get the event ID of LIT table type*/

#define ISDB_SI_GET_TYPE_EVT_RLTN_ENTRY          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  21))    /**<  The operation to get event relation entry type*/

#define ISDB_SI_GET_TYPE_NUM_LOGO_ENTRY          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  22))    /**<  The operation to get the count of logo type*/
    
#define ISDB_SI_GET_TYPE_LOGO_ENTRY          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  23))    /**<  The operation to get logo entry type*/

#define ISDB_SI_GET_TYPE_LOGO_DATA           \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  24))    /**<  The operation to get logo data type*/


#define ISDB_SI_GET_TYPE_CCONT_ENTRY          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  25))    /**<  The operation to get the content of PCAT table entry type*/

#define ISDB_SI_GET_TYPE_SCONT_ENTRY_SDESC           \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  26))    /**< The operation to get the descriptor of SDTT table type*/

#define ISDB_SI_GET_ON_TS_SVC_ID               \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  27))    /**<  The operation to get the service locator(ONID,TSID and ServicID) type*/

#define ISDB_SI_GET_TYPE_EVT_EXT_COND          \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  28))    /**< The operation to get the event extend condition type*/
    /* dyin: 081030 */


/* ARIB-SI engine load types. */
#define ISDB_SI_LOAD_TYPE_BQ_ID                  \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  0))    /**<  The operation to load BIT by bouquet ID type */

#define ISDB_SI_LOAD_TYPE_NW_ID                  \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  1))    /**<  The operation to load NIT by network ID type*/

#define ISDB_SI_LOAD_TYPE_ON_TS_ID               \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  2))    /**<  The operation to load SDT by ONID and TSID type */

#define ISDB_SI_LOAD_TYPE_TS_DESCR               \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  3))    /**<  The operation to load SDT by transport stream type */

#define ISDB_SI_LOAD_TYPE_ON_TS_SVC_ID           \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  4))    /**< The operation to load EIT present and following by ONID, TSID and service ID type */

#define ISDB_SI_LOAD_TYPE_ON_TS_SVC_ID_TIME      \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  5))    /**<  The operation to load EIT schedule by service locator and time range type*/

#define ISDB_SI_LOAD_TYPE_TS_DESCR_SVC_ID        \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  6))    /**< The operation to load EIT present and following by transport stream descriptor and service ID type */

#define ISDB_SI_LOAD_TYPE_TS_DESCR_SVC_ID_TIME   \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  7))    /**< The operation to load EIT schedule by transport stream descriptor and service ID and time range type */

#define ISDB_SI_LOAD_TYPE_IDX_SVC_ID             \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  8))    /**<  The operation to load EIT present and following by NIT loop index and service ID type */

#define ISDB_SI_LOAD_TYPE_IDX_SVC_ID_TIME        \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  9))    /**<  The operation to load EIT schedule by NIT loop index and service ID and time range type */

#define ISDB_SI_LOAD_TYPE_IDX_TIME               \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 10))    /**< The operation to load EIT schedule by SDT loop index and time range type */

#define ISDB_SI_LOAD_TYPE_SVC_ID                 \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 11))    /**<  The operation to load EIT present and following by SDT loop index type*/

#define ISDB_SI_LOAD_TYPE_SVC_ID_TIME            \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 12))    /**<  The operation to load EIT schedule by NIT loop and service ID and range time type*/

#define ISDB_SI_LOAD_TYPE_NIT_ACTUAL             \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 13))    /**<  The operation to load NIT actual type*/

#define ISDB_SI_LOAD_TYPE_SDT_ACTUAL             \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 14))    /**< The operation to load SDT actual type*/
    
#define ISDB_SI_LOAD_TYPE_ON_ID                  \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  15))    /**< The operation to load CDT by ONID type*/

#define ISDB_SI_LOAD_TYPE_ON_TS_SVC_CONTENT_ID      \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  16))    /**<  The operation to load PCAT by ONID, TSID, service ID and content ID type */



#define ISDB_SI_LOAD_TYPE_EVT_RELATION_ID           \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  17))    /**< The operation to load LDT by event relation ID type */

#define ISDB_SI_LOAD_TYPE_ON_TS_SVC_EVENT_ID        \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  18))    /**<  The operation to load ERT by ONID,TSID,service ID and event ID type */

#define ISDB_SI_LOAD_TYPE_EVT_ID                    \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)   19))    /**< The operation to load ITT by a event ID type */

#define ISDB_SI_LOAD_TYPE_ON_DD_ID                  \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)   20))    /**<  The operation to load CDT by ONID and download data ID type */

#define ISDB_SI_LOAD_TYPE_MAKER_MODEL_ID                  \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)   21))    /**<  The operation to load SDTT by maker and model ID type */




/* ARIB-SI engine configuration structure. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_CONFIG_T
 *  @brief It has all config information for ISDB SI engine.
 *  @code
 *  typedef struct _ISDB_SI_CONFIG_T
 *  {
 *      UINT8   ui1_num_bat_filters;
 *      UINT8   ui1_num_nit_filters;
 *      UINT8   ui1_num_sdt_filters;
 *      UINT8   ui1_num_heit_filters;
 *      UINT8   ui1_num_tdt_filters;
 *      UINT8   ui1_num_tot_filters;
 *      UINT8   ui1_num_rst_filters;
 *      UINT8   ui1_num_pcat_filters;
 *      UINT8   ui1_num_bit_filters;
 *      UINT8   ui1_num_nbit_filters;
 *      UINT8   ui1_num_ldt_filters;
 *      UINT8   ui1_num_leit_filters;
 *      UINT8   ui1_num_meit_filters;
 *      UINT8   ui1_num_lit_filters;
 *      UINT8   ui1_num_ert_filters;
 *      UINT8   ui1_num_itt_filters;
 *      UINT8   ui1_num_cdt_filters;
 *      UINT8   ui1_num_lsdtt_filters;
 *      UINT8   ui1_num_hsdtt_filters;
 *      SIZE_T  z_bat_buff_size;
 *      SIZE_T  z_nit_buff_size;
 *      SIZE_T  z_sdt_buff_size;
 *      SIZE_T  z_heit_buff_size;
 *      SIZE_T  z_tdt_buff_size;
 *      SIZE_T  z_tot_buff_size;
 *      SIZE_T  z_rst_buff_size;
 *      SIZE_T  z_pcat_buff_size;
 *      SIZE_T  z_bit_buff_size;
 *      SIZE_T  z_nbit_buff_size;
 *      SIZE_T  z_ldt_buff_size;
 *      SIZE_T  z_leit_buff_size;
 *      SIZE_T  z_meit_buff_size;
 *      SIZE_T  z_lit_buff_size;
 *      SIZE_T  z_ert_buff_size;
 *      SIZE_T  z_itt_buff_size;
 *      SIZE_T  z_cdt_buff_size;
 *      SIZE_T  z_lsdtt_buff_size;
 *      SIZE_T  z_hsdtt_buff_size;
 *      UINT16  ui2_num_bat_tbl_objs;
 *      UINT16  ui2_num_nit_tbl_objs;
 *      UINT16  ui2_num_sdt_tbl_objs;
 *      UINT16  ui2_num_heit_tbl_objs;
 *      UINT16  ui2_num_tdt_tbl_objs;
 *      UINT16  ui2_num_tot_tbl_objs;
 *      UINT16  ui2_num_rst_tbl_objs;
 *      UINT16  ui2_num_txt_tbl_objs;
 *      UINT16  ui2_num_pcat_tbl_objs;
 *      UINT16  ui2_num_bit_tbl_objs;
 *      UINT16  ui2_num_nbit_tbl_objs;
 *      UINT16  ui2_num_ldt_tbl_objs;
 *      UINT16  ui2_num_leit_tbl_objs;
 *      UINT16  ui2_num_meit_tbl_objs;
 *      UINT16  ui2_num_lit_tbl_objs;
 *      UINT16  ui2_num_ert_tbl_objs;
 *      UINT16  ui2_num_itt_tbl_objs;
 *      UINT16  ui2_num_cdt_tbl_objs;
 *      UINT16  ui2_num_lsdtt_tbl_objs;
 *      UINT16  ui2_num_hsdtt_tbl_objs;
 *      UINT16  ui2_num_mon_bat_tbl_objs;
 *      UINT16  ui2_num_mon_nit_tbl_objs;
 *      UINT16  ui2_num_mon_sdt_tbl_objs;
 *      UINT8   ui1_num_heit_tbl_objs_per_filter;
 *      UINT32  ui4_heit_tbl_filter_timeout;
 *  }   ISDB_SI_CONFIG_T;
 *  @endcode
 *  @li@c  ui1_num_bat_filters                               - The count of BAT filter.
 *  @li@c  ui1_num_nit_filters                               - The count of NIT filter.
 *  @li@c  ui1_num_sdt_filters                               - The count of SDT filter.
 *  @li@c  ui1_num_heit_filters                              - The count of High EIT filter.
 *  @li@c  ui1_num_tdt_filters                               - The count of TDT filter.
 *  @li@c  ui1_num_tot_filters                               - The count of TOT filter.
 *  @li@c  ui1_num_rst_filters                               - The count of RST filter.
 *  @li@c  ui1_num_pcat_filters                              - The count of PCAT filter.
 *  @li@c  ui1_num_bit_filters                               - The count of BIT filter.
 *  @li@c  ui1_num_nbit_filters                              - The count of NBIT filter.
 *  @li@c  ui1_num_ldt_filters                               - The count of LDT filter.
 *  @li@c  ui1_num_leit_filters                              - The count of lower EIT filter.
 *  @li@c  ui1_num_meit_filters                              - The count of Middle EIT filter.
 *  @li@c  ui1_num_lit_filters                               - The count of LIT filter.
 *  @li@c  ui1_num_ert_filters                               - The count of ERT filter.
 *  @li@c  ui1_num_itt_filters                               - The count of ITT filter.
 *  @li@c  ui1_num_cdt_filters                               - The count of CDT filter.
 *  @li@c  ui1_num_lsdtt_filters                             - The count of lower SDTT filter.
 *  @li@c  ui1_num_hsdtt_filters                             - The count of high SDTT filter.
 *  @li@c  z_bat_buff_size                                   - The buffer size of BAT.
 *  @li@c  z_nit_buff_size                                   - The buffer size of NIT.
 *  @li@c  z_sdt_buff_size                                   - The buffer size of SDT.
 *  @li@c  z_heit_buff_size                                  - The buffer size of High EIT.
 *  @li@c  z_tdt_buff_size                                   - The buffer size of TDT.
 *  @li@c  z_tot_buff_size                                   - The buffer size of TOT.
 *  @li@c  z_rst_buff_size                                   - The buffer size of RST.
 *  @li@c  z_pcat_buff_size                                  - The buffer size of PCAT.
 *  @li@c  z_bit_buff_size                                   - The buffer size of BIT.
 *  @li@c  z_nbit_buff_size                                  - The buffer size of NBIT.
 *  @li@c  z_ldt_buff_size                                   - The buffer size of LDT.
 *  @li@c  z_leit_buff_size                                  - The buffer size of low EIT.
 *  @li@c  z_meit_buff_size                                  - The buffer size of middle EIT.
 *  @li@c  z_lit_buff_size                                   - The buffer size of LIT.
 *  @li@c  z_ert_buff_size                                   - The buffer size of ERT.
 *  @li@c  z_itt_buff_size                                   - The buffer size of ITT.
 *  @li@c  z_cdt_buff_size                                   - The buffer size of CDT.
 *  @li@c  z_lsdtt_buff_size                                 - The buffer size of lower SDTT.
 *  @li@c  z_hsdtt_buff_size                                 - The buffer size of middle SDTT.
 *  @li@c  ui2_num_bat_tbl_objs                              - The count of BAT table object buffer.
 *  @li@c  ui2_num_nit_tbl_objs                              - The count of NIT table object buffer.
 *  @li@c  ui2_num_sdt_tbl_objs                              - The count of SDT table object buffer.
 *  @li@c  ui2_num_heit_tbl_objs                             - The count of high EIT table object buffer.
 *  @li@c  ui2_num_tdt_tbl_objs                              - The count of TDT table object buffer.
 *  @li@c  ui2_num_tot_tbl_objs                              - The count of TOT table object buffer.
 *  @li@c  ui2_num_rst_tbl_objs                              - The count of RST table object buffer.
 *  @li@c  ui2_num_txt_tbl_objs                              - The count of TXT table object buffer.
 *  @li@c  ui2_num_pcat_tbl_objs                             - The count of PCAT table object buffer.
 *  @li@c  ui2_num_bit_tbl_objs                              - The count of BIT table object buffer.
 *  @li@c  ui2_num_nbit_tbl_objs                             - The count of NBIT table object buffer.
 *  @li@c  ui2_num_ldt_tbl_objs                              - The count of LDT table object buffer.
 *  @li@c  ui2_num_leit_tbl_objs                             - The count of lower EIT table object buffer.
 *  @li@c  ui2_num_meit_tbl_objs                             - The count of middle EIT table object buffer.
 *  @li@c  ui2_num_lit_tbl_objs                              - The count of LIT table object buffer.
 *  @li@c  ui2_num_ert_tbl_objs                              - The count of ERT table object buffer.
 *  @li@c  ui2_num_itt_tbl_objs                              - The count of ITT table object buffer.
 *  @li@c  ui2_num_cdt_tbl_objs                              - The count of CDT table object buffer.
 *  @li@c  ui2_num_lsdtt_tbl_objs                            - The count of lower SDTT table object buffer.
 *  @li@c  ui2_num_hsdtt_tbl_objs                            - The count of high SDTT table object buffer.
 *  @li@c  ui2_num_mon_bat_tbl_objs                          - The count of monitor BAT table object buffer.
 *  @li@c  ui2_num_mon_nit_tbl_objs                          - The count of monitor NIT table object buffer.
 *  @li@c  ui2_num_mon_sdt_tbl_objs                          - The count of monitor SDT table object buffer.
 *  @li@c  ui1_num_heit_tbl_objs_per_filter                  - The count of high eit for every filter object buffer.
 *  @li@c  ui4_heit_tbl_filter_timeout                       - The time out of load high EIT table.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_CONFIG_T
{
    UINT8   ui1_num_bat_filters;
    UINT8   ui1_num_nit_filters;
    UINT8   ui1_num_sdt_filters;
    UINT8   ui1_num_heit_filters;
    UINT8   ui1_num_tdt_filters;
    UINT8   ui1_num_tot_filters;
    UINT8   ui1_num_rst_filters;

    UINT8   ui1_num_pcat_filters;
    UINT8   ui1_num_bit_filters;
    UINT8   ui1_num_nbit_filters;
    UINT8   ui1_num_ldt_filters;
    
    UINT8   ui1_num_leit_filters;
    UINT8   ui1_num_meit_filters;

    UINT8   ui1_num_lit_filters;
    UINT8   ui1_num_ert_filters;
    UINT8   ui1_num_itt_filters;

    UINT8   ui1_num_cdt_filters;
    UINT8   ui1_num_lsdtt_filters;
    UINT8   ui1_num_hsdtt_filters;

    SIZE_T  z_bat_buff_size;
    SIZE_T  z_nit_buff_size;
    SIZE_T  z_sdt_buff_size;
    SIZE_T  z_heit_buff_size;
    SIZE_T  z_tdt_buff_size;
    SIZE_T  z_tot_buff_size;
    SIZE_T  z_rst_buff_size;

    SIZE_T  z_pcat_buff_size;
    SIZE_T  z_bit_buff_size;
    SIZE_T  z_nbit_buff_size;
    SIZE_T  z_ldt_buff_size;

    SIZE_T  z_leit_buff_size;
    SIZE_T  z_meit_buff_size;
    
    SIZE_T  z_lit_buff_size;
    SIZE_T  z_ert_buff_size;
    SIZE_T  z_itt_buff_size;

    SIZE_T  z_cdt_buff_size;
    SIZE_T  z_lsdtt_buff_size;
    SIZE_T  z_hsdtt_buff_size;

    UINT16  ui2_num_bat_tbl_objs;
    UINT16  ui2_num_nit_tbl_objs;
    UINT16  ui2_num_sdt_tbl_objs;
    UINT16  ui2_num_heit_tbl_objs;
    UINT16  ui2_num_tdt_tbl_objs;
    UINT16  ui2_num_tot_tbl_objs;
    UINT16  ui2_num_rst_tbl_objs;
    UINT16  ui2_num_txt_tbl_objs;
    
    UINT16  ui2_num_pcat_tbl_objs;
    UINT16  ui2_num_bit_tbl_objs;
    UINT16  ui2_num_nbit_tbl_objs;
    UINT16  ui2_num_ldt_tbl_objs;
    
    UINT16  ui2_num_leit_tbl_objs;
    UINT16  ui2_num_meit_tbl_objs;
    
    UINT16  ui2_num_lit_tbl_objs;
    UINT16  ui2_num_ert_tbl_objs;
    UINT16  ui2_num_itt_tbl_objs;
    
    UINT16  ui2_num_cdt_tbl_objs;
    UINT16  ui2_num_lsdtt_tbl_objs;
    UINT16  ui2_num_hsdtt_tbl_objs;
    
    UINT16  ui2_num_mon_bat_tbl_objs;
    UINT16  ui2_num_mon_nit_tbl_objs;
    UINT16  ui2_num_mon_sdt_tbl_objs;

    UINT8   ui1_num_heit_tbl_objs_per_filter;
    UINT32  ui4_heit_tbl_filter_timeout;
}   ISDB_SI_CONFIG_T;

/* Possible values of ui1_content_indicator of ISDB_SI_CONTENT_VER_ENTRY_INFO_T */
#define PCAT_FLAG_CONTENT_VER_INVALID        ((UINT8) 0x00)    /**< All versions are targeted(Version specification is invalid) */
#define PCAT_FLAG_CONTENT_VER_AFTER          ((UINT8) 0x01)    /**< Version(s) specified or later are targeted. */
#define PCAT_FLAG_CONTENT_VER_BEFORE         ((UINT8) 0x02)    /**< Version(s) specified or earlier are targeted. */
#define PCAT_FLAG_CONTENT_VER_ONLY           ((UINT8) 0x03)    /**< Only specified version is targeted. */

/* CDT logo data information */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_LOGO_INFO_T
 *  @brief The information of CDT logo data.
 *  @code
 *  typedef struct _ISDB_SI_LOGO_INFO_T
 *  {
 *      UINT16    ui2_logo_id;
 *      UINT16    ui2_logo_version;
 *      UINT8     ui1_logo_type;
 *      UINT16    ui2_data_len;
 *  } ISDB_SI_LOGO_INFO_T;
 *  @endcode
 *  @li@c  ui2_logo_id                                       - The ID value of the logo data defined in this service.
 *  @li@c  ui2_logo_version                                  - The version number of this logo ID.
 *  @li@c  ui1_logo_type                                     - The type of this logo ID.
 *  @li@c  ui2_data_len                                      - The PNG data of this logo ID.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_LOGO_INFO_T
{
    UINT16    ui2_logo_id;
    UINT16    ui2_logo_version;
    UINT8     ui1_logo_type;
    UINT16    ui2_data_len;
} ISDB_SI_LOGO_INFO_T;

/* Software Download Trigger Table Entry Information */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_CONTENT_GROUP_ENTRY_INFO_T
 *  @brief Software Download Trigger Table Entry Information.
 *  @code
 *  typedef struct _ISDB_SI_CONTENT_GROUP_ENTRY_INFO_T
 *  {
 *      UINT8     ui1_group_id;
 *      UINT16    ui2_target_ver;
 *      UINT16    ui2_new_ver;
 *      UINT8     ui1_download_level;
 *      UINT8     ui1_ver_indicator;
 *      UINT8     ui1_stime_shift_info;
 *      UINT16    ui2_num_schedule_desc;
 *  } ISDB_SI_CONTENT_GROUP_ENTRY_INFO_T;
 *  @endcode
 *  @li@c  ui1_group_id                                      - The field contains a ID of this group.
 *  @li@c  ui2_target_ver                                    - This field indicates version number of the download content to be updated.
 *  @li@c  ui2_new_ver                                       - This field indicates a version number of content to be downloaded.
 *  @li@c  ui1_download_level                                - "01" indicates compulsory downloading, and "00" indicates discretionary downloading.
 *  @li@c  ui1_ver_indicator                                 - The field contains a vession indicator of this group.
 *  @li@c  ui1_stime_shift_info                              - Explanation of the schedule time-shift information.
 *  @li@c  ui2_num_schedule_desc                             - The count of schedule descriptor.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_CONTENT_GROUP_ENTRY_INFO_T
{
    UINT8     ui1_group_id;
    UINT16    ui2_target_ver;
    UINT16    ui2_new_ver;
    UINT8     ui1_download_level;
    UINT8     ui1_ver_indicator;
    UINT8     ui1_stime_shift_info;
    
    UINT16    ui2_num_schedule_desc;
    
} ISDB_SI_CONTENT_GROUP_ENTRY_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_CONTENT_VER_ENTRY_INFO_T
 *  @brief the information of transmission schedule of partial content data in accumulating type data broadcasting.
 *  @code
 *  typedef struct _ISDB_SI_CONTENT_VER_ENTRY_INFO_T
 *  {
 *      UINT16    ui2_content_ver;
 *      UINT16    ui2_content_min_ver;
 *      UINT8     ui1_content_indicator;
 *      UINT16    ui2_num_schedule_desc;
 *  } ISDB_SI_CONTENT_VER_ENTRY_INFO_T;
 *  @endcode
 *  @li@c  ui2_content_ver                                   - This field indicates the total contents versions of the partial content.
 *  @li@c  ui2_content_min_ver                               - This field indicates partial contents version.
 *  @li@c  ui1_content_indicator                             - The field indicates the meaning related to contents version and contents minor version.
 *  @li@c  ui2_num_schedule_desc                             - The count of the schedule desc.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_CONTENT_VER_ENTRY_INFO_T
{
    UINT16    ui2_content_ver;
    UINT16    ui2_content_min_ver;
    UINT8     ui1_content_indicator;

    UINT16    ui2_num_schedule_desc;
} ISDB_SI_CONTENT_VER_ENTRY_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_SCHEDULE_DESC_INFO_T
 *  @brief The brief information of start time and duration.
 *  @code
 *  typedef struct _ISDB_SI_SCHEDULE_DESC_INFO_T
 *  {
 *      TIME_T   t_start_time;
 *      TIME_T   t_duration;
 *  } ISDB_SI_SCHEDULE_DESC_INFO_T;
 *  @endcode
 *  @li@c  t_start_time                                      - The start time.
 *  @li@c  t_duration                                        - The duration value of this start time.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_SCHEDULE_DESC_INFO_T
{
    TIME_T   t_start_time;
    TIME_T   t_duration;
} ISDB_SI_SCHEDULE_DESC_INFO_T;


/* Possible values of ui1_desc_body_loc_flag of ISDB_SI_NET_BOARD_INFO_T */
#define  NBIT_FLAG_LOC_UNDEF              ((UINT8)0x00)    /**< Undefined. */
#define  NBIT_FLAG_LOC_ACTUAL_TS          ((UINT8)0x01)    /**< Detail information is described in the actual TS table. */
#define  NBIT_FLAG_LOC_SI_PRIME_TS        ((UINT8)0x02)    /**< Detail informaion is described in SI prime TS table. */
#define  NBIT_FLAG_LOC_RESERVED           ((UINT8)0x03)    /**< Reserved for future use. */

/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_NET_BOARD_INFO_T
 *  @brief The information for transmitted board information on network.
 *  @code
 *  typedef struct _ISDB_SI_NET_BOARD_INFO_T
 *  {
 *      UINT16      ui2_info_id;
 *      UINT8       ui1_info_type;
 *      UINT8       ui1_desc_body_loc_flag;
 *      UINT16      ui2_num_keys;
 *  } ISDB_SI_NET_BOARD_INFO_T;
 *  @endcode
 *  @li@c  ui2_info_id                                       - The field indicates ID number of the submitted information.
 *  @li@c  ui1_info_type                                     - The field indicates the submitted information type.
 *  @li@c  ui1_desc_body_loc_flag                            - The field indicates the location of the table where contents of the informaion.
 *  @li@c  ui2_num_keys                                      - The field indicates the number of the key.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_NET_BOARD_INFO_T
{
    UINT16      ui2_info_id;
    UINT8       ui1_info_type;
    UINT8       ui1_desc_body_loc_flag;

    UINT16      ui2_num_keys;
} ISDB_SI_NET_BOARD_INFO_T;



/* Possible values of ui1_flags of ISDB_SI_EVT_ENTRY_INFO_T */
#define ARIB_EVT_FLAG_STATUS_UNDEFINED       ((UINT8) 0x00)    /**<  Undefined */
#define ARIB_EVT_FLAG_NOT_RUNNING            ((UINT8) 0x01)    /**<  Not running. */
#define ARIB_EVT_FLAG_SECONDS_LATER          ((UINT8) 0x02)    /**<  Seconds later.*/
#define ARIB_EVT_FLAG_PAUSING                ((UINT8) 0x03)    /**<  Pausing state. */
#define ARIB_EVT_FLAG_RUNNING                ((UINT8) 0x04)    /**<  Running state. */
#define ARIB_EVT_FLAG_CA_SCRAMBLED           ((UINT8) 0x08)    /**<  CA scrambled state. */

#define ARIB_EVT_MASK_RUNNING_STATUS         ((UINT8) 0x07)    /**<  All running states. */

/* This structure contains all information of an EIT present / following , p/f after or 
   schedule table objects event loop. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_EVT_ENTRY_INFO_T
 *  @brief The information of current event data.
 *  @code
 *  typedef struct _ISDB_SI_EVT_ENTRY_INFO_T
 *  {
 *      UINT16  ui2_evt_id;
 *      TIME_T  t_start_time;
 *      TIME_T  t_duration;
 *      UINT8   ui1_flags;
 *      UINT8   ui1_sect_num;
 *  }   ISDB_SI_EVT_ENTRY_INFO_T;
 *  @endcode
 *  @li@c  ui2_evt_id                                        - The ID of current event.
 *  @li@c  t_start_time                                      - The start time of this event ID.
 *  @li@c  t_duration                                        - The duration time of this event ID.
 *  @li@c  ui1_flags                                         - The running status flag for this event ID.
 *  @li@c  ui1_sect_num                                      - The section number of this event ID.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_EVT_ENTRY_INFO_T
{
    UINT16  ui2_evt_id;

    TIME_T  t_start_time;
    TIME_T  t_duration;

    UINT8   ui1_flags;
    UINT8   ui1_sect_num;/*bliu add*/
}   ISDB_SI_EVT_ENTRY_INFO_T;


/* This structure contains an index and a time, which is used to acquire the 
   schedule data of a specific period. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_IDX_TIME_INFO_T
 *  @brief  This structure contains an index and a time, which is used to acquire the 
 *          schedule data of a specific period. 
 *  @code
 *  typedef struct _ISDB_SI_IDX_TIME_INFO_T
 *  {
 *      UINT16  ui2_idx;
 *      TIME_T  t_time;
 *  }   ISDB_SI_IDX_TIME_INFO_T;
 *  @endcode
 *  @li@c  ui2_idx                                           - The current index.
 *  @li@c  t_time                                            - The time of this index.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_IDX_TIME_INFO_T
{
    UINT16  ui2_idx;
    TIME_T  t_time;
}   ISDB_SI_IDX_TIME_INFO_T;


/* This structure contains an index and a table・s service id value. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_IDX_SVC_ID_INFO_T
 *  @brief This structure contains an index and a table・s service id value.
 *  @code
 *  typedef struct _ISDB_SI_IDX_SVC_ID_INFO_T
 *  {
 *      UINT16  ui2_idx;
 *      UINT16  ui2_svc_id;
 *  }   ISDB_SI_IDX_SVC_ID_INFO_T;
 *  @endcode
 *  @li@c  ui2_idx                                           - The current index.
 *  @li@c  ui2_svc_id                                        - The service ID of this index.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_IDX_SVC_ID_INFO_T
{
    UINT16  ui2_idx;
    UINT16  ui2_svc_id;
}   ISDB_SI_IDX_SVC_ID_INFO_T;


/* This structure contains an index and a table・s service id value. In 
   addition, this structure holds a time, which is used to acquire the
   schedule data of a specific period. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_IDX_SVC_ID_TIME_INFO_T
 *  @brief This structure contains an index and a table・s service id value. 
 *         In addition, this structure holds a time, which is used to acquire
 *         the schedule data of a specific period. 
 *  @code
 *  typedef struct _ISDB_SI_IDX_SVC_ID_TIME_INFO_T
 *  {
 *      UINT16  ui2_idx;
 *      UINT16  ui2_svc_id;
 *      TIME_T  t_time;
 *  }   ISDB_SI_IDX_SVC_ID_TIME_INFO_T;
 *  @endcode
 *  @li@c  ui2_idx                                           - The current index.
 *  @li@c  ui2_svc_id                                        - The service ID of this index.
 *  @li@c  t_time                                            - The time of this index.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_IDX_SVC_ID_TIME_INFO_T
{
    UINT16  ui2_idx;
    UINT16  ui2_svc_id;
    TIME_T  t_time;
}   ISDB_SI_IDX_SVC_ID_TIME_INFO_T;


/* This structure contains a tables original network and transport stream id 
   values. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_ON_TS_ID_INFO_T
 *  @brief This structure contains a tables original network and transport stream id values.
 *  @code
 *  typedef struct _ISDB_SI_ON_TS_ID_INFO_T
 *  {
 *      UINT16  ui2_on_id;
 *      UINT16  ui2_ts_id;
 *  }   ISDB_SI_ON_TS_ID_INFO_T;
 *  @endcode
 *  @li@c  ui2_on_id                                         - The current original network ID.
 *  @li@c  ui2_ts_id                                         - The current transport stream ID.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_ON_TS_ID_INFO_T
{
    UINT16  ui2_on_id;
    UINT16  ui2_ts_id;
}   ISDB_SI_ON_TS_ID_INFO_T;


/* This structure contains a tables original network,  transport stream and 
   service id values. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_ON_TS_SVC_ID_INFO_T
 *  @brief This structure contains a tables original network,  transport stream 
 *         and service id values.
 *  @code
 *  typedef struct _ISDB_SI_ON_TS_SVC_ID_INFO_T
 *  {
 *      UINT16  ui2_on_id;
 *      UINT16  ui2_ts_id;
 *      UINT16  ui2_svc_id;
 *  }   ISDB_SI_ON_TS_SVC_ID_INFO_T;
 *  @endcode
 *  @li@c  ui2_on_id                                         - The original network ID.
 *  @li@c  ui2_ts_id                                         - The transport stream ID.
 *  @li@c  ui2_svc_id                                        - The service ID.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_ON_TS_SVC_ID_INFO_T
{
    UINT16  ui2_on_id;
    UINT16  ui2_ts_id;
    UINT16  ui2_svc_id;
}   ISDB_SI_ON_TS_SVC_ID_INFO_T;


/* This structure contains a tables original network,  transport stream, 
   service and content id values. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_ON_TS_SVC_CONTENT_ID_INFO_T
 *  @brief This structure contains a tables original network, transport 
 *         stream, service and content id values.
 *  @code
 *  typedef struct _ISDB_SI_ON_TS_SVC_CONTENT_ID_INFO_T
 *  {
 *      UINT16  ui2_on_id;
 *      UINT16  ui2_ts_id;
 *      UINT16  ui2_svc_id;
 *      UINT32  ui4_content_id;
 *  }   ISDB_SI_ON_TS_SVC_CONTENT_ID_INFO_T;
 *  @endcode
 *  @li@c  ui2_on_id                                         - The original network ID.
 *  @li@c  ui2_ts_id                                         - The transport stream ID.
 *  @li@c  ui2_svc_id                                        - The service ID.
 *  @li@c  ui4_content_id                                    - The content ID of this locator.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_ON_TS_SVC_CONTENT_ID_INFO_T
{
    UINT16  ui2_on_id;
    UINT16  ui2_ts_id;
    UINT16  ui2_svc_id;
    UINT32  ui4_content_id;
}   ISDB_SI_ON_TS_SVC_CONTENT_ID_INFO_T;


/* This structure contains a tables original network, transport stream and 
   service id values. In addition, this structure holds a time, which is 
   used to acquire the schedule data of a specific period. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_ON_TS_SVC_ID_TIME_INFO_T
 *  @brief This structure contains a tables original network, transport stream
 *         and service id values. In addition, this structure holds a time, 
 *         which is used to acquire the schedule data of a specific period.
 *  @code
 *  typedef struct _ISDB_SI_ON_TS_SVC_ID_TIME_INFO_T
 *  {
 *      UINT16  ui2_on_id;
 *      UINT16  ui2_ts_id;
 *      UINT16  ui2_svc_id;
 *      TIME_T  t_time;
 *  }   ISDB_SI_ON_TS_SVC_ID_TIME_INFO_T;
 *  @endcode
 *  @li@c  ui2_on_id                                         - The original network ID.
 *  @li@c  ui2_ts_id                                         - The transport stream ID.
 *  @li@c  ui2_svc_id                                        - The service ID.
 *  @li@c  t_time                                            - The time for schedule data.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_ON_TS_SVC_ID_TIME_INFO_T
{
    UINT16  ui2_on_id;
    UINT16  ui2_ts_id;
    UINT16  ui2_svc_id;
    TIME_T  t_time;
}   ISDB_SI_ON_TS_SVC_ID_TIME_INFO_T;


/* Possible values of ui1_flags of ISDB_SI_SVC_ENTRY_INFO_T */
#define SVC_FLAG_STATUS_UNDEFINED       ((UINT8) 0x00)    /**<  Undefined. */
#define SVC_FLAG_NOT_RUNNING            ((UINT8) 0x01)    /**<  Not running. */
#define SVC_FLAG_SECONDS_LATER          ((UINT8) 0x02)    /**<  Seconds later. */
#define SVC_FLAG_PAUSING                ((UINT8) 0x03)    /**<  Pause state. */
#define SVC_FLAG_RUNNING                ((UINT8) 0x04)    /**<  Running state. */
#define SVC_FLAG_CA_SCRAMBLED           ((UINT8) 0x08)    /**<  CA srcambled. */
#define SVC_FLAG_HEIT_S_FLAG             ((UINT8) 0x10)    /**<  It contains the high EIT present following. */
#define SVC_FLAG_HEIT_PF_FLAG            ((UINT8) 0x20)    /**<  It contains the high EIT schedule. */

#define SVC_MASK_RUNNING_STATUS         ((UINT8) 0x07)    /**<  All running status mask. */

/* This structure contains all information of a SDT table objects service 
   loop. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_SVC_ENTRY_INFO_T
 *  @brief This structure contains all information of a SDT table objects 
 *         service loop.
 *  @code
 *  typedef struct _ISDB_SI_SVC_ENTRY_INFO_T
 *  {
 *      UINT16  ui2_svc_id;
 *      UINT8   ui1_flags;
 *      UINT8   ui1_user_def_flags;
 *  }   ISDB_SI_SVC_ENTRY_INFO_T;
 *  @endcode
 *  @li@c  ui2_svc_id                                        - The service ID.
 *  @li@c  ui1_flags                                         - The status of current service ID.
 *  @li@c  ui1_user_def_flags                                - The flag of user defined.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_SVC_ENTRY_INFO_T
{
    UINT16  ui2_svc_id;
    UINT8   ui1_flags;
    
    UINT8   ui1_user_def_flags;/*low three bits are used*/
}   ISDB_SI_SVC_ENTRY_INFO_T;

/* This structure contains all information of an event.*/
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_ON_TS_SVC_EVT_INFO_T
 *  @brief This structure contains all information of an event.
 *  @code
 *  typedef struct _ISDB_SI_ON_TS_SVC_EVT_INFO_T
 *  {
 *      UINT16  ui2_ts_id;
 *      UINT16  ui2_on_id;
 *      UINT16  ui2_svc_id;
 *      UINT16  ui2_evt_id;
 *  } ISDB_SI_ON_TS_SVC_EVT_INFO_T;
 *  @endcode
 *  @li@c  ui2_ts_id                                         - The transport stream ID.
 *  @li@c  ui2_on_id                                         - The original network ID.
 *  @li@c  ui2_svc_id                                        - The service ID.
 *  @li@c  ui2_evt_id                                        - The event ID.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_ON_TS_SVC_EVT_INFO_T
{
    UINT16  ui2_ts_id;
    UINT16  ui2_on_id;
    UINT16  ui2_svc_id;
    UINT16  ui2_evt_id;
    
} ISDB_SI_ON_TS_SVC_EVT_INFO_T;

/*This structure contains loading information of LIT and ITT*/
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_PROGRAM_IDX_INFO_T
 *  @brief This structure contains loading information of LIT and ITT.
 *  @code
 *  typedef struct _ISDB_SI_PROGRAM_IDX_INFO_T
 *  {
 *      union
 *      {
 *          ISDB_SI_ON_TS_SVC_EVT_INFO_T    t_on_ts_svc_evt_id;
 *          UINT16                          ui2_idx;
 *          UINT16                          ui2_evt_id;
 *      } u;
 *      MPEG_2_PID_T    t_pid;
 *  } ISDB_SI_PROGRAM_IDX_INFO_T;
 *  @endcode
 *  @li@c  t_on_ts_svc_evt_id                                - The locator of current event.
 *  @li@c  ui2_idx                                           - The index of current event.
 *  @li@c  ui2_evt_id                                        - The current event ID.
 *  @li@c  t_pid                                             - The PID of current LIT or ITT.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_PROGRAM_IDX_INFO_T
{
    union
    {
        ISDB_SI_ON_TS_SVC_EVT_INFO_T    t_on_ts_svc_evt_id;
        UINT16                          ui2_idx;
        UINT16                          ui2_evt_id;
    } u;
    MPEG_2_PID_T    t_pid;
} ISDB_SI_PROGRAM_IDX_INFO_T;


/*This structure contains loading information of ERT*/
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_ERT_LOADING_INFO_T
 *  @brief This structure contains loading information of ERT.
 *  @code
 *  typedef struct _ISDB_SI_ERT_LOADING_INFO_T
 *  {
 *      UINT16          ui2_info_provider_id;
 *      UINT16          ui2_evt_relation_id;
 *      MPEG_2_PID_T    t_pid;
 *  } ISDB_SI_ERT_LOADING_INFO_T;
 *  @endcode
 *  @li@c  ui2_info_provider_id                              - The provider information ID.
 *  @li@c  ui2_evt_relation_id                               - The event relation ID.
 *  @li@c  t_pid                                             - The PID of current ERT.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_ERT_LOADING_INFO_T
{
    UINT16          ui2_info_provider_id;
    UINT16          ui2_evt_relation_id;
    MPEG_2_PID_T    t_pid;
    
} ISDB_SI_ERT_LOADING_INFO_T;

/*This structure contains the Event Relation table loop information*/
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_EVT_RELATION_NODE_INFO_T
 *  @brief This structure contains the Event Relation table loop information
 *  @code
 *  typedef struct _ISDB_SI_EVT_RELATION_NODE_INFO_T
 *  {
 *      UINT8       ui1_relation_type;
 *      UINT16      ui2_node_id;
 *      UINT8       ui1_collection_mode;
 *      UINT16      ui2_parent_node_id;
 *      UINT8       ui1_refer_num;
 *  } ISDB_SI_EVT_RELATION_NODE_INFO_T;
 *  @endcode
 *  @li@c  ui1_relation_type                                 - The relation type.
 *  @li@c  ui2_node_id                                       - The node ID.
 *  @li@c  ui1_collection_mode                               - The collection mode of ERT.
 *  @li@c  ui2_parent_node_id                                - The parent node ID.
 *  @li@c  ui1_refer_num                                     - The reference number.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_EVT_RELATION_NODE_INFO_T
{
    UINT8       ui1_relation_type;
    
    UINT16      ui2_node_id;
    UINT8       ui1_collection_mode;
    UINT16      ui2_parent_node_id;
    UINT8       ui1_refer_num;
} ISDB_SI_EVT_RELATION_NODE_INFO_T;

/* This structure contains a table・s generic TS description and a service 
   id value. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_TS_DESCR_SVC_ID_INFO_T
 *  @brief This structure contains a table・s generic TS description 
 *         and a service id value.
 *  @code
 *  typedef struct _ISDB_SI_TS_DESCR_SVC_ID_INFO_T
 *  {
 *      TS_DESCR_T  t_ts_descr;
 *      UINT16  ui2_svc_id;
 *  }   ISDB_SI_TS_DESCR_SVC_ID_INFO_T;
 *  @endcode
 *  @li@c  t_ts_descr                                        - The transport stream information.
 *  @li@c  ui2_svc_id                                        - The ID of service in current TS.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_TS_DESCR_SVC_ID_INFO_T
{
    TS_DESCR_T  t_ts_descr;

    UINT16  ui2_svc_id;
}   ISDB_SI_TS_DESCR_SVC_ID_INFO_T;


/* This structure contains a table・s generic TS description and a service 
   id values. In addition, this structure holds a time, which is used to 
   acquire the schedule data of a specific period. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_TS_DESCR_SVC_ID_TIME_INFO_T
 *  @brief This structure contains a table・s generic TS description and 
 *         a service id values. In addition, this structure holds a time, 
 *         which is used to acquire the schedule data of a specific period.
 *  @code
 *  typedef struct _ISDB_SI_TS_DESCR_SVC_ID_TIME_INFO_T
 *  {
 *      TS_DESCR_T  t_ts_descr;
 *      UINT16  ui2_svc_id;
 *      TIME_T  t_time;
 *  }   ISDB_SI_TS_DESCR_SVC_ID_TIME_INFO_T;
 *  @endcode
 *  @li@c  t_ts_descr                                        - The transport stream information.
 *  @li@c  ui2_svc_id                                        - The ID of service in current TS.
 *  @li@c  t_time                                            - The time information.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_TS_DESCR_SVC_ID_TIME_INFO_T
{
    TS_DESCR_T  t_ts_descr;

    UINT16  ui2_svc_id;
    TIME_T  t_time;
}   ISDB_SI_TS_DESCR_SVC_ID_TIME_INFO_T;


/* This structure contains a table・s service id value and a time, which 
   is used to acquire the schedule data of a specific period. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_SVC_ID_TIME_INFO_T
 *  @brief This structure contains a table・s service id value and a time,
 *         which is used to acquire the schedule data of a specific period.
 *  @code
 *  typedef struct _ISDB_SI_SVC_ID_TIME_INFO_T
 *  {
 *      UINT16  ui2_svc_id;
 *      TIME_T  t_time;
 *  }   ISDB_SI_SVC_ID_TIME_INFO_T;
 *  @endcode
 *  @li@c  ui2_svc_id                                        - The service ID.
 *  @li@c  t_time                                            - The time information.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_SVC_ID_TIME_INFO_T
{
    UINT16  ui2_svc_id;
    TIME_T  t_time;
}   ISDB_SI_SVC_ID_TIME_INFO_T;


/* This structure contains the end time of HEIT-S information, which is 
   just an estimated value if the b_true_end_time value is FALSE and 
   clients should load this period of schedule information in order to get 
   the true end time. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_SCHED_END_TIME_INFO_T
 *  @brief This structure contains the end time of HEIT-S information, 
 *         which is just an estimated value if the b_true_end_time value 
 *         is FALSE and clients should load this period of schedule 
 *         information in order to get the true end time.
 *  @code
 *  typedef struct _ISDB_SI_SCHED_END_TIME_INFO_T 
 *  { 
 *      TIME_T  t_time;
 *      BOOL    b_true_end_time; 
 *  }   ISDB_SI_SCHED_END_TIME_INFO_T; 
 *  @endcode
 *  @li@c  t_time                                            - The end time of current event schedule.
 *  @li@c  b_true_end_time                                   - The flag of end time.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_SCHED_END_TIME_INFO_T 
{ 
    TIME_T  t_time;
    BOOL    b_true_end_time; 
}   ISDB_SI_SCHED_END_TIME_INFO_T; 


/* This structure contains all information of a RST table. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_EVT_RUNNING_STATUS_INFO_T
 *  @brief This structure contains all information of a RST table.
 *  @code
 *  typedef struct _ISDB_SI_EVT_RUNNING_STATUS_INFO_T
 *  {
 *      UINT16  ui2_ts_id;
 *      UINT16  ui2_on_id;
 *      UINT16  ui2_svc_id;
 *      UINT16  ui2_evt_id;
 *      UINT8   ui1_running_status;
 *  }   ISDB_SI_EVT_RUNNING_STATUS_INFO_T;
 *  @endcode
 *  @li@c  ui2_ts_id                                         - The transport stream ID.
 *  @li@c  ui2_on_id                                         - The original network ID.
 *  @li@c  ui2_svc_id                                        - The service ID.
 *  @li@c  ui2_evt_id                                        - The event ID.
 *  @li@c  ui1_running_status                                - The running status of current event.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_EVT_RUNNING_STATUS_INFO_T
{
    UINT16  ui2_ts_id;
    UINT16  ui2_on_id;
    UINT16  ui2_svc_id;
    UINT16  ui2_evt_id;
    UINT8   ui1_running_status;
}   ISDB_SI_EVT_RUNNING_STATUS_INFO_T;


/* Possible values of requested text mode. */
/*------------------------------------------------------------------*/
/*! @enum ISDB_SI_GET_TXT_MODE_T
 *  @brief Possible values of requested text mode.
 *  @code
 *  typedef enum
 *  {
 *      ISDB_SI_GET_TXT_NORMAL,
 *      ISDB_SI_GET_TXT_SHORT
 *  }   ISDB_SI_GET_TXT_MODE_T;
 *  @endcode
 *  @li@c  ISDB_SI_GET_TXT_NORMAL                            - The normal text.
 *  @li@c  ISDB_SI_GET_TXT_SHORT                             - The short text.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ISDB_SI_GET_TXT_NORMAL,
    ISDB_SI_GET_TXT_SHORT
}   ISDB_SI_GET_TXT_MODE_T;

/* This structure contains the UTF-8 string as well as the length of 
   the string. */
/*------------------------------------------------------------------*/
/*! @struct ISDB_SI_TXT_INFO_T
 *  @brief This structure contains the UTF-8 string as well as the 
 *         length of the string.
 *  @code
 *  typedef struct _ISDB_SI_TXT_INFO_T
 *  {
 *      CHAR*               ps_txt;
 *      SIZE_T*             pz_txt_len;
 *      ISO_3166_COUNT_T    t_count_code;
 *  }   ISDB_SI_TXT_INFO_T;
 *  @endcode
 *  @li@c  ps_txt                                            - The buffer for text information.
 *  @li@c  pz_txt_len                                        - The length of current text.
 *  @li@c  t_count_code                                      - The country code of current text.
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_SI_TXT_INFO_T
{
    CHAR*               ps_txt;
    SIZE_T*             pz_txt_len;
    ISO_3166_COUNT_T    t_count_code;
}   ISDB_SI_TXT_INFO_T;

#endif /* _U_ISDB_SI_H */

/*----------------------------------------------------------------------------*/
/*! @} */                              /* end of groupMW_TBL_MNGR_ISDB_SI_ENG */
/*----------------------------------------------------------------------------*/

