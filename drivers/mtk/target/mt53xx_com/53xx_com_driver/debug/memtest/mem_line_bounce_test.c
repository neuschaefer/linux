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
#include "x_hal_arm.h"
#include "x_os.h"

extern void _errlog(int, int, int, const char*, int);
#define LINE_BOUNCE_SEQUENCE_1 1
#define LINE_BOUNCE_SEQUENCE_2 2
#define errlog(offset, val1, val2)  _errlog((offset), (val1), (val2), __FILE__, __LINE__)

//#define HW_LINE_BOUNCE_PATTERN(s, i) (s == LINE_BOUNCE_SEQUENCE_1)? ((i&0x1)? (~(0x1 << (i>>1))):0x0) : ((i&0x1)? (0x1 << (i>>1)):0xffff) 
static unsigned short HW_LINE_BOUNCE_PATTERN(unsigned int s, unsigned char i)
{
	if(s == LINE_BOUNCE_SEQUENCE_1)
	{
			if(i&0x1)
				return  ~(0x1 << (i>>1));
			else
				return 0x0;
	}
	else
	{
		if(i&0x1)
			return 0x1 << (i>>1);
		else
			return 0xffff;	
	}
	
}
//#define BYTE_LINE_BOUNCE_PATTERN(s, i) (s == LINE_BOUNCE_SEQUENCE_1)? ((i&0x2)? ((i&0x1)? ~(0x1 << (i>>2)): 0xff):0x0) : ((i&0x2)? ((i&0x1)? (0x1 << (i>>2)):(0x00)):0xff) 
static unsigned char BYTE_LINE_BOUNCE_PATTERN(unsigned int s, unsigned char i)
{
	unsigned char byte;
	if(i&0x2)
	{
		if(i&0x20)
		{
			if(i&0x1)	
				byte = 0xff;
			else	
				byte = ~(0x1 << ((i&0x1f)>>2));
		}	
		else
		{
			if(i&0x1)	
				byte = ~(0x1 << ((i&0x1f)>>2));
			else	
				byte = 0xff;
		}	
	}
	else
	{
		byte = 0x00;
	}
	
	if(s == LINE_BOUNCE_SEQUENCE_1)
		return byte;
	else
		return ~byte;
}
//===========================================================================
#define memDbg_Printf(v,fmt...) if(v) printf(fmt)
int memLineBounceTest(unsigned int u4Start, unsigned int u4Size, unsigned int verbose)
{
    unsigned int uNStart, hw_size;
    int i, tmp;
    volatile unsigned short* addr;
    unsigned short u4data;
    volatile unsigned char* byte_addr;
    unsigned char byte, u4byte;

    if(u4Start & 0x00000001)
    {
        u4Start++;// half-word align
    }
    u4Size &= 0xfffffffe;

    uNStart = NONCACHE(u4Start);
    hw_size = u4Size >> 1;

    memDbg_Printf(verbose,"Beginning of memory line bounce test...\n");

    //-----------------------------------------------------------------

    memDbg_Printf(verbose,"  Testing non-cachable, address = 0x%08x, size = 0x%x\n",
        uNStart, u4Size);
    HalFlushInvalidateDCacheMultipleLine(u4Start, u4Size);
    memDbg_Printf(verbose,"    Halfword testing...\n");

	addr = (volatile unsigned short*)uNStart;

	for (i = 0; i < hw_size; i++)
		*(addr + i) = HW_LINE_BOUNCE_PATTERN(LINE_BOUNCE_SEQUENCE_1, i & 0x0000001f);

	for (i = 0; i < hw_size; i++)
	{
		tmp = *(addr + i);
		u4data = HW_LINE_BOUNCE_PATTERN(LINE_BOUNCE_SEQUENCE_1, i & 0x0000001f);
		if (tmp != u4data)
		{
                    errlog(((int)addr + (i<<1)), tmp, u4data);
                    return 0;  
		}    
	}

	for (i = 0; i < hw_size; i++)
		*(addr + i) = HW_LINE_BOUNCE_PATTERN(LINE_BOUNCE_SEQUENCE_2, i & 0x0000001f);

	for (i = 0; i < hw_size; i++)
	{
		tmp = *(addr + i);
		u4data = HW_LINE_BOUNCE_PATTERN(LINE_BOUNCE_SEQUENCE_2, i & 0x0000001f);
		if (tmp != u4data)
		{
                    errlog(((int)addr + (i<<1)), tmp, u4data);
                    return 0;  
		}    
	}

        memDbg_Printf(verbose,"    Byte testing...\n");
        byte_addr = (volatile unsigned char*)uNStart;
  
	for (i = 0; i < u4Size; i++)
		*(byte_addr + i) = BYTE_LINE_BOUNCE_PATTERN(LINE_BOUNCE_SEQUENCE_1, i & 0x0000003f);
		
	for (i = 0x00; i < u4Size; i++)
	{
		byte = *(byte_addr + i);
		u4byte = BYTE_LINE_BOUNCE_PATTERN(LINE_BOUNCE_SEQUENCE_1, i & 0x0000003f);
		if (u4byte != byte)
		{
                    errlog(((int)byte_addr + i), byte, u4byte);
                    return 0;  
		}  	  
	}

	for (i = 0; i < u4Size; i++)
		*(byte_addr + i) = BYTE_LINE_BOUNCE_PATTERN(LINE_BOUNCE_SEQUENCE_2, i & 0x0000003f);
		
	for (i = 0x00; i < u4Size; i++)
	{
		byte = *(byte_addr + i);
		u4byte = BYTE_LINE_BOUNCE_PATTERN(LINE_BOUNCE_SEQUENCE_2, i & 0x0000003f);
		if (u4byte != byte)
		{
                    errlog(((int)byte_addr + i), byte, u4byte);
                    return 0;
		}  	  
	}

	memDbg_Printf(verbose,"Memory line bounce test ok...\n");
	return 1;
}


LINT_RESTORE
