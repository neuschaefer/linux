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
 * Description:
 *
 *---------------------------------------------------------------------------*/

#ifndef CC_MTK_LOADER

#include "x_typedef.h"
#include "x_assert.h"

#ifdef __KERNEL__


#include <linux/bitops.h>
extern UINT64 u8Div6432(UINT64 u8Dividend, UINT64 u8Divider, UINT64 *pu8Remainder);

static BOOL fgCheckOverFlow(UINT64 u8A, UINT64 u8B)
{
    UINT32 u4Ah, u4Al, u4Bh, u4Bl;
    UINT64 u8Tmp;

    u4Ah = u8A >> 32;
    u4Bh = u8B >> 32;

    if ((u4Ah > 0) && (u4Bh > 0))
    {
        return TRUE;
    }

    u4Al = u8A & (0xFFFFFFFF);
    u4Bl = u8B & (0xFFFFFFFF);

    u8Tmp = ((UINT64) u4Ah * u4Bl) + ((UINT64) u4Al * u4Bh);
    if (u8Tmp & 0xFFFFFFFF00000000LL)
    {
        return TRUE;
    }

    return FALSE;
}

/*
 * Do u8Dividend / u8Divider.
 * If pu8Remainder is not NULL, remainder will be returned.
 * The function returns quotient.
 */
UINT64 u8Div6464(UINT64 u8Dividend, UINT64 u8Divider, UINT64 *pu8Remainder)
{
    UINT32 u4High;
    UINT64 u8Ret;
    INT64 i8Remainder, i;

    ASSERT((INT64) u8Dividend >= 0);
    ASSERT((INT64) u8Divider > 0);

    if (u8Divider > u8Dividend)
    {
        if (pu8Remainder)
        {
            *pu8Remainder = (UINT64) u8Dividend;
        }
        return 0;
    }

    u4High = u8Divider >> 32;
    if (u4High) {
        UINT32 u4Shift = fls(u4High);
        u8Ret = u8Div6432(u8Dividend >> u4Shift, u8Divider >> u4Shift, NULL);
    } else
    {
        u8Ret = u8Div6432(u8Dividend, u8Divider, NULL);
    }

    while (fgCheckOverFlow(u8Divider, u8Ret))
    {
        u8Ret--;
    }

    i8Remainder = u8Dividend - (u8Divider * u8Ret);

    i = 0;
    while (i8Remainder < 0)
    {
        i8Remainder += u8Divider;
        i++;
    }
    u8Ret -= i;

    i = 0;
    while (i8Remainder > u8Divider)
    {
        i8Remainder -= u8Divider;
        i++;
    }
    u8Ret += i;

    if (pu8Remainder)
    {
        *pu8Remainder = (UINT64) i8Remainder;
    }

    return u8Ret;
}


#else /* __KERNEL__ not defined */


/*
 * Do u8Dividend / u8Divider.
 * If pu8Remainder is not NULL, remainder will be returned.
 * The function returns quotient.
 */
UINT64 u8Div6464(UINT64 u8Dividend, UINT64 u8Divider, UINT64 *pu8Remainder)
{
    UINT64 u8Quotient;

    u8Quotient = u8Dividend / u8Divider;
    if (pu8Remainder)
    {
        *pu8Remainder = u8Dividend % u8Divider;
    }
    return u8Quotient;
}


#endif /* __KERNEL__ */
#endif // CC_MTK_LOADER

