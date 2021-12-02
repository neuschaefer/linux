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

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/** number of bits used in pinmux group (sel)
 */
static const UINT8 _au1PinmuxFunctionMasks[TOTAL_PINMUX_BITS] =
{
//  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
    3, 0, 3, 0, 3, 0, 3, 0, 7, 0, 0, 1, 3, 0, 0, 1,     // PINMUX0  11/15?
    3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0,     // PINMUX0

    3, 0, 7, 0, 0, 7, 0, 0, 7, 0, 0, 1, 7, 0, 0, 1,     // PINMUX1  11/15?
    7, 0, 0, 1, 7, 0, 0, 1, 1, 1, 1, 1, 3, 0, 1, 1,     // PINMUX1  23/24/25/26/27/30/31?

    3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 7, 0, 0, 1,     // PINMUX2  15?
    7, 0, 0, 1, 7, 0, 0, 1, 7, 0, 0, 1, 7, 0, 0, 1,     // PINMUX2  19/23/27/31?

    7, 0, 0, 1, 7, 0, 0, 1, 7, 0, 0, 1, 1, 1, 1, 1,     // PINMUX3  3/7/11
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,     // PINMUX3  
};

/** The gpio pin selection information.
 */
static const UINT8 _aau1GpioSelFunc[TOTAL_GPIO_NUM][2] =
{
    { GPIO_00           , PINMUX_FUNCTION0 },   // 0
    { GPIO_01           , PINMUX_FUNCTION0 },   // 1
    { GPIO_02           , PINMUX_FUNCTION0 },   // 2
    { GPIO_03           , PINMUX_FUNCTION0 },   // 3
    { GPIO_04           , PINMUX_FUNCTION0 },   // 4
    { GPIO_05           , PINMUX_FUNCTION0 },   // 5
    { GPIO_06           , PINMUX_FUNCTION0 },   // 6
    { GPIO_07           , PINMUX_FUNCTION0 },   // 7
    { GPIO_08           , PINMUX_FUNCTION0 },   // 8
    { GPIO_09           , PINMUX_FUNCTION0 },   // 9
    { GPIO_10           , PINMUX_FUNCTION0 },   // 10
    { GPIO_11           , PINMUX_FUNCTION0 },   // 11
    { GPIO_12           , PINMUX_FUNCTION0 },   // 12
    { GPIO_13           , PINMUX_FUNCTION0 },   // 13
    { JRTCK_O           , PINMUX_FUNCTION1 },   // 14
    { JTMS_I            , PINMUX_FUNCTION1 },   // 15
    { JTDI_I            , PINMUX_FUNCTION1 },   // 16
    { JTDO_O            , PINMUX_FUNCTION1 },   // 17
    { JTCK_I            , PINMUX_FUNCTION1 },   // 18
    { JTRST_I           , PINMUX_FUNCTION1 },   // 19
    { POCE1_O           , PINMUX_FUNCTION1 },   // 20
    { PARB_I            , PINMUX_FUNCTION1 },   // 21+
    { POWE_O            , PINMUX_FUNCTION1 },   // 22+
    { PAALE_O           , PINMUX_FUNCTION1 },   // 23+
    { PACLE_O           , PINMUX_FUNCTION1 },   // 24+
    { PDD2_IO           , PINMUX_FUNCTION1 },   // 25+
    { PDD3_IO           , PINMUX_FUNCTION1 },   // 26+
    { PDD4_IO           , PINMUX_FUNCTION1 },   // 27+
    { PDD5_IO           , PINMUX_FUNCTION1 },   // 28+
    { PDD6_IO           , PINMUX_FUNCTION1 },   // 29+
    { PDD7_IO           , PINMUX_FUNCTION1 },   // 30+
    { U2RX_I            , PINMUX_FUNCTION0 },   // 31
    { U2TX_O            , PINMUX_FUNCTION0 },   // 32
    { OSDA0_IO          , PINMUX_FUNCTION1 },   // 33+
    { OSCL0_O           , PINMUX_FUNCTION1 },   // 34+
    { OSDA1_IO          , PINMUX_FUNCTION0 },   // 35
    { OSCL1_I           , PINMUX_FUNCTION0 },   // 36
    { OSDA2_IO          , PINMUX_FUNCTION0 },   // 37
    { OSCL2_O           , PINMUX_FUNCTION0 },   // 38
    { OWRP2_O           , PINMUX_FUNCTION0 },   // 39
    { OPWM0_O           , PINMUX_FUNCTION0 },   // 40
    { OPWM1_O           , PINMUX_FUNCTION0 },   // 41
    { OPWM2_O           , PINMUX_FUNCTION0 },   // 42
    { OIRO_O            , PINMUX_FUNCTION0 },   // 43
    { OSDA3_IO          , PINMUX_FUNCTION0 },   // 44+
    { OSCL3_O           , PINMUX_FUNCTION0 },   // 45+
    { ALIN_I            , PINMUX_FUNCTION0 },   // 46
    { AOMCLK_IO         , PINMUX_FUNCTION0 },   // 47
    { AOLRCK_O          , PINMUX_FUNCTION0 },   // 48
    { AOBCK_O           , PINMUX_FUNCTION0 },   // 49
    { AOSDATA0_O        , PINMUX_FUNCTION0 },   // 50
    { AOSDATA1_O        , PINMUX_FUNCTION0 },   // 51
    { AOSDATA2_O        , PINMUX_FUNCTION0 },   // 52
    { AOSDATA3_O        , PINMUX_FUNCTION0 },   // 53
    { AOSDATA4_O        , PINMUX_FUNCTION0 },   // 54
    { ASPDIF            , PINMUX_FUNCTION0 },   // 55
    { ATN_TX            , PINMUX_FUNCTION1 },   // 56+
    { ATN_RX            , PINMUX_FUNCTION1 },   // 57+
    { ATN_DET           , PINMUX_FUNCTION1 },   // 58+
    { TUNER_CLK         , PINMUX_FUNCTION1 },   // 59+
    { TUNER_DATA        , PINMUX_FUNCTION1 },   // 60+
    { IF_AGC            , PINMUX_FUNCTION1 },   // 61+
    { RG_AGC            , PINMUX_FUNCTION1 },   // 62+
    { AL1               , PINMUX_FUNCTION1 },   // 63o
    { AR1               , PINMUX_FUNCTION1 },   // 64o
    { AL2               , PINMUX_FUNCTION1 },   // 65o
    { AR2               , PINMUX_FUNCTION1 },   // 66o
    { VCXO              , PINMUX_FUNCTION1 },   // 67+
    { AIN0_L            , PINMUX_FUNCTION1 },   // 68i+
    { AIN0_R            , PINMUX_FUNCTION1 },   // 69i+
    { AIN1_L            , PINMUX_FUNCTION1 },   // 70i+
    { AIN1_R            , PINMUX_FUNCTION1 },   // 71i+
    { AIN2_L            , PINMUX_FUNCTION1 },   // 72i+
    { AIN2_R            , PINMUX_FUNCTION1 },   // 73i+
    { AIN3_L            , PINMUX_FUNCTION1 },   // 74i+
    { AIN3_R            , PINMUX_FUNCTION1 },   // 75i+
    { AIN4_L            , PINMUX_FUNCTION1 },   // 76i+
    { AIN4_R            , PINMUX_FUNCTION1 },   // 77i+
    { AIN5_L            , PINMUX_FUNCTION1 },   // 78i+
    { AIN5_R            , PINMUX_FUNCTION1 },   // 79i+
    { AIN6_L            , PINMUX_FUNCTION1 },   // 80i+
    { AIN6_R            , PINMUX_FUNCTION1 },   // 81i+
    { AIN7_L            , PINMUX_FUNCTION1 },   // 82i+
    { AIN7_R            , PINMUX_FUNCTION1 },   // 83i+
    { A0P               , PINMUX_FUNCTION4 },   // 84+
    { A0N               , PINMUX_FUNCTION4 },   // 85+
    { A1P               , PINMUX_FUNCTION4 },   // 86+
    { A1N               , PINMUX_FUNCTION4 },   // 87+
    { A2P               , PINMUX_FUNCTION4 },   // 88+
    { A2N               , PINMUX_FUNCTION4 },   // 89+
    { CK1P              , PINMUX_FUNCTION4 },   // 90+
    { CK1N              , PINMUX_FUNCTION4 },   // 91+
    { A3P               , PINMUX_FUNCTION4 },   // 92+
    { A3N               , PINMUX_FUNCTION4 },   // 93+
    { A4P               , PINMUX_FUNCTION4 },   // 94+
    { A4N               , PINMUX_FUNCTION4 },   // 95+
    { A5P               , PINMUX_FUNCTION4 },   // 96+
    { A5N               , PINMUX_FUNCTION4 },   // 97+
    { A6P               , PINMUX_FUNCTION4 },   // 98+
    { A6N               , PINMUX_FUNCTION4 },   // 99+
    { CK2P              , PINMUX_FUNCTION4 },   // 100+
    { CK2N              , PINMUX_FUNCTION4 },   // 101+
    { A7P               , PINMUX_FUNCTION4 },   // 102+
    { A7N               , PINMUX_FUNCTION4 },   // 103+
    { TP2               , PINMUX_FUNCTION4 },   // 104+
    { TN2               , PINMUX_FUNCTION4 },   // 105+
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
    UINT32 u4Mask, u4Idx, u4Bit;

    if ((i4Func < 0) ||
        (i4Func > 6) ||
        (i4PinSel < 0) ||
        (i4PinSel >= TOTAL_PINMUX_BITS))
    {
        return -1;
    }

    if (_au1PinmuxFunctionMasks[i4PinSel] == 0)
    {
        return -1;
    }

    u4Idx = (((UINT32)i4PinSel) >> 5);    // i4PinSel/32 is the pinmux register index.
    u4Bit = (i4PinSel & 0x1f);  // i4PinSel%32 is the pinmux register bit.

    u4Mask = ~((UINT32)_au1PinmuxFunctionMasks[i4PinSel] << u4Bit);
    PINMUX_WRITE(u4Idx, (PINMUX_REG(u4Idx) & u4Mask)|((UINT32)i4Func << u4Bit));

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
    // PDWNC PWR GPIO just pass. no pinmux selection, so just return 0.
    if ((i4Gpio >= PWR_GPIO0) && (i4Gpio <= PWR_GPIO8))
    {
        return 0;
    }

    if ((i4Gpio >= SERVO_GPIO0) && (i4Gpio <= SERVO_GPIO4))
    {
        return 0;
    }

    if ((i4Gpio >= MT8292_GPIO0) && (i4Gpio <= MT8292_GPIO9))
    {
        return 0;
    }

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_GPIO_NUM))
    {
        return -1;
    }

    if ((i4Gpio >= 14) && (i4Gpio <= 19))
    {
        return 0;
    }

    return BSP_PinSet(_aau1GpioSelFunc[i4Gpio][0], _aau1GpioSelFunc[i4Gpio][1]);
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
    // PDWNC PWR GPIO just pass. no pinmux selection, so just return 0.
    if ((i4Gpio >= PWR_GPIO0) && (i4Gpio <= PWR_GPIO8))
    {
        return 0;
    }

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_GPIO_NUM))
    {
        return -1;
    }

    if (pi4PinSel == NULL)
    {
        return -1;
    }

    *pi4PinSel =  _aau1GpioSelFunc[i4Gpio][0];
    
    return BSP_PinGet(_aau1GpioSelFunc[i4Gpio][0]);
}


