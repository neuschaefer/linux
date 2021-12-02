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
 * $RCSfile: drv_tcon.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif

#ifdef CC_COPLAT_MT82
#include "util.h"
#endif

#include "general.h"

#include "x_pinmux.h"

#include "hw_ckgen.h"
#include "hw_tcon.h"
#include "hw_lvds.h"
#include "hw_mlvds.h"
#include "hw_tg.h"

#include "drv_tcon.h"
#include "drv_vdoclk.h"
#include "drv_lvds.h"


#include "sif_if.h"
#include "panel.h"
#include "drv_display.h"

#include "nptv_debug.h"

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#define TCON_ADDR_BASE 0xf0000000

static UINT8 _u1TconCmd;
static UINT8 _u1GammaReady;
static UINT8 _u1TconReady;
static UINT8 _u1TconForceMute;
static UINT8 _u1TconState;
static UINT8 _u1TconStateOld;

// PCB settings
#define M1V1
//#define P1V1

// Target panel

#define LGDV4
//#define LGDV5GIP
//#define LGDV6GIP

//#define CMO2011
//#define HIWXGA
//#define CHILINFHD
//#define AUOFHD
//#define SHAFHD

//miniLVDS
struct sTCONREGTABLE sRegMini[] = {

  // ckgen
//  0xf000d1a0, 0x0f28a820, 0xffffffff,
//  0xf000d1a4, 0x0ec8186e, 0xffffffff,
  0xf000d1b4, 0x106F0000, 0xffffffff,

#if defined (CC_MT5395)

  //minilvds PIN select
  0xf0035104, 0x2100A560, 0xffffffff,
  0xf0035108, 0x1200A560, 0xffffffff,

  //minilvds reset pulse
  0xf003511c, 0x00000000, 0xffffffff,
  0xf0035120, 0x00000000, 0xffffffff,
  0xf0035100, 0x0000003f, 0xffffffff,
  0xf003510c, 0x04049898, 0xffffffff,  

  //minilvds PN setting
  0xf0035130, 0x00000000, 0xffffffff,

#elif defined (CC_MT5365)

#if defined (M1V1)   // MT5366M1V1
  // minilvds PIN select
  0xf0035104, 0x01200546, 0xffffffff,
  0xf0035108, 0x01020465, 0xffffffff,
  
  #if defined(SHAFHD)
  //minilvds reset pulse: invert
  0xf003511c, 0xffffffff, 0xffffffff,
  0xf0035120, 0xffffffff, 0xffffffff,
  0xf0035100, 0xffffff3f, 0xffffffff,
  
  //minilvds PN setting
  0xf0035130, 0xbffff7ab, 0xffffffff,
  
  #elif defined(CHILINFHD)
  //minilvds reset pulse: invert
  0xf003511c, 0xffffffff, 0xffffffff,
  0xf0035120, 0xffffffff, 0xffffffff,
  0xf0035100, 0xff0000c0, 0xffffffff,
  
	  //minilvds PN setting
	  0xf0035130, 0xf9fff3ab, 0xffffffff,   // clock not invert
  #else
	  //minilvds reset pulse
	  0xf003511c, 0x00000000, 0xffffffff,
	  0xf0035120, 0x00000000, 0xffffffff,
	  0xf0035100, 0x0000003f, 0xffffffff,
	  
	  //minilvds PN setting
  0xf0035130, 0x00000054, 0xffffffff,
  #endif
  
#elif defined(P1V1) // MT5365p1V1
  //minilvds PIN select
  0xf0035104, 0x00120456, 0xffffffff,
  0xf0035108, 0x00120456, 0xffffffff,
    
  //minilvds reset pulse
  0xf003511c, 0x00000000, 0xffffffff,
  0xf0035120, 0x00000000, 0xffffffff,
  0xf0035100, 0x0000003f, 0xffffffff,
	  
  //minilvds PN setting
  0xf0035130, 0x00000000, 0xffffffff,
#endif

#endif
	  
  // mute pattern
  0xf0035114, 0x0000cFF2, 0xffffffff,
  

  // Bit / RL swap :
  0xf0035110, 0x00100000, 0xffffffff,

  // output enable
  0xf0035118, 0x00000000, 0xffffffff,

  // clk source
  #if defined(CHILINFHD) || defined(AUOFHD) || defined(SHAFHD)
  0xf0035000, 0x4002a259, 0xffffffff,
  #elif defined(CMO2011)
  0xf0035000, 0x4002B359, 0xffffffff,
  #else   // lgdv4,v5
  0xf0035000, 0x4002a359, 0xffffffff,
  #endif

  // minilvds input source
  0xf0035280, 0x80000000, 0xffffffff,
  
  // mute pattern H/V total, WXGA
#ifdef HIWXGA
  0xf0035004, 0x05580647, 0xffffffff,
  0xf0035008, 0x03000329, 0xffffffff,
#else
  0xf0035004, 0x07800898, 0xffffffff,
  0xf0035008, 0x04380465, 0xffffffff,
#endif

  #if  HISENSE_IPS_2010_PANEL 
  
  0xf0035004, 0x05560640, 0xffffffff,//0xf0035004, 0x07800898, 0xffffffff,
  0xf0035008, 0x03000326, 0xffffffff,//  0xf0035008, 0x04380465, 0xffffffff,
  0xf0035104, 0x00120456, 0xffffffff,
  0xf0035108, 0x00120456, 0xffffffff,
    // mute pattern
  //0xf0035114, 0x0000cFF2, 0xffffffff,
  0xf0035114, 0x0000cFF0, 0xffffffff,
  //minilvds PN setting
  0xf0035130, 0x00000000, 0xffffffff,

    // minilvds input source
  0xf0035280, 0x82AA0028, 0xffffffff,//for middle line of IPS2010 Panel
  #else
 0xf0035004, 0x07800898, 0xffffffff,
 0xf0035008, 0x04380465, 0xffffffff,
  #endif

  0xf003500c, 0x00400006, 0xffffffff,
  0xf0035010, 0x00100001, 0xffffffff,
  0xf0035014, 0x03ff0000, 0xffffffff,
  0xf0035018, 0x01e00000, 0xffffffff,
  0xf003501c, 0x00f0000c, 0xffffffff,
  0xf0035020, 0x00000000, 0xffffffff,
  0xf0035128, 0x01f901df, 0xffffffff,
  0xf003512c, 0x80000000, 0xffffffff,

  0xf0035284, 0x00000000, 0xffffffff, 
  0xf0035200, 0x00000000, 0xffffffff, 
  0xf0035204, 0x00000000, 0xffffffff, 
  0xf0035208, 0x00000000, 0xffffffff, 
  0xf003520c, 0x00000000, 0xffffffff, 
  0xf0035210, 0x00000000, 0xffffffff, 
  0xf0035214, 0x00000000, 0xffffffff, 
  0xf0035218, 0x00000000, 0xffffffff, 
  0xf003521c, 0x00000000, 0xffffffff, 
  0xf0035220, 0x00010000, 0xffffffff, 
  0xf0035224, 0x12c10001, 0xffffffff, 
  0xf0035228, 0x12c10001, 0xffffffff, 
  0xf003522c, 0x20000000, 0xffffffff
};

//tg
struct sTCONREGTABLE sRegTg[] = {

  // GR0
  0xf0034800, 0x00000000, 0xffffffff,  

  // GR1
  //0xf0034804, 0x00000000, 0xffffffff,

  // GR7
  0xf003481c, 0x00000000, 0xffffffff,

//////////////////////////////////////////////////// mt5395
#if defined (CC_MT5395)

  #if defined (LGDV4)
  // SOE, TG0
  0xf0034820, 0x00C807d0, 0xffffffff,
  0xf0034824, 0x00000000, 0xffffffff,
  0xf0034828, 0x00C807d0, 0xffffffff,  
  0xf003482c, 0x04430007, 0xffffffff,
  0xf0034830, 0x20000000, 0xffffffff,
  0xf0034834, 0x80000000, 0xffffffff,
  0xf0034838, 0x00000007, 0xffffffff,
  0xf003483c, 0x00000000, 0xffffffff,

  // POL, TG1
  0xf0034840, 0x07080708, 0xffffffff,
  0xf0034844, 0x00000000, 0xffffffff,
  0xf0034848, 0x07080708, 0xffffffff,  
  0xf003484c, 0x04430007, 0xffffffff,
  0xf0034850, 0x80000404, 0xffffffff,
  0xf0034854, 0x80020004, 0xffffffff,
  0xf0034858, 0x00000007, 0xffffffff,
  0xf003485c, 0x10000000, 0xffffffff,

  // GSP, TG11
  0xf0034980, 0x04060406, 0xffffffff,
  0xf0034984, 0x00000000, 0xffffffff,
  0xf0034988, 0x00000000, 0xffffffff,
  0xf003498c, 0x00080007, 0xffffffff,
  0xf0034990, 0x00000000, 0xffffffff,
  0xf0034994, 0x00000000, 0xffffffff,
  0xf0034998, 0x00000004, 0xffffffff,
  0xf003499c, 0x00000000, 0xffffffff,

  // GSP_R, TG8
  0xf0034920, 0x04060406, 0xffffffff,
  0xf0034924, 0x00000000, 0xffffffff,
  0xf0034928, 0x00000000, 0xffffffff,
  0xf003492c, 0x00080007, 0xffffffff,
  0xf0034930, 0x00000000, 0xffffffff,
  0xf0034934, 0x00000000, 0xffffffff,
  0xf0034938, 0x00000004, 0xffffffff,
  0xf003493c, 0x00000000, 0xffffffff,

  // GOE, TG2
  0xf0034860, 0x07BC062C, 0xffffffff,
  0xf0034864, 0x00000000, 0xffffffff,
  0xf0034868, 0x07BC062C, 0xffffffff,
  0xf003486c, 0x04420006, 0xffffffff,
  0xf0034870, 0x20000000, 0xffffffff,
  0xf0034874, 0x80000000, 0xffffffff,
  0xf0034878, 0x00000007, 0xffffffff,
  0xf003487c, 0x00000000, 0xffffffff,
  
  // GSC, TG12
  0xf00349a0, 0x07260406, 0xffffffff,
  0xf00349a4, 0x00000000, 0xffffffff,
  0xf00349a8, 0x07260406, 0xffffffff,
  0xf00349ac, 0x04420006, 0xffffffff,
  0xf00349b0, 0x20000000, 0xffffffff,
  0xf00349b4, 0x80000000, 0xffffffff,
  0xf00349b8, 0x00000107, 0xffffffff,
  0xf00349bc, 0x00000000, 0xffffffff,
   
  // FLK, TG9  
  0xf0034940, 0x049C0726, 0xffffffff,
  0xf0034944, 0x00000000, 0xffffffff,
  0xf0034948, 0x049C0726, 0xffffffff,
  0xf003494c, 0x04420006, 0xffffffff,
  0xf0034950, 0x20000000, 0xffffffff,
  0xf0034954, 0x80000000, 0xffffffff,
  0xf0034958, 0x00000007, 0xffffffff,
  0xf003495c, 0x00000000, 0xffffffff,
  
  // DPM, TG10
  0xf0034978, 0x00000030, 0xffffffff,
  
  // H_CONV, TG5
  0xf00348d8, 0x00000020, 0xffffffff,

  // OPT_N, TG6
  0xf00348f8, 0x00000030, 0xffffffff,
  
  // 0PT_P , TG7
  0xf0034918, 0x00000020, 0xffffffff,

    #elif defined(LGDV6GIP)
  // tg
  // GR0                             																														
 // 0xf0034800, 0x8100ffff, 0xffffffff,
                                       																														
  // GR1                             																														
  0xf0034804, 0x00000000, 0xffffffff,																													
                                       																														
  // GR7                             																														
  0xf003481c, 0x00000000, 0xffffffff,																														
                               													
   // SOE, TG0
  0xf0034820, 0x003c0816, 0xffffffff,
  0xf0034824, 0x00000000, 0xffffffff,
  0xf0034828, 0x003C0816, 0xffffffff,  
  0xf003482c, 0x04440001, 0xffffffff,
  0xf0034830, 0x20000000, 0xffffffff,
  0xf0034834, 0x80000000, 0xffffffff,
  0xf0034838, 0x00000007, 0xffffffff,
  0xf003483c, 0x00000000, 0xffffffff,																													
                                       																														
  // POL, OK                    																														
  0xf0034840, 0x07D007D0, 0xffffffff,																														
  0xf0034844, 0x00000000, 0xffffffff,																														
  0xf0034848, 0x07D007D0, 0xffffffff,																														
  //0xf003484c, 0x043F0007, 0xffffffff,																														
  0xf003484c, 0x00010001, 0xffffffff,																														  
  0xf0034850, 0x80000404, 0xffffffff,																														
  //0xf0034854, 0x80020004, 0xffffffff,																														
  0xf0034854, 0x80040004, 0xffffffff,
  0xf0034858, 0x00000007, 0xffffffff,																														
  0xf003485c, 0x10000000, 0xffffffff,																														
                                       																														
  // GSP, OK                   																														
  0xf0034860, 0x01F405DC, 0xffffffff,
  0xf0034864, 0x00000000, 0xffffffff,
  0xf0034868, 0x00000000, 0xffffffff,
  0xf003486c, 0x00030001, 0xffffffff,
  0xf0034870, 0x00000000, 0xffffffff,
  0xf0034874, 0x00000000, 0xffffffff,
  0xf0034878, 0x00000004, 0xffffffff,
  0xf003487c, 0x00000000, 0xffffffff,
  																												
  // GCLK 1, // V6 GCLK																					
  0xf0034880, 0x03000280, 0xffffffff,
  0xf0034884, 0x00000000, 0xffffffff,
  0xf0034888, 0x03000280, 0xffffffff,
  0xf003488c, 0x043f0003, 0xffffffff,
  0xf0034890, 0x20000000, 0xffffffff,
  0xf0034894, 0x80000000, 0xffffffff,
  0xf0034898, 0x00000007, 0xffffffff,
  0xf003489c, 0x00000000, 0xffffffff,
  
  // GCLK 2,  // V6 MCLK		
  0xf00348A0, 0x06F006E0, 0xffffffff,
  0xf00348A4, 0x00000000, 0xffffffff,
  0xf00348A8, 0x06F006E0, 0xffffffff,
  0xf00348Ac, 0x04410005, 0xffffffff,
  0xf00348B0, 0x20000000, 0xffffffff,
  0xf00348B4, 0x80000000, 0xffffffff,
  0xf00348B8, 0x00000007, 0xffffffff,
  0xf00348Bc, 0x00000000, 0xffffffff,
  
  // GCLK 3, OK       																														
  0xf00348C0, 0x06AE083E, 0xffffffff,
  0xf00348C4, 0x00000000, 0xffffffff,
  0xf00348C8, 0x06AE083E, 0xffffffff,
  0xf00348CC, 0x04440007, 0xffffffff,
  0xf00348D0, 0x80000606, 0xffffffff,
  0xf00348D4, 0x80030006, 0xffffffff,
  0xf00348D8, 0x00000007, 0xffffffff,
  0xf00348DC, 0x00000000, 0xffffffff,
  
  // GCLK 4, OK     																														
  0xf00348E0, 0x06AE083E, 0xffffffff,
  0xf00348E4, 0x00000000, 0xffffffff,
  0xf00348E8, 0x06AE083E, 0xffffffff,
  0xf00348EC, 0x043F0002, 0xffffffff,
  0xf00348F0, 0x80000606, 0xffffffff,
  0xf00348F4, 0x80020005, 0xffffffff,
  0xf00348F8, 0x00000007, 0xffffffff,
  0xf00348FC, 0x00000000, 0xffffffff,
  
  // GCLK 5	, // TG7, V6 or with GST1
  0xf0034900, 0x01F405DC, 0xffffffff,
  0xf0034904, 0x00000000, 0xffffffff,
  0xf0034908, 0x00000000, 0xffffffff,
  0xf003490C, 0x04480445, 0xffffffff,
  0xf0034910, 0x00000000, 0xffffffff,
  0xf0034914, 0x00000000, 0xffffffff,
  0xf0034918, 0x00000004, 0xffffffff,
  0xf003491C, 0x00000000, 0xffffffff,

  // TGE, V6 | with GCLK
  0xf00349C0, 0x03000280, 0xffffffff,
  0xf00349C4, 0x00000000, 0xffffffff,
  0xf00349C8, 0x03000280, 0xffffffff,
  0xf00349CC, 0x0443043f, 0xffffffff,
  0xf00349D0, 0x00000000, 0xffffffff,
  0xf00349D4, 0x00000000, 0xffffffff,
  0xf00349D8, 0x00000004, 0xffffffff,
  0xf00349DC, 0x00000000, 0xffffffff,     
  
  // TGE, V6 xor with GST1
  0xf00349E0, 0x01F405DC, 0xffffffff,
  0xf00349E4, 0x00000000, 0xffffffff,
  0xf00349E8, 0x00000000, 0xffffffff,
  0xf00349EC, 0x00030001, 0xffffffff,
  0xf00349F0, 0x00000000, 0xffffffff,
  0xf00349F4, 0x00000000, 0xffffffff,
  0xf00349F8, 0x00000004, 0xffffffff,
  0xf00349FC, 0x00000000, 0xffffffff,   
  
  // TGF, V6 xor with EO
  0xf0034A00, 0x001500CA, 0xffffffff,
  0xf0034A04, 0x001500CA, 0xffffffff,
  0xf0034A08, 0x001500CA, 0xffffffff,
  0xf0034A0C, 0x00010000, 0xffffffff,
  0xf0034A10, 0x00000000, 0xffffffff,
  0xf0034A14, 0x04500450, 0xffffffff,
  0xf0034A18, 0x00000103, 0xffffffff,
  0xf0034A1C, 0x800ff001, 0xffffffff,
  
  //FLK, always high for GIP, OK																																																		
  0xf0034958, 0x00000037, 0xffffffff,
   
  //ODD	, // TGB V6 EO																											
  0xf0034980, 0x00CA0015, 0xffffffff,
  0xf0034984, 0x00CA0015, 0xffffffff,
  0xf0034988, 0x00CA0015, 0xffffffff,
  0xf003498C, 0x00010000, 0xffffffff,
  0xf0034990, 0x00000000, 0xffffffff,
  0xf0034994, 0x04500450, 0xffffffff,
  0xf0034998, 0x00000103, 0xffffffff,
  0xf003499C, 0x800ff001, 0xffffffff,
  
  //EVEN, // TGC V6 GST1
  0xf00349A0, 0x01F405DC, 0xffffffff,
  0xf00349A4, 0x00000000, 0xffffffff,
  0xf00349A8, 0x00000000, 0xffffffff,
  0xf00349AC, 0x04440441, 0xffffffff,
  0xf00349B0, 0x00000000, 0xffffffff,
  0xf00349B4, 0x00000000, 0xffffffff,
  0xf00349B8, 0x00000004, 0xffffffff,
  0xf00349BC, 0x00000000, 0xffffffff,


  // TG merge
  0xf0034814, 0x02000080, 0xffffffff,
  0xf0034818, 0x01C00040, 0xffffffff,    
    
    
  // DPM, OK            																														
  0xf0034978, 0x00000030, 0xffffffff,

    
  #elif defined(LGDV5GIP)
  // tg
  // GR0                             																														
 // 0xf0034800, 0x8100ffff, 0xffffffff,
                                       																														
  // GR1                             																														
  0xf0034804, 0x00000000, 0xffffffff,																													
                                       																														
  // GR7                             																														
  0xf003481c, 0x00000000, 0xffffffff,																														
                               													
   // SOE, TG0
  0xf0034820, 0x00C807d0, 0xffffffff,
  0xf0034824, 0x00000000, 0xffffffff,
  0xf0034828, 0x00C807d0, 0xffffffff,  
  0xf003482c, 0x04430007, 0xffffffff,
  0xf0034830, 0x20000000, 0xffffffff,
  0xf0034834, 0x80000000, 0xffffffff,
  0xf0034838, 0x00000007, 0xffffffff,
  0xf003483c, 0x00000000, 0xffffffff,																													
                                       																														
  // POL, OK                    																														
  0xf0034840, 0x07D007D0, 0xffffffff,																														
  0xf0034844, 0x00000000, 0xffffffff,																														
  0xf0034848, 0x07D007D0, 0xffffffff,																														
  0xf003484c, 0x043F0007, 0xffffffff,																														
  0xf0034850, 0x80000404, 0xffffffff,																														
  0xf0034854, 0x80020004, 0xffffffff,																														
  0xf0034858, 0x00000007, 0xffffffff,																														
  0xf003485c, 0x10000000, 0xffffffff,																														
                                       																														
  // GSP, OK                   																														
  0xf0034860, 0x01F405DC, 0xffffffff,
  0xf0034864, 0x00000000, 0xffffffff,
  0xf0034868, 0x00000000, 0xffffffff,
  0xf003486c, 0x00030001, 0xffffffff,
  0xf0034870, 0x00000000, 0xffffffff,
  0xf0034874, 0x00000000, 0xffffffff,
  0xf0034878, 0x00000004, 0xffffffff,
  0xf003487c, 0x00000000, 0xffffffff,
  																												
           
  // GCLK 1, OK																					
  0xf0034880, 0x06AE083E, 0xffffffff,
  0xf0034884, 0x00000000, 0xffffffff,
  0xf0034888, 0x06AE083E, 0xffffffff,
  0xf003488c, 0x04420005, 0xffffffff,
  0xf0034890, 0x80000606, 0xffffffff,
  0xf0034894, 0x80050002, 0xffffffff,
  0xf0034898, 0x00000007, 0xffffffff,
  0xf003489c, 0x00000000, 0xffffffff,
  
  // GCLK 2, OK          																														
  0xf00348A0, 0x06AE083E, 0xffffffff,
  0xf00348A4, 0x00000000, 0xffffffff,
  0xf00348A8, 0x06AE083E, 0xffffffff,
  0xf00348AC, 0x04430006, 0xffffffff,
  0xf00348B0, 0x80000606, 0xffffffff,
  0xf00348B4, 0x80040001, 0xffffffff,
  0xf00348B8, 0x00000007, 0xffffffff,
  0xf00348BC, 0x00000000, 0xffffffff,
  
  // GCLK 3, OK       																														
  0xf00348C0, 0x06AE083E, 0xffffffff,
  0xf00348C4, 0x00000000, 0xffffffff,
  0xf00348C8, 0x06AE083E, 0xffffffff,
  0xf00348CC, 0x04440007, 0xffffffff,
  0xf00348D0, 0x80000606, 0xffffffff,
  0xf00348D4, 0x80030006, 0xffffffff,
  0xf00348D8, 0x00000007, 0xffffffff,
  0xf00348DC, 0x00000000, 0xffffffff,
  
  // GCLK 4, OK     																														
  0xf00348E0, 0x06AE083E, 0xffffffff,
  0xf00348E4, 0x00000000, 0xffffffff,
  0xf00348E8, 0x06AE083E, 0xffffffff,
  0xf00348EC, 0x043F0002, 0xffffffff,
  0xf00348F0, 0x80000606, 0xffffffff,
  0xf00348F4, 0x80020005, 0xffffffff,
  0xf00348F8, 0x00000007, 0xffffffff,
  0xf00348FC, 0x00000000, 0xffffffff,
  
  // GCLK 5	, ok													
  0xf0034900, 0x06AE083E, 0xffffffff,
  0xf0034904, 0x00000000, 0xffffffff,
  0xf0034908, 0x06AE083E, 0xffffffff,
  0xf003490C, 0x04400003, 0xffffffff,
  0xf0034910, 0x80000606, 0xffffffff,
  0xf0034914, 0x80010004, 0xffffffff,
  0xf0034918, 0x00000007, 0xffffffff,
  0xf003491C, 0x00000000, 0xffffffff,
  
  // GCLK 6, ok
  0xf0034920, 0x06AE083E, 0xffffffff,
  0xf0034924, 0x00000000, 0xffffffff,
  0xf0034928, 0x06AE083E, 0xffffffff,
  0xf003492C, 0x04410004, 0xffffffff,
  0xf0034930, 0x80000606, 0xffffffff,
  0xf0034934, 0x80060003, 0xffffffff,
  0xf0034938, 0x00000007, 0xffffffff,
  0xf003493C, 0x00000000, 0xffffffff,
  									
  
  //FLK, always high for GIP, OK																																																		
  0xf0034958, 0x00000037, 0xffffffff,
   
  //ODD	, ok																											
  0xf0034980, 0x002d0015, 0xffffffff,
  0xf0034984, 0x002d0015, 0xffffffff,
  0xf0034988, 0x002d0015, 0xffffffff,
  0xf003498C, 0x00010000, 0xffffffff,
  0xf0034990, 0x00000000, 0xffffffff,
  0xf0034994, 0x04500450, 0xffffffff,
  0xf0034998, 0x00000103, 0xffffffff,
  0xf003499C, 0x80080001, 0xffffffff,
  
  //EVEN, OK              																														
  0xf00349A0, 0x0015002d, 0xffffffff,
  0xf00349A4, 0x0015002d, 0xffffffff,
  0xf00349A8, 0x0015002d, 0xffffffff,
  0xf00349AC, 0x00010000, 0xffffffff,
  0xf00349B0, 0x00000000, 0xffffffff,
  0xf00349B4, 0x04500450, 0xffffffff,
  0xf00349B8, 0x00000003, 0xffffffff,
  0xf00349BC, 0x80080001, 0xffffffff,
  
  // DPM, OK            																														
  0xf0034978, 0x00000030, 0xffffffff,
    																						                                    																														
  // GR0                             																														
 // 0xf0034800, 0x8100ffff, 0xffffffff,
  
  
  #elif defined(CMO2011) || defined(CHILINFHD)

  // SOE, TG0
  0xf0034820, 0x085007d0, 0xffffffff,
  0xf0034824, 0x00000000, 0xffffffff,
  0xf0034828, 0x085007d0, 0xffffffff,
  0xf003482c, 0x043E0007, 0xffffffff,
  0xf0034830, 0x20000000, 0xffffffff,
  0xf0034834, 0x80000000, 0xffffffff,
  0xf0034838, 0x00000007, 0xffffffff,
  0xf003483c, 0x00000000, 0xffffffff,

  // POL, TG1
  0xf0034840, 0x04000400, 0xffffffff,
  0xf0034844, 0x00000000, 0xffffffff,
  0xf0034848, 0x04000400, 0xffffffff,
  0xf003484c, 0x043F0006, 0xffffffff,
  0xf0034850, 0x00000404, 0xffffffff,
  0xf0034854, 0x80020004, 0xffffffff,
  0xf0034858, 0x00000007, 0xffffffff,
  0xf003485c, 0x10000000, 0xffffffff,

  // GSP, TG11
  0xf0034980, 0x04800480, 0xffffffff,
  0xf0034984, 0x00000000, 0xffffffff,
  0xf0034988, 0x00000000, 0xffffffff,
  0xf003498c, 0x00080007, 0xffffffff,
  0xf0034990, 0x00000000, 0xffffffff,
  0xf0034994, 0x00000000, 0xffffffff,
  0xf0034998, 0x00000004, 0xffffffff,
  0xf003499c, 0x00000000, 0xffffffff,

  // GSP_R, TG8
  0xf0034920, 0x04060406, 0xffffffff,
  0xf0034924, 0x00000000, 0xffffffff,
  0xf0034928, 0x00000000, 0xffffffff,
  0xf003492c, 0x00080007, 0xffffffff,
  0xf0034930, 0x00000000, 0xffffffff,
  0xf0034934, 0x00000000, 0xffffffff,
  0xf0034938, 0x00000004, 0xffffffff,
  0xf003493c, 0x00000000, 0xffffffff,

  // GOE, TG2
  0xf0034860, 0x07e006a0, 0xffffffff,
  0xf0034864, 0x00000000, 0xffffffff,
  0xf0034868, 0x07e006a0, 0xffffffff,
  0xf003486c, 0x046e0001, 0xffffffff,
  0xf0034870, 0x20000000, 0xffffffff,
  0xf0034874, 0x80000000, 0xffffffff,
  0xf0034878, 0x00000007, 0xffffffff,
  0xf003487c, 0x00000000, 0xffffffff,
  
  // GSC, TG12
  0xf00349a0, 0x040007c0, 0xffffffff,
  0xf00349a4, 0x00000000, 0xffffffff,
  0xf00349a8, 0x040007c0, 0xffffffff,
  0xf00349ac, 0x046e0001, 0xffffffff,
  0xf00349b0, 0x20000000, 0xffffffff,
  0xf00349b4, 0x80000000, 0xffffffff,
  0xf00349b8, 0x00000007, 0xffffffff,
  0xf00349bc, 0x00000000, 0xffffffff,
   
  // FLK, TG9  
  0xf0034958, 0x00000037, 0xffffffff,
  
  // DPM, TG10
  0xf0034978, 0x00000030, 0xffffffff,
  
  // H_CONV, TG5
  0xf00348d8, 0x00000020, 0xffffffff,

  // OPT_N, TG6
  0xf00348f8, 0x00000030, 0xffffffff,
  
  // 0PT_P , TG7
  0xf0034918, 0x00000020, 0xffffffff,  

  #elif defined(HIWXGA)
  
  // SOE, ok                         
  0xf0034820, 0x005885B2, 0xffffffff,
  0xf0034824, 0x00000000, 0xffffffff,
  0xf0034828, 0x005885B2, 0xffffffff,
  0xf003482c, 0x032c0001, 0xffffffff,
  0xf0034830, 0x20000000, 0xffffffff,
  0xf0034834, 0x80000000, 0xffffffff,
  0xf0034838, 0x00000007, 0xffffffff,
  0xf003483c, 0x00000000, 0xffffffff,
                                     
  // POL, MR - ~ML OK                
  0xf0034840, 0x05600560, 0xffffffff,
  0xf0034844, 0x00000000, 0xffffffff,
  0xf0034848, 0x05600560, 0xffffffff,
  0xf003484c, 0x032c0001, 0xffffffff,
  0xf0034850, 0x00000404, 0xffffffff,
  0xf0034854, 0x80020004, 0xffffffff,
  0xf0034858, 0x00000107, 0xffffffff,
  0xf003485c, 0x10000000, 0xffffffff,
                                      
  // GOE, tg2 data 2 ok                   
  0xf0034860, 0x00CF00CF, 0xffffffff,
  0xf0034864, 0x00000000, 0xffffffff,
  0xf0034868, 0x00000000, 0xffffffff,
  0xf003486c, 0x00090007, 0xffffffff,
  0xf0034870, 0x00000000, 0xffffffff,
  0xf0034874, 0x00000000, 0xffffffff,
  0xf0034878, 0x00000004, 0xffffffff,
  0xf003487c, 0x00000000, 0xffffffff,

  // GSP --> GOE or GSC  // TPK 3          
  0xf0034980, 0x05B00270, 0xffffffff,																														
  0xf0034984, 0x00000000, 0xffffffff,																														
  0xf0034988, 0x05B00270, 0xffffffff,																														
  0xf003498c, 0x032c0001, 0xffffffff,																														
  0xf0034990, 0x20000000, 0xffffffff,																														
  0xf0034994, 0x80000000, 0xffffffff,																														
  0xf0034998, 0x00000007, 0xffffffff,																														
  0xf003499c, 0x00000000, 0xffffffff,

  // TPK 3 for data 1
  0xf0034880, 0x01000410, 0xffffffff,
  0xf0034884, 0x00000000, 0xffffffff,
  0xf0034888, 0x01000410, 0xffffffff,
  0xf003488c, 0x00080006, 0xffffffff,
  0xf0034890, 0x20000000, 0xffffffff,
  0xf0034894, 0x80000000, 0xffffffff,
  0xf0034898, 0x00000107, 0xffffffff,
  0xf003489c, 0x00000000, 0xffffffff,  

                                       
  // GSP_R, OK   
  #if 0
  0xf0034920, 0x01000410, 0xffffffff,
  0xf0034924, 0x00000000, 0xffffffff,
  0xf0034928, 0x01000410, 0xffffffff,
  0xf003492c, 0x00080006, 0xffffffff,
  0xf0034930, 0x20000000, 0xffffffff,
  0xf0034934, 0x80000000, 0xffffffff,
  0xf0034938, 0x00000107, 0xffffffff,
  0xf003493c, 0x00000000, 0xffffffff,  
  #endif
                                     
  // GSC, data 1 ok                  
  0xf00349a0, 0x00CF00CF, 0xffffffff,
  0xf00349a4, 0x00000000, 0xffffffff,
  0xf00349a8, 0x00000000, 0xffffffff,
  0xf00349ac, 0x00090006, 0xffffffff,
  0xf00349b0, 0x00000000, 0xffffffff,
  0xf00349b4, 0x00000000, 0xffffffff,
  0xf00349b8, 0x00000004, 0xffffffff,
  0xf00349bc, 0x00000000, 0xffffffff,
                                     
  // FLK                             
  0xf0034940, 0x044C0578, 0xffffffff,
  0xf0034944, 0x00000000, 0xffffffff,
  0xf0034948, 0x044C0578, 0xffffffff,
  0xf003494c, 0x03080005, 0xffffffff,
  0xf0034950, 0x20000000, 0xffffffff,
  0xf0034954, 0x80000000, 0xffffffff,
  0xf0034958, 0x00000007, 0xffffffff,
  0xf003495c, 0x00000000, 0xffffffff,
                                     
  // TPK 13 for data 2       
  0xf00349C0, 0x01000410, 0xffffffff,
  0xf00349C4, 0x00000000, 0xffffffff,
  0xf00349C8, 0x01000410, 0xffffffff,
  0xf00349Cc, 0x00080006, 0xffffffff,
  0xf00349D0, 0x20000000, 0xffffffff,
  0xf00349D4, 0x80000000, 0xffffffff,
  0xf00349D8, 0x00000107, 0xffffffff,
  0xf00349Dc, 0x00000000, 0xffffffff,
                                     
                                     
  // DPM, OK                         
  0xf0034978, 0x00000030, 0xffffffff,
                                     
  // H_CONV, OK                      
  0xf00348d8, 0x00000020, 0xffffffff,
                                     
  // OPT_N, POL, ML OK               
  0xf00348e0, 0x05600560, 0xffffffff,
  0xf00348e4, 0x00000000, 0xffffffff,
  0xf00348e8, 0x05600560, 0xffffffff,
  0xf00348ec, 0x032c0001, 0xffffffff,
  0xf00348f0, 0x00000404, 0xffffffff,
  0xf00348f4, 0x80020004, 0xffffffff,
  0xf00348f8, 0x00000007, 0xffffffff,
  0xf00348fc, 0x10000000, 0xffffffff,
                                     
  // 0PT_P , TG7                     
  0xf0034918, 0x00000020, 0xffffffff,

  // GR5     
  0xf0034814, 0x03000020, 0xffffffff,
           
  // GR6    
  0xf0034818, 0x02000020, 0xffffffff,
                             
  #endif

//////////////////////////////////////////////////// mt5365
#elif defined (CC_MT5365)

///////////////////////////////////////// tg table: m1v1 LGDV4
#if defined(M1V1) && defined(LGDV4)
  // SOE, TG4
  0xf00348a0, 0x00C807D0, 0xffffffff,
  0xf00348a4, 0x00000000, 0xffffffff,
  0xf00348a8, 0x00C807D0, 0xffffffff,
  0xf00348ac, 0x04430007, 0xffffffff,
  0xf00348b0, 0x20000000, 0xffffffff,
  0xf00348b4, 0x80000000, 0xffffffff,
  0xf00348b8, 0x00000007, 0xffffffff,
  0xf00348bc, 0x00000000, 0xffffffff,

  // POL, TG8
  0xf0034920, 0x076C076C, 0xffffffff,
  0xf0034924, 0x00000000, 0xffffffff,
  0xf0034928, 0x076C076C, 0xffffffff,
  0xf003492c, 0x04430007, 0xffffffff,
  0xf0034930, 0x80000404, 0xffffffff,
  0xf0034934, 0x80020004, 0xffffffff,
  0xf0034938, 0x00000007, 0xffffffff,
  0xf003493c, 0x10000000, 0xffffffff,

  // GSP, TG14
  0xf00349e0, 0x04060406, 0xffffffff,
  0xf00349e4, 0x00000000, 0xffffffff,
  0xf00349e8, 0x00000000, 0xffffffff,
  0xf00349ec, 0x00080007, 0xffffffff,
  0xf00349f0, 0x00000000, 0xffffffff,
  0xf00349f4, 0x00000000, 0xffffffff,
  0xf00349f8, 0x00000004, 0xffffffff,
  0xf00349fc, 0x00000000, 0xffffffff,

  // GSP_R, TG15
  0xf0034A00, 0x04060406, 0xffffffff,
  0xf0034A04, 0x00000000, 0xffffffff,
  0xf0034A08, 0x00000000, 0xffffffff,
  0xf0034A0c, 0x00080007, 0xffffffff,
  0xf0034A10, 0x00000000, 0xffffffff,
  0xf0034A14, 0x00000000, 0xffffffff,
  0xf0034A18, 0x00000004, 0xffffffff,
  0xf0034A1c, 0x00000000, 0xffffffff,

  // GOE, TG3
  0xf0034880, 0x07BC062C, 0xffffffff,
  0xf0034884, 0x00000000, 0xffffffff,
  0xf0034888, 0x07BC062C, 0xffffffff,
  0xf003488c, 0x04420006, 0xffffffff,
  0xf0034890, 0x20000000, 0xffffffff,
  0xf0034894, 0x80000000, 0xffffffff,
  0xf0034898, 0x00000007, 0xffffffff,
  0xf003489c, 0x00000000, 0xffffffff,
  
  // GSC, TG11
  0xf0034980, 0x07260406, 0xffffffff,
  0xf0034984, 0x00000000, 0xffffffff,
  0xf0034988, 0x07260406, 0xffffffff,
  0xf003498c, 0x04420006, 0xffffffff,
  0xf0034990, 0x20000000, 0xffffffff,
  0xf0034994, 0x80000000, 0xffffffff,
  0xf0034998, 0x00000107, 0xffffffff,
  0xf003499c, 0x00000000, 0xffffffff,
   
  // FLK, TG7  
  0xf0034900, 0x049C0726, 0xffffffff,
  0xf0034904, 0x00000000, 0xffffffff,
  0xf0034908, 0x049C0726, 0xffffffff,
  0xf003490c, 0x04420006, 0xffffffff,
  0xf0034910, 0x20000000, 0xffffffff,
  0xf0034914, 0x80000000, 0xffffffff,
  0xf0034918, 0x00000007, 0xffffffff,
  0xf003491c, 0x00000000, 0xffffffff,

  
  // DPM, TG12
  0xf00349B8, 0x00000030, 0xffffffff,
  
  // H_CONV, TG1
  0xf0034858, 0x00000020, 0xffffffff,

  // OPT_N, TG2
  0xf0034878, 0x00000030, 0xffffffff,
  
  // 0PT_P , TG9
  0xf0034958, 0x00000020, 0xffffffff,

#elif defined(M1V1) && defined(SHAFHD)
  // SOE, TG4
  0xf00348a0, 0x00d00838, 0xffffffff,
  0xf00348a4, 0x00000000, 0xffffffff,
  0xf00348a8, 0x00d00838, 0xffffffff,
  0xf00348ac, 0x04660001, 0xffffffff,
  0xf00348b0, 0x20000000, 0xffffffff,
  0xf00348b4, 0x80000000, 0xffffffff,
  0xf00348b8, 0x00000007, 0xffffffff,
  0xf00348bc, 0x00000000, 0xffffffff,

  // POL, TG8
  0xf0034920, 0x06e006e0, 0xffffffff,
  0xf0034924, 0x00000000, 0xffffffff,
  0xf0034928, 0x06e006e0, 0xffffffff,
  0xf003492c, 0x04660001, 0xffffffff,
  0xf0034930, 0x80000202, 0xffffffff,
  0xf0034934, 0x80010002, 0xffffffff,
  0xf0034938, 0x00000007, 0xffffffff,
  0xf003493c, 0x10000000, 0xffffffff,

  // GSP, TG14
  0xf00349e0, 0x00ff06e0, 0xffffffff,
  0xf00349e4, 0x00000000, 0xffffffff,
  0xf00349e8, 0x00000000, 0xffffffff,
  0xf00349ec, 0x00070006, 0xffffffff,
  0xf00349f0, 0x00000000, 0xffffffff,
  0xf00349f4, 0x00000000, 0xffffffff,
  0xf00349f8, 0x00000004, 0xffffffff,
  0xf00349fc, 0x00000000, 0xffffffff,

  // GSP_R, TG15
  0xf0034A00, 0x00ff06e0, 0xffffffff,
  0xf0034A04, 0x00000000, 0xffffffff,
  0xf0034A08, 0x00000000, 0xffffffff,
  //0xf0034A0c, 0x04350434, 0xffffffff,
  0xf0034A0c, 0x00070006, 0xffffffff,
  0xf0034A10, 0x00000000, 0xffffffff,
  0xf0034A14, 0x00000000, 0xffffffff,
  //0xf0034A18, 0x00000004, 0xffffffff,
  0xf0034A18, 0x00000024, 0xffffffff,   // set low for sharp
  0xf0034A1c, 0x00000000, 0xffffffff,

  // GOE, TG3
  0xf0034880, 0x07BC062C, 0xffffffff,
  0xf0034884, 0x00000000, 0xffffffff,
  0xf0034888, 0x07BC062C, 0xffffffff,
  0xf003488c, 0x04420006, 0xffffffff,
  0xf0034890, 0x20000000, 0xffffffff,
  0xf0034894, 0x80000000, 0xffffffff,
  0xf0034898, 0x00000027, 0xffffffff,
  0xf003489c, 0x00000000, 0xffffffff,
  
  // GSC, TG11
  0xf0034980, 0x080b0725, 0xffffffff,
  0xf0034984, 0x00000000, 0xffffffff,
  0xf0034988, 0x080b0725, 0xffffffff,
  0xf003498c, 0x04650001, 0xffffffff,
  0xf0034990, 0x20000000, 0xffffffff,
  0xf0034994, 0x80000000, 0xffffffff,
  0xf0034998, 0x00000007, 0xffffffff,
  0xf003499c, 0x00000000, 0xffffffff,
   
  // FLK, TG7  
  0xf0034900, 0x049C0726, 0xffffffff,
  0xf0034904, 0x00000000, 0xffffffff,
  0xf0034908, 0x049C0726, 0xffffffff,
  0xf003490c, 0x04420006, 0xffffffff,
  0xf0034910, 0x20000000, 0xffffffff,
  0xf0034914, 0x80000000, 0xffffffff,
  0xf0034918, 0x00000037, 0xffffffff,
  0xf003491c, 0x00000000, 0xffffffff,

  
  // DPM, TG12
  0xf00349B8, 0x00000030, 0xffffffff,
  
  // H_CONV, TG1, CS1
#if 0  
  0xf0034858, 0x00000020, 0xffffffff,
#else
  0xf0034840, 0x06e006e0, 0xffffffff,
  0xf0034844, 0x00000000, 0xffffffff,
  0xf0034848, 0x06e006e0, 0xffffffff,
  //0xf003484c, 0x0443000b, 0xffffffff,
  0xf003484c, 0x0465000c, 0xffffffff,
  0xf0034850, 0x80003030, 0xffffffff,
  0xf0034854, 0x800d0025, 0xffffffff,
  0xf0034858, 0x00000007, 0xffffffff,
  //0xf003485c, 0x10000000, 0xffffffff,
  0xf003485c, 0x20000000, 0xffffffff,
#endif

  // OPT_N, TG2, CS2
#if 0  
  0xf0034878, 0x00000030, 0xffffffff,
#else
  0xf0034860, 0x06e006e0, 0xffffffff,
  0xf0034864, 0x00000000, 0xffffffff,
  0xf0034868, 0x06e006e0, 0xffffffff,
  //0xf003486c, 0x0447000f, 0xffffffff,
  0xf003486c, 0x04650010, 0xffffffff,
  0xf0034870, 0x80003030, 0xffffffff,
  0xf0034874, 0x80090021, 0xffffffff,
  0xf0034878, 0x00000007, 0xffffffff,
  //0xf003487c, 0x10000000, 0xffffffff,
  0xf003487c, 0x20000000, 0xffffffff,
#endif

  // 0PT_P , TG9  , LRBR, CS3
#if 0
  0xf0034958, 0x00000020, 0xffffffff,
#else
  0xf0034940, 0x06e006e0, 0xffffffff,
  0xf0034944, 0x00000000, 0xffffffff,
  0xf0034948, 0x06e006e0, 0xffffffff,
  //0xf003494c, 0x044B0013, 0xffffffff,
  0xf003494c, 0x04650014, 0xffffffff,
  0xf0034950, 0x80003030, 0xffffffff,
  0xf0034954, 0x8005001d, 0xffffffff,
  0xf0034958, 0x00000007, 0xffffffff,
  //0xf003495c, 0x10000000, 0xffffffff,
  0xf003495c, 0x20000000, 0xffffffff,
#endif

  // TG0, CS4
  0xf0034820, 0x06e006e0, 0xffffffff,
  0xf0034824, 0x00000000, 0xffffffff,
  0xf0034828, 0x06e006e0, 0xffffffff,
  //0xf003482c, 0x044f0017, 0xffffffff,
  0xf003482c, 0x04650018, 0xffffffff,
  0xf0034830, 0x80003030, 0xffffffff,
  0xf0034834, 0x80010019, 0xffffffff,
  0xf0034838, 0x00000007, 0xffffffff,
  //0xf003483c, 0x10000000, 0xffffffff,
  0xf003483c, 0x20000000, 0xffffffff,

  // TG5, CS5
  0xf00348C0, 0x06e006e0, 0xffffffff,
  0xf00348C4, 0x00000000, 0xffffffff,
  0xf00348C8, 0x06e006e0, 0xffffffff,
  //0xf00348Cc, 0x0453001B, 0xffffffff,
  0xf00348Cc, 0x0465001c, 0xffffffff,
  0xf00348D0, 0x80003030, 0xffffffff,
  0xf00348D4, 0x802d0015, 0xffffffff,
  0xf00348D8, 0x00000007, 0xffffffff,
  //0xf00348Dc, 0x10000000, 0xffffffff,
  0xf00348Dc, 0x20000000, 0xffffffff,

  // TG6, CS6
  0xf00348E0, 0x06e006e0, 0xffffffff,
  0xf00348E4, 0x00000000, 0xffffffff,
  0xf00348E8, 0x06e006e0, 0xffffffff,
  //0xf00348Ec, 0x0457001F, 0xffffffff,
  0xf00348Ec, 0x04650020, 0xffffffff,
  0xf00348F0, 0x80003030, 0xffffffff,
  0xf00348F4, 0x80290011, 0xffffffff,
  0xf00348F8, 0x00000007, 0xffffffff,
  //0xf00348Fc, 0x10000000, 0xffffffff,
  0xf00348Fc, 0x20000000, 0xffffffff,

///////////////////////////////////////////// tg table: m1v1 cmo2011
#elif defined(M1V1) && (defined(CMO2011) || defined(CHILINFHD))

  // SOE, TG4
  0xf00348a0, 0x085007d0, 0xffffffff,
  0xf00348a4, 0x00000000, 0xffffffff,
  0xf00348a8, 0x085007d0, 0xffffffff,
  0xf00348ac, 0x043E0007, 0xffffffff,
  0xf00348b0, 0x20000000, 0xffffffff,
  0xf00348b4, 0x80000000, 0xffffffff,
  0xf00348b8, 0x00000007, 0xffffffff,
  0xf00348bc, 0x00000000, 0xffffffff,
  
  // POL, TG8
  0xf0034920, 0x04000400, 0xffffffff,
  0xf0034924, 0x00000000, 0xffffffff,
  0xf0034928, 0x04000400, 0xffffffff,
  0xf003492c, 0x043F0006, 0xffffffff,
  0xf0034930, 0x00000404, 0xffffffff,
  0xf0034934, 0x80020004, 0xffffffff,
  0xf0034938, 0x00000007, 0xffffffff,
  0xf003493c, 0x10000000, 0xffffffff,
  
  // GSP, TG14
  0xf00349e0, 0x04800480, 0xffffffff,
  0xf00349e4, 0x00000000, 0xffffffff,
  0xf00349e8, 0x00000000, 0xffffffff,
  0xf00349ec, 0x00080007, 0xffffffff,
  0xf00349f0, 0x00000000, 0xffffffff,
  0xf00349f4, 0x00000000, 0xffffffff,
  0xf00349f8, 0x00000004, 0xffffffff,
  0xf00349fc, 0x00000000, 0xffffffff,
  
  // GSP_R, TG15
  0xf0034A00, 0x04060406, 0xffffffff,
  0xf0034A04, 0x00000000, 0xffffffff,
  0xf0034A08, 0x00000000, 0xffffffff,
  0xf0034A0c, 0x00080007, 0xffffffff,
  0xf0034A10, 0x00000000, 0xffffffff,
  0xf0034A14, 0x00000000, 0xffffffff,
  0xf0034A18, 0x00000004, 0xffffffff,
  0xf0034A1c, 0x00000000, 0xffffffff,
  
  // GOE, TG3
  0xf0034880, 0x07e006a0, 0xffffffff,
  0xf0034884, 0x00000000, 0xffffffff,
  0xf0034888, 0x07e006a0, 0xffffffff,
  0xf003488c, 0x046e0001, 0xffffffff,
  0xf0034890, 0x20000000, 0xffffffff,
  0xf0034894, 0x80000000, 0xffffffff,
  0xf0034898, 0x00000007, 0xffffffff,
  0xf003489c, 0x00000000, 0xffffffff,
  
  // GSC, TG11
  0xf0034980, 0x040007c0, 0xffffffff,
  0xf0034984, 0x00000000, 0xffffffff,
  0xf0034988, 0x040007c0, 0xffffffff,
  0xf003498c, 0x046e0001, 0xffffffff,
  0xf0034990, 0x20000000, 0xffffffff,
  0xf0034994, 0x80000000, 0xffffffff,
  0xf0034998, 0x00000007, 0xffffffff,
  0xf003499c, 0x00000000, 0xffffffff,
  
  // FLK, TG7  
  0xf0034918, 0x00000037, 0xffffffff,
  																																																								
  // DPM, TG12
  0xf00349B8, 0x00000030, 0xffffffff,
  
  // H_CONV, TG1
  0xf0034858, 0x00000020, 0xffffffff,
  
  // OPT_N, TG2
  0xf0034878, 0x00000030, 0xffffffff,
  
  // 0PT_P , TG9
  0xf0034958, 0x00000020, 0xffffffff,

/////////////////////////////////////// tg table: mt5366p2v1 AUO
#elif defined(M1V1) && defined(AUOFHD)
  
  // SOE, TG4
  0xf00348a0, 0x080207D0, 0xffffffff,
  0xf00348a4, 0x00000000, 0xffffffff,
  0xf00348a8, 0x080207D0, 0xffffffff,
  0xf00348ac, 0x046B0007, 0xffffffff,
  0xf00348b0, 0x20000000, 0xffffffff,
  0xf00348b4, 0x80000000, 0xffffffff,
  0xf00348b8, 0x00000007, 0xffffffff,
  0xf00348bc, 0x00000000, 0xffffffff,
  
  // POL, TG8
  0xf0034920, 0x079E079E, 0xffffffff,
  0xf0034924, 0x00000000, 0xffffffff,
  0xf0034928, 0x079E079E, 0xffffffff,
  0xf003492c, 0x04650001, 0xffffffff,
  0xf0034930, 0x80000202, 0xffffffff,
  0xf0034934, 0x80010002, 0xffffffff,
  0xf0034938, 0x00000007, 0xffffffff,
  0xf003493c, 0x20000000, 0xffffffff,
  
  
  // GSP, TG14
  0xf00349e0, 0x00C800C8, 0xffffffff,
  0xf00349e4, 0x00000000, 0xffffffff,
  0xf00349e8, 0x00000000, 0xffffffff,
  0xf00349ec, 0x00080007, 0xffffffff,
  0xf00349f0, 0x00000000, 0xffffffff,
  0xf00349f4, 0x00000000, 0xffffffff,
  0xf00349f8, 0x00000004, 0xffffffff,
  0xf00349fc, 0x00000000, 0xffffffff,
  
  // GSP_R, TG15
  0xf0034A00, 0x00C800C8, 0xffffffff,
  0xf0034A04, 0x00000000, 0xffffffff,
  0xf0034A08, 0x00000000, 0xffffffff,
  0xf0034A0c, 0x00080007, 0xffffffff,
  0xf0034A10, 0x00000000, 0xffffffff,
  0xf0034A14, 0x00000000, 0xffffffff,
  0xf0034A18, 0x00000004, 0xffffffff,
  0xf0034A1c, 0x00000000, 0xffffffff,
  
  // GOE, TG3
  0xf0034880, 0x073A07FD, 0xffffffff,
  0xf0034884, 0x00000000, 0xffffffff,
  0xf0034888, 0x073A07FD, 0xffffffff,
  0xf003488c, 0x043F0007, 0xffffffff,
  0xf0034890, 0x20000000, 0xffffffff,
  0xf0034894, 0x80000000, 0xffffffff,
  0xf0034898, 0x00000007, 0xffffffff,
  0xf003489c, 0x00000000, 0xffffffff,
  
  // GSC, TG11
  0xf0034980, 0x080207D0, 0xffffffff,
  0xf0034984, 0x00000000, 0xffffffff,
  0xf0034988, 0x080207D0, 0xffffffff,
  0xf003498c, 0x046B0007, 0xffffffff,
  0xf0034990, 0x20000000, 0xffffffff,
  0xf0034994, 0x80000000, 0xffffffff,
  0xf0034998, 0x00000007, 0xffffffff,
  0xf003499c, 0x00000000, 0xffffffff,
  
  // FLK, TG7  
  0xf0034900, 0x076C0708, 0xffffffff,
  0xf0034904, 0x00000000, 0xffffffff,
  0xf0034908, 0x076C0708, 0xffffffff,
  0xf003490c, 0x043F0008, 0xffffffff,
  0xf0034910, 0x20000000, 0xffffffff,
  0xf0034914, 0x80000000, 0xffffffff,
  0xf0034918, 0x00000107, 0xffffffff,
  0xf003491c, 0x00000000, 0xffffffff,
  
  
  // DPM, TG12
  0xf00349B8, 0x00000030, 0xffffffff,
  
  // H_CONV, TG1
  0xf0034858, 0x00000020, 0xffffffff,
  
  // OPT_N, TG2
  0xf0034878, 0x00000030, 0xffffffff,
  
  // 0PT_P , TG9
  0xf0034958, 0x00000020, 0xffffffff,
  

/////////////////////////////////////// tg table: p1v1 LGDV4
#elif defined(P1V1) && defined(LGDV4)

  // SOE, TG7
  0xf0034900, 0x00C80834, 0xffffffff,
  0xf0034904, 0x00000000, 0xffffffff,
  0xf0034908, 0x00C80834, 0xffffffff,
  0xf003490c, 0x04430007, 0xffffffff,
  0xf0034910, 0x20000000, 0xffffffff,
  0xf0034914, 0x80000000, 0xffffffff,
  0xf0034918, 0x00000007, 0xffffffff,
  0xf003491c, 0x00000000, 0xffffffff,
  
  // POL, TG1
  0xf0034840, 0x076C076C, 0xffffffff,
  0xf0034844, 0x00000000, 0xffffffff,
  0xf0034848, 0x076C076C, 0xffffffff,
  0xf003484c, 0x04430007, 0xffffffff,
  0xf0034850, 0x80000404, 0xffffffff,
  0xf0034854, 0x80020004, 0xffffffff,
  0xf0034858, 0x00000007, 0xffffffff,
  0xf003485c, 0x10000000, 0xffffffff,
  
  // GSP, TG3
  0xf0034880, 0x04060406, 0xffffffff,
  0xf0034884, 0x00000000, 0xffffffff,
  0xf0034888, 0x00000000, 0xffffffff,
  0xf003488c, 0x00080007, 0xffffffff,
  0xf0034890, 0x00000000, 0xffffffff,
  0xf0034894, 0x00000000, 0xffffffff,
  0xf0034898, 0x00000004, 0xffffffff,
  0xf003489c, 0x00000000, 0xffffffff,
  
  // GSP_R, TG6
  0xf00348e0, 0x04060406, 0xffffffff,
  0xf00348e4, 0x00000000, 0xffffffff,
  0xf00348e8, 0x00000000, 0xffffffff,
  0xf00348ec, 0x00080007, 0xffffffff,
  0xf00348f0, 0x00000000, 0xffffffff,
  0xf00348f4, 0x00000000, 0xffffffff,
  0xf00348f8, 0x00000004, 0xffffffff,
  0xf00348fc, 0x00000000, 0xffffffff,
  
  // GOE, TG0
  0xf0034820, 0x07BC062C, 0xffffffff,
  0xf0034824, 0x00000000, 0xffffffff,
  0xf0034828, 0x07BC062C, 0xffffffff,
  0xf003482c, 0x04420006, 0xffffffff,
  0xf0034830, 0x20000000, 0xffffffff,
  0xf0034834, 0x80000000, 0xffffffff,
  0xf0034838, 0x00000007, 0xffffffff,
  0xf003483c, 0x00000000, 0xffffffff,
  
  // GSC, TG2
  0xf0034860, 0x07260406, 0xffffffff,
  0xf0034864, 0x00000000, 0xffffffff,
  0xf0034868, 0x07260406, 0xffffffff,
  0xf003486c, 0x04420006, 0xffffffff,
  0xf0034870, 0x20000000, 0xffffffff,
  0xf0034874, 0x80000000, 0xffffffff,
  0xf0034878, 0x00000107, 0xffffffff,
  0xf003487c, 0x00000000, 0xffffffff,
  
  // FLK, TG4  
  0xf00348a0, 0x049C0726, 0xffffffff,
  0xf00348a4, 0x00000000, 0xffffffff,
  0xf00348a8, 0x049C0726, 0xffffffff,
  0xf00348ac, 0x04420006, 0xffffffff,
  0xf00348b0, 0x20000000, 0xffffffff,
  0xf00348b4, 0x80000000, 0xffffffff,
  0xf00348b8, 0x00000007, 0xffffffff,
  0xf00348bc, 0x00000000, 0xffffffff,
  
  // DPM, TG8
  0xf0034938, 0x00000030, 0xffffffff,
  
  // H_CONV, TG12
  0xf00349b8, 0x00000020, 0xffffffff,
  
  // OPT_N, TG11
  0xf0034998, 0x00000030, 0xffffffff,
  
  // 0PT_P , TG10
  0xf0034978, 0x00000020, 0xffffffff,


#elif defined(P1V1) && defined(HIWXGA)

  // SOE, TG7
  0xf0034900, 0x00C80834, 0xffffffff,
  0xf0034904, 0x00000000, 0xffffffff,
  0xf0034908, 0x00C80834, 0xffffffff,
  0xf003490c, 0x04430007, 0xffffffff,
  0xf0034910, 0x20000000, 0xffffffff,
  0xf0034914, 0x80000000, 0xffffffff,
  0xf0034918, 0x00000007, 0xffffffff,
  0xf003491c, 0x00000000, 0xffffffff,
  
  // POL, TG1
  0xf0034840, 0x076C076C, 0xffffffff,
  0xf0034844, 0x00000000, 0xffffffff,
  0xf0034848, 0x076C076C, 0xffffffff,
  0xf003484c, 0x04430007, 0xffffffff,
  0xf0034850, 0x80000404, 0xffffffff,
  0xf0034854, 0x80020004, 0xffffffff,
  0xf0034858, 0x00000007, 0xffffffff,
  0xf003485c, 0x10000000, 0xffffffff,
  
  // GSP, TG3
  0xf0034880, 0x04060406, 0xffffffff,
  0xf0034884, 0x00000000, 0xffffffff,
  0xf0034888, 0x00000000, 0xffffffff,
  0xf003488c, 0x00080007, 0xffffffff,
  0xf0034890, 0x00000000, 0xffffffff,
  0xf0034894, 0x00000000, 0xffffffff,
  0xf0034898, 0x00000004, 0xffffffff,
  0xf003489c, 0x00000000, 0xffffffff,
  
  // GSP_R, TG6
  0xf00348e0, 0x04060406, 0xffffffff,
  0xf00348e4, 0x00000000, 0xffffffff,
  0xf00348e8, 0x00000000, 0xffffffff,
  0xf00348ec, 0x00080007, 0xffffffff,
  0xf00348f0, 0x00000000, 0xffffffff,
  0xf00348f4, 0x00000000, 0xffffffff,
  0xf00348f8, 0x00000004, 0xffffffff,
  0xf00348fc, 0x00000000, 0xffffffff,
  
  // GOE, TG0
  0xf0034820, 0x07BC062C, 0xffffffff,
  0xf0034824, 0x00000000, 0xffffffff,
  0xf0034828, 0x07BC062C, 0xffffffff,
  0xf003482c, 0x04420006, 0xffffffff,
  0xf0034830, 0x20000000, 0xffffffff,
  0xf0034834, 0x80000000, 0xffffffff,
  0xf0034838, 0x00000007, 0xffffffff,
  0xf003483c, 0x00000000, 0xffffffff,
  
  // GSC, TG2
  0xf0034860, 0x07260406, 0xffffffff,
  0xf0034864, 0x00000000, 0xffffffff,
  0xf0034868, 0x07260406, 0xffffffff,
  0xf003486c, 0x04420006, 0xffffffff,
  0xf0034870, 0x20000000, 0xffffffff,
  0xf0034874, 0x80000000, 0xffffffff,
  0xf0034878, 0x00000107, 0xffffffff,
  0xf003487c, 0x00000000, 0xffffffff,
  
  // FLK, TG4  
  0xf00348a0, 0x049C0726, 0xffffffff,
  0xf00348a4, 0x00000000, 0xffffffff,
  0xf00348a8, 0x049C0726, 0xffffffff,
  0xf00348ac, 0x04420006, 0xffffffff,
  0xf00348b0, 0x20000000, 0xffffffff,
  0xf00348b4, 0x80000000, 0xffffffff,
  0xf00348b8, 0x00000007, 0xffffffff,
  0xf00348bc, 0x00000000, 0xffffffff,
  
  // DPM, TG8
  0xf0034938, 0x00000030, 0xffffffff,
  
  // H_CONV, TG12
  0xf00349b8, 0x00000020, 0xffffffff,
  
  // OPT_N, TG11
  0xf0034998, 0x00000030, 0xffffffff,
  
  // 0PT_P , TG10
  0xf0034978, 0x00000020, 0xffffffff,

  #endif

#endif
};

#ifndef CC_MTK_LOADER
static CHAR * _aszTconState[] =
{
    "TCON_STATE_NOTREADY",
    "TCON_STATE_INIT",
    "TCON_STATE_STANDBY",
    "TCON_STATE_MUTE",
    "TCON_STATE_NORMAL",
    "TCON_STATE_ERROR",
    "TCON_STATE_MAX"    
};
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void vTconRegWrite(UINT32 u4addr, UINT32 val)
{
    #ifndef CC_MTK_LOADER
    UINT32 u4tmp;
    #endif

	if ((u4addr == 0xffffffff) || ((u4addr&TCON_ADDR_BASE) != TCON_ADDR_BASE))
	{
		LOG(1,"Write addr Err!!! w 0x%x 0x%x\n", u4addr, val);
	}
    else
    {
	    vIO32Write4B(u4addr, val);
        #ifndef CC_MTK_LOADER
   	    u4tmp = u4IO32Read4B(u4addr);
        LOG(1,"0x%8x, 0x%8x, 0xffffffff,\n", u4addr, u4tmp);
        #endif
    }
}

static UINT32 u4TconRegRead(UINT32 u4addr)
{
    UINT32 u4tmp;
	
	if ((u4addr == 0xffffffff) || ((u4addr&TCON_ADDR_BASE) != TCON_ADDR_BASE))
	{
		LOG(1,"Read addr Err!!! w 0x%x\n", u4addr);
		return (0);
	}
    else
    {
   	    u4tmp = u4IO32Read4B(u4addr);
        LOG(1,"0x%8x, 0x%8x, 0xffffffff,\n", u4addr, u4tmp);		
		return (u4tmp);
    }
}

// DPM power control
static void vTconDPMCtrl(UINT8 u1Val)
{
  u1TconCustDPMCtrlEN1(1);
  u1TconCustDPMCtrlEN2(1);  	
}

static void vLogTconStateChange(UINT8 u1State)
{
    // Check if the state is correct
    if (u1State < TCON_STATE_MAX)
    {
        if (_u1TconStateOld != u1State)
        {
            #ifndef CC_MTK_LOADER
            LOG(1, "Tcon state change from %s to %s\n", _aszTconState[_u1TconStateOld],_aszTconState[u1State]);
            #endif
            _u1TconStateOld = u1State;
        }
    }
    else // Undefined state
    {
        if (_u1TconStateOld != u1State)
        {
            #ifndef CC_MTK_LOADER
            LOG(1, "Tcon state change from %s to UNDEFINED_STATE\n", _aszTconState[_u1TconStateOld]);
            #endif
            _u1TconStateOld = TCON_STATE_NOTREADY;
        }
    }
}

/**
 * @brief u1DrvTconTestPattern

 * TCON test pattern

 * @param  void
 * @retval 0: fail, 1: success
 */
UINT8 u1DrvTconTP(UINT8 val)
{
    UINT8 u1Ret = 0;

    if(_u1TconReady == 0) return (0);

	vIO32WriteFldAlign(EH_00, 0, MUTE_MLVDS_EN);
	vIO32WriteFldAlign(EH_00, 0, CLK_MUTE_RESET_EN);
	vIO32WriteFldAlign(EH_00, 0, SET_DATA_NO_GATE);	
	vIO32WriteFldAlign(EH_14,1,PTGEN_TYPE_EN);
	
	switch(val)
	{
	    case TCON_TP_OFF:
            vIO32WriteFldAlign(EH_00, 0, MUTE_MLVDS_EN);
   	        vIO32WriteFldAlign(EH_00, 0, CLK_MUTE_RESET_EN);
   	        vIO32WriteFldAlign(EH_00, 0, SET_DATA_NO_GATE);				
			vIO32WriteFldAlign(EH_15, 2, PTGEN_TYPE);
			vIO32WriteFldAlign(EH_14, 0, PTGEN_TYPE_EN);
			vIO32WriteFldAlign(EH_15, 0, PTGEN_H_RUN);
			vIO32WriteFldAlign(EH_15, 0, PTGEN_V_RUN);
			vIO32WriteFldAlign(EH_15, 0, AUTO_CHG_PTGEN_EN);
			break;
			
		case TCON_TP_R:
			vIO32WriteFldAlign(EH_15, 1, PTGEN_TYPE);
			vIO32WriteFldAlign(EH_14, 0xff, PTGEN_R);
			vIO32WriteFldAlign(EH_14, 0, PTGEN_G);
			vIO32WriteFldAlign(EH_14, 0, PTGEN_B);
			break;
			
		case TCON_TP_G:
			vIO32WriteFldAlign(EH_15, 1, PTGEN_TYPE);
			vIO32WriteFldAlign(EH_14, 0, PTGEN_R);
			vIO32WriteFldAlign(EH_14, 0xff, PTGEN_G);
			vIO32WriteFldAlign(EH_14, 0, PTGEN_B);
			break;
			
		case TCON_TP_B:
			vIO32WriteFldAlign(EH_15, 1, PTGEN_TYPE);
			vIO32WriteFldAlign(EH_14, 0, PTGEN_R);
			vIO32WriteFldAlign(EH_14, 0, PTGEN_G);
			vIO32WriteFldAlign(EH_14, 0xff, PTGEN_B);			
			break;
			
		case TCON_TP_BLACK:
			vIO32WriteFldAlign(EH_15, 1, PTGEN_TYPE);
			vIO32WriteFldAlign(EH_14, 0, PTGEN_R);
			vIO32WriteFldAlign(EH_14, 0, PTGEN_G);
			vIO32WriteFldAlign(EH_14, 0, PTGEN_B);			
			break;
			
		case TCON_TP_WHITE:
			vIO32WriteFldAlign(EH_15, 1, PTGEN_TYPE);
			vIO32WriteFldAlign(EH_14, 0xff, PTGEN_R);
			vIO32WriteFldAlign(EH_14, 0xff, PTGEN_G);
			vIO32WriteFldAlign(EH_14, 0xff, PTGEN_B);		
			break;
			
		case TCON_TP_COLORRAMP:
			vIO32WriteFldAlign(EH_15, 2, PTGEN_TYPE);
			vIO32WriteFldAlign(EH_15, 0, PTGEN_H_RUN);
			vIO32WriteFldAlign(EH_15, 0, PTGEN_V_RUN);
			vIO32WriteFldAlign(EH_15, 0, AUTO_CHG_PTGEN_EN);
			break;
			
		case TCON_TP_MOVING:
			vIO32WriteFldAlign(EH_15, 2, PTGEN_TYPE);
			vIO32WriteFldAlign(EH_15, 1, PTGEN_H_RUN);
			vIO32WriteFldAlign(EH_15, 1, PTGEN_V_RUN);
			vIO32WriteFldAlign(EH_15, 0, AUTO_CHG_PTGEN_EN);			
			break;
			
		case TCON_TP_AUTO:
			vIO32WriteFldAlign(EH_15, 0, PTGEN_H_RUN);
			vIO32WriteFldAlign(EH_15, 0, PTGEN_V_RUN);
			vIO32WriteFldAlign(EH_15, 1, AUTO_CHG_PTGEN_EN);
			break;
			
		default:
			break;

	}

	return(u1Ret);	
}
#define EH_PANEL_TABLE
#ifdef EH_PANEL_TABLE
#define EH_PANEL_FORMULATE 0
#endif
void vErrorHandleSetByPanel(void)
{
#ifdef EH_PANEL_TABLE 

      #if EH_PANEL_FORMULATE  
      UINT32 H_LOW_TH,H_HIGH_TH,H_MID_TH;
      UINT32 V_LOW_TH,V_HIGH_TH,V_MID_TH; 
      UINT32 EH_TOTAL, EH_CLK_TOTAL_RATION;
      #else
      UINT32 H_LOW_TH,H_MID_TH;
      UINT32 V_LOW_TH,V_MID_TH; 
      UINT32 EH_TOTAL, EH_CLK_HIGH_TH, EH_CLK_HIGH_LH;
      #endif

      #ifdef CC_DEBUG
       Printf("[SA7] Error handling init \n ");
       Printf("[SA7] PANEL_GetPanelWidth=0x%x \n PANEL_GetPanelHeight=0x%x \n wDrvGetOutputHTotal=0x%x \n  wDrvGetOutputVTotal=0x%x \n ",
              PANEL_GetPanelWidth(),PANEL_GetPanelHeight(),wDrvGetOutputHTotal(),wDrvGetOutputVTotal());        
      #endif     


      #if EH_PANEL_FORMULATE             
       H_LOW_TH = PANEL_GetPanelWidth() + 100;
       H_HIGH_TH = wDrvGetOutputHTotal() * 2;
       H_MID_TH = ( H_HIGH_TH + H_LOW_TH ) / 2;

       if (PANEL_GetPanelHeight()>=1060)
       {
       V_LOW_TH = PANEL_GetPanelHeight() + 30;
       V_HIGH_TH = wDrvGetOutputVTotal() * 2;
       V_MID_TH = ( V_HIGH_TH + V_LOW_TH ) / 2;
       }
       else
       {
       V_LOW_TH = PANEL_GetPanelHeight() + 20;
       V_HIGH_TH = wDrvGetOutputVTotal() * 2;
       V_MID_TH = ( V_HIGH_TH + V_LOW_TH ) / 2;
       }
       
       EH_TOTAL = 1024*27000 / (PANEL_GetPixelClk60Hz()/1000);
       EH_CLK_TOTAL_RATION = EH_TOTAL / 4;        
      #else      
      #ifdef CC_DEBUG
       Printf("[SA7] PANEL_GetHTotalMax=0x%x \n PANEL_GetHTotalMin=0x%x \n PANEL_GetVTotalMax=0x%x \n  PANEL_GetVTotalMin=0x%x \n ",
              PANEL_GetHTotalMax(),PANEL_GetHTotalMin(),PANEL_GetVTotalMax(),PANEL_GetVTotalMin());        
       Printf("[SA7] PANEL_GetPixelClkMax=%d \n PANEL_GetPixelClkMin=%d \n",
              PANEL_GetPixelClkMax(),PANEL_GetPixelClkMin());                      
      #endif        
       H_MID_TH = (PANEL_GetHTotalMax() + PANEL_GetHTotalMin()) / 2;
       H_LOW_TH = PANEL_GetHTotalMin();

       V_MID_TH = (PANEL_GetVTotalMax() + PANEL_GetVTotalMin()) / 2;
       V_LOW_TH = PANEL_GetVTotalMin();
      
       EH_CLK_HIGH_TH = 1024*27000/ (PANEL_GetPixelClkMax()/1000);       
       EH_CLK_HIGH_LH = 1024*27000/ (PANEL_GetPixelClkMin()/1000);  
       EH_TOTAL =  (EH_CLK_HIGH_TH + EH_CLK_HIGH_LH)/2;
      #endif
     
       #ifdef CC_DEBUG
        Printf("[SA7] PANEL_GetPixelClk60Hz = 0x%x \n ",PANEL_GetPixelClk60Hz());
       #endif
             
       vIO32WriteFldAlign(EH_02,H_MID_TH-H_LOW_TH,FAST_MUTE_H_TOTAL_TH);
       vIO32WriteFldAlign(EH_02,H_MID_TH,FAST_MUTE_H_TOTAL);        
     
       vIO32WriteFldAlign(EH_03,V_MID_TH-V_LOW_TH,FAST_MUTE_V_TOTAL_TH);
       vIO32WriteFldAlign(EH_03,V_MID_TH,FAST_MUTE_V_TOTAL);                
             
       vIO32WriteFldAlign(EH_04,PANEL_GetPanelWidth(),FAST_MUTE_DE_TOTAL);
       vIO32WriteFldAlign(EH_05,PANEL_GetPanelHeight(),FAST_MUTE_VDE_TOTAL);        

       #if EH_PANEL_FORMULATE       
       vIO32WriteFldAlign(EH_06,EH_CLK_TOTAL_RATION,FAST_MUTE_CLK_TOTAL_TH);        
       #else
       vIO32WriteFldAlign(EH_06,EH_TOTAL-EH_CLK_HIGH_TH,FAST_MUTE_CLK_TOTAL_TH);  
       #endif
       vIO32WriteFldAlign(EH_06,EH_TOTAL,FAST_MUTE_CLK_TOTAL);                       
     
       vIO32WriteFldAlign(EH_0B,(PANEL_GetPanelHeight()-1),CALI_RANGE_H);   
   
       #if EH_PANEL_FORMULATE
       //vIO32WriteFldAlign(EH_0C,PANEL_GetPanelWidth()+60,DE_HS_START);                
       //vIO32WriteFldAlign(EH_0D,PANEL_GetPanelHeight()+18,DE_VS_START);          
       // vIO32WriteFldAlign(EH_0D,(V_LOW_TH-IO32ReadFldAlign(EH_0D,DE_VS_WIDTH)),DE_VS_START);         

       vIO32WriteFldAlign(EH_0C, PANEL_GetHSyncWidth(),DE_HS_WIDTH);                
       // [Note] minus 4 --> Dealy 4T in DE fix mode .
       if (PANEL_GetHPosition60Hz()>=0x800)
       {
       vIO32WriteFldAlign(EH_0C,(PANEL_GetPanelWidth() -4 +(PANEL_GetHPosition60Hz() - 0x800)),DE_HS_START);       
       }
       else
       {
       vIO32WriteFldAlign(EH_0C,(PANEL_GetPanelWidth() -4 -(0x800 - PANEL_GetHPosition60Hz())),DE_HS_START);       
       }
       
       vIO32WriteFldAlign(EH_0D, PANEL_GetVSyncWidth()-1,DE_VS_WIDTH); 
       // [Note] minus OS_75_VSYNC_DELSEL
       // [Note] minus 2 --> Dealy 1 Line in DE fix mode and 1 Line in HSYNC & VSYNC align.
       #ifdef CC_MT5395
       if (PANEL_GetVPosition()>=0x400)
       {
       vIO32WriteFldAlign(EH_0D,(PANEL_GetPanelHeight() + (wDrvGetOutputVTotal() - PANEL_GetPanelHeight() - 6) - 2 + (PANEL_GetVPosition() - 0x400)),DE_VS_START);
       }
       else
       {
       vIO32WriteFldAlign(EH_0D,(PANEL_GetPanelHeight() + (wDrvGetOutputVTotal() - PANEL_GetPanelHeight() - 6) - 2 - (0x400 -PANEL_GetVPosition())),DE_VS_START);
       }
       #else
       if (PANEL_GetVPosition()>=0x400)
       {
       vIO32WriteFldAlign(EH_0D,(PANEL_GetPanelHeight() + (wDrvGetOutputVTotal() - PANEL_GetPanelHeight() - 5) - 2 + (PANEL_GetVPosition() - 0x400)),DE_VS_START);
       }
       else
       {
       vIO32WriteFldAlign(EH_0D,(PANEL_GetPanelHeight() + (wDrvGetOutputVTotal() - PANEL_GetPanelHeight() - 5) - 2 - (0x400 - PANEL_GetVPosition())),DE_VS_START);
       }
       #endif
       #else
       vErrorHandleSetByTiming();       
       #endif

       vIO32WriteFldAlign(EH_0F,PANEL_GetPanelWidth()-20,FIX_VDE_TH);           
       vIO32WriteFldAlign(EH_0F,PANEL_GetPanelWidth(),FIXED_DE_TOATL);  
       
       vIO32WriteFldAlign(EH_10,PANEL_GetPanelWidth(),PTGEN_H_ACTIVE);           
       vIO32WriteFldAlign(EH_10,wDrvGetOutputHTotal(),PTGEN_H_TOTAL);                   
     
       vIO32WriteFldAlign(EH_12,PANEL_GetPanelHeight(),PTGEN_V_ACTIVE);           
       vIO32WriteFldAlign(EH_12,wDrvGetOutputVTotal(),PTGEN_V_TOTAL);    
     
       vIO32WriteFldAlign(EH_18,wDrvGetOutputVTotal(),DE_VS_TOTAL_SW);           
       vIO32WriteFldAlign(EH_18,wDrvGetOutputHTotal(),DE_HS_TOTAL_SW); 
     
       vIO32WriteFldAlign(EH_19,PANEL_GetPanelWidth()+80,FIX_HTOTAL_VALUE);           
       vIO32WriteFldAlign(EH_19,PANEL_GetPanelWidth()+80,FIX_HTOTAL_TH);         
       
       if ((BSP_GetIcVersion() >= IC_VER_5395_AB)||(BSP_GetIcVersion() >= IC_VER_5365_AB))
       {
       // code for MT5365 1st ECO version IC only
       // code for MT5395 1st ECO version IC only
        vIO32WriteFldAlign(EH_1A,PANEL_GetPanelWidth()-20,FIX_VDE_TH);           
	//for middle line of IPS2010 Panel	 
	#if HISENSE_IPS_2010_PANEL
        vIO32WriteFldAlign(EH_1A,PANEL_GetPanelWidth() + 2,FIXED_DE_TOATL); 
	 #else
        vIO32WriteFldAlign(EH_1A,PANEL_GetPanelWidth(),FIXED_DE_TOATL); 
	 #endif
       }     

#endif

}

void vErrorHandleSetByTiming(void)
{
    UINT16 u2GetHSyncWidth,u2GetVSyncWidth;

    #ifdef CC_DEBUG
    Printf("[SA7] PANEL_GetHSyncWidth()=%d\n", PANEL_GetHSyncWidth());
    Printf("[SA7] PANEL_GetPanelWidth()=%d\n", PANEL_GetPanelWidth());  
    Printf("[SA7] OS_16_H_POS()=%d\n", RegReadFldAlign(OUTSTG_OS_16, OS_16_H_POS));
    Printf("[SA7] PANEL_GetVSyncWidth()=%d\n", PANEL_GetVSyncWidth());
    Printf("[SA7] wDrvGetOutputVTotal()=%d\n", wDrvGetOutputVTotal());  
    Printf("[SA7] OS_18_V_POS()=%d\n", RegReadFldAlign(OUTSTG_OS_18, OS_18_V_POS));
    Printf("[SA7] OS_75_VSYNC_DELSEL2()=%d\n",  RegReadFldAlign(OUTSTG_OS_75,OS_75_VSYNC_DELSEL2));    
    Printf("[SA7] u2GetHSyncWidth =%d\n",  RegReadFldAlign(OUTSTG_OS_17,OS_17_HS_WIDTH));    
    Printf("[SA7] u2GetVSyncWidth =%d\n",  RegReadFldAlign(OUTSTG_OS_18,OS_18_VS_WIDTH));        
    #endif
   
    u2GetHSyncWidth = RegReadFldAlign(OUTSTG_OS_17,OS_17_HS_WIDTH);
    
    vIO32WriteFldAlign(EH_0C, u2GetHSyncWidth,DE_HS_WIDTH);                
    // [Note] minus 4 --> Dealy 4T in DE fix mode .
    if(RegReadFldAlign(OUTSTG_OS_16, OS_16_H_POS)>=0x800)
    {
    vIO32WriteFldAlign(EH_0C,(PANEL_GetPanelWidth() -4 + (RegReadFldAlign(OUTSTG_OS_16, OS_16_H_POS) - 0x800)),DE_HS_START);
    }
    else
    {
    vIO32WriteFldAlign(EH_0C,(PANEL_GetPanelWidth()- 4 - (0x800 - RegReadFldAlign(OUTSTG_OS_16, OS_16_H_POS))),DE_HS_START);
    }

    u2GetVSyncWidth = RegReadFldAlign(OUTSTG_OS_18,OS_18_VS_WIDTH);

    vIO32WriteFldAlign(EH_0D, u2GetVSyncWidth,DE_VS_WIDTH);     
    
    // [Note] minus OS_75_VSYNC_DELSEL 
    // VPOS=(PANEL_GetPanelHeight()+(PANEL_GetVPosition()-0x400)+(wDrvGetOutputVTotal()-PANEL_GetPanelHeight()-5))
    // [Note] minus 2 --> Dealy 1 Line in DE fix mode and 1 Line in HSYNC & VSYNC align.
    if (RegReadFldAlign(OUTSTG_OS_18, OS_18_V_POS)>=0x400)
    {
    vIO32WriteFldAlign(EH_0D,((wDrvGetOutputVTotal() - RegReadFldAlign(OUTSTG_OS_75,OS_75_VSYNC_DELSEL2)) -2 + (RegReadFldAlign(OUTSTG_OS_18, OS_18_V_POS) - 0x400)),DE_VS_START);
    }
    else
    {
    vIO32WriteFldAlign(EH_0D,((wDrvGetOutputVTotal() - RegReadFldAlign(OUTSTG_OS_75,OS_75_VSYNC_DELSEL2)) -2 - (0x400 - RegReadFldAlign(OUTSTG_OS_18, OS_18_V_POS))),DE_VS_START);    
    }

}

void vErrorHandleInit(void)
{

#ifdef CC_MT5395
if (BSP_GetIcVersion() == IC_VER_5395_AA)
{

if (LVDS_OUTPUT_PORT == SINGLE_PORT) // WXGA/60
{
    vIO32WriteFldAlign(EH_00,0x00030000,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_01,0x963F3067,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_02,0x03590935,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_03,0x01CB0487,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_04,0x00100558,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_05,0x00100300,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_06,0x007201B7,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_07,0x02200502,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_08,0x0202050A,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_09,0x0202050A,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0A,0x00008002,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0B,0x40020300,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0C,0x80100600,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0D,0x00060310,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0E,0xc3090000,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_10,0x05580647,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_11,0x00180032,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_12,0x03000329,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_13,0x00100013,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_14,0x00000000,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_15,0x0000FF02,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_16,0x00000000,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_17,0x00000200,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_18,0x13290647,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_19,0x05a604a6,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0F,0x05420558,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_STATUS_05,0x00000000,EH_FLD_ALL);   
}
else  if (LVDS_OUTPUT_PORT == DUAL_PORT) // 1080P/60
{
    vIO32WriteFldAlign(EH_00,0x00030000,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_01,0x963F3067,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_02,0x041A0BE8,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_03,0x01DA0626,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_04,0x00100780,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_05,0x00100438,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_06,0x007200BA,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_07,0x02200502,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_08,0x0202050A,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_09,0x0202050A,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0A,0x00008002,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0B,0x40020438,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0C,0x801007C4,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0D,0x0006044A,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0E,0xc3090000,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_10,0x07800898,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_11,0x00180032,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_12,0x04380464,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_13,0x00100013,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_14,0x00000000,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_15,0x0000FF02,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_16,0x00000000,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_17,0x00000200,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_18,0x14650898,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_19,0x07d007d0,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0F,0x076C0780,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_STATUS_05,0x00000000,EH_FLD_ALL);      
}
else //if (LVDS_OUTPUT_PORT == FOUR_PORT) 
{
    vIO32WriteFldAlign(EH_00,0x00030000,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_01,0x963F3067,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_02,0x041A0BE8,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_03,0x01DA0626,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_04,0x00100780,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_05,0x00100438,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_06,0x0072005D,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_07,0x02200502,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_08,0x0202050A,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_09,0x0202050A,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0A,0x00008002,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0B,0x40020438,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0C,0x801007C4,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0D,0x0006044A,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0E,0xc3090000,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_10,0x07800898,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_11,0x00180032,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_12,0x04380464,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_13,0x00100013,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_14,0x00000000,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_15,0x0000FF02,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_16,0x00000000,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_17,0x00000200,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_18,0x14650898,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_19,0x07d007d0,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_0F,0x076C0780,EH_FLD_ALL);
    vIO32WriteFldAlign(EH_STATUS_05,0x00000000,EH_FLD_ALL); 
}
}
else
{
    if (LVDS_OUTPUT_PORT == SINGLE_PORT) // WXGA/60
    {
        vIO32WriteFldAlign(EH_00,0x00030000,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_01,0x963F3067,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_02,0x03590935,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_03,0x01CB0487,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_04,0x00100558,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_05,0x00100300,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_06,0x007201B7,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_07,0x02200502,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_08,0x0202050A,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_09,0x0202050A,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0A,0x00008002,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0B,0x40020300,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0C,0x80100600,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0D,0x00060310,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0E,0x83090000,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_10,0x05580647,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_11,0x00180032,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_12,0x03000329,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_13,0x00100013,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_14,0x00000000,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_15,0x0000FF02,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_16,0x00000000,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_17,0x00000200,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_18,0x03290647,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_19,0x05a604a6,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_1A,0x05420558,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_1B,0x0000BFFF,EH_FLD_ALL);                      
        vIO32WriteFldAlign(EH_STATUS_05,0x00000000,EH_FLD_ALL);   
      }
      else  if (LVDS_OUTPUT_PORT == DUAL_PORT) // 1080P/60
      {
        vIO32WriteFldAlign(EH_00,0x00030000,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_01,0x963F3067,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_02,0x041A0BE8,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_03,0x01DA0626,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_04,0x00100780,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_05,0x00100438,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_06,0x007200BA,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_07,0x02200502,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_08,0x0202050A,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_09,0x0202050A,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0A,0x00008002,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0B,0x40020438,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0C,0x801007C4,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0D,0x0006044A,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0E,0x83090000,EH_FLD_ALL);                      
        vIO32WriteFldAlign(EH_10,0x07800898,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_11,0x00180032,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_12,0x04380464,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_13,0x00100013,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_14,0x00000000,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_15,0x0000FF02,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_16,0x00000000,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_17,0x00000200,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_18,0x04650898,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_19,0x07d007d0,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_1A,0x076C0780,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_1B,0x0000BFFF,EH_FLD_ALL);                      
        vIO32WriteFldAlign(EH_STATUS_05,0x00000000,EH_FLD_ALL);      
      }
      else //if (LVDS_OUTPUT_PORT == FOUR_PORT) 
      {
        vIO32WriteFldAlign(EH_00,0x00030000,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_01,0x963F3067,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_02,0x041A0BE8,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_03,0x01DA0626,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_04,0x00100780,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_05,0x00100438,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_06,0x0072005D,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_07,0x02200502,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_08,0x0202050A,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_09,0x0202050A,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0A,0x00008002,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0B,0x40020438,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0C,0x801007C4,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0D,0x0006044A,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0E,0x83090000,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_0F,0x80000780,EH_FLD_ALL);                                   
        vIO32WriteFldAlign(EH_10,0x07800898,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_11,0x00180032,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_12,0x04380464,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_13,0x00100013,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_14,0x00000000,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_15,0x0000FF02,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_16,0x00000000,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_17,0x00000200,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_18,0x04650898,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_19,0x07d007d0,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_1A,0x076C0780,EH_FLD_ALL);
        vIO32WriteFldAlign(EH_1B,0x0000BFFF,EH_FLD_ALL);                      
        vIO32WriteFldAlign(EH_STATUS_05,0x00000000,EH_FLD_ALL); 
    }      

}
#else
    if (BSP_GetIcVersion() == IC_VER_5365_AA) //// code for MT5365 A version IC only
    {

    if (LVDS_OUTPUT_PORT == SINGLE_PORT) // WXGA/60
    {
      vIO32WriteFldAlign(EH_00,0x00070000,EH_FLD_ALL); 
      vIO32WriteFldAlign(EH_01,0x963F1467,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_02,0x03590935,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_03,0x01CB0487,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_04,0x00100558,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_05,0x00100300,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_06,0x007201B7,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_07,0x02200502,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_08,0x0202050A,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_09,0x0202050A,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_0A,0x00008002,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_0B,0x42FF0002,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_0C,0x00100600,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_0D,0x00060310,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_0E,0x00020000,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_0F,0x80000558,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_10,0x05580647,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_11,0x00180032,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_12,0x03000329,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_13,0x00100013,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_14,0x00000000,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_15,0x0000FF02,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_16,0x00000000,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_17,0x00000200,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_STATUS_05,0x00000000,EH_FLD_ALL);
    }
    else  if (LVDS_OUTPUT_PORT == DUAL_PORT) // 1080P/60
    {
      vIO32WriteFldAlign(EH_00,0x00030000,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_01,0x963F3067,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_02,0x041A0BE8,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_03,0x01DA0626,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_04,0x00100780,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_05,0x00100438,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_06,0x007200BA,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_07,0x02200502,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_08,0x0202050A,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_09,0x0202050A,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_0A,0x00008002,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_0B,0x40020438,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_0C,0x801007C4,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_0D,0x0006044A,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_0E,0xc3090000,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_0F,0x80000780,EH_FLD_ALL);     
      vIO32WriteFldAlign(EH_10,0x07800898,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_11,0x00180032,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_12,0x04380464,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_13,0x00100013,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_14,0x00000000,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_15,0x0000FF02,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_16,0x00000000,EH_FLD_ALL);
      vIO32WriteFldAlign(EH_17,0x00000200,EH_FLD_ALL);                      
      vIO32WriteFldAlign(EH_STATUS_05,0x00000000,EH_FLD_ALL);        
    }
    else //if (LVDS_OUTPUT_PORT == FOUR_PORT) 
    {

    }
    }
    else // // code for MT5365 1st ECO version IC only
    {

      if (LVDS_OUTPUT_PORT == SINGLE_PORT) // WXGA/60
      {
          vIO32WriteFldAlign(EH_00,0x00030000,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_01,0x963F3067,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_02,0x03590935,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_03,0x01CB0487,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_04,0x00100558,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_05,0x00100300,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_06,0x007201B7,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_07,0x02200502,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_08,0x0202050A,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_09,0x0202050A,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0A,0x00008002,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0B,0x40020300,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0C,0x80100600,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0D,0x00060310,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0E,0xc3090000,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_10,0x05580647,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_11,0x00180032,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_12,0x03000329,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_13,0x00100013,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_14,0x00000000,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_15,0x0000FF02,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_16,0x00000000,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_17,0x00000200,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_18,0x13290647,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_19,0x05a604a6,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_1A,0x05420558,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_1B,0x0000BFFF,EH_FLD_ALL);                      
          vIO32WriteFldAlign(EH_STATUS_05,0x00000000,EH_FLD_ALL);   
        }
        else  if (LVDS_OUTPUT_PORT == DUAL_PORT) // 1080P/60
        {
          vIO32WriteFldAlign(EH_00,0x00030000,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_01,0x963F3067,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_02,0x041A0BE8,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_03,0x01DA0626,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_04,0x00100780,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_05,0x00100438,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_06,0x007200BA,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_07,0x02200502,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_08,0x0202050A,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_09,0x0202050A,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0A,0x00008002,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0B,0x40020438,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0C,0x801007C4,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0D,0x0006044A,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0E,0xc3090000,EH_FLD_ALL);                      
          vIO32WriteFldAlign(EH_10,0x07800898,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_11,0x00180032,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_12,0x04380464,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_13,0x00100013,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_14,0x00000000,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_15,0x0000FF02,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_16,0x00000000,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_17,0x00000200,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_18,0x14650898,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_19,0x07d007d0,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_1A,0x076C0780,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_1B,0x0000BFFF,EH_FLD_ALL);                      
          vIO32WriteFldAlign(EH_STATUS_05,0x00000000,EH_FLD_ALL);      
        }
        else //if (LVDS_OUTPUT_PORT == FOUR_PORT) 
        {
          vIO32WriteFldAlign(EH_00,0x00030000,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_01,0x963F3067,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_02,0x041A0BE8,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_03,0x01DA0626,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_04,0x00100780,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_05,0x00100438,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_06,0x0072005D,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_07,0x02200502,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_08,0x0202050A,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_09,0x0202050A,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0A,0x00008002,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0B,0x40020438,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0C,0x801007C4,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0D,0x0006044A,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0E,0xc3090000,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_0F,0x80000780,EH_FLD_ALL);                                   
          vIO32WriteFldAlign(EH_10,0x07800898,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_11,0x00180032,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_12,0x04380464,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_13,0x00100013,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_14,0x00000000,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_15,0x0000FF02,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_16,0x00000000,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_17,0x00000200,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_18,0x14650898,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_19,0x07d007d0,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_1A,0x076C0780,EH_FLD_ALL);
          vIO32WriteFldAlign(EH_1B,0x0000BFFF,EH_FLD_ALL);                      
          vIO32WriteFldAlign(EH_STATUS_05,0x00000000,EH_FLD_ALL); 
      }      
    }
#endif
  vErrorHandleSetByPanel();
}

void vErrorHandleStatus(void)
{
  UINT32 u2MUTE_STATUS; 

  u2MUTE_STATUS = IO32ReadFldAlign(EH_STATUS_02,EH_FLD_ALL);
  
  Printf("---------- Error Handling Status ----------\n"); 

  Printf("[Encode Input Information]\n");
  if (IO32ReadFldAlign(MLVDS_P0,RG_MLVDSM_IN_SEL)==0)
  Printf("[0]Error handling + Dither + OD\n");
  else if (IO32ReadFldAlign(MLVDS_P0,RG_MLVDSM_IN_SEL)==1)
  Printf("[1]Outputstage\n");
  else if (IO32ReadFldAlign(MLVDS_P0,RG_MLVDSM_IN_SEL)==2)
  Printf("[2]Error handling\n");
  else if (IO32ReadFldAlign(MLVDS_P0,RG_MLVDSM_IN_SEL)==3)
  Printf("[3]Error handling + OD\n");

  Printf("[V,F Information]\n");
  Printf("DE_TOTAL_MON=%d, H_TOTAL_MON=%d, VDE_TOTAL_MON=%d, V_TOTAL_MON=%d\n",
          IO32ReadFldAlign(EH_STATUS_00,DE_TOTAL_MON),IO32ReadFldAlign(EH_STATUS_00,H_TOTAL_MON),
          IO32ReadFldAlign(EH_STATUS_01,VDE_TOTAL_MON),IO32ReadFldAlign(EH_STATUS_01,V_TOTAL_MON));          

  Printf("[UNREADY Status]\n"); 
  if (((u2MUTE_STATUS >> EHS_OD_READY)& 0x01) == 0)
  Printf("| OD_UNREADY");
  if (((u2MUTE_STATUS >> EHS_OD_OK)& 0x01) == 0)
  Printf("| OD_UNOK");
  if (((u2MUTE_STATUS >> EHS_DISP_OK)& 0x01) == 0)
  Printf("| DISP_UNOK");
  if (((u2MUTE_STATUS >> EHS_FAST_MUTE_READY)& 0x01) == 0)
  Printf("FAST_MUTE_UNREADY");
  Printf("\n");

  Printf("[MUTE Condition]\n");
  Printf("HOTAL[High = %4d,Low = %4d]\n",
          IO32ReadFldAlign(EH_02,FAST_MUTE_H_TOTAL) + IO32ReadFldAlign(EH_02,FAST_MUTE_H_TOTAL_TH),
          IO32ReadFldAlign(EH_02,FAST_MUTE_H_TOTAL) - IO32ReadFldAlign(EH_02,FAST_MUTE_H_TOTAL_TH));
  Printf("VOTAL[High = %4d,Low = %4d]\n",
          IO32ReadFldAlign(EH_03,FAST_MUTE_V_TOTAL) + IO32ReadFldAlign(EH_03,FAST_MUTE_V_TOTAL_TH),
          IO32ReadFldAlign(EH_03,FAST_MUTE_V_TOTAL) - IO32ReadFldAlign(EH_03,FAST_MUTE_V_TOTAL_TH));
  Printf("DE   [High = %4d,Low = %4d]\n",
          IO32ReadFldAlign(EH_04,FAST_MUTE_DE_TOTAL) + IO32ReadFldAlign(EH_04,FAST_MUTE_DE_TOTAL_TH),
          IO32ReadFldAlign(EH_04,FAST_MUTE_DE_TOTAL) - IO32ReadFldAlign(EH_04,FAST_MUTE_DE_TOTAL_TH));          
  Printf("VDE  [High = %4d,Low = %4d]\n",
          IO32ReadFldAlign(EH_05,FAST_MUTE_VDE_TOTAL) + IO32ReadFldAlign(EH_05,FAST_MUTE_VDE_TOTAL_TH),
          IO32ReadFldAlign(EH_05,FAST_MUTE_VDE_TOTAL) - IO32ReadFldAlign(EH_05,FAST_MUTE_VDE_TOTAL_TH));          
  Printf("Clock[High = %d MHz,Low = %d MHz]\n",
          ((1024*27)/(IO32ReadFldAlign(EH_06,FAST_MUTE_CLK_TOTAL) - IO32ReadFldAlign(EH_06,FAST_MUTE_CLK_TOTAL_TH))),
          ((1024*27)/(IO32ReadFldAlign(EH_06,FAST_MUTE_CLK_TOTAL) + IO32ReadFldAlign(EH_06,FAST_MUTE_CLK_TOTAL_TH))));

  Printf("[Mute Status]\n");    
  if (((u2MUTE_STATUS >> EHS_OD_INTF_ERROR)& 0x01) == 1)
  Printf("OD_INTF_ERROR");
  if (((u2MUTE_STATUS >> EHS_MUTE_LVDS)& 0x01) == 1)
  Printf("| MUTE_LVDS");
  if (((u2MUTE_STATUS >> EHS_DE_MISS_MUTE)& 0x01) == 1)
  Printf("| DE_MISS_MUTE");
  if (((u2MUTE_STATUS >> EHS_INPUT_OK)& 0x01) == 1)
  Printf("| INPUT_OK");
  if (((u2MUTE_STATUS >> EHS_OD_BYPASS)& 0x01) == 1)
  Printf("| OD_BYPASS");
  if (((u2MUTE_STATUS >> EHS_OD_MUTE_DDDS)& 0x01) == 1)
  Printf("| OD_MUTE_DDDS");
  Printf("\n");    

  if (((u2MUTE_STATUS >> EHS_H_MISS_MUTE)& 0x01) == 1)
  Printf("| H_MISS_MUTE");
  if (((u2MUTE_STATUS >> EHS_CLOCK_MUTE)& 0x01) == 1)
  Printf("| CLOCK MUTE");
  if (((u2MUTE_STATUS >> EHS_VDE_MUTE)& 0x01) == 1)
  Printf("| VDE_MUTE");
  if (((u2MUTE_STATUS >> EHS_V_MUTE)& 0x01) == 1)
  Printf("| V_MUTE");
  if (((u2MUTE_STATUS >> EHS_DE_MUTE)& 0x01) == 1)
  Printf("| DE_MUTE");
  if (((u2MUTE_STATUS >> EHS_H_MUTE)& 0x01) == 1)
  Printf("| H_MUTE");
  if (((u2MUTE_STATUS >> EHS_MUTE)& 0x01) == 1)
  Printf("| MUTE");  
  Printf("\n");    

  Printf("[UNSTABLE Status]\n");    

  if (((u2MUTE_STATUS >> EHS_CONFIG_READY)& 0x01) == 0)
  Printf("Configuration_UNREADY");
  if (((u2MUTE_STATUS >> EHS_DDDS_LOCK)& 0x01) == 0)
  Printf("| DDDS_UNLOCK");
  if (((u2MUTE_STATUS >> EHS_CLOCK_STABLE)& 0x01) == 0)
  Printf("| CLOCK_UNSTABLE");
  if (((u2MUTE_STATUS >> EHS_VDE_STABLE)& 0x01) == 0)
  Printf("| VDE_UNSTABLE");
  if (((u2MUTE_STATUS >> EHS_V_STABLE)& 0x01) == 0)
  Printf("| V_UNSTABLE");
  if (((u2MUTE_STATUS >> EHS_DE_STABLE)& 0x01) == 0)
  Printf("| DE_UNSTABLE");
  if (((u2MUTE_STATUS >> EHS_H_STABLE)& 0x01) == 0)
  Printf("| H_UNSTABLE");
  if (((u2MUTE_STATUS >> EHS_STABLE)& 0x01) == 0)
  Printf("| UNSTABLE");  
  Printf("\n");    
          

}

void u1ErrorHandleFixEnable(UINT8 val)
{
  vIO32WriteFldMulti(EH_0E, P_Fld(val, FIXED_DE_EN)
                                       |P_Fld(val, FIXED_VDE_EN)|P_Fld(val, DE_DEGLITCH_EN));

}

void u1ErrorHandleMuteEnable(UINT8 val)
{
   vIO32WriteFldAlign(EH_00, val, MUTE_MLVDS_EN);   
}


void u1TconSourceSet(UINT8 val)
{
  if (val<=3)
  {
   vIO32WriteFldAlign(MLVDS_P0, val, RG_MLVDSM_IN_SEL);   
  } 
}

void vMiniLVDS_AnalogInit(void)
{
    UINT32 u4Value;
        
    // set TVO(driving current) if necessary
    u4Value = PANEL_GetDrivingCurrent();
    if (u4Value <= 15)
    {
      vDrvLVDSSetDriving(u4Value);
    }
    else
    {
      vDrvLVDSSetDriving(4);
    }
        
    #ifdef CC_MT5395
        vIO32WriteFldMulti(LVDSTX_ANA0, P_Fld(3, LVDSA0_TVCM)|P_Fld(3, LVDSA0_NSRC)|P_Fld(4, LVDSA0_PSRC)|P_Fld(0x0, LVDSA0_PD));
        vIO32WriteFldMulti(LVDSTX_ANA4, P_Fld(3, LVDSB4_TVCM)|P_Fld(3, LVDSB4_NSRC)|P_Fld(4, LVDSB4_PSRC)|P_Fld(0x0, LVDSB4_PD));   

        vIO32WriteFldMulti(LVDSTX_ANA3, P_Fld(1, LVDSA3_TERM_EN)|P_Fld(1, LVDSA3_MINI_SEL_CK2)|P_Fld(1, LVDSA3_MINI_SEL_CK1)|P_Fld(1, LVDSA3_MINI_SEL));
        vIO32WriteFldMulti(LVDSTX_ANA7, P_Fld(1, LVDSB7_TERM_EN)|P_Fld(1, LVDSB7_MINI_SEL_CK2)|P_Fld(1, LVDSB7_MINI_SEL_CK1)|P_Fld(1, LVDSB7_MINI_SEL));
        
        vIO32WriteFldMulti(LVDSTX_ANA1, P_Fld(1, LVDSA1_CLK_SEL_CK2)|P_Fld(1, LVDSA1_CLK_SEL_CK1)|P_Fld(0, LVDSA1_CLK_SEL));
        vIO32WriteFldMulti(LVDSTX_ANA5, P_Fld(1, LVDSB5_CLK_SEL_CK2)|P_Fld(1, LVDSB5_CLK_SEL_CK1)|P_Fld(0, LVDSB5_CLK_SEL));
    #else
        vRegWriteFldMulti(OSTG_LVDS_04, P_Fld(3, LVDS_04_RG_LVDS_ATVCM)|P_Fld(3, LVDS_04_RG_LVDS_ANSRC)|P_Fld(4, LVDS_04_RG_LVDS_APSRC)|
                                        P_Fld(0, LVDS_04_RG_LVDS_APD));
        vRegWriteFldMulti(OSTG_LVDS_05, P_Fld(0, LVDS_05_RG_LVDS_APSRC_O5)|P_Fld(0, LVDS_05_RG_LVDS_APSRC_E5));    

        vRegWriteFldMulti(OSTG_LVDS_03, P_Fld(0, LVDS_03_RG_LVDS_AE8)|P_Fld(3, LVDS_03_RG_LVDS_AE4));            

        vRegWriteFldMulti(OSTG_LVDS_02, P_Fld(1, LVDS_02_RG_LVDS_ACLK_SEL_CK2)|P_Fld(1, LVDS_02_RG_LVDS_ACLK_SEL_CK1)|P_Fld(0, LVDS_02_RG_LVDS_ACLK_SEL));            
        
        vRegWriteFldMulti(OSTG_LVDS_01, P_Fld(1, LVDS_01_RG_LVDS_ATERM_EN)|P_Fld(1, LVDS_01_RG_LVDS_ATERM_EN_O5)|P_Fld(1, LVDS_01_RG_LVDS_ATERM_EN_E5)|
                                        P_Fld(0, LVDS_01_RG_LVDS_BIASA_O5_PD)|P_Fld(0, LVDS_01_RG_LVDS_BIASA_E5_PD)|P_Fld(0, LVDS_01_RG_LVDS_ASMTEN)|
                                        P_Fld(1, LVDS_01_RG_LVDS_AMINI_SEL_CK2)|P_Fld(1, LVDS_01_RG_LVDS_AMINI_SEL_CK1)|P_Fld(1, LVDS_01_RG_LVDS_AMINI_SEL));
    #endif              

    // set top pinmux to LDVS out
    vIO32WriteFldAlign(CKGEN_PMUX_MISC, 0, FLD_RG_ATE_PINMUX_EN);
    vIO32WriteFldAlign(CKGEN_PMUX_MISC, 0, FLD_RG_SYS_PINMUX_EN);
    
    // enable LVDS PAD
    #ifdef CC_MT5365
     vIO32WriteFldAlign(CKGEN_PMUX_MISC, 1, FLD_RG_LVDS_PAD_EN);
    #endif
    #ifdef CC_MT5395
     vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PMUX3_LVDS_EN);
    #endif
        
}

/**
 * @brief vDrvErrorHandleEN

 * Error handle enable control

 * @param  bEn
 * @retval void
 */

static void vDrvErrorHandleEN(UINT8 bEn) 
{
    vIO32WriteFldAlign(EH_00,bEn,MUTE_MLVDS_EN);
    vIO32WriteFldMulti(EH_01,P_Fld(bEn,FAST_MUTE_EN)|P_Fld(bEn,DE_MISS_MUTE_EN));

}

static void vDrvTconGammaInit(void)
{   
	#if  HISENSE_IPS_2010_PANEL 
	   x_thread_delay(15);
	#endif

    if (u1TconCustGammaInit() == 1)
    {
        _u1GammaReady = 1;
    }
    else
    {
        _u1GammaReady = 0;
    }
    
	LOG(1,"Tcon Gamma init\n");
}

static void vDrvMiniLvdsOn(void)
{
    //power up bandgap and all reference circuit
#ifdef CC_MT5395
    vIO32WriteFldAlign(LVDSTX_ANA0, 0, LVDSA0_BIAS_PD);
    vIO32WriteFldAlign(LVDSTX_ANA4, 0, LVDSB4_BIAS_PD);    
#else
    vRegWriteFldAlign(OSTG_LVDS_04, 0, LVDS_04_RG_LVDS_BIASA_PD);
    vRegWriteFldMulti(OSTG_LVDS_01, P_Fld(0, LVDS_01_RG_LVDS_BIASA_O5_PD)|P_Fld(0, LVDS_01_RG_LVDS_BIASA_E5_PD));
#endif


#ifdef CC_MT5395
vIO32WriteFldAlign(LVDSTX_ANA0, 1, LVDSA0_RST);// analog LVDS reset
vIO32WriteFldAlign(LVDSTX_ANA4, 1, LVDSB4_RST);// analog LVDS reset    
#else
vRegWriteFldAlign(OSTG_LVDS_04, 1, LVDS_04_RG_LVDS_ARST);// analog LVDS reset
#endif

#ifdef CC_COPLAT_MT82
vUtDelay2us(5);
#else
HAL_Delay_us(10);
#endif

#ifdef CC_MT5395
vIO32WriteFldAlign(LVDSTX_ANA0, 0, LVDSA0_RST);// analog LVDS normal
vIO32WriteFldAlign(LVDSTX_ANA4, 0, LVDSB4_RST);// analog LVDS normal    
#else
vRegWriteFldAlign(OSTG_LVDS_04, 0, LVDS_04_RG_LVDS_ARST);// analog LVDS normal
#endif

}

/*
static void vDrvMiniLvdsOff(void)
{
#ifdef CC_MT5395
    // power down analog
    vIO32WriteFldAlign(LVDSTX_ANA0, 0xFFF, LVDSA0_PD);
    vIO32WriteFldAlign(LVDSTX_ANA4, 0xFFF, LVDSB4_PD);
    
    //power down bandgap and all reference circuit
    vIO32WriteFldAlign(LVDSTX_ANA0, 1, LVDSA0_BIAS_PD);
    vIO32WriteFldAlign(LVDSTX_ANA4, 1, LVDSB4_BIAS_PD);    
    
    // analog reset
    vIO32WriteFldAlign(LVDSTX_ANA0, 1, LVDSA0_RST);
    vIO32WriteFldAlign(LVDSTX_ANA4, 1, LVDSB4_RST);    
#else
    // power down analog
    vRegWriteFldAlign(OSTG_LVDS_04, 0x3FFF, LVDS_04_RG_LVDS_APD);
    //power down bandgap and all reference circuit
    vRegWriteFldAlign(OSTG_LVDS_04, 1, LVDS_04_RG_LVDS_BIASA_PD);
    vRegWriteFldMulti(OSTG_LVDS_01, P_Fld(1, LVDS_01_RG_LVDS_BIASA_O5_PD)|P_Fld(1, LVDS_01_RG_LVDS_BIASA_E5_PD));
    // analog reset
    vRegWriteFldAlign(OSTG_LVDS_04, 1, LVDS_04_RG_LVDS_ARST);
#endif

}
*/

static void vDrvMiniLvdsInit(void)
{
	UINT8 i = 0;
	UINT8 u1count;
	UINT32 u4tmp, u4tmp1;

	LOG(1,"miniLvds init\n");
	u1count = (sizeof(sRegMini) / sizeof(struct sTCONREGTABLE));

	for ( i = 0 ; i < u1count ; i++)
    {
        u4tmp = sRegMini[i].u4Addr;
		u4tmp1 = (u4TconRegRead(u4tmp) & ~(sRegMini[i].u4Msk)) | (sRegMini[i].u4Val);

		vTconRegWrite(u4tmp,u4tmp1);
    }
    
  	LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegMini), sizeof(struct sTCONREGTABLE));
	

}

static void vDrvTgInit(void)
{
	UINT8 i = 0;
	UINT8 u1count;
	UINT32 u4tmp, u4tmp1;

	LOG(1,"tg init\n");

	u1count = (sizeof(sRegTg) / sizeof(struct sTCONREGTABLE));

	for ( i = 0 ; i < u1count ; i++)
    {
        u4tmp = sRegTg[i].u4Addr;
		u4tmp1 = (u4TconRegRead(u4tmp) & ~(sRegTg[i].u4Msk)) | (sRegTg[i].u4Val);

		vTconRegWrite(u4tmp,u4tmp1);
    }

  	LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegTg), sizeof(struct sTCONREGTABLE));

}

static void vDrvTgOn(void)
{
  LOG(1,"tg on\n");

  // GR0
  vTconRegWrite(0xf0034800,0x81feffff);
}

static void _vDrvTconSetPinMux(void)
{
#if defined(CC_MT5395)
    BSP_PinSet( PIN_SOE, PINMUX_FUNCTION1);  // SOE
    BSP_PinSet( PIN_POL, PINMUX_FUNCTION1);  // POL
    BSP_PinSet( PIN_VST, PINMUX_FUNCTION1);  // VST
    BSP_PinSet( PIN_GCLK1, PINMUX_FUNCTION1);  // GCLK1
    BSP_PinSet( PIN_GCLK2, PINMUX_FUNCTION1);  // GCLK2
    BSP_PinSet( PIN_GCLK3, PINMUX_FUNCTION1);  // GCLK3
    BSP_PinSet( PIN_GCLK4, PINMUX_FUNCTION1);  // GCLK4
    BSP_PinSet( PIN_GCLK5, PINMUX_FUNCTION1);  // GCLK5
    BSP_PinSet( PIN_GCLK6, PINMUX_FUNCTION1);  // GCLK6
    BSP_PinSet( PIN_FLK, PINMUX_FUNCTION1);  // FLK
    BSP_PinSet( PIN_DPM, PINMUX_FUNCTION1);  // DPM
    BSP_PinSet( PIN_VGH_ODD, PINMUX_FUNCTION1);  // VGH_ODD
    BSP_PinSet( PIN_VGH_EVEN, PINMUX_FUNCTION1);  // VGH_EVEN 
#elif defined(CC_MT5365)
    BSP_PinSet( PIN_GPIO1, PINMUX_FUNCTION1);  // H_CONV
    BSP_PinSet( PIN_GPIO2, PINMUX_FUNCTION1);  // OPT_N
    BSP_PinSet( PIN_GPIO3, PINMUX_FUNCTION1);  // GOE_A
    BSP_PinSet( PIN_GPIO4, PINMUX_FUNCTION1);  // SOE_A
    BSP_PinSet( PIN_GPIO7, PINMUX_FUNCTION1);  // FLK_A
    BSP_PinSet( PIN_GPIO8, PINMUX_FUNCTION1);  // POL_A
    BSP_PinSet( PIN_GPIO9, PINMUX_FUNCTION1);  // OPT_P
    BSP_PinSet( PIN_GPIO11, PINMUX_FUNCTION1);  // GSC_A
    BSP_PinSet( PIN_GPIO12, PINMUX_FUNCTION1);  // DPM_A
    BSP_PinSet( PIN_GPIO14, PINMUX_FUNCTION1);  // GSP_A
    BSP_PinSet( PIN_GPIO15, PINMUX_FUNCTION1);  // GSP_R_A

    BSP_PinSet( PIN_GPIO0, PINMUX_FUNCTION1);  // TG0, CS4
    BSP_PinSet( PIN_GPIO5, PINMUX_FUNCTION1);  // TG5, CS5
    BSP_PinSet( PIN_GPIO6, PINMUX_FUNCTION1);  // TG6, CS6    
    BSP_PinSet( PIN_GPIO10, PINMUX_FUNCTION1);  // GSC_A 
#endif

}

/**
 * @brief vDrvTconMainLoop

 * TCON main loop

 * @param  void
 * @retval void
 */
void vDrvTconMainLoop(void)
{

    if(_u1TconReady == 0) return;

    vLogTconStateChange(_u1TconState);		
    switch (_u1TconState)
    {
        case TCON_STATE_INIT:
            break;
			
        case TCON_STATE_STANDBY:
            break;
			
        case TCON_STATE_MUTE:
            break;

        case TCON_STATE_NORMAL:
            break;
			
        case TCON_STATE_ERROR:
            break;
			
        default:
            _u1TconState = TCON_STATE_INIT;
            break;
    }
	
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
/**
 * @brief u1DrvTconPreInit

 * Init TCON at DC on

 * @param  void
 * @retval 0: fail, 1: success
 */
UINT8 u1DrvTconPreInit(void)
{
    UINT8 u1Ret = 0;
	
	// init HW at DC on

    // mute Tcon output

	//TODO_JOSH
    // gpio(default as input) or pinmux for tcon function
    // pinmux as tcon function

	// tg output ctrl
	/*
	1. DPM, gamma power
	2. GSP
	3. FLK --> high or low, according to customer spec.
	*/
	//vIO32WriteFldAlign(GR0, 0, GR0_TCTG_OEN);

    // minilvds
    
    // disable gamma power
	vTconDPMCtrl(0);
    
	return(u1Ret);
}

/**
 * @brief u1DrvTconMLDSVOPLLFreqSet

 * Set vopll for MiniLVDS

 * @param  UINT32 u4Freq, (Hz)
 * @param  MLVDS_PORT: Single(0), Dual(4), Four(8)
 * @param  MLVDS_RATION: 1(0), 1P5(1), 2(2), 3(3), 4(4)
 * @retval void
 */


void u1DrvTconMLDSVOPLLFreqSet(UINT32 u4Freq, UINT8 MLVDS_PORT,UINT8 MLVDS_RATION)
{
 vDrvMLDSVOPLLFreqSet(u4Freq,MLVDS_PORT,MLVDS_RATION);
}


/**
 * @brief u1DrvTconInit

 * Init TCON

 * @param  void
 * @retval 0: fail, 1: success
 */
UINT8 u1DrvTconInit(void)
{
    UINT8 u1Ret = 0;

#ifndef CC_FPGA
    // tcon HW init
    vDrvTconHwInit();
#endif
	// init settings at booting
    // SW setting
    _u1TconState = TCON_STATE_INIT;
    _u1TconStateOld = TCON_STATE_NOTREADY;

    _u1TconForceMute = 0;
	_u1GammaReady = 0;

	// error handling
    vErrorHandleInit();

    // miniLVDS
    vDrvMiniLvdsInit();
	u1TconCustMlvdsInit();
    
    // mLVDS analog part
	vMiniLVDS_AnalogInit();

    // set pinmux
    _vDrvTconSetPinMux();
    
	// timing controller
	if (u1TconCustTgInit() == 0)
	{
	    vDrvTgInit();
	}
	
	vDrvTgOn();
	
	// enable gamma power
	vTconDPMCtrl(1);
	
	// Gamma voltage
	vDrvTconGammaInit();

    // Power On MiniLVDS
	vDrvMiniLvdsOn();

#ifndef CC_FPGA
    // tcon on
    vDrvTconOn();
#endif
	_u1TconReady = 1;

	return(u1Ret);
}

/**
 * @brief vDrvTconConfig

 * Configure TCON

 * @param  config
 * @retval 0: fail, 1: success
 */
UINT8 u1DrvTconConfig(UINT8 config)
{
    UINT8 u1Ret = 0;
	
    if(_u1TconReady == 0) return (0);

	// config Tcon settings
	
	return(u1Ret);	
}

void vDrvMiniLvdsConfig(UINT8 u1port, UINT8 u1pair, UINT8 u1bits)
{
    UINT8 u1tmp = 0;

	// minilvds ports
	u1tmp = 1;
	switch (u1port)
	{
		case MLVDS_SINGLE_PORT:
		    u1tmp = 0;
			break;
		case MLVDS_DUAL_PORT:
		    u1tmp = 1;
			break;
		case MLVDS_QUAL_PORT:
		    u1tmp = 2;
			break;
        default:
			LOG(1,"miniLVDS port number error %d!!!", u1port);
			break;
	}
    vIO32WriteFldAlign(MLVDS_B0, u1tmp, RG_MLVDS_PORT);

	// minilvds pairs
	u1tmp = 6;
	switch (u1pair)
	{
		case MLVDS_3_PAIR:
		    u1tmp = 3;
			break;
		case MLVDS_4_PAIR:
		    u1tmp = 4;
			break;
		case MLVDS_5_PAIR:
		    u1tmp = 5;
			break;
		case MLVDS_6_PAIR:
		    u1tmp = 6;
			break;
		case MLVDS_8_PAIR:
		    u1tmp = 8;
			break;
        default:
			LOG(1,"miniLVDS pair number error %d!!!", u1pair);
			break;
	}
    vIO32WriteFldAlign(MLVDS_B0, u1tmp, RG_MLVDS_PAIR);

	// minilvds bit length
	u1tmp = 1;
	switch (u1bits)
	{
		case MLVDS_6_BIT:
		    u1tmp = 0;
			break;
		case MLVDS_8_BIT:
		    u1tmp = 1;
			break;
		case MLVDS_10_BIT:
		    u1tmp = 2;
			break;
        default:
			LOG(1,"miniLVDS bit length error %d!!!", u1bits);
			break;
	}
    vIO32WriteFldAlign(MLVDS_B0, u1tmp, RG_MLVDS_BIT);

	LOG(1,"miniLvds config %d port %d pair %d bit\n", u1port, u1pair, u1bits);	
}


/**
 * @brief u1DrvTCONOffOn

 * TCON power control

 * @param  void
 * @retval 0: fail, 1: success
 */
UINT8 u1DrvTconOffOn(UINT8 val)
{
    UINT8 u1Ret = 0;
	
    if(_u1TconReady == 0) return (0);

	if (val)
	{
      vDrvErrorHandleEN(TCON_ENABLE);


	  // check gamma voltage first
	  if (_u1GammaReady == 0)
	  {
		LOG(1,"!!! init tcon gamma first\n");
		
	    u1Ret = 0;
	  	vDrvTconGammaInit();
	  }
	  else
	  {
	    // power on

		// enable DPM only when gamma voltage is ready
	  }
	  
	}
	else
	{
		// power down analog
		
		// power down all digital channel
      vDrvErrorHandleEN(TCON_DISABLE);		
		//power down bandgap and all reference circuit
		
		// analog reset
	}

	return(u1Ret);	
}

/**
 * @brief u1DrvTconMute

 * mute / unmute TCON

 * @param  void
 * @retval 0: fail, 1: success
 */
UINT8 u1DrvTconForceMute(UINT8 val)
{
    UINT8 u1Ret = 0;
	
    if(_u1TconReady == 0) return (0);

    if (val)
    {
    	// mute
    	_u1TconForceMute = 1;
    }
    else
    {
    	// unmute
    	_u1TconForceMute = 0;
    }

    return(u1Ret);
}


/**
 * @brief u1DrvTconIsReady

 * TCON Ready or not

 * @param  void
 * @retval 0: not ready, 1: ready
 */
UINT8 u1DrvTconIsReady(void)
{ 
    return (_u1TconReady ? 1:0);
}

/**
 * @brief u1DrvTconState

 * TCON state

 * @param  void
 * @retval state
 */
UINT8 u1DrvTconState(void)
{
    return (_u1TconState);
}

/**
 * @brief vDrvTconDump

 * TCON Dump

 * @param  void
 * @retval 0: fail, 1: success
 */
UINT8 u1DrvTconRegDump(void)
{
    UINT8 u1Ret = 1;
	UINT8 i = 0;
	UINT8 u1count;
	UINT32 u4tmp;
	  
    // CKGEN

	// error handling

	// post

	// minilvds
  	LOG(1,"minilvds reg dump\n");
	u1count = (sizeof(sRegMini) / sizeof(struct sTCONREGTABLE));
    for ( i = 0 ; i < u1count ; i++)
    {
        u4tmp = sRegMini[i].u4Addr;
		u4TconRegRead(u4tmp);
    }
    LOG(1,"Tcon reg dump %d counts, %d / %d\n", i, sizeof(sRegMini),sizeof(struct sTCONREGTABLE));
	


	// tg
  	LOG(1,"tg reg dump\n");
	u1count = (sizeof(sRegTg) / sizeof(struct sTCONREGTABLE));
    for ( i = 0 ; i < u1count ; i++)
    {
        u4tmp = sRegTg[i].u4Addr;
		u4TconRegRead(u4tmp);
    }
    LOG(1,"Tcon reg dump %d counts, %d / %d\n", i, sizeof(sRegTg),sizeof(struct sTCONREGTABLE));

	return(u1Ret);
}

/**
 * @brief u1DrvTconRegDiff

 * TCON register diff

 * @param  void
 * @retval 0: fail, 1: success
 */
UINT8 u1DrvTconRegDiff(void)
{
    UINT8 u1Ret = 1;
	UINT8 i = 0;
	UINT8 u1count;
	UINT32 u4tmp, u4tmp1;
	
	// minilvds
  	LOG(1,"minilvds reg diff\n");
	u1count = (sizeof(sRegMini) / sizeof(struct sTCONREGTABLE));
    for ( i = 0 ; i < u1count ; i++)
    {
        u4tmp = sRegMini[i].u4Addr;
		u4tmp1 = u4TconRegRead(u4tmp);
		if (u4tmp1 != sRegMini[i].u4Val)
		{
            LOG(1,"!!! diff addr 0x%8x, 0x%8x --> 0x%8x\n", u4tmp, u4tmp1, sRegMini[i].u4Val);			
		}
    }
    LOG(1,"Tcon reg diff %d counts, %d / %d\n", i, sizeof(sRegMini),sizeof(struct sTCONREGTABLE));

	// tg
  	LOG(1,"tg reg diff\n");
	u1count = (sizeof(sRegTg) / sizeof(struct sTCONREGTABLE));
    for ( i = 0 ; i < u1count ; i++)
    {
        u4tmp = sRegTg[i].u4Addr;
		u4tmp1 = u4TconRegRead(u4tmp);
		if (u4tmp1 != sRegTg[i].u4Val)
		{
            LOG(1,"!!! diff addr 0x%8x, 0x%8x --> 0x%8x\n", u4tmp, u4tmp1, sRegTg[i].u4Val);			
		}
    }
    LOG(1,"Tcon reg diff %d counts, %d / %d\n", i, sizeof(sRegTg),sizeof(struct sTCONREGTABLE));

    return (u1Ret);
}

/**
 * @brief vDrvTconDmsg

 * TCON status for debugging

 * @param  void
 * @retval void
 */
void vDrvTconDmsg(void)
{   
    UNUSED(_u1TconForceMute);
    // dump debug message
    LOG(1,"Tcon force muting: %d \n", _u1TconForceMute);

	// dump gamma chip settings
    //vDrvTconGammaDump();
}

/**
 * @brief vDrvTconCmd

 * TCON cmds

 * @param  void
 * @retval void
 */
UINT8 u1DrvTconDbgCmd(UINT8 cmd)
{
    UINT8 u1Ret = 0;

    switch (_u1TconCmd)
    {
    	case TCON_CMD_NONE:
            break;
			
        case TCON_CMD_INIT:
            break;
	    case TCON_CMD_GAMMA:
			vDrvTconGammaInit();
			break;
			
        case TCON_CMD_STANDBY:
            break;
			
        case TCON_CMD_POWERON:
            break;
			
        case TCON_CMD_POWEROFF:
            break;
			
        case TCON_CMD_MUTE:
            break;
			
        case TCON_CMD_UNMUTE:
            break;
			
        case TCON_CMD_REGDUMP:
            break;
			
        case TCON_CMD_DMSG:
            break;
			
        case TCON_CMD_DLEVEL:
            break;
			
        case TCON_CMD_FW_STOP:
            break;
			
        case TCON_CMD_FW_RESUME:
            break;
		default:
			break;
    }	
	
    return(u1Ret);
}

