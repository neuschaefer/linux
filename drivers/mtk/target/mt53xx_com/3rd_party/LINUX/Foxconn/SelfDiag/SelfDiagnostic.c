
#include "x_hal_arm.h"
#include "x_hal_io.h"
#include "x_gpio.h"
#include "x_timer.h"
#include "eeprom_if.h"
#include "pdwnc_if.h"
#include "mute_if.h"
#include "rtc_if.h"
#include "sv_const.h"
#include "drvcust_if.h"
#include "drv_display.h"
#include "SelfDiag.h"
#include "SelfDiag_debug.h"
#include "CustomCtrl.h"

//-----------------------------------------------------------------------------
// Constant definitions & Macro
//-----------------------------------------------------------------------------
#define CHK_SELF_DIAG(Condition, TimeCount, TimeThres, ErrorCode)       \
    if (!_bfgSelfDiagGo)                                                \
    {                                                                   \
        break;                                                          \
    }                                                                   \
    if (Condition)                                                      \
    {                                                                   \
        TimeCount += SELF_DIAG_DETECT_PERIOD;                           \
        if (TimeCount >= TimeThres)                                     \
        {                                                               \
            LOG(5, "Enter Safety Shutdown for Error (%d)\n", ErrorCode);\
            LOG(5, "TimeCount = %d ms, TimeThres = %d ms\n", TimeCount, TimeThres);\
            _bfgSelfDiagGo = FALSE;                                     \
            SelfDiagSetShutdown(ErrorCode);                             \
            break;                                                      \
        }                                                               \
    }                                                                   \
    else                                                                \
    {                                                                   \
        TimeCount = 0;                                                  \
    }

#define GET_SELF_DIAG_THRES(Addr, Thres, Max, Value, DataLen, Default)  \
    UNUSED(EEPROM_Read(Addr, (UINT32)(VOID *)&Value, DataLen));         \
    if (DataLen == sizeof(UINT16))                                      \
    {                                                                   \
        Value = (((Value & 0xFF00) >> 8) | ((Value & 0xFF) << 8));      \
    }                                                                   \
    Thres = (Value >= Max) ? Default : Value;                           \
    if (Thres != Value)                                                 \
    {                                                                   \
        Value = Thres;                                                  \
        if (DataLen == sizeof(UINT16))                                  \
        {                                                               \
            Value = (((Thres & 0xFF00) >> 8) | ((Thres & 0xFF) << 8));  \
        }                                                               \
        UNUSED(EEPROM_Write(Addr, (UINT32)(VOID *)&Value, DataLen));    \
    }

#define CHK_SELF_DIAG_PAUSE()                                           \
    if (_bfgSelfDiagPause)                                              \
    {                                                                   \
        x_thread_suspend();                                             \
    }


//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported funtions 
//-----------------------------------------------------------------------------
extern void CHIP_Reboot(void);
extern UINT32 FlashPanel_GetInchSize(void);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static INT32  _i4ThresTemp = 0;
static UINT8  _u1SelfDiagOnOffCnt = 0;
static UINT32 _u4SelfDiagModelType = 0;
static UINT32 _u4Detect12VAmpTimeCnt = 0;
static UINT32 _u4Detect12VSWTimeCnt = 0;
static UINT32 _u4Detect24VPwrTimeCnt = 0;
static UINT32 _u4DetectAudTimeCnt = 0;
static UINT32 _u4Detect3V3_5VTimeCnt = 0;
static UINT32 _u4DetectPanelPWRTimeCnt = 0;
static UINT32 _u4DetectBacklightTimeCnt = 0;
static UINT32 _u4BacklightIsNormalTime = 0;
static UINT32 _u4DetectTemperatureTimeCnt = 0;
static HANDLE_T _hSelfDiagHandle = NULL;
static HANDLE_T _hSelfDiagSema = NULL;
static HANDLE_T _hSelfDiagErrorHandle = NULL;
static HANDLE_T _hSelfDiagTimer = NULL;
static BOOL _bfgSelfDiagCheckBacklight = FALSE;
static BOOL _bfgSelfDiagMuteOff = FALSE;
static BOOL _bfgSelfDiagPause = FALSE;
static BOOL _bfgSelfDiagBackltOn = FALSE;

#ifdef CC_SELF_DIAG_FORCE_DISABLE
static BOOL _bfgSelfDiagForceDisable = TRUE;
#else
static BOOL _bfgSelfDiagForceDisable = FALSE;
#endif /* CC_SELF_DIAG_FORCE_DISABLE */

BOOL   _bfgSelfDiagGo = FALSE;
HANDLE_T _hSelfDiagMsgQue = NULL;
SELF_DIAG_TABLE _eSelfDiagEnable;
SELF_DIAG_TABLE _eSelfDiagShutdownLog;

extern BOOL _bgIsCMO37Panel;
extern BOOL _bgIsThermalDetect;
extern UINT32 _u4gModelType;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
static void SelfDiagTimerHandle(HANDLE_T hHandle, VOID* pTag)
{
    UNUSED(hHandle);
    UNUSED(pTag);
    
    x_thread_resume(_hSelfDiagHandle);
}

static void SelfDiagErrorHandle(void *pvArg)
{
    UINT16 u2MsgIdx;
    UINT32 u4Msg;
    PFN_USR_POWER_SEQUENCE pfnUsrPowerOffSequence = NULL;
    PFN_USR_POWER_SEQUENCE pfnUsrPowerOnSequence = NULL;

    // Query panel power on/off sequence function
    UNUSED(DRVCUST_InitQuery(eCustomerPowerOffSequence, 
                             (UINT32 *)(void *)&(pfnUsrPowerOffSequence)));
    UNUSED(DRVCUST_InitQuery(eCustomerPowerOnSequence, 
                             (UINT32 *)(void *)&(pfnUsrPowerOnSequence)));

    UNUSED(pvArg);
    while (1)
    {
        SIZE_T zMsgSize = sizeof(UINT32);
        INT32  i4MsgRet;
     
        i4MsgRet = x_msg_q_receive(&u2MsgIdx, &u4Msg, &zMsgSize,
                               &_hSelfDiagMsgQue, 1, X_MSGQ_OPTION_WAIT);
        if (i4MsgRet == OSR_OK)
        {
            switch (u4Msg)
            {
            case SELF_DIAG_ACT_REBOOT:
                {
                    if (pfnUsrPowerOffSequence)
                    {
                        LOG(5, "[SelfDiagErrorHandle] ");
                        LOG(5, "Enter panel power off sequence\n");
                        pfnUsrPowerOffSequence();
                    }

                    LOG(5, "D_BACKLIGHT_ERROR : try to reboot (%d time)\n",
                        _u1SelfDiagOnOffCnt);
                    PRINT_TIMESTAMP_DBG_MSG("[Self Diag]",
                                            "D_BACKLIGHT_ERROR : try to reboot (%d time)\n",
                                            _u1SelfDiagOnOffCnt);
                    CHIP_Reboot();
                }
                break;
            case SELF_DIAG_ACT_PANEL_ONOFF:
                {
                    if (pfnUsrPowerOffSequence)
                    {
                        LOG(5, "[SelfDiagErrorHandle] ");
                        LOG(5, "Enter panel power off sequence\n");
                        pfnUsrPowerOffSequence();
                    }

                    // Wait for SMicro reboot
                    x_thread_delay(3000);
                
                    if (pfnUsrPowerOnSequence)
                    {
                        LOG(5, "[SelfDiagErrorHandle] ");
                        LOG(5, "Enter panel power on sequence\n");
                        pfnUsrPowerOnSequence();
                    }
                }
                break;
            }
        }
    }
}

static void SelfDiagHandle(void *pvArg)
{
    BOOL   bIsWoofer = FALSE;
    INT32  i4Temperature = 0;
    UINT32 u4Value = 0;
    UINT32 u4LampError = 1;
    UINT32 u4BackLightDelayTime = 0;
    UINT32 u4MuteState;
    
    UNUSED(pvArg);
    
#ifdef WAFFLE_PCB_EVT_CHM
    u4LampError = (_bgIsCMO37Panel) ? 0 : 1;

    bIsWoofer = (_u4gModelType & DRV_CUSTOM_MODEL_AUD_WOOFER) ? TRUE : FALSE;
#endif /* WAFFLE_PCB_EVT_CHM */

    PRINT_TIMESTAMP_MSG("[Power On]", "Self-Diag goes\n");
    while (_bfgSelfDiagGo)
    {
        // Detect D_12V_AMP_ERROR 
        CHK_SELF_DIAG_PAUSE();
        if (_eSelfDiagEnable.u2Amp12VError == SELF_DIAG_ENABLE)
        {
            u4Value = GPIO_Input(SELF_DIAG_12V_AMP_DROP_DET);
            LOG(9, "SELF_DIAG_12V_AMP_DROP_DET = %d\n", u4Value);
            CHK_SELF_DIAG((u4Value == 0), _u4Detect12VAmpTimeCnt,
                          SELF_DIAG_POWER_DET_TIME, D_12V_AMP_ERROR);
        }
    
        // Detect D_12V_SW_ERROR 
        CHK_SELF_DIAG_PAUSE();
        if (_eSelfDiagEnable.u2SW12VError == SELF_DIAG_ENABLE)
        {
            u4Value = GPIO_Input(SELF_DIAG_12V_SW_DROP_DET);
            LOG(9, "SELF_DIAG_12V_SW_DROP_DET = %d\n", u4Value);
            CHK_SELF_DIAG((u4Value == 0), _u4Detect12VSWTimeCnt,
                          SELF_DIAG_POWER_DET_TIME, D_12V_SW_ERROR);
        }
    
        // Detect D_24V_ERROR 
        CHK_SELF_DIAG_PAUSE();
        if ((_eSelfDiagEnable.u224VError == SELF_DIAG_ENABLE) && (bIsWoofer))
        {
            u4Value = GPIO_Input(SELF_DIAG_24V_AMP_VDD_DROP_DET);
            LOG(9, "SELF_DIAG_24V_POWER_DET = %d\n", u4Value);
            CHK_SELF_DIAG((u4Value == 0), _u4Detect24VPwrTimeCnt,
                          SELF_DIAG_POWER_DET_TIME, D_24V_PWR_ERROR);
        }
    
        // Detect D_AUD_ERROR 
        CHK_SELF_DIAG_PAUSE();
        if (_eSelfDiagEnable.u2AudError == SELF_DIAG_ENABLE)
        {
            u4Value = GPIO_Input(SELF_DIAG_AUD_AMP_ERROR_DET);
            LOG(9, "SELF_DIAG_AUD_AMP_ERROR_DET = %d\n", u4Value);
            CHK_SELF_DIAG((u4Value == 0), _u4DetectAudTimeCnt,
                          SELF_DIAG_MAIN_DET_TIME, D_AUD_ERROR);
        }
    
        // Detect D_3V3_5V_ERROR 
        CHK_SELF_DIAG_PAUSE();
        if (_eSelfDiagEnable.u2VDrop3V_5VError == SELF_DIAG_ENABLE)
        {
            u4Value = GPIO_Input(SELF_DIAG_V_DROP_DET);
            LOG(9, "SELF_DIAG_V_DROP_DET = %d\n", u4Value);
            CHK_SELF_DIAG((u4Value == 0), _u4Detect3V3_5VTimeCnt,
                          SELF_DIAG_MAIN_DET_TIME, D_3V3_5V_ERROR);
        }
    
        // Detect D_PANEL_POWER_ERROR 
        CHK_SELF_DIAG_PAUSE();
        if (_eSelfDiagEnable.u2PanelPowerError == SELF_DIAG_ENABLE)
        {
            u4Value = GPIO_Input(SELF_DIAG_PANEL_V_DROP_DET);
            LOG(9, "SELF_DIAG_PANEL_V_DROP_DET = %d\n", u4Value);
            CHK_SELF_DIAG((u4Value == 0), _u4DetectPanelPWRTimeCnt,
                          SELF_DIAG_LAMP_DET_TIME, D_PANEL_POWER_ERROR);
        }

        // Detect D_TEMPERATURE_ERROR
        CHK_SELF_DIAG_PAUSE();
        
        // If there is no thermal device on PCB, need not to get temperature info.
        if (_bgIsThermalDetect)
        {
            i4Temperature = CustomReadTemperature();
            
            if (_eSelfDiagEnable.u2TemperatureError == SELF_DIAG_ENABLE)
            {
                // Read Temp Sensor
                LOG(9, "Temperature = %d\n", i4Temperature);
                CHK_SELF_DIAG((i4Temperature > _i4ThresTemp),
                              _u4DetectTemperatureTimeCnt,
                              SELF_DIAG_MAIN_DET_TIME, D_TEMPERATURE_ERROR);
            }
        }

        // Detect D_BACKLIGHT_ERROR 
        CHK_SELF_DIAG_PAUSE();
        u4Value = GPIO_Input(SELF_DIAG_LAMP_DET);
        if (u4Value == u4LampError)
        {
            if ((_eSelfDiagEnable.u2BacklightError == SELF_DIAG_ENABLE) &&
                _bfgSelfDiagCheckBacklight && _bfgSelfDiagBackltOn)
            {
                PRINT_TIMESTAMP_DBG_MSG("[Self Diag]", "LAMP_DET = %x \n", u4Value);
                _u4BacklightIsNormalTime = 0;
                _u4DetectBacklightTimeCnt += SELF_DIAG_DETECT_PERIOD;
                if (_u4DetectBacklightTimeCnt >= SELF_DIAG_INVERTER_DET_TIME)                                     \
                {
                    // Read OnOff counter from EEPROM
                    UNUSED(EEPROM_Read(SELF_DIAG_ON_OFF_CNT_ADDR,
                                       (UINT32)(VOID *)&_u1SelfDiagOnOffCnt,
                                       sizeof(UINT8)));
                    
                    // If the EEPROM is empty, reset the OnOff counter
                    if (_u1SelfDiagOnOffCnt == 0xFF)
                    {
                        _u1SelfDiagOnOffCnt = 0;
                    }
                    
                    // If the OnOff counter < retry limit (3), reboot TV
                    if (++_u1SelfDiagOnOffCnt < SELF_DIAG_ON_OFF_RETRY_LIMIT)
                    {
                        UNUSED(EEPROM_Write(SELF_DIAG_ON_OFF_CNT_ADDR,
                                            (UINT32)(VOID *)&_u1SelfDiagOnOffCnt,
                                            sizeof(UINT8)));

                        // Start the Error handle task to reboot
                        UINT32 u4Value = SELF_DIAG_ACT_REBOOT;
                        UNUSED(x_msg_q_send(_hSelfDiagMsgQue,
                                            &u4Value, sizeof(UINT32), 1));
                    }
                    else
                    {
                        _u1SelfDiagOnOffCnt = 0;
                        SelfDiagSetShutdown(D_BACKLIGHT_ERROR);
                        break;
                    }
                }
            }
        }
        else
        {
            static BOOL bfgCheckNormal = TRUE;
            
            _u4DetectBacklightTimeCnt = 0;
            if (bfgCheckNormal)
            {
                _u4BacklightIsNormalTime += SELF_DIAG_DETECT_PERIOD;
                if (_u4BacklightIsNormalTime > SELF_DIAG_BL_RESET_TIME)
                {
                    // Reset the reboot count
                    _u1SelfDiagOnOffCnt = 0;
                    bfgCheckNormal = FALSE;
                    UNUSED(EEPROM_Write(SELF_DIAG_ON_OFF_CNT_ADDR,
                                        (UINT32)(VOID *)&_u1SelfDiagOnOffCnt,
                                        sizeof(UINT8)));
                    PRINT_TIMESTAMP_MSG("[Power On]", "Reset the reboot count\n");
                }
            }
        }
        
        UNUSED(x_timer_start(_hSelfDiagTimer, SELF_DIAG_DETECT_PERIOD,
                             X_TIMER_FLAG_ONCE, SelfDiagTimerHandle, NULL));
        x_thread_suspend();

        if (!_bfgSelfDiagCheckBacklight)
        {
            if (!_bfgSelfDiagMuteOff)
            {
                u4MuteState = _u4DrvVideoGetMute(SV_VP_MAIN);
                LOG(5, "[SelfDiagHandle] Check mute state = %d\n",
                       u4MuteState);
                if (u4MuteState == SV_OFF)
                {
                    PRINT_TIMESTAMP_MSG("[Power On]", "Picture mute off\n");
                    _bfgSelfDiagMuteOff = TRUE;
                }
                else
                {
                    continue;
                }
            }
            
            u4BackLightDelayTime += SELF_DIAG_DETECT_PERIOD;
            if (u4BackLightDelayTime > SELF_DIAG_BL_DELAY_TIME)
            {
                PRINT_TIMESTAMP_MSG("[Power On]", "Start backlight & balancer check\n");
                _bfgSelfDiagCheckBacklight = TRUE;
                u4BackLightDelayTime = 0;
            }
        }
    }
    
    VERIFY(x_sema_unlock(_hSelfDiagSema) == OSR_OK);
    x_sema_delete(_hSelfDiagSema);
    _hSelfDiagSema = NULL;

//    x_thread_exit();
}

static void SelfDiagSetDefault(void)
{
    UINT8 au1Data[SELF_DIAG_EEPROM_SIZE];
    UINT8 u1DataInit = 0;
    UINT32 u4Len = sizeof(au1Data);
    UINT32 i;
    SELF_DIAG_TABLE eSelfDiagTbl;

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
                return;
            }
        }
    }

    // Set default value if EEPROM is empty
    // Disable Self-Diag Enable bit
#if defined (CC_SELF_DIAG_DEFAULT_ON)
#ifdef WAFFLE_PCB_EVT_CHM
    *((UINT16 *)((VOID *)&eSelfDiagTbl)) = 0x0000;  /*Enable*/
#else
    *((UINT16 *)((VOID *)&eSelfDiagTbl)) = 0x00A0;  /*Enable*/
#endif /* WAFFLE_PCB_EVT_CHM */
    Printf("Self-Diag default on\n");
    
#else
    *((UINT16 *)((VOID *)&eSelfDiagTbl)) = 0xFFFF;  /*Disable*/
    Printf("Self-Diag default off\n");
#endif
    UNUSED(EEPROM_Write(SELF_DIAG_DET_ENABLE_ADDR,
                        (UINT32)(VOID *)&eSelfDiagTbl, sizeof(UINT16)));
        
    // Clear Self-Diag Shutdown log
    *((UINT16 *)((VOID *)&eSelfDiagTbl)) = 0;
    UNUSED(EEPROM_Write(SELF_DIAG_ERROR_LOG_ADDR,
                        (UINT32)(VOID *)&eSelfDiagTbl, sizeof(UINT16)));

    // Enable Self-Diag Recovery bit
    *((UINT16 *)((VOID *)&eSelfDiagTbl)) = 0;
    UNUSED(EEPROM_Write(SELF_DIAG_BOOT_RETRY_ADDR,
                        (UINT32)(VOID *)&eSelfDiagTbl, sizeof(UINT16)));

    // Clear Shutdown state in EEPROM
    UINT8 u1Val = 0;
    UNUSED(EEPROM_Write(SELF_DIAG_SHUTDOWN_OCCUR_ADDR,
                        (UINT32)(VOID *)&u1Val, sizeof(UINT8)));
    // Clear counters
    SelfDiagResetCounter();
    SelfDiagResetBootCount();
    
    // Clear Self-Diag data init bit in EEPROM
    u1DataInit = SELF_DIAG_CLEAR_DATA_INIT;
    UNUSED(EEPROM_Write(SELF_DIAG_DATA_INIT_ADDR,
                        (UINT32)(VOID *)&u1DataInit, sizeof(UINT8)));
}

static void SelfDiagInit(void)
{
    UINT8 u1Val = 0;
    UINT16 u2Value = 0;
    UINT32 u4Len = sizeof(UINT16);
    PDWNC_FuncTbl *prPdwncFuncTbl;

    // Set Failure LED blink
    prPdwncFuncTbl = PDWNC_EXT_GetFunc();
    if (prPdwncFuncTbl != NULL)
    {
        // Reset T8032 blink control
        prPdwncFuncTbl->pfnSet8032uPLedBlink(0);
    }
    
    SelfDiagSetDefault();
    x_memset((void *)&_eSelfDiagShutdownLog, 0, u4Len);
    
    // Clear Shutdown state in EEPROM
    UNUSED(EEPROM_Read(SELF_DIAG_SHUTDOWN_OCCUR_ADDR,
                       (UINT32)(VOID *)&u1Val, sizeof(UINT8)));
    u1Val &= (~CUSTOM_SHUTDOWN_ERROR);
    UNUSED(EEPROM_Write(SELF_DIAG_SHUTDOWN_OCCUR_ADDR,
                        (UINT32)(VOID *)&u1Val, sizeof(UINT8)));

#if 0
    // Clear the shutdown log
    UNUSED(EEPROM_Write(SELF_DIAG_ERROR_LOG_ADDR,
                        (UINT32)(VOID *)&_eSelfDiagShutdownLog, u4Len));
#else    
    // Get Shutdown log from EEPROM
    u4Len = sizeof(SELF_DIAG_TABLE);
    UNUSED(EEPROM_Read(SELF_DIAG_ERROR_LOG_ADDR,
                      (UINT32)(VOID *)&u2Value, u4Len));
    
    // The EEPROM is empty, try to initial the shotdown log
    if (u2Value == 0xFFFF)
    {
        UNUSED(EEPROM_Write(SELF_DIAG_ERROR_LOG_ADDR,
                            (UINT32)(VOID *)&_eSelfDiagShutdownLog, u4Len));
    }
#endif
    
    // Get Enable bit from EEPROM
    u4Len = sizeof(SELF_DIAG_TABLE);
    UNUSED(EEPROM_Read(SELF_DIAG_DET_ENABLE_ADDR,
                      (UINT32)(VOID *)&_eSelfDiagEnable, u4Len));
    
    // If ALL_ERROR bit == disable, Disable all enable bit
    if (_eSelfDiagEnable.u2AllError == SELF_DIAG_DISABLE)
    {
        u2Value = 0xFFFF;
        x_memset(&_eSelfDiagEnable, u2Value, sizeof(UINT16));
    }
    
    // Get Threshold default value from EEPROM
    u4Len = sizeof(UINT16);
    
    GET_SELF_DIAG_THRES(SELF_DIAG_TEMP_LIMIT_ADDR, _i4ThresTemp,
                        SELF_DIAG_TEMP_VALID_MAX, u2Value, u4Len,
                        SELF_DIAG_TEMP_LIMIT_DEFAULT);
    if (_i4ThresTemp & (1 << 8))
    {
        _i4ThresTemp = 0xFFFFFF00 | (_i4ThresTemp & 0x00FF);
    }
}

//-----------------------------------------------------------------------------
// Global Functions
//-----------------------------------------------------------------------------
void SelfDiagSetModelType(UINT32 u4ModelType)
{
    UNUSED(_u4SelfDiagModelType);
    _u4SelfDiagModelType = u4ModelType;
}

void SelfDiagPause(void)
{
    _bfgSelfDiagPause = TRUE;
    UNUSED(x_timer_stop(_hSelfDiagTimer));
}

void SelfDiagResume(void)
{
    _bfgSelfDiagPause = FALSE;
    x_thread_resume(_hSelfDiagHandle);
}

void SelfDiagStart(D_SYSTEM_STATE eState)
{
    UINT32 u4Priority = DRVCUST_OptGet(eReleaseWatchdogPriority);
    
    if (_bfgSelfDiagForceDisable)
    {
        LOG(3, "[SelfDiagStart] Force disable self-diag....\n");
        return;
    }

    if (_bfgSelfDiagGo == TRUE)
    {
        // Self-diag task already starts now
        LOG(3, "[SelfDiagStart] Self-Diag task already runs....\n");

        if (_bfgSelfDiagPause)
        {
            printf("[SelfDiagStart] Self-Diag task resume....\n");
            SelfDiagResume();
        }
        return;
    }

    SelfDiagInit();
    if (_hSelfDiagMsgQue == NULL)
    {
        VERIFY(x_msg_q_create(&_hSelfDiagMsgQue, "SelfDiagMsgQue", 
                              sizeof(UINT32), 8) == OSR_OK);
        
        // Create the self-diag error handle task
        if (x_thread_create(&_hSelfDiagErrorHandle, "SelfDiagErrHandle",
                            0x800, u4Priority, SelfDiagErrorHandle,
                            0, NULL) != OSR_OK)
        {
            LOG(3, "\nError: Create Self-Diag error handle thread fail\n");
            x_msg_q_delete(_hSelfDiagMsgQue);
            _hSelfDiagMsgQue = NULL;
        }
    }

    _bfgSelfDiagPause = FALSE;
    
    // If self-diag starts in panel power on state,
    // it is necessary to wait 5 sec. to reset the reboot counter.
    if (eState == D_SYSTEM_PANEL_ONOFF)
    {
        _bfgSelfDiagCheckBacklight = FALSE;
        _bfgSelfDiagMuteOff = FALSE;
        _u4BacklightIsNormalTime = 0;
    }

    // Create self-diag Handle timer.
    UNUSED(x_timer_create(&_hSelfDiagTimer));
    
    // Create the self-diag handle semaphore
    VERIFY(x_sema_create(&_hSelfDiagSema, X_SEMA_TYPE_BINARY,
                         X_SEMA_STATE_LOCK) == OSR_OK);
    
    // Create the self-diag handle task
    _bfgSelfDiagGo = TRUE;
    if (x_thread_create(&_hSelfDiagHandle, "SelfDiagHandle", 0x800,
                        u4Priority, SelfDiagHandle, 0, NULL) != OSR_OK)
    {
        LOG(3, "\nError: Create error handle thread fail\n");
        x_sema_delete(_hSelfDiagSema);
        _hSelfDiagSema = NULL;
        _bfgSelfDiagGo = FALSE;
    }
}

void SelfDiagStop(D_SYSTEM_STATE eState)
{
    INT32 i4_ret;
    
    if (_bfgSelfDiagGo == FALSE)
    {
        // Self-diag task stops now
        LOG(3, "[SelfDiagStop] Self-Diag task already stops....\n");
        return;
    }
    
    SelfDiagPause();
    
    // Wait self-diag thread exit
    _bfgSelfDiagGo = FALSE;
    x_thread_resume(_hSelfDiagHandle);
    if (_hSelfDiagSema != NULL)
    {
        i4_ret = x_sema_lock_timeout(_hSelfDiagSema, 1500);
        if (i4_ret == OSR_OK)
        {
            x_sema_delete(_hSelfDiagSema);
            _hSelfDiagSema = NULL;
        }
        else if (i4_ret != OSR_TIMEOUT)
        {
            Printf("Self-Diag: lock semaphore timeout, force stop!!!\n");
        }
        else if (i4_ret != OSR_TIMEOUT)
        {
            Assert("x_sema_lock_timeout(_hSelfDiagSema, 1500)", __FILE__, (INT32)__LINE__);
        }
    }
    
    UNUSED(x_timer_delete(_hSelfDiagTimer));
    _hSelfDiagHandle = NULL;
    _hSelfDiagTimer = NULL;
    
    // If self-diag stops in panel power off state,
    // it is necessary to delete the timer handle of backlight detection.
    if (eState == D_SYSTEM_PANEL_ONOFF)
    {
        _bfgSelfDiagCheckBacklight = FALSE;
    }
}

void SelfDiagSetShutdown(D_SELF_DIAG_ERROR_TYPE eErrorType)
{
    UINT8  u1Val = 0, u1Count = 0;
    UINT32 u4LEDBlinkTime = 0;
    UINT64 u8Offset = SELF_DIAG_ERROR_CNT_ADDR;
    SELF_DIAG_TABLE eLastShutdownLog;
    PDWNC_FuncTbl *prPdwncFuncTbl;
    
    PRINT_TIMESTAMP_DBG_MSG("[Self Diag]", "Shutdown by error (%d)\n", 
                            (UINT32)eErrorType);
    
    // Set Shutdown state as "Error occur"
    UNUSED(EEPROM_Read(SELF_DIAG_SHUTDOWN_OCCUR_ADDR,
                       (UINT32)(VOID *)&u1Val, sizeof(UINT8)));
    u1Val |= CUSTOM_SHUTDOWN_ERROR;
    UNUSED(EEPROM_Write(SELF_DIAG_SHUTDOWN_OCCUR_ADDR,
                        (UINT32)(VOID *)&u1Val, sizeof(UINT8)));
                        
    LOG(5, "[SelfDiagSetShutdown] error type = %d\n", eErrorType);
    // Read the last shutdown log from EEPROM
    UNUSED(EEPROM_Read(SELF_DIAG_ERROR_LOG_ADDR, 
                       (UINT32)(VOID *)&eLastShutdownLog, sizeof(UINT16)));
    
    // Self diagnostic is abnormal, record the error information
    switch (eErrorType)
    {
    case D_12V_AMP_ERROR:
        u4LEDBlinkTime = 2;
        _eSelfDiagShutdownLog.u2Amp12VError = 1;
        u8Offset += SELF_DIAG_12V_AMP_ERR_OFFSET;
        break;
    case D_12V_SW_ERROR:
        u4LEDBlinkTime = 2;
        _eSelfDiagShutdownLog.u2SW12VError = 1;
        u8Offset += SELF_DIAG_12V_SW_ERR_OFFSET;
        break;
    case D_AUD_ERROR:
        u4LEDBlinkTime = 3;
        _eSelfDiagShutdownLog.u2AudError = 1;
        u8Offset += SELF_DIAG_AUD_ERR_OFFSET;
        break;
    case D_3V3_5V_ERROR:
        u4LEDBlinkTime = 3;
        _eSelfDiagShutdownLog.u2VDrop3V_5VError = 1;
        u8Offset += SELF_DIAG_3V3_5V_ERR_OFFSET;
        break;
    case D_PANEL_POWER_ERROR:
        u4LEDBlinkTime = 5;
        _eSelfDiagShutdownLog.u2PanelPowerError = 1;
        u8Offset += SELF_DIAG_PANEL_PWR_ERR_OFFSET;
        break;
    case D_BACKLIGHT_ERROR:
        u4LEDBlinkTime = 6;
        _eSelfDiagShutdownLog.u2BacklightError = 1;
        u8Offset += SELF_DIAG_BACKLIGHT_ERR_OFFSET;
        break;
    }
    
    // Write the shutdown log to EEPROM
    UNUSED(EEPROM_Write(SELF_DIAG_ERROR_LOG_ADDR, 
                        (UINT32)(VOID *)&_eSelfDiagShutdownLog, sizeof(UINT16)));

    // Read the shutdown counter from EEPROM
    UNUSED(EEPROM_Read(u8Offset, (UINT32)(VOID *)&u1Count, sizeof(UINT8)));

    // Increase the shutdown counter & update to EEPROM
    u1Count++;
    UNUSED(EEPROM_Write(u8Offset, (UINT32)(VOID *)&u1Count, sizeof(UINT8)));

    // Compare the current shutdown log and the previous one
    if (x_memcmp((void *)&eLastShutdownLog, (void *)&_eSelfDiagShutdownLog,
                 sizeof(UINT16)) == 0)
    {
        // There is the same error
        // Read the retry counter from EEPROM
        UNUSED(EEPROM_Read(SELF_DIAG_RETRY_CNT_ADDR,
                           (UINT32)(VOID *)&u1Count, sizeof(UINT8)));
        // Increase the retry counter
        u1Count++;
    }
    else
    {
        // Recovery successes
        // Reset the retry counter
        u1Count = 0;
    }
    
    // Update the retry counter to EEPROM
    UNUSED(EEPROM_Write(SELF_DIAG_RETRY_CNT_ADDR,
                        (UINT32)(VOID *)&u1Count, sizeof(UINT8)));
    
    // Set Failure LED blink
    prPdwncFuncTbl = PDWNC_EXT_GetFunc();
    if (prPdwncFuncTbl != NULL)
    {
        prPdwncFuncTbl->pfnSet8032uPLedBlink(u4LEDBlinkTime);
    }
    
    // Enter standby 
    PDWNC_EnterPowerDown((WAK_IRRX | WAK_UNOR), PDWNC_POWER_DOWN_IRRC);
}

UINT32 SelfDiagGetBootCounter(UINT32 *pu4BootCounter)
{
    UINT32 u4BootCounter = 0;
    
    // Read the boot counter from EEPROM
    if (EEPROM_Read(SELF_DIAG_BOOT_CNT_ADDR, 
                    (UINT32)(VOID *)&u4BootCounter, sizeof(UINT32)) == 0)
    {
        *pu4BootCounter = u4BootCounter;
        return (SELF_DIAG_OK);
    }
    else
    {
        return (SELF_DIAG_FAIL);
    }
}

UINT32 SelfDiagGetCounter(D_SELF_DIAG_STATE *prErrorInfo)
{
    UINT32 u4Ret = SELF_DIAG_OK;
    UINT32 u4Value = 0;
    UINT64 u8Offset = SELF_DIAG_ERROR_CNT_ADDR;
    SELF_DIAG_TABLE eShutdownLog;

    x_memset(&eShutdownLog, 0, sizeof(SELF_DIAG_TABLE));

    // Read the shutdown log from EEPROM
    UNUSED(EEPROM_Read(SELF_DIAG_ERROR_LOG_ADDR,
                       (UINT32)(VOID *)&eShutdownLog, sizeof(UINT16)));

    // Get the EEPROM offset of the shutdown counter
    switch (prErrorInfo->eErrorType)
    {
    case D_12V_AMP_ERROR:
        prErrorInfo->bErrorOccur = eShutdownLog.u2Amp12VError;
        u8Offset += SELF_DIAG_12V_AMP_ERR_OFFSET;
        break;
    case D_12V_SW_ERROR:
        prErrorInfo->bErrorOccur = eShutdownLog.u2SW12VError;
        u8Offset += SELF_DIAG_12V_SW_ERR_OFFSET;
        break;
    case D_24V_PWR_ERROR:
        prErrorInfo->bErrorOccur = eShutdownLog.u2SW12VError;
        u8Offset += SELF_DIAG_24V_PWR_ERR_OFFSET;
        break;
    case D_AUD_ERROR:
        prErrorInfo->bErrorOccur = eShutdownLog.u2AudError;
        u8Offset += SELF_DIAG_AUD_ERR_OFFSET;
        break;
    case D_3V3_5V_ERROR:
        prErrorInfo->bErrorOccur = eShutdownLog.u2VDrop3V_5VError;
        u8Offset += SELF_DIAG_3V3_5V_ERR_OFFSET;
        break;
    case D_PANEL_POWER_ERROR:
        prErrorInfo->bErrorOccur = eShutdownLog.u2PanelPowerError;
        u8Offset += SELF_DIAG_PANEL_PWR_ERR_OFFSET;
        break;
    case D_BACKLIGHT_ERROR:
        prErrorInfo->bErrorOccur = eShutdownLog.u2BacklightError;
        u8Offset += SELF_DIAG_BACKLIGHT_ERR_OFFSET;
        break;
    case D_TEMPERATURE_ERROR:
        prErrorInfo->bErrorOccur = eShutdownLog.u2BacklightError;
        u8Offset += SELF_DIAG_TEMPERATURE_ERR_OFFSET;
        break;
    default:
        u4Ret = SELF_DIAG_FAIL;
        break;
    }

    // Read the shutdown counter from EEPROM
    UNUSED(EEPROM_Read(u8Offset, (UINT32)(VOID *)&u4Value, sizeof(UINT8)));
    prErrorInfo->u4ErrorCnt = u4Value;
    return u4Ret;
}

UINT32 SelfDiagUpdateOnOffCount(void)
{
    // Update OnOff counter
    UNUSED(EEPROM_Write(SELF_DIAG_ON_OFF_CNT_ADDR,
                        (UINT32)(VOID *)&_u1SelfDiagOnOffCnt, sizeof(UINT8)));
    return SELF_DIAG_OK;
}

UINT32 SelfDiagResetCounter(void)
{
    UINT8  u1Data[D_ALL_ERROR];
    UINT32 u4DataLen = D_ALL_ERROR * sizeof(UINT8);
    UINT64 u8Offset = SELF_DIAG_ERROR_CNT_ADDR;

    x_memset(u1Data, 0, u4DataLen);
    UNUSED(EEPROM_Write(u8Offset, (UINT32)(VOID *)u1Data, u4DataLen));
    u8Offset = SELF_DIAG_ON_OFF_CNT_ADDR;
    UNUSED(EEPROM_Write(u8Offset, (UINT32)(VOID *)u1Data, 2));
    return SELF_DIAG_OK;
}

UINT32 SelfDiagResetBootCount(void)
{
    UINT32 u4Count = 0;
    UINT64 u8Offset = SELF_DIAG_BOOT_CNT_ADDR;

    UNUSED(EEPROM_Write(u8Offset, (UINT32)(VOID *)&u4Count, sizeof(u4Count)));
    return SELF_DIAG_OK;
}

void SelfDiagUpdateBacklightState(BOOL bIsBackltOn)
{
    CRIT_STATE_T rCritState;
    
    rCritState = x_crit_start();
    _bfgSelfDiagBackltOn = bIsBackltOn;
    x_crit_end(rCritState);
}

