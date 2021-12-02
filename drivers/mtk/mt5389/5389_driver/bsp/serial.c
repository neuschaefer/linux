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
 * $RCSfile: serial.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file serial.c
 *  Serial port support routines for BSP.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "serial.h"
#include "x_os.h"
#include "x_hal_5381.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/delay.h>


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define KEY_QUEUE_NAME                  "KEY"
#define KEY_QUEUE_SIZE                  1024

#define SERIAL_BASE         	    (IO_VIRT + 0xc000)
#define REG_SER_CFG                 (*((volatile UINT32*)(SERIAL_BASE + 0x04)))
#define REG_SER_PC_READ_MODE        (*((volatile UINT32*)(SERIAL_BASE + 0x18)))


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static HANDLE_T _hKeyQueue = (HANDLE_T) 0;
static BOOL _fgSetoutbyte = TRUE;
static BOOL _fgSerInit = FALSE;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void outbyte(CHAR c)
{
    printk("%c", c);
}

void SerPollPutChar(UINT8 cc)
{
    outbyte(cc);
}

void Serial_Init(void)
{
    if (_fgSerInit)
    {
        return;
    }

    _fgSerInit = TRUE;

    if (x_msg_q_create(&_hKeyQueue, KEY_QUEUE_NAME,
        sizeof(char), KEY_QUEUE_SIZE) != OSR_OK)
    {
        printk(KERN_DEBUG "Fail to create key queue!\n");
    }
}

INT32 Serial_PutKey(char cKey)
{
    if (!_fgSerInit)
    {
        Serial_Init();
    }

    if (x_msg_q_send(_hKeyQueue, (VOID*)&cKey, sizeof(char), 255)
        != OSR_OK)
    {
        printk(KERN_DEBUG "Key queue full!\n");
        return -1;
    }

    return 0;
}

char Serial_GetKey(void)
{
    INT32 i4Ret;
    UINT16 u2QueueIndex;
    SIZE_T zMsgSize;
    char cKey;

    if (!_fgSerInit)
    {
        Serial_Init();
    }

    zMsgSize = sizeof(char);
    i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&cKey, &zMsgSize,
        &_hKeyQueue, 1, X_MSGQ_OPTION_WAIT);

    if (i4Ret != OSR_OK)
    {
        printk(KERN_DEBUG "Error on receiving key!\n");
        return -1;
    }

    return cKey;
}

char Serial_GetKey_Timeout(void)
{
    INT32 i4Ret;
    UINT16 u2QueueIndex;
    SIZE_T zMsgSize;
    char cKey;

    if (!_fgSerInit)
    {
        Serial_Init();
    }

    zMsgSize = sizeof(char);
    i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&cKey, &zMsgSize,
        &_hKeyQueue, 1, 100);

    if (i4Ret != OSR_OK)
    {
        return 0;
    }

    return cKey;
}

UINT8 SerPollGetChar(void)
{
    return (UINT8)Serial_GetKey();
}

UINT8 SerGetChar(void)
{
    return (UINT8)Serial_GetKey();
}

BOOL SerInByte(UCHAR* puc)
{
    *puc = Serial_GetKey_Timeout();

    return (*puc != 0);
}

//-----------------------------------------------------------------------------
/** UINT8 SerSetoutbyte() set outbyte function enable/disable
 *  @parameter fgSet TRUE: enable outbyte(), FALSE: disable outbyte()
 */
//-----------------------------------------------------------------------------
void SerSetoutbyte(BOOL fgSet)
{
    _fgSetoutbyte = fgSet;
}

//-----------------------------------------------------------------------------
/** void SerTransparent(void) set Serial port as transparent mode \n
 *      (RISC output debug message)
 */
//-----------------------------------------------------------------------------
void SerTransparent(void)
{
    REG_SER_CFG = 0xe2;     	// Set to transparent mode
    SerSetoutbyte(TRUE);
}

//-----------------------------------------------------------------------------
/** void SerTransparent(void) set Serial port as normal mode \n
 *      (for MTK tools, DSP tools).
 */
//-----------------------------------------------------------------------------
void SerNormalMode(void)
{
    REG_SER_CFG = 0;     	// Set to normal mode
    REG_SER_PC_READ_MODE = 0; // Set RS232 active read mode
}

//-----------------------------------------------------------------------------
/** void SerEnd(void) stop Serial port routine.
 */
//-----------------------------------------------------------------------------
void SerEnd(void)
{
    SerNormalMode();         // Back to normal mode
    SerSetoutbyte(FALSE);
    // REG_SER_INT_EN = 0;      // Enable UART0 interrupt.
}
