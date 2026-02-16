/*
 *
 * iosfsb_api.h
 * Description:
 * iosfsb common type declarations
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2012 Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 * The file contains the main data structure and API definitions for Linux Hardware Mutex driver
 * Intel CE processor supports 4 masters and 12 mutexes avalible
 *
 */


/*------------------------------------------------------------------------------
* File Name: puma6_gpio_ctrl.h
*------------------------------------------------------------------------------
*/
//! \file
#ifndef  GPIO_API_H
#define  GPIO_API_H





/********************************************************************************************************/
/* IOCTL commands:

   If you are adding new ioctl's to the kernel, you should use the _IO
   macros defined in <linux/ioctl.h> _IO macros are used to create ioctl numbers:

    _IO(type, nr)         - an ioctl with no parameter.
   _IOW(type, nr, size)  - an ioctl with write parameters (copy_from_user), kernel would actually read data from user space
   _IOR(type, nr, size)  - an ioctl with read parameters (copy_to_user), kernel would actually write data to user space
   _IOWR(type, nr, size) - an ioctl with both write and read parameters

   'Write' and 'read' are from the user's point of view, just like the
    system calls 'write' and 'read'.  For example, a SET_FOO ioctl would
    be _IOW, although the kernel would actually read data from user space;
    a GET_FOO ioctl would be _IOR, although the kernel would actually write
    data to user space.

    The first argument to _IO, _IOW, _IOR, or _IOWR is an identifying letter
    or number from the SoC_ModuleIds_e enum located in this file.

    The second argument to _IO, _IOW, _IOR, or _IOWR is a sequence number
    to distinguish ioctls from each other.

   The third argument to _IOW, _IOR, or _IOWR is the type of the data going
   into the kernel or coming out of the kernel (e.g.  'int' or 'struct foo').

   NOTE!  Do NOT use sizeof(arg) as the third argument as this results in
   your ioctl thinking it passes an argument of type size_t.

*/

#define GPIO_INTERFACE_DRIVER_DEV_NAME "/dev/docsis_gpio_dev"
#define GPIO_MODULE_ID                         (0x02)
#define GPIO_SET_DIRECTION_CMD                 _IOW(GPIO_MODULE_ID, 1, unsigned long)
#define GPIO_GET_DIRECTION_CMD                 _IOWR(GPIO_MODULE_ID, 2, unsigned long)
#define GPIO_OUT_BIT_CMD                       _IOW(GPIO_MODULE_ID, 3, unsigned long)
#define GPIO_IN_BIT_CMD                        _IOWR(GPIO_MODULE_ID, 3, unsigned long)

#define GPIO_PROC_FILE   "/proc/gpio"
#define GPIO_PROC_NAME   "gpio"
#define PUMA6_MAX_GPIOS  (128)

struct gpio_user_info
{
    unsigned int  gpio_pin;
    unsigned int  pin_direction;
    unsigned int  value;
};


typedef enum GPIO_PIN_DIRECTION_tag
{
    GPIO_OUTPUT_DIR = 0,
    GPIO_INPUT_DIR = 1
} GPIO_PIN_DIRECTION_T;

#endif /* GPIO_API_H */


