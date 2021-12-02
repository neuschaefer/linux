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
 * $RCSfile: drv_vdoclk.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#define _VDO_CLK_C_

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_vdoin.h"
#include "drv_vdoclk.h"
#include "drv_adcset.h"
#include "vga_auto.h"
#include "vga_table.h"
#include "drv_vga.h"
#include "drv_lvds.h"
#include "drv_display.h"
#include "video_def.h"
#include "api_eep.h"
#include "eepdef.h"
#include "util.h"
#include "drvcust_if.h"
#include "x_ckgen.h"
#include "x_chip_id.h"
#include "hw_hdtv.h"
//#include "x_pdwnc.h"
#include "x_typedef.h"
#include "x_util.h"
#include "nptv_debug.h"
#include"x_assert.h"

//#include "hw_ckgen.h"

#include"x_hal_5381.h"
#include "hw_pdwnc.h"
#include "drv_async.h"
#include "drv_auto.h"
#include "nptv_drvif.h"
#include "hw_scslicer.h"

LINT_EXT_HEADER_END
#include "vdo_misc.h"//for dynamic MDR
#include "drv_hdtv.h"
#include "drv_tvd.h"
#include "hw_scpos.h"
#include "hw_lvds.h"
#define SS_FREQ 30      //kHz

// ###8202### - The followings are left to be redefined.
// REG_CAL_SEL 0x03

#define ADCPLL_DEBUG 0
#define DVIPLL_DEBUG 0
#define SYSPLL_DEBUG 0
#define CLK_5371 1

#define SYS_CLK_OLD		1
#if SYS_CLK_OLD 
#define SYSPLL 3240
#else
#define SYSPLL 4320
#endif

#define SYSPLL432 4320
#define SYSPLL324 3240
#define SYSPLL459 4590

enum {
    VO_PLL_CLKOUT,
    VO_PLL_XTAL
};
#if defined(CC_MT5396) || defined(CC_MT5389)
#define PIX_CLK_INV_Criterion 1030
#else
#define PIX_CLK_INV_Criterion 1390
#endif

#define CLKIN_LOCK_LINE_CNT_VAL 0x02
#define CLKIN_LOCK_THR_VAL 0x02
#define CLKIN_HSYNC_WIDTH 32
UINT32 _u4VPllFrequency = 0;
#ifndef CC_MTK_LOADER
static UINT32 _u4Permillage = 0;
static UINT32 _u4Freq = 0;//for modulation rate
#endif

UINT8   _bCLKByte3;
UINT8   _bCLKByte2;
UINT8   _bCLKByte1;
UINT8   _bCLKSetFlag;
UINT8   VOPLL_PARAMETER[6];

UINT8   _bGetCLKByte3;
UINT8   _bGetCLKByte2;
UINT8   _bGetCLKByte1;
UINT8   _bGetCLKByte0;

//UINT8   _bDVIPLL_FREQ=111;

//extern UINT16 _wVgaClockOffset;

UINT32	_dSumTmp[3];
#if NEW_PHASE_ALGORITHM      
   UINT32  _dBestSumALL[CHANNEL_NUM];
   UINT8   _bMaxSlope[CHANNEL_NUM];
   UINT8   _bChBestPhase[CHANNEL_NUM];
   UINT32  _dSumTmpPhase[CHANNEL_NUM][MAX_PHASE_ARRAY];
   UINT32  _dwPhase3CH[CHANNEL_NUM];
#endif
UINT8 code LPF_Table_1[10][4] =
	{//ADCREV_1_LPF[21:17]  ADCREV_2_LPF[21:17]   ADCREV_3_LPF[21:17]   REG_VGA_Normal_CFG8[27:24]
    {0x00,                        0x00,                        0x00,                        0x00},	  // 0. Bypass_LPF                     (0, 0)
    {0x0F,                        0x0F,                        0x0F,                        0x0F},       // 1. YPbPr_480i_LPF               (F, F)
    {0x0D,                        0x0D,                        0x0D,                        0x0A},       // 2. YPbPr_480p_LPF              (A, A)
    {0x08,                        0x08,                        0x08,                        0x05},	  // 3. YPbPr_720p_LPF              (3, 5)
    {0x02,                        0x02,                        0x02,                        0x01},	  // 4. YPbPr_1080p_LPF            (2, 1)
    {0x0C,                        0x0A,                        0x08,                        0x0F},	  // 5. VGA_40MHZ_Less_LPF    (7, F)
    {0x09,                        0x08,                        0x08,                        0x08},	  // 6. VGA_40_60MHZ_LPF       (6, A)
    {0x03,                        0x02,                        0x01,                        0x04},	  // 7. VGA_60_90MHZ_LPF       (3, 5)
    {0x03,                        0x02,                        0x01,                        0x01},	  // 8. VGA_90_130MHZ_LPF     (2, 2)
    {0x03,                        0x02,                        0x01,                        0x00}	  // 9. VGA_120MHZ_More_LPF  (1, 1)  
	};
UINT8 code LPF_Table_2[10][3] =
     {//PDWNC_VGACFG3[7:6]      PDWNC_VGACFG6[7:6]    REG_VGA_Normal_CFG8[27:24]     no use
    {0x00,                         0x00,                        0x00},	  // 0. Bypass_LPF                     (0, 0)
    {0x01,                         0x03,                        0x0F},       // 1. YPbPr_480i_LPF               (F, F)
    {0x01,                         0x02,                        0x0A},       // 2. YPbPr_480p_LPF              (A, A)
    {0x00,                         0x02,                        0x05},	  // 3. YPbPr_720p_LPF              (3, 5)
    {0x00,                         0x01,                        0x01},	  // 4. YPbPr_1080p_LPF            (2, 1)
    {0x00,                         0x03,                        0x0F},	  // 5. VGA_40MHZ_Less_LPF    (7, F)
    {0x00,                         0x02,                        0x08},	  // 6. VGA_40_60MHZ_LPF       (6, A)
    {0x00,                         0x01,                        0x04},	  // 7. VGA_60_90MHZ_LPF       (3, 5)
    {0x00,                         0x01,                        0x01},	  // 8. VGA_90_130MHZ_LPF     (2, 2)
    {0x00,                         0x01,                        0x00}	  // 9. VGA_120MHZ_More_LPF  (1, 1)  
};

extern UINT8 bSI_DISPLAY_DCLK_TYPE;


#ifdef CC_UP8032_ATV
extern UINT64 u8Div6432(UINT64 u8Dividend, UINT64 u8Divider, UINT64 *pu8Remainder);
#endif

#ifndef CC_MTK_LOADER
void vDrvClkInit(void)  // ToDo - vDrvVideoInit
{
//CC_MT5365 according to Walter's suggestion
 vRegWriteFldAlign(VFE_18, 1, RG_SDDS_DATA_SYNC);  
 vUtDelay2us(1);
 vRegWriteFldAlign(VFE_18, 0, RG_SDDS_DATA_SYNC);  
 vRegWriteFldAlign(VFE_18, 1, RG_SDDS_FIFO_START);  
 vRegWriteFldAlign(VFE_18, 0, SDDS_CLK3_INV);  
 vRegWriteFldAlign(VFE_18, 1, SDDS_CLK1_INV);  

    //vDrvTVADCPLLIni();
 //vDrvCLKINPsncoRST(1);      //MT5387 don't have this bit  //bit29          : C_PSNCO_RST = 1 
 #if  ADAPTIVE_SLICER_PLLERR_CHK  
 vDrvCLKINMaxPeriod(3);        //bit 28~27    : C_MAX_PERIOD =maximum error detection period
 #else
 vDrvCLKINMaxPeriod(0);        //bit 28~27    : C_MAX_PERIOD =maximum error detection period
 #endif
 vDrvCLKINFMPeriod(0);         //bit 26~25    : C_FM_PERIOD = Frequency modulation detection period
 //vDrvCLKINFreeHsyncPol(0);  //bit 24         : C_HSYNC_POL = polarity of external sync
 vDrvCLKINSetLockThr(2);     //bit 23~22   : C_LOCK_THR
 vDrvCLKINSetKI0(8);              //bit 21~18  : C_DCLK1_KI0
 vDrvCLKINSetKI1(1);              //bit 17~16  : C_DCLK1_KI1
 vDrvCLKINSetLockCnt(2);       //bit 15~14  : C_LOCK_LINE_CNT 
 vDrvCLKINSetKP0(8);              //bit 13~10  : C_DCLK1_KP0 //DTV00130828(7->8)
 vDrvCLKINSetKP1(1);              //bit 9~8     : C_DCLK1_KP1
 vDrvCLKINSetErrLim(0xff);     //bit 7~0     : C_ERR_LIM
 vRegWriteFldAlign(VFE_17, 0x9, SDDS_CKSEL);//for SDDS 324MHz clock phase selection

 //fix ADCPLL o/p after initialized
 vDrvCLKINFreeRun(ENABLE);

    #ifdef CC_MT5363
    vIO32WriteFldAlign(CKGEN_DISP_CKCFG1, 1, FLD_RESYNC_PCLK_SEL);
    #endif

  
    #if Phase3Channel// 3 phase
        //vIO32WriteFldMulti(CKGEN_AFEPLLCFG4, P_Fld(0x1, FLD_RG_VGAPLL_CKO_SEL)|P_Fld(0x1, FLD_RG_VGAPLL_B_EN)|P_Fld(0x1,FLD_RG_VGAPLL_G_EN)|P_Fld(0x1,FLD_RG_VGAPLL_R_EN));
        vIO32WriteFldMulti(REG_PLL_GROUP_CFG6, P_Fld(0x1, FLD_RG_VGAPLL_CKO_SEL)|P_Fld(0x1, FLD_RG_VGAPLL_B_EN)|P_Fld(0x1,FLD_RG_VGAPLL_G_EN)|P_Fld(0x1,FLD_RG_VGAPLL_R_EN));
        vIO32WriteFldMulti(REG_VGA_Normal_CFG12, P_Fld(1,CLKINV_EN)|P_Fld(1,RELATCH_EN)); //be careful the define for this bit is different between MT5365 AND MT8223		
    #else//single phase
        //vIO32WriteFldMulti(CKGEN_AFEPLLCFG4, P_Fld(0x2, FLD_RG_VGAPLL_CKO_SEL)|P_Fld(0x0, FLD_RG_VGAPLL_B_EN)|P_Fld(0x0,FLD_RG_VGAPLL_G_EN)|P_Fld(1,FLD_RG_VGAPLL_R_EN));
        vIO32WriteFldMulti(REG_PLL_GROUP_CFG6, P_Fld(0x2, FLD_RG_VGAPLL_CKO_SEL)|P_Fld(0x0, FLD_RG_VGAPLL_B_EN)|P_Fld(0x0,FLD_RG_VGAPLL_G_EN)|P_Fld(0x1,FLD_RG_VGAPLL_R_EN));
        vIO32WriteFldMulti(REG_VGA_Normal_CFG12, P_Fld(0,CLKINV_EN)|P_Fld(0,RELATCH_EN)); //be careful the define for this bit is different between MT5365 AND MT8223			
    #endif
    //vIO32WriteFldMulti(CKGEN_AFEPLLCFG1,P_Fld(1,FLD_RG_VGAPLL_PD)|P_Fld(0,FLD_RG_VGAPLL_MONAIOEN)|P_Fld(0,FLD_RG_VGAPLL_MONCLKEN));
      vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 1, FLD_RG_VGAPLL_PWD);
      vIO32WriteFldMulti(REG_PLL_GROUP_CFG6, P_Fld(1, FLD_RG_VGAPLL_SDDS_PD_PDB)|P_Fld(1,FLD_RG_VGAPLL_SDDS_PD_EN)|P_Fld(0,FLD_RG_VGATL_BIAS_PWD)|P_Fld(1,FLD_RG_XDDS_PWDB)|P_Fld(7,FLD_RG_XDDS_PI_C)|P_Fld(1,FLD_RG_XDDS_HF)|P_Fld(1,FLD_RG_VGAPLL_RTB_EN));
      vIO32WriteFldMulti(REG_PLL_GROUP_CFG1, P_Fld(0, FLD_RG_VGAPLL_MONCKEN)|P_Fld(0,FLD_RG_VGAPLL_MONEN)|P_Fld(1,FLD_RG_VGAPLL_RESERVE));

   //vIO32WriteFldMulti(CKGEN_AFEPLLCFG4, P_Fld(0x0, FLD_RG_VGAPLL_B_EN)|P_Fld(0x0, FLD_RG_VGAPLL_G_EN)|P_Fld(0x0, FLD_RG_VGAPLL_R_EN)|P_Fld(0,FLD_RG_VGAPLL_ENTM)|P_Fld(0x29,FLD_RG_VGAPLL_ENTL)|P_Fld(0,FLD_RG_VGAPLL_INTH_EN));
   //vIO32WriteFldMulti(REG_PLL_GROUP_CFG6, P_Fld(0x0, FLD_RG_VGAPLL_B_EN)|P_Fld(0x0,FLD_RG_VGAPLL_G_EN)|P_Fld(0x0,FLD_RG_VGAPLL_R_EN));
   vIO32WriteFldMulti(REG_PLL_GROUP_CFG7, P_Fld(0x0, FLD_RG_VGAPLL_ENTL)|P_Fld(0x1,FLD_RG_VGAPLL_ENTL)|P_Fld(0x0,FLD_RG_VGAPLL_INTH_EN));
	
}

// => bApiVideoMainSubSrc
/**
 * @brief Set all the ADCPLL on / off
 *
 * Set all the ADCPLL on / off
 * @param bPow bPow is the on / off selection (SV_TRUE, SV_FALSE)
 */
void vDrvAllADCPLLPow(UINT8 bPow)
{

	if (bPow == SV_FALSE)//Power off
	{    
        //vIO32WriteFldAlign(CKGEN_AFEPLLCFG1, 1, FLD_RG_VGAPLL_PD);
       vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 1, FLD_RG_VGAPLL_PWD);
       // vIO32WriteFldMulti(CKGEN_AFEPLLCFG4, P_Fld(0x0, FLD_RG_VGAPLL_B_EN)|P_Fld(0x0, FLD_RG_VGAPLL_G_EN)|P_Fld(0x0, FLD_RG_VGAPLL_R_EN));
       vIO32WriteFldMulti(REG_PLL_GROUP_CFG6, P_Fld(0x0, FLD_RG_VGAPLL_B_EN)|P_Fld(0x0,FLD_RG_VGAPLL_G_EN)|P_Fld(0x0,FLD_RG_VGAPLL_R_EN));
       }
}


#if CHANGE_SDDS_KPI  //michael 20090123 ***
BOOL _KPIReset; 
UINT8 _sdds_count;

void vDrvEnableChang_SDDS_BW(void)
{
    _KPIReset = TRUE; 
    _sdds_count = 0;    
}

/**
 * @brief CLKIN loop filter BW 
 *
 * Set CLKIN loop filter 
 * @retval void
 */
void Set_SDDS_KPI(UINT8 bValue)
{
    if (fgIsMainYPbPr() || fgIsPipYPbPr() || fgIsMainVga() || fgIsPipVga())
    {
	  if(bValue==0)
	  {  	
	    if(_KPIReset)
	    {
		  _sdds_count++;
		  if(_sdds_count>5)
		  {

            #if defined(CC_VFE_PLL_HIGH_GAIN) && defined(CC_VFE_PLL_HIGH_GAIN_YPBPR)
			 vDrvCLKINSetKI0(5);
			 vDrvCLKINSetKP0(7);
			 Printf("Set Kp0=5, Ki0=7\n");			 
		    #elif defined(CC_VFE_PLL_HIGH_GAIN) // for Chroma 2135 1024*768/60
		      if (fgIsMainVga() || fgIsPipVga())
		      {
                vDrvCLKINSetKI0(5);
			    vDrvCLKINSetKP0(7);
			    Printf("Set Kp0=5, Ki0=7\n");	
		      }
		      else
		      {
                vDrvCLKINSetKI0(1);			  //bit 21~18  : C_DCLK1_KI0
			    vDrvCLKINSetKP0(3);			  //bit 13~10  : C_DCLK1_KP0
			    Printf("Set Kp0=3, Ki0=1\n");	
		      }
            #elif defined(CC_VFE_PLL_HIGH_GAIN_YPBPR) // for Chroma 2135 1024*768/60
		      if (fgIsMainYPbPr() || fgIsPipYPbPr())
		      {
                vDrvCLKINSetKI0(5);
			    vDrvCLKINSetKP0(7);
			    Printf("Set Kp0=5, Ki0=7\n");	
		      }
		      else
		      {
                vDrvCLKINSetKI0(1);			  //bit 21~18  : C_DCLK1_KI0
			    vDrvCLKINSetKP0(3);			  //bit 13~10  : C_DCLK1_KP0
			    Printf("Set Kp0=3, Ki0=1\n");	
		      }
			 #else
			 vDrvCLKINSetKI0(1);			  //bit 21~18  : C_DCLK1_KI0
			 vDrvCLKINSetKP0(3);			  //bit 13~10  : C_DCLK1_KP0
			 Printf("Set Kp0=3, Ki0=1\n");			 
			 #endif
			 
			 _KPIReset= 0;				  
			 _sdds_count = 0;	   
		  }
	    }
	  }
	  else
	  {
		  vDrvCLKINSetKI0(8);			   //bit 21~18	: C_DCLK1_KI0
		  vDrvCLKINSetKP0(8);			   //bit 13~10	: C_DCLK1_KP0
		  Printf("Set Kp0=8, Ki0=8\n");		  
		  _KPIReset= 0; 			   
		  _sdds_count = 0;		
	  }
    }
}

//For SCART chang SDDS PI gain
void vDrvClkSetLockBandwidth(UINT8 bIsLock)
{	
     if(bIsLock) // big	
     {		
          vDrvCLKINSetKI0(1);              //bit 21~18  : C_DCLK1_KI0		
          //vDrvCLKINSetKI1(1);              //bit 17~16  : C_DCLK1_KI1
          vDrvCLKINSetKP0(2);              //bit 13~10  : C_DCLK1_KP0
          //vDrvCLKINSetKP1(1);              //bit 9~8     : C_DCLK1_KP1
      }
     else
     {		
          vDrvCLKINSetKI0(8);              //bit 21~18  : C_DCLK1_KI0
          //vDrvCLKINSetKI1(1);              //bit 17~16  : C_DCLK1_KI1
          vDrvCLKINSetKP0(8);              //bit 13~10  : C_DCLK1_KP0
          //vDrvCLKINSetKP1(1);              //bit 9~8     : C_DCLK1_KP1
      }
}
#endif

void vPGA_LPF_BW(UINT8 bLPF)
{
    if(bLPF < Max_Input_timing)
    {
          if((BSP_GetIcVersion() == IC_VER_5396_AA)||(BSP_GetIcVersion() == IC_VER_5368_AA))
          {
            vIO32WriteFldAlign(PDWNC_VGACFG3,        LPF_Table_2[bLPF][0], FLD_RG_MUXCAP_EN2);
            vIO32WriteFldAlign(PDWNC_VGACFG6,        LPF_Table_2[bLPF][1], FLD_RG_MUXCAP_EN1);
            vIO32WriteFldAlign(REG_VGA_Normal_CFG8, LPF_Table_2[bLPF][2], CAP_EN);
          }
          else
          {
            vIO32WriteFldAlign(REG_VGA_Normal_CFG1,        LPF_Table_1[bLPF][0], ADCREV_1_LPF);
            vIO32WriteFldAlign(REG_VGA_Normal_CFG5,        LPF_Table_1[bLPF][1], ADCREV_2_LPF);
             vIO32WriteFldAlign(REG_VGA_Normal_CFG9,       LPF_Table_1[bLPF][2], ADCREV_3_LPF);
             vIO32WriteFldAlign(REG_VGA_Normal_CFG8, LPF_Table_1[bLPF][3], CAP_EN);
             }
    }
}

void vSet_VFE_LPF(UINT16 wInputPixClk)
{
    if(fgIsMainVga()||fgIsPipVga()) 
    {
        if (wInputPixClk < 410)
        {
            vPGA_LPF_BW(VGA_40MHZ_Less_LPF); //BW is approximate 33MHz
        }
        else if (wInputPixClk<600)
        {
            vPGA_LPF_BW(VGA_40_60MHZ_LPF); //BW is approximate 43MHz
        }
        else if (wInputPixClk< 900)
        {
            vPGA_LPF_BW(VGA_60_90MHZ_LPF); //BW is approximate 62MHz
        }    
        else if (wInputPixClk< 1200)
        {
            vPGA_LPF_BW(VGA_90_130MHZ_LPF);  //BW is approximate 89MHz
        }    
        else
        {
            vPGA_LPF_BW(VGA_120MHZ_More_LPF); //BW is approximate 119MHz
        }
    }
    else if (fgIsMainYPbPr()||fgIsPipYPbPr())
    {
        if (wInputPixClk <300)
        {
            vPGA_LPF_BW(YPbPr_480i_LPF); //13.5MHz oversample to 27.0 MHz
        }
        else if (wInputPixClk<600)
        {
            vPGA_LPF_BW(YPbPr_480p_LPF); // 27MHz oversample to 54.0 MHz
        }    
        else if (wInputPixClk < 900)
        {
            vPGA_LPF_BW(YPbPr_720p_LPF); // 720p is 74.25MHz
        }    
        else
        {
            vPGA_LPF_BW(YPbPr_1080p_LPF);
        }
    }
    else if (fgIsMainScart()||fgIsPipScart())
    {
        vPGA_LPF_BW(YPbPr_480i_LPF);
    }
}

/**
 * @brief set ADCPLL/CLKIN and Htotal precise frequency
 *
 * Set ADCPLL and CLKIN correct frequency
 * @param wPixClk wPixClk is the target clock rate (MHz)
 * @param wHtotal wHtotal is the quantity of the pixel clock per hsync
 * @retval void
 */
void vDrvADCPLLSet(UINT16 wPixClk, UINT16 wHtotal)
{
    UINT32 dTmp;
    UINT8 bMcode;
    UINT8 bADCPLL_NS;
   // UINT32 delay_count;
   // UINT8 bmode;

    vDrvCLKINDCLKFast(ENABLE); // Enable for avoid suddenly large errror // Walter suggest disable, Tomson Ena /Dis DCLK Fast lock Loop
    vDrvCLKINDCLKPFDSEL(ENABLE); // Tomson Ena /Dis DCLK PFD High Resoultion detection
#if CHANGE_SDDS_KPI
    Set_SDDS_KPI(1);
#endif

   //vIO32WriteFldAlign(CKGEN_TVDPLLCFG0, 0, FLD_RG_ADCPLL_BYP_SEL);
    //TVD_REV7 ADCPLL input reference clock selection 0: from phase sel , 1: from sdd sout 

    // set Delay cell for term variation change Tomson
   //vIO32WriteFldAlign(CKGEN_TVDPLLCFG0, 0, FLD_RG_ADCPLL_DE_FB_CN);
   //vIO32WriteFldAlign(CKGEN_DTDPLLCFG0, 0, FLD_RG_ADCPLL2_DE_FB_CN);
       
   
    vDrvCLKINSetHtotal(wHtotal);
    vRegWriteFldAlign(VFE_16, 10, DCLK_HSYNC_WIDTH);  // vWriteVDOINMsk(VFE_16, 0x80, 0xC0, 1);
    // Avoid the the Flash noise by Data and CLK Delay mismatch
#if defined(CC_MT5396) || defined(CC_MT5389)
    if(wPixClk>=PIX_CLK_INV_Criterion)
    {
     vIO32WriteFldAlign(CKGEN_RGBCLK_CFG, 0, FLD_C_PIX_CLK_INV);
    }
    else
    {
     vIO32WriteFldAlign(CKGEN_RGBCLK_CFG, 1, FLD_C_PIX_CLK_INV);     
    }    
#else
//    if(wPixClk>=PIX_CLK_INV_Criterion)
//    {
//     vIO32WriteFldAlign(CKGEN_RGBCLK_CFG, 0, FLD_VGA_CKO_PIX_TOG);
//    }
//    else
//    {
//     vIO32WriteFldAlign(CKGEN_RGBCLK_CFG, 1, FLD_VGA_CKO_PIX_TOG);     
//    }    

#endif
    vSet_VFE_LPF(wPixClk);

//new band selection
    //SDDS Setting			//Let SDclk CW range:    324/5 ~ 324/14

      vIO32WriteFldMulti(REG_PLL_GROUP_CFG0, P_Fld(1, FLD_RG_VGAPLL_DIVEN) |P_Fld(1, FLD_RG_VGAPLL_V11EN)|P_Fld(1,FLD_RG_VGAPLL_VODEN)|P_Fld(0,FLD_RG_VGAPLL_BP)|P_Fld(0,FLD_RG_VGAPLL_BR)|P_Fld(0,FLD_RG_VGAPLL_HF)|P_Fld(1,FLD_RG_VGAPLL_LF)|P_Fld(0, FLD_RG_VGAPLL_FBSEL));

   //Setting ADCPLL Band
    if(wPixClk<450)
    {
     vIO32WriteFldAlign(REG_PLL_GROUP_CFG1, 0x3, FLD_RG_VGAPLL_BS);       
    }
    else if((wPixClk<800)&&(wPixClk>=450))
    {
     vIO32WriteFldAlign(REG_PLL_GROUP_CFG1, 0x2, FLD_RG_VGAPLL_BS);   
    }
    else
    {
     vIO32WriteFldAlign(REG_PLL_GROUP_CFG1, 0x0, FLD_RG_VGAPLL_BS);   
    }


   //Setting before ADCPLL    
    if(wPixClk<300)
    {
      bADCPLL_NS = 1;
      wPixClk = wPixClk << 1; // Pixel clock 
      //vIO32WriteFldAlign(CKGEN_AFEPLLCFG5, 1, FLD_RG_VGAPLL_SDDSO_DIV12_SEL);
     vIO32WriteFldAlign(REG_PLL_GROUP_CFG6, 0x0, FLD_RG_XDDS_CKSEL);   
   }
    else if((wPixClk<650)&&(wPixClk>=300))			//(SDclk CW)/1, ADCPLLx1 
    {
      bADCPLL_NS = 1;
      //vIO32WriteFldAlign(CKGEN_AFEPLLCFG5,0, FLD_RG_VGAPLL_SDDSO_DIV12_SEL);
     vIO32WriteFldAlign(REG_PLL_GROUP_CFG6, 0x1, FLD_RG_XDDS_CKSEL);   
    }
    else if((wPixClk<1200) && (wPixClk>=650))	//(SDclk CW)/2, ADCPLLx2     //wu
    {      
      bADCPLL_NS = 2;
      wPixClk = wPixClk >> 1; // Pixel clock divider by 2
      //vIO32WriteFldAlign(CKGEN_AFEPLLCFG5,0, FLD_RG_VGAPLL_SDDSO_DIV12_SEL);
     vIO32WriteFldAlign(REG_PLL_GROUP_CFG6, 0x1, FLD_RG_XDDS_CKSEL);   
   }
    else	// pixel >1000 (SDclk CW)/3, ADCPLLx3
    {
       bADCPLL_NS = 3;
       wPixClk = (UINT16)(wPixClk / 3);// Pixel clock divider by 3
       //vIO32WriteFldAlign(CKGEN_AFEPLLCFG5,0, FLD_RG_VGAPLL_SDDSO_DIV12_SEL);
     vIO32WriteFldAlign(REG_PLL_GROUP_CFG6, 0x1, FLD_RG_XDDS_CKSEL);   
    }	

   //Setting before PFD    
    if(wPixClk<300)
    {
       bADCPLL_NS = bADCPLL_NS *1-1;
     vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 0, FLD_RG_VGAPLL_PREDIV);   
    }
    else if((wPixClk<650)&&(wPixClk>=300))
    {
       bADCPLL_NS = bADCPLL_NS *2-1;
     vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 1, FLD_RG_VGAPLL_PREDIV);   
    }
    else
    {
       bADCPLL_NS = bADCPLL_NS *4-1;
     vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 3, FLD_RG_VGAPLL_PREDIV);   
    }		
       
      // vIO32WriteFldAlign(CKGEN_AFEPLLCFG5, 2, FLD_RG_VGAPLL_SDDS_FBKSEL);
     vIO32WriteFldAlign(REG_PLL_GROUP_CFG7, 0x2, FLD_RG_VGAPLL_SDDS_FBKSEL);   

      //vIO32WriteFldAlign(CKGEN_AFEPLLCFG0, bADCPLL_NS, FLD_RG_VGAPLL_DIV);
     vIO32WriteFldAlign(REG_PLL_GROUP_CFG1, bADCPLL_NS, FLD_RG_VGAPLL_FBDIV);   
    bMcode = (UINT8) (SYSPLL324 / wPixClk);  //cal the M code
    dTmp = SYSPLL324 - (UINT32) wPixClk *bMcode; //calculate residue        
		
    dTmp = (dTmp << 16) / wPixClk;  //calculate 24 bit residue, only calculate CW2,1 , CW0 don't care
                                                        //if <<24 will cause (residue ^24 overflow) , so <<16 is enough
    dTmp = (bMcode << 24) | (dTmp << 8) ;
    
    //LOG(3,"sDDS CW is  0x%x\n", dTmp);

    vDrvCLKINFreeRun(ENABLE); //SDDS freerun bit[31] set "1" 
    
    /////// move to vDrvClkInit
    
    vRegWrite4B(VFE_15,dTmp);             //SDDS CW set
    
    //vIO32WriteFldMulti(CKGEN_AFEPLLCFG1,P_Fld(1,FLD_RG_VGAPLL_VCOVTSEL) |P_Fld(0,FLD_RG_VGAPLL_VCOCAL_EN)|P_Fld(1,FLD_RG_VGAPLL_VCOCALSEL)|P_Fld(1,FLD_RG_VGAPLL_PD));    
//    vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 1, FLD_RG_VGAPLL_PWD);
   //vIO32WriteFldMulti(CKGEN_AFEPLLCFG4,P_Fld(0,FLD_RG_VGAPLL_G_EN)|P_Fld(0,FLD_RG_VGAPLL_B_EN)|P_Fld(1,FLD_RG_VGAPLL_R_EN));    
//   vIO32WriteFldMulti(REG_PLL_GROUP_CFG6, P_Fld(0x0, FLD_RG_VGAPLL_G_EN)|P_Fld(0x0,FLD_RG_VGAPLL_B_EN)|P_Fld(0x1,FLD_RG_VGAPLL_R_EN));
   // vIO32WriteFldMulti(CKGEN_AFEPLLCFG4,P_Fld(1,FLD_RG_VGAPLL_G_EN)|P_Fld(1,FLD_RG_VGAPLL_B_EN)|P_Fld(1,FLD_RG_VGAPLL_R_EN));    
   vIO32WriteFldMulti(REG_PLL_GROUP_CFG6, P_Fld(0x1, FLD_RG_VGAPLL_G_EN)|P_Fld(0x1,FLD_RG_VGAPLL_B_EN)|P_Fld(0x1,FLD_RG_VGAPLL_R_EN));
    //vIO32WriteFldMulti(CKGEN_AFEPLLCFG1,P_Fld(0,FLD_RG_VGAPLL_PD)|P_Fld(1,FLD_RG_VGAPLL_VCOCAL_EN));
      //  vIO32WriteFldAlign(REG_PLL_GROUP_CFG6, 0x1,FLD_RG_VGAPLL_R_EN);
       vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 0, FLD_RG_VGAPLL_PWD);
    //HAL_TIME_T rHDTVADCTime;
    //HAL_GetTime(&rHDTVADCTime);
    //Printf("VGAPLL cal start time=%d  %d\n",rHDTVADCTime.u4Seconds, rHDTVADCTime.u4Micros);   

 //   for(delay_count=0;delay_count<50;delay_count++)
 //   {
 //       vUtDelay1ms(1);
 //       if((IO32ReadFldAlign(CKGEN_ACD_DBG_REG0,ACD_DBG_REG0_RGS_VGA_VCOCALOK)))
 //       {
 //           delay_count=50;
 //       }
 //       else if(delay_count==49)
//        {
//            Printf("VGAPLL INIT FAIL");         
//        }
 //    }
     //HAL_GetTime(&rHDTVADCTime);
     //Printf("VGAPLL cal  end time=%d  %d\n",rHDTVADCTime.u4Seconds, rHDTVADCTime.u4Micros);   
    vDrvCLKINDCLKFast(ENABLE); // Enable for avoid suddenly large errror //Walter suggest disable, Tomson Ena /Dis DCLK Fast lock Loop
    
    vDrvCLKINFreeRun(DISABLE);// vWriteVDOINMsk(VFE_14,0x00,0x80,3); SDDS lock 20022490[31] set "0"
    vUtDelay2us(200);

    _bCLKSetFlag |= 0x04;
    
#if CHANGE_SDDS_KPI            //michael 20090123 ***
    vDrvEnableChang_SDDS_BW();
#endif
//    vIO32WriteFldAlign(PDWNC_SOGYCFG,0,FLD_SELECT);

	_RETIMENeedReset = TRUE;                                //Reset RETIME in VGA ISR
}


//for slicer CLI
void vDrvSlicerCheck(void)
{  
     UINT8 i;
     UINT8 Lock;
     UINT8 VHSEL;
     UINT8 VLSEL;

     for(VHSEL=0; VHSEL<=15;VHSEL++)
     {
         for(VLSEL=0; VLSEL<=15;VLSEL++)
         {   
             Lock=0;         
              vRegWriteFldMulti(SyncSlice_Control_01, P_Fld(VHSEL,VTH_SEL)|P_Fld(VLSEL,VTL_SEL))
	      vUtDelay1ms(1);	
             for(i=0; i<=127;i++)
             {
                Lock += fgIsCLKLock();
      	         vUtDelay1ms(1);		//1080p HSYNC 15uS fastest
             }
             if(Lock>=127)
                Printf("slicer test Lock=%2d, VHSEL=%2d, VLSEL=%2d\n", Lock, VHSEL, VLSEL);
         }
     }
              vRegWriteFldMulti(SyncSlice_Control_01, P_Fld(0x08,VTH_SEL)|P_Fld(0x08,VTL_SEL))
}

#if EnableSDDSCalibration
UINT16 _wSDDS_delay_counter;
UINT8 _bSDDSCaliState;
UINT8 _bKI0,_bKP0;
UINT32 _bVFE14_backup;
UINT8 _bKP0_MAX, _bKI0_MAX,_bKP0_init; //,_bKI0_init;
enum
{
   SDDS_CALI_NOTHING,
   SDDS_CALI_START,
   SDDS_CALI_WAIT,
   SDDS_CALI_END
};

#define KI0_MAXVALUE 0x0b
#define KP0_MAXVALUE 0x0b
#define MEM_SIZE 0x1000
#define REPEAT_NUM  8
#endif

void SDDS_wait_message(void)
{
   #if EnableSDDSCalibration
         if  (_bSDDSCaliState == SDDS_CALI_WAIT)
             {
                 _wSDDS_delay_counter= _wSDDS_delay_counter+1;
             }
   #endif
}


void vDrvEnable_SDDS_Calibration(UINT8 bKP0_START,UINT8 bKP0_END, UINT8 bKI0_START, UINT8 bKI0_END)
{
#if EnableSDDSCalibration
   if(_bSDDSCaliState == SDDS_CALI_NOTHING)
   {
       _bVFE14_backup = u4RegRead4B(VFE_14);  //backup VFE_14
       _bSDDSCaliState = SDDS_CALI_START;
       if(bKP0_END> KP0_MAXVALUE)
       {
           bKP0_END = KP0_MAXVALUE;
       }
       _bKP0_MAX = bKP0_END;
       
       if(bKI0_END> KI0_MAXVALUE)
       {
           bKI0_END = KI0_MAXVALUE;
       }
       _bKI0_MAX = bKI0_END;
       
       if(bKP0_START>bKP0_END)
       {
          bKP0_START = bKP0_END;
       }
       
       if(bKI0_START>bKI0_END)
       {
          bKI0_START = bKI0_END;
       }

       _bKP0 = bKP0_START;
       _bKP0_init = bKP0_START;
       _bKI0 = bKI0_START;
       //_bKI0_init = bKI0_START;
   }
#endif   
}

void vDrvSDDS_Response(void)
{
#if EnableSDDSCalibration
    UINT32 CW_INIT,CW_backup;
    UINT32 *CW;
    UINT16 SDDS_ERR; 
    //INT16 ERR;
    UINT16 i;
    UINT16 j;
    UINT32 dwTmp; //,dwTotalCount; 
    UINT32 CW_old=0;
    UINT16 repeat_counter,wRepeatimes;
    UINT8 k,bTmpCount;//repeat_counter[REPEAT_NUM];
    UINT8 mode;
  
    switch (_bSDDSCaliState)
    {
        case SDDS_CALI_NOTHING:
                 break;

        case SDDS_CALI_START:
             k=0;
             bTmpCount = 0;
             wRepeatimes = 0;
             repeat_counter=0;

             CW = (UINT32 *)x_mem_alloc(MEM_SIZE); 
             if(!CW)
             {
              ASSERT(CW);
              return;
             } 
             dwTmp= u4RegRead4B(0x240c);     
             vRegWrite4B(0x240c, 0xffffffff);   //disable all interrupt
             _wSDDS_delay_counter = 0;
             vRegWriteFldAlign(VFE_14, _bKP0, DCLK1_KP_0);
             vRegWriteFldAlign(VFE_14, _bKI0, DCLK1_KI_0); 
             vDrvCLKINFreeRun(ENABLE);
             
             //vRegWriteFldAlign(VFE_14, 1, C_DCLK_INIT);
             //for (j=0; j<7; j++)
             //{
             //   dwTmp2 = u4RegRead4B(VFE_14+(j<<2));
             //   Printf("a. Reg address=0x2002%x, data=0x%x\n",(VFE_14+(j<<2)),dwTmp2);
             //}
             
             CW_INIT = u4RegRead4B(VFE_15);
             CW_INIT = CW_INIT & 0x0fffffff;
             CW_backup = CW_INIT;
             Printf("SDDS target_CW=%7x, KP0=%u,  KI0=%u\n", CW_INIT,_bKP0, _bKI0);
             CW_INIT = CW_INIT - 0x01000000; //(CW_INIT >>2);
             vRegWrite4B(VFE_15, CW_INIT);
             Printf("SDDS initial_CW=%7x, KP0=%u,  KI0=%u\n", CW_INIT,_bKP0, _bKI0);
             vDrvCLKINFreeRun(DISABLE);
             
             //vUtDelay1ms(20);	
             //vRegWriteFldAlign(VFE_14, 0, C_DCLK_INIT);
             //vRegWriteFldAlign(VFE_14, 1, C_DCLK_RST);
             
             for(i=0; i<(MEM_SIZE/4);i++)
             {
                 for(j=0; j<20; j++)
                 {
                   SDDS_ERR = u4RegRead4B(VFE_STA_00);
                   SDDS_ERR = SDDS_ERR & 0x00000fff;
                 }
                 
                 if(repeat_counter>0)
                 {
                     bTmpCount=bTmpCount+1;
                     CW[i]  = (u4RegRead4B(VFE_STA_01) & 0x0fffffff);  //read the CW status
                     if(CW[i] !=CW_old)
                     {
                         bTmpCount = 0;
                         CW_old = CW[i];                                         // update CW_old by this new value
                     }
                     else
                     {
                         if(bTmpCount>=(repeat_counter-1))                   //repeat_counter[DREPEAT_NUM-1]) //store the CW every repeat_counter[DREPEAT_NUM-1] times
                         {
                           bTmpCount = 0;
                           CW_old = CW[i];                                         // update CW_old by this new value
                         }
                         else if(i>0)
                         {
                            i = i -1;
                         }
                     }
                 }
                 else
                 {              
                     CW[i]  = (u4RegRead4B(VFE_STA_01) & 0x0fffffff);
                     if(CW[i] ==CW_old)
                     {
                         if(i>0)
                         {
                             bTmpCount=bTmpCount+1;             // increase counter in case of the same value happened
                             i = i-1;
                         }
                     }
                     else                                                       //Current value != old value
                     {
                         if((k<REPEAT_NUM)&&(bTmpCount>0))
                         {  
                             if(k>0)                                          // skip the first time
                             {
                                 wRepeatimes+=bTmpCount;
                             }
                             bTmpCount = 0;                              //reset counter
                             if(k==(REPEAT_NUM-1))
                             {
                                 wRepeatimes = wRepeatimes/(REPEAT_NUM-1);
                                 repeat_counter = 2*wRepeatimes;
                             }
                             k=k+1;
                         }
                         CW_old = CW[i];                                 // update CW_old by this new value
                     }
                 }
             }

             for(i=0; i<(MEM_SIZE/4);i++)
             {
                 Printf("KP0=%x, KI0=%x, SD_CW=%7x\n", _bKP0,_bKI0, CW[i]);
                 //Printf("SDDSCW ERR=%6d,CW=%7x,CW_I=%7x\n", ERR[i], CW[i], CW_INIT);    
             }
             CW_old= RegReadFldAlign(VFE_16, DCLK_HLINE_LENGTH)+1;
             if (fgIsMainVga()||fgIsPipVga())
             {
	       	mode=_bVgaTiming;
	      }
	      else
             {
	        mode=_bHdtvTiming;
             }
             CW_old = CW_old*(100*VGATIMING_TABLE[mode].IHF) ;
             Printf("Repeat rate=%u, Hsync=%u Hz,  Pixel rate=%u MHz\n", wRepeatimes, (100*VGATIMING_TABLE[mode].IHF) ,CW_old);
             x_mem_free(CW);
             
             vRegWrite4B(VFE_15, CW_backup);
             vRegWrite4B(VFE_14,_bVFE14_backup);    //restore VFE_14
             vRegWrite4B(0x240c, dwTmp);
             _wSDDS_delay_counter = 0;
             _bSDDSCaliState = SDDS_CALI_WAIT;

             break;

        case SDDS_CALI_WAIT:
             if(_wSDDS_delay_counter>200)
             {
                _bSDDSCaliState = SDDS_CALI_END;
             }
             break;        

        case SDDS_CALI_END:
            if((_bKI0== _bKI0_MAX) && (_bKP0== _bKP0_MAX))
            {
                _bSDDSCaliState = SDDS_CALI_NOTHING;
                Printf("End of SDDS identification\n");
            }
            else
            {
                if(_bKP0 == _bKP0_MAX)
                {
                    _bKI0 = _bKI0+1;
                    _bKP0 = _bKP0_init;
                }
                else
                {
                    _bKP0 = _bKP0+1;
                }
                _bSDDSCaliState = SDDS_CALI_START;
            }        
            break;        
       }
#endif
}


/**
 * @brief Set the Htotal length
 *
 * Set the quantity of pixel clocks per hsync (hsync length)
 * @param wHtotal wHtotal is the quantity of pixel clocks per hsync
 * @retval void
 */
void vDrvCLKINSetHtotal(UINT16 wHtotal)
{

    if (wHtotal != 0)
    {
        wHtotal --;

        vRegWriteFldAlign(VFE_16, wHtotal, DCLK_HLINE_LENGTH);
        
       // vWriteVDOINMsk(VFE_16, (UINT8) (wHtotal >> 8), 0x3F, 1);
       // vWriteVDOIN(VFE_16, 0, (UINT8) wHtotal);
    }
#if 0
    if (wHtotal == 0)
        return;
    wHtotal --;
    vWriteVDOINMsk(VFE_16, (UINT8) (wHtotal >> 8), C_HLINE_LENGTH1, 1);
    vWriteVDOIN(VFE_16, 0, (UINT8) wHtotal);

#endif

}


/**
 * @brief Set the internal hsync width
 *
 * Set the internal hsync width by pixels
 * @param wWidth wWidth is the quantity of hsync width
 * @retval void
 */
void vDrvCLKINSetHsyncWid(UINT8 wWidth)  //  Only use 8-bits , because the HsyncWifth << 255
{
 
  //vWriteVDOIN(VFE_16, 2, wWidth>>2);
  //vWriteVDOINMsk(VFE_16, (wWidth&0x03)<<6, 0xc0, 1);
  vRegWriteFldAlign(VFE_16, wWidth,DCLK_HSYNC_WIDTH);
  
}

/**
 * @brief VGA mode manual clock adjusting
 *
 * Set the Htotal when VGA mode manual adjusting
 * @param wHtotal wHtotal is the quantity of pixel clocks per hsync
 * @retval void
 */
void vDrvVGASetClock(UINT16 wHtotal)
{
    UINT32 dRealClk;
    UINT32 dCalClk;

   //LogSW("wHtotal", wHtotal);
    dRealClk = ((((((UINT32)_bGetCLKByte3 << 8) | _bGetCLKByte2) << 8) | _bGetCLKByte1) << 8) | _bGetCLKByte0;

    if ((BYTE3(dRealClk) == 14) && (BYTE2(dRealClk) > 0xc0) && (_wVgaClock > wHtotal)) {
        return;
    }
    dRealClk = dRealClk + (0x01000000L);    //(0x00010000L);

    // Cindy: divide by zero case, should we do some protection?
      if(wHtotal == 0)
        {
          wHtotal = 1;
      }

    dCalClk = (dRealClk / wHtotal) * _wVgaClock;
    vDrvCLKINSetHtotal(wHtotal);
    _bCLKByte3 = BYTE3(dCalClk);
    _bCLKByte2 = BYTE2(dCalClk);
    _bCLKByte1 = BYTE1(dCalClk);

     _wVgaClock = wHtotal;
    _bCLKSetFlag |= 0x01;
}

void vDrvInitPhaseVar(void)
{
  #if NEW_PHASE_ALGORITHM
  UINT8 bCnt,bchannel;
        for(bchannel=0; bchannel<CHANNEL_NUM; bchannel++)
        {
             for(bCnt=0; bCnt<31; bCnt++)
             {
                   _dSumTmpPhase[bchannel][bCnt]=0;
             }
             _dBestSumALL[bchannel]=0;
             _bChBestPhase[bchannel]=0xff;
        }
  #endif
}

#if NEW_PHASE_ALGORITHM	
#if Phase3Channel
UINT8 bDrvADCOutputClock(void)
{
UINT8  bMinimum,bMini_Index,bCnt, i;
UINT8  bOrderPhase[6],bTmpPhase[6], bIndex[6];    

      //The following program is to decide the ADC output clock
      //Step 1: sorting the phase ascending
      for ( bCnt = 0; bCnt<CHANNEL_NUM; bCnt++) 
      {
            bTmpPhase[bCnt]=_bChBestPhase[bCnt];  //copy the 3 best phases to tempary variable
          //  Printf("Best phase for 3 channel are CH%u = %u\n",bCnt+1,_bChBestPhase[bCnt]);
      }
      for ( bCnt = 0; bCnt<CHANNEL_NUM; bCnt++) 
      {
          bMinimum =0xff;
          bMini_Index = 0;
          for (i=0; i<CHANNEL_NUM; i++) 
          {
              if ( bTmpPhase[i] < bMinimum )
              {
                  bMinimum = bTmpPhase[i] ;
                  bMini_Index = i;
              }
          }
          bTmpPhase[bMini_Index] = 0xff;  
          bIndex[bCnt]= bMini_Index; 
          bOrderPhase[bCnt]= bMinimum ;
          bIndex[bCnt+CHANNEL_NUM]= bMini_Index; //extend    
          bOrderPhase[bCnt+CHANNEL_NUM]= bOrderPhase[bCnt]+32; //extend      
         // Printf("After sorting best phase: bOrderPhase= %u ,bIndex= %u\n",bOrderPhase[bCnt],bIndex[bCnt]);
       }
      //Step 2: to check the validation of the three phases
      bCnt =0; 
      i=0;
      do 
      {  
           if((bOrderPhase[i])> 63) //because the maximum extended phase=31+32=63
           {  
              bCnt = 0;
           }
           else
           {
           bCnt = (bOrderPhase[i+1]<(bOrderPhase[i]+7)) ? (bCnt+1): (0);
               //Printf("check bOrderPhase[i+1]=%u  and bOrderPhase[i]+7=%u , bcnt=%u\n",bOrderPhase[i+1],(bOrderPhase[i]+7),bCnt);
           }
           i++;
      } while(( bCnt <2) &&(i<5));
      if ((bCnt >1) &&(i>0)&& (i<5)) // i only can be one of 1,2,3,4.
      {
          //Printf("successful condition : i= %u\n",i);
          //Printf("Center Phase=%u, ADC output clock= %u\n",bOrderPhase[i-1],(bIndex[i-1]+1) );
          return (bIndex[i-1]);  //the value of bIndex[x] only can be 0 or 1 or 2
      }
      else 
      {    
           //Printf("Too big difference for 3 channel : Search condition:i= %u\n",i);
           return (0xFF) ;
      }
}
#endif

void vDrvSetBestPhase(void)
{
UINT8 bCnt, bPhase_Index;
#if Phase3Channel
UINT32 dwTmp[MAX_PHASE_ARRAY],dwTmpMAX;
#endif

       #if Phase3Channel

       if (fgIsMainYPbPr() || fgIsPipYPbPr())                  //wu
       {
           bPhase_Index = 0;
           vRegWriteFldAlign(ASYNC_0F, _bChBestPhase[0] , AS_PHASESEL_RX);
           vRegWriteFldAlign(ASYNC_0F, _bChBestPhase[0]  , AS_PHASESEL_GX);
           vRegWriteFldAlign(ASYNC_0F, _bChBestPhase[0]  , AS_PHASESEL_BX);
       }
       else
        {
           bPhase_Index = bDrvADCOutputClock();
           if(bPhase_Index == 0xff) //if no channel clock selected
           {   
                //if the difference between the 3 best phase is too big then calculate the average of the 3 channel 
                //and set the phase which has the maximum PSNE to the 3 clock
                for(bCnt=0; bCnt<(MAX_PHASE_ARRAY-2); bCnt++)
                {
                    dwTmp[bCnt] = _dSumTmpPhase[0][bCnt] + _dSumTmpPhase[1][bCnt]+_dSumTmpPhase[2][bCnt]   ; 
                    //mPrintf("Sum of Phase=%u,  PSNE=%u\n",bCnt, dwTmp[bCnt]);
                }
                dwTmpMAX =0x00;
                for (bCnt=0; bCnt<(MAX_PHASE_ARRAY-2); bCnt++)
                {
                     if(dwTmp[bCnt]>dwTmpMAX)
                     {
                            dwTmpMAX = dwTmp[bCnt];
                            bPhase_Index =bCnt;
                     }
                }
                _bChBestPhase[0] = bPhase_Index;  //force to the same phase
                _bChBestPhase[1] = bPhase_Index;  //force to the same phase
                _bChBestPhase[2] = bPhase_Index;  //force to the same phase
                bPhase_Index = 1;                                 //Use clock 1 to be the ADC output clock
                //Printf("Force ADC output clock=1, Average Phase= %u\n",bPhase_Index);
           }
           
           vRegWriteFldAlign(ASYNC_0F, _bChBestPhase[0] , AS_PHASESEL_RX);
           vRegWriteFldAlign(ASYNC_0F, _bChBestPhase[1]  , AS_PHASESEL_GX);
           vRegWriteFldAlign(ASYNC_0F, _bChBestPhase[2]  , AS_PHASESEL_BX);
        }
           switch (bPhase_Index)
           {
                case 0:
                vIO32WriteFldAlign(REG_VGA_Normal_CFG0, 1, CLKOSEL_1);	
                vIO32WriteFldAlign(REG_VGA_Normal_CFG0, 0, CLKOSEL_2);	
                vIO32WriteFldAlign(REG_VGA_Normal_CFG0, 0, CLKOSEL_3);	
                case 1:
                vIO32WriteFldAlign(REG_VGA_Normal_CFG0, 1, CLKOSEL_2);	
                vIO32WriteFldAlign(REG_VGA_Normal_CFG0, 0, CLKOSEL_1);	
                vIO32WriteFldAlign(REG_VGA_Normal_CFG0, 0, CLKOSEL_3);	
                case 2:
                default:
                vIO32WriteFldAlign(REG_VGA_Normal_CFG0, 1, CLKOSEL_3);	
                vIO32WriteFldAlign(REG_VGA_Normal_CFG0, 0, CLKOSEL_1);	
                vIO32WriteFldAlign(REG_VGA_Normal_CFG0, 0, CLKOSEL_2);	
                break;
            }
           
       #else
           bPhase_Index = 0;
           for(bCnt=0; bCnt<CHANNEL_NUM; bCnt++)
           {
               if(_bChBestPhase[bCnt] <=0x1f)
               {
                   bPhase_Index |=(0x01<<bCnt);
               }
           }
           if(_bChBestPhase[0]>0x1f) //to avoid the unreasonable phase
           {
               _bChBestPhase[0]=0x10;
           }
           vRegWriteFldAlign(ASYNC_0F, _bChBestPhase[0] , AS_PHASESEL_RX);
           vRegWriteFldAlign(ASYNC_0F, _bChBestPhase[0]  , AS_PHASESEL_GX);
           vRegWriteFldAlign(ASYNC_0F, _bChBestPhase[0]  , AS_PHASESEL_BX);
       #endif 
}

void vPhaseSearchCore_New(void)
{
	UINT32 dTmp ;
	UINT8 bRealPhase, a , b ;
	UINT8 bChannel_num;

	_bCurPhase = (_bBestPhase == 31) ? 0 : (_bCurPhase + 1);
       dTmp = dVGAGetAllDiffValue();
       _bBestPhase = 0;
       bRealPhase = (_bCurPhase == 0) ? 31 : (_bCurPhase -1);
       
       for(bChannel_num=0; bChannel_num<CHANNEL_NUM; bChannel_num++)
       {
           if(bRealPhase < MAX_PHASE_ARRAY)
           {
                _dSumTmpPhase[bChannel_num][bRealPhase] = _dwPhase3CH[bChannel_num]; //dTmp ;
           }
       }
        #if Phase3Channel 
              //DBG_Printf(DBG_AUTO,"bRealPhase=%2u, Sum1=%8u,   Sum2=%8u,   Sum3=%8u\n",bRealPhase,_dwPhase3CH[0],_dwPhase3CH[1],_dwPhase3CH[2]);
              DBG_Printf(DBG_AUTO,"3ch bRealPhase= %2u ,Sum1= %8u ,Sum2= %8u ,Sum3= %8u\n",bRealPhase,_dwPhase3CH[0],_dwPhase3CH[1],_dwPhase3CH[2]);
        #else
              DBG_Printf(DBG_AUTO,"bRealPhase=%u, Sum=%u\n",bRealPhase,dTmp);
        #endif
        
              if(bRealPhase == 0)
              {
                  a = 31 ; 
                  b = 0 ;
              }
              else
              {
                  a = bRealPhase -1 ; 
                  b = bRealPhase ;       
              }
              for(bChannel_num=0; bChannel_num<CHANNEL_NUM; bChannel_num++)
              {
                  if ((a<MAX_PHASE_ARRAY) && (b<MAX_PHASE_ARRAY))  //for klocwork check to protect over dimension
                  {
                      if( _dSumTmpPhase[bChannel_num][a] > _dSumTmpPhase[bChannel_num][b])
                      {
                      //find max slop
                          dTmp = _dSumTmpPhase[bChannel_num][a] -_dSumTmpPhase[bChannel_num][b];
                          if(dTmp > _dBestSumALL[bChannel_num] )
                          {
                              _dBestSumALL[bChannel_num] =dTmp ;
                              _bMaxSlope[bChannel_num]  = a ;
                          }    
                      }        
                  }
              }    
              
              if (_bCurPhase <32)   //      (_bCurPhase <= 32)
              {
              //vDrvVGASetPhase((_bCurPhase == 32) ? 0 : _bCurPhase);
              vDrvVGASetPhase_Simple((_bCurPhase == 32) ? 0 : _bCurPhase);
              //_bVgaDelayCnt = 1;
              }
              else
              {     
                 for(bChannel_num=0; bChannel_num<CHANNEL_NUM; bChannel_num++)
                 {
                    _dBestSumALL[bChannel_num]  =  0 ;
                    b= (_bMaxSlope[bChannel_num] >=3)?(_bMaxSlope[bChannel_num]-3): (_bMaxSlope[bChannel_num]+31-2);    //wu
                    //b = _bMaxSlope[bChannel_num] ;	    
                    a = 8;
                    DBG_Printf(DBG_AUTO,"CH %u Sorting Phase from %2u,  %2u\n",(bChannel_num+1),_bMaxSlope[bChannel_num] ,b);
                    //Printf("CH %u Sorting Phase from %2u,  %2u\n",(bChannel_num+1),_bMaxSlope[bChannel_num], b);
                  
                    do
                    {
                          if (b<MAX_PHASE_ARRAY)  //for klocwork check 
                          {
                              DBG_Printf(DBG_AUTO,"bChannel_num =%2u, phase=%2u, data=%8u \n", (bChannel_num+1) ,b,_dSumTmpPhase[bChannel_num][b]);
           
                              if(_dSumTmpPhase[bChannel_num][b] > _dBestSumALL[bChannel_num])
                              {
                                   _dBestSumALL[bChannel_num] = _dSumTmpPhase[bChannel_num][b]  ;
                                   _bChBestPhase[bChannel_num] = b ;
                              }
                              b = (b == 0) ? 31 : (b - 1);	
                              a-- ;
                           }
                           else   //for klocwork check 
                           {
                              a=0;
                            }
                    }while(a > 0);
                 }
              }
}

void vYPbPrPhaseIsr_New(void)    //NEW_PHASE_ALGORITHM
{
        if(_bCurPhase <32 )  // do use <=32
        {
            vPhaseSearchCore_New();
            _RETIMENeedReset = TRUE ;
        }
        else          // if(_bCurPhase >31 )
        {
              vDrvSetBestPhase();
		_RETIMENeedReset = TRUE ;
		vClrAutoFlg(SP0_AUTO_PHASE);

		vApiNotifyAutoDone(((fgIsMainYPbPr())?0:1),TRUE);
		#if NOTIFY_PROGRESS
		vApiNotifyVGAAutoProgress(((fgIsMainYPbPr())?0:1),100);
		#endif
		vApiVideoClrEvtFlg(VDO_FLG_VGAAUTO_DOING);

		vSetSP0AutoState(VDO_AUTO_NOT_BEGIN);
		//vVgaAutoPosInit();
		#if CHANGE_SDDS_KPI
		vDrvEnableChang_SDDS_BW();
		#endif		
		_bVgaDelayCnt = 2;
		_bAutoISR = 0 ;
        #if Phase3Channel 		
		_bChBestPhase[0] = RegReadFldAlign(ASYNC_0F,AS_PHASESEL_RX);
		_bChBestPhase[1] = RegReadFldAlign(ASYNC_0F,AS_PHASESEL_GX);
		_bChBestPhase[2] = RegReadFldAlign(ASYNC_0F,AS_PHASESEL_BX);
		DBG_Printf(DBG_AUTO,"YPbPr ch1 phase=%2u ,ch2 phase=%2u ,ch3 phase=%2u\n",_bChBestPhase[0],_bChBestPhase[1],_bChBestPhase[2]);
        #else
		_bChBestPhase[0] = RegReadFldAlign(ASYNC_0F,AS_PHASESEL_RX);
		DBG_Printf(DBG_AUTO,"YPbPr phase=%2u\n",_bChBestPhase[0]);
        #endif
        
        }
}

void vVgaPhaseIsr_New(void)   //NEW_PHASE_ALGORITHM
{
        if(_bCurPhase <32 )  // do use <=32
        {
            vPhaseSearchCore_New();
            _RETIMENeedReset = TRUE ;
        }
        else          // if(_bCurPhase >31 )
        {
              vDrvSetBestPhase();
		_RETIMENeedReset = TRUE ;
		vClrAutoFlg(SP0_AUTO_PHASE);

		vSetSP0AutoState(VDO_AUTO_POSITION_1_START);
		vVgaAutoPosInit();
        #if CHANGE_SDDS_KPI            
		vDrvEnableChang_SDDS_BW();
	 #endif		
		_bVgaDelayCnt = 2;
		_bAutoISR = 0 ;
        #if Phase3Channel 		
		_bChBestPhase[0] = RegReadFldAlign(ASYNC_0F,AS_PHASESEL_RX);
		_bChBestPhase[1] = RegReadFldAlign(ASYNC_0F,AS_PHASESEL_GX);
		_bChBestPhase[2] = RegReadFldAlign(ASYNC_0F,AS_PHASESEL_BX);
		DBG_Printf(DBG_AUTO,"VGA ch1 phase=%2u ,ch2 phase=%2u ,ch3 phase=%2u\n",_bChBestPhase[0],_bChBestPhase[1],_bChBestPhase[2]);
		//Printf("VGA ch1 phase=%2u ,ch2 phase=%2u ,ch3 phase=%2u\n",_bChBestPhase[0],_bChBestPhase[1],_bChBestPhase[2]);
        #else
		_bChBestPhase[0] = RegReadFldAlign(ASYNC_0F,AS_PHASESEL_RX);
		DBG_Printf(DBG_AUTO,"VGA phase=%2u\n",_bChBestPhase[0]);
        #endif		
        
        }
}
#else
void vPhaseSearchCore(void)
{
	UINT32 dTmp ;

	//find the max phase sum of p-1, p, p+1
	_bCurPhase = (_bBestPhase == 31) ? 0 : (_bCurPhase + 1);
	if (_bCurPhase < 3)
	{
		_dSumTmp[_bCurPhase] = dHDTVGetAllDiffValue();
		_dBestSum = _dBestSum + _dSumTmp[_bCurPhase];
		_bBestPhase = 0;
       #if VGA_AUTO_PHASE_DBG
		//LogD(_dSumTmp[_bCurPhase]);
              if(_bCurPhase==2)
              {
                  DBG_Printf(DBG_AUTO,"bRealPhase=%u, Sum=%8u\n",(_bCurPhase-2),_dBestSum);
              }
       #endif
	}
	else
	{
		_dSumTmp[0] = _dSumTmp[1];
		_dSumTmp[1] = _dSumTmp[2];
		_dSumTmp[2] = dHDTVGetAllDiffValue();

		dTmp = _dSumTmp[0] + _dSumTmp[1] + _dSumTmp[2];
         #if VGA_AUTO_PHASE_DBG
		//LogD(_dSumTmp[2]);	//sc_yang
              DBG_Printf(DBG_AUTO,"bRealPhase=%u, Sum=%8u\n",(_bCurPhase-2),dTmp);
        #endif

		if (dTmp > _dBestSum)
		{
			_dBestSum = dTmp;
			_bBestPhase = _bCurPhase - 2;
		}
	}
}
void vYPbPrPhaseIsr(void)
{
	if (_bCurPhase <= 32)
	{
	       vPhaseSearchCore();
		//vDrvVGASetPhase((_bCurPhase == 32) ? 0 : _bCurPhase);
		vDrvVGASetPhase_Simple((_bCurPhase == 32) ? 0 : _bCurPhase);
		//_bVgaDelayCnt = 1;
	}
	else
	{
		vDrvVGASetPhase(_bBestPhase);	//only this not Simple
		//vDrvVGASetPhase_Simple(_bBestPhase); //Do auto replace current setting
		vClrAutoFlg(SP0_AUTO_PHASE);
          #if VGA_AUTO_UI_DBG
		LogSB("Phase eeprom=>", _bBestPhase);
          #endif
		//vSetSP0AutoState(VDO_AUTO_POSITION_1_START);


		vApiNotifyAutoDone(((fgIsMainYPbPr())?0:1),TRUE);
		#if NOTIFY_PROGRESS
		vApiNotifyVGAAutoProgress(((fgIsMainYPbPr())?0:1),100);
		#endif
		vApiVideoClrEvtFlg(VDO_FLG_VGAAUTO_DOING);

		vSetSP0AutoState(VDO_AUTO_NOT_BEGIN);
		//vVgaAutoPosInit();
		#if CHANGE_SDDS_KPI
		vDrvEnableChang_SDDS_BW();
		#endif		
		_bVgaDelayCnt = 2;
		_bAutoISR = 0 ;
	}
}

void vVgaPhaseIsr(void)
{
	if (_bCurPhase <= 32)
	{
	       vPhaseSearchCore();
		//vDrvVGASetPhase((_bCurPhase == 32) ? 0 : _bCurPhase);
		vDrvVGASetPhase_Simple((_bCurPhase == 32) ? 0 : _bCurPhase); //Do auto replace current setting
		//_bVgaDelayCnt = 1;
	}
	else
	{	
		vDrvVGASetPhase(_bBestPhase);	//only this not Simple
		vClrAutoFlg(SP0_AUTO_PHASE);
        #if VGA_AUTO_UI_DBG
		LogSB("Phase eeprom=>", _bBestPhase);
        #endif
		vSetSP0AutoState(VDO_AUTO_POSITION_1_START);
		vVgaAutoPosInit();
        #if CHANGE_SDDS_KPI            
		vDrvEnableChang_SDDS_BW();
	 #endif		
		_bVgaDelayCnt = 2;
		_bAutoISR = 0 ;
	}
}
#endif //endif NEW_PHASE_ALGORITHM
#endif

// DDDS


#if EnableDDDSCalibration
UINT16 _wDDDS_delay_counter;
UINT8 _bDDDSCaliState;
UINT8 _bDKI0,_bDKP0,_bERROR_LIM;
UINT32 _OUTSTG_OS_KPKI_backup;
UINT8 _bDKP0_MAX, _bDKI0_MAX,_bDKP0_init; //,_bKI0_init;
UINT32 _wControlWord;
enum
{
   DDDS_CALI_NOTHING,
   DDDS_CALI_START,
   DDDS_CALI_WAIT,
   DDDS_CALI_END
};

#define DKI0_MAXVALUE 0x0b
#define DKP0_MAXVALUE 0x0b
#define DMEM_SIZE 0x4000  //0x20000
#define DREPEAT_NUM  8
#endif

void DDDS_wait_message(void)
{
   #if EnableDDDSCalibration
         if  (_bDDDSCaliState == DDDS_CALI_WAIT)
             {
                 _wDDDS_delay_counter= _wDDDS_delay_counter+1;
             }
   #endif
}


void vDrvEnable_DDDS_Calibration(UINT8 bKP0_START,UINT8 bKP0_END, UINT8 bKI0_START, UINT8 bKI0_END, UINT8 ERROR_LIM,UINT32 CW)
{
#if EnableDDDSCalibration
   if(_bDDDSCaliState == DDDS_CALI_NOTHING)
   {
       _OUTSTG_OS_KPKI_backup = IO32ReadFldAlign(DDDS_02,DDDS_02_DDDS_KPKI);  //backup original KP0 and KI0
       _bDDDSCaliState = DDDS_CALI_START;
       if(bKP0_END> DKP0_MAXVALUE)
       {
           bKP0_END = DKP0_MAXVALUE;
       }
       _bDKP0_MAX = bKP0_END;
       
       if(bKI0_END> DKI0_MAXVALUE)
       {
           bKI0_END = DKI0_MAXVALUE;
       }
       _bDKI0_MAX = bKI0_END;
       
       if(bKP0_START>bKP0_END)
       {
          bKP0_START = bKP0_END;
       }
       
       if(bKI0_START>bKI0_END)
       {
          bKI0_START = bKI0_END;
       }

       _bDKP0 = bKP0_START;
       _bDKP0_init = bKP0_START;
       _bDKI0 = bKI0_START;
       _bERROR_LIM = ERROR_LIM;
       _wControlWord = CW;
       //_bKI0_init = bKI0_START;
   }
#endif   
}

#define TO_COUNT_LINE_BY_LINE 1
#if TO_COUNT_LINE_BY_LINE
#define SIMPLE_METHOD  0
#endif
void vDrvDDDS_Response(void)
{
#ifndef CC_MTK_LOADER    
#if EnableDDDSCalibration
    UINT32 CW_backup,dwTmp,CW_old;
    UINT32 *CW;
    UINT8 *fglock;    
    UINT16 i;
    UINT16 j;
    #if TO_COUNT_LINE_BY_LINE
         #if SIMPLE_METHOD
         #else
              UINT16 repeat_counter,wRepeatimes;
              UINT8 k,bTmpCount;//repeat_counter[DREPEAT_NUM];
        #endif
    #endif
    UINT8 mode;
  
    switch (_bDDDSCaliState)
    {
        case DDDS_CALI_NOTHING:
                 break;

        case DDDS_CALI_START:
        #if TO_COUNT_LINE_BY_LINE
             #if SIMPLE_METHOD
             #else
                  k=0;
                  bTmpCount = 0;
                  wRepeatimes = 0;
                  repeat_counter=0;
             #endif
        #endif
             CW = (UINT32 *)x_mem_alloc(DMEM_SIZE); 
             fglock = (UINT8 *)x_mem_alloc(DMEM_SIZE);              
             if(!CW)
             {
              ASSERT(CW);
              return;
             } 
             dwTmp= u4RegRead4B(0x240c);     
             vRegWrite4B(0x240c, 0xffffffff);   //disable all interrupt
             _wDDDS_delay_counter = 0;
             
             CW_backup = IO32ReadFldAlign(DDDS_00,DDDS_00_DDDS_FREQ_CW);   //read original target CW status
             Printf("DDDS target_CW=%7x, KP0=%u,  KI0=%u\n", IO32ReadFldAlign(DDDS_00,DDDS_00_DDDS_FREQ_CW),_bDKP0, _bDKI0);
             
             vIO32WriteFldAlign(DDDS_03, _bERROR_LIM, DDDS_03_DDDS_ERR_LIM);
          	 vIO32WriteFldMulti(DDDS_02, P_Fld(_bDKI0,DDDS_02_DDDS_KI_0)|P_Fld(_bDKP0,DDDS_02_DDDS_KP_0));
	         //vIO32WriteFldMulti(DDDS_02, P_Fld(1,DDDS_02_DDDS_KI_1)|P_Fld(1,DDDS_02_DDDS_KP_1));             
             vIO32WriteFldMulti(DDDS_00,P_Fld(0,DDDS_00_DISP_EN)|P_Fld(0,DDDS_00_FIX_FS_DDDS_SEL));             
             vIO32WriteFldAlign(DDDS_03, 1, DDDS_03_SPREAD_INIT); // disable  SS            
                        
             vIO32WriteFldAlign(DDDS_00,_wControlWord,DDDS_00_DDDS_FREQ_CW);

             
             CW_old = IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_FREQ_CW);
             Printf("DDDS initial_CW=%7x, KP0=%u,  KI0=%u\n", CW_old,_bDKP0, _bDKI0);
             vIO32WriteFldMulti(DDDS_00,P_Fld(1,DDDS_00_DISP_EN)|P_Fld(1,DDDS_00_FIX_FS_DDDS_SEL));             
             vIO32WriteFldAlign(DDDS_03, 0, DDDS_03_SPREAD_INIT); // Enable  SS                    
             
             CW[0] = _wControlWord;             
             for(i=1; i<(DMEM_SIZE/4);i++)
             {
             #if TO_COUNT_LINE_BY_LINE
                 for(j=0; j<4; j++)
                 {
                  //vIO32WriteFldMulti(DDDS_00,P_Fld(1,DDDS_00_DISP_EN)|P_Fld(1,DDDS_00_FIX_FS_DDDS_SEL));                               
                  //vIO32WriteFldAlign(DDDS_03, 0, DDDS_03_SPREAD_INIT); // Enable  SS                                      
                  //vIO32WriteFldAlign(DDDS_03, _bERROR_LIM, DDDS_03_DDDS_ERR_LIM);
                 }
                 #if SIMPLE_METHOD
                        CW[i]  = IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_FREQ_CW);  //read the CW status
                        if (CW[i] ==CW_old)
                        {
                              i=i-1;
                        }
                        else
                        {
                            CW_old = CW[i];
                            //Printf("D_CW=%7x\n",CW[i]);
                        }
                 #else
                 if(repeat_counter>0)
                 {
                     bTmpCount=bTmpCount+1;
                     CW[i]  = IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_FREQ_CW);  //read the CW status
                     fglock[i]  = IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_LOCK);                     
                     if(CW[i] !=CW_old)
                     {
                         bTmpCount = 0;
                         CW_old = CW[i];                                         // update CW_old by this new value
                     }
                     else
                     {
                         if(bTmpCount>=(repeat_counter-1))                   //repeat_counter[DREPEAT_NUM-1]) //store the CW every repeat_counter[DREPEAT_NUM-1] times
                         {
                                 bTmpCount = 0;
                                 CW_old = CW[i];                                         // update CW_old by this new value
                         }
                         else if(i>0)
                         {
                             i = i -1;
                         }
                     }
                 }
                 else
                 {              
                     CW[i]  = IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_FREQ_CW);
                     fglock[i]  = IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_LOCK);                     
                     if(CW[i] ==CW_old)
                     {
                         if(i>0)
                         {
                             bTmpCount=bTmpCount+1;             // increase counter in case of the same value happened
                             i = i-1;
                         }
                     }
                     else                                                       //Current value != old value
                     {
                         if((k<DREPEAT_NUM)&&(bTmpCount>0))
                         {  
                             if(k>0)                                          // skip the first time
                             {
                                 wRepeatimes+=bTmpCount;
                             }
                             bTmpCount = 0;                              //reset counter
                             if(k==(DREPEAT_NUM-1))
                             {
                                 wRepeatimes = wRepeatimes/(DREPEAT_NUM-1);
                                 repeat_counter = 2*wRepeatimes;
                             }
                             k=k+1;
                         }
                         CW_old = CW[i];                                 // update CW_old by this new value
                     }
                 }
                 #endif
             #else
                     CW[i]  = IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_FREQ_CW); 
             #endif
             }
             for(i=0; i<(DMEM_SIZE/4);i++)
             {
                 Printf("CW=%7x fgLock=%1d\n",CW[i],fglock[i]);
                 //Printf("ER=%x, KP0=%x, KI0=%x, SD_CW=%7x\n",_bERROR_LIM, _bDKP0,_bDKI0, CW[i]);
                 //Printf("SDDSCW ERR=%6d,CW=%7x,CW_I=%7x\n", ERR[i], CW[i], CW_INIT);    
             }
            
             CW_old= RegReadFldAlign(VFE_16, DCLK_HLINE_LENGTH)+1;
             if (fgIsMainVga()||fgIsPipVga())
             {
	       	mode=_bVgaTiming;
	      }
	      else
             {
	        mode=_bHdtvTiming;
             }
             CW_old = CW_old*(100*VGATIMING_TABLE[mode].IHF) ;
           #if TO_COUNT_LINE_BY_LINE   
             #if SIMPLE_METHOD
             #else
             Printf("Repeat rate=%u, Hsync=%u Hz,  Pixel rate=%u MHz\n", wRepeatimes, (100*VGATIMING_TABLE[mode].IHF) ,CW_old);
             #endif
           #endif
             x_mem_free(CW);

             vIO32WriteFldAlign(DDDS_00,CW_backup,DDDS_00_DDDS_FREQ_CW);  //restore original target CW
             vIO32WriteFldAlign(DDDS_02,_OUTSTG_OS_KPKI_backup,DDDS_02_DDDS_KPKI);     //restore original KI, KP
             vRegWrite4B(0x240c, dwTmp);
             _wDDDS_delay_counter = 0;
             _bDDDSCaliState = DDDS_CALI_WAIT;

             break;

        case DDDS_CALI_WAIT:
             if(_wDDDS_delay_counter>200)
             {
                _bDDDSCaliState = DDDS_CALI_END;
             }
             break;        

        case DDDS_CALI_END:
            if((_bDKI0== _bDKI0_MAX) && (_bDKP0== _bDKP0_MAX))
            {
                _bDDDSCaliState = DDDS_CALI_NOTHING;
                Printf("End of SDDS identification\n");
            }
            else
            {
                if(_bDKP0 == _bDKP0_MAX)
                {
                    _bDKI0 = _bDKI0+1;
                    _bDKP0 = _bDKP0_init;
                }
                else
                {
                    _bDKP0 = _bDKP0+1;
                }
                _bDDDSCaliState = DDDS_CALI_START;
            }        
            break;        
       }
#endif
#endif
}

/**
 * @brief Display Digital clock setting
 *
 * @param dwDCLK is target frequency(Hz) of display digital clock
 * @retval void
 * FW almost done, but LVDS setting need to check
 */
void vDrvVOFreqSet(UINT32 dwDCLK)   // called by Video
{
    _u4VPllFrequency = dwDCLK;
    vDrvVOPLLFreqSet(dwDCLK);
    // Spread Spectrum Setting
    //#if (WFB_LOCK_ENABLE == WFB_MODE_ON)
    //vDrvSetVOSpread(dwDCLK,  SS_PERCENT, SS_FREQ);
	
    //vDrvSetVOSpread(DRVCUST_OptGet(eSpreadSpectrumPermillage));//for dynamic MDR
	
	//#endif
}
#ifndef CC_MTK_LOADER

UINT32 vDrvReadVoSpreSTEP(void)
{
    return (IO32ReadFldAlign(DDDS_04,DDDS_04_SPREAD_STEP));
}


UINT32 DDDS_GetSpreadSpectrum(void)
{
    return _u4Permillage;
}

UINT32 DDDS_GetSpreadSpectrumFreq(void)
{
    return _u4Freq;
}

void vDrvSetVoSpreSTEP(UINT32 bVal)
{
    vDrvVoDclkSpreSpecSTEP(bVal);  
}

static UINT8 u1SSCliEn=0;
void vDrvSetSSWithCLICmd(UINT8 u1Val)
{
 u1SSCliEn = u1Val;
}

/**
 * @brief Set modulation rate of SS the same as input Hsync
 *
 *  @retval void
 * FW almost done, but LVDS setting need to check
 */
void vDrvSetSSMDRAsInputHsync(void)//for dynamic MDR
{
     UINT32 dwGetVTotal = 0;
     UINT32 dwGetFrameRate = 0;
     UINT32 dwInHsync;
     UINT32 u4LockOthers;
     UINT32 u4FrameRate;
     UINT32 u4Freq;
     UINT32 u4Permillage;

     if(u1SSCliEn)
     {
      return;
     }

     PANEL_GetSpreadSpectrum(&u4Freq, &u4Permillage) ;
	if (u4Freq == 0)
	{
	//Printf("vDrvSetSSMDRAsInputHsync\n");
	     u4LockOthers = 0;
	     if (DRVCUST_PanelQuery(eDDDSLockOnOthers, &u4LockOthers) != 0)
	     {
	            u4LockOthers = 1;
	     }
	     u4FrameRate = (UINT32)bDrvVideoGetRefreshRate(SV_VP_MAIN);

	     if ((u4LockOthers == 0) && 
	     	(u4FrameRate != 50) && (u4FrameRate != 60) && (u4FrameRate != 24) && (u4FrameRate != 25) && (u4FrameRate != 30))
	     {
	          //Printf("------> u4LockOthers == 0 and non-standard FR\n");
	          dwInHsync = 62937; //30000;
	     }
	     else
	     {
	        UINT8 bType = bGetVideoDecType(SV_VP_MAIN);

	        if (bType == SV_VD_TVD3D)
	        {
	            //Printf("------> bType = %d\n", bType);
	            RTvdNSTDStatus rStatus;
	            vTvd3dGetNSTDStatus(&rStatus);
	            if (rStatus.fgIsNSTD)
	            {
		    	     dwGetVTotal = rStatus.wVTotal;
		    	     dwGetFrameRate = rStatus.bRefreshRate;
	            }
	            else
	            {
			     dwGetVTotal = wDrvVideoGetVTotal(SV_VP_MAIN);
			     dwGetFrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);
	            }                
	        }
	        else if (bType == SV_VD_YPBPR)
	        {
	            //Printf("------> bType = %d\n", bType);
	            RHDTVNSTDStatus rHDTVStatus;
	            vHDTVGetNSTDStatus(&rHDTVStatus);
	            if (rHDTVStatus.fgIsNSTD)
	            {
			     dwGetVTotal = rHDTVStatus.wVTotal;
			     dwGetFrameRate = rHDTVStatus.bRefreshRate;
	            }
	            else
	            {
			     dwGetVTotal = wDrvVideoGetVTotal(SV_VP_MAIN);
			     dwGetFrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);
	            }                            
	        }     
	        else
	        {
	            //Printf("------> bType = %d\n", bType);
		     //wDrvVideoGetHTotal(SV_VP_MAIN);
		     dwGetVTotal = wDrvVideoGetVTotal(SV_VP_MAIN);
		     dwGetFrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);
	        }
		     dwInHsync = dwGetVTotal * dwGetFrameRate;
	        if (dwInHsync < 34000)
	        {
	            dwInHsync = dwInHsync * 2;
	        }
	     }
	     //Printf("------> GetVTotal = %d\n", dwGetVTotal);
	     //Printf("------> GetFrameRate = %d\n", dwGetFrameRate);
	     //Printf("------> InputHsync = %d\n", dwInHsync);
	     DDDS_SetSpreadSpectrum(dwInHsync, u4Permillage);
	}
	else
	{
	       //Printf("------> Fixed Modulation Rate\n");
		DDDS_SetSpreadSpectrum(u4Freq, u4Permillage);
	}
}


void vDrvVoDclkSpreSpecFIX_STEP(UINT16 u2Val)
{
  vIO32WriteFldAlign(DDDS_05, u2Val, DDDS_05_FIX_STEP);
  vIO32WriteFldAlign(DDDS_1A, u2Val, DDDS_1A_NOR_FIX_STEP_DOWN); 
  vIO32WriteFldAlign(DDDS_1A, u2Val, DDDS_1A_MUTE_FIX_STEP_DOWN);  
}

/**
 * @brief set DDDS Spread Spectrum
 *
 * Set spreading period and step
 * @param u4Freq  : u4Freq is spread speed (Hz)
 * @param u4Range : u4Range is spread amount (permillage)
 *                  u4Range = upeer + lower Range
 * @retval void
*/
//#define DDDS_SS_STEP_UNIT 38250000 //  459/12*1000000 Hz
#define DDDS_SS_STEP_UNIT 54000000 //  648/12*1000000 Hz
void DDDS_SetSpreadSpectrum(UINT32 u4Freq, UINT32 u4Range)
{
    UINT64 u8Step = 0;
    UINT32 u4Tick;
    UINT32 u4CloseLoop;

    LOG(3, "_u4VPllFrequency=%d Hz\n",_u4VPllFrequency);
    LOG(3, "u4Freq=%d Hz,_u4Permillage=%d (0/00)\n",u4Freq,u4Range);
   
    if (_u4VPllFrequency == 0)
    {
     LOG(3, "[SS] _u4VPllFrequency = 0\n");
     return;
    }
    if ((u4Freq == _u4Freq) && (u4Range == _u4Permillage))
    {
     LOG(3, "[SS] Same SS\n");
     return;
    }

    u4CloseLoop =  IO32ReadFldAlign(DDDS_00, DDDS_00_FIX_FS_DDDS_SEL);    

    _u4Permillage = u4Range;

    if (u4CloseLoop == 0)
    {
    vDrvVoDclkSpreadInit(1);    //VOPLL free run
    }
    
    // [Step1] c_psread_period=0.25* spread_controller_clock / spreading_speed ...(1)
    //             c_psread_period= 1 / 4 * spread_controller_clock / spreading_speed
    //             spreading_speed/spread_controller_clock = 1/4/ c_psread_period ....(1a)    
    if (u4Freq > 0)
    {       
        _u4Freq = u4Freq;
    }
    else
    {
        _u4Freq = 30000; // 62973;
    }

    if(IO32ReadFldAlign(DDDS_04,DDDS_04_STEP_FREQ_SEL))
    {
     u4Tick =  ( DDDS_SS_STEP_UNIT * 2 / 4 ) / _u4Freq;
    }
    else
    {
     u4Tick =  ( DDDS_SS_STEP_UNIT / 4 ) / _u4Freq;
    }

    LOG(3, "u4Tick=%d, _u4Freq=%d \n",u4Tick,_u4Freq);
    
    vDrvVoDclkSpreSpecPeriod(u4Tick);
#if ENABLE_DDDS2_FEEDBACK
     if(u4Range<=60)
     {
      if (u4Tick>10) vDrvVoDclkSpreSpecFIX_STEP(u4Tick-10);
      else  vDrvVoDclkSpreSpecFIX_STEP(10);     
     }
     else
     {
      if (u4Tick>20) vDrvVoDclkSpreSpecFIX_STEP(u4Tick-20);
      else  vDrvVoDclkSpreSpecFIX_STEP(20);
     }     
#endif 

    // [Step2] c_psread_step=4*2^24*syspll/f_dds* spreading_amount/2* spreading_speed / spread_controller_clock   ..(2)
    //             c_spread_cw = syspllclk / f_dds
    // according(1a)(2),
    //             c_psread_step=4*2^24*syspll/f_dds* spreading_amount/2* (1/4/ c_psread_period)
    //             c_psread_step=2^24*syspll/f_dds* spreading_amount/2/ c_psread_period
    //             c_psread_step=2^23*syspll/f_dds* spreading_amount/c_psread_period.........(2a)           

    // Get  syspll * postdiver
    u8Step = (UINT32)BSP_GetPLLClock(SRC_SYSPLL_CLK);
    if (LVDS_OUTPUT_PORT == SINGLE_PORT)
    {
        u8Step = u8Step * 1;    //single : VOPLL -> 1x by OCLK view
    }
    else if(LVDS_OUTPUT_PORT == DUAL_PORT)
    {
       u8Step = u8Step * 2;  //dual : VOPLL -> 2x by OCLK view
    }
    else//four port
    {
       u8Step = u8Step * 4;  //four : VOPLL -> 4x by OCLK view
    } 
    
    u8Step = (u8Step * _u4Permillage) << 23;
    u8Step = u8Div6432(u8Step, _u4VPllFrequency, NULL);
    u8Step = u8Div6432(u8Step, (UINT32)(u4Tick * 1000), NULL);
    // vDrvVoDclkSpreSpecSTEP((UINT32)u8Step);
    vDrvSetVoSpreSTEP((UINT32)u8Step);
    LOG(3, "u8Step=%d \n",u8Step);

    vDrvVoDclkSpreSpecDir(SPREAD_CENTER);

    #if ENABLE_DDDS2_FEEDBACK
    if (u4Range)
    {
     vIO32WriteFldMulti(DDDS_04,P_Fld(1,DDDS_04_SPREAD_CENBAL_EN)|P_Fld(1,DDDS_04_SPREAD_CONT_EN)|
                                P_Fld(1,DDDS_04_SPREAD_FR_EN));    
    }
    else
    {
     vIO32WriteFldMulti(DDDS_04,P_Fld(1,DDDS_04_SPREAD_CENBAL_EN)|P_Fld(1,DDDS_04_SPREAD_CONT_EN)|
                                P_Fld(0,DDDS_04_SPREAD_FR_EN));    
    }    
    #endif

    if (u4CloseLoop == 0)
    {
     HAL_Delay_us(100);   // need to wait more than 1 spread_period  2* 1/30k  = 66.7 us
     vDrvVoDclkSpreadInit(0);    //VOPLL locked
    }
}

void SetSpreadSpectrumOnOff(UINT8 Onoff)
{
	if(Onoff==1)
	{
		vDrvVoDclkSpreSpecPeriod(318); // 30kHz -->  0.25*459/12*1000/30=318.75=0x13E
        #if ENABLE_DDDS2_FEEDBACK    
         vDrvVoDclkSpreSpecFIX_STEP(308);
        #endif  		
	}
	else
	{
		vDrvVoDclkSpreSpecPeriod(0);
        #if ENABLE_DDDS2_FEEDBACK    
         vDrvVoDclkSpreSpecFIX_STEP(0);
        #endif  				
	}
}

/**
 * @brief Display Digital clock , spread spectrum setting
 *
 * @param dwDCLK is target frequency(Hz) of display digital clock
 * @retval void
 * FW almost done, but LVDS setting need to check
 */
void vDrvSetVOSpread(UINT32 u4Permillage)
{
    DDDS_SetSpreadSpectrum(0, u4Permillage);

}
#endif

extern UINT32 IS_Support120HZ(void);
/**
 * @brief vDDDSInit

 * Init the DDDS Settings

 * @param  void
 * @retval void
 */
void vDDDSInit(void)
{
#ifdef SUPPORT_PANEL_ERRORLIMIT_TABLE
#else  
        UINT32 u4Error;
#endif	

    if(!bIsPanelOn())
    {

	Printf("[SA7] vDDDSInit\n");

    vIO32WriteFldMulti(DDDS_01, P_Fld(0x0B, DDDS_01_INIT_SEL)|
                                P_Fld(1,DDDS_01_HLEN_NUM)|P_Fld(1,DDDS_01_HLEN_INT));
    vIO32WriteFldAlign(DDDS_02, 1, DDDS_02_HLEN_DEN);
    vIO32WriteFldAlign(DDDS_05, 0, DDDS_05_DDDS_RESETB);
    vIO32WriteFldAlign(DDDS_03, 1, DDDS_03_MUTE_FUNC_OFF);    
    //SET DDDS_RESETB TO 1:normal operation
    vIO32WriteFldAlign(DDDS_05, 1, DDDS_05_DDDS_RESETB);    
    //SET P/I Gain default value
#ifdef CC_MT5396
	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
	{
		vIO32WriteFldMulti(DDDS_02, P_Fld(1,DDDS_02_DDDS_KI_1)|P_Fld(3,DDDS_02_DDDS_KP_1)|
		                            P_Fld(1,DDDS_02_DDDS_KI_0)|P_Fld(1,DDDS_02_DDDS_KP_0));
	}
	else
#endif
	{
		vIO32WriteFldMulti(DDDS_02, P_Fld(1,DDDS_02_DDDS_KI_1)|P_Fld(3,DDDS_02_DDDS_KP_1)|
		                            P_Fld(3,DDDS_02_DDDS_KI_0)|P_Fld(1,DDDS_02_DDDS_KP_0));
	}

    //SET SCPOS_H_SEL to original vdoin: 0:mddi, 1:blk-ip, 2:vdoin
    vIO32WriteFldAlign(DDDS_02, 2,DDDS_02_SCPOS_H_SEL);

    // Step frequency setting : 0: 459/12=38.25 MHz 1: 459/6 = 76.5 MHz
    vIO32WriteFldAlign(DDDS_04,0,DDDS_04_STEP_FREQ_SEL);

    // For DDDS lock quickly 
    vIO32WriteFldAlign(DDDS_00,1,DDDS_00_H_PRE_LOCK);

	vIO32WriteFldAlign(DDDS_03, 1,DDDS_03_DDDS_ERR_LIM_RES);

    // Set error limit
    vIO32WriteFldAlign(DDDS_03, _u4ErrorLimit, DDDS_03_DDDS_ERR_LIM);
    #ifdef SUPPORT_PANEL_ERRORLIMIT_TABLE
     vDrvSetErrorLimit(PANEL_GetErrorLimit());
    #else  
     if (DRVCUST_PanelQuery(eDDDSErrorLimit, &u4Error) == 0)
     {
      vDrvSetErrorLimit(u4Error);
     }
	#endif	

    #ifndef CC_MTK_LOADER
    #ifdef SUPPORT_DDDS_STEP_TRACKING
    vDrvDDDSStepTrackingOnOff(SV_ON);
    #endif
    #endif


    #if ENABLE_DDDS2_FEEDBACK
      vIO32WriteFldMulti(DDDS_04,P_Fld(0,DDDS_04_SPREAD_CENBAL_EN)|P_Fld(0,DDDS_04_SPREAD_CONT_EN)|
                                 P_Fld(0,DDDS_04_SPREAD_FR_EN));    
      vIO32WriteFldAlign(CKGEN_DDDSVSP_CFG, FS_OC_TSTCK_SEL_DDDS2_NOSS, FLD_FS_OC_TSTCK_SEL);      
    #else
      vIO32WriteFldMulti(DDDS_04,P_Fld(0,DDDS_04_SPREAD_CENBAL_EN)|P_Fld(0,DDDS_04_SPREAD_CONT_EN)|
                                 P_Fld(0,DDDS_04_SPREAD_FR_EN));    
      vIO32WriteFldAlign(CKGEN_DDDSVSP_CFG, FS_OC_TSTCK_SEL_DDDS1_SS, FLD_FS_OC_TSTCK_SEL);
    #endif

    vIO32WriteFldAlign(CKGEN_DDDS1_CKCFG, 1, FLD_DDDS1_CK_SEL);
    }
}

/**
 * @brief Set_DDDS_KPI

 * KP/ KI  of DDDS Setting

 * @param  0: Low gain Other: High gain
 * @retval void
 */

 
void Set_DDDS_KPI(UINT8 bValue)
{
  if(bValue==0)
  {  	
	 vDrvDDDSSetKI0(1);
	 vDrvDDDSSetKP0(3);
     //vDrvSetErrorLimit(_u4ErrorLimit);
     #ifdef NDEBUG
	 Printf("Set VOPLL Low gain\n");			 
	 #endif
  }
  else
  {
	  vDrvDDDSSetKI0(4);
	  vDrvDDDSSetKP0(8);
	  //vDrvSetErrorLimit(0x3F);
	  #ifdef NDEBUG
	  Printf("Set VOPLL high gain\n");		  
	  #endif
  }
}


void vLVDSSetPadMuxEnable(void)
{
    // Enable LVDS Pad
   //mt5396 no need
   // vIO32WriteFldMulti(CKGEN_PADCTRL1, P_Fld(0, FLD_ATE_PINMUX_EN)|P_Fld(0, FLD_SYS_PINMUX_EN)|P_Fld(1, FLD_LVDS_PAD_DIS));

}

/**
 * @brief calculate DDDS Error Limit
 *
 * @param u4Range : _u4Permillage = upeer + lower Range
 *                             
 * @retval u2ErrorLimit
*/

UINT16 u2DDDS_CalErrorLimit(UINT32 _u4Permillage)
{
 UINT32 DDDSCW;
 UINT16 u2ErrorLimit;
 
 DDDSCW = IO32ReadFldAlign(DDDS_00,DDDS_00_DDDS_FREQ_CW);
 u2ErrorLimit = ((_u4Permillage *DDDSCW) >> 24)* 256 / 1000 / 2;
 return (u2ErrorLimit);
}

 /**

 * @brief vDrvVOPLLConfig
 *
 * Config VOPLL clock schema
 *
 * @param  u1ClockSrc: vopll clock source 
 * @retval void 
 */
void vDrvSetVOPLLClockSchema(UINT8 u1ClockSrc)
{
    LOG(3,"vDrvSetVOPLLClockSchema, u1ClockSrc=%d.\n", u1ClockSrc);

    vIO32WriteFldAlign(CKGEN_XDDS_CKCFG, u1ClockSrc, FLD_XDDS_CK_SEL);
	switch (u1ClockSrc)
	{
		case VOPLL_CLK_SRC_DDDS:
			
#ifdef CC_MT5396
			if ((IS_PANEL_PDP)||(IS_PANEL_WXGA240))
			vIO32WriteFldAlign(CKGEN_OCLK_TEST, 1, FLD_OCLK_DIV2_SEL);//for SS-XGA 240P and SP-PDP panel, set /2
            else
			vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0, FLD_OCLK_DIV2_SEL);
#endif

            if (LVDS_OUTPUT_PORT == SINGLE_PORT)
            {
#ifdef CC_MT5396
			  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0, FLD_OCLK_DIV_SEL);
#else
              vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_OCLK_DIV_SEL);
#endif
            }
            else if (LVDS_OUTPUT_PORT == DUAL_PORT)
            {
#ifdef CC_MT5396
              vIO32WriteFldAlign(CKGEN_OCLK_TEST, 1, FLD_OCLK_DIV_SEL);
#else
              vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_OCLK_DIV_SEL);
#endif
            }
            else // FOUR_PORT 
            {
//              if(IS_LVDS_HIGHSPEED)
//              {
//			   vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_OCLK_DIV_SEL);
//              }
//			  else
			  {
#ifdef CC_MT5396
              vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_OCLK_DIV_SEL);
#endif
			  }
            }
			break;
		case VOPLL_CLK_SRC_XTAL:

			break;
		default:	
			break;
	}

}

void vDrvSetOCLKClockSchemaInit(void)
{
	static UINT8 _u1BondingEn = 0;

    Printf("vDrvSetOCLKClockSchemaInit.\n");
	// Bonding control	
	if (((!IS_Support120HZ())&&(LVDS_OUTPUT_PORT == FOUR_PORT))
		||((!IS_SupportCINEMA())&&((IS_PANEL_ASPECT_RATIO_21_9)|| wDISPLAY_WIDTH >= 2560)))	

	{
	 // 1. Change OCLK, OCLK2, TCLK source from GPIO pin.  and power down oclk, tclk
	 #ifdef CC_MT5396
	 _u1BondingEn = 1;
     Printf("Not support 120Hz panel or 21:9 panel \n"); 	
	 vIO32WriteFldMulti(CKGEN_OCLK_TEST, P_Fld(1,FLD_TCLK_TSTSEL)|P_Fld(1,FLD_OCLK_TSTSEL));
	 #else
	 _u1BondingEn = 0;
	 #endif
	}
	#ifdef CC_MT5389
	else if ((IS_SupportWXGAOnly())&&(LVDS_OUTPUT_PORT == DUAL_PORT))
	{
	 _u1BondingEn = 1;
	 Printf("Not support FHD60 panel\n"); 
	}	
	#endif
	else
	{
	 _u1BondingEn = 0;
	}

	if (_u1BondingEn)
	{
#ifdef CC_MT5389
      vIO32WriteFldAlign(CKGEN_OCLK_TEST, 1, FLD_TCLK_PD);
#else
      vIO32WriteFldMulti(CKGEN_OCLK_TEST, P_Fld(1, FLD_TCLK_PD)| P_Fld(1, FLD_OCLK_PD));
#endif
	  ASSERT(0);
	}
	else
	{
	
	}

}

void vDrvSetDDDSInput(UINT8 bType)
{
    switch (bType)
    {
        case SV_VD_MPEGHD:
        case SV_VD_MPEGSD:
            vIO32WriteFldAlign(DDDS_02, 1,DDDS_02_SCPOS_H_SEL);//B2R
            break;

        default:
            vIO32WriteFldAlign(DDDS_02, 2,DDDS_02_SCPOS_H_SEL);//Main
            break;
    }
}

