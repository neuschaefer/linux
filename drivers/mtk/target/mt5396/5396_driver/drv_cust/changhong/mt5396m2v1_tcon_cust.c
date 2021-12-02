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


//-----------------------------------------------------------------------------
// mini LVDS Interface Setting
//-----------------------------------------------------------------------------

//miniLVDS LGDV4/V5/V6
  struct sTCONREGTABLE sRegMini_PANEL_MLVDS_LGDV4FHD60[] = {

   // minilvds Pair select
   {0xf0035104, 0x1230A567, 0xffffffff}, // Pair Selection of L Link 
   {0xf0035108, 0x1320A765, 0xffffffff}, // Pair Selection of R Link  
   {0xf003510C, 0x04049898, 0xffffffff}, 

   //minilvds reset pulse            
   {0xf003511c, 0x00000000, 0xffffffff},
   {0xf0035120, 0x00000000, 0xffffffff},
   {0xf0035100, 0x0000003f, 0xffffffff},

   //minilvds PN Swap Setting        
   {0xf0035130, 0x00000025, 0xffffffff},

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
	{0xf0035104, 0x01200546, 0xffffffff},  // Pair Selection of L Link 
	{0xf0035108, 0x01020465, 0xffffffff},  // Pair Selection of R Link  
  
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
	{0xf0035000, 0x2002B359, 0xffffffff},
  
	// minilvds input source
	{0xf0035280, 0x80000000, 0xffffffff}, //Input Source, WXGA Dummy Pixel
  
  };

//mini LVDS : PCB P1V1
struct sTCONREGTABLE sRegCustMlvds_PANASONIC32WXGA8BIT[] = {

  // clk source
  {0xf0035000, 0x2012A358, 0xffffffff},
  	
  // Bit[20]: mini LVDS LR swap
  {0xf0035110, 0x00000000, 0xffffffff},

  //minilvds PN Swap Setting		
  {0xf0035130, 0x00000025, 0xffffffff},

  // minilvds pair select
  {0xf0035104, 0x1230A567, 0xffffffff}, // Pair Selection of L Link 
  {0xf0035108, 0x1320A765, 0xffffffff}, // Pair Selection of R Link  
  {0xf003510C, 0x04049898, 0xffffffff},
  
  // minilvds reset pulse
  {0xf0035100, 0x0000003f, 0xffffffff},

  // minilvds PN setting
  {0xf0035130, 0x00000000, 0xffffffff},
  
  // mute pattern H/V total, WXGA
  {0xf0035004, 0x05580628, 0xffffffff},
  {0xf0035008, 0x0300033A, 0xffffffff},
  
  // WXGA Dummy Pixel
  {0xf0035280, 0x82AA0028, 0xffffffff},

  // minilvds Output Enable         
  {0xf0035118, 0x00000000, 0xffffffff},
  // Fix DE
  //0xf0034468, 0x05420558, 0xffffffff,
};

//miniLVDS CMO WXGA
  struct sTCONREGTABLE sRegMini_PANEL_MLVDS_WXGA[] = {

  // minilvds Pair select
  {0xf0035104, 0x01200546, 0xffffffff},  // Pair Selection of L Link 
  {0xf0035108, 0x01020465, 0xffffffff},  // Pair Selection of R Link  

  //minilvds reset pulse
  {0xf003511c, 0x00000000, 0xffffffff},
  {0xf0035120, 0x00000000, 0xffffffff},
  {0xf0035100, 0x0000003f, 0xffffffff},
	  
  //minilvds PN Swap Setting
  {0xf0035130, 0x00000054, 0xffffffff},
		  
  // minilvds Mute Pattern Color and Enable
  {0xf0035114, 0x0000cFF2, 0xffffffff},
	  
  // bit[20]: RL Link Swap
  {0xf0035110, 0x00000000, 0xffffffff},
	  
  // minilvds Output Enable
  {0xf0035118, 0x00000000, 0xffffffff},
	  
  // clk source
  //Interface Selection, Blue Shift, bit Selection, Pair Selection, Port Selection
  {0xf0035000, 0x2002a359, 0xffffffff},

  // minilvds input source
  {0xf0035280, 0x80000000, 0xffffffff}, //Input Source, WXGA Dummy Pixel

  // mute pattern H/V total, WXGA
  {0xf0035004, 0x05580647, 0xffffffff}, //minilvds Mute Pattern H-active, H-Total
  {0xf0035008, 0x03000329, 0xffffffff}, //minilvds Mute Pattern V-active, V-Total

  };


//-----------------------------------------------------------------------------
// EPI Interface Setting
//-----------------------------------------------------------------------------

  //EPI LGDV11FHD60
  struct sTCONREGTABLE sReg_PANEL_EPI_LGDV11FHD120[] = {

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

  //EPI LGDV12FHD120
  struct sTCONREGTABLE sReg_PANEL_EPI_LGDV12FHD120[] = {

  //panel interface
  //0xf0034600,  0x0400a359, 0xffffffff,          // bit[16] bypass 6line bit[20]: disable 2ch // modify  Y.W.
  {0xf0034600, 0x0402a359, 0xffffffff},  // bit[16] bypass 6line bit[20]: disable 2ch // modify  Y.W.

//  0xf0034604, 0x07800894, 0xffffffff,  // h active, h total 
  {0xf0034710, 0x80000000, 0xffffffff}, 
  //0xf0034614, 0x03090100, 0xffffffff,
//  0xf0034618, 0x02090100, 0xffffffff,  // bit[16] enable ptgen, bit[17] disable 2ch bit[19] enable seq, bit[20] disable mirror

  //pattern gen pure color
  //0xf0034618, 0x01090100, 0xffffffff,              // 24:25= 1 pure
  {0xf0034614, 0x000003ff, 0xffffffff},  //RGB [9:0] B [19:10] G [29:20] R

  //mlvds
  {0xf0035000, 0x8002A559, 0xffffffff}, //scan line upside down
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
  struct sTCONREGTABLE sRegTg_PANEL_MLVDS_LGDV4FHD60[] = {

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

  //defined(LGDV5GIP)
  struct sTCONREGTABLE sRegTg_PANEL_MLVDS_LGDV5GIPFHD60[] = {
                            													
  // SOE, TG0
  {0xf0034840, 0x00C807d0, 0xffffffff},
  {0xf0034844, 0x00000000, 0xffffffff},
  {0xf0034848, 0x00C807d0, 0xffffffff},  
  {0xf003484c, 0x04430007, 0xffffffff},
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
  {0xf00349A0, 0x01F405DC, 0xffffffff},
  {0xf00349A4, 0x00000000, 0xffffffff},
  {0xf00349A8, 0x00000000, 0xffffffff},
  {0xf00349Ac, 0x00030001, 0xffffffff},
  {0xf00349B0, 0x00000000, 0xffffffff},
  {0xf00349B4, 0x00000000, 0xffffffff},
  {0xf00349B8, 0x00000004, 0xffffffff},
  {0xf00349Bc, 0x00000000, 0xffffffff},
  																											
  // GCLK 1, TG8																				
  {0xf0034940, 0x06AE083E, 0xffffffff},
  {0xf0034944, 0x00000000, 0xffffffff},
  {0xf0034948, 0x06AE083E, 0xffffffff},
  {0xf003494c, 0x04420005, 0xffffffff},
  {0xf0034950, 0x80000606, 0xffffffff},
  {0xf0034954, 0x80050002, 0xffffffff},
  {0xf0034958, 0x00000007, 0xffffffff},
  {0xf003495c, 0x00000000, 0xffffffff},
                                 
  // GCLK 2, TG5          																													
  {0xf00348E0, 0x06AE083E, 0xffffffff},
  {0xf00348E4, 0x00000000, 0xffffffff},
  {0xf00348E8, 0x06AE083E, 0xffffffff},
  {0xf00348EC, 0x04430006, 0xffffffff},
  {0xf00348F0, 0x80000606, 0xffffffff},
  {0xf00348F4, 0x80040001, 0xffffffff},
  {0xf00348F8, 0x00000007, 0xffffffff},
  {0xf00348FC, 0x00000000, 0xffffffff},
                                 
  // GCLK 3, TG12       																													
  {0xf00349C0, 0x06AE083E, 0xffffffff},
  {0xf00349C4, 0x00000000, 0xffffffff},
  {0xf00349C8, 0x06AE083E, 0xffffffff},
  {0xf00349CC, 0x04440007, 0xffffffff},
  {0xf00349D0, 0x80000606, 0xffffffff},
  {0xf00349D4, 0x80030006, 0xffffffff},
  {0xf00349D8, 0x00000007, 0xffffffff},
  {0xf00349DC, 0x00000000, 0xffffffff},
                                 
  // GCLK 4, TG2     																													
  {0xf0034880, 0x06AE083E, 0xffffffff},
  {0xf0034884, 0x00000000, 0xffffffff},
  {0xf0034888, 0x06AE083E, 0xffffffff},
  {0xf003488C, 0x043F0002, 0xffffffff},
  {0xf0034890, 0x80000606, 0xffffffff},
  {0xf0034894, 0x80020005, 0xffffffff},
  {0xf0034898, 0x00000007, 0xffffffff},
  {0xf003489C, 0x00000000, 0xffffffff},
                                 
  // GCLK 5, TG6												
  {0xf0034900, 0x06AE083E, 0xffffffff},
  {0xf0034904, 0x00000000, 0xffffffff},
  {0xf0034908, 0x06AE083E, 0xffffffff},
  {0xf003490C, 0x04400003, 0xffffffff},
  {0xf0034910, 0x80000606, 0xffffffff},
  {0xf0034914, 0x80010004, 0xffffffff},
  {0xf0034918, 0x00000007, 0xffffffff},
  {0xf003491C, 0x00000000, 0xffffffff},
                                 
  // GCLK 6, TG7                 
  {0xf0034920, 0x06AE083E, 0xffffffff},
  {0xf0034924, 0x00000000, 0xffffffff},
  {0xf0034928, 0x06AE083E, 0xffffffff},
  {0xf003492C, 0x04410004, 0xffffffff},
  {0xf0034930, 0x80000606, 0xffffffff},
  {0xf0034934, 0x80060003, 0xffffffff},
  {0xf0034938, 0x00000007, 0xffffffff},
  {0xf003493C, 0x00000000, 0xffffffff},
  							
  // FLK, TG9 always high for GIP																																																	                               
  {0xf0034820, 0x02000200, 0x02000200}, //GPO :1
                                 
  // ODD, TG3																										
  {0xf00348A0, 0x002d0015, 0xffffffff},
  {0xf00348A4, 0x002d0015, 0xffffffff},
  {0xf00348A8, 0x002d0015, 0xffffffff},
  {0xf00348AC, 0x00010000, 0xffffffff},
  {0xf00348B0, 0x00000000, 0xffffffff},
  {0xf00348B4, 0x04500450, 0xffffffff},
  {0xf00348B8, 0x00000103, 0xffffffff},
  {0xf00348BC, 0x80080001, 0xffffffff},
  
  // EVEN, TG4             																														
  {0xf00348C0, 0x0015002d, 0xffffffff},
  {0xf00348C4, 0x0015002d, 0xffffffff},
  {0xf00348C8, 0x0015002d, 0xffffffff},
  {0xf00348CC, 0x00010000, 0xffffffff},
  {0xf00348D0, 0x00000000, 0xffffffff},
  {0xf00348D4, 0x04500450, 0xffffffff},
  {0xf00348D8, 0x00000003, 0xffffffff},
  {0xf00348DC, 0x80080001, 0xffffffff},
                                
  // DPM, TG10     
  {0xf0034820, 0x04000400, 0x04000400}, //GPO :1
    																						                                    																														
  // GR0                             																														
  // 0xf0034800, 0x8100ffff, 0xffffffff,
  
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
	{0xf00349Dc, 0x00000000, 0xffffffff},               
	                                  
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

  //TCON Control Signal
  struct sTCONREGTABLE sRegCustTg_PANASONIC32WXGA8BIT[] = {

	{0xF003481C, 0x00000000, 0xffffffff},
	{0xF0034820, 0x00000000, 0xffffffff},
	{0xF0034824, 0x00000000, 0xffffffff},

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
                                          
	// OR merge TP2 and TP14 to DATA1 
	// OR merge TP5 and TP1   to DATA2
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

				
  //defined(LGDV6GIP)
  struct sTCONREGTABLE sRegTg_PANEL_MLVDS_LGDV6GIPFHD60[] = {
					 
  // SOE, TG0  
  {0xf0034820, 0x003c0816, 0xffffffff},
  {0xf0034824, 0x00000000, 0xffffffff},
  {0xf0034828, 0x003C0816, 0xffffffff},  
  {0xf003482c, 0x04440001, 0xffffffff},
  {0xf0034830, 0x20000000, 0xffffffff},
  {0xf0034834, 0x80000000, 0xffffffff},
  {0xf0034838, 0x00000007, 0xffffffff},
  {0xf003483c, 0x00000000, 0xffffffff},																													
                                       																														
  // POL, OK                    																														
  {0xf0034840, 0x07D007D0, 0xffffffff},																														
  {0xf0034844, 0x00000000, 0xffffffff},																														
  {0xf0034848, 0x07D007D0, 0xffffffff},																														
  //0xf003484c, 0x043F0007, 0xffffffff,																														
  {0xf003484c, 0x00010001, 0xffffffff},																														  
  {0xf0034850, 0x80000404, 0xffffffff},																														
  //0xf0034854, 0x80020004, 0xffffffff,																														
  {0xf0034854, 0x80040004, 0xffffffff},
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
  																												
  // GCLK 1, // V6 GCLK																					
  {0xf0034880, 0x03000280, 0xffffffff},
  {0xf0034884, 0x00000000, 0xffffffff},
  {0xf0034888, 0x03000280, 0xffffffff},
  {0xf003488c, 0x043f0003, 0xffffffff},
  {0xf0034890, 0x20000000, 0xffffffff},
  {0xf0034894, 0x80000000, 0xffffffff},
  {0xf0034898, 0x00000007, 0xffffffff},
  {0xf003489c, 0x00000000, 0xffffffff},
                                    
  // GCLK 2,  // V6 MCLK		
  {0xf00348A0, 0x06F006E0, 0xffffffff},
  {0xf00348A4, 0x00000000, 0xffffffff},
  {0xf00348A8, 0x06F006E0, 0xffffffff},
  {0xf00348Ac, 0x04410005, 0xffffffff},
  {0xf00348B0, 0x20000000, 0xffffffff},
  {0xf00348B4, 0x80000000, 0xffffffff},
  {0xf00348B8, 0x00000007, 0xffffffff},
  {0xf00348Bc, 0x00000000, 0xffffffff},
                                    
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
                                    
  // GCLK 5	, // TG7, V6 or with GST1
  {0xf0034900, 0x01F405DC, 0xffffffff},
  {0xf0034904, 0x00000000, 0xffffffff},
  {0xf0034908, 0x00000000, 0xffffffff},
  {0xf003490C, 0x04480445, 0xffffffff},
  {0xf0034910, 0x00000000, 0xffffffff},
  {0xf0034914, 0x00000000, 0xffffffff},
  {0xf0034918, 0x00000004, 0xffffffff},
  {0xf003491C, 0x00000000, 0xffffffff},
                                    
  // TGE, V6 | with GCLK            
  {0xf00349C0, 0x03000280, 0xffffffff},
  {0xf00349C4, 0x00000000, 0xffffffff},
  {0xf00349C8, 0x03000280, 0xffffffff},
  {0xf00349CC, 0x0443043f, 0xffffffff},
  {0xf00349D0, 0x00000000, 0xffffffff},
  {0xf00349D4, 0x00000000, 0xffffffff},
  {0xf00349D8, 0x00000004, 0xffffffff},
  {0xf00349DC, 0x00000000, 0xffffffff},     
                                    
  // TGE, V6 xor with GST1          
  {0xf00349E0, 0x01F405DC, 0xffffffff},
  {0xf00349E4, 0x00000000, 0xffffffff},
  {0xf00349E8, 0x00000000, 0xffffffff},
  {0xf00349EC, 0x00030001, 0xffffffff},
  {0xf00349F0, 0x00000000, 0xffffffff},
  {0xf00349F4, 0x00000000, 0xffffffff},
  {0xf00349F8, 0x00000004, 0xffffffff},
  {0xf00349FC, 0x00000000, 0xffffffff},   
                                    
  // TGF, V6 xor with EO            
  {0xf0034A00, 0x001500CA, 0xffffffff},
  {0xf0034A04, 0x001500CA, 0xffffffff},
  {0xf0034A08, 0x001500CA, 0xffffffff},
  {0xf0034A0C, 0x00010000, 0xffffffff},
  {0xf0034A10, 0x00000000, 0xffffffff},
  {0xf0034A14, 0x04500450, 0xffffffff},
  {0xf0034A18, 0x00000103, 0xffffffff},
  {0xf0034A1C, 0x800ff001, 0xffffffff},
                                    
  //FLK, always high for GIP, OK																																																		
  {0xf0034958, 0x00000037, 0xffffffff},
                                    
  //ODD	, // TGB V6 EO																											
  {0xf0034980, 0x00CA0015, 0xffffffff},
  {0xf0034984, 0x00CA0015, 0xffffffff},
  {0xf0034988, 0x00CA0015, 0xffffffff},
  {0xf003498C, 0x00010000, 0xffffffff},
  {0xf0034990, 0x00000000, 0xffffffff},
  {0xf0034994, 0x04500450, 0xffffffff},
  {0xf0034998, 0x00000103, 0xffffffff},
  {0xf003499C, 0x800ff001, 0xffffffff},
                                    
  //EVEN, // TGC V6 GST1            
  {0xf00349A0, 0x01F405DC, 0xffffffff},
  {0xf00349A4, 0x00000000, 0xffffffff},
  {0xf00349A8, 0x00000000, 0xffffffff},
  {0xf00349AC, 0x04440441, 0xffffffff},
  {0xf00349B0, 0x00000000, 0xffffffff},
  {0xf00349B4, 0x00000000, 0xffffffff},
  {0xf00349B8, 0x00000004, 0xffffffff},
  {0xf00349BC, 0x00000000, 0xffffffff},
                                    
                                    
  // TG merge                       
  {0xf0034814, 0x02000080, 0xffffffff},
  {0xf0034818, 0x01C00040, 0xffffffff},    
                                    
                                    
  // DPM, OK            																														
  {0xf0034978, 0x00000030, 0xffffffff},
                                    
  };                                
                                    
  //defined CMO WXGA                
  struct sTCONREGTABLE sRegTg_PANEL_MLVDS_WXGA[] = {
                                    
  // SOE, ok                         
  {0xf0034820, 0x005885B2, 0xffffffff},
  {0xf0034824, 0x00000000, 0xffffffff},
  {0xf0034828, 0x005885B2, 0xffffffff},
  {0xf003482c, 0x032c0001, 0xffffffff},
  {0xf0034830, 0x20000000, 0xffffffff},
  {0xf0034834, 0x80000000, 0xffffffff},
  {0xf0034838, 0x00000007, 0xffffffff},
  {0xf003483c, 0x00000000, 0xffffffff},
                                     
  // POL, MR - ~ML OK                
  {0xf0034840, 0x05600560, 0xffffffff},
  {0xf0034844, 0x00000000, 0xffffffff},
  {0xf0034848, 0x05600560, 0xffffffff},
  {0xf003484c, 0x032c0001, 0xffffffff},
  {0xf0034850, 0x00000404, 0xffffffff},
  {0xf0034854, 0x80020004, 0xffffffff},
  {0xf0034858, 0x00000107, 0xffffffff},
  {0xf003485c, 0x10000000, 0xffffffff},
                                      
  // GOE, tg2 data 2 ok                   
  {0xf0034860, 0x00CF00CF, 0xffffffff},
  {0xf0034864, 0x00000000, 0xffffffff},
  {0xf0034868, 0x00000000, 0xffffffff},
  {0xf003486c, 0x00090007, 0xffffffff},
  {0xf0034870, 0x00000000, 0xffffffff},
  {0xf0034874, 0x00000000, 0xffffffff},
  {0xf0034878, 0x00000004, 0xffffffff},
  {0xf003487c, 0x00000000, 0xffffffff},
                                    
  // GSP --> GOE or GSC  // TPK 3          
  {0xf0034980, 0x05B00270, 0xffffffff},																														
  {0xf0034984, 0x00000000, 0xffffffff},																														
  {0xf0034988, 0x05B00270, 0xffffffff},																														
  {0xf003498c, 0x032c0001, 0xffffffff},																														
  {0xf0034990, 0x20000000, 0xffffffff},																														
  {0xf0034994, 0x80000000, 0xffffffff},																														
  {0xf0034998, 0x00000007, 0xffffffff},																														
  {0xf003499c, 0x00000000, 0xffffffff},
                                    
  // TPK 3 for data 1               
  {0xf0034880, 0x01000410, 0xffffffff},
  {0xf0034884, 0x00000000, 0xffffffff},
  {0xf0034888, 0x01000410, 0xffffffff},
  {0xf003488c, 0x00080006, 0xffffffff},
  {0xf0034890, 0x20000000, 0xffffffff},
  {0xf0034894, 0x80000000, 0xffffffff},
  {0xf0034898, 0x00000107, 0xffffffff},
  {0xf003489c, 0x00000000, 0xffffffff},  
                                    
                                       
  // GSP_R, OK                      
  #if 0                             
  {0xf0034920, 0x01000410, 0xffffffff},
  {0xf0034924, 0x00000000, 0xffffffff},
  {0xf0034928, 0x01000410, 0xffffffff},
  {0xf003492c, 0x00080006, 0xffffffff},
  {0xf0034930, 0x20000000, 0xffffffff},
  {0xf0034934, 0x80000000, 0xffffffff},
  {0xf0034938, 0x00000107, 0xffffffff},
  {0xf003493c, 0x00000000, 0xffffffff},  
  #endif                            
                                     
  // GSC, data 1 ok                  
  {0xf00349a0, 0x00CF00CF, 0xffffffff},
  {0xf00349a4, 0x00000000, 0xffffffff},
  {0xf00349a8, 0x00000000, 0xffffffff},
  {0xf00349ac, 0x00090006, 0xffffffff},
  {0xf00349b0, 0x00000000, 0xffffffff},
  {0xf00349b4, 0x00000000, 0xffffffff},
  {0xf00349b8, 0x00000004, 0xffffffff},
  {0xf00349bc, 0x00000000, 0xffffffff},
                                     
  // FLK                             
  {0xf0034940, 0x044C0578, 0xffffffff},
  {0xf0034944, 0x00000000, 0xffffffff},
  {0xf0034948, 0x044C0578, 0xffffffff},
  {0xf003494c, 0x03080005, 0xffffffff},
  {0xf0034950, 0x20000000, 0xffffffff},
  {0xf0034954, 0x80000000, 0xffffffff},
  {0xf0034958, 0x00000007, 0xffffffff},
  {0xf003495c, 0x00000000, 0xffffffff},
                                     
  // TPK 13 for data 2              
  {0xf00349C0, 0x01000410, 0xffffffff},
  {0xf00349C4, 0x00000000, 0xffffffff},
  {0xf00349C8, 0x01000410, 0xffffffff},
  {0xf00349Cc, 0x00080006, 0xffffffff},
  {0xf00349D0, 0x20000000, 0xffffffff},
  {0xf00349D4, 0x80000000, 0xffffffff},
  {0xf00349D8, 0x00000107, 0xffffffff},
  {0xf00349Dc, 0x00000000, 0xffffffff},
                                     
                                     
  // DPM, OK                         
  {0xf0034978, 0x00000030, 0xffffffff},
                                     
  // H_CONV, OK                      
  {0xf00348d8, 0x00000020, 0xffffffff},
                                     
  // OPT_N, POL, ML OK               
  {0xf00348e0, 0x05600560, 0xffffffff},
  {0xf00348e4, 0x00000000, 0xffffffff},
  {0xf00348e8, 0x05600560, 0xffffffff},
  {0xf00348ec, 0x032c0001, 0xffffffff},
  {0xf00348f0, 0x00000404, 0xffffffff},
  {0xf00348f4, 0x80020004, 0xffffffff},
  {0xf00348f8, 0x00000007, 0xffffffff},
  {0xf00348fc, 0x10000000, 0xffffffff},
                                     
  // 0PT_P , TG7                     
  {0xf0034918, 0x00000020, 0xffffffff},
                                    
  // GR5                            
  {0xf0034814, 0x03000020, 0xffffffff},
                                    
  // GR6                            
  {0xf0034818, 0x02000020, 0xffffffff},
  };

struct sTCONREGTABLE sRegTg_EPI_LGDV11FHD120[] = {
    //GCLK1    TCO[0]  0xf0034840
    {0xf0034840, 0x012c0001, 0xffffffff},
    {0xf0034844, 0x00000000, 0xffffffff},
    {0xf0034848, 0x012c0001, 0xffffffff},
    {0xf003484c, 0x00070006, 0xffffffff},
    {0xf0034850, 0x00000000, 0xffffffff},
    {0xf0034854, 0x00000000, 0xffffffff},
    {0xf0034858, 0x00000004, 0xffffffff},
    {0xf003485c, 0x00000000, 0xffffffff},
                                      
    //GCLK2    TCO[1]  0xf0034860     
    {0xf0034860, 0x00000000, 0xffffffff},
    {0xf0034864, 0x00000000, 0xffffffff},
    {0xf0034868, 0x00000000, 0xffffffff},
    {0xf003486c, 0x00000000, 0xffffffff},
    {0xf0034870, 0x00000000, 0xffffffff},
    {0xf0034874, 0x00000000, 0xffffffff},
    {0xf0034878, 0x00000080, 0xffffffff},
    {0xf003487c, 0x20000000, 0xffffffff},
                                      
    //GCLK3    TCO[2]  0xf0034880     
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
                                      
                                      
    // FLK, TG9 always high for GIP                                                                                                                                                                                                                                      
    {0xf0034820, 0x02000200, 0x02000200}, //GPO :1
                                      
    {0xf0034800, 0x8100ffff, 0xffffffff},
    {0xf0034804, 0x00000000, 0xffffffff},
    {0xf003481c, 0x00000000, 0xffffffff},

    };

struct sTCONREGTABLE sRegTg_EPI_LGDV12FHD120[] = {
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
    {0xf00348Cc, 0x04420005, 0xffffffff},
    {0xf00348D0, 0x20000000, 0xffffffff},
    {0xf00348D4, 0x80000000, 0xffffffff},
    {0xf00348D8, 0x00000007, 0xffffffff},
    {0xf00348Dc, 0x00000000, 0xffffffff},

    //GCLK    TCO[3]  0xf00348a0
    {0xf00348A0, 0x0080006A, 0xffffffff},
    {0xf00348A4, 0x00000000, 0xffffffff},
    {0xf00348A8, 0x0080006A, 0xffffffff},
    {0xf00348Ac, 0x04400003, 0xffffffff},
    {0xf00348B0, 0x20000000, 0xffffffff},
    {0xf00348B4, 0x80000000, 0xffffffff},
    {0xf00348B8, 0x00000007, 0xffffffff},
    {0xf00348Bc, 0x00000000, 0xffffffff},

    //GCLK_END    TCO[8]  0xf0034940
    {0xf0034940, 0x0080006A, 0xffffffff},
    {0xf0034944, 0x00000000, 0xffffffff},
    {0xf0034948, 0x0080006A, 0xffffffff},
    {0xf003494c, 0x04440440, 0xffffffff},
    {0xf0034950, 0x00000000, 0xffffffff},
    {0xf0034954, 0x00000000, 0xffffffff},
    {0xf0034958, 0x00000004, 0xffffffff},
    {0xf003495c, 0x00000000, 0xffffffff}, 

    //EO   TCO[6]  0xf0034900
    {0xf0034900, 0x00CA0015, 0xffffffff},
    {0xf0034904, 0x00000000, 0xffffffff},
    {0xf0034908, 0x00CA0015, 0xffffffff},
    {0xf003490c, 0x04510450, 0xffffffff},
    {0xf0034910, 0x00000000, 0xffffffff},
    {0xf0034914, 0x04510450, 0xffffffff},
    {0xf0034918, 0x00000007, 0xffffffff},
    {0xf003491c, 0x80008001, 0xffffffff},

    //    TCO[9]  0xf0034960
//    {0xf0034960, 0x001500CA, 0xffffffff},
//    {0xf0034964, 0x00000000, 0xffffffff},
//    {0xf0034968, 0x001500CA, 0xffffffff},
//    {0xf003496c, 0x00010000, 0xffffffff},
//    {0xf0034970, 0x00000000, 0xffffffff},
//    {0xf0034974, 0x04500450, 0xffffffff},
//    {0xf0034978, 0x00000103, 0xffffffff},
//    {0xf003497c, 0x80008001, 0xffffffff}, 

    //VCOM_DYN  TCO[7]  0xf0034920
    {0xf0034920, 0x00010001, 0xffffffff},
    {0xf0034924, 0x00000000, 0xffffffff},
    {0xf0034928, 0x00010001, 0xffffffff},
    {0xf003492c, 0x02320001, 0xffffffff},
    {0xf0034930, 0x00000000, 0xffffffff},
    {0xf0034934, 0x00000000, 0xffffffff},
    {0xf0034938, 0x00000004, 0xffffffff},
    {0xf003493c, 0x00000000, 0xffffffff},

    {0xf0034820, 0x00000000, 0xffffffff}, //GPO :1
                                      
    {0xf0034800, 0x8100ffff, 0xffffffff},
    {0xf0034804, 0x00000000, 0xffffffff},
    {0xf0034814, 0x00008000, 0xffffffff}, //Bmux select
    {0xf0034818, 0x00000040, 0xffffffff}, //combination
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
  vIO32WriteFldAlign(GR8, 1, GR8_TCGPOEN_A);
	
  if (val)
  {
      vIO32WriteFldAlign(GR8, 1, GR8_TCGPO_A);
  }
  else
  {
      vIO32WriteFldAlign(GR8, 0, GR8_TCGPO_A);
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

	     u1count = (sizeof(sRegTg_PANEL_MLVDS_LGDV4FHD60) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
           u4tmp = sRegTg_PANEL_MLVDS_LGDV4FHD60[i].u4Addr;
	       u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegTg_PANEL_MLVDS_LGDV4FHD60[i].u4Msk)) | (sRegTg_PANEL_MLVDS_LGDV4FHD60[i].u4Val);

		   vIO32Write4B(u4tmp,u4tmp1);
         }

  	     LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegTg_PANEL_MLVDS_LGDV4FHD60), sizeof(struct sTCONREGTABLE));

         break;
		 
		 case PANEL_MLVDS_LGDV4FHD120: 
		 
		   LOG(1,"cust tg init\n");
		 
		   u1count = (sizeof(sRegTg_PANEL_MLVDS_LGDV4FHD60) / sizeof(struct sTCONREGTABLE));
		 
		   for ( i = 0 ; i < u1count ; i++)
		   {
			 u4tmp = sRegTg_PANEL_MLVDS_LGDV4FHD60[i].u4Addr;
			 u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegTg_PANEL_MLVDS_LGDV4FHD60[i].u4Msk)) | (sRegTg_PANEL_MLVDS_LGDV4FHD60[i].u4Val);
		 
			 vIO32Write4B(u4tmp,u4tmp1);
		   }
		 
		   LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegTg_PANEL_MLVDS_LGDV4FHD60), sizeof(struct sTCONREGTABLE));
		 
		   break;


	   case PANEL_MLVDS_LGDV6GIPFHD60: 

		 LOG(1,"cust tg init\n");

	     u1count = (sizeof(sRegTg_PANEL_MLVDS_LGDV6GIPFHD60) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
           u4tmp = sRegTg_PANEL_MLVDS_LGDV6GIPFHD60[i].u4Addr;
	       u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegTg_PANEL_MLVDS_LGDV6GIPFHD60[i].u4Msk)) | (sRegTg_PANEL_MLVDS_LGDV6GIPFHD60[i].u4Val);

		   vIO32Write4B(u4tmp,u4tmp1);
         }

  	     LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegTg_PANEL_MLVDS_LGDV6GIPFHD60), sizeof(struct sTCONREGTABLE));

         break;

		 
	   case PANEL_MLVDS_LGDV5GIPFHD60: 

		 LOG(1,"cust tg init\n");

	     u1count = (sizeof(sRegTg_PANEL_MLVDS_LGDV5GIPFHD60) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
           u4tmp = sRegTg_PANEL_MLVDS_LGDV5GIPFHD60[i].u4Addr;
	       u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegTg_PANEL_MLVDS_LGDV5GIPFHD60[i].u4Msk)) | (sRegTg_PANEL_MLVDS_LGDV5GIPFHD60[i].u4Val);

		   vIO32Write4B(u4tmp,u4tmp1);
         }

  	     LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegTg_PANEL_MLVDS_LGDV5GIPFHD60), sizeof(struct sTCONREGTABLE));

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
		 
       case PANEL_MLVDS_WXGA: 
	   	
         LOG(1,"cust tg init\n");

	     u1count = (sizeof(sRegTg_PANEL_MLVDS_WXGA) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
           u4tmp = sRegTg_PANEL_MLVDS_WXGA[i].u4Addr;
	       u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegTg_PANEL_MLVDS_WXGA[i].u4Msk)) | (sRegTg_PANEL_MLVDS_WXGA[i].u4Val);

		   vIO32Write4B(u4tmp,u4tmp1);
         }

  	     LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegTg_PANEL_MLVDS_WXGA), sizeof(struct sTCONREGTABLE));

         break;

         case PANEL_EPI_LGDV11GIPFHD120: 
         
           LOG(1,"cust tg init\n");
         
           u1count = (sizeof(sRegTg_EPI_LGDV11FHD120) / sizeof(struct sTCONREGTABLE));
         
           for ( i = 0 ; i < u1count ; i++)
           {
             u4tmp = sRegTg_EPI_LGDV11FHD120[i].u4Addr;
             u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegTg_EPI_LGDV11FHD120[i].u4Msk)) | (sRegTg_EPI_LGDV11FHD120[i].u4Val);
         
             vIO32Write4B(u4tmp,u4tmp1);
           }
         
           LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegTg_EPI_LGDV11FHD120), sizeof(struct sTCONREGTABLE));
         
           break;
		 
         case PANEL_EPI_LGDV12GIPFHD120: 
         
           LOG(1,"cust tg init\n");
         
           u1count = (sizeof(sRegTg_EPI_LGDV12FHD120) / sizeof(struct sTCONREGTABLE));
         
           for ( i = 0 ; i < u1count ; i++)
           {
             u4tmp = sRegTg_EPI_LGDV12FHD120[i].u4Addr;
             u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegTg_EPI_LGDV12FHD120[i].u4Msk)) | (sRegTg_EPI_LGDV12FHD120[i].u4Val);
         
             vIO32Write4B(u4tmp,u4tmp1);
           }
         
           LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegTg_EPI_LGDV12FHD120), sizeof(struct sTCONREGTABLE));
         
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

	     u1count = (sizeof(sRegMini_PANEL_MLVDS_LGDV4FHD60) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
          u4tmp = sRegMini_PANEL_MLVDS_LGDV4FHD60[i].u4Addr;
	      u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegMini_PANEL_MLVDS_LGDV4FHD60[i].u4Msk)) | (sRegMini_PANEL_MLVDS_LGDV4FHD60[i].u4Val);

		  vIO32Write4B(u4tmp,u4tmp1);
         }

       LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegMini_PANEL_MLVDS_LGDV4FHD60), sizeof(struct sTCONREGTABLE));

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
  
	 case PANEL_MLVDS_PANASONIC32WXGA8BIT:

	     LOG(1,"cust tg init\n");

	     u1count = (sizeof(sRegCustMlvds_PANASONIC32WXGA8BIT) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
          u4tmp = sRegCustMlvds_PANASONIC32WXGA8BIT[i].u4Addr;
	      u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegCustMlvds_PANASONIC32WXGA8BIT[i].u4Msk)) | (sRegCustMlvds_PANASONIC32WXGA8BIT[i].u4Val);

		  vIO32Write4B(u4tmp,u4tmp1);
         }

       LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegCustMlvds_PANASONIC32WXGA8BIT), sizeof(struct sTCONREGTABLE));

       break;	   
	   
	 case PANEL_MLVDS_WXGA:

	     LOG(1,"cust tg init\n");

	     u1count = (sizeof(sRegMini_PANEL_MLVDS_WXGA) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
          u4tmp = sRegMini_PANEL_MLVDS_WXGA[i].u4Addr;
	      u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegMini_PANEL_MLVDS_WXGA[i].u4Msk)) | (sRegMini_PANEL_MLVDS_WXGA[i].u4Val);

		  vIO32Write4B(u4tmp,u4tmp1);
         }

       LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegMini_PANEL_MLVDS_WXGA), sizeof(struct sTCONREGTABLE));

       break;
		}
  return(1);
}

UINT8 u1TconCustEPIInit(void)
{
 	UINT8 i = 0;
	UINT8 u1count;
	UINT32 u4tmp, u4tmp1;
	
	switch (GetCurrentPanelIndex())
	 {
	   
	 case PANEL_EPI_LGDV11GIPFHD120:

	     LOG(1,"cust tg init\n");

	     u1count = (sizeof(sReg_PANEL_EPI_LGDV11FHD120) / sizeof(struct sTCONREGTABLE));

	     for ( i = 0 ; i < u1count ; i++)
         {
          u4tmp = sReg_PANEL_EPI_LGDV11FHD120[i].u4Addr;
	      u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sReg_PANEL_EPI_LGDV11FHD120[i].u4Msk)) | (sReg_PANEL_EPI_LGDV11FHD120[i].u4Val);

		  vIO32Write4B(u4tmp,u4tmp1);
         }

       LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sReg_PANEL_EPI_LGDV11FHD120), sizeof(struct sTCONREGTABLE));

       case PANEL_EPI_LGDV12GIPFHD120:
       
           LOG(1,"cust tg init\n");
       
           u1count = (sizeof(sReg_PANEL_EPI_LGDV12FHD120) / sizeof(struct sTCONREGTABLE));
       
           for ( i = 0 ; i < u1count ; i++)
           {
            u4tmp = sReg_PANEL_EPI_LGDV12FHD120[i].u4Addr;
            u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sReg_PANEL_EPI_LGDV12FHD120[i].u4Msk)) | (sReg_PANEL_EPI_LGDV12FHD120[i].u4Val);
       
            vIO32Write4B(u4tmp,u4tmp1);
           }
       
         LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sReg_PANEL_EPI_LGDV12FHD120), sizeof(struct sTCONREGTABLE));

       break;
		}
  return(1);
}

