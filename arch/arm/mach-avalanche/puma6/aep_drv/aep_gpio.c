/*
 *
 * aep_gpio.c
 * Description:
 * aep driver
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
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <aep.h>
#include "aep_gpio.h"

#define AEP_IPC_GPIO_WRITE    0
#define AEP_IPC_GPIO_READ     1

/* Structure that hold input message format for "GPIO access" IPC message */
typedef struct _aep_gpio_access_input_t
{
  unsigned int offset;
  unsigned int is_read;
  unsigned int write_data;
  unsigned int write_mask;
} aep_gpio_access_input_t;

/* Structure that hold response message format for "GPIO access" IPC message */
typedef struct _aep_gpio_access_output_t
{
  unsigned int status;
  unsigned int read_data;
} aep_gpio_access_output_t;


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
int aep_ipc_gpio_is_active()
{
    /* Check if AEP is enabled */
    return aep_is_active();
}
EXPORT_SYMBOL(aep_ipc_gpio_is_active);


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
int aep_ipc_gpio_read(unsigned int reg_offset, unsigned int* value)
{
    aep_gpio_access_input_t  msg_input;
    aep_gpio_access_output_t msg_output;

    // printk("AEP IPC GPIO: [debug] aep_ipc_gpio_read (reg_offset=0x%.8X)\n",reg_offset);

    /* initialize input payload message */
    msg_input.offset     = cpu_to_le32(reg_offset);
    msg_input.is_read    = cpu_to_le32(AEP_IPC_GPIO_READ);
    msg_input.write_data = 0;
    msg_input.write_mask = 0;

    /* Send AEP IPC Message */
    if (aep_ipc_send_message(AEP_IPC_CMD_ID_GPIO_ACCESS,(int*)&msg_input,sizeof(aep_gpio_access_input_t),(int*)&msg_output,sizeof(aep_gpio_access_output_t)) != 0)
    {
        printk(KERN_ERR "AEP IPC GPIO error, Read from offset 0x%.8X failed.\n",reg_offset);
        return -1;
    }

    /* Check response status */
    if (msg_output.status != 0) 
    {
        printk(KERN_ERR "AEP IPC GPIO error, IPC response status = 0x%X\n",le32_to_cpu(msg_output.status));
        return -1;
    }

    /* update output response */
    if (value != NULL)
    {
        *value = msg_output.read_data ;
        // printk("AEP IPC GPIO: [debug] aep_ipc_gpio_read return OK (value=0x%.8X),\n",*value);
    }
    return 0;
}
EXPORT_SYMBOL(aep_ipc_gpio_read);


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
int aep_ipc_gpio_write(unsigned int reg_offset, unsigned int value, unsigned int mask)
{
    aep_gpio_access_input_t  msg_input;
    aep_gpio_access_output_t msg_output;

    // printk("AEP IPC GPIO: [debug] aep_ipc_gpio_write (reg_offset=0x%.8X, value=0x%.8X, mask=0x%.8X)\n",reg_offset,value,mask);

    /* initialize input payload message */
    msg_input.offset     = cpu_to_le32(reg_offset);
    msg_input.is_read    = cpu_to_le32(AEP_IPC_GPIO_WRITE);
    msg_input.write_data = value;
    msg_input.write_mask = mask;

    /* Send AEP IPC Message */
    if (aep_ipc_send_message(AEP_IPC_CMD_ID_GPIO_ACCESS,(int*)&msg_input,sizeof(aep_gpio_access_input_t),(int*)&msg_output,sizeof(aep_gpio_access_output_t)) != 0)
    {
        printk(KERN_ERR "AEP IPC GPIO error, Write to offset 0x%.8X value=0x%.8X ,mask=0x%.8X failed.\n",reg_offset,value,mask);
        return -1;
    }

    /* Check response status */
    if (le32_to_cpu(msg_output.status) != AEP_IPC_STATUS_OK) 
    {
        printk(KERN_ERR "AEP IPC GPIO: error, IPC response status = 0x%X\n",le32_to_cpu(msg_output.status));
        return -1;
    }

    // printk("AEP IPC GPIO: [debug] aep_ipc_gpio_write return OK,\n");
    return 0;
}
EXPORT_SYMBOL(aep_ipc_gpio_write);



