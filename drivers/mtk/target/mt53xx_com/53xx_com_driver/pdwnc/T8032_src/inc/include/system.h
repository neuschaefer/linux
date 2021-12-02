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
 * @file system.h
 *
 * This file define system configuration
 *
 * @author Mtk00469
 */

#ifndef _8202_SYSTEM_H_
#define _8202_SYSTEM_H_

/****************************************************************************
  Define system clock rate, in MHz
 ****************************************************************************/
#if  defined(CC_MT5365) || defined(CC_MT5395)
	#define SYS_CLOCK_RATE 27
#else	
	#define SYS_CLOCK_RATE 3
#endif


/****************************************************************************
  Define ST(Sampling Time) for 8032 timer, in ms
 ****************************************************************************/
#if defined(CC_MT5365) || defined(CC_MT5395)
	#define TIMER_BASE 20
#else
    #define TIMER_BASE 50
#endif
#define TIMER1_BASE 1

#define TIMER0_ST TIMER_BASE
#define TIMER1_ST TIMER1_BASE

/*=====================================================================
  REGISTER BANK SETTING
=====================================================================*/
#define MAIN_BANK   0		// General routines use bank 0
#define EX0INT_BANK 1		// EX0 interrupt use bank 1
#define TIMER0_BANK 2		// Timer 0 interrupt use bank 2
#define TIMER1_BANK 2		// Timer 1 interrupt use bank 2
#define TIMER2_BANK 2		// Timer 2 interrupt use bank 2

/*=====================================================================
  INTERRUPT VECTOR SETTING
=====================================================================*/
#define EX0INT      0		// iterrupt, EX0
#define T0INT       1		// Timer 0 interrupt
#define EX1INT       2		// iterrupt, EX1
#define T1INT       3		// Timer 1 interrupt
#define UARTINT       4	// Serial port interrupt
#define T2INT       5		// Timer 2 interrupt
#define PFINT       6		// power fail interrupt
#define WDINT       12		// watch dog timer interrupt

/****************************************************************************
  Define macro to calculate timer configuration
 ****************************************************************************/
#define MAKE_TIMER_CFG(bVal) (65535 - ((((bVal * 1000L) * SYS_CLOCK_RATE)/12) + 1))


/****************************************************************************
  Define macro to calculate timer configuration in us
 ****************************************************************************/
#define MAKE_TIMER_CFG_us(bVal) (65535 - (((bVal  * SYS_CLOCK_RATE)/12) + 1))


/****************************************************************************
  Define Timer 0 Configuration
 ****************************************************************************/
 #if 0
 #define T0_CFG         MAKE_TIMER_CFG_us(TIMER0_ST)
 #else
#define T0_CFG         MAKE_TIMER_CFG(TIMER0_ST)
#endif
#define TH0_CFG        ((T0_CFG & 0xFF00) >> 8)
#define TL0_CFG        (T0_CFG & 0x00FF)
#define T0_MODE        0x01	/* 16-bit timer */

/****************************************************************************
  Define Timer 1 Configuration
 ****************************************************************************/
#define T1_CFG         MAKE_TIMER_CFG(TIMER1_ST)
#define TH1_CFG        ((T1_CFG  & 0xFF00) >> 8)
#define TL1_CFG        (T1_CFG  & 0x00FF)
#define T1_MODE        0x10	/* 16-bit timer */

/****************************************************************************
  Define timer mode
 ****************************************************************************/
#define TIMER_MODE     (T0_MODE + T1_MODE)

/****************************************************************************
  Define ST(Sampling Time) for soft timer, in ms
 ****************************************************************************/
#define UPDT_TM_T0_ST    500	/* updata current time's ST -- timer 0 */
#define SYS_CLK_T1_ST    1000	/* system clock's ST -- timer 1 */

#endif				/* _8202_SYSTEM_H_ */
