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
/*! @file x_dt.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Date Time related API definitions,
 *         which are exported to other Middleware components.
 *
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_DT_H_
#define _X_DT_H_

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_dt.h"

/*****************************************************************
  	functions declarations
****************************************************************/


/*------------------------------------------------------------------*/
/*! @brief  Set the timezone offset to be used in the system time
 *               library for computing the local time from GMT and  vice versa.
 *               The timezone offset is specified as the number of seconds
 *               (+/-) from the Greenwich timezone.
 *
 *               For example:
 *
 *               For the Pacific Time zone (Westcoast U.S),
 *               the timezone offset is:
 *
 *                  t_tz_offset = (-8 hour * 3600 sec/hour )
 *                                  = -28800 seconds.
 *
 *  @param [in]    t_tz_offset   -  timezone offset in number of seconds.
 *
 *  @param [out]  None
 *
 *  @note
 *  @see
 *
 *  @return  VOID
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern
VOID x_dt_set_tz(TIME_T t_tz_offset);


/*------------------------------------------------------------------*/
/*! @brief	Get the time zone offset used in the system time library
 *				for computing the local time from GMT and vice versa.
 *				Return timezone offset value as number of seconds.
 *
 *				For example, for the Pacific Time zone, the returned value
 *				is
 *
 *				   (-8 Hour ) * 3600 sec/hour = -28800 sec
 *
 *	@param [in]  VOID
 *
 *	@param [out] None
 *
 *	@note
 *	@see
 *
 *	@return  Return the timezon value as the number of seconds.
 *	@retval 	 -
 */
/*------------------------------------------------------------------*/
extern
TIME_T x_dt_get_tz(VOID);



extern
VOID x_dt_set_ds_change(TIME_T t_ds_change);



/*------------------------------------------------------------------*/
/*! @brief  Get UTC time (in sec) when DST change will occur.
 *
 *  @param [in]  VOID
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *
 *  @return  Return the UTC time.
 *  @retval      -
 */
/*------------------------------------------------------------------*/

extern 
TIME_T x_dt_get_ds_change(VOID);




extern
VOID x_dt_set_ds_offset(TIME_T t_ds_offset);


/*------------------------------------------------------------------*/
/*! @brief  Get DST offset.
 *
 *  @param [in]  VOID
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *
 *  @return  Return the DST offset.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern 
TIME_T x_dt_get_ds_offset(VOID);



/*------------------------------------------------------------------*/
/*! @brief Set the system country code. If the received system time table contains
 *				 a matching country code, it will be   used to adjust the time zone.
 *
 *	@param [in]  t_count_code  - Contains the system country code.
 *	@param [in]  ui2_region_id	- Contains a system region id.
 *
 *	@param [out] None
 *
 *	@note
 *	@see
 *
 *	@return  Return the error code.
 *	@retval    DTR_OK	 - The routine was successful.
 *	@retval    DTR_FAIL  - The routine was failed.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_dt_set_sys_count_code (ISO_3166_COUNT_T  t_count_code,
                               UINT16            ui2_region_id);


/*------------------------------------------------------------------*/
/*! @brief	Get the system country code and region id.
  *
  *	@param [in] None
  *
  *	@param [out]   pt_count_code  - Contains the system country code.
  *	@param [out]   pui2_region_id  - Contains the system region id.
  *
  *	@note
  *	@see
  *
  *	@return  Return the error code.
  *	@retval    DTR_OK	 - The routine was successful.
  *	@retval    DTR_FAIL  - The routine was failed.
  */
 /*------------------------------------------------------------------*/
extern
INT32 x_dt_get_sys_count_code (ISO_3166_COUNT_T*  pt_count_code,
                               UINT16*            pui2_region_id);


/*------------------------------------------------------------------*/
/*! @brief Get a country code as signaled in the system time /date table.
 *
 *  @param [in]    ui2_idx   - Contains an index.
 *
 *  @param [out]  pt_count_code  - Contains the country code.
 *  @param [out]  pui2_region_id  - Contains the region id.
 *  @param [out]  pt_tz_offset      - The time offset for the country code and region.
 *
 *  @note
 *  @see
 *
 *  @return  Return the error code.
 *  @retval    DTR_OK    - The routine was successful.
 *  @retval    DTR_FAIL  - The routine was failed.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_dt_get_count_code (UINT16             ui2_idx,
                           ISO_3166_COUNT_T*  pt_count_code,
                           UINT16*            pui2_region_id,
                           TIME_T*            pt_tz_offset);


/*------------------------------------------------------------------*/
/*! @brief Get the number of country code's signaled in the system  time / date table.
 *
 *  @param [in] VOID
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *
 *  @return   Return the number of individual country code entries.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern
UINT16 x_dt_get_num_count_code (VOID);


/*------------------------------------------------------------------*/
/*! @brief  Set/Unset the current Daylight-Saving-Time.
 *
 *  @param [in]  b_dls - Flag to enable/disable DST. *
 *                     TRUE  -Enable Daylight-Saving-Time.
 *                     FALSE -Disable Daylight-Saving-Time.
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern
VOID x_dt_set_dst(BOOL b_dls);


/*------------------------------------------------------------------*/
/*! @brief  Get the current Daylight-Saving-Time.
 *
 *  @param [in] VOID
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *
 *  @return  Return the Dalylight-Saving-Time.
 *  @retval    TRUE  - Daylight-Saving-Time on.
 *  @retval    FALSE - Daylight-Saving-Time off.
 */
/*------------------------------------------------------------------*/
extern
BOOL x_dt_get_dst(VOID);

/*------------------------------------------------------------------*/
/*! @brief  Set/Unset if Daylight-Saving-Time should be apply to  this locality. 
 *              In some states (Arizona, Hawaii, and  Indiana) the Day-Light-Saving is 
 *              not used.  Thus for  these locality, the DST control should be set to
 *              FALSE, so that any daylight-saving parameters received  from the
 *              broadcast stream will be ignored.
 *
 *  @param [in]  b_dls    - Flag to enable/disable DST control.
 *                     TRUE     -Enable Daylight-Saving-Time control.
 *                     FALSE    -Disable Daylight-Saving-Time control.
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *
 *  @return
 *  @retval  -
 */
/*------------------------------------------------------------------*/
extern
VOID x_dt_set_dst_ctrl(BOOL b_dls);


/*------------------------------------------------------------------*/
/*! @brief Get the flag indicating if the Daylight-Saving-Time should  be applied
 *              for this locality.
 *
 *  @param [in] VOID
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *
 *  @return  Daylight-Saving-Time control.
 *  @retval  TRUE  - Daylight-Saving-Time control is on for this location.
 *  @retval  FALSE - Daylight-Saving-Time control is off for this location.
 */
/*------------------------------------------------------------------*/
extern
BOOL x_dt_get_dst_ctrl(VOID);

/*------------------------------------------------------------------*/
/*! @brief Get the last effective table ID used for time sync'ing.
 *         For ATSC, result may be STT (in/out-band); for DVB, 
 *         result may be TDT or TOT; for analog, result may be VBI
 *         teletext. If DT never sync'ed, return zero.
 *  @return  Table ID for the last effective time sync.
 *  @retval  DT_STT_IN_BAND_TBL_ID        -   STT (in-band)
 *  @retval  DT_STT_OUT_OF_BAND_TBL_ID    -   STT (out-band)
 *  @retval  DT_DVB_TDT_TBL_ID            -   TDT
 *  @retval  DT_DVB_TOT_TBL_ID            -   TOT
 *  @retval  DT_VBI_TELETEXT_TBL_ID       -   VBI Teletext
 */
/*------------------------------------------------------------------*/
extern
UINT8 x_dt_get_last_sync_tbl_id(VOID);

/*------------------------------------------------------------------*/
/*! @brief Get the current system clock time in UTC time frame,   the result is 
 *             returned as the number of seconds from  a default epoch of
 *             00:00:00Z, Jan 1, 1970.
 *
 *  @param [in] None
 *
 *  @param [out]  pui2_milli_sec -  Optional parameter to store the number of
 *                                                 milliseconds (0 to 999). If the parameter is NULL,  
 *                                                 milliseconds is ignored.
 *  @param [out]   pt_dt_cond     - The condition of the system clock: DT_NOT_RUNNING,
 *                                                 DT_FREE_RUNNING, DT_SYNC_RUNNING.  If NULL, 
 *                                                 then  condition code is  ignored.
 *
 *  @note
 *  @see
 *
 *  @return  Return the number of seconds since default epoch for the current  UTC time.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern
TIME_T x_dt_get_utc(UINT16*  pui2_milli_sec, DT_COND_T* pt_dt_cond);

/*------------------------------------------------------------------*/
/*! @brief  Set the current system clock time in UTC time frame.
 *
 *  @param [in]  t_sec            - The number of seconds since the default epoch, 00:00:00Z,
 *                                            Jan 1, 1970.
 *  @param [in]  ui2_milli_sec  -Optional parameter to set the number of milliseconds
 *                                            (0 to 999).
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *
 *  @return  Return the error code.
 *  @retval    DTR_OK        -  Success
 *  @retval    DTR_FAIL      -  Error on setting time on the Real-Time-Clock.
 *  @retval    DTR_NO_RTC - No Real-Time-Clock on the system platform.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_dt_set_utc
(
    TIME_T   t_tsec,
    UINT16   ui2_milli_sec
);


/*------------------------------------------------------------------*/
/*! @brief Get the current system clock time as the number of econds since 
 *              the GPS epoch, e.g, January 6, 1980,  and also the number of leap
 *              seconds since the GPS   epoch.
 *
 *              Note: As of 2005, the GPS time is ahead of UTC by  13 seconds.
 *
 *  @param [in] None.
 *
 *  @param [out]  pi4_gps_leap_sec - Number of GPS_UTC leap seconds (UTC) since
 *                                                    GPS epoch.
 *  @param [out] pui2_milli_sec	    - Optional parameter to store the number of
 *                                                     milliseconds (0 to 999). If this parameter is  NULL,
 *                                                     milliseconds is ignored.
 *
 *  @note
 *  @see
 *
 *  @return Return the number of seconds from the GPS epoch (00:00:00Z, Jan 06, 1980).
 *  @retval     
 */
/*------------------------------------------------------------------*/
extern
TIME_T x_dt_get_gps
(
    INT32* pi4_gps_leap_sec,
    UINT16*	pui2_milli_sec
);


/*------------------------------------------------------------------*/
/*! @brief Conversion from the number of GPS elapse seconds since  default epoch 
 *              to GMT YYYYMMDDHHMMSS.
 *
 *  @param [in]   t_gps_sec    -Number of elapse seconds from default epoch in GPS
 *                                          time frame.
 *
 *  @param [out]  pt_dtg        -Structure containing Day-Time-Group values in UTC
 *                                           time scale.
 *
 *  @note
 *  @see
 *
 *  @return  Return the error code.
 *  @retval  DTR_OK            -The routine was successful.
 *  @retval   DTR_INV_ARG  - pt_utc dtg is NULL.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_dt_gps_sec_to_dtg
(
    TIME_T t_gps_sec,
    DTG_T* pt_dtg
);

/*------------------------------------------------------------------*/
/*! @brief Conversion from the number of UTC elapse seconds since  default epoch 
 *              to GMT YYYYMMDDHHMMSS.
 *
 *  @param [in]   t_utc_sec    -Number of elapse seconds from default epoch in UTC
 *                                          time frame.
 *
 *  @param [out]  pt_dtg       -Structure containing Day-Time-Group values in UTC
 *                                           time scale.
 *
 *  @note
 *  @see
 *
 *  @return  Return the error code.
 *  @retval  DTR_OK            -The routine was successful.
 *  @retval   DTR_INV_ARG  - pt_utc dtg is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_dt_utc_sec_to_dtg
(
    TIME_T t_utc_sec,
    DTG_T* pt_dtg
);

/*------------------------------------------------------------------*/
/*! @brief   Conversion from the number of UTC elapse seconds since default 
 *                epoch to YYYYMMDDHHMMSS in local time scale.
 *
 *  @param [in]   t_utc_sec     - Number of elapse seconds from default epoch in UTC
 *                                            time frame.
 *
 *  @param [out]  pt_dtg        - Structure containing Day-Time-Group values in local
 *                                            time scale.
 *
 *  @note
 *  @see
 *
 *  @return  Return the error code.
 *  @retval    DTR_OK          - The routine was successful.
 *  @retval    DTR_INV_ARG - pt_loc_dtg dtg is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_dt_utc_sec_to_loc_dtg
(
    TIME_T t_utc_sec,
    DTG_T* pt_dtg
);

/*------------------------------------------------------------------*/
/*! @brief   Convert between UTC day-time-group to Local 
 *                day-time-group (take into account of day-light-saving
 *                time and timezone).
 *
 *                If the input dtg is local, then convert the local dtg value to UTC dtg value; 
 *                else if the input dtg is UTC  dtg, then convert the UTC dtg value to Loca
 *                dtg value.
 *
 *  @param [in]    t_input_dtg    - Input day-time-group value (local or UTC)
 *
 *  @param [out]   t_output_dtg - UTC or Local DTG calendar value.
 *
 *  @note
 *  @see
 *
 *  @return  Return he error code.
 *  @retval   DTR_OK          - The routine was successful.
 *  @retval   DTR_INV_ARG -  pt_output_dtg is NULL.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_dt_conv_utc_local(const DTG_T* t_input_dtg,  DTG_T* pt_output_dtg);


/*------------------------------------------------------------------*/
/*! @brief Compute the number of seconds for a specified DTG  from the default 
 *              epoch. If the input DTG is local,  then the difference is computed
 *              between local time and 00:00:00Z UTC Jan 1, 1970.
 *
 *  @param [in]  t_dtg  - Input day-time-group value (local or UTC)
 *
 *  @param [out]
 *  @note
 *  @see
 *  @return  Return number of seconds since the default epoch.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern
TIME_T x_dt_dtg_to_sec(const DTG_T *t_dtg);

/*------------------------------------------------------------------*/
/*! @brief  Convert GPS seconds to UTC seconds
 *
 *  @param [in]  t_gps_sec - Number of seconds since GPS epoch (0000Z, 06 Jan 1980)
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *
 *  @return  Return the number of seconds since UTC epoch (0000Z, 01 Jan 1970).
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern
TIME_T x_dt_gps_sec_to_utc_sec
(
    TIME_T	t_gps_sec
);

/*------------------------------------------------------------------*/
/*! @brief  Compute the difference in seconds between two DTG times
 *              delta seconds = t_dtg_to - t_dtg_from.
 *
 *  @param [in]  pt_dtg_from - Pointer to a Day-Time-Group (UTC or Local) structure.
  *  @param [in]  pt_dtg_to    - Pointer to a Day-Time-Group (UTC or Local) structure.
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *
 *  @return   Return the umber of seconds between two DTG values.   NULL_TIME if 
 *                'pt_dtg_from' or 'pt_dtg_to' is NULL.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern
TIME_T x_dt_diff(const DTG_T* pt_dtg_from, const DTG_T*  pt_dtg_to);


/*------------------------------------------------------------------*/
/*! @brief  Add or subtract seconds to a calendar time, and return  the new calendar time.
 *
 *  @param [in] pt_dtg_old     - Pointer to Day-Time-Group (UTC or Local) structure
 *  @param [in] t_sec             -  Number of second to be add/subtract (+/-)
 *
 *  @param [out]  pt_dtg_new -New Day-Time-Group (UTC or Local)
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval   DTR_OK          - The route was successful.
 *  @retval   DTR_INV_ARG - 'pt_dtg_old' or 'pt_dtg_new' is NULL pointer.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_dt_add(const DTG_T* pt_dtg_old, TIME_T t_sec, DTG_T* pt_dtg_new);

/*------------------------------------------------------------------*/
/*! @brief   Given a year, determine if it is a leap year.
 *
 *  @param [in]   t_yr  - Specify the year to be evaluated
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the special year is leap year or not.
 *  @retval   TRUE  - leap-year.
 *  @retval   FALSE - not a leap-year.
 */
/*------------------------------------------------------------------*/
extern
BOOL x_dt_leap_yr(TIME_T t_yr);


/*------------------------------------------------------------------*/
/*! @brief This API sets the synchronization source for the current  time.
 *
 *  @param [in]  t_sync_src_type - Specifies the types of synchronization source.
 *  @param [in]  t_src_type_desc - Specifies the source characteristic.
 *  @param [in]  pv_src_info        - Pointer to a structure containing information
 *                                                 about the synchronization source.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval   DTR_OK   - synchronization source is set.
 *  @retval   DTR_FAIL - Can not set the source of the synchronization for
 *                                  the current time.
 *
 */
/*------------------------------------------------------------------*/
extern
INT32 x_dt_set_sync_src
(
    DT_SYNC_SRC_TYPE_T          t_sync_src_type,
    DT_SRC_TYPE_DESC_T          t_src_desc_type,
    const VOID*                 pv_src_info
);


/*------------------------------------------------------------------*/
/*! @brief This API registers a notification function that will be called by the Date
 *              Time library when the system clock is synchronized with the current time 
 *              value from the STT,  TDT, or TOT, or when the system clock switches to
 *              a free running state.
 *
 *              The notification function will be called in the section manager callback context.
 *
 *  @param [in]  pf_nfy   - Notification function provided by the caller.  This notification
 *                                     function is called when the system clock  is synchronized with
 *                                     the current time value from STT,  or TOT, or TDT.
 *  @param [in]   pv_tag  - A tag value provided by the caller. This tag value will
 *                                     be passed to the caller provided notification function.
 *
 *  @param [out] ph_dt	  - Handle reference to the client's notification function.
 *
 *  @note  Up to a total of 16 notification functions can be registered.
 *  @see
 
 *  @return Return the error code.
 *  @retval   DTR_OK                                   - The notification function is registered.
 *  @retval   DTR_INV_ARG                          - pf_nfy or ph_dt is NULL.
 *  @retval   DTR_FAIL                                 - Internal error: can not registered the 
 *                                                                 notification  function.
 *  @retval   DTR_AEE_OUT_OF_RESOURCES - Can not allocate AEE resources.
 *  @retval    DTR_AEE_NO_RIGHTS              - No right for the AEE resources.
 */
/*------------------------------------------------------------------*/

extern
INT32 x_dt_reg_nfy_fct
(
    x_dt_nfy_fct   pf_nfy,
    VOID*          pv_tag,
    HANDLE_T*      ph_dt
);

/*------------------------------------------------------------------*/
/*! @brief This function converts the 40 bits string that consists of 16 LSBs MJD
 *              (Modified Julian Date) + 24 bits of BCD (binary code decimal) for hour,
 *               minutes, and seconds into a DTG_T structure.
 * 
 *    The 40 bits MJD+BCD data are received from DVB TDT and TOT table.
 *
 *  @param [in]  pui1_data    -  A 40 bits code string consisting of 16 LSB's MJD value and
 *                                           24 bits of BCD (binary code decimal)
 *
 *  @param [out]  pt_dtg       - Pointer to a DTG_T structure.
 *
 *  @note
 *  @see
 *
 *  @return  Return the error code.
 *  @retval   DTR_OK          -  Conversion is successful.
 *  @retval  DTR_INV_ARG  -  pt_dtg is NULL.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_dt_mjd_bcd_to_dtg(const UINT8* pui1_data,   DTG_T* pt_dtg);


 /*------------------------------------------------------------------*/
 /*! @brief This function converts the	24 bits of BCD (binary code decimal) for hour, 
  * 			 minutes, and seconds into number of seconds.
  *
  *  @param [in]  pui1_data   - A 24 bits BCD (binary code decimal) string for HHMMSS.
  *
  *  @param [out]  pt_sec:	  - Seconds from the BCD HHMMSS.
  *
  *  @note
  *  @see
  *
  *  @return Return the error code.
  *  @retval  DTR_OK   - conversion is successful.
  *  @retval  DTR_FAIL - conversion is failed.
  */
 /*------------------------------------------------------------------*/

extern  INT32   x_dt_bcd_to_sec(const UINT8* pui1_data,   TIME_T* pt_sec);

/*------------------------------------------------------------------*/
/*! @brief  Get the current broadcast time in UTC time frame,  the result is  returned
 *               as the number of seconds from  a default epoch of 00:00:00Z, Jan 1, 1970.
 *
 *  @param [in] None.
 *
 *  @param [out]  pui2_milli_sec  - Optional parameter to store the number of
 *                                                 milliseconds (0 to 999). If the parameter is NULL,
 *                                                 milliseconds is ignored.
 *
 *  @param [out]  pt_dt_cond	-The condition of the system clock: DT_NOT_RUNNING,
 *                                                 DT_FREE_RUNNING, DT_SYNC_RUNNING.  If NULL, then
 *                                                 condition code is ignored.
 *  @note
 *  @see
 *
 *  @return   Return the number of seconds since default epoch for the current.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern  TIME_T  x_dt_get_brdcst_utc(UINT16*  pui2_milli_sec,
                                    DT_COND_T* pt_dt_cond);

/*------------------------------------------------------------------*/
/*! @brief   Synchronize the user set clock time with the broadcast  time received.
 *
 *  @param [in]   b_sync_flag  - Flag to enable/disable user clock and broadcast clock
 *                                            are sync'd.
 *                                            TRUE:  user clock and broadcast clock are sync'd
 *                                            FALSE: user clock and broadcast clock are not sync'd.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  DTR_OK    - The route was successful.
 *  @retval  DTR_FAIL  - Internal error.
 */
/*------------------------------------------------------------------*/
extern  INT32   x_dt_sync_with_brdcst_utc(BOOL b_sync_flag);

/*------------------------------------------------------------------*/
/*! @brief Get the flag indicating if the timezone parameter received from the 
 *              broadcast table should  be applied for this locality.
 *
 *  @param [in] VOID.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return if use timezone information from broadcast table for this location or not.
 *  @retval  TRUE  - Use timezone information from broadcast table for  this location.
 *  @retval  FALSE - Do not use timezone information from broadcast table for this location.
 */
/*------------------------------------------------------------------*/
extern  BOOL    x_dt_get_tz_ctrl(VOID);

/*------------------------------------------------------------------*/
/*! @brief  Set/Unset if Timezone offset information received from  broadcast
 *                stream table should be apply to this locality.  By default, this setting
 *                is TRUE.  However, if the user want to explicitly specify the timezone 
 *                offset, then  it should call this API with FALSE value. This will ignore 
 *                any Timezone parameters received from the broadcast stream.
 *
 *  @param [in]  b_tz  - Flag to enable/disable timezone parameter from broadcast
 *                                stream.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return  None.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern  VOID    x_dt_set_tz_ctrl(BOOL b_tz);


/*------------------------------------------------------------------*/
/*! @brief  This function registers DVB table processing function  to the DT library.
 *
 *  @param [in] VOID
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *   
 *  @return Return error code.
 *  @retval  DTR_OK                 - Table section has been processed.
 *  @retval  DTR_WILL_NOTIFY  - Table section has been process and we need to
 *                                               notify the upper layer that system clock has
 *                                               changed into sync running state.
 *  @retval  DTR_FAIL               - SST table rejected.
 */
/*------------------------------------------------------------------*/
extern  INT32   x_dt_reg_dvb(VOID);


/*------------------------------------------------------------------*/
/*! @brief  This function register function table for parsing ATSC  table.
 *
 *  @param [in] VOID
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *
 *  @return  Return the error code.
 *  @retval  DTR_OK                - Table section has been processed.
 *  @retval  DTR_WILL_NOTIFY - Table section has been process and we need to
 *                                              notify the upper layer that system clock has
 *                                              changed into sync running state.
 *  @retval  DTR_FAIL              - SST table rejected.  
 */
/*------------------------------------------------------------------*/
extern  INT32   x_dt_reg_atsc(VOID);

/*------------------------------------------------------------------*/
/*! @brief This function registers teletext table processing function  to the DT library.
 *
 *  @param [in] VOID
 *
 *  @param [out] None
 *
 *  @note
 *  @see
 *
 *  @return
 *  @return  Return the error code.
 *  @retval  DTR_OK                - Table section has been processed.
 *  @retval  DTR_WILL_NOTIFY - Table section has been process and we need to
 *                                              notify the upper layer that system clock has
 *                                              changed into sync running state.
 *  @retval  DTR_FAIL              - SST table rejected.  
 */
/*------------------------------------------------------------------*/
extern  INT32   x_dt_reg_teletext(VOID);


/*------------------------------------------------------------------*/
/*! @brief   Return number of day from Jan 1 of a year from  a specified calendar date
 *
 *  @param [in]  pt_dtg - Structure containing Day-Time-Group values in Local
 *                                  time frame.
 *
 *  @param [out] pui2_day_of_yr - Contains the number of day from Jan 1 of a year.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  DTR_OK           - The routine is successful.
 *  @retval  DTR_INV_ARG  - pt_dtg or pui2_day_of_yr is NULL.
 */
/*------------------------------------------------------------------*/
extern  INT32   x_dt_day_of_year(const DTG_T* pt_dtg, UINT16*  pui2_day_of_yr);


/*------------------------------------------------------------------*/
/*! @brief  Compute the week of the year from the DTG.
 *
 *  @param [in]  t_dtg             - Calendar date (local or utc)
 *  @param [in] i4_flag            - Contains the week number 1 has first sunday or first mouday.
 *                       1:  Week number 1 has first Sunday,  previous days are week 0.
 *                       2:  Week number 1 has the first Monday;  previous days are week 0.
 *
 *  @param [out]  pi2_wk_of_yr  - week of year (0-53, Sun-Sat).
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  DTR_OK - The routine was successful.
 */
/*------------------------------------------------------------------*/
extern  INT32   x_dt_wk_of_yr(const DTG_T* pt_dtg, INT32 i4_flag, INT16* pi2_wk_of_yr);

/*------------------------------------------------------------------*/
/*! @brief This function converts the MJD value to a calendar date.
 *
 *  @param [in]  ui4_mjd   -  Modified-Julia-Date value (for example: 45000)
 *
 *  @param [out]  pt_dtg   -  Pointer to a DTG_T structure.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval   DTR_OK     - Conversion is successful.  Only the year, month, and date
 *                                   fields in the DTG_T structure are meaningful.
 */
/*------------------------------------------------------------------*/
extern  INT32   x_dt_mjd_to_dtg(UINT32 ui4_mjd, DTG_T* pt_dtg);

/*------------------------------------------------------------------*/
/*! @brief  This function converts a calendar value to the MJD date.
 *
 *  @param [in]  pt_dtg    -  Pointer to a DTG_T structure. Only the year,
 *                            month, and date fields in the DTG_T structure are
 *                            meaningful.
 *
 *  @param [out] pui2_mjd  -  Modified-Julia-Date value (for example: 45000).
 *
 *
 *  @return  Return the error code.
 *  @retval  DTR_OK    - Conversion is successful.  
 *  @retval DTR_FAIL  - The routine is fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_dt_dtg_to_mjd(const DTG_T*  pt_dtg, UINT16* pui2_mjd);

/*------------------------------------------------------------------*/
/*! @brief  This function converts a calendar value to the MJD date,
 *          BCD hour/minute and second.
 *          Note that the hour/minute 16-bit BCD code endian is 
 *          platform-specific. It may need endian conversion to
 *          reconstruct the 40-bit date time data.
 *
 *  @param [in]  pt_dtg    -  Pointer to a DTG_T structure. Only the year,
 *                            month, and date fields in the DTG_T structure are
 *                            meaningful.
 *
 *  @param [out] pui2_mjd  -  Modified-Julia-Date value.
 *  @param [out] pui2_hr_min- Hour/Minute in BCD format.
 *  @param [out] pui1_sec  -  Second in BCD format.
 *
 *
 *  @return  Return the error code.
 *  @retval  DTR_OK    - Conversion is successful.  
 *  @retval DTR_FAIL  - The routine is fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_dt_dtg_to_mjd_bcd(const DTG_T*  pt_dtg,
                                 UINT16*       pui2_mjd,
                                 UINT16*       pui2_hr_min,
                                 UINT8*        pui1_sec);


/*------------------------------------------------------------------*/
/*! @brief   Enable or dis-able the check input function.
 *
 *  @param [in]  b_enable -  Flag to enable/disable checking of input time.
 *                                       TRUE  : Enable checking of input time.
 *                                       FALSE: Dis-able checking of input time. (Every time
 *                                                  value received are accepted.)
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval  DTR_OK - The routine was successful.
 */
/*------------------------------------------------------------------*/
extern  INT32   x_dt_check_input_time(BOOL b_enable);

/*! @brief   Configure the check time engine. 
 *
 *  @param [in] e_set_parm - Specifies the parameter to set in the next argument: *
 *                                          DT_USE_DEFAULT_CONFIG:  Revert to default parameters.
  *                                         DT_SET_CONSECUTIVE_VAL_TO_CHK: Specifies number of
 *                                                                                                   of consecutive good input 
 *                                                                                                   value to be processed
 *                                                                                                   before accepting the next
 *                                                                                                   good input time value.
 *                                          DT_SET_TIME_WINDOW_ADJUSTMENT: Specifies the delta value
 *                                                                                                    (tolerance) between the time
 *                                                                                                    difference between the current
 *                                                                                                    and previous one received for
 *                                                                                                    the input time, and the  time 
 *                                                                                                    difference between the current
 *                                                                                                     and last reading of system time values.
 *
 *  @param [in] pv_value   - Pointer to the variable containg value to set.
 *                                       For DT_SET_CONSECUTIVE_VAL_TO_CHK and DT_SET_TIME_WINDOW_ADJUSTMENT, 
 *                                       this pointer  refer to a 'INT32' variable containing the set values. 
 *                                       For DT_USE_DEFAULT_CONFIG, this pointer is ignored.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return   Return the error code.
 *  @retval     DTR_OK : The routine was  successful.
 *  @retval     DTR_INV_ARG : the pv_value is NULL.              
 */
/*------------------------------------------------------------------*/

extern  INT32   x_dt_config_check_input_time(DT_CHECK_TIME_CONFIG  e_set_parm,
                                             VOID*                 pv_value);


/*------------------------------------------------------------------*/
/*! @brief This function set the configuration for DT library behavior.
 *
 *  @param [in] ui4_flag   - Masked flag.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return  Return the error code.
 *  @retval  DTR_OK   -The routine was successful.
 *  @retval  DTR_FAIL -Then routine was failed.
 */
/*------------------------------------------------------------------*/
extern  INT32   x_dt_set_config(UINT32  ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief   Initialize software clock component.
 *
 *  @param [in]  s_clock_name - Name of the clock to create.
 *  @param [in]  pf_nfy            - Notification function.
*   @param [in]  pv_tag            - Private tag pass to notification function.
 *
 *  @param [out] ph_clock       - Handle to the specified clock.
 *
 *  @note
 *  @see
 *
 *  @return  Return the error code.
 *  @retval  DTR_OK                       - The routing was successful.
 *  @retval  DTR_INV_ARG              - The ph_clock is NUL.
 *  @retval  DTR_FAIL                     - The routine was failed.
 *  @retval  DTR_OUT_OF_HANDLE  - The  clock number exceeds the max support clock number.
 *  @retval  DTR_OUT_OF_MEM       -  Allocate memory failed.
 */
/*------------------------------------------------------------------*/
extern  INT32   x_dt_clock_create
(
    CHAR*              s_clock_name,
    x_dt_nfy_fct       pf_nfy,
    VOID*              pv_tag,
    HANDLE_T*          ph_clock
);


/*------------------------------------------------------------------*/
/*! @brief Get the current broadcast clock time in UTC time frame,
 *              the result is  returned as the number of seconds from
 *              a default epoch of 00:00:00Z, Jan 1, 1970.
 *
 *              If the broadcast time has not bee set, then NULL_TIME is  returned.
 *
 *  @param [in]  h_clock             -  Handle to the clock.
 *
 *  @param [out] pui2_milli_sec	- Optional parameter to store the number of
 *                                                 milliseconds (0 to 999). If the parameter is NULL,
 *                                                 milliseconds is ignored.
 *
 *  @param [out]   pt_dt_cond	- The condition of the system clock: DT_NOT_RUNNING,
 *                                                 DT_FREE_RUNNING, DT_SYNC_RUNNING.  If NULL, then
 *                                                 condition code is ignored.
 *
 *  @note
 *  @see
 *
 *  @return  Return the number of seconds since default epoch for the current  UTC time.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern  TIME_T x_dt_clock_get_utc
(
    HANDLE_T         h_clock,
    UINT16*          pui2_milli_sec,
    DT_COND_T*       pt_dt_cond
);

/*------------------------------------------------------------------*/
/*! @brief  Set the clock time in UTC time frame.
 *
 *  @param [in] h_clock         -  Handle to the clock.
 *  @param [in]  t_sec           -  The number of seconds since the default epoch, 00:00:00Z,
 *                                            Jan 1, 1970.
 *
 *  @param [in] ui2_milli_sec  - Optional parameter to set the number of milliseconds
 *                                            (0 to 999).
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return  Return the error code.
 *  @retval   DTR_OK          - The routine was successflu.
 *  @retval   DTR_FAIL        -  Error on setting time on the Real-Time-Clock.
 *  @retval   DTR_NO_RTC  -  No Real-Time-Clock on the system platform.
 */
/*------------------------------------------------------------------*/
extern INT32 x_dt_clock_set_utc
(
    HANDLE_T         h_clock,
    TIME_T           t_sec,
    UINT16           ui2_milli_sec
);

/*------------------------------------------------------------------*/
/*! @brief This API sets the synchronization source for the current  time.
 *
 *  @param [in]  t_sync_src_type  - Specifies the types of synchronization source.
 *  @param [in]  t_src_type_desc  - Specifies the source characteristic.
 *  @param [in]  pv_src_info        -  Pointer to a structure containing information
 *                                                  about the synchronization source. 
 *
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return Return the error code.
 *  @retval   DTR_OK   - Synchronization source is set.
 *  @retval  DTR_FAIL  - Can not set the source of the synchronization for
 *                                  the current time. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_dt_clock_set_sync_src
(
    HANDLE_T                    h_clock,
    DT_SYNC_SRC_TYPE_T          t_sync_src_type,
    DT_SRC_TYPE_DESC_T          t_src_type_desc,
    const VOID*                 pv_src_info
);

#endif /* _X_DT_H_ */
