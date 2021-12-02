
#include "x_pdwnc.h"
#include "x_ckgen.h"
#include "x_timer.h"
#include "x_debug.h"
#include "x_hal_io.h"
#include "x_dram.h"
#include "x_irq.h"
#include "x_pwr_ctrl.h"
#include "x_pinmux.h"
#include "drv_pwm.h"
#include "pdwnc_if.h"
#include "eeprom_if.h"
#include "nptv_vdec_if.h"
#include "PowerSeq.h"
#include "CustomCtrl.h"
#include "Walkure_gpio.h"
#include "Walkure_pdwnc.h"
#include "c_model.h"
#include "codec_mt5365.h"
#include "Walkure_pin_setup.h"
#if !defined(CC_MTK_LOADER) && !defined(CC_MTK_IR_ENABLE)
#include "u_irrc_btn_def.h"
#include "ir_if.h"
#include "sirc_ir_tbl.h"
#endif


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#if !defined(CC_MTK_LOADER) && !defined(CC_MTK_IR_ENABLE)
#define KEY_EQUAL(x, y) ((x)==(y))

static const KEYARRAY_T arKeyArray[] =
{
    { 0x1, (sizeof(_arpulsecheckKeyMap01)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMap01 },    
    { 0x3, (sizeof(_arpulsecheckKeyMap03)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMap03 },      	
    { 0x4, (sizeof(_arpulsecheckKeyMap04)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMap04 },
    { 0x1A, (sizeof(_arpulsecheckKeyMap1A)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMap1A },
    { 0x77, (sizeof(_arpulsecheckKeyMap77)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMap77 },
    { 0x97, (sizeof(_arpulsecheckKeyMap97)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMap97 },
    { 0xA4, (sizeof(_arpulsecheckKeyMapA4)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMapA4 },     
};
#endif

#define PDWNC_CMD_ARMT8032_IR_WAKEUP_KEY    (PDWNC_CMD_ARMT8032_MAX + 1)
#define T8032_IR_WAK_POWER                  0x01
#define T8032_IR_WAK_CH_UP_DOWN             0x02    
#define T8032_IR_WAK_NUMERIC                0x04        
#define T8032_IR_WAK_DTV                    0x08    
#define T8032_IR_WAK_ATV                    0x10    
#define T8032_IR_WAK_JUMP                   0x20
#define T8032_IR_WAK_ADTV_SW                0x40
#define PDWNC_CMD_ARMT8032_DELAY_WAKEUP_TIMER    (PDWNC_CMD_ARMT8032_MAX + 2)
//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------
static void Custom_InitGPIOTimer(void);

//-----------------------------------------------------------------------------
// Imported funtions 
//-----------------------------------------------------------------------------
extern UINT32 IRRC_IsPowerButtonOff(void);

extern BOOL _fgLedBlink[D_LED_TOTAL + 1];
extern BOOL _fgLedOn[D_LED_TOTAL + 1];
extern BOOL _fgLedAllBlink;
extern BOOL _fgStandbyDiable;
extern UINT32 _u4gModelType;
INT32 PDWNC_SetSonyErrorCode(UINT8 u1Period, UINT8 u1Interval);
INT32 PDWNC_SetSonyErrorCodeBlinking(UINT8 u1times);
#ifndef CC_MTK_PRELOADER
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static PDWNC_FuncTbl rPDWNC_EXT_FuncTbl = 
{
    PDWNC_Custom_Init,                  // pfnInit
    PDWNC_Custom_SetupPowerDown,        // pfnPowerDown
    PDWNC_Custom_ReadWakeupReason,      // pfnReadWakeupReason
    PDWNC_Custom_WatchDogInit,          // pfnWatchDogInit;
    PDWNC_Custom_ErrorHandleInit,       // pfnErrorHandleInit
    PDWNC_Custom_ErrorCode,             // pfnWriteErrorCode
    NULL,                               // pfnReadErrorCode
    NULL,                               // pfnGetCurrentState
    NULL,                               // pfnDetermineNextState
    NULL,                               // pfnEnterNextState
    PDWNC_Custom_Set8032uPLedBlink,     // pfnSet8032uPLedBlink
    NULL,                               // pfnCustomRead
    NULL                                // pfnCustomWrite
};

#ifdef CC_MTK_LOADER
static PFN_IRQ_HANDLER _pfnCustomNextIsr = NULL;
#endif /* CC_MTK_LOADER */

static UINT32 _fgPseudoWatchdogAssert = CC_PSEUDO_WATCHDOG_ASSERT;
//static BOOL   _fgDebug = FALSE;
#if !defined(CC_MTK_LOADER) && !defined(CC_MTK_IR_ENABLE)
static UINT32 _fgEverReqWakeupReason = 0;
static UINT32 _fgT8032WakeupReason = (UINT32)PDWNC_WAKE_UP_REASON_IRRC;
#endif

static BOOL   _fgIsNormalMode = TRUE;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
#ifdef CUSTOM_SELF_DIAG_ENABLE
static UINT32 u4GetLEDBlinkCode(SELF_DIAG_TABLE eErrorLog)
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
    else if (eErrorLog.u2PanelPowerError)
    {
        u4LEDBlinkTime = 5;
    }
    else if (eErrorLog.u2BacklightError)
    {
        u4LEDBlinkTime = 6;
    }
    
    return u4LEDBlinkTime;
}

static BOOL CustomCheckShutdown(void)
{
    BOOL   bfgIsEmpty = TRUE;
    BOOL   bfgShutdown = FALSE;
    UINT8  u1DataInit = 0;
    UINT8  au1Data[SELF_DIAG_EEPROM_SIZE];
    UINT8  u1RetryLimit = 0, u1RetryCount = 0;
    UINT16 u2ShutdownLog = 0, u2Recovery = 0;
    UINT32 u4BootCounter = 0;
    UINT32 u4Len = sizeof(au1Data);
    UINT32 i;
    
    x_memset((void *)au1Data, 0xFF, u4Len);
    
    // Get Self-Diag data init bit from EEPROM
    UNUSED(EEPROM_Read(SELF_DIAG_DATA_INIT_ADDR,
                      (UINT32)(VOID *)&u1DataInit, sizeof(UINT8)));

    if (!(u1DataInit & SELF_DIAG_LOAD_DEFAULT))
    {
        // Get Self-Diag data from EEPROM
        UNUSED(EEPROM_Read(SELF_DIAG_DET_ENABLE_ADDR,
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
    }

    if (bfgIsEmpty)
    {
        return FALSE;
    }

    // Read the boot counter from EEPROM
    UNUSED(EEPROM_Read(SELF_DIAG_BOOT_CNT_ADDR, 
                       (UINT32)(VOID *)&u4BootCounter, sizeof(UINT32)));
    
    // Increase the boot counter & update to EEPROM
    u4BootCounter++;
    UNUSED(EEPROM_Write(SELF_DIAG_BOOT_CNT_ADDR, 
                        (UINT32)(VOID *)&u4BootCounter, sizeof(UINT32)));

    // Read the shutdown log from EEPROM
    UNUSED(EEPROM_Read(SELF_DIAG_ERROR_LOG_ADDR, 
                       (UINT32)(VOID *)&u2ShutdownLog, sizeof(UINT16)));

    // No error
    if (u2ShutdownLog == 0)
    {
        return FALSE;
    }

#ifdef CUSTOM_SELF_DIAG_ENABLE
    if (PDWNC_ReadWakeupReason() == PDWNC_WAKE_UP_REASON_AC_POWER)
    {   
        UINT8 u1Val = 0;
        
        // Get Shutdown state from EEPROM
        UNUSED(EEPROM_Read(SELF_DIAG_SHUTDOWN_OCCUR_ADDR,
                           (UINT32)(VOID *)&u1Val, sizeof(UINT8)));
        if (u1Val & CUSTOM_SHUTDOWN_ERROR)
        {
            Printf("%s(%d) Self_diag shutdown by CUSTOM_SHUTDOWN_ERROR (%d)\n", __FILE__, __LINE__, u1Val);
            bfgShutdown = TRUE;
        }
    }
#endif /* CUSTOM_SELF_DIAG_ENABLE */
    
    // Read the recovery table from EEPROM
    UNUSED(EEPROM_Read(SELF_DIAG_BOOT_RETRY_ADDR, 
                       (UINT32)(VOID *)&u2Recovery, sizeof(UINT16)));

    // There is a shutdown log but its recovery bit is disable.
    // So, system need to go to self shutdown
    if ((u2ShutdownLog & u2Recovery) > 0)
    {
        Printf("%s(%d) Self_diag shutdown by RECOVERY disalbe\n", __FILE__, __LINE__);
        Printf("u2ShutdownLog(%d), u2Recovery(%d)\n", u2ShutdownLog, u2Recovery);
        bfgShutdown = TRUE;
    }

    // Start to retry from safety shutdown
    
    // Read the retry limitation from EEPROM
    // If the retry limit = 0 -> system does not receive recovery command
    UNUSED(EEPROM_Read(SELF_DIAG_RETRY_LIMIT_ADDR, 
                       (UINT32)(VOID *)&u1RetryLimit, sizeof(UINT8)));
    if (u1RetryLimit == 0)
    {
        Printf("%s(%d) Self_diag shutdown by Retry limit is 0\n", __FILE__, __LINE__);
        bfgShutdown = TRUE;
    }

    // If the retry limit > 0, read the current retry counter from EEPROM
    // Check if the counter reaches the retry limitation.
    // if yes, system does not recovery any more
    UNUSED(EEPROM_Read(SELF_DIAG_RETRY_CNT_ADDR, 
                       (UINT32)(VOID *)&u1RetryCount, sizeof(UINT8)));
    if (u1RetryCount == u1RetryLimit)
    {
        Printf("%s(%d) Self_diag shutdown by Retry limit equal with retry cnt\n", __FILE__, __LINE__);
        Printf("u1RetryCount(%d), u1RetryLimit(%d)\n", u1RetryCount, u1RetryLimit);
        bfgShutdown = TRUE;
    }

    if (bfgShutdown)
    {
        SELF_DIAG_TABLE eShutdownLog;
        UINT32 u4LEDErrorCode;

        x_memcpy(&eShutdownLog, &u2ShutdownLog, sizeof(SELF_DIAG_TABLE));
        u4LEDErrorCode = u4GetLEDBlinkCode(eShutdownLog);
        PDWNC_Custom_Set8032uPLedBlink(u4LEDErrorCode);
    }
    
    return bfgShutdown;    
}
#endif /* CUSTOM_SELF_DIAG_ENABLE */

//-----------------------------------------------------------------------------
/** _IRRX_CrystalLookup()
 */
//-----------------------------------------------------------------------------
#if !defined(CC_MTK_LOADER) && !defined(CC_MTK_IR_ENABLE)
static UINT32 _SirCrystalLookup(const KEYUNIT_T *prKeyMap,
                                UINT32 u4Size, UINT32 u4Data)
{
    UINT32 u4Min, u4Max, u4Avg;

    if (prKeyMap == NULL)
    {
        return BTN_INVALID;
    }

    if(u4Size == 1)
    {
        if (KEY_EQUAL(prKeyMap[0].u4Key, u4Data))
        {
            return prKeyMap[0].u4Crystal;
        }        
        else
        {
            return BTN_INVALID;
        }
    }

    u4Min = 0;
    u4Max = u4Size - 1;

    while (u4Max > u4Min)
    {
        u4Avg = (u4Max + u4Min) / 2;

        if (KEY_EQUAL(prKeyMap[u4Min].u4Key, u4Data))
        {
            return prKeyMap[u4Min].u4Crystal;
        }

        if (KEY_EQUAL(prKeyMap[u4Max].u4Key, u4Data))
        {
            return prKeyMap[u4Max].u4Crystal;
        }

        if (KEY_EQUAL(prKeyMap[u4Avg].u4Key, u4Data))
        {
            return prKeyMap[u4Avg].u4Crystal;
        }
        else if (prKeyMap[u4Avg].u4Key > u4Data)
        {
            u4Max = u4Avg;
        }
        else if (prKeyMap[u4Avg].u4Key < u4Data)
        {
            if (u4Min == u4Avg)
            {
                u4Max = u4Avg;
            }
            else
            {
                u4Min = u4Avg;
            }
        }
    }
    return BTN_INVALID;
}

static UINT32 _SirRawKey2WakeupReason(UINT8 u1IrKey)
{
    UINT32 u4Key;
    u4Key =  _SirCrystalLookup(arKeyArray[0].prKeyMap, arKeyArray[0].u4Size,
                               (UINT32)(u1IrKey & 0x7f));
    if(u4Key == BTN_POWER || u4Key == BTN_POWER_ON)
    {
        return PDWNC_WAKE_UP_REASON_IRRC;
    }
    else if(u4Key == BTN_PRG_UP)
    {
        return PDWNC_WAKE_UP_REASON_RC_PRG_UP;
    }
    else if(u4Key == BTN_PRG_DOWN)
    {
        return PDWNC_WAKE_UP_REASON_RC_PRG_DOWN;
    }
    else if(u4Key == BTN_DIGIT_0)
    {
        return PDWNC_WAKE_UP_REASON_RC_DIGIT_0;
    }
    else if(u4Key == BTN_DIGIT_1)
    {
        return PDWNC_WAKE_UP_REASON_RC_DIGIT_1;
    }
    else if(u4Key == BTN_DIGIT_2)
    {
        return PDWNC_WAKE_UP_REASON_RC_DIGIT_2;
    }
    else if(u4Key == BTN_DIGIT_3)
    {
        return PDWNC_WAKE_UP_REASON_RC_DIGIT_3;
    }
    else if(u4Key == BTN_DIGIT_4)
    {
        return PDWNC_WAKE_UP_REASON_RC_DIGIT_4;
    }
    else if(u4Key == BTN_DIGIT_5)
    {
        return PDWNC_WAKE_UP_REASON_RC_DIGIT_5;
    }
    else if(u4Key == BTN_DIGIT_6)
    {
        return PDWNC_WAKE_UP_REASON_RC_DIGIT_6;
    }
    else if(u4Key == BTN_DIGIT_7)
    {
        return PDWNC_WAKE_UP_REASON_RC_DIGIT_7;
    }
    else if(u4Key == BTN_DIGIT_8)
    {
        return PDWNC_WAKE_UP_REASON_RC_DIGIT_8;
    }
    else if(u4Key == BTN_DIGIT_9)
    {
        return PDWNC_WAKE_UP_REASON_RC_DIGIT_9;
    }
    else
    {
        return PDWNC_WAKE_UP_REASON_IRRC;
    }
        
}
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
BOOL PDWNC_Check_Enter_SemiStandy(void)
{
    UINT32 u4WakeupReason = PDWNC_ReadWakeupReason();
    DTVCFG_T rDtvCfg;

    x_memset(&rDtvCfg, 0, sizeof(DTVCFG_T));
    UNUSED(EEPDTV_GetCfg(&rDtvCfg));
    
    if ((u4WakeupReason == PDWNC_WAKE_UP_REASON_RTC) &&
        (rDtvCfg.u1Flags2 & DTVCFG_FLAG2_QUIET_BOOT))
    {
        return TRUE;
    }

    return FALSE;
}

PDWNC_FuncTbl* PDWNC_EXT_GetFunc(void)
{
    return (&rPDWNC_EXT_FuncTbl);
}

void PDWNC_Custom_Init(void)
{
#ifndef CC_MTK_LOADER
    UNUSED(PDWNC_SetSonyErrorCode(CUSTOM_BLINK_TIME, CUSTOM_BLINK_INVERAL));
#endif /* CC_MTK_LOADER */

    return;
}

void PDWNC_Custom_WatchDogInit(void)
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

void PDWNC_Custom_ErrorHandleInit(void)
{
    return;
}

void PDWNC_Custom_ReadWakeupReason(UINT32* pu4Reason)
{
    static BOOL bInit = TRUE;
    static UINT32 u4BootReason = (UINT32)PDWNC_WAKE_UP_REASON_IRRC;
    UINT32 u4Val;
    DTVCFG_T rDtvCfg;
    static INT32 i4PowerKeyPad = -1;
    INT32 i;
    SGL_GPIO_T   *prSingleGpioKeyList;
    
    if (!bInit)
    {
        *pu4Reason = u4BootReason;
        return;
    }
    
    if(i4PowerKeyPad < 0)
    {
        i = DRVCUST_InitQuery(eSingleGpioKeyList, (UINT32 *)(void *)&prSingleGpioKeyList);
        if(i < 0)
        {
            Printf("Warning!! can't find out keypad information\n");
        }
        else
        {
            i = 0;
            while ((prSingleGpioKeyList) &&
                    (prSingleGpioKeyList[i].u4Key != 0) &&
                    (prSingleGpioKeyList[i].u4Key != DKBIT_NONE))
            {
                if (prSingleGpioKeyList[i].u4Key & DKBIT_POWER)
                {
                    i4PowerKeyPad = prSingleGpioKeyList[i].u4Gpio - OPCTRL(0);
                    Printf("Power keypad GPIO is %d\n", prSingleGpioKeyList[i].u4Gpio);
                }
                i++;
            }
        }
        
    }
    
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
#ifdef CUSTOM_SELF_DIAG_ENABLE
        UINT8  u1ShutdownState = 0;
        UNUSED(EEPROM_Read(SELF_DIAG_SHUTDOWN_OCCUR_ADDR, 
                           (UINT32)(VOID *)&u1ShutdownState, sizeof(UINT8)));
        if (u1ShutdownState & CUSTOM_SHUTDOWN_AC_DET)
        {
            *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_RTC_SPECIAL;
        }
        else
#endif /* CUSTOM_SELF_DIAG_ENABLE */
        {
            if (EEPDTV_GetCfg(&rDtvCfg) == 0)
            {
                if (rDtvCfg.u1Flags2 & DTVCFG_FLAG2_QUIET_BOOT)
                {
                    UNUSED(_fgIsNormalMode);
                    _fgIsNormalMode = FALSE; // enter semi-standby
                }
            }
            
            *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_RTC;
        }
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
    else if (u4Val & WAK_GPIOX((UINT8)i4PowerKeyPad))     // Button power
    {
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_KEYPAD;
    }
#if 0
#ifdef WAFFLE_PCB_EVT_CHM
    else if (u4Val & WAK_GPIO3)     // DVD insert
    {
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_DVD;
    }
#endif /* WAFFLE_PCB_EVT_CHM */
#endif
    else // if (u4Val & WAK_IRRX)
    {
#if !defined(CC_MTK_LOADER) && !defined(CC_MTK_IR_ENABLE)

        PDWNC_T8032_CMD_T rCmd;
        PDWNC_T8032_RESPONSE_T rResponse;
        if (_fgEverReqWakeupReason == 0)
        {
            x_memset(&rCmd, 0, sizeof(PDWNC_T8032_CMD_T));
            rCmd.u1Cmd = PDWNC_CMD_ARMT8032_READ_WAKEUP_REASON;
            *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_IRRC;
            
            if (PDWNC_T8032Cmd(&rCmd, &rResponse) == 0)
            {
                switch (rResponse.au1Data[2])
                {
                case CUSTOM_SERVICE_MODE:
                    *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_CUSTOM_1;
                    break;
                case CUSTOM_SELF_DIAGNOSTIC_MODE:
                    *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_CUSTOM_2;
                    break;
                case CUSTOM_ACTIVE_HOTEL_MODE:
                    *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_CUSTOM_3;
                      
                    if (EEPDTV_GetCfg(&rDtvCfg) == 0)
                    {
                        rDtvCfg.u1Flags2 |= DTVCFG_FLAG2_DISABLE_LOGO;
                        UNUSED(EEPDTV_SetCfg(&rDtvCfg));
                    }
                    break;
                case CUSTOM_EXIT_HOTEL_MODE:
                    *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_CUSTOM_4;
                       
                    if (EEPDTV_GetCfg(&rDtvCfg) == 0)
                    {
                        rDtvCfg.u1Flags2 &= ~DTVCFG_FLAG2_DISABLE_LOGO;
                        UNUSED(EEPDTV_SetCfg(&rDtvCfg));
                    }
                    break;
                default:
                    //david: Need to get detail spec. to return correct reason 20090913
                    if (rResponse.au1Data[0] == PDWNC_WAKE_UP_REASON_IRRC)
                    {
                        *pu4Reason = _SirRawKey2WakeupReason(rResponse.au1Data[1]);
                        Printf("IR wakeup key code: 0x%2x, reason: %d\n",
                               rResponse.au1Data[1], *pu4Reason);
                    }
                    else if (rResponse.au1Data[0] == T8032_WAKEUP_REASON_CEC_ONE_TOUCH_PLAY)
                    {
                        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_HDMI;
                    }                        
                    else if(rResponse.au1Data[0] == PDWNC_WAKE_UP_REASON_UART_NORMAL)
                    {
                        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_UART;
                    }
                    break;
                }
            }

            _fgEverReqWakeupReason = 1;
            _fgT8032WakeupReason = *pu4Reason;
            Printf("Wakeup reason from T8032 is %d\n", *pu4Reason);            
        }
        else
        {
            *pu4Reason = (UINT32)_fgT8032WakeupReason;
            Printf("Wakeup reason is %d\n", *pu4Reason);
        }

#else
        *pu4Reason = (UINT32)PDWNC_WAKE_UP_REASON_IRRC;
#endif
    }

    bInit = FALSE;
    u4BootReason = *pu4Reason;
}
UINT8 u1DisableWakeupCnt = 0;
void PDWNC_Custom_SetupPowerDown(UINT32 u4PowerDownEvent, UINT32 *pu4PowerDownEn)
{
    UINT32 u4Addr;
    UINT32 u4Val2,u4Val3;
    UINT8 au1Data[6];
    PDWNC_T8032_CMD_T rCmd;
    
    UNUSED(u4PowerDownEvent);
    UNUSED(pu4PowerDownEn);    

#ifdef CC_IR_POWER_BUTTON_RECEIVE_PRELOADER
    Printf("T8032 will disable HW IRRX wakeup!\n");
#endif
    
    // If the next power state is standby, enable CEC.
    // Otherwise, disable CEC
#ifdef CUSTOM_POWER_STATE_CTRL_ENABLE
    if (_u4PowerState == STATE_STANDBY)
#endif /* CUSTOM_POWER_STATE_CTRL_ENABLE */        
    {
        // Write CEC configuration to uP
        if((DRVCUST_InitGet(eT8032uPOffset) != 0) && ((u4Addr = DRVCUST_InitGet(eEepromCECMenuLanguageOffset)) != 0x0) &&
        ((DRVCUST_InitGet(eEepromCECEnableOffset)) == 0x00) && ((DRVCUST_InitGet(eEepromCECOneTouchPlayEnableOffset)) == 0x00))
        {
            EEPROM_Read(u4Addr, (UINT32)au1Data, 6);		
            rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_MENU_LANGUAGE;
            rCmd.u1SubCmd = 0;
            x_memcpy(rCmd.au1Data, au1Data, 4);
            PDWNC_T8032Cmd(&rCmd, NULL);

            rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_CEC;
            x_memcpy(rCmd.au1Data, &au1Data[4], 1);
            PDWNC_T8032Cmd(&rCmd, NULL);

            rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_ONE_TOUCH_PLAY;
            x_memcpy(rCmd.au1Data, &au1Data[5], 1);
            PDWNC_T8032Cmd(&rCmd, NULL);		
        }
        else if((DRVCUST_InitGet(eT8032uPOffset) != 0) && ((u4Addr= DRVCUST_InitGet(eEepromCECMenuLanguageOffset)) != 0x0) &&
                ((u4Val2 = DRVCUST_InitGet(eEepromCECEnableOffset)) != 0x00) && ((u4Val3 = DRVCUST_InitGet(eEepromCECOneTouchPlayEnableOffset)) != 0x00))
        {
            EEPROM_Read(u4Addr, (UINT32)au1Data, 4);		
            rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_MENU_LANGUAGE;
            rCmd.u1SubCmd = 0;
            x_memcpy(rCmd.au1Data, au1Data, 4);
            PDWNC_T8032Cmd(&rCmd, NULL);
		
            EEPROM_Read(u4Val2, (UINT32)au1Data, 1);		  
            rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_CEC;
            au1Data[1]=0;//----youneng--0427
            au1Data[2]=0;//----youneng--0427
            au1Data[3]=0;//----youneng--0427
            x_memcpy(rCmd.au1Data, au1Data, 4);//----youneng--0427
            PDWNC_T8032Cmd(&rCmd, NULL);

            EEPROM_Read(u4Val3, (UINT32)au1Data, 1);		
            rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_ONE_TOUCH_PLAY;
            x_memcpy(rCmd.au1Data, au1Data, 1);
            PDWNC_T8032Cmd(&rCmd, NULL);		
        }
	
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_T8032_CTL_CEC; //Notify T8032 start/stop control CEC
        rCmd.u1SubCmd = 0;
        rCmd.au1Data[0] = 1;	//T8032 control CEC
        PDWNC_T8032Cmd(&rCmd, NULL);
    }

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_IR_WAKEUP_KEY;
    rCmd.u1SubCmd = (UINT8)(T8032_IR_WAK_CH_UP_DOWN | T8032_IR_WAK_POWER);

    if (CustomPCBAQueryCountry() == DRV_CUSTOM_COUNTRY_PAA)
    {
        rCmd.u1SubCmd |= (UINT8)(T8032_IR_WAK_NUMERIC);
    }

    printf("Set IR wakeup key 0x%2x\n", rCmd.u1SubCmd);
    rCmd.au1Data[0] = rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;	 
    PDWNC_T8032Cmd(&rCmd, NULL);

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_DELAY_WAKEUP_TIMER;
    if(u1DisableWakeupCnt % 2)
    {
        rCmd.u1SubCmd = u1DisableWakeupCnt;// 200 ==> 4 seconds (unit 20ms)
    }
    else
    {
        rCmd.u1SubCmd = 5;// ==> 100ms (unit 20ms)
    }
    printf("Disable Wakeup %d ms\n", (rCmd.u1SubCmd) * 20);
    rCmd.au1Data[0] = (UINT8)((*pu4PowerDownEn) & 0x0000000ff);
    rCmd.au1Data[1] = (UINT8)(((*pu4PowerDownEn) >> 8) & 0x0000000ff);
    rCmd.au1Data[2] = (UINT8)(((*pu4PowerDownEn) >> 16) & 0x0000000ff);        
    rCmd.au1Data[3] = (UINT8)(((*pu4PowerDownEn) >> 24) & 0x0000000ff);            
    if(rCmd.u1SubCmd != 0)
    {
        *pu4PowerDownEn = 0;
    }
    PDWNC_T8032Cmd(&rCmd, NULL);
    
    vIO32WriteFldAlign(PDWNC_SADCCFG0, 0x1, FLD_RG_SRV_PWD_B);
    vIO32WriteFldAlign(PDWNC_SADCCFG0, 0x0, FLD_RG_SRV_INBUFF_PD);
    vIO32WriteFldAlign(PDWNC_CLKPDN, 0x0, FLD_SRVPD);
    vIO32WriteFldAlign(PDWNC_SRVCFG0, 0x1, FLD_HWEN);

#if !defined(CC_MTK_LOADER) & defined (CUSTOM_SELF_DIAG_ENABLE)
    SelfDiagUpdateOnOffCount();
#endif /* !defined(CC_MTK_LOADER) & defined (CUSTOM_SELF_DIAG_ENABLE) */
    return;
}

void PDWNC_Custom_ErrorCode(UINT8 u1ErrorCode)
{
    UNUSED(u1ErrorCode);
    return;
}

//-----------------------------------------------------------------------------
/** PDWNC_SetErrorCode to set system error code.
 *  @param u1ErrorCode the error code.
 * au1Data[0] : Servo_gpio (1) or OPCTRL (0)
 * au1Data[1] : offset gpio number of servo or opctrl
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_SetSonyErrorCode(UINT8 u1Period, UINT8 u1Interval)
{
    PDWNC_T8032_CMD_T rCmd;
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_ERROR_CODE;//PDWNC_CMD_ARMT8032_SET_SONY_ERROR_CODE;
//    rCmd.u1SubCmd = u1ErrorCode;
    rCmd.au1Data[0] = rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;    
    
    if(DRVCUST_InitGet(eT8032CtrlErrLed) && DRVCUST_InitGet(eT8032CtrlErrLedOnVal))
    {
       UINT32 u4T8032CtrlGpio, u4OnVal;
       DRVCUST_InitQuery(eT8032CtrlErrLed, &u4T8032CtrlGpio);
       DRVCUST_InitQuery(eT8032CtrlErrLedOnVal, &u4OnVal);
       Printf("Set t8032 ctrl gpio %d (On val: %d, Para1: %d, Para2: %d)\n", u4T8032CtrlGpio, u4OnVal, u1Period, u1Interval);
       if(u4T8032CtrlGpio >= SERVO(0))
       {
            u4T8032CtrlGpio -= SERVO(0);
            rCmd.u1SubCmd = 1; // TYPE (1: SERVO, 0: OPCTRL)
            rCmd.au1Data[0] = (UINT8)u4T8032CtrlGpio; //OFFSET GPIO INDEX
            rCmd.au1Data[1] = (UINT8)u4OnVal; // POLARITY
            rCmd.au1Data[2] =  u1Period;
            rCmd.au1Data[3] =  u1Interval;             
       }
       else if(u4T8032CtrlGpio >= OPCTRL(0))
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
}

//-----------------------------------------------------------------------------
/** PDWNC_SetErrorCodeBlinking to enable system error code blinking function.
 *  @param u1ErrorCode the error code.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_SetSonyErrorCodeBlinking(UINT8 u1times)
{
    PDWNC_T8032_CMD_T rCmd;
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_ERROR_CODE_BLINKING;
    rCmd.u1SubCmd = u1times;
    rCmd.au1Data[0] = rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;    
    return PDWNC_T8032Cmd(&rCmd, NULL);
}


INT32 PDWNC_Custom_Set8032uPLedBlink(UINT32 u4BlinkTimes)
{
    UNUSED(PDWNC_SetSonyErrorCode(CUSTOM_BLINK_TIME, CUSTOM_BLINK_INVERAL));
    return (PDWNC_SetSonyErrorCodeBlinking(u4BlinkTimes));
}

#ifdef CC_MTK_LOADER
void LOADER_Custom_TimerOn(void)
{
    UINT32 u4Val;

    // Enable timer 0
    u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Val |= ((TMR1_CNTDWN_EN) | (TMR1_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);
}

void LOADER_Custom_TimerOff(void)
{
    UINT32 u4Val;

    // disable timer 0
    u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Val &= ~((TMR1_CNTDWN_EN) | (TMR1_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);            
}

static void LOADER_Custom_TimerIsr(void)
{
    if (BIM_IsIrqPending(VECTOR_T1))
    {
        D_LED_TYPE eLedType;
        if (_fgLedAllBlink)
        {
            CustomLEDBlinkCtrl(D_LED_TOTAL);
        }
        else
        {
            for (eLedType = D_LED_POWER; eLedType <= D_LED_TIMER; eLedType++)
            {
                if (_fgLedBlink[eLedType] == TRUE)
                {
                    CustomLEDBlinkCtrl(eLedType);
                }
            }
        }
        //_NonOSTimerOff();        
        BIM_ClearIrq(VECTOR_T1);        
    }
    
    if (_pfnCustomNextIsr) { _pfnCustomNextIsr(); }
}
#endif /* CC_MTK_LOADER */

void LOADER_Custom_EnvInitFunc(void *prLdrData, UINT32 fgValidFlag)
{
#ifdef CC_MTK_LOADER
    BOOL   bUpgradeBreak;
    UINT8  u1UpgradeState = 0;
    UINT32 u4UpgradeGoAddr;
#endif    
    UINT32 u4Val;
    if (fgValidFlag == 0)
    {
    
    if(sizeof(_gu4SysPinmuxInit))
    {
        for(u4Val = 0;u4Val < sizeof(_gu4SysPinmuxInit)/(sizeof(_gu4SysPinmuxInit[0])); u4Val++)
        {
            BSP_PinSet(_gu4SysPinmuxInit[u4Val][0], _gu4SysPinmuxInit[u4Val][1]);
        }
    }

#ifdef CC_WAFFLE_PA_ANALOG_ONLY_RC_SUPPORT
        INT32 i4Value = 1;
        UNUSED(GPIO_Output(SIGNAL_X_U_RST, &i4Value));
        PRINT_TIMESTAMP_DBG_MSG("[Power On]", "X_U_RST = %d\n", i4Value);
#endif /* CC_WAFFLE_PA_ANALOG_ONLY_RC_SUPPORT */

        // PWM0 for ESD
        // Calculation       
        UINT32 u4Clock, u4TimeBase;
        UINT32 u4NewFreq, u4Freq;
        UINT32 u4Duty, u4DutyOn, u4DutyAll;
        
        u4Freq = CUSTOM_ESD_PWM_FREQ;
        u4Duty = CUSTOM_ESD_PWM_DUTY_CYLE;
        u4Clock = BSP_GetDomainClock(SRC_BUS_CLK);
        u4TimeBase = ((float)(u4Clock >> 8) / (float)u4Freq);
        u4NewFreq = (u4Clock >> 8) / (u4TimeBase + 1);
        u4DutyAll = (u4NewFreq * 255) / u4Freq;
        u4DutyOn = (u4DutyAll * u4Duty) / 100;
    
        vDrvSetPWM(CUSTOM_ESD_PWM_PORT, u4TimeBase, u4DutyOn, u4DutyAll);
        return;
    }

#ifdef CC_MTK_LOADER
    UINT32 u4Value = PDWNC_ReadWakeupReason();
    
    if ((u4Value == PDWNC_WAKE_UP_REASON_AC_POWER) ||
        (u4Value == PDWNC_WAKE_UP_REASON_WATCHDOG))
    {
        vInternalEDIDInit();
    }

    if ((u4Value == PDWNC_WAKE_UP_REASON_AC_POWER) &&
        (CustomPCBAQueryCountry() == DRV_CUSTOM_COUNTRY_COL))
    {
        LDR_DATA_T *prLoaderData = prLdrData;
        BOOL bIsBoot = FALSE;
        
        bIsBoot = (prLoaderData->rDtvCfg.u1Flags & DTVCFG_FLAG_POWERON_BOOT) ? 
                  TRUE : FALSE;
        if (!bIsBoot)
        {
            // Always enter the power down state in COL model
            PDWNC_EnterPowerDown((WAK_IRRX | WAK_UNOR), PDWNC_POWER_DOWN_IRRC);
        }
    }
#endif /* CC_MTK_LOADER */

    GPIO_CustomReadStrapPin();

#ifdef CUSTOM_SELF_DIAG_ENABLE
    if (CustomCheckShutdown())
    {
        PDWNC_EnterPowerDown((WAK_IRRX | WAK_UNOR), PDWNC_POWER_DOWN_IRRC);
    }
#endif /* CUSTOM_SELF_DIAG_ENABLE */

#ifdef CC_MTK_LOADER
    if (DRVCUST_InitQuery(eLoaderUpgradeBreakStateOffset, &u4UpgradeGoAddr) == 0)
    {
        UNUSED(EEPROM_Read(u4UpgradeGoAddr, 
                           (UINT32)(void *)&u1UpgradeState, sizeof(UINT8)));
        if (!((u1UpgradeState == DTVCFG_FLAG_USB_UPGRADE_ON) ||
              (u1UpgradeState == 0)))
        {
            // Clear the status if it is invalid
            u1UpgradeState = 0;
            UNUSED(EEPROM_Write(u4UpgradeGoAddr, 
                                (UINT32)(void *)&u1UpgradeState, sizeof(UINT8)));
        }
    }
    
    bUpgradeBreak = (u1UpgradeState & DTVCFG_FLAG_USB_UPGRADE_ON) ? 
                    TRUE : FALSE;
    if (bUpgradeBreak)
    {
        // Do not check the upgrade break error now.
        u1UpgradeState &= (~DTVCFG_FLAG_USB_UPGRADE_ON);
        UNUSED(EEPROM_Write(u4UpgradeGoAddr, (UINT32)(void *)&u1UpgradeState,
                            sizeof(UINT8)));
    }
    
    // Query boot/standby decision by eeprom and wakeup reason.
    if (LDR_IsGoBootup(prLdrData) == 0)
    {
        // The decision is standby from above data structure.
        GPIO_CustomLedCtrlFunc(eLedLoaderPowerOff);
        if (bUpgradeBreak)
        {
            // Restore the status record for the next check.
            u1UpgradeState |= DTVCFG_FLAG_USB_UPGRADE_ON;
            UNUSED(EEPROM_Write(u4UpgradeGoAddr, (UINT32)(void *)&u1UpgradeState,
                                sizeof(UINT8)));
        }
        Custom_InitGPIOTimer();
        return;
    }
    
    if (bUpgradeBreak)
    {
        // Restore the status record for the next check.
        u1UpgradeState |= DTVCFG_FLAG_USB_UPGRADE_ON;
        UNUSED(EEPROM_Write(u4UpgradeGoAddr, (UINT32)(void *)&u1UpgradeState,
                            sizeof(UINT8)));
    }
#endif /* CC_MTK_LOADER */
    
    if (!PDWNC_Check_Enter_SemiStandy())
    {
        GPIO_CustomLedCtrlFunc(eLedLoaderPowerOn);
    }
    
    Custom_InitGPIOTimer();
}

static void Custom_InitGPIOTimer(void)
{
#ifdef CC_MTK_LOADER
    UINT32 u4BusClock;
    UINT32 u4TimerClock;
    
    _pfnCustomNextIsr = RegisterIrqHandler(LOADER_Custom_TimerIsr);

    u4BusClock = BSP_GetDomainClock(SRC_TIMER_CLK);
    u4TimerClock = (UINT32)(u4BusClock / 1000);
    u4TimerClock *= CUSTOM_USB_UPGRADE_BLINK_PERIOD;
    
    // Setup Timer 0 and triger 1st interrupt.
    BIM_WRITE32(REG_RW_TIMER1_LLMT, u4TimerClock);
    BIM_WRITE32(REG_RW_TIMER1_HLMT, 0);
    BIM_WRITE32(REG_RW_TIMER1_LOW , u4TimerClock);
    BIM_WRITE32(REG_RW_TIMER1_HIGH, 0);

    LOADER_Custom_TimerOff();
    
    BIM_EnableIrq(VECTOR_T1);

    HalEnableIRQ();

#endif /* CC_MTK_LOADER */
}

#endif /* CC_MTK_PRELOADER */

