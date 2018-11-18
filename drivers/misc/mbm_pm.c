/* 2012-04-12: File added and changed by Sony Corporation */
/*
 * drivers/misc/mbm_pm.c
 *
 * Copyright (c) 2008-2010, NVIDIA Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/suspend.h>
#include <linux/percpu.h>

#define DRIVER_NAME "mbm_pm"
#undef VERBOSE_DEBUG

#define dbg(STR, ARGS...)                                  \
        do {                                               \
            printk("%s: " STR "\n" , DRIVER_NAME, ##ARGS); \
        } while (0)                                        

#ifdef VERBOSE_DEBUG
#define vdbg(STR, ARGS...)                                            \
        do {                                                          \
            printk(KERN_DEBUG "%s: " STR "\n" , DRIVER_NAME, ##ARGS); \
        } while (0)                                                   
#else
#define vdbg(STR, ARGS...) {}
#endif

pid_t s_mbm_pid = 0;

#if defined(CONFIG_PM)
//
// /sys/power/gps/notifier
//

wait_queue_head_t mbm_ack_wait;
wait_queue_head_t mbm_pm_notifier_wait;

int continue_ok;

struct kobject *mbm_kobj;

const char* sys_mbm_notifier;

static const char *STRING_PM_SUSPEND_PREPARE = "PM_SUSPEND_PREPARE";
static const char *STRING_PM_POST_SUSPEND    = "PM_POST_SUSPEND";
static const char *STRING_PM_CONTINUE        = "PM_CONTINUE";
static const char *STRING_PM_SIGNAL          = "PM_SIGNAL";


static void notify_mbm(const char* notice);

// Reading blocks if the value is not available.
static ssize_t
mbm_pm_notifier_show(struct kobject *kobj, struct kobj_attribute *attr,
                   char *buf)
{
    int nchar;

    // Block if the value is not available yet.
    if (! sys_mbm_notifier)
    {
        vdbg("blocking");
        if( wait_event_interruptible(mbm_pm_notifier_wait, sys_mbm_notifier )) {
            return -ERESTARTSYS;
        }
    }

    // In case of false wakeup, return "".
    if (! sys_mbm_notifier)
    {
        dbg("false wakeup, returning with '\\n'");
        nchar = sprintf(buf, "\n");
        return nchar;
    }

    // Return the value, and clear.
    dbg("returning with '%s'", sys_mbm_notifier);
    nchar = sprintf(buf, "%s\n", sys_mbm_notifier);
    sys_mbm_notifier = NULL;
    return nchar;
}

// Writing is no blocking.
static ssize_t
mbm_pm_notifier_store(struct kobject *kobj, struct kobj_attribute *attr,
                    const char *buf, size_t count)
{
    if (!strncmp(buf, STRING_PM_CONTINUE, strlen(STRING_PM_CONTINUE))) {
        // Wake up pm_notifier.
        dbg("PM_CONTINUE");
        continue_ok = 1;
        wake_up(&mbm_ack_wait);
    }
    else if (!strncmp(buf, STRING_PM_SIGNAL, strlen(STRING_PM_SIGNAL))) {
        s_mbm_pid = 0;
        sscanf(buf, "%*s %d", &s_mbm_pid);
        dbg("GPS HAL pid=%d", s_mbm_pid);
    }
    else {
        dbg("wrong value '%s'", buf);
    }

    return count;
}


static struct kobj_attribute mbm_pm_notifier_attribute =
       __ATTR(notifier, 0400, mbm_pm_notifier_show, mbm_pm_notifier_store);


//
// PM notifier
//

static void notify_mbm(const char* notice)
{
    long timeout = HZ * 30;

    // In case daemon's pid is not reported, do not signal or wait.
    if (!s_mbm_pid) {
        dbg("don't know GPS HAL pid");
        return;
    }

    continue_ok = 0;

    // Notify GPS HAL.
    sys_mbm_notifier = notice;
    wake_up(&mbm_pm_notifier_wait);

    // Wait for the reply from GPS HAL.
    vdbg("wait for GPS HAL");
    if (wait_event_timeout(mbm_ack_wait,
                   continue_ok, timeout) == 0) {
        dbg("timed out. GPS HAL did not reply");
    }

    // Go back to the initial state.
    sys_mbm_notifier = NULL;
}

int mbm_pm_notifier(struct notifier_block *nb,
                      unsigned long event, void *nouse)
{
    vdbg("start processing event=%lx", event);

    // Notify the event to GPS HAL.
    switch (event) {
    case PM_SUSPEND_PREPARE:
        notify_mbm(STRING_PM_SUSPEND_PREPARE);
        break;
    case PM_POST_SUSPEND:
        notify_mbm(STRING_PM_POST_SUSPEND);
        break;
    default:
        dbg("unknown event %ld", event);
        return NOTIFY_DONE;
    }

    vdbg("finished processing event=%ld", event);
    return NOTIFY_OK;
}
#endif


static int __init mbm_pm_init(void)
{
    int ret = 0;
    dbg("init");

    #if defined(CONFIG_PM)
    // Create /sys/power/gps/notifier.
    mbm_kobj = kobject_create_and_add("gps", power_kobj);
    ret = sysfs_create_file(mbm_kobj, &mbm_pm_notifier_attribute.attr);
    if(ret) {
        dbg("entry with the given name already exists");
        return ret;
    }

    init_waitqueue_head(&mbm_pm_notifier_wait);
    init_waitqueue_head(&mbm_ack_wait);

    sys_mbm_notifier = NULL;
    continue_ok = 0;

    // Register PM notifier.
    pm_notifier(mbm_pm_notifier, 1);
    #endif

    return ret;
}

static void __exit mbm_pm_deinit(void)
{
    dbg("deinit");
}

module_init(mbm_pm_init);
module_exit(mbm_pm_deinit);
