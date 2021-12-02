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
#include "drv_display.h"
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
#ifdef CC_MT5396
        au4PortRotte[2] = LVDS_C_INDEPENDENT_SETTING;
        au4PortRotte[3] = LVDS_D_INDEPENDENT_SETTING;
#endif
    }

    // PCB layout rotate
    u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
    u4LvdsCtrlAOdd = DRVCUST_PanelGet(eLvdsACtrlOdd);
    u4LvdsCtrlAEven = (u4LvdsCtrlAEven < 4)? au4PortRotte[u4LvdsCtrlAEven] : 0;
    u4LvdsCtrlAOdd = (u4LvdsCtrlAOdd < 4)? au4PortRotte[u4LvdsCtrlAOdd] : 0;   

#ifdef CC_MT5396
    u4LvdsCtrlBEven = DRVCUST_PanelGet(eLvdsBCtrlEven);
    u4LvdsCtrlBOdd = DRVCUST_PanelGet(eLvdsBCtrlOdd);
    u4LvdsCtrlBEven = (u4LvdsCtrlBEven < 4)? au4PortRotte[u4LvdsCtrlBEven] : 0;
    u4LvdsCtrlBOdd = (u4LvdsCtrlBOdd < 4)? au4PortRotte[u4LvdsCtrlBOdd] : 0;   
#endif

    // A/B channel for PORTA, C/D channel for PORTB    
    vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlAEven, RG_A_SW);
    vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlAOdd,  RG_B_SW);
    vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlBEven, RG_C_SW);
    vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlBOdd,  RG_D_SW);

}

void vDrvLVDSIFReorder(UINT8 val)
{
  if (val<=3)
  {
     vIO32WriteFldAlign(VINTF_P4, val,  VINTF_REORDER_FORMAIN_IN); 
  }
}

static void _vDrvLVDSPanelInterfaceConfig(void)
{

    vIO32WriteFldAlign(VINTF_B0, 0,  VINTF_6LINE_BYPASS); 
    vIO32WriteFldAlign(VINTF_P4, 1,  VINTF_REORDER_BYPASS); 	
    vIO32WriteFldAlign(VINTF_P4, wDISPLAY_WIDTH/2 ,  VINTF_REORDER_MIDDLE_PT);

    if(PANEL_IsSupportMlvds())
    {
     vIO32WriteFldAlign(VINTF_B0, 0 ,  VINTF_HV_BYPASS);
     vIO32WriteFldAlign(VINTF_B0, 0 ,  VINTF_HV_HOLD);     
     vIO32WriteFldAlign(VINTF_B0, 5,  VINTF_6LINE_LDLY);     
    }
    else
    {
     vIO32WriteFldAlign(VINTF_B0, 0 ,  VINTF_HV_BYPASS);
     vIO32WriteFldAlign(VINTF_B0, 1 ,  VINTF_HV_HOLD);     
     vIO32WriteFldAlign(VINTF_B0, 1,  VINTF_6LINE_LDLY);
    }

    if((PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)||PANEL_IsSupportMlvds()) // EPI need the hotal 6x
    {
     vIO32WriteFldAlign(VINTF_B0, 1,  VINTF_H_TOTAL_COMP);
    } 
	else
	{
 	 vIO32WriteFldAlign(VINTF_B0, 0,  VINTF_H_TOTAL_COMP);
	}
	
    if(IS_PANEL_WXGA240)
    {
     vIO32WriteFldAlign(VINTF_B0, 1,  VINTF_240P_TO_I);
	 vIO32WriteFldAlign(VINTF_B0, 0,  VINTF_PDP_EN);
    }
    else if(IS_PANEL_PDP)
    {
     vIO32WriteFldAlign(VINTF_B0, 1,  VINTF_PDP_EN);
	 vIO32WriteFldAlign(VINTF_B0, 0,  VINTF_240P_TO_I);
    }
    else
    {
     vIO32WriteFldAlign(VINTF_B0, 0,  VINTF_PDP_EN);    
     vIO32WriteFldAlign(VINTF_B0, 0,  VINTF_240P_TO_I);
    }

    // reorder setting
    if (IS_PANEL_REODER_SEQ_TO_HALF)
    {
     vIO32WriteFldAlign(VINTF_P4, 0,  VINTF_REORDER_BYPASS);    
     vIO32WriteFldAlign(VINTF_P4, REORDER_FORMAIN_OUT_HALF,  VINTF_REORDER_FORMAIN_OUT);     
    }	
#ifdef CC_SCPOS_3DTV_SUPPORT 
	else if ((LVDS_DISP_3D == LVDS_DISP_3D_SHUTTER) && (LVDS_DISP_SPLIT == LVDS_DISP_SPLIT_ON))
	{
	 vIO32WriteFldAlign(VINTF_P4, 0,  VINTF_REORDER_BYPASS);    
 	 vIO32WriteFldAlign(VINTF_P4, REORDER_FORMAIN_OUT_HALF,	VINTF_REORDER_FORMAIN_OUT); 	
	}
#endif	
    else
    {
     vIO32WriteFldAlign(VINTF_P4, REORDER_FORMAIN_OUT_SEQ,  VINTF_REORDER_FORMAIN_OUT);     
    }


    if(IS_LBRB_3D_ON)
    {
     vIO32WriteFldAlign(VINTF_B7, 1,  VINTF_LBRB_EN); 
    }
    else
    {
     vIO32WriteFldAlign(VINTF_B7, 0,  VINTF_LBRB_EN); 
    }

	// For 3D_Glasses Control - software work around
    vIO32WriteFldAlign(VINTF_P11, 3,  VINTF_LEYE_V_DLY); 		
    vIO32WriteFldAlign(VINTF_P11, 1,  VINTF_LEYE_POL); 		
}

void vTXCRCStatus(void)
{
    UINT8 u1group,Group_Cnt;
    
    vIO32WriteFldAlign(MLVDS_SO4, 60,  RG_CRC_VCNT);
	if((LVDS_OUTPUT_PORT == SINGLE_PORT))
	{
     Group_Cnt = 1;
	}
	else if((LVDS_OUTPUT_PORT == DUAL_PORT))
	{
	 Group_Cnt = 2;
	}
	else // FOUR_PORT
	{
	 Group_Cnt = 4;
	}
	
    for (u1group = 0; u1group < Group_Cnt; u1group++)
    {
     vIO32WriteFldAlign(MLVDS_SO4, u1group,  RG_CRC_SEL);     
     vIO32WriteFldAlign(MLVDS_SO4, 1,  RG_CRC_CLR); 
     x_thread_delay(10);//10mS    
     vIO32WriteFldAlign(MLVDS_SO4, 0,  RG_CRC_CLR);     
     vIO32WriteFldAlign(MLVDS_SO4, 0,  RG_CRC_START);              
     x_thread_delay(10);//10mS         
     vIO32WriteFldAlign(MLVDS_SO4, 1,  RG_CRC_START);         
     while (!(IO32ReadFldAlign(MLVDS_SO6, ST_LVDS_CRC_RDY))) { }    
     if(IO32ReadFldAlign(MLVDS_SO6, ST_LVDS_CRC_ERR))
     {
      Printf("Fail = [LVDS_%d] = %x%x\n",u1group,IO32ReadFldAlign(MLVDS_SO7, ST_LVDS_CRC_OUT_41_24),IO32ReadFldAlign(MLVDS_SO6, ST_LVDS_CRC_OUT_23_0));
	  Printf("Fail = [VGA_%d] = %x\n",u1group,IO32ReadFldAlign(MLVDS_SO5, ST_VGA_CRC_OUT));
     }
     else
     {
      Printf("OK = [LVDS_%d] = %x%x\n",u1group,IO32ReadFldAlign(MLVDS_SO7, ST_LVDS_CRC_OUT_41_24),IO32ReadFldAlign(MLVDS_SO6, ST_LVDS_CRC_OUT_23_0));
	  Printf("OK = [VGA_%d] = %x\n",u1group,IO32ReadFldAlign(MLVDS_SO5, ST_VGA_CRC_OUT));
     } 
    }

    vIO32WriteFldAlign(MLVDS_SO14, 1,  RG_LVDSRX_FIFO_EN);

	if(wDrvGetOutputVTotal()>=1)
	{
	 vIO32WriteFldAlign(MLVDS_SO15, PANEL_GetPanelHeight()-1,  RG_LVDSRX_CRC_H_END);
	} 
	vIO32WriteFldAlign(MLVDS_SO15, 1,  RG_LVDSRX_CRC_H_START);	

	vIO32WriteFldAlign(MLVDS_SO14, 1,  RG_LVDSRX_CRC_CLR);	
	x_thread_delay(10);//10mS  
	vIO32WriteFldAlign(MLVDS_SO14, 0,  RG_LVDSRX_CRC_CLR);	
	vIO32WriteFldAlign(MLVDS_SO14, 0,  RG_LVDSRX_CRC_CLR);	
	x_thread_delay(10);//10mS  
	vIO32WriteFldAlign(MLVDS_SO14, 1,  RG_LVDSRX_CRC_START);
	
	while (!(IO32ReadFldAlign(MLVDS_SO16,  LVDSRX_CRC_DONE))) { }    
	if(IO32ReadFldAlign(MLVDS_SO16, LVDSRX_CRC_FAIL))
	{
	 Printf("Fail = [FIFO LVDS] = %x\n",IO32ReadFldAlign(MLVDS_SO16, LVDSRX_CRC_CAL));
	}
	else
	{
	 Printf("OK = [FIFO LVDS] = %x\n",IO32ReadFldAlign(MLVDS_SO16, LVDSRX_CRC_CAL));
	} 

	
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

    _vDrvLVDSPanelInterfaceConfig();
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
 #define TERMINATION_1K_OHM 0 // 1: 1K ohm 0:100 ohm

void vDrvLVDSInit(void)
{
    UINT32 u4Value;
    UINT32 *pu4PinMap,*pu4PNSwap;
	
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
        vDrvLVDSSetDriving(5); // MT5396 5: 1mA+4mA =5mA
        #endif
    }

    // switch to LVDS TX mode
    vIO32WriteFldAlign(MLVDS_B0, 0, RG_FIFO_SEL);                

    // Disable 2CH architecture in MT5368/96
    vIO32WriteFldAlign(VINTF_B0, 0, VINTF_2CH_EN);
    if (IS_PANEL_PDP)
    {
    vIO32WriteFldAlign(MLVDS_B5, 1, RG_LVDS_2CH_ARCH); 
    }
    else
    {
     vIO32WriteFldAlign(MLVDS_B5, 0, RG_LVDS_2CH_ARCH); 		
    }

    //bypass LVDS input buffer since the clock source is the same
        // it can avoid vsync reset makes data improper
        vIO32WriteFldAlign(MLVDS_B5, 1, RG_IBUF_BYPASS);

    // ==Pair Swap ==
    if(NULL == (pu4PinMap = (UINT32*)DRVCUST_PanelGet(ePanelLvds10bitPinMap)))
    {
        // Port B Setting  // pair swap BE1/BE0/BO2/BO0
        vIO32WriteFldMulti(MLVDS_T2, P_Fld(7, RG_RLV7_SEL)|P_Fld(5, RG_RLV6_SEL)|P_Fld(6, RG_RLV5_SEL)|P_Fld(4, RG_RLV4_SEL)|P_Fld(3, RG_RLV3_SEL)|P_Fld(0, RG_RLV2_SEL)|P_Fld(1, RG_RLV1_SEL)|P_Fld(2, RG_RLV0_SEL));
        vIO32WriteFldMulti(MLVDS_T3, P_Fld(11, RG_RLV_CK1_SEL)|P_Fld(10, RG_RLV_CK0_SEL)|P_Fld(9, RG_RLV9_SEL)|P_Fld(8, RG_RLV8_SEL));		

	    // Port A Setting	
        vIO32WriteFldMulti(MLVDS_T1, P_Fld(7, RG_LLV7_SEL)|P_Fld(6, RG_LLV6_SEL)|P_Fld(5, RG_LLV5_SEL)|P_Fld(4, RG_LLV4_SEL)|P_Fld(3, RG_LLV3_SEL)|P_Fld(2, RG_LLV2_SEL)|P_Fld(1, RG_LLV1_SEL)|P_Fld(0, RG_LLV0_SEL));
        vIO32WriteFldMulti(MLVDS_T3, P_Fld(11, RG_LLV_CK1_SEL)|P_Fld(10, RG_LLV_CK0_SEL)|P_Fld(9, RG_LLV9_SEL)|P_Fld(8, RG_LLV8_SEL));
    }
    else
    {
        // remap data pairs
        vIO32WriteFldMulti(MLVDS_T1, P_Fld(pu4PinMap[7], RG_LLV7_SEL)|P_Fld(pu4PinMap[6], RG_LLV6_SEL)|P_Fld(pu4PinMap[5], RG_LLV5_SEL)|P_Fld(pu4PinMap[4], RG_LLV4_SEL)|P_Fld(pu4PinMap[3], RG_LLV3_SEL)|P_Fld(pu4PinMap[2], RG_LLV2_SEL)|P_Fld(pu4PinMap[1], RG_LLV1_SEL)|P_Fld(pu4PinMap[0], RG_LLV0_SEL));
        vIO32WriteFldMulti(MLVDS_T3, P_Fld(pu4PinMap[11], RG_LLV_CK1_SEL)|P_Fld(pu4PinMap[10], RG_LLV_CK0_SEL)|P_Fld(pu4PinMap[9], RG_LLV9_SEL)|P_Fld(pu4PinMap[8], RG_LLV8_SEL));
        vIO32WriteFldMulti(MLVDS_T2, P_Fld(pu4PinMap[19], RG_RLV7_SEL)|P_Fld(pu4PinMap[18], RG_RLV6_SEL)|P_Fld(pu4PinMap[17], RG_RLV5_SEL)|P_Fld(pu4PinMap[16], RG_RLV4_SEL)|P_Fld(pu4PinMap[15], RG_RLV3_SEL)|P_Fld(pu4PinMap[14], RG_RLV2_SEL)|P_Fld(pu4PinMap[13], RG_RLV1_SEL)|P_Fld(pu4PinMap[12], RG_RLV0_SEL));
        vIO32WriteFldMulti(MLVDS_T3, P_Fld(pu4PinMap[23], RG_RLV_CK1_SEL)|P_Fld(pu4PinMap[22], RG_RLV_CK0_SEL)|P_Fld(pu4PinMap[21], RG_RLV9_SEL)|P_Fld(pu4PinMap[20], RG_RLV8_SEL));
    }

	// == PN Swap ==
	// pn swap BE0(RLV5)/ BO2(RLV2)/ BO0(RLV0)
	if(NULL == (pu4PNSwap = (UINT32*)DRVCUST_PanelGet(ePanelLvds10bitPNSwap)))
	{
	 vIO32WriteFldMulti(MLVDS_T12, 
	 P_Fld(0, RG_RLVCK1_PN_SWAP)|P_Fld(0, RG_RLVCK0_PN_SWAP)|P_Fld(0, RG_RLV9_PN_SWAP)|P_Fld(0, RG_RLV8_PN_SWAP)|P_Fld(0, RG_RLV7_PN_SWAP)|P_Fld(0, RG_RLV6_PN_SWAP)|
	 P_Fld(1, RG_RLV5_PN_SWAP)|P_Fld(0, RG_RLV4_PN_SWAP)|P_Fld(0, RG_RLV3_PN_SWAP)|P_Fld(1, RG_RLV2_PN_SWAP)|P_Fld(0, RG_RLV1_PN_SWAP)|P_Fld(1, RG_RLV0_PN_SWAP)|	
	 P_Fld(0, RG_LLVCK1_PN_SWAP)|P_Fld(0, RG_LLVCK0_PN_SWAP)|P_Fld(0, RG_LLV9_PN_SWAP)|P_Fld(0, RG_LLV8_PN_SWAP)|P_Fld(0, RG_LLV7_PN_SWAP)|P_Fld(0, RG_LLV6_PN_SWAP)|
	 P_Fld(0, RG_LLV5_PN_SWAP)|P_Fld(0, RG_LLV4_PN_SWAP)|P_Fld(0, RG_LLV3_PN_SWAP)|P_Fld(0, RG_LLV2_PN_SWAP)|P_Fld(0, RG_LLV1_PN_SWAP)|P_Fld(0, RG_LLV0_PN_SWAP)
	 );
	}
	else
	{
	 vIO32WriteFldMulti(MLVDS_T12, 
	 P_Fld(pu4PNSwap[11], RG_RLVCK1_PN_SWAP)|P_Fld(pu4PNSwap[10], RG_RLVCK0_PN_SWAP)|P_Fld(pu4PNSwap[9], RG_RLV9_PN_SWAP)|P_Fld(pu4PNSwap[8], RG_RLV8_PN_SWAP)|P_Fld(pu4PNSwap[7], RG_RLV7_PN_SWAP)|P_Fld(pu4PNSwap[6], RG_RLV6_PN_SWAP)|
	 P_Fld(pu4PNSwap[5], RG_RLV5_PN_SWAP)|P_Fld(pu4PNSwap[4], RG_RLV4_PN_SWAP)|P_Fld(pu4PNSwap[3], RG_RLV3_PN_SWAP)|P_Fld(pu4PNSwap[2], RG_RLV2_PN_SWAP)|P_Fld(pu4PNSwap[1], RG_RLV1_PN_SWAP)|P_Fld(pu4PNSwap[0], RG_RLV0_PN_SWAP)|   
	 P_Fld(pu4PNSwap[23], RG_LLVCK1_PN_SWAP)|P_Fld(pu4PNSwap[22], RG_LLVCK0_PN_SWAP)|P_Fld(pu4PNSwap[21], RG_LLV9_PN_SWAP)|P_Fld(pu4PNSwap[20], RG_LLV8_PN_SWAP)|P_Fld(pu4PNSwap[19], RG_LLV7_PN_SWAP)|P_Fld(pu4PNSwap[18], RG_LLV6_PN_SWAP)|
	 P_Fld(pu4PNSwap[17], RG_LLV5_PN_SWAP)|P_Fld(pu4PNSwap[16], RG_LLV4_PN_SWAP)|P_Fld(pu4PNSwap[15], RG_LLV3_PN_SWAP)|P_Fld(pu4PNSwap[14], RG_LLV2_PN_SWAP)|P_Fld(pu4PNSwap[13], RG_LLV1_PN_SWAP)|P_Fld(pu4PNSwap[12], RG_LLV0_PN_SWAP)
	 );
	}
    #ifdef CC_MT5396
    vIO32WriteFldMulti(REG_LVDS_TX_CFG9, P_Fld(0x0B, RG_LVDSA_TVCM)|P_Fld(1, RG_LVDSA_NSRC)|P_Fld(1, RG_LVDSA_PSRC)
					|P_Fld(1, RG_LVDSA_EN)|P_Fld(0, RG_LVDSA_BIAS_ODD_PWD)|P_Fld(0, RG_LVDSA_BIAS_EVEN_PWD)
					|P_Fld(0xfff, RG_LVDSA_EXT_EN_D11));
    vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0xfff, RG_LVDSB_EXT_EN_D11);
    #else
    vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0x3fff, RG_LVDSB_EXT_EN_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG8, 0, RG_LVDSB_LDO_PWD_D11);
    #endif 					
    vIO32WriteFldMulti(REG_LVDS_TX_CFG0, P_Fld(0x0B, RG_LVDSB_TVCM)|P_Fld(3, RG_LVDSB_NSRC)|P_Fld(4, RG_LVDSB_PSRC)
					|P_Fld(1, RG_LVDSB_EN)|P_Fld(0, RG_LVDSB_BIAS_ODD_PWD)|P_Fld(0, RG_LVDSB_BIAS_EVEN_PWD));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG12, P_Fld(0, RG_LVDSA_HSN_MODE_EVEN_EN_D11)|P_Fld(0, RG_LVDSA_HSP_MODE_EVEN_EN_D11)|P_Fld(0, RG_LVDSA_HSN_MODE_ODD_EN_D11)|P_Fld(0, RG_LVDSA_HSP_MODE_ODD_EN_D11));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG10, P_Fld(4, RG_LVDSA_BIAS_SEL)|P_Fld(1, RG_LVDSA_NSRC_PRE)|P_Fld(1, RG_LVDSA_PSRC_PRE));
    vIO32WriteFldAlign(REG_LVDS_TX_CFG1, 1, RG_LVDSB_BIAS_SEL);

    vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0xFFF, RG_LVDSA_LVDS_SEL);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x0, RG_LVDSA_CLK_IQ_SEL);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG2, 0x0, RG_LVDSB_CLK_IQ_SEL);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x1, RG_LVDSA_RESYNC_LVDS_SEL_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_RESYNC_CLK_IQ_SEL_D11);
    vIO32WriteFldMulti(REG_LVDS_TX_CFG17, P_Fld(0, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11)|P_Fld(0, RG_LVDSA_LDO_EVEN_PWD_D11)|P_Fld(0, RG_LVDSA_LDO_BIAS_ODD_PWD_D11)|P_Fld(0, RG_LVDSA_LDO_ODD_PWD_D11));

/*    #if TERMINATION_1K_OHM
    vIO32WriteFldAlign(LVDSTX_ANA3, 2, LVDSA3_TERM_EN);
    vIO32WriteFldAlign(LVDSTX_ANA7, 2, LVDSB7_TERM_EN);
    #else
    vIO32WriteFldAlign(LVDSTX_ANA3, 1, LVDSA3_TERM_EN);
    vIO32WriteFldAlign(LVDSTX_ANA7, 1, LVDSB7_TERM_EN);
	#endif
*/    
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

    
    // Enable LVDS Pad
   //mt5396 no need
   // vLVDSSetPadMuxEnable();
}

#define LVDSDrivingIndex 8
PRIVATE UINT8 code bLVDSDrivingCurrent[LVDSDrivingIndex+1] =
{
   0x00, 0x00,0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E,
 
//  1mA,  1mA,  2mA, 3mA,   4mA,   5mA,   6mA,   7mA,  8mA,
};


void vDrvLVDSSetDriving(UINT8 bDriving)
{    
    //mt5363/87 bit [0] 1 mA bit [1] 1 mA bit [2] 1 mA bit [3] 2 mA
    //mt5365/95 bit [0] 1 mA bit [1] 2 mA bit [2] 4 mA bit [3] 4 mA
    //mt5396      bit [0] 05mA bit [1] 1 mA bit [2] 2 mA bit [3] 4 mA
    bDriving &= 0xf;  //0~15
    if (bDriving > LVDSDrivingIndex) bDriving = LVDSDrivingIndex;
    bDriving = bLVDSDrivingCurrent[bDriving];
    #ifdef CC_MT5396
    vIO32WriteFldAlign(REG_LVDS_TX_CFG9, bDriving, RG_LVDSA_TVO);
    #endif
    vIO32WriteFldAlign(REG_LVDS_TX_CFG0, (bDriving+1), RG_LVDSB_TVO);
}

UINT8 vDrvLVDSGetDriving(void)
{
    UINT8 bDriving;
    #ifdef CC_MT5396
    bDriving = IO32ReadFldAlign(REG_LVDS_TX_CFG9,  RG_LVDSA_TVO) /2+1;
    #else
    bDriving =( IO32ReadFldAlign(REG_LVDS_TX_CFG0,  RG_LVDSB_TVO) -1)/2+1;
    #endif
	bDriving &= 0xf;  //0~15
    return (bDriving);
}

void vDrvLVDSSetVCM(UINT8 bVCM)
{    
    bVCM &= 0xf;  //0~15
    #ifdef CC_MT5396
    vIO32WriteFldAlign(REG_LVDS_TX_CFG9, bVCM, RG_LVDSA_TVCM);
	#endif
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, bVCM, RG_LVDSB_TVCM);	
}

UINT8 vDrvLVDSGetVCM(void)
{
    UINT8 bVCM;
    bVCM = IO32ReadFldAlign(REG_LVDS_TX_CFG0,  RG_LVDSB_TVCM);
	bVCM &= 0xf;  //0~15
    return (bVCM);
}

void LVDS_SwitchDataEnable(BOOL fEnable)
{
    if (fEnable)
    {
       //OUTSTG_DEN_EN==0 : data enable
        vRegWriteFldAlign(OSTG_PTGEN_00, 0, FLD_DEN_EN);
    }
    else
    {
    	//OUTSTG_DEN_EN==1 : data disable
        vRegWriteFldAlign(OSTG_PTGEN_00, 1, FLD_DEN_EN);
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

    #ifdef CC_SCPOS_3DTV_SUPPORT
    if ((LVDS_MODE_T)u4Mode != eLvdsVidNormal) //  MLVDS_SO0_RG_RES_FLD msut be 1
    {
		UINT32 u4EnPair;
		if (DRVCUST_PanelQuery(eDISP_RLVDSEncodeLink, &u4EnPair) == 0)
		{
			u1SetDISP_REncodeEn(((UINT8)u4EnPair)|0x01); //  MLVDS_SO0_RG_RES_FLD msut be 1
		} 
		else
		{
			vIO32WriteFldAlign(MLVDS_SO2, 1, RG_OSD_3D_IN_HS);
			vIO32WriteFldAlign(MLVDS_SO2, 1, RG_OSD_3D_IN_VS);	 
		}
    }
	else
	{
	 vIO32WriteFldAlign(MLVDS_SO2, 0, RG_OSD_3D_IN_HS);
	 vIO32WriteFldAlign(MLVDS_SO2, 0, RG_OSD_3D_IN_VS);	
	}
	#endif
	
    // select one mode
    switch ((LVDS_MODE_T)u4Mode)
    {
    case eLvdsVidNormal:
        // Back to the initial setting and return
        vRegWriteFldAlign(OSTG_01, 0, REG_YUV2YC_EN);
        vRegWriteFldAlign(OSTG_01, 0, REG_LPF_EN);
        vRegWriteFldAlign(OSTG_01, 0, REG_C_LINE_EXT);
        vIO32WriteFldMulti(MLVDS_SO0, P_Fld(0, MLVDS_SO0_RG_MERGE_OSD)|P_Fld(0, MLVDS_SO0_RG_2CH_MERGE)
                                            |P_Fld(0, MLVDS_SO0_RG_RGB_444_MERGE)|P_Fld(u48BitMode, MLVDS_SO0_RG_8BIT_DUAL));
        vDrvVOPLLSet();
        break;


    case eLvdsVidYuv422TwoPort:
    case eLvdsVidYuv422TwoPort8Bit:
        // enable Tx mode 3
        vRegWriteFldAlign(OSTG_01, 1, REG_YUV2YC_EN);
        vRegWriteFldAlign(OSTG_01, 1, REG_LPF_EN);
        vRegWriteFldAlign(OSTG_01, 1, REG_C_LINE_EXT);
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
        vRegWriteFldAlign(OSTG_01, 0, REG_YUV2YC_EN);
        vRegWriteFldAlign(OSTG_01, 0, REG_LPF_EN);
        vRegWriteFldAlign(OSTG_01, 0, REG_C_LINE_EXT);
        vIO32WriteFldMulti(MLVDS_SO0, P_Fld(1, MLVDS_SO0_RG_MERGE_OSD)|P_Fld(1, MLVDS_SO0_RG_2CH_MERGE)
                                            |P_Fld(1, MLVDS_SO0_RG_RGB_444_MERGE)|P_Fld(u48BitMode, MLVDS_SO0_RG_8BIT_DUAL));
        vDrvVOPLLSet();
        break;

    case eLvdsVidYuv422OnePort:
    case eLvdsVidYuv422OnePort8Bit:
        vRegWriteFldAlign(OSTG_01, 1, REG_YUV2YC_EN);
        vRegWriteFldAlign(OSTG_01, 1, REG_LPF_EN);
        vRegWriteFldAlign(OSTG_01, 1, REG_C_LINE_EXT);
        vIO32WriteFldMulti(MLVDS_SO0, P_Fld(1, MLVDS_SO0_RG_MERGE_OSD)|P_Fld(0, MLVDS_SO0_RG_2CH_MERGE)
                                            |P_Fld(0, MLVDS_SO0_RG_RGB_444_MERGE)|P_Fld(u48BitMode, MLVDS_SO0_RG_8BIT_DUAL));
        vDrvVOPLLSet();
        break;

    case eLvdsVidRgb444OnePort:
    case eLvdsVidRgb444OnePort8Bit:
        vRegWriteFldAlign(OSTG_01, 0, REG_YUV2YC_EN);
        vRegWriteFldAlign(OSTG_01, 0, REG_LPF_EN);
        vRegWriteFldAlign(OSTG_01, 0, REG_C_LINE_EXT);
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
#ifdef CC_MT5396
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
    //const UINT32 au4PadRemapTbl[12]={0,1,2,5,4,0,1,2,5,4,3/*EvenCLK*/,3/*OddCLK*/};
    const UINT32 au4PadRemapTbl[12]={0,1,2,3,4,0,1,2,3,4,5/*EvenCLK*/,5/*OddCLK*/};

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
   vIO32WriteFldAlign(REG_LVDS_TX_CFG9, (0xFFF-u4PDVal), RG_LVDSA_EXT_EN_D11);
   vIO32WriteFldAlign(REG_LVDS_TX_CFG0, (0xFFF-(u4PDVal>>12)), RG_LVDSB_EXT_EN_D11);
   vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x1, RG_LVDSA_EXT_EN_D11_E0);
   vIO32WriteFldAlign(REG_LVDS_TX_CFG10, u4PDVal, RG_LVDSA_DRV_PWD);
   vIO32WriteFldAlign(REG_LVDS_TX_CFG1, (u4PDVal>>12), RG_LVDSB_DRV_PWD);
    //vIO32WriteFldAlign(LVDSTX_ANA4, ((u4PDVal>>12) & 0xfff), LVDSB4_PD);
 #else
   vIO32WriteFldAlign(REG_LVDS_TX_CFG0, (0x3FFF-u4PDVal), RG_LVDSB_EXT_EN_D11);
   vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0x1, RG_LVDSB_EXT_EN_D11_E0);
   vIO32WriteFldAlign(REG_LVDS_TX_CFG1, u4PDVal, RG_LVDSB_DRV_PWD);
#endif   
}

#define DEBUG_MSG_TX 0
static void vSetLVDSPadSourcePD(UINT32 u4DispBit, UINT32 u4LVDSPort)
{
	UINT8  u1EnableDataBit,u1Link;	
	UINT32 u4ClockPD, u4LinkPD, u4EnablePair, u4FinaleEnablePair;		
    UINT32 u4PadPDTbl[4]={0,1,2,3}; // {PD_A,PD_B,PD_C,PD_D}
    
    #if DEBUG_MSG_TX
    Printf("[SA7]u4DispBit = 0x%8x, u4LVDSPort = 0x%8x, \n",u4DispBit,u4LVDSPort);
    #else
    Printf("[SA7]u4DispBit = 0x%8x, u4LVDSPort = 0x%8x",u4DispBit,u4LVDSPort);    
    #endif

    u4LinkPD = 0x00; 
    u4EnablePair = 0;
    u4LinkPD = 0; 
    u4ClockPD = 0;  
	
	// Enable bit Control
	if (u4DispBit == DISP_18BIT) u1EnableDataBit = 0x18;
	else if (u4DispBit == DISP_24BIT) u1EnableDataBit = 0x10;	
   	else if (u4DispBit == DISP_30BIT) u1EnableDataBit = 0x00;		
   	else u1EnableDataBit = 0x00;
   
   #if DEBUG_MSG_TX
   Printf("u1EnableDataBit = 0x%8x\n",u1EnableDataBit);  
   #endif
                 
    for (u1Link=0; u1Link<4; u1Link++)
    {
     u4EnablePair = u4EnablePair + (u1EnableDataBit << (u1Link*5));        
    }             
    #if DEBUG_MSG_TX                   
    Printf("u4EnablePair = 0x%8x\n",u4EnablePair);
    #endif
    
   	// Power Down Link Control   	
	if (u4LVDSPort == SINGLE_PORT) 
    {
       for (u1Link=0; u1Link<4; u1Link++)
       {
         if(u4PadPDTbl[u1Link]!=0)
         {
            u4LinkPD = u4LinkPD +   (0x1F<<(u1Link*5));          
         }                             
       }                
    }
	else if (u4LVDSPort == DUAL_PORT)
	
	{
       for (u1Link=0; u1Link<4; u1Link++)
       {
         if ((u4PadPDTbl[u1Link]==2) || (u4PadPDTbl[u1Link]==3))
         {
            u4LinkPD = u4LinkPD +   (0x1F<<(u1Link*5));
         }                             
       }                  
    }             
   	else if (u4LVDSPort == FOUR_PORT) u4LinkPD = 0x00;		
   	else u4LinkPD = 0x00;   	   	

    #if DEBUG_MSG_TX
    Printf("u4EnablePair = 0x%8x\n",u4EnablePair);
    Printf("u4LinkPD = 0x%8x\n",u4LinkPD);    
    #endif
    
    // Power Down clock  path
	if (u4LVDSPort == SINGLE_PORT)
	{
       for (u1Link=0; u1Link<4; u1Link++)
       {
         if(u4PadPDTbl[u1Link]!=0)
         {
            u4ClockPD = u4ClockPD +   (0x1<<u1Link); 
         }                             
       }              
    }               
	else if (u4LVDSPort == DUAL_PORT)
	{
       for (u1Link=0; u1Link<4; u1Link++)
       {
         if ((u4PadPDTbl[u1Link]==2) || (u4PadPDTbl[u1Link]==3))
         {
            u4ClockPD = u4ClockPD +   (0x1<<u1Link);  
         }                             
       }             
    }     
   	else if (u4LVDSPort == FOUR_PORT) u4ClockPD = 0x00;		
   	else u4ClockPD = 0x00;   
    
    #if DEBUG_MSG_TX
    Printf("u1ClockPD = 0x%8x\n",u4ClockPD);
    #endif

	// Combin
    u4FinaleEnablePair = (u4LinkPD | u4EnablePair | (u4ClockPD << 20)) & 0xFFFFFF; 
    Printf("u4FinaleEnablePair = 0x%8x\n",u4FinaleEnablePair); 
	vIO32WriteFldAlign(MLVDS_SO1, u4FinaleEnablePair, RG_PD);
}     


extern UINT8 _Lvds7To4FifoReset;
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
	UINT32 u4LvdsCtrlAEven;
	  
    //A group power on sequence
    #ifdef CC_MT5396
    vIO32WriteFldMulti(REG_LVDS_TX_CFG9,P_Fld( 0, RG_LVDSA_BIAS_ODD_PWD)|P_Fld( 0, RG_LVDSA_BIAS_EVEN_PWD));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG17,P_Fld( 0, RG_LVDSA_LDO_BIAS_ODD_PWD_D11)|P_Fld( 0, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG17, P_Fld( 0, RG_LVDSA_LDO_ODD_PWD_D11)|P_Fld( 0, RG_LVDSA_LDO_EVEN_PWD_D11));
    vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0, RG_LVDSA_DRV_PWD);    
    vIO32WriteFldMulti(REG_LVDS_TX_CFG9, P_Fld(0xFFF, RG_LVDSA_EXT_EN_D11)|P_Fld(1,RG_LVDSA_EN));    
    #endif
    //B group power on sequence
    vIO32WriteFldMulti(REG_LVDS_TX_CFG0,P_Fld( 0, RG_LVDSB_BIAS_ODD_PWD)|P_Fld( 0, RG_LVDSB_BIAS_EVEN_PWD));
    vIO32WriteFldAlign(REG_LVDS_TX_CFG8, 0, RG_LVDSB_LDO_PWD_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG1, 0, RG_LVDSB_DRV_PWD);    
    vIO32WriteFldMulti(REG_LVDS_TX_CFG0, P_Fld(0x3FFF, RG_LVDSB_EXT_EN_D11)|P_Fld(1,RG_LVDSB_EN));    


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

	u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
    switch (u4DispBit)
    {
        case DISP_30BIT:
            // enable LVDS 10-bit
            vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_10B_EN);                
#ifdef CC_MT5396
            if(u4LVDSPort == FOUR_PORT)
            {
                vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_DUAL);

                vSetLVDSPadPD(0);
            }
            else       
#endif
            if (u4LVDSPort == DUAL_PORT)
            {
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, u4TwoChMerge, MLVDS_SO0_RG_DUAL);                
                
                #if defined(CC_MT5368) || defined(CC_MT5389)
                u4PDValue = 0x1020;   // PD O5, E5
                #endif
                #ifdef CC_MT5396
                u4PDValue = 0x00fff000; // PD BO/BE
                #endif
                vSetLVDSPadPD(u4PDValue);
            }
            else  // single port
            {
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_DUAL);                
                if (u4LvdsCtrlAEven == 1)  // power down Port A-Odd
                {
                    #if defined(CC_MT5368) || defined(CC_MT5389)
                    u4PDValue = 0x107F;     // PD O0~O5, OCK, E5
                    #endif
                    #ifdef CC_MT5396
                    u4PDValue = 0x00fff03f; // PD BO/BE/AO
                    #endif
                    // Analog related setting - 0: O0 ; 1: O1 ; 2: O2 ; 3: O3 ; 4: O4 ; 5: O5 ; 6: OCK ; 7: E0 ; 8: E1 ; 9: E2 ; 10: E3 ; 11: E4 ; 12: E5 ; 13: ECK
                    vSetLVDSPadPD(u4PDValue);
                }
                else if (u4LvdsCtrlAEven == 0) // power down Port A-Even
                {
                    #if defined(CC_MT5368) || defined(CC_MT5389)
                    u4PDValue = 0x3FA0;     // PD E0~E5, ECK, O5
                    #endif
                    #ifdef CC_MT5396
                    u4PDValue = 0x00ffffc0; // PD BO/BE/AE
                    #endif
                    // Analog related setting - 0: O0 ; 1: O1 ; 2: O2 ; 3: O3 ; 4: O4 ; 5: O5 ; 6: OCK ; 7: E0 ; 8: E1 ; 9: E2 ; 10: E3 ; 11: E4 ; 12: E5 ; 13: ECK
                    vSetLVDSPadPD(u4PDValue);
                }
            }
            break;

        case DISP_24BIT:
            // disable LVDS 10-bit
            vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_10B_EN);                
#ifdef CC_MT5396
            if(u4LVDSPort == FOUR_PORT)
            {
                vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_DUAL);

                vSetLVDSPadPD(0x00410410);  // PD O4, E4 
            }
            else       
#endif
            if (u4LVDSPort == DUAL_PORT)
            {
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_DUAL);
                #if defined(CC_MT5368) || defined(CC_MT5389)
                u4PDValue = 0x1830;   // PD O5, O4, E5, E4
                #endif
                #ifdef CC_MT5396
                u4PDValue = 0x00fff410;   // PD BO/BE/AE4/AO4
                #endif
                vSetLVDSPadPD(u4PDValue);
            }
            else  // single port
            {
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_DUAL);
                if (u4LvdsCtrlAEven == 1)  // power down Port A-Odd
                {
                    #if defined(CC_MT5368) || defined(CC_MT5389)
                    u4PDValue = 0x183F;     // PD O0~O5, OCK, E5, E4
                    #endif
                    #ifdef CC_MT5396
                    u4PDValue = 0x00fff43f;   // PD BO/BE/AO/AE4
                    #endif
                    // Analog related setting - 0: O0 ; 1: O1 ; 2: O2 ; 3: O3 ; 4: O4 ; 5: O5 ; 6: OCK ; 7: E0 ; 8: E1 ; 9: E2 ; 10: E3 ; 11: E4 ; 12: E5 ; 13: ECK
                    vSetLVDSPadPD(u4PDValue);
                }
                else if (u4LvdsCtrlAEven == 0) // power down Port A-Even
                {
                    #if defined(CC_MT5368) || defined(CC_MT5389)
                     u4PDValue = 0x3FB0;     // PD E0~E5, ECK, O5, O4
                    #endif
                    #ifdef CC_MT5396
                    u4PDValue = 0x00ffffd0;   // PD BO/BE/AE/AO4
                    #endif
                    // Analog related setting - 0: O0 ; 1: O1 ; 2: O2 ; 3: O3 ; 4: O4 ; 5: O5 ; 6: OCK ; 7: E0 ; 8: E1 ; 9: E2 ; 10: E3 ; 11: E4 ; 12: E5 ; 13: ECK
                    vSetLVDSPadPD(u4PDValue);
                }
            }
            break;

        case DISP_18BIT:
            // disable LVDS 10-bit
            vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_10B_EN);                
            if (u4LVDSPort == DUAL_PORT)
            {
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_DUAL);
                #ifdef CC_MT5396
                vSetLVDSPadPD(0x00fffC30);
                 #else
                vSetLVDSPadPD(0x1C38);
                #endif
           }
            else  // single port
            {
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_4CH);                
                vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_DUAL);
                if (u4LvdsCtrlAEven == 1)  // power down Port A-Odd
                {
                    #ifdef CC_MT5396
                    vSetLVDSPadPD(0x00fffC3F);
                    #else
                    vSetLVDSPadPD(0x1C7F);
                    #endif
                }
                else if (u4LvdsCtrlAEven == 0) // power down Port A-Even
                {
                    #ifdef CC_MT5396
                    vSetLVDSPadPD(0x00fffff0);
                    #else
                    vSetLVDSPadPD(0x3FB8);
                    #endif
                }
            }
            break;

        default:
            break;
    }

    // Digital Pair Power down control
	vSetLVDSPadSourcePD(u4DispBit,u4LVDSPort);

    //vIO32WriteFldAlign(LVDSTX_ANA0, 1, LVDSA0_RST);// analog LVDS reset
    //vIO32WriteFldAlign(LVDSTX_ANA4, 1, LVDSB4_RST);// analog LVDS reset    

    #ifdef CC_COPLAT_MT82
    vUtDelay2us(5);
    #else
    HAL_Delay_us(10);
    #endif
	
	#ifdef CC_MT5396
	vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0,RG_LVDSA_EN);	
	HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 1,RG_LVDSA_EN);
	#else
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0,RG_LVDSB_EN);	
	HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 1,RG_LVDSB_EN);	
	#endif
	
    #ifdef CC_MTK_LOADER    
    vDrvLVDS7To4FifoEnable();   
	#else
	_Lvds7To4FifoReset = 2;	
	#endif

    Printf(" ======= [SA7] vDrvLVDSOn toggele RG_LVDSA_EN ===========\n");

    //vIO32WriteFldAlign(LVDSTX_ANA0, 0, LVDSA0_RST);// analog LVDS normal
    //vIO32WriteFldAlign(LVDSTX_ANA4, 0, LVDSB4_RST);// analog LVDS normal    

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

    //A group power off sequence
    #ifdef CC_MT5396
    vIO32WriteFldMulti(REG_LVDS_TX_CFG9, P_Fld(0, RG_LVDSA_EXT_EN_D11)|P_Fld(0,RG_LVDSA_EN));    
    vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0xFFF, RG_LVDSA_DRV_PWD);    
    vIO32WriteFldMulti(REG_LVDS_TX_CFG17, P_Fld( 1, RG_LVDSA_LDO_ODD_PWD_D11)|P_Fld( 1, RG_LVDSA_LDO_EVEN_PWD_D11));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG17,P_Fld( 1, RG_LVDSA_LDO_BIAS_ODD_PWD_D11)|P_Fld( 1, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG9,P_Fld( 1, RG_LVDSA_BIAS_ODD_PWD)|P_Fld( 1, RG_LVDSA_BIAS_EVEN_PWD));
    #endif
    //B group power off sequence
    vIO32WriteFldMulti(REG_LVDS_TX_CFG0, P_Fld(0, RG_LVDSB_EXT_EN_D11)|P_Fld(0,RG_LVDSB_EN));    
    #if defined(CC_MT5368) || defined(CC_MT5389)
    vIO32WriteFldAlign(REG_LVDS_TX_CFG1, 0x3FFF, RG_LVDSB_DRV_PWD);    
    #else
    vIO32WriteFldAlign(REG_LVDS_TX_CFG1, 0xFFF, RG_LVDSB_DRV_PWD);    
    #endif
    vIO32WriteFldAlign(REG_LVDS_TX_CFG8, 1, RG_LVDSB_LDO_PWD_D11);
    vIO32WriteFldMulti(REG_LVDS_TX_CFG0,P_Fld( 1, RG_LVDSB_BIAS_ODD_PWD)|P_Fld( 1, RG_LVDSB_BIAS_EVEN_PWD));
 // analog reset
    //vIO32WriteFldAlign(LVDSTX_ANA0, 1, LVDSA0_RST);
    //vIO32WriteFldAlign(LVDSTX_ANA4, 1, LVDSB4_RST);    
    
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
        if (LVDS_OUTPUT_PORT == SINGLE_PORT)
        {
         vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_ODD_SW); 
         vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_CH_SW);        
        }
        else
        {
         vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_ODD_SW); 
		 vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_CH_SW); 
        } 
    }
    else
    {
        if (LVDS_OUTPUT_PORT == SINGLE_PORT)
        {
         vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_CH_SW);        
        }
        else
        {    
        vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_ODD_SW);                
        }
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

void u1TCONIFSourceSet(UINT8 val)
{
  if (val<=2)
  {
   vIO32WriteFldAlign(VINTF_P0, val, VINTF_PANEL_IN_SEL);   
  } 
}

UINT8 u1TCONIFSourceGet(void)
{
  return (IO32ReadFldAlign(VINTF_P0, VINTF_PANEL_IN_SEL));
}

void u1TCONIFLineBufferSet(UINT8 val)
{
   if (val<=1)
   {
    vIO32WriteFldAlign(VINTF_B0, val, VINTF_6LINE_BYPASS);   
   }	
}

UINT8 u1TCONIFLineBufferGet(void)
{
  return (IO32ReadFldAlign(VINTF_B0, VINTF_6LINE_BYPASS));
}


void vDrvLVDS7To4FifoEnable(void)
{
	Printf("[LVDS] vDrvLVDS7To4FifoEnable !\n");

    vRegWriteFldAlign(MUTE_00, 1,  R_MUTE_POST_EN);
    vIO32WriteFldAlign(MLVDS_SO2, 0, RG_LVDS_74FIFO_EN);                
    vIO32WriteFldAlign(MLVDS_SO2, 1, RG_LVDS_74FIFO_EN);                
    
    vIO32WriteFldAlign(MLVDS_T11, 1, RG_ANA_INTF_FIFO_RST);   
    vIO32WriteFldAlign(MLVDS_T11, 0, RG_ANA_INTF_FIFO_RST);   

    vRegWriteFldAlign(MUTE_00, 0, R_MUTE_POST_EN);
}

//automation cli 
void LVDS_ATERM_ATVO_Setting(UINT8 mode)
{
		   if(mode==1)
		   {
			//vIO32WriteFldAlign(LVDSTX_ANA3,mode,LVDSA3_TERM_EN);
			//vIO32WriteFldAlign(LVDSTX_ANA7,mode,LVDSB7_TERM_EN);
			//Sleep(200);
			#ifdef CC_MT5396
			vIO32WriteFldAlign(REG_LVDS_TX_CFG9,8,RG_LVDSA_TVO);
			#endif
			vIO32WriteFldAlign(REG_LVDS_TX_CFG0,9,RG_LVDSB_TVO);
			//vIO32WriteFldAlign(LVDSTX_ANA4,3,LVDSB4_TVO);
			//Sleep(200);
		   }
		   else if(mode==2)
		   {
			//vIO32WriteFldAlign(LVDSTX_ANA3,mode,LVDSA3_TERM_EN);
			//vIO32WriteFldAlign(LVDSTX_ANA7,mode,LVDSB7_TERM_EN);

			//Sleep(200);
			#ifdef CC_MT5396
			vIO32WriteFldAlign(REG_LVDS_TX_CFG9,4,RG_LVDSA_TVO);
			#endif
			vIO32WriteFldAlign(REG_LVDS_TX_CFG0,5,RG_LVDSB_TVO);
			//vIO32WriteFldAlign(LVDSTX_ANA4,2,LVDSB4_TVO);
			//Sleep(200);
		   }	
		   else if(mode==3)
		   {
			//vIO32WriteFldAlign(LVDSTX_ANA3,mode,LVDSA3_TERM_EN);
			//vIO32WriteFldAlign(LVDSTX_ANA7,mode,LVDSB7_TERM_EN);

			//Sleep(200);
			#ifdef CC_MT5396
			vIO32WriteFldAlign(REG_LVDS_TX_CFG9,8,RG_LVDSA_TVO);
			#endif
			vIO32WriteFldAlign(REG_LVDS_TX_CFG0,9,RG_LVDSB_TVO);
			//vIO32WriteFldAlign(LVDSTX_ANA4,3,LVDSB4_TVO);
			//Sleep(200);
		   }			   

}

void LVDS_ATERM_ATVO_Restore(UINT8 ATERN, UINT8 ATVO)
{
       // vIO32WriteFldAlign(LVDSTX_ANA3,ATERN,LVDSA3_TERM_EN);
       // vIO32WriteFldAlign(LVDSTX_ANA7,ATERN,LVDSB7_TERM_EN);
        //Sleep(200);
	#ifdef CC_MT5396
        vIO32WriteFldAlign(REG_LVDS_TX_CFG9,ATVO,RG_LVDSA_TVO);
        #endif
        vIO32WriteFldAlign(REG_LVDS_TX_CFG0,ATVO,RG_LVDSB_TVO);
        //vIO32WriteFldAlign(LVDSTX_ANA4,ATVO,LVDSB4_TVO);    
}

void LVDS_ATVO_Set(UINT8 mode)
{
	#ifdef CC_MT5396
        vIO32WriteFldAlign(REG_LVDS_TX_CFG9,mode,RG_LVDSA_TVO);
        #endif
        vIO32WriteFldAlign(REG_LVDS_TX_CFG0,(mode+1),RG_LVDSB_TVO);
	//	vIO32WriteFldAlign(LVDSTX_ANA4,mode,LVDSB4_TVO);
}

void LVDS_APSRC_Set(UINT8 mode)
{
        //vIO32WriteFldAlign(REG_LVDS_TX_CFG9,mode,RG_LVDSA_PSRC);
        vIO32WriteFldAlign(REG_LVDS_TX_CFG0,mode,RG_LVDSB_PSRC);
	//	vIO32WriteFldAlign(LVDSTX_ANA4,mode,LVDSB4_PSRC);
}

void LVDS_ANSRC_Set(UINT8 mode)
{
       // vIO32WriteFldAlign(REG_LVDS_TX_CFG9,mode,RG_LVDSA_NSRC);
        vIO32WriteFldAlign(REG_LVDS_TX_CFG0,mode,RG_LVDSB_NSRC);
       // vIO32WriteFldAlign(LVDSTX_ANA4,mode,LVDSB4_NSRC);
}

void LVDS_PADPD_Set(UINT8 mode)
{
    if(mode == LVDS_PWDOWN)
    {
    #ifdef CC_MT5396
    vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x0, RG_LVDSA_EXT_EN_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0x0, RG_LVDSB_EXT_EN_D11);
    #else
    vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0x0, RG_LVDSB_EXT_EN_D11);
    #endif
 	// vIO32WriteFldAlign(LVDSTX_ANA4,0xFFF,LVDSB4_PD);
    }
    else
   	{
    #ifdef CC_MT5396
    vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0xFFF, RG_LVDSA_EXT_EN_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0xFFF, RG_LVDSB_EXT_EN_D11);
    #else
    vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0x3FFF, RG_LVDSB_EXT_EN_D11);
    #endif
	// vIO32WriteFldAlign(LVDSTX_ANA4,0,LVDSB4_PD);
    }
}

void LVDS_RESET_Set(UINT8 mode)
{
        //vIO32WriteFldAlign(LVDSTX_ANA0,mode,LVDSA0_RST);
	 //   vIO32WriteFldAlign(LVDSTX_ANA4,mode,LVDSB4_RST);
}

void MLVDS_LS_Set(UINT8 mode)
{
   if (mode==0)
      {
        #ifdef CC_MT5396
        vIO32WriteFldMulti(REG_LVDS_TX_CFG9, P_Fld(1, RG_LVDSA_NSRC)|P_Fld(1, RG_LVDSA_PSRC));
        #endif
        vIO32WriteFldMulti(REG_LVDS_TX_CFG0, P_Fld(3, RG_LVDSB_NSRC)|P_Fld(4, RG_LVDSB_PSRC));
   // vIO32WriteFldMulti(LVDSTX_ANA4, P_Fld(3, LVDSB4_NSRC)|P_Fld(4, LVDSB4_PSRC));
      } 
      else
      {
        #ifdef CC_MT5396
        vIO32WriteFldMulti(REG_LVDS_TX_CFG9, P_Fld(0, RG_LVDSA_NSRC)|P_Fld(7, RG_LVDSA_PSRC));
        #endif
        vIO32WriteFldMulti(REG_LVDS_TX_CFG0, P_Fld(0, RG_LVDSB_NSRC)|P_Fld(7, RG_LVDSB_PSRC));
       //vIO32WriteFldMulti(LVDSTX_ANA4, P_Fld(0, LVDSB4_NSRC)|P_Fld(7, LVDSB4_PSRC));
      }
}

void MLVDS_TestMODE_Set(UINT8 mode)
{
  vIO32WriteFldAlign(MLVDS_B9,mode,RG_MLVDS_ANA_TEST_EN);
  vIO32WriteFldAlign(MLVDS_B9,0x05,RG_MLVDS_ANA_FORCE);  
}

void vDrvVCOPhase_SEL(UINT8 u1Phase)
{
	vIO32WriteFldAlign(REG_VOPLL_CFG1, u1Phase, RG_VPLL_DLY_DATA);
}

void SET_DDDSFreeRun(void)
{
  vIO32WriteFldAlign(DDDS_00,0,DDDS_00_DISP_EN);
  vIO32WriteFldAlign(DDDS_00,0,DDDS_00_FIX_FS_DDDS_SEL);			
  vIO32WriteFldAlign(DDDS_03,0,DDDS_03_SPREAD_INIT);
  vIO32WriteFldAlign(DDDS_03,1,DDDS_03_SPREAD_INIT);
  vIO32WriteFldAlign(DDDS_03,0,DDDS_03_SPREAD_INIT);						
}

void vSetPI_GROUP(UINT8 u1AEven, UINT8 u1AODD, UINT8 u1BEven, UINT8 u1BOdd)
{
 vIO32WriteFldAlign(MLVDS_SO2, u1AEven, RG_A_SW);
 vIO32WriteFldAlign(MLVDS_SO2, u1AODD,  RG_B_SW);
 vIO32WriteFldAlign(MLVDS_SO2, u1BEven, RG_C_SW);
 vIO32WriteFldAlign(MLVDS_SO2, u1BOdd,  RG_D_SW);
}

UINT8 vGetPI_GROUP(UINT8 u1Link)
{
 if(u1Link == 0)
 return (IO32ReadFldAlign(MLVDS_SO2, RG_A_SW));
 else if(u1Link == 1)
 return (IO32ReadFldAlign(MLVDS_SO2, RG_B_SW));
 else if(u1Link == 2) 
 return (IO32ReadFldAlign(MLVDS_SO2, RG_C_SW));
 else if(u1Link == 3)  
 return (IO32ReadFldAlign(MLVDS_SO2, RG_D_SW));
 else
 return 0;
}


