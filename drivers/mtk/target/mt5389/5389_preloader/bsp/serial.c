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
 *  In serial.c, it provides basic functions for input/output through UART.
 *  This is a poll mode serial driver for boot start package(bsp).  It also
 *  can use the advanced uart driver to run in interrupt mode.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "x_serial.h"
#include "x_bim.h"
#include "x_assert.h"
#include "x_timer.h"
#include "ddr.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#ifndef CC_MTK_LOADER
#define USE_UART_DRV
#endif /* CC_MTK_LOADER */

#ifdef USE_UART_DRV
#include "uart_if.h"
#endif

//-----------------------------------------------------------------------------
// External Function
//-----------------------------------------------------------------------------
EXTERN void vUsbCLIPush(UINT8 u1Char);

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


// Local Define
#define UART_PORT_0  (UINT8)0
#define UART_PORT_1  (UINT8)1

#define DBG_PORT UART_PORT_0

// UART Register Define
#define SERIAL_BASE                 (IO_VIRT + 0xc000)

// Following are MT5371 UART registers
#define REG_SER_PORT                (*((volatile UINT8*)(SERIAL_BASE + /*0x40*/0)))
#define REG_SER_CTR                 (*((volatile UINT16*)(SERIAL_BASE + 0x48)))
    #define SER_BAUDRATE_115200     0x0000
    #define SER_BAUDRATE_921600     0x0300
#define REG_SER_CFG                 (*((volatile UINT32*)(SERIAL_BASE + 0x04)))
    #define REG_SER_NORMAL          0
    #define REG_SER_TRANSMODE       0x40
    #define REG_TRANS_MAGIC         0xe2
#define REG_SER_PC_READ_MODE        (*((volatile UINT32*)(SERIAL_BASE + 0x18)))
    #define SER_UR_ACTIVE           (0)
    #define SER_ADSP_ACTIVE         (1)
    #define SER_RISC_ACTIVE         (2)
#define REG_SER_BUFFER_SIZE         (*((volatile UINT32*)(SERIAL_BASE + 0x1c)))
#define REG_SER_STATUS              (*((volatile UINT16*)(SERIAL_BASE + 0x4c)))
    #define SER_WRITE_ALLOW         0x1F00
    #define SER_READ_ALLOW          0x001F

#define REG_SER01_PORT              (*((volatile UINT8*)(SERIAL_BASE + 0xC0)))
#define REG_SER01_CTR               (*((volatile UINT32*)(SERIAL_BASE + 0xC8)))
#define REG_SER01_STATUS            (*((volatile UINT32*)(SERIAL_BASE + 0xCC)))

#define REG_SER_INT_EN              (*((volatile UINT32*)(SERIAL_BASE + 0xC)))
#define REG_SER_INT_STATUS          (*((volatile UINT32*)(SERIAL_BASE + 0x10)))
    #define INT_SER00_TX_EMPTY      (UINT32)(0x1<<19)
    #define INT_SER00_RX_TOUT       (UINT32)(0x1<<18)
    #define INT_SER00_RX_FULL       (UINT32)(0x1<<17)
    #define INT_SER00_RX_ERR        (UINT32)(0x1<<16)
    #define INT_SER00_RX_OVERFLOW   (UINT32)(0x1<<21)
    #define INT_SER01_TX_EMPTY      (UINT32)(0x1<<3)
    #define INT_SER01_RX_TOUT       (UINT32)(0x1<<2)
    #define INT_SER01_RX_FULL       (UINT32)(0x1<<1)
    #define INT_SER01_RX_ERR        (UINT32)(0x1<<0)
    #define INT_SER01_RX_OVERFLOW   (UINT32)(0x1<<5)
    #define INT_SER01_MODEM         (UINT32)(0x1<<4)

#define REG_SER_MERGE_MODE          (*((volatile UINT32*)(SERIAL_BASE + 0x14)))

#define REG_SER0_BUF_CTL     (*((volatile UINT32*)(SERIAL_BASE + 0x50)))
#define REG_SER1_BUF_CTL     (*((volatile UINT32*)(SERIAL_BASE + 0xD0)))

#define REG_UART_UART1_LSR                  (*(volatile UINT32 *)(SERIAL_BASE+0x00CC))
#define REG_UART_UART2_LSR                  (*(volatile UINT32 *)(SERIAL_BASE+0x010C))

// For UART Interrupt mode
#define UART_INT_BUF_SIZE       1024
#define UART_VECTOR             17
#define SER_PORT_0  (0)
#define SER_PORT_1  (1)

// Use old UART2 buffer control register as testing register
#define TEST_REGEST 0xf000c110

// the default value of the last data in image.
#define MAGIC_NUM (0xc0cac01a)


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
#ifndef CC_MTK_PRELOADER
#ifdef CC_MTK_LOADER
// XXX USB busy loop callback function.
// extern void MUSB_NoneRunBackground(void);
#endif /* CC_MTK_LOADER */
#endif /* CC_MTK_PRELOADER */

#ifdef CC_MTK_PRELOADER
extern void CHIP_DisplayChar(UINT32 u4Char);
#endif /* CC_MTK_PRELOADER */


#if (!defined(CC_ROM_BOOT) || !defined(CC_MTK_PRELOADER)) || defined(DRAM_SLT_DEBUG)
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef CC_MTK_PRELOADER

#define _u1DebugPort    (DBG_PORT)
#define _fgSetoutbyte   (TRUE)

#else /* CC_MTK_PRELOADER */

#ifdef USE_UART_DRV
static UINT32 _u4SerIntMode = 0;
#endif /* USE_UART_DRV */
static UINT8 _u1DebugPort = DBG_PORT;
// static BOOL _fgIsMT5371 = TRUE;
static BOOL _fgSetoutbyte = TRUE;

#endif /* CC_MTK_PRELOADER */


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _BspDebugOutputToPort0() to check the debug port is port 0 or not.
 *  @retval TRUE    Use Port 0 as Debug Port.
 *  @retval FALSE   Not use Port 0 as Debug Port.
 */
//-----------------------------------------------------------------------------
static BOOL _BspDebugOutputToPort0()
{
    return (_u1DebugPort == SER_PORT_0);
}

//-----------------------------------------------------------------------------
/** _BspSerGetRxDataCnt()
 *  @return the number of characters in the RX buffer.
 */
//-----------------------------------------------------------------------------
static UINT8 _BspSerGetRxDataCnt()
{
    UINT8 u1RxCnt;

    if (_BspDebugOutputToPort0())
    {
        u1RxCnt = (UINT8)(REG_SER_STATUS & SER_READ_ALLOW);
    }
    else // Use UART1
    {
        u1RxCnt = (UINT8)(REG_SER01_STATUS & SER_READ_ALLOW);
    }

    return u1RxCnt;
}


//-----------------------------------------------------------------------------
/** void _BspSerGetTxEmptyCnt()
 *  @return TX buffer empty count.
 */
//-----------------------------------------------------------------------------
static UINT8 _BspSerGetTxEmptyCnt()
{
    if (_BspDebugOutputToPort0())
    {
        return (UINT8)((UINT16)(REG_SER_STATUS & SER_WRITE_ALLOW) >> 8);
    }
    else // Use UART1
    {
        return (UINT8)((UINT16)(REG_SER01_STATUS & SER_WRITE_ALLOW) >> 8);
    }
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** SerIsrEnable()
 *
 */
//-----------------------------------------------------------------------------
void SerIsrEnable(void)
{
}

//-----------------------------------------------------------------------------
/** SerIsrReg()
 *
 */
//-----------------------------------------------------------------------------
void SerIsrReg(void)
{
#ifdef USE_UART_DRV
    _u4SerIntMode = 1;
    VERIFY(UART_Init() == UART_SUCC);
#endif
}

//-----------------------------------------------------------------------------
/** SerIsrDisable()
 *
 */
//-----------------------------------------------------------------------------
void SerIsrDisable(void)
{
}

//-----------------------------------------------------------------------------
/** void SerInit(void) init Serial port routine.
 *
 */
//-----------------------------------------------------------------------------
void SerInit(void)
{
    // Setup a. baudrate
    //       b. local interrupt enable
    //       c. merge mode
    if (_BspDebugOutputToPort0())
    {
        REG_SER_CTR = SER_BAUDRATE_115200; // defualt 115200
        REG_SER_INT_EN = INT_SER00_RX_FULL;
#ifdef USE_UART_DRV
        REG_SER0_BUF_CTL = (0x1A00 | (32 - 10));
#else /* USE_UART_DRV */
        REG_SER0_BUF_CTL = (0x1A00 | (1));
#endif /* USE_UART_DRV */
    }
    else
    {
        REG_SER01_CTR = SER_BAUDRATE_115200; // defualt 115200
        REG_SER_INT_EN = INT_SER01_RX_FULL;
        REG_SER1_BUF_CTL = (0x1A00 | (32 - 10));
    }
}

//-----------------------------------------------------------------------------
/** void SerWaitTxBufClear(void) wait until UR1 TxBuf Clear
 *
 */
//-----------------------------------------------------------------------------
void SerWaitTxBufClear(void)
{
    while ((REG_SER01_STATUS & 0x1f00) != 0x1f00) { }
}

//-----------------------------------------------------------------------------
/** void SerTransparent(void) set Serial port as transparent mode \n
 *      (RISC output debug message)
 */
//-----------------------------------------------------------------------------
void SerTransparent(void)
{
    REG_SER_CFG = REG_TRANS_MAGIC;         // Set to transparent mode
#ifndef CC_MTK_PRELOADER
    SerSetoutbyte(TRUE);
#endif /* CC_MTK_PRELOADER */
}

//-----------------------------------------------------------------------------
/** void SerEnableMergeMode(BOOL fgEnable) set MergeMode
 *  @param fgEnable boolean value to enable/disable merge mode.
 */
//-----------------------------------------------------------------------------
void SerEnableMergeMode(BOOL fgEnable)
{
    if (fgEnable)
    {
        REG_SER_MERGE_MODE = 0x74;
    }
    else
    {
        REG_SER_MERGE_MODE = 0;
    }
}

//-----------------------------------------------------------------------------
/** void SerStart(void) start to enable RISC to uart.
 */
//-----------------------------------------------------------------------------
void SerStart(void)
{
    REG_SER_PC_READ_MODE = SER_RISC_ACTIVE;
#ifndef CC_MTK_PRELOADER
    SerSetoutbyte(TRUE);
#endif /* CC_MTK_PRELOADER */
}

//-----------------------------------------------------------------------------
/** void SerEnd(void) stop Serial port routine.
 */
//-----------------------------------------------------------------------------
void SerEnd(void)
{
    SerNormalMode();                        // Back to normal mode
    REG_SER_PC_READ_MODE = SER_UR_ACTIVE;   // Set RS232 active read mode
#ifndef CC_MTK_PRELOADER
    SerSetoutbyte(FALSE);
#endif /* CC_MTK_PRELOADER */
    // REG_SER_INT_EN = 0;      // Enable UART0 interrupt.
}

//-----------------------------------------------------------------------------
/** void SerInByte(UCHAR *puc) read RX character.
 *  @param puc a pointer to unsigned char, it cannot be NULL.
 *  @retval TRUE read a char from RX buffer and put into *puc.
 *  @retval FALSE no char in RX buffer.
 */
//-----------------------------------------------------------------------------
BOOL SerInByte(UCHAR* puc)
{
    if (_BspSerGetRxDataCnt() == 0)
    {
        return FALSE;
    }
    else
    {
        ASSERT(puc!=NULL);
        if (_BspDebugOutputToPort0())
        {
            *puc = REG_SER_PORT;
        }
        else // Use UART1
        {
            *puc = REG_SER01_PORT;
        }
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** void putchar(UINT8) put char, poll mode
 *
 */
//-----------------------------------------------------------------------------
void SerPollPutChar(UINT8 cc)
{
    while(_BspSerGetTxEmptyCnt() == 0) {}

    // If normal mode, set RISC active mode
    if ((REG_SER_CFG & REG_SER_TRANSMODE) == 0)
    {
        REG_SER_PC_READ_MODE = SER_RISC_ACTIVE;
    }

    if (_BspDebugOutputToPort0())
    {
        REG_SER_PORT = cc;
    }
    else // Use UART1
    {
        REG_SER01_PORT = cc;
    }

    // If normal mode, wait tx buffer clear & set UR active mode.
    if ((REG_SER_CFG & REG_SER_TRANSMODE) == 0)
    {
        SerWaitTxBufClear();
        REG_SER_PC_READ_MODE = SER_UR_ACTIVE;
    }
}

//-----------------------------------------------------------------------------
/** UINT32 SerGetRxDataCnt() to get the RxData count.
 *  @retval the char number in RX buffer.
 */
//-----------------------------------------------------------------------------
UINT32 SerGetRxDataCnt()
{
    return (_BspSerGetRxDataCnt());
}

//-----------------------------------------------------------------------------
/** UINT8 SerPollGetChar(void) get char, poll mode
 *  @return the character in RX buffer wait until there is data in RX buffer.
 */
//-----------------------------------------------------------------------------
UINT8 SerPollGetChar(/*UINT32 timeout */)
{
#ifndef CC_MTK_LOADER
#ifdef USE_UART_DRV
    if (_u4SerIntMode)
    {
        UINT8 ucCC;

        VERIFY(UART_Read(DBG_PORT, &ucCC, 1, TRUE) == 1);
        return ucCC;
    }
#endif
#endif /* CC_MTK_LOADER */

    while (_BspSerGetRxDataCnt() == 0)
    {
#ifndef CC_MTK_PRELOADER
#ifdef CC_MTK_LOADER
#ifndef CC_UBOOT
        // XXX USB busy loop callback function.
//        MUSB_NoneRunBackground();        
#endif /* CC_UBOOT */
#endif /* CC_MTK_LOADER */
#endif /* CC_MTK_PRELOADER */
    }

    if (_BspDebugOutputToPort0())
    {
        return REG_SER_PORT;
    }
    else // Use UART1
    {
        return REG_SER01_PORT;
    }
}

//-----------------------------------------------------------------------------
/** UINT8 SerGetChar(void) get char
 *  @return the character in RX buffer, if no data in RX buffer, return 0xff.
 */
//-----------------------------------------------------------------------------
UINT8 SerGetChar(void)
{
    // if data NOT ready, return 0xFF
    if (_BspSerGetRxDataCnt() == 0)
    {
        return (0xFF);
    }

    if (_BspDebugOutputToPort0())
    {
        return REG_SER_PORT;
    }
    else // Use UART1
    {
        return REG_SER01_PORT;
    }
}

//-----------------------------------------------------------------------------
/** UINT8 SerGetCharTimeout() get char with timeout loop number
 *  @param ucChar a pointer to char to store the getting char from RX buffer.
 *  @param ui4Time timeout value in ms.
 *  @reval 0 Until time out, there is no data to get.
 *  @reval 1 There is one char at *ucChar.
 */
//-----------------------------------------------------------------------------
UINT8 SerGetCharTimeout(UINT8* ucChar, UINT32 ui4Time)
{
    UINT32  ui4Delta = 0;

    while (ui4Delta < ui4Time)
    {
        if ((REG_SER_STATUS & SER_READ_ALLOW) == SER_READ_ALLOW)
        {
            ASSERT(ucChar != NULL);
            *ucChar = (UINT8)REG_SER_PORT;
            return 1;
        }
#ifdef CC_MTK_PRELOADER
        CHIP_Delay_us(1000);
#else
        HAL_Delay_us(1000);
#endif
        ui4Delta += 1000;
    }
    return 0;
}

//-----------------------------------------------------------------------------
/** char * SerGetS(char* s) get String, poll mode
 *  @param s a pointer to char array to store the string from RX buffer.
 *  @reval always return s.
 */
//-----------------------------------------------------------------------------
CHAR* SerGetS(CHAR* s)
{
    CHAR* p;
    UINT8 c;

    ASSERT(s != NULL);

    for (p = s; (c = SerPollGetChar()) != '\r' /* && c >= 0 */; p++) //+++ '\n' or '\r'
    {
        *p = (CHAR)c;
    }

    *p = '\0';

    return (s);
}

#ifndef CC_MTK_PRELOADER
//-----------------------------------------------------------------------------
/** UINT8 SerGetDebugPortNum() get debug port number
 *  @reval debug port number (0 or 1)
 */
//-----------------------------------------------------------------------------
UINT8 SerGetDebugPortNum(void)
{
    return _u1DebugPort;
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
#endif /* CC_MTK_PRELOADER */

//-----------------------------------------------------------------------------
/** void outbyte(int c) out char, poll mode, I/F with Print() (printf)
 *  @param c a character which is going to put into TX buffer.
 */
//-----------------------------------------------------------------------------
void outbyte(CHAR c)
{
#ifndef CC_MTK_LOADER
    static CHAR prev = 0;
#endif /* CC_MTK_LOADER */

    if (!_fgSetoutbyte)
    {
        return;
    }

    if ((c < ' ') && (c != '\r') && (c != '\n') && (c != '\t') && (c != '\b'))
    {
        return;
    }

#ifdef CC_MTK_LOADER
    if (c == '\n')
    {
#ifdef CC_MTK_PRELOADER
        CHIP_DisplayChar((UINT32)'\r');
#else /* CC_MTK_PRELOADER */
        SerPollPutChar('\r');
#endif /* CC_MTK_PRELOADER */
    }
#else /* CC_MTK_LOADER */
    if ((c == '\n') && (prev != '\r'))
    {
        SerPollPutChar('\r');
    }
    prev = c;
#endif /* CC_MTK_LOADER */

#ifdef CC_MTK_PRELOADER
        CHIP_DisplayChar((UINT32)c);
#else /* CC_MTK_PRELOADER */
    SerPollPutChar((UINT8)c);
#endif /* CC_MTK_PRELOADER */

#ifndef CC_MTK_LOADER
	// Put byte into USB CLI buffer
    if (c == '\n')
    {
        vUsbCLIPush('\r');
    }
    vUsbCLIPush(c);
#endif

}
#endif
//-----------------------------------------------------------------------------
/** void SerTransparent(void) set Serial port as normal mode \n
 *      (for MTK tools, DSP tools).
 */
//-----------------------------------------------------------------------------
void SerNormalMode(void)
{
    REG_SER_CFG = REG_SER_NORMAL;        // Set to normal mode
}
