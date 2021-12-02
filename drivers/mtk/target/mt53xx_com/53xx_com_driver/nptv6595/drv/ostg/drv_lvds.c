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
#include "hw_mlvds.h"
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
#include "x_ckgen.h"
#include "x_gpio.h"
#include "drv_od.h"
#include "panel.h"

// switch the source of both group b and group c from internal group b
//#define CC_LVDS_WA1


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef DRV_SUPPORT_EXTMJC
static UINT32 _u4CurrentLvdsMode = (UINT32)eLvdsVidNormal;
static UINT32 _u4LvdsColorDomain = eColorDomainRGB;
#endif

//automation
enum
{
  LVDS_PWON,
  LVDS_PWDOWN
};


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
        au4PortRotte[0] = LVDS_A_INDEPENDENT_SETTING;
        au4PortRotte[1] = LVDS_B_INDEPENDENT_SETTING;
#ifdef CC_MT5395
        au4PortRotte[2] = LVDS_C_INDEPENDENT_SETTING;
        au4PortRotte[3] = LVDS_D_INDEPENDENT_SETTING;
#endif
    }

    // PCB layout rotate
    u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
    u4LvdsCtrlAOdd = DRVCUST_PanelGet(eLvdsACtrlOdd);
    u4LvdsCtrlAEven = (u4LvdsCtrlAEven < 4)? au4PortRotte[u4LvdsCtrlAEven] : 0;
    u4LvdsCtrlAOdd = (u4LvdsCtrlAOdd < 4)? au4PortRotte[u4LvdsCtrlAOdd] : 0;   

#ifdef CC_MT5395
    u4LvdsCtrlBEven = DRVCUST_PanelGet(eLvdsBCtrlEven);
    u4LvdsCtrlBOdd = DRVCUST_PanelGet(eLvdsBCtrlOdd);
    u4LvdsCtrlBEven = (u4LvdsCtrlBEven < 4)? au4PortRotte[u4LvdsCtrlBEven] : 0;
    u4LvdsCtrlBOdd = (u4LvdsCtrlBOdd < 4)? au4PortRotte[u4LvdsCtrlBOdd] : 0;   
#endif

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
        u4LvdsCtrlAEven = (PANEL_GetControlWord() >> 10) & 0x3;
        u4LvdsCtrlAOdd = (PANEL_GetControlWord() >> 12) & 0x3;
        u4LvdsCtrlBEven = (PANEL_GetControlWord() >> 14) & 0x3;
        u4LvdsCtrlBOdd = (PANEL_GetControlWord() >> 16) & 0x3;
    }
    #endif

/*
    vRegWriteFldAlign(OUTSTG_OS_81, u4LvdsCtrlAEven, OS_81_LVDS_A_CTRL);
    vRegWriteFldAlign(OUTSTG_OS_81, u4LvdsCtrlAOdd,  OS_81_LVDS_B_CTRL);
    #ifdef CC_LVDS_WA1
    vRegWriteFldAlign(OUTSTG_OS_81, u4LvdsCtrlAOdd, OS_81_LVDS_C_CTRL);
    #else
    vRegWriteFldAlign(OUTSTG_OS_81, u4LvdsCtrlBEven, OS_81_LVDS_C_CTRL);
    #endif
    vRegWriteFldAlign(OUTSTG_OS_81, u4LvdsCtrlBOdd,  OS_81_LVDS_D_CTRL);
*/    
#ifdef CC_MT5395
    // A/C channel for PORTA, B/D channel for PORTB
    vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlAEven, RG_A_SW);
    vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlBEven, RG_B_SW);
    vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlAOdd,  RG_C_SW);
    vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlBOdd,  RG_D_SW);
#else
    vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlAEven, RG_A_SW);
    vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlAOdd,  RG_B_SW);
    vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlBEven, RG_C_SW);
    vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlBOdd,  RG_D_SW);
#endif

}


static void _vDrvLVDSConfigFormat(void)
{
    UINT32 u4MSBSW;
    
    u4MSBSW = LVDS_DISP_FORMAT;
    #ifdef DRV_SUPPORT_EXTMJC
    if (IS_COMPANION_CHIP_ON())
    {
        if (vDrvGetCurLvdsMode() !=  eLvdsVidNormal)
        {
            u4MSBSW = LVDS_MSB_SW_OFF;
#ifdef CC_MT5395
            // DISP_R in HS
            vIO32WriteFldAlign(MLVDS_SO2, 1, RG_OSD_3D_IN_HS);
#endif
        }
        else
        {
#ifdef CC_MT5395
            vIO32WriteFldAlign(MLVDS_SO2, 0, RG_OSD_3D_IN_HS);
#endif
        }
    }
    #endif

    
    // set lvds format
    if (u4MSBSW == LVDS_SPECIAL_NS) // special NS
    {
        vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_NS_VESA_EN);        
        vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_SPECIAL_NS);        
    }
    else if (u4MSBSW == LVDS_MSB_SW_ON) // NS
    {
        vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_NS_VESA_EN);        
        vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_SPECIAL_NS);        
    }
    else // JEIDA
    {
        vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_NS_VESA_EN);        
        vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_SPECIAL_NS);        
    }

}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#ifdef CC_MT5395
 #define TERMINATION_1K_OHM 0 // 1: 1K ohm 0:100 ohm
#else
 #define TERMINATION_1K_OHM 1 // 1: 1K ohm 0:100 ohm
#endif

void vDrvLVDSInit(void)
{
    UINT32 u4Value;
    #ifdef CC_MT5365
    UINT32 u4QFP = 0;
    #endif
    #ifdef CC_MT5395
    UINT32 *pu4PinMap;
    #endif
	
    // set TVO(driving current) if necessary
    u4Value = PANEL_GetDrivingCurrent();
    if (u4Value <= 15)
    {
        vDrvLVDSSetDriving(u4Value);
    }
    else
    {
        #if TERMINATION_1K_OHM
        vDrvLVDSSetDriving(2); // MT5366/95 3: 1mA+2mA =3mA
        #else
        vDrvLVDSSetDriving(3); // MT5366/95 3: 1mA+1mA+2mA =4mA
        #endif
    }

    // switch to LVDS TX mode
    vIO32WriteFldAlign(MLVDS_B0, 0, RG_FIFO_SEL);                

    // bypass 6-line buffer
    vIO32WriteFldAlign(MLVDS_B0, 1, RG_6LINE_BYPASS);                

 #ifdef CC_SUPPORT_OD
 #if OD_FW_WA4
if (IS_OD_ENABLE == OD_ON)
{
     // enable 6-line buffer
    vIO32WriteFldAlign(MLVDS_B0, 0, RG_6LINE_BYPASS); 
}

 #endif
 #endif

    #ifdef CC_MT5365
    u4QFP = DRVCUST_InitGet(eFlagDDRQfp);
    if (u4QFP == 0)
    {
        // PN swap for Port B pin2,4,6
        vIO32WriteFldAlign(MLVDS_T12, 0x54, RG_TOP_RLV_PN);                

        // pin swap
        vIO32WriteFldMulti(MLVDS_T1, P_Fld(4, RG_LLV6_SEL)|P_Fld(3, RG_LLV5_SEL)
                                            |P_Fld(1, RG_LLV2_SEL)|P_Fld(2, RG_LLV1_SEL)|P_Fld(0, RG_LLV0_SEL));
        vIO32WriteFldMulti(MLVDS_T2, P_Fld(4, RG_LLV6_SEL)|P_Fld(3, RG_LLV4_SEL)
                                            |P_Fld(2, RG_LLV2_SEL)|P_Fld(0, RG_LLV1_SEL)|P_Fld(1, RG_LLV0_SEL));       
    }
    else    // QFP
    {
        // pin swap
        vIO32WriteFldMulti(MLVDS_T1, P_Fld(4, RG_LLV5_SEL)|P_Fld(3, RG_LLV4_SEL)
                                            |P_Fld(2, RG_LLV2_SEL)|P_Fld(1, RG_LLV1_SEL)|P_Fld(0, RG_LLV0_SEL));
        vIO32WriteFldMulti(MLVDS_T2, P_Fld(4, RG_LLV5_SEL)|P_Fld(3, RG_LLV4_SEL)
                                            |P_Fld(2, RG_LLV2_SEL)|P_Fld(1, RG_LLV1_SEL)|P_Fld(0, RG_LLV0_SEL));       
    }    
    #endif

    //bypass LVDS input buffer since the clock source is the same
    if (((BSP_GetIcVersion() >= IC_VER_5365_AB) && IS_IC_5365()) ||
        ((BSP_GetIcVersion() >= IC_VER_5395_AB) && IS_IC_5395()) )
    {
        // it can avoid vsync reset makes data improper
        vIO32WriteFldAlign(MLVDS_B5, 1, RG_IBUF_BYPASS);
    }

    #ifdef CC_MT5395
    if(NULL == (pu4PinMap = (UINT32*)DRVCUST_PanelGet(ePanelLvds10bitPinMap)))
    {
        // pin swap AE1/AE2
        vIO32WriteFldMulti(MLVDS_T1, P_Fld(6, RG_LLV7_SEL)|P_Fld(7, RG_LLV6_SEL)|P_Fld(5, RG_LLV5_SEL)|P_Fld(4, RG_LLV4_SEL)|P_Fld(3, RG_LLV3_SEL)|P_Fld(2, RG_LLV2_SEL)|P_Fld(1, RG_LLV1_SEL)|P_Fld(0, RG_LLV0_SEL));
    }
    else
    {
        // remap data pairs
        vIO32WriteFldMulti(MLVDS_T1, P_Fld(pu4PinMap[6], RG_LLV7_SEL)|P_Fld(pu4PinMap[7], RG_LLV6_SEL)|P_Fld(pu4PinMap[5], RG_LLV5_SEL)|P_Fld(pu4PinMap[4], RG_LLV4_SEL)|P_Fld(pu4PinMap[3], RG_LLV3_SEL)|P_Fld(pu4PinMap[2], RG_LLV2_SEL)|P_Fld(pu4PinMap[1], RG_LLV1_SEL)|P_Fld(pu4PinMap[0], RG_LLV0_SEL));
        vIO32WriteFldMulti(MLVDS_T3, P_Fld(pu4PinMap[11], RG_LLV_CK1_SEL)|P_Fld(pu4PinMap[10], RG_LLV_CK0_SEL)|P_Fld(pu4PinMap[9], RG_LLV9_SEL)|P_Fld(pu4PinMap[8], RG_LLV8_SEL));
        vIO32WriteFldMulti(MLVDS_T2, P_Fld(pu4PinMap[7], RG_RLV7_SEL)|P_Fld(pu4PinMap[6], RG_RLV6_SEL)|P_Fld(pu4PinMap[5], RG_RLV5_SEL)|P_Fld(pu4PinMap[4], RG_RLV4_SEL)|P_Fld(pu4PinMap[3], RG_RLV3_SEL)|P_Fld(pu4PinMap[2], RG_RLV2_SEL)|P_Fld(pu4PinMap[1], RG_RLV1_SEL)|P_Fld(pu4PinMap[0], RG_RLV0_SEL));
        vIO32WriteFldMulti(MLVDS_T3, P_Fld(pu4PinMap[11], RG_RLV_CK1_SEL)|P_Fld(pu4PinMap[10], RG_RLV_CK0_SEL)|P_Fld(pu4PinMap[9], RG_RLV9_SEL)|P_Fld(pu4PinMap[8], RG_RLV8_SEL));
    }
    #endif

    #ifdef CC_MT5395
    vIO32WriteFldMulti(LVDSTX_ANA0, P_Fld(3, LVDSA0_TVCM)|P_Fld(3, LVDSA0_NSRC)|P_Fld(4, LVDSA0_PSRC));
    vIO32WriteFldMulti(LVDSTX_ANA4, P_Fld(3, LVDSB4_TVCM)|P_Fld(3, LVDSB4_NSRC)|P_Fld(4, LVDSB4_PSRC));

    #if TERMINATION_1K_OHM
    vIO32WriteFldAlign(LVDSTX_ANA3, 2, LVDSA3_TERM_EN);
    vIO32WriteFldAlign(LVDSTX_ANA7, 2, LVDSB7_TERM_EN);
    #else
    vIO32WriteFldAlign(LVDSTX_ANA3, 1, LVDSA3_TERM_EN);
    vIO32WriteFldAlign(LVDSTX_ANA7, 1, LVDSB7_TERM_EN);
    #endif
    #else
    vRegWriteFldMulti(OSTG_LVDS_04, P_Fld(3, LVDS_04_RG_LVDS_ATVCM)|P_Fld(3, LVDS_04_RG_LVDS_ANSRC)|P_Fld(4, LVDS_04_RG_LVDS_APSRC));
    vRegWriteFldMulti(OSTG_LVDS_05, P_Fld(4, LVDS_05_RG_LVDS_APSRC_O5)|P_Fld(4, LVDS_05_RG_LVDS_APSRC_E5));    
    
    vRegWriteFldMulti(OSTG_LVDS_01, P_Fld(2, LVDS_01_RG_LVDS_ATERM_EN)|P_Fld(2, LVDS_01_RG_LVDS_ATERM_EN_O5)|P_Fld(2, LVDS_01_RG_LVDS_ATERM_EN_E5));
    #endif
    
    // enable vsync output for dual LVDS channels
    vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_VS_SEL);                
    // enable hsync output for dual LVDS channels
    vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_HS_SEL);                
    // enable data_en output for dual LVDS channels
    vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_DE_SEL);                
    
        
    //LVDS 7->4 FIFO enable 
    #ifdef CC_MTK_LOADER
    vDrvLVDS7To4FifoEnable();
    #else
//    vRegWriteFldAlign(OUTSTG_OS_81, 1, OS_81_REG_LVDS_E);
    vIO32WriteFldAlign(MLVDS_SO2, 1, RG_LVDS_74FIFO_EN);                
    #endif

    // set top pinmux to LDVS out
    vIO32WriteFldAlign(CKGEN_PMUX_MISC, 0, FLD_RG_ATE_PINMUX_EN);
    vIO32WriteFldAlign(CKGEN_PMUX_MISC, 0, FLD_RG_SYS_PINMUX_EN);
    
    // enable LVDS PAD
    #ifdef CC_MT5365
     vIO32WriteFldAlign(CKGEN_PMUX_MISC, 1, FLD_RG_LVDS_PAD_EN);
    #endif
    #ifdef CC_MT5395
     vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PMUX3_LVDS_EN);
    #endif

    
}

void vDrvLVDSSetDriving(UINT8 bDriving)
{    
    //mt5363/87 bit [0] 1 mA bit [1] 1 mA bit [2] 1 mA bit [3] 2 mA
    //mt5365/95 bit [0] 1 mA bit [1] 2 mA bit [2] 4 mA bit [3] 4 mA
    bDriving &= 0xf;  //0~15
    #ifdef CC_MT5395
    vIO32WriteFldAlign(LVDSTX_ANA0, bDriving, LVDSA0_TVO);
    vIO32WriteFldAlign(LVDSTX_ANA4, bDriving, LVDSB4_TVO);    
    #else
    vRegWriteFldAlign(OSTG_LVDS_04, bDriving, LVDS_04_RG_LVDS_ATVO);
    #endif
}

UINT8 vDrvLVDSGetDriving(void)
{
    #ifdef CC_MT5395
    return IO32ReadFldAlign(LVDSTX_ANA0,  LVDSA0_TVO);
    #else
    return RegReadFldAlign(OSTG_LVDS_04,  LVDS_04_RG_LVDS_ATVO);
    #endif
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
    UINT32 u48BitMode;


    if (((LVDS_MODE_T)u4Mode == eLvdsVidYuv422TwoPort8Bit)
        || ((LVDS_MODE_T)u4Mode == eLvdsVidYuv422OnePort8Bit)
        || ((LVDS_MODE_T)u4Mode == eLvdsVidRgb444OnePort8Bit))
    {
        u48BitMode = 1;
    }
    else
    {
        u48BitMode = 0;
    }
    
    // select one mode
    switch ((LVDS_MODE_T)u4Mode)
    {
    case eLvdsVidNormal:
        // Back to the initial setting and return
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_YUV2YC_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_LPF_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_C_LINE_EXT);
        vIO32WriteFldMulti(MLVDS_SO0, P_Fld(0, MLVDS_SO0_RG_MERGE_OSD)|P_Fld(0, MLVDS_SO0_RG_2CH_MERGE)
                                            |P_Fld(0, MLVDS_SO0_RG_RGB_444_MERGE)|P_Fld(u48BitMode, MLVDS_SO0_RG_8BIT_DUAL));
        vDrvVOPLLSet();
        break;


    case eLvdsVidYuv422TwoPort:
    case eLvdsVidYuv422TwoPort8Bit:
        // enable Tx mode 3
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_YUV2YC_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_LPF_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_C_LINE_EXT);
        vIO32WriteFldMulti(MLVDS_SO0, P_Fld(1, MLVDS_SO0_RG_MERGE_OSD)|P_Fld(1, MLVDS_SO0_RG_2CH_MERGE)
                                            |P_Fld(0, MLVDS_SO0_RG_RGB_444_MERGE)|P_Fld(u48BitMode, MLVDS_SO0_RG_8BIT_DUAL));
        #if 0
        #ifdef DRV_SUPPORT_EXTMJC_MODE7
        if (DRVCUST_InitGet(eFlagDDRQfp)) //mt5387 condition
        {
            vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_8BIT_DUAL);
        }
        #endif
        #endif
        vDrvVOPLLSet();
        break;

    case eLvdsVidRgb444TwoPort:  
        // enable Tx mode 4
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_YUV2YC_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_LPF_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_C_LINE_EXT);
        vIO32WriteFldMulti(MLVDS_SO0, P_Fld(1, MLVDS_SO0_RG_MERGE_OSD)|P_Fld(1, MLVDS_SO0_RG_2CH_MERGE)
                                            |P_Fld(1, MLVDS_SO0_RG_RGB_444_MERGE)|P_Fld(u48BitMode, MLVDS_SO0_RG_8BIT_DUAL));
        vDrvVOPLLSet();
        break;

    case eLvdsVidYuv422OnePort:
    case eLvdsVidYuv422OnePort8Bit:
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_YUV2YC_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_LPF_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_C_LINE_EXT);
        vIO32WriteFldMulti(MLVDS_SO0, P_Fld(1, MLVDS_SO0_RG_MERGE_OSD)|P_Fld(0, MLVDS_SO0_RG_2CH_MERGE)
                                            |P_Fld(0, MLVDS_SO0_RG_RGB_444_MERGE)|P_Fld(u48BitMode, MLVDS_SO0_RG_8BIT_DUAL));
        vDrvVOPLLSet();
        break;

    case eLvdsVidRgb444OnePort:
    case eLvdsVidRgb444OnePort8Bit:
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_YUV2YC_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_LPF_EN);
        vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_C_LINE_EXT);
        vIO32WriteFldMulti(MLVDS_SO0, P_Fld(1, MLVDS_SO0_RG_MERGE_OSD)|P_Fld(0, MLVDS_SO0_RG_2CH_MERGE)
                                            |P_Fld(1, MLVDS_SO0_RG_RGB_444_MERGE)|P_Fld(u48BitMode, MLVDS_SO0_RG_8BIT_DUAL));
        vDrvVOPLLSet();
        break;
    default:
        LOG(0, "Not Support LVDS Mode (%d)\n", u4Mode);
        break;
    }

    // the clock changed, reset 74Fifo
    vDrvLVDS7To4FifoEnable();

    // set lvds format
    _vDrvLVDSConfigFormat();

    // config LVDS PD, dual port, and 10bit control bits
    vDrvLVDSOn();
    
}

void vSetLVDSPadPD(UINT32 u4PDVal)
{
#ifdef CC_MT5395
    // Lvds control : 
    UINT32 u4LvdsCtrlAEven = 0;
    UINT32 u4LvdsCtrlAOdd = 1;
    UINT32 u4LvdsCtrlBEven = 2;
    UINT32 u4LvdsCtrlBOdd = 3;
    UINT32 au4PortRotte[4] = {0, 1, 2, 3};
    UINT32 au4PDCh[4];
    UINT32 u4TmpCnt;
    UINT32 *pu4PinMap;
    UINT32 u4TmpPDCh;
    UINT32 u4TmpCnt2;
    UINT8 u1PinCh;
    const UINT32 au4PadRemapTbl[12]={0,1,2,5,4,0,1,2,5,4,3/*EvenCLK*/,3/*OddCLK*/};

    // panel specific port rotate
    if (LVDS_USE_INDEPENDENT_SETTING)
    {
        au4PortRotte[0] = LVDS_A_INDEPENDENT_SETTING;
        au4PortRotte[1] = LVDS_B_INDEPENDENT_SETTING;
        au4PortRotte[2] = LVDS_C_INDEPENDENT_SETTING;
        au4PortRotte[3] = LVDS_D_INDEPENDENT_SETTING;
    }

    // PCB layout rotate
    u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
    u4LvdsCtrlAOdd = DRVCUST_PanelGet(eLvdsACtrlOdd);
    u4LvdsCtrlAEven = (u4LvdsCtrlAEven < 4)? au4PortRotte[u4LvdsCtrlAEven] : 0;
    u4LvdsCtrlAOdd = (u4LvdsCtrlAOdd < 4)? au4PortRotte[u4LvdsCtrlAOdd] : 0;   

    u4LvdsCtrlBEven = DRVCUST_PanelGet(eLvdsBCtrlEven);
    u4LvdsCtrlBOdd = DRVCUST_PanelGet(eLvdsBCtrlOdd);
    u4LvdsCtrlBEven = (u4LvdsCtrlBEven < 4)? au4PortRotte[u4LvdsCtrlBEven] : 0;
    u4LvdsCtrlBOdd = (u4LvdsCtrlBOdd < 4)? au4PortRotte[u4LvdsCtrlBOdd] : 0;   

    pu4PinMap = (UINT32*)DRVCUST_PanelGet(ePanelLvds10bitPinMap);

    // u4PDVal: BO/BE/AO/AE (3/2/1/0)
    for(u4TmpCnt=0;u4TmpCnt<4;u4TmpCnt++)
    {
        au4PDCh[u4TmpCnt] = ((u4PDVal >> (6*u4TmpCnt)) & 0x3f);
        if(NULL != pu4PinMap)
        {
            u4TmpPDCh = 0; //au4PDCh[u4TmpCnt]; 
            // remap the analog pad channel
            for(u4TmpCnt2=0;u4TmpCnt2<6;u4TmpCnt2++)
            {
                if(u4TmpCnt2 < 5)
                {   //data channel
                    u1PinCh = u4TmpCnt2+5*(u4TmpCnt%2);
                }
                else
                {   //clk channel
                    u1PinCh = 10+(u4TmpCnt%2);
                }
                if(0 != (au4PDCh[u4TmpCnt] & (1<< au4PadRemapTbl[u1PinCh])))
                {
                    //power down this channel
                    u4TmpPDCh |= (1 << au4PadRemapTbl[pu4PinMap[u1PinCh]]);
                }  
            }
            au4PDCh[u4TmpCnt] = u4TmpPDCh;
        }
    }
    u4PDVal = (au4PDCh[0] << (6*u4LvdsCtrlAEven)) |
              (au4PDCh[1] << (6*u4LvdsCtrlAOdd)) |
              (au4PDCh[2] << (6*u4LvdsCtrlBEven)) |
              (au4PDCh[3] << (6*u4LvdsCtrlBOdd)) ;

    vIO32WriteFldAlign(LVDSTX_ANA0, (u4PDVal & 0xfff), LVDSA0_PD);
    vIO32WriteFldAlign(LVDSTX_ANA4, ((u4PDVal>>12) & 0xfff), LVDSB4_PD);
#else
    vRegWriteFldAlign(OSTG_LVDS_04, u4PDVal, LVDS_04_RG_LVDS_APD);  // Analog related setting
#endif
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
    UINT32 u4PDValue;
    UINT32 u4TwoChMerge;
    #ifdef CC_MT5365
    UINT32 u4QFP;
    u4QFP = DRVCUST_InitGet(eFlagDDRQfp);
    #endif
	  
    //power up bandgap and all reference circuit
    #ifdef CC_MT5395
    vIO32WriteFldAlign(LVDSTX_ANA0, 0, LVDSA0_BIAS_PD);
    vIO32WriteFldAlign(LVDSTX_ANA4, 0, LVDSB4_BIAS_PD);    
    #else
    vRegWriteFldAlign(OSTG_LVDS_04, 0, LVDS_04_RG_LVDS_BIASA_PD);
    vRegWriteFldMulti(OSTG_LVDS_01, P_Fld(0, LVDS_01_RG_LVDS_BIASA_O5_PD)|P_Fld(0, LVDS_01_RG_LVDS_BIASA_E5_PD));
    #endif
    
    u4DispBit = DISP_BIT;
    u4LVDSPort = LVDS_OUTPUT_PORT;
    u4TwoChMerge = (LVDS_OUTPUT_PORT == DUAL_PORT)? 1: 0;
    #ifdef DRV_SUPPORT_EXTMJC
    if (IS_COMPANION_CHIP_ON())
    {
        if (vDrvGetCurLvdsMode() !=  eLvdsVidNormal)        
        {
            u4DispBit = DISP_30BIT;
            u4LVDSPort = DUAL_PORT;

            if ((vDrvGetCurLvdsMode() == eLvdsVidYuv422TwoPort8Bit) || (vDrvGetCurLvdsMode() == eLvdsVidYuv422TwoPort) || (vDrvGetCurLvdsMode() == eLvdsVidRgb444TwoPort))
            {
                u4TwoChMerge = 1;
            }
            else if ((vDrvGetCurLvdsMode() == eLvdsVidYuv422OnePort) || (vDrvGetCurLvdsMode() == eLvdsVidRgb444OnePort) || (vDrvGetCurLvdsMode() == eLvdsVidYuv422OnePort8Bit) || (vDrvGetCurLvdsMode() == eLvdsVidRgb444OnePort8Bit))
            {
                u4TwoChMerge = 0;
            }
        }
    }
    #endif
    
    switch (u4DispBit)
    {
        case DISP_30BIT:
            // enable LVDS 10-bit
            vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_10B_EN);                
#ifdef CC_MT5395
            if(u4LVDSPort == FOUR_PORT)
            {
                vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_DUAL);

                vSetLVDSPadPD(0);
                vIO32WriteFldAlign(MLVDS_SO1, 0x0, RG_PD);                // Digital related setting 
            }
            else       
#endif
            if (u4LVDSPort == DUAL_PORT)
            {
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, u4TwoChMerge, MLVDS_SO0_RG_DUAL);                
                #ifdef CC_MT5365
                if (u4QFP == 0)
                {
                    u4PDValue = 0x0808;    // PD O3, E4 
                }
                else
                {
                    u4PDValue = 0x1020;   // PD O5, E5
                }
                #endif
                #ifdef CC_MT5395
                u4PDValue = 0x00fff000; // PD BO/BE
                #endif
                vSetLVDSPadPD(u4PDValue);
                vIO32WriteFldAlign(MLVDS_SO1, 0xcffc00, RG_PD);                // Digital related setting 
            }
            else  // single port
            {
                UINT32 u4LvdsCtrlAEven;

                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_DUAL);

                u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
                if (u4LvdsCtrlAEven == 1)  // power down Port A-Odd
                {
                    #ifdef CC_MT5365
                    if (u4QFP == 0)
                    {
                        u4PDValue = 0x087F;    // PD O0~O5, OCK, E4 
                    }
                    else
                    {
                        u4PDValue = 0x107F;     // PD O0~O5, OCK, E5
                    }
                    #endif
                    #ifdef CC_MT5395
                    u4PDValue = 0x00fff03f; // PD BO/BE/AO
                    #endif
                    // Analog related setting - 0: O0 ; 1: O1 ; 2: O2 ; 3: O3 ; 4: O4 ; 5: O5 ; 6: OCK ; 7: E0 ; 8: E1 ; 9: E2 ; 10: E3 ; 11: E4 ; 12: E5 ; 13: ECK
                    vSetLVDSPadPD(u4PDValue);
                }
                else if (u4LvdsCtrlAEven == 0) // power down Port A-Even
                {
                    #ifdef CC_MT5365
                    if (u4QFP == 0)
                    {
                        u4PDValue = 0x3F88;    // PD E0~E5, EOCK, O3 
                    }
                    else
                    #endif
                    {
                        u4PDValue = 0x3FA0;     // PD E0~E5, ECK, O5
                    }
                    #ifdef CC_MT5395
                    u4PDValue = 0x00ffffc0; // PD BO/BE/AE
                    #endif
                    // Analog related setting - 0: O0 ; 1: O1 ; 2: O2 ; 3: O3 ; 4: O4 ; 5: O5 ; 6: OCK ; 7: E0 ; 8: E1 ; 9: E2 ; 10: E3 ; 11: E4 ; 12: E5 ; 13: ECK
                    vSetLVDSPadPD(u4PDValue);
                }
                vIO32WriteFldAlign(MLVDS_SO1, 0xefffe0, RG_PD);                // Digital related setting                 
            }
            break;

        case DISP_24BIT:
            // disable LVDS 10-bit
            vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_10B_EN);                
#ifdef CC_MT5395
            if(u4LVDSPort == FOUR_PORT)
            {
                vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_DUAL);

                vSetLVDSPadPD(0x00410410);  // PD O4, E4 
                vIO32WriteFldAlign(MLVDS_SO1, 0x084210, RG_PD);                // Digital related setting 
            }
            else       
#endif
            if (u4LVDSPort == DUAL_PORT)
            {
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_DUAL);
                #ifdef CC_MT5365
                if (u4QFP == 0)
                {
                    u4PDValue = 0x1828;    // PD O5, O3, E5, E4 
                }
                else
                {
                    u4PDValue = 0x1830;   // PD O5, O4, E5, E4
                }
                #endif
                #ifdef CC_MT5395
                u4PDValue = 0x00fff410;   // PD BO/BE/AE4/AO4
                #endif
                vSetLVDSPadPD(u4PDValue);
                vIO32WriteFldAlign(MLVDS_SO1, 0xcffe10, RG_PD);                // Digital related setting                 
            }
            else  // single port
            {
                UINT32 u4LvdsCtrlAEven;

                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_DUAL);

                u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
                if (u4LvdsCtrlAEven == 1)  // power down Port A-Odd
                {
                    #ifdef CC_MT5365
                    if (u4QFP == 0)
                    {
                        u4PDValue = 0x183F;    // PD O0~O5, OCK, E5, E4 
                    }
                    else
                    {
                        u4PDValue = 0x183F;     // PD O0~O5, OCK, E5, E4
                    }
                    #endif
                    #ifdef CC_MT5395
                    u4PDValue = 0x00fff43f;   // PD BO/BE/AO/AE4
                    #endif
                    // Analog related setting - 0: O0 ; 1: O1 ; 2: O2 ; 3: O3 ; 4: O4 ; 5: O5 ; 6: OCK ; 7: E0 ; 8: E1 ; 9: E2 ; 10: E3 ; 11: E4 ; 12: E5 ; 13: ECK
                    vSetLVDSPadPD(u4PDValue);
                }
                else if (u4LvdsCtrlAEven == 0) // power down Port A-Even
                {
                    #ifdef CC_MT5365
                    if (u4QFP == 0)
                    {
                        u4PDValue = 0x3FA8;    // PD E0~E5, ECK, O5, O3 
                    }
                    else
                    {
                        u4PDValue = 0x3FB0;     // PD E0~E5, ECK, O5, O4
                    }
                    #endif
                    #ifdef CC_MT5395
                    u4PDValue = 0x00ffffd0;   // PD BO/BE/AE/AO4
                    #endif
                    // Analog related setting - 0: O0 ; 1: O1 ; 2: O2 ; 3: O3 ; 4: O4 ; 5: O5 ; 6: OCK ; 7: E0 ; 8: E1 ; 9: E2 ; 10: E3 ; 11: E4 ; 12: E5 ; 13: ECK
                    vSetLVDSPadPD(u4PDValue);
                }
                vIO32WriteFldAlign(MLVDS_SO1, 0xeffff0, RG_PD);                // Digital related setting                 
            }
            break;

        case DISP_18BIT:
            // disable LVDS 10-bit
            vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_10B_EN);                
            if (u4LVDSPort == DUAL_PORT)
            {
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_DUAL);
                #ifdef CC_MT5395
                vSetLVDSPadPD(0x00fffC30);
                #else
                vSetLVDSPadPD(0x1C38);
                #endif
                vIO32WriteFldAlign(MLVDS_SO1, 0xcfff18, RG_PD);                // Digital related setting                 
            }
            else  // single port
            {
                UINT32 u4LvdsCtrlAEven;

                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_DUAL);
                u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
                if (u4LvdsCtrlAEven == 1)  // power down Port A-Odd
                {
                    #ifdef CC_MT5395
                    vSetLVDSPadPD(0x00fffC3F);
                    #else
                    vSetLVDSPadPD(0x1C7F);
                    #endif
                }
                else if (u4LvdsCtrlAEven == 0) // power down Port A-Even
                {
                    #ifdef CC_MT5395
                    vSetLVDSPadPD(0x00fffff0);
                    #else
                    vSetLVDSPadPD(0x3FB8);
                    #endif
                }
                vIO32WriteFldAlign(MLVDS_SO1, 0xeffff8, RG_PD);                // Digital related setting                                
            }
            break;

        default:
            break;
    }

    #ifdef CC_MT5395
    vIO32WriteFldAlign(LVDSTX_ANA0, 1, LVDSA0_RST);// analog LVDS reset
    vIO32WriteFldAlign(LVDSTX_ANA4, 1, LVDSB4_RST);// analog LVDS reset    
    #else
    vRegWriteFldAlign(OSTG_LVDS_04, 1, LVDS_04_RG_LVDS_ARST);// analog LVDS reset
    #endif

    #ifdef CC_COPLAT_MT82
    vUtDelay2us(5);
    #else
    HAL_Delay_us(10);
    #endif

    #ifdef CC_MT5395
    vIO32WriteFldAlign(LVDSTX_ANA0, 0, LVDSA0_RST);// analog LVDS normal
    vIO32WriteFldAlign(LVDSTX_ANA4, 0, LVDSB4_RST);// analog LVDS normal    
    #else
    vRegWriteFldAlign(OSTG_LVDS_04, 0, LVDS_04_RG_LVDS_ARST);// analog LVDS normal
    #endif

    #ifdef CC_EXTERNAL_LVDS_CHIP 
    IMpac3P_SetLvds(1);
    #endif
#ifdef CC_EXTMJC_WA	
	UINT8 u1MsgErr;
	drv_extmjc_reset_rx_tx(100,&u1MsgErr);
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
    vIO32WriteFldAlign(MLVDS_SO1, 0xffffff, RG_PD);                                           

    #ifdef CC_MT5395
    // power down analog
    vIO32WriteFldAlign(LVDSTX_ANA0, 0xFFF, LVDSA0_PD);
    vIO32WriteFldAlign(LVDSTX_ANA4, 0xFFF, LVDSB4_PD);
    
    //power down bandgap and all reference circuit
    vIO32WriteFldAlign(LVDSTX_ANA0, 1, LVDSA0_BIAS_PD);
    vIO32WriteFldAlign(LVDSTX_ANA4, 1, LVDSB4_BIAS_PD);    
    
    // analog reset
    vIO32WriteFldAlign(LVDSTX_ANA0, 1, LVDSA0_RST);
    vIO32WriteFldAlign(LVDSTX_ANA4, 1, LVDSB4_RST);    
    #else
    // power down analog
    vRegWriteFldAlign(OSTG_LVDS_04, 0x3FFF, LVDS_04_RG_LVDS_APD);
    //power down bandgap and all reference circuit
    vRegWriteFldAlign(OSTG_LVDS_04, 1, LVDS_04_RG_LVDS_BIASA_PD);
    vRegWriteFldMulti(OSTG_LVDS_01, P_Fld(1, LVDS_01_RG_LVDS_BIASA_O5_PD)|P_Fld(1, LVDS_01_RG_LVDS_BIASA_E5_PD));
    // analog reset
    vRegWriteFldAlign(OSTG_LVDS_04, 1, LVDS_04_RG_LVDS_ARST);
    #endif
    
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
        vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_ODD_SW);        
    }
    else
    {
        vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_ODD_SW);                
    }

    if (HSYNC_POLARITY == HSYNC_LOW)
    {
        vIO32WriteFldAlign(MLVDS_P0, 1, RG_LVDS_HSYNC_P);                
    }
    else
    {
        vIO32WriteFldAlign(MLVDS_P0, 0, RG_LVDS_HSYNC_P);                
    }

    if (VSYNC_POLARITY == VSYNC_LOW)
    {
        vIO32WriteFldAlign(MLVDS_P0, 1, RG_LVDS_VSYNC_P);                
        vIO32WriteFldAlign(MLVDS_P0, 1, RG_LVDS_IBUF_RST_SEL);                
    }
    else
    {
        vIO32WriteFldAlign(MLVDS_P0, 0, RG_LVDS_VSYNC_P);                
        vIO32WriteFldAlign(MLVDS_P0, 0, RG_LVDS_IBUF_RST_SEL);                
    }


    // Lvds port swap control
    _vDrvLVDSSetPortMap();
   
}

void vDrvLVDS7To4FifoEnable(void)
{
    vRegWriteFldAlign(OUTSTG_OS_29, 1, OS_29_FAST_BLANK);
    vIO32WriteFldAlign(MLVDS_SO2, 0, RG_LVDS_74FIFO_EN);                
    vIO32WriteFldAlign(MLVDS_SO2, 1, RG_LVDS_74FIFO_EN);                
    
    vIO32WriteFldAlign(MLVDS_T11, 1, RG_ANA_INTF_FIFO_RST);   
    vIO32WriteFldAlign(MLVDS_T11, 0, RG_ANA_INTF_FIFO_RST);   

    vRegWriteFldAlign(OUTSTG_OS_29, 0, OS_29_FAST_BLANK);
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//automation cli 
void LVDS_ATERM_ATVO_Setting(UINT8 mode)
{
  #ifdef CC_MT5365
		   if(mode==1)
		   {
			vRegWriteFldAlign(OSTG_LVDS_01,mode,LVDS_01_RG_LVDS_ATERM_EN);
			//Sleep(200);
			vRegWriteFldAlign(OSTG_LVDS_04,3,LVDS_04_RG_LVDS_ATVO);
			//Sleep(200);
		   }
		   else if(mode==2)
		   {
			vRegWriteFldAlign(OSTG_LVDS_01,mode,LVDS_01_RG_LVDS_ATERM_EN);
			//Sleep(200);
			vRegWriteFldAlign(OSTG_LVDS_04,2,LVDS_04_RG_LVDS_ATVO);
			//Sleep(200);
		   }
  #elif  defined(CC_MT5395)
  //#else
		   if(mode==1)
		   {
			vIO32WriteFldAlign(LVDSTX_ANA3,mode,LVDSA3_TERM_EN);
			vIO32WriteFldAlign(LVDSTX_ANA7,mode,LVDSB7_TERM_EN);
			//Sleep(200);
			vIO32WriteFldAlign(LVDSTX_ANA0,3,LVDSA0_TVO);
			vIO32WriteFldAlign(LVDSTX_ANA4,3,LVDSB4_TVO);
			//Sleep(200);
		   }
		   else if(mode==2)
		   {
			vIO32WriteFldAlign(LVDSTX_ANA3,mode,LVDSA3_TERM_EN);
			vIO32WriteFldAlign(LVDSTX_ANA7,mode,LVDSB7_TERM_EN);

			//Sleep(200);
			vIO32WriteFldAlign(LVDSTX_ANA0,2,LVDSA0_TVO);
			vIO32WriteFldAlign(LVDSTX_ANA4,2,LVDSB4_TVO);
			//Sleep(200);
		   }	
		   else if(mode==3)
		   {
			vIO32WriteFldAlign(LVDSTX_ANA3,mode,LVDSA3_TERM_EN);
			vIO32WriteFldAlign(LVDSTX_ANA7,mode,LVDSB7_TERM_EN);

			//Sleep(200);
			vIO32WriteFldAlign(LVDSTX_ANA0,3,LVDSA0_TVO);
			vIO32WriteFldAlign(LVDSTX_ANA4,3,LVDSB4_TVO);
			//Sleep(200);
		   }			   
   #endif	


}
////////////////////////////////////////////////
void LVDS_ATERM_ATVO_Restore(UINT8 ATERN, UINT8 ATVO)
{
  #ifdef CC_MT5365
        vRegWriteFldAlign(OSTG_LVDS_01,ATERN,LVDS_01_RG_LVDS_ATERM_EN);
        //Sleep(200);
        vRegWriteFldAlign(OSTG_LVDS_04,ATVO,LVDS_04_RG_LVDS_ATVO);
  #elif defined(CC_MT5395)
        vIO32WriteFldAlign(LVDSTX_ANA3,ATERN,LVDSA3_TERM_EN);
        vIO32WriteFldAlign(LVDSTX_ANA7,ATERN,LVDSB7_TERM_EN);
        //Sleep(200);
        vIO32WriteFldAlign(LVDSTX_ANA0,ATVO,LVDSA0_TVO);
        vIO32WriteFldAlign(LVDSTX_ANA4,ATVO,LVDSB4_TVO);    
  #endif
}
////////////////////////////////////////////////
void LVDS_ATVO_Set(UINT8 mode)
{
  #ifdef CC_MT5365
        vRegWriteFldAlign(OSTG_LVDS_04,mode,LVDS_04_RG_LVDS_ATVO);
  #elif defined(CC_MT5395)
        vIO32WriteFldAlign(LVDSTX_ANA0,mode,LVDSA0_TVO);
		vIO32WriteFldAlign(LVDSTX_ANA4,mode,LVDSB4_TVO);
  #endif
}
////////////////////////////////////////////////
void LVDS_APSRC_Set(UINT8 mode)
{
  #ifdef CC_MT5365
        vRegWriteFldAlign(OSTG_LVDS_04,mode,LVDS_04_RG_LVDS_APSRC);
  #elif defined(CC_MT5395)
        vIO32WriteFldAlign(LVDSTX_ANA0,mode,LVDSA0_PSRC);
		vIO32WriteFldAlign(LVDSTX_ANA4,mode,LVDSB4_PSRC);
  #endif
}
////////////////////////////////////////////////
void LVDS_ANSRC_Set(UINT8 mode)
{
  #ifdef CC_MT5365
        vRegWriteFldAlign(OSTG_LVDS_04,mode,LVDS_04_RG_LVDS_ANSRC);
  #elif defined(CC_MT5395)
        vIO32WriteFldAlign(LVDSTX_ANA0,mode,LVDSA0_NSRC);
        vIO32WriteFldAlign(LVDSTX_ANA4,mode,LVDSB4_NSRC);
  #endif
}
////////////////////////////////////////////////
void LVDS_PADPD_Set(UINT8 mode)
{
  #ifdef CC_MT5365
    if(mode == LVDS_PWDOWN)
	{
	 vRegWriteFldAlign(OSTG_LVDS_04,0x3FFF,LVDS_04_RG_LVDS_APD);
	}
	else
	{
	 vRegWriteFldAlign(OSTG_LVDS_04,0,LVDS_04_RG_LVDS_APD);
	}
  #elif defined(CC_MT5395)
    if(mode == LVDS_PWDOWN)
    {
 	 vIO32WriteFldAlign(LVDSTX_ANA0,0xFFF,LVDSA0_PD);
 	 vIO32WriteFldAlign(LVDSTX_ANA4,0xFFF,LVDSB4_PD);
    }
    else
   	{
 	 vIO32WriteFldAlign(LVDSTX_ANA0,0,LVDSA0_PD);
	 vIO32WriteFldAlign(LVDSTX_ANA4,0,LVDSB4_PD);
    }
  #endif
}
////////////////////////////////////////////////
void LVDS_RESET_Set(UINT8 mode)
{
  #ifdef CC_MT5365
        vRegWriteFldAlign(OSTG_LVDS_04,mode,LVDS_04_RG_LVDS_ARST);
  #elif defined(CC_MT5395)
        vIO32WriteFldAlign(LVDSTX_ANA0,mode,LVDSA0_RST);
	    vIO32WriteFldAlign(LVDSTX_ANA4,mode,LVDSB4_RST);
  #endif
}
////////////////////////////////////////////////
void MLVDS_LS_Set(UINT8 mode)
{
   if (mode==0)
      {
   #ifdef CC_MT5365
       //vRegWriteFldMulti(OSTG_LVDS_04, P_Fld(3, LVDS_04_RG_LVDS_ANSRC)|P_Fld(4, LVDS_04_RG_LVDS_APSRC));
       vRegWriteFldAlign(OSTG_LVDS_04, 3, LVDS_04_RG_LVDS_ANSRC);
       vRegWriteFldAlign(OSTG_LVDS_04, 4, LVDS_04_RG_LVDS_APSRC);
   #elif defined(CC_MT5395)
       vIO32WriteFldMulti(LVDSTX_ANA0, P_Fld(3, LVDSA0_NSRC)|P_Fld(4, LVDSA0_PSRC));
       vIO32WriteFldMulti(LVDSTX_ANA4, P_Fld(3, LVDSB4_NSRC)|P_Fld(4, LVDSB4_PSRC));
   #endif
      } 
      else
      {
   #ifdef CC_MT5365
       //vRegWriteFldMulti(OSTG_LVDS_04, P_Fld(0, LVDS_04_RG_LVDS_ANSRC)|P_Fld(7, LVDS_04_RG_LVDS_APSRC));
       vRegWriteFldAlign(OSTG_LVDS_04, 0, LVDS_04_RG_LVDS_ANSRC);
       vRegWriteFldAlign(OSTG_LVDS_04, 7, LVDS_04_RG_LVDS_APSRC);
   #elif defined(CC_MT5395)
       vIO32WriteFldMulti(LVDSTX_ANA0, P_Fld(0, LVDSA0_NSRC)|P_Fld(7, LVDSA0_PSRC));
       vIO32WriteFldMulti(LVDSTX_ANA4, P_Fld(0, LVDSB4_NSRC)|P_Fld(7, LVDSB4_PSRC));
   #endif
      }
}
////////////////////////////////////////////////
void MLVDS_TestMODE_Set(UINT8 mode)
{
  #if  defined(CC_MT5365)||defined(CC_MT5395)
  vIO32WriteFldAlign(MLVDS_B9,mode,RG_MLVDS_ANA_TEST_EN);
  vIO32WriteFldAlign(MLVDS_B9,0x05,RG_MLVDS_ANA_FORCE);  
  #endif
}
////////////////////////////////////////////////
void vDrvVCOPhase_SEL(UINT8 u1Phase)
{
  #ifdef CC_MT5395
	vIO32WriteFldAlign(CKGEN_VOPLLCFG_03, u1Phase, VOPLL03_VPLL_DLY_DIG);
  #endif
}
////////////////////////////////////////////////
void SET_DDDSFreeRun(void)
{
  #if  defined(CC_MT5365)||defined(CC_MT5395)
  vIO32WriteFldAlign(DDDS_00,0,DDDS_00_DISP_EN);
  vIO32WriteFldAlign(DDDS_00,0,DDDS_00_FIX_FS_DDDS_SEL);			
  vIO32WriteFldAlign(DDDS_03,0,DDDS_03_SPREAD_INIT);
  vIO32WriteFldAlign(DDDS_03,1,DDDS_03_SPREAD_INIT);
  vIO32WriteFldAlign(DDDS_03,0,DDDS_03_SPREAD_INIT);						
  #endif
}







