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
 * $RCSfile: mpv_errcode.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mpv_errcode.h
 *  This header file contains error code definition of MPV.
 */


#ifndef MPV_ERRCODE_H
#define MPV_ERRCODE_H


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// VParser error 

//syntax error
#define NONE_ERROR          0x00000000
#define SEQH_LOC_ERR        0x00000001
#define GOPH_LOC_ERR        0x00000002
#define PICH_LOC_ERR        0x00000003
#define MPEG_VER_ERR        0x00000004
#define ASP_RAT_RES         0x00000005
#define FRAME_RATE_ERR      0x00000006
#define BIT_RATE_RES        0x00000007  
#define SEQH_MKB_ERR        0x00000008 
#define PROF_ID_RES         0x00000009
#define LEV_ID_RES          0x0000000a
#define CH_FORMAT_RES       0x0000000b
#define SEQE_MKB_ERR        0x0000000c
#define COLOR_PRI_ERR       0x0000000d
#define TRA_CHA_ERR         0x0000000e
#define MAT_COE_ERR         0x0000000f
#define SEQDE_MKB_ERR       0x00000010
#define GOP_MKB_ERR         0x00000011
#define PIC_COD_TYP_ERR     0x00000012
#define F_CODE_00_ERR       0x00000013
#define F_CODE_01_ERR       0x00000014
#define F_CODE_10_ERR       0x00000015
#define F_CODE_11_ERR       0x00000016
#define PIC_STR_ERR         0x00000017
#define PICDE_MKB1_ERR      0x00000018
#define PICDE_MKB2_ERR      0x00000019
#define PICCE_MKB1_ERR      0x0000001a
#define PICCE_MKB2_ERR      0x0000001b 
#define PICCE_MKB3_ERR      0x0000001c
#define EXT_ST_COD_ERR      0x0000001d
#define USR_DAT_MKB_ERR     0x0000001e
#define EXT_DATA_IDX_ERR    0x0000001f
#define BAR_SHIFTER_ERR     0x00000020

#define NO_START_C_ERR1     0x00000101
#define NO_START_C_ERR2     0x00000102
#define NO_START_C_ERR3     0x00000103
#define NO_START_C_ERR4     0x00000104
#define NO_START_C_ERR5     0x00000105
#define NO_START_C_ERR6     0x00000106
#define NO_START_C_ERR7     0x00000107
#define NO_START_C_ERR8     0x00000108
#define NO_START_C_ERR9     0x00000109
#define NO_START_C_ERR10    0x00000110
#define NO_START_C_ERR11    0x00000111
#define NO_START_C_ERR12    0x00000112
#define NO_START_C_ERR13    0x00000113
#define NO_START_C_ERR14    0x00000114
#define NO_START_C_ERR15    0x00000115
#define NO_START_C_ERR16    0x00000116

#define VLD_NO_START_CODE   0x00000201
#define NO_START_CODE       0x00000202
#define NO_EXT_START_CODE   0x00000203
#define SLICE_IDX_ERROR     0x00000204

#define FB_ID_UNKNOWN       0x00000300
#define NO_DECODE           0x00000301
#define NO_DECODE_MPEG2     0x00000302
#define NO_DECODE_MPEG1     0x00000303

#define NEED_EXTRACT_CC     0x00009000
#define P_REFERNCE_INVALID  0x00009001
#define B_REFERNCE_INVALID  0x00009002
#define FIELD_ERR           0x00009004
#define REFERNCE_MISSING    0x00009005
#define FBG_ID_UNKNOWN      0x00009006
#define B_WAIT_INTRA_PIC    0x00009007
#define POINTER_NOT_AVAILABLE       0x00009008
#define GET_MM_FIRST_SEQ_HEADER     0x00009009


// Decode error 
#define VDEC_SUCCESS        0x0 
#define VDEC_ERR            0x1
#define VDEC_DTS_TIMEOUT    0x2
#define VDEC_TIMEOUT        0x3


#endif// MPV_ERRCODE_H


