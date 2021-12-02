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
/*! @file u_tsl.h 
 *  $File: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/middleware/tsl/u_tsl.h $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *    This header file contains Transport Stream list (TSL) database
 *    API definitions, which are exported to other Middleware components
 *    and applications.
 *         
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_TSL_H_
#define _U_TSL_H_
/*----------------------------------------------------------------------------*/
/*! @defgroup   groupMW_TSL Transport Stream List
 *  @ingroup    groupMW
 *  @brief      The TSL module is used to store and retrieve the tuning
 *              parameter information for the broadcast streams.
 *  @see        groupMW_CDB
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"
#include "u_tuner.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/

/* Return code status from API */
#define     TSLR_OK                    ((INT32)  0)    /**< Operation succeeded. */
#define	    TSLR_FAIL                  ((INT32) -1)    /**< Operation failed. */
#define     TSLR_OUT_OF_HANDLE         ((INT32) -2)    /**< Handle allocation failed. */
#define	    TSLR_OUT_OF_MEM            ((INT32) -3)    /**< Memory exhausted. */
#define	    TSLR_INV_HANDLE            ((INT32) -4)    /**< Bad handle. */
#define     TSLR_INV_ARG               ((INT32) -5)    /**< Invalid argument. */
#define	    TSLR_REC_NOT_FOUND         ((INT32) -6)    /**< Record not found. */
#define	    TSLR_NO_WRITE_LOCK         ((INT32) -7)    /**< Failed initializing RW Lock. */
#define     TSLR_NOT_FOUND             ((INT32) -8)    /**< Specified TSL object not found. */
#define     TSLR_END_OF_ITERATION      ((INT32) -9)    /**< Iteration reached end. */
#define     TSLR_MODIFIED              ((INT32) -10)    /**< Database modified since last iteration. */
#define     TSLR_DUPLICATE_REC         ((INT32) -11)    /**< Existing record with the same key value is found. */
#define     TSLR_INV_FILE_PATH         ((INT32) -12)    /**< Invalid file path. */
#define     TSLR_OUT_OF_SPACE          ((INT32) -13)    /**< Failed to allocate space on file system. */
#define     TSLR_AEE_OUT_OF_RESOURCES  ((INT32) -14)    /**< Can not allocate AEE resources. */
#define     TSLR_AEE_NO_RIGHTS         ((INT32) -15)    /**< No right for the AEE resources. */
#define     TSLR_FILE_READ_FAIL        ((INT32) -16)    /**< Failed to read from file. */
#define     TSLR_FILE_WRITE_FAIL       ((INT32) -17)    /**< Failed to write to file. */
#define     TSLR_INV_FILE_VER          ((INT32) -18)    /**< Incompatible file version. */


/* Handle type for the TSL handle. */
#define TSLT_TRANSPORT_STREAM_LIST (HT_GROUP_TSL + ((HANDLE_TYPE_T) 0))    /**< Handle type for the Transport Stream List. */

#define MAX_NW_NAME_LEN   25    /**< Maximum length for network name. */

/* Enumeration for TSL database condition */
/*------------------------------------------------------------------*/
/*!  @brief  Enumeration for TSL Database conditions
 *  @code
 *  typedef  enum
 *  {
 *      TSL_UPDATING = 1,  
 *      TSL_UPDATED,       
 *      TSL_CLOSED,        
 *      TSL_UNKNOWN
 *  } TSL_COND_T;
 *  @endcode
 *  @li@c  TSL_UPDATING                  - When tsl_lock is called.
 *  @li@c  TSL_UPDATED                   - When tsl_unlock is called.
 *  @li@c  TSL_CLOSED                    - When tsl_delete is called.
 *  @li@c  TSL_UNKNOWN                   - Unknown condition.
 */
/*------------------------------------------------------------------*/
typedef  enum
{
    TSL_UPDATING = 1,  /* When tsl_lock is called. */
    TSL_UPDATED,       /* When tsl_unlock is called. */
    TSL_CLOSED,        /* When tsl_delete is called. */
    TSL_UNKNOWN
} TSL_COND_T;

/* Notification reasons */
#define     TSL_REASON_UNKNOWN   ((UINT32)(0x0000))    /**< Reason unknown */
#define     TSL_RECORD_ADD       (MAKE_BIT_MASK_32(1))    /**< Reason for adding records */
#define     TSL_RECORD_DEL       (MAKE_BIT_MASK_32(2))    /**< Reason for deleting records */
#define     TSL_RECORD_MOD       (MAKE_BIT_MASK_32(3))    /**< Reason for modifying records */

/* Max length of the transport stream list name (bytes) */
#define     MAX_TSL_NAME_LEN          16    /**< Max length of the transport stream list name (bytes). */

/* Size (byte) of TSL record header and data structure. */
#define	    TSL_REC_HEADER_PADDING      12    /**< Padding size (byte) of TSL record header structure. */
#define	    TSL_REC_DATA_PADDING        172  /**60**/   /* was 52 */    /**< Padding size (byte) of TSL record data structure. */

/* Pre-set size of the TSL record structure. */
#define     TSL_REC_SIZE   (TSL_REC_HEADER_PADDING+TSL_REC_DATA_PADDING+4+MAX_NW_NAME_LEN+3+TSL_REC_ALTERNATE_TUNE_INFO_PADDING+TSL_REC_REFERENCE_PADDING)    /**< Pre-set size of the TSL record structure. */


/* Typedef and constants for TSL record id query flag. */
typedef     UINT8                 TSL_REC_ID_QRY_T;    /**< Typedef for TSL record id query flag. */

#define     TSL_FIRST_REC_ID      ((TSL_REC_ID_QRY_T) 1)    /**< First record ID. */
#define     TSL_LAST_REC_ID       ((TSL_REC_ID_QRY_T) 2)    /**< Last record ID. */
#define     TSL_NEXT_REC_ID       ((TSL_REC_ID_QRY_T) 3)    /**< Next record ID. */

/* Typedef and constants for TSL memory mangement policy. */
typedef	    UINT8	              TSL_MEM_POLICY_T;    /**< Typedef for TSL memory mangement policy. */

#define	    TSL_MEM_STATIC        ((TSL_MEM_POLICY_T) 0)    /**< Static memory policy. */
#define	    TSL_MEM_DYNAMIC       ((TSL_MEM_POLICY_T) 1)    /**< Dynamic memory policy. */

/* Null TSL database version id. */
#define     TSL_NULL_VER_ID       ((UINT32)0)    /**< Null TSL database version id. */

/* NULL TSL record ID value */
#define     TSL_NULL_REC_ID       ((UINT16)0)    /**< NULL TSL record ID value */

/* NULL TSL ID value. */
#define     TSL_NULL_ID           ((UINT16)0)    /**< NULL TSL ID value. */

#define     ISDB_TSL_NAME_LEN     ((UINT16) 40)    /**< Maximum length of TS name in TS Information Descriptor used in ISDB */

/* Typedef and constants for TSL record iteration flag. */
typedef	    UINT8                 TSL_ITERATE_T;    /**< Typedef for TSL record iteration flag. */

#define     TSL_NEXT_REC          ((TSL_ITERATE_T) 1)    /**< Next record */
#define     TSL_PREV_REC          ((TSL_ITERATE_T) 2)    /**< Previous record */
#define     TSL_FIRST_REC         ((TSL_ITERATE_T) 3)    /**< First record */
#define     TSL_LAST_REC          ((TSL_ITERATE_T) 4)    /**< Last record */

/*---------------------------------------------------------------------------
 Structures
----------------------------------------------------------------------------*/

#define MAX_NB_ALTERNATE_FREQ   5    /**< Maximum number of alternative frequencies. */

/*------------------------------------------------------------------*/
/*! @brief  Structure for frequency information retrieved from NIT frequency
 *          list descriptor.
 *  @code
 *  typedef struct _FREQ_INFO_T_
 *  {
 *      UINT32                      ui4_freq;
 *      TUNER_BANDWIDTH_T           e_bandwidth;
 *  }  FREQ_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                      - Central frequency.
 *  @li@c  e_bandwidth                   - Bandwidth.
 */
/*------------------------------------------------------------------*/
typedef struct _FREQ_INFO_T_
{
    UINT32                      ui4_freq;
    TUNER_BANDWIDTH_T           e_bandwidth;
}  FREQ_INFO_T;

/*------------------------------------------------------------------*/
/*! @brief  Structure for alternative tuning information used in RiksTV.
 *  @code
 *  typedef struct _ALTERNATE_TUNE_INFO_T_
 *  {
 *      FREQ_INFO_T    t_all_freq[MAX_NB_ALTERNATE_FREQ];
 *      FREQ_INFO_T    t_orig_freq;
 *      TIME_T         t_loss_event_time;
 *      UINT8          ui1_first_tune;
 *      BOOL           b_freq_updated;
 *  }  ALTERNATE_TUNE_INFO_T;
 *  @endcode
 *  @li@c  t_all_freq                    - Alternative frequencies in
 *                                         frequency list descriptor.
 *  @li@c  t_orig_freq                   - Original frequency information
 *                                         used for comparison with current
 *                                         frequency at first tune.
 *  @li@c  t_loss_event_time             - UTC time of reception signal
 *                                         loss event. After 30 days, we no
 *                                         longer need to check original &
 *                                         current frequency.
 *  @li@c  ui1_first_tune                - If 0, the TS has not been tuned to
 *                                         yet since boot up. After the first
 *                                         tune, this field is set to 1.
 *  @li@c  b_freq_updated                - Indicate whether frequency list
 *                                         descriptor has changed.
 */
/*------------------------------------------------------------------*/
typedef struct _ALTERNATE_TUNE_INFO_T_
{
    FREQ_INFO_T    t_all_freq[MAX_NB_ALTERNATE_FREQ];
    FREQ_INFO_T    t_orig_freq;
    TIME_T         t_loss_event_time;
    UINT8          ui1_first_tune;
    BOOL           b_freq_updated;
}  ALTERNATE_TUNE_INFO_T;

/* sizeof(FREQ_INFO_T) * (MAX_NB_ALTERNATE_FREQ+1) + sizeof(TIME_T) + 1 + 1
   = 8 * (5+1) + 8 + 1 + 1
   = 48 + 8 + 1 + 1
   = 58

   round off to 4 bytes boundary:  60

   Caution: in Linux solution, although offsetof(ALTERNATE_TUNE_INFO_T,
   b_freq_updated) + sizeof(BOOL) == 58, sizeof(ALTERNATE_TUNE_INFO_T) == 64.
*/
#define TSL_REC_ALTERNATE_TUNE_INFO_PADDING   64    /**< Padding size (byte) of TSL record alternative tuning information structure. */

/*------------------------------------------------------------------*/
/*! @brief  Structure for TSL record reference information for DVB-S.
 *  @code
 *  typedef struct _TSL_REC_REFERENCE_T_
 *  {
 *      UINT32          ui4_mask;
 *      UINT16          ui2_nwl_id;
 *      UINT16          ui2_nwl_rec_id;
 *      UINT16          ui2_satl_id;
 *      UINT16          ui1_satl_rec_id;
 *  }  TSL_REC_REFERENCE_T;
 *  @endcode
 *  @li@c  ui4_mask                      - User defined bit mask.
 *  @li@c  ui2_nwl_id                    - NWL ID.
 *  @li@c  ui2_nwl_rec_id                - NWL record ID.
 *  @li@c  ui2_satl_id                   - SATL ID.
 *  @li@c  ui1_satl_rec_id               - SATL record ID.
 */
/*------------------------------------------------------------------*/
typedef struct _TSL_REC_REFERENCE_T_
{
    UINT32          ui4_mask;           /* bit mask */
    UINT16          ui2_nwl_id;         /* NWL id. */
    UINT16          ui2_nwl_rec_id;     /* NWL record id. */
    UINT16          ui2_satl_id;        /* SATL id */
    UINT16          ui2_satl_rec_id;    /* SATL record id*/
}  TSL_REC_REFERENCE_T;

#define TSL_REC_REFERENCE_PADDING   12    /**< Padding size (byte) of TSL record reference structure. */

/********** Structure for common data *********/

/*------------------------------------------------------------------*/
/*!  @brief  Structure for ISDB-T digital TS infomation
 *  @code
 *  typedef struct _ISDB_TER_DIG_INFO_T
 *  {
 *      TUNER_TER_DIG_TUNE_INFO_T           t_ter_dig;
 *  	UINT8								ui1_rc_key_id;
 *  	UINT8                               ui1_num_layers;
 *  	UINT16                              ui2_prim_svc;
 *  	INT32								ai4_pr_svc_id[8];
 *  	CHAR                                ac_ts_name[ISDB_TSL_NAME_LEN+1];
 *  } ISDB_TER_DIG_INFO_T;
 *  @endcode
 *  @li@c  t_ter_dig                     - For actural tuner related information
 *  @li@c  ui1_rc_key_id                 - Remote control key ID
 *  @li@c  ui1_num_layers                - Number of layers
 *  @li@c  ui2_prim_svc                  - Service correspondent to rc_key
 *  @li@c  ai4_pr_svc_id                 - Partial reception service ID's
 *  @li@c  ac_ts_name                    - Name of the TS
 */
/*------------------------------------------------------------------*/
typedef struct _ISDB_TER_DIG_INFO_T
{
    /*for actural tuner related information */
    TUNER_TER_DIG_TUNE_INFO_T           t_ter_dig;

    UINT32                              ui4_mask;
	UINT8								ui1_rc_key_id;
	UINT8                               ui1_num_layers;
	UINT16                              ui2_prim_svc;/*only Low protection layer */

	INT32								ai4_pr_svc_id[8];/* partial reception layer service id*/
	CHAR                                ac_ts_name[ISDB_TSL_NAME_LEN+1];
} ISDB_TER_DIG_INFO_T;

/* Structure for the TSL data record */
/*------------------------------------------------------------------*/
/*!  @brief  Structure for the TSL record
 *  @code
 *  typedef struct _TSL_REC_T
 *  {
 *      UINT16      ui2_tsl_rec_id; 
 *      union 
 *      {
 *          TS_DESCR_T   t_desc ;  
 *  	    UINT8  _rec_header_padding[TSL_REC_HEADER_PADDING];
 *      } uheader;
 *      union
 *      {
 *          TUNER_CAB_DIG_TUNE_INFO_T   t_cab_dig;
 *          TUNER_SAT_DIG_TUNE_INFO_T   t_sat_dig;
 *          TUNER_TER_DIG_TUNE_INFO_T   t_ter_dig; 
 *          TUNER_CAB_ANA_TUNE_INFO_T   t_cab_ana;
 *          TUNER_SAT_ANA_TUNE_INFO_T   t_sat_ana;
 *          TUNER_TER_ANA_TUNE_INFO_T   t_ter_ana;
 *          ISDB_TER_DIG_INFO_T         t_isdbt_dig;
 *          UINT8  _rec_data_padding[TSL_REC_DATA_PADDING];
 *      } udata;
 *      CHAR s_nw_name[MAX_NW_NAME_LEN];
 *      union
 *      {
 *          ALTERNATE_TUNE_INFO_T       t_alt_ter_dig;
 *          UINT8  _alt_tune_data_padding[TSL_REC_ALTERNATE_TUNE_INFO_PADDING];
 *      } utune;
 *      union
 *      {
 *          TSL_REC_REFERENCE_T         t_ref;
 *          UINT8  _ref_data_padding[TSL_REC_REFERENCE_PADDING];
 *      } uref;
 *  } TSL_REC_T;
 *  @endcode
 *  @li@c  ui2_tsl_rec_id                - Specify the TSL record id.
 *  @li@c  t_desc                        - Specify the common description for
 *                                         this TS record.
 *  @li@c  _rec_header_padding           - Padding for the record header union
 *                                         structure.
 *  @li@c  t_cab_dig                     - Cable digital tuning parameters.
 *  @li@c  t_sat_dig                     - Satellite digital tuning parameters.
 *  @li@c  t_ter_dig                     - Terrestrial digital tuning
 *                                         parameters.
 *  @li@c  t_cab_ana                     - Cable analog tuning parameters.
 *  @li@c  t_sat_ana                     - Satellite analog tuning parameters.
 *  @li@c  t_ter_ana                     - Terrestrial analog tuning parameters.
 *  @li@c  t_isdbt_dig                   - ISDB-T didgital tuning parameters and
 *                                         related information.
 *  @li@c  _rec_data_padding             - Padding for the record data union
 *                                         structure.
 *  @li@c  s_nw_name[MAX_NW_NAME_LEN]    - Name of the network the TS belongs
 *                                         to.
 *  @li@c  t_alt_ter_dig                 - Alternate tuning freq information for
 *                                         RIKSTV support
 *  @li@c  _alt_tune_data_padding        - Padding for the alternate tuning info
 *                                         structure.
 *  @li@c  t_ref                         - TSL record reference information for
 *                                         DVB-S.
 *  @li@c  _ref_data_padding             - Padding for the TSL record reference
 *                                         structure.
 */
/*------------------------------------------------------------------*/
typedef struct _TSL_REC_T
{
    /* Transport Stream record id */
    UINT16      ui2_tsl_rec_id; 

    union 
    {
        /* See 'u_common.h' for definition of TS_DESCR_T structure. */
        TS_DESCR_T   t_desc ;  

        /* Padding to make "uheader" union 16 bytes. */
	    UINT8  _rec_header_padding[TSL_REC_HEADER_PADDING];
    } uheader;
      
    union
    {
        /* Following structures for tuning parameters are defined in 
           'u_tuner.h'. */
        TUNER_CAB_DIG_TUNE_INFO_T   t_cab_dig;
        TUNER_SAT_DIG_TUNE_INFO_T   t_sat_dig;
        TUNER_TER_DIG_TUNE_INFO_T   t_ter_dig; 

        /* Analog tuning info structure. */
        TUNER_CAB_ANA_TUNE_INFO_T   t_cab_ana;
        TUNER_SAT_ANA_TUNE_INFO_T   t_sat_ana;
        TUNER_TER_ANA_TUNE_INFO_T   t_ter_ana;
        
        /* ISDB terrestrial tuning info structure*/
        ISDB_TER_DIG_INFO_T         t_isdbt_dig;

        /* Padding to make 'udata' union 140 bytes.  */
        UINT8  _rec_data_padding[TSL_REC_DATA_PADDING];
    } udata;

    CHAR s_nw_name[MAX_NW_NAME_LEN];

    /* Alternate tuning freq information; RIKSTV support */
    union
    {
        ALTERNATE_TUNE_INFO_T       t_alt_ter_dig;
        UINT8  _alt_tune_data_padding[TSL_REC_ALTERNATE_TUNE_INFO_PADDING];
    } utune;
        
    /* TSL record reference information; DVB-S support */
    union
    {
        TSL_REC_REFERENCE_T         t_ref;
        UINT8  _ref_data_padding[TSL_REC_REFERENCE_PADDING];
    } uref;

} TSL_REC_T;

/*
 * Structure for memory dump.
 */
/*------------------------------------------------------------------*/
/*!  @brief  Structure for memory dump
 *  @code
 *  typedef struct _TSL_MEM_DUMP_T_
 *  {
 *      UINT16      ui2_tsl_id;
 *      CHAR        ps_tsl_name[MAX_TSL_NAME_LEN+1];
 *      VOID*       pv_data;
 *      UINT32      ui4_data_len;
 *  }  TSL_MEM_DUMP_T ;
 *  @endcode
 *  @li@c  ui2_tsl_id                    - ID of the Transport Stream List
 *  @li@c  ps_tsl_name                   - Name of the Transport Stream List
 *  @li@c  pv_data                       - Memory dump of TSL database
 *  @li@c  ui4_data_len                  - Size of memory allocated for pv_data
 */
/*------------------------------------------------------------------*/
typedef struct _TSL_MEM_DUMP_T_
{
    UINT16      ui2_tsl_id;
    CHAR        ps_tsl_name[MAX_TSL_NAME_LEN+1];
    VOID*       pv_data;
    UINT32      ui4_data_len;
}  TSL_MEM_DUMP_T ;

/*---------------------------------------------------------------------------
 Type definition
----------------------------------------------------------------------------*/

/*
  typedef for the TSL handle callback function.  This fucntion
  is implemented by the client of TSL.  
*/
/*------------------------------------------------------------------*/
/*! @brief  Typedef for the TSL handle callback function.  This fucntion
 *          is implemented by the client of TSL.
 *  @param  [in]  h_tsl             - Handle to the TSL object.
 *  @param  [in]  e_cond            - The status flag for the TSL object.
 *  @param  [in]  ui4_reason        - The reason for the notification.
 *  @param  [in]  pv_tag            - Private tag for the client.
 *  @param  [in]  ui4_data          - Data pass back to the application.
 *  @note   It is very important that the client MUST NOT try to read the TSL in
 *          the context of notification function.  Otherwise, a `Dead lock'
 *          situation would arise.  The client should only send a message to
 *          itself from the notification function or set a condition flag, and
 *          execute the read operation outside the context of the notification
 *          function.
 *          @par
 *          Client should also avoid doing CPU intensive task in the
 *          notification function.  If CPU intensive task needs to be performed,
 *          then the notification function should send a message to the client
 *          thread, and have the CPU intensive task executed in the client
 *          thread context.
 *  @see    c_tsl_create, c_tsl_open, c_tsl_open_by_name, c_tsl_fs_load

 */
/*------------------------------------------------------------------*/
typedef VOID (*x_tsl_nfy_fct)
(
    HANDLE_T            h_tsl,
    TSL_COND_T          e_cond,
    UINT32              ui4_reason,
    VOID*               pv_tag,
    UINT32              ui4_data
);

/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/
#endif /* _U_TSL_H_ */
