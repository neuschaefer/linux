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
 * $Author: nick674.liu $
 * $Date: 2012/12/11 $
 * $RCSfile: setpll.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

#include "x_typedef.h"
#include "x_ckgen.h"
#include "x_timer.h"
#include "sv_const.h"

#include "hw_vdoin.h"
#include "hw_ckgen.h"
#include "hw_ddds.h"
#include "hw_tg.h"


#ifndef CC_MTK_LOADER
#include "x_util.h"
#include "feature.h"
#include "drv_display.h"
#else /* CC_MTK_LOADER */
#define u8Div6432(a, b, c)  ((a) / (b))
#endif /* CC_MTK_LOADER */

#include "x_bim.h"
#include "hw_lvds.h"
#include "x_pdwnc.h"
#include "drv_vdoclk.h"
#include "drv_tcon.h"
#include "drv_lvds.h"
#include "drv_vbyone.h"
#include "ostg_if.h"
//=====================================================================
// Configuration

#define VPLL_SPREAD_CW              (0x1fffffff)
#define VPLL_SPREAD_INT_SHIFT       (24)
#ifndef NDEBUG
#define VOPLL_Debug
#endif
#define VOPLL_STRESS_TEST 0

#define ENABLE_DDDS2_FEEDBACK 1
#define SUPPORT_EPI


#define AD_DDDS1_VSP 0x14    
#define AD_DDDS2_VSP 0x15    
#define AD_VPLL_TCON_CK 0x16    
#define AD_LVDS_DPIX_CK 0x17    
#define AD_PPLL_DPIX_CK 0x18    
#define AD_CLKDIG_CTS_D11 0x19 
#define DBGCKSEL_tclk 0x1a 
#define DBGCKSEL_oclk 0x1b 

#define DBGCKSEL_mon_oclk_div 0x53
#define DBGCKSEL_mon_oclk_div2 0x52

#define DBGCKSEL_lvds_clk_cts 0x1c 
#define DBGCKSEL_mon_clk75m_ck 0x54 

#define VOPLLSetRecoverCnt 12


static BOOL _fgVopllUseDDDS = TRUE; //use xtal for first IC verify
UINT8 _fgVOPLLReady = FALSE;
UINT8 _fgPIXPLLReady = FALSE;

UINT8 _Lvds7To4FifoReset = 0;
UINT8 _u1TconMode =0;

UINT8 _u1VCO32PHASE_FREQ =0;


static UINT8 MINI_LVDS_OUTPUT_PORT;
static UINT8 TCLK_PCLK_RATION;
static UINT8 TCLK_PCLK_RATION_NUM;
static UINT8 TCLK_PCLK_RATION_DEN;

enum 
{
  LVDS_MODE,
  MLVDS_MODE,
  VB1_MODE,
  EPI_MODE,
  iDP_MODE
};

enum 
{
  VCO32PHASE_594M,
  VCO32PHASE_297M,
  VCO32PHASE_148P5M,
  VCO32PHASE_222P8M,
  VCO32PHASE_445P5M  
};


/**
 * @brief set VOPLL clock
 *
 * Set video output PLL frequency
 * @param bCLK is the target VOPLL clock rate (MHz)
 * @retval void
 */
#include "panel.h"
#ifdef SUPPORT_CW_CHANGE_BY_STEP 
extern UINT32 _u4CurrentCW;
extern UINT32 _u4TargetCW;
#endif

UINT8 bCheckVOPLLReady(void)
{
 return(_fgVOPLLReady);
}


UINT8 bCheckPIXLLReady(void)
{
 return(_fgPIXPLLReady);
}

#if 0 //def SUPPORT_EPI
void vDrvVPPLLSetting_TRIGGER(UINT8 u1Enable)
{
	if (_u1TconMode==VB1_MODE)
	{
		vIO32WriteFldAlign(CKGEN_VOPLLCFG_04, 1, VOPLL04_VPLL_TRIG);
		vIO32WriteFldAlign(REG_VOPLL_CFG2, 0, RG_VPLL_MONEN);
	}
	else
	{
 vIO32WriteFldAlign(CKGEN_VOPLLCFG_04, u1Enable, VOPLL04_VPLL_TRIG);
}
}
#endif

void vDrvPIXPLLPathEnable(UINT8 u1Enable)
{

  if ((_u1TconMode!=LVDS_MODE) && (u1Enable))
  {
	  if(_u1TconMode==iDP_MODE)
	  {
		  vIO32WriteFldAlign(REG_VOPLL_CFG4, PPLL_REFSEL_IDP, RG_PPLL_REFSEL);
		  vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(0, RG_VPLL_FMEN)|P_Fld(0, RG_VPLL_FPEN));
	  }
	  else
	  {
		  vIO32WriteFldAlign(REG_VOPLL_CFG4, PPLL_REFSEL_VPLL, RG_PPLL_REFSEL);
		  vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(1, RG_VPLL_FMEN)|P_Fld(0, RG_VPLL_FPEN));
	  }
  } 
  else
  {
	  vIO32WriteFldAlign(REG_VOPLL_CFG4, PPLL_REFSEL_IDP, RG_PPLL_REFSEL);
	  vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(0, RG_VPLL_FMEN)|P_Fld(1, RG_VPLL_FPEN));
  }
      
}


void vDrvPIXPLLInit_iDP(void)
{
    vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(1, RG_PPLL_VODEN)|P_Fld(0, RG_PPLL_LF)|P_Fld(0, RG_PPLL_BP)|
                                       P_Fld(0, RG_PPLL_BR)|P_Fld(0, RG_PPLL_DIVEN));

    vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(0, RG_PPLL_PREDIV)|P_Fld(0, RG_PPLL_POSDIV)|P_Fld(0, RG_PPLL_FBKSEL)|
                                       P_Fld(7, RG_PPLL_FBKDIV)|P_Fld(1, RG_PPLL_POSDIV2));
}

void vDrvPIXPLLInit_VbyOne(void)
{
    vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(1, RG_PPLL_VODEN)|P_Fld(1, RG_PPLL_LF)|P_Fld(0, RG_PPLL_BP)|
                                       P_Fld(0, RG_PPLL_BR)|P_Fld(0, RG_PPLL_DIVEN));

    vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(0, RG_PPLL_PREDIV)|P_Fld(0, RG_PPLL_POSDIV)|P_Fld(0, RG_PPLL_FBKSEL)|
                                       P_Fld(15, RG_PPLL_FBKDIV)|P_Fld(1, RG_PPLL_POSDIV2));
}

#ifdef SUPPORT_EPI
void vDrvPIXPLLInit_EPI(void)
{
    vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(1, RG_PPLL_VODEN)|P_Fld(1, RG_PPLL_LF)|P_Fld(0, RG_PPLL_BP)|
                                       P_Fld(0, RG_PPLL_BR)|P_Fld(0, RG_PPLL_DIVEN));

    vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(0, RG_PPLL_PREDIV)|P_Fld(0, RG_PPLL_POSDIV)|P_Fld(0, RG_PPLL_FBKSEL)|
                                       P_Fld(11, RG_PPLL_FBKDIV)|P_Fld(1, RG_PPLL_POSDIV2));
}
#endif


void vDrvVOPLLInit_iDP(void)
{
 #ifdef CC_MT8283_TODO

 #endif
}

#ifdef SUPPORT_EPI
void vDrvVOPLLInit_EPI(void)
{

    	vIO32WriteFldAlign(REG_VOPLL_CFG2, 1, RG_VPLL_MONEN);
    	vIO32WriteFldAlign(REG_VOPLL_CFG2, 2, RG_VPLL_BIR);
    	vIO32WriteFldAlign(REG_VOPLL_CFG1, 1, RG_VPLL_AUTOK_EN);
    	vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0, RG_VPLL_BG_VB1_TX_PWD)|P_Fld(0, RG_VPLL_BG_VB1_PRE_PWD)|P_Fld(0, RG_VPLL_BG_VB1_COMP_PWD));
 		vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(3, RG_VPLL_DIVEN)|P_Fld(2, RG_VPLL_BC)|
 		                                   P_Fld(12, RG_VPLL_BP));
        vIO32WriteFldAlign(REG_VOPLL_CFG4, 0, RG_PPLL_BP);
        vIO32WriteFldAlign(REG_VOPLL_CFG4, 1, RG_PPLL_MONEN);

    	vIO32WriteFldAlign(REG_VOPLL_CFG4, 0, RG_PPLL_TSTEN);
    	vIO32WriteFldAlign(REG_VOPLL_CFG4, 0, RG_PPLL_RESERVE);
	
}
#endif

void vDrvVOPLLInit_VbyOne(void)
{
	//set d1a4 and d1b4
	UINT32 u4DispBit;
    	u4DispBit = DISP_BIT;

    	vIO32WriteFldAlign(REG_VOPLL_CFG2, 1, RG_VPLL_MONEN);
    	vIO32WriteFldAlign(REG_VOPLL_CFG2, 2, RG_VPLL_BIR);
    	vIO32WriteFldAlign(REG_VOPLL_CFG1, 1, RG_VPLL_AUTOK_EN);	
    	vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0, RG_VPLL_BG_VB1_TX_PWD)|P_Fld(0, RG_VPLL_BG_VB1_PRE_PWD)|P_Fld(0, RG_VPLL_BG_VB1_COMP_PWD));
	vIO32WriteFldAlign(REG_VOPLL_CFG1, 0, RG_VPLL_LVDSCHL_EN);		  
    	switch (u4DispBit)
    	{
       	 case DISP_30BIT:
				vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(1, RG_VPLL_DIVEN)|P_Fld(3, RG_VPLL_BC)|
				                                   P_Fld(10, RG_VPLL_BP)); 
                vIO32WriteFldAlign(REG_VOPLL_CFG4, 0, RG_PPLL_BP);
                vIO32WriteFldAlign(REG_VOPLL_CFG4, 1, RG_PPLL_MONEN);
        	break;
        	 case DISP_24BIT:
       	 		vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(3, RG_VPLL_DIVEN)|P_Fld(2, RG_VPLL_BC)|
       	 		                                   P_Fld(12, RG_VPLL_BP));
                vIO32WriteFldAlign(REG_VOPLL_CFG4, 0, RG_PPLL_BP);
                vIO32WriteFldAlign(REG_VOPLL_CFG4, 1, RG_PPLL_MONEN);

        	break;
        	default:
        	break;
    	}
    	vIO32WriteFldAlign(REG_VOPLL_CFG4, 0, RG_PPLL_TSTEN);
    	vIO32WriteFldAlign(REG_VOPLL_CFG4, 0, RG_PPLL_RESERVE);
	
}
void vDrvVOPLLInit_LVDS(void)
{
    // MT5395 PFD/CHP/LPF/BAND setting

  vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0, RG_VPLL_BG_VB1_TX_PWD)|P_Fld(0, RG_VPLL_BG_VB1_PRE_PWD)|P_Fld(0, RG_VPLL_BG_VB1_COMP_PWD));
  vIO32WriteFldAlign(REG_VOPLL_CFG1, 1, RG_VPLL_LVDSCHL_EN);		
  
  vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(3, RG_VPLL_DIVEN)|P_Fld(1, RG_VPLL_BIC)
                                       |P_Fld(2, RG_VPLL_BIR)|P_Fld(2, RG_VPLL_BC)
                                       |P_Fld(0xE, RG_VPLL_BP)|P_Fld(4, RG_VPLL_BR));
                                        
  vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(0, RG_PPLL_BP)|P_Fld(0, RG_PPLL_BR)); 
}
void vDrvVOPLLInit_MINI(void)
{


  if (_u1VCO32PHASE_FREQ == VCO32PHASE_594M)
  {

   vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(2, RG_VPLL_DIVEN)|P_Fld(1, RG_VPLL_BIC)
                                        |P_Fld(1, RG_VPLL_BIR)|P_Fld(3, RG_VPLL_BC)
                                        |P_Fld(0x3, RG_VPLL_BP)|P_Fld(2, RG_VPLL_BR));
                                        
   vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(0, RG_PPLL_BP)|P_Fld(0, RG_PPLL_BR)); 

  }
  else if (_u1VCO32PHASE_FREQ == VCO32PHASE_445P5M)
  {
   vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(2, RG_VPLL_DIVEN)|P_Fld(1, RG_VPLL_BIC)
                                        |P_Fld(2, RG_VPLL_BIR)|P_Fld(2, RG_VPLL_BC)
                                        |P_Fld(0x5, RG_VPLL_BP)|P_Fld(4, RG_VPLL_BR));
      
   vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(0, RG_PPLL_BP)|P_Fld(0, RG_PPLL_BR)); 
   
  }    
  else if (_u1VCO32PHASE_FREQ == VCO32PHASE_297M)
  {
   vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(3, RG_VPLL_DIVEN)|P_Fld(1, RG_VPLL_BIC)
                                        |P_Fld(1, RG_VPLL_BIR)|P_Fld(3, RG_VPLL_BC)
                                        |P_Fld(0x7, RG_VPLL_BP)|P_Fld(4, RG_VPLL_BR));
                                                
   vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(0, RG_PPLL_BP)|P_Fld(0, RG_PPLL_BR)); 

  }  
  else if (_u1VCO32PHASE_FREQ == VCO32PHASE_222P8M)
  {
   vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(3, RG_VPLL_DIVEN)|P_Fld(1, RG_VPLL_BIC)
                                        |P_Fld(1, RG_VPLL_BIR)|P_Fld(2, RG_VPLL_BC)
                                        |P_Fld(0x5, RG_VPLL_BP)|P_Fld(4, RG_VPLL_BR));
      
   vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(0, RG_PPLL_BP)|P_Fld(0, RG_PPLL_BR)); 

  }  
  else if (_u1VCO32PHASE_FREQ == VCO32PHASE_148P5M)
  {
   vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(2, RG_VPLL_DIVEN)|P_Fld(1, RG_VPLL_BIC)
                                        |P_Fld(1, RG_VPLL_BIR)|P_Fld(2, RG_VPLL_BC)
                                        |P_Fld(0x3, RG_VPLL_BP)|P_Fld(4, RG_VPLL_BR));
      
   vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(0, RG_PPLL_BP)|P_Fld(0, RG_PPLL_BR)); 

  }  
}

static BOOL fgVOPLLInit = FALSE;
static UINT8 u1VOPLLType = LVDS_MODE;

void vDrvVOPLLInit(void)
{

    if(u1VOPLLType != _u1TconMode)
    {
        // re-init vopll if tcon mode changed.
        fgVOPLLInit = 0;
    }

    if((!bIsPanelOn()) && (!fgVOPLLInit ))
    {

        _fgVOPLLReady = FALSE;
        _fgPIXPLLReady = FALSE;
       
//       #ifdef SUPPORT_EPI
//       vDrvVPPLLSetting_TRIGGER(0);
//       #endif

       vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_OCLK_SEL); 	
       vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_TCLK_SEL);

       if (BSP_GetIcVersion() >= IC_VER_5396_AC)
       {
           if(_u1TconMode == VB1_MODE)
               vIO32WriteFldAlign(CKGEN_PMUX_MISC, 0, FLD_TCON_OPT);
           else
               vIO32WriteFldAlign(CKGEN_PMUX_MISC, 2, FLD_TCON_OPT);
       }
       // MT5395 VOPLL Power Control
       vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_LVDS_PWD);       
       vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_PWD);      
       vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_PWD);       
       HAL_Delay_us(100);
       
       vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_PWD);
       HAL_Delay_us(1000);       


	   vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0, RG_VPLL_BG_CKEN)|P_Fld(0, RG_VPLL_XTAL_DIV)|P_Fld(8, RG_VPLL_BG_TRIM)|P_Fld(2, RG_VPLL_BG_RASEL)|P_Fld(2, RG_VPLL_BG_RBSEL)
	   	|P_Fld(0, RG_VPLL_BG_PWD)|P_Fld(0, RG_VPLL_BG_VB1_COMP_PWD)|P_Fld(0, RG_VPLL_BG_VB1_PRE_PWD)|P_Fld(0, RG_VPLL_BG_VB1_TX_PWD)|P_Fld(0, RG_VPLL_BG_LVDS_PWD)|P_Fld(1, RG_VPLL_BIAS_BYP));


   	   if(_u1TconMode == VB1_MODE)
       {
   		 vDrvVOPLLInit_VbyOne();
   		 vDrvPIXPLLInit_VbyOne();
   		 printf("###VbyOne VOPLLInit  finsih###");
	   }
       else if(_u1TconMode == iDP_MODE)
       {
     	 vDrvVOPLLInit_iDP();
     	 vDrvPIXPLLInit_iDP();     	 
     	 printf("###iDP VOPLLInit  finsih###");
       }	
#ifdef SUPPORT_EPI
       else if(_u1TconMode == EPI_MODE)
       {
	     vDrvVOPLLInit_EPI();
    	 vDrvPIXPLLInit_EPI();
   	   }
#endif
	   else
	   {
        // MT5365 PFD/CHP/LPF/BAND setting
        vDrvVOPLLInit_LVDS();
	   }
    
	   vDrvPIXPLLPathEnable(1);

       // MT5365 Input clcok type : single-end & VCO Band seclect
       vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_REFSEL);
       vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BAND);
    

       vDrvSetOCLKClockSchemaInit();
             
       vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 7, FLD_RG_DDDS1_PI_C);
       vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 7, FLD_RG_DDDS2_PI_C);
//       #ifdef SUPPORT_EPI
//       vDrvVPPLLSetting_TRIGGER(1);
//       #endif

       fgVOPLLInit = TRUE;       
       u1VOPLLType = _u1TconMode;       
    }
	else
	{
      vDrvSetOCLKClockSchemaInit();	
	}   
}

void vDrvDPIX_CLK_SEL(void)
{
    if (_u1TconMode==MLVDS_MODE)
    {     
     vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_MINIEN, VPLL_POST_SEL);
    }
    else if (_u1TconMode==LVDS_MODE)
    {
     vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_LVDSEN, VPLL_POST_SEL);
    }
    else if (_u1TconMode==VB1_MODE)
    {
     vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_VB1EN, VPLL_POST_SEL);
    }
    #ifdef SUPPORT_EPI
    else if (_u1TconMode==EPI_MODE)
    {
//     vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_EPIEN, VPLL_POST_SEL);
    }    
    #endif
    else if (_u1TconMode==iDP_MODE)
    {
//     vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_IDPEN, VPLL_POST_SEL);     
    }       
}


#define DEFAULT_VCO_PHASE 8
static void vDrvVCOPhase_Init(void)
{
    vIO32WriteFldMulti(REG_VOPLL_CFG2,P_Fld(0x08,RG_VPLL_DLY_CLKA)|P_Fld(0x08,RG_VPLL_DLY_CLKB));
    vIO32WriteFldAlign(REG_VOPLL_CFG1, 0x09, RG_VPLL_DLY_DATA);
    
//    vIO32WriteFldMulti(REG_LVDS_TX_CFG11, P_Fld(1,RG_LVDSA_CLK_IQ_SEL_E3)|P_Fld(1,RG_LVDSA_CLK_IQ_SEL_O3));
//    vIO32WriteFldMulti(REG_LVDS_TX_CFG2, P_Fld(1,RG_LVDSB_CLK_IQ_SEL_E3)|P_Fld(1,RG_LVDSB_CLK_IQ_SEL_O3));
}

void vDrvVCOMode_SEL(void)
{
    if (_u1TconMode==MLVDS_MODE) // 32-Phase VCO
    {     
       vDrvVCOPhase_Init();
    }
    else // 4-Phase VCO
    {
    }
}

UINT8 _u1DddsCWMul =1;
extern UINT32 vDrvGetCurLvdsMode(void);

void vDrvVOPLLSet(void)
{
    UINT8 u1PostDiv=0;
    UINT8 fgLVDSClkDiv2=0;
    UINT8 fgPixelClkDiv2=0;
    UINT8 u1FBDiv=0x07;
    UINT8 u1PPLLFBDiv=0x07;
    UINT8 u1SsOclkDiv = 0;
    UINT32 u4DispBit;
    UINT32 u4VB1Lane;

    u4DispBit = DISP_BIT;
    u4VB1Lane = LVDS_OUTPUT_PORT;
    
//    #ifdef SUPPORT_EPI
//    vDrvVPPLLSetting_TRIGGER(0);
//    #endif

    if (_u1TconMode == MLVDS_MODE)
    {      
      // To OSD and LVDSTx setting
      vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0, FLD_OCLK_DIV_SEL);
      
      
        if (TCLK_PCLK_RATION == TCLK_PCLK_RATION_1)
        {          
         if (MINI_LVDS_OUTPUT_PORT == SINGLE_PORT)
         {
          _u1DddsCWMul =1;
          u1FBDiv=0x03;
		  u1PPLLFBDiv=0x03;
          _u1VCO32PHASE_FREQ = VCO32PHASE_148P5M;
         }
         else  if (MINI_LVDS_OUTPUT_PORT == DUAL_PORT) 
         {
          _u1DddsCWMul =2;
          u1FBDiv=0x07;                
		  u1PPLLFBDiv=0x07;
          _u1VCO32PHASE_FREQ = VCO32PHASE_297M;
         }
         else //if (LVDS_OUTPUT_PORT == FOUR_PORT) 
         {
          _u1DddsCWMul =4;
          u1FBDiv=0x0F;     
		  u1PPLLFBDiv=0x0F;
          _u1VCO32PHASE_FREQ = VCO32PHASE_594M;
         }      
          TCLK_PCLK_RATION_NUM = 1;
          TCLK_PCLK_RATION_DEN = 1;      
         }
         else if  (TCLK_PCLK_RATION == TCLK_PCLK_RATION_1P5)
         {
          _u1DddsCWMul =1;
          u1FBDiv=0x05;             
		  u1PPLLFBDiv=0x03;
          _u1VCO32PHASE_FREQ = VCO32PHASE_222P8M;
          TCLK_PCLK_RATION_NUM = 3;
          TCLK_PCLK_RATION_DEN = 2;
         }
         else if  (TCLK_PCLK_RATION == TCLK_PCLK_RATION_2)
         {
          if (MINI_LVDS_OUTPUT_PORT == SINGLE_PORT)
          {
           _u1DddsCWMul =1;
           u1FBDiv=0x07;                     
		   u1PPLLFBDiv=0x03;
          _u1VCO32PHASE_FREQ = VCO32PHASE_297M;           
          }
          else  if (MINI_LVDS_OUTPUT_PORT == DUAL_PORT) 
          {
           _u1DddsCWMul =2;
           u1FBDiv=0x0F;                      
		   u1PPLLFBDiv=0x07;
          _u1VCO32PHASE_FREQ = VCO32PHASE_594M;
          }        
           TCLK_PCLK_RATION_NUM = 2;
           TCLK_PCLK_RATION_DEN = 1;        
         }
         else if  (TCLK_PCLK_RATION == TCLK_PCLK_RATION_3)
         {
           u1FBDiv=0x0B;      
		   u1PPLLFBDiv=0x3;
           _u1DddsCWMul =1;
           _u1VCO32PHASE_FREQ = VCO32PHASE_445P5M;
           TCLK_PCLK_RATION_NUM = 3;
           TCLK_PCLK_RATION_DEN = 1;        
         }
         else if  (TCLK_PCLK_RATION == TCLK_PCLK_RATION_4)
         {
           u1FBDiv=0x0F;      
		   u1PPLLFBDiv=0x03;
           _u1DddsCWMul =1;
           _u1VCO32PHASE_FREQ = VCO32PHASE_594M;           
           TCLK_PCLK_RATION_NUM = 4;
           TCLK_PCLK_RATION_DEN = 1;        
         } 
		 else if  (TCLK_PCLK_RATION == TCLK_PCLK_RATION_0P75)
		 {
		   if (MINI_LVDS_OUTPUT_PORT == SINGLE_PORT)
		   {
			_u1DddsCWMul =1;
            u1FBDiv=0x02;
            u1PPLLFBDiv=0x3;
		   }	  
		   else if (MINI_LVDS_OUTPUT_PORT == DUAL_PORT) 
		   {
			 _u1DddsCWMul =2;
             u1FBDiv=0x05;
             u1PPLLFBDiv=0x7;
		   }	  
		   TCLK_PCLK_RATION_NUM = 3;
		   TCLK_PCLK_RATION_DEN = 4;
		 }		
         else
         {
           TCLK_PCLK_RATION_NUM = 1;
           TCLK_PCLK_RATION_DEN = 1;
         }        
         vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x01, RG_VPLL_PREDIV)|P_Fld(0x00, RG_VPLL_FBKSEL)
                                              |P_Fld(u1FBDiv, RG_VPLL_FBKDIV));
                     
         // 2.B Set PPLL
         vIO32WriteFldMulti(REG_VOPLL_CFG3,P_Fld(0x00,RG_PPLL_PREDIV)|P_Fld(0x00,RG_PPLL_FBKSEL)
                                              |P_Fld(0x01,RG_PPLL_POSDIV)|P_Fld(0x00,RG_PPLL_POSDIV2)
                                              |P_Fld(u1PPLLFBDiv,RG_PPLL_FBKDIV));                                   
                
         vDrvVOPLLInit_MINI();

    }
    else if (_u1TconMode == LVDS_MODE)
    {
    if (LVDS_OUTPUT_PORT == SINGLE_PORT)
    {
       if(IS_LVDS_HIGHSPEED)
       { 
	   	_u1DddsCWMul =2;
		u1PostDiv=0;
       }
	   else
	   {
        _u1DddsCWMul =1;
        u1PostDiv=1; //total VOPLL = VCO / 2 
   	   }	    
        
        fgLVDSClkDiv2=FALSE;
        fgPixelClkDiv2=TRUE;
        // To OSD and LVDSTx setting
        vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0, FLD_OCLK_DIV_SEL);        
    }
    else  if (LVDS_OUTPUT_PORT == DUAL_PORT) 
    {
        if(IS_LVDS_HIGHSPEED)
        {    
         _u1DddsCWMul =4;
         u1PostDiv=0; 		 
        }
		else
		{
		 _u1DddsCWMul =2;
         u1PostDiv=1;  //total VOPLL = VCO / 2 	 
		}
		fgLVDSClkDiv2=FALSE;   
		fgPixelClkDiv2=FALSE;
        u1SsOclkDiv = 1; //default lvds clock for dual-port

        #ifdef DRV_SUPPORT_EXTMJC
        if (IS_COMPANION_CHIP_ON())
        {
            if ((vDrvGetCurLvdsMode() ==  eLvdsVidYuv422OnePort ) || (vDrvGetCurLvdsMode() ==  eLvdsVidRgb444OnePort )
                || (vDrvGetCurLvdsMode() ==  eLvdsVidYuv422OnePort8Bit ) || (vDrvGetCurLvdsMode() ==  eLvdsVidRgb444OnePort8Bit ))
            {
                u1PostDiv=0;  
                fgLVDSClkDiv2=FALSE;
                fgPixelClkDiv2=TRUE;
                u1SsOclkDiv = 0; //lvds clock for one-port video data
            }
        }   
        #endif
        
        // To OSD and LVDSTx setting        
         vIO32WriteFldAlign(CKGEN_OCLK_TEST, u1SsOclkDiv, FLD_OCLK_DIV_SEL);        
    }
    else //if (LVDS_OUTPUT_PORT == FOUR_PORT) 
    {
        if(IS_LVDS_HIGHSPEED)
        {
         _u1DddsCWMul =8;
        fgLVDSClkDiv2=FALSE;		 
        u1SsOclkDiv = 1;
        }
        else
        {
         _u1DddsCWMul =4;
        fgLVDSClkDiv2=TRUE;		 
        u1SsOclkDiv = 2;        
        }
        u1PostDiv=0;   //total VOPLL = VCO / 1 
        fgPixelClkDiv2=FALSE;

        // To OSD and LVDSTx setting        
        vIO32WriteFldAlign(CKGEN_OCLK_TEST, u1SsOclkDiv, FLD_OCLK_DIV_SEL);
    }
    
    // 2. setup vpll start -----------------------------------------------------

    // 2.A FB divider setting

    // VCO = 74.25  / VPLL_PREDIV * (VPLL_FBDIVBYPEN & VPLL_FBDIV3P5EN) * VPLL01_VPLL_FBDIV

    if(_fgVopllUseDDDS)
    {
    // 519.75 = 74.25 / 2 * 1 *14
	vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x01, RG_VPLL_PREDIV)|P_Fld(0x00, RG_VPLL_FBKSEL)
										 |P_Fld(0x0D, RG_VPLL_FBKDIV));

    }
    else
    {

       if(IS_LVDS_HIGHSPEED)
       {
           // 1080 = 27 / 0 * 1 *40
		   vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x00, RG_VPLL_PREDIV)|P_Fld(0x00, RG_VPLL_FBKSEL)
												|P_Fld(0x14, RG_VPLL_FBKDIV));
       }
       else
       {
    // 540 = 27 / 0 * 1 *20
	vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x00, RG_VPLL_PREDIV)|P_Fld(0x00, RG_VPLL_FBKSEL)
										 |P_Fld(0x14, RG_VPLL_FBKDIV));
       }
    }

    // 2.B Set post divider
    vIO32WriteFldAlign(REG_VOPLL_CFG1, (fgLVDSClkDiv2<<1), RG_VPLL_TXCLK_DIV2);     
    vIO32WriteFldMulti(REG_VOPLL_CFG1,P_Fld(u1PostDiv,RG_VPLL_TXCLK_DIV)|
                                         P_Fld(fgPixelClkDiv2,RG_VPLL_LVDS_DPIX_DIV2));                                   
    vIO32WriteFldAlign(REG_VOPLL_CFG0,fgLVDSClkDiv2,RG_VPLL_POSDIV);                                   
   
    }           
#ifdef SUPPORT_EPI
    else if (_u1TconMode == EPI_MODE)
    {
		if(LVDS_OUTPUT_PORT == FOUR_PORT)
		{	
			_u1DddsCWMul =6;
			vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_POSDIV);
			switch (u4DispBit)
			{
				case DISP_30BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x0, RG_VPLL_PREDIV)|P_Fld(0x0, RG_VPLL_FBKSEL)
														 |P_Fld(16, RG_VPLL_FBKDIV));
			
				break;
				case DISP_24BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x0, RG_VPLL_PREDIV)|P_Fld(0x0, RG_VPLL_FBKSEL)
														 |P_Fld(13, RG_VPLL_FBKDIV));
			
				break;
				case DISP_18BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x0, RG_VPLL_PREDIV)|P_Fld(0x0, RG_VPLL_FBKSEL)
														 |P_Fld(10, RG_VPLL_FBKDIV));
				break;
				default:
				break;
			}
            vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_VB1EN|VPLL_POST_EPIEN, VPLL_POST_SEL);
			vIO32WriteFldMulti(REG_VOPLL_CFG1, P_Fld(0, RG_VPLL_LVDSCHL_EN)|P_Fld(0, RG_VPLL_TXCLK_DIV));		  
            vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_RESYNC_LVDS_SEL_D11);
            vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0, RG_LVDSA_LVDS_SEL);
		
		}
		if (LVDS_OUTPUT_PORT == DUAL_PORT)
		{
			_u1DddsCWMul =3;				   
			vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_POSDIV);
			switch (u4DispBit)
			{
				case DISP_30BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x1, RG_VPLL_PREDIV)|P_Fld(0x1, RG_VPLL_FBKSEL)
														 |P_Fld(16, RG_VPLL_FBKDIV));
			
				break;
				case DISP_24BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x1, RG_VPLL_PREDIV)|P_Fld(0x1, RG_VPLL_FBKSEL)
														 |P_Fld(13, RG_VPLL_FBKDIV));
			
				break;
				case DISP_18BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x1, RG_VPLL_PREDIV)|P_Fld(0x1, RG_VPLL_FBKSEL)
														 |P_Fld(10, RG_VPLL_FBKDIV));
				break;
				default:
				break;
			}
				   
            vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_EPIEN, VPLL_POST_SEL);
			vIO32WriteFldMulti(REG_VOPLL_CFG1, P_Fld(1, RG_VPLL_LVDSCHL_EN)|P_Fld(1, RG_VPLL_TXCLK_DIV)
                                                |P_Fld(0, RG_VPLL_TXCLK_DIV2));       
            vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x1, RG_LVDSA_RESYNC_LVDS_SEL_D11);
            vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x378, RG_LVDSA_LVDS_SEL);
		}
/*		if (LVDS_OUTPUT_PORT == SINGLE_PORT)
		{
			_u1DddsCWMul =1;			   
			vIO32WriteFldAlign(REG_VOPLL_CFG3, 3, RG_PPLL_POSDIV);
			switch (u4DispBit)
			{
				case DISP_30BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x2, RG_VPLL_PREDIV)|P_Fld(0x0, RG_VPLL_FBKSEL)
														 |P_Fld(16, RG_VPLL_FBKDIV));
			
				break;
				case DISP_24BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x2, RG_VPLL_PREDIV)|P_Fld(0x0, RG_VPLL_FBKSEL)
														 |P_Fld(13, RG_VPLL_FBKDIV));
			
				break;
				case DISP_18BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x2, RG_VPLL_PREDIV)|P_Fld(0x0, RG_VPLL_FBKSEL)
														 |P_Fld(10, RG_VPLL_FBKDIV));
				break;
				default:
				break;
			}
		}
*/    
    }
#endif
    else if (_u1TconMode == VB1_MODE)    
    {
                             
		if(u4VB1Lane == FOUR_PORT)
		{	
			if(IS_LVDS_HIGHSPEED)
				_u1DddsCWMul =8;
			else
				_u1DddsCWMul =4;
			vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_POSDIV);
		
		}
		if (u4VB1Lane == DUAL_PORT)
		{
			_u1DddsCWMul =2;				   
			vIO32WriteFldAlign(REG_VOPLL_CFG3, 1, RG_PPLL_POSDIV);
				   
		}
		if (u4VB1Lane == SINGLE_PORT)
		{
			_u1DddsCWMul =1;			   
			vIO32WriteFldAlign(REG_VOPLL_CFG3, 3, RG_PPLL_POSDIV);
		}
		switch (u4DispBit)
		{
			case DISP_36BIT:
				vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x01, RG_VPLL_PREDIV)|P_Fld(0x01, RG_VPLL_FBKSEL)
													 |P_Fld(24, RG_VPLL_FBKDIV));

			break;
			case DISP_30BIT:
				vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x01, RG_VPLL_PREDIV)|P_Fld(0x01, RG_VPLL_FBKSEL)
													 |P_Fld(19, RG_VPLL_FBKDIV));

			break;
	 		case DISP_24BIT:
				vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x01, RG_VPLL_PREDIV)|P_Fld(0x01, RG_VPLL_FBKSEL)
													 |P_Fld(14, RG_VPLL_FBKDIV));
			break;
			default:
			break;
    	}
    	#if (!(VOPLL_STRESS_TEST))
    	  printf("###VbyOne VOPLLSet  finsih###");
    	#endif
    }    
    else if (_u1TconMode == iDP_MODE)
    {
     #ifdef CC_MT8283_TODO 

     #endif
    }

    // 3. set AD_DPIX_CLK output
    vDrvDPIX_CLK_SEL();

    // 4. select VCO mode
    vDrvVCOMode_SEL();    

    
//    #ifdef SUPPORT_EPI
//    vDrvVPPLLSetting_TRIGGER(1);
//    #endif
}

void vDrvVOPLLVCOKBand(void)
{
    if (_u1TconMode==VB1_MODE)
    {
    
    }
}


UINT32 GetPLLCounter(UINT8 u1case)
{
    vIO32WriteFldAlign(CKGEN_PLLCALIB, 0, FLD_CAL_SEL);        
    vIO32WriteFldAlign(CKGEN_PLLCALIB, 0, FLD_CAL_MODE); // 11 bit                   
    
    vIO32WriteFldAlign(CKGEN_PLLCALIB,u1case,FLD_DBGCKSEL);
    vIO32WriteFldAlign(CKGEN_PLLCALIB,0x01,FLD_SOFT_RST_CAL);
    HAL_Delay_us(20);        
    vIO32WriteFldAlign(CKGEN_PLLCALIB,0x01,FLD_CAL_TRI);         
    // Wait calibration finishing.
    while (IO32ReadFldAlign(CKGEN_PLLCALIB, FLD_CAL_TRI)) { }        
    return(IO32ReadFldAlign(CKGEN_PLLCALIBCNT,FLD_CALI_CNT));

}

void u1VOPLLSource(UINT8 u1arg1)
{
    if (u1arg1<2)
    {   
    _fgVopllUseDDDS = !(u1arg1);
    }
}

void vSetVOPLLFreq(UINT32 u4Freq)
{

    UINT32 u4Syspll;
    UINT64 u8Cw;
    UINT8 u1ResetCnt=0,u1Mode=0;    
    UINT16 u2Oclk=0, u2Oclk2=0;   
    UINT32 u2Oclk_target=0, u2Oclk2_target=0;
    UINT32 u2Oclk_targetH=0, u2Oclk_targetL=0,u2Oclk2_targetH=0,u2Oclk2_targetL=0;    
    UINT32 u4RefCount=0;
    UINT32 u4XtalMul=0x12;
    #ifdef VOPLL_Debug  
      UINT32 CW_Read,CWS_Read;
    #endif
    #if VOPLL_STRESS_TEST
      UINT32 u8TestCnt=1000000;
    #endif

    if (u4Freq == 0)
    {
        return;
    }

#ifdef VOPLL_Debug 
 #if (!(VOPLL_STRESS_TEST))
           Printf("[LVDS] u4Freq=%d \n",u4Freq);     
 #endif
#endif

      
    #if VOPLL_STRESS_TEST
    do
    {
    #endif    

     if(!bIsPanelOn())     
     {   
      u1ResetCnt = VOPLLSetRecoverCnt;
     }
     else
     {
      u1ResetCnt = 0;
     }

     #ifdef VOPLL_Debug 
      #if (!(VOPLL_STRESS_TEST))
      Printf("[LVDS] Do Loop=%d \n",u1ResetCnt);     
      #endif
     #endif

     vDrvVOPLLInit();
      
     do
     {
      #ifdef VOPLL_Debug     
      if (u1ResetCnt!=VOPLLSetRecoverCnt)
      {
       Printf("[LVDS] Do Loop=%d \n",u1ResetCnt);     
      } 
      #endif       

      vDrvVOPLLSet();
      
    if(_fgVopllUseDDDS)// use ddds
    {       

//        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 1, FLD_RG_DDDS1_PWDB);
//        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 1, FLD_RG_DDDS2_PWDB);        		
		Printf("[SA7] _fgVopllUseDDDS = True\n");
	        
        // B1. setup ddds start -------------------------------------------------
        vIO32WriteFldAlign(DDDS_03, 1,DDDS_03_SPREAD_INIT);
   
        // B2. calculate control word
        u4Syspll = BSP_GetPLLClock(CAL_SRC_SYSPLL_D2)*2; //SYSPLL = 459MHz in MT5395 //CNTseng check
//		if (_u1TconMode==EPI_MODE)
		{
//			u4Freq = u4Freq*2/3;
		}
        u8Cw = u8Div6432(((UINT64)u4Syspll << VPLL_SPREAD_INT_SHIFT), u4Freq, NULL);
#if DDDS_OUT_DIV2
        u8Cw = (((UINT32)u8Cw*_u1DddsCWMul) & VPLL_SPREAD_CW) >> 1;
#else
        u8Cw = (((UINT32)u8Cw*_u1DddsCWMul) & VPLL_SPREAD_CW);
#endif

#if defined(SUPPORT_CW_CHANGE_BY_STEP) && !defined(CC_MTK_LOADER) 
		if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
		{
        if(!bIsPanelOn())
        {
            vIO32WriteFldAlign(DDDS_00, u8Cw,DDDS_00_DDDS_FREQ_CW);
        }
        _u4CurrentCW = IO32ReadFldAlign(DDDS_00, DDDS_00_DDDS_FREQ_CW);
        _u4TargetCW = (UINT32)u8Cw;
        }
        else
        {
            vIO32WriteFldAlign(DDDS_00, u8Cw,DDDS_00_DDDS_FREQ_CW);
        }
#else
        vIO32WriteFldAlign(DDDS_00, u8Cw,DDDS_00_DDDS_FREQ_CW);
#endif

        // B3. enable init
        vIO32WriteFldAlign(DDDS_03, 0,DDDS_03_SPREAD_INIT);        
        // setup ddds done -----------------------------------------------------
        #ifdef SUPPORT_DDDS_STEP_TRACKING
        vDrvUpdateDisplayCenterCW((UINT32)u8Cw);
        #endif

		vIO32WriteFldAlign(DDDS_03, 1, DDDS_03_DATA_SYNC_AUTO); 	

#if DDDS_OUT_DIV2
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 0, FLD_RG_DDDS1_CKSEL); //VPLL from DDDS1
    #if ENABLE_DDDS2_FEEDBACK
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 0, FLD_RG_DDDS2_CKSEL); //PIXPLL from DDDS2
    #endif
#else
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 1, FLD_RG_DDDS1_CKSEL); //VPLL from DDDS1
    #if ENABLE_DDDS2_FEEDBACK
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 1, FLD_RG_DDDS2_CKSEL); //PIXPLL from DDDS2
    #endif
#endif
    }
    else // use xtal
    {
        u4XtalMul = (u4Freq / _u1DddsCWMul)* 7;
		u4XtalMul =	(u4XtalMul / 27000000) - 1;
		vIO32WriteFldAlign(REG_VOPLL_CFG0, u4XtalMul, RG_VPLL_FBKDIV);
		
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 2, FLD_RG_DDDS1_CKSEL); //VPLL from DDDS1
        #if ENABLE_DDDS2_FEEDBACK        
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 2, FLD_RG_DDDS2_CKSEL); //PIXPLL from DDDS2
        #endif
    }    

    #ifndef CC_FPGA
    vDrvSetVOPLLClockSchema(VOPLL_CLK_SRC_DDDS);
    #endif 

#ifdef CC_MT5396
	if (BSP_GetIcVersion() >= IC_VER_5396_AB)
	{
		if ((IS_PANEL_PDP)||(IS_PANEL_WXGA240))
		vIO32WriteFldAlign(CKGEN_OCLK_TEST, 1, FLD_OCLK_INV);
		else
		vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0, FLD_OCLK_INV);
	}
#endif

    // 3. Power on VOPLL and check clock
    if(bIsPanelOn())
    {
        u1ResetCnt = 0;
    }
    else
    {
     // MT5395 toggle VOPLL Power
     vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_LVDS_PWD);
     vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_VB1_TX_PWD);
     vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_PWD);
     vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BIAS_BYP);
     vIO32WriteFldAlign(REG_VOPLL_CFG3, 1, RG_PPLL_PWD);	   
     HAL_Delay_us(10);
     
     vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_PWD); 
//     vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_PWD);
     if (_u1TconMode!=LVDS_MODE) //use PPLL
     {
        HAL_Delay_us(100);
	 	vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_PWD);
     }
     vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_LVDS_PWD);
     vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_VB1_TX_PWD);
     vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BIAS_BYP);
     HAL_Delay_us(20);
     

       if((u1ResetCnt%4) == 1)
       {      
        #ifdef VOPLL_Debug
         Printf(" [LVDS] Toggle DDDS u1ResetCnt=%d  \n",u1ResetCnt);
        #endif
        vIO32WriteFldAlign(DDDS_03,1,DDDS_03_SPREAD_INIT);
        HAL_Delay_us(20);
        vIO32WriteFldAlign(DDDS_03,0,DDDS_03_SPREAD_INIT);            
        if (_u1TconMode!=LVDS_MODE) //use PPLL
        {
           vIO32WriteFldAlign(REG_VOPLL_CFG3, 1, RG_PPLL_PWD);       
           HAL_Delay_us(200);
           vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_PWD);
        }
       }
      
      vDrvVOPLLVCOKBand();

      if(_u1TconMode==LVDS_MODE)//switch DPIX clock
      {
//        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, FLD_OCLK_SEL_LVDS, FLD_OCLK_SEL); 	  
        u2Oclk =  GetPLLCounter(AD_LVDS_DPIX_CK);      
        u2Oclk2 = GetPLLCounter(AD_VPLL_TCON_CK);//AD_VPLL_LVDS_CTSCLK cntseng   
      }
      else
      {
//        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, FLD_OCLK_SEL_PPLL, FLD_OCLK_SEL); 	
        u2Oclk =  GetPLLCounter(AD_PPLL_DPIX_CK);	  
        u2Oclk2 = GetPLLCounter(AD_VPLL_TCON_CK);//AD_VPLL_LVDS_CTSCLK cntseng   
      }
      
      #ifdef VOPLL_Debug
        #if (!(VOPLL_STRESS_TEST))
            Printf("[LVDS] u2Oclk2=0x%x  \n",u2Oclk2);
            Printf("[LVDS] u2Oclk=0x%x  \n",u2Oclk);            
        #endif
      #endif
       
      #ifdef VOPLL_Debug     
      CW_Read=IO32ReadFldAlign(DDDS_00,DDDS_00_DDDS_FREQ_CW);
      CWS_Read=IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_FREQ_CW);
      
      #if (!(VOPLL_STRESS_TEST))       
      Printf("[LVDS]CW_Read=0x%x \n",CW_Read);
      Printf("[LVDS]CWS_Read=0x%x \n",CWS_Read);  
      #endif 
      #endif

      u1Mode = IO32ReadFldAlign(CKGEN_PLLCALIB, FLD_CAL_MODE);
      
      switch(u1Mode) {
      case 0: u4RefCount = 11; break;
      case 1: u4RefCount = 16; break;
      case 2: u4RefCount = 20; break;
      case 3: u4RefCount = 24; break;
      default: u4RefCount = 11; break;
      }

      if (((LVDS_OUTPUT_PORT == FOUR_PORT)&&(IS_LVDS_HIGHSPEED)&&(_u1TconMode==LVDS_MODE))
	  	  || ((_u1TconMode==VB1_MODE) && (IS_LVDS_HIGHSPEED)))
      {
	  u2Oclk_target= ((u4Freq/2/1000000)<<u4RefCount)/(GET_XTAL_CLK()/1000000);
      }
	  else
	  {
      u2Oclk_target= ((u4Freq/1000000)<<u4RefCount)/(GET_XTAL_CLK()/1000000);
	  }
      u2Oclk_targetH= u2Oclk_target + (u2Oclk_target*5/100);
      u2Oclk_targetL= u2Oclk_target - (u2Oclk_target*5/100);
    
      if(_u1TconMode==MLVDS_MODE)
      {     
       u2Oclk2_target= (u2Oclk_target*TCLK_PCLK_RATION_NUM)/TCLK_PCLK_RATION_DEN;
       u2Oclk2_targetH= u2Oclk2_target + (u2Oclk2_target*5/100);
       u2Oclk2_targetL= u2Oclk2_target - (u2Oclk2_target*5/100);
      }
      else if (_u1TconMode==LVDS_MODE)
      {
       if ((LVDS_OUTPUT_PORT == FOUR_PORT)&&(IS_LVDS_HIGHSPEED))
       {
        u2Oclk2_target= (u2Oclk_target*7)/(_u1DddsCWMul/2)/5;
       } 
       else if ((LVDS_OUTPUT_PORT == DUAL_PORT)&&(IS_LVDS_HIGHSPEED))
       {
        u2Oclk2_target= (u2Oclk_target*7)/(_u1DddsCWMul/2)/2/5;
       } 	   
       else if ((LVDS_OUTPUT_PORT == SINGLE_PORT)&&(IS_LVDS_HIGHSPEED))
       {
        u2Oclk2_target= (u2Oclk_target*7)/(_u1DddsCWMul/2)/2/5;
       }	   
       else
       {
        u2Oclk2_target= (u2Oclk_target*7)/_u1DddsCWMul/2/5;
       } 
      u2Oclk2_targetH= u2Oclk2_target + (u2Oclk2_target*5/100);
      u2Oclk2_targetL= u2Oclk2_target - (u2Oclk2_target*5/100);
      }    
      else if (_u1TconMode==VB1_MODE)
      {
        if(DISP_BIT == DISP_36BIT)
			u2Oclk2_target= (u2Oclk_target*5)/_u1DddsCWMul;
        else if(DISP_BIT == DISP_30BIT)
			u2Oclk2_target= (u2Oclk_target*4)/_u1DddsCWMul;
		else
			u2Oclk2_target= (u2Oclk_target*3)/_u1DddsCWMul;
	   u2Oclk2_targetH= u2Oclk2_target + (u2Oclk2_target*5/100);
	   u2Oclk2_targetL= u2Oclk2_target - (u2Oclk2_target*5/100);
	  }    
      else if (_u1TconMode==EPI_MODE)
      {
		  if(DISP_BIT == DISP_30BIT)
			  u2Oclk2_target= (u2Oclk_target*17)/3/10;
		  else if(DISP_BIT == DISP_24BIT)
			  u2Oclk2_target= (u2Oclk_target*14)/3/10;
		  else
			  u2Oclk2_target= (u2Oclk_target*11)/3/10;
	   u2Oclk2_targetH= u2Oclk2_target + (u2Oclk2_target*5/100);
	   u2Oclk2_targetL= u2Oclk2_target - (u2Oclk2_target*5/100);
      }      
      else if (_u1TconMode==iDP_MODE)
      {
       #ifdef CC_MT8283_TODO 

       #endif
      }      

#if (!(VOPLL_STRESS_TEST))  
          if (_u1TconMode==MLVDS_MODE)
          {
           Printf("[MLVDS]_u1TconMode=0x%x \n",_u1TconMode);
           Printf("[MLVDS]TCLK_PCLK_RATION_NUM=0x%x \n",TCLK_PCLK_RATION_NUM);
           Printf("[MLVDS]TCLK_PCLK_RATION_DEN=0x%x \n",TCLK_PCLK_RATION_DEN);  
           Printf("[MLVDS]_u1VCO32PHASE_FREQ=0x%x \n",_u1VCO32PHASE_FREQ);
           Printf("[MLVDS]u2Oclk_target=0x%x \n",u2Oclk_target);  
           Printf("[MLVDS]u2Oclk2_target=0x%x \n",u2Oclk2_target);            
          }
#endif

    
    if(((((u2Oclk2>=u2Oclk2_targetL)&&(u2Oclk2<=u2Oclk2_targetH))&&((u2Oclk>=u2Oclk_targetL)&&(u2Oclk<=u2Oclk_targetH)))
    	 && ((_u1TconMode == MLVDS_MODE)||(_u1TconMode == LVDS_MODE)))
    	 || (((u2Oclk>=u2Oclk_targetL)&&(u2Oclk<=u2Oclk_targetH))&& ((_u1TconMode == VB1_MODE)||(_u1TconMode == EPI_MODE))))
    	 
    {
       u1ResetCnt=0;
       if(_u1TconMode != VB1_MODE)
       {	   
       //LVDS 7->4 FIFO enable // it's already output mode.                // already on.
       _Lvds7To4FifoReset = 2; // for compiler error       
       }
       _fgVOPLLReady = TRUE;
       if (_u1TconMode==EPI_MODE) _fgPIXPLLReady = TRUE;
       else _fgPIXPLLReady = FALSE;
    #if (!(VOPLL_STRESS_TEST))
       Printf("[LVDS] VOPLL Initialize successful !\n");
    #endif
		if(_u1TconMode==LVDS_MODE)//switch DPIX clock
		{
		  vIO32WriteFldAlign(CKGEN_VPCLK_CFG, FLD_OCLK_SEL_LVDS, FLD_OCLK_SEL); 	
		  vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_TCLK_SEL);	// vpll_tcon_ck	
		}
		else
		{
		  vIO32WriteFldAlign(CKGEN_VPCLK_CFG, FLD_OCLK_SEL_PPLL, FLD_OCLK_SEL);   
		  vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_TCLK_SEL);	// vpll_tcon_ck	
		}

		#ifdef CC_MT5396
		 vIO32WriteFldAlign(CKGEN_MBIST_CK_SEL, 0, FLD_BIST_VPCLK_SEL);
		#else
		 vIO32WriteFldAlign(CKGEN_MBIST_CK_CTL, 0, FLD_VPCLK_SEL);
		#endif        
         
		 vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0,RG_LVDSA_EN);	 
		 HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
		 vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 1,RG_LVDSA_EN);	
		 

      }        
      else
      {
       u1ResetCnt--;      
       #ifdef VOPLL_Debug 
       Printf("[LVDS] u2Oclk2=0x%x \n",u2Oclk2);
       Printf("[LVDS] u2Oclk=0x%x \n",u2Oclk);
       if(u2Oclk2==u2Oclk)
       {
        Printf("[LVDS] VOPLL Reset Fail- OCLK no clock\n");          
       }
       else
       {
        Printf("[LVDS] VOPLL Reset Fail- OCLK wrong clock \n");          
       }
       Printf("[LVDS] CW_Read=0x%x \n",CW_Read);
       Printf("[LVDS] CWS_Read=0x%x \n",CWS_Read);       
       if (u1ResetCnt==0)
       {
        Printf("[LVDS] VOPLL Rrecover Fail \n");          
       }
       #endif
       if (u1ResetCnt==0)
       {       
        Printf("[LVDS] u2Oclk2=0x%x \n",u2Oclk2);
        Printf("[LVDS] u2Oclk=0x%x \n",u2Oclk);       
        Printf("[MLVDS]u2Oclk_target=0x%x \n",u2Oclk_target);  
        Printf("[MLVDS]u2Oclk2_target=0x%x \n",u2Oclk2_target);          
        Printf("[LVDS] VOPLL Initialize Fail !\n");
       } 
     #if VOPLL_STRESS_TEST
	   while(1);
      #endif    
     }
      }      
     }while(u1ResetCnt>0);
     
     #if VOPLL_STRESS_TEST
	 u8TestCnt--;
	 Printf("%d \n",u8TestCnt);	   
     }while(u8TestCnt>0);
     #endif    

}

void vPanelPowerSaving(void)
{

	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_LVDS)
	{
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_TCON);
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_MLVDS);
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_VB1);
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_EPI);
	}
	else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_MLVDS)
	{
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_TCON);
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_MLVDS);
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_VB1);
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_EPI);	
	}
	else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
	{
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_TCON);
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_MLVDS);
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_VB1);
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_EPI);	
	}	 
	else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
	{
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_TCON);
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_MLVDS);
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_VB1);
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_EPI);
	}
	else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_iDP)
	{
	}  

}

void vDrvVOPLLFreqSet(UINT32 u4Freq)
{
  PANEL_MINILVDS_ATTRIBUTE_T *pParam;
  if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_LVDS)
  {
   _u1TconMode = LVDS_MODE;
  }
  else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_MLVDS)
  {

   _u1TconMode = MLVDS_MODE;
   if (PANEL_GetMlvdsParam(&pParam) == SV_TRUE)
   {
    if(pParam != NULL)
    {   
     MINI_LVDS_OUTPUT_PORT = pParam->u1Port;
     TCLK_PCLK_RATION = pParam->u1Ratio; 
    }
   }
  }
  else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
  {
   _u1TconMode = VB1_MODE;
  }    
  else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
  {
   _u1TconMode = EPI_MODE;
  }
  else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_iDP)
  {
   _u1TconMode = iDP_MODE;
  }  
  vSetVOPLLFreq(u4Freq);
  vPanelPowerSaving();  
}

void vDrvMLDSVOPLLFreqSet(UINT32 u4Freq, UINT8 MLVDS_PORT,UINT8 MLVDS_RATION)
{
  _u1TconMode = MLVDS_MODE;
  MINI_LVDS_OUTPUT_PORT=MLVDS_PORT;
  TCLK_PCLK_RATION=MLVDS_RATION;
#ifdef VOPLL_Debug 
  Printf("[LVDS] MINI_LVDS_OUTPUT_PORT=%d \n",MINI_LVDS_OUTPUT_PORT);     
  Printf("[LVDS] TCLK_PCLK_RATION=%d \n",TCLK_PCLK_RATION);     
#endif  
  vSetVOPLLFreq(u4Freq);
}

void vDrvVB1VOPLLFreqSet(UINT32 u4Freq)
{
  _u1TconMode = VB1_MODE;
  vSetVOPLLFreq(u4Freq);
}

void vDrvEPIVOPLLFreqSet(UINT32 u4Freq)
{
  _u1TconMode = EPI_MODE;
  vSetVOPLLFreq(u4Freq);
}

void vDrviDPVOPLLFreqSet(UINT32 u4Freq)
{
  _u1TconMode = iDP_MODE;
  vSetVOPLLFreq(u4Freq);
}

void vDrvVOPLLStatus(void)
{
    UINT32 u4Syspll, u4cw, u4dclock, VCO_clk, PPLL_VCO_clk, DPIX_clk, CTS_clk,u4HFreq_ext;
    UINT8 u1MutFactor1,u1MutFactor2, u1DivFactor1, u1DivFactor2, u1DivFactor4, u1DivFactor5;
    UINT16 u2DivFactor3;    

    Printf("==[TconMode]==\n");
	Printf("_u1TconMode = %d ,",_u1TconMode); 
	if (_u1TconMode == LVDS_MODE)
    Printf("[0]LVDS \n"); 	
	else if (_u1TconMode == MLVDS_MODE)
    Printf("[1]MLVDS \n"); 		
	else if (_u1TconMode == VB1_MODE)
    Printf("[2]VB1 \n"); 		
	else if (_u1TconMode == EPI_MODE)
    Printf("[3]EPI \n"); 		
	else if (_u1TconMode == iDP_MODE)
    Printf("[4]iDP \n"); 		
	
	if(_fgVopllUseDDDS)
	{	
    Printf("==[DVOPLL]==\n");  
    u4cw = IO32ReadFldAlign(DDDS_00,DDDS_00_DDDS_FREQ_CW);
    Printf("Close loop=%d\n",IO32ReadFldAlign(DDDS_00,DDDS_00_DISP_EN));
    Printf("CW=0x%7x, CWS=0x%7x, Error Limit=0x%3x\n",u4cw,IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_FREQ_CW),IO32ReadFldAlign(DDDS_03,DDDS_03_DDDS_ERR_LIM));
    Printf("Lock Status=%d, Overflow Status=%d\n",IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_LOCK)
                                                 ,IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_FIFO_OFLOW));        
    u4Syspll = BSP_GetPLLClock(CAL_SRC_SYSPLL_D2)*2; //SYSPLL = 459MHz in MT5395 //CNTseng check
    Printf("Syspll=%d \n",u4Syspll);         
    u4dclock = u8Div6432(((UINT64)u4Syspll << VPLL_SPREAD_INT_SHIFT), u4cw, NULL);
    Printf("DVOPLL Clock=%d Hz \n",u4dclock);     	
    u4HFreq_ext = ((IO32ReadFldAlign(DDDS_02,DDDS_02_HLEN_DEN) * IO32ReadFldAlign(DDDS_01,DDDS_01_HLEN_INT)) + IO32ReadFldAlign(DDDS_01,DDDS_01_HLEN_NUM)) * 10 / IO32ReadFldAlign(DDDS_02,DDDS_02_HLEN_DEN);
	u4HFreq_ext = u4dclock / u4HFreq_ext * 10;
    Printf("HFreq_ext=%d Hz \n",u4HFreq_ext);     
	}
	else
	{
     u4dclock = 27000;
	 Printf("PLL Source : Xtal \n");     
	}

    // 519.75 = 74.25 / 2 * 1 *14
    u1DivFactor1 = IO32ReadFldAlign(REG_VOPLL_CFG0,RG_VPLL_PREDIV);    
    if (u1DivFactor1==0)
    u1DivFactor1 = 1;
    else if (u1DivFactor1==1)
    u1DivFactor1 = 2;    
    else if (u1DivFactor1==3)
    u1DivFactor1 = 4;    

    u1MutFactor1 = IO32ReadFldAlign(REG_VOPLL_CFG0, RG_VPLL_FBKSEL);

    if (u1MutFactor1 == 0)
    u1MutFactor1 = 10;  // 1.0 * 10
    else if (u1MutFactor1==1)
	u1MutFactor1 = 20;	// 2.0 * 10
    else
    u1MutFactor1 = 40;  // 4.0 * 10

    u1MutFactor2 = IO32ReadFldAlign(REG_VOPLL_CFG0, RG_VPLL_FBKDIV)+1;

    VCO_clk = u4dclock/u1DivFactor1*u1MutFactor1/1000*u1MutFactor2/10;

    Printf("==[AVOPLL]==\n"); 
    Printf("u1DivFactor1=%d, u1MutFactor1=%d,u1MutFactor2=%d \n",u1DivFactor1,u1MutFactor1,u1MutFactor2);         
    Printf("VCO Clock=%d KHz\n",VCO_clk);

    u4dclock = u4dclock/u1DivFactor1; //VPLL_PREDIV
    u1DivFactor1 = IO32ReadFldAlign(REG_VOPLL_CFG3,RG_PPLL_PREDIV);    
    if (u1DivFactor1==0)
    u1DivFactor1 = 1;
    else if (u1DivFactor1==1)
    u1DivFactor1 = 2;    
    else if (u1DivFactor1==3)
    u1DivFactor1 = 4;    

    u1MutFactor1 = IO32ReadFldAlign(REG_VOPLL_CFG3, RG_PPLL_FBKSEL);

    if (u1MutFactor1 == 0)
    u1MutFactor1 = 10;  // 1.0 * 10
    else if (u1MutFactor1==1)
	u1MutFactor1 = 20;	// 2.0 * 10
    else
    u1MutFactor1 = 40;  // 4.0 * 10

    u1MutFactor2 = IO32ReadFldAlign(REG_VOPLL_CFG3, RG_PPLL_FBKDIV)+1;

    PPLL_VCO_clk = u4dclock/u1DivFactor1*u1MutFactor1/1000*u1MutFactor2/10;

    Printf("==[AVOPLL]==\n"); 
    Printf("u1DivFactor1=%d, u1MutFactor1=%d,u1MutFactor2=%d \n",u1DivFactor1,u1MutFactor1,u1MutFactor2);         
    Printf("PPLL VCO Clock=%d KHz\n",PPLL_VCO_clk);


    if (_u1TconMode == LVDS_MODE)
    {
     u1DivFactor2 = IO32ReadFldAlign(REG_VOPLL_CFG1, RG_VPLL_TXCLK_DIV);
     u2DivFactor3 = IO32ReadFldAlign(REG_VOPLL_CFG1, RG_VPLL_LVDS_DPIX_DIV2); 
     u1DivFactor4 = 1;
     u1DivFactor5 = IO32ReadFldAlign(REG_VOPLL_CFG1, RG_VPLL_TXCLK_DIV2);

     if (u2DivFactor3 == 0)
     u2DivFactor3 = 175; // 1.75*100
     else
     u2DivFactor3 = 350; // 3.5*100     

     if (u1DivFactor5 == 1)
     u1DivFactor5 = 2;
     else
     u1DivFactor5 = 1;
     
    }
    else
    {
      u1DivFactor2 = IO32ReadFldAlign(REG_VOPLL_CFG3, RG_PPLL_POSDIV);    
      u2DivFactor3 = IO32ReadFldAlign(REG_VOPLL_CFG3, RG_PPLL_POSDIV2);      
	  if (u2DivFactor3==0)
		  u2DivFactor3 = 1;
	  else if (u2DivFactor3==1)
		  u2DivFactor3 = 2;
	  else if (u2DivFactor3==2)
		  u2DivFactor3 = 4;
	  else
		  u2DivFactor3 = 8;
      u1DivFactor4 = 1;
      u1DivFactor5 = 1;
    }

    if (u1DivFactor2==0)
    u1DivFactor2 = 1;
    else if (u1DivFactor2==1)
    u1DivFactor2 = 2;    
    else
    u1DivFactor2 = 4;  
   
    if (_u1TconMode == LVDS_MODE)
	    DPIX_clk = VCO_clk / u1DivFactor2 / u2DivFactor3 /u1DivFactor4 * 100;
	else
	    DPIX_clk = PPLL_VCO_clk / u1DivFactor2 / u2DivFactor3 /u1DivFactor4 * 100;
    Printf("u1DivFactor2=%d, u2DivFactor3=%d, u1DivFactor4=%d\n",u1DivFactor2,u2DivFactor3,u1DivFactor4);    
    Printf("DPIX_clk=%d KHz\n",DPIX_clk);

     CTS_clk = VCO_clk / 2;
     Printf("CTS_clk=%d KHz\n",CTS_clk);

    Printf("==[Clock Measure]==\n"); 

    if (_u1TconMode==MLVDS_MODE)
    {
     Printf("[MLVDS]_u1TconMode=0x%x \n",_u1TconMode);
     Printf("[LVDS] TCLK_PCLK_RATION=%d \n",TCLK_PCLK_RATION);      
     Printf("[MLVDS]TCLK_PCLK_RATION_NUM=0x%x \n",TCLK_PCLK_RATION_NUM);
     Printf("[MLVDS]TCLK_PCLK_RATION_DEN=0x%x \n",TCLK_PCLK_RATION_DEN);     
     Printf("[MLVDS]_u1VCO32PHASE_FREQ=0x%x \n",_u1VCO32PHASE_FREQ);
    } 
 
    Printf("[MLVDS]AD_DDDS1_VSP=%d \n",_CalGetPLLCounter(AD_DDDS1_VSP));
    Printf("[MLVDS]AD_DDDS2_VSP=%d \n",_CalGetPLLCounter(AD_DDDS2_VSP));     

    Printf("[LVDS]AD_VPLL_TCON_CK=%d \n",_CalGetPLLCounter(AD_VPLL_TCON_CK));
    Printf("[LVDS]AD_LVDS_DPIX_CK=%d \n",_CalGetPLLCounter(AD_LVDS_DPIX_CK));
    Printf("[LVDS]AD_PPLL_DPIX_CK=%d \n",_CalGetPLLCounter(AD_PPLL_DPIX_CK));

    Printf("[LVDS]AD_CLKDIG_CTS_D11=%d \n",_CalGetPLLCounter(AD_CLKDIG_CTS_D11));
    Printf("[LVDS]tclk=%d \n",_CalGetPLLCounter(DBGCKSEL_tclk));
    Printf("[LVDS]olck=%d \n",_CalGetPLLCounter(DBGCKSEL_oclk));
    Printf("[LVDS]lvds_clk_cts=%d \n",_CalGetPLLCounter(DBGCKSEL_lvds_clk_cts));
    Printf("[LVDS]mon_clk75m_ck=%d \n",_CalGetPLLCounter(DBGCKSEL_mon_clk75m_ck));
    Printf("[LVDS]DBGCKSEL_mon_oclk_div=%d \n",_CalGetPLLCounter(DBGCKSEL_mon_oclk_div));
    Printf("[LVDS]DBGCKSEL_mon_oclk_div2=%d \n",_CalGetPLLCounter(DBGCKSEL_mon_oclk_div2));	


}

#ifdef CC_FAST_INIT
void VOPLL_pm_resume(void)    
{
 fgVOPLLInit = FALSE;
}
#endif

