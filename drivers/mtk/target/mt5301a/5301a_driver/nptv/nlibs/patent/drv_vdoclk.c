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
#include "drv_autocolor.h"
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
#include "hw_hdtv.h"
//#include "x_pdwnc.h"
#include "x_typedef.h"
#include "x_util.h"
#include "nptv_debug.h"

//#include "hw_ckgen.h"

#include"x_hal_5381.h"
#include "hw_pdwnc.h"
#include "drv_async.h"
#include "drv_auto.h"
#include "nptv_drvif.h"
#if VGAADCPLL_WORKAROUND
#include "vdo_misc.h"               //bDrvVideoGetTiming()
#endif  //VGAADCPLL_WORKAROUND
LINT_EXT_HEADER_END


#define SS_FREQ 30L      //kHz

// ###8202### - The followings are left to be redefined.
// REG_CAL_SEL 0x03

#define ADCPLL_DEBUG 0
#define DVIPLL_DEBUG 0
#define SYSPLL_DEBUG 0
#define CLK_5371 1

#define SYS_CLK_OLD		1
#if SYS_CLK_OLD
#define SYSPLL 3240L
#else
#define SYSPLL 4320L
#endif

#define SYSPLL432 4320L
#define SYSPLL324 3240L

enum
{
    VO_PLL_CLKOUT,
    VO_PLL_XTAL
};

#define PIX_CLK_INV_Criterion 750 // avoid the the Flash noise by Data and CLK Delay mismatch, MT8223 min latency : 3.35 ns
#define CLKIN_LOCK_LINE_CNT_VAL 0x02
#define CLKIN_LOCK_THR_VAL 0x02
#define CLKIN_HSYNC_WIDTH 32
UINT32 _u4VPllFrequency = 0;
static UINT32 _u4Permillage = 0;
static UINT32 _u4Freq = 0;//for modulation rate

UINT8   _bTHERMAL;
UINT8 	_bTHERMAL_TEMP;
UINT16  _bTHERMAL_CNT;
UINT8   _bTHERMALCOM_STATE;



UINT8   _bCLKByte3;
UINT8   _bCLKByte2;
UINT8   _bCLKByte1;
UINT8   _bCLKSetFlag;
UINT8   VOPLL_PARAMETER[6];

UINT8   _bGetCLKByte3;
UINT8   _bGetCLKByte2;
UINT8   _bGetCLKByte1;
UINT8   _bGetCLKByte0;

UINT8   _bOSD1, _bOSD2, _bOSD3 ;
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




void vDrvClkInit(void)  // ToDo - vDrvVideoInit
{
    //vDrvTVADCPLLIni();
    vDrvCLKINDclkRST(0);
    vDrvCLKINDclkRST(1);           //bit 30         : C_DCLK_RST = 1 means to reset DCLK loop
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


#if Phase3Channel
    vIO32WriteFldAlign(CKGEN_AFECFG7, 0x1, FLD_RG_ADCPLL_CKO_SEL);
    vIO32WriteFldAlign(CKGEN_AFECFG7, 0x1,  FLD_RG_ADCPLL_PH_INTPERPOLATOR_PWDN );
#else
    vIO32WriteFldAlign(CKGEN_AFECFG7, 0x3, FLD_RG_ADCPLL_CKO_SEL);
    vIO32WriteFldAlign(CKGEN_AFECFG7, 0x0, FLD_RG_ADCPLL_PH_INTPERPOLATOR_PWDN );
#endif
    vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_ENABLE);
    vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_ENABLE_FAK);

    vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_PD);
    vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_ILVDS_PD);
    vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_BG_PD);
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

    vRegWriteFldAlign(VFE_17, bPow, DCLK_RESETB);
    vIO32WriteFldAlign(CKGEN_AFECFG6, bPow, FLD_RG_ADCPLL_ENABLE);
    vIO32WriteFldAlign(CKGEN_AFECFG6, bPow, FLD_RG_ADCPLL_ENABLE_FAK);
    if (bPow == SV_TRUE)//Power on
    {
        vIO32WriteFldAlign(CKGEN_AFECFG7, 3, FLD_RG_ADCPLL_VCOG_SEL);
    }
    else if (bPow == SV_FALSE)//Power down
    {
        vIO32WriteFldAlign(CKGEN_AFECFG7, 0, FLD_RG_ADCPLL_VCOG_SEL);
    }
}


#if VGAADCPLL_WORKAROUND
BOOL bDrvADCheckSpecialClk(UINT16 pixclk)
{
    BOOL ret = FALSE ;

    if ((pixclk <= 800) && (pixclk > 350))
    {
        ret = TRUE;
    }

    return ret;
}
#endif  //VGAADCPLL_WORKAROUND


#if CHANGE_SDDS_KPI  //michael 20090123 ***
BOOL _KPIReset;
UINT8 _sdds_count;

void vDrvEnableChang_SDDS_BW(void)
{
    _KPIReset = TRUE;
    _sdds_count = 0;
}

void Set_SDDS_KPI(UINT8 bValue)
{
    if (fgIsMainYPbPr() || fgIsPipYPbPr() || fgIsMainVga() || fgIsPipVga())
    {
    if (bValue == 0)
    {
        if (_KPIReset)
        {
            _sdds_count++;
            if (_sdds_count > 5)
            {
                vDrvCLKINSetKI0(1);			  //bit 21~18  : C_DCLK1_KI0
                vDrvCLKINSetKP0(3);			  //bit 13~10  : C_DCLK1_KP0
                _KPIReset = 0;
                _sdds_count = 0;
            }
        }
    }
    else
    {
        vDrvCLKINSetKI0(8);			   //bit 21~18	: C_DCLK1_KI0
        vDrvCLKINSetKP0(8);			   //bit 13~10	: C_DCLK1_KP0
        _KPIReset = 0;
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
          vDrvCLKINSetKP0(2);              //bit 13~10  : C_DCLK1_KP0
      }
     else
     {		
          vDrvCLKINSetKI0(8);              //bit 21~18  : C_DCLK1_KI0
          vDrvCLKINSetKP0(8);              //bit 13~10  : C_DCLK1_KP0
      }
}
#endif

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
    UINT8 bVCOGain;
    UINT8 bADCPLL_NS;

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
    if (wPixClk >= PIX_CLK_INV_Criterion)
    {
        vIO32WriteFldAlign(CKGEN_RGBCLK_CFG, 0, FLD_C_PIX_CLK_INV);
    }
    else
    {
        vIO32WriteFldAlign(CKGEN_RGBCLK_CFG, 1, FLD_C_PIX_CLK_INV);
    }

    //SC hwu suggest , Michael Modify 20091125
    if (wPixClk>= 1300)
    {
        vPGA_LPF_BW(4);
    }
    else if (wPixClk>= 900)
    {
        vPGA_LPF_BW(3);
    }
    else if (wPixClk>= 550)
    {
        vPGA_LPF_BW(2);
    }
    else
    {
        vPGA_LPF_BW(1);
    }

    //Tomson For MT5381 only ; By Freq set VCO gain
    /*
        if(wPixClk<=260)
        {
    	    bVCOGain = 1 ;
               vIO32WriteFldAlign(CKGEN_AFECFG7, 1, FLD_RG_ADCPLL_EN_SWC);
               vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_VTI_RB);
        }
        else if((wPixClk<=400) && (wPixClk>260))
        {
    	    bVCOGain = 2;
               vIO32WriteFldAlign(CKGEN_AFECFG7, 1, FLD_RG_ADCPLL_EN_SWC);
               vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_VTI_RB);
        }
        else if((wPixClk<=500) && (wPixClk>400))
        {
    	    bVCOGain =  1 ;
               vIO32WriteFldAlign(CKGEN_AFECFG7, 0, FLD_RG_ADCPLL_EN_SWC);
               vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_VTI_RB);
        }
        else if((wPixClk<=700) && (wPixClk>500))
        {
    #if VGAADCPLL_WORKAROUND
            if( bDrvADCheckSpecialClk(wPixClk) && _ApplyVgaADCPLLWorkAround )
            {
                if(IO32ReadFldAlign(CKGEN_AFECFG7, FLD_RG_ADCPLL_VCOG_SEL)  == 2)
                {
                    bVCOGain = 1;
                }
                else
                {
                    bVCOGain = 2;
                }
                _ApplyVgaADCPLLWorkAround = FALSE;
            }
            else
    #endif  //VGAADCPLL_WORKAROUND
    	       bVCOGain =  2 ;
            vIO32WriteFldAlign(CKGEN_AFECFG7, 0, FLD_RG_ADCPLL_EN_SWC);
            vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_VTI_RB);
        }
        else if((wPixClk<=1080) && (wPixClk>700))
        {
    #if VGAADCPLL_WORKAROUND
            if( bDrvADCheckSpecialClk(wPixClk) && _ApplyVgaADCPLLWorkAround )
            {
                if(IO32ReadFldAlign(CKGEN_AFECFG7, FLD_RG_ADCPLL_VCOG_SEL) == 2)
                {
                    bVCOGain = 1;
                }
                else
                {
                    bVCOGain = 2;
                }
                _ApplyVgaADCPLLWorkAround = FALSE;
            }
            else
    #endif  //VGAADCPLL_WORKAROUND
    	       bVCOGain = 3 ;
            vIO32WriteFldAlign(CKGEN_AFECFG7, 0, FLD_RG_ADCPLL_EN_SWC);
            vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_VTI_RB);
        }
        else if((wPixClk<=1260) && (wPixClk>1080))
        {
    	    bVCOGain = 7 ;
               vIO32WriteFldAlign(CKGEN_AFECFG7, 0, FLD_RG_ADCPLL_EN_SWC);
               vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_VTI_RB);
        }
        else
        {
    	    bVCOGain = 7 ;
               vIO32WriteFldAlign(CKGEN_AFECFG7, 0, FLD_RG_ADCPLL_EN_SWC);
               vIO32WriteFldAlign(CKGEN_AFECFG6, 3, FLD_RG_ADCPLL_VTI_RB);
        }
        vIO32WriteFldAlign(CKGEN_AFECFG7, bVCOGain, FLD_RG_ADCPLL_VCOG_SEL);
    */
//new band selection
    if (wPixClk <= 260)
    {
        bVCOGain = 1 ;
        vIO32WriteFldAlign(CKGEN_AFECFG7, 1, FLD_RG_ADCPLL_EN_SWC);
        vIO32WriteFldAlign(CKGEN_AFECFG6, 0, FLD_RG_ADCPLL_VTI_RB);
    }
    else if ((wPixClk <= 370) && (wPixClk > 260))
    {
        bVCOGain = 1;
        vIO32WriteFldAlign(CKGEN_AFECFG7, 1, FLD_RG_ADCPLL_EN_SWC);
        vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_VTI_RB);
    }
    else if ((wPixClk <= 480) && (wPixClk > 370))
    {
        bVCOGain =  1 ;
        vIO32WriteFldAlign(CKGEN_AFECFG7, 1, FLD_RG_ADCPLL_EN_SWC);
        vIO32WriteFldAlign(CKGEN_AFECFG6, 3, FLD_RG_ADCPLL_VTI_RB);
    }
    else if ((wPixClk <= 600) && (wPixClk > 480))
    {
#if VGAADCPLL_WORKAROUND
        if ( bDrvADCheckSpecialClk(wPixClk) && _ApplyVgaADCPLLWorkAround )
        {
            if (IO32ReadFldAlign(CKGEN_AFECFG7, FLD_RG_ADCPLL_VCOG_SEL)  == 2)
            {
                bVCOGain = 1;
            }
            else
            {
                bVCOGain = 2;
            }
            _ApplyVgaADCPLLWorkAround = FALSE;
        }
        else
#endif  //VGAADCPLL_WORKAROUND
            bVCOGain =  1 ;
        vIO32WriteFldAlign(CKGEN_AFECFG7, 0, FLD_RG_ADCPLL_EN_SWC);
        vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_VTI_RB);
    }
    else if ((wPixClk <= 700) && (wPixClk > 600))
    {
#if VGAADCPLL_WORKAROUND
        if ( bDrvADCheckSpecialClk(wPixClk) && _ApplyVgaADCPLLWorkAround )
        {
            if (IO32ReadFldAlign(CKGEN_AFECFG7, FLD_RG_ADCPLL_VCOG_SEL) == 2)
            {
                bVCOGain = 1;
            }
            else
            {
                bVCOGain = 2;
            }
            _ApplyVgaADCPLLWorkAround = FALSE;
        }
        else
#endif  //VGAADCPLL_WORKAROUND
            bVCOGain = 1 ;
        vIO32WriteFldAlign(CKGEN_AFECFG7, 0, FLD_RG_ADCPLL_EN_SWC);
        vIO32WriteFldAlign(CKGEN_AFECFG6, 3, FLD_RG_ADCPLL_VTI_RB);
    }
    else if ((wPixClk <= 960) && (wPixClk > 700))
    {
        bVCOGain = 2 ;
        vIO32WriteFldAlign(CKGEN_AFECFG7, 0, FLD_RG_ADCPLL_EN_SWC);
        vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_VTI_RB);
    }
    else if ((wPixClk <= 1220) && (wPixClk > 960))
    {
        bVCOGain = 3 ;
        vIO32WriteFldAlign(CKGEN_AFECFG7, 0, FLD_RG_ADCPLL_EN_SWC);
        vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_VTI_RB);
    }
    else if ((wPixClk <= 1490) && (wPixClk > 1220))
    {
        bVCOGain = 6 ;
        vIO32WriteFldAlign(CKGEN_AFECFG7, 0, FLD_RG_ADCPLL_EN_SWC);
        vIO32WriteFldAlign(CKGEN_AFECFG6, 1, FLD_RG_ADCPLL_VTI_RB);
    }
    else//>1490
    {
        bVCOGain = 6 ;
        vIO32WriteFldAlign(CKGEN_AFECFG7, 0, FLD_RG_ADCPLL_EN_SWC);
        vIO32WriteFldAlign(CKGEN_AFECFG6, 3, FLD_RG_ADCPLL_VTI_RB);
    }
    vIO32WriteFldAlign(CKGEN_AFECFG7, bVCOGain, FLD_RG_ADCPLL_VCOG_SEL);



    //interpolation current and loading control
    if (wPixClk < 500)
    {
        vIO32WriteFldAlign(CKGEN_AFECFG17, 2, FLD_RG_APLL2_REV_5_4);
        vIO32WriteFldAlign(CKGEN_AFECFG17, 3, FLD_RG_APLL2_REV_3_2);
    }
    else if ((wPixClk < 750) && (wPixClk >= 500))
    {
        vIO32WriteFldAlign(CKGEN_AFECFG17, 2, FLD_RG_APLL2_REV_5_4);
        vIO32WriteFldAlign(CKGEN_AFECFG17, 2, FLD_RG_APLL2_REV_3_2);
    }
    else if ((wPixClk < 1000) && (wPixClk >= 750))
    {
        vIO32WriteFldAlign(CKGEN_AFECFG17, 1, FLD_RG_APLL2_REV_5_4);
        vIO32WriteFldAlign(CKGEN_AFECFG17, 2, FLD_RG_APLL2_REV_3_2);
    }
    else if ((wPixClk < 1500) && (wPixClk >= 1000))
    {
        vIO32WriteFldAlign(CKGEN_AFECFG17, 0, FLD_RG_APLL2_REV_5_4);
        vIO32WriteFldAlign(CKGEN_AFECFG17, 1, FLD_RG_APLL2_REV_3_2);
    }
    else //f >= 1500
    {
        vIO32WriteFldAlign(CKGEN_AFECFG17, 0, FLD_RG_APLL2_REV_5_4);
        vIO32WriteFldAlign(CKGEN_AFECFG17, 0, FLD_RG_APLL2_REV_3_2);
    }

    //SDDS Setting			//Let SDclk CW range:    324/6 ~ 324/14

    /*
        #if SYS_CLK_OLD
        if(wPixClk<500)			//(SDclk CW)/1, ADCPLLx1
        #else
        if(wPixClk<600)			//(SDclk CW)/1, ADCPLLx1
        #endif
        {
    			bTmp = 0;
          if (wPixClk<16)
          {
          	wPixClk=16;
          }
        }
        #if SYS_CLK_OLD
        else if((wPixClk<1000) && (wPixClk>=500))	//(SDclk CW)/2, ADCPLLx2
        #else
        else if((wPixClk<1000) && (wPixClk>=600))	//(SDclk CW)/2, ADCPLLx2
        #endif
        {
    			bTmp = 1;
          wPixClk = wPixClk >> 1; // Pixel clock divider by 2
        }
        else	// pixel >1000 (SDclk CW)/3, ADCPLLx3
        {
    			bTmp = 2;
          wPixClk = (UINT16)(wPixClk / 3);// Pixel clock divider by 3
        }
    */
    //Do something for MT5360A
    if (wPixClk < 500)			//(SDclk CW)/1, ADCPLLx1
    {
        bADCPLL_NS = 0;
        if (wPixClk < 16)
        {
            wPixClk = 16;
        }
    }
    else if ((wPixClk < 1000) && (wPixClk >= 500))	//(SDclk CW)/2, ADCPLLx2
    {
        bADCPLL_NS = 1;
        wPixClk = wPixClk >> 1; // Pixel clock divider by 2
    }
    else	// pixel >1000 (SDclk CW)/3, ADCPLLx3
    {
        bADCPLL_NS = 2;
        wPixClk = (UINT16)(wPixClk / 3);// Pixel clock divider by 3
    }
    vIO32WriteFldAlign(CKGEN_AFECFG7, 2, FLD_RG_ADCPLL_SDDS_FBK_SEL);
    vIO32WriteFldAlign(CKGEN_AFECFG6, bADCPLL_NS, FLD_RG_ADCPLL_NS);

    bMcode = (UINT8) (SYSPLL324 / wPixClk);  //cal the M code
    dTmp = SYSPLL324 - ((UINT32) wPixClk * (UINT32)bMcode); //calculate residue

    dTmp = (dTmp << 16) / wPixClk;  //calculate 24 bit residue, only calculate CW2,1 , CW0 don't care
    //if <<24 will cause (residue ^24 overflow) , so <<16 is enough
    dTmp = ((UINT32)bMcode << 24) | ((UINT32)dTmp << 8) ;


    vDrvCLKINFreeRun(ENABLE); //SDDS freerun bit[31] set "1"

    /////// move to vDrvClkInit

    vRegWrite4B(VFE_15, dTmp);            //SDDS CW set

    vDrvCLKINDCLKFast(ENABLE); // Enable for avoid suddenly large errror //Walter suggest disable, Tomson Ena /Dis DCLK Fast lock Loop

    vDrvCLKINFreeRun(DISABLE);// vWriteVDOINMsk(VFE_14,0x00,0x80,3); SDDS lock 20022490[31] set "0"
    vUtDelay2us(200);

    _bCLKSetFlag |= 0x04;

#if CHANGE_SDDS_KPI            //michael 20090123 ***
    vDrvEnableChang_SDDS_BW();
#endif

    _RETIMENeedReset = TRUE;                                //Reset RETIME in VGA ISR
}



/**
 * @brief CLKIN default setting
 *
 * Set CLKIN loop filter default setting
 * @retval void
 */
#if 0
void vDrvCLKINDefaultSet(void)
{
    vDrvCLKINSetLockThr(2); // set Thrhold = 10
    vDrvCLKINSetKI0(1); // set ki0=1
    vDrvCLKINSetKI1(1); // set ki1 = 1
    //vWriteVDOIN(VFE_14, 2, 0x85);  //lock thr = 2, ki0=1, ki1 = 1
    vDrvCLKINSetLockCnt(2); // lock cnt = 2
    vDrvCLKINSetKP0(5);// kp0 = 5
    vDrvCLKINSetKP1(1);// kp1 = 1
    //vWriteVDOIN(VFE_14, 1, 0x95);  //lock cnt = 2, kp0 = 5, kp1 = 1
}
#endif


/**
 * @brief The second CLKIN default setting
 *
 * Set CLKIN loop filter default setting in the second region
 * @retval void
 */
#if !REMOVE_UNCALLED_FUNCTION
void vDrvCLKIN2DefaultSet(void)
{

    vDrvCLKINSetSecKI0(1); // set ki0=1
    vDrvCLKINSetSecKI1(1); // set ki1 = 1

    vDrvCLKINSetSecKP0(5);// kp0 = 5
    vDrvCLKINSetSecKP1(1);// kp1 = 1

}
#endif
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
    vDrvVOPLLSet(dwDCLK);
    // Spread Spectrum Setting
    //#if (WFB_LOCK_ENABLE == WFB_MODE_ON)
    //vDrvSetVOSpread(dwDCLK,  SS_PERCENT, SS_FREQ);

    vDrvSetVOSpread(DRVCUST_OptGet(eSpreadSpectrumPermillage));

    //#endif
}

/**
 * @brief Set modulation rate of SS the same as input Hsync
 *
 *  @retval void
 * FW almost done, but LVDS setting need to check
 */
void vDrvSetSSMDRAsInputHsync(void)//for dynamic MDR
{
  #if 0
     UINT32 dwGetVTotal = 0;
     UINT32 dwGetFrameRate = 0;
     UINT32 dwInHsync;
     UINT32 u4LockOthers;
     UINT32 u4FrameRate;
     UINT32 u4Freq;
     UINT32 u4Permillage;

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

	#endif
}

/**
 * @brief Display Digital clock , spread spectrum setting
 *
 * @param dwDCLK is target frequency(Hz) of display digital clock
 * @retval void
 * FW almost done, but LVDS setting need to check
 */
#if !REMOVE_UNCALLED_FUNCTION
void vDrvSetVoSpreSTEP(UINT32 bVal)
{
    vDrvVoDclkSpreSpecSTEP(bVal);
}
#endif
//for slicer CLI
#if !REMOVE_UNCALLED_FUNCTION
void vDrvSlicerCheck(void)
{
    UINT8 i;
    UINT8 Lock;
    UINT8 VHSEL;
    UINT8 VLSEL;

    for (VHSEL = 0; VHSEL <= 15;VHSEL++)
    {
        for (VLSEL = 0; VLSEL <= 15;VLSEL++)
        {
            Lock = 0;
            vIO32WriteFldAlign(SYNCFG1, VHSEL, FLD_RG_SOGY_VHSEL);
            vIO32WriteFldAlign(SYNCFG1, VLSEL, FLD_RG_SOGY_VLSEL);
            vUtDelay1ms(1);
            for (i = 0; i <= 127;i++)
            {
                Lock += fgIsCLKLock();
                vUtDelay1ms(1);		//1080p HSYNC 15uS fastest
            }
        }
    }
    vIO32WriteFldAlign(SYNCFG1, 0x08, FLD_RG_SOGY_VHSEL);
    vIO32WriteFldAlign(SYNCFG1, 0x08, FLD_RG_SOGY_VLSEL);
}
#endif
#if EnableSDDSCalibration
UINT16 _wSDDS_delay_counter;
UINT8 _bSDDSCaliState;
UINT8 _bKI0, _bKP0;
UINT32 _bVFE14_backup;
UINT8 _bKP0_MAX, _bKI0_MAX, _bKP0_init; //,_bKI0_init;
enum
{
    SDDS_CALI_NOTHING,
    SDDS_CALI_START,
    SDDS_CALI_WAIT,
    SDDS_CALI_END
};

#define KI0_MAXVALUE 0x0b
#define KP0_MAXVALUE 0x0b
#if ENABLE_IC_TEST_MODULE
#define MEM_SIZE 0x300//slt_yl
#else
#define MEM_SIZE 0x1000
#endif
#define REPEAT_NUM  8
#endif
void SDDS_wait_message(void)
{
#if EnableSDDSCalibration
    if  (_bSDDSCaliState == SDDS_CALI_WAIT)
    {
        _wSDDS_delay_counter = _wSDDS_delay_counter + 1;
    }
#endif
}

#if EnableSDDSCalibration
vDrvEnable_SDDS_Calibration(UINT8 bKP0_START, UINT8 bKP0_END, UINT8 bKI0_START, UINT8 bKI0_END)
{
    if (_bSDDSCaliState == SDDS_CALI_NOTHING)
    {
        _bVFE14_backup = u4RegRead4B(VFE_14);  //backup VFE_14
        _bSDDSCaliState = SDDS_CALI_START;
        if (bKP0_END > KP0_MAXVALUE)
        {
            bKP0_END = KP0_MAXVALUE;
        }
        _bKP0_MAX = bKP0_END;

        if (bKI0_END > KI0_MAXVALUE)
        {
            bKI0_END = KI0_MAXVALUE;
        }
        _bKI0_MAX = bKI0_END;

        if (bKP0_START > bKP0_END)
        {
            bKP0_START = bKP0_END;
        }

        if (bKI0_START > bKI0_END)
        {
            bKI0_START = bKI0_END;
        }

        _bKP0 = bKP0_START;
        _bKP0_init = bKP0_START;
        _bKI0 = bKI0_START;
        //_bKI0_init = bKI0_START;
    }
}
#endif

void vDrvSDDS_Response(void)
{
#if EnableSDDSCalibration
    UINT32 CW_INIT, CW_backup;
    #if ENABLE_IC_TEST_MODULE
    UINT32 CW[MEM_SIZE];    //slt_yl
    #else
    UINT32 *CW;
    #endif
    UINT16 SDDS_ERR;
    //INT16 ERR;
    UINT16 i;
    UINT16 j;
    UINT32 dwTmp; //,dwTotalCount;
    UINT32 CW_old = 0;
    UINT16 repeat_counter, wRepeatimes;
    UINT8 k, bTmpCount;//repeat_counter[REPEAT_NUM];
    UINT8 mode;

    switch (_bSDDSCaliState)
    {
        case SDDS_CALI_NOTHING:
            break;

        case SDDS_CALI_START:
            k = 0;
            bTmpCount = 0;
            wRepeatimes = 0;
            repeat_counter = 0;

            #if !ENABLE_IC_TEST_MODULE
            CW = (UINT32 *)x_mem_alloc(MEM_SIZE); //slt_yl
            #endif
            dwTmp = u4RegRead4B(0x240c);
            vRegWrite4B(0x240c, 0xffffffff);   //disable all interrupt
            _wSDDS_delay_counter = 0;
            vRegWriteFldAlign(VFE_14, _bKP0, DCLK1_KP_0);
            vRegWriteFldAlign(VFE_14, _bKI0, DCLK1_KI_0);
            vDrvCLKINFreeRun(ENABLE);

            //vRegWriteFldAlign(VFE_14, 1, C_DCLK_INIT);
            //for (j=0; j<7; j++)
            //{
            //   dwTmp2 = u4RegRead4B(VFE_14+(j<<2));
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

            for (i = 0; i < (MEM_SIZE / 4);i++)
            {
                for (j = 0; j < 20; j++)
                {
                    SDDS_ERR = u4RegRead4B(VFE_STA_00);
                    SDDS_ERR = SDDS_ERR & 0x00000fff;
                }

                if (repeat_counter > 0)
                {
                    bTmpCount = bTmpCount + 1;
                    if (bTmpCount >= (repeat_counter - 1))              //repeat_counter[REPEAT_NUM-1]) //store the CW every repeat_counter[REPEAT_NUM-1] times
                    {
                        CW[i]  = (u4RegRead4B(VFE_STA_01) & 0x0fffffff);
                        if (CW[i] == CW_old)
                        {                                                                    //the current CW is equal to CW_old after repeat_counter[REPEAT_NUM-1] times sampling.
                            if (bTmpCount >= wRepeatimes)                    //if current value is equal to CW_old after 2*repeat_counter[REPEAT_NUM-1] times sampling.
                            {                                                                //  then accept this CW[i] as a new value
                                bTmpCount = 0;
                            }
                            else if (i > 0)
                            {
                                i = i - 1;
                            }
                        }
                        else
                        {
                            bTmpCount = 0;
                            CW_old = CW[i];                                         // update CW_old by this new value
                        }
                    }
                    else if (i > 0)
                    {
                        i = i - 1;
                    }
                }
                else
                {
                    CW[i]  = (u4RegRead4B(VFE_STA_01) & 0x0fffffff);
                    if (CW[i] == CW_old)
                    {
                        if (i > 0)
                        {
                            bTmpCount = bTmpCount + 1;         // increase counter in case of the same value happened
                            i = i - 1;
                        }
                    }
                    else                                                       //Current value != old value
                    {
                        if ((k < REPEAT_NUM) && (bTmpCount > 0))
                        {
                            if (k > 0)                                       // skip the first time
                            {
                                wRepeatimes += bTmpCount;
                            }
                            bTmpCount = 0;                              //reset counter
                            if (k == (REPEAT_NUM - 1))
                            {
                                wRepeatimes = wRepeatimes / (REPEAT_NUM - 1);
                                repeat_counter = 2 * wRepeatimes;
                            }
                            k = k + 1;
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
            CW_old = RegReadFldAlign(VFE_16, DCLK_HLINE_LENGTH) + 1;
            if (fgIsMainVga() || fgIsPipVga())
            {
                mode = _bVgaTiming;
            }
            else
            {
                mode = _bHdtvTiming;
            }
            #if ENABLE_IC_TEST_MODULE
            CW_old = CW_old * (100L * (UINT32)Get_VGAMODE_IHF(mode));//slt_yl
            #else
            CW_old = CW_old * (100L * (UINT32)VGATIMING_TABLE[mode].IHF);
            x_mem_free(CW);
            #endif
            //x_mem_free(CW);//

            vRegWrite4B(VFE_15, CW_backup);
            vRegWrite4B(VFE_14, _bVFE14_backup);   //restore VFE_14
            vRegWrite4B(0x240c, dwTmp);
            _wSDDS_delay_counter = 0;
            _bSDDSCaliState = SDDS_CALI_WAIT;

            break;

        case SDDS_CALI_WAIT:
            if (_wSDDS_delay_counter > 200)
            {
                _bSDDSCaliState = SDDS_CALI_END;
            }
            break;

        case SDDS_CALI_END:
            if ((_bKI0 == _bKI0_MAX) && (_bKP0 == _bKP0_MAX))
            {
                _bSDDSCaliState = SDDS_CALI_NOTHING;
                Printf("End of SDDS identification\n");
            }
            else
            {
                if (_bKP0 == _bKP0_MAX)
                {
                    _bKI0 = _bKI0 + 1;
                    _bKP0 = _bKP0_init;
                }
                else
                {
                    _bKP0 = _bKP0 + 1;
                }
                _bSDDSCaliState = SDDS_CALI_START;
            }
            break;
    }
#endif
}

UINT32 DDDS_GetSpreadSpectrum(void)
{ 
    return _u4Permillage;
}

UINT32 DDDS_GetSpreadSpectrumFreq(void)
{
    return _u4Freq;
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
void DDDS_SetSpreadSpectrum(UINT32 u4Freq, UINT32 u4Range)
{
#if 1//def CC_MT8223
UINT32 u4Tick = 107;//225 for 62973MHz
UINT32 u4CloseLoop;
UINT32 u4Step = 0,u4Step1 = 0;

     if (_u4VPllFrequency == 0)
     {
          return;
     }
    
     if ((u4Freq == _u4Freq) && (u4Range == _u4Permillage))
     {
         return;
     }

     _u4Permillage = u4Range;
     
     u4CloseLoop =  RegReadFldAlign(OUTSTG_OS_E3, OS_E3_FIX_FS_DDDS_SEL);

     if (u4CloseLoop == 0)
     {
      vDrvVoDclkSpreadInit(1);    //VOPLL free run
     }     
        
     // [Step1] c_psread_period=0.25* spread_controller_clock / spreading_speed ...(1)
     if (u4Freq >0)
     {
         u4Tick = ( 27000000 / 4 ) / u4Freq;
         _u4Freq = u4Freq;
     }
     else
     {
         _u4Freq = 62973;//or 62937Hz
     }
     vDrvVoDclkSpreSpecPeriod(u4Tick);
     Printf("[SA7] u4Tick = %xl \n",u4Tick);
     
     // [Step2] c_psread_step=4*2^24*syspll/f_dds* spreading_amount/2* spreading_speed / spread_controller_clock   ..(2)
     //         c_spread_cw = syspllclk / f_dds
     // according(1)(2),
     //         c_psread_step=4*2^24*syspll/f_dds* spreading_amount/2 / (4* c_psread_period)
     //         c_psread_step=2^25*syspll/f_dds* spreading_amount / ( 4* c_psread_period)
         
     if (LVDS_OUTPUT_PORT == SINGLE_PORT)
     {
        u4Step1 = (UINT32)(BSP_GetPLLClock(CAL_SRC_SYSPLL) * 1);//single : VOPLL -> 1x by OCLK view
     }
     else if(LVDS_OUTPUT_PORT == DUAL_PORT)
     {
        u4Step1 = (UINT32)(BSP_GetPLLClock(CAL_SRC_SYSPLL) * 2);//dual : VOPLL -> 2x by OCLK view
     }
     else//four port
     {
        u4Step1 = (UINT32)(BSP_GetPLLClock(CAL_SRC_SYSPLL) * 4);//four : VOPLL -> 4x by OCLK view
     } 
     
     if(u4Range<250)
     {
      u4Step = ((((u4Step1/(_u4VPllFrequency/100))<< 12)/1000* u4Range) << 13)/100/ (u4Tick<< 2);
      Printf("[SA7-1] u4Step = %xl \n",u4Step);
     } 
     else
     {
      u4Step = ((((u4Step1/(_u4VPllFrequency/10))<< 12)/1000* u4Range) << 13)/10/ (u4Tick<< 2);
      Printf("[SA7-2] u4Step = %xl \n",u4Step);     
     } 
     
     Printf("========================================================= \n");

     vDrvVoDclkSpreSpecSTEP((UINT32)u4Step);
             
     vDrvVoDclkSpreSpecDir(SPREAD_CENTER);
     
     if (u4CloseLoop == 0)
     {
     vDrvVoDclkSpreadInit(0);    //VOPLL locked
     }
     
#else
        UINT64 u8Step = 0;
        UINT32 u4Tick = 107;//225 for 62973MHz
        UINT32 u4CloseLoop;
    
        if (_u4VPllFrequency == 0)
        {
        //Printf("_u4VPllFrequency = 0");
            return;
        }
        if ((u4Freq == _u4Freq) && (u4Range == _u4Permillage))
        {
        //Printf("Same SS\n");
            return;
        }
    
        u4CloseLoop =  RegReadFldAlign(OUTSTG_OS_E3, OS_E3_FIX_FS_DDDS_SEL);
    
    //Printf("Set SS: %d, %d\n", u4Freq, u4Range);
        _u4Permillage = u4Range;
    
        if (u4CloseLoop == 0)
        {
        vDrvVoDclkSpreadInit(1);    //VOPLL free run
        }
        // to prevent tick is 0
        if (u4Freq > 0)
        {
            // tick = [u4Period * 10^(-6)] / [1 / 27*(10^6)]
            //u4Tick = u4Period * 27;
            u4Tick =  ( 27000000 / 4 ) / u4Freq;//change the argument from period to frequency
            _u4Freq = u4Freq;
        }
        else//because Tick = 225
        {
            _u4Freq = 62973; //30000;
        }
        vDrvVoDclkSpreSpecPeriod(u4Tick);
    
        // control word = (syspll*1.5/_u4VPllFrequency) << 24
        // step = control * (range/2) / tick
        //u8Step = (UINT32)(BSP_GetPLLClock(CAL_SRC_SYSPLL) * 3 / 2);//for spread range test
        if (LVDS_OUTPUT_PORT == SINGLE_PORT)
        {
           u8Step = (UINT32)(BSP_GetPLLClock(CAL_SRC_SYSPLL) * 1);//single : VOPLL -> 1x by OCLK view
        }
        else if(LVDS_OUTPUT_PORT == DUAL_PORT)
        {
           u8Step = (UINT32)(BSP_GetPLLClock(CAL_SRC_SYSPLL) * 2);//dual : VOPLL -> 2x by OCLK view
        }
        else//four port
        {
           u8Step = (UINT32)(BSP_GetPLLClock(CAL_SRC_SYSPLL) * 4);//four : VOPLL -> 4x by OCLK view
        } 
        u8Step = (u8Step * _u4Permillage) << 23;
        //u8Step = u8Step / _u4VPllFrequency;
        u8Step = u8Div6432(u8Step, _u4VPllFrequency, NULL);
        //u8Step = u8Step / (UINT32)(u4Tick * 1000);
        u8Step = u8Div6432(u8Step, (UINT32)(u4Tick * 1000), NULL);
        vDrvVoDclkSpreSpecSTEP((UINT32)u8Step);
    
        vDrvVoDclkSpreSpecDir(SPREAD_CENTER);
    
        if (u4CloseLoop == 0)
        {
        vDrvVoDclkSpreadInit(0);    //VOPLL locked
        }
    }

#endif
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

// 5371 style
#if 0
    UINT32 dTmp;
    UINT64 u8Tmp;

    if (_u4VPllFrequency == 0)
    {
        return;
    }

    _u4Permillage = u4Permillage;

    vDrvVoDclkSpreadInit(1);    //VOPLL free run

    //set step
    //dTmp =  ((((UINT32)(bPercent))*12/2)<<22)/((UINT32)(dwDCLK/1000)*25/SS_FREQ);
    u8Tmp = _u4Permillage;
    u8Tmp = (u8Tmp * 24 * SS_FREQ) << 26;
    dTmp = (UINT32)(u8Tmp / _u4VPllFrequency);

    //dTmp =  (UINT32)((((UINT64)(u4Permillage * 24 * SS_FREQ)) << 26) / _u4VPllFrequency);
    vDrvVoDclkSpreSpecSTEP(dTmp);

    //set period
    dTmp = (27000 / 4) / SS_FREQ;
    vDrvVoDclkSpreSpecPeriod(dTmp);

    vDrvVoDclkSpreSpecDir(SPREAD_CENTER);

    vDrvVoDclkSpreadInit(0);    //VOPLL locked
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
 #if !REMOVE_UNCALLED_FUNCTION
void vDrvCLKINSetHsyncWid(UINT8 wWidth)  //  Only use 8-bits , because the HsyncWifth << 255
{

    //vWriteVDOIN(VFE_16, 2, wWidth>>2);
    //vWriteVDOINMsk(VFE_16, (wWidth&0x03)<<6, 0xc0, 1);
    vRegWriteFldAlign(VFE_16, wWidth, DCLK_HSYNC_WIDTH);

}
#endif

/**
 * @brief Set the ADCPLL power on/off
 *
 * Set the ADCPLL power on/off
 * @param bPow bPow is the on/off state (POWER_ON, POWER_DOWN)
 * @retval void
 */
void vDrvADCPLLPower(UINT8 bPow)
{
#ifndef CC_UP8032_ATV
    UNUSED(bPow);
#endif

}


/**
 * @brief Set the ADCPLL power on/off and the lowest power
 *
 * Set the ADCPLL power on/off, and use the lowest power setting
 * @param bPow bPow is the on/off state (POWER_ON, POWER_DOWN)
 * @retval void
 */
void vDrvADCPLLMinPower(UINT8 bPow)
{
    //vDrvADCCLKSrc(CLK_OF_XTAL);
    vDrvADCPLLPower(bPow);
    //CKGEN_WRITE32(offset, value)(0x150, ((CKGEN_READ32(0x150) & 0x00000000))); --Tomson mini power function turn off ADCPLL power
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
    dRealClk = ((((((UINT32)_bGetCLKByte3 << 8) | (UINT32)_bGetCLKByte2) << 8) | (UINT32)_bGetCLKByte1) << 8) | (UINT32)_bGetCLKByte0;

    if ((BYTE3(dRealClk) == 14) && (BYTE2(dRealClk) > 0xc0) && (_wVgaClock > wHtotal))
    {
        return;
    }
    dRealClk = dRealClk + (0x01000000L);    //(0x00010000L);

    // Cindy: divide by zero case, should we do some protection?
    if (wHtotal == 0)
    {
        wHtotal = 1;
    }

    dCalClk = (dRealClk / wHtotal) * (UINT32)_wVgaClock;
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
    UINT8 bCnt, bchannel;
    for (bchannel = 0; bchannel < CHANNEL_NUM; bchannel++)
    {
        for (bCnt = 0; bCnt < 31; bCnt++)
        {
            _dSumTmpPhase[bchannel][bCnt] = 0;
        }
        _dBestSumALL[bchannel] = 0;
        _bChBestPhase[bchannel] = 0xff;
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
          Printf(" bOrderPhase= %u ,bIndex= %u\n",bOrderPhase[bCnt],bIndex[bCnt]);
      }
      //Step 2: to check the validation of the three phases
      bCnt =0; 
      i=0;
      do 
      {  
           bCnt = (bOrderPhase[i+1]<(bOrderPhase[i]+7)) ? (bCnt+1): (0);
           i++;
      } while(( bCnt <2) &&(i<5));
      if ((bCnt >1) &&(i>0)&& (i<5)) // i only can be one of 1,2,3,4.
      {
          Printf("successful condition : i= %u\n",i);
          Printf("Center Phase=%u, ADC output clock= %u\n",bOrderPhase[i-1],(bIndex[i-1]+1) );
          return (bIndex[i-1]);  //the value of bIndex[x] only can be 0 or 1 or 2
      }
      else 
      {    
           Printf("Search condition:i= %d\n",i);
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
                Printf("ADC output clock=1, Average Phase= %d\n",bPhase_Index);
           }
           
           vRegWriteFldAlign(ASYNC_0F, _bChBestPhase[0] , AS_PHASESEL_RX);
           vRegWriteFldAlign(ASYNC_0F, _bChBestPhase[1]  , AS_PHASESEL_GX);
           vRegWriteFldAlign(ASYNC_0F, _bChBestPhase[2]  , AS_PHASESEL_BX);

           switch (bPhase_Index)
           {
                case 0:
                vRegWriteFldAlign(VFE_00, 1, CLKOSEL_1);	
                vRegWriteFldAlign(VFE_00, 0, CLKOSEL_2);	
                vRegWriteFldAlign(VFE_00, 0, CLKOSEL_3);	
                case 1:
                vRegWriteFldAlign(VFE_00, 1, CLKOSEL_2);	
                vRegWriteFldAlign(VFE_00, 0, CLKOSEL_1);	
                vRegWriteFldAlign(VFE_00, 0, CLKOSEL_3);	
                case 2:
                default:
                vRegWriteFldAlign(VFE_00, 1, CLKOSEL_3);	
                vRegWriteFldAlign(VFE_00, 0, CLKOSEL_1);	
                vRegWriteFldAlign(VFE_00, 0, CLKOSEL_2);	
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
    bRealPhase = (_bCurPhase == 0) ? 31 : (_bCurPhase - 1);

    for (bChannel_num = 0; bChannel_num < CHANNEL_NUM; bChannel_num++)
    {
        if (bRealPhase < MAX_PHASE_ARRAY)
        {
            _dSumTmpPhase[bChannel_num][bRealPhase] = _dwPhase3CH[bChannel_num]; //dTmp ;
        }
    }
    #if Phase3Channel 
              DBG_Printf(DBG_AUTO,"bRealPhase=%2u, Sum1=%8u,   Sum2=%8u,   Sum3=%8u\n",bRealPhase,_dwPhase3CH[0],_dwPhase3CH[1],_dwPhase3CH[2]);
    #else
              DBG_Printf(DBG_AUTO,"bRealPhase=%u, Sum=%u\n",bRealPhase,dTmp);
    #endif

    if (bRealPhase == 0)
    {
        a = 31 ;
        b = 0 ;
    }
    else
    {
        a = bRealPhase - 1 ;
        b = bRealPhase ;
    }
    for (bChannel_num = 0; bChannel_num < CHANNEL_NUM; bChannel_num++)
    {
        if ((a < MAX_PHASE_ARRAY) && (b < MAX_PHASE_ARRAY))  //for klocwork check to protect over dimension
        {
            if ( _dSumTmpPhase[bChannel_num][a] > _dSumTmpPhase[bChannel_num][b])
            {
                //find max slop
                dTmp = _dSumTmpPhase[bChannel_num][a] - _dSumTmpPhase[bChannel_num][b];
                if (dTmp > _dBestSumALL[bChannel_num] )
                {
                    _dBestSumALL[bChannel_num] = dTmp ;
                    _bMaxSlope[bChannel_num]  = a ;
                }
            }
        }
    }

    if (_bCurPhase < 32)  //      (_bCurPhase <= 32)
    {
        //vDrvVGASetPhase((_bCurPhase == 32) ? 0 : _bCurPhase);
        vDrvVGASetPhase_Simple((_bCurPhase == 32) ? 0 : _bCurPhase);
        //_bVgaDelayCnt = 1;
    }
    else
    {
        for (bChannel_num = 0; bChannel_num < CHANNEL_NUM; bChannel_num++)
        {
            _dBestSumALL[bChannel_num]  =  0 ;
            b = (_bMaxSlope[bChannel_num] >= 3) ? (_bMaxSlope[bChannel_num] - 3) : (_bMaxSlope[bChannel_num] + 31 - 3);
            //b = _bMaxSlope[bChannel_num] ;
            a = 8;
                    DBG_Printf(DBG_AUTO,"CH %u Sorting Phase from %2u,  %2u\n",(bChannel_num+1),_bMaxSlope[bChannel_num] ,b);
                    //Printf("CH %u Sorting Phase from %2u,  %2u\n",(bChannel_num+1),_bMaxSlope[bChannel_num], b);
                  
            do
            {
                if (b < MAX_PHASE_ARRAY)  //for klocwork check
                {
                              DBG_Printf(DBG_AUTO,"bChannel_num =%2u, phase=%2u, data=%8u \n", (bChannel_num+1) ,b,_dSumTmpPhase[bChannel_num][b]);
           
                    if (_dSumTmpPhase[bChannel_num][b] > _dBestSumALL[bChannel_num])
                    {
                        _dBestSumALL[bChannel_num] = _dSumTmpPhase[bChannel_num][b]  ;
                        _bChBestPhase[bChannel_num] = b ;
                    }
                    b = (b == 0) ? 31 : (b - 1);
                    a-- ;
                }
                else   //for klocwork check
                {
                    a = 0;
                }
                    }while(a > 0);
        }
    }
}

void vYPbPrPhaseIsr_New(void)    //NEW_PHASE_ALGORITHM
{
    if (_bCurPhase < 32 )  // do use <=32
    {
        vPhaseSearchCore_New();
    }
    else          // if(_bCurPhase >31 )
    {
        vDrvSetBestPhase();
        _RETIMENeedReset = TRUE ;
        vClrAutoFlg(SP0_AUTO_PHASE);

        vApiNotifyAutoDone(((fgIsMainYPbPr()) ? 0 : 1), TRUE);
#if NOTIFY_PROGRESS
        vApiNotifyVGAAutoProgress(((fgIsMainYPbPr()) ? 0 : 1), 100);
#endif
        vApiVideoClrEvtFlg(VDO_FLG_VGAAUTO_DOING);

        vSetSP0AutoState(VDO_AUTO_NOT_BEGIN);
        //vVgaAutoPosInit();
#if CHANGE_SDDS_KPI
        vDrvEnableChang_SDDS_BW();
#endif
        _bVgaDelayCnt = 2;
        _bAutoISR = 0 ;
        _bChBestPhase[0] = RegReadFldAlign(ASYNC_0F, AS_PHASESEL_RX);
        _bChBestPhase[1] = RegReadFldAlign(ASYNC_0F, AS_PHASESEL_GX);
        _bChBestPhase[2] = RegReadFldAlign(ASYNC_0F, AS_PHASESEL_BX);
        Printf("YPbPr ch1_phase= %2u ,ch2_phase= %2u ,ch3_phase= %2u\n",_bChBestPhase[0],_bChBestPhase[1],_bChBestPhase[2]);
    }
}

void vVgaPhaseIsr_New(void)   //NEW_PHASE_ALGORITHM
{
    if (_bCurPhase < 32 )  // do use <=32
    {
        vPhaseSearchCore_New();
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
        _bChBestPhase[0] = RegReadFldAlign(ASYNC_0F, AS_PHASESEL_RX);
        _bChBestPhase[1] = RegReadFldAlign(ASYNC_0F, AS_PHASESEL_GX);
        _bChBestPhase[2] = RegReadFldAlign(ASYNC_0F, AS_PHASESEL_BX);
        Printf("VGA ch1_phase= %2u ,ch2_phase= %2u ,ch3_phase= %2u\n",_bChBestPhase[0],_bChBestPhase[1],_bChBestPhase[2]);
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
	//	LogD(_dSumTmp[_bCurPhase]);
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
	//	LogD(_dSumTmp[2]);	//sc_yang
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


        vApiNotifyAutoDone(((fgIsMainYPbPr()) ? 0 : 1), TRUE);
#if NOTIFY_PROGRESS
        vApiNotifyVGAAutoProgress(((fgIsMainYPbPr()) ? 0 : 1), 100);
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

#if EnableDDDSCalibration
UINT16 _wDDDS_delay_counter;
UINT8 _bDDDSCaliState;
UINT8 _bDKI0,_bDKP0,_bERROR_LIM;
UINT32 _OUTSTG_OS_E3_backup;
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
#define DMEM_SIZE 0x2000  //0x20000
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


#if THERMAL_COMPENSATION_ENABLE
void vThermalCompInit(void)
{
    UINT8 bTempCurr;
    vDrvAdcpllSetPhaseDelay(0x40);
    bTempCurr = vCurTemperature();
    fgApiEepromWriteByte(EEP_TEMPERATURE, bTempCurr);
}

/**
 * @brief System PLL pwoer on/off switch
 *
 * Set System PLL pwoer on/off
 * @param on/off
 * @retval void
 */
//UINT8   _bTHERMAL;
//UINT8   _bTHERMALCOM_STATE;

void vDrvThermalCompensate(void)
{
    UINT8 bTempEE, bTempCurr, bTempDiff, bPD;
    UINT16 bTemp;
    bTempCurr = vCurTemperature();

    switch (_bTHERMALCOM_STATE)
    {
        case VDO_THERMALCOM_INI:
            _bTHERMAL_CNT++;
            if (_bTHERMAL_CNT == 200)
            {
                //load Temperature from EE, save Temperature to EEprom while VGA auto
                bTempEE = bApiEepromReadByte(EEP_TEMPERATURE);
                bPD = 0x40 + (bTempEE - bTempCurr);
                /*
                if (bTempCurr>bTempEE)	//if Temperature++, Phase delay --
                {
                	bTempDiff = bTempCurr - bTempEE;
                	bPD = 0x40 - bTempDiff;
                }
                else					//if Temperature--, Phase delay ++
                {
                	bTempDiff = bTempEE - bTempCurr;
                	bPD = 0x40 + bTempDiff;
                }*/
                vDrvAdcpllSetPhaseDelay(bPD);
                _bTHERMAL = bTempCurr;
                _bTHERMAL_TEMP = bTempCurr;
                _bTHERMAL_CNT = 0;
                _bTHERMALCOM_STATE = VDO_THERMALCOM_STATE1;
            }
            break;
        case VDO_THERMALCOM_STATE1:
            bTemp = (_bTHERMAL_TEMP + bTempCurr) >> 1;
            _bTHERMAL_CNT++;
            if (DIFF(bTemp, _bTHERMAL) > 1)
            {
                bTempEE = bApiEepromReadByte(EEP_TEMPERATURE);
                bPD = 0x40 + (bTempEE - bTemp);
                vDrvAdcpllSetPhaseDelay(bPD);
                _bTHERMAL = bTemp;
                _bTHERMAL_TEMP = bTemp;
                _bTHERMAL_CNT = 0;
                //_bTHERMALCOM_STATE=VDO_THERMALCOM_STATE2;
            }
            break;
        case VDO_THERMALCOM_STATE2:
            break;
        case VDO_THERMALCOM_STATE3:
            break;
        default:
            break;
    }	//switch case end



    // Set Phase delay
    if (bTempDiff > 2)
    {
        vDrvAdcpllSetPhaseDelay(bPD);
    }
}



/**
 * @brief System PLL pwoer on/off switch
 *
 * Set System PLL pwoer on/off
 * @param on/off
 * @retval void
 */
UINT8 vAdcpllCurPhaseDelay(void)
{
    UINT8 bTemp;
    bTemp = (CKGEN_READ32(0x170) & 0x80ffffff) >> 24 ;
    return bTemp;
}

/**
 * @brief System PLL pwoer on/off switch
 *
 * Set System PLL pwoer on/off
 * @param on/off
 * @retval void
 */
void vDrvAdcpllSetPhaseDelay(UINT8 bVal)
{
    CKGEN_WRITE32(0x170, ((CKGEN_READ32(0x170) & 0x80FFFFFF)  | bVal << 24));
    CKGEN_WRITE32(0x180, ((CKGEN_READ32(0x180) & 0x80FFFFFF)  | bVal << 24));
}


/**
 * @brief System PLL pwoer on/off switch
 *
 * Set System PLL pwoer on/off
 * @param on/off
 * @retval void
 */
UINT8 vCurTemperature(void)
{
    UINT8 bTempCurr;
    //power on ServoAdc Ch5/Ch6
    PDWNC_WRITE32(SRVCFG_REG, (PDWNC_READ32(SRVCFG_REG)&0xffffff3A) | 0x000000C1);
    //Temp_Sta=ch6-ch5
    bTempCurr = PDWNC_READ32(0x358) & 0xff;
    return bTempCurr;
}

#endif
