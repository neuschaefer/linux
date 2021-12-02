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
 * $RCSfile: gfx_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
 
/** @file gfx_drvif.h
 *  gfx driver internal public interfaces
 *  
 */
#ifndef _VGFX_DRVIF_
#define _VGFX_DRVIF_

typedef enum 
{
    VG_CLR_EXP_REPEAT_MSB,
    VG_CLR_EXP_PADDING_ZERO,
} VGFX_CLR_EXP_MD;

typedef enum 
{
    VG_ALPHA_EDGE_NEAREST,   // argb nearest neighbor
    VG_ALPHA_EDGE_ZERO,   // a=0, rgb nearest neighbor
    VG_ALPHA_EDGE_NORMAL,  //normal 
} VG_ALPHA_EDGE_MD;

typedef struct _VGFX_MATRIX_T
{
	UINT32          u4_a;
    UINT32          u4_b;
    UINT32          u4_c;
    UINT32          u4_d;
    UINT32          u4_e;
    UINT32          u4_f;
    UINT32          u4_g;
    UINT32          u4_h;
    UINT32          u4_i;
}VGFX_MATRIX_T;


typedef struct _VG_PARAM_SET_T
{
    UINT32          u4ImgAddr;
    UINT32          u4ImgPitch;
    UINT32          u4ImgClrMode;
    UINT32          u4ImgGlobalAlpha;
    
    UINT32          u4TextureHStart;
    UINT32          u4TextureVStart;
    UINT32          u4TextureWidth;
    UINT32          u4TextureHeight;

	UINT32          u4WBAddr;
    UINT32          u4WBPitch;
    UINT32          u4WBXStart;
    UINT32          u4WBYStart;
    UINT32          u4WBWidth;
    UINT32          u4WBHeight;
    UINT32          u4WBClrMode;
    UINT32          u4WBGlobalAlpha;
	
    UINT32          u4PreSrcRd;
    UINT32          u4PreDstRd;
    UINT32          u4PreDstWt;
    UINT32          u4PreOverFlowEn;
	
    UINT32          u4BBXStart;
    UINT32          u4BBYStart;
    UINT32          u4BBWidth;
    UINT32          u4BBHeight;
    VGFX_MATRIX_T   t_matrix;

    UINT32          u4AlphaCompAr;
    UINT32          u4AlphaCompOpCode;  
    BOOL            fgAlphaCompEn;
    BOOL            fgAlphaCompNormal;

    BOOL            fgAntiAliasing;             //anti-alising          0x300_OK//en
    BOOL            fgFilterEnable;
	ALPHA_EDGE_MD   u4AlphaEdgeMode;
} VGFX_PARAM_SET_T;


extern void gaussian_elimination( double A[][8], double X[], double B[]);
extern void derive_inverse_transform( float src_x[], float src_y[], float dst_x[], float dst_y[], float inv_matrix[][3] );
extern unsigned int fnum_converter (float in);

extern void _VGFX_Init(void);

extern void _VGFX_SetWb(UINT32 u4BaseAddr, UINT32 u4ColrMode, UINT32 u4GlAr, UINT32 u4Pitch);
extern void _VGFX_SetTexture(UINT32 u4StartX, UINT32 u4StartY, UINT32 u4EndX, UINT32 u4EndY);

extern void _VGFX_SetBb(UINT32 u4StartX, UINT32 u4StartY, UINT32 u4Width, UINT32 u4Height);

extern void _VGFX_SetImgage(UINT32 u4BaseAddr, UINT32 u4ColrMode, UINT32 u4GlAr, UINT32 u4Pitch);

extern void _VGFX_SetMatrixCoef(VGFX_MATRIX_T* pt_matrix);

extern void  _VGFX_SetAlcom(UINT32 u4AlcomEn);

extern void _VGFX_Scaler(VGFX_PARAM_SET_T *vgParam);


extern void _VGFX_SetQualityEnv(UINT32 u4WbExpMode, UINT32 u4ImgExpMode, UINT32 u4AntiAliasEn, UINT32 u4FilterEn, UINT32 u4AlMode);

extern void  _VGFX_SetPostWrEnv(UINT32 u4PostWriteThreshold, UINT32 fgWdleEnable,  UINT32 fgWBRndEn);

extern void _VGFX_SetWriteArgbOrder( UINT32 u4Aorder,UINT32 u4Rorder,UINT32 u4Gorder,UINT32 u4Border);

#endif

