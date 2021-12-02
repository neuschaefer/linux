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
 * $RCSfile: vga_table.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _VGA_TABLE_H_
#define _VGA_TABLE_H_

#include "general.h"

#define CO(_PRO_INT, _DIV_MUL, _ADCPLLDIV, _CLKIN_MCODE) ((_PRO_INT<<6) | ((_DIV_MUL>>1) <<5) |((_ADCPLLDIV>>1)<<4) |_CLKIN_MCODE)
#define Displaymode_delay 0 //Modify for Disaplymode delay

#ifndef _VGAMODE_STRUCT_
#define _VGAMODE_STRUCT_
typedef struct VGAMODE		// 14 bytes
{
	UINT16 IHF; // Horizontal Frequency for timing search
	UINT8 IVF; // Vertical Frequency for timing search
	UINT16 ICLK; // Pixel Frequency
	UINT16 IHTOTAL; // H Total
	UINT16 IVTOTAL; // V Total 
	UINT16 IPH_SYNCW; // H Sync Width
	UINT16 IPH_WID; // H Resolution
	UINT16 IPH_BP; // H Back Porch
	UINT16 IPV_STA; // V Back Porch + Sync Width
	UINT16 IPV_LEN; // V Resolution
	UINT16 COMBINE; // ??
	//UINT32 CLKIN_CWCODE; // ??
}  VGAMODE ; // using __attribute__((packed)) make armcc --gnu  internal fault ?
#endif


extern const UINT8 bHdtvTimings;
extern const UINT8 bVgaTimings;
extern const UINT8 bUserVgaTimings;
extern const UINT8 bAllTimings;

extern const UINT8 bUserVgaTimingBegin;

#if SUPPORT_VGA_USERMODE
#define  USERMODE_TIMING 8
typedef struct VGA_USRMODE	
{
	UINT16 vlen:11;	/* reference STA12 */
	UINT16 hsync_wvar:4;
	UINT16 vpol:1;	/* reference STA12 */
	UINT16 hlen:11;	/* reference STA12 */
	UINT16 hlen_var:4;
	UINT16 hpol:1;	/* reference STA12 */
	UINT8 hsync_w;	
	UINT8 isCVTRB:1;
	UINT8 unused:3;
	UINT8 forceXY:2;	/* force XY resolution*/
	UINT8 override:1; /* override preset timing */
	UINT8 id:1;		/*inverse to change return mode number */
} VGA_USRMODE;

typedef struct VGA_USRMODE_EXT
{
	UINT16	h_res;	//orig input h resolotion
} VGA_USRMODE_EXT;

extern VGA_USRMODE rVgaUsrEEP[USERMODE_TIMING];		//both  EEP & RAM
extern VGA_USRMODE_EXT rVgaUsrExt[USERMODE_TIMING]; //only on RAM
#endif


#ifdef CC_UP8032_ATV
extern VGAMODE code VGATIMING_TABLE[];
#else
extern VGAMODE  VGATIMING_TABLE[];
#endif




// VGA Mode
extern UINT16 Get_VGAMODE_IHF(UINT8 mode) ;
extern UINT8 Get_VGAMODE_IVF(UINT8 mode) ;
extern UINT16 Get_VGAMODE_ICLK(UINT8 mode) ;
extern UINT16 Get_VGAMODE_IHTOTAL(UINT8 mode) ;
extern UINT16 Get_VGAMODE_IVTOTAL(UINT8 mode) ;
extern UINT16 Get_VGAMODE_IPH_STA(UINT8 mode) ;
extern UINT16 Get_VGAMODE_IPH_SYNCW(UINT8 mode) ;
extern UINT16 Get_VGAMODE_IPH_WID(UINT8 mode) ;
extern UINT16 Get_VGAMODE_IPH_BP(UINT8 mode) ;
extern UINT8 Get_VGAMODE_IPV_STA(UINT8 mode) ;
extern UINT16 Get_VGAMODE_IPV_LEN(UINT8 mode) ;
extern UINT16 Get_VGAMODE_COMBINE(UINT8 mode) ;
extern UINT8 Get_VGAMODE_OverSample(UINT8 mode) ;


//#define Get_VGAMODE_OverSample(bMode) (Get_VGAMODE_COMBINE(bMode)&0x01) //oversample
#define Get_VGAMODE_HSyncWidthChk(bMode) ((Get_VGAMODE_COMBINE(bMode)>>1)&0x01)
#define Get_VGAMODE_AmbiguousH(bMode) ((Get_VGAMODE_COMBINE(bMode)>>2)&0x01)
#define Get_VGAMODE_VPol(bMode) ((Get_VGAMODE_COMBINE(bMode)>>3)&0x01)
#define Get_VGAMODE_HPol(bMode) ((Get_VGAMODE_COMBINE(bMode)>>4)&0x01)
#define Get_VGAMODE_VSyncWidthChk(bMode) ((Get_VGAMODE_COMBINE(bMode)>>5)&0x01)
#define Get_VGAMODE_INTERLACE(bMode) ((Get_VGAMODE_COMBINE(bMode)>>6)&0x01)
#define Get_VGAMODE_PolChk(bMode) ((Get_VGAMODE_COMBINE(bMode)>>7)&0x01)
#define Get_HDMIMODE_SupportVideo(bMode) ((Get_VGAMODE_COMBINE(bMode)>>8)&0x01)
#define Get_VGAMODE_VgaDisabled(bMode) ((Get_VGAMODE_COMBINE(bMode)>>9)&0x01)
#define Get_VGAMODE_YpbprDisabled(bMode) ((Get_VGAMODE_COMBINE(bMode)>>10)&0x01)


#endif
