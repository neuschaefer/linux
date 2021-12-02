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

#ifndef CC_MTK_LOADER
#include "pcmcia_if.h"
#ifdef __KERNEL__
#include "cb_data.h"
#include "mtperipheral.h"
#endif
#endif

#include "c_model.h"

#endif

//---------------------------------------------------------------------------
// Types
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
// #define DEFINE_MT8292_INTERRUPT
#ifdef CC_WT6720
static UINT32 _gu4WT6720_INIT = 0;
#endif
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define GPIO_INT_INDEX_MAX 6

//-----------------------------------------------------------------------------
// Prototypes
//----------------------------------------------------------------------------- 
// static INT32 _MT8292_Gpio_Init(void);
// static INT32 _MT8292_Gpio_Enable(INT32 i4GpioNum, const INT32 * pi4Polarity/*0:input, 1:output, null:query*/);
// static INT32 _MT8292_Gpio_Input(INT32 i4GpioNum);
// static INT32 _MT8292_Gpio_Output(INT32 i4GpioNum, const INT32 *pfgSet);
//#endif
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
#ifdef __KERNEL__
static INT32 _CKGEN_GPIO_SetIntExtParam(INT32 i4Gpio, UINT32 u4Arg);
static INT32 _CKGEN_GPIO_GetIntExtParam(INT32 i4Gpio, UINT32 *pu4Arg);
#endif
#ifndef CC_MTK_LOADER
//static void _GPIO_BothEdgeIsr(UINT16 u2VctId);
static void _GPIO_DedicatedIsr(UINT16 u2VctId);

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
#ifdef CC_WT6720
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
#ifdef CC_WT6720
#ifndef CC_MTK_LOADER
static HANDLE_T _hWT6720Mutex = NULL;
#endif /* CC_MTK_LOADER */
#endif
#if 1
static volatile UINT32 _au4GpioPrevPoll[TOTAL_GPIO_IDX];
static volatile UINT32 _au4GpioPollFall[TOTAL_GPIO_IDX];
static volatile UINT32 _au4GpioPollRise[TOTAL_GPIO_IDX];
#endif /* CC_MTK_LOADER */
#ifndef CC_MTK_LOADER

static volatile PFN_GPIO_CALLBACK _afnGpioCallback[NORMAL_GPIO_NUM];//NORMAL_GPIO_NUM for SW gpio polling thread
#endif /* CC_MTK_LOADER */

#define GPIO_INTR_RISE_REG(gpio)    \
	(GPIO_RISE_REG(gpio) & (1U << (gpio & EDGE_INDEX_MASK)))
#define GPIO_INTR_RISE3_REG(gpio)    \
	(GPIO_RISE3_REG(gpio) & (1U << (gpio & EDGE_INDEX_MASK)))
	
#define GPIO_INTR_REG(gpio)     \
	(GPIO_EXTINTEN_REG(gpio) & (1U << (gpio & GPIO_INDEX_MASK)))    
#define GPIO_INTR3_REG(gpio)     \
	(GPIO_EXTINTEN3_REG(gpio) & (1U << (gpio & GPIO_INDEX_MASK)))

#define GPIO_INTR_FALL_REG(gpio)    \
	(GPIO_FALL_REG(gpio) & (1U << (gpio & EDGE_INDEX_MASK)))    
#define GPIO_INTR_FALL3_REG(gpio)    \
	(GPIO_FALL3_REG(gpio) & (1U << (gpio & EDGE_INDEX_MASK)))
    
#define GPIO_INTR_CLR(gpio) vIO32Write4B(REG_GPIO_EXTINTEN0 + (GPIO_TO_INDEX(gpio) << 2) , GPIO_EXTINTEN_REG(gpio)  & (~(1U << (gpio & EDGE_INDEX_MASK))))
#define GPIO_INTR_CLR3(gpio) vIO32Write4B(REG_GPIO_EXTINTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2) , GPIO_EXTINTEN3_REG(gpio)  & (~(1U << (gpio & EDGE_INDEX_MASK))))

#define GPIO_INTR_FALL_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_ED2INTEN_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_LEVINTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_LEVINTEN_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_ENTPOL0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_ENTPOL_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_EXTINTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_EXTINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)))
#define GPIO_INTR_FALL3_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_ED2INTEN3_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_LEVINTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_LEVINTEN3_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_ENTPOL3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_ENTPOL3_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_EXTINTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_EXTINTEN3_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)))

#define GPIO_INTR_RISE_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_ED2INTEN_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_LEVINTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_LEVINTEN_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_ENTPOL0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_ENTPOL_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_EXTINTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_EXTINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)))
#define GPIO_INTR_RISE3_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_ED2INTEN3_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_LEVINTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_LEVINTEN3_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_ENTPOL3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_ENTPOL3_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_EXTINTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_EXTINTEN3_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)))
    
#define GPIO_INTR_BOTH_EDGE_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_ED2INTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_LEVINTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_LEVINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_ENTPOL0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_ENTPOL_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_EXTINTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_EXTINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)))
#define GPIO_INTR_BOTH_EDGE3_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_ED2INTEN3_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_LEVINTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_LEVINTEN3_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_ENTPOL3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_ENTPOL3_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
    vIO32Write4B(REG_GPIO_EXTINTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_EXTINTEN3_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)))


#define GPIO_INTR_LEVEL_LOW_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_ED2INTEN_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_LEVINTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_LEVINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK))); \
    vIO32Write4B(REG_GPIO_ENTPOL0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_ENTPOL_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_EXTINTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_EXTINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)))
#define GPIO_INTR_LEVEL_LOW3_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_ED2INTEN3_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_LEVINTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_LEVINTEN3_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK))); \
    vIO32Write4B(REG_GPIO_ENTPOL3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_ENTPOL3_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_EXTINTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_EXTINTEN3_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)))

#define GPIO_INTR_LEVEL_HIGH_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_ED2INTEN_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
    vIO32Write4B(REG_GPIO_LEVINTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_LEVINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK))); \
    vIO32Write4B(REG_GPIO_ENTPOL0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_ENTPOL_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));   \
    vIO32Write4B(REG_GPIO_EXTINTEN0 + (GPIO_TO_INDEX(gpio) << 2), GPIO_EXTINTEN_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)))

#define GPIO_INTR_LEVEL_HIGH3_SET(gpio)    \
    vIO32Write4B(REG_GPIO_ED2INTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_ED2INTEN3_REG(gpio) & (~(1U << (gpio & EDGE_INDEX_MASK)))); \
	vIO32Write4B(REG_GPIO_LEVINTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_LEVINTEN3_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK))); \
	vIO32Write4B(REG_GPIO_ENTPOL3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_ENTPOL3_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)));    \
	vIO32Write4B(REG_GPIO_EXTINTEN3 + ((GPIO_TO_INDEX(gpio)-3) << 2), GPIO_EXTINTEN3_REG(gpio) | (1U << (gpio & EDGE_INDEX_MASK)))

#define IS_CKGEN_GPIO_LEVEL_INT(gpio) ((GPIO_TO_INDEX(gpio) < 3)?((GPIO_LEVINTEN_REG(gpio) & (1U << (gpio & EDGE_INDEX_MASK)))? 1 : 0):((GPIO_LEVINTEN3_REG(gpio) & (1U << (gpio & EDGE_INDEX_MASK)))? 1 : 0))

#define IS_CKGEN_GPIO_BOTH_EDGE_INT(gpio) ((GPIO_TO_INDEX(gpio) < 3)?((GPIO_ED2INTEN_REG(gpio) & (1U << (gpio & EDGE_INDEX_MASK)))? 1 : 0):((GPIO_ED2INTEN3_REG(gpio) & (1U << (gpio & EDGE_INDEX_MASK)))? 1 : 0))

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
        PDWNC_GpioReg,
        _PDWNC_RegLedBlinker,
        NULL,
        NULL,
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
#endif        
    }
#ifndef CC_MTK_LOADER
#if 1
    ,
    // for MT5135
    {
        CI_MT5135_GPIO_Range_Check,
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
#ifdef CC_WT6720
,
{
    _WT6720_GPIO_Range_Check,
    _WT6720_GPIO_Init,
    _WT6720_GPIO_Enable,
    _WT6720_GPIO_Input,
    _WT6720_GPIO_Intrq,
    _WT6720_GPIO_Output,
    _WT6720_GPIO_Query,
    NULL,
    NULL,
    NULL,
    NULL, 
    NULL,
    NULL, 
}
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
#if 1
#ifndef CC_MTK_LOADER  
            case GPIO_HANDLER_MT5135:
                return i4GpioNum - MT5135_GPIO0;            
#endif				
     #endif           
#ifdef CC_WT6720
            case GHIO_HANDLER_WT6720:
                return i4GpioNum - WT6720_GPIO0;            
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
    UINT32 u4Val, u4Idx;
    if ((i4GpioNum < 0) || (i4GpioNum >= NORMAL_GPIO_NUM))
    {
        return -1;
    } 
	u4Idx = GPIO_TO_INDEX(i4GpioNum);
	
	if(u4Idx <3)
		{
    		u4Val = u4IO32Read4B(REG_GPIO_EXTINTEN0 + (u4Idx << 2));
		}
	else
		{		
			u4Val = u4IO32Read4B(REG_GPIO_EXTINTEN3 + ((u4Idx - 3) << 2));
		}
 	
    if (pfgSet == NULL)
    {
        return ((u4Val & (1U << (i4GpioNum & GPIO_INDEX_MASK))) ? 1 : 0);
    }
    
    u4Val = (*pfgSet) ?
            (u4Val | (1U << (i4GpioNum & GPIO_INDEX_MASK))) :
            (u4Val & ~(1U << (i4GpioNum & GPIO_INDEX_MASK)));
			
	if(u4Idx <3)
		{
    		vIO32Write4B(REG_GPIO_EXTINTEN0 + (u4Idx<< 2), u4Val);
		}
	else
		{		
			vIO32Write4B(REG_GPIO_EXTINTEN3 + ((u4Idx - 3) << 2), u4Val);
		}

    return (*pfgSet);

}

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
    _au4CkgenGpioCallBackArg[i4Gpio] = u4Arg;
     return 0;
}

static INT32 _CKGEN_GPIO_GetIntExtParam(INT32 i4Gpio, UINT32 *pu4Arg)
{
    
    *pu4Arg = _au4CkgenGpioCallBackArg[i4Gpio];
     return 0;
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
    for (i4Idx = 0; i4Idx < 3; i4Idx++)
    {
	    vIO32Write4B(REG_GPIO_EXTINTEN0 + (i4Idx << 2), 0);
	}
    for (i4Idx = 3; i4Idx < GPIO_INT_INDEX_MAX; i4Idx++)
    {
	    vIO32Write4B(REG_GPIO_EXTINTEN3 + ((i4Idx-3) << 2), 0);
	}
	 
    for (i4Idx = 0; i4Idx < TOTAL_GPIO_IDX; i4Idx++)
    {
        _au4GpioPrevPoll[i4Idx] = 0;
        _au4GpioPollFall[i4Idx] = 0;
        _au4GpioPollRise[i4Idx] = 0;
    }
#endif /* CC_MTK_LOADER */

    // clear all edge triggered interrupt status 0.
    for (i4Idx = 0; i4Idx < 3; i4Idx++)
    {
	    vIO32Write4B(REG_GPIO_EXTINT0 + (i4Idx << 2), 0);
	}
    for (i4Idx = 3; i4Idx < GPIO_INT_INDEX_MAX; i4Idx++)
    {
	    vIO32Write4B(REG_GPIO_EXTINT3 + ((i4Idx-3) << 2), 0);
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
	UINT32 u4Val = 0;
    if (pi4Intr)
    {		
        if((GPIO_TO_INDEX(i4Gpio))<3)
			{u4Val = GPIO_INTR_REG((UINT32)i4Gpio);}
		else
			{u4Val = GPIO_INTR3_REG((UINT32)i4Gpio);}
		*pi4Intr = (INT32)(u4Val ? 1 : 0);
    }
    if (pi4Fall)
    {
        if((GPIO_TO_INDEX(i4Gpio))<3)
			{u4Val = GPIO_INTR_FALL_REG((UINT32)i4Gpio);}
		else
			{u4Val = GPIO_INTR_FALL3_REG((UINT32)i4Gpio);}
        *pi4Fall = (INT32)(u4Val ? 1 : 0);
    }
    if (pi4Rise)
    {    
		if((GPIO_TO_INDEX(i4Gpio))<3)
			{u4Val = GPIO_INTR_RISE_REG((UINT32)i4Gpio);}
		else
			{u4Val = GPIO_INTR_RISE3_REG((UINT32)i4Gpio);}
        *pi4Rise = (INT32)(u4Val ? 1 : 0);
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
    VERIFY(BSP_PinGpioSel(i4Gpio)==0);

    _au4GpioPrevPoll[GPIO_TO_INDEX(i4Gpio)] = _GPIO_POLL_IN_REG(GPIO_TO_INDEX(i4Gpio));

    /* Set the register and callback function. */
    switch(eType)
    {
    case GPIO_TYPE_INTR_FALL:		
        if((GPIO_TO_INDEX(i4Gpio))<3)
			{GPIO_INTR_FALL_SET((UINT32)i4Gpio);}
		else
			{GPIO_INTR_FALL3_SET((UINT32)i4Gpio);}
        break;
		
    case GPIO_TYPE_INTR_RISE:		
        if((GPIO_TO_INDEX(i4Gpio))<3)
			{GPIO_INTR_RISE_SET((UINT32)i4Gpio);}
		else
			{GPIO_INTR_RISE3_SET((UINT32)i4Gpio);}
        break;
		
    case GPIO_TYPE_INTR_BOTH:		
        if((GPIO_TO_INDEX(i4Gpio))<3)
			{GPIO_INTR_BOTH_EDGE_SET((UINT32)i4Gpio);}
		else
			{GPIO_INTR_BOTH_EDGE3_SET((UINT32)i4Gpio);}
        break;
		
    case GPIO_TYPE_INTR_LEVEL_LOW:
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
	        if((GPIO_TO_INDEX(i4Gpio))<3)
				{GPIO_INTR_LEVEL_LOW_SET((UINT32)i4Gpio);}
			else
				{GPIO_INTR_LEVEL_LOW3_SET((UINT32)i4Gpio);}
	    }
#else            
        if((GPIO_TO_INDEX(i4Gpio))<3)
			{GPIO_INTR_LEVEL_LOW_SET((UINT32)i4Gpio);}
		else
			{GPIO_INTR_LEVEL_LOW3_SET((UINT32)i4Gpio);}
#endif            
        break;
    case GPIO_TYPE_INTR_LEVEL_HIGH:
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
	        if((GPIO_TO_INDEX(i4Gpio))<3)
				{GPIO_INTR_LEVEL_HIGH_SET((UINT32)i4Gpio);}
			else
				{GPIO_INTR_LEVEL_HIGH3_SET((UINT32)i4Gpio);}
	    }
#else            
		if((GPIO_TO_INDEX(i4Gpio))<3)
			{GPIO_INTR_LEVEL_HIGH_SET((UINT32)i4Gpio);}
		else
			{GPIO_INTR_LEVEL_HIGH3_SET((UINT32)i4Gpio);}
#endif            
        break;
    case GPIO_TYPE_NONE:
		if((GPIO_TO_INDEX(i4Gpio))<3)
			{GPIO_INTR_CLR((UINT32)i4Gpio);}
		else
			{GPIO_INTR_CLR3((UINT32)i4Gpio);}
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
	UINT32 i4Idx;
	
	for(i4Idx=0; i4Idx<3; i4Idx++)
		{			
		vIO32Write4B((REG_GPIO_EXTINTEN0 + 4*i4Idx), 0);
		}
	for(i4Idx=3; i4Idx<GPIO_INT_INDEX_MAX; i4Idx++)
		{
		vIO32Write4B((REG_GPIO_EXTINTEN3 + 4*(i4Idx-3)), 0);
		}

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
    UNUSED(GPIO_Enable(i4GpioNum, &i4Mode));

    return GPIO_Input(i4GpioNum);
}

//-----------------------------------------------------------------------------
// GPIO Interface functions.
//-----------------------------------------------------------------------------


#ifndef CC_MTK_LOADER

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

static void _GPIO_DedicatedIsr(UINT16 u2VctId)
{
    INT32 i4Idx;
    UINT32 u4Status[GPIO_INT_INDEX_MAX] = {0};
    UINT32 u4Val,i;
    INT32 i4GpioNum;

    ASSERT(u2VctId == VECTOR_EXT1);
#ifndef CC_UP8032_ATV
    VERIFY(BIM_ClearIrq((UINT32)u2VctId));
#endif
    // get edge triggered interrupt status. 
    for(i=0; i<3; i++)
    	{
    	u4Status[i] = u4IO32Read4B(REG_GPIO_EXTINT0 + 4*i);
    	}
    for(i=3; i<GPIO_INT_INDEX_MAX; i++)
    	{
    	u4Status[i] = u4IO32Read4B(REG_GPIO_EXTINT3 + 4*(i-3));
    	}

    // handle edge triggered interrupt.
    for (i4Idx = 0; i4Idx < GPIO_SUPPORT_INT_NUM; i4Idx++)
    {
		i = GPIO_TO_INDEX(i4Idx);
		
		VERIFY(i < GPIO_INT_INDEX_MAX);
		
        u4Val = u4Status[i] & (1U << (i4Idx & GPIO_INDEX_MASK));
        if(u4Val)
        {
            i4GpioNum = i4Idx;
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
     // clear edge triggered interrupt status. 
	 for(i=0; i<3; i++)
		 {			 
		 vIO32Write4B((REG_GPIO_EXTINT0 + 4*i), 0);
		 }
	 for(i=3; i<GPIO_INT_INDEX_MAX; i++)
		 {
		 vIO32Write4B((REG_GPIO_EXTINT3 + 4*(i-3)), 0);
		 }
	//vIO32Write4B(REG_GPIO_EXTINT, 0);
}
#endif /* CC_MTK_LOADER */
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
#endif
#endif /* CC_MTK_LOADER */
#ifdef CC_WT6720
/*************** WT6720 GPIO Driver Functions *******************************/
#define SIF_CLK_DIV                                0x100
#define WT6720_DEV_ADDR 0x88
#define WT6720_GPIO_EN_ADDR0 0x08
#define WT6720_GPIO_INDEX_MASK 0x7
#define WT6720_GPIO_STATE_ADDR0 0x0a
#define WT6720_GPIO_OUT_MODE 0x1
#define WT6720_GPIO_IN_MODE 0x1
#define WT6720_GPIO_POWER_CTRL 0x0e
#ifndef CC_MTK_LOADER
static UINT32 _UTIL_Current_Task_Pointer()
{
    UINT32 u4Id, u4Tag;
    UNUSED(u4Tag);
    VERIFY(OSR_DRV_OK == x_os_drv_thread_self((VOID **)&u4Id, (VOID **)&u4Tag));
    return u4Id;
};
#endif

static INT32 _WT6720_Write(UINT8 ucRegAddr, UINT8 value)
{
    UINT32 u4WriteCount = 0; 
    u4WriteCount = SIF_Write(SIF_CLK_DIV, WT6720_DEV_ADDR, ucRegAddr, &value, 1);
//    Printf("I2C write (address,data) = (0x %x,0x %x)\n",ucRegAddr, value);
     if (u4WriteCount > 0)
    {
    	;
    }	
    else
    {
    	Printf( "Error WT6720 register setting\n");
       return -1;
    }	
    return 0;
}

static INT32 _WT6720_Read(UINT8 ucRegAddr)
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data=0;
//david : the following implement maybe need to be modified
#if 1
    u4ReadCount = SIF_Read(SIF_CLK_DIV, WT6720_DEV_ADDR, ucRegAddr, &u1Data, 1);
//    Printf("I2C read (address,data) = (0x%x, 0x%x)\n",ucRegAddr, u1Data);

    if (u4ReadCount > 0)
    {
        return u1Data;
    }   
    else
    {
        Printf("WT6720 READ FAIL\n");
        return FALSE;
    }   

#else

    if (SIF_WriteNoSubAddr(SIF_CLK_DIV, WT6720_DEV_ADDR, &ucRegAddr, 1) > 0)
    {
        u4ReadCount = SIF_ReadNoSubAddr(SIF_CLK_DIV, WT6720_DEV_ADDR, &ucRegAddr, 1);
        if (u4ReadCount > 0)
        {
            return u1Data;
        }   
        else
        {
            Printf("WT6720 READ FAIL\n");
            return FALSE;
        }   
        
    }   
    else
    {
        Printf("WT6720 READ FAIL\n");
        return FALSE;
    }   


#endif

}

static INT32 _WT6720_GPIO_Range_Check(INT32 i4GpioNum)
{
      if ((i4GpioNum > WT6720_GPIO_MAX) || (i4GpioNum < WT6720_GPIO0))
      {
            return 0;
       }
        else
        {
            return 1;
        }    
}

static INT32 _WT6720_GPIO_Enable(INT32 i4GpioNum, INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Idx;      
    ASSERT(i4GpioNum >= 0);
    ASSERT(i4GpioNum <= WT6720_GPIO_MAX);
    //ASSERT( *pfgSet==1 || *pfgSet==0 || pfgSet!=NULL );    
    _WT6720_GPIO_Init();
#ifndef CC_MTK_LOADER    
    VERIFY( _UTIL_Current_Task_Pointer() != 0);//Both of Linux & Nucleus will assert if in isr
    VERIFY(x_sema_lock(_hWT6720Mutex, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif     
        i4Idx = ((UINT32)i4GpioNum >> 3);
    
        u4Val = _WT6720_Read(i4Idx + WT6720_GPIO_EN_ADDR0);
        if (pfgSet == NULL)
        {
#ifndef CC_MTK_LOADER    
            VERIFY(x_sema_unlock(_hWT6720Mutex) == OSR_OK);
#endif 
            return ((u4Val & (1U << (i4GpioNum & WT6720_GPIO_INDEX_MASK))) ? 1 : 0);
        }
        
        u4Val = (*pfgSet) ?
                (u4Val | (1U << (i4GpioNum & WT6720_GPIO_INDEX_MASK))) :
                (u4Val & ~(1U << (i4GpioNum & WT6720_GPIO_INDEX_MASK)));
        
        _WT6720_Write(i4Idx + WT6720_GPIO_EN_ADDR0, u4Val);
#ifndef CC_MTK_LOADER    
        VERIFY(x_sema_unlock(_hWT6720Mutex) == OSR_OK);
#endif 

        return (*pfgSet);

}

static INT32 _WT6720_GPIO_Output(INT32 i4GpioNum, INT32 *pfgSet)
{
        UINT32 u4Val;
        INT32 i4Idx;
        _WT6720_GPIO_Init();
#ifndef CC_MTK_LOADER
        VERIFY(x_sema_lock(_hWT6720Mutex, X_SEMA_OPTION_WAIT) == OSR_OK);
        VERIFY( _UTIL_Current_Task_Pointer() != 0);//Both of Linux & Nucleus will assert if in isr
#endif 
        i4Idx = ((UINT32)i4GpioNum >> 3);
    
        u4Val = _WT6720_Read(i4Idx + WT6720_GPIO_STATE_ADDR0);
        if (pfgSet == NULL)
        {
#ifndef CC_MTK_LOADER    
            VERIFY(x_sema_unlock(_hWT6720Mutex) == OSR_OK);
#endif 
            return ((u4Val & (1U << (i4GpioNum & WT6720_GPIO_INDEX_MASK))) ? 1 : 0);
        }
        
        u4Val = (*pfgSet) ?
                (u4Val | (1U << (i4GpioNum & WT6720_GPIO_INDEX_MASK))) :
                (u4Val & ~(1U << (i4GpioNum & WT6720_GPIO_INDEX_MASK)));
        
        _WT6720_Write(i4Idx + WT6720_GPIO_STATE_ADDR0, u4Val);
        // Set the output mode.
        i4Idx = WT6720_GPIO_OUT_MODE;
        _WT6720_GPIO_Enable(i4GpioNum, &i4Idx);
#ifndef CC_MTK_LOADER    
            VERIFY(x_sema_unlock(_hWT6720Mutex) == OSR_OK);
#endif 

        return (*pfgSet);

}


static INT32 _WT6720_GPIO_Input(INT32 i4GpioNum)
{
    UINT32 u4Val;
    INT32 i4Idx;
    _WT6720_GPIO_Init();
#ifndef CC_MTK_LOADER
    VERIFY(x_sema_lock(_hWT6720Mutex, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif    
    // Set the input mode.
    i4Idx = WT6720_GPIO_IN_MODE;
     _WT6720_GPIO_Enable(i4GpioNum, &i4Idx);
    // Read input value.
    i4Idx = ((UINT32)i4GpioNum >> 3);
    u4Val = _WT6720_Read(i4Idx + WT6720_GPIO_STATE_ADDR0);
#ifndef CC_MTK_LOADER    
    VERIFY(x_sema_unlock(_hWT6720Mutex) == OSR_OK);
#endif    
    return ((u4Val & (1U << (i4GpioNum & WT6720_GPIO_INDEX_MASK))) ? 1U : 0);

}

static INT32 _WT6720_GPIO_Intrq(INT32 i4GpioNum, INT32 *pfgSet)
{
    UNUSED(i4GpioNum);
    return *pfgSet;
}
#if defined(CC_FAST_INIT)
static INT32 _WT6720_GPIO_resume(void)
{
     if(_gu4WT6720_INIT)
    {
        return 0;
    }
    _gu4WT6720_INIT = 1;
    SIF_pm_resume();//SIF_Init();
    _WT6720_Write(WT6720_GPIO_POWER_CTRL, 0);
    Printf("WT6720 gpio resume\n");
    return 0;   
}

static INT32 _WT6720_GPIO_suspend(void)
{
    _gu4WT6720_INIT = 0;
}
#endif

static INT32 _WT6720_GPIO_Init(void)
{
    if(_gu4WT6720_INIT)
    {
        return 0;
    }
    _gu4WT6720_INIT = 1;
    SIF_Init();
#ifndef CC_MTK_LOADER    
    VERIFY( _UTIL_Current_Task_Pointer() != 0);//Both of Linux & Nucleus will assert if in isr
    VERIFY(x_sema_create(&_hWT6720Mutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif    
    _WT6720_Write(WT6720_GPIO_POWER_CTRL, 0);
    Printf("WT6720 gpio init\n");
    return 0;
}

static INT32 _WT6720_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
    UNUSED(i4Gpio);
    UNUSED(*pi4Intr);
    UNUSED(*pi4Fall);
    UNUSED(*pi4Rise);    
    return 0;
}
#endif

