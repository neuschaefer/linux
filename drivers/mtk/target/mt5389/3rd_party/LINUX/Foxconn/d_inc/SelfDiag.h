
#ifndef CUSTOM_SELF_DIAG_H
#define CUSTOM_SELF_DIAG_H

//#include "d_Waffle_cust.h"

#ifdef TARGET_SPEC_DRV_CUST_CUSTOMIZED_HDR_FILE
#include TARGET_SPEC_DRV_CUST_CUSTOMIZED_HDR_FILE
#endif

//---------------------------------------------------------------------------
// Macro
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define SELF_DIAG_OK                            (UINT32)0
#define SELF_DIAG_FAIL                          (UINT32)(-1)
#define SELF_DIAG_ENABLE                        (0)
#define SELF_DIAG_DISABLE                       (1)

// Self Diagnostic definition
#define SELF_DIAG_12V_AMP_DROP_DET              GPIO(22)    // GPIO 22
#define SELF_DIAG_12V_SW_DROP_DET               GPIO(17)    // GPIO 17
#define SELF_DIAG_24V_AMP_VDD_DROP_DET          GPIO(0)     // GPIO 0
#define SELF_DIAG_AUD_AMP_ERROR_DET             GPIO(1)     // GPIO 1
#define SELF_DIAG_V_DROP_DET                    GPIO(11)    // GPIO 11
#define SELF_DIAG_PANEL_V_DROP_DET              GPIO(4)     // GPIO 4
#define SELF_DIAG_LAMP_DET                      GPIO(9)     // GPIO 9

// Threshold & default value
#define SELF_DIAG_TEMP_LIMIT_DEFAULT            0xA0        // 160 (80C)

#define SELF_DIAG_SERVO_VALID_MAX               63
#define SELF_DIAG_TEMP_VALID_MAX                0x1FF
#define SELF_DIAG_COUNTER_VALID_MAX             0xFFFFFFFF
#define SELF_DIAG_ON_OFF_RETRY_LIMIT            0x3

// Detect time period definition
#define SELF_DIAG_DETECT_PERIOD                 100         // 100 ms

#define SELF_DIAG_POWER_DET_TIME                900         // 900 ms
#define SELF_DIAG_MAIN_DET_TIME                 3000        // 3 sec
#define SELF_DIAG_INVERTER_DET_TIME             900         // 900 ms
#define SELF_DIAG_LAMP_DET_TIME                 3000        // 3 sec
#define SELF_DIAG_BL_DELAY_TIME                 400         // 400 ms
#define SELF_DIAG_BL_RESET_TIME                 5000        // 5 sec

// EEPROM information
#define SELF_DIAG_DATA_INIT_ADDR                0x2380
#define SELF_DIAG_SHUTDOWN_OCCUR_ADDR           0x2381
#define SELF_DIAG_DET_ENABLE_ADDR               0x23C0
#define SELF_DIAG_ERROR_LOG_ADDR                0x23C2
#define SELF_DIAG_BOOT_RETRY_ADDR               0x23C4
#define SELF_DIAG_ERROR_CNT_ADDR                0x23C8
#define SELF_DIAG_ON_OFF_CNT_ADDR               0x23DA
#define SELF_DIAG_RETRY_CNT_ADDR                0x23DB
#define SELF_DIAG_BOOT_CNT_ADDR                 0x23DC

#define SELF_DIAG_12V_AMP_ERR_OFFSET            0
#define SELF_DIAG_12V_SW_ERR_OFFSET             1
#define SELF_DIAG_24V_PWR_ERR_OFFSET            2
#define SELF_DIAG_AUD_ERR_OFFSET                3
#define SELF_DIAG_3V3_5V_ERR_OFFSET             4
#define SELF_DIAG_PANEL_PWR_ERR_OFFSET          5
#define SELF_DIAG_BACKLIGHT_ERR_OFFSET          6
#define SELF_DIAG_TEMPERATURE_ERR_OFFSET        7
#define SELF_DIAG_TCON__ERR_OFFSET              8
#define SELF_DIAG_BALANCE_ERR_OFFSET            9

#define SELF_DIAG_TEMP_LIMIT_ADDR               0x23E0
#define SELF_DIAG_RETRY_LIMIT_ADDR              0x23D8

#define SELF_DIAG_EEPROM_SIZE                   64

// LOAD default data
#define SELF_DIAG_LOAD_DEFAULT                  (1 << 0)
#define SELF_DIAG_CLEAR_DATA_INIT               (0)

typedef enum
{
    SELF_DIAG_ACT_REBOOT,
    SELF_DIAG_ACT_PANEL_ONOFF,
    SELF_DIAG_ACT_SHUTDOWN,
    SELF_DIAG_ACT_START,
    SELF_DIAG_ACT_STOP
} SELF_DIAG_ERR_ACTION;

typedef struct _SELF_DIAG_TABLE_T
{
    UINT16  u2TemperatureError : 1;
    UINT16  u2BacklightError : 1;
    UINT16  u2PanelPowerError : 1;
    UINT16  u2VDrop3V_5VError : 1;
    UINT16  u2AudError : 1;
    UINT16  u224VError : 1;
    UINT16  u2SW12VError : 1;
    UINT16  u2Amp12VError : 1;
    UINT16  u2Reverved : 5;
    UINT16  u2AllError : 1;
    UINT16  u2BalancerError : 1;
    UINT16  u2TconError : 1;
} SELF_DIAG_TABLE;

//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------
void SelfDiagSetModelType(UINT32 u4ModelType);
void SelfDiagSetShutdown(D_SELF_DIAG_ERROR_TYPE eErrorType);
void SelfDiagStart(D_SYSTEM_STATE eState);
void SelfDiagStop(D_SYSTEM_STATE eState);
void SelfDiagPause(void);
void SelfDiagResume(void);
void SelfDiagUpdateBacklightState(BOOL bIsBackltOn);
UINT32 SelfDiagGetCounter(D_SELF_DIAG_STATE *prErrorInfo);
UINT32 SelfDiagUpdateOnOffCount(void);
UINT32 SelfDiagResetCounter(void);
UINT32 SelfDiagResetBootCount(void);


//#else
//#error "reinclude SelfDiag.h..."
#endif

