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

#include "pi_demod_atbm8846e_dtmb.h"
#include "pi_atbm8846e_common_api_exp.h"
#include "tuner_interface_if.h"
#include "pi_atbm8846e_common_def.h"



STATIC DEMOD_I2C_DATA_T DTMB_IniTbl[]=
{        
 {0x000a,0x01},//1 bit: 1:Enable DSP self-reset when fatal error occur in DTMB mode,disable
 {0x0009,0x01},//1 bit: 1:Enable DSP self-reset when non-fatal error occur in DTMB mode,disable
 {0x0013,0x00},//1 bit: 1:Enable DVBC mode 0:Disable DVBC mode
 //Marked after SDK V2.1
 //{0x0B03,0x28},//6 bits: [1:0] thrshold of DSP self-reset in terms of the consecutive error frames number
               //[3:2] thrshold of non-fatal error DSP self-reset 
 {0x1518,0x00},//1 bit: 1:Enable DVBC self-reset 0:Disable DVBC self-reset
 {0x1515,0x00},//Enable/Disable error indicator signal in DVBC mode
 {0x1511,0x00},
 {0x1512,0x00},
 //100720 update 2.0 Optimized for PN945 single carrier SNR caculation
 //Marked after SDK V2.1
 /*
 {0x0012,0x01},
 {0x0998,0x01},
 {0x0999,0x01},
 {0x0D08,0x06},
 {0x0D09,0x03}, 
 {0x0D0E,0x20}, 
 {0x142F,0x3F}, 
 */
 //100720 update
 //100810 update SDK V2.1-u4Data
 // DTMB_Internal
 {0x0b03,0x28},
 {0x0012,0x01},
 {0x0998,0x01},
 {0x0999,0x01},
 {0x0D08,0x06},
 {0x0D09,0x03}, 
 {0x0D0E,0x20}, 
 {0x142F,0x3F}, 
 {0x146f,0x05},
 {0x1470,0x18},
 {0x1471,0x04},
 {0x1472,0x05},
 {0x1473,0x0b},
 {0x1474,0x04},
 {0x1415,0x01},
 {0x1445,0x90},
 {0x1446,0xc0},
 {0x1447,0x80},
 {0x1449,0xc0},
 {0x0989,0x0c},
 {0x098b,0x0c},
 {0x08a0,0x00},
 {0x08a2,0x00},
 {0x0852,0x28},
 {0x131c,0x01},
 //DTMB_CCI_ACI
/* {0x0a3b, 0x14},
 {0x0a42, 0x20},
 {0x0a43, 0xf4},
 {0x0a46, 0x04},
 {0x0a47, 0x00},
 {0x0a48, 0x96},
 {0x0a49, 0x14},
 {0x0a4a, 0x75},
 {0x0a52, 0x20},
 {0x0a53, 0x80},
 {0x0a54, 0xb4},
 {0x0a58, 0x10},
 {0x0a59, 0x00},
 {0x0a5b, 0x04},
 {0x0a5c, 0x00},
 {0x0a5d, 0x01},
 {0x0a5e, 0x40},
 {0x0a5f, 0x03},
 {0x0a60, 0x28},
 {0x0a62, 0x00},
 {0x0a91, 0x00},
 {0x0a95, 0xfc},
 {0x0aa0, 0xfe},
 {0x0aa1, 0x04},
 {0x0aa2, 0x01},
 {0x0aac, 0x00},
 {0x0ab2, 0x04},
 {0x0ab5, 0x55},
 {0x0ab7, 0x6d},
 {0x0abe, 0x9e},
 {0x0ac0, 0x15},
 {0x0ad2, 0x20},
 {0x0ad3, 0x20},
 {0x0ad4, 0x20},
 {0x0ad5, 0x15},
 {0x0ad9, 0xc8},
 {0x1081, 0x04},
 {0x1082, 0x01},
 {0x109d, 0x5d},
 {0x109f, 0x3f},
 {0x10a1, 0x33},
 {0x10a2, 0xff},
 {0x10a7, 0x0a},
 {0x10ac, 0x4a},
 */
  //100810 update SDK V2.1-u4Data
 {0x0935,0x14},
 {0x0852,0x28},
 {0x0004,0x01},//1 bit: 1:Software complete the register setting 0:register setting  is not completed
};
static UINT8 ATBM_ChipID;

 UINT8 ATBMChipID(pDTMB_Demod_Ctx pDemodCtx) // This function checks ATBM884x chip ID,  The chip ID of ATBM884x is 0x20.   
{  //pDTMB_Demod_Ctx pDemodCtx; 
    BOOL fgRet=TRUE;
	UINT8 chipID;
	fgRet&= ATBMRead(pDemodCtx->u1I2cAddr,0x0000,&chipID);
//	printf("Chip ID= 0x%x, chipID ATBM8846:0x20 ATBM8846e:0x30\n",chipID);
	return chipID;
}



//Only need to be invoked when Standby or disconnect called
BOOL ATBM_DTMB_Init(pDTMB_Demod_Ctx pDemodCtx)
{ BOOL fgRet=TRUE;
  ATBM_ChipID = ATBMChipID(pDemodCtx);
  
 // fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0003,0x01);//SW reset Need  10 clock to finish reset
 // mcDELAY_MS(10);
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0004, 0x00);// bit: 1:Software complete the register setting 0:register setting  is not completed
  
  if(ATBM_ChipID == 0x30)
	  {
	   fgRet&=ATBM_Init(pDemodCtx->u1I2cAddr);

       fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x000a, 0x01);  
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0009, 0x01); 
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0013, 0x00);  
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1518, 0x00);  
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1515, 0x00);  
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1511, 0x00);  
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x1512, 0x00);  
	   
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0016, 0x1d);  
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0988, 0x08);  
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0989, 0x0c);  
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x099a, 0x40); 
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0c24, 0x0a);
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0c26, 0x0a);
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0e01, 0x08);  

	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0935, 0x14); 
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0852, 0x28); 
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x0004, 0x01);  
	   fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x060e, 0x70); //ts clock=48.6MHz
	  // printf("8846E ATBM_DTMB_Init\n");
	  }
  
  if(ATBM_ChipID == 0x20)
      { //printf("8846 ATBM_DTMB_Init\n");
       fgRet&=ATBM_Init(pDemodCtx->u1I2cAddr) ;
	   fgRet&=ATBMWriteDemodData(pDemodCtx->u1I2cAddr,DTMB_IniTbl,sizeof(DTMB_IniTbl)/sizeof(DTMB_IniTbl[0]));
      }
  return fgRet;

}
//Frame error ratio
BOOL ATMB_DTMB_GetFER(pDTMB_Demod_Ctx pDemodCtx,UINT16 *u2ErrFrameCount,UINT16 * u2TotalFrameCount)
{
 BOOL fgRet=TRUE;
  UINT8 u1InstanReadFlag=1;
  UINT8 u1Data;
  ATBM_ChipID = ATBMChipID(pDemodCtx);
  //Error frame count
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x084d,u1InstanReadFlag);//1bit the register is used to make sure that several  registers for the same variable are latched and read at the same instant
  //0x1029[1:0],0x1028[7:0]PWN counter value
  if(ATBM_ChipID == 0x20)
   {
  	fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0B04,&u1Data);
	* u2ErrFrameCount=u1Data;
	fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0B05,&u1Data);
	 * u2ErrFrameCount|=((u1Data&0x7F)<<8);
   }
  if(ATBM_ChipID == 0x30)
   {
    fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0B14,&u1Data);//Number of uncorrectable frames  in the window defined in 0x0B0A,0x0B09
    * u2ErrFrameCount=u1Data;
    fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0B15,&u1Data);
    * u2ErrFrameCount|=((u1Data&0x7F)<<8);
  	}
  //Total calculated frame count
  u1InstanReadFlag=0;
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x084d,u1InstanReadFlag);//1bit the register is used to make sure that several  registers for the same variable are latched and read at the same instant
   //Total 
   u1InstanReadFlag=1;
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x084d,u1InstanReadFlag);
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0B09,&u1Data);
   * u2TotalFrameCount=u1Data;
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0B0A,&u1Data);
    u1InstanReadFlag=0;
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x084d,u1InstanReadFlag);
   * u2TotalFrameCount|=((u1Data&0x7F)<<8);
   
   return fgRet;
  }
BOOL ATBM_DTMB_GetSNR(pDTMB_Demod_Ctx pDemodCtx,UINT16 *u2SNR)
{ 
 BOOL fgRet=TRUE;
 UINT8 u1InstanReadFlag=1;
 UINT8 u1PNNo,u1Tmp_noise,u1signal_l,u1signal_m,u1signal_h,u1noise_l,u1noise_m,u1noise_h;
 UINT32 u4signal_power,u4noise_power;
 UINT32 u4Data=0;
 UINT8 u1IsSC=0;
 ATBM_ChipID = ATBMChipID(pDemodCtx);
 
 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x084d,u1InstanReadFlag);//1bit the register is used to make sure that several  registers for the same variable are latched and read at the same instant
 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x082D,&u1PNNo);
 u1PNNo&=0x03;
 
 //fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1407,&u1snr_noise_sc_sel);
 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x14AF,&u1Tmp_noise);
 
 if(ATBM_ChipID==0x20)
{mcSHOW_DBG_MSG(("u1PNNo=%d\n",u1PNNo));
 if(u1PNNo==2)//PN595
 {
 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x14DC,&u1signal_l);
 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x14DD,&u1signal_m);
 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x14DE,&u1signal_h);
 
 u4signal_power=(UINT32)((u1signal_h&0x0F)<<16)+(UINT32)(u1signal_m<<8)+u1signal_l;
 
 u4signal_power>>=4;
 
 if(u1Tmp_noise!=3)
 {
 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x14F8,&u1noise_l);
 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x14F9,&u1noise_h);
 }
 else
 {
 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1340,&u1noise_l);
 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1341,&u1noise_h);
 }
 u4noise_power=((u1noise_h&0x3f)<<8)+u1noise_l;
 if(u4signal_power==0)
 {
 u4signal_power=1;//Should be >0 in log calculation
 }
 u4Data=Log10_Convert((u4signal_power)/(u4noise_power+1));
 }
 
 if((u1PNNo==1) || (u1PNNo==3))//PN420 & PN945
 {
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0D0F,&u1IsSC);//Single carrier
  
  if(u1IsSC!=1)
  {
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0F1D,&u1signal_l);//0x1D
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0F1E,&u1signal_m);//0x89
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0F1F,&u1signal_h);//0x01
  
 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0F1A,&u1noise_l);//0x4B
 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0F1B,&u1noise_m);//0x00
 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0F1C,&u1noise_h);//0x00
 
 u4signal_power=(UINT32)((u1signal_h&0x0F)<<16)+(UINT32)(u1signal_m<<8)+u1signal_l;//0x01891D=100637
 u4noise_power=(UINT32)((u1noise_h&0x07)<<16)+(UINT32)(u1noise_m<<8)+u1noise_l;//0x00004B=75
 }
 
  if(u1IsSC==1)
  {
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x09CC,&u1signal_l);//0x1D
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x09CD,&u1signal_h);//0x01
   
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x09DE,&u1noise_l);//0x4B
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x09DF,&u1noise_h);//0x00
   
   u4signal_power=(UINT32)(u1signal_h<<8)+u1signal_l;//0x01891D=100637
   u4noise_power=(UINT32)(u1noise_h<<8)+u1noise_l;//0x00004B=75
  }
 
 if(u4signal_power==0)
 {
 u4signal_power=1;//Should be >0 in log calculation
 }
 if(u4noise_power==0)
 { u4Data=Log10_Convert((u4signal_power)/(u4noise_power+1));
  mcSHOW_DBG_MSG(("Noise power is 0 !!\n"));
 }
 else
 {
  mcSHOW_DBG_MSG(("Signal_pwr=%d,noise_power=%d,Log10_Convert(%d)=%d\n",u4signal_power,u4noise_power,(u4signal_power)/(u4noise_power),Log10_Convert((u4signal_power*10000)/(u4noise_power*10000))));
  u4Data=Log10_Convert((u4signal_power)/(u4noise_power));
 }
 }
 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x084d,0);//1bit the register is used to make sure that several  registers for the same variable are latched and read at the same instant
 
 *u2SNR=(UINT16)u4Data;
 }

 else if(ATBM_ChipID==0x30)
 {fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0D0F,&u1IsSC);//Single carrier
  
   if(u1IsSC!=1)
   {
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0F1D,&u1signal_l);//0x1D
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0F1E,&u1signal_m);//0x89
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0F1F,&u1signal_h);//0x01
 
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0F1A,&u1noise_l);//0x4B
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0F1B,&u1noise_m);//0x00
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0F1C,&u1noise_h);//0x00
 
   u4signal_power=(UINT32)((u1signal_h&0x0F)<<16)+(UINT32)(u1signal_m<<8)+u1signal_l;//0x01891D=100637
   u4noise_power=(UINT32)((u1noise_h&0x07)<<16)+(UINT32)(u1noise_m<<8)+u1noise_l;//0x00004B=75
     if(u4signal_power==0)
		{
		 u4signal_power=1;//Should be >0 in log calculation
		}
		u4Data=Log10_Convert((u4signal_power)/(u4noise_power+1));

    }
    if(u1IsSC==1)
		{
		if(u1PNNo==2) //PN595
			{fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x14dc,&u1signal_l);
             fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x14dd,&u1signal_m);
             fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x14de,&u1signal_h);
             u4signal_power=(UINT32)((u1signal_h&0x0F)<<16)+(UINT32)(u1signal_m<<8)+u1signal_l;//0x01891D=100637
             u4signal_power>>=4;
			 if(u1Tmp_noise!=3)
			  {
			  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x14F8,&u1noise_l);
			  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x14F9,&u1noise_h);
			  }
			  else
			  {
			  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1340,&u1noise_l);
			  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x1341,&u1noise_h);
			  }
			  u4noise_power=((u1noise_h&0x3f)<<8)+u1noise_l;
			  
			  if(u4signal_power==0)
			  {
			  u4signal_power=1;//Should be >0 in log calculation
			  }
			  u4Data=Log10_Convert((u4signal_power)/(u4noise_power+1));

		     }
		if((u1PNNo==1) || (u1PNNo==3)) //PN420 & PN945
			{fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x09cc,&u1signal_l);//0x1D
             fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x09cd,&u1signal_h);
             u4signal_power=(u1signal_h<<8)+u1signal_l;

			 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x09ed,&u1noise_l);
			 fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x09ee,&u1noise_h);
			 u4noise_power=((u1noise_h&0x3f)<<8)+u1noise_l;
			  if(u4signal_power==0)
                  {
                   u4signal_power=1;//Should be >0 in log calculation
                  }
               if(u4noise_power==0)
               {
               u4Data=Log10_Convert((u4signal_power)/(u4noise_power+1));
                }
               else
                {
                 mcSHOW_DBG_MSG(("Signal_pwr=%d,noise_power=%d,Log10_Convert(%d)=%d\n",u4signal_power,u4noise_power,(u4signal_power)/(u4noise_power),Log10_Convert((u4signal_power*10000)/(u4noise_power*10000))));
                u4Data=Log10_Convert((u4signal_power)/(u4noise_power));
                }
             }
	     }
	 fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x084d,0);//1bit the register is used to make sure that several  registers for the same variable are latched and read at the same instant
	 *u2SNR=(UINT16)u4Data;

 }
 return fgRet;
 	
}

BOOL ATBM_DTMB_GetTPSIndex(pDTMB_Demod_Ctx pDemodCtx,UINT8* u1TPSIndex)
{
  BOOL fgRet=TRUE;
  UINT8 u1Data;
  
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0d04,&u1Data);
  * u1TPSIndex=u1Data;
  
  return fgRet;
}
BOOL ATBM_DTMB_GetCFO(pDTMB_Demod_Ctx pDemodCtx,INT32* pu4CfoKHz)
{
  BOOL fgRet=TRUE;
  UINT8 u1Data;
  UINT32 u4Data;

  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x084d,1);////Lock register value
  
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0827,&u1Data);//LSB
  u4Data=u1Data;
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0828,&u1Data);
  u4Data=(u4Data<<8)| u1Data;
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0829,&u1Data);
  u4Data=(u4Data<<8)| u1Data;
  
  fgRet&=ATBMWrite(pDemodCtx->u1I2cAddr,0x084d,0);////Lock register value
  
  u4Data&=0x3FFFFF;
  
  if(u4Data & 0x200000)//negative
  {
  u4Data=(0x3fffff+1-u4Data)*1434/10000;
  *pu4CfoKHz=(INT32)(u4Data)*(-1);
  }
  else
  {
  u4Data=(0x3fffff+1-u4Data)*1434/10000;
  *pu4CfoKHz=(INT32)u4Data;
  }
  *pu4CfoKHz/=1000;
   return fgRet;
}

BOOL ATBM_DTMB_GetQAMSize(pDTMB_Demod_Ctx pDemodCtx,UINT16 * pu2Qamsize)
{
   BOOL fgRet=TRUE;
  UINT8 u1Data=0;
  
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0d04,&u1Data);//5bit
   mcSHOW_DBG_MSG(("Qamsize Data=%d\n",u1Data));
  u1Data&=0x1F;
  
  if((u1Data==3) || (u1Data==4))//4QAM-NR
  {
  * pu2Qamsize=3;//4QAM-NR
  }
  else if((u1Data>4) && (u1Data<11))//4QAM
  {
  * pu2Qamsize=4;
   mcSHOW_DBG_MSG(("QamSize is 4\n"));
  }
   else if((u1Data>10) && (u1Data<17))//16QAM
  {
  * pu2Qamsize=16;
   mcSHOW_DBG_MSG(("QamSize is 16\n"));
  }
     else if((u1Data>16) && (u1Data<19))//32QAM
  {
  * pu2Qamsize=32;
  mcSHOW_DBG_MSG(("QamSize is 32\n"));
  }
  else if((u1Data>18) && (u1Data<25))//64QAM
  {
  * pu2Qamsize=64;
   mcSHOW_DBG_MSG(("QamSize is 64\n"));
  }
  else
  {
  * pu2Qamsize=0;
  }
   return fgRet;
}
//Return PN no 01 PN420  10 PN595 11 PN945 
BOOL ATBM_DTMB_GetPNMode(pDTMB_Demod_Ctx pDemodCtx,UINT8 * u1PnNo)
{
   BOOL fgRet=TRUE;
   fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x082d,u1PnNo);//2bit
   *u1PnNo&=0x03;
   return fgRet;
}
//Return if current signal is single carrier
BOOL ATBM_DTMB_IsSC(pDTMB_Demod_Ctx pDemodCtx,BOOL * fgIsSC)
{
   BOOL fgRet=TRUE;
  fgRet&=ATBMRead(pDemodCtx->u1I2cAddr,0x0D0F,fgIsSC);//Single carrier 1bit
  * fgIsSC&=0x01;
  return fgRet;
}
