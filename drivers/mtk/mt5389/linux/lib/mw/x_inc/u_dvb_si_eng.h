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
/*! @file u_dvb_si_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains DVB-SI engine specific definitions,
 *         which are exported.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_TBL_MNGR_DVB_SI_ENGINE DVB SI Engine
 *  @ingroup groupMW_TBL_MNGR
 *  @brief Provides DVB Service Information operation.
 *  %This module is Table Manager DVB-SI instance.
 *  @see groupMW_TBL_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_DVB_SI_H_
#define _U_DVB_SI_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle_grp.h"
#include "u_tm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define DVB_SI_DEFAULT_NAME     "dvb_si_engine"    /**<        */


/* Character coding definitions */
typedef enum {
    DVB_TEXT_CHAR_CODE_DEFAULT,
    DVB_TEXT_CHAR_CODE_ISO_8859_1,
    DVB_TEXT_CHAR_CODE_ISO_8859_2,
    DVB_TEXT_CHAR_CODE_ISO_8859_3,
    DVB_TEXT_CHAR_CODE_ISO_8859_4,
    DVB_TEXT_CHAR_CODE_ISO_8859_5,
    DVB_TEXT_CHAR_CODE_ISO_8859_6,
    DVB_TEXT_CHAR_CODE_ISO_8859_7,
    DVB_TEXT_CHAR_CODE_ISO_8859_8,
    DVB_TEXT_CHAR_CODE_ISO_8859_9,
    DVB_TEXT_CHAR_CODE_ISO_8859_10,
    DVB_TEXT_CHAR_CODE_ISO_8859_11,
    DVB_TEXT_CHAR_CODE_ISO_8859_12,
    DVB_TEXT_CHAR_CODE_ISO_8859_13,
    DVB_TEXT_CHAR_CODE_ISO_8859_14,
    DVB_TEXT_CHAR_CODE_ISO_8859_15,
    DVB_TEXT_CHAR_CODE_ISO_8859_16,
    DVB_TEXT_CHAR_CODE_ISO_8859_9_HUN,
    DVB_TEXT_CHAR_CODE_ISO_10646_1,
    DVB_TEXT_CHAR_CODE_KSC_5601,
    DVB_TEXT_CHAR_CODE_GB_2312,
    DVB_TEXT_CHAR_CODE_ISO_10646_1_ZH_TW,
    DVB_TEXT_CHAR_CODE_ISO_10646_1_UTF_8,
    DVB_TEXT_CHAR_CODE_INVALID,
}   E_DVB_TEXT_CHAR_TBL;

typedef INT32 (*x_dvb_si_custom_coding_func) (ISO_3166_COUNT_T              t_country_code,
                                              E_DVB_TEXT_CHAR_TBL*          pe_coding_flag);


/* DVB-SI engine handle types. */
#define TMT_DVB_BAT             (TMT_DVB + ((HANDLE_TYPE_T)  0))    /**< handle type for BAT    */
#define TMT_DVB_NIT             (TMT_DVB + ((HANDLE_TYPE_T)  1))    /**< handle type for NIT    */
#define TMT_DVB_SDT             (TMT_DVB + ((HANDLE_TYPE_T)  2))    /**< handle type for SDT    */
#define TMT_DVB_EIT_PF          (TMT_DVB + ((HANDLE_TYPE_T)  3))    /**< handle type for EIT-PF       */
#define TMT_DVB_EIT_S           (TMT_DVB + ((HANDLE_TYPE_T)  4))    /**< handle type for EIT-S       */
#define TMT_DVB_TDT             (TMT_DVB + ((HANDLE_TYPE_T)  5))    /**< handle type for TDT       */
#define TMT_DVB_TOT             (TMT_DVB + ((HANDLE_TYPE_T)  6))    /**< handle type for TOT       */
#define TMT_DVB_RST             (TMT_DVB + ((HANDLE_TYPE_T)  7))    /**< handle type for RST       */
#define TMT_DVB_UNT             (TMT_DVB + ((HANDLE_TYPE_T)  8))    /**< handle type for UNT       */

#define TMT_DVB_BAT_MON         (TMT_DVB + ((HANDLE_TYPE_T)  9))    /**< handle type for monitor BAT       */
#define TMT_DVB_NIT_ACTUAL_MON  (TMT_DVB + ((HANDLE_TYPE_T) 10))    /**< handle type for monitor actual NIT       */
#define TMT_DVB_NIT_OTHER_MON   (TMT_DVB + ((HANDLE_TYPE_T) 11))    /**< handle type for monitor other NIT       */
#define TMT_DVB_SDT_ACTUAL_MON  (TMT_DVB + ((HANDLE_TYPE_T) 12))    /**< handle type for monitor actual SDT       */
#define TMT_DVB_SDT_OTHER_MON   (TMT_DVB + ((HANDLE_TYPE_T) 13))    /**< handle type for monitor other SDT       */
#define TMT_DVB_UNT_MON         (TMT_DVB + ((HANDLE_TYPE_T) 14))    /**< handle type for monitor UNT        */

#define TMT_DVB_TXT             (TMT_DVB + ((HANDLE_TYPE_T) 15))    /**< handle type for DVB text*/

#define TMT_DVB_EIT_PF_SECTIONS  (TMT_DVB + ((HANDLE_TYPE_T)  16))    /**< handle type for EIT-PF sections      */
#define TMT_DVB_EIT_S_SECTIONS   (TMT_DVB + ((HANDLE_TYPE_T)  17))    /**< handle type for EIT-S sections      */

#define TMT_DVB_AIT             (TMT_DVB + ((HANDLE_TYPE_T)  18))    /**< handle type for AIT    */
#define TMT_DVB_AIT_MON         (TMT_DVB + ((HANDLE_TYPE_T)  19))    /**< handle type for monitor AIT    */



#define TMT_DVB_LAST_ENTRY      (TMT_DVB + ((HANDLE_TYPE_T) 20))    /**< Last Entry       */


/* DVB-SI engine get operations. */
#define DVB_SI_GET_TYPE_TS_IDS                  \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 0))    /**< get information, reference to DVB_SI_ON_TS_ID_INFO_T*       */

#define DVB_SI_GET_TYPE_TS_ENTRY                \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 1))    /**< get information, reference to DVB_SI_ON_TS_ID_INFO_T*      */

#define DVB_SI_GET_TYPE_SVC_IDS                 \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 2))    /**< get information, reference to DVB_SI_ON_TS_SVC_ID_INFO_T*      */

#define DVB_SI_GET_TYPE_SVC_ENTRY               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 3))    /**< get information, reference to DVB_SI_ON_TS_SVC_ID_INFO_T*      */

#define DVB_SI_GET_TYPE_EVT_ENTRY               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 4))    /**< get information, reference to DVB_SI_EVT_ENTRY_INFO_T*      */

#define DVB_SI_GET_TYPE_ERS_ENTRY               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 5))    /**< get information, reference to DVB_SI_EVT_RUNNING_STATUS_INFO_T*      */

#define DVB_SI_GET_TYPE_SCHED_END_TIME          \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 6))    /**< get information, reference to DVB_SI_SCHED_END_TIME_INFO_T*      */

#define DVB_SI_GET_TYPE_TXT_DESCR               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 7))    /**< get information, reference to TXT handle      */

#define DVB_SI_GET_TYPE_TXT                     \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 8))    /**< get TXT information       */

#define DVB_SI_GET_TYPE_TXT_LEN                 \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 9))    /**< get TXT length      */

#define DVB_SI_GET_TYPE_SHORT_TXT               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 10))    /**< get TXT short text*/

#define DVB_SI_GET_TYPE_SHORT_TXT_LEN           \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 11))    /**< get TXT short text length       */

#define DVB_SI_GET_TYPE_TXT_STYLE               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 12))    /**< get TXT style       */

#define DVB_SI_GET_TYPE_TXT_STYLE_SIZE          \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 13))    /**< get TXT style length       */

#define DVB_SI_GET_TYPE_OUI                     \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 14))    /**< get UNT's table OUI.     */

#define DVB_SI_GET_TYPE_PROCESSING_ORDER        \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 15))    /**< get an individual UNT service entry       */

#ifdef MW_ISDB_EIT_SUPPORT
#define DVB_SI_GET_TYPE_EVT_EXT_COND           \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 16))    /**< get EIT extend condition      */
#endif

#define DVB_SI_GET_TYPE_APP_ENTRY           \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 17))    /**< get EIT extend condition      */


/* DVB-SI engine monitor types. */
#define DVB_SI_MON_TYPE_PID                     \
    (TM_MON_TYPE_LAST_ENTRY + (((TM_MON_TYPE_T)  0) | TM_MON_TYPE_SINGLE_SRC))    /**< Load type for PID       */


/* DVB-SI engine load types. */
#define DVB_SI_LOAD_TYPE_BQ_ID                  \
    (TM_LOAD_TYPE_LAST_ENTRY + (((TM_LOAD_TYPE_T)  0) | TM_LOAD_TYPE_SINGLE_SRC))    /**< Load type for BQ id       */

#define DVB_SI_LOAD_TYPE_NW_ID                  \
    (TM_LOAD_TYPE_LAST_ENTRY + (((TM_LOAD_TYPE_T)  1) | TM_LOAD_TYPE_SINGLE_SRC))    /**< Load type for network id     */

#define DVB_SI_LOAD_TYPE_ON_TS_ID               \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  2))    /**<  Load type for ON TS id      */

#define DVB_SI_LOAD_TYPE_ON_TS_SVC_ID           \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  3))    /**<  Load type for ON TS SERVICE id      */

#define DVB_SI_LOAD_TYPE_ON_TS_SVC_ID_TIME      \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  4))    /**<   Load type for ON TS SERVICE id and Time information      */

#define DVB_SI_LOAD_TYPE_TS_DESCR_SVC_ID        \
    (TM_LOAD_TYPE_LAST_ENTRY + (((TM_LOAD_TYPE_T)  5) | TM_LOAD_TYPE_SINGLE_SRC))    /**<  Load type for TS description      */

#define DVB_SI_LOAD_TYPE_TS_DESCR_SVC_ID_TIME   \
    (TM_LOAD_TYPE_LAST_ENTRY + (((TM_LOAD_TYPE_T)  6) | TM_LOAD_TYPE_SINGLE_SRC))    /**<   Load type for TS description and time information   */

#define DVB_SI_LOAD_TYPE_IDX_SVC_ID             \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  7))    /**<  Load type for index of service      */

#define DVB_SI_LOAD_TYPE_IDX_SVC_ID_TIME        \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  8))    /**<  Load type for index if service and time information       */

#define DVB_SI_LOAD_TYPE_IDX_TIME               \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)  9))    /**<  Load type for index of time      */

#define DVB_SI_LOAD_TYPE_SVC_ID                 \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 10))    /**<   Load type for service id      */

#define DVB_SI_LOAD_TYPE_SVC_ID_TIME            \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 11))    /**<   Load type for service id and time information     */

#define DVB_SI_LOAD_TYPE_NIT_ACTUAL             \
    (TM_LOAD_TYPE_LAST_ENTRY + (((TM_LOAD_TYPE_T) 12) | TM_LOAD_TYPE_SINGLE_SRC))    /**< Load type for NIT actual       */

#define DVB_SI_LOAD_TYPE_SDT_ACTUAL             \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 13))    /**< Load type for SDT actual        */

#define DVB_SI_LOAD_TYPE_PID_OUI                \
    (TM_LOAD_TYPE_LAST_ENTRY + (((TM_LOAD_TYPE_T) 14) | TM_LOAD_TYPE_SINGLE_SRC))    /**< Load type for  PID and OUI information      */

#define DVB_SI_LOAD_TYPE_PID_APP_TYPE                \
    (TM_LOAD_TYPE_LAST_ENTRY + (((TM_LOAD_TYPE_T) 15)))    /**< Load type for  PID and app type information      */

/*DVB SI global configuration*/
typedef UINT32 DVB_SI_CONFIG_MASK_T;

#define DVB_SI_CONFIG_MASK_CUST_1           (MAKE_BIT_MASK_32(0))
#define DVB_SI_CONFIG_MASK_TRIM_SPACE       (MAKE_BIT_MASK_32(1))



/* DVB-SI engine configuration structure. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_CONFIG_T
 *  @brief DVB service information configure
 *  @code
 *  typedef struct _DVB_SI_CONFIG_T
 *  {
 *      UINT8   ui1_num_bat_filters;
 *      UINT8   ui1_num_nit_filters;
 *      UINT8   ui1_num_sdt_filters;
 *      UINT8   ui1_num_eit_filters;
 *      UINT8   ui1_num_tdt_filters;
 *      UINT8   ui1_num_tot_filters;
 *      UINT8   ui1_num_rst_filters;
 *      UINT8   ui1_num_unt_filters;
 *      SIZE_T  z_bat_buff_size;
 *      SIZE_T  z_nit_buff_size;
 *      SIZE_T  z_sdt_buff_size;
 *      SIZE_T  z_eit_buff_size;
 *      SIZE_T  z_tdt_buff_size;
 *      SIZE_T  z_tot_buff_size;
 *      SIZE_T  z_rst_buff_size;
 *      SIZE_T  z_unt_buff_size;
 *      UINT16  ui2_num_bat_tbl_objs;
 *      UINT16  ui2_num_nit_tbl_objs;
 *      UINT16  ui2_num_sdt_tbl_objs;
 *      UINT16  ui2_num_eit_tbl_objs;
 *      UINT16  ui2_num_tdt_tbl_objs;
 *      UINT16  ui2_num_tot_tbl_objs;
 *      UINT16  ui2_num_rst_tbl_objs;
 *      UINT16  ui2_num_txt_tbl_objs;
 *      UINT16  ui2_num_unt_tbl_objs;
 *      UINT16  ui2_num_mon_bat_tbl_objs;
 *      UINT16  ui2_num_mon_nit_tbl_objs;
 *      UINT16  ui2_num_mon_sdt_tbl_objs;
 *      UINT16  ui2_num_mon_unt_tbl_objs;
 *      UINT8   ui1_num_eit_tbl_objs_per_filter;
 *      UINT32  ui4_eit_tbl_filter_timeout;
 *  }   DVB_SI_CONFIG_T;
 *  @endcode
 *  @li@c  ui1_num_bat_filters                               -filter number of BAT
 *  @li@c  ui1_num_nit_filters                               -filter number of NIT
 *  @li@c  ui1_num_sdt_filters                               -filter number of SDT
 *  @li@c  ui1_num_eit_filters                               -filter number of EIT
 *  @li@c  ui1_num_tdt_filters                               -filter number of TDT
 *  @li@c  ui1_num_tot_filters                               -filter number of TOT
 *  @li@c  ui1_num_rst_filters                               -filter number of RST
 *  @li@c  ui1_num_unt_filters                               -filter number of UNT
 *  @li@c  z_bat_buff_size                                   -section buffer size of BAT
 *  @li@c  z_nit_buff_size                                   -section buffer size of NIT
 *  @li@c  z_sdt_buff_size                                   -section buffer size of SDT
 *  @li@c  z_eit_buff_size                                   -section buffer size of EIT
 *  @li@c  z_tdt_buff_size                                   -section buffer size of TDT
 *  @li@c  z_tot_buff_size                                   -section buffer size of TOT
 *  @li@c  z_rst_buff_size                                   -section buffer size of RST
 *  @li@c  z_unt_buff_size                                   -section buffer size of UNT
 *  @li@c  ui2_num_bat_tbl_objs                              -table object's number of BAT
 *  @li@c  ui2_num_nit_tbl_objs                              -table object's number of NIT
 *  @li@c  ui2_num_sdt_tbl_objs                              -table object's number of SDT
 *  @li@c  ui2_num_eit_tbl_objs                              -table object's number of EIT
 *  @li@c  ui2_num_tdt_tbl_objs                              -table object's number of TDT
 *  @li@c  ui2_num_tot_tbl_objs                              -table object's number of TOT
 *  @li@c  ui2_num_rst_tbl_objs                              -table object's number of RST
 *  @li@c  ui2_num_txt_tbl_objs                              -table object's number of TXT
 *  @li@c  ui2_num_unt_tbl_objs                              -table object's number of UNT
 *  @li@c  ui2_num_mon_bat_tbl_objs                          -monitor table object's number of BAT
 *  @li@c  ui2_num_mon_nit_tbl_objs                          -monitor table object's number of NIT
 *  @li@c  ui2_num_mon_sdt_tbl_objs                          -monitor table object's number of SDT
 *  @li@c  ui2_num_mon_unt_tbl_objs                          -monitor table object's number of UNT
 *  @li@c  ui1_num_eit_tbl_objs_per_filter                   -
 *  @li@c  ui4_eit_tbl_filter_timeout                        -time out of eit filter
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_CONFIG_T
{
    UINT8   ui1_num_bat_filters;
    UINT8   ui1_num_nit_filters;
    UINT8   ui1_num_sdt_filters;
    UINT8   ui1_num_eit_filters;
    UINT8   ui1_num_tdt_filters;
    UINT8   ui1_num_tot_filters;
    UINT8   ui1_num_rst_filters;
    UINT8   ui1_num_unt_filters;
    UINT8   ui1_num_eit_sec_filters;
    UINT8   ui1_num_ait_filters;

    SIZE_T  z_bat_buff_size;
    SIZE_T  z_nit_buff_size;
    SIZE_T  z_sdt_buff_size;
    SIZE_T  z_eit_buff_size;
    SIZE_T  z_tdt_buff_size;
    SIZE_T  z_tot_buff_size;
    SIZE_T  z_rst_buff_size;
    SIZE_T  z_unt_buff_size;
    SIZE_T  z_eit_sec_buff_size;
    SIZE_T  z_ait_buff_size;

    UINT16  ui2_num_bat_tbl_objs;
    UINT16  ui2_num_nit_tbl_objs;
    UINT16  ui2_num_sdt_tbl_objs;
    UINT16  ui2_num_eit_tbl_objs; /*share with EIT section feed*/
    UINT16  ui2_num_tdt_tbl_objs;
    UINT16  ui2_num_tot_tbl_objs;
    UINT16  ui2_num_rst_tbl_objs;
    UINT16  ui2_num_txt_tbl_objs;
    UINT16  ui2_num_unt_tbl_objs;
    UINT16  ui2_num_ait_tbl_objs;

    UINT16  ui2_num_mon_bat_tbl_objs;
    UINT16  ui2_num_mon_nit_tbl_objs;
    UINT16  ui2_num_mon_sdt_tbl_objs;
    UINT16  ui2_num_mon_unt_tbl_objs;
    UINT16  ui2_num_mon_ait_tbl_objs;

    UINT8   ui1_num_eit_tbl_objs_per_filter;
    UINT32  ui4_eit_tbl_filter_timeout;
    DVB_SI_CONFIG_MASK_T  ui4_config_mask;
    x_dvb_si_custom_coding_func    pf_dvb_si_custom_coding_func;
}   DVB_SI_CONFIG_T;


/* Possible values of ui1_flags of DVB_SI_EVT_ENTRY_INFO_T */
#define EVT_FLAG_STATUS_UNDEFINED       ((UINT8) 0x00)    /**< flag undefined       */
#define EVT_FLAG_NOT_RUNNING            ((UINT8) 0x01)    /**< not running       */
#define EVT_FLAG_SECONDS_LATER          ((UINT8) 0x02)    /**< seconds later       */
#define EVT_FLAG_PAUSING                ((UINT8) 0x03)    /**< pause       */
#define EVT_FLAG_RUNNING                ((UINT8) 0x04)    /**< runnine       */
#define EVT_FLAG_CA_SCRAMBLED           ((UINT8) 0x08)    /**< ca scrambled       */

#define EVT_MASK_RUNNING_STATUS         ((UINT8) 0x07)    /**< running status       */

/* This structure contains all information of an EIT present / following or 
   schedule table objects event loop. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_EVT_ENTRY_INFO_T
 *  @brief  This structure contains all information of an EIT present / following or 
 *          schedule table objects event loop.
 *  @code
 *  typedef struct _DVB_SI_EVT_ENTRY_INFO_T
 *  {
 *      UINT16  ui2_evt_id;
 *      TIME_T  t_start_time;
 *      TIME_T  t_duration;
 *      UINT8   ui1_flags;
 *      UINT8   ui1_sect_num;
 *  }   DVB_SI_EVT_ENTRY_INFO_T;
 *  @endcode
 *  @li@c  ui2_evt_id                                        - event id
 *  @li@c  t_start_time                                      - start time
 *  @li@c  t_duration                                        - duration
 *  @li@c  ui1_flags                                         - flag
 *  @li@c  ui1_sect_num                                      - section number
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_EVT_ENTRY_INFO_T
{
    UINT16  ui2_evt_id;

    TIME_T  t_start_time;
    TIME_T  t_duration;

    UINT8   ui1_flags;
    UINT8   ui1_sect_num;
}   DVB_SI_EVT_ENTRY_INFO_T;


/* This structure contains an index and a time, which is used to acquire the 
   schedule data of a specific period. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_IDX_TIME_INFO_T
 *  @brief  This structure contains an index and a time, which is used to acquire the 
 *          schedule data of a specific period.
 *  @code
 *  typedef struct _DVB_SI_IDX_TIME_INFO_T
 *  {
 *      UINT16  ui2_idx;
 *      TIME_T  t_time;
 *  }   DVB_SI_IDX_TIME_INFO_T;
 *  @endcode
 *  @li@c  ui2_idx                                           -index of EIT
 *  @li@c  t_time                                            -schedule time
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_IDX_TIME_INFO_T
{
    UINT16  ui2_idx;
    TIME_T  t_time;
}   DVB_SI_IDX_TIME_INFO_T;


/* This structure contains an index and a table・s service id value. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_IDX_SVC_ID_INFO_T
 *  @brief  This structure contains an index and a table・s service id value.
 *  @code
 *  typedef struct _DVB_SI_IDX_SVC_ID_INFO_T
 *  {
 *      UINT16  ui2_idx;
 *      UINT16  ui2_svc_id;
 *  }   DVB_SI_IDX_SVC_ID_INFO_T;
 *  @endcode
 *  @li@c  ui2_idx                                           -index
 *  @li@c  ui2_svc_id                                        -service id
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_IDX_SVC_ID_INFO_T
{
    UINT16  ui2_idx;
    UINT16  ui2_svc_id;
}   DVB_SI_IDX_SVC_ID_INFO_T;


/* This structure contains an index and a table・s service id value. In 
   addition, this structure holds a time, which is used to acquire the
   schedule data of a specific period. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_IDX_SVC_ID_TIME_INFO_T
 *  @brief  This structure contains an index and a table・s service id value. In 
 *          addition, this structure holds a time, which is used to acquire the
 *          schedule data of a specific period.
 *  @code
 *  typedef struct _DVB_SI_IDX_SVC_ID_TIME_INFO_T
 *  {
 *      UINT16  ui2_idx;
 *      UINT16  ui2_svc_id;
 *      TIME_T  t_time;
 *  }   DVB_SI_IDX_SVC_ID_TIME_INFO_T;
 *  @endcode
 *  @li@c  ui2_idx                                           - 
 *  @li@c  ui2_svc_id                                        -
 *  @li@c  t_time                                            -
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_IDX_SVC_ID_TIME_INFO_T
{
    UINT16  ui2_idx;
    UINT16  ui2_svc_id;
    TIME_T  t_time;
}   DVB_SI_IDX_SVC_ID_TIME_INFO_T;


/* This structure contains a tables original network and transport stream id 
   values. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_ON_TS_ID_INFO_T
 *  @brief  This structure contains a tables original network and transport stream id values.
 *  @code
 *  typedef struct _DVB_SI_ON_TS_ID_INFO_T
 *  {
 *      UINT16  ui2_on_id;
 *      UINT16  ui2_ts_id;
 *  }   DVB_SI_ON_TS_ID_INFO_T;
 *  @endcode
 *  @li@c  ui2_on_id                                         -on_id
 *  @li@c  ui2_ts_id                                         -ts_id
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_ON_TS_ID_INFO_T
{
    UINT16  ui2_on_id;
    UINT16  ui2_ts_id;
}   DVB_SI_ON_TS_ID_INFO_T;


/* This structure contains a tables original network,  transport stream and 
   service id values. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_ON_TS_SVC_ID_INFO_T
 *  @brief  This structure contains a tables original network,  transport stream and 
 *          service id values.
 *  @code
 *  typedef struct _DVB_SI_ON_TS_SVC_ID_INFO_T
 *  {
 *      UINT16  ui2_on_id;
 *      UINT16  ui2_ts_id;
 *      UINT16  ui2_svc_id;
 *  }   DVB_SI_ON_TS_SVC_ID_INFO_T;
 *  @endcode
 *  @li@c  ui2_on_id                                         -on_id
 *  @li@c  ui2_ts_id                                         -ts_is
 *  @li@c  ui2_svc_id                                        -service id
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_ON_TS_SVC_ID_INFO_T
{
    UINT16  ui2_on_id;
    UINT16  ui2_ts_id;
    UINT16  ui2_svc_id;
}   DVB_SI_ON_TS_SVC_ID_INFO_T;


/* This structure contains a tables original network, transport stream and 
   service id values. In addition, this structure holds a time, which is 
   used to acquire the schedule data of a specific period. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_ON_TS_SVC_ID_TIME_INFO_T
 *  @brief  This structure contains a tables original network, transport stream and 
 *          service id values. In addition, this structure holds a time, which is 
 *          used to acquire the schedule data of a specific period. 
 *  @code
 *  typedef struct _DVB_SI_ON_TS_SVC_ID_TIME_INFO_T
 *  {
 *      UINT16  ui2_on_id;
 *      UINT16  ui2_ts_id;
 *      UINT16  ui2_svc_id;
 *      TIME_T  t_time;
 *  }   DVB_SI_ON_TS_SVC_ID_TIME_INFO_T;
 *  @endcode
 *  @li@c  ui2_on_id                                         -on_id
 *  @li@c  ui2_ts_id                                         -ts_id
 *  @li@c  ui2_svc_id                                        -service id
 *  @li@c  t_time                                            -schedule time
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_ON_TS_SVC_ID_TIME_INFO_T
{
    UINT16  ui2_on_id;
    UINT16  ui2_ts_id;
    UINT16  ui2_svc_id;
    TIME_T  t_time;
}   DVB_SI_ON_TS_SVC_ID_TIME_INFO_T;


/* This structure contains a tables original network, transport stream and 
   service id values. In addition, this structure holds a time, which is 
   used to acquire the schedule data of a specific period. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_ON_TS_SVC_ID_SEC_INFO_T
 *  @brief  This structure contains a tables original network, transport stream and 
 *          service id values. In addition, this structure holds a time, which is 
 *          used to acquire the schedule data of a specific period. 
 *  @code
 *  typedef struct _DVB_SI_ON_TS_SVC_ID_SEC_INFO_T
 *  {
 *     UINT16  ui2_on_id;
 *     UINT16  ui2_ts_id;
 *     UINT16  ui2_svc_id;
 *     UINT8   ui1_tbl_id;
 *     UINT8   ui1_sec_num;
 *  }   DVB_SI_ON_TS_SVC_ID_SEC_INFO_T;
 *
 *  @endcode
 *  @li@c  ui2_on_id                                         -on_id
 *  @li@c  ui2_ts_id                                         -ts_id
 *  @li@c  ui2_svc_id                                        -service id
 *  @li@c  
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_ON_TS_SVC_ID_SEC_INFO_T
{
    UINT16  ui2_on_id;
    UINT16  ui2_ts_id;
    UINT16  ui2_svc_id;
    UINT8   ui1_tbl_id;
    UINT8   ui1_sec_num;
    BOOL    b_all_sections;
}   DVB_SI_ON_TS_SVC_ID_SEC_INFO_T;



/* Possible values of ui1_flags of DVB_SI_SVC_ENTRY_INFO_T */
#define SVC_FLAG_STATUS_UNDEFINED       ((UINT8) 0x00)    /**< flag:undefined       */
#define SVC_FLAG_NOT_RUNNING            ((UINT8) 0x01)    /**< flag:not running       */
#define SVC_FLAG_SECONDS_LATER          ((UINT8) 0x02)    /**< flag:second later       */
#define SVC_FLAG_PAUSING                ((UINT8) 0x03)    /**< flag:pause       */
#define SVC_FLAG_RUNNING                ((UINT8) 0x04)    /**< flag:running       */
#define SVC_FLAG_CA_SCRAMBLED           ((UINT8) 0x08)    /**< flag:CA scrambled       */
#define SVC_FLAG_EIT_S_FLAG             ((UINT8) 0x10)    /**< flag:EIT schedule       */
#define SVC_FLAG_EIT_PF_FLAG            ((UINT8) 0x20)    /**< flag:EIT PF      */

#define SVC_MASK_RUNNING_STATUS         ((UINT8) 0x07)    /**< flag:running status       */

/* This structure contains all information of a SDT table objects service 
   loop. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_SVC_ENTRY_INFO_T
 *  @brief  This structure contains all information of a SDT table objects service 
 *          loop.
 *  @code
 *  typedef struct _DVB_SI_SVC_ENTRY_INFO_T
 *  {
 *      UINT16  ui2_svc_id;
 *      UINT8   ui1_flags;
 *  #ifdef MW_ISDB_SUPPORT
 *      UINT8   ui1_user_def_flags;
 *  #endif
 *  }   DVB_SI_SVC_ENTRY_INFO_T;
 *  @endcode
 *  @li@c  ui2_svc_id                                        -service id
 *  @li@c  ui1_flags                                         -flag
 *  @li@c  ui1_user_def_flags                                -use defined flags for ISDB used
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_SVC_ENTRY_INFO_T
{
    UINT16  ui2_svc_id;
    UINT8   ui1_flags;

#ifdef MW_ISDB_SUPPORT
   /* For ISDB: Each broadcaster can define this 3 bits field individually as 
    * the extension to indicate whether it transmits EIT or not.*/ /* dyin: 081030 */
    UINT8   ui1_user_def_flags;/*low three bits are used*/
#endif

}   DVB_SI_SVC_ENTRY_INFO_T;


/* This structure contains a table・s generic TS description and a service 
   id value. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_TS_DESCR_SVC_ID_INFO_T
 *  @brief  This structure contains a table・s generic TS description and a service 
 *  id value.
 *  @code
 *  typedef struct _DVB_SI_TS_DESCR_SVC_ID_INFO_T
 *  {
 *      TS_DESCR_T  t_ts_descr;
 *      UINT16  ui2_svc_id;
 *  }   DVB_SI_TS_DESCR_SVC_ID_INFO_T;
 *  @endcode
 *  @li@c  t_ts_descr                                        -ts descript,reference TS_DESCR_T
 *  @li@c  ui2_svc_id                                        -service id
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_TS_DESCR_SVC_ID_INFO_T
{
    TS_DESCR_T  t_ts_descr;

    UINT16  ui2_svc_id;
}   DVB_SI_TS_DESCR_SVC_ID_INFO_T;


/* This structure contains a table・s generic TS description and a service 
   id values. In addition, this structure holds a time, which is used to 
   acquire the schedule data of a specific period. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_TS_DESCR_SVC_ID_TIME_INFO_T
 *  @brief  This structure contains a table・s generic TS description and a service 
 *          id values. In addition, this structure holds a time, which is used to 
 *          acquire the schedule data of a specific period.
 *  @code
 *  typedef struct _DVB_SI_TS_DESCR_SVC_ID_TIME_INFO_T
 *  {
 *      TS_DESCR_T  t_ts_descr;
 *      UINT16  ui2_svc_id;
 *      TIME_T  t_time;
 *  }   DVB_SI_TS_DESCR_SVC_ID_TIME_INFO_T;
 *  @endcode
 *  @li@c  t_ts_descr                                        -ts descript,reference TS_DESCR_T
 *  @li@c  ui2_svc_id                                        -service id
 *  @li@c  t_time                                            -schedule time
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_TS_DESCR_SVC_ID_TIME_INFO_T
{
    TS_DESCR_T  t_ts_descr;

    UINT16  ui2_svc_id;
    TIME_T  t_time;
}   DVB_SI_TS_DESCR_SVC_ID_TIME_INFO_T;


/* This structure contains a table・s service id value and a time, which 
   is used to acquire the schedule data of a specific period. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_SVC_ID_TIME_INFO_T
 *  @brief  This structure contains a table・s service id value and a time, which 
 *          is used to acquire the schedule data of a specific period. 
 *  @code
 *  typedef struct _DVB_SI_SVC_ID_TIME_INFO_T
 *  {
 *      UINT16  ui2_svc_id;
 *      TIME_T  t_time;
 *  }   DVB_SI_SVC_ID_TIME_INFO_T;
 *  @endcode
 *  @li@c  ui2_svc_id                                        -service id
 *  @li@c  t_time                                            -schedule time
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_SVC_ID_TIME_INFO_T
{
    UINT16  ui2_svc_id;
    TIME_T  t_time;
}   DVB_SI_SVC_ID_TIME_INFO_T;


/* This structure contains the end time of EIT-S information, which is 
   just an estimated value if the b_true_end_time value is FALSE and 
   clients should load this period of schedule information in order to get 
   the true end time. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_SCHED_END_TIME_INFO_T
 *  @brief  This structure contains the end time of EIT-S information, which is 
 *          just an estimated value if the b_true_end_time value is FALSE and 
 *          clients should load this period of schedule information in order to get 
 *          the true end time.
 *  @code
 *  typedef struct _DVB_SI_SCHED_END_TIME_INFO_T 
 *  { 
 *      TIME_T  t_time;
 *      BOOL    b_true_end_time; 
 *  }   DVB_SI_SCHED_END_TIME_INFO_T; 
 *  @endcode
 *  @li@c  t_time                                            -EIT-S end time
 *  @li@c  b_true_end_time                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_SCHED_END_TIME_INFO_T 
{ 
    TIME_T  t_time;
    BOOL    b_true_end_time; 
}   DVB_SI_SCHED_END_TIME_INFO_T; 


/* This structure contains all information of a RST table. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_EVT_RUNNING_STATUS_INFO_T
 *  @brief  This structure contains all information of a RST table.
 *  @code
 *  typedef struct _DVB_SI_EVT_RUNNING_STATUS_INFO_T
 *  {
 *      UINT16  ui2_ts_id;
 *      UINT16  ui2_on_id;
 *      UINT16  ui2_svc_id;
 *      UINT16  ui2_evt_id;
 *      UINT8   ui1_running_status;
 *  }   DVB_SI_EVT_RUNNING_STATUS_INFO_T;
 *  @endcode
 *  @li@c  ui2_ts_id                                         -ts id
 *  @li@c  ui2_on_id                                         -on id
 *  @li@c  ui2_svc_id                                        -service id
 *  @li@c  ui2_evt_id                                        -event id
 *  @li@c  ui1_running_status                                -running status
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_EVT_RUNNING_STATUS_INFO_T
{
    UINT16  ui2_ts_id;
    UINT16  ui2_on_id;
    UINT16  ui2_svc_id;
    UINT16  ui2_evt_id;
    UINT8   ui1_running_status;
}   DVB_SI_EVT_RUNNING_STATUS_INFO_T;


/* Possible values of requested text mode. */
/*------------------------------------------------------------------*/
/*! @enum DVB_SI_GET_TXT_MODE_T
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      DVB_SI_GET_TXT_NORMAL,
 *      DVB_SI_GET_TXT_SHORT
 *  }   DVB_SI_GET_TXT_MODE_T;
 *  @endcode
 *  @li@c  DVB_SI_GET_TXT_NORMAL                             -normal text
 *  @li@c  DVB_SI_GET_TXT_SHORT                              -short text
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DVB_SI_GET_TXT_NORMAL,
    DVB_SI_GET_TXT_SHORT
}   DVB_SI_GET_TXT_MODE_T;

/* This structure contains the UTF-8 string as well as the length of 
   the string. */
/*------------------------------------------------------------------*/
/*! @struct DVB_SI_TXT_INFO_T
 *  @brief  This structure contains the UTF-8 string as well as the length of 
 *          the string.
 *  @code
 *  typedef struct _DVB_SI_TXT_INFO_T
 *  {
 *      CHAR*               ps_txt;
 *      SIZE_T*             pz_txt_len;
 *      ISO_3166_COUNT_T    t_count_code;
 *  }   DVB_SI_TXT_INFO_T;
 *  @endcode
 *  @li@c  ps_txt                                            -UTF8 text buffer
 *  @li@c  pz_txt_len                                        -length
 *  @li@c  t_count_code                                      -country code
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_TXT_INFO_T
{
    CHAR*               ps_txt;
    SIZE_T*             pz_txt_len;
    ISO_3166_COUNT_T    t_count_code;
    E_DVB_TEXT_CHAR_TBL e_char_table;
}   DVB_SI_TXT_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct DVB_SI_PID_OUI_INFO_T
 *  @brief This structure contains PID and OUI information
 *  @code
 *  typedef struct _DVB_SI_PID_OUI_INFO_T
 *  {
 *      MPEG_2_PID_T        t_pid;
 *      UINT32              ui4_oui;
 *  }   DVB_SI_PID_OUI_INFO_T;
 *  @endcode
 *  @li@c  t_pid                                             -pid
 *  @li@c  ui4_oui                                           -oui number
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_PID_OUI_INFO_T
{
    MPEG_2_PID_T        t_pid;
    UINT32              ui4_oui;
}   DVB_SI_PID_OUI_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct DVB_SI_EIT_QUERY_INFO_T
 *  @brief 
 *  @code
 *  typedef struct _DVB_SI_EIT_QUERY_INFO_T
 *  {
 *      BOOL        b_actual_only;
 *      UINT8       ui1_tbl_id;
 *  }   DVB_SI_EIT_QUERY_INFO_T;
 *  @endcode
 *  @li@c 
 *  @li@c  
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_EIT_QUERY_INFO_T
{
    BOOL        b_actual_only;
    UINT8       ui1_tbl_id;
}   DVB_SI_EIT_QUERY_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct DVB_SI_AIT_APP_ID_T
 *  @brief 
 *  @code
 *  typedef struct _DVB_SI_AIT_APP_ID_T
 *  {
 *      UINT32     ui4_org_id;
 *      UINT16     ui2_app_id;
 *  }   DVB_SI_AIT_APP_ID_T;
 *  @endcode
 *  @li@c 
 *  @li@c  
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_AIT_APP_ID_T
{
    UINT32     ui4_org_id;
    UINT16     ui2_app_id;
}   DVB_SI_AIT_APP_ID_T;

/*------------------------------------------------------------------*/
/*! @struct DVB_SI_AIT_APP_ID_CTRL_CODE_T
 *  @brief 
 *  @code
 *  typedef struct _DVB_SI_AIT_APP_ID_CTRL_CODE_T
 *  {
 *      DVB_SI_AIT_APP_ID_T    t_app_id;
 *      UINT8                          ui1_ctrl_code;
 *  }   DVB_SI_AIT_APP_ID_CTRL_CODE_T;
 *  @endcode
 *  @li@c 
 *  @li@c  
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_AIT_APP_ID_CTRL_CODE_T
{
    UINT32                 ui4_org_id;
    UINT16                 ui2_app_id;
    UINT8                  ui1_ctrl_code;
} DVB_SI_AIT_APP_ID_CTRL_CODE_T;



/*------------------------------------------------------------------*/
/*! @struct DVB_SI_PID_APP_TYPE_T
 *  @brief 
 *  @code
 *  typedef struct _DVB_SI_PID_APP_TYPE_T
 *  {
 *      UINT32     ui4_org_id;
 *      UINT16     ui2_app_id;
 *  }   DVB_SI_PID_APP_TYPE_T;
 *  @endcode
 *  @li@c 
 *  @li@c  
 */
/*------------------------------------------------------------------*/
typedef struct _DVB_SI_PID_APP_TYPE_T
{
    MPEG_2_PID_T  t_pid;
    UINT16        ui2_app_type;
}   DVB_SI_PID_APP_TYPE_T;



#endif /* _U_DVB_SI_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                 /* end of groupMW_CONN_MNGR_BRDCST */
/*----------------------------------------------------------------------------*/

