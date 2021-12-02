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
 * $RCSfile: drv_vbyone.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "general.h"
#include "hw_vbyone.h"
#include "x_typedef.h"
#include "drv_lvds.h"
#include "hw_mlvds.h"
#include "drv_vbyone.h"

/**
 * @brief vVByOneAnalogSetting
 * VB1 analog setting  
 * @param  void
 * @retval void
 */
void vVByOneAnalogSetting(void)
{

    #if 0
	vIO32Write4B(0xf0035844,0x17441882); 
	vIO32Write4B(0xf0035840,0x88888888);
	vIO32Write4B(0xf0035838,0x17441882);
	vIO32Write4B(0xf0035848,0x17441882);
	vIO32Write4B(0xf003584c,0x17441882);
	vIO32Write4B(0xf0035850,0x17441882);
	vIO32Write4B(0xf0035854,0x17441882);
	vIO32Write4B(0xf0035858,0x00260000); 
    #else

    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_txsr);
    vIO32WriteFldAlign(ANA0, 1, ANA0_vb1a_txpre_en);
    vIO32WriteFldAlign(ANA0, 3, ANA0_vb1a_txpre_amp);
    vIO32WriteFldAlign(ANA0, 1, ANA0_vb1a_txdrv_en);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_txbuf_mon);
    vIO32WriteFldAlign(ANA0, 4, ANA0_vb1a_txbuf_amp);
    vIO32WriteFldAlign(ANA0, 1, ANA0_vb1a_txamp_tst);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_txamp_cal);
    vIO32WriteFldAlign(ANA0, 6, ANA0_vb1a_txamp);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_tckpll_en);
    vIO32WriteFldAlign(ANA0, 1, ANA0_vb1a_impcal_pwd);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_imp);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_bist_en);
    vIO32WriteFldAlign(ANA0, 1, ANA0_vb1a_ampclk_pwd);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_abist_en);

   
    vIO32WriteFldAlign(ANA1, 1, ANA1_epib_tx_en);
    vIO32WriteFldAlign(ANA1, 0, ANA1_epib_tstst_en);
    vIO32WriteFldAlign(ANA1, 0, ANA1_epib_txsra);
    vIO32WriteFldAlign(ANA1, 1, ANA1_epia_tx_en);
    vIO32WriteFldAlign(ANA1, 0, ANA1_epia_tstst_en);
    vIO32WriteFldAlign(ANA1, 0, ANA1_epia_txsra);
    vIO32WriteFldAlign(ANA1, 1, ANA1_vb1d_tx_en);
    vIO32WriteFldAlign(ANA1, 0, ANA1_vb1d_tstst_en);
    vIO32WriteFldAlign(ANA1, 0, ANA1_vb1d_txsra);
    vIO32WriteFldAlign(ANA1, 1, ANA1_vb1c_tx_en);
    vIO32WriteFldAlign(ANA1, 0, ANA1_vb1c_tstst_en);
    vIO32WriteFldAlign(ANA1, 0, ANA1_vb1c_txsra);
    vIO32WriteFldAlign(ANA1, 1, ANA1_vb1b_tx_en);
    vIO32WriteFldAlign(ANA1, 0, ANA1_vb1b_tstst_en);
    vIO32WriteFldAlign(ANA1, 0, ANA1_vb1b_txsra);
    vIO32WriteFldAlign(ANA1, 1, ANA1_vb1a_tx_en);
    vIO32WriteFldAlign(ANA1, 0, ANA1_vb1a_tstst_en);
    vIO32WriteFldAlign(ANA1, 0, ANA1_vb1a_txsra);


    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_txsr);
    vIO32WriteFldAlign(ANA2, 1, ANA2_vb1b_txpre_en);
    vIO32WriteFldAlign(ANA2, 3, ANA2_vb1b_txpre_amp);
    vIO32WriteFldAlign(ANA2, 1, ANA2_vb1b_txdrv_en);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_txbuf_mon);
    vIO32WriteFldAlign(ANA2, 4, ANA2_vb1b_txbuf_amp);
    vIO32WriteFldAlign(ANA2, 1, ANA2_vb1b_txamp_tst);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_txamp_cal);
    vIO32WriteFldAlign(ANA2, 6, ANA2_vb1b_txamp);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_tckpll_en);
    vIO32WriteFldAlign(ANA2, 1, ANA2_vb1b_impcal_pwd);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_imp);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_bist_en);
    vIO32WriteFldAlign(ANA2, 1, ANA2_vb1b_ampclk_pwd);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_abist_en);


    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_txsr);
    vIO32WriteFldAlign(ANA3, 1, ANA3_vb1c_txpre_en);
    vIO32WriteFldAlign(ANA3, 3, ANA3_vb1c_txpre_amp);
    vIO32WriteFldAlign(ANA3, 1, ANA3_vb1c_txdrv_en);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_txbuf_mon);
    vIO32WriteFldAlign(ANA3, 4, ANA3_vb1c_txbuf_amp);
    vIO32WriteFldAlign(ANA3, 1, ANA3_vb1c_txamp_tst);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_txamp_cal);
    vIO32WriteFldAlign(ANA3, 6, ANA3_vb1c_txamp);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_tckpll_en);
    vIO32WriteFldAlign(ANA3, 1, ANA3_vb1c_impcal_pwd);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_imp);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_bist_en);
    vIO32WriteFldAlign(ANA3, 1, ANA3_vb1c_ampclk_pwd);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_abist_en);

    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_txsr);
    vIO32WriteFldAlign(ANA4, 1, ANA4_vb1d_txpre_en);
    vIO32WriteFldAlign(ANA4, 3, ANA4_vb1d_txpre_amp);
    vIO32WriteFldAlign(ANA4, 1, ANA4_vb1d_txdrv_en);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_txbuf_mon);
    vIO32WriteFldAlign(ANA4, 4, ANA4_vb1d_txbuf_amp);
    vIO32WriteFldAlign(ANA4, 1, ANA4_vb1d_txamp_tst);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_txamp_cal);
    vIO32WriteFldAlign(ANA4, 6, ANA4_vb1d_txamp);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_tckpll_en);
    vIO32WriteFldAlign(ANA4, 1, ANA4_vb1d_impcal_pwd);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_imp);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_bist_en);
    vIO32WriteFldAlign(ANA4, 1, ANA4_vb1d_ampclk_pwd);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_abist_en);
    

    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_txsr);
    vIO32WriteFldAlign(ANA5, 1, ANA5_EPIA_txpre_en);
    vIO32WriteFldAlign(ANA5, 3, ANA5_EPIA_txpre_amp);
    vIO32WriteFldAlign(ANA5, 1, ANA5_EPIA_txdrv_en);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_txbuf_mon);
    vIO32WriteFldAlign(ANA5, 4, ANA5_EPIA_txbuf_amp);
    vIO32WriteFldAlign(ANA5, 1, ANA5_EPIA_txamp_tst);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_txamp_cal);
    vIO32WriteFldAlign(ANA5, 6, ANA5_EPIA_txamp);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_tckpll_en);
    vIO32WriteFldAlign(ANA5, 1, ANA5_EPIA_impcal_pwd);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_imp);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_bist_en);
    vIO32WriteFldAlign(ANA5, 1, ANA5_EPIA_ampclk_pwd);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_abist_en);


    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_txsr);
    vIO32WriteFldAlign(ANA6, 1, ANA6_EPIB_txpre_en);
    vIO32WriteFldAlign(ANA6, 3, ANA6_EPIB_txpre_amp);
    vIO32WriteFldAlign(ANA6, 1, ANA6_EPIB_txdrv_en);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_txbuf_mon);
    vIO32WriteFldAlign(ANA6, 4, ANA6_EPIB_txbuf_amp);
    vIO32WriteFldAlign(ANA6, 1, ANA6_EPIB_txamp_tst);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_txamp_cal);
    vIO32WriteFldAlign(ANA6, 6, ANA6_EPIB_txamp);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_tckpll_en);
    vIO32WriteFldAlign(ANA6, 1, ANA6_EPIB_impcal_pwd);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_imp);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_bist_en);
    vIO32WriteFldAlign(ANA6, 1, ANA6_EPIB_ampclk_pwd);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_abist_en);
    

    vIO32WriteFldAlign(ANA7, 0, ANA7_vb1_reset);
    vIO32WriteFldAlign(ANA7, 1, ANA7_impcal_pwd);
    vIO32WriteFldAlign(ANA7, 0, ANA7_mpxsel_imp);
    vIO32WriteFldAlign(ANA7, 0, ANA7_vb1top_comp_pd);
    vIO32WriteFldAlign(ANA7, 1, ANA7_vb1topr_monaiopd);
    vIO32WriteFldAlign(ANA7, 1, ANA7_vb1topl_monaiopd);
    vIO32WriteFldAlign(ANA7, 0, ANA7_vb1topr_monsel);
    vIO32WriteFldAlign(ANA7, 0, ANA7_vb1topl_monsel);
    vIO32WriteFldAlign(ANA7, 0, ANA7_vb1_dum);
    vIO32WriteFldAlign(ANA7, 0, ANA7_epi_det10k_pd);
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
	vIO32Write4B(0xf000d1a8,0x00214080);
	vIO32Write4B(0xf000d1ac,0x80800000);
	vIO32Write4B(0xf000d1a4,0x712c92e0);
	vIO32Write4B(0xf000d1a0,0x00020e00);
	vIO32Write4B(0xf000d1b8,0x00214080);
	vIO32Write4B(0xf000d1bc,0x80800000);
	vIO32Write4B(0xf000d1b4,0x712c92e0);
	vIO32Write4B(0xf000d1b0,0x00020e00);
	vIO32Write4B(0xf000d1b0,0x80020e00);
	vIO32Write4B(0xf000d1a8,0x00224080);
	vIO32Write4B(0xf000d1a4,0x712c82e0);
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
	vIO32Write4B(0xf000d1a8,0x40214080);
	vIO32Write4B(0xf000d1ac,0x80800000);
	vIO32Write4B(0xf000d1a4,0x392a94e0);
	vIO32Write4B(0xf000d1a0,0x00021200);
	vIO32Write4B(0xf000d1b8,0x40214080);
	vIO32Write4B(0xf000d1bc,0x80800000);
	vIO32Write4B(0xf000d1b4,0x392a94e0);
	vIO32Write4B(0xf000d1b0,0x00021200);
	vIO32Write4B(0xf000d1b0,0x80021200);
	vIO32Write4B(0xf000d1a8,0x40224080);
	vIO32Write4B(0xf000d1a4,0x392a84e0);
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
	vIO32Write4B(0xf000d1a8,0x10214080);
	vIO32Write4B(0xf000d1ac,0x80800000);
	vIO32Write4B(0xf000d1a4,0x712c92e0);
	vIO32Write4B(0xf000d1a0,0x00020e00);
	vIO32Write4B(0xf000d1b8,0x10214080);
	vIO32Write4B(0xf000d1bc,0x80800000);
	vIO32Write4B(0xf000d1b4,0x712c92e0);
	vIO32Write4B(0xf000d1b0,0x00020e00);
	vIO32Write4B(0xf000d1b0,0x80020e00);
	vIO32Write4B(0xf000d1a8,0x10224080);
	vIO32Write4B(0xf000d1a4,0x712c82e0);
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
	vIO32Write4B(0xf000d1a8,0x50214080);
	vIO32Write4B(0xf000d1ac,0x80800000);
	vIO32Write4B(0xf000d1a4,0x392a94e0);
	vIO32Write4B(0xf000d1a0,0x00021200);
	vIO32Write4B(0xf000d1b8,0x50214080);
	vIO32Write4B(0xf000d1bc,0x80800000);
	vIO32Write4B(0xf000d1b4,0x392a94e0);
	vIO32Write4B(0xf000d1b0,0x00021200);
	vIO32Write4B(0xf000d1b0,0x80021200);
	vIO32Write4B(0xf000d1a8,0x50224080);
	vIO32Write4B(0xf000d1a4,0x392a84e0);
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
	vIO32Write4B(0xf000d1a8,0x30214080);
	vIO32Write4B(0xf000d1ac,0x80800000);
	vIO32Write4B(0xf000d1a4,0x712c92e0);
	vIO32Write4B(0xf000d1a0,0x00020e00);
	vIO32Write4B(0xf000d1b8,0x30214080);
	vIO32Write4B(0xf000d1bc,0x80800000);
	vIO32Write4B(0xf000d1b4,0x712c92e0);
	vIO32Write4B(0xf000d1b0,0x00020e00);
	vIO32Write4B(0xf000d1b0,0x80020e00);
	vIO32Write4B(0xf000d1a8,0x30224080);
	vIO32Write4B(0xf000d1a4,0x712c82e0);
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
	vIO32Write4B(0xf000d1a8,0x70214080);
	vIO32Write4B(0xf000d1ac,0x80800000);
	vIO32Write4B(0xf000d1a4,0x392a94e0);
	vIO32Write4B(0xf000d1a0,0x00021200);
	vIO32Write4B(0xf000d1b8,0x70214080);
	vIO32Write4B(0xf000d1bc,0x80800000);
	vIO32Write4B(0xf000d1b4,0x392a94e0);
	vIO32Write4B(0xf000d1b0,0x00021200);
	vIO32Write4B(0xf000d1b0,0x80021200);
	vIO32Write4B(0xf000d1a8,0x70224080);
	vIO32Write4B(0xf000d1a4,0x392a84e0);
	vVByOneAnalogSetting();
}
void vVByOneInit(void)
{
#if 1
UINT8 i;
UINT32 u4tmp;

	i=0;
	u4tmp=0;
	do
	{
		u4tmp = u4IO32Read4B(0xf000d734);
		vUtDelay2us(10);
		i++;
		if(i>200)
		{
			printf("VB1 Vpll clock check error......\n ");
			break;
		}
		
	}while((u4tmp &0x8000)!=0x8000);

    #if 0
	vIO32Write4B(0xf0035814,0x0000079a);
	vIO32Write4B(0xf0035818,0x00787200);
	vIO32Write4B(0xf003581c,0xFFFFFFFF);
	vIO32Write4B(0xf0035820,0x000003FF);
	vIO32Write4B(0xf003582c,0x00000003);
    vIO32Write4B(0xf000d2a8,0x00000083);
    vIO32Write4B(0xf000d5e0,0x00000001);
    #else

    vIO32WriteFldAlign(SYS0, 0, SYS0_bit_inv);
    vIO32WriteFldAlign(SYS0, 0, SYS0_vsyn_inv);
    vIO32WriteFldAlign(SYS0, 0, SYS0_hsyn_inv);
    vIO32WriteFldAlign(SYS0, 1, SYS0_lane_3_en);
    vIO32WriteFldAlign(SYS0, 1, SYS0_lane_2_en);
    vIO32WriteFldAlign(SYS0, 1, SYS0_lane_1_en);
    vIO32WriteFldAlign(SYS0, 1, SYS0_lane_0_en);
    vIO32WriteFldAlign(SYS0, 0, SYS0_FIELDBET);
    vIO32WriteFldAlign(SYS0, 0, SYS0_patgen_start);
    vIO32WriteFldAlign(SYS0, 1, SYS0_de_rst_ctrl);
    vIO32WriteFldAlign(SYS0, 2, SYS0_lane_num);
    vIO32WriteFldAlign(SYS0, 2, SYS0_COL);


    vIO32WriteFldAlign(SYS1, 0, SYS1_crc_en);
    vIO32WriteFldAlign(SYS1, 0, SYS1_lane_prbs_en);
    vIO32WriteFldAlign(SYS1, 1, SYS1_lane3_ana);
    vIO32WriteFldAlign(SYS1, 1, SYS1_lane2_ana);
    vIO32WriteFldAlign(SYS1, 1, SYS1_lane1_ana);
    vIO32WriteFldAlign(SYS1, 1, SYS1_lane0_ana);
    vIO32WriteFldAlign(SYS1, 0, SYS1_lane3_inv);
    vIO32WriteFldAlign(SYS1, 0, SYS1_lane2_inv);
    vIO32WriteFldAlign(SYS1, 0, SYS1_lane1_inv);
    vIO32WriteFldAlign(SYS1, 0, SYS1_lane0_inv);
    vIO32WriteFldAlign(SYS1, 3, SYS1_lane3_mux);
    vIO32WriteFldAlign(SYS1, 2, SYS1_lane2_mux);
    vIO32WriteFldAlign(SYS1, 1, SYS1_lane1_mux);
    vIO32WriteFldAlign(SYS1, 0, SYS1_lane0_mux);
    vIO32WriteFldAlign(SYS1, 0, SYS1_mon_sel);
    vIO32WriteFldAlign(SYS1, 0, SYS1_ttl_reorder);

	vIO32Write4B(SYS2,0xFFFFFFFF);
         
    vIO32WriteFldAlign(SYS3, 0, SYS3_ana_rx_en);
    vIO32WriteFldAlign(SYS3, 0, SYS3_rx_lane0);
    vIO32WriteFldAlign(SYS3, 0x3FF, SYS3_lane3_ana_value);

    vIO32WriteFldAlign(SYS6, 0, SYS6_fifo_ctrl);
    vIO32WriteFldAlign(SYS6, 1, SYS6_fifo_en);
    vIO32WriteFldAlign(SYS6, 1, SYS6_start);
	
	vIO32Write4B(0xf000d2a8,0x00000083);
	vIO32Write4B(0xf000d5e0,0x00000001);
	#endif

    #if 0
	vIO32Write4B(0xf0035838,0x01081802);
    vIO32Write4B(0xf0035840,0x00000000);
	vIO32Write4B(0xf0035844,0x01081802);
	vIO32Write4B(0xf0035848,0x01081802);
	vIO32Write4B(0xf003584c,0x01081802);
	vIO32Write4B(0xf0035850,0x01081802);
	vIO32Write4B(0xf0035854,0x01081802);
	vIO32Write4B(0xf0035858,0x00060000);
	vIO32Write4B(SYS5,0x00000240);
	vIO32Write4B(SYS5,0x000002d0);
    #else
    	
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_txsr);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_txpre_en);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_txpre_amp);
    vIO32WriteFldAlign(ANA0, 1, ANA0_vb1a_txdrv_en);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_txbuf_mon);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_txbuf_amp);
    vIO32WriteFldAlign(ANA0, 2, ANA0_vb1a_txamp_tst);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_txamp_cal);
    vIO32WriteFldAlign(ANA0, 6, ANA0_vb1a_txamp);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_tckpll_en);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_impcal_pwd);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_imp);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_bist_en);
    vIO32WriteFldAlign(ANA0, 1, ANA0_vb1a_ampclk_pwd);
    vIO32WriteFldAlign(ANA0, 0, ANA0_vb1a_abist_en);

    vIO32Write4B(0xf0035840,0x00000000);
  
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_txsr);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_txpre_en);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_txpre_amp);
    vIO32WriteFldAlign(ANA2, 1, ANA2_vb1b_txdrv_en);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_txbuf_mon);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_txbuf_amp);
    vIO32WriteFldAlign(ANA2, 2, ANA2_vb1b_txamp_tst);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_txamp_cal);
    vIO32WriteFldAlign(ANA2, 6, ANA2_vb1b_txamp);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_tckpll_en);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_impcal_pwd);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_imp);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_bist_en);
    vIO32WriteFldAlign(ANA2, 1, ANA2_vb1b_ampclk_pwd);
    vIO32WriteFldAlign(ANA2, 0, ANA2_vb1b_abist_en);

    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_txsr);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_txpre_en);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_txpre_amp);
    vIO32WriteFldAlign(ANA3, 1, ANA3_vb1c_txdrv_en);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_txbuf_mon);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_txbuf_amp);
    vIO32WriteFldAlign(ANA3, 2, ANA3_vb1c_txamp_tst);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_txamp_cal);
    vIO32WriteFldAlign(ANA3, 6, ANA3_vb1c_txamp);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_tckpll_en);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_impcal_pwd);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_imp);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_bist_en);
    vIO32WriteFldAlign(ANA3, 1, ANA3_vb1c_ampclk_pwd);
    vIO32WriteFldAlign(ANA3, 0, ANA3_vb1c_abist_en);

    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_txsr);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_txpre_en);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_txpre_amp);
    vIO32WriteFldAlign(ANA4, 1, ANA4_vb1d_txdrv_en);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_txbuf_mon);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_txbuf_amp);
    vIO32WriteFldAlign(ANA4, 2, ANA4_vb1d_txamp_tst);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_txamp_cal);
    vIO32WriteFldAlign(ANA4, 6, ANA4_vb1d_txamp);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_tckpll_en);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_impcal_pwd);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_imp);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_bist_en);
    vIO32WriteFldAlign(ANA4, 1, ANA4_vb1d_ampclk_pwd);
    vIO32WriteFldAlign(ANA4, 0, ANA4_vb1d_abist_en);
   
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_txsr);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_txpre_en);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_txpre_amp);
    vIO32WriteFldAlign(ANA5, 1, ANA5_EPIA_txdrv_en);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_txbuf_mon);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_txbuf_amp);
    vIO32WriteFldAlign(ANA5, 2, ANA5_EPIA_txamp_tst);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_txamp_cal);
    vIO32WriteFldAlign(ANA5, 6, ANA5_EPIA_txamp);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_tckpll_en);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_impcal_pwd);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_imp);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_bist_en);
    vIO32WriteFldAlign(ANA5, 1, ANA5_EPIA_ampclk_pwd);
    vIO32WriteFldAlign(ANA5, 0, ANA5_EPIA_abist_en);

    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_txsr);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_txpre_en);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_txpre_amp);
    vIO32WriteFldAlign(ANA6, 1, ANA6_EPIB_txdrv_en);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_txbuf_mon);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_txbuf_amp);
    vIO32WriteFldAlign(ANA6, 2, ANA6_EPIB_txamp_tst);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_txamp_cal);
    vIO32WriteFldAlign(ANA6, 6, ANA6_EPIB_txamp);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_tckpll_en);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_impcal_pwd);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_imp);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_bist_en);
    vIO32WriteFldAlign(ANA6, 1, ANA6_EPIB_ampclk_pwd);
    vIO32WriteFldAlign(ANA6, 0, ANA6_EPIB_abist_en);
    
    vIO32WriteFldAlign(ANA7, 0, ANA7_vb1_reset);
    vIO32WriteFldAlign(ANA7, 0, ANA7_impcal_pwd);
    vIO32WriteFldAlign(ANA7, 0, ANA7_mpxsel_imp);
    vIO32WriteFldAlign(ANA7, 0, ANA7_vb1top_comp_pd);
    vIO32WriteFldAlign(ANA7, 1, ANA7_vb1topr_monaiopd);
    vIO32WriteFldAlign(ANA7, 1, ANA7_vb1topl_monaiopd);
    vIO32WriteFldAlign(ANA7, 0, ANA7_vb1topr_monsel);
    vIO32WriteFldAlign(ANA7, 0, ANA7_vb1topl_monsel);
    vIO32WriteFldAlign(ANA7, 0, ANA7_vb1_dum);
    vIO32WriteFldAlign(ANA7, 0, ANA7_epi_det10k_pd);
	
    vIO32WriteFldAlign(SYS5, 0, SYS5_rx_ck_sel);
    vIO32WriteFldAlign(SYS5, 0, SYS5_imp_cal_set);
    vIO32WriteFldAlign(SYS5, 0, SYS5_rximp);
    vIO32WriteFldAlign(SYS5, 0, SYS5_rg_autok_rx_imp);
    vIO32WriteFldAlign(SYS5, 0, SYS5_rg_tximp);
    vIO32WriteFldAlign(SYS5, 1, SYS5_rg_autok_tx_imp);
    vIO32WriteFldAlign(SYS5, 0, SYS5_impfreqsel);
    vIO32WriteFldAlign(SYS5, 0, SYS5_sapis_mode_sync);
    vIO32WriteFldAlign(SYS5, 1, SYS5_sel_1_all);
    vIO32WriteFldAlign(SYS5, 0, SYS5_reg_imp_dp_dis);
    vIO32WriteFldAlign(SYS5, 0, SYS5_ctl_en_calibration);
    vIO32WriteFldAlign(SYS5, 0, SYS5_tx_clk_inv);
    vIO32WriteFldAlign(SYS5, 0, SYS5_rx_clk_inv);

    vIO32WriteFldAlign(SYS5, 1, SYS5_sapis_mode_sync);
    vIO32WriteFldAlign(SYS5, 1, SYS5_ctl_en_calibration);
    #endif


	
	i=0;
	u4tmp=0;
	do
	{
		u4tmp = u4IO32Read4B(SYS8);
		vUtDelay2us(10);
		i++;
		if(i>200)
		{
    			printf("VB1 impedance cal error then break......\n ");
    			break;
		}
		
	}while((u4tmp &0xdc)!=0xdc);
	
	u4tmp=(u4IO32Read4B(SYS8)&0xf00)>>8;
	if(u4tmp>0 && u4tmp<0xf)
	{
    		printf("VB1 VB1 impedance cal  pass and value =%x .................\n",u4tmp);
	}
	else
	{
    		printf("VB1 VB1 impedance cal  failed .........................\n");
	}
	
	vIO32WriteFldAlign(SYS1, 0x0, Fld(1, 19, AC_MSKB2));
	vIO32WriteFldAlign(SYS1, 0x0, Fld(1, 20, AC_MSKB2));
	vIO32WriteFldAlign(SYS1, 0x0, Fld(1, 21, AC_MSKB2));
	vIO32WriteFldAlign(SYS1, 0x0, Fld(1, 22, AC_MSKB2));
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
    //vIO32Write4B(0xf003582c,0x00000000);
    vIO32WriteFldAlign(SYS6, 0, SYS6_fifo_ctrl);
    vIO32WriteFldAlign(SYS6, 0, SYS6_fifo_en);
    vIO32WriteFldAlign(SYS6, 0, SYS6_start);
    
    vIO32Write4B(0xf000d5e0,0x00000001);
    vIO32Write4B(0xf0035000,0x8003a359);
    vIO32Write4B(0xf0035280,0x40000000);
}
/**
 * @brief vDrvVB1On
 * Power On VBYONE
 * @param  void
 * @retval void
 */
void vDrvVByOneOn(void)
{
    UINT32 u4DispBit;
    UINT32 u4VB1Lane;
    
    u4DispBit = DISP_BIT;
    u4VB1Lane = LVDS_OUTPUT_PORT;

     vIO32WriteFldAlign(0xf000d2a8, 0x1, Fld(1, 0, AC_MSKB0));//TCK enable
    	vIO32WriteFldAlign(ANA7,0x1,ANA7_vb1_reset);
	vIO32WriteFldAlign(ANA7,0x0,ANA7_vb1_reset);
	switch (u4DispBit)
    	{
       	 case DISP_30BIT:
       	 	
       	 	  vIO32WriteFldAlign(SYS0,0x2,SYS0_COL);
       	 	if(u4VB1Lane == FOUR_PORT)
            		{
            			//vVByOne4lane10bitAnalogSetting();
            			vVByOneAnalogSetting();
            		 	vVByOneDigitalReset();
            			vIO32Write4B(0xf000d2a8,0x00000082);
            			vIO32WriteFldAlign(SYS0,0x2,SYS0_lane_num);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_3_en);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_2_en);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_1_en);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_0_en);
       	 	}
       	 	if (u4VB1Lane == DUAL_PORT)
            		{
            			//vVByOne2lane10bitAnalogSetting();
            			vVByOneAnalogSetting();
            			vVByOneDigitalReset();
            			vIO32Write4B(0xf000d2a8,0x00000042);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_num);
            			vIO32WriteFldAlign(SYS0,0x0,SYS0_lane_3_en);
            			vIO32WriteFldAlign(SYS0,0x0,SYS0_lane_2_en);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_1_en);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_0_en);
            			
       	 	}
       	 	if (u4VB1Lane == SINGLE_PORT)
       	 	{
       	 		//vVByOne1lane10bitAnalogSetting();
       	 		vVByOneAnalogSetting();
       	 		vVByOneDigitalReset();
       	 		vIO32Write4B(0xf000d2a8,0x00000002);
       	 		vIO32WriteFldAlign(SYS0,0x0,SYS0_lane_num);
       	 		vIO32WriteFldAlign(SYS0,0x0,SYS0_lane_3_en);
            			vIO32WriteFldAlign(SYS0,0x0,SYS0_lane_2_en);
            			vIO32WriteFldAlign(SYS0,0x0,SYS0_lane_1_en);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_0_en);
       	 	}
        	break;
        	 case DISP_24BIT:
        	 	vIO32WriteFldAlign(SYS0,0x1,SYS0_COL);
       	 	if(u4VB1Lane == FOUR_PORT)
            		{
            			//vVByOne4lane8bitAnalogSetting();
            			vVByOneAnalogSetting();
            			vVByOneDigitalReset();
            			vIO32Write4B(0xf000d2a8,0x00000082);
            			vIO32WriteFldAlign(SYS0,0x2,SYS0_lane_num);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_3_en);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_2_en);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_1_en);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_0_en);
       	 	}
       	 	if (u4VB1Lane == DUAL_PORT)
            		{
            			//vVByOne2lane8bitAnalogSetting();
            			vVByOneAnalogSetting();
            			vVByOneDigitalReset();
            			vIO32Write4B(0xf000d2a8,0x00000042);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_num);
            			vIO32WriteFldAlign(SYS0,0x0,SYS0_lane_3_en);
            			vIO32WriteFldAlign(SYS0,0x0,SYS0_lane_2_en);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_1_en);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_0_en);
       	 	}
       	 	if (u4VB1Lane == SINGLE_PORT)
       	 	{
       	 		//vVByOne1lane8bitAnalogSetting();
       	 		vVByOneAnalogSetting();
       	 		vVByOneDigitalReset();
       	 		vIO32Write4B(0xf000d2a8,0x00000002);
       	 		vIO32WriteFldAlign(SYS0,0x0,SYS0_lane_num);
       	 		vIO32WriteFldAlign(SYS0,0x0,SYS0_lane_3_en);
            			vIO32WriteFldAlign(SYS0,0x0,SYS0_lane_2_en);
            			vIO32WriteFldAlign(SYS0,0x0,SYS0_lane_1_en);
            			vIO32WriteFldAlign(SYS0,0x1,SYS0_lane_0_en);
       	 	}
        	break;
        	default:
        	break;
    	}
	vIO32WriteFldAlign(SYS0,0x1,SYS0_de_rst_ctrl);
	vIO32WriteFldAlign(SYS1,0x3,SYS1_lane3_mux);
	vIO32WriteFldAlign(SYS1,0x2,SYS1_lane2_mux);
	vIO32WriteFldAlign(SYS1,0x1,SYS1_lane1_mux);
	vIO32WriteFldAlign(SYS1,0x0,SYS1_lane0_mux);
	vIO32WriteFldAlign(SYS1,0x0,SYS1_mon_sel);
	vIO32WriteFldAlign(SYS1,0x0,SYS1_ttl_reorder);

	//vIO32Write4B(0xf003582c,0x00000003);
	vIO32WriteFldAlign(SYS6, 0, SYS6_fifo_ctrl);
    vIO32WriteFldAlign(SYS6, 1, SYS6_fifo_en);
    vIO32WriteFldAlign(SYS6, 1, SYS6_start);
	
}

/**
 * @brief vDrvVB1Off
 * Power off VBYONE digital
 * @param  void
 * @retval void
 */
void vDrvVByOneOff(void)
{
	 //vIO32Write4B(0xf003582c,0x00000000);
	 vIO32WriteFldAlign(SYS6, 0, SYS6_fifo_ctrl);
     vIO32WriteFldAlign(SYS6, 0, SYS6_fifo_en);
     vIO32WriteFldAlign(SYS6, 0, SYS6_start);
}

/**
 * @brief vDrvVByOneCRCCheck
 * crc check
 * @param  void
 * @retval void
 */
void vDrvVByOneCRCCheck(void)
{
    UINT32 u4VB1Lane;
   
    u4VB1Lane = LVDS_OUTPUT_PORT;
    
	// if(IO32ReadFldAlign(SYS1, SYS1_crc_en)==1)
	vIO32WriteFldAlign(SYS1,0x0,SYS1_crc_en);
	vIO32WriteFldAlign(SYS1,0x1,SYS1_crc_en);
       if(u4VB1Lane == FOUR_PORT)
       {
            	if(IO32ReadFldAlign(SYS7, SYS7_crc1_fail) ||IO32ReadFldAlign(SYS7, SYS7_crc2_fail) ||IO32ReadFldAlign(SYS7, SYS7_crc3_fail)||IO32ReadFldAlign(SYS7, SYS7_crc4_cail))
            	{
            		printf("4 lane CRC fail and Fali value =%x",IO32ReadFldAlign(SYS7, SYS7_crc1_fail) |IO32ReadFldAlign(SYS7, SYS7_crc2_fail) |IO32ReadFldAlign(SYS7, SYS7_crc3_fail)|IO32ReadFldAlign(SYS7, SYS7_crc4_cail));
            	}
            	else
            	{
            		printf("4 lane CRC pass");
            	}
       }
       if (u4VB1Lane == DUAL_PORT)
       {   
            	if(IO32ReadFldAlign(SYS7, SYS7_crc1_fail) ||IO32ReadFldAlign(SYS7, SYS7_crc2_fail) )
            	{
            		printf("2 lane CRC fail and Fali value =%x",IO32ReadFldAlign(SYS7, SYS7_crc1_fail) |IO32ReadFldAlign(SYS7, SYS7_crc2_fail) );
            	}	
            	else
            	{
            		printf("2 lane CRC pass");
            	}
        }
      	if (u4VB1Lane == SINGLE_PORT)
       {
       	if(IO32ReadFldAlign(SYS7, SYS7_crc1_fail))
            	{
            		printf("1 lane CRC fail and Fali value =%x",IO32ReadFldAlign(SYS7, SYS7_crc1_fail));
            	}	
       	else
            	{
            		printf("1 lane CRC pass");
            	}
        }
}

void VByOneLane4_8bit_script()
{
  vIO32Write4B( 0xf000d1a8, 0x00214080);
  vIO32Write4B( 0xf000d1ac, 0x80800000);
  vIO32Write4B( 0xf000d1a4, 0x712c92e0);
  vIO32Write4B( 0xf000d1a0, 0x00020e00);
  vIO32Write4B( 0xf000d1b8, 0x00214080);
  vIO32Write4B( 0xf000d1bc, 0x80800000);
  vIO32Write4B( 0xf000d1b4, 0x712c92e0);
  vIO32Write4B( 0xf000d1b0, 0x00020e00);
  vIO32Write4B( 0xf000d1b0, 0x80020e00);
  vIO32Write4B( 0xf000d1a8, 0x00224080);
  vIO32Write4B( 0xf000d1a4, 0x712c82e0);
  vIO32Write4B( 0xf0035844, 0x17441882); 
  vIO32Write4B( 0xf0035840, 0x88888888);
  vIO32Write4B( 0xf0035838, 0x17441882);
  vIO32Write4B( 0xf0035848, 0x17441882);
  vIO32Write4B( 0xf003584c, 0x17441882);
  vIO32Write4B( 0xf0035850, 0x17441882);
  vIO32Write4B( 0xf0035854, 0x17441882);
  vIO32Write4B( 0xf0035858, 0x00260000); 
  vIO32Write4B( 0xf003582c, 0x00000000);
  vIO32Write4B( 0xf000d2a8, 0x00000082);
  vIO32Write4B( 0xf000d5e0, 0x00000001); 
  vIO32Write4B( 0xf0035000, 0x8003a359);
  vIO32Write4B( 0xf0035280, 0x40000000);
  vIO32Write4B( 0xf0035800, 0x00050032);
  vIO32Write4B( 0xf0035804, 0x006b62d0);
  vIO32Write4B( 0xf0035808, 0x00005014);
  vIO32Write4B( 0xf003580C, 0x000f020d);
  vIO32Write4B( 0xf0035810, 0x02ffffff);
  vIO32Write4B( 0xf0035814, 0x00000799);
  vIO32Write4B( 0xf0035818, 0x00007200);
  vIO32Write4B( 0xf003582c, 0x00000003);

}

void VByOneLane4_10bit_script()
{
vIO32Write4B( 0xf000d1a8, 0x40214080);
vIO32Write4B( 0xf000d1ac, 0x80800000);
vIO32Write4B( 0xf000d1a4, 0x392a94e0);
vIO32Write4B( 0xf000d1a0, 0x00021200);
vIO32Write4B( 0xf000d1b8, 0x40214080);
vIO32Write4B( 0xf000d1bc, 0x80800000);
vIO32Write4B( 0xf000d1b4, 0x392a94e0);
vIO32Write4B( 0xf000d1b0, 0x00021200);
vIO32Write4B( 0xf000d1b0, 0x80021200);
vIO32Write4B( 0xf000d1a8, 0x40224080);
vIO32Write4B( 0xf000d1a4, 0x392a84e0);
vIO32Write4B( 0xf0035844, 0x17441882);
vIO32Write4B( 0xf0035840, 0x88888888);
vIO32Write4B( 0xf0035838, 0x17441882);
vIO32Write4B( 0xf0035848, 0x17441882);
vIO32Write4B( 0xf003584c, 0x17441882);
vIO32Write4B( 0xf0035850, 0x17441882);
vIO32Write4B( 0xf0035854, 0x17441882);
vIO32Write4B( 0xf0035858, 0x00260000); 
vIO32Write4B( 0xf003582c, 0x00000000);
vIO32Write4B( 0xf000d2a8, 0x00000082);
vIO32Write4B( 0xf000d5e0, 0x00000001); 
vIO32Write4B( 0xf0035000, 0x8003a359);
vIO32Write4B( 0xf0035280, 0x40000000);
vIO32Write4B( 0xf0035800, 0x00050032);
vIO32Write4B( 0xf0035804, 0x006b62d0);
vIO32Write4B( 0xf0035808, 0x00005014);
vIO32Write4B( 0xf003580C, 0x000f020d);
vIO32Write4B( 0xf0035810, 0x02ffffff);
vIO32Write4B( 0xf0035814, 0x0000079a);
vIO32Write4B( 0xf0035818, 0x00007201);
vIO32Write4B( 0xf003582c, 0x00000003);
 
}

void VByOneLane4_12bit_script()
{
vIO32Write4B( 0xf000d1a8, 0xc0214080);
vIO32Write4B( 0xf000d1ac, 0x80800000);
vIO32Write4B( 0xf000d1a4, 0x393b94e0);
vIO32Write4B( 0xf000d1a0, 0x00021800);
vIO32Write4B( 0xf000d1b8, 0xc0214080);
vIO32Write4B( 0xf000d1bc, 0x80800000);
vIO32Write4B( 0xf000d1b4, 0x393b94e0);
vIO32Write4B( 0xf000d1b0, 0x00021800);
vIO32Write4B( 0xf000d1b0, 0x80021800);
vIO32Write4B( 0xf000d1a8, 0xc0224080);
vIO32Write4B( 0xf000d1a4, 0x393b84e0);
vIO32Write4B( 0xf0035844, 0x17441882); 
vIO32Write4B( 0xf0035840, 0x88888888);
vIO32Write4B( 0xf0035838, 0x17441882);
vIO32Write4B( 0xf0035848, 0x17441882);
vIO32Write4B( 0xf003584c, 0x17441882);
vIO32Write4B( 0xf0035850, 0x17441882);
vIO32Write4B( 0xf0035854, 0x17441882);
vIO32Write4B( 0xf0035858, 0x00260000);
vIO32Write4B( 0xf003582c, 0x00000000);
vIO32Write4B( 0xf000d2a8, 0x00000082);
vIO32Write4B( 0xf000d5e0, 0x00000001); 
vIO32Write4B( 0xf0035000, 0x8003a359);
vIO32Write4B( 0xf0035280, 0x40000000);
vIO32Write4B( 0xf0035800, 0x00050032);
vIO32Write4B( 0xf0035804, 0x006b62d0);
vIO32Write4B( 0xf0035808, 0x00005014);
vIO32Write4B( 0xf003580C, 0x000f020d);
vIO32Write4B( 0xf0035810, 0x02ffffff);
vIO32Write4B( 0xf0035814, 0x0000079b);
vIO32Write4B( 0xf0035818, 0x00007200);
vIO32Write4B( 0xf003582c, 0x00000003); 
 
}
 
