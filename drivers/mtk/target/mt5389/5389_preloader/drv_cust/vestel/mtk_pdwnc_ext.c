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

#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_ckgen.h"
#include "x_timer.h"
#include "x_debug.h"
#include "x_hal_arm.h"
#include "x_dram.h"
#include "pdwnc_if.h"
#include "eeprom_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#ifndef CC_PSEUDO_WATCHDOG_ASSERT
#define CC_PSEUDO_WATCHDOG_ASSERT           0
#endif /* CC_PSEUDO_WATCHDOG_ASSERT */

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------

void PDWNC_MTK_Init(void);
void PDWNC_MTK_WatchDogInit(void);
void PDWNC_MTK_ErrorHandleInit(void);
void PDWNC_MTK_ReadWakeupReason(UINT32* pu4Reason);
void PDWNC_MTK_SetupPowerDown(UINT32 u4PowerDownEvent,  UINT32 *pu4PowerDownEn);
void PDWNC_MTK_ErrorCode(UINT8 u1ErrorCode);
INT32 PDWNC_SetLedErrorCodeBlinking(UINT8 u1times);
INT32 PDWNC_SetLedErrorCode(UINT8 u1Period, UINT8 u1Interval);
INT32 PDWNC_MTK_Set8032uPLedBlink(UINT32 u4BlinkTimes);
//-----------------------------------------------------------------------------
// Imported funtions
//-----------------------------------------------------------------------------
EXTERN UINT32 _PDWNC_ReadWakeupStatus(void);
EXTERN UINT32 _PDWNC_ReadWatchDogStatus(void);

#define MTK_ERR_LED_BLINK_TIME      5
#define MTK_ERR_LED_BLINK_INVERAL   3

#ifndef CC_MTK_PRELOADER
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static PDWNC_FuncTbl rPDWNC_EXT_FuncTbl =
{
    PDWNC_MTK_Init,             // pfnInit
    PDWNC_MTK_SetupPowerDown,   // pfnPowerDown
    PDWNC_MTK_ReadWakeupReason, // pfnReadWakeupReason
    PDWNC_MTK_WatchDogInit,     // pfnWatchDogInit;
    PDWNC_MTK_ErrorHandleInit,  // pfnErrorHandleInit
    PDWNC_MTK_ErrorCode,        // pfnWriteErrorCode
    NULL,                       // pfnReadErrorCode
    NULL,                       // pfnGetCurrentState
    NULL,                       // pfnDetermineNextState
    NULL,                       // pfnEnterNextState
    PDWNC_MTK_Set8032uPLedBlink, //pfnSet8032uPLedBlink
    NULL,                       // pfnCustomRead
    NULL                        // pfnCustomWrite
};

static UINT32 _fgPseudoWatchdogAssert = CC_PSEUDO_WATCHDOG_ASSERT;
static UINT32 TimerDimmingFreq = 5;
static UINT32 GpioNumberLed ;
static UINT32 LedContronOnVal ;
#ifndef CC_LOADER_SHRINK_LED
static PDWNC_T8032_CMD_T rCmd;
#endif
UINT32 PDWNC_SetupPseudoWatchdogAssert(UINT32 *pu4Flag)
{
    UNUSED(pu4Flag);
    if (pu4Flag != NULL)
    {
        _fgPseudoWatchdogAssert = *pu4Flag;
    }
    return _fgPseudoWatchdogAssert;
}
void _PDWNC_ResumeT8032ProtectionMode(void)
{
#if 0
    PDWNC_T8032_CMD_T rCmd;
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_RESUME_POWER_PROTECT;
    rCmd.au1Data[0] = 1;//start communication with T8032
    PDWNC_T8032Cmd(&rCmd, NULL);
#endif
}

void _PDWNC_StopT8032ProtectionMode(void)
{
#if 0
    PDWNC_T8032_CMD_T rCmd;
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_STOP_POWER_PROTECT;
    rCmd.au1Data[0] = 1;//start communication with T8032
    PDWNC_T8032Cmd(&rCmd, NULL);
#endif
}

BOOL active_standby_state = 0;


void PDWNC_MTK_ActiveStandby(UINT32 active)
{
    UINT32 i, u4Val;

    if (DRVCUST_InitQuery(/*ePanelPowerGpio*/eBackLightGpio, &i) == 0)
    {
        UNUSED(DRVCUST_InitQuery(/*ePanelPowerOnVal*/eBackLightOnVal, &u4Val));

        if (!active)
        {
            _PDWNC_ResumeT8032ProtectionMode();
            if(active_standby_state)
            {
            #ifndef CC_MTK_LOADER
                vApiVestelPUPSequence();//This sequence is not used at first panel power on
            #endif
            }  
            GPIO_SetOut(i, u4Val);
        }
        else
        {
            _PDWNC_StopT8032ProtectionMode();
            GPIO_SetOut(i, (1 - u4Val));
            //    vApiVestelPDWNSequence();  //add lbh change for powersequence 20120118
            #ifndef CC_MTK_LOADER
                vApiVestelPDWNSequence();
            #endif
        }
		active_standby_state = active;
    }

    /** LED2_GPIO **/
    //        GPIO_SetOut(LED2_GPIO,LED2_STBY);

}


UINT32 PDWNC_MTK_GetWatchdogTimeout(void)
{
    Printf("%s TimerDimmingFreq=%d \n", __FUNCTION__, TimerDimmingFreq);
    return TimerDimmingFreq;

}



void PDWNC_MTK_SetStandbyLed(UINT32 mode)
{
#ifndef CC_LOADER_SHRINK_LED

    UINT32 u4BlinkingLedGpio = GpioNumberLed, u4BlinkingLedOnVal = LedContronOnVal;
    UINT32 u4LedBlinkingPeriod = 20;
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_LED_BLINKING;
    rCmd.u1SubCmd = mode;

    if (u4BlinkingLedGpio >= SERVO(0))
    {
        u4BlinkingLedGpio -= SERVO(0);
        rCmd.au1Data[0] = (UINT8)u4BlinkingLedGpio; //OFFSET GPIO INDEX
        rCmd.au1Data[1] = (UINT8)u4BlinkingLedOnVal; // POLARITY
#if defined(CC_MT5365) || defined(CC_MT5395)
        rCmd.au1Data[2] =  (UINT8)(u4LedBlinkingPeriod / 20);
#else
        rCmd.au1Data[2] =  (UINT8)(u4LedBlinkingPeriod / 50);
#endif
        rCmd.au1Data[3] =  1;
    }
    else if (u4BlinkingLedGpio >= OPCTRL(0))
    {
        u4BlinkingLedGpio -= OPCTRL(0);
        rCmd.au1Data[0] = (UINT8)u4BlinkingLedGpio; //OFFSET GPIO INDEX
        rCmd.au1Data[1] = (UINT8)u4BlinkingLedOnVal; // POLARITY
#if defined(CC_MT5365) || defined(CC_MT5395)
        rCmd.au1Data[2] =  (UINT8)(u4LedBlinkingPeriod / 20);
#else
        rCmd.au1Data[2] =  (UINT8)(u4LedBlinkingPeriod / 50);
#endif
        rCmd.au1Data[3] =  0;
    }
    else
    {
        Printf("Set t8032 led gpio error (gpio index error)!!!\n");
        return ;
    }
    if (mode)
    {
        GPIO_SetOut(u4BlinkingLedGpio, ~u4BlinkingLedOnVal);
        Printf("Enable Led blinking (gpio_idx %d, on_value %d, period %d, type: %s)\n", rCmd.au1Data[0], rCmd.au1Data[1], rCmd.au1Data[2], ((rCmd.au1Data[3]) ? "SRV" : "OPCTRL"));
    }
    else
    {
        Printf("Disable gpio %d Led blinking\n", u4BlinkingLedGpio);
    }

    return ;
#endif
}


#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
/** _WatchDogSimIsr() a watch dog simulation interrupt routine.
 */
//-----------------------------------------------------------------------------
static void _WatchDogSimIsr(UINT16 u2VctId)
{
    UNUSED(u2VctId);
    Printf("Pseudo watchdog isr timeout\n");
#ifdef NDEBUG
    if (_fgPseudoWatchdogAssert)
    {
        Assert("_WatchDogSimIsr", __FILE__, (INT32)__LINE__);
    }
#endif /* NDEBUG */
    VERIFY(BIM_ClearIrq(VECTOR_T1));
}

#define WATCHDOG_SEC_CNT(x)  (0x7fffffffU - (x * 0x1B00000))
#define WATCHDOG_MSEC_CNT(x)  (0x7fffffffU - (((x/100) * 0x1B00000)/10))


void PDWNC_MTK_WatchdogEnable(UINT32 enable)
{
    if (enable)
    {
#ifdef NDEBUG
        vIO32Write4B(PDWNC_WDTCTL, u4IO32Read4B(PDWNC_WDTCTL) | 0xff);
#endif
    }
    else
    {
        vIO32Write4B(PDWNC_WDTCTL, u4IO32Read4B(PDWNC_WDTCTL) & 0x00ffff00);
    }
}
void PDWNC_MTK_SetWatchdogTimeout(UINT32 timeout)
{
    TimerDimmingFreq = timeout;
    Printf("%s TimerDimmingFreq=%d \n", __FUNCTION__, TimerDimmingFreq);
}

void PDWNC_MTK_HardWare_Led(UINT32 number, UINT32 state)
{
    GpioNumberLed = number;
    LedContronOnVal = state;
}


UINT8 bEnableWatchDog[3] = {1, 1, 1};//for watchdog 0, 1, 2


//-----------------------------------------------------------------------------
/** _WatchDogBody() a watch dog thread body.
 */
//-----------------------------------------------------------------------------
static void _WatchDogBody(void *pvArg)
{
    UINT32 u4Clk, u4Sec;
    UINT32 u4Val, u4State;
    void (* pfnOldIsr)(UINT16);
    HAL_TIME_T rPrevTime, rTime, rDelta;

    UNUSED(pvArg);

    // Set simulation timer initial value.
    if (DRVCUST_OptQuery(eTimerDimmingFreq, &u4Val) != 0)
    {
        INT32 i4Ret;

        // no one use timer1, Hook interrupt
        i4Ret = x_reg_isr(VECTOR_T1, _WatchDogSimIsr, &pfnOldIsr);
        ASSERT(i4Ret == OSR_OK);
        UNUSED(i4Ret);

        // set initial timer, 25 seconds.
        BIM_WRITE32(REG_RW_TIMER1_LOW , 0x80000000);
        BIM_WRITE32(REG_RW_TIMER1_HIGH, 0);

        // enable timer 1
        BIM_LockModifyReg32(REG_RW_TIMER_CTRL, ~(TMR1_AUTOLD_EN), (TMR1_CNTDWN_EN));
    }

    // Disable the watchdog
    vIO32Write4B(PDWNC_WDTCTL, u4IO32Read4B(PDWNC_WDTCTL) & 0x00ffff00);//vIO32WriteFldAlign(PDWNC_WDTCTL, 0, FLD_WDT0E);

#ifdef NDEBUG
    // Enable the watchdog. release version only.
    if (DRVCUST_OptGet(eReleaseEnableWatchDog))
    {
        // Set watchdog timer initial value, 25 seconds.
        vIO32Write4B(PDWNC_WDT0, 0x8000000);//PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, 0x8000000);
        // Enable the watchdog, then watchdog can be disable by outside.
        vIO32Write4B(PDWNC_WDTCTL, (u4IO32Read4B(PDWNC_WDTCTL) & 0x00ffffff) | 0xff);//vIO32WriteFldAlign(PDWNC_WDTCTL, 0xFF, FLD_WDT0E);
        u4Clk = BSP_GetDomainClock(SRC_BUS_CLK);
        u4Sec = DRVCUST_OptGet(eReleaseWatchdogTimeout);
        Printf("Watchdog enable:%d\n", (u4Sec*u4Clk));
    }
#endif

    while (1)
    {
        // Make set counter as critical section.
        // All values come from register.
        u4State = HalCriticalStart();
        u4Clk = BSP_GetDomainClock(SRC_TIMER_CLK);
        u4Sec = DRVCUST_OptGet(eReleaseWatchdogTimeout);
        u4Clk = u4Sec * u4Clk;
        if (DRVCUST_OptQuery(eTimerDimmingFreq, &u4Val) != 0)
        {
            BIM_WRITE32(REG_RW_TIMER1_LOW , u4Clk);
            BIM_WRITE32(REG_RW_TIMER1_HIGH, 0);
            BIM_LockModifyReg32(REG_RW_TIMER_CTRL, ~0, TMR1_CNTDWN_EN);
        }

        if (bEnableWatchDog[0] == 0)
        {
            vIO32Write4B(PDWNC_WDTCTL, (u4IO32Read4B(PDWNC_WDTCTL) & 0x00ffff00));
        }
        else
        {
            vIO32Write4B(PDWNC_WDTCTL, (u4IO32Read4B(PDWNC_WDTCTL) & 0x00ffffff) | 0xff);
        }

        vIO32Write4B(PDWNC_WDT0, WATCHDOG_SEC_CNT(u4Sec));

        HalCriticalEnd(u4State);

        // Measure time for debugging.
        HAL_GetTime(&rPrevTime);
        x_thread_delay(500);
        HAL_GetTime(&rTime);
        HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);
        if (rDelta.u4Seconds >= u4Sec)
        {
            Printf("Pseudo watchdog timeout\n");
#ifdef NDEBUG
            // Assert("rDelta.u4Seconds < u4Sec", __FILE__, (INT32)__LINE__);
#endif
        }
    }
}


static void _HWatchDogBody(void *pvArg)
{
    UINT32 u4State;

    UNUSED(pvArg);

    // Disable the watchdog
    vIO32Write4B(PDWNC_WDTCTL, u4IO32Read4B(PDWNC_WDTCTL) & 0x00ff00ff);// vIO32WriteFldAlign(PDWNC_WDTCTL, 0, FLD_WDT1E);

    x_thread_delay(3000);

#ifdef NDEBUG
    // Enable the watchdog. release version only.
    if (DRVCUST_OptGet(eReleaseEnableWatchDog))
    {
        // Set watchdog timer initial value, 25 seconds.
        vIO32Write4B(PDWNC_WDT1, 0x8000000);//PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, 0x8000000);
        // Enable the watchdog, then watchdog can be disable by outside.
        vIO32Write4B(PDWNC_WDTCTL, (u4IO32Read4B(PDWNC_WDTCTL) & 0x00ffffff) | 0xff00);//vIO32WriteFldAlign(PDWNC_WDTCTL, 0xFF, FLD_WDT1E);
    }
#endif

    while (1)
    {
        if (bEnableWatchDog[1] == 0)
        {
            vIO32Write4B(PDWNC_WDTCTL, (u4IO32Read4B(PDWNC_WDTCTL) & 0x00ff00ff));
        }
        else
        {
            vIO32Write4B(PDWNC_WDTCTL, (u4IO32Read4B(PDWNC_WDTCTL) & 0x00ffffff) | 0xff00);
        }
        // Make set counter as critical section.
        // All values come from register.
        u4State = HalCriticalStart();
        vIO32Write4B(PDWNC_WDT1, WATCHDOG_MSEC_CNT(1000));
        HalCriticalEnd(u4State);

        x_thread_delay(100);
    }
}

#endif /* CC_MTK_LOADER */




//-----------------------------------------------------------------------------
// Global functions
//-----------------------------------------------------------------------------


PDWNC_FuncTbl* PDWNC_EXT_GetFunc(void)
{
    return (&rPDWNC_EXT_FuncTbl);
}

void PDWNC_MTK_Init(void)
{
#ifndef CC_LOADER_SHRINK_NORMAL

    return;
#endif
}

void PDWNC_MTK_WatchDogInit(void)
{
#if 1
#if !defined(CC_MTK_LOADER) && !defined(CPU_TEST_ENABLE)
    static HANDLE_T hWatchDog;
    static HANDLE_T hHWatchDog;
   // DRVCUST_OptGet(eReleaseWatchdogPriority)

    vIO32Write4B(PDWNC_WDTCTL, 0);//PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 0);
    if (x_thread_create(&hWatchDog, "WatchDog", 0x800,
                        DRVCUST_OptGet(eReleaseWatchdogPriority),
                        _WatchDogBody, 0, NULL) != OSR_OK)
    {
        LOG(1, "\nError: Create watchdog thread fail\n");
        return;
    }
    if (x_thread_create(&hHWatchDog, "2WatchDog", 0x800,
#if defined(LINUX_TURNKEY_SOLUTION)||defined (__KERNEL__)
                        1,
#else
                        49,
#endif
                        _HWatchDogBody, 0, NULL) != OSR_OK)
    {
        LOG(1, "\nError: Create watchdog thread fail\n");
        return;
    }
#endif /* CC_MTK_LOADER */
#endif
}

void PDWNC_MTK_ErrorHandleInit(void)
{
#ifndef CC_LOADER_SHRINK_NORMAL

    return;
#endif
}

void PDWNC_MTK_ReadWakeupReason(UINT32* pu4Reason)
{
    UINT32 u4Val;
    static UINT32 u4EverReqWakeupReason = 0;
    static UINT32 u4T8032WakeupReason = 0;

    // Wakeup REASON...
    u4Val = _PDWNC_ReadWakeupStatus();
    if (u4Val == 0)
    {
        if (_PDWNC_ReadWatchDogStatus())
        {
            *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_WATCHDOG;
#ifdef CC_MTK_LOADER
            if (IO32ReadFldAlign(PDWNC_WDTCTL, FLD_WDT0RST))
            {
                Printf("Boot reason: Watch dog0 reset (0x%x)\n", u4IO32Read4B(PDWNC_WDTCTL));
            }
            else if (IO32ReadFldAlign(PDWNC_WDTCTL, FLD_WDT1RST))
            {
                Printf("Boot reason: Watch dog1 reset (0x%x)\n", u4IO32Read4B(PDWNC_WDTCTL));
            }
            else if (IO32ReadFldAlign(PDWNC_WDTCTL, FLD_WDT2RST))
            {
                Printf("Boot reason: Watch dog2 reset (0x%x)\n", u4IO32Read4B(PDWNC_WDTCTL));
            }
#endif
        }
        else
        {
            *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_AC_POWER;
#ifdef CC_MTK_LOADER
            Printf("Boot reason: A/C power on\n");
#endif
        }
    }
    else if (u4Val & WAK_RTC)
    {
        Printf("Boot reason: PDWNC_WAKE_UP_REASON_RTC\n");
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_RTC;
    }
    else if (u4Val & WAK_VGA)
    {

        Printf("Boot reason: PDWNC_WAKE_UP_REASON_VGA\n");
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_VGA;
    }
    else if (u4Val & WAK_RS232)
    {

        Printf("Boot reason: PDWNC_WAKE_UP_REASON_UART\n");
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_UART;
    }
    else if (u4Val & WAK_UNOR)
    {
        Printf("Boot reason: PDWNC_WAKE_UP_REASON_UART_NORMAL\n");
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_UART_NORMAL;
    }
    else if (u4Val & (WAK_HDMI/* | WAK_CEC*/))
    {

        Printf("Boot reason: PDWNC_WAKE_UP_REASON_HDMI\n");
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_HDMI;
    }
    else if (u4Val & WAK_T8032)
    {
        PDWNC_T8032_CMD_T rCmd;
        PDWNC_T8032_RESPONSE_T rResponse;

        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_READ_WAKEUP_REASON;  //Get the real wakeup reason
        rCmd.u1SubCmd = 0; rCmd.au1Data[0] = rCmd.au1Data[1] = rCmd.au1Data[2] = rCmd.au1Data[3] = 0;
        rResponse.au1Data[0] = PDWNC_WAKE_UP_REASON_HDMI;//T8032_WAKEUP_REASON_CEC_ONE_TOUCH_PLAY;
        if (u4EverReqWakeupReason == 0)
        {
            if (PDWNC_T8032Cmd(&rCmd, &rResponse))
            {
                *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_HDMI;
            }

            else
            {
                *pu4Reason = (UINT32)rResponse.au1Data[0];
            }
            u4EverReqWakeupReason = 1;
            u4T8032WakeupReason = *pu4Reason;
        }
        else
        {
            *pu4Reason = (UINT32)u4T8032WakeupReason;
        }
        if(*pu4Reason == PDWNC_WAKE_UP_REASON_DVD)
        {
          Printf("Boot reason:From T8032 is DVD wakeup\n");  
        }
        else if(*pu4Reason == PDWNC_WAKE_UP_REASON_SCART)
        {
          Printf("Boot reason:From T8032 is scart wakeup\n");  
        }
        else if(*pu4Reason == PDWNC_WAKE_UP_REASON_FP_POWER)
        {
          Printf("Boot reason:From T8032 is touchpad(3-way) power\n");  
        }
        else
        {
            Printf("Boot reason:From T8032 is %d\n", *pu4Reason);
        }
    }

    else if (PDWNC_IsWakeupByPowerButton())
    {

        Printf("Boot reason: PDWNC_WAKE_UP_REASON_KEYPAD\n");
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_KEYPAD;
    }
    else // if (u4Val & WAK_IRRX)
    {
        Printf("Boot reason: PDWNC_WAKE_UP_REASON_IRRC\n");
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_IRRC;
    }
}

void PDWNC_MTK_SetupPowerDown(UINT32 u4PowerDownEvent, UINT32 *pu4PowerDownEn)
{
#ifndef CC_LOADER_SHRINK_NORMAL

    UNUSED(u4PowerDownEvent);
    UNUSED(pu4PowerDownEn);
    return;
#endif
}

void PDWNC_MTK_ErrorCode(UINT8 u1ErrorCode)
{
#ifndef CC_LOADER_SHRINK_NORMAL
    UNUSED(u1ErrorCode);
    return;
#endif
}

//-----------------------------------------------------------------------------
/** PDWNC_SetErrorCode to set system error code.
 *  @param u1ErrorCode the error code.
 * au1Data[0] : Servo_gpio (1) or OPCTRL (0)
 * au1Data[1] : offset gpio number of servo or opctrl
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_SetLedErrorCode(UINT8 u1Period, UINT8 u1Interval)
{
#ifndef CC_LOADER_SHRINK_LED

    PDWNC_T8032_CMD_T rCmd;
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_ERROR_CODE;
//    rCmd.u1SubCmd = u1ErrorCode;
    rCmd.au1Data[0] = rCmd.au1Data[1] = rCmd.au1Data[2] = rCmd.au1Data[3] = 0;

    if (DRVCUST_InitGet(eT8032CtrlErrLed) && DRVCUST_InitGet(eT8032CtrlErrLedOnVal))
    {
        UINT32 u4T8032CtrlGpio, u4OnVal;
        DRVCUST_InitQuery(eT8032CtrlErrLed, &u4T8032CtrlGpio);
        DRVCUST_InitQuery(eT8032CtrlErrLedOnVal, &u4OnVal);
        Printf("Set t8032 ctrl gpio %d (On val: %d, Para1: %d, Para2: %d)\n", u4T8032CtrlGpio, u4OnVal, u1Period, u1Interval);
#if 1//#if !defined(CC_MT5365) && !defined(CC_MT5395)
        if (u4T8032CtrlGpio >= SERVO(0))
        {
            u4T8032CtrlGpio -= SERVO(0);
            rCmd.u1SubCmd = 1; // TYPE (1: SERVO, 0: OPCTRL)
            rCmd.au1Data[0] = (UINT8)u4T8032CtrlGpio; //OFFSET GPIO INDEX
            rCmd.au1Data[1] = (UINT8)u4OnVal; // POLARITY
            rCmd.au1Data[2] =  u1Period;
            rCmd.au1Data[3] =  u1Interval;
        }
        else
#endif
            if (u4T8032CtrlGpio >= OPCTRL(0))
            {
                rCmd.u1SubCmd = 0; // TYPE (1: SERVO, 0: OPCTRL)
                u4T8032CtrlGpio -= OPCTRL(0);
                rCmd.au1Data[0] = (UINT8)u4T8032CtrlGpio; //OFFSET GPIO INDEX
                rCmd.au1Data[1] = (UINT8)u4OnVal; // POLARITY
                rCmd.au1Data[2] =  u1Period;
                rCmd.au1Data[3] =  u1Interval;
            }
            else
            {
                Printf("Set t8032 ctrl gpio error (gpio index error)!!!\n");
                return -1;
            }
        return PDWNC_T8032Cmd(&rCmd, NULL);
    }
    else
    {
        Printf("Set t8032 ctrl gpio error (no setting) !!!\n");
        return -1;
    }
#else
    return 0;
#endif
}

//-----------------------------------------------------------------------------
/** PDWNC_SetErrorCodeBlinking to enable system error code blinking function (at standby mode).
 *  @param u1ErrorCode the error code.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_SetLedErrorCodeBlinking(UINT8 u1times)
{
#ifndef CC_LOADER_SHRINK_LED
    PDWNC_T8032_CMD_T rCmd;
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_ERROR_CODE_BLINKING;
    rCmd.u1SubCmd = u1times;
    rCmd.au1Data[0] = rCmd.au1Data[1] = rCmd.au1Data[2] = rCmd.au1Data[3] = 0;
    return PDWNC_T8032Cmd(&rCmd, NULL);
#else
    return 0;
#endif
}

INT32 PDWNC_MTK_Set8032uPLedBlink(UINT32 u4BlinkTimes)
{
#ifndef CC_LOADER_SHRINK_LED

    UNUSED(PDWNC_SetLedErrorCode(MTK_ERR_LED_BLINK_TIME, MTK_ERR_LED_BLINK_INVERAL));
    return (PDWNC_SetLedErrorCodeBlinking(u4BlinkTimes));
#else
    return 0;
#endif
}

#endif /* CC_MTK_PRELOADER */

