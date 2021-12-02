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
#ifndef _MTK_VGA_AUTOCOLOR_TABLE_H_
#define _MTK_VGA_AUTOCOLOR_TABLE_H_

INT8 code bEFuseCompensation[4][3] =    //notice if negative, how to express
{
    {0,  0,  0},    //YPbPr     ch1, ch2, ch3 3, 5, 11
    {0,  0,  0},    //VGA       ch1, ch2, ch3 2, 2, 4
    {0,  0, 0},    //SCART     ch1, ch2, ch3
    {0,  0, 0}     //VGA_YPBPR ch1, ch2, ch3
};

UINT8 code bVGADefaultGainTABLE_100[4][3] =
{
    //MT5395 2010.5.12
    {0x87, 0x8F, 0x94},	 //YPBPR    //2009.7.6 MT5388 ECO
    {0x4F, 0x4E, 0x51},  //RGB      //{0x4F, 0x52, 0x53},  //RGB
    {0x24, 0x23, 0x26},  //SCART RGB//{0x26, 0x28, 0x29},  //SCART RGB
    {0x2D, 0x26, 0x30}	  //VGA_YPBPR		
    
};

UINT16 code bVGADefaultGainTABLE_100_DIGITAL[4][3] =
{
    {0xC544, 0xCBCB, 0xCBCB},  //YPBPR    //2010.02.25 MT8223H/L 
    {0xAA29, 0xAA29, 0xAA29},  //RGB
    {0x9514, 0x9514, 0x9514},  //SCART RGB
    {0xC645, 0xC03F, 0xC746}   //VGA_YPBPR		
    
};
    
UINT16 code bVGADefaultOffsetTABLE_100_DIGITAL[4][3] =
{
    {0x13D, 0x17E, 0x182},	 //YPBPR
    {0x164, 0x16D, 0x171},  //RGB
    {0x136, 0x13B, 0x13E},  //SCART RGB
    {0x178, 0x149, 0x17A}  //VGA_YPBPR		
};



UINT8 code bVGADefaultOffsetTABLE_100[4][3] =
{   //MT5395
    {0x30, 0x6F, 0x73},	 //YPBPR
    {0x56, 0x51, 0x58},  //RGB
    {0x31, 0x30, 0x31},  //SCART RGB
    {0x6F, 0x30, 0x73}  //VGA_YPBPR	//{0x78, 0x49, 0x7A}  //VGA_YPBPR		
};


UINT8 code bVGADefaultGainTABLE_120[4][3] =
{
    //{0x5D, 0x77, 0x79},	 //YPBPR
    //{0x49, 0x4B, 0x4C},  //RGB
    //{0x21, 0x22, 0x23},  //SCART RGB
    {0x63, 0x7B, 0x7E},	 //YPBPR   //2009.7.6 MT5388 ECO
    {0x4F, 0x4E, 0x51},  //RGB      //{0x4F, 0x52, 0x53},  //RGB
    {0x24, 0x23, 0x26},  //SCART RGB//{0x26, 0x28, 0x29},  //SCART RGB
    {0x2D, 0x26, 0x30}	  //VGA_YPBPR		
};
UINT8 code bVGADefaultOffsetTABLE_120[4][3] =
{
    {0x3D, 0x7E, 0x82},	 //YPBPR
    {0x56, 0x51, 0x58},  //RGB
    {0x31, 0x30, 0x31},  //SCART RGB
    {0x6F, 0x30, 0x73}  //VGA_YPBPR	//{0x78, 0x49, 0x7A}  //VGA_YPBPR		
};

UINT8 code bVGADefaultGainTABLE_75_100[4][3] =
{
    //{0x21, 0x2A, 0x2B},	 //YPBPR
    //{0x49, 0x4B, 0x4C},  //RGB
    //{0x21, 0x22, 0x23},  //SCART RGB
    {0x28, 0x38, 0x38},	 //YPBPR   //2010.5.12 MT5395 
    {0x4F, 0x4E, 0x51},  //RGB      //{0x4F, 0x52, 0x53},  //RGB
    {0x24, 0x23, 0x26},  //SCART RGB//{0x26, 0x28, 0x29},  //SCART RGB
    {0x2D, 0x26, 0x30}	  //VGA_YPBPR		
};
UINT8 code bVGADefaultOffsetTABLE_75_100[4][3] =
{
    {0x3D, 0x7E, 0x82},	 //YPBPR
    {0x56, 0x51, 0x58},  //RGB
    {0x31, 0x30, 0x31},  //SCART RGB
    {0x6F, 0x30, 0x73}  //VGA_YPBPR	//{0x78, 0x49, 0x7A}  //VGA_YPBPR		
};

/*
UINT8 code bVGADefaultGainTABLE_SONY[4][3] =
{
    {0x88, 0x90, 0x93},	 //YPBPR   //2009.7.6 MT5388 ECO
    {0x3F, 0x42, 0x44},  //RGB
    {0x0D, 0x10, 0x11},  //SCART RGB
    {0x2A, 0x21, 0x2B}	  //VGA_YPBPR		
};
*/

/*	
UINT8 code bVGADefaultOffsetTABLE_SONY[4][3] =
{
    {0x3D, 0x7E, 0x82},	 //YPBPR
    {0x78, 0x70, 0x80},  //RGB
    {0x36, 0x3B, 0x3E},  //SCART RGB
    {0x7E, 0x3D, 0x82}  //VGA_YPBPR		
};
*/

UINT16 code wColorBlankValueNew[4][3] =
{
    {(0x10<<5), (0x80<<5), (0x80<<5)},	//YPBPR(5381)
    {(0x02<<5), (0x02<<5), (0x02<<5)},	//RGB 
    {(0x10<<5), (0x10<<5), (0x10<<5)},	//SCART RGB
    {(0x80<<5), (0x10<<5), (0x80<<5)}	 //VGA_YPBPR
};

//Table for 100% input signal
UINT8 code bColorMaxType1[4][3] =    //Type 1 is for YPbPr 100% signal 
{
    {236, 241, 241},	 //YPBPR
    {253, 253, 253},  //RGB
    {236, 236, 236},  //SCART RGB
    {241, 236, 241}	  //VGA_YPBPR		//add VGACOMP by Hua 20061116
};

//Table for 120% input signal
UINT8 code bColorMaxType2[4][3] =  //Type 2 is for YPbPr 120% signal 
{
    {216, 230, 230},	 // (236-16)*0.9+16=214, (241-128)*0.9+128=229.7
    {253, 253, 253},  //RGB
    {236, 236, 236},  //SCART RGB
    {241, 236, 241}	  //VGA_YPBPR		//add VGACOMP by Hua 20061116
};

//Table for SONY input signal
UINT8 code bColorMaxType3[4][3] =  //Type 3 is for SONY  
{
    {235, 240, 240},  //YPbPr	 
    {240, 240, 240},  //RGB
    {216, 216, 216},  //SCART RGB
    {241, 236, 241}   //VGA_YPBPR		
};

UINT16 code wOnChipColorMaxType[4][3] =
{
    {1809, 1851, 1851},	//YPBPR(5381) //(236-16)=220, (241-16)=225, (241-16)=225
    {2065, 2065, 2065}, //RGB         //(253-2)=251 , (253-2)=251,  (253-2)=251
    {1925, 1925, 1925}, //SCART RGB   //(236-2)=234 , (236-2)=234,  (236-2)=234
    {1851, 1809, 1851}	 //VGA_YPBPR		 //
    
  /*
    {1735, 1774, 1774},	//YPBPR(5381) //(236-16)=220, (241-16)=225, (241-16)=225
    {1987, 1987, 1987}, //VGA         //(254-2)=252 , (254-2)=252,  (254-2)=252
    {1845, 1845, 1845}, //SCART RGB   //(236-2)=234 , (236-2)=234,  (236-2)=234
    {1774, 1735, 1774}	 //VGA_YPBPR		 //
*/    
};

UINT16 code wOnChipColorGainTable[4][3]=
{
    {1463, 1503, 1503}, //YPBPR(5381) (236-16)/2*74.5/56, (241-128)*74.5/56, (241-128)*74.5/56, all elementa*10
    {1255, 1255, 1255}, //VGA (253-2)/2
    {1100, 1100, 1100}, //SCART RGB (236-16)/2
    {1130, 1100, 1130}  //VGA_YPBPR	(236-16)/2
};
UINT16 code wOnChipColorGainTable_75[4][3]=
{
    {1100, 1130, 1130}, //YPBPR(5381) (236-16)/2*74/56, (241-128)*74/56, (241-128)*74/56, all elementa*10
    {1255, 1255, 1255}, //VGA (253-2)/2
    {1100, 1100, 1100}, //SCART RGB (236-16)/2
    {1130, 1100, 1130}  //VGA_YPBPR	(236-16)/2
};

UINT16 code wYPbPrMappingVgaGainTable[4][3]=
{
     //{110, 112, 112}, //YPBPR(5381) //ÂÂTable
    //{126, 126, 126}, //VGA
    //{118, 118, 118}, //SCART RGB
    //{112, 110, 112} //VGA_YPBPR	
    {1470, 1490, 1490}, //YPBPR(5381) (236-16)/2*74/56, (241-128)*74/56, (241-128)*74/56, all elementa*10
    {1255, 1255, 1255}, //VGA (253-2)/2
    {1100, 1100, 1100}, //SCART RGB (236-16)/2
    {1255, 1255, 1255}  //VGA_YPBPR	(253-2)/2
};

/*
UINT16 code wOnChipColorGainTable_SONY[4][3]=
{
    {1460, 1480, 1480}, //YPBPR (235-16)/2*74/56+2, (240-128)*74/56, (240-128)*74/56, all elements*10
    {1190, 1190, 1190}, //VGA (240-2)/2
    {1100, 1100, 1100}, //SCART RGB (236-16)/2
    {1255, 1255, 1255}  //VGA_YPBPR	(240-2)/2 //kalcheng
};
*/
UINT8 code bOnChipCalibrateTolerance[5][4]=
{
    {0x04, 0x04, 0x30, 0x30},	//mode=0,  gain calibration by using on chip voltage
    {0x0c, 0x0c, 0x40, 0x40},      //mode=1,  offset calibration by using on chip voltage
    {0x0c, 0x0c, 0x40, 0x40},      //mode=2,   offset calibration  0x20=1 code (8bit)
    {0x00, 0x00, 0x04, 0x04},      //mode=3,  gain calibration for external signal
    {0x00, 0x00, 0x04, 0x04},      //mode=4,  digital offset calibration for external signal
};

UINT8 code bOnChipCheckTolerance[5][2]=
{
    {0x04, 0x04},	//mode=0,    gain calibration by using on chip voltage
    {0x10, 0x10},   //mode=1,    offset calibration by using on chip voltage
    {0x10, 0x10},   //mode=2,    offset calibration  0x20=1 code (8bit)
    {0x00, 0x00},   //mode=3,    gain calibration for external signal
    {0x00, 0x00},   //mode=4,    digital offset calibration for external signal
};

// support 100% with 18/56 ohm
UINT8 code GAIN_HIGH_LIMIT_100[4][3]=    
{ 
  {0xC6, 0xD7, 0xD6},  //YPbPr
  {0x81, 0x85, 0x84},  //VGA         //{0x70, 0x73, 0x74},  //VGA
  {0x52, 0x55, 0x54},  //SCART R,G,B //{0x43, 0x45, 0x46},  //SCART R,G,B
  {0x45, 0x3E, 0x49}   // VGA_YPBPR
 };
UINT8 code GAIN_LOW_LIMIT_100[4][3] =    
{ 
   {0x55, 0x62, 0x61}, //YPbPr              CH1 : 0x85 > X > 0X48,  CH2: 0xA1 > X > 0X60 , CH3: 0xA1 > X > 0X60
   {0x28, 0x2B, 0x2A}, //VGA //{0x31, 0x24, 0x35}, //VGA              CH1 : 0x73 > X > 0x36,  CH2: 0x74 > X > 0x36   ,CH3: 0x74 > X > 0x36
   {0x04, 0x07, 0x06}, //SCART R,G,B//{0x0C, 0x0E, 0x0F},    CH1: 0x44> X > 0x0F,  CH2: 0x44 > X > 0x0F,  CH3: 0x44 > X > 0x0F   
   {0x16, 0x10, 0x19}  // VGA_YPBPR
};
//DIGITAL_NEW_GAIN
UINT16 code GAIN_HIGH_LIMIT_100_DIGITAL[4][3]=    
{ 
  {0xD958, 0xE0E0, 0xE1E1},  //YPbPr
  {0xB837, 0xB9B9, 0xBA39},  //VGA
  {0xA1A1, 0xA2A2, 0xA322},  //SCART R,G,B
  {0xDBDB, 0xD5D5, 0xDBDB}   // VGA_YPBPR
 };
UINT16 code GAIN_LOW_LIMIT_100_DIGITAL[4][3] =    
{ 
//tempary setting
   {0x8000, 0x8000, 0x8000}, //YPbPr              CH1 : 0x85 > X > 0X48,  CH2: 0xA1 > X > 0X60 , CH3: 0xA1 > X > 0X60
   {0x7000, 0x7000, 0x7000}, //VGA               CH1 : 0x73 > X > 0x36,  CH2: 0x74 > X > 0x36   ,CH3: 0x74 > X > 0x36
   {0x6000, 0x6000, 0x6000}, //SCART R,G,B    CH1: 0x44> X > 0x0F,  CH2: 0x44 > X > 0x0F,  CH3: 0x44 > X > 0x0F   
   {0x8000, 0x8000, 0x8000}  // VGA_YPBPR

//   {0xB02F, 0xB635, 0xB6B6}, //YPbPr              CH1 : 0x85 > X > 0X48,  CH2: 0xA1 > X > 0X60 , CH3: 0xA1 > X > 0X60
//   {0x9898, 0x9A19, 0x9A9A}, //VGA               CH1 : 0x73 > X > 0x36,  CH2: 0x74 > X > 0x36   ,CH3: 0x74 > X > 0x36
//   {0x8605, 0x8706, 0x8787}, //SCART R,G,B    CH1: 0x44> X > 0x0F,  CH2: 0x44 > X > 0x0F,  CH3: 0x44 > X > 0x0F   
//   {0xB1B1, 0xABAB, 0xB231}  // VGA_YPBPR
};

// support 120% with 18/56 ohm
UINT8 code GAIN_HIGH_LIMIT_120[4][3]=    
{ 
  {0x9D, 0xBE, 0xBD},  //YPbPr
  {0x81, 0x85, 0x84},  //VGA
  {0x52, 0x55, 0x54},  //SCART R,G,B
  {0x45, 0x3E, 0x49}   // VGA_YPBPR
};

UINT8 code GAIN_LOW_LIMIT_120[4][3] =    
{ 
   {0x36, 0x50, 0x45}, //YPbPr              CH1 : 0x85 > X > 0X48,  CH2: 0xA1 > X > 0X60 , CH3: 0xA1 > X > 0X60
   {0x28, 0x2B, 0x2A}, //VGA               CH1 : 0x73 > X > 0x36,  CH2: 0x74 > X > 0x36   ,CH3: 0x74 > X > 0x36
   {0x04, 0x07, 0x06}, //SCART R,G,B    CH1: 0x44> X > 0x0F,  CH2: 0x44 > X > 0x0F,  CH3: 0x44 > X > 0x0F   
   {0x16, 0x10, 0x19}  // VGA_YPBPR
};

UINT8 code GAIN_HIGH_LIMIT_75_100[4][3]=    
{ 
  {0x52, 0x5F, 0x5E},  //YPbPr
  {0x81, 0x85, 0x84},  //VGA
  {0x52, 0x55, 0x54},  //SCART R,G,B
  {0x45, 0x3E, 0x49}   // VGA_YPBPR
};

UINT8 code GAIN_LOW_LIMIT_75_100[4][3] =    
{ 
   {0x04, 0x0E, 0x0D}, //YPbPr             
   {0x28, 0x2B, 0x2A}, //VGA               CH1 : 0x73 > X > 0x36,  CH2: 0x74 > X > 0x36   ,CH3: 0x74 > X > 0x36
   {0x04, 0x07, 0x06}, //SCART R,G,B    CH1: 0x44> X > 0x0F,  CH2: 0x44 > X > 0x0F,  CH3: 0x44 > X > 0x0F   
   {0x16, 0x10, 0x19}  // VGA_YPBPR
};
/*
UINT8 code GAIN_HIGH_LIMIT_SONY[4] [3]=    
{ 
  {0xC0, 0xC0, 0xC0},  //YPbPr
  {0x60, 0x64, 0x65},  //VGA
  {0x33, 0x35, 0x36},  //SCART R,G,B
  {0xB7, 0xAB, 0xB7}   // VGA_YPBPR
};
*/
/*
UINT8 code GAIN_LOW_LIMIT_SONY[4][3] =    
{ 
  {0x60, 0x6C, 0x6D}, //YPbPr            
  {0x20, 0x20, 0x20}, //VGA             
  {0x02, 0x02, 0x02}, //SCART R,G,B   
  {0x63, 0x57, 0x64}  // VGA_YPBPR
};
*/

#endif
