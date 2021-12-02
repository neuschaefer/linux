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
 * $RCSfile: hw_nr.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file NR register definition
 *
 * @author mf_tien@mtk.com.tw
 */

#ifndef _HW_NR_H_
#define _HW_NR_H_

#include "drv_nr.h"

/* mt5387 register */

#ifndef CC_UP8032_ATV
#define IO_PREPROC_BASE (0x3000)
#else
#define IO_VIRT 0
#define IO_PREPROC_BASE (0x5000)
#endif

#define NR_TOP_MAIN_00 (IO_PREPROC_BASE+0x800)
    #define c_metric_data_sel Fld(1, 27, AC_MSKB3) //27
    #define c_metric_v_inv Fld(1, 26, AC_MSKB3) //26
    #define c_det_data_sel Fld(1, 25, AC_MSKB3) //25
    #define c_det_v_inv Fld(1, 24, AC_MSKB3) //24
    #define c_m2h_inv Fld(1, 23, AC_MSKB2) //23
    #define c_m2v_inv Fld(1, 22, AC_MSKB2) //22
    #define c_m2f_inv Fld(1, 21, AC_MSKB2) //21
    #define c_main_V_rst Fld(1, 20, AC_MSKB2) //20
    #define c_crc_sel Fld(2, 2, AC_MSKB0) //3:2
    #define c_main_nr_en Fld(1, 0, AC_MSKB0) //0
#define NR_TOP_MAIN_01 (IO_PREPROC_BASE+0x804)
    #define c_pre_proc_dbg_mode Fld(4, 5, AC_MSKW10) //8:5
#define NR_2DNR00 (IO_PREPROC_BASE+0xd80)
    #define rbFullEdgeComTp Fld(3, 29, AC_MSKB3) //31:29
    #define ruiFullStEdgeComTp Fld(2, 27, AC_MSKB3) //28:27
    #define ruiprb_y_pos Fld(11, 16, AC_MSKW32) //26:16
    #define rbFullEdgeTp Fld(1, 15, AC_MSKB1) //15
    #define rbFullStTp Fld(1, 14, AC_MSKB1) //14
    #define ruiprb_x_pos Fld(11, 0, AC_MSKW10) //10:0
#define NR_2DNR01 (IO_PREPROC_BASE+0xd84)
    #define rImgWidth Fld(11, 16, AC_MSKW32) //26:16
    #define rImgHeight Fld(11, 0, AC_MSKW10) //10:0
#define NR_2DNR02 (IO_PREPROC_BASE+0xd88)
    #define rbBKMeterDetOnlyH_FrSt Fld(1, 31, AC_MSKB3) //31
    #define rbBKMeterDetOnlyH_St Fld(1, 30, AC_MSKB3) //30
    #define rbBKMeterDetOnlyH_Mo Fld(1, 29, AC_MSKB3) //29
    #define rbBKMeterDetOnlyH_BK Fld(1, 28, AC_MSKB3) //28
    #define rbBKMeterDetOnlyH_Def Fld(1, 27, AC_MSKB3) //27
    #define ruiSliceXPos Fld(11, 16, AC_MSKW32) //26:16
    #define ruiFixY Fld(8, 8, AC_FULLB1) //15:8
    #define ruiFixX Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR03 (IO_PREPROC_BASE+0xd8c)
	#define r_bBoundRepeat10Pxl Fld(1, 23, AC_MSKB2) //23
    #define ruiYStart Fld(11, 12, AC_MSKW21) //22:12
    #define rbBoundaryRepeatEn Fld(1, 11, AC_MSKB1) //11
    #define ruiXStart Fld(11, 0, AC_MSKW10) //10:0
#define NR_2DNR04 (IO_PREPROC_BASE+0xd90)
    #define rbMess3132RuleDis_Def Fld(1, 31, AC_MSKB3) //31
    #define ruiHDetTypeThl_Def Fld(3, 28, AC_MSKB3) //30:28
    #define rbMessBKBackEdgeDis_Def Fld(1, 27, AC_MSKB3) //27
    #define rbMessBKBackRuleDis_Def Fld(1, 26, AC_MSKB3) //26
    #define ruiHDetType_Def Fld(2, 24, AC_MSKB3) //25:24
    #define rbHSrcLPorCom_Def Fld(1, 23, AC_MSKB2) //23
    #define ruiDyVEdgeThlMess_Def Fld(11, 12, AC_MSKW21) //22:12
    #define rbHDetCorr_Def Fld(1, 11, AC_MSKB1) //11
    #define ruiDyVEdgeThlSM_Def Fld(11, 0, AC_MSKW10) //10:0
#define NR_2DNR05 (IO_PREPROC_BASE+0xd94)
    #define rbSMBK3x3FilterV_Def Fld(1, 29, AC_MSKB3) //29
    #define rbSMBK3x3FilterH_Def Fld(1, 28, AC_MSKB3) //28
    #define rbDetailBK5x5FilterV_Def Fld(1, 27, AC_MSKB3) //27
    #define rbDetailBK5x5FilterH_Def Fld(1, 26, AC_MSKB3) //26
    #define rbMessBKBackEdgeDisH_Def Fld(1, 25, AC_MSKB3) //25
    #define rbMessBKBackRuleDisH_Def Fld(1, 24, AC_MSKB3) //24
    #define ruiHMaxRate_Def Fld(2, 22, AC_MSKB2) //23:22
    #define ruiHMinRate_Def Fld(2, 20, AC_MSKB2) //21:20
    #define ruiVMaxRate_Def Fld(2, 18, AC_MSKB2) //19:18
    #define ruiVMinRate_Def Fld(2, 16, AC_MSKB2) //17:16
    #define ruiDyVBKThlMess_Def Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyVBKThlSM_Def Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR06 (IO_PREPROC_BASE+0xd98)
    #define ruiDyHEdgeThlMess_Def Fld(8, 24, AC_FULLB3) //31:24
    #define ruiDyHEdgeThlSM_Def Fld(8, 16, AC_FULLB2) //23:16
    #define ruiDyHBKThlMess_Def Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyHBKThlSM_Def Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR07 (IO_PREPROC_BASE+0xd9c)
    #define ruiDyHA3132MaxThlMess_Def Fld(8, 24, AC_FULLB3) //31:24
    #define ruiDyHA3132MaxThlSM_Def Fld(8, 16, AC_FULLB2) //23:16
    #define ruiDyVA3132MaxThlMess_Def Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyVA3132MaxThlSM_Def Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR08 (IO_PREPROC_BASE+0xda0)
    #define rbMess3132RuleDis_BK Fld(1, 31, AC_MSKB3) //31
    #define ruiHDetTypeThl_BK Fld(3, 28, AC_MSKB3) //30:28
    #define rbMessBKBackEdgeDis_BK Fld(1, 27, AC_MSKB3) //27
    #define rbMessBKBackRuleDis_BK Fld(1, 26, AC_MSKB3) //26
    #define ruiHDetType_BK Fld(2, 24, AC_MSKB3) //25:24
    #define rbHSrcLPorCom_BK Fld(1, 23, AC_MSKB2) //23
    #define ruiDyVEdgeThlMess_BK Fld(11, 12, AC_MSKW21) //22:12
    #define rbHDetCorr_BK Fld(1, 11, AC_MSKB1) //11
    #define ruiDyVEdgeThlSM_BK Fld(11, 0, AC_MSKW10) //10:0
#define NR_2DNR09 (IO_PREPROC_BASE+0xda4)
    #define rbSMBK3x3FilterV_BK Fld(1, 29, AC_MSKB3) //29
    #define rbSMBK3x3FilterH_BK Fld(1, 28, AC_MSKB3) //28
    #define rbDetailBK5x5FilterV_BK Fld(1, 27, AC_MSKB3) //27
    #define rbDetailBK5x5FilterH_BK Fld(1, 26, AC_MSKB3) //26
    #define rbMessBKBackEdgeDisH_BK Fld(1, 25, AC_MSKB3) //25
    #define rbMessBKBackRuleDisH_BK Fld(1, 24, AC_MSKB3) //24
    #define ruiHMaxRate_BK Fld(2, 22, AC_MSKB2) //23:22
    #define ruiHMinRate_BK Fld(2, 20, AC_MSKB2) //21:20
    #define ruiVMaxRate_BK Fld(2, 18, AC_MSKB2) //19:18
    #define ruiVMinRate_BK Fld(2, 16, AC_MSKB2) //17:16
    #define ruiDyVBKThlMess_BK Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyVBKThlSM_BK Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR0A (IO_PREPROC_BASE+0xda8)
    #define ruiDyHEdgeThlMess_BK Fld(8, 24, AC_FULLB3) //31:24
    #define ruiDyHEdgeThlSM_BK Fld(8, 16, AC_FULLB2) //23:16
    #define ruiDyHBKThlMess_BK Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyHBKThlSM_BK Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR0B (IO_PREPROC_BASE+0xdac)
    #define ruiDyHA3132MaxThlMess_BK Fld(8, 24, AC_FULLB3) //31:24
    #define ruiDyHA3132MaxThlSM_BK Fld(8, 16, AC_FULLB2) //23:16
    #define ruiDyVA3132MaxThlMess_BK Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyVA3132MaxThlSM_BK Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR0C (IO_PREPROC_BASE+0xdb0)
    #define rbMess3132RuleDis_Mo Fld(1, 31, AC_MSKB3) //31
    #define ruiHDetTypeThl_Mo Fld(3, 28, AC_MSKB3) //30:28
    #define rbMessBKBackEdgeDis_Mo Fld(1, 27, AC_MSKB3) //27
    #define rbMessBKBackRuleDis_Mo Fld(1, 26, AC_MSKB3) //26
    #define ruiHDetType_Mo Fld(2, 24, AC_MSKB3) //25:24
    #define rbHSrcLPorCom_Mo Fld(1, 23, AC_MSKB2) //23
    #define ruiDyVEdgeThlMess_Mo Fld(11, 12, AC_MSKW21) //22:12
    #define rbHDetCorr_Mo Fld(1, 11, AC_MSKB1) //11
    #define ruiDyVEdgeThlSM_Mo Fld(11, 0, AC_MSKW10) //10:0
#define NR_2DNR0D (IO_PREPROC_BASE+0xdb4)
    #define rbSMBK3x3FilterV_Mo Fld(1, 29, AC_MSKB3) //29
    #define rbSMBK3x3FilterH_Mo Fld(1, 28, AC_MSKB3) //28
    #define rbDetailBK5x5FilterV_Mo Fld(1, 27, AC_MSKB3) //27
    #define rbDetailBK5x5FilterH_Mo Fld(1, 26, AC_MSKB3) //26
    #define rbMessBKBackEdgeDisH_Mo Fld(1, 25, AC_MSKB3) //25
    #define rbMessBKBackRuleDisH_Mo Fld(1, 24, AC_MSKB3) //24
    #define ruiHMaxRate_Mo Fld(2, 22, AC_MSKB2) //23:22
    #define ruiHMinRate_Mo Fld(2, 20, AC_MSKB2) //21:20
    #define ruiVMaxRate_Mo Fld(2, 18, AC_MSKB2) //19:18
    #define ruiVMinRate_Mo Fld(2, 16, AC_MSKB2) //17:16
    #define ruiDyVBKThlMess_Mo Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyVBKThlSM_Mo Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR0E (IO_PREPROC_BASE+0xdb8)
    #define ruiDyHEdgeThlMess_Mo Fld(8, 24, AC_FULLB3) //31:24
    #define ruiDyHEdgeThlSM_Mo Fld(8, 16, AC_FULLB2) //23:16
    #define ruiDyHBKThlMess_Mo Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyHBKThlSM_Mo Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR0F (IO_PREPROC_BASE+0xdbc)
    #define ruiDyHA3132MaxThlMess_Mo Fld(8, 24, AC_FULLB3) //31:24
    #define ruiDyHA3132MaxThlSM_Mo Fld(8, 16, AC_FULLB2) //23:16
    #define ruiDyVA3132MaxThlMess_Mo Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyVA3132MaxThlSM_Mo Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR10 (IO_PREPROC_BASE+0xdc0)
    #define rbMess3132RuleDis_St Fld(1, 31, AC_MSKB3) //31
    #define ruiHDetTypeThl_St Fld(3, 28, AC_MSKB3) //30:28
    #define rbMessBKBackEdgeDis_St Fld(1, 27, AC_MSKB3) //27
    #define rbMessBKBackRuleDis_St Fld(1, 26, AC_MSKB3) //26
    #define ruiHDetType_St Fld(2, 24, AC_MSKB3) //25:24
    #define rbHSrcLPorCom_St Fld(1, 23, AC_MSKB2) //23
    #define ruiDyVEdgeThlMess_St Fld(11, 12, AC_MSKW21) //22:12
    #define rbHDetCorr_St Fld(1, 11, AC_MSKB1) //11
    #define ruiDyVEdgeThlSM_St Fld(11, 0, AC_MSKW10) //10:0
#define NR_2DNR11 (IO_PREPROC_BASE+0xdc4)
    #define rbSMBK3x3FilterV_St Fld(1, 29, AC_MSKB3) //29
    #define rbSMBK3x3FilterH_St Fld(1, 28, AC_MSKB3) //28
    #define rbDetailBK5x5FilterV_St Fld(1, 27, AC_MSKB3) //27
    #define rbDetailBK5x5FilterH_St Fld(1, 26, AC_MSKB3) //26
    #define rbMessBKBackEdgeDisH_St Fld(1, 25, AC_MSKB3) //25
    #define rbMessBKBackRuleDisH_St Fld(1, 24, AC_MSKB3) //24
    #define ruiHMaxRate_St Fld(2, 22, AC_MSKB2) //23:22
    #define ruiHMinRate_St Fld(2, 20, AC_MSKB2) //21:20
    #define ruiVMaxRate_St Fld(2, 18, AC_MSKB2) //19:18
    #define ruiVMinRate_St Fld(2, 16, AC_MSKB2) //17:16
    #define ruiDyVBKThlMess_St Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyVBKThlSM_St Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR12 (IO_PREPROC_BASE+0xdc8)
    #define ruiDyHEdgeThlMess_St Fld(8, 24, AC_FULLB3) //31:24
    #define ruiDyHEdgeThlSM_St Fld(8, 16, AC_FULLB2) //23:16
    #define ruiDyHBKThlMess_St Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyHBKThlSM_St Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR13 (IO_PREPROC_BASE+0xdcc)
    #define ruiDyHA3132MaxThlMess_St Fld(8, 24, AC_FULLB3) //31:24
    #define ruiDyHA3132MaxThlSM_St Fld(8, 16, AC_FULLB2) //23:16
    #define ruiDyVA3132MaxThlMess_St Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyVA3132MaxThlSM_St Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR14 (IO_PREPROC_BASE+0xdd0)
    #define rbMess3132RuleDis_FrSt Fld(1, 31, AC_MSKB3) //31
    #define ruiHDetTypeThl_FrSt Fld(3, 28, AC_MSKB3) //30:28
    #define rbMessBKBackEdgeDis_FrSt Fld(1, 27, AC_MSKB3) //27
    #define rbMessBKBackRuleDis_FrSt Fld(1, 26, AC_MSKB3) //26
    #define ruiHDetType_FrSt Fld(2, 24, AC_MSKB3) //25:24
    #define rbHSrcLPorCom_FrSt Fld(1, 23, AC_MSKB2) //23
    #define ruiDyVEdgeThlMess_FrSt Fld(11, 12, AC_MSKW21) //22:12
    #define rbHDetCorr_FrSt Fld(1, 11, AC_MSKB1) //11
    #define ruiDyVEdgeThlSM_FrSt Fld(11, 0, AC_MSKW10) //10:0
#define NR_2DNR15 (IO_PREPROC_BASE+0xdd4)
    #define rbSMBK3x3FilterV_FrSt Fld(1, 29, AC_MSKB3) //29
    #define rbSMBK3x3FilterH_FrSt Fld(1, 28, AC_MSKB3) //28
    #define rbDetailBK5x5FilterV_FrSt Fld(1, 27, AC_MSKB3) //27
    #define rbDetailBK5x5FilterH_FrSt Fld(1, 26, AC_MSKB3) //26
    #define rbMessBKBackEdgeDisH_FrSt Fld(1, 25, AC_MSKB3) //25
    #define rbMessBKBackRuleDisH_FrSt Fld(1, 24, AC_MSKB3) //24
    #define ruiHMaxRate_FrSt Fld(2, 22, AC_MSKB2) //23:22
    #define ruiHMinRate_FrSt Fld(2, 20, AC_MSKB2) //21:20
    #define ruiVMaxRate_FrSt Fld(2, 18, AC_MSKB2) //19:18
    #define ruiVMinRate_FrSt Fld(2, 16, AC_MSKB2) //17:16
    #define ruiDyVBKThlMess_FrSt Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyVBKThlSM_FrSt Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR16 (IO_PREPROC_BASE+0xdd8)
    #define ruiDyHEdgeThlMess_FrSt Fld(8, 24, AC_FULLB3) //31:24
    #define ruiDyHEdgeThlSM_FrSt Fld(8, 16, AC_FULLB2) //23:16
    #define ruiDyHBKThlMess_FrSt Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyHBKThlSM_FrSt Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR17 (IO_PREPROC_BASE+0xddc)
    #define ruiDyHA3132MaxThlMess_FrSt Fld(8, 24, AC_FULLB3) //31:24
    #define ruiDyHA3132MaxThlSM_FrSt Fld(8, 16, AC_FULLB2) //23:16
    #define ruiDyVA3132MaxThlMess_FrSt Fld(8, 8, AC_FULLB1) //15:8
    #define ruiDyVA3132MaxThlSM_FrSt Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR18 (IO_PREPROC_BASE+0xde0)
    #define ruiA3331ThlMess_BK Fld(8, 24, AC_FULLB3) //31:24
    #define ruiA3331ThlSM_BK Fld(8, 16, AC_FULLB2) //23:16
    #define ruiA3331ThlMess_Def Fld(8, 8, AC_FULLB1) //15:8
    #define ruiA3331ThlSM_Def Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR19 (IO_PREPROC_BASE+0xde4)
    #define ruiA3331ThlMess_St Fld(8, 24, AC_FULLB3) //31:24
    #define ruiA3331ThlSM_St Fld(8, 16, AC_FULLB2) //23:16
    #define ruiA3331ThlMess_Mo Fld(8, 8, AC_FULLB1) //15:8
    #define ruiA3331ThlSM_Mo Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR1A (IO_PREPROC_BASE+0xde8)
    #define rA30BigCheckEnVMess_Def Fld(1, 31, AC_MSKB3) //31
    #define rA30BigCheckEnVSM_Def Fld(1, 30, AC_MSKB3) //30
    #define rA30BigCheckEnHMess_Def Fld(1, 29, AC_MSKB3) //29
    #define rA30BigCheckEnHSM_Def Fld(1, 28, AC_MSKB3) //28
    #define rA30BigCheckEnVMess_BK Fld(1, 27, AC_MSKB3) //27
    #define rA30BigCheckEnVSM_BK Fld(1, 26, AC_MSKB3) //26
    #define rA30BigCheckEnHMess_BK Fld(1, 25, AC_MSKB3) //25
    #define rA30BigCheckEnHSM_BK Fld(1, 24, AC_MSKB3) //24
    #define rA30BigCheckEnVMess_Mo Fld(1, 23, AC_MSKB2) //23
    #define rA30BigCheckEnVSM_Mo Fld(1, 22, AC_MSKB2) //22
    #define rA30BigCheckEnHMess_Mo Fld(1, 21, AC_MSKB2) //21
    #define rA30BigCheckEnHSM_Mo Fld(1, 20, AC_MSKB2) //20
    #define rA30BigCheckEnVMess_St Fld(1, 19, AC_MSKB2) //19
    #define rA30BigCheckEnVSM_St Fld(1, 18, AC_MSKB2) //18
    #define rA30BigCheckEnHMess_St Fld(1, 17, AC_MSKB2) //17
    #define rA30BigCheckEnHSM_St Fld(1, 16, AC_MSKB2) //16
    #define ruiA3331ThlMess_FrSt Fld(8, 8, AC_FULLB1) //15:8
    #define ruiA3331ThlSM_FrSt Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR1B (IO_PREPROC_BASE+0xdec)
    #define rLocalSMDis_Mo Fld(1, 29, AC_MSKB3) //29
    #define rLPDetEn_Mo Fld(1, 28, AC_MSKB3) //28
    #define rSrcDetEn_Mo Fld(1, 27, AC_MSKB3) //27
    #define rSrcRuleEn_Mo Fld(1, 26, AC_MSKB3) //26
    #define rLPRuleEn_Mo Fld(1, 25, AC_MSKB3) //25
    #define rLSrcRuleEn_Mo Fld(1, 24, AC_MSKB3) //24
    #define rLLPRuleEn_Mo Fld(1, 23, AC_MSKB2) //23
    #define rLocalSMDis_BK Fld(1, 22, AC_MSKB2) //22
    #define rLPDetEn_BK Fld(1, 21, AC_MSKB2) //21
    #define rSrcDetEn_BK Fld(1, 20, AC_MSKB2) //20
    #define rSrcRuleEn_BK Fld(1, 19, AC_MSKB2) //19
    #define rLPRuleEn_BK Fld(1, 18, AC_MSKB2) //18
    #define rLSrcRuleEn_BK Fld(1, 17, AC_MSKB2) //17
    #define rLLPRuleEn_BK Fld(1, 16, AC_MSKB2) //16
    #define rLocalSMDis_Def Fld(1, 15, AC_MSKB1) //15
    #define rLPDetEn_Def Fld(1, 14, AC_MSKB1) //14
    #define rSrcDetEn_Def Fld(1, 13, AC_MSKB1) //13
    #define rSrcRuleEn_Def Fld(1, 12, AC_MSKB1) //12
    #define rLPRuleEn_Def Fld(1, 11, AC_MSKB1) //11
    #define rLSrcRuleEn_Def Fld(1, 10, AC_MSKB1) //10
    #define rLLPRuleEn_Def Fld(1, 9, AC_MSKB1) //9
    #define rBKBackAndMode_FrSt Fld(1, 8, AC_MSKB1) //8
    #define rBKBackAndMode_St Fld(1, 7, AC_MSKB0) //7
    #define rBKBackAndMode_Mo Fld(1, 6, AC_MSKB0) //6
    #define rBKBackAndMode_BK Fld(1, 5, AC_MSKB0) //5
    #define rBKBackAndMode_Def Fld(1, 4, AC_MSKB0) //4
    #define rA30BigCheckEnVMess_FrSt Fld(1, 3, AC_MSKB0) //3
    #define rA30BigCheckEnVSM_FrSt Fld(1, 2, AC_MSKB0) //2
    #define rA30BigCheckEnHMess_FrSt Fld(1, 1, AC_MSKB0) //1
    #define rA30BigCheckEnHSM_FrSt Fld(1, 0, AC_MSKB0) //0
#define NR_2DNR1C (IO_PREPROC_BASE+0xdf0)
    #define ruiVDetType_FrSt Fld(2, 22, AC_MSKB2) //23:22
    #define ruiVDetType_St Fld(2, 20, AC_MSKB2) //21:20
    #define ruiVDetType_Mo Fld(2, 18, AC_MSKB2) //19:18
    #define ruiVDetType_BK Fld(2, 16, AC_MSKB2) //17:16
    #define ruiVDetType_Def Fld(2, 14, AC_MSKB1) //15:14
    #define rLocalSMDis_FrSt Fld(1, 13, AC_MSKB1) //13
    #define rLPDetEn_FrSt Fld(1, 12, AC_MSKB1) //12
    #define rSrcDetEn_FrSt Fld(1, 11, AC_MSKB1) //11
    #define rSrcRuleEn_FrSt Fld(1, 10, AC_MSKB1) //10
    #define rLPRuleEn_FrSt Fld(1, 9, AC_MSKB1) //9
    #define rLSrcRuleEn_FrSt Fld(1, 8, AC_MSKB1) //8
    #define rLLPRuleEn_FrSt Fld(1, 7, AC_MSKB0) //7
    #define rLocalSMDis_St Fld(1, 6, AC_MSKB0) //6
    #define rLPDetEn_St Fld(1, 5, AC_MSKB0) //5
    #define rSrcDetEn_St Fld(1, 4, AC_MSKB0) //4
    #define rSrcRuleEn_St Fld(1, 3, AC_MSKB0) //3
    #define rLPRuleEn_St Fld(1, 2, AC_MSKB0) //2
    #define rLSrcRuleEn_St Fld(1, 1, AC_MSKB0) //1
    #define rLLPRuleEn_St Fld(1, 0, AC_MSKB0) //0
#define NR_2DNR1D (IO_PREPROC_BASE+0xdf4)
    #define ruiVDetTypeThl_FrSt Fld(3, 12, AC_MSKB1) //14:12
    #define ruiVDetTypeThl_St Fld(3, 9, AC_MSKB1) //11:9
    #define ruiVDetTypeThl_Mo Fld(3, 6, AC_MSKW10) //8:6
    #define ruiVDetTypeThl_BK Fld(3, 3, AC_MSKB0) //5:3
    #define ruiVDetTypeThl_Def Fld(3, 0, AC_MSKB0) //2:0
#define NR_2DNR1E (IO_PREPROC_BASE+0xdf8)
    #define rbBKFilterInkMode_0 Fld(1, 31, AC_MSKB3) //31
    #define rbBKFilterInkMode_1 Fld(1, 30, AC_MSKB3) //30
    #define rbBKFilterInkMode_2 Fld(1, 29, AC_MSKB3) //29
    #define rbBKFilterInkMode_3 Fld(1, 28, AC_MSKB3) //28
    #define rbBKFilterInkMode_4 Fld(1, 27, AC_MSKB3) //27
    #define rbBKFilterInkMode_5 Fld(1, 26, AC_MSKB3) //26
    #define rbBKFilterInkMode_6 Fld(1, 25, AC_MSKB3) //25
    #define rbBKFilterInkMode_7 Fld(1, 24, AC_MSKB3) //24
    #define rbBKFilterInkMode_8 Fld(1, 23, AC_MSKB2) //23
    #define rbBKFilterInkMode_9 Fld(1, 22, AC_MSKB2) //22
    #define rbBKFilterInkMode_10 Fld(1, 21, AC_MSKB2) //21
    #define rbBKFilterInkMode_11 Fld(1, 20, AC_MSKB2) //20
    #define rbBKFilterInkMode_12 Fld(1, 19, AC_MSKB2) //19
    #define rbBKFilterInkMode_13 Fld(1, 18, AC_MSKB2) //18
    #define rbBKFilterInkMode_14 Fld(1, 17, AC_MSKB2) //17
    #define rbBKFilterInkMode_15 Fld(1, 16, AC_MSKB2) //16
    #define rbBKFilterInkMode_16 Fld(1, 15, AC_MSKB1) //15
    #define rbBKFilterInkMode_17 Fld(1, 14, AC_MSKB1) //14
    #define rbBKFilterInkMode_18 Fld(1, 13, AC_MSKB1) //13
    #define rbBKFilterInkMode_19 Fld(1, 12, AC_MSKB1) //12
    #define rbBKFilterInkMode_20 Fld(1, 11, AC_MSKB1) //11
    #define rbBKFilterInkMode_21 Fld(1, 10, AC_MSKB1) //10
    #define rbBKFilterInkMode_22 Fld(1, 9, AC_MSKB1) //9
    #define rbBKFilterInkMode_23 Fld(1, 8, AC_MSKB1) //8
    #define rbBKFilterInkMode_24 Fld(1, 7, AC_MSKB0) //7
    #define rbBKFilterInkMode_25 Fld(1, 6, AC_MSKB0) //6
    #define Reserved_1 Fld(1, 5, AC_MSKB0) //5
    #define rbBoundaryRepeatDis Fld(1, 4, AC_MSKB0) //4
    #define rui_BKFilterInkLumaThl Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR1F (IO_PREPROC_BASE+0xe00)
    #define rbEdgeCor_MessBld Fld(1, 31, AC_MSKB3) //31
    #define rui_FixedPatNoSM Fld(1, 30, AC_MSKB3) //30
    #define rbFixFilter Fld(1, 29, AC_MSKB3) //29
    #define ruiFixedFilterType Fld(3, 26, AC_MSKB3) //28:26
    #define rbCursor_on Fld(1, 25, AC_MSKB3) //25
    #define rbOSDEn Fld(1, 24, AC_MSKB3) //24
    #define ruiprb_time_mode Fld(2, 22, AC_MSKB2) //23:22
    #define rink_sel Fld(1, 21, AC_MSKB2) //21
    #define rblock_flag_sel Fld(2, 19, AC_MSKB2) //20:19
    #define rfr_base_st_ink Fld(1, 18, AC_MSKB2) //18
    #define rcolor_ink Fld(1, 17, AC_MSKB2) //17
    #define rfr_ink Fld(1, 16, AC_MSKB2) //16
    #define rbk_meter_ink Fld(1, 15, AC_MSKB1) //15
    #define rdebug Fld(1, 14, AC_MSKB1) //14
    #define rleft_src Fld(1, 13, AC_MSKB1) //13
    #define rslice Fld(1, 12, AC_MSKB1) //12
    #define rcolor_en Fld(1, 11, AC_MSKB1) //11
    #define rdeblock_extend Fld(1, 10, AC_MSKB1) //10
    #define rbWeiBKEn Fld(1, 9, AC_MSKB1) //9
    #define rblock_meter Fld(1, 8, AC_MSKB1) //8
    #define rblock_pdp Fld(1, 7, AC_MSKB0) //7
    #define rfr_base_st_en Fld(1, 6, AC_MSKB0) //6
    #define rfr_st_en Fld(1, 5, AC_MSKB0) //5
    #define rfr_smo_en Fld(1, 4, AC_MSKB0) //4
    #define rbfour_dir_sm Fld(1, 3, AC_MSKB0) //3
    #define rbNewPxlBkEn Fld(1, 2, AC_MSKB0) //2
    #define rlow_angle Fld(1, 1, AC_MSKB0) //1
    #define r2D_enable Fld(1, 0, AC_MSKB0) //0
#define NR_2DNR20 (IO_PREPROC_BASE+0xe04)
    #define rbDetailExtendEn Fld(1, 31, AC_MSKB3) //31
    #define rVBK_Dis Fld(1, 30, AC_MSKB3) //30
    #define rHBK_Dis Fld(1, 29, AC_MSKB3) //29
    #define rbBKMeterDetOnly_FrSt Fld(1, 28, AC_MSKB3) //28
    #define rbBKMeterDetOnly_St Fld(1, 27, AC_MSKB3) //27
    #define rbBKMeterDetOnly_Mo Fld(1, 26, AC_MSKB3) //26
    #define rbBKMeterDetOnly_BK Fld(1, 25, AC_MSKB3) //25
    #define rbBKMeterDetOnly_Def Fld(1, 24, AC_MSKB3) //24
    #define ruiBlendOutLv Fld(4, 20, AC_MSKB2) //23:20
    #define ruiFrMoCntThl Fld(4, 16, AC_MSKB2) //19:16
    #define ruiFrStCntThl Fld(4, 12, AC_MSKB1) //15:12
    #define rb8BitMode Fld(1, 11, AC_MSKB1) //11
    #define rfr_sum_thl Fld(11, 0, AC_MSKW10) //10:0
#define NR_2DNR21 (IO_PREPROC_BASE+0xe08)
    #define rcur_type_sum_thl_def Fld(4, 28, AC_MSKB3) //31:28
    #define redge_correct2_en_def Fld(1, 27, AC_MSKB3) //27
    #define redge_correct_en_def Fld(1, 26, AC_MSKB3) //26
    #define ruismooth_thl_def Fld(10, 16, AC_MSKW32) //25:16
    #define rpat_sm_def Fld(8, 8, AC_FULLB1) //15:8
    #define rpat_th_def Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR22 (IO_PREPROC_BASE+0xe0c)
    #define rbpat_det_en_def Fld(1, 24, AC_MSKB3) //24
    #define rsmooth_filter_type_def Fld(2, 22, AC_MSKB2) //23:22
    #define rmess_filter_type_def Fld(2, 20, AC_MSKB2) //21:20
    #define rbcorner_smooth_def Fld(1, 19, AC_MSKB2) //19
    #define rtype_class_sum_thl_def Fld(3, 16, AC_MSKB2) //18:16
    #define rblend_en_def Fld(1, 15, AC_MSKB1) //15
    #define rsol_rate_thl_def Fld(3, 12, AC_MSKB1) //14:12
    #define redge_filter_type_def Fld(2, 10, AC_MSKB1) //11:10
    #define ruiedge_thl_def Fld(10, 0, AC_MSKW10) //9:0
#define NR_2DNR23 (IO_PREPROC_BASE+0xe10)
    #define rcur_type_sum_thl_St Fld(4, 28, AC_MSKB3) //31:28
    #define redge_correct2_en_St Fld(1, 27, AC_MSKB3) //27
    #define redge_correct_en_St Fld(1, 26, AC_MSKB3) //26
    #define ruismooth_thl_St Fld(10, 16, AC_MSKW32) //25:16
    #define rpat_sm_St Fld(8, 8, AC_FULLB1) //15:8
    #define rpat_th_St Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR24 (IO_PREPROC_BASE+0xe14)
    #define rbpat_det_en_St Fld(1, 24, AC_MSKB3) //24
    #define rsmooth_filter_type_St Fld(2, 22, AC_MSKB2) //23:22
    #define rmess_filter_type_St Fld(2, 20, AC_MSKB2) //21:20
    #define rbcorner_smooth_St Fld(1, 19, AC_MSKB2) //19
    #define rtype_class_sum_thl_St Fld(3, 16, AC_MSKB2) //18:16
    #define rblend_en_St Fld(1, 15, AC_MSKB1) //15
    #define rsol_rate_thl_St Fld(3, 12, AC_MSKB1) //14:12
    #define redge_filter_type_St Fld(2, 10, AC_MSKB1) //11:10
    #define ruiedge_thl_St Fld(10, 0, AC_MSKW10) //9:0
#define NR_2DNR25 (IO_PREPROC_BASE+0xe18)
    #define rcur_type_sum_thl_Mo Fld(4, 28, AC_MSKB3) //31:28
    #define redge_correct2_en_Mo Fld(1, 27, AC_MSKB3) //27
    #define redge_correct_en_Mo Fld(1, 26, AC_MSKB3) //26
    #define ruismooth_thl_Mo Fld(10, 16, AC_MSKW32) //25:16
    #define rpat_sm_Mo Fld(8, 8, AC_FULLB1) //15:8
    #define rpat_th_Mo Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR26 (IO_PREPROC_BASE+0xe1c)
    #define rbpat_det_en_Mo Fld(1, 24, AC_MSKB3) //24
    #define rsmooth_filter_type_Mo Fld(2, 22, AC_MSKB2) //23:22
    #define rmess_filter_type_Mo Fld(2, 20, AC_MSKB2) //21:20
    #define rbcorner_smooth_Mo Fld(1, 19, AC_MSKB2) //19
    #define rtype_class_sum_thl_Mo Fld(3, 16, AC_MSKB2) //18:16
    #define rblend_en_Mo Fld(1, 15, AC_MSKB1) //15
    #define rsol_rate_thl_Mo Fld(3, 12, AC_MSKB1) //14:12
    #define redge_filter_type_Mo Fld(2, 10, AC_MSKB1) //11:10
    #define ruiedge_thl_Mo Fld(10, 0, AC_MSKW10) //9:0
#define NR_2DNR27 (IO_PREPROC_BASE+0xe20)
    #define rcur_type_sum_thl_BK Fld(4, 28, AC_MSKB3) //31:28
    #define redge_correct2_en_BK Fld(1, 27, AC_MSKB3) //27
    #define redge_correct_en_BK Fld(1, 26, AC_MSKB3) //26
    #define ruismooth_thl_BK Fld(10, 16, AC_MSKW32) //25:16
    #define rpat_sm_BK Fld(8, 8, AC_FULLB1) //15:8
    #define rpat_th_BK Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR28 (IO_PREPROC_BASE+0xe24)
    #define rbpat_det_en_BK Fld(1, 24, AC_MSKB3) //24
    #define rsmooth_filter_type_BK Fld(2, 22, AC_MSKB2) //23:22
    #define rmess_filter_type_BK Fld(2, 20, AC_MSKB2) //21:20
    #define rbcorner_smooth_BK Fld(1, 19, AC_MSKB2) //19
    #define rtype_class_sum_thl_BK Fld(3, 16, AC_MSKB2) //18:16
    #define rblend_en_BK Fld(1, 15, AC_MSKB1) //15
    #define rsol_rate_thl_BK Fld(3, 12, AC_MSKB1) //14:12
    #define redge_filter_type_BK Fld(2, 10, AC_MSKB1) //11:10
    #define ruiedge_thl_BK Fld(10, 0, AC_MSKW10) //9:0
#define NR_2DNR29 (IO_PREPROC_BASE+0xe28)
    #define rcur_type_sum_thl_FrSt Fld(4, 28, AC_MSKB3) //31:28
    #define redge_correct2_en_FrSt Fld(1, 27, AC_MSKB3) //27
    #define redge_correct_en_FrSt Fld(1, 26, AC_MSKB3) //26
    #define ruismooth_thl_FrSt Fld(10, 16, AC_MSKW32) //25:16
    #define rpat_sm_FrSt Fld(8, 8, AC_FULLB1) //15:8
    #define rpat_th_FrSt Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR2A (IO_PREPROC_BASE+0xe2c)
    #define rbpat_det_en_FrSt Fld(1, 24, AC_MSKB3) //24
    #define rsmooth_filter_type_FrSt Fld(2, 22, AC_MSKB2) //23:22
    #define rmess_filter_type_FrSt Fld(2, 20, AC_MSKB2) //21:20
    #define rbcorner_smooth_FrSt Fld(1, 19, AC_MSKB2) //19
    #define rtype_class_sum_thl_FrSt Fld(3, 16, AC_MSKB2) //18:16
    #define rblend_en_FrSt Fld(1, 15, AC_MSKB1) //15
    #define rsol_rate_thl_FrSt Fld(3, 12, AC_MSKB1) //14:12
    #define redge_filter_type_FrSt Fld(2, 10, AC_MSKB1) //11:10
    #define ruiedge_thl_FrSt Fld(10, 0, AC_MSKW10) //9:0
#define NR_2DNR2B (IO_PREPROC_BASE+0xe30)
    #define rcur_type_sum_thl_Co1 Fld(4, 28, AC_MSKB3) //31:28
    #define redge_correct2_en_Co1 Fld(1, 27, AC_MSKB3) //27
    #define redge_correct_en_Co1 Fld(1, 26, AC_MSKB3) //26
    #define ruismooth_thl_Co1 Fld(10, 16, AC_MSKW32) //25:16
    #define rpat_sm_Co1 Fld(8, 8, AC_FULLB1) //15:8
    #define rpat_th_Co1 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR2C (IO_PREPROC_BASE+0xe34)
    #define rbpat_det_en_Co1 Fld(1, 24, AC_MSKB3) //24
    #define rsmooth_filter_type_Co1 Fld(2, 22, AC_MSKB2) //23:22
    #define rmess_filter_type_Co1 Fld(2, 20, AC_MSKB2) //21:20
    #define rbcorner_smooth_Co1 Fld(1, 19, AC_MSKB2) //19
    #define rtype_class_sum_thl_Co1 Fld(3, 16, AC_MSKB2) //18:16
    #define rblend_en_Co1 Fld(1, 15, AC_MSKB1) //15
    #define rsol_rate_thl_Co1 Fld(3, 12, AC_MSKB1) //14:12
    #define redge_filter_type_Co1 Fld(2, 10, AC_MSKB1) //11:10
    #define ruiedge_thl_Co1 Fld(10, 0, AC_MSKW10) //9:0
#define NR_2DNR2D (IO_PREPROC_BASE+0xe38)
    #define rcur_type_sum_thl_Co2 Fld(4, 28, AC_MSKB3) //31:28
    #define redge_correct2_en_Co2 Fld(1, 27, AC_MSKB3) //27
    #define redge_correct_en_Co2 Fld(1, 26, AC_MSKB3) //26
    #define ruismooth_thl_Co2 Fld(10, 16, AC_MSKW32) //25:16
    #define rpat_sm_Co2 Fld(8, 8, AC_FULLB1) //15:8
    #define rpat_th_Co2 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR2E (IO_PREPROC_BASE+0xe3c)
    #define rbpat_det_en_Co2 Fld(1, 24, AC_MSKB3) //24
    #define rsmooth_filter_type_Co2 Fld(2, 22, AC_MSKB2) //23:22
    #define rmess_filter_type_Co2 Fld(2, 20, AC_MSKB2) //21:20
    #define rbcorner_smooth_Co2 Fld(1, 19, AC_MSKB2) //19
    #define rtype_class_sum_thl_Co2 Fld(3, 16, AC_MSKB2) //18:16
    #define rblend_en_Co2 Fld(1, 15, AC_MSKB1) //15
    #define rsol_rate_thl_Co2 Fld(3, 12, AC_MSKB1) //14:12
    #define redge_filter_type_Co2 Fld(2, 10, AC_MSKB1) //11:10
    #define ruiedge_thl_Co2 Fld(10, 0, AC_MSKW10) //9:0
#define NR_2DNR2F (IO_PREPROC_BASE+0xe40)
    #define rcur_type_sum_thl_Co3 Fld(4, 28, AC_MSKB3) //31:28
    #define redge_correct2_en_Co3 Fld(1, 27, AC_MSKB3) //27
    #define redge_correct_en_Co3 Fld(1, 26, AC_MSKB3) //26
    #define ruismooth_thl_Co3 Fld(10, 16, AC_MSKW32) //25:16
    #define rpat_sm_Co3 Fld(8, 8, AC_FULLB1) //15:8
    #define rpat_th_Co3 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR30 (IO_PREPROC_BASE+0xe44)
    #define rbpat_det_en_Co3 Fld(1, 24, AC_MSKB3) //24
    #define rsmooth_filter_type_Co3 Fld(2, 22, AC_MSKB2) //23:22
    #define rmess_filter_type_Co3 Fld(2, 20, AC_MSKB2) //21:20
    #define rbcorner_smooth_Co3 Fld(1, 19, AC_MSKB2) //19
    #define rtype_class_sum_thl_Co3 Fld(3, 16, AC_MSKB2) //18:16
    #define rblend_en_Co3 Fld(1, 15, AC_MSKB1) //15
    #define rsol_rate_thl_Co3 Fld(3, 12, AC_MSKB1) //14:12
    #define redge_filter_type_Co3 Fld(2, 10, AC_MSKB1) //11:10
    #define ruiedge_thl_Co3 Fld(10, 0, AC_MSKW10) //9:0
#define NR_2DNR31 (IO_PREPROC_BASE+0xe48)
    #define rcur_type_sum_thl_Co4 Fld(4, 28, AC_MSKB3) //31:28
    #define redge_correct2_en_Co4 Fld(1, 27, AC_MSKB3) //27
    #define redge_correct_en_Co4 Fld(1, 26, AC_MSKB3) //26
    #define ruismooth_thl_Co4 Fld(10, 16, AC_MSKW32) //25:16
    #define rpat_sm_Co4 Fld(8, 8, AC_FULLB1) //15:8
    #define rpat_th_Co4 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR32 (IO_PREPROC_BASE+0xe4c)
    #define rbpat_det_en_Co4 Fld(1, 24, AC_MSKB3) //24
    #define rsmooth_filter_type_Co4 Fld(2, 22, AC_MSKB2) //23:22
    #define rmess_filter_type_Co4 Fld(2, 20, AC_MSKB2) //21:20
    #define rbcorner_smooth_Co4 Fld(1, 19, AC_MSKB2) //19
    #define rtype_class_sum_thl_Co4 Fld(3, 16, AC_MSKB2) //18:16
    #define rblend_en_Co4 Fld(1, 15, AC_MSKB1) //15
    #define rsol_rate_thl_Co4 Fld(3, 12, AC_MSKB1) //14:12
    #define redge_filter_type_Co4 Fld(2, 10, AC_MSKB1) //11:10
    #define ruiedge_thl_Co4 Fld(10, 0, AC_MSKW10) //9:0
#define NR_2DNR33 (IO_PREPROC_BASE+0xe50)
    #define rcur_type_sum_thl_Co5 Fld(4, 28, AC_MSKB3) //31:28
    #define redge_correct2_en_Co5 Fld(1, 27, AC_MSKB3) //27
    #define redge_correct_en_Co5 Fld(1, 26, AC_MSKB3) //26
    #define ruismooth_thl_Co5 Fld(10, 16, AC_MSKW32) //25:16
    #define rpat_sm_Co5 Fld(8, 8, AC_FULLB1) //15:8
    #define rpat_th_Co5 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR34 (IO_PREPROC_BASE+0xe54)
    #define rbpat_det_en_Co5 Fld(1, 24, AC_MSKB3) //24
    #define rsmooth_filter_type_Co5 Fld(2, 22, AC_MSKB2) //23:22
    #define rmess_filter_type_Co5 Fld(2, 20, AC_MSKB2) //21:20
    #define rbcorner_smooth_Co5 Fld(1, 19, AC_MSKB2) //19
    #define rtype_class_sum_thl_Co5 Fld(3, 16, AC_MSKB2) //18:16
    #define rblend_en_Co5 Fld(1, 15, AC_MSKB1) //15
    #define rsol_rate_thl_Co5 Fld(3, 12, AC_MSKB1) //14:12
    #define redge_filter_type_Co5 Fld(2, 10, AC_MSKB1) //11:10
    #define ruiedge_thl_Co5 Fld(10, 0, AC_MSKW10) //9:0
#define NR_2DNR35 (IO_PREPROC_BASE+0xe58)
    #define rcur_type_sum_thl_Co6 Fld(4, 28, AC_MSKB3) //31:28
    #define redge_correct2_en_Co6 Fld(1, 27, AC_MSKB3) //27
    #define redge_correct_en_Co6 Fld(1, 26, AC_MSKB3) //26
    #define ruismooth_thl_Co6 Fld(10, 16, AC_MSKW32) //25:16
    #define rpat_sm_Co6 Fld(8, 8, AC_FULLB1) //15:8
    #define rpat_th_Co6 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR36 (IO_PREPROC_BASE+0xe5c)
    #define rbpat_det_en_Co6 Fld(1, 24, AC_MSKB3) //24
    #define rsmooth_filter_type_Co6 Fld(2, 22, AC_MSKB2) //23:22
    #define rmess_filter_type_Co6 Fld(2, 20, AC_MSKB2) //21:20
    #define rbcorner_smooth_Co6 Fld(1, 19, AC_MSKB2) //19
    #define rtype_class_sum_thl_Co6 Fld(3, 16, AC_MSKB2) //18:16
    #define rblend_en_Co6 Fld(1, 15, AC_MSKB1) //15
    #define rsol_rate_thl_Co6 Fld(3, 12, AC_MSKB1) //14:12
    #define redge_filter_type_Co6 Fld(2, 10, AC_MSKB1) //11:10
    #define ruiedge_thl_Co6 Fld(10, 0, AC_MSKW10) //9:0
#define NR_2DNR37 (IO_PREPROC_BASE+0xe60)
    #define rMessSel_smooth_Co1Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Co1Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Co1Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Co1Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Co1Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Co1Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR38 (IO_PREPROC_BASE+0xe64)
    #define rMessSel_mess_Co1Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Co1Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Co1Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Co1Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Co1Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Co1Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR39 (IO_PREPROC_BASE+0xe68)
    #define rMessSel_smooth_Co1St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Co1St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Co1St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Co1St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Co1St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Co1St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR3A (IO_PREPROC_BASE+0xe6c)
    #define rMessSel_mess_Co1St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Co1St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Co1St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Co1St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Co1St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Co1St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR3B (IO_PREPROC_BASE+0xe70)
    #define rMessSel_smooth_Co2Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Co2Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Co2Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Co2Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Co2Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Co2Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR3C (IO_PREPROC_BASE+0xe74)
    #define rMessSel_mess_Co2Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Co2Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Co2Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Co2Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Co2Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Co2Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR3D (IO_PREPROC_BASE+0xe78)
    #define rMessSel_smooth_Co2St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Co2St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Co2St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Co2St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Co2St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Co2St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR3E (IO_PREPROC_BASE+0xe7c)
    #define rMessSel_mess_Co2St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Co2St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Co2St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Co2St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Co2St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Co2St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR3F (IO_PREPROC_BASE+0xe80)
    #define rMessSel_smooth_Co3Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Co3Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Co3Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Co3Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Co3Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Co3Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR40 (IO_PREPROC_BASE+0xe84)
    #define rMessSel_mess_Co3Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Co3Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Co3Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Co3Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Co3Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Co3Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR41 (IO_PREPROC_BASE+0xe88)
    #define rMessSel_smooth_Co3St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Co3St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Co3St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Co3St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Co3St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Co3St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR42 (IO_PREPROC_BASE+0xe8c)
    #define rMessSel_mess_Co3St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Co3St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Co3St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Co3St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Co3St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Co3St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR43 (IO_PREPROC_BASE+0xe90)
    #define rMessSel_smooth_Co4Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Co4Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Co4Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Co4Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Co4Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Co4Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR44 (IO_PREPROC_BASE+0xe94)
    #define rMessSel_mess_Co4Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Co4Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Co4Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Co4Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Co4Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Co4Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR45 (IO_PREPROC_BASE+0xe98)
    #define rMessSel_smooth_Co4St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Co4St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Co4St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Co4St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Co4St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Co4St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR46 (IO_PREPROC_BASE+0xe9c)
    #define rMessSel_mess_Co4St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Co4St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Co4St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Co4St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Co4St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Co4St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR47 (IO_PREPROC_BASE+0xea0)
    #define rMessSel_smooth_Co5Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Co5Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Co5Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Co5Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Co5Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Co5Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR48 (IO_PREPROC_BASE+0xea4)
    #define rMessSel_mess_Co5Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Co5Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Co5Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Co5Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Co5Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Co5Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR49 (IO_PREPROC_BASE+0xea8)
    #define rMessSel_smooth_Co5St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Co5St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Co5St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Co5St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Co5St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Co5St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR4A (IO_PREPROC_BASE+0xeac)
    #define rMessSel_mess_Co5St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Co5St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Co5St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Co5St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Co5St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Co5St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR4B (IO_PREPROC_BASE+0xeb0)
    #define rMessSel_smooth_Co6Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Co6Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Co6Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Co6Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Co6Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Co6Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR4C (IO_PREPROC_BASE+0xeb4)
    #define rMessSel_mess_Co6Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Co6Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Co6Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Co6Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Co6Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Co6Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR4D (IO_PREPROC_BASE+0xeb8)
    #define rMessSel_smooth_Co6St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Co6St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Co6St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Co6St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Co6St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Co6St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR4E (IO_PREPROC_BASE+0xebc)
    #define rMessSel_mess_Co6St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Co6St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Co6St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Co6St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Co6St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Co6St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR4F (IO_PREPROC_BASE+0xec0)
    #define rMessSel_smooth_FrSt Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_FrSt Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_FrSt Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_FrSt Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_FrSt Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_FrSt Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR50 (IO_PREPROC_BASE+0xec4)
    #define rMessSel_mess_FrSt Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_FrSt Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_FrSt Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_FrSt Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_FrSt Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_FrSt Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR51 (IO_PREPROC_BASE+0xec8)
    #define rMessSel_smooth_Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR52 (IO_PREPROC_BASE+0xecc)
    #define rMessSel_mess_Mo Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Mo Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Mo Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Mo Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Mo Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Mo Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR53 (IO_PREPROC_BASE+0xed0)
    #define rMessSel_smooth_St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR54 (IO_PREPROC_BASE+0xed4)
    #define rMessSel_mess_St Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_St Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_St Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_St Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_St Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_St Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR55 (IO_PREPROC_BASE+0xed8)
    #define rMessSel_smooth_BK Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_BK Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_BK Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_BK Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_BK Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_BK Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR56 (IO_PREPROC_BASE+0xedc)
    #define rMessSel_mess_BK Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_BK Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_BK Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_BK Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_BK Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_BK Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR57 (IO_PREPROC_BASE+0xee0)
    #define rMessSel_smooth_Def Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_smooth_Def Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_smooth_Def Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_edge_Def Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_edge_Def Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_edge_Def Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR58 (IO_PREPROC_BASE+0xee4)
    #define rMessSel_mess_Def Fld(2, 30, AC_MSKB3) //31:30
    #define rMessSft_mess_Def Fld(6, 24, AC_MSKB3) //29:24
    #define rMessThl_mess_Def Fld(6, 16, AC_MSKB2) //21:16
    #define rMessSel_mos_Def Fld(2, 14, AC_MSKB1) //15:14
    #define rMessSft_mos_Def Fld(6, 8, AC_MSKB1) //13:8
    #define rMessThl_mos_Def Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR92 (IO_PREPROC_BASE+0xef8)
    #define rfst_h_pratio_thr_high Fld(3, 29, AC_MSKB3) //31:29
    #define rfst_h_pratio_thr_low Fld(3, 26, AC_MSKB3) //28:26
    #define rfst_h_pdiff_thr_gain Fld(3, 23, AC_MSKW32) //25:23
    #define rfst_v_pdiff_thr_gain Fld(3, 20, AC_MSKB2) //22:20
    #define rfst_v_pratio_thr_high Fld(3, 16, AC_MSKB2) //18:16
    #define rfst_v_pratio_thr_low Fld(3, 12, AC_MSKB1) //14:12
    #define rfst_pdiff_thr_high2 Fld(4, 8, AC_MSKB1) //11:8
    #define rfst_pdiff_thr_high_v Fld(4, 4, AC_MSKB0) //7:4
    #define rfst_pdiff_thr_low_v Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR59 (IO_PREPROC_BASE+0xefc)
    #define rfst_pdiff_thr_high_h Fld(4, 28, AC_MSKB3) //31:28
    #define rfst_pdiff_thr_low_h Fld(4, 24, AC_MSKB3) //27:24
    #define rpmo_pdiff_thr_high_h Fld(4, 20, AC_MSKB2) //23:20
    #define rpmo_pdiff_thr_low_h Fld(4, 16, AC_MSKB2) //19:16
    #define rpst_pdiff_thr_high_h Fld(4, 12, AC_MSKB1) //15:12
    #define rpst_pdiff_thr_low_h Fld(4, 8, AC_MSKB1) //11:8
    #define rpdiff_thr_high_h Fld(4, 4, AC_MSKB0) //7:4
    #define rpdiff_thr_low_h Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR5A (IO_PREPROC_BASE+0xf00)
    #define rvpos_end Fld(8, 24, AC_FULLB3) //31:24
    #define rvpos_start Fld(8, 16, AC_FULLB2) //23:16
    #define rhpos_end Fld(8, 8, AC_FULLB1) //15:8
    #define rhpos_start Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR5B (IO_PREPROC_BASE+0xf04)
    #define rvsync_delay_sel Fld(2, 30, AC_MSKB3) //31:30
    #define rhbflag_delay_sel Fld(2, 28, AC_MSKB3) //29:28
    #define rvsync_inv Fld(1, 27, AC_MSKB3) //27
    #define rhsync_inv Fld(1, 26, AC_MSKB3) //26
    #define rvideo_width Fld(10, 16, AC_MSKW32) //25:16
    #define rh_line_sel Fld(1, 15, AC_MSKB1) //15
    #define rblockstep Fld(3, 12, AC_MSKB1) //14:12
    #define ractive_en Fld(1, 11, AC_MSKB1) //11
    #define rvideo_height Fld(11, 0, AC_MSKW10) //10:0
#define NR_2DNR5C (IO_PREPROC_BASE+0xf08)
    #define rblockcount_thr_high Fld(8, 24, AC_FULLB3) //31:24
    #define rblockcount_thr_low Fld(8, 16, AC_FULLB2) //23:16
    #define rlinecount_thr_high Fld(4, 12, AC_MSKB1) //15:12
    #define rlinecount_thr_low Fld(4, 8, AC_MSKB1) //11:8
    #define rpvalue_thr_high Fld(4, 4, AC_MSKB0) //7:4
    #define rpvalue_thr_low Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR5D (IO_PREPROC_BASE+0xf0c)
    #define rfst_v_ldiffsum_thr Fld(6, 24, AC_MSKB3) //29:24
    #define rpmo_en Fld(1, 23, AC_MSKB2) //23
    #define rfst_en Fld(1, 22, AC_MSKB2) //22
    #define rfst_h_ldiffsum_thr Fld(6, 16, AC_MSKB2) //21:16
    #define rpxl_mo_sel Fld(2, 14, AC_MSKB1) //15:14
    #define rv_ldiffsum_thr Fld(6, 8, AC_MSKB1) //13:8
    #define rinksel Fld(2, 6, AC_MSKB0) //7:6
    #define rh_ldiffsum_thr Fld(6, 0, AC_MSKB0) //5:0
#define NR_2DNR5E (IO_PREPROC_BASE+0xf10)
    #define rv_linenum_thr Fld(4, 28, AC_MSKB3) //31:28
    #define rv_lratio_thr3 Fld(4, 24, AC_MSKB3) //27:24
    #define rv_lratio_thr2 Fld(4, 20, AC_MSKB2) //23:20
    #define rv_lratio_thr1 Fld(4, 16, AC_MSKB2) //19:16
    #define rv_peakdet_sel Fld(2, 12, AC_MSKB1) //13:12
    #define rh_lratio_thr2 Fld(4, 8, AC_MSKB1) //11:8
    #define rh_lratio_thr1 Fld(4, 4, AC_MSKB0) //7:4
    #define rh_linenum_thr Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR5F (IO_PREPROC_BASE+0xf14)
    #define rfst_v_linenum_thr Fld(4, 28, AC_MSKB3) //31:28
    #define rfst_v_lratio_thr3 Fld(4, 24, AC_MSKB3) //27:24
    #define rfst_v_lratio_thr2 Fld(4, 20, AC_MSKB2) //23:20
    #define rfst_v_lratio_thr1 Fld(4, 16, AC_MSKB2) //19:16
    #define rfst_h_lratio_thr2 Fld(4, 8, AC_MSKB1) //11:8
    #define rfst_h_lratio_thr1 Fld(4, 4, AC_MSKB0) //7:4
    #define rfst_h_linenum_thr Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR60 (IO_PREPROC_BASE+0xf18)
    #define rh_pratio_thr_high Fld(3, 29, AC_MSKB3) //31:29
    #define rh_pratio_thr_low Fld(3, 26, AC_MSKB3) //28:26
    #define rh_pdiff_thr_gain Fld(3, 23, AC_MSKW32) //25:23
    #define rv_pdiff_thr_gain Fld(3, 20, AC_MSKB2) //22:20
    #define rv_pratio_thr_high Fld(3, 16, AC_MSKB2) //18:16
    #define rv_pratio_thr_low Fld(3, 12, AC_MSKB1) //14:12
    #define rpdiff_thr_high2 Fld(4, 8, AC_MSKB1) //11:8
    #define rpdiff_thr_high_v Fld(4, 4, AC_MSKB0) //7:4
    #define rpdiff_thr_low_v Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR61 (IO_PREPROC_BASE+0xf1c)
    #define rpmo_h_pratio_thr_high Fld(3, 29, AC_MSKB3) //31:29
    #define rpmo_h_pratio_thr_low Fld(3, 26, AC_MSKB3) //28:26
    #define rpmo_h_pdiff_thr_gain Fld(3, 23, AC_MSKW32) //25:23
    #define rpmo_v_pdiff_thr_gain Fld(3, 20, AC_MSKB2) //22:20
    #define rpmo_v_pratio_thr_high Fld(3, 16, AC_MSKB2) //18:16
    #define rpmo_v_pratio_thr_low Fld(3, 12, AC_MSKB1) //14:12
    #define rpmo_pdiff_thr_high2 Fld(4, 8, AC_MSKB1) //11:8
    #define rpmo_pdiff_thr_high_v Fld(4, 4, AC_MSKB0) //7:4
    #define rpmo_pdiff_thr_low_v Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR62 (IO_PREPROC_BASE+0xf20)
    #define rpst_h_pratio_thr_high Fld(3, 29, AC_MSKB3) //31:29
    #define rpst_h_pratio_thr_low Fld(3, 26, AC_MSKB3) //28:26
    #define rpst_h_pdiff_thr_gain Fld(3, 23, AC_MSKW32) //25:23
    #define rpst_v_pdiff_thr_gain Fld(3, 20, AC_MSKB2) //22:20
    #define rpst_v_pratio_thr_high Fld(3, 16, AC_MSKB2) //18:16
    #define rpst_v_pratio_thr_low Fld(3, 12, AC_MSKB1) //14:12
    #define rpst_pdiff_thr_high2 Fld(4, 8, AC_MSKB1) //11:8
    #define rpst_pdiff_thr_high_v Fld(4, 4, AC_MSKB0) //7:4
    #define rpst_pdiff_thr_low_v Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR63 (IO_PREPROC_BASE+0xf24)
    #if NR_ADAPTIVE_DEBA_ENABLED
    #define rbkmeter_reserved1 Fld(2, 30, AC_MSKB3) //31:30
    #define rbkmeter_hdelay Fld(2, 28, AC_MSKB3) //29:28
    #define rbkmeter_reserved2 Fld(15, 13, AC_MSKDW) //27:13    
    #else
    #define rbkmeter_reserved Fld(19, 13, AC_MSKDW) //31:13
    #endif
    #define rRD_hsync_value Fld(1, 12, AC_MSKB1) //12
    #define rbLineCntPulse Fld(1, 11, AC_MSKB1) //11
    #define rvdiffdiv_sel Fld(1, 10, AC_MSKB1) //10
    #define rvbk_jump_sel Fld(2, 8, AC_MSKB1) //9:8
    #define rnew_hdiff_en Fld(1, 7, AC_MSKB0) //7
    #define rstatus_sel Fld(1, 6, AC_MSKB0) //6
    #define rhv_pixelink_sel Fld(2, 4, AC_MSKB0) //5:4
    #define rv_bcolumn_sel Fld(1, 3, AC_MSKB0) //3
    #define rh_bcolumn_sel Fld(1, 2, AC_MSKB0) //2
    #define rc_vpeakdet_sel Fld(2, 0, AC_MSKB0) //1:0
#define NR_2DNR64 (IO_PREPROC_BASE+0xf28)
    #define c_smooth_detect_sel Fld(2, 30, AC_MSKB3) //31:30
    #define c_pre_filter_sel Fld(4, 24, AC_MSKB3) //27:24
    #define c_force_mess_diff_sft Fld(6, 16, AC_MSKB2) //21:16
    #define c_force_mess_diff_thd1 Fld(6, 8, AC_MSKB1) //13:8
    #define c_force_smooth_en Fld(1, 6, AC_MSKB0) //6
    #define c_force_mess_type_sel Fld(2, 4, AC_MSKB0) //5:4
    #define c_force_pre_filter Fld(1, 3, AC_MSKB0) //3
    #define c_direct_reject_type2_en Fld(1, 2, AC_MSKB0) //2
    #define c_direct_reject_type1_en Fld(1, 1, AC_MSKB0) //1
    #define c_small_area_reject_en Fld(1, 0, AC_MSKB0) //0
#define NR_2DNR65 (IO_PREPROC_BASE+0xf2c)
    #define c_mnr_sm_thr Fld(8, 24, AC_FULLB3) //31:24
    #define c_mnr_edge_thr Fld(8, 16, AC_FULLB2) //23:16
    #define c_sm_num_thr Fld(4, 8, AC_MSKB1) //11:8
    #define c_dynamic_edge_en Fld(1, 6, AC_MSKB0) //6
    #define c_edgedet_sel Fld(1, 5, AC_MSKB0) //5
    #define c_nearedge_sel3 Fld(1, 4, AC_MSKB0) //4
    #define c_nearedge_selwidth Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR66 (IO_PREPROC_BASE+0xf30)
    #define mnr_l_thr Fld(8, 8, AC_FULLB1) //15:8
    #define mnr_h_thr Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR67 (IO_PREPROC_BASE+0xf34)
    #define c_max_pre Fld(8, 24, AC_FULLB3) //31:24
    #define c_turn_2_pre Fld(8, 16, AC_FULLB2) //23:16
    #define c_turn_1_pre Fld(8, 8, AC_FULLB1) //15:8
    #define c_init_pre Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR68 (IO_PREPROC_BASE+0xf38)
    #define c_slope0_pre Fld(8, 16, AC_FULLB2) //23:16
    #define c_slope1_pre Fld(8, 8, AC_FULLB1) //15:8
#define NR_2DNR6A (IO_PREPROC_BASE+0xf40)
    #define c_still_src_small_thl Fld(2, 28, AC_MSKB3) //29:28
    #define c_still_mos3_en Fld(1, 26, AC_MSKB3) //26
    #define c_still_mos2_en Fld(1, 25, AC_MSKB3) //25
    #define c_still_mos1_en Fld(1, 24, AC_MSKB3) //24
    #define c_still_fil_small_en Fld(1, 23, AC_MSKB2) //23
    #define c_still_dir_det2_en Fld(1, 22, AC_MSKB2) //22
    #define c_still_src_small_en Fld(1, 21, AC_MSKB2) //21
    #define c_still_dir_det1_en Fld(1, 20, AC_MSKB2) //20
    #define c_still_smooth_cntthl Fld(4, 16, AC_MSKB2) //19:16
    #define c_still_smooth_diffthl Fld(8, 8, AC_FULLB1) //15:8
    #define c_still_src_smooth_diff Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR6B (IO_PREPROC_BASE+0xf44)
    #define c_motion_src_small_thl Fld(2, 28, AC_MSKB3) //29:28
    #define c_motion_mos3_en Fld(1, 26, AC_MSKB3) //26
    #define c_motion_mos2_en Fld(1, 25, AC_MSKB3) //25
    #define c_motion_mos1_en Fld(1, 24, AC_MSKB3) //24
    #define c_motion_fil_small_en Fld(1, 23, AC_MSKB2) //23
    #define c_motion_dir_det2_en Fld(1, 22, AC_MSKB2) //22
    #define c_motion_src_small_en Fld(1, 21, AC_MSKB2) //21
    #define c_motion_dir_det1_en Fld(1, 20, AC_MSKB2) //20
    #define c_motion_smooth_cntthl Fld(4, 16, AC_MSKB2) //19:16
    #define c_motion_smooth_diffthl Fld(8, 8, AC_FULLB1) //15:8
    #define c_motion_src_smooth_diff Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR6C (IO_PREPROC_BASE+0xf48)
    #define tone6_central_chk_dis Fld(1, 29, AC_MSKB3) //29
    #define tone5_central_chk_dis Fld(1, 28, AC_MSKB3) //28
    #define tone4_central_chk_dis Fld(1, 27, AC_MSKB3) //27
    #define tone3_central_chk_dis Fld(1, 26, AC_MSKB3) //26
    #define tone2_central_chk_dis Fld(1, 25, AC_MSKB3) //25
    #define tone1_central_chk_dis Fld(1, 24, AC_MSKB3) //24
    #define tone6_diff Fld(4, 20, AC_MSKB2) //23:20
    #define tone5_diff Fld(4, 16, AC_MSKB2) //19:16
    #define tone4_diff Fld(4, 12, AC_MSKB1) //15:12
    #define tone3_diff Fld(4, 8, AC_MSKB1) //11:8
    #define tone2_diff Fld(4, 4, AC_MSKB0) //7:4
    #define tone1_diff Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR6E (IO_PREPROC_BASE+0xf50)
    #define c_y_global_blend Fld(4, 28, AC_MSKB3) //31:28
    #define c_c_rec_sel Fld(2, 16, AC_MSKB2) //17:16
    #define c_c_filter_sel Fld(4, 12, AC_MSKB1) //15:12
    #define c_y_blend Fld(3, 8, AC_MSKB1) //10:8
    #define c_y_rec_sel Fld(2, 4, AC_MSKB0) //5:4
    #define c_y_filter_sel Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR6F (IO_PREPROC_BASE+0xf54)
    #define c_turn_2_rec Fld(8, 16, AC_FULLB2) //23:16
    #define c_turn_1_rec Fld(8, 8, AC_FULLB1) //15:8
    #define c_init_rec Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR70 (IO_PREPROC_BASE+0xf58)
    #define c_max_rec Fld(8, 24, AC_FULLB3) //31:24
    #define c_slope2_rec Fld(8, 16, AC_FULLB2) //23:16
    #define c_slope1_rec Fld(8, 8, AC_FULLB1) //15:8
    #define c_slope0_rec Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR71 (IO_PREPROC_BASE+0xf60)
    #define rmax_u_co1 Fld(8, 24, AC_FULLB3) //31:24
    #define rmin_u_co1 Fld(8, 16, AC_FULLB2) //23:16
    #define rmax_v_co1 Fld(8, 8, AC_FULLB1) //15:8
    #define rmin_v_co1 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR72 (IO_PREPROC_BASE+0xf64)
    #define rmax_u_co2 Fld(8, 24, AC_FULLB3) //31:24
    #define rmin_u_co2 Fld(8, 16, AC_FULLB2) //23:16
    #define rmax_v_co2 Fld(8, 8, AC_FULLB1) //15:8
    #define rmin_v_co2 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR73 (IO_PREPROC_BASE+0xf68)
    #define rmax_u_co3 Fld(8, 24, AC_FULLB3) //31:24
    #define rmin_u_co3 Fld(8, 16, AC_FULLB2) //23:16
    #define rmax_v_co3 Fld(8, 8, AC_FULLB1) //15:8
    #define rmin_v_co3 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR74 (IO_PREPROC_BASE+0xf6c)
    #define rmax_u_co4 Fld(8, 24, AC_FULLB3) //31:24
    #define rmin_u_co4 Fld(8, 16, AC_FULLB2) //23:16
    #define rmax_v_co4 Fld(8, 8, AC_FULLB1) //15:8
    #define rmin_v_co4 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR75 (IO_PREPROC_BASE+0xf70)
    #define rmax_u_co5 Fld(8, 24, AC_FULLB3) //31:24
    #define rmin_u_co5 Fld(8, 16, AC_FULLB2) //23:16
    #define rmax_v_co5 Fld(8, 8, AC_FULLB1) //15:8
    #define rmin_v_co5 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR76 (IO_PREPROC_BASE+0xf74)
    #define rmax_u_co6 Fld(8, 24, AC_FULLB3) //31:24
    #define rmin_u_co6 Fld(8, 16, AC_FULLB2) //23:16
    #define rmax_v_co6 Fld(8, 8, AC_FULLB1) //15:8
    #define rmin_v_co6 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR77 (IO_PREPROC_BASE+0xf78)
    #define rmax_y_co1 Fld(8, 24, AC_FULLB3) //31:24
    #define rmin_y_co1 Fld(8, 16, AC_FULLB2) //23:16
    #define rmax_y_co2 Fld(8, 8, AC_FULLB1) //15:8
    #define rmin_y_co2 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR78 (IO_PREPROC_BASE+0xf7c)
    #define rmax_y_co3 Fld(8, 24, AC_FULLB3) //31:24
    #define rmin_y_co3 Fld(8, 16, AC_FULLB2) //23:16
    #define rmax_y_co4 Fld(8, 8, AC_FULLB1) //15:8
    #define rmin_y_co4 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR79 (IO_PREPROC_BASE+0xf80)
    #define rmax_y_co5 Fld(8, 24, AC_FULLB3) //31:24
    #define rmin_y_co5 Fld(8, 16, AC_FULLB2) //23:16
    #define rmax_y_co6 Fld(8, 8, AC_FULLB1) //15:8
    #define rmin_y_co6 Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR93 (IO_PREPROC_BASE+0xf84)
    #define ysignsel Fld(1, 17, AC_MSKB2) //17
    #define disysign Fld(1, 16, AC_MSKB2) //16
    #define hcdifth Fld(8, 8, AC_FULLB1) //15:8
    #define vcdifth Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR7A (IO_PREPROC_BASE+0xf88)
    #define csel1 Fld(2, 30, AC_MSKB3) //31:30
    #define manugain Fld(4, 24, AC_MSKB3) //27:24
    #define enmanu Fld(1, 23, AC_MSKB2) //23
    #define lpfweith Fld(7, 16, AC_MSKB2) //22:16
    #define coloregth Fld(8, 8, AC_FULLB1) //15:8
    #define dcgain Fld(3, 4, AC_MSKB0) //6:4
    #define dygain Fld(3, 0, AC_MSKB0) //2:0
#define NR_2DNR7B (IO_PREPROC_BASE+0xf8c)
    #define front_enf Fld(1, 31, AC_MSKB3) //31
    #define front_adv Fld(1, 30, AC_MSKB3) //30
    #define back_enf Fld(1, 29, AC_MSKB3) //29
    #define back_adv Fld(1, 28, AC_MSKB3) //28
    #define bldsel Fld(1, 27, AC_MSKB3) //27
    #define yhpsel Fld(1, 26, AC_MSKB3) //26
    #define cdnr_filter_type Fld(2, 24, AC_MSKB3) //25:24
    #define motionth Fld(8, 8, AC_FULLB1) //15:8
    #define inkth_cdnr Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR7C (IO_PREPROC_BASE+0xf90)
    #define nuvdifbgsel Fld(1, 31, AC_MSKB3) //31
    #define y234sel Fld(1, 30, AC_MSKB3) //30
    #define nooversample Fld(1, 29, AC_MSKB3) //29
    #define vydotsel Fld(2, 8, AC_MSKB1) //9:8
    #define dot17sel Fld(2, 6, AC_MSKB0) //7:6
    #define enyversm Fld(1, 5, AC_MSKB0) //5
    #define enyhorsm Fld(1, 4, AC_MSKB0) //4
    #define areasel Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR7D (IO_PREPROC_BASE+0xf94)
    #define nuvdifbgth Fld(8, 24, AC_FULLB3) //31:24
    #define y01234difsumth Fld(8, 16, AC_FULLB2) //23:16
    #define umdcdsumth Fld(8, 8, AC_FULLB1) //15:8
    #define udydifsmth Fld(8, 0, AC_FULLB0) //7:0
#define NR_2DNR7E (IO_PREPROC_BASE+0xf98)
    #define ruiBldLv_BK_St Fld(4, 28, AC_MSKB3) //31:28
    #define ruiBldLv_SM_St Fld(4, 24, AC_MSKB3) //27:24
    #define ruiBldLv_Mess_St Fld(4, 20, AC_MSKB2) //23:20
    #define ruiBldLv_Edge_St Fld(4, 16, AC_MSKB2) //19:16
    #define ruiBldLv_BK_Def Fld(4, 12, AC_MSKB1) //15:12
    #define ruiBldLv_SM_Def Fld(4, 8, AC_MSKB1) //11:8
    #define ruiBldLv_Mess_Def Fld(4, 4, AC_MSKB0) //7:4
    #define ruiBldLv_Edge_Def Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR7F (IO_PREPROC_BASE+0xf9c)
    #define ruiBldLv_BK_BK Fld(4, 28, AC_MSKB3) //31:28
    #define ruiBldLv_SM_BK Fld(4, 24, AC_MSKB3) //27:24
    #define ruiBldLv_Mess_BK Fld(4, 20, AC_MSKB2) //23:20
    #define ruiBldLv_Edge_BK Fld(4, 16, AC_MSKB2) //19:16
    #define ruiBldLv_BK_Mo Fld(4, 12, AC_MSKB1) //15:12
    #define ruiBldLv_SM_Mo Fld(4, 8, AC_MSKB1) //11:8
    #define ruiBldLv_Mess_Mo Fld(4, 4, AC_MSKB0) //7:4
    #define ruiBldLv_Edge_Mo Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR80 (IO_PREPROC_BASE+0xfa0)
    #define ruiBldLv_BK_FrSt Fld(4, 28, AC_MSKB3) //31:28
    #define ruiBldLv_SM_FrSt Fld(4, 24, AC_MSKB3) //27:24
    #define ruiBldLv_Mess_FrSt Fld(4, 20, AC_MSKB2) //23:20
    #define ruiBldLv_Edge_FrSt Fld(4, 16, AC_MSKB2) //19:16
    #define ruiBldLv_BK_Co1 Fld(4, 12, AC_MSKB1) //15:12
    #define ruiBldLv_SM_Co1 Fld(4, 8, AC_MSKB1) //11:8
    #define ruiBldLv_Mess_Co1 Fld(4, 4, AC_MSKB0) //7:4
    #define ruiBldLv_Edge_Co1 Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR81 (IO_PREPROC_BASE+0xfa4)
    #define ruiBldLv_BK_Co2 Fld(4, 28, AC_MSKB3) //31:28
    #define ruiBldLv_SM_Co2 Fld(4, 24, AC_MSKB3) //27:24
    #define ruiBldLv_Mess_Co2 Fld(4, 20, AC_MSKB2) //23:20
    #define ruiBldLv_Edge_Co2 Fld(4, 16, AC_MSKB2) //19:16
    #define ruiBldLv_BK_Co3 Fld(4, 12, AC_MSKB1) //15:12
    #define ruiBldLv_SM_Co3 Fld(4, 8, AC_MSKB1) //11:8
    #define ruiBldLv_Mess_Co3 Fld(4, 4, AC_MSKB0) //7:4
    #define ruiBldLv_Edge_Co3 Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR82 (IO_PREPROC_BASE+0xfa8)
    #define ruiBldLv_BK_Co4 Fld(4, 28, AC_MSKB3) //31:28
    #define ruiBldLv_SM_Co4 Fld(4, 24, AC_MSKB3) //27:24
    #define ruiBldLv_Mess_Co4 Fld(4, 20, AC_MSKB2) //23:20
    #define ruiBldLv_Edge_Co4 Fld(4, 16, AC_MSKB2) //19:16
    #define ruiBldLv_BK_Co5 Fld(4, 12, AC_MSKB1) //15:12
    #define ruiBldLv_SM_Co5 Fld(4, 8, AC_MSKB1) //11:8
    #define ruiBldLv_Mess_Co5 Fld(4, 4, AC_MSKB0) //7:4
    #define ruiBldLv_Edge_Co5 Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR83 (IO_PREPROC_BASE+0xfac)
    #define ruiBldLv_mos_BK Fld(4, 28, AC_MSKB3) //31:28
    #define ruiBldLv_mos_Mo Fld(4, 24, AC_MSKB3) //27:24
    #define ruiBldLv_mos_St Fld(4, 20, AC_MSKB2) //23:20
    #define ruiBldLv_mos_Def Fld(4, 16, AC_MSKB2) //19:16
    #define ruiBldLv_BK_Co6 Fld(4, 12, AC_MSKB1) //15:12
    #define ruiBldLv_SM_Co6 Fld(4, 8, AC_MSKB1) //11:8
    #define ruiBldLv_Mess_Co6 Fld(4, 4, AC_MSKB0) //7:4
    #define ruiBldLv_Edge_Co6 Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR84 (IO_PREPROC_BASE+0xfb0)
    #define ruiBldLv_mos_Co6 Fld(4, 24, AC_MSKB3) //27:24
    #define ruiBldLv_mos_Co5 Fld(4, 20, AC_MSKB2) //23:20
    #define ruiBldLv_mos_Co4 Fld(4, 16, AC_MSKB2) //19:16
    #define ruiBldLv_mos_Co3 Fld(4, 12, AC_MSKB1) //15:12
    #define ruiBldLv_mos_Co2 Fld(4, 8, AC_MSKB1) //11:8
    #define ruiBldLv_mos_Co1 Fld(4, 4, AC_MSKB0) //7:4
    #define ruiBldLv_Near_FrSt Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR85 (IO_PREPROC_BASE+0xfb4)
    #define ruiBKWeiH_4 Fld(3, 27, AC_MSKB3) //29:27
    #define ruiBKWeiH_3 Fld(3, 24, AC_MSKB3) //26:24
    #define ruiBKWeiH_2 Fld(3, 21, AC_MSKB2) //23:21
    #define ruiBKWeiH_1 Fld(3, 18, AC_MSKB2) //20:18
    #define ruiBKWeiH_0 Fld(3, 15, AC_MSKW21) //17:15
    #define ruiBKWei_4 Fld(3, 12, AC_MSKB1) //14:12
    #define ruiBKWei_3 Fld(3, 9, AC_MSKB1) //11:9
    #define ruiBKWei_2 Fld(3, 6, AC_MSKW10) //8:6
    #define ruiBKWei_1 Fld(3, 3, AC_MSKB0) //5:3
    #define ruiBKWei_0 Fld(3, 0, AC_MSKB0) //2:0
#define NR_2DNR86 (IO_PREPROC_BASE+0xfb8)
    #define ruiLinearType_BK_St Fld(4, 28, AC_MSKB3) //31:28
    #define ruiLinearType_SM_St Fld(4, 24, AC_MSKB3) //27:24
    #define ruiLinearType_Mess_St Fld(4, 20, AC_MSKB2) //23:20
    #define ruiLinearType_Edge_St Fld(4, 16, AC_MSKB2) //19:16
    #define ruiLinearType_BK_Def Fld(4, 12, AC_MSKB1) //15:12
    #define ruiLinearType_SM_Def Fld(4, 8, AC_MSKB1) //11:8
    #define ruiLinearType_Mess_Def Fld(4, 4, AC_MSKB0) //7:4
    #define ruiLinearType_Edge_Def Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR87 (IO_PREPROC_BASE+0xfbc)
    #define ruiLinearType_BK_BK Fld(4, 28, AC_MSKB3) //31:28
    #define ruiLinearType_SM_BK Fld(4, 24, AC_MSKB3) //27:24
    #define ruiLinearType_Mess_BK Fld(4, 20, AC_MSKB2) //23:20
    #define ruiLinearType_Edge_BK Fld(4, 16, AC_MSKB2) //19:16
    #define ruiLinearType_BK_Mo Fld(4, 12, AC_MSKB1) //15:12
    #define ruiLinearType_SM_Mo Fld(4, 8, AC_MSKB1) //11:8
    #define ruiLinearType_Mess_Mo Fld(4, 4, AC_MSKB0) //7:4
    #define ruiLinearType_Edge_Mo Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR88 (IO_PREPROC_BASE+0xfc0)
    #define ruiLinearType_BK_FrSt Fld(4, 28, AC_MSKB3) //31:28
    #define ruiLinearType_SM_FrSt Fld(4, 24, AC_MSKB3) //27:24
    #define ruiLinearType_Mess_FrSt Fld(4, 20, AC_MSKB2) //23:20
    #define ruiLinearType_Edge_FrSt Fld(4, 16, AC_MSKB2) //19:16
    #define ruiLinearType_BK_Co1 Fld(4, 12, AC_MSKB1) //15:12
    #define ruiLinearType_SM_Co1 Fld(4, 8, AC_MSKB1) //11:8
    #define ruiLinearType_Mess_Co1 Fld(4, 4, AC_MSKB0) //7:4
    #define ruiLinearType_Edge_Co1 Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR89 (IO_PREPROC_BASE+0xfc4)
    #define ruiLinearType_BK_Co2 Fld(4, 28, AC_MSKB3) //31:28
    #define ruiLinearType_SM_Co2 Fld(4, 24, AC_MSKB3) //27:24
    #define ruiLinearType_Mess_Co2 Fld(4, 20, AC_MSKB2) //23:20
    #define ruiLinearType_Edge_Co2 Fld(4, 16, AC_MSKB2) //19:16
    #define ruiLinearType_BK_Co3 Fld(4, 12, AC_MSKB1) //15:12
    #define ruiLinearType_SM_Co3 Fld(4, 8, AC_MSKB1) //11:8
    #define ruiLinearType_Mess_Co3 Fld(4, 4, AC_MSKB0) //7:4
    #define ruiLinearType_Edge_Co3 Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR8A (IO_PREPROC_BASE+0xfc8)
    #define ruiLinearType_BK_Co4 Fld(4, 28, AC_MSKB3) //31:28
    #define ruiLinearType_SM_Co4 Fld(4, 24, AC_MSKB3) //27:24
    #define ruiLinearType_Mess_Co4 Fld(4, 20, AC_MSKB2) //23:20
    #define ruiLinearType_Edge_Co4 Fld(4, 16, AC_MSKB2) //19:16
    #define ruiLinearType_BK_Co5 Fld(4, 12, AC_MSKB1) //15:12
    #define ruiLinearType_SM_Co5 Fld(4, 8, AC_MSKB1) //11:8
    #define ruiLinearType_Mess_Co5 Fld(4, 4, AC_MSKB0) //7:4
    #define ruiLinearType_Edge_Co5 Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR8B (IO_PREPROC_BASE+0xfcc)
    #define ruiLinearType_mos_BK Fld(4, 28, AC_MSKB3) //31:28
    #define ruiLinearType_mos_Mo Fld(4, 24, AC_MSKB3) //27:24
    #define ruiLinearType_mos_St Fld(4, 20, AC_MSKB2) //23:20
    #define ruiLinearType_mos_Def Fld(4, 16, AC_MSKB2) //19:16
    #define ruiLinearType_BK_Co6 Fld(4, 12, AC_MSKB1) //15:12
    #define ruiLinearType_SM_Co6 Fld(4, 8, AC_MSKB1) //11:8
    #define ruiLinearType_Mess_Co6 Fld(4, 4, AC_MSKB0) //7:4
    #define ruiLinearType_Edge_Co6 Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR8C (IO_PREPROC_BASE+0xfd0)
    #define ruiLinearType_mos_Co6 Fld(4, 24, AC_MSKB3) //27:24
    #define ruiLinearType_mos_Co5 Fld(4, 20, AC_MSKB2) //23:20
    #define ruiLinearType_mos_Co4 Fld(4, 16, AC_MSKB2) //19:16
    #define ruiLinearType_mos_Co3 Fld(4, 12, AC_MSKB1) //15:12
    #define ruiLinearType_mos_Co2 Fld(4, 8, AC_MSKB1) //11:8
    #define ruiLinearType_mos_Co1 Fld(4, 4, AC_MSKB0) //7:4
    #define ruiLinearType_mos_FrSt Fld(4, 0, AC_MSKB0) //3:0
#define NR_2DNR8D (IO_PREPROC_BASE+0xfd4)
    #define rui_LinearForce_BK_St Fld(1, 31, AC_MSKB3) //31
    #define rui_LinearForce_SM_St Fld(1, 30, AC_MSKB3) //30
    #define rui_LinearForce_Mess_St Fld(1, 29, AC_MSKB3) //29
    #define rui_LinearForce_Edge_St Fld(1, 28, AC_MSKB3) //28
    #define rui_LinearForce_BK_Def Fld(1, 27, AC_MSKB3) //27
    #define rui_LinearForce_SM_Def Fld(1, 26, AC_MSKB3) //26
    #define rui_LinearForce_Mess_Def Fld(1, 25, AC_MSKB3) //25
    #define rui_LinearForce_Edge_Def Fld(1, 24, AC_MSKB3) //24
    #define rui_LinearForce_BK_BK Fld(1, 23, AC_MSKB2) //23
    #define rui_LinearForce_SM_BK Fld(1, 22, AC_MSKB2) //22
    #define rui_LinearForce_Mess_BK Fld(1, 21, AC_MSKB2) //21
    #define rui_LinearForce_Edge_BK Fld(1, 20, AC_MSKB2) //20
    #define rui_LinearForce_BK_Mo Fld(1, 19, AC_MSKB2) //19
    #define rui_LinearForce_SM_Mo Fld(1, 18, AC_MSKB2) //18
    #define rui_LinearForce_Mess_Mo Fld(1, 17, AC_MSKB2) //17
    #define rui_LinearForce_Edge_Mo Fld(1, 16, AC_MSKB2) //16
    #define rui_LinearForce_BK_FrSt Fld(1, 15, AC_MSKB1) //15
    #define rui_LinearForce_SM_FrSt Fld(1, 14, AC_MSKB1) //14
    #define rui_LinearForce_Mess_FrSt Fld(1, 13, AC_MSKB1) //13
    #define rui_LinearForce_Edge_FrSt Fld(1, 12, AC_MSKB1) //12
    #define rui_LinearForce_BK_Co1 Fld(1, 11, AC_MSKB1) //11
    #define rui_LinearForce_SM_Co1 Fld(1, 10, AC_MSKB1) //10
    #define rui_LinearForce_Mess_Co1 Fld(1, 9, AC_MSKB1) //9
    #define rui_LinearForce_Edge_Co1 Fld(1, 8, AC_MSKB1) //8
    #define rui_LinearForce_BK_Co2 Fld(1, 7, AC_MSKB0) //7
    #define rui_LinearForce_SM_Co2 Fld(1, 6, AC_MSKB0) //6
    #define rui_LinearForce_Mess_Co2 Fld(1, 5, AC_MSKB0) //5
    #define rui_LinearForce_Edge_Co2 Fld(1, 4, AC_MSKB0) //4
    #define rui_LinearForce_BK_Co3 Fld(1, 3, AC_MSKB0) //3
    #define rui_LinearForce_SM_Co3 Fld(1, 2, AC_MSKB0) //2
    #define rui_LinearForce_Mess_Co3 Fld(1, 1, AC_MSKB0) //1
    #define rui_LinearForce_Edge_Co3 Fld(1, 0, AC_MSKB0) //0
#define NR_2DNR8E (IO_PREPROC_BASE+0xfd8)
    #define rbLongFilterTpEn Fld(1, 25, AC_MSKB3) //25
    #define rbOverSpBgExd Fld(1, 24, AC_MSKB3) //24
    #define rbOverSampleEn Fld(1, 23, AC_MSKB2) //23
    #define rui_LinearForce_BK_Co4 Fld(1, 22, AC_MSKB2) //22
    #define rui_LinearForce_SM_Co4 Fld(1, 21, AC_MSKB2) //21
    #define rui_LinearForce_Mess_Co4 Fld(1, 20, AC_MSKB2) //20
    #define rui_LinearForce_Edge_Co4 Fld(1, 19, AC_MSKB2) //19
    #define rui_LinearForce_BK_Co5 Fld(1, 18, AC_MSKB2) //18
    #define rui_LinearForce_SM_Co5 Fld(1, 17, AC_MSKB2) //17
    #define rui_LinearForce_Mess_Co5 Fld(1, 16, AC_MSKB2) //16
    #define rui_LinearForce_Edge_Co5 Fld(1, 15, AC_MSKB1) //15
    #define rui_LinearForce_BK_Co6 Fld(1, 14, AC_MSKB1) //14
    #define rui_LinearForce_SM_Co6 Fld(1, 13, AC_MSKB1) //13
    #define rui_LinearForce_Mess_Co6 Fld(1, 12, AC_MSKB1) //12
    #define rui_LinearForce_Edge_Co6 Fld(1, 11, AC_MSKB1) //11
    #define rui_LinearForce_mos_BK Fld(1, 10, AC_MSKB1) //10
    #define rui_LinearForce_mos_Mo Fld(1, 9, AC_MSKB1) //9
    #define rui_LinearForce_mos_St Fld(1, 8, AC_MSKB1) //8
    #define rui_LinearForce_mos_Def Fld(1, 7, AC_MSKB0) //7
    #define rui_LinearForce_mos_Co6 Fld(1, 6, AC_MSKB0) //6
    #define rui_LinearForce_mos_Co5 Fld(1, 5, AC_MSKB0) //5
    #define rui_LinearForce_mos_Co4 Fld(1, 4, AC_MSKB0) //4
    #define rui_LinearForce_mos_Co3 Fld(1, 3, AC_MSKB0) //3
    #define rui_LinearForce_mos_Co2 Fld(1, 2, AC_MSKB0) //2
    #define rui_LinearForce_mos_Co1 Fld(1, 1, AC_MSKB0) //1
    #define rui_LinearForce_mos_FrSt Fld(1, 0, AC_MSKB0) //0
#define NR_2DNR8F (IO_PREPROC_BASE+0xff4)
    #define Reserved_31 Fld(1, 31, AC_MSKB3) //31
    #define rVLineCnt Fld(7, 24, AC_MSKB3) //30:24
    #define Reserved_23 Fld(1, 23, AC_MSKB2) //23
    #define rHLineCnt Fld(7, 16, AC_MSKB2) //22:16
    #define Reserved_15_8 Fld(8, 8, AC_FULLB1) //15:8
    #define rVBlockDecision Fld(1, 7, AC_MSKB0) //7
    #define rVBlockLevel Fld(3, 4, AC_MSKB0) //6:4
    #define rHBlockDecision Fld(1, 3, AC_MSKB0) //3
    #define rHBlockLevel Fld(3, 0, AC_MSKB0) //2:0
#define NR_2DNR90 (IO_PREPROC_BASE+0xff8)
    #define Reserved_31_19 Fld(13, 19, AC_MSKW32) //31:19
    #define rblock_ft Fld(1, 18, AC_MSKB2) //18
    #define Reserved_17 Fld(1, 17, AC_MSKB2) //17
    #define rb_fr_base_st Fld(1, 16, AC_MSKB2) //16
    #define rFrMoCnt Fld(16, 0, AC_FULLW10) //15:0
#define NR_2DNR91 (IO_PREPROC_BASE+0xffc)
    #define BlockSize_Current Fld(2, 30, AC_MSKB3) //31:30
    #define BlockSize1_Current Fld(6, 24, AC_MSKB3) //29:24
    #define Reserved_23_22 Fld(2, 22, AC_MSKB2) //23:22
    #define BlockSize2_Current Fld(6, 16, AC_MSKB2) //21:16
    #define Reserved_15_14 Fld(2, 14, AC_MSKB1) //15:14
    #define BlockSize3_Current Fld(6, 8, AC_MSKB1) //13:8
    #define Reserved_7_6 Fld(2, 6, AC_MSKB0) //7:6
    #define BlockSize4_Current Fld(6, 0, AC_MSKB0) //5:0
#define NR_3DNR00 (IO_PREPROC_BASE+0xc00)
    #define TNRenable Fld(1, 31, AC_MSKB3) //31
    #define c_src_420 Fld(1, 29, AC_MSKB3) //29
    #define c_nr_src_sel Fld(1, 28, AC_MSKB3) //28
    #define c_sw_init Fld(1, 27, AC_MSKB3) //27
    #define c_v_bound_protect Fld(1, 26, AC_MSKB3) //26
    #define enink_path Fld(1, 24, AC_MSKB3) //24
    #define nr_read_enable Fld(1, 10, AC_MSKB1) //10
    #define nr_c_delay_sel Fld(1, 9, AC_MSKB1) //9
    #define nr_progressive Fld(1, 8, AC_MSKB1) //8
    #define nr_yuv_mode Fld(2, 6, AC_MSKB0) //7:6
    #define nr_display_mode Fld(2, 4, AC_MSKB0) //5:4
    #define nr_main_pip_sel Fld(1, 3, AC_MSKB0) //3
    #define nr_bit_sel_rv Fld(1, 2, AC_MSKB0) //2
    #define nr_bit_sel_ru Fld(1, 1, AC_MSKB0) //1
    #define nr_bit_sel_ry Fld(1, 0, AC_MSKB0) //0
#define NR_3DNR01 (IO_PREPROC_BASE+0xc04)
    #define clavgsel Fld(2, 12, AC_MSKB1) //13:12
    #define totlcnt Fld(12, 0, AC_MSKW10) //11:0
#define NR_3DNR02 (IO_PREPROC_BASE+0xc08)
    #define enforce_tbl Fld(1, 31, AC_MSKB3) //31
    #define enfbch Fld(1, 30, AC_MSKB3) //30
    #define enegcond Fld(2, 28, AC_MSKB3) //29:28
    #define enchdmo Fld(1, 27, AC_MSKB3) //27
    #define motpsel Fld(1, 26, AC_MSKB3) //26
    #define ccs_dtbsel Fld(1, 25, AC_MSKB3) //25
    #define encmousec Fld(1, 24, AC_MSKB3) //24
    #define enymousey Fld(1, 23, AC_MSKB2) //23
    #define cmousey Fld(1, 22, AC_MSKB2) //22
    #define enwcufix Fld(1, 21, AC_MSKB2) //21
    #define enhdmo Fld(1, 18, AC_MSKB2) //18
    #define manwcuH Fld(1, 17, AC_MSKB2) //17
    #define enfwch Fld(1, 16, AC_MSKB2) //16
    #define b8mode Fld(1, 15, AC_MSKB1) //15
    #define tavgmode Fld(1, 13, AC_MSKB1) //13
    #define manwcu Fld(1, 12, AC_MSKB1) //12
    #define disghF16 Fld(1, 11, AC_MSKB1) //11
    #define freeze Fld(1, 10, AC_MSKB1) //10
    #define rd0sel Fld(1, 6, AC_MSKB0) //6
#define NR_3DNR03 (IO_PREPROC_BASE+0xc0c)
    #define olddmr Fld(1, 30, AC_MSKB3) //30
    #define enhsmpc Fld(1, 29, AC_MSKB3) //29
    #define en_pv7fti Fld(1, 28, AC_MSKB3) //28
    #define usePWst Fld(1, 27, AC_MSKB3) //27
    #define PWfst3D Fld(1, 26, AC_MSKB3) //26
    #define dblpsel Fld(2, 24, AC_MSKB3) //25:24
    #define ensmogt Fld(1, 23, AC_MSKB2) //23
    #define rdsel Fld(1, 22, AC_MSKB2) //22
    #define bpyc2d Fld(1, 20, AC_MSKB2) //20
    #define bpysb2d Fld(1, 19, AC_MSKB2) //19
    #define bpcsb2d Fld(1, 18, AC_MSKB2) //18
    #define useycavg Fld(1, 17, AC_MSKB2) //17
    #define enmangnr Fld(1, 11, AC_MSKB1) //11
    #define cuseytbl Fld(1, 10, AC_MSKB1) //10
    #define endbf Fld(1, 9, AC_MSKB1) //9
    #define entbext Fld(1, 8, AC_MSKB1) //8
    #define en_hbtnr Fld(1, 7, AC_MSKB0) //7
    #define enrdprot Fld(1, 6, AC_MSKB0) //6
    #define fdlysel Fld(2, 4, AC_MSKB0) //5:4
    #define rskin_cond_bigmo Fld(1, 2, AC_MSKB0) //2
    #define rskin_cond_smlmo Fld(1, 1, AC_MSKB0) //1
    #define rskin_cond_still Fld(1, 0, AC_MSKB0) //0
#define NR_3DNR04 (IO_PREPROC_BASE+0xc10)
    #define inksel Fld(5, 12, AC_MSKW21) //16:12
    #define rlymoth Fld(8, 0, AC_FULLB0) //7:0
#define NR_3DNR05 (IO_PREPROC_BASE+0xc14)
    #define en_32lv Fld(1, 20, AC_MSKB2) //20
    #define en_16lv Fld(1, 19, AC_MSKB2) //19
    #define en_manlsb Fld(1, 18, AC_MSKB2) //18
    #define enp4mo Fld(1, 17, AC_MSKB2) //17
    #define reg_manlsb Fld(2, 14, AC_MSKB1) //15:14
    #define molpsel Fld(2, 12, AC_MSKB1) //13:12
    #define eniddin8 Fld(1, 11, AC_MSKB1) //11
    #define enid Fld(1, 10, AC_MSKB1) //10
    #define disfcnd Fld(1, 8, AC_MSKB1) //8
    #define endiskin Fld(1, 7, AC_MSKB0) //7
    #define enbrnc Fld(1, 6, AC_MSKB0) //6
    #define vga3d Fld(1, 4, AC_MSKB0) //4
    #define Hftblsel Fld(1, 3, AC_MSKB0) //3
    #define vsmsel Fld(1, 2, AC_MSKB0) //2
    #define enskin Fld(1, 1, AC_MSKB0) //1
    #define inv_uvseq Fld(1, 0, AC_MSKB0) //0
#define NR_3DNR07 (IO_PREPROC_BASE+0xc1c)
    #define schg_fdsel Fld(2, 20, AC_MSKB2) //21:20
    #define ymoth Fld(10, 8, AC_MSKW21) //17:8
    #define enwhink Fld(1, 4, AC_MSKB0) //4
    #define enctiir Fld(1, 3, AC_MSKB0) //3
    #define enidf Fld(1, 2, AC_MSKB0) //2
    #define enprfir Fld(1, 1, AC_MSKB0) //1
    #define enytiir Fld(1, 0, AC_MSKB0) //0
#define NR_3DNR08 (IO_PREPROC_BASE+0xc20)
    #define fbigmo_cnt Fld(20, 0, AC_MSKDW) //19:0
#define NR_3DNR09 (IO_PREPROC_BASE+0xc2c)
    #define ciir_tbl Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR0C (IO_PREPROC_BASE+0xc30)
    #define b2tbth Fld(7, 16, AC_MSKB2) //22:16
    #define ccs_moth Fld(7, 0, AC_MSKB0) //6:0
#define NR_3DNR0D (IO_PREPROC_BASE+0xc34)
    #define lnbufmode Fld(2, 30, AC_MSKB3) //31:30
    #define lnbuf_manual_length Fld(10, 20, AC_MSKW32) //29:20
    #define fsmallmo_cnt Fld(20, 0, AC_MSKDW) //19:0
#define NR_3DNR10 (IO_PREPROC_BASE+0xc40)
    #define p4motbl Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR11 (IO_PREPROC_BASE+0xc44)
    #define nop4motbl Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR12 (IO_PREPROC_BASE+0xc48)
    #define skintbl Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR13 (IO_PREPROC_BASE+0xc4c)
    #define smogtbl Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR14 (IO_PREPROC_BASE+0xc50)
    #define skintbth Fld(7, 16, AC_MSKB2) //22:16
    #define nop4motbth Fld(7, 8, AC_MSKB1) //14:8
    #define p4motbth Fld(7, 0, AC_MSKB0) //6:0
#define NR_3DNR15 (IO_PREPROC_BASE+0xc54)
    #define ncth Fld(8, 20, AC_MSKW32) //27:20
    #define fstill_cnt Fld(20, 0, AC_MSKDW) //19:0
#define NR_3DNR16 (IO_PREPROC_BASE+0xc58)
    #define motionth Fld(8, 8, AC_FULLB1) //15:8
    #define yhordifsmth Fld(8, 0, AC_FULLB0) //7:0
#define NR_3DNR17 (IO_PREPROC_BASE+0xc5c)
    #define hdmoth Fld(10, 0, AC_MSKW10) //9:0
#define NR_3DNR19 (IO_PREPROC_BASE+0xc64)
    #define c_y_blend Fld(3, 8, AC_MSKB1) //10:8
    #define c_y_rec_sel Fld(2, 4, AC_MSKB0) //5:4
    #define c_y_filter_sel Fld(4, 0, AC_MSKB0) //3:0
#define NR_3DNR1A (IO_PREPROC_BASE+0xc68)
    #define fstilltb Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR1B (IO_PREPROC_BASE+0xc6c)
    #define fsmlmotb Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR1C (IO_PREPROC_BASE+0xc70)
    #define defaultb Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR1D (IO_PREPROC_BASE+0xc74)
    #define fbigmotb Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR1E (IO_PREPROC_BASE+0xc78)
    #define bgmotbth Fld(7, 24, AC_MSKB3) //30:24
    #define deftbth Fld(7, 16, AC_MSKB2) //22:16
    #define fsmlmotbth Fld(7, 8, AC_MSKB1) //14:8
    #define fstilltbth Fld(7, 0, AC_MSKB0) //6:0
#define NR_3DNR20 (IO_PREPROC_BASE+0xc80)
    #define VBMOCNT Fld(20, 0, AC_MSKDW) //19:0
#define NR_3DNR21 (IO_PREPROC_BASE+0xc84)
    #define shiftth Fld(10, 11, AC_MSKW21) //20:11
    #define crsumbg Fld(11, 0, AC_MSKW10) //10:0
#define NR_3DNR22 (IO_PREPROC_BASE+0xc88)
    #define disciwithc Fld(1, 30, AC_MSKB3) //30
    #define shiftsel Fld(1, 29, AC_MSKB3) //29
    #define disshift Fld(1, 28, AC_MSKB3) //28
    #define nocsamepsel Fld(1, 27, AC_MSKB3) //27
    #define newcr Fld(2, 24, AC_MSKB3) //25:24
    #define blockcrth Fld(8, 16, AC_FULLB2) //23:16
    #define cismall Fld(8, 8, AC_FULLB1) //15:8
    #define ybpfth Fld(8, 0, AC_FULLB0) //7:0
#define NR_3DNR23 (IO_PREPROC_BASE+0xc8c)
    #define doitexv Fld(2, 30, AC_MSKB3) //31:30
    #define doitexh Fld(1, 29, AC_MSKB3) //29
    #define doitsel Fld(1, 28, AC_MSKB3) //28
    #define withcth Fld(8, 16, AC_FULLB2) //23:16
    #define inkth_ccs Fld(8, 8, AC_FULLB1) //15:8
    #define ink Fld(6, 0, AC_MSKB0) //5:0
#define NR_3DNR24 (IO_PREPROC_BASE+0xc90)
    #define chromath Fld(5, 26, AC_MSKB3) //30:26
    #define chromasmth Fld(13, 12, AC_MSKDW) //24:12
    #define clpsel Fld(1, 10, AC_MSKB1) //10
    #define lumadifsmth Fld(8, 0, AC_FULLB0) //7:0
#define NR_3DNR25 (IO_PREPROC_BASE+0xc94)
    #define whocolorsmpix Fld(13, 19, AC_MSKW32) //31:19
    #define wholecolor_en Fld(1, 18, AC_MSKB2) //18
    #define ccscr Fld(1, 17, AC_MSKB2) //17
    #define encsmanu2 Fld(1, 16, AC_MSKB2) //16
    #define ccsmanug Fld(4, 12, AC_MSKB1) //15:12
    #define encsmanu1 Fld(1, 11, AC_MSKB1) //11
    #define enblock1 Fld(1, 10, AC_MSKB1) //10
    #define yipbothhp Fld(1, 9, AC_MSKB1) //9
    #define enwordg Fld(1, 8, AC_MSKB1) //8
    #define yipbothhpcg Fld(4, 4, AC_MSKB0) //7:4
    #define wordcg Fld(4, 0, AC_MSKB0) //3:0
#define NR_3DNR26 (IO_PREPROC_BASE+0xc98)
    #define llv3 Fld(10, 20, AC_MSKW32) //29:20
    #define csmlvgth Fld(20, 0, AC_MSKDW) //19:0
#define NR_3DNR27 (IO_PREPROC_BASE+0xc9c)
    #define enrndwfix Fld(1, 31, AC_MSKB3) //31
    #define wfix16_32 Fld(1, 29, AC_MSKB3) //29
    #define wfix8_16 Fld(1, 28, AC_MSKB3) //28
    #define wfix4_8 Fld(1, 27, AC_MSKB3) //27
    #define wfix2_4 Fld(1, 26, AC_MSKB3) //26
    #define wfix1_2 Fld(1, 25, AC_MSKB3) //25
    #define wfix0_1 Fld(1, 24, AC_MSKB3) //24
    #define dinth Fld(8, 16, AC_FULLB2) //23:16
    #define bp_interlv Fld(1, 15, AC_MSKB1) //15
    #define bp_frtg Fld(1, 14, AC_MSKB1) //14
    #define idfsel Fld(2, 12, AC_MSKB1) //13:12
    #define en_lumainc Fld(1, 11, AC_MSKB1) //11
    #define lumainc Fld(10, 0, AC_MSKW10) //9:0
#define NR_3DNR28 (IO_PREPROC_BASE+0xca0)
    #define peaklumatb Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR29 (IO_PREPROC_BASE+0xca4)
    #define uvsmallth Fld(8, 24, AC_FULLB3) //31:24
    #define yi7x2maxth Fld(8, 16, AC_FULLB2) //23:16
    #define yi7x2minth Fld(8, 8, AC_FULLB1) //15:8
    #define uvsmsel Fld(2, 4, AC_MSKB0) //5:4
    #define phnum Fld(4, 0, AC_MSKB0) //3:0
#define NR_3DNR2A (IO_PREPROC_BASE+0xca8)
    #define ccsen Fld(1, 31, AC_MSKB3) //31
    #define ccsen4y Fld(1, 30, AC_MSKB3) //30
    #define yipbothhpcd_old Fld(4, 4, AC_MSKB0) //7:4
    #define wordcg_old Fld(4, 0, AC_MSKB0) //3:0
#define NR_3DNR2B (IO_PREPROC_BASE+0xcac)
    #define Empty Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR2D (IO_PREPROC_BASE+0xcb4)
    #define c_clear Fld(1, 31, AC_MSKB3) //31
    #define c_small_en Fld(1, 30, AC_MSKB3) //30
    #define c_luma_aux_en Fld(1, 29, AC_MSKB3) //29
    #define c_src_sel Fld(1, 28, AC_MSKB3) //28
    #define c_vline_ed Fld(11, 16, AC_MSKW32) //26:16
    #define c_luma_sel Fld(2, 12, AC_MSKB1) //13:12
    #define c_vline_st Fld(11, 0, AC_MSKW10) //10:0
#define NR_3DNR2F (IO_PREPROC_BASE+0xcbc)
    #define c_small_countthl Fld(4, 28, AC_MSKB3) //31:28
    #define c_max_sel Fld(1, 27, AC_MSKB3) //27
    #define c_scale_sel Fld(3, 24, AC_MSKB3) //26:24
    #define c_motion_thl Fld(8, 16, AC_FULLB2) //23:16
    #define c_small_diffthl Fld(4, 12, AC_MSKB1) //15:12
    #define c_hpix_ed Fld(12, 0, AC_MSKW10) //11:0
#define NR_3DNR30 (IO_PREPROC_BASE+0xcc0)
    #define dkbostbl Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR31 (IO_PREPROC_BASE+0xcc4)
    #define hegth Fld(8, 12, AC_MSKW21) //19:12
    #define ylv_yth Fld(10, 0, AC_MSKW10) //9:0
#define NR_3DNR32 (IO_PREPROC_BASE+0xcc8)
    #define Hftbl Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR33 (IO_PREPROC_BASE+0xccc)
    #define hend_metric Fld(11, 12, AC_MSKW21) //22:12
    #define hstart_metric Fld(11, 0, AC_MSKW10) //10:0
#define NR_3DNR34 (IO_PREPROC_BASE+0xcd0)
    #define vend_metric Fld(11, 12, AC_MSKW21) //22:12
    #define vstart_metric Fld(11, 0, AC_MSKW10) //10:0
#define NR_3DNR36 (IO_PREPROC_BASE+0xcd8)
    #define dkmoth Fld(8, 24, AC_FULLB3) //31:24
    #define st_manwcu Fld(6, 16, AC_MSKB2) //21:16
    #define hdstth Fld(8, 8, AC_FULLB1) //15:8
    #define hsmpctbth Fld(7, 0, AC_MSKB0) //6:0
#define NR_3DNR37 (IO_PREPROC_BASE+0xcdc)
    #define c_turn_2_prev_rec Fld(8, 16, AC_FULLB2) //23:16
    #define c_turn_1_prev_rec Fld(8, 8, AC_FULLB1) //15:8
    #define c_init_prev_rec Fld(8, 0, AC_FULLB0) //7:0
#define NR_3DNR38 (IO_PREPROC_BASE+0xce0)
    #define c_max_prev_rec Fld(8, 24, AC_FULLB3) //31:24
    #define c_slope2_prev_rec Fld(8, 16, AC_FULLB2) //23:16
    #define c_slope1_prev_rec Fld(8, 8, AC_FULLB1) //15:8
    #define c_slope0_prev_rec Fld(8, 0, AC_FULLB0) //7:0
#define NR_3DNR39 (IO_PREPROC_BASE+0xce4)
    #define yedgeth Fld(8, 24, AC_FULLB3) //31:24
    #define hsmopixcnt_th Fld(20, 0, AC_MSKDW) //19:0
#define NR_3DNR3A (IO_PREPROC_BASE+0xce8)
    #define gnr_tbth Fld(7, 24, AC_MSKB3) //30:24
    #define highmoth Fld(4, 20, AC_MSKB2) //23:20
    #define hsmopixcnt_th Fld(20, 0, AC_MSKDW) //19:0
#define NR_3DNR3B (IO_PREPROC_BASE+0xcec)
    #define fsmoothtb Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR3C (IO_PREPROC_BASE+0xcf0)
    #define bigmoth Fld(8, 24, AC_FULLB3) //31:24
    #define manbwcu2 Fld(6, 16, AC_MSKB2) //21:16
    #define manbwcuH Fld(6, 8, AC_MSKB1) //13:8
    #define manbwcu Fld(6, 0, AC_MSKB0) //5:0
#define NR_3DNR3D (IO_PREPROC_BASE+0xcf4)
    #define ruiPxlMotionThl_Mo2D Fld(8, 8, AC_FULLB1) //15:8
    #define ruiPxlMotionThl_St2D Fld(8, 0, AC_FULLB0) //7:0
#define NR_3DNR3E (IO_PREPROC_BASE+0xcf8)
    #define hstart Fld(11, 12, AC_MSKW21) //22:12
    #define hend Fld(11, 0, AC_MSKW10) //10:0
#define NR_3DNR3F (IO_PREPROC_BASE+0xcfc)
    #define vstart Fld(11, 12, AC_MSKW21) //22:12
    #define vend Fld(11, 0, AC_MSKW10) //10:0
#define NR_3DNR40 (IO_PREPROC_BASE+0xd00)
    #define rui_FrSt_StEdge_table Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR41 (IO_PREPROC_BASE+0xd04)
    #define rui_FrSt_MoEdge_table Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR42 (IO_PREPROC_BASE+0xd08)
    #define rui_FrMo_StEdge_table Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR43 (IO_PREPROC_BASE+0xd0c)
    #define rui_FrMo_MoEdge_table Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR44 (IO_PREPROC_BASE+0xd10)
    #define rui_FrMo_MoEdge_table_th Fld(7, 24, AC_MSKB3) //30:24
    #define rui_FrMo_StEdge_table_th Fld(7, 16, AC_MSKB2) //22:16
    #define rui_FrSt_MoEdge_table_th Fld(7, 8, AC_MSKB1) //14:8
    #define rui_FrSt_StEdge_table_th Fld(7, 0, AC_MSKB0) //6:0
#define NR_3DNR45 (IO_PREPROC_BASE+0xd14)
    #define rui3DMoType_Mo Fld(3, 28, AC_MSKB3) //30:28
    #define ruiPxlLPThl_Mo Fld(10, 16, AC_MSKW32) //25:16
    #define rui3DMoType_St Fld(3, 12, AC_MSKB1) //14:12
    #define ruiPxlLPThl_St Fld(10, 0, AC_MSKW10) //9:0
#define NR_3DNR46 (IO_PREPROC_BASE+0xd18)
    #define ruiSkinToneType Fld(3, 28, AC_MSKB3) //30:28
    #define ruiPxlMotionThl_Mo Fld(10, 16, AC_MSKW32) //25:16
    #define ruiPxlMotionThl_St Fld(10, 0, AC_MSKW10) //9:0
#define NR_3DNR47 (IO_PREPROC_BASE+0xd1c)
    #define ruiNewMoLPTp Fld(2, 28, AC_MSKB3) //29:28
    #define rbDyEdgeDetEn_All Fld(1, 26, AC_MSKB3) //26
    #define rbMoEdgeDetEn_FrMo Fld(1, 25, AC_MSKB3) //25
    #define rbStEdgeDetEn_FrMo Fld(1, 24, AC_MSKB3) //24
    #define rbMoEdgeDetEn_FrSt Fld(1, 23, AC_MSKB2) //23
    #define rbStEdgeDetEn_FrSt Fld(1, 22, AC_MSKB2) //22
    #define rbFrBaseStEn Fld(1, 21, AC_MSKB2) //21
    #define rbDyFrBaseStEn Fld(1, 20, AC_MSKB2) //20
    #define ruiFrMoCntThl3D Fld(4, 16, AC_MSKB2) //19:16
    #define ruiFrStCntThl3D Fld(4, 12, AC_MSKB1) //15:12
    #define ruiFrSumThl3D Fld(11, 0, AC_MSKW10) //10:0
#define NR_3DNR48 (IO_PREPROC_BASE+0xd20)
    #define hend_FrSt Fld(11, 20, AC_MSKW32) //30:20
    #define hstart_FrSt Fld(11, 0, AC_MSKW10) //10:0
#define NR_3DNR49 (IO_PREPROC_BASE+0xd24)
    #define vend_FrSt Fld(11, 20, AC_MSKW32) //30:20
    #define vstart_FrSt Fld(11, 0, AC_MSKW10) //10:0
#define NR_3DNR4A (IO_PREPROC_BASE+0xd2c)
    #define rui3DMoType_Mo2D Fld(3, 28, AC_MSKB3) //30:28
    #define ruiPxlLPThl_Mo2D Fld(10, 16, AC_MSKW32) //25:16
    #define rui3DMoType_St2D Fld(3, 12, AC_MSKB1) //14:12
    #define rb2DNewMoEn Fld(1, 11, AC_MSKB1) //11
    #define rb2DMoSumTp Fld(1, 10, AC_MSKB1) //10
    #define ruiPxlLPThl_St2D Fld(10, 0, AC_MSKW10) //9:0
#define NR_3DNR50 (IO_PREPROC_BASE+0xd40)
    #define new_meter_diff_area2 Fld(16, 16, AC_FULLW32) //31:16
    #define new_meter_diff_area1 Fld(16, 0, AC_FULLW10) //15:0
#define NR_3DNR51 (IO_PREPROC_BASE+0xd44)
    #define new_meter_diff_area4 Fld(16, 16, AC_FULLW32) //31:16
    #define new_meter_diff_area3 Fld(16, 0, AC_FULLW10) //15:0
#define NR_3DNR52 (IO_PREPROC_BASE+0xd48)
    #define new_meter_diff_area6 Fld(16, 16, AC_FULLW32) //31:16
    #define new_meter_diff_area5 Fld(16, 0, AC_FULLW10) //15:0
#define NR_3DNR53 (IO_PREPROC_BASE+0xd4c)
    #define new_meter_diff_area8 Fld(16, 16, AC_FULLW32) //31:16
    #define new_meter_diff_area7 Fld(16, 0, AC_FULLW10) //15:0
#define NR_3DNR54 (IO_PREPROC_BASE+0xd50)
    #define new_meter_diff_area10 Fld(16, 16, AC_FULLW32) //31:16
    #define new_meter_diff_area9 Fld(16, 0, AC_FULLW10) //15:0
#define NR_3DNR55 (IO_PREPROC_BASE+0xd54)
    #define new_meter_diff_area12 Fld(16, 16, AC_FULLW32) //31:16
    #define new_meter_diff_area11 Fld(16, 0, AC_FULLW10) //15:0
#define NR_3DNR56 (IO_PREPROC_BASE+0xd58)
    #define new_meter_diff_area14 Fld(16, 16, AC_FULLW32) //31:16
    #define new_meter_diff_area13 Fld(16, 0, AC_FULLW10) //15:0
#define NR_3DNR57 (IO_PREPROC_BASE+0xd5c)
    #define new_meter_diff_area16 Fld(16, 16, AC_FULLW32) //31:16
    #define new_meter_diff_area15 Fld(16, 0, AC_FULLW10) //15:0
#define NR_3DNR58 (IO_PREPROC_BASE+0xd60)
    #define new_meter_diff_3d Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR59 (IO_PREPROC_BASE+0xd64)
    #define new_meter_diff_2d Fld(32, 0, AC_FULLDW) //31:0
#define NR_3DNR5A (IO_PREPROC_BASE+0xd68)
    #define new_meter_smooth_count Fld(16, 16, AC_FULLW32) //31:16
    #define new_meter_motion_status Fld(16, 0, AC_FULLW10) //15:0
#define NR_3DNR5B (IO_PREPROC_BASE+0xd6c)
    #define new_meter_small_status Fld(16, 16, AC_FULLW32) //31:16
    #define new_meter_noise_value Fld(16, 0, AC_FULLW10) //15:0
#define NR_3DNR5C (IO_PREPROC_BASE+0xd70)
    #define b_fr_base_st_NewMo_3D Fld(1, 31, AC_MSKB3) //31
    #define PW_MoCnt_Final Fld(20, 0, AC_MSKDW) //19:0
#define NR_3DNR5D (IO_PREPROC_BASE+0xd74)
    #define sp_fr_smlmo Fld(1, 31, AC_MSKB3) //31
    #define line_length Fld(11, 20, AC_MSKW32) //30:20
    #define mopixcnt Fld(20, 0, AC_MSKDW) //19:0
#define NR_3DNR5E (IO_PREPROC_BASE+0xd78)
    #define fr_no_color Fld(1, 5, AC_MSKB0) //5
    #define fr_smlmo Fld(1, 4, AC_MSKB0) //4
    #define old_fr_sti Fld(1, 3, AC_MSKB0) //3
    #define fr_bigmo Fld(1, 2, AC_MSKB0) //2
    #define fr_very_bigmo Fld(1, 1, AC_MSKB0) //1
    #define fr_smooth Fld(1, 0, AC_MSKB0) //0
#define NR_3DNR5F (IO_PREPROC_BASE+0xd7c)
    #define peak_luma Fld(16, 16, AC_FULLW32) //31:16
    #define noise_value Fld(16, 0, AC_FULLW10) //15:0
#define NR_TOP_PIP_00 (IO_PREPROC_BASE+0xa00)
    #define C_SHOW_POS_PIP_POSITION Fld(1, 31, AC_MSKB3) //31
    #define C_SHOW_PIP_STEP Fld(1, 27, AC_MSKB3) //27
    #define C_P2H_INV Fld(1, 23, AC_MSKB2) //23
    #define C_P2V_INV Fld(1, 22, AC_MSKB2) //22
    #define C_P2F_INV Fld(1, 21, AC_MSKB2) //21
    #define C_PIP2_V_RST Fld(1, 20, AC_MSKB2) //20
    #define C_PIP_NR_EN Fld(1, 0, AC_MSKB0) //0    
/* end of 5387 register*/    

#endif //_HW_NR_H_
