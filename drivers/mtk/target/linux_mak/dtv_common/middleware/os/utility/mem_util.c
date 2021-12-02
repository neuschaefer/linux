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
 * $RCSfile: mem_util.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "os/inc/os.h"
#include "os/memory/mem_lib.h"
#include <string.h>


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

extern MEM_GLOBAL_T *pt_mem_global;


/*----------------------------------------------------------------------------
 * Function: x_memcpy()
 *
 * Description:
 *      this API copies pv_from to pv_to up to z_l bytes. it performs
 *      1. parse argument and return pv_to on error.
 *      2. process to make the most UINT32 copies, then UINT16 copies, then
 *         UINT8 copies. 
 *
 * Inputs:
 *      pv_to : memory block to be copied to.
 *      pv_from : memory block to be copied from.
 *      z_l : bytes to be copied
 *
 * Outputs: -
 *
 * Returns:
 *      pointer to pv_to
 *---------------------------------------------------------------------------*/
VOID *
x_memcpy(VOID *pv_to, const VOID *pv_from, SIZE_T z_l)
{
    if ((pv_to != NULL) || (z_l == 0))
    {
        return memcpy(pv_to, pv_from, z_l);
    }
    else
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_NULL_ADDRESS);
    }
    
    return pv_to;
}


/*----------------------------------------------------------------------------
 * Function: x_memcmp()
 *
 * Description:
 *      this API compares 2 memory blocks pv_mem1 and pv_mem2 up to z_l bytes.
 *      1. if both are NULL, return 0.
 *      2. if pv_mem1 is NULL and pv_mem2 is not NULL, return -1.
 *      3. if pv_mem1 is not NULL and pv_mem2 is NULL, return 1.
 *      4. compute the difference between pv_mem1 and pv_mem2 side by side.
 *      5. if difference is less than zero, return -1; if difference is
 *         positive, return 1; if diffenece is 0, move both pointers, repeat
 *         steps 4, 5.
 *      6. stop when z_l bytes have met.
 *
 * Inputs:
 *      pv_mem1 : null ('\0') terminated character string.
 *      pv_mem2 : null ('\0') terminated character string.
 *      z_l : byte count to compare.
 *
 * Outputs: -
 *
 * Returns:
 *      0 : pv_mem1 is identical to pv_mem2.
 *      1 : pv_mem1 is greater than pv_mem2.
 *      -1 : pv_mem1 is less than pv_mem2.
 *---------------------------------------------------------------------------*/
INT32
x_memcmp(const VOID *pv_mem1, const VOID *pv_mem2, SIZE_T z_l)
{
    UCHAR *puc_s1, *puc_s2;

    if (z_l == 0)
    {
        return(0);
    }

    if (pv_mem1 == NULL)
    {
        if (pv_mem2 != NULL)
        {
            return(-1);
        }
        else
        {
            return(0);
        }
    }
    else if (pv_mem2 == NULL)
    {
        return(1);
    }

    puc_s1 = (UCHAR *) pv_mem1;
    puc_s2 = (UCHAR *) pv_mem2;
    do
    {
        if (*puc_s1 != *puc_s2)
        {
            if (*puc_s1 > *puc_s2)
            {
                return(1);
            }
            else
            {
                return(-1);
            }
        }
        puc_s1 ++; 
        puc_s2 ++;
    } while (--z_l > 0);

    return(0);
}


/*----------------------------------------------------------------------------
 * Function: x_memmove()
 *
 * Description:
 *      this API copies pv_from to pv_to up to z_l bytes. it performs
 *      1. parse argument and return ps_to on error.
 *      2. check overlap between pv_to and pv_from and copy contents backward.
 *      3. process to make the most UINT32 copies, then UINT16 copies, then
 *         UINT8 copies. 
 *
 * Inputs:
 *      pv_to : memory block to be copied to.
 *      pv_from : memory block to copy from.
 *      z_l : bytes to be copied
 *
 * Outputs: -
 *
 * Returns:
 *      pointer to pv_to
 *---------------------------------------------------------------------------*/
VOID *
x_memmove(VOID *pv_to, const VOID *pv_from, SIZE_T z_l)
{
    if ((pv_to != NULL) || (z_l == 0))
    {
        return memmove(pv_to, pv_from, z_l);
    }
    else
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_NULL_ADDRESS);
    }
    
    return pv_to;
}


/*----------------------------------------------------------------------------
 * Function: x_memset()
 *
 * Description:
 *      this API copies ui1_c to pv_to up to z_l bytes. it performs
 *      1. parse argument and return ps_to on error.
 *      2. process to make the most UINT32 copies, then UINT16 copies, then
 *         UINT8 copies. 
 *
 * Inputs:
 *      pv_to : memory block to be copied to.
 *      ui1_c : data to populate to ps_to.
 *      z_l : bytes to be copied
 *
 * Outputs: -
 *
 * Returns:
 *      pointer to pv_to
 *---------------------------------------------------------------------------*/
VOID *
x_memset(VOID *pv_to, UINT8 ui1_c, SIZE_T z_l)
{
    if ((pv_to != NULL) || (z_l == 0))
    {
        return memset(pv_to, ui1_c, z_l);
    }
    else
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_NULL_ADDRESS);
    }
    
    return pv_to;
}

/*----------------------------------------------------------------------------
 * Function: x_memchr
 *
 * Description:
 *      This API scans the first z_n bytes of the memory area pointed to by
 *      pv_mem for the first character ui4_c.
 *
 * Inputs:
 *      pv_mem  References the memory area.
 *      ui4_c   Specifies the character to look for (interpreted and a UCHAR).
 *      z_n     Contains the number of bytes to scan.
 *
 * Outputs: -
 *
 * Returns:
 *      Pointer to the matching byte if any, or NULL.
 *---------------------------------------------------------------------------*/
VOID* x_memchr (const VOID*  pv_mem,
                UINT8        ui1_c,
                SIZE_T       z_len)
{
    const UINT8*  pui1_mem;
    
    pui1_mem = (const UINT8*) pv_mem;
    
    while (z_len--)
    {
        if (*pui1_mem == ui1_c)
        {
            return (VOID*) pui1_mem;
        }
        
        pui1_mem++;
    }
    
    return NULL;
}

/*----------------------------------------------------------------------------
 * Function: x_memrchr
 *
 * Description:
 *      Similar to x_memrchr, but searches backward from the end of the z_n
 *      bytes pointed to by pv_mem instead of forwards from the front.
 *
 * Inputs:
 *      pv_mem  References the memory area.
 *      ui1_c   Specifies the character to look for.
 *      z_n     Contains the number of bytes to scan.
 *
 * Outputs: -
 *
 * Returns:
 *      Pointer to the matching byte if any, or NULL.
 *---------------------------------------------------------------------------*/
VOID* x_memrchr (const VOID*  pv_mem,
                 UINT8        ui1_c,
                 SIZE_T       z_len)
{
    const UINT8*  pui1_mem;
    
    pui1_mem = ((const UINT8*) pv_mem) + z_len - 1;
    
    while (z_len--)
    {
        if (*pui1_mem == ui1_c)
        {
            return (VOID*) pui1_mem;
        }
        
        pui1_mem--;
    }
    
    return NULL;
}

/*----------------------------------------------------------------------------
 * Function: x_os_drv_reg_mem_functions
 *
 * Description:
 *      This API registers driver-specific memory functions. This is useful
 *      when the driver implementation is optimized with DMA or assembly for
 *      example.
 *
 * Inputs:
 *      pt_functions  References a structure containing the function pointers.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK       Functions have been registered successfully.
 *      OSR_INV_ARG  Invalid argument (NULL pointer).
 *      OSR_FAIL     Too late to register the functions because the OS layer is
 *                   already initialized.
 *---------------------------------------------------------------------------*/
INT32 x_os_drv_reg_mem_functions (const DRV_MEM_FUNCTIONS_T*  pt_functions)
{
    INT32  i4_res;
    
    if ((pt_functions != NULL) && (pt_mem_global == NULL))
    {
        i4_res = OSR_OK;
    }
    else if (pt_functions == NULL)
    {
        i4_res = OSR_INV_ARG;
    }
    else
    {
        /* Memory functions can only be registered before OS is initialized */
        
        i4_res = OSR_FAIL;
    }
    
    return i4_res;
}
