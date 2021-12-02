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
 * $RCSfile: end_cnvt.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains the endian conversion implementation.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "util/x_end_cnvt.h"


/*-----------------------------------------------------------------------------
 * Name: x_end_cnvt_big_16
 *
 * Description: This API converts 16 bit values to / from big endian. If
 *              argument "pv_dest" contains a NULL pointer then this API
 *              will perform the endian conversion in the source array
 *              referenced by agument "pv_src".
 *
 * Inputs:  pv_src         References an array of 16 bit values which shall be
 *                         converted. This argument may not be set to NULL.
 *          z_num_entries  Contains the number of 16 bit values in the array
 *                         referenced by argument "pv_src".
 *
 * Outputs: pv_dest  References the array which shall contain the converted
 *                   16 bit values.
 *
 * Returns: ENDCR_OK       Routine successful.
 *          ENDCR_INV_ARG  Invalid argument.
 ----------------------------------------------------------------------------*/
INT32 x_end_cnvt_big_16 (VOID*   pv_src,
                         SIZE_T  z_num_entries,
                         VOID*   pv_dest)
{
    INT32  i4_return;

#ifdef _CPU_LITTLE_ENDIAN_
    UINT16 ui2_data;
#endif
    

    i4_return = ENDCR_INV_ARG;

    if ((pv_src        != NULL) &&
        (z_num_entries != 0))
    {
        i4_return = ENDCR_OK;
        
#ifdef _CPU_LITTLE_ENDIAN_
        if (pv_dest == NULL)
        {
            pv_dest = pv_src;
        }

        while (z_num_entries > 0)
        {
            ui2_data = GET_UINT16_FROM_PTR_BIG_END (pv_src);

            PUT_UINT16_TO_PTR_LITTLE_END (ui2_data, pv_dest);

            pv_src  = (VOID*) (((UINT32) pv_src)  + sizeof (UINT16));
            pv_dest = (VOID*) (((UINT32) pv_dest) + sizeof (UINT16));
            
            z_num_entries--;
        }
#else        
        if (pv_dest != NULL)
        {
            /* Simply perform a memory copy. No conversion required. */
            x_memcpy (pv_dest, pv_src, (z_num_entries * sizeof (UINT16)));
        }
#endif
    }
    
    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_end_cnvt_big_32
 *
 * Description: This API converts 32 bit values to / from big endian. If
 *              argument "pv_dest" contains a NULL pointer then this API
 *              will perform the endian conversion in the source array
 *              referenced by agument "pv_src".
 *
 * Inputs:  pv_src         References an array of 32 bit values which shall be
 *                         converted. This argument may not be set to NULL.
 *          z_num_entries  Contains the number of 32 bit values in the array
 *                         referenced by argument "pv_src".
 *
 * Outputs: pv_dest  References the array which shall contain the converted
 *                   32 bit values.
 *
 * Returns: ENDCR_OK       Routine successful.
 *          ENDCR_INV_ARG  Invalid argument.
 ----------------------------------------------------------------------------*/
INT32 x_end_cnvt_big_32 (VOID*   pv_src,
                         SIZE_T  z_num_entries,
                         VOID*   pv_dest)
{
    INT32  i4_return;

#ifdef _CPU_LITTLE_ENDIAN_
    UINT32 ui4_data;
#endif
    

    i4_return = ENDCR_INV_ARG;

    if ((pv_src        != NULL) &&
        (z_num_entries != 0))
    {
        i4_return = ENDCR_OK;
        
#ifdef _CPU_LITTLE_ENDIAN_
        if (pv_dest == NULL)
        {
            pv_dest = pv_src;
        }

        while (z_num_entries > 0)
        {
            ui4_data = GET_UINT32_FROM_PTR_BIG_END (pv_src);

            PUT_UINT32_TO_PTR_LITTLE_END (ui4_data, pv_dest);

            pv_src  = (VOID*) (((UINT32) pv_src)  + sizeof (UINT32));
            pv_dest = (VOID*) (((UINT32) pv_dest) + sizeof (UINT32));
            
            z_num_entries--;
        }
#else        
        if (pv_dest != NULL)
        {
            /* Simply perform a memory copy. No conversion required. */
            x_memcpy (pv_dest, pv_src, (z_num_entries * sizeof (UINT32)));
        }
#endif
    }
    
    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_end_cnvt_big_64
 *
 * Description: This API converts 64 bit values to / from big endian. If
 *              argument "pv_dest" contains a NULL pointer then this API
 *              will perform the endian conversion in the source array
 *              referenced by agument "pv_src".
 *
 * Inputs:  pv_src         References an array of 64 bit values which shall be
 *                         converted. This argument may not be set to NULL.
 *          z_num_entries  Contains the number of 64 bit values in the array
 *                         referenced by argument "pv_src".
 *
 * Outputs: pv_dest  References the array which shall contain the converted
 *                   64 bit values.
 *
 * Returns: ENDCR_OK       Routine successful.
 *          ENDCR_INV_ARG  Invalid argument.
 ----------------------------------------------------------------------------*/
INT32 x_end_cnvt_big_64 (VOID*   pv_src,
                         SIZE_T  z_num_entries,
                         VOID*   pv_dest)
{
    INT32  i4_return;

#ifdef _CPU_LITTLE_ENDIAN_
    UINT64 ui8_data;
#endif
    

    i4_return = ENDCR_INV_ARG;

    if ((pv_src        != NULL) &&
        (z_num_entries != 0))
    {
        i4_return = ENDCR_OK;
        
#ifdef _CPU_LITTLE_ENDIAN_
        if (pv_dest == NULL)
        {
            pv_dest = pv_src;
        }

        while (z_num_entries > 0)
        {
            ui8_data = GET_UINT64_FROM_PTR_BIG_END (pv_src);

            PUT_UINT64_TO_PTR_LITTLE_END (ui8_data, pv_dest);

            pv_src  = (VOID*) (((UINT32) pv_src)  + sizeof (UINT64));
            pv_dest = (VOID*) (((UINT32) pv_dest) + sizeof (UINT64));
            
            z_num_entries--;
        }
#else        
        if (pv_dest != NULL)
        {
            /* Simply perform a memory copy. No conversion required. */
            x_memcpy (pv_dest, pv_src, (z_num_entries * sizeof (UINT64)));
        }
#endif
    }
    
    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_end_cnvt_little_16
 *
 * Description: This API converts 16 bit values to / from little endian. If
 *              argument "pv_dest" contains a NULL pointer then this API
 *              will perform the endian conversion in the source array
 *              referenced by agument "pv_src".
 *
 * Inputs:  pv_src         References an array of 16 bit values which shall be
 *                         converted. This argument may not be set to NULL.
 *          z_num_entries  Contains the number of 16 bit values in the array
 *                         referenced by argument "pv_src".
 *
 * Outputs: pv_dest  References the array which shall contain the converted
 *                   16 bit values.
 *
 * Returns: ENDCR_OK       Routine successful.
 *          ENDCR_INV_ARG  Invalid argument.
 ----------------------------------------------------------------------------*/
INT32 x_end_cnvt_little_16 (VOID*   pv_src,
                            SIZE_T  z_num_entries,
                            VOID*   pv_dest)
{
    INT32  i4_return;

#ifdef _CPU_BIG_ENDIAN_
    UINT16 ui2_data;
#endif

    
    i4_return = ENDCR_INV_ARG;

    if ((pv_src        != NULL) &&
        (z_num_entries != 0))
    {
        i4_return = ENDCR_OK;
        
#ifdef _CPU_BIG_ENDIAN_
        if (pv_dest == NULL)
        {
            pv_dest = pv_src;
        }

        while (z_num_entries > 0)
        {
            ui2_data = GET_UINT16_FROM_PTR_LITTLE_END (pv_src);

            PUT_UINT16_TO_PTR_BIG_END (ui2_data, pv_dest);

            pv_src  = (VOID*) (((UINT32) pv_src)  + sizeof (UINT16));
            pv_dest = (VOID*) (((UINT32) pv_dest) + sizeof (UINT16));
            
            z_num_entries--;
        }
#else        
        if (pv_dest != NULL)
        {
            /* Simply perform a memory copy. No conversion required. */
            x_memcpy (pv_dest, pv_src, (z_num_entries * sizeof (UINT16)));
        }
#endif
    }
    
    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_end_cnvt_little_32
 *
 * Description: This API converts 32 bit values to / from little endian. If
 *              argument "pv_dest" contains a NULL pointer then this API
 *              will perform the endian conversion in the source array
 *              referenced by agument "pv_src".
 *
 * Inputs:  pv_src         References an array of 32 bit values which shall be
 *                         converted. This argument may not be set to NULL.
 *          z_num_entries  Contains the number of 32 bit values in the array
 *                         referenced by argument "pv_src".
 *
 * Outputs: pv_dest  References the array which shall contain the converted
 *                   32 bit values.
 *
 * Returns: ENDCR_OK       Routine successful.
 *          ENDCR_INV_ARG  Invalid argument.
 ----------------------------------------------------------------------------*/
INT32 x_end_cnvt_little_32 (VOID*   pv_src,
                            SIZE_T  z_num_entries,
                            VOID*   pv_dest)
{
    INT32  i4_return;

#ifdef _CPU_BIG_ENDIAN_
    UINT32 ui4_data;
#endif

    
    i4_return = ENDCR_INV_ARG;

    if ((pv_src        != NULL) &&
        (z_num_entries != 0))
    {
        i4_return = ENDCR_OK;
        
#ifdef _CPU_BIG_ENDIAN_
        if (pv_dest == NULL)
        {
            pv_dest = pv_src;
        }

        while (z_num_entries > 0)
        {
            ui4_data = GET_UINT32_FROM_PTR_LITTLE_END (pv_src);

            PUT_UINT32_TO_PTR_BIG_END (ui4_data, pv_dest);

            pv_src  = (VOID*) (((UINT32) pv_src)  + sizeof (UINT32));
            pv_dest = (VOID*) (((UINT32) pv_dest) + sizeof (UINT32));
            
            z_num_entries--;
        }
#else
        if (pv_dest != NULL)
        {
            /* Simply perform a memory copy. No conversion required. */
            x_memcpy (pv_dest, pv_src, (z_num_entries * sizeof (UINT32)));
        }
#endif
    }
    
    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_end_cnvt_little_64
 *
 * Description: This API converts 64 bit values to / from little endian. If
 *              argument "pv_dest" contains a NULL pointer then this API
 *              will perform the endian conversion in the source array
 *              referenced by agument "pv_src".
 *
 * Inputs:  pv_src         References an array of 64 bit values which shall be
 *                         converted. This argument may not be set to NULL.
 *          z_num_entries  Contains the number of 64 bit values in the array
 *                         referenced by argument "pv_src".
 *
 * Outputs: pv_dest  References the array which shall contain the converted
 *                   64 bit values.
 *
 * Returns: ENDCR_OK       Routine successful.
 *          ENDCR_INV_ARG  Invalid argument.
 ----------------------------------------------------------------------------*/
INT32 x_end_cnvt_little_64 (VOID*   pv_src,
                            SIZE_T  z_num_entries,
                            VOID*   pv_dest)
{
    INT32  i4_return;

#ifdef _CPU_BIG_ENDIAN_
    UINT64 ui8_data;
#endif
    

    i4_return = ENDCR_INV_ARG;

    if ((pv_src        != NULL) &&
        (z_num_entries != 0))
    {
        i4_return = ENDCR_OK;
        
#ifdef _CPU_BIG_ENDIAN_
        if (pv_dest == NULL)
        {
            pv_dest = pv_src;
        }

        while (z_num_entries > 0)
        {
            ui8_data = GET_UINT64_FROM_PTR_LITTLE_END (pv_src);

            PUT_UINT64_TO_PTR_BIG_END (ui8_data, pv_dest);

            pv_src  = (VOID*) (((UINT32) pv_src)  + sizeof (UINT64));
            pv_dest = (VOID*) (((UINT32) pv_dest) + sizeof (UINT64));
            
            z_num_entries--;
        }
#else
        if (pv_dest != NULL)
        {
            /* Simply perform a memory copy. No conversion required. */
            x_memcpy (pv_dest, pv_src, (z_num_entries * sizeof (UINT64)));
        }
#endif
    }
    
    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_sys_end_is_big
 *
 * Description: This API returns the systems or CPU endianess.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if the system or CPU is big endian else FALSE.
 ----------------------------------------------------------------------------*/
BOOL x_sys_end_is_big (VOID)
{
#ifdef _CPU_BIG_ENDIAN_
    return (TRUE);
#else
    return (FALSE);
#endif
}
