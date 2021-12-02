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
 * $RCSfile: appinit.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file appinit.c
 *  appinit.c defines functions for Nucleus to initialize the whold system.
 *  appinit.c provides Application_Initialize() for Nucleus to call.  Nucleus
 *  will iniailize all OS components and call Application_Initialize() to
 *  initialize the "application" which run on Nucleus.  It will enter the
 *  scheduler to start multi-threading scheduling.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#ifndef CC_MTK_LOADER

#include "drv_common.h"
#include "x_hal_5381.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_timer.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_lint.h"
#include "x_pdwnc.h"
#include "drvcust_if.h"

LINT_EXT_HEADER_BEGIN

#include "x_util.h"
#include "x_start_mini.h"
#include "nucleus.h"
#include <string.h>

LINT_EXT_HEADER_END

extern UINT32 IsBootVerbose(void);

//-----------------------------------------------------------------------------
// Config settings
//-----------------------------------------------------------------------------

#define NUCLEUS_MEMORY_LOG              0


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define SYS_MEM_NAME                    "SYSMEM"
#define SYS_MEM_MIN_SIZE                16

#define MAIN_TASK_NAME                  "Main"
#define MAIN_TASK_SLICE                 5
#define MAIN_TASK_STACK_SIZE            8192
#define MAIN_TASK_PRIORITY              32

#define DRAM_BASE_ADDR                  DRAM_B_BASE

//---------------------------------------------------------------------

#define FBM_ALIGNMENT_BOUNDARY          0x4000000           // 64M

#define OS_DRIVER_RESERVED_SIZE         0


// Reserved memory size. The reserved memory is used for
// 1. The OS driver. OS driver cannot access OSAI interface, thus it needs
//    to access the native OS interface
// 2. Display frame buffer. The display frame buffer cannot cross 64MB
//    boundary. Thus it is arranged to resident in the last 64M segment.
// 3. MT5371 reserved for NPTV.
// To acess the reserved memory (after initialing OSAI), use the functions
// BSP_AllocateReserved() and BSP_FreeReserved()
//
#define TOTAL_RESERVED_MEMORY_SIZE \
    (OS_DRIVER_RESERVED_SIZE + MAIN_TASK_STACK_SIZE + REAPER_STACK_SIZE + \
    MEMORY_ALIGNMENT + 256)

#define FIRST_THREAD_STACK              (0x2000)
#define DATA_MEM_POOL                   (0x80)
#define MMU_MEM_SIZE                    (0x4000)

// Maximum number of handles for handle library
//
#define MAX_NUM_HANDLES                 1024
#define ALIGN_4KB_MASK                  (0x0fffU)


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
EXTERN UINT32 BSP_GetMemSize(void);

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

EXTERN UINT32 g_u4BootRemap;

LINT_SAVE_AND_DISABLE
EXTERN UINT32 Image$$BOOT$$Base;
EXTERN UINT32 Image$$BSS$$ZI$$Limit;
LINT_RESTORE

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static NU_MEMORY_POOL  s_rSystemMemory;
static UINT32 s_u4HeapSize;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _BspSystemError() the only entry when error happens in appinit.c.
 *  @param msg CHAR pointer to a string indicating what's the error.
 */
//-----------------------------------------------------------------------------
static void _BspSystemError(CHAR* msg)
{
    Printf("\nError: %s\n", msg);

    for (;;)
    {
    }
}

//-----------------------------------------------------------------------------
/** _BspMainTask(), simply invoke the entry point of user program CMain
 *  @parm argc: Number of arguments
 *  @parm argv: The argument array
 */
//-----------------------------------------------------------------------------

static VOID _BspMainTask(UNSIGNED argc, VOID *argv)
{
    VOID* pv_mem_addr;
    SIZE_T z_mem_size;

    UNUSED(argc);
    UNUSED(argv);

    // Time measure point 10, time of the 1st thread starting to run.
//    BIM_SetTimeLog((g_u4BootRemap==0) ? 10 : 20);

    // Initialize HAL timer
    VERIFY(HAL_InitTimer());

    // read panel index from eeprom
//    ReadPanelIndexFromEeprom();

    // Make sure the entire reserved memory is in the last 64M segment
    ASSERT(TOTAL_RESERVED_MEMORY_SIZE <= FBM_ALIGNMENT_BOUNDARY);

    // Allocate memory for OSAI
    LINT_SAVE_AND_DISABLE;
//    if ((UINT32)_BspMainTask > 0x02000000U)
//    {
        //this is loader
//      z_mem_size = s_u4HeapSize - 0x10000;
//    }
//    else
//    {
        z_mem_size = (SIZE_T)((UINT32)s_u4HeapSize - (UINT32)TOTAL_RESERVED_MEMORY_SIZE);
//    }
    LINT_RESTORE;

    VERIFY(NU_Allocate_Aligned_Memory(&s_rSystemMemory, &pv_mem_addr,
        z_mem_size, MEMORY_ALIGNMENT, NU_NO_SUSPEND) == NU_SUCCESS);

    if (IsBootVerbose())
    {
        Printf("\n============================================\n");
        Printf("Dram Size: 0x%08x\n", BSP_GetMemSize());
LINT_SAVE_AND_DISABLE
        Printf("Memory for Image at 0x%08x(0x%08x):\n", &Image$$BOOT$$Base, &Image$$BSS$$ZI$$Limit);
LINT_RESTORE
        Printf("Memory for OSAI at 0x%08x(0x%08x)\n", (UINT32)pv_mem_addr,
            (UINT32)z_mem_size);
        Printf("Memory Reserved for ARM lib at 0x%08x(0x%08x)\n",
            (((BSP_GetMemSize() - MMU_MEM_SIZE) - DRVCUST_OptGet(eFbmMemSize)))
                - TOTAL_RESERVED_MEMORY_SIZE,
            TOTAL_RESERVED_MEMORY_SIZE);
        Printf("Memory Reserved for FBM at 0x%08x(0x%08x)\n",
            ((BSP_GetMemSize() - MMU_MEM_SIZE) - DRVCUST_OptGet(eFbmMemSize)),
            DRVCUST_OptGet(eFbmMemSize));
        Printf("Memory Reserved for MMU at 0x%08x(0x%08x)\n",
            (BSP_GetMemSize() - MMU_MEM_SIZE), MMU_MEM_SIZE);
    }

    // Time measure point 11, time to call x_sys_start.
//    BIM_SetTimeLog((g_u4BootRemap==0) ? 11 : 21);
    vIO32Write4B(PDWNC_WDT0, 0); //PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, 0);

    // Start application
    x_sys_start(pv_mem_addr, z_mem_size, NULL);

    // Initial PrintThread at util/util_log.c
    UNUSED(UTIL_LogThreadInit());
    // UTIL_Log("%s(%d) LogThreadInit(%d)\n", __FUNCTION__, __LINE__, i4Ret);

    // Setup watch dog if not loader and release version
    if (g_u4BootRemap)
    {
        PDWNC_CreateWatchDogThread();
    }

    // Note: "main" thread terminates here
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** The application initialization routine called by Nucleus
 *  @param first_available_memory: The start address of free memory
 */
//-----------------------------------------------------------------------------
void Application_Initialize(void *first_available_memory)
{
    static NU_TASK rTaskMain;

    void *pointer;

    // Time measure point 8, end of Nucleus initial function.
//    BIM_SetTimeLog((g_u4BootRemap==0) ? 8 : 18);

    // Initialize dynamic memory pool
    //
    s_u4HeapSize = (BSP_GetMemSize());
    s_u4HeapSize -= (((UINT32)first_available_memory) + ALIGN_4KB_MASK) & (~ALIGN_4KB_MASK);
    s_u4HeapSize -= FIRST_THREAD_STACK;
    s_u4HeapSize -= DATA_MEM_POOL;
    s_u4HeapSize -= MMU_MEM_SIZE;
#if 1
    if (s_u4HeapSize > DRVCUST_OptGet(eFbmMemSize))
    {
        s_u4HeapSize -= DRVCUST_OptGet(eFbmMemSize);
    }
    else
    {
        s_u4HeapSize -= FBM_MEM_CFG_MT5387_VERF;
        Printf("Dram Size: 0x%08x FBM: 0x%08x, cannot RUN FBM!!!\n", BSP_GetMemSize(), DRVCUST_OptGet(eFbmMemSize));
    }
#endif
    if (NU_Create_Memory_Pool(&s_rSystemMemory, SYS_MEM_NAME,
        first_available_memory, s_u4HeapSize, SYS_MEM_MIN_SIZE, NU_FIFO)
        != NU_SUCCESS)
    {
        _BspSystemError("Failed to initialize system memory pool!");
    }

    if (IsBootVerbose())
    {
        Printf("Nucleus Heap is at 0x%08x(0x%08x)\n",
            (UINT32)first_available_memory, s_u4HeapSize);
    }

    // Start the user program thread
    //
    if (NU_Allocate_Aligned_Memory(&s_rSystemMemory, &pointer,
        MAIN_TASK_STACK_SIZE, MEMORY_ALIGNMENT, NU_NO_SUSPEND) != NU_SUCCESS)
    {
        _BspSystemError("Failed to allocate memory!");
    }
    memset(pointer, 0, MAIN_TASK_STACK_SIZE);

    if (IsBootVerbose())
    {
        Printf("Main task stack is at 0x%08x (0x%08x)\n",
            (UINT32)pointer, MAIN_TASK_STACK_SIZE);
    }

    // Time measure point 9, time to create the 1st thread
//    BIM_SetTimeLog((g_u4BootRemap==0) ? 9 : 19);

    if (NU_Create_Task(&rTaskMain, MAIN_TASK_NAME, _BspMainTask, 0, NU_NULL,
        pointer, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, MAIN_TASK_SLICE,
        NU_PREEMPT, NU_START) != NU_SUCCESS)
    {
        _BspSystemError("Failed to create main thread!");
    }

    // Start the idle thread
    // ...
}

//-----------------------------------------------------------------------------
/** The memory allocator for non-OSAI, Nuclus reserved memory pool.
 *  @param u4Size the size of the required memory
 */
//-----------------------------------------------------------------------------
void* BSP_AllocateReserved(UINT32 u4Size)
{
    void* p;

    if (NU_Allocate_Aligned_Memory(&s_rSystemMemory, &p, u4Size,
        MEMORY_ALIGNMENT, (UNSIGNED)NU_NO_SUSPEND) != NU_SUCCESS)
    {
        return NULL;
    }
#if NUCLEUS_MEMORY_LOG
    if (IsBootVerbose())
    {
        Printf("Allocate 0x%08x(0x%08x)\n", (UINT32)p, u4Size);
    }
#endif

    return p;
}

//-----------------------------------------------------------------------------
/** The memory allocator for non-OSAI, Nuclus reserved memory pool with alignment.
 *  @param u4Size the size of the required memory
 *  @param u4Align the alignment requirement.
 */
//-----------------------------------------------------------------------------
void* BSP_AllocateReservedAlign(UINT32 u4Size, UINT32 u4Align)
{
    void* p;

    if (NU_Allocate_Aligned_Memory(&s_rSystemMemory, &p, u4Size,
        u4Align, (UNSIGNED)NU_NO_SUSPEND) != NU_SUCCESS)
    {
        return NULL;
    }

#if NUCLEUS_MEMORY_LOG
    if (IsBootVerbose())
    {
        Printf("AllocateAlign 0x%08x(0x%08x)\n", (UINT32)p, u4Size);
    }
#endif

    return p;
}

//-----------------------------------------------------------------------------
/** The memory free function for non-OSAI, Nuclus reserved memory block.
 *  @param p the memory pointer.
 */
//-----------------------------------------------------------------------------
void BSP_FreeReserved(void* p)
{
    STATUS i;
    i = NU_Deallocate_Memory(p);
    UNUSED(i);
}

//-----------------------------------------------------------------------------
/** Map a physical memory of reserved region to virtual address.
 *  @param  pv_Phys     Physical memory to map.
 *  @param  z_Size      Size of the physical memory (bytes).
 *  @return If success, the mapped virtual address. \n
 *          Otherwise, NULL.
 */
//-----------------------------------------------------------------------------
void* BSP_MapReservedMem(void *pv_Phys, UINT32 z_Size)
{
    UNUSED(z_Size);
    return pv_Phys;
}

#endif /* CC_MTK_LOADER */
