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
#ifdef CC_MT5395
#include "drv_mjc.h"
#include "drv_mjc_if.h"
#endif


//-----------------------------------------------------------------------------
/** Brief of vScposSetMuteColor.
 * Set Background Color for Main and PIP Channel
 */
//-----------------------------------------------------------------------------
void vScposSetMuteColor(UINT8 bPath, UINT32 u4Bg)
{
//Mute color in 5362B were changed to 12 bits, we need to set 8bit into 12 bits as "00xxxxxxxx00"
    UINT16 u2Red, u2Green, u2Blue;

    u2Red = ((u4Bg >> 16) & 0xFF) << 2;
    u2Green = ((u4Bg >> 8) & 0xFF) << 2;
    u2Blue = (u4Bg & 0xFF) << 2;

    #ifdef CC_MT5395
    if (bPath == SV_VP_MAIN)
    {
        vDrvMJCSetMuteColor(u2Red >> 2, u2Green >> 2, u2Blue >> 2);
    }
    #endif
    
     LOG(3, "vScposSetMuteColor(%d) %d %d %d\n", bPath, u2Red, u2Green, u2Blue);
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OUTSTG_OS_1C, u2Red, OS_1C_R_MUTE_MAIN);
        vRegWriteFldAlign(OUTSTG_OS_1B, u2Green, OS_1B_G_MUTE_MAIN);
        vRegWriteFldAlign(OUTSTG_OS_1B, u2Blue, OS_1B_B_MUTE_MAIN);
    }
#if SUPPORT_POP
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_1D, u2Red, OS_1D_R_MUTE_PIP);
        vRegWriteFldAlign(OUTSTG_OS_1D, u2Green, OS_1D_G_MUTE_PIP);
        vRegWriteFldAlign(OUTSTG_OS_1C, u2Blue, OS_1C_B_MUTE_PIP);
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
        vRegWriteFldAlign(OS_26,1, GMT_WEN);
        vScposSetMuteColor(SV_VP_MAIN, 0);
        break;

    case SV_OFF:
        vDrvMainMute(SV_OFF);
        vRegWriteFldAlign(OS_26, 1, GMTWMODE);

        vRegWriteFldAlign(OS_26, 0, GAMMA_CS);

        vRegWriteFldAlign(OS_22, 0, N_GM_WADD);

        vRegWriteFldAlign(OS_22, 0, N_GM_WDATA_R);
        vRegWriteFldAlign(OS_23, 0, N_GM_WDATA_G);
        vRegWriteFldAlign(OS_23, 0, N_GM_WDATA_B);        

        vRegWriteFldAlign(OS_26, 7, GAMMA_CS);

        vRegWriteFldAlign(OS_22, 0, N_GM_WDATA_R);
        vRegWriteFldAlign(OS_23, 0, N_GM_WDATA_G);
        vRegWriteFldAlign(OS_23, 0, N_GM_WDATA_B); 

        vRegWriteFldAlign(OS_26, 0, GMT_WEN);
        vRegWriteFldAlign(OS_26, 0, GMTWMODE);
        break;

    default:
        break;
    }
}

void vScpipOutRGBColor(UINT16 RValue, UINT16 GValue, UINT16 BValue)
{
    //Set RValue

    vRegWriteFldAlign(OS_26, 0, GAMMA_CS);

    vRegWriteFldAlign(OS_26, 2, GMTWMODE);

    vRegWriteFldAlign(OS_22, 0, N_GM_WADD);

    vRegWriteFldAlign(OS_22, RValue, N_GM_WDATA_R);

    vRegWriteFldAlign(OS_26, 7, GAMMA_CS);

    vRegWriteFldAlign(OS_22, RValue, N_GM_WDATA_R);

    vRegWriteFldAlign(OS_26, 0, GAMMA_CS);


    //Set GValue
    vRegWriteFldAlign(OS_26, 3, GMTWMODE);

    vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
    vRegWriteFldAlign(OS_23, GValue, N_GM_WDATA_G);

    vRegWriteFldAlign(OS_26, 7, GAMMA_CS);

    vRegWriteFldAlign(OS_23, GValue, N_GM_WDATA_G);

    vRegWriteFldAlign(OS_26, 0, GAMMA_CS);

    //Set BValue
    vRegWriteFldAlign(OS_26, 4, GMTWMODE);

    vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
    vRegWriteFldAlign(OS_23, BValue, N_GM_WDATA_B);
    vRegWriteFldAlign(OS_26, 7, GAMMA_CS);

    vRegWriteFldAlign(OS_23, BValue, N_GM_WDATA_B);
}


void vDrvSetOSTGOutPattern(UINT32 u4OnOff)
{
    if (u4OnOff == SV_ON)
    {
         vRegWriteFldAlign(OUTSTG_OS_29, 1, OS_29_PATGEN_EN);
    }
    else
    {
         vRegWriteFldAlign(OUTSTG_OS_29, 0, OS_29_PATGEN_EN);
    }
}


void vDrvSetOSTGInPattern(UINT32 u4VdpId, UINT32 u4OnOff)
{
    if (u4VdpId == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OUTSTG_OS_26, (u4OnOff? 1: 0), OS_26_MAIN_PTGEN_EN);
    }
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_26, (u4OnOff? 1: 0), OS_26_PIP_PTGEN_EN);
    }

}

void vOSTGSetBorderOnOff(UINT32 u4VdpId, UINT32 u4OnOff)
{
    if (u4VdpId == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OUTSTG_OS_08, (u4OnOff? 1: 0), OS_08_REG_VBOUND_1_ON);
    }
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_08, (u4OnOff? 1: 0), OS_08_REG_VBOUND_2_ON);
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
        vRegWriteFldAlign(OUTSTG_OS_09, u4RGB, OS_09_REG_BOUND_CLO1);
    }
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_0A, u4RGB, OS_0A_REG_BOUND_CLO2);
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
#ifdef CC_MT5395
    if( u1DrvMJCIsOnOff() != FALSE)
    {
        u4XOffset = (u2DrvMJCGetOutputHTotal()-1) - wDISPLAY_WIDTH;
    }
    else
#endif
    {
        u4XOffset = RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_MOSYNCLEN);
    }
    u4YOffset = RegReadFldAlign(OUTSTG_OS_75, OS_75_VSYNC_DELSEL2);
    u4XOffset += u4X;
    u4YOffset += u4Y;
    
    if (u4VdpId == SV_VP_MAIN)
    {
        vRegWriteFldMulti(OUTSTG_OS_0B, 
            P_Fld(u4XOffset, OS_0B_REG_VBOUND1_HPOS)|P_Fld(u4YOffset, OS_0B_REG_VBOUND1_VPOS)|P_Fld(u4Thickness, OS_0B_REG_VBOUND1_HWIDTH));
        vRegWriteFldMulti(OUTSTG_OS_0C, 
            P_Fld(u4Thickness, OS_0C_REG_VBOUND1_VWIDTH)|P_Fld(u4W, OS_0C_REG_VBOUND1_HLEN)|P_Fld(u4H, OS_0C_REG_VBOUND1_VLEN));
    }
    else
    {
        vRegWriteFldMulti(OUTSTG_OS_0D, 
            P_Fld(u4XOffset, OS_0D_REG_VBOUND2_HPOS)|P_Fld(u4YOffset, OS_0D_REG_VBOUND2_VPOS)|P_Fld(u4Thickness, OS_0D_REG_VBOUND2_HWIDTH));
        vRegWriteFldMulti(OUTSTG_OS_0E, 
            P_Fld(u4Thickness, OS_0E_REG_VBOUND2_VWIDTH)|P_Fld(u4W, OS_0E_REG_VBOUND2_HLEN)|P_Fld(u4H, OS_0E_REG_VBOUND2_VLEN));
    }

}


