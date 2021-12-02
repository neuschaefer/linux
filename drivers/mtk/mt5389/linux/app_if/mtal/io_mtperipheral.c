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
/*-----------------------------------------------------------------------------
 * $RCSfile: io_mtperipheral.c,v $
 * $Revision: #4 $
 * $Date: 2012/07/11 $
 * $Author: hongjun.chu $
 *
 * Description: Peripherals, such as NOR/NAND/SIF/RTC/PWM/GPIO (and others), are centralized in
 *                   this file
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "mtperipheral.h"
#include <linux/mtal_ioctl.h>
#include "x_bim.h" 
#include "x_pinmux.h" 
#include "x_pdwnc.h" 
#include "x_gpio.h" 
#include "nor_if.h"
#include "nand_if.h"
#include "sif_if.h"
#include "sif_sw_if.h"
#include "rtc_if.h"
#include "srm_drvif.h"
#include "ir_if.h"
#include "drvcust_if.h"
#include "x_bim.h"
//#include "x_chip_id.h"
#ifdef CC_SUPPORT_SMC
#include "smc_if.h"
#endif
#include "cb_data.h"
#include "cb_low.h"

#include "osd_drvif.h"
#include "drv_pwm.h"
#include "eeprom_if.h"
#include "drv_hdmi.h"
#include "sif_if.h"
#include "ctrl_bus.h"
//#include "x_pdwnc.h"
                      
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define PWM_REG(eSrc)       (_arPwmRegData[eSrc].u4Reg)
#define PWM_PINSEL(eSrc)    (_arPwmRegData[eSrc].u4PinSel)
#define PWM_FUNC(eSrc)      (_arPwmRegData[eSrc].u4Func)
#define PWM_GPIO(eSrc)      (_arPwmRegData[eSrc].u4Gpio)
#define PWM_EXTPBIT(eSrc)   (_arPwmRegData[eSrc].u4ExtPBit)


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct 
{
    UINT32 u4Freq;
    UINT32 u4DutyOn;
    UINT32 u4ExtP;
    UINT32 u4RegVal;
    UINT32 fgEnable;
} _PWM_ATTR_T;

typedef struct
{
    UINT32 u4Reg;
    UINT32 u4PinSel;
    UINT32 u4Func;
    UINT32 u4Gpio;
    UINT32 u4ExtPBit;
} _PWM_REG_DATA_T;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368)//|| defined(CC_MT5389)
static const _PWM_REG_DATA_T _arPwmRegData[PWM_SRC2+1] = {
    { REG_RW_PWM0, PIN_OPWM0, PINMUX_FUNCTION2, GPIO(86),   0xff },
    { REG_RW_PWM1, PIN_OPWM1, PINMUX_FUNCTION2, GPIO(87),     0 },
    { REG_RW_PWM2, PIN_OPWM2, PINMUX_FUNCTION2, GPIO(88),     16 },
};
#elif defined(CC_MT5389)
static const _PWM_REG_DATA_T _arPwmRegData[PWM_SRC2+1] = {
    { REG_RW_PWM0, OPWM0_O, PINMUX_FUNCTION0, OPCTRL(12),   0xff },
    { REG_RW_PWM1, OPWM1_O, PINMUX_FUNCTION1, GPIO(54),     0 },
    { REG_RW_PWM2, OPWM2_O, PINMUX_FUNCTION1, GPIO(55),     16 },
};    
#endif


static _PWM_ATTR_T _arPwmCache[PWM_SRC2+1] = {{0, 0}, {0, 0}, {0, 0}};
static UINT32 _u4PowerDownEn = WAK_IRRX;
#if 0
static UINT32 _au4GpioCallBackPfn[NORMAL_GPIO_NUM];
static UINT32 _au4GpioCallBackArg[NORMAL_GPIO_NUM];
static UINT32 _au4GpioCallBackLevelType[NORMAL_GPIO_NUM];
static INT32 _ai4GpioCallBackLevelValue[NORMAL_GPIO_NUM];
#endif
static UINT32 _gu4PwmPhaseCount = 0;        // range from 0 to 3. maintain by _MtpwmTimer().
static UINT32 _gu4PwmPhaseTolerance = 0;    // timer clock. pass from _MtpwmTriggerPhaseTimer() to _MtpwmTimer().
static UINT32 _gu4PwmTargetPhase = 0;       // range from 0 to 3. pass from _MtpwmTriggerPhaseTimer() to _MtpwmTimer().
static UINT32 _gu4PwmPhaseReg2 = 0;         // PWM Phase Reg 2. pass from _MtpwmTriggerPhaseTimer() to _MtpwmTimer().
static UINT32 _fgEverReqWakeupReason = 0;
static UINT32 _fgT8032WakeupReason = (UINT32)PDWNC_WAKE_UP_REASON_HDMI;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(INT32)))                            \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(INT32)))                        \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_8ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_8ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_8ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_8ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");      \
        return MTR_ERR_INV;                                          \
    }       
	
#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_peripheral argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define COPY_FROM_USER_ARG_SIZE(ArgUsr, ArgKernel, size)       \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       (size)))                             \
    {                                                       \
        printk("mtal_ioctl_mtperipheral argument error\n");      \
        return MTR_ERR_INV;                                          \
    }        

#define COPY_TO_USER_ARG_SIZE(ArgUsr, ArgKernel,size)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_mtperipheral argument error\n");      \
        return MTR_ERR_INV;                                          \
    } \

#define USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(arg,size)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   size))              \
    {                                                       \
        printk("mtal_ioctl_mtdmx argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                

	
#define USR_SPACE_ACCESS_VALIDATE_6ARG(arg)                 \
		if (!access_ok(VERIFY_READ, (void __user *)arg, 		\
					   sizeof(MTAL_IOCTL_6ARG_T)))				\
		{														\
			printk("mtal_ioctl_auddec argument error\n");	  \
			return MTR_ERR_INV; 										 \
		}														\
	 
#define COPY_FROM_USER_6ARG(ArgUsr, ArgKernel)              \
		if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,	\
						   sizeof(MTAL_IOCTL_6ARG_T)))			\
		{														\
			printk("mtal_ioctl_auddec argument error\n");	   \
			return MTR_ERR_INV; 										 \
		}
 #ifdef MTMPLAYER_RTC_PRINT
 #undef MTMPLAYER_RTC_PRINT
 #endif
#define MTMPLAYER_RTC_PRINT(func, rtc)    	do {\
                										if(IS_MTMPLAYER_LOG_ENABLE)	\
                										{printf("%s->u1Year=%u:",func,(rtc)->u1Year);	\
                										printf("%s->u1Month=%u:",func,(rtc)->u1Month);	\
                										printf("%s->u1Day=%u:",func,(rtc)->u1Day);	\
                										printf("%s->u1Hour=%u:",func,(rtc)->u1Hour);	\
                										printf("%s->u1Minute=%u:",func,(rtc)->u1Minute);	\
                										printf("%s->u1Second=%u:",func,(rtc)->u1Second);	\
                										printf("\n");}\
                    									} while (0)


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void _RtcAlertHandler(void* pvTag)
{
    // Just put event with a dummy tag
    UINT32 u4Dummy;
    _CB_PutEvent(CB_MTAL_PERIPHERAL_RTC_TRIGGER, sizeof(UINT32), &u4Dummy);
}

static void _MtpwmTimer(void *pvTag)
{
    _gu4PwmPhaseCount = (_gu4PwmPhaseCount+1) & 0x3;
    if (_gu4PwmTargetPhase != _gu4PwmPhaseCount)
    {
        return;
    }

    if ((BIM_READ32(REG_RW_TIMER1_LOW) + _gu4PwmPhaseTolerance) >= BIM_READ32(REG_RW_TIMER1_LLMT))
    {
        BIM_WRITE32(_gu4PwmPhaseReg2, BIM_READ32(_gu4PwmPhaseReg2) & (~(PWME)));
        BIM_WRITE32(_gu4PwmPhaseReg2, BIM_READ32(_gu4PwmPhaseReg2) | PWME);
        BIM_WRITE32(REG_RW_TIMER_CTRL, BIM_READ32(REG_RW_TIMER_CTRL) & ~(TMR1_CNTDWN_EN | TMR1_AUTOLD_EN));
        _gu4PwmPhaseCount = 0;
    }
}
#if 0
static void _GpioPeriodCallbackHandler(INT32 i4Gpio, BOOL fgStatus)
{
    if( (i4Gpio < 0) || (i4Gpio >= NORMAL_GPIO_NUM) )
    {
        ASSERT(0);
        return;
    }

#ifdef CC_PERIODICAL_LEVEL_TRIGGER   ////period-level callback gpio
    if (_au4GpioCallBackPfn[i4Gpio] != 0)
    {
        MTGPIO_CB_T rGpioCb;

        rGpioCb.i4Gpio = i4Gpio;
        rGpioCb.fgStatus = fgStatus;
        rGpioCb.u4Pfn = _au4GpioCallBackPfn[i4Gpio];
        rGpioCb.u4Arg = _au4GpioCallBackArg[i4Gpio];

        _CB_PutEvent(CB_MTAL_GPIO_TRIGGER, sizeof(MTGPIO_CB_T), &rGpioCb);
    }
    else
    {
        ASSERT(0);
        return;
    }
#else
    if (_au4GpioCallBackPfn[i4Gpio] != 0)
    {
        MTGPIO_CB_T rGpioCb;

        if (_au4GpioCallBackLevelType[i4Gpio] != 0)
        {
            INT32 i4Intr;
        
            // disable for level type ISR
            i4Intr = 0;
            GPIO_Intrq( i4Gpio, &i4Intr);
        }
 
        rGpioCb.i4Gpio = i4Gpio;
        rGpioCb.fgStatus = fgStatus;
        rGpioCb.u4Pfn = _au4GpioCallBackPfn[i4Gpio];
        rGpioCb.u4Arg = _au4GpioCallBackArg[i4Gpio];

        _CB_PutEvent(CB_MTAL_GPIO_TRIGGER, sizeof(MTGPIO_CB_T), &rGpioCb);
    }
    else
    {
        ASSERT(0);
        return;
    }
 #endif
}
#endif
#ifdef CC_SUPPORT_SMC
//-----------------------------------------------------------------------------
/** _MtTsRecGBLCallback
 */
//-----------------------------------------------------------------------------
static void _SmcCallbackHandler(UINT8 u1HotPlugStatus)
{
    MTSMC_CB_T rCb;    
    rCb.u1Status = u1HotPlugStatus;
    UNUSED(_CB_PutEvent(CB_MTAL_SMC_TRIGGER, sizeof (MTSMC_CB_T), &rCb));
}
#endif

static MT_RESULT_T _MtpwmSetFrequency(MTPWMSRC_TYPE_T eSrc, UINT32 u4Frequency, UINT32 u4DutyOn)
{
    UINT32 u4PwmRsn, u4PwmP, u4PwmH, u4PwmExtP;
    UINT32 u4BusClk;
	UINT32 u4Val;

	UNUSED(u4Val);

    // Validate duty cycle
    if(u4DutyOn > 1000)
    {
        return MTR_ERR_INV;
    }

    u4BusClk = BSP_GetDomainClock(SRC_BUS_CLK);   

    // Validate frequency upper bound is ((Bus clock)/256)
    if ((u4BusClk >> 8) <= u4Frequency)
    {
        return MTR_ERR_INV;
    }

    // Check Rsn value.
    u4PwmRsn = (u4DutyOn == 1000) ? 0xfe : 0xff;
    u4PwmP = ((u4BusClk >> 8)/ u4Frequency);

    if (((PWM_EXTPBIT(eSrc) > 0x20) && (u4PwmP > 0xfff)) || (u4PwmP > 0xfffff))
    {
        return MTR_ERR_INV;
    }
	u4PwmH = ((u4DutyOn == 1000) || (((u4DutyOn * 257) / 1000) > 0xff)) ? 0xff : ((u4DutyOn * 257) / 1000);
    u4PwmExtP = ((u4PwmP >> 12) & 0xff);
	#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389) 
	vDrvSetPWM(eSrc,u4PwmP,u4PwmH,u4PwmRsn);
	#else
    u4Val = BIM_READ32(REG_RW_PWMLVDS);

    BIM_WRITE32(REG_RW_PWMLVDS, (u4Val & (~(0xff << PWM_EXTPBIT(eSrc)))) | ((u4PwmExtP & 0xff) << PWM_EXTPBIT(eSrc)));
    u4Val = (((u4PwmRsn & 0xff) << 24) | ((u4PwmH & 0xff) << 16) | ((u4PwmP & 0xfff) << 4) | PWME);

    if (u4Val != BIM_READ32(PWM_REG(eSrc)))
    {
        BIM_WRITE32(PWM_REG(eSrc), u4Val);
    }
	#endif
    _arPwmCache[eSrc].u4Freq = u4Frequency;
    _arPwmCache[eSrc].u4DutyOn = u4DutyOn;
    _arPwmCache[eSrc].u4ExtP = u4PwmExtP;
#if (!defined(CC_MT5396))&&(!defined(CC_MT5368))&&(!defined(CC_MT5389))
    _arPwmCache[eSrc].u4RegVal = u4Val;
#endif

    return MTR_OK;            
}

static MT_RESULT_T _MtpwmTriggerPhaseTimer(MTPWMSRC_TYPE_T eSrc1, MTPWMSRC_TYPE_T eSrc2, UINT32 u4Frequency, UINT8 u1Phase)
{
    static UINT32 _fgInit = 0;
    static MTPWMSRC_TYPE_T _eSrc1;
    static MTPWMSRC_TYPE_T _eSrc2;
    static UINT32 _u4Frequency;
    static UINT8 _u1Phase;

    UINT32 u4Timer, u4Val;
    void (* pfnOldIsr)(UINT16);

    if (_fgInit)
    {
        if ((_eSrc1 == eSrc1) && (_eSrc2 == eSrc2) && (_u4Frequency == u4Frequency) && (_u1Phase == u1Phase))
        {
            return MTR_OK;
        }
    }
    else
    {
        _fgInit = 1;
    }

    _eSrc1 = eSrc1;
    _eSrc2 = eSrc2;
    _u4Frequency = u4Frequency;
    _u1Phase = u1Phase;

    if (u1Phase == PWM_PHASE_0)
    {
        BIM_WRITE32(PWM_REG(eSrc1), BIM_READ32(PWM_REG(eSrc1)) & (~(PWME)));
        BIM_WRITE32(PWM_REG(eSrc2), BIM_READ32(PWM_REG(eSrc2)) & (~(PWME)));
        BIM_WRITE32(PWM_REG(eSrc1), BIM_READ32(PWM_REG(eSrc1)) | PWME);
        BIM_WRITE32(PWM_REG(eSrc2), BIM_READ32(PWM_REG(eSrc2)) | PWME);
        return MTR_OK;
    }

    // Check wait time.
    u4Timer = BSP_GetDomainClock(SRC_TIMER_CLK) / u4Frequency;
    _gu4PwmPhaseTolerance = u4Timer / 80;
    u4Timer = u4Timer / 4;

    switch(u1Phase)
    {
    case PWM_PHASE_90:
        _gu4PwmTargetPhase = 1;
        break;
    case PWM_PHASE_180:
        _gu4PwmTargetPhase = 2;
        break;
    case PWM_PHASE_270:
        _gu4PwmTargetPhase = 3;
        break;
    default:
        return MTR_NOT_SUPPORTED;
    }

    // Hook timer interrupt.
    _gu4PwmPhaseCount = 0;
    VERIFY(x_reg_isr(VECTOR_T1, (x_os_isr_fct)_MtpwmTimer, &pfnOldIsr) == OSR_OK);
    BIM_WRITE32(REG_RW_TIMER1_LLMT, u4Timer);
    BIM_WRITE32(REG_RW_TIMER1_HLMT, 0);
    BIM_WRITE32(REG_RW_TIMER1_LOW, u4Timer);
    BIM_WRITE32(REG_RW_TIMER1_HIGH, 0);

    u4Val = BIM_READ32(REG_RW_TIMER_CTRL) | TMR1_CNTDWN_EN | TMR1_AUTOLD_EN;
    BIM_WRITE32(PWM_REG(eSrc1), BIM_READ32(PWM_REG(eSrc1)) & (~(PWME)));
    BIM_WRITE32(PWM_REG(eSrc1), BIM_READ32(PWM_REG(eSrc1)) | PWME);
    _gu4PwmPhaseReg2 = PWM_REG(eSrc2);
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);
    return MTR_OK;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
//GPIO

//----------------------------------------------------------------------------
/** MTGPIO_Init() Initialize GPIO driver and hardware
 *  @retval MTR_OK, Succeed; MTR_NOT_OK, Fail.
 */
//----------------------------------------------------------------------------

static MT_RESULT_T _MTGPIO_Init (VOID)
{

x_os_drv_set_timestamp("MTGPIO_Init start");

#if 0    
    UINT32 u4Idx;
#endif

    MTPERIPHERAL_PRINT(" - void \n");
    GPIO_Init();

#if 0
    for (u4Idx = 0; u4Idx < NORMAL_GPIO_NUM; u4Idx++)
    {
        _au4GpioCallBackPfn[u4Idx] = 0;
        _au4GpioCallBackArg[u4Idx] = 0;
        _au4GpioCallBackLevelType[u4Idx] = 0;
        _ai4GpioCallBackLevelValue[u4Idx] = 0;
    }
#endif
	x_os_drv_set_timestamp("MTGPIO_Init end");

    return MTR_OK;
}


static MT_RESULT_T  _MTWD_Init(VOID)
{
	x_os_drv_set_timestamp("MTWD_Init start");
	
	MTPERIPHERAL_PRINT(" - void \n");
	PDWNC_CreateWatchDogThread();
	x_os_drv_set_timestamp("MTWD_Init end");
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** MTGPIO_SetDirection()  The GPIO Direction (in/out) setting function.
 *  @param i4GpioNum the gpio number to set.
 *  @param eDir Direction in or out
 *  @retval MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTGPIO_SetDirection (unsigned long arg)
{
     MTAL_IOCTL_2ARG_T rArg;
     INT32 i4GpioNum, i4DrvRet;
     MTGPIO_DIR_T eDir;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    i4GpioNum=(INT32)rArg.ai4Arg[0];
    eDir=(MTGPIO_DIR_T)rArg.ai4Arg[1];

 //add driver implementation code here
    i4DrvRet= GPIO_Enable(i4GpioNum, (INT32*)&eDir);

    MTPERIPHERAL_PRINT(" - i4GpioNum = %d, eDir = %d\n",
		i4GpioNum,
		eDir);
    
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** _MTGPIO_GetDirection()  The GPIO Direction (in/out) setting function.
 *  @param i4GpioNum the gpio number to set.
 *  @retval MTGPIO_DIR_T, gpio direction
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTGPIO_GetDirection (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    INT32 i4GpioNum,i4DrvRet;
//    MTGPIO_DIR_T * peDir;
    MTGPIO_DIR_T eDir;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    i4GpioNum=(INT32)rArg.ai4Arg[0];
//    peDir=(MTGPIO_DIR_T*)rArg.ai4Arg[1];

 //add driver implementation code here
    i4DrvRet=GPIO_Enable(i4GpioNum, NULL);
    eDir =i4DrvRet;
 
    USR_SPACE_ACCESS_VALIDATE_ARG(rArg.ai4Arg[1],MTGPIO_DIR_T);
    COPY_TO_USER_ARG(rArg.ai4Arg[1],eDir,MTGPIO_DIR_T);

    MTPERIPHERAL_PRINT(" - i4GpioNum = %d, direction = %d\n",
		i4GpioNum,
		eDir);    
    return MTR_OK;
}
//-----------------------------------------------------------------------------
/** _MTGPIO_GetDirection()  The GPIO Direction (in/out) setting function.
 *  @param i4GpioNum the gpio number to set.
 *  @retval MTGPIO_DIR_T, gpio direction
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTI2C_Check (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    INT32 i4Add_dev,i4DrvRet;


    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    i4Add_dev=(INT32)rArg.ai4Arg[0];
    i4DrvRet =(INT32) u1I2CCheck( i4Add_dev ) ;
 
 
    USR_SPACE_ACCESS_VALIDATE_ARG(rArg.ai4Arg[1],UINT32);
    COPY_TO_USER_ARG(rArg.ai4Arg[1],i4DrvRet,UINT32);

    return MTR_OK;
}


static MT_RESULT_T  _MTIR_GetTouchPadRawData (unsigned long arg)
{  

MTAL_IOCTL_2ARG_T rArg;
    INT32 i4ServoADNum,i4DrvRet;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    i4ServoADNum = (INT32)rArg.ai4Arg[0];    
    i4DrvRet = IRRX_ReadSvadc(i4ServoADNum) ;
 
    USR_SPACE_ACCESS_VALIDATE_ARG(rArg.ai4Arg[1],UINT32);
    COPY_TO_USER_ARG(rArg.ai4Arg[1],i4DrvRet,UINT32);

    MTPERIPHERAL_PRINT(" - i4ServoADNum = %d, i4DrvRet = %d\n",
        i4ServoADNum,
        i4DrvRet);        
   
    
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** MTGPIO_Intrq() The GPIO interrupt enable setting functions. It will check
 *  the i4GpioNum and set to related register bit as 0 or 1.  In this
 *  function, 0 is interrupt disable mode and 1 is interrupt enable mode.
 *  @param i4GpioNum the gpio number to set or read.
 *  @param pi4Intr A integer pointer.  If it's NULL, this function will return
 *  the current setting of gpio number (0 or 1).  If it's not NULL, it must
 *  reference to a integer.  If the integer is 0, this function will set the
 *  mode of the gpio number as interrupt disable mode, otherwise set as
 *  interrupt enable mode.
 *  @retval If pi4Intr is NULL, it return 0 or 1 (0 is interrupt disable mode,
 *          1 is interrupt enable mode.)  Otherwise, return (*pi4Intr).
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTGPIO_Intrq (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    INT32 i4GpioNum;
    INT32 *pi4Intr;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    i4GpioNum=(INT32)rArg.ai4Arg[0];
    pi4Intr=(INT32 *)rArg.ai4Arg[1];
    MTPERIPHERAL_PRINT(" - i4GpioNum = %d\n",
		i4GpioNum);
#if 0
    if ((_au4GpioCallBackLevelType[i4GpioNum] != 0) &&
        (pi4Intr != NULL) &&
        (*pi4Intr != 0))
    {
        if ( GPIO_GetIn(i4GpioNum) == _ai4GpioCallBackLevelValue[i4GpioNum] )
        {
            // trigger level match, call back immediately
            MTGPIO_CB_T rGpioCb;

            rGpioCb.i4Gpio = i4GpioNum;
            rGpioCb.fgStatus = _ai4GpioCallBackLevelValue[i4GpioNum];
            rGpioCb.u4Pfn = _au4GpioCallBackPfn[i4GpioNum];
            rGpioCb.u4Arg = _au4GpioCallBackArg[i4GpioNum];

            _CB_PutEvent(CB_MTAL_GPIO_TRIGGER, sizeof(MTGPIO_CB_T), &rGpioCb);

            return MTR_OK;
        }
    }
#endif
     //add driver implementation code here
    if(GPIO_Intrq( i4GpioNum, pi4Intr ) != -1)
    {
         return MTR_OK;   
    }
    else
    {
        return MTR_NOT_OK;
    }
}

//-----------------------------------------------------------------------------
/** MTGPIO_Query()  The GPIO interrupt query  functions. It will read abd return interrupt
 *  setting register bit.
 *  @param i4Gpio the gpio number to read.
 *  @param *pi4Intr Interrupt is set (=1) or not(=0)
 *  @param *pi4Fall  Falling Edge Interrupt is set (=1) or not(=0)
 *  @param *pi4Rise  Rising Edge Interrupt is set (=1) or not(=0)
 *  @retval MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTGPIO_Query (unsigned long arg)
{
     MTAL_IOCTL_4ARG_T rArg;
     INT32 i4Gpio;
     INT32 *pi4Intr=NULL;
     INT32 *pi4Fall=NULL;
     INT32 *pi4Rise=NULL;

     USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
     COPY_FROM_USER_4ARG(arg, rArg);

    i4Gpio=( INT32 )rArg.ai4Arg[0];
    pi4Intr=(INT32 *)rArg.ai4Arg[1];
    pi4Fall=(INT32 *)rArg.ai4Arg[2];
    pi4Rise=(INT32 *)rArg.ai4Arg[3];

     //add driver implementation code here
    GPIO_Query(i4Gpio, pi4Intr, pi4Fall, pi4Rise );

    MTPERIPHERAL_PRINT(" - i4Gpio = %d\n"
        "pi4Intr = %d\n"
        "pi4Fall = %d\n"
        "pi4Rise = %d\n",
	 i4Gpio,
	 (int)*pi4Intr,
	 (int)*pi4Fall,
	 (int)*pi4Rise);
	 
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** MTGPIO_Reg()  The GPIO Interrupt Callback register funciton. It will copy and keep callback
  * funciton pointer (pfnCallback) in data stucture.
 *  @param i4Gpio the gpio number to read.
 *  @param eType Interrupt type, such as double-edge, rising edge, and falling edge.
 *  @param pfnCallback  Interrupt Callback function
 *  @retval MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTGPIO_Reg (unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    INT32 i4Gpio;
    MTGPIO_TYPE_T eType;
#if 0    
    MTPFN_GPIO_CALLBACK pfnCallback;
#endif
    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    i4Gpio=( INT32 )rArg.ai4Arg[0];
    eType = (MTGPIO_TYPE_T)(rArg.ai4Arg[1]);
#if 0    
    pfnCallback = NULL;
#endif

#if 0
    if( (i4Gpio < 0) || (i4Gpio >= NORMAL_GPIO_NUM) )
    {
        return MTR_NOT_OK;
    }

    _au4GpioCallBackPfn[i4Gpio] = (UINT32) rArg.ai4Arg[2];
    _au4GpioCallBackArg[i4Gpio] = (UINT32) rArg.ai4Arg[3];
    _au4GpioCallBackLevelType[i4Gpio] = 0;
    
    if (_au4GpioCallBackPfn[i4Gpio] != 0)
    {
        pfnCallback = _GpioPeriodCallbackHandler;
    }
    else
    {
        // deregister, change type to NONE instead of install NULL pointer
        eType = GPIO_TYPE_NONE;
    }
#endif
    MTPERIPHERAL_PRINT("_MTGPIO_Reg - i4Gpio = %d, eType = %d, pfnCallback = 0x%x\n",
        i4Gpio,
        eType,
        (UINT32) rArg.ai4Arg[2]);
    GPIO_SetIntExtParam(i4Gpio, (UINT32) rArg.ai4Arg[3]);  //This function *MUST" be executed before GPIO_Reg!!!              
    if(GPIO_Reg(i4Gpio, eType, (PFN_GPIO_CALLBACK)((UINT32) rArg.ai4Arg[2])) == 0)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;
    }
}

//-----------------------------------------------------------------------------
/** MTGPIO_SetOut() to set gpio output value.
 *  @param i4GpioNum
 *  @param i4Val 0 or 1.
 *  @retval to return current gpio out register setting.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTGPIO_SetOut (unsigned long arg)
{
     MTAL_IOCTL_2ARG_T rArg;
     INT32 i4GpioNum;
     INT32 i4Val;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    i4GpioNum=(INT32)rArg.ai4Arg[0];
    i4Val=(INT32 )rArg.ai4Arg[1];
    MTPERIPHERAL_PRINT(" - i4GpioNum = %d, i4Val = 0x%x\n",
		i4GpioNum,
		i4Val);

     //add driver implementation code here
     GPIO_SetOut(i4GpioNum, i4Val );

    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** MTGPIO_GetIn()  The GPIO input setting and reading  functions. It will check the
 *  i4GpioNum, set its as an input pin,  and read its polarity register bit to return.
 *  @param i4GpioNum the gpio number to read.
 *  @retval 0 or 1.  (GPIO input value.)
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTGPIO_GetIn (unsigned long arg)
{
     MTAL_IOCTL_2ARG_T rArg;
     INT32 i4GpioNum;
     UINT8 *pi4Val;
     UINT8  i4Val = 0;
     MT_RESULT_T ret=MTR_NOT_OK;
	 INT32 i4Mode=0;

     USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
     COPY_FROM_USER_2ARG(arg, rArg);

    i4GpioNum=(INT32)rArg.ai4Arg[0];
    pi4Val=(UINT8*)rArg.ai4Arg[1];

    //add driver implementation code here
    // Set to input mode.
    if ( GPIO_Enable(i4GpioNum, &i4Mode)==i4Mode )
    {
        i4Val = GPIO_GetIn(i4GpioNum);
        if(i4Val >=0)
        {
            ret =MTR_OK;
            *pi4Val = i4Val;
        }
        else //error
        {
            ret =MTR_NOT_OK;
            pi4Val = NULL;
        }
    }
    else
    {
        ret =MTR_NOT_SUPPORTED;
        pi4Val = NULL;
    }
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pi4Val,UINT8);
    COPY_TO_USER_ARG(pi4Val,i4Val,UINT8);

     MTPERIPHERAL_PRINT(" - i4GpioNum = %d, i4Val = 0x%x\n",
		i4GpioNum,
		i4Val);

    return ret;
}

//-----------------------------------------------------------------------------
/** MTGPIO_PeriodLevelCallback()
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTGPIO_PeriodLevelCallback(unsigned long arg)
{
    MTAL_IOCTL_8ARG_T rArg;
    INT32 i4Gpio; INT32 i4HighLow; UINT32 u4Period/*mili-second*/; //PFN_GPIO_CALLBACK pfnCallback;
    INT32 i4LowRet;
    UINT32 u4GpioCallBackPfn;
    UINT32 u4GpioCallBackArg;
    USR_SPACE_ACCESS_VALIDATE_8ARG(arg);
    COPY_FROM_USER_8ARG(arg, rArg);    
	UNUSED(i4LowRet);

    i4Gpio=(INT32)rArg.ai4Arg[0];
    i4HighLow=(INT32)rArg.ai4Arg[1];
    u4Period=(UINT32)rArg.ai4Arg[2];
   // pfnCallback = NULL;
/*
    if( (i4Gpio < 0) || (i4Gpio > NORMAL_GPIO_NUM) )
    {
        return MTR_NOT_OK;
    }
*/
    u4GpioCallBackPfn = (UINT32) rArg.ai4Arg[3];//    _au4GpioCallBackPfn[i4Gpio] = (UINT32) rArg.ai4Arg[3];
    u4GpioCallBackArg = (UINT32) rArg.ai4Arg[4];//    _au4GpioCallBackArg[i4Gpio] = (UINT32) rArg.ai4Arg[4];
#if 0    
    _ai4GpioCallBackLevelValue[i4Gpio] = i4HighLow;

    if(u4GpioCallBackPfn != 0 )//if (_au4GpioCallBackPfn[i4Gpio] != 0)
    {
        pfnCallback = _GpioPeriodCallbackHandler;
    }
#endif
#ifdef CC_PERIODICAL_LEVEL_TRIGGER   ////period-level callback gpio

    ASSERT(0);
    i4LowRet= GPIO_RegPeriodLevel( i4Gpio, i4HighLow , u4Period , pfnCallback);

    if(i4LowRet==0)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;
    }
    
#else

    if((void *)u4GpioCallBackPfn == NULL)//if(_au4GpioCallBackPfn[i4Gpio] == NULL)
    {
        GPIO_Reg(i4Gpio, GPIO_TYPE_NONE, NULL);
        //_au4GpioCallBackLevelType[i4Gpio] = 0;
    }
    else
    {
#if 1   
    MTPERIPHERAL_PRINT("_MTGPIO_PeriodLevelCallback - i4Gpio = %d, i4HighLow = %d, u4Period = %d, pfnCallback = 0x%x\n",
        i4Gpio,
        i4HighLow,
        u4Period,
        u4GpioCallBackPfn);

        if(!i4HighLow)     
        {
            GPIO_SetIntExtParam(i4Gpio, u4GpioCallBackArg);//This function *MUST" be executed before GPIO_Reg!!!
            if(GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_LEVEL_LOW, (PFN_GPIO_CALLBACK)u4GpioCallBackPfn) == 0)
            {
                return MTR_OK;
            }
            else
            {
                return MTR_NOT_OK;
            }
        }
        else
        {
            GPIO_SetIntExtParam(i4Gpio, u4GpioCallBackArg);//This function *MUST" be executed before GPIO_Reg!!!
            if(GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_LEVEL_HIGH, (PFN_GPIO_CALLBACK)u4GpioCallBackPfn) == 0)
            {
                return MTR_OK;
            }
            else
            {
                return MTR_NOT_OK;
            }
        }
#else        
        _au4GpioCallBackLevelType[i4Gpio] = 1;

        if ( GPIO_GetIn(i4Gpio) == i4HighLow )
        {
            // trigger level match, call back immediately
            MTGPIO_CB_T rGpioCb;

            rGpioCb.i4Gpio = i4Gpio;
            rGpioCb.fgStatus = i4HighLow;
            rGpioCb.u4Pfn = _au4GpioCallBackPfn[i4Gpio];
            rGpioCb.u4Arg = _au4GpioCallBackArg[i4Gpio];

            _CB_PutEvent(CB_MTAL_GPIO_TRIGGER, sizeof(MTGPIO_CB_T), &rGpioCb);
        }
        else
        {
            // trigger level not match, register callback

            if (i4HighLow != 0)
            {
                // High Level Trigger, wait for rising edge
                GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_RISE, pfnCallback);
            }
            else
            {
                // Low Level Trigger, wait for falling edge
                GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_FALL, pfnCallback);
            }
        }
#endif        
    }

    return MTR_OK;
#endif
}

//PWM
//-----------------------------------------------------------------------------
/** MTPWM_SetFrequency()  This driver control the PWM0 & PWM1 output
 *  frequence and duty cycle
 *  @param eSrc      PWM source (SrcPWM0/1/2)
 *  @param u4Frequency Frequency demanded (in Hz)
 *  @param u4DutyOn  Duty cycle percentage multiple by 10, ex. u4DutyOn =367 -->36.7% Duty Cycle
 *  @param fgEnable
 *  @return MT_RESULT_T
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPWM_SetFrequency(unsigned long arg)
{
    MT_RESULT_T rRet;
    MTAL_IOCTL_4ARG_T rArg;
    MTPWMSRC_TYPE_T eSrc;
    UINT32 u4Frequency;
    UINT32 u4DutyOn;
    BOOL fgEnable;
     
    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    eSrc=( MTPWMSRC_TYPE_T )rArg.ai4Arg[0];
    u4Frequency = (UINT32)rArg.ai4Arg[1];
    u4DutyOn=(UINT32)rArg.ai4Arg[2];
    fgEnable=(BOOL)rArg.ai4Arg[3];

    MTPERIPHERAL_PRINT(" - eSrc = %d\n"
		"u4Frequency = 0x%x\n"
		"u4DutyOn = 0x%x\n"
		"fgEnable = %d\n",
		eSrc,
		u4Frequency,
		u4DutyOn,
		fgEnable);

    if (eSrc > PWM_SRC2)
    {
        return MTR_ERR_INV;
    }

    if (!fgEnable)
    {
        GPIO_SetOut(PWM_GPIO(eSrc), 0);
    }

    rRet = _MtpwmSetFrequency(eSrc, u4Frequency, u4DutyOn);

    _arPwmCache[eSrc].fgEnable = fgEnable;

    if (fgEnable)
    {
        UNUSED(BSP_PinSet(PWM_PINSEL(eSrc), PWM_FUNC(eSrc)));
    }

    return rRet;
}

//-----------------------------------------------------------------------------
/** MTPWM_SetPhase()  This API controls the phase between PWMs.
 *  Must the same frequency and must both enable.
 *  @param prPwm1 PWM source1 and duty
 *  @param prPwm2 PWM source2 and duty
 *  @param u4Frequency Frequency demanded (in Hz)
 *  @param u1Phase phase between src1 and src2
 *  @return MT_RESULT_T
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPWM_SetPhase(unsigned long arg)
{
    MTAL_IOCTL_8ARG_T rArg;
    MTPWMSET_TYPE_T *prPwm1, *prPwm2;
    UINT32 u4Frequency;
    UINT8 u1Phase;
    MT_RESULT_T rRet;

    USR_SPACE_ACCESS_VALIDATE_8ARG(arg);
    COPY_FROM_USER_8ARG(arg, rArg);

    prPwm1 = (MTPWMSET_TYPE_T *)&rArg.ai4Arg[0];
    prPwm2 = (MTPWMSET_TYPE_T *)&rArg.ai4Arg[2];
    u4Frequency = (UINT32)rArg.ai4Arg[4];
    u1Phase = (UINT8)rArg.ai4Arg[5];

    MTPERIPHERAL_PRINT("yyy - eSrc1 = %d, Duty1:0x%x\n"
        "eSrc2 = %d, Duty2:0x%x\n"
		"u4Frequency = 0x%x\n"
		"u1Phase = %d\n",
		prPwm1->ePwmSrc, prPwm1->u4DutyOn,
		prPwm2->ePwmSrc, prPwm2->u4DutyOn,
		u4Frequency,
		(UINT32)u1Phase);

    if ((prPwm1->u4DutyOn > 1000) || (prPwm2->u4DutyOn > 1000))
    {
        return MTR_ERR_INV;
    }

    rRet = _MtpwmSetFrequency(prPwm2->ePwmSrc, u4Frequency, prPwm2->u4DutyOn);
    if (rRet != MTR_OK)
    {
        return rRet;
    }

    rRet = _MtpwmSetFrequency(prPwm1->ePwmSrc, u4Frequency, prPwm1->u4DutyOn);
    if (rRet != MTR_OK)
    {
        return rRet;
    }

    return _MtpwmTriggerPhaseTimer(prPwm1->ePwmSrc, prPwm2->ePwmSrc, u4Frequency, u1Phase);
}


//Servo ADC
//-----------------------------------------------------------------------------
/** MTPDWNC_ReadServoADCChannelValue() Read the ServoADC Channel Value
 *  @param u4Channel the channel number
 *  @retval the channel adc value (0-255).
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPDWNC_ReadServoADCChannelValue(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4Channel;
    UINT8* pu1Value;
    UINT8 u1Value;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);	
    u4Channel = (UINT32)rArg.ai4Arg[0];
    pu1Value = (UINT8*)rArg.ai4Arg[1];

    // Validate servo ADC number
     if(u4Channel >= TOTAL_SERVOADC_NUMBER)
     {
         return MTR_NOT_SUPPORTED;
     }
     
     u1Value = PDWNC_ReadServoADCChannelValue(u4Channel);
     *pu1Value = u1Value;

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1Value,UINT8);
    COPY_TO_USER_ARG(pu1Value, u1Value, UINT8);
	
    MTPERIPHERAL_PRINT(" - u4Channel = 0x%x, u1Value = 0x%x\n",
		(int)u4Channel,
		(int)u1Value);     
    return MTR_OK;
}

//-------------------------------------------------------------------------
/** _MTPDWNC_EnterStandby
 *  Let ARM enter standby mode
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static MT_RESULT_T _MTPDWNC_EnterStandby (unsigned long arg)
{
    UINT32 u4PowerDownType;

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, u4PowerDownType);    
    
    PDWNC_EnterPowerDown(_u4PowerDownEn, u4PowerDownType);
    
    return MTR_OK;
}

extern INT32 PDWNC_T8032Cmd(PDWNC_T8032_CMD_T* prCmd, PDWNC_T8032_RESPONSE_T* prResponse);
//-------------------------------------------------------------------------
/** _MTPDWNC_T8032Cmd
 *  ARM/T8032 command interface
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static MT_RESULT_T _MTPDWNC_T8032Cmd (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTPDWNC_T8032_CMD_T    *usr_prCmd;    
    PDWNC_T8032_CMD_T rCmd;

    MTPDWNC_T8032_RESPONSE_T    *usr_prRes;    
    PDWNC_T8032_RESPONSE_T rRes;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    usr_prCmd = (MTPDWNC_T8032_CMD_T*)rArg.ai4Arg[0];
    usr_prRes = (MTPDWNC_T8032_RESPONSE_T*)rArg.ai4Arg[1];
    if (PDWNC_T8032Cmd(&rCmd, &rRes) == IR_FAIL)
    {
        //MTPERIPHERAL_PRINT("\nIRRX_PollMtkIr ===> IR_FAIL\n");
        return MTR_NOT_OK;
    }
    //MTPERIPHERAL_PRINT("\nIRRX_PollMtkIr ===> IR_Succ\n");

    USR_SPACE_ACCESS_VALIDATE_ARG(usr_prCmd, MTPDWNC_T8032_CMD_T);
    COPY_TO_USER_ARG(usr_prCmd, rCmd, MTPDWNC_T8032_CMD_T); 

    USR_SPACE_ACCESS_VALIDATE_ARG(usr_prRes, PDWNC_T8032_RESPONSE_T);
    COPY_TO_USER_ARG(usr_prRes, rRes, PDWNC_T8032_RESPONSE_T);    
    return MTR_OK;
}

EXTERN UINT32 _PDWNC_ReadWakeupStatus(void);
EXTERN UINT32 _PDWNC_ReadWatchDogStatus(void);
//-------------------------------------------------------------------------
/** _MTPDWNC_GetWakeupReason
 *  Query system wakeup reason
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static UINT32 _PDWNC_ReadWakeupReason(void)
{
    UINT32 u4Val;
    UINT32 u4Reason;

    // Wakeup REASON...
    u4Val = _PDWNC_ReadWakeupStatus();     
    if (u4Val == 0)
    {
        if(_PDWNC_ReadWatchDogStatus())
        {
            u4Reason = (UINT32)PDWNC_WAKE_UP_REASON_WATCHDOG;
#ifdef CC_MTK_LOADER
            MTPERIPHERAL_PRINT("Boot reason: Watch dog reset!!");            
#endif
        }
        else
        {
            u4Reason = (UINT32)PDWNC_WAKE_UP_REASON_AC_POWER;
#ifdef CC_MTK_LOADER            
            MTPERIPHERAL_PRINT("Boot reason: A/C power on!!");                       
#endif
        }
    }
    else if (u4Val & WAK_RTC)
    {
        u4Reason = (UINT32)PDWNC_WAKE_UP_REASON_RTC;
    }
    else if (u4Val & WAK_VGA)
    {
        u4Reason = (UINT32)PDWNC_WAKE_UP_REASON_VGA;
    }
    else if (u4Val & WAK_RS232)
    {
        u4Reason = (UINT32)PDWNC_WAKE_UP_REASON_UART;
    }
    else if (u4Val & WAK_UNOR)
    {
        u4Reason = (UINT32)PDWNC_WAKE_UP_REASON_UART_NORMAL;
    }
    else if (u4Val & WAK_HDMI)
    {
        u4Reason = (UINT32)PDWNC_WAKE_UP_REASON_HDMI;
    }
    else if (u4Val & WAK_T8032)
    {
        PDWNC_T8032_CMD_T rCmd; 
        PDWNC_T8032_RESPONSE_T rResponse; 
 
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_READ_WAKEUP_REASON;  //Get the real wakeup reason
        rCmd.u1SubCmd = 0; rCmd.au1Data[0] = rCmd.au1Data[1] = rCmd.au1Data[2] = rCmd.au1Data[3] = 0;
        rResponse.au1Data[0] = PDWNC_WAKE_UP_REASON_HDMI;//T8032_WAKEUP_REASON_CEC_ONE_TOUCH_PLAY; 
        if(_fgEverReqWakeupReason == 0)
        {
            if(PDWNC_T8032Cmd(&rCmd, &rResponse)) 
            {
                u4Reason = (UINT32)PDWNC_WAKE_UP_REASON_HDMI;
            }
/*            
            else if (rResponse.au1Data[0] > PC_WAKE_UP_REASON_CUSTOM_4)
            {
                u4Reason = (UINT32)PDWNC_WAKE_UP_REASON_IRRC;
            }
*/            
            else
            {
                u4Reason = (UINT32)rResponse.au1Data[0];
            }
            _fgEverReqWakeupReason = 1;
            _fgT8032WakeupReason = u4Reason;
            MTPERIPHERAL_PRINT("Wakeup reason from T8032 is %d\n", u4Reason);            
        }
        else
        {
            u4Reason = (UINT32)_fgT8032WakeupReason;
            MTPERIPHERAL_PRINT("Wakeup reason is %d\n", u4Reason);
        }
    }
    else if (PDWNC_IsWakeupByPowerButton())
    {
        u4Reason = (UINT32)PDWNC_WAKE_UP_REASON_KEYPAD;
    }
    else // if (u4Val & WAK_IRRX)
    {
        u4Reason = (UINT32)PDWNC_WAKE_UP_REASON_IRRC;
    }
    return u4Reason;
}


static MT_RESULT_T _MTPDWNC_GetWakeupReason(unsigned long arg)
{
    UINT32 *pu4Val;
     UINT32  u4Reason;
    UINT32 rArg;
     
    pu4Val=(UINT32*)arg;

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg,rArg);
  //add driver implementation code here
    u4Reason = _PDWNC_ReadWakeupReason();

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Val,UINT32);
    COPY_TO_USER_ARG(pu4Val, u4Reason, UINT32);

    MTPERIPHERAL_PRINT(" - size = 0x%x \n",u4Reason);
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** _MTPDWNC_SetWakeupSource
 *  Set wakeup 
 *  @param pu4Key, 
 *  @param pu4Raw, 
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 *
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPDWNC_SetWakeupSource(unsigned long arg)
{
    UINT32 u4Arg;

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, u4Arg);    

    // Some necessary check...
    
    _u4PowerDownEn = u4Arg;

    return MTR_OK;
}

//SIF
//-------------------------------------------------------------------------
/** MTSIF_Init
 *  Initialize SIF module.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static MT_RESULT_T  _MTSIF_Init (VOID)
{
	
   MTPERIPHERAL_PRINT(" - void \n");

     //add driver implementation code here
    SIF_Init();

    return MTR_OK;
}

//-------------------------------------------------------------------------
/** MTSIF_Stop
 *  terminate SIF module.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static  MT_RESULT_T  _MTSIF_Stop (VOID)
{
	MTPERIPHERAL_PRINT(" - void \n");

     //add driver implementation code here
     //SIF_Stop();

    return MTR_OK;
}

//-------------------------------------------------------------------------
/** MTSIF_WriteMultipleSubAddr
 *  write data with word address to SIF module.
 *  @param  ePort  	SIF Port ID
 *  @param  u4ClkVal 	Demaned Sif Clock in Khz
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  pu1Addr 	Pointer to Serial interface word address.
 *                      *pu1Addr is sent first on bus.
 *  @param  u2AddrCnt 	Serial interface word address count (up to 3).
 *  @param  pu1Data 	Pointer to data
 *  @param  u2DataCnt 	Number of byte to read. Max = 256 bytes.
 *  @retval   MT_RESULT_T
 *            MTR_OK for OK
 *            MTR_PARAMETER_ERROR for wrong parameter, 
 *            MTR_NOT_OK for execution error
 */
//-------------------------------------------------------------------------
static MT_RESULT_T  _MTSIF_WriteMultipleSubAddr (unsigned long arg )
{
//MTSIFPORT_TYPE_T ePort, UINT32 u4ClkVal, UINT8 u1DevAddr, 
//       UINT8 *pu1Addr, UINT16 u2AddrCnt, UINT8 *pu1Data, UINT16 u2DataCnt    
    MTAL_IOCTL_8ARG_T rArg;
    MTSIFPORT_TYPE_T ePort;
    UINT32 u4ClkVal;
    UINT8 u1DevAddr;
    UINT8 *pu1Addr = NULL;
    UINT16 u2AddrCnt;
    UINT8 *pu1Data_User = NULL;
    UINT16 u2DataCnt;
    UINT8 * pu1KernelBuf = NULL;     
    UINT32 u4WordAddr = 0;
    UINT8 *pu1AddrKrnl = NULL;
    SIF_BUS_ID_T mtDrv_SifBusId;
    UINT16 u2Ret;

    USR_SPACE_ACCESS_VALIDATE_8ARG(arg);
    COPY_FROM_USER_8ARG(arg, rArg);

    ePort = (MTSIFPORT_TYPE_T)rArg.ai4Arg[0];
    u4ClkVal = (UINT32)rArg.ai4Arg[1];
    u1DevAddr = (UINT8)rArg.ai4Arg[2];
    pu1Addr = (UINT8 *)rArg.ai4Arg[3];
    u2AddrCnt = (UINT16)rArg.ai4Arg[4];
    pu1Data_User = (UINT8 *)rArg.ai4Arg[5];
    u2DataCnt = (UINT16)rArg.ai4Arg[6];

    MTPERIPHERAL_PRINT(" - ePort = %d\n"
		"u4ClkVal = 0x%x\n"
		"u1DevAddr = 0x%x\n"
		"u2AddrCnt = 0x%x\n"
		"u2DataCnt = 0x%x\n",
		ePort,
		u4ClkVal,
		u1DevAddr,
		u2AddrCnt,
		u2DataCnt);
    
    //parameter check
    if ((u2AddrCnt > 3) || (u4ClkVal == 0) || (pu1Data_User == NULL) || (u2DataCnt == 0))
    {
        return MTR_PARAMETER_ERROR;
    }

    if (u2AddrCnt && !pu1Addr)
    {
        return MTR_PARAMETER_ERROR;
    }

    //add driver implementation code here
    pu1KernelBuf = kmalloc(u2DataCnt, GFP_KERNEL);
    if (!pu1KernelBuf)
    {
        return MTR_PARAMETER_ERROR;
    }
                
    if (ePort == SIF_PORT_0)
    {
        mtDrv_SifBusId = SIF_BUS_SYSTEM_I2C;
    }
    else if (ePort == SIF_PORT_1)
    {
        mtDrv_SifBusId = SIF_BUS_SECOND_I2C;
    }
    else if (ePort == SIF_PORT_TUNER)
    {
        mtDrv_SifBusId = SIF_BUS_TUNER_I2C;
    }
    else if (ePort == SIF_PORT_PDWNC)
    {
        mtDrv_SifBusId = SIF_BUS_PDWNC_I2C;
    }
	#if defined(CC_MT5396) 
	else if (ePort == SIF_PORT_PDWNC_1)
    {
        mtDrv_SifBusId = SIF_BUS_PDWNC_I2C_1;
    }
	#endif
    else
    {
        return MTR_PARAMETER_ERROR;
    }
        
    if (copy_from_user((void *) pu1KernelBuf, (const void __user *) pu1Data_User, u2DataCnt))
    {        
        kfree(pu1KernelBuf);
        return MTR_PARAMETER_ERROR;
    }
            
    if (u2AddrCnt && pu1Addr) //with address
    {
        int i;

        pu1AddrKrnl = kmalloc(u2AddrCnt, GFP_KERNEL);
        if (!pu1AddrKrnl)
        {
            kfree(pu1KernelBuf);
            return MTR_PARAMETER_ERROR;
        }
        
        if(copy_from_user((void *) pu1AddrKrnl, (const void __user *) pu1Addr, u2AddrCnt))
    	{        	
			kfree(pu1KernelBuf);			
			kfree(pu1AddrKrnl);
			return MTR_NOT_OK;
    	}

        for (i = 0; i < u2AddrCnt; i++)
        {
            u4WordAddr = u4WordAddr << 8;
            u4WordAddr |= *(pu1AddrKrnl + i);
        }
    }
    
    u2Ret = SIF_X_Write(mtDrv_SifBusId, (27000/u4ClkVal), u1DevAddr, u2AddrCnt,
                        u4WordAddr, pu1KernelBuf, u2DataCnt);

    if (pu1KernelBuf) 
    {
        kfree(pu1KernelBuf);
    }
    if (pu1AddrKrnl) 
    {
        kfree(pu1AddrKrnl);
    }

    if (u2Ret)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;
    }
}

//-------------------------------------------------------------------------
/** MTSIF_ReadMultipleSubAddr
 *  read data with word address from SIF module.
 *  @param  ePort   	SIF Port ID
 *  @param  u4ClkVal 	Demaned Sif Clock in Khz
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  pu1Addr 	Pointer to Serial interface word address.
 *                      *pu1Addr is sent first on bus.
 *  @param  u2AddrCnt 	Serial interface word address count (up to 3).
 *  @param  pu1Data 		Pointer to data
 *  @param  u2DataCnt 	Number of byte to read. Max = 256 bytes.
 *  @retval   MT_RESULT_T
 *            MTR_OK for OK
 *            MTR_PARAMETER_ERROR for wrong parameter, 
 *            MTR_NOT_OK for execution error
 */
//-------------------------------------------------------------------------
static MT_RESULT_T  _MTSIF_ReadMultipleSubAddr (unsigned long arg)
{
//MTSIFPORT_TYPE_T ePort, UINT32 u4ClkVal, UINT8 u1DevAddr, 
//       UINT8 *pu1Addr, UINT16 u2AddrCnt, UINT8 *pu1Data, UINT16 u2DataCnt    
    MTAL_IOCTL_8ARG_T rArg;
    MTSIFPORT_TYPE_T ePort;
    UINT32 u4ClkVal;
    UINT8 u1DevAddr;
    UINT8 *pu1Addr = NULL;
    UINT16 u2AddrCnt;
    UINT8 *pu1Data_User = NULL;
    UINT16 u2DataCnt;
    UINT8 * pu1KernelBuf = NULL;     
    UINT32 u4WordAddr = 0;
    UINT8 *pu1AddrKrnl = NULL;
    SIF_BUS_ID_T mtDrv_SifBusId;
    UINT16 u2Ret;
    
    USR_SPACE_ACCESS_VALIDATE_8ARG(arg);
    COPY_FROM_USER_8ARG(arg, rArg);

    ePort = (MTSIFPORT_TYPE_T)rArg.ai4Arg[0];
    u4ClkVal = (UINT32)rArg.ai4Arg[1];
    u1DevAddr = (UINT8)rArg.ai4Arg[2];
    pu1Addr = (UINT8 *)rArg.ai4Arg[3];
    u2AddrCnt = (UINT16)rArg.ai4Arg[4];
    pu1Data_User = (UINT8 *)rArg.ai4Arg[5];
    u2DataCnt = (UINT16)rArg.ai4Arg[6];

    MTPERIPHERAL_PRINT(" - ePort = %d\n"
		"u4ClkVal = 0x%x\n"
		"u1DevAddr = 0x%x\n"
		"u2AddrCnt = 0x%x\n"
		"u2DataCnt = 0x%x\n",
		ePort,
		u4ClkVal,
		u1DevAddr,
		u2AddrCnt,
		u2DataCnt);

    //parameter check
    if( (u2AddrCnt > 3) || (u4ClkVal==0) || (pu1Data_User==NULL) || (u2DataCnt==0))
    {
        return MTR_PARAMETER_ERROR;
    }

    if (u2AddrCnt && !pu1Addr)
    {
        return MTR_PARAMETER_ERROR;
    }

    //add driver implementation code here
    pu1KernelBuf = kmalloc(u2DataCnt, GFP_KERNEL);
    if (!pu1KernelBuf)
    {
        return MTR_PARAMETER_ERROR;
    }
                
    if (ePort == SIF_PORT_0)
    {
        mtDrv_SifBusId = SIF_BUS_SYSTEM_I2C;
    }
    else if (ePort == SIF_PORT_1)
    {
        mtDrv_SifBusId = SIF_BUS_SECOND_I2C;
    }
    else if (ePort == SIF_PORT_TUNER)
    {
        mtDrv_SifBusId = SIF_BUS_TUNER_I2C;
    }
    else if (ePort == SIF_PORT_PDWNC)
    {
        mtDrv_SifBusId = SIF_BUS_PDWNC_I2C;
    }
#if defined(CC_MT5396) 
	else if (ePort == SIF_PORT_PDWNC_1)
    {
        mtDrv_SifBusId = SIF_BUS_PDWNC_I2C_1;
    }
#endif

    else
    {
        return MTR_PARAMETER_ERROR;
    }

    if (u2AddrCnt && pu1Addr) //with address
    {
        int i;

        pu1AddrKrnl = kmalloc(u2AddrCnt, GFP_KERNEL);
        if (!pu1AddrKrnl)
        {
            kfree(pu1KernelBuf);
            return MTR_PARAMETER_ERROR;
        }
        
        if(copy_from_user((void *) pu1AddrKrnl, (const void __user *) pu1Addr, u2AddrCnt))
    	{
            kfree(pu1KernelBuf);
			kfree(pu1AddrKrnl);
			return MTR_NOT_OK;
    	}

        for (i = 0; i < u2AddrCnt; i++)
        {
            u4WordAddr = u4WordAddr << 8;
            u4WordAddr |= *(pu1AddrKrnl + i);
        }
    }
    
    u2Ret = SIF_X_Read(mtDrv_SifBusId, (27000/u4ClkVal), u1DevAddr, u2AddrCnt,
                        u4WordAddr, pu1KernelBuf, u2DataCnt);

    if(copy_to_user((void __user *)pu1Data_User, (const void *)pu1KernelBuf, u2DataCnt))
	{
        kfree(pu1KernelBuf);
        kfree(pu1AddrKrnl);
		return MTR_PARAMETER_ERROR;
	}
    if (pu1KernelBuf) 
    {
        kfree(pu1KernelBuf);
    }
    if (pu1AddrKrnl) 
    {
        kfree(pu1AddrKrnl);
    }

    if (u2Ret)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;
    }
}
//SIFHW
//-------------------------------------------------------------------------
/** _MTSIFSW_Init
 *  Initialize SIFSW module.
 *  @param  u4SDA/u4SCL: data_gpio_id/addr_gpio_id
 *  @retval MTR_OK, Succeed; MTR_NOT_OK, Fail
 */
//-------------------------------------------------------------------------
static MT_RESULT_T  _MTSIFSW_Init (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4SDA;
    UINT32 u4SCL;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    u4SDA = (UINT32)rArg.ai4Arg[0];
    u4SCL = (UINT32)rArg.ai4Arg[1];

    //add driver implementation code here
    SIFSW_Init(u4SDA, u4SCL);
     
    return MTR_OK;
}

//-------------------------------------------------------------------------
/** MTSIFSW_X_Read
 * I2C read command (general type)
 *
 * @param bDevAddr device address
 * @param bSubAddr sub-address
 * @param bDataCount numbber of bytes to be read
 * @param pbData pointer of data to be read
 * @param bOption options for this command
 * @param bOption bit 2-0: bit delay (us)
 * @param bOption bit 5-3: UINT8 delay (us)
 * @param bOption bit 6: need to check ACK
 * @param bOption bit 7: Reserve
 * @param bOption bit 8: halt and wait for ACK (full implementation of spec ch.7.2)
 * @return 1 or 0
 * @retval = 1 Read success
 * @retval = 0 Read fail
 */

 //-------------------------------------------------------------------------
static MT_RESULT_T  _MTSIFSW_X_Read (unsigned long arg )
{
    MTAL_IOCTL_6ARG_T rArg;
	UINT16 u2ClkDiv;
    UINT8 u1Ret;
	UINT8 u1DevAddr;
	UINT8 u1WordAddrNum;
	UINT32 u4WordAddr;
	UINT8 * pu1KernelData = NULL;
	UINT8 *pbData = NULL;
	UINT16 u2ByteCnt;
	
	USR_SPACE_ACCESS_VALIDATE_6ARG(arg);
    COPY_FROM_USER_6ARG(arg, rArg);

	u2ClkDiv = (UINT16)rArg.ai4Arg[0];
    u1DevAddr = (UINT8)rArg.ai4Arg[1];
    u1WordAddrNum = (UINT8)rArg.ai4Arg[2];
    u4WordAddr = (UINT32)rArg.ai4Arg[3];
	pbData = (UINT8 *)rArg.ai4Arg[4];
    u2ByteCnt = (UINT16)rArg.ai4Arg[5];

	   //parameter check
    if ((pbData == NULL) || (u2ByteCnt == 0))
    {
        return MTR_PARAMETER_ERROR;
    }

    pu1KernelData = kmalloc(u2ByteCnt, GFP_KERNEL);
    if (!pu1KernelData)
    {
        return MTR_PARAMETER_ERROR;
    }
    
    MTPERIPHERAL_PRINT(" - _MTSIFSW_X_Read"
		"u1DevAddr = 0x%x\n"
        "u1WordAddrNum = 0x%x\n",
		"u4WordAddr = 0x%x"
		"u2ByteCnt = %d\n",
		u1DevAddr,  
		u1WordAddrNum ,
		u4WordAddr, 
		u2ByteCnt);

	u1Ret= SIFSW_X_Read((UINT8)0, u2ClkDiv,u1DevAddr, u1WordAddrNum, u4WordAddr, pu1KernelData, u2ByteCnt);
    
    if(copy_to_user((void __user *)pbData, (const void *)pu1KernelData, u2ByteCnt))
	{
        kfree(pu1KernelData);
        return MTR_NOT_OK;
	}

    if (pu1KernelData) 
    {
        kfree(pu1KernelData);
    }
    if (u1Ret == TRUE)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;
    }

}

//-------------------------------------------------------------------------
/** MTSIFSW_X_Write
 * I2C read command (general type)
 *
 * @param bDevAddr device address
 * @param bSubAddr sub-address
 * @param bDataCount numbber of bytes to be read
 * @param pbData pointer of data to be read
 * @param bOption options for this command
 * @param bOption bit 2-0: bit delay (us)
 * @param bOption bit 5-3: UINT8 delay (us)
 * @param bOption bit 6: need to check ACK
 * @param bOption bit 7: Reserve
 * @param bOption bit 8: halt and wait for ACK (full implementation of spec ch.7.2)
 * @return 1 or 0
 * @retval = 1 read success
 * @retval = 0 read fail
 */
 //-------------------------------------------------------------------------
static MT_RESULT_T  _MTSIFSW_X_Write (unsigned long arg)
{

	MTAL_IOCTL_6ARG_T rArg;
	UINT16 u2ClkDiv;
    UINT8 u1Ret;
	UINT8 u1DevAddr;
	UINT8 u1WordAddrNum;
	UINT32 u4WordAddr;
	UINT8 * pu1KernelData = NULL;
	UINT8 *pu1Buf = NULL;
	UINT16 u2ByteCnt;
	
	USR_SPACE_ACCESS_VALIDATE_6ARG(arg);
    COPY_FROM_USER_6ARG(arg, rArg);

	u2ClkDiv = (UINT16)rArg.ai4Arg[0];
    u1DevAddr = (UINT8)rArg.ai4Arg[1];
    u1WordAddrNum = (UINT8)rArg.ai4Arg[2];
    u4WordAddr = (UINT32)rArg.ai4Arg[3];
    pu1Buf = (UINT8 *)rArg.ai4Arg[4];
	u2ByteCnt = (UINT16)rArg.ai4Arg[5];
	if ((pu1Buf == NULL) || (u2ByteCnt == 0))
    {
        return MTR_PARAMETER_ERROR;
    }
	 pu1KernelData = kmalloc(u2ByteCnt, GFP_KERNEL);
	if (!pu1KernelData)
    {
        return MTR_PARAMETER_ERROR;
    }
    
    if (copy_from_user((void *) pu1KernelData, (const void __user *) pu1Buf, u2ByteCnt))
    {        
        kfree(pu1KernelData);
        return MTR_PARAMETER_ERROR;
    }
	 MTPERIPHERAL_PRINT(" - _MTSIFSW_X_Write"
		"u2ClkDiv = 0x%x\n"
        "u1DevAddr = 0x%x\n",
        "u1WordAddrNum[0] = 0x%x\n"
		"u4WordAddr = 0x%x"
		"u2Delay = %d\n",
		u2ClkDiv,  
		u1DevAddr ,
		u1WordAddrNum,
		u4WordAddr, 
		u2ByteCnt);
	 //add driver implementation code here
	u1Ret= SIFSW_X_Write((UINT8 )0, u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,pu1Buf,u2ByteCnt);
	
	 if (pu1KernelData) 
    {
        kfree(pu1KernelData);
    }
    if (u1Ret == TRUE)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;
    }
}
//SIFSW
//-------------------------------------------------------------------------
/** _MTSIFSW_Remove
 *  terminate SIF module.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
MT_RESULT_T  _MTSIFSW_Remove (VOID)
{
	SIFSW_Remove();
    return MTR_OK;
}
//-------------------------------------------------------------------------
/** MTBUS_Init
 *  Initialize SPI module for Demod MT5135.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static MT_RESULT_T  _MTBUS_Init (VOID)
{
    MTPERIPHERAL_PRINT(" - void \n");

     //add driver implementation code here
    SPIBus_Init();

    return MTR_OK;
}

//RTC
//----------------------------------------------------------------------------
/** MTRTC_RtcToUtc() Translate RTC to UTC
 *  @param prRtc, Input, The RTC to be translated
 *  @param pu8Utc, Output, The translated UTC
 *  @retval MTR_OK, Succeed; MTR_NOT_OK, Fail
 */
//----------------------------------------------------------------------------
static MT_RESULT_T  _MTRTC_RtcToUtc (unsigned long arg)
{
     MTAL_IOCTL_3ARG_T rArg;
     UINT64 *pu8Utc=NULL;
     const MTRTC_T *prRtc=NULL;
     UINT64 u8Utc;
     BOOL fret;
     MT_RESULT_T mtret;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    pu8Utc = (UINT64 *)rArg.ai4Arg[0];
    prRtc=(MTRTC_T *)(&rArg.ai4Arg[1]);

     //add driver implementation code here
     fret =RTC_RtcToUtc(&u8Utc, (RTC_T *)prRtc);
     mtret = fret?MTR_OK:MTR_NOT_OK;

    USR_SPACE_ACCESS_VALIDATE_ARG(pu8Utc, UINT64);
    COPY_TO_USER_ARG(pu8Utc, u8Utc, UINT64);

    MTPERIPHERAL_PRINT(" -u8Utc = 0x%08x08%x\n",(INT32)(u8Utc>>32),(INT32)u8Utc);
    MTMPLAYER_RTC_PRINT("prRtc", prRtc);

    return mtret;
}

//----------------------------------------------------------------------------
/** MTRTC_UtcToRtc() Translate UTC to RTC
 *  @param u8Utc, Input, The UTC to be translated
 *  @param prRtc, Output, The translated RTC
 *  @retval MTR_OK, Succeed; MTR_NOT_OK, Fail.
 */
//----------------------------------------------------------------------------
static MT_RESULT_T  _MTRTC_UtcToRtc (unsigned long arg)
{
     MTAL_IOCTL_3ARG_T rArg;
     MTRTC_T *prRtc=NULL;
     UINT64  u8Utc;
     MTRTC_T rRtc;
     BOOL fret;
     MT_RESULT_T mtret;

     USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
     COPY_FROM_USER_3ARG(arg, rArg);

    prRtc=(MTRTC_T *)rArg.ai4Arg[0];
    u8Utc = *(UINT64*)(&rArg.ai4Arg[1]);

 //add driver implementation code here
    fret = RTC_UtcToRtc((RTC_T *)&rRtc, u8Utc);
    mtret = fret?MTR_OK:MTR_NOT_OK;

    USR_SPACE_ACCESS_VALIDATE_ARG(prRtc, MTRTC_T);
    COPY_TO_USER_ARG(prRtc, rRtc, MTRTC_T);

    MTPERIPHERAL_PRINT(" -u8Utc = 0x%08x08%x\n",(INT32)(u8Utc>>32),(INT32)u8Utc);
    MTMPLAYER_RTC_PRINT("prRtc", prRtc);

    return MTR_OK;
}

//----------------------------------------------------------------------------
/** MTRTC_Init() Initialize RTC driver and hardware
 *  @retval MTR_OK, Succeed; MTR_NOT_OK, Fail.
 */
//----------------------------------------------------------------------------
static MT_RESULT_T  _MTRTC_Init (VOID)
{
     MTPERIPHERAL_PRINT(" - void \n");

      //add driver implementation code here
     RTC_Init();

     RTC_HookAlertHandler((PFN_RTC_ALERT_HANDLER_T)_RtcAlertHandler);

     return MTR_OK;
}

//----------------------------------------------------------------------------
/** MTRTC_GetTimeDate() Get time from RTC
 *  @param prTime, Output, The time
 *  @retval MTR_OK, Succeed; MTR_NOT_OK, Fail.
 */
//----------------------------------------------------------------------------
static MT_RESULT_T  _MTRTC_GetTimeDate (unsigned long arg)
{
    const UINT64 *prTime;
    UINT64  rTime;

    prTime =(UINT64*)arg;  

  //add driver implementation code here
    RTC_GetTimeDate( &rTime );

    USR_SPACE_ACCESS_VALIDATE_ARG(prTime,UINT64);
    COPY_TO_USER_ARG(prTime,rTime,UINT64);
    MTPERIPHERAL_PRINT(" - rTime = 0x%08x08%x\n",(INT32)(rTime>>32),(INT32)rTime);

    return MTR_OK;
}

//----------------------------------------------------------------------------
/** MTRTC_SetTimeDate() Set time to RTC
 *  @param prTime, Input, The time to set.
 *  @retval MTR_OK, Succeed; MTR_NOT_OK, Fail.
 */
//----------------------------------------------------------------------------
static MT_RESULT_T  _MTRTC_SetTimeDate (unsigned long arg)
{
    const UINT64 *prTime;
    UINT64 rTime;
    prTime = (UINT64 *)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(prTime,UINT64);
    COPY_FROM_USER_ARG(prTime,rTime,UINT64);   

    MTPERIPHERAL_PRINT(" - rTime = 0x%08x08%x\n",(INT32)(rTime>>32),(INT32)rTime);

    RTC_SetTimeDate( &rTime );

    return MTR_OK;
}

//----------------------------------------------------------------------------
/** MTRTC_SetAlertTime() Set time to RTC
 *  @param prTime, Input, The time to set.
 *  @retval MTR_OK, Succeed; MTR_NOT_OK, Fail.
 */
//----------------------------------------------------------------------------
static MT_RESULT_T  _MTRTC_SetAlertTime (unsigned long arg)
{
    UINT64* pu8Utc;
    UINT64 u8Utc;

    pu8Utc = (UINT64*)arg;
    USR_SPACE_ACCESS_VALIDATE_ARG(pu8Utc,UINT64);
    COPY_FROM_USER_ARG(pu8Utc,u8Utc,UINT64); 
    MTPERIPHERAL_PRINT(" -u8Utc = 0x%08x08%x\n",(INT32)(u8Utc>>32),(INT32)u8Utc);

      //add driver implementation code here
    RTC_SetAlertTime(u8Utc);

    return MTR_OK;
}

//----------------------------------------------------------------------------
/** MTRTC_GetAlertTime() Set time to RTC
 *  @param prTime, Input, The time to set.
 *  @retval MTR_OK, Succeed; MTR_NOT_OK, Fail.
 */
//----------------------------------------------------------------------------
static MT_RESULT_T  _MTRTC_GetAlertTime (unsigned long arg)
{
    UINT64 *prTime=NULL;
    INT64  rTime = 0;

    prTime =(INT64*)arg;

  //add driver implementation code here
    RTC_GetAlertTime( prTime );

    USR_SPACE_ACCESS_VALIDATE_ARG(prTime,INT64);
    COPY_TO_USER_ARG(prTime,rTime,INT64);

    MTPERIPHERAL_PRINT(" - rTime = 0x%08x08%x\n",(INT32)(rTime>>32),(INT32)rTime);
    return MTR_OK;
}

//----------------------------------------------------------------------------
/** MTRTC_HookAlertHandler()
 *  @param pfnHandler the callback function.
 */
//----------------------------------------------------------------------------
static MT_RESULT_T  _MTRTC_HookAlertHandler (unsigned long arg)
{
      MTPFN_RTC_ALERT_HANDLER_T pfnHandler;
      pfnHandler = (MTPFN_RTC_ALERT_HANDLER_T)arg;

      MTPERIPHERAL_PRINT(" - pfnHandler \n");

        //add driver implementation code here
      RTC_HookAlertHandler( pfnHandler);

      return MTR_OK;
}

//----------------------------------------------------------------------------
/** MTRTC_SetWakeup()
 *  @param pfnHandler the callback function.
 */
//----------------------------------------------------------------------------
static MT_RESULT_T  _MTRTC_SetWakeup (unsigned long arg)
{
    MTPFN_RTC_WEAKEUP_HANDLER_T *pfnHandler=NULL;
    pfnHandler =(MTPFN_RTC_WEAKEUP_HANDLER_T *)arg;

    MTPERIPHERAL_PRINT(" - pfnHandler \n");

      //add driver implementation code here
    RTC_SetWakeup((UINT32 *)pfnHandler);

    return MTR_OK;
}

//NOR
//-----------------------------------------------------------------------------
/** to erase data to 0xff on nor flash.
 *  @param u4SectIdx  Sector Index
 *  @param u4SectNum  Sector Numer
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNOR_Erase(unsigned long arg)
{
#if 0
     MTAL_IOCTL_2ARG_T rArg;
     UINT32 u4SectIdx;
     UINT32 u4SectNum;
     INT32 fret;
     MT_RESULT_T mtRet;

     USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
     COPY_FROM_USER_2ARG(arg, rArg);

     u4SectIdx=( UINT32 )rArg.ai4Arg[0];
     u4SectNum=(UINT64 )rArg.ai4Arg[1];

     MTPERIPHERAL_PRINT(" - u4SectIdx = 0x%x, u4SectNum = 0x%x\n",
		u4SectIdx,
		u4SectNum);

  //add driver implementation code here
    fret = NOR_Erase( u4SectIdx,  u4SectNum);
    mtRet=(fret==0)?MTR_OK:MTR_NOT_OK;

    return mtRet;
#else
    return MTR_OK;
#endif        
}

//-----------------------------------------------------------------------------
/** erase data to 0xff on nor flash.
 *  @param u4Offset  Offset that start to write
 *  @param u4ByteCount  Size in byte of writting
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNOR_EraseAddr(unsigned long arg)
{
#if 0
     MTAL_IOCTL_2ARG_T rArg;
     UINT32 u4Offset;
     UINT32 u4ByteCount;
     INT32 fret;
     MT_RESULT_T mtRet;
     
     USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
     COPY_FROM_USER_2ARG(arg, rArg);

     u4Offset=( UINT32 )rArg.ai4Arg[0];
     u4ByteCount=(UINT32 )rArg.ai4Arg[1];
     MTPERIPHERAL_PRINT(" - u4Offset = 0x%x, u4ByteCount = 0x%x\n",
 		u4Offset,
 		u4ByteCount);

  //add driver implementation code here
    fret = NOR_EraseAddr( u4Offset,  u4ByteCount);
    mtRet=(fret==0)?MTR_OK:MTR_NOT_OK;

    return mtRet;
#else
    return MTR_OK;
#endif        
}

//-----------------------------------------------------------------------------
/** get the size of norflash by bytes.
 *  @return Nor flash size in byte
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNOR_GetSize(unsigned long arg)
{
#if 0
    UINT32 *pu4Val;
    UINT32   u4Val;
    INT32 rArg;
    MT_RESULT_T mtRet;

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg,rArg);

    pu4Val=(UINT32 *)arg;

  //add driver implementation code here
    u4Val = NOR_GetSize();
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Val,UINT32);
    COPY_TO_USER_ARG(pu4Val,u4Val,UINT32);

    MTPERIPHERAL_PRINT(" - size = 0x%x \n",u4Val);
    return MTR_OK;
#else
    return MTR_OK;
#endif        
}

//-----------------------------------------------------------------------------
/** read data from nor flash.
 *  @param u4Offset  Position that start to read from
 *  @param pu1Buf    Pointer to storing read-back data
 *  @param u4Len     Length (i.e. size) to read in byte
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNOR_Read(unsigned long arg)
{
#if 0
     MTAL_IOCTL_3ARG_T rArg;
     UINT32 u4Offset;
     UINT8* pu1Buf=NULL;
     UINT8   u1Buf;
     UINT32 u4Len;
     INT32 fret;
     MT_RESULT_T mtRet;
     UINT8 *pu1DstBuf = NULL;

     USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
     COPY_FROM_USER_3ARG(arg, rArg);

    u4Offset=( UINT32 )rArg.ai4Arg[0];
    pu1Buf=(UINT8*)rArg.ai4Arg[1];
    u4Len=(UINT32)rArg.ai4Arg[2];

  //add driver implementation code here
    pu1DstBuf = kmalloc(u4Len, GFP_KERNEL);
    fret = NOR_Read( u4Offset,  pu1DstBuf,  u4Len);
    mtRet=(fret==0)?MTR_OK:MTR_NOT_OK;

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1Buf,UINT8);
    COPY_TO_USER_ARG(pu1Buf,u1Buf,UINT8);

    copy_to_user(pu1Buf,pu1DstBuf,u4Len);
    MTPERIPHERAL_PRINT(" - u4Offset = 0x%x,u4Len = 0x%x\n",
		u4Offset,
		u4Len);

    kfree(pu1DstBuf);
     return mtRet;
#else
    return MTR_OK;
#endif         
}

//-----------------------------------------------------------------------------
/** write data to nor flash.
 *  @param u4Offset  Position that start to write to
 *  @param pu1Buf    Pointer to storing write-to data
 *  @param u4Len     Length (i.e. size) to read in byte
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNOR_Write(unsigned long arg)
{
#if 0
     MTAL_IOCTL_3ARG_T rArg;
     UINT32 u4Offset;
     UINT8* pu1Buf=NULL;
     UINT32 u4Len;
     INT32 fret;
     MT_RESULT_T mtRet;
     UINT8 *pu1SrcBuf = NULL;

     USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
     COPY_FROM_USER_3ARG(arg, rArg);

    u4Offset=( UINT32 )rArg.ai4Arg[0];
    pu1Buf=(UINT8*)rArg.ai4Arg[1];
    u4Len=(UINT32)rArg.ai4Arg[2];
    MTPERIPHERAL_PRINT(" - u4Offset = 0x%x, u4Len = 0x%x\n",
		u4Offset,
		u4Len);

  //add driver implementation code here
    pu1SrcBuf = kmalloc(u4Len, GFP_KERNEL);
    copy_from_user(pu1SrcBuf,pu1Buf,u4Len);
    fret = NOR_Write( u4Offset,  pu1SrcBuf,  u4Len);
    mtRet=(fret==0)?MTR_OK:MTR_NOT_OK;
    kfree(pu1SrcBuf);

    return mtRet;
#else
    return MTR_OK;
#endif        
}

//-----------------------------------------------------------------------------
/** write data on nor flash and erase automaticallly if need.
 *  @param u4Offset  Position that start to write to
 *  @param pu1MemPtr    Pointer to storing write-to data
 *  @param u4Len     Length (i.e. size) to read in byte
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNOR_WrOnE(unsigned long arg)
{
#if 0
     MTAL_IOCTL_3ARG_T rArg;
     UINT32 u4Offset;
     UINT8* pu1MemPtr=NULL;
     UINT32 u4Len;
     INT32 fret;
     MT_RESULT_T mtRet;
     UINT8 *pu1SrcBuf = NULL;
     
     USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
     COPY_FROM_USER_3ARG(arg, rArg);

    u4Offset=( UINT32 )rArg.ai4Arg[0];
    pu1MemPtr=(UINT8*)rArg.ai4Arg[1];
    u4Len=(UINT32)rArg.ai4Arg[2];
    MTPERIPHERAL_PRINT(" - u4Offset = 0x%x, u4Len = 0x%x\n",
		u4Offset,
		u4Len);

    pu1SrcBuf = kmalloc(u4Len, GFP_KERNEL);
    copy_from_user(pu1SrcBuf,pu1MemPtr,u4Len);
  //add driver implementation code here
    fret = NOR_WrNoE( u4Offset,  pu1SrcBuf,  u4Len);
    mtRet = (fret==0)?MTR_OK:MTR_NOT_OK;
    
    kfree(pu1SrcBuf);
    return mtRet;
#else
    return MTR_OK;
#endif        
}

//-----------------------------------------------------------------------------
/** calculate CRC from input source
 *  @param u4Addr, Start Address
 *  @param u4Size, Size beginning from u4Addr
 *  @param pu4Checksum, Memory buffer storing check sum value
 *  @retval MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNOR_CalCRC(unsigned long arg)
{
#if 0
     MTAL_IOCTL_3ARG_T rArg;
     UINT32 u4Addr;
     UINT32 u4Size;
     UINT32 * pu4Checksum=NULL;
     UINT32 u4Checksum;
     INT32 fret;
     MT_RESULT_T mtRet;
     
     USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
     COPY_FROM_USER_3ARG(arg, rArg);

    u4Addr=( UINT32 )rArg.ai4Arg[0];
    u4Size=(UINT32)rArg.ai4Arg[1];
    pu4Checksum=(UINT32 *)rArg.ai4Arg[2];

  //add driver implementation code here
    fret = NOR_CalCRC( u4Addr,  u4Size,  &u4Checksum);
    mtRet=(fret==0)?MTR_OK:MTR_NOT_OK;
      
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Checksum,UINT32);
    COPY_TO_USER_ARG(pu4Checksum,u4Checksum,UINT32);
    MTPERIPHERAL_PRINT(" - u4Addr = 0x%x\n"
        "u4Size = 0x%x\n"
        "u4Checksum = 0x%x\n",
	 u4Addr,
	 u4Size,
	 u4Checksum);

    return mtRet;
#else
    return MTR_OK;
#endif
}

// NAND
//-----------------------------------------------------------------------------
/** Initialize the nand device and nand driver.
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNAND_Init(VOID)
{
#if 0
    MTPERIPHERAL_PRINT(" - void \n");
  //add driver implementation code here
    NAND_Init();

    return MTR_OK;
#else
    return MTR_OK;
#endif 
}

//-----------------------------------------------------------------------------
/** Reset the nand device.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNAND_Reset(VOID)
{
#if 0
    MTPERIPHERAL_PRINT(" - void \n");

      //add driver implementation code here
      NAND_Reset();

    return MTR_OK;
#else
    return MTR_OK;
#endif  
}

//-----------------------------------------------------------------------------
/** Read nand chip ID.
 *  @param pu4ID, chip ID
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNAND_ReadID(unsigned long arg)
{
#if 0
    UINT32* pu4ID=NULL;
    UINT32   u4ID;
     INT32 fret;
     MT_RESULT_T mtRet;
    UINT32 rArg;
     
    pu4ID =(UINT32*)arg;
    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg,rArg);

  //add driver implementation code here
    fret = NAND_ReadID( &rArg);
    mtRet=(fret==0)?MTR_OK:MTR_NOT_OK;

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4ID,UINT32);
    COPY_TO_USER_ARG(pu4ID,rArg,UINT32);

    MTPERIPHERAL_PRINT(" - ID = 0x%x\n",rArg);
    return mtRet;
#else
    return MTR_OK;
#endif    
}

//-----------------------------------------------------------------------------
/** Get NAND size.
 *  @return NAND size in page.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNAND_GetSize(unsigned long arg)
{
#if 0
    UINT32 *pu4Val;
    UINT32   u4Val;
     INT64  i8fet;
    UINT32 rArg;
     
    pu4Val=(UINT32*)arg;

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg,rArg);
  //add driver implementation code here
    i8fet = NAND_GetSize();

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Val,UINT32);
    COPY_TO_USER_ARG(pu4Val,i8fet,UINT32);

    MTPERIPHERAL_PRINT(" - size = 0x%x \n",i8fet);
    return MTR_OK;
#else
    return MTR_OK;
#endif    
}

//-----------------------------------------------------------------------------
/** NAND Read in block
 *  @param u4Offset, block offset to be read
 *  @param pu4Buf, data destination point
 *  @param u4Len, data destination buffer size
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 *
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNAND_ReadBlocks(unsigned long arg)
{
#if 0
     MTAL_IOCTL_3ARG_T rArg;
     UINT32 u4Offset;
     UINT8* pu1Buf=NULL;
     UINT8   u1Buf;
     UINT32 u4Len;
     INT32 fret;
     MT_RESULT_T mtRet;
     UINT8 *pu1DstBuf = NULL;
     
     USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
     COPY_FROM_USER_3ARG(arg, rArg);

     u4Offset=(UINT32)rArg.ai4Arg[0];
     pu1Buf=(UINT8 *)rArg.ai4Arg[1];
     u4Len=(UINT32)rArg.ai4Arg[2];

  //add driver implementation code here
     pu1DstBuf = kmalloc(u4Len, GFP_KERNEL);
     fret = NAND_Read( u4Offset,  pu1DstBuf,  u4Len);
    mtRet=(fret==0)?MTR_OK:MTR_NOT_OK;
    
     copy_to_user(pu1Buf,pu1DstBuf,u4Len);
    USR_SPACE_ACCESS_VALIDATE_ARG(pu1Buf,UINT8);

     MTPERIPHERAL_PRINT(" - u4Offset = 0x%x, u4Len = 0x%x\n",
		u4Offset,
		u4Len);

    kfree(pu1DstBuf);
    return MTR_OK;    
#else
    return MTR_OK;
#endif    
}

//-----------------------------------------------------------------------------
/** NAND Write in block
 *  @param u4Offset, block offset to be read
 *  @param pu4Buf, data destination point
 *  @param u4Len, data destination buffer size
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 *
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTNAND_WriteBlocks(unsigned long arg)
{
#if 0
     MTAL_IOCTL_3ARG_T rArg;
     UINT32 u4Offset;
     UINT8* pu1Buf=NULL;
     UINT32 u4Len;
     INT32 fret;
     MT_RESULT_T mtRet;
    UINT8 *pu1SrcBuf = NULL;
     
     USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
     COPY_FROM_USER_3ARG(arg, rArg);

    u4Offset=( UINT32 )rArg.ai4Arg[0];
    pu1Buf=(UINT8*)rArg.ai4Arg[1];
    u4Len=(UINT32)rArg.ai4Arg[2];
    MTPERIPHERAL_PRINT(" - u4Offset = 0x%x, u4Len = 0x%x\n",
		u4Offset,
             u4Len);

  //add driver implementation code here
    pu1SrcBuf = kmalloc(u4Len, GFP_KERNEL);
    copy_from_user(pu1SrcBuf,pu1Buf,u4Len);
    fret = NAND_EraseAddr(u4Offset,u4Len);
    fret = NAND_Write( u4Offset,  pu1SrcBuf,  u4Len);
    mtRet=(fret==0)?MTR_OK:MTR_NOT_OK;
    
    kfree(pu1SrcBuf);
    return MTR_OK;
#else
    return MTR_OK;
#endif        
}

EXTERN INT32 IRRC_Init(UINT32 u4PriType);
//IRRX
//-------------------------------------------------------------------------
/** MTIR_Init
 *  Initialize IRRX module.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static MT_RESULT_T  _MTIR_Init (MTIR_TYPE_T eType)
{
    UINT32 u4PriType;
	
	x_os_drv_set_timestamp("_MTIR_Init start");
#if 1
    if(eType == MTIR_TYPE_DEFAULT)
    {
        u4PriType = DRVCUST_OptGet(eIrrxType);
        printk("MTAL: Init IR Type: 0x%x\n", u4PriType);
    }
    else    
#endif    
    if(eType == MTIR_TYPE_NEC)
    {
        u4PriType = IRRX_NEC_TYPE;
    }
    else if(eType == MTIR_TYPE_RC56)
    {
        u4PriType = IRRX_RC5_TYPE;    
    }
    else if(eType == MTIR_TYPE_SHARP)
    {
        u4PriType = IRRX_SHARP_TYPE;    
    }    
    else
    {
        return MTR_NOT_SUPPORTED;
    }
    //if(IRRX_InitMtkIr(u4PriType) != IR_SUCC)
    if(IRRC_Init(u4PriType) != IR_SUCC)
    {
        return MTR_NOT_OK;
    }
	
	x_os_drv_set_timestamp("_MTIR_Init end");
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** IR Polling function
 *  @param pu4Key, 
 *  @param pu4Raw, 
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 *
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTIR_Poll(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32  *usr_pu4Key;
    UINT32  u4Key;
    //IRRX_RAW_DATA_T *usr_prRaw;
    MTIR_IRRX_RAW_DATA_T    *usr_prRaw;    
    IRRX_RAW_DATA_T rRaw;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    usr_pu4Key = (UINT32*)rArg.ai4Arg[0];
    usr_prRaw = (MTIR_IRRX_RAW_DATA_T*)rArg.ai4Arg[1];
    if (IRRX_PollMtkIr(&u4Key, &rRaw) == IR_FAIL)
    {
        //MTPERIPHERAL_PRINT("\nIRRX_PollMtkIr ===> IR_FAIL\n");
        return MTR_NOT_OK;
    }
    //MTPERIPHERAL_PRINT("\nIRRX_PollMtkIr ===> IR_Succ\n");
    
    USR_SPACE_ACCESS_VALIDATE_ARG(usr_pu4Key,UINT32);
    COPY_TO_USER_ARG(usr_pu4Key,u4Key,UINT32); 

    USR_SPACE_ACCESS_VALIDATE_ARG(usr_prRaw,MTIR_IRRX_RAW_DATA_T);
    COPY_TO_USER_ARG(usr_prRaw,rRaw,MTIR_IRRX_RAW_DATA_T);    
    return MTR_OK;
}

EXTERN INT32 IRRC_WaitMtkIr(UINT32* prBtnState, UINT32* prKey, IRRX_RAW_DATA_T* rRawData);
//-----------------------------------------------------------------------------
/** Waiting IR Event function
 *  @param pu4Key, 
 *  @param pu4Raw, 
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 *
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTIR_Wait(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    UINT32 u4BtnState;
    UINT32 u4Key;
    IRRX_RAW_DATA_T rRaw;    

    UINT32* usr_pu4Key;
    UINT32* usr_pu4BtnState;    
    MTIR_IRRX_RAW_DATA_T    *usr_prRaw;    
    
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    usr_pu4Key = (UINT32*)rArg.ai4Arg[0];
    usr_pu4BtnState = (UINT32*)rArg.ai4Arg[1];
    usr_prRaw = (MTIR_IRRX_RAW_DATA_T*)rArg.ai4Arg[2];
//static INT32 IRRX_WaitMtkIr(UINT32* prBtnState, UINT32* prKey, RAW_DATA_T* rRawData)    
    //MTPERIPHERAL_PRINT("IRRC_WaitMtkIr...\n");
    if (IRRC_WaitMtkIr(&u4BtnState, &u4Key, &rRaw) == IR_FAIL)
    {
        //MTPERIPHERAL_PRINT("\nIRRX_PollMtkIr ===> IR_FAIL\n");
        return MTR_NOT_OK;
    }
   // MTPERIPHERAL_PRINT("IRRC_WaitMtkIr BtnState(0x%8x), Key (0x%8x)\n", u4BtnState, u4Key);
    
    USR_SPACE_ACCESS_VALIDATE_ARG(usr_pu4Key,UINT32);
    COPY_TO_USER_ARG(usr_pu4Key,u4Key,UINT32); 

    USR_SPACE_ACCESS_VALIDATE_ARG(usr_pu4BtnState,UINT32);
    COPY_TO_USER_ARG(usr_pu4BtnState,u4BtnState,UINT32); 

    USR_SPACE_ACCESS_VALIDATE_ARG(usr_prRaw,MTIR_IRRX_RAW_DATA_T);
    COPY_TO_USER_ARG(usr_prRaw,rRaw,MTIR_IRRX_RAW_DATA_T);    
    return MTR_OK;
}

static MT_RESULT_T  _MTIR_GetRawData (unsigned long arg)
{  
    MTAL_IOCTL_2ARG_T rArg ;
    UINT32 *pu4IrRawData = NULL;
    UINT32 *pu4Repeat = NULL;
    UINT32 u4IrRawData = 0;
    UINT32 u4Repeat = FALSE;
    INT32 i4Ret = 0;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    pu4IrRawData = (UINT32*)rArg.ai4Arg[0];
    pu4Repeat = (UINT32*)rArg.ai4Arg[1];

    i4Ret = _IRRC_GetCurrentIrRawData(&u4IrRawData, (BOOL *)&u4Repeat);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4IrRawData, UINT32);
    COPY_TO_USER_ARG(pu4IrRawData, u4IrRawData, UINT32);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Repeat, UINT32);
    COPY_TO_USER_ARG(pu4Repeat, u4Repeat, UINT32);

    if (IR_SUCC == i4Ret)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;
    }
}
//-----------------------------------------------------------------------------
/** _MTIR_SetFilterSystemCode
 (BOOL fgEnable, BOOL fgPositiveFilter, UINT32 u4Num, UINT32 au4FilterArray[])
 *  @param fgEnable, en/dis RC56 system code fileter
 *  @param fgPositiveFilter,  only support positive filter now
 *  @param u4Num, the numbers of system code in au4FilterArray
 *  @param au4FilterArray,  system code array
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 *
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTIR_SetFilterSystemCode(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    UINT32 u4Enable;
    UINT32 u4fgPositiveFilter;
    UINT32 u4CodeCnt;
    UINT32 *pu4Data_User = NULL;
    UINT32 *pu4KernelBuf = NULL;     
    INT32 i4Ret;
    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    u4Enable = (UINT32)rArg.ai4Arg[0];
    u4fgPositiveFilter = (UINT32)rArg.ai4Arg[1];
    u4CodeCnt = (UINT32)rArg.ai4Arg[2];
    pu4Data_User = (UINT32 *)rArg.ai4Arg[3];

    MTPERIPHERAL_PRINT(
		"u4Enable = 0x%x\n"
		"u4fgPositiveFilter = 0x%x\n"
		"u4CodeCnt = 0x%x\n",
		u4Enable,
		u4fgPositiveFilter,
		u4CodeCnt);
    
    //parameter check
    if ((pu4Data_User == NULL) || (u4CodeCnt == 0))
    {
        return MTR_PARAMETER_ERROR;
    }
    //add driver implementation code here
    pu4KernelBuf = kmalloc(u4CodeCnt * sizeof(UINT32), GFP_KERNEL);
    if (!pu4KernelBuf)
    {
        return MTR_PARAMETER_ERROR;
    }
                
    if (copy_from_user((void *) pu4KernelBuf, (const void __user *) pu4Data_User, u4CodeCnt * sizeof(UINT32)))
    {        
        kfree(pu4KernelBuf);
		return MTR_NOT_OK;
    }
            
    //u2Ret = SIF_X_Write(mtDrv_SifBusId, (27000/u4ClkVal), u1DevAddr, u2AddrCnt, u4WordAddr, pu1KernelBuf, u2DataCnt);
    i4Ret = IRRX_FilterSystemCode(u4Enable, u4fgPositiveFilter, u4CodeCnt, pu4KernelBuf);
    
    if (pu4KernelBuf) 
    {
        kfree(pu4KernelBuf);
    }

    if (i4Ret == IR_SUCC)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;
    }    
}


//-----------------------------------------------------------------------------
/** Pinmux Set Function
 *  @param i4PinSel is the bit group of pinmuxers
 *  @param i4Func is the function we want to select
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 *
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTPINMUX_Set(unsigned long arg)
{
    INT32 i4PinSel;         INT32 i4Func;
    MTAL_IOCTL_2ARG_T rArg;

     USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
     COPY_FROM_USER_2ARG(arg, rArg);

    i4PinSel = rArg.ai4Arg[0];      i4Func= rArg.ai4Arg[1];

    MTPERIPHERAL_PRINT("In kernelL i4PinSel= %d, i4Func =%d\n", i4PinSel, i4Func);
    if( BSP_PinSet(i4PinSel, i4Func ) )
    {
        return MTR_NOT_OK;
    }
    
    return MTR_OK;
}

//-----------------------------------------------------------------------------
/** Check if it is secure boot IC or not
 *  @param pu4ScrBt, 1: Secure boot IC; 0: Non-Secure boot IC
 *  @retval MTR_OK, Succeed; MTR_NOT_OK, Fail
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTSCRBT_Check(unsigned long arg)
{
    UINT32 *pu4ScrBt;
    UINT32  u4ScrBt;    

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);

    pu4ScrBt =(UINT32 *)arg;

    #if defined(CC_MT5365) || defined(CC_MT5395)
    if (BIM_IsSecureBoot())
    {
    	u4ScrBt = 1;
    }
    else
    #endif    
    {
    	u4ScrBt = 0;    
    }
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4ScrBt, UINT32);
    COPY_TO_USER_ARG(pu4ScrBt, u4ScrBt, UINT32);

    MTPERIPHERAL_PRINT(" - SecureBoot = 0x%x \n", u4ScrBt);

    return MTR_OK;
}


//-----------------------------------------------------------------------------
/** _MTDRAM_GetBandwidthUsage() provide the total dram bandwidth usage
 *  @param u4MonitorSecond the monitor perioid in second.
 *  @param pu4Usage the total consumption in permillage
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTDRAM_GetBandwidthUsage (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4MonitorSecond;
    UINT32 *pu4Usage;
    UINT32  u4Usage;
    MT_RESULT_T ret = MTR_OK;
    UINT32 u4DelayClock;
    UINT64 u8Usage;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4MonitorSecond = (UINT32)rArg.ai4Arg[0];
    pu4Usage = (UINT32*)rArg.ai4Arg[1];

    // driver implementation start -----

    if (u4MonitorSecond == 0)
    {
        u4MonitorSecond = 1;
    }
#if defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389)
	u4DelayClock = BSP_GetDomainClock(SRC_MEM_CLK) * u4MonitorSecond;
#else
	u4DelayClock = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) * u4MonitorSecond;
#endif

#if defined(CC_MT5395) || defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389)
    u4Usage = SRM_DramMonitor(0,SRM_DRV_TOTAL, u4DelayClock, u4MonitorSecond * 1000);
#else
    u4Usage = SRM_DramMonitor(SRM_DRV_TOTAL, u4DelayClock, u4MonitorSecond * 1000);
#endif
    u8Usage = (UINT64)u4Usage;
    u8Usage *= 1000;
    u8Usage = u8Div6432(u8Usage, (UINT64)u4DelayClock, NULL);
    u4Usage = (UINT32)u8Usage;
    // driver implementation end -----

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Usage, UINT32);
    COPY_TO_USER_ARG(pu4Usage, u4Usage, UINT32);

    MTPERIPHERAL_PRINT(" - u4MonitorSecond = %d, pu4Usage = %d\n",
        u4MonitorSecond,
        *pu4Usage);

    return ret;
}

#ifdef CC_SUPPORT_SMC
//-----------------------------------------------------------------------------
/** _MTSMC_Init() provide the total dram bandwidth usage
 *  @param u4MonitorSecond the monitor perioid in second.
 *  @param pu4Usage the total consumption in permillage
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTSMC_Init (unsigned long arg)
{
    MT_RESULT_T rRet = MTR_OK;

    if(SMC_Init() != 0)
    {
        rRet = MTR_NOT_OK;
    }
    SMC_SetHotPlugCb(_SmcCallbackHandler);	
    
    return rRet;
}

//-----------------------------------------------------------------------------
/** _MTSMC_Init() provide the total dram bandwidth usage
 *  @param u4MonitorSecond the monitor perioid in second.
 *  @param pu4Usage the total consumption in permillage
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTSMC_GetCardPresence (unsigned long arg)
{
    MT_RESULT_T rRet = MTR_OK;
    UINT32 u4Status;

    u4Status = (UINT32)SMC_GetCardPresense();

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, UINT32);
    COPY_TO_USER_ARG(arg, u4Status, UINT32);

    return rRet;
}

//-----------------------------------------------------------------------------
/** _MTSMC_Reset() provide the total dram bandwidth usage
 *  @param u4MonitorSecond the monitor perioid in second.
 *  @param pu4Usage the total consumption in permillage
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTSMC_Reset (unsigned long arg)
{
    MT_RESULT_T rRet = MTR_OK;

    if(SMC_Reset() != 0)
    {
        rRet = MTR_NOT_OK;
    }

    return rRet;
}

//-----------------------------------------------------------------------------
/** _MTSMC_SendReceiveAPDU() provide the total dram bandwidth usage
 *  @param u4MonitorSecond the monitor perioid in second.
 *  @param pu4Usage the total consumption in permillage
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTSMC_SendReceiveAPDU (unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    MT_RESULT_T rRet = MTR_OK;
    UINT32 u4SendLen = 0, u4RecvLen = 0;
    static UINT8 au1SendBuf[254];
    static UINT8 au1RecvBuf[254];    

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);


    u4SendLen = (UINT8)rArg.ai4Arg[1];
    // Copy send APDU data from user space
    COPY_FROM_USER_ARG_SIZE(rArg.ai4Arg[0], au1SendBuf, u4SendLen);

    if(SMC_SendReceiveAPDU(0, 
        au1SendBuf, u4SendLen, au1RecvBuf, (UINT8*)&u4RecvLen) != 0)
    {
        rRet = MTR_NOT_OK;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(rArg.ai4Arg[3], UINT32);
    COPY_TO_USER_ARG(rArg.ai4Arg[3], u4RecvLen, UINT32);
 	
    USR_SPACE_ACCESS_VALIDATE_ARG_SIZE(rArg.ai4Arg[2], u4RecvLen);
    COPY_TO_USER_ARG_SIZE(rArg.ai4Arg[2], au1RecvBuf, u4RecvLen);

    MTPERIPHERAL_PRINT(" ");

    return rRet;
}

//-----------------------------------------------------------------------------
/** _MTSMC_SendReceiveAPDU() provide the total dram bandwidth usage
 *  @param u4MonitorSecond the monitor perioid in second.
 *  @param pu4Usage the total consumption in permillage
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTSMC_RequestIFS (unsigned long arg)
{
    MT_RESULT_T rRet = MTR_OK;

    if(SMC_RequestIFS((UINT8)arg) != 0)
    {
        rRet = MTR_NOT_OK;
    }    	

    MTPERIPHERAL_PRINT(" ");

    return rRet;
}

//-----------------------------------------------------------------------------
/** _MTSMC_SendReceiveAPDU() provide the total dram bandwidth usage
 *  @param u4MonitorSecond the monitor perioid in second.
 *  @param pu4Usage the total consumption in permillage
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTSMC_Activate (unsigned long arg)
{
    MT_RESULT_T rRet = MTR_OK;

    if(SMC_Activate() != 0)
    {
        rRet = MTR_NOT_OK;
    }    	

    MTPERIPHERAL_PRINT(" ");

    return rRet;
}

//-----------------------------------------------------------------------------
/** _MTSMC_SendReceiveAPDU() provide the total dram bandwidth usage
 *  @param u4MonitorSecond the monitor perioid in second.
 *  @param pu4Usage the total consumption in permillage
 *  @return MTR_OK successful,otherwise MTR_NOT_OK.
 */
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTSMC_Deactivate (unsigned long arg)
{
    MT_RESULT_T rRet = MTR_OK;

    if(SMC_Deactivate() != 0)
    {
        rRet = MTR_NOT_OK;
    }    	

    MTPERIPHERAL_PRINT(" ");

    return rRet;
}
#endif

//EEPROM
//-------------------------------------------------------------------------
/** MTEEPROM_Init
 *  Initialize SIF module.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static MT_RESULT_T  _MTEEPROM_Init (VOID)
{
    MTPERIPHERAL_PRINT(" - void \n");

     //add driver implementation code here
    EEPROM_Init();

    return MTR_OK;
}

//-------------------------------------------------------------------------
/** MTEEPROM_Read
 *  read data with word address from SIF module.
 *  @param  ePort   	SIF Port ID
 *  @param  u4ClkVal 	Demaned Sif Clock in Khz
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  pu1Addr 	Pointer to Serial interface word address.
 *                      *pu1Addr is sent first on bus.
 *  @param  u2AddrCnt 	Serial interface word address count (up to 3).
 *  @param  pu1Data 		Pointer to data
 *  @param  u2DataCnt 	Number of byte to read. Max = 256 bytes.
 *  @retval   MT_RESULT_T
 *            MTR_OK for OK
 *            MTR_PARAMETER_ERROR for wrong parameter, 
 *            MTR_NOT_OK for execution error
 */
//-------------------------------------------------------------------------
static MT_RESULT_T  _MTEEPROM_Read (unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    UINT32 u4Offset;
    UINT8 *pu1Data_User = NULL;
    UINT32 u4DataCnt;
    UINT8 * pu1KernelBuf = NULL;     
    INT32 i4Ret;    
    
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u4Offset = (UINT32)rArg.ai4Arg[0];
    pu1Data_User = (UINT8*)rArg.ai4Arg[1];
    u4DataCnt = (UINT32)rArg.ai4Arg[2];


    //parameter check
    if((pu1Data_User==NULL) || (u4DataCnt==0))
    {
        return MTR_PARAMETER_ERROR;
    }

    //add driver implementation code here
    pu1KernelBuf = kmalloc(u4DataCnt, GFP_KERNEL);
    if (!pu1KernelBuf)
    {
        return MTR_PARAMETER_ERROR;
    }
    
    i4Ret = EEPROM_Read((UINT64)u4Offset, (UINT32)pu1KernelBuf, u4DataCnt);

    if(copy_to_user((void __user *)pu1Data_User, (const void *)pu1KernelBuf, u4DataCnt))
	{
		kfree(pu1KernelBuf);
		return MTR_NOT_OK;
	}

    if (pu1KernelBuf) 
    {
        kfree(pu1KernelBuf);
    }

    if (i4Ret == 0)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;
    }
}

//-------------------------------------------------------------------------
/** MTEEPROM_Write
 *  write data with word address to SIF module.
 *  @param  ePort  	SIF Port ID
 *  @param  u4ClkVal 	Demaned Sif Clock in Khz
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  pu1Addr 	Pointer to Serial interface word address.
 *                      *pu1Addr is sent first on bus.
 *  @param  u2AddrCnt 	Serial interface word address count (up to 3).
 *  @param  pu1Data 	Pointer to data
 *  @param  u2DataCnt 	Number of byte to read. Max = 256 bytes.
 *  @retval   MT_RESULT_T
 *            MTR_OK for OK
 *            MTR_PARAMETER_ERROR for wrong parameter, 
 *            MTR_NOT_OK for execution error
 */
//-------------------------------------------------------------------------
static MT_RESULT_T  _MTEEPROM_Write (unsigned long arg )
{
    MTAL_IOCTL_3ARG_T rArg;
    UINT32 u4Offset;
    UINT8 *pu1Data_User = NULL;
    UINT32 u4DataCnt;
    UINT8 * pu1KernelBuf = NULL;     
    INT32 i4Ret;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u4Offset = (UINT32)rArg.ai4Arg[0];
    pu1Data_User = (UINT8*)rArg.ai4Arg[1];
    u4DataCnt = (UINT32)rArg.ai4Arg[2];
    
    //parameter check
    if ((pu1Data_User == NULL) || (u4DataCnt == 0))
    {
        return MTR_PARAMETER_ERROR;
    }

    //add driver implementation code here
    pu1KernelBuf = kmalloc(u4DataCnt, GFP_KERNEL);
    if (!pu1KernelBuf)
    {
        return MTR_PARAMETER_ERROR;
    }
        
    if (copy_from_user((void *) pu1KernelBuf, (const void __user *) pu1Data_User, u4DataCnt))
    {        
        kfree(pu1KernelBuf);
		return MTR_NOT_OK;
    }            
    
    i4Ret = EEPROM_Write((UINT64)u4Offset, (UINT32)pu1KernelBuf, u4DataCnt);

    if (pu1KernelBuf) 
    {
        kfree(pu1KernelBuf);
    }

    if (i4Ret == 0)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;
    }
}

//-------------------------------------------------------------------------
/** MTEEPROM_GetProperty
 *  read data with word address from SIF module.
 *  @param  ePort   	SIF Port ID
 *  @param  u4ClkVal 	Demaned Sif Clock in Khz
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  pu1Addr 	Pointer to Serial interface word address.
 *                      *pu1Addr is sent first on bus.
 *  @param  u2AddrCnt 	Serial interface word address count (up to 3).
 *  @param  pu1Data 		Pointer to data
 *  @param  u2DataCnt 	Number of byte to read. Max = 256 bytes.
 *  @retval   MT_RESULT_T
 *            MTR_OK for OK
 *            MTR_PARAMETER_ERROR for wrong parameter, 
 *            MTR_NOT_OK for execution error
 */
//-------------------------------------------------------------------------
static MT_RESULT_T  _MTEEPROM_GetProperty (unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4Size;    
    UINT32 u4MwOffset;
    UINT8 *pu1Size_User = NULL;
    UINT8 *pu1MwOffset_User = NULL;    
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    pu1Size_User = (UINT8*)rArg.ai4Arg[0];
    pu1MwOffset_User = (UINT8*)rArg.ai4Arg[1];

    //parameter check
    if((pu1Size_User==NULL) || (pu1MwOffset_User==0))
    {
        return MTR_PARAMETER_ERROR;
    }

    u4Size = (INT32)EEPROM_GetSize();
    u4MwOffset = DRVCUST_InitGet(eEepromMwOffset);
    
    if(copy_to_user((void __user *)pu1Size_User, (const void *)&u4Size, sizeof(int)))
	{
		return MTR_NOT_OK;
	}
    if(copy_to_user((void __user *)pu1MwOffset_User, (const void *)&u4MwOffset, sizeof(int)))
	{
		return MTR_NOT_OK;
	}
    return MTR_OK;
}

//----------------------------------------------------------------------------
/** MTAGENT_SetBandwidthParam()
 *  @param u4Module, module number to control
 *  @param u4Type, register to control
 *  @param u4Value, value to set 
 *  @retval MTR_OK, Succeed; MTR_NOT_OK, Fail
 */
//----------------------------------------------------------------------------
static MT_RESULT_T _MTAGENT_SetBandwidthParam(unsigned long arg)
{
    UINT32 u4Module; 
    UINT32 u4Type; 
    UINT32 u4Value;
    MTAL_IOCTL_3ARG_T rArg;

     USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
     COPY_FROM_USER_3ARG(arg, rArg);

    u4Module = rArg.ai4Arg[0];      
    u4Type= rArg.ai4Arg[1];
    u4Value= rArg.ai4Arg[2];

    MTPERIPHERAL_PRINT("In kernelL u4Module= %d, u4Type =%d, u4Value =%d\n", u4Module, u4Type,u4Value);

    if(u4Module>=MAX_BW_MODULE_NUM)
    {
    	MTPERIPHERAL_PRINT("Set BW parameter module  type error!\n");
    	return MTR_PARAMETER_ERROR;
    }
    
    if((u4Module==BM_MODULE_OSD1)||(u4Module==BM_MODULE_OSD2))
    {
    	switch(u4Type)
    	{
    		case SBWP_OSD_CONT_REQ_LMT0:
    			OSD_PLA_SetContReqLmt0(u4Module+1,u4Value);
    			break;
    		case SBWP_OSD_PAUSE_CNT:
    			OSD_PLA_SetPauseCnt(u4Module+1,u4Value);
    			break;
    		case SBWP_OSD_VAC_TH:
    			OSD_PLA_SetVacTh(u4Module+1,u4Value);
    			break;
    		case SBWP_OSD_FIFO_SIZE:
    			OSD_PLA_SetFifoSize(u4Module+1,u4Value);
    			break;
    		case SBWP_OSD_CONT_REQ_LMT:
    			OSD_PLA_SetContReqLmt(u4Module+1,u4Value);
    			break;
    		default:
    			MTPERIPHERAL_PRINT("Set BW parameter reg type error!\n");
    			return MTR_PARAMETER_ERROR;
    				
    	}
    }
    
    
    return MTR_OK;
}



int mtal_ioctl_mtperipheral(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
	switch (cmd)
	{
		case MTAL_IO_MTPERIPHERAL_GPIO_SETDIRECTION:
                    return  _MTGPIO_SetDirection(arg);
		case MTAL_IO_MTPERIPHERAL_GPIO_GETDIRECTION:
                    return  _MTGPIO_GetDirection(arg);
        case MTAL_IO_MTPERIPHERAL_I2C_CHECK:
            return  _MTI2C_Check(arg);            
		case MTAL_IO_MTPERIPHERAL_GPIO_INTRQ:
                    return  _MTGPIO_Intrq(arg);
		case MTAL_IO_MTPERIPHERAL_GPIO_QUERY:
                    return  _MTGPIO_Query(arg);
		case MTAL_IO_MTPERIPHERAL_GPIO_REG:
                    return  _MTGPIO_Reg(arg);
		case MTAL_IO_MTPERIPHERAL_GPIO_SETOUT:
                    return  _MTGPIO_SetOut(arg);
		case MTAL_IO_MTPERIPHERAL_GPIO_GETIN:
                    return  _MTGPIO_GetIn(arg);
		case MTAL_IO_MTPERIPHERAL_PWM_SETFREQUENCY:
                    return  _MTPWM_SetFrequency(arg);
		case MTAL_IO_MTPERIPHERAL_PDWNC_READSERVOVALUE:
                    return  _MTPDWNC_ReadServoADCChannelValue(arg);
		case MTAL_IO_MTPERIPHERAL_SIF_INIT:
                    return  _MTSIF_Init();
		case MTAL_IO_MTPERIPHERAL_SIF_STOP:
                    return  _MTSIF_Stop();
		case MTAL_IO_MTPERIPHERAL_SIF_WRITEMULTIPLE:
                    return  _MTSIF_WriteMultipleSubAddr(arg);
		case MTAL_IO_MTPERIPHERAL_SIF_READMULTIPLE:
                    return  _MTSIF_ReadMultipleSubAddr(arg);
		case MTAL_IO_MTPERIPHERAL_SIFSW_INIT:
					return	_MTSIFSW_Init(arg);
		case MTAL_IO_MTPERIPHERAL_SIFSW_WRITEDATA:
					return	_MTSIFSW_X_Write(arg);
		case MTAL_IO_MTPERIPHERAL_SIFSW_READDATA:
					return	_MTSIFSW_X_Read(arg);
		case MTAL_IO_MTPERIPHERAL_SIFSW_REMOVE:
					return	_MTSIFSW_Remove();
		case MTAL_IO_MTPERIPHERAL_RTC_RTCTOUTC:
                    return  _MTRTC_RtcToUtc(arg);
		case MTAL_IO_MTPERIPHERAL_RTC_UTCTORTC:
                    return  _MTRTC_UtcToRtc(arg);
		case MTAL_IO_MTPERIPHERAL_RTC_INIT:
                    return  _MTRTC_Init();
		case MTAL_IO_MTPERIPHERAL_RTC_GETTIMEDATA:
                    return  _MTRTC_GetTimeDate(arg);
		case MTAL_IO_MTPERIPHERAL_RTC_SETTIMEDATA:
                    return  _MTRTC_SetTimeDate(arg);
		case MTAL_IO_MTPERIPHERAL_RTC_SETALERTTIME:
                    return  _MTRTC_SetAlertTime(arg);
		case MTAL_IO_MTPERIPHERAL_RTC_GETALERTTIME:
                    return  _MTRTC_GetAlertTime(arg);
		case MTAL_IO_MTPERIPHERAL_RTC_HOOKALERTHANDLER:
                    return  _MTRTC_HookAlertHandler(arg);
		case MTAL_IO_MTPERIPHERAL_RTC_SETWAKEUP:
                    return  _MTRTC_SetWakeup(arg);
		case MTAL_IO_MTPERIPHERAL_NOR_ERASE:
                    return  _MTNOR_Erase(arg);
		case MTAL_IO_MTPERIPHERAL_NOR_ERASEADDR:
                    return  _MTNOR_EraseAddr(arg);
		case MTAL_IO_MTPERIPHERAL_NOR_GETSIZE:
                    return  _MTNOR_GetSize(arg);
		case MTAL_IO_MTPERIPHERAL_NOR_READ:
                    return  _MTNOR_Read(arg);
		case MTAL_IO_MTPERIPHERAL_NOR_WRITE:
                    return  _MTNOR_Write(arg);
		case MTAL_IO_MTPERIPHERAL_NOR_WRONE:
                    return  _MTNOR_WrOnE(arg);
		case MTAL_IO_MTPERIPHERAL_NOR_CALCRC:
                    return  _MTNOR_CalCRC(arg);
		case MTAL_IO_MTPERIPHERAL_NAND_INIT:
                    return  _MTNAND_Init();
		case MTAL_IO_MTPERIPHERAL_NAND_RESET:
                    return  _MTNAND_Reset();
		case MTAL_IO_MTPERIPHERAL_NAND_READID:
                    return  _MTNAND_ReadID(arg);
		case MTAL_IO_MTPERIPHERAL_NAND_GETSIZE:
                    return  _MTNAND_GetSize(arg);
		case MTAL_IO_MTPERIPHERAL_NAND_READBLOCKS:
                    return  _MTNAND_ReadBlocks(arg);
		case MTAL_IO_MTPERIPHERAL_NAND_WRITEBLOCKS:
                    return  _MTNAND_WriteBlocks(arg);
		case MTAL_IO_MTPERIPHERAL_IR_INIT:
                    return  _MTIR_Init(arg);
		case MTAL_IO_MTPERIPHERAL_IR_POLL:
                    return  _MTIR_Poll(arg);                    
		case MTAL_IO_MTPERIPHERAL_IR_WAIT:
                    return  _MTIR_Wait(arg);                    
        case MTAL_IO_MTPERIPHERAL_IR_GETTouchPadRAWDATA:
            return   _MTIR_GetTouchPadRawData(arg); 
        case MTAL_IO_MTPERIPHERAL_IR_GETRAWDATA:
            return  _MTIR_GetRawData(arg); 
        case MTAL_IO_MTPERIPHERAL_IR_SET_RC56_SYSTEM_CODE:
                    return  _MTIR_SetFilterSystemCode(arg);
		case MTAL_IO_MTPERIPHERAL_PINMUX_SET:
                    return  _MTPINMUX_Set(arg);                    
		case MTAL_IO_MTPERIPHERAL_POWER_ENTERSTANDBY:
                    return  _MTPDWNC_EnterStandby(arg);                    
		case MTAL_IO_MTPERIPHERAL_T8032_CMD:
                    return  _MTPDWNC_T8032Cmd(arg);
		case MTAL_IO_MTPERIPHERAL_POWER_WAKEUP_REASON:
                    return  _MTPDWNC_GetWakeupReason(arg);
		case MTAL_IO_MTPERIPHERAL_POWER_SETWAKEUPSOURCE:
                    return  _MTPDWNC_SetWakeupSource(arg);                      
		case MTAL_IO_MTPERIPHERAL_GPIO_PERIODLEVELCALLBACK:
                    return  _MTGPIO_PeriodLevelCallback(arg);
		case MTAL_IO_MTPERIPHERAL_GPIO_INIT:
                    return  _MTGPIO_Init();
		case MTAL_IO_MTPERIPHERAL_WD_THREAD:
					return   _MTWD_Init();
		case MTAL_IO_MTPERIPHERAL_PWM_SETPHASE:
                    return  _MTPWM_SetPhase(arg);
		case MTAL_IO_MTPERIPHERAL_SCRBT_CHECK:
                    return  _MTSCRBT_Check(arg);
		case MTAL_IO_MTPERIPHERAL_DRAM_GETBANDWIDTHUSAGE:
                    return  _MTDRAM_GetBandwidthUsage(arg);
#ifdef CC_SUPPORT_SMC
		case MTAL_IO_MTPERIPHERAL_SMC_INIT:
                    return  _MTSMC_Init(arg);     
		case MTAL_IO_MTPERIPHERAL_SMC_GETCARDPRESENCE:
                    return  _MTSMC_GetCardPresence(arg);                      
		case MTAL_IO_MTPERIPHERAL_SMC_RESET:
                    return  _MTSMC_Reset(arg);                              
		case MTAL_IO_MTPERIPHERAL_SMC_SENDRECEIVEAPDU:
                    return  _MTSMC_SendReceiveAPDU(arg);
		case MTAL_IO_MTPERIPHERAL_SMC_REQUESTIFS:
                    return  _MTSMC_RequestIFS(arg);   
		case MTAL_IO_MTPERIPHERAL_SMC_ACTIVATE:
                    return  _MTSMC_Activate(arg);
		case MTAL_IO_MTPERIPHERAL_SMC_DEACTIVATE:
                    return  _MTSMC_Deactivate(arg);
#endif                     
		case MTAL_IO_MTPERIPHERAL_EEPROM_INIT:
                    return  _MTEEPROM_Init();   
		case MTAL_IO_MTPERIPHERAL_EEPROM_READ:
                    return  _MTEEPROM_Read(arg);                                       
		case MTAL_IO_MTPERIPHERAL_EEPROM_WRITE:
                    return  _MTEEPROM_Write(arg);                     
		case MTAL_IO_MTPERIPHERAL_EEPROM_GETPROPERTY:
                    return  _MTEEPROM_GetProperty(arg); 
                case MTAL_IO_MTPERIPHERAL_SET_BW_PARAMETERS:
                    return  _MTAGENT_SetBandwidthParam(arg);                     
        case MTAL_IO_MTPERIPHERAL_BUS_TUNER_INIT:
            return  _MTBUS_Init();          
		default:
                    return  MTR_NOT_OK;
      }
	return MTR_OK;
 }

