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
 * $RCSfile: gfx_drvif.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx_drvif.c
 *  internal public interfaces of gfx driver
 *
 */

/*
    lint inhibition(s)
        506 - Constant value Boolean [MTK Rule 6.1.3]
        534 - Ignoring return value of function
        774 - Boolean within 'right side of && within if' always
              evaluates to True [Reference: file gfx_if.c: lines 919, 935]
        826 - Suspicious pointer-to-pointer conversion (area too small)
        831 - Loss of sign (assignment) (signed char to unsigned long)
*/


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "gfx_if.h"
#include "gfx_drvif.h"
#include "gfx_dif.h"
#include "gfx_hw.h"
#include "gfx_cmdque.h"
#include "gfx_debug.h"
#include "fbm_drvif.h"
#include "vdp_if.h"
#include "vgfx_drvif.h"
#ifdef CC_FLIP_MIRROR_SUPPORT
#include "eeprom_if.h"
#include "drv_display.h"
#endif

#if defined(CC_MT5368)||defined(CC_MT5396) || defined(CC_MT5389)||defined(CC_MT5398)||defined(CC_MT5880)
// acronym of gfx register name in register file
#define VGREG(XREG)       _prVgfxRegFile->rField.XREG

// push back relative register value in register file.
#if defined(GFX_RISC_MODE)  // risc mode
#define VGFX_CMDENQ(XREG, INDEX)        \
    (VGFX_RiscPushBack((UINT32)(XREG), _au4VgfxRegFile[(UINT32)(INDEX)]))
#else                       // cmdque mode
#define VGFX_CMDENQ(XREG, INDEX)        \
    (VGFX_CmdQuePushBack((UINT32)(XREG), _au4VgfxRegFile[(UINT32)(INDEX)]))

#endif // #if defined(GFX_RISC_MODE)


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------


static UINT32 _au4VgfxRegFile[VGREG_LAST_NUM];


/*lint -save -e826 */
static MI_DIF_VGFX_UNION_T *_prVgfxRegFile = (MI_DIF_VGFX_UNION_T *)_au4VgfxRegFile;
/*lint -restore */
static volatile UINT32* _prHwVgfxReg =
(UINT32 *)VGFX_ADDR;

//-------------------------------------------------------------------------
/** _GFX_SetColCnvSrc
 *  set color conversion source.
 *  luma input address, line size.
 *  chroma input address, linesize.
 *  field/frame mode.
 */
//-------------------------------------------------------------------------

UINT32  _VGFX_index2Reg(UINT32 u4Idx)
{
    switch(u4Idx)
    {
       case  VGREG_G_CONFIG_NUM:    // 0x5c000
        return VGREG_G_CONFIG;
        case VGREG_G_STATUS_NUM:     // 0x5c004
            return VGREG_G_STATUS;
        case VGREG_0x5c008_NUM:     // 0x5c008
            return VGREG_0x5c008;
        case VGREG_0x5c00c_NUM:     // 0x5c00C
            return VGREG_0x5c00c;
        case VGREG_WB_MODE_NUM:     // 0x5c010
            return VGREG_WB_MODE;
        case VGREG_IMG_BSAD_NUM:    // 0x5c020
            return VGREG_IMG_BSAD;
        case VGREG_TXT_START_NUM:     // 0x5c024
            return VGREG_TXT_START;
        case VGREG_TXT_END_NUM:     // 0x5c028
            return VGREG_TXT_END;
        case VGREG_IMG_PITCH_NUM:     // 0x5c02C
            return VGREG_IMG_PITCH;
        case VGREG_IMG_CONFIG_NUM:     // 0x5c030
            return VGREG_IMG_CONFIG;
        case VGREG_ALPHA_SHRINK_NUM:     // 0x5c040
            return VGREG_ALPHA_SHRINK;
        case VGREG_WB_BSAD_NUM:     //0x5c100
            return VGREG_WB_BSAD;
        case VGREG_BB_START_NUM:    //0x5c104
            return VGREG_BB_START;
        case VGREG_BB_SIZE_NUM: //0x5c108
            return VGREG_BB_SIZE;
        case VGREG_WB_PITCH_NUM:    //0x5c10c
            return VGREG_WB_PITCH;
        case VGREG_BB_BASED_NUM:    //0x5c110
            return VGREG_BB_BASED;
        case VGREG_WB_EXP_NUM:      //0x5c114
            return VGREG_WB_EXP;
        case VGREG_COEF_A_NUM:  //0x5c200
            return  VGREG_COEF_A;
        case VGREG_COEF_B_NUM:  //0x5c204
            return VGREG_COEF_B;
        case VGREG_COEF_C_NUM:  //0x5c208
            return VGREG_COEF_C;
        case VGREG_COEF_D_NUM:  //0x5c20c
            return VGREG_COEF_D;
        case VGREG_COEF_E_NUM:  //0x5c210
            return VGREG_COEF_E;
        case VGREG_COEF_F_NUM:  //0x5c214
            return VGREG_COEF_F;
        case VGREG_COEF_G_NUM:  //0x5c218
            return VGREG_COEF_G;
        case VGREG_COEF_H_NUM:  //0x5c21c
            return VGREG_COEF_H;
        case VGREG_COEF_I_NUM:  //0x5c220
            return VGREG_COEF_I;
        case VGREG_FILTER_CONFIG_NUM:       //0x5c300
            return VGREG_FILTER_CONFIG;
        #if defined(CC_MT5389) 
        case VGREG_REF_CONFIG_NUM:      //0x5c304
            return VGREG_REF_CONFIG;
        case VGREG_REFRATIO_CONFIG_NUM: //0x5c308
            return VGREG_REFRATIO_CONFIG;
        case VGREG_FILTER_FIFO_NUM: //0x5c30c
            return VGREG_FILTER_FIFO;
        #endif

        case VGREG_COEF_4_TAP_0_NUM:	//0x5c340
        case VGREG_COEF_4_TAP_1_NUM:	//0x5c344
        case VGREG_COEF_4_TAP_2_NUM:	//0x5c348
        case VGREG_COEF_4_TAP_3_NUM:	//0x5c34c
        case VGREG_COEF_4_TAP_4_NUM:	//0x5c350
        case VGREG_COEF_4_TAP_5_NUM:	//0x5c354
        case VGREG_COEF_4_TAP_6_NUM:	//0x5c358
        case VGREG_COEF_4_TAP_7_NUM:	//0x5c35c
        case VGREG_COEF_4_TAP_8_NUM:   //0x5c360
        case VGREG_COEF_4_TAP_9_NUM:   //0x5c364
        case VGREG_COEF_4_TAP_A_NUM:   //0x5c368
        case VGREG_COEF_4_TAP_B_NUM:   //0x5c36c
        case VGREG_COEF_4_TAP_C_NUM:   //0x5c370
        case VGREG_COEF_4_TAP_D_NUM:   //0x5c374
        case VGREG_COEF_4_TAP_E_NUM:   //0x5c378
        case VGREG_COEF_4_TAP_F_NUM:   //0x5c37c
            return (VGREG_COEF_4_TAP_0 + u4Idx - VGREG_COEF_4_TAP_0_NUM);
        case VGREG_ALCOM_LOOP_NUM:      //0x5c500
            return VGREG_ALCOM_LOOP;
        case VGREG_LEGAL_START_NUM:     //0x5c600
            return VGREG_LEGAL_START;
        case VGREG_LEGAL_END_NUM:       //0x5c604
            return VGREG_LEGAL_END;
        case VGREG_PCTCTL_FIFO_NUM:     //0x5608
            return VGREG_PCTCTL_FIFO;
        case VGREG_PCTCTL_END_NUM:      //0x560c
            return VGREG_PCTCTL_END;
        case VGREG_WB_RND_NUM:      //0x5c610
            return VGREG_WB_RND;
        case VGREG_PSTCTL_ARGB_NUM:         //0x5c614
            return VGREG_PSTCTL_ARGB;
       default:
        return 0xffff;
    }
}
void _VGFX_Init(void)
{
   UINT32 u4Idx = 0;
    GFX_UNUSED_RET(x_memset(_au4VgfxRegFile, 0, sizeof(_au4VgfxRegFile)));
     for (; u4Idx<VGREG_LAST_NUM; u4Idx++)
        {
            _au4VgfxRegFile[u4Idx]  = _prHwVgfxReg[_VGFX_index2Reg(u4Idx)];
        }
}



void _VGFX_SetWb(UINT32 u4BaseAddr, UINT32 u4ColrMode, UINT32 u4GlAr, UINT32 u4Pitch)
{
    GFX_CHK_64B_PITCH(u4BaseAddr);

    VGREG(vfg_WB_BSAD)      = (UINT32)((u4BaseAddr + 0x3f) & 0xFFFFFFC0);
    VGREG(vfg_WB_CM)     = (UINT32)u4ColrMode;
    VGREG(vfg_WB_GLOBAL_ALPHA)  = (UINT32)u4GlAr;
    VGREG(vfg_WB_PITCH) = (UINT32)((u4Pitch + 0x3f) & 0x1FC0);
    VGFX_CMDENQ(VGREG_WB_BSAD, VGREG_WB_BSAD_NUM);
    VGFX_CMDENQ(VGREG_WB_MODE, VGREG_WB_MODE_NUM);
    VGFX_CMDENQ(VGREG_WB_PITCH, VGREG_WB_PITCH_NUM);
}


void _VGFX_SetImgage(UINT32 u4BaseAddr, UINT32 u4ColrMode, UINT32 u4GlAr, UINT32 u4Pitch)
{
    GFX_CHK_PITCH(u4BaseAddr);

    VGREG(vfg_IMG_BSAD)      = (UINT32)((u4BaseAddr + 0xf) & 0xFFFFFFF0);
    VGREG(vfg_IMG_CM)     = (UINT32)u4ColrMode;
    VGREG(vfg_IMG_GL_ALPHA)  = (UINT32)u4GlAr;
    VGREG(vfg_IMG_PITCH) = (UINT32)u4Pitch;
    VGFX_CMDENQ(VGREG_IMG_BSAD, VGREG_IMG_BSAD_NUM);
    VGFX_CMDENQ(VGREG_IMG_PITCH, VGREG_IMG_PITCH_NUM);
    VGFX_CMDENQ(VGREG_IMG_CONFIG, VGREG_IMG_CONFIG_NUM);
}


void _VGFX_SetTexture(UINT32 u4StartX, UINT32 u4StartY, UINT32 u4EndX, UINT32 u4EndY)
{
    VGREG(vfg_TXT_HSTART)      = (UINT32)u4StartX;
    VGREG(vfg_TXT_VSTART)     = (UINT32)u4StartY;
    VGREG(vfg_TXT_HEND)  = (UINT32)u4EndX;
    VGREG(vfg_TXT_VEND) = (UINT32)u4EndY;
    VGFX_CMDENQ(VGREG_TXT_START, VGREG_TXT_START_NUM);
    VGFX_CMDENQ(VGREG_TXT_END, VGREG_TXT_END_NUM);
}

void _VGFX_SetBb(UINT32 u4StartX, UINT32 u4StartY, UINT32 u4Width, UINT32 u4Height)
{
    VGREG(vfg_BB_TL_X)      = (UINT32)u4StartX;
    VGREG(vfg_BB_TL_Y)     = (UINT32)u4StartY;
    VGREG(vfg_BB_WIDTH)  = (UINT32)u4Width;
    VGREG(vfg_BB_HEIGHT) = (UINT32)(u4Height - 1);
    VGFX_CMDENQ(VGREG_BB_START, VGREG_BB_START_NUM);
    VGFX_CMDENQ(VGREG_BB_SIZE, VGREG_BB_SIZE_NUM);
}

void _VGFX_SetMatrixCoef(VGFX_MATRIX_T* pt_matrix)
{
    VGREG(vfg_COEF_A)      = pt_matrix->u4_a;
    VGREG(vfg_COEF_B)      = pt_matrix->u4_b;
    VGREG(vfg_COEF_C)      = pt_matrix->u4_c;
    VGREG(vfg_COEF_D)      = pt_matrix->u4_d;
    VGREG(vfg_COEF_E)      = pt_matrix->u4_e;
    VGREG(vfg_COEF_F)      = pt_matrix->u4_f;
    VGREG(vfg_COEF_G)      = pt_matrix->u4_g;
    VGREG(vfg_COEF_H)      = pt_matrix->u4_h;
    VGREG(vfg_COEF_I)      = pt_matrix->u4_i;
    VGFX_CMDENQ(VGREG_COEF_A, VGREG_COEF_A_NUM);
    VGFX_CMDENQ(VGREG_COEF_B, VGREG_COEF_B_NUM);
    VGFX_CMDENQ(VGREG_COEF_C, VGREG_COEF_C_NUM);
    VGFX_CMDENQ(VGREG_COEF_D, VGREG_COEF_D_NUM);
    VGFX_CMDENQ(VGREG_COEF_E, VGREG_COEF_E_NUM);
    VGFX_CMDENQ(VGREG_COEF_F, VGREG_COEF_F_NUM);
    VGFX_CMDENQ(VGREG_COEF_G, VGREG_COEF_G_NUM);
    VGFX_CMDENQ(VGREG_COEF_H, VGREG_COEF_H_NUM);
    VGFX_CMDENQ(VGREG_COEF_I, VGREG_COEF_I_NUM);
}


void _VGFX_SetMatrixCoef_Emu(UINT32 u4MatrixA, UINT32 u4MatrixB, UINT32 u4MatrixC, UINT32 u4MatrixD,
                         UINT32 u4MatrixE, UINT32 u4MatrixF, UINT32 u4MatrixG, UINT32 u4MatrixH, UINT32 u4MatrixI)
{
    VGREG(vfg_COEF_A)      = (UINT32)u4MatrixA;
    VGREG(vfg_COEF_B)      = (UINT32)u4MatrixB;
    VGREG(vfg_COEF_C)      = (UINT32)u4MatrixC;
    VGREG(vfg_COEF_D)      = (UINT32)u4MatrixD;
    VGREG(vfg_COEF_E)      = (UINT32)u4MatrixE;
    VGREG(vfg_COEF_F)      = (UINT32)u4MatrixF;
    VGREG(vfg_COEF_G)      = (UINT32)u4MatrixG;
    VGREG(vfg_COEF_H)      = (UINT32)u4MatrixH;
    VGREG(vfg_COEF_I)      = (UINT32)u4MatrixI;
    VGFX_CMDENQ(VGREG_COEF_A, VGREG_COEF_A_NUM);
    VGFX_CMDENQ(VGREG_COEF_B, VGREG_COEF_B_NUM);
    VGFX_CMDENQ(VGREG_COEF_C, VGREG_COEF_C_NUM);
    VGFX_CMDENQ(VGREG_COEF_D, VGREG_COEF_D_NUM);
    VGFX_CMDENQ(VGREG_COEF_E, VGREG_COEF_E_NUM);
    VGFX_CMDENQ(VGREG_COEF_F, VGREG_COEF_F_NUM);
    VGFX_CMDENQ(VGREG_COEF_G, VGREG_COEF_G_NUM);
    VGFX_CMDENQ(VGREG_COEF_H, VGREG_COEF_H_NUM);
    VGFX_CMDENQ(VGREG_COEF_I, VGREG_COEF_I_NUM);
}



void _VGFX_SetCoef4Tap(VGFX_COEFF_4_TAP* pt_Coef4Tap)
{
    
    VGREG(vfg_COEFF_4_TAP_00)   = pt_Coef4Tap->u4Coef00;
    VGREG(vfg_COEFF_4_TAP_01)   = pt_Coef4Tap->u4Coef01;
    VGREG(vfg_COEFF_4_TAP_10)   = pt_Coef4Tap->u4Coef10;
    VGREG(vfg_COEFF_4_TAP_11)   = pt_Coef4Tap->u4Coef11;
    VGREG(vfg_COEFF_4_TAP_20)   = pt_Coef4Tap->u4Coef20;
    VGREG(vfg_COEFF_4_TAP_21)   = pt_Coef4Tap->u4Coef21;
    VGREG(vfg_COEFF_4_TAP_30)   = pt_Coef4Tap->u4Coef30;
    VGREG(vfg_COEFF_4_TAP_31)   = pt_Coef4Tap->u4Coef31;
    VGREG(vfg_COEFF_4_TAP_40)   = pt_Coef4Tap->u4Coef40;
    VGREG(vfg_COEFF_4_TAP_41)   = pt_Coef4Tap->u4Coef41;
    VGREG(vfg_COEFF_4_TAP_50)   = pt_Coef4Tap->u4Coef50;
    VGREG(vfg_COEFF_4_TAP_51)   = pt_Coef4Tap->u4Coef51;
    VGREG(vfg_COEFF_4_TAP_60)   = pt_Coef4Tap->u4Coef60;
    VGREG(vfg_COEFF_4_TAP_61)   = pt_Coef4Tap->u4Coef61;
    VGREG(vfg_COEFF_4_TAP_70)   = pt_Coef4Tap->u4Coef70;
    VGREG(vfg_COEFF_4_TAP_71)   = pt_Coef4Tap->u4Coef71;
    VGREG(vfg_COEFF_4_TAP_80)   = pt_Coef4Tap->u4Coef80;
    VGREG(vfg_COEFF_4_TAP_81)   = pt_Coef4Tap->u4Coef81;
    VGREG(vfg_COEFF_4_TAP_90)   = pt_Coef4Tap->u4Coef90;
    VGREG(vfg_COEFF_4_TAP_91)   = pt_Coef4Tap->u4Coef91;
    VGREG(vfg_COEFF_4_TAP_A0)   = pt_Coef4Tap->u4CoefA0;
    VGREG(vfg_COEFF_4_TAP_A1)   = pt_Coef4Tap->u4CoefA1;
    VGREG(vfg_COEFF_4_TAP_B0)   = pt_Coef4Tap->u4CoefB0;
    VGREG(vfg_COEFF_4_TAP_B1)   = pt_Coef4Tap->u4CoefB1;
    VGREG(vfg_COEFF_4_TAP_C0)   = pt_Coef4Tap->u4CoefC0;
    VGREG(vfg_COEFF_4_TAP_C1)   = pt_Coef4Tap->u4CoefC1;
    VGREG(vfg_COEFF_4_TAP_D0)   = pt_Coef4Tap->u4CoefD0;
    VGREG(vfg_COEFF_4_TAP_D1)   = pt_Coef4Tap->u4CoefD1;
    VGREG(vfg_COEFF_4_TAP_E0)   = pt_Coef4Tap->u4CoefE0;
    VGREG(vfg_COEFF_4_TAP_E1)   = pt_Coef4Tap->u4CoefE1;
    VGREG(vfg_COEFF_4_TAP_F0)   = pt_Coef4Tap->u4CoefF0;
    VGREG(vfg_COEFF_4_TAP_F1)   = pt_Coef4Tap->u4CoefF1;

    VGFX_CMDENQ(VGREG_COEF_4_TAP_0, VGREG_COEF_4_TAP_0_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_1, VGREG_COEF_4_TAP_1_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_2, VGREG_COEF_4_TAP_2_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_3, VGREG_COEF_4_TAP_3_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_4, VGREG_COEF_4_TAP_4_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_5, VGREG_COEF_4_TAP_5_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_6, VGREG_COEF_4_TAP_6_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_7, VGREG_COEF_4_TAP_7_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_8, VGREG_COEF_4_TAP_8_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_9, VGREG_COEF_4_TAP_9_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_A, VGREG_COEF_4_TAP_A_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_B, VGREG_COEF_4_TAP_B_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_C, VGREG_COEF_4_TAP_C_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_D, VGREG_COEF_4_TAP_D_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_E, VGREG_COEF_4_TAP_E_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_F, VGREG_COEF_4_TAP_F_NUM);
}

void _VGFX_ResetCoef4Tap(void)
{
    VGREG(vfg_COEFF_4_TAP_00)   = VG_COEFF_4_TAP_0_0;
    VGREG(vfg_COEFF_4_TAP_01)   = VG_COEFF_4_TAP_0_1;
    VGREG(vfg_COEFF_4_TAP_10)   = VG_COEFF_4_TAP_1_0;
    VGREG(vfg_COEFF_4_TAP_11)   = VG_COEFF_4_TAP_1_1;
    VGREG(vfg_COEFF_4_TAP_20)   = VG_COEFF_4_TAP_2_0;
    VGREG(vfg_COEFF_4_TAP_21)   = VG_COEFF_4_TAP_2_1;
    VGREG(vfg_COEFF_4_TAP_30)   = VG_COEFF_4_TAP_3_0;
    VGREG(vfg_COEFF_4_TAP_31)   = VG_COEFF_4_TAP_3_1;
    VGREG(vfg_COEFF_4_TAP_40)   = VG_COEFF_4_TAP_4_0;
    VGREG(vfg_COEFF_4_TAP_41)   = VG_COEFF_4_TAP_4_1;
    VGREG(vfg_COEFF_4_TAP_50)   = VG_COEFF_4_TAP_5_0;
    VGREG(vfg_COEFF_4_TAP_51)   = VG_COEFF_4_TAP_5_1;
    VGREG(vfg_COEFF_4_TAP_60)   = VG_COEFF_4_TAP_6_0;
    VGREG(vfg_COEFF_4_TAP_61)   = VG_COEFF_4_TAP_6_1;
    VGREG(vfg_COEFF_4_TAP_70)   = VG_COEFF_4_TAP_7_0;
    VGREG(vfg_COEFF_4_TAP_71)   = VG_COEFF_4_TAP_7_1;
    VGREG(vfg_COEFF_4_TAP_80)   = VG_COEFF_4_TAP_8_0;
    VGREG(vfg_COEFF_4_TAP_81)   = VG_COEFF_4_TAP_8_1;
    VGREG(vfg_COEFF_4_TAP_90)   = VG_COEFF_4_TAP_9_0;
    VGREG(vfg_COEFF_4_TAP_91)   = VG_COEFF_4_TAP_9_1;
    VGREG(vfg_COEFF_4_TAP_A0)   = VG_COEFF_4_TAP_A_0;
    VGREG(vfg_COEFF_4_TAP_A1)   = VG_COEFF_4_TAP_A_1;
    VGREG(vfg_COEFF_4_TAP_B0)   = VG_COEFF_4_TAP_B_0;
    VGREG(vfg_COEFF_4_TAP_B1)   = VG_COEFF_4_TAP_B_1;
    VGREG(vfg_COEFF_4_TAP_C0)   = VG_COEFF_4_TAP_C_0;
    VGREG(vfg_COEFF_4_TAP_C1)   = VG_COEFF_4_TAP_C_1;
    VGREG(vfg_COEFF_4_TAP_D0)   = VG_COEFF_4_TAP_D_0;
    VGREG(vfg_COEFF_4_TAP_D1)   = VG_COEFF_4_TAP_D_1;
    VGREG(vfg_COEFF_4_TAP_E0)   = VG_COEFF_4_TAP_E_0;
    VGREG(vfg_COEFF_4_TAP_E1)   = VG_COEFF_4_TAP_E_1;
    VGREG(vfg_COEFF_4_TAP_F0)   = VG_COEFF_4_TAP_F_0;
    VGREG(vfg_COEFF_4_TAP_F1)   = VG_COEFF_4_TAP_F_1;

    VGFX_CMDENQ(VGREG_COEF_4_TAP_0, VGREG_COEF_4_TAP_0_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_1, VGREG_COEF_4_TAP_1_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_2, VGREG_COEF_4_TAP_2_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_3, VGREG_COEF_4_TAP_3_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_4, VGREG_COEF_4_TAP_4_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_5, VGREG_COEF_4_TAP_5_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_6, VGREG_COEF_4_TAP_6_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_7, VGREG_COEF_4_TAP_7_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_8, VGREG_COEF_4_TAP_8_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_9, VGREG_COEF_4_TAP_9_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_A, VGREG_COEF_4_TAP_A_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_B, VGREG_COEF_4_TAP_B_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_C, VGREG_COEF_4_TAP_C_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_D, VGREG_COEF_4_TAP_D_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_E, VGREG_COEF_4_TAP_E_NUM);
    VGFX_CMDENQ(VGREG_COEF_4_TAP_F, VGREG_COEF_4_TAP_F_NUM);
}

void _VGFX_SetAlcom(UINT32 u4AlcomEn)
{

    //GFX_ASSERT(u4AlcomEn  == 1);
    if (u4AlcomEn)
        VGREG(vfg_ALU_EN) = 1;
    else VGREG(vfg_ALU_EN) = 0;
    VGREG(vfg_ALCOM_LOOP)      = (UINT32)u4AlcomEn;
    VGFX_CMDENQ(VGREG_ALCOM_LOOP, VGREG_ALCOM_LOOP_NUM);
}


void _VGFX_SetQualityEnv(UINT32 u4WbExpMode, UINT32 u4ImgExpMode, UINT32 u4AntiAliasEn, UINT32 u4FilterEn, UINT32 u4AlMode)
{
    VGREG(vfg_WB_EXP)      = (UINT32)u4WbExpMode;
    VGREG(vfg_IMG_EXP_MODE)     = (UINT32)u4ImgExpMode;
    VGREG(vfg_ANTI_ALIAS_EN)  = (UINT32)u4AntiAliasEn;
    VGREG(vfg_FILTER_EN) = (UINT32)u4FilterEn;
    VGREG(vfg_ALPHA_EDGE_MODE) = (UINT32)u4AlMode;
    VGFX_CMDENQ(VGREG_WB_EXP, VGREG_WB_EXP_NUM);
    VGFX_CMDENQ(VGREG_FILTER_CONFIG, VGREG_FILTER_CONFIG_NUM);
}


void  _VGFX_SetPostWrEnv(UINT32 u4PostWriteThreshold, UINT32 fgWdleEnable, UINT32 fgWBRndEn)
{
    UINT32 ui4_post_depth   = 0x3f;
    
    VGREG(vfg_POST_THRS)      = (UINT32)u4PostWriteThreshold;
    VGREG(vfg_POST_DEPTH)     = ui4_post_depth;
    VGREG(vfg_WDLE_EN)      = (UINT32)fgWdleEnable;
    VGREG(vfg_WB_RND_EN)    = (UINT32)fgWBRndEn;
    VGFX_CMDENQ(VGREG_PCTCTL_FIFO, VGREG_PCTCTL_FIFO_NUM);
    VGFX_CMDENQ(VGREG_PCTCTL_END, VGREG_PCTCTL_END_NUM);
    VGFX_CMDENQ(VGREG_WB_RND, VGREG_WB_RND_NUM);
}

void _VGFX_SetWriteArgbOrder( UINT32 u4Aorder, UINT32 u4Rorder, UINT32 u4Gorder, UINT32 u4Border)
{

    VGREG(vfg_B_SEL)  = u4Border;
    VGREG(vfg_G_SEL)  = u4Gorder;
    VGREG(vfg_R_SEL)  = u4Rorder;
    VGREG(vfg_A_SEL)  = u4Aorder;
    VGFX_CMDENQ(VGREG_PSTCTL_ARGB, VGREG_PSTCTL_ARGB_NUM);

}

void _VGFX_Scaler(VGFX_PARAM_SET_T *pt_vgfx)
{
    _VGFX_SetImgage(pt_vgfx->u4ImgAddr,
            pt_vgfx->u4ImgClrMode,
            pt_vgfx->u4ImgGlobalAlpha,
            pt_vgfx->u4ImgPitch);

    _VGFX_SetWb(pt_vgfx->u4WBAddr,
            pt_vgfx->u4WBClrMode,
            pt_vgfx->u4WBGlobalAlpha,
            pt_vgfx->u4WBPitch);

    _VGFX_SetTexture(pt_vgfx->u4TextureHStart,
            pt_vgfx->u4TextureVStart,
            (pt_vgfx->u4TextureHStart+pt_vgfx->u4TextureWidth-1),
            (pt_vgfx->u4TextureVStart+pt_vgfx->u4TextureHeight-1));

    _VGFX_SetBb(pt_vgfx->u4BBXStart,
            pt_vgfx->u4BBYStart,
            pt_vgfx->u4BBWidth,
            pt_vgfx->u4BBHeight);

    _VGFX_SetQualityEnv(VG_CLR_EXP_REPEAT_MSB,
            VG_CLR_EXP_REPEAT_MSB,
            pt_vgfx->fgAntiAliasing,
            pt_vgfx->fgFilterEnable,
            pt_vgfx->u4AlphaEdgeMode);

    if(pt_vgfx->u4AlEdgeRGB0)
    {
        _VGFX_SetAlphaEdgeRGB0(1, pt_vgfx->u4AlEdgeThresHold);
    }
    else
    {
        _VGFX_SetAlphaEdgeRGB0(1, 0);
    }
     
    _VGFX_SetFilterType( pt_vgfx->u4FilterType);
    if(VG_FILTER_TYPE_4TAP ==  pt_vgfx->u4FilterType)
    {
        if(pt_vgfx->u4Filter4TapDefault)
        {
            _VGFX_ResetCoef4Tap();
        }
        else
        {
            _VGFX_SetCoef4Tap(&pt_vgfx->t_coeff_4_tap);
        }
    }

    if(pt_vgfx->t_reflect.u4RefEn)
    {
        _VGFX_SetReflect(1, 
                        pt_vgfx->t_reflect.u4RefOnly, 
                        pt_vgfx->t_reflect.u4TextTureHeight, 
                        pt_vgfx->t_reflect.u4RefRatioInit, 
                        pt_vgfx->t_reflect.u4RatioLineStep, 
                        pt_vgfx->t_reflect.u4RefAAEn);
    }
    else
    {
        _VGFX_SetReflect(0,0,0,0,0,0);
    }

    _VGFX_SetMatrixCoef(&pt_vgfx->t_matrix);
    _VGFX_SetAlcom(pt_vgfx->fgAlphaCompEn);

    if(pt_vgfx->fgAlphaCompEn)
    {
        _VGFX_GeneralAlphaComSet(pt_vgfx->u4ImgClrMode,
                pt_vgfx->u4WBClrMode,
                pt_vgfx->u4AlphaCompAr,
                pt_vgfx->u4AlphaCompOpCode,
                pt_vgfx->fgAlphaCompNormal,
                pt_vgfx->u4PreSrcRd,
                pt_vgfx->u4PreDstRd,
                pt_vgfx->u4PreDstWt,
                pt_vgfx->u4PreOverFlowEn);
    }

    _VGFX_Enable();

    return;
}


void _VGFX_Scaler_Emu(VG_PARAM_SET_T *vgParam)
{
   #if !defined(CC_MT5396) && !defined(CC_MT5368) && !defined(CC_MT5389)&& !defined(CC_MT5398)&&!defined(CC_MT5880)
    float f4Xs;
    float f4Ys;
    float f4Hs;
    float f4Ws;
    float f4Xd;
    float f4Yd;
    float f4Hd;
    float f4Wd, f4Temp;
    UINT32 u4MatrixA;
    UINT32 u4MatrixB;
    UINT32 u4MatrixC;
    UINT32 u4MatrixD;
    UINT32 u4MatrixE;
    UINT32 u4MatrixF;
    UINT32 u4MatrixG;
    UINT32 u4MatrixH;
    UINT32 u4MatrixI;
    UINT32 u4ValTmp;
     switch (vgParam->u4WBClrMode)     // color mode
        {
            case 11:
                u4ValTmp = 2;
                break;
            case 12:
                u4ValTmp = 2;
                break;
            case 13:
                u4ValTmp = 2;
                break;
            case 14:
                u4ValTmp = 4;
                break;
            default:    
        Printf("VGFX only support ARGB8888, ARGB4444, ARGB1555 RGB565 color mode\n");
                return;
        }
    GFX_CHK_16B_ALIGN(vgParam->u4ImgAddr);
    GFX_CHK_64B_ALIGN(vgParam->u4WBAddr);
    GFX_CHK_64B_PITCH(vgParam->u4WBPitch);
    GFX_ASSERT(vgParam->u4BBXStart < (vgParam->u4WBPitch / u4ValTmp));
    GFX_ASSERT(vgParam->u4BBYStart < vgParam->u4WBHeight);
   f4Xs = (float)(vgParam->u4TextureHStart );
   f4Ys = (float)(vgParam->u4TextureVStart);
   f4Hs = (float)(vgParam->u4TextureWidth );
   f4Ws= (float)(vgParam->u4TextureHeight);
   f4Xd = (float)(vgParam->u4BBXStart );
   f4Yd = (float)(vgParam->u4BBYStart);
   f4Hd = (float)(vgParam->u4BBWidth );
   f4Wd = (float)(vgParam->u4BBHeight);

   f4Temp = (f4Ws-1)/(f4Wd-1);
   u4MatrixA = fnum_converter(f4Temp) >> 9;
   u4MatrixB = 0;
   f4Temp = f4Xs - f4Xd*(f4Ws-1)/(f4Wd-1);
   u4MatrixC = fnum_converter(f4Temp) >> 9;
   u4MatrixD= 0;
   f4Temp = (f4Hs-1)/(f4Hd-1);
   u4MatrixE =fnum_converter(f4Temp) >> 9;
   f4Temp = f4Ys-f4Yd*(f4Hs-1)/(f4Hd-1);
   u4MatrixF =fnum_converter(f4Temp) >> 9;
   u4MatrixG = 0;
   u4MatrixH = 0;
   f4Temp = 1;
   u4MatrixI = fnum_converter(f4Temp) >> 9;

    _VGFX_SetImgage(vgParam->u4ImgAddr, vgParam->u4ImgClrMode,
                    vgParam->u4ImgGlobalAlpha, vgParam->u4ImgPitch);

    _VGFX_SetWb(vgParam->u4WBAddr, vgParam->u4WBClrMode,
                vgParam->u4WBGlobalAlpha, vgParam->u4WBPitch);

    _VGFX_SetTexture(vgParam->u4TextureHStart, vgParam->u4TextureVStart,
                     vgParam->u4TextureHStart + vgParam->u4TextureWidth - 1, vgParam->u4TextureVStart + vgParam->u4TextureHeight - 1);

    _VGFX_SetBb(vgParam->u4BBXStart , vgParam->u4BBYStart, vgParam->u4BBWidth, vgParam->u4BBHeight);
    _VGFX_SetQualityEnv(VG_CLR_EXP_REPEAT_MSB, VG_CLR_EXP_REPEAT_MSB, vgParam->fgAntiAliasing,
                        vgParam->fgFilterEnable, vgParam->u4AlphaEdgeMode);
    _VGFX_SetMatrixCoef_Emu( u4MatrixA,  u4MatrixB,  u4MatrixC,  u4MatrixD,
                          u4MatrixE,  u4MatrixF,  u4MatrixG,  u4MatrixH,  u4MatrixI);
    _VGFX_SetAlcom(vgParam->fgAlphaCompEn);
    if (vgParam->fgAlphaCompEn)
    {
        _VGFX_GeneralAlphaComSet(vgParam->u4ImgClrMode, vgParam->u4WBClrMode, vgParam->u4AlphaCompAr,
                                 vgParam->u4AlphaCompOpCode, vgParam->fgAlphaCompNormal, 0, 0, 0, 1);
    }
    _VGFX_Enable();
#endif
}


void _VGFX_SetAlphaEdgeRGB0( UINT32 u4AlEdgeRGB0, UINT32 u4AlEdgeThresHold )
{
	VGREG(vfg_ALPHA_EDGE_RGB0)  = (UINT32)u4AlEdgeRGB0;
	VGREG(vfg_ALPHA_EDGE_THRS)  = (UINT32)u4AlEdgeThresHold;
	VGFX_CMDENQ(VGREG_FILTER_CONFIG, VGREG_FILTER_CONFIG_NUM);
}

void _VGFX_SetFilterType( UINT32 u4FilterType)
{
    VGREG(vfg_FILTER_TYPE)  = (UINT32)u4FilterType;
    VGFX_CMDENQ(VGREG_FILTER_CONFIG, VGREG_FILTER_CONFIG_NUM);
}

void _VGFX_SetReflect( UINT32 u4RefEn, UINT32 u4RefOnly, UINT32 u4TextTureHeight, 
                   UINT32 u4RefRatioInit, UINT32 u4RatioLineStep, UINT32 u4RefAAEn)
{
    VGREG(vfg_REF_EN)  = (UINT32)u4RefEn;
    VGREG(vfg_REF_AA_EN)  = (UINT32)u4RefAAEn;
    VGREG(vfg_REF_ONLY)  = (UINT32)u4RefOnly;
    VGREG(vfg_REF_TXT_H)  = (UINT32)u4TextTureHeight;
    VGREG(vfg_REF_RATIO_STEP)  = (UINT32)u4RatioLineStep;
    VGREG(vfg_REF_RATIO_INIT)  = (UINT32)u4RefRatioInit;
    VGFX_CMDENQ(VGREG_REF_CONFIG, VGREG_REF_CONFIG_NUM);
    VGFX_CMDENQ(VGREG_REFRATIO_CONFIG, VGREG_REFRATIO_CONFIG_NUM);
}

void _VGFX_ResetReflect(void)
{
    VGREG(vfg_REF_EN)           = (UINT32)0;
    VGREG(vfg_REF_AA_EN)        = (UINT32)0;
    VGREG(vfg_REF_ONLY)         = (UINT32)0;
    VGREG(vfg_REF_TXT_H)        = (UINT32)0;
    VGREG(vfg_REF_RATIO_STEP)   = (UINT32)0;
    VGREG(vfg_REF_RATIO_INIT)   = (UINT32)0;
    VGFX_CMDENQ(VGREG_REF_CONFIG, VGREG_REF_CONFIG_NUM);
    VGFX_CMDENQ(VGREG_REFRATIO_CONFIG, VGREG_REFRATIO_CONFIG_NUM);

    return;
}

void _VGFX_SetTextureArgbOrder( UINT32 u4Aorder, UINT32 u4Rorder, UINT32 u4Gorder, UINT32 u4Border)
{

    VGREG(vfg_TXT_B_ORDER)  = u4Border;
    VGREG(vfg_TXT_G_ORDER)  = u4Gorder;
    VGREG(vfg_TXT_R_ORDER)  = u4Rorder;
    VGREG(vfg_TXT_A_ORDER)  = u4Aorder;
    VGFX_CMDENQ(VGREG_FILTER_CONFIG, VGREG_FILTER_CONFIG_NUM);
}


BOOL _VGfx_JugRect(const VGFX_QUADRILATERAL_SET_T _Quadrilateral)
{
	if( (_Quadrilateral.x1 == _Quadrilateral.x4) &&
		(_Quadrilateral.x2 == _Quadrilateral.x3) &&
		(_Quadrilateral.y1 == _Quadrilateral.y2) &&
		(_Quadrilateral.y3 == _Quadrilateral.y4) &&
		(_Quadrilateral.x1 < _Quadrilateral.x2) &&
		(_Quadrilateral.y1 < _Quadrilateral.y4)
	)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void _VGfx_SetVGfxScaler(const VGFX_SCALER_PARAM_SET_T *prCompBlt)
{
	VGFX_TEXTURE_SET_T _SetTexture;
	VGFX_RECT_SET_T _TextRect;
	VGFX_PARAM_SET_T _SetVGFXSc_T;

	GFX_CHK_16B_ALIGN(prCompBlt->_IMAGType.u4BaseAddr);
	GFX_CHK_64B_ALIGN(prCompBlt->_WBType.u4BaseAddr);
	GFX_CHK_64B_PITCH(prCompBlt->_WBType.u4Pitch);
	GFX_ASSERT(prCompBlt->_BBType.u4StartX < prCompBlt->_WBType.u4WBWidth);
	GFX_ASSERT(prCompBlt->_BBType.u4StartY < prCompBlt->_WBType.u4WBHeight);

	 if( !_VGfx_JugRect(prCompBlt->_SrcQuard) )
	 {
		 return;
	 }
	 else
	 {
			 _SetTexture.u4StartX =prCompBlt->_SrcQuard.x1;
			 _SetTexture.u4StartY =prCompBlt->_SrcQuard.y1;
			 _SetTexture.u4EndX =prCompBlt->_SrcQuard.x3;
			 _SetTexture.u4EndY =prCompBlt->_SrcQuard.y3;
			 
			 _TextRect.u4StartX = _SetTexture.u4StartX;
			 _TextRect.u4StartY = _SetTexture.u4StartY;
			 _TextRect.u4Width = _SetTexture.u4EndX - _SetTexture.u4StartX;
			 _TextRect.u4Height = _SetTexture.u4EndY - _SetTexture.u4StartY;
	 }
	
	 if( prCompBlt->_SetAlphaComp.u4AlphaCompEn)
	 {
		 _SetVGFXSc_T.fgAlphaCompEn = 1;
		 _SetVGFXSc_T.fgAlphaCompNormal = prCompBlt->_SetAlphaComp.fgAlphaCompNormal;
		 _SetVGFXSc_T.u4AlphaCompAr = prCompBlt->_SetAlphaComp.u4AlphaCompAr;
		 _SetVGFXSc_T.u4AlphaCompOpCode = prCompBlt->_SetAlphaComp.u4AlphaCompOpCode;
		 _SetVGFXSc_T.u4AlphaEdgeMode = (ALPHA_EDGE_MD)prCompBlt->_SetAlphaComp._AlphaEdgeMd;
		 _SetVGFXSc_T.u4PreDstRd = prCompBlt->_SetAlphaComp.u4PreDstRd;
		 _SetVGFXSc_T.u4PreDstWt = prCompBlt->_SetAlphaComp.u4PreDstWt;
		 _SetVGFXSc_T.u4PreSrcRd = prCompBlt->_SetAlphaComp.u4PreSrcRd;
	
	 }
	 else
	 {
		 _SetVGFXSc_T.fgAlphaCompEn = 0;
		 _SetVGFXSc_T.fgAlphaCompNormal = prCompBlt->_SetAlphaComp.fgAlphaCompNormal;
		 _SetVGFXSc_T.u4AlphaCompAr = prCompBlt->_SetAlphaComp.u4AlphaCompAr;
		 _SetVGFXSc_T.u4AlphaCompOpCode = prCompBlt->_SetAlphaComp.u4AlphaCompOpCode;
		 _SetVGFXSc_T.u4AlphaEdgeMode = (ALPHA_EDGE_MD)prCompBlt->_SetAlphaComp._AlphaEdgeMd;
		 _SetVGFXSc_T.u4PreDstRd = prCompBlt->_SetAlphaComp.u4PreDstRd;
		 _SetVGFXSc_T.u4PreDstWt = prCompBlt->_SetAlphaComp.u4PreDstWt;
		 _SetVGFXSc_T.u4PreSrcRd = prCompBlt->_SetAlphaComp.u4PreSrcRd;

	
	 }
	
	 _SetVGFXSc_T.fgAntiAliasing = prCompBlt->u4AntiAliasing;
	 _SetVGFXSc_T.fgFilterEnable = prCompBlt->_SetFilter.u4FilterEn;
	
	
	 _SetVGFXSc_T.u4BBHeight = prCompBlt->_BBType.u4Height ;
	 _SetVGFXSc_T.u4BBWidth = prCompBlt->_BBType.u4Width ;
	 _SetVGFXSc_T.u4BBXStart = prCompBlt->_BBType.u4StartX ;
	 _SetVGFXSc_T.u4BBYStart = prCompBlt->_BBType.u4StartY ;
	 
	 _SetVGFXSc_T.u4ImgAddr = prCompBlt->_IMAGType.u4BaseAddr ;
	 _SetVGFXSc_T.u4ImgClrMode = prCompBlt->_IMAGType.u4ColrMode ;
	 _SetVGFXSc_T.u4ImgGlobalAlpha = prCompBlt->_IMAGType.u4GlAr ;
	 _SetVGFXSc_T.u4ImgPitch = prCompBlt->_IMAGType.u4Pitch;
	
	 _SetVGFXSc_T.u4TextureHStart = _TextRect.u4StartX;
	 _SetVGFXSc_T.u4TextureVStart = _TextRect.u4StartY;
	 _SetVGFXSc_T.u4TextureWidth = _TextRect.u4Width;
	 _SetVGFXSc_T.u4TextureHeight = _TextRect.u4Height;
	
	 _SetVGFXSc_T.u4WBAddr = prCompBlt->_WBType.u4BaseAddr;
	 _SetVGFXSc_T.u4WBClrMode = prCompBlt->_WBType.u4ColrMode;
	 _SetVGFXSc_T.u4WBGlobalAlpha = prCompBlt->_WBType.u4GlAr;
	 _SetVGFXSc_T.u4WBPitch = prCompBlt->_WBType.u4Pitch;
	 _SetVGFXSc_T.u4WBHeight = prCompBlt->_WBType.u4WBHeight;
	 _SetVGFXSc_T.u4WBWidth = prCompBlt->_WBType.u4WBWidth;
	
	_SetVGFXSc_T.t_matrix.u4_a = prCompBlt->_SetMartix.u4_a;
	_SetVGFXSc_T.t_matrix.u4_b = prCompBlt->_SetMartix.u4_b;
	_SetVGFXSc_T.t_matrix.u4_c = prCompBlt->_SetMartix.u4_c;
	_SetVGFXSc_T.t_matrix.u4_d = prCompBlt->_SetMartix.u4_d;
	_SetVGFXSc_T.t_matrix.u4_e = prCompBlt->_SetMartix.u4_e;
	_SetVGFXSc_T.t_matrix.u4_f = prCompBlt->_SetMartix.u4_f;
	_SetVGFXSc_T.t_matrix.u4_g = prCompBlt->_SetMartix.u4_g;
	_SetVGFXSc_T.t_matrix.u4_h = prCompBlt->_SetMartix.u4_h;
	_SetVGFXSc_T.t_matrix.u4_i = prCompBlt->_SetMartix.u4_i;
	
	if( prCompBlt->_SetReflect.u4RefEn)
	{
		_VGFX_SetReflect(TRUE,
					prCompBlt->_SetReflect.u4RefOnly,
					prCompBlt->_SetReflect.u4TextTureHeight,
					prCompBlt->_SetReflect.u4RefRatioInit,
					prCompBlt->_SetReflect.u4RatioLineStep,
					prCompBlt->_SetReflect.u4RefAAEn);
	}
	else
	{
		_VGFX_SetReflect(FALSE,
					prCompBlt->_SetReflect.u4RefOnly,
					prCompBlt->_SetReflect.u4TextTureHeight,
					prCompBlt->_SetReflect.u4RefRatioInit,
					prCompBlt->_SetReflect.u4RatioLineStep,
					prCompBlt->_SetReflect.u4RefAAEn);

	}
	
	_SetVGFXSc_T.t_reflect.u4RefEn= 0x0;
	_SetVGFXSc_T.u4AlEdgeRGB0     = 0x0;
	_SetVGFXSc_T.u4FilterType     = VG_FILTER_TYPE_4TAP;

    _VGFX_Scaler(&_SetVGFXSc_T);
}

#endif
