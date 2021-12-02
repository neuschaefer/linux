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
 * $RCSfile: c_mem_util.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains application level OS functions
 *         (memeory, timer, string operations) prototypes.
 *      
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "os/inc/x_os.h"

/*-------------------------------------------------------------------------
               functions implementations
 -------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Function: c_memcpy()
 *
 * Description:
 *      this API copies pv_from to pv_to up to z_l bytes. 
 *
 * Inputs:
 *      pv_to : memory block to be copied to.
 *      pv_from : memory block to be copied from 
 *      z_len : bytes to be copied
 *
 * Outputs: -
 *
 * Returns:
 *      pointer to pv_to
 *---------------------------------------------------------------------------*/
VOID* c_memcpy
(
    VOID*        pv_to,
    const VOID*  pv_from,
    SIZE_T       z_len
)
{
    return x_memcpy(pv_to, pv_from, z_len);
}

/*----------------------------------------------------------------------------
 * Function: c_memcmp()
 *
 * Description:
 *      this API compares 2 memory blocks pv_mem1 and pv_mem2 up to z_len
 *      bytes.
 *      1. if both are NULL, return 0.
 *      2. if ps_mem1 is NULL and pv_s2 is not NULL, return -1.
 *      3. if ps_mem1 is not NULL and pv_s2 is NULL, return 1.
 *      4. compute the difference between pv_s1 and pv_s2 side by side.
 *      5. if difference is less than zero, return -1; if difference is
 *         positive, return 1; if diffenece is 0, move both pointers, repeat
 *         steps 4, 5.
 *      6. stop when z_len bytes have met.
 *
 * Inputs:
 *      pv_mem1 : pointer to the first memory block.
 *      pv_mem2 : pointer to the second memory block.
 *      z_len : byte count to compare.
 *
 * Outputs: -
 *
 * Returns:
 *      0 : pv_mem1 is identical to pv_mem2.
 *      1 : pv_mem1 is greater than pv_mem2.
 *      -1 : pv_mem1 is less than pv_mem2.
 *---------------------------------------------------------------------------*/
INT32 c_memcmp
(
    const VOID*  pv_mem1,
    const VOID*  pv_mem2,
    SIZE_T       z_len
)
{
    return x_memcmp(pv_mem1, pv_mem2, z_len);
}

/*----------------------------------------------------------------------------
 * Function: c_memmove()
 *
 * Description:
 *      this API copies pv_from to pv_to up to z_len bytes. 
 *
 * Inputs:
 *      pv_to : memory block to write to.
 *      pv_from : memory block to copy from.
 *      z_len : number of bytes to be copied
 *
 * Outputs: -
 *
 * Returns:
 *      pointer to pv_to
 *---------------------------------------------------------------------------*/
VOID* c_memmove
(
    VOID*        pv_to,
    const VOID*  pv_from,
    SIZE_T       z_len
)
{
    return x_memmove(pv_to, pv_from, z_len);
}

/*--------------------------------------------------------------------------
 * Function: c_memset()
 *
 * Description:
 *      this API copies ui1_c to pv_to up to z_len bytes. 
 *
 * Inputs:
 *      pui1_mem : memory block to write to.
 *      ui1_c : data to populate to pui1_mem.
 *      z_len : number of bytes in 'pui1_mem' to write to.
 *
 * Outputs: -
 *
 * Returns:
 *      pointer to pv_to
 *-------------------------------------------------------------------------*/
VOID* c_memset
(
    VOID*   pui1_mem,
    UINT8   ui1_c,
    SIZE_T  z_len
)
{
    return x_memset(pui1_mem, ui1_c, z_len);
}

