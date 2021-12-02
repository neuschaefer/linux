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
 * $Author: cheng.chen $
 * $Date: 2012/07/06 $
 * $RCSfile: gfx_dif_reg.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx_dif_reg.h
 *  gfx driver hardware registers
 *  5371 specific
 */

/*
    lint inhibition(s)
        46   : suppress MISRA rule 111, field type should be int
        960  : suppress MISRA rule 110, bitfields inside union
               here size of bitfield has been carefully calculated
               to fit union size
*/

#ifndef __VGFX_DIF_REG_H__
#define __VGFX_DIF_REG_H__

/*lint -save -e46 -e960 */

// vgfx hardware register enumeration
typedef enum _E_VGFX_REG_T
{
    VGREG_G_CONFIG           = 0x00,     // 0x5c000
    VGREG_G_STATUS           = 0x01,     // 0x5c004
    VGREG_0x5c008         = 0x02,     // 0x5c008
    VGREG_0x5c00c          = 0x03,     // 0x5c00C
    VGREG_WB_MODE             = 0x04,     // 0x5c010
    VGREG_IMG_BSAD             = 0x08,     // 0x5c020
    VGREG_TXT_START             = 0x09,     // 0x5c024
    VGREG_TXT_END           = 0x0A,     // 0x5c028
    VGREG_IMG_PITCH        = 0x0B,     // 0x5c02C
    VGREG_IMG_CONFIG            = 0x0C,     // 0x5c030
    VGREG_ALPHA_SHRINK        = 0x10,     // 0x5c040
    VGREG_WB_BSAD          = 0x40,	//0x5c100
    VGREG_BB_START            = 0x41,	//0x5c104
    VGREG_BB_SIZE           = 0x42,	//0x5c108
    VGREG_WB_PITCH            = 0x43,	//0x5c10c
    VGREG_BB_BASED            = 0x44,	//0x5c110
    VGREG_WB_EXP		= 0x45,//0x5c114
    VGREG_COEF_A            = 0x80,	//0x5c200
    VGREG_COEF_B            = 0x81,	//0x5c204
    VGREG_COEF_C            = 0x82,	//0x5c208
    VGREG_COEF_D            = 0x83,	//0x5c20c
    VGREG_COEF_E            = 0x84,	//0x5c210
    VGREG_COEF_F            = 0x85,	//0x5c214
    VGREG_COEF_G            = 0x86,	//0x5c218
    VGREG_COEF_H            = 0x87,	//0x5c21c
    VGREG_COEF_I            = 0x88,	//0x5c220
    VGREG_FILTER_CONFIG            = 0xC0,		//0x5c300 

	VGREG_REF_CONFIG			 = 0xC1,	 //0x5c304
	VGREG_REFRATIO_CONFIG			  = 0xC2,		 //0x5c308
	VGREG_FILTER_FIFO		 =0xC3,

    VGREG_COEF_4_TAP_0            = 0xD0, //0x5c340
    VGREG_COEF_4_TAP_1            = 0xD1, //0x5c344
    VGREG_COEF_4_TAP_2            = 0xD2, //0x5c348
    VGREG_COEF_4_TAP_3            = 0xD3, //0x5c34c
    VGREG_COEF_4_TAP_4            = 0xD4, //0x5c350
    VGREG_COEF_4_TAP_5            = 0xD5, //0x5c354
    VGREG_COEF_4_TAP_6            = 0xD6, //0x5c358
    VGREG_COEF_4_TAP_7            = 0xD7, //0x5c35c
    VGREG_COEF_4_TAP_8            = 0xD8, //0x5c360
    VGREG_COEF_4_TAP_9            = 0xE0, //0x5c364
    VGREG_COEF_4_TAP_A            = 0xE0, //0x5c368
    VGREG_COEF_4_TAP_B            = 0xE1, //0x5c36c
    VGREG_COEF_4_TAP_C            = 0xE2, //0x5c370
    VGREG_COEF_4_TAP_D            = 0xE3, //0x5c374
    VGREG_COEF_4_TAP_E            = 0xE4, //0x5c378
    VGREG_COEF_4_TAP_F            = 0xE5, //0x5c37c

    VGREG_ALCOM_LOOP            = 0x140,		//0x5c500
    VGREG_LEGAL_START            = 0x180,		//0x5c600
    VGREG_LEGAL_END            = 0x181,		//0x5c604
    VGREG_PCTCTL_FIFO		=0x182,//0x5608
    VGREG_PCTCTL_END            = 0x183,		//0x560c
    VGREG_WB_RND            = 0x184,		//0x5c610
    VGREG_PSTCTL_ARGB            = 0x185,		//0x5c614
    VGREG_LAST     // use VGREG_NUM replace
} E_VGFX_REG_T;

enum
{
    VGREG_G_CONFIG_NUM           = 0x00,     // 0x5c000
    VGREG_G_STATUS_NUM  ,     // 0x5c004
    VGREG_0x5c008_NUM ,     // 0x5c008
    VGREG_0x5c00c_NUM  ,     // 0x5c00C
    VGREG_WB_MODE_NUM    ,     // 0x5c010
    VGREG_IMG_BSAD_NUM,     // 0x5c020
    VGREG_TXT_START_NUM,     // 0x5c024
    VGREG_TXT_END_NUM,     // 0x5c028
    VGREG_IMG_PITCH_NUM,     // 0x5c02C
    VGREG_IMG_CONFIG_NUM,     // 0x5c030
    VGREG_ALPHA_SHRINK_NUM,     // 0x5c040
    VGREG_WB_BSAD_NUM,	//0x5c100
    VGREG_BB_START_NUM,	//0x5c104
    VGREG_BB_SIZE_NUM,	//0x5c108
    VGREG_WB_PITCH_NUM,	//0x5c10c
    VGREG_BB_BASED_NUM,	//0x5c110
    VGREG_WB_EXP_NUM,		//0x5c114
    VGREG_COEF_A_NUM,	//0x5c200
    VGREG_COEF_B_NUM,	//0x5c204
    VGREG_COEF_C_NUM,	//0x5c208
    VGREG_COEF_D_NUM,	//0x5c20c
    VGREG_COEF_E_NUM,	//0x5c210
    VGREG_COEF_F_NUM,	//0x5c214
    VGREG_COEF_G_NUM,	//0x5c218
    VGREG_COEF_H_NUM,	//0x5c21c
    VGREG_COEF_I_NUM,	//0x5c220
    VGREG_FILTER_CONFIG_NUM,		//0x5c300

    VGREG_REF_CONFIG_NUM,		//0x5c304
    VGREG_REFRATIO_CONFIG_NUM,	//0x5c308
    VGREG_FILTER_FIFO_NUM,	//0x5c30c


    VGREG_COEF_4_TAP_0_NUM,	//0x5c340
    VGREG_COEF_4_TAP_1_NUM,	//0x5c344
    VGREG_COEF_4_TAP_2_NUM,	//0x5c348
    VGREG_COEF_4_TAP_3_NUM,	//0x5c34c
    VGREG_COEF_4_TAP_4_NUM,	//0x5c350
    VGREG_COEF_4_TAP_5_NUM,	//0x5c354
    VGREG_COEF_4_TAP_6_NUM,	//0x5c358
    VGREG_COEF_4_TAP_7_NUM,	//0x5c35c
    VGREG_COEF_4_TAP_8_NUM,   //0x5c360
    VGREG_COEF_4_TAP_9_NUM,   //0x5c364
    VGREG_COEF_4_TAP_A_NUM,   //0x5c368
    VGREG_COEF_4_TAP_B_NUM,   //0x5c36c
    VGREG_COEF_4_TAP_C_NUM,   //0x5c370
    VGREG_COEF_4_TAP_D_NUM,   //0x5c374
    VGREG_COEF_4_TAP_E_NUM,   //0x5c378
    VGREG_COEF_4_TAP_F_NUM,   //0x5c37c

    VGREG_ALCOM_LOOP_NUM,		//0x5c500
    VGREG_LEGAL_START_NUM,		//0x5c600
    VGREG_LEGAL_END_NUM,		//0x5c604
    VGREG_PCTCTL_FIFO_NUM,		//0x5608
    VGREG_PCTCTL_END_NUM,		//0x560c
    VGREG_WB_RND_NUM,		//0x5c610
    VGREG_PSTCTL_ARGB_NUM ,		//0x5c614
    VGREG_LAST_NUM // 0x4100
} ;


// gfx hw register bitfield

typedef struct _MI_DIF_VGFX_T0
{
    // DWORD - G_CONFIG         (5c000h)
UINT32           :  9;
UINT32 vfg_DFIFO_THRS      :  2;
UINT32 vfg_DST_FF_FILL        :  1;
UINT32 vfg_DST_LEN_EN        :  1;
UINT32                      :  4;
UINT32 vfg_ENG_LG          :  1;
UINT32 vfg_MON_SET                     :  7;
UINT32 vfg_CQ_RST            :  2;
UINT32              :  3;
UINT32 vfg_G_RST             :  2;

    // DWORD          (5c004h)
UINT32 vfg_IDLE		:  1; // (5c004h)
UINT32                      :  31; // (5c004h)
UINT32                      :  32;  // (5c008h)
UINT32			  :  32;  // (5c00ch)

    // DWORD - DRAMQ_STAD       (5c010h)
UINT32 vfg_WB_CM          :  4;
UINT32           :  12;
UINT32 vfg_WB_GLOBAL_ALPHA        : 8;
UINT32           :  8;

    // DWORD - img base address        (5c020h)
UINT32 vfg_IMG_BSAD        : 32;
    // DWORD - txt config        (5c024h)
UINT32 vfg_TXT_VSTART        : 11;
UINT32          : 5;
UINT32 vfg_TXT_HSTART        : 11;
UINT32         : 5;
    // DWORD - txt config        (5c028h)
UINT32 vfg_TXT_VEND        : 11;
UINT32          : 5;
UINT32 vfg_TXT_HEND        : 11;
UINT32         : 5;
    // DWORD - img pitch       (5c02ch)
UINT32 vfg_IMG_PITCH        : 13;
UINT32          : 19;
    // DWORD - img config       (5c030h)
UINT32 vfg_IMG_GL_ALPHA        : 8;
UINT32  vfg_IMG_CM        : 4;
UINT32          : 20;
    // DWORD - alpha shrink       (5c040h)
UINT32 vfg_ALPHA_NONZERO 		: 1;
UINT32		: 31;
    // DWORD - WB basd add       (5c100h)
UINT32 vfg_WB_BSAD        : 32;
    // DWORD - BB coordinate of top-left       (5c104h)
UINT32 vfg_BB_TL_X          : 11;
UINT32 		: 5;
UINT32 vfg_BB_TL_Y          : 11;
UINT32		: 5;
    // DWORD - BB size       (5c108h)
UINT32 vfg_BB_WIDTH        : 11;
UINT32 		: 5;
UINT32 vfg_BB_HEIGHT         : 11;
UINT32		: 5;
    // DWORD - WB pitch       (5c10ch)
UINT32 vfg_WB_PITCH          : 13;
UINT32 		: 19;
    // DWORD - BB base address       (5c110h)
UINT32 vfg_BB_BSAD         : 32;
    // DWORD - wb expansion       (5c114h)
UINT32 vfg_WB_EXP         : 1;
UINT32		: 31;
    // DWORD - coefficient  a       (5c200h)
UINT32 vfg_COEF_A         : 23;
UINT32		: 9;
    // DWORD - coefficient  a       (5c204h)
UINT32 vfg_COEF_B         : 23;
UINT32		: 9;
    // DWORD - coefficient  a       (5c208h)
UINT32 vfg_COEF_C         : 23;
UINT32		: 9;
    // DWORD - coefficient  a       (5c20ch)
UINT32 vfg_COEF_D         : 23;
UINT32		: 9;
    // DWORD - coefficient  a       (5c210h)
UINT32 vfg_COEF_E         : 23;
UINT32		: 9;
    // DWORD - coefficient  a       (5c214h)
UINT32 vfg_COEF_F         : 23;
UINT32		: 9;
    // DWORD - coefficient  a       (5c218h)
UINT32 vfg_COEF_G         : 23;
UINT32		: 9;
    // DWORD - coefficient  a       (5c21ch)
UINT32 vfg_COEF_H        : 23;
UINT32		: 9;
    // DWORD - coefficient  a       (5c220h)
UINT32 vfg_COEF_I         : 23;
UINT32		: 9;
    // DWORD - filter config       (5c300h)
UINT32 vfg_FILTER_EN         : 1;
UINT32 vfg_ANTI_ALIAS_EN		: 1;
UINT32 vfg_IMG_EXP_MODE		: 1;

UINT32 vfg_ALPHA_EDGE_RGB0		: 1;
UINT32 vfg_ALPHA_EDGE_MODE		: 2;

UINT32 vfg_FILTER_TYPE		: 1;
UINT32 		: 1;
UINT32 vfg_ALPHA_EDGE_THRS	: 8;
UINT32		: 8;
UINT32 vfg_TXT_B_ORDER  	:2;
UINT32 vfg_TXT_G_ORDER  	:2;
UINT32 vfg_TXT_R_ORDER  	:2;
UINT32 vfg_TXT_A_ORDER  	:2;

// DWORD - reflect config    (5c304h)
UINT32 vfg_REF_EN	: 1; 
UINT32 vfg_REF_AA_EN	: 1;
UINT32 vfg_REF_ONLY : 1;
UINT32	: 13;
UINT32 vfg_REF_TXT_H	: 11;
UINT32		: 5;
// DWORD - reflect ratio config   (5c308h)
UINT32 vfg_REF_RATIO_STEP  :8;
UINT32		:8;
UINT32 vfg_REF_RATIO_INIT	:5;
UINT32		:11;
// DWORD - filter FIFO depth	  (5c30ch)
UINT32 vfg_VQUE_DEPTH	:7;
UINT32		:9;
UINT32 vfg_AQUE_DEPTH	:5;
UINT32		:3;
UINT32 vfg_XQUE_DEPTH	:5;
UINT32		:3;

// DWORD - filter FIFO depth	  (5c340h)
UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_01	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_00	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_11	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_10	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_21	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_20	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_31	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_30	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_41	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_40	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_51	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_50	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_61	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_60	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_71	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_70	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_81	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_80	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_91	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_90	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_A1	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_A0	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_B1	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_B0	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_C1	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_C0	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_D1	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_D0	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_E1	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_E0	:10;
UINT32		                :2;

UINT32		                :4;
UINT32 vfg_COEFF_4_TAP_F1	:10;
UINT32		                :6;
UINT32 vfg_COEFF_4_TAP_F0	:10;
UINT32		                :2;


    // DWORD - alpha composition        (5c500h)
UINT32		: 5;
UINT32 vfg_ALCOM_LOOP		: 1;
UINT32 vfg_ROP_BITBLT		: 1;
UINT32  vfg_BITBLT_EN		: 1;
UINT32 vfg_ALU_EN         : 1;
UINT32  		: 23;
    // DWORD - legal start address        (5c600h)
UINT32 vfg_LG_ST_AD		: 32;
    // DWORD - legal end address        (5c604h)
UINT32 vfg_LG_END_AD		: 32;
    // DWORD - post ctl fifo        (5c608h)
UINT32 vfg_POST_THRS		: 5;
UINT32 		: 3;
UINT32 vfg_POST_DEPTH		: 5;
UINT32  		: 19;
    // DWORD - post ctl end criterion		(5c60ch)
UINT32 vfg_WDLE_EN	   : 1;
UINT32 vfg_WR_PROT_EN	   : 1;
UINT32   	   : 30;
    // DWORD - wb rounding wnable		(5c610h)
UINT32 vfg_WB_RND_EN	   : 1;
UINT32  	   : 31;
    // DWORD - post write argb sequence 		(5c614h)
UINT32 vfg_B_SEL	   : 2;
UINT32 vfg_G_SEL	   : 2;
UINT32 vfg_R_SEL	   : 2;
UINT32 vfg_A_SEL	   : 2;
UINT32  	   : 24;

} MI_DIF_VGFX_T0;


// gfx hw register name/bitfield union
typedef union _MI_DIF_VGFX_UNION_T
{
    E_VGFX_REG_T   rReg;
    MI_DIF_VGFX_T0  rField;
} MI_DIF_VGFX_UNION_T;

/*lint -restore */
#endif

