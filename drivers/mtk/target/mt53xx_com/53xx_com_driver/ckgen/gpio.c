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
#include "x_ckgen_mt82.h"
#include "x_pinmux_mt82.h"
#include "x_gpio_mt82.h"
#include "x_bim_mt82.h"
#include "x_assert.h"
#include "drvcust_if.h"
#include "pdwnc_drvif.h"
#define DEFINE_IS_LOG	IR_IsLog
#else
#include "x_lint.h"
#include "x_bim.h"
#include "x_pdwnc.h"
#include "x_pinmux.h"
#include "x_printf.h"
#include "x_hal_5381.h"
#include "x_assert.h"
#ifndef CC_MTK_LOADER
#include "x_os.h"
#else
#include "x_irq.h" 
#endif /* CC_MTK_LOADER */
#include "sif_if.h"
#include "drvcust_if.h"
#include "pdwnc_drvif.h"

//#define DEFINE_IS_LOG IR_IsLog
#include "x_debug.h"

#include "x_os.h"
#include "drv_common.h"

#include "c_model.h"
#endif
//---------------------------------------------------------------------------
// Types
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
// #define DEFINE_MT8292_INTERRUPT


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


static const INT32 _GPIO2BitOffsetOfIntReg[NORMAL_GPIO_NUM] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, -1, -1, 13, 14, 15, 16, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 18, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 17,
    9, 10, -1, -1, -1, -1, -1, -1, 11, 12, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1  
};


#ifndef CC_MTK_LOADER
static const INT32 _BitOffsetOfIntReg2GPIO[GPIO_SUPPORT_INT_NUM] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 48, 49, 56, 57, 11, 12, 13,14,47,25
};
#endif /* CC_MTK_LOADER */


//-----------------------------------------------------------------------------
// Prototypes
//----------------------------------------------------------------------------- 
#ifndef CC_MTK_LOADER
// static INT32 _MT8292_Gpio_Init(void);
// static INT32 _MT8292_Gpio_Enable(INT32 i4GpioNum, const INT32 * pi4Polarity/*0:input, 1:output, null:query*/);
// static INT32 _MT8292_Gpio_Input(INT32 i4GpioNum);
// static INT32 _MT8292_Gpio_Output(INT32 i4GpioNum, const INT32 *pfgSet);
#endif
#if defined(DEFINE_MT8292_INTERRUPT) && !defined(CC_MTK_LOADER)

//static INT32 _MT8292_Gpio_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
//static INT32 _MT8292_Gpio_Intrq(INT32 i4Gpio, INT32 *pfgSet);
//static  void _MT8292_IntrThreadMain( void * pvArg);
#endif
static INT32 _GPIO_OwnerNumOffset(UINT32,INT32);
static INT32 _CKGEN_GPIO_Range_Check(INT32 i4GpioNum);
static INT32 _CKGEN_GPIO_Enable(INT32 i4GpioNum, INT32 *pfgSet);
static INT32 _CKGEN_GPIO_Output(INT32 i4GpioNum, INT32 *pfgSet);
static INT32 _CKGEN_GPIO_Input(INT32 i4GpioNum);
static INT32 _CKGEN_GPIO_VotageLevel(INT32 i4GpioNum);
static INT32 _CKGEN_GPIO_Intrq(INT32 i4GpioNum, INT32 *pfgSet);
static INT32 _CKGEN_GPIO_Init(void);
static INT32 _CKGEN_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise);
static INT32 _CKGEN_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
static INT32 _CKGEN_GPIO_Stop(void);
#ifndef CC_MTK_LOADER
//static void _GPIO_BothEdgeIsr(UINT16 u2VctId);
static void _GPIO_DedicatedIsr(UINT16 u2VctId);
#endif
//-----------------------------------------------------------------------------
// Imported functions
//----------------------------------------------------------------------------- 
#ifdef CC_MT8295
//#if 1
#ifndef CC_MTK_LOADER
extern INT32 _MT8295_GPIO_Init(void);
static INT32 _MT8295_GPIO_Enable(INT32 i4GpioNum, INT32 * pi4Polarity/*0:input, 1:output, null:query*/);
static INT32 _MT8295_GPIO_Input(INT32 i4GpioNum);
static INT32 _MT8295_GPIO_Output(INT32 i4GpioNum, INT32 * pi4DriveHighLow/*0:drive0, 1:drive 1, null:query*/) ;
static INT32 _MT8295_GPIO_Range_Check(INT32 i4GpioNum);
#endif
//#if defined(DEFINE_MT8292_INTERRUPT) && !defined(CC_MTK_LOADER)
#ifndef CC_MTK_LOADER
static INT32 _MT8295_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
extern INT32 _MT8295_GPIO_Intrq(INT32 i4Gpio, INT32 *pfgSet);
#endif
#endif
#ifdef CC_MTK_LOADER
externC void HalEnableIRQ(void);
#else
extern BOOL LockRegAccess(void);
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT32 _u4GPIOInit = 0;

// Set all interrupt are HW(only GPIO_SUPPORT_INT_NUM)/SW(TOTAL_GPIO_IDX) double edge trigger, so need variables below.
//#ifndef CC_MTK_LOADER
#if 1
static volatile UINT32 _au4GpioPrevPoll[TOTAL_GPIO_IDX];
static volatile UINT32 _au4GpioPollFall[TOTAL_GPIO_IDX];
static volatile UINT32 _au4GpioPollRise[TOTAL_GPIO_IDX];
#endif /* CC_MTK_LOADER */

#if GPIO_THREAD_ENABLE
static HANDLE_T _hGPIOPollThread;
static volatile INT32 _i4GPIOExist = 1;
static volatile INT32 _i4GPIOLoop = 0;
#endif /* GPIO_THREAD_ENABLE */

#ifndef CC_MTK_LOADER
static volatile PFN_GPIO_CALLBACK _afnGpioCallback[NORMAL_GPIO_NUM];//NORMAL_GPIO_NUM for SW gpio polling thread
#endif /* CC_MTK_LOADER */



#define GPIO_POLLFALL_REG(gpio) \
    (_au4GpioPollFall[(gpio) >> 5] & (1U << ((gpio) & GPIO_INDEX_MASK)))

#define GPIO_POLLRISE_REG(gpio) \
    (_au4GpioPollRise[(gpio) >> 5] & (1U << ((gpio) & GPIO_INDEX_MASK)))

#define GPIO_POLLFALL_SET(gpio) \
    (_au4GpioPollFall[(gpio) >> 5] |= (1U << ((gpio) & GPIO_INDEX_MASK)))

#define GPIO_POLLFALL_CLR(gpio) \
    (_au4GpioPollFall[(gpio) >> 5] &= ~(1U << ((gpio) & GPIO_INDEX_MASK)))

#define GPIO_POLLRISE_SET(gpio) \
    (_au4GpioPollRise[(gpio) >> 5] |= (1U << ((gpio) & GPIO_INDEX_MASK)))

#define GPIO_POLLRISE_CLR(gpio) \
    (_au4GpioPollRise[(gpio)>> 5] &= ~(1U << ((gpio) & GPIO_INDEX_MASK)))


#define GPIO_INTR_RISE_REG(gpio)    \
    (GPIO_RISE_REG & (1U << ((gpio) & EDGE_INDEX_MASK)))
#define GPIO_INTR_REG(gpio)     \
    (CKGEN_REG32(REG_GPIO_EXTINTEN) & (1U << ((gpio) & GPIO_INDEX_MASK)))
#define GPIO_INTR_FALL_REG(gpio)    \
    (GPIO_FALL_REG & (1U << ((gpio) & EDGE_INDEX_MASK)))
#define GPIO_INTR_CLR(gpio)     \
    CKGEN_WRITE32(REG_GPIO_EXTINTEN, CKGEN_REG32(REG_GPIO_EXTINTEN) & (~(1U << ((gpio) & EDGE_INDEX_MASK))))



#define GPIO_INTR_FALL_SET(gpio)    \
    CKGEN_WRITE32(REG_GPIO_ED2INTEN, CKGEN_REG32(REG_GPIO_ED2INTEN) & (~(1U << ((gpio) & EDGE_INDEX_MASK)))); \
    CKGEN_WRITE32(REG_GPIO_LEVINTEN, CKGEN_REG32(REG_GPIO_LEVINTEN) & (~(1U << ((gpio) & EDGE_INDEX_MASK)))); \
    CKGEN_WRITE32(REG_GPIO_ENTPOL, CKGEN_REG32(REG_GPIO_ENTPOL) & (~(1U << ((gpio) & EDGE_INDEX_MASK)))); \
    CKGEN_WRITE32(REG_GPIO_EXTINTEN, CKGEN_REG32(REG_GPIO_EXTINTEN) | (1U << ((gpio) & EDGE_INDEX_MASK)));

#define GPIO_INTR_RISE_SET(gpio)    \
    CKGEN_WRITE32(REG_GPIO_ED2INTEN, CKGEN_REG32(REG_GPIO_ED2INTEN) & (~(1U << ((gpio) & EDGE_INDEX_MASK)))); \
    CKGEN_WRITE32(REG_GPIO_LEVINTEN, CKGEN_REG32(REG_GPIO_LEVINTEN) & (~(1U << ((gpio) & EDGE_INDEX_MASK)))); \
    CKGEN_WRITE32(REG_GPIO_ENTPOL, CKGEN_REG32(REG_GPIO_ENTPOL) | (1U << ((gpio) & EDGE_INDEX_MASK)));    \
    CKGEN_WRITE32(REG_GPIO_EXTINTEN, CKGEN_REG32(REG_GPIO_EXTINTEN) | (1U << ((gpio) & EDGE_INDEX_MASK)));
    
#define GPIO_INTR_BOTH_EDGE_SET(gpio)    \
    CKGEN_WRITE32(REG_GPIO_ED2INTEN, CKGEN_REG32(REG_GPIO_ED2INTEN) | (1U << ((gpio) & EDGE_INDEX_MASK)));    \
    CKGEN_WRITE32(REG_GPIO_LEVINTEN, CKGEN_REG32(REG_GPIO_LEVINTEN) | (1U << ((gpio) & EDGE_INDEX_MASK)));    \
    CKGEN_WRITE32(REG_GPIO_ENTPOL, CKGEN_REG32(REG_GPIO_ENTPOL) | (1U << ((gpio) & EDGE_INDEX_MASK)));    \
    CKGEN_WRITE32(REG_GPIO_EXTINTEN, CKGEN_REG32(REG_GPIO_EXTINTEN) | (1U << ((gpio) & EDGE_INDEX_MASK)));



#define GPIO_INTR_LEVEL_LOW_SET(gpio)    \
    CKGEN_WRITE32(REG_GPIO_ED2INTEN, CKGEN_REG32(REG_GPIO_ED2INTEN) & (~(1U << ((gpio) & EDGE_INDEX_MASK)))); \
    CKGEN_WRITE32(REG_GPIO_LEVINTEN, CKGEN_REG32(REG_GPIO_LEVINTEN) | (1U << ((gpio) & EDGE_INDEX_MASK))); \
    CKGEN_WRITE32(REG_GPIO_ENTPOL, CKGEN_REG32(REG_GPIO_ENTPOL) & (~(1U << ((gpio) & EDGE_INDEX_MASK)))); \
    CKGEN_WRITE32(REG_GPIO_EXTINTEN, CKGEN_REG32(REG_GPIO_EXTINTEN) | (1U << ((gpio) & EDGE_INDEX_MASK)));

#define GPIO_INTR_LEVEL_HIGH_SET(gpio)    \
    CKGEN_WRITE32(REG_GPIO_ED2INTEN, CKGEN_REG32(REG_GPIO_ED2INTEN) & (~(1U << ((gpio) & EDGE_INDEX_MASK)))); \
    CKGEN_WRITE32(REG_GPIO_LEVINTEN, CKGEN_REG32(REG_GPIO_LEVINTEN) | (1U << ((gpio) & EDGE_INDEX_MASK))); \
    CKGEN_WRITE32(REG_GPIO_ENTPOL, CKGEN_REG32(REG_GPIO_ENTPOL) | (1U << ((gpio) & EDGE_INDEX_MASK)));    \
    CKGEN_WRITE32(REG_GPIO_EXTINTEN, CKGEN_REG32(REG_GPIO_EXTINTEN) | (1U << ((gpio) & EDGE_INDEX_MASK)));

static GPIO_HANDLER_FUNC_TBL gGPIO_HANDLER_FuncTbl[] = 
{
    // pfnIsOwner, pfnInit, pfnEnable, pfnInput, pfnIntrp, pfnOutput, pfnQuery, pfnReg, pfnRegLedBlinker, pfnStop, pfnVoltageLevel  
    {
        _CKGEN_GPIO_Range_Check,
        _CKGEN_GPIO_Init,
        _CKGEN_GPIO_Enable,
        _CKGEN_GPIO_Input,
        _CKGEN_GPIO_Intrq,
        _CKGEN_GPIO_Output,
        _CKGEN_GPIO_Query,
        _CKGEN_GPIO_Reg,
        NULL,
        _CKGEN_GPIO_Stop,
        _CKGEN_GPIO_VotageLevel
    },

    {
        PDWNC_GpioRangeCheck,
        /*_PDWNC_InitGpio, _CKGEN_GPIO_Init->PDWNC_Init->_PDWNC_InitGpio*/NULL,
        PDWNC_GpioEnable,
        PDWNC_GpioInput,
        PDWNC_GpioIntrq,
        PDWNC_GpioOutput,
        PDWNC_GpioQuery,
        PDWNC_GpioReg,
        _PDWNC_RegLedBlinker,
        NULL,
        NULL
    },

    {
        PDWNC_ServoGpioRangeCheck,
        PDWNC_InitServoGpio,//        NULL,
        PDWNC_ServoGpioEnable,
        PDWNC_ServoGpioInput,
        PDWNC_ServoGpioIntrq,//        NULL,
        PDWNC_ServoGpioOutput,
        PDWNC_ServoGpioQuery,//NULL,
        PDWNC_ServoGpioReg,//NULL,
        NULL,
        NULL,
        NULL,
    }
#ifdef CC_MT8295
#ifndef CC_MTK_LOADER   
    ,
    {
        _MT8295_GPIO_Range_Check,
        _MT8295_GPIO_Init,
        _MT8295_GPIO_Enable,
        _MT8295_GPIO_Input,
        _MT8295_GPIO_Intrq,
        _MT8295_GPIO_Output,
        NULL,
        _MT8295_GPIO_Reg,
        NULL,
        NULL,
        NULL
    }
#endif /* CC_MTK_LOADER */
#endif /* CC_MT8295 */
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static INT32 _GPIO_OwnerNumOffset(UINT32 u4Val,INT32 i4GpioNum)
{
        switch(u4Val)
        {
            case GPIO_HANDLER_CKGEN:
                return i4GpioNum;
            case GPIO_HANDLER_PDWNC_GPIO:
                return i4GpioNum - OPCTRL(0);
            case GPIO_HANDLER_PDWNC_SRVAD:
                return i4GpioNum - SERVO_GPIO0;
            case GPIO_HANDLER_MT8295:
                return i4GpioNum - MT8295_GPIO0;            
        }
    return i4GpioNum;
}

//-----------------------------------------------------------------------------
/******           GPIO register read/write functions                    *******
******           GPIO register read/write functions                    *******/
//-----------------------------------------------------------------------------
UINT32 _GPIO_POLL_IN_REG(INT32 i4Idx)
{//only for normal gpio
    return GPIO_IN_REG(i4Idx);
}


/*************** CKGEN GPIO Driver Functions *******************************/
static INT32 _CKGEN_GPIO_Range_Check(INT32 i4GpioNum)
{
      if ((i4GpioNum < 0) || (i4GpioNum >= NORMAL_GPIO_NUM))
    {
            return 0;
    }
        else
        {
            return 1;
        }
}


static INT32 _CKGEN_GPIO_Enable(INT32 i4GpioNum, INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Idx;        
#ifndef CC_MTK_LOADER
        CRIT_STATE_T rCrit;
#else /* CC_MTK_LOADER */
        //UINT32 u4State;
#endif    
    i4Idx = ((UINT32)i4GpioNum >> 5);

#ifndef CC_MTK_LOADER
        rCrit = x_crit_start();
#endif /* CC_MTK_LOADER */    
    u4Val = GPIO_EN_REG(i4Idx);
    if (pfgSet == NULL)
    {
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */         
        return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1 : 0);
    }
    
    u4Val = (*pfgSet) ?
            (u4Val | (1U << (i4GpioNum & GPIO_INDEX_MASK))) :
            (u4Val & ~(1U << (i4GpioNum & GPIO_INDEX_MASK)));
    
    GPIO_EN_WRITE(i4Idx, u4Val);
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */         
    return (*pfgSet);
}

static INT32 _CKGEN_GPIO_Output(INT32 i4GpioNum, INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Idx;
#ifndef CC_MTK_LOADER
        CRIT_STATE_T rCrit;
#else /* CC_MTK_LOADER */
        //UINT32 u4State;
#endif
    i4Idx = ((UINT32)i4GpioNum >> 5);

#ifndef CC_MTK_LOADER
        rCrit = x_crit_start();
#endif /* CC_MTK_LOADER */    
    u4Val = GPIO_OUT_REG(i4Idx);
    if (pfgSet == NULL)
    {
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */     
        return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1 : 0);
    }
    
    u4Val = (*pfgSet) ?
            (u4Val | (1U << (i4GpioNum & GPIO_INDEX_MASK))) :
            (u4Val & ~(1U << (i4GpioNum & GPIO_INDEX_MASK)));
    
    GPIO_OUT_WRITE(i4Idx, u4Val);
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */     
    // Set the output mode.
    i4Idx = 1;
    //In GPIO_Enable, it will remap GPIO by itself, so restore to virtual GPIO
    VERIFY(BSP_PinGpioSel(i4GpioNum)==0);
    VERIFY(_CKGEN_GPIO_Enable(i4GpioNum, &i4Idx)==i4Idx);
    
    return (*pfgSet);
}


static INT32 _CKGEN_GPIO_Input(INT32 i4GpioNum)
{
    UINT32 u4Val;
    INT32 i4Idx;
    
    // Set the input mode.
    // i4Idx = 0;
    // GPIO_Enable(i4GpioNum, &i4Idx);
    VERIFY(BSP_PinGpioSel(i4GpioNum)==0);
    
    // Read input value.
    i4Idx = ((UINT32)i4GpioNum >> 5);
    u4Val = GPIO_IN_REG(i4Idx);
    
    return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1U : 0);

}

static INT32 _CKGEN_GPIO_VotageLevel(INT32 i4GpioNum)
{
    UINT32 u4Val;
    INT32 i4Idx;

    // Read input value.
    i4Idx = ((UINT32)i4GpioNum >> 5);
    u4Val = GPIO_IN_REG(i4Idx);
    
    return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1U : 0);
    
}


static INT32 _CKGEN_GPIO_Intrq(INT32 i4GpioNum, INT32 *pfgSet)
{
    UINT32 u4Val;
    //INT32 i4Idx;
    i4GpioNum = _GPIO2BitOffsetOfIntReg[i4GpioNum];
    if(i4GpioNum == -1)
        return -1;
  //i4Idx = ((UINT32)i4GpioNum >> 5);
    u4Val = CKGEN_REG32(REG_GPIO_EXTINTEN);
    if (pfgSet == NULL)
    {
        return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1 : 0);
    }
    
    u4Val = (*pfgSet) ?
            (u4Val | (1U << (i4GpioNum & GPIO_INDEX_MASK))) :
            (u4Val & ~(1U << (i4GpioNum & GPIO_INDEX_MASK)));
    
    CKGEN_WRITE32(REG_GPIO_EXTINTEN, u4Val);
    
    return (*pfgSet);

}


static INT32 _CKGEN_GPIO_Init()
{
    
//    UINT32 u4Val;
    INT32 i4Ret;    
#ifndef CC_MTK_LOADER    
    INT32 i4Idx;
    void (* pfnOldIsr)(UINT16);
#endif /* CC_MTK_LOADER */

        
#ifndef CC_MTK_LOADER
    for (i4Idx = 0; i4Idx < NORMAL_GPIO_NUM; i4Idx++)
    {
        _afnGpioCallback[i4Idx] = NULL;
    }
    // disable all gpio interrupt 
    CKGEN_WRITE32(REG_GPIO_EXTINTEN, 0);
    
    for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
    {
        _au4GpioPrevPoll[i4Idx] = 0;
        _au4GpioPollFall[i4Idx] = 0;
        _au4GpioPollRise[i4Idx] = 0;
    }
#endif /* CC_MTK_LOADER */

    // clear all edge triggered interrupt status 0.
    CKGEN_WRITE32(REG_GPIO_EXTINT, 0);

    i4Ret = 0;
#ifndef CC_MTK_LOADER
#ifndef CC_UP8032_ATV
//    i4Ret = x_reg_isr(VECTOR_EXT2, _GPIO_BothEdgeIsr, &pfnOldIsr);
//    ASSERT(i4Ret == OSR_OK);
    i4Ret = x_reg_isr(VECTOR_EXT1, _GPIO_DedicatedIsr, &pfnOldIsr);
    ASSERT(i4Ret == OSR_OK);
    UNUSED(i4Ret);
#endif 
#endif /* CC_MTK_LOADER */
/*
    u4Val = BIM_READ32(REG_RW_MISC);
    u4Val &= ~(MISC_EI_LEV);
    u4Val |= MISC_EI_2ED;
    BIM_WRITE32(REG_RW_MISC, u4Val);
*/
#if GPIO_THREAD_ENABLE
    if (_i4GPIOExist == 0)
    {
        i4Ret = x_thread_create(&_hGPIOPollThread, GPIO_POLL_THREAD_NAME,
                    GPIO_POLL_STACK_SIZE, GPIO_POLL_THREAD_PRIORITY,
                    _GPIO_PollThread, 0, NULL);
        ASSERT(i4Ret==OSR_OK);
    }
#endif /* GPIO_THREAD_ENABLE */

    i4Ret = PDWNC_Init();//why we put PDWNC_Init() here...?
    {
        PFN_GPIO_TIMERINIT_FUNC pfnTimerInit = NULL;

        // call driver customization loader env init function.
        DRVCUST_InitQuery(eGpiotimerInitFunc, (UINT32 *)(void *)&pfnTimerInit);
        if (pfnTimerInit != NULL)
        {
            // call driver customization gpio timer init function.
            pfnTimerInit();
        }
    }

    // Move from _BoardInit() at board.c for MW/Demo/Linux sharing
#if !defined(CC_MTK_LOADER) && defined(CUSTOMER_PASSWORD)
    {
        UINT8 *pu1CustPass;
        if ((DRVCUST_OptQuery(eCustomerPassword, (UINT32 *)&pu1CustPass)==0) &&
            (*pu1CustPass != 0)) 
        {
            LockRegAccess();
        }
    }
#endif

    return i4Ret;   
}

//david: may need to add level trigger information for this query function
static INT32 _CKGEN_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
    if (pi4Intr)
    {
        *pi4Intr = (GPIO_INTR_REG((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]) ? 1 : 0);
    }
    if (pi4Fall)
    {
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] != -1)
        {
            *pi4Fall = (GPIO_INTR_FALL_REG((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]) ? 1 : 0);
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
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] != -1)
        {
            *pi4Rise = (GPIO_INTR_RISE_REG((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]) ? 1 : 0);
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


static INT32 _CKGEN_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
//remove warning for klocwork checking
     if ((i4Gpio < 0) || (i4Gpio >= NORMAL_GPIO_NUM))
    {
        return -1;
    }        
    VERIFY(BSP_PinGpioSel(i4Gpio)==0);

    _au4GpioPrevPoll[GPIO_TO_INDEX(i4Gpio)] = _GPIO_POLL_IN_REG(GPIO_TO_INDEX(i4Gpio));

    /* Set the register and callback function. */
    switch(eType)
    {
    case GPIO_TYPE_INTR_FALL:
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] == -1)
        {
            GPIO_POLLFALL_SET((UINT32)i4Gpio);
        }
        else
        {
            GPIO_INTR_FALL_SET((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]);
        }
        break;
    case GPIO_TYPE_INTR_RISE:
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] == -1)
        {
            GPIO_POLLRISE_SET((UINT32)i4Gpio);
        }
        else
        {
            GPIO_INTR_RISE_SET((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]);
        }
        break;
    case GPIO_TYPE_INTR_BOTH:
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] == -1)
        {
            GPIO_POLLRISE_SET((UINT32)i4Gpio);
            GPIO_POLLFALL_SET((UINT32)i4Gpio);
        }
        else
        {
            GPIO_INTR_BOTH_EDGE_SET((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]);
        }
        break;
    case GPIO_TYPE_INTR_LEVEL_LOW:
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] != -1)
        {
            GPIO_INTR_LEVEL_LOW_SET((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]);
        }
        break;
    case GPIO_TYPE_INTR_LEVEL_HIGH:
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] != -1)
        {
            GPIO_INTR_LEVEL_HIGH_SET((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]);
        }
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
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] != -1)
        {
            GPIO_INTR_CLR((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]);
        }            
#ifndef CC_MTK_LOADER        
        _afnGpioCallback[i4Gpio] = NULL;
#endif        
        return 0;
    default:
        return -1;
    }
#ifndef CC_MTK_LOADER
    if (pfnCallback)
    {
        _afnGpioCallback[i4Gpio] = pfnCallback;
    }
#endif    
    return 0;
}

static INT32 _CKGEN_GPIO_Stop()
{
//    INT32 i4Idx;

#if GPIO_THREAD_ENABLE
    _i4GPIOLoop = 0;
    while (!_i4GPIOLoop)
    {
        x_thread_delay(100);
    }
#endif /* GPIO_THREAD_ENABLE */

        CKGEN_WRITE32(REG_GPIO_EXTINTEN, 0);
#ifndef CC_UP8032_ATV
    VERIFY(BIM_DisableIrq(VECTOR_EXT1));
#endif
//    VERIFY(BIM_DisableIrq(VECTOR_EXT2));
    return 0;
}

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
INT32 GPIO_Enable(INT32 i4GpioNum, INT32 *pfgSet)
{
    UINT32 u4Val;

        for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
        {
            if(gGPIO_HANDLER_FuncTbl[u4Val].pfnIsOwner(i4GpioNum))
            {
    //            VERIFY(BSP_PinGpioSel(i4GpioNum)==0);            
                if(gGPIO_HANDLER_FuncTbl[u4Val].pfnEnable != NULL)
                    return  gGPIO_HANDLER_FuncTbl[u4Val].pfnEnable(_GPIO_OwnerNumOffset(u4Val, i4GpioNum), pfgSet);
                else
                    Printf( "The enable function of this module is NULL\n");    
            }       
        }
        return -1;
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
INT32 GPIO_Output(INT32 i4GpioNum, INT32 *pfgSet)
{
    UINT32 u4Val;

        for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
        {
            if(gGPIO_HANDLER_FuncTbl[u4Val].pfnIsOwner(i4GpioNum))
            {
//                VERIFY(BSP_PinGpioSel(i4GpioNum)==0);
                if(gGPIO_HANDLER_FuncTbl[u4Val].pfnOutput != NULL)
                    return  gGPIO_HANDLER_FuncTbl[u4Val].pfnOutput(_GPIO_OwnerNumOffset(u4Val, i4GpioNum), pfgSet);
                else
                    Printf( "The output function of this module is NULL\n");    
            }       
        }
        return -1;      
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

        for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
        {
            if(gGPIO_HANDLER_FuncTbl[u4Val].pfnIsOwner(i4GpioNum))
            {
//                VERIFY(BSP_PinGpioSel(i4GpioNum)==0);                        
                if(gGPIO_HANDLER_FuncTbl[u4Val].pfnInput != NULL)
                    return  gGPIO_HANDLER_FuncTbl[u4Val].pfnInput(_GPIO_OwnerNumOffset(u4Val, i4GpioNum));
                else
                    Printf( "The input function of this module is NULL\n");                     
            }       
        }
        return -1;      
}

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

        for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
        {
            if(gGPIO_HANDLER_FuncTbl[u4Val].pfnIsOwner(i4GpioNum))
            {
                if(gGPIO_HANDLER_FuncTbl[u4Val].pfnVoltageLevel != NULL)
                    return  gGPIO_HANDLER_FuncTbl[u4Val].pfnVoltageLevel(_GPIO_OwnerNumOffset(u4Val, i4GpioNum));
                else
                    Printf( "The VoltageLevel function of this module is NULL\n");                      
            }       
        }
        return -1;
}

#ifndef CC_MTK_LOADER
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
INT32 GPIO_Intrq(INT32 i4GpioNum, INT32 *pfgSet)
{
    UINT32 u4Val;
    
        for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
        {
            if(gGPIO_HANDLER_FuncTbl[u4Val].pfnIsOwner(i4GpioNum))
            {
                if(gGPIO_HANDLER_FuncTbl[u4Val].pfnIntrq != NULL)
                    return  gGPIO_HANDLER_FuncTbl[u4Val].pfnIntrq(_GPIO_OwnerNumOffset(u4Val, i4GpioNum), pfgSet);
                else
                    Printf( "The intrq function of this module is NULL\n");                             
                    
            }       
        }
        return -1;      
}
#endif /* CC_MTK_LOADER */

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

    //In mt536x, We need to remap GPIO number from virtual to physical
    //i4GpioNum = _GPIO_Remap(i4GpioNum);

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

    //In mt536x, We need to remap GPIO number from virtual to physical
    //i4GpioNum = _GPIO_Remap(i4GpioNum);
        
    // Set to input mode.
    i4Mode = 0;
    VERIFY(GPIO_Enable(i4GpioNum, &i4Mode)==i4Mode);

    return GPIO_Input(i4GpioNum);
}

//-----------------------------------------------------------------------------
// GPIO Interface functions.
//-----------------------------------------------------------------------------


#ifndef CC_MTK_LOADER
#if 0
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

    ASSERT(u2VctId == VECTOR_EXT2);
    VERIFY(BIM_ClearIrq((UINT32)u2VctId));

    for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
    {
        /* Check falling 1->0 */
        u4Mask = _au4GpioPrevPoll[i4Idx] & _au4GpioPollFall[i4Idx];

        if (u4Mask)
        {
            _GPIO_CheckCallback(u4Mask, i4Idx, NORMAL_GPIO_NUM, 0);
        }

        /* Check Rising. 0->1 */
        u4Mask = ~(_au4GpioPrevPoll[i4Idx]) & _au4GpioPollRise[i4Idx];

        if (u4Mask)
        {
            _GPIO_CheckCallback(u4Mask, i4Idx, NORMAL_GPIO_NUM, 1);
        }

        /* Assign Previous value. */
        _au4GpioPrevPoll[i4Idx] = _GPIO_POLL_IN_REG(i4Idx);
    }

}
#endif


static void _GPIO_DedicatedIsr(UINT16 u2VctId)
{
    INT32 i4Idx;
    UINT32 u4Status;
    UINT32 u4Val;
    INT32 i4GpioNum;

    ASSERT(u2VctId == VECTOR_EXT1);
#ifndef CC_UP8032_ATV
    VERIFY(BIM_ClearIrq((UINT32)u2VctId));
#endif
    // get edge triggered interrupt status.       
    u4Status = CKGEN_REG32(REG_GPIO_EXTINT);
    // handle edge triggered interrupt.
    for (i4Idx = 0; i4Idx < GPIO_SUPPORT_INT_NUM; i4Idx++)
    {
        u4Val = u4Status & (1 << i4Idx);
        if(u4Val)
        {
            i4GpioNum = _BitOffsetOfIntReg2GPIO[i4Idx];
            if ((i4GpioNum < NORMAL_GPIO_NUM) && (_afnGpioCallback[i4GpioNum] != NULL))
            { 
                _afnGpioCallback[i4GpioNum](i4GpioNum, _GPIO_POLL_IN_REG(GPIO_TO_INDEX(i4GpioNum)) & (1U << (i4GpioNum & GPIO_INDEX_MASK))? 1:0);
            }   
        }
    }   
         // clear edge triggered interrupt status.       
    CKGEN_WRITE32(REG_GPIO_EXTINT, 0);
}

#endif /* CC_MTK_LOADER */

#if GPIO_THREAD_ENABLE
static void _GPIO_PollThread(void *pvDummy)
{
    INT32 i4Idx, i4Bit, i4Gpio;
    UINT32 u4Mask;

    UNUSED(pvDummy);
    for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
    {
        _au4GpioPrevPoll[i4Idx] = _GPIO_POLL_IN_REG(i4Idx);
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
                        ~(_GPIO_POLL_IN_REG(i4Idx)) & _au4GpioPollFall[i4Idx];

            if (u4Mask)
            {
                _GPIO_CheckCallback(u4Mask, i4Idx, NORMAL_GPIO_NUM, 0);
            }

            /* Check Rising. 0->1 */
            u4Mask = ~(_au4GpioPrevPoll[i4Idx]) &
                        _GPIO_POLL_IN_REG(i4Idx) & _au4GpioPollRise[i4Idx];

            if (u4Mask)
            {
                _GPIO_CheckCallback(u4Mask, i4Idx, NORMAL_GPIO_NUM, 1);
            }

            /* Assign Previous value. */
            _au4GpioPrevPoll[i4Idx] = _GPIO_POLL_IN_REG(i4Idx);
        }
    }
    _i4GPIOLoop = 1;
    _i4GPIOExist = 0;
}
#endif /* GPIO_THREAD_ENABLE */

INT32 GPIO_Init()
{
    UINT32 u4Val;
        
        if (_u4GPIOInit)
    {
        return 0;
    }
    _u4GPIOInit = 1;

        for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
        {
            if(gGPIO_HANDLER_FuncTbl[u4Val].pfnInit != NULL)
                gGPIO_HANDLER_FuncTbl[u4Val].pfnInit();
//            else
    //                Printf( "The init function of this module is NULL\n");                  
        }   
        return 0;

}

INT32 GPIO_Stop()
{
    UINT32 u4Val;

        for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
        {
                if(gGPIO_HANDLER_FuncTbl[u4Val].pfnStop != NULL)
                    return  gGPIO_HANDLER_FuncTbl[u4Val].pfnStop();
                else
                    Printf( "The stop function of this module is NULL\n");                      
        }   
        return -1;
    
}

#ifndef CC_MTK_LOADER
#ifndef CC_UP8032_ATV
INT32 GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
    
    UINT32 u4Val;

        for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
        {
            if(gGPIO_HANDLER_FuncTbl[u4Val].pfnIsOwner(i4Gpio))
            {
                if(gGPIO_HANDLER_FuncTbl[u4Val].pfnQuery != NULL)
                    return  gGPIO_HANDLER_FuncTbl[u4Val].pfnQuery(_GPIO_OwnerNumOffset(u4Val, i4Gpio),pi4Intr, pi4Fall, pi4Rise);
                else
                    Printf( "The query function of this module is NULL\n"); 
            }       
        }   
        return -1;

}

INT32 GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
   UINT32 u4Val;

        for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
        {
            if(gGPIO_HANDLER_FuncTbl[u4Val].pfnIsOwner(i4Gpio))
            {
//                  VERIFY(BSP_PinGpioSel(i4Gpio)==0);
                if(gGPIO_HANDLER_FuncTbl[u4Val].pfnReg != NULL)
                {
                    return  gGPIO_HANDLER_FuncTbl[u4Val].pfnReg(_GPIO_OwnerNumOffset(u4Val, i4Gpio),eType, pfnCallback);
                }
                else
                {
                    Printf( "The reg function of this module is NULL\n");
                }
            }       
        }   
        return -1;
}
#endif
#endif /* CC_MTK_LOADER */




#ifdef CC_MT8295
//#if 1
/*************** MT8295 GPIO Driver Functions *******************************/
#ifndef CC_MTK_LOADER

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

static INT32 _MT8295_GPIO_Range_Check(INT32 i4GpioNum)
{
      if( (i4GpioNum>=MT8295_GPIO0) && (i4GpioNum<=MT8295_GPIO14) )
    {
            return 0;
    }
        else
        {
            return 1;
        }   
}

static INT32 _MT8295_GPIO_Enable(INT32 i4GpioNum, INT32 * pi4Polarity/*0:input, 1:output, null:query*/)
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

INT32 _MT8295_GPIO_Output(INT32 i4GpioNum, INT32 * pi4DriveHighLow/*0:drive0, 1:drive 1, null:query*/) 
{
    ASSERT(i4GpioNum>= 0);
    ASSERT(i4GpioNum<=TOTAL_8295_GPIO_NUM);
    ASSERT( *pi4DriveHighLow==1 || *pi4DriveHighLow==0 || pi4DriveHighLow==NULL );    
    _MT8295_GPIO_Init();

    
    INT32 i4Ret;
    INT32 i4Idx;
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

    // Set the output mode.
    i4Idx = 1;
    VERIFY(i4Idx==_MT8295_GPIO_Enable(i4GpioNum, &i4Idx));

    return (*pi4DriveHighLow);
}

INT32 _MT8295_GPIO_Intrq(INT32 i4Gpio, INT32 *pfgSet)
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

static INT32 _MT8295_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
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
//    case GPIO_TYPE_INTR_LEVEL:
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
#endif /* CC_MT8295 */
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

#endif /* CC_MTK_LOADER */

