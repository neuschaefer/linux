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
                    include files
 ----------------------------------------------------------------------------*/

#include "x_typedef.h"

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
void *
x_memcpy(void *pv_to, const void *pv_from, UINT32 z_l)
{
    UINT32 *pui4_t, *pui4_f;
    INT32 i4_i, i4_j, i4_k;
    CHAR *ps_c, *ps_f;

    if ((pv_to == (CHAR *) NULL) || (pv_from == (CHAR *) NULL) || (z_l == 0))
    {
        return(pv_to);
    }

    i4_i = ((UINT32) pv_to) & 0x3;      /* off from INT32 boundary */
    i4_j = ((UINT32) pv_from) & 0x3;
    if (i4_i == i4_j)           /* check for INT32 assignment */
    {
        if (i4_i & 0x1)         /* off 1 or 3 bytes mean 3 or 1 leading bytes */
        {
            i4_i = 4 - i4_i;    /* leading bytes to next INT32 aligned */
        }
        i4_j = z_l - i4_i;      /* remaining bytes excl. leading bytes */
        i4_k = i4_j >> 2;       /* count of INT32 */
    }
    else
    {
        i4_k = 0;
    }

    if (i4_k > 0)               /* do INT32 assignment */
    {
        pui4_t = (UINT32 *) pv_to;
        pui4_f = (UINT32 *) pv_from;

        /* leading bytes */
        if (i4_i & 0x1)    /* 1 or 3 leading bytes  */
        {
            *((CHAR *) pui4_t) = *((CHAR *) pui4_f);
            pui4_t = (UINT32 *) (((UINT32) pui4_t) + (UINT32) 1);
            pui4_f = (UINT32 *) (((UINT32) pui4_f) + (UINT32) 1);
        }
        if (i4_i & 0x2)    /* 2 or 3 leading bytes */
        {
            *(UINT16 *) pui4_t = *(UINT16 *) pui4_f;
            pui4_t = (UINT32 *) (((UINT32) pui4_t) + (UINT32) 2);
            pui4_f = (UINT32 *) (((UINT32) pui4_f) + (UINT32) 2);
        }

        do      /* do INT32 assignment */
        {
            *pui4_t++ = *pui4_f++;
        } while (--i4_k > 0);

        /* finish the remaining bytes */
        if (i4_j & 0x2)      /* do an INT16 assignment if possible */
        {
            *(UINT16 *) pui4_t = *(UINT16 *) pui4_f;
            pui4_t = (UINT32 *) (((UINT32) pui4_t) + (UINT32) 2);
            pui4_f = (UINT32 *) (((UINT32) pui4_f) + (UINT32) 2);
        }
        if (i4_j & 0x1)      /* do byte assignment */
        {
            *(CHAR *) pui4_t = *(CHAR *) pui4_f;
        }
        return(pv_to);
    }
    else
    {
        ps_c = (CHAR *) pv_to;
        ps_f = (CHAR *) pv_from;
        do
        {
            *ps_c++ = *ps_f++;
        } while (--z_l > 0);

        return(pv_to);
    }
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
void *
x_memset(void *pv_to, UINT8 ui1_c, UINT32 z_l)
{
    UINT32 *pui4_t, *pui4_s;
    INT32 i4_i, i4_j, i4_k;
    CHAR *ps_u, *ps_to;

    if ((pv_to == NULL) || (z_l == 0))
    {
        return(pv_to);
    }

    ps_u = ps_to = (CHAR *) pv_to;

    i4_i =  ((UINT32) ps_to) & 0x3;   /* off from current INT32 aligned */
    if (i4_i & 0x1)
    {
        i4_i = 4 - i4_i;            /* leading bytes to next INT32 */
    }
    i4_j = z_l - i4_i;              /* remaining bytes excl. leading bytes */
    i4_k = i4_j >> 2;               /* count of INT32 */

    if (i4_k > 0)
    {
        pui4_s = (UINT32 *) (ps_to + i4_i);
        if (i4_i & 0x3)             /* leading bytes */
        {
            if (i4_i & 0x1)         /* 1 or 3 leading bytes  */
            {
                *(CHAR *) ps_to++ = ui1_c;
            }
            if (i4_i & 0x2)         /* 2 or 3 leading bytes */
            {
                *(UINT16 *) ps_to = (UINT16) ((ui1_c << 8) | ui1_c);
            }
        }

        *pui4_s = (ui1_c << 24) | (ui1_c << 16) | (ui1_c << 8) | ui1_c;
        pui4_t = pui4_s + 1;

        while (--i4_k > 0)         /* do INT32 assignment */
        {
            *pui4_t++ = *pui4_s;
        }

        if (i4_j & 0x3)            /* finish the remaining bytes */
        {
            if (i4_j & 0x2)        /* do an INT16 assignment if possible */
            {
                *((UINT16 *) pui4_t) = *(UINT16 *) pui4_s;
                pui4_t = (UINT32 *) ((UINT32) pui4_t + (UINT32) 2);
            }
            if (i4_j & 0x1)        /* do byte assignment */
            {
                *((CHAR *) pui4_t) = ui1_c;
            }
        }

        return(ps_u);
    }
    else
    {
        do
        {
            *(CHAR *) ps_to++ = ui1_c;
        } while (--z_l > 0);

        return(ps_u);
    }
}
