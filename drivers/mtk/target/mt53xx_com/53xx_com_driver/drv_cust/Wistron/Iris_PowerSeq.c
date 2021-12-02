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

#include "x_debug.h"
#include "x_hal_arm.h"
#include "x_hal_io.h"
#include "ostg_if.h"
#include "panel.h"
#include "pdwnc_if.h"
#include "rtc_if.h"
#include "SMicro/SMicro_prc.h"
#include "SMicro/SMicro_if.h"

//-----------------------------------------------------------------------------
// Constant definitions & Macro
//-----------------------------------------------------------------------------
#define CC_IRIS_GET_DELAY_FROM_EEPROM
#define CUSTOMER_POWER_ON_SEQUENCE          Iris_PowerOnSequence
#define CUSTOMER_POWER_OFF_SEQUENCE         Iris_PowerOffSequence

// EEPROM information

// Detect time period definition
#define TCON_READY_DETECT_TIME              3000

#ifdef CC_MTK_LOADER
#define IRIS_SMICRO_INIT                    SMicro_prc_rtx_Init
#define IRIS_SMICRO_SEND_CMD                u4SMicroSendCmd
#else
#define IRIS_SMICRO_INIT                    u4SMicroHWInit
#define IRIS_SMICRO_SEND_CMD                u4SMicroSendCmd
#endif

#define GET_POWER_SEQ_DELAY(Address, Delay, Value, DataLen, Default)    \
    UNUSED(EEPROM_Read(Address, (UINT32)(VOID *)&Value, DataLen));      \
    Value = (((Value & 0xFF00) >> 8) | ((Value & 0xFF) << 8));          \
    Delay = (Value == IRIS_DELAY_VALID_MAX) ? Default : Value;          \
    if (Value != Delay)                                                 \
    {                                                                   \
        Value = (((Delay & 0xFF00) >> 8) | ((Delay & 0xFF) << 8));      \
        UNUSED(EEPROM_Write(Address, (UINT32)(VOID *)&Value, DataLen)); \
    }

//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------
static void Iris_PowerOnSequence(void);
static void Iris_PowerOffSequence(void);


//-----------------------------------------------------------------------------
// Imported funtions 
//-----------------------------------------------------------------------------
EXTERN void vDrvTurnOnPanel(void);
EXTERN void vDrvTurnOffPanel(void);
EXTERN void vDrvLVDSOff(void);
EXTERN void vDrvLVDSConfig(void);
EXTERN void vDrvLVDSOn(void);
EXTERN UINT32 PANEL_GetLvdsOnDalay(void);
EXTERN UINT32 PANEL_GetLvdsOffDalay(void);
EXTERN UINT32 PANEL_GetBacklightOnDelay(void);
EXTERN UINT32 PANEL_GetBacklightOffDelay(void);
EXTERN void vDrvSetLock(UINT32 u4Src, UINT32 u4LockON);


EXTERN UINT32 _u4BrightLevel;


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgIs120hzPanel = FALSE;
static UINT16 _au2PowerSeqDelay[IRIS_PWR_SEQ_DELAY_SIZE];
static UINT16 _au2DefaultDelay[IRIS_PWR_SEQ_DELAY_SIZE] = 
       {10, 300, 50, 300, 500, 400, 200, 100, 30, 200, 10, 200};
HAL_TIME_T _rTime;

#if defined(IRIS_PCBA_EVT) && !defined(CC_MTK_LOADER)
static BOOL _bfgACDetectGo = FALSE;
static UINT32 _u4ACDetectDelay = 0;
static HANDLE_T _hIrisACDetectHandle = NULL;
static HANDLE_T _hIrisACDetectSema = NULL;


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
static void IrisACDetectHandle(void *pvArg)
{
    INT32  i4Value = 0;
    UINT32 u4Counter = 0;
    BOOL bACDrop = FALSE;

    UNUSED(pvArg);
    while (_bfgACDetectGo)
    {
        // Detect AC drop
        i4Value = GPIO_Input(SIGNAL_AC_DET);
        if (i4Value == AC_DETECT_LOW)
        {
            PRINT_TIMESTAMP_DBG_MSG("[AC Detect]", "AC_DET = %d\n", i4Value);
            
            // backlight power -> lvds sugnal -> lvds power
            PRINT_TIMESTAMP_DBG_MSG("[AC Detect]", "BL_ON = %d\n",
                                    (INT32)!DRVCUST_PanelGet(eBackLightOnVal));
            vDrvTurnOffBacklt(); // Backlight off
            
            // Stop SelfDiag
            PRINT_TIMESTAMP_MSG("[AC Detect]", "Stop Self Diagnostic\n");
            IrisSelfDiagStop(D_SYSTEM_NORMAL);

            bACDrop = TRUE;
            for (u4Counter = 0; u4Counter < 2; u4Counter++)
            {
                // Detect AC drop
                i4Value = GPIO_Input(SIGNAL_AC_DET);
                PRINT_TIMESTAMP_DBG_MSG("[AC Detect]", "AC_DET = %d\n", i4Value);
                
                if (i4Value == AC_DETECT_HIGH)
                {
                    x_thread_delay(_au2PowerSeqDelay[11]);
                    
                    // Turn on backlight
                    PRINT_TIMESTAMP_DBG_MSG("[AC Detect]", "LVDS_PWR_ON = %d\n",
                                            DRVCUST_PanelGet(eBackLightOnVal));
                    vDrvTurnOnBacklt();
                    
                    // Start Self Diagnostic
                    PRINT_TIMESTAMP_MSG("[AC Detect]", "Start Self Diagnostic\n");
                    IrisSelfDiagStart(D_SYSTEM_NORMAL);
                    
                    bACDrop = FALSE;
                    break;
                }
                
                x_thread_delay(_u4ACDetectDelay);
            }
        }

        // If "AC_DET == AC_DETECT_LOW" continues 3 times.
        // Go to panel off
        if (bACDrop)
        {
            UINT64 u8Time;
            PRINT_TIMESTAMP_MSG("[AC Detect]", "AC_DET error !\n");
            
            i4Value = 0;
            UNUSED(GPIO_Output(SIGNAL_POWER_ON_RDY, &i4Value));
            PRINT_TIMESTAMP_DBG_MSG("[AC Detect]", 
                                    "POWER_ON_RDY = %d\n", i4Value);
    
            PRINT_TIMESTAMP_MSG("[AC Detect]", "LVDS_OUTPUT = 0\n");
            vDrvLVDSOff();
            x_thread_delay(_au2PowerSeqDelay[10]);
            
            vDrvTurnOffPanel(); // Panel off
            PRINT_TIMESTAMP_DBG_MSG("[AC Detect]", "BL_ON = %d\n",
                                    (INT32)!DRVCUST_PanelGet(ePanelPowerOnVal));
            
            i4Value = 0;
            UNUSED(GPIO_Output(SIGNAL_X_U_RST, &i4Value));
            PRINT_TIMESTAMP_DBG_MSG("[AC Detect]", "X_U_RST = %d\n", i4Value);
            
            // Turn off all Interrupt.
            BIM_WRITE32(REG_RW_IRQCL, 0xffffffff);
            BIM_WRITE32(REG_RW_IRQEN, 0);
            BIM_WRITE32(REG_RW_FIQCL, 0xffffffff);
            BIM_WRITE32(REG_RW_FIQEN, 0);

            if (DRVCUST_InitGet(ePdwncPolarity))
            {
                vIO32WriteFldAlign(PDWNC_PDMISC, 0x1, FLD_PDWN_POL);
            }
            else
            {
                vIO32WriteFldAlign(PDWNC_PDMISC, 0x0, FLD_PDWN_POL);
            }
            
            // Disable the watchdog
            vIO32Write4B(PDWNC_WDTCTL, 0);
                
            RTC_GetTimeDate(&u8Time);
            u8Time += 3;
            RTC_SetAlertTime(u8Time);
        
            // Enable RTC wakeup            
            vIO32WriteFldAlign(PDWNC_WAKEN, 0x1, FLD_RTC_WAKEN);
            
            // Control LED
            GPIO_IrisLedCtrlFunc(eLedSystemPowerOff);
            
            Printf("Standby !\n");
            vIO32Write4B(PDWNC_PDCODE, PWRDWN_CODE1);
            vIO32Write4B(PDWNC_PDCODE, PWRDWN_CODE2);
            return;
        }
        
        x_thread_delay(_u4ACDetectDelay);
    }

    VERIFY(x_sema_unlock(_hIrisACDetectSema) == OSR_OK);
    x_thread_exit();
}

void Iris_StartACDetec(void)
{
    _bfgACDetectGo = TRUE;
    
    // Create the AC detection handle semaphore
    VERIFY(x_sema_create(&_hIrisACDetectSema, X_SEMA_TYPE_BINARY,
                         X_SEMA_STATE_LOCK) == OSR_OK);
    
    // Create the self-diag handle task
    if (x_thread_create(&_hIrisACDetectHandle, "IrisACDetectHandle", 0x800,
                        DRVCUST_OptGet(eReleaseWatchdogPriority),
                        IrisACDetectHandle, 0, NULL) != OSR_OK)
    {
        LOG(1, "\nError: Create AC Detect thread fail\n");
        x_sema_delete(_hIrisACDetectSema);
        _hIrisACDetectSema = NULL;
        _bfgACDetectGo = FALSE;
    }
}

void Iris_StopACDetec(void)
{
    // Wait AC_DET thread exit
    if (_bfgACDetectGo == TRUE)
    {
        _bfgACDetectGo = FALSE;
        VERIFY(x_sema_lock(_hIrisACDetectSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        
        x_sema_delete(_hIrisACDetectSema);
        _hIrisACDetectSema = NULL;
        _hIrisACDetectHandle = NULL;
    }
}
#endif /* defined(IRIS_PCBA_EVT) && !defined(CC_MTK_LOADER) */

void Iris_GetPowerSequenceDelay(void)
{
    UINT32 u4Len = sizeof(UINT16) * IRIS_PWR_SEQ_DELAY_SIZE;
    
#ifdef CC_IRIS_GET_DELAY_FROM_EEPROM
    UINT16 u2Val = 0, i;
    UINT64 u8Addr = IRIS_PWR_SEQ_AC_DET_TIME_ADDR;

    x_memset(_au2PowerSeqDelay, 0xFF, u4Len);
    
    // Get power sequence delay from EEPROM
    u4Len = sizeof(UINT16);
    for (i = 0; i < IRIS_PWR_SEQ_DELAY_SIZE; i++)
    {
        GET_POWER_SEQ_DELAY(u8Addr, _au2PowerSeqDelay[i], 
                            u2Val, u4Len, _au2DefaultDelay[i]);
        u8Addr += sizeof(UINT16);
    }
    
#else
    x_memcpy(_au2PowerSeqDelay, _au2DefaultDelay, u4Len);
    
#endif /* CC_IRID_GET_DELAY_FROM_EEPROM */

#if defined(IRIS_PCBA_EVT) && !defined(CC_MTK_LOADER)
    _u4ACDetectDelay = _au2PowerSeqDelay[0];
#endif /* defined(IRIS_PCBA_EVT) && !defined(CC_MTK_LOADER) */
}

void Iris_CheckTCON_RDY(void)
{
    UINT32 u4TimeCount = 0;
    INT32   i4Value = 0;
    
    PRINT_TIMESTAMP_MSG("[Power On]", "Check TCON_RDY\n");
    
    // Check TCON-RDY signal
    // If (TCON-RDY = 0) continues 3 seconds, 
    // enter the safty shutdown (self diagnostics)
    u4TimeCount = 0;
    do
    {
        i4Value = GPIO_Input(SIGNAL_TCON_RDY);
        PRINT_TIMESTAMP_DBG_MSG("[Power On]", "TCON_RDY = %d\n", i4Value);
        if (i4Value == 1)   break;

        x_thread_delay(_au2PowerSeqDelay[7]);
        u4TimeCount += _au2PowerSeqDelay[7];
    } while (u4TimeCount < TCON_READY_DETECT_TIME);

    if (u4TimeCount >= TCON_READY_DETECT_TIME)
    {
        PRINT_TIMESTAMP_DBG_MSG("[Power On]", "TCON_RDY is not ready over %d ms\n", u4TimeCount);
        
#ifndef CC_MTK_LOADER        
        // go to self diagnostics
        IrisSelfDiagSetShutdown(D_TCON_ERROR);
#endif /* CC_MTK_LOADER */
    }
}

void Iris_StartDimmerOutput(void)
{
    PRINT_TIMESTAMP_MSG("[Power On]", "START_DIMMER_OUTPUT\n");
    
    // Start dimmer output        
    // after panel on, we could set backlight
    vApiSetPanelBright(_u4BrightLevel);
    if (DRVCUST_PanelGet(eFlagBacklightDimmingLock))
    {
        vDrvSetLock(DRVCUST_PanelGet(ePanelBacklightPwmPort), SV_ON);
    }
    else
    {
        vDrvSetLock(DRVCUST_PanelGet(ePanelBacklightPwmPort), SV_OFF);
    }
}

UINT32 Iris_GetTemperature(void)
{
    UINT8  au1Temperature[IRIS_SELF_DIAG_TEMP_DATA_LEN] = {0, 0};
    UINT32 u4Temperature;
    
    UNUSED(SIF_Read(IRIS_SELF_DIAG_TEMP_SIF_CLK_DIV, 
                    IRIS_SELF_DIAG_TEMP_SIF_DEV_ADDR,
                    IRIS_SELF_DIAG_TEMP_SIF_SUB_ADDR,
                    au1Temperature, IRIS_SELF_DIAG_TEMP_DATA_LEN));
    u4Temperature = au1Temperature[0] << 1;
    u4Temperature |= au1Temperature[1] >> 7;
    return u4Temperature;
}

void Iris_60HzPowerOnSequence(void)
{
#ifdef IRIS_PCBA_EVT    
    UINT32 u4Value;
#endif /* IRIS_PCBA_EVT */

    INT32 i4Value;
    
    i4Value = 1;
    UNUSED(GPIO_Output(SIGNAL_X_U_RST, &i4Value));
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "X_U_RST = %d\n", i4Value);

    i4Value = 0;
    UNUSED(GPIO_Output(SIGNAL_BUS_INT, &i4Value));
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "BUS_INT = %d\n", i4Value);
        
    x_thread_delay(_au2PowerSeqDelay[1]);
    
#ifdef IRIS_PCBA_EVT    
    // Read p_sel
    u4Value = PDWNC_ReadServoADCChannelValue(SIGNAL_PANEL_SEL);
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "PANEL_SEL = %d\n", u4Value);
    
    // normal power sequence, lvds power -> lvds signal -> backlight power
    vDrvTurnOnPanel();
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "LVDS_PWR_ON = %d\n",
                            DRVCUST_PanelGet(ePanelPowerOnVal));

    // Set "BUS_SW_ON" as HIGH
    i4Value = 1;
    UNUSED(GPIO_Output(SIGNAL_BUS_SW_ON, &i4Value));
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "BUS_SW_ON = %d\n", i4Value);
    
    x_thread_delay(_au2PowerSeqDelay[2]);
#endif /* IRIS_PCBA_EVT */
    
    // Read Panel ID
    PRINT_TIMESTAMP_MSG("[Power On]", "PANEL_ID sequence\n");
    
    // Read panel ID by SIF bus
    // TBD
        
    // If failed, go to self diagnostics
#if 0
    if (_eSelfDiagEnable.u2PanelNVMError == IRIS_SELF_DIAG_ENABLE)
    {
        IrisSelfDiagSetShutdown(D_PANEL_NVM_ERROR);
    }
#endif
    
    // Send LVDS output
    PRINT_TIMESTAMP_MSG("[Power On]", "LVDS_OUTPUT (Send LVDS output)\n");
    vDrvLVDSConfig();
    vDrvLVDSOn();
    
    x_thread_delay(_au2PowerSeqDelay[6]);

    // Check TCON_RDY
    Iris_CheckTCON_RDY();
    
    // Read Temp Sensor
    u4Value = Iris_GetTemperature();
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "Temp sensor = %d\n", u4Value);
    
    // Start dimmer output        
    Iris_StartDimmerOutput();
    
    x_thread_delay(_au2PowerSeqDelay[8]);

    // Set DCC_LUT0, DCC_LUT1, DCC_LUT2 -> for 60Hz(FHD)
    // DCC_LUT0 -> HDMI_SDA0
    // DCC_LUT1 -> HDMI_SCL0
    // DCC_LUT2 -> HDMI_SCL1
    PRINT_TIMESTAMP_MSG("[Power On]", 
                        "DCC_LUT0 = (none)\nDCC_LUT1 = (none)\nDCC_LUT2 = (none)\n");

    x_thread_delay(_au2PowerSeqDelay[9]);
}

void Iris_120HzPowerOnSequence(void)
{
#ifdef IRIS_PCBA_EVT    
    UINT32 u4Value;
#endif /* IRIS_PCBA_EVT */

    UINT8  u1LVDSMute, u1AutoDetect;
    UINT8  au1PanelID[16], au1PanelInfo[22];
    INT32  i4Value;
    
    UNUSED(IRIS_SMICRO_INIT());
    
    // Set "U_POWER_READY" as HIGH
    i4Value = 1;
    UNUSED(GPIO_Output(SIGNAL_POWER_ON_RDY, &i4Value));
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "POWER_ON_RDY = %d\n", i4Value);

    UNUSED(GPIO_Output(SIGNAL_X_U_RST, &i4Value));
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "X_U_RST = %d\n", i4Value);

    x_thread_delay(_au2PowerSeqDelay[1]);
    
    // normal power sequence, lvds power -> lvds signal -> backlight power
    vDrvTurnOnPanel();
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "LVDS_PWR_ON = %d\n",
                            DRVCUST_PanelGet(ePanelPowerOnVal));

#ifdef IRIS_PCBA_EVT    
    // Set "BUS_SW_ON" as HIGH
    i4Value = 1;
    UNUSED(GPIO_Output(SIGNAL_BUS_SW_ON, &i4Value));
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "BUS_SW_ON = %d\n", i4Value);
#endif /* IRIS_PCBA_EVT */

    // Read panel ID from Sony-Micro
    // Send command : Panel ID from sub-micro
    // Module ID = 0, Command ID = 4, Input data = None
    PRINT_TIMESTAMP_MSG("[Power On]", "SMicro command 1\n");
    UNUSED(IRIS_SMICRO_SEND_CMD(MODULEID_GENERIC, CMD0_GET_PANEL_ID,
                                NULL, 0, NULL, 0));
    
    // Set panel ID auto detect
    // Send command : auto detect = 1 -> get Panel Id from sub-micro
    //                auto detect = 0 -> get Panel Id from NVM
    // Module ID = 0, Command ID = 5, Input data = 1
    PRINT_TIMESTAMP_MSG("[Power On]", "SMicro command 2\n");
    u1AutoDetect = 1;
    UNUSED(IRIS_SMICRO_SEND_CMD(MODULEID_GENERIC, CMD0_SET_PANEL_ID_AUTO_DETECTION,
                                &u1AutoDetect, 1, au1PanelID, 16));
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "Panel ID = %s\n", au1PanelID);
    
    x_thread_delay(_au2PowerSeqDelay[2]);
    
#ifdef IRIS_PCBA_EVT    
    // Read p_sel
    u4Value = PDWNC_ReadServoADCChannelValue(SIGNAL_PANEL_SEL);
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "PANEL_SEL = %d\n", u4Value);
#endif /* IRIS_PCBA_EVT */

    // Set panel ID
    // Send command : sent to sony-micro panel ID which is returned by sony-micro
    // Module ID = 0, Command ID = 3, Input data = 1
    PRINT_TIMESTAMP_MSG("[Power On]", "SMicro command 3\n");
    x_memset(au1PanelInfo, 0, sizeof(au1PanelInfo));
    au1PanelInfo[0] = 1;
    au1PanelInfo[21] = 4;
    x_memcpy(&au1PanelInfo[1], au1PanelID, sizeof(au1PanelID));
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "Set Panel Info = %s\n", au1PanelInfo);
//    UNUSED(IRIS_SMICRO_SEND_CMD(MODULEID_GENERIC, CMD0_SET_PANEL_INFO,
//                                &au1PanelInfo, 22, NULL, 0));

    // Read Panel ID
    PRINT_TIMESTAMP_MSG("[Power On]", "PANEL_ID sequence\n");
    #ifdef LOAD_PANEL_FROM_FLASH_CUSTOM
    CHAR *pStr = FlashPanel_GetIDString();
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "FlashPanel_GetIDString = %s\n", pStr);
    if (u4PanelStringCmp((CHAR *)au1PanelID, pStr) == 0)
    {
        PRINT_TIMESTAMP_MSG("[Power On]", "Panel ID match\n");
    }
    else
    {
        PRINT_TIMESTAMP_MSG("[Power On]", "Panel ID mismatch\n");
        if (!LOAD_PANEL_FROM_FLASH_CUSTOM(LOAD_PANEL_FROM_FLASH_BY_NAME, (VOID *)au1PanelID))
        {
            PRINT_TIMESTAMP_MSG("[Power On]", "Panel ID error\n");
        }
        else
        {
            PRINT_TIMESTAMP_MSG("[Power On]", "Write panel ID to EEPROM & reboot\n");
            WritePanelIndexToEeprom(FlashPanel_GetID());   // Write panel ID to EEPROM
            // FIXME
            // FIXME
            // Need to reboot ??
        }
    }
    #endif
    
    // Send LVDS output
    PRINT_TIMESTAMP_MSG("[Power On]", "LVDS_OUTPUT (Send LVDS output)\n");
    vDrvLVDSConfig();
    vDrvLVDSOn();
        
    // Send command : LVDS mute off to sub-micro
    // Module ID = 17, Command ID = 1, Input data = 0 (LVDS mute off)
    PRINT_TIMESTAMP_MSG("[Power On]", "SMicro command 4\n");
    u1LVDSMute = 0;
    UNUSED(IRIS_SMICRO_SEND_CMD(MODULEID_PANEL_PARAM, CMD17_SET_LVDS_MUTE,
                               &u1LVDSMute, 1, NULL, 0));

    // Wait TCON-RDY from sub-micro
    // Module ID = 17, command ID = 39, Input data = None
    // ACK is returned after detecting TCON_RDY=1
    PRINT_TIMESTAMP_MSG("[Power On]", "SMicro command 5\n");
    UNUSED(IRIS_SMICRO_SEND_CMD(MODULEID_PANEL_PARAM, CMD17_NOTIFY_TCON_READY,
                               NULL, 0, NULL, 0));

    x_thread_delay(_au2PowerSeqDelay[3]);

    // Check TCON_RDY
    Iris_CheckTCON_RDY();

    // Read Temp Sensor
    u4Value = Iris_GetTemperature();
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "Temp sensor = %d\n", u4Value);
    
    // Start dimmer output        
    Iris_StartDimmerOutput();

    x_thread_delay(_au2PowerSeqDelay[4]);
}

void Iris_PowerOnSequence(void)
{
    
#ifndef CC_PCB_DETECT_DISABLE
    
    // Read option from EEPROM to check if needs to force 120Hz panel setting
    // Should be removed if the final flow of panel ID control is done
    _fgIs120hzPanel = (_u4gPCBType == GPIO_PCB_TYPE_2) ? TRUE : FALSE;

#endif /* CC_PCB_DETECT_DISABLE */
    
    Iris_GetPowerSequenceDelay();
    vDrvLVDSOff();

    if (_fgIs120hzPanel)
    {
        Iris_120HzPowerOnSequence();
    }
    else
    {
        Iris_60HzPowerOnSequence();
    }

    // Turn on backlight
    PRINT_TIMESTAMP_DBG_MSG("[Power On]", "BL_ON = %d\n",
                            DRVCUST_PanelGet(eBackLightOnVal));
    vDrvTurnOnBacklt();

    // Read RGB sensor
    PRINT_TIMESTAMP_MSG("[Power On]", "RGB sensor = (none)\n");

#if defined(IRIS_PCBA_EVT) && !defined(CC_MTK_LOADER)
    IrisSelfDiagSetModelType(_u4gIrisModelType);
    
    // Start Self Diagnostic
    PRINT_TIMESTAMP_MSG("[Power On]", "Start Self Diagnostic\n");
    IrisSelfDiagStart(D_SYSTEM_PANEL_ONOFF);

    // Start AC detection
    PRINT_TIMESTAMP_MSG("[Power On]", "Start AC_DET\n");
    Iris_StartACDetec();
    
#endif /* defined(IRIS_PCBA_EVT) && !defined(CC_MTK_LOADER) */
}

void Iris_PowerOffSequence(void)
{
    INT32 i4Value;
    
#if defined(IRIS_PCBA_EVT) && !defined(CC_MTK_LOADER)

    // Stop Self Diagnostic
    PRINT_TIMESTAMP_MSG("[Power Off]", "Stop Self Diagnostic\n");
    IrisSelfDiagStop(D_SYSTEM_PANEL_ONOFF);

    // Stop AC detection
    PRINT_TIMESTAMP_MSG("[Power Off]", "Stop AC_DET\n");
    Iris_StopACDetec();
    
#endif /* defined(IRIS_PCBA_EVT) && !defined(CC_MTK_LOADER) */

    // backlight power -> lvds sugnal -> lvds power
    PRINT_TIMESTAMP_DBG_MSG("[Power Off]", "BL_ON = %d\n",
                            (INT32)!DRVCUST_PanelGet(eBackLightOnVal));
    vDrvTurnOffBacklt(); // Backlight off

    if (_fgIs120hzPanel)
    {
        // Set "U_POWER_READY" as LOW
        i4Value = 0;
        UNUSED(GPIO_Output(SIGNAL_POWER_ON_RDY, &i4Value));
        PRINT_TIMESTAMP_DBG_MSG("[Power Off]", "POWER_ON_RDY = %d\n", i4Value);
    }
    
    PRINT_TIMESTAMP_MSG("[Power Off]", "LVDS_OUTPUT = 0\n");
    vDrvLVDSOff();

    x_thread_delay(_au2PowerSeqDelay[10]);

    // Panel power power can not turn off right now
    // It will hang SIF bus if the panel power is down.
#ifdef IRIS_PCBA_EVT2
    vDrvTurnOffPanel(); // Panel off
#endif /* IRIS_PCBA_EVT2 */
    PRINT_TIMESTAMP_DBG_MSG("[Power Off]", "LVDS_PWR_ON = %d\n",
                            (INT32)!DRVCUST_PanelGet(ePanelPowerOnVal));
    
    i4Value = 0;
    UNUSED(GPIO_Output(SIGNAL_X_U_RST, &i4Value));
    PRINT_TIMESTAMP_DBG_MSG("[Power Off]", "X_U_RST = %d\n", i4Value);
}

UINT32 u4PanelStringCmp(CHAR *pSrcStr, CHAR *DesStr)
{
    // remove pending 0x20 and 0xff
    UINT32 u4DesLen = x_strlen(DesStr);
    UINT32 u4SrcLen = x_strlen(pSrcStr);
    UINT32 idx;

    u4DesLen = (u4DesLen>16)? 16: u4DesLen;
    u4SrcLen = (u4SrcLen>16)? 16: u4SrcLen;

    if ((u4DesLen < 1) || (u4SrcLen < 1))
    {
        return 1;
    }

    for (idx = u4DesLen-1; idx>0; idx--)
    {
        if ((*(DesStr+idx) == 0x20) || (*(DesStr+idx) == 0xff))
        {
            *(DesStr+idx) = '\0';
        }
    }

    for (idx = u4SrcLen-1; idx>0; idx--)
    {
        if ((*(pSrcStr+idx) == 0x20) || (*(pSrcStr+idx) == 0xff))
        {
            *(pSrcStr+idx) = '\0';
        }
    }

    return x_strcmp(DesStr, pSrcStr);
}

BOOL bIsFRCModel(void)
{
    return _fgIs120hzPanel;
}



