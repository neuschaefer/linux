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
//#define YAJUN
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
//#include "osai_api.h"
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
//#include "../brdcstif_vendor/common/osai_api.h"///

#include "c_model.h"
#endif
#ifndef CC_MTK_LOADER
#ifdef __KERNEL__
#include "cb_data.h"
#include "mtperipheral.h"
#endif
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
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,    

    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1  

};


#ifndef CC_MTK_LOADER
static const INT32 _BitOffsetOfIntReg2GPIO[GPIO_SUPPORT_INT_NUM] = 
{
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63	
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
#if defined(CC_FAST_INIT)
static INT32 _CKGEN_GPIO_Resume(void);
static INT32 _CKGEN_GPIO_ResumeINTEN(INT32 i4Gpio, GPIO_TYPE eType);
#endif
static INT32 _CKGEN_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise);
static INT32 _CKGEN_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
static INT32 _CKGEN_GPIO_Stop(void);
#ifdef __KERNEL__
static INT32 _CKGEN_GPIO_SetIntExtParam(INT32 i4Gpio, UINT32 u4Arg);
static INT32 _CKGEN_GPIO_GetIntExtParam(INT32 i4Gpio, UINT32 *pu4Arg);
#endif

#ifndef CC_MTK_LOADER
//static void _GPIO_BothEdgeIsr(UINT16 u2VctId);
static void _GPIO_DedicatedIsr(UINT16 u2VctId);

#if GPIO_THREAD_ENABLE
static void _GPIO_PollThread(void *pvDummy);
#endif
#endif
#if 0
static INT32 _MT5135_Write(UINT8 ucRegAddr, UINT8 *pucBuffer, UINT16 ucByteCount);
static INT32 _MT5135_Read(UINT8 ucRegAddr, UINT8 *pucBuffer, UINT16 ucByteCount);
static INT32 _MT5135_GPIO_Range_Check(INT32 i4GpioNum);
static INT32 _MT5135_GPIO_Enable(INT32 i4GpioNum, INT32 *pfgSet);
static INT32 _MT5135_GPIO_Output(INT32 i4GpioNum, INT32 *pfgSet);
static INT32 _MT5135_GPIO_Input(INT32 i4GpioNum);
static INT32 _MT5135_GPIO_Intrq(INT32 i4GpioNum, INT32 *pfgSet);
static INT32 _MT5135_GPIO_Init(void);
static INT32 _MT5135_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise);
#endif
#if 0
static INT32 _WT6720_Write(UINT8 ucRegAddr, UINT8 value);
static INT32 _WT6720_Read(UINT8 ucRegAddr);
static INT32 _WT6720_GPIO_Range_Check(INT32 i4GpioNum);
static INT32 _WT6720_GPIO_Enable(INT32 i4GpioNum, INT32 *pfgSet);
static INT32 _WT6720_GPIO_Output(INT32 i4GpioNum, INT32 *pfgSet);
static INT32 _WT6720_GPIO_Input(INT32 i4GpioNum);
static INT32 _WT6720_GPIO_Intrq(INT32 i4GpioNum, INT32 *pfgSet);
static INT32 _WT6720_GPIO_Init(void);
static INT32 _WT6720_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise);
#endif
//-----------------------------------------------------------------------------
// Imported functions
//----------------------------------------------------------------------------- 
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
#ifndef CC_MTK_LOADER
#if GPIO_THREAD_ENABLE
static HANDLE_T _hGPIOPollThread;
static volatile INT32 _i4GPIOExist = 0;
static volatile INT32 _i4GPIOLoop = 0;
#endif /* GPIO_THREAD_ENABLE */


static volatile PFN_GPIO_CALLBACK _afnGpioCallback[NORMAL_GPIO_NUM];//NORMAL_GPIO_NUM for SW gpio polling thread
#endif /* CC_MTK_LOADER */



#define GPIO_POLLFALL_REG(gpio) \
    (_au4GpioPollFall[GPIO_TO_INDEX(gpio)] & (1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_POLLRISE_REG(gpio) \
    (_au4GpioPollRise[GPIO_TO_INDEX(gpio)] & (1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_POLLFALL_SET(gpio) \
    (_au4GpioPollFall[GPIO_TO_INDEX(gpio)] |= (1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_POLLFALL_CLR(gpio) \
    (_au4GpioPollFall[GPIO_TO_INDEX(gpio)] &= ~(1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_POLLRISE_SET(gpio) \
    (_au4GpioPollRise[GPIO_TO_INDEX(gpio)] |= (1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_POLLRISE_CLR(gpio) \
    (_au4GpioPollRise[GPIO_TO_INDEX(gpio)] &= ~(1U << (gpio & GPIO_INDEX_MASK)))


#define GPIO_INTR_RISE_REG(gpio)    \
    (GPIO_RISE_REG(gpio) & (1U << (gpio & EDGE_INDEX_MASK)))
#define GPIO_INTR_REG(gpio)     \
    (GPIO_EXTINTEN_REG(gpio) & (1U << (gpio & GPIO_INDEX_MASK)))
#define GPIO_INTR_FALL_REG(gpio)    \
    (GPIO_FALL_REG(gpio) & (1U << (gpio & EDGE_INDEX_MASK)))
#define GPIO_INTR_CLR(gpio)     \
    vIO32Write4B(REG_GPIO_EXTINTEN + (GPIO_TO_INDEX(gpio) << 2) , GPIO_EXTINTEN_REG(gpio)  & (~(1U << (gpio & EDGE_INDEX_MASK))))
#define GPIO_INTR_FALL_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_ED2INTEN_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_LEVINTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_LEVINTEN_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_ENTPOL + (GPIO_TO_INDEX(gpio) << 2), GPIO_ENTPOL_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_EXTINTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_EXTINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));

#define GPIO_INTR_RISE_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_ED2INTEN_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_LEVINTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_LEVINTEN_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_ENTPOL + (GPIO_TO_INDEX(gpio) << 2), GPIO_ENTPOL_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_EXTINTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_EXTINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));
    
#define GPIO_INTR_BOTH_EDGE_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_ED2INTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_LEVINTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_LEVINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_ENTPOL + (GPIO_TO_INDEX(gpio) << 2), GPIO_ENTPOL_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_EXTINTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_EXTINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));



#define GPIO_INTR_LEVEL_LOW_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_ED2INTEN_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_LEVINTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_LEVINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK))); \
    vIO32Write4B(REG_GPIO_ENTPOL + (GPIO_TO_INDEX(gpio) << 2), GPIO_ENTPOL_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_EXTINTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_EXTINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));

#define GPIO_INTR_LEVEL_HIGH_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_ED2INTEN_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_LEVINTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_LEVINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK))); \
    vIO32Write4B(REG_GPIO_ENTPOL + (GPIO_TO_INDEX(gpio) << 2), GPIO_ENTPOL_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_EXTINTEN + (GPIO_TO_INDEX(gpio) << 2), GPIO_EXTINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));

#define IS_CKGEN_GPIO_LEVEL_INT(gpio) (GPIO_LEVINTEN_REG(gpio) & (1U << (gpio & EDGE_INDEX_MASK)))? 1 : 0
#define IS_CKGEN_GPIO_BOTH_EDGE_INT(gpio)  (GPIO_ED2INTEN_REG(gpio) & (1U << (gpio & EDGE_INDEX_MASK)))? 1 : 0

static const GPIO_HANDLER_FUNC_TBL gGPIO_HANDLER_FuncTbl[] = 
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
        _CKGEN_GPIO_VotageLevel,
#if defined(CC_FAST_INIT)
        NULL,
        _CKGEN_GPIO_Resume,
        _CKGEN_GPIO_ResumeINTEN,
#endif
#ifdef __KERNEL__
        _CKGEN_GPIO_SetIntExtParam,
        _CKGEN_GPIO_GetIntExtParam,
#else
        NULL,
        NULL,
#endif
    },

    {
        PDWNC_GpioRangeCheck,
        /*_PDWNC_InitGpio, _CKGEN_GPIO_Init->PDWNC_Init->_PDWNC_InitGpio*/NULL,
        PDWNC_GpioEnable,
        PDWNC_GpioInput,
        PDWNC_GpioIntrq,
        PDWNC_GpioOutput,
        PDWNC_GpioQuery,
        #if 1
		PDWNC_GpioReg,
		#else	//test gpio int to t8032
        PDWNC_T8032GpioReg,	
        #endif
        _PDWNC_RegLedBlinker,
        NULL,
        NULL,
#if defined(CC_FAST_INIT)
        NULL,//        PDWNC_GPIO_Suspend,
        NULL,//_CKGEN_GPIO_Resume->PDWNC_pm_resume->PDWNC_GPIO_Resume
        PDWNC_GpioResumeINTEN,
#endif
#ifdef __KERNEL__
        PDWNC_GpioSetIntExtParam,
        PDWNC_GpioGetIntExtParam,
#else
        NULL,
        NULL,
#endif
    },

    {
        PDWNC_ServoGpioRangeCheck,
#if 1//defined(CC_MT5365) || defined(CC_MT5395) //change after 5365/95/...
            NULL,
            PDWNC_GpioEnable,
            PDWNC_GpioInput,
            PDWNC_GpioIntrq,
            PDWNC_GpioOutput,
            PDWNC_GpioQuery,
            PDWNC_GpioReg,
            _PDWNC_RegLedBlinker,
            NULL,
            NULL,
#if defined(CC_FAST_INIT)
            NULL,
            NULL,//_CKGEN_GPIO_Resume->PDWNC_pm_resume->PDWNC_GPIO_Resume
            PDWNC_GpioResumeINTEN,
#endif
#ifdef __KERNEL__
        PDWNC_GpioSetIntExtParam,
        PDWNC_GpioGetIntExtParam,
#else
        	NULL,
        	NULL,
#endif        
#else
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
#if defined(CC_FAST_INIT)
            NULL,
            NULL,
            NULL,            
#endif
        NULL,
        NULL,
#endif        
    }
#ifndef CC_MTK_LOADER
#if 0//for MT5135 and MT8295
    ,
    {
        CI_GPIO_Range_Check,
        CI_GPIO_Init,
        CI_GPIO_Enable,
        CI_GPIO_Input,
        CI_GPIO_Intrq,
        CI_GPIO_Output,
        CI_GPIO_Query,
        CI_GPIO_Reg,
        NULL,
        NULL
    }
#endif
#endif

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
#if 1//defined(CC_MT5365) || defined(CC_MT5395) //change after 5365/95/...
                 return (i4GpioNum - SERVO_0_ALIAS) + (SERVO_GPIO0 - OPCTRL(0))  ;
#else
                return i4GpioNum - SERVO_GPIO0;
#endif
#if 0
#ifndef CC_MTK_LOADER  
            case GPIO_HANDLER_CI:
                return i4GpioNum - MT5135_GPIO0;            
#endif				
     #endif           
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
    UINT32 u4Val,u4Val1;
    INT32 i4Idx,i4Val;
#ifndef CC_MTK_LOADER
        CRIT_STATE_T rCrit;
#else /* CC_MTK_LOADER */
        //UINT32 u4State;
#endif
    i4Idx = ((UINT32)i4GpioNum >> 5);

#ifndef CC_MTK_LOADER
        rCrit = x_crit_start();
#endif /* CC_MTK_LOADER */    

    VERIFY(BSP_PinGpioSel(i4GpioNum)==0);

    if (pfgSet == NULL)	//NULL: for query gpio status, must be GPIO output , but not change pin level
    {
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */     
		if(_CKGEN_GPIO_Enable(i4GpioNum, NULL) == 0)//old is input state, change to be output
		{
			//get pin level
			u4Val1 = GPIO_IN_REG(i4Idx);
			//get current out register setting
			u4Val = GPIO_OUT_REG(i4Idx);
			u4Val = (u4Val1 & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ?
	            (u4Val | (1U << (i4GpioNum & GPIO_INDEX_MASK))) :
	            (u4Val & ~(1U << (i4GpioNum & GPIO_INDEX_MASK)));
			GPIO_OUT_WRITE(i4Idx, u4Val);
			// Set the output mode.
			i4Val = 1;
			_CKGEN_GPIO_Enable(i4GpioNum, &i4Val);
		}
		u4Val = GPIO_OUT_REG(i4Idx);
        return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1 : 0);
    }
    
    u4Val = GPIO_OUT_REG(i4Idx);
    u4Val = (*pfgSet) ?
            (u4Val | (1U << (i4GpioNum & GPIO_INDEX_MASK))) :
            (u4Val & ~(1U << (i4GpioNum & GPIO_INDEX_MASK)));
    
    GPIO_OUT_WRITE(i4Idx, u4Val);
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */     
    // Set the output mode.
    i4Val = 1;
    UNUSED(_CKGEN_GPIO_Enable(i4GpioNum, &i4Val));
    
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
    i4GpioNum = _GPIO2BitOffsetOfIntReg[i4GpioNum];
    if(i4GpioNum == -1)
        return -1;
    u4Val = GPIO_EXTINTEN_REG(i4GpioNum);
    if (pfgSet == NULL)
    {
        return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1 : 0);
    }
    
    u4Val = (*pfgSet) ?
            (u4Val | (1U << (i4GpioNum & GPIO_INDEX_MASK))) :
            (u4Val & ~(1U << (i4GpioNum & GPIO_INDEX_MASK)));
    vIO32Write4B(REG_GPIO_EXTINTEN + (GPIO_TO_INDEX(i4GpioNum)<< 2), u4Val);

    return (*pfgSet);

}
#if defined(CC_FAST_INIT)
static INT32 _CKGEN_GPIO_Resume(void)
{
    INT32 i4Ret;    
    INT32 i4Idx;
        
#ifndef CC_MTK_LOADER
    // disable all gpio interrupt 
    for (i4Idx = 0; i4Idx < GPIO_TO_INDEX(GPIO_SUPPORT_INT_NUM); i4Idx++)
    {
	    vIO32Write4B(REG_GPIO_EXTINTEN + (i4Idx << 2), 0);
	}

    for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
    {
        _au4GpioPrevPoll[i4Idx] = 0;
        _au4GpioPollFall[i4Idx] = 0;
        _au4GpioPollRise[i4Idx] = 0;
    }
#endif /* CC_MTK_LOADER */

    // clear all edge triggered interrupt status 0.
    for (i4Idx = 0; i4Idx < GPIO_TO_INDEX(GPIO_SUPPORT_INT_NUM); i4Idx++)
    {
        vIO32Write4B(REG_GPIO_EXTINT + (i4Idx << 2), 0);
    }
    i4Ret = 0;
    BIM_EnableIrq(VECTOR_EXT1);
    PDWNC_pm_resume();//why we put PDWNC_Init() here...?
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

    return 0;    
}
#endif

INT32 GPIO_SetIntExtParam(INT32 i4GpioNum, UINT32 u4Arg)
{
    UINT32 u4Val;

        for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
        {
            if(gGPIO_HANDLER_FuncTbl[u4Val].pfnIsOwner(i4GpioNum))
            {
//                VERIFY(BSP_PinGpioSel(i4GpioNum)==0);                        
                if(gGPIO_HANDLER_FuncTbl[u4Val].pfnSetIntExtParam != NULL)
                    return  gGPIO_HANDLER_FuncTbl[u4Val].pfnSetIntExtParam(_GPIO_OwnerNumOffset(u4Val, i4GpioNum), u4Arg);
//                else
//                    Printf( "The SetIntExtParam function of this module is NULL\n");                     
            }       
        }
        return -1;      
}

INT32 GPIO_GetIntExtParam(INT32 i4GpioNum, UINT32 *pu4Arg)
{
    UINT32 u4Val;

        for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
        {
            if(gGPIO_HANDLER_FuncTbl[u4Val].pfnIsOwner(i4GpioNum))
            {
//                VERIFY(BSP_PinGpioSel(i4GpioNum)==0);                        
                if(gGPIO_HANDLER_FuncTbl[u4Val].pfnGetIntExtParam != NULL)
                    return  gGPIO_HANDLER_FuncTbl[u4Val].pfnGetIntExtParam(_GPIO_OwnerNumOffset(u4Val, i4GpioNum), pu4Arg);
//                else
//                    Printf( "The GetIntExtParam function of this module is NULL\n");                     
            }       
        }
        return -1;          
}

/*****************************************************************/
#ifdef __KERNEL__ 
static UINT32 _au4CkgenGpioCallBackArg[NORMAL_GPIO_NUM];
static UINT8 _au4CkgenGpioCallBackFromLinuxAP[NORMAL_GPIO_NUM];
static INT32 _CKGEN_GPIO_SetIntExtParam(INT32 i4Gpio, UINT32 u4Arg)
{
    _au4CkgenGpioCallBackFromLinuxAP[i4Gpio] = 1;
    if(_GPIO2BitOffsetOfIntReg[i4Gpio] != -1)
    {
        _au4CkgenGpioCallBackArg[i4Gpio] = u4Arg;
        return 0;
    }
    else
    {
        return -1;        
    }
}

static INT32 _CKGEN_GPIO_GetIntExtParam(INT32 i4Gpio, UINT32 *pu4Arg)
{
    if(_GPIO2BitOffsetOfIntReg[i4Gpio] != -1)
    {
        *pu4Arg = _au4CkgenGpioCallBackArg[i4Gpio];
        return 0;
    }
    else
    {
        return -1;        
    }	
}
#endif
/***************************************************************/

static INT32 _CKGEN_GPIO_Init()
{
    
//    UINT32 u4Val;
    INT32 i4Ret;    
    INT32 i4Idx;
#ifndef CC_MTK_LOADER    
    void (* pfnOldIsr)(UINT16);
#endif /* CC_MTK_LOADER */

        
#ifndef CC_MTK_LOADER
    for (i4Idx = 0; i4Idx < NORMAL_GPIO_NUM; i4Idx++)
    {
        _afnGpioCallback[i4Idx] = NULL;
#ifdef __KERNEL__         
        _au4CkgenGpioCallBackFromLinuxAP[i4Idx] = 0;
        _au4CkgenGpioCallBackArg[i4Idx] = 0;
#endif        
    }
    // disable all gpio interrupt 
    for (i4Idx = 0; i4Idx < GPIO_TO_INDEX(GPIO_SUPPORT_INT_NUM); i4Idx++)
    {
	    vIO32Write4B(REG_GPIO_EXTINTEN + (i4Idx << 2), 0);
	}
    
    for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
    {
        _au4GpioPrevPoll[i4Idx] = 0;
        _au4GpioPollFall[i4Idx] = 0;
        _au4GpioPollRise[i4Idx] = 0;
    }
#endif /* CC_MTK_LOADER */

    // clear all edge triggered interrupt status 0.
    for (i4Idx = 0; i4Idx < GPIO_TO_INDEX(GPIO_SUPPORT_INT_NUM); i4Idx++)
    {
        vIO32Write4B(REG_GPIO_EXTINT + (i4Idx << 2), 0);
    }
    i4Ret = 0;
#ifndef CC_MTK_LOADER
#ifndef CC_UP8032_ATV
//    i4Ret = x_reg_isr(VECTOR_EXT2, _GPIO_BothEdgeIsr, &pfnOldIsr);
//    ASSERT(i4Ret == OSR_OK);
    i4Ret = x_reg_isr(VECTOR_EXT1, _GPIO_DedicatedIsr, &pfnOldIsr);
    ASSERT(i4Ret == OSR_OK);
    UNUSED(i4Ret);
#endif 

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
	//Printf("x_thread_created: %d\n",i4Ret);
        ASSERT(i4Ret==OSR_OK);
    }
#endif /* GPIO_THREAD_ENABLE */
#endif /* CC_MTK_LOADER */

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

#ifdef CC_MTK_LOADER
static INT32 _CKGEN_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
    return -1;   
}
static INT32 _CKGEN_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
    return -1;   
}

#else
//david: may need to add level trigger information for this query function
static INT32 _CKGEN_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
    if (pi4Intr)
    {
        *pi4Intr = (INT32)(GPIO_INTR_REG((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]) ? 1 : 0);
    }
    if (pi4Fall)
    {
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] != -1)
        {
            *pi4Fall = (INT32)(GPIO_INTR_FALL_REG((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]) ? 1 : 0);
        }
        else
        {
#if GPIO_THREAD_ENABLE
            *pi4Fall = (INT32)(GPIO_POLLFALL_REG((UINT32)i4Gpio) ? 1 : 0);
#endif /* GPIO_THREAD_ENABLE */
        }
    }
    if (pi4Rise)
    {
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] != -1)
        {
            *pi4Rise = (INT32)(GPIO_INTR_RISE_REG((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]) ? 1 : 0);
        }
        else
        {
#if GPIO_THREAD_ENABLE
            *pi4Rise = (INT32)(GPIO_POLLRISE_REG((UINT32)i4Gpio) ? 1 : 0);
#endif /* GPIO_THREAD_ENABLE */
        }
    }

    return 0;
}


static INT32 _CKGEN_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
#ifdef CC_LEVEL_INT_TRIG_ONCE
    INT32 i4Mode;
#endif    
//remove warning for klocwork checking
     if ((i4Gpio < 0) || (i4Gpio >= NORMAL_GPIO_NUM))
    {
        return -1;
    } 
	 
#ifndef CC_FAST_INIT
    if((eType != GPIO_TYPE_NONE) && (pfnCallback == NULL))
    {
        return -1;        
    }
#endif

    VERIFY(BSP_PinGpioSel(i4Gpio)==0);

    _au4GpioPrevPoll[GPIO_TO_INDEX(i4Gpio)] = _GPIO_POLL_IN_REG(GPIO_TO_INDEX(i4Gpio));

    /* Set the register and callback function. */
    LOG(7,"_CKGEN_GPIO_Reg gpio: %d\n", i4Gpio);
    switch(eType)
    {
    case GPIO_TYPE_INTR_FALL:
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] == -1)
        {
            GPIO_POLLFALL_SET((UINT32)i4Gpio);
        }
        else
        {
            LOG(7,"_CKGEN_GPIO_Reg type: GPIO_TYPE_INTR_FALL\n");            
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
            LOG(7,"_CKGEN_GPIO_Reg type: GPIO_TYPE_INTR_RISE\n");            
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
            LOG(7,"_CKGEN_GPIO_Reg type: GPIO_TYPE_INTR_BOTH\n");            
            GPIO_INTR_BOTH_EDGE_SET((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]);
        }
        break;
    case GPIO_TYPE_INTR_LEVEL_LOW:
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] != -1)
        {
            LOG(7,"_CKGEN_GPIO_Reg type: GPIO_TYPE_INTR_LEVEL_LOW\n");
#ifdef CC_LEVEL_INT_TRIG_ONCE
            i4Mode = 0;
            UNUSED(_CKGEN_GPIO_Enable(i4Gpio, &i4Mode));
            if ( _CKGEN_GPIO_Input(i4Gpio) == 0 )
            {
                    #ifdef __KERNEL__                    
                    if(!_au4CkgenGpioCallBackFromLinuxAP[i4Gpio])
                    {
                        pfnCallback(i4Gpio, 0);
                    }
                    else
                    {
                        MTGPIO_CB_T rGpioCb;
                        rGpioCb.i4Gpio = i4Gpio;
                        rGpioCb.fgStatus = 0;
                        rGpioCb.u4Pfn = pfnCallback;
                        rGpioCb.u4Arg = _au4CkgenGpioCallBackArg[i4Gpio];
                        LOG(7," gpio interrupt _CB_PutEvent: gpio_%d, level=%d\n",rGpioCb.i4Gpio, rGpioCb.fgStatus );
                        _CB_PutEvent(CB_MTAL_GPIO_TRIGGER, sizeof(MTGPIO_CB_T), &rGpioCb);
                    }
                    #else
                    pfnCallback(i4Gpio, 0);
                    #endif
            }
            else
            {
            GPIO_INTR_LEVEL_LOW_SET((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]);
        }
#else            
            GPIO_INTR_LEVEL_LOW_SET((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]);
#endif            
        }
        break;
    case GPIO_TYPE_INTR_LEVEL_HIGH:
        if(_GPIO2BitOffsetOfIntReg[i4Gpio] != -1)
        {
            LOG(7,"_CKGEN_GPIO_Reg type: GPIO_TYPE_INTR_LEVEL_HIGH\n");
#ifdef CC_LEVEL_INT_TRIG_ONCE
            i4Mode = 0;
            UNUSED(_CKGEN_GPIO_Enable(i4Gpio, &i4Mode));
            if ( _CKGEN_GPIO_Input(i4Gpio) == 1 )
            {
                    #ifdef __KERNEL__                    
                    if(!_au4CkgenGpioCallBackFromLinuxAP[i4Gpio])
                    {
                        pfnCallback(i4Gpio, 1);
                    }
                    else
                    {
                        MTGPIO_CB_T rGpioCb;
                        rGpioCb.i4Gpio = i4Gpio;
                        rGpioCb.fgStatus = 1;
                        rGpioCb.u4Pfn = pfnCallback;
                        rGpioCb.u4Arg = _au4CkgenGpioCallBackArg[i4Gpio];
                        LOG(7," gpio interrupt _CB_PutEvent: gpio_%d, level=%d\n",rGpioCb.i4Gpio, rGpioCb.fgStatus );
                        _CB_PutEvent(CB_MTAL_GPIO_TRIGGER, sizeof(MTGPIO_CB_T), &rGpioCb);
                    }
                    #else
                    pfnCallback(i4Gpio, 1);
                    #endif
            }
            else
            {
            GPIO_INTR_LEVEL_HIGH_SET((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]);
        }
#else            
            GPIO_INTR_LEVEL_HIGH_SET((UINT32)_GPIO2BitOffsetOfIntReg[i4Gpio]);
#endif            
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
        LOG(7,"_CKGEN_GPIO_Reg type: GPIO_TYPE_NONE\n");
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
#endif
static INT32 _CKGEN_GPIO_Stop()
{
    INT32 i4Idx;
#ifndef CC_MTK_LOADER
#if GPIO_THREAD_ENABLE
    _i4GPIOLoop = 0;
    while (!_i4GPIOLoop)
    {
        x_thread_delay(100);
    }
#endif /* GPIO_THREAD_ENABLE */
#endif   //CC_MTK_LOADER

	for (i4Idx = 0; i4Idx < GPIO_TO_INDEX(GPIO_SUPPORT_INT_NUM); i4Idx++)
	{
		vIO32Write4B(REG_GPIO_EXTINTEN + (i4Idx << 2), 0);
	}
	
#ifndef CC_UP8032_ATV
    VERIFY(BIM_DisableIrq(VECTOR_EXT1));
#endif
//    VERIFY(BIM_DisableIrq(VECTOR_EXT2));
    return 0;
}
#if defined(CC_FAST_INIT)
static INT32 _CKGEN_GPIO_ResumeINTEN(INT32 i4Gpio, GPIO_TYPE eType)
{
//note: At resume stage, pass NULL will not overwrite _afnGpioCallback[] which setup at initial stage    
    return _CKGEN_GPIO_Reg(i4Gpio, eType, NULL);
}
#endif
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
    UNUSED(GPIO_Enable(i4GpioNum, &i4Mode));

    return GPIO_Input(i4GpioNum);
}

//-----------------------------------------------------------------------------
// GPIO Interface functions.
//-----------------------------------------------------------------------------


#ifndef CC_MTK_LOADER

 #if GPIO_THREAD_ENABLE
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
 #endif

#if 0

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
#endif //0
#ifdef __KERNEL__  

extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

#endif

//#define GPIO_VERYFY
static void _GPIO_DedicatedIsr(UINT16 u2VctId)
{
    INT32 i4Idx;
    UINT32 u4Status,u4Status2;
    UINT32 u4Val;
    INT32 i4GpioNum;

    ASSERT(u2VctId == VECTOR_EXT1);
#ifndef CC_UP8032_ATV
    VERIFY(BIM_ClearIrq((UINT32)u2VctId));
#endif
    // get edge triggered interrupt status.       
    u4Status = u4IO32Read4B(REG_GPIO_EXTINT);
	u4Status2 = u4IO32Read4B(REG_GPIO_EXTINT + 4);

#ifdef GPIO_VERYFY
		for(i4Idx=0;i4Idx<37;i4Idx+=12)
		{
			LOG(2,"0x%08X | %08X \n",CKGEN_ED2INTEN0+i4Idx,u4IO32Read4B(CKGEN_ED2INTEN0 + i4Idx));
			LOG(2,"0x%08X | %08X \n",CKGEN_ED2INTEN0+i4Idx+4,u4IO32Read4B(CKGEN_ED2INTEN0 + i4Idx + 4));
		}
#endif

    // handle edge triggered interrupt.
    for (i4Idx = 0; i4Idx < GPIO_SUPPORT_INT_NUM; i4Idx++)
    {
    	if(i4Idx < 32)
    	{
        u4Val = u4Status & (1 << i4Idx);
        if(u4Val)
        {
            i4GpioNum = _BitOffsetOfIntReg2GPIO[i4Idx];
            if ((i4GpioNum < NORMAL_GPIO_NUM) && (_afnGpioCallback[i4GpioNum] != NULL))
            { 
#ifdef CC_LEVEL_INT_TRIG_ONCE                    
                    if((IS_CKGEN_GPIO_LEVEL_INT(i4GpioNum)) && (!(IS_CKGEN_GPIO_BOTH_EDGE_INT(i4GpioNum))))
                    {
                        INT32 i4Intr;
                    
                        // disable for level type ISR
                        i4Intr = 0;
                        _CKGEN_GPIO_Intrq( i4GpioNum, &i4Intr);
                        LOG(7,"Disable level interrupt (just trigger once)");
                    }
#endif                    
#ifdef __KERNEL__                    
                    if(!_au4CkgenGpioCallBackFromLinuxAP[i4GpioNum])
                    {
                        _afnGpioCallback[i4GpioNum](i4GpioNum, _GPIO_POLL_IN_REG(GPIO_TO_INDEX(i4GpioNum)) & (1U << (i4GpioNum & GPIO_INDEX_MASK))? 1:0);
                    }
                    else
                    {
                        MTGPIO_CB_T rGpioCb;
                        rGpioCb.i4Gpio = i4GpioNum;
                        rGpioCb.fgStatus = _GPIO_POLL_IN_REG(GPIO_TO_INDEX(i4GpioNum)) & (1U << (i4GpioNum & GPIO_INDEX_MASK))? 1:0;
                        rGpioCb.u4Pfn = (UINT32)_afnGpioCallback[i4GpioNum];
                        rGpioCb.u4Arg = _au4CkgenGpioCallBackArg[i4GpioNum];
                        LOG(7," gpio interrupt _CB_PutEvent: gpio_%d, level=%d\n",rGpioCb.i4Gpio, rGpioCb.fgStatus );
                        _CB_PutEvent(CB_MTAL_GPIO_TRIGGER, sizeof(MTGPIO_CB_T), &rGpioCb);
                    }
#else
                _afnGpioCallback[i4GpioNum](i4GpioNum, _GPIO_POLL_IN_REG(GPIO_TO_INDEX(i4GpioNum)) & (1U << (i4GpioNum & GPIO_INDEX_MASK))? 1:0);
#endif                            
            }   
        }
    }   
		else
		{
			u4Val = u4Status2 & (1 << (i4Idx - 32));
	        if(u4Val)
	        {
	            i4GpioNum = _BitOffsetOfIntReg2GPIO[i4Idx];
	            if ((i4GpioNum < NORMAL_GPIO_NUM) && (_afnGpioCallback[i4GpioNum] != NULL))
	            { 
#ifdef CC_LEVEL_INT_TRIG_ONCE                    
                    if((IS_CKGEN_GPIO_LEVEL_INT(i4GpioNum)) && (!(IS_CKGEN_GPIO_BOTH_EDGE_INT(i4GpioNum))))
                    {
                        INT32 i4Intr;
                    
                        // disable for level type ISR
                        i4Intr = 0;
                        _CKGEN_GPIO_Intrq( i4GpioNum, &i4Intr);
                        LOG(9, "Disable level interrupt (just trigger once)");
                    }
#endif                    
#ifdef __KERNEL__                    
                    if(!_au4CkgenGpioCallBackFromLinuxAP[i4GpioNum])
                    {
	                _afnGpioCallback[i4GpioNum](i4GpioNum, _GPIO_POLL_IN_REG(GPIO_TO_INDEX(i4GpioNum)) & (1U << (i4GpioNum & GPIO_INDEX_MASK))? 1:0);
	            }   
                    else
                    {
                        MTGPIO_CB_T rGpioCb;
                        rGpioCb.i4Gpio = i4GpioNum;
                        rGpioCb.fgStatus = _GPIO_POLL_IN_REG(GPIO_TO_INDEX(i4GpioNum)) & (1U << (i4GpioNum & GPIO_INDEX_MASK))? 1:0;
                        rGpioCb.u4Pfn = (UINT32)_afnGpioCallback[i4GpioNum];
                        rGpioCb.u4Arg = _au4CkgenGpioCallBackArg[i4GpioNum];
                
                        _CB_PutEvent(CB_MTAL_GPIO_TRIGGER, sizeof(MTGPIO_CB_T), &rGpioCb);
                    }
#else
                    _afnGpioCallback[i4GpioNum](i4GpioNum, _GPIO_POLL_IN_REG(GPIO_TO_INDEX(i4GpioNum)) & (1U << (i4GpioNum & GPIO_INDEX_MASK))? 1:0);
#endif                             
	            }   
	        }
		}
    }   
         // clear edge triggered interrupt status.       
    vIO32Write4B(REG_GPIO_EXTINT, 0);
	vIO32Write4B(REG_GPIO_EXTINT+4, 0);	
}



#if GPIO_THREAD_ENABLE
static void _GPIO_PollThread(void *pvDummy)
{
    INT32 i4Idx;//, i4Bit, i4Gpio;
    UINT32 u4Mask;

    UNUSED(pvDummy);
//	UNUSED(_GPIO_PollThread);
    for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
    {
        _au4GpioPrevPoll[i4Idx] = _GPIO_POLL_IN_REG(i4Idx);
    }

    _i4GPIOLoop = 1;
    _i4GPIOExist = 1;
    while (_i4GPIOLoop)
    {
        x_thread_delay(200);
	//Printf("GPIO  PollThread\n");

        for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
        {
            /* Check falling 1->0 */
            u4Mask = _au4GpioPrevPoll[i4Idx] &
                        ~(_GPIO_POLL_IN_REG(i4Idx)) & _au4GpioPollFall[i4Idx];

            if (u4Mask)
            {
                _GPIO_CheckCallback(u4Mask, i4Idx, NORMAL_GPIO_NUM, 0);
  			//Printf("GPIO  FALLING\n");
            }

            /* Check Rising. 0->1 */
            u4Mask = ~(_au4GpioPrevPoll[i4Idx]) &
                        _GPIO_POLL_IN_REG(i4Idx) & _au4GpioPollRise[i4Idx];

            if (u4Mask)
            {
                _GPIO_CheckCallback(u4Mask, i4Idx, NORMAL_GPIO_NUM, 1);
			//Printf("GPIO  RISING\n");
            }

            /* Assign Previous value. */
            _au4GpioPrevPoll[i4Idx] = _GPIO_POLL_IN_REG(i4Idx);
        }
    }
    _i4GPIOLoop = 1;
    _i4GPIOExist = 0;
}
#endif /* GPIO_THREAD_ENABLE */
#endif /* CC_MTK_LOADER */
#if defined(CC_FAST_INIT)
INT32 GPIO_pm_resume()
{
    UINT32 u4Val;
        
    if (_u4GPIOInit)
    {
        Printf("Warning!! Gpio driver has been initialed...\n");
        return 0;
    }
    _u4GPIOInit = 1;
    for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
    {
        if(gGPIO_HANDLER_FuncTbl[u4Val].pfnResume != NULL)
            gGPIO_HANDLER_FuncTbl[u4Val].pfnResume();
    }   
    return 0;

}

INT32 GPIO_pm_suspend()
{
    UINT32 u4Val;
        
    for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
    {
        if(gGPIO_HANDLER_FuncTbl[u4Val].pfnSuspend != NULL)
            gGPIO_HANDLER_FuncTbl[u4Val].pfnSuspend();
    }   
    _u4GPIOInit = 0;
    return 0;
}
#endif

INT32 GPIO_Init()
{
    UINT32 u4Val;
        
        if (_u4GPIOInit)
    {
        return 0;
    }
#ifndef YAJUN
    _u4GPIOInit = 1;
#endif
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
#if defined(CC_FAST_INIT)
INT32 GPIO_INTEN(INT32 i4Gpio, GPIO_TYPE eType)
{
   UINT32 u4Val;

        for(u4Val = 0; u4Val < (sizeof(gGPIO_HANDLER_FuncTbl)/sizeof(GPIO_HANDLER_FUNC_TBL)); u4Val++)
        {
            if(gGPIO_HANDLER_FuncTbl[u4Val].pfnIsOwner(i4Gpio))
            {
//                  VERIFY(BSP_PinGpioSel(i4Gpio)==0);
                if(gGPIO_HANDLER_FuncTbl[u4Val].pfnReg != NULL)
                {
                    return  gGPIO_HANDLER_FuncTbl[u4Val].pfnResumeIntEn(_GPIO_OwnerNumOffset(u4Val, i4Gpio),eType);
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

#endif
#endif /* CC_MTK_LOADER */

