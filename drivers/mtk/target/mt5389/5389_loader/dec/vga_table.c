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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: vga_table.c $
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 *   Driver for mode detection
 *
 * Author:
 * -------
 *
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vga_table.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#define _VGA_TABLE_C_

#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "vga_table.h"

#define VGAMODE_OFFSET 0
#define VGACAPTURE_OFFSET (VGAMODE_OFFSET + (UINT16)MAX_TIMING_FORMAT * sizeof(VGAMODE))	// 79 * 14 = 1106

#define ADCPLL_WORKAROUND   1
#define OVERSAMPLE_THRESHOLD    250//350 //25MHz

#if SUPPORT_VGA_USERMODE
VGA_USRMODE rVgaUsrEEP[USERMODE_TIMING];	    //both  EEP & RAM
VGA_USRMODE_EXT rVgaUsrExt[USERMODE_TIMING]; //only on RAM
#endif
////////////////////////////////////////////////////////////////////////////////

UINT16 Get_VGAMODE_IHF(UINT8 mode)
{
	return VGATIMING_TABLE[mode].IHF;
}

UINT8 Get_VGAMODE_IVF(UINT8 mode)
{
	return VGATIMING_TABLE[mode].IVF;
}

UINT16 Get_VGAMODE_ICLK(UINT8 mode)
{
#if ADCPLL_WORKAROUND
       if(fgIsVgaTiming(mode) && (VGATIMING_TABLE[mode].ICLK < OVERSAMPLE_THRESHOLD))
       {
	    return (VGATIMING_TABLE[mode].ICLK*2);
       }
       else
#endif       
       {
	    return VGATIMING_TABLE[mode].ICLK;
       }
}

UINT16 Get_VGAMODE_IHTOTAL(UINT8 mode)
{
#if ADCPLL_WORKAROUND
        if(fgIsVgaTiming(mode) && (VGATIMING_TABLE[mode].ICLK < OVERSAMPLE_THRESHOLD))
        {
	        return (VGATIMING_TABLE[mode].IHTOTAL*2);        
        }
        else
#endif        
        {
	        return VGATIMING_TABLE[mode].IHTOTAL;
        }
}

UINT16 Get_VGAMODE_IVTOTAL(UINT8 mode)
{
	return VGATIMING_TABLE[mode].IVTOTAL;
}

UINT16 Get_VGAMODE_IPH_STA(UINT8 mode)
{
#if ADCPLL_WORKAROUND
        if(fgIsVgaTiming(mode) && (VGATIMING_TABLE[mode].ICLK < OVERSAMPLE_THRESHOLD))
        {
	        return ((VGATIMING_TABLE[mode].IPH_BP+VGATIMING_TABLE[mode].IPH_SYNCW)*2);        
        }
        else
#endif        
        {
	        return (VGATIMING_TABLE[mode].IPH_BP+VGATIMING_TABLE[mode].IPH_SYNCW) ;
        }
}

UINT16 Get_VGAMODE_IPH_WID(UINT8 mode)
{
#if ADCPLL_WORKAROUND
        if(fgIsVgaTiming(mode) && (VGATIMING_TABLE[mode].ICLK < OVERSAMPLE_THRESHOLD))
        {
	        return (VGATIMING_TABLE[mode].IPH_WID*2);        
        }
        else
#endif        
        {
	        return VGATIMING_TABLE[mode].IPH_WID;
        }
}

UINT16 Get_VGAMODE_IPH_SYNCW(UINT8 mode)
{
#if ADCPLL_WORKAROUND
        if(fgIsVgaTiming(mode) && (VGATIMING_TABLE[mode].ICLK < OVERSAMPLE_THRESHOLD))
        {
	        return (VGATIMING_TABLE[mode].IPH_SYNCW*2);        
        }
        else
#endif        
        {
	        return VGATIMING_TABLE[mode].IPH_SYNCW;
        }
}

UINT16 Get_VGAMODE_IPH_BP(UINT8 mode)
{
#if ADCPLL_WORKAROUND
        if(fgIsVgaTiming(mode) && (VGATIMING_TABLE[mode].ICLK < OVERSAMPLE_THRESHOLD))
        {
	        return (VGATIMING_TABLE[mode].IPH_BP*2);        
        }
        else
#endif        
        {
	        return VGATIMING_TABLE[mode].IPH_BP;
        }
}

UINT8 Get_VGAMODE_IPV_STA(UINT8 mode)
{
	return VGATIMING_TABLE[mode].IPV_STA; //-Displaymode_delay; //Modify for Disaplymode delay
}

UINT16 Get_VGAMODE_IPV_LEN(UINT8 mode)
{
	return VGATIMING_TABLE[mode].IPV_LEN;
}

UINT16 Get_VGAMODE_COMBINE(UINT8 mode)
{
	return VGATIMING_TABLE[mode].COMBINE;
}

 UINT8 Get_VGAMODE_OverSample(UINT8 mode)
 {
#if ADCPLL_WORKAROUND 
        if(fgIsVgaTiming(mode) && (VGATIMING_TABLE[mode].ICLK < OVERSAMPLE_THRESHOLD))
        {
	        return 1;        //alwasy oversample
        }
        else
#endif        
        {
	        return (VGATIMING_TABLE[mode].COMBINE &0x01);
	 }  
 }


////////////////////////////////////////////////////////////////////////////////
