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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_wmvpsrd.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_wmvpsrd.h
 *  This header file declares exported APIs of wmv.
 */

#ifndef VDEC_WMVPSRD_H
#define VDEC_WMVPSRD_H
// just for definition of wmvpsr.c


#define NONE_ERROR          0x00000000
#define SEQH_LOC_ERR        0x00000001
#define GOPH_LOC_ERR        0x00000002
#define PICH_LOC_ERR        0x00000003
#define SEQ_ERR_CODE        0x00000004
#define HORI_SIZE_ERR       0x00000005
#define VERT_SIZE_ERR       0x00000006

//#define ASP_RAT_RES       0x00000007
#define ASP_RAT_RES         0x00000000

#define FRAME_RATE_ERR      0x00000008
#define BIT_RATE_RES        0x00000009
#define SEQH_MKB_ERR        0x0000000a
#define I_Q_MAT_ERR         0x0000000b
#define NI_Q_MAT_ERR        0x0000000c

#define SEQE_LOC_ERR        0x0000000d
#define PROF_ID_RES         0x0000000e
#define LEV_ID_RES          0x0000000f
#define CH_FORMAT_RES       0x00000010
#define SEQE_MKB_ERR        0x00000011

#define COLOR_PRI_ERR       0x00000012
#define TRA_CHA_ERR         0x00000013
#define MAT_COE_ERR         0x00000014
#define SEQDE_MKB_ERR       0x00000015
#define GOP_MKB_ERR         0x00000016
#define PIC_COD_TYP_ERR     0x00000017
#define F_CODE_00_ERR       0x00000018
#define F_CODE_01_ERR       0x00000019
#define F_CODE_10_ERR       0x0000001a
#define F_CODE_11_ERR       0x0000001b
#define PIC_STR_ERR         0x0000001c
#define PICDE_MKB1_ERR      0x0000001d
#define PICDE_MKB2_ERR      0x0000001e
#define PICCE_RES_ERR       0x0000001f
#define PICCE_MKB1_ERR      0x00000020
#define PICCE_MKB2_ERR      0x00000021
#define PICCE_MKB3_ERR      0x00000022

#define EXT_ST_COD_ERR      0x00000023
#define USR_DAT_BG_ERR      0x00000024

#define NO_START_C_ERR1     0x00000251
#define NO_START_C_ERR2     0x00000252
#define NO_START_C_ERR3     0x00000253
#define NO_START_C_ERR4     0x00000254
#define NO_START_C_ERR5     0x00000255
#define NO_START_C_ERR6     0x00000256
#define NO_START_C_ERR7     0x00000257
#define NO_START_C_ERR8     0x00000258
#define NO_START_C_ERR9     0x00000259
#define NO_START_C_ERR10    0x0000025A
#define NO_START_C_ERR11    0x0000025B
#define NO_START_C_ERR12    0x0000025C
#define NO_START_C_ERR13    0x0000025D
#define NO_START_C_ERR14    0x0000025E
#define NO_START_C_ERR15    0x0000025F
#define NO_START_C_ERR16    0x00000260

#define EXT_DATA_IDX_ERR    0x00000026
#define LINE21_MKB_ERR      0x00000027
#define LINE21_OVER_FLOW    0x00000028
#define BAR_SHIFTER_ERR     0x00000029

#define PIC_DIM_ERR         0x00000030

#define END_OF_FILE         0x00000031
#define VDSCL_SET_ERROR     0x00001234

// 14496-2
/* Visual Object layer */
#define VIS_OBJ_TYPE_ERR    0x00000001
#define M4V_NO_START_C_ERR1 0x00000002

/* VOL layer */
#define VOL_TOP_ERR         0x00000101
#define UNKNOWN_SHAPE_ERR   0x00000102
#define UNKNOWN_SPRITE_ERR  0x00000103
#define GMC_BR_CHG_ERR      0x00000104
#define NOT_8_BIT_ERR       0x00000105
#define COMP_EST_ERR        0x00000106
#define NEWPRED_ERR         0x00000107
#define SCALABILITY_ERR     0x00000108
#define INTRA_Q_BARSH_ERR   0x00000109
#define NINTRA_Q_BARSH_ERR  0x0000010a
#define DATA_PARTITION_ERR  0x0000010b

/* Short Header */
#define SRC_FMT_ERR         0x00000201

/* GOV layer */
#define GOV_MKB_ERR         0x00000301

/* VOP layer */
#define VOP_SC_ERR          0x00000401
#define WARPING_PT_ERR      0x00000402
#define VOP_CODED_0         0x00000403

/* WMV layer */
#define INVALID_32          0xffffffff
#define NO_USE              0xffffffff

/***************/
/* definitions */
/***************/
// WMV start code(s)
#define WMV_SC_PREFIX 0x000001
#define WMV_SC_ENDOFSEQ     0x10A
#define WMV_SC_SLICE        0x10B
#define WMV_SC_FIELD        0x10C
#define WMV_SC_FRAME        0x10D
#define WMV_SC_ENTRY        0x10E
#define WMV_SC_SEQ          0x10F
#define WMV_SC_SLICE_DATA   0x11B
#define WMV_SC_FIELD_DATA   0x11C
#define WMV_SC_FRAME_DATA   0x11D
#define WMV_SC_ENTRY_DATA   0x11E
#define WMV_SC_SEQ_DATA     0x11F

static UINT8 s_vopFirstFieldType[8] =
{ IVOP, IVOP, PVOP, PVOP, BVOP, BVOP, BIVOP, BIVOP };
static UINT8 s_vopSecondFieldType[8] =
{ IVOP, PVOP, IVOP, PVOP, BVOP, BIVOP, BVOP, BIVOP };
static UINT32 s_pXformLUT[4] =
{ XFORMMODE_8x8, XFORMMODE_8x4, XFORMMODE_4x8, XFORMMODE_4x4 };

/* wmv variable*/

// P field scaling
static CMVScale s_sMVScaleValuesFirstField[4] =
{
    { 32, 8, 37, 10, 512, 219, 128 },
    { 48, 12, 20, 5, 341, 236, 192 },
    { 53, 13, 14, 4, 307, 242, 213 },
    { 56, 14, 11, 3, 293, 245, 224 }
};
static CMVScale s_sMVScaleValuesSecondField[4] =
{
    { 32, 8, 37, 10, 512, 219, 128 },
    { 16, 4, 52, 13, 1024, 204, 64 },
    { 11, 3, 56, 14, 1536, 200, 43 },
    { 8, 2, 58, 15, 2048, 198, 32 }
};

// B field scaling
static CMVScale s_sMVScaleValuesFirstFieldB[4] =
{
    { 43, 11, 26, 7, 384, 230, 171 },
    { 51, 13, 17, 4, 320, 239, 205 },
    { 55, 14, 12, 3, 299, 244, 219 },
    { 57, 14, 10, 3, 288, 246, 228 }
};
static CMVScale s_sMVScaleValuesSecondFieldB[4] =
{
    { 32, 8, 37, 10, 512, 219, 128 },
    { 48, 12, 20, 5, 341, 236, 192 },
    { 53, 13, 14, 4, 307, 242, 213 },
    { 56, 14, 11, 3, 293, 245, 224 }
};

#if 0
static tPICTYPE WmvAdvPicType[16] =
{
    { PVOP, 1 },
    { PVOP, 1 },
    { PVOP, 1 },
    { PVOP, 1 },
    { PVOP, 1 },
    { PVOP, 1 },
    { PVOP, 1 },
    { PVOP, 1 },
    { BVOP, 2 },
    { BVOP, 2 },
    { BVOP, 2 },
    { BVOP, 2 },
    { IVOP, 3 },
    { IVOP, 3 },
    { BIVOP, 4 },
    { SKIPFRAME, 4 }
};
#endif

UINT32 _iStepRemap[32] =
{   NO_USE, 1, 2, 3, 4, 5, 6, 7, // (PQUANT)
    8, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 27, 29, 31
};
UINT32 _iNumShortVLC[7] =
{ 1, 1, 2, 1, 3, 1, 2 };
UINT32 _iDenShortVLC[7] =
{ 2, 3, 3, 4, 4, 5, 5 };
UINT32 _iNumLongVLC[14] =
{ 3, 4, 1, 5, 1, 2, 3, 4, 5, 6, 1, 3, 5, 7 };
UINT32 _iDenLongVLC[14] =
{ 5, 5, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8 };
UINT32 _iBInverse[8] =
{ 256, 128, 85, 64, 51, 43, 37, 32 };

#endif

