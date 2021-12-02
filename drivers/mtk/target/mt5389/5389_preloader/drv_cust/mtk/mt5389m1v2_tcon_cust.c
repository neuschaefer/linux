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
 * $Author: muran.xiang $
 * $Date: 2012/06/01 $
 * $RCSfile: drv_tcon.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/
#include "sv_const.h"
#include "feature.h"

#include "hw_tg.h"

#include "sif_if.h"

#include "drv_tcon.h"

#ifdef DEFINE_IS_LOG
#undef DEFINE_IS_LOG
#endif
#ifdef CC_MTK_LOADER
#define DEFINE_IS_LOG(level, fmt...)   Printf(fmt)
#else /* CC_MTK_LOADER */
#define DEFINE_IS_LOG   IR_IsLog
#endif /* CC_MTK_LOADER */



//#define EPI_V12
#if 0//def EPI_V12
// for gamma IC
#define GAMMAIC_BUS 0
#define GAMMAIC_CLK_RATE 0x100
#define GAMMAIC_ID 0x40
#define GAMMAIC_DATA_CNT 1


// Gamma
struct sTCONREGTABLE sRegCustGamma[] = {
  // Gamma voltage
  {0x0000, 0x26, 0xffffffff},
  {0x0001, 0x09, 0xffffffff},
  {0x0002, 0x03, 0xffffffff},
  {0x0003, 0x03, 0xffffffff},
  {0x0004, 0x0B, 0xffffffff},
  {0x0005, 0x0B, 0xffffffff},
  {0x0006, 0x20, 0xffffffff},
  {0x0007, 0x01, 0xffffffff},
  {0x0008, 0x03, 0xffffffff},
  {0x0009, 0x03, 0xffffffff},
  {0x000A, 0x1, 0xffffffff},
  {0x000B, 0x42, 0xffffffff},
  {0x000C, 0x1, 0xffffffff},
  {0x000D, 0x04, 0xffffffff},
  {0x000E, 0x0, 0xffffffff},
  {0x000F, 0x81, 0xffffffff},
  {0x0010, 0x1, 0xffffffff},
  {0x0011, 0x78, 0xffffffff},
  {0x0012, 0x1, 0xffffffff},
  {0x0013, 0x01, 0xffffffff},
  {0x0014, 0x0, 0xffffffff},
  {0x0015, 0xC3, 0xffffffff},
  {0x0016, 0x0, 0xffffffff},
  {0x0017, 0xB2, 0xffffffff},
  {0x0018, 0x4, 0xffffffff},
};
#else
// for gamma IC
#define GAMMAIC_BUS 0
#define GAMMAIC_CLK_RATE 0x100
#define GAMMAIC_ID (0x74 << 1)
#define GAMMAIC_DATA_CNT 2




// Gamma
struct sTCONREGTABLE sRegCustGamma[] = {
  // Gamma voltage
  {0x0000, 0x6B03, 0xffffffff},
  {0x0001, 0xEB03, 0xffffffff},
  {0x0002, 0x6B03, 0xffffffff},
  {0x0003, 0x3E03, 0xffffffff},
  {0x0004, 0xFD02, 0xffffffff},
  {0x0005, 0xB602, 0xffffffff},
  {0x0006, 0x8402, 0xffffffff},
  {0x0007, 0x0902, 0xffffffff},
  {0x0008, 0xEF01, 0xffffffff},
  {0x0009, 0x7601, 0xffffffff},
  {0x000A, 0x4201, 0xffffffff},
  {0x000B, 0xFB00, 0xffffffff},
  {0x000C, 0xB900, 0xffffffff},
  {0x000D, 0x8E00, 0xffffffff},
  {0x000E, 0x1300, 0xffffffff},
  {0x000F, 0x8E00, 0xffffffff},
  {0x0012, 0xB581, 0xffffffff},
};
#endif

#if !defined(CC_MTK_LOADER) || !defined(CC_LOADER_SHRINK_TCON) 

//-----------------------------------------------------------------------------
// mini LVDS Interface Setting
//-----------------------------------------------------------------------------

//miniLVDS LGDV4/V5/V6
  struct sTCONREGTABLE sRegMini_PANEL_MLVDS_LGDV4FHD60[] = {

   // minilvds Pair select
 {0xf0035104, 0x00012456, 0xffffffff}, // Pair Selection of L Link 
 {0xf0035108, 0x00012456, 0xffffffff}, // Pair Selection of R Link  
 {0xf003510C, 0xBABA9898, 0xffffffff}, 

   //minilvds reset pulse
   {0xf003511c, 0x00000000, 0xffffffff},
   {0xf0035120, 0x00000000, 0xffffffff},
   {0xf0035100, 0x0000003f, 0xffffffff},

   //minilvds PN Swap Setting
 {0xf0035130, 0x00000000, 0xffffffff},
	
 // minilvds Mute Pattern Color and Enable
 {0xf0035114, 0x0000cFF2, 0xffffffff},
	
 // bit[20]: RL Link Swap
 {0xf0035110, 0x00100000, 0xffffffff},
	
 // minilvds Output Enable
 {0xf0035118, 0x00000000, 0xffffffff},

 // clk source
 //Interface Selection, Blue Shift, bit Selection, Pair Selection, Port Selection
 {0xf0035000, 0x2002a359, 0xffffffff},

 // minilvds input source
 {0xf0035280, 0x80000000, 0xffffffff}, //Input Source, WXGA Dummy Pixel

};

struct sTCONREGTABLE sRegMini_PANEL_MLVDS_LGDV4FHD120[] = {

 // minilvds Pair select
 {0xf0035104, 0x00012456, 0xffffffff}, // Pair Selection of L Link 
 {0xf0035108, 0x00012456, 0xffffffff}, // Pair Selection of R Link  
 {0xf003510C, 0xBABA9898, 0xffffffff}, 

 //minilvds reset pulse
 {0xf003511c, 0x00000000, 0xffffffff},
 {0xf0035120, 0x00000000, 0xffffffff},
 {0xf0035100, 0x0000003f, 0xffffffff},

 //minilvds PN Swap Setting
 {0xf0035130, 0x00000000, 0xffffffff},
	  
   // minilvds Mute Pattern Color and Enable
   {0xf0035114, 0x0000cFF2, 0xffffffff},
	  
   // bit[20]: RL Link Swap
   {0xf0035110, 0x00100000, 0xffffffff},
	  
   // minilvds Output Enable
   {0xf0035118, 0x00000000, 0xffffffff},

   // clk source
   //Interface Selection, Blue Shift, bit Selection, Pair Selection, Port Selection
   {0xf0035000, 0x2002a359, 0xffffffff},

   // minilvds input source
   {0xf0035280, 0x80000000, 0xffffffff}, //Input Source, WXGA Dummy Pixel

  };

//miniLVDS CMO 2011 Flip-Pixel-2
  struct sTCONREGTABLE sRegMini_PANEL_MLVDS_CMO2011FHD60[] = {
  
	// minilvds Pair select
	{0xf0035104, 0x00012456, 0xffffffff},  // Pair Selection of L Link 
	{0xf0035108, 0x00012456, 0xffffffff},  // Pair Selection of R Link  
    {0xf003510C, 0xBABA9898, 0xffffffff},
  
	//minilvds reset pulse
	{0xf003511c, 0x00000000, 0xffffffff},
	{0xf0035120, 0x00000000, 0xffffffff},
	{0xf0035100, 0x0000003f, 0xffffffff},
  
	//minilvds PN Swap Setting
	{0xf0035130, 0x00000000, 0xffffffff},
		
	// minilvds Mute Pattern Color and Enable
	{0xf0035114, 0x0000cFF2, 0xffffffff},
		
	// bit[20]: RL Link Swap
	{0xf0035110, 0x00000000, 0xffffffff},
		
	// minilvds Output Enable
	{0xf0035118, 0x00000000, 0xffffffff},
  
	// clk source
	//Interface Selection, Blue Shift, bit Selection, Pair Selection, Port Selection
	{0xf0035000, 0x2002A359, 0xffffffff},
  
	// minilvds input source
	{0xf0035280, 0x80000000, 0xffffffff}, //Input Source, WXGA Dummy Pixel
  
  };

 struct sTCONREGTABLE sRegMini_PANEL_MLVDS_CMO2011FHD120[] = {

  // minilvds Pair select
  {0xf0035104, 0x00012456, 0xffffffff},  // Pair Selection of L Link 
  {0xf0035108, 0x00012456, 0xffffffff},  // Pair Selection of R Link	
  {0xf003510C, 0xBABA9898, 0xffffffff},
  
  //minilvds reset pulse
  {0xf003511c, 0x00000000, 0xffffffff},
  {0xf0035120, 0x00000000, 0xffffffff},
  {0xf0035100, 0x0000003f, 0xffffffff},

  //minilvds PN Swap Setting
  {0xf0035130, 0x00000000, 0xffffffff},
	  
  // minilvds Mute Pattern Color and Enable
  {0xf0035114, 0x0000cFF2, 0xffffffff},
	  
  // bit[20]: RL Link Swap
  {0xf0035110, 0x00000000, 0xffffffff},
	  
  // minilvds Output Enable
  {0xf0035118, 0x00000000, 0xffffffff},

  // clk source
  //Interface Selection, Blue Shift, bit Selection, Pair Selection, Port Selection
  {0xf0035000, 0x2002A359, 0xffffffff},
  
	// minilvds input source
	{0xf0035280, 0x80000000, 0xffffffff}, //Input Source, WXGA Dummy Pixel
  
  };

 struct sTCONREGTABLE sRegMini_PANEL_MLVDS_PANASONIC32WXGA8BIT[] = {

  // clk source
   {0xf0035000, 0x2012A358, 0xffffffff},
  	
  // Bit[20]: mini LVDS LR swap
  {0xf0035110, 0x00000000, 0xffffffff},

  // minilvds pair select
   {0xf0035104, 0x00012456, 0xffffffff},
   {0xf0035108, 0x00012456, 0xffffffff},
   {0xf003510C, 0xBABA9898, 0xffffffff},
    
  // minilvds reset pulse
  {0xf0035100, 0x0000003f, 0xffffffff},

  // minilvds PN setting
  {0xf0035130, 0x00000000, 0xffffffff},
  
  // mute pattern H/V total, WXGA
  {0xf0035004, 0x05580628, 0xffffffff},
  {0xf0035008, 0x0300033A, 0xffffffff},
  
  // WXGA Dummy Pixel
  {0xf0035280, 0x82AA0028, 0xffffffff},
  
  // Fix DE
  //0xf0034468, 0x05420558, 0xffffffff,
};

struct sTCONREGTABLE sRegMini_PANEL_MLVDS_SAMSUNGMB4[] = {

  // minilvds Pair select
 {0xf0035104, 0x00012456, 0xffffffff}, // Pair Selection of L Link 
 {0xf0035108, 0x00012456, 0xffffffff}, // Pair Selection of R Link  
 {0xf003510C, 0xBABA9898, 0xffffffff}, 

  //minilvds reset pulse
  {0xf003511c, 0x00000000, 0xffffffff},
  {0xf0035120, 0x00000000, 0xffffffff},
  {0xf0035100, 0x0000003f, 0xffffffff},
	  
  //minilvds PN Swap Setting
 {0xf0035130, 0x00000000, 0xffffffff},
		  
  // minilvds Mute Pattern Color and Enable
  {0xf0035114, 0x0000cFF2, 0xffffffff},
	  
  // bit[20]: RL Link Swap
 {0xf0035110, 0x00100000, 0xffffffff},
	  
  // minilvds Output Enable
  {0xf0035118, 0x00000000, 0xffffffff},
	  
  // clk source
  //Interface Selection, Blue Shift, bit Selection, Pair Selection, Port Selection
 {0xf0035000, 0x2302AB59, 0xffffffff},

  // minilvds input source
  {0xf0035280, 0x80000000, 0xffffffff}, //Input Source, WXGA Dummy Pixel

 //MB4 Enable
 {0xf00350dc, 0x80000000, 0xffffffff},

  };

struct sTCONREGTABLE sRegMini_PANEL_MLVDS_SAMSUNGMB7[] = {


 // minilvds Pair select
 {0xf0035104, 0x00012456, 0xffffffff}, // Pair Selection of L Link 
 {0xf0035108, 0x00012456, 0xffffffff}, // Pair Selection of R Link  
 {0xf003510C, 0xBABA9898, 0xffffffff}, 

 //minilvds reset pulse
 {0xf003511c, 0x00000000, 0xffffffff},
 {0xf0035120, 0x00000000, 0xffffffff},
 {0xf0035100, 0x0000003f, 0xffffffff},

 //minilvds PN Swap Setting
 {0xf0035130, 0x00000000, 0xffffffff},
	
 // minilvds Mute Pattern Color and Enable
 {0xf0035114, 0x0000cFF2, 0xffffffff},
	
 // bit[20]: RL Link Swap
 {0xf0035110, 0x00100000, 0xffffffff},
	
 // minilvds Output Enable
 //0xf0035118, 0x00000000, 0xffffffff,
 
 // clk source
 //Interface Selection, Blue Shift, bit Selection, Pair Selection, Port Selection
 {0xf0035000, 0x2302AB59, 0xffffffff},

 // minilvds input source
 {0xf0035280, 0x80000000, 0xffffffff}, //Input Source, WXGA Dummy Pixel

 //MB4 Enable
 {0xf00350dc, 0x80000000, 0xffffffff},

 {0xf0035004, 0x083C0898, 0xffffffff},
 {0xf00350dc, 0x10000000, 0xffffffff},
 {0xf00350c0, 0x8100ecef, 0xffffffff},
 {0xf00350cc, 0x00200020, 0xffffffff},
 {0xf00350c4, 0x3f1f3f1f, 0xffffffff},
 {0xf00350c8, 0x3f1f0020, 0xffffffff},
 {0xf00350d0, 0x20160400, 0xffffffff},
 {0xf00350d4, 0x0000e0a1, 0xffffffff},
 {0xf00350d8, 0x00b7016c, 0xffffffff},
 {0xf0035018, 0x000202a0, 0xffffffff},
 {0xf0035114, 0x0000fffe, 0xffffffff},
 {0xf0035280, 0x00000003, 0xffffffff},
 {0xf0035118, 0x00000000, 0xffffffff},
 {0xf003513c, 0x00000001, 0xffffffff},
 {0xf0035000, 0x200a8859, 0xffffffff},
 {0xf0035100, 0x0000003F, 0xffffffff},
 {0xf003512c, 0x84420000, 0xffffffff},

 //0xf0035118, 0xffffffff, 0xffffffff,
 
};

//-----------------------------------------------------------------------------
// EPI Interface Setting
//-----------------------------------------------------------------------------

  //EPI LGDV11FHD60
  struct sTCONREGTABLE sReg_PANEL_EPI_LGDV11FHD60[] = {

  //panel interface
  //0xf0034600,  0x0400a359, 0xffffffff,          // bit[16] bypass 6line bit[20]: disable 2ch // modify  Y.W.
//  0xf0034600, 0x0402A359, 0xffffffff,  // bit[16] bypass 6line bit[20]: disable 2ch // modify  Y.W.

//  0xf0034604, 0x07800894, 0xffffffff,  // h active, h total 
  {0xf0034710, 0x80000000, 0xffffffff}, 
  //0xf0034614, 0x03090100, 0xffffffff,
//  0xf0034618, 0x02090100, 0xffffffff,  // bit[16] enable ptgen, bit[17] disable 2ch bit[19] enable seq, bit[20] disable mirror

  //pattern gen pure color
  //0xf0034618, 0x01090100, 0xffffffff,              // 24:25= 1 pure
  {0xf0034614, 0x000003ff, 0xffffffff},  //RGB [9:0] B [19:10] G [29:20] R

  //mlvds
  {0xf0035000, 0x8002A159, 0xffffffff}, 
  {0xf003501c, 0x80f0000c, 0xffffffff}, 
  {0xf0035018, 0x010002a0, 0xffffffff}, 
  {0xf0035124, 0x00000080, 0xffffffff},

  //EPI
  //TCON_TOP
  {0xf0034014, 0x00210000, 0xffffffff},

  //SOE
  {0xf0035508, 0x00000600, 0xffffffff},

  {0xf003541C, 0x00000000, 0xffffffff},  // disenable each lane ctrl (ctrl2)
  {0xf0035500, 0xfdfe0103, 0xffffffff},  //modify yw 11/10 mute blue shift[8]
  //0xf0035D00,  0xFDFE3F03, 0xffffffff,         //modify yw 11/10 mute blue shift[8]

  {0xf003551c, 0x04380006, 0xffffffff},  //v active [31:16] v start [15:0] = 5
  {0xf0035524, 0x00020002, 0xffffffff},  //h width [31:16] h start [15:0]
  {0xf0035528, 0x00000000, 0xffffffff},  //G[25:16] R[9:0]
  {0xf003552c, 0x000003ff, 0xffffffff},  //B[9:0]

  {0xf0035530, 0x76543211, 0xffffffff}, 
  {0xf0035534, 0xfedcba08, 0xffffffff}, 

  {0xf003550c, 0x00001800, 0xffffffff}, 
  {0xf0035400, 0x00001601, 0xffffffff}, 	 // 8-bit

  {0xf0035514, 0x00000000, 0xffffffff},
  //0x0000a514, 0x01000001, 0xffffffff,             //epi sw_mute_en
  {0xf0035514, 0x00000001, 0xffffffff},

  };


//-----------------------------------------------------------------------------
// Timing Control Signal Setting
//-----------------------------------------------------------------------------

//LGDV4FHD60
//tg
struct sTCONREGTABLE sRegCustTg_MLVDS_LGDV4FHD60[] = {
	// SOE, TG0
  {0xf0034840, 0x00780850, 0xffffffff},
  {0xf0034844, 0x00000000, 0xffffffff},
  {0xf0034848, 0x00780850, 0xffffffff},  
  {0xf003484c, 0x04430007, 0xffffffff},
  {0xf0034850, 0x20000000, 0xffffffff},
  {0xf0034854, 0x80000000, 0xffffffff},
  {0xf0034858, 0x00000007, 0xffffffff},
  {0xf003485c, 0x00000000, 0xffffffff},
	
  // POL, TG1
  {0xf0034860, 0x07080708, 0xffffffff},
  {0xf0034864, 0x00000000, 0xffffffff},
  {0xf0034868, 0x07080708, 0xffffffff},  
  {0xf003486c, 0x04430007, 0xffffffff},
  {0xf0034870, 0x80000404, 0xffffffff},
  {0xf0034874, 0x80020004, 0xffffffff},
  {0xf0034878, 0x00000007, 0xffffffff},
  {0xf003487c, 0x10000000, 0xffffffff},
	
  // GSP, TG11
  {0xf00349A0, 0x04060406, 0xffffffff},
  {0xf00349A4, 0x00000000, 0xffffffff},
  {0xf00349A8, 0x00000000, 0xffffffff},
  {0xf00349Ac, 0x00080007, 0xffffffff},
  {0xf00349B0, 0x00000000, 0xffffffff},
  {0xf00349B4, 0x00000000, 0xffffffff},
  {0xf00349B8, 0x00000004, 0xffffffff},
  {0xf00349Bc, 0x00000000, 0xffffffff},
	
  // GSP_R, TG8
  {0xf0034940, 0x04060406, 0xffffffff},
  {0xf0034944, 0x00000000, 0xffffffff},
  {0xf0034948, 0x00000000, 0xffffffff},
  {0xf003494c, 0x00080007, 0xffffffff},
  {0xf0034950, 0x00000000, 0xffffffff},
  {0xf0034954, 0x00000000, 0xffffffff},
  {0xf0034958, 0x00000004, 0xffffffff},
  {0xf003495c, 0x00000000, 0xffffffff},
	
  // GOE, TG2
  {0xf0034880, 0x07BC062C, 0xffffffff},
  {0xf0034884, 0x00000000, 0xffffffff},
  {0xf0034888, 0x07BC062C, 0xffffffff},
  {0xf003488c, 0x04420006, 0xffffffff},
  {0xf0034890, 0x20000000, 0xffffffff},
  {0xf0034894, 0x80000000, 0xffffffff},
  {0xf0034898, 0x00000007, 0xffffffff},
  {0xf003489c, 0x00000000, 0xffffffff},
	  
  // GSC, TG12
  {0xf00349c0, 0x07260406, 0xffffffff},
  {0xf00349c4, 0x00000000, 0xffffffff},
  {0xf00349c8, 0x07260406, 0xffffffff},
  {0xf00349cc, 0x04420006, 0xffffffff},
  {0xf00349d0, 0x20000000, 0xffffffff},
  {0xf00349d4, 0x80000000, 0xffffffff},
  {0xf00349d8, 0x00000007, 0xffffffff},
  {0xf00349dc, 0x0f000000, 0xffffffff},
	  
  // FLK, TG9  
  {0xf0034960, 0x049C0726, 0xffffffff},
  {0xf0034964, 0x00000000, 0xffffffff},
  {0xf0034968, 0x049C0726, 0xffffffff},
  {0xf003496c, 0x04420006, 0xffffffff},
  {0xf0034970, 0x20000000, 0xffffffff},
  {0xf0034974, 0x80000000, 0xffffffff},
  {0xf0034978, 0x00000007, 0xffffffff},
  {0xf003497c, 0x00000000, 0xffffffff},
	  
  // DPM, TG10
  {0xf0034820, 0x04000400, 0x04000400}, //GPO :1
	  
  // H_CONV, TG5
  {0xf0034820, 0x00000020, 0x00000020}, //GPO :0
	
  // OPT_N, TG6
  {0xf0034820, 0x00400040, 0x00400040}, //GPO :1
	  
  // 0PT_P , TG7
  {0xf0034820, 0x00000080, 0x00000080}  //GPO :0

};

struct sTCONREGTABLE sRegCustTg_MLVDS_LGDV4FHD120[] = {
  // SOE, TG0
  {0xf0034840, 0x00780850, 0xffffffff},
  {0xf0034844, 0x00000000, 0xffffffff},
  {0xf0034848, 0x00780850, 0xffffffff},  
  {0xf003484c, 0x04430007, 0xffffffff},
  {0xf0034850, 0x20000000, 0xffffffff},
  {0xf0034854, 0x80000000, 0xffffffff},
  {0xf0034858, 0x00000007, 0xffffffff},
  {0xf003485c, 0x00000000, 0xffffffff},

  // POL, TG1
  {0xf0034860, 0x07080708, 0xffffffff},
  {0xf0034864, 0x00000000, 0xffffffff},
  {0xf0034868, 0x07080708, 0xffffffff},  
  {0xf003486c, 0x04430007, 0xffffffff},
  {0xf0034870, 0x80000404, 0xffffffff},
  {0xf0034874, 0x80020004, 0xffffffff},
  {0xf0034878, 0x00000007, 0xffffffff},
  {0xf003487c, 0x10000000, 0xffffffff},

  // GSP, TG11
  {0xf00349A0, 0x04060406, 0xffffffff},
  {0xf00349A4, 0x00000000, 0xffffffff},
  {0xf00349A8, 0x00000000, 0xffffffff},
  {0xf00349Ac, 0x00080007, 0xffffffff},
  {0xf00349B0, 0x00000000, 0xffffffff},
  {0xf00349B4, 0x00000000, 0xffffffff},
  {0xf00349B8, 0x00000004, 0xffffffff},
  {0xf00349Bc, 0x00000000, 0xffffffff},
                                    
  // GSP_R, TG8                     
  {0xf0034940, 0x04060406, 0xffffffff},
  {0xf0034944, 0x00000000, 0xffffffff},
  {0xf0034948, 0x00000000, 0xffffffff},
  {0xf003494c, 0x00080007, 0xffffffff},
  {0xf0034950, 0x00000000, 0xffffffff},
  {0xf0034954, 0x00000000, 0xffffffff},
  {0xf0034958, 0x00000004, 0xffffffff},
  {0xf003495c, 0x00000000, 0xffffffff},
                                    
  // GOE, TG2                       
  {0xf0034880, 0x07BC062C, 0xffffffff},
  {0xf0034884, 0x00000000, 0xffffffff},
  {0xf0034888, 0x07BC062C, 0xffffffff},
  {0xf003488c, 0x04420006, 0xffffffff},
  {0xf0034890, 0x20000000, 0xffffffff},
  {0xf0034894, 0x80000000, 0xffffffff},
  {0xf0034898, 0x00000007, 0xffffffff},
  {0xf003489c, 0x00000000, 0xffffffff},
                                    
  // GSC, TG12                      
  {0xf00349c0, 0x07260406, 0xffffffff},
  {0xf00349c4, 0x00000000, 0xffffffff},
  {0xf00349c8, 0x07260406, 0xffffffff},
  {0xf00349cc, 0x04420006, 0xffffffff},
  {0xf00349d0, 0x20000000, 0xffffffff},
  {0xf00349d4, 0x80000000, 0xffffffff},
  {0xf00349d8, 0x00000007, 0xffffffff},
  {0xf00349dc, 0x0f000000, 0xffffffff},
                                    
  // FLK, TG9                       
  {0xf0034960, 0x049C0726, 0xffffffff},
  {0xf0034964, 0x00000000, 0xffffffff},
  {0xf0034968, 0x049C0726, 0xffffffff},
  {0xf003496c, 0x04420006, 0xffffffff},
  {0xf0034970, 0x20000000, 0xffffffff},
  {0xf0034974, 0x80000000, 0xffffffff},
  {0xf0034978, 0x00000007, 0xffffffff},
  {0xf003497c, 0x00000000, 0xffffffff},
                                    
  // DPM, TG10                      
  {0xf0034820, 0x04000400, 0x04000400}, //GPO :1
                                    
  // H_CONV, TG5                    
  {0xf0034820, 0x00000020, 0x00000020}, //GPO :0
                                    
  // OPT_N, TG6                     
  {0xf0034820, 0x00400040, 0x00400040}, //GPO :1
                                    
  // 0PT_P , TG7                    
  {0xf0034820, 0x00000080, 0x00000080}  //GPO :0
  };

//CMO  2011 Flip-Pixel-2
 struct sTCONREGTABLE sRegTg_PANEL_MLVDS_CMO2011FHD60[] = {
                            													
  // SOE, TG0
  {0xf0034840, 0x00400840, 0xffffffff},
  {0xf0034844, 0x00000000, 0xffffffff},
  {0xf0034848, 0x00400840, 0xffffffff},
  {0xf003484c, 0x04430007, 0xffffffff},
  {0xf0034850, 0x20000000, 0xffffffff},
  {0xf0034854, 0x80000000, 0xffffffff},
  {0xf0034858, 0x00000007, 0xffffffff},
  {0xf003485c, 0x00000000, 0xffffffff},																													
                                       																														
  // POL, TG1                    																														
  {0xf0034860, 0x07080708, 0xffffffff},
  {0xf0034864, 0x00000000, 0xffffffff},																														
  {0xf0034868, 0x07080708, 0xffffffff},
  {0xf003486c, 0x00070007, 0xffffffff},
  {0xf0034870, 0x80000404, 0xffffffff},																														
  {0xf0034874, 0x80040004, 0xffffffff},
  {0xf0034878, 0x00000007, 0xffffffff},																														
  {0xf003487c, 0x10000000, 0xffffffff},																														
                                       																														
  // GSP, TG11                   																														
  {0xf00349A0, 0x04060406, 0xffffffff},
  {0xf00349A4, 0x00000000, 0xffffffff},
  {0xf00349A8, 0x00000000, 0xffffffff},
  {0xf00349Ac, 0x00080006, 0xffffffff},
  {0xf00349B0, 0x00000000, 0xffffffff},
  {0xf00349B4, 0x00000000, 0xffffffff},
  {0xf00349B8, 0x00000004, 0xffffffff},
  {0xf00349Bc, 0x00000000, 0xffffffff},
  																												
   // GOE, TG2				 
  {0xf0034880, 0x08200600, 0xffffffff},				 
  {0xf0034884, 0x00000000, 0xffffffff},
  {0xf0034888, 0x08200600, 0xffffffff},				 
  {0xf003488c, 0x04430007, 0xffffffff},				 
  {0xf0034890, 0x20000000, 0xffffffff},				 
  {0xf0034894, 0x80000000, 0xffffffff},				 
  {0xf0034898, 0x00000007, 0xffffffff},
  {0xf003489c, 0x00000000, 0xffffffff},				 
                                    
   // GSC, TG12 			  
  {0xf00349C0, 0x07C00406, 0xffffffff},				 
  {0xf00349C4, 0x00000000, 0xffffffff},				 
  {0xf00349C8, 0x07C00406, 0xffffffff},				 
  {0xf00349Cc, 0x04430007, 0xffffffff},				 
  {0xf00349D0, 0x20000000, 0xffffffff},				 
  {0xf00349D4, 0x80000000, 0xffffffff},				 
  {0xf00349D8, 0x00000107, 0xffffffff},				 
  {0xf00349Dc, 0x0f000000, 0xffffffff},				 
     // FLK, TG9				   
  {0xf0034960, 0x049C0726, 0xffffffff},				 
  {0xf0034964, 0x00000000, 0xffffffff},				 
  {0xf0034968, 0x049C0726, 0xffffffff},				 
  {0xf003496c, 0x04430004, 0xffffffff},				 
  {0xf0034970, 0x20000000, 0xffffffff},				 
  {0xf0034974, 0x80000000, 0xffffffff},				 
  {0xf0034978, 0x00000007, 0xffffffff},				 
  {0xf003497c, 0x00000000, 0xffffffff},				 
                                    
  // DPM, TG10            																														
  {0xf0034820, 0x04000400, 0x04000400}, //GPO :1
    																						                                    																														
  // H_CONV, TG5			  
  {0xf0034820, 0x00000020, 0x00000020}, //GPO :0 			
                                    
  // OPT_N, TG6			
  {0xf00348f8, 0x00400040, 0x00400040}, //GPO :1 			 
			     
  // 0PT_P , TG7			 
  {0xf0034918, 0x00000080, 0x00000080}  //GPO :0 			  
  };

 struct sTCONREGTABLE sRegTg_PANEL_MLVDS_CMO2011FHD120[] = {
  
  // SOE, TG0
	{0xf0034840, 0x00400840, 0xffffffff},
	{0xf0034844, 0x00000000, 0xffffffff},
	{0xf0034848, 0x00400840, 0xffffffff},
	{0xf003484c, 0x04430007, 0xffffffff},
	{0xf0034850, 0x20000000, 0xffffffff},
	{0xf0034854, 0x80000000, 0xffffffff},
	{0xf0034858, 0x00000007, 0xffffffff},
	{0xf003485c, 0x00000000, 0xffffffff},
	
	// POL, TG1
	{0xf0034860, 0x07080708, 0xffffffff},
	{0xf0034864, 0x00000000, 0xffffffff},
	{0xf0034868, 0x07080708, 0xffffffff},
	{0xf003486c, 0x00070007, 0xffffffff},
	{0xf0034870, 0x80000404, 0xffffffff},
	{0xf0034874, 0x80040004, 0xffffffff},
	{0xf0034878, 0x00000007, 0xffffffff},
	{0xf003487c, 0x10000000, 0xffffffff},
	
	// GSP, TG11
	{0xf00349A0, 0x04060406, 0xffffffff},
	{0xf00349A4, 0x00000000, 0xffffffff},
	{0xf00349A8, 0x00000000, 0xffffffff},
	{0xf00349Ac, 0x00080006, 0xffffffff},
	{0xf00349B0, 0x00000000, 0xffffffff},
	{0xf00349B4, 0x00000000, 0xffffffff},
	{0xf00349B8, 0x00000004, 0xffffffff},
	{0xf00349Bc, 0x00000000, 0xffffffff},
                           
	// GOE, TG2               
	{0xf0034880, 0x08200600, 0xffffffff},               
	{0xf0034884, 0x00000000, 0xffffffff},               
	{0xf0034888, 0x08200600, 0xffffffff},               
	{0xf003488c, 0x04430007, 0xffffffff},               
	{0xf0034890, 0x20000000, 0xffffffff},               
	{0xf0034894, 0x80000000, 0xffffffff},               
	{0xf0034898, 0x00000007, 0xffffffff},               
	{0xf003489c, 0x00000000, 0xffffffff},               
	               
	// GSC, TG12               
	{0xf00349C0, 0x07C00406, 0xffffffff},               
	{0xf00349C4, 0x00000000, 0xffffffff},               
	{0xf00349C8, 0x07C00406, 0xffffffff},               
	{0xf00349Cc, 0x04430007, 0xffffffff},               
	{0xf00349D0, 0x20000000, 0xffffffff},               
	{0xf00349D4, 0x80000000, 0xffffffff},               
	{0xf00349D8, 0x00000107, 0xffffffff},               
	{0xf00349Dc, 0x0f000000, 0xffffffff},               
	               
	// FLK, TG9                 
	{0xf0034960, 0x049C0726, 0xffffffff},               
	{0xf0034964, 0x00000000, 0xffffffff},               
	{0xf0034968, 0x049C0726, 0xffffffff},               
	{0xf003496c, 0x04430004, 0xffffffff},               
	{0xf0034970, 0x20000000, 0xffffffff},               
	{0xf0034974, 0x80000000, 0xffffffff},               
	{0xf0034978, 0x00000007, 0xffffffff},               
	{0xf003497c, 0x00000000, 0xffffffff},               
	               
	// DPM, TG10
	{0xf0034820, 0x04000400, 0x04000400}, //GPO :1               
	               
	// H_CONV, TG5             
	{0xf0034820, 0x00000020, 0x00000020}, //GPO :0             
	               
	// OPT_N, TG6            
	{0xf00348f8, 0x00400040, 0x00400040}, //GPO :1              
	               
	// 0PT_P , TG7            
	{0xf0034918, 0x00000080, 0x00000080}  //GPO :0               
  };

  struct sTCONREGTABLE sRegCustTg_PANASONIC32WXGA8BIT[] = {

	 // TP2 for DATA1
	{0xF0034880, 0x03850321, 0xffffffff},
	{0xF0034884, 0x00000000, 0xffffffff},
	{0xF0034888, 0x03850321, 0xffffffff},
	{0xF003488C, 0x00070006, 0xffffffff},
	{0xF0034890, 0x20000000, 0xffffffff},
	{0xF0034894, 0x80000000, 0xffffffff},
	{0xF0034898, 0x00000007, 0xffffffff},
	{0xF003489C, 0x00000000, 0xffffffff},  
                                          
	 // TP14 for DATA1                
	{0xF0034A00, 0x03210321, 0xffffffff},
	{0xF0034A04, 0x00000000, 0xffffffff},
	{0xF0034A08, 0x00000000, 0xffffffff},
	{0xF0034A0c, 0x00090008, 0xffffffff},
	{0xf0034A10, 0x00000000, 0xffffffff},
	{0xf0034A14, 0x00000000, 0xffffffff},
	{0xf0034A18, 0x00000004, 0xffffffff},
	{0xf0034A1c, 0x0F000000, 0xffffffff},  
                                          
	 // TPK5 for DATA2                
	{0xf00348E0, 0x03850321, 0xffffffff},
	{0xf00348E4, 0x00000000, 0xffffffff},
	{0xf00348E8, 0x03850321, 0xffffffff},
	{0xf00348EC, 0x00080007, 0xffffffff},
	{0xf00348F0, 0x20000000, 0xffffffff},
	{0xf00348F4, 0x80000000, 0xffffffff},
	{0xf00348F8, 0x00000007, 0xffffffff},
	{0xf00348FC, 0x00000000, 0xffffffff},  
                                          
	 // TPK1 for DATA2                
	{0xf0034860, 0x03210321, 0xffffffff},
	{0xf0034864, 0x00000000, 0xffffffff},
	{0xf0034868, 0x00000000, 0xffffffff},
	{0xf003486C, 0x03090308, 0xffffffff},
	{0xf0034870, 0x00000000, 0xffffffff},
	{0xf0034874, 0x00000000, 0xffffffff},
	{0xf0034878, 0x00000004, 0xffffffff},
	{0xf003487C, 0x00000000, 0xffffffff},  
                                          
	// OR merge TP2 and TP14 to DATA2 
	// OR merge TP5 and TP1   to DATA1
	{0xF0034814, 0x00100E00, 0xffffffff},
	{0xF0034818, 0x00000410, 0xffffffff},  
                                          
	// TPK6 for CPV		   
	{0xf0034900, 0x03B40358, 0xffffffff},
	{0xf0034904, 0x00000000, 0xffffffff},
	{0xf0034908, 0x03B40358, 0xffffffff},
	{0xf003490C, 0x04420006, 0xffffffff},
	{0xf0034910, 0x20000000, 0xffffffff},
	{0xf0034914, 0x80000000, 0xffffffff},
	{0xf0034918, 0x00000007, 0xffffffff},
	{0xf003491C, 0x00000000, 0xffffffff},
                                          
	// TPK0 for LP		           
	{0xf0034840, 0x00600B9D, 0xffffffff},
	{0xf0034844, 0x00000000, 0xffffffff},
	{0xf0034848, 0x00600B9D, 0xffffffff},
	{0xf003484C, 0x04420006, 0xffffffff},
	{0xf0034850, 0x20000000, 0xffffffff},
	{0xf0034854, 0x80000000, 0xffffffff},
	{0xf0034858, 0x00000007, 0xffffffff},
	{0xf003485C, 0x00000000, 0xffffffff},
                                          
	// TPK7 for POLL						   
	{0xf0034920, 0x04550455, 0xffffffff},
	{0xf0034924, 0x00000000, 0xffffffff},
	{0xf0034928, 0x04550455, 0xffffffff},
	{0xf003492C, 0x00010001, 0xffffffff},
	{0xf0034930, 0x80000404, 0xffffffff},
	{0xf0034934, 0x80040002, 0xffffffff},
	{0xf0034938, 0x00000007, 0xffffffff},
	{0xf003493C, 0x10000000, 0xffffffff},
								   
	// TPK12 for POLR                 
	{0xf00349C0, 0x04550455, 0xffffffff},																													   
	{0xf00349C4, 0x00000000, 0xffffffff}, 																													  
	{0xf00349C8, 0x04550455, 0xffffffff},																													   
	{0xf00349CC, 0x00010001, 0xffffffff}, 																													  
	{0xf00349D0, 0x80000404, 0xffffffff}, 																													  
	{0xf00349D4, 0x80040002, 0xffffffff}, 																													  
	{0xf00349D8, 0x00000087, 0xffffffff}, 																													  
	{0xf00349DC, 0x1f000000, 0xffffffff},

  };

struct sTCONREGTABLE sRegCustTg_MLVDS_SAMSUNGMB4[] = {
  // SOE, TG0  
	  {0xf0034840, 0x00580832, 0xffffffff},
	  {0xf0034844, 0x00000000, 0xffffffff},
	  {0xf0034848, 0x00580832, 0xffffffff},  
	  {0xf003484c, 0x04430006, 0xffffffff},
	  {0xf0034850, 0x20000000, 0xffffffff},
	  {0xf0034854, 0x80000000, 0xffffffff},
	  {0xf0034858, 0x00000007, 0xffffffff},
	  {0xf003485c, 0x00000000, 0xffffffff},
                                       																														
	  // POL, TG1                       
	  {0xf0034860, 0x07D007D0, 0xffffffff},
	  {0xf0034864, 0x00000000, 0xffffffff}, 
	  {0xf0034868, 0x07D007D0, 0xffffffff},  
	  {0xf003486c, 0x043F0007, 0xffffffff},
	  {0xf0034870, 0x80000404, 0xffffffff},
	  {0xf0034874, 0x80020004, 0xffffffff},
	  {0xf0034878, 0x00000007, 0xffffffff},
	  {0xf003487c, 0x10000000, 0xffffffff},
  																											
	  // GSP, TG11                      
	  {0xf00349A0, 0x03F405DC, 0xffffffff},
	  {0xf00349A4, 0x00000000, 0xffffffff},
	  {0xf00349A8, 0x00000000, 0xffffffff},
	  {0xf00349Ac, 0x00080005, 0xffffffff},
	  {0xf00349B0, 0x00000000, 0xffffffff},
	  {0xf00349B4, 0x00000000, 0xffffffff},
	  {0xf00349B8, 0x00000004, 0xffffffff},
	  {0xf00349Bc, 0x00000000, 0xffffffff},
                                            
	  // H-CONV, TG5 -> CPV1            
	  {0xf00348e0, 0x024406AE, 0xffffffff},
	  {0xf00348e4, 0x00000000, 0xffffffff},
	  {0xf00348e8, 0x024406AE, 0xffffffff},
	  {0xf00348ec, 0x04420006, 0xffffffff},
	  {0xf00348f0, 0x80000303, 0xffffffff},
	  {0xf00348f4, 0x80020002, 0xffffffff},
	  {0xf00348f8, 0x00000007, 0xffffffff},
	  {0xf00348fc, 0x00000000, 0xffffffff},
                                            
	  // GSC, TG12 -> CPV2              
	  {0xf00349C0, 0x024406AE, 0xffffffff},
	  {0xf00349C4, 0x00000000, 0xffffffff},
	  {0xf00349C8, 0x024406AE, 0xffffffff},
	  {0xf00349Cc, 0x04420007, 0xffffffff},
	  {0xf00349D0, 0x80000303, 0xffffffff},
	  {0xf00349D4, 0x80010001, 0xffffffff},
	  {0xf00349D8, 0x00000007, 0xffffffff},
	  {0xf00349Dc, 0x0f000000, 0xffffffff},	  
                                            
	  // GSP_R, TG8 -> CPV3             
	  {0xf0034940, 0x024406AE, 0xffffffff},
	  {0xf0034944, 0x00000000, 0xffffffff},
	  {0xf0034948, 0x024406AE, 0xffffffff},
	  {0xf003494c, 0x04420008, 0xffffffff},
	  {0xf0034950, 0x80000303, 0xffffffff},
	  {0xf0034954, 0x80030003, 0xffffffff},
	  {0xf0034958, 0x00000007, 0xffffffff},
	  {0xf003495c, 0x00000000, 0xffffffff},
                                            
	  // FLK, TG9  -> CPV3              
	  {0xf0034960, 0x049C0726, 0xffffffff},
	  {0xf0034964, 0x00000000, 0xffffffff},
	  {0xf0034968, 0x049C0726, 0xffffffff},
	  {0xf003496c, 0x04420006, 0xffffffff},
	  {0xf0034970, 0x20000000, 0xffffffff},
	  {0xf0034974, 0x80000000, 0xffffffff},
	  {0xf0034978, 0x00000007, 0xffffffff},
	  {0xf003497c, 0x00000000, 0xffffffff},
                                            
	  // DPM, TG10                      
	  {0xf0034820, 0x04000400, 0x04000400}, //GPO :1
  
};
   
struct sTCONREGTABLE sRegCustTg_MLVDS_SAMSUNGMB7[] = {

	  {0xf0034800, 0x00000000, 0xffffffff},
	  // SOE, TG0                       
	  {0xf0034840, 0x006a0048, 0xffffffff},
	  {0xf0034844, 0x00000000, 0xffffffff},
	  {0xf0034848, 0x006a0048, 0xffffffff},  
	  {0xf003484c, 0x00120013, 0xffffffff},
	  {0xf0034850, 0x20000000, 0xffffffff},
	  {0xf0034854, 0x80000000, 0xffffffff},
	  {0xf0034858, 0x00000007, 0xffffffff},
	  {0xf003485c, 0x00000000, 0xffffffff},
	                                    
	  // POL, TG1  1 Line Inversion                   
	  //{0xf0034860, 0x00600060, 0xffffffff},
	  //{0xf0034864, 0x00000000, 0xffffffff},
	  //{0xf0034868, 0x00600060, 0xffffffff},  
	  //{0xf003486c, 0x0Cdf0007, 0xffffffff},
	  //{0xf0034870, 0x80000202, 0xffffffff},
	  //{0xf0034874, 0x80010002, 0xffffffff},
	  //{0xf0034878, 0x00000007, 0xffffffff},
	  //{0xf003487c, 0x10000000, 0xffffffff},
	  
 	  // POL, TG1  1 Frame Inversion                   
	  {0xf0034860, 0x00010002, 0xffffffff},
	  {0xf0034864, 0x00000000, 0xffffffff},
	  {0xf0034868, 0x00010002, 0xffffffff},  
	  {0xf003486c, 0x00130013, 0xffffffff},
	  {0xf0034870, 0x00000000, 0xffffffff},
	  {0xf0034874, 0x00130013, 0xffffffff},
	  {0xf0034878, 0x00000007, 0xffffffff},
	  {0xf003487c, 0x80008001, 0xffffffff},   
	  
	  // GSP, TG11                      
	  {0xf00349A0, 0x01a101a1, 0xffffffff},
	  {0xf00349A4, 0x00000000, 0xffffffff},
	  {0xf00349A8, 0x00000000, 0xffffffff},
	  {0xf00349Ac, 0x00140010, 0xffffffff},
	  {0xf00349B0, 0x00000000, 0xffffffff},
	  {0xf00349B4, 0x00000000, 0xffffffff},
	  {0xf00349B8, 0x00000004, 0xffffffff},
	  {0xf00349Bc, 0x00000000, 0xffffffff},
                                            
	  // H-CONV, TG5 -> CPV1            
	  {0xf00348e0, 0x01900200, 0xffffffff},
	  {0xf00348e4, 0x00000000, 0xffffffff},
	  {0xf00348e8, 0x01900200, 0xffffffff},
	  {0xf00348ec, 0x0Ce00011, 0xffffffff},
	  {0xf00348f0, 0x80000303, 0xffffffff},
	  {0xf00348f4, 0x80020002, 0xffffffff},
	  {0xf00348f8, 0x00000007, 0xffffffff},
	  {0xf00348fc, 0x00000000, 0xffffffff},
                                            
	  // GSC, TG12 -> CPV2              
	  {0xf00349C0, 0x01900200, 0xffffffff},
	  {0xf00349C4, 0x00000000, 0xffffffff},
	  {0xf00349C8, 0x01900200, 0xffffffff},
	  {0xf00349Cc, 0x0Ce10012, 0xffffffff},
	  {0xf00349D0, 0x80000303, 0xffffffff},
	  {0xf00349D4, 0x80010001, 0xffffffff},
	  {0xf00349D8, 0x00000007, 0xffffffff},
	  {0xf00349Dc, 0x0f000000, 0xffffffff},	  
	                                    
	  // GSP_R, TG8 -> CPV3             
	  {0xf0034940, 0x01900200, 0xffffffff},
	  {0xf0034944, 0x00000000, 0xffffffff},
	  {0xf0034948, 0x01900200, 0xffffffff},
	  {0xf003494c, 0x0Cdf0013, 0xffffffff},
	  {0xf0034950, 0x80000303, 0xffffffff},
	  {0xf0034954, 0x80030003, 0xffffffff},
	  {0xf0034958, 0x00000007, 0xffffffff},
	  {0xf003495c, 0x00000000, 0xffffffff},
                                            
	  // FLK, TG9  -> CPV3              
	  {0xf0034960, 0x049C0726, 0xffffffff},
	  {0xf0034964, 0x00000000, 0xffffffff},
	  {0xf0034968, 0x049C0726, 0xffffffff},
	  {0xf003496c, 0x04420006, 0xffffffff},
	  {0xf0034970, 0x20000000, 0xffffffff},
	  {0xf0034974, 0x80000000, 0xffffffff},
	  {0xf0034978, 0x00000007, 0xffffffff},
	  {0xf003497c, 0x00000000, 0xffffffff},
	                                    
	  // DPM, TG10                      
	  {0xf0034820, 0x04000400, 0x04000400}, //GPO :1
	 
  };


struct sTCONREGTABLE sRegCustTg_MLVDS_LGDV5GIPFHD60[] = {

// lgdv5 gip

  // GR0
  {0xf0034800, 0x00000000, 0xffffffff},  
                                    
  // GR1                            
  //0xf0034804, 0x00000000, 0xffffffff,
                                    
  // GR7                            
  {0xf003481c, 0x00000000, 0xffffffff},																													
                               													
  // SOE, TG0                       
  {0xf0034820, 0x00C807d0, 0xffffffff},
  {0xf0034824, 0x00000000, 0xffffffff},
  {0xf0034828, 0x00C807d0, 0xffffffff},  
  {0xf003482c, 0x04430007, 0xffffffff},
  {0xf0034830, 0x20000000, 0xffffffff},
  {0xf0034834, 0x80000000, 0xffffffff},
  {0xf0034838, 0x00000007, 0xffffffff},
  {0xf003483c, 0x00000000, 0xffffffff},
                                     
  // POL, OK                    																														
  {0xf0034840, 0x07D007D0, 0xffffffff},																														
  {0xf0034844, 0x00000000, 0xffffffff},
  {0xf0034848, 0x07D007D0, 0xffffffff},																														
  {0xf003484c, 0x043F0007, 0xffffffff},																														
  {0xf0034850, 0x80000404, 0xffffffff},																														
  {0xf0034854, 0x80020004, 0xffffffff},
  {0xf0034858, 0x00000007, 0xffffffff},																														
  {0xf003485c, 0x10000000, 0xffffffff},
                                      
  // GSP, OK                   																														
  {0xf0034860, 0x01F405DC, 0xffffffff},
  {0xf0034864, 0x00000000, 0xffffffff},
  {0xf0034868, 0x00000000, 0xffffffff},
  {0xf003486c, 0x00030001, 0xffffffff},
  {0xf0034870, 0x00000000, 0xffffffff},
  {0xf0034874, 0x00000000, 0xffffffff},
  {0xf0034878, 0x00000004, 0xffffffff},
  {0xf003487c, 0x00000000, 0xffffffff},
                                    
                                    
  // GCLK 1, OK																					
  {0xf0034880, 0x06AE083E, 0xffffffff},
  {0xf0034884, 0x00000000, 0xffffffff},
  {0xf0034888, 0x06AE083E, 0xffffffff},
  {0xf003488c, 0x04420005, 0xffffffff},
  {0xf0034890, 0x80000606, 0xffffffff},
  {0xf0034894, 0x80050002, 0xffffffff},
  {0xf0034898, 0x00000007, 0xffffffff},
  {0xf003489c, 0x00000000, 0xffffffff},
                                    
  // GCLK 2, OK          																														
  {0xf00348A0, 0x06AE083E, 0xffffffff},
  {0xf00348A4, 0x00000000, 0xffffffff},
  {0xf00348A8, 0x06AE083E, 0xffffffff},
  {0xf00348AC, 0x04430006, 0xffffffff},
  {0xf00348B0, 0x80000606, 0xffffffff},
  {0xf00348B4, 0x80040001, 0xffffffff},
  {0xf00348B8, 0x00000007, 0xffffffff},
  {0xf00348BC, 0x00000000, 0xffffffff},
                                    
  // GCLK 3, OK       																														
  {0xf00348C0, 0x06AE083E, 0xffffffff},
  {0xf00348C4, 0x00000000, 0xffffffff},
  {0xf00348C8, 0x06AE083E, 0xffffffff},
  {0xf00348CC, 0x04440007, 0xffffffff},
  {0xf00348D0, 0x80000606, 0xffffffff},
  {0xf00348D4, 0x80030006, 0xffffffff},
  {0xf00348D8, 0x00000007, 0xffffffff},
  {0xf00348DC, 0x00000000, 0xffffffff},
                                    
  // GCLK 4, OK     																														
  {0xf00348E0, 0x06AE083E, 0xffffffff},
  {0xf00348E4, 0x00000000, 0xffffffff},
  {0xf00348E8, 0x06AE083E, 0xffffffff},
  {0xf00348EC, 0x043F0002, 0xffffffff},
  {0xf00348F0, 0x80000606, 0xffffffff},
  {0xf00348F4, 0x80020005, 0xffffffff},
  {0xf00348F8, 0x00000007, 0xffffffff},
  {0xf00348FC, 0x00000000, 0xffffffff},
                                    
  // GCLK 5	, ok													
  {0xf0034900, 0x06AE083E, 0xffffffff},
  {0xf0034904, 0x00000000, 0xffffffff},
  {0xf0034908, 0x06AE083E, 0xffffffff},
  {0xf003490C, 0x04400003, 0xffffffff},
  {0xf0034910, 0x80000606, 0xffffffff},
  {0xf0034914, 0x80010004, 0xffffffff},
  {0xf0034918, 0x00000007, 0xffffffff},
  {0xf003491C, 0x00000000, 0xffffffff},
                                    
  // GCLK 6, ok                     
  {0xf0034920, 0x06AE083E, 0xffffffff},
  {0xf0034924, 0x00000000, 0xffffffff},
  {0xf0034928, 0x06AE083E, 0xffffffff},
  {0xf003492C, 0x04410004, 0xffffffff},
  {0xf0034930, 0x80000606, 0xffffffff},
  {0xf0034934, 0x80060003, 0xffffffff},
  {0xf0034938, 0x00000007, 0xffffffff},
  {0xf003493C, 0x00000000, 0xffffffff},
  									
                                    
  //FLK, always high for GIP, OK																																																		
  {0xf0034958, 0x00000037, 0xffffffff},
                                    
  //ODD	, ok																											
  {0xf0034980, 0x002d0015, 0xffffffff},
  {0xf0034984, 0x002d0015, 0xffffffff},
  {0xf0034988, 0x002d0015, 0xffffffff},
  {0xf003498C, 0x00010000, 0xffffffff},
  {0xf0034990, 0x00000000, 0xffffffff},
  {0xf0034994, 0x04500450, 0xffffffff},
  {0xf0034998, 0x00000103, 0xffffffff},
  {0xf003499C, 0x800ff001, 0xffffffff},
                                    
  //EVEN, OK              																														
  {0xf00349A0, 0x0015002d, 0xffffffff},
  {0xf00349A4, 0x0015002d, 0xffffffff},
  {0xf00349A8, 0x0015002d, 0xffffffff},
  {0xf00349AC, 0x00010000, 0xffffffff},
  {0xf00349B0, 0x00000000, 0xffffffff},
  {0xf00349B4, 0x04500450, 0xffffffff},
  {0xf00349B8, 0x00000003, 0xffffffff},
  {0xf00349BC, 0x800ff001, 0xffffffff},
                                    
  // DPM, OK            																														
  {0xf0034978, 0x00000030, 0xffffffff},
};

struct sTCONREGTABLE sRegTg_EPI_LGDV11FHD60[] = {
    //GSP    TCO[0]  0xf0034840
    {0xf0034840, 0x012c0001, 0xffffffff},
    {0xf0034844, 0x00000000, 0xffffffff},
    {0xf0034848, 0x012c0001, 0xffffffff},
    {0xf003484c, 0x00070006, 0xffffffff},
    {0xf0034850, 0x00000000, 0xffffffff},
    {0xf0034854, 0x00000000, 0xffffffff},
    {0xf0034858, 0x00000004, 0xffffffff},
    {0xf003485c, 0x00000000, 0xffffffff},
    
    //POL    TCO[1]  0xf0034860
    {0xf0034860, 0x00000000, 0xffffffff},
    {0xf0034864, 0x00000000, 0xffffffff},
    {0xf0034868, 0x00000000, 0xffffffff},
    {0xf003486c, 0x00000000, 0xffffffff},
    {0xf0034870, 0x00000000, 0xffffffff},
    {0xf0034874, 0x00000000, 0xffffffff},
    {0xf0034878, 0x00000080, 0xffffffff},
    {0xf003487c, 0x20000000, 0xffffffff},
    
    //GST    TCO[5]  0xf00348e0
    {0xf00348E0, 0x01040001, 0xffffffff},
    {0xf00348E4, 0x00000000, 0xffffffff},
    {0xf00348E8, 0x01040001, 0xffffffff},
    {0xf00348Ec, 0x00020001, 0xffffffff},
    {0xf00348F0, 0x00000000, 0xffffffff},
    {0xf00348F4, 0x00000000, 0xffffffff},
    {0xf00348F8, 0x00000004, 0xffffffff},
    {0xf00348Fc, 0x00000000, 0xffffffff},

    //MCLK    TCO[4]  0xf00348c0
    {0xf00348C0, 0x01280125, 0xffffffff},
    {0xf00348C4, 0x00000000, 0xffffffff},
    {0xf00348C8, 0x01280125, 0xffffffff},
    {0xf00348Cc, 0x04410005, 0xffffffff},
    {0xf00348D0, 0x20000000, 0xffffffff},
    {0xf00348D4, 0x80000000, 0xffffffff},
    {0xf00348D8, 0x00000007, 0xffffffff},
    {0xf00348Dc, 0x00000000, 0xffffffff},

    //GCLK    TCO[3]  0xf00348a0
    {0xf00348A0, 0x0080006A, 0xffffffff},
    {0xf00348A4, 0x00000000, 0xffffffff},
    {0xf00348A8, 0x0080006A, 0xffffffff},
    {0xf00348Ac, 0x043f0003, 0xffffffff},
    {0xf00348B0, 0x20000000, 0xffffffff},
    {0xf00348B4, 0x80000000, 0xffffffff},
    {0xf00348B8, 0x00000007, 0xffffffff},
    {0xf00348Bc, 0x00000000, 0xffffffff},

    //GCLK_END    TCO[8]  0xf0034940
    {0xf0034940, 0x0080006A, 0xffffffff},
    {0xf0034944, 0x00000000, 0xffffffff},
    {0xf0034948, 0x0080006A, 0xffffffff},
    {0xf003494c, 0x0443043f, 0xffffffff},
    {0xf0034950, 0x00000000, 0xffffffff},
    {0xf0034954, 0x00000000, 0xffffffff},
    {0xf0034958, 0x00000004, 0xffffffff},
    {0xf003495c, 0x00000000, 0xffffffff}, 

    //EO   TCO[6]  0xf0034900
    {0xf0034900, 0x00CA0015, 0xffffffff},
    {0xf0034904, 0x00000000, 0xffffffff},
    {0xf0034908, 0x00CA0015, 0xffffffff},
    {0xf003490c, 0x00010000, 0xffffffff},
    {0xf0034910, 0x00000000, 0xffffffff},
    {0xf0034914, 0x04500450, 0xffffffff},
    {0xf0034918, 0x00000103, 0xffffffff},
    {0xf003491c, 0x80008001, 0xffffffff},

    //Xor with EO    TCO[9]  0xf0034960
    {0xf0034960, 0x001500CA, 0xffffffff},
    {0xf0034964, 0x00000000, 0xffffffff},
    {0xf0034968, 0x001500CA, 0xffffffff},
    {0xf003496c, 0x00010000, 0xffffffff},
    {0xf0034970, 0x00000000, 0xffffffff},
    {0xf0034974, 0x04500450, 0xffffffff},
    {0xf0034978, 0x00000103, 0xffffffff},
    {0xf003497c, 0x80008001, 0xffffffff}, 

    //VCOM_DYN  TCO[7]  0xf0034920
    {0xf0034920, 0x00010001, 0xffffffff},
    {0xf0034924, 0x00000000, 0xffffffff},
    {0xf0034928, 0x00010001, 0xffffffff},
    {0xf003492c, 0x02320001, 0xffffffff},
    {0xf0034930, 0x00000000, 0xffffffff},
    {0xf0034934, 0x00000000, 0xffffffff},
    {0xf0034938, 0x00000004, 0xffffffff},
    {0xf003493c, 0x00000000, 0xffffffff},
/*    //GCLK3    TCO[2]  0xf0034880
    {0xf0034880, 0x00e20112, 0xffffffff},
    {0xf0034884, 0x00000000, 0xffffffff},
    {0xf0034888, 0x00e20112, 0xffffffff},
    {0xf003488c, 0x043f0002, 0xffffffff},
    {0xf0034890, 0x80000606, 0xffffffff},
    {0xf0034894, 0x80020005, 0xffffffff},
    {0xf0034898, 0x00000007, 0xffffffff},
    {0xf003489c, 0x00000000, 0xffffffff},
    
    //GCLK4    TCO[3]  0xf00348a0
    {0xf00348A0, 0x002d0015, 0xffffffff},
    {0xf00348A4, 0x00000000, 0xffffffff},
    {0xf00348A8, 0x002d0015, 0xffffffff},
    {0xf00348Ac, 0x00010000, 0xffffffff},
    {0xf00348B0, 0x00000000, 0xffffffff},
    {0xf00348B4, 0x04500450, 0xffffffff},
    {0xf00348B8, 0x00000083, 0xffffffff},
    {0xf00348Bc, 0x80200001, 0xffffffff},
    
    //GCLK5    TCO[4]  0xf00348c0
    {0xf00348C0, 0x0015002d, 0xffffffff},
    {0xf00348C4, 0x00000000, 0xffffffff},
    {0xf00348C8, 0x0015002d, 0xffffffff},
    {0xf00348Cc, 0x00010000, 0xffffffff},
    {0xf00348D0, 0x00000000, 0xffffffff},
    {0xf00348D4, 0x04500450, 0xffffffff},
    {0xf00348D8, 0x00000003, 0xffffffff},
    {0xf00348Dc, 0x80200001, 0xffffffff},
    
    //GCLK6    TCO[5]  0xf00348e0
    {0xf00348E0, 0x00e20112, 0xffffffff},
    {0xf00348E4, 0x00000000, 0xffffffff},
    {0xf00348E8, 0x00e20112, 0xffffffff},
    {0xf00348Ec, 0x04430005, 0xffffffff},
    {0xf00348F0, 0x80000606, 0xffffffff},
    {0xf00348F4, 0x80040001, 0xffffffff},
    {0xf00348F8, 0x00000007, 0xffffffff},
    {0xf00348Fc, 0x00000000, 0xffffffff},
    
    //VGH_ODD   TCO[6]  0xf0034900
    {0xf0034900, 0x00e20112, 0xffffffff},
    {0xf0034904, 0x00000000, 0xffffffff},
    {0xf0034908, 0x00e20112, 0xffffffff},
    {0xf003490c, 0x04400003, 0xffffffff},
    {0xf0034910, 0x80000606, 0xffffffff},
    {0xf0034914, 0x80010004, 0xffffffff},
    {0xf0034918, 0x00000007, 0xffffffff},
    {0xf003491c, 0x00000000, 0xffffffff},
    
    //VGH_EVEN  TCO[7]  0xf0034920
    {0xf0034920, 0x00e20112, 0xffffffff},
    {0xf0034924, 0x00000000, 0xffffffff},
    {0xf0034928, 0x00e20112, 0xffffffff},
    {0xf003492c, 0x04410003, 0xffffffff},
    {0xf0034930, 0x80000606, 0xffffffff},
    {0xf0034934, 0x80060003, 0xffffffff},
    {0xf0034938, 0x00000007, 0xffffffff},
    {0xf003493c, 0x00000000, 0xffffffff},
    
    //VSTIN    TCO[8]  0xf0034940
    {0xf0034940, 0x00e20112, 0xffffffff},
    {0xf0034944, 0x00000000, 0xffffffff},
    {0xf0034948, 0x00e20112, 0xffffffff},
    {0xf003494c, 0x04420005, 0xffffffff},
    {0xf0034950, 0x80000606, 0xffffffff},
    {0xf0034954, 0x80050002, 0xffffffff},
    {0xf0034958, 0x00000007, 0xffffffff},
    {0xf003495c, 0x00000000, 0xffffffff}, 
    
    //OPT_N    TCO[9]  0xf0034960
    
    //GSP     TCO[10] 0xf0034980
    {0xf0034980, 0x00000000, 0xffffffff},  
    {0xf0034984, 0x00000000, 0xffffffff},  
    {0xf0034988, 0x00000000, 0xffffffff},  
    {0xf003498c, 0x00000000, 0xffffffff},  
    {0xf0034990, 0x00000000, 0xffffffff},  
    {0xf0034994, 0x00000000, 0xffffffff},  
    {0xf0034998, 0x00000080, 0xffffffff},  
    {0xf003499c, 0x00000000, 0xffffffff},  
    
    //POL     TCO[11] 0xf00349a0
    {0xf00349A0, 0x01040001, 0xffffffff}, 
    {0xf00349A4, 0x00000000, 0xffffffff}, 
    {0xf00349A8, 0x01040001, 0xffffffff}, 
    {0xf00349Ac, 0x00030001, 0xffffffff}, 
    {0xf00349B0, 0x00000000, 0xffffffff}, 
    {0xf00349B4, 0x00000000, 0xffffffff}, 
    {0xf00349B8, 0x00000004, 0xffffffff}, 
    {0xf00349Bc, 0x00000000, 0xffffffff},  
    
    //DPM     TCO[12] 0xf00349c0
    {0xf00349C0, 0x00e20112, 0xffffffff},
    {0xf00349C4, 0x00000000, 0xffffffff},
    {0xf00349C8, 0x00e20112, 0xffffffff},
    {0xf00349Cc, 0x04430004, 0xffffffff},
    {0xf00349D0, 0x80000606, 0xffffffff},
    {0xf00349D4, 0x80030006, 0xffffffff},
    {0xf00349D8, 0x00000007, 0xffffffff},
    {0xf00349Dc, 0x0F000000, 0xffffffff},
    
    //POL    TCO[13]  0xf00349E0
    {0xf00349E0, 0x00000000, 0xffffffff},
    {0xf00349E4, 0x00000000, 0xffffffff},
    {0xf00349E8, 0x00000000, 0xffffffff},
    {0xf00349EC, 0x00000000, 0xffffffff},
    {0xf00349F0, 0x00000000, 0xffffffff},
    {0xf00349F4, 0x00000000, 0xffffffff},
    {0xf00349F8, 0x00000080, 0xffffffff},
    {0xf00349FC, 0x2F000000, 0xffffffff},
*/
    {0xf0034820, 0x00000000, 0xffffffff}, //GPO :1

    {0xf0034800, 0x8100ffff, 0xffffffff},
    {0xf0034804, 0x00000000, 0xffffffff},
    {0xf0034814, 0x09008000, 0xffffffff}, //Bmux select
    {0xf0034818, 0x00003040, 0xffffffff}, //combination
    {0xf003481c, 0x00000000, 0xffffffff},

    };

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static UINT32 _u4TconCustGammaWrite(UINT8 u1offset, UINT8 *pu1Buf, UINT8 u1Count)
{
	Printf("0x%8x, 0x%8x, 0xffffffff,\n", u1offset, (pu1Buf[0]<<8) | pu1Buf[1]);
	return (SIF_Write(GAMMAIC_CLK_RATE, GAMMAIC_ID, u1offset, (UINT8 *)pu1Buf, GAMMAIC_DATA_CNT));
}

#if 0 // for warning free
static UINT32 _u4TconCustGammaRead(UINT8 u1offset, UINT8 *pu1Buf, UINT8 u1Count)
{
	return (SIF_Read(GAMMAIC_CLK_RATE, GAMMAIC_ID, u1offset, (UINT8 *)pu1Buf, GAMMAIC_DATA_CNT));
}
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

UINT8 u1TconCustDPMCtrlEN1(UINT8 val)
{
//  vIO32WriteFldAlign(GR8, 1, GR8_TCGPOEN_A);
	
  if (val)
  {
//      vIO32WriteFldAlign(GR8, 1, GR8_TCGPO_A);
  }
  else
  {
//      vIO32WriteFldAlign(GR8, 0, GR8_TCGPO_A);
  }
  
  return(1);
}

UINT8 u1TconCustDPMCtrlEN2(UINT8 val)
{
    if (val)
    {
        // TODO
    }
    else
    {
        // TODO
    }

    return(1);
}

// Gamma
UINT8 u1TconCustGammaInit(void)
{
	UINT8 u1Err = 0;
	UINT8 i = 0;
	UINT8 u1count;
	UINT8 u1tmp[2];
	UINT16 u2GammaVoltage = 0;

	Printf("Tcon Gamma init\n");

	u1count = (sizeof(sRegCustGamma) / sizeof(struct sTCONREGTABLE));

	for ( i = 0 ; i < u1count ; i++)
    {
                //#if  HISENSE_IPS_2010_PANEL 
      		//u1tmp[1] = (UINT8)((sRegCustGamma[i].u4Val) >> 8);
		//u1tmp[0] = (UINT8)((sRegCustGamma[i].u4Val) & 0xff);
		u2GammaVoltage = sRegCustGamma[i].u4Val;
		u1tmp[1] = (UINT8)(u2GammaVoltage >> 8);
		u1tmp[0] = (UINT8)(u2GammaVoltage & 0xff);
		if (_u4TconCustGammaWrite(sRegCustGamma[i].u4Addr, u1tmp, 2) == 0)
		{
		
			u1Err = 1;
			Printf("!!!Tcon Gamma write fail %d, %d, %d \n", \
				              sRegCustGamma[i].u4Addr, \
				              u1tmp[0], \
				              u1tmp[1]);			
		}
		else
		{
    		Printf("Tcon Gamma write ok %d , %d , %d \n", \
                              sRegCustGamma[i].u4Addr, \
                              u1tmp[0], \
                              u1tmp[1]);
		}
    }

    // check if init ok
	if (u1Err == 0)
	{
		Printf("Program Tcon Gamma dacs %d counts, %d / %d\n", i, sizeof(sRegCustGamma), sizeof(struct sTCONREGTABLE));
	}
	else
	{
	    Printf("Program Tcon Gamma dacs fail !!!\n");
	}
	
	return(1);
}

// program gamma value into non-volatile memory
UINT8  u1TconCustGammaFlashing(void)
{
	UINT8 u1Err = 0;
	UINT8 i = 0;
	UINT8 u1count;
	UINT8 u1tmp[2];
	UINT8 u1Ret = 0;
	UINT16 u2GammaVoltage = 0;

#ifdef TCON_DBG_MSG_EN
   	printf("Tcon Gamma init\n");
#endif
	u1count = (sizeof(sRegCustGamma) / sizeof(struct sTCONREGTABLE));

	for ( i = 0 ; i < u1count ; i++)
    {
    
		//when write non-voltial memory  D15 D14 must be 0 1
		u2GammaVoltage = (((sRegCustGamma[i].u4Val)&0x03FF)|0x4000);
		u1tmp[0] = (UINT8)((u2GammaVoltage >> 8) );  // program gamma into non-voilate
		u1tmp[1] = (UINT8)(u2GammaVoltage & 0xff);

		if (_u4TconCustGammaWrite(sRegCustGamma[i].u4Addr, u1tmp, 2) == 0)
		{
		
			u1Err = 1;
#ifdef TCON_DBG_MSG_EN
 		  	printf("!!!Tcon Gamma write fail 0x%x, 0x%x, 0x%x\n",\
				              sRegCustGamma[i].u4Addr,\
				              u1tmp[0],\
				              u1tmp[1]);
#endif
		}
		else
		{
#ifdef TCON_DBG_MSG_EN
		   	printf("Tcon Gamma write ok 0x%x, 0x%x, 0x%x\n", \
                              sRegCustGamma[i].u4Addr, \
                              u1tmp[0], \
                              u1tmp[1]);
#endif
		}
    }

    // check if init ok
	if (u1Err == 0)
	{
#ifdef TCON_DBG_MSG_EN
    	printf("Program Tcon Gamma dacs %d counts, %d / %d\n", i, sizeof(sRegCustGamma), sizeof(struct sTCONREGTABLE));
#endif
	}
	else
	{
#ifdef TCON_DBG_MSG_EN
   	   printf("Program Tcon Gamma dacs fail !!!\n");
#endif
	}

    return (u1Ret);
}

UINT8 u1TconCustTgInit(void)
{
	UINT8 i = 0;
	UINT8 u1count;
	UINT32 u4tmp, u4tmp1;

	switch (GetCurrentPanelIndex())
	 {
	   case PANEL_MLVDS_LGDV4FHD60: 

		 LOG(1,"cust tg init\n");

			 u1count = (sizeof(sRegCustTg_MLVDS_LGDV4FHD60) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
			   u4tmp = sRegCustTg_MLVDS_LGDV4FHD60[i].u4Addr;
			   u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegCustTg_MLVDS_LGDV4FHD60[i].u4Msk)) | (sRegCustTg_MLVDS_LGDV4FHD60[i].u4Val);

		   vIO32Write4B(u4tmp,u4tmp1);
         }

			 LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegCustTg_MLVDS_LGDV4FHD60), sizeof(struct sTCONREGTABLE));

         break;

		   case PANEL_MLVDS_LGDV4FHD120: 

		 LOG(1,"cust tg init\n");

			 u1count = (sizeof(sRegCustTg_MLVDS_LGDV4FHD120) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
			   u4tmp = sRegCustTg_MLVDS_LGDV4FHD120[i].u4Addr;
			   u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegCustTg_MLVDS_LGDV4FHD120[i].u4Msk)) | (sRegCustTg_MLVDS_LGDV4FHD120[i].u4Val);

		   vIO32Write4B(u4tmp,u4tmp1);
         }

			 LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegCustTg_MLVDS_LGDV4FHD120), sizeof(struct sTCONREGTABLE));

         break;
		 
	   case PANEL_MLVDS_CMO2011FHD60: 
	   	
         LOG(1,"cust tg init\n");

	     u1count = (sizeof(sRegTg_PANEL_MLVDS_CMO2011FHD60) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
           u4tmp = sRegTg_PANEL_MLVDS_CMO2011FHD60[i].u4Addr;
	       u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegTg_PANEL_MLVDS_CMO2011FHD60[i].u4Msk)) | (sRegTg_PANEL_MLVDS_CMO2011FHD60[i].u4Val);

		   vIO32Write4B(u4tmp,u4tmp1);
         }

  	     LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegTg_PANEL_MLVDS_CMO2011FHD60), sizeof(struct sTCONREGTABLE));

         break;
		 
	       case PANEL_MLVDS_CMO2011FHD120: 
	   	
         LOG(1,"cust tg init\n");

	         u1count = (sizeof(sRegTg_PANEL_MLVDS_CMO2011FHD120) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
              u4tmp = sRegTg_PANEL_MLVDS_CMO2011FHD120[i].u4Addr;
	          u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegTg_PANEL_MLVDS_CMO2011FHD120[i].u4Msk)) | (sRegTg_PANEL_MLVDS_CMO2011FHD120[i].u4Val);

		   vIO32Write4B(u4tmp,u4tmp1);
         }

  	         LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegTg_PANEL_MLVDS_CMO2011FHD120), sizeof(struct sTCONREGTABLE));

         break;

		   case PANEL_MLVDS_PANASONIC32WXGA8BIT: 

		     LOG(1,"cust tg init\n");

	         u1count = (sizeof(sRegCustTg_PANASONIC32WXGA8BIT) / sizeof(struct sTCONREGTABLE));

	         for ( i = 0 ; i < u1count ; i++)
             {
              u4tmp = sRegCustTg_PANASONIC32WXGA8BIT[i].u4Addr;
              u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegCustTg_PANASONIC32WXGA8BIT[i].u4Msk)) | (sRegCustTg_PANASONIC32WXGA8BIT[i].u4Val);

	          vIO32Write4B(u4tmp,u4tmp1);
             }

  	         LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegCustTg_PANASONIC32WXGA8BIT), sizeof(struct sTCONREGTABLE));

             break;

         case PANEL_EPI_LGDV11GIPFHD60: 
         
           LOG(1,"cust tg init\n");
         
           u1count = (sizeof(sRegTg_EPI_LGDV11FHD60) / sizeof(struct sTCONREGTABLE));
         
           for ( i = 0 ; i < u1count ; i++)
           {
             u4tmp = sRegTg_EPI_LGDV11FHD60[i].u4Addr;
             u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegTg_EPI_LGDV11FHD60[i].u4Msk)) | (sRegTg_EPI_LGDV11FHD60[i].u4Val);
         
             vIO32Write4B(u4tmp,u4tmp1);
           }
         
           LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegTg_EPI_LGDV11FHD60), sizeof(struct sTCONREGTABLE));
         
           break;
		 
         case PANEL_MLVDS_SS_MB4: 
         
           LOG(1,"cust tg init\n");
         
           u1count = (sizeof(sRegCustTg_MLVDS_SAMSUNGMB4) / sizeof(struct sTCONREGTABLE));
         
           for ( i = 0 ; i < u1count ; i++)
           {
             u4tmp = sRegCustTg_MLVDS_SAMSUNGMB4[i].u4Addr;
             u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegCustTg_MLVDS_SAMSUNGMB4[i].u4Msk)) | (sRegCustTg_MLVDS_SAMSUNGMB4[i].u4Val);
         
             vIO32Write4B(u4tmp,u4tmp1);
           }
         
           LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegCustTg_MLVDS_SAMSUNGMB4), sizeof(struct sTCONREGTABLE));
         
           break;           

         case PANEL_MLVDS_SS_MB7: 
         
           LOG(1,"cust tg init\n");
         
           u1count = (sizeof(sRegCustTg_MLVDS_SAMSUNGMB7) / sizeof(struct sTCONREGTABLE));
         
           for ( i = 0 ; i < u1count ; i++)
           {
             u4tmp = sRegCustTg_MLVDS_SAMSUNGMB7[i].u4Addr;
             u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegCustTg_MLVDS_SAMSUNGMB7[i].u4Msk)) | (sRegCustTg_MLVDS_SAMSUNGMB7[i].u4Val);
         
             vIO32Write4B(u4tmp,u4tmp1);
           }
         
           LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegCustTg_MLVDS_SAMSUNGMB7), sizeof(struct sTCONREGTABLE));
         
           break;    
           
	   default:
		 break;
    }	

  return(1);
}

UINT8 u1TconCustMlvdsInit(void)
{
	UINT8 i = 0;
	UINT8 u1count;
	UINT32 u4tmp, u4tmp1;
	
	switch (GetCurrentPanelIndex())
		 {
		   case PANEL_MLVDS_LGDV4FHD60 :// | PANEL_MLVDS_LGDV5GIPFHD60 | PANEL_MLVDS_LGDV6GIPFHD60:
	
			 LOG(1,"cust tg init\n");
	
			 u1count = (sizeof(sRegMini_PANEL_MLVDS_LGDV4FHD60) / sizeof(struct sTCONREGTABLE));
	
			 for ( i = 0 ; i < u1count ; i++)
			 {
			  u4tmp = sRegMini_PANEL_MLVDS_LGDV4FHD60[i].u4Addr;
			  u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegMini_PANEL_MLVDS_LGDV4FHD60[i].u4Msk)) | (sRegMini_PANEL_MLVDS_LGDV4FHD60[i].u4Val);
	
			  vIO32Write4B(u4tmp,u4tmp1);
			 }
	
		   LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegMini_PANEL_MLVDS_LGDV4FHD60), sizeof(struct sTCONREGTABLE));
	
		   break;
	
			case PANEL_MLVDS_LGDV4FHD120 :// | PANEL_MLVDS_LGDV5GIPFHD60 | PANEL_MLVDS_LGDV6GIPFHD60:
		 
			  LOG(1,"cust tg init\n");
		 
			  u1count = (sizeof(sRegMini_PANEL_MLVDS_LGDV4FHD120) / sizeof(struct sTCONREGTABLE));
		 
			  for ( i = 0 ; i < u1count ; i++)
			  {
			   u4tmp = sRegMini_PANEL_MLVDS_LGDV4FHD120[i].u4Addr;
			   u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegMini_PANEL_MLVDS_LGDV4FHD120[i].u4Msk)) | (sRegMini_PANEL_MLVDS_LGDV4FHD120[i].u4Val);
		 
			   vIO32Write4B(u4tmp,u4tmp1);
			  }
		 
			LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegMini_PANEL_MLVDS_LGDV4FHD120), sizeof(struct sTCONREGTABLE));
		 
			break;
	
		 case PANEL_MLVDS_CMO2011FHD60:
	
			 LOG(1,"cust tg init\n");
	
			 u1count = (sizeof(sRegMini_PANEL_MLVDS_CMO2011FHD60) / sizeof(struct sTCONREGTABLE));
	
			 for ( i = 0 ; i < u1count ; i++)
			 {
			  u4tmp = sRegMini_PANEL_MLVDS_CMO2011FHD60[i].u4Addr;
			  u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegMini_PANEL_MLVDS_CMO2011FHD60[i].u4Msk)) | (sRegMini_PANEL_MLVDS_CMO2011FHD60[i].u4Val);
	
			  vIO32Write4B(u4tmp,u4tmp1);
			 }
	
		   LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegMini_PANEL_MLVDS_CMO2011FHD60), sizeof(struct sTCONREGTABLE));
	
		   break;
		   
		 case PANEL_MLVDS_CMO2011FHD120:
	
			 LOG(1,"cust tg init\n");
	
			 u1count = (sizeof(sRegMini_PANEL_MLVDS_CMO2011FHD120) / sizeof(struct sTCONREGTABLE));
	
			 for ( i = 0 ; i < u1count ; i++)
			 {
			  u4tmp = sRegMini_PANEL_MLVDS_CMO2011FHD120[i].u4Addr;
			  u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegMini_PANEL_MLVDS_CMO2011FHD120[i].u4Msk)) | (sRegMini_PANEL_MLVDS_CMO2011FHD120[i].u4Val);
	
			  vIO32Write4B(u4tmp,u4tmp1);
			 }
	
		   LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegMini_PANEL_MLVDS_CMO2011FHD120), sizeof(struct sTCONREGTABLE));
	
		   break;
		   
		 case PANEL_MLVDS_PANASONIC32WXGA8BIT:

	         LOG(1,"cust tg init\n");

	         u1count = (sizeof(sRegMini_PANEL_MLVDS_PANASONIC32WXGA8BIT) / sizeof(struct sTCONREGTABLE));

	         for ( i = 0 ; i < u1count ; i++)
             {
              u4tmp = sRegMini_PANEL_MLVDS_PANASONIC32WXGA8BIT[i].u4Addr;
	          u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegMini_PANEL_MLVDS_PANASONIC32WXGA8BIT[i].u4Msk)) | (sRegMini_PANEL_MLVDS_PANASONIC32WXGA8BIT[i].u4Val);

	    	  vIO32Write4B(u4tmp,u4tmp1);
             }

           LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegMini_PANEL_MLVDS_PANASONIC32WXGA8BIT), sizeof(struct sTCONREGTABLE));

           break;	   
	   	   
		 case PANEL_MLVDS_SS_MB4:

	         LOG(1,"cust tg init\n");

	         u1count = (sizeof(sRegMini_PANEL_MLVDS_SAMSUNGMB4) / sizeof(struct sTCONREGTABLE));

	         for ( i = 0 ; i < u1count ; i++)
	 {
              u4tmp = sRegMini_PANEL_MLVDS_SAMSUNGMB4[i].u4Addr;
	          u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegMini_PANEL_MLVDS_SAMSUNGMB4[i].u4Msk)) | (sRegMini_PANEL_MLVDS_SAMSUNGMB4[i].u4Val);

	    	  vIO32Write4B(u4tmp,u4tmp1);
             }

           LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegMini_PANEL_MLVDS_SAMSUNGMB4), sizeof(struct sTCONREGTABLE));

         break;

		 case PANEL_MLVDS_SS_MB7:

	         LOG(1,"cust tg init\n");

	         u1count = (sizeof(sRegMini_PANEL_MLVDS_SAMSUNGMB7) / sizeof(struct sTCONREGTABLE));

	         for ( i = 0 ; i < u1count ; i++)
	 {
              u4tmp = sRegMini_PANEL_MLVDS_SAMSUNGMB7[i].u4Addr;
	          u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegMini_PANEL_MLVDS_SAMSUNGMB7[i].u4Msk)) | (sRegMini_PANEL_MLVDS_SAMSUNGMB7[i].u4Val);

	    	  vIO32Write4B(u4tmp,u4tmp1);
             }

           LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegMini_PANEL_MLVDS_SAMSUNGMB7), sizeof(struct sTCONREGTABLE));

         break;         
         
		 default:
		  	
		   break;
		}
    return(1);
}

UINT8 u1TconCustEPIInit(void)
{
 	UINT8 i = 0;
	UINT8 u1count;
	UINT32 u4tmp, u4tmp1;
	
//    vIO32WriteFldAlign(CKGEN_CPU_CKCFG, 0, FLD_BUS_CK_SEL);//Bus clk
	switch (GetCurrentPanelIndex())
	 {
	   
	 case PANEL_EPI_LGDV11GIPFHD60:

	     LOG(1,"cust tg init\n");

	     u1count = (sizeof(sReg_PANEL_EPI_LGDV11FHD60) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
          u4tmp = sReg_PANEL_EPI_LGDV11FHD60[i].u4Addr;
	      u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sReg_PANEL_EPI_LGDV11FHD60[i].u4Msk)) | (sReg_PANEL_EPI_LGDV11FHD60[i].u4Val);

		  vIO32Write4B(u4tmp,u4tmp1);
         }

       LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sReg_PANEL_EPI_LGDV11FHD60), sizeof(struct sTCONREGTABLE));

       break;
		}
  return(1);
}

#ifdef CC_SUPPORT_FLASH_TCON
struct sTCONREGTABLE * pTconCustTgGet(UINT32 *u4Length)
{
	switch (GetCurrentPanelIndex())
	 {
	   case PANEL_MLVDS_LGDV4FHD60: 

	     *u4Length = (sizeof(sRegCustTg_MLVDS_LGDV4FHD60) / sizeof(struct sTCONREGTABLE));
	     return ((struct sTCONREGTABLE *) &sRegCustTg_MLVDS_LGDV4FHD60[0]);
         
	  case PANEL_MLVDS_LGDV4FHD120: 

	     *u4Length = (sizeof(sRegCustTg_MLVDS_LGDV4FHD120) / sizeof(struct sTCONREGTABLE));
	     return ((struct sTCONREGTABLE *) &sRegCustTg_MLVDS_LGDV4FHD120[0]);

	  case PANEL_MLVDS_SS_MB4: 

	     *u4Length = (sizeof(sRegCustTg_MLVDS_SAMSUNGMB4) / sizeof(struct sTCONREGTABLE));
	     return ((struct sTCONREGTABLE *) &sRegCustTg_MLVDS_SAMSUNGMB4[0]);

	  case PANEL_MLVDS_SS_MB7: 

	     *u4Length = (sizeof(sRegCustTg_MLVDS_SAMSUNGMB7) / sizeof(struct sTCONREGTABLE));
	     return ((struct sTCONREGTABLE *) &sRegCustTg_MLVDS_SAMSUNGMB7[0]);
         
	   default:
	   	u4Length = 0;
		 break;
    }	

  return(NULL);
}
#endif /*CC_SUPPORT_FLASH_TCON*/

//#ifdef CC_SMALL_TCON_BOARD
#define TCON_EEPROM_ChannelID 2
#define TCON_EEPROM_CLK_RATE 0x100
#define TCON_EEPROM_ID 0xA0
#define TCON_DVR_ID 0x9E
#define TCON_GAMMA_ID 0xE8
#define TCON_PMIC_ID 0x42
#define TCON_EEPROM_DATA_CNT 144*4/3
#define TCON_REG_START 0xF0034800
//UINT8 sRegCustEEPROM[TCON_EEPROM_DATA_CNT];

void vTconCustTGPrint(void)
{
UINT8 i = 0;
//UINT8 u1count;
UINT32 u4tmp,u4tmp1;

     for ( i = 0 ; i < TCON_EEPROM_DATA_CNT/4*3 ; i++)
     {
         u4tmp = TCON_REG_START + 4*i;
         u4tmp1 = u4IO32Read4B(u4tmp);
         printf("Addr = 0x%x, Vaule=0x%x\n",u4tmp, (u4tmp1)&0xFF);
         printf("Addr = 0x%x, Vaule=0x%x\n",u4tmp+1, (u4tmp1>>8)&0xFF);
         printf("Addr = 0x%x, Vaule=0x%x\n",u4tmp+2, (u4tmp1>>16)&0xFF);
         printf("Addr = 0x%x, Vaule=0x%x\n",u4tmp+3, (u4tmp1>>24)&0xFF);
     }
}


void u4TconCustEepromRead(UINT32 u4offset, UINT8 *pu1Buf,  UINT16 u2Count)
{

//   SIF_Read(TCON_EEPROM_CLK_RATE, TCON_EEPROM_ID, 0, sRegCustEEPROM, TCON_EEPROM_DATA_CNT); //Read total data
   SIF_X_Read(TCON_EEPROM_ChannelID,TCON_EEPROM_CLK_RATE, TCON_EEPROM_ID, 2 , u4offset,  (UINT8 *)pu1Buf, u2Count);

}
static UINT32 _u4TconCustI2CWrite(UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4offset, UINT8 *pu1Buf, UINT16 u2Count)
{
//	Printf("0x%8x, 0x%8x, 0xffffffff,\n", u1offset, (pu1Buf[0]<<8) | pu1Buf[1]);
    return(SIF_X_Write(TCON_EEPROM_ChannelID, TCON_EEPROM_CLK_RATE, u1DevAddr, u1WordAddrNum, u4offset, (UINT8 *)pu1Buf, u2Count));

}

void vTconCustTGLoadEEP(void)
{
	UINT8 i = 0;
	UINT8 u1count;
	UINT32 u4tmp, u4tmp1;
	UINT8 *u1tmp;

    u1tmp = (UINT8 *)x_mem_alloc(TCON_EEPROM_DATA_CNT);
	if (u1tmp == NULL)
    {        
       LOG(0, "vTconCustTGLoadEEP : allocate memory fail\n");
       return;
    }
//////// for PMIC and gamma start /////////////////
    vIO32Write4BMsk(GR8, 0x00800080,0x00800080); // write protect

    u4TconCustEepromRead(5121,u1tmp,9);
    _u4TconCustI2CWrite(TCON_PMIC_ID,1,0,u1tmp,9);

    u4TconCustEepromRead(8145,u1tmp,1);
    _u4TconCustI2CWrite(TCON_DVR_ID,0,0,u1tmp,1);

    u4TconCustEepromRead(8146,u1tmp,14);
    _u4TconCustI2CWrite(TCON_GAMMA_ID,1,0,u1tmp,14);

    vIO32Write4BMsk(GR8, 0x00000000,0x00800080); // write protect
//////// for PMIC and gamma end /////////////////

//////// for TG timing start /////////////////
    u4TconCustEepromRead(5136,u1tmp,TCON_EEPROM_DATA_CNT);
    LOG(1,"cust tg init\n");
    
    u1count = TCON_EEPROM_DATA_CNT/4;
    
    for ( i = 0 ; i < u1count ; i++)
    {
      u4tmp = TCON_REG_START + 4*i;
      u4tmp1 = (((UINT32)u1tmp[i*4]))|(((UINT32)u1tmp[i*4+1])<<8)|(((UINT32)u1tmp[i*4+2])<<16)|(((UINT32)u1tmp[i*4+3])<<24);
    
      vIO32Write4B(u4tmp,u4tmp1);
	  LOG(1,"Tcon reg addr 0x%x data 0x%x \n", u4tmp, u4tmp1);
    }
    
    u4TconCustEepromRead(5328,u1tmp,TCON_EEPROM_DATA_CNT);
    u1count = TCON_EEPROM_DATA_CNT/4;
    
    for ( i = 0 ; i < u1count ; i++)
    {
      u4tmp = TCON_REG_START + 4*i + 0xC0;
      u4tmp1 = (((UINT32)u1tmp[i*4]))|(((UINT32)u1tmp[i*4+1])<<8)|(((UINT32)u1tmp[i*4+2])<<16)|(((UINT32)u1tmp[i*4+3])<<24);
    
      vIO32Write4B(u4tmp,u4tmp1);
	  LOG(1,"Tcon reg addr 0x%x data 0x%x \n", u4tmp, u4tmp1);
    }
    
    u4TconCustEepromRead(5520,u1tmp,TCON_EEPROM_DATA_CNT);
    u1count = TCON_EEPROM_DATA_CNT/4;
    
    for ( i = 0 ; i < u1count ; i++)
    {
      u4tmp = TCON_REG_START + 4*i + 0x180;
      u4tmp1 = (((UINT32)u1tmp[i*4]))|(((UINT32)u1tmp[i*4+1])<<8)|(((UINT32)u1tmp[i*4+2])<<16)|(((UINT32)u1tmp[i*4+3])<<24);
    
      vIO32Write4B(u4tmp,u4tmp1);
	  LOG(1,"Tcon reg addr 0x%x data 0x%x \n", u4tmp, u4tmp1);
    }
    
    LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegCustTg_MLVDS_SAMSUNGMB7), sizeof(struct sTCONREGTABLE));
//////// for TG timing end /////////////////
    

    x_mem_free( (VOID *)u1tmp );
}
UINT8 tcon_Vcom_Read(void)
{
	UINT8 *u1tmp;
	UINT8 u1data;

        u1tmp = (UINT8 *)x_mem_alloc(TCON_EEPROM_DATA_CNT);
		if (u1tmp == NULL)
        {        
          LOG(0, "tcon_Vcom_Read : allocate memory fail\n");
          return 0;
        }
        u4TconCustEepromRead(8145,u1tmp,1);
        u1data = u1tmp[0];    
        x_mem_free( (VOID *)u1tmp );
    return u1data;
}
UINT8 tcon_Vcom_Write(UINT8 val)
{
	UINT8 *u1tmp;
	UINT8 u1result=0;
    u1tmp = (UINT8 *)x_mem_alloc(TCON_EEPROM_DATA_CNT);
	if (u1tmp == NULL)
    {        
      LOG(0, "tcon_Vcom_Write : allocate memory fail\n");
      return 0;
    }
    u1tmp[0] = val;
	GPIO_SetOut(39, 1);
	x_thread_delay(100);
    _u4TconCustI2CWrite(TCON_DVR_ID,0,0,u1tmp,1);
	GPIO_SetOut(39, 0);
    u1result = _u4TconCustI2CWrite(TCON_EEPROM_ID,2,8145,u1tmp,1);

    return u1result;
}
//UINT8 tcon_DTV(void)
//{
//  return fgIsMainDTV();
//}
//#endif
#endif

