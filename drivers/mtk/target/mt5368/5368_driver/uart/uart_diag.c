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
 * $RCSfile: uart_diag.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file uart_diag.c
 *  Brief of file uart_diag.c.
 *  Details of file uart_diag.c (optional).
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

//#define CC_CLI

#ifdef CC_CLI

#include "x_lint.h"

// Do not lint this file

LINT_SAVE_AND_DISABLE

#include "x_serial.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_assert.h"
#include "x_printf.h"
#include "uart_hw_drvif.h"
#include "uart_drv.h"
#include "uart_debug.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define TEST_CNT    ((UINT32)512)
#define TEST_PORT   ((UINT8)UART_PORT_1)

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static UINT8 _au1TxBuf[TEST_CNT];
static UINT8 _au1RxBuf[TEST_CNT];
//static UINT32 u4ReadCnt = 0;
//static UINT32 u4WriteCnt = 0;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
BOOL _Uart_DataTest(UINT8 u1Port, RS_232_SETUP_INFO_T *prUartSetupInfo) 
{
    INT32 i;

    // set port parameter.
    UART_SetParameter(u1Port, prUartSetupInfo);

    // wait pc to set port parameter.
    x_thread_delay(1000);

    UART_Write(u1Port, _au1TxBuf, TEST_CNT, TRUE);

    // Fill in testing stream
    for (i = 0; i < TEST_CNT; i++)
    {
        _au1RxBuf[i] = 0;
    }

    UART_Read(u1Port, _au1RxBuf, TEST_CNT, TRUE);

    // Fill in testing stream
    for (i = 0; i < TEST_CNT; i++)
    {
        if (_au1TxBuf[i] != _au1RxBuf[i])
        {
            return FALSE;
        }
    }

    // wait pc to process port parameter.
    //x_thread_delay(1000);
    return TRUE;
}

INT32 UART_Test(UINT8 u1Port)
{
    BOOL  fResult = TRUE;
    INT32 i;
    UINT8 eStopBit = 0;
    UINT8 eParity = 0;
    UINT8 eDataLen = 0;
    UINT8 eBaudRate = 0;
    RS_232_SETUP_INFO_T rUartSetupInfo;

    //swap output pin
    if (u1Port == UART_PORT_1)
    {
        //  MERGE_MODE	UART Pin Swap Configuration 
        //  0b  UART pin swap is disabled
        //  1b  UART pin swap is enabled 
        UART_WRITE32(0x14, 1); 
        
        // wait uart 0 to send out all character before switch output pin.
        x_thread_delay(1000);
        
        // 1:0 LOOPBACK_MODE	UART Internal Loopback Control Configuration
        //
        // 00b UART0 loopback is disabled. UART1 loopback is disabled.
        // 01b UART0 self loopback is enabled, ie UART0's input pins are 
        //     connected to UART0's output pins inside chip.
        //     UART1 loopback is disabled.
        // 10b UART0 loopback is disabled and UART1 self loopback is enabled, 
        //     ie UART1's input pins are connected to UART1's output pins
        //     inside chip.
        // 11b UART cross loopback is enabled, ie UART0's input pins are 
        //     connected to UART1's output pins inside chip,
        //     and UART1's input pins are connected to UART0's output pins.
        UART_WRITE32(0x24, 0x5AF00002);              

        // wait uart 0 to send out all character before switch output pin.
        x_thread_delay(1000);
    }
    else
    {
        UART_WRITE32(0x24, 0x5AF00001);
    }

	
	for (eDataLen = RS_232_DATA_LEN_5; eDataLen <= RS_232_DATA_LEN_8; eDataLen++)		
	{

    // Fill in testing stream
    for (i = 0; i < TEST_CNT; i++)
    {
		    _au1TxBuf[i] = ((i << (8-(eDataLen+4))) & 0xFF) >>(8-(eDataLen+4));	    
    }

    for (eBaudRate = RS_232_SPEED_110; eBaudRate <= RS_232_SPEED_921600;
         eBaudRate++)
    {
        if ((eBaudRate == RS_232_SPEED_134) || (eBaudRate == RS_232_SPEED_150) ||
            (eBaudRate == RS_232_SPEED_600) || (eBaudRate == RS_232_SPEED_1800) ||
            (eBaudRate == RS_232_SPEED_14400) || (eBaudRate == RS_232_SPEED_128000))
        {
            break;
        }
            for (eParity = RS_232_PARITY_NONE; eParity <= RS_232_PARITY_ODD;
                 eParity++)            
            {             
                rUartSetupInfo.e_speed = (RS_232_SPEED_T)eBaudRate;
                rUartSetupInfo.e_stop_bit = (RS_232_STOP_BIT_T)eStopBit;
                rUartSetupInfo.e_parity = (RS_232_PARITY_T)eParity;
                rUartSetupInfo.e_data_len = (RS_232_DATA_LEN_T)eDataLen;                    
                fResult = _Uart_DataTest(TEST_PORT, &rUartSetupInfo);
                if (!fResult)
                {
                    goto end;
                }
		else
		{
        	     Printf("Baudrate = %d, Data Length = %d, Parity = %d,ok!\n",
               	            eBaudRate, eDataLen, eParity);
		}
            }
        }
    }
    
end:
    //swap output pin
    if (TEST_PORT == UART_PORT_1)
    {
        UART_WRITE32(0x24, 0x5AF00000);
        
        // wait uart 0 to send out all character before switch output pin.
        x_thread_delay(1000);
        
        UART_WRITE32(0x14, 0);
        x_thread_delay(100);
    }

    // restore UART 0 parameter.
    rUartSetupInfo.e_speed = (RS_232_SPEED_T)RS_232_SPEED_115200;
    rUartSetupInfo.e_stop_bit = (RS_232_STOP_BIT_T)STOP_BIT_1;
    rUartSetupInfo.e_parity = (RS_232_PARITY_T)RS_232_PARITY_NONE;
    rUartSetupInfo.e_data_len = (RS_232_DATA_LEN_T)RS_232_DATA_LEN_8;                    
    UART_SetParameter(UART_PORT_0, &rUartSetupInfo);
    SerSetoutbyte(TRUE);
    x_thread_delay(1000);
    if (fResult)
    {
        Printf("UART Port (%d) testing done~\n", TEST_PORT);
    }
    else
    {
        Printf("UART Port (%d) testing failed~\n", TEST_PORT);
        Printf("    Baudrate = %d, Data Length = %d, Parity = %d\n",
               eBaudRate, eDataLen, eParity);
    }
    return 0;
}

// Restore Lint settings
LINT_RESTORE

#endif
