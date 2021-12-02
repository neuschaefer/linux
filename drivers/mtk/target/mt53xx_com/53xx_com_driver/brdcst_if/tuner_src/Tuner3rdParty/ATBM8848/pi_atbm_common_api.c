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
 * $RCSfile: pd_DTMB_glue.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/** @file pi_atbm_common_api.c
 *  DTMB pi layer
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "ctrl_bus.h"
#include "pi_atbm_common_def.h"
#include "tuner_interface_if.h"
#include "fe_tuner_common_if.h"
#include "x_tuner.h"
#include "drvcust_if.h"
#include "x_gpio.h"
#include "x_os.h"
#include "sif_if.h"

#define I2C_TIMING 135

STATIC DEMOD_I2C_DATA_T InitTbl[]=
{
 {0x0606,0x00},//disable oscillator clock output 
 {0x0B09,0x00},//
 {0x0B0A,0x08},//2048 frames count to calculate LDCP uncorrecable error frames
 {0x0102,0x01},//Enable the I2C FSM  to synchronize with I2C master with I2C master when I2C master and slave are out of sync
 {0x020C,0x15},//SW ver 2.1
 //Marked after SDK V2.1
 //{0x146f,0x10},
 //{0x1470,0x20},
 //0x04a1 marked by Haibo 100621. To Switch IF_AGC with TESTIO
 //{0x04A1,0xFF},//TS lock and err signal will be output to drving LED 
 {0x04A2,0xFF},
};

STATIC DEMOD_I2C_DATA_T AGCParaTbl[]=
{
 {0x1020,0x00},//IF_Min
 {0x1023,0x00},//IF_Max Middle byte
 {0x1024,0x02},//IF_Max MSB 3 bytes
}; 

STATIC DEMOD_I2C_DATA_T TSOutFMTTbl[]=
{
 {0x0301,0x01},//2 bits bit 0, 0:rising edge 1:fallling edge;bit1,0:Enable TS output 1: Disable TS output
 {0x0305,0x01},//1bit 0:SPI 1:Serial
 //100424 if set gated clock,64QAM,FEC0.8 GI420 TI720 will be unlock
 {0x0306,0x01},//Clock mode 1bit 0:gated 1:continuous 
};

BOOL ATBMWrite(UINT8 u1I2cAddr,UINT16 u2RegAddr, UINT8 data)
{
    UINT8 u1RegAddr[2];

	u1RegAddr[0]=(UINT8)((u2RegAddr>>8) & 0xFF);//function address
	u1RegAddr[1]=(UINT8)(u2RegAddr & 0xFF);//offset address
	
	#if 0
	//Write Address
	if(ICtrlBus_I2cTunerWrite(I2C_TIMING,u1I2cAddr,u1RegAddr,2))
	{
	 mcSHOW_ERR_MSG(("Fail to write 0x%02X:0x%02X-0x%02X\n",u1I2cAddr,u1RegAddr[0],u1RegAddr[1]));
	 return FALSE;
	}
	//Write data
	if(ICtrlBus_I2cTunerWrite(I2C_TIMING,u1I2cAddr,&data,1))
	{
	 mcSHOW_ERR_MSG(("Fail to write 0x%02X:0x%02X-0x%02X-0x%02X\n",u1I2cAddr,u1RegAddr[0],u1RegAddr[1],data));
	 return FALSE;
	}
	#else
	  UINT32 u4DemodBusId=SIF_BUS_TUNER_I2C;//default
	  
    DRVCUST_OptQuery(eDemodI2cBusID,&u4DemodBusId);
	
    if (SIF_X_Write_SpecialDemod((UINT8)(u4DemodBusId), I2C_TIMING, u1I2cAddr, 2, (UINT32)u2RegAddr, &data, 1) == 0)
	{
	  mcSHOW_ERR_MSG(("Fail to write 0x%02X:0x%02X-0x%02X-0x%02X\n",u1I2cAddr,u1RegAddr[0],u1RegAddr[1],data));
	 return FALSE;
	}
	#endif
	mcSHOW_DBG_MSG3((" Write 0x%02X:0x%02X-0x%02X-0x%02X\n",u1I2cAddr,u1RegAddr[0],u1RegAddr[1],data));
	return TRUE;	
}

BOOL ATBMRead(UINT8 u1I2cAddr,UINT16 u2RegAddr, UINT8 * data)
{
    UINT8 u1RegAddr[2];
	
	
	u1RegAddr[0]=(UINT8)((u2RegAddr>>8) & 0xFF);//function address
	u1RegAddr[1]=(UINT8)(u2RegAddr & 0xFF);//offset address
	
	#if 0
	//Write Address
	if(ICtrlBus_I2cTunerWrite(I2C_TIMING,u1I2cAddr,u1RegAddr,2))
	{
	 mcSHOW_ERR_MSG(("Fail to write 0x%02X:0x%02X-0x%02X\n",u1I2cAddr,u1RegAddr[0],u1RegAddr[1]));
	 return FALSE;
	}
	//Read  data
	if(ICtrlBus_I2cTunerRead(I2C_TIMING,u1I2cAddr,data,1))
	{
	 mcSHOW_ERR_MSG(("Fail to read 0x%02X:0x%02X-0x%02X\n",u1I2cAddr,u1RegAddr[0],u1RegAddr[1]));
	 return FALSE;
	}
	#else
	UINT32 u4DemodBusId=SIF_BUS_TUNER_I2C;
	  DRVCUST_OptQuery(eDemodI2cBusID,&u4DemodBusId);
	
    if (SIF_X_Read((UINT8)(u4DemodBusId), I2C_TIMING, u1I2cAddr, 2, (UINT32)u2RegAddr,data, 1) == 0)
	{
	  mcSHOW_ERR_MSG(("Fail to read 0x%02X:0x%02X-0x%02X\n",u1I2cAddr,u1RegAddr[0],u1RegAddr[1]));
	 return FALSE;
	}
	#endif
	mcSHOW_DBG_MSG4((" Read 0x%02X:0x%02X-0x%02X-0x%02X %d\n",u1I2cAddr,u1RegAddr[0],u1RegAddr[1],*data,I2C_TIMING));
	
	return TRUE;	
}

BOOL ATBMWriteDemodData(UINT8 u1I2cAddr,pDEMOD_I2C_DATA pData,UINT8 u1Len)
{
    UINT8 ii=0;
	
	while(ii<u1Len)
	{
     if(!ATBMWrite(u1I2cAddr,pData->u2RegAddr,pData->u1RegVal))
    {
     return FALSE;
     }
	 pData++;
	 ii++;
	}
	return TRUE;
}

STATIC BOOL ATBMSetTsOutFMT(UINT8 u1I2cAddr)
{
   return ATBMWriteDemodData(u1I2cAddr,TSOutFMTTbl,sizeof(TSOutFMTTbl)/sizeof(TSOutFMTTbl[0]));
}

BOOL ATBMGetSync(UINT8 u1I2cAddr,BOOL * fgLock)
{
   UINT8 u1Data1,u1Data2,u1Mode;
   BOOL fgRet=TRUE;
   fgRet&=ATBMRead(u1I2cAddr,0x030d,&u1Data1);
   
   fgRet&=ATBMRead(u1I2cAddr,0x0B08,&u1Data2);//decode state
   
   fgRet&=ATBMRead(u1I2cAddr,0x0013,&u1Mode);//0:DTMB;1:DVBC
   
   *fgLock=((u1Data1==1) || (((u1Data2&0x01)==0x01) &&(u1Mode==0x00)));//If DTMB,decode state can be also referenced
   
   return fgRet;   
}
STATIC BOOL SetDemodIO(UINT8 u1I2cAddr,UINT8 portSts)
{
  BOOL fgRet=TRUE;
  UINT8 u1Data=0x00;//Set to 0x04a1
  
  fgRet&=ATBMWrite(u1I2cAddr,0x04a0,0x10);
  fgRet&=ATBMWrite(u1I2cAddr,0x04a2,0xFF);
  
  mcSHOW_DBG_MSG(("Set 0x%02X\n",portSts));
  
  switch(portSts)
  {
    case (TESTIO10_0 | TESTIO11_0):
	{
	  u1Data=0x02;
	  fgRet&=ATBMWrite(u1I2cAddr,0x0404,0x00);
	}
	break;
	case (TESTIO10_1 | TESTIO11_0):
	{
	  u1Data=0x01;
	  fgRet&=ATBMWrite(u1I2cAddr,0x0401,0x00);
	}
	break;
	case (TESTIO10_0 | TESTIO11_1):
	{
	  u1Data=0x0B;
	  fgRet&=ATBMWrite(u1I2cAddr,0x0407,0x00);
	}
	break;
    case (TESTIO10_1 | TESTIO11_1):
	{
	  u1Data=0x05;
	  fgRet&=ATBMWrite(u1I2cAddr,0x0500,0x00);
	}
	break;
	default:
	break;
  }
  fgRet&=ATBMWrite(u1I2cAddr,0x04a1,u1Data);
  
  return fgRet;
}
VOID ATBM_SwitchIfAGC(UINT8 u1I2cAddr,BOOL fgAnalog)
{
#ifdef CC_FUNAI_LC9//Special for Funai LC9 ATV(DTMB))
UINT8 u1Gpio7Sts,u1Gpio8Sts;

 if(fgAnalog)//ATV
 {
   u1Gpio7Sts=1;
   u1Gpio8Sts=0;
 }
 else    //DTV
 {
   u1Gpio7Sts=0;
   u1Gpio8Sts=1;
 }
 GPIO_SetOut(7,u1Gpio7Sts);
 GPIO_SetOut(8,u1Gpio8Sts);
 mcSHOW_DBG_MSG(("Switch IF_AGC to %s,GPIO7=%d,GPIO8=%d\n",(fgAnalog)?"ATV":"DTV",u1Gpio7Sts,u1Gpio8Sts));
 mcSHOW_DBG_MSG4(("Not use i2c addr 0x%02X\n",u1I2cAddr));//Fix potential build warn
#else
BOOL fgSts=TRUE;
 if(fgAnalog)//ATV
 {
  #ifdef CC_TUNER_ADAPT_PCB_5301D
 fgSts&=SetDemodIO(u1I2cAddr,TESTIO11_0);
 #else
 fgSts&=SetDemodIO(u1I2cAddr,TESTIO11_1);
 #endif
  }
 else//DTV
 {
 #ifdef CC_TUNER_ADAPT_PCB_5301D
 fgSts&=SetDemodIO(u1I2cAddr,TESTIO11_1);
 #else
 fgSts&=SetDemodIO(u1I2cAddr,TESTIO11_0);
 #endif
 }
 if(!fgSts)
 {
    mcSHOW_ERR_MSG(("Switch IF_AGC to ATV mode fail!\n")) ;
 }
#endif 
}
 #ifdef CC_TUNER_ADAPT_PCB_5301D
STATIC VOID ATBM_SwitchDIF(BOOL fgAnalog)
{
UINT8 u1GpioPort=47;
UINT8 u1GpioSts=0;
 if(fgAnalog)
 {
  u1GpioSts=1;
 }
 else
 {
  u1GpioSts=0;
 }
 GPIO_SetOut((UINT32)u1GpioPort,u1GpioSts);
 mcSHOW_DBG_MSG(("Switch DIF to %s,Gpio %d=%d\n",(fgAnalog?"ATV":"DTV"),u1GpioPort,u1GpioSts));
}
#endif
BOOL ATBM_Standby(UINT8 u1I2cAddr)
{
 UINT8 u1Data;
 BOOL fgRet=TRUE;
  #ifdef CC_TUNER_ADAPT_PCB_5301D
    //Switch DIF to ATV
   ATBM_SwitchDIF(TRUE);
  #endif 
 //Swtich IF_AGC to ATV
 //fgRet&=SetDemodIO(u1I2cAddr,TESTIO11_1);
  ATBM_SwitchIfAGC(u1I2cAddr,TRUE);
 fgRet&=ATBMWrite(u1I2cAddr,0x0005,0x01);//Set bit0 to 1 to turnoff DSP
 
 //Turn off ADC reference clock
 fgRet&=ATBMRead(u1I2cAddr,0x0600,&u1Data);
 u1Data|=0x02;//bit1 set to 1 to tuen off ADC reference voltage
 fgRet&=ATBMWrite(u1I2cAddr,0x0600,u1Data);
 
 //Turn off ADC
 fgRet&=ATBMRead(u1I2cAddr,0x0602,&u1Data);
 u1Data|=0x01;//bit0 set to 1 to tuen off ADC 
 fgRet&=ATBMWrite(u1I2cAddr,0x0602,u1Data);
 
 //Turnoff DVBC clock
 fgRet&=ATBMWrite(u1I2cAddr,0x1500,0x00);//Set bit0 to 0 to turnoff DVBC
 
 //disable TS output
 fgRet&=ATBMRead(u1I2cAddr,0x0301,&u1Data);
 u1Data|=0x02;//bit1 set to 1 to set TS port Hign-Z
 fgRet&=ATBMWrite(u1I2cAddr,0x0301,u1Data); 
 
 return fgRet;
}
BOOL ATBM_WakeUp(UINT8 u1I2cAddr)
{
  UINT8 u1Data;
  BOOL  fgRet=TRUE;
 
  fgRet&=ATBMWrite(u1I2cAddr,0x0005,0x00);//Set bit0 to 0 to turn on DSP
 
 //Turn on ADC reference clock
 fgRet&=ATBMRead(u1I2cAddr,0x0600,&u1Data);
 u1Data&=0xFD;//bit1 set to 0 to tuen on ADC reference voltage
 fgRet&=ATBMWrite(u1I2cAddr,0x0600,u1Data);
 
 //Turn on ADC
 fgRet&=ATBMRead(u1I2cAddr,0x0602,&u1Data);
 u1Data&=0xFE;//bit0 set to 0 to tuen off ADC 
 fgRet&=ATBMWrite(u1I2cAddr,0x0602,u1Data);
 
 //Turn on  DVBC clock
 fgRet&=ATBMWrite(u1I2cAddr,0x1500,0x01);//Set bit0 to 1 to turn on DVBC
 
 //enable TS output
 fgRet&=ATBMRead(u1I2cAddr,0x0301,&u1Data);
 u1Data&=0xFD;//bit1 set to 0 to enable TSs output
 fgRet&=ATBMWrite(u1I2cAddr,0x0301,u1Data);
 return fgRet;	
}
BOOL ATBM_BypassI2C(UINT8 u1I2cAddr,BOOL fgBypass)
{
//fgBypass=true,bypass ON,otherwise bypass OFF
   return ATBMWrite(u1I2cAddr,0x0103,fgBypass&0x01);
}

STATIC BOOL ATBM_SetIF(UINT8 u1I2cAddr)
{
  UINT16 u2IfFreq=ITunerGetCtx()->u2IF_Freq;//KHz
  UINT32 u4Freq_lif=0x4A1350;
  UINT32 u4ADC_Rate;
  UINT8 u1Data;
  BOOL fgRet=TRUE;
  //u4Freq_lif=2*PI*(IF_Freq_MHz-Sample_CLK_MHZ)*2^22/Sample_CLK_MHZ
  //if(u2IfFreq==36000)
  //{
   //u4Freq_lif=0x4A1350;//sample clock freq is 30.4MHz
  //}
  //Set IF
  if(u2IfFreq==0)
  {
  /*Tuner selection. 0: ZIF silicon tuner; 1: IF tuner*/  
      fgRet&=ATBMWrite(u1I2cAddr,0x0001,0);//ZIF Silicon tuner
      fgRet&=ATBMWrite(u1I2cAddr,0x0602,0x16);	  /*ADC config*/
	  
      mcSHOW_USER_MSG(("IF_Freq=0??\n"));
	  return FALSE;
  }
  if(u2IfFreq>C_SAMPLE_CLK_FREQ)
  {
   u4Freq_lif=(UINT32)((16*13176794/C_SAMPLE_CLK_FREQ)*(u2IfFreq-C_SAMPLE_CLK_FREQ)/8);
  }
  else//Low if
  {
   u4Freq_lif=(UINT32)((2*13176794/C_SAMPLE_CLK_FREQ)*u2IfFreq);

  }
  u1Data=(UINT8)(u4Freq_lif&0xFF);
  fgRet&=ATBMWrite(u1I2cAddr,0x0A00,u1Data);
  
  u1Data=(UINT8)((u4Freq_lif>>8)&0xFF);
  fgRet&=ATBMWrite(u1I2cAddr,0x0A01,u1Data);
  
  u1Data=(UINT8)((u4Freq_lif>>16)&0xFF);
  fgRet&=ATBMWrite(u1I2cAddr,0x0A02,u1Data);
  mcSHOW_DBG_MSG(("Set IF 0x%08X IF_Freq=%d KHz\n",u4Freq_lif,u2IfFreq));
  
  //Set ADC clock rate
  //ADC clock rate=2*Sample_CLK_MHz*2^19/30.4
  //Suppose sample clk MHZ is 30.4MHz
  if(C_SAMPLE_CLK_FREQ==30400)
  {
   u4ADC_Rate=0x100000;
  }
  else
  {
  u4ADC_Rate=(UINT32)(C_SAMPLE_CLK_FREQ*68985/2000);//Not accuracy
  }
  u1Data=(UINT8)(u4ADC_Rate&0xFF);
  fgRet&=ATBMWrite(u1I2cAddr,0x0A03,u1Data);
  
  u1Data=(UINT8)((u4ADC_Rate>>8)&0xFF);
  fgRet&=ATBMWrite(u1I2cAddr,0x0A04,u1Data);
  
  u1Data=(UINT8)((u4ADC_Rate>>16)&0xFF);
  fgRet&=ATBMWrite(u1I2cAddr,0x0A05,u1Data);
  mcSHOW_DBG_MSG(("Set ADC rate 0x%08X sample clk=%d KHz\n",u4ADC_Rate,C_SAMPLE_CLK_FREQ));
  
  return fgRet;
}
STATIC BOOL ATBM_SetTunerAGC(UINT8 u1I2cAddr)
{
    return ATBMWriteDemodData(u1I2cAddr,AGCParaTbl,sizeof(AGCParaTbl)/sizeof(AGCParaTbl[0]));
}
BOOL ATBM_Init(UINT8 u1I2cAddr)
{
   BOOL fgRet=TRUE;
   
   fgRet&=ATBMWrite(u1I2cAddr,0x0003,0x01);//SW reset Need  10 clock to finish reset
   mcDELAY_MS(10);
   
   //Switch IF_AGC to DTV TESTIO11=0
   //fgRet&=SetDemodIO(u1I2cAddr,TESTIO11_0);
    #ifdef CC_TUNER_ADAPT_PCB_5301D
   //Switch DIF to DTV
   ATBM_SwitchDIF(FALSE);
   #endif
    ATBM_SwitchIfAGC(u1I2cAddr,FALSE);
   fgRet&=ATBM_SetIF(u1I2cAddr);
   fgRet&=ATBM_SetTunerAGC(u1I2cAddr);
   fgRet&=ATBMSetTsOutFMT(u1I2cAddr);
   
   fgRet &=ATBMWriteDemodData(u1I2cAddr,InitTbl,sizeof(InitTbl)/sizeof(InitTbl[0]));
   
   return fgRet;
}

STATIC BOOL ATBM_GetIfAGCCtrl(UINT8 u1I2cAddr,UINT16 * u2IfAGCCtrl)
{
  BOOL fgRet=TRUE;
  UINT8 u1InstanReadFlag=1;
  UINT8 u1Data;
  
  fgRet&=ATBMWrite(u1I2cAddr,0x084d,u1InstanReadFlag);//1bit the register is used to make sure that several  registers for the same variable are latched and read at the same instant
  //0x1029[1:0],0x1028[7:0]PWN counter value
  fgRet&=ATBMRead(u1I2cAddr,0x1028,&u1Data);
  * u2IfAGCCtrl=u1Data;
  fgRet&=ATBMRead(u1I2cAddr,0x1029,&u1Data);
  * u2IfAGCCtrl|=((u1Data&0x03)<<8);
  
  u1InstanReadFlag=0;
  fgRet&=ATBMWrite(u1I2cAddr,0x084d,u1InstanReadFlag);//1bit the register is used to make sure that several  registers for the same variable are latched and read at the same instant
  return fgRet;
}
BOOL ATMB_SetFreq(UINT8 u1I2cAddr,PARAM_SETFREQ_T * pPara)
{
 BOOL fgRet=TRUE;
 
 fgRet&=ATBM_BypassI2C(u1I2cAddr,TRUE);
 
 if(ITuner_SetFreq(ITunerGetCtx(),pPara)!=0)
 {
  mcSHOW_ERR_MSG(("Fail to set freq\n"));
  fgRet=FALSE;
 }
  fgRet&=ATBM_BypassI2C(u1I2cAddr,FALSE);
  return fgRet;
}
/*UINT16 log2(UINT32 u4Data)
{
  UINT16 jj=1;

  if(u4Data<=1)return 0;

  while(1)
  {
   u4Data>>=1;
   if(u4Data<2)break;
   jj++;
  }
return jj;
}
*/
STATIC UINT16 SNR_log_table_int[]=
{
	0, 100,
		2, 104,
		4, 109,
		6, 114,
		8, 120,
		10, 125,
		12, 131,
		14, 138,
		16, 144,
		18, 151,
		20, 158,
		22, 166,
		24, 173,
		26, 182,
		28, 190,
		30, 199,
		32, 208	
};

/****************************************************************************
Function:    Left_Table
Parameters:  InValue
Return:      OutValue  

  Description:
  This is a sub function called by Log10_Convert.
******************************************************************************/
STATIC UINT16 Left_Table(UINT16 InValue)
{
	UINT16 OutValue = 0;
	UINT16 TableSize = sizeof(SNR_log_table_int)/sizeof(UINT16);
	UINT16 i;	
	for(i=0; i<TableSize; i+=2)
	{	
		if(InValue<=SNR_log_table_int[i+1])
		{		
			OutValue = SNR_log_table_int[i];	
		//	mcSHOW_USER_MSG(("InValue = %d, OutValue = %d\n", InValue, OutValue));
			return OutValue;
		}	
	}
	OutValue = 32;
	return OutValue;
}

/****************************************************************************
Function:    Log10_Convert
Parameters:  InValue
Return:      log_ret  

  Description:
  Calculate log value
******************************************************************************/
UINT32 Log10_Convert(UINT32 InValue)
{
	UINT32		u32_temp = 1;
	UINT8		indx = 0;
	UINT32             InValue_left;
	UINT32 log_ret;    

	do {	
		u32_temp = u32_temp << 1;
		if (InValue < u32_temp)
			break;		
	}while(++indx < 32);
	
	log_ret = (301*indx+50)/100;

	InValue_left = InValue*100/(1<<indx);
	
	log_ret = log_ret + Left_Table(InValue_left)/10;
	
	//mcSHOW_USER_MSG((" InValue = %d  log ret = %d\n", InValue,  log_ret));
	return log_ret;	
}
BOOL ATBM_TimeOut(UINT8 u1I2cAddr,BOOL* fgTimeOut)
{
  BOOL fgRet=TRUE;
  UINT8 u1Data1,u1Data2;
  UINT8 u1Mode=0;//DTMB or DVBC mode
  fgRet&=ATBMRead(u1I2cAddr,0x0d04,&u1Data1);
  //Not in GB modulation type
  fgRet&=ATBMRead(u1I2cAddr,0x082B,&u1Data2);
  
   fgRet&=ATBMRead(u1I2cAddr,0x0013,&u1Mode);//Get Current work mode.0:DTMB 1:DVBC
   
   * fgTimeOut=FALSE;
   
   if(u1Mode==0)//DTMB
   {
   if((u1Data2!=6)||(u1Data1<3)||(u1Data1>24))
   {
   * fgTimeOut=TRUE;
   }
   }
   else//DVBC
   {
    * fgTimeOut=TRUE;
   }
 
  return fgRet;
}

BOOL ATBMGetTsFMT(UINT8 u1I2cAddr,TS_FMT_T * pTsFmt)
{
   BOOL fgRet=TRUE;
   UINT8 u1Data;
   
    pTsFmt->ClockPolarity=POL_HIGH_ACTIVE;
	 pTsFmt->fgTristate=BOOL_TRUE;
	 pTsFmt->SerOrPar=SP_PARALLEL;
	 pTsFmt->fgGatedClock=BOOL_TRUE;
	 
	 pTsFmt->u4TsClockInKHz=7600;//7.6M parallel
	 
   fgRet&=ATBMRead(u1I2cAddr,0x0301,&u1Data);
   if(u1Data&0x01)
   {
   //0x0301 bit0 0:rising edge 1:falling edge
   pTsFmt->ClockPolarity=POL_LOW_ACTIVE;
   }
    if(u1Data&0x02)
   {
   //0x0301 bit1 0:enable ts output 1:disable ts output
   pTsFmt->fgTristate=BOOL_FALSE;
   }
    fgRet&=ATBMRead(u1I2cAddr,0x0305,&u1Data);
   if(u1Data&0x01)
   {
   //0x0305 bit0 0:SPI 1:SSI
   pTsFmt->SerOrPar=SP_SERIAL;
   pTsFmt->u4TsClockInKHz=60800;
   }
   
       fgRet&=ATBMRead(u1I2cAddr,0x0306,&u1Data);
   if(u1Data&0x01)
   {
   //0x0306  bit0 0:Gated 1:non-gated
   pTsFmt->fgGatedClock=BOOL_FALSE;
   }
   pTsFmt->fgExternalDemod=BOOL_TRUE;
   return fgRet;
}
BOOL ATBMGetSigLvl(UINT8 u1I2cAddr,MOD_TYPE_T mode,UINT8 * pu1SigLvl)
{
 BOOL fgRet=TRUE;
 UINT16 u2IfagcCtrl;
 fgRet&=ATBM_GetIfAGCCtrl(u1I2cAddr,&u2IfagcCtrl);
 mcSHOW_USER_MSG(("IF_AGC_Ctrl=0x%04X(%d)\n",u2IfagcCtrl,u2IfagcCtrl));
  ITuner_OP(ITunerGetCtx(),itGetSignalLevel,mode,&u2IfagcCtrl);
 
 * pu1SigLvl=(UINT8)u2IfagcCtrl;
 
 return fgRet;
}
BOOL ATBMIsDemodOk(UINT8 u1I2cAddr,BOOL * fgNeedReset)
{
 BOOL fgRet=TRUE;
 UINT8 u1Data;
 
 fgRet&=ATBMRead(u1I2cAddr,0x0607,&u1Data);//HW status
 
 if((u1Data!=0x05) || (!fgRet))
 {
  * fgNeedReset=TRUE;
 }
 else
 {
  * fgNeedReset=FALSE;
 }
 
 return fgRet;
}
VOID ATBMDoHWReset(VOID)
{
  UINT32 u4ResetPin;
  
 if(DRVCUST_OptQuery(eDemodResetGpio,&u4ResetPin)==0)
  {
  mcSHOW_USER_MSG(("Reset Demod GPIO %d 0->1 50 ms\n",u4ResetPin));
  GPIO_SetOut(u4ResetPin,0);
  x_thread_delay((UINT32)(50));//50ms low
  GPIO_SetOut(u4ResetPin,1);  
  x_thread_delay((UINT32)(30));//30ms to wait i2c ok
   }
  else
   {
    mcSHOW_USER_MSG(("No demod reset pin defined\n"));    
   }
 }
