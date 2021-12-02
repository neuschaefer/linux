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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_kr.c $ 
 *
 * Project:
 * --------
 *   MT5388
 *
 * Description:
 * ------------
 *    Kernel initailize functions.
 *   
 * Author:
 * -------
 *   yj_chiangyj@mtk.com.tw
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/05/27 10:47p $    
 *
 * $Revision: #1 $
****************************************************************************/
#include "general.h"
#include "hw_bim.h"
#include "hw_clk.h"
//#include "hw_rs232.h"
// *********************************************************************
// Function : void vKrDelay2us(UINT8 bValue)
// Description : Delay for a multiple of 2us (in 8032 27MHz).
// Parameter : None
// Return    : None
// *********************************************************************
void vKrDelay2us(UINT8 bValue)
{
	UINT8 bCnt;
	for (bCnt = 0; bCnt < bValue; bCnt++) 
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}

// *********************************************************************
// Function : void  vKrDelay1ms(UINT8 bValue)
// Description :
// Parameter :
// Return    :
// *********************************************************************
void vKrDelay1ms(UINT8 bValue)
{
	UINT8 i;
	for (i = 0; i < bValue; i++) 
	{
		vKrDelay2us(250);
		vKrDelay2us(250);
	}
}

// *********************************************************************
// Function : void  vKrDelay10ms(UINT8 bValue)
// Description :
// Parameter :
// Return    :
// *********************************************************************
void vKrDelay10ms(UINT8 bValue)
{
	UINT8 i, j;
	for (i = 0; i < bValue; i++)
	{
		for (j = 0; j < 20; j++)
		{
			vKrDelay2us(250);
		}
	}
}

/**
 * Initialize 8032 related hardware.
 */
void vKrInit(void)
{
	// HW Init
	
		// Board kernel specific setting.
		//vRegSetBit(REG_TURBO32, CPU_HALF_EN);

		//vRegSetBit(REG_FLASH, DUAL_FETCH_EN);	// enable 8202-8032 dual fetch
}

/**
 * Enable interrupts to 8032.
 */
void vKrEnableInt(void)
{
	/* Enable 8032 Interrupt */
	MEnableINT();
	MEnableMainINT();
}

/**
 * Disable interrupts to 8032.
 */
void vKrDisableInt(void)
{
	/* Disable 8032 Interrupt */
	MDisableINT();
	MDisableMainINT();
	
	/* Disable BIM Interrupt */
	//vDisableSysInt(INT_EN_ALL);
}

#if 0
/*
 * power off hareware module
 */
void vKrPowerOff(void)
{
}

void vKrSoftReset(void)
{
	// Disable All Interrupt
	EA = 0;
	EX0 = 0;
	EX1 = 0;
	ET0 = 0;
	ET1 = 0;
	
	vRegWrite1BOr(REG_AUX8032, SOFT_RST);
	vRegWrite1BAnd(REG_AUX8032, ~SOFT_RST);
	vRegWrite1BAnd(REG_FLASH, 0xf0);
}
#endif

 ////////////////////////////////////////////////////////////////////////////////
// Register Access Function

/**
 * @brief ExFunc2 brief explanation
 *
 * (detail explanation) ExFunc2()pass the sum of parameters
 *
 * @param a parameter 1
 * @param b parameter 1
 * @return the sum of parameters
 */
UINT16 u2RegRead2B(UINT16 reg16) reentrant
{
	UINT16 wTmp;

	bLoByte(wTmp) = XBYTE[reg16];
	bHiByte(wTmp) = XBYTE[reg16+1];

	return wTmp;
}

/**
 * @brief ExFunc2 brief explanation
 *
 * (detail explanation) ExFunc2()pass the sum of parameters
 *
 * @param a parameter 1
 * @param b parameter 1
 * @return the sum of parameters
 */
UINT32 u4RegRead4B(UINT16 reg16) reentrant
{
	UINT32 dTmp;

	BYTE0(dTmp) = XBYTE[reg16];
	BYTE1(dTmp) = XBYTE[reg16+1];
	BYTE2(dTmp) = XBYTE[reg16+2];
	BYTE3(dTmp) = XBYTE[reg16+3];

	return dTmp;
}

/**
 * @brief ExFunc2 brief explanation
 *
 * (detail explanation) ExFunc2()pass the sum of parameters
 *
 * @param a parameter 1
 * @param b parameter 1
 * @return the sum of parameters
 */
void vRegWrite2B(UINT16 reg16, UINT16 val16) reentrant
{
	XBYTE[reg16] = bLoByte(val16);
	XBYTE[reg16+1] = bHiByte(val16);
}

/**
 * @brief ExFunc2 brief explanation
 *
 * (detail explanation) ExFunc2()pass the sum of parameters
 *
 * @param a parameter 1
 * @param b parameter 1
 * @return the sum of parameters
 */
void vRegWrite2BMsk(UINT16 reg16, UINT16 val16, UINT16 msk16) reentrant
{
	vRegWrite1BMsk(reg16, bLoByte(val16), bLoByte(msk16));
	vRegWrite1BMsk(reg16+1, bHiByte(val16), bHiByte(msk16));
}

/**
 * @brief ExFunc2 brief explanation
 *
 * (detail explanation) ExFunc2()pass the sum of parameters
 *
 * @param a parameter 1
 * @param b parameter 1
 * @return the sum of parameters
 */
void vRegWrite4B(UINT16 reg16, UINT32 val32) reentrant
{
	XBYTE[reg16+3] = BYTE3(val32);	
	XBYTE[reg16+2] = BYTE2(val32);	
	XBYTE[reg16+1] = BYTE1(val32);	
	XBYTE[reg16] = BYTE0(val32);
}

/**
 * @brief ExFunc2 brief explanation
 *
 * (detail explanation) ExFunc2()pass the sum of parameters
 *
 * @param a parameter 1
 * @param b parameter 1
 * @return the sum of parameters
 */
void vRegWrite4B_S(UINT16 reg16, UINT8 bByte3, UINT8 bByte2, UINT8 bByte1, UINT8 bByte0) reentrant
{
	XBYTE[reg16] = bByte0;
	XBYTE[reg16+1] = bByte1;
	XBYTE[reg16+2] = bByte2;
	XBYTE[reg16+3] = bByte3;
}

/**
 * @brief ExFunc2 brief explanation
 *
 * (detail explanation) ExFunc2()pass the sum of parameters
 *
 * @param a parameter 1
 * @param b parameter 1
 * @return the sum of parameters
 */
void vRegWrite4BMsk(UINT16 reg16, UINT32 val32, UINT32 msk32) reentrant
{
	vRegWrite1BMsk(reg16, BYTE0(val32), BYTE0(msk32));
	vRegWrite1BMsk(reg16+1, BYTE1(val32), BYTE1(msk32));
	vRegWrite1BMsk(reg16+2, BYTE2(val32), BYTE2(msk32));
	vRegWrite1BMsk(reg16+3, BYTE3(val32), BYTE3(msk32));
}
////////////////////////////////////////////////////////////////////////////////
