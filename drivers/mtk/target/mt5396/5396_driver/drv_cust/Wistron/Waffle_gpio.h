
#ifndef CUSTOM_GPIO_H
#define CUSTOM_GPIO_H

#include "x_pdwnc.h"
#include "drv_pwm.h"
//#include "CustomCtrl.h"
#ifndef CC_UBOOT
#include "CustomCtrl.h"
#else
#ifdef TARGET_SPEC_DRV_CUST_CUSTOMIZED_HDR_FILE
#include TARGET_SPEC_DRV_CUST_CUSTOMIZED_HDR_FILE
#endif
#endif
//---------------------------------------------------------------------------
// Customer function definitions
//---------------------------------------------------------------------------
#ifndef CC_UBOOT
#define GPIO_POWER_ON_FUNC      (GPIO_CustomPowerOnFunc)
#define GPIO_POWER_OFF_FUNC     (GPIO_CustomPowerOffFunc)
#define GPIO_LED_CTRL_FUNC      (GPIO_CustomLedCtrlFunc)
#else
#define GPIO_POWER_ON_FUNC      (NULL)
#define GPIO_POWER_OFF_FUNC     (NULL)
#define GPIO_LED_CTRL_FUNC      (NULL)
#define LOADER_ENV_INIT_FUNC    (NULL)
#endif

#ifdef CC_MTK_LOADER
//#define LOADER_UPGRADE_VER_CMP_FUNC             (NULL)
#ifndef CC_UBOOT
#define LOADER_UPGRADE_DLM_CB_FUNC              (_LdrUpgradeDlmCbFunc)
#define LOADER_UPGRADE_DLM_GET_TAG_VER_CB_FUNC  (_LdrUpgradeDlmGetTagVerCbFunc)
#define LOADER_UPGRADE_DLM_CMP_TAG_VER_CB_FUNC  (_LdrUpgradeDlmCmpTagVerCbFunc)
#else
#define LOADER_UPGRADE_DLM_CB_FUNC              (NULL)
#define LOADER_UPGRADE_DLM_GET_TAG_VER_CB_FUNC  (NULL)
#define LOADER_UPGRADE_DLM_CMP_TAG_VER_CB_FUNC  (NULL)
#endif
//#define LOADER_UPGRADE_BREAK_HANDLE_FUNC        (_LdrUpgradeBreakFunc)
#else
#ifdef CC_MODEL_DETECT_SUPPORT
#define CUSTOMER_POWER_ON_SEQUENCE              (CustomPowerOnSequence)
#define CUSTOMER_POWER_OFF_SEQUENCE             (CustomPowerOffSequence)
#endif

#define LOGO_NOTIFY_FUNC                        (CustomLogoNotifyFunc)
#define CUSTOMER_LOGO_DISPLAY_FUNC              (CustomDisplayLogo)
#endif /* CC_MTK_LOADER */

#ifdef CUSTOM_POWER_STATE_CTRL_ENABLE
#define LOADER_CEC_CONFIG_QUERY_FUNC            (CustomCheckCECCtrlState)
#define POWER_STATE_QUERY_FUNC                  (PDWNC_Custom_CheckPowerState)
#endif /* CUSTOM_POWER_STATE_CTRL_ENABLE */

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define CUSTOM_IR_LED_BLINK_PERIOD              150         // 150ms

// Strap GPIO definitaion on PCB
#define CUSTOM_GPIO_PANEL_ID_PIN_0              GPIO(7)     // WXGA / FHD
#define CUSTOM_GPIO_PANEL_NVM_WP                GPIO(15)    // GPIO 15

#ifdef WAFFLE_PCB_DVT
#define CUSTOM_GPIO_LIGHT_SENSOR_PIN            GPIO(62)    // ETTXD0, 0: with light sensor
#define CUSTOM_GPIO_THERMAL_SENSOR_PIN          GPIO(59)    // ETRXD3, 0: with thermal sensor
#endif /* WAFFLE_PCB_DVT */

#ifdef WAFFLE_PCB_EVT_CHM
#define CUSTOM_GPIO_WOOFER_PIN                  GPIO(60)    // Woofer
#define CUSTOM_GPIO_DVD_INSERT_PIN              GPIO(203)   // OPCTRL3
#define CUSTOM_GPIO_PANEL_ID_PIN_1              GPIO(207)   // HDMI_HPD, 1: with DVD
#define CUSTOM_GPIO_CMO37_DET_PIN               GPIO(206)   // PWR5V, 0: CMO 37" FHD
#define CUSTOM_GPIO_PANLE_8_10_BIT_PIN          GPIO(217)   // HDMI_SCL, 1: 10 bit FHD
#else
#define CUSTOM_GPIO_PANEL_ID_PIN_1              GPIO(8)     // GPIO 8
#endif /* WAFFLE_PCB_EVT_CHM */

#define CUSTOM_GPIO_1H2H_DETCTION_PIN          GPIO(56)   // 1: 2H; 0:1H model

#define CUSTOM_PCB_TYPE_1_WXGA                  0x00
#define CUSTOM_PCB_TYPE_1_FHD                   0x01
#define CUSTOM_PCB_TYPE_2                       0x02
#define CUSTOM_PCB_TYPE_3                       0x03
#define CUSTOM_PCB_TYPE_5                       0x05

#define CUSTOM_GPIO_PANEL_OTHER                 2
#define CUSTOM_GPIO_PANEL_WXGA                  1
#define CUSTOM_GPIO_PANEL_FHD                   0
#define CUSTOM_GPIO_LVDS_OTHER_BIT              2
#define CUSTOM_GPIO_LVDS_10BIT                  1
#define CUSTOM_GPIO_LVDS_8BIT                   0
#define CUSTOM_GPIO_FACTORY_SONY                0
#define CUSTOM_GPIO_FACTORY_WISTRON             1
#define CUSTOM_GPIO_DVD_NA                      0
#define CUSTOM_GPIO_DVD_INCLUDE                 1
#define CUSTOM_GPIO_FHD_CMO_37                  0
#define CUSTOM_GPIO_FHD_OTHER                   1
#define CUSTOM_GPIO_AUD_NORMAL                  0
#define CUSTOM_GPIO_AUD_WOOFER                  1
#define CUSTOM_GPIO_LIGHT_SENSOR_INCLUDE        0
#define CUSTOM_GPIO_LIGHT_SENSOR_NA             1
#define CUSTOM_GPIO_THERMAL_SENSOR_NA           0
#define CUSTOM_GPIO_THERMAL_SENSOR_INCLUDE      1
#define CUSTOM_PCB_TYPE_1H						0
#define CUSTOM_PCB_TYPE_2H						1


#define CUSTOM_PANEL_WIDTH_FHD                  1920
#define CUSTOM_PANEL_WIDTH_WXGA                 1366

// LED GPIO 
#define CUSTOM_GPIO_LED_STANDBY                 GPIO(211)   // VGA_SDA

#define CUSTOM_GPIO_LED_POWER                   ADIN(2)     // ADIN2
#define CUSTOM_GPIO_LED_TIMER_AMBER             ADIN(5)     // ADIN5
#define CUSTOM_GPIO_LED_TIMER_GREEN             GPIO(26)    // GPIO 26
#define CUSTOM_LED_POWER_ON_VAL                 1
#define CUSTOM_LED_STANDBY_ON_VAL               1
#define CUSTOM_LED_TIMER_ON_VAL                 1

#define T8032_CTRL_ERR_LED                      CUSTOM_GPIO_LED_STANDBY
#define T8032_CTRL_ERR_LED_ON_VAL               CUSTOM_LED_STANDBY_ON_VAL

// Power Key
#define BUTTON_POWER_KEY                        OPCTRL(2)

// Shutdown status
#define CUSTOM_SHUTDOWN_ERROR                   (1 << 0)
#define CUSTOM_SHUTDOWN_AC_DET                  (1 << 1)
#define CUSTOM_SHUTDOWN_DISABLE_STANDBY_LED     (1 << 2)

// DVD GPIO
#define CUSTOM_GPIO_DVD_RESET                   GPIO(12)
#define CUSTOM_DVD_POWER_ON                     1
#define CUSTOM_DVD_POWER_OFF                    0

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

#define INIT_SET_GPIO(gpio, onval)                                  \
    if (DRVCUST_InitQuery(gpio, &i)==0)                             \
    {                                                               \
        if (DRVCUST_InitQuery(onval, (UINT32 *)&i4Val)==0)          \
        {                                                           \
            VERIFY(GPIO_Output((INT32)i, &i4Val) == i4Val);         \
        }                                                           \
    }

#define DOWN_SET_GPIO(gpio, onval)                                  \
    if (DRVCUST_InitQuery(gpio, &i)==0)                             \
    {                                                               \
        if (DRVCUST_InitQuery(onval, (UINT32 *)&i4Val)==0)          \
        {                                                           \
            i4Val = !i4Val;                                         \
            VERIFY(GPIO_Output((INT32)i, &i4Val)== i4Val);          \
        }                                                           \
    }

//-----------------------------------------------------------------------------
// Structure definition
//-----------------------------------------------------------------------------
typedef struct _LedCtrl
{
    D_LED_TYPE eLedType;
    UINT32 u4LedState;
} LED_CTRL_STATE;

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
void GPIO_CustomPowerOnFunc(void);
void GPIO_CustomPowerOffFunc(UINT32 u4PowerDownEn);
void GPIO_CustomLedCtrlFunc(LED_STATUS_T eLedStatus);
void CustomPowerOnSequence(void);
void CustomPowerOffSequence(void);
UINT32 CustomModelQueryFunc(VOID);
void _LdrUpgradeBreakFunc(void);
void _LdrUpgradeDlmCbFunc(UINT32 u4BufSize);
void _LdrUpgradeDlmGetTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer);
INT32 _LdrUpgradeDlmCmpTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer, CHAR *szUpgVer);
#endif /* CUSTOM_GPIO_H*/

