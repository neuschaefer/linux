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

/** @file pinmux.c
 *  pinmux.c contains functions to do pin selection & gpio setup.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#include "x_pdwnc_mt82.h"
#include "hw_pdwnc.h"
#include "x_pinmux_mt82.h"
#include "x_gpio_mt82.h"
#include "x_ckgen_mt82.h"
#include "x_assert.h"
#include "drvcust_if.h"
#define DEFINE_IS_LOG	//IR_IsLog
#else
#include "x_lint.h"
#include "x_bim.h"
#include "x_pdwnc.h"
#include "x_pinmux.h"
#include "x_printf.h"
#include "x_hal_5381.h"
#include "x_assert.h"
#include "drvcust_if.h"

#define DEFINE_IS_LOG	IR_IsLog
#include "x_debug.h"

#include "x_os.h"
#include "drv_common.h"
#endif

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#define code_atv code 
#else
#define code_atv  
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifndef CC_UP8032_ATV
extern UINT32 HalCriticalStart(void);
extern void HalCriticalEnd(UINT32 u4Flags);
#endif
/** number of bits used in pinmux group (sel)
 */
static const UINT8  code_atv _au1PinmuxFunctionMasks[TOTAL_PINMUX_BITS] =
{
//  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
    7, 0, 0, 0, 7, 0, 0, 0, 3, 0, 3, 0, 3, 0, 3, 0,     // PINMUX0
// 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31    
    3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 7, 0, 0, 0,     // PINMUX0

    1, 7, 0, 0, 7, 0, 0, 0, 3, 0, 3, 0, 3, 0, 3, 0,     // PINMUX1
    3, 0, 3, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0,     // PINMUX1

    7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0,     // PINMUX2
    7, 0, 0, 0, 7, 0, 0, 0, 3, 0, 3, 0, 7, 0, 3, 0,     // PINMUX2

    3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 0, 0,     // PINMUX3
    7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0,     // PINMUX3  

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     // 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     // 

    3, 0, 3, 0, 7, 0, 0, 0, 7, 0, 0, 0, 1, 1, 0, 0,     // PINMUX4
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     // PINMUX4  

    3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0,     // PDWNCMUX
    7, 0, 0, 1, 3, 0, 3, 0, 1, 1, 1, 1, 3, 0, 3, 0,     // PDWNCMUX, pdmx[19] is not define in register map, but we can't set the corrsponding value (in this array) as 0, because we map gpio 208/212 (only one function)to bit19 (check HDMI_HPD0 define)
};

/** The gpio pin selection information.
 */
static const UINT8 code_atv _aau1GpioSelFunc[NORMAL_GPIO_NUM][2] =
{
                                            // Virtual  |   Physical
    { PINMUX_GPIO_00, PINMUX_FUNCTION0 },   // GPIO 0
    { PINMUX_GPIO_01, PINMUX_FUNCTION0 },   // GPIO 1
    { PINMUX_GPIO_02, PINMUX_FUNCTION0 },   // GPIO 2
    { PINMUX_GPIO_03, PINMUX_FUNCTION0 },   // GPIO 3
    { PINMUX_GPIO_04, PINMUX_FUNCTION0 },   // GPIO 4
    { PINMUX_GPIO_05, PINMUX_FUNCTION0 },   // GPIO 5
    { PINMUX_GPIO_06, PINMUX_FUNCTION0 },   // GPIO 6
    { PINMUX_GPIO_07, PINMUX_FUNCTION0 },   // GPIO 7
    { PINMUX_GPIO_08, PINMUX_FUNCTION0 },   // GPIO 8
    { TP_VPLL				, PINMUX_FUNCTION4 },   // GPIO 9
    { ASPDIF				, PINMUX_FUNCTION0 },   // GPIO 10      |   GPIO 80
    { AOMCLK				, PINMUX_FUNCTION6 },   // GPIO 11      |   GPIO 81
    { AOLRCK				, PINMUX_FUNCTION6 },   // GPIO 12      |   GPIO 82
    { AOBCK					, PINMUX_FUNCTION6 },   // GPIO 13      |   GPIO 83
    { AOSDATA0			, PINMUX_FUNCTION6},   // GPIO 14      |   GPIO 84
    { AOSDATA1			, PINMUX_FUNCTION0 },   // GPIO 15      |   GPIO 85
    { AOSDATA2			, PINMUX_FUNCTION0 },   // GPIO 16      |   GPIO 86
    { AOSDATA3			, PINMUX_FUNCTION0 },   // GPIO 17      |   GPIO 87
    { TUNER_CLK			, PINMUX_FUNCTION0 },   // GPIO 18      |   GPIO 88
    { TUNER_DATA		, PINMUX_FUNCTION0 },   // GPIO 19      |   GPIO 89
    { IF_AGC 				, PINMUX_FUNCTION0 },   // GPIO 20      |   GPIO 90
    { PTSI_CLK      , PINMUX_FUNCTION1 },   // GPIO 21
    { PTSI_VALID    , PINMUX_FUNCTION1 },   // GPIO 22
    { PTSI_SYNC     , PINMUX_FUNCTION1 },   // GPIO 23
    { PTSI_D0       , PINMUX_FUNCTION1 },   // GPIO 24
    { PTSI_D1       , PINMUX_FUNCTION3 },   // GPIO 25
    { PTSI_D2       , PINMUX_FUNCTION3 },   // GPIO 26
    { PTSI_D3      	, PINMUX_FUNCTION3 },   // GPIO 27.
    { PTSI_D4       , PINMUX_FUNCTION3 },   // GPIO 28
    { PTSI_D5      	, PINMUX_FUNCTION3 },   // GPIO 29
    { PTSI_D6       , PINMUX_FUNCTION3 },   // GPIO 30
    { PTSI_D7       , PINMUX_FUNCTION3 },   // GPIO 31
    { JTDO       		, PINMUX_FUNCTION1 },   // GPIO 32
    { JTCK       		, PINMUX_FUNCTION1 },   // GPIO 33
    { JTRST        	, PINMUX_FUNCTION1 },   // GPIO 34
    { POCE1        	, PINMUX_FUNCTION2 },   // GPIO 35
    { PARB        	, PINMUX_FUNCTION2 },   // GPIO 36
    { POWE    	 		, PINMUX_FUNCTION2 },   // GPIO 37
    { PAALE      		, PINMUX_FUNCTION2 },   // GPIO 38
    { PACLE       	, PINMUX_FUNCTION2 },   // GPIO 39
    { PDD2    			, PINMUX_FUNCTION2 },   // GPIO 40
    { PDD3    			, PINMUX_FUNCTION2 },   // GPIO 41
    { PDD4    			, PINMUX_FUNCTION2 },   // GPIO 42
    { PDD5    			, PINMUX_FUNCTION2 },   // GPIO 43
    { PDD6    			, PINMUX_FUNCTION2 },   // GPIO 44
    { PDD7   				, PINMUX_FUNCTION2 },   // GPIO 45
    { USB_PWR_EN0 	, PINMUX_FUNCTION3 },   // GPIO 46
    { USB_PWR_ERR0  , PINMUX_FUNCTION3 },   // GPIO 47
    { U1RX        	, PINMUX_FUNCTION0 },   // GPIO 48
    { U1TX 					, PINMUX_FUNCTION0 },   // GPIO 49
    { U2RX 					, PINMUX_FUNCTION0 },   // GPIO 50
    { U2TX 					, PINMUX_FUNCTION0 },   // GPIO 51
    { OSDA0 				, PINMUX_FUNCTION0 },   // GPIO 52
    { OSCL0 				, PINMUX_FUNCTION0 },   // GPIO 53
    { OSDA1 				, PINMUX_FUNCTION0 },   // GPIO 54
    { OSCL1 				, PINMUX_FUNCTION0 },   // GPIO 55
    { OPWM0       	, PINMUX_FUNCTION0 },   // GPIO 56
    { OPWM1      		, PINMUX_FUNCTION0 },   // GPIO 57
    { OPWM2      		, PINMUX_FUNCTION2 },   // GPIO 58
    { PINMUX_NO_PIN , PINMUX_FUNCTION0 },   // GPIO 59
    { PINMUX_NO_PIN , PINMUX_FUNCTION0 },   // GPIO 60
    { AIN0_L_AADC 	, PINMUX_FUNCTION1 },   // GPIO 61
    { AIN0_R_AADC 	, PINMUX_FUNCTION1 },   // GPIO 62
    { AIN1_L_AADC 	, PINMUX_FUNCTION1 },   // GPIO 63
    { AIN1_R_AADC 	, PINMUX_FUNCTION1 },   // GPIO 64
    { AIN2_L_AADC 	, PINMUX_FUNCTION1 },   // GPIO 65
    { AIN2_R_AADC 	, PINMUX_FUNCTION1 },   // GPIO 66
    { AIN3_L_AADC   , PINMUX_FUNCTION3 },   // GPIO 67
    { AIN3_R_AADC   , PINMUX_FUNCTION3 },   // GPIO 68
    { AIN4_L_AADC   , PINMUX_FUNCTION3 },   // GPIO 69
    { AIN4_R_AADC   , PINMUX_FUNCTION3 },   // GPIO 70
    { AIN5_L_AADC 	, PINMUX_FUNCTION3 },   // GPIO 71
    { AIN5_R_AADC 	, PINMUX_FUNCTION3 },   // GPIO 72
    { AIN5_L_AADC 	, PINMUX_FUNCTION3 },   // GPIO 73
    { AIN5_R_AADC 	, PINMUX_FUNCTION3 },   // GPIO 74
    { AL0_ADAC 			, PINMUX_FUNCTION3 },   // GPIO 75
    { AR0_ADAC 			, PINMUX_FUNCTION3 },   // GPIO 76
    { AL1_ADAC 			, PINMUX_FUNCTION6 },   // GPIO 77
    { AR1_ADAC 			, PINMUX_FUNCTION6 },   // GPIO 78
    { LVDS_PWR_ON 	, PINMUX_FUNCTION0 },   // GPIO 79
    { LVDS_CTRL1    , PINMUX_FUNCTION0 },   // GPIO 80      |    GPIO 10
    { LVDS_CTRL2    , PINMUX_FUNCTION0 },   // GPIO 81      |    GPIO 11
    { O0P        		, PINMUX_FUNCTION0 },   // GPIO 82      |    GPIO 12
    { O0N        		, PINMUX_FUNCTION0 },   // GPIO 83      |    GPIO 13
    { O1P        		, PINMUX_FUNCTION0 },   // GPIO 84      |    GPIO 14
    { O1N       		, PINMUX_FUNCTION0 },   // GPIO 85      |    GPIO 15
    { O2P       		, PINMUX_FUNCTION0 },   // GPIO 86      |    GPIO 16
    { O2N        		, PINMUX_FUNCTION0 },   // GPIO 87      |    GPIO 17
    { OCKP        	, PINMUX_FUNCTION0 },   // GPIO 88      |    GPIO 18
    { PCKN        	, PINMUX_FUNCTION0 },   // GPIO 89      |    GPIO 19
    { O3P        		, PINMUX_FUNCTION0 },   // GPIO 90      |    GPIO 20
    { O3N 					, PINMUX_FUNCTION0 },   // GPIO 91
    { O4P 					, PINMUX_FUNCTION0 },   // GPIO 92
    { O4N 					, PINMUX_FUNCTION0 },   // GPIO 93
    { E0P 					, PINMUX_FUNCTION0 },   // GPIO 94
    { E0N    				, PINMUX_FUNCTION0 },   // GPIO 95
    { E1P    				, PINMUX_FUNCTION0 },   // GPIO 96
    { E1N      			, PINMUX_FUNCTION0 },   // GPIO 97
    { E2P      			, PINMUX_FUNCTION0 },   // GPIO 98
    { E2N      			, PINMUX_FUNCTION0 },   // GPIO 99
    { ECKP      		, PINMUX_FUNCTION0 },   // GPIO 100
    { ECKN      		, PINMUX_FUNCTION0 },   // GPIO 101
    { E3P      			, PINMUX_FUNCTION0 },   // GPIO 102
    { E3N          	, PINMUX_FUNCTION0 },   // GPIO 103
    { E4P          	, PINMUX_FUNCTION0 },   // GPIO 104
    { E4N          	, PINMUX_FUNCTION0 },   // GPIO 105
};

static const UINT8  code_atv _aau1OpctrlSelFunc[TOTAL_OPCTRL_NUM][2] =
{
    //PWDNC PINMUX
    { PINMUX_OPCTRL_0   , PINMUX_FUNCTION0 },   // GPIO 200
    { PINMUX_OPCTRL_1   , PINMUX_FUNCTION0 },   // GPIO 201
    { PINMUX_OPCTRL_2   , PINMUX_FUNCTION1 },   // GPIO 202
    { PINMUX_OPCTRL_3   , PINMUX_FUNCTION0 },   // GPIO 203
    { HDMI_HPD2   			, PINMUX_FUNCTION3 },   // GPIO 204
    { PWR5V_2   				, PINMUX_FUNCTION3 },   // GPIO 205   
    { HDMI_SDA2         , PINMUX_FUNCTION3 },   // GPIO 206 PINMUX_OPCTRL_6
    { HDMI_SCL2         , PINMUX_FUNCTION3 },   // GPIO 207 PINMUX_OPCTRL_7
    { HDMI_HPD1         , PINMUX_FUNCTION1 },   // GPIO 208 PINMUX_OPCTRL_8
    { PWR5V_1         	, PINMUX_FUNCTION1 },   // GPIO 209 PINMUX_OPCTRL_9
    { HDMI_SDA1         , PINMUX_FUNCTION1 },   // GPIO 210 PINMUX_OPCTRL_10
    { HDMI_SCL1         , PINMUX_FUNCTION1 },   // GPIO 211 PINMUX_OPCTRL_11
    { HDMI_HPD0         , PINMUX_FUNCTION0 },   // GPIO 212 PINMUX_OPCTRL_12
    { PWR5V_0          	, PINMUX_FUNCTION1 },   // GPIO 213 PINMUX_OPCTRL_13
    { HDMI_SDA0         , PINMUX_FUNCTION1 },   // GPIO 214 PINMUX_OPCTRL_14
    { OIRI          		, PINMUX_FUNCTION0 },   // GPIO 215 PINMUX_OPCTRL_15
    { HDMI_CEC          , PINMUX_FUNCTION3 },   // GPIO 216 PINMUX_OPCTRL_16
    { U0RX          		, PINMUX_FUNCTION4 },   // GPIO 217 PINMUX_OPCTRL_17    
    { U0TX          		, PINMUX_FUNCTION4 },   // GPIO 218 PINMUX_OPCTRL_18
    { HDMI_SCL0         , PINMUX_FUNCTION1 },   // GPIO 219 PINMUX_OPCTRL_19        
};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** BSP_PinSet() to set pinmux data
 *  @param i4PinSel is the bit group of pinmuxers
 *  @param i4Func is the function we want to select
 *  @retval 0 is successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 BSP_PinSet(INT32 i4PinSel, INT32 i4Func)
{
    UINT32 u4Mask, u4Idx, u4Bit, u4Crit;

    if ((i4Func < 0) ||
        (i4Func > 9) ||
        (i4PinSel < 0) ||
        (i4PinSel >= TOTAL_PINMUX_BITS))
    {
        return -1;
    }

    if (_au1PinmuxFunctionMasks[i4PinSel] == 0)
    {
        return -1;
    }
    
    if (i4PinSel >= NORMAL_PINMUX_BITS)         // PDMX
    {
        u4Bit = (i4PinSel & 0x1f);  // i4PinSel%32 is the pinmux register bit.
        u4Mask = ~((UINT32)_au1PinmuxFunctionMasks[i4PinSel] << u4Bit);
#ifndef CC_UBOOT
        u4Crit = HalCriticalStart();
#endif
        vIO32Write4B(PDWNC_PINMUX, (u4IO32Read4B(PDWNC_PINMUX) & u4Mask) | ((UINT32)i4Func << u4Bit));//PDWNC_WRITE32(REG_RW_PINMUX, (PDWNC_READ32(REG_RW_PINMUX) & u4Mask) | ((UINT32)i4Func << u4Bit));
#ifndef CC_UBOOT
        HalCriticalEnd(u4Crit);
#endif
        return 0;
    }

    u4Idx = (((UINT32)i4PinSel) >> 5);    // i4PinSel/32 is the pinmux register index.
    u4Bit = (i4PinSel & 0x1f);  // i4PinSel%32 is the pinmux register bit.

    u4Mask = ~((UINT32)_au1PinmuxFunctionMasks[i4PinSel] << u4Bit);
#ifndef CC_UBOOT
    u4Crit = HalCriticalStart();
#endif
    PINMUX_WRITE(u4Idx, (PINMUX_REG(u4Idx) & u4Mask)|((UINT32)i4Func << u4Bit));
#ifndef CC_UBOOT
    HalCriticalEnd(u4Crit);
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** BSP_PinGet() to get pinmux data
 *  @param i4PinSel is the bit group of pinmuxers
 *  @retval the function we select now.
 */
//-----------------------------------------------------------------------------
INT32 BSP_PinGet(INT32 i4PinSel)
{
    UINT32 u4Idx, u4Bit;

    if ((i4PinSel < 0) ||
        (i4PinSel >= TOTAL_PINMUX_BITS))
    {
        return -1;
    }

    if (i4PinSel >= NORMAL_PINMUX_BITS)         // PDMX
    {
        u4Bit = (i4PinSel & 0x1f);  // i4PinSel%32 is the pinmux register bit.
        return ((u4IO32Read4B(PDWNC_PINMUX) >> (u4Bit)) & _au1PinmuxFunctionMasks[i4PinSel]);
    }

    u4Idx = (((UINT32)i4PinSel) >> 5);    // i4PinSel/32 is the pinmux register index.
    u4Bit = (i4PinSel & 0x1f);  // i4PinSel%32 is the pinmux register bit.
    return ((PINMUX_REG(u4Idx) >> (u4Bit)) & _au1PinmuxFunctionMasks[i4PinSel]);
}

//-----------------------------------------------------------------------------
/** BSP_PinGpioSel() to set the pin of the gpio number at gpio function.
 *  @param i4Gpio the gpio number we want to enable to gpio function.
 *  @retval 0 is successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 BSP_PinGpioSel(INT32 i4Gpio)
{
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_GPIO_NUM))
    {
        return -1;
    }

    // Skip JTAG pinmux
    if ((i4Gpio >= 81) && (i4Gpio <= 85))
    {
        return 0;
    }

    if (i4Gpio >= OPCTRL0)
    {
        return BSP_PinSet(_aau1OpctrlSelFunc[i4Gpio - OPCTRL0][0], _aau1OpctrlSelFunc[i4Gpio - OPCTRL0][1]);
    }
    else
    {
        return BSP_PinSet(_aau1GpioSelFunc[i4Gpio][0], _aau1GpioSelFunc[i4Gpio][1]);
    }
}

//-----------------------------------------------------------------------------
/** BSP_PinGpioGet() to Get the original function of selected gpio pin.
 *  @param i4Gpio the gpio number we want to enable to gpio function.
 *  @param pi4PinSel  return of logical gpio mapping physical pin.
 *  @retval the function we select now.
 */
//-----------------------------------------------------------------------------
INT32 BSP_PinGpioGet(INT32 i4Gpio, INT32 *pi4PinSel)
{
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_GPIO_NUM))
    {
        return -1;
    }

    if (pi4PinSel == NULL)
    {
        return -1;
    }

    if (i4Gpio >= OPCTRL0)
    {
        *pi4PinSel =  _aau1OpctrlSelFunc[i4Gpio - OPCTRL0][0];
        return BSP_PinGet(_aau1OpctrlSelFunc[i4Gpio - OPCTRL0][0]);    
    }
    else 
    {
        *pi4PinSel =  _aau1GpioSelFunc[i4Gpio][0];
        return BSP_PinGet(_aau1GpioSelFunc[i4Gpio][0]);
    }

}


