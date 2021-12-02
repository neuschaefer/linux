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
#include "x_hal_5381.h"
#include "x_ldr_env.h"

//---------------------------------------------------------------------
// External declarations
//---------------------------------------------------------------------
#ifndef CC_LOADER_MEM_OFFSET
#define CC_LOADER_MEM_OFFSET            (0xd00000)
#endif /* CC_LOADER_MEM_OFFSET */

#if defined(CC_MTK_PRELOADER) && defined(NDEBUG)
EXTERN void CHIP_DisplayString(const UINT8 *szString);
EXTERN void CHIP_DisplayHex(UINT32);
EXTERN void CHIP_DisplayChar(UINT32);
#else /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */
#define CHIP_DisplayString(x)       Printf("%s", x);
#define CHIP_DisplayHex(x)          Printf("%08x", x);
#define CHIP_DisplayChar(x)         Printf("%c", x);
EXTERN INT32 Printf(const CHAR *format, ...);
#endif /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */
EXTERN UINT32 INT_Reset;
EXTERN UINT32 INT_Vectors;

#ifndef __KERNEL__
EXTERN UINT32 Image$$BSS$$Limit;
#else /*__KERNEL__*/
EXTERN UINT32 __bss_start;
#endif /*__KERNEL__*/
//---------------------------------------------------------------------
// Internal declarations
//---------------------------------------------------------------------
#ifndef CC_NAND_BOOT
#ifndef __KERNEL__
#else /*__KERNEL__*/
    #define ZBOOT_IMG_SIZE  240 //zboot.img size is 240 bytes   
#endif /*__KERNEL__*/
#else  /*CC_NAND_BOOT*/
void CHIP_NANDCopyToDRAMLZHS(UINT32 *pu4DstBuf, UINT32 u4ByteOffset , UINT32 u4Len);
#ifndef __KERNEL__
#else /*__KERNEL__*/
#endif /*__KERNEL__*/
#endif /*CC_NAND_BOOT*/
//---------------------------------------------------------------------
// Private functions
//---------------------------------------------------------------------

VOID LinkSymbol(UINT32 fgDisplay)
{
    if (fgDisplay)
    {
        CHIP_DisplayHex(INT_Reset);
        CHIP_DisplayHex(INT_Vectors);
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
#ifndef __KERNEL__
VOID CMain()
{
#ifdef CC_SECURE_BOOT
    UINT32 u4BSSLimit = 0x4000; //16k
#else
    #ifdef CC_ROM_BOOT
        UINT32 u4BSSLimit = (UINT32)&Image$$BSS$$Limit - SRAM_ITCM_BASE;
    #else /* CC_ROM_BOOT */
        UINT32 u4BSSLimit = (UINT32)&Image$$BSS$$Limit;
    #endif /* CC_ROM_BOOT */
#endif
    UINT32 u4ImageAddr, u4Size;
    UINT8 u1Checksum;

#ifndef __KERNEL__
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
#endif

    LinkSymbol(0);
    CHIP_DisplayHex(u4BSSLimit);
    CHIP_DisplayChar('\n');

#ifndef CC_NAND_BOOT
    u4ImageAddr = (((u4BSSLimit + 0x3f) & 0xffffffc0) + 0x40);
    ShowHex("LZHS addr:", u4ImageAddr);
    u4ImageAddr += 0x28000000;
#else
    #ifndef CC_MTK_PRELOADER
        // copy loader.bin.lzhs to dram.
        CHIP_NANDCopyToDRAMLZHS((UINT32 *)(void *)0x100000, (16*1024), (240*1024));
    #else
        u4BSSLimit = (((u4BSSLimit + 0x3f) & 0xffffffc0) /*+ 0x40*/);
        CHIP_NANDCopyToDRAMLZHS((UINT32 *)(void *)0x100000, u4BSSLimit, (240*1024));
    #endif /* CC_MTK_PRELOADER */
    u4ImageAddr = (((0x100000 + 0x3f) & 0xffffffc0) + 0x40);        //64 bytes image Header
    ShowHex("LZHS addr:", u4ImageAddr);
#endif /*CC_NAND_BOOT*/

    u4Size = (*((UINT32 *)u4ImageAddr));
    ShowHex("LZHS size:", u4Size);

// store loader size,address for loader usage
#ifndef __KERNEL__
    prLdrEnv->u4LoaderPlainSize = u4Size;
    prLdrEnv->u4LoaderLzhsSize = *((UINT32 *)(u4ImageAddr + 4));
    prLdrEnv->u4LoaderLzhsAddress = ((u4BSSLimit + 0x3f) & ~0x3f) + 0x40 + 0x10;
#endif

    u1Checksum = (*((UINT8 *)(u4ImageAddr + 8)));
    ShowHex("LZHS checksum:", (UINT32)u1Checksum);

    if (LZHS_Dec(u4ImageAddr+0x10, CC_LOADER_MEM_OFFSET, u4Size, &u1Checksum) != 0)
    {
        CHIP_DisplayString("LZHS failed\n");
    }
    else
    {
        CHIP_DisplayString("LZHS ok\n");
    }
    while (1) { }
}
#else /*__KERNEL__*/

typedef void (*PFN_IMAGE)(UINT32, UINT32);

VOID CMain()
{
    PFN_IMAGE pfnImage;
    UINT32 u4ImageAddr;
    ShowHex("Preloader end addr:", (&__bss_start));
    // Copy Zboot.img to 0xc00000
#ifdef CC_NAND_BOOT
    x_memcpy(0xc00000, (&__bss_start), 0x400);
#else /*CC_NAND_BOOT*/
    x_memcpy(0xc00000, ((UINT32)(&__bss_start))+0x28000000, 0x400);
#endif /*CC_NAND_BOOT*/
    
    pfnImage = (PFN_IMAGE)(0xc00000);
    
#ifdef CC_NAND_BOOT
    // copy loader.bin.lzhs to dram.
    CHIP_NANDCopyToDRAMLZHS((UINT32 *)(void *)0x100000, (16*1024), (240*1024));
    u4ImageAddr = 0x100000;
#else /*CC_NAND_BOOT*/
    #ifdef CC_ROM_BOOT
        u4ImageAddr = ((UINT32)(&__bss_start)) -SRAM_ITCM_BASE + ZBOOT_IMG_SIZE + 0x28000000;
    #else /*CC_ROM_BOOT*/
        u4ImageAddr = ((UINT32)(&__bss_start)) + ZBOOT_IMG_SIZE + 0x28000000;
    #endif /*CC_ROM_BOOT*/
#endif /*CC_NAND_BOOT*/
	ShowHex("LZHS addr:", u4ImageAddr);
    pfnImage( u4ImageAddr, 0xd00000 );
}

#endif /*__KERNEL__*/

