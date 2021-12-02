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

#include "x_os.h"
#include "x_drv_cli.h"

#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_serial.h"
#include "x_hal_io.h"
#include "x_assert.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_pdwnc.h"
#include "x_ckgen.h"
#include "x_dram.h"
#include "x_pinmux.h"
// #include "drv_ir.h"
#include "x_bim.h"
#include "x_lint.h"
// #include "panel.h"
//#include "drv_uart.h"
#include "drvcust_if.h"


UINT32 u4CMD_IO_OFFSET = 0x00000000;


/******************************************************************************
* Function prototype
******************************************************************************/
#ifdef CC_CLI
static INT32 _CmdMemSet(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _CmdMemSetByte(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _CmdMemCopy(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _CmdMemRead(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _CmdMemReadHalfWord(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _CmdMemReadByte(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _CmdMemWrite(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _CmdMemWriteHalfWord(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _CmdMemWriteByte(INT32 i4Argc, const CHAR ** szArgv);
#endif /* CC_CLI */
#ifdef __KERNEL__
static INT32 _CmdLinuxMode(INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _CmdStopUart(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _CmdSvDetect(INT32 i4Argc, const CHAR ** szArgv);

EXTERN void UART_SetDebugPortFactory(BOOL fgSet);

#ifdef CC_CLI
/******************************************************************************
* Function      : _CmdMemSet
* Description   : memory set
******************************************************************************/
static INT32 _CmdMemSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 ui4DestAddr;
    UINT32 ui4Value;
    UINT32 ui4Len;
    UINT32 ui4Idx;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        return 0;
    }

    ui4DestAddr = StrToInt(szArgv[1]);
    ui4Value = StrToInt(szArgv[2]);
    ui4Len = StrToInt(szArgv[3]);

    // DW alignment
    ui4DestAddr &= 0xFFFFFFFC;

    for (ui4Idx = 0; ui4Idx < ui4Len; ui4Idx++)
    {
        ASSERT(ui4DestAddr != NULL);
        IO_WRITE32(u4CMD_IO_OFFSET, ui4DestAddr, ui4Value);
        ui4DestAddr += 4;
    }

    return 0;
//lint -e{818}
}


/******************************************************************************
* Function      : _CmdMemSetByte
* Description   : memory set
******************************************************************************/
static INT32 _CmdMemSetByte(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4DestAddr;
    UINT8 ucValue;
    UINT32 u4Len;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        return 0;
    }

    u4DestAddr = StrToInt(szArgv[1]);
    ucValue = (UINT8) StrToInt(szArgv[2]) & 0xFF;
    u4Len = StrToInt(szArgv[3]);

    VERIFY(x_memset((void *)u4DestAddr, ucValue, u4Len) == (void *)u4DestAddr);

    return 0;

//lint -e{818}
}


/******************************************************************************
* Function      : _CmdMemCopy
* Description   : memory copy
******************************************************************************/
static INT32 _CmdMemCopy(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4DestAddr;
    UINT32 u4SrcAddr;
    UINT32 u4Len;

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        return 0;
    }

    u4DestAddr = StrToInt(szArgv[1]);
    u4SrcAddr = StrToInt(szArgv[2]);
    u4Len = StrToInt(szArgv[3]);

    VERIFY(x_memcpy((void *)u4DestAddr, (void *)u4SrcAddr, u4Len) == (void *)u4DestAddr);

    return 0;
//lint -e{818}
}
#endif /* CC_CLI */


/******************************************************************************
* Function      : _CmdMemRead
* Description   : memory read, word
******************************************************************************/
static INT32 _CmdMemRead(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4SrcAddr;
    UINT32 u4Len;
    UINT32 u4Idx;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        return 0;
    }

    u4SrcAddr = StrToInt(szArgv[1]);
    u4Len = StrToInt(szArgv[2]);

    if (u4Len == 0)
    {
        return 0;
    }
    if (u4Len > 0x1000)
    {
        u4Len = 0x1000;
    }

    // DW alignment
    u4SrcAddr &= 0xFFFFFFFC;

    for (u4Idx = 0;
        u4Idx < u4Len;
        u4Idx += 16)
    {
        //lint -e{613}
        Printf("0x%08X | %08X %08X %08X %08X\n"
            , (u4SrcAddr + u4Idx)
            , IO_READ32(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 0)
            , IO_READ32(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 4)
            , IO_READ32(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 8)
            , IO_READ32(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 12));
    }

    return 0;
//lint -e{818}
}


#ifdef CC_CLI
/******************************************************************************
* Function      : _CmdMemReadHalfWord
* Description   : memory read, half word
******************************************************************************/
static INT32 _CmdMemReadHalfWord(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4SrcAddr;
    UINT32 u4Len;
    UINT32 u4Idx;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        return 0;
    }

    u4SrcAddr = StrToInt(szArgv[1]);
    u4Len = StrToInt(szArgv[2]);

    if (u4Len == 0)
    {
        return 0;
    }
    if (u4Len > 0x1000)
    {
        u4Len = 0x1000;
    }

    // WORD alignment
    u4SrcAddr &= 0xFFFFFFFE;

    for (u4Idx = 0;
        u4Idx < u4Len;
        u4Idx += 16)
    {
        //lint -e{613}
        Printf("0x%08X | %04X %04X %04X %04X %04X %04X %04X %04X\n"
            , (u4SrcAddr + u4Idx)
            , IO_READ16(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 0)
            , IO_READ16(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 2)
            , IO_READ16(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 4)
            , IO_READ16(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 6)
            , IO_READ16(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 8)
            , IO_READ16(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 10)
            , IO_READ16(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 12)
            , IO_READ16(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 14));
    }

    return 0;
//lint -e{818}
}


/******************************************************************************
* Function      : _CmdMemReadByte
* Description   : memory read, byte
******************************************************************************/
static INT32 _CmdMemReadByte(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4SrcAddr;
    UINT32 u4Len;
    UINT32 u4Idx;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        return 0;
    }

    u4SrcAddr = StrToInt(szArgv[1]);
    u4Len = StrToInt(szArgv[2]);

    if (u4Len == 0)
    {
        return 0;
    }
    if (u4Len > 0x1000)
    {
        u4Len = 0x1000;
    }

    for (u4Idx = 0;
        u4Idx < u4Len;
        u4Idx += 16)
    {
        //lint -e{613}
        Printf("0x%08X | %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n"
            , (u4SrcAddr + u4Idx)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 0)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 1)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 2)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 3)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 4)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 5)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 6)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 7)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 8)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 9)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 10)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 11)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 12)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 13)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 14)
            , IO_READ8(u4CMD_IO_OFFSET, u4SrcAddr + u4Idx + 15));
    }

    return 0;
//lint -e{818}
}
#endif /* CC_CLI */


/******************************************************************************
* Function      : _CmdMemWrite
* Description   : memory write, word
******************************************************************************/
static INT32 _CmdMemWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4DestAddr;
    UINT32 u4Value;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        return 0;
    }

    u4DestAddr = StrToInt(szArgv[1]);
    u4Value = StrToInt(szArgv[2]);

    // DW alignment
    u4DestAddr &= 0xFFFFFFFC;

    ASSERT(u4DestAddr != NULL);
    IO_WRITE32(u4CMD_IO_OFFSET, u4DestAddr, u4Value);

    return 0;
//lint -e{818}
}


#ifdef CC_CLI
/******************************************************************************
* Function      : _CmdMemWriteHalfWord
* Description   : memory write, half word
******************************************************************************/
static INT32 _CmdMemWriteHalfWord(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4DestAddr;
    UINT16 u2Value;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        return 0;
    }

    u4DestAddr = StrToInt(szArgv[1]);
    u2Value = (UINT16) StrToInt(szArgv[2]);

    // Word alignment
    u4DestAddr &= 0xFFFFFFFE;

    ASSERT(u4DestAddr != NULL);
    IO_WRITE16(0, u4DestAddr, u2Value);

    return 0;
//lint -e{818}
}


/******************************************************************************
* Function      : _CmdMemWriteByte
* Description   : memory write, byte
******************************************************************************/
static INT32 _CmdMemWriteByte(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4DestAddr;
    UINT8 ucValue;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        return 0;
    }

    u4DestAddr = StrToInt(szArgv[1]);
    ucValue = (UINT8) StrToInt(szArgv[2]);

    ASSERT(u4DestAddr != NULL);
    IO_WRITE8(0, u4DestAddr, ucValue);

    return 0;
//lint -e{818}
}

#endif /* CC_CLI */

/******************************************************************************
* Function      : _CmdLinuxMode
* Description   : Turn on/off linux mode
******************************************************************************/
#ifdef __KERNEL__
static INT32 _CmdLinuxMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Value;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Turn on/off linux mode. 1 (on), 0 (off)\n");
        return 0;
    }

    u4Value = StrToInt(szArgv[1]);

    Printf("Linux mode is turned %s\n", u4Value ? "On" : "Off");
    if (u4Value)
    {
        u4CMD_IO_OFFSET = 0xD0000000;
        Printf("Read/Write 0x20000000 is mapped to 0xF0000000 now!\n");
    }
    else
    {
        u4CMD_IO_OFFSET = 0x00000000;
    }

    return 0;
//lint -e{818}
}
#endif

/******************************************************************************
* Function      : _CmdEnterUart
* Description   : stop cli console, RS232 leave transparent mode
******************************************************************************/
static INT32 _CmdEnterUart(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

//    while (UART_InputSwitch()==1) { }
    SerTransparent();
    Printf("RS232 enter transparent mode! no 0 any more.\n");
    return 0;
//lint -e{818}
}

/******************************************************************************
* Function      : _CmdFactoryUart
* Description   : Set RS232 enter factory mode
******************************************************************************/
static INT32 _CmdFactoryUart(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("RS232 enter factory mode! Console stop! \n");
#ifndef CC_5391_LOADER
#ifndef __KERNEL__
    UART_SetDebugPortFactory(TRUE);
#endif /* __KERNEL__ */
#endif /* CC_5391_LOADER */
    return 0;
//lint -e{818}
}

/******************************************************************************
* Function      : _CmdStopUart
* Description   : stop cli console, RS232 leave transparent mode
******************************************************************************/
static INT32 _CmdStopUart(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("RS232 leave transparent mode! Console stop! \n");
#ifndef CC_5391_LOADER
#ifndef __KERNEL__
    UART_SetDebugPortFactory(FALSE);
#endif /* __KERNEL__ */
#endif /* CC_5391_LOADER */
    SerEnd();
    return 0;
//lint -e{818}
}

#define GPIO_JTDI               16
#define SEL_JTDI                JTDI_I
#define PINMUX_FUNCTION1        1

INT32 CLI_SetCliMode(CLI_ACCESS_RIGHT_T eCliAccess);

static INT32 _CmdSvDetect(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Func;
    INT32 i4ModeRead = 0;
    INT32 i4PinData;

    UNUSED(i4Argc);
    UNUSED(szArgv);

//    i4Func = BSP_PinGet(SEL_JTDI);
    LINT_SUPPRESS_NEXT_EXPRESSION(534);
//    BSP_PinSet(SEL_JTDI, PINMUX_FUNCTION1);
    LINT_SUPPRESS_NEXT_EXPRESSION(534);
//    GPIO_Enable(GPIO_JTDI, &i4ModeRead);
//    i4PinData = GPIO_Input(GPIO_JTDI);
    LINT_SUPPRESS_NEXT_EXPRESSION(534);
//    BSP_PinSet(SEL_JTDI, i4Func);
    if (i4PinData == 0)
    {
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
        CLI_SetCliMode(CLI_SUPERVISOR);
        Printf("SV mode\n");
    }
    else
    {
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
        CLI_SetCliMode(CLI_GUEST);
        Printf("GUEST mode\n");
    }
    return 0;
}


#ifdef CC_CLI

#define PRINT_CHAR(c) ((c > 0x7f) ? '?' : ((c < 0x20) ? '?' : c))

//lint -save -e818
/******************************************************************************
*    Function : _CmdMemDump(UINT32 i4Argc, const CHAR ** szArgv)
* Description : Process command of memory comparision
*   Parameter :
*       i4Argc    - IN    - The number of command arguements.
*       szArgv    - IN    - The arguement array list.
*   Return    : 1 failed, and 0 successed.
******************************************************************************/
static INT32 _CmdMemDump(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Len, i, j;
    UINT8 *pu1Ptr;

    ASSERT(szArgv!=NULL);
    if (i4Argc != 3) {
        Printf("memcmp\t\t Compare memory.\tEx: memcmp [addr] [byte-len].\n");
        return 1;
    }

    pu1Ptr = (UINT8 *)StrToInt(szArgv[1]);
    i4Len = (INT32)StrToInt(szArgv[2]);

    Printf("\n");
    for (i=0; i<i4Len; i+=0x10) {
        Printf("0x%08x |", ((INT32)pu1Ptr) + i);
        for (j=0; j<0x10; j++) {
            Printf(" %02x", pu1Ptr[i+j]);
        }
        Printf(" | ");
        for (j=0; j<0x10; j++) {
            Printf("%c", PRINT_CHAR(pu1Ptr[i+j]));
        }
        Printf("\n");
    }
    return 0;
}

/******************************************************************************
*    Function : _CmdMemCmp(UINT32 i4Argc, const CHAR ** szArgv)
* Description : Process command of memory comparision
*   Parameter :
*       i4Argc    - IN    - The number of command arguements.
*       szArgv    - IN    - The arguement array list.
*   Return    : 1 failed, and 0 successed.
******************************************************************************/
static INT32 _CmdMemCmp(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr1, u4Addr2, u4Len, u4Off, i;
    UINT8 *pu1Addr1, *pu1Addr2;

    ASSERT(szArgv!=NULL);
    if (i4Argc != 4) {
        Printf("memcmp\t\t Compare memory.\tEx: memcmp [addr1] [addr2] [byte-len].\n");
        return 1;
    }

    u4Addr1 = StrToInt(szArgv[1]);
    u4Addr2 = StrToInt(szArgv[2]);
    u4Len = StrToInt(szArgv[3]);
    for (u4Off = 0; (u4Len - u4Off) >= 4; u4Off+=4) {
        if ((*((UINT32 *)(u4Addr1 + u4Off))) != (*((UINT32 *)(u4Addr2 + u4Off)))) {
            Printf("At offset 0x%08x, Addr1 is 0x%08x and Addr2 is 0x%08x\n", u4Off,
                (*((UINT32 *)(u4Addr1 + u4Off))), (*((UINT32 *)(u4Addr2 + u4Off))));
            if (((*((UINT32 *)(u4Addr1 + u4Off))) - (*((UINT32 *)(u4Addr2 + u4Off)))) > 0) {
                return (INT32)((*((UINT32 *)(u4Addr1 + u4Off))) - (*((UINT32 *)(u4Addr2 + u4Off))));
            }
            return (INT32)((*((UINT32 *)(u4Addr2 + u4Off))) - (*((UINT32 *)(u4Addr2 + u4Off))));
        }
    }

    pu1Addr1 = ((UINT8 *)(u4Addr1 + u4Off));
    pu1Addr2 = ((UINT8 *)(u4Addr2 + u4Off));
    for (i=0; i < (u4Len - u4Off); i++) {
        if (pu1Addr1[i]!=pu1Addr2[i]) {
            Printf("At offset 0x%08x, Addr1 is 0x%02x and Addr2 is 0%02x\n", u4Off+i, pu1Addr1[i], pu1Addr2[i]);
            if ((pu1Addr1[i] - pu1Addr2[i]) > 0) {
                return (INT32)(pu1Addr1[i] - pu1Addr2[i]);
            }
            return (INT32)(pu1Addr2[i] - pu1Addr2[i]);
        }
    }
    Printf("Memory comparision is the same!!!\n");

    return 0;
}

//lint -emacro({717}, SET_LOOP)  to disable "do {...} while (0)" lint warning
#define SET_LOOP(ptr, type, addr, ival)                                 \
    do {                                                                \
        ptr = (type *)(addr);                                           \
        for (i = 0; i < u4Len; i++) {                                   \
            ptr[i] = (type)((((UINT64)u4Step) * ((UINT64)i)) + ival);   \
        }                                                               \
    } while (0)

/******************************************************************************
*    Function : L1CMDMemOrder(UINT32 i4Argc, const CHAR ** szArgv)
* Description : Set memory with order value
*   Parameter :
*       i4Argc    - IN    - The number of command arguements.
*       szArgv    - IN    - The arguement array list.
*   Return    : 1 failed, and 0 successed.
******************************************************************************/
static INT32 _CmdMemOrder(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 *pu1;
    UINT16 *pu2;
    UINT32 *pu4;
    UINT64 *pu8;
    UINT32 u4ByteUnit, u4Len, u4Step, i, u4Addr;
    UINT64 u8InitVal;

    ASSERT(szArgv!=NULL);
    if (i4Argc <= 5) {
        Printf("memorder\t\t Set memory order value.\tEx: memorder [addr] [bunit] [len] [step] [[ival]].\n");
        return 1;
    }

    u8InitVal = 0;
    if (i4Argc > 5) {
        u8InitVal = StrToInt(szArgv[5]);
    }
    u4Addr = StrToInt(szArgv[1]);
    u4ByteUnit = StrToInt(szArgv[2]);
    u4Len = StrToInt(szArgv[3]);
    u4Step = StrToInt(szArgv[4]);

    switch(u4ByteUnit) {
    case 1: SET_LOOP(pu1, UINT8, u4Addr, u8InitVal); break;
    case 2: SET_LOOP(pu2, UINT16, u4Addr, u8InitVal); break;
    case 4: SET_LOOP(pu4, UINT32, u4Addr, u8InitVal); break;
    case 8: SET_LOOP(pu8, UINT64, u4Addr, u8InitVal); break;
    default:
        Printf("bunit must be 1, 2, 4, or 8.\n");
        return 1;
    }

    return 0;
}
#endif /* CC_CLI */

//lint -restore

EXTERN void CHIP_Reboot(void);

static INT32 _CmdReboot(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    CHIP_Reboot();
    return 0;
}

static INT32 _CmdCalibratePLL(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Clk, u4NewClk, i, u4CalClk;

    UNUSED(szArgv);
    ASSERT(szArgv!=NULL);
    if (i4Argc < 2)
    {
        Printf("calpll [ps|vo|adc|apl1|apl2] [clock Hz]\n");
        return 1;
    }

    u4NewClk = (i4Argc >= 3) ? StrToInt(szArgv[2]) : 0;

    switch(szArgv[1][0])
    {
    case 'p': case 'P':
        u4Clk = (u4NewClk) ? (u4NewClk) : PSPLL_DEFAULT_CLOCK;
        Printf("Set PSPLL to %d Hz...\n", u4Clk);
//        VERIFY(BSP_Calibrate(CAL_SRC_PSPLL, u4Clk));
        // Delay a 1 ms
        for (i=0; i<30000; i++) { }
//        u4CalClk = BSP_GetDomainClock(CAL_SRC_PSPLL);
        Printf("PSPLL is %d Hz\n", u4CalClk);
        break;
    case 'v': case 'V':
        u4Clk = (u4NewClk) ? (u4NewClk) : VPLL_DEFAULT_CLOCK;
        Printf("Set VPLL to %d Hz...\n", u4Clk);
//        VERIFY(BSP_Calibrate(CAL_SRC_VOPLL, u4Clk));
        // Delay a 1 ms
        for (i=0; i<30000; i++) { }
//        u4CalClk = BSP_GetDomainClock(CAL_SRC_VOPLL);
        Printf("VOPLL is %d Hz\n", u4CalClk);
        break;
    case 'a': case 'A':
        if ((szArgv[1][1]=='d') || (szArgv[1][1]=='D'))
        {
            u4Clk = (u4NewClk) ? (u4NewClk) : ADCPLL_DEFAULT_CLOCK;
            Printf("Calibrate ADCPLL to %d Hz...\n", u4Clk);
//            VERIFY(BSP_Calibrate(CAL_SRC_ADCPLL, u4Clk));
            // Delay a 1 ms
            for (i=0; i<30000; i++) { }
//            u4CalClk = BSP_GetDomainClock(CAL_SRC_ADCPLL);
            Printf("ADCPLL is %d Hz\n", u4CalClk);
        }
        else if ((szArgv[1][3]=='1') || (szArgv[1][4]=='1'))
        {
            u4Clk = (u4NewClk) ? (u4NewClk) : APLL1_DEFAULT_CLOCK;
            Printf("Calibrate APLL1 to %d Hz...\n", u4Clk);
//            VERIFY(BSP_Calibrate(CAL_SRC_APLL1, u4Clk));
        }
        else if ((szArgv[1][3]=='2') || (szArgv[1][4]=='2'))
        {
            u4Clk = (u4NewClk) ? (u4NewClk) : APLL2_DEFAULT_CLOCK;
            Printf("Calibrate APLL2 to %d Hz...\n", u4Clk);
//            VERIFY(BSP_Calibrate(CAL_SRC_APLL2, u4Clk));
        }
        else
        {
            Printf("calpll [cpu|sys|ps|vo|adc|b2r|apl1|apl2] [clock Hz]\n");
            return 1;
        }
        break;
    default:
        Printf("calpll [cpu|sys|ps|vo|adc|b2r|apl1|apl2] [clock Hz]\n");
        return 1;
    }
    return 0;
}

static INT32 _CmdKBandPLL(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i, u4Band, u4Clock, u4CalClk;
    UINT32 u4Val, u4Reg, u4Bit, u4Shift;
    CAL_SRC_T eSel;

    UNUSED(szArgv);
    if (i4Argc < 2)
    {
        Printf("kpll [cpu|dm|sys|ps|vo|adc|b2r|apl1|apl2|hdmi] [band num]\n");
        return 1;
    }
    ASSERT(szArgv!=NULL);

    u4Band = 0xff; // default value.
    if (i4Argc > 2)
    {
        u4Band = StrToInt(szArgv[2]);
    }
    u4Reg = 0; // default no band
    u4Bit = 0;
    u4Shift = 0;

    switch(szArgv[1][0])
    {
    case 'c': case 'C':
        eSel = SRC_CPU_CLK;
        u4Clock = CPUPLL_DEFAULT_CLOCK;
        break;
    case 'd': case 'D':
        eSel = CAL_SRC_DMPLL;
        u4Clock = DMPLL_DEFAULT_CLOCK;
        break;
    case 's': case 'S':
        eSel = CAL_SRC_SYSPLL;
        u4Clock = SYSPLL_DEFAULT_CLOCK;
        break;
    case 'p': case 'P':
        eSel = CAL_SRC_PSPLL;
        u4Clock = PSPLL_DEFAULT_CLOCK;
        break;
    case 'v': case 'V':
        eSel = CAL_SRC_VOPLL;
        u4Clock = VPLL_DEFAULT_CLOCK;
        break;
    case 'h': case 'H':
        eSel = CAL_SRC_HDMIPLL;
        u4Clock = 0;
        break;
    case 'a': case 'A':
        if ((szArgv[1][1]=='d') || (szArgv[1][1]=='D'))
        {
            eSel = CAL_SRC_ADCPLL;
            u4Clock = ADCPLL_DEFAULT_CLOCK;
        }
        else if ((szArgv[1][3]=='1') || (szArgv[1][4]=='1'))
        {
            eSel = CAL_SRC_APLL1;
            u4Reg = REG_APLL1_CFG1;
            u4Bit = RG_APLL1_VCO_BAND;
            u4Shift = APLL1_VCO_BAND_SHIFT;
            u4Clock = APLL1_DEFAULT_CLOCK;
        }
        else if ((szArgv[1][3]=='2') || (szArgv[1][4]=='2'))
        {
            u4Reg = REG_APLL2_CFG1;
            u4Bit = RG_APLL2_VCO_BAND;
            u4Shift = APLL2_VCO_BAND_SHIFT;
            eSel = CAL_SRC_APLL2;
            u4Clock = APLL2_DEFAULT_CLOCK;
        }
        else
        {
            Printf("kpll [cpu|dm|sys|ps|vo|adc|b2r|apl1|apl2|hdmi] [band num]\n");
            return 1;
        }
        break;
    default:
        Printf("kpll [cpu|dm|sys|ps|vo|adc|b2r|apl1|apl2|hdmi] [band num]\n");
        return 1;
    }

    // Select band
    if ((u4Reg!=0) && (u4Band != 0xff))
    {
        u4Val = CKGEN_READ32(u4Reg);
        u4Val &= ~(u4Bit);
        u4Val |= (u4Bit & (u4Band << u4Shift));
        CKGEN_WRITE32(u4Reg, u4Val);
    }

    if (u4Reg != 0)
    {
        u4Val = CKGEN_READ32(u4Reg);
        u4Val &= (u4Bit);
        u4Band = (u4Val >> u4Shift);
    }

    // Delay a 1 ms
    for (i=0; i<30000; i++) { }

//    u4CalClk = BSP_GetDomainClock(eSel);

    // Print message
    if (u4Reg)
    {
        Printf("Band %d calibrate %d kHz Target:%d Hz\n", u4Band,
                u4CalClk, u4Clock);
    }
    else
    {
        Printf("Calibrate %d kHz Target:%d Hz\n", u4CalClk, u4Clock);
    }

    return 0;
}

static INT32 _CmdDoNothing(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    return 0;
}

static VER_FUNC _pfnVersion = NULL;

VER_FUNC DRVCLI_SetVerFunc(VER_FUNC pfnVer)
{
    VER_FUNC pfnRet;

    pfnRet = _pfnVersion;
    _pfnVersion = pfnVer;
    return pfnRet;
}

#ifndef BRDCST_IF
#define BRDCST_IF   "?????"
#endif /* BRDCST_IF */

static void ShowSystemInfo(void)
{
    BOOL fgICached, fgDCached, fgMMUed;
    UINT32 u4Mode, u4Turbo, u4Bus, u4SystemClock, u4Val;
    UINT32 u4RingOscClock, u4RingOscTick;
    CHAR *szBuildDate, *szBuildTime, *szBrdCstIf, *szDrvCust;
    UINT32 u4DdrClock, u4ColAddr;

    if (DRVCUST_InitQuery(eBuildDate, (UINT32 *)(void *)&szBuildDate)!=0)
    {
        szBuildDate="??";
    }
    if (DRVCUST_InitQuery(eBuildTime, (UINT32 *)(void *)&szBuildTime)!=0)
    {
        szBuildTime="??";
    }
    if (DRVCUST_InitQuery(eBrdCstIf, (UINT32 *)(void *)&szBrdCstIf)!=0)
    {
        szBrdCstIf="??";
    }
    if (DRVCUST_InitQuery(eDrvCust, (UINT32 *)(void *)&szDrvCust)!=0)
    {
        szDrvCust="??";
    }
    if (DRVCUST_InitQuery(eDdrClock, &u4DdrClock)!=0)
    {
        u4DdrClock = 0;
    }
    if (DRVCUST_InitQuery(eDramColAddr, &u4ColAddr)!=0)
    {
        u4ColAddr = 9999;
    }

    Printf("\n\nSystem information\n");
    Printf("------------------------------------------------------------\n");

    // Cache status
    fgICached = HalIsICacheEnabled();
    Printf("I-cache %s", fgICached ? "enabled" : "disabled");
    if (fgICached)
    {
        Printf(", size: %d bytes, cache line: %d bytes",
            HalGetICacheSize(), HalGetICacheLineSize());
    }
    fgDCached = HalIsDCacheEnabled();
    Printf("\nD-cache %s", fgDCached ? "enabled" : "disabled");
    if (fgDCached)
    {
        Printf(", size: %d bytes, cache line: %d bytes\n",
            HalGetDCacheSize(), HalGetDCacheLineSize());
    }

    // MMU status
    fgMMUed = HalIsMMUEnabled();
    Printf("\nMMU %s\n", fgMMUed ? "enabled" : "disabled");

    // DRAM configurations
    u4Mode = DRAM_READ32(REG_DRAM_MODE);
    u4Turbo = (u4Mode >> MODE_TURBO_SHIFT) & MODE_TURBO_MASK;
    u4Bus = (u4Mode >> MODE_BUS_SHIFT) & MODE_BUS_MASK;
    Printf("DRAM configuration: %d-bit, %s mode\n",
        ((u4Bus == MODE_BUS_64BIT) ? 64 : 32),
        ((u4Turbo == u4Turbo) ? "turbo" : "normal"));

    // System clock
//    u4SystemClock = BSP_GetDomainClock(SRC_BUS_CLK);
    Printf("System clock(Bus clock): %u\n", u4SystemClock);
//    Printf("System clock(CPU clock): %u\n", BSP_GetDomainClock(SRC_CPU_CLK));

    // Ring oscillator clock
    u4RingOscTick = BIM_GetOscillator();
    u4RingOscClock = (u4SystemClock / u4RingOscTick) * 4096;
    Printf("Ring oscillator ticks : %u\n", u4RingOscTick);
    Printf("Ring oscillator clock : %u (%u / %u * %u)\n", u4RingOscClock, u4SystemClock, u4RingOscTick, 4096);

//    u4SystemClock = BSP_GetDomainClock(CAL_SRC_DMPLL);
    u4Val = DRAM_READ32(REG_DRAM_DDR2CTRL);
    if (u4Val & 0xf0000000)
    {
        Printf("DRAM clock: %u (DDR-II %d %s)\n", (u4SystemClock >> 1),
            (u4SystemClock / 1000000),
            ((u4Val & 1) ? "Half frequency" : "Full frequency"));
    }
    else
    {
        Printf("DRAM clock: %u (%s)\n", (u4SystemClock >> 1),
            ((u4Val & 1) ? "DDR-I Half frequency" : "DDR-I Full frequency"));
    }

    Printf("BRDCST_IF: %s\t\t\t", szBrdCstIf);
    Printf("DRV_CUST: %s\n", szDrvCust);
    Printf("Default DDR clock:%d\t\tDefault ColAddr:%d\n", u4DdrClock, u4ColAddr);
//    Printf("PANEL: %s\n", GetCurrentPanelName());
    Printf("------------------------------------------------------------\n");
    Printf("Build Date: %s %s\n", szBuildDate, szBuildTime);
}

static INT32 _CmdVersion(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    ShowSystemInfo();
    if (_pfnVersion == NULL)
    {
        return 0;
    }
    _pfnVersion();
    return 0;
}

#ifndef CC_MINI_DRIVER
static INT32 _CmdSleep(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Val;

    UNUSED(szArgv);
    if(i4Argc < 2)
    {
        Printf("b.sleep [micro seconds, ps. must > 10]\n");
        return -1;
    }

    ASSERT(szArgv!=NULL);
    u4Val = StrToInt(szArgv[1]);
    if (u4Val > 10)
    {
        x_thread_delay(u4Val);
    }

    return 0;
}
#endif /* CC_MINI_DRIVER */

#ifdef CC_CLI
static INT32 _CmdSetCache(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Flag;
    INT32 i4Val;

    UNUSED(szArgv);
    if (i4Argc < 2)
    {
        Printf("sc [0|4|8|16], set I/D cache as 4/8/16 KB, 0 is disabled.");
        i4Val = -1;
    }
    else
    {
        ASSERT(szArgv!=NULL);
        i4Val = (INT32)StrToInt(szArgv[1]);
    }

    switch(i4Val)
    {
    case -1:    // do nothing, just report cache status.
        break;
    case 0: // disabled I/D cache
        HalDisableCaches();
        break;
    case 1: case 2: case 3: case 4: // Set I/D cache 4KB
        u4Flag = HalCriticalStart();
        HalDisableCaches();
        BIM_SetCacheCfg(0x303);
        HalEnableCaches();
        HalCriticalEnd(u4Flag);
        break;
    case 5: case 6: case 7: case 8: // Set I/D cache 8KB
        u4Flag = HalCriticalStart();
        HalDisableCaches();
        BIM_SetCacheCfg(0x404);
        HalEnableCaches();
        HalCriticalEnd(u4Flag);
        break;
    default:    // Set I/D cache 16KB
        u4Flag = HalCriticalStart();
        HalDisableCaches();
        BIM_SetCacheCfg(0x505);
        HalEnableCaches();
        HalCriticalEnd(u4Flag);
        break;
    }

    if (HalIsICacheEnabled())
    {
        Printf("I-Cache size is %d\n", HalGetICacheSize());
    }
    else
    {
        Printf("I-Cache is disabled\n");
    }

    if (HalIsDCacheEnabled())
    {
        Printf("D-Cache size is %d\n", HalGetDCacheSize());
    }
    else
    {
        Printf("D-Cache is disabled\n");
    }

    return 0;
}
#endif /* CC_CLI */

#ifdef CC_CLI
#define GetMiliSecond(x, mhz)       ((x) / (1000 * mhz))
#define GetNeroSecond(x, mhz)       ((((x) * 1000) / mhz) % 1000000)

CHAR *_aszPointString[] = {
    "loader, before set CPUPLL",
    "loader, after set CPUPLL",
    "loader, before memory shadow",
    "loader, before Remap",
    "loader, before __main",
    "loader, before __rt_entry",
    "loader, before Nucleus initial",
    "loader, after Nucleus initial",
    "loader, before create MainThread",
    "loader, start MainThread",
    "loader, before x_sys_start",
    "loader, before decompress image",
    "loader, after decompress image",
    "loader, before jump to app image",
    "App, before __main",
    "App, before __rt_entry",
    "App, before Nucleus initial",
    "App, after Nucleus initial",
    "App, before create MainThread",
    "App, start MainThread",
    "App, before x_sys_start",
    "App, before x_drv_init",
    "App, after x_drv_init",
};

static INT32 _CmdGetBootTime(INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    UINT32 i, u4Clock;
    UINT32 u4ClockSlots[32];

    UNUSED(i4Argc);
    UNUSED(szArgv);
    u4ClockSlots[1] = 4*(0xffffffff - g_pu4TIME_Array_Head[1]);
    u4ClockSlots[2] = 4*(g_pu4TIME_Array_Head[1] - g_pu4TIME_Array_Head[2]);
    for (i=3; i<24; i++)
    {
        u4ClockSlots[i] = g_pu4TIME_Array_Head[i-1] - g_pu4TIME_Array_Head[i];
    }
    u4Clock = 0;
    for (i=1; i<24; i++)
    {
        u4Clock += u4ClockSlots[i];
        Printf("%-3d.%06d - %s\n", GetMiliSecond(u4Clock, 81), GetNeroSecond(u4Clock, 81),
                        _aszPointString[i-1]);
    }
#else
    UNUSED(i4Argc);
    UNUSED(szArgv);
#endif
    return 0;
}
#endif

#ifdef CC_CLI
static INT32 _CmdSetInternalRtc(INT32 i4Argc, const CHAR ** szArgv)
{
    ASSERT(szArgv!=NULL);
    UNUSED(szArgv);
    if (i4Argc >= 2)
    {
        BIM_WRITE32(REG_RW_RTC_YEAR, StrToInt(szArgv[1]));
        BIM_WRITE32(REG_RW_RTC_MONTH, StrToInt(szArgv[2]));
        BIM_WRITE32(REG_RW_RTC_DAY, StrToInt(szArgv[3]));
        BIM_WRITE32(REG_RW_RTC_HOUR, StrToInt(szArgv[4]));
        BIM_WRITE32(REG_RW_RTC_MIN, StrToInt(szArgv[5]));
        BIM_WRITE32(REG_RW_RTC_SEC, StrToInt(szArgv[6]));
    }

    Printf("RTC now is 20%02x/%02x/%02x %02x:%02x:%02x\n",
        BIM_READ32(REG_RW_RTC_YEAR),
        BIM_READ32(REG_RW_RTC_MONTH),
        BIM_READ32(REG_RW_RTC_DAY),
        BIM_READ32(REG_RW_RTC_HOUR),
        BIM_READ32(REG_RW_RTC_MIN),
        BIM_READ32(REG_RW_RTC_SEC));

    return 0;
}

static INT32 _CmdClockPerInstruction(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MilliSec;
    UINT64 u8Instruct, u8Clocks;

    ASSERT(szArgv!=NULL);
    UNUSED(szArgv);
    UNUSED(i4Argc);
    u4MilliSec = 1000 * StrToInt(szArgv[1]);
    BIM_WRITE32(REG_RW_CPICFG, CPICFG_CLR);
    BIM_WRITE32(REG_RW_CPICFG, CPICFG_EN);
    x_thread_delay(u4MilliSec);
    BIM_WRITE32(REG_RW_CPICFG, 0);
    Printf("Instruct:0x%08x%08x Clock:0x%08x%08x\n",
        BIM_READ32(REG_RW_CPI1),
        BIM_READ32(REG_RW_CPI0),
        BIM_READ32(REG_RW_CPI3),
        BIM_READ32(REG_RW_CPI2));
    u8Instruct = BIM_READ32(REG_RW_CPI1);
    u8Instruct <<= 32;
    u8Instruct |= BIM_READ32(REG_RW_CPI0);
    u8Clocks = BIM_READ32(REG_RW_CPI3);
    u8Clocks <<= 32;
    u8Clocks |= BIM_READ32(REG_RW_CPI2);
    Printf("%d.%06d", (UINT32)(u8Clocks/u8Instruct),
            (UINT32)(((u8Clocks%u8Instruct)*100000)/u8Instruct));
    return 0;
}

static INT32 _CmdSetWatchdog(INT32 i4Argc, const CHAR ** szArgv)
{
    ASSERT(szArgv != NULL);
    UNUSED(szArgv);
    if (i4Argc > 1)
    {
        PDWNC_WRITE32(REG_RW_WATCHDOG_EN, StrToInt(szArgv[1]));
    }

    Printf("0x%08x\n", PDWNC_READ32(REG_RW_WATCHDOG_EN));
    return 0;
}

#endif /* CC_CLI */

#ifndef CC_5391_LOADER
#ifndef __KERNEL__
#ifndef CC_DRIVER_DEMO
EXTERN CHAR* x_sys_get_customer(VOID);
#endif /* CC_DRIVER_DEMO */

static INT32 _CmdCustomer(INT32 i4Argc, const CHAR ** szArgv)
{
    CHAR szBuf[64];
    INT32 i;

    UNUSED(i4Argc);
    UNUSED(szArgv);
#ifdef CC_DRIVER_DEMO
#ifdef CUSTOMER_NAME
    UNUSED(x_snprintf(szBuf, 64, "[%s]\n", CUSTOMER_NAME));
#else /* CUSTOMER_NAME */
    UNUSED(x_snprintf(szBuf, 64, "[cmtk / mtk]\n"));
#endif /* CUSTOMER_NAME */
#else /* CC_DRIVER_DEMO */
    UNUSED(x_snprintf(szBuf, 64, "[%s]\n", x_sys_get_customer()));
#endif /* CC_DRIVER_DEMO */

    for (i=0; i<64; i++)
    {
        if (szBuf[i]!=0)
        {
            SerPollPutChar((UINT8)szBuf[i]);
        }
        else
        {
            i = 64;
        }
    }

    return 0;
}
#endif /* __KERNEL__ */
#endif /* CC_5391_LOADER */

static INT32 _CmdSetOpt(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i;
    UINT32 u4Val;

    UNUSED(szArgv);
    ASSERT(szArgv != NULL);
    if ((i4Argc == 2) && (szArgv[1][0]=='1'))
    {
        UNUSED(BIM_EnableIrq(VECTOR_DRAMC));
        PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 1);
        u4Val = 1;
//        UNUSED(PDWNC_SetupPseudoWatchdogAssert(&u4Val));
    }
    else if ((i4Argc == 2) && (szArgv[1][0]=='0'))
    {
        UNUSED(BIM_DisableIrq(VECTOR_DRAMC));
        PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 0);
        u4Val = 0;
//        UNUSED(PDWNC_SetupPseudoWatchdogAssert(&u4Val));
    } 
    else if (i4Argc < 3)
    {
        Printf("%s -m [0|1] -w [0|1] -p [0|1]\n", szArgv[0]);
//        Printf("\t\t-m MemProtect:%d   -w WatchDog:%d     -p PseudoAssert:%d\n",
//            ((BIM_READ32(REG_RW_IRQEN) & IRQ_DRAMC) ? 1 : 0),
//            ((PDWNC_READ32(REG_RW_WATCHDOG_EN)) ? 1 : 0),
//            PDWNC_SetupPseudoWatchdogAssert(NULL));
        return 0;
    }

    for (i=1; i<(i4Argc-1); i++)
    {
        if (szArgv[i][0] != '-')
        {
            continue;
        }
        switch(szArgv[i][1]) {
        case 'M': case 'm':
            if (StrToInt(szArgv[i+1]))
            {
                UNUSED(BIM_EnableIrq(VECTOR_DRAMC));
            }
            else
            {
                UNUSED(BIM_DisableIrq(VECTOR_DRAMC));
            }
            break;
        case 'W': case 'w':
            if (StrToInt(szArgv[i+1]))
            {
                PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 1);
            }
            else
            {
                PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 0);
            }
            break;
        case 'P': case 'p':
            u4Val = StrToInt(szArgv[i+1]);
//            UNUSED(PDWNC_SetupPseudoWatchdogAssert(&u4Val));
            break;
        default:
            break;
        }
    }

//    Printf("\t\t-m MemProtect:%d   -w WatchDog:%d     -p PseudoAssert:%d\n",
//            ((BIM_READ32(REG_RW_IRQEN) & IRQ_DRAMC) ? 1 : 0),
//            ((PDWNC_READ32(REG_RW_WATCHDOG_EN)) ? 1 : 0),
//            PDWNC_SetupPseudoWatchdogAssert(NULL));
    return 0;
}

/******************************************************************************
* Variable      : cli "basic" command table
******************************************************************************/
//lint --e{64}
const CLI_EXEC_T _arBasicCmdTbl[] = {

#ifdef CC_CLI
    {
        "readb",                    //pszCmdStr
        "rb",
        _CmdMemReadByte,            //execution function
        NULL,
        "Memory read(byte)",
        CLI_ADMIN
    },

    {
        "readh",                    //pszCmdStr
        "rh",
        _CmdMemReadHalfWord,        //execution function
        NULL,
        "Memory read(half word)",
        CLI_ADMIN
    },

    {
        "writeb",                   //pszCmdStr
        "wb",
        _CmdMemWriteByte,           //execution function
        NULL,
        "Memory write(byte)",
        CLI_ADMIN
    },

    {
        "writeh",                   //pszCmdStr
        "wh",
        _CmdMemWriteHalfWord,       //execution function
        NULL,
        "Memory write(half word)",
        CLI_ADMIN
    },

    {
        "memcpy",                   //pszCmdStr
        "mc",
        _CmdMemCopy,                //execution function
        NULL,
        "Memory copy",
        CLI_ADMIN
    },

    {
        "memset",                   //pszCmdStr
        "ms",
        _CmdMemSet,                 //execution function
        NULL,
        "Memory set",
        CLI_ADMIN
    },

    {
        "memsetb",                  //pszCmdStr
        "msb",
        _CmdMemSetByte,             //execution function
        NULL,
        "Memory set(byte)",
        CLI_ADMIN
    },

    {
        "memorder",                 //pszCmdStr
        "mord",
        _CmdMemOrder,               //execution function
        NULL,
        "Memory order set, Ex: mord [addr] [bunit] [len] [step] [init val]",
        CLI_ADMIN
    },

    {
        "memcmp",                   //pszCmdStr
        "mcmp",
        _CmdMemCmp,             //execution function
        NULL,
        "Memory comparision, Ex: mcmp [addr1] [addr2] [byte-len]",
        CLI_ADMIN
    },

    {
        "memdump",                  //pszCmdStr
        "mdmp",
        _CmdMemDump,                //execution function
        NULL,
        "Memory dump, Ex: mord [addr] [bytelen]",
        CLI_ADMIN
    },

#endif /* CC_CLI */

    {
        "stop",                     //pszCmdStr
        NULL,
        _CmdStopUart,               //execution function
        NULL,
        "Stop RS232 transparent mode",
        CLI_GUEST
    },

    {
        "sv",
        NULL,
        _CmdSvDetect,
        NULL,
        "System mode detection",
        CLI_GUEST
    },

    {
        "version",
        "ver",
        _CmdVersion,
        NULL,
        "System version/build date",
        CLI_GUEST
    },

    {
        "reboot",
        NULL,
        _CmdReboot,
        NULL,
        "System reboot/restart",
        CLI_GUEST
    },

    {
        "calpll",
        "cp",
        _CmdCalibratePLL,
        NULL,
        "open loop, calpll [cpu|sys|ps|vo|adc|b2r|apl1|apl2] [clock Hz]",
        CLI_SUPERVISOR
    },

    {
        "getpllclk",
        "gpc",
        _CmdKBandPLL,
        NULL,
        "close loop, gpc [cpu|sys|ps|vo|adc|b2r|apl1|apl2] [[band num]]",
        CLI_GUEST
    },

#ifndef CC_MINI_DRIVER
    {
        "sleep",
        "slp",
        _CmdSleep,
        NULL,
        "sleep [micro seconds]",
        CLI_SUPERVISOR
    },
#endif /* CC_MINI_DRIVER */

#ifdef CC_CLI
    {
        "setcache",
        "sc",
        _CmdSetCache,
        NULL,
        "sc [0|4|8|16], set I/D cache as 4/8/16 KB, 0 is disabled.",
        CLI_SUPERVISOR
    },

    {
        "boottime",
        "bt",
        _CmdGetBootTime,
        NULL,
        "bt",
        CLI_SUPERVISOR
    },

    {
        "rtc",
        "rtc",
        _CmdSetInternalRtc,
        NULL,
        "rtc 0x[YY] 0x[MM] 0x[DD] 0x[hh] 0x[mm] 0x[ss]",
        CLI_SUPERVISOR
    },

    {
        "cpi",
        "cpi",
        _CmdClockPerInstruction,
        NULL,
        "cpi [period seconds]",
        CLI_SUPERVISOR
    },

    {
        "swd",
        "swd",
        _CmdSetWatchdog,
        NULL,
        "swd [0|1]",
        CLI_GUEST,
    },
#endif /* CC_CLI */

    {
        "setopt",                   //pszCmdStr
        "so",
        _CmdSetOpt,                 //execution function
        NULL,
        "Memory protect on/off, Watchdog on/off, Pseudo watchdog assert on/off.",
        CLI_GUEST
    },

    // last cli command record, NULL
    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};

const CLI_EXEC_T _ar0CmdTbl[] = {
    {
        "et",                       //pszCmdStr
        NULL,
        _CmdEnterUart,              //execution function
        NULL,
        "Enter RS232 transparent mode",
        CLI_GUEST
    },

    {
        "ft",                       //pszCmdStr
        NULL,
        _CmdFactoryUart,               //execution function
        NULL,
        "Set RS232 factory mode",
        CLI_GUEST
    },

    {
        "st",                       //pszCmdStr
        NULL,
        _CmdStopUart,               //execution function
        NULL,
        "Stop RS232 transparent mode",
        CLI_GUEST
    },

    // last cli command record, NULL
    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};

/******************************************************************************
* Variable      : cli default table
******************************************************************************/
const CLI_EXEC_T _arDefaultCmdTbl[] CLI_MAIN_COMMAND =
{
    {
        "read",                     //pszCmdStr
        "r",
        _CmdMemRead,                    //execution function
        NULL,
        "Memory read(word)",
        CLI_GUEST
    },

    {
        "write",                    //pszCmdStr
        "w",
        _CmdMemWrite,               //execution function
        NULL,
        "Memory write(word)",
        CLI_GUEST
    },

#ifndef CC_5391_LOADER
#ifndef __KERNEL__
    {
        "customer",                 //pszCmdStr
        "cust",
        _CmdCustomer,               //execution function
        NULL,
        "Get customer name",
        CLI_GUEST
    },
#endif /* __KERNEL__ */
#endif /* CC_5391_LOADER */

    {
        "basic_",                    //pszCmdStr
        "b",
        NULL,                       //execution function
        (CLI_EXEC_T *)_arBasicCmdTbl,
        "basic command",
        CLI_GUEST
    },

#ifdef __KERNEL__
    {
        "linuxmode",                //pszCmdStr
        "l",
        _CmdLinuxMode,              //execution function
        NULL,
        "Turn on/off linux mode",
        CLI_GUEST
    },
#endif

    {
        "mtktool",                  //pszCmdStr
        "0",
        _CmdEnterUart,              //execution function
        (CLI_EXEC_T *)_ar0CmdTbl,
        "mtktool command",
        CLI_GUEST
    },

    {
        "//",
        ";",
        _CmdDoNothing,
        NULL,
        "Line comment",
        CLI_SUPERVISOR
    },
#ifndef CC_5391_LOADER
    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
#endif /* CC_5391_LOADER */
};


const CLI_EXEC_T _arNullCmdTbl[] CLI_LAST_COMMAND =
{
    // last cli command record, NULL
    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};

#ifndef CC_5391_LOADER
/******************************************************************************
* Function      : CLI_GetDefaultCmdTbl
* Description   : retrun default command table
******************************************************************************/
CLI_EXEC_T* CLI_GetDefaultCmdTbl(void)
{
    UNUSED(_arNullCmdTbl);
    return (CLI_EXEC_T *)_arDefaultCmdTbl;
}
#endif /* !CC_5391_LOADER */

