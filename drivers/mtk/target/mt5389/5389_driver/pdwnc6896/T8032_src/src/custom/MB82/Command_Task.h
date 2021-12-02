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

#ifndef _COMMAND_TASK_H_
#define _COMMAND_TASK_H_

#include "custom_def.h"

//Command ARM to T8032
#define Cmd_EnterPowerDown       0x01
#define Cmd_EnterPowerNormal    0x02
	#define Cmd_Sub_PowerStage_loader         0x01
    	#define Cmd_Sub_PowerStage_normal      0x02
#define Cmd_QuerySwVersion        0x03
#define Cmd_EnterXdataSwitch      0x04
    #define Cmd_Sub_Xdata_ARM         0x01
    #define Cmd_Sub_Xdata_T8032      0x02
#define Cmd_ReadT8032WakeUp   0x05
#define Cmd_SetLightGuideLevel   0x06
#define Cmd_AliveCheck		      0x07
#define Cmd_SetClearErrCode	      0x08
	#define Cmd_Sub_ClearErrBuf	      0xFF
#define Cmd_EnableDisableErrCode 0x09
	#define Cmd_Sub_EnableErrCode	      0x01
	#define Cmd_Sub_DisableErrCode	      0x02
//CEC command
#define Cmd_CEC_MenuLanguage 	0x0a
#define Cmd_CEC_EnableCEC	 	0x0b
#define Cmd_CEC_EnableOneTouch	0x0c
#define Cmd_SetLEDBlink			0x0d
#define Cmd_SetFWUpgrate		0x0e
#define Cmd_SetPowerDownJob	0x0f
#define Cmd_ClrPowerDownJob	0x10

#define Cmd_ColdReboot			0x11
#define Cmd_DigitKeyWakeup		0x12

#define Cmd_SetCustData	0x13

#define Cmd_SetBaudrate	0x16

#if 1//CUS_S_MULTI_KEY_ECO
#define Cmd_QueryServiceMode	0x40
#endif
#if RC56_IR_DRIVER
#define Cmd_SetClrIR_FILTER		0x41
	#define Cmd_Sub_SetIR_FilterSysCode	      0x01
	#define Cmd_Sub_ClrIR_FilterSysCode	      0x02
#endif
#define Cmd_CEC_SupportUserControlPressed 0x50
#define Cmd_CEC_SetVendorID 0x51
#define Cmd_CEC_SetVersion 	0x52
#define Cmd_CEC_SetOSDName1 0x53
#define Cmd_CEC_SetOSDName2 0x54
#define Cmd_CEC_SetOSDName3 0x55
#define Cmd_CEC_SetOSDName4 0x56
#define Cmd_CEC_SetWakeupOpCode1	0x57
#define Cmd_CEC_SetWakeupOpCode2	0x58
#define Cmd_CEC_SetWakeupOpCode3	0x59
#define Cmd_CEC_SetT8032CtlCEC		0x5a

#if SIF_PDWNC_TEST
#define    Cmd_PDSIF_READ	  0x60
#define    Cmd_PDSIF_WRITE	  0x61
#define    Cmd_PDSIF_EEP_READ	  0x62
#define    Cmd_PDSIF_EEP_WRITE	  0x63
#endif

#define Cmd_GPIOOutputEnable 	 0x80
#define Cmd_GPIOOutput			 0x81
#define Cmd_GPIOInput			 0x82
#define Cmd_ServoOutputEnable 	 0x83
#define Cmd_ServoOutput			 0x84
#define Cmd_ServoInput			 0x85
#define Cmd_OtherEngineSwitch	 0x86
	#define Cmd_Sub_OtherEngine_ARM         0x01
	#define Cmd_Sub_OtherEngine_T8032      0x02
#define Cmd_Turnkey_Max	 0x87

//#define Cmd_QueryIRPowerKeyState            0x99
//#define Cmd_QueryIRPowerKeyStateOK	 0x99

//Command T8032 to ARM
#define Cmd_EnterPowerNormalReady    0x02
#define Cmd_ResponseSWVersion            0x03
#define Cmd_XdataSwitchOutReady         0x04
#define Cmd_ResponseWakeupReason    0x05
    #define Cmd_Sub_WakeUP_CEC         	0x01
    #define Cmd_Sub_WakeUP_ColdReboot    0x02
#define Cmd_SetLightGuideLevelOK         0x06
#define Cmd_T8032Alive			         0x07
	#define Cmd_Sub_T8032AliveInitializing	0x00
	#define Cmd_Sub_T8032AliveReady		0x01
#define Cmd_SetClearErrCodeOK		  0x08
#define Cmd_EnableDisableErrCodeOK    0x09
#define Cmd_ResponseCEC_MenuLanguage 	0x0a
#define Cmd_ResponseCEC_EnableCEC	 	0x0b
#define Cmd_ResponseCEC_EnableOneTouch	0x0c
#define Cmd_SetLEDBlinkOK	0x0d
#define Cmd_SetFWUpgrateOK	0x0e
#define Cmd_SetPowerDownJobOK	0x0f
#define Cmd_ClrPowerDownJobOK	0x10

//#define Cmd_ColdRebootOK			0x11 //cold reboot doesn't need response
#define Cmd_DigitKeyWakeupOK		0x12
#define Cmd_ResponseSetCustData	0x13
#if 1//CUS_S_MULTI_KEY_ECO
#define Cmd_responseServiceMode	0x40
#endif

#define Cmd_ResponseCEC_SupportUserControlPressed 	0x50
#define Cmd_ResponseCEC_SetVendorID 	0x51
#define Cmd_ResponseCEC_SetVersion 		0x52
#define Cmd_ResponseCEC_SetOSDName1 	0x53
#define Cmd_ResponseCEC_SetOSDName2 	0x54
#define Cmd_ResponseCEC_SetOSDName3 	0x55
#define Cmd_ResponseCEC_SetOSDName4 	0x56
#define Cmd_ResponseCEC_SetWakeupOpCode1	0x57
#define Cmd_ResponseCEC_SetWakeupOpCode2	0x58
#define Cmd_ResponseCEC_SetWakeupOpCode3	0x59
#define Cmd_ResponseCEC_SetT8032CtlCEC		0x5a

#define Cmd_ResponseGPIOOutputEnable 	 0x80
#define Cmd_ResponseGPIOOutput			 0x81
#define Cmd_ResponseGPIOInput			 0x82
#define Cmd_ResponseServoOutputEnable 	 0x83
#define Cmd_ResponseServoOutput		 0x84
#define Cmd_ResponseServoInput			 0x85
#define Cmd_ResponseOtherEngineSwitch	 0x86
#define Cmd_ARM_T8032_GetIROKPRESSEDSTA 0x87
#define Cmd_ARM_T8032_SetIROKPRESSEDHOOKORNOT 0x88
#define Cmd_ARM_T8032_GetLED_STATUS 0x89
#define Cmd_ARM_T8032_SetLED    0x8a
#define Cmd_ARM_T8032_SetSERVADC 0x8b
#define Cmd_ARM_T8032_SetScartWakeUp 0x8c
#define Cmd_ARM_T8032_GetSERVADC 0x8d

#if 0// SUPORT_DIGIT_KEY_WAKEUP
#define NEC_DIGIT_0 0xea15e31c
#define NEC_DIGIT_1 0xf50ae31c
#define NEC_DIGIT_2 0xf609e31c
#define NEC_DIGIT_3 0xf708e31c
#define NEC_DIGIT_4 0xed12e31c
#define NEC_DIGIT_5 0xee11e31c
#define NEC_DIGIT_6 0xef10e31c
#define NEC_DIGIT_7 0xf10ee31c
#define NEC_DIGIT_8 0xf20de31c
#define NEC_DIGIT_9 0xf30ce31c
#endif

#if 0
extern UINT8 fgStandbyMode;
extern UINT8 fgPreStandbyStatus;    // 1:from AC on, 2:from Normal mode
extern UINT8 fgSWVersionQurey; 
extern UINT8 fgPreparepowerNormal;
extern UINT8 fgXdataSwitch;  //0: Xdata is by ARM, 1: preparing to switch, 2: Xdata is by T8032
extern UINT8 fgWakeupReason;
#endif
#if IR_DEBOUNCE
extern UINT8 u1IRDebounceTime; 
#endif
#if LIGHT_GUIDE
extern UINT8 fgSetLightGuideLevel;
#endif
#if 0
extern UINT8 fgAliveCheck;
#endif


#if ENABLE_CEC
extern UINT8 fgCECMenuLanguage;
extern UINT8 fgCECEnableCEC;
extern UINT8 fgCECEnableOneTouchPlay;
extern UINT8 fgCECT8032CtlCEC;
extern UINT8 fgCECSetVendorID;
extern UINT8 fgCECSetVersion;
#ifdef CEC_FEATURE_3_1 
extern UINT8 fgCECSetOSDName1;
extern UINT8 fgCECSetOSDName2;
extern UINT8 fgCECSetOSDName3;
extern UINT8 fgCECSetOSDName4;
extern UINT8 fgCECSetWkpCode1;
extern UINT8 fgCECSetWkpCode2;
extern UINT8 fgCECSetWkpCode3;
#endif
#endif
#if GPIO_CTRL_SUPPORT
extern UINT8 fgGPIOEnable;
extern UINT8 fgGPIOOUT;
extern UINT8 fgGPIOIN;
#endif
#if SERVO_CTRL_SUPPORT
extern UINT8 fgServoADEnable;
extern UINT8 fgServoADOUT;
extern UINT8 fgServoADIN;
#endif
extern UINT8 fgOtherEngineSwitch;
extern UINT8 bOtherEngine;
#if UART_PD_RX_EN
extern UINT8 xdata uRs232Data[32];
extern UINT8 xdata uRs232Rxidx;
extern UINT8 xdata fgRs232Rx;
#endif

#if IR_MULTIKEY_WAKEUP
extern UINT8 xdata u1IRkeyBuf[5];
extern UINT8 xdata u1IRBufIdx;
#endif
#if ERR_DATA_RESPONSE
extern UINT8 fgErrorDataResponse;
#endif


#if SUPORT_DIGIT_KEY_WAKEUP
extern UINT8 fgWakeUpDigitKey;
extern UINT8 u1WakeUpDigit;
#endif
extern UINT8 xdata u1WakeupReason;
extern UINT8 fgTrigARMIntEn;


extern void vCommandTaskInit(void);
extern void vCommandTask(void);
extern void vCMD_RESONSE(UINT8 u1Cmd, UINT8 u1Para0, UINT8 u1Para1, UINT8 u1Para2);
#if 0
typedef union DATA_FROM_ARM
{
       struct DATA_FROM_ARM_BYTE
	{
            UINT8 byte0;
            UINT8 byte1;
            UINT8 byte2;
            UINT8 byte3;            
        }BYTES;

    UINT32 u4Value;
}DATA_FROM_ARM;
#endif

#endif //_COMMAND_TASK_H_
