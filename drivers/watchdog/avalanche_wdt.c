/*
 *
 * avalanche_wdt.c
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


/*
 * linux/drivers/char/watchdog/avalanche_wdt.c
 *
 * Watchdog driver for the Avalanche WDT
 *
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/ioctl.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/bitops.h>
#include <linux/moduleparam.h>
#include <asm-arm/arch-avalanche/generic/pal.h>

static int timer_margin = AVALANCHE_WDT_MARGIN_DEF_VAL;    /* in seconds */
static unsigned long avalanche_wdt_users;
static struct miscdevice avalanche_wdt_miscdev;    /* Forward declaration */
static int wdt_init = 0;


/* Set timeout period and restart the timer */
static int avalanche_wdt_set_timeout(int timeout)
{
    int ret = 0;

    /* disable watchdog timer */
    if((ret = PAL_sysWdtimerCtrl(AVALANCHE_WDT_DISABLE_VALUE)))
        goto wdt_set_err;

    if (timeout == -1)
    {
        /* If timeout is set to -1, then wdt is left disable */
        printk(KERN_INFO "%s: Disabled.\n",avalanche_wdt_miscdev.name);

        timer_margin = -1; /* set timer_margin as a flag to 'disabled' */
        return 0;
    }

    /* check valid range */
    if ((timeout < AVALANCHE_WDT_MARGIN_MIN_VAL) ||
        (timeout > AVALANCHE_WDT_MARGIN_MAX_VAL))  
    {

        timeout = AVALANCHE_WDT_MARGIN_DEF_VAL;
    }

    /* set timer (in msecs) */
    if((ret = PAL_sysWdtimerSetPeriod(timeout * 1000)))
        goto wdt_set_err;

    timer_margin = timeout;
    /* enable watchdog timer */
    if((ret = PAL_sysWdtimerCtrl(AVALANCHE_WDT_ENABLE_VALUE)))
        goto wdt_set_err;

    /* kick is required to reload the new timeout margin */
    ret = PAL_sysWdtimerKick();

    printk(KERN_INFO "%s: Enabled, timer margin set to %d seconds.\n",avalanche_wdt_miscdev.name,timer_margin);

wdt_set_err:
    if(ret != 0)
        return -EIO;
    else
        return 0;
}



/*
 *    Allow only one person to hold it open
 */
static int avalanche_wdt_open(struct inode *inode, struct file *file)
{
    int ret=0;
    if (test_and_set_bit(1, &avalanche_wdt_users))
        return -EBUSY;

    if((ret = avalanche_wdt_set_timeout(timer_margin)))
        return ret;
#if 0
    /* reload timeout margin */
    PAL_sysWdtimerKick();

    /* open enables wdt */
    if(PAL_sysWdtimerCtrl(AVALANCHE_WDT_ENABLE_VALUE)) {
        test_and_clear_bit(1, &avalanche_wdt_users);
        return -EBUSY;
    }

    /* reload timeout margin */
    PAL_sysWdtimerKick();
#endif
    return 0;
}

static int avalanche_wdt_release(struct inode *inode, struct file *file)
{
    test_and_clear_bit(1, &avalanche_wdt_users);
    /* we must disable watchdog if WATCHDOG_NOWAYOUT was not defined (as per linux framework) */
    #ifndef CONFIG_WATCHDOG_NOWAYOUT
    return PAL_sysWdtimerCtrl(AVALANCHE_WDT_DISABLE_VALUE);
    #else
    return 0;
    #endif
}

static loff_t avalanche_wdt_llseek(struct file *file, loff_t offset, int whence)
{
    return -ESPIPE;        /* Not seekable */
}

static ssize_t
avalanche_wdt_write(struct file *file, const char *data, size_t len,
          loff_t * ppos)
{
    if (timer_margin == -1) 
    {
        /* in case the wdt is not enable, then do nothing */
        return 1;
    }

    /* Refresh LOAD_TIME. */
    if (len) {
        PAL_sysWdtimerKick();
        return 1;
    }
    return 0;
}



static int
avalanche_wdt_unlocked_ioctl(struct file *file,
          unsigned int cmd, unsigned long arg)
{
    int timeout = 0;
    
    static struct watchdog_info ident = {
        .identity = AVALANCHE_WDT_NAME,
        .options = WDIOF_CARDRESET,
        .firmware_version = 0,
    };

    switch (cmd) {
    case WDIOC_GETSUPPORT:
        return copy_to_user((struct watchdog_info *)arg, &ident,
                    sizeof(ident));
    case WDIOC_GETSTATUS:
        return put_user(0, (int *)arg);

    case WDIOC_KEEPALIVE:
        PAL_sysWdtimerKick();
        return 0;

    case WDIOC_SETTIMEOUT:
        if (copy_from_user(&timeout, (void __user *)arg, sizeof(int)))
        {
            return -EFAULT;
        }
        if(avalanche_wdt_set_timeout(timeout))
            goto wdt_ioctl_err;
        return 0;

    case WDIOC_GETTIMEOUT:
        return put_user(timer_margin, (int *)arg);

    default:
        return -ENOIOCTLCMD;
    }

wdt_ioctl_err:
    return -EIO;
}

static struct file_operations avalanche_wdt_fops = {
    .llseek    = avalanche_wdt_llseek,
    .write    = avalanche_wdt_write,
    .unlocked_ioctl    = avalanche_wdt_unlocked_ioctl,
    .open    = avalanche_wdt_open,
    .release = avalanche_wdt_release,
};

static struct miscdevice avalanche_wdt_miscdev = {
    .minor = WATCHDOG_MINOR,
    .name = "watchdog",
    .fops = &avalanche_wdt_fops
};

static int __init avalanche_wdt_init(void)
{
    int ret = 0;

    if((ret = misc_register(&avalanche_wdt_miscdev)))
        goto wdt_err;

    /* get the Watchdog timer module out of reset */
    PAL_sysResetCtrl(AVALANCHE_WDT_RESET, OUT_OF_RESET);

    /* Initialize watchdog timer */
    PAL_sysWdtimerInit(AVALANCHE_WATCHDOG_TIMER_BASE,PAL_sysClkcGetFreq(PAL_SYS_CLKC_WDT));

    printk(KERN_INFO "%s: TI Avalanche Watchdog Timer.\n",avalanche_wdt_miscdev.name);
    
    /* set timeout and start */
    if((ret = avalanche_wdt_set_timeout(timer_margin)))
        goto wdt_err;

    wdt_init = 1;

    return ret;

wdt_err:
    printk(KERN_INFO "%s: TI Avalanche Watchdog Timer: Initialization Failed \
                            (timer margin %d sec, err = %d)\n",
          avalanche_wdt_miscdev.name, timer_margin, ret);

    return ret;
}

static void __exit avalanche_wdt_exit(void)
{
    misc_deregister(&avalanche_wdt_miscdev);
    /* we must disable watchdog if WATCHDOG_NOWAYOUT was not defined (as per linux framework) */
    #ifndef CONFIG_WATCHDOG_NOWAYOUT
    PAL_sysWdtimerCtrl(AVALANCHE_WDT_DISABLE_VALUE);
    //PAL_sysResetCtrl(PSC_WDT_ARM, IN_RESET);
    #endif
}


static int set_param_str(const char *val, const struct kernel_param *kp)
{
    /* Read the new value */
    sscanf(val,"%d",&timer_margin);

    /* The function might be called before avalanche_wdt_init(), so in that case we just save the value */
    if (wdt_init != 0)
    {
        avalanche_wdt_set_timeout(timer_margin);
    }

    return 0;
}

static int get_param_str(char *buffer, const struct kernel_param *kp)
{
    if (timer_margin == -1) 
    {
        printk("watchdog: Disabled.\n");
    }
    else
    {
        printk("watchdog: time_mergin = %d\n", timer_margin);
    }

    return 0;
}

static struct kernel_param_ops param_ops_str = {
    .set = set_param_str,
    .get = get_param_str,
};



module_init(avalanche_wdt_init);
module_exit(avalanche_wdt_exit);

MODULE_AUTHOR("Texas Instruments");

/* 
Usage example:
   To read time margin:      # cat /sys/module/avalanche_wdt/parameters/timer_margin
   To set new time margin    # echo 30 > /sys/module/avalanche_wdt/parameters/timer_margin
   To disable watchdog       # echo -1 > /sys/module/avalanche_wdt/parameters/timer_margin
   To disable watch do from Linux command line, add 'avalanche_wdt.timer_margin=-1' 
*/
module_param_cb(timer_margin, &param_ops_str, NULL, 0644);



