/*
 * Copyright:
 * ----------------------------------------------------------------------------
 * This confidential and proprietary software may be used only as authorised 
 * by a licensing agreement from ARM Limited.
 *      (C) COPYRIGHT 2008-2009 ARM Limited, ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised copies and 
 * copies may only be made to the extent permitted by a licensing agreement
 * from ARM Limited.
 * ----------------------------------------------------------------------------
 * $LastChangedRevision: 2831 $
 * ----------------------------------------------------------------------------
 */

#ifndef TGT_MEMORY_MAP_H
#define TGT_MEMORY_MAP_H

#include "drvcust_if.h"
#include "c_model.h"

//#define LINUX_KERNEL_MEM_SIZE     0x08000000
/* ===========================================================================
    Physical Addresses
=========================================================================== */
#define PA_LOAD_ADDRESS         LINUX_KERNEL_MEM_SIZE

#if 0
#define PA_TABLE_BASE           PA_LOAD_ADDRESS
#define PA_TABLE_LEN            0x4000
#define PA_TABLE_END            (PA_TABLE_BASE + PA_TABLE_LEN)

#define PA_CODE_BASE            PA_TABLE_END
#define PA_CODE_LEN             0x40000 //256K
#define PA_CODE_END             (PA_CODE_BASE + PA_CODE_LEN)

#define PA_MON_CODE_BASE        PA_CODE_END
#define PA_MON_CODE_LEN         0x1000
#define PA_MON_CODE_END         (PA_MON_CODE_BASE + PA_MON_CODE_LEN)

#define PA_STACK_DATA_BASE      PA_MON_CODE_END
#define PA_STACK_DATA_LEN       0x4000
#define PA_STACK_DATA_END       (PA_STACK_DATA_BASE + PA_STACK_DATA_LEN)

#define PA_RWZI_DATA_BASE       PA_STACK_DATA_END
#define PA_RWZI_DATA_LEN        0x4000
#define PA_RWZI_DATA_END        (PA_RWZI_DATA_BASE + PA_RWZI_DATA_LEN)

#define PA_HEAP_DATA_BASE       PA_RWZI_DATA_END
#define PA_HEAP_DATA_LEN        0x4000
#define PA_HEAP_DATA_END        (PA_HEAP_DATA_BASE + PA_HEAP_DATA_LEN)

#define PA_MON_STACK_DATA_BASE  PA_HEAP_DATA_END
#define PA_MON_STACK_DATA_LEN   0x1000
#define PA_MON_STACK_DATA_END   (PA_MON_STACK_DATA_BASE+PA_MON_STACK_DATA_LEN)

#define PA_MEMORY_POOL_BASE     (PA_MON_STACK_DATA_END)
#define PA_MEMORY_POOL_END      (LINUX_KERNEL_MEM_SIZE + TRUSTZONE_MEM_SIZE)

#else

#define PA_TABLE_BASE           PA_LOAD_ADDRESS
#define PA_TABLE_LEN            0x4000
#define PA_TABLE_END            (PA_TABLE_BASE + PA_TABLE_LEN)

#define PA_CODE_BASE            PA_TABLE_END
#define PA_CODE_LEN             0x40000 //256K
#define PA_CODE_END             (PA_CODE_BASE + PA_CODE_LEN)

#define PA_STACK_DATA_BASE      PA_CODE_END
#define PA_STACK_DATA_LEN       0x4000
#define PA_STACK_DATA_END       (PA_STACK_DATA_BASE + PA_STACK_DATA_LEN)

#define PA_RWZI_DATA_BASE       PA_STACK_DATA_END
#define PA_RWZI_DATA_LEN        0x4000
#define PA_RWZI_DATA_END        (PA_RWZI_DATA_BASE + PA_RWZI_DATA_LEN)

#define PA_MON_CODE_BASE        PA_RWZI_DATA_END
#define PA_MON_CODE_LEN         0x1000
#define PA_MON_CODE_END         (PA_MON_CODE_BASE + PA_MON_CODE_LEN)

#define PA_MON_STACK_DATA_BASE  PA_MON_CODE_END
#define PA_MON_STACK_DATA_LEN   0x1000
#define PA_MON_STACK_DATA_END   (PA_MON_STACK_DATA_BASE+PA_MON_STACK_DATA_LEN)

#define PA_HEAP_DATA_BASE       PA_MON_STACK_DATA_END
#define PA_HEAP_DATA_END        (PA_LOAD_ADDRESS + TRUSTZONE_MEM_SIZE)
#define PA_HEAP_DATA_LEN        (PA_HEAP_DATA_END - PA_HEAP_DATA_BASE) //712K

#endif

/* ===========================================================================
    Virtual Addresses
=========================================================================== */
#define VA_LOAD_ADDRESS         LINUX_KERNEL_MEM_SIZE
#define PA_SUB_VA               (PA_LOAD_ADDRESS - VA_LOAD_ADDRESS)

#define VA_TABLE_BASE           (PA_TABLE_BASE - PA_SUB_VA)
#define VA_TABLE_END            (PA_TABLE_END  - PA_SUB_VA)

#define VA_CODE_BASE            (PA_CODE_BASE - PA_SUB_VA)
#define VA_CODE_END             (PA_CODE_END  - PA_SUB_VA)

#define VA_RWZI_DATA_BASE       (PA_RWZI_DATA_BASE - PA_SUB_VA)
#define VA_RWZI_DATA_END        (PA_RWZI_DATA_END  - PA_SUB_VA)

#define VA_HEAP_DATA_BASE       (PA_HEAP_DATA_BASE - PA_SUB_VA)
#define VA_HEAP_DATA_END        (PA_HEAP_DATA_END  - PA_SUB_VA)

#define VA_STACK_DATA_BASE      (PA_STACK_DATA_BASE - PA_SUB_VA)
#define VA_STACK_DATA_END       (PA_STACK_DATA_END  - PA_SUB_VA)

#define VA_MON_CODE_BASE        (PA_MON_CODE_BASE - PA_SUB_VA)
#define VA_MON_CODE_END         (PA_MON_CODE_END  - PA_SUB_VA)

#define VA_MON_STACK_DATA_BASE  (PA_MON_STACK_DATA_BASE - PA_SUB_VA)
#define VA_MON_STACK_DATA_END   (PA_MON_STACK_DATA_END  - PA_SUB_VA)

#define VA_WSM0_BASE            0x00800000

#define VA_WSM1_BASE            0x00900000

#define VA_WSM_BASE             VA_WSM0_BASE

/* ===========================================================================
    Page Table Addresses
=========================================================================== */
#define VA_TABLE_L1_BASE        (VA_TABLE_BASE+0x000)

#define VA_TABLE_L2_MAIN_BASE   (VA_TABLE_BASE+0x400)

#define VA_TABLE_L2_WSM0_BASE   (VA_TABLE_BASE+0x800)
#define VA_TABLE_L2_WSM1_BASE   (VA_TABLE_BASE+0xc00)

#endif /* TGT_MEMORY_MAP_H */
