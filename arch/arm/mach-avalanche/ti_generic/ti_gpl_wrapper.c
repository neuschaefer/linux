/*
 *
 * ti_gpl_wrapper.c 
 * Description:
 * wrapper for GPL APIs
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
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/string.h>


struct bus_type *platform_bus_type_ptr = &platform_bus_type;
EXPORT_SYMBOL(platform_bus_type_ptr);

int ti_driver_create_file(struct device_driver * drv, struct driver_attribute * attr)
{
    return driver_create_file(drv, attr);
}

void ti_driver_remove_file(struct device_driver * drv, struct driver_attribute * attr)
{
    return driver_remove_file(drv, attr);
}

int ti_driver_register(struct device_driver * drv)
{
    return driver_register(drv);
}

void ti_driver_unregister(struct device_driver * drv)
{
    return driver_unregister(drv);
}

int ti_platform_device_register(struct platform_device * pdev)
{
    return platform_device_register(pdev);
}

void ti_platform_device_unregister(struct platform_device * pdev)
{
    return platform_device_unregister(pdev);
}

struct platform_device *ti_platform_device_register_simple(char *name, unsigned int id,
							struct resource *res, unsigned int num)
{

    return platform_device_register_simple(name, id, res, num);
}

int fastcall ti_queue_work(struct workqueue_struct *wq, struct work_struct *work)
{
    return queue_work(wq, work);
}

void ti_destroy_workqueue(struct workqueue_struct *wq)
{
    return destroy_workqueue(wq);
}




struct workqueue_struct *__ti_create_workqueue(const char *name, int singlethread)
{
    if(singlethread)
       return create_singlethread_workqueue(name);  
    else
       return create_workqueue(name);    
}

EXPORT_SYMBOL(ti_driver_create_file);
EXPORT_SYMBOL(ti_driver_remove_file);
EXPORT_SYMBOL(ti_driver_register);
EXPORT_SYMBOL(ti_driver_unregister);
EXPORT_SYMBOL(ti_platform_device_register);
EXPORT_SYMBOL(ti_platform_device_unregister);
EXPORT_SYMBOL(ti_platform_device_register_simple);
EXPORT_SYMBOL(ti_queue_work);
EXPORT_SYMBOL(ti_destroy_workqueue);
EXPORT_SYMBOL(__ti_create_workqueue);

