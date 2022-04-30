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



#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/rtc.h>

#include <linux/interrupt.h>
#include <linux/rtc.h>
#include <asm/mach/irq.h>

#include <asm/arch/hardware.h>

#include <asm/arch/spear.h>
#include "spr_rtc_st_hal.h"

extern raw_spinlock_t rtc_lock;

/* Local BCD/BIN conversion macros: */
#ifdef BCD_TO_BIN
#undef BCD_TO_BIN
#endif

#define BCD_TO_BIN(val)	((val)=((val)&15) + ((val&0xf0)>>4)*10 + ((val&0xf00)>>8)*100  + ((val&0xf000)>> 12)*1000)
#ifdef BIN_TO_BCD
#undef BIN_TO_BCD
#endif

#define BIN_TO_BCD(val)	((val)= (((val)/1000)<< 12) + (((val%1000)/100)<<8) + ((((val%1000)%100)/10)<<4) + (val)%10)

/*
 *	We sponge a minor off of the misc major. No need slurping
 *	up another valuable major dev number for this. If you add
 *	an ioctl, make sure you don't conflict with SPARC's RTC
 *	ioctls.
 */

static struct fasync_struct *rtc_async_queue;

static DECLARE_WAIT_QUEUE_HEAD(rtc_wait);

static ssize_t SPEAR_RTC_Read(struct file *file, char *buf,
			size_t count, loff_t *ppos);

static int SPEAR_RTC_Ioctl(struct inode *inode, struct file *file,
		     unsigned int cmd, unsigned long arg);

static unsigned int SPEAR_RTC_Poll(struct file *file, poll_table *wait);

static void SPEAR_RTC_GetTime (struct rtc_time *rtc_tm);
static void SPEAR_RTC_GetAlmTime (struct rtc_time *alm_tm);

static inline unsigned char RTC_IsUpdating(void);

static int RTC_ReadProc(char *page, char **start, off_t off,
                         int count, int *eof, void *data);


#define RTC_IS_OPEN		0x01	/* means /dev/rtc is in use	*/

/*
 * REVISIT: fix this comment:
 * rtc_status is never changed by rtc_interrupt, and ioctl/open/close is
 * protected by the big kernel lock.
 */
static unsigned long rtc_status = 0;	/* bitmapped status byte.	*/
static unsigned long rtc_irq_data = 0;	/* our output to the world	*/

/*
 *	If this driver ever becomes modularised, it will be really nice
 *	to make the epoch retain its value across module reload...
 */

static unsigned long epoch = 1900;	/* year corresponding to 0x00	*/

static const unsigned char days_in_mo[] =
{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


/*
 *	A very tiny interrupt handler. It runs with SA_INTERRUPT set.
 */

irqreturn_t SPEAR_RTC_IntHandler(int irq, void *dev_id)
{
	/*
	 *	Either an alarm interrupt or update complete interrupt.
	 *	We store the status in the low byte and the number of
	 *	interrupts received since the last read in the remainder
	 *	of rtc_irq_data.
	 */

	spin_lock (&rtc_lock);

	rtc_irq_data += 0x100;
	rtc_irq_data &= ~0xff;
	rtc_irq_data |= RTC_GetIntStatus();

	if(rtc_irq_data & RTC_INT_STATUS)
	RTC_ClearInt();
	/*if (rtc_irq_data & OMAP_RTC_STATUS_ALARM)
		CMOS_WRITE(OMAP_RTC_STATUS_ALARM, OMAP_RTC_STATUS_REG);*/

	spin_unlock (&rtc_lock);

	/* Now do the rest of the actions */
	wake_up_interruptible(&rtc_wait);

	kill_fasync (&rtc_async_queue, SIGIO, POLL_IN);
	return IRQ_HANDLED;
}

/*
 *	Now all the various file operations that we export.
 */

static ssize_t SPEAR_RTC_Read(struct file *file, char *buf,
			size_t count, loff_t *ppos)
{
	DECLARE_WAITQUEUE(wait, current);
	unsigned long data;
	ssize_t retval;

	if (count < sizeof(unsigned long))
		return -EINVAL;

	add_wait_queue(&rtc_wait, &wait);
	set_current_state(TASK_INTERRUPTIBLE);

	for (;;) {
		spin_lock_irq (&rtc_lock);
		data = rtc_irq_data;
		if (data != 0) {
			rtc_irq_data = 0;
			break;
		}
		spin_unlock_irq (&rtc_lock);

		if (file->f_flags & O_NONBLOCK) {
			retval = -EAGAIN;
			goto out;
		}
		if (signal_pending(current)) {
			retval = -ERESTARTSYS;
			goto out;
		}
		schedule();
	}

	spin_unlock_irq (&rtc_lock);
	retval = put_user(data, (unsigned long *)buf);
	if (!retval)
		retval = sizeof(unsigned long);
 out:
	set_current_state(TASK_RUNNING);
	remove_wait_queue(&rtc_wait, &wait);

	return retval;
}


/****************************************************************************/
/* NAME:           SPEAR_RTC_Ioctl                                        */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine resets the enable bit of the register                  */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   inode. file* , cmd,arg                                           */
/*                                                                          */
/* OUT :   arg                                                              */
/*                                                                          */
/*                                                                          */
/* RETURN:                                                                  */
/*        int                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static int SPEAR_RTC_Ioctl(struct inode *inode, struct file *file, unsigned int cmd,
		     unsigned long arg)
{
	struct rtc_time wtime;

	switch (cmd) {

	case RTC_ALM_READ:	/* Read the present alarm time */
	{
		/*
		 * This returns a struct rtc_time. Reading >= 0xc0
		 * means "don't care" or "match all". Only the tm_hour,
		 * tm_min, and tm_sec values are filled in.
		 */
		memset(&wtime, 0, sizeof(struct rtc_time));
		SPEAR_RTC_GetAlmTime(&wtime);
		break;
	}
	case RTC_ALM_SET:	/* Store a time into the alarm */
	{
		struct rtc_time alm_tm;
		unsigned char mon, day, hrs, min, sec, leap_yr;
		unsigned int yrs;
		unsigned int time;
		unsigned int date;


		if (copy_from_user(&alm_tm, (struct rtc_time*)arg,
				   sizeof(struct rtc_time)))
			return -EFAULT;

		yrs = alm_tm.tm_year + 1900;
		mon = alm_tm.tm_mon + 1;
		day = alm_tm.tm_mday;
		hrs = alm_tm.tm_hour;
		min = alm_tm.tm_min;
		sec = alm_tm.tm_sec;

		if (yrs < 1970)
			return -EINVAL;

		leap_yr = ((!(yrs % 4) && (yrs % 100)) || !(yrs % 400));

		if ((mon > 12) || (day == 0))
			return -EINVAL;

		if (day > (days_in_mo[mon] + ((mon == 2) && leap_yr)))
			return -EINVAL;

		if ((hrs >= 24) || (min >= 60) || (sec >= 60))
			return -EINVAL;

#if 0
		if ((yrs -= epoch) > 255)    /* They are unsigned */
			return -EINVAL;

		if (yrs > 169) {   //Arif : we don't need it as we can go beyond 2070
			return -EINVAL;
		}

		if (yrs >= 100)  //Arif: we can have more than 100 yrs as we can store YYYY & our BIN_TO_BCD supports
			yrs -= 100;
#else
 yrs -= epoch ;/*As disabled above: it is enabled  */
#endif
		BIN_TO_BCD(sec);
		BIN_TO_BCD(min);
		BIN_TO_BCD(hrs);
		BIN_TO_BCD(day);
		BIN_TO_BCD(mon);
		BIN_TO_BCD(yrs);
	
		time = sec | min << MINUTES_SHIFT | hrs << HOURS_SHIFT;
		date = day | mon << MONTHS_SHIFT | yrs << YEARS_SHIFT;

		spin_lock_irq(&rtc_lock);

		RTC_SetAlarmTime(time);
		RTC_SetAlarmDate(date);
		spin_unlock_irq(&rtc_lock);

		return 0;
	}
	case RTC_RD_TIME:	/* Read the time/date from RTC	*/
	{
		memset(&wtime, 0, sizeof(struct rtc_time));
		SPEAR_RTC_GetTime(&wtime);
		break;
	}
	case RTC_SET_TIME:	/* Set the RTC */
	{
		struct rtc_time rtc_tm;
		unsigned char mon, day, hrs, min, sec, leap_yr;
		//unsigned char save_control;
		unsigned int yrs;
		unsigned int time;
		unsigned int date;


		if (!capable(CAP_SYS_TIME))
			return -EACCES;

		if (copy_from_user(&rtc_tm, (struct rtc_time*)arg,
				   sizeof(struct rtc_time)))
			return -EFAULT;


		yrs = rtc_tm.tm_year + 1900;
		mon = rtc_tm.tm_mon + 1;   /* tm_mon starts at zero */
		day = rtc_tm.tm_mday;
		hrs = rtc_tm.tm_hour;
		min = rtc_tm.tm_min;
		sec = rtc_tm.tm_sec;

		if (yrs < 1970)
			return -EINVAL;

		leap_yr = ((!(yrs % 4) && (yrs % 100)) || !(yrs % 400));

		if ((mon > 12) || (day == 0))
			return -EINVAL;

		if (day > (days_in_mo[mon] + ((mon == 2) && leap_yr)))
			return -EINVAL;

		if ((hrs >= 24) || (min >= 60) || (sec >= 60))
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


		BIN_TO_BCD(sec);
		BIN_TO_BCD(min);
		BIN_TO_BCD(hrs);
		BIN_TO_BCD(day);
		BIN_TO_BCD(mon);
		BIN_TO_BCD(yrs);


		time = sec | min << MINUTES_SHIFT | hrs << HOURS_SHIFT;
		date = day | mon << MONTHS_SHIFT | yrs << YEARS_SHIFT;

		spin_lock_irq(&rtc_lock);
		RTC_SetTime(time);
		RTC_SetDate(date);
		spin_unlock_irq(&rtc_lock);
		return 0;
	}
	case RTC_EPOCH_READ:	/* Read the epoch.	*/
	{
		return put_user (epoch, (unsigned long *)arg);
	}
	case RTC_EPOCH_SET:	/* Set the epoch.	*/
	{
		unsigned long tmp;
		if (copy_from_user(&tmp, (unsigned long*)arg,
						   sizeof(unsigned long)))
			return -EFAULT;
		/*
		 * There were no RTC clocks before 1900.
		 */
		//if (arg < 1900)
		if (tmp < 1900)
			return -EINVAL;

		if (!capable(CAP_SYS_TIME))
			return -EACCES;
		epoch = tmp;

		return 0;
	}
	default:
#if	!defined(CONFIG_ARCH_OMAP)
		return -ENOTTY;
#else
		return -EINVAL;
#endif
	}
	return copy_to_user((void *)arg, &wtime, sizeof wtime) ? -EFAULT : 0;
}



/*
 *	We enforce only one user at a time here with the open/close.
 *	Also clear the previous interrupt data on an open, and clean
 *	up things on a close.
 */

/* We use rtc_lock to protect against concurrent opens. So the BKL is not
 * needed here. Or anywhere else in this driver. */
static int SPEAR_RTC_Open(struct inode *inode, struct file *file)
{
	spin_lock_irq (&rtc_lock);

	if(rtc_status & RTC_IS_OPEN)
		goto out_busy;

	rtc_status |= RTC_IS_OPEN;

	rtc_irq_data = 0;
	spin_unlock_irq (&rtc_lock);
	return 0;

out_busy:
	spin_unlock_irq (&rtc_lock);
	return -EBUSY;
}

static int SPEAR_RTC_FaSync (int fd, struct file *filp, int on)

{
	return fasync_helper (fd, filp, on, &rtc_async_queue);
}

static int SPEAR_RTC_Release(struct inode *inode, struct file *file)
{
//	unsigned char tmp;

	/*
	 * Turn off all interrupts once the device is no longer
	 * in use, and clear the data.
	 */

	spin_lock_irq(&rtc_lock);
	RTC_IntDisable();
	spin_unlock_irq(&rtc_lock);

	if (file->f_flags & FASYNC) {
		SPEAR_RTC_FaSync (-1, file, 0);
	}

	spin_lock_irq (&rtc_lock);
	rtc_irq_data = 0;
	spin_unlock_irq (&rtc_lock);

	/* No need for locking -- nobody else can do anything until this rmw
	 * is committed, and we don't implement timer support in omap-rtc.
	 */
	rtc_status &= ~RTC_IS_OPEN;
	return 0;
}

/* Called without the kernel lock - fine */
static unsigned int SPEAR_RTC_Poll(struct file *file, poll_table *wait)
{
	unsigned long l;

	poll_wait(file, &rtc_wait, wait);

	spin_lock_irq (&rtc_lock);
	l = rtc_irq_data;
	spin_unlock_irq (&rtc_lock);

	if (l != 0)
		return POLLIN | POLLRDNORM;
	return 0;
}


/*
 *	The various file operations we support.
 */

static struct file_operations RTC_Fops = {
	owner:		THIS_MODULE,
	llseek:		no_llseek,
	read:		SPEAR_RTC_Read,
	poll:		SPEAR_RTC_Poll,
	ioctl:		SPEAR_RTC_Ioctl,
	open:		SPEAR_RTC_Open,
	release:	SPEAR_RTC_Release,
	fasync:		SPEAR_RTC_FaSync,
};

static struct miscdevice rtc_dev=
{
	RTC_MINOR,
	"rtc",
	&RTC_Fops
};

static int __init SPEAR_RTC_Init(void)
{
#ifdef CONFIG_ARCH_SPEARPLUS
	RTC_SetBaseAddress(VA_SPEARPLUS_START_APB_RTC);
#else
	RTC_SetBaseAddress(VA_SPEARBASIC_START_APB_RTC);
#endif
	RTC_IntEnable();

#ifdef CONFIG_PREEMPT_HARDIRQS
	if (request_irq(IRQ_BASIC_RTC, SPEAR_RTC_IntHandler, 0, "spear-rtc alarm", NULL))
#else
	if (request_irq(IRQ_BASIC_RTC, SPEAR_RTC_IntHandler, SA_INTERRUPT, "spear-rtc alarm", NULL))
#endif
	{
		printk(KERN_ERR "%s: RTC alarm interrupt IRQ%d is not free.\n",
		       rtc_dev.name, IRQ_BASIC_RTC);
		return -EIO;
	}
	misc_register(&rtc_dev);
	create_proc_read_entry ("driver/rtc", 0, 0, RTC_ReadProc, NULL);

	RTC_SetComparisionMask(MASK_SEC| MASK_MIN|MASK_HOUR|MASK_DAY|MASK_MONTH|MASK_YEAR);
	printk("\nReal Time Clock Driver Initialized\n");

	return 0;
}


static void __exit SPEAR_RTC_Exit (void)
{
	free_irq (IRQ_BASIC_RTC, NULL);
	remove_proc_entry ("driver/rtc", NULL);

	misc_deregister(&rtc_dev);

}


/*
 *	Info exported via "/proc/driver/rtc".
 */

static int RTC_ProcOutput (char *buf)
{
#define YN(value) ((value) ? "yes" : "no")
#define NY(value) ((value) ? "no" : "yes")
	char *p;
	struct rtc_time tm;

	p = buf;

	SPEAR_RTC_GetTime(&tm);

	/*
	 * There is no way to tell if the luser has the RTC set for local
	 * time or for Universal Standard Time (GMT). Probably local though.
	 */
	p += sprintf(p,
		     "rtc_time\t: %02d:%02d:%02d\n"
		     "rtc_date\t: %04d-%02d-%02d\n"
	 	     "rtc_epoch\t: %04lu\n",
		     tm.tm_hour, tm.tm_min, tm.tm_sec,
		     tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, epoch);

	SPEAR_RTC_GetAlmTime(&tm);

	/*
	 * We implicitly assume 24hr mode here. Alarm values >= 0xc0 will
	 * match any value for that particular field. Values that are
	 * greater than a valid time, but less than 0xc0 shouldn't appear.
	 */
	p += sprintf(p,
		     "alarm_time\t: %02d:%02d:%02d\n"
		     "alarm_date\t: %04d-%02d-%02d\n",
		     tm.tm_hour, tm.tm_min, tm.tm_sec,
		     tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

	p += sprintf(p,
		     "BCD\t\t: %s\n"
		     "24hr\t\t: %s\n"
		     "yes",
		     "yes");

	return  p - buf;
#undef YN
#undef NY
}



static int RTC_ReadProc(char *page, char **start, off_t off,
                         int count, int *eof, void *data)
{
        int len = RTC_ProcOutput (page);
        if (len <= off+count) *eof = 1;
        *start = page + off;
        len -= off;
        if (len>count) len = count;
        if (len<0) len = 0;
        return len;
}

/*
 * Returns true if a clock update is in progress
 */
/* FIXME shouldn't this be above rtc_init to make it fully inlined? */
static inline unsigned char RTC_IsUpdating(void)
{
	unsigned char uip;
#if 0
	spin_lock_irq(&rtc_lock);
	uip = (CMOS_READ(OMAP_RTC_STATUS_REG) & OMAP_RTC_STATUS_BUSY); 
	spin_unlock_irq(&rtc_lock);
#endif
	return uip;
}

/****************************************************************************/
/* NAME:           SPEAR_RTC_GetAlmTime                                        */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine returns the alm time of RTC after reading the RTC      */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*                                                                          */
/* OUT :   rtc_time *                                                        */
/*                                                                          */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static void SPEAR_RTC_GetAlmTime(struct rtc_time *alm_tm)
{
	//unsigned char ctrl;
	unsigned int time;
	unsigned int date;

	spin_lock_irq(&rtc_lock);
	RTC_GetAlarmTime(&time);
	RTC_GetAlarmDate(&date );
	spin_unlock_irq(&rtc_lock);
	alm_tm->tm_sec =  (time & (SECONDS_UNITS | SECONDS_TENS)) >> SECONDS_SHIFT;
	alm_tm->tm_min = (time & (MINUTES_UNITS | MINUTES_TENS)) >> MINUTES_SHIFT;
	alm_tm->tm_hour = (time & (HOURS_UNITS | HOURS_TENS)) >> HOURS_SHIFT ;
	alm_tm->tm_mday = (date & (DAYS_UNITS | DAYS_TENS)) >> DAYS_SHIFT;
	alm_tm->tm_mon = (date & (MONTHS_UNITS | MONTHS_TENS)) >> MONTHS_SHIFT;
	alm_tm->tm_year = (date & (YEARS_UNITS | YEARS_TENS |YEARS_HUNDREDS |YEARS_MILLENIUMS)) >> YEARS_SHIFT;



	BCD_TO_BIN(alm_tm->tm_sec);
	BCD_TO_BIN(alm_tm->tm_min);
	BCD_TO_BIN(alm_tm->tm_hour);
	BCD_TO_BIN(alm_tm->tm_mday);
	BCD_TO_BIN(alm_tm->tm_mon);
	BCD_TO_BIN(alm_tm->tm_year);

	/*
	 * Account for differences between how the RTC uses the values
	 * and how they are defined in a struct rtc_time;
	 */
#if 0 //Arif: we have removed the check for 100 in setting the time
	if ((alm_tm->tm_year += (epoch - 1900)) <= 69)
		alm_tm->tm_year += 100;
#endif
	alm_tm->tm_mon--;
}

/****************************************************************************/
/* NAME:           SPEAR_RTC_GetTime                                        */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine returns the alm time of RTC after reading the RTC      */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*                                                                          */
/* OUT :   rtc_time *                                                        */
/*                                                                          */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static void SPEAR_RTC_GetTime(struct rtc_time *rtc_tm)
{
	unsigned int time,date;

	/* REVISIT: Fix this comment!!!
	 * read RTC once any update in progress is done. The update
	 * can take just over 2ms. We wait 10 to 20ms. There is no need to
	 * to poll-wait (up to 1s - eeccch) for the falling edge of OMAP_RTC_STATUS_BUSY.
	 * If you need to know *exactly* when a second has started, enable
	 * periodic update complete interrupts, (via ioctl) and then
	 * immediately read /dev/rtc which will block until you get the IRQ.
	 * Once the read clears, read the RTC time (again via ioctl). Easy.
	 */

#if	0 /* REVISIT: This need to do as the TRM says. */
	unsigned long uip_watchdog = jiffies;
	if (RTC_IsUpdating() != 0)
		while (jiffies - uip_watchdog < 2*HZ/100) {
			barrier();
			cpu_relax();
		}
#endif

	/*
	 * Only the values that we read from the RTC are set. We leave
	 * tm_wday, tm_yday and tm_isdst untouched. Even though the
	 * RTC has RTC_DAY_OF_WEEK, we ignore it, as it is only updated
	 * by the RTC when initially set to a non-zero value.
	 */
	spin_lock_irq(&rtc_lock);
	RTC_GetTime(&time);
	RTC_GetDate(&date);
	spin_unlock_irq(&rtc_lock);
	rtc_tm->tm_sec =  (time & (SECONDS_UNITS | SECONDS_TENS)) >> SECONDS_SHIFT;
	rtc_tm->tm_min = (time & (MINUTES_UNITS | MINUTES_TENS)) >> MINUTES_SHIFT;
	rtc_tm->tm_hour = (time & (HOURS_UNITS | HOURS_TENS)) >> HOURS_SHIFT ;
	rtc_tm->tm_mday = (date & (DAYS_UNITS | DAYS_TENS)) >> DAYS_SHIFT;
	rtc_tm->tm_mon = (date & (MONTHS_UNITS | MONTHS_TENS)) >> MONTHS_SHIFT;
	rtc_tm->tm_year = (date & (YEARS_UNITS | YEARS_TENS | YEARS_HUNDREDS |YEARS_MILLENIUMS)) >> YEARS_SHIFT;


	BCD_TO_BIN(rtc_tm->tm_sec);
	BCD_TO_BIN(rtc_tm->tm_min);
	BCD_TO_BIN(rtc_tm->tm_hour);
	BCD_TO_BIN(rtc_tm->tm_mday);
	BCD_TO_BIN(rtc_tm->tm_mon);
	BCD_TO_BIN(rtc_tm->tm_year);

	/*
	 * Account for differences between how the RTC uses the values
	 * and how they are defined in a struct rtc_time;
	 */
#if 0 /*Arif: we have removed the check for 100*/
	if ((rtc_tm->tm_year += (epoch - 1900)) <= 69)
		rtc_tm->tm_year += 100;
#endif
	rtc_tm->tm_mon--; /*in omap*/
}


MODULE_AUTHOR("Arif Khan");
MODULE_LICENSE("GPL");

module_init(SPEAR_RTC_Init);
module_exit(SPEAR_RTC_Exit);
