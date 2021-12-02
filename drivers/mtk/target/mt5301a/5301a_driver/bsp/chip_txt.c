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
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_bim.h"
#include "x_ckgen.h"
#include "x_pdwnc.h"
#include "x_hal_5381.h"
#include "rs232.h"

#ifndef CC_LOADER_MEM_OFFSET
#define CC_LOADER_MEM_OFFSET            (0xd00000)
#endif /* CC_LOADER_MEM_OFFSET */

UINT32 CHIP_RS232Init(void);
void CHIP_DisplayChar(UINT32 u4Char);
void CHIP_DisplayInteger(INT32 i);
void put_num(const CHAR c);
UINT32 CHIP_IsRemap(void);

UINT32 CHIP_RS232Init()
{
    #if defined(CC_MT8223)
    BIM_WRITE32(0xe04, 0xe2);
    #endif
    // Setup CPU to UART0 output parameter.
//    RS232_WRITE32(REG_CMD_STATUS, CMD_MODE);
    RS232_WRITE32(REG_RS232_MODE, RISC_ACTIVE);
    RS232_WRITE32(REG_BUFFER_SIZE, SINGLE_BYTE);
    RS232_WRITE32(REG_U0_BUFCTL, DEF_BUFCTL);

    // Turn UART TX pad.
        vIO32Write4B(PDWNC_GPIOEN, (u4IO32Read4B(PDWNC_GPIOEN) | PAD_UR0_TX_EN));

//#ifndef CC_NAND_BOOT
#if 0
    // if ROM enable & ROM is not moved yet, remove ROM and goto 0 again.
    CKGEN_WRITE32(0x10, 0xffffffff);
    if ((((BIM_READ32(0x664) & (0xfU << 20))==(0xfU << 20)) ||
         ((BIM_READ32(0x664) & (0xfU << 20))==(0x7U << 20)) ||
         ((BIM_READ32(0x664) & (0xfU << 20))==(0xbU << 20)) ||
         ((BIM_READ32(0x664) & (0xfU << 20))==(0xdU << 20)) ||
         ((BIM_READ32(0x664) & (0xfU << 20))==(0xeU << 20)) ||
         (CKGEN_READ32(0) & (1U << 12))) &&
        (BIM_READ32(0x4) == 0))
    {
        BIM_WRITE32(0x4, 0x80000000);
        CKGEN_WRITE32(0x10, 0);
        return 1;
    }
#endif /* CC_NAND_BOOT */
    return 0;
}

UINT32 CHIP_GetStack()
{
#ifdef CC_MTK_PRELOADER
    #ifdef CC_NAND_BOOT
        return SRAM_DMX_END - 0x1000;  // DMAX SRAM - 4K
    #else /* CC_NAND_BOOT */
        if (!(BIM_READ32(REG_RW_REMAP)))
        {
            if (BIM_READ32(REG_RO_FPGA_ID))
            {
                // if no DMX,OSD SRAM in FPGA...
                return SRAM_ITCM_BASE + 0x1000;  // ITCM + 4K
            }
            else
            {
                #if 1 // def CC_MT8223
                //return SRAM_OSD_END - 0x1000;  // OSD SRAM - 4K
                return SRAM_ITCM_BASE + 0x1000;  // ITCM + 4K
                #else
                return SRAM_DMX_END - 0x1000;  // DMAX SRAM - 4K
                #endif
            }
        }
        else
        {
            return 0x00cf0000;  // DRAM ready, use 12.75MB as stack.
        }
    #endif /* CC_NAND_BOOT */
#else /* CC_MTK_PRELOADER */
    #ifdef CC_MTK_LOADER
        return (CC_LOADER_MEM_OFFSET - 0x10000);    // DRAM ready, loader use 12.75MB as stack.
    #else /* CC_MTK_LOADER */
        return SRAM_OSD_BASE;  // TV image, use LZHS buffer as stack, but TV image won't call this function.
    #endif /* CC_MTK_LOADER */
#endif /* CC_MTK_PRELOADER */
}

#if defined(CC_MTK_PRELOADER) && defined(NDEBUG)

void CHIP_DisplayDigit(UINT32 u4Char)
{
    if (u4Char <= 9) { u4Char += '0'; }
    if ((u4Char >= 0x0a) && (u4Char <= 0x0f)) { u4Char += ('a' - 0x0a); }
    CHIP_DisplayChar(u4Char);
}

#endif /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */

void CHIP_DisplayString(const INT8 *szString)
{
    if(szString == NULL)
    {
        return;
    }
    
    while (*szString != 0)
    {
        // busy wait if no tx buffer slot.
        CHIP_DisplayChar(((UINT32)((UINT8)(*szString))));
        szString++;
    }
}

void CHIP_DisplayChar(UINT32 u4Char)
{
    while ((RS232_READ32(REG_U0_STATUS) & STATUS_TXBUF) != STATUS_TXBUF) { }
    RS232_WRITE32(REG_U0_OUTPORT, (UINT32)(u4Char & 0xff));
    if (u4Char == ((UINT32)'\n'))
    {
        RS232_WRITE32(REG_U0_OUTPORT, (UINT32)'\r');
    }
}

UINT32 CHIP_IsRemap()
{
#ifdef CC_RUN_ON_NOR_EMPTY
    return 1;
#else /* CC_RUN_ON_NOR_EMPTY */
    return (BIM_READ32(REG_RW_REMAP) & REMAP_ENABLE);
#endif /* CC_RUN_ON_NOR_EMPTY */
}

void put_num(const CHAR c)
{
    CHIP_DisplayChar((UINT32)(UINT8)(c + '0'));
}

void CHIP_DisplayInteger(INT32 i)
{
    INT32 j, k;

    if (i < 0)
    {
        CHIP_DisplayChar('-');
        i = 0 - i;
    }

    k= i / 100;
    j = (i-k*100) / 10;
    i = i - (j * 10) - (k * 100);

    if (k != 0)
    {
        put_num(k);
    }

    if ( (j != 0) || ((j==0)&&(k!=0)) )
    {
        put_num(j);
    }
    put_num(i);
}

