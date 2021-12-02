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

#include "x_typedef.h"
#include "x_ckgen.h"
#include "x_timer.h"

#include "hw_vdoin.h"
#include "hw_ckgen.h"

#ifndef CC_MTK_LOADER
#include "x_util.h"
#else /* CC_MTK_LOADER */
#define u8Div6432(a, b, c)  ((a) / (b))
#endif /* CC_MTK_LOADER */

#include "x_bim.h"
#include "hw_scpos.h"


//=====================================================================
// Configuration

#define VPLL_SPREAD_CW              (0x0fffffff)
#define VPLL_SPREAD_INT_SHIFT       (24)


static BOOL _fgVopllUseDDDS = TRUE;
//static UINT32 _u4CurrentVClk = 0;
BOOL _Lvds7To4FifoReset = 0;

#define VOPLL_Debug
#define VOPLL_STRESS_TEST 0
#define VOPLL_STRESS_TEST_XTAL 0
#define VOPLLSetRecoverCnt 12


#if 0  // please hekp to remove this function by YJ
UINT8 Is_MT8223_E1()
{
    return (u1RegRead1B(0x83) != 0xA6);
}
#endif


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
void vDrvVOPLLSet(UINT32 u4Freq)
{

//copy from MT8281-------------------------------------------
    static BOOL fgInit = FALSE;
    //UINT32 u4Val; 
    UINT32 u4Syspll;
    #ifdef CC_UP8032_ATV
    UINT32 dTmp;
    #else
    UINT64 u8Cw;
    #endif

    UINT8 u1FBdiv=0;
    UINT8 u1DddsCWMul =1;
    UINT8 fgLVDSClkDiv2=0;
    UINT8 fgPixelClkMul2=0;
    UINT32 u4CurrentPClk=0;//for band selection
    UINT16 u2Oclk=0, u2Oclk2=0;
    UINT8 u1ResetCnt=0;    
    UINT32 u2Oclk_target=0, u2Oclk2_target=0;
    UINT32 u2Oclk_targetH=0, u2Oclk_targetL=0,u2Oclk2_targetH=0,u2Oclk2_targetL=0;        
    UINT32 u4RefCount=0;
    #ifdef CC_UP8032_ATV
    UINT32 u4Freq_tmp;
    #endif
#ifdef VOPLL_Debug  
        UINT32 CW_Read,CWS_Read;
#endif

    if (u4Freq == 0)
    {
        return;
    }

      // save current value
    //_u4CurrentVClk = u4Freq;

    // save current pixel clk for band selection
    u4CurrentPClk = u4Freq;
    #ifdef CC_UP8032_ATV
    u4Freq_tmp=u4Freq;
    #endif

   //set init value of VOPLL at vDrvVOPLLPWUPIni()

#if VOPLL_STRESS_TEST
   do
   {
    vKrDelay1ms(200);
#endif  

    if (!fgInit)
    {   
     u1ResetCnt = VOPLLSetRecoverCnt;
    }
    else
    {
     u1ResetCnt = 0;
    }

    #ifdef VOPLL_Debug
    //LogSB("u1ResetCnt",u1ResetCnt);
    #endif
    
    do
    {
    
    if (!fgInit)
    {


       //add MT8281 VOPLL initial value
       vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 2, FLD_RG_VPLL_VCO_I);
       vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 7, FLD_RG_VPLL_VBGSEL);//designer advise "7"
       vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_PFDEN);
       vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 2, FLD_RG_VPLL_LPFR);//set LF value from designer
       vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_INCLK_SEL);//VOPLL single-end I/P
       vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_ILVDS_PD);
       vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_ICP);//set Icp value from designer advise "1"
       vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_IBGSEL);
       vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 2, FLD_RG_VPLL_VCO_R);
//copy from MT8281--------------------------------------------


        // Inverse LVDS clock, confirmed by BJ & Jeffrey
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_LVDS_DPIX_SEL);

        // OCLK selection = 3: ~vopll_ck
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0x1, FLD_OCLK_SEL);//MT8223 main only//the same as MT5392B
    }
    
    
    if (LVDS_OUTPUT_PORT == SINGLE_PORT)
    {
        u1FBdiv=1; //total VOPLL(Pre + FB divider) : 3.5x
        u1DddsCWMul =1;
        fgLVDSClkDiv2=FALSE;
        fgPixelClkMul2=FALSE;
 #ifdef CC_MT8223
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_OCLK_DIV_SEL);//MT8223 "Div 1"
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_OCLK_DIV_TOG);
 #else
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 2, FLD_OCLK_DIV_SEL);//MT8223 "Div 1"
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_OCLK_DIV_TOG);
        //XBYTE[0xaa1] = (XBYTE[0xaa1] & ~0xC) | 0x8;
 #endif
    }
    else  if (LVDS_OUTPUT_PORT == DUAL_PORT) 
    {
        u1FBdiv=1; //total VOPLL(Pre + FB divider) : 3.5x
        u1DddsCWMul =2;
        fgLVDSClkDiv2=FALSE;
        fgPixelClkMul2=TRUE;
#ifdef CC_MT8223
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_OCLK_DIV_SEL);//MT8223  "Div 2"
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_OCLK_DIV_TOG);
#else 
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_OCLK_DIV_SEL);//MT8223  "Div 2"
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_OCLK_DIV_TOG);
        //XBYTE[0xaa1] = (XBYTE[0xaa1] & ~0xC) | 0x4;
#endif

    }
    else //if (LVDS_OUTPUT_PORT == FOUR_PORT) 
    {
        u1FBdiv=2;//total VOPLL(Pre + FB divider) : 7x
        u1DddsCWMul =4;
        fgLVDSClkDiv2=TRUE;
        fgPixelClkMul2=TRUE;
#ifdef CC_MT8223
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 2, FLD_OCLK_DIV_SEL);//MT8223 "Div 4", doesn't support 4ch
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_OCLK_DIV_TOG);
#else 
        //vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 2, FLD_OCLK_DIV_SEL);//MT5387 doesn't have "Div 4"
#endif
    }

    if(_fgVopllUseDDDS)// use ddds
    {
        vIO32WriteFldAlign(CKGEN_AFECFG4, 1, FLD_RG_CKSEL_VP);//VPLL from DDDS
   // 1. setup ddds start -------------------------------------------------
        // rise ddds spread init, bit[31]
        vRegWriteFldAlign(OUTSTG_OS_E0, 1, OS_E0_SPREAD_INIT);
   
        // calculate control word
        u4Syspll = BSP_GetPLLClock(CAL_SRC_SYSPLL); 
        #ifdef CC_UP8032_ATV
          //Contorol Word setting Start
         // Display DClk caculate
         //MT8281 don't support UINT64 ???
        u4Syspll=u4Syspll/1000; //Hz --> KHz
        u4Freq_tmp = u4Freq/u1DddsCWMul;//for different port use the same CW???
        u4Freq_tmp = u4Freq_tmp/1000; //Hz --> KHz
        dTmp =(UINT32) ((((u4Syspll%u4Freq_tmp)<<14)/u4Freq_tmp)<<10);//calculate residue

        //control word setting
        //vDrvVoDDDSCW(dTmp); 
        //vDrvVoDDDSM((UINT8)(324000/u4Freq)); // Set DDDS CW(M-code)
        vRegWriteFldAlign(OUTSTG_OS_E2, dTmp, OS_E2_DDDS_FREQ_CW_N);
        vRegWriteFldAlign(OUTSTG_OS_E2, (UINT8)(u4Syspll/u4Freq_tmp), OS_E2_DDDS_FREQ_CW_M);

        //Contorol Word setting done  
        #else
        //u4Syspll = BSP_GetPLLClock(CAL_SRC_SYSPLL); //SYSPLL = 324MHz in MT5387
        u8Cw = u8Div6432(((UINT64)u4Syspll << VPLL_SPREAD_INT_SHIFT), u4Freq, NULL);
        u8Cw = (((UINT32)u8Cw*u1DddsCWMul) & VPLL_SPREAD_CW);
        vRegWriteFldAlign(OUTSTG_OS_E2, u8Cw, OS_E2_DDDS_FREQ_CW);
        #endif
        

        // enable spread spectrum
        vRegWriteFldAlign(OUTSTG_OS_E0, 0, OS_E0_SPREAD_INIT);
        // setup ddds done -----------------------------------------------------
    }
    else// use xtal
    {
    	 vIO32WriteFldAlign(CKGEN_AFECFG4, 0, FLD_RG_CKSEL_VP);//VPLL from Xtal
    }

    // 2. setup vpll start -----------------------------------------------------

    // Pre-divider and FB divider setting
   if(u1FBdiv <2) // FBdiv flag =1
    {
        //when single and dual port, total VOPLL = (1/2)*3.5*2 = 3.5
        //VOPLL output=LVDS CLK * 3.5
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 1, FLD_RG_VPLL_DIV12);//set DIV12=1
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_PDIV);//set pre-divider=0 --> /2
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_DIV);//set FB divider_2 = "0" --> 2X
   }
   else // FBdiv flag =2
    {
        //when four port, total VOPLL = (1/2)*3.5*4 = 7
        //VOPLL output=LVDS CLK * 7
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 1, FLD_RG_VPLL_DIV12);//set DIV12=1
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_PDIV);//set pre-divider=0 --> /2
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 2, FLD_RG_VPLL_DIV);//set FB divider_2 = "2" --> 4X
    }
    
    // LVDSClk/2 and PixelClk x2 setting
    vIO32WriteFldAlign(CKGEN_VOPLLCFG1, fgLVDSClkDiv2, FLD_RG_VPLL_VCODIV2_SEL);//set VCO CLK DIV2
    vIO32WriteFldAlign(CKGEN_VOPLLCFG0, fgPixelClkMul2, FLD_RG_VPLL_DPIX_X2);//set VOPLL DPIX X2
    vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_DPIX_DIV2);//set VOPLL DPIX DIV2=default value
    vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_DPIX_CKSEL);//set LVDS_DPIX=(1/2)*VCOCKN(P)

//New band setting (because of delay 104n long term eye jitter and clk jitter):
//setting band, selection band depended on output of VOPLL
    if(fgPixelClkMul2)//VOPLL output = pixel clk * 1.75
    {
         if (u4CurrentPClk > 320000000L)//320000000, (VCO Freq > 560000000L)
         {
               vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_BAND);
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_FS);
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_VCO_R);//Actually, it is the VCO_I
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_IFS); 
         }
         else if(u4CurrentPClk > 240000000L)//240000000, (VCO Freq > 420000000L)
         {
	             vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_BAND);
	             vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_FS);
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_VCO_R);//Actually, it is the VCO_I
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_IFS);  
         }
         else if(u4CurrentPClk > 171428571L)//171428571.43, (VCO Freq > 300000000L)
         {
	             vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 1, FLD_RG_VPLL_BAND);
	        	   vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_FS);
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_VCO_R);//Actually, it is the VCO_I
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_IFS);   
         }
         else if(u4CurrentPClk > 114285714L)//114285714.29, (VCO Freq > 200000000L)
         {
	             vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 2, FLD_RG_VPLL_BAND);
	        	   vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_FS);
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 2, FLD_RG_VPLL_VCO_R);//Actually, it is the VCO_I
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_IFS);   
         }
         else
         {
               vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 3, FLD_RG_VPLL_BAND); 
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_FS);
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 2, FLD_RG_VPLL_VCO_R);//Actually, it is the VCO_I
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_IFS); 
         }
     }    
    else//VOPLL output = pixel clk * 3.5
    {
    	  if (u4CurrentPClk > 160000000L)//160000000, (VCO Freq > 560000000L)
         {
               vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_BAND);
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_FS);
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_VCO_R);//Actually, it is the VCO_I
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_IFS); 
         }
         else if(u4CurrentPClk > 120000000L)//120000000, (VCO Freq > 420000000L)
         {
               vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_BAND);
	             vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_FS);
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_VCO_R);//Actually, it is the VCO_I
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_IFS);  
         }
         else if(u4CurrentPClk > 85714286L)//85714285.71, (VCO Freq >300000000L)
         {
	             vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 1, FLD_RG_VPLL_BAND);
	        	   vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_FS);
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_VCO_R);//Actually, it is the VCO_I
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_IFS); 
         }
         else if(u4CurrentPClk > 57142857L)//57142857.14, (VCO Freq > 200000000L)
         {
	             vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 2, FLD_RG_VPLL_BAND);
	        	   vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_FS);
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 2, FLD_RG_VPLL_VCO_R);//Actually, it is the VCO_I
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_IFS); 
         }
         else
         {
               vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 3, FLD_RG_VPLL_BAND); 
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_FS);
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 2, FLD_RG_VPLL_VCO_R);//Actually, it is the VCO_I
               vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_IFS); 
         }
    }


    if (!fgInit)
    {
        #if VOPLL_STRESS_TEST_XTAL
        vIO32WriteFldAlign(CKGEN_AFECFG4, 0, FLD_RG_CKSEL_VP);//VPLL from XTAL // nick123
        HAL_Delay_us(20);
        #endif
        //reset flow of 3p5 FB divider
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 1, FLD_RG_VPLL_PD);   
        HAL_Delay_us(20);        
        //if (Is_MT8223_E1())
        {
         vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 1, FLD_RG_VPLL_RESET_POSTDIV);
         HAL_Delay_us(20);   
        }                
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_FBDIV_3P5);
        HAL_Delay_us(20);
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 1, FLD_RG_FBDIV_3P5);        
        HAL_Delay_us(20);
        //if (Is_MT8223_E1())
        {
         vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_RESET_POSTDIV);
         HAL_Delay_us(20);   
        }        
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_PD);        
        HAL_Delay_us(20);
        
        // reset DDDS
        vRegWriteFldAlign(OUTSTG_OS_E0, 1, OS_E0_SPREAD_INIT);
        HAL_Delay_us(20);
        vRegWriteFldAlign(OUTSTG_OS_E0, 0, OS_E0_SPREAD_INIT);
        HAL_Delay_us(20);

        // Read counter        
        vIO32WriteFldAlign(CKGEN_PLLCALIB,0x09,FLD_DBGCKSEL);
        vIO32WriteFldAlign(CKGEN_PLLCALIB,0x00,FLD_CALI_MODE);         
        HAL_Delay_us(20);        
        vIO32WriteFldAlign(CKGEN_PLLCALIB,0x01,FLD_CAL_TRI);         
        // Wait calibration finishing.
        while (IO32ReadFldAlign(CKGEN_PLLCALIB, FLD_CAL_TRI)) { }        
        u2Oclk2=IO32ReadFldAlign(CKGEN_PLLCALIB,FLD_CAL_CNT);
        #ifdef VOPLL_Debug
        Printf("u2Oclk2 = 0x%x\n",u2Oclk2);
        #endif
        
        vIO32WriteFldAlign(CKGEN_PLLCALIB,0x0A,FLD_DBGCKSEL);
        vIO32WriteFldAlign(CKGEN_PLLCALIB,0x00,FLD_CALI_MODE); 
        HAL_Delay_us(20);        
        vIO32WriteFldAlign(CKGEN_PLLCALIB,0x01,FLD_CAL_TRI);
        // Wait calibration finishing.
        while (IO32ReadFldAlign(CKGEN_PLLCALIB, FLD_CAL_TRI)) { }              
        u2Oclk=IO32ReadFldAlign(CKGEN_PLLCALIB,FLD_CAL_CNT);
        #ifdef VOPLL_Debug        
        Printf("u2Oclk = 0x%x\n",u2Oclk);
        #endif

        #ifdef VOPLL_Debug  
        CW_Read=RegReadFldAlign(OUTSTG_OS_E2,OS_E2_DDDS_FREQ_CW);
        CWS_Read=RegReadFldAlign(OUTSTG_OS_E5,OS_E5_DDDS_FREQ_CW);
        #endif
      	
        //u2Oclk_target= ((u4Freq*2048)/1000)/(GET_XTAL_CLK()/1000000L);     

        if(IO32ReadFldAlign(CKGEN_PLLCALIB, FLD_CALI_MODE))
        {
   		 u4RefCount = 256;
   	    }
   	    else
        {
   		 u4RefCount = 2048;
   	    }
   	  
        #if VOPLL_STRESS_TEST_XTAL
        u2Oclk_target= 0x800;
        #else
        u2Oclk_target= (((u4Freq/1000)*u4RefCount)/1000)/(GET_XTAL_CLK()/1000000L);        
        #endif
        u2Oclk_targetH= u2Oclk_target + (u2Oclk_target*5/100);
        u2Oclk_targetL= u2Oclk_target - (u2Oclk_target*5/100);
       
        u2Oclk2_target= (u2Oclk_target*7)/u1DddsCWMul/4;
        u2Oclk2_targetH= u2Oclk2_target + (u2Oclk2_target*5/100);
        u2Oclk2_targetL= u2Oclk2_target - (u2Oclk2_target*5/100);     

        if(((u2Oclk2>=u2Oclk2_targetL)&&(u2Oclk2<=u2Oclk2_targetH))&&((u2Oclk>=u2Oclk_targetL)&&(u2Oclk<=u2Oclk_targetH)))
        {
          u1ResetCnt=0;
          //LVDS 7->4 FIFO enable           
          _Lvds7To4FifoReset = 2;       
          fgInit = TRUE;                   
          Printf("[SA7] VOPLL Reset OK\n");           
        }        
        else
        {
          u1ResetCnt--;        
          #ifdef VOPLL_Debug
           Printf("[2]CW_Read= 0x%x\n", CW_Read);
           Printf("[2]CWS_Read= 0x%x\n", CWS_Read);          
           Printf("[SA7]u2Oclk2= 0x%x\n",u2Oclk2);
           Printf("[SA7]u2Oclk =0x%x\n",u2Oclk);
           if(u2Oclk2==u2Oclk)
           {
            Printf("[SA7] VOPLL Reset Fail- OCLK no clock\n");          
           }
           else
           {
            Printf("[SA7] VOPLL Reset Fail- OCLK wrong clock \n");          
           }  
           if (u1ResetCnt==0)
           {       
            Printf("[SA7] PLL Initialize Fail !\n");
           }            
          #endif          
          Printf("[SA7] VOPLL Reset Fail count = %d\n",u1ResetCnt);
        }
            

        }
     }while(u1ResetCnt>0);    

    #if VOPLL_STRESS_TEST
     fgInit = FALSE; 
     }while(1);
    #endif  


/*
//setting band, selection band depended on output of VOPLL
    if(fgPixelClkMul2)//VOPLL output = pixel clk * 1.75
    {
         if (u4CurrentPClk > 240000000L)//240000000, (VCO Freq > 420000000L)
         {
               vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_BAND); 
         }
         else if(u4CurrentPClk > 182857142L)//182857142.86, (VCO Freq > 320000000L)
         {
	        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 1, FLD_RG_VPLL_BAND); 
         }
         else if(u4CurrentPClk > 114285714L)//114285714.29, (VCO Freq > 200000000L)
         {
	        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 2, FLD_RG_VPLL_BAND); 
         }
         else
         {
              vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 3, FLD_RG_VPLL_BAND); 
         }

         //set FS
         if(u4CurrentPClk > 114285714L)//114285714.29, (VCO Freq > 200000000L)
	  {
		vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_FS);
	  }
         else
	  {
		vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_FS);
	  }
    
         //set IFS
         if(u4CurrentPClk > 314285714L)//314285714.29, (VCO Freq > 550000000L)
	  {
		vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_IFS);
	  }
         else
	 {
		vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_IFS);
	 }
    }
    else//VOPLL output = pixel clk * 3.5
    {
    	  if (u4CurrentPClk > 120000000L)//120000000, (VCO Freq > 420000000L)
         {
               vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_BAND); 
         }
         else if(u4CurrentPClk > 91428571L)//91428571.43, (VCO Freq > 320000000L)
         {
	        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 1, FLD_RG_VPLL_BAND); 
         }
         else if(u4CurrentPClk > 57142857L)//57142857.14, (VCO Freq > 200000000L)
         {
	        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 2, FLD_RG_VPLL_BAND); 
         }
         else
         {
              vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 3, FLD_RG_VPLL_BAND); 
         }

         //set FS
         if(u4CurrentPClk > 57142857L)//57142857.14, (VCO Freq > 200000000L)
	  {
		vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_FS);
	  }
         else
	  {
		vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_FS);
	  }
    
         //set IFS
         if(u4CurrentPClk > 157142857L)//157142857.14, (VCO Freq > 550000000L)
	  {
		vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_IFS);
	  }
         else
	 {
		vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_IFS);
	 }
    }
//setting band --> need to check when and how to select band???
*/

//set spread spectrum
//need to check u4Freq when different port???
//vDrvVOSetSpreadSpectrum(u4Freq,  SS_PERCENT, SS_FREQ);

//copy from MT8281-------------------------------------------

}
#if 0
/**
 * @brief set ADCPLL frequency
 *
 * Set ADCPLL rough frequency
 * @param bADCCLK bADCCLK is the target frequency (MHz)
 * @retval void
 */
void vDrvADCPLLSetFreq(UINT8 bADCCLK)
{
    UINT32 u4Val;

    u4Val = bADCCLK;
    u4Val = u4Val * 1000000;
    CKGEN_SetADCPLLFreq(u4Val);
}

void CKGEN_SetVOPLLInputAsDDDS(BOOL fgDDDS)
{
    _fgVopllUseDDDS = fgDDDS;
    if (_u4CurrentVClk != 0)
    {
        vDrvVOPLLSet(_u4CurrentVClk);
    }

    if (BSP_GetIcVersion() < IC_VER_AB)
    {
        if (fgDDDS)
        {
            // power down sdds
            IO_WRITE32MSK(VIDEO_IN0_BASE, 0x490, 0, 1 << 30);
        }
        else
        {
            // power on sdds
            IO_WRITE32MSK(VIDEO_IN0_BASE, 0x490, 1 << 30, 1 << 30);
        }
    }
}

void CKGEN_SetVOPLLFreq(UINT32 u4Freq)
{
    vDrvVOPLLSet(u4Freq);
}

void CKGEN_SetADCPLLFreq(UINT32 u4Freq)
{
#if 0 //mt5360 pll change
    UINT32 u4Val, u4Ns, u4Rev, u4Syspll;
    UINT64 u8Cw;
    IC_VERSION_T eVer;

    eVer = BSP_GetIcVersion();

    if (eVer <= IC_VER_AB) {
        u4Ns = 2;   // 5 is recommend for 162MHz, 2 for 81MHz. Xtal.
        u4Rev = 1;
    } else {
        u4Rev = 1;
        if (u4Freq < CLK_50MHZ) {
            u4Ns = 1;   /* CW from 6 ~ 12 */
        } else if (u4Freq < CLK_81MHZ) {
            u4Ns = 2;   /* CW from 6 ~ 12 */
        } else {
            u4Ns = 5;   /* CW from 6 ~ 12 */
        }
    }
    u4Syspll = BSP_GetPLLClock(CAL_SRC_SYSPLL);
    u8Cw = (UINT64)u4Syspll;
    u8Cw = ((u8Cw * ((UINT64)u4Ns + 1)) / ((UINT64)u4Rev + 1));
    u4Val = ((UINT32)((u8Cw << ADCPLL_FREQ_INT_SHIFT) / u4Freq)) & ADCPLL_FREQ_CW;
    IO_WRITE32(VIDEO_IN0_BASE, REG_RW_ADCPLL_DCLK_FREQ_CTRL, u4Val);

    u4Val = CKGEN_READ32(REG_ADCPLL_CFG1);
    u4Val &= ~(ADC_NS | ADC_REV | VCO_BAND_ADC);
    u4Val |= ((u4Ns << ADC_NS_SHIFT) | (u4Rev << ADC_REV_SHIFT)
                | ADC_EN_SEFBIA);
    CKGEN_WRITE32(REG_ADCPLL_CFG1, u4Val);

    u4Val = CKGEN_READ32(REG_ADCPLL_CFG0);
    u4Val &= ~(VCO_GAIN_ADC | V2I_RB_ADC | DIV_SEL_ADC);
    u4Val |= ((2 << V2I_RB_ADC_SHIFT) | (1 << DIV_SEL_ADC_SHIFT));

    if (u4Freq < CLK_30MHZ)   // 10MHz~50MHz
    {
        u4Val |= (1 << VCO_GAIN_ADC_SHIFT);
    }
    else if (u4Freq < CLK_100MHZ) // 20MHz~140MHz
    {
        u4Val |= (3 << VCO_GAIN_ADC_SHIFT);
    }
    else    // 50MHz~200MHz
    {
        u4Val |= (5 << VCO_GAIN_ADC_SHIFT);
    }
    CKGEN_WRITE32(REG_ADCPLL_CFG0, u4Val);
#endif
}

void CKGEN_SetPSPLLFreq(UINT32 u4Freq)
{
#if 0
    UINT32 u4Val, u4Nco, u4Syspll;
    UINT64 u8Cw;
    IC_VERSION_T eVer;

    eVer = BSP_GetIcVersion();
    // To enable/initialize PScan NCO clock
    u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_PSPLL_NCO_FREQ_CTRL);
    u4Val &= ~(PS_NCO_CW);
    u4Val |= PS_NCO_INIT;
    IO_WRITE32(VIDEO_IN0_BASE, REG_RW_PSPLL_NCO_FREQ_CTRL, u4Val);

    // PSPLL_CLK = ((NCO_CLK x 4)/(DIV_SEL_PS)) * (1/2)
    u4Val = CKGEN_READ32(REG_PSPLL_CFG0);
    u4Val &= ~(V2I_RB_PS | VCO_GAIN_PS | CKSEL_0_PS |
                CKSEL_1_PS | PD_PSPLL | DIV_SEL_PS);
    u4Val |= ((1 << V2I_RB_PS_SHIFT) | (5 << VCO_GAIN_PS_SHIFT));
    u4Val |= (CKSEL_0_PS);

    // Must the same as u4Freq shifter
    // Set DIV_SEL_PS is 1, 2, or 4.
    u4Val |= ((u4Freq > CLK_54MHZ) ? (PSPLL_DIV_1 << DIV_SEL_PS_SHIFT) :
                ((u4Freq > CLK_27MHZ) ? (PSPLL_DIV_2 << DIV_SEL_PS_SHIFT) :
                                        (PSPLL_DIV_4 << DIV_SEL_PS_SHIFT)));
    CKGEN_WRITE32(REG_PSPLL_CFG0, u4Val);

    u4Val = CKGEN_READ32(REG_PSPLL_CFG1);
    u4Val &= ~(PS_REV | VCO_BAND_PS | CHP_PS);

    if (eVer <= IC_VER_AB) {
        // 2U to open NCO
        u4Val |= ((2U << PS_REV_SHIFT) | (1U << CHP_PS_SHIFT));
    } else {
        // 0x62 to open NCO, to set All PLLs related to NCO works.
        u4Val |= ((0x62U << PS_REV_SHIFT) | (1U << CHP_PS_SHIFT));
    }
    CKGEN_WRITE32(REG_PSPLL_CFG1, u4Val);

    // Must the same as PSPLL_DIV_#
    // u4Nco = u4Freq * DIV_SEL_PS / 2;
    u4Nco = ((u4Freq > CLK_54MHZ) ? (u4Freq >> 1) :
            ((u4Freq > CLK_27MHZ) ? (u4Freq) :
                                    (u4Freq << 1)));

    u4Syspll = BSP_GetPLLClock(CAL_SRC_SYSPLL);
    // (NCO clock)= syspll/[27:24].[23:0], for example, 148.5MHz => 0x45d1745
    u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_PSPLL_NCO_FREQ_CTRL);
    u4Val &= ~(PS_NCO_CW);
    u8Cw = u4Syspll;
    u8Cw = (u8Cw << PS_NCO_INT_SHIFT);
    u8Cw = (u8Cw / u4Nco);
    u4Val |= ((UINT32)u8Cw & PS_NCO_CW);
    IO_WRITE32(VIDEO_IN0_BASE, REG_RW_PSPLL_NCO_FREQ_CTRL, u4Val);
#endif
}

void CKGEN_SetB2RPLLFreq(UINT32 u4Freq)
{
#if 0
    UINT32 u4Val, u4Nco, u4Syspll;
    UINT64 u8Cw;

    // To enable/initialize B2R NCO clock
    u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_B2RPLL_NCO_FREQ_CTRL);
    u4Val &= ~(B2R_NCO_CW);
    u4Val |= B2R_NCO_INIT;
    IO_WRITE32(VIDEO_IN0_BASE, REG_RW_B2RPLL_NCO_FREQ_CTRL, u4Val);

    // B2RPLL = NCO_CLK x 4
    u4Val = CKGEN_READ32(REG_B2RPLL_CFG0);
    u4Val &= ~(PD_B2RPLL);
    u4Val |= (CKSEL_B2R | (1U << 25));  // XXX why bit 25?
    CKGEN_WRITE32(REG_B2RPLL_CFG0, u4Val);

    // u4Nco = u4Freq / 4;
    u4Nco = (u4Freq >> 2);
    u4Syspll = BSP_GetPLLClock(CAL_SRC_SYSPLL);
    // (NCO clock)= syspll/[27:24].[23:0], for example, 148.352 MHz => 0x8bc6ace
    u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_B2RPLL_NCO_FREQ_CTRL);
    u4Val &= ~(B2R_NCO_CW);
    u8Cw = u4Syspll;
    u8Cw = (u8Cw << B2R_NCO_INT_SHIFT);
    u8Cw = ((u8Cw + (u4Nco >> 1)) / u4Nco);

    if (u4Freq == B2RPLL_DEFAULT_CLOCK)
    {
        u4Val |= 0x8bc6ace; // 148.352 MHz, why? 0x8bc6924 is more close, XXX
    }
    else
    {
        u4Val |= ((UINT32)u8Cw & B2R_NCO_CW);
    }
    IO_WRITE32(VIDEO_IN0_BASE, REG_RW_B2RPLL_NCO_FREQ_CTRL, u4Val);
#else
    UNUSED(u4Freq);
#endif
}
#endif

