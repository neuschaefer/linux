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
 * $RCSfile: x_sm_ttx_common.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         
 *         
 *---------------------------------------------------------------------------*/

#ifndef _X_SM_TTX_COMMON_H_
#define _X_SM_TTX_COMMON_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_sm_ttx_common.h"
#include "x_hamming_code.h"
#include "x_sm_ttx_common_ex.h"
#include "u_gl.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/


/****************************************************************************** 
    Page render information 
******************************************************************************/

/* 
    Color lookup table 
*/

/* RGB444 color: R(bits 8~11), G(bits 4~7), B(bits 0~3) */
typedef UINT16  SM_TTX_WGL_CLR_RGB444_T;
/* index: bits 3~4, entry: bits 0~2 */
typedef UINT8   SM_TTX_WGL_CLR_IDX_T;

#define SM_TTX_WGL_CLUTS_TBL_NUM            4
#define SM_TTX_WGL_CLUT_ENTRY_NUM           8
typedef struct _SM_TTX_WGL_CLUTS_T
{
    /* 4 CLUTs, each has 8 entries, indexed by SM_TTX_WGL_CLR_IDX_T */
    SM_TTX_WGL_CLR_RGB444_T        at_clrs[SM_TTX_WGL_CLUTS_TBL_NUM * SM_TTX_WGL_CLUT_ENTRY_NUM];
    
} SM_TTX_WGL_CLUTS_T;    

/*
    DRCS
*/

#define SM_TTX_WGL_DRCS_MAX_PTU_NUM            384

/* DRCS mode */
typedef enum _SM_TTX_WGL_DRCS_MODE_T
{
    SM_TTX_WGL_DRCS_MODE_12x10x1   = 0,
    SM_TTX_WGL_DRCS_MODE_12x10x2,
    SM_TTX_WGL_DRCS_MODE_12x10x4,
    SM_TTX_WGL_DRCS_MODE_6x5x4
} SM_TTX_WGL_DRCS_MODE_T;

/* PTU structure */
typedef struct _SM_TTX_WGL_DRCS_PTU_T
{
    GL_COLOR_T             t_fg_clr;
    GL_COLOR_T             t_bg_clr;
    UINT8                       aui1_ptu[20];
} SM_TTX_WGL_DRCS_PTU_T;


/* DRCS info */
typedef struct _SM_TTX_WGL_DRCS_INFO_T
{
    SM_TTX_WGL_CLR_IDX_T            at_dclut4_global[4];
    SM_TTX_WGL_CLR_IDX_T            at_dclut4_normal[4];
    SM_TTX_WGL_CLR_IDX_T            at_dclut16_global[16];
    SM_TTX_WGL_CLR_IDX_T            at_dclut16_normal[16];
    
    UINT16                          ui2_num_ptu;    
    SM_TTX_WGL_DRCS_PTU_T*          pat_ptus;       /* max: SM_TTX_WGL_DRCS_MAX_PTU_NUM */
    UINT8*                          paui1_modes;    /* max: (SM_TTX_WGL_DRCS_MAX_PTU_NUM * 2 + 7) / 8 */
} SM_TTX_WGL_DRCS_INFO_T;

/* 
    Cell Information 
*/

/* Cell structure */
typedef struct _SM_TTX_WGL_CELL_T
{
    UINT32                      ui4_dp1;
    UINT32                      ui4_dp2;
#ifdef TTX_L25_ENHANCEMENT
    UINT32                      ui4_dp3;
#endif
} SM_TTX_WGL_CELL_T;    

#ifdef TTX_L25_ENHANCEMENT
#define TTX_DEFAULT_CELL    { 0x00200000, 0x00000000, 0x00000000}
#else
#define TTX_DEFAULT_CELL    { 0x00200000, 0x00000000}
#endif

#define SM_TTX_WGL_CELL_COMPARE(pt_cell_1, pt_cell_2)   \
    ((pt_cell_1->ui4_dp1 == pt_cell_2->ui4_dp1) &&        \
     ((pt_cell_1->ui4_dp2 & 0x0007FFFF) == (pt_cell_2->ui4_dp2 & 0x0007FFFF)) )

/* General Set / Get / Has/ Enable / Clear function */
#define SM_TTX_WGL_CELL_SET_B1(pt_cell, data, mask, idx)   \
    pt_cell->ui4_dp1 = ((pt_cell->ui4_dp1 & (~((UINT32) mask))) | ((((UINT32)data) << idx) & ((UINT32) mask)))
#define SM_TTX_WGL_CELL_GET_B1(pt_cell, mask, idx)   \
    (((pt_cell->ui4_dp1) & ((UINT32) mask)) >> idx)
#define SM_TTX_WGL_CELL_HAS_B1(pt_cell, mask)   \
    (0 != ((pt_cell->ui4_dp1) & ((UINT32) mask)))
#define SM_TTX_WGL_CELL_SET_FLAG_B1(pt_cell, mask)   \
    pt_cell->ui4_dp1 = pt_cell->ui4_dp1 | ((UINT32) mask)
#define SM_TTX_WGL_CELL_CLR_B1(pt_cell, mask)   \
    pt_cell->ui4_dp1 = pt_cell->ui4_dp1 & ~((UINT32) mask)

#define SM_TTX_WGL_CELL_SET_B2(pt_cell, data, mask, idx)   \
    pt_cell->ui4_dp2 = ((pt_cell->ui4_dp2 & (~((UINT32) mask))) | ((((UINT32)data) << idx) & ((UINT32) mask)))
#define SM_TTX_WGL_CELL_GET_B2(pt_cell, mask, idx)   \
    (((pt_cell->ui4_dp2) & ((UINT32) mask)) >> idx)
#define SM_TTX_WGL_CELL_HAS_B2(pt_cell, mask)   \
    (0 != ((pt_cell->ui4_dp2) & ((UINT32) mask)))
#define SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, mask)   \
    pt_cell->ui4_dp2 = pt_cell->ui4_dp2 | ((UINT32) mask)
#define SM_TTX_WGL_CELL_CLR_B2(pt_cell, mask)   \
    pt_cell->ui4_dp2 = pt_cell->ui4_dp2 & ~((UINT32) mask)

#ifdef TTX_L25_ENHANCEMENT
#define SM_TTX_WGL_CELL_SET_B3(pt_cell, data, mask, idx)   \
    pt_cell->ui4_dp3 = ((pt_cell->ui4_dp3 & (~((UINT32) mask))) | ((((UINT32)data) << idx) & ((UINT32) mask)))
#define SM_TTX_WGL_CELL_GET_B3(pt_cell, mask, idx)   \
    (((pt_cell->ui4_dp3) & ((UINT32) mask)) >> idx)
#define SM_TTX_WGL_CELL_HAS_B3(pt_cell, mask)   \
    (0 != ((pt_cell->ui4_dp3) & ((UINT32) mask)))
#define SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, mask)   \
    pt_cell->ui4_dp3 = pt_cell->ui4_dp3 | ((UINT32) mask)
#define SM_TTX_WGL_CELL_CLR_B3(pt_cell, mask)   \
    pt_cell->ui4_dp3= pt_cell->ui4_dp3 & ~((UINT32) mask)
#endif


/* Character set group */
#define SM_TTX_WGL_CELL_CHSG_G0                 0x00
#define SM_TTX_WGL_CELL_CHSG_G2                 0x01
#define SM_TTX_WGL_CELL_CHSG_G1G3               0x02
#define SM_TTX_WGL_CELL_CHSG_DRCS               0x03
#define SM_TTX_WGL_CELL_SET_CHSG(pt_cell, data)    \
    SM_TTX_WGL_CELL_SET_B1(pt_cell, data, 0x00000003, 0)
#define SM_TTX_WGL_CELL_GET_CHSG(pt_cell)   \
    SM_TTX_WGL_CELL_GET_B1(pt_cell, 0x00000003, 0)

#define SM_TTX_WGL_CELL_IS_DRCS(pt_cell) \
    (SM_TTX_WGL_CELL_CHSG_DRCS == SM_TTX_WGL_CELL_GET_CHSG(pt_cell))
/* G0 Source */
#define SM_TTX_WGL_CELL_G0G2_SRC_DEFAULT          0x00
#define SM_TTX_WGL_CELL_G0G2_SRC_SECOND           0x01
#define SM_TTX_WGL_CELL_G0G2_SRC_MODIFIED         0x02
#define SM_TTX_WGL_CELL_SET_G0_SRC(pt_cell, data)    \
    SM_TTX_WGL_CELL_SET_B1(pt_cell, data, 0x0000000C, 2)
#define SM_TTX_WGL_CELL_GET_G0_SRC(pt_cell) \
    SM_TTX_WGL_CELL_GET_B1(pt_cell, 0x0000000C, 2)

/* G0 Lang Code */
#define SM_TTX_WGL_CELL_SET_G0_LANG_CODE(pt_cell, data)    \
    SM_TTX_WGL_CELL_SET_B1(pt_cell, data, 0x000007F0, 4)
#define SM_TTX_WGL_CELL_GET_G0_LANG_CODE(pt_cell) \
    SM_TTX_WGL_CELL_GET_B1(pt_cell, 0x000007F0, 4)

/* G0 from packet X26 */
#define SM_TTX_WGL_CELL_HAS_G0_FROM_X26(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B1(pt_cell, 0x00000800)
#define SM_TTX_WGL_CELL_SET_G0_FROM_X26(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B1(pt_cell, 0x00000800)
#define SM_TTX_WGL_CELL_CLR_G0_FROM_X26(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B1(pt_cell, 0x00000800)

/* G0 Diacritical Mark */
#define SM_TTX_WGL_CELL_SET_G0_DCTL_MARK(pt_cell, data)    \
    SM_TTX_WGL_CELL_SET_B1(pt_cell, data, 0x0000F000, 12)
#define SM_TTX_WGL_CELL_GET_G0_DCTL_MARK(pt_cell) \
    SM_TTX_WGL_CELL_GET_B1(pt_cell, 0x0000F000, 12)

/* G2 Source */
#define SM_TTX_WGL_CELL_SET_G2_SRC(pt_cell, data)    \
    SM_TTX_WGL_CELL_SET_B1(pt_cell, data, 0x0000000C, 2)
#define SM_TTX_WGL_CELL_GET_G2_SRC(pt_cell) \
    SM_TTX_WGL_CELL_GET_B1(pt_cell, 0x0000000C, 2)

/* G2 Lang Code */
#define SM_TTX_WGL_CELL_SET_G2_LANG_CODE(pt_cell, data)    \
    SM_TTX_WGL_CELL_SET_B1(pt_cell, data, 0x000007F0, 4)
#define SM_TTX_WGL_CELL_GET_G2_LANG_CODE(pt_cell) \
    SM_TTX_WGL_CELL_GET_B1(pt_cell, 0x000007F0, 4)

/* Is G1 (Otherwise G3) */
#define SM_TTX_WGL_CELL_HAS_IS_G1(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B1(pt_cell, 0x00000004)
#define SM_TTX_WGL_CELL_SET_IS_G1(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B1(pt_cell, 0x00000004)
#define SM_TTX_WGL_CELL_CLR_IS_G1(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B1(pt_cell, 0x00000004)

/* DRCS ID */
#define SM_TTX_WGL_CELL_SET_DRCS_ID(pt_cell, data)    \
    SM_TTX_WGL_CELL_SET_B1(pt_cell, data, 0x000007FC, 2)
#define SM_TTX_WGL_CELL_GET_DRCS_ID(pt_cell) \
    SM_TTX_WGL_CELL_GET_B1(pt_cell, 0x000007FC, 2)

/* Global DRCS */
#define SM_TTX_WGL_CELL_HAS_DRCS_GLOBAL(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B1(pt_cell, 0x00000800)
#define SM_TTX_WGL_CELL_SET_DRCS_GLOBAL(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B1(pt_cell, 0x00000800)
#define SM_TTX_WGL_CELL_CLR_DRCS_GLOBAL(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B1(pt_cell, 0x00000800)


/* Character Code */
#define SM_TTX_WGL_CELL_SET_CHC(pt_cell, data)    \
    SM_TTX_WGL_CELL_SET_B1(pt_cell, data, 0x007F0000, 16)
#define SM_TTX_WGL_CELL_GET_CHC(pt_cell) \
    SM_TTX_WGL_CELL_GET_B1(pt_cell, 0x007F0000, 16)

/* Proportional Space */
#define SM_TTX_WGL_CELL_HAS_PPS(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B1(pt_cell, 0x00800000)
#define SM_TTX_WGL_CELL_SET_PPS(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B1(pt_cell, 0x00800000)
#define SM_TTX_WGL_CELL_CLR_PPS(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B1(pt_cell, 0x00800000)

/* Bold */
#define SM_TTX_WGL_CELL_HAS_BOLD(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B1(pt_cell, 0x01000000)
#define SM_TTX_WGL_CELL_SET_BOLD(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B1(pt_cell, 0x01000000)
#define SM_TTX_WGL_CELL_CLR_BOLD(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B1(pt_cell, 0x01000000)

/* Italic */
#define SM_TTX_WGL_CELL_HAS_ITALIC(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B1(pt_cell, 0x02000000)
#define SM_TTX_WGL_CELL_SET_ITALIC(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B1(pt_cell, 0x02000000)
#define SM_TTX_WGL_CELL_CLR_ITALIC(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B1(pt_cell, 0x02000000)

/* Separated */
#define SM_TTX_WGL_CELL_HAS_SPRT_L1(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B1(pt_cell, 0x04000000)
#define SM_TTX_WGL_CELL_SET_SPRT_L1(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B1(pt_cell, 0x04000000)
#define SM_TTX_WGL_CELL_CLR_SPRT_L1(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B1(pt_cell, 0x04000000)

/* FLASH */
#define SM_TTX_WGL_CELL_SET_FLASH(pt_cell, data)    \
    SM_TTX_WGL_CELL_SET_B1(pt_cell, data, 0xF8000000, 27)
#define SM_TTX_WGL_CELL_GET_FLASH(pt_cell) \
    SM_TTX_WGL_CELL_GET_B1(pt_cell, 0xF8000000, 27)

/* box, window */
#define SM_TTX_WGL_CELL_HAS_BOX(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x00000001)
#define SM_TTX_WGL_CELL_SET_BOX(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x00000001)
#define SM_TTX_WGL_CELL_CLR_BOX(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x00000001)

/* Double Width */
#define SM_TTX_WGL_CELL_HAS_DW(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x00000002)
#define SM_TTX_WGL_CELL_SET_DW(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x00000002)
#define SM_TTX_WGL_CELL_CLR_DW(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x00000002)

/* Double Height */
#define SM_TTX_WGL_CELL_HAS_DH(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x00000004)
#define SM_TTX_WGL_CELL_SET_DH(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x00000004)
#define SM_TTX_WGL_CELL_CLR_DH(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x00000004)

/* Below content defines upper or lower of double height macros.*/
/* 0 for upper, 1 for lower */
#define SM_TTX_WGL_CELL_HAS_DH_LOWER(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x00000008)
#define SM_TTX_WGL_CELL_SET_DH_LOWER(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x00000008)
#define SM_TTX_WGL_CELL_CLR_DH_LOWER(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x00000008)

/* Below content defines left or right of double width macros.*/
/* 0 for left, 1 for right */
#define SM_TTX_WGL_CELL_HAS_DW_RIGHT(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x00000010)
#define SM_TTX_WGL_CELL_SET_DW_RIGHT(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x00000010)
#define SM_TTX_WGL_CELL_CLR_DW_RIGHT(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x00000010)

/* char_grid_id */
#define SM_TTX_WGL_CELL_SET_CHR_GRID_ID(pt_cell, data)    \
    SM_TTX_WGL_CELL_SET_B2(pt_cell, data, 0x000000F8, 3)
#define SM_TTX_WGL_CELL_GET_CHR_GRID_ID(pt_cell) \
    SM_TTX_WGL_CELL_GET_B2(pt_cell, 0x000000F8, 3)

/* 4-bit Size Config, DW, DH, TB and LR */
#define SM_TTX_WGL_CELL_SET_SIZE_CFG(pt_cell, data) \
    SM_TTX_WGL_CELL_SET_B2(pt_cell, data, 0x0000001E, 1)
#define SM_TTX_WGL_CELL_GET_SIZE_CFG(pt_cell) \
    SM_TTX_WGL_CELL_GET_B2(pt_cell, 0x0000001E, 1)

/* Foreground color */
#define SM_TTX_WGL_CELL_SET_FG_CLR(pt_cell, data)    \
    SM_TTX_WGL_CELL_SET_B2(pt_cell, data, 0x00001F00, 8)
#define SM_TTX_WGL_CELL_GET_FG_CLR(pt_cell) \
    SM_TTX_WGL_CELL_GET_B2(pt_cell, 0x00001F00, 8)

/* Background color */
#define SM_TTX_WGL_CELL_SET_BG_CLR(pt_cell, data)    \
    SM_TTX_WGL_CELL_SET_B2(pt_cell, data, 0x0003E000, 13)
#define SM_TTX_WGL_CELL_GET_BG_CLR(pt_cell) \
    SM_TTX_WGL_CELL_GET_B2(pt_cell, 0x0003E000, 13)

/* Conceal */
#define SM_TTX_WGL_CELL_HAS_CNCL(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x00040000)
#define SM_TTX_WGL_CELL_SET_CNCL(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x00040000)
#define SM_TTX_WGL_CELL_CLR_CNCL(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x00040000)

#ifdef TTX_L25_ENHANCEMENT
/* FG color info */
#define SM_TTX_WGL_CELL_HAS_FGC_INFO(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00000001)
#define SM_TTX_WGL_CELL_SET_FGC_INFO(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00000001)
#define SM_TTX_WGL_CELL_CLR_FGC_INFO(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00000001)

/* BG color info */
#define SM_TTX_WGL_CELL_HAS_BGC_INFO(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00000002)
#define SM_TTX_WGL_CELL_SET_BGC_INFO(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00000002)
#define SM_TTX_WGL_CELL_CLR_BGC_INFO(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00000002)

/* conceal info */
#define SM_TTX_WGL_CELL_HAS_CNCL_INFO(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00000004)
#define SM_TTX_WGL_CELL_SET_CNCL_INFO(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00000004)
#define SM_TTX_WGL_CELL_CLR_CNCL_INFO(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00000004)

/* Invert colour info */
#define SM_TTX_WGL_CELL_HAS_INVERTCLR_INFO(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00000008)
#define SM_TTX_WGL_CELL_SET_INVERTCLR_INFO(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00000008)
#define SM_TTX_WGL_CELL_CLR_INVERTCLR_INFO(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00000008)

/* underline or seperate mosaic info */
#define SM_TTX_WGL_CELL_HAS_UDL_SPR_L25(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00000010)
#define SM_TTX_WGL_CELL_SET_UDL_SPR_L25(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00000010)
#define SM_TTX_WGL_CELL_CLR_UDL_SPR_L25(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00000010)

/* double width info */
#define SM_TTX_WGL_CELL_HAS_DW_INFO(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00000020)
#define SM_TTX_WGL_CELL_SET_DW_INFO(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00000020)
#define SM_TTX_WGL_CELL_CLR_DW_INFO(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00000020)

/* double height info */
#define SM_TTX_WGL_CELL_HAS_DH_INFO(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00000040)
#define SM_TTX_WGL_CELL_SET_DH_INFO(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00000040)
#define SM_TTX_WGL_CELL_CLR_DH_INFO(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00000040)

/* box/windows info */
#define SM_TTX_WGL_CELL_HAS_BOX_INFO(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00000080)
#define SM_TTX_WGL_CELL_SET_BOX_INFO(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00000080)
#define SM_TTX_WGL_CELL_CLR_BOX_INFO(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00000080)

/* char in passive objs */
#define SM_TTX_WGL_CELL_IS_PASSIVE(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00000100)
#define SM_TTX_WGL_CELL_SET_PASSIVE(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00000100)
#define SM_TTX_WGL_CELL_CLR_PASSIVE(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00000100)

/* char in adaptive objs */
#define SM_TTX_WGL_CELL_IS_ADAPTIVE_CHAR(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00000200)
#define SM_TTX_WGL_CELL_SET_ADAPTIVE_CHAR(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00000200)
#define SM_TTX_WGL_CELL_CLR_ADAPTIVE_CHAR(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00000200)

/* char in local objs */
#define SM_TTX_WGL_CELL_IS_LOCAL_CHAR(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00000400)
#define SM_TTX_WGL_CELL_SET_LOCAL_CHAR(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00000400)
#define SM_TTX_WGL_CELL_CLR_LOCAL_CHAR(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00000400)

/* char in active objs */
#define SM_TTX_WGL_CELL_IS_ACTIVE_CHAR(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00000800)
#define SM_TTX_WGL_CELL_SET_ACTIVE_CHAR(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00000800)
#define SM_TTX_WGL_CELL_CLR_ACTIVE_CHAR(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00000800)

/* L1 black bg */
#define SM_TTX_WGL_CELL_HAS_L1_BLK_BG(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00001000)
#define SM_TTX_WGL_CELL_SET_L1_BLK_BG(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00001000)
#define SM_TTX_WGL_CELL_CLR_L1_BLK_BG(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00001000)

/* additional flash info */
#define SM_TTX_WGL_CELL_HAS_FLASH_INFO(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00002000)
#define SM_TTX_WGL_CELL_SET_FLASH_INFO(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00002000)
#define SM_TTX_WGL_CELL_CLR_FLASH_INFO(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00002000)

/* L1 DH upper */
#define SM_TTX_WGL_CELL_HAS_L1_DH_UPPER(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00004000)
#define SM_TTX_WGL_CELL_SET_L1_DH_UPPER(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00004000)
#define SM_TTX_WGL_CELL_CLR_L1_DH_UPPER(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00004000)

/* L1 DH lower */
#define SM_TTX_WGL_CELL_HAS_L1_DH_LOWER(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B3(pt_cell, 0x00008000)
#define SM_TTX_WGL_CELL_SET_L1_DH_LOWER(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B3(pt_cell, 0x00008000)
#define SM_TTX_WGL_CELL_CLR_L1_DH_LOWER(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B3(pt_cell, 0x00008000)
#endif
/*
    Main Panel Row Structure
*/
#define SM_TTX_WGL_MAIN_PNL_ROW_FLAG_UPDATE_MASK                0x03
#define SM_TTX_WGL_MAIN_PNL_ROW_FLAG_NO_UPDATE                  0x00
#define SM_TTX_WGL_MAIN_PNL_ROW_FLAG_FULL_UPDATE                0x01
#define SM_TTX_WGL_MAIN_PNL_ROW_FLAG_PARTIAL_UPDATE             0x02

#define SM_TTX_WGL_MAIN_PNL_ROW_FLAG_ROW_CLR_AVAIL              0x04
#define SM_TTX_WGL_MAIN_PNL_ROW_FLAG_CELL_NOT_AVAIL             0x08

#define SM_TTX_WGL_MAIN_PNL_ROW_FLAG_USED_BITS_MASK      \
    (SM_TTX_WGL_MAIN_PNL_ROW_FLAG_UPDATE_MASK | \
     SM_TTX_WGL_MAIN_PNL_ROW_FLAG_ROW_CLR_AVAIL | \
     SM_TTX_WGL_MAIN_PNL_ROW_FLAG_CELL_NOT_AVAIL )

typedef struct _SM_TTX_WGL_MAIN_PNL_ROW_DATA_T
{
    UINT8                           ui1_flag;
    UINT8                           ui1_row_id;
    SM_TTX_WGL_CLR_IDX_T            t_clri_row;
    SM_TTX_WGL_CELL_T               at_cells[SM_TTX_WGL_MAIN_PNL_COL_NUM];
} SM_TTX_WGL_MAIN_PNL_ROW_DATA_T;    

/*
    Header Panel Structure
*/
#define SM_TTX_WGL_HDR_ROW_FLAG_UPDATE_MASK             SM_TTX_WGL_MAIN_PNL_ROW_FLAG_UPDATE_MASK
#define SM_TTX_WGL_HDR_ROW_FLAG_NO_UPDATE               SM_TTX_WGL_MAIN_PNL_ROW_FLAG_NO_UPDATE
#define SM_TTX_WGL_HDR_ROW_FLAG_FULL_UPDATE             SM_TTX_WGL_MAIN_PNL_ROW_FLAG_FULL_UPDATE
#define SM_TTX_WGL_HDR_ROW_FLAG_PARTIAL_UPDATE          SM_TTX_WGL_MAIN_PNL_ROW_FLAG_PARTIAL_UPDATE

#define SM_TTX_WGL_HDR_ROW_FLAG_ROW_CLR_AVAIL           SM_TTX_WGL_MAIN_PNL_ROW_FLAG_ROW_CLR_AVAIL
#define SM_TTX_WGL_HDR_ROW_FLAG_ROW_CELL_NOT_AVAIL      SM_TTX_WGL_MAIN_PNL_ROW_FLAG_CELL_NOT_AVAIL

#define SM_TTX_WGL_HDR_ROW_FLAG_USED_BITS_MASK      \
    (SM_TTX_WGL_HDR_ROW_FLAG_UPDATE_MASK | \
     SM_TTX_WGL_HDR_ROW_FLAG_ROW_CLR_AVAIL | \
     SM_TTX_WGL_HDR_ROW_FLAG_ROW_CELL_NOT_AVAIL )

typedef SM_TTX_WGL_MAIN_PNL_ROW_DATA_T                  SM_TTX_WGL_HDR_ROW_DATA_T;

typedef struct _SM_TTX_WGL_HDR_PNL_T
{
    SM_TTX_WGL_HDR_ROW_DATA_T   *pt_hdr_row;   
} SM_TTX_WGL_HDR_PNL_T;

/*
    Side Panel Structure
*/
#define SM_TTX_WGL_SIDE_PNL_MAX_ROW_NUM                       26  /* Side Panel contains row 0 ~ 25 */

#define SM_TTX_WGL_SIDE_PNL_ROW_FLAG_UPDATE_MASK                0x03
#define SM_TTX_WGL_SIDE_PNL_ROW_FLAG_NO_UPDATE                  0x00
#define SM_TTX_WGL_SIDE_PNL_ROW_FLAG_FULL_UPDATE                0x01
#define SM_TTX_WGL_SIDE_PNL_ROW_FLAG_PARTIAL_UPDATE             0x02

#define SM_TTX_WGL_SIDE_PNL_ROW_FLAG_ROW_CLR_AVAIL              0x04
#define SM_TTX_WGL_SIDE_PNL_ROW_FLAG_ROW_CELL_NOT_AVAIL         0x08

#define SM_TTX_WGL_SIDE_PNL_ROW_FLAG_USED_BITS_MASK      \
    (SM_TTX_WGL_SIDE_PNL_ROW_FLAG_UPDATE_MASK | \
     SM_TTX_WGL_SIDE_PNL_ROW_FLAG_ROW_CLR_AVAIL | \
     SM_TTX_WGL_SIDE_PNL_ROW_FLAG_ROW_CELL_NOT_AVAIL )

typedef struct _SM_TTX_WGL_SIDE_PNL_ROW_DATA_T
{
    UINT8                                   ui1_flag;
    UINT8                                   ui1_row_id;
    SM_TTX_WGL_CLR_IDX_T                    t_clri_row;
    SM_TTX_WGL_CELL_T                       at_cells[SM_TTX_WGL_SIDE_PNL_MAX_COL_NUM];
} SM_TTX_WGL_SIDE_PNL_ROW_DATA_T;

typedef struct _SM_TTX_WGL_SIDE_PNL_T
{
#ifdef TTX_L25_ENHANCEMENT
    BOOL b_left;
    BOOL b_right;
#endif
    UINT8 ui1_col_num_left;
    UINT8 ui1_eff_row_num;
    SM_TTX_WGL_SIDE_PNL_ROW_DATA_T*         pat_rows; /* Max row number is SM_TTX_WGL_SIDE_PNL_MAX_ROW_NUM */
} SM_TTX_WGL_SIDE_PNL_T;


/*
    Channel Decoder
*/
#define ttx_msb_hamming_84_decoding                 x_msb_hamming_84_decoding
#define ttx_msb_hamming_2418_decoding_fmt1          x_msb_hamming_2418_decoding_fmt1
#define ttx_msb_odd_parity_check                    x_msb_odd_parity_check

#define ttx_lsb_hamming_84_decoding                 x_lsb_hamming_84_decoding
#define ttx_lsb_hamming_2418_decoding_fmt1          x_lsb_hamming_2418_decoding_fmt1
#define ttx_lsb_odd_parity_check                    x_lsb_odd_parity_check

#define ttx_inv_msb_hamming_84_decoding             x_inv_msb_hamming_84_decoding
#define ttx_inv_msb_hamming_2418_decoding_fmt1      x_inv_msb_hamming_2418_decoding_fmt1
#define ttx_inv_msb_odd_parity_check                x_inv_msb_odd_parity_check

#define ttx_inv_lsb_hamming_84_decoding             x_inv_lsb_hamming_84_decoding
#define ttx_inv_lsb_hamming_2418_decoding_fmt1      x_inv_lsb_hamming_2418_decoding_fmt1
#define ttx_inv_lsb_odd_parity_check                x_inv_lsb_odd_parity_check

#endif /* _X_SM_TTX_COMMON_H_ */

