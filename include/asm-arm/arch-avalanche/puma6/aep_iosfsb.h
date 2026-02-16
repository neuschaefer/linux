
/*
 *
 * aep_iosfsb.h
 * Description:
 * 
 * AEP - IOSF-SB IPC Kernel Driver
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
 * The file contains the main data structure and API definitions for Linux AEP - P-UNIT IPC Kernel Driver
 *
 */



#ifndef AEP_IOSFSB_H
#define AEP_IOSFSB_H

/********************/
/**   Driver API   **/
/********************/

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
int aep_ipc_iosfsb_is_active(void);

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
 *  opcode       - IOSF_SB opcode.
 *  addr         - IOSF-SB address
 *  value        - [output], the read value.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
int aep_ipc_iosfsb_read(char port, char opcode, unsigned int addr, unsigned int* value);


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
int aep_ipc_iosfsb_write(char port, char opcode, unsigned int addr, unsigned int value);

#endif /* AEP_IOSFSB_H */

