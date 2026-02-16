/*
 *
 * aep_gpio.h
 * Description:
 * 
 * AEP - GPIO IPC Kernel Driver
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2013 Intel Corporation. All rights reserved.
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
 * The file contains the main data structure and API definitions for Linux AEP - GPIO IPC Kernel Driver
 *
 */



#ifndef AEP_GPIO_H
#define AEP_GPIO_H


/********************/
/**   Driver API   **/
/********************/

/* 
 * Get AEP F/W Status.
 * 
 * aep_ipc_gpio_is_active(void)
 *
 * Description:
 *  This API is used to check if the AEP F/W is exist and running.
 *  On A0 or B0 parts there is no AEP IP. The API will return 0
 *  On C0 part and above the AEP can be active or not active, depend on the product design.  
 *
 * Precondition:
 *  This API can be call with out any precondition.
 *
 * Parameters:
 *  None.
 *
 * Return:
 *  0 on Not Active
 *  1 on Active.
 */
int aep_ipc_gpio_is_active(void);

/* 
 * Read from GPIO register .
 * 
 * aep_ipc_gpio_read(unsigned int reg_offset, unsigned int* value)
 *
 * Description:
 *  This API is used to read GPIO register when the AEP is active.
 *
 * Precondition:
 *  AEP must be active
 *
 * Parameters:
 *  reg_offset   - GPIO  register offset to read from.
 *  value        - [output], the read value.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
int aep_ipc_gpio_read(unsigned int reg_offset, unsigned int* value);

/* 
 * Write to GPIO register.
 * 
 * int aep_ipc_gpio_write(unsigned int reg_offset, unsigned int value, unsigned int mask)
 *
 * Description:
 *  This API is used to write to a GPIO register when the AEP is active.
 *
 * Precondition:
 *  AEP must be active
 *
 * Parameters:
 *  reg_offset   - GPIO  register offset to read from.
 *  value        - The value to write.
 *  mask         - Mask on written value.
 *
 * Return:
 *  0 on Success, <0 on error.
 */

int aep_ipc_gpio_write(unsigned int reg_offset, unsigned int value, unsigned int mask);

#endif /* AEP_GPIO_H */
