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
 * $RCSfile: drv_lvds.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#ifdef CC_COPLAT_MT82
#include "util.h"
#endif
#include "general.h"
#include "hw_lvds.h"
#include "hw_scpos.h"
#include "drv_lvds.h"
#include "drv_vdoclk.h"
#include "x_timer.h"
#include "nptv_debug.h"
#ifndef CC_COPLAT_MT82
#include "drvcust_if.h"
#include "pmx_drvif.h"
#include "osd_drvif.h"
#include "vdp_if.h"
#endif


// switch the source of both group b and group c from internal group b
#define CC_LVDS_WA1


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef DRV_SUPPORT_EXTMJC
static UINT32 _u4CurrentLvdsMode = (UINT32)eLvdsVidNormal;
static UINT32 _u4LvdsColorDomain = eColorDomainRGB;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _vDrvLVDSSetPortMap(void)
{
    // Lvds control : 
    UINT32 u4LvdsCtrlAEven = 0;
    UINT32 u4LvdsCtrlAOdd = 1;
    #ifndef CC_LVDS_WA1
    UINT32 u4LvdsCtrlBEven = 2;
    #endif
    UINT32 u4LvdsCtrlBOdd = 3;
    UINT32 au4PortRotte[4] = {0, 1, 2, 3};

    // panel specific port rotate
    if (LVDS_USE_INDEPENDENT_SETTING)
    {
        au4PortRotte[0] = (PANEL_GetControlWord() >> 9) & 0x3;
        au4PortRotte[1] = (PANEL_GetControlWord() >> 11) & 0x3;
//        au4PortRotte[2] = (PANEL_GetControlWord() >> 13) & 0x3;
//        au4PortRotte[3] = (PANEL_GetControlWord() >> 15) & 0x3;
    }

    // PCB layout rotate
    u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
    u4LvdsCtrlAOdd = DRVCUST_PanelGet(eLvdsACtrlOdd);
//    u4LvdsCtrlBEven = DRVCUST_PanelGet(eLvdsBCtrlEven);
//    u4LvdsCtrlBOdd = DRVCUST_PanelGet(eLvdsBCtrlOdd);
    u4LvdsCtrlAEven = (u4LvdsCtrlAEven < 4)? au4PortRotte[u4LvdsCtrlAEven] : 0;
    u4LvdsCtrlAOdd = (u4LvdsCtrlAOdd < 4)? au4PortRotte[u4LvdsCtrlAOdd] : 0;   

    #if 0
    if ( !LVDS_USE_INDEPENDENT_SETTING )
    {
        u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
        u4LvdsCtrlAOdd = DRVCUST_PanelGet(eLvdsACtrlOdd);
        u4LvdsCtrlBEven = DRVCUST_PanelGet(eLvdsBCtrlEven);
        u4LvdsCtrlBOdd = DRVCUST_PanelGet(eLvdsBCtrlOdd);
    }
    else
    {
        u4LvdsCtrlAEven = (PANEL_GetControlWord() >> 9) & 0x3;
        u4LvdsCtrlAOdd = (PANEL_GetControlWord() >> 11) & 0x3;
        u4LvdsCtrlBEven = (PANEL_GetControlWord() >> 13) & 0x3;
        u4LvdsCtrlBOdd = (PANEL_GetControlWord() >> 15) & 0x3;
    }
    #endif

    vRegWriteFldAlign(OUTSTG_OS_81, u4LvdsCtrlAEven, OS_81_LVDS_A_CTRL);
    vRegWriteFldAlign(OUTSTG_OS_81, u4LvdsCtrlAOdd,  OS_81_LVDS_B_CTRL);
    #ifdef CC_LVDS_WA1
    vRegWriteFldAlign(OUTSTG_OS_81, u4LvdsCtrlAOdd, OS_81_LVDS_C_CTRL);
    #else
    vRegWriteFldAlign(OUTSTG_OS_81, u4LvdsCtrlBEven, OS_81_LVDS_C_CTRL);
    #endif
    vRegWriteFldAlign(OUTSTG_OS_81, u4LvdsCtrlBOdd,  OS_81_LVDS_D_CTRL);
}


static void _vDrvLVDSConfigFormat(void)
{
    UINT32 u4MSBSW;
    
    u4MSBSW = LVDS_DISP_MSB_SW;
    #ifdef DRV_SUPPORT_EXTMJC
    if (IS_COMPANION_CHIP_ON())
    {
        if (vDrvGetCurLvdsMode() !=  eLvdsVidNormal)
        {
            u4MSBSW = LVDS_MSB_SW_OFF;
        }
    }
    #endif

    
    // set lvds format
    if (u4MSBSW == LVDS_MSB_SW_ON) // NS
    {
        vRegWriteFldAlign(OSTG_LVDS_00, 1, LVDS_00_MSB_INV);
    }
    else // JEIDA
    {
        vRegWriteFldAlign(OSTG_LVDS_00, 0, LVDS_00_MSB_INV);
    }

}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void vDrvLVDSInit(void)
{
    UINT32 u4Value;
	
    // set TVO(driving current) if necessary
    u4Value = PANEL_GetDrivingCurrent();
    if (u4Value <= 15)
    {
        vDrvLVDSSetDriving(u4Value);
    }
    else
    {
        vDrvLVDSSetDriving(3);
    }

    vRegWriteFldAlign(OSTG_LVDS_01, 2, LVDS_01_RG_LVDS_ATERM_EN);
	
    vRegWriteFldAlign(OSTG_LVDS_00, 0,LVDS_00_REG_7_4FIFO_RST);

    //Set RCNT=1 for 5388 low voltage and SLT capture board
    vRegWriteFldAlign(OSTG_LVDS_00, 1, LVDS_00_RCNT_SEL);

    // enable vsync output for dual LVDS channels
    vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_VS_SEL);
    // enable hsync output for dual LVDS channels
    vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_HS_SEL);
    // enable data_en output for dual LVDS channels
    vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_DE_SEL);
    
    //LVDS 7->4 FIFO enable 
    #ifdef CC_MTK_LOADER
    vDrvLVDS7To4FifoEnable();
    #else
    vRegWriteFldAlign(OUTSTG_OS_81, 1, OS_81_REG_LVDS_E);
    #endif
}

void vDrvLVDSSetDriving(UINT8 bDriving)
{
    bDriving &= 0xf;  //0~15
    vRegWriteFldAlign(OSTG_LVDS_01, bDriving, LVDS_01_RG_LVDS_ATVO);
}

UINT8 vDrvLVDSGetDriving(void)
{
	return RegReadFldAlign(OSTG_LVDS_01,  LVDS_01_RG_LVDS_ATVO);
}

void vDrvLVDSFifoRstEnable(UINT32 u4OnOff)
{
// Fifo Reset workaround
// #define LVDS_FIFORST_WA  == >LVDS_DE_FOR_RST
// The fifo reset mechanism cannot turn on immediately with data enable.
// This will cause the fisrt line of frame suffering from missing fifo reset.
    if (u4OnOff)
    {
        vRegWriteFldAlign(OSTG_LVDS_00, 1, LVDS_00_FIFORST);
        vRegWriteFldAlign(OSTG_LVDS_00, 1, LVDS_00_DE_FOR_RST);
        LOG(3, "[DM] Turn on FIFORESET\n");
    }
    else
    {
        vRegWriteFldAlign(OSTG_LVDS_00, 0, LVDS_00_FIFORST);
        vRegWriteFldAlign(OSTG_LVDS_00, 0, LVDS_00_DE_FOR_RST);
        LOG(3, "[DM] Turn off FIFORESET\n");
    }
}


void LVDS_SwitchDataEnable(BOOL fEnable)
{
    if (fEnable)
    {
       //OUTSTG_DEN_EN==0 : data enable
        vRegWriteFldAlign(OUTSTG_OS_15, 0, OS_15_DEN_EN);
    }
    else
    {
    	//OUTSTG_DEN_EN==1 : data disable
        vRegWriteFldAlign(OUTSTG_OS_15, 1, OS_15_DEN_EN);
    }
}

#ifdef DRV_SUPPORT_EXTMJC
void vDrvSetLvdsColorDomain(UINT32 u4ColorDomain)
{
#ifndef CC_MTK_LOADER
    	UINT32 u4BgColor;
#endif

	_u4LvdsColorDomain =u4ColorDomain;
#ifndef CC_MTK_LOADER
	// set mute color
	VDP_GetBg(SV_VP_MAIN, &u4BgColor);
	VDP_SetBg(SV_VP_MAIN, u4BgColor);
	VDP_GetBg(SV_VP_PIP, &u4BgColor);
	VDP_SetBg(SV_VP_PIP, u4BgColor);
#endif
    	if(vDrvGetLvdsColorDomain() == eColorDomainYUV)
    	{
		//PMX_SetYuvMode(TRUE);
		//OSD_BASE_SetYuvOutput(TRUE);
		PMX_SetYuvMode(FALSE); // RGB to YUV do by LVDS TX Transform
		OSD_BASE_SetYuvOutput(FALSE); // RGB to YUV do by LVDS TX Transform
    	}
    	else
    	{
    		PMX_SetYuvMode(FALSE);
    	       OSD_BASE_SetYuvOutput(FALSE);
    	}

}

UINT32 vDrvGetLvdsColorDomain(void)
{
	return _u4LvdsColorDomain;
}

void vDrvSetCurLvdsMode(UINT32 u4LvdsMode)
{
    if (_u4CurrentLvdsMode == u4LvdsMode)
    {
        return;
    }
    
    _u4CurrentLvdsMode = u4LvdsMode;
    LOG(1, "vDrvSetCurLvdsMode, _u4CurrentLvdsMode=%d\n", _u4CurrentLvdsMode);

    vDrvLVDSChgMod(_u4CurrentLvdsMode);
}

UINT32 vDrvGetCurLvdsMode(void)
{
    return _u4CurrentLvdsMode;
}

#endif

/**
 * @brief vDrvLVDSChgMod
 * Change mode for 53xx + 828x
 *
 * @param  u4Mode
 * @retval void
 */
void vDrvLVDSChgMod(UINT32 u4Mode)
{
   // UINT32 u4PortNum;
    BOOL fgLVDSClkDiv2, fgPixelClkMul2;

   /* if (LVDS_OUTPUT_PORT == SINGLE_PORT)
    {
        u4PortNum = 1;
    }
    else 
    {
        u4PortNum = 2;
    }*/

    // select one mode
    switch ((LVDS_MODE_T)u4Mode)
    {
    case eLvdsVidNormal:
        // Back to the initial setting and return
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_YUV2YC_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_LPF_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_C_LINE_EXT);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_MERGE_OSD);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_2CH_MERGE);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_RGB_444_MERGE);


        if (LVDS_OUTPUT_PORT == SINGLE_PORT)
        {
            fgLVDSClkDiv2=FALSE;
            fgPixelClkMul2=FALSE;
            #ifdef CC_MT5363 
            vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 2, FLD_OCLK_DIV_SEL);
            #else
            vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_OCLK_DIV_SEL);
            #endif
        }
        else //if (LVDS_OUTPUT_PORT == DUAL_PORT) 
        {
            fgLVDSClkDiv2=FALSE;
            fgPixelClkMul2=TRUE;
#ifdef CC_MT5363
            vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_LVDS_DPIX_SEL);
	     vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_OCLK_DIV_SEL);
 #else	
            vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 2, FLD_OCLK_DIV_SEL);
 #endif
        }

        //when single and dual port, total VOPLL = (1/2)*3.5*2 = 3.5
        //VOPLL output=LVDS CLK * 3.5
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 1, FLD_RG_VPLL_DIV12);//set DIV12=1
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_PDIV);//set pre-divider=0 --> /2
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_DIV);//set FB divider_2 = "0" --> 2X

        // LVDSClk/2 and PixelClk x2 setting
#ifdef CC_UP8032_ATV
        if(fgLVDSClkDiv2)
        {
            vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_VCODIV2_SEL);//set VCO CLK DIV2
        }
        else
        {
            vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_VCODIV2_SEL);//set VCO CLK DIV2
        }

        if(fgPixelClkMul2)
        {
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 1, FLD_RG_VPLL_DPIX_X2);//set VOPLL DPIX X2
        }
        else
        {
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_DPIX_X2);//set VOPLL DPIX X2
        }
#else
        vIO32WriteFldAlign(CKGEN_VOPLLCFG1, fgLVDSClkDiv2, FLD_RG_VPLL_VCODIV2_SEL);//set VCO CLK DIV2
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, fgPixelClkMul2, FLD_RG_VPLL_DPIX_X2);//set VOPLL DPIX X2
#endif
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_DPIX_DIV2);//set VOPLL DPIX DIV2=default value
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_DPIX_CKSEL);//set LVDS_DPIX=(1/2)*VCOCKN(P)
         
        break;


    case eLvdsVidYuv422TwoPort:
    case eLvdsVidYuv422TwoPort8Bit:
        // OSD DMA LVDS freq. div2
        // FIXME
//      OSD_DMA_SetLVDSClk(1);
//      IO_WRITE32MSK(OSD_BASE, 0x70, 0x60000080, 0xffff0080);
    
        // Internal Protocal fix JEIDA
   //     vRegWriteFldAlign(OSTG_LVDS_00, 0, LVDS_00_MSB_INV);

        // enable Tx mode 3
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_YUV2YC_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_LPF_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_C_LINE_EXT);
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_MERGE_OSD);
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_2CH_MERGE);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_RGB_444_MERGE);
        #ifdef DRV_SUPPORT_EXTMJC_MODE7
        if (DRVCUST_InitGet(eFlagDDRQfp)) //mt5387 condition
        {
            vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_8BIT_DUAL);
        }
        #endif

        fgLVDSClkDiv2=FALSE;
        fgPixelClkMul2=TRUE;
#ifdef CC_MT5363
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_LVDS_DPIX_SEL);
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_OCLK_DIV_SEL);
#else        
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 2, FLD_OCLK_DIV_SEL);
#endif
 
        //when single and dual port, total VOPLL = (1/2)*3.5*2 = 3.5
        //VOPLL output=LVDS CLK * 3.5
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 1, FLD_RG_VPLL_DIV12);//set DIV12=1
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_PDIV);//set pre-divider=0 --> /2
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_DIV);//set FB divider_2 = "0" --> 2X

        // LVDSClk/2 and PixelClk x2 setting
#ifdef CC_UP8032_ATV
        if(fgLVDSClkDiv2)
        {
            vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_VCODIV2_SEL);//set VCO CLK DIV2
        }
        else
        {
            vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_VCODIV2_SEL);//set VCO CLK DIV2
        }

        if(fgPixelClkMul2)
        {
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 1, FLD_RG_VPLL_DPIX_X2);//set VOPLL DPIX X2
        }
        else
        {
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_DPIX_X2);//set VOPLL DPIX X2
        }
#else
        vIO32WriteFldAlign(CKGEN_VOPLLCFG1, fgLVDSClkDiv2, FLD_RG_VPLL_VCODIV2_SEL);//set VCO CLK DIV2
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, fgPixelClkMul2, FLD_RG_VPLL_DPIX_X2);//set VOPLL DPIX X2
#endif
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_DPIX_DIV2);//set VOPLL DPIX DIV2=default value
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_DPIX_CKSEL);//set LVDS_DPIX=(1/2)*VCOCKN(P)
        break;

    case eLvdsVidRgb444TwoPort:
        // OSD DMA LVDS freq. div2
        // FIXME
//      OSD_DMA_SetLVDSClk(1);
//      IO_WRITE32MSK(OSD_BASE, 0x70, 0x60000080, 0xffff0080);
    
        // Internal Protocal fix JEIDA
   //     vRegWriteFldAlign(OSTG_LVDS_00, 0, LVDS_00_MSB_INV);

        // enable Tx mode 4
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_YUV2YC_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_LPF_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_C_LINE_EXT);
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_MERGE_OSD);
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_2CH_MERGE);
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_RGB_444_MERGE);
 
        fgLVDSClkDiv2=FALSE;
        fgPixelClkMul2=TRUE;
#ifdef CC_MT5363
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_LVDS_DPIX_SEL);
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_OCLK_DIV_SEL);
#else
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 2, FLD_OCLK_DIV_SEL);
#endif
 
        //when single and dual port, total VOPLL = (1/2)*3.5*2 = 3.5
        //VOPLL output=LVDS CLK * 3.5
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 1, FLD_RG_VPLL_DIV12);//set DIV12=1
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_PDIV);//set pre-divider=0 --> /2
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_DIV);//set FB divider_2 = "0" --> 2X

        // LVDSClk/2 and PixelClk x2 setting
#ifdef CC_UP8032_ATV
        if(fgLVDSClkDiv2)
        {
            vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 1, FLD_RG_VPLL_VCODIV2_SEL);//set VCO CLK DIV2
        }
        else
        {
            vIO32WriteFldAlign(CKGEN_VOPLLCFG1, 0, FLD_RG_VPLL_VCODIV2_SEL);//set VCO CLK DIV2
        }

        if(fgPixelClkMul2)
        {
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 1, FLD_RG_VPLL_DPIX_X2);//set VOPLL DPIX X2
        }
        else
        {
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, 0, FLD_RG_VPLL_DPIX_X2);//set VOPLL DPIX X2
        }
#else
        vIO32WriteFldAlign(CKGEN_VOPLLCFG1, fgLVDSClkDiv2, FLD_RG_VPLL_VCODIV2_SEL);//set VCO CLK DIV2
        vIO32WriteFldAlign(CKGEN_VOPLLCFG0, fgPixelClkMul2, FLD_RG_VPLL_DPIX_X2);//set VOPLL DPIX X2
#endif
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_DPIX_DIV2);//set VOPLL DPIX DIV2=default value
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 0, FLD_RG_VPLL_DPIX_CKSEL);//set LVDS_DPIX=(1/2)*VCOCKN(P)
        break;

    case eLvdsVidYuv422OnePort:
    case eLvdsVidRgb444OnePort:
    case eLvdsVidYuv422OnePort8Bit:
    case eLvdsVidRgb444OnePort8Bit:
    default:
        LOG(0, "Not Support LVDS Mode (%d)\n", u4Mode);
        break;
    }

    // set lvds format
    _vDrvLVDSConfigFormat();

    // config LVDS PD, dual port, and 10bit control bits
    vDrvLVDSOn();
    
}

/**
 * @brief vDrvLVDSOn

 * Power On LVDS

 * @param  void
 * @retval void
 */
void vDrvLVDSOn(void)
{
    UINT32 u4DispBit;
    UINT32 u4LVDSPort;
    
    //power up bandgap and all reference circuit
    vRegWriteFldAlign(OSTG_LVDS_01, 0, LVDS_01_RG_LVDS_BIASA_PD);

    u4DispBit = DISP_BIT;
    u4LVDSPort = LVDS_OUTPUT_PORT;
    #ifdef DRV_SUPPORT_EXTMJC
    if (IS_COMPANION_CHIP_ON())
    {
        if (vDrvGetCurLvdsMode() !=  eLvdsVidNormal)
        {
            u4DispBit = DISP_30BIT;
        }
        if ((vDrvGetCurLvdsMode() == eLvdsVidYuv422TwoPort8Bit) || (vDrvGetCurLvdsMode() == eLvdsVidYuv422TwoPort) || (vDrvGetCurLvdsMode() == eLvdsVidRgb444TwoPort))
        {
            u4LVDSPort = DUAL_PORT;
        }
        else if ((vDrvGetCurLvdsMode() == eLvdsVidYuv422OnePort) || (vDrvGetCurLvdsMode() == eLvdsVidRgb444OnePort))
        {
            // not support
        }
    }
    #endif
    
    switch (u4DispBit)
    {
        case DISP_30BIT:
            // enable LVDS 10-bit
            vRegWriteFldAlign(OUTSTG_OS_29, 1, OS_29_10B_EN);
            if (u4LVDSPort == DUAL_PORT)
            {
                vRegWriteFldAlign(OSTG_LVDS_00, 1, LVDS_00_DUAL);
                vRegWriteFldAlign(OSTG_LVDS_01, 0, LVDS_01_RG_LVDS_APD);  // Analog related setting
                vRegWriteFldAlign(OUTSTG_OS_8F, 0, OS_8F_LVDS_PD_21_20);     // Clock 
                vRegWriteFldAlign(OUTSTG_OS_8F, 0, OS_8F_LVDS_PD_9_0);      // Digital related setting  
            }
            else  // single port
            {
                UINT32 u4LvdsCtrlAEven;

                vRegWriteFldAlign(OSTG_LVDS_00, 0, LVDS_00_DUAL);

                u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
                if (u4LvdsCtrlAEven == 1)  // power on Port A-Odd
                {
                    vRegWriteFldAlign(OSTG_LVDS_01, 0x3f, LVDS_01_RG_LVDS_APD);  // Analog related setting: ECLK, E4~E0, OCLK, O4~O0
                }
                else if (u4LvdsCtrlAEven == 0) // power on Port A-Even
                {
                    vRegWriteFldAlign(OSTG_LVDS_01, 0xfc0, LVDS_01_RG_LVDS_APD);  // Analog related setting: ECLK, E4~E0, OCLK, O4~O0
                }
                vRegWriteFldAlign(OUTSTG_OS_8F, 2, OS_8F_LVDS_PD_21_20);     // Clock                
                vRegWriteFldAlign(OUTSTG_OS_8F, 0x3e0, OS_8F_LVDS_PD_9_0);      //  Digital related setting  
            }
            break;

        case DISP_24BIT:
            // disable LVDS 10-bit
            vRegWriteFldAlign(OUTSTG_OS_29, 0, OS_29_10B_EN);
            if (u4LVDSPort == DUAL_PORT)
            {
                vRegWriteFldAlign(OSTG_LVDS_00, 1, LVDS_00_DUAL);
                vRegWriteFldAlign(OSTG_LVDS_01, 0x410, LVDS_01_RG_LVDS_APD);  // Analog related setting
                vRegWriteFldAlign(OUTSTG_OS_8F, 0, OS_8F_LVDS_PD_21_20);     //  Clock
                vRegWriteFldAlign(OUTSTG_OS_8F, 0x210, OS_8F_LVDS_PD_9_0);      // Digital related setting  
            }
            else  // single port
            {
                UINT32 u4LvdsCtrlAEven;

                vRegWriteFldAlign(OSTG_LVDS_00, 0, LVDS_00_DUAL);

                u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
                if (u4LvdsCtrlAEven == 1)  // power on Port A-Odd
                {
                    vRegWriteFldAlign(OSTG_LVDS_01, 0x43f, LVDS_01_RG_LVDS_APD);  // Analog related setting
                }
                else if (u4LvdsCtrlAEven == 0) // power on Port A-Even
                {
                    vRegWriteFldAlign(OSTG_LVDS_01, 0xfd0, LVDS_01_RG_LVDS_APD);  // Analog related setting
                }

                vRegWriteFldAlign(OUTSTG_OS_8F, 2, OS_8F_LVDS_PD_21_20);     //  Clock
                vRegWriteFldAlign(OUTSTG_OS_8F, 0x3f0, OS_8F_LVDS_PD_9_0);      // Digital related setting 
            }
            break;

        case DISP_18BIT:
            // disable LVDS 10-bit
            vRegWriteFldAlign(OUTSTG_OS_29, 0, OS_29_10B_EN);
            if (u4LVDSPort == DUAL_PORT)
            {
                vRegWriteFldAlign(OSTG_LVDS_00, 1, LVDS_00_DUAL);
                vRegWriteFldAlign(OSTG_LVDS_01, 0x618, LVDS_01_RG_LVDS_APD);  // Analog related setting
                vRegWriteFldAlign(OUTSTG_OS_8F, 0, OS_8F_LVDS_PD_21_20);     //  Clock
                vRegWriteFldAlign(OUTSTG_OS_8F, 0x318, OS_8F_LVDS_PD_9_0);      // Digital related setting  
            }
            else  // single port
            {
                UINT32 u4LvdsCtrlAEven;

                vRegWriteFldAlign(OSTG_LVDS_00, 0, LVDS_00_DUAL);
                u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
                if (u4LvdsCtrlAEven == 1)  // power on Port A-Odd
                {
                    vRegWriteFldAlign(OSTG_LVDS_01, 0x63f, LVDS_01_RG_LVDS_APD);  // Analog related setting
                }
                else if (u4LvdsCtrlAEven == 0) // power on Port A-Even
                {
                    vRegWriteFldAlign(OSTG_LVDS_01, 0xfd8, LVDS_01_RG_LVDS_APD);  // Analog related setting
                }
                vRegWriteFldAlign(OUTSTG_OS_8F, 2, OS_8F_LVDS_PD_21_20);     //  Clock
                vRegWriteFldAlign(OUTSTG_OS_8F, 0x3f8, OS_8F_LVDS_PD_9_0);      // Digital related setting 
            }
            break;

        default:
            break;
    }

    vDrvLVDSFifoRstEnable(SV_ON);
    vRegWriteFldAlign(OSTG_LVDS_01, 1, LVDS_01_RG_LVDS_ARST);// analog LVDS reset

    #ifdef CC_COPLAT_MT82
    vUtDelay2us(5);
    #else
    HAL_Delay_us(10);
    #endif
    
    if (u4LVDSPort == SINGLE_PORT)
    {
        vDrvLVDSFifoRstEnable(SV_OFF);
        #ifdef CC_COPLAT_MT82
        vUtDelay2us(5);
        #else
        HAL_Delay_us(10);
        #endif
    }
    vRegWriteFldAlign(OSTG_LVDS_01, 0, LVDS_01_RG_LVDS_ARST);// analog LVDS normal

    #ifdef CC_EXTERNAL_LVDS_CHIP 
    IMpac3P_SetLvds(1);
    #endif
}

/**
 * @brief vDrvLVDSOff

 * Power Down LVDS

 * @param  void
 * @retval void
 */
void vDrvLVDSOff(void)
{
    // power down all digital channel
    //vRegWriteFldAlign(OUTSTG_OS_88, 0xfff, OUTSTG_LVDS_PD_11_0);
    vRegWriteFldAlign(OUTSTG_OS_8F, 3, OS_8F_LVDS_PD_21_20);      
    vRegWriteFldAlign(OUTSTG_OS_8F, 0x3ff, OS_8F_LVDS_PD_9_0);      
    
    // power down analog
    vRegWriteFldAlign(OSTG_LVDS_01, 0xfff, LVDS_01_RG_LVDS_APD);
    //power down bandgap and all reference circuit
    vRegWriteFldAlign(OSTG_LVDS_01, 1, LVDS_01_RG_LVDS_BIASA_PD);
    // analog reset
    vRegWriteFldAlign(OSTG_LVDS_01, 1, LVDS_01_RG_LVDS_ARST);
    
#ifdef CC_EXTERNAL_LVDS_CHIP 
    IMpac3P_SetLvds(0);
#endif

}

/**
 * @brief vDrvLVDSConfig

 * Power Down LVDS

 * @param  void
 * @retval void
 */
void vDrvLVDSConfig(void)
{   
    // set lvds format
    _vDrvLVDSConfigFormat();

    if (LVDS_DISP_ODD_SW == LVDS_ODD_SW_ON)
    {
        vRegWriteFldAlign(OSTG_LVDS_00, 1, LVDS_00_ODD_INV);
    }
    else
    {
        vRegWriteFldAlign(OSTG_LVDS_00, 0, LVDS_00_ODD_INV);
    }

    // Lvds control : NOT USED in 536x
    _vDrvLVDSSetPortMap();
   
#if 0
    vRegWriteFldAlign(OUTSTG_OS_E0, 1, OS_E0_SPREAD_INIT);
    #ifdef CC_COPLAT_MT82
    vUtDelay2us(50);
    #else
    HAL_Delay_us(100);
    #endif
    vRegWriteFldAlign(OUTSTG_OS_E0, 0, OS_E0_SPREAD_INIT);
#endif    
}

void vDrvLVDS7To4FifoEnable(void)
{
    vRegWriteFldAlign(OUTSTG_OS_29, 1, OS_29_FAST_BLANK);
    vRegWriteFldAlign(OUTSTG_OS_81, 0, OS_81_REG_LVDS_E);
    vRegWriteFldAlign(OUTSTG_OS_81, 1, OS_81_REG_LVDS_E);
    vRegWriteFldAlign(OUTSTG_OS_29, 0, OS_29_FAST_BLANK);
}

