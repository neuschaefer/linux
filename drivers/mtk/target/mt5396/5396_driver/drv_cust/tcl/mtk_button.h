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

#ifndef MTK_BUTTON_H
#define MTK_BUTTON_H

#if 0
static MULTI_KEY_T _arMTKMultiKeyList[] = {
    { BTN_FACTORY_MODE_1,   (DKBIT_VOL_DOWN | DKBIT_INPUT_SRC) },
    { BTN_FUNCTION_1,       (DKBIT_VOL_DOWN | DKBIT_POWER) },
    { BTN_NONE,             (DKBIT_NONE) },
};
#endif

/*According to SONY SPEC modified by Hua 0612*/
static SGL_SERVO_T _arMTKSingleServoKey[] = {
	// Servo,		Top,	Btm,	Key
#if 1
#if 1	//liufeng 110901
	{	SERVO(7),	940,    880,	 DKBIT_MENU },
	{	SERVO(7),	800,    740,	  DKBIT_ENTER },
	{	SERVO(7),	640,    580,	  DKBIT_VOL_UP},
	{	SERVO(7),	490,    430,	  DKBIT_VOL_DOWN  },
	{	SERVO(7),	340, 	280,	  DKBIT_PRG_UP	},
	{	SERVO(7),	190,	130,	  DKBIT_PRG_DOWN  },
	{	SERVO(4),	100,	0,	  DKBIT_POWER  },
#else
	// Servo,		Top,	Btm,	Key
	{	SERVO(0),	0x103,	  0x98,     DKBIT_MENU  },//101
	{	SERVO(1),	0x5a3,	  0x59f,    DKBIT_INPUT_SRC },//5a1
	{	SERVO(2),	0x3ff,	  0x3fd,    DKBIT_VOL_UP},//3ff
	{	SERVO(3),	0x10a,	  0x106,    DKBIT_VOL_DOWN  },//108
	{	SERVO(4),	0xac,	  0xa8,     DKBIT_PRG_UP	},//aa
	{	SERVO(5),	0xbe,	  0xba,		DKBIT_PRG_DOWN  },//bc
	{	SERVO(6),	0x5a,	  0x56,		DKBIT_POWER  },//58
	{	SERVO(7),	0x3ff,	  0x3fd,	DKBIT_ENTER  },//3ff
#endif	
#else
	{	SERVO(0),	0x215,	  0x211,    DKBIT_MENU  },//213
	{	SERVO(1),	0x15c,	  0x158,    DKBIT_INPUT_SRC },//15a
	{	SERVO(2),	0x3,      0x0,	    DKBIT_VOL_UP},// 1
	{	SERVO(3),	0x1e0,    0x1dc,	DKBIT_VOL_DOWN  },//1de
	{	SERVO(4),	0x101,    0xfd,	    DKBIT_PRG_UP	},//ff
	{	SERVO(5),	0x322,	  0x31e,	DKBIT_PRG_DOWN  },//320
	{	SERVO(6),	0x3d2,	  0x3ce,	DKBIT_POWER  },//3d0
	{	SERVO(7),	0x2,	  0x0,	    DKBIT_ENTER  },//0
#endif
//	{	SERVO(5),	61,    52,	  DKBIT_MENU  },
//	{	SERVO(5),	50,    39,	  DKBIT_INPUT_SRC },
//	{	SERVO(5),	38,    26,	  DKBIT_VOL_UP},
//	{	SERVO(5),	25,    16,	  DKBIT_VOL_DOWN  },
//	{	SERVO(5),	15, 	9,	  DKBIT_PRG_UP	},
//	{	SERVO(5),	7,		3,	  DKBIT_PRG_DOWN  },
/*	{	SERVO(5),	2,		0,	  DKBIT_POWER  },*/
	{	0,			0,		0,	  BTN_NONE		},
};


/*
static SGL_GPIO_T _arMTKSingleGpioKey[] = {
    // GPIO,       Btm,    Key
    {   OPCTRL0,   0,    DKBIT_POWER },
    {   0,         0,    DKBIT_NONE  },
};

#define SINGLE_GPIO_KEY_LIST		(_arMTKSingleGpioKey)
*/
#define SINGLE_SERVO_KEY_LIST       (_arMTKSingleServoKey)
// #define MULTI_KEY_LIST              (_arMTKMultiKeyList)

#endif /* MTK_BUTTON_H */

