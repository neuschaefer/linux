
#ifndef POWERSEQ_H
#define POWERSEQ_H

#include "Waffle_gpio.h"

//-----------------------------------------------------------------------------
// Constant definitions & Macro
//-----------------------------------------------------------------------------
#ifdef CC_MODEL_DETECT_SUPPORT
#define CUSTOM_PANEL_ID_CHECK_ENABLE
#endif /* CC_MODEL_DETECT_SUPPORT */

#define CUSTOM_GET_AUTO_DETECT_FROM_EEPROM
#define CUSTOM_GET_DELAY_FROM_EEPROM
//#define CUSTOM_ALL_AC_DET_LOG

// EEPROM information

// Detect time period definition
#define TCON_READY_DETECT_TIME              3000

#define POWER_SEQ_AC_DET_TIME_ADDR          0x2384
#define POWER_SEQ_AC_DET_RETRY_ADDR         0x2386
#define POWER_SEQ_DELAY_ADDR                0x2388
#define POWER_SEQ_CHECK_PANEL_ADDR          0x23BF

#define POWER_SEQ_AC_DET_TIME_DEFAULT       10
#define POWER_SEQ_AC_DET_RETRY_LIMIT        5
#define POWER_SEQ_DELAY_VALID_MAX           0xFFFF

#define POWER_SEQ_DELAY_DATA_SIZE           14

// AC detect pin definition
#define SIGNAL_AC_DET                       GPIO(2)     // GPIO 2
#define AC_DETECT_NORMAL                    0
#define AC_DETECT_ERROR                     1

// Panel format select pin definition
#define SIGNAL_LVDS_SEL                     GPIO(3)     // GPIO 3

#ifdef WAFFLE_PCB_EVT_CHM
#define SIGNAL_X_U_RST                      GPIO(208)   // HDMI_SDA
#endif /* WAFFLE_PCB_EVT_CHM */

#define GET_POWER_SEQ_DELAY(Address, Delay, Value, DataLen, Default)    \
    UNUSED(EEPROM_Read(Address, (UINT32)(VOID *)&Value, DataLen));      \
    Value = (((Value & 0xFF00) >> 8) | ((Value & 0xFF) << 8));          \
    Delay = (Value == POWER_SEQ_DELAY_VALID_MAX) ? Default : Value;     \
    if (Value != Delay)                                                 \
    {                                                                   \
        Value = (((Delay & 0xFF00) >> 8) | ((Delay & 0xFF) << 8));      \
        UNUSED(EEPROM_Write(Address, (UINT32)(VOID *)&Value, DataLen)); \
    }

#ifndef CC_MTK_LOADER
typedef enum
{
    CUSTOM_POWER_SEQ_CLEAR_SHUTDOWN_STATE,
    CUSTOM_POWER_SEQ_SET_SHUTDOWN_STATE
} CUSTOM_POWER_SEQ_ACTION;
#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------
void CustomPowerOnSequence(void);
void CustomPowerOffSequence(void);
void CustomStartACDetect(void);
void CustomStopACDetect(void);

#endif /* POWERSEQ_H */
