/*
 *
 * led_manager.h 
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
 * FILE PURPOSE:     - LED manager Header                                       
 ******************************************************************************    
 * FILE NAME:     led_manager.h                                                        
 *                                                                                 
 * DESCRIPTION:  Header file for LED manager                                        
 *                                                                                 
   
******************************************************************************/    
                                                                                   
#ifndef __LED_MANAGER__
#define __LED_MANAGER__


#include <led_hal.h>


struct led_manager_user_module		                
{
	unsigned char *name;			
	unsigned int instance;	    	
	unsigned int handle;	
};

struct led_manager_user_action
{
	unsigned int handle;						 
	unsigned int state_id;						
};

#define LED_MANAGER_MAGIC			0xD1

#define LED_MANAGER_CONFIG              _IOW(LED_MANAGER_MAGIC, 1, MOD_CFG_T)
#define LED_MANAGER_REGISTER	        _IOWR(LED_MANAGER_MAGIC, 2, struct led_manager_user_module)
#define LED_MANAGER_UNREGISTER	        _IOW(LED_MANAGER_MAGIC, 3, void *)
#define LED_MANAGER_ACTION		_IOW(LED_MANAGER_MAGIC, 4, struct led_manager_user_action)

typedef struct led_manager_user_module LED_MODULE_T;
typedef struct led_manager_user_action LED_STATE_T;

#define LED_CONFIG			LED_MANAGER_CONFIG 		
#define LED_GET_HANDLE			LED_MANAGER_REGISTER 	
#define LED_ACTION			LED_MANAGER_ACTION 		
#define LED_RELEASE_HANDLE		LED_MANAGER_UNREGISTER

#define avalanche_led_register		led_manager_register_module
#define avalanche_led_action		led_manager_led_action
#define avalanche_led_unregister	led_manager_unregister_module


MOD_OBJ_HND *led_manager_register_module(char *module_name, int instance );
void led_manager_unregister_module( void *module_handle );
LED_OBJ_HND * led_manager_install_callbacks(LED_FUNCS_T * funcs);
int led_manager_led_action( void *module_handle, int state_id );
int led_manager_uninstall_callbacks( void *module_handle);
int led_manager_cfg_mod( MOD_CFG_T *mod_cfg);

#endif
