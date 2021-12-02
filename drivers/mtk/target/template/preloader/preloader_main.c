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
 *         The main routine of boot preloader
 *
 *---------------------------------------------------------------------------*/

#include "x_typedef.h"
#include "x_bim.h"

//---------------------------------------------------------------------
// External declarations
//---------------------------------------------------------------------

#if defined(CC_5391_PRELOADER) && defined(NDEBUG)
EXTERN void CHIP_DisplayString(const UINT8 *szString);
EXTERN void CHIP_DisplayHex(UINT32);
EXTERN void CHIP_DisplayChar(UINT32);
#else /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */
#define CHIP_DisplayString(x)       Printf("%s", x);
#define CHIP_DisplayHex(x)          Printf("%08x", x);
#define CHIP_DisplayChar(x)         Printf("%c", x);
#endif /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */
EXTERN UINT32 INT_Reset;
EXTERN UINT32 INT_Vectors;
EXTERN UINT32 Image$$BSS$$Limit;

//---------------------------------------------------------------------
// Internal declarations
//---------------------------------------------------------------------
INT32 au4BSS[4];

//---------------------------------------------------------------------
// Private functions
//---------------------------------------------------------------------

VOID LinkSymbol(UINT32 fgDisplay)
{
    if (fgDisplay)
    {
        // au4BSS[0]++;
        CHIP_DisplayHex(au4BSS[0]);
        CHIP_DisplayChar('\n');
        CHIP_DisplayHex(INT_Reset);
        CHIP_DisplayChar('\n');
        CHIP_DisplayHex(INT_Vectors);
        CHIP_DisplayChar('\n');
    }
}

VOID ShowHex(const UINT8 *pu1Str, UINT32 u4Hex)
{
    CHIP_DisplayString(pu1Str);
    CHIP_DisplayHex(u4Hex);
    CHIP_DisplayChar('\n');
}

//---------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------

/*----------------------------------------------------------------------------
 *---------------------------------------------------------------------------*/
VOID CMain()
{
    UINT32 u4BSSLimit = (UINT32)&Image$$BSS$$Limit;
    UINT32 u4ImageAddr, u4Size;
    UINT8 u1Checksum;

    LinkSymbol(0);
    CHIP_DisplayHex(u4BSSLimit);
    CHIP_DisplayChar('\n');

    u4ImageAddr = (((u4BSSLimit + 0x3f) & 0xffffffc0) + 0x40);
    ShowHex("LZHS addr:", u4ImageAddr);
    u4ImageAddr += 0x28000000;

    u4Size = (*((UINT32 *)u4ImageAddr));
    ShowHex("LZHS size:", u4Size);

    u1Checksum = (*((UINT8 *)(u4ImageAddr + 8)));
    ShowHex("LZHS checksum:", (UINT32)u1Checksum);

    if (LZHS_Dec(u4ImageAddr+0x10, 0xd00000, u4Size, &u1Checksum) != 0)
    {
        CHIP_DisplayString("LZHS failed\n");
    }
    else
    {
        CHIP_DisplayString("LZHS ok\n");
    }
    while (1) { }
}

