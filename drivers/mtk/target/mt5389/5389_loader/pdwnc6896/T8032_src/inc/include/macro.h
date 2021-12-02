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
/**
 * @file macro.h
 *
 * Macro function
 *
 * @author Mtk00469
 */

#ifndef _MACRO_H_
#define _MACRO_H_

/*=====================================================================
  MACRO : Enable and Disable Interrupt
=====================================================================*/
#define MEnableINT()            (EA = 1)
#define MDisableINT()           (EA = 0)
#define MEnableMainINT()        (EX0 = 1)
#define MDisableMainINT()       (EX0 = 0)
#define irq_save(flags)\
  	do {\
  		if (!EX0) {\
  			flags = 0;\
  		} else {\
  			flags = 1;\
  		}\
  		EX0 = 0;\
  	} while (0)
#define irq_restore(flags)\
  	do {\
  		if (flags == 0) {\
  			EX0 = 0;\
  		} else {\
  			EX0 = 1;\
  		}\
  	} while (0)
#define MMainIntPTLow()         (PX0 = 0)
#define MMainIntPTHigh()        (PX0 = 1)

/*=====================================================================
  MACRO : Enable and Disable Timer0/Timer1/Timer2 Interrupt/Counter
=====================================================================*/
#define MEnableTimer0()         (ET0 = 1)
#define MDisableTimer0()        (ET0 = 0)
#define MTrgTimer0Cnt()         (TR0 = 1)
#define MStopTimer0Cnt()        (TR0 = 0)
#define MTimer0PTHigh()         (PT0 = 1)
#define MTimer0PTLow()          (PT0 = 0)
#define MSetTimer0CntLow(val)   (TL0 = val)
#define MSetTimer0CntHigh(val)  (TH0 = val)
#define MSetTimer0Mode(val)     (TMOD = ((TMOD&0xf0)|val))

#define MEnableTimer1()         (ET1 = 1)
#define MDisableTimer1()        (ET1 = 0)
#define MTrgTimer1Cnt()         (TR1 = 1)
#define MStopTimer1Cnt()        (TR1 = 0)
#define MTimer1PTHigh()         (PT1 = 1)
#define MTimer1PTLow()          (PT1 = 0)
#define MSetTimer1CntLow(val)   (TL1 = val)
#define MSetTimer1CntHigh(val)  (TH1 = val)
#define MSetTimer1Mode(val)     (TMOD = ((TMOD&0x0f)|val))

#define MEnableTimer2()         (ET2 = 1)
#define MDisableTimer2()        (ET2 = 0)
#define MTrgTimer2Cnt()         (TR2 = 1)
#define MStopTimer2Cnt()        (TR2 = 0)
#define MTimer2PTHigh()         (PT2 = 1)
#define MTimer2PTLow()          (PT2 = 0)
#define MSetTimer2CntLow(val)   (RCAP2L = TL2 = val)
#define MSetTimer2CntHigh(val)  (RCAP2H = TH2 = val)
#define MSetTimer2Mode(val)     (T2CON = val)

#define MSetTimerMode(val)      (TMOD = val)

#define M8032Halt()             (PCON = 0x02)

#define vChgToFlashMode()        (P32 = 0)
#define vChgToRegMode()          (P32 = 1)

/*=====================================================================
  MACRO : byte/word access (HIGH UINT8 FIRST)
=====================================================================*/
#define bHiByte(arg)      (*(UINT8*)&arg)
#define bLoByte(arg)      (*((UINT8*)&arg + 1))

#define wHiWord(arg)      (*(UINT16*)&arg)
#define wLoWord(arg)      (*((UINT16*)&arg + 1))

#define BYTE0(arg)        (*((UINT8 *)&(arg) + 3))
#define BYTE1(arg)        (*((UINT8 *)&(arg) + 2))
#define BYTE2(arg)        (*((UINT8 *)&(arg) + 1))
#define BYTE3(arg)        (* (UINT8 *)&(arg))

#define BYTE21(arg)      (*(UINT16*)((UINT8*)&arg+1))

#define LOADL_DWRD(arg1, arg2)  BYTE3(arg2) = *((UINT8 *)(arg1) + 3); \
                                  BYTE2(arg2) = *((UINT8 *)(arg1) + 2); \
                                  BYTE1(arg2) = *((UINT8 *)(arg1) + 1); \
                                  BYTE0(arg2) = *((UINT8 *)(arg1))

/*=====================================================================
  MACRO : Get table size
=====================================================================*/
#define TBL_SIZE(tbl)   (sizeof(tbl)/sizeof(tbl[0]))

/*=====================================================================
  MACRO : For BCD number
=====================================================================*/
#define BCD_HI(bcd)  ((bcd) >> 4)
#define BCD_LO(bcd)  ((bcd) & 0x0f)
#define BCD2DEC(bcd) ((BCD_HI(bcd) * 10) +BCD_LO(bcd))
#define DEC2BCD(dec) ((((dec) / 10) << 4) + ((dec) % 10))

/*=====================================================================
  MACRO : For Digial to HEX in ASCII conversion
=====================================================================*/
#define bDigToAsc(bDig) (((bDig) > 9) ? (((bDig) - 10) + 'A') : ((bDig) + '0'))

/*=====================================================================
  MACRO : Number Comparison Macro
=====================================================================*/
#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*=====================================================================
  MACRO : Expand Value to Meet Special Request
=====================================================================*/
#define MAKE_EVEN(val) /*lint -save -e572*/((((val) + 1) >> 1) << 1) /*lint -restore*/	/* to be even */
#define MAKE_ODD(val) ((val) | 0x01)	/* to be odd */

  /* Expand given value to be multiple of 8 in upper alignment */
#define MAKE_8X(val) ((((val) + 0x07) >> 3) << 3)

  /* Expand given value to be multiple of 16 in upper alignment */
#define MAKE_16X(val) ((((val) + 0x0F) >> 4) << 4)

  /* Make given value to be multiple of 16 in center alignment */
#define MAKE_16X_EX(val) ((((val) + 0x08) >> 4) << 4)

  /* Expand given value to be multiple of 4 in center alignment */
#define  MAKE_4X(val)  ((((val) + 0x01) >> 2) << 2)

  /* Expand given value to be multiple of 32 in upper alignment */
#define MAKE_32X(val) ((((val) + 0x1F) >> 5) << 5)

  /* Expand given value to be multiple of 64 in upper alignment */
#define MAKE_64X(val) ((((val) + 0x3F) >> 6) << 6)

/*=====================================================================
 *  Description:
 *    Make a item position with same step
 *  Parameters:
 *    start		base start position of all items
 *    index		index of the item to be calculated
 *    step		step of all items
=====================================================================*/
#define MAKE_ITEM_POS(start, index, step) ((start) + ((index) * (step)))

/*=====================================================================
  MACRO : Swap Integers Macro
=====================================================================*/
#define SWAP(a, b)     {(a) ^= (b); (b) ^= (a); (a) ^= (b);}

/*=====================================================================
  MACRO : Retrieve Value from Pointer
=====================================================================*/
#define dwGetPtrVal(ptr)  (*((UINT32 *) (ptr)))	/* Get DWORD value from a pointer */
#define wGetPtrVal(ptr)   (*((UINT16 *) (ptr)))	/* Get  UINT16 value from a pointer */
#define vSetPtrValDW(ptr, dwVal)  (*((UINT32 *)ptr) = dwVal)
#define vSetPtrValW(ptr, wVal)  (*((UINT16 *)ptr) = wVal)

#if 0
/*=====================================================================
  MACRO : Retrieve value of each digit for decimal integer
=====================================================================*/
/* Maximum digit number of decimal integer. Please extend it if needed */
#define MAX_DIGIT_NUM 4

#ifdef _MAIN_FILE_
CODE UINT16 nMagicDigit[MAX_DIGIT_NUM] = {
	1, 10, 100, 1000
};
#else
extern CODE UINT16 nMagicDigit[MAX_DIGIT_NUM];
#endif

/*  Description:
 *    Get value of each digit for decimal integer
 *  Parameters:
 *    nVal		Integer value to be parsed
 *    bIdx		index of digit to be retrieved, zero based.
 */
#define bGetDigit(nVal, bIdx) (((nVal) / nMagicDigit[bIdx]) % 10)
#endif

#define dwMemberOffset(type, member)    ((UINT32)(&(((type*)0)->member)))

#define vResetSystem() ((void (code*) (void)) 0x0000)()

#endif
