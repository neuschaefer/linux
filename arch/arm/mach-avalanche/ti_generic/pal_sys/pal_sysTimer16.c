/*
 *
 * pal_sysTimer16.c
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */

/******************************************************************************
 * FILE PURPOSE:    16 bit Timer Module Source
 ********************************************************************************
 * FILE NAME:       timer16.c
 *
 * DESCRIPTION:     Platform and OS independent Implementation for 16 bit timer module
 *
 * REVISION HISTORY:
 * 27 Nov 02 - PSP TII
 *
 * 
 * This timer module has a prescalar and a counter which divide the input reference
 * frequency and an interrupt is generated at the end of the count value.
 * 
 *                        ref_freq 
 * Interrupt freq = ---------------------
 *                  (prescalar * counter)
 * 
 * This timer supports timer values in uSecs. Thus
 * 
 *           prescalar * counter * 1 MHZ
 * uSecs =   --------------------------
 *                  ref_freq
 * 
 *******************************************************************************/
#include <linux/math64.h>
#include <asm-arm/arch-avalanche/generic/pal.h>

#define TIMER16_CNTRL_PRESCALE_ENABLE       0x8000
#define TIMER16_CNTRL_PRESCALE              0x003C
#define TIMER16_CNTRL_MODE                  0x0002

#define TIMER16_MINPRESCALE                 2
#define TIMER16_MAXPRESCALE                 8192
#define TIMER16_MIN_LOAD_VALUE              1
#define TIMER16_MAX_LOAD_VALUE              0xFFFF

#define MHZ                                 1000000

/* set min clock divisor to a little higher value
 * so that we are not close to the edge.
 * so multiply by factor 2
 */
#define TIMER16_MAX_CLK_DIVISOR (TIMER16_MAX_LOAD_VALUE * TIMER16_MAXPRESCALE)
#define TIMER16_MIN_CLK_DIVISOR (TIMER16_MIN_LOAD_VALUE * TIMER16_MINPRESCALE * 2)

#define ERROR   -1
#define OK       0


/****************************************************************************
 * FUNCTION: PAL_sysTimer16GetFreqRange
 ****************************************************************************
 * Description: The routine is called to obtain the values of maximum and
 *              minimum possible timer values  that the timer module can
 *              support.
 ***************************************************************************/

void PAL_sysTimer16GetFreqRange
(
    UINT32  refclk_freq,
    UINT32 *p_max_usec,
    UINT32 *p_min_usec
)
{
    UINT32 refclk_mhz = (refclk_freq / MHZ);
    
    refclk_mhz = (refclk_freq / MHZ);
    /* Min usecs */
    if(p_min_usec)
    {
        if ( refclk_mhz < TIMER16_MIN_CLK_DIVISOR )
        {
            *p_min_usec = TIMER16_MIN_CLK_DIVISOR / refclk_mhz;
        }
        else
        {
            *p_min_usec = 1;
        }
    }        

    if(p_max_usec)
        *p_max_usec = (TIMER16_MAX_CLK_DIVISOR) / refclk_mhz ;
}


/****************************************************************************
 * FUNCTION: PAL_sysTimer16SetParams
 ****************************************************************************
 * Description: The routine is called to configure the timer mode and
 *              time period (in micro seconds).
 *              Max time - 4M uSec
 * param [in/out] err_nsec: timing error --> actual time tick - usec [nSec]
 *                  in: >0 --> make err >=0, <0 --> make err <=0, 0 --> get as close as possible
 *                  out: usec - actual time tick (in nSec)
 *                  NULL: get as close as possible, no output
 ***************************************************************************/
INT32 PAL_sysTimer16SetParams
(
    UINT32                    base_address,
    UINT32                    refclk_freq,
    PAL_SYS_TIMER16_MODE_T    mode,
    UINT32                    usec,
    INT32                     *err_nsec
)
{
    volatile PAL_SYS_TIMER16_STRUCT_T *p_timer;
    UINT32    prescale;
    UINT32    count;
	u64 count64;
    UINT32    refclk_mhz = (refclk_freq / MHZ);
    INT32     in_err = err_nsec == NULL ? 0 : *err_nsec;
    long long calc_err;
    UINT32    actual_clk;

    if( (base_address == 0) || (usec == 0) )
        return ERROR;

    if ( (mode != TIMER16_CNTRL_ONESHOT) && (mode != TIMER16_CNTRL_AUTOLOAD) )
        return ERROR;
        
    /* The min time period is 1 usec and since the reference clock freq is always going 
       to be more than "min" divider value, minimum value is not checked.
       Check the max time period that can be derived from the timer in micro-seconds
    */
    if (usec > ( (TIMER16_MAX_CLK_DIVISOR) / refclk_mhz ))
    {
        return ERROR; /* input argument speed, out of range */
    }

    p_timer = (PAL_SYS_TIMER16_STRUCT_T *)(base_address);

    count64 = ((u64)refclk_freq) * ((u64)usec) + ((u64)MHZ/2);
    count = div_u64 (count64, MHZ );

    for(prescale = 0; prescale < 12; prescale++)
    {
        count = count >> 1;
        if( count <= TIMER16_MAX_LOAD_VALUE )
        {
            break;
        }
    }

    /* calc_err should now be as close as possible */
    /* calc_err = count / clk */
    actual_clk = refclk_freq >> (prescale + 1);
    calc_err = count;
    calc_err *= NSEC_PER_SEC;
    calc_err += actual_clk/2;
    do_div(calc_err, actual_clk);
    /* Now calc_err holds the tick time [nSec] */
    calc_err -= usec * 1000;
    /* Should we get a different calc_err? */
    /* Not if calc_err == 0*/
    /* Maybe if err != NULL*/
    if (err_nsec != NULL)
    {
        if (calc_err != 0)
        {
            if (in_err > 0)
            {
                /* Ensure err is >=0*/
                while (calc_err < 0LL)
                {
                    count++;
                    calc_err = count;
                    calc_err *= NSEC_PER_SEC;
                    calc_err += actual_clk/2;
                    do_div(calc_err, actual_clk);
                    calc_err -= usec * 1000;
                }
            }
            else if (in_err < 0)
            {
                /* Ensure err is <=0 */
                while (calc_err > 0LL)
                {
                    count--;
                    calc_err = count;
                    calc_err *= NSEC_PER_SEC;
                    calc_err += actual_clk/2;
                    do_div(calc_err, actual_clk);
                    calc_err -= usec * 1000;
                }
            }
        }
        *err_nsec = calc_err;
    }

    /* Since the timer adds an additional clock, we remove it */
    count--;

    /*write the load counter value*/
    p_timer->load_reg = count;

    /* write prescalar and mode to control reg */
    p_timer->ctrl_reg = mode | TIMER16_CNTRL_PRESCALE_ENABLE | (prescale << 2 );

    //printk("refclk %d, usec %d, load_reg %d, mode %d, PRESCALE %d, actual_clk %d, in_err %d, out err %lld\n", 
    //       refclk_freq, usec, count, mode, prescale, actual_clk, in_err, calc_err);

    return OK;
}

/****************************************************************************
 * FUNCTION: PAL_sysTimer16Ctrl
 ****************************************************************************
 * Description: The routine is called to start/stop the timer
 *
 ***************************************************************************/
void PAL_sysTimer16Ctrl(UINT32 base_address, PAL_SYS_TIMER16_CTRL_T status)
{
    volatile PAL_SYS_TIMER16_STRUCT_T *p_timer;

    if( base_address )
    {
        p_timer = (PAL_SYS_TIMER16_STRUCT_T *)(base_address);

        if( status == TIMER16_CTRL_START )
        {
            p_timer->ctrl_reg |= TIMER16_CTRL_START;
        }
        else
        {
             p_timer->ctrl_reg &=   ~(TIMER16_CTRL_START);
        }
    }

}

/****************************************************************************
 * FUNCTION: PAL_sysTimer16GetNSecPerCount
 ****************************************************************************
 * Description: Calculate nSec in every timer count
 * param [in] base_address - of specific timer
 * param [in] load_time_nsec - time of reload [nSec]
 * Return - time of each timer count (in nSec)
 ***************************************************************************/
UINT32 PAL_sysTimer16GetNSecPerCount
(
    UINT32                    base_address,
    UINT32                    load_time_nsec
)
{
    volatile PAL_SYS_TIMER16_STRUCT_T *p_timer;
    UINT32 load;
    UINT32 count_nsec;

    if (base_address == 0)
    {
        return 0;
    }

    p_timer = (PAL_SYS_TIMER16_STRUCT_T *)(base_address);

    /* 1 more since it also counts 0 */
    load = p_timer->load_reg + 1;

    /* Calc time of each tick
       count_nsec = load_time_nsec / num counts in load
    */
    count_nsec = load_time_nsec;
    do_div(count_nsec, load);

    return count_nsec;
}

