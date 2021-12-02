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
 * $Date  $
 * $RCSfile: hw_scart.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _HW_SCART_H_
#define _HW_SCART_H_

#include "x_hal_5381.h"

#define SCART_00 0x2620
 #define SCART_00_0 0x2620
 #define SCART_00_1 0x2621
 #define SCART_00_2 0x2622
 #define SCART_00_3 0x2623
	#define FLD_HOLD_SCART_STATUS Fld(1, 31, AC_MSKB3) //31
	#define FLD_FB_TST Fld(1, 30, AC_MSKB3) //30
	#define FLD_FB_DBG Fld(1, 29, AC_MSKB3) //29
    #define FLD_RGBOFFSET Fld(5, 16, AC_MSKB2) //20:16
    #define FLD_YOFFSET Fld(6, 10, AC_MSKB1) //15:10
	#define FLD_MIX_ORDER Fld(1, 9, AC_MSKB1) //9
	#define FLD_MIX_MODE Fld(1, 8, AC_MSKB1) //8
	#define FLD_MON_V_POL Fld(1, 7, AC_MSKB0) //7
	#define FLD_MON_H_POL Fld(1, 6, AC_MSKB0) //6
	#define FLD_HDTV_V_POL Fld(1, 5, AC_MSKB0) //5
	#define FLD_HDTV_H_POL Fld(1, 4, AC_MSKB0) //4
	#define FLD_TVD_V_POL Fld(1, 3, AC_MSKB0) //3
	#define FLD_TVD_H_POL Fld(1, 2, AC_MSKB0) //2
	#define FLD_FIFO_MODE Fld(1, 1, AC_MSKB0) //1
	#define FLD_OUTPUT_EN Fld(1, 0, AC_MSKB0) //0
#define SCART_01 0x2624
 #define SCART_01_0 0x2624
 #define SCART_01_1 0x2625
 #define SCART_01_2 0x2626
 #define SCART_01_3 0x2627
	#define FLD_FIFO_DEPTH Fld(7, 24, AC_MSKB3) //30:24
	#define FLD_FB_DELAY Fld(5, 16, AC_MSKB2) //20:16
	#define FLD_FB_HIGH_TH Fld(3, 12, AC_MSKB1) //14:12
	#define FLD_FB_LOW_TH Fld(3, 8, AC_MSKB1) //10:8
	#define FLD_FB_DEGLITCH Fld(2, 4, AC_MSKB0) //5:4
	#define FLD_FB_CLAMP_EN Fld(1, 3, AC_MSKB0) //3
	#define FLD_FB_POL Fld(1, 2, AC_MSKB0) //2
	#define FLD_FB_FORCE_OFF Fld(1, 1, AC_MSKB0) //1
	#define FLD_FB_FORCE_ON Fld(1, 0, AC_MSKB0) //0
#define SCART_02 0x2628
 #define SCART_02_0 0x2628
 #define SCART_02_1 0x2629
 #define SCART_02_2 0x262a
 #define SCART_02_3 0x262b
	#define FLD_TV_MUTE Fld(1, 28, AC_MSKB3) //28
	#define FLD_MUTE_Y Fld(4, 24, AC_MSKB3) //27:24
	#define FLD_MUTE_CB Fld(4, 20, AC_MSKB2) //23:20
	#define FLD_MUTE_CR Fld(4, 16, AC_MSKB2) //19:16
	#define FLD_RGB_IMUX Fld(3, 4, AC_MSKB0) //6:4
	#define FLD_CVBS_H_SEL Fld(1, 2, AC_MSKB0) // 2	
	#define FLD_ERR_DIFF Fld(1, 1, AC_MSKB0) //1
	#define FLD_RGB2YCBCR Fld(1, 0, AC_MSKB0) //0
#define SCART_STA_00 0x2070
 #define SCART_STA_00_0 0x2070
 #define SCART_STA_00_1 0x2071
 #define SCART_STA_00_2 0x2072
 #define SCART_STA_00_3 0x2073
	#define FLD_FB_ON_FLAG Fld(4, 28, AC_MSKB3) //31:28
	#define FLD_FB_OFF_FLAG Fld(4, 24, AC_MSKB3) //27:24
	#define FLD_FB_ON_CNT Fld(12, 12, AC_MSKW21) //23:12
	#define FLD_FB_OFF_CNT Fld(12, 0, AC_MSKW10) //11:0

#endif

