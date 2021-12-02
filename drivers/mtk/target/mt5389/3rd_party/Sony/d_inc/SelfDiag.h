
#ifndef CUSTOM_SELF_DIAG_H
#define CUSTOM_SELF_DIAG_H

#include "d_sony_2k12_cust.h"

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
#define SELF_DIAG_12V_AMP_DROP_DET              ADIN(4)     // ADIN4
#define SELF_DIAG_12V_SW_DROP_DET               ADIN(3)     // ADIN3 
#define SELF_DIAG_V_DROP_DET                    GPIO(38)    // DEMOD_TSCLK
#define SELF_DIAG_PANEL_V_DROP_DET              GPIO(66)    // DEMOD_TSVAL
#define SELF_DIAG_TUNER_PWR_DROP_DET            OPCTRL(10)  // VGA_SCL

#ifdef CC_WAFFLE_PA_PANALOG_ONLY_RC_SUPPORT  
    #define SELF_DIAG_AUD_AMP_ERROR_DET             GPIO(1)    // GPIO 1
#else
	#define SELF_DIAG_AUD_AMP_ERROR_DET 			GPIO(48)    // SPI_CLE
    #define CUSTOM_AMP_SIF_CLK_DIV          0x100
    #define CUSTOM_AMP_SIF_DEV_ADDR         0x54
    #define CUSTOM_AMP_SIF_SUB_ADDR         0x2
    #define CUSTOM_AMP_DATA_LEN             1
#endif

#define SELF_DIAG_LAMP_DET                      GPIO(49)    // CI_INT

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
#define SELF_DIAG_TUNER_DET_TIME                3000        // 3 sec
#define SELF_DIAG_INVERTER_DET_TIME             900         // 900 ms
#define SELF_DIAG_LAMP_DET_TIME                 3000        // 3 sec
#define SELF_DIAG_BL_DELAY_TIME                 400         // 400 ms
#define SELF_DIAG_BL_RESET_TIME                 5000        // 5 sec

// EEPROM information
#define SELF_DIAG_BASE_ADDR                     (0x1F00)
#define SELF_DIAG_DATA_INIT_ADDR                (SELF_DIAG_BASE_ADDR)
#define SELF_DIAG_SHUTDOWN_OCCUR_ADDR           (SELF_DIAG_BASE_ADDR + 0x01)
#define SELF_DIAG_DET_ENABLE_ADDR               (SELF_DIAG_BASE_ADDR + 0x40)
#define SELF_DIAG_ERROR_LOG_ADDR                (SELF_DIAG_BASE_ADDR + 0x42)
#define SELF_DIAG_BOOT_RETRY_ADDR               (SELF_DIAG_BASE_ADDR + 0x44)
#define SELF_DIAG_ERROR_CNT_ADDR                (SELF_DIAG_BASE_ADDR + 0x48)
#define SELF_DIAG_ON_OFF_CNT_ADDR               (SELF_DIAG_BASE_ADDR + 0x5A)
#define SELF_DIAG_RETRY_CNT_ADDR                (SELF_DIAG_BASE_ADDR + 0x5B)
#define SELF_DIAG_BOOT_CNT_ADDR                 (SELF_DIAG_BASE_ADDR + 0x5C)

#define SELF_DIAG_12V_AMP_ERR_OFFSET            D_12V_AMP_ERROR//0
#define SELF_DIAG_12V_SW_ERR_OFFSET             D_12V_SW_ERROR//1
#define SELF_DIAG_24V_PWR_ERR_OFFSET            D_24V_PWR_ERROR//2
#define SELF_DIAG_AUD_ERR_OFFSET                D_AUD_ERROR//3
#define SELF_DIAG_3V3_5V_ERR_OFFSET             D_3V3_5V_ERROR//4
#define SELF_DIAG_PANEL_PWR_ERR_OFFSET          D_PANEL_POWER_ERROR//5
#define SELF_DIAG_BACKLIGHT_ERR_OFFSET          D_BACKLIGHT_ERROR//6
#define SELF_DIAG_TEMPERATURE_ERR_OFFSET        D_TEMPERATURE_ERROR//7
#define SELF_DIAG_TCON_ERR_OFFSET               D_TCON_ERROR//8
#define SELF_DIAG_BALANCE_ERR_OFFSET            D_BALANCER_ERROR//9
#define SELF_DIAG_TUNER_ERR_OFFSET              D_TUNER_ERROR//10

#define SELF_DIAG_FUNCTION_ENABLE(x)                     (1U << (x)) 
/*Disable u224VError, u2BalancerError and u2TconError detection on Waffle PAA 2H model*/
#define DEFAULT_SELF_DIAG_ENABLED_ITEMS         (                                              \
                                                    SELF_DIAG_FUNCTION_ENABLE(D_12V_AMP_ERROR)      |   \
                                                    SELF_DIAG_FUNCTION_ENABLE(D_12V_SW_ERROR)       |   \
/*                                                  SELF_DIAG_FUNCTION_ENABLE(D_24V_PWR_ERROR)      |*/ \
                                                    SELF_DIAG_FUNCTION_ENABLE(D_AUD_ERROR)          |   \
                                                    SELF_DIAG_FUNCTION_ENABLE(D_3V3_5V_ERROR)       |   \
                                                    SELF_DIAG_FUNCTION_ENABLE(D_PANEL_POWER_ERROR)  |   \
                                                    SELF_DIAG_FUNCTION_ENABLE(D_BACKLIGHT_ERROR)    |   \
                                                    SELF_DIAG_FUNCTION_ENABLE(D_TEMPERATURE_ERROR)  |   \
/*                                                  SELF_DIAG_FUNCTION_ENABLE(D_TCON_ERROR)         |*/ \
/*                                                  SELF_DIAG_FUNCTION_ENABLE(D_BALANCER_ERROR)     |*/ \
                                                    SELF_DIAG_FUNCTION_ENABLE(D_TUNER_ERROR)        |   \
                                                    0)

#define SELF_DIAG_TEMP_LIMIT_ADDR               (SELF_DIAG_BASE_ADDR + 0x60)
#define SELF_DIAG_RETRY_LIMIT_ADDR              (SELF_DIAG_BASE_ADDR + 0x58)

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

//-----------------------------------------------------------------------------
// The order of the following struct members should be sync. to 'Shutdown Log' on
// Self-diagnostical eeprom layout 
// If new monitor item were added, the item should be next to u2AllError and u2Reverved should be decrease one
//-----------------------------------------------------------------------------

typedef struct _SELF_DIAG_TABLE_T
{
    UINT16  u2TemperatureError : 1;     //Bit0 of low byte of Shutdown Log
    UINT16  u2BacklightError : 1;       //Bit1 of low byte of Shutdown Log
    UINT16  u2PanelPowerError : 1;      //Bit2 of low byte of Shutdown Log
    UINT16  u2VDrop3V_5VError : 1;      //Bit3 of low byte of Shutdown Log
    UINT16  u2AudError : 1;             //Bit4 of low byte of Shutdown Log
    UINT16  u224VError : 1;             //Bit5 of low byte of Shutdown Log
    UINT16  u2SW12VError : 1;           //Bit6 of low byte of Shutdown Log
    UINT16  u2Amp12VError : 1;          //MSB of low byte of Shutdown Log
    UINT16  u2Reverved : 4;             //Bit3~0 of high byte of Shutdown Log
    UINT16  u2AllError : 1;             //Bit4 of high byte of Shutdown Log
    UINT16  u2TunerError : 1;           //Bit5 of high byte of Shutdown Log
    UINT16  u2BalancerError : 1;        //Bit6 of high byte of Shutdown Log
    UINT16  u2TconError : 1;            //MSB of high byte of Shutdown Log
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

