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
#include "x_hal_io.h"
#include "x_dram.h"
#include "x_irq.h"
#include "drv_pwm.h"
#include "pdwnc_if.h"
#include "eeprom_if.h"
#include "nand_if.h"
#include "SelfDiag/Iris_SelfDiag.h"

#ifdef CC_MTK_LOADER
#include "Iris_SMicroUpgrade.h"
#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#ifndef CC_PSEUDO_WATCHDOG_ASSERT
#define CC_PSEUDO_WATCHDOG_ASSERT           0
#endif /* CC_PSEUDO_WATCHDOG_ASSERT */

#define LOADER_ENV_INIT_FUNC                (LOADER_Iris_EnvInitFunc)
#define WATCHDOG_SEC_CNT(x)                 (0x0fffffffU - (x * 0x300000))

#define IRIS_BLINK_TIME                     (10)    /* 10 x 50ms = 500ms*/
#define IRIS_BLINK_INVERAL                  (60)    /* 60 x 50ms = 3000ms*/
#define IRIS_USB_UPGRADE_BLINK_PERIOD       500     // 500ms

#ifdef IRIS_PCBA_EVT
#define SIGNAL_X_U_RST                      30         // PTSI_D6
#define SIGNAL_POWER_ON_RDY                 81         // LVDS_CTRL2
#else
#define SIGNAL_X_U_RST                      69
#define SIGNAL_POWER_ON_RDY                 63
#endif /* IRIS_PCBA_EVT */

// OPWM configuration for ESD
#define IRIS_ESD_PWM_PORT                   (2)
#define IRIS_ESD_PWM_DUTY_ON                0x80
#define IRIS_ESD_PWM_DUTY_ALL               0xFF
#define IRIS_ESD_PWM_FREQ                   57500   // 57.5KHz

// Multi-key definition
#define IRIS_uP_CMD_GET_IR_MULTIKEY_WAKEUP  0x40
#define IRIS_UNKNOWN_MODE                   0
#define IRIS_SERVICE_MODE                   1
#define IRIS_SELF_DIAGNOSTIC_MODE           2
#define IRIS_ACTIVE_HOTEL_MODE              3
#define IRIS_EXIT_HOTEL_MODE                4


//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------

void PDWNC_Iris_Init(void);
void PDWNC_Iris_WatchDogInit(void);
void PDWNC_Iris_ErrorHandleInit(void);
void PDWNC_Iris_ReadWakeupReason(UINT32* pu4Reason);
void PDWNC_Iris_SetupPowerDown(UINT32 u4PowerDownEvent, UINT32 *pu4PowerDownEn);
void PDWNC_Iris_ErrorCode(UINT8 u1ErrorCode);
INT32 PDWNC_Iris_Set8032uPLedBlink(UINT32 u4BlinkTimes);
UINT32 PDWNC_Iris_GetStatus(UINT32 u4Addr, VOID* pInfo, UINT32 u4Size);
UINT32 PDWNC_Iris_LEDSet(UINT32 u4Addr, VOID* pInfo, UINT32 u4Size);
static void Iris_InitGPIOTimer(void);


//-----------------------------------------------------------------------------
// Imported funtions 
//-----------------------------------------------------------------------------
extern UINT32 _PDWNC_ReadWakeupStatus(void);
extern UINT32 _PDWNC_ReadWatchDogStatus(void);

extern void Iris_LEDBlinkCtrl(D_LED_TYPE eLedType);


extern BOOL _fgLedBlink[D_LED_TOTAL + 1];
extern BOOL _fgLedOn[D_LED_TOTAL + 1];
extern BOOL _fgLedAllBlink;


#ifndef CC_MTK_PRELOADER
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static PDWNC_FuncTbl rPDWNC_EXT_FuncTbl = 
{
    PDWNC_Iris_Init,                // pfnInit
    PDWNC_Iris_SetupPowerDown,      // pfnPowerDown
    PDWNC_Iris_ReadWakeupReason,    // pfnReadWakeupReason
    PDWNC_Iris_WatchDogInit,        // pfnWatchDogInit;
    PDWNC_Iris_ErrorHandleInit,     // pfnErrorHandleInit
    PDWNC_Iris_ErrorCode,           // pfnWriteErrorCode
    NULL,                           // pfnReadErrorCode
    NULL,                           // pfnGetCurrentState
    NULL,                           // pfnDetermineNextState
    NULL,                           // pfnEnterNextState
    PDWNC_Iris_Set8032uPLedBlink,   // pfnSet8032uPLedBlink
    NULL,                           // pfnCustomRead
    NULL                            // pfnCustomWrite
};

#ifdef CC_MTK_LOADER
static PFN_IRQ_HANDLER _pfnIrisNextIsr = NULL;
#endif /* CC_MTK_LOADER */

static UINT32 _fgPseudoWatchdogAssert = CC_PSEUDO_WATCHDOG_ASSERT;
//static BOOL   _fgDebug = FALSE;


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
#ifdef IRIS_PCBA_EVT
static void IrisInit8032uP(void)
{
    UINT32 u4Offset, u4Size;

    // Setup 8032 uP
    u4Offset = DRVCUST_InitGet(eT8032uPOffset);
    u4Size = DRVCUST_InitGet(eT8032uPSize);
    if (u4Offset != 0)  // here needs to setup 8032uP.
    {
        if (u4Size == 0)
        {
            u4Size = (16*1024);
        }

#ifdef CC_NAND_LOADER
        if (u4Offset & ADDR_NAND_FLASH)
        {
            //EXTERN NAND_LoadImage(UINT64, UINT32, UINT32);
            UINT32 u4Dest = (UINT32)x_mem_alloc(u4Size);
            if (u4Dest == 0)
            {
                Printf("mem alloc fail\n");
                return;
            }
            NAND_LoadImage(0,u4Dest,(UINT64)(u4Offset &(~ADDR_NAND_FLASH)),  u4Size);
            u4Offset = u4Dest;
        }
#endif /* CC_NAND_LOADER */

        PDWNC_InitT8032(u4Offset, u4Size);
    }
}

static UINT32 u4GetLEDBlinkCode(IRIS_SELF_DIAG_TABLE eErrorLog)
{
    UINT32 u4LEDBlinkTime = 0;
    
    // Self diagnostic is abnormal, record the error information
    if (eErrorLog.u2Amp12VError || eErrorLog.u2SW12VError)
    {
        u4LEDBlinkTime = 2;
    }
    else if (eErrorLog.u2AudError || eErrorLog.u2VDrop3V_5VError)
    {
        u4LEDBlinkTime = 3;
    }
    else if (eErrorLog.u2BalanceError)
    {
        u4LEDBlinkTime = 4;
    }
    else if (eErrorLog.u2TCONError || eErrorLog.u2HFRError || 
             eErrorLog.u2PanelNVMError || eErrorLog.u2PanelPowerError)
    {
        u4LEDBlinkTime = 5;
    }
    else if (eErrorLog.u2BacklightError)
    {
        u4LEDBlinkTime = 6;
    }
    else if (eErrorLog.u2TemperatureError)
    {
        u4LEDBlinkTime = 7;
    }
    
    return u4LEDBlinkTime;
}

static BOOL IrisCheckShutdown(void)
{
    BOOL   bfgIsEmpty = TRUE;
    BOOL   bfgShutdown = FALSE;
    UINT8  au1Data[IRIS_SELF_DIAG_EEPROM_SIZE];
    UINT8  u1RetryLimit = 0, u1RetryCount = 0;
    UINT16 u2ShutdownLog = 0, u2Recovery = 0;
    UINT32 u4BootCounter = 0;
    UINT32 u4Len = sizeof(au1Data);
    UINT32 i;
    
    x_memset((void *)au1Data, 0xFF, u4Len);
    
    // Get Self-Diag data from EEPROM
    UNUSED(EEPROM_Read(IRIS_SELF_DIAG_DET_ENABLE_ADDR,
                      (UINT32)(VOID *)au1Data, u4Len));

    // Check if all 64 bytes are 0xFF
    // If yes -> EEPROM is empty and should load default value
    for (i = 0; i < u4Len; i++)
    {
        if (au1Data[i] != 0xFF)
        {
            bfgIsEmpty = FALSE;
            break;
        }
    }

    if (bfgIsEmpty)
    {
        return FALSE;
    }
    
    // Read the boot counter from EEPROM
    UNUSED(EEPROM_Read(IRIS_SELF_DIAG_BOOT_CNT_ADDR, 
                       (UINT32)(VOID *)&u4BootCounter, sizeof(UINT32)));
    
    // Increase the boot counter & update to EEPROM
    u4BootCounter++;
    UNUSED(EEPROM_Write(IRIS_SELF_DIAG_BOOT_CNT_ADDR, 
                        (UINT32)(VOID *)&u4BootCounter, sizeof(UINT32)));

    // Read the shutdown log from EEPROM
    UNUSED(EEPROM_Read(IRIS_SELF_DIAG_ERROR_LOG_ADDR, 
                       (UINT32)(VOID *)&u2ShutdownLog, sizeof(UINT16)));

    // No error
    if (u2ShutdownLog == 0)
    {
        return FALSE;
    }

    if (PDWNC_ReadWakeupReason() == PDWNC_WAKE_UP_REASON_AC_POWER)
    {   
        UINT8 u1Val = 0;
        
        // Get Shutdown state from EEPROM
        UNUSED(EEPROM_Read(IRIS_SELF_DIAG_SHUTDOWN_OCCUR_ADDR,
                           (UINT32)(VOID *)&u1Val, sizeof(UINT8)));
        if (u1Val > 0)
        {
            bfgShutdown = TRUE;
        }
    }
    
    // Read the recovery table from EEPROM
    UNUSED(EEPROM_Read(IRIS_SELF_DIAG_BOOT_RETRY_ADDR, 
                       (UINT32)(VOID *)&u2Recovery, sizeof(UINT16)));

    // There is a shutdown log but its recovery bit is disable.
    // So, system need to go to self shutdown
    if ((u2ShutdownLog & u2Recovery) > 0)
    {
        bfgShutdown = TRUE;
    }

    // Start to retry from safety shutdown
    
    // Read the retry limitation from EEPROM
    // If the retry limit = 0 -> system does not receive recovery command
    UNUSED(EEPROM_Read(IRIS_SELF_DIAG_RETRY_LIMIT_ADDR, 
                       (UINT32)(VOID *)&u1RetryLimit, sizeof(UINT8)));
    if (u1RetryLimit == 0)
    {
        bfgShutdown = TRUE;
    }

    // If the retry limit > 0, read the current retry counter from EEPROM
    // Check if the counter reaches the retry limitation.
    // if yes, system does not recovery any more
    UNUSED(EEPROM_Read(IRIS_SELF_DIAG_RETRY_CNT_ADDR, 
                       (UINT32)(VOID *)&u1RetryCount, sizeof(UINT8)));
    if (u1RetryCount == u1RetryLimit)
    {
        bfgShutdown = TRUE;
    }

    if (bfgShutdown)
    {
        IRIS_SELF_DIAG_TABLE eShutdownLog;
        UINT32 u4LEDErrorCode;

        x_memcpy(&eShutdownLog, &u2ShutdownLog, sizeof(IRIS_SELF_DIAG_TABLE));
        u4LEDErrorCode = u4GetLEDBlinkCode(eShutdownLog);
        
        // Set Failure LED blink
        IrisInit8032uP();
        x_thread_delay(500);
        PDWNC_Iris_Set8032uPLedBlink(u4LEDErrorCode);
    }
    
    return bfgShutdown;    
}
#endif /* IRIS_PCBA_EVT */


UINT32 PDWNC_SetupPseudoWatchdogAssert(UINT32 *pu4Flag)
{
    UNUSED(pu4Flag);
    if (pu4Flag != NULL)
    {
        _fgPseudoWatchdogAssert = *pu4Flag;
    }
    return _fgPseudoWatchdogAssert;
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
        u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
        u4Val &= ~(TMR1_AUTOLD_EN);
        u4Val |= (TMR1_CNTDWN_EN);
        BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);
    }

    // Disable the watchdog
    vIO32Write4B(PDWNC_WDTCTL, 0);//PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 0);

#ifdef NDEBUG
    // Enable the watchdog. release version only.
    if (DRVCUST_OptGet(eReleaseEnableWatchDog))
    {
        // Set watchdog timer initial value, 25 seconds.
        vIO32Write4B(PDWNC_WDT0, 0x8000000);//PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, 0x8000000);
        // Enable the watchdog, then watchdog can be disable by outside.
        vIO32Write4B(PDWNC_WDTCTL, 1); //PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 1);
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
        u4Clk = u4Sec*u4Clk;
        if (DRVCUST_OptQuery(eTimerDimmingFreq, &u4Val) != 0)
        {
            BIM_WRITE32(REG_RW_TIMER1_LOW , u4Clk);
            BIM_WRITE32(REG_RW_TIMER1_HIGH, 0);
            u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
            u4Val |= TMR1_CNTDWN_EN;
            BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);
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

#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
// Global functions
//-----------------------------------------------------------------------------
PDWNC_FuncTbl* PDWNC_EXT_GetFunc(void)
{
    return (&rPDWNC_EXT_FuncTbl);
}

void PDWNC_Iris_Init(void)
{
#ifndef CC_MTK_LOADER
    UNUSED(PDWNC_SetSonyErrorCode(IRIS_BLINK_TIME, IRIS_BLINK_INVERAL));
#endif /* CC_MTK_LOADER */

    return;
}

void PDWNC_Iris_WatchDogInit(void)
{
#ifndef CC_MTK_LOADER
    static HANDLE_T hWatchDog;

    vIO32Write4B(PDWNC_WDTCTL, 0);//PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 0);
    if (x_thread_create(&hWatchDog, "WatchDog", 0x800, 
                        DRVCUST_OptGet(eReleaseWatchdogPriority),
                        _WatchDogBody, 0, NULL) != OSR_OK)
    {
        LOG(1, "\nError: Create watchdog thread fail\n");
        return;
    }
#endif /* CC_MTK_LOADER */
}

void PDWNC_Iris_ErrorHandleInit(void)
{
    return;
}

void PDWNC_Iris_ReadWakeupReason(UINT32* pu4Reason)
{
    UINT32 u4Val;

    // Wakeup REASON...
    u4Val = _PDWNC_ReadWakeupStatus();     
    if (u4Val == 0)
    {
        if(_PDWNC_ReadWatchDogStatus())
        {
            *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_WATCHDOG;
#ifdef CC_MTK_LOADER
            Printf("Boot reason: Watch dog reset!!");            
#endif
        }
        else
        {
            *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_AC_POWER;
#ifdef CC_MTK_LOADER            
            Printf("Boot reason: A/C power on!!");                       
#endif
        }
    }
    else if (u4Val & WAK_RTC)
    {
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_RTC;
    }
    else if (u4Val & WAK_VGA)
    {
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_VGA;
    }
    else if (u4Val & WAK_RS232)
    {
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_UART;
    }
    else if (u4Val & WAK_UNOR)
    {
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_UART_NORMAL;
    }
    else if (u4Val & (WAK_HDMI/* | WAK_CEC*/))
    {
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_HDMI;
    }
    else if (PDWNC_IsWakeupByPowerButton())
    {
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_KEYPAD;
    }
    else // if (u4Val & WAK_IRRX)
    {
#ifndef CC_MTK_LOADER
        PDWNC_T8032_CMD_T rCmd;
        PDWNC_T8032_RESPONSE_T rResponse;

        x_memset(&rCmd, 0, sizeof(PDWNC_T8032_CMD_T));
        rCmd.u1Cmd = IRIS_uP_CMD_GET_IR_MULTIKEY_WAKEUP;
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_IRRC;
        
        if (PDWNC_T8032Cmd(&rCmd, &rResponse) == 0)
        {
            switch (rResponse.au1Data[0])
            {
            case IRIS_SERVICE_MODE:
                *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_CUSTOM_1;
                break;
            case IRIS_SELF_DIAGNOSTIC_MODE:
                *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_CUSTOM_2;
                break;
            case IRIS_ACTIVE_HOTEL_MODE:
                *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_CUSTOM_3;
                break;
            case IRIS_EXIT_HOTEL_MODE:
                *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_CUSTOM_4;
                break;
            }
        }
        Printf("Wakeup reason is %d\n", *pu4Reason);
#else
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_IRRC;
#endif
    }
}

void PDWNC_Iris_SetupPowerDown(UINT32 u4PowerDownEvent, UINT32 *pu4PowerDownEn)
{
    UNUSED(u4PowerDownEvent);
    UNUSED(pu4PowerDownEn);    
    
#ifndef CC_MTK_LOADER
    IrisSelfDiagUpdateOnOffCount();
#endif /* CC_MTK_LOADER */

    return;
}

void PDWNC_Iris_ErrorCode(UINT8 u1ErrorCode)
{
    UNUSED(u1ErrorCode);
    return;
}

INT32 PDWNC_Iris_Set8032uPLedBlink(UINT32 u4BlinkTimes)
{
    UNUSED(PDWNC_SetSonyErrorCode(IRIS_BLINK_TIME, IRIS_BLINK_INVERAL));
    return (PDWNC_SetSonyErrorCodeBlinking(u4BlinkTimes));
}

#ifdef CC_MTK_LOADER
static void LOADER_Iris_TimerOn(void)
{
    UINT32 u4Val;

    // Enable timer 0
    u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Val |= ((TMR1_CNTDWN_EN) | (TMR1_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);
}

static void LOADER_Iris_TimerOff(void)
{
    UINT32 u4Val;

    // disable timer 0
    u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Val &= ~((TMR1_CNTDWN_EN) | (TMR1_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);            
}

static void LOADER_Iris_TimerIsr(void)
{
    if (BIM_IsIrqPending(VECTOR_T1))
    {
        D_LED_TYPE eLedType;
        if (_fgLedAllBlink)
        {
            Iris_LEDBlinkCtrl(D_LED_TOTAL);
        }
        else
        {
            for (eLedType = D_LED_POWER; eLedType <= D_LED_TIMER; eLedType++)
            {
                if (_fgLedBlink[eLedType] == TRUE)
                {
                    Iris_LEDBlinkCtrl(eLedType);
                }
            }
        }
        //_NonOSTimerOff();        
        BIM_ClearIrq(VECTOR_T1);        
    }
    
    if (_pfnIrisNextIsr) { _pfnIrisNextIsr(); }
}
#endif /* CC_MTK_LOADER */

static void LOADER_Iris_EnvInitFunc(void)
{
#ifndef IRIS_PCBA_EVT2   
    UINT32 i;
#endif /* IRIS_PCBA_EVT2 */

    UINT32 u4Val;
    D_LED_TYPE eLedType;
    
#ifdef IRIS_PCBA_EVT
    if (IrisCheckShutdown())
    {
        PDWNC_EnterPowerDown((WAK_IRRX | WAK_UNOR | SRVAD_WAKEN(WAK_SRVADX(2))),
                             PDWNC_POWER_DOWN_IRRC);
    }
#endif /* IRIS_PCBA_EVT */

    vDrvSetPWM(IRIS_ESD_PWM_PORT, 
               ((BSP_GetDomainClock(SRC_BUS_CLK) >> 8) / IRIS_ESD_PWM_FREQ) - 1,
               IRIS_ESD_PWM_DUTY_ON,
               IRIS_ESD_PWM_DUTY_ALL);
    
    u4Val = 1;
    UNUSED(GPIO_SetOut(SIGNAL_X_U_RST, u4Val));

    // Set "U_POWER_READY" as HIGH
    UNUSED(GPIO_SetOut(SIGNAL_POWER_ON_RDY, u4Val));
    
#ifndef IRIS_PCBA_EVT2   
    // Turn on panel power in loader
    if (DRVCUST_InitQuery(ePanelPowerGpio, &i) == 0)
    {
        UNUSED(DRVCUST_InitQuery(ePanelPowerOnVal, &u4Val));
        GPIO_SetOut(i, u4Val);
    }
#endif /* IRIS_PCBA_EVT2 */

    for (eLedType = D_LED_POWER; eLedType < D_LED_TOTAL; eLedType++)
    {
        _fgLedOn[eLedType] = FALSE;
        _fgLedBlink[eLedType] = FALSE;
    }
    
    Iris_InitGPIOTimer();
}

static void Iris_InitGPIOTimer(void)
{
#ifdef CC_MTK_LOADER
    UINT32 u4BusClock;
    UINT32 u4TimerClock;
    
    _pfnIrisNextIsr = RegisterIrqHandler(LOADER_Iris_TimerIsr);

    u4BusClock = BSP_GetDomainClock(SRC_BUS_CLK);
    u4TimerClock = (UINT32)(u4BusClock / 1000);
    u4TimerClock *= IRIS_USB_UPGRADE_BLINK_PERIOD;
    
    // Setup Timer 0 and triger 1st interrupt.
    BIM_WRITE32(REG_RW_TIMER1_LLMT, u4TimerClock);
    BIM_WRITE32(REG_RW_TIMER1_HLMT, 0);
    BIM_WRITE32(REG_RW_TIMER1_LOW , u4TimerClock);
    BIM_WRITE32(REG_RW_TIMER1_HIGH, 0);

    LOADER_Iris_TimerOff();
    
    BIM_EnableIrq(VECTOR_T1);

    HalEnableIRQ();

//    LOADER_Iris_TimerOn();
#else
#endif /* CC_MTK_LOADER */
}

#endif /* CC_MTK_PRELOADER */

