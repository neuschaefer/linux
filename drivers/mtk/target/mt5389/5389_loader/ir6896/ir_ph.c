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
 * $RCSfile: ir_ph.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "ir_debug.h"
#include "ir_ph.h"

#include "x_debug.h"


static const UINT8 _au1BitReverse[] =
{
	0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
	0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
	0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
	0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
	0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
	0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
	0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
	0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
	0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
	0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
	0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
	0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
	0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
	0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
	0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
	0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
	0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
	0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
	0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
	0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
	0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
	0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
	0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
	0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
	0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
	0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
	0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
	0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
	0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
	0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
	0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
	0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

static const UINT8 _au1InvRc5BiPhase[] =
{
	0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x02, 0x03,
	0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x02, 0x03,
	0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07,
	0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07,
	0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x02, 0x03,
	0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x02, 0x03,
	0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07,
	0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07,
	0x08, 0x09, 0x08, 0x09, 0x0a, 0x0b, 0x0a, 0x0b,
	0x08, 0x09, 0x08, 0x09, 0x0a, 0x0b, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x0e, 0x0f,
	0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x0e, 0x0f,
	0x08, 0x09, 0x08, 0x09, 0x0a, 0x0b, 0x0a, 0x0b,
	0x08, 0x09, 0x08, 0x09, 0x0a, 0x0b, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x0e, 0x0f,
	0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x0e, 0x0f,
	0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x02, 0x03,
	0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x02, 0x03,
	0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07,
	0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07,
	0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x02, 0x03,
	0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x02, 0x03,
	0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07,
	0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07,
	0x08, 0x09, 0x08, 0x09, 0x0a, 0x0b, 0x0a, 0x0b,
	0x08, 0x09, 0x08, 0x09, 0x0a, 0x0b, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x0e, 0x0f,
	0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x0e, 0x0f,
	0x08, 0x09, 0x08, 0x09, 0x0a, 0x0b, 0x0a, 0x0b,
	0x08, 0x09, 0x08, 0x09, 0x0a, 0x0b, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x0e, 0x0f,
	0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x0e, 0x0f
};

static const UINT8 _au1Rc5BiPhase[] =
{
    0x2,
    0x1
};

UINT32 IR_PH_BiPhase(UINT32 u4Input, UINT32 u4Bits)
{  
    UINT32 i;
    UINT32 u4Output = 0;
    for(i = 0; i < u4Bits; i++)
    {
        u4Output |= (_au1Rc5BiPhase[(u4Input >> i) & 0x1] << (i << 1));
    }
    return u4Output;
}

UINT8 IR_PH_ReverseBit8(UINT8 u1Input)
{
	return _au1BitReverse[u1Input];
}

UINT16 IR_PH_ReverseBit16(UINT16 u2Input)
{
//	return ((UINT16)IR_PH_ReverseBit8((UINT8)(u2Input >> 8)) |
		//(UINT16)(IR_PH_ReverseBit8((UINT8)(u2Input & 0xff)) << 8)); modify for QAC
	return ((UINT16)IR_PH_ReverseBit8((UINT8)(u2Input >> 8)) |
			(((UINT16)IR_PH_ReverseBit8((UINT8)(u2Input & 0xff))) << 8));
}

UINT32 IR_PH_ReverseBit32(UINT32 u4Input)
{
//	return ((UINT32)IR_PH_ReverseBit16((UINT16)(u4Input >> 16)) |
	//	(UINT32)(IR_PH_ReverseBit16((UINT16)(u4Input & 0xffff)) << 16)); modify for QAC
	return ((UINT32)IR_PH_ReverseBit16((UINT16)(u4Input >> 16)) |
		(((UINT32)IR_PH_ReverseBit16((UINT16)(u4Input & 0xffff))) << 16));
		
}


BOOL IR_PH_BiPhaseTest(UINT8 u1Bits, UINT32 u4Val)
{
	UINT32 u4DePhase;
	UINT32 u4ValidMask;

	u4ValidMask = BITMASK_LO(u1Bits) & SCREEN_LO;
	u4DePhase = u4Val ^ (u4Val >> 1);
	u4DePhase &= SCREEN_LO;
	return (u4DePhase ==  u4ValidMask) ? TRUE : FALSE;
}

UINT32 IR_PH_Rc5BiPhaseDecode(UINT8 u1Bits, UINT32 u4BiPhase)
{
	/* normalize */
	u1Bits &= 0xfeU;//~(UINT32)1; modify for QAC
	u4BiPhase &= BITMASK_LO(u1Bits);
	/* 4 bytes */
	return ~((UINT32)_au1InvRc5BiPhase[u4BiPhase & 0xff]				|
		(((UINT32)_au1InvRc5BiPhase[(u4BiPhase >>  8) & 0xff]) << 4)	|
		(((UINT32)_au1InvRc5BiPhase[(u4BiPhase >> 16) & 0xff]) << 8)	|
		(((UINT32)_au1InvRc5BiPhase[(u4BiPhase >> 24)]) << 12)) & 0xffff;
}

UINT32 IR_PH_Mt2Rc5Shift(UINT8 u1Bits, UINT32 u4MtCode)
{
	UINT32 u4Shifted;
	UINT32 u4Reversed;
	UINT32 u4LowNibble;
	RC5_RAW_T rRc5Raw;

	u4LowNibble = u4MtCode & 0xf;
	if ((u4LowNibble == 5) || (u4LowNibble == 9))
	{
		// try left shift 2 bits
		u4Shifted = (u4MtCode << 2) | 3;
		u4Reversed = IR_PH_ReverseBit32(u4Shifted);
		rRc5Raw.u4MtCode = u4Reversed;
		LOG(7, "start(x%01x) toggle(x%01x) address(x%03x) command(x%03x)\n",
			rRc5Raw.u.toggle, rRc5Raw.u.address, rRc5Raw.u.command);
		if (IR_PH_BiPhaseTest( 2, rRc5Raw.u.toggle)  &&
			IR_PH_BiPhaseTest(10, rRc5Raw.u.address) &&
			IR_PH_BiPhaseTest(12, rRc5Raw.u.command))
		{
			return 2;
		}
	}
	if (((u4LowNibble == 3) || (u4LowNibble == 5)) ||
        ((u4LowNibble == 11) || (u4LowNibble == 13)))
	{
		// try left shift 3 bits
		u4Shifted = (u4MtCode << 3) | 1;
		u4Reversed = IR_PH_ReverseBit32(u4Shifted);
		rRc5Raw.u4MtCode = u4Reversed;
		LOG(7, "start(x%01x) toggle(x%01x) address(x%03x) command(x%03x)\n",
			rRc5Raw.u.toggle, rRc5Raw.u.address, rRc5Raw.u.command);
		if (IR_PH_BiPhaseTest( 2, rRc5Raw.u.toggle)  &&
			IR_PH_BiPhaseTest(10, rRc5Raw.u.address) &&
			IR_PH_BiPhaseTest(12, rRc5Raw.u.command))
		{
			return 3;
		}
	}

	UNUSED(u1Bits);

	return 0;
}

UINT32 IR_PH_Mt2Rc5Decode(UINT32 u4MtCode)
{
	UINT32 u4Reversed;
	UINT32 u4PhaseDecoded;

	u4Reversed = IR_PH_ReverseBit32(u4MtCode);
	u4PhaseDecoded = IR_PH_Rc5BiPhaseDecode(32, u4Reversed);

	return u4PhaseDecoded;
}

void IR_PH_Rc5Split(UINT32 u4Rc5Code, UINT32 *pu4Start, UINT32 *pu4Toggle,
	UINT32 *pu4Address, UINT32 *pu4Command)
{
	RC5_SPLITTER_T rRc5Splitter;

	rRc5Splitter.u4Rc5Code = u4Rc5Code;

	if (pu4Start)
	{
		*pu4Start = (UINT32) rRc5Splitter.u.start;
		LOG(7, "RC5 start(%u)\n", *pu4Start);		
	}

	if (pu4Toggle)
	{
		*pu4Toggle = (UINT32) rRc5Splitter.u.toggle;
		LOG(7, "RC5 toggle(%u)\n", *pu4Toggle);			
	}

	if (pu4Address)
	{
		*pu4Address = (UINT32) rRc5Splitter.u.address;
		LOG(7, "RC5 addr(%u)\n", *pu4Address);
	}

	if (pu4Command)
	{
		*pu4Command = (UINT32) rRc5Splitter.u.command;
		LOG(7, "RC5 cmd(%u)\n", *pu4Command);
	}
}

BOOL IR_PH_Rc5Validate(UINT8 u1Bits, UINT32 u4MtCode, UINT32 u4Rc5Code)
{
	if ((u1Bits < 25) || (u1Bits > 32))
	{
		return FALSE;
	}

	UNUSED(u4MtCode);
	UNUSED(u4Rc5Code);

	return TRUE;
}

/* rc6 decoder */
UINT32 IR_PH_Mt2Rc6Decode(UINT32 u4MtCode)
{
	return IR_PH_ReverseBit32(u4MtCode);
}

void IR_PH_Rc6Split(UINT32 u4Rc6Code, UINT32 *pu4Start, UINT32 *pu4Mb, UINT32 *pu4Toggle,
	UINT32 *pu4Address, UINT32 *pu4Command)
{
	RC6_SPLITTER_T rRc6Splitter;

	rRc6Splitter.u4Rc6Mode0Code = u4Rc6Code;

	if (pu4Start)
	{
		*pu4Start = (UINT32) rRc6Splitter.u.start;
	}

	if (pu4Mb)
	{
		*pu4Mb = (UINT32) rRc6Splitter.u.mb;
	}
	
	if (pu4Toggle)
	{
		*pu4Toggle = (UINT32) rRc6Splitter.u.toggle;
	}

	if (pu4Address)
	{
		*pu4Address = (UINT32) rRc6Splitter.u.address;
	}

	if (pu4Command)
	{
		*pu4Command = (UINT32) rRc6Splitter.u.command;
	}
}

BOOL IR_PH_Rc6Validate(UINT8 u1Bits, UINT32 u4MtCode, UINT32 u4Rc6Code)
{
	RC6_SPLITTER_T rRc6Splitter;

	if ((u1Bits < 25) || (u1Bits > 32))
	{
		return FALSE;
	}

	rRc6Splitter.u4Rc6Mode0Code = u4Rc6Code;
	if ((rRc6Splitter.u.toggle == (UINT32)0) ||
		(rRc6Splitter.u.toggle == (UINT32)3) ||
		(rRc6Splitter.u.mb     != (UINT32)0) ||
		(rRc6Splitter.u.start  != (UINT32)3))
	{
		LOG(7, "RC6 start(%d) mb(%d) toggle(%d)\n\taddress(%d) command(%d) tail(%d)\n",
			rRc6Splitter.u.start,
			rRc6Splitter.u.mb,
			rRc6Splitter.u.toggle,
			rRc6Splitter.u.address,
			rRc6Splitter.u.command,
			rRc6Splitter.u.tail);
		return FALSE;
	}

	UNUSED(u4MtCode);

	return TRUE;
}

