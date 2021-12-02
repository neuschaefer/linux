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

#include "x_typedef.h"
#include "x_assert.h"
#include "mu_mem.h"

#ifndef NDEBUG
//#define MEM_DEBUG
#endif    


#define MEMPOOL_SIZE_20  ((UINT32)20)
#define MEMPOOL_SIZE_48  ((UINT32)48)
#define MEMPOOL_SIZE_96  ((UINT32)96)
#define MEMPOOL_SIZE_180  ((UINT32)180)
#define MEMPOOL_SIZE_300  ((UINT32)300)
#define MEMPOOL_SIZE_420  ((UINT32)420)
#define MEMPOOL_SIZE_600  ((UINT32)600)
#define MEMPOOL_SIZE_800  ((UINT32)800)
#define MEMPOOL_SIZE_1200  ((UINT32)1200)

#define MEMPOOL_NO_20  ((UINT32)20)
#define MEMPOOL_NO_48  ((UINT32)20)
#define MEMPOOL_NO_96  ((UINT32)8)
#define MEMPOOL_NO_180  ((UINT32)8)
#define MEMPOOL_NO_300  ((UINT32)12)
#define MEMPOOL_NO_420  ((UINT32)12)
#define MEMPOOL_NO_600  ((UINT32)1)
#define MEMPOOL_NO_800  ((UINT32)1)
#define MEMPOOL_NO_1200  ((UINT32)4)

#define MEMPOOL_IDX_20  ((UINT32)0)
#define MEMPOOL_IDX_48  (MEMPOOL_IDX_20 + MEMPOOL_NO_20)
#define MEMPOOL_IDX_96  (MEMPOOL_IDX_48 + MEMPOOL_NO_48)
#define MEMPOOL_IDX_180  (MEMPOOL_IDX_96 + MEMPOOL_NO_96)
#define MEMPOOL_IDX_300  (MEMPOOL_IDX_180 + MEMPOOL_NO_180)
#define MEMPOOL_IDX_420  (MEMPOOL_IDX_300 + MEMPOOL_NO_300)
#define MEMPOOL_IDX_600  (MEMPOOL_IDX_420 + MEMPOOL_NO_420)
#define MEMPOOL_IDX_800  (MEMPOOL_IDX_600 + MEMPOOL_NO_600)
#define MEMPOOL_IDX_1200  (MEMPOOL_IDX_800 + MEMPOOL_NO_800)
#define MEMPOOLNUM           (MEMPOOL_IDX_1200 + MEMPOOL_NO_1200)

#define MEMPOOL_SUM_20     ((UINT32)0)
#define MEMPOOL_SUM_48  (MEMPOOL_SUM_20 + (MEMPOOL_NO_20 * MEMPOOL_SIZE_20))
#define MEMPOOL_SUM_96  (MEMPOOL_SUM_48 + (MEMPOOL_NO_48 * MEMPOOL_SIZE_48))
#define MEMPOOL_SUM_180  (MEMPOOL_SUM_96 + (MEMPOOL_NO_96 * MEMPOOL_SIZE_96))
#define MEMPOOL_SUM_300  (MEMPOOL_SUM_180 + (MEMPOOL_NO_180 * MEMPOOL_SIZE_180))
#define MEMPOOL_SUM_420  (MEMPOOL_SUM_300 + (MEMPOOL_NO_300 * MEMPOOL_SIZE_300))
#define MEMPOOL_SUM_600  (MEMPOOL_SUM_420 + (MEMPOOL_NO_420 * MEMPOOL_SIZE_420))
#define MEMPOOL_SUM_800  (MEMPOOL_SUM_600 + (MEMPOOL_NO_600 * MEMPOOL_SIZE_600))
#define MEMPOOL_SUM_1200  (MEMPOOL_SUM_800 + (MEMPOOL_NO_800 * MEMPOOL_SIZE_800))

extern void *pvUSBMemPtr;

typedef struct{
    UINT32 u4Used;
    void* pvMem;
}MEMPOOL_T;

static MEMPOOL_T _arMemPool[MEMPOOLNUM];
static UINT32 _MemPoolInit = FALSE;
/*----------------------------------------------------------------------------
 * Function: x_mem_alloc()
 *
 * Description:
 *      this API allocates memory from default partition. it performs
 *      1. check if default partition exists (created via os_main()).
 *      2. call partition allocate funtiocn pf_alloc().
 *
 * Inputs:
 *      z_size: requested memory size
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL: routine was successful.
 *      NULL: routine failed.
 *---------------------------------------------------------------------------*/
void *
x_mem_alloc(size_t z_size)
{
    UINT32  i;
    UINT32 u4MemAddr;
    
    MUSB_ASSERT(pvUSBMemPtr);

    if (!_MemPoolInit)
    {
        for (i=0; i<MEMPOOLNUM; i++)
        {
            _arMemPool[i].u4Used = FALSE;
            _arMemPool[i].pvMem = NULL;                
        }    

        u4MemAddr = (UINT32)pvUSBMemPtr;
        
        // align 4 bytes memory boundary.
        if (u4MemAddr & 0x3)
        {
            u4MemAddr += 4 - (u4MemAddr & 0x3);
            pvUSBMemPtr = (void *)u4MemAddr;
        }

        _MemPoolInit = TRUE;
    }

    if (z_size < MEMPOOL_SIZE_20)
    {
        for (i=MEMPOOL_IDX_20; i<MEMPOOL_IDX_48; i++)
        {
            if (!_arMemPool[i].u4Used)
            {
                _arMemPool[i].u4Used = TRUE;

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d.\n", z_size, MEMPOOL_SIZE_20, i);                
#endif    
                _arMemPool[i].pvMem =(void *)((UINT32)pvUSBMemPtr + 
                    ((i-MEMPOOL_IDX_20) * MEMPOOL_SIZE_20) + MEMPOOL_SUM_20);
                return _arMemPool[i].pvMem;
            }    
#ifdef MEM_DEBUG
            Printf("Alloc size = %d from pool = %d, index = %d used.\n", z_size, MEMPOOL_SIZE_20, i);                
#endif    
        }        
    }
    
    if (z_size < MEMPOOL_SIZE_48)
    {
        for (i=MEMPOOL_IDX_48; i<MEMPOOL_IDX_96; i++)
        {
            if (!_arMemPool[i].u4Used)
            {
                _arMemPool[i].u4Used = TRUE;

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d.\n", z_size, MEMPOOL_SIZE_48, i);                
#endif    
                _arMemPool[i].pvMem =(void *)((UINT32)pvUSBMemPtr + 
                    ((i-MEMPOOL_IDX_48) * MEMPOOL_SIZE_48) + MEMPOOL_SUM_48);
                return _arMemPool[i].pvMem;
            }    
#ifdef MEM_DEBUG
            Printf("Alloc size = %d from pool = %d, index = %d used.\n", z_size, MEMPOOL_SIZE_48, i);                
#endif    
        }        
    }

    if (z_size < MEMPOOL_SIZE_96)
    {
        for (i=MEMPOOL_IDX_96; i<MEMPOOL_IDX_180; i++)
        {
            if (!_arMemPool[i].u4Used)
            {
                _arMemPool[i].u4Used = TRUE;

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d.\n", z_size, MEMPOOL_SIZE_96, i);                
#endif    
                _arMemPool[i].pvMem = (void *)((UINT32)pvUSBMemPtr + 
                    ((i-MEMPOOL_IDX_96) * MEMPOOL_SIZE_96) + MEMPOOL_SUM_96);
                return _arMemPool[i].pvMem;
            }    
#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d used.\n", z_size, MEMPOOL_SIZE_96, i);                
#endif    
            
        }        
    }

    if (z_size < MEMPOOL_SIZE_180)    
    {
        for (i=MEMPOOL_IDX_180; i<MEMPOOL_IDX_300; i++)
        {
            if (!_arMemPool[i].u4Used)
            {
                _arMemPool[i].u4Used = TRUE;

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d.\n", z_size, MEMPOOL_SIZE_180, i);                
#endif    

                _arMemPool[i].pvMem = (void *)((UINT32)pvUSBMemPtr + 
                    ((i-MEMPOOL_IDX_180) * MEMPOOL_SIZE_180) + MEMPOOL_SUM_180);
                return _arMemPool[i].pvMem;                    
            }    

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d used.\n", z_size, MEMPOOL_SIZE_180, i);                
#endif    
            
        }        
    }    

    if (z_size < MEMPOOL_SIZE_300)    
    {
        for (i=MEMPOOL_IDX_300; i<MEMPOOL_IDX_420; i++)
        {
            if (!_arMemPool[i].u4Used)
            {
                _arMemPool[i].u4Used = TRUE;

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d.\n", z_size, MEMPOOL_SIZE_300, i);                
#endif    
                _arMemPool[i].pvMem = (void *)((UINT32)pvUSBMemPtr + 
                    ((i-MEMPOOL_IDX_300) * MEMPOOL_SIZE_300) + MEMPOOL_SUM_300);
                return _arMemPool[i].pvMem;                    
            }    

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d used.\n", z_size, MEMPOOL_SIZE_300, i);                
#endif    
            
        }        
    }    

    if (z_size < MEMPOOL_SIZE_420)    
    {
        for (i=MEMPOOL_IDX_420; i<MEMPOOL_IDX_600; i++)
        {
            if (!_arMemPool[i].u4Used)
            {
                _arMemPool[i].u4Used = TRUE;

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d.\n", z_size, MEMPOOL_SIZE_420, i);                
#endif    
                _arMemPool[i].pvMem = (void *)((UINT32)pvUSBMemPtr + 
                    ((i-MEMPOOL_IDX_420) * MEMPOOL_SIZE_420) + MEMPOOL_SUM_420);
                return _arMemPool[i].pvMem;                    
            }    

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d used.\n", z_size, MEMPOOL_SIZE_420, i);                
#endif                
        }        
    }    

    if (z_size < MEMPOOL_SIZE_600)    
    {
        for (i=MEMPOOL_IDX_600; i<MEMPOOL_IDX_800; i++)
        {
            if (!_arMemPool[i].u4Used)
            {
                _arMemPool[i].u4Used = TRUE;

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d.\n", z_size, MEMPOOL_SIZE_600, i);                
#endif    
                _arMemPool[i].pvMem = (void *)((UINT32)pvUSBMemPtr + 
                    ((i-MEMPOOL_IDX_600) * MEMPOOL_SIZE_600) + MEMPOOL_SUM_600);
                return _arMemPool[i].pvMem;                    
            }    

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d used.\n", z_size, MEMPOOL_SIZE_600, i);                
#endif                
            
        }           
    }    

    if (z_size < MEMPOOL_SIZE_800)    
    {
        for (i=MEMPOOL_IDX_800; i<MEMPOOL_IDX_1200; i++)
        {
            if (!_arMemPool[i].u4Used)
            {
                _arMemPool[i].u4Used = TRUE;

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d.\n", z_size, MEMPOOL_SIZE_800, i);                
#endif    

                _arMemPool[i].pvMem = (void *)((UINT32)pvUSBMemPtr + 
                    ((i-MEMPOOL_IDX_800) * MEMPOOL_SIZE_800) + MEMPOOL_SUM_800);
                return _arMemPool[i].pvMem;                    
            }    

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d used.\n", z_size, MEMPOOL_SIZE_800, i);                
#endif                
            
        }               
    }    

    if (z_size < MEMPOOL_SIZE_1200)    
    {
        for (i=MEMPOOL_IDX_1200; i<MEMPOOLNUM; i++)
        {
            if (!_arMemPool[i].u4Used)
            {
                _arMemPool[i].u4Used = TRUE;

#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d.\n", z_size, MEMPOOL_SIZE_1200, i);                
#endif    

                _arMemPool[i].pvMem = (void *)((UINT32)pvUSBMemPtr + 
                    ((i-MEMPOOL_IDX_1200) * MEMPOOL_SIZE_1200) + MEMPOOL_SUM_1200);
                return _arMemPool[i].pvMem;                    
            }    
#ifdef MEM_DEBUG
                Printf("Alloc size = %d from pool = %d, index = %d used.\n", z_size, MEMPOOL_SIZE_1200, i);                
#endif                
            
        }                   
    }    

#ifdef MEM_DEBUG
    Printf("Memory size = %d not found.\n", z_size);
#endif

    return NULL;
}

/*----------------------------------------------------------------------------
 * Function: x_mem_free()
 *
 * Description:
 *      This API frees a memory block. it performs
 *      1. check validity of pv_mem.
 *      2. call os_mem_free() to actual free the memory block.
 *
 * Inputs:
 *      pv_mem: pointer to a memory block to be freed.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
void
x_mem_free(void *pv_mem)
{
    UINT32  i;

    for (i=0; i<MEMPOOLNUM; i++)
    {
        if (_arMemPool[i].pvMem == pv_mem)
        {
            MUSB_ASSERT(_arMemPool[i].u4Used);
            _arMemPool[i].u4Used = FALSE;
            return;
        }    
    }

    MUSB_ASSERT(0);
}
