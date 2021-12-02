/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: panel_table.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file panel_tablc.c
 *  This file includes panel timing configuration.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#ifndef CC_5391_PRELOADER

#include "panel.h"
#if 0
#include "eeprom_if.h"
#include "sif_if.h"
#endif
#include "drvcust_if.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_printf.h"
#include "x_pinmux.h"
#include "x_hal_5381.h"
#include "x_assert.h"
LINT_EXT_HEADER_END


#ifdef CUSTOM_CFG_FILE
#include CUSTOM_CFG_FILE
#endif /* CUSTOM_CFG_FILE */

#include "drv_default.h"


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef struct
{
    UINT16 u2PanelWidth;
    UINT16 u2PanelHeight;

    // Clk60Hz = htotal*vtotal*60/1.001
    // Clk50Hz = htotal*vtotal*50
    UINT32 u4PixelClkMax;
    UINT32 u4PixelClk60Hz;
    UINT32 u4PixelClk50Hz;
    UINT32 u4PixelClkMin;

    // HTotal = spec value - 1
    UINT16 u2HTotalMax;
    UINT16 u2HTotal60Hz;
    UINT16 u2HTotal50Hz;
    UINT16 u2HTotalMin;

    // VTotal = spec value - 1
    UINT16 u2VTotalMax;
    UINT16 u2VTotal60Hz;
    UINT16 u2VTotal50Hz;
    UINT16 u2VTotalMin;
    UINT8 u1VClkMax;
    UINT8 u1VClkMin;

    UINT8 u1HSyncWidth;
    UINT8 u1VSyncWidth;
    UINT8 u1HPosition;
    UINT8 u1VPosition;
    UINT16 u2ControlWord;

    // backlight ragne
    UINT8 u1BacklightHigh;
    UINT8 u1BacklightMiddle;
    UINT8 u1BacklightLow;
    UINT8 u1Reserved;

    UINT16 u2DimmingFrequency60Hz;
    UINT16 u2DimmingFrequency50Hz;

    // delay unit: 10ms
    UINT8 u1LvdsOnDalay;
    UINT8 u1BacklightOnDelay;
    UINT8 u1BacklightOffDelay;
    UINT8 u1LvdsOffDalay;

    UINT16 u2MaxOverscan;
    UINT16 u2Reserved;
} PANEL_ATTRIBUTE_T;


typedef struct
{
    UINT16 u2PanelWidth;
    UINT16 u2PanelHeight;

    UINT32 u4PixelClkMax;
    UINT32 u4PixelClk60Hz;
    UINT32 u4PixelClk50Hz;
    UINT32 u4PixelClkMin;

    UINT32 u4Reserved1;

    UINT16 u2HTotalMax;
    UINT16 u2HTotal60Hz;
    UINT16 u2HTotal50Hz;
    UINT16 u2HTotalMin;

    UINT16 u2Reserved2;
    UINT16 u2VTotalMax;
    UINT16 u2VTotal60Hz;
    UINT16 u2VTotal50Hz;
    UINT16 u2VTotalMin;
    UINT16 u2Reserved3;

    UINT32 u4Reserved4;

    UINT16 u2Reserved5;
    UINT8 u1VClkMax;
    UINT8 u1VClkMin;

    UINT16 u2Reserved6;
    UINT8 u1HSyncWidth;
    UINT8 u1VSyncWidth;

    UINT8 u1HPosition;
    UINT8 u1VPosition;
    UINT16 u2Reserved7;

    UINT16 u4Reserved8;
    UINT8 u1LvdsMapping;
    UINT8 u1MaxOverscan1;

    UINT8 u1MaxOverscan2;
    UINT8 u1ControlWord1;
    UINT8 u1ControlWord2;
    UINT8 u1PanelOnTime1;

    UINT8 u1LvdsOnDalay;

    UINT8 u1BacklightOnDelay;
    UINT8 u1PanelOnTime4;
    UINT8 u1Reserved8;

    UINT8 u1PanelOffTime1;
    UINT8 u1BacklightOffDelay;
    UINT8 u1LvdsOffDalay;
    UINT8 u1PanelOffTime4;
} PANEL_FLASH_ATTRIBUTE_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/*lint -save -e960 */
#define PANEL_ATTRIBUTE_SET_TMPL(NAME, FIELD) \
    void PANEL_Set##NAME(UINT32 u4Value) \
    { \
        _prPanelAttribute->FIELD = u4Value; \
    } \

#define PANEL_ATTRIBUTE_GET_TMPL(NAME, FIELD) \
    UINT32 PANEL_Get##NAME(void) \
    { \
        return _prPanelAttribute->FIELD; \
    } \

#define PANEL_ATTRIBUTE_SETGET_TMPL(NAME, FIELD) \
    PANEL_ATTRIBUTE_SET_TMPL(NAME, FIELD) \
    PANEL_ATTRIBUTE_GET_TMPL(NAME, FIELD) \

/* above line is intendedly left blank */
/*lint -restore */

#define IS_PANEL_INDEX_INVALID(X) ((X) >= u4TotalNumber)
#define CHECK_ZERO(X) \
    { \
        if ((X) == 0) \
        { \
            Printf( "Line %d: is zero\r\n", __LINE__ ); \
            return FALSE; \
        } \
    }

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifndef CC_MULTI_PANEL
// we build all panel timing in default
#define CC_MULTI_PANEL 1
#endif

// this is the panel_table_cust.c in each customer's directory
#include PANEL_TABLE_CUST_FILE

#if CC_MULTI_PANEL
static PANEL_ATTRIBUTE_T* _prPanelAttribute =
                          &_arPanelAttribute[DEFAULT_PANEL_SELECT];
static UINT32 _u4PanelIndex = DEFAULT_PANEL_SELECT;
static UINT32 _u4EepromPanelIndex = DEFAULT_PANEL_SELECT;
#else
static PANEL_ATTRIBUTE_T* _prPanelAttribute = &_arPanelAttribute[0];
static UINT32 _u4PanelIndex = 0;
static UINT32 _u4EepromPanelIndex = 0;
#endif
static UINT32 u4TotalNumber = PANEL_TOTAL_NUMBER;
static INT32 _i4HPixelShift = 0;
static INT32 _i4VPixelShift = 0;
static BOOL _fgPixelShiftEnable = FALSE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static BOOL _PanelLoadTableFromFlash(UINT32 u4Offset)
{
    PANEL_FLASH_ATTRIBUTE_T rFlashTable;

    Printf("Load panel table from flash, address=0x%x\n", u4Offset);
    x_memcpy((void*)&rFlashTable, (void*)u4Offset, sizeof(PANEL_FLASH_ATTRIBUTE_T));

    _arPanelAttribute[0].u2PanelWidth = rFlashTable.u2PanelWidth;
    CHECK_ZERO(_arPanelAttribute[0].u2PanelWidth);
    _arPanelAttribute[0].u2PanelHeight = rFlashTable.u2PanelHeight;
    CHECK_ZERO(_arPanelAttribute[0].u2PanelHeight);
    _arPanelAttribute[0].u4PixelClkMax = rFlashTable.u4PixelClkMax;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClkMax);
    _arPanelAttribute[0].u4PixelClk60Hz = rFlashTable.u4PixelClk60Hz;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClk60Hz);
    _arPanelAttribute[0].u4PixelClk50Hz = rFlashTable.u4PixelClk50Hz;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClk50Hz);
    _arPanelAttribute[0].u4PixelClkMin = rFlashTable.u4PixelClkMin;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClkMin);

    _arPanelAttribute[0].u2HTotalMax = rFlashTable.u2HTotalMax;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotalMax);
    _arPanelAttribute[0].u2HTotal60Hz = rFlashTable.u2HTotal60Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotal60Hz);   
    _arPanelAttribute[0].u2HTotal50Hz = rFlashTable.u2HTotal50Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotal50Hz);
    _arPanelAttribute[0].u2HTotalMin = rFlashTable.u2HTotalMin;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotalMin);

    _arPanelAttribute[0].u2VTotalMax = rFlashTable.u2VTotalMax;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotalMax);
    _arPanelAttribute[0].u2VTotal60Hz = rFlashTable.u2VTotal60Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotal60Hz);
    _arPanelAttribute[0].u2VTotal50Hz = rFlashTable.u2VTotal50Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotal50Hz);
    _arPanelAttribute[0].u2VTotalMin = rFlashTable.u2VTotalMin;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotalMin);

    _arPanelAttribute[0].u1VClkMax = rFlashTable.u1VClkMax;
    CHECK_ZERO(_arPanelAttribute[0].u1VClkMax);
    _arPanelAttribute[0].u1VClkMin = rFlashTable.u1VClkMin;
    CHECK_ZERO(_arPanelAttribute[0].u1VClkMin);

    _arPanelAttribute[0].u1HSyncWidth = rFlashTable.u1HSyncWidth;
    CHECK_ZERO(_arPanelAttribute[0].u1HSyncWidth);
    _arPanelAttribute[0].u1VSyncWidth = rFlashTable.u1VSyncWidth;
    CHECK_ZERO(_arPanelAttribute[0].u1VSyncWidth);
    _arPanelAttribute[0].u1HPosition = rFlashTable.u1HPosition;
    CHECK_ZERO(_arPanelAttribute[0].u1HPosition);
    _arPanelAttribute[0].u1VPosition = rFlashTable.u1VPosition;
    CHECK_ZERO(_arPanelAttribute[0].u1VPosition);
    _arPanelAttribute[0].u2MaxOverscan = rFlashTable.u1MaxOverscan1;
    _arPanelAttribute[0].u2MaxOverscan |= rFlashTable.u1MaxOverscan2 << 8;
    _arPanelAttribute[0].u2ControlWord = rFlashTable.u1ControlWord1;
    _arPanelAttribute[0].u2ControlWord |= rFlashTable.u1ControlWord2 << 8;

    _arPanelAttribute[0].u1LvdsOnDalay = rFlashTable.u1LvdsOnDalay;
    _arPanelAttribute[0].u1BacklightOnDelay = rFlashTable.u1BacklightOnDelay;
    _arPanelAttribute[0].u1BacklightOffDelay = rFlashTable.u1BacklightOffDelay;
    _arPanelAttribute[0].u1LvdsOffDalay = rFlashTable.u1LvdsOffDalay;

#ifdef THE_3RD_PARTY_SW_SUPPORT
    _arPanelAttribute[0].u2DimmingFrequency60Hz = THE_3RD_PARTY_GET_BL_EX(1);
    _arPanelAttribute[0].u2DimmingFrequency50Hz = THE_3RD_PARTY_GET_BL_EX(0);
#endif

    SelectPanel(0);
    
    return TRUE;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

PANEL_ATTRIBUTE_SET_TMPL(PanelWidth, u2PanelWidth)
PANEL_ATTRIBUTE_SET_TMPL(PanelHeight, u2PanelHeight)
PANEL_ATTRIBUTE_SETGET_TMPL(PixelClkMax, u4PixelClkMax)
PANEL_ATTRIBUTE_SETGET_TMPL(PixelClk60Hz, u4PixelClk60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(PixelClk50Hz, u4PixelClk50Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(PixelClkMin, u4PixelClkMin)
PANEL_ATTRIBUTE_SETGET_TMPL(HTotalMax, u2HTotalMax)
PANEL_ATTRIBUTE_SETGET_TMPL(HTotal60Hz, u2HTotal60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(HTotal50Hz, u2HTotal50Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(HTotalMin, u2HTotalMin)
PANEL_ATTRIBUTE_SETGET_TMPL(VTotalMax, u2VTotalMax)
PANEL_ATTRIBUTE_SETGET_TMPL(VTotal60Hz, u2VTotal60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(VTotal50Hz, u2VTotal50Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(VTotalMin, u2VTotalMin)
PANEL_ATTRIBUTE_SETGET_TMPL(VClkMax, u1VClkMax)
PANEL_ATTRIBUTE_SETGET_TMPL(VClkMin, u1VClkMin)
PANEL_ATTRIBUTE_SETGET_TMPL(HSyncWidth, u1HSyncWidth)
PANEL_ATTRIBUTE_SETGET_TMPL(VSyncWidth, u1VSyncWidth)
PANEL_ATTRIBUTE_SETGET_TMPL(HPosition60Hz, u1HPosition)
PANEL_ATTRIBUTE_SET_TMPL(VPosition, u1VPosition)
PANEL_ATTRIBUTE_SETGET_TMPL(ControlWord, u2ControlWord)
PANEL_ATTRIBUTE_SETGET_TMPL(DimmingFrequency60Hz, u2DimmingFrequency60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(DimmingFrequency50Hz, u2DimmingFrequency50Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(LvdsOnDalay, u1LvdsOnDalay)
PANEL_ATTRIBUTE_SETGET_TMPL(BacklightOnDelay, u1BacklightOnDelay)
PANEL_ATTRIBUTE_SETGET_TMPL(BacklightOffDelay, u1BacklightOffDelay)
PANEL_ATTRIBUTE_SETGET_TMPL(LvdsOffDalay, u1LvdsOffDalay)
PANEL_ATTRIBUTE_SETGET_TMPL(MaxOverscan, u2MaxOverscan)


//----------------------------------------------------------------------------
/** DRVCUST_PanelQuery() return customization paramter.
 */
//----------------------------------------------------------------------------
INT32 DRVCUST_PanelQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data)
{
    ASSERT(pu4Data!=NULL);

    switch(eQryType)
    {
    // Panel
    case eDefaultPanelSelect:
        *pu4Data = DEFAULT_PANEL_SELECT;
        return 0;
    case eBackLightGpio:
        *pu4Data = BACKLIGHT_GPIO;
        return 0;
    case eBackLightOnVal:
        *pu4Data = BACKLIGHT_ON_VAL;
        return 0;
    case ePanelPowerGpio:
        *pu4Data = PANELPOWER_GPIO;
        return 0;
    case ePanelPowerOnVal:
        *pu4Data = PANELPOWER_ON_VAL;
        return 0;
    case ePanelDCRGpio:
#ifdef PANEL_DCR_GPIO
        *pu4Data = PANEL_DCR_GPIO;
        return 0;
#else
        return -1;
#endif /* PANEL_DCR_GPIO */

    case ePanelDCROnVal:
#ifdef PANEL_DCR_ON_VAL
        *pu4Data = PANEL_DCR_ON_VAL;
        return 0;
#else
        return -1;
#endif /* PANEL_DCR_ON_VAL */

    case ePanelPdpVsGpio:
#ifdef PANEL_PDP_VS_GPIO
        *pu4Data = PANEL_PDP_VS_GPIO;
        return 0;
#else
        return -1;
#endif /* PANEL_PDP_VS_GPIO */

    case ePanelPdpVsOnVal:
#ifdef PANEL_PDP_VS_ON_VAL
        *pu4Data = PANEL_PDP_VS_ON_VAL;
        return 0;
#else
        return -1;
#endif /* PANEL_PDP_VS_ON_VAL */

    case eLvdsMsbSw:
#ifdef LVDS_MSB_SW
        *pu4Data = LVDS_MSB_SW;
        return 0;
#else
        return -1;
#endif /* LVDS_MSB_SW */

    case ePanelPwmPrescalar:
#ifdef PWM_PRESCALAR
        *pu4Data = PWM_PRESCALAR;
        return 0;
#else
        return -1;
#endif /* PWM_PRESCALAR */

    case ePanelDisableDimming:
#ifdef PANEL_DISABLE_DIMMING
        *pu4Data = PANEL_DISABLE_DIMMING;
        return 0;
#else
        return -1;
#endif /* PANEL_DISABLE_DIMMING */

    case ePanelChannelSwap:
        *pu4Data = PANEL_CHANNEL_SWAP;
        return 0;

    case ePanelPwmPolarity:
#ifdef PWM_POLARITY
        *pu4Data = PWM_POLARITY;
        return 0;
#else
        return -1;
#endif /* PWM_POLARITY */
    case ePanelIndexFromEeprom:
#ifdef PANEL_INDEX_FROM_EEPROM
        *pu4Data = PANEL_INDEX_FROM_EEPROM;
        return 0;
#else
        return -1;
#endif /* PANEL_INDEX_FROM_EEPROM */

    case ePanelIndexOffset1:
#ifdef PANEL_INDEX_OFFSET1
        *pu4Data = PANEL_INDEX_OFFSET1;
        return 0;
#else
        return -1;
#endif /* PANEL_INDEX_OFFSET1 */

    case ePanelIndexOffset2:
#ifdef PANEL_INDEX_OFFSET2
        *pu4Data = PANEL_INDEX_OFFSET2;
        return 0;
#else
        return -1;
#endif /* PANEL_INDEX_OFFSET2 */

    case ePanelIndexOffset3:
#ifdef PANEL_INDEX_OFFSET3
        *pu4Data = PANEL_INDEX_OFFSET3;
        return 0;
#else
        return -1;
#endif /* PANEL_INDEX_OFFSET3 */
    case ePanelIndexFromGpioNum:
        *pu4Data = PANEL_INDEX_FROM_GPIO_NUM;
        return 0;

    case ePanelIndexFromGpioIdx:
        *pu4Data = (UINT32)PANEL_INDEX_FROM_GPIO_IDX;
        return 0;

    case ePanelIndexFromGpioMap:
        *pu4Data = (UINT32)PANEL_INDEX_FROM_GPIO_MAP;
        return 0;

    case ePanelLvds8bitPinMap:
        *pu4Data = (UINT32)PANEL_LVDS_8BIT_PIN_MAP;
        return 0;

    case ePanelLvds10bitPinMap:
        *pu4Data = (UINT32)PANEL_LVDS_10BIT_PIN_MAP;
        return 0;

    case eBacklightHigh:
#ifdef BACKLIGHT_HIGH_PWM
        *pu4Data = BACKLIGHT_HIGH_PWM;
        return 0;
#else
        return -1;
#endif /* BACKLIGHT_HIGH_PWM */

    case eBacklightMiddle:
#ifdef BACKLIGHT_MIDDLE_PWM
        *pu4Data = BACKLIGHT_MIDDLE_PWM;
        return 0;
#else
        return -1;
#endif /* BACKLIGHT_MIDDLE_PWM */

    case eBacklightLow:
#ifdef BACKLIGHT_LOW_PWM
        *pu4Data = BACKLIGHT_LOW_PWM;
        return 0;
#else
        return -1;
#endif /* BACKLIGHT_LOW_PWM */

    case eBackLightGpio2:
#ifdef BACKLIGHT_GPIO2
        *pu4Data = BACKLIGHT_GPIO2;
        return 0;
#else
        return -1;
#endif /* BACKLIGHT_GPIO2 */

    case eSpreadSpectrumPermillage:
#ifdef SPREAD_SPECTRUM_PERMILLAGE
        *pu4Data = SPREAD_SPECTRUM_PERMILLAGE;
        return 0;
#else
        return -1;
#endif /* SPREAD_SPECTRUM_PERMILLAGE */

    case eTimerDimmingFreq:
#ifdef TIMER_DIMMING_FREQ
        *pu4Data = TIMER_DIMMING_FREQ;
        return 0;
#else
        return -1;
#endif /* TIMER_DIMMING_FREQ */

    case eDimmingFreq60:
        *pu4Data = DIMMING_FREQ_60;
        return 0;

    case eDimmingFreq50:
        *pu4Data = DIMMING_FREQ_50;
        return 0;

    case ePanelTTLOutput:
        *pu4Data = PANEL_TTL_OUTPUT;
        return 0;

    case ePanelFlashTableSize:
        *pu4Data = PANEL_FLASH_TABLE_SIZE;
        return 0;

    case ePanelFlashTableAddress:
        *pu4Data = PANEL_FLASH_TABLE_ADDRESS;
        return 0;

    case ePanelEepromToFlashIndexMap:
        *pu4Data = (UINT32)PANEL_EEPROM_TO_FLASH_INDEX_MAP;
        return 0;

    case eLvdsDrivingCurrent:
#ifdef LVDS_DRIVING_CURRENT
        *pu4Data = LVDS_DRIVING_CURRENT;
        return 0;
#else
        return -1;
#endif /* LVDS_DRIVING_CURRENT */

    default:
        return -1;
    }
}

UINT32 DRVCUST_PanelGet(QUERY_TYPE_T eQryType)
{
    UINT32 u4Ret;

    DRVCUST_PanelQuery(eQryType, &u4Ret);
    return u4Ret;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 IsLvdsMsbSwOn(void)
{
    UINT32 u4Value;
    if (DRVCUST_PanelQuery(eLvdsMsbSw, &u4Value) == 0)
    {
        return u4Value;
    }
    if (IS_COMPANION_CHIP_ON())
    {
        return LVDS_JEIDA;
    }
    return PANEL_GetControlWord() & LVDS_MSB_SW_ON;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 IsPwmLowPanelBright(void)
{
    UINT32 u4Value;
    if (DRVCUST_PanelQuery(ePanelPwmPolarity, &u4Value) == 0)
    {
        return u4Value;
    }
    return PANEL_GetControlWord() & PWM_LOW_PANEL_BRIGHT;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void SelectPanel(UINT32 u4Index)
{
    // read panel index from customer define
    if (u4Index == PANEL_INVALID)
    {
        u4Index = DRVCUST_PanelGet(eDefaultPanelSelect);
    }
    _u4PanelIndex = u4Index;

#ifndef CC_MULTI_PANEL
     _u4PanelIndex = 0;
#endif

    _prPanelAttribute = &_arPanelAttribute[_u4PanelIndex];

}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

CHAR* GetCurrentPanelName(void)
{
    return GetPanelName(GetCurrentPanelIndex());
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 GetCurrentPanelIndex(void)
{
    return _u4PanelIndex;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void DumpPanelAttribute(UINT32 u4Flag)
{
    UINT32 u4Size = sizeof(_arPanelAttribute) / sizeof(PANEL_ATTRIBUTE_T);
    UINT32 i = 0;
    PANEL_ATTRIBUTE_T* prTbl;

    if ((u4Flag & PANEL_DUMP_ALL) == 0)
    {
        i = _u4PanelIndex;
        u4Size = _u4PanelIndex + 1;
    }
    for (; i < u4Size; i++)
    {
        prTbl = &_arPanelAttribute[i];
        Printf("---------- Panel[%3d] %s %dx%d ----------\n",
               i, GetPanelName(i), prTbl->u2PanelWidth, prTbl->u2PanelHeight);
        if ((u4Flag & PANEL_DUMP_TITLE) == PANEL_DUMP_TITLE)
        {
            continue;
        }
        Printf("PixelClk[Max=%d, 60Hz=%d, 50Hz=%d, Min=%d]\n",
               prTbl->u4PixelClkMax, prTbl->u4PixelClk60Hz,
               prTbl->u4PixelClk50Hz, prTbl->u4PixelClkMin);
        Printf("HTotal[Max=%d, 60Hz=%d, 50Hz=%d, Min=%d] HSyncLen[60Hz=%d, 50Hz=%d]\n",
               prTbl->u2HTotalMax, prTbl->u2HTotal60Hz,
               prTbl->u2HTotal50Hz, prTbl->u2HTotalMin,
               PANEL_GetHSyncLen60Hz(), PANEL_GetHSyncLen50Hz());
        Printf("VTotal[Max=%d, 60Hz=%d, 50Hz=%d, Min=%d] VClk[Max=%d, Min=%d]\n",
               prTbl->u2VTotalMax, prTbl->u2VTotal60Hz,
               prTbl->u2VTotal50Hz, prTbl->u2VTotalMin,
               prTbl->u1VClkMax, prTbl->u1VClkMin);
        Printf("HSyncWidth=%d, ", prTbl->u1HSyncWidth);
        Printf("VSyncWidth=%d, ", prTbl->u1VSyncWidth);
        Printf("HPosition60Hz=%d, ", prTbl->u1HPosition);
        Printf("HPosition50Hz=%d, ", PANEL_GetHPosition50Hz());
        Printf("VPosition=%d\n", prTbl->u1VPosition);
        if (prTbl->u2ControlWord & LVDS_MSB_SW_ON)
        {
            Printf("LVDS_NS, ");
        }
        else
        {
            Printf("LVDS_JEIDA, ");
        }
        if (prTbl->u2ControlWord & LVDS_ODD_SW_ON)
        {
            Printf("LVDS_ODD_SW_ON, ");
        }
        else
        {
            Printf("LVDS_ODD_SW_OFF, ");
        }
        if (prTbl->u2ControlWord & DUAL_PORT)
        {
            Printf("DUAL_PORT, ");
        }
        else
        {
            Printf("SINGLE_PORT, ");
        }

        if ((prTbl->u2ControlWord & DISP_BIT_MSK) == DISP_30BIT)
        {
            Printf("DISP_30BIT, ");
        }
        else if ((prTbl->u2ControlWord & DISP_BIT_MSK) == DISP_24BIT)
        {
            Printf("DISP_24BIT\n");
        }
        else
        {
            Printf("DISP_16BIT\n");
        }
        if (prTbl->u2ControlWord & WFB_PANEL_RESET_ON)
        {
            Printf("WFB_PANEL_RESET_ON, ");
        }
        else
        {
            Printf("WFB_PANEL_RESET_OFF, ");
        }
        if (prTbl->u2ControlWord & PWM_LOW_PANEL_BRIGHT)
        {
            Printf("PWM_LOW_PANEL_BRIGHT\n");
        }
        else
        {
            Printf("PWM_HIGH_PANEL_BRIGHT\n");
        }
        Printf("Backlight[High=0x%x, Middle=0x%x, Low=0x%x]\n",
               prTbl->u1BacklightHigh, prTbl->u1BacklightMiddle,
               prTbl->u1BacklightLow);
        Printf("DimFreq60Hz=%d, DimFreq50Hz=%d\n",
               prTbl->u2DimmingFrequency60Hz, prTbl->u2DimmingFrequency50Hz);
        Printf("Delay[LvdsOn=%d0, BacklightOn=%d0, BacklightOff=%d0, LvdsOff=%d0]\n",
               prTbl->u1LvdsOnDalay, prTbl->u1BacklightOnDelay,
               prTbl->u1BacklightOffDelay, prTbl->u1LvdsOffDalay);
        Printf("MaxOverscan=%d\n", prTbl->u2MaxOverscan);
    }
    Printf("\nCurrent panel setting is %s\n", GetCurrentPanelName());
    Printf("Backlight gpio=%d, turn on value=%d, ",
           DRVCUST_PanelGet(eBackLightGpio), DRVCUST_PanelGet(eBackLightOnVal));
    Printf("LVDS gpio=%d, turn on value=%d\n",
           DRVCUST_PanelGet(ePanelPowerGpio), DRVCUST_PanelGet(ePanelPowerOnVal));
    Printf("Pixel Shift=%d (%d x %d)\n", _fgPixelShiftEnable,
           PANEL_GetPanelWidth(), PANEL_GetPanelHeight());

}


UINT32 PANEL_TranslateEepromToFlashIndex(UINT32 u4EepromIndex)
{
    UINT32 u4Size, u4FlashIndex;
    UINT32* pu4Array;

    u4Size = DRVCUST_PanelGet(ePanelFlashTableSize);
    if ((pu4Array = (UINT32*)DRVCUST_PanelGet(ePanelEepromToFlashIndexMap)) !=  NULL)
    {
        for (u4FlashIndex = 0; u4FlashIndex < u4Size; u4FlashIndex++)
        {
            if (pu4Array[u4FlashIndex] == u4EepromIndex)
            {
                return u4FlashIndex;
            }
        }
        if (u4FlashIndex == u4Size)
        {
            return 0;
        }
    }
    return u4EepromIndex;
}

UINT32 PANEL_GetEepromPanelIndex(void)
{
    return _u4EepromPanelIndex;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void LoadPanelIndex(void)
{
    UINT8 au1Index[] = {PANEL_INVALID, PANEL_INVALID, PANEL_INVALID};
    QUERY_TYPE_T arKey[] = {ePanelIndexOffset1, ePanelIndexOffset2,
                            ePanelIndexOffset3};
    UINT32 u4Size = sizeof(arKey) / sizeof(QUERY_TYPE_T);
    UINT32 u4Value, u4ValidCounter = 0, i, j;
    UINT32 u4FinalIndex = PANEL_INVALID, i4Ret;

    if ((DRVCUST_PanelQuery(ePanelIndexFromEeprom, &u4Value) == 0) &&
        (u4Value == 1))
    {
        // hack u4TotalNumber to pass IS_PANEL_INDEX_INVALID
        if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
        {
            u4TotalNumber = 0xff;
        }

        // read configuration from eeprom
        for (i = 0; i < u4Size; i++)
        {
            if (DRVCUST_PanelQuery(arKey[i], &u4Value) == 0)
            {
//                i4Ret = EEPROM_Read(u4Value, (UINT32)&au1Index[i], 1);
                if ((i4Ret != 0) || IS_PANEL_INDEX_INVALID(au1Index[i]))
                {
                    au1Index[i] = PANEL_INVALID;
                }
                else
                {
                    u4ValidCounter++;
                }
            }
        }

        // decide valid panel index
        if (u4ValidCounter == 1)
        {
            for (i = 0; i < u4Size; i++)
            {
                if (au1Index[i] != PANEL_INVALID)
                {
                    u4FinalIndex = au1Index[i];
                    break;
                }
            }
        }
        else if (u4ValidCounter >= 2)
        {
            for (i = 0; i < u4Size; i++)
            {
                if (au1Index[i] == PANEL_INVALID)
                {
                    continue;
                }
                for (j = 0; j < u4Size; j++)
                {
                    if ((i != j) && (au1Index[i] == au1Index[j]))
                    {
                        u4FinalIndex = au1Index[i];
                        break;
                    }
                }
                if (u4FinalIndex != PANEL_INVALID)
                {
                    break;
                }
            }
        }
        
        _u4EepromPanelIndex = u4FinalIndex;
        if (u4FinalIndex != PANEL_INVALID)
        {
            Printf("Find panel index %d(%s) from EEPROM\n",
                   u4FinalIndex, GetCurrentPanelName());
            if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
            {
                // if there is no mapping table, return the same index
                // if can't find flash index from mapping table, return 0
                u4FinalIndex = PANEL_TranslateEepromToFlashIndex(u4FinalIndex);
                Printf("Translate panel index to %d\n", u4FinalIndex);

                // a backup solution
                if (u4FinalIndex > 100)
                {
                    SelectPanel(u4FinalIndex - 100);
                }
                else
                {
                    UINT32 offset = DRVCUST_PanelGet(ePanelFlashTableAddress)+
                    (u4FinalIndex * DRVCUST_PanelGet(ePanelFlashTableSize));
                    if (!_PanelLoadTableFromFlash(offset))
                    {
                        SelectPanel(1);
                    }
                }
            }
            else
            {
                SelectPanel(u4FinalIndex);
            }
        }
        else
        {
            Printf("No valid index found from EEPROM\n");
            SelectPanel(PANEL_INVALID);
        }
        
        WritePanelIndexToEeprom(_u4EepromPanelIndex);
    }
    else if ((u4Value = DRVCUST_PanelGet(ePanelIndexFromGpioNum)) > 0)
    {
        UINT32* pu4Array = (UINT32*)DRVCUST_PanelGet(ePanelIndexFromGpioIdx);
        u4FinalIndex = 0;
        for (i = 0; i < u4Value; i++)
        {
            u4FinalIndex <<= 1;
//            u4FinalIndex |= GPIO_Input(pu4Array[i]);
            //Printf("gpio[%d]=%d, u4FinalIndex=%d\n", i, pu4Array[i], u4FinalIndex);
        }
        pu4Array = (UINT32*)DRVCUST_PanelGet(ePanelIndexFromGpioMap);
        u4FinalIndex = pu4Array[u4FinalIndex];
        if (IS_PANEL_INDEX_INVALID(u4FinalIndex))
        {
            Printf("No valid index found from GPIO\n");
            SelectPanel(PANEL_INVALID);
        }
        else
        {
            SelectPanel(u4FinalIndex);
            Printf("Find panel index %d(%s) from GPIO\n",
                   u4FinalIndex, GetCurrentPanelName());
        }
    }
    else
    {
        // use compile time defined panel index
        SelectPanel(PANEL_INVALID);
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void WritePanelIndexToEeprom(UINT32 u4PanelIndex)
{
    UINT32 u4Value, i;
    QUERY_TYPE_T arKey[] = {ePanelIndexOffset1, ePanelIndexOffset2,
                            ePanelIndexOffset3};
    UINT32 u4Size = sizeof(arKey) / sizeof(QUERY_TYPE_T);

    if ((DRVCUST_PanelQuery(ePanelIndexFromEeprom, &u4Value) == 0) &&
        (u4Value == 1))
    {
        for (i = 0; i < u4Size; i++)
        {
            if (DRVCUST_PanelQuery(arKey[i], &u4Value) == 0)
            {
//                if (EEPROM_Write((UINT64)u4Value, (UINT32)&u4PanelIndex, 1) != 0)
//                {
//                    Printf("WritePanelIndexToEeprom fail, eeprom addr=0x%x\n",
//                           u4Value);
//                }
            }
        }
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 PANEL_GetBacklightHigh(void)
{
    UINT32 value;
    if (DRVCUST_PanelQuery(eBacklightHigh, &value) == 0)
    {
        return value;
    }

    return _prPanelAttribute->u1BacklightHigh;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 PANEL_GetBacklightMiddle(void)
{
    UINT32 value;
    if (DRVCUST_PanelQuery(eBacklightMiddle, &value) == 0)
    {
        return value;
    }
    return _prPanelAttribute->u1BacklightMiddle;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 PANEL_GetBacklightLow(void)
{
    UINT32 value;
    if (DRVCUST_PanelQuery(eBacklightLow, &value) == 0)
    {
        return value;
    }
    return _prPanelAttribute->u1BacklightLow;
}

UINT32 PANEL_GetHSyncLen60Hz(void)
{
    UINT32 u4Len = 0;

    if (_fgPixelShiftEnable)
    {
        // leave right mask as (PIXEL_SHIFT_H_MAX + 1) - _i4HPixelShift
        u4Len = (PANEL_GetHTotal60Hz() - PANEL_GetPanelWidth()) -
                ((PIXEL_SHIFT_H_MAX + 1) - _i4HPixelShift);
    }
    else
    {
        // leave right mask as PIXEL_SHIFT_H_MAX*2 + 1
        u4Len = (PANEL_GetHTotal60Hz() - PANEL_GetPanelWidth()) -
                (PIXEL_SHIFT_H_MAX * 2 + 1);
    }
    // protect overflow
    return (u4Len > 511)?511:u4Len;
}

UINT32 PANEL_GetHSyncLen50Hz(void)
{
    UINT32 u4Len = 0;

    if (_fgPixelShiftEnable)
    {
        // leave right mask as (PIXEL_SHIFT_H_MAX + 1) - _i4HPixelShift
        u4Len = (PANEL_GetHTotal50Hz() - PANEL_GetPanelWidth()) -
                ((PIXEL_SHIFT_H_MAX + 1) - _i4HPixelShift);
    }
    else
    {
        // leave right mask as PIXEL_SHIFT_H_MAX*2 + 1
        u4Len = (PANEL_GetHTotal50Hz() - PANEL_GetPanelWidth()) -
                (PIXEL_SHIFT_H_MAX * 2 + 1);
    }
    // protect overflow
    return (u4Len > 511)?511:u4Len;
}

UINT32 PANEL_GetHPosition50Hz(void)
{
    return (((PANEL_GetHPosition60Hz() << 1) + PANEL_GetHTotal50Hz())
        - PANEL_GetHTotal60Hz()) >> 1;
}

UINT32 PANEL_GetVPosition(void)
{
    if (_fgPixelShiftEnable)
    {
        return (UINT32)((INT32)(_prPanelAttribute->u1VPosition +
                                PIXEL_SHIFT_V_MAX) - _i4VPixelShift);
    }
    else
    {
        return _prPanelAttribute->u1VPosition;
    }
}

UINT32 PANEL_GetPanelWidth(void)
{
    if (_fgPixelShiftEnable)
    {
        return _prPanelAttribute->u2PanelWidth + (PIXEL_SHIFT_H_MAX * 2);
    }
    else
    {
        return _prPanelAttribute->u2PanelWidth;
    }
}

UINT32 PANEL_GetPanelHeight(void)
{
    if (_fgPixelShiftEnable)
    {
        return _prPanelAttribute->u2PanelHeight + (PIXEL_SHIFT_V_MAX * 2);
    }
    else
    {
        return _prPanelAttribute->u2PanelHeight;
    }
}

void PANEL_SetPixelShift(BOOL fgEnable, INT32 i4HPixelShift, INT32 i4VPixelShift)
{
    _fgPixelShiftEnable = fgEnable;
    _i4HPixelShift = i4HPixelShift;
    _i4VPixelShift = i4VPixelShift;
}

#endif /* CC_5391_PRELOADER */

