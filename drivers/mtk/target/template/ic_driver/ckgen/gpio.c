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
#ifndef CC_5391_LOADER
#include "x_os.h"
#endif /* CC_5391_LOADER */
#include "sif_if.h"
#include "drvcust_if.h"

#define DEFINE_IS_LOG	IR_IsLog
#include "x_debug.h"

#include "x_os.h"
#include "drv_common.h"

#ifndef CC_5391_LOADER
#include "nand_if.h"
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
#ifndef CC_5391_LOADER   
#define MT8292_RET_ON_FAIL(X) \
    if((X) != 1) \
    { \
        UNUSED(x_sema_unlock(_hMT8292GpioMutex));\
        return -1; \
    }
#else
#define MT8292_RET_ON_FAIL(X) \
    if((X) != 1) \
    { \
        return -1; \
    }
#endif

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
#define DEFINE_MT8292_INTERRUPT
static INT32 _MT8292_Gpio_Init(void);
static INT32 _MT8292_Gpio_Enable(INT32 i4GpioNum, const INT32 * pi4Polarity/*0:input, 1:output, null:query*/);
static INT32 _MT8292_Gpio_Input(INT32 i4GpioNum);
static INT32 _MT8292_Gpio_Output(INT32 i4GpioNum, const INT32 *pfgSet);
static INT32 _MT8292_Gpio_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
static INT32 _MT8292_Gpio_Intrq(INT32 i4Gpio, const INT32 *pfgSet);
static  void  _MT8292_IntrThreadMain( void * pvArg);

extern INT32  _MT8295_GPIO_Init(void);
extern INT32 _MT8295_GPIO_Enable(INT32 i4GpioNum, const INT32 * pi4Polarity/*0:input, 1:output, null:query*/);
extern INT32 _MT8295_GPIO_Input(INT32 i4GpioNum);
extern INT32 _MT8295_GPIO_Output(INT32 i4GpioNum, const INT32 * pi4DriveHighLow/*0:drive0, 1:drive 1, null:query*/) ;
extern INT32 _MT8295_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
extern INT32 _MT8295_GPIO_Intrq(INT32 i4Gpio, const INT32 *pfgSet);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT32 _u4GPIOInit = 0;

#ifndef CC_5391_LOADER
static HANDLE_T _hMT8292GpioMutex;
#endif /* CC_5391_LOADER */

//-----------------------------------------------------------------------------
/******           GPIO register read/write functions                    *******

#define TOTAL_GPIO_NUM  106
#define TOTAL_GPIO_IDX  ((TOTAL_GPIO_NUM + 31) >> 5)    ==> 5 (4byte on Index)
#define GPIO_INDEX_MASK ((1 << 5) - 1)     ==> 31

******           GPIO register read/write functions                    *******/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** GPIO_Enable() The GPIO input/output mode setting functions. It will check 
 *  the i4GpioNum and set to related register bit as 0 or 1.  In this 
 *  function, 0 is input mode and 1 is output mode.
 *  @param i4GpioNum the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *  the current mode of gpio number (0 or 1).  If it's not NULL, it must
 *  reference to a integer.  If the integer is 0, this function will set the
 *  mode of the gpio number as input mode, otherwise set as output mode.
 *  @retval If pfgSet is NULL, it return 0 or 1 (0 is input mode, 1 is output
 *          mode.)  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 GPIO_Enable(INT32 i4GpioNum, const INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Idx;
#if 0
    // PDWNC PWR GPIO just pass. no pinmux selection, so just return 0.
    if ((i4GpioNum >= PWR_GPIO0) && (i4GpioNum <= PWR_GPIO8))
    {
        return PDWNC_GpioEnable((i4GpioNum - PWR_GPIO0), pfgSet);
    }
    // Servo ADC GPIO
    else if((i4GpioNum >= SERVO_GPIO0) && (i4GpioNum <= SERVO_GPIO4))
    {
        return PDWNC_ServoGpioEnable((i4GpioNum - SERVO_GPIO0), pfgSet);
    }
    // MT8292 GPIO
    else if((i4GpioNum >= MT8292_GPIO0) && (i4GpioNum <= MT8292_GPIO9))
    {
        return _MT8292_Gpio_Enable(i4GpioNum, pfgSet);
    }
#ifdef CC_MT8295
#ifndef CC_5391_LOADER
    else if( (i4GpioNum>=MT8295_GPIO0) && (i4GpioNum<=MT8295_GPIO14) )
    {
    	return _MT8295_GPIO_Enable( i4GpioNum-MT8295_GPIO0 , pfgSet );
    }
#endif /* CC_5391_LOADER */
#endif /* CC_MT8295 */
#endif /* 0 */

    if ((i4GpioNum < 0) || (i4GpioNum >= TOTAL_GPIO_NUM))
    {
        return -1;
    }

    i4Idx = ((UINT32)i4GpioNum >> 5);
    u4Val = GPIO_EN_REG(i4Idx);
    if (pfgSet == NULL)
    {
        return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1 : 0);
    }

    u4Val = (*pfgSet) ?
            (u4Val | (1U << (i4GpioNum & GPIO_INDEX_MASK))) :
            (u4Val & ~(1U << (i4GpioNum & GPIO_INDEX_MASK)));

    GPIO_EN_WRITE(i4Idx, u4Val);

    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/** GPIO_Output() The GPIO output value setting functions. It will check the 
 *  i4GpioNum and set to related register bit as 0 or 1.
 *  @param i4GpioNum the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *          the bit of gpio number (0 or 1).  If it's not NULL, it must 
 *          reference to a integer.  If the integer is 0, this function 
 *          will set the bit of the gpio number as 0, otherwise set as 1.
 *  @retval If pfgSet is NULL, it return 0 or 1 (the bit value of the gpio
 *          number of output mode.  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 GPIO_Output(INT32 i4GpioNum, const INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Idx;

#if 0
    // PDWNC PWR GPIO just pass. no pinmux selection, so just return 0.
    if ((i4GpioNum >= PWR_GPIO0) && (i4GpioNum <= PWR_GPIO8))
    {
        return PDWNC_GpioOutput((i4GpioNum - PWR_GPIO0), pfgSet);
    }
    // Servo GPIO 
    else if ((i4GpioNum >= SERVO_GPIO0) && (i4GpioNum <= SERVO_GPIO4))
    {
        return PDWNC_ServoGpioOutput((i4GpioNum - SERVO_GPIO0), pfgSet);
    }    
    // MT8292 GPIO
    else if((i4GpioNum >= MT8292_GPIO0) && (i4GpioNum <= MT8292_GPIO9))
    {
        return _MT8292_Gpio_Output(i4GpioNum, pfgSet);
    }
#ifdef CC_MT8295
#ifndef CC_5391_LOADER
    else if( (i4GpioNum>=MT8295_GPIO0) && (i4GpioNum<=MT8295_GPIO14) )
    {
        i4Idx = 1;
    	UNUSED(_MT8295_GPIO_Enable( i4GpioNum-MT8295_GPIO0 , &i4Idx));
    	return _MT8295_GPIO_Output( i4GpioNum-MT8295_GPIO0 , pfgSet );
    }
#endif /* CC_5391_LOADER */
#endif /* CC_MT8295 */
#endif /* 0 */

    if ((i4GpioNum < 0) || (i4GpioNum >= TOTAL_GPIO_NUM))
    {
        return -1;
    }

    i4Idx = ((UINT32)i4GpioNum >> 5);
    u4Val = GPIO_OUT_REG(i4Idx);
    if (pfgSet == NULL)
    {
        return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1 : 0);
    }

    u4Val = (*pfgSet) ?
            (u4Val | (1U << (i4GpioNum & GPIO_INDEX_MASK))) :
            (u4Val & ~(1U << (i4GpioNum & GPIO_INDEX_MASK)));

    GPIO_OUT_WRITE(i4Idx, u4Val);

    // Set the output mode.
    i4Idx = 1;
    VERIFY(GPIO_Enable(i4GpioNum, &i4Idx)==i4Idx);

    VERIFY(BSP_PinGpioSel(i4GpioNum)==0);

    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/** GPIO_Input()  The GPIO input reading functions. It will check the 
 *  i4GpioNum and read related register bit to return.
 *  @param i4GpioNum the gpio number to read.
 *  @retval 0 or 1.  (GPIO input value.)
 */
//-----------------------------------------------------------------------------
INT32 GPIO_Input(INT32 i4GpioNum)
{
    UINT32 u4Val;
    INT32 i4Idx;

#if 0
    // PDWNC PWR GPIO just pass. no pinmux selection, so just return 0.
    if ((i4GpioNum >= PWR_GPIO0) && (i4GpioNum <= PWR_GPIO8))
    {
        return PDWNC_GpioInput(i4GpioNum - PWR_GPIO0);
    }
    // Servo GPIO
    else if ((i4GpioNum >= SERVO_GPIO0) && (i4GpioNum <= SERVO_GPIO4))
    {
        return PDWNC_ServoGpioInput(i4GpioNum - SERVO_GPIO0);
    }
    // MT8292 GPIO
    else if((i4GpioNum >= MT8292_GPIO0) && (i4GpioNum <= MT8292_GPIO9))
    {
        return _MT8292_Gpio_Input(i4GpioNum);
    }    
#ifdef CC_MT8295
#ifndef CC_5391_LOADER
    else if( (i4GpioNum>=MT8295_GPIO0) && (i4GpioNum<=MT8295_GPIO14) )
    {
    	return _MT8295_GPIO_Input( i4GpioNum-MT8295_GPIO0 );
    }
#endif /* CC_5391_LOADER */
#endif /* CC_MT8295 */
#endif /* 0 */
    
    if ((i4GpioNum < 0) || (i4GpioNum >= TOTAL_GPIO_NUM))
    {
        return -1;
    }

    // Set the input mode.
    // i4Idx = 0;
    // GPIO_Enable(i4GpioNum, &i4Idx);
    VERIFY(BSP_PinGpioSel(i4GpioNum)==0);

    // Read input value.
    i4Idx = ((UINT32)i4GpioNum >> 5);
    u4Val = GPIO_IN_REG(i4Idx);

    return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1U : 0);
}

#ifndef CC_5391_LOADER
//-----------------------------------------------------------------------------
/** GPIO_Intrq() The GPIO interrupt enable setting functions. It will check
 *  the i4GpioNum and set to related register bit as 0 or 1.  In this 
 *  function, 0 is interrupt disable mode and 1 is interrupt enable mode.
 *  @param i4GpioNum the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *  the current setting of gpio number (0 or 1).  If it's not NULL, it must
 *  reference to a integer.  If the integer is 0, this function will set the
 *  mode of the gpio number as interrupt disable mode, otherwise set as
 *  interrupt enable mode.
 *  @retval If pfgSet is NULL, it return 0 or 1 (0 is interrupt disable mode,
 *          1 is interrupt enable mode.)  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 GPIO_Intrq(INT32 i4GpioNum, const INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Idx;

#if 0
    // PDWNC PWR GPIO just pass. no pinmux selection, so just return 0.
    if ((i4GpioNum >= PWR_GPIO0) && (i4GpioNum <= PWR_GPIO7))
    {
        return PDWNC_GpioIntrq((i4GpioNum - PWR_GPIO0), pfgSet);
    }
    // Servo GPIO.
    if ((i4GpioNum >= SERVO_GPIO0) && (i4GpioNum <= SERVO_GPIO4))
    {
        return PDWNC_ServoGpioIntrq((i4GpioNum - SERVO_GPIO0), pfgSet);
    }
#ifdef DEFINE_MT8292_INTERRUPT
    else if((i4GpioNum >= MT8292_GPIO0) && (i4GpioNum <= MT8292_GPIO9))
    {
        return _MT8292_Gpio_Intrq( i4GpioNum ,  pfgSet );
    }
#endif        
#ifdef CC_MT8295
#ifndef CC_5391_LOADER
    else if( (i4GpioNum>=MT8295_GPIO0) && (i4GpioNum<=MT8295_GPIO14) )
    {
        return _MT8295_GPIO_Intrq( i4GpioNum-MT8295_GPIO0 ,  pfgSet );
    }
#endif /* CC_5391_LOADER */
#endif /* CC_MT8295 */
#endif /* 0 */
    
    if ((i4GpioNum < 0) || (i4GpioNum >= TOTAL_GPIO_NUM))
    {
        return -1;
    }

    i4Idx = ((UINT32)i4GpioNum >> 5);
    u4Val = GPIO_RQ_REG(i4Idx);
    if (pfgSet == NULL)
    {
        return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1 : 0);
    }

    u4Val = (*pfgSet) ?
            (u4Val | (1U << (i4GpioNum & GPIO_INDEX_MASK))) :
            (u4Val & ~(1U << (i4GpioNum & GPIO_INDEX_MASK)));

    GPIO_RQ_WRITE(i4Idx, u4Val);

    return (*pfgSet);
}
#endif /* CC_5391_LOADER */

//-----------------------------------------------------------------------------
/** GPIO_SetOut() to set gpio output value.
 *  @param i4GpioNum
 *  @param i4Val 0 or 1.
 *  @retval to return current gpio out register setting.
 */
//-----------------------------------------------------------------------------
INT32 GPIO_SetOut(INT32 i4GpioNum, INT32 i4Val)
{
    INT32 fgSet, i4Ret;

    fgSet = i4Val;
    i4Ret = GPIO_Output(i4GpioNum, &fgSet);
    if (i4Ret != i4Val)
    {
        return i4Ret;
    }
    return GPIO_Output(i4GpioNum, NULL);
}

//-----------------------------------------------------------------------------
/** GPIO_Input()  The GPIO input reading functions. It will check the 
 *  i4GpioNum and read related register bit to return.
 *  @param i4GpioNum the gpio number to read.
 *  @retval 0 or 1.  (GPIO input value.)
 */
//-----------------------------------------------------------------------------
INT32 GPIO_GetIn(INT32 i4GpioNum)
{
    INT32 i4Mode;
    // Set to input mode.
    i4Mode = 0;
    VERIFY(GPIO_Enable(i4GpioNum, &i4Mode)==i4Mode);

    return GPIO_Input(i4GpioNum);
}

//-----------------------------------------------------------------------------
// GPIO Interface functions.
//-----------------------------------------------------------------------------

// Set all interrupt are h/w double edge trigger, so need variables below.
#ifndef CC_5391_LOADER
static volatile UINT32 _au4GpioPrevPoll[TOTAL_GPIO_IDX];
static volatile UINT32 _au4GpioPollFall[TOTAL_GPIO_IDX];
static volatile UINT32 _au4GpioPollRise[TOTAL_GPIO_IDX];
#endif /* CC_5391_LOADER */

#if GPIO_THREAD_ENABLE
static HANDLE_T _hGPIOPollThread;
static volatile INT32 _i4GPIOExist = 1;
static volatile INT32 _i4GPIOLoop = 0;
#endif /* GPIO_THREAD_ENABLE */


#define GPIO_POLLFALL_REG(gpio) \
    (_au4GpioPollFall[gpio >> 5] & (1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_POLLRISE_REG(gpio) \
    (_au4GpioPollRise[gpio >> 5] & (1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_POLLFALL_SET(gpio) \
    (_au4GpioPollFall[gpio >> 5] |= (1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_POLLFALL_CLR(gpio) \
    (_au4GpioPollFall[gpio >> 5] &= ~(1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_POLLRISE_SET(gpio) \
    (_au4GpioPollRise[gpio >> 5] |= (1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_POLLRISE_CLR(gpio) \
    (_au4GpioPollRise[gpio >> 5] &= ~(1U << (gpio & GPIO_INDEX_MASK)))


#ifndef CC_5391_LOADER
static volatile PFN_GPIO_CALLBACK _afnGpioCallback[TOTAL_GPIO_NUM];
#endif /* CC_5391_LOADER */

// Level triggered GPIO interrupt Enable register
#define GPIO_INTR_REG(gpio)     \
    (GPIO_RQ_REG(gpio >> 5) & (1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_INTR_SET(gpio)     \
    (GPIO_RQ_REG(gpio >> 5) |= (1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_INTR_CLR(gpio)     \
    (GPIO_RQ_REG(gpio >> 5) &= ~(1U << (gpio & GPIO_INDEX_MASK)))

// Rising edge triggered interrupt Enable register
#define GPIO_INTR_RISE_REG(gpio)    \
    (GPIO_RISE_REG(gpio >> 5) & (1U << (gpio & EDGE_INDEX_MASK)))

#define GPIO_INTR_RISE_SET(gpio)    \
    (GPIO_RISE_REG(gpio >> 5) |= (1U << (gpio & EDGE_INDEX_MASK)))

#define GPIO_INTR_RISE_CLR(gpio)    \
    (GPIO_RISE_REG(gpio >> 5) &= ~(1U << (gpio & EDGE_INDEX_MASK)))

// Falling edge triggered interrupt Enable register
#define GPIO_INTR_FALL_REG(gpio)    \
    (GPIO_FALL_REG(gpio >> 5) & (1U << (gpio & EDGE_INDEX_MASK)))

#define GPIO_INTR_FALL_SET(gpio)    \
    (GPIO_FALL_REG(gpio >> 5) |= (1U << (gpio & EDGE_INDEX_MASK)))

#define GPIO_INTR_FALL_CLR(gpio)    \
    (GPIO_FALL_REG(gpio >> 5) &= ~(1U << (gpio & EDGE_INDEX_MASK)))

// Edge triggered interrupt status register
#define GPIO_INTR_EDGE_REG(gpio)    \
    (GPIO_EDGE_REG(gpio >> 5) & (1U << (gpio & EDGE_INDEX_MASK)))

#define GPIO_INTR_EDGE_SET(gpio)    \
    (GPIO_EDGE_REG(gpio >> 5) |= (1U << (gpio & EDGE_INDEX_MASK)))

#define GPIO_INTR_EDGE_CLR(gpio)    \
    (GPIO_EDGE_REG(gpio >> 5) &= ~(1U << (gpio & EDGE_INDEX_MASK)))


#ifndef CC_5391_LOADER
static void _GPIO_CheckCallback(UINT32 u4Mask, INT32 i4Idx,
                                INT32 i4Total, BOOL fgStatus)
{
    INT32 i4Bit, i4Gpio;

    for (i4Bit = 0; (UINT32)i4Bit < 32; i4Bit++)
    {
        if (u4Mask & (1U << i4Bit))
        {
            i4Gpio = (INT32)((UINT32)i4Idx * 32 ) + i4Bit;

            /* Callback */
            if ((i4Gpio < i4Total) &&
                (_afnGpioCallback[i4Gpio] != NULL))
            {
                _afnGpioCallback[i4Gpio](i4Gpio, fgStatus);
            }
        }
    }
}

static void _GPIO_BothEdgeIsr(UINT16 u2VctId)
{
    INT32 i4Idx;
    UINT32 u4Mask;

    ASSERT(u2VctId == VECTOR_EXT);
    VERIFY(BIM_ClearIrq((UINT32)u2VctId));

    for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
    {
        /* Check falling 1->0 */
        u4Mask = _au4GpioPrevPoll[i4Idx] & _au4GpioPollFall[i4Idx];

        if (u4Mask)
        {
            _GPIO_CheckCallback(u4Mask, i4Idx, TOTAL_GPIO_NUM, 0);
        }

        /* Check Rising. 0->1 */
        u4Mask = ~(_au4GpioPrevPoll[i4Idx]) & _au4GpioPollRise[i4Idx];

        if (u4Mask)
        {
            _GPIO_CheckCallback(u4Mask, i4Idx, TOTAL_GPIO_NUM, 1);
        }

        /* Assign Previous value. */
        _au4GpioPrevPoll[i4Idx] = GPIO_IN_REG(i4Idx);
    }
}

static void _GPIO_DedicatedIsr(UINT16 u2VctId)
{
    INT32 i4Idx;
    UINT32 u4MaskRise;
    UINT32 u4MaskFall;    
    UINT32 u4Status;

    ASSERT(u2VctId == VECTOR_EXT_L2);
    VERIFY(BIM_ClearIrq((UINT32)u2VctId));

    // handle edge triggered interrupt.
    for (i4Idx = 0; i4Idx < TOTAL_EDGE_IDX; i4Idx++)
    {
        // get edge triggered interrupt status.       
        u4Status = GPIO_EDGE_REG(i4Idx);
    	CKGEN_WRITE32(REG_GPIO_EDGE_ST_INTR0+(4*i4Idx), 0);
       
        // clear edge triggered interrupt status.       
        GPIO_EDGE_WRITE(i4Idx, 0);

        // Check Rising
        u4MaskRise = u4Status & GPIO_RISE_REG(i4Idx);        
        u4MaskRise &= ~(_au4GpioPrevPoll[i4Idx]);
        if (u4MaskRise)
        {
            _GPIO_CheckCallback(u4MaskRise, i4Idx, GPIO_EDGE_INTR_NUM, 1);
        }
        else
        {
            // Check Falling
            u4MaskFall = u4Status & GPIO_FALL_REG(i4Idx);
            u4MaskRise &= (_au4GpioPrevPoll[i4Idx]);
            if (u4MaskFall)
            {
                _GPIO_CheckCallback(u4MaskFall, i4Idx, GPIO_EDGE_INTR_NUM, 0);
            }
        }
        _au4GpioPrevPoll[i4Idx] = GPIO_IN_REG(i4Idx);
    }

/* 
    for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
    {
        u4Mask = ((_fgPolarityRising) ?
                    (GPIO_IN_REG(i4Idx)) : (~GPIO_IN_REG(i4Idx)));
        u4Mask &= GPIO_RQ_REG(i4Idx);

        if (u4Mask)
        {
            _GPIO_CheckCallback(u4Mask, i4Idx, TOTAL_GPIO_NUM,
                                    _fgPolarityRising);
        }
    }
*/
}
#endif /* CC_5391_LOADER */

#if GPIO_THREAD_ENABLE
static void _GPIO_PollThread(void *pvDummy)
{
    INT32 i4Idx, i4Bit, i4Gpio;
    UINT32 u4Mask;

    UNUSED(pvDummy);
    for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
    {
        _au4GpioPrevPoll[i4Idx] = GPIO_IN_REG(i4Idx);
    }

    _i4GPIOLoop = 1;
    _i4GPIOExist = 1;
    while (_i4GPIOLoop)
    {
        x_thread_delay(200);

        for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
        {
            /* Check falling 1->0 */
            u4Mask = _au4GpioPrevPoll[i4Idx] &
                        ~(GPIO_IN_REG(i4Idx)) & _au4GpioPollFall[i4Idx];

            if (u4Mask)
            {
                _GPIO_CheckCallback(u4Mask, i4Idx, TOTAL_GPIO_NUM, 0);
            }

            /* Check Rising. 0->1 */
            u4Mask = ~(_au4GpioPrevPoll[i4Idx]) &
                        GPIO_IN_REG(i4Idx) & _au4GpioPollRise[i4Idx];

            if (u4Mask)
            {
                _GPIO_CheckCallback(u4Mask, i4Idx, TOTAL_GPIO_NUM, 1);
            }

            /* Assign Previous value. */
            _au4GpioPrevPoll[i4Idx] = GPIO_IN_REG(i4Idx);
        }
    }
    _i4GPIOLoop = 1;
    _i4GPIOExist = 0;
}
#endif /* GPIO_THREAD_ENABLE */


INT32 GPIO_Init()
{
    UINT32 u4Val;
    INT32 i4Idx;
    INT32 i4Ret;    
#ifndef CC_5391_LOADER
    void (* pfnOldIsr)(UINT16);
#endif /* CC_5391_LOADER */

    if (_u4GPIOInit)
    {
        return 0;
    }
    _u4GPIOInit = 1;
        
#ifndef CC_5391_LOADER
    for (i4Idx = 0; i4Idx < TOTAL_GPIO_NUM; i4Idx++)
    {
        _afnGpioCallback[i4Idx] = NULL;
    }

    for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
    {
        GPIO_RQ_WRITE(i4Idx, 0);
#if 1 // GPIO_THREAD_ENABLE
        _au4GpioPrevPoll[i4Idx] = 0;
        _au4GpioPollFall[i4Idx] = 0;
        _au4GpioPollRise[i4Idx] = 0;
#endif /* GPIO_THREAD_ENABLE */
    }
#endif /* CC_5391_LOADER */

    // clear all edge triggered interrupt status 0.
    for (i4Idx = 0; i4Idx < TOTAL_EDGE_IDX; i4Idx++)
    {                
        GPIO_RISE_WRITE(i4Idx, 0);
        GPIO_FALL_WRITE(i4Idx, 0);
        GPIO_EDGE_WRITE(i4Idx, 0);
    }

    i4Ret = 0;
#ifndef CC_5391_LOADER
    i4Ret = x_reg_isr(VECTOR_EXT, _GPIO_BothEdgeIsr, &pfnOldIsr);
    ASSERT(i4Ret == OSR_OK);
    i4Ret = x_reg_isr(VECTOR_EXT_L2, _GPIO_DedicatedIsr, &pfnOldIsr);
    ASSERT(i4Ret == OSR_OK);
    UNUSED(i4Ret);
#endif /* CC_5391_LOADER */

    u4Val = BIM_READ32(REG_RW_MISC);
    u4Val &= ~(MISC_EI_LEV);
    u4Val |= MISC_EI_2ED;
    BIM_WRITE32(REG_RW_MISC, u4Val);

#if GPIO_THREAD_ENABLE
    if (_i4GPIOExist == 0)
    {
        i4Ret = x_thread_create(&_hGPIOPollThread, GPIO_POLL_THREAD_NAME,
                    GPIO_POLL_STACK_SIZE, GPIO_POLL_THREAD_PRIORITY,
                    _GPIO_PollThread, 0, NULL);
        ASSERT(i4Ret==OSR_OK);
    }
#endif /* GPIO_THREAD_ENABLE */

#if 0
    UNUSED(_MT8292_Gpio_Init);
    i4Ret = PDWNC_Init();

#ifndef CC_5391_LOADER
    if(DRVCUST_InitGet(eFlagMt8292GpioSupport) == 1)
    {
        VERIFY(_MT8292_Gpio_Init() == 0);
    }
#else
    if(DRVCUST_InitGet(eFlagMt8292GpioSupport) == 1)
    {
        INT32 i4Gpio, i4Val;
        // If customization option does not set 8292 reset GPIO, it must be polled by H/W        
        i4Gpio = DRVCUST_InitGet(eMt8292ResetGpio);
        if(i4Gpio != (INT32)0xFFFFFFFF)
        {
            i4Val = 0;
            VERIFY(GPIO_Enable(i4Gpio, &i4Val) == 0); /* Change to input mode first. */    
            i4Val = 1;
            VERIFY(GPIO_Output(i4Gpio, &i4Val) == 1);
            i4Val = 1;
            VERIFY(GPIO_Enable(i4Gpio, &i4Val) == 1); /* Change to output mode. */ 
        }
        // If customization option does not set 8292 mute GPIO, it must be polled by H/W        
        i4Gpio = DRVCUST_InitGet(eMt8292MuteGpio);        
        if(i4Gpio != (INT32)0xFFFFFFFF)
        {
            i4Val = 0;
            VERIFY(GPIO_Enable(i4Gpio, &i4Val) == 0); /* Change to input mode first. */    
            i4Val = 1;
            VERIFY(GPIO_Output(i4Gpio, &i4Val) == 1);
            i4Val = 1;
            VERIFY(GPIO_Enable(i4Gpio, &i4Val) == 1); /* Change to output mode. */         
        }
    }
#endif /* CC_5391_LOADER */
#ifdef CC_MT8295
#ifndef CC_5391_LOADER
    	_MT8295_GPIO_Init();
#endif /* CC_5391_LOADER */
#endif /* CC_MT8295 */
#endif /* 0 */

    return i4Ret;
}

INT32 GPIO_Stop()
{
    INT32 i4Idx;

#if GPIO_THREAD_ENABLE
    _i4GPIOLoop = 0;
    while (!_i4GPIOLoop)
    {
        x_thread_delay(100);
    }
#endif /* GPIO_THREAD_ENABLE */

    for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
    {
        GPIO_RQ_WRITE(i4Idx, 0);
    }

    VERIFY(BIM_DisableIrq(VECTOR_EXT));
    VERIFY(BIM_DisableIrq(VECTOR_EXT_L2));
    return 0;
}

#ifndef CC_5391_LOADER
INT32 GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
#if 0
    // PDWNC PWR GPIO just pass. no pinmux selection, so just return 0.
    if ((i4Gpio >= PWR_GPIO0) && (i4Gpio <= PWR_GPIO7))
    {
        return PDWNC_GpioQuery((i4Gpio - PWR_GPIO0), pi4Intr, pi4Fall, pi4Rise);
    }
#endif /* 0 */

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_GPIO_NUM))
    {
        return -1;
    }

    if (pi4Intr)
    {
        *pi4Intr = (GPIO_INTR_REG((UINT32)i4Gpio) ? 1 : 0);
    }
    if (pi4Fall)
    {
        if (i4Gpio < GPIO_EDGE_INTR_NUM)
        {
            *pi4Fall = (GPIO_INTR_FALL_REG((UINT32)i4Gpio) ? 1 : 0);
        }
        else
        {
#if 1 // GPIO_THREAD_ENABLE
            *pi4Fall = (GPIO_POLLFALL_REG((UINT32)i4Gpio) ? 1 : 0);
#endif /* GPIO_THREAD_ENABLE */
        }
    }
    if (pi4Rise)
    {
        if (i4Gpio < GPIO_EDGE_INTR_NUM)
        {
            *pi4Rise = (GPIO_INTR_RISE_REG((UINT32)i4Gpio) ? 1 : 0);
        }
        else
        {
#if 1 // GPIO_THREAD_ENABLE
            *pi4Rise = (GPIO_POLLRISE_REG((UINT32)i4Gpio) ? 1 : 0);
#endif /* GPIO_THREAD_ENABLE */
        }
    }

    return 0;
}

INT32 GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
#if 0
    // PDWNC PWR GPIO just pass. no pinmux selection, so just return 0.
    if ((i4Gpio >= PWR_GPIO0) && (i4Gpio <= PWR_GPIO7))
    {
        return PDWNC_GpioReg((i4Gpio - PWR_GPIO0), eType, pfnCallback);
    }
    else if((i4Gpio >= SERVO_GPIO0) && (i4Gpio <= SERVO_GPIO4))
    {
        return PDWNC_ServoGpioReg((i4Gpio - SERVO_GPIO0), eType, pfnCallback);    
    }
#ifdef DEFINE_MT8292_INTERRUPT 
    else if((i4Gpio >= MT8292_GPIO0) && (i4Gpio <= MT8292_GPIO9))
    {
        return _MT8292_Gpio_Reg( i4Gpio ,  eType,  pfnCallback);
    }
#endif    
#ifdef CC_MT8295
#ifndef CC_5391_LOADER
    else if( (i4Gpio>=MT8295_GPIO0) && (i4Gpio<=MT8295_GPIO14) )
    {
        return _MT8295_GPIO_Reg( i4Gpio-MT8295_GPIO0 ,  eType,  pfnCallback);
    }
#endif /* CC_5391_LOADER */
#endif /* CC_MT8295 */
#endif /* 0 */
    
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_GPIO_NUM))
    {
        return -1;
    }

    VERIFY(BSP_PinGpioSel(i4Gpio)==0);
    _au4GpioPrevPoll[GPIO_TO_INDEX(i4Gpio)] = GPIO_IN_REG(GPIO_TO_INDEX(i4Gpio));

    /* Set the register and callback function. */
    switch(eType)
    {
    case GPIO_TYPE_INTR_FALL:
        if (i4Gpio >= GPIO_EDGE_INTR_NUM)
        {
            GPIO_POLLFALL_SET((UINT32)i4Gpio);
        }
        else
        {
            GPIO_INTR_FALL_SET((UINT32)i4Gpio);
        }
        break;
    case GPIO_TYPE_INTR_RISE:
        if (i4Gpio >= GPIO_EDGE_INTR_NUM)
        {
            GPIO_POLLRISE_SET((UINT32)i4Gpio);
        }
        else
        {
            GPIO_INTR_RISE_SET((UINT32)i4Gpio);
        }
        break;
    case GPIO_TYPE_INTR_BOTH:
        if (i4Gpio >= GPIO_EDGE_INTR_NUM)
        {
            GPIO_POLLRISE_SET((UINT32)i4Gpio);
            GPIO_POLLFALL_SET((UINT32)i4Gpio);
        }
        else
        {
            GPIO_INTR_RISE_SET((UINT32)i4Gpio);
            GPIO_INTR_FALL_SET((UINT32)i4Gpio);
        }
        break;
    case GPIO_TYPE_INTR_LEVEL:
        break;
#if GPIO_THREAD_ENABLE
    case GPIO_TYPE_POLL_BOTH:
        GPIO_POLLRISE_SET((UINT32)i4Gpio);
        GPIO_POLLFALL_SET((UINT32)i4Gpio);
        break;
    case GPIO_TYPE_POLL_FALL:
        GPIO_POLLFALL_SET((UINT32)i4Gpio);
        break;
    case GPIO_TYPE_POLL_RISE:
        GPIO_POLLRISE_SET((UINT32)i4Gpio);
        break;
#endif /* GPIO_THREAD_ENABLE */
    case GPIO_TYPE_NONE:
#if GPIO_THREAD_ENABLE
        GPIO_POLLRISE_CLR((UINT32)i4Gpio);
        GPIO_POLLFALL_CLR((UINT32)i4Gpio);
#endif /* GPIO_THREAD_ENABLE */
        GPIO_INTR_CLR((UINT32)i4Gpio);
        _afnGpioCallback[i4Gpio] = NULL;
        return 0;
    default:
        return -1;
    }

    // Enable interrupt bit for the specific gpio number.
    if (i4Gpio >= GPIO_EDGE_INTR_NUM)
    {
        GPIO_INTR_SET((UINT32)i4Gpio);
    }

    if (pfnCallback)
    {
        _afnGpioCallback[i4Gpio] = pfnCallback;
    }
    return 0;
}
#endif /* CC_5391_LOADER */

#if 0
static UINT32 _u4DimmingRunHighClock = 0;
static UINT32 _u4DimmingRunLowClock = 0;
static UINT32 _u4DimmingWaitHighClock = 0;
static UINT32 _u4DimmingWaitLowClock = 0;
static UINT32 _u4DimmingState = 0;
static UINT32 _u4DimmingIsrEn = 0;

static void _GPIO_DimmingIsr(UINT16 u2VctId)
{
    UINT32 u4Val;

    UNUSED(u2VctId);
    _u4DimmingState = !_u4DimmingState;

    if (_u4DimmingWaitHighClock && _u4DimmingWaitLowClock && _u4DimmingState)
    {
        _u4DimmingRunHighClock = _u4DimmingWaitHighClock;
        _u4DimmingRunLowClock = _u4DimmingWaitLowClock;
        _u4DimmingWaitHighClock = 0;
        _u4DimmingWaitLowClock = 0;
    }

    if (_u4DimmingState)    // to do the 1st high pulse.
    {
        u4Val = CKGEN_READ32(REG_GPIO_OUT2);
        u4Val |= 0x10;
        CKGEN_WRITE32(REG_GPIO_OUT2, u4Val);
        BIM_WRITE32(REG_RW_TIMER1_LOW, _u4DimmingRunHighClock);
        LOG(10, "1\n");
    }
    else
    {
        u4Val = CKGEN_READ32(REG_GPIO_OUT2);
        u4Val &= ~(0x10);
        CKGEN_WRITE32(REG_GPIO_OUT2, u4Val);
        BIM_WRITE32(REG_RW_TIMER1_LOW, _u4DimmingRunLowClock);
        LOG(10, "0\n");
    }
    // enable timer 1
    u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Val |= TMR1_CNTDWN_EN;
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);
    VERIFY(BIM_ClearIrq(VECTOR_T1));
}
#endif

INT32 GPIO_SetDimming(UINT32 u4Hz, UINT32 u4Duty)
{
#if 0
    void (* pfnOldIsr)(UINT16);
    INT32 i4Ret;
    UINT32 u4Val, u4Clk;
    CRIT_STATE_T rState;

    VERIFY(0==BSP_PinGpioSel(GPIO_OPWM1));

    if ((u4Duty == 0) || (u4Duty == 100))
    {
        i4Ret = x_reg_isr(VECTOR_T1, NULL, &pfnOldIsr);
        ASSERT(i4Ret == OSR_OK);
        i4Ret = (u4Duty) ? 1 : 0;
        VERIFY(GPIO_Output(GPIO_OPWM1, &i4Ret)==i4Ret);
        _u4DimmingRunHighClock = 0;
        _u4DimmingRunLowClock = 0;
        _u4DimmingWaitHighClock = 0;
        _u4DimmingWaitLowClock = 0;
        _u4DimmingState = 0;
        _u4DimmingIsrEn = 0;
        i4Ret = (u4Duty) ? 1 : 0;
        VERIFY(GPIO_Output(GPIO_OPWM1, &i4Ret)==i4Ret);
        return 0;
    }

    // Only 4Hz ~ 400 Hz
    ASSERT(u4Hz > 4);
    ASSERT(u4Hz < 400);

    // Calculate clock from u4Hz and u4Duty
    u4Clk = BSP_GetDomainClock(SRC_BUS_CLK);
    ASSERT(u4Clk != 0);
    // u4Val is the clock number per pulse.
    u4Val = u4Clk / u4Hz;

    // u4Duty is the high pulse percentage. critical section
    rState = x_crit_start();
    _u4DimmingWaitHighClock = (u4Val * u4Duty) / 100;
    _u4DimmingWaitLowClock = u4Val - _u4DimmingWaitHighClock;
    ASSERT(_u4DimmingWaitHighClock!=0);
    ASSERT(_u4DimmingWaitLowClock!=0);
    x_crit_end(rState);
    LOG(8, "HighClock:%d\n", _u4DimmingWaitHighClock);
    LOG(8, "LowClock:%d\n", _u4DimmingWaitLowClock);

    if (_u4DimmingIsrEn == 0)
    {
        i4Ret = 1;
        VERIFY(GPIO_Output(GPIO_OPWM1, &i4Ret)==i4Ret);

        // Hook interrupt
        i4Ret = x_reg_isr(VECTOR_T1, _GPIO_DimmingIsr, &pfnOldIsr);
        ASSERT(i4Ret == OSR_OK);

        // Set Dimming state is goint to do the high pulse.
        _u4DimmingState = 0;

        // set first timer 1 isr
        BIM_WRITE32(REG_RW_TIMER1_LLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER1_HLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER1_LOW , 0xffff);
        BIM_WRITE32(REG_RW_TIMER1_HIGH, 0);

        // Set Dimming is initialized.
        _u4DimmingIsrEn = 1;

        // enable timer 1
        u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
        u4Val &= ~(TMR1_AUTOLD_EN);
        u4Val |= TMR1_CNTDWN_EN;
        BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);
    }
#else
    UNUSED(u4Hz);
    UNUSED(u4Duty);
#endif /* No s/w dimming. */
    return 0;
}

static INT32 _MT8292_Gpio_Init(void)
{    
    UINT8 u1Data;
    static BOOL fgInit = FALSE;  

    if(fgInit)
    {
        return 0;
    }
    fgInit = TRUE;

#ifndef CC_5391_LOADER      
    VERIFY(x_sema_create(&_hMT8292GpioMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif /* CC_5391_LOADER */

    u1Data = 0;
    UNUSED(SIF_Write(0x100, 0x70, 0x17, &u1Data, 1));   
    u1Data = 0x14;
    UNUSED(SIF_Write(0x100, 0x70, 0x15, &u1Data, 1));     
    u1Data = 0;
    UNUSED(SIF_Write(0x100, 0x70, 0x10, &u1Data, 1));     
    u1Data = 0x1;
    UNUSED(SIF_Write(0x100, 0x70, 0x11, &u1Data, 1));      
#if 0 // Disable to avoid audio noise  
    u1Data = 0;
    UNUSED(SIF_Write(0x100, 0x70, 0xf, &u1Data, 1));     
#endif    
    u1Data = 0x17;
    UNUSED(SIF_Write(0x100, 0x70, 0x8, &u1Data, 1));    
    u1Data = 0x17;
    UNUSED(SIF_Write(0x100, 0x70, 0x9, &u1Data, 1));          
    u1Data = 0x1D;
    UNUSED(SIF_Write(0x100, 0x70, 0xa, &u1Data, 1));    
    u1Data = 0x1D;
    UNUSED(SIF_Write(0x100, 0x70, 0xb, &u1Data, 1));

#if  defined(DEFINE_MT8292_INTERRUPT) && !defined(CC_5391_LOADER)
{
    static HANDLE_T hMT8292_IntrqThread;
    VERIFY( x_thread_create(&hMT8292_IntrqThread, 
                    "MT8292_IntrqThread",
                     1024,   //stack size
                     128,    //priority
                     _MT8292_IntrThreadMain,
                     0,
                     NULL) == OSR_OK);        
}
#endif    
    return 0;    
}

static INT32 _MT8292_Gpio_Enable(INT32 i4GpioNum, const INT32 * pi4Polarity/*0:input, 1:output, null:query*/)
{
    UINT8 u1Val;
    UINT8 u1Addr;
    INT32 i4Ret;
    UINT32 u4Num,u4Shift;
    ASSERT( (*pi4Polarity==1)||(*pi4Polarity==0)||(pi4Polarity==NULL) );
#ifndef CC_5391_LOADER      
    UNUSED(x_sema_lock(_hMT8292GpioMutex, X_SEMA_OPTION_WAIT));
#endif /* CC_5391_LOADER */
    /* enable GPIO function */
    u1Addr =(i4GpioNum>MT8292_GPIO7)?0x28:0x27;
    MT8292_RET_ON_FAIL(SIF_Read(0x100, 0x70, u1Addr, &u1Val, 1));
    u1Val  |=  (1U<<((i4GpioNum-MT8292_GPIO0)%8));
    MT8292_RET_ON_FAIL(SIF_Write(0x100, 0x70, u1Addr, &u1Val, 1));


    /* Read Polarity Reg */
    u4Num = (((UINT32)i4GpioNum) - ((UINT32)MT8292_GPIO0));
    u1Addr = 0x29 + (u4Num >> 1);       //each reg contains two sets of gpio input/output switch
    u4Shift = (((u4Num + 1) >> 1) == (u4Num >> 1))?0:3;     //even:shift 0, odd:shift 3
    MT8292_RET_ON_FAIL(SIF_Read(0x100, 0x70, u1Addr, &u1Val, 1));
    if( pi4Polarity == NULL)                            //query IO pad direction
    {
        i4Ret = (u1Val & (1U<<u4Shift))?1:0;
    }else                                                       //set IO pad direction
    {
        u1Val &= ~(1U<< u4Shift);       //clear bit
        u1Val |= ((*pi4Polarity?1:0) << u4Shift);       //set bit
        MT8292_RET_ON_FAIL(SIF_Write(0x100, 0x70, u1Addr, &u1Val, 1));
        i4Ret =*pi4Polarity;
    }
#ifndef CC_5391_LOADER      
    UNUSED(x_sema_unlock(_hMT8292GpioMutex));
#endif /* CC_5391_LOADER */
    return i4Ret;
}

static INT32 _MT8292_Gpio_Input(INT32 i4GpioNum)
{
    UINT8 u1Val;
    UINT8 u1Addr;
    INT32 i4Ret;
#ifndef CC_5391_LOADER      
    UNUSED(x_sema_lock(_hMT8292GpioMutex, X_SEMA_OPTION_WAIT));
#endif /* CC_5391_LOADER */

    u1Addr = (i4GpioNum>MT8292_GPIO7)?0x37:0x36;
    MT8292_RET_ON_FAIL(SIF_Read(0x100, 0x70, u1Addr, &u1Val, 1));       //read input register
    i4Ret = ((u1Val&(1U<<(i4GpioNum%8)))>0?1:0) ;

#ifndef CC_5391_LOADER      
    UNUSED(x_sema_unlock(_hMT8292GpioMutex));
#endif /* CC_5391_LOADER */
    return i4Ret;
}

static INT32 _MT8292_Gpio_Output(INT32 i4GpioNum, const INT32 *pfgSet)
{
    UINT8 u1Val;
    UINT8 u1Addr;
    UINT32 u4Shift;
    UINT32 u4Num;
    INT32 i4Gpio;
    INT32 i4Val;

#ifndef CC_5391_LOADER      
    UNUSED(x_sema_lock(_hMT8292GpioMutex, X_SEMA_OPTION_WAIT));
#endif /* CC_5391_LOADER */

    i4Gpio = (INT32)DRVCUST_InitGet(eMt8292ResetGpio);
    if ((i4Gpio != (INT32)0xFFFFFFFF) && (i4Gpio < (INT32)MT8292_GPIO0))
    {
        i4Val = 1;
        VERIFY(GPIO_Enable(i4Gpio, &i4Val) == 1); /* Change to output mode. */ 
    }

    u4Num = (((UINT32)i4GpioNum) - ((UINT32)MT8292_GPIO0));
    u1Addr = 0x29 + (u4Num >> 1);
    MT8292_RET_ON_FAIL(SIF_Read(0x100, 0x70, u1Addr, &u1Val, 1));

    u4Shift = (((u4Num + 1) >> 1) == (u4Num >> 1))?0:3;
    u1Val |= (0x7 << u4Shift);
    
    MT8292_RET_ON_FAIL(SIF_Write(0x100, 0x70, u1Addr, &u1Val, 1));
    if (i4GpioNum < MT8292_GPIO8)
    {
        MT8292_RET_ON_FAIL(SIF_Read(0x100, 0x70, 0x27, &u1Val, 1));
        u1Val |= 1 << u4Num;
        MT8292_RET_ON_FAIL(SIF_Write(0x100, 0x70, 0x27, &u1Val, 1));

        MT8292_RET_ON_FAIL(SIF_Read(0x100, 0x70, 0x2E, &u1Val, 1));
        
        if (pfgSet == NULL)
        {
            return ((u1Val >> u4Num) & 0x1);
        }                
        if (*pfgSet)
        {
            u1Val |= 1U << u4Num;
            MT8292_RET_ON_FAIL(SIF_Write(0x100, 0x70, 0x2E, &u1Val, 1));
        }
        else
        {
            u1Val &= ~(1U << u4Num);
            MT8292_RET_ON_FAIL(SIF_Write(0x100, 0x70, 0x2E, &u1Val, 1));        
        }
    }
    else if(i4GpioNum <= MT8292_GPIO9)    	
    {
        u4Num -= 8;
        MT8292_RET_ON_FAIL(SIF_Read(0x100, 0x70, 0x28, &u1Val, 1));
        u1Val |= 1U << u4Num;
        MT8292_RET_ON_FAIL(SIF_Write(0x100, 0x70, 0x28, &u1Val, 1));

        MT8292_RET_ON_FAIL(SIF_Read(0x100, 0x70, 0x2F, &u1Val, 1));

        if (pfgSet == NULL)
        {
            return ((u1Val >> u4Num) & 0x1);
        }                
        if(*pfgSet == 1)
        {
            u1Val |= 1U << u4Num;
            MT8292_RET_ON_FAIL(SIF_Write(0x100, 0x70, 0x2F, &u1Val, 1));
        }
        else
        {
            u1Val &= ~(1U << u4Num);
            MT8292_RET_ON_FAIL(SIF_Write(0x100, 0x70, 0x2F, &u1Val, 1));        
        }
    }
    else
    {
        return -1;
    }

#ifndef CC_5391_LOADER      
    UNUSED(x_sema_unlock(_hMT8292GpioMutex));
#endif /* CC_5391_LOADER */

    return *pfgSet;
}

#if defined(DEFINE_MT8292_INTERRUPT) && !defined(CC_5391_LOADER)
static PFN_GPIO_CALLBACK ar8292_GpioCallback[MT8292_GPIO9-MT8292_GPIO0+1];
static GPIO_TYPE ar8292_GpioType[MT8292_GPIO9-MT8292_GPIO0+1];
static UINT8 ar8292_Input_Bits[MT8292_GPIO9-MT8292_GPIO0+1];
static UINT8 ar8292_Enable_Bits[MT8292_GPIO9-MT8292_GPIO0+1];
static INT32 _MT8292_Gpio_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
#ifndef CC_5391_LOADER      
    UNUSED(x_sema_lock(_hMT8292GpioMutex, X_SEMA_OPTION_WAIT));
#endif /* CC_5391_LOADER */
    INT32 fgSet; 
    /*----check parameters-----*/
    ASSERT(i4Gpio>= MT8292_GPIO0);
    ASSERT(i4Gpio<=MT8292_GPIO9);
    ASSERT(eType==GPIO_TYPE_INTR_FALL || eType==GPIO_TYPE_INTR_RISE \
        || eType==GPIO_TYPE_INTR_BOTH || eType==GPIO_TYPE_INTR_BOTH ||  eType==GPIO_TYPE_NONE );

    if( i4Gpio>MT8292_GPIO9 || i4Gpio<MT8292_GPIO0 )
    {
        return -1;
    }
    
    switch(eType)
    {
    case GPIO_TYPE_INTR_FALL:
    case GPIO_TYPE_INTR_RISE:
    case GPIO_TYPE_INTR_BOTH:
        ar8292_GpioType[(i4Gpio-MT8292_GPIO0)]=eType;
        fgSet = 1;
        _MT8292_Gpio_Intrq( i4Gpio, &fgSet );
        break;
    case GPIO_TYPE_NONE:
        ar8292_GpioType[(i4Gpio-MT8292_GPIO0)]=GPIO_TYPE_NONE; //clean
        ar8292_GpioCallback[(i4Gpio-MT8292_GPIO0)] = NULL;                
        fgSet = 0;
        _MT8292_Gpio_Intrq( i4Gpio, &fgSet );
        break;
    case GPIO_TYPE_INTR_LEVEL:
        //not support
    default:
        return -2;
    }

    if (pfnCallback)
    {
        ar8292_GpioCallback[(i4Gpio-MT8292_GPIO0)] = pfnCallback;
    }
#ifndef CC_5391_LOADER      
    UNUSED(x_sema_unlock(_hMT8292GpioMutex));
#endif /* CC_5391_LOADER */
    return 0;
}
static INT32 _MT8292_Gpio_Intrq(INT32 i4Gpio, const INT32 *pfgSet)
{
#ifndef CC_5391_LOADER      
    UNUSED(x_sema_lock(_hMT8292GpioMutex, X_SEMA_OPTION_WAIT));
#endif /* CC_5391_LOADER */
    /*---check parameters----*/
    ASSERT(i4Gpio>= MT8292_GPIO0);
    ASSERT(i4Gpio<=MT8292_GPIO9);
    ASSERT( pfgSet ==NULL  || *pfgSet==1 || *pfgSet==0 );
    INT32 i4Ret;
    if( i4Gpio>MT8292_GPIO9 || i4Gpio<MT8292_GPIO0 )
    {
        return -1;
    }

    /*----- compare gpio input bits  ------*/
    if( pfgSet ==NULL)  //query
    {
        i4Ret= (INT32) ar8292_Enable_Bits[(i4Gpio-MT8292_GPIO0)] ;
    }
    else
    {
        ar8292_Enable_Bits[(i4Gpio-MT8292_GPIO0)] = *pfgSet;
        i4Ret = *pfgSet;
    }
  
#ifndef CC_5391_LOADER      
    UNUSED(x_sema_unlock(_hMT8292GpioMutex));
#endif /* CC_5391_LOADER */
    return (i4Ret);    
}
static  void  _MT8292_IntrThreadMain( void * pvArg)
{
    UINT32 u4GpioNum;        UINT32 i4Var;

    /* remember input bits for the first time */
    for( u4GpioNum =MT8292_GPIO0 ; u4GpioNum<=MT8292_GPIO9 ; u4GpioNum++)
    {
        i4Var =_MT8292_Gpio_Input(u4GpioNum);
        ar8292_Input_Bits[(u4GpioNum-MT8292_GPIO0)]=i4Var;
    }
    
    while( TRUE )
    {
        x_thread_delay(100);
        /*           compare input bits             */
        for( u4GpioNum =MT8292_GPIO0 ; u4GpioNum<=MT8292_GPIO9 ; u4GpioNum++)
        {
            i4Var =_MT8292_Gpio_Input(u4GpioNum);
            if( i4Var != ar8292_Input_Bits[(u4GpioNum-MT8292_GPIO0)] )       //transcation happens !!
            {
                ar8292_Input_Bits[(u4GpioNum-MT8292_GPIO0)] = i4Var;         //dont forget update input bits
                //GPI( i ) has occured
                   //-->check polarity
                if( ar8292_Enable_Bits[(u4GpioNum-MT8292_GPIO0)]==1  )
                {
                    if (
                        (ar8292_GpioType[(u4GpioNum-MT8292_GPIO0)] == GPIO_TYPE_INTR_BOTH) ||                                         
                        (ar8292_GpioType[(u4GpioNum-MT8292_GPIO0)] == GPIO_TYPE_INTR_RISE &&  (i4Var==1))    ||  // low to high
                        (ar8292_GpioType[(u4GpioNum-MT8292_GPIO0)] == GPIO_TYPE_INTR_FALL &&  (i4Var==0))       //  high to low
                                    ) 
                    {
                        LOG(7, "8292 gpio %d 's polarity is %s, callback addr %08X\n", u4GpioNum, (i4Var==1)?"high":"low", ar8292_GpioCallback[(u4GpioNum-MT8292_GPIO0)]);
                        (*ar8292_GpioCallback[(u4GpioNum-MT8292_GPIO0)])( u4GpioNum,  i4Var?TRUE:FALSE );
                    }
                }
            }
        }
    }
}
#endif  //defined(DEFINE_MT8292_INTERRUPT) && !defined(CC_5391_LOADER)

//-----------------------------------------------------------------------------
/** GPIO_VotageLevel()  The GPIO VoltageLevel Sense functions. It will check the 
 *  i4GpioNum and read related register bit to return. (but will not switch 
 *  PinMux.
 *  @param i4GpioNum the gpio number to read.
 *  @retval 0 or 1.  (GPIO input value.)
 */
//-----------------------------------------------------------------------------
INT32 GPIO_VotageLevel(INT32 i4GpioNum)
{
    UINT32 u4Val;
    INT32 i4Idx;

    // PDWNC PWR GPIO just pass. no pinmux selection, so just return 0.
    if ((i4GpioNum >= PWR_GPIO0) && (i4GpioNum <= PWR_GPIO8))
    {
        return -1;
        //return PDWNC_GpioInput(i4GpioNum - PWR_GPIO0);
    }
    // Servo GPIO
    else if ((i4GpioNum >= SERVO_GPIO0) && (i4GpioNum <= SERVO_GPIO4))
    {
        return -1;
        //return PDWNC_ServoGpioInput(i4GpioNum - SERVO_GPIO0);
    }
    
    if ((i4GpioNum < 0) || (i4GpioNum >= TOTAL_GPIO_NUM))
    {
        return -1;
    }

    // Set the input mode.
    // i4Idx = 0;
    // GPIO_Enable(i4GpioNum, &i4Idx);
    //VERIFY(BSP_PinGpioSel(i4GpioNum)==0);

    // Read input value.
    i4Idx = ((UINT32)i4GpioNum >> 5);
    u4Val = GPIO_IN_REG(i4Idx);

    return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1U : 0);
}

#if 0
/*************** MT8295 GPIO Driver Functions *******************************/
#ifndef CC_5391_LOADER

#define TOTAL_8295_GPIO_NUM 14
#define REG_POD_GPR_CNTL_SET0   0x88
#define REG_POD_GPR_CNTL_SET1   0x8c        //29-15:GPIO_EN   14-0:GPIO_OUT
#define REG_POD_GPR_CNTL_SET2   0x90        
#define REG_POD_GPR_CNTL_SET3   0x94
#define REG_POD_GPR_CNTL_SET4   0x98        //14-0:GPIO_IN
#define REG_GPIO_INT_ENABLE     0x84
PFN_GPIO_CALLBACK ar8295_GpioCallback[TOTAL_8295_GPIO_NUM];
GPIO_TYPE ar8295_GpioType[TOTAL_8295_GPIO_NUM];

static UINT32 _PCMCIA_READ32(UINT32 u4Addr)
{
        UINT32 u4Data;
        NAND_PodRead(u4Addr>>2, &u4Data);
        return u4Data;
}
static INT32 _PCMCIA_WRITE32(UINT32 u4Addr, UINT32 value)
{
       INT32 i4ret;
       i4ret =  NAND_PodWrite(u4Addr>>2,value); 
       return i4ret;
}

INT32 _MT8295_GPIO_Enable(INT32 i4GpioNum, const INT32 * pi4Polarity/*0:input, 1:output, null:query*/)
{
    ASSERT(i4GpioNum>= 0);
    ASSERT(i4GpioNum<=TOTAL_8295_GPIO_NUM);
    ASSERT( *pi4Polarity==1 || *pi4Polarity==0 || pi4Polarity==NULL );    
    _MT8295_GPIO_Init();
    
    INT32 i4Ret;
    UINT32 u4EnbValue , u4Val;
    UINT32 u4EnbMask = (1<<i4GpioNum)<<15;
    UINT32 u4RegValue = _PCMCIA_READ32(REG_POD_GPR_CNTL_SET1);
    if( pi4Polarity == NULL )// just query
    {
    	i4Ret = ( (u4RegValue & u4EnbMask)>0?1:0 );
    	return i4Ret;
    }

    u4EnbValue = (*pi4Polarity<<i4GpioNum)<<15;     
    u4Val = u4RegValue & ~u4EnbMask | u4EnbValue;
    _PCMCIA_WRITE32(REG_POD_GPR_CNTL_SET1,  u4Val );
    return (*pi4Polarity);
}

INT32 _MT8295_GPIO_Input(INT32 i4GpioNum)
{
    ASSERT(i4GpioNum>= 0);
    ASSERT(i4GpioNum<=TOTAL_8295_GPIO_NUM);
    INT32 i4Ret;
    _MT8295_GPIO_Init();

    UINT32 u4InMask = (1<<i4GpioNum);
    UINT32 u4InRegValue = _PCMCIA_READ32(REG_POD_GPR_CNTL_SET4);

    i4Ret = ((u4InRegValue & u4InMask)>0?1:0);
    return i4Ret;  
}

INT32 _MT8295_GPIO_Output(INT32 i4GpioNum, const INT32 * pi4DriveHighLow/*0:drive0, 1:drive 1, null:query*/) 
{
    ASSERT(i4GpioNum>= 0);
    ASSERT(i4GpioNum<=TOTAL_8295_GPIO_NUM);
    ASSERT( *pi4DriveHighLow==1 || *pi4DriveHighLow==0 || pi4DriveHighLow==NULL );    
    _MT8295_GPIO_Init();

    
    INT32 i4Ret;
    UINT32 u4OutValue , u4Val;
    UINT32 u4OutMask = (1<<i4GpioNum);    
    UINT32 u4RegValue = _PCMCIA_READ32(REG_POD_GPR_CNTL_SET1);
    if( pi4DriveHighLow == NULL)
    {
    	i4Ret = ( (u4RegValue & u4OutMask)>0?1:0 );
    	return i4Ret;
    }
    u4OutValue = (*pi4DriveHighLow<<i4GpioNum);  
    u4Val = u4RegValue & ~u4OutMask | u4OutValue;
    _PCMCIA_WRITE32(REG_POD_GPR_CNTL_SET1,  u4Val);
    return (*pi4DriveHighLow);
}

INT32 _MT8295_GPIO_Intrq(INT32 i4Gpio, const INT32 *pfgSet)
{
    /*---check parameters----*/
    ASSERT(i4Gpio>= 0);
    ASSERT(i4Gpio<=TOTAL_8295_GPIO_NUM);
    ASSERT( pfgSet ==NULL  || *pfgSet==1 || *pfgSet==0 );
    _MT8295_GPIO_Init();

    UINT32 u4Reg;
    if( i4Gpio>TOTAL_8295_GPIO_NUM || i4Gpio<0 )
    {
        return -1;
    }

    /*-----manipulate registers------*/
    u4Reg  =_PCMCIA_READ32(REG_GPIO_INT_ENABLE);
    if( pfgSet ==NULL)  //query
    {
        return ( u4Reg & (1U<<i4Gpio) );
    }
    else
    {
        _PCMCIA_WRITE32( REG_GPIO_INT_ENABLE, u4Reg&~(1U<<i4Gpio)|(*pfgSet<<i4Gpio) );        //clean bit&set bit
    }
    LOG(7, "_MT8295_GPIO_Intrq: (REG_GPIO_INT_ENABLE)=%08X\n", _PCMCIA_READ32(REG_GPIO_INT_ENABLE));
   
    return (*pfgSet);    
}

INT32 _MT8295_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
    INT32 fgSet; 
    /*----check parameters-----*/
    ASSERT(i4Gpio>= 0);
    ASSERT(i4Gpio<=TOTAL_8295_GPIO_NUM);
    ASSERT(eType==GPIO_TYPE_INTR_FALL || eType==GPIO_TYPE_INTR_RISE \
        || eType==GPIO_TYPE_INTR_BOTH || eType==GPIO_TYPE_INTR_BOTH ||  eType==GPIO_TYPE_NONE );
    _MT8295_GPIO_Init();

    if( i4Gpio>TOTAL_8295_GPIO_NUM || i4Gpio<0 )
    {
        return -1;
    }
    
    switch(eType)
    {
    case GPIO_TYPE_INTR_FALL:
    case GPIO_TYPE_INTR_RISE:
    case GPIO_TYPE_INTR_BOTH:
        ar8295_GpioType[i4Gpio]=eType;
        fgSet = 1;
        _MT8295_GPIO_Intrq( i4Gpio, &fgSet );
        break;
    case GPIO_TYPE_NONE:
        ar8295_GpioType[i4Gpio]=GPIO_TYPE_NONE; //clean
        ar8295_GpioCallback[i4Gpio] = NULL;                
        fgSet = 0;
        _MT8295_GPIO_Intrq( i4Gpio, &fgSet );
        break;
    case GPIO_TYPE_INTR_LEVEL:
        //not support
    default:
        return -2;
    }

    if (pfnCallback)
    {
        ar8295_GpioCallback[i4Gpio] = pfnCallback;
    }
    return 0;
}
#endif /* 0 */
/*
INT32 _MT8295_GPIO_GetIn(INT32 i4GpioNum)
{
    ASSERT(i4GpioNum>= 0);
    ASSERT(i4GpioNum<=14);
    _MT8295_GPIO_Enable(i4GpioNum, 0 );
    return _MT8295_GPIO_Input(i4GpioNum);
}
INT32 _MT8295_GPIO_SetOut(INT32 i4GpioNum)
{
    ASSERT(i4GpioNum>= 0);
    ASSERT(i4GpioNum<=14);
    _MT8295_GPIO_Enable(i4GpioNum, 1 );
    return _MT8295_GPIO_Output(i4GpioNum);
}
*/
#endif /* CC_5391_LOADER */

