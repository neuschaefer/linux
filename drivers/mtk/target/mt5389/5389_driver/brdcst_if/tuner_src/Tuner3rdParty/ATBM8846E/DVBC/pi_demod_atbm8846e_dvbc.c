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

#include "pi_demod_atbm8846e_dvbc.h"
#include "pi_atbm8846e_common_api_exp.h"
#include "pi_atbm8846e_common_def.h"


static UINT8 ATBM_ChipID;





STATIC DEMOD_I2C_DATA_T DVBC_IniTbl[]=
{ 
 {0x000A,0x00},//1 bit: 1:Enable DSP self-reset when non-fatal error occur in DTMB mode,0:disable .Mandatory setting for DVBC mode
 {0x0009,0x00},//1 bit: 1:Enable DSP self-reset when fatal error occur in DTMB mode,0:disable.Mandatory setting for DVBC mode
 {0x0013,0x01},//1 bit: 1:Enable DVBC mode 0:Disable DVBC mode
 {0x1518,0x01},//1 bit: 1:Enable DVBC self-reset 0:Disable DVBC self-reset;Mandatory setting for DVBC mode
 //Configure  registers in DTMB register space, that assume different values from DTMB mode.
 {0x1515,0x01},//Enable/Disable error indicator signal in DVBC mode
 {0x1511,0x01},
 {0x1512,0x01},
/*
 {0x0a3b,0x28},
 {0x0a42,0x10},
 {0x0a43,0x05},
 {0x0a46,0x00},
 {0x0a47,0x01},
 {0x0a48,0x14},
 {0x0a49,0x00},
 {0x0a4a,0x76},
 {0x0a52,0x10},
 {0x0a53,0x52},
 {0x0a54,0xb1},
 {0x0a58,0x00},
 {0x0a59,0x40},
 {0x0a5b,0x00},
 {0x0a5c,0x1f},
 {0x0a5d,0x05},
 {0x0a5e,0x20},
 {0x0a5f,0x00},
 {0x0a60,0x04},
 {0x0a62,0x1e},
 //Marked after SDK V2.1
 {0x0a66,0xff},
 {0x0a67,0x0f},
 {0x0a6a,0xff},
 {0x0a6b,0x0f},
 

 {0x0a91,0x0f},
 {0x0a95,0x38},
 {0x0aa0,0xff},
 {0x0aa1,0xfa},
 {0x0aa2,0x00},
 {0x0aac,0xff},
 {0x0ab2,0x00},
//Marked after SDK V2.1
 //{0x0ab3,0xff},
 //{0x0ab4,0x0f},
 {0x0ab5,0x68},
 {0x0ab7,0x5c},
 //Marked after SDK V2.1
 //{0x0ab9,0xff},
 //{0x0aba,0x0f},
 {0x0abe,0x9d},
 {0x0ac0,0x14},
 {0x0ad2,0x30},
 {0x0ad3,0x00},
 {0x0ad4,0x05},
 {0x0ad5,0x40},
 {0x0ad9,0x0a},
 {0x1081,0xfa},
 {0x1082,0x00},
 {0x109d,0x00},
 {0x109f,0x00},
 {0x10a1,0x00},
 {0x10a2,0x00},
 {0x10a7,0x0b},
 {0x10ac,0x4d},
 */
 //Configure DVBC address space finish
};

BOOL ATBM_DVBC_Write(pDVBC_Demod_Ctx pDemodCtx,UINT16 u2RegAddr,UINT32 u4Data)
{
 BOOL fgRet=TRUE;
 UINT8 u1Data;
 
 mcSHOW_DBG_MSG4(("Prepare to write DVBC register 0x%04X-0x%08X\n",u2RegAddr,u4Data));
 
 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1503,0x00);//reg_wr_en disabled (LOW)
 //Write register address to 0x1501 0x1502 little-endian
 
 u1Data=(UINT8)(u2RegAddr & 0xFF);//LOW address
 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1501,u1Data);
 u1Data=(UINT8)((u2RegAddr>>8) & 0xFF);//High address
 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1502,u1Data);
 
 //Write data into register 0x1504~0x1507 little-endian
 u1Data=(UINT8)(u4Data & 0xFF);//1st data
 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1504,u1Data);
 u1Data=(UINT8)((u4Data>>8) & 0xFF);//2nd data
 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1505,u1Data);
 u1Data=(UINT8)((u4Data>>16) & 0xFF);//3rd data
 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1506,u1Data);
  u1Data=(UINT8)((u4Data>>24) & 0xFF);//4th data
 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1507,u1Data);
 
 //write signal enable
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1503,0x01);//reg_wr_en enable (High)
  
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1503,0x00);//reg_wr_en disabled  (falling)
  
  return fgRet;
}

BOOL ATBM_DVBC_Read(pDVBC_Demod_Ctx pDemodCtx,UINT16 u2RegAddr,UINT32 * u4Data)
{
 BOOL fgRet=TRUE;
 UINT8 u1Data;
 UINT8 u1InstanReadFlag=1;
 
 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1503,0x00);//read enable(LOW)
 //Write register address to 0x1501 0x1502 little-endian
 
 u1Data=(UINT8)(u2RegAddr & 0xFF);//LOW address
 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1501,u1Data);
 u1Data=(UINT8)((u2RegAddr>>8) & 0xFF);//High address
 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1502,u1Data);
 
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x084d,u1InstanReadFlag);
 //Read data from  register 0x1508~0x150B  little-endian
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1508,&u1Data);
  *u4Data=u1Data;
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1509,&u1Data);
  *u4Data|=(u1Data<<8);
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x150A,&u1Data);
  *u4Data|=(u1Data<<16);
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x150B,&u1Data);
  *u4Data|=(u1Data<<24);
 
  u1InstanReadFlag=0;
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x084d,u1InstanReadFlag);
  
  mcSHOW_DBG_MSG4((" Read DVBC register 0x%04X-0x%08X\n",u2RegAddr,*u4Data));
  
  return fgRet;
}

static UINT8 ATBMChipID(pDVBC_Demod_Ctx pDemodCtx) // This function checks ATBM884x chip ID,  The chip ID of ATBM884x is 0x20.   
{ BOOL fgRet=TRUE;

	UINT8 chipID;
	fgRet&= ATBMRead(pDemodCtx->u1I2cAddr,0x0000,&chipID);
//	printf("Chip ID= 0x%x, chipID ATBM8846:0x20 ATBM8846e:0x30\n",chipID);

	return chipID;
}

BOOL ATBM_DVBC_SetCarrier(pDVBC_Demod_Ctx pDemodCtx)
{
BOOL fgRet=TRUE;
UINT32 u4iIfSrchStartFreq,u4iIfSrchStepFreq;
UINT16 dSymbolRatek=pDemodCtx->u2SymbolRateKs; 
    
   
	u4iIfSrchStartFreq = (UINT32)(dSymbolRatek*331129)/500; //662.2585
	fgRet&=ATBM_DVBC_Write(pDemodCtx,0x3B4,u4iIfSrchStartFreq);	  

	u4iIfSrchStepFreq = (UINT32)(dSymbolRatek*331129)/1500; //662.2585
    fgRet&=ATBM_DVBC_Write(pDemodCtx,0x3B0,u4iIfSrchStepFreq);
	// Program nominal centre frequency (assumed to be zero for ZIF tuner)
   // fgRet&=ATBM_DVBC_Write(pDemodCtx,0x118,0x00);
	// Step size
    fgRet&=ATBM_DVBC_Write(pDemodCtx,0x388,0x14);// 20 for 1% step;
	// Search range
   fgRet&=ATBM_DVBC_Write(pDemodCtx,0x38C,0x27);// 39 for +/-15% limits
    // Standard times and thresholds for all other QAM levels
   fgRet&=ATBM_DVBC_Write(pDemodCtx,0x348,0x04);

   return fgRet;
}

BOOL ATBM_DVBC_SetSymbolRate(pDVBC_Demod_Ctx pDemodCtx)
{
  BOOL fgRet=TRUE;
  UINT32 u4TrlSampleRate;
  UINT32 u4Log2DecFactor;
  UINT32 u4TrlSampleRateRecip;
  UINT32 u4DecCicOutputShift;
  UINT32 u4AfcSlowControlTC;
  UINT16 dSymbolRatek=pDemodCtx->u2SymbolRateKs;
  
  //u4Log2DecFactor=log2((C_SAMPLE_CLK_FREQ*100)/(4*pDemodCtx->u2SymbolRateKs));
  
  	// Samples per symbol
		if (dSymbolRatek>3800)
	{
		u4Log2DecFactor = 0;
	}
	else if(dSymbolRatek>1900)
	{
		u4Log2DecFactor = 1;
	}
	else if(dSymbolRatek>950)
	{
		u4Log2DecFactor = 2;
	}
	else
	{
		u4Log2DecFactor = 3;
	}
   	u4TrlSampleRate = (UINT32)(((1<<16)*dSymbolRatek/C_SAMPLE_CLK_FREQ*(1<<u4Log2DecFactor))*2+1/2);
    fgRet&=ATBM_DVBC_Write(pDemodCtx,0x210,u4TrlSampleRate);
	
	u4TrlSampleRateRecip = (UINT32)((((1<<11)*C_SAMPLE_CLK_FREQ/dSymbolRatek)/(1<<u4Log2DecFactor)/2)+1/2);
    fgRet&=ATBM_DVBC_Write(pDemodCtx,0x214,u4TrlSampleRateRecip);
	
	u4DecCicOutputShift=1;

	if(u4Log2DecFactor ==1)
	{
		u4DecCicOutputShift = 4;
	}
	if(u4Log2DecFactor == 2)
	{
		u4DecCicOutputShift = 7;
	}
	if(u4Log2DecFactor == 3)
	{
		u4DecCicOutputShift = 10;
	}
     fgRet&=ATBM_DVBC_Write(pDemodCtx,0x128,u4DecCicOutputShift);
	 fgRet&=ATBM_DVBC_Write(pDemodCtx,0x124,u4Log2DecFactor);
	   
	 u4AfcSlowControlTC = (1<<(u4Log2DecFactor+2));
     fgRet&=ATBM_DVBC_Write(pDemodCtx,0x3BC,u4AfcSlowControlTC);
	 
     return fgRet;
}

BOOL ATBM_DVBC_SetQAM(pDVBC_Demod_Ctx pDemodCtx)
{
  BOOL fgRet=TRUE;
  
  fgRet&=ATBM_DVBC_Write(pDemodCtx,0x3CC,0x03F8);//Atuo QAM detect
  fgRet&=ATBM_DVBC_Write(pDemodCtx,0x364,(UINT32)(60000>>12));
  
  return fgRet;
}


//Only need to be invoked when Standby or disconnect called
BOOL ATBM_DVBC_Init(pDVBC_Demod_Ctx pDemodCtx)
{
  UINT16 u2IfFreq=ITunerGetCtx()->u2IF_Freq;
  int iItbCoarseFrequencyOffset;
  BOOL fgRet=TRUE;
  ATBM_ChipID = ATBMChipID(pDemodCtx);

  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0004, 0x00); 
  
  fgRet&=ATBM_Init(pDemodCtx->u1I2cAddr);//Common part init

  fgRet&=ATBMWriteDemodData(pDemodCtx->u1I2cAddr,DVBC_IniTbl,sizeof(DVBC_IniTbl)/sizeof(DVBC_IniTbl[0]));//Configure DVBC address space

  if(ATBM_ChipID == 0x30)
   	{
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0016,0x00);
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1520,0x00);
       fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1521,0x20);
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x152e,0x01);
    }
  
 

  //ATBM_DVBC_Init (custom_config) 
  fgRet&=ATBM_DVBC_Write(pDemodCtx,0x0004,0x01);//DVBC Module Reset

  if(ATBM_ChipID == 0x20)
	{
	 fgRet&=ATBM_DVBC_Write(pDemodCtx,0x0414,0x01);
	 fgRet&=ATBM_DVBC_Write(pDemodCtx,0x0424,0x2000);//Ber Count number
	}
  
  fgRet&=ATBM_DVBC_Write(pDemodCtx,0x0100,0x01);//IF mode

  iItbCoarseFrequencyOffset=(int)(4415*(C_SAMPLE_CLK_FREQ - u2IfFreq));
  fgRet&=ATBM_DVBC_Write(pDemodCtx,0x0118,iItbCoarseFrequencyOffset); //????
  
  fgRet&=ATBM_DVBC_SetCarrier(pDemodCtx);
  fgRet&=ATBM_DVBC_SetSymbolRate(pDemodCtx);
  fgRet&=ATBM_DVBC_SetQAM(pDemodCtx);
   
  fgRet&=ATBM_DVBC_Write(pDemodCtx,0x0438,0x0001);//write to 1 to intiate a BER measurement window and clear the ner counters
  fgRet&=ATBM_DVBC_Write(pDemodCtx,0x0104,0x0034);// gain control   
  fgRet&=ATBM_DVBC_Write(pDemodCtx,0x020C,0x0010);
  if(ATBM_ChipID == 0x30)
  	{
     ATBM_DVBC_Write(pDemodCtx,0x0204,0x400); //DVB-C symbol rate offset related
  	}
  
  fgRet&=ATBM_DVBC_Write(pDemodCtx,0x0004,0x00);
  
  
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0935, 0x14);
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0852, 0x28);
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0004, 0x01);
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x060e, 0x70); //ts clock=48.6MHz
  return  fgRet;
}
//Bit error ratio.unit is 1e-5
BOOL ATMB_DVBC_GetBER(pDVBC_Demod_Ctx pDemodCtx,UINT32 * u4Ber)
{

 UINT8 data1,data2;
 BOOL fgRet=TRUE;
 UINT32 u4BerReadySts;
 UINT32 u4TotalBits;//=13369344; //SampleSize: 8192*204*8;
 UINT8 ii=0;
 UINT32 u4CorrectedBerBits;
 STATIC UINT32 u4PreBer=0;
 * u4Ber=0;
 ATBM_ChipID = ATBMChipID(pDemodCtx);
 if(ATBM_ChipID == 0x20)
 {fgRet&=ATBM_DVBC_Read(pDemodCtx,0x0438,&u4BerReadySts);

 fgRet&=ATBM_DVBC_Read(pDemodCtx,0x0430,&u4CorrectedBerBits);
  u4CorrectedBerBits&=0xFFFF;
 fgRet&=ATBM_DVBC_Read(pDemodCtx,0x0424,&u4TotalBits);//16bit width
 u4TotalBits&=0xFFFF;
 u4TotalBits*=204*8;
 }
 if(ATBM_ChipID == 0x30)
 {fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1523,&data2);
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1522,&data1);
  u4CorrectedBerBits=data2*256 + data1;

  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1521,&data2);
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1520,&data1);
  u4TotalBits=data2*256 + data1*204*8;

 }
 if(u4CorrectedBerBits==0)
  {
    * u4Ber=0;
    if(ATBM_ChipID == 0x20)
     {	fgRet&=ATBM_DVBC_Write(pDemodCtx,0x0438,0x01);}
    return fgRet;
   }


   for(ii=0;ii<12;ii++)
   {
      *u4Ber=u4CorrectedBerBits/u4TotalBits;
      if(*u4Ber)
	  {
	     break;
	  }
	  else
	  {
	    u4CorrectedBerBits*=10; 
	  }
    }
  
 
 if(u4TotalBits==0)
 {
 mcSHOW_DBG_MSG(("Total bits number is zero!\n"))
  return fgRet;
 }
 
 if(u4BerReadySts)//Not ready
 {
 mcSHOW_DBG_MSG(("BER counting not end\n"));
 * u4Ber=u4PreBer;
 return TRUE;
 }

 *u4Ber=(u4CorrectedBerBits*100)/u4TotalBits;
 *u4Ber*=1000;//units is 1e-5
 u4PreBer=*u4Ber;
 if(ATBM_ChipID == 0x20)
   {fgRet&=ATBM_DVBC_Write(pDemodCtx,0x0438,0x01);}
 
 mcSHOW_DBG_MSG(("Corrected Bits number=%d,Total Bits number=%d\n",u4CorrectedBerBits,u4TotalBits));
 mcSHOW_DBG_MSG(("ATBM_DVBC_GetBER fgRet=%d\n",fgRet));
   return fgRet;
}
BOOL ATMB_DVBC_GetPER(pDVBC_Demod_Ctx pDemodCtx,UINT32 * u4Per) 
{
 
 UINT8 data1,data2;
 BOOL fgRet=TRUE;
 UINT32 u4PerReadySts;
 UINT32 u4TotalPackets; //SampleSize: 8192;
 
 UINT32 u4UnCorrectedPackets;
 STATIC UINT32 u4PrePer=0;
 * u4Per=0;
 ATBM_ChipID = ATBMChipID(pDemodCtx);

 if(ATBM_ChipID == 0x20)
 {fgRet&=ATBM_DVBC_Read(pDemodCtx,0x0438,&u4PerReadySts);

 fgRet&=ATBM_DVBC_Read(pDemodCtx,0x0434,&u4UnCorrectedPackets);
  u4UnCorrectedPackets&=0xFFFF;
 fgRet&=ATBM_DVBC_Read(pDemodCtx,0x0424,&u4TotalPackets);//16bit width
 u4TotalPackets&=0xFFFF;

 }
 if(ATBM_ChipID == 0x30)
 {fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1527,&data2);
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1526,&data1);
  u4UnCorrectedPackets=data2*256 + data1;

  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1521,&data1);
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1520,&data2);
  u4TotalPackets=data2*256 + data1*204;

 }


 if(u4TotalPackets==0)
 {
 mcSHOW_DBG_MSG(("Total bits number is zero!\n"))
  return fgRet;
 }
 
 if(u4PerReadySts)//Not ready
 {
 mcSHOW_DBG_MSG(("PER counting not end\n"));
 * u4Per=u4PrePer;
 return fgRet;
 }
 
 if(ATBM_ChipID == 0x20)
 fgRet&=ATBM_DVBC_Write(pDemodCtx,0x0438,0x01);//Reset BER calculator
 
 *u4Per=((u4UnCorrectedPackets&0xFFFF)*100000)/u4TotalPackets;//units is 1e-5
 
 u4PrePer=*u4Per;
 
 mcSHOW_DBG_MSG(("UnCorrected packets number=%d,Total packets number=%d\n",u4UnCorrectedPackets,u4TotalPackets));
 
   return fgRet;
}
BOOL ATMB_DVBC_GetSNR(pDVBC_Demod_Ctx pDemodCtx,UINT16 * u2SNR)
{
 BOOL fgRet=TRUE;
 UINT32 u4QAMSize;
 UINT32 u4Snr;
 //0x3C4  This status register gives an approximate measument of the Modulation Error Ratio of the received signal
 //MER=-20*log10([0x3C4]/2^16)-0.8=320*log10(2)-20log10([0x3C4])-0.8 QAM=16,64,256
 //MER=-20*log10([0x3C4]/2^16)-4.0,QAM=32,128
 //QAM size is indicated in 0x3D0 bit2:0
 fgRet&=ATBM_DVBC_Read(pDemodCtx,0x03C4,&u4Snr);
 
 u4Snr&=0xFFFF;//16bit width
 
 if(u4Snr==0)
 {
  * u2SNR=0;
  return fgRet;
 }
 //log10(2)=0.301
 //log2(10)=3.3219
 mcSHOW_DBG_MSG(("Log10_Convert(%d)=%d\n",u4Snr,Log10_Convert(u4Snr)));
 //u4Snr=320*301-20*log2(u4Snr)*1000*1000/3322;
 u4Snr = 2*Log10_Convert(65536/u4Snr)*1000;

 mcSHOW_DBG_MSG(("u4Snr=%d\n",u4Snr));
 
 //Get QAMSize
 fgRet&=ATBM_DVBC_Read(pDemodCtx,0x03D0,&u4QAMSize);
 u4QAMSize=1L<<((u4QAMSize&0x07)+1);//3bit width
  mcSHOW_DBG_MSG(("QAMSize=%d\n",u4QAMSize));
  
 switch(u4QAMSize)
 {
 case 16:
 case 64:
 case 256:
 u4Snr-=800;
 break;
  case 32:
 case 128:
  u4Snr-=4000;
  break;
 default:
 u4Snr=0;
 }
 
 *u2SNR=(UINT16)u4Snr/1000;
 return fgRet;
}
BOOL ATBM_DVBC_GetQAMSize(pDVBC_Demod_Ctx pDemodCtx,UINT16 * u2QAMSize) 
{
    UINT32 u4Data;
    BOOL fgRet=TRUE;
	 
    fgRet&=ATBM_DVBC_Read(pDemodCtx,0x03D0,&u4Data);
	u4Data=1L<<((u4Data&0x07)+1);//3bit width
	 * u2QAMSize=(UINT16)u4Data;
    return fgRet;	 
}

BOOL ATBM_DVBC_GetCFO(pDVBC_Demod_Ctx pDemodCtx,INT32 * i4CFO)
{
BOOL fgRet=TRUE;
UINT32 u4Data;
INT32 i4CarrierFreq;
INT32 i4CentralFreq;
INT32 i4CoarseFreq;

//Carrier freq in Hz=(u4CarrierFreq/2^29)*SymboRateM
fgRet&=ATBM_DVBC_Read(pDemodCtx,0x03C0,&u4Data);//32bit,signed,read only
i4CarrierFreq=(INT32)u4Data;

if(i4CarrierFreq<0)i4CarrierFreq*=-1;
i4CarrierFreq=(i4CarrierFreq/1000)*(pDemodCtx->u2SymbolRateKs/10)/53687;//KHz
//0x11C central frequency of current input frequency
//27bits.Signed
//Central frequency =[0x11C>>27]*MasterClockFrequency
fgRet&=ATBM_DVBC_Read(pDemodCtx,0x011C,&u4Data);
i4CentralFreq=(INT32)u4Data;

i4CentralFreq&=0x7FFFFFF;
if((i4CentralFreq) &(1L<<26))//MSB is signed bit
{
 i4CentralFreq=-(0x7FFFFFF-i4CentralFreq+1);
}
if(i4CentralFreq<0)
{
i4CentralFreq*=-1;
i4CentralFreq=(i4CentralFreq/100)*(C_SAMPLE_CLK_FREQ/100)/13422;//KHz
i4CentralFreq*=-1;
}
else
{
i4CentralFreq=(i4CentralFreq/100)*(C_SAMPLE_CLK_FREQ/100)/13422;//KHz
}
//Coase Frequency offset
fgRet&=ATBM_DVBC_Read(pDemodCtx,0x0118,&u4Data);//32bit,signed,read only
i4CoarseFreq=(INT32)u4Data;
i4CoarseFreq=(i4CoarseFreq/100)*(C_SAMPLE_CLK_FREQ/100)/13422;//KHz

mcSHOW_DBG_MSG(("i4CarrierFreq=%d Hz,i4CentralFreq=%dHz,i4CoarseFreq=%dHz\n",i4CarrierFreq,i4CentralFreq,i4CoarseFreq));
//Calculate overall CFO
if(i4CentralFreq<0)
{
 *i4CFO=i4CentralFreq+i4CarrierFreq+i4CoarseFreq;
}
else
{
 *i4CFO=i4CentralFreq+i4CarrierFreq-i4CoarseFreq;
}
return fgRet;
}

BOOL ATBM_DVBC_GetSync(pDVBC_Demod_Ctx pDemodCtx,BOOL * fgLock)
{
   UINT8 u1Data1,u1Mode;
 
   BOOL fgRet=TRUE;
   
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0013,&u1Mode);//0:DTMB;1:DVBC
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x030D,&u1Data1);
  
   *fgLock=((u1Data1==1) &&(u1Mode==0x01));   
    mcSHOW_DBG_MSG4(("ATBM_DVBC_GetSync fgLock=%d,fgRet=%d\n",*fgLock,fgRet));
    return fgRet;   
}

