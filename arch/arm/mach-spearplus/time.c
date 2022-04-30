/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2007 by viresh Kumar  			*/
/*   viresh.kumar@st.com							*/
/*										*/
/*   This program is free software; you can redistribute it and/or modify	*/
/*   it under the terms of the GNU General Public License as published by	*/
/*   the Free Software Foundation; either version 2 of the License, or		*/
/*   (at your option) any later version.					*/
/*										*/
/*   This program is distributed in the hope that it will be useful,		*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 		*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*/
/*   GNU General Public License for more details.				*/
/*										*/
/*   You should have received a copy of the GNU General Public License		*/
/*   along with this program; if not, write to the				*/
/*   Free Software Foundation, Inc.,						*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.			*/
/********************************************************************************/

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/system.h>
#include <asm/leds.h>
#include <asm/mach-types.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mach/time.h>
#include <asm/uaccess.h>
#include <asm/hardware.h>
#include <asm/arch/timex.h>
#include <asm/mach/irq.h>

#include <asm/arch/spear.h>
#include <linux/rtc.h>

#include <asm/arch/spr_timer.h>	/* HCL definitions */

#include <../drivers/rtc/spr_rtc_st_hal.h>

extern raw_spinlock_t rtc_lock;

extern int (*set_rtc)(void);
extern int rtc_tm_to_time(struct rtc_time *tm, unsigned long *time);
extern int rtc_tm_to_time(struct rtc_time *tm, unsigned long *time);
/* Local BCD/BIN conversion macros: */

#ifdef BCD_TO_BIN
#undef BCD_TO_BIN
#endif   /* End of BCD_TO BIN */
#define BCD_TO_BIN(val) ((val)=((val)&15) +\
        ((val&0xf0)>>4)*10 + ((val&0xf00)>>8)*100  + ((val&0xf000)>> 12)*1000)

#ifdef BIN_TO_BCD
#undef BIN_TO_BCD
#endif   /* End of BIN_TO BCD */

#define BIN_TO_BCD(val)	((val)= (((val)/1000)<< 12) +\
        (((val%1000)/100)<<8) + ((((val%1000)%100)/10)<<4) + (val)%10)


#define CONFIG_ARM_CLK CLOCK_TICK_RATE
#define PRESCALER   4
#define CLOCKS_PER_USEC ((CONFIG_ARM_CLK>>PRESCALER)/1000000)

#define VA_VIC_BASE  IO_ADDRESS(SPEARPLUS_VIC1_BASE) 

static t_tmr_id  tmr0_id, tmr1_id ;
static t_tmr_id  tmr0_1_id ;

static unsigned long epoch = 1900;	/* year corresponding to 0x00	*/

static const unsigned char days_in_mo[] =
{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static volatile unsigned long timer_cnt;

static unsigned long timer_reload;

//----------------------------------------------
extern void  spear_timer0_1_init( void );
extern void  spear_timer1_0_init( void );
extern void  spear_timer1_1_init( void );
extern void  spear_timer2_0_init( void );
extern void  spear_timer2_1_init( void );
extern void  spear_timer3_0_init( void );
extern void  spear_timer3_1_init( void );



/****************************************************************************/
/* NAME: spear_get_rtc_time                                                 */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is included to get the rtc time                        */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*         None                                                             */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN:                                                                  */
/*        long                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
    static inline 
unsigned long spear_get_rtc_time(void)
{
#ifdef CONFIG_SPEAR_RTC_ST
    struct rtc_time wtime;
    unsigned long time_sec;                   
    unsigned int time,date;

    RTC_GetTime ( &time );
    RTC_GetDate ( &date );

    wtime.tm_sec =  ( time & ( SECONDS_UNITS | SECONDS_TENS ) ) >> SECONDS_SHIFT;
    wtime.tm_min = ( time & ( MINUTES_UNITS | MINUTES_TENS ) ) >> MINUTES_SHIFT;
    wtime.tm_hour = ( time & ( HOURS_UNITS | HOURS_TENS ) ) >> HOURS_SHIFT ;
    wtime.tm_mday = ( date & ( DAYS_UNITS | DAYS_TENS ) ) >> DAYS_SHIFT;
    wtime.tm_mon = ( date & ( MONTHS_UNITS | MONTHS_TENS ) ) >> MONTHS_SHIFT;
    wtime.tm_year = ( date & ( YEARS_UNITS | YEARS_TENS | YEARS_HUNDREDS |YEARS_MILLENIUMS ) ) >> YEARS_SHIFT;

    BCD_TO_BIN ( wtime.tm_sec );
    BCD_TO_BIN ( wtime.tm_min );
    BCD_TO_BIN ( wtime.tm_hour );
    BCD_TO_BIN ( wtime.tm_mday );
    BCD_TO_BIN ( wtime.tm_mon );
    BCD_TO_BIN ( wtime.tm_year );

    wtime.tm_mon--; 

    rtc_tm_to_time ( &wtime,&time_sec );

    return time_sec;
#else
    return 0;
#endif
}


/****************************************************************************/
/* NAME:  spear_set_rtc		                                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is to set the rtc time                                 */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   None                                                             */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: int                                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
    static int 
spear_set_rtc ( void )
{

#ifdef CONFIG_SPEAR_RTC_ST
    struct rtc_time rtc_tm;
    unsigned char mon, day, hrs, min, sec, leap_yr;
    unsigned int yrs;
    unsigned int time;
    unsigned int date;
    //printk ( "\nxtime.tv_sec= %u",xtime.tv_sec );

    rtc_time_to_tm ( xtime.tv_sec, &rtc_tm );

    yrs = rtc_tm.tm_year + 1900;
    mon = rtc_tm.tm_mon + 1;        /* tm_mon starts at zero */
    day = rtc_tm.tm_mday;
    hrs = rtc_tm.tm_hour;
    min = rtc_tm.tm_min;
    sec = rtc_tm.tm_sec;

    if ( yrs > 2000 )
        printk ( "\ndate: day=%d mon=%d yr=%d hr=%d min =%d sec=%d",
                rtc_tm.tm_mday,rtc_tm.tm_mon,rtc_tm.tm_year,rtc_tm.tm_hour,
                rtc_tm.tm_min,rtc_tm.tm_sec );

    if ( yrs < 1970 )
        return -EINVAL;

    leap_yr =  ( ( ! ( yrs % 4 ) && ( yrs % 100 ) ) || ! ( yrs % 400 ) );

    if ( ( mon > 12 ) || ( day == 0 ) )
        return -EINVAL;

    if ( day > ( days_in_mo[mon] +  ( ( mon == 2) && leap_yr ) ) )
        return -EINVAL;

    if ( ( hrs >= 24 ) || ( min >= 60 ) || ( sec >= 60 ) )
        return -EINVAL;
#if 0    //Arif: We don't need Above mentioned
    if ((yrs -= epoch) > 255)    /* They are unsigned */
        return -EINVAL;

    if (yrs > 169) {
        return -EINVAL;
    }

    if (yrs >= 100)
        yrs -= 100;
#else
    yrs -= epoch ;/*As disabled above: it is enabled  */
#endif

    BIN_TO_BCD ( sec );
    BIN_TO_BCD ( min );
    BIN_TO_BCD ( hrs );
    BIN_TO_BCD ( day );
    BIN_TO_BCD ( mon );
    BIN_TO_BCD ( yrs );

    time = sec | min << MINUTES_SHIFT | hrs << HOURS_SHIFT;
    date = day | mon << MONTHS_SHIFT | yrs << YEARS_SHIFT;

    spin_lock_irq ( &rtc_lock );
    RTC_SetTime ( time );
    RTC_SetDate ( date );
    spin_unlock_irq ( &rtc_lock );
#endif
    return 1;
}

/****************************************************************************/
/* NAME:  spearplus_gettimeoffset                                           */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is to get the time offset                              */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   None                                                             */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: long                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
    unsigned long 
spearplus_gettimeoffset ( void )
{
    long ticks1, ticks2, status=0,usec;

    TMR_GetCounterValue ( tmr0_id, &usec );
    ticks2 = usec &0xffff;
    do {
        ticks1 = ticks2;
        status = __raw_readl ( VA_VIC_BASE + VIC_IRQ_STATUS ); 
        TMR_GetCounterValue ( tmr0_id, &usec );
        ticks2 = usec & 0xffff;
    } while ( ticks2 < ticks1 );

    if ( status & ( 1 << IRQ_GPT0 ) ) 
        ticks2 += timer_reload;


    return ticks2 / CLOCKS_PER_USEC;

}


/****************************************************************************/
/* NAME:  spearplus_timer_read                                              */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is to read the timer                                   */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :    nr  : Timer Number                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN:long                                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
    unsigned long 
spearplus_timer_read ( int nr )
{
    unsigned long usec;
    /* Timer 0 is used by spearplus _gettimeoffset */
    switch ( nr ) 
    {
        case 1:
            TMR_GetCounterValue ( tmr1_id, &usec );
            return usec;
            break;

        default:
            printk ( KERN_ERR "spearplus_timer_read(%d): unknown timer\n", nr);
            BUG();
    }
    return 0;
}

EXPORT_SYMBOL ( spearplus_timer_read );


/****************************************************************************/
/* NAME:  spearplus_timer_interrupt                                         */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is to generate interrupt request                       */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :     irq , *dev_id , *regs                                          */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/*                                                                          */
/* RETURN:                                                                  */
/*         irqreturn_t                                                      */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
    static irqreturn_t
spearplus_timer_interrupt ( int irq, void *dev_id, struct pt_regs *regs )
{
    write_seqlock ( &xtime_lock );

    TMR_ClearIRQSrc ( TMR_ID_0 );        /* clear any pending interrupts */

    timer_cnt++;

    timer_tick (  );

    write_sequnlock ( &xtime_lock );

    return IRQ_HANDLED;

}

static struct irqaction spearplus_timer_irq = 
{
    .name           = "Spearplus Timer Tick",
    .flags          = SA_INTERRUPT,	
    .handler        = spearplus_timer_interrupt
};


/****************************************************************************/
/* NAME:          __init spearplus_time_init                                */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is to initialize the timer                             */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*         None                                                             */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: None                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
    void 
__init spearplus_time_init ( void )
{

    t_tmr_error tmr_error;
    struct timespec tvi;
    
#ifdef CONFIG_SPEAR_RTC_ST
    RTC_SetBaseAddress ( VA_SPEARPLUS_START_APB_RTC ); 
#endif
    set_rtc = spear_set_rtc;

    tvi.tv_nsec = 0;
    tvi.tv_sec = spear_get_rtc_time();

    do_settimeofday ( &tvi );

    timer_reload = ( ( CLOCK_TICK_RATE >> TMR_CLK_PRESCALER_16 )  / ( HZ ) ); 


    TMR_Init( TMR_DEVICE_ID_0, ( t_logical_address )VA_SPEARPLUS_START_APB_GPT0 ); 

    TMR_SetBaseAddress( TMR_DEVICE_ID_0, ( t_logical_address ) VA_SPEARPLUS_START_APB_GPT0 ); 

    TMR_Reset( TMR_DEVICE_ID_0 );

    tmr_error = TMR_AllocTimer ( &tmr0_id );   /* Allocate timer */
    if ( tmr_error != TMR_OK )
    {
        printk ( KERN_INFO "tmr allocation error = %u ", tmr_error );	 
    }
    else
    {
        printk ( "\ntmr allocated for ticks Tmr id = %u\n ", tmr0_id );   	 
    }

    tmr_error = TMR_Configure(  tmr0_id,
            TMR_MODE_AUTO_RELOAD,
            TMR_MODE_NO_CAPTURE,
            ( ( CLOCK_TICK_RATE>>TMR_CLK_PRESCALER_16 )  / ( HZ ) ),
            TMR_CLK_PRESCALER_16
            );

    printk ( KERN_INFO "Timer0: time_init():  TICK_RATE: %u, HZ: , period: %u\n", CLOCK_TICK_RATE, HZ );

    setup_irq ( IRQ_GPT0, &spearplus_timer_irq ); 

    TMR_EnableIRQSrc ( tmr0_id );

    tmr_error = TMR_StartTimer ( tmr0_id );
    if ( tmr_error != TMR_OK )
    {
        printk(KERN_INFO "tmr error 0 = %u ", tmr_error); 
    }

    //---------- test func called up --------------------    
    //spear_timer0_1_init();
    //spear_timer1_0_init();
    //spear_timer1_1_init();
    //spear_timer2_0_init();
    //spear_timer2_1_init();
    //spear_timer3_0_init();
    //spear_timer3_1_init();
}

struct sys_timer spearplus_timer = 
{
    .init           = spearplus_time_init,
    .offset         = spearplus_gettimeoffset,
};
/* End of file - time.c */
