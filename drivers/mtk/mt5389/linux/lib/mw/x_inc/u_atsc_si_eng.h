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
/*! @file u_atsc_si_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains ATSC-SI engine specific definitions,
 *         which are exported.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_ATSC_SI_H_
#define _U_ATSC_SI_H_    /**<        */

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_TBL_MNGR_ATSC_SI_ENG
 *  @{
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle_grp.h"
#include "u_tm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define ATSC_SI_DEFAULT_NAME  "atsc_si_engine"    /**<  The default name of the atsc si engine.    */


/* ATSC-SI engine handle types. */
#define TMT_ATSC_MGT    (TMT_ATSC + ((HANDLE_TYPE_T)  0))    /**<ATSC-SI engine handle types MGT       */
#define TMT_ATSC_STT    (TMT_ATSC + ((HANDLE_TYPE_T)  1))    /**<ATSC-SI engine handle types STT       */
#define TMT_ATSC_RRT    (TMT_ATSC + ((HANDLE_TYPE_T)  2))    /**<ATSC-SI engine handle types RRT       */
#define TMT_ATSC_DCCT   (TMT_ATSC + ((HANDLE_TYPE_T)  3))    /**<ATSC-SI engine handle types DCCT       */
#define TMT_ATSC_DCCSCT (TMT_ATSC + ((HANDLE_TYPE_T)  4))    /**<ATSC-SI engine handle types DCCSCT       */
#define TMT_ATSC_TVCT   (TMT_ATSC + ((HANDLE_TYPE_T)  5))    /**<ATSC-SI engine handle types TVCT       */
#define TMT_ATSC_CVCT   (TMT_ATSC + ((HANDLE_TYPE_T)  6))    /**<ATSC-SI engine handle types CVCT       */
#define TMT_ATSC_EIT    (TMT_ATSC + ((HANDLE_TYPE_T)  7))    /**<ATSC-SI engine handle types EIT       */
#define TMT_ATSC_ETT    (TMT_ATSC + ((HANDLE_TYPE_T)  8))    /**<ATSC-SI engine handle types ETT       */
#define TMT_ATSC_TXT    (TMT_ATSC + ((HANDLE_TYPE_T)  9))    /**<ATSC-SI engine handle types TXT       */
#define TMT_ATSC_EIT_0  (TMT_ATSC + ((HANDLE_TYPE_T) 10))    /**<ATSC-SI engine handle types EIT       */
#define TMT_ATSC_EAS    (TMT_ATSC + ((HANDLE_TYPE_T) 11))    /**<ATSC-SI engine handle types EAS       */

#define TMT_ATSC_LAST_ENTRY (TMT_ATSC + ((HANDLE_TYPE_T) 12))    /**<ATSC-SI engine handle type count        */


/* ATSC-SI engine get operations. */
#define ATSC_SI_GET_TYPE_MGT_ENTRY              \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 0))    /**<Argument pv_get_info contains a value of type UINT16, which contains the index value of table loop entries.*/

#define ATSC_SI_GET_TYPE_VCT_ENTRY              \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 1))    /**<Argument pv_get_info contains a value of type UINT16, which contains the index value of channel loop entries.*/

#define ATSC_SI_GET_TYPE_RRT_ENTRY              \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 2))    /**<Argument pv_get_info contains a value of type UINT16, which contains the index value of deminsion loop entries.*/

#define ATSC_SI_GET_TYPE_EIT_ENTRY              \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 3))    /**<Argument pv_get_info contains a value of type UINT16, which contains the index value of event loop entries.*/

#define ATSC_SI_GET_TYPE_REGION                 \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 4))    /**<Argument pv_get_info is ignored. */

#define ATSC_SI_GET_TYPE_ETM_ID                 \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 5))    /**<Argument pv_get_info is ignored. */

#define ATSC_SI_GET_TYPE_TXT_IDX                \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 6))    /**<Argument pv_get_info contains a value of type UINT16, which contains an index value */

#define ATSC_SI_GET_TYPE_TXT_EXT                \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 7))    /**<Argument pv_get_info is ignored. */

#define ATSC_SI_GET_TYPE_TXT_REGION             \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 8))    /**<Argument pv_get_info is ignored. */

#define ATSC_SI_GET_TYPE_TXT_ABRV_DIM_VAL_IDX   \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 9))    /**<Argument pv_get_info references a structure of type ATSC_SI_TXT_DIM_VAL_IDX_INFO_T, which contains a dimensioin index value and a rating value. */

#define ATSC_SI_GET_TYPE_TXT_DIM_VAL_IDX        \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 10))    /**<Argument pv_get_info references a structure of type ATSC_SI_TXT_DIM_VAL_IDX_INFO_T, which contains a dimensioin index value and a rating value. */

#define ATSC_SI_GET_TYPE_TXT_DESCR              \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 11))    /**<Argument pv_get_info references a buiffer of type const UINT8, which contains the multiple-string-structure object.*/

#define ATSC_SI_GET_TYPE_TXT_NUM_LANG           \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 12))    /**<Argument pv_get_info is ignored. */

#define ATSC_SI_GET_TYPE_TXT_LANG_INFO          \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 13))    /**<Argument pv_get_info contains a value of type UINT16, which contains the index value of language loop entries.*/

#define ATSC_SI_GET_TYPE_TXT_SEG_INFO           \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 14))    /**<Argument pv_get_info references a structure of type ATSC_SI_LANG_SEG_IDX_INFO_T, which contains the language index and segment index*/

#define ATSC_SI_GET_TYPE_TXT                    \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 15))    /**<Argument pv_get_info references a structure of type ATSC_SI_LANG_SEG_IDX_INFO_T, which contains the language index and segment index.*/
    
#define ATSC_SI_GET_TYPE_PROTOCOL_VERSION       \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 16))    /**<Argument pv_get_info is ignored. */
    
#define ATSC_SI_GET_TYPE_EAS_EVT_DATA           \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 17))    /**<Argument pv_get_info is ignored.*/

#define ATSC_SI_GET_TYPE_EAS_EVT_CODE_LEN       \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 18))    /**<Argument pv_get_info is ignored.*/

#define ATSC_SI_GET_TYPE_EAS_EVT_CODE           \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 19))    /**<Argument pv_get_info is ignored */

#define ATSC_SI_GET_TYPE_EAS_LOCATION_ENTRY     \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 20))    /**<Argument pv_get_info contains a value of type UINT16, which contains the index value of EAS location loop entries.*/

#define ATSC_SI_GET_TYPE_EAS_EXCEPTION_ENTRY    \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 21))    /**<Argument pv_get_info contains a value of type UINT16, which contains the index value of EAS exception loop entries. */

#define ATSC_SI_GET_TYPE_EAS_TXT_NATURE_OF_ACTIVATION   \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 22))    /**<Argument pv_get_info is ignored.*/

#define ATSC_SI_GET_TYPE_EAS_TXT_ALERT          \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 23))    /**<Argument pv_get_info is ignored. */


/* ATSC-SI engine load types. */
#define ATSC_SI_LOAD_TYPE_REGION                \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 0))    /**<Argument pv_load_info contains a value of type UINT8, which contains the region id.        */

#define ATSC_SI_LOAD_TYPE_IDX_TIME              \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 1))    /**<Argument pv_load_info references a structure of type ATSC_SI_IDX_TIME_INFO_T, which contains the index in VCT channel loop and time the requested EIT covers.        */

#define ATSC_SI_LOAD_TYPE_SRC_ID_TIME           \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 2))    /**<Argument pv_load_info references a structure of type ATSC_SI_SRC_ID_TIME_INFO_T, which contains the source id and time the requested EIT covers. */

#define ATSC_SI_LOAD_TYPE_SRC_ID                \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 3))    /**<Argument pv_load_info contains a value of type UINT16, which contains the source id value. */

#define ATSC_SI_LOAD_TYPE_SRC_EVT_ID_TIME       \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 4))    /**<Argument pv_load_info references a structure of type ATSC_SI_SRC_EVT_ID_TIME_INFO_T, which contains the source id, event id and time the event ETT covers. */

#define ATSC_SI_LOAD_TYPE_EVT_ID                \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 5))    /**<Argument pv_load_info contains a value of type UINT16, which contains the event id value. */
    
#define ATSC_SI_LOAD_TYPE_TS_DESCR_WO_TIME_CHECK    \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 6))    /**<Argument pv_load_info references a structure of type TS_DESCR_T, which contains the broadcast medium, broadcast type, on id, and ts id. */


/* ATSC-SI engine set operations. */
#define ATSC_SI_SET_TYPE_FILTER_BY_TS_ID                  \
    (TM_SET_TYPE_LAST_ENTRY + ((TM_SET_TYPE_T) 1))    /**<ATSC_SI engine set filter by TS id        */


/* Holds values of a MGT loop entry. */
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_MGT_INFO_T
 *  @brief It contains the information of the MGT table.
 *  @code
 *  typedef struct _ATSC_SI_MGT_INFO_T
 *  {
 *      UINT32  ui4_table_size;
 *      UINT16  ui2_table_type;
 *      MPEG_2_PID_T  t_table_pid;
 *      UINT8  ui1_table_version;
 *  }   ATSC_SI_MGT_INFO_T;
 *  @endcode
 *  @li@c  ui4_table_size                                    - This field contains the size of the table type.
 *  @li@c  ui2_table_type                                    - This field contains the table type value, must be MGT 
 *  @li@c  t_table_pid                                       - This field contains the PID value of the table type,MGT table pid value
 *  @li@c  ui1_table_version                                 - This field contains the version value of the table type,MGT table version
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_MGT_INFO_T
{
    UINT32  ui4_table_size;

    UINT16  ui2_table_type;
    
    MPEG_2_PID_T  t_table_pid;

    UINT8  ui1_table_version;
}   ATSC_SI_MGT_INFO_T;


/* Possible values of ui1_flags of ATSC_SI_VCT_INFO_T */
#define VCT_FLAG_ETM_LOC_NONE           ((UINT8) 0x00)    /**<This flag indicates that there¡¦s no ETM for the virutal channel.        */
#define VCT_FLAG_ETM_LOC_THIS_PSIP      ((UINT8) 0x01)    /**<This flag indicates that the ETM for the virutal channel is located in the same physical transmission channel which this VCT table resides in        */
#define VCT_FLAG_ETM_LOC_CHANNEL_TS_ID  ((UINT8) 0x02)    /**<This flag indicates that the ETM for the virutal channel is located in the physical transmission channel specified by the channel TS id.        */
#define VCT_FLAG_ACCESS_CONTROLLED      ((UINT8) 0x04)    /**<This flag indicates whether or not the events associated with this virutal channel is access controlled        */
#define VCT_FLAG_HIDDEN                 ((UINT8) 0x08)    /**<This flag indicates whether or not the virtual channel is hidden and skipped when the user is channel surfing.        */
#define VCT_FLAG_HIDE_GUIDE             ((UINT8) 0x10)    /**<This flag indicates whether or not the hidden virtual channel (the VCT_FLAG_HIDDEN flag is also set) and its events appear in EPG displays.        */

#define VCT_MASK_ETM_LOC                ((UINT8) 0x03)    /**<The following defines a mask for VCT ETM location.        */

/* Holds values of a CVCT or TVCT loop entry. */
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_VCT_INFO_T
 *  @brief It contains the information of the VCT table
 *  @code
 *  typedef struct _ATSC_SI_VCT_INFO_T
 *  {
 *      UINT32  ui4_freq;
 *      UINT16  ui2_major_ch_num;
 *      UINT16  ui2_minor_ch_num;
 *      UINT16  ui2_ts_id;
 *      UINT16  ui2_svc_id;
 *      UINT16  ui2_src_id;
 *      UINT8  ui1_svc_type;   
 *      UINT8  ui1_mod_mode;
 *      UINT8  ui1_flags;
 *  }   ATSC_SI_VCT_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                          -This field contains the frequency.
 *  @li@c  ui2_major_ch_num                                  -This field contains the major channel number of the virtual channel.
 *  @li@c  ui2_minor_ch_num                                  -This field contains the minor channel number of the virtual channel.
 *  @li@c  ui2_ts_id                                         -This field contains the transport stream id.
 *  @li@c  ui2_svc_id                                        -This field contains the service id (program number).
 *  @li@c  ui2_src_id                                        -This field contains the source id.
 *  @li@c  ui1_svc_type                                      -This field contains the service type.
 *  @li@c  ui1_mod_mode                                      -This field contains the modulation mode.
 *  @li@c  ui1_flags                                         -This field contains the properties of the virtual channel. See VCT flags in ¡§Constants, enumerations, and macros¡¨ section for details.
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_VCT_INFO_T
{
    UINT32  ui4_freq;

    UINT16  ui2_major_ch_num;
    UINT16  ui2_minor_ch_num;

    UINT16  ui2_ts_id;
    UINT16  ui2_svc_id;

    UINT16  ui2_src_id;
    
    UINT8  ui1_svc_type;   /* [ffr] Will be changed to a common enumeration in the future. */
    
    UINT8  ui1_mod_mode;

    UINT8  ui1_flags;
}   ATSC_SI_VCT_INFO_T;

/* Holds values of a RRT first loop entry. */
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_RRT_INFO_T
 *  @brief It contains the information of the RRT.
 *  @code
 *  typedef struct _ATSC_SI_RRT_INFO_T
 *  {
 *      UINT8  ui1_num_values;
 *      BOOL  b_grad_scale;
 *  }   ATSC_SI_RRT_INFO_T;
 *  @endcode
 *  @li@c  ui1_num_values                                    -This field contains the number of values defined in a rating dimension.
 *  @li@c  b_grad_scale                                      -This field contains a boolean value, which indicates whether or not the rating values in this dimension represent a graduated scale.
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_RRT_INFO_T
{
    UINT8  ui1_num_values;

    BOOL  b_grad_scale;
}   ATSC_SI_RRT_INFO_T;


/* Possible values of ui1_flags of ATSC_SI_EIT_INFO_T */
#define EIT_FLAG_ETM_LOC_NONE           ((UINT8) 0x00)    /**<This flag indicates that there is no ETM for the event.        */
#define EIT_FLAG_ETM_LOC_THIS_PSIP      ((UINT8) 0x01)    /**<This flag indicates that the ETM for the event is located in the same physical transmission channel which this EIT table resides in.        */
#define EIT_FLAG_ETM_LOC_THIS_EVENT     ((UINT8) 0x02)    /**<This flag indicates that the ETM for the event is located in the physical transmission channel which carries the actual event streams.        */

#define EIT_MASK_ETM_LOC                ((UINT8) 0x03)    /**<The following defines a mask for EIT ETM location.        */

/* Holds values of an EIT loop entry. */
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_EIT_INFO_T
 *  @brief It contains the information of the EIT table.
 *  @code
 *  typedef struct _ATSC_SI_EIT_INFO_T
 *  {
 *      TIME_T  t_start_time;  
 *      TIME_T  t_duration;    
 *      UINT16  ui2_evt_id;
 *      UINT8  ui1_flags;
 *  }   ATSC_SI_EIT_INFO_T;
 *  @endcode
 *  @li@c  t_start_time                                      -This field contains the start time in GMT seconds.
 *  @li@c  t_duration                                        -This field contains the length in seconds of the event.
 *  @li@c  ui2_evt_id                                        -This field contains the event id value.
 *  @li@c  ui1_flags                                         -This field contains the properties of the evetn. See EIT flags in ¡§Constants, enumerations, and macros¡¨ section for details
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_EIT_INFO_T
{
    TIME_T  t_start_time;  /* Start time / date referenced to GMT. */
    TIME_T  t_duration;    /* Duration in seconds. */
    
    UINT16  ui2_evt_id;

    UINT8  ui1_flags;
}   ATSC_SI_EIT_INFO_T;


/* Holds values of a text language loop entry. */
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_TXT_LANG_INFO_T
 *  @brief It contains the information of the text and language.
 *  @code
 *  typedef struct _ATSC_SI_TXT_LANG_INFO_T
 *  {
 *      ISO_639_LANG_T  t_lang;
 *      UINT16  ui2_num_seg;
 *  }   ATSC_SI_TXT_LANG_INFO_T;
 *  @endcode
 *  @li@c  t_lang                                            -This field contains the language code.
 *  @li@c  ui2_num_seg                                       -This field contains the number of segments in this text language loop entry.
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_TXT_LANG_INFO_T
{
    ISO_639_LANG_T  t_lang;

    UINT16  ui2_num_seg;
}   ATSC_SI_TXT_LANG_INFO_T;

/* Holds values of a text segment loop entry. */
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_TXT_SEG_INFO_T
 *  @brief It contains the information of the text segment.
 *  @code
 *  typedef struct _ATSC_SI_TXT_SEG_INFO_T
 *  {
 *      SIZE_T  z_str_len;
 *      UINT8  ui1_compr_type;
 *      UINT8  ui1_compr_mode;
 *      BOOL  b_can_be_decompressed;
 *  }   ATSC_SI_TXT_SEG_INFO_T;
 *  @endcode
 *  @li@c  z_str_len                                         -This field contains the length of the text segment.
 *  @li@c  ui1_compr_type                                    -This field contains the compression type of the text segment.
 *  @li@c  ui1_compr_mode                                    -This field contains the compression mode of the text segment.
 *  @li@c  b_can_be_decompressed                             -This field contains a boolean value, which indicates whether or not the text segment can be decompressed.
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_TXT_SEG_INFO_T
{
    SIZE_T  z_str_len;
    
    UINT8  ui1_compr_type;
    UINT8  ui1_compr_mode;

    BOOL  b_can_be_decompressed;
}   ATSC_SI_TXT_SEG_INFO_T;


/*
    Structure for the pv_get_info when e_get_type set to
    ATSC_SI_GET_TYPE_TXT_SEG_INFO or ATSC_SI_GET_TYPE_TXT.
*/
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_LANG_SEG_IDX_INFO_T
 *  @brief It contains the index of language and segment of the text.
 *  @code
 *  typedef struct _ATSC_SI_LANG_SEG_IDX_INFO_T
 *  {
 *      UINT16  ui2_lang_idx;
 *      UINT16  ui2_seg_idx;
 *  }   ATSC_SI_LANG_SEG_IDX_INFO_T;
 *  @endcode
 *  @li@c  ui2_lang_idx                                      -This field contains a language index value.
 *  @li@c  ui2_seg_idx                                       -This field contains a segment index value.
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_LANG_SEG_IDX_INFO_T
{
    UINT16  ui2_lang_idx;
    UINT16  ui2_seg_idx;
}   ATSC_SI_LANG_SEG_IDX_INFO_T;


/*
    Structure for the output pv_data when e_get_type set to
    ATSC_SI_GET_TYPE_TXT.
*/
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_TXT_INFO_T
 *  @brief It contains the text string and length of the text.
 *  @code
 *  typedef struct _ATSC_SI_TXT_INFO_T
 *  {
 *      CHAR*   ps_txt;
 *      SIZE_T* pz_txt_len;
 *  }   ATSC_SI_TXT_INFO_T;
 *  @endcode
 *  @li@c  ps_txt                                            -This field references a UTF8 string.
 *  @li@c  pz_txt_len                                        -This field references a length, which contains the actual size of the string ps_txt.
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_TXT_INFO_T
{
    CHAR*   ps_txt;
    SIZE_T* pz_txt_len;
}   ATSC_SI_TXT_INFO_T;


/*
    Structure for the pv_get_info when e_get_type set to
    ATSC_SI_GET_TYPE_TXT_ABRV_DIM_VAL_IDX or 
    ATSC_SI_GET_TYPE_TXT_DIM_VAL_IDX.
*/
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_TXT_DIM_VAL_IDX_INFO_T
 *  @brief It contains the index of the dimension and the value.
 *  @code
 *  typedef struct _ATSC_SI_TXT_DIM_VAL_IDX_INFO_T
 *  {
 *      UINT16  ui2_dim_idx;
 *      UINT16  ui2_val_idx;
 *  }   ATSC_SI_TXT_DIM_VAL_IDX_INFO_T;
 *  @endcode
 *  @li@c  ui2_dim_idx                                       -This field contains a dimension index.
 *  @li@c  ui2_val_idx                                       -This field contains a rating value index.
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_TXT_DIM_VAL_IDX_INFO_T
{
    UINT16  ui2_dim_idx;
    UINT16  ui2_val_idx;
}   ATSC_SI_TXT_DIM_VAL_IDX_INFO_T;


/*
    Structure for the pv_load_info when e_load_type set to
    ATSC_SI_LOAD_TYPE_IDX_TIME.
*/
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_IDX_TIME_INFO_T
 *  @brief It contains the index of the event in the table and the time of this event.
 *  @code
 *  typedef struct _ATSC_SI_IDX_TIME_INFO_T
 *  {
 *      UINT16  ui2_idx;
 *      TIME_T  t_time;
 *  }   ATSC_SI_IDX_TIME_INFO_T;
 *  @endcode
 *  @li@c  ui2_idx                                           -This field contains an index value, which selects a loop entry from the parent table object.
 *  @li@c  t_time                                            -This field contains a time value, which specifies a specific time or time slot that covers it.
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_IDX_TIME_INFO_T
{
    UINT16  ui2_idx;
    TIME_T  t_time;
}   ATSC_SI_IDX_TIME_INFO_T;


/*
    Structure for the pv_load_info when e_load_type set to
    ATSC_SI_LOAD_TYPE_SRC_ID_TIME.
*/
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_SRC_ID_TIME_INFO_T
 *  @brief It contains the time of the event and source ID which identifies 
 *         the programming
 *  @code
 *  typedef struct _ATSC_SI_SRC_ID_TIME_INFO_T
 *  {
 *      UINT16  ui2_src_id;
 *      TIME_T  t_time;
 *  }   ATSC_SI_SRC_ID_TIME_INFO_T;
 *  @endcode
 *  @li@c  ui2_src_id                                        -This field contains a source id, which identifies the programming source associated with a virtual channel.
 *  @li@c  t_time                                            -This field contains a time value, which specifies a specific time or time slot that covers it.
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_SRC_ID_TIME_INFO_T
{
    UINT16  ui2_src_id;
    TIME_T  t_time;
}   ATSC_SI_SRC_ID_TIME_INFO_T;


/*
    Structure for the pv_load_info when e_load_type set to
    ATSC_SI_LOAD_TYPE_SRC_EVT_ID_TIME.
*/
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_SRC_EVT_ID_TIME_INFO_T
 *  @brief It contains the source id of current program, the ID and 
 *         time of the event.
 *  @code
 *  typedef struct _ATSC_SI_SRC_EVT_ID_TIME_INFO_T
 *  {
 *      UINT16  ui2_src_id;
 *      UINT16  ui2_evt_id;
 *      TIME_T  t_time;
 *  }   ATSC_SI_SRC_EVT_ID_TIME_INFO_T;
 *  @endcode
 *  @li@c  ui2_src_id                                        -This field contains a source id, which identifies the programming source associated with a virtual channel.
 *  @li@c  ui2_evt_id                                        -This field contains a event id, which identifies a program event on the specified virutal channel.
 *  @li@c  t_time                                            -This field contains a time value, which specifies a specific time or time slot that covers it.
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_SRC_EVT_ID_TIME_INFO_T
{
    UINT16  ui2_src_id;
    UINT16  ui2_evt_id;
    TIME_T  t_time;
}   ATSC_SI_SRC_EVT_ID_TIME_INFO_T;


/* EAS definitions begins from here. */

#define EAS_ORIGINATOR_LEN    4    /**<Null-terminated ASCII string, Three ASCII characters reflect the Originator code delivered in the broadcast EAS message.        */

/* EAS event main body description. */
/*------------------------------------------------------------------*/
/*! @struct EAS_EVENT_T
 *  @brief It contains the information of the EAS event.
 *  @code
 *  typedef struct  _EAS_EVENT_T_
 *  {
 *      UINT16  ui2_evt_id;
 *      CHAR    s_originator[EAS_ORIGINATOR_LEN];   
 *      UINT8   ui1_alert_priority;         
 *      TIME_T  t_msg_time_remaining;       
 *      TIME_T  t_evt_start_time;           
 *      TIME_T  t_evt_duration;             
 *      UINT16  ui2_source_id;              
 *      UINT16  ui2_major_chan_num;         
 *      UINT16  ui2_minor_chan_num;         
 *      UINT16  ui2_audio_oob_source_id;
 *  }   EAS_EVENT_T;
 *  @endcode
 *  @li@c  ui2_evt_id                                        - This field contains the EAS event id.
 *  @li@c  s_originator[EAS_ORIGINATOR_LEN]                  - Null-terminated ASCII string, Three ASCII characters reflect the Originator code delivered in the broadcast EAS message.
 *  @li@c  ui1_alert_priority                                - Indicates the priority of this alert.
 *  @li@c  t_msg_time_remaining                              - Gives the number of seconds remaining for display processing of this message, up to maximum of 120.
 *  @li@c  t_evt_start_time                                  - Indicates the start time of the EAS event in GPS seconds.
 *  @li@c  t_evt_duration                                    - Indicates the number of minutes, starting at t_evt_start_time, that the EAS event is expected to last.
 *  @li@c  ui2_source_id                                     - Indicates the Source ID of the Details channel.
 *  @li@c  ui2_major_chan_num                                - The major channel number of the Details channel.  Used when no out-of-band channel is available.
 *  @li@c  ui2_minor_chan_num                                - The minor channel number of the Details channel.  Used when no out-of-band channel is available.
 *  @li@c  ui2_audio_oob_source_id                           - Indicates the Source ID of an audio service that can be switched to for the duration of the alert message.
 */
/*------------------------------------------------------------------*/
typedef struct  _EAS_EVENT_T_
{
    UINT16  ui2_evt_id;
    CHAR    s_originator[EAS_ORIGINATOR_LEN];   /* Null-terminated ASCII string */
    UINT8   ui1_alert_priority;         /* 0-15, 0 is to be ignored. */

    /* EAS message display time and EA warning valid time information. */

    TIME_T  t_msg_time_remaining;       /* given in number of seconds */
    TIME_T  t_evt_start_time;           /* time (UTC)which the EAS event start. */
    TIME_T  t_evt_duration;             /* given in number of seconds. */

    /* Detail channels carrying EAS audio/video information, and body of EA event text. */
    UINT16  ui2_source_id;              /* If no detail channel is available, set to 0 */
    UINT16  ui2_major_chan_num;         /* 0 if no detail channel is available. */
    UINT16  ui2_minor_chan_num;         /* 0 if no detail channel is available. */
    UINT16  ui2_audio_oob_source_id;
}   EAS_EVENT_T;


/* EAS location entry description: state code, county code, etc */
/*------------------------------------------------------------------*/
/*! @struct EAS_LOC_INFO_T
 *  @brief It contains the EAS location entry description.
 *  @code
 *  typedef struct   _EAS_LOC_INFO_T_
 *  {
 *      UINT8   ui1_state_code;
 *      UINT8   ui1_county_subdivision;
 *      UINT16  ui2_county_code;
 *  }   EAS_LOC_INFO_T;
 *  @endcode
 *  @li@c  ui1_state_code                                    -Identifies the state affected by this alert.  Value zero means a national-level alert (all states).
 *  @li@c  ui1_county_subdivision                            -The country area is sub-divided into 9 sections on a 3-by-3 grid.
 *  @li@c  ui2_county_code                                   -This is a three-decimal-digit code number representing a country within the state identified by the ui1_state_code.
 */
/*------------------------------------------------------------------*/
typedef struct   _EAS_LOC_INFO_T_
{
    UINT8   ui1_state_code;
    UINT8   ui1_county_subdivision;
    UINT16  ui2_county_code;
}   EAS_LOC_INFO_T;


/* EAS exception channels entry description. */
#define CHAN_NUM_SZ             2    /**<For in-band exception channel.  The first element is the Major Channel number, the second element is Minor Channel number.  */

/*------------------------------------------------------------------*/
/*! @struct EAS_XCEPT_INFO_T
 *  @brief It contains the EAS exception channels entry description
 *  @code
 *  typedef struct   _EAS_XCEPT_INFO_T_
 *  {
 *      BOOL    b_in_band_ref;
 *      union 
 *      {
 *          UINT16  aui2_chan_num[CHAN_NUM_SZ];
 *          UINT16  ui2_oob_source_id;
 *      } u;
 *  }  EAS_XCEPT_INFO_T;
 *  @endcode
 *  @li@c  b_in_band_ref                                     -This field contains a boolean, which is used to determine the field in the followed union.
 *  @li@c  aui2_chan_num[CHAN_NUM_SZ]                        -For in-band exception channel.  The first element is the Major Channel number, the second element is Minor Channel number. 
 *  @li@c  ui2_oob_source_id                                 -If b_in_band_ref is set to FALSE, this field contains the Out-Of-Band source id.
 */
/*------------------------------------------------------------------*/
typedef struct   _EAS_XCEPT_INFO_T_
{
    BOOL    b_in_band_ref;
    union 
    {
        /* For in-band exception channel.  The first element is the Major 
            Channel number, the second element is Minor Channel number. */
        UINT16  aui2_chan_num[CHAN_NUM_SZ];

        /* For OOB exception channel, if no exception channel is available, 
            set to 0.*/
        UINT16  ui2_oob_source_id;
    } u;
}  EAS_XCEPT_INFO_T;


#define XCEPT_MAJOR_CHAN_NUM(_xcept_)   (_xcept_)->u.aui2_chan_num[0]    /**<Get the major channel number        */
#define XCEPT_MINOR_CHAN_NUM(_xcept_)   (_xcept_)->u.aui2_chan_num[1]    /**<Get the minor channel number        */
#define XCEPT_OOB_SOURCE_ID(_xcept_)    (_xcept_)->u.ui2_oob_source_id    /**<Source id of OOB        */


/*------------------------------------------------------------------*/
/*! @struct EAS_EVT_CODE_INFO_T
 *  @brief It contains the information of EAS code
 *  @code
 *  typedef struct  _EAS_EVT_CODE_INFO_T
 *  {
 *      CHAR*      ps_evt_code;
 *      SIZE_T*    pz_evt_code_len;
 *  }   EAS_EVT_CODE_INFO_T;
 *  @endcode
 *  @li@c  ps_evt_code                                       -This field contains the EAS event code string.
 *  @li@c  pz_evt_code_len                                   -This field contains the length of ps_evt_code.
 */
/*------------------------------------------------------------------*/
typedef struct  _EAS_EVT_CODE_INFO_T
{
    CHAR*      ps_evt_code;
    SIZE_T*    pz_evt_code_len;
}   EAS_EVT_CODE_INFO_T;

/* ATSC-SI engine configuration structure. */
/*------------------------------------------------------------------*/
/*! @struct ATSC_SI_CONFIG_T
 *  @brief It contains configuration value of the ATSC-SI engine.
 *  @code
 *  typedef struct _ATSC_SI_CONFIG_T
 *  {
 *      UINT8   ui1_num_mgt_filters;
 *      UINT8   ui1_num_tvct_filters;
 *      UINT8   ui1_num_cvct_filters;
 *      UINT8   ui1_num_eit_filters;
 *      UINT8   ui1_num_ett_filters;
 *      UINT8   ui1_num_stt_filters;
 *      UINT8   ui1_num_rrt_filters;
 *      UINT8   ui1_num_eas_filters;
 *      SIZE_T  z_mgt_buff_size;
 *      SIZE_T  z_tvct_buff_size;
 *      SIZE_T  z_cvct_buff_size;
 *      SIZE_T  z_eit_buff_size;
 *      SIZE_T  z_ett_buff_size;
 *      SIZE_T  z_stt_buff_size;
 *      SIZE_T  z_rrt_buff_size;
 *      SIZE_T  z_eas_buff_size;
 *      UINT16  ui2_num_mgt_tbl_objs;
 *      UINT16  ui2_num_tvct_tbl_objs;
 *      UINT16  ui2_num_cvct_tbl_objs;
 *      UINT16  ui2_num_eit_tbl_objs;
 *      UINT16  ui2_num_ett_tbl_objs;
 *      UINT16  ui2_num_stt_tbl_objs;
 *      UINT16  ui2_num_rrt_tbl_objs;
 *      UINT16  ui2_num_txt_tbl_objs;
 *      UINT16  ui2_num_eas_tbl_objs;
 *  }   ATSC_SI_CONFIG_T;
 *  @endcode
 *  @li@c  ui1_num_mgt_filters                               -This field contains the number of MGT filters the ATSC engine shall maintain. The value of this field could be zero if the value of ui2_num_mgt_tbl_objs is also set to zero.       
 *  @li@c  ui1_num_tvct_filters                              -This field contains the number of TVCT filters the ATSC-SI engine shall maintain. The value of this field could be zero if the value of ui2_num_tvct_tbl_objs is also set to zero.  
 *  @li@c  ui1_num_cvct_filters                              -This field contains the number of CVCT filters the ATSC-SI engine shall maintain. The value of this field could be zero if the value of ui2_num_cvct_tbl_objs is also set to zero.  
 *  @li@c  ui1_num_eit_filters                               -This field contains the number of EIT filters the ATSC-SI engine shall maintain. The value of this field could be zero if the value of ui2_num_eit_tbl_objs is also set to zero.    
 *  @li@c  ui1_num_ett_filters                               -This field contains the number of ETT filters the ATSC-SI engine shall maintain. The value of this field could be zero if the value of ui2_num_ett_tbl_objs is also set to zero.    
 *  @li@c  ui1_num_stt_filters                               -This field contains the number of STT filters the ATSC-SI engine shall maintain. The value of this field could be zero if the value of ui2_num_ett_tbl_objs is also set to zero.    
 *  @li@c  ui1_num_rrt_filters                               -This field contains the number of RRT filters the ATSC-SI engine shall maintain. The value of this field could be zero if the value of ui2_num_rrt_tbl_objs is also set to zero.                                                                                                                              
 *  @li@c  ui1_num_eas_filters                               -This field contains the number of EAS filters the ATSC-SI engine shall maintain. The value of this field could be zero if the value of ui2_num_eas_tbl_objs is also set to zero.                                                                                                                              
 *  @li@c  z_mgt_buff_size                                   -This field contains the demux filter buffer size per MGT filter in number of Bytes. When the value of this field is set to zero, default MGT buffer size will be used.                                                                                                                                        
 *  @li@c  z_tvct_buff_size                                  -This field contains the demux filter buffer size per TVCT filter in number of Bytes. When the value of this field is set to zero, default TVCT buffer size will be used.                                                                                                                                      
 *  @li@c  z_cvct_buff_size                                  -This field contains the demux filter buffer size per CVCT filter in number of Bytes. When the value of this field is set to zero, default CVCT buffer size will be used.                                                                                                                                      
 *  @li@c  z_eit_buff_size                                   -This field contains the demux filter buffer size per EIT filter in number of Bytes. When the value of this field is set to zero, default EIT buffer size will be used.                                                                                                                                        
 *  @li@c  z_ett_buff_size                                   -This field contains the demux filter buffer size per ETT filter in number of Bytes. When the value of this field is set to zero, default ETT buffer size will be used.                                                                                                                                        
 *  @li@c  z_stt_buff_size                                   -This field contains the demux filter buffer size per STT filter in number of Bytes. When the value of this field is set to zero, default ETT buffer size will be used.                                                                                                                                        
 *  @li@c  z_rrt_buff_size                                   -This field contains the demux filter buffer size per RRT filter in number of Bytes. When the value of this field is set to zero, default RRT buffer size will be used.                                                                                                                                        
 *  @li@c  z_eas_buff_size                                   -This field contains the demux filter buffer size per EAS filter in number of Bytes. When the value of this field is set to zero, default EAS buffer size will be used.                                                                                                                                        
 *  @li@c  ui2_num_mgt_tbl_objs                              -Specifies the number of MGT objects which can be cached simultaneously.                                                                                                                                                                                                                                       
 *  @li@c  ui2_num_tvct_tbl_objs                             -Specifies the number of TVCT objects which can be cached simultaneously.                                                                                                                                                                                                                                      
 *  @li@c  ui2_num_cvct_tbl_objs                             -Specifies the number of CVCT objects which can be cached simultaneously.                                                                                                                                                                                                                                      
 *  @li@c  ui2_num_eit_tbl_objs                              -Specifies the number of EIT objects which can be cached simultaneously.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
 *  @li@c  ui2_num_ett_tbl_objs                              -Specifies the number of ETT objects which can be cached simultaneously.                                                                                                                                                                                                                                                                                                               
 *  @li@c  ui2_num_stt_tbl_objs                              -Specifies the number of STT objects which can be cached simultaneously.                                                                                                                                                                                                                                                                                           
 *  @li@c  ui2_num_rrt_tbl_objs                              -Specifies the number of RRT objects which can be cached simultaneously.
 *  @li@c  ui2_num_txt_tbl_objs                              -Specifies the number of TXT objects which can be cached simultaneously.
 *  @li@c  ui2_num_eas_tbl_objs                              -Specifies the number of EAS objects which can be cached simultaneously.
 */
/*------------------------------------------------------------------*/
typedef struct _ATSC_SI_CONFIG_T
{
    UINT8   ui1_num_mgt_filters;
    UINT8   ui1_num_tvct_filters;
    UINT8   ui1_num_cvct_filters;
    UINT8   ui1_num_eit_filters;
    UINT8   ui1_num_ett_filters;
    UINT8   ui1_num_stt_filters;
    UINT8   ui1_num_rrt_filters;
    UINT8   ui1_num_eas_filters;

    SIZE_T  z_mgt_buff_size;
    SIZE_T  z_tvct_buff_size;
    SIZE_T  z_cvct_buff_size;
    SIZE_T  z_eit_buff_size;
    SIZE_T  z_ett_buff_size;
    SIZE_T  z_stt_buff_size;
    SIZE_T  z_rrt_buff_size;
    SIZE_T  z_eas_buff_size;

    UINT16  ui2_num_mgt_tbl_objs;
    UINT16  ui2_num_tvct_tbl_objs;
    UINT16  ui2_num_cvct_tbl_objs;
    UINT16  ui2_num_eit_tbl_objs;
    UINT16  ui2_num_ett_tbl_objs;
    UINT16  ui2_num_stt_tbl_objs;
    UINT16  ui2_num_rrt_tbl_objs;
    UINT16  ui2_num_txt_tbl_objs;
    UINT16  ui2_num_eas_tbl_objs;
}   ATSC_SI_CONFIG_T;


#ifdef RAW_MEM_DISPLAY

/*------------------------------------------------------------------*/
/*! @enum ATSC_SI_RM_NFY_T
 *  @brief The notification type of raw memory of the ATSC SI engine .
 *  @code
 *  typedef enum
 *  {
 *      ATSC_SI_RM_NFY_UNKNOWN = 0,
 *      ATSC_SI_RM_NFY_TRANSITION,
 *      ATSC_SI_RM_NFY_AVAIL,
 *      ATSC_SI_RM_NFY_UNAVAIL,
 *      ATSC_SI_RM_NFY_TOTAL_NUM
 *  } ATSC_SI_RM_NFY_T;
 *  @endcode
 *  @li@c  ATSC_SI_RM_NFY_UNKNOWN                  - The nitification indicates that the raw memory 
 *                                                   data is unkonown.
 *  @li@c  ATSC_SI_RM_NFY_TRANSITION               - The nitification indicates that the raw memory data is transmitting.
 *  @li@c  ATSC_SI_RM_NFY_AVAIL                    - The nitification indicates that the raw memory data is available.
 *  @li@c  ATSC_SI_RM_NFY_UNAVAIL                  - The nitification indicates that the raw memory data is unavailable.
 *  @li@c  ATSC_SI_RM_NFY_TOTAL_NUM                - The nitification indicates that the data is the total number of 
 *                                                   raw memory data
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ATSC_SI_RM_NFY_UNKNOWN = 0,
    ATSC_SI_RM_NFY_TRANSITION,
    ATSC_SI_RM_NFY_AVAIL,
    ATSC_SI_RM_NFY_UNAVAIL,
    ATSC_SI_RM_NFY_TOTAL_NUM
} ATSC_SI_RM_NFY_T;

/*------------------------------------------------------------------*/
/*! @enum ATSC_SI_RM_SET_T
 *  @brief The set type of the raw memory of ATSC SE engine.
 *  @code
 *  typedef enum
 *  {
 *      ATSC_SI_RM_SET_INIT = 0,
 *      ATSC_SI_RM_SET_REFRESH,
 *      ATSC_SI_RM_SET_STOP,
 *      ATSC_SI_RM_SET_TOTAL_NUM
 *  } ATSC_SI_RM_SET_T;
 *  @endcode
 
 *  @li@c  ATSC_SI_RM_SET_INIT                  - The set type indicates to do initialization.
 *  @li@c  ATSC_SI_RM_SET_REFRESH               - The set type indicates to refresh data.
 *  @li@c  ATSC_SI_RM_SET_STOP                  - The set type indicates to stop recieving raw data and set 
 *                                                the state to uninitialization.
 *  @li@c  ATSC_SI_RM_SET_TOTAL_NUM             - The set type indicates the total number of raw memroy data. 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ATSC_SI_RM_SET_INIT = 0,
    ATSC_SI_RM_SET_REFRESH,
    ATSC_SI_RM_SET_STOP,
    ATSC_SI_RM_SET_TOTAL_NUM
} ATSC_SI_RM_SET_T;

/*------------------------------------------------------------------*/
/*! @brief The prototype of copy function to copy the raw memory data.
 *  @param [in]     pz_dest_len - The length of the destination buffer.
 *  @param [out]    pv_dest     - The destination buffer address.
 *                    
 *  @note
 *  @see
 *  @return Return the error code.
 *  @retval TMR_OK                  - Routine successful.
 *  @retval TMR_NOT_INIT            - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG             - One or more of the arguments contain 
 *                                    invalid data.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*atsc_si_rm_copy_fct) (VOID*    pv_dest,
                                      SIZE_T*  pz_dest_len);

/*------------------------------------------------------------------*/
/*! @brief The prototype of callback funtion for notifying the client.
 *  @param [in] e_nfy_type      - Notification type of copying the 
 *                                the raw memory data.
 *              pf_copy         - The copy funtion pointer.
 *              ui4_data        - Contains the data.
 *  @param [out]
 *  @note
 *  @see
 *  @return Return the error code.
 *  @retval TMR_OK                  - Routine successful.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*atsc_si_rm_nfy_fct) (ATSC_SI_RM_NFY_T    e_nfy_type,
                                     atsc_si_rm_copy_fct pf_copy,
                                     UINT32              ui4_data);

#endif /* RAW_MEM_DISPLAY */   

/*----------------------------------------------------------------------------*/
/*! @} */                              /* end of groupMW_TBL_MNGR_ATSC_SI_ENG */
/*----------------------------------------------------------------------------*/

#endif /* _U_ATSC_SI_H_ */
