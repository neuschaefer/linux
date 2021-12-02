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
/*! @file u_dt.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Date Time related API definitions,
 *         which are exported to other Middleware components and application.
 *
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_DT_H_
#define _U_DT_H_

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_DT Date and Time
 *
 *  @ingroup groupMW
 *  @brief The Date and Time module could be get broadcasting and system time 
 *         information.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could get 
 *   broadcasting and system time information.
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/
/* Return code status from API */
#define         DTR_WILL_NOTIFY             ((INT32)  1)    /**<Return Code:will notify caller        */
#define         DTR_OK                      ((INT32)  0)    /**<Return Code:OK        */
#define	        DTR_FAIL                    ((INT32) -1)    /**<Return Code:Fail        */
#define	        DTR_OUT_OF_MEM              ((INT32) -2)    /**<Return Code:Out of memory        */
#define         DTR_INV_ARG                 ((INT32) -3)    /**<Return Code:Invalid argument        */
#define         DTR_NO_RTC                  ((INT32) -4)    /**<Return Code:No RTC        */
#define         DTR_OUT_OF_HANDLE           ((INT32) -5)    /**<Return Code:Out of handle        */
#define         DTR_AEE_OUT_OF_RESOURCES    ((INT32) -6)    /**<Return Code:Aee out of resource        */
#define         DTR_AEE_NO_RIGHTS           ((INT32) -7)    /**<Return Code:Aee no rights        */
#define         DTR_INV_IDX                 ((INT32) -8)    /**<Return Code:Invalid index        */

/* -------------------------------------------------------------------------
  Typedefs
------------------------------------------------------------------------- */
typedef INT32  DT_COND_T;    /**<        */

/* system clock condition. */
#define      DT_NOT_RUNNING                 ((DT_COND_T) 1)    /**<Condition:Not running        */
#define      DT_FREE_RUNNING                ((DT_COND_T) 2)    /**<Condition:Free running        */
#define      DT_SYNC_RUNNING                ((DT_COND_T) 3)    /**<Condition:sync running        */
#define      DT_SYNC_DISCONT                ((DT_COND_T) 4)    /**<Condition:sync discontinue        */
#define      DT_LTO_UPDATED                 ((DT_COND_T) 5)    /**<Condition:Local timezone updated        */
#define      DT_NO_SYNC_SRC                 ((DT_COND_T) 6)    /**<Condition:No sync source        */
#define      DT_IS_RUNNING                  ((DT_COND_T) 7)    /**<Condition:Is running        */
#define      DT_DAY_LGT_SAV_CHANGED         ((DT_COND_T) 8)    /**<Condition:Daylight saving changed        */
#define      DT_TZ_OFFSET_CHANGED           ((DT_COND_T) 9)    /**<Condition:Timezone offset changed        */

#define      DTT_NFY_T         ((HT_GROUP_DT)+((HANDLE_TYPE_T) 0))    /**<Handle type:Notify        */
#define      DTT_CLOCK_T       ((HT_GROUP_DT)+((HANDLE_TYPE_T) 1))    /**<Handle type:Clock        */

/* DT library configuration flag.

   DT_USE_DST_AT_SPECIFIED_TIME:
   DT_USE_DST_AT_CURRENT_TIME:

   Flags to specify which DST state should be used for the UTC to local
  
   For example, if specified UTC time to be coverted to local is a time 
   in the future (e.g., after the DST transition has taken place), but
   currently (e.g., at the time when API is called to make this UTC to
   local conversion), the DST transition has not take place.

   DT_USE_DST_AT_CURRENT_TIME" this flag specifies the conversion should
   use the current DST state, e.g., the DST state when the UTC-local
   conversion API is called.

   DT_USE_DST_AT_SPECIFIED_TIME: this flag specified the conversion
   should use the DST state (if known) at the UTC time (this time
   could be sometime in the future) that is specified
   for conversion.
*/

#define      DT_USE_DST_AT_SPECIFIED_TIME              2    /**<UTC to local: use special daylight saving        */
#define      DT_USE_DST_AT_CURRENT_TIME                4    /**<UTC to local: use current time         */
#define      DT_DVB_MATCH_CNTRY_AND_REG_CODE           8    /**<DVB match type:use country code and region code        */
#define      DT_DVB_MATCH_CNTRY_CODE                   16   /**<DVB match type:use country code*/
#define      DT_DVB_MATCH_ANY_CNTRY_OR_REG_CODE        32   /**<DVB match type:use any country or region code                */
#define      DT_ANALOG_USE_TELETEXT_LOCAL_TIME_OFFSET  64   /**<Analog: use teletext local time offset        */
#define      DT_ANALOG_UNSET_CONFIG_SETTING            128  /**<Analog: unset config */
#define      DT_DVB_LTO_CHECK_CHG_TIME                 256  /**<DT_DVB_LTO_CHECK_CHG_TIME                 */
#define      DT_DVB_LTO_RAW_TZ_WHEN_DST_CTRL_OFF       512  /**<*DT_DVB_LTO_RAW_TZ_WHEN_DST_CTRL_OFF*/




/*
  The following definitions shall comply with those in dt_brdcst_api.h
*/
#define DT_STT_IN_BAND_TBL_ID      ((UINT8) 0xcd) /**< Table ID for the ATSC SST carried in with in-band data stream.*/
#define DT_STT_OUT_OF_BAND_TBL_ID  ((UINT8) 0xc5) /**< Table ID for the ATSC STT carried in out-of-band data stream.*/
#define DT_DVB_TDT_TBL_ID          ((UINT8) 0x70) /**< Table ID for the DVB Time and Date (TDT) section.  */
#define DT_DVB_TOT_TBL_ID          ((UINT8) 0x73) /**< Table ID for the DVB Time Offset Table (TOT) section.  */
#define DT_VBI_TELETEXT_TBL_ID     ((UINT8) 0xFF) /**< Define an ID for VBI Teletext, so we can identify the source of analog current time/date information.  */



/*
  Enum for specifying the synchronization source for the current
  system time.

  The synchronization sources can be from on of the MPEG2 tables,
  ((TDT, TOT, or STT), or set by the user, or some other mechanisms which
  will be defined in the future.

*/
/*------------------------------------------------------------------*/
/*! @enum DT_SYNC_SRC_TYPE_T
 *  @brief Enum for specifying the source to synchronize the current time
 *  @code
 *  typedef enum
 *  {
 *      DT_SYNC_SRC_NONE = 0,
 *      DT_SYNC_WITH_DVB_TDT,
 *      DT_SYNC_WITH_DVB_TOT,
 *      DT_SYNC_WITH_ATSC_STT,
 *      DT_SYNC_WITH_SCTE_STT,          
 *      DT_SYNC_WITH_DVB_TDT_OR_TOT,
 *      DT_SYNC_WITH_DVB_TOT_OR_TDT,
 *      DT_SYNC_SRC_ANALOG_NONE,
 *      DT_SYNC_WITH_ANALOG_TELETEXT
 *  }  DT_SYNC_SRC_TYPE_T ;
 *  @endcode
 *  @li@c  DT_SYNC_SRC_NONE                      - Synchronization will be done manually (by user)
 *  @li@c  DT_SYNC_WITH_DVB_TDT                  - Synchronize with DVB TDT table.
 *  @li@c  DT_SYNC_WITH_DVB_TOT                  - Synchronize with DVB TOT table
 *  @li@c  DT_SYNC_WITH_ATSC_STT                 - Synchronize with ATSC STT table
 *  @li@c  DT_SYNC_WITH_SCTE_STT                 - Synchronize with cable out-of-band (OOB) STT table
 *  @li@c  DT_SYNC_WITH_DVB_TDT_OR_TOT           - Initially, the broadcast clock accepts either TDT or TOT table
 *                                                                        as synchronization source. But once a TDT table is received, 
 *                                                                        then only current time from TDT is accepted. All TOT tables 
 *                                                                        received after the TDT table is received are discarded.  
 *                                                                        If TOT table is received first, then set the time using TOT table,
 *                                                                        but if subsequently a TDT table is received, then only use the
 *                                                                        TDT table to set the system there after
 *  @li@c  DT_SYNC_WITH_DVB_TOT_OR_TDT           - Initially, the broadcast clock accepts either TOT or TDT table 
 *                                                                        as synchronization source.  But once a TOT table is received, 
 *                                                                        then only current time from TOT is accepted. All TDT tables 
 *                                                                        received after the TOT table is received are discarded.  
 *                                                                        If TDT table is received first, then set the time using TDT table, 
 *                                                                        but if subsequently a TOT table is received, then only use the
 *                                                                        TOT table to set the system there after
 *  @li@c  DT_SYNC_SRC_ANALOG_NONE               - No sync source while tune to analog broadcast
 *  @li@c  DT_SYNC_WITH_ANALOG_TELETEXT          - Synchronize with current time and date information carried 
 *                                                                        in the teletext stream
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DT_SYNC_SRC_NONE = 0,
    DT_SYNC_WITH_DVB_TDT,
    DT_SYNC_WITH_DVB_TOT,
    DT_SYNC_WITH_ATSC_STT,
    DT_SYNC_WITH_SCTE_STT,          /* For Cable out-of-band source. */
    DT_SYNC_WITH_DVB_TDT_OR_TOT,
    DT_SYNC_WITH_DVB_TOT_OR_TDT,
    DT_SYNC_SRC_ANALOG_NONE,
    DT_SYNC_WITH_ANALOG_TELETEXT
}  DT_SYNC_SRC_TYPE_T ;

/*
  Enum to describe the characteristics of the synchronization source.
  For example, given a synchronization type, it source might be from
  MPEG2 broadcast, or TCP-IP, etc.

  Currently, only one source characteristic type is defined, e.g.,
  for MPEG2 Broadcast source.  In the future, if other source type
  (component id, tcp-ip) are defined, we will add additional definition
  here.

*/
/*------------------------------------------------------------------*/
/*! @enum DT_SRC_TYPE_DESC_T
 *  @brief ENUM type describing the characteristics of the synchronization source.  
 *            For example, given a synchronization type, it source might be from MPEG2 
 *            broadcast, or TCP-IP, etc. 
 * 
 *           Currently, several source characteristic types are defined, e.g., 
 *           for MPEG2 broadcast source, and for VBI teletext (from analog terrestrial 
 *           source, or from analog cable source, or from analog satellite source).  In
 *           the future, other source types will be added if new source types are 
 *           supported
 *  @code
 *  typedef enum
 *  {
 *      DT_SRC_TYPE_FORMAT_UNKNOWN = 0,
 *      DT_SRC_TYPE_MPEG_2_BRDCST,
 *      DT_SRC_TYPE_VBI_ANA_TER,
 *      DT_SRC_TYPE_VBI_ANA_CAB,
 *      DT_SRC_TYPE_VBI_ANA_SAT,
 *      DT_SRC_TYPE_CONN_HANDLE
 *  }  DT_SRC_TYPE_DESC_T ;
 *  @endcode
 *  @li@c  DT_SRC_TYPE_FORMAT_UNKNOWN     -Synchronization source:Format unknown
 *  @li@c  DT_SRC_TYPE_MPEG_2_BRDCST      -Synchronization source:MPEG 2 broadcase
 *  @li@c  DT_SRC_TYPE_VBI_ANA_TER        -Synchronization source:analog terrestrial source
 *  @li@c  DT_SRC_TYPE_VBI_ANA_CAB        -Synchronization source:analog cable source
 *  @li@c  DT_SRC_TYPE_VBI_ANA_SAT        -Synchronization source:analog satellite source
 *  @li@c  DT_SRC_TYPE_CONN_HANDLE        -Synchronization source:connect handle
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DT_SRC_TYPE_FORMAT_UNKNOWN = 0,
    DT_SRC_TYPE_MPEG_2_BRDCST,
    DT_SRC_TYPE_VBI_ANA_TER,
    DT_SRC_TYPE_VBI_ANA_CAB,
    DT_SRC_TYPE_VBI_ANA_SAT,
    DT_SRC_TYPE_CONN_HANDLE
}  DT_SRC_TYPE_DESC_T ;

/*
    Calendar Time (Date-Time-Group structure)
*/
/*------------------------------------------------------------------*/
/*! @struct DTG_T
 *  @brief  Structure for Date Time Group, it represent field for a calendar date
 *  @code
 *  typedef struct _DTG_T_ {
 *      UINT16 ui2_yr;
 *      UINT8  ui1_mo;      
 *      UINT8  ui1_day;     
 *      UINT8  ui1_dow;     
 *      UINT8  ui1_hr;      
 *      UINT8  ui1_min;     
 *      UINT8  ui1_sec;     
 *      BOOL   b_gmt;       
 *      BOOL   b_dst;       
 *  } DTG_T ;
 *  @endcode
 *  @li@c  ui2_yr       - Year,  4 digit
 *  @li@c  ui1_mo       - 1-12, month of the year
 *  @li@c  ui1_day      - 1-31, day of the month.
 *  @li@c  ui1_dow      - 0-6, Sunday to Saturday
 *  @li@c  ui1_hr       - Hours: 0-23
 *  @li@c  ui1_min      - Minutes: 0-59
 *  @li@c  ui1_sec      - Seconds: 0-61
 *  @li@c  b_gmt        - TRUE: Universal Time Coordinate (UTC)
 *                               FALSE: Local Time
 *  @li@c  b_dst        - TRUE:  DayLight-Saving-Time on
 *                               FALSE: Day-Light-Saving Time off
 */
/*------------------------------------------------------------------*/
typedef struct _DTG_T_ {
    UINT16 ui2_yr;
    UINT8  ui1_mo;      /* 1-12, month of the year. */
    UINT8  ui1_day;     /* 1-31, day of the month. */
    UINT8  ui1_dow;     /* 0-6, Sunday to Saturday. */
    UINT8  ui1_hr;      /* 0-23 */
    UINT8  ui1_min;     /* 0-59 */
    UINT8  ui1_sec;     /* 0-61 */

    BOOL   b_gmt;       /* TRUE:  Universal Time Coordinate (UTC)
                           FALSE: Local Time
                        */
    BOOL   b_dst;       /* TRUE:  DayLight-Saving-Time on
                           FALSE: Day-Light_Saving Time off
                        */
} DTG_T ;

/*
    Delta time structure.
*/
/*------------------------------------------------------------------*/
/*! @struct DELTA_DTG_T
 *  @brief Structure for time difference between two Date Time Group, it is represented 
 *             in calendar date format 
 *  @code
 *  typedef struct _DELTA_DTG_T_
 *  {
 *      TIME_T  t_day;
 *      TIME_T  t_hr;
 *      TIME_T  t_min;
 *      TIME_T  t_sec;
 *  } DELTA_DTG_T;
 *  @endcode
 *  @li@c  t_day      - Day
 *  @li@c  t_hr       - Hour
 *  @li@c  t_min      - Minute
 *  @li@c  t_sec      - Second
 */
/*------------------------------------------------------------------*/
typedef struct _DELTA_DTG_T_
{
    TIME_T  t_day;
    TIME_T  t_hr;
    TIME_T  t_min;
    TIME_T  t_sec;
} DELTA_DTG_T;

/*
  Configuration structure for the DateTime library.

  Depending on the sync source and its desc type, the 3rd parameter
  of this DT_CONFIG_T structure, 'pv_src_info' will point to different data.
  The data is used to describes the characteristics of the source type.

  For example, if the 't_src_desc' is DT_SRC_TYPE_MPEG_2_BRDCST, then
  the 'pv_src_info' will point to a string containing the source name
  (maximum 12 valid characters + an Null byte) which table manager can
  used to connect to the source.

*/
/*------------------------------------------------------------------*/
/*! @struct DT_CONFIG_T
 *  @brief Structure for the DateTime library configuration
 *  @code
 *  typedef struct _DT_CONFIG_T_
 *  {
 *      DT_SYNC_SRC_TYPE_T    t_src_type;
 *      DT_SRC_TYPE_DESC_T    t_src_desc;
 *      VOID*                            pv_src_info;
 *  } DT_CONFIG_T;
 *  @endcode
 *  @li@c  t_src_type    - specifies the synchronization source type
 *  @li@c  t_src_desc    - specifies the source characteristic
 *  @li@c  pv_src_info   - pointer to source type specific structure describing the 
 *                                  characteristics of the source.  For example, 
 *                                  if the 't_src_desc' is DT_SRC_TYPE_MPEG_2_BRDCST,
 *                                  then the 'pv_src_info' point to a string containing the
 *                                  source name
 */
/*------------------------------------------------------------------*/
typedef struct _DT_CONFIG_T_
{
    DT_SYNC_SRC_TYPE_T    t_src_type;
    DT_SRC_TYPE_DESC_T    t_src_desc;
    VOID*                 pv_src_info;
} DT_CONFIG_T;

/*
  Prototype for the client notification function. This notification
  function is called when system clock is initialized or synchronized
  with the current time value from the STT, TOT, or TDT., or when system
  clock become free running state.
*/
/*------------------------------------------------------------------*/
/*! @brief  Function prototype for the notification function called when system clock
 *               condition is changed
 *
 *  @param [in]   h_hdl       - Handle to registered function 
 *  @param [in]   pv_tag      - Tag value specified by the caller when the caller
 *                                        register the callback function. 
 *  @param [in]   t_dt_cond   - The condition of the system clock state: DT_NOT_RUNNING, 
 *                                        DT_FREE_RUNNING, DT_SYNC_RUNNING, DT_SYNC_DISCONT. 
 *  @param [in]   t_delta     - The discontinuity(positive or negative value) between the SST
 *                                        time received from the transport stream and the current 
 *                                        system time.  This value is computed as:
 *
 *                                        t_delta = STT - current system time. 
 *
 *                                         For example,  if t_delta is a positive value (5 seconds), it indicates 
 *                                         that the STT time received is 5 seconds ahead of the current 
 *                                         system time.  
 *
 *                                         If the t_delta is a negative value, for example, -60 seconds, 
 *                                         then it indicates the STT is behind the current system time by 
 *                                         60 seconds.  This situation can happen if the stream is play-back.
 * 
 *
 *
 *  @note The t_delta value is only valid if the notification reason is DT_SYNC_DISCONT. 
 *             For other notification reasons, the 't_delta' is 0.
 *  @see
 *
 *  @return VOID.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_dt_nfy_fct)
(
    HANDLE_T     h_hdl,
    VOID*        pv_tag,
    DT_COND_T    t_dt_cond,
    TIME_T       t_delta
);

/**********************************************************************/
/* Enum for check_time API set paramters. */
/**********************************************************************/
/*------------------------------------------------------------------*/
/*! @enum DT_CHECK_TIME_CONFIG
 *  @brief Enum configuration to check received current time
 *  @code
 *  typedef enum
 *  {
 *      DT_USE_DEFAULT_CONFIG = 0,
 *      DT_SET_CONSECUTIVE_VAL_TO_CHK,
 *      DT_SET_TIME_WINDOW_ADJUSTMENT,
 *      DT_REJECT_BRDCST_TIME_BEFORE_THIS_DATE
 *  } DT_CHECK_TIME_CONFIG;
 *  @endcode
 *  @li@c  DT_USE_DEFAULT_CONFIG                          - Revert to default parameters
 *  @li@c  DT_SET_CONSECUTIVE_VAL_TO_CHK                  - Specifies number of consecutive
 *                                                                                       good input value to be processed 
 *                                                                                       before accepting the next good 
 *                                                                                       input time value
 *  @li@c  DT_SET_TIME_WINDOW_ADJUSTMENT                  - Specifies the delta value (tolerance)
 *                                                                                        between the time difference between 
 *                                                                                        the current and previous one received 
 *                                                                                        for the input time, and the time difference
 *                                                                                       between the current and last reading of system
 *                                                                                       time values
 *  @li@c  DT_REJECT_BRDCST_TIME_BEFORE_THIS_DATE  - specifies the gross error check be performed
 *                                                                                        on the input broadcast time.  If the intput broadcast 
 *                                                                                        time is older than the specified the date, then it is
 *                                                                                        rejected
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DT_USE_DEFAULT_CONFIG = 0,
    DT_SET_CONSECUTIVE_VAL_TO_CHK,
    DT_SET_TIME_WINDOW_ADJUSTMENT,
    DT_REJECT_BRDCST_TIME_BEFORE_THIS_DATE
} DT_CHECK_TIME_CONFIG;


#endif /* _U_DT_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                                /* end of groupMW_DT */
/*----------------------------------------------------------------------------*/
