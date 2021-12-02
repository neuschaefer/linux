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
 * $RCSfile: exception.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file exception.c
 *  Exception handling
 */

#ifndef CC_5391_LOADER

#pragma arm

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_SUPPRESS_SYMBOL(18, Printf)

#include "x_hal_5381.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_serial.h"
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_stl_lib.h"

//lint --e{522} Expected void type, assignment, increment or decrement
LINT_SUPPRESS_SYMBOL(530, t_ap)
LINT_SUPPRESS_SYMBOL(613, unknown-name)
LINT_SUPPRESS_SYMBOL(718, __va_start)


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define NUM_GENERAL_REGISTERS           15

#define REG_SP                          13
#define REG_LR                          14

#define PSR_MODE_MASK                   0x1f
#define PSR_MODE_SHIFT                  0

#define MODE_USER                       0x10
#define MODE_FIQ                        0x11
#define MODE_IRQ                        0x12
#define MODE_SUPERVISOR                 0x13
#define MODE_ABORT                      0x17
#define MODE_UNDEFINED                  0x1b
#define MODE_SYSTEM                     0x1f

#define MIN_ADDR                        0x0

#define STACK_ALIGNMENT                 0x8
#define STACK_DUMP_SIZE                 0x60
#define WORD_PER_LINE                   4

// UART registers used in BSP_DoCoreDump
#define SERIAL_BASE         	(IO_VIRT + 0xc000)
#define SER_STATUS          	(*((volatile UINT8 *)(SERIAL_BASE + 4)))
#define SER_READ_ALLOW      	0x01
#define SER_WRITE_ALLOW    	 	0x02
#define UART0_STATUS            0x004
#define UART_READ32(offset)	    IO_READ32(RS232_BASE, (offset))
#define UART0_DATA              0x000

// parameter in BSP_DoCoreDump
// #define COREDUMP_DEBUG 1
// #define SZ_MIN_DRAM (0x04000000) /* 64 MB */
// #define SZ_MIN_DRAM (0x00400000) /* 4 MB */
#define SZ_MIN_DRAM (g_u4DramSize)
#define SZ_BLK_XFER 0x00000400


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// Note: The following definition of structure CPU_CONTEXT_T should be
// identical to the definition in int.s
//
typedef struct
{
    UINT32 au4Gprs[NUM_GENERAL_REGISTERS];
    UINT32 u4Cpsr;
    UINT32 u4Spsr;
    UINT32 u4Far;
    UINT32 u4IFsr;
    UINT32 u4DFsr;
    UINT32 u4Elr;
    UINT32 fgDataAbort;
} CPU_CONTEXT_T;

// the type of printf function
typedef INT32 (* PFN_PRINTF)(const CHAR *format, ...);


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

EXTERN UINT32 g_u4DramSize;
EXTERN UINT32 System_Stack_V;
EXTERN UINT32 System_Limit_V;
EXTERN UINT32 BSS_End_Ptr;
EXTERN CPU_CONTEXT_T g_SaveCPUContext;


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

EXTERN void BSP_DoCoreDump(const CPU_CONTEXT_T* prCtx);
EXTERN void BSP_DumpContext(const CPU_CONTEXT_T* prCtx);
EXTERN void BSP_ShowReg(void);
EXTERN CPU_CONTEXT_T* INT_FillSaveArea(void);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
// used in _BspPrintf4Byte()
#ifndef CC_MINI_DRIVER
// static INT32 _i4Cnt = 0;
// static UINT32 _u4Data = 0;
// static CHAR _szBuf[256];
#endif /* CC_MINI_DRIVER */

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _BspAlign Return the largest aligned address equal to or less than the \n
 *              given address
 *  @param u4Addr: The given address
 *  @param u4Alignment: The alignment
 *  @return The aligned address
 */
//-----------------------------------------------------------------------------
static UINT32 _BspAlign(UINT32 u4Addr, UINT32 u4Alignment)
{
	UINT32 u4Unaligned;

	if (u4Alignment <= 1)
	{
		return u4Addr;
    }

	u4Unaligned = u4Addr % u4Alignment;
	if (u4Unaligned != 0)
	{
		u4Addr += u4Alignment - u4Unaligned;
    }

	return u4Addr;
}

//-----------------------------------------------------------------------------
/** _BspIsAligned() Check if the given address is aligned to the given \n
 *                  alignment
 *  @param u4Addr: The address to be checked
 *  @param u4Alignment: The given alignment
 *  @retval TRUE: Yes
 *  @retval FALSE: No
 */
//-----------------------------------------------------------------------------
static BOOL _BspIsAligned(UINT32 u4Addr, UINT32 u4Alignment)
{
	return u4Addr == _BspAlign(u4Addr, u4Alignment);
}


//-----------------------------------------------------------------------------
/** _BspModeToStr() Map CPU mode to readable string
 *  @param prCtx: The CPU context
 *  @param u4Mode: The given mode
 *  @return The translated string
 */
//-----------------------------------------------------------------------------
static const CHAR* _BspModeToStr(const CPU_CONTEXT_T* prCtx, UINT32 u4Mode)
{
    const CHAR* szStr;

    ASSERT(prCtx != NULL);

    switch (u4Mode)
    {
    case MODE_USER:
        szStr = "USER";
        break;

    case MODE_FIQ:
        szStr = "FIQ";
        break;

    case MODE_IRQ:
        szStr = "IRQ";
        break;

    case MODE_SUPERVISOR:
        szStr = "SUPERVISOR";
        break;

    case MODE_ABORT:
        if (prCtx->fgDataAbort)
        {
            szStr = "DATA ABORT";
        }
        else
        {
            szStr = "PREFETCH ABORT";
        }
        break;

    case MODE_UNDEFINED:
        szStr = "UNDEFINIED";
        break;

    case MODE_SYSTEM:
        szStr = "SYSTEM";
        break;

    default:
        szStr = "UNKNOWN";
        break;
    }

    return szStr;
}


//-----------------------------------------------------------------------------
/** _BspGetMode() Get CPU mode from a given value of PSR
 *  @param u4Psr: The PSR value
 *  @return The CPU mode
 *---------------------------------------------------------------------------*/
//-----------------------------------------------------------------------------
static UINT32 _BspGetMode(UINT32 u4Psr)
{
    return (u4Psr >> PSR_MODE_SHIFT) & PSR_MODE_MASK;
}


//-----------------------------------------------------------------------------
/** _BspIsValidAddress() Check if an address is a valid DRAM address
 *  @param u4Addr: The address to be checked
 *  @retval TRUE: Valid address
 *  @retval FALSE: Invalid address
 */
//-----------------------------------------------------------------------------
static BOOL _BspIsValidAddress(UINT32 u4Addr)
{
    return (u4Addr < (MIN_ADDR + g_u4DramSize));
}


//-----------------------------------------------------------------------------
/** _BspIsValidStackPointer() Check if a given address can be a valid stack \n
 *                              pointer
 *  @param u4Addr: The address to be checked
 *  @retval TRUE: Valid
 *  @retval FALSE: Invalid
 */
//-----------------------------------------------------------------------------
static BOOL _BspIsValidStackPointer(UINT32 u4Addr)
{
    return _BspIsValidAddress(u4Addr) && _BspIsAligned(u4Addr, STACK_ALIGNMENT);
}

//-----------------------------------------------------------------------------
/** _BspDumpMemory() Dump memory at a given address and size
 *  @param u4Addr: The address
 *  @param i4Size: The size
 *  @param pfnPrintf: The function to output.
 *  @param fgComma: if true, Add comma after carriage return.
 */
//-----------------------------------------------------------------------------
static void _BspDumpMemory(UINT32 u4Addr, INT32 i4Size,
                            PFN_PRINTF pfnPrintf, INT32 fgComma)
{
    UINT32* p;
    INT32 i;
/*
    if (i4Size > u4Addr)
    {
        i4Size = i4Size - u4Addr;
    }
*/
    ASSERT(pfnPrintf != NULL);
    p = (UINT32*)u4Addr;
    while (i4Size > 0)
    {
        pfnPrintf("    %08x: ", (UINT32)p);
        for (i = 0; i < WORD_PER_LINE; i++)
        {
            if (i < (i4Size / WORD_PER_LINE))
            {
                pfnPrintf("%08x ", p[i]);
            }
            else
            {
                pfnPrintf("         ");
            }
        }
        if (fgComma)
        {
            pfnPrintf("\n;");
        }
        else
        {
            pfnPrintf("\n");
        }
        i4Size -= 4 * WORD_PER_LINE;
        p += WORD_PER_LINE;
    }
}

//-----------------------------------------------------------------------------
/** _BspPrintfDump() Dump information when assertion
 *  @param prCtx the pointer to an CPU_CONTEXT_T structure.
 *  @param pfnPrintf The function to output.
 */
//-----------------------------------------------------------------------------
static void _BspPrintfDump(const CPU_CONTEXT_T* prCtx,
                        PFN_PRINTF pfnPrintf)
{
    UINT32 u4PrevMode, u4CurMode, u4InstOffset, u4Sp, i;

    ASSERT(prCtx != NULL);
    ASSERT(pfnPrintf != NULL);

    // Get previous mode and exception mode
    u4PrevMode = _BspGetMode(prCtx->u4Spsr);
    u4CurMode = _BspGetMode(prCtx->u4Cpsr);

    pfnPrintf("\n\n========================== EXCEPTION ");
    pfnPrintf("==========================\n\n");
    pfnPrintf("Exception type: %s\n", _BspModeToStr(prCtx, u4CurMode));
    pfnPrintf("Previous mode: %s\n", _BspModeToStr(prCtx, u4PrevMode));

    // Dump general-purpose registers
    pfnPrintf("\nGeneral registers:\n\n");
    for (i = 0; i < NUM_GENERAL_REGISTERS; i += 4)
    {
        UINT32 j;

        for (j = 0; j < 4; j++)
        {
            UINT32 u4Reg;
            CHAR buf[16];

            u4Reg = i + j;
            if (u4Reg >= NUM_GENERAL_REGISTERS)
            {
                break;
            }
            SPrintf(buf, "R%u", u4Reg);
            pfnPrintf("  %4s: 0x%08x", buf, prCtx->au4Gprs[u4Reg]);
        }
        pfnPrintf("\n");
    }

    pfnPrintf("\nSpecial registers:\n");

    // Dump PSR
    pfnPrintf("\n    Program Status Word: 0x%08x\n", prCtx->u4Spsr);

    pfnPrintf("DFSR: 0x%08x     IFSR: 0x%08x\n", prCtx->u4DFsr, prCtx->u4IFsr);

    // Determine fault instruction
    u4InstOffset = 0;
    if (u4CurMode == MODE_ABORT)
    {
        if (prCtx->fgDataAbort)
        {
            u4InstOffset = 8;       // Data abort
        }
        else
        {
            u4InstOffset = 4;       // Prefetch abort
        }
    }

    // Dump fault instruction and address
    pfnPrintf("    Fault instruction: 0x%08x", prCtx->u4Elr - u4InstOffset);
    if (prCtx->fgDataAbort)
    {
        pfnPrintf(", fault address: 0x%08x", prCtx->u4Far);
    }

    // Dump stack
    pfnPrintf("\n\nInitial Stack: 0x%08x, 8KB-SVC, 4KB-IRQ, 2KB-FIQ\n", BSS_End_Ptr);
    pfnPrintf("System Limit: 0x%08x Stack:0x%08x\n", System_Limit_V, System_Stack_V);
    pfnPrintf("CPU Context:0x%08x\n", &g_SaveCPUContext);
    u4Sp = prCtx->au4Gprs[REG_SP];
    pfnPrintf("Stack: 0x%08x\n\n", u4Sp);
    if (_BspIsValidStackPointer(u4Sp))
    {
        _BspDumpMemory(u4Sp, STACK_DUMP_SIZE, pfnPrintf, 0);
    }
    else
    {
        pfnPrintf("Stack pointer is not a valid address!\n");
    }

    pfnPrintf("\nDo not input terminal - System halted\n\n");
}

//-----------------------------------------------------------------------------
/** _BspCmmRegDump() Dump coredump used .cmm data when assertion
 *  @param prCtx the pointer to an CPU_CONTEXT_T structure.
 *  @param pfnPrintf The function to output.
 */
//-----------------------------------------------------------------------------
static void _BspCmmRegDump(const CPU_CONTEXT_T* prCtx,
                        PFN_PRINTF pfnPrintf)
{
    UINT32 u4PrevMode, u4CurMode, u4InstOffset, u4Sp, i;

    ASSERT(prCtx != NULL);
    ASSERT(pfnPrintf != NULL);

    // Get previous mode and exception mode
    u4PrevMode = _BspGetMode(prCtx->u4Spsr);
    u4CurMode = _BspGetMode(prCtx->u4Cpsr);

    pfnPrintf("\n\n; ========================== EXCEPTION ");
    pfnPrintf("==========================\n\n");
    pfnPrintf("; Exception type: %s\n", _BspModeToStr(prCtx, u4CurMode));
    pfnPrintf("; Previous mode: %s\n", _BspModeToStr(prCtx, u4PrevMode));

    // Dump general-purpose registers
    pfnPrintf("\n; General registers:\n\n");
    for (i = 0; i < NUM_GENERAL_REGISTERS; i++)
    {
        pfnPrintf("R.S R%u 0x%08x\n", i, prCtx->au4Gprs[i]);
    }

    pfnPrintf("R.S PC 0x%08x\n", prCtx->au4Gprs[i-1]);
    pfnPrintf("R.S CPSR 0x%08x\n", prCtx->u4Cpsr);
    pfnPrintf("R.S SPSR 0x%08x\n", prCtx->u4Spsr);

    pfnPrintf("; DFSR: 0x%08x     IFSR: 0x%08x\n", prCtx->u4DFsr, prCtx->u4IFsr);
//    pfnPrintf("\nd.load.binary memorydump.dram\n");
//    pfnPrintf("\nd.load mt5381_demo_dbg /nocode\n");
//    pfnPrintf("\ny.spath.srd ../../middleware ");
//    pfnPrintf("../../target/mt5381 ../../custom/dev\n");

    pfnPrintf("\n; Special registers:\n");

    // Dump PSR
    pfnPrintf("\n;    Program Status Word: 0x%08x\n", prCtx->u4Spsr);

    // Determine fault instruction
    u4InstOffset = 0;
    if (u4CurMode == MODE_ABORT)
    {
        if (prCtx->fgDataAbort)
        {
            u4InstOffset = 8;       // Data abort
        }
        else
        {
            u4InstOffset = 4;       // Prefetch abort
        }
    }

    // Dump fault instruction and address
    pfnPrintf(";    Fault instruction: 0x%08x", prCtx->u4Elr - u4InstOffset);
    if (prCtx->fgDataAbort)
    {
        pfnPrintf(", fault address: 0x%08x", prCtx->u4Far);
    }

    // Dump stack
    u4Sp = prCtx->au4Gprs[REG_SP];
    pfnPrintf("\n\n; Initial Stack: 0x%08x, 8KB-SVC, 4KB-IRQ, 2KB-FIQ\n", BSS_End_Ptr);
    pfnPrintf("; System Limit: 0x%08x Stack:0x%08x\n", System_Limit_V, System_Stack_V);
    pfnPrintf("; CPU Context:0x%08x\n", &g_SaveCPUContext);
    pfnPrintf("; Stack: 0x%08x\n\n;", u4Sp);
    if (_BspIsValidStackPointer(u4Sp))
    {
        _BspDumpMemory(u4Sp, STACK_DUMP_SIZE, pfnPrintf, 1);
    }
    else
    {
        pfnPrintf("; Stack pointer is not a valid address!\n");
    }
    pfnPrintf("                                         \n");
}

#if 0
//-----------------------------------------------------------------------------
/** _BspPrint4Bytes() Print string into a 4bytes output regiser
 *  @param szBuf the string to output
 */
//-----------------------------------------------------------------------------
static void _BspPrint4Bytes(const CHAR *szBuf)
{
    UINT8 *pch;

    ASSERT(szBuf!=NULL);
    for (pch = (UINT8 *)szBuf; *pch!=0; pch++)
    {
        _u4Data = (_u4Data << 8) | *pch;
        _i4Cnt++;
        if (_i4Cnt == 4)
        {
            _i4Cnt = 0;
            while ((SER_STATUS & SER_WRITE_ALLOW) != SER_WRITE_ALLOW) { }
            (*((volatile UINT32 *)(SERIAL_BASE + 0))) = _u4Data;
        }

        if (*pch=='\n')
        {
            _u4Data = (_u4Data << 8) | '\r';
            _i4Cnt++;
            if (_i4Cnt == 4)
            {
                _i4Cnt = 0;
                while ((SER_STATUS & SER_WRITE_ALLOW) != SER_WRITE_ALLOW) { }
                (*((volatile UINT32 *)(SERIAL_BASE + 0))) = _u4Data;
            }
        }
    }
}

//-----------------------------------------------------------------------------
/** _PrintLast() Print the data not in 4 bytes.
 */
//-----------------------------------------------------------------------------
static void _PrintLast()
{
    for (; _i4Cnt < 4; _i4Cnt++)
    {
        _u4Data = (_u4Data << 8);
    }

    while ((SER_STATUS & SER_WRITE_ALLOW) != SER_WRITE_ALLOW) { }
    (*((volatile UINT32 *)(SERIAL_BASE + 0))) = _u4Data;
    _i4Cnt = 0;
    _u4Data = 0;
}

//-----------------------------------------------------------------------------
/** _BspPrintf4Byte() Printf format function to put string into 4 byte register.
 *  @param format the printf format string with ...
 */
//-----------------------------------------------------------------------------
static INT32 _BspPrintf4Byte(const CHAR *format, ...)
{
    VA_LIST t_ap;
    INT32 i4Len;

    VA_START(t_ap, format);
    i4Len = x_vsprintf(_szBuf, format, t_ap);
    VA_END(t_ap);
    _BspPrint4Bytes(_szBuf);
    return i4Len;
}

//-----------------------------------------------------------------------------
/** _BspSwap4Bytes() Print the data not in 4 bytes.
 */
//-----------------------------------------------------------------------------
static UINT32 _BspSwap4Bytes(UINT32 u4In)
{
    UINT32 u4Ret;

    u4Ret = (u4In >> 24);
    u4Ret |= ((u4In >> 16) & 0x0ff) << 8;
    u4Ret |= ((u4In >>  8) & 0x0ff) << 16;
    u4Ret |= (u4In << 24);
    return u4Ret;
}
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** BSP_DumpContext() Dump the given CPU context. This function is called \n
 *                    mainly from exception handlers.
 *  @param prCtx: The CPU context
 */
//-----------------------------------------------------------------------------
void BSP_DoCoreDump(const CPU_CONTEXT_T* prCtx)
{
    UINT32 u4IrqEn, u4FiqEn, u4DramSize;
    UINT32 u4Cmd;
    INT32 i4Loop = 1;

    /* Set to normal and wait command. */
    SerWaitTxBufClear();
    SerEnd();

    u4IrqEn = BIM_REG32(REG_IRQEN);
    u4FiqEn = BIM_REG32(REG_FIQEN);
    BIM_REG32(REG_IRQEN) = 0;
    BIM_REG32(REG_FIQEN) = 0;

    u4DramSize = g_u4DramSize;
    if (u4DramSize > (SZ_MIN_DRAM)) {
        u4DramSize = (SZ_MIN_DRAM);
    }

    do {
        if ((UART_READ32(UART0_STATUS) & SER_READ_ALLOW) == SER_READ_ALLOW) {
            u4Cmd = UART_READ32(UART0_DATA);
            switch(u4Cmd) {
            case '0': /* 'RDRG' to dump register */
                if (prCtx == NULL) {
                    prCtx = INT_FillSaveArea();
                }
                SerStart();
                _BspCmmRegDump(prCtx, (PFN_PRINTF)Printf);
                SerEnd();
                break;
            case 'S': /* 'SIZE' to get DRAM size. */
                SerStart();
                (*((volatile UINT32 *)(SERIAL_BASE + 0))) = u4DramSize;
                SerEnd();
                break;
            default:
                break;
            }
        }
    } while (i4Loop);

    BIM_REG32(REG_IRQEN) = u4IrqEn;
    BIM_REG32(REG_FIQEN) = u4FiqEn;
    SerStart();
}

//-----------------------------------------------------------------------------
/** BSP_DumpContext() Dump the given CPU context. This function is called \n
 *      mainly from exception handlers.
 *  @param prCtx: The CPU context
 */
//-----------------------------------------------------------------------------
void BSP_DumpContext(const CPU_CONTEXT_T* prCtx)
{
    // text dump information.
    _BspPrintfDump(prCtx, (PFN_PRINTF)Printf);
    // if watchdog enable, reboot system.
#ifdef NDEBUG
    if (BSP_QuerySetAbortRebootFlag(NULL)) { PDWNC_Reboot(); }
#else /* NDEBUG */
    PDWNC_WatchDogReboot();
#endif /* NDEBUG */
    // if watchdog not enable, wait for negociation.
    BSP_DoCoreDump(prCtx);
}

//-----------------------------------------------------------------------------
/** BSP_ShowReg() Dump the current CPU context.
 */
//-----------------------------------------------------------------------------
void BSP_ShowReg()
{
    CPU_CONTEXT_T* prCtx;

    prCtx = INT_FillSaveArea();
    _BspPrintfDump(prCtx, (PFN_PRINTF)Printf);
}

#endif /* CC_5391_LOADER */

