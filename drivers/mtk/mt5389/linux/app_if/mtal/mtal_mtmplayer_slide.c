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
 * $RCSfile: mtal_mtmplayer_slide.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
/** @file mtal_mtmplayer_slide.c
 */

/********************************************************************
  INCLUDE FILES
********************************************************************/
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "mtal_mtmplayer_slide.h"

/********************************************************************
  MACRO CONSTANT DEFINITIONS
********************************************************************/


/********************************************************************
  TYPE DEFINITION
********************************************************************/
#define SLIDE_SEG_FULLSIZE      16
#define SLIDE_SEG_VHALFSIZE     12
#define SLIDE_SEG_WHALFSIZE     8

#if 0
VOID MTMPLAYER_BarSlideLR(IMG_VP_INFO_T rVpInfo)
{    
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    UINT32 u4I;
    
    for (u4I = 16; u4I <= rVpInfo.u4Width; u4I += 16)
    {
        rScaleParamV.u4IsRsIn = 1;        
        rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[0];
        rScaleParamV.u4YSrcBufLen = ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YSrcHOffset = rVpInfo.u4Width - u4I;
        rScaleParamV.u4YSrcVOffset = 0;
        rScaleParamV.u4YSrcW = u4I;
        rScaleParamV.u4YSrcH = rVpInfo.u4Height;
        rScaleParamV.u4CSrcW = u4I >> 1;
        rScaleParamV.u4CSrcH = rVpInfo.u4Height;
        rScaleParamV.u4CSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[1];
        rScaleParamV.u4CSrcHOffset = (rVpInfo.u4Width - u4I) >> 1;
        rScaleParamV.u4CSrcVOffset = 0;
        rScaleParamV.u4IsRsOut = 1;
        rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4IsVdo2Osd = 0;     
        rScaleParamV.u4YTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[0];       
        rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YTgBufLen = ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YTgHOffset = 0;
        rScaleParamV.u4YTgVOffset = 0;
        rScaleParamV.u4YTgW = u4I;
        rScaleParamV.u4YTgH = rVpInfo.u4Height;
        rScaleParamV.u4CTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[1];        
        rScaleParamV.u4CTgW = u4I >> 1;
        rScaleParamV.u4CTgH = rVpInfo.u4Height;        
        rScaleParamV.u4SrcSwap = 0;    
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();
    }
    return MTR_OK;
}
#endif


/**horizontal bar wipe effect
*@param rVpInfo vdp parameters
*@param fgL2R, TRUE: left to right, FALSE: right to left
*@retval
*/
VOID MTMPLAYER_BarWipeH(IMG_VP_INFO_T rVpInfo, BOOL fgL2R)
{    
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    UINT32 u4I;
    
    for (u4I = SLIDE_SEG_FULLSIZE; u4I <= rVpInfo.u4Width; u4I += SLIDE_SEG_FULLSIZE)
    {
        rScaleParamV.u4IsRsIn = 1;        
        rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[0];
        rScaleParamV.u4YSrcBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YSrcHOffset = (fgL2R) ? u4I - SLIDE_SEG_FULLSIZE : rVpInfo.u4Width - u4I;
        rScaleParamV.u4YSrcVOffset = 0;
        rScaleParamV.u4YSrcW = SLIDE_SEG_FULLSIZE;
        rScaleParamV.u4YSrcH = rVpInfo.u4Height;
        rScaleParamV.u4CSrcW = SLIDE_SEG_FULLSIZE >> 1;
        rScaleParamV.u4CSrcH = rVpInfo.u4Height;
        rScaleParamV.u4CSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[1];
        rScaleParamV.u4CSrcHOffset = (fgL2R)?(u4I - SLIDE_SEG_FULLSIZE) >> 1:(rVpInfo.u4Width - u4I) >> 1;
        rScaleParamV.u4CSrcVOffset = 0;
        rScaleParamV.u4IsRsOut = 1;
        rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4IsVdo2Osd = 0;     
        rScaleParamV.u4YTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[0];       
        rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YTgBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YTgHOffset = (fgL2R) ? u4I - SLIDE_SEG_FULLSIZE : rVpInfo.u4Width - u4I;
        rScaleParamV.u4YTgVOffset = 0;
        rScaleParamV.u4YTgW = SLIDE_SEG_FULLSIZE;
        rScaleParamV.u4YTgH = rVpInfo.u4Height;
        rScaleParamV.u4CTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[1];        
        rScaleParamV.u4CTgW = SLIDE_SEG_FULLSIZE >> 1;
        rScaleParamV.u4CTgH = rVpInfo.u4Height;        
        rScaleParamV.u4SrcSwap = 0;    
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();
        x_thread_delay(1);
    }
    return;
}


/**vertical bar wipe effect
*@param rVpInfo vdp parameters
*@param fgT2B, TRUE: top to bottom, FALSE: bottom to top
*@retval
*/
VOID MTMPLAYER_BarWipeV(IMG_VP_INFO_T rVpInfo, BOOL fgT2B)
{    
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    UINT32 u4I;
    
    for (u4I = SLIDE_SEG_FULLSIZE; u4I <= rVpInfo.u4Height; u4I += SLIDE_SEG_FULLSIZE)
    {
        rScaleParamV.u4IsRsIn = 1;        
        rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[0];
        rScaleParamV.u4YSrcBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YSrcHOffset = 0;
        rScaleParamV.u4YSrcVOffset = (fgT2B) ? u4I - SLIDE_SEG_FULLSIZE : rVpInfo.u4Height - u4I;
        rScaleParamV.u4YSrcW = rVpInfo.u4Width;
        rScaleParamV.u4YSrcH = SLIDE_SEG_FULLSIZE;
        rScaleParamV.u4CSrcW = rVpInfo.u4Width >> 1;
        rScaleParamV.u4CSrcH = SLIDE_SEG_FULLSIZE;
        rScaleParamV.u4CSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[1];
        rScaleParamV.u4CSrcHOffset = 0;
        rScaleParamV.u4CSrcVOffset = (fgT2B) ? u4I - SLIDE_SEG_FULLSIZE : rVpInfo.u4Height - u4I;
        rScaleParamV.u4IsRsOut = 1;
        rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4IsVdo2Osd = 0;     
        rScaleParamV.u4YTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[0];       
        rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YTgBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YTgHOffset = 0;
        rScaleParamV.u4YTgVOffset = (fgT2B) ? u4I - SLIDE_SEG_FULLSIZE : rVpInfo.u4Height - u4I;
        rScaleParamV.u4YTgW = rVpInfo.u4Width;
        rScaleParamV.u4YTgH = SLIDE_SEG_FULLSIZE;
        rScaleParamV.u4CTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[1];        
        rScaleParamV.u4CTgW = rVpInfo.u4Width >> 1;
        rScaleParamV.u4CTgH = SLIDE_SEG_FULLSIZE;        
        rScaleParamV.u4SrcSwap = 0;    
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();
        x_thread_delay(1);
    }
    return;
}


/**box wipe effect
*@param rVpInfo vdp parameters
*@param fgT2B, TRUE: top to bottom, FALSE: bottom to top
*@param fgL2R, TRUE: left to right, FALSE: right to left
*@retval
*/
VOID MTMPLAYER_BoxWipe(IMG_VP_INFO_T rVpInfo, BOOL fgT2B, BOOL fgL2R)
{    
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    UINT32 u4I;
    
    for (u4I = SLIDE_SEG_FULLSIZE; u4I <= rVpInfo.u4Width; u4I += SLIDE_SEG_FULLSIZE)
    {
        rScaleParamV.u4IsRsIn = 1;        
        rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[0];
        rScaleParamV.u4YSrcBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);        
        rScaleParamV.u4YSrcW = u4I;
        rScaleParamV.u4YSrcH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4YSrcHOffset = (fgL2R) ? 0 : rVpInfo.u4Width - u4I;
        rScaleParamV.u4YSrcVOffset = (fgT2B) ? 0 : rVpInfo.u4Height - rScaleParamV.u4YSrcH;
        rScaleParamV.u4CSrcW = u4I >> 1;
        rScaleParamV.u4CSrcH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4CSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[1];
        rScaleParamV.u4CSrcHOffset = (fgL2R) ? 0 : (rVpInfo.u4Width - u4I) >> 1;
        rScaleParamV.u4CSrcVOffset = (fgT2B) ? 0 : rVpInfo.u4Height - rScaleParamV.u4YSrcH;
        rScaleParamV.u4IsRsOut = 1;
        rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4IsVdo2Osd = 0;     
        rScaleParamV.u4YTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[0];       
        rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YTgBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YTgHOffset = (fgL2R) ? 0 : rVpInfo.u4Width - u4I;
        rScaleParamV.u4YTgVOffset = (fgT2B) ? 0 : rVpInfo.u4Height - rScaleParamV.u4YSrcH;
        rScaleParamV.u4YTgW = u4I;
        rScaleParamV.u4YTgH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4CTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[1];        
        rScaleParamV.u4CTgW = u4I >> 1;
        rScaleParamV.u4CTgH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;        
        rScaleParamV.u4SrcSwap = 0;    
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();
    }
    return;
}

#if 0
VOID MTMPLAYER_BoxWipeTR(IMG_VP_INFO_T rVpInfo)
{    
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    UINT32 u4I;
    
    for (u4I = SLIDE_SEG_FULLSIZE; u4I <= rVpInfo.u4Width; u4I += SLIDE_SEG_FULLSIZE)
    {
        rScaleParamV.u4IsRsIn = 1;        
        rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[0];
        rScaleParamV.u4YSrcBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YSrcHOffset = rVpInfo.u4Width - u4I;
        rScaleParamV.u4YSrcVOffset = 0;
        rScaleParamV.u4YSrcW = u4I;
        rScaleParamV.u4YSrcH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4CSrcW = u4I >> 1;
        rScaleParamV.u4CSrcH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4CSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[1];
        rScaleParamV.u4CSrcHOffset = (rVpInfo.u4Width - u4I) >> 1;
        rScaleParamV.u4CSrcVOffset = 0;
        rScaleParamV.u4IsRsOut = 1;
        rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4IsVdo2Osd = 0;     
        rScaleParamV.u4YTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[0];       
        rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YTgBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YTgHOffset = rVpInfo.u4Width - u4I;
        rScaleParamV.u4YTgVOffset = 0;
        rScaleParamV.u4YTgW = u4I;
        rScaleParamV.u4YTgH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4CTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[1];        
        rScaleParamV.u4CTgW = u4I >> 1;
        rScaleParamV.u4CTgH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;        
        rScaleParamV.u4SrcSwap = 0;    
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();
    }
    return;
}


VOID MTMPLAYER_BoxWipeBL(IMG_VP_INFO_T rVpInfo)
{    
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    UINT32 u4I;
    
    for (u4I = SLIDE_SEG_FULLSIZE; u4I <= rVpInfo.u4Width; u4I += SLIDE_SEG_FULLSIZE)
    {
        rScaleParamV.u4IsRsIn = 1;        
        rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[0];
        rScaleParamV.u4YSrcBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YSrcW = u4I;
        rScaleParamV.u4YSrcH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4CSrcW = u4I >> 1;
        rScaleParamV.u4CSrcH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4YSrcHOffset = 0;
        rScaleParamV.u4YSrcVOffset = rVpInfo.u4Height - rScaleParamV.u4YSrcH;
        rScaleParamV.u4CSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[1];
        rScaleParamV.u4CSrcHOffset = 0;
        rScaleParamV.u4CSrcVOffset = rVpInfo.u4Height - rScaleParamV.u4YSrcH;
        rScaleParamV.u4IsRsOut = 1;
        rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4IsVdo2Osd = 0;     
        rScaleParamV.u4YTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[0];       
        rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YTgBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YTgW = u4I;
        rScaleParamV.u4YTgH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4CTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[1];        
        rScaleParamV.u4CTgW = u4I >> 1;
        rScaleParamV.u4CTgH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;      
        rScaleParamV.u4YTgHOffset = 0;
        rScaleParamV.u4YTgVOffset = rVpInfo.u4Height - rScaleParamV.u4YTgH;
        rScaleParamV.u4SrcSwap = 0;    
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();
    }
    return;
}


VOID MTMPLAYER_BoxWipeBR(IMG_VP_INFO_T rVpInfo)
{    
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    UINT32 u4I;
    
    for (u4I = SLIDE_SEG_FULLSIZE; u4I <= rVpInfo.u4Width; u4I += SLIDE_SEG_FULLSIZE)
    {
        rScaleParamV.u4IsRsIn = 1;        
        rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[0];
        rScaleParamV.u4YSrcBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YSrcW = u4I;
        rScaleParamV.u4YSrcH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4CSrcW = u4I >> 1;
        rScaleParamV.u4CSrcH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4YSrcHOffset = rVpInfo.u4Width - u4I;
        rScaleParamV.u4YSrcVOffset = rVpInfo.u4Height - rScaleParamV.u4YSrcH;
        rScaleParamV.u4CSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[1];
        rScaleParamV.u4CSrcHOffset = (rVpInfo.u4Width - u4I) >> 1;
        rScaleParamV.u4CSrcVOffset = rVpInfo.u4Height - rScaleParamV.u4YSrcH;
        rScaleParamV.u4IsRsOut = 1;
        rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4IsVdo2Osd = 0;     
        rScaleParamV.u4YTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[0];       
        rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YTgBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YTgW = u4I;
        rScaleParamV.u4YTgH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4CTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[1];        
        rScaleParamV.u4CTgW = u4I >> 1;
        rScaleParamV.u4CTgH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;      
        rScaleParamV.u4YTgHOffset = rVpInfo.u4Width - u4I;
        rScaleParamV.u4YTgVOffset = rVpInfo.u4Height - rScaleParamV.u4YTgH;
        rScaleParamV.u4SrcSwap = 0;    
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();
    }
    return;
}
#endif

/**horizontal bandoor wipe effect
*@param rVpInfo vdp parameters
*@param fgL2R, TRUE: close, FALSE: open
*@retval
*/
VOID MTMPLAYER_DoorWipeH(IMG_VP_INFO_T rVpInfo, BOOL fgClose)
{    
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    UINT32 u4Width;
    UINT32 u4I;

    u4Width = rVpInfo.u4Width >> 1;
    for (u4I = SLIDE_SEG_FULLSIZE; u4I <= u4Width; u4I += SLIDE_SEG_FULLSIZE)
    {
        rScaleParamV.u4IsRsIn = 1;        
        rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[0];
        rScaleParamV.u4YSrcBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YSrcHOffset = (fgClose) ? u4I - SLIDE_SEG_FULLSIZE : u4Width - u4I;
        rScaleParamV.u4YSrcVOffset = 0;
        rScaleParamV.u4YSrcW = SLIDE_SEG_FULLSIZE;
        rScaleParamV.u4YSrcH = rVpInfo.u4Height;
        rScaleParamV.u4CSrcW = SLIDE_SEG_FULLSIZE >> 1;
        rScaleParamV.u4CSrcH = rVpInfo.u4Height;
        rScaleParamV.u4CSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[1];
        rScaleParamV.u4CSrcHOffset = (fgClose) ? (u4I - SLIDE_SEG_FULLSIZE) >> 1 : (u4Width - u4I) >> 1;
        rScaleParamV.u4CSrcVOffset = 0;
        rScaleParamV.u4IsRsOut = 1;
        rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4IsVdo2Osd = 0;     
        rScaleParamV.u4YTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[0];       
        rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YTgBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YTgHOffset = (fgClose) ? u4I - SLIDE_SEG_FULLSIZE : u4Width - u4I;
        rScaleParamV.u4YTgVOffset = 0;
        rScaleParamV.u4YTgW = SLIDE_SEG_FULLSIZE;
        rScaleParamV.u4YTgH = rVpInfo.u4Height;
        rScaleParamV.u4CTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[1];        
        rScaleParamV.u4CTgW = SLIDE_SEG_FULLSIZE >> 1;
        rScaleParamV.u4CTgH = rVpInfo.u4Height;        
        rScaleParamV.u4SrcSwap = 0;    
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();

        rScaleParamV.u4YSrcHOffset = (fgClose) ? rVpInfo.u4Width - u4I : 
                                                 u4Width + u4I - SLIDE_SEG_FULLSIZE;
        rScaleParamV.u4CSrcHOffset = (fgClose) ? (rVpInfo.u4Width - u4I) >> 1 : 
                                                 (u4Width + u4I - SLIDE_SEG_FULLSIZE) >> 1;
        rScaleParamV.u4YTgHOffset = (fgClose) ? rVpInfo.u4Width - u4I : 
                                                 u4Width + u4I - SLIDE_SEG_FULLSIZE;
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();
        x_thread_delay(1);
    }
    return;
}


/**vertical bandoor wipe effect
*@param rVpInfo vdp parameters
*@param fgL2R, TRUE: close, FALSE: open
*@retval
*/
VOID MTMPLAYER_DoorWipeV(IMG_VP_INFO_T rVpInfo, BOOL fgClose)
{    
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    UINT32 u4Height;
    UINT32 u4I;

    u4Height = rVpInfo.u4Height >> 1;
    for (u4I = SLIDE_SEG_VHALFSIZE; u4I <= u4Height; u4I += SLIDE_SEG_VHALFSIZE)
    {
        rScaleParamV.u4IsRsIn = 1;        
        rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[0];
        rScaleParamV.u4YSrcBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YSrcHOffset = 0;
        rScaleParamV.u4YSrcVOffset = (fgClose) ? u4I - SLIDE_SEG_VHALFSIZE : u4Height - u4I;
        rScaleParamV.u4YSrcW = rVpInfo.u4Width;
        rScaleParamV.u4YSrcH = SLIDE_SEG_VHALFSIZE;
        rScaleParamV.u4CSrcW = rVpInfo.u4Width >> 1;
        rScaleParamV.u4CSrcH = SLIDE_SEG_VHALFSIZE;
        rScaleParamV.u4CSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[1];
        rScaleParamV.u4CSrcHOffset = 0;
        rScaleParamV.u4CSrcVOffset = (fgClose) ? u4I - SLIDE_SEG_VHALFSIZE : u4Height - u4I;
        rScaleParamV.u4IsRsOut = 1;
        rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4IsVdo2Osd = 0;     
        rScaleParamV.u4YTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[0];       
        rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YTgBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YTgHOffset = 0;
        rScaleParamV.u4YTgVOffset = (fgClose) ? u4I - SLIDE_SEG_VHALFSIZE : u4Height - u4I;
        rScaleParamV.u4YTgW = rVpInfo.u4Width;
        rScaleParamV.u4YTgH = SLIDE_SEG_VHALFSIZE;
        rScaleParamV.u4CTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[1];        
        rScaleParamV.u4CTgW = rVpInfo.u4Width >> 1;
        rScaleParamV.u4CTgH = SLIDE_SEG_VHALFSIZE;        
        rScaleParamV.u4SrcSwap = 0;    
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();

        rScaleParamV.u4YSrcVOffset = (fgClose) ? rVpInfo.u4Height - u4I : 
                                                 u4Height + u4I - SLIDE_SEG_VHALFSIZE;
        rScaleParamV.u4CSrcVOffset = (fgClose) ? rVpInfo.u4Height - u4I : 
                                                 u4Height + u4I - SLIDE_SEG_VHALFSIZE;
        rScaleParamV.u4YTgVOffset = (fgClose) ? rVpInfo.u4Height - u4I : 
                                                 u4Height + u4I - SLIDE_SEG_VHALFSIZE;
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();
        x_thread_delay(1);
    }
    return;
}

#if 0
VOID MTMPLAYER_DoorWipeVO(IMG_VP_INFO_T rVpInfo)
{    
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    UINT32 u4Height;
    UINT32 u4I;

    u4Height = rVpInfo.u4Height >> 1;
    for (u4I = SLIDE_SEG_VHALFSIZE; u4I <= u4Height; u4I += SLIDE_SEG_VHALFSIZE)
    {
        rScaleParamV.u4IsRsIn = 1;        
        rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[0];
        rScaleParamV.u4YSrcBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YSrcHOffset = 0;
        rScaleParamV.u4YSrcVOffset = u4Height - u4I;
        rScaleParamV.u4YSrcW = rVpInfo.u4Width;
        rScaleParamV.u4YSrcH = SLIDE_SEG_VHALFSIZE;
        rScaleParamV.u4CSrcW = rVpInfo.u4Width >> 1;
        rScaleParamV.u4CSrcH = SLIDE_SEG_VHALFSIZE;
        rScaleParamV.u4CSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[1];
        rScaleParamV.u4CSrcHOffset = 0;
        rScaleParamV.u4CSrcVOffset = u4Height - u4I;
        rScaleParamV.u4IsRsOut = 1;
        rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4IsVdo2Osd = 0;     
        rScaleParamV.u4YTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[0];       
        rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YTgBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YTgHOffset = 0;
        rScaleParamV.u4YTgVOffset = u4Height - u4I;
        rScaleParamV.u4YTgW = rVpInfo.u4Width;
        rScaleParamV.u4YTgH = SLIDE_SEG_VHALFSIZE;
        rScaleParamV.u4CTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[1];        
        rScaleParamV.u4CTgW = rVpInfo.u4Width >> 1;
        rScaleParamV.u4CTgH = SLIDE_SEG_VHALFSIZE;        
        rScaleParamV.u4SrcSwap = 0;    
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();

        rScaleParamV.u4YSrcVOffset = u4Height + u4I - SLIDE_SEG_VHALFSIZE;
        rScaleParamV.u4CSrcVOffset = u4Height + u4I - SLIDE_SEG_VHALFSIZE;
        rScaleParamV.u4YTgVOffset = u4Height + u4I - SLIDE_SEG_VHALFSIZE;
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();
    }
    return;
}
#endif

VOID MTMPLAYER_IrisWipe(IMG_VP_INFO_T rVpInfo)
{    
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    UINT32 u4Width, u4Height;
    UINT32 u4I, u4Half;

    u4Width = rVpInfo.u4Width >> 1;
    u4Height = rVpInfo.u4Height >> 1;
    for (u4I = SLIDE_SEG_FULLSIZE; u4I <= rVpInfo.u4Width; u4I += SLIDE_SEG_FULLSIZE)
    {
        u4Half = u4I >> 1;
        rScaleParamV.u4IsRsIn = 1;        
        rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[0];
        rScaleParamV.u4YSrcBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YSrcHOffset = u4Width - u4Half;
        rScaleParamV.u4YSrcVOffset = (u4Height < u4Half) ?  0 : u4Height - u4Half;
        rScaleParamV.u4YSrcW = u4I;
        rScaleParamV.u4YSrcH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4CSrcW = u4I >> 1;
        rScaleParamV.u4CSrcH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4CSrcBase = (UINT32)rVpInfo.pu1BackFrameAddr[1];
        rScaleParamV.u4CSrcHOffset = (u4Width - u4Half) >> 1;
        rScaleParamV.u4CSrcVOffset = (u4Height < u4Half) ?  0 : u4Height - u4Half;
        rScaleParamV.u4IsRsOut = 1;
        rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4IsVdo2Osd = 0;     
        rScaleParamV.u4YTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[0];       
        rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_422;
        rScaleParamV.u4YTgBufLen = D_RZ_ALIGN16(rVpInfo.u4Width);
        rScaleParamV.u4YTgHOffset = u4Width - u4Half;
        rScaleParamV.u4YTgVOffset = (u4Height < u4Half) ?  0 : u4Height - u4Half;
        rScaleParamV.u4YTgW = u4I;
        rScaleParamV.u4YTgH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;
        rScaleParamV.u4CTgBase = (UINT32)rVpInfo.pu1CurFrameAddr[1];        
        rScaleParamV.u4CTgW = u4I >> 1;
        rScaleParamV.u4CTgH = (u4I > rVpInfo.u4Height) ? rVpInfo.u4Height : u4I;        
        rScaleParamV.u4SrcSwap = 0;    
        IMGRZ_Lock();
        IMGRZ_ReInit();
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
        IMGRZ_Scale((void *)(&rScaleParamV));           
        IMGRZ_Flush();
        IMGRZ_Wait();
        IMGRZ_Unlock();
    }
    return;
}

