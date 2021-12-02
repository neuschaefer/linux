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
 * $RCSfile: mheg5_mem.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all memory related interfaces between the MHEG-5
 *         stack from OceanBlue and the OSAI.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/u_handle.h"
#include "os/inc/x_os.h"
#include "os/inc/u_os.h"
#include "mheg5/u_mheg5.h"

/* OceanBlue header files. */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_memory.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define MEM_PART_NAME  "mheg_5_partion"


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

static HANDLE_T  h_mem_part = NULL_HANDLE;  /* Memory partition handle. */


/*-----------------------------------------------------------------------------
 * Name:  MEMOpen
 *
 * Description: This API opens a memory partition to be used by the MHEG-5
 *              stack.
 *
 * Inputs:  memoryRegionHandle  Ignored.
 *          memoryRegionSize    The memory region size in number of Bytes.
 *
 * Outputs: -
 *
 * Returns: MEM_OK if succesfull else an error code.
 ----------------------------------------------------------------------------*/
MEMErr_t MEMOpen (void*   memoryRegionHandle,
                  U32BIT  memoryRegionSize)
{
    VOID*       pv_mem_part;
    MEMErr_t    e_return;
    HANDLE_T    h_mp;

    e_return = MEM_ERR_SYSTEM_ERROR;

    h_mp = (HANDLE_T) memoryRegionHandle;
    
    if (NULL_HANDLE == h_mp)
    {
        e_return = MEM_ERR_BAD_PARAMETER;

        if (memoryRegionSize > 0)
        {
            e_return = MEM_ERR_OUT_OF_MEMORY;

            /* And now, allocate memory for the MHEG-5 partition. */
            if ((pv_mem_part = x_mem_alloc (((SIZE_T) memoryRegionSize))) != NULL)
            {
                if (x_mem_part_create (&h_mem_part, MEM_PART_NAME, pv_mem_part, ((SIZE_T) memoryRegionSize), 0) == OSR_OK)
                {
                    e_return = MEM_OK;
                }
                else
                {
                    /* Oops, out of memory. Clean-up. */
                    x_mem_free (pv_mem_part);
                }
            }
        }
    }
    else
    {
        if (NULL_HANDLE == h_mem_part)
        {
            h_mem_part = h_mp;
            e_return = MEM_OK;
        }
    }

    return (e_return);
}


/*-----------------------------------------------------------------------------
 * Name:  MEMClose
 *
 * Description: This API closes a memory partition used by the MHEG-5 stack.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: MEM_OK if succesfull else an error code.
 ----------------------------------------------------------------------------*/
MEMErr_t MEMClose (void)
{
    MEMErr_t e_return;

    e_return = MEM_ERR_SYSTEM_ERROR;

    if (h_mem_part != NULL_HANDLE)
    {
        if (x_mem_part_delete (h_mem_part) == OSR_OK)
        {
            e_return = MEM_OK;

            h_mem_part = NULL_HANDLE;
        }
    }

    return (e_return);
}


/*-----------------------------------------------------------------------------
 * Name:  MEMAlloc
 *
 * Description: This API allocates some memory from the MHEG-5 partition.
 *
 * Inputs:  memSize  Indicates the memory size which shall be allocated.
 *
 * Outputs: -
 *
 * Returns: Reference to the allocated chunk of memory or NULL.
 ----------------------------------------------------------------------------*/
void* MEMAlloc (U32BIT memSize)
{
    VOID* pv_mem;

    pv_mem = NULL;

    if (h_mem_part != NULL_HANDLE)
    {
        pv_mem = x_mem_part_alloc (h_mem_part, ((SIZE_T) memSize));
    }
    /*x_dbg_stmt("MEMAlloc(memSize:%lu) -> %p\n", memSize, pv_mem);*/

    return ((void*) pv_mem);
}


/*-----------------------------------------------------------------------------
 * Name:  MEMCalloc
 *
 * Description: This API allocates some memory from the MHEG-5 partition.
 *
 * Inputs:  memSize  Indicates the memory size which shall be allocated.
 *
 * Outputs: -
 *
 * Returns: Reference to the allocated chunk of memory or NULL.
 ----------------------------------------------------------------------------*/
void* MEMCalloc (U32BIT  memSize,
                 S32BIT  numElems)
{
    VOID* pv_mem;

    pv_mem = NULL;

    if (h_mem_part != NULL_HANDLE)
    {
        pv_mem = x_mem_part_calloc (h_mem_part, ((UINT32) numElems), ((SIZE_T) memSize));
    }
    /*x_dbg_stmt("MEMCalloc(memSize:%lu, numElems:%ld) -> %p\n", memSize, numElems, pv_mem);*/

    return ((void*) pv_mem);
}


/*-----------------------------------------------------------------------------
 * Name:  MEMFree
 *
 * Description: This API frees a previously allocated chunk of memory.
 *
 * Inputs:  Reference to the previously allocated memory chunk.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
void MEMFree (void* pData)
{  
    /*x_dbg_stmt("MEMFree(pData:%p)\n", pData);*/
    x_mem_free ((VOID*) pData);    
}

