
//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_timer.h"
#include "x_assert.h"
#include "x_debug.h"
#include "x_gpio.h"
#include "x_pdwnc.h"
#include "x_util.h"

#include "ir_if.h"
#include "panel.h"
#include "eeprom_if.h"
#include "PowerSeq.h"
#include "CustomCtrl.h"
#include "Figaro_pdwnc.h"
#include "hw_hdmi.h"
//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
void GPIO_CustomPowerOnFunc(void);
void GPIO_CustomPowerOffFunc(UINT32 u4PowerDownEn);
void GPIO_CustomLedCtrlFunc(LED_STATUS_T eLedStatus);
UINT32 CustomPCBAQueryFunc(VOID);
UINT32 CustomPanelQueyFunc(VOID);
BOOL ICustomPCBARGBQueryFunc(VOID);

extern BOOL PDWNC_Check_Enter_SemiStandy(void);
extern void DRVCUST_AutoBacklightDisable(void);

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
UINT32 _u4gPanelType = CUSTOM_GPIO_PANEL_WXGA;
UINT32 _u4gPCBType = CUSTOM_PCB_TYPE_1_WXGA;
UINT32 _u4gFactoryType = CUSTOM_GPIO_FACTORY_WISTRON;
UINT32 _u4gLVDSType = CUSTOM_GPIO_LVDS_8BIT;
UINT32 _u4gModelType = 0;

BOOL   _bgIsCMO37Panel = FALSE;
BOOL   _bgIsThermalDetect = TRUE;
BOOL   _bgIsLightDetect = TRUE;
    
//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
UINT32 CustomPCBAQueryCountry(VOID)
{
    UINT8  u1Val = 0;
    UINT64 u8Offset = (UINT64)CUSTOM_COUNTRY_EXT_ADDR;
    static BOOL bInit = TRUE;
    static UINT32 u4Country = DRV_CUSTOM_COUNTRY_EU;

    if (!bInit)
    {
        return u4Country;
    }

    if (EEPROM_Read(u8Offset, (UINT32)(void *)&u1Val, 1) == 0)
    {
    	switch (u1Val)
    	{
    	case DRV_CUSTOM_COUNTRY_OPTION_EU:
    	    u4Country= DRV_CUSTOM_COUNTRY_EU;
    	    break;
    	case DRV_CUSTOM_COUNTRY_OPTION_PAD:
    	    u4Country= DRV_CUSTOM_COUNTRY_PAD;
    	    break;
    	case DRV_CUSTOM_COUNTRY_OPTION_PAA:
    	    u4Country= DRV_CUSTOM_COUNTRY_PAA;
    	    break;
    	case DRV_CUSTOM_COUNTRY_OPTION_COL:
    	    u4Country= DRV_CUSTOM_COUNTRY_COL;
    	    break;
    	}
    }
    
	return u4Country;
}

UINT32 CustomPCBAQueryFunc(VOID)
{
    UNUSED(CustomPCBAQueryFunc);
    
    switch (_u4gPCBType)
    {
    case CUSTOM_PCB_TYPE_1_WXGA:
    case CUSTOM_PCB_TYPE_1_FHD:
        return 1;
    case CUSTOM_PCB_TYPE_2:
        return 2;
    case CUSTOM_PCB_TYPE_3:
        return 3;
    case CUSTOM_PCB_TYPE_5:
        return 5;
    }
    
    /*should not reach here*/
    return 1;
}

/* return CUSTOM_GPIO_PANEL_WXGA / CUSTOM_GPIO_PANEL_FHD */
UINT32 CustomPanelQueyFunc(VOID)
{
    UNUSED(CustomPanelQueyFunc);
    
    return _u4gPanelType;
}

BOOL CustomPCBARGBQueryFunc(VOID)
{
    UNUSED(CustomPCBARGBQueryFunc);
    return FALSE;
}

UINT32 CustomModelQueryFunc(VOID)
{
    UNUSED(CustomModelQueryFunc);
    
    return _u4gModelType;
}

UINT32 CustomModelOptionQueryFunc(VOID)
{
    UNUSED(CustomModelOptionQueryFunc);
    
    UINT64 u8Offset;
    static BOOL bInit = TRUE;
    static UINT8 u1Index = 0;
    
    if (bInit)
    {
        u8Offset = (UINT64)CUSTOM_MODEL_EXT_ADDR;
        
        if (EEPROM_Read(u8Offset, (UINT32)(void *)&u1Index, 1) == 0)
        {
            /* Model Type:
                0: E,   1: E',  2: L,   3: P,   4: C,   5: F1,  6: F1S
            */
	     if (u1Index > 6)
            {
                 u1Index = 0;
            }
        }
        
        bInit = FALSE;
    }
    
	return u1Index;
}

void GPIO_CustomReadStrapPin(void)
{
    UINT32 u4Val = 0;
    // Set gpio 15 as the input pin
    UNUSED(GPIO_Input(CUSTOM_GPIO_PANEL_NVM_WP));
    
    _u4gPanelType = (UINT32)GPIO_Input(CUSTOM_GPIO_PANEL_ID_PIN_0);
    _u4gPCBType = (_u4gPanelType == CUSTOM_GPIO_PANEL_WXGA) ? 
        DRV_CUSTOM_MODEL_WXGA_PANEL : DRV_CUSTOM_MODEL_FHD_PANEL;
    
    _u4gModelType |= (_u4gFactoryType == CUSTOM_GPIO_FACTORY_WISTRON) ?
        DRV_CUSTOM_MODEL_FACTORY_WISTRON : DRV_CUSTOM_MODEL_FACTORY_SONY;

    _u4gModelType |= (_u4gPanelType == CUSTOM_GPIO_PANEL_WXGA) ?
        DRV_CUSTOM_MODEL_WXGA_PANEL : DRV_CUSTOM_MODEL_FHD_PANEL;

	u4Val = (UINT32)GPIO_Input(CUSTOM_GPIO_1H2H_DETCTION_PIN);
	_u4gModelType |= (u4Val == CUSTOM_PCB_TYPE_2H) ?
		DRV_CUSTOM_MODEL_2H : DRV_CUSTOM_MODEL_1H;

#ifdef WAFFLE_PCB_EVT_CHM

#ifndef WAFFLE_PCB_LED
    u4Val = (UINT32)GPIO_Input(CUSTOM_GPIO_PANEL_ID_PIN_1);
    _u4gModelType |= (u4Val == CUSTOM_GPIO_DVD_INCLUDE) ?
        DRV_CUSTOM_MODEL_WITH_DVD : DRV_CUSTOM_MODEL_NO_DVD;

    u4Val = (UINT32)GPIO_Input(CUSTOM_GPIO_WOOFER_PIN);
    _u4gModelType |= (u4Val == CUSTOM_GPIO_AUD_WOOFER) ?
        DRV_CUSTOM_MODEL_AUD_WOOFER : DRV_CUSTOM_MODEL_AUD_NO_WOOFER;
    
    if (_u4gPanelType == CUSTOM_GPIO_PANEL_FHD)
    {
        u4Val = (UINT32)GPIO_Input(CUSTOM_GPIO_CMO37_DET_PIN);
        _bgIsCMO37Panel = (u4Val == CUSTOM_GPIO_FHD_CMO_37) ? TRUE : FALSE;
        Printf("[GPIO_CustomReadStrapPin] _bgIsCMO37Panel = %2x\n", _bgIsCMO37Panel);
        
        _u4gLVDSType = (UINT32)GPIO_Input(CUSTOM_GPIO_PANLE_8_10_BIT_PIN);
        Printf("[GPIO_CustomReadStrapPin] _u4gLVDSType = %2x\n", _u4gLVDSType);
    }
#endif /* WAFFLE_PCB_LED */
#endif /* WAFFLE_PCB_EVT_CHM */

    // Determin Light sensor enable/disable by Option bit
    UINT64 u8Offset = (UINT64)CUSTOM_LIGHT_SENSOR_OPTION_ADDR;
    UINT8 u1Val = 0;
    
    if (EEPROM_Read(u8Offset, (UINT32)(void *)&u1Val, 1) == 0)
    {
        if (u1Val == CUSTOM_OPTION_LIGHT_SENSOR_OFF)
        {
#ifndef CC_MTK_LOADER
            DRVCUST_AutoBacklightDisable();
#endif /* CC_MTK_LOADER */
        }
    }
    
#ifdef WAFFLE_PCB_DVT

#if 0   // Disable Light sensor detect pin
    u4Val = (UINT32)GPIO_Input(CUSTOM_GPIO_LIGHT_SENSOR_PIN);
    if (u4Val == CUSTOM_GPIO_LIGHT_SENSOR_NA)
    {
#ifndef CC_MTK_LOADER
        DRVCUST_AutoBacklightDisable();
#endif /* CC_MTK_LOADER */
    }
#endif

    u4Val = (UINT32)GPIO_Input(CUSTOM_GPIO_THERMAL_SENSOR_PIN);
    _bgIsThermalDetect = (u4Val == CUSTOM_GPIO_THERMAL_SENSOR_INCLUDE) ? 
                         TRUE : FALSE;
#endif /* WAFFLE_PCB_DVT */

    Printf("[GPIO_CustomReadStrapPin] _u4gModelType = 0x%2x\n", _u4gModelType);

    UINT8 au1Index[] = {PANEL_INVALID, PANEL_INVALID, PANEL_INVALID};
    QUERY_TYPE_T arKey[] = {ePanelIndexOffset1, ePanelIndexOffset2,
                            ePanelIndexOffset3};
    UINT32 u4Size = sizeof(arKey) / sizeof(QUERY_TYPE_T);
    UINT32 u4Value, i;

    if ((DRVCUST_PanelQuery(ePanelIndexFromEeprom, &u4Value) == 0) &&
        (u4Value == 1))
    {
        // read configuration from eeprom
        for (i = 0; i < u4Size; i++)
        {
            if (DRVCUST_PanelQuery(arKey[i], &u4Value) == 0)
            {
                UNUSED(EEPROM_Read((UINT64)u4Value, (UINT32)&au1Index[i], 1));
            }
        }

        // Write the default panel index to EEPROM
        if ((au1Index[0] == PANEL_INVALID) && (au1Index[1] == PANEL_INVALID) &&
            (au1Index[2] == PANEL_INVALID))
        {
#ifdef WAFFLE_PCB_EVT_CHM
            UINT32 u4DefaultPanel = 4;//DEFAULT_PANEL_WXGA_SELECT;
            if (_u4gPanelType == CUSTOM_GPIO_PANEL_FHD)
            {
                u4DefaultPanel = (_u4gLVDSType == CUSTOM_GPIO_LVDS_10BIT) ? 
                      /*DEFAULT_PANEL_FHD_10BIT_SELECT*/4 : 7 /*DEFAULT_PANEL_FHD_SELECT*/;            
            }
#else
            UINT32 u4DefaultPanel = (_u4gPanelType == CUSTOM_GPIO_PANEL_FHD) ? 
                      /*DEFAULT_PANEL_FHD_SELECT*/4 : 7/*DEFAULT_PANEL_WXGA_SELECT*/;
#endif /* WAFFLE_PCB_EVT_CHM */

            WritePanelIndexToEeprom(u4DefaultPanel);
        }
    }
}

void GPIO_CustomPowerOnFunc(void)
{
    UNUSED(GPIO_CustomPowerOnFunc);
    
#ifdef CC_MTK_LOADER
    UINT32 i;
    UINT32 polarity;

#if 0    
	vRegWrite1B(PD_SYS_2, 0xff);
	vRegWriteFldAlign(ANA_INTF_1, 0x1, RG_HDMI_TERM_EN);	
	vRegWriteFldAlign(ANA_INTF_2, 0x1, RG_HDMI_SW_EN);
#endif
			
    // Audio mute circuit mute in loader
    if (DRVCUST_InitQuery(eAudioMuteGpioNum, &i) == 0)
    {
        UNUSED(DRVCUST_InitQuery(eAudioMuteGpioPolarity, &polarity));
        GPIO_SetOut(i, polarity);
    }

    if (!PDWNC_Check_Enter_SemiStandy())
    {
        GPIO_CustomLedCtrlFunc(eLedLoaderPowerOn);
    }

#else /* CC_MTK_LOADER */

    GPIO_CustomReadStrapPin();

    if (PDWNC_Check_Enter_SemiStandy())
    {
        IRRC_LedBlinkConfig(FALSE);
    }
    CustomIRLedHandleStart();
    
#ifdef DRV_3RD_PARTY_CUSTOM_INIT    /* 3rd Pary driver init routine */
    DRV_3RD_PARTY_CUSTOM_INIT();
#endif

#ifdef CC_AC_DETECT_ENABLE
    // Start AC detection
    HAL_TIME_T rTime;
    HAL_GetTime(&rTime);
    Printf("[Power On]");
    Printf(" (%06d:%03d) ", rTime.u4Seconds, (rTime.u4Micros / 1000));
    Printf("Start AC_DET\n");      
    CustomStartACDetect();
#endif /* CC_AC_DETECT_ENABLE */
    
#endif /* CC_MTK_LOADER */
}

void GPIO_CustomPowerOffFunc(UINT32 u4PowerDownEn)
{
    UINT32 i;
    INT32 i4Val;
    
    UNUSED(GPIO_CustomPowerOffFunc);

#ifndef CC_MTK_LOADER
    CustomIRLedHandleStop();
#endif /* CC_MTK_LOADER */

    BOOL bOnTimer = (u4PowerDownEn & WAK_RTC) ? TRUE : FALSE;
    CustomSetTimerLED(bOnTimer);

    if (u4PowerDownEn & WAK_VGA)
    {
        GPIO_CustomLedCtrlFunc(eLedVgaStandby);
    }
    else
    {
#ifdef CC_MTK_LOADER
        GPIO_CustomLedCtrlFunc(eLedLoaderPowerOff);
#else /* CC_MTK_LOADER */
        GPIO_CustomLedCtrlFunc(eLedSystemPowerOff);
#endif /* CC_MTK_LOADER */
    }

#ifdef WAFFLE_PCB_EVT_CHM
    INT32 i4Value = 0;
    UNUSED(GPIO_Output(SIGNAL_X_U_RST, &i4Value));
    PRINT_TIMESTAMP_DBG_MSG("[Power Off]", "X_U_RST = %d\n", i4Value);
#endif /* WAFFLE_PCB_EVT_CHM */

#ifndef CC_MTK_LOADER
    // Stop Self Diagnostic
    PRINT_TIMESTAMP_MSG("[Power Off]", "Stop Self Diagnostic\n");
    SELF_DIAG_STOP(D_SYSTEM_PANEL_ONOFF);

    // Stop AC detection
    PRINT_TIMESTAMP_MSG("[Power Off]", "Stop AC_DET\n");
    CustomStopACDetect();
#endif /* CC_MTK_LOADER */

    DOWN_SET_GPIO(eBackLightGpio, eBackLightOnVal);
    DOWN_SET_GPIO(ePanelPowerGpio, ePanelPowerOnVal);
    DOWN_SET_GPIO(ePowerCtrlGpio, ePowerCtrlOnVal);
}

