/*
 *
 * aep.h
 * Description:
 * 
 * AEP - Kernel Driver
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



#ifndef AEP_H
#define AEP_H


/* Host to AEP commands ID */
#define AEP_IPC_CMD_ID_BIST                      0
#define AEP_IPC_CMD_ID_SEC_BOOT_NOTIFY           1
#define AEP_IPC_CMD_ID_GPIO_ACCESS               2
#define AEP_IPC_CMD_ID_PUINT_ACCESS              3
#define AEP_IPC_CMD_ID_AEP_FW_VERSION            4
#define AEP_IPC_CMD_ID_NP_ATOM_COMM              5
#define AEP_IPC_CMD_ID_SET_ATTRIB                6
/* #define AEP_IPC_CMD_ID_SET_EMMC_FLASH_PARTITION  7 */
#define AEP_IPC_CMD_ID_ARM_BOOT_COMLETE          8
#define AEP_IPC_CMD_ID_SET_GPIO_ACCESS           9
#define AEP_IPC_CMD_ID_IOSF_SB_ACCESS            10
#define AEP_IPC_CMD_ID_GET_PUNIT_FW_VERSION      11
#define AEP_IPC_CMD_ID_LAST                      12 

/* Return Code	Description */
#define AEP_IPC_STATUS_OK                   0x00000000      /* Command completed successfully */
#define AEP_IPC_STATUS_BAD_HOST_REQUEST     0x80000000	    /* The IPC input doorbell command was invalid */
#define AEP_IPC_STATUS_INVALID_MODE         0x80000001	    /* One of the modes in the IPC input payload was invalid */
#define AEP_IPC_STATUS_INVALID_ADDRESS      0x80000002	    /* One of the addresses in the IPC input payload was invalid */
#define AEP_IPC_STATUS_TASK_NOT_FOUND       0x80000003	    /* The NP2ATOM or the PUNIT return IPC mechanism failed to find a valid initial command */
	



#define AEP_IPC_CMD_ID_COMPLETE_FLAG             0x80000000

typedef unsigned int (*aep_recv_message_cb_t)(unsigned int cmd_id, char* message, char* response, int res_length, int* has_rsp);


/* 
 * Get AEP F/W Status.
 * 
 * int aep_is_active(void)
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
int aep_is_active(void);


/* 
 * Get AEP F/W Version.
 * 
 * int aep_is_active(void)
 *
 * Description:
 *  This API is used to read the AEP F/W version 
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  None.
 *
 * Return:
 *  AEP F/W version
 */
int aep_get_aep_fw_version(char *fw_ver_soc, char *fw_ver_stepping, char *fw_ver_major ,char *fw_ver_minor);


/* 
 * Clear MMC interrupt.
 * 
 * int aep_clear_interrupt(void)
 *
 * Description:
 *  Clear AEP interrupt, once the  driver get interrupt from the AEP, it must clear the interrupt register 
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  None.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
int aep_clear_mmc_interrupt(void);


/* 
 * Send Sync IPC Message
 * 
 * int aep_ipc_send_message(int cmd_id, int* message, int msg_length, int* response, int res_length)
 *
 * Description:
 *  This API is used to send sync AEP IPC message.
 *  The function wait for response message from the AEP before it return.
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  cmd_id      - AEP IPC Command ID to send.
 *  message     - AEP IPC Command Payload (must be 4 bytes aligned).
 *  msg_length  - AEP IPC Command Payload length
 *  response    - [output] pointer to AEP IPC Response Payload (must be 4 bytes aligned)
 *  res_length  - Max size of AEP IPC Response Payload buffer (expected size). 
 *
 * Return:
 *  0 on Success, <0 on error.
 */
int aep_ipc_send_message(unsigned int cmd_id, int* message, int msg_length, int* response, int res_length);


/* 
 * Register Receive Callback function
 * 
 * int aep_ipc_register_event_recv(unsigned int cmd_id,aep_recv_message_cb_t cb)
 *
 * Description:
 *  This API is used to register callback function for specific IPC command ID.
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  cmd_id           - AEP IPC Command ID to register.
 *  cmd_message_size - AEP IPC Command expected message size.
 *  cb               - Pointer to the call back function.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
int aep_ipc_register_event_recv(unsigned int cmd_id, unsigned int cmd_message_size, aep_recv_message_cb_t cb);


/* 
 * Send IPC response message
 * 
 * int aep_ipc_send_response(int cmd_id, int* response, int res_length)
 *
 * Description:
 *  This API is used to send a response message, after the AEP F/W call the callback function, 
 *  with a new AEP command message.
 *
 * Precondition:
 *  This API can be only if the AEP is active.
 *
 * Parameters:
 *  cmd_id      - AEP IPC Command ID to response.
 *  response    - AEP IPC Command Payload to response.
 *  res_length  - Playload length in bytes.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
int aep_ipc_send_response(unsigned int cmd_id, int* response, int res_length);



#endif /* AEP_H */
