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
 * $RCSfile: dt_util.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains utility function for calendar dates
 *         calculation.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "dt/x_dt.h"
#include "dbg/x_dbg.h"
#include "dt/_dt.h"



/* number of day in each month, except for the leap year */
static INT16 mosiz[] = {0,31,28,31,30,31,30,31,31,30,31,30,31 };

/*----------------------------------------------------------------
 *
 * Name:  dt_sec_to_delta_dtg
 *
 * Description:   Convert number of seconds to a Delta DTG.
 *
 * Input:
 *    t_delta_sec:   number of seconds.
 *
 * Output
 *    pt_delta:      DTG value (year, month, day, hr, sec) of the
 *                   input 't_delta_sec'.
 *
 * Return:
 *    DTR_OK:     conversion successful.
-----------------------------------------------------------------*/

INT32 dt_sec_to_delta_dtg(TIME_T t_delta_sec, DELTA_DTG_T* pt_delta)
{
    TIME_T t_remainder;
    TIME_T t_sec;

    t_sec = ABS_VAL(t_delta_sec);

    pt_delta->t_day = 0;
    pt_delta->t_hr = 0;
    pt_delta->t_min = 0;
    pt_delta->t_sec = 0;
    
    pt_delta->t_day = t_sec / SEC_IN_DAY ;
    if ( (t_remainder=(t_sec%SEC_IN_DAY)) > 0 )
    {
        pt_delta->t_hr=t_remainder/SEC_IN_HR ;
        if ( (t_remainder=(t_remainder%SEC_IN_HR)) > 0 )
        {
            pt_delta->t_min=t_remainder/SEC_IN_MIN ;
            if ( (t_remainder=(t_remainder%SEC_IN_MIN)) > 0 )
            {
                pt_delta->t_sec=t_remainder;
            }
        }
    }
    return DTR_OK;
}


/*
  dt_delta_dtg_to_sec

  convert delta_dtg to number of seconds.

*/
TIME_T dt_delta_dtg_to_sec(const DELTA_DTG_T* pt_delta)
{
    return ( (pt_delta->t_day * SEC_IN_DAY)    +
             (pt_delta->t_hr *  SEC_IN_HR)     +
             (pt_delta->t_min * SEC_IN_MIN)    +
             (pt_delta->t_sec) );
}


/*--------------------------------------------------------------
 *
 * Name:  dt_add
 *
 * Description: Add a yyyymmddhhmmss to a specified calendar time,
 *              and return the new calendar time.
 *
 * Input:
 *    pt_dtg_old: Pointer to Day-Time-Group (UTC or Local) structure
 *    ui1_day:      Number of days to be added
 *    ui1_hr:       Number of hours to be added
 *    ui1_min:      Number of minutes to be added   
 *    ui1_sec:      Number of seconds to be added
 *
 * Output:
 *    pt_dtg_new: New Day-Time-Group (UTC or Local)
 *
 * Return:
 *    DTR_OK:    Operation success. 
---------------------------------------------------------------------*/

INT32 dt_add
(
    const DTG_T* pt_dtg_old,
    UINT32       ui4_day,
    UINT32       ui4_hr,
    UINT32       ui4_min,
    UINT32       ui4_sec,
    DTG_T*       pt_dtg_new
)
{
    INT32    i4_i;
    BOOL     b_continue;
    TIME_T   t_yr;
    TIME_T   t_mo;
    TIME_T   t_day;
    TIME_T   t_hr;
    TIME_T   t_min;
    TIME_T   t_sec;
    
    if ( pt_dtg_old != NULL && pt_dtg_new != NULL )
    {
        t_yr  = (TIME_T) pt_dtg_old->ui2_yr ;
        t_mo  = (TIME_T) pt_dtg_old->ui1_mo ;
        t_day = (TIME_T) pt_dtg_old->ui1_day  ;
        t_hr  = (TIME_T) pt_dtg_old->ui1_hr  ;
        t_min = (TIME_T) pt_dtg_old->ui1_min  ;
        t_sec = (TIME_T) pt_dtg_old->ui1_sec  ;

        /*
           Add seconds, minutes, hours, and days directly.  Then normalize
           to days, then normalize extra days to months/years.
        */
        t_sec += ui4_sec;
        t_min += ui4_min;
        t_hr  += ui4_hr;
        t_day += ui4_day;

        if ( t_sec > 59)
        {
            i4_i= (INT32)(t_sec / 60);
            t_min += i4_i ;
            t_sec = t_sec % 60 ;
        }

        if ( t_min > 59 )
        {
            i4_i= (INT32)(t_min / 60);
            t_hr += i4_i ;
            t_min = t_min % 60 ;
        }

        if (t_hr > 23)
        {
            i4_i = (INT32)(t_hr / 24);
            t_day += i4_i ;
            t_hr = t_hr % 24;
        }


        b_continue = TRUE;

        while ( t_day > mosiz[t_mo] &&
                b_continue
            )
        {
            /*  Adjust for the Leap Year */
            if ( t_mo==2                     &&
                 x_dt_leap_yr(t_yr) == TRUE
                )
            {
                if (t_day == 29)
                {
                    b_continue=FALSE;
                }
                else
                {
                    t_day -= 29;
                    t_mo++;
                }
            }
            else
            {
                t_day -= mosiz[t_mo];
                t_mo++;
            }

            while (t_mo > 12)
            {
                t_mo-=12;
                t_yr++;
            }
        }
        
        pt_dtg_new->ui2_yr  = (UINT16) t_yr;
        pt_dtg_new->ui1_mo  = (UINT8) t_mo ;
        pt_dtg_new->ui1_day = (UINT8) t_day  ;
        pt_dtg_new->ui1_hr  = (UINT8) t_hr  ;
        pt_dtg_new->ui1_hr  = (UINT8) t_hr  ;
        pt_dtg_new->ui1_min = (UINT8) t_min  ;
        pt_dtg_new->ui1_sec = (UINT8) t_sec  ;
        pt_dtg_new->b_gmt   = pt_dtg_old->b_gmt;
        pt_dtg_new->b_dst   = pt_dtg_old->b_dst;
        
        return DTR_OK;
    }
    return DTR_INV_ARG;
}


/*
  dt_subtract
  
  Subtract a yyyymmddhhmmss from a specified calendar time, and return the
  new calendar time.  This function is used to compute local <->
  GMT time conversion.
  
  Input:
      pt_dtg_old: Pointer to Day-Time-Group (UTC or Local) structure
      ui1_day:      Number of days to be subtracted
      ui1_hr:       Number of hours to be subtracted
      ui1_min:      Number of minutes to be subtracted   
      ui1_sec:      Number of seconds to be subtracted

  Output:
      pt_dtg_new: New Day-Time-Group (UTC or Local)
*/

INT32 dt_subtract
(
    const DTG_T* pt_dtg_old,
    UINT32       ui4_day,
    UINT32       ui4_hr,
    UINT32       ui4_min,
    UINT32       ui4_sec,
    DTG_T*       pt_dtg_new
)
{
    TIME_T   t_yr;
    TIME_T   t_mo;
    TIME_T   t_day;
    TIME_T   t_hr;
    TIME_T   t_min;
    TIME_T   t_sec;
    
    if ( pt_dtg_old != NULL && pt_dtg_new != NULL )
    {
        t_yr  = (TIME_T) pt_dtg_old->ui2_yr ;
        t_mo  = (TIME_T) pt_dtg_old->ui1_mo ;
        t_day = (TIME_T) pt_dtg_old->ui1_day  ;
        t_hr  = (TIME_T) pt_dtg_old->ui1_hr  ;
        t_hr  = (TIME_T) pt_dtg_old->ui1_hr  ;
        t_min = (TIME_T) pt_dtg_old->ui1_min  ;
        t_sec = (TIME_T) pt_dtg_old->ui1_sec  ;

        /*
          Subtract minutes, hour, and days directly.  Then normalize
          to days, then normalize deficient days from months/years.
        */
        t_sec = t_sec - ui4_sec ;
        t_min = t_min - ui4_min ;
        t_hr  = t_hr  - ui4_hr ;
        t_day = t_day - ui4_day ;

        while (t_sec < 0)
        {
            t_sec += 60;
            t_min--;
        }
        while (t_min < 0)
        {
            t_min += 60;
            t_hr--;
        }
        while (t_hr < 0)
        {
            t_hr += 24;
            t_day--;
        }

        while (t_day < 1)
        {
            t_mo--;
            if (t_mo < 1)
            {
                t_mo=12;
                t_yr--;
            }
            if ( t_mo==2 && x_dt_leap_yr(t_yr) == TRUE )
            {
                t_day += 29;
            }
            else
            {
                t_day += mosiz[t_mo];
            }
        }

        pt_dtg_new->ui2_yr  = (UINT16) t_yr;
        pt_dtg_new->ui1_mo  = (UINT8) t_mo ;
        pt_dtg_new->ui1_day = (UINT8) t_day  ;
        pt_dtg_new->ui1_hr  = (UINT8) t_hr  ;
        pt_dtg_new->ui1_hr  = (UINT8) t_hr  ;
        pt_dtg_new->ui1_min = (UINT8) t_min  ;
        pt_dtg_new->ui1_sec = (UINT8) t_sec  ;
        pt_dtg_new->b_gmt   = pt_dtg_old->b_gmt;
        pt_dtg_new->b_dst   = pt_dtg_old->b_dst;

        return DTR_OK;
    }
    return DTR_INV_ARG;
}


/***********************************************************************
  Check if the specified DTG value is later than 00:00:00Z Jan 01, 1970.

  Input:
     pt_dtg:   DTG to be check.

  return:
     TRUE:   pt_dtg is later than 00:00:00Z, Jan 01, 1970.
     FALSE:  pt_dtg is earlier than 00:00:00Z, Jan 01, 1970.
************************************************************************/

BOOL  dt_valid_dtg(const DTG_T* pt_dtg)
{
    TIME_T   t_tz_offset;
    UINT16   ui2_hr;
    UINT16   ui2_mn;
    DTG_T    t_dtg_new;
    
    /*
      Check if this time is older than Jan 01, 1970.
    */
    if ( pt_dtg->b_gmt == TRUE )
    {
        if ( pt_dtg->ui2_yr < GMT_EPOCH_YEAR  )
        {
            return FALSE;
        }
    }
    else
    {
        /*
          convert the input time to GMT and then
          check.
          
          The equation is : GMT = Local - TimeZone - DayLightSaving
          
        */
        
        t_tz_offset = x_dt_get_tz();

        if ( pt_dtg->b_dst == TRUE )
        {
            t_tz_offset = t_tz_offset + 3600 ;
        }   

        /*
          convert the time zone offset to hr and minute.
        */
        ui2_hr = (UINT16) ABS_VAL(t_tz_offset/3600);
        ui2_mn = (UINT16) ABS_VAL((t_tz_offset%3600)/60) ;
        
        if ( t_tz_offset < 0 )
        {
            dt_add(pt_dtg,0,ui2_hr,ui2_mn,0,&t_dtg_new);
        }
        else
        {
            dt_subtract(pt_dtg,0,ui2_hr,ui2_mn,0,&t_dtg_new);
        }

        if ( t_dtg_new.ui2_yr < GMT_EPOCH_YEAR )
        {
            return FALSE;
        }
    }
    return TRUE;
}

/***************************************************************
  Name: dt_dow

  Desc: Compute the Day of the Week from UTC seconds (seconds since
  00:00:00Z, 01 Jan 1970).

  Input:
     t_sec:   Number of GMT seconds.

  return:
     The Day of Week (0-6, 0=Sunday, 6=Saturday).
******************************************************************/

INT16 dt_dow(TIME_T t_sec)
{
    TIME_T   t_mjd;
    INT16    i2_dow;
    
    t_mjd = DT_DIFF_DAYS_MJD_EPOCH_UNIX_EPOCH +
        ((INT32)(t_sec/SEC_IN_DAY)) + 1;

    i2_dow = (INT16) ( ((t_mjd%7)+3)%7 ) ;
    
    return i2_dow;
}


/****************************************************************
  Name dt_dtg_to_dow

  Desc: Compute the Day of the week from the DTG.

  Input:
     t_dtg:   Calendar date (local or utc)

  Return:
     The day of the week (0-6, Sun-Sat).
******************************************************************/
INT16 dt_dtg_to_dow(const DTG_T* pt_dtg)
{
    TIME_T t_sec;
    t_sec=x_dt_dtg_to_sec(pt_dtg);
    return dt_dow(t_sec);
}


/****************************************************************
  Name dt_wk_of_yr

  Desc: Compute the week of the year from the DTG.

  Input:
     t_dtg:   Calendar date (local or utc)

     i4_flag  1:  Week number 1 has first Sunday,
                  previous days are week 0.

              2:  Week number 1 has the first Monday;
                  previous days are week 0.
  Output:
     pi2_wk_of_yr week of year (0-53, Sun-Sat).

  Return:
     DTR_OK

******************************************************************/
INT32 x_dt_wk_of_yr(const DTG_T* pt_dtg, INT32 i4_flag, INT16* pi2_wk_of_yr)
{
    DTG_T  t_jan_first;
    INT16  i2_dow_start = 0;
    UINT16 ui2_day_of_yr;

    /* find out what day of the week is Jan first. */
    t_jan_first.ui2_yr   = pt_dtg->ui2_yr;
    t_jan_first.ui1_mo   = 1 ;
    t_jan_first.ui1_day  = 1;
    t_jan_first.ui1_hr   = 0;
    t_jan_first.ui1_min  = 0;
    t_jan_first.ui1_sec  = 0;
    t_jan_first.b_gmt    = TRUE;
    t_jan_first.b_dst    = FALSE;

    i2_dow_start = dt_dtg_to_dow(&t_jan_first);
    
    /* calculate the offset to be added to the Julian day */
    if ( i4_flag == 1 )
    {
        /*
          Jan first is a Sunday. so the first week is week 1.
        */
        if ( i2_dow_start == 0 )
        {
            i2_dow_start = 7;
        }
        else
        {
            i2_dow_start = i2_dow_start - 1;
        }
    }
    else if ( i4_flag == 2 )
    {
        /*
          Jan first is a Monday. So the first week is week 1.
        */
        if ( i2_dow_start == 1 )
        {
            i2_dow_start = 7 ;
        }
        else
        {
            i2_dow_start = i2_dow_start - 2 ;
        }        
    }

    /* get the Julian days (1 - 366 ) of the date. */
    x_dt_day_of_year(pt_dtg, &ui2_day_of_yr);

    *pi2_wk_of_yr = (INT16)((ui2_day_of_yr + i2_dow_start)/7);
    
    return DTR_OK;
}

/*-----------------------------------------------------------------------
 * Name: dt_print_dtg
 *
 * Description:
 *    This function print out dtg information.  Output format is:
 *
 *    year/month/date hours:minutes:seconds day-of-week (Local or UTC)
 *    (If Local time, is Day-Light-Saving setting on or off).
 *
 * Inputs:
 *   pt_dtg      pointer to DTG_T structure.
 * 
 * Outputs: -
 *   None.
 *
 * Returns:
 *   None
 -------------------------------------------------------------------------*/
VOID dt_print_dtg(DTG_T*  pt_dtg)
{
    x_dbg_stmt("%d/%d/%d %d:%d:%d ",
               pt_dtg->ui2_yr,
               pt_dtg->ui1_mo,
               pt_dtg->ui1_day,
               pt_dtg->ui1_hr,
               pt_dtg->ui1_min,
               pt_dtg->ui1_sec);

    switch(pt_dtg->ui1_dow)
    {
        case 0:
        {
            x_dbg_stmt("Sun ");
        }
        break;

        case 1:
        {
            x_dbg_stmt("Mon ");
        }
        break;

        case 2:
        {
            x_dbg_stmt("Tues ");
        }
        break;

        case 3:
        {
            x_dbg_stmt("Wed ");
        }
        break;

        case 4:
        {
            x_dbg_stmt("Thur ");
        }
        break;

        case 5:
        {
            x_dbg_stmt("Fri ");
        }
        break;

        case 6:
        {
            x_dbg_stmt("Sat ");
        }
        break;

        default:
        {
            x_dbg_stmt("Unknown ");
        }
        break;
    }

    if ( pt_dtg->b_gmt == TRUE )
    {
        x_dbg_stmt("UTC ");
    }
    else
    {
        x_dbg_stmt("Local ");
        if ( pt_dtg->b_dst == TRUE )
        {
            x_dbg_stmt("(DST on) ");
        }
        else
        {
            x_dbg_stmt("(DST off) ");
        }
    }
}


VOID dt_print_utc(TIME_T t_utc_sec)
{
    DTG_T   t_dtg;

    x_dt_utc_sec_to_dtg(t_utc_sec, &t_dtg);
    dt_print_dtg(&t_dtg);
}
            
/*****************************************************************
 * Name: dt_mjd_to_dtg
 *
 * Description:  This function is convert MJD (Modified Julian Date)
 *               to a day time group structure.
 *
 * Inputs:
 *   ui4_mjd     MJD value
 *
 * Outputs: -
 *   pt_dtg:     pointer to a DTG_T structure.
 *
 * Returns:
 *   VOID
 *
 *****************************************************************/
VOID dt_mjd_to_dtg(UINT32 ui4_mjd, DTG_T*  pt_dtg)
{
    INT32 i4_year;
    INT32 i4_month;
    UINT32 ui4_day;

    /*
     
      original formula:

      i4_year = (INT32)((ui4_mjd-15078.2)/365.25);

    */
    i4_year = (INT32)((ui4_mjd-15078.20)/365.25);

    /*
      original formula:

      i4_month = (INT32)((ui4_mjd-14956.1-(INT32)(i4_year*365.25))/30.6001);
    */
    i4_month = (INT32)((ui4_mjd-14956.1-(INT32)(i4_year*365.25))/30.6001);

    /*
      original formula:

      day_of_month = (UINT8) ( ui4_mjd - 14956 -
                                (INT32)(i4_year*365.2500) -
                                (INT32)(i4_month*30.6001) )
    */
    ui4_day = ((UINT32) ( ui4_mjd - 14956 -
                         (INT32)(i4_year*365.25) -
                         (INT32)(i4_month*30.6001) ));

    pt_dtg->ui1_day = (UINT8) ui4_day;
    

    if ( i4_month == 14 ||  i4_month == 15 )
    {
        pt_dtg->ui2_yr= (UINT16) (i4_year+1) ;
        pt_dtg->ui1_mo= (UINT8)  (i4_month-13);
    }
    else
    {
        pt_dtg->ui2_yr= (UINT16) i4_year;
        pt_dtg->ui1_mo= (UINT8) (i4_month-1);
    }

    pt_dtg->ui1_dow=(UINT8) ( (ui4_mjd+2)%7+1 ) ;  /* Monday = 1, Sunday = 7 */
    /* convert to Sunday = 0 ; Saturday = 6. */
    if (pt_dtg->ui1_dow == 7 )
    {
        pt_dtg->ui1_dow = 0 ;
    }
    pt_dtg->ui2_yr= (UINT16) (pt_dtg->ui2_yr+1900);
    pt_dtg->b_gmt = TRUE;

    return;
}

/*****************************************************************
 * Name: dt_dtg_to_mjd
 *
 * Description:  This function is convert a day time group structure
 *               to MJD (Modified Julian Date). Only Year/Month/Day
 *               in the DTG structure count.
 *
 * Inputs:
 *   pt_dtg:     pointer to a DTG_T structure.
 *
 * Outputs:
 *   pui4_mjd    pointer to MJD value
 *
 * Returns:
 *   VOID
 *
 *****************************************************************/
VOID dt_dtg_to_mjd(const DTG_T*  pt_dtg, UINT16* pui2_mjd)
{
    INT32 i4_year;
    INT32 i4_month;
    UINT32 ui4_day;
    INT32 i4_century;
    INT32 i4_year_lo;
    INT32 i4_mjd;

    i4_year  = (INT32) pt_dtg->ui2_yr;
    i4_month = pt_dtg->ui1_mo;
    ui4_day  = pt_dtg->ui1_day;

    if (i4_month < 3)
    {
        i4_month += 12;
        i4_year  -= 1;
    }

    i4_century = i4_year / 100;
    i4_year_lo = i4_year % 100;

    i4_mjd = (INT32) (i4_century * 36524.25) + (INT32) (i4_year_lo * 365.25) +\
             (INT32) (i4_month * 30.6 - 91.4) + ui4_day - 678882;

    *pui2_mjd = (UINT16) i4_mjd;
}



