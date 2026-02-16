/*
 *
 * aep_iosfsb.c
 * Description:
 * iosf-sb driver over AEP
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
#include "aep_iosfsb.h"

/* Structure that hold input message format for "IOSF SB Access" IPC message */
typedef struct _aep_iosfsb_access_input_t
{
    char port;
    char opcode;
    char res1;
    char res2;
    unsigned int address;    
    unsigned int write_data; 
} aep_iosfsb_access_input_t;

/* Structure that hold response message format for "IOSF SB Access" IPC message */
typedef struct _aep_iosfsb_access_output_t
{
  unsigned int status;
  unsigned int read_data; /* need only 2 bytes - see message spec */
} aep_iosfsb_access_output_t;


/* 
 * Get AEP F/W Status.
 * 
 * aep_ipc_iosfsb_is_active(void)
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
int aep_ipc_iosfsb_is_active(void)
{
    /* Check if AEP is enabled */
    return aep_is_active();
}
EXPORT_SYMBOL(aep_ipc_iosfsb_is_active);


/* 
 * Read from IOSF-SB port .
 * 
 * int aep_ipc_iosfsb_read(char port, char opcode, unsigned int addr, unsigned int* value)
 *
 * Description:
 *  This API is used to read IOSF-SB port when the AEP is active.
 *
 * Precondition:
 *  AEP must be active
 *
 * Parameters:
 *  port         - IOSF-SB port number to read from.
 *  opcode       - IOSF_SB opcode
 *  addr         - IOSF-SB address
 *  value        - [output], the read value.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
int aep_ipc_iosfsb_read(char port, char opcode, unsigned int addr, unsigned int* value)
{
    aep_iosfsb_access_input_t  msg_input;
    aep_iosfsb_access_output_t msg_output;
    
    /* initialize input payload message */
    msg_input.port       = port;
    msg_input.opcode     = opcode;
    msg_input.res1       = 0;
    msg_input.res2       = 0;
    msg_input.address    = cpu_to_le32(addr);
    msg_input.write_data = 0;

    msg_output.read_data = 0; 
    msg_output.status = 0; 

    // printk("AEP IOSF_SB: [debug] aep_ipc_iosfsb_read (port=%d, opcode=0x%.2X , addr=0x%.8X)\n",port,opcode,addr);

    /* Send AEP IPC Message */
    if (aep_ipc_send_message(AEP_IPC_CMD_ID_IOSF_SB_ACCESS,(int*)&msg_input,sizeof(aep_iosfsb_access_input_t),(int*)&msg_output,sizeof(aep_iosfsb_access_output_t)) != 0)
    {
        printk(KERN_ERR "AEP IOSF_SB:  Read from port=%d, opcode=0x%.2X , addr=0x%.8X, failed.\n",port,opcode,addr);
        return -1;
    }

    /* Check response status */
    if (le32_to_cpu(msg_output.status) != AEP_IPC_STATUS_OK) 
    {
        printk(KERN_ERR "AEP IPC IOSF-SB error, IPC response status = 0x%X\n",le32_to_cpu(msg_output.status));
        return -1;
    }

    /* update output response */
    if (value != NULL)
    {
        *value = le32_to_cpu(msg_output.read_data);
        // printk("AEP IOSF_SB: [debug] aep_ipc_iosfsb_read value=0x%.8X\n",*value);
    }

    return 0;
}
EXPORT_SYMBOL(aep_ipc_iosfsb_read);


/* 
 * Write to IOSF-SB port.
 * 
 * int aep_ipc_iosfsb_write(char port, char opcode, unsigned int addr, unsigned int value)
 *
 * Description:
 *  This API is used to write to a GPIO register when the AEP is active.
 *
 * Precondition:
 *  AEP must be active
 *
 * Parameters:
 *  port         - IOSF-SB port number to write to.
 *  opcode       - IOSF_SB opcode
 *  addr         - IOSF-SB address
 *  value        - The value to write.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
int aep_ipc_iosfsb_write(char port, char opcode, unsigned int addr, unsigned int value)
{
    aep_iosfsb_access_input_t  msg_input;
    aep_iosfsb_access_output_t msg_output;

    /* initialize input payload message */
    msg_input.port       = port; 
    msg_input.opcode     = opcode;
    msg_input.res1       = 0;
    msg_input.res2       = 0;
    msg_input.address    = cpu_to_le32(addr);
    msg_input.write_data = cpu_to_le32(value);

    // printk("AEP IOSF_SB: [debug] aep_ipc_iosfsb_write(port=%d, opcode=0x%.2X, addr=0x%.8X, value=0x%.8X)\n",port,opcode,addr,value);

    /* Send AEP IPC Message */
    if (aep_ipc_send_message(AEP_IPC_CMD_ID_IOSF_SB_ACCESS,(int*)&msg_input,sizeof(aep_iosfsb_access_input_t),(int*)&msg_output,sizeof(aep_iosfsb_access_output_t)) != 0)
    {
        printk(KERN_ERR"AEP IOSF_SB: Write to port=%d, opcode=0x%.2X, addr=0x%.8X, value=0x%.8X, failed.\n",port,opcode,addr,value);
        return -1;
    }

    /* Check response status */
    if (msg_output.status != AEP_IPC_STATUS_OK) 
    {
        printk(KERN_ERR "AEP IPC GPIO error, IPC response status = 0x%X\n",le32_to_cpu(msg_output.status));
        return -1;
    }

    // printk("AEP IOSF_SB: [debug] aep_ipc_iosfsb_write return OK,\n");
    return 0;
}
EXPORT_SYMBOL(aep_ipc_iosfsb_write);



