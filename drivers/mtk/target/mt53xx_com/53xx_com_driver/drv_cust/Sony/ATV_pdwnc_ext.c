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
#include "pdwnc_if.h"
#include "d_ATV_cust.h"
#include "eeprom_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#ifndef CC_MODEL_TYPE
#define CC_MODEL_TYPE 0
#endif

#ifndef CC_PSEUDO_WATCHDOG_ASSERT
#define CC_PSEUDO_WATCHDOG_ASSERT           0
#endif /* CC_PSEUDO_WATCHDOG_ASSERT */

#define LOADER_ENV_INIT_FUNC                (LOADER_ATV_EnvInitFunc)
#define WATCHDOG_SEC_CNT(x)                 (0x0fffffffU - (x * 0x300000))

#define ATV_OK                             (UINT32)0
#define ATV_FAIL                           (UINT32)(-1)
#define ATV_USB_UPGRADE_BLINK_PERIOD   500 // 500ms

// Self Diagnostic definition
#define ATV_12V_AMP_DROP_DET               ADIN1
#define ATV_12V_SW_DROP_DET                ADIN0
#define ATV_AUD_AMP_ERROR_DET              GPIO(27)    // PTSI_D3
#define ATV_V_DROP_DET                     GPIO(72)    // AIN5_R_AADC
#define ATV_BALANCE_DET                    ADIN5       // ADIN5_SRV
#define ATV_TCON_RDY_DET                   GPIO(70)    // AIN4_R_AADC
#define ATV_PANEL_V_DROP_DET               GPIO(24)    // PTSI_D0

#define ATV_12V_THRES_DEFAULT              0x13        // 085v
#define ATV_BALANCE_LV_DEFAULT             0x16        // 1v
#define ATV_BALANCE_HV_DEFAULT             0x28        // 1.8v

typedef enum
{
    ATV_CUSTOMER_SELF_DIAGNOSTIC,
} ATV_CUSTOMER_CMD_TYPE;

typedef enum
{
    ATV_12V_AMP_ERROR,
    ATV_12V_SW_ERROR,
    ATV_AUD_ERROR,
    ATV_3V3_5V_ERROR,
    ATV_BALANCER_ERROR,
    ATV_TCON_ERROR,
    ATV_HFR_ERROR,
    ATV_PANEL_NVM_ERROR,
    ATV_PANEL_POWER_ERROR,
    ATV_BACKLIGHT_ERROR,
    ATV_TEMPERATURE_ERROR
} ATV_ERROR_DET;

typedef struct _ATV_Error_Detect
{
    UINT32 u4CmdType;
    UINT32 u4ErrorType;
    UINT32 u4DetValue;
} ATV_ERR_DETECT_T;

//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------

void PDWNC_ATV_Init(void);
void PDWNC_ATV_WatchDogInit(void);
void PDWNC_ATV_ErrorHandleInit(void);
void PDWNC_ATV_ReadWakeupReason(UINT32* pu4Reason);
void PDWNC_ATV_SetupPowerDown(UINT32 u4PowerDownEvent, UINT32 *pu4PowerDownEn);
void PDWNC_ATV_ErrorCode(UINT8 u1ErrorCode);
INT32 PDWNC_ATV_Set8032uPLedBlink(UINT32 u4BlinkTimes);
UINT32 PDWNC_ATV_GetStatus(UINT32 u4Addr, VOID* pInfo, UINT32 u4Size);
UINT32 PDWNC_ATV_LEDSet(UINT32 u4Addr, VOID* pInfo, UINT32 u4Size);
static void ATV_InitGPIOTimer(void);


//-----------------------------------------------------------------------------
// Imported funtions 
//-----------------------------------------------------------------------------
EXTERN UINT32 _PDWNC_ReadWakeupStatus(void);
EXTERN UINT32 _PDWNC_ReadWatchDogStatus(void);

extern void ATV_LEDBlinkCtrl(D_LED_TYPE eLedType);
extern void ATVLEDCtrlFunc(D_LED_TYPE eLedType, UINT32 u4LedState);

extern BOOL _fgLedBlink[D_LED_TOTAL + 1];
extern BOOL _fgLedOn[D_LED_TOTAL + 1];
extern BOOL _fgLedAllBlink;

#ifndef CC_MTK_PRELOADER
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static PDWNC_FuncTbl rPDWNC_EXT_FuncTbl = 
{
    PDWNC_ATV_Init,             // pfnInit
    PDWNC_ATV_SetupPowerDown,   // pfnPowerDown
    PDWNC_ATV_ReadWakeupReason, // pfnReadWakeupReason
    PDWNC_ATV_WatchDogInit,     // pfnWatchDogInit;
    PDWNC_ATV_ErrorHandleInit,  // pfnErrorHandleInit
    PDWNC_ATV_ErrorCode,        // pfnWriteErrorCode
    NULL,                       // pfnReadErrorCode
    NULL,                       // pfnGetCurrentState
    NULL,                       // pfnDetermineNextState
    NULL,                       // pfnEnterNextState
    PDWNC_ATV_Set8032uPLedBlink, // pfnSet8032uPLedBlink
    PDWNC_ATV_GetStatus,           // pfnCustomRead
    NULL                        // pfnCustomWrite
};


#ifdef CC_MTK_LOADER
static PFN_IRQ_HANDLER _pfnATVNextIsr = NULL;
#endif /* CC_MTK_LOADER */

static UINT32 _fgPseudoWatchdogAssert = CC_PSEUDO_WATCHDOG_ASSERT;
//static BOOL   _fgDebug = FALSE;

UINT32 _u4gATVModelType = 0;
//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
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

static void ATVGetModelType(void)
{  
#if (CC_MODEL_TYPE == 0)
    UINT32 u4ADCVal;
#endif
    UINT8  u1ModelType=0;

#if (CC_MODEL_TYPE == 0)
    u4ADCVal=PDWNC_ReadServoADCChannelValue(ATV_MODEL_TYPE_GPIO);
    if((u4ADCVal>0)&&(u4ADCVal)<=0xE) //Model Type 1
    {
        u1ModelType = 1;
    }
    else if((u4ADCVal>=0xF)&&(u4ADCVal)<=0x1E)//Model Type 2
    {
        u1ModelType = 2;
    }
    else if((u4ADCVal>=0x1F)&&(u4ADCVal)<=0x2C)//Model Type 3
    {
        u1ModelType = 3;
    }
    else if((u4ADCVal>=0x2D)&&(u4ADCVal)<=0x3F)//Model Type 4
    {
        u1ModelType = 4;
    }
#else
    u1ModelType = CC_MODEL_TYPE;
#endif

    if(1 == u1ModelType) //Model Type 1
    {
        _u4gATVModelType = DRV_CUSTOM_ATV_MODEL_MOTION_FLOW;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_RGB_SENSOR;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_FLIGHT_SENSOR;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_NON_FM_RADIO;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_HDMIX4;

    }
    else if(2 == u1ModelType)//Model Type 2
    {
        _u4gATVModelType = DRV_CUSTOM_ATV_MODEL_NON_MOTION_FLOW;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_RGB_SENSOR;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_FLIGHT_SENSOR;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_NON_FM_RADIO;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_HDMIX4;

    }
    else if(3 == u1ModelType)//Model Type 3
    {
        _u4gATVModelType = DRV_CUSTOM_ATV_MODEL_NON_MOTION_FLOW;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_RGB_SENSOR;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_FLIGHT_SENSOR;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_NON_FM_RADIO;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_HDMIX4;

    }
    else if(u1ModelType >= 4)//Model Type 4
    {
        _u4gATVModelType = DRV_CUSTOM_ATV_MODEL_NON_MOTION_FLOW;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_NON_RGB_SENSOR;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_NON_FLIGHT_SENSOR;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_FM_RADIO;
        _u4gATVModelType |= DRV_CUSTOM_ATV_MODEL_HDMIX2;

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

void PDWNC_ATV_Init(void)//
{
#ifndef CC_MTK_LOADER
    UNUSED(PDWNC_SetSonyErrorCode(10, 60));
    ATVGetModelType();
#endif /* CC_MTK_LOADER */

    return;
}

void PDWNC_ATV_WatchDogInit(void)
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

void PDWNC_ATV_ErrorHandleInit(void)
{
    return;
}

void PDWNC_ATV_ReadWakeupReason(UINT32* pu4Reason)
{
    UINT32 u4Val;
#ifdef CC_MTK_LOADER            	
    UINT32 au1RecoveryBitData[2];
#endif
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
            //calvin add to clear Recovery Bit When AC On
			au1RecoveryBitData[0] = 0x00;
			EEPROM_Write((UINT64)(0x2803),(UINT32)au1RecoveryBitData,(UINT32)(1));
			//Clear IDX_CUSTOM_SHUTDOWN_OFFON_CNT
			au1RecoveryBitData[1] = 0x00;
			EEPROM_Write((UINT64)(0x2801),(UINT32)(au1RecoveryBitData+1),(UINT32)(1));                     
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
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_IRRC;
    }
}

void PDWNC_ATV_SetupPowerDown(UINT32 u4PowerDownEvent, UINT32 *pu4PowerDownEn)
{
    UNUSED(u4PowerDownEvent);
    UNUSED(pu4PowerDownEn);    
    return;
}

void PDWNC_ATV_ErrorCode(UINT8 u1ErrorCode)
{
    UNUSED(u1ErrorCode);
    return;
}

INT32 PDWNC_ATV_Set8032uPLedBlink(UINT32 u4BlinkTimes)
{
#ifdef CC_MTK_LOADER
    return 0;
#else
    UNUSED(PDWNC_SetSonyErrorCode(10, 60));
    return (PDWNC_SetSonyErrorCodeBlinking(u4BlinkTimes));
#endif /* CC_MTK_LOADER */
}
 

UINT32 PDWNC_ATV_GetStatus(UINT32 u4Addr, VOID* pInfo, UINT32 u4Size)
{
    UINT32 u4Value = 0, u4Ret = ATV_OK;
    ATV_ERR_DETECT_T *prErrorInfo = (ATV_ERR_DETECT_T *)pInfo;

    UNUSED(u4Addr);
    
    if ((ATV_CUSTOMER_CMD_TYPE)prErrorInfo->u4CmdType!= ATV_CUSTOMER_SELF_DIAGNOSTIC)
    {
#ifndef CC_MTK_LOADER    
        LOG(7, "[PDWNC_ATV_GetStatus] Invalid ATV_CUSTOME_CMD_TYPE %d\n", 
            u4Addr);
#endif /* CC_MTK_LOADER */
        return ATV_FAIL;
    }
    
    switch (prErrorInfo->u4ErrorType)
    {
    case ATV_12V_AMP_ERROR:
        //u4Value = PDWNC_ReadServoADCChannelValue(ATV_12V_AMP_DROP_DET);
        //u4Ret = (u4Value < ATV_12V_THRES_DEFAULT) ? ATV_FAIL : ATV_OK;
        break;
    case ATV_12V_SW_ERROR:
        //u4Value = PDWNC_ReadServoADCChannelValue(ATV_12V_SW_DROP_DET);
        //u4Ret = (u4Value < ATV_12V_THRES_DEFAULT) ? ATV_FAIL : ATV_OK;
        break;
    case ATV_AUD_ERROR:
        //u4Value = GPIO_Input(ATV_AUD_AMP_ERROR_DET);
        //u4Ret = (u4Value == 0) ? ATV_FAIL : ATV_OK;
        break;
    case ATV_3V3_5V_ERROR:
        //u4Value = GPIO_Input(ATV_V_DROP_DET);
        //u4Ret = (u4Value == 0) ? ATV_FAIL : ATV_OK;
        break;
    case ATV_BALANCER_ERROR:
        //u4Value = PDWNC_ReadServoADCChannelValue(ATV_BALANCE_DET);
        //if ((u4Value > ATV_BALANCE_HV_DEFAULT) ||
        //    (u4Value < ATV_BALANCE_LV_DEFAULT))
        //{
        //    u4Ret = ATV_FAIL;
        //}
        break;
    case ATV_TCON_ERROR:
        //u4Value = GPIO_Input(ATV_TCON_RDY_DET);
        //u4Ret = (u4Value == 0) ? ATV_FAIL : ATV_OK;
        break;
    case ATV_HFR_ERROR:
        break;
    case ATV_PANEL_NVM_ERROR:
        break;
    case ATV_PANEL_POWER_ERROR:
        //u4Value = GPIO_Input(ATV_PANEL_V_DROP_DET);
        //u4Ret = (u4Value == 0) ? ATV_FAIL : ATV_OK;
        break;
    case ATV_BACKLIGHT_ERROR:
        //u4Value = PDWNC_ReadServoADCChannelValue(ATV_BALANCE_DET);
        //u4Ret = (u4Value > ATV_BALANCE_HV_DEFAULT) ? ATV_FAIL : ATV_OK;
        break;
    case ATV_TEMPERATURE_ERROR:
        break;
    }
    
    prErrorInfo->u4DetValue = u4Value;
    return (u4Ret);
}

UINT32 PDWNC_ATV_LEDSet(UINT32 u4Addr, VOID* pInfo, UINT32 u4Size)
{
    D_LED_DISPLAY *prLEDInfo;

    UNUSED(u4Addr);
    if (u4Size != sizeof(D_LED_DISPLAY))
    {
        return 0;
    }

    prLEDInfo = (D_LED_DISPLAY *)pInfo;
    
#ifndef CC_MTK_LOADER    
    LOG(7, "[PDWNC_ATV_LEDSet] Set led %d as %d\n", 
        prLEDInfo->eLedType, prLEDInfo->eLedState);
#endif /* CC_MTK_LOADER */        

    ATVLEDCtrlFunc(prLEDInfo->eLedType, prLEDInfo->eLedState);
    return 0;
}

#ifdef CC_MTK_LOADER
static void LOADER_ATV_TimerOn(void)
{
    UINT32 u4Val;

    // Enable timer 0
    u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Val |= ((TMR1_CNTDWN_EN) | (TMR1_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);
}

static void LOADER_ATV_TimerOff(void)
{
    UINT32 u4Val;

    // disable timer 0
    u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Val &= ~((TMR1_CNTDWN_EN) | (TMR1_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);            
}

static void LOADER_ATV_TimerIsr(void)
{
    if (BIM_IsIrqPending(VECTOR_T1))
    {
        D_LED_TYPE eLedType;
        if (_fgLedAllBlink)
        {
            ATV_LEDBlinkCtrl(D_LED_TOTAL);
        }
        else
        {
            for (eLedType = D_LED_POWER; eLedType <= D_LED_TIMER; eLedType++)
        {
            if (_fgLedBlink[eLedType] == TRUE)
            {
                ATV_LEDBlinkCtrl(eLedType);
            }
        }
        }
        //_NonOSTimerOff();        
        BIM_ClearIrq(VECTOR_T1);        
    }
    
    if (_pfnATVNextIsr) { _pfnATVNextIsr(); }
}
#endif /* CC_MTK_LOADER */

static void LOADER_ATV_EnvInitFunc(void)
{
    D_LED_TYPE eLedType;
    for (eLedType = D_LED_POWER; eLedType < D_LED_TOTAL; eLedType++)
    {
        _fgLedOn[eLedType] = FALSE;
        _fgLedBlink[eLedType] = FALSE;
    }
    
    ATV_InitGPIOTimer();
}

static void ATV_InitGPIOTimer(void)
{
#ifdef CC_MTK_LOADER
    UINT32 u4BusClock;
    UINT32 u4TimerClock;
    
    _pfnATVNextIsr = RegisterIrqHandler(LOADER_ATV_TimerIsr);

    u4BusClock = BSP_GetDomainClock(SRC_BUS_CLK);
    u4TimerClock = (UINT32)(u4BusClock / 1000);
    u4TimerClock *= ATV_USB_UPGRADE_BLINK_PERIOD;
//    Printf("[ATV_InitGPIOTimer] u4TimerClock = %d\n", u4TimerClock);
    
    // Setup Timer 0 and triger 1st interrupt.
    BIM_WRITE32(REG_RW_TIMER1_LLMT, u4TimerClock);
    BIM_WRITE32(REG_RW_TIMER1_HLMT, 0);
    BIM_WRITE32(REG_RW_TIMER1_LOW , u4TimerClock);
    BIM_WRITE32(REG_RW_TIMER1_HIGH, 0);

    LOADER_ATV_TimerOff();
    
    BIM_EnableIrq(VECTOR_T1);

    HalEnableIRQ();

//    LOADER_ATV_TimerOn();
#else
#endif /* CC_MTK_LOADER */
}

#endif /* CC_MTK_PRELOADER */

