
//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_timer.h"
#include "x_pinmux.h"
#include "x_debug.h"
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#include "x_pdwnc.h"

#include "eeprom_if.h"
#include "ir_if.h"
#include "Waffle_gpio.h"
#include "Waffle_pdwnc.h"
#include "CustomCtrl.h"

//---------------------------------------------------------------------------
// Customer function definitions
//---------------------------------------------------------------------------
#define PCBA_QUERY_FUCTION                  (CustomPCBAQueryFunc)
#define PCBA_RGB_SENSOR_QUERY_FUCTION       (CustomPCBARGBQueryFunc)
#define LOGO_NOTIFY_FUNC                    (CustomLogoNotifyFunc)

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
static void CustomIRNfyFunc(UINT32 u4BtnState, UINT32 u4Key,
                            IRRX_RAW_DATA_T *prRawData);
#endif /* CC_MTK_LOADER */

extern void _MW_PCDCheckInit(void);


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
BOOL _fgLedOn[D_LED_TOTAL + 1] = {FALSE, FALSE, FALSE, FALSE, FALSE};
BOOL _fgLedBlink[D_LED_TOTAL + 1] = {FALSE, FALSE, FALSE, FALSE, FALSE};
BOOL _fgLedAllBlink = FALSE;
BOOL _fgStandbyDiable = FALSE;
BOOL _fgIsShowLogo = FALSE;
BOOL _fgTimerOnEnable = FALSE;

#ifndef CC_MTK_LOADER
HANDLE_T _hIRLedCtrlTimer = NULL;
HANDLE_T _hLedCtrlMsgQue = NULL;
HANDLE_T _hLedCtrlHandle = NULL;
#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

static BOOL CustomIsDisabeStandbyLED(void)
{
    static BOOL bReadState = FALSE;
    UINT8 u1ShutdownState = 0;

#ifdef CUSTOM_POWER_STATE_CTRL_ENABLE
    if (CustomQueryPowerState() == STATE_POWER_OFF)
    {
        return TRUE;
    }
#endif /* CUSTOM_POWER_STATE_CTRL_ENABLE */
    
    if (!bReadState)
    {
        // Update LED control state
        UNUSED(EEPROM_Read(CUSTOM_SHUTDOWN_OCCUR_ADDR,
                           (UINT32)(VOID *)&u1ShutdownState, sizeof(UINT8)));
    
        // Disable standby LED control
        _fgStandbyDiable = 
            (u1ShutdownState & CUSTOM_SHUTDOWN_DISABLE_STANDBY_LED) ? 
            TRUE : FALSE;
        bReadState = TRUE;
    }
    return _fgStandbyDiable;
}

void CustomLEDCtrlHandle(D_LED_TYPE eLedType, UINT32 u4LedState)
{
    INT32 i4Gpio;
    INT32 i4Val;
    
#ifndef CC_MTK_LOADER    
    CRIT_STATE_T rCritState;
#endif /* CC_MTK_LOADER */

    switch (eLedType)
    {
    case D_LED_TOTAL:
        if ((D_LED_GENERIC_STATE)u4LedState == D_LED_GENERIC_ON)
        {
            i4Val = CUSTOM_LED_TIMER_ON_VAL;

#ifndef CC_MTK_LOADER    
            rCritState = x_crit_start();
#endif /* CC_MTK_LOADER */

            _fgLedOn[D_LED_POWER] = TRUE;
            _fgLedOn[D_LED_STANDBY] = TRUE;
            _fgLedOn[D_LED_TIMER] = TRUE;
            _fgLedOn[D_LED_TIMER_2] = TRUE;
            _fgLedOn[D_LED_TOTAL] = TRUE;
            
#ifndef CC_MTK_LOADER    
            x_crit_end(rCritState);
#endif /* CC_MTK_LOADER */
        }
        else
        {
            i4Val = 1 - CUSTOM_LED_TIMER_ON_VAL;

#ifndef CC_MTK_LOADER    
            rCritState = x_crit_start();
#endif /* CC_MTK_LOADER */

            _fgLedOn[D_LED_POWER] = FALSE;
            _fgLedOn[D_LED_STANDBY] = FALSE;
            _fgLedOn[D_LED_TIMER] = FALSE;
            _fgLedOn[D_LED_TIMER_2] = FALSE;
            _fgLedOn[D_LED_TOTAL] = FALSE;
            
#ifndef CC_MTK_LOADER    
            x_crit_end(rCritState);
#endif /* CC_MTK_LOADER */
        }

        VERIFY(GPIO_Output(CUSTOM_GPIO_LED_POWER, &i4Val)== i4Val);
        VERIFY(GPIO_Output(CUSTOM_GPIO_LED_STANDBY, &i4Val)== i4Val);
        VERIFY(GPIO_Output(CUSTOM_GPIO_LED_TIMER_GREEN, &i4Val)== i4Val);
        VERIFY(GPIO_Output(CUSTOM_GPIO_LED_TIMER_AMBER, &i4Val)== i4Val);
        break;

    case D_LED_POWER:
        i4Gpio = CUSTOM_GPIO_LED_POWER;
        if ((D_LED_POWER_STATE)u4LedState == D_LED_POWER_ON)
        {
            _fgLedOn[eLedType] = TRUE;
            i4Val = CUSTOM_LED_POWER_ON_VAL;
        }
        else
        {
            _fgLedOn[eLedType] = FALSE;
            i4Val = (1 - CUSTOM_LED_POWER_ON_VAL);
        }
        VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
        break;

    case D_LED_STANDBY:
        i4Gpio = CUSTOM_GPIO_LED_STANDBY;
        if ((D_LED_STANDBY_STATE)u4LedState == D_LED_STANDBY_ON)
        {
#ifndef CC_MTK_LOADER    
            rCritState = x_crit_start();
#endif /* CC_MTK_LOADER */

            _fgLedOn[eLedType] = TRUE;

#ifndef CC_MTK_LOADER    
            x_crit_end(rCritState);
#endif /* CC_MTK_LOADER */

            i4Val = CUSTOM_LED_STANDBY_ON_VAL;
        }
        else
        {
#ifndef CC_MTK_LOADER    
            rCritState = x_crit_start();
#endif /* CC_MTK_LOADER */

            _fgLedOn[eLedType] = FALSE;

#ifndef CC_MTK_LOADER    
            x_crit_end(rCritState);
#endif /* CC_MTK_LOADER */

            i4Val = (1 - CUSTOM_LED_STANDBY_ON_VAL);
        }
        VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
        break;

    case D_LED_TIMER:
        switch (u4LedState)
        {
        case D_LED_TIMER_ORANGE:
            i4Gpio = CUSTOM_GPIO_LED_TIMER_GREEN;
            i4Val = 1 - CUSTOM_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER_2] = FALSE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            i4Gpio = CUSTOM_GPIO_LED_TIMER_AMBER;
            i4Val = CUSTOM_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER] = TRUE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            break;

        case D_LED_TIMER_GREEN :
            i4Gpio = CUSTOM_GPIO_LED_TIMER_GREEN;
            i4Val = CUSTOM_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER_2] = TRUE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            i4Gpio = CUSTOM_GPIO_LED_TIMER_AMBER;
            i4Val = 1 - CUSTOM_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER] = FALSE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            break;

        case D_LED_TIMER_BOTH :
            i4Gpio = CUSTOM_GPIO_LED_TIMER_GREEN;
            i4Val = CUSTOM_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER_2] = TRUE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            i4Gpio = CUSTOM_GPIO_LED_TIMER_AMBER;
            i4Val = CUSTOM_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER] = TRUE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            break;

        default :
            i4Gpio = CUSTOM_GPIO_LED_TIMER_GREEN;
            i4Val = 1 - CUSTOM_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER_2] = FALSE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            i4Gpio = CUSTOM_GPIO_LED_TIMER_AMBER;
            i4Val = 1 - CUSTOM_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER] = FALSE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            break;
        }
        break;
    }
}

#ifndef CC_MTK_LOADER    
static void CustomLedCtrlMsgHandle(void *pvArg)
{
    UINT16 u2MsgIdx;
    LED_CTRL_STATE rLedCtrlMsg;
    
    while (1)
    {
        SIZE_T zMsgSize = sizeof(LED_CTRL_STATE);
        INT32  i4MsgRet;
     
        i4MsgRet = x_msg_q_receive(&u2MsgIdx, &rLedCtrlMsg, &zMsgSize,
                                   &_hLedCtrlMsgQue, 1, X_MSGQ_OPTION_WAIT);
        if (i4MsgRet == OSR_OK)
        {
            CustomLEDCtrlHandle(rLedCtrlMsg.eLedType, rLedCtrlMsg.u4LedState);
        }
    }
}
#endif /* CC_MTK_LOADER */

void CustomLEDCtrlFunc(D_LED_TYPE eLedType, UINT32 u4LedState)
{
#ifdef CC_MTK_LOADER    
    CustomLEDCtrlHandle(eLedType, u4LedState);
#else

    LED_CTRL_STATE rLedCtrlMsg;

    if (_hLedCtrlMsgQue == NULL)
    {
        VERIFY(x_msg_q_create(&_hLedCtrlMsgQue, "LedCtrlMsgQue", 
                              sizeof(LED_CTRL_STATE), 8) == OSR_OK);
        
        // Create the Led state handle handle task
        if (x_thread_create(&_hLedCtrlHandle, "LedCtrlMsgHandle",
                            0x800, DRVCUST_OptGet(eReleaseWatchdogPriority),
                            CustomLedCtrlMsgHandle, 0, NULL) != OSR_OK)
        {
            LOG(3, "\nError: Create Led state handle handle thread fail\n");
            x_msg_q_delete(_hLedCtrlMsgQue);
            _hLedCtrlMsgQue = NULL;
        }
    }

    if (_hLedCtrlMsgQue != NULL)
    {
        rLedCtrlMsg.eLedType = eLedType;
        rLedCtrlMsg.u4LedState = u4LedState;
        UNUSED(x_msg_q_send(_hLedCtrlMsgQue, &rLedCtrlMsg,
                            sizeof(LED_CTRL_STATE), 1));
    }

#endif /* CC_MTK_LOADER */
}

void CustomDisableStandbyLEDCtrl(UINT8 u1Disable)
{
    UINT8 u1ShutdownState = 0;

    // Disable standby LED control
    _fgStandbyDiable = (BOOL)(u1Disable);
    
    // Update LED control state
    UNUSED(EEPROM_Read(CUSTOM_SHUTDOWN_OCCUR_ADDR,
                       (UINT32)(VOID *)&u1ShutdownState, sizeof(UINT8)));
    u1ShutdownState = (_fgStandbyDiable) ?
                      (u1ShutdownState | CUSTOM_SHUTDOWN_DISABLE_STANDBY_LED) :
                      (u1ShutdownState & (~CUSTOM_SHUTDOWN_DISABLE_STANDBY_LED));
    UNUSED(EEPROM_Write(CUSTOM_SHUTDOWN_OCCUR_ADDR,
                        (UINT32)(VOID *)&u1ShutdownState, sizeof(UINT8)));
}

void CustomLEDBlinkCtrl(D_LED_TYPE eLedType)
{
    UINT32 u4LedState;

    // Control the LED blinking,
    // LED On : 1000 ms & LED off : 1000ms
    if (eLedType == D_LED_TIMER)
    {
        u4LedState = (_fgLedOn[D_LED_TIMER_2]) ? 
                     (UINT32)D_LED_TIMER_OFF : (UINT32)D_LED_TIMER_GREEN;
    }
    else
    {
        u4LedState = (_fgLedOn[eLedType]) ? 
                     (UINT32)D_LED_GENERIC_OFF : (UINT32)D_LED_GENERIC_ON;
    }
    
    CustomLEDCtrlFunc(eLedType, u4LedState);
}

#ifndef CC_MTK_LOADER    
void CustomLogoNotifyFunc(BOOL bIsShowLogo)
{
    UNUSED(CustomLogoNotifyFunc);
    _fgIsShowLogo = bIsShowLogo;
    
#ifdef CC_TIME_MEASUREMENT
    if (bIsShowLogo)
    {
        x_os_drv_set_timestamp("Before logo shows");
    }
    else
    {
        x_os_drv_set_timestamp("After logo shows");
    }
#endif /* CC_TIME_MEASUREMENT */
}

void CustomIRNfyFunc(UINT32 u4BtnState, UINT32 u4Key, IRRX_RAW_DATA_T *prRawData)
{
    BOOL fEnterPowerOff = FALSE;

    UNUSED(u4Key);
    UNUSED(prRawData);

#ifdef CUSTOM_POWER_STATE_CTRL_ENABLE
    fEnterPowerOff = (CustomQueryPowerState() == STATE_POWER_ON) ? FALSE : TRUE;
#else
    CRIT_STATE_T rCritState = x_crit_start();
    fEnterPowerOff = _fgLedOn[D_LED_STANDBY];
    x_crit_end(rCritState);
#endif /* CUSTOM_POWER_STATE_CTRL_ENABLE */

    IRRC_LedBlinkConfig(!fEnterPowerOff);
}

void CustomIRPowerOffHandler(UINT32 u4BtnState, UINT32 u4Key,
                             IRRX_RAW_DATA_T *prRawData)
{
    static BOOL bIsUserHoldPowerBtnToPowerOn = TRUE;
    static BOOL bIsSkipPowerOffHandle = FALSE;
    
    UNUSED(prRawData);

    if (!_fgIsShowLogo)
    {
        return;
    }

    if ((u4Key & (~KEY_SRC_FRONT_PANEL)) == BTN_POWER)
    {
        if (u4BtnState == IRRX_COND_BTN_UP)
        {
            bIsSkipPowerOffHandle = bIsUserHoldPowerBtnToPowerOn;
            if (!bIsSkipPowerOffHandle)
            {
                UINT32 u4Val = u4IO32Read4B(PDWNC_WAKEN);
                DTVCFG_T rDtvCfg;
            
                UNUSED(EEPDTV_GetCfg(&rDtvCfg));
                rDtvCfg.u1Flags &= ~((UINT8)DTVCFG_FLAG_SYSTEM_STATE_ON);
                UNUSED(EEPDTV_SetCfg(&rDtvCfg));
            
                bIsUserHoldPowerBtnToPowerOn = TRUE;
                _MW_PCDCheckInit();
                CustomPowerOffSequence();
                PDWNC_EnterPowerDown((u4Val | WAK_IRRX), PDWNC_POWER_DOWN_IRRC);
            }
        }
        else
        {
            // u4BtnState == IRRX_COND_BTN_DOWN
            bIsUserHoldPowerBtnToPowerOn = FALSE;
        }
    }
}

void CustomIRLedHandleStart(void)
{
    static BOOL bInit = FALSE;
    
    if (bInit)
    {
        return;
    }

    if (x_timer_create(&_hIRLedCtrlTimer) != OSR_OK)
    {
        LOG(5, "[GPIO_CustomPowerOnFunc] Create IR LED Blink timer fail !\n");
        return;
    }

    UNUSED(CustomIRNfyFunc);
//    UNUSED(IRRC_RegCallbackFunc(CustomIRNfyFunc));
    UNUSED(IRRC_RegCallbackFunc(CustomIRPowerOffHandler));
}

void CustomIRLedHandleStop(void)
{
    if (_hIRLedCtrlTimer != NULL)
    {
        x_timer_stop(_hIRLedCtrlTimer);
        x_timer_delete(_hIRLedCtrlTimer);
        _hIRLedCtrlTimer = NULL;
    }
}

void CustomIRLedTimerStop(void)
{
    if (_hIRLedCtrlTimer != NULL)
    {
        x_timer_stop(_hIRLedCtrlTimer);
    }
}
#endif /* CC_MTK_LOADER */

void GPIO_CustomLedCtrlFunc(LED_STATUS_T eLedStatus)
{
    UINT32 u4LedCtrl = D_LED_GENERIC_OFF;
    BOOL   bOnTimer = FALSE;
    
    BOOL fgLedStandbyDiable = CustomIsDisabeStandbyLED();
    switch(eLedStatus)
    {
    case eLedLoaderPowerOn:
        Printf("eLedLoaderPowerOn\n");
        // VGA Standby LED OFF
        CustomLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        //Power LED ON
        CustomLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);
        
        // TIMER LED ON/OFF
        bOnTimer = IO32ReadFldAlign(PDWNC_WAKEN, FLD_RTC_WAKEN) ? TRUE : FALSE;
        CustomSetTimerLED(bOnTimer);
        u4LedCtrl = _fgTimerOnEnable ? D_LED_TIMER_ORANGE : D_LED_TIMER_OFF;
        CustomLEDCtrlFunc(D_LED_TIMER, u4LedCtrl);
        break;

    case eLedLoaderPowerOff:
        Printf("eLedLoaderPowerOff\n");
        // Power LED OFF
        CustomLEDCtrlFunc(D_LED_POWER, D_LED_POWER_OFF);
        
        // TIMER LED ON/OFF
        u4LedCtrl = _fgTimerOnEnable ? D_LED_TIMER_ORANGE : D_LED_TIMER_OFF;
        CustomLEDCtrlFunc(D_LED_TIMER, u4LedCtrl);
        
        // Standby LED ON
        u4LedCtrl = fgLedStandbyDiable ? D_LED_STANDBY_OFF : D_LED_STANDBY_ON;
        CustomLEDCtrlFunc(D_LED_STANDBY, u4LedCtrl);
        break;

    case eLedSystemPowerOn:
        Printf("eLedSystemPowerOn\n");
        // VGA Standby LED OFF
        CustomLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        // Power LED ON
        CustomLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);

        // TIMER LED ON/OFF
        bOnTimer = IO32ReadFldAlign(PDWNC_WAKEN, FLD_RTC_WAKEN) ? TRUE : FALSE;
        CustomSetTimerLED(bOnTimer);
        u4LedCtrl = _fgTimerOnEnable ? D_LED_TIMER_ORANGE : D_LED_TIMER_OFF;
        CustomLEDCtrlFunc(D_LED_TIMER, u4LedCtrl);
        break;

    case eLedSystemPowerOff:
        Printf("eLedSystemPowerOff\n");
        // Power LED OFF
        CustomLEDCtrlFunc(D_LED_POWER, D_LED_POWER_OFF);
        
        // TIMER LED ON/OFF
        u4LedCtrl = _fgTimerOnEnable ? D_LED_TIMER_ORANGE : D_LED_TIMER_OFF;
        CustomLEDCtrlFunc(D_LED_TIMER, u4LedCtrl);
        
        // Standby LED ON
        u4LedCtrl = fgLedStandbyDiable ? D_LED_STANDBY_OFF : D_LED_STANDBY_ON;
        CustomLEDCtrlFunc(D_LED_STANDBY, u4LedCtrl);
        break;

    case eLedDarkLed:
    case eLedIrOn:
    case eLedIrOff:
        break;

    case eLedVgaStandby:
        Printf("eLedVgaStandby\r\n");
        //VGA Standby LED ON
        // Power LED OFF
        CustomLEDCtrlFunc(D_LED_POWER, D_LED_POWER_OFF);
        
        // TIMER LED ON/OFF
        u4LedCtrl = _fgTimerOnEnable ? D_LED_TIMER_ORANGE : D_LED_TIMER_OFF;
        CustomLEDCtrlFunc(D_LED_TIMER, u4LedCtrl);
        
        // Standby LED ON
        u4LedCtrl = fgLedStandbyDiable ? D_LED_STANDBY_OFF : D_LED_STANDBY_ON;
        CustomLEDCtrlFunc(D_LED_STANDBY, u4LedCtrl);
        break;

#ifdef CC_MTK_LOADER
    case eLedUpgradeBlink:
        Printf("eLedUpgradeBlink\r\n");
        LOADER_Custom_TimerOff();

        // Power LED OFF
        CustomLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);
        // Standby LED ON
        CustomLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        // Orange LED ON
        CustomLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_GREEN);

        //Orange LED blinking On
        _fgLedBlink[D_LED_POWER] = TRUE;
        _fgLedBlink[D_LED_STANDBY] = FALSE;
        _fgLedBlink[D_LED_TIMER] = TRUE;
        _fgLedAllBlink = FALSE;

        LOADER_Custom_TimerOn();
        Printf("Begin USB Upgrade\r\n");
        break;

    case eLedUpgradeFinish:
        Printf("eLedUpgradeFinish\r\n");
        LOADER_Custom_TimerOff();

        _fgLedBlink[D_LED_POWER] = FALSE;
        _fgLedBlink[D_LED_STANDBY] = FALSE;
        _fgLedBlink[D_LED_TIMER] = FALSE;
        _fgLedAllBlink = FALSE;

        // Standby LED OFF
        CustomLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        // Orange LED OFF
        CustomLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_OFF);
        // Power LED ON
        CustomLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);

        Printf("Finish USB Upgrade\r\n");
        break;

    case eLedUpgradeFail:
        Printf("eLedUpgradeFail\r\n");
        LOADER_Custom_TimerOff();
        _fgLedBlink[D_LED_POWER] = FALSE;
        _fgLedBlink[D_LED_STANDBY] = FALSE;
        _fgLedBlink[D_LED_TIMER] = FALSE;
        _fgLedAllBlink = FALSE;

        // Standby LED OFF
        CustomLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        // Orange LED OFF
        CustomLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_OFF);
        // Power LED ON
        CustomLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);
        _LdrUpgradeBreakFunc();
        break;

#endif /* CC_MTK_LOADER */

    default:
        break;
    }
}

void CustomSetTimerLED(BOOL bIsEnable)
{
    if (CustomPCBAQueryCountry() == DRV_CUSTOM_COUNTRY_PAA)
    {
        _fgTimerOnEnable = bIsEnable;
    }
}

void _LdrUpgradeBreakFunc(void)
{
    while (1)
    {
        CustomLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_ORANGE);
        CustomLEDCtrlFunc(D_LED_STANDBY, D_LED_POWER_ON);
        x_thread_delay(CUSTOM_USB_UPGRADE_BLINK_PERIOD);
        CustomLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_OFF);
        CustomLEDCtrlFunc(D_LED_STANDBY, D_LED_POWER_OFF);
        x_thread_delay(CUSTOM_USB_UPGRADE_BLINK_PERIOD);
    }
}

