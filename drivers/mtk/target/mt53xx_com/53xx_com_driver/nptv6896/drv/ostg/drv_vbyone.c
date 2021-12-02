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
 * $RCSfile: drv_vbyone.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

#include "general.h"
#include "hw_vbyone.h"
#include "x_typedef.h"
#include "drv_lvds.h"
#include "hw_mlvds.h"
#include "hw_lvds.h"
#include "hw_tg.h"
#include "drv_vbyone.h"
#include "drv_vdoclk.h"
#include "x_pinmux.h"

#ifndef CC_MT5396
  #define CC_VB1_DISABLE
#endif

typedef struct RVB1AnaRegTbl
{
	UINT32 wReg;
	UINT32 wValue;
} RVB1AnaRegTbl;

RVB1AnaRegTbl const VB1_ana_default[] = // Analog Setting A Group default
{
{0xF0060024, 0x0448c000},
{0xF0060028, 0x0c480fff},
{0xF006002c, 0x0fff0000},
{0xF0060030, 0x00000000},
{0xF0060034, 0x00000000},
{0xF0060038, 0x00000000},
{0xF006003C, 0x00000000},
{0xF0060040, 0x00000000},
{0xF0060044, 0x000f0000},
{0xF0060048, 0x00000000},
{0xF006004c, 0x00000000},
{0xFFFFFFFF, 0x00000000}
};



void _svDrvVB1LoadRegTbl(RVB1AnaRegTbl const *pRegTbl)
{
	if(pRegTbl == NULL)
	{
		return;
	}

	while(pRegTbl->wReg != 0xFFFFFFFF)
	{
		vIO32Write4B(pRegTbl->wReg, pRegTbl->wValue);
		pRegTbl++;
	}
}

/**
 * @brief vVByOneAnalogSetting
 * VB1 analog setting  
 * @param  void
 * @retval void
 */
void vVByOneAnalogSetting(void)
{
#ifndef CC_VB1_DISABLE


//** Analog Setting A Group**
//load default setting
	_svDrvVB1LoadRegTbl(VB1_ana_default);

//REG(RG_LVDSA_VSW_CALIB_SEL_D11)=0b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_VSW_CALIB_SEL_D11); 
//REG(RG_LVDSA_ABIST_VL_ODD_SEL_D11[4:0])=11100b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG15, 0x1c, RG_LVDSA_ABIST_VL_ODD_SEL_D11); 
//REG(RG_LVDSA_TVO[3:0])=1101b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0xd, RG_LVDSA_TVO); 
//REG(RG_LVDSA_TVCM[3:0])=0100b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x4, RG_LVDSA_TVCM);
//REG(RG_LVDSA_BIAS_SEL[3:0])=0100b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0x4, RG_LVDSA_BIAS_SEL); 
//REG(RG_LVDSA_NSRC[2:0])=110b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x6, RG_LVDSA_NSRC);
//REG(RG_LVDSA_PSRC[2:0])=110b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x6, RG_LVDSA_PSRC);
//REG(RG_LVDSA_EXT_EN_D11[11:0])=111111111111b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x170, RG_LVDSA_EXT_EN_D11);
//REG(RG_LVDSA_LVDS_SEL[11:0])=000000000000b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x0, RG_LVDSA_LVDS_SEL);
//REG(RG_LVDSA_CLK_IQ_SEL[11:0])=000000000000b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x0, RG_LVDSA_CLK_IQ_SEL);
//REG(RG_LVDSA_ENEDGE_SEL_D11)=0b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_ENEDGE_SEL_D11);
//REG(RG_LVDSA_RESYNC_CLK_IQ_SEL_D11)=0b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_RESYNC_CLK_IQ_SEL_D11);
//REG(RG_LVDSA_RESYNC_LVDS_SEL_D11)=0b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_RESYNC_LVDS_SEL_D11);
//REG(RG_LVDSA_BIAS_ODD_PWD)=0b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x0, RG_LVDSA_BIAS_ODD_PWD);
//REG(RG_LVDSA_BIAS_EVEN_PWD)=0b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x0, RG_LVDSA_BIAS_EVEN_PWD);
//REG(RG_LVDSA_LDO_BIAS_ODD_PWD_D11)=0b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_LDO_BIAS_ODD_PWD_D11);
//REG(RG_LVDSA_LDO_BIAS_EVEN_PWD_D11)=0b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11);
//REG(RG_LVDSA_LDO_ODD_PWD_D11)=0b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_LDO_ODD_PWD_D11);
//REG(RG_LVDSA_LDO_EVEN_PWD_D11)=0b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_LDO_EVEN_PWD_D11);
//REG(RG_LVDSA_DRV_PWD[11:0])=000000000000b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0xE8F, RG_LVDSA_DRV_PWD);
//REG(RG_LVDSA_PRE_PWD[11:0])=111010001111b   AOCK AO4 AE0 AE2
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0xe8f, RG_LVDSA_PRE_PWD);
//REG(RG_LVDSA_TVO_PRE[2:0])=001b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0x1, RG_LVDSA_TVO_PRE);
//REG(RG_LVDSA_NSRC_PRE[2:0])=100b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0x4, RG_LVDSA_NSRC_PRE);
//REG(RG_LVDSA_PSRC_PRE[2:0])=100b
	vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0x4, RG_LVDSA_PSRC_PRE);
    vIO32WriteFldMulti(REG_LVDS_TX_CFG12, P_Fld(1, RG_LVDSA_HSN_MODE_EVEN_EN_D11)|P_Fld(1, RG_LVDSA_HSP_MODE_EVEN_EN_D11)
                                         |P_Fld(1, RG_LVDSA_HSN_MODE_ODD_EN_D11)|P_Fld(1, RG_LVDSA_HSP_MODE_ODD_EN_D11));
//REG(RG_LVDSA_EN)=1b

	vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x0, RG_LVDSA_EN);
	HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x1, RG_LVDSA_EN);

	
#endif
}
/**
 * @brief vVByOne4lane8bitAnalogSetting
 * analog setting 4 lane 8 bit 
 * @param  void
 * @retval void
 */
void vVByOne4lane8bitAnalogSetting(void)
{
	vVByOneAnalogSetting();
}
/**
 * @brief vVByOne4lane10bitAnalogSetting
 * analog setting 4 lane 10 bit 
 * @param  void
 * @retval void
 */
void vVByOne4lane10bitAnalogSetting(void)
{
	vVByOneAnalogSetting();
}
/**
 * @brief vVByOne2lane8bitAnalogSetting
 * analog setting 2 lane 8 bit 
 * @param  void
 * @retval void
 */
void vVByOne2lane8bitAnalogSetting(void)
{
	vVByOneAnalogSetting();
}
/**
 * @brief vVByOne2lane10bitAnalogSetting
 * analog setting 2 lane 10 bit 
 * @param  void
 * @retval void
 */
void vVByOne2lane10bitAnalogSetting(void)
{
	vVByOneAnalogSetting();
}
/**
 * @brief vVByOne1lane8bitAnalogSetting
 * analog setting 1 lane 8 bit 
 * @param  void
 * @retval void
 */
void vVByOne1lane8bitAnalogSetting(void)
{
	vVByOneAnalogSetting();
}
/**
 * @brief vVByOne1lane10bitAnalogSetting
 * analog setting 1 lane 10 bit 
 * @param  void
 * @retval void
 */
void vVByOne1lane10bitAnalogSetting(void)
{
	vVByOneAnalogSetting();
}
void vVByOneInit(void)
{
#ifndef CC_VB1_DISABLE

UINT8 i;
UINT32 u4tmp;

	i=0;
	u4tmp=0;
	do
	{
		u4tmp = u4IO32Read4B(RGS_VOPLL_CFG0);
		vUtDelay2us(10);
		i++;
		if(i>200)
		{
			printf("VB1 Vpll clock check error......\n ");
			break;
		}
		
	}while((u4tmp &0xC0000000)!=0x80000000); //bit 31 30


    // switch to LVDS TX mode
    vIO32WriteFldAlign(MLVDS_B0, 2, RG_FIFO_SEL);                

    vIO32WriteFldAlign(VB1SYS0, 0, VB1SYS0_bit_inv);
    vIO32WriteFldAlign(VB1SYS0, 0, VB1SYS0_vsyn_inv);
    vIO32WriteFldAlign(VB1SYS0, 0, VB1SYS0_hsyn_inv);
    vIO32WriteFldAlign(VB1SYS0, 1, VB1SYS0_lane_3_en);
    vIO32WriteFldAlign(VB1SYS0, 1, VB1SYS0_lane_2_en);
    vIO32WriteFldAlign(VB1SYS0, 1, VB1SYS0_lane_1_en);
    vIO32WriteFldAlign(VB1SYS0, 1, VB1SYS0_lane_0_en);
    vIO32WriteFldAlign(VB1SYS0, 0, VB1SYS0_FIELDBET);
    vIO32WriteFldAlign(VB1SYS0, 0, VB1SYS0_patgen_start);
    vIO32WriteFldAlign(VB1SYS0, 1, VB1SYS0_de_rst_ctrl);
    vIO32WriteFldAlign(VB1SYS0, 2, VB1SYS0_lane_num);
    vIO32WriteFldAlign(VB1SYS0, 2, VB1SYS0_COL);


    vIO32WriteFldAlign(VB1SYS3, 0, VB1SYS3_crc_en);
    vIO32WriteFldAlign(VB1SYS3, 0, VB1SYS3_lane_prbs_en);
    vIO32WriteFldAlign(VB1SYS3, 1, VB1SYS3_lane3_ana);
    vIO32WriteFldAlign(VB1SYS3, 1, VB1SYS3_lane2_ana);
    vIO32WriteFldAlign(VB1SYS3, 1, VB1SYS3_lane1_ana);
    vIO32WriteFldAlign(VB1SYS3, 1, VB1SYS3_lane0_ana);
    vIO32WriteFldAlign(VB1SYS3, 0, VB1SYS3_lane3_inv);
    vIO32WriteFldAlign(VB1SYS3, 0, VB1SYS3_lane2_inv);
    vIO32WriteFldAlign(VB1SYS3, 0, VB1SYS3_lane1_inv);
    vIO32WriteFldAlign(VB1SYS3, 0, VB1SYS3_lane0_inv);
    vIO32WriteFldAlign(VB1SYS1, 3, VB1SYS1_lane3_mux);
    vIO32WriteFldAlign(VB1SYS1, 2, VB1SYS1_lane2_mux);
    vIO32WriteFldAlign(VB1SYS1, 1, VB1SYS1_lane1_mux);
    vIO32WriteFldAlign(VB1SYS1, 0, VB1SYS1_lane0_mux);
    vIO32WriteFldAlign(VB1SYS1, 0, VB1SYS1_mon_sel);

         
    vIO32WriteFldAlign(VB1SYS3, 0, VB1SYS3_ana_rx_en);
    vIO32WriteFldAlign(VB1SYS3, 0x3FF, VB1SYS3_lane3_ana_value);

    vIO32WriteFldAlign(TMGR5, 0, TMGR5_fifo_ctrl);
    vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
    vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
	
	
    //////////////// VB1 Analog ////////////////////////////////	
	//REG(0xf0034040[31:0]) = 0x000fffff
		vIO32WriteFldAlign(TMGR16,0xfffff,TMGR16_tmds_reg_en);
	//REG(0xf0034044[31:0]) = 0x000003ff
		vIO32WriteFldAlign(TMGR17,0x3ff,TMGR17_tmds_reg_value);
	//REG(0xf0034014[31:0]) = 0x00710000
		vIO32WriteFldAlign(TMGR5,0x7,TMGR5_fifo_ctrl);
	//REG(RG_LVDSA_VSW_CALIB_SEL_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_VSW_CALIB_SEL_D11); 
	//REG(RG_LVDSA_ABIST_VL_ODD_SEL_D11[4:0])=11100b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG15, 0x1c, RG_LVDSA_ABIST_VL_ODD_SEL_D11); 
	//REG(RG_LVDSA_TVO[3:0])=1101b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0xd, RG_LVDSA_TVO); 
	//REG(RG_LVDSA_TVCM[3:0])=0100b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x4, RG_LVDSA_TVCM);
	//REG(RG_LVDSA_BIAS_SEL[3:0])=0100b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0x4, RG_LVDSA_BIAS_SEL); 
	//REG(RG_LVDSA_NSRC[2:0])=110b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x6, RG_LVDSA_NSRC);
	//REG(RG_LVDSA_PSRC[2:0])=110b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x6, RG_LVDSA_PSRC);
	//REG(RG_LVDSA_EXT_EN_D11[11:0])=111111111111b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x170, RG_LVDSA_EXT_EN_D11);
	//REG(RG_LVDSA_LVDS_SEL[11:0])=000000000000b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x0, RG_LVDSA_LVDS_SEL);
	//REG(RG_LVDSA_CLK_IQ_SEL[11:0])=000000000000b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x0, RG_LVDSA_CLK_IQ_SEL);
	//REG(RG_LVDSA_ENEDGE_SEL_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_ENEDGE_SEL_D11);
	//REG(RG_LVDSA_RESYNC_CLK_IQ_SEL_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_RESYNC_CLK_IQ_SEL_D11);
	//REG(RG_LVDSA_RESYNC_LVDS_SEL_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_RESYNC_LVDS_SEL_D11);
	//REG(RG_LVDSA_BIAS_ODD_PWD)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x0, RG_LVDSA_BIAS_ODD_PWD);
	//REG(RG_LVDSA_BIAS_EVEN_PWD)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x0, RG_LVDSA_BIAS_EVEN_PWD);
	//REG(RG_LVDSA_LDO_BIAS_ODD_PWD_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_LDO_BIAS_ODD_PWD_D11);
	//REG(RG_LVDSA_LDO_BIAS_EVEN_PWD_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11);
	//REG(RG_LVDSA_LDO_ODD_PWD_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_LDO_ODD_PWD_D11);
	//REG(RG_LVDSA_LDO_EVEN_PWD_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_LDO_EVEN_PWD_D11);
	//REG(RG_LVDSA_DRV_PWD[11:0])=000000000000b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0xE8F, RG_LVDSA_DRV_PWD);
	//REG(RG_LVDSA_PRE_PWD[11:0])=111111111111b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0xfff, RG_LVDSA_PRE_PWD);
	//REG(RG_LVDSA_TVO_PRE[2:0])=001b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0x1, RG_LVDSA_TVO_PRE);
	//REG(RG_LVDSA_NSRC_PRE[2:0])=100b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0x4, RG_LVDSA_NSRC_PRE);
	//REG(RG_LVDSA_PSRC_PRE[2:0])=100b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0x4, RG_LVDSA_PSRC_PRE);
    vIO32WriteFldMulti(REG_LVDS_TX_CFG12, P_Fld(1, RG_LVDSA_HSN_MODE_EVEN_EN_D11)|P_Fld(1, RG_LVDSA_HSP_MODE_EVEN_EN_D11)
                                         |P_Fld(1, RG_LVDSA_HSN_MODE_ODD_EN_D11)|P_Fld(1, RG_LVDSA_HSP_MODE_ODD_EN_D11));
	//REG(RG_LVDSA_EN)=1b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9, 0x1, RG_LVDSA_EN);
	//REG(RG_LVDSA_IMP_COMP_EN_D11)=1b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x1, RG_LVDSA_IMP_COMP_EN_D11);
	
	vUtDelay2us(100);
	vIO32Write4B(TMGR7,0x00200014);
	vUtDelay2us(100);
	vIO32Write4B(TMGR7,0x00200017);
	
	i=0;
	u4tmp=0;
	do
	{
		u4tmp = IO32ReadFldAlign(TMGR18,TMGR18_hold);
		vUtDelay2us(10);
		i++;
		if(i>200)
		{
				printf("VB1 impedance cal error then break......\n ");
				break;
		}
		
	}while(!u4tmp);

	u4tmp = IO32ReadFldAlign(TMGR18,TMGR18_impcal_tx_lat);
	if(u4tmp>0 && u4tmp<0xf)
	{
    		printf("VB1 VB1 impedance cal  pass and value =%x .................\n",u4tmp);
	}
	else
	{
    		printf("VB1 VB1 impedance cal  failed .........................\n");
	}
	
	vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_IMP_COMP_EN_D11);

    vIO32WriteFldAlign(VB1SYS3, 0x0, VB1SYS3_lane3_ana);
    vIO32WriteFldAlign(VB1SYS3, 0x0, VB1SYS3_lane2_ana);
    vIO32WriteFldAlign(VB1SYS3, 0x0, VB1SYS3_lane1_ana);
    vIO32WriteFldAlign(VB1SYS3, 0x0, VB1SYS3_lane0_ana);

	
#endif
}
/**
 * @brief vVByOneDigitalReset
 * disable VB1 digital module
 * @param  void
 * @retval void
 */
void vVByOneDigitalReset(void)
{
#ifndef CC_VB1_DISABLE

    vIO32WriteFldAlign(TMGR5, 0, TMGR5_fifo_ctrl);
    vIO32WriteFldAlign(TMGR5, 0, TMGR5_fifo_en);
    vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
    
#endif
}
/**
 * @brief vDrvVB1On
 * Power On VBYONE
 * @param  void
 * @retval void
 */
void vDrvVByOneOn(void)
{
#ifndef CC_VB1_DISABLE
    UINT32 u4DispBit;
    UINT32 u4VB1Lane;
    
    u4DispBit = DISP_BIT;
    u4VB1Lane = LVDS_OUTPUT_PORT;

    BSP_PinSet( PIN_GPIO42, PINMUX_FUNCTION2);  // LOCKN
    BSP_PinSet( PIN_TCON11, PINMUX_FUNCTION3);  // HDPN
//	vIO32WriteFldAlign(TMGR5, 0, vb1_reorder);
	switch (u4DispBit)
    	{
    #ifndef SUPPORT_VESTEL_FLASH_PANEL
		case DISP_36BIT:
		   
			 vIO32WriteFldAlign(VB1SYS0,0x3,VB1SYS0_COL);
//			 vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1,FLD_OCLK_DIV_SEL);
		   if(u4VB1Lane == FOUR_PORT)
		   {
			   vIO32WriteFldAlign(VB1SYS1,0,VB1SYS1_ttl_reorder);
			   vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2,FLD_VB1_CK_SEL);
			   if(IS_LVDS_HIGHSPEED)//240Hz
				   {
					   //vVByOne4lane12bitAnalogSetting();
					   vVByOneAnalogSetting();
					   vVByOneDigitalReset();
					   vIO32WriteFldAlign(VB1SYS0,0x2,VB1SYS0_lane_num);
					   vIO32WriteFldAlign(VB1SYS0,0xff,VB1SYS0_lane_en_mux);
//					   vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 0,FLD_OCLK_DIV_SEL);
//					   vIO32WriteFldAlign(TMGR5, 3, vb1_reorder);
				   }
			   else
				   {
					   //vVByOne4lane12bitAnalogSetting();
					   vVByOneAnalogSetting();
					   vVByOneDigitalReset();
					   vIO32WriteFldAlign(VB1SYS0,0x2,VB1SYS0_lane_num);
					   vIO32WriteFldAlign(VB1SYS0,0xf,VB1SYS0_lane_en_mux);
				   }
		   }
		   if (u4VB1Lane == DUAL_PORT)
				   {
			   vIO32WriteFldAlign(VB1SYS1,1,VB1SYS1_ttl_reorder);
			   vIO32WriteFldAlign(CKGEN_OCLK_TEST, 1,FLD_VB1_CK_SEL);
					   //vVByOne2lane12bitAnalogSetting();
					   vVByOneAnalogSetting();
					   vVByOneDigitalReset();
					   vIO32WriteFldAlign(VB1SYS0,0x1,VB1SYS0_lane_num);
					   vIO32WriteFldAlign(VB1SYS0,0x3,VB1SYS0_lane_en_mux);
					   
		   }
		   if (u4VB1Lane == SINGLE_PORT)
		   {
			   vIO32WriteFldAlign(VB1SYS1,1,VB1SYS1_ttl_reorder);
			   vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0,FLD_VB1_CK_SEL);
			   //vVByOne1lane12bitAnalogSetting();
			   vVByOneAnalogSetting();
			   vVByOneDigitalReset();
			   vIO32WriteFldAlign(VB1SYS0,0x0,VB1SYS0_lane_num);
			   vIO32WriteFldAlign(VB1SYS0,0x3,VB1SYS0_lane_en_mux);
		   }
		   break;
	#endif
       	 case DISP_30BIT:
       	 	
       	 	  vIO32WriteFldAlign(VB1SYS0,0x2,VB1SYS0_COL);
//			  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1,FLD_OCLK_DIV_SEL);
       	 	if(u4VB1Lane == FOUR_PORT)
            		{
				vIO32WriteFldAlign(VB1SYS1,0,VB1SYS1_ttl_reorder);
				vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2,FLD_VB1_CK_SEL);
				if(IS_LVDS_HIGHSPEED)//240Hz
					{
						//vVByOne4lane10bitAnalogSetting();
						vVByOneAnalogSetting();
						vVByOneDigitalReset();
						vIO32WriteFldAlign(VB1SYS0,0x2,VB1SYS0_lane_num);
						vIO32WriteFldAlign(VB1SYS0,0xff,VB1SYS0_lane_en_mux);
//						vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 0,FLD_OCLK_DIV_SEL);
//						vIO32WriteFldAlign(TMGR5, 3, vb1_reorder);
					}
				else
					{
            			//vVByOne4lane10bitAnalogSetting();
            			vVByOneAnalogSetting();
            		 	vVByOneDigitalReset();
            			vIO32WriteFldAlign(VB1SYS0,0x2,VB1SYS0_lane_num);
            			vIO32WriteFldAlign(VB1SYS0,0xf,VB1SYS0_lane_en_mux);
					}
       	 	}
       	 	if (u4VB1Lane == DUAL_PORT)
            		{
				vIO32WriteFldAlign(VB1SYS1,1,VB1SYS1_ttl_reorder);
				vIO32WriteFldAlign(CKGEN_OCLK_TEST, 1,FLD_VB1_CK_SEL);
            			//vVByOne2lane10bitAnalogSetting();
            			vVByOneAnalogSetting();
            			vVByOneDigitalReset();
            			vIO32WriteFldAlign(VB1SYS0,0x1,VB1SYS0_lane_num);
            			vIO32WriteFldAlign(VB1SYS0,0x3,VB1SYS0_lane_en_mux);
            			
       	 	}
       	 	if (u4VB1Lane == SINGLE_PORT)
       	 	{
				vIO32WriteFldAlign(VB1SYS1,1,VB1SYS1_ttl_reorder);
				vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0,FLD_VB1_CK_SEL);
       	 		//vVByOne1lane10bitAnalogSetting();
       	 		vVByOneAnalogSetting();
       	 		vVByOneDigitalReset();
                vIO32WriteFldAlign(VB1SYS0,0x0,VB1SYS0_lane_num);
                vIO32WriteFldAlign(VB1SYS0,0x3,VB1SYS0_lane_en_mux);
       	 	}
        	break;
        	 case DISP_24BIT:
       	 	vIO32WriteFldAlign(VB1SYS0,0x1,VB1SYS0_COL);
//			vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1,FLD_OCLK_DIV_SEL);
       	 	if(u4VB1Lane == FOUR_PORT)
            		{
				vIO32WriteFldAlign(VB1SYS1,0,VB1SYS1_ttl_reorder);
				vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2,FLD_VB1_CK_SEL);
				if(IS_LVDS_HIGHSPEED)//240Hz
				{
					//vVByOne4lane8bitAnalogSetting();
					vVByOneAnalogSetting();
					vVByOneDigitalReset();
					vIO32WriteFldAlign(VB1SYS0,0x2,VB1SYS0_lane_num);
					vIO32WriteFldAlign(VB1SYS0,0xff,VB1SYS0_lane_en_mux);
//					vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 0,FLD_OCLK_DIV_SEL);
//					vIO32WriteFldAlign(TMGR5, 3, vb1_reorder);
				}
				else
				{
            			//vVByOne4lane8bitAnalogSetting();
            			vVByOneAnalogSetting();
            			vVByOneDigitalReset();
                    vIO32WriteFldAlign(VB1SYS0,0x2,VB1SYS0_lane_num);
                    vIO32WriteFldAlign(VB1SYS0,0xf,VB1SYS0_lane_en_mux);
				}
       	 	}
       	 	if (u4VB1Lane == DUAL_PORT)
            		{
				vIO32WriteFldAlign(VB1SYS1,1,VB1SYS1_ttl_reorder);
				vIO32WriteFldAlign(CKGEN_OCLK_TEST, 1,FLD_VB1_CK_SEL);
            			//vVByOne2lane8bitAnalogSetting();
            			vVByOneAnalogSetting();
            			vVByOneDigitalReset();
       			vIO32WriteFldAlign(VB1SYS0,0x1,VB1SYS0_lane_num);
                vIO32WriteFldAlign(VB1SYS0,0x3,VB1SYS0_lane_en_mux);
       	 	}
       	 	if (u4VB1Lane == SINGLE_PORT)
       	 	{
				vIO32WriteFldAlign(VB1SYS1,1,VB1SYS1_ttl_reorder);
				vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0,FLD_VB1_CK_SEL);
       	 		//vVByOne1lane8bitAnalogSetting();
       	 		vVByOneAnalogSetting();
       	 		vVByOneDigitalReset();
       	 		vIO32WriteFldAlign(VB1SYS0,0x0,VB1SYS0_lane_num);
                vIO32WriteFldAlign(VB1SYS0,0x3,VB1SYS0_lane_en_mux);
       	 	}
        	break;
        	default:
        	break;
    	}
	vIO32WriteFldAlign(VB1SYS0,0x1,VB1SYS0_de_rst_ctrl);
	vIO32WriteFldAlign(VB1SYS1,0x3,VB1SYS1_lane3_mux);
	vIO32WriteFldAlign(VB1SYS1,0x2,VB1SYS1_lane2_mux);
	vIO32WriteFldAlign(VB1SYS1,0x1,VB1SYS1_lane1_mux);
	vIO32WriteFldAlign(VB1SYS1,0x0,VB1SYS1_lane0_mux);
	vIO32WriteFldAlign(VB1SYS1,0x0,VB1SYS1_mon_sel);

	//vIO32Write4B(0xf003582c,0x00000003);
	//vIO32WriteFldAlign(SYS6, 0, SYS6_fifo_ctrl);
    //vIO32WriteFldAlign(SYS6, 1, SYS6_fifo_en);
    //vIO32WriteFldAlign(SYS6, 1, VB1SYS6_start);

    vIO32WriteFldAlign(TMGR5, 0, TMGR5_fifo_ctrl);
    vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
    vIO32WriteFldAlign(VB1SYS6, 1, VB1SYS6_start);
	
#endif
}

/**
 * @brief vDrvVB1Off
 * Power off VBYONE digital
 * @param  void
 * @retval void
 */
void vDrvVByOneOff(void)
{
#ifndef CC_VB1_DISABLE
     vIO32WriteFldAlign(TMGR5, 0, TMGR5_fifo_ctrl);
     vIO32WriteFldAlign(TMGR5, 0, TMGR5_fifo_en);
     vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
#endif
}

/**
 * @brief vDrvVByOneCRCCheck
 * crc check
 * @param  void
 * @retval void
 */
void vDrvVByOneCRCCheck(void)
{
#ifndef CC_VB1_DISABLE
    UINT32 u4VB1Lane;
   
    u4VB1Lane = LVDS_OUTPUT_PORT;
    
	vIO32WriteFldAlign(VB1RES,0x1,VB1RES_REG_contrl);
	vIO32WriteFldAlign(VB1SYS3,0x0,VB1SYS3_crc_en);
	vIO32WriteFldAlign(VB1SYS3,0x1,VB1SYS3_crc_en);
       if(u4VB1Lane == FOUR_PORT)
       {
            	if(IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc1_fail) ||IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc2_fail) ||IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc3_fail)||IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc4_fail))
            	{
            		printf("4 lane CRC fail and Fali value =%x",IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc1_fail) |IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc2_fail) |IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc3_fail)|IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc4_fail));
            	}
            	else
            	{
            		printf("4 lane CRC pass");
            	}
       }
       if (u4VB1Lane == DUAL_PORT)
       {   
            	if(IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc1_fail) ||IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc2_fail) )
            	{
            		printf("2 lane CRC fail and Fali value =%x",IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc1_fail) |IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc2_fail) );
            	}	
            	else
            	{
            		printf("2 lane CRC pass");
            	}
        }
      	if (u4VB1Lane == SINGLE_PORT)
       {
       	if(IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc1_fail))
            	{
            		printf("1 lane CRC fail and Fali value =%x",IO32ReadFldAlign(VB1SYS8, VB1SYS8_crc1_fail));
            	}	
       	else
            	{
            		printf("1 lane CRC pass");
            	}
        }
#endif
}

void VByOne_Impedance_cal()
{
#ifndef CC_VB1_DISABLE
	UINT8 i;
	UINT32 u4tmp;

		vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
	//REG(0xf0034040[31:0]) = 0x000fffff
		vIO32WriteFldAlign(TMGR16,0xfffff,TMGR16_tmds_reg_en);
	//REG(0xf0034044[31:0]) = 0x000003ff
		vIO32WriteFldAlign(TMGR17,0x3ff,TMGR17_tmds_reg_value);
	//REG(0xf0034014[31:0]) = 0x00710000
		vIO32WriteFldAlign(TMGR5,0x7,TMGR5_fifo_ctrl);
	//REG(RG_LVDSA_VSW_CALIB_SEL_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17,0,RG_LVDSA_VSW_CALIB_SEL_D11);
	//REG(RG_LVDSA_ABIST_VL_ODD_SEL_D11[4:0])=11100b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG15,0x1c,RG_LVDSA_ABIST_VL_ODD_SEL_D11);
	//REG(RG_LVDSA_TVO[3:0])=1101b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9,0xd,RG_LVDSA_TVO);
	//REG(RG_LVDSA_TVCM[3:0])=0100b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9,0x4,RG_LVDSA_TVCM);
	//REG(RG_LVDSA_BIAS_SEL[3:0])=0100b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG10,0x4,RG_LVDSA_BIAS_SEL);
	//REG(RG_LVDSA_NSRC[2:0])=110b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9,0x6,RG_LVDSA_NSRC);
	//REG(RG_LVDSA_PSRC[2:0])=110b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9,0x6,RG_LVDSA_PSRC);
	//REG(RG_LVDSA_EXT_EN_D11[11:0])=111111111111b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9,0x170,RG_LVDSA_EXT_EN_D11);
	//REG(RG_LVDSA_LVDS_SEL[11:0])=000000000000b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG12,0,RG_LVDSA_LVDS_SEL);
	//REG(RG_LVDSA_CLK_IQ_SEL[11:0])=000000000000b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG11,0,RG_LVDSA_CLK_IQ_SEL);
	//REG(RG_LVDSA_ENEDGE_SEL_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17,0,RG_LVDSA_ENEDGE_SEL_D11);
	//REG(RG_LVDSA_RESYNC_CLK_IQ_SEL_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17,0,RG_LVDSA_RESYNC_CLK_IQ_SEL_D11);
	//REG(RG_LVDSA_RESYNC_LVDS_SEL_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17,0,RG_LVDSA_RESYNC_LVDS_SEL_D11);
	//REG(RG_LVDSA_BIAS_ODD_PWD)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9,0,RG_LVDSA_BIAS_ODD_PWD);
	//REG(RG_LVDSA_BIAS_EVEN_PWD)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9,0,RG_LVDSA_BIAS_EVEN_PWD);
	//REG(RG_LVDSA_LDO_BIAS_ODD_PWD_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17,0,RG_LVDSA_LDO_BIAS_ODD_PWD_D11);
	//REG(RG_LVDSA_LDO_BIAS_EVEN_PWD_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17,0,RG_LVDSA_LDO_BIAS_EVEN_PWD_D11);
	//REG(RG_LVDSA_LDO_ODD_PWD_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17,0,RG_LVDSA_LDO_ODD_PWD_D11);
	//REG(RG_LVDSA_LDO_EVEN_PWD_D11)=0b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17,0,RG_LVDSA_LDO_EVEN_PWD_D11);
	//REG(RG_LVDSA_DRV_PWD[11:0])=000000000000b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG10,0xE8F,RG_LVDSA_DRV_PWD);
	//REG(RG_LVDSA_PRE_PWD[11:0])=111111111111b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG11,0xfff,RG_LVDSA_PRE_PWD);
	//REG(RG_LVDSA_TVO_PRE[2:0])=000b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG10,0x1,RG_LVDSA_TVO_PRE);
	//REG(RG_LVDSA_NSRC_PRE[2:0])=100b
		vIO32WriteFldAlign(RG_LVDSA_NSRC_PRE,0x4,RG_LVDSA_NSRC_PRE);
	//REG(RG_LVDSA_PSRC_PRE[2:0])=100b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG10,0x4,RG_LVDSA_PSRC_PRE);
    vIO32WriteFldMulti(REG_LVDS_TX_CFG12, P_Fld(1, RG_LVDSA_HSN_MODE_EVEN_EN_D11)|P_Fld(1, RG_LVDSA_HSP_MODE_EVEN_EN_D11)
                                         |P_Fld(1, RG_LVDSA_HSN_MODE_ODD_EN_D11)|P_Fld(1, RG_LVDSA_HSP_MODE_ODD_EN_D11));
	//REG(RG_LVDSA_EN)=1b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG9,0x1,RG_LVDSA_EN);
	//REG(RG_LVDSA_IMP_COMP_EN_D11)=1b
		vIO32WriteFldAlign(REG_LVDS_TX_CFG17,0x1,RG_LVDSA_IMP_COMP_EN_D11);
	
	vIO32Write4B(TMGR7,0x00200014);
	vUtDelay2us(10);
	vIO32Write4B(TMGR7,0x00200017);

	i=0;
	u4tmp=0;
	do
	{
		u4tmp = IO32ReadFldAlign(TMGR18,TMGR18_hold);
		vUtDelay2us(10);
		i++;
		if(i>200)
		{
				printf("VB1 impedance cal error then break......\n ");
				break;
		}
		
		}while(!u4tmp);
	
	u4tmp = IO32ReadFldAlign(TMGR18,TMGR18_impcal_tx_lat);
	if(u4tmp>0 && u4tmp<0xf)
	{
			printf("VB1 VB1 impedance cal  pass and value =%x .................\n",u4tmp);
	}
	else
	{
			printf("VB1 VB1 impedance cal  failed .........................\n");
	}
	
	vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x0, RG_LVDSA_IMP_COMP_EN_D11);

#endif
}

void VByOneLane4_8bit_script()
{
#ifndef CC_VB1_DISABLE
							   
  vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_POSDIV);
  vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x01, RG_VPLL_PREDIV)|P_Fld(0x01, RG_VPLL_FBKSEL)
								    |P_Fld(14, RG_VPLL_FBKDIV));
  // MT5395 toggle VOPLL Power
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_LVDS_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_VB1_TX_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG3, 1, RG_PPLL_PWD);		
  HAL_Delay_us(400);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_PWD); 
  vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_LVDS_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_VB1_TX_PWD);
  HAL_Delay_us(20);

  vIO32WriteFldAlign(VB1SYS0,0x1,VB1SYS0_COL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1,FLD_OCLK_DIV_SEL);
  vIO32WriteFldAlign(VB1SYS1,0,VB1SYS1_ttl_reorder);
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2,FLD_VB1_CK_SEL);
  //vVByOneAnalogSetting();
  //vVByOneDigitalReset();
  vIO32WriteFldAlign(VB1SYS0,0x2,VB1SYS0_lane_num);
  vIO32WriteFldAlign(VB1SYS0,0xf,VB1SYS0_lane_en_mux);
#endif

}

void VByOneLane4_10bit_script()
{
#ifndef CC_VB1_DISABLE
  vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_POSDIV);
  vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x01, RG_VPLL_PREDIV)|P_Fld(0x01, RG_VPLL_FBKSEL)
									   |P_Fld(19, RG_VPLL_FBKDIV));
  // MT5395 toggle VOPLL Power
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_LVDS_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_VB1_TX_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG3, 1, RG_PPLL_PWD);		
  HAL_Delay_us(400);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_PWD); 
  vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_LVDS_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_VB1_TX_PWD);
  HAL_Delay_us(20);

  vIO32WriteFldAlign(VB1SYS0,0x2,VB1SYS0_COL);
//  vIO32WriteFldAlign(VB1SYS1,0,VB1SYS1_ttl_reorder);
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2,FLD_VB1_CK_SEL);
  //vVByOneAnalogSetting();
  //vVByOneDigitalReset();
  vIO32WriteFldAlign(VB1SYS0,0x2,VB1SYS0_lane_num);
  vIO32WriteFldAlign(VB1SYS0,0xf,VB1SYS0_lane_en_mux);
#endif  
}

void VByOneLane4_12bit_script()
{
#ifndef CC_VB1_DISABLE
  vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_POSDIV);
  vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x01, RG_VPLL_PREDIV)|P_Fld(0x01, RG_VPLL_FBKSEL)
									   |P_Fld(24, RG_VPLL_FBKDIV));
  // MT5395 toggle VOPLL Power
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_LVDS_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_VB1_TX_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG3, 1, RG_PPLL_PWD);		
  HAL_Delay_us(400);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_PWD); 
  vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_LVDS_PWD);
  vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_VB1_TX_PWD);
  HAL_Delay_us(20);

  vIO32WriteFldAlign(VB1SYS0,0x3,VB1SYS0_COL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1,FLD_OCLK_DIV_SEL);
  vIO32WriteFldAlign(VB1SYS1,0,VB1SYS1_ttl_reorder);
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2,FLD_VB1_CK_SEL);
  //vVByOneAnalogSetting();
  //vVByOneDigitalReset();
  vIO32WriteFldAlign(VB1SYS0,0x2,VB1SYS0_lane_num);
  vIO32WriteFldAlign(VB1SYS0,0xf,VB1SYS0_lane_en_mux);
#endif  
}

///////////////////////////////////////////////////////////////
//////////////////////8 bits///////////////////////////////////
void Normal_60HzSEQ(void)
{
#ifndef CC_VB1_DISABLE
  //////normal 60HZ SEQ ssoclk:tcon_oclk = 1:1
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00200000 RW  // tcon_oclk = 1:1
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  ////I2C_EXT 58 00003604[31:00] 055605f8 RW   // htotal/hactive       
  //I2C_EXT 58 00003604[31:00] 0AAC0bf0 RW   // htotal/hactive   
  //I2C_EXT 58 00003608[31:00] 03000316 RW   // vtotal/vactive
  //I2C_EXT 58 00003618[31:00] 0201001e RW   //use HH pattern , two channel
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010003 RW   // vb1 reorder
  //I2C_EXT 58 00004818[31:00] 7D634401 RW    // modify pinmux
  //I2C_EXT 58 00004814[31:00] 00000315 RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00200000 RW  // tcon_oclk = 1:1 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);
  
  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x0aac0bf0);
  vIO32Write4B(VINTF_B2, 0x03000316);
  vIO32Write4B(VINTF_B6, 0x0201001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010003 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 3, vb1_reorder);
  
  vIO32Write4B(VB1SYS1, 0x7D634401);
  vIO32Write4B(VB1SYS0, 0x00000315);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}

void Normal_120Hz_8bitSEQ(void)
{
#ifndef CC_VB1_DISABLE
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00000000 RW  // tcon_oclk = 1:2
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  //I2C_EXT 58 00003604[31:00] 03c0044c RW   // htotal/hactive /2
  //I2C_EXT 58 00003618[31:00] 0207001e RW   //use seq pattern , two channel
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010000 RW   // vb1 reorder 0
  //I2C_EXT 58 00004818[31:00] 7D634400 RW    // modify pinmux
  //I2C_EXT 58 00004814[31:00] 00000f19 RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00000000 RW  // tcon_oclk = 1:2 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_DIV_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);

  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x03c0044c);
  vIO32Write4B(VINTF_B6, 0x0207001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010000 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 0, vb1_reorder);

  vIO32Write4B(VB1SYS1, 0x7D634400);
  vIO32Write4B(VB1SYS0, 0x00000f19);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}

void Normal_120Hz_8bitSEQ_60HzPanel(void)
{
#ifndef CC_VB1_DISABLE
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00000000 RW  // tcon_oclk = 1:2
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  //I2C_EXT 58 00003604[31:00] 03c0044c RW   // htotal/hactive /2
  //I2C_EXT 58 00003618[31:00] 0203001e RW   //use HH pattern , two channel
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010001 RW   // vb1 reorder 1
  //I2C_EXT 58 00004818[31:00] 7D634401 RW    // modify pinmux , use 60HZ panel and every time check one lane
  //I2C_EXT 58 00004814[31:00] 00000f19 RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00000000 RW  // tcon_oclk = 1:2 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_DIV_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);

  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x03c0044c);
  vIO32Write4B(VINTF_B6, 0x0203001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010001 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 1, vb1_reorder);
  
  vIO32Write4B(VB1SYS1, 0x7D634401);
  vIO32Write4B(VB1SYS0, 0x00000f19);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}

void Normal_240Hz_8bitSEQ_Lane1_4(void)
{
#ifndef CC_VB1_DISABLE
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  //I2C_EXT 58 00003604[31:00] 07800898 RW   // htotal/hactive
  //I2C_EXT 58 00003618[31:00] 0207001e RW   //use seq pattern
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010002 RW   // vb1 reorder 2
  //I2C_EXT 58 00004818[31:00] 7D634400 RW    // modify pinmux
  //I2C_EXT 58 00004814[31:00] 0000ff19 RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);
  
  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x07800898);
  vIO32Write4B(VINTF_B6, 0x0207001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010002 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 2, vb1_reorder);

  vIO32Write4B(VB1SYS1, 0x7D634400);
  vIO32Write4B(VB1SYS0, 0x0000ff19);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}

void Normal_240Hz_8bitSEQ_Lane5_8(void)
{
#ifndef CC_VB1_DISABLE
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  //I2C_EXT 58 00003604[31:00] 07800898 RW   // htotal/hactive
  //I2C_EXT 58 00003618[31:00] 0207001e RW   //use seq pattern
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010002 RW   // vb1 reorder 2
  //I2C_EXT 58 00004818[31:00] 3447D600 RW    // modify pinmux
  //I2C_EXT 58 00004814[31:00] 0000ff19 RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);
  
  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x07800898);
  vIO32Write4B(VINTF_B6, 0x0207001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010002 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 2, vb1_reorder);

  vIO32Write4B(VB1SYS1, 0x3447D600);
  vIO32Write4B(VB1SYS0, 0x0000ff19);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}


void Normal_240Hz_8bitSEQ_Lane1_4_HH_reorder3(void)
{
#ifndef CC_VB1_DISABLE
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  //I2C_EXT 58 00003604[31:00] 07800898 RW   // htotal/hactive
  //I2C_EXT 58 00003618[31:00] 0203001e RW   //use HH pattern
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010003 RW   // vb1 reorder 3
  //I2C_EXT 58 00004818[31:00] 7D634400 RW    // modify pinmux
  //I2C_EXT 58 00004814[31:00] 0000ff19 RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);
  
  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x07800898);
  vIO32Write4B(VINTF_B6, 0x0203001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010003 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 3, vb1_reorder);
  
  vIO32Write4B(VB1SYS1, 0x7D634400);
  vIO32Write4B(VB1SYS0, 0x0000ff19);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}

void Normal_240Hz_8bitSEQ_Lane5_8_HH_reorder3(void)
{
#ifndef CC_VB1_DISABLE
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  //I2C_EXT 58 00003604[31:00] 07800898 RW   // htotal/hactive
  //I2C_EXT 58 00003618[31:00] 0203001e RW   //use HH pattern
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010003 RW   // vb1 reorder 3
  //I2C_EXT 58 00004818[31:00] 3447D600 RW    // modify pinmux
  //I2C_EXT 58 00004814[31:00] 0000ff19 RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);
  
  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x07800898);
  vIO32Write4B(VINTF_B6, 0x0203001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010003 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 3, vb1_reorder);
  
  vIO32Write4B(VB1SYS1, 0x3447D600);
  vIO32Write4B(VB1SYS0, 0x0000ff19);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}

///////////////////////////////////////////////////////////////
//////////////////////10 bits//////////////////////////////////

void Normal_120Hz_10bitSEQ_reorder0(void)
{
#ifndef CC_VB1_DISABLE
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00000000 RW  // tcon_oclk = 1:2
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  //I2C_EXT 58 00003604[31:00] 03c0044c RW   // htotal/hactive /2
  //I2C_EXT 58 00003618[31:00] 0207001e RW   //use seq pattern , two channel
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010000 RW   // vb1 reorder 0
  //I2C_EXT 58 00004818[31:00] 7D634400 RW    // modify pinmux
  //I2C_EXT 58 00004814[31:00] 00000f1a RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00000000 RW  // tcon_oclk = 1:2 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_DIV_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);

  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x03c0044c);
  vIO32Write4B(VINTF_B6, 0x0207001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010000 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 0, vb1_reorder);

  
  vIO32Write4B(VB1SYS1, 0x7D634400);
  vIO32Write4B(VB1SYS0, 0x00000f1a);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}

void Normal_120Hz_10bitSEQ_reorder1(void)
{
#ifndef CC_VB1_DISABLE
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00000000 RW  // tcon_oclk = 1:2
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  //I2C_EXT 58 00003604[31:00] 03c0044c RW   // htotal/hactive /2
  //I2C_EXT 58 00003618[31:00] 0203001e RW   //use HH pattern , two channel
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010001 RW   // vb1 reorder 1
  //I2C_EXT 58 00004818[31:00] 7D634401 RW    // modify pinmux , use 60HZ panel and every time check one lane
  //I2C_EXT 58 00004814[31:00] 00000f1a RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00000000 RW  // tcon_oclk = 1:2 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_DIV_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);
  
  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x03c0044c);
  vIO32Write4B(VINTF_B6, 0x0203001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010001 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 1, vb1_reorder);
  
  vIO32Write4B(VB1SYS1, 0x7D634401);
  vIO32Write4B(VB1SYS0, 0x00000f1a);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}

void Normal_240Hz_10bitSEQ_Lane1_4_reorder2(void)
{
#ifndef CC_VB1_DISABLE
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  //I2C_EXT 58 00003604[31:00] 07800898 RW   // htotal/hactive
  //I2C_EXT 58 00003618[31:00] 0207001e RW   //use seq pattern
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010002 RW   // vb1 reorder 2
  //I2C_EXT 58 00004818[31:00] 7D634400 RW    // modify pinmux
  //I2C_EXT 58 00004814[31:00] 0000ff1a RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);
  
  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x07800898);
  vIO32Write4B(VINTF_B6, 0x0207001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010002 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 2, vb1_reorder);
  
  vIO32Write4B(VB1SYS1, 0x7D634400);
  vIO32Write4B(VB1SYS0, 0x0000ff1a);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}

void Normal_240Hz_10bitSEQ_Lane5_8_reorder2(void)
{
#ifndef CC_VB1_DISABLE
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  //I2C_EXT 58 00003604[31:00] 07800898 RW   // htotal/hactive
  //I2C_EXT 58 00003618[31:00] 0207001e RW   //use seq pattern
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010002 RW   // vb1 reorder 2
  //I2C_EXT 58 00004818[31:00] 3447D600 RW    // modify pinmux
  //I2C_EXT 58 00004814[31:00] 0000ff1a RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);
  
  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x07800898);
  vIO32Write4B(VINTF_B6, 0x0207001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010002 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 2, vb1_reorder);
  
  vIO32Write4B(VB1SYS1, 0x3447D600);
  vIO32Write4B(VB1SYS0, 0x0000ff1a);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}

void Normal_240Hz_10bitSEQ_Lane1_4_reorder3(void)
{
#ifndef CC_VB1_DISABLE
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  //I2C_EXT 58 00003604[31:00] 07800898 RW   // htotal/hactive
  //I2C_EXT 58 00003618[31:00] 0203001e RW   //use HH pattern
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010003 RW   // vb1 reorder 3
  //I2C_EXT 58 00004818[31:00] 7D634400 RW    // modify pinmux
  //I2C_EXT 58 00004814[31:00] 0000ff1a RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);
  
  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x07800898);
  vIO32Write4B(VINTF_B6, 0x0203001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010003 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 3, vb1_reorder);
  
  vIO32Write4B(VB1SYS1, 0x7D634400);
  vIO32Write4B(VB1SYS0, 0x0000ff1a);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}

void Normal_240Hz_10bitSEQ_Lane5_8_reorder3(void)
{
#ifndef CC_VB1_DISABLE
  //I2C_EXT 58 0000482c[31:00] 00000000 RW  // vb1 off
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1
  //I2C_EXT 58 00003600[31:00] 0013a359 RW   // bypass 6 lane
  //I2C_EXT 58 00003604[31:00] 07800898 RW   // htotal/hactive
  //I2C_EXT 58 00003618[31:00] 0203001e RW   //use HH pattern
  //I2C_EXT 58 00004000[31:00] a002a395 RW //mini - VB1
  //I2C_EXT 58 00003014[31:00] 00010003 RW   // vb1 reorder 3
  //I2C_EXT 58 00004818[31:00] 3447D600 RW    // modify pinmux
  //I2C_EXT 58 00004814[31:00] 0000ff1a RW   // normal pattern
  //I2C_EXT 58 0000482c[31:00] 00000001 RW  // begin

  vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_start);
  //I2C_EXT 58 00001c00[31:00] 00100000 RW  // tcon_oclk = 1:1 CKGEN 待查
  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_VB1_CK_SEL);
//  vIO32WriteFldAlign(CKGEN_OCLK_CKCFG, 1, FLD_OCLK_SEL);
  
  vIO32Write4B(VINTF_B0, 0x0013a359);
  vIO32Write4B(VINTF_B1, 0x07800898);
  vIO32Write4B(VINTF_B6, 0x0203001e);
  vIO32Write4B(MLVDS_B0, 0xa002a395);
  //I2C_EXT 58 00003014[31:00] 00010003 RW   // vb1 reorder  YW 待查
  vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
//  vIO32WriteFldAlign(TMGR5, 3, vb1_reorder);
  
  vIO32Write4B(VB1SYS1, 0x3447D600);
  vIO32Write4B(VB1SYS0, 0x0000ff1a);
  vIO32Write4B(VB1SYS6, 0x00000001);

#endif  
}


 
