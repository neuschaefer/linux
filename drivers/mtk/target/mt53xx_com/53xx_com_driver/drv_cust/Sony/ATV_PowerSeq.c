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
#include "SMicro/SMicro_prc.h"
#include "SMicro/SMicro_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define CUSTOMER_POWER_ON_SEQUENCE          ATV_PowerOnSequence
#define CUSTOMER_POWER_OFF_SEQUENCE         ATV_PowerOffSequence


//Hua 20090723 for ATV  CC_CVT PCB
// Signal gpio definition
#define SIGNAL_X_U_RST                      GPIO(27) //PTSI_D3        
#define SIGNAL_TCON_RDY                     GPIO(0)  //GPIO0            
#define SIGNAL_POWER_ON_RDY                 GPIO(22) //PTSI_VALID         
//#define SIGNAL_BUS_INT                                   //64              //SPEC not sepcific 
//#define SIGNAL_ADIN4_SRV                              //ADIN(4)      //SPEC not sepcific   //not use yet


// EEPROM information
#define DC_DETECT_THRES_OFFSET              100

// Default value
#define SERVO_VALID_MAX                     63
#define DC_DETECT_THRES_DEFAULT             0x1f

// Detect time period definition
#define IRIS_POWER_DETECT_PERIOD            10  // ms
#define DC_DETECT_TIMES                     40
#define TCON_READY_DETECT_TIMES             3000


//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------
static void ATV_PowerOnSequence(void);
static void ATV_PowerOffSequence(void);
UINT32 u4PanelStringCmp(CHAR *pSrcStr, CHAR *DesStr);


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
//static BOOL _fgIs120hzPanel = FALSE;

static BOOL _fgIs100hzPanel = FALSE;//Hua 20090723 for ATV

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
static void ATV_PowerOnSequence(void)
{
#ifndef CC_AC_DET_DROP_DISABLE
#if 0//marked by Hua ,Not ready to use.//Hua 20090723 for ATV
    UINT8 u1Val = 0;
#endif
#endif

#ifdef CC_DRV_SUPPORT_SMICRO
	_fgIs100hzPanel = TRUE;//Hua 20090723 for ATV
#endif

    UINT32 u4TimeCount = 0;
    INT32 i4Value;
    
    vDrvLVDSOff();
	
    i4Value = 1;
    UNUSED(GPIO_Output(SIGNAL_X_U_RST, &i4Value));//GPIO 27 for ATV //Hua 20090723 for ATV
	

    // Set "U_POWER_READY" as HIGH
    UNUSED(GPIO_Output(SIGNAL_POWER_ON_RDY, &i4Value));

    // Read p_sel
    // TBD ....

    // DC detection
//    if (EEPROM_Read(DC_DETECT_THRES_OFFSET, (UINT32)(void *)&u1Val, 1) == 0)
#ifndef CC_AC_DET_DROP_DISABLE
#if 0//marked by Hua ,Not ready to use.//Hua 20090723 for ATV
    {
        UINT32 u4ADCThreshold = (UINT32)u1Val;
        UINT32 u4POWER_DWN;
        UINT32 u4DetectTimes = 0;

        Printf("[ATV_PowerOnSequence] Read DC_DETECT_THRES_OFFSET from EEPROM = %d\n", u1Val);
//        u4ADCThreshold = ((UINT32)u1Val > SERVO_VALID_MAX) ? 
//                         DC_DETECT_THRES_DEFAULT : (UINT32)u1Val;
        u4ADCThreshold = DC_DETECT_THRES_DEFAULT;
        while (u4DetectTimes < DC_DETECT_TIMES)
        {
            u4POWER_DWN = PDWNC_ReadServoADCChannelValue(SIGNAL_ADIN4_SRV);
            if (u4POWER_DWN >= u4ADCThreshold)
            {
                Printf("[ATV_PowerOnSequence] SIGNAL_ADIN4_SRV = %d, u4DetectTimes = %d\n",
                       u4POWER_DWN, u4DetectTimes);
                u4DetectTimes++;
            }
            else
            {
                Printf("[ATV_PowerOnSequence] SIGNAL_ADIN4_SRV = %d, error\n",
                       u4POWER_DWN);
                u4DetectTimes = 0;
            }
            x_thread_delay(IRIS_POWER_DETECT_PERIOD);
        }
    }
#endif
#endif    
    x_thread_delay(10);
    
    // normal power sequence, lvds power -> lvds signal -> backlight power
    vDrvTurnOnPanel();
    x_thread_delay(50);

    // the magic 1 makes this delay > 10 * PANEL_GetLvdsOnDalay()
    // otherwise it will less than 10 * PANEL_GetLvdsOnDalay()
    x_thread_delay((10 * PANEL_GetLvdsOnDalay()) + 1);
        
    if (_fgIs100hzPanel)
    {
#ifndef CC_MTK_LOADER
        UINT8 u1LVDSMute;
#endif        

        // Read panel ID
        // TBD

        // Check Panel ID
        // TBD

        // Send LVDS output
        vDrvLVDSConfig();
        vDrvLVDSOn();

		
        // Send command : LVDS mute off to sub-micro
        // Module ID = 17, Command ID = 1, Input data = 0 (LVDS mute off)
        
        // TBD

        // Wait TCON-RDY from sub-micro
        // Module ID = 17, command ID = 39, Input data = None
        // ACK is returned after detecting TCON_RDY=1
        
        // TBD
		i4Value = 1;//Hua 20090723 for ATV
		UNUSED(GPIO_Output(SIGNAL_X_U_RST, &i4Value));//GPIO 27 for ATV //Hua 20090723 for ATV
		//printf("\n***********pull low finised!************\n");//Hua 20090723 for ATV

#ifndef CC_MTK_LOADER//Hua 20090723 for ATV
		UNUSED(u4SMicroHWInit());

		// Send command : LVDS mute off to sub-micro
		// Module ID = 17, Command ID = 1, Input data = 0 (LVDS mute off)
		u1LVDSMute = 0;
		UNUSED(u4SMicroSendCmd(MODULEID_PANEL_PARAM, CMD17_SET_LVDS_MUTE,
							   &u1LVDSMute, 1, NULL, 0));

		// Wait TCON-RDY from sub-micro
		// Module ID = 17, command ID = 39, Input data = None
		// ACK is returned after detecting TCON_RDY=1
		UNUSED(u4SMicroSendCmd(MODULEID_PANEL_PARAM, CMD17_NOTIFY_TCON_READY,
							   NULL, 0, NULL, 0));
#endif
        x_thread_delay(300);
    }
    else
    {
        i4Value = 1;
        //UNUSED(GPIO_Output(SIGNAL_BUS_INT, &i4Value)); //ATV marked by Hua
        
        // Read panel ID
        // TBD

        // Check Panel ID
        // TBD

        // Send LVDS output
        vDrvLVDSConfig();
        vDrvLVDSOn();
//        x_thread_delay(10 * PANEL_GetBacklightOnDelay());

        x_thread_delay(200);
    }

    // Check TCON-RDY signal
    // If (TCON-RDY = 0) continues 3 seconds, 
    // enter the safty shutdown (self diagnostics)
    u4TimeCount = 0;
    do
    {
        if (GPIO_Input(SIGNAL_TCON_RDY) == 1)   break;

        x_thread_delay(IRIS_POWER_DETECT_PERIOD);
        u4TimeCount += IRIS_POWER_DETECT_PERIOD;
    } while (u4TimeCount < TCON_READY_DETECT_TIMES);

    if (u4TimeCount >= TCON_READY_DETECT_TIMES)
    {
        Printf("[ATV_PowerOnSequence] SIGNAL_TCON_RDY is not ready over %d ms\n", u4TimeCount); 
        
        // go to self diagnostics
//        return;
    }

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

    x_thread_delay(30);

    if (_fgIs100hzPanel)
    {
        x_thread_delay(500);
    }
    else
    {
        // Set DCC_LUT0, DCC_LUT1, DCC_LUT2 -> for 60Hz(FHD)
        // DCC_LUT0 -> HDMI_SDA0
        // DCC_LUT1 -> HDMI_SCL0
        // DCC_LUT2 -> HDMI_SCL1
        
        x_thread_delay(200);
    }

    // Turn on backlight
    vDrvTurnOnBacklt();
}

static void ATV_PowerOffSequence(void)
{
    INT32 i4Value;
    
    // backlight power -> lvds sugnal -> lvds power
    vDrvTurnOffBacklt(); // Backlight off
    x_thread_delay(10 * PANEL_GetBacklightOffDelay());

    if (_fgIs100hzPanel)
    {
        // Set "U_POWER_READY" as LOW
        i4Value = 0;
        UNUSED(GPIO_Output(SIGNAL_POWER_ON_RDY, &i4Value));		
    }
    
    vDrvLVDSOff();

    x_thread_delay(10 * PANEL_GetLvdsOffDalay());
	
    if (_fgIs100hzPanel)//GPIO 27 for ATV //Hua 20090723 for ATV
    {
		i4Value = 0;
		UNUSED(GPIO_Output(SIGNAL_X_U_RST, &i4Value));//GPIO 27 for ATV //Hua 20090723 for ATV
    }
    // Panel power power can not turn off right now
    // It will hang SIF bus if the panel power is down.
//    vDrvTurnOffPanel(); // Panel off
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
    return _fgIs100hzPanel;
}



