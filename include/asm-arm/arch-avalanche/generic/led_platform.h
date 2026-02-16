/*
 *
 * led_platform.h
 * Description:
 * see below
 *
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2008-2014 Intel Corporation.

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

  Copyright(c) 2008-2014 Intel Corporation. All rights reserved.

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


/******************************************************************************    
 * FILE PURPOSE:     - LED Platform specific Header file                                      
 ******************************************************************************    
 * FILE NAME:     led_platform.h                                                        
 *                                                                                 
 * DESCRIPTION:   Linux specific implementation for OS abstracted function calls
 *                made by LED HAL module. This file has functions defined for
 *                Memory allocation calls, Mutex calls, String and Timer 
 *                operations.
 *                                                                 
 * REVISION HISTORY:                                                               
 * 11 Oct 03 - PSP TII                                                             
 *                                                                                 
 *******************************************************************************/  

                                                                                   
#ifndef __LED_PLATFORM__
#define __LED_PLATFORM__

#include <linux/slab.h>
#include <asm-arm/arch-avalanche/generic/pal.h>

#define os_strcmp            os_strcasecmp
#define os_strcpy            strcpy

/* #define LED_HAL_DEBUG */

#if defined(LED_HAL_DEBUG)
#define log_msg(...)              printk(__VA_ARGS__)
#else
#define log_msg(...)
#endif

/* String handling functions  not defined in asm/string.h */
static __inline__ int os_strlen(char *str)
{
    int i;
    for(i=0;str[i];i++);
    return i;
}


#define LOWER(x)    ((x < 'a') ? (x - 'A' + 'a'):(x))                                    
#define ISALPHA(x)   ((( x >= 'a') && (x <= 'z')) || (( x >= 'A') && (x <= 'Z')))        
#define COMP(x,y)   ((x == y) || ((ISALPHA(x) && ISALPHA(y)) && (LOWER(x) == LOWER(y)))) 

#define OS_TIMER_ADD( func, handle, delay, arg )  os_timer_add( handle, delay, arg )

static __inline__ int os_strcasecmp(char *str1, char *str2)
{                                         
    int i;                            
    
    for(i=0;str1[i] && str2[i];i++)
    {
        char x,y;
        
        x = str1[i];
        y = str2[i];

        if(!COMP(x,y))
            break;
    }              

    return(str1[i] || str2[i]);
}                                                                                  

/* Functions for timer related operations */
static __inline__ void * os_timer_init(void (*func)(int))
{
    struct timer_list *ptimer;
    ptimer = (struct timer_list *) kmalloc(sizeof(struct timer_list),GFP_KERNEL);
    init_timer( ptimer );
    ptimer->function = (void *)func;
    return (void *)ptimer;
}

static __inline__ int os_timer_add(void *timer_handle,int milisec,int arg)
{
    struct timer_list *ptimer=timer_handle;
    ptimer->expires = ((HZ * milisec)/1000) + jiffies;
    ptimer->data = arg;
    add_timer(ptimer);	
    return 0;
}

static __inline__ int os_timer_delete(void *timer_handle)
{
    struct timer_list *ptimer=timer_handle;
    del_timer(ptimer);
    return 0;
}

static __inline__ int os_timer_destroy(void *timer_handle)
{
    struct timer_list *ptimer=timer_handle;
    del_timer_sync(ptimer);
    kfree(ptimer);
    return 0;
}
#endif /* __LED_PLATFORM__ */

