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
 * $RCSfile: pi_tuner_teqe3.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#define cVIF_TARGET_FREQ    -3375  //kHz
#define cPGA_GAIN_IDX_MIN   0x00
#define cPGA_GAIN_IDX_MAX   0x12

#define cPWM_TUNER_30V_FREQ_Hz   800000   //  800Khz
#define cPWM_TUNER_30V_DUTY   0x06      //Duty Cycle = cPWM_TUNER_30V_DUTY / cPWM_TUNER_30V_DUTYALL 
#define cPWM_TUNER_30V_DUTYALL   0x09 
#define cLOW_IF_BOUNDARY 		10000
#if TVD_SUPPORT_SNOW_MODE
#define cTVD_SNOW_INTENSITY  20
#endif
extern UINT16 g_CPR_delay;
extern UINT16 g_CPR_cnt;
extern UINT16 g_pra_cnt;
extern UINT16 g_CR_BW_AutoConfig_Delay_cnt;

EXTERN BOOL DeScrambleBox;

UINT32 u4LastRfFreq = 0;
UINT32 u4LastColorSystem=0;

STATIC UINT16 u1ArrRegisterMap[]=
{
//module                                 start_addr                                           len
MODULE_MASK_ADCIF, REG_ATD_ADCIF_CFG,      4,//0x120~0x123
MODULE_MASK_ADCIF, REG_ATD_SLD_ADCIF_CFG1, 14,//0x280~0x28d
MODULE_MASK_TVDIF, REG_ATD_TVDIF_CFG0,     4,//0x124~0x127 
MODULE_MASK_TVDIF, REG_ATD_TVDIF_CFG4,     8,//0x1e0~0x1e7
MODULE_MASK_DC,    REG_ATD_ATD_DC_RFA2,    4,//0x128~0x12B  
MODULE_MASK_EQ,    REG_ATD_EQ_CFG,         4,//0x12c~0x12f
MODULE_MASK_CTL,   REG_ATD_CTL_SYS1,       16,//0x130~0x13F
MODULE_MASK_DRO,   REG_ATD_DRO_LP_CFG01,   28,//0x164~0x17F
MODULE_MASK_DRO,   REG_ATD_DRO_OBS2,     32,//0x200~0x21F
MODULE_MASK_DRO,   REG_ATD_DRO_MAG_NOISE_0,32,//0x230~0x24F
MODULE_MASK_DRO,   REG_ATD_DRO_VSI_CFG1,   16,//0x270~0x27F
MODULE_MASK_POA,   REG_ATD_POA_CFG,        16,//0x180~0x18F
MODULE_MASK_CR,    REG_ATD_ACR_PN_CFG0,    48,//0x190~0x1BF
MODULE_MASK_CR,    REG_ATD_ADVPS_CFG0,     32,//0x250~0x26F
MODULE_MASK_AVS,   REG_ATD_AVS_RFA3,       8,//0x1c0~0x1c7
MODULE_MASK_VOP,   REG_ATD_VOP_FWCPOIH,    8,//0x1c8~0x1cf
MODULE_MASK_CPR,   REG_ATD_CPR_EXT_FREQ1,  8,//0x1d0~0x1d7
MODULE_MASK_AUC,   REG_ATD_AUC_CFG1,       8,//0x220~0x227
MODULE_MASK_DAGC,  REG_ATD_SLD_DAGC_00,    32,//0x290~0x2AF
MODULE_MASK_PRA,   REG_ATD_SLD_PRA_00 ,    79,//0x2b0~0x2fe
};
STATIC const CHAR * sModule[]=
{
  	"ADCIF",
	"TVDIF",
	"DC",
	"EQ",
	"CTL",
	"DRO",
	"POA",
	"CR",
	"AVS",
	"VOP",
	"CPR",
	"AUC",
	"DAGC",
	"PRA"
};
STATIC UINT16 u2HiddenRegIdx[]=
{
//Start idx ,   len
 0, 4,
11, 7,
21, 1,
29, 1,
30, 4,
35, 3,
59, 1,
63, 6,
81, 1,
82,8,
93, 2,
103,4,
114,2,
120,1,
123,4,
132,2,
155,7,
163,2,
168,4,
174,4,
190,12,
205,6,	
222,3,
228,1,
232,2,
235,1,
237,3,
250,8,
258,1,
263,3,
270,4,
278,2,
284,6,
308,2,
312,10,
329,3,
354,17,
372,1,
374,1,
376,2,
392,1,
400,1
};
UINT8 ATD_ChipInit(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucData;
    //   return 0;
/*
    if(IS_XTAL_54MHZ())
    {
        vDrvSetPWM(SrcPWM0, 0x00, 0x0e, 0x31);
        mcSHOW_USER_MSG(("IS_XTAL_54MHZ set pwm 30v"));
    }
    else// if(IS_XTAL_27MHZ())
    {
        vDrvSetPWM(SrcPWM0, 0x00, 0x5d, 0x95);
        mcSHOW_USER_MSG(("IS_XTAL_27MHZ set pwm 30v"));
    }
    
    IO_WRITE32(0x2000d000, 0x21c, x00030001);
    */

    // Analog FE setting
#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    // ADC Setup
    //IO_WRITE32(0xF0061000, 0x504, 0x0008A894); 
    //IO_WRITE32(0xF0061000, 0x400, 0x00AAA882); 
   // IO_WRITE32(0xF0061000, 0x500, 0x00000102); 
    IO_WRITE32(0xF0061000, 0x500, 0x00000100);//d110804_Haibo:Power on PGA (0xf0061500[2:1]=00)
    IO_WRITE32(0xF0061000, 0x504, 0x0008A694); //d110421_Haibo:change from 0x0008A894 to 0x0008A694 as CC Hou suggested
    IO_WRITE32(0xF0061000, 0x400, 0x00AAA882);//d110804_Haibo:Power on ADC(0xF0061400[0]=0) 
    //IO_WRITE32(0xF0061000, 0x400, 0x00AAA882); 
    //IO_WRITE32(0xF0061000, 0x400, 0x00AAA882); 
    // Demod Clock
    IO_WRITE32(0xF000D000, 0x204, 0x00000300);     //From xtal(27MHz) // IO_WRITE32(0xF000D000, 0x204, 0x00000100);
    IO_WRITE32(0xF000D000, 0x304, 0x00000001); 
    IO_WRITE32(0xF000D000, 0x218, 0x00000001); 
    IO_WRITE32(0xF000D000, 0x21C, 0x00000002); 
    IO_WRITE32(0xF000D000, 0x300, 0x00000002); 
    IO_WRITE32(0xF000D000, 0x308, 0x00000001); 
    IO_WRITE32(0xF000D000, 0x30C, 0x00000001); 
	    // Demod mode
   #if defined(CC_MT5396) || defined(CC_MT5368)//d110617_Haibo:Only MT5396/68 need to switch register base 
   #if defined(DTD_INCLUDE)//NA mode.Base is 0x2B
   IO_WRITE32(0xF002B000, 0x4B4, 0x000F0000);  
	   #if 0
   IO_WRITE32(0xF002B000, 0xE00, 0x000010C0); 
	   #else
	   IO_WRITE32(0xF002B000, 0xE00, 0x000010C2);//lei_code_dB110810 for CAM 10.7M clock issue.
	   #endif
   #else//EU mode.Base is 0x2A
    IO_WRITE32(0xF002A000, 0x4B4, 0x000F0000);  
	   #if 0
    IO_WRITE32(0xF002A000, 0xE00, 0x00001040); 
	   #else
	   IO_WRITE32(0xF002A000, 0xE00, 0x00001042);//lei_code_dB110810 for CAM 10.7M clock issue.
       #endif
   #endif
   #else//MT5389
   RegWrite8(REG_ATD_DEMOD_GPM_E00,0xC0);//NA ATD mode
   #endif
   //d110907_Haibo:Reset CDC(ana if)
   ATD_GetReg(psDemodCtx,REG_ATD_DEMOD_GPM_E01,&ucData,1);
   ucData&=0xEF;
   ATD_SetReg(psDemodCtx,REG_ATD_DEMOD_GPM_E01,&ucData,1);
   ucData|=0x10;
   ATD_SetReg(psDemodCtx,REG_ATD_DEMOD_GPM_E01,&ucData,1);
   mcSHOW_DBG_MSG(("Reset Demod Ana IF\n"));
#else    
    RegWrite8(REG_ATD_AREG00, 0x82);
    RegWrite8(REG_ATD_AREG01, 0xa8);
    RegWrite8(REG_ATD_AREG02, 0x5a);
    RegWrite8(REG_ATD_AREG03, 0x00);
    RegWrite8(REG_ATD_AREG04, 0x00);
    RegWrite8(REG_ATD_AREG05, 0x00);
    RegWrite8(REG_ATD_AREG06, 0x01);
    RegWrite8(REG_ATD_AREG07, 0x00);
    RegWrite8(REG_ATD_AREG08, 0x00);
    RegWrite8(REG_ATD_AREG09, 0x95);
    RegWrite8(REG_ATD_AREG10, 0xa8);
    RegWrite8(REG_ATD_AREG11, 0x09);
    //IO_WRITE32(0xF002B000, 0x100, 0x005aa882); 
    //IO_WRITE32(0xF002B000, 0x104, 0x00010000);
    //IO_WRITE32(0xF002B000, 0x108, 0x09a89500);
    //PDWNC
    IO_WRITE32MSK(0xF0028000, 0x0DC, 0x0d000084, 0x000000F0); // Xtal driving
    // CKGEN
    IO_WRITE32MSK(0xF000D000, 0x204, 0x00000300, 0x00000300); // C27M Clock, 11: c27m_ck = xtal_ck
    IO_WRITE32(0xF000d000, 0x21C, 0x00000001); //ATD54M Clock	
    IO_WRITE32(0xF000d000, 0x218, 0x00000001); //ATDAD Clock	
#endif

    // pinmux selection    
#ifdef 	CC_MT5395
  //Tuner IFAGC 0xF000D404[6]:1
  IO_WRITE32MSK(0xF000D000, 0x404, 0x00000040, 0x00000040); //0xD404[6] 
  //Tuner RF_AGC 0xF000D404[7]:1
  IO_WRITE32MSK(0xF000D000, 0x404, 0x00000080, 0x00000080); //0xD404[7] 
  //PWM30V PWM0
  #if defined(CC_Tuner30V)
  IO_WRITE32MSK(0xF000D000, 0x404, 0x00000003, 0x00000002); //0xD404[1:0]  function2 PWM0
  IO_WRITE32(0xF0008000, 0x230, 0x59300001); // PWM0 = 1.6M 
   mcSHOW_DBG_MSG(("Tuner PWM30V  Enable for MT5395 SAWLess ATD\n"));
   #endif
#elif defined(CC_MT5365)//65/66
    if (DRVCUST_InitGet(eFlagDDRQfp)) // for QFP
    {
	
	if(psDemodCtx->pTCtx->fgRFAGC == TRUE)
			IO_WRITE32MSK(0xF000D000, 0x400, 0x00000100, 0x00000100); //bit8 RF AGC
			
 if(!psDemodCtx->pTCtx->fgRFTuner)       // legacy tuner IF AGC 
 {      
        IO_WRITE32MSK(0xF000D000, 0x400, 0x00001000, 0x00003000); //MX0[13:12]   
		
        mcSHOW_DBG_MSG(("Tuner IF AGC Enable for MT5365 LEGACY ATD\n"));
  }	 
 #if defined(CC_Tuner30V)
   else     // tuner 30V PWM, use drv API instead of direct reg access
{
if (DRVCUST_OptGet(eChinaATVTuner30v))  //liuyuan add//define at mt5365_driver/drv_cust/mtk/config/mt5365_china_atv.h
        IO_WRITE32MSK(0xF000D000, 0x414, 0x01800000, 0x03800000);  //MT5365_P1V1_TUNER_30V use pimx5[25:23],fuction3
else
        IO_WRITE32MSK(0xF000D000, 0x400, 0x00002000, 0x00003000); //MX0[13:12]   
         
		//IO_WRITE32(0xF0008000, 0x234, 0x59300001); // PWM1 = 1.6M
        //IO_WRITE32(0xF0008000, 0x234, 0xff801601); // PWM1 = 48k 
        mcSHOW_DBG_MSG(("Tuner PWM30V Enable for MT5365 SAWLess ATD\n"));
        vDrvSetPWM( SrcPWM1,((BSP_GetDomainClock(SRC_BUS_CLK)/((cPWM_TUNER_30V_FREQ_Hz)*(cPWM_TUNER_30V_DUTYALL+1)))-1),
                    cPWM_TUNER_30V_DUTY,cPWM_TUNER_30V_DUTYALL);
        // tuner I2C
   }
   #endif
    }
    else  // for BGA
    {
      if(!psDemodCtx->pTCtx->fgRFTuner)       // legacy tuner IF AGC 
    {
        IO_WRITE32MSK(0xF000D000, 0x400, 0x00001000, 0x00003000); //MX0[13:12], IF AGC

        mcSHOW_DBG_MSG(("Tuner IF AGC Enable for MT5366 LEGACY ATD\n"));
	}	
	   #if defined(CC_Tuner30V)
     else//sawless tuner
    {       // tuner 30V PWM, use drv API instead of direct reg access
        IO_WRITE32MSK(0xF000D000, 0x40C, 0x10180000, 0x07000000); //MX3[26:24]
        //IO_WRITE32MSK(0xF000D000, 0x41C, 0x00012001, 0x00010000); //MXMISC[16]
        //IO_WRITE32(0xF0008000, 0x238, 0x59300001); // PWM1 = 1.6M
        mcSHOW_DBG_MSG(("Tuner PWM30V Enable for MT5366 SAWLess ATD\n"));
        vDrvSetPWM(SrcPWM2,((BSP_GetDomainClock(SRC_BUS_CLK)/((cPWM_TUNER_30V_FREQ_Hz)*(cPWM_TUNER_30V_DUTYALL+1)))-1),
                    cPWM_TUNER_30V_DUTY,cPWM_TUNER_30V_DUTYALL);

        //IO_WRITE32(0xF0008000, 0x238, 0xfe8000b1); // PWM2 = 48k
        // tuner I2C, use PAD_OSCL0
        //IO_WRITE32MSK(0xF000D000, 0x400, 0x42A02159, 0x000000C0); //MX0[7:6]
        // tuner I2C, use PAD_OSCL1
        //IO_WRITE32MSK(0xF000D000, 0x400, 0x42A02159, 0x0000000C); //MX0[3:2]        
     }
	 #endif
    }
#elif defined(CC_MT5396) || defined(CC_MT5368) //96
  //Tuner IFAGC 0xF000D408[27]:1
  IO_WRITE32MSK(0xF000D000, 0x408, 0x08000000, 0x08000000); //0xD408[27]  PMUX2[27]
  //Tuner RF_AGC 0xF000D404[19]:1
  IO_WRITE32MSK(0xF000D000, 0x404, 0x00080000, 0x00080000); //0xD404[19]  PMUX1[19]
   #if defined(CC_Tuner30V) 
   //PWM30V PWM0
  //d110331_Haibo:ToDo:Set PWM frequency
   vDrvSetPWM(SrcPWM0,((BSP_GetDomainClock(SRC_BUS_CLK)/((cPWM_TUNER_30V_FREQ_Hz)*(cPWM_TUNER_30V_DUTYALL+1)))-1),cPWM_TUNER_30V_DUTY,cPWM_TUNER_30V_DUTYALL);
   mcSHOW_DBG_MSG(("tuner30V--PWM0\n"));// PWM0 pin will be use to  TCL F20WT tuner 30V for 5368 not RFAGC switch
   #else
   {//d110526_Haibo:Fix build warning:ISO C90 forbids mixed declarations and code
   // RF AGC switch
   UINT32 u4GpioforRFAGCSwitch;
   if(DRVCUST_OptQuery(eTunerRFAGCSwitchGpio,&u4GpioforRFAGCSwitch)==0)
   {
     GPIO_SetOut(u4GpioforRFAGCSwitch,0);//d20110503_Haibo:Gpio set low for ATV
	 mcSHOW_DBG_MSG(("GPIO %d set low to switch RF AGC control\n",u4GpioforRFAGCSwitch));
   }
	}
    #endif
#elif defined(CC_MT5389)
// d110617_Haibo:Please refer to mt5389m1v2_pin_setup.h to get IF/RF AGC pinmux setting
#if defined(CC_Tuner30V)
 vDrvSetPWM(SrcPWM1,((BSP_GetDomainClock(SRC_BUS_CLK)/((cPWM_TUNER_30V_FREQ_Hz)*(cPWM_TUNER_30V_DUTYALL+1)))-1),
                    cPWM_TUNER_30V_DUTY,cPWM_TUNER_30V_DUTYALL);//MT5389 tuner 30V  use PWM1	
 mcSHOW_DBG_MSG(("Set OPWM1 for tuner 30V \n"));
#endif					
#endif   
 
    //Power on clk
    RegWrite8(REG_ATD_CTL_TEST, 0x00); //REG_ATD_CTL_TEST, all clk power on
    
	#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
		 //Set MT5396 RFAGC and IFAGC to normal mode  (0xF62_b3: RF AGC PAD and 0xF62_b2: IF AGC PAD) //d110331_Haibo:RF/IF hi-z register alternation
    ATD_GetReg(psDemodCtx, REG_ATD_SLD_CFG_1, &ucData, 1);
    ucData &= 0xF3;
    ucData |= 0x0C;
    ATD_SetReg(psDemodCtx, REG_ATD_SLD_CFG_1, &ucData, 1);
	#else
    //Set MT5365 RFAGC and IFAGC PAD to enable  (0x124_b3: RF AGC PAD and 0x124_b2: IF AGC PAD) //EC_code_dA414 for eIntATD_ExtDTD
    ATD_GetReg(psDemodCtx, REG_ATD_TVDIF_CFG0, &ucData, 1);
    ucData &= 0xF3;
    ucData |= 0x0C;
    ATD_SetReg(psDemodCtx, REG_ATD_TVDIF_CFG0, &ucData, 1);
    #endif
    return 0;
}


void ATD_SetIF(ATD_CTX_T *psDemodCtx)
{
    INT32 s4DsmOutFreq;
    INT16 s2IfCtrlWord;
    UINT8 ucData;
	INT32 s4IfCtrlWordtFreq;

	if(psDemodCtx->pTCtx->u2IF_Freq_A < cLOW_IF_BOUNDARY)
	{
		s4DsmOutFreq = (INT32)(-psDemodCtx->pTCtx->u2IF_Freq_A);
		RegWrite8(REG_ATD_SLD_ADCIF_DSM4A, 0xa0);
	  ucData = 0xA0;
	  ATD_SetReg(psDemodCtx, REG_ATD_SLD_ADCIF_DSM4A, &ucData, 1);
	}
	else
    	s4DsmOutFreq = (INT32)(-psDemodCtx->pTCtx->u2IF_Freq_A) +54000 -13500;  // vedio carrier freq after DSM out

    // calculate IF ctrl word
    if (psDemodCtx->u1SubSysIdx == MOD_SECAM_L1) {
        ucData = 0x98; // spectrum inverse
        s4DsmOutFreq = -cVIF_TARGET_FREQ - s4DsmOutFreq; // freq shift in DC
    }
    else {
        ucData = 0x80;
        s4DsmOutFreq =  cVIF_TARGET_FREQ - s4DsmOutFreq; // freq shift in DC
    }
    s2IfCtrlWord = (INT16)((s4DsmOutFreq << 10) /27000); 
	
	s4IfCtrlWordtFreq = (INT32)(s2IfCtrlWord*27000);	 //LW;d100926;FM resolution issue
	if (((s4DsmOutFreq*1024)-s4IfCtrlWordtFreq) > 13312) //LW_d100929; The resolution of DC frequency is about 26.367kHz
	{
		  s2IfCtrlWord += 1;
	}
    mcSHOW_DBG_MSG(("IfCtrlWord = %X , IF_Freq = %d\n", s2IfCtrlWord, psDemodCtx->pTCtx->u2IF_Freq_A));

    // write IF ctrl word to DC
    ucData |= ((UINT8)(s2IfCtrlWord>>8) & 0x07);
    RegWrite8(REG_ATD_ATD_DC_CFG2, ucData);
    RegWrite8(REG_ATD_ATD_DC_CFG3, (UINT8)s2IfCtrlWord);

}

void ATD_SetPGA(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucData;
     UINT8 ii;

if(!psDemodCtx->pTCtx->fgRFTuner)//legacy tuner
{
    RegWrite8(REG_ATD_SLD_PRA_2F, 0x11);  // Enable IF AGC

	if(psDemodCtx->pTCtx->fgRFAGC == TRUE)
		RegWrite8(REG_ATD_SLD_PRA_2F, 0x31);  // Enable IF and RF AGC

}
else//SAWLess tuner
{
    // write PAG gain table into DAGC
    RegWrite8(REG_ATD_SLD_DAGC_0E, 0x80);  // for PGA gain dec
    for (ii = cPGA_GAIN_IDX_MIN; ii < cPGA_GAIN_IDX_MAX+1; ii++) {
        RegWrite8(REG_ATD_SLD_DAGC_0F, 0xEF);  // -0.6dB
        //RegWrite8(REG_ATD_SLD_DAGC_0F, 0xE4);  // -1.0dB
    }

    RegWrite8(REG_ATD_SLD_DAGC_0E, 0xA0);  // for PGA gain inc
    for (ii = cPGA_GAIN_IDX_MIN; ii < cPGA_GAIN_IDX_MAX+1; ii++) {
        RegWrite8(REG_ATD_SLD_DAGC_0F, 0x12);  // 0.6dB
        //RegWrite8(REG_ATD_SLD_DAGC_0F, 0x1E);  // 1.0dB
    }

    // set PGA max/min gain index
    ATD_GetReg(psDemodCtx, REG_ATD_SLD_PRA_31, &ucData, 1);
    ucData &= 0xE0;
    ucData |= cPGA_GAIN_IDX_MAX;
    ATD_SetReg(psDemodCtx, REG_ATD_SLD_PRA_31, &ucData, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_SLD_PRA_32, &ucData, 1);
    ucData &= 0xE0;
    ucData |= cPGA_GAIN_IDX_MIN;
    ATD_SetReg(psDemodCtx, REG_ATD_SLD_PRA_32, &ucData, 1);
}  

}

void ATD_SetAgcSld(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucData;
    BOOL fgNeedAdjustSLD=FALSE;

    if((psDemodCtx->pTCtx->fgRFTuner== FALSE) && (psDemodCtx->pTCtx->fgRFAGC == TRUE))
    {
        //RegWrite8(REG_ATD_SLD_PRA_24, 0xFF); // 0x2d4, RF bias
        //RegWrite8(REG_ATD_SLD_PRA_2D, 0xF0); // 0x2dd, IF min1	
        //RegWrite8(REG_ATD_SLD_PRA_27, 0xd0); // 0x2d7, RF slope
        //RegWrite8(REG_ATD_SLD_PRA_28, 0xd0); // 0x2d8, IF slope1

        RegWrite8(REG_ATD_SLD_PRA_07, 0x15); // 0x2b7
        RegWrite8(REG_ATD_SLD_PRA_09, 0x15); // 0x2b9
		RegWrite8(REG_ATD_SLD_PRA_24, 0x10); // 0x2d4
        RegWrite8(REG_ATD_SLD_PRA_25, 0xfe); // 0x2d5
        RegWrite8(REG_ATD_SLD_PRA_26, 0xfe); // 0x2d6
        RegWrite8(REG_ATD_SLD_PRA_27, 0xE0); // 0x2d7
        RegWrite8(REG_ATD_SLD_PRA_28, 0xEE); // 0x2d8
        RegWrite8(REG_ATD_SLD_PRA_29, 0xEE); // 0x2d9
        RegWrite8(REG_ATD_SLD_PRA_2A, 0x7F); // 0x2da
        RegWrite8(REG_ATD_SLD_PRA_2B, 0x7F); // 0x2db
        RegWrite8(REG_ATD_SLD_PRA_2C, 0x80); // 0x2dc
      //  RegWrite8(REG_ATD_SLD_PRA_2D, 0xF0); // 0x2dd
      //  RegWrite8(REG_ATD_SLD_PRA_2E, 0xF0); // 0x2de
        RegWrite8(REG_ATD_SLD_PRA_1B, 0x01); // 0x2cb

		RegWrite8(REG_ATD_SLD_PRA_01, 0x11); // 0x2b1
		RegWrite8(REG_ATD_SLD_PRA_02, 0x06); // 0x2b2
		RegWrite8(REG_ATD_SLD_PRA_03, 0xC0); // 0x2b3
    }
	else
{
    RegWrite8(REG_ATD_SLD_PRA_24, 0x04); // 0x2d4, RF bias
    RegWrite8(REG_ATD_SLD_PRA_25, 0x04); // 0x2d5, IF bias1: 0xE0 --> 0x04
    RegWrite8(REG_ATD_SLD_PRA_26, 0x04); // 0x2d6, IF bias2: 0xE0 --> 0x04

    RegWrite8(REG_ATD_SLD_PRA_27, 0xec); // 0x2d7, RF slope
    RegWrite8(REG_ATD_SLD_PRA_28, 0xF4); // 0x2d8, IF slope1: 0xD0 --> 0xF4
    RegWrite8(REG_ATD_SLD_PRA_29, 0xF4); // 0x2d9, IF slope2: 0xD0 --> 0xF4

    RegWrite8(REG_ATD_SLD_PRA_2A, 0x7f); // 0x2da, RF max
    RegWrite8(REG_ATD_SLD_PRA_2B, 0x7f); // 0x2db, IF max
    RegWrite8(REG_ATD_SLD_PRA_2C, 0x80); // 0x2dc, RF min
//
	// 100809;[DTV00091902]; transient issue
//	switch (psDemodCtx->u1SubSysIdx)	
//	{
//	case MOD_PAL_BG: 
//	case MOD_PAL_DK: 
//	case MOD_PAL_I:
//		ucData = 0x02;
//	    ATD_SetReg(psDemodCtx, REG_ATD_SLD_PRA_1B, &ucData, 1);
//		break;
//    case MOD_SECAM_L:
//    case MOD_SECAM_L1:
//        RegWrite8(REG_ATD_SLD_PRA_1B, 0x10); // 0x2cb
//       break;
//	default :
//		ucData = 0x00;	
//	    ATD_SetReg(psDemodCtx, REG_ATD_SLD_PRA_1B, &ucData, 1);
//		break;
//	}
}
    if(psDemodCtx->pTCtx->fgRFTuner)//sawless tuner
	{
	 RegWrite8(REG_ATD_SLD_PRA_1A, 0x77); // 0x2ca
	}
	else
	{
	 RegWrite8(REG_ATD_SLD_PRA_1A, 0x88); // 0x2ca
}
	#if defined(CC_MT5389)
	fgNeedAdjustSLD=TRUE;
	#elif defined(CC_MT5396)
    if(IC_VER_5396_AA==BSP_GetIcVersion())//MT5396 before ECO
    {
	 fgNeedAdjustSLD=TRUE;
    }
	#endif
	if(fgNeedAdjustSLD)
	{
	 if(psDemodCtx->pTCtx->u2IF_Freq_A < cLOW_IF_BOUNDARY)//Low if
	 {
	    RegWrite8(REG_ATD_SLD_PRA_07, 0x10); // PRA target
		RegWrite8(REG_ATD_SLD_PRA_09, 0x10); // PRA target
		RegWrite8(REG_ATD_SLD_DAGC_00,0x08); // DAGC setting
		RegWrite8(REG_ATD_SLD_DAGC_04,0x2a);
		RegWrite8(REG_ATD_SLD_DAGC_14,0x2a);
		
		RegWrite8(REG_ATD_CTL_SYS2,0x80);//0x13b
		RegWrite8(REG_ATD_PE_CFG0,0x17);//null region
		//secam
		 if ((psDemodCtx->u1SubSysIdx == MOD_SECAM_L1) || (psDemodCtx->u1SubSysIdx==MOD_SECAM_L)) 
		 {
		 RegWrite8(REG_ATD_SLD_PRA_07, 0x05); // PRA target
		 RegWrite8(REG_ATD_SLD_PRA_09, 0x05); // PRA target
		 RegWrite8(REG_ATD_SLD_DAGC_00,0x88); // DAGC setting
		 RegWrite8(REG_ATD_CTL_SYS2,0x81);//0x13b
		 }
	 }
	 else//normal IF
	 {
		RegWrite8(REG_ATD_SLD_DAGC_00,0x48); // DAGC setting
		RegWrite8(REG_ATD_SLD_DAGC_04,0x38);
		RegWrite8(REG_ATD_SLD_DAGC_14,0x38);		
		RegWrite8(REG_ATD_PE_CFG0,0x27);//null region
		
		//sawless tuner has different PRA target level.Need hysteresis
		if(psDemodCtx->pTCtx->fgRFTuner)
		{
		RegWrite8(REG_ATD_SLD_PRA_07, 0x15); // PRA target
		RegWrite8(REG_ATD_SLD_PRA_09, 0x10); // PRA target
		}
		else
		{
		RegWrite8(REG_ATD_SLD_PRA_07, 0x15); // PRA target
		RegWrite8(REG_ATD_SLD_PRA_09, 0x15); // PRA target
		}
		//secam
		 if ((psDemodCtx->u1SubSysIdx == MOD_SECAM_L1) || (psDemodCtx->u1SubSysIdx==MOD_SECAM_L)) 
		 {
		    if(psDemodCtx->pTCtx->fgRFTuner)
			{
			  RegWrite8(REG_ATD_SLD_PRA_07, 0x10); // PRA target
		      RegWrite8(REG_ATD_SLD_PRA_09, 0x0C); // PRA target
			}
			else
			{
			  RegWrite8(REG_ATD_SLD_PRA_07, 0x09); // PRA target
		      RegWrite8(REG_ATD_SLD_PRA_09, 0x09); // PRA target
			}
		    RegWrite8(REG_ATD_SLD_DAGC_00,0xC8); // DAGC setting
		 }
	 }
}
}                                     

void ATD_SetTunerRel(ATD_CTX_T *psDemodCtx)
{
ATD_TUNER_REL_REG_T * pTunerRelReg;
UINT8 ucData;

mcSHOW_DBG_MSG(("ATD_SetTunerRel\n"));

	if (ITuner_OP(psDemodCtx->pTCtx, itSetIfMin, psDemodCtx->u1SubSysIdx, &ucData) == ITUNER_OK)
    {
        RegWrite8(REG_ATD_SLD_PRA_2D, ucData); // 0x2dd, IF min1
        RegWrite8(REG_ATD_SLD_PRA_2E, ucData); // 0x2de, IF min2
    }
    else
    {
        RegWrite8(REG_ATD_SLD_PRA_2D, 0x80); // 0x2dd, IF min1
        RegWrite8(REG_ATD_SLD_PRA_2E, 0x80); // 0x2de, IF min2
    }

 if (ITuner_OP(psDemodCtx->pTCtx, itGetTunerRelReg, psDemodCtx->u1SubSysIdx, &pTunerRelReg) == ITUNER_OK)
 {
   if(pTunerRelReg)
   {
   while(pTunerRelReg->u2RegAddr!=0)
   {
   mcSHOW_DBG_MSG(("RegWrite 0x%04X 0x%02X\n",pTunerRelReg->u2RegAddr,pTunerRelReg->u1RegVal));
   ATD_SetReg(psDemodCtx,pTunerRelReg->u2RegAddr, &pTunerRelReg->u1RegVal,1);
   pTunerRelReg++;
   }
   }
 }
}
#if defined(CC_MT5396)
void ATD_ApplyECOSetting(ATD_CTX_T *psDemodCtx,UINT8 u1SubSysIdx)
{
   UINT16 ii=0;
   UINT16 u2SetSystemEcoRegLen = sizeof(ATD_SetSystem_96ECO_Reg)/sizeof(ATD_REG_T);
   UINT8 ucData;
   UINT8 u1TvSpec = u1SubSysIdx - MOD_ANA_TYPE_BEGIN;
   mcSHOW_DBG_MSG(("u2SetSystemEcoRegLen = %d, u1TvSpec = %d\n", u2SetSystemEcoRegLen, u1TvSpec));

   for (ii = 0; ii < u2SetSystemEcoRegLen; ii++) 
	  {
        RegWrite8(ATD_SetSystem_96ECO_Reg[ii].Addr, ATD_SetSystem_96ECO_Reg[ii].RegVal[u1TvSpec]);        
      }
}
#endif
void ATD_SetSystem(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx)
{   
    UINT8 ucData;
    UINT16 ii;
    UINT8 u1TvSpec;
    UINT16 u2SetSystemRegLen = sizeof(ATD_SetSystem_Reg)/sizeof(ATD_REG_T);
	if(u1SubSysIdx==MOD_TYPE_UNKNOWN)//scan mode
	{
	    u1SubSysIdx=MOD_PAL_BG;   
	}
    psDemodCtx->u1SubSysIdx = u1SubSysIdx;
    u1TvSpec = u1SubSysIdx - MOD_ANA_TYPE_BEGIN;

    if ((u1SubSysIdx < MOD_ANA_TYPE_BEGIN) || (u1SubSysIdx > MOD_NTSC_M)) {
        mcSHOW_USER_MSG(("Wrong TvSpec = %d !!!!\n", u1SubSysIdx));
        return;
    }

    // reset ATD datapath and registers
    //RegWrite8(REG_ATD_CTL_CFG, 0x00);  // remove to avoid video flash when ch scan

    mcSHOW_DBG_MSG(("u2SetSystemRegLen = %d, u1TvSpec = %d\n", u2SetSystemRegLen, u1TvSpec));
    for (ii = 0; ii < u2SetSystemRegLen; ii++) {
//        mcSHOW_DBG_MSG(("addr = 0x%03X, value = 0x%02X\n", ATD_SetSystem_Reg[ii].Addr, ATD_SetSystem_Reg[ii].RegVal[u1TvSpec]));
        RegWrite8(ATD_SetSystem_Reg[ii].Addr, ATD_SetSystem_Reg[ii].RegVal[u1TvSpec]);        
    }
    ATD_SetAgcSld(psDemodCtx);
    ATD_SetPGA(psDemodCtx);
    ATD_SetIF(psDemodCtx);
    ATD_SetEQ(psDemodCtx, itGetEqScriptNormal);
    ATD_SetNormalMode(psDemodCtx); 
	ATD_SetTunerRel(psDemodCtx);
    #if defined(CC_MT5396)
	 if(IC_VER_5396_AA==BSP_GetIcVersion())//MT5396 before ECO
	 {
	   ATD_ApplyECOSetting(psDemodCtx,u1SubSysIdx);//d110922_Haibo:Load default register setting for MT5396IC before MP
	 }
	#endif 
}



UINT8 ATD_StartAcq(ATD_CTX_T *psDemodCtx, UINT8 fgScanMode)
{

    UINT8 ii, ucData,ucData3[3];
    UINT32 u4PilotPower,u4SignalPower;
    UINT32 u4ColorSystem=0;

    ATD_SetPF(psDemodCtx, psDemodCtx->u1PF);
    if(0x00 == (psDemodCtx->u1PF))
    {
      ATD_SetNullRegion(psDemodCtx,1);
      g_CPR_cnt = 18;  //PA advice:70ms is enough for CR loop tracking
	}
    else
    {
       g_CPR_cnt=0;
    }
    if(fgScanMode)
    {
        ATD_SetScanMode(psDemodCtx);

        switch (psDemodCtx->u1SubSysIdx)	  
        {
            case MOD_PAL_BG: 
            case MOD_PAL_DK: 
            case MOD_PAL_I:
              u4ColorSystem=0;//negative modulation
            	 break;
            case MOD_SECAM_L:
            case MOD_SECAM_L1:
             u4ColorSystem=1;//positive modulation
            break;
            default :
            u4ColorSystem=0;//negative modulation
            break;
        }

        // do not reset demod if RF freq is not changed to avoid video flash
        if (u4LastRfFreq == psDemodCtx->pTCtx->u4RF_Freq && u4LastColorSystem == u4ColorSystem)
        {
		    mcSHOW_DBG_MSG3(("Same frequency,same modulation method ! ignore tuner acq, freq = %d\n", u4LastRfFreq));
		    return 0;
        }
        else 
        {
			u4LastRfFreq = psDemodCtx->pTCtx->u4RF_Freq;
			u4LastColorSystem=u4ColorSystem;//diff clor system detect :,pal VS secam
        }
    }
    //ATD_SetPF(psDemodCtx, psDemodCtx->u1PF);

	ATD_SetCrBwAutoCfg(psDemodCtx,FALSE);//d111108_Haibo:Disable CRBW auto config due to it will effect cr tracking speed

    ucData = 0x00;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST1, &ucData, 1);
	#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
	ATD_SetReg(psDemodCtx, REG_ATD_SLD_RST, &ucData, 1);//reset SLD part
	#endif
    //ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);

    //Release PRA & CR,....
    ucData = 0xFF;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST1, &ucData, 1);
	ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);    //for Channel scan balck mute CR
    #if defined(CC_MT5396)   || defined(CC_MT5368) || defined(CC_MT5389)
    ATD_SetReg(psDemodCtx, REG_ATD_SLD_RST, &ucData, 1);
    #endif
    //PRA LOOP
    //CheckTmp = 0;
   // PGA gain fix min, move from SetPGA because the register will be default after reset ATD
    if(!psDemodCtx->pTCtx->fgRFTuner)//legacy tuner use PGA gain 0x00     
    {
        ucData = 0x00;
        ATD_SetReg(psDemodCtx, REG_ATD_SLD_PRA_30, &ucData, 1);
    }
    //RegWrite8(REG_ATD_SLD_PRA_30, 0x00);  // PGA gain fix min
    for (ii = 0; ii < 50; ii ++)
    {
        mcDELAY_MS(10);
        ATD_GetReg(psDemodCtx, REG_ATD_SLD_PRA_34, &ucData, 1);//for 8223 2E4[6:5]=10 means pra tracking state
        if ((ucData & 0x60) == 0x40) 
        {
            if((psDemodCtx->pTCtx->fgRFTuner==TRUE) )//only for sawless tuner ,
			{	
			    ATD_GetRFAGC(psDemodCtx);  //show RFAGC message
                ATDPI_GetIFAGC(psDemodCtx); //show IFAGC message(show PAG Gain for sawless tuner)
  
                if(ii > g_pra_cnt)
                {
                    break;
                }
			}
			else 
			{
                break;
            }
            
            mcSHOW_DBG_MSG(("PRA lock!, cnt = %d\n", ii));
        }
    }
	
    if(ii==50)
    {
        mcSHOW_USER_MSG(("PRA acquisition fail!\n"));
        return 1;
    }
	
   //Reset CPR
	ucData = 0x10;  //don't reset DRO module
	ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
	mcSHOW_DBG_MSG(("reset CPR!\n"));

    //Release CRP....
    ucData = 0xFF;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);

    //CPR LOOP
    for (ii = 0; ii < 20; ii ++)
    {
        mcDELAY_MS(10);
        ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE0, &ucData, 1);
        if ((ucData & 0x03) == 0x03)
        {
            mcSHOW_DBG_MSG(("ATD acquisition OK!\n"));
        
            Weak_Sinal_Indicator = 0;  // initialize driver patch variables
            Strong_Chroma_Indicator = 0;
            weak_count = 0;
          //  CCI_Indicator=0;//8223 no CCI patch
            //CCITest=0;                  	                   
            NoOVMCounter0 = cNO_OVM_THRESHOLD;
            PNCounter = cPN_CNTR_THRESHOLD;	
            ATD_GetReg(psDemodCtx, REG_ATD_PE_CFG0, &u1NullRegion, 1);      
            u4PilotPower=ATD_GetPilotPower(psDemodCtx);
            ucData = (0x07 << 3); // Select ADVPS_PROB as SignalPower by setting 0x9BB[b5:b3] 0x07
            ATD_SetReg(psDemodCtx, REG_ATD_ADVPS_CFG2 , &ucData, 1);
            ATD_GetReg(psDemodCtx, REG_ATD_ADVPS_PRB0, ucData3, 3);            
            u4SignalPower = mcUNSIGN_2BYTE(ucData3[0], ucData3[1], 8, 8);
            u4SignalPower = (u4SignalPower << 8) + ucData3[2];
            mcSHOW_DBG_MSG(("Signal Power = %08d\n", u4SignalPower));

            ATD_GetCFOKHz(psDemodCtx, TRUE);          

            if((ii>g_CPR_cnt) || (u4PilotPower>150))
            {
                 break;
            }
        }
    }

   if(0!=g_CPR_delay)
   {
     mcDELAY_MS(g_CPR_delay);
	 mcSHOW_DBG_MSG(("Delay %dms for CR lock!\n",g_CPR_delay));
   }
    if(ii==20)
    {
        mcSHOW_USER_MSG(("CR acquisition fail!\n"));
        return 1;
    }

    return 0;  
}


void ATD_SetDelayLine(ATD_CTX_T *psDemodCtx,UINT8 IsSetDelay)
{

UINT8 ucData;
       if(IsSetDelay)
	    {	
	  if (psDemodCtx->pTCtx->fgRFTuner== FALSE)
	  {ucData=0X77;
	    ATD_SetReg(psDemodCtx, REG_ATD_FIFO_CFG1, &ucData, 1);
        //RegWrite8(0x15f, 0x77);	 //enable  delay 1 line setting in MT5365 
        mcSHOW_DBG_MSG(("Set DelayLine 5365 LEGACY! \n"));}
      else 
	  {ucData=0X7C;
	    ATD_SetReg(psDemodCtx, REG_ATD_FIFO_CFG1, &ucData, 1);
	   // RegWrite8(0x15f, 0x7c);	 //enable  delay 1 line + 1.2us setting in MT5365 
        mcSHOW_DBG_MSG(("Set DelayLine 5365 SAWLESS! \n"));
       	}
       	}
	   else
	   	ucData=0X00;
	    ATD_SetReg(psDemodCtx, REG_ATD_FIFO_CFG1, &ucData, 1);
	   //	RegWrite8(0x15f, 0x00);   //disable  delay 1 line setting in MT5365

}

void ATD_SetNormalMode(ATD_CTX_T *psDemodCtx)
{
#ifdef CC_MT8223
    UINT8 ucData;
    // Reg0x197[b7-b6] set to 00 Turn-off CR auto Re-acq config
    ATD_GetReg(psDemodCtx, REG_ATD_ACR_CFG, &ucData, 1);
    ucData = ucData & 0x3F;
    ATD_SetReg(psDemodCtx, REG_ATD_ACR_CFG, &ucData, 1);
#endif
    mcSHOW_DBG_MSG(("ATD_SetNormalMode(), System = %d\n", psDemodCtx->u1SubSysIdx-MOD_ANA_TYPE_BEGIN));
    
    ATD_SetSystemAudio(psDemodCtx, psDemodCtx->u1SubSysIdx);
    /*
    switch (psDemodCtx->u1SubSysIdx)
    {
    case MOD_PAL_BG: 
        // set AVS/AUC audio filter,center:5.65M and BW(3dB)=0.9M
        ucData = 0x94; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1);
        ucData = 0xE8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1);
        ucData = 0x94; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0xAD; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);        
        break;
    
    case MOD_PAL_DK: 
        // set AVS/AUC audio filter,center:6.22M and BW(3dB)=1.256M
        ucData = 0x8A; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1); 
        ucData = 0x20; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1); 
        ucData = 0x14; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0xD8; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);
        break;
    
    case MOD_PAL_I: 
        // set AVS/AUC audio filter,center:6.31M and BW(3dB)=1.256M
        ucData = 0x88; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1); 
        ucData = 0x60; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1); 
        ucData = 0x14; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);
        break;
    
    case MOD_SECAM_L: 
        break;
    
    case MOD_SECAM_L1: 
        break;  
    case MOD_NTSC_M:
        // set AVS/AUC audio filter,center:4.5M and BW(3dB)=0.9M
        ucData = 0xAA; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1); 
        ucData = 0xE8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1); 
        ucData = 0x94; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0x55; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);
        break;         
    default:
        mcSHOW_DBG_MSG(("Set System = DEFAULT!!\n"));
        break;
    }
    
    //AVS register can not be set real time and need to reset POA
    ATD_GetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    ucData &= 0xFB;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    ucData |= 0x04;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
	
    ATD_SetDelayLine(psDemodCtx,FALSE);    
    */
}

//strong after weak patch
void ATD_AVSStrongAfterWeakM(ATD_CTX_T *psDemodCtx)
{
        UINT8 ucData;
	   //Center 6.0 BW(3dB)=1.256M/2
	   mcSHOW_DBG_MSG(("ATD_AVSStrongAfterWeakM\n"));
        ucData = 0x8E; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1); //0x1c2
        ucData = 0x64; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1); //0x1c3
        ucData = 0x14; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData, 1);//0x1c4
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);//0x220
        ucData = 0xC7; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);//0x221
    //AVS register can not be set real time and need to reset POA
    ATD_GetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    ucData &= 0xFB;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    ucData |= 0x04;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
}
void ATD_SetSystemAudio(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx)
{
	UINT8   ucData=0;
	
		ATD_GetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
    ucData = ucData | 0x08;
    ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
	
    switch (u1SubSysIdx)
    {
    case MOD_PAL_BG: 
        // set AVS/AUC audio filter,center:5.65M and BW(3dB)=0.9M
        ucData = 0x94; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1);
        ucData = 0xE8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1);
        ucData = 0x94; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0xAD; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);        
        break;
    
    case MOD_PAL_DK: 
        // set AVS/AUC audio filter,center:6.22M and BW(3dB)=1.256M
        ucData = 0x8A; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1); 
        ucData = 0x20; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1); 
        ucData = 0x14; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0xD8; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);
        break;
    
    case MOD_PAL_I: 
        // set AVS/AUC audio filter,center:6.31M and BW(3dB)=1.256M
        ucData = 0x88; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1); 
        ucData = 0x60; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1); 
        ucData = 0x14; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);
        break;
    
    case MOD_SECAM_L: 
		// set AVS/AUC audio filter,center:6.22M and BW(3dB)=1.256M
        ucData = 0x8A; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1); 
        ucData = 0x20; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1); 
        ucData = 0x14; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0xD8; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);
        break;
    
    case MOD_SECAM_L1: 
		// set AVS/AUC audio filter,center:6.22M and BW(3dB)=1.256M
        ucData = 0x8A; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1); 
        ucData = 0x20; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1); 
        ucData = 0x14; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0xD8; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);
        break;  
    case MOD_NTSC_M:
        // set AVS/AUC audio filter,center:4.5M and BW(3dB)=0.9M
        ucData = 0xAA; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1); 
        ucData = 0xE8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1); 
        ucData = 0x94; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0x55; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);
        break;         
    default:
        mcSHOW_DBG_MSG(("Set System = DEFAULT!!\n"));
        break;
    }	
    
    //AVS register can not be set real time and need to reset POA
    ATD_GetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    ucData &= 0xFB;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    ucData |= 0x04;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);
    ucData &= 0x80;
    ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);
    ucData |= 0x7F;
    ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);
    ATD_SetDelayLine(psDemodCtx,DeScrambleBox);   
	
}

void ATD_DisConnect(ATD_CTX_T *psDemodCtx)
{

    UINT8 ucData = 0;

    // reset u4LastRfFreq if ATD disconnected to make sure 0x134/135 would be enabled in StartAcq()
    u4LastRfFreq = 0;

	#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    //Set MT5396 RFAGC and IFAGC to high-Z   (0xF62_b3: RF AGC PAD and 0xF62_b2: IF AGC PAD) //d110331_Haibo:RF/IF hi-z register alternation
    ATD_GetReg(psDemodCtx, REG_ATD_SLD_CFG_1, &ucData, 1);//0xF62
    ucData &= 0xF3;
    ATD_SetReg(psDemodCtx, REG_ATD_SLD_CFG_1, &ucData, 1);   
   #else
	ATD_GetReg(psDemodCtx, REG_ATD_TVDIF_CFG0, &ucData, 1);
    ucData &= 0xF3;
    ATD_SetReg(psDemodCtx, REG_ATD_TVDIF_CFG0, &ucData, 1);
    // set RF/IF AGC pad to open
    #endif
    //Disbale ATD (b4 power down ADCLK)
    ucData = 0x00;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST1, &ucData, 1);
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
	
	#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
	ATD_SetReg(psDemodCtx, REG_ATD_SLD_RST, &ucData, 1);//d110331_Haibo:power down SLD part
	#endif
	
    mcSHOW_DBG_MSG(("[ATD_DisConnect] Disable ATD Module\n"));
     
    //REG_ATD_CTL_TEST, all clk power down; Please don't power down CTL_TEST due to TVD module still refer to demod54m_ck/demod60m_ck
    RegWrite8(REG_ATD_CTL_TEST, 0xFF); //REG_ATD_CTL_TEST, all clk power down; 
    mcSHOW_DBG_MSG(("[ATD_DisConnect] Disable ATD_CTL_TEST CLK\n"));
    
    //Power down ADC/PAG clock
//    IO_WRITE32MSK(0xF002B000, 0x100, 0x00000003, 0x00000003); 
//    IO_WRITE32MSK(0xF002B000, 0x104, 0x00000600, 0x00000600); 
    // IFADC power down 0x100H[b1_b0]: RG_HPF_PWD_DEMOD=1, RG_ADC_PWD_DEMOD=1
	#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
	    IO_WRITE32MSK(0xF0061000, 0x500, 0x00000006,0x00000006);//d110804_Haibo:Power off PGA (0xf0061500[2:1]=11)
	    IO_WRITE32MSK(0xF0061000, 0x400, 0x00000001,0x00000001);//d110804_Haibo:Power off ADC(0xF0061400[0]=1) 
	#else
    ATD_GetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);
    ucData |= 0x03;
    ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);
    // 2b105[2:1]=3. PGA power down
    ATD_GetReg(psDemodCtx, REG_ATD_AREG05, &ucData, 1);
    ucData |= 0x06;
    ATD_SetReg(psDemodCtx, REG_ATD_AREG05, &ucData, 1);
	#endif
    mcSHOW_DBG_MSG(("[ATD_DisConnect] Power down IFADC CLK and PGA\n"));

}


void ATD_SetSnowMode(ATD_CTX_T *psDemodCtx, UINT8 fgSnowOn)
{
#if TVD_SUPPORT_SNOW_MODE
    mcSHOW_DBG_MSG(("Set Snow mode %s\n",fgSnowOn?"On":"Off"));
    _svSnowModeSet(fgSnowOn,cTVD_SNOW_INTENSITY);
#else
    UINT8 ucData;

    ATD_GetReg(psDemodCtx, REG_ATD_TVDIF_CFG1, &ucData, 1);

    if (fgSnowOn) {
        ucData |= 0x11;
    }
    else {
        ucData &= 0xFE;
    }
    ATD_SetReg(psDemodCtx, REG_ATD_TVDIF_CFG1, &ucData, 1);
#endif
}
void ATD_BypassSIF2Aud(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx)
{
   UINT8 ucData;
    
   if ((u1SubSysIdx < MOD_ANA_TYPE_BEGIN) || (u1SubSysIdx > MOD_TYPE_UNKNOWN))
   	{
        mcSHOW_USER_MSG(("Wrong TvSpec = %d !!!!\n", u1SubSysIdx));
        return;
    }
	mcSHOW_DBG_MSG(("ATD_BypassSIF2Aud %d\n",u1SubSysIdx));
	//DC bypass
	  RegWrite8(REG_ATD_SLD_ADCIF_DSM4A,0xA0);//0x282
	  RegWrite8(REG_ATD_ATD_DC_CFG2,0x80);
	  RegWrite8(REG_ATD_ATD_DC_CFG3,0x00);
	  //Shift -3.375MHz via CPR
	 RegWrite8(REG_ATD_CPR_EXT_FREQ1,0x03);//0x1D0
     RegWrite8(REG_ATD_CPR_EXT_FREQ2,0x80);//0x1D1
	 RegWrite8(REG_ATD_CPR_EXT_FREQ3,0x00);//0x1D2
	 RegWrite8(REG_ATD_CPR_EXT_FREQ4,0x00);//0x1D3
	 RegWrite8(REG_ATD_CPR_CFO_CFG,0x02);//0x1D6 Disable CR CFO compensation,Enable Externel Frequency setting
  
     RegWrite8(REG_ATD_PJ_CFG,0x6D);//0x1B7 Disable FPC
     RegWrite8(REG_ATD_PFLL_CFG,0xC9);//0x199 Disable FIFO
     if(u1SubSysIdx==MOD_TYPE_UNKNOWN)//scan mode
     {
	    mcSHOW_DBG_MSG(("Enable AVS bypass for scan mode\n"));
       	ATD_GetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
        ucData &= 0xF7;
        ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1); 
     }
  
   //Reset ATD data path
   RegWrite8(REG_ATD_CTL_RST1,0x00);
   RegWrite8(REG_ATD_CTL_RST1,0xFF);
   RegWrite8(REG_ATD_CTL_RST2,0x00);	
   RegWrite8(REG_ATD_CTL_RST2,0xFF);	
   RegWrite8(REG_ATD_SLD_PRA_30,0x00);//Fix PGA as minimum  value

}
STATIC UINT8 ATD_ShowRegVal(UINT16 u2RegIdx,UINT8 u1StartFindIdx)
{
   UINT8 u1Len=sizeof(u2HiddenRegIdx)/sizeof(u2HiddenRegIdx[0]);
   
   if(u1StartFindIdx>=u1Len/2)
   {
    return 0;
   }
   else
  {
   if(u2RegIdx==u2HiddenRegIdx[u1StartFindIdx*2])
   {
     return u2HiddenRegIdx[u1StartFindIdx*2+1];//d110629_Haibo:return next jump index
   }
   else
   {
     return 0;
   }
  }
}
void ATD_DumpRegister(ATD_CTX_T *psDemodCtx, UINT16 u2ModuleMask)
{
    UINT8 u1Len=sizeof(u1ArrRegisterMap)/sizeof(u1ArrRegisterMap[0]);
    UINT8  ii,jj,kk,ucData[80] = {0}; 
    UINT16 u2RegIdx=0;	
	UINT8 u1StartFindIdx=0;
	UINT8 u1RegOft;
	//mcSHOW_USER_MSG(("u1Len=%d modulemsk=0x%04X\n",u1Len,u2ModuleMask));
	if((u2ModuleMask&0x7FFF)==0)//info
	{
	        for(ii=0;ii<(sizeof(sModule)/sizeof(sModule[0]));ii++)
             {
                mcSHOW_USER_MSG(("Module %s:0x%04X\n",sModule[ii],(UINT16)(1L<<ii))); 			 
             }
			 mcSHOW_USER_MSG(("Module All:0x3FFF\n")); 	
            return;			 
	}
	for(kk=0;kk<14;kk++)
	{
	//mcSHOW_USER_MSG(("0x%04X\n",(1L<<kk)));
	
	if(u2ModuleMask & (1L<<kk))
	{
	   for(ii=0;ii<u1Len;ii+=3)
	   {
	     // mcSHOW_USER_MSG(("Register map  %d 0x%04X\n",ii,u1ArrRegisterMap[ii]));
		  
	       if(u1ArrRegisterMap[ii] & (1L<<kk))
		   {
		   // mcPRINTF("\n%s:\n",sModule[kk]);
		    ATD_GetReg(psDemodCtx,u1ArrRegisterMap[ii+1],ucData,u1ArrRegisterMap[ii+2]);
	      //   mcPRINTF("0x%03X:\n ",u1ArrRegisterMap[ii+1]+ATD_REG_OFFSET);
			for(jj=1;(jj<=u1ArrRegisterMap[ii+2]) && jj<(sizeof(ucData)/sizeof(ucData[0]));)
			{
			if(u2ModuleMask&0x8000)//d110629_Haibo:Show Partial registers
			{
			u1RegOft=ATD_ShowRegVal(u2RegIdx,u1StartFindIdx);
             
			 mcSHOW_DBG_MSG(("u2RegIdx=%d,u1StartFindIdx=%d,u1RegOft=%d,jj=%d\n",u2RegIdx,u1StartFindIdx,u1RegOft,jj));
			 
			 if(u1RegOft)
			 {
			   jj+=u1RegOft;
			   u2RegIdx+=u1RegOft;
			   u1StartFindIdx++;
			   continue;
			 }
			 }
			  mcPRINTF("0x%02X 0x%03X %s \n ",ucData[jj-1],u1ArrRegisterMap[ii+1]+ATD_REG_OFFSET+jj-1,sModule[kk]);
			  u2RegIdx++;
			  jj++;
			}
		   }
	   }
	}	    
	}
}

