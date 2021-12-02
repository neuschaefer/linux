/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: smc_apdu.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file smc_apdu.c
 *  smc_apdu.c is the implementation of APDU layer.
 */

#include "x_os.h"
#include "x_assert.h"
#include "smc_debug.h"
#include "smc_drvif.h"
#include "smc_apdu.h"
#include "smc_hp.h"
#include "smc_if.h"

static UINT8               ns=0;

static UINT8 _getlrc (UINT8 *pbData, UINT16 iCount)
{
    //TRACE_ENTRY ();
    UINT8      chLRC = 0;
    UINT8      i = 0;

    for (i=0; i < iCount; ++i) {
        chLRC ^= pbData[i];
    }

    //TRACE_EXIT ();
    return chLRC;
}

EXTERN void _SMC_ResetNs(void)
{
    ns = 0;
}

EXTERN INT32 _SMC_SendAPDU (UINT8 u1SlotNbr, SMC_CMD_SEND_T rCommand, UINT8 *pu1SendData, UINT8 u1SendLength)

{
    //TRACE_ENTRY ();
    static UINT8              block_data[MAX_RESPONSE_LENGTH];    
    INT32      ret_val = SMCR_OK;
    UINT8              ret1;
    UINT32             block_data_length= 0;
    UINT8              pcb;
    INT32    osal_ret;

    if (_SMC_GetStatus() != SMC_CARD_ACTIVATED) 
    {
        ret_val = SMCR_CARD_ERROR;
        goto ret;
    } 
    if(u1SendLength >= 255)
    {
        ret_val = SMCR_INV_ARG;
        goto ret;        
    }
    switch (rCommand.block_type) {

    case    INFO_BLOCK:
        /*
        ** inside the info block ins code is the instruction to be passed
        ** filling the PCB with the send sequence number
        */
        pcb = rCommand.ns << 6;
        
        // block_data[2:0]: Prologue field 
        block_data[0] = NAD;
        block_data[1] = pcb;
        block_data[2] = u1SendLength;

        if (pu1SendData != NULL) 
        {
            // block_data[(u1SendLength + 2):3]: Information field             	
            x_memcpy (&block_data[3], (UINT8 *) pu1SendData, u1SendLength);
        }
        block_data_length = u1SendLength + 3;

        /* ---- Filling the last byte with the LRC of the complete block ---- */
        // block_data[u1SendLength + 3]: Epilogue field        
        block_data[block_data_length] = _getlrc(block_data,block_data_length);
        block_data_length++;
        break;

    case    RECEIVE_BLOCK:
        pcb = REC_BLOCK;
        /* ------- Filling PCB with send sequence number of receive block --- */
        pcb |= (rCommand.ns << 4);
        block_data[0] = NAD;
        block_data[1] = pcb;
        /* ---------- In the receive ready block the data part is 0 --------- */
        block_data[2] = u1SendLength;
        block_data[3] = _getlrc(block_data,3);
        block_data_length = 4;
        break;

    case    SUPER_BLOCK:
        /* In the super block ins code contains the request or response
           information */
        pcb  = SUP_BLOCK;
        /* ---- Filling the control infomation for the supervisory block ---- */
        pcb |= rCommand.ins_code;
        block_data[0] = NAD;
        block_data[1] = pcb;
        block_data[2] = u1SendLength;
        if (pu1SendData == NULL) {
            SMC_INFO_PRINT ("send data null\n");
            block_data[3] = _getlrc(block_data, 3);
            block_data_length = 4;
        }
        else {
            /* -------------- control data is set here ---------------------- */
            SMC_INFO_PRINT ("superdata: %x\n", *pu1SendData);
            block_data[3] = *pu1SendData;
            block_data[4] = _getlrc(block_data,4);
            block_data_length = 5;
        }
        break;

    default:
        /* ------------------ Invalid data for the block formation ---------- */
        ret_val = SMCR_INV_ARG;
        goto ret;
    }

    /*
    osal_ret = OSAL_sem_take (smc_hal_sem_id, 500);
    */
    
    /* -------------- Sending the block to the smartcard -------------------- */
    osal_ret = _SMC_SendBlock(block_data,
               block_data_length,
               &ret1);
    if (osal_ret != 0) {
        /* ------------ Error in sending data to smartcard ------------------ */
        ret_val = SMCR_CARD_WRITE_ERROR;
        goto ret;
    }
    ret_val = SMCR_OK;
    
ret:
    //TRACE_EXIT ();
    return ret_val;
}

INT32 _SMC_ReceiveAPDU  (UINT8 u1SlotNbr, SMC_CMD_SEND_T *prCmd, UINT8 *pu1ReceiveData, UINT8 *pu1ReceiveLength)
{
    //TRACE_ENTRY ();
    static UINT8              data[MAX_RESPONSE_LENGTH];  
    INT32      ret_val = SMCR_OK;
    UINT8              ret1=0;
    UINT8 lrc;
    UINT32             data_read_len = 0; //, maxlen = 258;    /*Max for smart card*/
    INT32    osal_ret;
    UNUSED(osal_ret);

    if (_SMC_GetStatus() != SMC_CARD_ACTIVATED) 
    {
        ret_val = SMCR_CARD_ERROR;
        goto ret;
    } 
    
    /*
    osal_ret = OSAL_sem_take (smc_hal_sem_id, 100);
    if (osal_ret == OSAL_NO_ERROR) {
        OSAL_task_delay(50);
    }
    */
    
    x_memset (data, 0, MAX_RESPONSE_LENGTH);
    osal_ret = _SMC_ReceiveBlock(data,  &data_read_len, &ret1);
    //osal_ret = OSAL_sem_give (smc_hal_sem_id);
    /* ------------------- If the read is not succesful --------------------- */
    /* ----- If the no of bytes read is less than 4 it indicates an error --- */
    if (data_read_len < 4) {
        /* error in the read from smartcard */
        ret_val = SMCR_CARD_READ_ERROR;
        goto ret;
    }

    /* If the response is contains information block PCB starts with 0 as msb */
    if ( ! (data[1] & INF_MASK) ) {
        prCmd->block_type =  INFO_BLOCK;
        /*
        ** For the info block filling the ns with
        ** the ns obatined from the response.
        */
        prCmd->ns = ( (data[1] & 0x40) >>6 );
    }

    /* If the response contains receive block PCB starts with 10 in the msb */
    else if (!(data[1] & REC_MASK)) {
        prCmd->block_type =  RECEIVE_BLOCK;
        /*
        ** For the receive block filling the ns with
        ** the value of nr obtained frm response.
        */
        prCmd->ns = ((data[1] & 0x10)>>4);
    }

    /*
    ** If the response contains supervisory
    ** block PCB starts with 11 in the msb
    */
    else {
        prCmd->block_type =  SUPER_BLOCK;
    }

    /* -- Filling up the ins code with the PCB obtained from the response --- */
    prCmd->ins_code = data[1];

    /*
    ** If the response contains data part also
    ** it need to be filled in the pu1ReceiveData
    */

    *pu1ReceiveLength = data_read_len - 4;
    if (data_read_len > 4) {
        x_memcpy(pu1ReceiveData, (UINT8 *) &data[3], *pu1ReceiveLength);
    }

    /* ------------------ Checking the LRC of the data ---------------------- */
    lrc = _getlrc (data, data_read_len - 1);
    if (lrc == data[data_read_len-1]) {
        ret_val = SMCR_OK;
        goto ret;
    }
    else {
        ret_val = SMCR_LRC_ERROR;
        goto ret;
    }

ret:
    if (ret_val != SMCR_OK) {
        *pu1ReceiveLength = 0;
        pu1ReceiveData = NULL;
    }

    {
    	 UINT32 i;
        SMC_INFO_PRINT("Receive APDU:\n");
        for(i = 0; i < *pu1ReceiveLength; i++)
        {
            SMC_INFO_PRINT("0x%X ", pu1ReceiveData[i]);
            if(i % 8 == 7)
            {
                SMC_INFO_PRINT("\n");    
            }            
        }
        SMC_INFO_PRINT("\n");            
    }
    
    //TRACE_EXIT ();
    return ret_val;
}

INT32 _SMC_SendReceiveAPDU (UINT8 slot_nbr, UINT8 *pu1SendData,
                                 UINT8 u1SendLength, UINT8 *pu1ResponseData, UINT8 *pu1ResponseLength)
{
    //TRACE_ENTRY ();
    static UINT8                  receive_data[MAX_RESPONSE_LENGTH];
    INT32       ret_val = SMCR_OK;
    SMC_CMD_SEND_T               command, cmd;
    UINT8 receive_length;
    UINT8                  counter =0;
    //INT32        osal_ret;

    //osal_ret = OSAL_sem_take (card_sem_id, -1);
    
    cmd.block_type  = INFO_BLOCK;   
    cmd.ns  = 0;        
    command.block_type  = INFO_BLOCK;
    // Let APP prepare whole APDU  
    command.ns          = ns;
    
    ret_val = _SMC_SendAPDU (slot_nbr, command, pu1SendData, u1SendLength);

    SMC_LOG_PRINT("I(%d, 0) -> \n", ns);
    
    ns = !ns;

    while (1) {
        ret_val = _SMC_ReceiveAPDU (slot_nbr, &cmd, receive_data,
                                                               &receive_length);
        if (counter > 4) 
        {
            ret_val = SMCR_FAIL;
            goto ret;
        }
        counter++;
        /* ---- cmd->ins_code contains the pcb obtained from the response --- */
        switch (cmd.block_type) 
        {
        case INFO_BLOCK:
            if (!ret_val) 
            {
                SMC_LOG_PRINT( "        <- I(%d, 0)\n", cmd.ns);            
                if (receive_length < 6) 
                {
                    /*
                    ** Error in command so sending back I
                    ** block with return code indicating that.
                    */
                    *pu1ResponseLength = receive_length;
                    pu1ResponseData = NULL;
                    // prResponse = NULL;
                    ret_val = SMCR_FAIL;
                    goto ret;
                }
                *pu1ResponseLength            = receive_length;
                x_memcpy(pu1ResponseData, receive_data, *pu1ResponseLength);
                ret_val = SMCR_OK;
                goto ret;
            }
            else 
            {
                SMC_LOG_PRINT( "x      <- I(%d, 0)\n", cmd.ns);                 
                SMC_LOG_PRINT( "R(%d)   -> x\n", cmd.ns);                    
                ns                  = !ns;
                command.block_type  = RECEIVE_BLOCK;
                //command.ins_code    = 0;
                command.ns          = ns;
                ret_val = _SMC_SendAPDU (slot_nbr, command, NULL, 0);
                ns = !ns;
            }
            break;

        case RECEIVE_BLOCK:
            if (!ret_val) {
                SMC_INFO_PRINT( "        <- R(%d): 0x%2x\n", cmd.ns, cmd.ins_code);             	                
                ns = !ns;           	                
                if (ns == cmd.ns) {
                    SMC_LOG_PRINT( "I(%d, 0) -> \n", ns);                   	
                    command.block_type  = INFO_BLOCK;
                    //command.ins_code    = pu1SendData[1];
                    command.ns          = ns;
                    ret_val = _SMC_SendAPDU (slot_nbr, command, pu1SendData,
                                                                   u1SendLength);
                    ns = !ns;
                }
                else {
                    ns                  = !ns;
                    SMC_LOG_PRINT( "R(%d) -> \n", ns);                           
                    command.block_type  = RECEIVE_BLOCK;
                    //command.ins_code    = 0;
                    command.ns          = ns;
                    ret_val = _SMC_SendAPDU (slot_nbr, command, NULL, 0);
                    ns = !ns;
                }
            }
            else {
                ns = !ns;
                SMC_LOG_PRINT( "R(%d) -> \n", ns);                   
                command.block_type      = RECEIVE_BLOCK;
                //command.ins_code        = 0;
                command.ns              = ns;
                ret_val = _SMC_SendAPDU (slot_nbr, command, NULL, 0);
                ns = !ns;
            }
            break;

        case SUPER_BLOCK:
            if (!ret_val) {
                SMC_LOG_PRINT( "        <- S(IFS)\n");               	
                // IFS/WTX request by card
                VERIFY(((cmd.ins_code |0xC0) == 0xC0) && ((cmd.ins_code & 0x1C) == 0));        
                SMC_LOG_PRINT( "S(IFS) -> \n");                  
                command.block_type = SUPER_BLOCK;
                /* ------------ ins contains the control info --------------- */
                command.ins_code        = (cmd.ins_code & 0x03);
                command.ins_code       |= 0x20;
                ret_val = _SMC_SendAPDU (slot_nbr, command, receive_data, 1);
            }
            else {
                SMC_LOG_PRINT( "x       <- S(IFS)\n");
                command.ns              = ns;                
                SMC_LOG_PRINT( "R(%d)    ->\n", ns);               	            	                
                command.block_type      = RECEIVE_BLOCK;
                command.ins_code        = 0;                
                ret_val = _SMC_SendAPDU (slot_nbr, command, NULL, 0);
            }
            break;

        default:
            ret_val = SMCR_FAIL;
            goto ret;
        }
    }

ret:
    //osal_ret = OSAL_sem_give (card_sem_id);
    
    //TRACE_EXIT ();
    return ret_val;
}

INT32 _SMC_RequestIFS(UINT8 u1Len)
{
    SMC_CMD_SEND_T command;
    UINT8 superdata;
    UINT8 receive_data[258];
    UINT8 receive_length;
    command.block_type  = SUPER_BLOCK;
    command.ins_code    = 0x1;
    command.ns          = 0;
    superdata           = u1Len;  //0x80
    
    if(_SMC_SendAPDU(0, command, &superdata, 0x1) != SMCR_OK)
    {
        return SMCR_CARD_WRITE_ERROR;
    }
    if(_SMC_ReceiveAPDU(0, &command, receive_data, &receive_length) != SMCR_OK)
    {
        return SMCR_CARD_READ_ERROR;    
    }
    
    return SMCR_OK;
}


