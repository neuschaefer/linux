/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2004, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: debug.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file timer.c
 *  timer.c provides timer function from boot time.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_hal_5381.h"
#include "x_ckgen.h"
#include "x_bim.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define RS232_READ32(off)       IO_READ32(RS232_BASE, (off))
#define RS232_WRITE32(off, val) IO_WRITE32(RS232_BASE, (off), (val))

#define REG_U0_OUTPORT          (0x0000)
#define REG_CMD_STATUS          (0x0004)
    #define CMD_MODE            (0x00)
#define REG_RS232_MODE          (0x0018)
    #define RISC_ACTIVE         (0x02)
#define REG_BUFFER_SIZE         (0x001c)
    #define SINGLE_BYTE         (0x00)
#define REG_U0_STATUS           (0x004c)
    #define STATUS_TXBUF        (0x1f00)
#define REG_U0_BUFCTL           (0x0050)
    #define DEF_BUFCTL          (0xff01)

#define PDWNC_READ32(off)       IO_READ32(PDWNC_BASE, (off))
#define PDWNC_WRITE32(off, val) IO_WRITE32(PDWNC_BASE, (off), (val))    

#define REG_RW_IOPADEN          (0x0080)        // IO Pad Output Enable Register
    #define PAD_UR0_TX_EN       (1U << 15)      // UART 0 TX PAD output enable
#define REG_RW_WATCHDOG_EN      0x0100          // Watchdog Timer Control Register
#define REG_RW_WATCHDOG_TMR     0x0104          // Watchdog Timer Register

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Below are the call sub-function routines
//----------------------------------------------------------------------------
#ifndef NDEBUG

#ifdef CC_NOR_DEBUG

#define REG_SER_PORT                (*((volatile UINT8*)(RS232_BASE + /*0x40*/0)))
#define REG_SER_STATUS              (*((volatile UINT16*)(RS232_BASE + 0x4c)))
    #define SER_READ_ALLOW          0x001F

UINT8 CHIP_GetChar(void)
{
    if (REG_SER_STATUS & SER_READ_ALLOW)
    {
        return REG_SER_PORT;
    }
    return 0xff;
}

#endif /* CC_NOR_DEBUG */

void CHIP_DisplayChar(UINT32 u4Char);
void CHIP_DisplayDigit(UINT32 u4Char);

void CHIP_DisplayByte(UINT8 u1Val)
{
    CHIP_DisplayChar((UINT32)'0');
    CHIP_DisplayChar((UINT32)'x');
    CHIP_DisplayDigit((UINT32)((u1Val >> 4)&0xf));
    CHIP_DisplayDigit((UINT32)((u1Val >> 0)&0xf));
}

void CHIP_DisplayHex(UINT32 u4Val)
{
    CHIP_DisplayChar((UINT32)'0');
    CHIP_DisplayChar((UINT32)'x');
    CHIP_DisplayDigit((u4Val >> 28)&0xf);
    CHIP_DisplayDigit((u4Val >> 24)&0xf);
    CHIP_DisplayDigit((u4Val >> 20)&0xf);
    CHIP_DisplayDigit((u4Val >> 16)&0xf);
    CHIP_DisplayDigit((u4Val >> 12)&0xf);
    CHIP_DisplayDigit((u4Val >> 8)&0xf);
    CHIP_DisplayDigit((u4Val >> 4)&0xf);
    CHIP_DisplayDigit((u4Val >> 0)&0xf);
}

#endif /* NDEBUG */

void CHIP_DisplayChar(UINT32 u4Char)
{
#ifdef CC_TRUSTZONE_DEBUG
    while ((RS232_READ32(REG_U0_STATUS) & STATUS_TXBUF) != STATUS_TXBUF) { }
    RS232_WRITE32(REG_U0_OUTPORT, (UINT32)(u4Char & 0xff));
    if (u4Char == ((UINT32)'\n'))
    {
        RS232_WRITE32(REG_U0_OUTPORT, (UINT32)'\r');
    }
#endif // CC_TRUSTZONE_DEBUG
}

void CHIP_DisplayDigit(UINT32 u4Char)
{
    if (u4Char <= 9) { u4Char += '0'; }
    if ((u4Char >= 0x0a) && (u4Char <= 0x0f)) { u4Char += ('a' - 0x0a); }
    CHIP_DisplayChar(u4Char);
}

void CHIP_DisplayString(const INT8 *szString)
{
    if(szString == NULL)
    {
        return;
    }
    
    while (*szString != 0)
    {
        // busy wait if no tx buffer slot.
        CHIP_DisplayChar(((UINT32)((UINT8)(*szString))));
        szString++;
    }
}

void put_num(const CHAR c)
{
    CHIP_DisplayChar((UINT32)(UINT8)(c + '0'));
}

void CHIP_DisplayInteger(INT32 i)
{
    INT32 j, k, l, m;

    if (i < 0)
    {
        CHIP_DisplayChar('-');
        i = 0 - i;
    }

    k = i;
    for (j=0; j<12; j++)
    {
        k = k / 10;
        if (k == 0) { break; }
    }

    for (m=j; m>=0; m--)
    {
        k = i;
        for (l=0; l<m; l++)
        {
            k = k / 10;
        }
        k = k % 10;
        put_num(k);
    }
}

void ShowCurrentTime() 
{
    UINT32 u4Val;
    
#ifdef NDEBUG
    // Setup CPU to UART0 output parameter.
    RS232_WRITE32(REG_CMD_STATUS, CMD_MODE);
    RS232_WRITE32(REG_RS232_MODE, RISC_ACTIVE);
    RS232_WRITE32(REG_BUFFER_SIZE, SINGLE_BYTE);
    RS232_WRITE32(REG_U0_BUFCTL, DEF_BUFCTL);

    // Turn UART TX pad.
    PDWNC_WRITE32(REG_RW_IOPADEN, (PDWNC_READ32(REG_RW_IOPADEN) | PAD_UR0_TX_EN));
#endif
    u4Val = BIM_READ32(REG_RW_TIMER2_LOW);
    u4Val = ~u4Val;
    //Change to us
    u4Val = u4Val / (GET_XTAL_CLK() / 1000000);
    CHIP_DisplayInteger(u4Val); 
    CHIP_DisplayString(" us.\n");
}

