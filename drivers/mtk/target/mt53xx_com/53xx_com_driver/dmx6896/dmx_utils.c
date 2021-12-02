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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dmx_utils.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_utils.c
 *  Demux driver - utilities
 */

#ifdef __MODEL_slt__
#undef __MODEL_slt__//build error
#endif


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_rand.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_os.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// The number of bytes in a TS packet header.
#define TS_HEADER_LEN                   (UINT8)4

// The number of bytes in the PES prefix header.
#define PES_HEADER_LEN                  (UINT8)6

// The index of the PES_packet_length from the PES header.
#define PES_PACKET_LEN0_IDX             (UINT8)4
#define PES_PACKET_LEN1_IDX             (UINT8)5


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define DWORDSWAP(u4Tmp) (((u4Tmp & 0xff) << 24) | ((u4Tmp & 0xff00) << 8) | ((u4Tmp & 0xff0000) >> 8) | ((u4Tmp & 0xff000000) >> 24))


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef DMX_CHECK_SECTION_CRC
static UINT32 _au4_crc32[256] =
{
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9,
    0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
    0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
    0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
    0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9,
    0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
    0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011,
    0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
    0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
    0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
    0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81,
    0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
    0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49,
    0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
    0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
    0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
    0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae,
    0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
    0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
    0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
    0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
    0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066,
    0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
    0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e,
    0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
    0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
    0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
    0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
    0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
    0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686,
    0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
    0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
    0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
    0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f,
    0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47,
    0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
    0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
    0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
    0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7,
    0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
    0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f,
    0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
    0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
    0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
    0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f,
    0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
    0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
    0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
    0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
    0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
    0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30,
    0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088,
    0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
    0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
    0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
    0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
    0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
    0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0,
    0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
    0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
    0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DmxGetByte
 *  Get a byte from a ring buffer
 *
 *  @param  ppu1StartAddr   The start address
 *  @param  u4EndAddr       End address
 *  @param  u4BufLen        Buffer size
 *  @param  u4Offset        Offset
 *  @param  u1Forward       1: forward direction, -1: backward direction
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
UINT8 _DMX_GetByte(UINT8** ppu1StartAddr, UINT32 u4EndAddr,
    UINT32 u4BufLen, UINT32 u4Offset, UINT8 u1Forward)
{
    UINT32 i;
    UINT8 u1Ret = 0;

    ASSERT(ppu1StartAddr != NULL);

    for(i = 0; i < u4Offset; i++)
    {
        (*ppu1StartAddr) += 1;
        if ((UINT32)(*ppu1StartAddr) >= u4EndAddr)
        {
            (*ppu1StartAddr) -= u4BufLen;
        }
    }

    u1Ret = (**ppu1StartAddr);

    (*ppu1StartAddr) += u1Forward;
    if ((UINT32)(*ppu1StartAddr) >= u4EndAddr)
    {
        (*ppu1StartAddr) -= u4BufLen;
    }

    return u1Ret;
}


//-----------------------------------------------------------------------------
/** _DMX_Align
 *  Get an aligned address which is equals to or greater than the original
 *  address
 *
 *  @param  u4Addr          The original address
 *  @param  u4Alignment     The alignment requirement in byte
 *
 *  @retval The aligned address
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_Align(UINT32 u4Addr, UINT32 u4Alignment)
{
	UINT32 u4Unaligned;

	if (u4Alignment <= 1)
	{
		return u4Addr;
	}

	u4Unaligned = u4Addr % u4Alignment;
	if (u4Unaligned != 0)
	{
		u4Addr += u4Alignment - u4Unaligned;
	}

	return u4Addr;
}


//-----------------------------------------------------------------------------
/** _DMX_IsAligned
 *  Check if an address is aligned or not
 *
 *  @param  u4Addr          The address to be checked
 *  @param  u4Alignment     The alignment
 *
 *  @retval TRUE            Yes
 *  @retval FALSE	        No
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IsAligned(UINT32 u4Addr, UINT32 u4Alignment)
{
	return u4Addr == _DMX_Align(u4Addr, u4Alignment);
}


//-----------------------------------------------------------------------------
/** _DMX_AdvanceAddr
 *  Update address in a ring buffer by the given increment value
 *
 *  @param  u4Addr          The address to be updated
 *  @param  i4Increment     The address increment. Negtive value means
 *                          backward increment.
 *  @param  u4Wp            Current write pointer
 *  @param  u4FifoStart     FIFO start address
 *  @param  u4FifoEnd       FIFO end address
 *
 *  @retval The new address, or 0 if no enough data for the given increment
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_AdvanceAddr(UINT32 u4Addr, INT32 i4Increment, UINT32 u4Wp,
    UINT32 u4FifoStart, UINT32 u4FifoEnd)
{
	UINT32 u4DataSize0, u4DataSize, u4FifoSize;

    // Debug
    if ((u4Addr < u4FifoStart) || (u4Addr >= u4FifoEnd))
    {
        return 0;
    }

    ASSERT((u4Addr >= u4FifoStart) && (u4Addr < u4FifoEnd));
    ASSERT((u4Wp >= u4FifoStart) && (u4Wp < u4FifoEnd));

	u4FifoSize = u4FifoEnd - u4FifoStart;
	u4DataSize0 = DATASIZE(u4Addr, u4Wp, u4FifoSize);

    u4Addr = (UINT32)((INT32)u4Addr + i4Increment);
    if (u4Addr >= u4FifoEnd)
    {
        u4Addr -= u4FifoSize;
    }
    if (u4Addr < u4FifoStart)
    {
    	u4Addr += u4FifoSize;
    }

	u4DataSize = DATASIZE(u4Addr, u4Wp, u4FifoSize);
	if (i4Increment > 0)
	{
		if (u4DataSize > u4DataSize0)
		{
			// Out of data
			return 0;
		}
	}
	else
	{
		if (u4DataSize < u4DataSize0)
		{
			return 0;
		}
	}

    return u4Addr;
}


//-----------------------------------------------------------------------------
/** _DMX_CopyRingBuffer
 *  Copy data from a ring buffer to another ring buffer
 *
 *  @param  u4Dst           Destination address
 *  @param  u4DstStart      Destination buffer start address
 *  @param  u4DstEnd        Destination buffer end address
 *  @param  u4Src           Source address
 *  @param  u4SrcStart      Source buffer start address
 *  @param  u4SrcEnd        Source buffer end address
 *  @param  u4Size          Size to copy
 *
 *  @retval The destination address
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_CopyRingBuffer(UINT32 u4Dst, UINT32 u4DstStart, UINT32 u4DstEnd,
    UINT32 u4Src, UINT32 u4SrcStart, UINT32 u4SrcEnd, UINT32 u4Size)
{
    UINT32 i;
    UINT8 *pSrc, *pDst;

    ASSERT((u4Dst >= u4DstStart) && (u4Dst < u4DstEnd));
    ASSERT((u4Src >= u4SrcStart) && (u4Src < u4SrcEnd));

    pSrc = (UINT8*)u4Src;
    pDst = (UINT8*)u4Dst;

    for (i = 0; i < u4Size; i++)
    {
        *pDst++ = *pSrc++;

        if ((UINT32)pDst >= u4DstEnd)
        {
            pDst = (UINT8*)u4DstStart;
        }

        if ((UINT32)pSrc >= u4SrcEnd)
        {
            pSrc = (UINT8*)u4SrcStart;
        }
    }

    return u4Dst;
}


BOOL _DMX_CopyDestRingBuffer(UINT32 u4Dst, UINT32 u4BufStart, UINT32 u4BufEnd,
    UINT32 u4Src, UINT32 u4Size)
{
    UINT32 u4BufSize;

    // Check if destination address is in buffer range
    if ((u4Dst < u4BufStart) || (u4Dst >= u4BufEnd))
    {
        return FALSE;
    }

    // Check if buffer size is sufficient
    u4BufSize = u4BufEnd - u4BufStart;
    if (u4Size > u4BufSize)
    {
        return FALSE;
    }

    // Copy data to DDI buffer
    if (u4Size > 0)
    {
        UINT32 u4StepSize;

        u4StepSize = MIN(u4BufEnd - u4Dst, u4Size);
        x_memcpy((void*)u4Dst, (void*)u4Src, u4StepSize);

        u4Src += u4StepSize;
        u4Dst += u4StepSize;
        if (u4Dst >= u4BufEnd)
        {
            u4Dst = u4BufStart;
        }

        u4Size -= u4StepSize;
    }

    if (u4Size > 0)
    {
        x_memcpy((void*)u4Dst, (void*)u4Src, u4Size);
    }

    return TRUE;
}


void DataSwap(UINT32 *pu4Dest, UINT32 *pu4Src, UINT32 u4Size, UINT32 u4Mode) 
{
    INT32 i = 0;
    UINT32 u4Tmp;
    
    if(u4Mode == 0) 
    {
        for(i =0; i < u4Size; i++) //memcpy
        {
            *(pu4Dest + i) = *(pu4Src + i);
        }
    }
    else if(u4Mode == 1) //Endien Change
    {
        for(i =0; i < u4Size; i++) 
        {
            u4Tmp = *(pu4Src + i);
            u4Tmp = DWORDSWAP(u4Tmp);
            *(pu4Dest + i) = u4Tmp;
        }
    }
    else if(u4Mode == 2) //Head Swap
    {
        for(i =0; i < u4Size; i++) 
        {
            *(pu4Dest + u4Size - 1 - i) = *(pu4Src + i);
        }
    }
    else if(u4Mode == 3) //Head Swap + Endien Change
    {
        for(i =0; i < u4Size; i++) 
        {
            u4Tmp = *(pu4Src + i);
            u4Tmp = DWORDSWAP(u4Tmp);            
            *(pu4Dest + u4Size - 1 - i) = u4Tmp;
        }
    }    
}


#if defined(CC_DMX_EMULATION) || defined(__MODEL_slt__)
void _GenerateRandomPattern(UINT32 u4Addr, UINT32 u4Size)
{
	UINT32 i, u4HeadAlign, u4WordSize;
	UCHAR* pucBuf;
	UINT32* pu4Buf;

	// Check if small size
	if (u4Size <= 16)
	{
		pucBuf = (UCHAR*)u4Addr;
		for (i = 0; i < u4Size; i++)
		{
			pucBuf[i] = random(256);
		}
		return;
	}

	// Generate leading unaligned bytes
	u4HeadAlign = (4 - (u4Addr & 3)) & 3;
	pucBuf = (UCHAR*)u4Addr;
	for (i = 0; i < u4HeadAlign; i++)
	{
		pucBuf[i] = random(256);
	}

	// Adjust address to 4-byte boundary
	pu4Buf = (UINT32*)(u4Addr + u4HeadAlign);
	u4Size -= u4HeadAlign;
	u4WordSize = u4Size / 4;

	// Generate middle aligned words
	for (i = 0; i < u4WordSize; i++)
	{
		pu4Buf[i] = (UINT32)rand();
		//pu4Buf[i] = i;
	}

	// Generate rest unaligned bytes
	u4Size &= 3;
	pucBuf = (UCHAR*)(pu4Buf + u4WordSize);
	for (i = 0; i < u4Size; i++)
	{
		pucBuf[i] = random(256);
	}
}


BOOL _MemCmp(UINT32 u4S1, UINT32 u4S2, UINT32 u4Size, UINT32* pu4DiffS1,
    UINT32* pu4DiffS2)
{
	UINT32 i;
	UINT8 *pu1S1, *pu1S2;
	UINT32 u4ErrByteCount=0;
    BOOL fgShift = FALSE;
#ifdef CA9_WORKAROUND
	HalFlushInvalidateDCache();
#endif

    pu1S1 = (UINT8*)u4S1;
    pu1S2 = (UINT8*)u4S2;

	for (i = 0; i < u4Size; i++)
	{
		if (*pu1S1 != *pu1S2)
		{
		    if (fgShift == FALSE)
		    {
		        fgShift = TRUE;
		        pu1S1 = pu1S1+(*pu1S2-*pu1S1)*188;
		        continue;
		    }
		    u4ErrByteCount++;
		    if ( u4ErrByteCount < 100 )
		    {
                Printf("Byte Error, offset=%d, 0x%x:0x%x\n", i, *pu1S1, *pu1S2);
            }
		    if (pu4DiffS1 != NULL)
		    {
		        *pu4DiffS1 = (UINT32)pu1S1;
		    }

		    if (pu4DiffS2 != NULL)
		    {
		        *pu4DiffS2 = (UINT32)pu1S2;
		    }

//			return FALSE;
		}

		pu1S1++;
		pu1S2++;
	}

    if ( u4ErrByteCount != 0 )
    {
        Printf("Error Byte Count : %d\n", u4ErrByteCount);
        return FALSE;
    }

	return TRUE;
}


UINT32 _MemCmpRing(UINT32 u4S1, UINT32 u4Buf1Start, UINT32 u4Buf1End,
	UINT32 u4S2, UINT32 u4Buf2Start, UINT32 u4Buf2End, UINT32 u4Size)
{
	UINT32 i;
	UINT8 *pu1S1, *pu1S2;

	ASSERT((u4S1 >= u4Buf1Start) && (u4S1 < u4Buf1End));
	ASSERT((u4S2 >= u4Buf2Start) && (u4S2 < u4Buf2End));

	pu1S1 = (UINT8*)u4S1;
	pu1S2 = (UINT8*)u4S2;

	for (i = 0; i < u4Size; i++)
	{
		if (*pu1S1 != *pu1S2)
		{
			//return (UINT32)pu1S1;
			return i;
		}

		pu1S1++;
		if ((UINT32)pu1S1 >= u4Buf1End)
		{
			pu1S1 = (UINT8*)u4Buf1Start;
		}

		pu1S2++;
		if ((UINT32)pu1S2 >= u4Buf2End)
		{
			pu1S2 = (UINT8*)u4Buf2Start;
		}
	}

	return 0xFFFFFFFF;
}


//-----------------------------------------------------------------------------
/* _GenerateSimpleTsHeader
 *
 * The size of all TS packets is specified by u4TsSize.  Because the first few
 * TS packets generated by this function may have Adaption Field only (i.e. no
 * payload).  It is highly recommended that u4TsSize is at least the size of a
 * few TS packets in order to avoid the case where pu1PesLen0_InTs and
 * pu1PesLen1_InTs are both NULL.
 */
//-----------------------------------------------------------------------------
BOOL _GenerateSimpleTsHeader(UINT32 u4TsAddr, UINT32 u4TsSize, UINT16 u2Pid,
    UINT32 u4PesAddr, UINT32 *pu4PesSize)
{
    UINT32 u4PesSize = 0;
    UINT8 *pu1TsHead;
    UINT8 u1AdaptationFieldControl, u1AdaptationFieldLength;
    UINT8 u1ContinuityCounter = 0xF;  // It could also be declared as "static".

    BOOL fgPesHeaderPrefixCreated = FALSE;
    UINT8 au1PesHeader[PES_HEADER_LEN] = {0x00, 0x00, 0x01, 0x33, 0x00, 0x00};
    UINT8 u1PesHeaderIdx = 0;  // must be less than PES_HEADER_LEN
    UINT8 *pu1PesLen0_InTs = NULL, *pu1PesLen1_InTs = NULL;
    UINT8 *pu1PesStartAddr = (UINT8*)u4PesAddr;

    if (u4TsAddr == 0)
    {
        return FALSE;
    }

    if (!_DMX_IsAligned(u4TsSize, 188))
    {
        return FALSE;
    }

    if (u2Pid > 0x1fff)
    {
        return FALSE;
    }
UINT32 u4PktIdx = 0;
    while (u4TsSize > 0)
    {
        UINT8 u1TsOffset, u1TsPayloadLen;

        pu1TsHead = (UINT8*)u4TsAddr;

        // Sync byte
        pu1TsHead[0] = 0x47;
if (u4PktIdx >= 0)
{
pu1TsHead[1] = 0;
pu1TsHead[2] = 0;
pu1TsHead[3] = u4PktIdx;
u4PktIdx++;
u4TsAddr += 188;
u4TsSize -= 188;
continue;
}

        // PUSI = 0, transport_error_indicator = 0, transport_priority = 0
        // PID = u2Pid
        pu1TsHead[1] = (u2Pid & 0x1f00) >> 8;
        pu1TsHead[2] = (UINT8)(u2Pid & 0xff);

        // Generate adaptation_field_control
        u1AdaptationFieldControl = random(10) + 1;
        switch (u1AdaptationFieldControl)
        {
        case 1:     // 01: No adaptation_field, payload only
            u1AdaptationFieldLength = 0;
            break;

        case 2:     // 10: Adaptation_field only, no payload
            u1AdaptationFieldLength = 184;
            break;

        default:    // 11: Adaptation_field followed by payload
            u1AdaptationFieldControl = 3;
            u1AdaptationFieldLength = random(183) + 1;  // 1 - 183
            break;
        }

        // The continuity_counter is incremented only when TS payload exists.
        if (u1AdaptationFieldControl & 0x1)
        {
            u1ContinuityCounter++;
            u1ContinuityCounter = u1ContinuityCounter & 0xF;  // if > 15, then 0.
        }

        // transport_scrambling_control = 0 (not scrambled)
        // plus adaptation_field_control and continuity_counter
        pu1TsHead[3] = (u1AdaptationFieldControl << 4) | u1ContinuityCounter;

        // Set adaptation_field_length in case it exists
        if ((u1AdaptationFieldControl & 2) != 0)
        {
            pu1TsHead[4] = u1AdaptationFieldLength - 1;
        }

        u1TsOffset = TS_HEADER_LEN + u1AdaptationFieldLength;  // TS payload offset
        ASSERT(u1TsOffset <= 188);
        u1TsPayloadLen = 188 - u1TsOffset;

        if (u1TsPayloadLen > 0)
        {
            if (!fgPesHeaderPrefixCreated)
            {
                UINT8 u1CopyCount, u1TsPayloadLenCountDown;

                u1TsPayloadLenCountDown = u1TsPayloadLen;
                u1CopyCount = 0;
                while (u1TsPayloadLenCountDown > 0)
                {
                    if (u1PesHeaderIdx == 0)
                    {
                        pu1TsHead[1] = pu1TsHead[1] | (1 << 6);  // Set the PUSI bit.
                    }
                    else if (u1PesHeaderIdx == PES_PACKET_LEN0_IDX)
                    {
                        pu1PesLen0_InTs = (UINT8*)(pu1TsHead + u1TsOffset + u1CopyCount);
                    }
                    else if (u1PesHeaderIdx == PES_PACKET_LEN1_IDX)
                    {
                        pu1PesLen1_InTs = (UINT8*)(pu1TsHead + u1TsOffset + u1CopyCount);
                    }

                    if (u1PesHeaderIdx < PES_HEADER_LEN)
                    {
                        // Copy the data in PES_Header to the payload of the current TS packet.
                        pu1TsHead[u1TsOffset + u1CopyCount] = au1PesHeader[u1PesHeaderIdx];
                    }
                    else if (u1PesHeaderIdx >= PES_HEADER_LEN)
                    {
                        ASSERT(0);
                    }

                    u1TsPayloadLenCountDown--;
                    u1PesHeaderIdx++;
                    u1CopyCount++;
                    if (u1PesHeaderIdx == PES_HEADER_LEN)
                    {
                        fgPesHeaderPrefixCreated = TRUE;
                        break;
                    }
                    else if (u1PesHeaderIdx > PES_HEADER_LEN)
                    {
                        ASSERT(0);
                    }
                }
            }

            // Copy randomly generated payload to u4Pes Addr
            if (u4PesAddr != 0)
            {
                x_memcpy((void*)u4PesAddr, (void*)(u4TsAddr + u1TsOffset), u1TsPayloadLen);
                u4PesAddr += u1TsPayloadLen;
                u4PesSize += u1TsPayloadLen;
            }
        }
        u4TsAddr += 188;
        u4TsSize -= 188;
    }
#if 0
    if (u4PesSize < 6)
    {
        Printf("%s:%d: PES size < 6!\n", __func__, __LINE__);
        return FALSE;
    }
    u4PesSize -= 6;

    if (pu4PesSize != NULL)
    {
        *pu4PesSize = u4PesSize;
    }

    if ((pu1PesLen0_InTs == NULL) || (pu1PesLen1_InTs == NULL))
    {
        Printf("%s:%d: address error!\n", __func__, __LINE__);
        return FALSE;
    }
    else
    {
        *pu1PesLen0_InTs = (u4PesSize >> 8) & 0xFF;
        *pu1PesLen1_InTs = (u4PesSize >> 0) & 0xFF;
        if (pu1PesStartAddr != NULL)
        {
            pu1PesStartAddr[4] = (u4PesSize >> 8) & 0xFF;
            pu1PesStartAddr[5] = (u4PesSize >> 0) & 0xFF;
        }
    }
#endif
    return TRUE;
}


void _DumpBuffer(const UINT8 au1Buf[], UINT32 u4Size,
    UINT32 u4BytesPerLine)
{
	UINT32 i, j;

    ASSERT(au1Buf != NULL);

	j = 0;
	for (i = 0; i < u4Size; i++)
	{
		if (j == 0)
		{
			printf("\n%04x:", i);
		}

		printf(" %02x", au1Buf[i]);

		if (j == 7)
		{
			printf(" |");
		}

		j++;
		if (j == u4BytesPerLine)
		{
			j = 0;
		}
	}
	printf("\n");
}
#endif  // defined(CC_DMX_EMULATION) || defined(__MODEL_slt__)


#ifdef DMX_CHECK_SECTION_CRC
UINT32 _DMX_GenCRC32(const UINT8 *pui1_data, SIZE_T z_size)
{
    SIZE_T z_i;
    UINT32 ui4_crc32;

    ui4_crc32 = (UINT32) 0xffffffff;
    if (pui1_data != (UINT8 *) NULL)
    {
        for (z_i = (SIZE_T) 0; z_i < z_size; z_i++)
        {
            ui4_crc32 = (ui4_crc32 << 8) ^
                                _au4_crc32[(ui4_crc32 >> 24) ^ pui1_data[z_i]];
        }
    }

    return(ui4_crc32);
}
#endif  // DMX_CHECK_SECTION_CRC

