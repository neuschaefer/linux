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
 *      Memory test module
 *
 *---------------------------------------------------------------------------*/

#include "x_lint.h"

LINT_SAVE_AND_DISABLE

#include "memtest.h"
#include "x_printf.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"
#include "x_os.h"

#define byte_write(addr, val)	*(volatile unsigned char *)(addr) = (val)
#define hw_write(addr, val)		*(volatile unsigned short *)(addr) = (val)
#define word_write(addr, val)	*(volatile unsigned long *)(addr) = (val)

#define byte_read(addr) 		(*(volatile unsigned char*)(addr))
#define hw_read(addr)	    	(*(volatile unsigned short *)(addr))
#define word_read(addr)		    (*(volatile unsigned long *)(addr))

extern void _errlog(int, int, int, const char*, int);
extern void flush_dcache(void);

#define errlog(offset, val1, val2)  _errlog((offset), (val1), (val2), __FILE__, __LINE__)

//---------------------------------------------------------------------------

#ifdef BIG_ENDIAN

#define addr2seed_16(addr)	    ((((addr) & 0xff) << 8) | (((addr) + 1) & 0xff))
#define addr2seed_32(addr)  	((((addr) & 0xff) << 24) | ((((addr) + 1) & 0xff) << 16) | \
                                ((((addr) + 2) & 0xff) << 8) | (((addr) + 3) & 0xff) )

#define tx32to16hi(value)		((value) >> 16)
#define tx32to16lo(value)		((value) & 0xffff)

#else   // BIG_ENDIAN

#define addr2seed_16(addr)	    ((((addr + 1) & 0xff) << 8) | (((addr)) & 0xff))
#define addr2seed_32(addr)  	((((addr + 3) & 0xff) << 24) | ((((addr) + 2) & 0xff) << 16) | \
				                ((((addr) + 1) & 0xff) << 8) | (((addr) + 0) & 0xff) )

#define tx32to16hi(value)		((value) & 0xffff)
#define tx32to16lo(value)		((value) >> 16)

#endif  // BIG_ENDIAN

//---------------------------------------------------------------------------

static void byte_intf(unsigned long Addr, unsigned int Length)
{
	unsigned int i;
	unsigned int addr;

	unsigned short 	tmp_16, seed_16;
	unsigned long	tmp_32, seed_32;

	// sequential mode
	for (i = 0, addr = Addr; i < Length; i += 4)
	{
		byte_write(addr, addr);
		byte_write(addr + 1, addr + 1);
		byte_write(addr + 2, addr + 2);
		byte_write(addr + 3, addr + 3);
		seed_16 = addr2seed_16(addr);
		if ((tmp_16 = hw_read(addr)) != seed_16)
			errlog(addr, tmp_16, seed_16);

		seed_16 = addr2seed_16(addr + 2);
		if ((tmp_16 = hw_read(addr + 2)) != seed_16)
			errlog(addr, tmp_16, seed_16);
		seed_32 = addr2seed_32(addr);
		if ((tmp_32 = word_read(addr)) != seed_32)
			errlog(addr, tmp_32, seed_32);

		addr += 4;
	}

	x_memset((void*)Addr, Length, 0xff);

	// scanning mode
	for (i = 0, addr = Addr; i < Length; i += 4)
	{
		byte_write(addr, addr);
		byte_write(addr + 1, addr + 1);
		byte_write(addr + 2, addr + 2);
		byte_write(addr + 3, addr + 3);
		addr += 4;
	}

	for (i = 0, addr = Addr; i < Length; i += 4)
	{
		seed_16 = addr2seed_16(addr);
		if ((tmp_16 = hw_read(addr)) != seed_16)
			errlog(addr, tmp_16, seed_16);

		seed_16 = addr2seed_16(addr + 2);
		if ((tmp_16 = hw_read(addr + 2)) != seed_16)
			errlog(addr, tmp_16, seed_16);
		seed_32 = addr2seed_32(addr);
		if ((tmp_32 = word_read(addr)) != seed_32)
			errlog(addr, tmp_32, seed_32);

		addr += 4;
	}
}


static void hw_intf(unsigned long Addr, unsigned int Length)
{
	unsigned int i;
	unsigned int addr;

	unsigned char   tmp_8;
	unsigned short 	seed_16;
	unsigned long	tmp_32;

	//sequentail mode:
	for (i = 0, addr = Addr; i < Length; i += 4)
	{
		seed_16 = addr2seed_16(addr);
		hw_write(addr, seed_16);

		seed_16 = addr2seed_16(addr + 2);
		hw_write(addr + 2, seed_16);

		tmp_8 = byte_read(addr);
		if (tmp_8 != (addr & 0xff))
			errlog(addr, tmp_8, addr & 0xff);

		tmp_8 = byte_read(addr + 1);
		if (tmp_8 != ((addr + 1) & 0xff))
			errlog(addr, tmp_8, (addr + 2) & 0xff);

		tmp_8 = byte_read(addr + 2);
		if (tmp_8 != ((addr + 2) & 0xff))
			errlog(addr, tmp_8, (addr + 2) & 0xff);

		tmp_8 = byte_read(addr + 3);
		if (tmp_8 != ((addr + 3) & 0xff))
			errlog(addr, tmp_8, (addr + 3) & 0xff);

		tmp_32 = word_read(addr);

		seed_16 = hw_read(addr);

		if (tx32to16hi(tmp_32) != seed_16)
			errlog(addr, tmp_32 >> 16, seed_16);

		seed_16 = hw_read(addr + 2);
		if (tx32to16lo(tmp_32) != seed_16)
			errlog(addr, tmp_32 & 0xffff, seed_16);

		addr += 4;
	}

	x_memset((void*)Addr, 0x00, Length);

	// scanning  mode
	for (i = 0, addr = Addr; i < Length; i += 4)
	{
		seed_16 = addr2seed_16(addr);
		hw_write(addr, seed_16);

		seed_16 = addr2seed_16(addr + 2);
		hw_write(addr + 2, seed_16);

		addr += 4;
	}

	for (i = 0, addr = Addr; i < Length; i += 4)
	{
		tmp_8 = byte_read(addr);
		if (tmp_8 != (addr & 0xff))
			errlog(addr, tmp_8, addr & 0xff);

		tmp_8 = byte_read(addr + 1);
		if (tmp_8 != ((addr + 1) & 0xff))
			errlog(addr, tmp_8, (addr + 2) & 0xff);

		tmp_8 = byte_read(addr + 2);
		if (tmp_8 != ((addr + 2) & 0xff))
			errlog(addr, tmp_8, (addr + 2) & 0xff);

		tmp_8 = byte_read(addr + 3);
		if (tmp_8 != ((addr + 3) & 0xff))
			errlog(addr, tmp_8, (addr + 3) & 0xff);

		tmp_32 = word_read(addr);

		seed_16 = hw_read(addr);
		if (tx32to16hi(tmp_32) != seed_16)
			errlog(addr, tmp_32 >> 16, seed_16);

		seed_16 = hw_read(addr + 2);
		if (tx32to16lo(tmp_32) != seed_16)
			errlog(addr, tmp_32 & 0xffff, seed_16);

		addr += 4;
	}
}


static void word_intf(unsigned long Addr, unsigned int Length)
{
	unsigned int i;
	unsigned long addr;

	unsigned char   tmp_8;
	unsigned short 	tmp_16;
	unsigned long	seed_32;

	// sequential mode:
	for (i = 0, addr = Addr; i < Length; i += 4)
	{
		seed_32 = addr2seed_32(addr);
		word_write(addr, seed_32);

		tmp_8 = byte_read(addr);
		if (tmp_8 != ((addr) & 0xff))
			errlog(addr, tmp_8, addr & 0xff);

		tmp_8 = byte_read(addr + 1);
		if (tmp_8 != ((addr + 1) & 0xff))
			errlog(addr, tmp_8, (addr + 1) & 0xff);

		tmp_8 = byte_read(addr + 2);
		if (tmp_8 != ((addr + 2) & 0xff))
			errlog(addr, tmp_8, (addr + 2) & 0xff);

		tmp_8 = byte_read(addr + 3);
		if (tmp_8 != ((addr + 3) & 0xff))
			errlog(addr, tmp_8, (addr + 3) & 0xff);

		tmp_16 = hw_read(addr);

		seed_32 = word_read(addr);
		if (tx32to16hi(seed_32) != tmp_16)
			errlog(addr, seed_32 >> 16, tmp_16);

		tmp_16 = hw_read(addr + 2);
		if (tx32to16lo(seed_32) != tmp_16)
			errlog(addr, seed_32 & 0xffff, tmp_16);

		addr += 4;
	}

	x_memset((void*)Addr, 0x55, Length);

	for (i = 0,addr = Addr; i < Length; i += 4)
	{
		seed_32 = addr2seed_32(addr);
		word_write(addr, seed_32);
		addr += 4;
	}

	for (i = 0,addr = Addr; i < Length; i += 4)
	{
		tmp_8 = byte_read(addr);
		if (tmp_8 != ((addr) & 0xff))
			errlog(addr, tmp_8, (addr) & 0xff);

		tmp_8 = byte_read(addr + 1);
		if (tmp_8 != ((addr + 1) & 0xff))
			errlog(addr, tmp_8, (addr + 1) & 0xff);

		tmp_8 = byte_read(addr + 2);
		if (tmp_8 != ((addr + 2) & 0xff))
			errlog(addr, tmp_8, (addr + 2) & 0xff);

		tmp_8 = byte_read(addr + 3);
		if (tmp_8 != ((addr + 3) & 0xff))
			errlog(addr, tmp_8, (addr + 3) & 0xff);

		tmp_16 = hw_read(addr);

		seed_32 = word_read(addr);
		if (tx32to16hi(seed_32) != tmp_16)
			errlog(addr, seed_32 >> 16, tmp_16);

		tmp_16 = hw_read(addr + 2);
		if (tx32to16lo(seed_32) != tmp_16)
			errlog(addr, seed_32 & 0xffff, tmp_16);

		addr += 4;
	}
}

//===========================================================================

void memintf(unsigned int uStart, unsigned int uSize)
{
	unsigned int uNStart;

	uStart = CACHE(uStart);
	uNStart = NONCACHE(uStart);

	printf("Beginning of memory interface test...\n");

    //-----------------------------------------------------------------

    printf("  Testing non-cachable, address = 0x%08x, size = 0x%x\n",
        uNStart, uSize);

    flush_dcache();

    x_memset((void*)uNStart, 0x78, uSize);
    printf("    Byte interface testing...\n");
    byte_intf(uNStart, uSize);

    x_memset((void*)uNStart, 0x9a, uSize);
    printf("    Halfword interface testing...\n");
    hw_intf(uNStart, uSize);

    x_memset((void*)uNStart, 0xbc, uSize);
    printf("    Word interface testing...\n");
    word_intf(uNStart, uSize);

    //-----------------------------------------------------------------

    printf("  Testing cachable, address = 0x%08x, size = 0x%x\n",
        uStart, uSize);

    printf("    Byte interface testing...\n");
    flush_dcache();
    x_memset((void*)uStart, 0x12, uSize);
	byte_intf(uStart, uSize);

	printf("    Halfword interface testing...\n");
    flush_dcache();
	x_memset((void*)uStart, 0x34, uSize);
	hw_intf(uStart, uSize);

	printf("    Word interface testing...\n");
    flush_dcache();
	x_memset((void*)uStart, 0x56, uSize);
	word_intf(uStart, uSize);

    //-----------------------------------------------------------------

	printf("Memory interface test ok...\n");
}


LINT_RESTORE
