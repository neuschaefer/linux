#ifndef CUSTOME_CTRL_H
#define CUSTOME_CTRL_H

#include "SelfDiag.h"
#include "drvcust_if.h"
#include "d_sony_2k12_cust.h"

//-----------------------------------------------------------------------------
// Constant definitions & Macro
//-----------------------------------------------------------------------------
//#define CUSTOM_SELF_DIAG_ENABLE
#define CUSTOM_SHUTDOWN_OCCUR_ADDR              SELF_DIAG_SHUTDOWN_OCCUR_ADDR
#define CUSTOM_POWER_STATE_ADDR                 0x039D
#define LOADER_UPGRADE_BREAK_STATE_OFFSET       0x039E
#define LOADER_UPGRADE_FORCE_DISABLE_OFFSET     0x039F

// Option bit in EEPROM
#define CUSTOM_COUNTRY_EXT_ADDR                 0x06C7
#define CUSTOM_MODEL_EXT_ADDR                   0x06CB
#define CUSTOM_LIGHT_SENSOR_OPTION_ADDR         0x06C4
#define CUSTOM_SHIPPING_REGION_OPTION_ADDR      0x06C9

#define CUSTOM_OPTION_LIGHT_SENSOR_OFF          (0)

// Shutdown status
#define CUSTOM_SHUTDOWN_STATE_DEFAULT           (0)
#define CUSTOM_SHUTDOWN_ERROR                   (1 << 0)
#define CUSTOM_SHUTDOWN_AC_DET                  (1 << 1)
#define CUSTOM_SHUTDOWN_ENABLE_STANDBY_LED     (1 << 2)

#ifdef CUSTOM_SELF_DIAG_ENABLE
#define SELF_DIAG_START(system_state)           SelfDiagStart(system_state)
#define SELF_DIAG_STOP(system_state)            SelfDiagStop(system_state)
#define SELF_DIAG_PAUSE(void)                   SelfDiagPause()
#define SELF_DIAG_RESUME(void)                  SelfDiagResume()
#define SELF_DIAG_SET_MODEL_TYPE(type)          SelfDiagSetModelType(type)
#define SELF_DIAG_SET_SHUTDOWN(error_type)      SelfDiagSetShutdown(error_type)
#else
#define SELF_DIAG_START(system_state)
#define SELF_DIAG_STOP(system_state)
#define SELF_DIAG_PAUSE(void)
#define SELF_DIAG_RESUME(void)
#define SELF_DIAG_SET_MODEL_TYPE(type)
#define SELF_DIAG_SET_SHUTDOWN(error_type)
#endif /* CUSTOM_SELF_DIAG_ENABLE */

// OPWM configuration for ESD
#ifdef WAFFLE_PCB_EVT_CHM
#define CUSTOM_ESD_PWM_PORT                     (0)
#define CUSTOM_ESD_PWM_DUTY_CYLE                40      // 50%
#else
#define CUSTOM_ESD_PWM_PORT                     (2)
#define CUSTOM_ESD_PWM_DUTY_CYLE                50      // 50%
#endif /* WAFFLE_PCB_EVT_CHM */

#define CUSTOM_ESD_PWM_FREQ                     30000   // 30KHz

typedef enum
{
    STATE_AC_OFF = 0,
    STATE_STANDBY,
    STATE_POWER_OFF,
    STATE_POWER_ON,
    STATE_TOTAL
} POWER_STATE_TYPE;

// Logo display configuration
#define LOGO_THREAD_NAME                        "StartLogo"
#define LOGO_THREAD_STACK_SIZE                  2048
#define LOGO_THREAD_PRIORITY                    100

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------
#if 0//#ifdef NDEBUG
#define PRINT_TIMESTAMP_MSG(Title, String)
#define PRINT_TIMESTAMP_DBG_MSG(Title, String, value)
#else
#define PRINT_TIMESTAMP_MSG(Title, String)                          \
{                                                                   \
    HAL_TIME_T _rTime;                                              \
    HAL_GetTime(&_rTime);                                           \
    Printf(Title);                                                  \
    Printf(" (%06d:%03d) ", _rTime.u4Seconds, _rTime.u4Micros/1000);\
    Printf(String);                                                 \
}

#define PRINT_TIMESTAMP_DBG_MSG(Title, String, value)               \
{                                                                   \
    HAL_TIME_T _rTime;                                              \
    HAL_GetTime(&_rTime);                                           \
    Printf(Title);                                                  \
    Printf(" (%06d:%03d) ", _rTime.u4Seconds, _rTime.u4Micros/1000);\
    Printf(String, value);                                          \
}
#endif

//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------
void GPIO_CustomReadStrapPin(void);
void GPIO_CustomLedCtrlFunc(LED_STATUS_T eLedStatus);
void CustomSetTimerLED(BOOL bIsEnable);
void CustomLEDBlinkCtrl(D_LED_TYPE eLedType);
void CustomLEDCtrlFunc(D_LED_TYPE eLedType, UINT32 u4LedState);
void CustomIRLedHandleStart(void);
void CustomIRLedHandleStop(void);
void CustomIRLedTimerStop(void);
void CustomLogoNotifyFunc(BOOL bIsShowLogo);
void CustomDisplayLogo(void);
void CustomUpdateBacklightState(BOOL bIsBackltOn);
BOOL CustomGetBackltStatus(void);
BOOL CustomGetPowerSeqStatus(void);
BOOL CustomCheckCECCtrlState(void);
BOOL PDWNC_Check_Enter_SemiStandy(void);
INT32 CustomQueryTemperature(void);
INT32 CustomReadTemperature(void);
UINT32 CustomQueryTempRawData(void);
UINT32 CustomQueryPowerSequenceDelay(UINT32 u4Index);
UINT32 CustomQueryPowerState(void);
UINT32 CustomPCBAQueryCountry(VOID);
UINT32 CustomModelOptionQueryFunc(VOID);
UINT32 CustomShippingRegionOptionQueryFunc(VOID);
UINT32 PDWNC_Custom_CheckPowerState(void);

void _LdrUpgradeBreakFunc(void);
void _LdrUpgradeDlmCbFunc(UINT32 u4BufSize);
void _LdrUpgradeDlmGetTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer);
INT32 _LdrUpgradeDlmCmpTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer, CHAR *szUpgVer);

#endif /* CUSTOME_CTRL_H*/

