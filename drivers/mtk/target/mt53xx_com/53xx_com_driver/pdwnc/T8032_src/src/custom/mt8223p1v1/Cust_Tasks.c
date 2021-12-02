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
#include "general.h"
//#include "drv_hdmicec.h"
void vWDTRebootProcess(void)
{
     LogS("  WDT");
 }


#if GPIO_INTERRUPT_ENABLE
void vCustGpioIsr(UINT32 u4IntStatus)
{

}
#endif

#if RC56_IR_DRIVER
#define MRC56_TOGGLE_BIT_MASK(x) ((x) & 0xffffff00)
#define fgIsTimerAtInitialState(x) fgIsTimerUp(x)
LPIR_struct gLPIR;
UINT8 RC6_B2_KEY[18];/* = {0x80,0xC0,0xA0,0xE0,0x90,0xD0,0xB0,0xF0,0x88,0xC8,0xD8,0x9D,0x98,0xf8,0x82,0xc2,0xa8,0x8e};*/
UINT8 RC6_B1_KEY[4];/* = {0x0, 0x4C, 0x24,0x58};*/
UINT16 RC5_B2B3_KEY[18];/* = {0x0eaa,0x1daa,0x0e6a,0x1d6a,0x0e9a,0x1d9a,0x0e5a,0x1d5a,0x0ea6,0x1da6,0x1d96,0x0eac,0x0e96,0x1d6a,0x0eaa,0x1daa,0x0e6a,0x0ea5};*/
UINT8 RC5_BLUE_B1_KEY[18];/* = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xD5,0xAA,0x56,0x6a,0x6a,0x6a,0x6a};*/
UINT8 SYS7_B1_KEY[18];/* = 		{0x95,0x95,0x95,0x95,0x95,0x95,0x95,0x95,0x95,0x95,0x95,0xff,0x95,0xff,0x55,0x55,0x55,0x6a};*/


//David: need to copy DATA segment by ourself...
void InitIR_KeyMap(void)
{		
/*
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
    IR_CH_SOURCE_IDX ==>17    
}CUST_P_IR_KEY_IDX;

*/    
	SYS7_B1_KEY[0] = 0x95;
	SYS7_B1_KEY[1] = 0x95;
	SYS7_B1_KEY[2] = 0x95;
	SYS7_B1_KEY[3] = 0x95;
	SYS7_B1_KEY[4] = 0x95;
	SYS7_B1_KEY[5] = 0x95;
	SYS7_B1_KEY[6] = 0x95;
	SYS7_B1_KEY[7] = 0x95;
	SYS7_B1_KEY[8] = 0x95;
	SYS7_B1_KEY[9] = 0x95;
	SYS7_B1_KEY[10] = 0x95;
	SYS7_B1_KEY[11] = 0xFF;
	SYS7_B1_KEY[12] = 0x95;
	SYS7_B1_KEY[13] = 0xFF;
	SYS7_B1_KEY[14] = 0x55;
	SYS7_B1_KEY[15] = 0x55;
	SYS7_B1_KEY[16] = 0x55;
	SYS7_B1_KEY[17] = 0x6A;
	
	RC5_BLUE_B1_KEY[0] = 0xAA;
	RC5_BLUE_B1_KEY[1] = 0xAA;
	RC5_BLUE_B1_KEY[2] = 0xAA;
	RC5_BLUE_B1_KEY[3] = 0xAA;
	RC5_BLUE_B1_KEY[4] = 0xAA;
	RC5_BLUE_B1_KEY[5] = 0xAA;
	RC5_BLUE_B1_KEY[6] = 0xAA;
	RC5_BLUE_B1_KEY[7] = 0xAA;
	RC5_BLUE_B1_KEY[8] = 0xAA;
	RC5_BLUE_B1_KEY[9] = 0xAA;
	RC5_BLUE_B1_KEY[10] = 0xAA;
	RC5_BLUE_B1_KEY[11] = 0xD5;
	RC5_BLUE_B1_KEY[12] = 0xAA;
	RC5_BLUE_B1_KEY[13] = 0x56;
	RC5_BLUE_B1_KEY[14] = 0x6A;
	RC5_BLUE_B1_KEY[15] = 0x6A;
	RC5_BLUE_B1_KEY[16] = 0x6A;
	RC5_BLUE_B1_KEY[17] = 0x6A;

	RC5_B2B3_KEY[0] = 0x0EAA;
	RC5_B2B3_KEY[1] = 0x1DAA;
	RC5_B2B3_KEY[2] = 0x0E6A;
	RC5_B2B3_KEY[3] = 0x1D6A;
	RC5_B2B3_KEY[4] = 0x0E9A;
	RC5_B2B3_KEY[5] = 0x1D9A;
	RC5_B2B3_KEY[6] = 0x0E5A;
	RC5_B2B3_KEY[7] = 0x1D5A;
	RC5_B2B3_KEY[8] = 0x0EA6;
	RC5_B2B3_KEY[9] = 0x1DA6;
	RC5_B2B3_KEY[10] = 0x1D96;
	RC5_B2B3_KEY[11] = 0x0EAC;
	RC5_B2B3_KEY[12] = 0x0E96;
	RC5_B2B3_KEY[13] = 0x1D6A;
	RC5_B2B3_KEY[14] = 0x0EAA;
	RC5_B2B3_KEY[15] = 0x1DAA;
	RC5_B2B3_KEY[16] = 0x0E6A;	
	RC5_B2B3_KEY[17] = 0x0EA5;

	RC6_B2_KEY[0] = 0x80;
	RC6_B2_KEY[1] = 0xC0;
	RC6_B2_KEY[2] = 0xA0;
	RC6_B2_KEY[3] = 0xE0;
	RC6_B2_KEY[4] = 0x90;
	RC6_B2_KEY[5] = 0xD0;
	RC6_B2_KEY[6] = 0xB0;
	RC6_B2_KEY[7] = 0xF0;
	RC6_B2_KEY[8] = 0x88;
	RC6_B2_KEY[9] = 0xC8;
	RC6_B2_KEY[10] = 0xD8;
	RC6_B2_KEY[11] = 0x9D;
	RC6_B2_KEY[12] = 0x98;
	RC6_B2_KEY[13] = 0xF8;
	RC6_B2_KEY[14] = 0x82;
	RC6_B2_KEY[15] = 0xC2;
	RC6_B2_KEY[16] = 0xA8;
	RC6_B2_KEY[17] = 0x8E;

	RC6_B1_KEY[0] = 0x0;
	RC6_B1_KEY[1] = 0x4C;
	RC6_B1_KEY[2] = 0x24;
	RC6_B1_KEY[3] = 0x58;
}
UINT8 GetColorIdx(UINT8 flag)
{
	switch(flag)
	{
		case IR_BLUE_CFG:
			return 0;
		case IR_YELLOW_CFG:			
			return 1;		
		case IR_RED_CFG:
			return 2;
		case IR_GREEN_CFG:
			return 3;
		default:
			break;
	}
	return 0;
}

UINT8 XferToColorCfg(UINT32 u4SysCode)
{
	switch(u4SysCode)
	{
		case 0:
		case 3:
			return IR_BLUE_CFG;
		case 11:
		case 14:			
		case 25:
			return IR_YELLOW_CFG;
		case 4:
		case 12:			
		case 18:
			return IR_RED_CFG;
		case 13:
		case 31:			
			return IR_GREEN_CFG;
			
		default:
			break;
	}
	return 0;
}

UINT8 Check_IR_Key(UINT32 u4Code, UINT8 key_map_idx)
{
	UINT32 u4tmp;
	UINT8 u1color_idx;
	UINT16 rc5_b2b3;
// For Aambilight button... there is not a rule between the raw code of each color
/*
	if(key_map_idx == IR_AMBILIGHT_IDX)
		return IS_KEY_AMBILIGHT(u4Code);
*/	
//Since we have the BYTE1 information of P_COMP RC6 yellow/red/green, RC6 is support MULIT RC now			
	u1color_idx = GetColorIdx(gLPIR.fgIR_FilterFlag);	
	u4tmp = 0x03000000 | ((UINT32)(RC6_B2_KEY[key_map_idx]) << 16) | (((UINT32)RC6_B1_KEY[u1color_idx]) << 8);	
        #if 0// UART_DBG
        if(key_map_idx == IR_POWER_IDX)
        {
            LogSD("Comp", u4tmp);
            LogD(u4Code);
            LogB(RC6_B2_KEY[key_map_idx]);
            LogB(RC6_B1_KEY[u1color_idx]);
        }
	#endif 	
	if(u4Code == u4tmp)
		return 1;

//system code7 always valid...		
	rc5_b2b3 = RC5_B2B3_KEY[key_map_idx];
	u4tmp = (((UINT32)rc5_b2b3) << 16) | (((UINT32)SYS7_B1_KEY[key_map_idx]) << 8);
	if(u4Code == u4tmp)
		return 1;
		
//Since we have not the BYTE1 information of P_COMP RC5 yellow/red/green, RC5 is not support MULIT RC now		
	u4tmp = (((UINT32)rc5_b2b3) << 16) | (((UINT32)RC5_BLUE_B1_KEY[key_map_idx]) << 8);
	if(u4Code == u4tmp)
		return 1;

	return 0;	
}

UINT8 bWakeupReasonFromKey(UINT32 u4Tmp)
{

	if(Check_IR_Key(u4Tmp, IR_DIGIT0_IDX))
	{
            //LogS("Digital0");
            return PDWNC_WAKE_UP_REASON_RC_DIGIT_0;
	}
	else if(Check_IR_Key(u4Tmp, IR_DIGIT1_IDX))
	{
            //LogS("Digital1");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_1;
	}		
	else if(Check_IR_Key(u4Tmp, IR_DIGIT2_IDX))
	{
	     //LogS("Digital2");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_2;
	}		
	else if(Check_IR_Key(u4Tmp, IR_DIGIT3_IDX))
	{
	     //LogS("Digital3");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_3;
	}		
	else if(Check_IR_Key(u4Tmp, IR_DIGIT4_IDX))
	{
	     //LogS("Digital4");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_4;
	}
	else if(Check_IR_Key(u4Tmp, IR_DIGIT5_IDX))
	{
	     //LogS("Digital5");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_5;
	}		
	else if(Check_IR_Key(u4Tmp, IR_DIGIT6_IDX))
	{
	     //LogS("Digital6");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_6;
	}
	else if(Check_IR_Key(u4Tmp, IR_DIGIT7_IDX))
	{
	     //LogS("Digital7");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_7;
	}
	else if(Check_IR_Key(u4Tmp, IR_DIGIT8_IDX))
	{
            //LogS("Digital8");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_8;
	}		
	else if(Check_IR_Key(u4Tmp, IR_DIGIT9_IDX))
	{
            //LogS("Digital9");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_9;
	}		
	else if(Check_IR_Key(u4Tmp, IR_CH_UP_IDX)) 
	{
            //LogS("ch_up");
		return PDWNC_WAKE_UP_REASON_RC_PRG_UP;
	}		
	else if(Check_IR_Key(u4Tmp, IR_CH_DOWN_IDX))
	{
            //LogS("ch_down");
		return PDWNC_WAKE_UP_REASON_RC_PRG_DOWN;
	}
/*	
	else if(Check_IR_Key(u4Tmp, IR_CH_PREVCH_IDX))
	{
	     LogS("prev_Ch");
		return 4;
	}
	else if(Check_IR_Key(u4Tmp, IR_CH_SOURCE_IDX))
	{
	     LogS("source");
		return PC_WAKE_UP_REASON_RC_INP_SRC;
	}
*/
    return 0;
}



void vTask1_InitCust_P_IrInit()
{
    if(MGET_SYS_PRE_STANDBY_STATUS() == STBY_FROM_AC_ON)
    {
        gLPIR.fgIR_FilterFlag = IR_BLUE_CFG;
    }
    InitIR_KeyMap();

    gLPIR.u1LPIR_CHK = 0;
    gLPIR.u4CurrentPowerCode = 0;
    gLPIR.u4PreviousPowerCode = 0;
    vSetTimer(LPIR_SHORT_TIMER_ID, 0);
    vSetTimer(LPIR_LONG_TIMER_ID, 0);    
}

void vTask1_Cust_P_IrStateMachine()
{
    if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
        return;
    if(gLPIR.u1LPIR_CHK == 0)
        return;
    if(fgIsTimerUp(LPIR_SHORT_TIMER_ID))
    {
        MIR_DIS_INT();        
#if UART_DBG        
        LogS("S_timeout"); 
#endif
        gLPIR.u1LPIR_CHK = 0;
        vSetTimer(LPIR_LONG_TIMER_ID, 0);
        gLPIR.u4PreviousPowerCode = gLPIR.u4CurrentPowerCode;
        vWakeUpSystem(PDWNC_WAKE_UP_REASON_IRRC, 0);
        MIR_EN_INT();
//        vTask1_InitCust_P_IrInit();
    }
#if 0

    else if(fgIsTimerUp(LPIR_LONG_TIMER_ID))
    {
            MIR_DIS_INT();
            #if UART_DBG        
            LogSD("L_timeout", gLPIR.u4CurrentPowerCode);         
            #endif
            gLPIR.u1LPIR_CHK = 0;                    
            vSetTimer(LPIR_SHORT_TIMER_ID, 0);
            gLPIR.u4PreviousPowerCode = gLPIR.u4CurrentPowerCode;
#if CEC_FEATURE_3            
            CEC_System_Standby();
#endif
            MIR_EN_INT();

//        vTask1_InitCust_P_IrInit();
    }
    #endif
}

static void _CustPCheckPowerKeyTask(void)
{
    if(gLPIR.u1LPIR_CHK == 0)
        return;
#if 0
    if(fgIsTimerUp(LPIR_SHORT_TIMER_ID))
    {
   //     MIR_DIS_INT();        
#if UART_DBG        
        LogS("S_timeout"); 
#endif
        gLPIR.u1LPIR_CHK = 0;
        vSetTimer(LPIR_LONG_TIMER_ID, 0);
        gLPIR.u4PreviousPowerCode = gLPIR.u4CurrentPowerCode;
        vWakeUpSystem(PDWNC_WAKE_UP_REASON_IRRC, 0);
     //   MIR_EN_INT();
//        vTask1_InitCust_P_IrInit();
    }

    else
#endif        
        if(fgIsTimerUp(LPIR_LONG_TIMER_ID))
    {
         //   MIR_DIS_INT();
            #if UART_DBG        
            LogSD("L_timeout", gLPIR.u4CurrentPowerCode);         
            #endif
            gLPIR.u1LPIR_CHK = 0;                    
            vSetTimer(LPIR_SHORT_TIMER_ID, 0);
            gLPIR.u4PreviousPowerCode = gLPIR.u4CurrentPowerCode;
#if CEC_FEATURE_3            
            CEC_System_Standby();
#endif
           // MIR_EN_INT();

//        vTask1_InitCust_P_IrInit();
    }
}



void vCUST_P_IrTask(IR_DATA_TYPE irData)
{
    UINT8 u1IrWakeupReason;
    if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
        return;

    u1IrWakeupReason = bWakeupReasonFromKey(MRC56_TOGGLE_BIT_MASK(irData));
    if(u1IrWakeupReason)
    {
        vWakeUpSystem(u1IrWakeupReason, 0);
        return;
    }

#if 1// P_COMP_LONG_POWER_CEC
#if  UART_DBG
        LogSD("vCUST_P_IrTask", bGetTimer(LPIR_LONG_TIMER_ID));
#endif    

//    if(irData == CUST_P_RC6_POWER)
    MIR_DIS_INT();
    _CustPCheckPowerKeyTask();            
    if(Check_IR_Key(MRC56_TOGGLE_BIT_MASK(irData), IR_POWER_IDX))
    {
        vSetTimer(LPIR_SHORT_TIMER_ID, 5);
        gLPIR.u4CurrentPowerCode = irData;
#if UART_DBG        
                LogSB("Power",GetColorIdx(gLPIR.fgIR_FilterFlag));
#endif
        if(gLPIR.u4CurrentPowerCode != gLPIR.u4PreviousPowerCode) // RC56 has toggle bit to detect if user has relseased the key 
        {
#if UART_DBG            
            LogSD("accept", gLPIR.u4PreviousPowerCode);
#endif
            if(fgIsTimerAtInitialState(LPIR_LONG_TIMER_ID) && (gLPIR.u1LPIR_CHK == 0))
            {
                gLPIR.u1LPIR_CHK = 1;
#if UART_DBG            
            LogS("Set Ltimer");
#endif                
                vSetTimer(LPIR_LONG_TIMER_ID, 60);
            }
        }
#if UART_DBG                
        else
        {
            LogS("discard");
        }
#endif       
    }

    MIR_EN_INT();
#else//mtk ir remoter
    if(irData == 0xfd02e31c)
    {
        LogS("IR Pwr");        
        gLPIR.u1LPIR_CHK = 1;
        gLPIR.u4CurrentPowerCode = irData;
        vSetTimer(LPIR_SHORT_TIMER_ID, 5);
        vSetTimer(LPIR_LONG_TIMER_ID, 30);
    }
    else if((gLPIR.u1LPIR_CHK == 1) && (irData == 0x00000000))// repeat key
    {
        LogS("IR Repeat");
        vSetTimer(LPIR_SHORT_TIMER_ID, 5);
    }
    else if(gLPIR.u4CurrentPowerCode != irData)
    {
        vTask1_InitCust_P_IrInit();
    }
#endif

}

#endif


#if SUPORT_COLD_REBOOT
UINT8 u1ColdRebootChk;

void vTask3Init_cold_rebootInit(void)
{
    u1ColdRebootChk = 0;
    vSetTimer(COLD_REBOOT_TIMER_ID, 0);
}

void vTask3_cold_reboot_state_machine(void)
{
    if((MGET_ARM_POWER_STATE() != ARM_POWER_OFF) || (u1ColdRebootChk == 0))
        return;
    if(fgIsTimerUp(COLD_REBOOT_TIMER_ID))
    {
#if UART_DBG        
        LogS("ColdReboot TimeOut"); 
#endif
        vTask3Init_cold_rebootInit();
        vWakeUpSystem(PDWNC_WAKE_UP_REASON_CUSTOM_4, 0);
    }

}
#endif


#if CUST_B_NEC_IR_DRIVER
static CUST_B_IR_COMBINATION_KEY eIR_COMBINATION_KEY_t;
static CUST_B_IR_TIMER_STAUTS eIR_TIMER_STATUS_t;
//Add by Cust_B 20091215
void vTask5_InitCUST_B_IrInit()
{	
	LogS("[vTask5_InitCUST_B_IrInit] Initial Reg ");
	vSetTimer(POWER_ON_MUTIL_KEY_TIMER_ID, 0);
	eIR_COMBINATION_KEY_t = IR_COMBINATION_NoMatch;
	eIR_TIMER_STATUS_t = IR_TIMER_STAUTS_OFF;
}

/*-----------------------------------------------------------------------------
 * Name: vCUST_B_IrTask 
 *
 * Description: 8032 parse IR remote code while 5363 standby
 *
 * Inputs:  -
 			irData:Ir key code
 *
 * Outputs: -
 *
 * Returns: -
 * Remark:	Add by Cust_B 20091215
 ----------------------------------------------------------------------------*/
void vCUST_B_IrTask(IR_DATA_TYPE irData)
{
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
        return;
	
	LogSD("vCUST_B_IrTask", irData);
//	#if 0
	switch(irData)
	{
		case CUST_B_NEC_POWER:
			LogS("[8032 IR command]Power on");
			vWakeUpSystem(PDWNC_WAKE_UP_REASON_IRRC,0);
			break;
		case CUST_B_NEC_MENU:
			LogS("[8032 IR command] MENU KEY ");
			eIR_COMBINATION_KEY_t = IR_COMBINATION_KEY1_MATCH;
			eIR_TIMER_STATUS_t = IR_TIMER_STAUTS_ON;
			vSetTimer(POWER_ON_MUTIL_KEY_TIMER_ID, 80);		//Per 4s reset eIR_COMBINATION_KEY_t			
			break;
		case CUST_B_NEC_OK:
			LogS("[8032 IR command] OK KEY ");
			
			if(eIR_COMBINATION_KEY_t == IR_COMBINATION_KEY1_MATCH)
			{
				eIR_COMBINATION_KEY_t = IR_COMBINATION_KEY2_MATCH;
			}
			else
			{
				vTask5_InitCUST_B_IrInit();	
			}
			
			break;
		case CUST_B_NEC_INFO:
			LogS("[8032 IR command] INFO KEY ");
			if(eIR_COMBINATION_KEY_t == IR_COMBINATION_KEY2_MATCH)
			{
				eIR_COMBINATION_KEY_t = IR_COMBINATION_KEY3_MATCH;
			}
			else if(eIR_COMBINATION_KEY_t == IR_COMBINATION_KEY3_MATCH) 
			{
				vTask5_InitCUST_B_IrInit();
				vWakeUpSystem(PDWNC_WAKE_UP_REASON_IRRC,0);		
			}
			else
			{
				vTask5_InitCUST_B_IrInit();	
			}
			break;
		default:
			vTask5_InitCUST_B_IrInit();
			break;
	
	}	
//	#endif
}

void vTask5_CUST_B_IrStateMachine()
{
	if (eIR_TIMER_STATUS_t == IR_TIMER_STAUTS_ON)
	{
		if(fgIsTimerUp(POWER_ON_MUTIL_KEY_TIMER_ID))
	    {
	    	LogS("vTask5 timer out");
			vTask5_InitCUST_B_IrInit();
		}
	}	
}

#endif


#if 0// CUST_P_DC_ON_SEQUENCE
//david: need to use a struct to replace the following variables...
UINT8 Chk_ARM_Alive_Retry;
UINT8 fgEnableDisableMonitorPowerDrop;
UINT8 gTICK_3_DC_ON_Timer_Cnt;
UINT8 fgDC_ON_TimerTimeout;
UINT8 fgEnableDisableMonitorPowerDrop_CMD;
UINT8 fgPHILIPS_SPACE_DC_ON_STATE ;
#define STANDBY_5S_STATE 0xf2
#define WAIT_ARM_TO_NOTIFY_STATE 0xf1
#define DC_ON_SEQUENCE_STATE 0xf3
#define WAIT_FOR_DC_ON_STATE 0xf4
#define SetOPCTRL5_asGPIO() vRegWrite4B(0x00b4,u4RegRead4B(0x00b4) & (~0x200));
#define ENABLE_OPCTRL5_INT() vRegWrite4B(0x0044,0x00000020); //need to enable opctrl5 interrupt
#define IS_DC_POWER_OK() (u1RegRead1B(0x0088) & 0x20)? 1:0
#define IS_ARM_ALIVE() (u4RegRead4B(0x827c))? 1:0


void vTask2Init_CUST_P_dc_on_state_machine_init(void)
{
    SetOPCTRL5_asGPIO();
    ENABLE_OPCTRL5_INT();//vRegWrite4B(0x0044,0x00000020); //need to enable opctrl5 interrupt
    fgDC_ON_TimerTimeout = 0;// 1;
    DC_ON_Timer_Cnt = 0;
    fgEnableDisableMonitorPowerDrop = ENABLE_MONITOR_POWER_DROP;
    fgEnableDisableMonitorPowerDrop_CMD = 0;
    Chk_ARM_Alive_Retry = 0;
    fgPHILIPS_SPACE_DC_ON_STATE = DC_ON_SEQUENCE_STATE;
    
}


static void vTask2_CUST_P_dc_on_state_machine(void)
{
    
        UINT8 u1Opctrl5Var;
    switch(fgPHILIPS_SPACE_DC_ON_STATE)
    {

        case WAIT_FOR_DC_ON_STATE:
                if(fgDC_ON_TimerTimeout == 1)
                 {
            		#if UART_DBG
                        vRegWrite1B(REG_DATAW, 0xfb);
            		#endif
                 
                        fg2StageColdReboot = 0;
                        ClrGPIOEN(0x20);
                        u1Opctrl5Var =  ReadGPIOIN(0x20) ;
                        if((Chk_ARM_Alive_Retry >= 1) ||(u1Opctrl5Var == 0))
                        {
                            u1WakeupReason=2;//WAKEUP_BY_COLD_REBOOT;
                            fgPHILIPS_SPACE_DC_ON_STATE = DC_ON_SEQUENCE_STATE;
                            Chk_ARM_Alive_Retry = 0;
                            SetDC_ON_Timer(0);
//the following code snippet is copied form if(u1SubCode==Cmd_Sub_EnableErrCode) in ext0.c                   
                            fgErrCodeLedOn=0;
                            vWritePowerDownEntry(0x14);    
                            vWritePowerDownEntry(0x04);  
                            vInit_LED_ErrCode();
                            for(u1Opctrl5Var = 0; u1Opctrl5Var < LEDcodeMax; u1Opctrl5Var++)
                            {
                             LED_Array[u1Opctrl5Var] = 0;
                            }
                                                        
                               LED_Array[0]= 0x02;
                            fgErrCodeLedOn=1;
                        }
                        else
                        {
                            SetDC_ON_Timer(CNT_5S);
                            fgPHILIPS_SPACE_DC_ON_STATE = STANDBY_5S_STATE;
                            Chk_ARM_Alive_Retry++;
                          //0629  ERR_LED_ON();
                            EnterPowerStandby();
                        }
                 }
            break;
        case WAIT_ARM_TO_NOTIFY_STATE:
            if(fgStandbyMode)//if fgStandbyMode!=0, we've gotten the enterloader command
            {
                fgPHILIPS_SPACE_DC_ON_STATE = DC_ON_SEQUENCE_STATE;
                SetDC_ON_Timer(0);                
                Chk_ARM_Alive_Retry = 0;
            }
            else
            {
                //if(ChkDC_ON_TimerTimeOut())
                if(fgDC_ON_TimerTimeout == 1)
                {
                    if(Chk_ARM_Alive_Retry >= 1/*3*/)
                    {
                            u1WakeupReason=2;//WAKEUP_BY_COLD_REBOOT;
                            fgPHILIPS_SPACE_DC_ON_STATE = DC_ON_SEQUENCE_STATE;
                            Chk_ARM_Alive_Retry = 0;
                            SetDC_ON_Timer(0);
//the following code snippet is copied form if(u1SubCode==Cmd_Sub_EnableErrCode) in ext0.c                   
                            fgErrCodeLedOn=0;
                            vWritePowerDownEntry(0x14);    
                            vWritePowerDownEntry(0x04);  
                            vInit_LED_ErrCode();
                            for(u1Opctrl5Var = 0; u1Opctrl5Var < LEDcodeMax; u1Opctrl5Var++)
                            {
                             LED_Array[u1Opctrl5Var] = 0;
                            }
                               LED_Array[0]= 0x02;                            
                            fgErrCodeLedOn=1;
                    }
                    else
                    {
                            SetDC_ON_Timer(CNT_5S);
                            fgPHILIPS_SPACE_DC_ON_STATE = STANDBY_5S_STATE;
                            Chk_ARM_Alive_Retry++;
             //0629               ERR_LED_ON();
                            EnterPowerStandby();
//                            while(1);
                    }                    
                }
            }
            break;
        case STANDBY_5S_STATE:
            if(fgStandbyMode)//if fgStandbyMode!=0, we've gotten the enterloader command
            {
                fgPHILIPS_SPACE_DC_ON_STATE = DC_ON_SEQUENCE_STATE;
                SetDC_ON_Timer(0);                
                Chk_ARM_Alive_Retry = 0;
            }
            else if(fgDC_ON_TimerTimeout == 1) //if(ChkDC_ON_TimerTimeOut())
            {
                vRegWrite4B(0x0120, u4RegRead4B(0x0120)|0x00020000); //Enable 8032 wake up
                #if RC56_IR_DRIVER                        
                u4PreviousPowerCode = 0;
                #endif
                vWakeUpSystem();
            }
            break;            
        case DC_ON_SEQUENCE_STATE:
        default:
            break;
            
    }

}

#endif

#if CUST_P_SX_SUPPORT
#define RES_ACK 0
#define RES_NACK 1
#define RES_NAV 2
UINT8 fgSerialXpressEnable;
UART_DRV_OBJ* fgSerialXpressData;
#if CUS_SH_HOTEL_MODE 
UINT8 fgCust_SH_HotelModeUartWakeupEn;    
#endif

void CUS_P_SX_send(UINT8* pPAyload, UINT8 count)
{
    UINT8 i;
    UINT8 Cmd_Array[8];
    UINT8 u1Checksum;
    u1Checksum=0;


    Cmd_Array[0]=0xe;
    Cmd_Array[1]=count+10;
    Cmd_Array[2]=0;
    Cmd_Array[3]=0;
    Cmd_Array[4]=5;
    Cmd_Array[5]=count+1;
    Cmd_Array[6]=0;
    Cmd_Array[7]=0xc;

    
    for(i=0;i<8;i++)
        vIO32Write1B(PDWNC_DATAW, Cmd_Array[i]);

    for(i=0;i<(count-1);i++,pPAyload++)
    {
        vIO32Write1B(PDWNC_DATAW,  *pPAyload);
        u1Checksum ^=(*pPAyload);
    }
    vIO32Write1B(PDWNC_DATAW,  u1Checksum);

    vIO32Write1B(PDWNC_DATAW,  0xa5);
    vIO32Write1B(PDWNC_DATAW,  0xa5);    
}

void CUS_P_SX_ReportEvent(UINT8 response)
{
    UINT8 Report_array[2];
    
    Report_array[0]=0x16;
    Report_array[1]=response;
    CUS_P_SX_send(Report_array,3); //bema add check sum 2-->3
}

void CUS_P_SX_ReportPowerState(UINT8 power_state)
{
    UINT8 Report_array[3];

    Report_array[0]=0x22;
    Report_array[1]=0x18;
    Report_array[2]=power_state;
    CUS_P_SX_send(Report_array,4); //bema add check sum 2-->3
}

void CUS_P_SX_parse(void)
{
#if CUS_SH_HOTEL_MODE 
#else
    UINT8 data i = 0;
    UINT8 data num_payload_size;
    UINT8 data bchecksum;
    bchecksum=0;
#endif    
    #if 0// SX_DBG
    
    for(i=0;i<32;i++)
        vIO32Write1B(PDWNC_DATAW, fgSerialXpressData->uRs232Data[i]);

        fgSerialXpressData->uRs232Rxidx=0;
    #endif
        

    fgSerialXpressData->uRs232Rxidx=0;
//for T_comp H_proj project
  if( (fgSerialXpressData->uRs232Data[0] == 0x57) &&
         (fgSerialXpressData->uRs232Data[1] == 0x01) &&
         (fgSerialXpressData->uRs232Data[2] == 0x02) &&
         (fgSerialXpressData->uRs232Data[3] == 0x00) &&
         (fgSerialXpressData->uRs232Data[4] == 0x00) &&
    (fgSerialXpressData->uRs232Data[5] == 0x5a)
    )
  {
       vWakeUpSystem(PDWNC_WAKE_UP_REASON_UART_NORMAL, 0);
  }

//for A_COMP BXX project
  if( (fgSerialXpressData->uRs232Data[0] == 0xbe) &&
         (fgSerialXpressData->uRs232Data[1] == 0x41) &&
         (fgSerialXpressData->uRs232Data[2] == 0x01) &&
         (fgSerialXpressData->uRs232Data[3] == 0x06) &&
         (fgSerialXpressData->uRs232Data[4] == 0x50) &&
         (fgSerialXpressData->uRs232Data[5] == 0x00) &&
         (fgSerialXpressData->uRs232Data[6] == 0x00) &&
         (fgSerialXpressData->uRs232Data[7] == 0x00) &&
         (fgSerialXpressData->uRs232Data[8] == 0x00) &&
         (fgSerialXpressData->uRs232Data[9] == 0x00) &&         
         (fgSerialXpressData->uRs232Data[10] == 0x00) &&                  
    (fgSerialXpressData->uRs232Data[11] == 0xa8)
    )
  {
       vWakeUpSystem(PDWNC_WAKE_UP_REASON_UART_NORMAL, 0);
  }

#if Cust_B_Debug_UART
	LogSB("Urx_len:",fgSerialXpressData->uRs232Rxidx);
	LogSB("Urx[0]:", fgSerialXpressData->uRs232Data[0]);
	LogSB("Urx[1]:", fgSerialXpressData->uRs232Data[1]);
	LogSB("Urx[2]:", fgSerialXpressData->uRs232Data[2]);
	LogSB("Urx[3]:", fgSerialXpressData->uRs232Data[3]);
	LogSB("Urx[4]:", fgSerialXpressData->uRs232Data[4]);
	LogSB("Urx[5]:", fgSerialXpressData->uRs232Data[5]);
	LogSB("Urx[6]:", fgSerialXpressData->uRs232Data[6]);
	LogSB("Urx[7]:", fgSerialXpressData->uRs232Data[7]);
	LogSB("Urx[8]:", fgSerialXpressData->uRs232Data[8]);
	LogSB("Urx[9]:", fgSerialXpressData->uRs232Data[9]);
	LogSB("Urx[10]:",fgSerialXpressData->uRs232Data[10]);
	LogSB("Urx[11]:",fgSerialXpressData->uRs232Data[11]);
	LogSB("Urx[12]:",fgSerialXpressData->uRs232Data[12]);
#endif	
/* A_COMP Hotel Op code: LCD TV ON
    Cust_B 20091215	
*/
	if( (fgSerialXpressData->uRs232Data[0] == '#') &&
         (fgSerialXpressData->uRs232Data[1] == 'S') &&
         (fgSerialXpressData->uRs232Data[2] == '0') &&
         (fgSerialXpressData->uRs232Data[3] == '0') &&
         (fgSerialXpressData->uRs232Data[4] == '1') &&
    (fgSerialXpressData->uRs232Data[5] == '#')
    )	
  {
       LogS(" [Hotel Op code]: LCD TV ON");	//Add by Cust_B 20091215
	   vWakeUpSystem(PDWNC_WAKE_UP_REASON_UART_NORMAL, 0);
  }
  
#if CUS_SH_HOTEL_MODE  
  //only for 9600 baud rate 
  if( (fgCust_SH_HotelModeUartWakeupEn == 1) &&
        (fgSerialXpressData->uRs232Data[0] == 'P') &&
       (fgSerialXpressData->uRs232Data[1] == 'O') &&
       (fgSerialXpressData->uRs232Data[2] == 'W') &&
       (fgSerialXpressData->uRs232Data[3] == 'R') &&
       (fgSerialXpressData->uRs232Data[4] == '1') &&
       (fgSerialXpressData->uRs232Data[5] == '#') &&
       (fgSerialXpressData->uRs232Data[6] == '#') &&
       (fgSerialXpressData->uRs232Data[7] == '#') &&
        (fgSerialXpressData->uRs232Data[8] == '<'))
    {
        vIO32Write1B(PDWNC_DATAW,  'O');
        vIO32Write1B(PDWNC_DATAW,  'K'); 
        vIO32Write1B(PDWNC_DATAW,  '<'); 
        vWakeUpSystem(PDWNC_WAKE_UP_REASON_UART_NORMAL, 0);
  }
  else
  {
      vIO32Write1B(PDWNC_DATAW,  'E');
      vIO32Write1B(PDWNC_DATAW,  'R'); 
      vIO32Write1B(PDWNC_DATAW,  'R'); 
      vIO32Write1B(PDWNC_DATAW,  '<');             
  }
#else
    if( (fgSerialXpressData->uRs232Data[0] == 0xE) &&
         (fgSerialXpressData->uRs232Data[2] == 0) &&
         (fgSerialXpressData->uRs232Data[3] == 0) &&
         (fgSerialXpressData->uRs232Data[4] == 5) &&
         (fgSerialXpressData->uRs232Data[6] == 0) &&
    (fgSerialXpressData->uRs232Data[7] == 0xc)
    )
    {
        num_payload_size = (fgSerialXpressData->uRs232Data[1]-10);

        for(i=0; i<(num_payload_size-1); i++)
        bchecksum ^= fgSerialXpressData->uRs232Data[8+i];

        if( (fgSerialXpressData->uRs232Data[5] == num_payload_size+1) &&
        (fgSerialXpressData->uRs232Data[8+num_payload_size-1] == bchecksum) &&
        (fgSerialXpressData->uRs232Data[8+num_payload_size] == 0xa5) &&
        (fgSerialXpressData->uRs232Data[8+num_payload_size+1] == 0xa5)
        )
        {
        
            switch(fgSerialXpressData->uRs232Data[8])
            {
                case 0x20:  //Set command
                if(fgSerialXpressData->uRs232Data[9] == 0xA3) //set RS232 control
                {

                   // CUS_P_SX_ReportEvent(RES_NAV); //Currently not support, since uP cannot access NVM
                    if(fgSerialXpressData->uRs232Data[10] == 0x01)    //Set Power on
                    {
                    
			    fgSerialXpressEnable = 1;
                    }
                    else
                    {
                
			    fgSerialXpressEnable = 0;
                    	}
                    CUS_P_SX_ReportEvent(RES_ACK);
                }
                else if(fgSerialXpressData->uRs232Data[9] == 0x18 && fgSerialXpressEnable == 1)    //set power state
                {
                
                    if(fgSerialXpressData->uRs232Data[10] == 0x01)    //Set Power on
                    {
                    
                        CUS_P_SX_ReportEvent(RES_ACK);
                        //SwitchBack_PORT0(); //switch back port0 to ARM
                        //set 0x28818 bit 2 low
		//vRegWrite1B(REG_RS232_MODE, 0); 
                        //vRegWrite4B(0x0120, u4RegRead4B(0x0120)|0x00020000); //Enable 8032 wake up
                       // SwitchBack_PORT0(); //switch back port0 to ARM                                    
/*                        vWakeUpSystem();
                        LED1_ON;
                        LED2_OFF;
                        u1WakeupReason = 3;
*/
                        vWakeUpSystem(PDWNC_WAKE_UP_REASON_UART_NORMAL, 0);
                    }
                }
                else    //command not support in standby mode
                {
                    CUS_P_SX_ReportEvent(RES_NAV);
                }
                break;

                case 0x21:  //Get command
                if(fgSerialXpressData->uRs232Data[9] == 0x18) //Get power state
                {
                    CUS_P_SX_ReportEvent(RES_ACK);
                    CUS_P_SX_ReportPowerState(0x0); //power state is standby
                }
                else    //command not support in standby mode
                {
                    CUS_P_SX_ReportEvent(RES_NAV);
                }
                break;

                default:    //not set/get command
                CUS_P_SX_ReportEvent(RES_NAV);
                break;
            }
        }
        else //protocol error, should response Nack
        {
            CUS_P_SX_ReportEvent(RES_NACK);
        }
    }
    else //protocol error, should response Nack
    {
    	if(fgSerialXpressData->uRs232Data[0] == 0xE)
        CUS_P_SX_ReportEvent(RES_NACK);
    }
#endif //CUS_SH_HOTEL_MODE
}
#endif

#if UART_TASK
void vRS232CustTaskInit(void)
{
    fgSerialXpressData = pGetUartDrvObj();
}

void vRS232CustTask(void)
{
    #if CUST_P_SX_SUPPORT
        CUS_P_SX_parse();
    #endif
}
#endif


#if SUPPORT_ERROR_LED_CTRL
Error_Code_Ctrl gErrCodeCtrl;

void SetCtrlLedEnable(void)
{
    if(gErrCodeCtrl.fgErrCodeLedType == 1)
    {
        SetSERVOEN((UINT8)(gErrCodeCtrl.fgErrCodeLedMsk));
        SetSERVOPADEN((UINT8)(gErrCodeCtrl.fgErrCodeLedMsk));
    }
    else
    {
        SetGPIOEN(gErrCodeCtrl.fgErrCodeLedMsk);
    }
}

//we assume the gpio is  enable before this task
static void TurnOnCtrlLed(void)
{
    if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)
//if(1)
    {
        if(gErrCodeCtrl.fgErrCodeLedType == 1)
        {
            if(gErrCodeCtrl.fgErrCodeLedPolarity)
                SetSERVOOUT((UINT8)(gErrCodeCtrl.fgErrCodeLedMsk));
            else
                ClrSERVOOUT((UINT8)(gErrCodeCtrl.fgErrCodeLedMsk)); 
        }
        else
        {
            if(gErrCodeCtrl.fgErrCodeLedPolarity)
                SetGPIOOUT((UINT8)(gErrCodeCtrl.fgErrCodeLedMsk));
            else
                ClrGPIOOUT((UINT8)(gErrCodeCtrl.fgErrCodeLedMsk)); 
        }
    }
}

static void TurnOffCtrlLed(void)
{
    if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)
//if(1)
    {
        if(gErrCodeCtrl.fgErrCodeLedType == 1)
        {
            if(gErrCodeCtrl.fgErrCodeLedPolarity)    
                ClrSERVOOUT((UINT8)(gErrCodeCtrl.fgErrCodeLedMsk));
            else
                SetSERVOOUT((UINT8)(gErrCodeCtrl.fgErrCodeLedMsk));
        }
        else
        {
            if(gErrCodeCtrl.fgErrCodeLedPolarity)        
                ClrGPIOOUT((UINT8)(gErrCodeCtrl.fgErrCodeLedMsk));
            else
                SetGPIOOUT((UINT8)(gErrCodeCtrl.fgErrCodeLedMsk)); 
        }
    }

}

  
void vCust_S_ErrCodeLedReset(void)
{
    vSetTimer(ERROR_LED_TIMER_ID, gErrCodeCtrl.fgErrCodeLedPeriod);
    gErrCodeCtrl.fgErrCodeBlinkingTimeCnt =  gErrCodeCtrl.fgErrCodeBlinkingTime;
    gErrCodeCtrl.fgS_compErrLedStateMachine = CUST_S_ERROR_CODE_STATE_1;
}

void vTask4Init_error_led_ctrl(void)
{
    memcpy(&(gErrCodeCtrl), 0, sizeof(Error_Code_Ctrl));
    gErrCodeCtrl.fgS_compErrLedStateMachine = CUST_S_ERROR_CODE_STATE_1;    
}

void vTask4_error_led_ctrl_state_machine(void)
{
    if((MGET_ARM_POWER_STATE() == ARM_POWER_OFF) && (gErrCodeCtrl.fgErrCodeBlinkingTime))
    {
        switch (gErrCodeCtrl.fgS_compErrLedStateMachine)
        {
            case CUST_S_ERROR_CODE_STATE_1:
                TurnOnCtrlLed();
                if(fgIsTimerUp(ERROR_LED_TIMER_ID))
                {
                      vSetTimer(ERROR_LED_TIMER_ID, gErrCodeCtrl.fgErrCodeLedPeriod);
                     gErrCodeCtrl.fgS_compErrLedStateMachine = CUST_S_ERROR_CODE_STATE_2;
    	#if UART_DBG
    //		vRegWrite1B(REG_DATAW,0xA3); //debug
    	//	vRegWrite1B(REG_DATAW,fgS_compErrLedStateMachine); //debug
    		//vRegWrite1B(REG_DATAW,0xA3); //debug		
            #endif                 
                }
                break;
            case CUST_S_ERROR_CODE_STATE_2:
                TurnOffCtrlLed();
                if(fgIsTimerUp(ERROR_LED_TIMER_ID))
                {
                    if(gErrCodeCtrl.fgErrCodeBlinkingTimeCnt)
                    {
                         gErrCodeCtrl.fgErrCodeBlinkingTimeCnt--;
                    }
                    if(gErrCodeCtrl.fgErrCodeBlinkingTimeCnt)
                    {
                         vSetTimer(ERROR_LED_TIMER_ID, gErrCodeCtrl.fgErrCodeLedPeriod);
                         gErrCodeCtrl.fgS_compErrLedStateMachine = CUST_S_ERROR_CODE_STATE_1;
                    }
                    else
                    {
                        vSetTimer(ERROR_LED_TIMER_ID, gErrCodeCtrl.fgErrCodeLedInterval);
                        gErrCodeCtrl.fgS_compErrLedStateMachine = CUST_S_ERROR_CODE_STATE_4;
                     }
    	#if  UART_DBG
    //		vRegWrite1B(REG_DATAW,0xA3); //debug
    //		vRegWrite1B(REG_DATAW,fgS_compErrLedStateMachine); //debug
    //		vRegWrite1B(REG_DATAW,0xA3); //debug		
            #endif                 
                     
                }
                break;
    /*            
            case CUST_S_ERROR_CODE_STATE_3:
                TurnOnCtrlLed();            
                if(fgS_CompErrLedTimerCnt == 0)
                {
                     fgS_CompErrLedTimerCnt =  fgErrCodeLedInterval;
                     fgS_compErrLedStateMachine = CUST_S_ERROR_CODE_STATE_4;
    	#if UART_DBG
    		vRegWrite1B(REG_DATAW,0xA3); //debug
    		vRegWrite1B(REG_DATAW,fgS_compErrLedStateMachine); //debug
    		vRegWrite1B(REG_DATAW,0xA3); //debug		
            #endif                 
                     
                }
                break;
    */            
            case CUST_S_ERROR_CODE_STATE_4:
                TurnOffCtrlLed();            
                if(fgIsTimerUp(ERROR_LED_TIMER_ID))
                {
                    vCust_S_ErrCodeLedReset();
    	#if UART_DBG
    //		vRegWrite1B(REG_DATAW,0xA3); //debug
    //		vRegWrite1B(REG_DATAW,fgS_compErrLedStateMachine); //debug
    //		vRegWrite1B(REG_DATAW,fgErrCodeBlinkingTimeCnt); //debug		
    //		vRegWrite1B(REG_DATAW,0xA3); //debug		
            #endif                 
                    
                }                
                break;

            default:
                break;
        }
    }
}
void vTask4WakeupProcess_error_led_ctrl(void)
{
    if(ReadGPIOIN(gErrCodeCtrl.fgErrCodeLedMsk) && (gErrCodeCtrl.fgErrCodeBlinkingTime))//david: error code may be light on at this monent
    {
        TurnOffCtrlLed();
    }
}

void vTask4StandbyProcess_error_led_ctrl(void)
{
    if(gErrCodeCtrl.fgErrCodeBlinkingTimeCnt)
    {
        SetCtrlLedEnable();
    }
}

#endif




