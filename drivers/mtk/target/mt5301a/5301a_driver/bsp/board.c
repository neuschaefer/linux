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
 * $RCSfile: board.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file board.c
 *  board.c provides functions of board initialization, board detection, and
 *
 */

#ifndef CC_MTK_LOADER

#include "x_bim.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_serial.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_printf.h"
#include "x_lint.h"
#include "x_pdwnc.h"
#include "x_pinmux.h"
#include "drvcust_if.h"

LINT_EXT_HEADER_BEGIN

#include "x_rand.h"
#include "x_os.h"
#include "x_util.h"
#include "nucleus.h"

LINT_EXT_HEADER_END

#include "c_model.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define FPGA_SYSTEM_CLOCK			20000000	// 20M Hz system clock
#define TICKS_PER_SECOND			NU_PLUS_Ticks_Per_Second
#define SIGNAL_MSG_NUM              15
#define MIN_MEMORY_ALIGNMENT		8


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef void (* PFN_VOID)(void);


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

EXTERN UINT32 g_u4BootRemap;

EXTERN UINT32 HeapStart;
// EXTERN void* g_pvPageTable;

EXTERN UINT32 g_u4ArmLibHeapStart;
EXTERN UINT32 g_u4ArmLibHeapSize;
EXTERN UINT32 g_u4HeapStart;


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

EXTERN void Restore_System_Data(void);
EXTERN void INC_Initialize(void* first_available_memory, void(*pfnPrint)(const CHAR*));


// Exported functions for RVCT library
EXTERN void __rt_raise(INT32 signal, INT32 type);

EXTERN void _sys_exit(INT32 return_code);

EXTERN void __rt_entry(void);

EXTERN UINT32 __rt_heap_extend(UINT32 size, void* block);

// The return value of __rt_lib_init() is an __argv_argv struct actually. But
// here we don't use it
//
EXTERN void __rt_lib_init(UINT32 u4HeapBase, UINT32 u4HeapTop);
EXTERN void _BSP_Main(void);
// Note: Calling __rt_lib_init() causes semihosting violation if there is no
// main() existed!

// below two function is used in void _RvctLibInit(void)
EXTERN void _init_alloc(UINT32 u4MemBase, UINT32 u4MemTop);
EXTERN void _fp_init(void);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT32 fgAbortReboot = 1;

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//=====================================================================
// RVCT-dependent stuffs

// The following symbol has no effect except to cause a link-time error if
// a function that uses the semihosting SWI is included from the library.
// The linker error message is:
//
//	Error: L6200E: Symbol __semihosting_swi_guard multiply defined
//					(by use_semi.o and use_no_semi.o)
//
// To resolve the error, link with -verbose -errors out.txt, then search
// out.txt for __semihosting_swi_guard to identify functions using semihosting
// directly or indirectly.
//
#pragma import(__use_no_semihosting_swi)


//-----------------------------------------------------------------------------
// RVCT used functions
//-----------------------------------------------------------------------------

/*----------------------------------------------------------------------------
 * Function: __rt_raise
 *
 * Description:
 *      __rt_raise() can be called from numerous places within RVCT compiled
 *      code, thus must be re-implemented. Refer to signal.h in RCVT for
 *      signal definitions.
 *
 * Inputs:
 *      signal:
 *      type:
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
void __rt_raise(INT32 signal, INT32 type)
{
    CHAR* msgs[SIGNAL_MSG_NUM] =
    {
        "Unknown",                              // 0
        "Abort",                                // 1
        "Arithmetic exception",                 // 2
        "Illegal instruction",                  // 3
        "Attention request from user",          // 4
        "Bad memory access",                    // 5
        "Termination request",                  // 6
        "Stack overflow",                       // 7
        "Run-time redirection error",           // 8
        "Run-time memory error",                // 9
        "User signal 1",                        // 10
        "User signal 2",                        // 11
        "Pure virtual function called",         // 12
        "Misc exception from C++ library",      // 13
        "new or new[] cannot allocate memory"   // 14
    };

    CHAR* pMsg;

    // Flush Log data.
    UTIL_LogFlush();

    pMsg = ((signal < 0) || (signal >= SIGNAL_MSG_NUM)) ? msgs[0] :
        msgs[signal];

    Printf("\nSignal - %s (%d), type: %d\n\n", pMsg, signal, type);

    BSP_HaltSystem();
}


/*----------------------------------------------------------------------------
 * Function: _sys_exit
 *
 * Description:
 *      This function is redefined to avoid semi-hosting
 *
 * Inputs:
 *      return_code:
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
void _sys_exit(INT32 return_code)
{
	UNUSED(return_code);

    BSP_HaltSystem();
}


/*----------------------------------------------------------------------------
 * Function: __rt_entry
 *
 * Description:
 *      The starting point for a program using ARM C library. Called directly
 *      from __main().
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
void __rt_entry(void)
{
// The return value of __rt_lib_init() is an __argv_argv struct actually. But
// here we don't use it
//
// EXTERN void __rt_lib_init(UINT32 u4HeapBase, UINT32 u4HeapTop);
// EXTERN void _BSP_Main(void);

// Note: Calling __rt_lib_init() causes semihosting violation if there is no
// main() existed!
/*
    // Initialize C library and constructors
    __rt_lib_init(g_u4ArmLibHeapStart,
        g_u4ArmLibHeapStart + g_u4ArmLibHeapSize);
*/
    /* Here is tv image. */
    BIM_SetTimeLog(5);

    _BSP_Main();
}


/*----------------------------------------------------------------------------
 * Function: __rt_heap_extend
 *
 * Description:
 *      Called in __main() while initializing heap memory
 *
 * Inputs:
 *      size:
 *      block:
 *
 * Outputs: -
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/
UINT32 __rt_heap_extend(UINT32 size, void* block)
{
    UNUSED(size);
    UNUSED(block);

    return 0;   // Refuse all requests for extra heap space
}

#if 1
void __rt_SIGRTMEM(INT32 signal, INT32 type)
{
    UNUSED(signal);
    UNUSED(type);
}

void __rt_SIGFPE_inner(INT32 signal, INT32 type)
{
    UNUSED(signal);
    UNUSED(type);
}
#else
void _ttywrch()
{
}
#endif


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _RvctLibInit() Initialize RVCT library
 */
//-----------------------------------------------------------------------------
static void _RvctLibInit(void)
{
// EXTERN void _init_alloc(UINT32 u4MemBase, UINT32 u4MemTop);
// EXTERN void _fp_init(void);

    // Initialize heap area
    _init_alloc(g_u4ArmLibHeapStart, g_u4ArmLibHeapStart + g_u4ArmLibHeapSize);

    // Initialize floating-point library
    _fp_init();
}


//=====================================================================
// Board level initialization


//-----------------------------------------------------------------------------
/** _TimerInit() Initialize timer
 */
//-----------------------------------------------------------------------------
static void _TimerInit(void)
{
    UINT32 u4SysClock, u4TimerInterval, u4Val;

	// Reset timer 0 value
    BIM_WRITE32(REG_RW_TIMER0_LOW, 0);
    BIM_WRITE32(REG_RW_TIMER0_HIGH, 0);

    // Setup timer interrupt interval
    u4SysClock = BSP_GetSystemClock();
    u4TimerInterval = u4SysClock / TICKS_PER_SECOND;
    BIM_WRITE32(REG_RW_TIMER0_LLMT, (u4TimerInterval - 1));
    BIM_WRITE32(REG_RW_TIMER0_LOW, (u4TimerInterval - 1));
    BIM_WRITE32(REG_RW_TIMER0_HLMT, 0);

    // Enable timer 0 with auto-load
    u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Val |= (TMR0_CNTDWN_EN | TMR0_AUTOLD_EN);
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);

    // Enable timer interrupt
    VERIFY(BIM_EnableIrq(VECTOR_T0));
}


//-----------------------------------------------------------------------------
/** _BoardInit Board-related initializations
 */
//-----------------------------------------------------------------------------
static void _BoardInit(void)
{
    // Initialize RVCT library
    _RvctLibInit();

	// Initialize timer used for OS
	_TimerInit();

	// Init serial port
	SerInit();

    // Init random number generator
    srand(BIM_READ32(REG_RW_TIMER0_LOW));

    BSP_CkgenInit();
}


/*----------------------------------------------------------------------------
 * Function: _InitMMU
 *
 * Description:
 *      Initialize MMU
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static void _InitMMU(void)
{
    HalEnableCaches();
//    VERIFY(HalInitMMU((UINT32)&g_pvPageTable));
    VERIFY(HalInitMMU(BSP_GetMemSize() - 0x4000));
}


//-----------------------------------------------------------------------------
/** _PrintMessage() Print a message
 *  @param msg: The message
 */
//-----------------------------------------------------------------------------
static void _PrintMessage(const CHAR* msg)
{
    Printf(msg);
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#pragma arm

//-----------------------------------------------------------------------------
/** _BSP_Main() The entry point of BSP main routine
 */
//-----------------------------------------------------------------------------
void _BSP_Main(void)
{
    vIO32Write4B(PDWNC_WDT0, 0);//PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, 0);

    // Restore certain system data zeroed by __main()
    Restore_System_Data();

    // Initialize MMU, if comment this, no MMU and NO I-cache & D-cache.
    _InitMMU();

    // Board level initialization
    _BoardInit();

    // Call INC_Initialize() (in Nucleus library) to initialize OS, which will
    // then call to above Application_Initialize(). INC_Initialize() will
    // start scheduler, thus it will never return.
    INC_Initialize((void*)g_u4HeapStart, _PrintMessage);

    // Never goes here
    ASSERT(FALSE);
}

//-----------------------------------------------------------------------------
/** BSP_GetSystemClock() Get system clock
 *  @return The system clock
 */
//-----------------------------------------------------------------------------
UINT32 BSP_GetSystemClock(void)
{
    if (BSP_IsFPGA())
    {
        return FPGA_SYSTEM_CLOCK;
    }
    else
    {
        return GET_XTAL_CLK();
    }
}


//-----------------------------------------------------------------------------
/** BSP_HaltSystem() Halt the system
 */
//-----------------------------------------------------------------------------
void BSP_HaltSystem(void)
{
    UINT32 u4Loop;

    SerWaitTxBufClear();

    // Disable interrupts
    HalDisableIRQ();
    HalDisableFIQ();

    SerEnd();

#ifdef NDEBUG
    if (fgAbortReboot) { PDWNC_Reboot(); }
#else /* NDEBUG */
    PDWNC_WatchDogReboot();
#endif /* NDEBUG */

    u4Loop = 1;

    // Halt the system
    while (u4Loop)
    {
    }

    // Enable interrupts
    HalEnableIRQ();
    HalEnableFIQ();

    SerStart();
}


//-----------------------------------------------------------------------------
/** BSP_AllocateDmaMemory() Allocate memory that can be used as DMA buffer
 *  @param u4Size: The desired memory size
 *  @return The pointer of the allocated memory, or NULL if out of memory
 */
//-----------------------------------------------------------------------------
void* BSP_AllocDmaMemory(UINT32 u4Size)
{
    UINT32 u4CacheLineSize, u4AllocSize;
    void* pMem;
    UINT32* p;

    u4CacheLineSize = HalGetDCacheLineSize();

    // Allocate 2 * cache_line bytes more, to guarantee that the entire DMA
    // buffer will not share cache lines with others
    u4AllocSize = u4Size + (u4CacheLineSize * 2) + sizeof(UINT32);

    pMem = x_mem_alloc(u4AllocSize);
    if (pMem == NULL)
    {
        return pMem;
    }

    p = (UINT32*)((UINT32)pMem + u4CacheLineSize);
    *p = (UINT32)pMem;

    return (void*)((UINT32)pMem + u4CacheLineSize + sizeof(UINT32));
}


//-----------------------------------------------------------------------------
/** BSP_FreeDmaMemory() Free the allocated DMA memory
 *  @param pUser: The memory pointer to be freed
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL BSP_FreeDmaMemory(void* pUser)
{
    UINT32 u4CacheLineSize;
    UINT32* p;
    void* pMem;

    // Check if NULL
    if (pUser == NULL)
    {
        return TRUE;
    }

    // Check if the memory is allocated from BSP_AllocDmaMemory()
    u4CacheLineSize = HalGetDCacheLineSize();
    p = (UINT32*)((UINT32)pUser - sizeof(UINT32));
    pMem = (void*)((UINT32)p - u4CacheLineSize);
    if (*p != (UINT32)pMem)
    {
        return FALSE;
    }

    x_mem_free(pMem);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** BSP_AllocateAlignedDmaMemory Allocate memory that can be used as DMA buffer
 *  @param u4Size: The desired memory size
 *  @return: The pointer of the allocated memory, or NULL if out of memory
 */
//-----------------------------------------------------------------------------
void* BSP_AllocAlignedDmaMemory(UINT32 u4Size, UINT32 u4Align)
{
    UINT32 u4CacheLineSize, u4AllocSize;
    void* pMem;
    UINT32* p;

	// Adjust alignment
	if (u4Align < MIN_MEMORY_ALIGNMENT)
	{
		u4Align = MIN_MEMORY_ALIGNMENT;
	}
	u4Align &= ~(MIN_MEMORY_ALIGNMENT - 1);

    u4CacheLineSize = HalGetDCacheLineSize();

    // Allocate 2 * cache_line bytes more, to guarantee that the entire DMA
    // buffer will not share cache lines with others
    u4AllocSize = u4Size + (u4CacheLineSize * 2) + u4Align;

    pMem = x_mem_alloc(u4AllocSize);
    if (pMem == NULL)
    {
        return pMem;
    }

    p = (UINT32*)(((UINT32)pMem + u4CacheLineSize + u4Align) & ~(u4Align - 1));
    *(p - 1) = (UINT32)pMem;
    *(UINT32*)pMem = (UINT32)p;

    return (void*)p;
}


//-----------------------------------------------------------------------------
/** BSP_FreeAlignedDmaMemory() Free the allocated DMA memory
 *  @param pUser: The memory pointer to be freed
 *  @retval  TRUE: Succeed
 *  @retval  FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL BSP_FreeAlignedDmaMemory(void* pUser)
{
    UINT32* p;
    void* pMem;

    // Check if NULL
	if (pUser == NULL)
	{
		return TRUE;
	}

    // Check if the memory is allocated from BSP_AllocDmaMemory()
    p = (UINT32*)pUser;
    pMem = (void*)*(p - 1);
    if (*(UINT32*)pMem != (UINT32)p)
    {
    	return FALSE;
    }

    x_mem_free(pMem);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** BSP_GetFbmMemAddr()
 *  @return the frame buffer start address
 */
//-----------------------------------------------------------------------------
UINT32 BSP_GetFbmMemAddr()
{
    return (BSP_GetMemSize() - DRVCUST_OptGet(eFbmMemSize)) - 0x4000;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
UINT32 BSP_QuerySetAbortRebootFlag(UINT32 *pu4Flag)
{
    if (pu4Flag != NULL)
    {
        fgAbortReboot = *pu4Flag;
    }
    return fgAbortReboot;
}

#endif /* CC_MTK_LOADER */
