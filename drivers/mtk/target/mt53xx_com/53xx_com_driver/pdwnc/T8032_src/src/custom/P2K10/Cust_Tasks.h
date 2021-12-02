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
#ifndef _CUST_TASKS_H_
#define _CUST_TASKS_H_

void vWDTRebootProcess(void);
void vSysCustWakeupProcess(void);
void vSysCustEnterPowerDownProcess(void);
#if UART_TASK
extern void vRS232CustTaskInit(void);
extern void vRS232CustTask(void);
#endif

#if CUST_B_NEC_IR_DRIVER
	extern void vCUST_B_IrTask(IR_DATA_TYPE a);	//Add by Cust_B 20091215
	extern void vTask5_InitCUST_B_IrInit(void);	//Add by Cust_B 20091215
	extern void vTask5_CUST_B_IrStateMachine(void);//Add by Cust_B 20091215
	#define vTask5Init         vTask5_InitCUST_B_IrInit	//Add by Cust_B 20091215
	#define vTask5      vTask5_CUST_B_IrStateMachine	//Add by Cust_B 20091215
    #define vIR_Task5	vCUST_B_IrTask		//Add by Cust_B 20091215
    #define CUST_B_NEC_POWER 0xFF009F60	//Add by Cust_B 20091215
	#define CUST_B_NEC_MENU 0xED129F60	//Add by Cust_B 20091215
	#define CUST_B_NEC_OK 0xAE519F60		//Add by Cust_B 20091215
	#define CUST_B_NEC_INFO 0xBA459F60	//Add by Cust_B 20091215

typedef enum
{
	IR_COMBINATION_KEY1_MATCH = 0,
	IR_COMBINATION_KEY2_MATCH,
	IR_COMBINATION_KEY3_MATCH,
	IR_COMBINATION_KEY4_MATCH,
	IR_COMBINATION_NoMatch
} CUST_B_IR_COMBINATION_KEY;	

typedef enum
{
	IR_TIMER_STAUTS_OFF = 0,
	IR_TIMER_STAUTS_ON	
} CUST_B_IR_TIMER_STAUTS;	
#endif	
#if CUS_SH_HOTEL_MODE 
#define PDWNC_CMD_ARMT8032_CUST_SH_CUSTDATA  (Cmd_Turnkey_Max + 1)
#endif

#if CUST_P_SX_SUPPORT
#define PDWNC_CMD_ARMT8032_P_COMPANY_BDSDATA  (Cmd_Turnkey_Max + 2)
#endif
#if P_ERROR_LED
#define PDWNC_CMD_ARMT8032_P_COMPANY_ERRLED_SET  (Cmd_Turnkey_Max + 3)
    #define Cmd_Sub_ClearErrBuf 0xff
#define PDWNC_CMD_ARMT8032_P_COMPANY_ERRLED_EN   (Cmd_Turnkey_Max + 4)
    #define Cmd_Sub_EnableErrCode 0x01
    #define Cmd_Sub_DisableErrCode 0x02    
#endif

#if RC56_IR_DRIVER
    extern void vTask1_InitCust_P_IrInit(void);
    extern void vTask1_Cust_P_IrStateMachine(void);
    extern void vCUST_P_IrTask(IR_DATA_TYPE a);
    extern UINT8 XferToColorCfg(UINT32 u4SysCode);
    #define vTask1Init         vTask1_InitCust_P_IrInit
    #define vTask1              vTask1_Cust_P_IrStateMachine
    #define vIR_Task1       vCUST_P_IrTask
    #define vTask1WakeupProcess vTask1_InitCust_P_IrInit
    #define vTask1StandbyProcess    vTask1_InitCust_P_IrInit
    
    #define CUST_P_RC6_POWER 0x03980000

    typedef enum
    {
        IR_DIGIT0_IDX = 0,
        IR_DIGIT1_IDX ,    	
        IR_DIGIT2_IDX ,    	
        IR_DIGIT3_IDX ,    	
        IR_DIGIT4_IDX ,    	
        IR_DIGIT5_IDX,
        IR_DIGIT6_IDX ,    	
        IR_DIGIT7_IDX ,    	
        IR_DIGIT8_IDX ,    	
        IR_DIGIT9_IDX,
        IR_MUTE_IDX ,    	
        IR_OK_IDX ,    	
        IR_POWER_IDX ,    	
        IR_AMBILIGHT_IDX,
        IR_CH_UP_IDX ,    	
        IR_CH_DOWN_IDX ,    	
        IR_CH_PREVCH_IDX,
        IR_CH_SOURCE_IDX    
    }CUST_P_IR_KEY_IDX;


    typedef struct 
    {
        IR_DATA_TYPE u4PreviousPowerCode;
        IR_DATA_TYPE u4CurrentPowerCode;
        UINT8 fgIR_FilterFlag;
        UINT8 u1LPIR_CHK;
    } LPIR_struct;
    extern LPIR_struct gLPIR;
    #define IR_BLUE_CFG 		0x01
    #define IR_YELLOW_CFG 	0x02
    #define IR_RED_CFG 		0x04
    #define IR_GREEN_CFG 	0x08

    #define RC6_KEY_MUTE 		0x03D80000
    #define RC6_KEY_DIGIT0		0x03800000
    #define RC6_KEY_DIGIT1		0x03C00000
    #define RC6_KEY_DIGIT2		0x03A00000
    #define RC6_KEY_DIGIT3		0x03E00000
    #define RC6_KEY_DIGIT4		0x03900000
    #define RC6_KEY_DIGIT5		0x03D00000
    #define RC6_KEY_DIGIT6		0x03B00000
    #define RC6_KEY_DIGIT7		0x03F00000
    #define RC6_KEY_DIGIT8		0x03880000
    #define RC6_KEY_DIGIT9		0x03C80000
    #define RC6_KEY_OK			0x039D0000
    #define RC6_KEY_POWER		0x03980000
    #define RC6_KEY_AMBILIGHT	0x03f88000  // raw key.
    #define RC6_KEY_CH_UP       0x03820000
    #define RC6_KEY_CH_DOWN     0x03C20000
    #define RC6_KEY_PREVCH      0x03A80000
    #define RC6_KEY_SOURCE      0x038E0000
#endif

#if CUST_P_DC_ON_SEQUENCE
extern void vTask2Init_CUST_P_dc_on_state_machine_init(void);
extern void vTask2_CUST_P_dc_on_state_machine(void);
#define vTask2Init          vTask2Init_CUST_P_dc_on_state_machine_init
#define vTask2               vTask2_CUST_P_dc_on_state_machine    
#define vTask2StandbyProcess               vTask2Init_CUST_P_dc_on_state_machine_init    
#endif    

#if SUPORT_COLD_REBOOT
#define vTask3Init          vTask3Init_cold_rebootInit
#define vTask3               vTask3_cold_reboot_state_machine    
extern void vTask3Init_cold_rebootInit(void);
extern void vTask3_cold_reboot_state_machine(void);
#endif


#if SUPPORT_ERROR_LED_CTRL
typedef struct _ErrorCodeCtrl
{
    UINT8 fgErrCodeLedPeriod;
    UINT8 fgErrCodeLedInterval;
    UINT8 fgErrCodeBlinkingTimeCnt; // 0: disable, non-0: blinking times
    UINT8 fgErrCodeBlinkingTime; // 0: disable, non-0: blinking times
    UINT8 fgErrCodeLedType; // 1: Servo 0: Opctrl
    UINT32 fgErrCodeLedMsk;
    UINT8 fgErrCodeLedPolarity;
    UINT8 fgS_compErrLedStateMachine;
  }Error_Code_Ctrl;


typedef enum
{
    CUST_S_ERROR_CODE_STATE_1,
    CUST_S_ERROR_CODE_STATE_2,
    CUST_S_ERROR_CODE_STATE_3,
    CUST_S_ERROR_CODE_STATE_4
}Error_Code_State;

extern void vTask4Init_error_led_ctrl(void);
extern void vTask4_error_led_ctrl_state_machine(void);
extern void vTask4WakeupProcess_error_led_ctrl(void);
extern void vTask4StandbyProcess_error_led_ctrl(void);
extern void vCust_S_ErrCodeLedReset(void);
extern void SetCtrlLedEnable(void);
#define vTask4Init      vTask4Init_error_led_ctrl
#define vTask4          vTask4_error_led_ctrl_state_machine
#define vTask4WakeupProcess vTask4WakeupProcess_error_led_ctrl
#define vTask4StandbyProcess vTask4StandbyProcess_error_led_ctrl
#endif


#if P_ERROR_LED
typedef struct _PErrorCodeCtrl
{
    UINT8 fgErrCodeLedOn;
    UINT8 LED_Status;
    UINT8 LED_ptr;
    UINT8 LED_cycle;
    UINT8 LED_Array[5];
    UINT8 fgErrCodeLedType; // 1: Servo 0: Opctrl
    UINT32 fgErrCodeLedMsk;
    UINT8 fgErrCodeLedPolarity;
    //    UINT8 gErrLedCnt;
    //    UINT8 gErrLedOffCnt;
    //    UINT8 LED_Cnt;
//    UINT8 fLED_TmrOut;    
//    UINT8 u1ErrCodeLedTimerTick;    
//    UINT8 fgSonyErrLedStateMachine;
  }PError_Code_Ctrl;


typedef enum
{  	_ErrCode_End =0,
	_ErrCode_Start,
	_ErrCode_Byte10,
	_ErrCode_NibbleGap,
	_ErrCode_Byte01,
	_ErrCode_ByteGap,
	_ErrCode_Stop1,
	_ErrCode_Stop2
}PError_Code_State;

#define	LEDcodeMax	5
#define CNT_250ms   5
#define CNT_750ms   15
#define CNT_1500ms  30
#define CNT_3000ms  60
#define CNT_5000ms  100

extern void vTask5_P_Init_error_led_ctrl(void);
extern void vTask5_P_error_led_ctrl_state_machine(void);
extern void vTask5_P_WakeupProcess_error_led_ctrl(void);
extern void vTask5_P_StandbyProcess_error_led_ctrl(void);
extern void vPErrCodeLedReset(void);
extern void vPSetCtrlLedEnable(void);
#define vTask5Init      vTask5_P_Init_error_led_ctrl
#define vTask5          vTask5_P_error_led_ctrl_state_machine
#define vTask5WakeupProcess vTask5_P_WakeupProcess_error_led_ctrl
#define vTask5StandbyProcess vTask5_P_StandbyProcess_error_led_ctrl
#endif

enum 
{
    TEST_TIMER_ID,
    #if RC56_IR_DRIVER	    
        LPIR_SHORT_TIMER_ID,
        LPIR_LONG_TIMER_ID,
    #endif
    #if CUST_P_DC_ON_SEQUENCE
        DC_ON_Timer_Cnt,
    #endif
    #if SUPORT_COLD_REBOOT    
        COLD_REBOOT_TIMER_ID,
    #endif
#if SUPPORT_ERROR_LED_CTRL
        ERROR_LED_TIMER_ID,
#endif
#if CUST_B_NEC_IR_DRIVER      //Add by Cust_B 20091215
        POWER_ON_MUTIL_KEY_TIMER_ID,
#endif
#if P_ERROR_LED
        P_ERR_LED_TIMER_ID,
#endif
	MAX_TIMER_ID
};





#endif

