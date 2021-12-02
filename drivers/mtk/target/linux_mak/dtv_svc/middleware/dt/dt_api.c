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
 * $RCSfile: dt_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/18 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: eeeee16d218b478d12d6bd39898cde8d $
 *
 * Description:
 *         This file contain implementation of Date Time API's,
 *         which are exported to other Middleware components.
 *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "dt/x_dt.h"
#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "dt/_dt.h"

/*
  DateTime library static variables.
*/

static TIME_T     t_timezone_val = 0 ;

/*
  Flag indicating if the Timezone parameter received from the broadcast
  stream table should be used to determine the current locality.
*/
static BOOL           B_TZ_CONTROL = TRUE;


/* number of seconds accumulated up to each month (non-leap year) */
static
TIME_T sec_acu[] = { 0,        0,        2678400,   5097600,  7776000,
                     10368000, 13046400, 15638400, 18316800, 20995200,
                     23587200, 26265600, 28857600, 31536000  };



/* number of seconds accumulated up to each month (leap year) */
static
TIME_T sec_acu_leap[] = { 0,        0,         2678400,  5184000,  7862400,
                          10454400, 13132800, 15724800, 18403200, 21081600,
                          23673600, 26352000, 28944000, 31622400  };


static const DT_BRDCST_DVB_API_T*    pt_dvb_brdcst_api = NULL;

static const DT_ANALOG_BRDCST_VBI_TELETEXT_API_T* pt_vbi_teletext_brdcst_api = NULL;


/*----------------------------------------------------------------------
 *
 * Name: x_dt_set_tz
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
VOID x_dt_set_tz(TIME_T t_tz_offset)
{
    CRIT_STATE_T   t_state;

    t_state=x_crit_start();
    t_timezone_val = t_tz_offset;
    x_crit_end(t_state);
    
#ifdef LINUX_TURNKEY_SOLUTION
    dt_set_linux_tz();
#endif

    return;
}

/*----------------------------------------------------------------------
 *
 * Name: x_dt_get_tz
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
TIME_T x_dt_get_tz(VOID)
{
    CRIT_STATE_T   t_state;
    TIME_T         t_tz;

    t_state=x_crit_start();
    t_tz=t_timezone_val;
    x_crit_end(t_state);
    return t_tz;
}

/*--------------------------------------------------------------------
 *
 * Name:  x_dt_set_tz_ctrl
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
VOID x_dt_set_tz_ctrl(BOOL b_tz)
{
    CRIT_STATE_T   t_state;

    t_state=x_crit_start();
    B_TZ_CONTROL = b_tz;
    x_crit_end(t_state);    
    return;
}

/*-----------------------------------------------------------------------
 *
 * Name: x_dt_get_tz_ctrl
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
BOOL x_dt_get_tz_ctrl(VOID)
{
    CRIT_STATE_T   t_state;
    BOOL           b_tz_ctrl_setting;
    
    t_state=x_crit_start();
    b_tz_ctrl_setting = B_TZ_CONTROL ;
    x_crit_end(t_state);
    return b_tz_ctrl_setting;
}

/*----------------------------------------------------------------------
 *
 * Name: x_dt_set_sys_count_code
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
INT32 x_dt_set_sys_count_code (ISO_3166_COUNT_T  t_count_code,
                              UINT16            ui2_region_id)
{
    INT32      i4_rc = DTR_OK;
    
    if ( pt_dvb_brdcst_api != NULL )
    {
        pt_dvb_brdcst_api->pf_dvb_set_cntry_code (
            t_count_code, ui2_region_id);
    }
    else
    {
        i4_rc = DTR_FAIL;
    }
    return i4_rc ;
}


/*----------------------------------------------------------------------
 *
 * Name: x_dt_get_sys_count_code
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
INT32 x_dt_get_sys_count_code (ISO_3166_COUNT_T*  pt_count_code,
                               UINT16*            pui2_region_id)
{
    INT32     i4_rc = DTR_FAIL ;
    
    if ( pt_dvb_brdcst_api != NULL )
    {
        i4_rc = pt_dvb_brdcst_api->pf_dvb_get_cntry_code (
            pt_count_code, pui2_region_id);
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 *
 * Name: x_dt_get_count_code
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
INT32 x_dt_get_count_code (UINT16             ui2_idx,
                           ISO_3166_COUNT_T*  pt_count_code,
                           UINT16*            pui2_region_id,
                           TIME_T*            pt_tz_offset)
{
    INT32     i4_rc = DTR_FAIL ;
    
    if ( pt_dvb_brdcst_api != NULL )
    {
        i4_rc = pt_dvb_brdcst_api->pf_dvb_get_lto (
            ui2_idx,pt_count_code,pui2_region_id,pt_tz_offset);
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 *
 * Name: x_dt_get_num_count_code
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
UINT16 x_dt_get_num_count_code (VOID)
{
    UINT16   ui2_nb = 0;
    
    if ( pt_dvb_brdcst_api != NULL )
    {
        ui2_nb = pt_dvb_brdcst_api->pf_dvb_get_nb_lto_entry();
    }
    return ui2_nb;
}

/*-----------------------------------------------------------------------
 *
 * Name: x_dt_get_brdcst_utc
 *
 * Description: Get the current broadcast clock time in UTC time frame,
 *              the result is  returned as the number of seconds from
 *              a default epoch of 00:00:00Z, Jan 1, 1970.
 *
 *              If the broadcast time has not bee set, then NULL_TIME is
 *              returned.
 *
 * Inputs:
 *    None.
 *
 * Outputs:
 *    pui2_milli_sec	Optional parameter to store the number of
 *                      milliseconds (0 to 999). If the parameter is NULL,
 *                      milliseconds is ignored.
 *
 *    pt_dt_cond	    The condition of the system clock: DT_NOT_RUNNING,
 *                      DT_FREE_RUNNING, DT_SYNC_RUNNING.  If NULL, then
 *                      condition code is ignored.
 *
 * Returns:
 *    The number of seconds since default epoch for the current
 *    UTC time.
-------------------------------------------------------------------------*/
TIME_T x_dt_get_brdcst_utc(UINT16*  pui2_milli_sec, DT_COND_T* pt_dt_cond)
{
    TIME_T    t_utc;
    /*
      Get the system time from broadcast clock.
    */
    t_utc = dt_get_utc_and_cond((INT32)DT_CLOCK_BRDCST_UTC, pui2_milli_sec,
                                pt_dt_cond);
    return t_utc ;
}

/*-----------------------------------------------------------------------
 *
 * Name: x_dt_get_utc
 *
 * Description: Get the current system clock time in UTC time frame,
 *              the result is  returned as the number of seconds from
 *              a default epoch of 00:00:00Z, Jan 1, 1970.
 *
 *              If the system time has not bee set, then NULL_TIME is
 *              returned.
 *
 * Inputs:
 *    None.
 *
 * Outputs:
 *    pui2_milli_sec	Optional parameter to store the number of
 *                      milliseconds (0 to 999). If the parameter is NULL,
 *                      milliseconds is ignored.
 *
 *    pt_dt_cond	    The condition of the system clock: DT_NOT_RUNNING,
 *                      DT_FREE_RUNNING, DT_SYNC_RUNNING.  If NULL, then
 *                      condition code is ignored.
 *
 * Returns:
 *    The number of seconds since default epoch for the current
 *    UTC time.
-------------------------------------------------------------------------*/
TIME_T x_dt_get_utc(UINT16*  pui2_milli_sec, DT_COND_T* pt_dt_cond)
{
    TIME_T    t_utc;
    /*
      Get the system time from RTC
    */
    if ( dt_rtc_get_time(&t_utc) == DTR_OK )
    {
        /*
          RTC does not provide milli-seconds, so we get the milliseconds
          from the software clock, as well as the System time Condition.
        */
        dt_get_utc_and_cond((INT32)DT_CLOCK_USER_UTC, pui2_milli_sec, pt_dt_cond);
    }
    else
    {
        t_utc = dt_get_utc_and_cond((INT32)DT_CLOCK_USER_UTC, pui2_milli_sec,
                                    pt_dt_cond);
    }
    return t_utc ;
}

/*------------------------------------------------------------------------
 *
 * Name: x_dt_set_utc
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
INT32 x_dt_set_utc(TIME_T t_sec, UINT16 ui2_milli_sec)
{
    DT_COND_T   t_dt_cond;
    /*
      Call the user time clock to set the current time.
    */
    os_set_sys_td(t_sec, ui2_milli_sec, DT_CLOCK_USER_UTC);
    dt_rtc_set_time(t_sec);
    dt_set_cond ((INT32)DT_CLOCK_USER_UTC, DT_IS_RUNNING);
    if (dt_get_cond((INT32)DT_CLOCK_BRDCST_UTC, &t_dt_cond) == DTR_OK &&
        t_dt_cond == DT_NOT_RUNNING)
    {
        os_set_sys_td(t_sec, ui2_milli_sec, DT_CLOCK_BRDCST_UTC);
        dt_set_cond ((INT32)DT_CLOCK_BRDCST_UTC, DT_IS_RUNNING);
    }
    return DTR_OK;
}

/*------------------------------------------------------------------------
 *
 * Name: dt_set_brdcst_utc
 *
 * Description: Set the broadcast time system clock with current UTC time.
 *              This function is not exported.
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
INT32 dt_set_brdcst_utc(TIME_T t_sec, UINT16 ui2_milli_sec)
{
    /*
      Call the user time clock to set the current time.
    */
    os_set_sys_td(t_sec, ui2_milli_sec, DT_CLOCK_BRDCST_UTC);
    dt_rtc_set_time(t_sec);
    dt_set_cond ((INT32)DT_CLOCK_BRDCST_UTC, DT_IS_RUNNING);

    if ( dt_user_clock_sync_with_brdcst_clock() == TRUE )
    {
        /* sync the user time clock with the brdcst time. */
        x_dt_set_utc(t_sec, ui2_milli_sec);
    }
    
    return DTR_OK;
}

#if 0
/*--------------------------------------------------------------------
 *
 * Name: x_dt_get_loc
 *
 * Description: Get the current system clock time in Local time frame,
 *              the result is returned as the number of seconds from a
 *              default epoch (take into account of both local timezone
 *              and Daylight-Saving-Time).
 *
 *              The time_t returned by x_dt_get_loc() is related to
 *              x_dt_get_utc() by the following:
 *
 *                  x_dt_get_loc() = x_dt_get_utc() +
 *                              (timezone_value + daylightsaving_time)
 *
 * Inputs: None
 *
 * Outputs:
 *    pui2_milli_sec   Optional parameter to store the number of
 *                     milliseconds (0 to 999). If this parameter is NULL,
 *                     milliseconds is ignored.
 *
 *    pt_dt_cond	   The condition of the system clock: DT_NOT_RUNNING,
 *                     DT_FREE_RUNNING, DT_SYNC_RUNNING.  If NULL, then
 *                     condition code is ignored.
 * Return:
 *     The number of seconds since default epoch in the current local
 *     time.
 *
-----------------------------------------------------------------------*/
TIME_T x_dt_get_loc(UINT16* pui2_milli_sec, DT_COND_T*  pt_dt_cond)
{
    TIME_T t_utc;

    /*
      Get the current system time (in UTC) and milliseconds.
    */
    t_utc = x_dt_get_utc(pui2_milli_sec, pt_dt_cond);

    /* Adjust the UTC seconds for timezone and Day-Light-Saving */

    return (x_dt_utc_sec_to_local_sec(t_utc));
}
#endif

/*-----------------------------------------------------------------------
 *
 * Name: x_dt_get_gps
 *
 * Description: Get the current user clock time as the number of
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
TIME_T x_dt_get_gps(INT32* pi4_gps_leap_sec, UINT16* pui2_milli_sec)
{
    TIME_T t_sec;
    INT32  i4_offset;

    /*
      Get the latest GPS_UTC offset value.
    */
    i4_offset = dt_get_gps_utc_offset();

    if ( pi4_gps_leap_sec != NULL )
    {
        *pi4_gps_leap_sec = i4_offset;
    }
    /*
      Call the OS system time driver to get the current system time
      and milliseconds.
    */
    t_sec = x_dt_get_utc(pui2_milli_sec, NULL);

    /*
       Convert the UTC seconds to GPS seconds
    */
    return (t_sec - DT_DIFF_GPS_EPOCH_UNIX_EPOCH + i4_offset);
}

/*--------------------------------------------------------------
  Conversion from the number of GPS elapse seconds since 00:00:00Z, Janunary
  6, 1980 to a calendar data represented by YYYYMMDDHHMMSS.

  Input:
      t_utc_sec:  number of elapse seconds from default epoch in UTC
                  time frame.
  Output:
      pt_utc_dtg: structure containing Day-Time-Group values.
-----------------------------------------------------------------*/
INT32 x_dt_gps_sec_to_dtg(TIME_T t_gps_sec, DTG_T* pt_dtg)
{
    if ( pt_dtg != NULL )
    {
        /*
          Convert GPS seconds to UTC seconds and
          calls the UTC sec to DTG conversion function.
        */
        return x_dt_utc_sec_to_dtg(x_dt_gps_sec_to_utc_sec(t_gps_sec),
                                   pt_dtg);

    }
    return DTR_INV_ARG;
}

/*----------------------------------------------------------------------
 *
 * Name: x_dt_utc_sec_to_dtg
 *
 * Description: Conversion from the number of UTC elapse seconds since
 *              default epoch to UTC YYYYMMDDHHMMSS.
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
INT32 x_dt_utc_sec_to_dtg(TIME_T t_utc_sec, DTG_T* pt_dtg)
{
    TIME_T   t_sec_in_yr;
    INT32    i4_i;

    /*
      In our UTC time scale, we don't allow date earlier than
      Jan 01, 1970
    */
    if ( (pt_dtg != NULL) && (t_utc_sec >= 0) && (t_utc_sec <= 15768000000) )
    {
        pt_dtg->ui2_yr    = GMT_EPOCH_YEAR;
        pt_dtg->ui1_mo    = 0;
        pt_dtg->ui1_day   = 0;
        pt_dtg->ui1_hr    = 0;
        pt_dtg->ui1_min   = 0;
        pt_dtg->ui1_sec   = 0;
        pt_dtg->b_gmt     = TRUE;
#if 0        
        pt_dtg->b_dst     = FALSE;  /* This is UTC, so we don't care
                                       about the DST setting. */
#endif        

        pt_dtg->b_dst = dt_get_dst_stat_for_utc_sec(t_utc_sec);

        /*
          compute Day of Week
        */
        pt_dtg->ui1_dow  = (UINT8)dt_dow(t_utc_sec);
        /*
          Process year
        */
        while (1)
        {
            if ( x_dt_leap_yr((TIME_T)(pt_dtg->ui2_yr)) == FALSE )
            {
                t_sec_in_yr =  SEC_IN_YR ;
            }
            else
            {
                t_sec_in_yr = SEC_IN_LEAP_YR ;
            }

            if ( t_utc_sec >= t_sec_in_yr  )
            {
                t_utc_sec = t_utc_sec - t_sec_in_yr;
                pt_dtg->ui2_yr++;
            }
            else
            {
                break;
            }
        }

        /* process month. */
        if ( x_dt_leap_yr((TIME_T)(pt_dtg->ui2_yr)) == FALSE )
        {
            for ( i4_i = 1 ; i4_i < 13 ; i4_i++ )
            {
                if ( t_utc_sec >= sec_acu[i4_i]    &&
                     t_utc_sec < sec_acu[i4_i+1]
                    )
                {
                    pt_dtg->ui1_mo = (UINT8) i4_i ;
                    t_utc_sec = t_utc_sec - sec_acu[i4_i] ;
                    break;
                }
            }
        }
        else
        {
            for ( i4_i = 1 ; i4_i < 13 ; i4_i++ )
            {
                if ( t_utc_sec >= sec_acu_leap[i4_i]   &&
                     t_utc_sec < sec_acu_leap[i4_i+1]
                    )
                {
                    pt_dtg->ui1_mo = (UINT8)i4_i ;
                    t_utc_sec = t_utc_sec - sec_acu_leap[i4_i] ;
                    break;
                }
            }
        }

        /* process date, hr, min, sec. */
        if ( t_utc_sec >= 0 )
        {
            pt_dtg->ui1_day = (UINT8)(t_utc_sec/SEC_IN_DAY) ;
            t_utc_sec = t_utc_sec%SEC_IN_DAY;

            /*
              If no remainding seconds, we need to increment the
              day by 1.
            */
            if ( t_utc_sec == 0 )
            {
                pt_dtg->ui1_day++ ;
            }

            if ( t_utc_sec > 0 )
            {
                pt_dtg->ui1_day++;
                pt_dtg->ui1_hr = (UINT8)(t_utc_sec/SEC_IN_HR);
                t_utc_sec = t_utc_sec%SEC_IN_HR;

                if ( t_utc_sec > 0 )
                {
                    pt_dtg->ui1_min = (UINT8)(t_utc_sec/SEC_IN_MIN);
                    pt_dtg->ui1_sec = (UINT8)(t_utc_sec%SEC_IN_MIN);
                }
            }
        }
        return DTR_OK;
    }
    return DTR_INV_ARG;
}

/*----------------------------------------------------------------------
 *
 * Name: x_dt_utc_sec_to_loc_dtg
 *
 * Description: Conversion from the number of UTC elapse seconds since
 *              default epoch to YYYYMMDDHHMMSS in local time.
 *
 * Input:
 *     t_utc_sec:  number of elapse seconds from default epoch in UTC
 *                 time frame.
 *
 * Output:
 *     pt_dtg:     Structure containing Day-Time-Group values in Local
 *                 time scale.
 *
 * Returns:
 *     DTR_OK:       Success.
 *     DTR_INV_ARG:  pt_utc dtg is NULL or input seconds is less than 0
------------------------------------------------------------------------*/
INT32 x_dt_utc_sec_to_loc_dtg(TIME_T t_utc_sec, DTG_T* pt_dtg)
{
    TIME_T   t_tz_val;
    TIME_T   t_dst_offset;
    TIME_T   t_offset;
    BOOL     b_dst_setting;
    
    t_tz_val = x_dt_get_tz();
    t_dst_offset = dt_get_dst_offset();
    
    /*
      In our UTC time scale, we don't allow date earlier than
      Jan 01, 1970
    */
    if ( pt_dtg != NULL && t_utc_sec >= 0 )
    {
        /*
          Determine if the conversion should take into accout of
          the local DST setting.
        */
        b_dst_setting = dt_get_dst_stat_for_utc_sec(t_utc_sec);

        if ( b_dst_setting == TRUE )
        {
            t_offset = t_tz_val + t_dst_offset ;
        }
        else
        {
            t_offset = t_tz_val;
        }

        /*
          Convert UTC second to local seconds
        */
        t_utc_sec = t_utc_sec + t_offset ;
        
        /*
          Call the seconds to DTG structure conversion function.
        */
        if ( x_dt_utc_sec_to_dtg(t_utc_sec, pt_dtg) == DTR_OK )
        {
            /*
              The result is in local time scale, so set the GMT
              and DST flag accordingly.
            */
            pt_dtg->b_gmt     = FALSE;
            pt_dtg->b_dst     = b_dst_setting;  
            
            return DTR_OK;
        }
    }
    return DTR_INV_ARG;
}

#if 0
/*--------------------------------------------------------------------
 *
 * Name:  x_dt_loc_sec_to_dtg
 *
 * Description: Conversion from the number of local elapse seconds
 *              since default epoch to Local YYYYMMDDHHMMSS.
 *
 * Inputs:
 *    t_loc_sec:  number of elapse seconds from default epoch in local
 *                time frame, adjusted for local time zone.
 * Outputs:
 *     pt_loc_dtg: structure containing Day-Time-Group values.
 *
 * Returns:
 *     DTR_OK:       Success.
 *     DTR_INV_ARG:  pt_loc_dtg is NULL.
-----------------------------------------------------------------------*/
INT32 x_dt_loc_sec_to_dtg(TIME_T t_loc_sec, DTG_T* pt_loc_dtg)
{
    INT32   i4_rc;

    if ( pt_loc_dtg != NULL )
    {
        /*
          Since this is local seconds, it implies the adjustment
          for TZ and DST has already been applied, so we just
          call the x_dt_utc_sec_to_dtg() function to convert
          seconds to DTG value.  After conversion, set the
          b_gmt flag to FALSE.
        */
        i4_rc=x_dt_utc_sec_to_dtg(t_loc_sec, pt_loc_dtg);
        pt_loc_dtg->b_gmt=FALSE;
        return i4_rc;
    }
    return DTR_INV_ARG;
}
#endif

/*------------------------------------------------------------------
 *
 * Name: x_dt_conv_utc_local
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
INT32 x_dt_conv_utc_local(const DTG_T* pt_input_dtg,  DTG_T* pt_output_dtg)
{
    DELTA_DTG_T    t_delta;
    TIME_T         t_offset;
    TIME_T         t_tz_val;
    TIME_T         t_dst_offset ;
    TIME_T         t_utc_sec;
    BOOL           b_dst_setting;

    t_tz_val = x_dt_get_tz();

    t_dst_offset = dt_get_dst_offset();
    

    if ( pt_input_dtg != NULL && pt_output_dtg != NULL )
    {

        /*
          convert the input DTG to number of UTC sec, this is needed to
          determine the DST status.
        */
        if ( pt_input_dtg->b_gmt != TRUE )
        {
            /* UTC = LOCAL - TZ */
            t_utc_sec = x_dt_dtg_to_sec(pt_input_dtg) - t_tz_val;
        }
        else
        {
            t_utc_sec = x_dt_dtg_to_sec(pt_input_dtg);
        }

        /*
          Determine if the conversion should take into accout of
          the local DST setting.
        */
        b_dst_setting = dt_get_dst_stat_for_utc_sec(t_utc_sec);

        if ( b_dst_setting == TRUE )
        {
            t_offset = t_tz_val + t_dst_offset ;
        }
        else
        {
            t_offset = t_tz_val;
        }

        /* convert the time offset to day,hr,min,sec */
        dt_sec_to_delta_dtg(t_offset, &t_delta);

        /* UTC time to local time,  use
           equation:

           Local_DTG = UTC_DTG + (Timezone + DST )

		  */
        if ( pt_input_dtg->b_gmt == TRUE )
        {
            if ( t_offset > 0 )
            {
                dt_add(pt_input_dtg,
                       (UINT32)t_delta.t_day,
                       (UINT32)t_delta.t_hr,
                       (UINT32)t_delta.t_min,
                       (UINT32)t_delta.t_sec,
                       pt_output_dtg);
            }
            else
            {
                dt_subtract(pt_input_dtg,
                            (UINT32)t_delta.t_day,
                            (UINT32)t_delta.t_hr,
                            (UINT32)t_delta.t_min,
                            (UINT32)t_delta.t_sec,
                            pt_output_dtg);
            }
            pt_output_dtg->b_gmt=FALSE;
        }
        else
        {
            /*
               Local Time to UTC time, use equation:

               UTC_dtg = Local_DTG - (Timezone + DST)
			*/
            if ( t_offset > 0 )
            {
                dt_subtract(pt_input_dtg,
                    (UINT32)t_delta.t_day,
                    (UINT32)t_delta.t_hr,
                    (UINT32)t_delta.t_min,
                    (UINT32)t_delta.t_sec,
                    pt_output_dtg);
            }
            else
            {
                dt_add(pt_input_dtg,
                       (UINT32)t_delta.t_day,
                       (UINT32)t_delta.t_hr,
                       (UINT32)t_delta.t_min,
                       (UINT32)t_delta.t_sec,
                       pt_output_dtg);
            }
            pt_output_dtg->b_gmt=TRUE;
        }

        /*
          Compute the day of the week for the new calerdar value.
        */
        pt_output_dtg->ui1_dow=(UINT8)dt_dtg_to_dow(pt_output_dtg);

        /*
          If the output DTG is local time, then save the
          DST setting used to compute the output value.
        */
        if ( pt_output_dtg->b_gmt == FALSE )
        {
            pt_output_dtg->b_dst = b_dst_setting ;
        }
        return DTR_OK;
    }
    return DTR_INV_ARG;
}

/*---------------------------------------------------------------------
 *
 * Name: x_dt_dtg_to_sec
 *
 * Description: Compute the number of seconds for a specified DTG
 *              from the default epoch.  Note: no adjustment for TZ
 *              or DST is made in this API.
 *
 *              Therefore, If the input DTG is local,
 *              then the difference is computed between local time
 *              and 00:00:00 local Jan 1, 1970.  If the input DTG
 *              is UTC, then the difference is computed between UTC time
 *              and 00:00:00 UTC  Jan 1, 1970.
 *              
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
TIME_T x_dt_dtg_to_sec(const DTG_T *pt_dtg)
{
    TIME_T  t_sec=0;
    TIME_T  t_yr1=GMT_EPOCH_YEAR;

    /*
      check if the date is before Janunary 01, 1970
    */
    if ( pt_dtg != NULL && pt_dtg->ui2_yr >= 1970 )
    {
        while ( t_yr1 < ((TIME_T)(pt_dtg->ui2_yr)) )
        {
            if ( x_dt_leap_yr(t_yr1) == TRUE )
            {
                t_sec += SEC_IN_LEAP_YR;
            }
            else
            {
                t_sec += SEC_IN_YR;
            }
            t_yr1++;
        }

        if ( x_dt_leap_yr((TIME_T)(pt_dtg->ui2_yr)) == TRUE )
        {
            t_sec = t_sec + sec_acu_leap[pt_dtg->ui1_mo]  +
                ((pt_dtg->ui1_day-1) * SEC_IN_DAY) +
                (pt_dtg->ui1_hr  * SEC_IN_HR)  +
                (pt_dtg->ui1_min * SEC_IN_MIN) +
                (pt_dtg->ui1_sec) ;
        }
        else
        {
            t_sec = t_sec + sec_acu[pt_dtg->ui1_mo] +
                ((pt_dtg->ui1_day-1) * SEC_IN_DAY) +
                (pt_dtg->ui1_hr  * SEC_IN_HR)  +
                (pt_dtg->ui1_min * SEC_IN_MIN) +
                (pt_dtg->ui1_sec) ;
        }
        return t_sec;
    }
    return NULL_TIME;
}

/*---------------------------------------------------------------------
 *
 * Name: x_dt_gps_sec_to_utc_sec
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
TIME_T x_dt_gps_sec_to_utc_sec
(
    TIME_T	t_gps_sec
)
{
    return (  (t_gps_sec - dt_get_gps_utc_offset())
              + DT_DIFF_GPS_EPOCH_UNIX_EPOCH) ;
}

/*----------------------------------------------------------------------
 *
 * Name: x_dt_diff
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
TIME_T x_dt_diff(const DTG_T* pt_dtg_from, const DTG_T*  pt_dtg_to)
{
    TIME_T   t_sec1=0;
    TIME_T   t_sec2=0;
    TIME_T   t_yr1;
    TIME_T   t_yr2;
    TIME_T   t_mo1;
    TIME_T   t_mo2;

    const DTG_T*   pt_dtg1;
    const DTG_T*   pt_dtg2;
    BOOL           b_swapped;
    TIME_T         t_tz_val;
    TIME_T         t_dst_offset ;

    t_dst_offset = dt_get_dst_offset();

    t_tz_val = x_dt_get_tz();

    if ( pt_dtg_from != NULL && pt_dtg_to != NULL )
    {
        /*
          The algorithm accumulates seconds from earlir date to later date,
          therefore we need to determine which two dates to use as
          the start point.
        */
        if (pt_dtg_from->ui2_yr < pt_dtg_to->ui2_yr )
        {
            pt_dtg1=pt_dtg_from;
            pt_dtg2=pt_dtg_to;
            b_swapped = FALSE;
        }
        else
        {
            pt_dtg1=pt_dtg_to;
            pt_dtg2=pt_dtg_from;
            b_swapped = TRUE;
        }
        t_yr1 = (TIME_T)(pt_dtg1->ui2_yr);
        t_yr2 = (TIME_T)(pt_dtg2->ui2_yr);

        while ( t_yr1 < t_yr2 )
        {
            if ( x_dt_leap_yr(t_yr1) == TRUE )
            {
                t_sec2 += SEC_IN_LEAP_YR;
            }
            else
            {
                t_sec2 += SEC_IN_YR;
            }
            t_yr1++;
        }

        t_mo1 = pt_dtg1->ui1_mo;
        t_mo2 = pt_dtg2->ui1_mo;

        if ( x_dt_leap_yr((TIME_T)(pt_dtg1->ui2_yr)) == TRUE )
        {
            t_sec1 = t_sec1 + sec_acu_leap[t_mo1]  +
                ((pt_dtg1->ui1_day-1) * SEC_IN_DAY) +
                (pt_dtg1->ui1_hr  * SEC_IN_HR)  +
                (pt_dtg1->ui1_min * SEC_IN_MIN) +
                (pt_dtg1->ui1_sec) ;
        }
        else
        {
            t_sec1 = t_sec1 + sec_acu[t_mo1] +
                ((pt_dtg1->ui1_day-1) * SEC_IN_DAY) +
                (pt_dtg1->ui1_hr  * SEC_IN_HR)  +
                (pt_dtg1->ui1_min * SEC_IN_MIN) +
                (pt_dtg1->ui1_sec) ;
        }

        if ( x_dt_leap_yr((TIME_T)(pt_dtg2->ui2_yr)) == TRUE )
        {
            t_sec2 = t_sec2 + sec_acu_leap[t_mo2]  +
                ((pt_dtg2->ui1_day-1) * SEC_IN_DAY) +
                (pt_dtg2->ui1_hr  * SEC_IN_HR)  +
                (pt_dtg2->ui1_min * SEC_IN_MIN) +
                (pt_dtg2->ui1_sec) ;
        }
        else
        {
            t_sec2 = t_sec2 + sec_acu[t_mo2]  +
                ((pt_dtg2->ui1_day-1) * SEC_IN_DAY) +
                (pt_dtg2->ui1_hr  * SEC_IN_HR)  +
                (pt_dtg2->ui1_min * SEC_IN_MIN) +
                (pt_dtg2->ui1_sec) ;
        }

        /*
           If one of input DTG is local time, then
           we need to convert it to UTC time scale before
           computing the difference
        */
        if ( pt_dtg1->b_gmt != pt_dtg2->b_gmt )
        {
            if ( pt_dtg1->b_gmt == FALSE )
            {
                t_sec1 = t_sec1 - t_tz_val ;
                if ( pt_dtg1->b_dst == TRUE )
                {
                    t_sec1 = t_sec1 - t_dst_offset;
                }
            }
            if ( pt_dtg2->b_gmt == FALSE )
            {
                t_sec2 = t_sec2 - t_tz_val ;
                if ( pt_dtg2->b_dst  == TRUE )
                {
                    t_sec2 = t_sec2 - t_dst_offset;
                }
            }
        }
        else
        {
            /*
              If both DTG structure is local time, then
              we need to check if there is a DST transition
              between the two times.
            */
            if ( pt_dtg1->b_gmt == FALSE &&
                 pt_dtg2->b_gmt == FALSE )
            {
                if ( pt_dtg1->b_dst == TRUE )
                {
                    t_sec1 = t_sec1 - t_dst_offset;
                }
                if ( pt_dtg2->b_dst  == TRUE )
                {
                    t_sec2 = t_sec2 - t_dst_offset;
                }
            }
        }
        

        if ( b_swapped == TRUE )
        {
            return (t_sec1 - t_sec2);
        }
        else
        {
            return (t_sec2 - t_sec1);
        }
    }
    return NULL_TIME;
}

/*------------------------------------------------------------------------
 *
 * Name: x_dt_add
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
#if 0
INT32 x_dt_add(const DTG_T* pt_dtg_old, TIME_T t_sec, DTG_T* pt_dtg_new)
{
    DELTA_DTG_T    t_delta;

    if ( pt_dtg_old != NULL && pt_dtg_new != NULL )
    {
        dt_sec_to_delta_dtg(t_sec, &t_delta);

        if ( t_sec > 0 )
        {
            dt_add(pt_dtg_old,
                   t_delta.t_day,
                   t_delta.t_hr,
                   t_delta.t_min,
                   t_delta.t_sec,
                   pt_dtg_new);
        }
        else
        {
            dt_subtract(pt_dtg_old,
                        t_delta.t_day,
                        t_delta.t_hr,
                        t_delta.t_min,
                        t_delta.t_sec,
                        pt_dtg_new);
        }
        /*
          Update the Day of week value.
        */
        pt_dtg_new->ui1_dow=dt_dtg_to_dow(pt_dtg_new);
        return DTR_OK;
    }
    return DTR_INV_ARG;
}
#endif

INT32 x_dt_add(const DTG_T* pt_dtg_old, TIME_T t_sec, DTG_T* pt_dtg_new)
{
    TIME_T         t_old_sec;
    TIME_T         t_tz_offset;
    TIME_T         t_dst_offset;
    BOOL           b_dst;

    t_tz_offset = x_dt_get_tz();
    t_dst_offset = dt_get_dst_offset();
    
    if ( pt_dtg_old != NULL && pt_dtg_new != NULL )
    {
        /*
          Convert inut DTG to UTC seconds.
        */
        t_old_sec = x_dt_dtg_to_sec(pt_dtg_old);

        if ( pt_dtg_old->b_gmt != TRUE )
        {
            /*
              adjust for Timezone.

              utc = local - TZ - DST
            */
            t_old_sec = t_old_sec - t_tz_offset ;

            if ( pt_dtg_old->b_dst == TRUE )
            {
                /*
                  Adjust for Daylight-Saving-Time.
                */
                t_old_sec = t_old_sec - t_dst_offset;
            }
        }

        t_old_sec = t_old_sec + t_sec ;

        /*
          If the output DTG structure for the new time is to be in the
          local time, then convert the new UTC time back to local.
        */
        if ( pt_dtg_old->b_gmt != TRUE )
        {
            /*
              Check if the new time cross the DST boundary.
            */
            b_dst = dt_get_dst_stat_for_utc_sec(t_old_sec);

            if ( b_dst == TRUE )
            {
                /*
                  Adjust for DST
                */
                t_old_sec = t_old_sec + t_dst_offset ;
            }

            /*
              adjust for timezone.
            */
            t_old_sec = t_old_sec + t_tz_offset ;

            /*
              Convert local adjusted seconds into DTG structure,
              call the UTC seconds to DTG conversion, and then
              set the b_gmt field to FALSE, because
              this is a local time.
            */
            x_dt_utc_sec_to_dtg(t_old_sec, pt_dtg_new);
            pt_dtg_new->b_gmt=FALSE;
            
            /*
              Set the DST flag.
            */
            pt_dtg_new->b_dst = b_dst;
        }
        else
        {
            /*
              convert UTC seconds into DTG
            */
            x_dt_utc_sec_to_dtg(t_old_sec, pt_dtg_new);
        }
        
        /*
          Update the Day of week value.
        */
        pt_dtg_new->ui1_dow=(UINT8)dt_dtg_to_dow(pt_dtg_new);
        return DTR_OK;
    }
    return DTR_INV_ARG;
}

/*-------------------------------------------------------------------------
 *
 * Name:  x_dt_leap_yr
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
BOOL x_dt_leap_yr(TIME_T t_yr)
{
    /* A leap year is every year divisible by four except for years which are
       both divisible by 100 and not divisible by 400.

       Therefore, the year 2000 will be a leap year, but the years 1700, 1800,
       and 1900 were not.
       The complete list of leap years in the first half of the 21st century is
       therefore 2000, 2004, 2008, 2012, 2016, 2020, 2024, 2028, 2032, 2036,
       2040, 2044, and 2048.
    */
    if ( (t_yr%4) == 0 )
    {
        if ( (t_yr%100) == 0 )
        {
            if ( (t_yr%400) != 0 )
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
        return TRUE;
    }
    return FALSE;
}


/*****************************************************************
 * Name: x_dt_mjd_bcd_to_dtg
 *
 * Description:  This function converts the 40 bits string that consists of 16 LSBs MJD
 *               (Modified Julian Date) + 24 bits of BCD (binary code decimal) for hour,
 *               minutes, and seconds into a DTG_T structure.
 *
 *               The 40 bits MJD+BCD data are received from DVB TDT and TOT table.
 *
 * Inputs:
 *   pui1_data   A 40 bits code string consisting of 16 LSB's MJD value and + 24 bits of
 *               BCD (binary code decimal)
 *
 * Outputs: -
 *   pt_dtg:     pointer to a DTG_T structure.
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
INT32 x_dt_mjd_bcd_to_dtg(const UINT8* pui1_data,   DTG_T* pt_dtg)
{
    INT32   i4_rc;
    UINT16  ui2_mjd;
    
    if ( pt_dtg != NULL )
    {
        /* Get the MJD: byte 0 to byte 1 */
        ui2_mjd = GET_UINT16_FROM_PTR_BIG_END( &(pui1_data[0]) );

        /* Get the HR value, byte 2 */
        pt_dtg->ui1_hr = (UINT8)BCD_2_DECIMAL(pui1_data[2]);

        /* Get the minute value, byte 3 */
        pt_dtg->ui1_min = (UINT8)BCD_2_DECIMAL(pui1_data[3]);

        /* Get the second value, byte 4 */
        pt_dtg->ui1_sec = (UINT8)BCD_2_DECIMAL(pui1_data[4]);

        /* Fill in the rest of DTG value from MJD */
        dt_mjd_to_dtg(ui2_mjd, pt_dtg);        

        i4_rc = DTR_OK;
    }
    else
    {
        i4_rc = DTR_INV_ARG;
    }
    return i4_rc;
}


/*****************************************************************
 * Name: x_dt_mjd_to_dtg
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
INT32 x_dt_mjd_to_dtg(UINT32 ui4_mjd, DTG_T*  pt_dtg)
{
	if (pt_dtg == NULL)
    {
        return DTR_INV_ARG;
    }
    dt_mjd_to_dtg(ui4_mjd, pt_dtg);
    return DTR_OK;
}

/*****************************************************************
 * Name: x_dt_dtg_to_mjd
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
INT32 x_dt_dtg_to_mjd(const DTG_T*  pt_dtg, UINT16* pui2_mjd)
{
    if (pt_dtg == NULL || pui2_mjd == NULL)
    {
        return DTR_INV_ARG;
    }
    dt_dtg_to_mjd(pt_dtg, pui2_mjd);
    return DTR_OK;
}

/*****************************************************************
 * Name: x_dt_dtg_to_mjd_bcd
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
INT32 x_dt_dtg_to_mjd_bcd(const DTG_T*  pt_dtg,
                          UINT16*       pui2_mjd,
                          UINT16*       pui2_hr_min,
                          UINT8*        pui1_sec)
{
    if (pt_dtg == NULL)
    {
        return DTR_INV_ARG;
    }
    if (pui2_mjd != NULL)
    {
        dt_dtg_to_mjd(pt_dtg, pui2_mjd);
    }
    if (pui2_hr_min != NULL)
    {
        *pui2_hr_min = (UINT16) ((DECIMAL_2_BCD(pt_dtg->ui1_hr) << 8) |
                                 DECIMAL_2_BCD(pt_dtg->ui1_min));
    }
    if (pui1_sec != NULL)
    {
        *pui1_sec = (UINT8) DECIMAL_2_BCD(pt_dtg->ui1_sec);
    }
    return DTR_OK;
}

/*****************************************************************
 * Name: x_dt_bcd_to_sec
 *
 * Description:  This function converts the  24 bits of BCD (binary code decimal)
 *               for hour, minutes, and seconds into number of seconds.
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
INT32 x_dt_bcd_to_sec(const UINT8* pui1_data,   TIME_T* pt_sec)
{
    INT32   i4_rc;
    
    if ( pt_sec != NULL )
    {
        *pt_sec = 0 ;
        
        /* Get the HR value, byte 0 */
        *pt_sec += ( BCD_2_DECIMAL(pui1_data[0]) * 3600 );

        /* Get the minute value, byte 1 */
        *pt_sec += ( BCD_2_DECIMAL(pui1_data[1]) * 60 ) ;

        /* Get the second value, byte 2 */
        *pt_sec += BCD_2_DECIMAL(pui1_data[2]);
        
        i4_rc = DTR_OK;
    }
    else
    {
        i4_rc = DTR_INV_ARG;
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 *
 * Name: x_dt_day_of_year
 *
 * Description: Return number of day from Jan 1 of a year from
 *   a specified calendar date.
 *
 * Input:
 *   pt_dtg:  Structure containing Day-Time-Group values in Local
 *            time frame.
 *
 * Output:
 *   none
 *
 * Returns:
 *   DTR_OK:       Success.
 *   DTR_INV_ARG:  pt_dtg or pui2_day_of_yr is NULL.
------------------------------------------------------------------------*/
INT32 x_dt_day_of_year(const DTG_T* pt_dtg, UINT16*  pui2_day_of_yr)
{
    INT32    i4_rc;
    
    if ( pt_dtg != NULL && pui2_day_of_yr != NULL )
    {
        *pui2_day_of_yr = 0;
            
        if ( pt_dtg->ui1_mo >= 1       &&
             pt_dtg->ui1_mo <= 12 )
        {
            if ( x_dt_leap_yr((TIME_T)(pt_dtg->ui2_yr)) )
            {
                *pui2_day_of_yr = (UINT16)
                    (sec_acu_leap[pt_dtg->ui1_mo]/SEC_IN_DAY);
            }
            else
            {
                *pui2_day_of_yr = (UINT16)
                    (sec_acu[pt_dtg->ui1_mo]/SEC_IN_DAY);
            }
            
            *pui2_day_of_yr = pt_dtg->ui1_day + (*pui2_day_of_yr);
            i4_rc = DTR_OK;
        }
        else
        {
            i4_rc = DTR_FAIL;
        }
    }
    else
    {
        i4_rc =  DTR_INV_ARG;
    }
    return i4_rc;
}

/*****************************************************************
 * Name: dt_reg_dvb_functions_2
 *
 * Description:  This function registers DVB function table.
 *
 * Inputs:
 *   pt_table     DVB function table processing API's.
 *
 * Outputs: -
 *   VOID
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
INT32 dt_reg_dvb_functions_2(const DT_BRDCST_DVB_API_T* pt_table)
{
    INT32   i4_rc = DTR_OK;
    
    pt_dvb_brdcst_api = pt_table;
    
    return i4_rc;
}


/*****************************************************************
 * Name: dt_reg_teletext_functions_2
 *
 * Description:  This function registers Teletext VBI function table.
 *
 * Inputs:
 *   pt_table     Teletext function table processing API's.
 *
 * Outputs: -
 *   VOID
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
INT32 dt_reg_teletext_functions_2(const DT_ANALOG_BRDCST_VBI_TELETEXT_API_T* pt_table)
{
    INT32   i4_rc = DTR_OK;
    
    pt_vbi_teletext_brdcst_api = pt_table;
    
    return i4_rc;
}

/*****************************************************************
 * Name: x_dt_set_config
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

/* static variable to store the current configuration setting. */
static UINT32    _s_ui4_dvb_cntry_flag   = 0;
static UINT32    _s_ui4_loc_dst_flag     = 0;

INT32   x_dt_set_config(UINT32  ui4_flag)
{
    /*
      Process Day-Light-Saving option for converting UTC to local
    */
    if ( (ui4_flag & DT_USE_DST_AT_SPECIFIED_TIME) ||
         (ui4_flag & DT_USE_DST_AT_CURRENT_TIME)
        )
    {
        /* For these 2 options, only one of them can be active
           at the same time.
        */
        if ( (ui4_flag & DT_USE_DST_AT_SPECIFIED_TIME) &&
             (ui4_flag & DT_USE_DST_AT_CURRENT_TIME)
            )
        {
            x_dbg_stmt("{DT} Option DT_USE_DST_AT_SPECIFIED_TIME and DT_USE_DST_AT_CURRENT_TIME can not be specified at the same time.\n");
            return DTR_INV_ARG;
        }

        if ( ui4_flag & DT_USE_DST_AT_SPECIFIED_TIME )
        {
            dt_set_dst_config(DT_USE_DST_AT_SPECIFIED_TIME);
        }

        if ( ui4_flag & DT_USE_DST_AT_CURRENT_TIME )
        {
            dt_set_dst_config(DT_USE_DST_AT_CURRENT_TIME);
        }

        _s_ui4_loc_dst_flag = ui4_flag;
        
    }

    /*
       Process DVB country code/region code selection options.
    */
    if ( (ui4_flag & DT_DVB_MATCH_CNTRY_AND_REG_CODE)   ||
         (ui4_flag & DT_DVB_MATCH_CNTRY_CODE)           ||
         (ui4_flag & DT_DVB_MATCH_ANY_CNTRY_OR_REG_CODE)
        )
    {
        if ( pt_dvb_brdcst_api != NULL )
        {
            pt_dvb_brdcst_api->pf_dvb_set_config(ui4_flag);
        }

        _s_ui4_dvb_cntry_flag = ui4_flag ;
        
    }

    /*
       Process VBI TELETEXT local time offset configuration.
    */
    if ( (ui4_flag & DT_ANALOG_USE_TELETEXT_LOCAL_TIME_OFFSET) ||
         (ui4_flag & DT_ANALOG_UNSET_CONFIG_SETTING)
        )
    {
        if ( pt_vbi_teletext_brdcst_api != NULL )
        {
            pt_vbi_teletext_brdcst_api->pf_teletext_set_config(ui4_flag);
        }
    }

    if (ui4_flag & (DT_DVB_LTO_CHECK_CHG_TIME | DT_DVB_LTO_RAW_TZ_WHEN_DST_CTRL_OFF) )
    {
        if ( pt_dvb_brdcst_api != NULL )
        {
            pt_dvb_brdcst_api->pf_dvb_set_config(ui4_flag);
        }
    }
    
    return  DTR_OK;
}

void dt_show_config_info(void)
{
    if ( _s_ui4_loc_dst_flag & DT_USE_DST_AT_SPECIFIED_TIME )
    {
        x_dbg_stmt("When compute Local time from UTC, adjust for DST offset by: DT_USE_DST_AT_SPECIFIED_TIME\n");
    }

    if ( _s_ui4_loc_dst_flag & DT_USE_DST_AT_CURRENT_TIME )
    {
        x_dbg_stmt("When compute Local time from UTC, adjust for DST offset by: DT_USE_DST_AT_CURRENT_TIME\n");
    }

    if (  _s_ui4_dvb_cntry_flag & DT_DVB_MATCH_CNTRY_AND_REG_CODE )
    {
        x_dbg_stmt("In DVB mode, select local-time-offset setting by matching <exact cntry name and region code>.\n");
    }

    if (  _s_ui4_dvb_cntry_flag & DT_DVB_MATCH_CNTRY_CODE )
    {
        x_dbg_stmt("In DVB mode, select local-time-offset setting by matching <exact cntry name and any region code>.\n");
    }

    if (  _s_ui4_dvb_cntry_flag & DT_DVB_MATCH_ANY_CNTRY_OR_REG_CODE )
    {
        x_dbg_stmt("In DVB mode, select local-time-offset setting by matching <any cntry name and region code>.\n");
    }


    return ;
}


