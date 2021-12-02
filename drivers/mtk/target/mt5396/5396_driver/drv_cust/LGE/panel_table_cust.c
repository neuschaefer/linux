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
#include "x_pdwnc.h"

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
    UINT16 u2Dither;
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
    UINT16 u2Dither;
    #endif
    #ifdef SUPPORT_PANEL_DRIVING
    UINT8 u1DrivingCurrent;
    #endif

    #ifdef SUPPORT_PANEL_SS
    UINT8 u1SSPermillage;
    UINT32 u4SSFreq;
    #endif    
} PANEL_ATTRIBUTE_EXT_T;


#if defined(SUPPORT_PANEL_48HZ) || defined(SUPPORT_PANEL_CUSTOMER_SPEC) || defined(SUPPORT_PANEL_SS) || defined(SUPPORT_PANEL_DITHER) || defined(SUPPORT_PANEL_DRIVING)
static PANEL_ATTRIBUTE_EXT_T _arPanelAttributeExt[]=
{
    #if ((PANEL_SELECT == PANEL_LG_32_EUD_SDA1) || CC_MULTI_PANEL) 
    {
        PANEL_LG_32_EUD_SDA1,
        #ifdef SUPPORT_PANEL_48HZ
        297000000,                  //u4PixelClk48Hz
        2200,                      // u2HTotal48Hz
        1406,                    // u2VTotal48Hz
        144,
        #endif    
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        {32, 32, 32},              //u1HSyncWidth[3]; 5bits, 1 represents 1 pixel in 5387
        {4, 4, 4},                 // u1VSyncWidth[3]; 4bits, 1 represents 1 line
        {16, 16, 16},         // H front porch, 1 represents 1 pixel
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
        #ifdef SUPPORT_PANEL_SS
        20,
        0
        #endif
    },
    #endif
};
#endif //defined(SUPPORT_PANEL_48HZ) || defined(SUPPORT_PANEL_CUSTOMER_SPEC)


// NOTE! the configuration order must follow panel index order
static PANEL_ATTRIBUTE_T _arPanelAttribute[] =
{
#if ((PANEL_SELECT == PANEL_LG_32_EUD_SDA1) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    4 * 77500000,   // u4PixelClkMax
    297000000,      // u4PixelClk60Hz
    297000000,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2720,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2080,           // u2HTotalMin

    1406,           // u2VTotalMax (display mode)
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1089,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    FOUR_PORT | DISP_30BIT | LVDS_NS | LVDS_ODD_SW_ON |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_LRLR ,
#else
    FOUR_PORT | DISP_30BIT | LVDS_NS | LVDS_ODD_SW_ON |
    WFB_PANEL_RESET_ON | PWM_LOW_PANEL_BRIGHT,
#endif

    0x08,           // u1BacklightHigh
    0x80,           // u1BacklightMiddle
    0x90,           // u1BacklightLow
    0,              // u1Reserved
    120,              // u2DimmingFrequency60Hz
    100,              // u2DimmingFrequency50Hz

    0,              // u1LvdsOnDalay
    0,             // u1BacklightOnDelay
    0,             // u1BacklightOffDelay
    0,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
#if ((PANEL_SELECT == PANEL_AUO_42LV3700_ZA) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    2 * 77000000,   // u4PixelClkMax
    148500000,      // u4PixelClk60Hz
    148500000,      // u4PixelClk50Hz
    2 * 70000000,   // u4PixelClkMin

    2559,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2200,           // u2HTotal50Hz
    2119,           // u2HTotalMin

    1200,         // For Dispmode, original:  1148,           // u2VTotalMax
    1125,           // u2VTotal60Hz
    1350,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x810,          // u1HPosition
    0x400,          // u1VPosition
#ifdef CC_SCPOS_3DTV_SUPPORT
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_ON |
    PWM_LOW_PANEL_BRIGHT | LVDS_DISP_3D_POLARIZED_LRLR,
#else
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_ON |
    PWM_LOW_PANEL_BRIGHT,
#endif
    0x23, // (100%) u1BacklightHigh     // u1BacklightHigh
    0x7D, // (67%) u1BacklightMiddle    // u1BacklightMiddle
    0xC5, // (50%) u1BacklightLow       // u1BacklightLow
    0,              // u1Reserved
    0,              // u2DimmingFrequency60Hz
    0,              // u2DimmingFrequency50Hz

    0,              // u1LvdsOnDalay
    0,             // u1BacklightOnDelay
    0,             // u1BacklightOffDelay
    0,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
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
};


static PANEL_LOCAL_DIMMING_ATTRIBUTE_T _arPanelLDAttribute[]=
{
  #if ((PANEL_SELECT == PANEL_LG_32_EUD_SDA1) || CC_MULTI_PANEL) 
	  {
        PANEL_LG_32_EUD_SDA1,
        E_SPI_LGD_PROTOCOL,                 // UINT8  u1ProtocolIndex;     
        SV_ON,                             // UINT8 u1SWModeEn;        
        SV_OFF,                             // UINT8  u1ScanningOnoff;

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
        2,     //H block number
        8,     //V block number
	   },
  #endif    
  #if ((PANEL_SELECT == PANEL_EPI_LGDV12GIPFHD120) || CC_MULTI_PANEL) 
	  {
        PANEL_EPI_LGDV12GIPFHD120,
        E_SPI_LGD_PROTOCOL,                 // UINT8  u1ProtocolIndex;     
        SV_ON,                             // UINT8 u1SWModeEn;        
        SV_OFF,                             // UINT8  u1ScanningOnoff;

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
        2,     //H block number
        8,     //V block number
	   },
  #endif      
};

        

static PANEL_MINILVDS_ATTRIBUTE_T _arPanelMiniLVDSAttribute[]=
{
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
    case PANEL_LG_32_EUD_SDA1: return "PANEL_LG_32_EUD_SDA1";
    case PANEL_AUO_42LV3700_ZA: return "PANEL_AUO_42LV3700_ZA";
    case PANEL_EPI_LGDV12GIPFHD120: return "PANEL_EPI_LGDV12GIPFHD120";

    default: return "NO NAME";
    }
}

// Set Pair swap table
static const UINT32 _au4PanelLvds10bitPinMap_LCD[24] =
{
    // Port A (PADA) LLV0~LLV9, LLK_CK0,LLK_CK1
    LVDS_A9, LVDS_A8, LVDS_ACLK2, LVDS_A6, LVDS_A5,
    LVDS_A4, LVDS_A3, LVDS_ACLK1, LVDS_A1, LVDS_A0,
    LVDS_A7, LVDS_A2, 

    // Port B (PADB) RLV0~RLV9, RLK_CK0,RLK_CK1
    LVDS_ACLK2, LVDS_A8, LVDS_A9, LVDS_A6, LVDS_A5,
    LVDS_A3, LVDS_A4, LVDS_ACLK1, LVDS_A1, LVDS_A0,
    LVDS_A7, LVDS_A2
};

static const UINT32 _au4PanelLvds10bitPinMap_PDP[24] =
{
    // Port A (PADA) LLV0~LLV9, LLK_CK0,LLK_CK1
    LVDS_A0, LVDS_A1, LVDS_A2, LVDS_A3, LVDS_A4,
    LVDS_A5, LVDS_A6, LVDS_A7, LVDS_A8, LVDS_A9,
    LVDS_ACLK1, LVDS_ACLK2, 

    // Port B (PADB) RLV0~RLV9, RLK_CK0,RLK_CK1
    LVDS_A5, LVDS_A6, LVDS_A7, LVDS_A8, LVDS_ACLK2,
    LVDS_A0, LVDS_A1, LVDS_A2, LVDS_A4, LVDS_A3,
    LVDS_A9, LVDS_ACLK1
};


// Set Pin swap table
static const UINT32 _au4PanelLvds10bitPNSwap_LCD[24] =
{
    // Port A (PADA) RLV0~RLV9, LLK_CK0,LLK_CK1
    0, 1, 0, 1, 1,
    0, 1, 1, 1, 1,
    1, 1, 

    // Port B (PADB) RLV0~RLV9, RLK_CK0,RLK_CK1
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1
};

static const UINT32 _au4PanelLvds10bitPNSwap_PDP[24] =
{
    // Port A (PADA) RLV0~RLV9, LLK_CK0,LLK_CK1
    1, 0, 1, 0, 0,
    1, 0, 0, 0, 0,
    0, 0, 

    // Port B (PADB) RLV0~RLV9, RLK_CK0,RLK_CK1
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0
};

static UINT32 *_pu4PanelLvds10bitPinMap = (UINT32*)_au4PanelLvds10bitPinMap_LCD;
static UINT32 *_pu4PanelLvds10bitPNSwap = (UINT32*)_au4PanelLvds10bitPNSwap_LCD;

#define MODEL_OPT_10 142
void PANEL_SetDisplayType(DISPLAY_TYPE_T display_type)
{
    //support EPI panel
    INT32 i4sel;

    switch(display_type)
    {
        case PLASMA_DISPLAY:
            _pu4PanelLvds10bitPinMap = (UINT32*)_au4PanelLvds10bitPinMap_PDP;
            _pu4PanelLvds10bitPNSwap = (UINT32*)_au4PanelLvds10bitPNSwap_PDP;
        break;
        default:
        case LCD_DISPLAY:
            _pu4PanelLvds10bitPinMap = (UINT32*)_au4PanelLvds10bitPinMap_LCD;
            _pu4PanelLvds10bitPNSwap = (UINT32*)_au4PanelLvds10bitPNSwap_LCD;
        break;
    }

    //support EPI panel
    i4sel = 0;

    GPIO_Enable(MODEL_OPT_10, &i4sel); /* Change to input mode first. */
    i4sel = GPIO_Input(MODEL_OPT_10);
    if(i4sel)
    {
        SelectPanel(2); //select EPI panel
    }
    return;
} 

