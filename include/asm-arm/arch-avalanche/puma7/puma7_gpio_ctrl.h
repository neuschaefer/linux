
/*
 *
 * puma7_gpio_ctrl.h
 *
  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2014-2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify 
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE 

  Copyright(c) 2014-2015 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*------------------------------------------------------------------------------
* File Name: puma7_gpio_ctrl.h
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
#define GPIO_IN_BIT_CMD                        _IOWR(GPIO_MODULE_ID, 4, unsigned long)
#define GPIO_CTRL_IO_PAD                       _IOW(GPIO_MODULE_ID, 5, unsigned long)

#define GPIO_PROC_FILE   "/proc/gpio"
#define GPIO_PROC_NAME   "gpio"


struct gpio_user_info
{
    unsigned int  gpio_pin;
    unsigned int  pin_direction;
    unsigned int  value;

};

struct io_ctrl_user_info
{
    unsigned int  opcode;
    unsigned int  value1;
    unsigned int  value2;
};


typedef enum GPIO_PIN_DIRECTION_tag
{
    GPIO_OUTPUT_DIR = 0,
    GPIO_INPUT_DIR = 1
} GPIO_PIN_DIRECTION_T;

/*All PUMA7 platform uses the same GPIO representation*/
typedef enum
{
    PUMA7_GPIO_US_BAND_SEL = 0,
    PUMA7_GPIO_DS_BAND_SEL,
    PUMA7_GPIO_NUM2,
    PUMA7_GPIO_NUM3,
    PUMA7_GPIO_TUNER_RESET,
    PUMA7_GPIO_NUM5,
    PUMA7_GPIO_NUM6,
    PUMA7_GPIO_NUM7,
    PUMA7_GPIO_NUM8,
    PUMA7_GPIO_NUM9,
    PUMA7_GPIO_NUM10,
    PUMA7_GPIO_NUM11,
    PUMA7_GPIO_NUM12,
    PUMA7_GPIO_NUM13,
    PUMA7_GPIO_NUM14,
    PUMA7_GPIO_NUM15,
    PUMA7_GPIO_RGMII_CONNECTOR_RESET,
    PUMA7_GPIO_EXTSWITCH_POWER,
    PUMA7_GPIO_ETH_PHY_RESET,
    PUMA7_GPIO_EXTSWITCH_RESET,
    PUMA7_GPIO_LED_POWER,
    PUMA7_GPIO_LED_DS,
    PUMA7_GPIO_LED_US,
    PUMA7_GPIO_LED_ONLINE,
    PUMA7_GPIO_LED_LINK,
    PUMA7_GPIO_LED_LINE1,
    PUMA7_GPIO_LED_LINE2,
    PUMA7_GPIO_LED_DECT,
    PUMA7_GPIO_LED_BATTERY,
    PUMA7_GPIO_LED_MOCA,
    PUMA7_GPIO_LED_WIFI2_4G,
    PUMA7_GPIO_LED_WIFI5G,

    PUMA7_MAX_GPIOS

}GPIO_PIN_NUMBER_T;

#endif /* GPIO_API_H */


