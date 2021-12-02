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
 *         The parser driver
 *
 *---------------------------------------------------------------------------*/

#if 0

#include "x_lint.h"

// Do not lint this file
LINT_SAVE_AND_DISABLE

#include "utils.h"
#include "psr_reg.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_rand.h"
#include "x_bim.h"
#include "x_hal_926.h"
#include "x_os.h"

//---------------------------------------------------------------------

// Debug

#define MAX_ROUND_DELAY     10000
#define MIN_ROUND_DELAY     50
#define ROUND_DELAY_STEP    50

#define VLD_ID              1

static volatile UINT32 _u4RoundDelay = 100;

//=====================================================================
/*
static void _RoundDelay(void)
{
    UINT32 i;

    for (i = 0; i < _u4RoundDelay; i++)
    {
    }
}
*/

void SetRoundDelay(UINT32 u4Rd)
{
    _u4RoundDelay = u4Rd;
}

UINT32 GetRoundDelay(void)
{
    return _u4RoundDelay;
}

void IncreaseRoundDelay(void)
{
    if (_u4RoundDelay < MAX_ROUND_DELAY)
    {
        _u4RoundDelay += ROUND_DELAY_STEP;
    }
}

void DecreaseRoundDelay(void)
{
    if (_u4RoundDelay > MIN_ROUND_DELAY)
    {
        _u4RoundDelay -= ROUND_DELAY_STEP;
    }
}

//=====================================================================

#if 0

void ResetDMA(void)
{
    // Reset and re-enable DMA engine
	PARSER_WRITE32(REG_DMACTL, DMACTL_RESET);
	PARSER_WRITE32(REG_DMACTL, 0);
}

BOOL TriggerDma(UINT32 u4Src, UINT32 u4Size)
{
    HalFlushInvalidateDCache();

    // Wait for DMA ready
    while ((PARSER_READ32(REG_DMACTL) & DMACTL_RDY) == 0)
    {
    }

	while (u4Size > 0)
	{
		// Set source adderss
		PARSER_WRITE32(REG_DMASTA, u4Src);

		// Trigger DMA
		PARSER_WRITE32(REG_DMACTL, DMACTL_TRIG);

		// Wait for completion
		while ((PARSER_READ32(REG_DMACTL) & DMACTL_RDY) == 0)
		{
		}

		// Update size
		u4Size -= (u4Size > DMA_SIZE ? DMA_SIZE : u4Size);
		u4Src += DMA_SIZE;
/*
        // Debug
        _RoundDelay();

        // Debug, flow control
        {
            UCHAR ucNum;

            ucNum = MPV_GetEsmMessageNumber(VLD_ID);
            while (ucNum > 32)
            {
                x_thread_delay(1);
                ucNum = MPV_GetEsmMessageNumber(VLD_ID);
            }
        }
*/
	}

	return TRUE;
}

#endif

//=====================================================================

// The following lltoa() has been moved to bsp/printf.c
/*

// Translate unsigned long long (64 bits) value to ASCIIZ string
char* lltoa(char* dst, UINT64 u8Val, UINT32 u4Radix)
{
	char buf[64];
	int i, idx = 0;
	UINT32 u4Mod;

	if (u8Val)
	{
		while (u8Val > 0)
		{
			u4Mod = u8Val % u4Radix;
			u8Val /= u4Radix;

			if (u4Mod < 10)
				buf[idx] = '0' + u4Mod;
			else
				buf[idx] = 'a' + u4Mod - 10;

			idx++;
		}
	}
	else
	{
		buf[idx++] = '0';
	}

	if (dst != NULL)
	{
		for (i = 0; i < idx; i++)
			dst[i] = buf[idx - i - 1];

		dst[idx] = '\0';
	}

	return dst;
}
*/

//=====================================================================

UCHAR* _memcmp(UCHAR* pucS1, UCHAR* pucS2, UINT32 u4Size)
{
	UINT32 i;

	for (i = 0; i < u4Size; i++)
	{
		if (*pucS1 != *pucS2)
		{
		    Printf("Compare fail at [0x%08x] = %08x, [0x%08x] = %08x\n", pucS1, *pucS1, pucS2, *pucS2);
			return pucS1;
		}

		pucS1++;
		pucS2++;
	}

	return NULL;
}

UCHAR* _memcmp_ring(UINT32 u4S1, UINT32 u4Buf1Start, UINT32 u4Buf1End,
	UINT32 u4S2, UINT32 u4Buf2Start, UINT32 u4Buf2End, UINT32 u4Size)
{
	UINT32 i;
	UINT8 *pucS1, *pucS2;

	ASSERT(u4S1 >= u4Buf1Start && u4S1 < u4Buf1End);
	ASSERT(u4S2 >= u4Buf2Start && u4S2 < u4Buf2End);

	pucS1 = (UINT8*)u4S1;
	pucS2 = (UINT8*)u4S2;

	for (i = 0; i < u4Size; i++)
	{
		if (*pucS1 != *pucS2)
		{
		    Printf("Compare fail at [0x%08x] = %08x, [0x%08x] = %08x\n", pucS1, *pucS1, pucS2, *pucS2);
		    return pucS1;
		}

		pucS1++;
		if ((UINT32)pucS1 >= u4Buf1End)
			pucS1 = (UCHAR*)u4Buf1Start;

		pucS2++;
		if ((UINT32)pucS2 >= u4Buf2End)
			pucS2 = (UCHAR*)u4Buf2Start;
	}

	return NULL;
}

//=====================================================================

void GenerateRandomPattern(UINT32 u4Addr, UINT32 u4Size)
{
	UINT32 i, u4HeadAlign, u4WordSize;
	UCHAR* pucBuf;
	UINT32* pu4Buf;

	// Check if small size
	if (u4Size <= 16)
	{
		pucBuf = (UCHAR*)u4Addr;
		for (i = 0; i < u4Size; i++)
			pucBuf[i] = random(256);
		return;
	}

	// Generate leading unaligned bytes
	u4HeadAlign = (4 - (u4Addr & 3)) & 3;
	pucBuf = (UCHAR*)u4Addr;
	for (i = 0; i < u4HeadAlign; i++)
		pucBuf[i] = random(256);

	// Adjust address to 4-byte boundary
	pu4Buf = (UINT32*)(u4Addr + u4HeadAlign);
	u4Size -= u4HeadAlign;
	u4WordSize = u4Size / 4;

	// Generate middle aligned words
	for (i = 0; i < u4WordSize; i++)
		pu4Buf[i] = rand();

	// Generate rest unaligned bytes
	u4Size &= 3;
	pucBuf = (UCHAR*)(pu4Buf + u4WordSize);
	for (i = 0; i < u4Size; i++)
		pucBuf[i] = random(256);
}

//=====================================================================
/*
void Set7Seg(UINT16 u2Value)
{
    UINT16 p0, p1, p2, p3;
    UINT16 u2Hex;

    if (u2Value > 9999)
        u2Value %= 10000;

    p0 = u2Value % 10, u2Value /= 10;
    p1 = u2Value % 10, u2Value /= 10;
    p2 = u2Value % 10, u2Value /= 10;
    p3 = u2Value;

    u2Hex = p3 << 12 | p2 << 8 | p1 << 4 | p0;

	BIM_WRITE32(REG_SEG7, u2Hex);
}

//=====================================================================

#define LED_ADDR        0x11000000      // hardcoded, fixme

void SetLed(UINT8 u1Value)
{
	IO_WRITE8(LED_ADDR, u1Value);
}

void TickLed(void)
{
    static UINT8 u1Value = 1;

    SetLed(u1Value);

    u1Value <<= 1;
    if (u1Value == 0)
        u1Value = 1;
}
*/
//---------------------------------------------------------------------------


// Restore Lint settings
LINT_RESTORE

#endif

