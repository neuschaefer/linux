
#include "x_bim.h"
#include "x_debug.h"
#include "x_hal_arm.h"
#include "x_timer.h"
#include "x_assert.h"
#include "ostg_if.h"
#include "panel.h"
#include "pdwnc_if.h"
#include "rtc_if.h"
#include "mute_if.h"
#include "eeprom_if.h"
#include "drv_display.h"
#include "drv_video.h"
#include "pmx_drvif.h"
#include "osd_drvif.h"
#include "aud_if.h"
#include "PowerSeq.h"
#include "CustomCtrl.h"
#include "pe_if.h"
#include "eepdef.h"
#include "api_eep.h"
#include "ir_if.h"

//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported funtions 
//-----------------------------------------------------------------------------
extern void CHIP_Reboot(void);
extern void vDrvTurnOnPanel(void);
extern void vDrvTurnOffPanel(void);
extern void vDrvLVDSOff(void);
extern void vDrvLVDSConfig(void);
extern void vDrvLVDSOn(void);
extern void vDrvSetLock(UINT32 u4Src, UINT32 u4LockON);
extern UINT32 PANEL_GetLvdsOnDalay(void);
extern UINT32 PANEL_GetLvdsOffDalay(void);
extern UINT32 PANEL_GetBacklightOnDelay(void);
extern UINT32 PANEL_GetBacklightOffDelay(void);


extern UINT32 _u4BrightLevel;
extern UINT32 _u4gModelType;
extern UINT32 _u4gPanelType;
extern UINT32 _u4gLVDSType;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
UINT16 _au2PowerSeqDelay[POWER_SEQ_DELAY_DATA_SIZE];
HAL_TIME_T _rTime;

static UINT16 _au2DefaultDelay[POWER_SEQ_DELAY_DATA_SIZE] = 
       {10, 300, 40, 300, 500, 400, 200, 100, 100, 270, 10, 200, 0, 200};

static BOOL _bfgPowerOnIsDone = FALSE;
static BOOL _bfgBackltOn = FALSE;
static BOOL _bfgLVDSOutput = FALSE;
#ifdef CUSTOM_PANEL_ID_CHECK_ENABLE
static BOOL _bfgCheckPanelID = TRUE;
#else
static BOOL _bfgCheckPanelID = FALSE;
#endif /* CUSTOM_PANEL_ID_CHECK_ENABLE */

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
void CustomGetPowerSequenceDelay(void)
{
    static BOOL bInit = FALSE;
    UINT32 u4Len = sizeof(UINT16) * POWER_SEQ_DELAY_DATA_SIZE;

#ifdef CUSTOM_GET_AUTO_DETECT_FROM_EEPROM
    UINT8  u1AutoDetect = 0xFF;
#endif /* CUSTOM_GET_AUTO_DETECT_FROM_EEPROM */

    if (bInit)
    {
        return;
    }

#ifdef CUSTOM_GET_AUTO_DETECT_FROM_EEPROM
    // Get Panel ID check option
    UNUSED(EEPROM_Read(POWER_SEQ_CHECK_PANEL_ADDR, 
                       (UINT32)(VOID *)&u1AutoDetect, sizeof(UINT8)));
    if (u1AutoDetect > 1)
    {
        u1AutoDetect = 1;
        UNUSED(EEPROM_Write(POWER_SEQ_CHECK_PANEL_ADDR, 
                            (UINT32)(VOID *)&u1AutoDetect, sizeof(UINT8)));
    }
    
    _bfgCheckPanelID = (BOOL)u1AutoDetect;
#endif /* CUSTOM_GET_AUTO_DETECT_FROM_EEPROM */

#ifdef CUSTOM_GET_DELAY_FROM_EEPROM
    UINT16 u2Val = 0, i;
    UINT64 u8Addr = POWER_SEQ_DELAY_ADDR;

    x_memset(_au2PowerSeqDelay, 0xFF, u4Len);
    
    // Get power sequence delay from EEPROM
    u4Len = sizeof(UINT16);
    for (i = 0; i < POWER_SEQ_DELAY_DATA_SIZE; i++)
    {
        GET_POWER_SEQ_DELAY(u8Addr, _au2PowerSeqDelay[i], 
                            u2Val, u4Len, _au2DefaultDelay[i]);
        u8Addr += sizeof(UINT16);
    }
#else
    x_memcpy(_au2PowerSeqDelay, _au2DefaultDelay, u4Len);
    
#endif /* CUSTOM_GET_DELAY_FROM_EEPROM */

    bInit = FALSE;
}

UINT32 CustomQueryPowerSequenceDelay(UINT32 u4Index)
{
    return (_au2PowerSeqDelay[u4Index]);
}

void CustomUpdateBacklightState(BOOL bIsBackltOn)
{
    CRIT_STATE_T rCritState;
    
    rCritState = x_crit_start();
    _bfgBackltOn = bIsBackltOn;
    x_crit_end(rCritState);
    
#ifdef CUSTOM_SELF_DIAG_ENABLE
    SelfDiagUpdateBacklightState(bIsBackltOn);
#endif /* CUSTOM_SELF_DIAG_ENABLE */
}

BOOL CustomGetBackltStatus(void)
{
#if 1
    return _bfgBackltOn;
#else
    static BOOL bInitial = TRUE;
    static UINT32 u4Gpio, u4Value;

    if (bInitial)
    {
        u4Gpio = DRVCUST_PanelGet(eBackLightGpio);
        u4Value = DRVCUST_PanelGet(eBackLightOnVal);
        bInitial = FALSE;
    }
    
    return ((u4Value == GPIO_Output(u4Gpio, NULL)) ? TRUE : FALSE);
#endif    
}

void CustomSetPowerSeqStatus(BOOL fgOnOff)
{
    CRIT_STATE_T rCritState;
    
    rCritState = x_crit_start();
    _bfgPowerOnIsDone = fgOnOff;
    x_crit_end(rCritState);
}

BOOL CustomGetPowerSeqStatus(void)
{
    BOOL bPowerOnIsDone;
    CRIT_STATE_T rCritState;
    
    rCritState = x_crit_start();
    bPowerOnIsDone = _bfgPowerOnIsDone;
    x_crit_end(rCritState);
    
    return bPowerOnIsDone;
}

void CustomStartDimmerOutput(void)
{
    PRINT_TIMESTAMP_MSG("[Power On]", "START_DIMMER_OUTPUT\n");
    
    // Start dimmer output        
    // after panel on, we could set backlight
#ifndef CC_SONY_BACKLIGHT_SEQUENCE
//    vApiSetPanelBright(_u4BrightLevel);
    BACKLT_SetDimming(BACKLT_GetDimmingFrequency(), (UINT32)_u4BrightLevel);
#endif /* CC_SONY_BACKLIGHT_SEQUENCE */

    if (DRVCUST_PanelGet(eFlagBacklightDimmingLock))
    {
        vDrvSetLock(DRVCUST_PanelGet(ePanelBacklightPwmPort), SV_ON);
    }
    else
    {
        vDrvSetLock(DRVCUST_PanelGet(ePanelBacklightPwmPort), SV_OFF);
    }
}

void CustomLVDSOutputCtrl(HANDLE_T hHandle, VOID* pTag)
{
    UNUSED(hHandle);
    UNUSED(pTag);

    CRIT_STATE_T rCritState;
    
    if (_bfgLVDSOutput)
    {
        return;
    }
    
    rCritState = x_crit_start();
    _bfgLVDSOutput = TRUE;
    x_crit_end(rCritState);
    
    // Send LVDS output
    PRINT_TIMESTAMP_MSG("[Power On]", "LVDS_OUTPUT (Send LVDS output)\n");
    vDrvLVDSConfig();
    vDrvLVDSOn();
}

void CustomCheckPanelId(void)
{
    UINT32 u4CurrentPanelType;
    UINT32 u4NewPanelIndex;
    UINT32 u4Resolution = PANEL_GetPanelWidth();

    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "Panel Width = %d\n", u4Resolution);
    switch (u4Resolution)
    {
    case CUSTOM_PANEL_WIDTH_FHD:
        u4CurrentPanelType = CUSTOM_GPIO_PANEL_FHD;
        break;
    case CUSTOM_PANEL_WIDTH_WXGA:
        u4CurrentPanelType = CUSTOM_GPIO_PANEL_WXGA;
        break;
    default:
        u4CurrentPanelType = CUSTOM_GPIO_PANEL_OTHER;
        break;
    }

    // If the resolution of the current panel setting is identical to 
    // the definition of PCB strap pin, then use the current setting
    if (u4CurrentPanelType == _u4gPanelType)
    {
#ifdef WAFFLE_PCB_EVT_CHM
        UINT32 u4CurrentOutputBit, u4ControlWord;
        switch (_u4gPanelType)
        {
        case CUSTOM_GPIO_PANEL_FHD:
            // Check the LVDS output bit if it is a FHD panel 
            u4ControlWord = PANEL_GetControlWord();
            if (u4ControlWord & DISP_30BIT)
            {
                u4CurrentOutputBit = CUSTOM_GPIO_LVDS_10BIT;
            }
            else if (u4ControlWord & DISP_24BIT)
            {
                u4CurrentOutputBit = CUSTOM_GPIO_LVDS_8BIT;
            }
            else
            {
                u4CurrentOutputBit = CUSTOM_GPIO_LVDS_OTHER_BIT;
            }
            
            PRINT_TIMESTAMP_DBG_MSG("[Power On]", "Current Output Bit = %d\n",
                                    u4CurrentOutputBit);
            if (u4CurrentOutputBit == _u4gLVDSType)
            {
                return;
            }
            break;
            
        case CUSTOM_GPIO_PANEL_WXGA:
        default:
            return;
        }
#else
        return;
#endif /* WAFFLE_PCB_EVT_CHM */
    }

    PRINT_TIMESTAMP_MSG("[Power On]", "Panel ID mismatch\n");
    
#ifdef WAFFLE_PCB_EVT_CHM
    u4NewPanelIndex = DEFAULT_PANEL_WXGA_SELECT;
    if (_u4gPanelType == CUSTOM_GPIO_PANEL_FHD)
    {
        u4NewPanelIndex = (_u4gLVDSType == CUSTOM_GPIO_LVDS_10BIT) ? 
                      DEFAULT_PANEL_FHD_10BIT_SELECT : DEFAULT_PANEL_FHD_SELECT;            
    }
#else
    u4NewPanelIndex = (_u4gPanelType == CUSTOM_GPIO_PANEL_FHD) ? 
                      DEFAULT_PANEL_FHD_SELECT : DEFAULT_PANEL_WXGA_SELECT;
#endif /* WAFFLE_PCB_EVT_CHM */

    WritePanelIndexToEeprom(u4NewPanelIndex);
    x_thread_delay(100);
    CHIP_Reboot();
}

void Custom60HzPowerOnSequence(void)
{
#ifndef CC_MTK_LOADER
    HANDLE_T hLVDSOutputTimer = NULL;

    // Create a timer to enable LVDS output.
    UNUSED(x_timer_create(&hLVDSOutputTimer));
#endif /* CC_MTK_LOADER */

#ifdef WAFFLE_PCB_EVT_CHM
    INT32 i4Value = 1;
    UNUSED(GPIO_Output(SIGNAL_X_U_RST, &i4Value));
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "X_U_RST = %d\n", i4Value);
#endif /* WAFFLE_PCB_EVT_CHM */

    // Panel ID sequence
    PRINT_TIMESTAMP_MSG("[Power On]", "PANEL_ID sequence\n");
    if (_bfgCheckPanelID)
    {
        CustomCheckPanelId();
    }
    
    // normal power sequence, lvds power -> lvds signal -> backlight power
    vDrvTurnOnPanel();
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "LVDS_PWR_ON = %d\n",
                            DRVCUST_PanelGet(ePanelPowerOnVal));
#ifndef CC_MTK_LOADER
    UNUSED(x_timer_start(hLVDSOutputTimer, _au2PowerSeqDelay[2],
                         X_TIMER_FLAG_ONCE, CustomLVDSOutputCtrl, NULL));
#endif /* CC_MTK_LOADER */

    x_thread_delay(_au2PowerSeqDelay[2]);
    
    // Send LVDS output
//    PRINT_TIMESTAMP_MSG("[Power On]", "LVDS_OUTPUT (Send LVDS output)\n");
//    vDrvLVDSConfig();
//    vDrvLVDSOn();
    CustomLVDSOutputCtrl(NULL, NULL);

#ifndef CC_MTK_LOADER
    UNUSED(x_timer_stop(hLVDSOutputTimer));
    UNUSED(x_timer_delete(hLVDSOutputTimer));
#endif /* CC_MTK_LOADER */

    x_thread_delay(_au2PowerSeqDelay[6]);

#ifndef CC_MTK_LOADER
    // Read Temp Sensor
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "Temp sensor = %d\n",
                            CustomQueryTemperature());

#endif /* CC_MTK_LOADER */

    // Start dimmer output        
    CustomStartDimmerOutput();
    
    x_thread_delay(_au2PowerSeqDelay[8]);
}

void CustomPowerOnSequence(void)
{
    UNUSED(CustomPowerOnSequence);

    CRIT_STATE_T rCritState;

    INT32 i4Value = 0;

    // Set panel format select pin to be low
    UNUSED(GPIO_Output(SIGNAL_LVDS_SEL, &i4Value));
    
    CustomGetPowerSequenceDelay();
    
    vDrvLVDSOff();

    Custom60HzPowerOnSequence();

    x_thread_delay(_au2PowerSeqDelay[9]);
    
    // Turn on backlight
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "BL_ON = %d\n",
                            DRVCUST_PanelGet(eBackLightOnVal));
    vDrvTurnOnBacklt();
    CustomUpdateBacklightState(TRUE);

    // Read RGB sensor
    PRINT_TIMESTAMP_MSG("[Power On]", "RGB sensor = (none)\n");

    // Start Self Diagnostic
    PRINT_TIMESTAMP_MSG("[Power On]", "Start Self Diagnostic\n");
    SELF_DIAG_SET_MODEL_TYPE(_u4gModelType);
    
#if defined (CC_SELF_DIAG_DEFAULT_ON)
    SELF_DIAG_START(D_SYSTEM_PANEL_ONOFF);
#endif

    // Update the power sequence status to be "done"
    rCritState = x_crit_start();
    CustomSetPowerSeqStatus(TRUE);
    x_crit_end(rCritState);
    
    IRRC_LedBlinkConfig(TRUE);
}

void CustomPowerOffSequence(void)
{
    UNUSED(CustomPowerOffSequence);
    
    CRIT_STATE_T rCritState;

    IRRC_LedBlinkConfig(FALSE);
    CustomUpdateBacklightState(FALSE);
    
    // Update the power sequence status to be "not finished"
    rCritState = x_crit_start();
    CustomSetPowerSeqStatus(FALSE);
    x_crit_end(rCritState);

    // Move to customer setup power down function    
#if 0    
    // Stop Self Diagnostic
    PRINT_TIMESTAMP_MSG("[Power Off]", "Stop Self Diagnostic\n");
    SELF_DIAG_STOP(D_SYSTEM_PANEL_ONOFF);

    // Stop AC detection
    PRINT_TIMESTAMP_MSG("[Power Off]", "Stop AC_DET\n");
    CustomStopACDetect();
#endif    

    // Pause Self Diagnostic
    PRINT_TIMESTAMP_MSG("[Power Off]", "Pause Self Diagnostic\n");
    SELF_DIAG_PAUSE();

    // backlight power -> lvds sugnal -> lvds power
    PRINT_TIMESTAMP_DBG_MSG("[Power Off]", "BL_ON = %d\n",
                            (INT32)!DRVCUST_PanelGet(eBackLightOnVal));
    vDrvTurnOffBacklt(); // Backlight off
    x_thread_delay(_au2PowerSeqDelay[13]);

    PRINT_TIMESTAMP_MSG("[Power Off]", "LVDS_OUTPUT = 0\n");
    vDrvLVDSOff();

    // Clear the LVDS output status
    rCritState = x_crit_start();
    _bfgLVDSOutput = FALSE;
    x_crit_end(rCritState);

    x_thread_delay(_au2PowerSeqDelay[10]);

    // Panel power power can not turn off right now
    // It will hang SIF bus if the panel power is down.
    PRINT_TIMESTAMP_DBG_MSG("[Power Off]", "LVDS_PWR_ON = %d\n",
                            (INT32)!DRVCUST_PanelGet(ePanelPowerOnVal));
    vDrvTurnOffPanel(); // Panel off

#if 0   // Move to GPIO_CustomPowerOffFunc   
#ifdef WAFFLE_PCB_EVT_CHM
    INT32 i4Value = 0;
    UNUSED(GPIO_Output(SIGNAL_X_U_RST, &i4Value));
    PRINT_TIMESTAMP_DBG_MSG("[Power Off]", "X_U_RST = %d\n", i4Value);
#endif /* WAFFLE_PCB_EVT_CHM */
#endif
}

#ifdef CC_AUD_SETUP_MELODY 
static void _StartLogoThread(void *pvArgs)
{
    //Unmute Audio
    ADAC_Enable(TRUE);
    ADAC_Mute(FALSE);
    ADAC_CodecMute(FALSE);

    x_thread_delay(50); // For PWM first power on pop noise.
    ADAC_GpioCodecMute(FALSE);
    ADAC_SpeakerEnable(TRUE);

    //Play Setup Melody
    AUD_PlaySetupMelody(AUD_DEC_MAIN, 3, 1);
}
#endif

void CustomDisplayLogo(void)
{
    DTVCFG_T rDtvCfg;
    
#ifdef CC_AUD_SETUP_MELODY 
    HANDLE_T hThread;
#endif

#ifdef CUSTOM_POWER_STATE_CTRL_ENABLE
    if (CustomQueryPowerState() != STATE_POWER_ON)
    {
        return;
    }
#endif /* CUSTOM_POWER_STATE_CTRL_ENABLE */
    
    if (EEPDTV_GetCfg(&rDtvCfg) == 0)
    {
        BOOL bIsShowLogo = (rDtvCfg.u1Flags2 & DTVCFG_FLAG2_DISABLE_LOGO) ?
                           FALSE : TRUE;
        if (bIsShowLogo)
        {
            BOOL fgFinished = FALSE;
            UNUSED(d_custom_misc_set(DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS,
                   &fgFinished, sizeof(fgFinished), FALSE));
            
            vApiPanelPowerSequence(TRUE);
	        DRVCUST_AdaptiveBacklightMode(2);
            
    #ifdef CC_AUD_SETUP_MELODY
            if (bApiEepromReadByte(EEP_BOOT_MUSIC_ONOFF) & AUD_POWER_ON_MUSIC_MASK)
            {
                x_thread_create(&hThread, LOGO_THREAD_NAME, LOGO_THREAD_STACK_SIZE,
                                LOGO_THREAD_PRIORITY, _StartLogoThread, 0, NULL);
            }
    #endif
            
            OSD_ShowLogo();
            DRVCUST_AdaptiveBacklightMode(0);
            
            fgFinished = TRUE;
            UNUSED(d_custom_misc_set(DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS,
                                     &fgFinished, sizeof(fgFinished), FALSE));
        }                
    }
}


