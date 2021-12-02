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
/** @file ddr.c
 *  Dram control and setting.
 */
/*
    Register :
    0x00-0x28, 0x7C-0x80, 0x8C-0xC0, 0xD8-0xE0, 0x3DC, 0x3FC
*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "ddr.h"
#include "drvcust_if.h"
#include "x_printf.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_bim.h"

LINT_EXT_HEADER_END

//lint --e{717} do ... while (0);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#if defined(CC_5391_PRELOADER) && defined(NDEBUG)
#else /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */
UINT32 _u4DDRLogLvl = CC_DEFAULT_LOGLVL;
#endif /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */


EXTERN void CHIP_DisplayString(const INT8 *szString);
EXTERN void CHIP_DisplayInteger(INT32 i);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _DdrSetRefcnt(UINT32 u4Clk)
{
    ASSERT((u4Clk & 0xFF) == u4Clk);

    DRAM_WRITE32(0x08, (DRAM_READ32(0x08) & (~0x000000FF)) | (u4Clk & 0xFF));
}

static void _DdrSetDramBeforeAI(UINT32 u4Clk, UINT32 fgFull, UINT32 fgDDR2,
    UINT32 fg64En, UINT32 fgDQS, UINT32 fgGDDR, UINT32 u4ChipNum)
{
    UNUSED(fgDQS);

    if (fgDDR2)
    {
        _DDR_SetDdr2BeforeAI(u4Clk, fgFull, fg64En, fgDQS, fgGDDR, u4ChipNum);
    }
    else
    {
        _DDR_SetDdr3BeforeAI(u4Clk, fgFull, fg64En, fgDQS, fgGDDR, u4ChipNum);
    }
}

static void _DdrSetDramAfterAI(UINT32 u4Clk, UINT32 fgFull, UINT32 fgDDR2,
    UINT32 fg64En, UINT32 fgDQS, UINT32 fgGDDR, UINT32 u4ChipNum)
{
    UINT32 u4DramRefreshCycle;

    UNUSED(fgDQS);
    UNUSED(u4ChipNum);

#if ((DEFAULT_DRAM_TYPE==DDR_III_x1) || (DEFAULT_DRAM_TYPE==DDR_III_x2))
    u4DramRefreshCycle = ((u4Clk / 1000000) * 39) / 160;
#else
    u4DramRefreshCycle = ((u4Clk / 1000000) * 39) / 160;
#endif

    if (!fgFull)
    {
        u4DramRefreshCycle = u4DramRefreshCycle >> 1;
    }

    _DdrSetRefcnt(u4DramRefreshCycle);

    if (fgDDR2)
    {
        _DDR_SetDdr2AfterAI(u4Clk, fgFull, fg64En, fgDQS, fgGDDR, u4ChipNum);
    }
    else
    {
        _DDR_SetDdr3AfterAI(u4Clk, fgFull, fg64En, fgDQS, fgGDDR, u4ChipNum);
    }
}


void CHIP_Delay1_us(UINT32 u4Micros)
{
    UINT32 u4Xtal = 27*u4Micros;
    UINT64 u8Current;
    UINT64 u8TimeStop;

    switch(XTAL_STRAP_MODE) {
    case SET_XTAL_27MHZ: u4Xtal = 27*u4Micros; break;
    case SET_XTAL_60MHZ: u4Xtal = 60*u4Micros; break;
    case SET_XTAL_48MHZ: u4Xtal = 48*u4Micros; break;
    case SET_XTAL_54MHZ: u4Xtal = 54*u4Micros; break;
    default:
//       while (1) { }
        break;
    }
    do {
        u8Current = BIM_READ32(REG_RW_TIMER2_HIGH);
        u8Current = u8Current << 32;
        u8Current |= BIM_READ32(REG_RW_TIMER2_LOW);
        u8TimeStop = u8Current - u4Xtal;
    } while (u8TimeStop > u8Current);
    do {
        u8Current = BIM_READ32(REG_RW_TIMER2_HIGH);
        u8Current = u8Current << 32;
        u8Current |= BIM_READ32(REG_RW_TIMER2_LOW);
    } while (u8TimeStop < u8Current);
}

#if (DEFAULT_DRAM_TYPE==DDR_III_x1)
static void _ScanDQSGatingWindow()
{
#if 1				//Gating window auto search
	UINT32 i = 0;
	UINT32 reg, reg1;
	UINT8 lowbound = 0xFF, highbound=0xFF;
	
	for(i=0; i<0x80; i++)
	{
		//_DdrSetDqsIen(i);		//Set DQS gating window delay
		reg = BIN_2_GREY(i & 0xFF) ;
		reg = (reg << 24) | (reg << 16) | (reg << 8) | reg;
		DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | reg); // Need to do early	
		
		reg = DRAM_READ32(0x08);
		DRAM_WRITE32(0x08, reg|0x70000000);		//Enable Test Agent 0~2
		while(  ((DRAM_READ32(0x3FC)&0x00070000) != 0x00070000) )
			;	//Waiting for Test agent 0~2 completed

		//WAIT_DRAM_STABLE();
		//Check DQS0~3 counter
		if( (DRAM_READ32(0x38C) == 0x03) && (DRAM_READ32(0x390) == 0x03) && (DRAM_READ32(0x394) == 0x03) && (DRAM_READ32(0x398) == 0x03) )
		{
			if( lowbound == 0xFF )
			{
				lowbound = i;
				i = i+5;			// Jump 5 steps for boundary skipping.
				i &= 0x7F;		// i value boundary check
			}
		}
		else
		{
			if( (lowbound!=0xFF) && ((i-lowbound) < 10) )
			{
				lowbound = 0xFF;			//Reset lowbound if not stable
			}
			
			if(  (lowbound!=0xFF) && (highbound == 0xFF) )
			{
				highbound = i-1;
			}

			//Reset gating window delay for next value	, PHY reset is a must only for wrong gating value.
			_DDR_ResetPhy();

		}

		DRAM_WRITE32(0x08, reg & 0x8FFFFFFF);		//Disable Test Agent 0~2
	
		if(highbound != 0xFF)
		{
			break;
		}
		
	}

	highbound &= 0x7F;				//highbound boundary check, if i=0x7F, still OK, highbound would be 0xFF => error!
	
	//INIT DDR
	reg = DRAM_READ32(0x08);
	reg1 = DRAM_READ32(0x04);
	DRAM_WRITE32(0x08, reg&0xFFFFFF00);
	
	DRAM_WRITE32(0x04, 0x00010000);
	CHIP_Delay1_us(1);
	//WAIT_DRAM_STABLE();

	DRAM_WRITE32(0x04, reg1);
	DRAM_WRITE32(0x08, reg);


	//Dump Message 
	CHIP_DisplayString("Gating Window = ");
	CHIP_DisplayInteger(lowbound);
	CHIP_DisplayString(" ~ ");
	CHIP_DisplayInteger(highbound);
	
	highbound = (highbound+lowbound)/2;
	//_DdrSetDqsIen(highbound);
	reg = BIN_2_GREY(highbound & 0xFF) ;
	reg = (reg << 24) | (reg << 16) | (reg << 8) | reg;
	DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | reg); // Need to do early	
	
	CHIP_DisplayString(", Set ");
	CHIP_DisplayInteger(highbound);
	CHIP_DisplayString("\n");
#else

	UINT32 reg;

	//Dump Message
	CHIP_DisplayString("Gating Window = ");
	CHIP_DisplayInteger(DDR_DQSIEN);
	CHIP_DisplayString("\n");
	
	reg = BIN_2_GREY(DDR_DQSIEN & 0xFF) ;
	reg = (reg << 24) | (reg << 16) | (reg << 8) | reg;
	DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | reg); // Need to do early	

	//CHIP_Delay1_us(2);
#endif	// Gating window auto search
}
#endif	//if DEFAULT_DRAM_TYPE==DDR_III_x1 x2


#if (DEFAULT_DRAM_TYPE==DDR_III_x2)
static void _ScanDQSGatingWindow_RK(UINT32 *lowbound, UINT32 *highbound)
{
	#if 1				//Gating window auto search
	UINT32 i = 0;
	UINT32 reg, reg1;
	//UINT8 lowbound = 0xFF, highbound=0xFF;

	*lowbound = 0xFF;
	*highbound = 0xFF;
	
	for(i=0; i<0x80; i++)
	{
		//_DdrSetDqsIen(i);		//Set DQS gating window delay
		reg = BIN_2_GREY(i & 0xFF) ;
		reg = (reg << 24) | (reg << 16) | (reg << 8) | reg;
		DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | reg); // Need to do early	
		
		reg = DRAM_READ32(0x08);
		DRAM_WRITE32(0x08, reg|0x70000000);		//Enable Test Agent 0~2
		while(  ((DRAM_READ32(0x3FC)&0x00070000) != 0x00070000) )
			;	//Waiting for Test agent 0~2 completed

		//WAIT_DRAM_STABLE();
		//Check DQS0~3 counter
		if( (DRAM_READ32(0x38C) == 0x03) && (DRAM_READ32(0x390) == 0x03) && (DRAM_READ32(0x394) == 0x03) && (DRAM_READ32(0x398) == 0x03) )
		{
			if( *lowbound == 0xFF )
			{
				*lowbound = i;
				i = i+5;			// Jump 5 steps for boundary skipping.
				i &= 0x7F;		// i value boundary check
			}
		}
		else
		{
			if( (*lowbound!=0xFF) && ((i-*lowbound) < 10) )
			{
				*lowbound = 0xFF;			//Reset lowbound if not stable
			}
			
			if(  (*lowbound!=0xFF) && (*highbound == 0xFF) )
			{
				*highbound = i-1;
			}

			//Reset gating window delay for next value	, PHY reset is a must only for wrong gating value.
			_DDR_ResetPhy();

		}

		DRAM_WRITE32(0x08, reg & 0x8FFFFFFF);		//Disable Test Agent 0~2
	
		if(*highbound != 0xFF)
		{
			break;
		}
		
	}

	*highbound &= 0x7F;				//highbound boundary check, if i=0x7F, still OK, highbound would be 0xFF => error!
	
	//INIT DDR
	reg = DRAM_READ32(0x08);
	reg1 = DRAM_READ32(0x04);
	DRAM_WRITE32(0x08, reg&0xFFFFFF00);
	
	DRAM_WRITE32(0x04, 0x00010000);
	CHIP_Delay1_us(1);
	//WAIT_DRAM_STABLE();

	DRAM_WRITE32(0x04, reg1);
	DRAM_WRITE32(0x08, reg);


	//Dump Message 
	CHIP_DisplayString("Gating Window = ");
	CHIP_DisplayInteger(*lowbound);
	CHIP_DisplayString(" ~ ");
	CHIP_DisplayInteger(*highbound);
	CHIP_DisplayString("\n");

/*	
	highbound = (highbound+lowbound)/2;
	//_DdrSetDqsIen(highbound);
	reg = BIN_2_GREY(highbound & 0xFF) ;
	reg = (reg << 24) | (reg << 16) | (reg << 8) | reg;
	DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | reg); // Need to do early	
	
	CHIP_DisplayString(", Set ");
	CHIP_DisplayInteger(highbound);
	CHIP_DisplayString("\n");*/
	#else

	UINT32 reg;

	//Dump Message
	CHIP_DisplayString("Gating Window = ");
	CHIP_DisplayInteger(DDR_DQSIEN);
	CHIP_DisplayString("\n");
	
	reg = BIN_2_GREY(DDR_DQSIEN & 0xFF) ;
	reg = (reg << 24) | (reg << 16) | (reg << 8) | reg;
	DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | reg); // Need to do early	

	//CHIP_Delay1_us(2);
	#endif	// Gating window auto search
}


static void _ScanDQSGatingWindow()
{
	#if 1
		UINT32 lowbound_rk0, highbound_rk0, lowbound_rk1, highbound_rk1;
		UINT32 reg04, reg08, reg110, reg;
		
		_ScanDQSGatingWindow_RK(&lowbound_rk0, &highbound_rk0);

		reg04 = DRAM_READ32(0x04);
		reg08 = DRAM_READ32(0x08);
		reg110 = DRAM_READ32(0x110);

		DRAM_WRITE32(0x08, reg08&0xFFFFFF00);
		DRAM_WRITE32(0x110, reg110^0x08);
	    DRAM_WRITE32(0x04, 0x00010000);
		CHIP_Delay1_us(10);// WAIT_DRAM_STABLE(); will not worked in gcc -O2

		DRAM_WRITE32(0x08, reg08);
	    DRAM_WRITE32(0x04, reg04);

		_ScanDQSGatingWindow_RK(&lowbound_rk1, &highbound_rk1);

		
		lowbound_rk0 = (lowbound_rk0 > lowbound_rk1) ? lowbound_rk0 : lowbound_rk1;
		highbound_rk0 = (highbound_rk0 < highbound_rk1) ? highbound_rk0 : highbound_rk1;

		CHIP_DisplayString("Final Gating Window = ");
		CHIP_DisplayInteger(lowbound_rk0);
		CHIP_DisplayString(" ~ ");
		CHIP_DisplayInteger(highbound_rk0);
		highbound_rk0 = (highbound_rk0+lowbound_rk0)/2;
		//_DdrSetDqsIen(highbound);
		reg = BIN_2_GREY(highbound_rk0 & 0xFF) ;
		reg = (reg << 24) | (reg << 16) | (reg << 8) | reg;
		DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | reg); // Need to do early	
	
		CHIP_DisplayString(", Set ");
		CHIP_DisplayInteger(highbound_rk0);
		CHIP_DisplayString("\n");

		//restore
		DRAM_WRITE32(0x08, reg08&0xFFFFFF00);
		DRAM_WRITE32(0x110, reg110);
	    DRAM_WRITE32(0x04, 0x00010000);
		CHIP_Delay1_us(10);// WAIT_DRAM_STABLE(); will not worked in gcc -O2

		DRAM_WRITE32(0x08, reg08);
	    DRAM_WRITE32(0x04, reg04);


	#else

	UINT32 reg;

	//Dump Message
	CHIP_DisplayString("Gating Window = ");
	CHIP_DisplayInteger(DDR_DQSIEN);
	CHIP_DisplayString("\n");
	
	reg = BIN_2_GREY(DDR_DQSIEN & 0xFF) ;
	reg = (reg << 24) | (reg << 16) | (reg << 8) | reg;
	DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | reg); // Need to do early	

	//CHIP_Delay1_us(2);
	#endif
}

#endif	//if DEFAULT_DRAM_TYPE==DDR_III_x1 x2


#if ((DEFAULT_DRAM_TYPE==DDR_II_x1) || (DEFAULT_DRAM_TYPE==DDR_II_x2))

static void _ScanDQSGatingWindow()
{
#if 0				//Gating window auto search
	UINT32 i = 0;
	UINT32 reg, reg1;
	UINT8 lowbound = 0xFF, highbound=0xFF;
	UINT32 reg00, reg04, reg08;


	reg00 = DRAM_READ32(0x00);
	reg04 = DRAM_READ32(0x04);
	reg08 = DRAM_READ32(0x08);

	DRAM_WRITE32(0x08, (reg08&0xfffff8ff)|0x200);	//BL=4
	DRAM_WRITE32(0x04, 0x00010000);
	CHIP_Delay1_us(10);

	DRAM_WRITE32(0x00, reg00|0x8000);	//bit15 = 1
	DRAM_WRITE32(0x04, reg04);
	DRAM_WRITE32(0x08, 0x20);

	Printf("reg00=%x reg04=%x, reg08=%x\n", DRAM_READ32(0x00),DRAM_READ32(0x04),DRAM_READ32(0x08));
	for(i=0; i<0x80; i++)
	{
		//_DdrSetDqsIen(i);		//Set DQS gating window delay
		reg = BIN_2_GREY(i & 0xFF) ;
		reg = (reg << 24) | (reg << 16) | (reg << 8) | reg;
		DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | reg); // Need to do early	

		DRAM_WRITE32(0x08, reg08|0x70000000);		//Enable Test Agent 0~2

//		Printf("old reg00=%x reg04=%x, reg08=%x\n", reg00,reg04,reg08);
//		Printf("new reg00=%x reg04=%x, reg08=%x\n", DRAM_READ32(0x00),DRAM_READ32(0x04),DRAM_READ32(0x08));
//		CHIP_Delay1_us(1);

		while(  ((DRAM_READ32(0x3FC)&0x00070000) != 0x00070000) )
			;	//Waiting for Test agent 0~2 completed

//		CHIP_Delay1_us(1);

		//WAIT_DRAM_STABLE();
		//Check DQS0~3 counter
		if( (DRAM_READ32(0x38C) == 0x03) && (DRAM_READ32(0x390) == 0x03) && (DRAM_READ32(0x394) == 0x03) && (DRAM_READ32(0x398) == 0x03) )
		{
			if( lowbound == 0xFF )
			{
				lowbound = i;
				i = i+5;			// Jump 5 steps for boundary skipping.
				i &= 0x7F;		// i value boundary check
			}
		}
		else
		{
			Printf("gating=%d, 0x38c=0x%x, 0x390=0x%x, 0x394=0x%x, 0x398=0x%x\n",i, DRAM_READ32(0x38C), DRAM_READ32(0x390),DRAM_READ32(0x394),DRAM_READ32(0x398));

			if( (lowbound!=0xFF) && ((i-lowbound) < 10) )
			{
				lowbound = 0xFF;			//Reset lowbound if not stable
			}
			
			if(  (lowbound!=0xFF) && (highbound == 0xFF) )
			{
				highbound = i-1;
			}

			//Reset gating window delay for next value	, PHY reset is a must only for wrong gating value.
			_DDR_ResetPhy();

		}

		DRAM_WRITE32(0x08, reg08 & 0x8FFFFFFF);		//Disable Test Agent 0~2
	
		if(highbound != 0xFF)
		{
			break;
		}
		
	}

	highbound &= 0x7F;				//highbound boundary check, if i=0x7F, still OK, highbound would be 0xFF => error!
	
	//INIT DDR
//	reg = DRAM_READ32(0x08);
//	reg1 = DRAM_READ32(0x04);
	DRAM_WRITE32(0x08, reg08&0xFFFFFF00);
	
	DRAM_WRITE32(0x04, 0x00010000);
	CHIP_Delay1_us(1);
	//WAIT_DRAM_STABLE();

	DRAM_WRITE32(0x00, reg00);
	DRAM_WRITE32(0x04, reg04);
	DRAM_WRITE32(0x08, reg08);


	//Dump Message 
	CHIP_DisplayString("Gating Window = ");
	CHIP_DisplayInteger(lowbound);
	CHIP_DisplayString(" ~ ");
	CHIP_DisplayInteger(highbound);
	
	highbound = (highbound+lowbound)/2;
	//_DdrSetDqsIen(highbound);
	reg = BIN_2_GREY(highbound & 0xFF) ;
	reg = (reg << 24) | (reg << 16) | (reg << 8) | reg;
	DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | reg); // Need to do early	
	
	CHIP_DisplayString(", Set ");
	CHIP_DisplayInteger(highbound);
	CHIP_DisplayString("\n");
#else

	UINT32 reg;

	//Dump Message
	CHIP_DisplayString("Gating Window = ");
	CHIP_DisplayInteger(DDR_DQSIEN);
	CHIP_DisplayString("\n");
	
	reg = BIN_2_GREY(DDR_DQSIEN & 0xFF) ;
	reg = (reg << 24) | (reg << 16) | (reg << 8) | reg;
	DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | reg); // Need to do early	

	//CHIP_Delay1_us(2);
#endif	// Gating window auto search
}

#endif


static void _DdrSetDdrMode(UINT32 u4Bits, UINT32 fgTurbo)
{
    UINT32 u4Val;
    switch(u4Bits)
    {
    case 64: // 64bit dram.
        u4Val = (fgTurbo) ? DDR_64_TURBO : DDR_64_NORMAL;
        break;
    case 32: // 32bit dram.
        u4Val = (fgTurbo) ? DDR_32_TURBO : DDR_32_NORMAL;
        break;
    default:
        return;
    }

    u4Val &= ~(0x00000300U);
    switch(TCMGET_COLADDR())
    {
    case COL_ADDR_BIT_8:
        u4Val |= (0 << 8);
        break;
    case COL_ADDR_BIT_9:
        u4Val |= (1 << 8);
        break;
    case COL_ADDR_BIT_10:
        u4Val |= (2 << 8);
        break;
    case COL_ADDR_BIT_11:
        u4Val |= (3 << 8);
        break;
    default:
        return;
    }
    DRAM_WRITE32(0x04, u4Val);
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

void _DDR_SetRtt(DDR_RTT_T eRtt)
{
    UINT32 u4Val;

    u4Val = (DRAM_READ32(0xE4) & ~0x0000FF00U) | (((UINT32)eRtt) << 8) |
            (((UINT32)eRtt) << 12);
    DRAM_WRITE32(0xE4, u4Val);
}

void _DDR_ResetPhy(void)
{
    DRAM_WRITE32(0xF0, DRAM_READ32(0xF0) | DDR_PHY_CTRL_1_RST);
    CHIP_Delay1_us(1000);
    DRAM_WRITE32(0xF4, DRAM_READ32(0xF4) | DDR_CTRL_1_RST);
    CHIP_Delay1_us(1000);
    DRAM_WRITE32(0xF0, DRAM_READ32(0xF0) & (~DDR_PHY_CTRL_1_RST));
    CHIP_Delay1_us(1000);
    DRAM_WRITE32(0xF4, DRAM_READ32(0xF4) & (~DDR_CTRL_1_RST));
    CHIP_Delay1_us(1000);
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#if CC_USE_ANALOG_DLL
void DDR_UseAnalogDll(void)
{
    DRAM_WRITE32(0x200, 0x00000000);
    DRAM_WRITE32(0x204, 0x92020000);
    DRAM_WRITE32(0x208, 0xF0FA0001);
    DRAM_WRITE32(0x20C, 0x000299FF);
    DRAM_WRITE32(0x210, 0x00000000);
    DRAM_WRITE32(0x214, 0x92020000);
    DRAM_WRITE32(0x218, 0xF0FA0001);
    DRAM_WRITE32(0x21C, 0x000299FF);
    DRAM_WRITE32(0x208, 0x90D00001);
    DRAM_WRITE32(0x218, 0x90D00001);
    DRAM_WRITE32(0x20C, 0x0002993F);
    DRAM_WRITE32(0x21C, 0x0002993F);
}
#endif

//----------------------------------------------------------------------------
/* DDR_SetDramController() to set DRAM controller parameter
 *  @param u4Clk    DMPLL clock divided by 2
 *  @param fgFull   0:Half, 1:Full frequency
 *  @param fgDDR2   0:DDR3, 1:DDR2
 *  @param fg32Bit  0:64bit enable, 1:64bit disable
 *  @param fgDQS    0:no default DQS parameter, 1:with default DQS setting,
 *                  2:force single DQS setting.
 */
//----------------------------------------------------------------------------
void DDR_SetDramController(UINT32 u4Clk, UINT32 fgFull, UINT32 fgDDR2,
                           UINT32 fg32Bit, UINT32 fgDQS, UINT32 u4ChipNum)
{
    UINT32 fgGDDR, fg64En;

    fg64En = !fg32Bit;

    fgGDDR = DRVCUST_InitGet(eFlagSamsungGDDR);

    LOG(1, "SetDramCtrlr(Clk:%d,Full:%d,DDR2:%d,GDDR:%d,32B:%d,DQS:%d,Chip:%d)\n",
        u4Clk, fgFull, fgDDR2, fgGDDR, fg32Bit, fgDQS, u4ChipNum);

    _DdrSetDramBeforeAI(u4Clk, fgFull, fgDDR2, fg64En, fgDQS, fgGDDR, u4ChipNum);

#if CC_DRAM_STOP_DEBUG
    CLK_QueryDramSetting("Before dram auto-init:");
#endif /* CC_DRAM_STOP_DEBUG */

    // Trigger auto initialization.
    DRAM_WRITE32(0x04, 0x00010000);
		CHIP_Delay1_us(10);// WAIT_DRAM_STABLE(); will not worked in gcc -O2

    _DdrSetDramAfterAI(u4Clk, fgFull, fgDDR2, fg64En, fgDQS, fgGDDR, u4ChipNum);
    _DdrSetDdrMode(fg32Bit ? 32 : 64, FALSE);

#if CC_DRAM_STOP_DEBUG
    CLK_QueryDramSetting("End of dram init:");
#endif /* CC_DRAM_STOP_DEBUG */


#if 1//((DEFAULT_DRAM_TYPE==DDR_III_x1) || (DEFAULT_DRAM_TYPE==DDR_III_x2))
	WAIT_DRAM_STABLE();
	_ScanDQSGatingWindow();
#endif

}

//-----------------------------------------------------------------------------
/** DDR_SetAgentPriority() Set dram group 1 agent priorities
 *  @param pu1Priorities   Priorities of aud, dmx, fci, vbi, osd, pscan,
 *                         b2r, cpu, scpos
 *  @retval none.
 */
//-----------------------------------------------------------------------------
void DDR_SetAgentPriority(const UINT8 *pu1Prio)
{
    UINT64 u8Priority = 0;
    INT32 i;
    UINT32 u4RegVal;
    UINT8 u1Prio;

    ASSERT(pu1Prio != NULL);

    for (i = 0; i < 12; i++)
    {
        u1Prio = pu1Prio[i];
        // '1' ~ '9' ==> Pri 1 ~ 9
        if ((u1Prio <= (UINT8)'9') && (u1Prio >= (UINT8)'0'))
        {
            u1Prio -= (UINT8)'0';
        }
        // 'A' ==> Pri 10
        else if ((u1Prio == (UINT8)'A') || (u1Prio == (UINT8)'a'))
        {
            u1Prio = 10;
        }
        // 'B' ==> Pri 11
        else if ((u1Prio == (UINT8)'B') || (u1Prio == (UINT8)'b'))
        {
            u1Prio = 11;
        }
        // 'C' ==> Pri 12
        else if ((u1Prio == (UINT8)'C') || (u1Prio == (UINT8)'c'))
        {
            u1Prio = 12;
        }
        else if (i == 9)    // backward compatible, for only 9 agents
        {
            u1Prio = 9;
        }
        else
        {
            ASSERT(u1Prio > 11);
            return;
        }

        u8Priority |= (((UINT64)u1Prio) << (4 * i));
    }

    DRAM_WRITE32(0x4C, (UINT32)(u8Priority & 0xffffffff));

    u4RegVal = DRAM_READ32(0x50);
    u4RegVal &= 0xffffff00;
    u4RegVal |= ((UINT32)(u8Priority >> 32) & 0x000000ff);
    DRAM_WRITE32(0x50, u4RegVal);

    u4RegVal = DRAM_READ32(0xf8);
    u4RegVal &= 0xff00ffff;
    u4RegVal |= ((UINT32)(u8Priority >> 24) & 0x00ff0000);
    DRAM_WRITE32(0xf8, u4RegVal);
}

//-----------------------------------------------------------------------------
/** DDR_SetBurstLen() Set dram group 1 agent burst length
 *  @param u4BurstLen1     Burst length of agent 1 ~ 8
 *  @param u4BurstLen2     Burst length of agent 9 ~ 16
 *  @retval none.
 */
//-----------------------------------------------------------------------------
void DDR_SetBurstLen(UINT32 u4BurstLen1, UINT32 u4BurstLen2)
{
    DRAM_WRITE32(0xCC, u4BurstLen1);
    DRAM_WRITE32(0xD0, u4BurstLen2);
}

//-----------------------------------------------------------------------------
/** DDR_SetArbitorTime() Set dram group arbiter time
 *  @param u1Group         Dram agent group (1,2,3)
 *  @param u1Time          Arbitor time (0 ~ 15)
 *  @retval none.
 */
//-----------------------------------------------------------------------------
void DDR_SetArbiterTime(UINT8 u1Group, UINT8 u1Time)
{
    UINT32 u4RegVal;

    if (u1Time > 15)
    {
        u1Time = 15;
    }

    u4RegVal = DRAM_READ32(0x50);
    switch(u1Group)
    {
    case 1:
        u4RegVal &= 0xff0fffff;
        u4RegVal |= (((UINT32)u1Time) << 20);
        break;
    case 2:
        u4RegVal &= 0xf0ffffff;
        u4RegVal |= (((UINT32)u1Time) << 24);
        break;
    case 3:
        u4RegVal &= 0x0fffffff;
        u4RegVal |= (((UINT32)u1Time) << 28);
        break;
    default:
        return;
    }
    DRAM_WRITE32(0x50, u4RegVal);
}

#if ENABLE_DRAM_DATA_SCRAMBLE == 1

void DDR_QueryDramScrambleDataKey(void)
{

#if 0
	INT32 i,i4Avg = 0;
	UCHAR uc;
    Printf("\nPress ESC to scramb dram data ... \n");
    SerTransparent();
    uc = 0;
    for (i=0; i<(200000000 >> 15); i++)
    {
        Printf("\r");
        switch(i & 0x3) {
        case 0:
            Printf("|"); break;
        case 1:
            Printf("/"); break;
        case 2:
            Printf("-"); break;
        case 3:
            Printf("\\"); break;
        default:
            continue;
        }

        if (SerInByte(&uc))
        {
            if (uc == KEY_ESCAPE)
            {
                break;
            }
        }
    }

    // Without ESC, just return.
    if (uc != KEY_ESCAPE)
    {
        SerNormalMode();
        Printf("\n");
        return ;
    } 

     i4Avg = 0;
     
			Printf("set data key as 0x0 (']' and '[' : to change)\n");
			Printf("Press '0' to exit:\n");
			SerTransparent();							
			while (1)
			{
  			// wait a key.
  			while (!SerInByte(&uc)) { }

        if (uc == '0')
        {
            break;
        }
        switch(uc)
        {
	        case ']':
	        	if(i4Avg != 0xff)
	        	{	
	        		i4Avg = i4Avg++;	
	        	}	
          break;
					case '[':
	        	if(i4Avg != 0)
	        	{	
		        	i4Avg = i4Avg--;	
		        }	
          break;        		            
        
        	default:
			    	Printf("\r%c\n", uc);
      			continue;
  			}

				Printf("set data key as 0x%2x (']' and '[' : to change) \n", i4Avg);
				Printf("Press '0' to exit:\n");

		 	}
	    SerNormalMode();	
    DRAM_WRITE32(0x104, 0xEDCB0100 | (0xff&i4Avg));		
#else
	CHIP_DisplayString("scramble\n");
	DRAM_WRITE32(0x100, DRAM_READ32(0x100) | 0x54480000);		

#endif
}
#endif     

