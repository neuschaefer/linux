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
  * $RCSfile: panel_table_cust.c,v $
  * $Revision: #1 $
  * $Date: 2012/04/27 $
  * $Author: dtvbm11 $
  * Description:
  *
  *---------------------------------------------------------------------------*/
#include "sv_const.h"
#include "feature.h"
#include "drv_tcon.h"
#include "drv_ldmspi.h"


typedef struct
{
    //--- Index Info -----------------------
    UINT8 u1PanelId;
    UINT8 au1PanelName[16];
    UINT8 u1InchSize;
}__attribute__ ((packed)) PANEL_HEADER_CUSTOM_ATTRIBUTE_T;

typedef struct
{
    UINT8 u1LUTSel;
    UINT32 u4Dither;
    UINT8 u1NVMType;

    UINT8 au1PQIndex[4];
    UINT8 au1AQIndex[4];

    UINT8 au1BLExtData[8];
    UINT8 au1DimExtData[3];
}__attribute__ ((packed)) PANEL_Ext2_CUSTOM_ATTRIBUTE_T;

typedef struct
{
    //--- Index Info -----------------------
    PANEL_HEADER_CUSTOM_ATTRIBUTE_T rHeader;
    //--- Basic Info -----------------------
    #if 0
    UINT16 u2PanelWidth;
    UINT16 u2PanelHeight;

    // Clk60Hz = htotal*vtotal*60/1.001
    // Clk50Hz = htotal*vtotal*50
    UINT32 u4PixelClkMax;
    UINT32 u4PixelClk60Hz;
    UINT32 u4PixelClk50Hz;
    UINT32 u4PixelClkMin;

    // HTotal = spec value - 1
    UINT16 u2HTotalMax;
    UINT16 u2HTotal60Hz;
    UINT16 u2HTotal50Hz;
    UINT16 u2HTotalMin;

    // VTotal = spec value - 1
    UINT16 u2VTotalMax;
    UINT16 u2VTotal60Hz;
    UINT16 u2VTotal50Hz;
    UINT16 u2VTotalMin;
    UINT8 u1VClkMax;
    UINT8 u1VClkMin;

    UINT8 u1HSyncWidth;
    UINT8 u1VSyncWidth;
    UINT16 u2HPosition;
    UINT16 u2VPosition;
    UINT32 u4ControlWord;

    // backlight ragne
    UINT8 u1BacklightHigh;
    UINT8 u1BacklightMiddle;
    UINT8 u1BacklightLow;
    UINT8 u1Reserved;

    UINT16 u2DimmingFrequency60Hz;
    UINT16 u2DimmingFrequency50Hz;

    // delay unit: 10ms
    UINT8 u1LvdsOnDalay;
    UINT8 u1BacklightOnDelay;
    UINT8 u1BacklightOffDelay;
    UINT8 u1LvdsOffDalay;

    UINT16 u2MaxOverscan;
    UINT16 u2Reserved;
    #endif
    PANEL_ATTRIBUTE_T rBasicInfo;
    //--- Extra Info -----------------------
    UINT32 u4PixelClk48Hz;
    UINT16 u2HTotal48Hz;
    UINT16 u2VTotal48Hz;

    UINT8 au1HSyncWidth[3];//0:    SV_DCLK_48HZ, 1: SV_DCLK_50HZ, 2:SV_DCLK_60HZ
    UINT8 au1VSyncWidth[3];//0:    SV_DCLK_48HZ, 1: SV_DCLK_50HZ, 2:SV_DCLK_60HZ
    UINT16 au2HFrontPorch[3];//0:    SV_DCLK_48HZ, 1: SV_DCLK_50HZ, 2:SV_DCLK_60HZ
    UINT16 au2VbackPorch[3];//0:    SV_DCLK_48HZ, 1: SV_DCLK_50HZ, 2:SV_DCLK_60HZ

    UINT8 u1HSyncPolarity;
    UINT8 u1VSyncPolarity;

    UINT8 u1HVSyncAlignment;
    UINT16 u2DimmingFrequency48Hz;

    //--- Reserved data -----------------------
    UINT8 au1Reserved[33];
    //--- Extra2 Info -----------------------
    UINT8 u1SSPermillage;
    UINT32 u4SSFreq;

    PANEL_Ext2_CUSTOM_ATTRIBUTE_T   rExt2Info;

    UINT8 au1Pedding[2];
}__attribute__ ((packed)) PANEL_CUSTOM_FLASH_ATTRIBUTE_T;

 //#ifdef SUPPORT_PANEL_48HZ

//#define EXT_PANEL_BEGIN_INDEX PANEL_SONY_X_GY_1_WXGA

typedef struct
{
    UINT32 u4PanelIndex ;
    #ifdef SUPPORT_PANEL_48HZ
    UINT32 u4PixelClk48Hz;
    UINT16 u2HTotal48Hz;
    UINT16 u2VTotal48Hz;
    UINT16 u2DimmingFrequency48Hz;
    #endif

    #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
    UINT8 u1HSyncWidth[3];//0:    SV_DCLK_48HZ, 1: SV_DCLK_50HZ, 2:SV_DCLK_60HZ
    UINT8 u1VSyncWidth[3];//0:    SV_DCLK_48HZ, 1: SV_DCLK_50HZ, 2:SV_DCLK_60HZ
    UINT16 u2HFrontPorch[3];//0:    SV_DCLK_48HZ, 1: SV_DCLK_50HZ, 2:SV_DCLK_60HZ
    UINT16 u2VbackPorch[3];//0:    SV_DCLK_48HZ, 1: SV_DCLK_50HZ, 2:SV_DCLK_60HZ
    UINT8 u1HSyncPolarity;
    UINT8 u1VSyncPolarity;
    #endif

    #ifdef SUPPORT_PANEL_DITHER
    UINT32 u4Dither;
    #endif
    #ifdef SUPPORT_PANEL_DRIVING
    UINT8 u1DrivingCurrent;
    #endif

    #ifdef SUPPORT_PANEL_ERRORLIMIT_TABLE
    UINT32 u4ErrorLimit;
    #endif

    #ifdef SUPPORT_PANEL_3D_PIXEL_SIZE
    UINT16 u2PixelSize;
    #endif

    #ifdef SUPPORT_PANEL_3D_SG_CTRL
    UINT16 u2VDelay60Hz;
    UINT16 u2HDelay60Hz;
    UINT16 u2HDutyCycle60Hz;
    UINT16 u2VDelay50Hz;
    UINT16 u2HDelay50Hz;
    UINT16 u2HDutyCycle50Hz;
    #ifdef SUPPORT_PANEL_48HZ
    UINT16 u2VDelay48Hz;
    UINT16 u2HDelay48Hz;
    UINT16 u2HDutyCycle48Hz;
	#endif
    #endif

    #ifdef SUPPORT_PANEL_SS
    UINT8 u1SSPermillage;
    UINT32 u4SSFreq;
    #endif
} PANEL_ATTRIBUTE_EXT_T;


#if defined(SUPPORT_PANEL_48HZ) || defined(SUPPORT_PANEL_CUSTOMER_SPEC) || defined(SUPPORT_PANEL_SS) || defined(SUPPORT_PANEL_DITHER) || defined(SUPPORT_PANEL_DRIVING)|| defined(SUPPORT_PANEL_ERRORLIMIT_TABLE)|| defined(SUPPORT_PANEL_3D_PIXEL_SIZE) || defined(SUPPORT_PANEL_3D_SG_CTRL)
static PANEL_ATTRIBUTE_EXT_T _arPanelAttributeExt[]=
{
    #if ((PANEL_SELECT == PANEL_SS_40_FHD) || CC_MULTI_PANEL)
    {
        PANEL_SS_40_FHD,
        #ifdef SUPPORT_PANEL_48HZ
        79700000,                  //u4PixelClk48Hz
        1723,                      // u2HTotal48Hz
        962,                    // u2VTotal48Hz
        150,
        #endif
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        {32, 32, 32},              //u1HSyncWidth[3]; 5bits, 1 represents 1 pixel in 5387
        {4, 4, 4},                 // u1VSyncWidth[3]; 4bits, 1 represents 1 line
        {276, 276, 92},         // H front porch, 1 represents 1 pixel
        {19, 19, 19},//{172, 134, 72}          // V back porch, 1represents 1 line
        HSYNC_LOW,
        VSYNC_LOW,
        #endif
        #ifdef SUPPORT_PANEL_DITHER
        0xa000,
        #endif
        #ifdef SUPPORT_PANEL_DRIVING
        0x3,
        #endif
	    #ifdef SUPPORT_PANEL_3D_PIXEL_SIZE
	    320, // Pixel Size = 0.320 mm = 320 um
	    #endif
	    #ifdef SUPPORT_PANEL_3D_SG_CTRL
	    0, // u2VDelay60Hz;
	    0, // u2HDelay60Hz;
	    50, // u2HDutyCycle60Hz;
	    0, // u2VDelay50Hz;
	    0, // u2HDelay50Hz;
	    50, // u2HDutyCycle50Hz;
        #ifdef SUPPORT_PANEL_48HZ
	    0, // u2VDelay48Hz;
	    0, // u2HDelay48Hz;
	    50, // u2HDutyCycle48Hz;
	    #endif
	    #endif
        #ifdef SUPPORT_PANEL_SS
        20,
        0
        #endif
    },
    #endif
    #if ((PANEL_SELECT == PANEL_SS_40_FHD_10BIT) || CC_MULTI_PANEL)
    {
		PANEL_SS_40_FHD_10BIT,
        #ifdef SUPPORT_PANEL_48HZ
        148500000,                //u4PixelClk48Hz
        2184,                      // u2HTotal48Hz
        1416,                    // u2VTotal48Hz
        150,
        #endif
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        {32, 32, 32},              //u1HSyncWidth[3]; 5bits, 1 represents 1 pixel in 5387
        {4, 4, 4},                // u1VSyncWidth[3]= VSyn, 4bits, 1 represents 1 line,
        {136, 136, 136},            // H front porch, 1 represents 1 pixel
        {16, 16, 16},//{316, 260, 34}            // V back porch, 1represents 1 line
        HSYNC_LOW,
        VSYNC_LOW,
        #endif
        #ifdef SUPPORT_PANEL_DITHER
        0xa000,
        #endif
        #ifdef SUPPORT_PANEL_DRIVING
        0x3,
        #endif
        #ifdef SUPPORT_PANEL_ERRORLIMIT_TABLE
        0x1F,
        #endif
	    #ifdef SUPPORT_PANEL_3D_PIXEL_SIZE
	    320, // Pixel Size = 0.320 mm = 320 um
	    #endif
	    #ifdef SUPPORT_PANEL_3D_SG_CTRL
	    0, // u2VDelay60Hz;
	    0, // u2HDelay60Hz;
	    50, // u2HDutyCycle60Hz;
	    0, // u2VDelay50Hz;
	    0, // u2HDelay50Hz;
	    50, // u2HDutyCycle50Hz;
        #ifdef SUPPORT_PANEL_48HZ
	    0, // u2VDelay48Hz;
	    0, // u2HDelay48Hz;
	    50, // u2HDutyCycle48Hz;
	    #endif
	    #endif
        #ifdef SUPPORT_PANEL_SS
        20,
        0
        #endif
    },
    #endif
    #if ((PANEL_SELECT == PANEL_SS_L40V8200_120SG) || CC_MULTI_PANEL)
    {
		PANEL_SS_40_FHD_10BIT,
        #ifdef SUPPORT_PANEL_48HZ
        148500000,                //u4PixelClk48Hz
        2184,                      // u2HTotal48Hz
        1416,                    // u2VTotal48Hz
        150,
        #endif
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        {32, 32, 32},              //u1HSyncWidth[3]; 5bits, 1 represents 1 pixel in 5387
        {4, 4, 4},                // u1VSyncWidth[3]= VSyn, 4bits, 1 represents 1 line,
        {136, 136, 136},            // H front porch, 1 represents 1 pixel
        {16, 16, 16},//{316, 260, 34}            // V back porch, 1represents 1 line
        HSYNC_LOW,
        VSYNC_LOW,
        #endif
        #ifdef SUPPORT_PANEL_DITHER
        0xa000,
        #endif
        #ifdef SUPPORT_PANEL_DRIVING
        0x3,
        #endif
        #ifdef SUPPORT_PANEL_ERRORLIMIT_TABLE
        0x1F,
        #endif
	    #ifdef SUPPORT_PANEL_3D_PIXEL_SIZE
	    320, // Pixel Size = 0.320 mm = 320 um
	    #endif
	    #ifdef SUPPORT_PANEL_3D_SG_CTRL
	    3, // u2VDelay60Hz;
	    675, // u2HDelay60Hz;
	    50, // u2HDutyCycle60Hz;
	    3, // u2VDelay50Hz;
	    675, // u2HDelay50Hz;
	    50, // u2HDutyCycle50Hz;
        #ifdef SUPPORT_PANEL_48HZ
	    3, // u2VDelay48Hz;
	    540, // u2HDelay48Hz;
	    50, // u2HDutyCycle48Hz;
	    #endif
	    #endif
        #ifdef SUPPORT_PANEL_SS
        20,
        0
        #endif
    },
    #endif // PANEL_SS_L40V8200_120SG
    #if ((PANEL_SELECT == PANEL_CMI_V420H2_LS1) || CC_MULTI_PANEL)
    {
		PANEL_SS_40_FHD_10BIT,
        #ifdef SUPPORT_PANEL_48HZ
        148500000,                //u4PixelClk48Hz
        2184,                      // u2HTotal48Hz
        1416,                    // u2VTotal48Hz
        150,
        #endif
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        {32, 32, 32},              //u1HSyncWidth[3]; 5bits, 1 represents 1 pixel in 5387
        {4, 4, 4},                // u1VSyncWidth[3]= VSyn, 4bits, 1 represents 1 line,
        {136, 136, 136},            // H front porch, 1 represents 1 pixel
        {16, 16, 16},//{316, 260, 34}            // V back porch, 1represents 1 line
        HSYNC_LOW,
        VSYNC_LOW,
        #endif
        #ifdef SUPPORT_PANEL_DITHER
        0xa000,
        #endif
        #ifdef SUPPORT_PANEL_DRIVING
        0x3,
        #endif
        #ifdef SUPPORT_PANEL_ERRORLIMIT_TABLE
        0x1F,
        #endif
	    #ifdef SUPPORT_PANEL_3D_PIXEL_SIZE
	    320, // Pixel Size = 0.320 mm = 320 um
	    #endif
	    #ifdef SUPPORT_PANEL_3D_SG_CTRL
	    0, // u2VDelay60Hz;
	    0, // u2HDelay60Hz;
	    50, // u2HDutyCycle60Hz;
	    0, // u2VDelay50Hz;
	    0, // u2HDelay50Hz;
	    50, // u2HDutyCycle50Hz;
        #ifdef SUPPORT_PANEL_48HZ
	    0, // u2VDelay48Hz;
	    0, // u2HDelay48Hz;
	    50, // u2HDutyCycle48Hz;
	    #endif
	    #endif
        #ifdef SUPPORT_PANEL_SS
        20,
        0
        #endif
    },
    #endif //PANEL_CMI_V420H2_LS1
#if ((PANEL_SELECT == PANEL_PDP_PD5L_XGA60) || CC_MULTI_PANEL) 
    {
		PANEL_PDP_PD5L_XGA60,
	#ifdef SUPPORT_PANEL_48HZ
        80961216,                //u4PixelClk48Hz
        2042,                      // u2HTotal48Hz
        826,                    // u2VTotal48Hz
        150,
	#endif	  
	#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        {16, 16, 16},              //u1HSyncWidth[3]; 5bits, 1 represents 1 pixel in 5387
        {2, 2, 2},                // u1VSyncWidth[3]= VSyn, 4bits, 1 represents 1 line,
        {970, 888, 564},            // H front porch, 1 represents 1 pixel
        {2, 2, 2},//{316, 260, 34}            // V back porch, 1represents 1 line
        HSYNC_HIGH,
        VSYNC_HIGH,
	#endif	  
	#ifdef SUPPORT_PANEL_DITHER
        0xa000,
	#endif
	#ifdef SUPPORT_PANEL_DRIVING
        0x3,
	#endif
	#ifdef SUPPORT_PANEL_ERRORLIMIT_TABLE	 
        0x1F,
	#endif			
	#ifdef SUPPORT_PANEL_3D_PIXEL_SIZE
	    320, // Pixel Size = 0.320 mm = 320 um
	#endif
	    #ifdef SUPPORT_PANEL_3D_SG_CTRL
	    3, // u2VDelay60Hz;
	    675, // u2HDelay60Hz;
	    50, // u2HDutyCycle60Hz;
	    3, // u2VDelay50Hz;
	    675, // u2HDelay50Hz;
	    50, // u2HDutyCycle50Hz;
        #ifdef SUPPORT_PANEL_48HZ
	    3, // u2VDelay48Hz;
	    540, // u2HDelay48Hz;
	    50, // u2HDutyCycle48Hz;
	    #endif
	    #endif
	#ifdef SUPPORT_PANEL_SS
        0,
        0
	#endif
    },
#endif
};
#endif //defined(SUPPORT_PANEL_48HZ) || defined(SUPPORT_PANEL_CUSTOMER_SPEC)


// NOTE! the configuration order must follow panel index order
static PANEL_ATTRIBUTE_T _arPanelAttribute[] =
{
#if ((PANEL_SELECT == PANEL_XGA) || CC_MULTI_PANEL) // refer to CLAA150XP03
    {
        1024,           // u2PanelWidth
        768,            // u2PanelHeight

        74000000,       // u4PixelClkMax
        64930909,       // u4PixelClk60Hz
        54163200,       // u4PixelClk50Hz
        44000000,       // u4PixelClkMin

        1522,           // u2HTotalMax
        1344,           // u2HTotal60Hz
        1344,           // u2HTotal50Hz
        1214,           // u2HTotalMin

        888,            // u2VTotalMax
        806,            // u2VTotal60Hz
        806,            // u2VTotal50Hz
        789,            // u2VTotalMin
        62,             // u1VClkMax
        48,             // u1VClkMin

        0x87,         // u1HSyncWidth
        5,            // u1VSyncWidth
        0x818,        // u1HPosition
        0x3E1,        // u1VPosition
        SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
        PWM_HIGH_PANEL_BRIGHT,

        0xFF,           // (100%) u1BacklightHigh
        0x68,           // (84%) u1BacklightMiddle
        0x00,           // (68%) u1BacklightLow
        0,              // u1Reserved
        0,              // u2DimmingFrequency60Hz
        0,              // u2DimmingFrequency50Hz

        7,              // u1LvdsOnDalay
        80,             // u1BacklightOnDelay
        21,             // u1BacklightOffDelay
        6,              // u1LvdsOffDalay

        50,             // u2MaxOverscan
        0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SXGA) || CC_MULTI_PANEL) // no spec, not recommed
    {
    1280,           // u2PanelWidth
    1024,           // u2PanelHeight

    120000000,      // u4PixelClkMax
    107856610,      // u4PixelClk60Hz
    89970400,       // u4PixelClk50Hz
    88000000,       // u4PixelClkMin

    1801,           // u2HTotalMax
    1688,           // u2HTotal60Hz
    1688,           // u2HTotal50Hz
    1150,           // u2HTotalMin

    1101,           // u2VTotalMax
    1066,           // u2VTotal60Hz
    1066,           // u2VTotal50Hz
    901,            // u2VTotalMin
    75,             // u1VClkMax
    55,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    5,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    5,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_19_A1) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    900,            // u2PanelHeight

    2 * 56000000,   // u4PixelClkMax
    88807193,       // u4PixelClk60Hz
    88807193,       // u4PixelClk50Hz
    2 * 36489600,   // u4PixelClkMin

    1920,           // u2HTotalMax
    1600,           // u2HTotal60Hz
    1920,           // u2HTotal50Hz
    1500,           // u2HTotalMin

    1050,           // u2VTotalMax
    926,            // u2VTotal60Hz
    926,            // u2VTotal50Hz
    905,            // u2VTotalMin
    75,             // u1VClkMax
    56,             // u1VClkMin

    30,              // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_NS | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh
    0x70,           // (77%) u1BacklightMiddle
    0x00,           // (55%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    45,             // u1BacklightOnDelay
    9,              // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_27_W1) || CC_MULTI_PANEL)
    {
    1280,           // u2PanelWidth
    720,            // u2PanelHeight

    79500000,       // u4PixelClkMax
    74175815,       // u4PixelClk60Hz
    72000000,       // u4PixelClk50Hz
    70500000,       // u4PixelClkMin

    2000,           // u2HTotalMax
    1650,           // u2HTotal60Hz
    1800,           // u2HTotal50Hz
    1450,           // u2HTotalMin

    850,            // u2VTotalMax
    750,            // u2VTotal60Hz
    800,            // u2VTotal50Hz
    730,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,           // u1VPosition
    //245,          // u1HPosition
    //126,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    45,             // u1BacklightOnDelay
    10,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_27_B1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    85678910,       // u4PixelClk60Hz
    71470500,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1798,           // u2HTotal60Hz
    1798,           // u2HTotal50Hz
    1436,           // u2HTotalMin

    888,            // u2VTotalMax
    795,            // u2VTotal60Hz
    795,            // u2VTotal50Hz
    770,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT, // PWM_HIGH_PANEL_BRIGHT not verified

    0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    50,             // u1BacklightOnDelay
    10,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_32_B1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    85678921,       // u4PixelClk60Hz
    75850000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1798,           // u2HTotal60Hz
    1850,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    888,            // u2VTotalMax
    795,            // u2VTotal60Hz
    820,            // u2VTotal50Hz
    770,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xff,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_37_H1) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 80000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 60000000,   // u4PixelClkMin

    2300,           // u2HTotalMax
    2250,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2100,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1115,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT, // PWM_HIGH_PANEL_BRIGHT not verified

    0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    10,             // u1BacklightOnDelay
    10,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_42_H1) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 80000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 60000000,   // u4PixelClkMin

    2300,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2100,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1115,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_ON |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    50,             // u1BacklightOnDelay
    10,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_20_XW2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    66898000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1660,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1063,           // u2VTotalMax,
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh
    0x59,           // (67%) u1BacklightMiddle
    0xB3,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    70,             // u1BacklightOnDelay
    2,              // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_32_XW1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    888,            // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0x00,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    7,              // u1LvdsOnDalay
    80,             // u1BacklightOnDelay
    21,             // u1BacklightOffDelay
    6,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_37_HW1) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 85000000,   // u4PixelClkMax
    149010989,      // u4PixelClk60Hz
    124300000,      // u4PixelClk50Hz
    2 * 65280000,   // u4PixelClkMin

    2360,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2000,           // u2HTotalMin

    1200,           // u2VTotalMax
    1130,           // u2VTotal60Hz
    1130,           // u2VTotal50Hz
    1088,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_LRLR,
#else
	DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
#endif

    0x00,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0xFF,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    50,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_26_W1) || CC_MULTI_PANEL)
    {
    1280,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    80012787,       // u4PixelClk60Hz
    66744000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1848,           // u2HTotalMax
    1648,           // u2HTotal60Hz
    1648,           // u2HTotal50Hz
    1328,           // u2HTotalMin

    845,            // u2VTotalMax
    810,            // u2VTotal60Hz
    810,            // u2VTotal50Hz
    775,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    6,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_32_W1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    72354836,       // u4PixelClk60Hz
    68530000,       // u4PixelClk50Hz
    68000000,       // u4PixelClkMin

    1776,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1540,           // u2HTotal50Hz
    1416,           // u2HTotalMin

    1063,           // u2VTotalMax
    790,            // u2VTotal60Hz
    890,            // u2VTotal50Hz
    775,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // (100%) u1BacklightHigh
    0xAB,           // (67%) u1BacklightMiddle
    0xE6,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    40,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_37_WX1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    72354836,       // u4PixelClk60Hz
    68530000,       // u4PixelClk50Hz
    68000000,       // u4PixelClkMin

    1776,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1540,           // u2HTotal50Hz
    1472,           // u2HTotalMin

    1063,           // u2VTotalMax
    790 ,            // u2VTotal60Hz
    890 ,            // u2VTotal50Hz
    776 ,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x00,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0xFF,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    4,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_37_WU1) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 70000000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,
    0x23, // (100%) u1BacklightHigh     // u1BacklightHigh
    0x7D, // (67%) u1BacklightMiddle    // u1BacklightMiddle
    0xC5, // (50%) u1BacklightLow       // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_42_W2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    76300000,       // u4PixelClkMax
    72354845,       // u4PixelClk60Hz
    72580000,       // u4PixelClk50Hz
    68100000,       // u4PixelClkMin

    1776,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1540,           // u2HTotal50Hz
    1456,           // u2HTotalMin

    1063,           // u2VTotalMax
    790,            // u2VTotal60Hz
    950,            // u2VTotal50Hz
    776,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,
    0x23, // (100%) u1BacklightHigh     // u1BacklightHigh
    0x7D, // (67%) u1BacklightMiddle    // u1BacklightMiddle
    0xC5, // (50%) u1BacklightLow       // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    0,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_40_WT) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    80367632,       // u4PixelClk60Hz
    67040000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    2040,           // u2HTotalMax
    1600,           // u2HTotal60Hz
    1600,           // u2HTotal50Hz
    1450,           // u2HTotalMin

    1500,           // u2VTotalMax
    838,            // u2VTotal60Hz
    838,            // u2VTotal50Hz
    773,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x00,           // u1BacklightHigh
    0x68,           // u1BacklightMiddle
    0xFF,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    110,            // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    5,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CLAA_15_XP3) || CC_MULTI_PANEL)
    {
    1024,           // u2PanelWidth
    768,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    64930901,       // u4PixelClk60Hz
    54163200,       // u4PixelClk50Hz
    50000000,       // u4PixelClkMin

    1370,           // u2HTotalMax
    1344,           // u2HTotal60Hz
    1344,           // u2HTotal50Hz
    1150,           // u2HTotalMin

    860,            // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    794,            // u2VTotalMin
    75,             // u1VClkMax
    55,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT, // PWM_LOW_PANEL_BRIGHT not verified

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_QD_32_HL1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    80012787,       // u4PixelClk60Hz
    74984000,       // u4PixelClk50Hz
    70000000,       // u4PixelClkMin

    1780,           // u2HTotalMax
    1648,           // u2HTotal60Hz
    1648,           // u2HTotal50Hz
    1512,           // u2HTotalMin

    1148,           // u2VTotalMax
    810,            // u2VTotal60Hz
    910,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,
    0x00,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0xFF,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_26_WX2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    72354836,       // u4PixelClk60Hz
    68530000,       // u4PixelClk50Hz
    68000000,       // u4PixelClkMin

    1776,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1540,           // u2HTotal50Hz
    1456,           // u2HTotalMin

    1063,           // u2VTotalMax
    790,            // u2VTotal60Hz
    890,            // u2VTotal50Hz
    776,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,//180,            // u2DimmingFrequency60Hz
    0,//150,            // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_37_XW2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    85162837,       // u4PixelClk60Hz
    71040000,       // u4PixelClk50Hz
    65130000,       // u4PixelClkMin

    1515,           // u2HTotalMax
    1480,           // u2HTotal60Hz
    1480,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    980,            // u2VTotalMax
    960,            // u2VTotal60Hz
    960,            // u2VTotal50Hz
    940,            // u2VTotalMin
    61,             // u1VClkMax
    49,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    // Inverse PWM.
    0x00,//0xFF,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0xFF,//0x00,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    70,             // u1BacklightOnDelay
    2,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_47_WU4) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 70000000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1149,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,//180,            // u2DimmingFrequency60Hz
    0,//150,            // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    40,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_22_Z1) || CC_MULTI_PANEL)
    {
    1680,           // u2PanelWidth
    1050,           // u2PanelHeight

    2 * 74500000,   // u4PixelClkMax
    119112887,      // u4PixelClk60Hz
    114460000,      // u4PixelClk50Hz
    94340000,       // u4PixelClkMin

    2000,           // u2HTotalMax
    1840,           // u2HTotal60Hz
    1940,           // u2HTotal50Hz
    1780,           // u2HTotalMin

    1251,           // u2VTotalMax
    1080,           // u2VTotal60Hz
    1180,           // u2VTotal50Hz
    1060,           // u2VTotalMin
    76,             // u1VClkMax
    50,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    12,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_19_M2) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    900,            // u2PanelHeight

    2 * 65700000,   // u4PixelClkMax
    103672328,      // u4PixelClk60Hz
     90250000,      // u4PixelClk50Hz
    2 * 44375000,   // u4PixelClkMin (refer to PANEL_HSD_19_MGW1)

    1920,           // u2HTotalMax
    1840,           // u2HTotal60Hz
    1900,           // u2HTotal50Hz
    1760,           // u2HTotalMin

    1063,           // u2VTotalMax
    940,            // u2VTotal60Hz
    950,            // u2VTotal50Hz
    908,            // u2VTotalMin
    76,             // u1VClkMax
    56,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh  (Need Norman to confirm)
    0x70,           // (77%) u1BacklightMiddle
    0x00,           // (55%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    15,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    15,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_26_B1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    78626374,       // u4PixelClk60Hz
    65587500,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1650,           // u2HTotal60Hz
    1650,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    1063,           // u2VTotalMax
    795,            // u2VTotal60Hz
    795,            // u2VTotal50Hz
    778,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
    // Increase max luminance.
    0x00,//0x60,           // (100%) u1BacklightHigh
    0x80,           // (67%) u1BacklightMiddle
    0xA0,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,//160,            // u2DimmingFrequency50Hz
    0,//160,            // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_42_WU2) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 59630000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1105,           // u2VTotalMin
    53,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh (Need Norman to confirm)
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    15,             // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_HSD_19_MGW1) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    900,            // u2PanelHeight

    2 * 68375000,   // u4PixelClkMax
    106593566,      // u4PixelClk60Hz
    88916800,       // u4PixelClk50Hz
    2 * 44375000,   // u4PixelClkMin

    1936,           // u2HTotalMax
    1904,           // u2HTotal60Hz
    1904,           // u2HTotal50Hz
    1600,           // u2HTotalMin

    1001,           // u2VTotalMax
    934,            // u2VTotal60Hz
    934,            // u2VTotal50Hz
    926,            // u2VTotalMin
    75,             // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh  (Need Norman to confirm)
    0x70,           // (77%) u1BacklightMiddle
    0x00,           // (55%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_32_XW2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    888,            // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0x00,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    80,             // u1BacklightOnDelay
    21,             // u1BacklightOffDelay
    6,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_315_B1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    85678910,       // u4PixelClk60Hz
    75850000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1798,           // u2HTotal60Hz
    1850,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    888,            // u2VTotalMax
    795,            // u2VTotal60Hz
    820,            // u2VTotal50Hz
    778,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    // Inverse PWM, increase luminance.
    0x00,//0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xff,//0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    50,             // u1BacklightOnDelay
    10,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_50_X4_PDP) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    76923000,       // u4PixelClkMax
    74170000,       // u4PixelClk60Hz
    74170000,       // u4PixelClk50Hz
    71429000,       // u4PixelClkMin

    1545,           // u2HTotalMax
    1542,           // u2HTotal60Hz
    1542,           // u2HTotal50Hz
    1540,           // u2HTotalMin

    961,            // u2VTotalMax
    802,            // u2VTotal60Hz
    960,            // u2VTotal50Hz
    801,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    96,             // u1HSyncWidth
    10,             // u1VSyncWidth
    0x810,          // u1HPosition
    0x340,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh
    0xAB,           // (67%) u1BacklightMiddle
    0xE6,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    0,              // u1LvdsOnDalay...relay_on->vs_on
    150,            // u1BacklightOnDelay...vs_on->disp_on
    250,            // u1BacklightOffDelay...disp_off->vs_off
    30,             // u1LvdsOffDalay...vs_off->relay_off

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_26_XW3) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    68510000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1700,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1063,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x60,           // (100%) u1BacklightHigh
    0x80,           // (67%) u1BacklightMiddle
    0xA0,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    5,              // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_315_XW2_V0) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1063,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0x00,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    5,              // u1LvdsOnDalay
    90,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    7,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_315_XW2_V1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1063,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0x00,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    5,              // u1LvdsOnDalay
    90,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    7,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_154_LCM_C01) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    900,            // u2PanelHeight

    2 * 60000000,   // u4PixelClkMax
    88807193,       // u4PixelClk60Hz
    74080000,       // u4PixelClk50Hz
    2 * 25000000,   // u4PixelClkMin

    1760,           // u2HTotalMax
    1600,           // u2HTotal60Hz
    1600,           // u2HTotal50Hz
    1520,           // u2HTotalMin

    1500,           // u2VTotalMax
    926,            // u2VTotal60Hz
    926,            // u2VTotal50Hz
    910,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0x70,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_23_W2_L1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    84000000,       // u4PixelClkMax
    81550130,       // u4PixelClk60Hz
    68026400,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    1750,           // u2HTotalMax
    1688,           // u2HTotal60Hz
    1688,           // u2HTotal50Hz
    1420,           // u2HTotalMin

    1063,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    780,            // u2VTotalMin
    65,             // u1VClkMax
    55,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x30,           // u1BacklightHigh
    0x59,           // u1BacklightMiddle
    0xf0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    120,            // u1BacklightOnDelay
    12,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_315_XW2_V6) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    164000000,      // u4PixelClkMax
    146016000,      // u4PixelClk120Hz
    149760000,      // u4PixelClk50Hz
    139400000,      // u4PixelClkMin

    1600,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1496,           // u2HTotalMin

    980,            // u2VTotalMax
    780,            // u2VTotal60Hz
    960,            // u2VTotal50Hz
    776,            // u2VTotalMin
    121,             // u1VClkMax
    99,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xa0,           // (100%) u1BacklightHigh
    0x80,           // (84%) u1BacklightMiddle
    0x60,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    35,             // u1BacklightOnDelay
    2,              // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_32_WX3_SLB1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    72354836,       // u4PixelClk60Hz
    71200000,       // u4PixelClk50Hz
    68000000,       // u4PixelClkMin

    1776,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1600,           // u2HTotal50Hz
    1456,           // u2HTotalMin

    1063,           // u2VTotalMax
    790,            // u2VTotal60Hz
    890,            // u2VTotal50Hz
    775,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // (100%) u1BacklightHigh
    0xAB,           // (67%) u1BacklightMiddle
    0xE6,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_37_WX1_SLA1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    72354836,       // u4PixelClk60Hz
    71200000,       // u4PixelClk50Hz
    63000000,       // u4PixelClkMin

    1840,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1600,           // u2HTotal50Hz
    1446,           // u2HTotalMin

    1063,           // u2VTotalMax
    790,            // u2VTotal60Hz
    890,            // u2VTotal50Hz
    775,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CLAA_32_WB2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    84000000,       // u4PixelClkMax
    80012787,       // u4PixelClk60Hz
    66744000,       // u4PixelClk50Hz
    62000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1648,           // u2HTotal60Hz
    1648,           // u2HTotal50Hz
    1575,           // u2HTotalMin

    1063,           // u2VTotalMax
    810,            // u2VTotal60Hz
    810,            // u2VTotal50Hz
    790,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x1B,           // (100%) u1BacklightHigh
    0x73,           // (67%) u1BacklightMiddle
    0xB2,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    16,             // u1BacklightOnDelay
    15,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_154_LCM_C04) || CC_MULTI_PANEL)
    {
    1280,           // u2PanelWidth
    800,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    71036164,       // u4PixelClk60Hz
    59256000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1600,           // u2HTotalMax
    1440,           // u2HTotal60Hz
    1440,           // u2HTotal50Hz
    1360,           // u2HTotalMin

    1063,           // u2VTotalMax
    823,            // u2VTotal60Hz
    823,            // u2VTotal50Hz
    810,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0x70,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_201_TVM_C01) || CC_MULTI_PANEL)
    {
    1400,           // u2PanelWidth
    1050,           // u2PanelHeight

    2 * 67500000,   // u4PixelClkMax
    107856623,      // u4PixelClk60Hz
    89970400,       // u4PixelClk50Hz
    2 * 40000000,   // u4PixelClkMin

    1960,           // u2HTotalMax
    1688,           // u2HTotal60Hz
    1688,           // u2HTotal50Hz
    1480,           // u2HTotalMin

    1300,           // u2VTotalMax
    1066,           // u2VTotal60Hz
    1066,           // u2VTotal50Hz
    1051,           // u2VTotalMin
    75,             // u1VClkMax
    56,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_ON |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0x70,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    15,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_37_WX1_SL2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    72354836,       // u4PixelClk60Hz
    68530000,       // u4PixelClk50Hz
    68000000,       // u4PixelClkMin

    1776,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1540,           // u2HTotal50Hz
    1472,           // u2HTotalMin

    1063,           // u2VTotalMax
    790,            // u2VTotal60Hz
    890,            // u2VTotal50Hz
    776,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_37_XW2_V5) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    87760000,       // u4PixelClkMax
    85162837,       // u4PixelClk60Hz
    71040000,       // u4PixelClk50Hz
    64710000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    980,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    62,             // u1VClkMax
    49,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0x00,//0xFF,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0xFF,//0x00,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    80,             // u1BacklightOnDelay
    11,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_42_XW1_V3) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    822,            // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0xFF,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    80,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    10,             // u1LvdsOffDalay

    15,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_QD_32_HL1_W) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    80012787,       // u4PixelClk60Hz
    74984000,       // u4PixelClk50Hz
    70000000,       // u4PixelClkMin

    1780,           // u2HTotalMax
    1648,           // u2HTotal60Hz
    1648,           // u2HTotal50Hz
    1512,           // u2HTotalMin

    1148,           // u2VTotalMax
    810,            // u2VTotal60Hz
    910,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x00,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0xFF,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_37_XW1_V1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    85162837,       // u4PixelClk60Hz
    71040000,       // u4PixelClk50Hz
    65130000,       // u4PixelClkMin

    1515,           // u2HTotalMax
    1480,           // u2HTotal60Hz
    1480,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    980,            // u2VTotalMax
    960,            // u2VTotal60Hz
    960,            // u2VTotal50Hz
    940,            // u2VTotalMin
    61,             // u1VClkMax
    49,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0xFF,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    80,             // u1BacklightOnDelay
    8,              // u1BacklightOffDelay
    15,             // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_37_XW1_V2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    85162837,       // u4PixelClk60Hz
    71040000,       // u4PixelClk50Hz
    65130000,       // u4PixelClkMin

    1515,           // u2HTotalMax
    1480,           // u2HTotal60Hz
    1480,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    980,            // u2VTotalMax
    960,            // u2VTotal60Hz
    960,            // u2VTotal50Hz
    940,            // u2VTotalMin
    61,             // u1VClkMax
    49,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0xFF,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    80,             // u1BacklightOnDelay
    8,              // u1BacklightOffDelay
    15,             // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_42_W2_SLA2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    76300000,       // u4PixelClkMax
    72354845,       // u4PixelClk60Hz
    72580000,       // u4PixelClk50Hz
    68100000,       // u4PixelClkMin

    1776,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1528,           // u2HTotal50Hz
    1456,           // u2HTotalMin

    1063,           // u2VTotalMax
    790,            // u2VTotal60Hz
    950,            // u2VTotal50Hz
    776,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x23, // (100%) u1BacklightHigh     // u1BacklightHigh
    0x7D, // (67%) u1BacklightMiddle    // u1BacklightMiddle
    0xC5, // (50%) u1BacklightLow       // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_315_XW2_V3) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    963,            // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0x00,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    5,              // u1LvdsOnDalay
    90,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    7,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_320_TVM_A01H) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1900,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1015,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    784,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0xFF,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    11,             // u1BacklightOffDelay
    13,             // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_42_HW1_V0) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 84960000,   // u4PixelClkMax
    149010989,      // u4PixelClk60Hz
    124300000,      // u4PixelClk50Hz
    2 * 67360000,   // u4PixelClkMin

    2360,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2060,           // u2HTotalMin

    1200,           // u2VTotalMax
    1130,           // u2VTotal60Hz
    1130,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_42_WU3_SLA1) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 59630000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_42_H1_L5) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 80000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 60000000,   // u4PixelClkMin

    2300,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2100,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1115,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_ON |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_154_C1_P1) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    900,            // u2PanelHeight

    2 * 60000000,   // u4PixelClkMax
    88807193,       // u4PixelClk60Hz
    74080000,       // u4PixelClk50Hz
    2 * 25000000,   // u4PixelClkMin

    1760,           // u2HTotalMax
    1600,           // u2HTotal60Hz
    1600,           // u2HTotal50Hz
    1520,           // u2HTotalMin

    1500,           // u2VTotalMax
    926,            // u2VTotal60Hz
    926,            // u2VTotal50Hz
    910,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0x70,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_154_I2_L2) || CC_MULTI_PANEL)
    {
    1280,           // u2PanelWidth
    800,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    71036164,       // u4PixelClk60Hz
    59256000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1600,           // u2HTotalMax
    1440,           // u2HTotal60Hz
    1440,           // u2HTotal50Hz
    1360,           // u2HTotalMin

    1063,           // u2VTotalMax
    823,            // u2VTotal60Hz
    823,            // u2VTotal50Hz
    810,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0x70,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_40_FHD) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    130000000,      // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    155000000,      // u4PixelClkMin

    2650,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2160,           // u2HTotalMin

    1550,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1118,           // u2VTotalMin
    65,             // u1VClkMax
    45,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_JEIDA | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_32_WT_L5) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    80367632,       // u4PixelClk60Hz
    80364200,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    2000,           // u2HTotalMax
    1600,           // u2HTotal60Hz
    1918,           // u2HTotal50Hz
    1460,           // u2HTotalMin

    1200,           // u2VTotalMax
    838,            // u2VTotal60Hz
    838,            // u2VTotal50Hz
    773,            // u2VTotalMin
    66,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x30,           // u1BacklightHigh
    0x59,           // u1BacklightMiddle
    0xf0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    101,            // u1BacklightOnDelay
    11,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_46_HS_L03) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 80000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 60000000,   // u4PixelClkMin

    2300,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2100,           // u2HTotalMin

    1139,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1115,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,

    0x20,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_42_H1_L5_DELTA) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 80000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 60000000,   // u4PixelClkMin

    2300,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2100,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1115,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_ON |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_154_I5_P2) || CC_MULTI_PANEL)
    {
    1280,           // u2PanelWidth
    800,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    71036164,       // u4PixelClk60Hz
    59256000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1600,           // u2HTotalMax
    1440,           // u2HTotal60Hz
    1440,           // u2HTotal50Hz
    1360,           // u2HTotalMin

    1063,           // u2VTotalMax
    823,            // u2VTotal60Hz
    823,            // u2VTotal50Hz
    810,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0x70,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_40_XW1_V0) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1000,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0xE0,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    70,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_40_WT_L17) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    80367632,       // u4PixelClk60Hz
    67040000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    2040,           // u2HTotalMax
    1600,           // u2HTotal60Hz
    1600,           // u2HTotal50Hz
    1450,           // u2HTotalMin

    1500,           // u2VTotalMax
    838,            // u2VTotal60Hz
    838,            // u2VTotal50Hz
    773,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x00,           // u1BacklightHigh
    0x68,           // u1BacklightMiddle
    0xFF,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    110,            // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    5,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_315_XW1_VH) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    80012787,       // u4PixelClk60Hz
    66744000,       // u4PixelClk50Hz
    55000000,       // u4PixelClkMin

    1780,           // u2HTotalMax
    1648,           // u2HTotal60Hz
    1648,           // u2HTotal50Hz
    1512,           // u2HTotalMin

    1148,           // u2VTotalMax
    810,            // u2VTotal60Hz
    910,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x00,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0xFF,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_320_TVM_C12H) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    85678910,       // u4PixelClk60Hz
    75850000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1798,           // u2HTotal60Hz
    1850,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    888,            // u2VTotalMax
    795,            // u2VTotal60Hz
    820,            // u2VTotal50Hz
    778,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    // Inverse PWM, increase luminance.
    0x00,//0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xff,//0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    50,             // u1BacklightOnDelay
    10,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_315_XW1_VG) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    80012787,       // u4PixelClk60Hz
    74984000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1780,           // u2HTotalMax
    1648,           // u2HTotal60Hz
    1648,           // u2HTotal50Hz
    1512,           // u2HTotalMin

    1148,           // u2VTotalMax
    810,            // u2VTotal60Hz
    910,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x00,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0xFF,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_320_TVM_A02H) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1900,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1015,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    784,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0xFF,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    11,             // u1BacklightOffDelay
    13,             // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_320_TVM_A12H) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1900,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1015,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    784,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0xFF,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    11,             // u1BacklightOffDelay
    13,             // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_320_TVM_A03H) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    80012787,       // u4PixelClk60Hz
    74984000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1780,           // u2HTotalMax
    1648,           // u2HTotal60Hz
    1648,           // u2HTotal50Hz
    1512,           // u2HTotalMin

    1148,           // u2VTotalMax
    810,            // u2VTotal60Hz
    910,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x00,           // u1BacklightHigh
    0x68,           // u1BacklightMiddle
    0xFF,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_320_TVM_A21H) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1900,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1015,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    784,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0xFF,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    11,             // u1BacklightOffDelay
    13,             // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_320_TVM_C02H) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    85678910,       // u4PixelClk60Hz
    75850000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1798,           // u2HTotal60Hz
    1850,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    888,            // u2VTotalMax
    795,            // u2VTotal60Hz
    820,            // u2VTotal50Hz
    778,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    // Inverse PWM, increase luminance.
    0x00,//0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xff,//0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    50,             // u1BacklightOnDelay
    10,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_260_TVM_C01H) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    74909481,       // u4PixelClk60Hz
    62480000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1572,           // u2HTotal60Hz
    1572,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    888,            // u2VTotalMax
    795,            // u2VTotal60Hz
    795,            // u2VTotal50Hz
    778,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    // Inverse PWM, increase luminance.
    0x00,//0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xff,//0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    11,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_260_TVM_C11H) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    74909481,       // u4PixelClk60Hz
    62480000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1572,           // u2HTotal60Hz
    1572,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    888,            // u2VTotalMax
    795,            // u2VTotal60Hz
    795,            // u2VTotal50Hz
    778,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    // Inverse PWM, increase luminance.
    0x00,//0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xff,//0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    11,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_260_TVM_A01H) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366224,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    55000000,       // u4PixelClkMin

    2000,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1063,            // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    784,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    // Inverse PWM, increase luminance.
    0x00,//0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xff,//0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    50,             // u1BacklightOnDelay
    2,             // u1BacklightOffDelay
    4,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_260_TVM_A11H) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366224,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    55000000,       // u4PixelClkMin

    2000,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1063,            // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    784,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    // Inverse PWM, increase luminance.
    0x00,//0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xff,//0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    50,             // u1BacklightOnDelay
    2,             // u1BacklightOffDelay
    4,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_26_B1_L04) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    78626374,       // u4PixelClk60Hz
    65587500,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1650,           // u2HTotal60Hz
    1650,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    1063,           // u2VTotalMax
    795,            // u2VTotal60Hz
    795,            // u2VTotal50Hz
    778,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
    // Increase max luminance.
    0x00,//0x60,           // (100%) u1BacklightHigh
    0x80,           // (67%) u1BacklightMiddle
    0xA0,           // (50%) u1BacklightLow
    0,              // u1Reserved
    160,            // u2DimmingFrequency60Hz
    160,            // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_260_TVM_A04H) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    80012777,       // u4PixelClk60Hz
    66744000,       // u4PixelClk50Hz
    55000000,       // u4PixelClkMin

    1780,           // u2HTotalMax
    1648,           // u2HTotal60Hz
    1648,           // u2HTotal50Hz
    1512,           // u2HTotalMin

    1053,           // u2VTotalMax
    810,            // u2VTotal60Hz
    810,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
    // Increase max luminance.
    0x00,//0x60,           // (100%) u1BacklightHigh
    0x80,           // (67%) u1BacklightMiddle
    0xFF,           // (50%) u1BacklightLow
    0,              // u1Reserved
    160,            // u2DimmingFrequency60Hz
    160,            // u2DimmingFrequency50Hz

    6,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    4,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_42_HW1_V1) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 84960000,   // u4PixelClkMax
    149010989,      // u4PixelClk60Hz
    124300000,      // u4PixelClk50Hz
    2 * 67360000,   // u4PixelClkMin

    2360,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2060,           // u2HTotalMin

    1200,           // u2VTotalMax
    1130,           // u2VTotal60Hz
    1130,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_HIGH_PANEL_BRIGHT,

    0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_46_HS_L03_BESTBUY) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 80000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 60000000,   // u4PixelClkMin

    2300,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2100,           // u2HTotalMin

    1139,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1115,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,

    0x20,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_154_LCM_C05) || CC_MULTI_PANEL)
    {
    1280,           // u2PanelWidth
    800,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    71036164,       // u4PixelClk60Hz
    59256000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1600,           // u2HTotalMax
    1440,           // u2HTotal60Hz
    1440,           // u2HTotal50Hz
    1360,           // u2HTotalMin

    1063,           // u2VTotalMax
    823,            // u2VTotal60Hz
    823,            // u2VTotal50Hz
    810,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0x70,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_154_LCM_C03) || CC_MULTI_PANEL)
    {
    1280,           // u2PanelWidth
    800,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    71036164,       // u4PixelClk60Hz
    59256000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1600,           // u2HTotalMax
    1440,           // u2HTotal60Hz
    1440,           // u2HTotal50Hz
    1360,           // u2HTotalMin

    1063,           // u2VTotalMax
    823,            // u2VTotal60Hz
    823,            // u2VTotal50Hz
    810,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0x70,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_154_I2_P2) || CC_MULTI_PANEL)
    {
    1280,           // u2PanelWidth
    800,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    71036164,       // u4PixelClk60Hz
    59256000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1600,           // u2HTotalMax
    1440,           // u2HTotal60Hz
    1440,           // u2HTotal50Hz
    1360,           // u2HTotalMin

    1063,           // u2VTotalMax
    823,            // u2VTotal60Hz
    823,            // u2VTotal50Hz
    810,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0x70,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_19_A1_P2) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    900,            // u2PanelHeight

    2 * 56000000,   // u4PixelClkMax
    88807193,       // u4PixelClk60Hz
    74080000,       // u4PixelClk50Hz
    2 * 36489600,   // u4PixelClkMin

    1920,           // u2HTotalMax
    1600,           // u2HTotal60Hz
    1600,           // u2HTotal50Hz
    1500,           // u2HTotalMin

    1050,           // u2VTotalMax
    926,            // u2VTotal60Hz
    926,            // u2VTotal50Hz
    905,            // u2VTotalMin
    75,             // u1VClkMax
    56,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh
    0x70,           // (77%) u1BacklightMiddle
    0x00,           // (55%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    45,             // u1BacklightOnDelay
    9,              // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_22_Z1_P1) || CC_MULTI_PANEL)
    {
    1680,           // u2PanelWidth
    1050,           // u2PanelHeight

    2 * 74500000,   // u4PixelClkMax
    119112887,      // u4PixelClk60Hz
    114460000,      // u4PixelClk50Hz
    94340000,       // u4PixelClkMin

    2000,           // u2HTotalMax
    1840,           // u2HTotal60Hz
    1940,           // u2HTotal50Hz
    1780,           // u2HTotalMin

    1251,           // u2VTotalMax
    1080,           // u2VTotal60Hz
    1180,           // u2VTotal50Hz
    1060,           // u2VTotalMin
    76,             // u1VClkMax
    50,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    12,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_154_I2_P1) || CC_MULTI_PANEL)
    {
    1280,           // u2PanelWidth
    800,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    71036164,       // u4PixelClk60Hz
    59256000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1600,           // u2HTotalMax
    1440,           // u2HTotal60Hz
    1440,           // u2HTotal50Hz
    1360,           // u2HTotalMin

    1063,           // u2VTotalMax
    823,            // u2VTotal60Hz
    823,            // u2VTotal50Hz
    810,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0x70,           // u1BacklightMiddle
    0x00,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_INNOLUX_19_AW01) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    900,            // u2PanelHeight

    170000000,      // u4PixelClkMax
    88807193,            // u4PixelClk60Hz
    74080000,       // u4PixelClk50Hz
    50000000,      // u4PixelClkMin...

    2000,           // u2HTotalMax
    1600,           // u2HTotal60Hz
    1600,           // u2HTotal50Hz
    1504,           // u2HTotalMin

    1063,           // u2VTotalMax
    926,            // u2VTotal60Hz
    926,            // u2VTotal50Hz
    905,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xA0,           // (100%) u1BacklightHigh //
    0x80,           // (77%) u1BacklightMiddle
    0x00,           // (55%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDelay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_PV_320_TVM_C01) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    85678910,       // u4PixelClk60Hz
    75850000,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1798,           // u2HTotal60Hz
    1850,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    888,            // u2VTotalMax
    795,            // u2VTotal60Hz
    820,            // u2VTotal50Hz
    778,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    // Inverse PWM, increase luminance.
    0x00,//0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xff,//0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    50,             // u1BacklightOnDelay
    10,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_42_H1_L8) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 80000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 60000000,   // u4PixelClkMin

    2300,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2100,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1115,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_ON |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_42_WX4_SLB1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    2 * 80000000,   // u4PixelClkMax
    144520280,      // u4PixelClk60Hz
    120554000,      // u4PixelClk50Hz
    2 * 56000000,   // u4PixelClkMin

    1558,           // u2HTotalMax
    1526,           // u2HTotal60Hz
    1526,           // u2HTotal50Hz
    1494,           // u2HTotalMin

    805,            // u2VTotalMax
    790,            // u2VTotal60Hz
    790,            // u2VTotal50Hz
    775,            // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_30BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_40_FHD_10BIT) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    130000000,      // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    155000000,      // u4PixelClkMin

    2650,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2160,           // u2HTotalMin

    1550,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1118,           // u2VTotalMin
    65,             // u1VClkMax
    45,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_30BIT | LVDS_JEIDA | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_42_WU5_SLB1) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
	FOUR_PORT | DISP_24BIT | LVDS_JEIDA| LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER,
#else
    FOUR_PORT | DISP_24BIT | LVDS_JEIDA | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_315_XW2_V6_8280) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    800,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    76675325,       // u4PixelClk120Hz
    74880000,       // u4PixelClk100Hz
    69700000,       // u4PixelClkMin

    1600,           // u2HTotalMax
    1560,           // u2HTotal120Hz
    1560,           // u2HTotal100Hz
    1496,           // u2HTotalMin

    980,            // u2VTotalMax
    820,            // u2VTotal120Hz
    960,            // u2VTotal100Hz
    776,            // u2VTotalMin
    122,            // u1VClkMax
    98,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    FOUR_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xa0,           // (100%) u1BacklightHigh
    0x80,           // (84%) u1BacklightMiddle
    0x60,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    40,             // u1BacklightOnDelay
    2,              // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_42_WU5_SLB1_8280) || CC_MULTI_PANEL)
    {
    1952,           // u2PanelWidth
    1096,           // u2PanelHeight

    2 * 75000000,   // u4PixelClkMax
    148351648,      // u4PixelClk120Hz
    148351648,      // u4PixelClk100Hz
    2 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal120Hz
    2200,           // u2HTotal100Hz
    2080,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal120Hz
    1350,           // u2VTotal100Hz
    1090,           // u2VTotalMin
    121,            // u1VClkMax
    91,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
},
#endif
#if ((PANEL_SELECT == PANEL_LG_42_WX4_SLB1_8280) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    800,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    75004196,       // u4PixelClk120Hz
    62566000,       // u4PixelClk100Hz
    56000000,       // u4PixelClkMin

    1558,           // u2HTotalMax
    1526,           // u2HTotal120Hz
    1526,           // u2HTotal100Hz
    1494,           // u2HTotalMin

    850,            // u2VTotalMax
    820,            // u2VTotal120Hz
    820,            // u2VTotal100Hz
    775,            // u2VTotalMin
    123,            // u1VClkMax
    97,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_42_HW1_V3) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

#ifdef CC_SCPOS_3DTV_SUPPORT
    4 * 84900000,   // u4PixelClkMax
    297000000,//297830400,      // u4PixelClk60Hz
    247500000,//248192000,      // u4PixelClk50Hz
    4 * 59400000,// 4 * 69400000,   // u4PixelClkMin

    2508,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,          // u2HTotal50Hz
    2112,           // u2HTotalMin
#else
    4 * 84900000,   // u4PixelClkMax
    297830400,      // u4PixelClk60Hz
    248192000,      // u4PixelClk50Hz
    4 * 69400000,   // u4PixelClkMin

    2508,           // u2HTotalMax
    2240,           // u2HTotal60Hz
    2240,           // u2HTotal50Hz
    2112,           // u2HTotalMin
#endif

    1201,           // u2VTotalMax
    1125,           // u2VTotal120Hz
    1125,           // u2VTotal100Hz
    1090,           // u2VTotalMin
    120,             // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_RLRL,
#else
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
#endif

    0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    5,              // u1LvdsOnDalay
    25,             // u1BacklightOnDelay
    0,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_40_HH_LH2) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    310000000,   // u4PixelClkMax
    297000000,      // u4PixelClk60Hz
    297000000,      // u4PixelClk50Hz
    240000000,   // u4PixelClkMin

    2701,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2140,           // u2HTotalMin

    1601,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1105,           // u2VTotalMin
    125,            // u1VClkMax
    90,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT |
    LVDS_A_FROM_AO | LVDS_B_FROM_BO | LVDS_C_FROM_AE | LVDS_D_FROM_BE,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_46_HH_L01) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    310000000,   // u4PixelClkMax
    297000000,      // u4PixelClk60Hz
    296700000,      // u4PixelClk50Hz
    260000000,   // u4PixelClkMin

    2350,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2300,           // u2HTotal50Hz
    2090,           // u2HTotalMin

    1380,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1290,           // u2VTotal50Hz
    1092,           // u2VTotalMin
    130,             // u1VClkMax
    90,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT |
    LVDS_A_FROM_AE | LVDS_B_FROM_BE | LVDS_C_FROM_AO | LVDS_D_FROM_BO,

    0x20,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_FHD60_LVDS_CAP) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 70000000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,         // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    4,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0,            // u1HPosition
    1024,    // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x23, // (100%) u1BacklightHigh     // u1BacklightHigh
    0x7D, // (67%) u1BacklightMiddle    // u1BacklightMiddle
    0xC5, // (50%) u1BacklightLow       // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_FHD120_LVDS_CAP) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 70000000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,         // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    4,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0,            // u1HPosition
    1027,    // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x23, // (100%) u1BacklightHigh     // u1BacklightHigh
    0x7D, // (67%) u1BacklightMiddle    // u1BacklightMiddle
    0xC5, // (50%) u1BacklightLow       // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_WXGA60_LVDS_CAP) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 70000000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,         // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    4,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0,            // u1HPosition
    1027,    // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x23, // (100%) u1BacklightHigh     // u1BacklightHigh
    0x7D, // (67%) u1BacklightMiddle    // u1BacklightMiddle
    0xC5, // (50%) u1BacklightLow       // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_WXGA120_LVDS_CAP) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 70000000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,         // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    4,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0,            // u1HPosition
    1027,    // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x23, // (100%) u1BacklightHigh     // u1BacklightHigh
    0x7D, // (67%) u1BacklightMiddle    // u1BacklightMiddle
    0xC5, // (50%) u1BacklightLow       // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SLT_FHD_60Hz_10BIT) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    148425750,      // u4PixelClMax
    148425750,      // u4PixelClk50Hz
    148425750,      // u4PixelClk60Hz
    148425750,      // u4PixelClkMin

    2650,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2160,           // u2HTotalMin

    1550,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1118,           // u2VTotalMin
    65,             // u1VClkMax
    45,             // u1VClkMin

    30,              // u1HSyncWidth
    1,               // u1VSyncWidth
    0x82c,           // u1HPosition

    0x3fc,            // u1VPosition
    DUAL_PORT | DISP_30BIT | LVDS_JEIDA | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SLT_FHD_120Hz_10BIT) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296460000,      // u4PixelClk60Hz
    296460000,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2650,           // u2HTotalMax
    2196,           // u2HTotal60Hz
    2196,           // u2HTotal50Hz
    2160,           // u2HTotalMin

    1125,
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1118,           // u2VTotalMin
    130,
    90,

    30,               // u1HSyncWidth
    1,                // u1VSyncWidth
    0x810,            // u1HPosition

    0x400,            // u1VPosition
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_INNOLUX_19_AW02_LIPS_LOW) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    900,            // u2PanelHeight

    170000000,      // u4PixelClkMax
    88807193,            // u4PixelClk60Hz
    74080000,       // u4PixelClk50Hz
    50000000,      // u4PixelClkMin...

    2000,           // u2HTotalMax
    1600,           // u2HTotal60Hz
    1600,           // u2HTotal50Hz
    1504,           // u2HTotalMin

    1063,           // u2VTotalMax
    926,            // u2VTotal60Hz
    926,            // u2VTotal50Hz
    905,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xF0,           // (100%) u1BacklightHigh
    0x90,           // (77%) u1BacklightMiddle
    0x00,           // (55%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDelay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_216_B1_L01_LIPS_LOW) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    78626374,       // u4PixelClk60Hz
    65587500,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1650,           // u2HTotal60Hz
    1650,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    1063,           // u2VTotalMax
    795,            // u2VTotal60Hz
    795,            // u2VTotal50Hz
    778,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_HIGH_PANEL_BRIGHT,
    // Increase max luminance.
    0xF0,           // (100%) u1BacklightHigh //
    0x90,           // (77%) u1BacklightMiddle
    0x00,           // (55%) u1BacklightLow
    0,              // u1Reserved
    0,            // u2DimmingFrequency60Hz
    0,            // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_260_B1_L11_LIPS_HIGH) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    78626374,       // u4PixelClk60Hz
    65587500,       // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1650,           // u2HTotal60Hz
    1650,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    1063,           // u2VTotalMax
    795,            // u2VTotal60Hz
    795,            // u2VTotal50Hz
    778,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
    // Increase max luminance.
    0x00,//0x60,           // (100%) u1BacklightHigh
    0x80,           // (67%) u1BacklightMiddle
    0xA0,           // (50%) u1BacklightLow
    0,              // u1Reserved
    160,            // u2DimmingFrequency60Hz
    160,            // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_19_PW1) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    900,            // u2PanelHeight

    2 * 64000000,   // u4PixelClkMax
    94970880,       // u4PixelClk60Hz
    79142400,       // u4PixelClk50Hz
    2 * 32500000,   // u4PixelClkMin

    2048,           // u2HTotalMax
    1728,           // u2HTotal60Hz
    1728,           // u2HTotal50Hz
    1520,           // u2HTotalMin

    2048,           // u2VTotalMax
    916,            // u2VTotal60Hz
    916,            // u2VTotal50Hz
    910,            // u2VTotalMin
    76,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh
    0x70,           // (77%) u1BacklightMiddle
    0xFF,           // (55%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    200,             // u1BacklightOnDelay
    200,              // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_216_B1_L1) || CC_MULTI_PANEL)
     {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    80415584,       // u4PixelClk60Hz //V(842) H(2376) F(60)
    67080000,       // u4PixelClk50Hz //V(842) H(2376) F(50)
    60000000,       // u4PixelClkMin...spec conflict

    1936,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    888,           // u2VTotalMax
    861,           // u2VTotal60Hz
    861,           // u2VTotal50Hz
    778,            // u2VTotalMin
    63,             // u1VClkMax
    57,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh
    0x70,           // (77%) u1BacklightMiddle//@I001 refine back light duty cycle
    0xFF,           // (55%) u1BacklightLow
    0,              // u1Reserved
    3200,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay 					//@I001
    30,             // u1BacklightOnDelay 				//@I001
    6,             // u1BacklightOffDelay 				//@I001
    1,              // u1LvdsOffDalay

    90,             // u2MaxOverscan                               //@I012 20080708
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_37_XW2_VC) || CC_MULTI_PANEL)  // @sammi modified 080910start
    {
    1366,           // u2PanelWidth
    768,           // u2PanelHeight

    85000000,   // u4PixelClkMax
    78115200,      // u4PixelClk60Hz
    59645000,      // u4PixelClk50Hz
    60000000,   // u4PixelClkMin

    1901,           // u2HTotalMax
    1649,           // u2HTotal60Hz
    1511,           // u2HTotal50Hz
    1415,           // u2HTotalMin

    1016,           // u2VTotalMax
    807,           // u2VTotal60Hz
    791,           // u2VTotal50Hz
    785,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition


    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // u1BacklightHigh
    0xB4,           // u1BacklightMiddle
    0x34,           // u1BacklightLow
    0,              // u1Reserved
    150,              // u2DimmingFrequency60Hz
    150,              // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    40,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved               // @sammi modified 080910 end
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_32_WT_1) || CC_MULTI_PANEL) // start add by stanley 080908
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    86000000,       // u4PixelClkMax
    72354836,       // u4PixelClk60Hz
    68530000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    2001,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1540,           // u2HTotal50Hz
    1451,           // u2HTotalMin

    1301,           // u2VTotalMax
    790,            // u2VTotal60Hz
    890,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,         // (100%) u1BacklightHigh 0x00
    0xB5,         // (67%) u1BacklightMiddle 0x8B
    0x34,         // (50%) u1BacklightLow    0xFF
    0,              // u1Reserved
    150,             // u2DimmingFrequency60Hz
    150,             // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    40,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_26_WT_1) || CC_MULTI_PANEL) // start add by stanley 080908
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    68510000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1700,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1063,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh
    0xB4,           // (67%) u1BacklightMiddle
    0x35,           // (50%) u1BacklightLow
    0,              // u1Reserved
    160,              // u2DimmingFrequency60Hz
    160,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    5,              // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_22_B1_L01) || CC_MULTI_PANEL)  // sammi modified 080912
    {
    1366,           // u2PanelWidth
    768,           // u2PanelHeight

    82000000,   // u4PixelClkMax
    76000000,      // u4PixelClk60Hz
    76000000,      // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1937,           // u2HTotalMax
    1561,           // u2HTotal60Hz
    1561,           // u2HTotal50Hz
    1443,           // u2HTotalMin

    984,           // u2VTotalMax
    807,           // u2VTotal60Hz
    807,           // u2VTotal50Hz
    779,           // u2VTotalMin
    76,             // u1VClkMax
    50,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition

    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,


    0xFF,           // u1BacklightHigh
    0xB5,           // u1BacklightMiddle
    0x34,           // u1BacklightLow
    0,              // u1Reserved
    150,              // u2DimmingFrequency60Hz
    150,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    12,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_19_XW1) || CC_MULTI_PANEL)   //sammi modifed 080912
      {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    90000000,   // u4PixelClkMax
    78000000,       // u4PixelClk60Hz
    78000000,       // u4PixelClk50Hz
    60000000,   // u4PixelClkMin

    2048,           // u2HTotalMax
    1607,           // u2HTotal60Hz
    1607,           // u2HTotal50Hz
    1417,           // u2HTotalMin

    1024,           // u2VTotalMax
    809,            // u2VTotal60Hz
    809,            // u2VTotal50Hz
    777,            // u2VTotalMin
    76,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh
    0x80,           // (77%) u1BacklightMiddle
    0x34,           // (55%) u1BacklightLow
    0,              // u1Reserved
    150,              // u2DimmingFrequency60Hz
    150,              // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    70,             // u1BacklightOnDelay
    2,              // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_52_FHD) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    155000000,      // u4PixelClkMax
    147029702,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    130000000,      // u4PixelClkMin

    2450,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2154,           // u2HTotalMin

    1480,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1100,           // u2VTotalMin
    62,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_JEIDA | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x20,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0xa0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    1,              // u1LvdsOnDalay
    20,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_32_WT_1_WXGA2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    86000000,       // u4PixelClkMax
    72354836,       // u4PixelClk60Hz
    68530000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    2001,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1540,           // u2HTotal50Hz
    1451,           // u2HTotalMin

    1301,           // u2VTotalMax
    790,            // u2VTotal60Hz
    890,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,         // (100%) u1BacklightHigh 0x00
    0xB5,         // (67%) u1BacklightMiddle 0x8B
    0x34,         // (50%) u1BacklightLow    0xFF
    0,              // u1Reserved
    150,             // u2DimmingFrequency60Hz
    150,             // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    40,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif // end add by stanley 080908
#if ((PANEL_SELECT == PANEL_AUO_315_XW2_VS_WXGA2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    164000000,      // u4PixelClkMax
    146016000,      // u4PixelClk120Hz
    149760000,      // u4PixelClk50Hz
    139400000,      // u4PixelClkMin

    1600,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1496,           // u2HTotalMin

    980,            // u2VTotalMax
    780,            // u2VTotal60Hz
    960,            // u2VTotal50Hz
    776,            // u2VTotalMin
    121,             // u1VClkMax
    99,             // u1VClkMin
    //61,             // u1VClkMax
    //49,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xa0,           // (100%) u1BacklightHigh
    0x80,           // (84%) u1BacklightMiddle
    0x60,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    35,             // u1BacklightOnDelay
    2,              // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_32NC_LBA_SESL_WXGA1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    86000000,       // u4PixelClkMax
    72354836,       // u4PixelClk60Hz
    68530000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    2001,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1540,           // u2HTotal50Hz
    1451,           // u2HTotalMin

    1301,           // u2VTotalMax
    790,            // u2VTotal60Hz
    890,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,         // (100%) u1BacklightHigh 0x00
    0xB5,         // (67%) u1BacklightMiddle 0x8B
    0x34,         // (50%) u1BacklightLow    0xFF
    0,              // u1Reserved
    150,             // u2DimmingFrequency60Hz
    150,             // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    40,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_32NC_LBA_SESL_WXGA2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    86000000,       // u4PixelClkMax
    72354836,       // u4PixelClk60Hz
    68530000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    2001,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1540,           // u2HTotal50Hz
    1451,           // u2HTotalMin

    1301,           // u2VTotalMax
    790,            // u2VTotal60Hz
    890,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,         // (100%) u1BacklightHigh 0x00
    0xB5,         // (67%) u1BacklightMiddle 0x8B
    0x34,         // (50%) u1BacklightLow    0xFF
    0,              // u1Reserved
    150,             // u2DimmingFrequency60Hz
    150,             // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    40,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_T315XW02_VS_WXGA1) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    888,            // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0x00,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    7,              // u1LvdsOnDalay
    80,             // u1BacklightOnDelay
    21,             // u1BacklightOffDelay
    6,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_T315XW02_VS_WXGA2) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    88000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    62868000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    888,            // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh
    0x68,           // (84%) u1BacklightMiddle
    0x00,           // (68%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    7,              // u1LvdsOnDalay
    80,             // u1BacklightOnDelay
    21,             // u1BacklightOffDelay
    6,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_32NC_LBA_SESL_FHD) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    155000000,      // u4PixelClkMax
    148500000,      // u4PixelClk60Hz
    148500000,      // u4PixelClk50Hz
    130000000,      // u4PixelClkMin

    2650,           // u2HTotalMax
    2184,           // u2HTotal60Hz
    2184,           // u2HTotal50Hz
    2160,           // u2HTotalMin

    1550,           // u2VTotalMax
    1134,           // u2VTotal60Hz
    1360,           // u2VTotal50Hz
    1118,           // u2VTotalMin
    62,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_30BIT | LVDS_JEIDA | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    40,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_46_HW03_V4) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    247500000,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
	FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER | LVDS_DISP_SPLIT_ON,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_65_HW02_V1) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 84900000,   // u4PixelClkMax
    297000000,      // u4PixelClk60Hz
    247500000,      // u4PixelClk50Hz
    4 * 59400000,   // u4PixelClkMin

    2508,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2112,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal120Hz
    1125,           // u2VTotal100Hz
    1090,           // u2VTotalMin
    120,             // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#if 0 // AUO Glasses
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    LVDS_A_FROM_AO | LVDS_B_FROM_BO | LVDS_C_FROM_AE | LVDS_D_FROM_BE |
    PWM_HIGH_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_RLRL,
#else // RealD Glasses
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    LVDS_A_FROM_AO | LVDS_B_FROM_BO | LVDS_C_FROM_AE | LVDS_D_FROM_BE |
    PWM_HIGH_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_LRLR,
#endif
    0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    5,              // u1LvdsOnDalay
    25,             // u1BacklightOnDelay
    0,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_42_WUD_SAC1_10BIT) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    247500000,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
	FOUR_PORT | DISP_30BIT | LVDS_NS | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER,
#else
    FOUR_PORT | DISP_30BIT | LVDS_NS | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    120,              // u2DimmingFrequency60Hz
    100,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT ==  PANEL_LG_42_WU5_SLB1_VB1) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER | DISP_TYPE_VB1,
#else
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | DISP_TYPE_VB1,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_LGDV4FHD60) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 59630000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_MLVDS_LGDV4FHD120) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER | DISP_TYPE_MLVDS,
#else
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_LGDV4GIPFHD60) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 59630000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_MLVDS_LGDV4GIPFHD120) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER | DISP_TYPE_MLVDS,
#else
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_ChangHong_PDP) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 76250000,   // u4PixelClkMax
    148500000,      // u4PixelClk60Hz
    148500000,      // u4PixelClk50Hz
    2 * 72250000,   // u4PixelClkMin

    2360,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2000,           // u2HTotalMin

    1500,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1089,           // u2VTotalMin
    62,             // u1VClkMax
    45,             // u1VClkMin

    20,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x3FB,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0x00,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0xFF,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    50,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_LGDV5GIPFHD60) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 59630000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_LGDV5GIPFHD120) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER | DISP_TYPE_MLVDS,
#else
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_LGDV6GIPFHD60) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 59630000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_LGDV6GIPFHD120) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER | DISP_TYPE_MLVDS,
#else
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_AUOFHD60) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 59630000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_AUOFHD120) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER | DISP_TYPE_MLVDS,
#else
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_SHAFHD60) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 59630000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_SHAFHD120) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER | DISP_TYPE_MLVDS,
#else
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_FUNAWXGA) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    68510000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1700,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1063,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,

    0x60,           // (100%) u1BacklightHigh
    0x80,           // (67%) u1BacklightMiddle
    0xA0,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    5,              // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_FUNAFHD60) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 59630000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_HIWXGA09) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    68510000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1700,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1063,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,

    0x60,           // (100%) u1BacklightHigh
    0x80,           // (67%) u1BacklightMiddle
    0xA0,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    5,              // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_HIWXGA10) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    68510000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1700,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1063,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,

    0x60,           // (100%) u1BacklightHigh
    0x80,           // (67%) u1BacklightMiddle
    0xA0,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    5,              // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_WXGA) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    68510000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    1722,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1700,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1063,           // u2VTotalMax
    806,            // u2VTotal60Hz
    806,            // u2VTotal50Hz
    789,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,

    0x60,           // (100%) u1BacklightHigh
    0x80,           // (67%) u1BacklightMiddle
    0xA0,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    5,              // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_CMO2011FHD60) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 59630000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | DISP_TYPE_MLVDS,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_MLVDS_PANASONIC32WXGA8BIT) || CC_MULTI_PANEL)
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    78106560,       // u4PixelClk60Hz
    78106560,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    1721,           // u2HTotalMax
    1576,    //1492,           // u2HTotal60Hz
    1576,    //1492,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    821,            // u2VTotalMax
    826,             // u2VTotal60Hz
    991,            // u2VTotal50Hz
    700,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | OD_ON  | DISP_TYPE_MLVDS,

    0x60,           // (100%) u1BacklightHigh
    0x80,           // (67%) u1BacklightMiddle
    0xA0,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    5,              // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_47_EUH_PR) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    247500000,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition

	// AmTRAN : LVDS = Special NS, PR order = LRLR(RealD glasses) or RLRL(AUO glasses)
	// TPV : LVDS = Special NS, PR order = RLRL(RealD glasses) or LRLR(AUO glasses)
	// Philip : LVDS = NS, PR order = RLRL(unknown glasses)
#if 1
	FOUR_PORT | DISP_30BIT | LVDS_SPECIAL_NS | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_LRLR |OD_ON,
#else
	FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_RLRL,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_M236H5_L0A) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    247500000,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
	FOUR_PORT | DISP_30BIT | LVDS_SPECIAL_NS | LVDS_ODD_SW_OFF |
    LVDS_A_FROM_AO | LVDS_B_FROM_BO | LVDS_C_FROM_AE | LVDS_D_FROM_BE |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER,
#else
    FOUR_PORT | DISP_30BIT | LVDS_SPECIAL_NS | LVDS_ODD_SW_OFF |
    LVDS_A_FROM_AO | LVDS_B_FROM_BO | LVDS_C_FROM_AE | LVDS_D_FROM_BE |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_LG_42_WUD_SAC1_10BIT_NS) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 77500000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    247500000,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1200,           // u2VTotalMax (display mode)
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
	FOUR_PORT | DISP_30BIT | LVDS_NS | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER,
#else
    FOUR_PORT | DISP_30BIT | LVDS_NS | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    120,              // u2DimmingFrequency60Hz
    100,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_42_WU5_SLB1_PR) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 77500000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax (display mode)
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
	FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_LRLR,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_42_WU5_SLB1_SG) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 77500000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax (display mode)
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
	FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMO_M236H3_LHH3) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    344784000,   // u4PixelClkMax
    316574400,      // u4PixelClk60Hz
    316736000,      // u4PixelClk50Hz
    220180000,   // u4PixelClkMin

    2200,           // u2HTotalMax
    2020,           // u2HTotal60Hz
    2020,           // u2HTotal50Hz
    2020,           // u2HTotalMin

    1600,           // u2VTotalMax (display mode)
    1306,           // u2VTotal60Hz
    1569,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
	FOUR_PORT | DISP_30BIT | LVDS_SPECIAL_NS | LVDS_ODD_SW_OFF | LVDS_DISP_SPLIT_ON|
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER,
#else
    FOUR_PORT | DISP_30BIT | LVDS_SPECIAL_NS | LVDS_ODD_SW_OFF | LVDS_DISP_SPLIT_ON|
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_LG_42_WU5_SLB1_AS3693) || CC_MULTI_PANEL)
   {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 77500000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax (display mode)
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
	FOUR_PORT | DISP_30BIT | LVDS_SPECIAL_NS| LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER,
#else
    FOUR_PORT | DISP_30BIT | LVDS_SPECIAL_NS | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
   },
#endif
#if ((PANEL_SELECT == PANEL_LG_42_WU5_SLB1_IWATT) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 77500000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax (display mode)
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
	FOUR_PORT | DISP_30BIT | LVDS_SPECIAL_NS| LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER,
#else
    FOUR_PORT | DISP_30BIT | LVDS_SPECIAL_NS | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_LG_42_WU5_SLB1_SG_BEX) || CC_MULTI_PANEL) // FHD-120Hz-2D also define as FHD-120Hz-SGX
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    344784000,   // u4PixelClkMax
    316574400,      // u4PixelClk60Hz
    316736000,      // u4PixelClk50Hz
    220180000,   // u4PixelClkMin

    2200,           // u2HTotalMax
    2020,           // u2HTotal60Hz
    2020,           // u2HTotal50Hz
    2020,           // u2HTotalMin

    1569,           // u2VTotalMax (display mode)
    1306,           // u2VTotal60Hz
    1569,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
	FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER,
#else
	FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_AUO_46_HW03_V4_FHD240) || CC_MULTI_PANEL) // FHD-240Hz-SG
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    8*76000000,   // u4PixelClkMax
    2*302016000,      // u4PixelClk60Hz
    2*296703297,      // u4PixelClk50Hz
    8*66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax (display mode)
    1144,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    240,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF | LVDS_SPECIAL_NS | PANEL_REODER_SEQ_TO_HALF |
    LVDS_A_FROM_AE | LVDS_B_FROM_BE | LVDS_C_FROM_AO | LVDS_D_FROM_BO |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER| LVDS_DISP_SPLIT_OFF | LVDS_HIGHSPEED,
    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    0, //2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_AUO_46_HW03_V3) || CC_MULTI_PANEL) // FHD-120Hz-2D also define as FHD-120Hz-SG
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 76000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax (display mode)
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_30BIT | LVDS_NS| LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER |
    LVDS_A_FROM_AE | LVDS_B_FROM_BE | LVDS_C_FROM_AO | LVDS_D_FROM_BO,
#else
    FOUR_PORT | DISP_30BIT | LVDS_NS | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT |
    LVDS_A_FROM_AE | LVDS_B_FROM_BE | LVDS_C_FROM_AO | LVDS_D_FROM_BO,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_LG_50_CINEMA) || CC_MULTI_PANEL)
    {
    2560,           // u2PanelWidth
    1080,           // u2PanelHeight

    4*103500000,     // u4PixelClkMax
    4*96770000,      // u4PixelClk60Hz
    4*96770000,      // u4PixelClk50Hz
    4*89900000,      // u4PixelClkMin

    4*710,           // u2HTotalMax
    4*700,           // u2HTotal60Hz
    4*700,           // u2HTotal50Hz
    4*680,           // u2HTotalMin

    1390,           // u2VTotalMax (display mode)
    1152,           // u2VTotal60Hz
    1383,           // u2VTotal50Hz
    1105,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF | LVDS_SPECIAL_NS |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_LRLR| LVDS_DISP_SPLIT_OFF |
    PANEL_ASPECT_RATIO_21_9,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    0,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_FOR_8283) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148500000,      // u4PixelClk60Hz
    148500000,      // u4PixelClk50Hz
    2 * 59630000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1400,           // u2VTotalMax (display mode)
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition

#ifdef CC_SCPOS_3DTV_SUPPORT
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER,
#else
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,
#endif

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif



#if ((PANEL_SELECT == PANEL_AUO_T576DC01_V1) || CC_MULTI_PANEL)
    {
    2560,           // u2PanelWidth
    1080,           // u2PanelHeight

    4*103008000,     // u4PixelClkMax
    4*96004800,      // u4PixelClk60Hz
    4*96004800,      // u4PixelClk50Hz
    4*92064000,      // u4PixelClkMin

    4*740,           // u2HTotalMax
    4*708,           // u2HTotal60Hz
    4*708,           // u2HTotal50Hz
    4*700,           // u2HTotalMin

    1392,           // u2VTotalMax (display mode)
    1130,           // u2VTotal60Hz
    1356,           // u2VTotal50Hz
    1096,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_RLRL| LVDS_DISP_SPLIT_OFF | PANEL_ASPECT_RATIO_21_9,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    0,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_CMI_V420H2_LS1) || CC_MULTI_PANEL) // FHD-120Hz-SG
	{
	1920,           // u2PanelWidth
	1080,           // u2PanelHeight

	4 * 77500000,   // u4PixelClkMax
	296703297,      // u4PixelClk60Hz
	296703297,      // u4PixelClk50Hz
	4 * 66970000,   // u4PixelClkMin

	2720,           // u2HTotalMax
	2200,           // u2HTotal60Hz
	2200,           // u2HTotal50Hz
	2080,           // u2HTotalMin

	1380,           // u2VTotalMax (display mode)
	1125,           // u2VTotal60Hz
	1350,           // u2VTotal50Hz
	1090,           // u2VTotalMin
	120,            // u1VClkMax
	60,             // u1VClkMin...spec not defined

	30,             // u1HSyncWidth
	3,              // u1VSyncWidth
	0x810,          // u1HPosition
	0x400,          // u1VPosition

#if 1 // order = 1023
	FOUR_PORT | DISP_30BIT | LVDS_NS | LVDS_ODD_SW_OFF |
	WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER |
	LVDS_A_FROM_AO | LVDS_B_FROM_AE| LVDS_C_FROM_BE | LVDS_D_FROM_BO,
	//WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
#else // // order = 0123
	FOUR_PORT | DISP_30BIT | LVDS_NS | LVDS_ODD_SW_OFF |
	WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER,
#endif

	0x08,           // u1BacklightHigh
	0x80,           // u1BacklightMiddle
	0x90,           // u1BacklightLow
	0,              // u1Reserved
	0,              // u2DimmingFrequency60Hz
	0,              // u2DimmingFrequency50Hz

	2,              // u1LvdsOnDalay
	30,             // u1BacklightOnDelay
	30,             // u1BacklightOffDelay
	2,              // u1LvdsOffDalay

	30,             // u2MaxOverscan
	0,              // u2Reserved
	},
#endif
#if ((PANEL_SELECT == PANEL_LC420EUD_SDF1_PR) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 77500000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    247500000,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

//    1165,           // u2VTotalMax
    1201,           // u2VTotalMax (display mode)
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition

#if 1
	FOUR_PORT | DISP_30BIT | LVDS_NS | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_LRLR,
#else
	FOUR_PORT | DISP_30BIT | LVDS_NS | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_RLRL,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_T645HW05_V0) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 84900000,   // u4PixelClkMax
    297000000,//297830400,      // u4PixelClk60Hz
    247500000,//248192000,      // u4PixelClk50Hz
    4 * 59400000,// 4 * 69400000,   // u4PixelClkMin

    2508,           // u2HTotalMax
    2200,//2239,           // u2HTotal60Hz
    2200,//2239,           // u2HTotal50Hz
    2112,           // u2HTotalMin

    1201,           // u2VTotalMax (display mode)
    1125,           // u2VTotal120Hz
    1125,           // u2VTotal100Hz
    1090,           // u2VTotalMin
    120,             // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition

    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    LVDS_A_FROM_AO | LVDS_B_FROM_BO | LVDS_C_FROM_AE | LVDS_D_FROM_BE |
    PWM_HIGH_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_RLRL,

    0xf0,           // u1BacklightHigh
    0xd0,           // u1BacklightMiddle
    0xb0,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    5,              // u1LvdsOnDalay
    25,             // u1BacklightOnDelay
    0,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_SS_42_PDP) || CC_MULTI_PANEL)
   {
	 1024,			 // u2PanelWidth
	 2 * 768,			 // u2PanelHeight
	 2 * 78000000,		// u4PixelClkMax
	 2 * 74000000,		// u4PixelClk60Hz
	 2 * 74000000,		 // u4PixelClk50Hz
	 2 * 71200000,		 // u4PixelClkMin
	 1505,			// u2HTotalMax
	 1504,			// u2HTotal60Hz
	 1504,			// u2HTotal50Hz
	 1451,			// u2HTotalMin
	 2 * 830,		   // u2VTotalMax
	 2 * 820, 		   // u2VTotal60Hz
	 2 * 984, 		   // u2VTotal50Hz
	 2 * 810, 		   // u2VTotalMin
	 63,			 // u1VClkMax
	 47,			 // u1VClkMin
	 10,			 // u1HSyncWidth
	 2 * 6, 			 // u1VSyncWidth
	 0x810, 		 // u1HPosition
	 0x400, 		 // u1VPosition
	 DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |PWM_LOW_PANEL_BRIGHT | PANEL_PDP
	 | LVDS_A_FROM_AO | LVDS_B_FROM_AE | LVDS_C_FROM_BE | LVDS_D_FROM_BO,
	 0x00,			 // u1BacklightHigh
	 0x80,			 // u1BacklightMiddle
	 0xFF,			 // u1BacklightLow
	 0, 			 // u1Reserved
	 0, 			 // u2DimmingFrequency60Hz
	 0, 			 // u2DimmingFrequency50Hz
	 1, 			 // u1LvdsOnDalay
	 20,			 // u1BacklightOnDelay
	 20,			 // u1BacklightOffDelay
	 1, 			 // u1LvdsOffDalay
	 50,			 // u2MaxOverscan
	 0, 			 // u2Reserved
   },
  #endif
#if ((PANEL_SELECT == PANEL_LVDS_TO_8282) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 59630000,   // u4PixelClkMin

    2560,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2120,           // u2HTotalMin

    1201,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1125,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_SS_58_PDP) || CC_MULTI_PANEL) // FHD-60Hz-2D also define as FHD-60Hz-PR
    {
    1920,           // u2PanelWidth
    2 * 1080,       // u2PanelHeight

    4 * 76250000,   // u4PixelClkMax
    4 * 74250000,   // u4PixelClk60Hz
    4 * 74250000,   // u4PixelClk50Hz
    4 * 72250000,   // u4PixelClkMin

    2360,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2000,           // u2HTotalMin

    2 * 1500,       // u2VTotalMax
    2 * 1125,       // u2VTotal60Hz
    2 * 1350,       // u2VTotal50Hz
    2 * 1089,       // u2VTotalMin
    124,            // u1VClkMax...spec not defined
    90,             // u1VClkMin...spec not defined

    40,             // u1HSyncWidth
    2 * 5,          // u1VSyncWidth
    0x858,          // u1HPosition
    (0x400 - 0x10*2),          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF | PANEL_PDP|
    PWM_HIGH_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_LRLR,
#else
    FOUR_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF | PANEL_PDP|
    PWM_HIGH_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_LRLR,
#endif

    0x00,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0xFF,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    50,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT ==  PANEL_EPI_LGDV11GIPFHD120) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2196,           // u2HTotal60Hz
    2196,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER | DISP_TYPE_EPI,
#else
    FOUR_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | DISP_TYPE_EPI,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_AUO_T315HB01_V1) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 82000000,   // u4PixelClkMax
    148500000,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    2 * 50000000,   // u4PixelClkMin

    2650, // 2649,           // u2HTotalMax
    2200, // 2199,           // u2HTotal60Hz
    2200, // 2199,           // u2HTotal50Hz
    2060, // 2059,           // u2HTotalMin

    1480, // 1479,           // u2VTotalMax
    1125, // 1124,           // u2VTotal60Hz
    1125, // 1124,           // u2VTotal50Hz
    1090, // 1089,           // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_RLRL,

    0x00,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0xFF,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    50,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    1,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_CMI_42_HW03_V3) || CC_MULTI_PANEL) // FHD-120Hz-2D also define as FHD-120Hz-SG
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 76000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax (display mode)
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_30BIT | LVDS_NS| LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER |
    LVDS_A_FROM_AO | LVDS_B_FROM_AE | LVDS_C_FROM_BE | LVDS_D_FROM_BO,
#else
    FOUR_PORT | DISP_30BIT | LVDS_NS | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT |
    LVDS_A_FROM_AO | LVDS_B_FROM_AE | LVDS_C_FROM_BE | LVDS_D_FROM_BO,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT ==  PANEL_EPI_LGDV12GIPFHD120) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 75000000,   // u4PixelClkMax
    296703297,      // u4PixelClk60Hz
    296703297,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2196,           // u2HTotal60Hz
    2196,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1380,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER | DISP_TYPE_EPI,
#else
    FOUR_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | DISP_TYPE_EPI,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_LG_32_PDP) || CC_MULTI_PANEL)
   {

    1366,           // u2PanelWidth
    2 * 768,            // u2PanelHeight

    2 * 80000000,       // u4PixelClkMax
    2 * 72354836,       // u4PixelClk60Hz
    2 * 71200000,       // u4PixelClk50Hz
    2 * 68000000,       // u4PixelClkMin

    1776,           // u2HTotalMax
    1528,           // u2HTotal60Hz
    1600,           // u2HTotal50Hz
    1456,           // u2HTotalMin

    2 * 1063,           // u2VTotalMax
    2 * 790,            // u2VTotal60Hz
    2 * 890,            // u2VTotal50Hz
    2 * 775,            // u2VTotalMin

	 63,			 // u1VClkMax
	 47,			 // u1VClkMin
	 10,			 // u1HSyncWidth
	 2 * 6, 			 // u1VSyncWidth
	 0x810, 		 // u1HPosition
	 0x400, 		 // u1VPosition
	 DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |PWM_LOW_PANEL_BRIGHT | PANEL_PDP
	 | LVDS_A_FROM_AE | LVDS_B_FROM_BO | LVDS_C_FROM_AO | LVDS_D_FROM_BE,
	 0x00,			 // u1BacklightHigh
	 0x80,			 // u1BacklightMiddle
	 0xFF,			 // u1BacklightLow
	 0, 			 // u1Reserved
	 0, 			 // u2DimmingFrequency60Hz
	 0, 			 // u2DimmingFrequency50Hz
	 1, 			 // u1LvdsOnDalay
	 20,			 // u1BacklightOnDelay
	 20,			 // u1BacklightOffDelay
	 1, 			 // u1LvdsOffDalay
	 50,			 // u2MaxOverscan
	 0, 			 // u2Reserved
   },
  #endif
#if ((PANEL_SELECT == PANEL_SS_L40V8200_120SG) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    310000000,   // u4PixelClkMax
    297000000,      // u4PixelClk60Hz
    297000000,      // u4PixelClk50Hz
    240000000,   // u4PixelClkMin

    2701,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2140,           // u2HTotalMin

    1601,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1105,           // u2VTotalMin
    125,            // u1VClkMax
    90,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_SHUTTER |
    LVDS_A_FROM_AO | LVDS_B_FROM_BO | LVDS_C_FROM_AE | LVDS_D_FROM_BE,

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    2,              // u1LvdsOnDalay
    30,             // u1BacklightOnDelay
    30,             // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif

#if ((PANEL_SELECT == PANEL_PDP_PD5L_XGA60) || CC_MULTI_PANEL)
	  {
	  1024, 		  // u2PanelWidth
	  768,			  // u2PanelHeight
  
	  90000000, 	  // u4PixelClkMax
	  81080160, 	  // u4PixelClk60Hz
	  80948000, 	  // u4PixelClk50Hz
	  70000000, 	  // u4PixelClkMin
  
	  2050, 		  // u2HTotalMax
	  1636, 		  // u2HTotal60Hz
	  1960, 		  // u2HTotal50Hz
	  1414, 		  // u2HTotalMin
	  
	  1063, 		  // u2VTotalMax
	  826,			  // u2VTotal60Hz
	  826,			  // u2VTotal50Hz
	  789,			  // u2VTotalMin
	  63,			  // u1VClkMax
	  48,			  // u1VClkMin
  
	  16,			  // u1HSyncWidth
	  2,			  // u1VSyncWidth
	  0xA34,		  // u1HPosition
	  0x400,		  // u1VPosition
	  SINGLE_PORT | DISP_30BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
	  PWM_LOW_PANEL_BRIGHT,
  
	  0x60, 		  // (100%) u1BacklightHigh
	  0x80, 		  // (67%) u1BacklightMiddle
	  0xA0, 		  // (50%) u1BacklightLow
	  0,			  // u1Reserved
	  120,				// u2DimmingFrequency60Hz
	  100,				// u2DimmingFrequency50Hz
  
	  3,			  // u1LvdsOnDalay
	  60,			  // u1BacklightOnDelay
	  5,			  // u1BacklightOffDelay
	  3,			  // u1LvdsOffDalay
  
	  30,			  // u2MaxOverscan
	  0,			  // u2Reserved
	  },
#endif
  
};

static PANEL_LOCAL_DIMMING_ATTRIBUTE_T _arPanelLDAttribute[]=
{
  #if ((PANEL_SELECT == PANEL_AUO_42_HW1_V3) || CC_MULTI_PANEL)
	  {
        PANEL_AUO_42_HW1_V3,
        E_SPI_AMS_PROTOCOL,                 // UINT8  u1ProtocolIndex;
        SV_ON,                             // UINT8 u1SWModeEn;
        SV_ON,                             // UINT8  u1ScanningOnoff;
        0,

        // clock configuration
        600000,                             // UINT32 u4OutputClock;      // SPI output clock , unit is Hz
        CPOL_OFF,                           // UINT8  u1CPOL;             // clock polarity
        CPHA_OFF,                           // UINT8  u1CPHA;             // clock phase

        // Vsync access timing control
        540,                                 // UINT8  u1VSyncHighTime;    // Vsync high time ,unit is us
        540,                                 // UINT8  u1WaitTime;         // wait time from Vsync low to CS low or data , unit is us

        // CS access timing control
        14800,                              // UINT16 u2CSHighTime;       // output CS hold time, unit is ns
        14,                                 // UINT8  u1Tcld;             // setup time from CS low to SCLK edge , unit is ns
        14,                                 // UINT8  u1Tclg;             // hold time from SCLK edge to CS high, unit is ns
        SV_OFF,                             // UINT8  u1HheadSingleEn;    // CS setting
        SV_OFF,                             // UINT8  u1HdataSingleEn;
        SV_OFF,                             // UINT8  u1HfootSingleEn;
        SV_OFF,                             // UINT8  u1VheadSingleEn;
        SV_ON,                              // UINT8  u1VdataSingleEn;
        SV_OFF,                             // UINT8  u1VfootSingleEn;

        // LED output format
        LD_FMT_DEPTH_8BIT,                  // UINT8 u1BitMode;           // bits in dimming data, 0:8bit , 1:10bit , 2:12bit ,  3:7bit

        0,     //UINT16 u2DimmingLowBound;
        995,                                // UINT16 u2HistMaxThd;         // set histogram threshold to 99.5%
        2,     //H block number
        8,     //V block number
	   },
  #endif
};


static PANEL_MINILVDS_ATTRIBUTE_T _arPanelMiniLVDSAttribute[]=
{
  #if ((PANEL_SELECT == PANEL_MLVDS_LGDV4FHD60) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_LGDV4FHD60,
		   DUAL_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_LGDV4FHD120) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_LGDV4FHD120,
		   FOUR_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_LGDV4GIPFHD60) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_LGDV4GIPFHD60,
		   DUAL_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_LGDV4GIPFHD120) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_LGDV4GIPFHD120,
		   FOUR_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_LGDV5GIPFHD60) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_LGDV5GIPFHD60,
		   DUAL_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_LGDV5GIPFHD120) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_LGDV5GIPFHD120,
		   FOUR_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_LGDV6GIPFHD60) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_LGDV6GIPFHD60,
		   DUAL_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_LGDV6GIPFHD120) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_LGDV6GIPFHD120,
		   FOUR_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_AUOFHD60) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_AUOFHD60,
		   DUAL_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_AUOFHD120) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_AUOFHD120,
		   FOUR_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_SHAFHD60) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_SHAFHD60,
		   DUAL_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_SHAFHD120) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_SHAFHD120,
		   FOUR_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_FUNAWXGA) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_FUNAWXGA,
		   SINGLE_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_FUNAFHD60) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_FUNAFHD60,
		   DUAL_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_HIWXGA09) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_HIWXGA09,
		   SINGLE_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_HIWXGA10) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_HIWXGA10,
		   SINGLE_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_WXGA) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_WXGA,
		   SINGLE_PORT,
		   TCLK_PCLK_RATION_1,
		 },
   #endif
  #if ((PANEL_SELECT == PANEL_MLVDS_CMO2011FHD60) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_CMO2011FHD60,
		   DUAL_PORT,
		   TCLK_PCLK_RATION_1,
		 },
  #endif
#if ((PANEL_SELECT == PANEL_MLVDS_PANASONIC32WXGA8BIT) || CC_MULTI_PANEL)
		 {
		   PANEL_MLVDS_PANASONIC32WXGA8BIT,
		   SINGLE_PORT,
		   TCLK_PCLK_RATION_2,
		 },
#endif

};



#ifdef LOAD_PANEL_FROM_FLASH_CUSTOM
UINT8 au1FlashPanelTableVersion[16];
PANEL_HEADER_CUSTOM_ATTRIBUTE_T rFlashPanelHeader;
PANEL_Ext2_CUSTOM_ATTRIBUTE_T rFlashPanelExt2;

CHAR * FlashPanel_GetVersion(void)
{
    return "Unknow";
}

CHAR * FlashPanel_GetIDString(void)
{
    return "Unknow";
}

UINT32 FlashPanel_GetDither(void)
{
    return 0;
}

UINT32 FlashPanel_GetAQIndex(UINT32 u4Idx)
{
    return 0;
}

UINT32 FlashPanel_GetPQIndex(UINT32 u4Idx)
{
    return 0;
}

BOOL MTK_PanelLoadTableFromFlash(UINT32 u4Type, void *pData)
{
    if (pData == NULL)
    {
        return FALSE;
    }

    return TRUE;
}
#endif

#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
//return num*10; for 120Hz panel
UINT32 PANEL_GetFrameDelay(void)
{
    return 0;
}
#endif

CHAR* GetPanelName(UINT32 u4Index)
{
    switch (u4Index)
    {
    case PANEL_XGA: return "PANEL_XGA(PANEL_DEFAULT)";
    case PANEL_SXGA: return "PANEL_SXGA";

    case PANEL_CMO_154_C1_P1: return "PANEL_CMO_154_C1_P1";
    case PANEL_CMO_154_I2_L2: return "PANEL_CMO_154_I2_L2";
    case PANEL_CMO_154_I2_P1: return "PANEL_CMO_154_I2_P1";
    case PANEL_CMO_154_I2_P2: return "PANEL_CMO_154_I2_P2";
    case PANEL_CMO_154_I5_P2: return "PANEL_CMO_154_I5_P2";
    case PANEL_CMO_19_A1: return "PANEL_CMO_19_A1";
    case PANEL_CMO_19_A1_P2: return "PANEL_CMO_19_A1_P2";
    case PANEL_CMO_22_Z1: return "PANEL_CMO_22_Z1";
    case PANEL_CMO_22_Z1_P1: return "PANEL_CMO_22_Z1_P1";
    case PANEL_CMO_26_B1: return "CMO_V260B1_L01";
    case PANEL_CMO_26_B1_L04: return "CMO_V260B1_L01_L04 (TEST)";
    case PANEL_CMO_27_W1: return "PANEL_CMO_27_W1";
    case PANEL_CMO_27_B1: return "PANEL_CMO_27_B1";
    case PANEL_CMO_315_B1: return "CMO_V315B1_L01";
    case PANEL_CMO_32_B1: return "CMO_V320B1_L04";
    case PANEL_CMO_37_H1: return "PANEL_CMO_37_H1";
    case PANEL_CMO_42_H1: return "PANEL_CMO_42_H1";
    case PANEL_CMO_42_H1_L5: return "CMO_V420H1_L05";
    case PANEL_CMO_42_H1_L8: return "CMO V420H1_L08";
    case PANEL_CMO_42_H1_L5_DELTA: return "CMO_V420H1_L05_DELTA (TEST)";

    case PANEL_AUO_20_XW2: return "PANEL_AUO_20_XW2";
    case PANEL_AUO_26_XW3: return "PANEL_AUO_26_XW3";
    case PANEL_AUO_315_XW1_VH: return "AUO_T315XW01_VH";
    case PANEL_AUO_315_XW1_VG: return "AUO_T315XW01_VG";
    case PANEL_AUO_315_XW2_V0: return "AUO_T315XW02_V0";
    case PANEL_AUO_315_XW2_V1: return "AUO_T315XW02_V1";
    case PANEL_AUO_315_XW2_V3: return "AUO_T315XW02_V3";
    case PANEL_AUO_315_XW2_V6: return "PANEL_AUO_315_XW2_V6";
    case PANEL_AUO_32_XW1: return "AUO_T320XW01_V5";
    case PANEL_AUO_32_XW2: return "PANEL_AUO_32_XW2";
    case PANEL_AUO_37_XW1_V1: return "AUO_T370XW01_V1";
    case PANEL_AUO_37_XW1_V2: return "AUO_T370XW01_V2";
    case PANEL_AUO_37_XW2: return "AUO_T370XW01_V0";
    case PANEL_AUO_37_XW2_V5: return "PANEL_AUO_37_XW2_V5";
    case PANEL_AUO_37_HW1: return "PANEL_AUO_37_HW1";
    case PANEL_AUO_40_XW1_V0: return "AUO_T400XW01_V0 (TEST)";
    case PANEL_AUO_42_HW1_V1: return "PANEL_AUO_42_HW1_V1 (TEST)";
    case PANEL_AUO_42_XW1_V3: return "AUO_T420XW01_V3";
    case PANEL_AUO_42_HW1_V0: return "PANEL_AUO_42_HW1_V0";
    case PANEL_AUO_42_HW1_V3: return "PANEL_AUO_42_HW1_V3";

    case PANEL_LG_26_W1: return "PANEL_LG_26_W1";
    case PANEL_LG_26_WX2: return "PANEL_LG_26_WX2";
    case PANEL_LG_32_W1: return "PANEL_LG_32_W1";
    case PANEL_LG_32_WX3_SLB1: return "PANEL_LG_32_WX3_SLB1";
    case PANEL_LG_37_WX1: return "PANEL_LG_37_WX1";
    case PANEL_LG_37_WX1_SL2: return "PANEL_LG_37_WX1_SL2";
    case PANEL_LG_37_WX1_SLA1: return "PANEL_LG_37_WX1_SLA1";
    case PANEL_LG_37_WU1: return "PANEL_LG_37_WU1";
    case PANEL_LG_42_W2: return "PANEL_LG_42_W2";
    case PANEL_LG_42_W2_SLA2: return "PANEL_LG_42_W2_SLA2";
    case PANEL_LG_42_WU2: return "PANEL_LG_42_WU2";
    case PANEL_LG_42_WU3_SLA1: return "PANEL_LG_42_WU3_SLA1";
    case PANEL_LG_42_WU5_SLB1: return "PANEL_LG_42_WU5_SLB1";
    case PANEL_LG_42_WX4_SLB1: return "PANEL_LG_42_WX4_SLB1";
    case PANEL_LG_47_WU4: return "PANEL_LG_47_WU4";
    case PANEL_LG_50_X4_PDP: return "PANEL_LG_50_X4_PDP";

    case PANEL_SS_19_M2: return "PANEL_SS_19_M2";
    case PANEL_SS_23_W2_L1: return "PANEL_SS_23_W2_L1";
    case PANEL_SS_32_WT_L5: return "PANEL_SS_32_WT_L5";
    case PANEL_SS_40_WT: return "SAMSUNG_LTA400WT_L11";
    case PANEL_SS_40_FHD: return "SAMSUNG_SS_40_FHD";
    case PANEL_SS_40_FHD_10BIT: return "SAMSUNG_SS_40_FHD_10BIT";
    case PANEL_SS_40_WT_L17: return "SAMSUNG_LTA400WT_L17 (TEST)";
    case PANEL_SS_40_HH_LH2:        return "SAMSUNG_LTA400HH_LH2";
    case PANEL_SS_46_HS_L03: return "SAMSUNG_LTA460HS_L03";
    case PANEL_SS_46_HH_L01: return "SAMSUNG_LTA460HH_L01";
    case PANEL_SS_46_HS_L03_BESTBUY: return "SAMSUNG_LTA460HS_L03_BESTBUY (TEST)";

    case PANEL_CLAA_15_XP3: return "PANEL_CLAA_15_XP3";
    case PANEL_CLAA_32_WB2: return "PANEL_CLAA_32_WB2";

    case PANEL_HSD_19_MGW1: return "PANEL_HSD_19_MGW1";
    case PANEL_QD_32_HL1: return "AUO_T315XW01_VC(T)";
    case PANEL_QD_32_HL1_W: return "AUO_T315XW01_VC(W)";

    case PANEL_PV_154_LCM_C01: return "PANEL_PV_154_LCM_C01";
    case PANEL_PV_154_LCM_C03: return "PANEL_PV_154_LCM_C03";
    case PANEL_PV_154_LCM_C04: return "PANEL_PV_154_LCM_C04";
    case PANEL_PV_154_LCM_C05: return "PANEL_PV_154_LCM_C05";
    case PANEL_PV_201_TVM_C01: return "PANEL_PV_201_TVM_C01";
    case PANEL_PV_260_TVM_A01H: return "Proview_PV260TVM_A01H (TEST)";
    case PANEL_PV_260_TVM_A04H: return "Proview_PV260TVM_A04H (TEST)";
    case PANEL_PV_260_TVM_A11H: return "Proview_PV260TVM_A11H (TEST)";
    case PANEL_PV_260_TVM_C01H: return "Proview_PV260TVM_C01H (TEST)";
    case PANEL_PV_260_TVM_C11H: return "Proview_PV260TVM_C11H (TEST)";
    case PANEL_PV_320_TVM_A01H: return "Proview_PV320TVM_A01H";
    case PANEL_PV_320_TVM_A02H: return "Proview_PV320TVM_A02H";
    case PANEL_PV_320_TVM_A03H: return "Proview_PV320TVM_A03H (TEST)";
    case PANEL_PV_320_TVM_A12H: return "Proview_PV320TVM_A12H (TEST)";
    case PANEL_PV_320_TVM_A21H: return "Proview_PV320TVM_A21H (TEST)";
    case PANEL_PV_320_TVM_C01:  return "Proview_PV320TVM_C01";
    case PANEL_PV_320_TVM_C02H: return "Proview_PV320TVM_C02H (TEST)";
    case PANEL_PV_320_TVM_C12H: return "Proview_PV320TVM_C12H";

    case PANEL_INNOLUX_19_AW01: return "PANEL_INNOLUX_19_AW01";

    case PANEL_AUO_315_XW2_V6_8280: return "PANEL_AUO_315_XW2_V6_8280";
    case PANEL_LG_42_WU5_SLB1_8280: return "PANEL_LG_42_WU5_SLB1_8280";
    case PANEL_LG_42_WX4_SLB1_8280: return "PANEL_LG_42_WX4_SLB1_8280";


    case PANEL_FHD60_LVDS_CAP: 	    return "PANEL_FHD60_LVDS_CAP";
    case PANEL_FHD120_LVDS_CAP:     return "PANEL_FHD120_LVDS_CAP";
    case PANEL_WXGA60_LVDS_CAP:     return "PANEL_WXGA60_LVDS_CAP";
    case PANEL_WXGA120_LVDS_CAP:    return "PANEL_WXGA120_LVDS_CAP";
    case PANEL_SLT_FHD_60Hz_10BIT:       return "PANEL_SLT_FHD_60Hz_10BIT";
    case PANEL_SLT_FHD_120Hz_10BIT:    return "PANEL_SLT_FHD_120Hz_10BIT";

    //For SBTVD
    case PANEL_INNOLUX_19_AW02_LIPS_LOW:	return "PANEL_INNOLUX_19_AW02_LIPS_LOW";
    case PANEL_CMO_216_B1_L01_LIPS_LOW:	return "PANEL_CMO_216_B1_L01_LIPS_LOW";
    case PANEL_CMO_260_B1_L11_LIPS_HIGH:	return "PANEL_CMO_260_B1_L11_LIPS_HIGH";

    case PANEL_CMO_216_B1_L1: return "PANEL_CMO_216_B1_L1";
    case PANEL_AUO_19_PW1: return "PANEL_AUO_19_PW1";
    case PANEL_AUO_19_XW1: return "PANEL_AUO_19_XW1";                    //sammi modifed 080912
    case PANEL_CMO_22_B1_L01: return "PANEL_CMO_22_B1_L01";            //sammi modified 080912
    case PANEL_AUO_37_XW2_VC: return "PANEL_AUO_37_XW2_VC";         //sammi modified 080912
    case PANEL_AUO_26_WT_1: return "PANEL_AUO_26_WT_1"; // add by stanley 080908
    case PANEL_SS_32_WT_1: return "PANEL_SS_32_WT_1";	// add by stanley 080908
    case PANEL_SS_52_FHD: return "SAMSUNG_SS_52_FHD";
    case PANEL_SS_32_WT_1_WXGA2: return "PANEL_SS_32_WT_1_WXGA2";
    case PANEL_AUO_315_XW2_VS_WXGA2: return "PANEL_AUO_315_XW2_VS_WXGA2";
    case PANEL_SS_32NC_LBA_SESL_WXGA1: return "PANEL_SS_32NC_LBA_SESL_WXGA1"; // WXGA1
    case PANEL_SS_32NC_LBA_SESL_WXGA2: return "PANEL_SS_32NC_LBA_SESL_WXGA2"; // WXGA2
    case PANEL_AUO_T315XW02_VS_WXGA1: return "PANEL_AUO_T315XW02_VS_WXGA1";// WXGA1
    case PANEL_AUO_T315XW02_VS_WXGA2: return "PANEL_AUO_T315XW02_VS_WXGA2"; // WXGA2
    case PANEL_SS_32NC_LBA_SESL_FHD: return "PANEL_SS_32NC_LBA_SESL_FHD";
    case PANEL_AUO_46_HW03_V4: return "PANEL_AUO_46_HW03_V4"; // WXGA2 // 1920x1080 (240Hz for MT5395+2xMT8282)
    case PANEL_AUO_65_HW02_V1: return "PANEL_AUO_65_HW02_V1"; // WXGA2 // 1920x1080 (120Hz PR Panel)

    case PANEL_LG_42_WUD_SAC1_10BIT: return "PANEL_LG_42_WUD_SAC1_10BIT";

    case PANEL_LG_42_WU5_SLB1_VB1: return "PANEL_LG_42_WU5_SLB1_VB1";

    case PANEL_MLVDS_LGDV4FHD60: return "PANEL_MLVDS_LGDV4FHD60";
    case PANEL_MLVDS_LGDV4FHD120: return "PANEL_MLVDS_LGDV4FHD120";
    case PANEL_MLVDS_LGDV4GIPFHD60: return "PANEL_MLVDS_LGDV4GIPFHD60";
    case PANEL_MLVDS_LGDV4GIPFHD120: return "PANEL_MLVDS_LGDV4GIPFHD120";
    case PANEL_ChangHong_PDP: return "PANEL_ChangHong_PDP";
    case PANEL_MLVDS_LGDV5GIPFHD60: return "PANEL_MLVDS_LGDV5GIPFHD60";
    case PANEL_MLVDS_LGDV5GIPFHD120: return "PANEL_MLVDS_LGDV5GIPFHD120";
    case PANEL_MLVDS_LGDV6GIPFHD60: return "PANEL_MLVDS_LGDV6GIPFHD60";
    case PANEL_MLVDS_LGDV6GIPFHD120: return "PANEL_MLVDS_LGDV6GIPFHD120";
    case PANEL_MLVDS_AUOFHD60: return "PANEL_MLVDS_AUOFHD60";
    case PANEL_MLVDS_AUOFHD120: return "PANEL_MLVDS_AUOFHD120";
    case PANEL_MLVDS_SHAFHD60: return "PANEL_MLVDS_SHAFHD60";
    case PANEL_MLVDS_SHAFHD120: return "PANEL_MLVDS_SHAFHD120";
    case PANEL_MLVDS_FUNAWXGA: return "PANEL_MLVDS_FUNAWXGA";
    case PANEL_MLVDS_FUNAFHD60: return "PANEL_MLVDS_FUNAFHD60";
    case PANEL_MLVDS_HIWXGA09: return "PANEL_MLVDS_HIWXGA09";
    case PANEL_MLVDS_HIWXGA10: return "PANEL_MLVDS_HIWXGA10";
    case PANEL_MLVDS_WXGA: return "PANEL_MLVDS_WXGA";
    case PANEL_MLVDS_CMO2011FHD60: return "PANEL_MLVDS_CMO2011FHD60";
    case PANEL_MLVDS_PANASONIC32WXGA8BIT: return "PANEL_MLVDS_PANASONIC32WXGA8BIT";
    case PANEL_LG_47_EUH_PR: return "PANEL_LG_47_EUH_PR";
    case PANEL_CMO_M236H5_L0A: return "PANEL_CMO_M236H5_L0A";
    case PANEL_LG_42_WUD_SAC1_10BIT_NS: return "PANEL_LG_42_WUD_SAC1_10BIT_NS";
    case PANEL_LG_42_WU5_SLB1_PR: return "PANEL_LG_42_WU5_SLB1_PR";
    case PANEL_LG_42_WU5_SLB1_SG: return "PANEL_LG_42_WU5_SLB1_SG";
    case PANEL_CMO_M236H3_LHH3:return "PANEL_CMO_M236H3_LHH3";
    case PANEL_LG_42_WU5_SLB1_AS3693: return "PANEL_LG_42_WU5_SLB1_AS3693";
    case PANEL_LG_42_WU5_SLB1_IWATT: return"PANEL_LG_42_WU5_SLB1_IWATT";
	case PANEL_LG_42_WU5_SLB1_SG_BEX: return "PANEL_LG_42_WU5_SLB1_SG_BEX";
	case PANEL_AUO_46_HW03_V4_FHD240: return"PANEL_AUO_46_HW03_V4_FHD240";
	case PANEL_AUO_46_HW03_V3: return"PANEL_AUO_46_HW03_V3";
	case PANEL_LG_50_CINEMA: return"PANEL_LG_50_CINEMA";
	case PANEL_FOR_8283: return "PANEL_FOR_8283";
	case PANEL_AUO_T576DC01_V1: return "PANEL_AUO_T576DC01_V1";
	case PANEL_CMI_V420H2_LS1: return "PANEL_CMI_V420H2_LS1";
	case PANEL_LC420EUD_SDF1_PR: return "PANEL_LC420EUD_SDF1_PR";
	case PANEL_AUO_T645HW05_V0: return "PANEL_AUO_T645HW05_V0";
	case PANEL_SS_42_PDP: return "PANEL_SS_42_PDP";
    case PANEL_LVDS_TO_8282: return "PANEL_LVDS_TO_8282";
	case PANEL_SS_58_PDP: return "PANEL_SS_58_PDP";
	case PANEL_EPI_LGDV11GIPFHD120: return "PANEL_EPI_LGDV11GIPFHD120";
	case PANEL_AUO_T315HB01_V1: return "PANEL_AUO_T315HB01_V1";
	case PANEL_CMI_42_HW03_V3: return "PANEL_CMI_42_HW03_V3";
	case PANEL_EPI_LGDV12GIPFHD120: return "PANEL_EPI_LGDV12GIPFHD120";
	case PANEL_LG_32_PDP: return "PANEL_LG_32_PDP";			
	case PANEL_SS_L40V8200_120SG: return "PANEL_SS_L40V8200_120SG";	
	case PANEL_PDP_PD5L_XGA60: return "PANEL_PDP_PD5L_XGA60";		
    default: return "NO NAME";
    }
}

/*
static const UINT32 _au4PanelLvds10bitPinMap[24] =
{
 // Port A (PAD-A) LLV0~LLV9, LLK_CK0,LLK_CK1
 LVDS_A9, LVDS_A8, LVDS_ACLK2, LVDS_A6, LVDS_A5,
 LVDS_A4, LVDS_A3, LVDS_ACLK1, LVDS_A1, LVDS_A0,
 LVDS_A7, LVDS_A2,
 // Port B (PAD-B) RLV0~RLV9, RLK_CK0,RLK_CK1
 LVDS_ACLK2, LVDS_A8, LVDS_A9, LVDS_A6, LVDS_A5,
 LVDS_A4, LVDS_A3, LVDS_ACLK1, LVDS_A1, LVDS_A0,
 LVDS_A7, LVDS_A2
};

static const UINT32 _au4PanelLvds10bitPNSwap[24] =
{
 // Port A (PADA) RLV0~RLV9, LLK_CK0,LLK_CK1
 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1,
 1, 1,
 // Port B (PADB) RLV0~RLV9, RLK_CK0,RLK_CK1
 0, 1, 0, 1, 1,
 1, 0, 1, 1, 1,
 1, 1
};
*/

