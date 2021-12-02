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

/** @file x_gpio.h
 *  x_gpio.h declare all pinmux/gpio related function and parameters.
 */


#ifndef X_GPIO_H
#define X_GPIO_H

#include "x_typedef.h"
#include "hw_ckgen.h"

//-----------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//#define NORMAL_PINMUX_BITS      128 //MX0~MX3, 32*4
//#define TOTAL_PINMUX_BITS       (NORMAL_PINMUX_BITS + 32 + 1)//Plus PDMX
#define TOTAL_NORMAL_GPIO_NUM   (NORMAL_GPIO_NUM)
#define NORMAL_GPIO_NUM         132
#define MAX_GPIO_NUM            (407)

#define TOTAL_OPCTRL_NUM        37

#define TOTAL_GPIO_NUM          (OPCTRL0 + TOTAL_OPCTRL_NUM)//(NORMAL_GPIO_NUM + 8 + TOTAL_OPCTRL_NUM)
#define TOTAL_GPIO_IDX          ((NORMAL_GPIO_NUM + 31) >> 5)
#define GPIO_INDEX_MASK         ((1 << 5) - 1)
#define GPIO_TO_INDEX(gpio)     (((UINT32)(gpio)) >> 5)
#define GPIO_SUPPORT_INT_NUM	132
#define GPIO_EDGE_INTR_NUM      GPIO_SUPPORT_INT_NUM
#define TOTAL_EDGE_IDX          ((GPIO_EDGE_INTR_NUM + 31) >> 5)
#define EDGE_INDEX_MASK         ((1 << 5) - 1)



#define PINMUX0_OFFSET          CKGEN_PMUX0
#define PINMUX1_OFFSET          CKGEN_PMUX1
#define PINMUX2_OFFSET          CKGEN_PMUX2
#define PINMUX3_OFFSET          CKGEN_PMUX3
#define PINMUX4_OFFSET          CKGEN_PMUX4
#define PINMUX_MISC_OFFSET      CKGEN_PMUX_MISC

#define GPIO_OUT0_OFFSET        CKGEN_GPIOOUT0
#define GPIO_EN0_OFFSET         CKGEN_GPIOEN0
#define GPIO_IN0_OFFSET         CKGEN_GPIOIN0
#define GPIO_OUT4_OFFSET        CKGEN_GPIOOUT4
#define GPIO_EN4_OFFSET         CKGEN_GPIOEN4
#define GPIO_IN4_OFFSET         CKGEN_GPIOIN4

#define REG_GPIO_EXTINTEN0       CKGEN_EXTINTEN0
#define REG_GPIO_EXTINT0         CKGEN_EXTINT0
#define REG_GPIO_EXTINTEN3       CKGEN_EXTINTEN3
#define REG_GPIO_EXTINT3         CKGEN_EXTINT3

#define REG_GPIO_ED2INTEN0       CKGEN_ED2INTEN0
#define REG_GPIO_LEVINTEN0       CKGEN_LEVINTEN0
#define REG_GPIO_ENTPOL0         CKGEN_ENTPOL0
#define REG_GPIO_ED2INTEN3       CKGEN_ED2INTEN3
#define REG_GPIO_LEVINTEN3       CKGEN_LEVINTEN3
#define REG_GPIO_ENTPOL3         CKGEN_ENTPOL3


#define PINMUX0_WRITE(value)            vIO32Write4B(PINMUX0_OFFSET, (value))
#define PINMUX1_WRITE(value)            vIO32Write4B(PINMUX1_OFFSET, (value))
#define PINMUX2_WRITE(value)            vIO32Write4B(PINMUX2_OFFSET, (value))
#define PINMUX_MISC_WRITE(value)        vIO32Write4B(PINMUX_MISC_OFFSET, (value))
#define PINMUX_WRITE(idx, value)        vIO32Write4B((PINMUX0_OFFSET+(idx << 2)), (value))
#define PINMUX0_REG()                   u4IO32Read4B(PINMUX0_OFFSET)
#define PINMUX1_REG()                   u4IO32Read4B(PINMUX1_OFFSET)
#define PINMUX2_REG()                   u4IO32Read4B(PINMUX2_OFFSET)
#define PINMUX_MISC_REG()               u4IO32Read4B(PINMUX_MISC_OFFSET)
#define PINMUX_REG(idx)                 u4IO32Read4B((PINMUX0_OFFSET+(idx << 2)))


#define GPIO_IN_REG(idx)                u4IO32Read4B(GPIO_IN0_OFFSET+(4*(idx)))
#define GPIO_OUT_REG(idx)               u4IO32Read4B(GPIO_OUT0_OFFSET+(4*(idx)))
#define GPIO_OUT_WRITE(idx,val)         vIO32Write4B((GPIO_OUT0_OFFSET+(4*(idx))), (val))
#define GPIO_EN_REG(idx)                u4IO32Read4B(GPIO_EN0_OFFSET+(4*(idx)))
#define GPIO_EN_WRITE(idx,val)          vIO32Write4B((GPIO_EN0_OFFSET+(4*(idx))), (val))

#define GPIO_ED2INTEN_REG(num)          u4IO32Read4B(REG_GPIO_ED2INTEN0 + (GPIO_TO_INDEX(num) << 2))
#define GPIO_LEVINTEN_REG(num)          u4IO32Read4B(REG_GPIO_LEVINTEN0 + (GPIO_TO_INDEX(num) << 2))
#define GPIO_ENTPOL_REG(num)            u4IO32Read4B(REG_GPIO_ENTPOL0 + (GPIO_TO_INDEX(num) << 2))
#define GPIO_EXTINTEN_REG(num)          u4IO32Read4B(REG_GPIO_EXTINTEN0 + (GPIO_TO_INDEX(num) << 2))
#define GPIO_RISE_REG(num)			    (~ GPIO_ED2INTEN_REG(num)) & (~ GPIO_LEVINTEN_REG(num))	 & (GPIO_ENTPOL_REG(num)) &  (GPIO_EXTINTEN_REG(num))// 				(~u4IO32Read4B(REG_GPIO_ED2INTEN)) & (~u4IO32Read4B(REG_GPIO_LEVINTEN)) & u4IO32Read4B(REG_GPIO_ENTPOL) & u4IO32Read4B(REG_GPIO_EXTINTEN)
#define GPIO_FALL_REG(num)			    (~ GPIO_ED2INTEN_REG(num)) & (~ GPIO_LEVINTEN_REG(num))	 & (~ GPIO_ENTPOL_REG(num)) &  (GPIO_EXTINTEN_REG(num))						//	(~u4IO32Read4B(REG_GPIO_ED2INTEN)) & (~u4IO32Read4B(REG_GPIO_LEVINTEN)) & (~u4IO32Read4B(REG_GPIO_ENTPOL)) & u4IO32Read4B(REG_GPIO_EXTINTEN)

#define GPIO_ED2INTEN3_REG(num)          u4IO32Read4B(REG_GPIO_ED2INTEN3 + ((GPIO_TO_INDEX(num)-3) << 2))
#define GPIO_LEVINTEN3_REG(num)          u4IO32Read4B(REG_GPIO_LEVINTEN3 + ((GPIO_TO_INDEX(num)-3) << 2))
#define GPIO_ENTPOL3_REG(num)            u4IO32Read4B(REG_GPIO_ENTPOL3 + ((GPIO_TO_INDEX(num)-3) << 2))
#define GPIO_EXTINTEN3_REG(num)          u4IO32Read4B(REG_GPIO_EXTINTEN3 + ((GPIO_TO_INDEX(num)-3) << 2))
#define GPIO_RISE3_REG(num)			    (~ GPIO_ED2INTEN3_REG(num)) & (~ GPIO_LEVINTEN3_REG(num))	 & (GPIO_ENTPOL3_REG(num)) &  (GPIO_EXTINTEN3_REG(num))
#define GPIO_FALL3_REG(num)			    (~ GPIO_ED2INTEN3_REG(num)) & (~ GPIO_LEVINTEN3_REG(num))	 & (~ GPIO_ENTPOL3_REG(num)) &  (GPIO_EXTINTEN3_REG(num))

//#define GPIO_LEVEL_REG										(~u4IO32Read4B(REG_GPIO_ED2INTEN)) & u4IO32Read4B(REG_GPIO_LEVINTEN) & u4IO32Read4B(REG_GPIO_EXTINTEN)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


typedef void (* PFN_GPIO_CALLBACK)(INT32 i4Gpio, BOOL fgStatus);

typedef enum {
    GPIO_TYPE_NONE = 0,
    GPIO_TYPE_INTR_RISE = 1,        
    GPIO_TYPE_INTR_FALL = 2,
    GPIO_TYPE_INTR_BOTH = 3,
    GPIO_TYPE_INTR_LEVEL_HIGH = 4,    
    GPIO_TYPE_INTR_LEVEL_LOW = 8,
    GPIO_TYPE_LAST = 9
} GPIO_TYPE;

typedef struct
{
    INT32 i4Gpio;
    BOOL fgGpioPolarity;
    UINT8 u1LightPeriod;
    UINT8 u1RoundPeriod;
    UINT8 u1Timer;
} GPIO_LED_BLINKER_T;

typedef enum {
    GPIO_HANDLER_CKGEN = 0,
    GPIO_HANDLER_PDWNC_GPIO,
    GPIO_HANDLER_PDWNC_SRVAD,
#ifndef CC_MTK_LOADER
    //GPIO_HANDLER_MT8295,
    GPIO_HANDLER_MT5135,
#endif
    GHIO_HANDLER_WT6720    
} GPIO_HANDLER;

typedef struct {
		INT32 (*pfnIsOwner)(INT32);    
    INT32 (*pfnInit)(void);	
    INT32 (*pfnEnable)(INT32, INT32*);    
    INT32 (*pfnInput)(INT32);    
    INT32 (*pfnIntrq)(INT32, INT32*);
    INT32 (*pfnOutput)(INT32, INT32*);
    INT32 (*pfnQuery)(INT32 , INT32 *, INT32 *, INT32 *);
    INT32 (*pfnReg)(INT32, GPIO_TYPE, PFN_GPIO_CALLBACK);    
    INT32 (*pfnRegLedBlinker)(INT32, BOOL, UINT8, UINT8);
    INT32 (*pfnStop)(void);
    INT32 (*pfnVoltageLevel)(INT32 i4GpioNum);
	INT32 (*pfnSetIntExtParam)(INT32, UINT32 );
    INT32 (*pfnGetIntExtParam)(INT32, UINT32*);
} GPIO_HANDLER_FUNC_TBL;

//-----------------------------------------------------------------------------
// Public function declaration
//-----------------------------------------------------------------------------

/* GPIO functions. */
extern INT32 GPIO_Init(void);
extern INT32 GPIO_Stop(void);

/* Register query/set functions */
EXTERN INT32 GPIO_Output(INT32 i4GpioNum, INT32 *pfgSet);
EXTERN INT32 GPIO_Enable(INT32 i4GpioNum, INT32 *pfgSet);
EXTERN INT32 GPIO_Input(INT32 i4GpioNum);
EXTERN INT32 GPIO_Intrq(INT32 i4GpioNum, INT32 *pfgSet);
EXTERN INT32 GPIO_VotageLevel(INT32 i4GpioNum);

/* GPIO functions. */
EXTERN INT32 GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise);
EXTERN INT32 GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
EXTERN INT32 GPIO_SetOut(INT32 i4GpioNum, INT32 i4Val);
EXTERN INT32 GPIO_GetIn(INT32 i4GpioNum);

EXTERN INT32 GPIO_RegPeriodLevel(INT32 i4Gpio, INT32 i4HighLow, UINT32 u4Period/*mili-second*/, PFN_GPIO_CALLBACK pfnCallback);
EXTERN INT32 GPIO_SetIntExtParam(INT32 i4Gpio, UINT32 u4Arg);
EXTERN INT32 GPIO_GetIntExtParam(INT32 i4Gpio, UINT32 *pu4Arg);

#endif //X_GPIO_H
