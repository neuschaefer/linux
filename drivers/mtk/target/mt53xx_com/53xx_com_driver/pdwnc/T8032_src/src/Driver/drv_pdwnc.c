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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: Services.c $ 
 *
 * Project:
 * --------
 *   MT5387
 *
 * Description:
 * ------------
 *   T8032 main loop
 *   
 * Author:
 * -------
 *   David Hsieh
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/05/27 10:47p $    
 *
 * $Revision: #1 $
****************************************************************************/
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "general.h"
#include "hw_bim.h"
#include "drv_kr.h"
#include "hw_reg.h"
//#include "hw_rs232.h"
#include "custom_def.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
static void vExInit(void);
static void _vCommandPush(void);
static void _IrRxHwBusyClr();
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
SYSEM_STATE data gSys_state;
#if IR_TASK
IR_DRV_OBJ data gIrObj;
#endif

#if UART_TASK
UART_DRV_OBJ data gUartObj;
#endif
PDWNC_T8032_CMD_T data fgCmdObj;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void vExInit(void)	//initial ext interrupt
{
	//IT0=1;	//Ext0 edge trigger
	IT0=0;	//Ext0 low level trigger
	EX0=1;	//Enable Ext0 interrupt
	PX0=1;	//Set Ext0 high priority
}	

void vConfigSrvad(UINT8 u1ch)
{
    if(u1ch >= CORE_POWER_SRV)//Never config ch6/7 !!!!!!!!!!!!!!
    {
        return;
    }
    MEnableSrvClk();//vRegWrite1B(0x0, u1RegRead1B(0x0) & (~0x04));//make sure SRV clock not stop 
    MPowerOnADC();//vRegWrite1B(0x0390, 0x03);//enable comparator ofset cancellation/adc power on
    MSrvadHwMode();//vRegWrite1B(0x0300, 0x1);//enable hw mode 
    MEnableSrvad(u1ch);//vRegWrite1B(0x0304, 0xC8);//only enable adin3
    MSelectSrvad(u1ch);//vRegWrite1B(0x038C, 0x03);//select ch3
    MUpdateSrvad(u1ch);//vRegWrite1B(0x0380, 0x0);//update adin3 
}

UINT8 bReadSrvad(UINT8 u1ch)
{
    if(u1ch >= CORE_POWER_SRV)
    {
        return 0;
    }
    
    MUpdateSrvad(u1ch);
    return MReadSrvad(u1ch);
}

void vResetSysStateVar(UINT8 u1state)
{
    if(u1state == STANDBY_STATE)
    {
        gSys_state.fgStandbyMode = STANDBY_STATE;   
        gSys_state.fgPowerOff = ARM_POWER_OFF;
        gSys_state.fgNewIrData = NO_EVENT;        
        gSys_state.fgServiceMode = 0;
        gSys_state.fgNewCmdCome = NO_EVENT;
        gSys_state.fgRs232Rx = NO_EVENT;
        gSys_state.fgWakeupReason = PDWNC_WAKE_UP_REASON_UNKNOWN;
        gSys_state.fgSubWakeupReason = 0;
        gSys_state.fgFWUpgrading = 0;
//        gSys_state.fgXdataSwitchReq = NO_EVENT;
//        gSys_state.fgXdataSwitchState = XDATA_BY_T8032;
    }
    else if(u1state == LOADER_STATE)
    {
        gSys_state.fgNewCmdCome = NO_EVENT;
        gSys_state.fgNewIrData = NO_EVENT;
        gSys_state.fgPowerOff = ARM_POWER_ON;
        gSys_state.fgRs232Rx = NO_EVENT;
        gSys_state.fgServiceMode = NO_SERVICE_MODE;
        gSys_state.fgStandbyMode = LOADER_STATE;
        gSys_state.fgWakeupReason = PDWNC_WAKE_UP_REASON_UNKNOWN;
        gSys_state.fgSubWakeupReason = 0;
        gSys_state.fgFWUpgrading = 0;
        gSys_state.fgSysUartBaudRate = BR_115200;
//        gSys_state.fgXdataSwitchState = XDATA_BY_T8032;
//        gSys_state.fgXdataSwitchReq = NO_EVENT;  
    }
}


void vSysCustWakeupProcess(void)
{

#ifdef vTask1WakeupProcess
    vTask1WakeupProcess();
#endif

#ifdef vTask2WakeupProcess
    vTask2WakeupProcess();
#endif

#ifdef vTask3WakeupProcess
    vTask3WakeupProcess();
#endif

#ifdef vTask4WakeupProcess
    vTask4WakeupProcess();
#endif

#ifdef vTask5WakeupProcess
    vTask5WakeupProcess();
#endif

#ifdef vTask6WakeupProcess
    vTask6WakeupProcess();
#endif

#ifdef vTask7WakeupProcess
    vTask7WakeupProcess();
#endif

#ifdef vTask8WakeupProcess
    vTask8WakeupProcess();
#endif

#ifdef vTask9WakeupProcess
    vTask9WakeupProcess();
#endif

#ifdef vTask10WakeupProcess
    vTask10WakeupProcess();
#endif

#ifdef vTask11WakeupProcess
    vTask11WakeupProcess();
#endif

#ifdef vTask12WakeupProcess
    vTask12WakeupProcess();
#endif

#ifdef vTask13WakeupProcess
    vTask13WakeupProcess();
#endif

#ifdef vTask14WakeupProcess
    vTask14WakeupProcess();
#endif

#ifdef vTask15WakeupProcess
    vTask15WakeupProcess();
#endif

}

void vSysCustEnterPowerDownProcess(void)
{
#ifdef vTask1StandbyProcess
    vTask1StandbyProcess();
#endif

#ifdef vTask2StandbyProcess
    vTask2StandbyProcess();
#endif

#ifdef vTask3StandbyProcess
    vTask3StandbyProcess();
#endif

#ifdef vTask4StandbyProcess
    vTask4StandbyProcess();
#endif

#ifdef vTask5StandbyProcess
    vTask5StandbyProcess();
#endif

#ifdef vTask6StandbyProcess
    vTask6StandbyProcess();
#endif

#ifdef vTask7StandbyProcess
    vTask7StandbyProcess();
#endif

#ifdef vTask8StandbyProcess
    vTask8StandbyProcess();
#endif

#ifdef vTask9StandbyProcess
    vTask9StandbyProcess();
#endif

#ifdef vTask10StandbyProcess
    vTask10StandbyProcess();
#endif

#ifdef vTask11StandbyProcess
    vTask11StandbyProcess();
#endif

#ifdef vTask12StandbyProcess
    vTask12StandbyProcess();
#endif

#ifdef vTask13StandbyProcess
    vTask13StandbyProcess();
#endif

#ifdef vTask14StandbyProcess
    vTask14StandbyProcess();
#endif
    
#ifdef vTask15StandbyProcess
    vTask15StandbyProcess();
#endif

}

void vWakeUpSystemTasks(void)
{
    MIR_DIS_INT();//vRegWrite1B(PDWNC_INT_ENABLE + 1, u1RegRead1B(PDWNC_INT_ENABLE + 1) & 0xdf);//disable IR interrupt
//    MOIRI_DIS_INT();//    vRegWrite1B(PDWNC_INT_ENABLE + 3, u1RegRead1B(PDWNC_INT_ENABLE + 3) & 0x7f);//disable OIRI interrupt    
    MSET_ARM_POWER_STATE(ARM_POWER_ON);
    vSysCustWakeupProcess();  
}


void vWakeUpSystem(UINT8 u1Reason, UINT8 u1SubReason) //reentrant
{
        MDisableINT() ;    
        
        MSET_SYS_WAKEUP_REASON(u1Reason);
        MSET_SYS_SUB_WAKEUP_REASON(u1SubReason);
#if ENABLE_CEC        
        fgCecStandbyToOn = 1; 
#endif
        #if UART_DBG
        LogS("MWAKEUP"); 
        LogB(u1IO32Read1B(PDWNC_T8032_INTEN + 1));
        #endif        
        vWakeUpSystemTasks();        
        MT8032_EN_WAKEUP();//vRegWrite1B(0x0120 + 2, u1RegRead1B(0x0120 + 2)|0x02); //Enable 8032 wake up        
        MWakeUpSystem();
        SwitchBack_PORT0();

        MEnableINT() ;
}


void vEnterPowerStandby(void)
{
/*    
    #if UART_DBG
        LogS("Standby mode 0\n");
    #endif
    if(!MGET_SYS_FW_UPGRADING())
*/    
if(1)
    {
    
        MDisableINT() ;
        MWritePowerDownEntry();
        MClrSysInt(0xffffffff); //clean all interrupt

     #if IR_TASK    
        MIR_EN_INT();
        MIR_DIS_HW_WAKEUP();
        _MIR_FlushKey();
        vRegWriteFldAlign(PDWNC_IRCFGH, 1, FLD_DISPD);
     #else
        MIR_DIS_INT();     
        vRegWriteFldAlign(PDWNC_IRCFGH, 0, FLD_DISPD);
    #endif
        vRegWriteFldAlign(PDWNC_IRCLR, 1, FLD_IRCLR);

        vResetSysStateVar(STANDBY_STATE);
        SwitchUartPD_PORT0();
#if 0// RW_REG_INTERFACE
                vConfigSrvad(4);
#endif
        vRs232Init(MGET_SYS_UART_BAUDRATE());
    #if ENABLE_CEC
        CEC_Init();
        #if CEC_FEATURE_3
        if ((fgCecEnRx == 1) && (MGET_SYS_PRE_STANDBY_STATUS() != STBY_FROM_NORMAL_POWER))     // 1:from AC on, 2:from Normal mode
        {
            CEC_Report_Physic_Addr();
            CEC_Device_Vendor_ID();		
        }
        #endif   
    #endif
        vSysCustEnterPowerDownProcess();
        MEnableINT() ;
    }
}


void vCmdTaskInit(void)
{
    memset(&fgCmdObj, 0, sizeof(fgCmdObj));
}

UINT8 bCommandPop(PDWNC_T8032_CMD_T* tCmd)
{
    BIT cmd_event;
    MARM_DIS_INT();
    cmd_event = MGET_CMD_STATE();
    if(cmd_event == NEW_EVENT)
    {
        tCmd->u1Cmd = fgCmdObj.u1Cmd;
        tCmd->u1SubCmd = fgCmdObj.u1SubCmd; 
        tCmd->au1Data[0] = fgCmdObj.au1Data[0];
        tCmd->au1Data[1] = fgCmdObj.au1Data[1];
        tCmd->au1Data[2] = fgCmdObj.au1Data[2];
        tCmd->au1Data[3] = fgCmdObj.au1Data[3];
        MSET_CMD_STATE(NO_EVENT); 
    }
    MARM_EN_INT();
    return cmd_event;
}

static void _vCommandPush(void)
{
    fgCmdObj.u1Cmd = u1IO32Read1B(T8032_CMD_FromARM);
    fgCmdObj.u1SubCmd = u1IO32Read1B(T8032_CMD_FromARM + 1); 
    fgCmdObj.au1Data[0] = u1IO32Read1B(T8032_DATA_FromARM);
    fgCmdObj.au1Data[1] = u1IO32Read1B(T8032_DATA_FromARM + 1);
    fgCmdObj.au1Data[2] = u1IO32Read1B(T8032_DATA_FromARM + 2);
    fgCmdObj.au1Data[3] = u1IO32Read1B(T8032_DATA_FromARM + 3);
    MSET_CMD_STATE(NEW_EVENT);    
}
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
#if IR_TASK
void vIrTask(void)
{
    IR_DATA_TYPE tmpIrData;
//    if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
//        return;    
    MIR_DIS_INT();
    if(_MGET_IR_EVENT() != NEW_EVENT)
    {
        MIR_EN_INT();
        return;
    }
    tmpIrData = _MIR_PopKey();
#if 0//UART_DBG
        LogSD("vIrTask", tmpIrData);
#endif    
    _MIR_FlushKey();
    MIR_EN_INT();  

#ifdef vIR_Task1
    vIR_Task1(tmpIrData);
#endif

#ifdef vIR_Task2
    vIR_Task2(tmpIrData);
#endif

#ifdef vIR_Task3
    vIR_Task3(tmpIrData);
#endif

#ifdef vIR_Task4
    vIR_Task4(tmpIrData);
#endif

#ifdef vIR_Task7
    vIR_Task7(tmpIrData);
#endif


}

#endif

#if UART_TASK
UART_DRV_OBJ* pGetUartDrvObj(void)
{
    return &gUartObj;
}

void _vUART_PushString(void)
{
    UINT8 data uRxCnt;
    //check if TOUT interrupt
    if(RegReadFldAlign(PDWNC_INT_ID, FLD_TOUT))
    {
    //Get the number received..
        uRxCnt = RegReadFldAlign(PDWNC_UART_PD_STATUS, FLD_RXD_BUF_STATE);
        for( ;uRxCnt>0; uRxCnt--)
        {
            *(gUartObj.uRs232Data + gUartObj.uRs232Rxidx) = MGET_UART_CHAR();
            gUartObj.uRs232Rxidx++;

            if(gUartObj.uRs232Rxidx == MAX_UART_BUF_LEN)
                gUartObj.uRs232Rxidx = 0;
        }
               // _LogS(" new evt");
               
		vRs232PriorityProc();//8032 r/w xdata function
		
        MSET_UART_STATE(NEW_EVENT);
    }
}
#endif
static void _IrRxHwBusyClr()
{
    vIO32WriteFldAlign(PDWNC_IRCLR, 0x1, FLD_IRCLR);//IRRX_WRITE32(IRRX_CLEAR_BUSY_REG, IRRX_CLR_BSY_IR);
}
/*
#if defined SIR_TASK
#if SIR_TASK
extern SIRC_CTRL_T idata gSIRC_ctrl;
#endif
#endif
*/
void vPDWNC_ISR(void) small interrupt EX0INT  
{
#ifdef CC_MT8223
		UINT8 u1PDWNC_SysIntStatus;
		UINT8 u1PDWNC_T8032IntStatus;
#if GPIO_INTERRUPT_ENABLE
		UINT32 u4PDWNC_IntStatus;
#endif
#if UART_DBG
	   LogS("in vPDWNC_ISR\n");
	   ISR_LogSD("ex0 t8032 009 en:", u1IO32Read1B(INTEN));
	   ISR_LogSD("ex0 t8032 007 en:", u1IO32Read1B(T8032_NINT));
	   ISR_LogSD("ex0 t8032 008 en:", u1IO32Read1B(INT_CLR));
	
		ISR_LogSD("ex0 intstat:", u4IO32Read4B(PDWNC_INTSTA));
		ISR_LogSD("ex0 int en:", u4IO32Read4B(PDWNC_T8032_INTEN));
		ISR_LogSD("ex0 int clr:", u4IO32Read4B(PDWNC_INTCLR));
		LogSD("u1PDWNC_SysIntStatus",u1PDWNC_SysIntStatus);
#endif
		//Processing non-gpio interrupt first...
		u1PDWNC_SysIntStatus = MREAD_NON_GPIO_INT_STATUS();
		u1PDWNC_T8032IntStatus = u1IO32Read1B(INTEN)&u1IO32Read1B(T8032_NINT);
	
		if(u1PDWNC_SysIntStatus & NON_GPIO_INTERRUPT_ARM_INT_FLAG)
		{	 
			//LogSB("----_vCommandPush--command from arm",fgCmdObj.u1Cmd);
			_vCommandPush();
			MClrSysInt(u1IO32Read1B(T8032_NINT));
		}
#if IR_TASK
		if(u1PDWNC_SysIntStatus & NON_GPIO_INTERRUPT_IR_INT_FLAG)
		{
#if UART_DBG
			ISR_LogSD("ir data", _ReadIrDataReg());
#endif
			if(_MIR_ACCEPTABLE())
			{
				_MIR_PushKey(_ReadIrDataReg());
			}
			_IrRxHwBusyClr();
		}
#endif
	
#if ENABLE_CEC
		if(u1PDWNC_T8032IntStatus & NON_GPIO_INTERRUPT_CEC_INT_FLAG)
		{
			//vClearSysInt(CEC_CLR);
			CEC_Interrupt();
		}
#endif
	
#ifdef ENABLE_AVLINK
		if(u1PDWNC_SysIntStatus & NON_GPIO_INTERRUPT_AVLINK_INT_FLAG)
		{
			ISR_LogS("AVLINK_INT");
	//		  vClearSysInt(AVLINK_INT);
		}
#endif
	
#if UART_TASK
		if(u1PDWNC_T8032IntStatus & NON_GPIO_INTERRUPT_UART_INT_FLAG)
		{
		   // if(_MUART_ACCEPTABLE())
		   ISR_LogS("8225 UART INT");
	
		 // 	  _vUART_PushString();
		 // 	 MCLR_ALL_UART_PD_NOTIFICATION();

		}
#endif
	
		M_NON_GPIO_INT_CLEAN(u1PDWNC_SysIntStatus);
	//most of models do not support this feature, we divided vPDWNC_ISR() into two parts for implement a totally 8-bits operation (non-gpio) isr...
#if GPIO_INTERRUPT_ENABLE
		u4PDWNC_IntStatus = MReadSysInt();
		vCustGpioIsr(u4PDWNC_IntStatus);
		MClrSysInt(u4PDWNC_IntStatus);
#endif
	///////////////////////////////////////////////////////////////////////
#else  //if not define CC_MT8223
	//////////////////////////////////////////////////////////////////////

    UINT8 u1PDWNC_SysIntStatus;
#if GPIO_INTERRUPT_ENABLE
    UINT32 u4PDWNC_IntStatus;
#endif
#if 0// RW_REG_INTERFACE
    UINT32 u4Data;
#endif

//Processing non-gpio interrupt first...
    u1PDWNC_SysIntStatus = MREAD_NON_GPIO_INT_STATUS() & MREAD_NON_GPIO_INT_ENABLE();
#if UART_DBG
    ISR_LogSD("ex0", u4IO32Read4B(PDWNC_INTSTA));
#endif

#if 0// RW_REG_INTERFACE

       u4Data = u4IO32Read4B(PDWNC_INTSTA);
 //   vRegWriteFldAlign(PDWNC_BCR, 0x1, FLD_CLEAR_TBUF);
//  while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};       
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};   vIO32Write1B(PDWNC_DATAW, 0x0D);
//    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};       
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};     vIO32Write1B(PDWNC_DATAW, 0x0A);    
    _LogS("INT");
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 
    vIO32Write1B(PDWNC_DATAW, '0');
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 
    vIO32Write1B(PDWNC_DATAW, 'x');    
    while (!(RegReadFldAlign(PDWNC_INT_ID, FLD_TBUF))){};    
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 
    _Print8bitsHex((UINT8)((u4Data >> 24) & 0x000000ff ));
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 
    _Print8bitsHex((UINT8)((u4Data >> 16)& 0x000000ff));
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 
    _Print8bitsHex((UINT8)((u4Data >> 8)& 0x000000ff));
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 
    _Print8bitsHex((UINT8)(u4Data & 0x000000ff));
//    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};      
//    while (!(RegReadFldAlign(PDWNC_INT_ID, FLD_TBUF))){};      
//    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};    vIO32Write1B(PDWNC_DATAW,  0x0D);
//    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};    vIO32Write1B(PDWNC_DATAW,  0x0A);    
//        while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};
#endif



    if(u1PDWNC_SysIntStatus & NON_GPIO_INTERRUPT_ARM_INT_FLAG)
    {
        _vCommandPush();
    }
#if IR_TASK
    if(u1PDWNC_SysIntStatus & NON_GPIO_INTERRUPT_IR_INT_FLAG)
    {
#if UART_DBG
        ISR_LogSD("ir data", _ReadIrDataReg());
#endif
/*
#if defined SIR_TASK && defined SIR_DBG
#if SIR_DBG	
	if(gSIRC_ctrl.u1DeepDbg)
	{
		vIO32Write1B(PDWNC_DATAW, 0xbb);
		vIO32Write1B(PDWNC_DATAW, 0xbb);
		vIO32Write1B(PDWNC_DATAW, 0xbb);
	}
#endif
#endif
*/
        if(_MIR_ACCEPTABLE())
        {
            _MIR_PushKey(_ReadIrDataReg());
			_MSetIR1stSample();
			_MSetIR2ndSample();			
			_MSetIR3rdSample();						
			_MSetIRBitCnt();						
        }
        _IrRxHwBusyClr();
    }
#endif

#if ENABLE_CEC
    if(u1PDWNC_SysIntStatus & NON_GPIO_INTERRUPT_CEC_INT_FLAG)
    {
        //vClearSysInt(CEC_CLR);
        CEC_Interrupt();
    }
#endif

#ifdef ENABLE_AVLINK
    if(u1PDWNC_SysIntStatus & NON_GPIO_INTERRUPT_AVLINK_INT_FLAG)
    {
        ISR_LogS("AVLINK_INT");
//        vClearSysInt(AVLINK_INT);
    }
#endif

#if UART_TASK
    if(u1PDWNC_SysIntStatus & NON_GPIO_INTERRUPT_UART_INT_FLAG)
    {
       // if(_MUART_ACCEPTABLE())
       //         _LogS("push");
            _vUART_PushString();
           MCLR_ALL_UART_PD_NOTIFICATION();
    }
#endif
    M_NON_GPIO_INT_CLEAN(u1PDWNC_SysIntStatus);

//most of models do not support this feature, we divided vPDWNC_ISR() into two parts for implement a totally 8-bits operation (non-gpio) isr...
#if GPIO_INTERRUPT_ENABLE
    u4PDWNC_IntStatus = MReadSysInt() & MReadSysIntEn();
    vCustGpioIsr(u4PDWNC_IntStatus);
    MClrSysInt(u4PDWNC_IntStatus);
#endif
    MClearT8032INT();
    IE0 = 0;
    //    _LogS(" out");
#endif//CC_MT8223
}




//-----------------------------------------------------------------------------
// Interface(exported) functions
//-----------------------------------------------------------------------------
void vPdwncInit(void)
{
    vExInit();
  
    vResetSysStateVar(LOADER_STATE);
#if IR_TASK
    gIrObj.fgCurIrKey = 0;
    gIrObj.fgNewIrData = NO_EVENT;
#endif

    MSET_SYS_PRE_STANDBY_STATUS(STBY_FROM_UNKNOW); 

#if ENABLE_CEC
    CEC_Init();
#endif    
LogS("CEC_Init");

#ifdef ENABLE_AVLINK
    i4AVL_init();
#endif  
}



