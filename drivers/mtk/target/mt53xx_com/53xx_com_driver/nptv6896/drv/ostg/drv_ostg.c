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
 * $RCSfile: drv_ostg.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

 #ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#ifndef CC_COPLAT_MT82
#include "x_os.h"
#include "x_hal_arm.h"
#endif

#include "hw_scpos.h"
#include "hw_ospe.h"
#include "drv_scpos.h"
#include "general.h"
#include "scpos_debug.h"
#ifdef CC_MT5396
#include "drv_mjc.h"
#endif


//-----------------------------------------------------------------------------
/** Brief of vScposSetMuteColor.
 * Set Background Color for Main and PIP Channel
 */
//-----------------------------------------------------------------------------
void vScposSetMuteColor(UINT8 bPath, UINT32 u4Bg)
{
    // Mute color is changed to 12 bits, convert 8-bit into 12 bits as "00xxxxxxxx00"
    UINT16 u2R, u2G, u2B;

    u2R = ((u4Bg >> 16) & 0xFF) << 2;
    u2G = ((u4Bg >> 8) & 0xFF) << 2;
    u2B = ((u4Bg & 0xFF)) << 2;

    LOG(3, "vScposSetMuteColor(%d) %d %d %d\n", bPath, u2R, u2G, u2B);
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(MUTE_02, u2R, R_MUTE_MAIN);
        vRegWriteFldAlign(MUTE_01, u2G, G_MUTE_MAIN);
        vRegWriteFldAlign(MUTE_01, u2B, B_MUTE_MAIN);
        #ifdef CC_MT5396
        vRegWriteFldAlign(MUTE_06, u2R, R_MUTE_MAIN_MJC);
        vRegWriteFldAlign(MUTE_05, u2G, G_MUTE_MAIN_MJC);
        vRegWriteFldAlign(MUTE_05, u2B, B_MUTE_MAIN_MJC);
        #endif
    }
    #if SUPPORT_POP
    else
    {
        vRegWriteFldAlign(MUTE_03, u2R, R_MUTE_PIP);
        vRegWriteFldAlign(MUTE_03, u2G, G_MUTE_PIP);
        vRegWriteFldAlign(MUTE_02, u2B, B_MUTE_PIP);
    }
    #endif
}

//-----------------------------------------------------------------------------

void vScpipOutRGBColorOnOff(UINT8 bOnOff)
{
    switch (bOnOff)
    {
    case SV_ON:
        vDrvMainMute(SV_ON);
        vScposSetMuteColor(SV_VP_MAIN, 0);
        vDrvGammaMute(0, 0, 0);
        break;

    case SV_OFF:
        vDrvMainMute(SV_OFF);
        vDrvGammaUnmute();
        break;

    default:
        break;
    }
}

void vScpipOutRGBColor(UINT8 RValue,UINT8 GValue,UINT8 BValue)
{
    vDrvGammaMute(((UINT16)RValue)<<4, ((UINT16)GValue)<<4, ((UINT16)BValue)<<4);
}

static void vDrvSetOSTGOutPatternSel(UINT32 u4PatType,UINT32 u4PatSpd)
{
    vRegWriteFldAlign(OSTG_PTGEN_00, u4PatType, FLD_PATGEN_SEL);   
    vRegWriteFldAlign(OSTG_PTGEN_00, u4PatSpd, FLD_PATGEN_RSEL);
}

static void vDrvSetOSTGOutPatternEn(UINT32 u4OnOff)
{
    if (u4OnOff == SV_ON)
    {
         vRegWriteFldAlign(OSTG_PTGEN_00, 1, FLD_PATGEN_EN);
    }
    else
    {
         vRegWriteFldAlign(OSTG_PTGEN_00, 0, FLD_PATGEN_EN);
    }
}

void vDrvSetOSTGOutPatternChg(UINT32 u4OnOff,UINT32 u4PatType,UINT32 u4PatSpd)
{
    vDrvSetOSTGOutPatternSel(u4PatType,u4PatSpd);
    vDrvSetOSTGOutPatternEn(u4OnOff);
}

void vDrvSetOSTGOutPattern(UINT32 u4OnOff)
{
    vDrvSetOSTGOutPatternSel(0,0);
    vDrvSetOSTGOutPatternEn(u4OnOff);
}

void vDrvSetOSTGInPattern(UINT32 u4VdpId, UINT32 u4OnOff)
{
    if (u4VdpId == SV_VP_MAIN)
    {
        vRegWriteFldAlign(YCV_OFFSET_00, (u4OnOff? 1: 0), REG_PTGEN_MAIN_EN);
    }
    else
    {
        vRegWriteFldAlign(YCV_OFFSET_00, (u4OnOff? 1: 0), REG_PTGEN_PIP_EN);
    }
}

void vOSTGSetBorderOnOff(UINT32 u4VdpId, UINT32 u4OnOff)
{
    if (u4VdpId == SV_VP_MAIN)
    {
        vRegWriteFldAlign(WINDOW_00, (u4OnOff? 1: 0), REG_WINDOW_1_ON);
    }
    else
    {
        vRegWriteFldAlign(WINDOW_00, (u4OnOff? 1: 0), REG_WINDOW_2_ON);
    }
}

void vOSTGSetBorderColor(UINT32 u4VdpId, UINT32 u4RGB)
{
    UINT32 u4Red, u4Green, u4Blue;

    u4Red = ((u4RGB >> 16) & 0xFF) << 2;
    u4Green = ((u4RGB >> 8) & 0xFF) << 2;
    u4Blue = (u4RGB & 0xFF) << 2;
    LOG(3, "vOSTGSetBorderColor(%d) %d %d %d\n", u4VdpId, u4Red, u4Green, u4Blue);

    u4RGB = (u4Red << 20) + (u4Green << 10) + u4Blue;
    if (u4VdpId == SV_VP_MAIN)
    {
        vRegWriteFldAlign(WINDOW_00, u4RGB, WINDOW_COLOR1);
    }
    else
    {
        vRegWriteFldAlign(WINDOW_01, u4RGB, WINDOW_COLOR2);
    }

}

void vOSTGSetBorderParam(UINT32 u4VdpId, UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 u4Thickness)
{
    UINT32 u4W, u4H;
    UINT32 u4XOffset, u4YOffset;

    if ((u4X + u4Width) > PANEL_GetPanelWidth())
    {
        u4W = PANEL_GetPanelWidth() - u4X;
    }
    else
    {
        u4W = u4Width;
    }
    if ((u4Y + u4Height) > PANEL_GetPanelHeight())
    {
        u4H = PANEL_GetPanelHeight() - u4Y;
    }
    else
    {
        u4H = u4Height;
    }

    // conver x, y index to internal addressing
    //@6896 review this ==> u4XOffset = RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_MOSYNCLEN);
    u4XOffset = 0; //FIXME
    u4YOffset = RegReadFldAlign(OSTG_PTGEN_00, FLD_VSYNC_DELSEL2);
    u4XOffset += u4X;
    u4YOffset += u4Y;
    
    if (u4VdpId == SV_VP_MAIN)
    {
        vRegWriteFldMulti(WINDOW_02, 
            P_Fld(u4XOffset+u4Thickness, WINDOW_1_HPOSLR)|P_Fld(u4XOffset, WINDOW_1_HPOSLL));
        vRegWriteFldMulti(WINDOW_03, 
            P_Fld(u4XOffset+u4W+u4Thickness, WINDOW_1_HPOSRR)|P_Fld(u4XOffset+u4W, WINDOW_1_HPOSRL));

        vRegWriteFldMulti(WINDOW_04, 
            P_Fld(u4YOffset+u4Thickness, WINDOW_1_VPOSUD)|P_Fld(u4YOffset, WINDOW_1_VPOSUD));
        vRegWriteFldMulti(WINDOW_05, 
            P_Fld(u4YOffset+u4H+u4Thickness, WINDOW_1_VPOSDD)|P_Fld(u4YOffset+u4H, WINDOW_1_VPOSDD));
    }
    else
    {
        vRegWriteFldMulti(WINDOW_06, 
            P_Fld(u4XOffset+u4Thickness, WINDOW_2_HPOSLR)|P_Fld(u4XOffset, WINDOW_2_HPOSLL));
        vRegWriteFldMulti(WINDOW_07, 
            P_Fld(u4XOffset+u4W+u4Thickness, WINDOW_2_HPOSRR)|P_Fld(u4XOffset+u4W, WINDOW_2_HPOSRL));

        vRegWriteFldMulti(WINDOW_08, 
            P_Fld(u4YOffset+u4Thickness, WINDOW_1_VPOSUD)|P_Fld(u4YOffset, WINDOW_1_VPOSUD));
        vRegWriteFldMulti(WINDOW_09, 
            P_Fld(u4YOffset+u4H+u4Thickness, WINDOW_2_VPOSDD)|P_Fld(u4YOffset+u4H, WINDOW_2_VPOSDD));
    }

}

void vOSTGSetBorderForMask(UINT32 u4VdpId, UINT32 u4HPosLR, UINT32 u4HPosLL, UINT32 u4HPosRR, UINT32 u4HPosRL, UINT32 u4HPosUD, UINT32 u4HPosUU, UINT32 u4HPosDD, UINT32 u4HPosDU)
{    
    if (u4VdpId == SV_VP_MAIN)
    {
        vRegWriteFldMulti(WINDOW_02, 
            P_Fld(u4HPosLR, WINDOW_1_HPOSLR)|P_Fld(u4HPosLL, WINDOW_1_HPOSLL));
        vRegWriteFldMulti(WINDOW_03, 
            P_Fld(u4HPosRR, WINDOW_1_HPOSRR)|P_Fld(u4HPosRL, WINDOW_1_HPOSRL));

        vRegWriteFldMulti(WINDOW_04, 
            P_Fld(u4HPosUD, WINDOW_1_VPOSUD)|P_Fld(u4HPosUU, WINDOW_1_VPOSUU));
        vRegWriteFldMulti(WINDOW_05, 
            P_Fld(u4HPosDD, WINDOW_1_VPOSDD)|P_Fld(u4HPosDU, WINDOW_1_VPOSDU));
    }
    else
    {
        vRegWriteFldMulti(WINDOW_06, 
            P_Fld(u4HPosLR, WINDOW_2_HPOSLR)|P_Fld(u4HPosLL, WINDOW_2_HPOSLL));
        vRegWriteFldMulti(WINDOW_07, 
            P_Fld(u4HPosRR, WINDOW_2_HPOSRR)|P_Fld(u4HPosRL, WINDOW_2_HPOSRL));

        vRegWriteFldMulti(WINDOW_08, 
            P_Fld(u4HPosUD, WINDOW_2_VPOSUD)|P_Fld(u4HPosUU, WINDOW_2_VPOSUU));
        vRegWriteFldMulti(WINDOW_09, 
            P_Fld(u4HPosDD, WINDOW_2_VPOSDD)|P_Fld(u4HPosDU, WINDOW_2_VPOSDU));
    }

}

