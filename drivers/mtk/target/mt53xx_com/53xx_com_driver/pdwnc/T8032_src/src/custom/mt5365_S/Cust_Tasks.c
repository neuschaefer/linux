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

#if SIR_TASK
SIRC_CTRL_T idata gSIRC_ctrl;
SIRC_ServiceMode_Ctrl gSIRC_ServiceMode;
UINT16 SIRC_TogglePeriod[VALID_OIRI_TOGGLE_CNT];
#define vSIR_FrameTimerReset() vSetTimer(SIR_FRAME_WAIT_TIMER_ID, 0)

#if CUSH_S_IR_CHK_SUPPORT
void timer2interrupt(void) small interrupt T2INT using TIMER2_BANK
{
    TF2 = 0;//timer2 interrupt clean
// 1cnt period of T2 = 12/27000000, 0.44us, 1 interrupt period for (RCAP2L, RCAP2H) = (0,0) is about 29ms, and u2fgTimer2Iteration overflow every 1900 seconds
    if(gSIRC_ctrl.u2fgTimer2Iteration == 0xffff)
    {
        gSIRC_ctrl.u2fgTimer2Iteration = 0;
    }
    else
    {
        gSIRC_ctrl.u2fgTimer2Iteration++;
    }
}

void vSIR_RawTimerHwReset(void)
{
	MDisableTimer2();
	TCLK=0;//use timer1 as baudrate clock source(useless since we don't use the native uart of t8032)
	CP_RL2=0;//non-capture mode (auto-reload mode)
	T2EX=1;//forced reloading by rising or faliing edge of external pin (useless since EXEN2=0)
	EXEN2 = 0;//disable reload by external signal
    RCLK=0;//12 clocks per count
	RCAP2L = 0; // TL2_CFG;
	RCAP2H = 0;// TH2_CFG;
	MEnableTimer2();	// Enable Timer 2 Interrupts
	MTrgTimer2Cnt();	// Start  Timer 2 Running
	MTimer2PTHigh();	// Set Timer 2 Interrupt As High Priority
}
#if 0
UINT16 wIrDiffRawTime(UINT8 u1idx) small
{
    UINT16  u2Diff;


   if (((gSIRC_ctrl._gau4TimeSlotT1.u2Iteration - gSIRC_ctrl._gau4TmpTimeSlotT0.u2Iteration) > 1) ||
    	(((gSIRC_ctrl._gau4TimeSlotT1.u2Iteration - gSIRC_ctrl._gau4TmpTimeSlotT0.u2Iteration) == 1) && (gSIRC_ctrl._gau4TimeSlotT1.u2IrHwTimer.u2IrTimerVal >= gSIRC_ctrl._gau4TmpTimeSlotT0.u2IrHwTimer.u2IrTimerVal)))
    {
        u2Diff = 0xffff;
    }
    else
    {
    	if (((gSIRC_ctrl._gau4TimeSlotT1.u2Iteration - gSIRC_ctrl._gau4TmpTimeSlotT0.u2Iteration) == 1) && (gSIRC_ctrl._gau4TimeSlotT1.u2IrHwTimer.u2IrTimerVal < gSIRC_ctrl._gau4TmpTimeSlotT0.u2IrHwTimer.u2IrTimerVal))
        {
            u2Diff = (0xffff - gSIRC_ctrl._gau4TmpTimeSlotT0.u2IrHwTimer.u2IrTimerVal) + gSIRC_ctrl._gau4TimeSlotT1.u2IrHwTimer.u2IrTimerVal;
        }
        else if (gSIRC_ctrl._gau4TimeSlotT1.u2IrHwTimer.u2IrTimerVal >gSIRC_ctrl._gau4TmpTimeSlotT0.u2IrHwTimer.u2IrTimerVal)
        {
            u2Diff = gSIRC_ctrl._gau4TimeSlotT1.u2IrHwTimer.u2IrTimerVal - gSIRC_ctrl._gau4TmpTimeSlotT0.u2IrHwTimer.u2IrTimerVal;
        }
    }
    return u2Diff;

}
#endif

UINT16 wIrDiffRawTime(UINT8 u1idx) small
{
	//if((gSIRC_ctrl.u4TimerOverflowFlag >> (u1idx + 1)) & 0x00000001)
	if((u1idx != 0) && (gSIRC_ctrl.u1OverflowSlot == u1idx))
	{
		return ((0xffff - gSIRC_ctrl._gau4TimeSlot[u1idx].u2IrHwTimer.u2IrTimerVal) + gSIRC_ctrl._gau4TimeSlot[u1idx + 1].u2IrHwTimer.u2IrTimerVal);
	}
	else
	{
		return (gSIRC_ctrl._gau4TimeSlot[u1idx + 1].u2IrHwTimer.u2IrTimerVal - gSIRC_ctrl._gau4TimeSlot[u1idx].u2IrHwTimer.u2IrTimerVal);
	}
}


BIT _IsGuidePulseInvalid(void) small
{
    
    if (gSIRC_ctrl._gu4TmpCurrSlot < 1) { return 1; }
    if ((SIRC_TogglePeriod[0] < TIME_1900us) ||    // 1.90ms
        (SIRC_TogglePeriod[0] > TIME_2800us))      // 2.75ms
    {
#if SIR_DBG
    LogS("e:G"); //s_comp ir debug
    LogW(SIRC_TogglePeriod[0]);
#endif       
        return 1;
    }
    return 0;
}

BIT _IsTimeOffInvalid(void) small
{
	UINT8 i;
//	if (gSIRC_ctrl._gu4TmpCurrSlot < 1) { return 1; }
	for(i = 1; i < (VALID_OIRI_TOGGLE_CNT-1); i+=2)
	{
		if ((SIRC_TogglePeriod[i] < TIME_450us) ||    // 1.90ms
				(SIRC_TogglePeriod[i] > TIME_700us))	   // 2.75ms
			{
#if SIR_DBG
				LogSB("e:Toff", i); //s_comp ir debug
				LogW(SIRC_TogglePeriod[i]);
#endif       
				return 1;
			}
	}		
	return 0;
}

BIT _IsTimeOn0Invalid(void) small
{
		UINT8 i;
	//	if (gSIRC_ctrl._gu4TmpCurrSlot < 1) { return 1; }
		for(i = 2; i < (gSIRC_ctrl._gu4TmpCurrSlot); i+=2)
		{
			if(SIRC_TogglePeriod[i] < TIME_900us)
			{
				if ((SIRC_TogglePeriod[i] < TIME_500us) ||	  // 1.90ms
						(SIRC_TogglePeriod[i] > TIME_800us))	   // 2.75ms
					{
#if SIR_DBG
						LogSB("e:Ton0", i); //s_comp ir debug
						LogW(SIRC_TogglePeriod[i]);
#endif       
						return 1;
					}
			}
		}		
		return 0;
}

BIT _IsTimeOn1Invalid(void) small
{
			UINT8 i;
		//	if (gSIRC_ctrl._gu4TmpCurrSlot < 1) { return 1; }
			for(i = 2; i < (gSIRC_ctrl._gu4TmpCurrSlot); i+=2)
			{
				if(SIRC_TogglePeriod[i] > TIME_1050us)
				{
			
					if ((SIRC_TogglePeriod[i] < TIME_1100us) ||	  // 1.90ms
							(SIRC_TogglePeriod[i] > TIME_1400us))	   // 2.75ms
						{
							#if SIR_DBG
							LogSB("e:Ton1", i); //s_comp ir debug
							LogW(SIRC_TogglePeriod[i]);
							#endif       
							return 1;
						}
				}
			}		
			return 0;

}

void _Cust_S_IrParsing(void)
{
	UINT8 i;
	gSIRC_ctrl._gu4TmpCurrSlot = gSIRC_ctrl._gu4CurrSlot;
	gSIRC_ctrl._gu4CurrSlot = 0;
	gSIRC_ctrl.u1SoftcatDontCareFlag = 0;
	for(i = 0; i < (VALID_OIRI_TOGGLE_CNT-1); i++)
	{
		SIRC_TogglePeriod[i] = wIrDiffRawTime(i);
	}

	for(i = 0; i < VALID_OIRI_TOGGLE_CNT; i++)
	{
		gSIRC_ctrl._gau4TimeSlot[i].u2Iteration = 0;
		gSIRC_ctrl._gau4TimeSlot[i].u2IrHwTimer.u2IrTimerVal = 0;
	}
#if SIR_DBG	
	if(gSIRC_ctrl.u1DeepDbg)
	{
		for(i = 0; i < (VALID_OIRI_TOGGLE_CNT-1); i++)
		{
			while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; vIO32Write1B(PDWNC_DATAW, i);
			while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; vIO32Write1B(PDWNC_DATAW, bHiByte(SIRC_TogglePeriod[i]));
			while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; vIO32Write1B(PDWNC_DATAW, bLoByte(SIRC_TogglePeriod[i]));		
		}
	}
#endif	
	return;
}


void _Cust_S_IrParsing_OIRI(BIT fgPinStatus) small
{
	MOIRI_DIS_INT();
    //Put current time to rCurrRawTime
	if((gSIRC_ctrl._gu4CurrSlot & 0x01) != fgPinStatus)//even toggle of pin_status should be 0; odd toggle of pin_status should be 1
	{
		gSIRC_ctrl.u1SoftcatDontCareFlag = 1;
        #if SIR_DBG
		if(gSIRC_ctrl.u1DeepDbg)
		{
			vIO32Write1B(PDWNC_DATAW, 0x99);
			vIO32Write1B(PDWNC_DATAW, 0x99);
			vIO32Write1B(PDWNC_DATAW, 0x99);
			vIO32Write1B(PDWNC_DATAW, 0x99);		
		}
		#endif
	}
	if (gSIRC_ctrl._gu4CurrSlot < VALID_OIRI_TOGGLE_CNT)
    {
        gSIRC_ctrl._gau4TimeSlot[gSIRC_ctrl._gu4CurrSlot].u2Iteration = gSIRC_ctrl.u2fgTimer2Iteration;
        gSIRC_ctrl._gau4TimeSlot[gSIRC_ctrl._gu4CurrSlot].u2IrHwTimer.BYTES.u1Ticks = TH2;
        gSIRC_ctrl._gau4TimeSlot[gSIRC_ctrl._gu4CurrSlot].u2IrHwTimer.BYTES.u1Cycles = TL2;
        #if SIR_DBG
		if(gSIRC_ctrl.u1DeepDbg)
		{
			vIO32Write1B(PDWNC_DATAW, bHiByte(gSIRC_ctrl._gau4TimeSlot[gSIRC_ctrl._gu4CurrSlot].u2Iteration));
			vIO32Write1B(PDWNC_DATAW, bLoByte(gSIRC_ctrl._gau4TimeSlot[gSIRC_ctrl._gu4CurrSlot].u2Iteration));
			vIO32Write1B(PDWNC_DATAW, gSIRC_ctrl._gau4TimeSlot[gSIRC_ctrl._gu4CurrSlot].u2IrHwTimer.BYTES.u1Ticks);
			vIO32Write1B(PDWNC_DATAW, gSIRC_ctrl._gau4TimeSlot[gSIRC_ctrl._gu4CurrSlot].u2IrHwTimer.BYTES.u1Cycles); 		
		}
        #endif
		
		if (gSIRC_ctrl._gu4CurrSlot != 0)//from 1 to (VALID_OIRI_TOGGLE_CNT-1)
		{
			if(gSIRC_ctrl._gau4TimeSlot[gSIRC_ctrl._gu4CurrSlot].u2Iteration != gSIRC_ctrl._gau4TimeSlot[gSIRC_ctrl._gu4CurrSlot - 1].u2Iteration)
			{
				gSIRC_ctrl.u1OverflowSlot = gSIRC_ctrl._gu4CurrSlot;
		        #if SIR_DBG
				if(gSIRC_ctrl.u1DeepDbg)
				{
					vIO32Write1B(PDWNC_DATAW, 0xee);
					vIO32Write1B(PDWNC_DATAW, gSIRC_ctrl.u1OverflowSlot);
					vIO32Write1B(PDWNC_DATAW, 0xee);		
				}
				#endif				
			}
		}
		else
		{
			gSIRC_ctrl.u1OverflowSlot = 0;
		}
        gSIRC_ctrl._gu4CurrSlot++;		
    }
    else
    {
        gSIRC_ctrl._gu4CurrSlot = 0;
		gSIRC_ctrl.u1OverflowSlot = 0;
    }
	MOIRI_EN_INT();		
}

#endif


#define CheckSIRC_BitCntValid()   (_MGetIRBitCnt() == 12)? 1 : 0

BIT _Cust_S_IrDataCorrection(void) small
{
#if CUSH_S_IR_CHK_SUPPORT
	if(gSIRC_ctrl._gu4TmpCurrSlot != VALID_OIRI_TOGGLE_CNT)
#else
	if(!CheckSIRC_BitCntValid())
#endif
    {
		#if SIR_DBG
		LogSD("e:BitCnt2", RegReadFldAlign(PDWNC_IRH, FLD_BIT_CNT)); //s_comp ir debug
		#endif 
        return 0; // to ignore
    }

#if CUSH_S_IR_CHK_SUPPORT
	if(_IsGuidePulseInvalid())
	{
		return 0;
	}
#if 0
	else if(_IsTimeOffInvalid())
	{
		return 0;
	}
	else if(_IsTimeOn0Invalid())
	{
		return 0;
	}
	else if(_IsTimeOn1Invalid())
	{
		return 0;
	}
#endif
	else
	{
		return 1;
	}
#else
	return 1;
#endif
}

void vTask7SIR_Task_WakeupProcess(void)
{
#if QUERY_IR_POWER_KEY_STATE_SUPPORT    
	//david: do not disable IR interrupt for monitoring if user is keeping pressing
#else
		#if SIR_DBG //SIR_BEHAVIOR_DBG
			LogS("DI"); //s_comp ir debug	   
    	#endif 
		MIR_DIS_INT();//disable IR interrupt
	#if CUSH_S_IR_CHK_SUPPORT
		MOIRI_DIS_INT();
	#endif
#endif
}

void vTask7SIR_Task_StandbyProcess(void)
{//if IR_TASK is defined as 1, MIR_EN_INT() has already executed in system standby process
//	MIR_EN_INT();
	memset(&(gSIRC_ctrl), 0, sizeof(gSIRC_ctrl));
	gSIRC_ctrl.u1PreIrRxData0 = NO_SIRC_DATA;

	memset(&(gSIRC_ServiceMode), 0, sizeof(gSIRC_ServiceMode));
	gSIRC_ServiceMode.PrevEvent	= NO_SIRC_DATA;
#if CUSH_S_IR_CHK_SUPPORT
	MOIRI_GPIO_BOTH_EDGE_SETUP();
	MOIRI_EN_INT();
#endif
#if QUERY_IR_POWER_KEY_STATE_SUPPORT 
	vSetTimer(SIR_LONG_PRESSING_CHK_TIMER_ID, 0);
	gSIRC_ctrl._gu1ARM_IR_WORKING = 0;
#endif
#if CUSH_S_IR_CHK_SUPPORT
	vSIR_RawTimerHwReset();
#endif
	vSIR_FrameTimerReset();
#if SIR_DBG
	SetGPIOEN(0x10);
#endif
}

void vTask7SIR_Task_Init(void)
{
#if CUSH_S_IR_CHK_SUPPORT
	vSIR_RawTimerHwReset();
#endif
	vSIR_FrameTimerReset();
	
	memset(&(gSIRC_ctrl), 0, sizeof(gSIRC_ctrl));
	gSIRC_ctrl.u1PreIrRxData0 = NO_SIRC_DATA;
	
	memset(&(gSIRC_ServiceMode), 0, sizeof(gSIRC_ServiceMode));
	gSIRC_ServiceMode.PrevEvent = NO_SIRC_DATA;
#if QUERY_IR_POWER_KEY_STATE_SUPPORT 
	vSetTimer(SIR_LONG_PRESSING_CHK_TIMER_ID, 0);
	gSIRC_ctrl._gu1ARM_IR_WORKING = 1;
#endif

#if SIR_DBG
	gSIRC_ctrl.u1DeepDbg = 0;
#endif	
}


UINT8 IsS_CompPowerKey(IR_DATA_TYPE data u1key)
{
#if 0
		if(u1key == SIR_KEY_POWER)
			return 1;
#if DISABLE_WAKUP_1_SECOND//rafflesia only
		else if((u1Region != LATIN) && (u1key == SIR_KEY_TV))
			return 1;
		else if((u1Region != LATIN) && ((u1key == SIR_KEY_PRG_UP) || (u1key == SIR_KEY_PRG_DOWN)))
			return 1;
#endif
		else if(fgPowerOff == ARM_POWER_OFF && u1key == 0xAE)
			return 1;
		else if(fgPowerOff == ARM_POWER_ON && u1key == 0xAF)
			return 1;
		
#if SUPORT_DIGIT_KEY_WAKEUP    
    #if CUS_S_MULTI_KEY_ECO    
        #if S_UART_COMMAND
				else if((u1Region != LATIN) && (gSIRC_ServiceMode.u1WakeUp_waiting_idx == 0) && (u1key >= SIR_KEY_1 && u1key <= SIR_KEY_0))
        #endif //        #if S_UART_COMMAND
    #else //    #if CUS_S_MULTI_KEY_ECO    
		else if(u1Region != LATIN) && (u1key >= SIR_KEY_0 && u1key <= SIR_KEY_9)
    #endif //    #if CUS_S_MULTI_KEY_ECO    
			return 1;
		else
#endif //#if SUPORT_DIGIT_KEY_WAKEUP        
			return 0;
#else
	if(u1key == SIR_KEY_POWER)
		return 1;
	else if((u1key == SIR_KEY_PRG_UP) || (u1key == SIR_KEY_PRG_DOWN))
		return 1;
	return 0;
#endif
}
void vSIRC_EventProcess(IR_DATA_TYPE irData)
{
// 1. always accept power on event
	if(IsS_CompPowerKey(irData))
	{
        vWakeUpSystem(PDWNC_WAKE_UP_REASON_IRRC, (UINT8)irData);
	}
// 2. as for the service mode booting,  due to we don't need to handle long press operation in standby mode, we discard the same event.
	if(gSIRC_ServiceMode.PrevEvent == irData)
	{
		#if SIR_DBG //SIR_BEHAVIOR_DBG
			LogS("thr"); //s_comp ir debug	   
    	#endif
		return;
	}	
	gSIRC_ServiceMode.PrevEvent = irData;
	if(irData == SIR_KEY_DISPLAY)
	{
		gSIRC_ServiceMode.u1WakeUp_waiting_idx = 1;
		MSET_SYS_SERVICE_MODE(0);
		gSIRC_ServiceMode.fgIsHotelCommand = 0;
		#if SIR_DBG //SIR_BEHAVIOR_DBG
			LogS("disp"); //s_comp ir debug	   
    	#endif		
	}
	else
	{
		switch (gSIRC_ServiceMode.u1WakeUp_waiting_idx)
		{
			case 1:
				if((irData == SIR_KEY_DIGIT5) || (irData == SIR_KEY_TEST) || (irData == SIR_KEY_MUTE))
				{
#if SIR_DBG //SIR_BEHAVIOR_DBG
					LogS("Service2"); //s_comp ir debug	   
#endif		
				   	if((irData == SIR_KEY_TEST) ||(irData == SIR_KEY_MUTE))
				   	{
						gSIRC_ServiceMode.fgIsHotelCommand = 2;
				   	}
				   	else//(irData==SIR_KEY_DIGIT5) 
					{
						gSIRC_ServiceMode.fgIsHotelCommand = 0;
					}
					gSIRC_ServiceMode.u1WakeUp_waiting_idx = 2;
				}
				else
				{
					gSIRC_ServiceMode.u1WakeUp_waiting_idx = 0;
#if SIR_DBG //SIR_BEHAVIOR_DBG
					LogS("Ser_res1"); //s_comp ir debug	   
#endif		
				}
				break;
			case 2:
				if(irData == SIR_KEY_VOL_PLUS)
				{
#if SIR_DBG //SIR_BEHAVIOR_DBG
					LogS("Service3"); //s_comp ir debug	   
#endif		
					gSIRC_ServiceMode.u1WakeUp_waiting_idx = 3;
				}
				else if(irData == SIR_KEY_VOL_MINUS)
				{
#if SIR_DBG //SIR_BEHAVIOR_DBG
					LogS("Service3"); //s_comp ir debug	   
#endif		
					gSIRC_ServiceMode.u1WakeUp_waiting_idx = 4;
				}
				else
				{
#if SIR_DBG //SIR_BEHAVIOR_DBG
					LogS("Ser_res2"); //s_comp ir debug	   
#endif		
					gSIRC_ServiceMode.u1WakeUp_waiting_idx = 0;
				}
				break;
			case 3:
				if(irData == SIR_KEY_POWER)
				{
					MSET_SYS_SERVICE_MODE(1 + gSIRC_ServiceMode.fgIsHotelCommand); //service mode
					#if SIR_DBG//#if 0// SIR_BEHAVIOR_DBG
					LogS("_Mode_");
					LogB(MGET_SYS_SERVICE_MODE());
					LogB(gSIRC_ServiceMode.fgIsHotelCommand);
					#endif
				}
				else
				{
#if SIR_DBG //SIR_BEHAVIOR_DBG
					LogS("Ser_res3"); //s_comp ir debug	   
#endif		
				
					MSET_SYS_SERVICE_MODE(0);
					gSIRC_ServiceMode.fgIsHotelCommand = 0;
				}
				gSIRC_ServiceMode.u1WakeUp_waiting_idx = 0;
				break;
			case 4:
				if(irData == SIR_KEY_POWER)
				{
					MSET_SYS_SERVICE_MODE(2 + gSIRC_ServiceMode.fgIsHotelCommand); //self-diagnostic mode
					#if 0// UART_DBG
					vRegWrite1B(REG_DATAW,0xcd); //debug
					#endif							
					#if SIR_DBG//#if 0// SIR_BEHAVIOR_DBG
					LogS("Mode");
					LogB(gSIRC_ServiceMode.fgServiceMode);
					#endif
				}
				else
				{
#if SIR_DBG //SIR_BEHAVIOR_DBG
					LogS("Ser_res4"); //s_comp ir debug	   
#endif		
					MSET_SYS_SERVICE_MODE(0);
					gSIRC_ServiceMode.fgIsHotelCommand = 0;
				}
				gSIRC_ServiceMode.u1WakeUp_waiting_idx=0;
				break;
			default:
				gSIRC_ServiceMode.u1WakeUp_waiting_idx=0;
#if SIR_DBG //SIR_BEHAVIOR_DBG
				LogS("Ser_res0"); //s_comp ir debug	   
#endif		
				//david : if power on, these information should be reserved until ARM issues the Cmd_responseServiceMode...
				if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)						
				{
					MSET_SYS_SERVICE_MODE(0);
					gSIRC_ServiceMode.fgIsHotelCommand = 0;
				}
				break;
		}
	}			

}



void vTask7SIR_Task(IR_DATA_TYPE irData)
{
	if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)
	{
#if CUSH_S_IR_CHK_SUPPORT		
		_Cust_S_IrParsing();
#endif
		if(_Cust_S_IrDataCorrection() || (gSIRC_ctrl.u1SoftcatDontCareFlag == 1))
		{    /* Validate if 1st frame equals to 2nd frame */
			if((gSIRC_ctrl.u1PreIrRxData0 == irData) && (!fgIsTimerUp(SIR_FRAME_WAIT_TIMER_ID)))
			{
				vSIRC_EventProcess(irData);
			}
			gSIRC_ctrl.u1PreIrRxData0 = irData;			
			vSetTimer(SIR_FRAME_WAIT_TIMER_ID, 5);//100ms
		}
	}
	else
	{
#if QUERY_IR_POWER_KEY_STATE_SUPPORT
		if(IsS_CompPowerKey(irData))
		{
			vSetTimer(SIR_LONG_PRESSING_CHK_TIMER_ID, 25);
		}
#endif
	}
}

#endif

#if GPIO_INTERRUPT_ENABLE
void vCustGpioIsr(UINT32 u4IntStatus)
{
	if(u4IntStatus & 0x40000000)
	{
		_Cust_S_IrParsing_OIRI(ReadOIRI());
	}
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
    }

}

static void _CustPCheckPowerKeyTask(void)
{
    if(gLPIR.u1LPIR_CHK == 0)
        return;

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

#if S_HOTEL_MODE_COMMAND
static void	S_command_parse(void)
{

}
#endif

#if UART_TASK
void vRS232CustTaskInit(void)
{
//    fgSerialXpressData = pGetUartDrvObj();
}

void vRS232CustTask(void)
{
//Due to UART Data will be flushed in each funciton, only one define can be enabled !!!
    #if S_HOTEL_MODE_COMMAND
        S_command_parse();
    #endif
}
#endif


#if SUPPORT_ERROR_LED_CTRL
Error_Code_Ctrl gErrCodeCtrl;

void SetCtrlLedEnable(void)
{
    if(gErrCodeCtrl.fgErrCodeLedType == 1)
    {
        SetSERVOEN((UINT32)(gErrCodeCtrl.fgErrCodeLedMsk));
        SetSERVOPADEN((UINT32)(gErrCodeCtrl.fgErrCodeLedMsk));
    }
    else
    {
        SetGPIOEN((UINT32)(gErrCodeCtrl.fgErrCodeLedMsk));
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
                SetSERVOOUT((UINT32)(gErrCodeCtrl.fgErrCodeLedMsk));
            else
                ClrSERVOOUT((UINT32)(gErrCodeCtrl.fgErrCodeLedMsk)); 
        }
        else
        {
            if(gErrCodeCtrl.fgErrCodeLedPolarity)
                SetGPIOOUT((UINT32)(gErrCodeCtrl.fgErrCodeLedMsk));
            else
                ClrGPIOOUT((UINT32)(gErrCodeCtrl.fgErrCodeLedMsk)); 
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
                ClrSERVOOUT((UINT32)(gErrCodeCtrl.fgErrCodeLedMsk));
            else
                SetSERVOOUT((UINT32)(gErrCodeCtrl.fgErrCodeLedMsk));
        }
        else
        {
            if(gErrCodeCtrl.fgErrCodeLedPolarity)        
                ClrGPIOOUT((UINT32)(gErrCodeCtrl.fgErrCodeLedMsk));
            else
                SetGPIOOUT((UINT32)(gErrCodeCtrl.fgErrCodeLedMsk)); 
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
    memset(&(gErrCodeCtrl), 0, sizeof(Error_Code_Ctrl));
    gErrCodeCtrl.fgS_compErrLedStateMachine = CUST_S_ERROR_CODE_STATE_1;    
}

void vTask4_error_led_ctrl_state_machine(void)
{
    if((MGET_ARM_POWER_STATE() == ARM_POWER_OFF) && (gErrCodeCtrl.fgErrCodeBlinkingTime))
//    if(gErrCodeCtrl.fgErrCodeBlinkingTime)
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
    				LogS("st1");
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
				LogS("st2");
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
					LogS("st4");
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


#if SIMPLE_LED_TOGGLE
Led_Toggle_Ctrl gLedToggleCtrl;
void vTask6LedToggleCtrl_Init(void)
{
	memset(&(gLedToggleCtrl), 0, sizeof(Led_Toggle_Ctrl));
}

static void TurnOnToggleLed(void)
{
	if(1)
    {
        if(gLedToggleCtrl.u1LedType == 1)
        {
            if(gLedToggleCtrl.u1LedPolarity)
                SetSERVOOUT((UINT8)(gLedToggleCtrl.u4LedPinMsk));
            else
                ClrSERVOOUT((UINT8)(gLedToggleCtrl.u4LedPinMsk)); 
        }
        else
        {
            if(gLedToggleCtrl.u1LedPolarity)
                SetGPIOOUT((UINT8)(gLedToggleCtrl.u4LedPinMsk));
            else
                ClrGPIOOUT((UINT8)(gLedToggleCtrl.u4LedPinMsk)); 
        }
    }
}

static void TurnOffToggleLed(void)
{
	if(1)
    {
        if(gLedToggleCtrl.u1LedType == 1)
        {
            if(gLedToggleCtrl.u1LedPolarity)    
                ClrSERVOOUT((UINT8)(gLedToggleCtrl.u4LedPinMsk));
            else
                SetSERVOOUT((UINT8)(gLedToggleCtrl.u4LedPinMsk));
        }
        else
        {
            if(gLedToggleCtrl.u1LedPolarity)        
                ClrGPIOOUT((UINT8)(gLedToggleCtrl.u4LedPinMsk));
            else
                SetGPIOOUT((UINT8)(gLedToggleCtrl.u4LedPinMsk)); 
        }
    }

}

 


void vTask6LedToggleCtrl_state_machine(void)
{
	if(gLedToggleCtrl.u1LedToggleEnable)
	{
		 if(fgIsTimerUp(LED_TOGGLE_TIMER_ID))
	 	{
			vSetTimer(LED_TOGGLE_TIMER_ID, gLedToggleCtrl.u1LedTogglePeriod);
			if(gLedToggleCtrl.u1LedCurrentValue == 1)
			{
				gLedToggleCtrl.u1LedCurrentValue = 0;
				TurnOnToggleLed();
			}
			else
			{
				gLedToggleCtrl.u1LedCurrentValue = 1;
				TurnOffToggleLed();
			}

	 	}
	}
}
/*
void vTask6LedToggleCtrl_Wakeup_Process(void)
{

}
void vTask6LedToggleCtrl_Standby_Process(void)
{

}
*/
void vSetToggleLedEnable(void)
{
    if(gLedToggleCtrl.u1LedType == 1)
    {
        SetSERVOEN((UINT8)(gLedToggleCtrl.u4LedPinMsk));
        SetSERVOPADEN((UINT8)(gLedToggleCtrl.u4LedPinMsk));
    }
    else
    {
        SetGPIOEN(gLedToggleCtrl.u4LedPinMsk);
    }

}
#endif

