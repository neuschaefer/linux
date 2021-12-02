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
/*-----------------------------------------------------------------------------
 * $RCSfile: c_dt_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: c8679ab5c7a96d6727163313dde1dfdc $
 *
 * Description:
 *         This file contain implementation of Date Time API's,
 *         which are exported to application components.
 *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "dt/x_dt.h"
#include "handle/handle.h"
#include "aee/aee.h"

/*----------------------------------------------------------------------
 *
 * Name: c_dt_set_tz
 *
 * Description:  Set the timezone offset to be used in the system time
 *               library for computing the local time from GMT and
 *               vice versa.
 *               The timezone offset is specified as the number of seconds
 *               (+/-) from the Greenwich timezone.
 *               For example:
 *
 *               For the Pacific Time zone (Westcoast U.S),
 *               the timezone offset is:
 *
 *                    t_tz_offset = (-8 hour * 3600 sec/hour )
 *                                = -28800 seconds.
 *
 * Input:
 *    t_tz_offset:   timezone offset in number of seconds.
 *
 * Output:
 *    None
 *
 * Returns:  VOID.
 *
------------------------------------------------------------------------*/
VOID c_dt_set_tz(TIME_T t_tz_offset)
{
    x_dt_set_tz(t_tz_offset);
    return ;
}

/*----------------------------------------------------------------------
 *
 * Name: c_dt_get_tz
 *
 * Description: Get the time zone offset used in the system time library
 *              for computing the local time from GMT and vice versa.
 *              Return timezone offset value as number of seconds.
 *
 *              For example, for the Pacific Time zone, the returned value
 *              is
 *
 *                 (-8 Hour ) * 3600 sec/hour = -28800 sec
 *
 * Inputs: VOID
 *
 * Outputs: VOID
 *
 * Returns: The timezon value as the number of seconds.
 *
------------------------------------------------------------------------*/
TIME_T c_dt_get_tz(VOID)
{
    return x_dt_get_tz();
}

/*--------------------------------------------------------------------
 *
 * Name:  c_dt_set_tz_ctrl
 *
 * Description: Set/Unset if Timezone offset information received from
 *              broadcast stream table should be apply to this locality.
 *              By default, this setting is TRUE.  However, if the user
 *              want to explicitly specify the timezone offset, then
 *              it should call this API with FALSE value. This
 *              will ignore any Timezone parameters received
 *              from the broadcast stream.
 *
 * Inputs:
 *      b_tz:   Flag to enable/disable Timezone parameter from broadcast
 *              stream.
 *
 * Outputs:
 *      VOID.
 *
 * Returns:
 *     TRUE   enable Timezone control from broadcast table.
 *     FALSE  disable Timezone control from broadcast table.
-----------------------------------------------------------------------*/
VOID c_dt_set_tz_ctrl(BOOL b_tz)
{
    x_dt_set_tz_ctrl(b_tz);
    return ;
}

/*-----------------------------------------------------------------------
 *
 * Name: c_dt_get_tz_ctrl
 *
 * Description: Get the flag indicating if the timezone parameter
 *              received from the broadcast table should
 *              be applied for this locality.
 *
 * Inputs: None
 *
 * Outputs: None
 *
 * Returns: 
 *      TRUE   Use timezone information from broadcast table for
 *             this location.
 *      FALSE  Do not use timezone information from broadcast table
 *             for this location.
------------------------------------------------------------------------*/
BOOL c_dt_get_tz_ctrl(VOID)
{
    return x_dt_get_tz_ctrl();
}

/*----------------------------------------------------------------------
 *
 * Name: c_dt_set_sys_count_code
 *
 * Description: Set the system country code. If the received system
 *              time table contains a matching country code, it will be
 *              used to adjust the time zone.
 *
 * Inputs: t_count_code   Contains the system country code.
 *         ui2_region_id  Contains a system region id.
 *
 * Outputs:
 *      VOID
 *
 * Returns:
 *      VOID
 *
------------------------------------------------------------------------*/
INT32 c_dt_set_sys_count_code (ISO_3166_COUNT_T  t_count_code,
                              UINT16            ui2_region_id)
{
    return x_dt_set_sys_count_code (t_count_code, ui2_region_id);
}



/*----------------------------------------------------------------------
 *
 * Name: c_dt_get_sys_count_code
 *
 * Description: Get the system country code.
 *
 * Inputs:
 *      VOID
 *
 * Outputs:
 *      pt_count_code   Contains the system country code.
 *      pui2_region_id  Contains the system region id.
 *
 * Returns:
 *      DTR_OK if successful else a negative error code.
 *
------------------------------------------------------------------------*/
INT32 c_dt_get_sys_count_code (ISO_3166_COUNT_T*  pt_count_code,
                               UINT16*            pui2_region_id)
{
    return (x_dt_get_sys_count_code (pt_count_code, pui2_region_id));
}


/*----------------------------------------------------------------------
 *
 * Name: c_dt_get_count_code
 *
 * Description: Get a country code as signaled in the system time /
 *              date table.
 *
 * Inputs:
 *      ui2_idx  Contains an index.
 *
 * Outputs:
 *      pt_count_code   Contains the country code.
 *      pui2_region_id  Contains the region id.
 *      pt_tz_offset    The time offset for the country code and region.
 *
 * Returns:
 *      DTR_OK if successful else a negative error code.
 *
------------------------------------------------------------------------*/
INT32 c_dt_get_count_code (UINT16             ui2_idx,
                           ISO_3166_COUNT_T*  pt_count_code,
                           UINT16*            pui2_region_id,
                           TIME_T*            pt_tz_offset)
{
    return (x_dt_get_count_code (ui2_idx,
                                 pt_count_code,
                                 pui2_region_id,
                                 pt_tz_offset));
}


/*----------------------------------------------------------------------
 *
 * Name: c_dt_get_num_count_code
 *
 * Description: Get the number of country code's signaled in the system
 *              time / date table.
 *
 * Inputs:
 *      VOID.
 *
 * Outputs:
 *      VOID.
 *
 * Returns:
 *      The number of individual country code entries.
 *
------------------------------------------------------------------------*/
UINT16 c_dt_get_num_count_code (VOID)
{
    return (x_dt_get_num_count_code ());
}


/*--------------------------------------------------------------------
 *
 * Name:  c_dt_set_dst
 *
 * Description: Set/Unset the current Daylight-Saving-Time.
 *
 * Inputs:
 *      b_dls: Flag to enable/disable DST.
 *
 *                TRUE   enable Daylight-Saving-Time.
 *                FALSE  disable Daylight-Saving-Time.
 *
 * Outputs:
 *      VOID.
 *
 * Returns:
 *     VOID
-----------------------------------------------------------------------*/
VOID c_dt_set_dst(BOOL b_dls)
{
    x_dt_set_dst(b_dls);
    return ;
}

/*-----------------------------------------------------------------------
 *
 * Name: c_dt_get_dst
 *
 * Description: Get the current Daylight-Saving-Time.
 *
 * Inputs: None
 *
 * Outputs: None
 *
 * Returns:
 *      TRUE   Daylight-Saving-Time on.
 *      FALSE  Daylight-Saving-Time off.
------------------------------------------------------------------------*/
BOOL c_dt_get_dst(VOID)
{
    return x_dt_get_dst();
}


/*--------------------------------------------------------------------
 *
 * Name:  c_dt_set_dst_ctrl
 *
 * Description: Set/Unset if Daylight-Saving-Time should be apply to
 *              this locality. In some states (Arizona, Hawaii, and
 *              Indiana) the Day-Light-Saving is not used.  Thus for
 *              these locality, the DST control should be set to
 *              FALSE, so that any daylight-saving parameters received
 *              from the broadcast stream will be ignored.
 *
 * Inputs:
 *      b_dls: Flag to enable/disable DST.
 *
 * Outputs:
 *      VOID.
 *
 * Returns:
 *     TRUE   enable Daylight-Saving-Time control.
 *     FALSE  disable Daylight-Saving-Time control.
-----------------------------------------------------------------------*/
VOID c_dt_set_dst_ctrl(BOOL b_dls)
{
    x_dt_set_dst_ctrl(b_dls);
    return;
}


/*-----------------------------------------------------------------------
 *
 * Name: c_dt_get_dst_ctrl
 *
 * Description: Get the flag indicating if the Daylight-Saving-Time should
 *              be applied for this locality.
 *
 * Inputs: None
 *
 * Outputs: None
 *
 * Returns:
 *      TRUE   Daylight-Saving-Time control is on for this location.
 *      FALSE  Daylight-Saving-Time control is off for this location.
------------------------------------------------------------------------*/
BOOL c_dt_get_dst_ctrl(VOID)
{
    return x_dt_get_dst_ctrl();
}





TIME_T c_dt_get_ds_change(VOID)
{
    return x_dt_get_ds_change();
}

VOID c_dt_set_ds_change(TIME_T t_ds_change)
{
    x_dt_set_ds_change(t_ds_change);
}


TIME_T c_dt_get_ds_offset(VOID)
{
    return x_dt_get_ds_offset();
}

VOID c_dt_set_ds_offset(TIME_T t_ds_offset)
{
    x_dt_set_ds_offset(t_ds_offset);
}


/*-----------------------------------------------------------------------
 *
 * Name: c_dt_get_utc
 *
 * Description: Get the current system clock time in UTC time frame,
 *              the result is  returned as the number of seconds from
 *              a default epoch of 00:00:00Z, Jan 1, 1970.
 *
 * Inputs:
 *    None.
 *
 * Outputs:
 *    pui2_milli_sec   Optional parameter to store the number of
 *                     milliseconds (0 to 999). If the parameter is NULL,
 *                     milliseconds is ignored.
 *
 *    pt_dt_cond	   The condition of the system clock: DT_NOT_RUNNING,
 *                     DT_FREE_RUNNING, DT_SYNC_RUNNING.  If NULL, then
 *                     condition code is ignored.
 *
 *
 * Returns:
 *    The number of seconds since default epoch for the current
 *    UTC time.
-------------------------------------------------------------------------*/
TIME_T c_dt_get_utc(UINT16*  pui2_milli_sec, DT_COND_T* pt_dt_cond )
{
    return x_dt_get_utc(pui2_milli_sec, pt_dt_cond);
}

/*------------------------------------------------------------------------
 *
 * Name: c_dt_set_utc
 *
 * Description: Set the current system clock time in UTC time frame.
 *
 * Inputs:
 *
 * Input:
 *    t_sec:  The number of seconds since the default epoch, 00:00:00Z,
 *            Jan 1, 1970.
 *
 *    ui2_milli_sec  Optional parameter to set the number of milliseconds
 *                   (0 to 999).
 * Output:
 *    None
 *
 * Returns
 *    DTR_OK:  Success
 *    DTR_FAIL:  Error on setting time on the Real-Time-Clock.
 *    DTR_NO_RTC: No Real-Time-Clock on the system platform.
------------------------------------------------------------------------*/
INT32 c_dt_set_utc(TIME_T t_sec, UINT16 ui2_milli_sec)
{
    return x_dt_set_utc(t_sec, ui2_milli_sec);
}

/*-----------------------------------------------------------------------
 *
 * Name: c_dt_get_gps
 *
 * Description: Get the current system clock time as the number of
 *              seconds since the GPS epoch, e.g, January 6, 1980,
 *              and also the number of leap seconds since the GPS
 *              epoch.
 *
 *              Note: As of 2005, the GPS time is ahead of UTC by
 *                    13 seconds.
 * Inputs:
 *     None.
 *
 * Output:
 *     pi4_gps_leap_sec:   Number of GPS_UTC leap seconds (UTC) since
 *                         GPS epoch.
 *
 *     pui2_milli_sec	   Optional parameter to store the number of
 *                         milliseconds (0 to 999). If this parameter is
 *                         NULL, milliseconds is ignored.
 * Return:
 *    The number of seconds from the GPS epoch (00:00:00Z, Jan 06, 1980),
 *
------------------------------------------------------------------------*/
TIME_T c_dt_get_gps(INT32* pi4_gps_leap_sec, UINT16* pui2_milli_sec)
{
    return x_dt_get_gps(pi4_gps_leap_sec, pui2_milli_sec);
}

/*----------------------------------------------------------------------
 *
 * Name: c_dt_utc_sec_to_dtg
 *
 * Description: Conversion from the number of UTC elapse seconds since
 *              default epoch to GMT YYYYMMDDHHMMSS.
 *
 * Input:
 *     t_utc_sec:  number of elapse seconds from default epoch in UTC
 *                 time frame.
 *
 * Output:
 *     pt_dtg:     Structure containing Day-Time-Group values in UTC
 *                 time scale.
 *
 * Returns:
 *     DTR_OK:       Success.
 *     DTR_INV_ARG:  pt_utc dtg is NULL.
------------------------------------------------------------------------*/
INT32 c_dt_utc_sec_to_dtg(TIME_T t_utc_sec, DTG_T* pt_dtg)
{
    return x_dt_utc_sec_to_dtg(t_utc_sec, pt_dtg);
}

/*----------------------------------------------------------------------
 *
 * Name: c_dt_utc_sec_to_loc_dtg
 *
 * Description: Conversion from the number of UTC elapse seconds since
 *              default epoch to YYYYMMDDHHMMSS in local time scale.
 *
 * Input:
 *     t_utc_sec:  number of elapse seconds from default epoch in UTC
 *                 time frame.
 *
 * Output:
 *     pt_dtg:     Structure containing Day-Time-Group values in local
 *                 time scale.
 *
 * Returns:
 *     DTR_OK:       Success.
 *     DTR_INV_ARG:  pt_utc dtg is NULL.
------------------------------------------------------------------------*/
INT32 c_dt_utc_sec_to_loc_dtg(TIME_T t_utc_sec, DTG_T* pt_dtg)
{
    return x_dt_utc_sec_to_loc_dtg(t_utc_sec, pt_dtg);
}

/*------------------------------------------------------------------
 *
 * Name: c_dt_conv_utc_local
 *
 * Description: Convert between UTC day-time-group to Local
 *              day-time-group (take into account of day-light-saving
 *              time and timezone).
 *
 *              If the input dtg is local, then convert the local dtg
 *              value to UTC dtg value; else if the input dtg is UTC
 *              dtg, then convert the UTC dtg value to Local dtg value.
 *
 * Input:
 *    t_input_dtg   UTC or Local DTG calendar value.
 *
 * Output:
 *     t_output_dtg  UTC or Local DTG calendar value.
 *
 * Returns:
 *     DTR_OK:       Success.
 *     DTR_INV_ARG:  pt_output_dtg is NULL.
--------------------------------------------------------------------*/
INT32 c_dt_conv_utc_local(const DTG_T* pt_input_dtg,  DTG_T* pt_output_dtg)
{
    return x_dt_conv_utc_local(pt_input_dtg, pt_output_dtg);
}

/*---------------------------------------------------------------------
 *
 * Name: c_dt_dtg_to_sec
 *
 * Description: Compute the number of seconds for a specified DTG
 *              from the default epoch. If the input DTG is local,
 *              then the difference is computed between local time
 *              and 00:00:00Z UTC Jan 1, 1970.
 *
 * Input:
 *     t_dtg:  Input day-time-group value (local or UTC)
 *
 * Output:
 *     None.
 *
 * Return:
 *     Number of seconds since the default epoch.
----------------------------------------------------------------------*/
TIME_T c_dt_dtg_to_sec(const DTG_T* pt_dtg)
{
    return x_dt_dtg_to_sec(pt_dtg);
}

/*---------------------------------------------------------------------
 *
 * Name: c_dt_gps_sec_to_utc_sec
 *
 * Description: Convert GPS seconds to UTC seconds
 *
 *
 * Input:
 *     t_gps_sec:  Number of seconds since GPS epoch (0000Z, 06 Jan 1980)
 *
 * Output:
 *     None
 *
 * Return:
 *     The number of seconds since UTC epoch (0000Z, 01 Jan 1970).
 *
----------------------------------------------------------------------*/
TIME_T c_dt_gps_sec_to_utc_sec(TIME_T	t_gps_sec)
{
    return x_dt_gps_sec_to_utc_sec(t_gps_sec);
}

/*----------------------------------------------------------------------
 *
 * Name: c_dt_diff
 *
 * Description: Compute the difference in seconds between two DTG times
 *              delta seconds = t_dtg_to - t_dtg_from.
 *
 * Input:
 *     pt_dtg_from:   Pointer to a Day-Time-Group (UTC or Local) structure.
 *     pt_dtg_to:     Pointer to a Day-Time-Group (UTC or Local) structure.
 *
 * Output:
 *     None.
 *
 * Return:
 *     Number of seconds between two DTG values.
 *     NULL_TIME if 'pt_dtg_from' or 'pt_dtg_to' is NULL.
----------------------------------------------------------------------*/
TIME_T c_dt_diff(const DTG_T* pt_dtg_from, const DTG_T*  pt_dtg_to)
{
    return x_dt_diff(pt_dtg_from, pt_dtg_to);
}

/*------------------------------------------------------------------------
 *
 * Name: c_dt_add
 *
 * Description: Add or subtract seconds to a calendar time, and return
 *              the new calendar time.
 *
 * Input:
 *     pt_dtg_old: Pointer to Day-Time-Group (UTC or Local) structure
 *     t_sec:      Number of second to be add/subtract (+/-)
 *
 * Output:
 *     pt_dtg_new: New Day-Time-Group (UTC or Local)
 *
 * Returns:
 *     DTR_OK:       Success.
 *     DTR_INV_ARG:  'pt_dtg_old' or 'pt_dtg_new' is NULL pointer.
---------------------------------------------------------------------------*/
INT32 c_dt_add(const DTG_T* pt_dtg_old, TIME_T t_sec, DTG_T* pt_dtg_new)
{
    return x_dt_add(pt_dtg_old, t_sec, pt_dtg_new) ;
}

/*-------------------------------------------------------------------------
 *
 * Name:  c_dt_leap_yr
 *
 * Description: Given a year, determine if it is a leap year.
 *
 * Input:
 *     t_yr    Specify the year to be evaluated
 *
 * Output:
 *     None
 *
 * Return:
 *     TRUE:  leap-year.
 *     FALSE: not a leap-year.
-------------------------------------------------------------------------*/
BOOL c_dt_leap_yr(TIME_T t_yr)
{
    return x_dt_leap_yr(t_yr);
}

/*--------------------------------------------------------------------------
 * Name: c_dt_set_sync_src
 *
 * Description: This API sets the synchronization source for the current
 *              time.
 *
 * Input  -
 *    t_sync_src_type:  Specifies the types of synchronization source.
 *
 *    t_src_type_desc:  Specifies the source characteristic.
 *
 *    pv_src_info:      Pointer to a structure containing information
 *                      about the synchronization source.
 *
 * Return - DTR_OK:   synchronization source is set.
 *          DTR_FAIL: Can not set the source of the synchronization for
 *                    the current time.
 *
 --------------------------------------------------------------------------*/
INT32 c_dt_set_sync_src(DT_SYNC_SRC_TYPE_T t_sync_src_type,
                        DT_SRC_TYPE_DESC_T t_src_desc_type,
                        const VOID* pv_src_info)
{
    return x_dt_set_sync_src(t_sync_src_type, t_src_desc_type, pv_src_info);
}


/*--------------------------------------------------------------------------
 * Name: c_dt_reg_nfy_fct
 *
 * Description -   This API registers a notification function that will
 *                 be called by the DateTime library when the system clock
 *                 is synchronized with the current time value from the STT,
 *                 TDT, or TOT, or when the system clock switches to a free
 *                 running state.
 *
 *                 The notification function will be called in the section
 *                 manager callback context.
 *
 *                 Note:  Up to a total of 16 notification functions can be
 *                 registered.
 *
 * Input
 *   pf_nfy        Notification function provided by the caller.  This
 *                 notification function is called when the system clock
 *                 is synchronized with the current time value from STT,
 *                 or TOT, or TDT.
 *
 *   pv_tag        A tag value provided by the caller. This tag value will
 *                 be passed to the caller provided notification function.
 *
 * Output
 *   ph_dt	       Handle reference to the client's notification function.
 *
 * Return
 *   DTR_OK:          The notification function is registered.
 *
 *   DTR_INV_ARG      pf_nfy or ph_dt is NULL.
 *
 *   DTR_FAIL         Internal error: can not registered the notification
 *                    function.
 *
 *   DTR_AEE_OUT_OF_RESOURCES Can not allocate AEE resources.
 *   DTR_AEE_NO_RIGHTS        No right for the AEE resources.

 --------------------------------------------------------------------------*/
INT32 c_dt_reg_nfy_fct
(
    x_dt_nfy_fct   pf_nfy,
    VOID*          pv_tag,
    HANDLE_T*      ph_dt
)
{
    HANDLE_T  h_aux;
    INT32     i4_rc;

    i4_rc = aee_grab_handle_resource(AEE_FLAG,
                                     1,
                                     &h_aux);

    switch (i4_rc)
    {
        case AEER_OK:
        {
            i4_rc = x_dt_reg_nfy_fct(pf_nfy, pv_tag, ph_dt);

            if (i4_rc == DTR_OK)
            {
                handle_link_to_aux(h_aux, *ph_dt);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = DTR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = DTR_AEE_NO_RIGHTS;
        }
        break;
    }

    return i4_rc;
}


/*****************************************************************
 * Name: c_dt_mjd_bcd_to_dtg
 *
 * Description:
 *    This function converts the 40 bits string that consists of 16 LSBs MJD
 *    (Modified Julian Date) + 24 bits of BCD (binary code decimal) for hour,
 *    minutes, and seconds into a DTG_T structure.
 *
 *    The 40 bits MJD+BCD data are received from DVB TDT and TOT table.
 *
 * Inputs:
 *   pui1_data   A 40 bits code string consisting of 16 LSB's MJD value and
 *               24 bits of BCD (binary code decimal)
 *
 * Outputs: -
 *   pt_dtg:     pointer to a DTG_T structure.
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_INV_ARG pt_dtg is NULL.
 *****************************************************************/
INT32 c_dt_mjd_bcd_to_dtg(const UINT8* pui1_data,   DTG_T* pt_dtg)
{
    return x_dt_mjd_bcd_to_dtg(pui1_data, pt_dtg);
}

/*****************************************************************
 * Name: c_dt_mjd_to_dtg
 *
 * Description:  This function converts the MJD value to a calendar date.
 *
 * Inputs:
 *   ui4_mjd     Modified-Julia-Date value (for example: 45000)
 *
 * Outputs: -
 *   pt_dtg:     pointer to a DTG_T structure.
 *
 * Returns:
 *   DTR_OK      conversion is successful.  Only the year, month, and date
 *               fields in the DTG_T structure are meaningful.
 *   DTR_FAIL
 *****************************************************************/
INT32 c_dt_mjd_to_dtg(UINT32 ui4_mjd, DTG_T*  pt_dtg)
{
    return x_dt_mjd_to_dtg(ui4_mjd, pt_dtg);
}

/*****************************************************************
 * Name: c_dt_dtg_to_mjd
 *
 * Description:  This function converts a calendar value to the MJD date.
 *
 * Inputs:
 *   pt_dtg:     pointer to a DTG_T structure. only year/month/day count.
 *
 * Outputs: -
 *   pui2_mjd    Modified-Julia-Date value.
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
INT32 c_dt_dtg_to_mjd(const DTG_T*  pt_dtg, UINT16* pui2_mjd)
{
    return x_dt_dtg_to_mjd(pt_dtg, pui2_mjd);
}

/*****************************************************************
 * Name: c_dt_dtg_to_mjd_bcd
 *
 * Description:  This function converts a calendar value to the MJD date,
 *               BCD hour/minute and second.
 *               Note that the hour/minute 16-bit BCD code endian is 
 *               platform-specific. It may need endian conversion to
 *               reconstruct the 40-bit date time data.
 *
 * Inputs:
 *   pt_dtg:     pointer to a DTG_T structure. only year/month/day count.
 *
 * Outputs: -
 *   pui2_mjd    Modified-Julia-Date value.
 *   pui2_hr_min Hour/Minute in BCD format.
 *   pui1_sec    Second in BCD format.
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
INT32 c_dt_dtg_to_mjd_bcd(const DTG_T*  pt_dtg,
                          UINT16*       pui2_mjd,
                          UINT16*       pui2_hr_min,
                          UINT8*        pui1_sec)
{
    return x_dt_dtg_to_mjd_bcd(pt_dtg, pui2_mjd, pui2_hr_min, pui1_sec);
}


/*****************************************************************
 * Name: c_dt_bcd_to_sec
 *
 * Description:  This function converts the  24 bits of BCD (binary code
 *               decimal) for hour, minutes, and seconds into number of
 *               seconds.
 *
 * Inputs:
 *   pui1_data   A 24 bits BCD (binary code decimal) string for HHMMSS
 *
 * Outputs: -
 *   pt_sec:     Seconds from the BCD HHMMSS
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/

INT32 c_dt_bcd_to_sec(const UINT8* pui1_data,   TIME_T* pt_sec)
{
    return x_dt_bcd_to_sec(pui1_data, pt_sec);
}


/*****************************************************************
 * Name:  c_dt_get_brdcst_utc
 *
* Description: Get the current broadcast time in UTC time frame,
 *             the result is  returned as the number of seconds from
 *             a default epoch of 00:00:00Z, Jan 1, 1970.
 *
 * Inputs:
 *    None.
 *
 * Outputs:
 *    pui2_milli_sec   Optional parameter to store the number of
 *                     milliseconds (0 to 999). If the parameter is NULL,
 *                     milliseconds is ignored.
 *
 *    pt_dt_cond	   The condition of the system clock: DT_NOT_RUNNING,
 *                     DT_FREE_RUNNING, DT_SYNC_RUNNING.  If NULL, then
 *                     condition code is ignored.
 *
 *
 * Returns:
 *    The number of seconds since default epoch for the current
 *
 *****************************************************************/
TIME_T c_dt_get_brdcst_utc
(
    UINT16*       pui2_milli_sec,
    DT_COND_T*    pt_dt_cond
)
{
    return x_dt_get_brdcst_utc(pui2_milli_sec, pt_dt_cond);
}

/*****************************************************************
 * Name:  c_dt_sync_with_brdcst_utc
 *
 * Description: Synchronize the user set clock time with the broadcast
 *              time received.
 *
 * Inputs:
 *    b_sync_flag:    TRUE:  user clock and broadcast clock are sync'd
 *                    FALSE: user clock and broadcast clock are not sync'd.
 *
 * Outputs:
 *    None.
 *
 * Returns:
 *    The number of seconds since default epoch for the current
 *
 *****************************************************************/

INT32 c_dt_sync_with_brdcst_utc(BOOL b_sync_flag)
{
    return x_dt_sync_with_brdcst_utc(b_sync_flag);
    
}

/*----------------------------------------------------------------------
 *
 * Name: c_dt_check_input_time
 *
 * Description:
 *   enable or dis-able the check input function.
 *  
 * Inputs:
 *   b_enable:   TRUE: enable checking of input time.
 *               FALSE: dis-able checking of input time. (Every time
 *               value received are accepted.)
 *
 * Outputs: VOID
 *
 * Returns: 
 *   DTR_OK
------------------------------------------------------------------------*/
INT32 c_dt_check_input_time(BOOL b_enable)
{
    return x_dt_check_input_time(b_enable);
}

/*----------------------------------------------------------------------
 *
 * Name: c_dt_config_check_input_time
 *
 * Description:
 *   Configure the check time engine. 
 *  
 * Inputs:
 *   e_set_parm:   Specifies the parameter to set in the next argument:
 *
 *                 DT_USE_DEFAULT_CONFIG:  revert to default parameters.
 *
 *                 DT_SET_CONSECUTIVE_VAL_TO_CHK: specifies number of
 *                 of consecutive good input value to be processed before
 *                 accepting the next good input time value.
 *               
 *                 DT_SET_TIME_WINDOW_ADJUSTMENT: specifies the delta value
 *                 (tolerance) between the time difference between the current
 *                 and previous one received for the input time, and the 
 *                 time difference between the current and last reading of
 *                 system time values.
 *
 *   pv_value:     Pointer to the variable containg value to set.
 *                 For DT_SET_CONSECUTIVE_VAL_TO_CHK and
 *                 DT_SET_TIME_WINDOW_ADJUSTMENT, this pointer
 *                 refer to a 'INT32' variable containing the set values.
 *
 *                 For DT_USE_DEFAULT_CONFIG, this pointer is ignored.
 *
 * Outputs: VOID
 *
 * Returns: 
 *   DTR_OK
------------------------------------------------------------------------*/
INT32 c_dt_config_check_input_time
(
    DT_CHECK_TIME_CONFIG  e_set_parm,
    VOID*                 pv_value
)
{
    return x_dt_config_check_input_time(e_set_parm, pv_value);
}

/*****************************************************************
 * Name: c_dt_set_config
 *
 * Description:  This function set the configuration for DT library
 *   behavior.
 *
 * Inputs:
 *   ui4_flag:   Masked flag.
 *
 * Outputs: -
 *   VOID
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
INT32   c_dt_set_config(UINT32  ui4_flag)
{
    return  x_dt_set_config(ui4_flag);
}

/*---------------------------------------------------------------------
 *
 * Name: c_dt_get_last_sync_tbl_id
 *
 * Description: Get the last effective table ID used for time sync'ing.
 *              For ATSC, result may be STT (in/out-band); for DVB, 
 *              result may be TDT or TOT; for analog, result may be VBI
 *              teletext. If DT never sync'ed, return zero.
 * Input:
 *     None
 *
 * Output:
 *     None
 *
 * Return:
 *     DT_STT_IN_BAND_TBL_ID        -   STT (in-band)
 *     DT_STT_OUT_OF_BAND_TBL_ID    -   STT (out-band)
 *     DT_DVB_TDT_TBL_ID            -   TDT
 *     DT_DVB_TOT_TBL_ID            -   TOT
 *     DT_VBI_TELETEXT_TBL_ID       -   VBI Teletext
 *     
----------------------------------------------------------------------*/

UINT8 c_dt_get_last_sync_tbl_id(VOID)
{
    return x_dt_get_last_sync_tbl_id();
}

