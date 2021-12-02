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
// NOTE! the configuration order must follow panel index order
static PANEL_ATTRIBUTE_T _arPanelAttribute[] =
{
#if ((PANEL_SELECT == PANEL_XGA) || CC_MULTI_PANEL) // refer to CLAA150XP03
    {
    1024,           // u2PanelWidth
    768,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    64930901,       // u4PixelClk60Hz
    //54163200,       // u4PixelClk50Hz...to display 50Hz input in VGA monitor
    64930901,       // u4PixelClk50Hz
    50000000,       // u4PixelClkMin

    1369,           // u2HTotalMax
    1343,           // u2HTotal60Hz
    1343,           // u2HTotal50Hz
    1149,           // u2HTotalMin

    859,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    793,            // u2VTotalMin
    75,             // u1VClkMax
    55,             // u1VClkMin

    30,             // u1HSyncWidth
    6,              // u1VSyncWidth
    0x800,          // u1HPosition
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
#if ((PANEL_SELECT == PANEL_SXGA) || CC_MULTI_PANEL) // no spec, not recommed
    {
    1280,           // u2PanelWidth
    1024,           // u2PanelHeight

    120000000,      // u4PixelClkMax
    107856610,      // u4PixelClk60Hz
    89970400,       // u4PixelClk50Hz
    88000000,       // u4PixelClkMin

    1800,           // u2HTotalMax
    1687,           // u2HTotal60Hz
    1687,           // u2HTotal50Hz
    1149,           // u2HTotalMin

    1100,           // u2VTotalMax
    1065,           // u2VTotal60Hz
    1065,           // u2VTotal50Hz
    900,            // u2VTotalMin
    75,             // u1VClkMax
    55,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    88807193,       // u4PixelClk50Hz  //74080000,       // u4PixelClk50Hz
    2 * 36489600,   // u4PixelClkMin...spec not defined

    1919,           // u2HTotalMax
    1599,           // u2HTotal60Hz
    1919,           // u2HTotal50Hz
    1499,           // u2HTotalMin

    1049,           // u2VTotalMax
    925,            // u2VTotal60Hz
    925,            // u2VTotal50Hz
    904,            // u2VTotalMin
    75,             // u1VClkMax
    56,             // u1VClkMin

    30,              // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_NS | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh //    Norman confirmed.
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

    1999,           // u2HTotalMax
    1649,           // u2HTotal60Hz
    1799,           // u2HTotal50Hz
    1449,           // u2HTotalMin

    849,            // u2VTotalMax
    749,            // u2VTotal60Hz
    799,            // u2VTotal50Hz
    729,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,           // u1VPosition
    //245,          // u1HPosition...for scpos writeback
    //126,          // u1VPosition...for scpos writeback
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

    1935,           // u2HTotalMax
    1797,           // u2HTotal60Hz
    1797,           // u2HTotal50Hz
    1435,           // u2HTotalMin

    887,            // u2VTotalMax
    794,            // u2VTotal60Hz
    794,            // u2VTotal50Hz
    769,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1935,           // u2HTotalMax
    1797,           // u2HTotal60Hz
    1849,           // u2HTotal50Hz
    1441,           // u2HTotalMin

    887,            // u2VTotalMax
    794,            // u2VTotal60Hz
    819,            // u2VTotal50Hz
    769,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2299,           // u2HTotalMax
    2249,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2099,           // u2HTotalMin

//    1134,           // u2VTotalMax
    1200,           // u2VTotalMax (display mode)
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1114,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2299,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2099,           // u2HTotalMin

//    1138,           // u2VTotalMax
    1200,           // u2VTotalMax (display mode)
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1114,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1659,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    1062,           // u2VTotalMax, William for Display Mode
//    821,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh   // Norman confirmed.
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

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    887,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh   // Norman confirmed.
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

    2359,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    1999,           // u2HTotalMin

    1199,           // u2VTotalMax
    1129,           // u2VTotal60Hz
    1129,           // u2VTotal50Hz
    1087,           // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
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
#if ((PANEL_SELECT == PANEL_LG_26_W1) || CC_MULTI_PANEL)
    {
    1280,           // u2PanelWidth
    768,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    80012787,       // u4PixelClk60Hz
    66744000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin...spec not defined

    1847,           // u2HTotalMax...spec not defined
    1647,           // u2HTotal60Hz
    1647,           // u2HTotal50Hz
    1327,           // u2HTotalMin

    844,            // u2VTotalMax...spec not defined
    809,            // u2VTotal60Hz
    809,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    6,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1775,           // u2HTotalMax
    1527,           // u2HTotal60Hz
    1539,           // u2HTotal50Hz
    1415,           // u2HTotalMin

    1062,           // u2VTotalMax
    789,            // u2VTotal60Hz
    889,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_LOW_PANEL_BRIGHT,

    0x70,           // (100%) u1BacklightHigh
    0xAB,           // (67%) u1BacklightMiddle
    0xE6,           // (50%) u1BacklightLow
    0,              // u1Reserved
    0,//180,            // u2DimmingFrequency60Hz
    0,//150,            // u2DimmingFrequency50Hz

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

    1775,           // u2HTotalMax
    1527,           // u2HTotal60Hz
    1539,           // u2HTotal50Hz
    1471,           // u2HTotalMin

    1062,           // u2VTotalMax
    789,            // u2VTotal60Hz
    889,            // u2VTotal50Hz
    775,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2559,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2119,           // u2HTotalMin

    1200,         // For Dispmode, original:  1148,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1775,           // u2HTotalMax
    1527,           // u2HTotal60Hz
    1539,           // u2HTotal50Hz
    1455,           // u2HTotalMin

    1062,           // u2VTotalMax
    789,            // u2VTotal60Hz
    949,            // u2VTotal50Hz
    775,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2039,           // u2HTotalMax
    1599,           // u2HTotal60Hz
    1599,           // u2HTotal50Hz
    1449,           // u2HTotalMin

    1499,           // u2VTotalMax
    837,            // u2VTotal60Hz
    837,            // u2VTotal50Hz
    772,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1369,           // u2HTotalMax
    1343,           // u2HTotal60Hz
    1343,           // u2HTotal50Hz
    1149,           // u2HTotalMin

    859,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    793,            // u2VTotalMin
    75,             // u1VClkMax
    55,             // u1VClkMin

    30,             // u1HSyncWidth
    6,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    70000000,       // u4PixelClkMin...spec not defined

    1779,           // u2HTotalMax
    1647,           // u2HTotal60Hz
    1647,           // u2HTotal50Hz
    1511,           // u2HTotalMin

    1147,           // u2VTotalMax
    809,            // u2VTotal60Hz
    909,            // u2VTotal50Hz
    773,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1775,           // u2HTotalMax
    1527,           // u2HTotal60Hz
    1539,           // u2HTotal50Hz
    1455,           // u2HTotalMin

    1062,           // u2VTotalMax
    789,            // u2VTotal60Hz
    889,            // u2VTotal50Hz
    775,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    6,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1514,           // u2HTotalMax
    1479,           // u2HTotal60Hz
    1479,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    979,            // u2VTotalMax
    959,            // u2VTotal60Hz
    959,            // u2VTotal50Hz
    939,            // u2VTotalMin
    61,             // u1VClkMax
    49,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2559,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2119,           // u2HTotalMin

    1148,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1999,           // u2HTotalMax
    1839,           // u2HTotal60Hz
    1939,           // u2HTotal50Hz
    1779,           // u2HTotalMin

    1250,           // u2VTotalMax  (William for Display Mode)
//    1194,           // u2VTotalMax
    1079,           // u2VTotal60Hz
    1179,           // u2VTotal50Hz
    1059,           // u2VTotalMin
    76,             // u1VClkMax
    50,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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
//    2 * 48400000,   // u4PixelClkMin

    1919,           // u2HTotalMax
    1839,           // u2HTotal60Hz
    1899,           // u2HTotal50Hz
    1759,           // u2HTotalMin

    1062,           // u2VTotalMax  (William for Display Mode)
//    959,            // u2VTotalMax
    939,            // u2VTotal60Hz
    949,            // u2VTotal50Hz
    907,            // u2VTotalMin
    76,             // u1VClkMax
    56,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1935,           // u2HTotalMax
    1649,           // u2HTotal60Hz
    1649,           // u2HTotal50Hz
    1441,           // u2HTotalMin

    1062,           // u2VTotalMax, William for Display Mode
//    887,            // u2VTotalMax
    794,            // u2VTotal60Hz
    794,            // u2VTotal50Hz
    777,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2559,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2119,           // u2HTotalMin

//    1148,           // u2VTotalMax
    1200,           // u2VTotalMax (display mode)
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1104,           // u2VTotalMin
    53,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1935,           // u2HTotalMax
    1903,           // u2HTotal60Hz
    1903,           // u2HTotal50Hz
    1599,           // u2HTotalMin

    1000,           // u2VTotalMax
//    941,            // u2VTotalMax
    933,            // u2VTotal60Hz
    933,            // u2VTotal50Hz
    925,            // u2VTotalMin
    75,             // u1VClkMax
    60,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    887,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh   // Norman confirmed.
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

    1935,           // u2HTotalMax
    1797,           // u2HTotal60Hz
    1849,           // u2HTotal50Hz
    1441,           // u2HTotalMin

    887,            // u2VTotalMax
    794,            // u2VTotal60Hz
    819,            // u2VTotal50Hz
    777,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1544,           // u2HTotalMax
    1541,           // u2HTotal60Hz
    1541,           // u2HTotal50Hz
    1539,           // u2HTotalMin

    960,            // u2VTotalMax
    801,            // u2VTotal60Hz
    959,            // u2VTotal50Hz
    800,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    96,             // u1HSyncWidth
    10,             // u1VSyncWidth
    0x800,          // u1HPosition
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

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1699,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    821,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    887,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    887,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1759,           // u2HTotalMax
    1599,           // u2HTotal60Hz
    1599,           // u2HTotal50Hz
    1519,           // u2HTotalMin

    1499,           // u2VTotalMax
    925,            // u2VTotal60Hz
    925,            // u2VTotal50Hz
    909,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1749,           // u2HTotalMax
    1687,           // u2HTotal60Hz
    1687,           // u2HTotal50Hz
    1419,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    999,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    779,            // u2VTotalMin
    65,             // u1VClkMax...spec not defined
    55,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1599,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1495,           // u2HTotalMin

    979,            // u2VTotalMax
    779,            // u2VTotal60Hz
    959,            // u2VTotal50Hz
    775,            // u2VTotalMin
    121,             // u1VClkMax
    99,             // u1VClkMin
    //61,             // u1VClkMax
    //49,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1775,           // u2HTotalMax
    1527,           // u2HTotal60Hz
    1599,           // u2HTotal50Hz
    1455,           // u2HTotalMin

    1062,           // u2VTotalMax
    789,            // u2VTotal60Hz
    889,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1839,           // u2HTotalMax
    1527,           // u2HTotal60Hz
    1599,           // u2HTotal50Hz
    1445,           // u2HTotalMin

    1062,           // u2VTotalMax
    789,            // u2VTotal60Hz
    889,            // u2VTotal50Hz
    774,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1935,           // u2HTotalMax
    1647,           // u2HTotal60Hz
    1647,           // u2HTotal50Hz
    1574,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    887,            // u2VTotalMax
    809,            // u2VTotal60Hz
    809,            // u2VTotal50Hz
    789,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    54000000,       // u4PixelClkMin...spec not defined

    1599,           // u2HTotalMax
    1439,           // u2HTotal60Hz
    1439,           // u2HTotal50Hz
    1359,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    999,            // u2VTotalMax
    822,            // u2VTotal60Hz
    822,            // u2VTotal50Hz
    809,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    2 * 40000000,   // u4PixelClkMin...spec not defined

    1959,           // u2HTotalMax
    1687,           // u2HTotal60Hz
    1687,           // u2HTotal50Hz
    1479,           // u2HTotalMin

    1299,           // u2VTotalMax
    1065,           // u2VTotal60Hz
    1065,           // u2VTotal50Hz
    1050,           // u2VTotalMin
    75,             // u1VClkMax
    56,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1775,           // u2HTotalMax
    1527,           // u2HTotal60Hz
    1539,           // u2HTotal50Hz
    1471,           // u2HTotalMin

    1062,           // u2VTotalMax
    789,            // u2VTotal60Hz
    889,            // u2VTotal50Hz
    775,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    979,           // u2VTotalMax, Winton for display mode
//    822,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    62,             // u1VClkMax
    49,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    821,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    70000000,       // u4PixelClkMin...spec not defined

    1779,           // u2HTotalMax
    1647,           // u2HTotal60Hz
    1647,           // u2HTotal50Hz
    1511,           // u2HTotalMin

    1147,           // u2VTotalMax
    809,            // u2VTotal60Hz
    909,            // u2VTotal50Hz
    773,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1514,           // u2HTotalMax
    1479,           // u2HTotal60Hz
    1479,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    979,            // u2VTotalMax
    959,            // u2VTotal60Hz
    959,            // u2VTotal50Hz
    939,            // u2VTotalMin
    61,             // u1VClkMax
    49,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1514,           // u2HTotalMax
    1479,           // u2HTotal60Hz
    1479,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    979,            // u2VTotalMax
    959,            // u2VTotal60Hz
    959,            // u2VTotal50Hz
    939,            // u2VTotalMin
    61,             // u1VClkMax
    49,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1775,           // u2HTotalMax
    1527,           // u2HTotal60Hz
    1527,           // u2HTotal50Hz
    1455,           // u2HTotalMin

    1062,           // u2VTotalMax
    789,            // u2VTotal60Hz
    949,            // u2VTotal50Hz
    775,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    962,            // u2VTotalMax, Winton for Display Mode
//    821,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1899,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    1014,           // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    783,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2359,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2059,           // u2HTotalMin

    1199,           // u2VTotalMax
    1129,           // u2VTotal60Hz
    1129,           // u2VTotal50Hz
    1089,           // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2559,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2119,           // u2HTotalMin

//    1148,           // u2VTotalMax
    1200,           // u2VTotalMax (display mode)
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2299,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2099,           // u2HTotalMin

//    1138,           // u2VTotalMax
    1200,           // u2VTotalMax (display mode)
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1114,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1759,           // u2HTotalMax
    1599,           // u2HTotal60Hz
    1599,           // u2HTotal50Hz
    1519,           // u2HTotalMin

    1499,           // u2VTotalMax
    925,            // u2VTotal60Hz
    925,            // u2VTotal50Hz
    909,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    54000000,       // u4PixelClkMin...spec not defined

    1599,           // u2HTotalMax
    1439,           // u2HTotal60Hz
    1439,           // u2HTotal50Hz
    1359,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    999,            // u2VTotalMax
    822,            // u2VTotal60Hz
    822,            // u2VTotal50Hz
    809,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2649,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2159,           // u2HTotalMin

    1549,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1117,           // u2VTotalMin
    65,             // u1VClkMax
    45,             // u1VClkMin

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1999,           // u2HTotalMax
    1599,           // u2HTotal60Hz
    1917,           // u2HTotal50Hz
    1459,           // u2HTotalMin

    1199,           // u2VTotalMax, Winton for Display Mode
    837,            // u2VTotal60Hz
    837,            // u2VTotal50Hz
    772,            // u2VTotalMin
    66,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2299,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2099,           // u2HTotalMin

    1138,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1114,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2299,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2099,           // u2HTotalMin

//    1138,           // u2VTotalMax
    1200,           // u2VTotalMax (display mode)
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1114,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    54000000,       // u4PixelClkMin...spec not defined

    1599,           // u2HTotalMax
    1439,           // u2HTotal60Hz
    1439,           // u2HTotal50Hz
    1359,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    999,            // u2VTotalMax
    822,            // u2VTotal60Hz
    822,            // u2VTotal50Hz
    809,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    999,           // u2VTotalMax, Winton for Display Mode
//    887,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2039,           // u2HTotalMax
    1599,           // u2HTotal60Hz
    1599,           // u2HTotal50Hz
    1449,           // u2HTotalMin

    1499,           // u2VTotalMax
    837,            // u2VTotal60Hz
    837,            // u2VTotal50Hz
    772,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1779,           // u2HTotalMax
    1647,           // u2HTotal60Hz
    1647,           // u2HTotal50Hz
    1511,           // u2HTotalMin

    1147,           // u2VTotalMax
    809,            // u2VTotal60Hz
    909,            // u2VTotal50Hz
    773,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1935,           // u2HTotalMax
    1797,           // u2HTotal60Hz
    1849,           // u2HTotal50Hz
    1441,           // u2HTotalMin

    887,            // u2VTotalMax
    794,            // u2VTotal60Hz
    819,            // u2VTotal50Hz
    777,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1779,           // u2HTotalMax
    1647,           // u2HTotal60Hz
    1647,           // u2HTotal50Hz
    1511,           // u2HTotalMin

    1147,           // u2VTotalMax
    809,            // u2VTotal60Hz
    909,            // u2VTotal50Hz
    773,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1899,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    1014,           // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    783,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1899,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    1014,           // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    783,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1779,           // u2HTotalMax
    1647,           // u2HTotal60Hz
    1647,           // u2HTotal50Hz
    1511,           // u2HTotalMin

    1147,           // u2VTotalMax
    809,            // u2VTotal60Hz
    909,            // u2VTotal50Hz
    773,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1899,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    1014,           // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    783,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1935,           // u2HTotalMax
    1797,           // u2HTotal60Hz
    1849,           // u2HTotal50Hz
    1441,           // u2HTotalMin

    887,            // u2VTotalMax
    794,            // u2VTotal60Hz
    819,            // u2VTotal50Hz
    777,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1935,           // u2HTotalMax
    1571,           // u2HTotal60Hz
    1571,           // u2HTotal50Hz
    1441,           // u2HTotalMin

    887,            // u2VTotalMax
    794,            // u2VTotal60Hz
    794,            // u2VTotal50Hz
    777,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1935,           // u2HTotalMax
    1571,           // u2HTotal60Hz
    1571,           // u2HTotal50Hz
    1441,           // u2HTotalMin

    887,            // u2VTotalMax
    794,            // u2VTotal60Hz
    794,            // u2VTotal50Hz
    777,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1999,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    1062,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    783,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1999,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    1062,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    783,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1935,           // u2HTotalMax
    1649,           // u2HTotal60Hz
    1649,           // u2HTotal50Hz
    1441,           // u2HTotalMin

    1062,           // u2VTotalMax, William for Display Mode
//    887,            // u2VTotalMax
    794,            // u2VTotal60Hz
    794,            // u2VTotal50Hz
    777,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1779,           // u2HTotalMax
    1647,           // u2HTotal60Hz
    1647,           // u2HTotal50Hz
    1511,           // u2HTotalMin

    1052,           // u2VTotalMax, William for Display Mode
//    887,            // u2VTotalMax
    809,            // u2VTotal60Hz
    809,            // u2VTotal50Hz
    773,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2359,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2059,           // u2HTotalMin

    1199,           // u2VTotalMax
    1129,           // u2VTotal60Hz
    1129,           // u2VTotal50Hz
    1089,           // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2299,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2099,           // u2HTotalMin

    1138,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1114,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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
/* duplicate!
#if ((PANEL_SELECT == PANEL_SS_40_FHD) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    130000000,      // u4PixelClkMax
    148351648,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    155000000,      // u4PixelClkMin

    2649,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2159,           // u2HTotalMin

    1549,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1117,           // u2VTotalMin
    65,             // u1VClkMax
    45,             // u1VClkMin

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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
*/
#if ((PANEL_SELECT == PANEL_PV_154_LCM_C05) || CC_MULTI_PANEL)
    {
    1280,           // u2PanelWidth
    800,            // u2PanelHeight

    80000000,       // u4PixelClkMax
    71036164,       // u4PixelClk60Hz
    59256000,       // u4PixelClk50Hz
    54000000,       // u4PixelClkMin...spec not defined

    1599,           // u2HTotalMax
    1439,           // u2HTotal60Hz
    1439,           // u2HTotal50Hz
    1359,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    999,            // u2VTotalMax
    822,            // u2VTotal60Hz
    822,            // u2VTotal50Hz
    809,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    54000000,       // u4PixelClkMin...spec not defined

    1599,           // u2HTotalMax
    1439,           // u2HTotal60Hz
    1439,           // u2HTotal50Hz
    1359,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    999,            // u2VTotalMax
    822,            // u2VTotal60Hz
    822,            // u2VTotal50Hz
    809,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    54000000,       // u4PixelClkMin...spec not defined

    1599,           // u2HTotalMax
    1439,           // u2HTotal60Hz
    1439,           // u2HTotal50Hz
    1359,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    999,            // u2VTotalMax
    822,            // u2VTotal60Hz
    822,            // u2VTotal50Hz
    809,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    2 * 36489600,   // u4PixelClkMin...spec not defined

    1919,           // u2HTotalMax
    1599,           // u2HTotal60Hz
    1599,           // u2HTotal50Hz
    1499,           // u2HTotalMin

    1049,           // u2VTotalMax
    925,            // u2VTotal60Hz
    925,            // u2VTotal50Hz
    904,            // u2VTotalMin
    75,             // u1VClkMax
    56,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh //    Norman confirmed.
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

    1999,           // u2HTotalMax
    1839,           // u2HTotal60Hz
    1939,           // u2HTotal50Hz
    1779,           // u2HTotalMin

    1250,           // u2VTotalMax  (William for Display Mode)
//    1194,           // u2VTotalMax
    1079,           // u2VTotal60Hz
    1179,           // u2VTotal50Hz
    1059,           // u2VTotalMin
    76,             // u1VClkMax
    50,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    54000000,       // u4PixelClkMin...spec not defined

    1599,           // u2HTotalMax
    1439,           // u2HTotal60Hz
    1439,           // u2HTotal50Hz
    1359,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
//    999,            // u2VTotalMax
    822,            // u2VTotal60Hz
    822,            // u2VTotal50Hz
    809,            // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1999,           // u2HTotalMax
    1599,           // u2HTotal60Hz
    1599,           // u2HTotal50Hz
    1503,           // u2HTotalMin

    1062,           // u2VTotalMax

    925,            // u2VTotal60Hz
    925,            // u2VTotal50Hz
    904,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1935,           // u2HTotalMax
    1797,           // u2HTotal60Hz
    1849,           // u2HTotal50Hz
    1441,           // u2HTotalMin

    887,            // u2VTotalMax
    794,            // u2VTotal60Hz
    819,            // u2VTotal50Hz
    777,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2299,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2099,           // u2HTotalMin

//    1138,           // u2VTotalMax
    1200,           // u2VTotalMax (display mode)
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1114,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1557,           // u2HTotalMax
    1525,           // u2HTotal60Hz
    1525,           // u2HTotal50Hz
    1493,           // u2HTotalMin

    804,            // u2VTotalMax
    789,            // u2VTotal60Hz
    789,            // u2VTotal50Hz
    774,            // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2649,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2159,           // u2HTotalMin

    1549,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1117,           // u2VTotalMin
    65,             // u1VClkMax
    45,             // u1VClkMin

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    247500000,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2719,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2079,           // u2HTotalMin

//    1165,           // u2VTotalMax
    1200,           // u2VTotalMax (display mode)
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1089,           // u2VTotalMin
    120,            // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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
#if ((PANEL_SELECT == PANEL_AUO_315_XW2_V6_8280) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    800,            // u2PanelHeight

    82000000,       // u4PixelClkMax
    76675325,       // u4PixelClk120Hz
    74880000,       // u4PixelClk100Hz
    69700000,       // u4PixelClkMin

    1599,           // u2HTotalMax
    1559,           // u2HTotal120Hz
    1559,           // u2HTotal100Hz
    1495,           // u2HTotalMin

    979,            // u2VTotalMax
    819,            // u2VTotal120Hz
    959,            // u2VTotal100Hz
    775,            // u2VTotalMin
    122,            // u1VClkMax
    98,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2719,           // u2HTotalMax
    2199,           // u2HTotal120Hz
    2199,           // u2HTotal100Hz
    2079,           // u2HTotalMin

//    1165,           // u2VTotalMax
    1200,           // u2VTotalMax (display mode)
    1124,           // u2VTotal120Hz
    1349,           // u2VTotal100Hz
    1089,           // u2VTotalMin
    121,            // u1VClkMax
    91,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1557,           // u2HTotalMax
    1525,           // u2HTotal120Hz
    1525,           // u2HTotal100Hz
    1493,           // u2HTotalMin

//    804,            // u2VTotalMax
    849,            // u2VTotalMax
    819,            // u2VTotal120Hz
    819,            // u2VTotal100Hz
    774,            // u2VTotalMin
    123,            // u1VClkMax
    97,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    4 * 84900000,   // u4PixelClkMax
    297830400,      // u4PixelClk60Hz
    248192000,      // u4PixelClk50Hz
    4 * 69400000,   // u4PixelClkMin

    2507,           // u2HTotalMax
    2239,           // u2HTotal60Hz
    2239,           // u2HTotal50Hz
    2111,           // u2HTotalMin

    1200,           // u2VTotalMax (display mode)
    1124,           // u2VTotal120Hz
    1124,           // u2VTotal100Hz
    1089,           // u2VTotalMin
    120,             // u1VClkMax
    60,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    FOUR_PORT | DISP_30BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

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
    247500000,      // u4PixelClk50Hz
    240000000,   // u4PixelClkMin

    2700,           // u2HTotalMax
    2200,           // u2HTotal60Hz
    2150,           // u2HTotal50Hz
    2150,           // u2HTotalMin

//    1165,           // u2VTotalMax
    1600,           // u2VTotalMax (display mode)
    1125,           // u2VTotal60Hz
    1110,           // u2VTotal50Hz
    1110,           // u2VTotalMin
    125,            // u1VClkMax
    90,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2349,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2299,           // u2HTotal50Hz
    2089,           // u2HTotalMin

    1379,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1289,           // u2VTotal50Hz
    1091,           // u2VTotalMin
    130,             // u1VClkMax
    90,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2559,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2119,           // u2HTotalMin

    1200,         // For Dispmode, original:  1148,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

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

    2559,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2119,           // u2HTotalMin

    1200,         // For Dispmode, original:  1148,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

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

    2559,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2119,           // u2HTotalMin

    1200,         // For Dispmode, original:  1148,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

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

    2559,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2119,           // u2HTotalMin

    1200,         // For Dispmode, original:  1148,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1090,           // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin...spec not defined

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

/*
    123750000,      // u4PixelClMax
    123750000,      // u4PixelClk50Hz
    123750000,      // u4PixelClk60Hz
    123750000,      // u4PixelClkMin

    //130000000,      // u4PixelClkMax
    148425750,      // u4PixelClkMax  
    //148351648,      // u4PixelClk60Hz
    148425750,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    155000000,      // u4PixelClkMin

    118900000,      // u4PixelClkMax
    118900000,      // u4PixelClk60Hz
    118900000,      // u4PixelClk50Hz
    118900000,      // u4PixelClkMin
*/
    2649,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2159,           // u2HTotalMin

    1549,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz

    1117,           // u2VTotalMin
    65,             // u1VClkMax
    45,             // u1VClkMin

    30,             // u1HSyncWidth
    1,             // u1VSyncWidth
    2095,            // u1HPosition
    
    1020,            // u1VPosition
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
    247500000,      // u4PixelClk50Hz
    4 * 66970000,   // u4PixelClkMin

    2649,           // u2HTotalMax
    2195,           // u2HTotal60Hz
    2195,           // u2HTotal50Hz
    2159,           // u2HTotalMin

    //1549,           // u2VTotalMax
    1124,
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1117,           // u2VTotalMin
    //65,             // u1VClkMax
    130,
    //45,             // u1VClkMin
    90,
    
    30,             // u1HSyncWidth
    1,             // u1VSyncWidth
    2046,            // u1HPosition
    
    1022,            // u1VPosition
    //FOUR_PORT | DISP_30BIT | LVDS_JEIDA | LVDS_ODD_SW_OFF |
    //PWM_LOW_PANEL_BRIGHT,
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

//For SBTVD
#if ((PANEL_SELECT == PANEL_INNOLUX_19_AW02_LIPS_LOW) || CC_MULTI_PANEL)
    {
    1440,           // u2PanelWidth
    900,            // u2PanelHeight

    170000000,      // u4PixelClkMax
    88807193,            // u4PixelClk60Hz
    74080000,       // u4PixelClk50Hz
    50000000,      // u4PixelClkMin...

    1999,           // u2HTotalMax
    1599,           // u2HTotal60Hz
    1599,           // u2HTotal50Hz
    1503,           // u2HTotalMin

    1062,           // u2VTotalMax

    925,            // u2VTotal60Hz
    925,            // u2VTotal50Hz
    904,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xF0,           // (100%) u1BacklightHigh //
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

    1935,           // u2HTotalMax
    1649,           // u2HTotal60Hz
    1649,           // u2HTotal50Hz
    1441,           // u2HTotalMin

    1062,           // u2VTotalMax, William for Display Mode
//    887,            // u2VTotalMax
    794,            // u2VTotal60Hz
    794,            // u2VTotal50Hz
    777,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    1935,           // u2HTotalMax
    1649,           // u2HTotal60Hz
    1649,           // u2HTotal50Hz
    1441,           // u2HTotalMin

    1062,           // u2VTotalMax, William for Display Mode
//    887,            // u2VTotalMax
    794,            // u2VTotal60Hz
    794,            // u2VTotal50Hz
    777,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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
    2 * 32500000,   // u4PixelClkMin...spec not defined

    2047,           // u2HTotalMax
    1727,           // u2HTotal60Hz
    1727,           // u2HTotal50Hz
    1519,           // u2HTotalMin

    2047,           // u2VTotalMax
    915,            // u2VTotal60Hz
    915,            // u2VTotal50Hz
    909,            // u2VTotalMin
    76,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0x00,           // (100%) u1BacklightHigh //    Norman confirmed.
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

    1935,           // u2HTotalMax 
    1559,           // u2HTotal60Hz 
    1559,           // u2HTotal50Hz 
    1441,           // u2HTotalMin 

    887,           // u2VTotalMax 
    860,//805,            // u2VTotal60Hz 
    860,//805,            // u2VTotal50Hz 
    777,            // u2VTotalMin 
    63,             // u1VClkMax 
    57,             // u1VClkMin 

    30,             // u1HSyncWidth 
    3,              // u1VSyncWidth 
    0x800,          // u1HPosition 
    0x400,          // u1VPosition 
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF | 
    PWM_LOW_PANEL_BRIGHT, 

    0x00,           // (100%) u1BacklightHigh //    Norman confirmed. 
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
//wistron @sammi add 080912 	
//AUO T370XW02 VC
#if ((PANEL_SELECT == PANEL_AUO_37_XW2_VC) || CC_MULTI_PANEL)  // @sammi modified 080910start
    {
    1366,           // u2PanelWidth
    768,           // u2PanelHeight

    85000000,   // u4PixelClkMax
    78115200,      // u4PixelClk60Hz
    59645000,      // u4PixelClk50Hz
    60000000,   // u4PixelClkMin

    1900,           // u2HTotalMax
    1648,           // u2HTotal60Hz
    1510,           // u2HTotal50Hz
    1414,           // u2HTotalMin

    1015 ,           // u2VTotalMax
    806 ,           // u2VTotal60Hz
    790 ,           // u2VTotal50Hz
    784 ,           // u2VTotalMin
    63,             // u1VClkMax...spec not defined
    47,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition


    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT, 

    0xFF,           // u1BacklightHigh       //Wistron @sammi modified 080912
    0xB4,           // u1BacklightMiddle
    0x34,           // u1BacklightLow       
    0,              // u1Reserved
    150,              // u2DimmingFrequency60Hz  //sammi modifed 080924
    150,              // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    40,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved               // @sammi modified 080910 end
    },
#endif	

//samsung LTY[Z]320A02
#if ((PANEL_SELECT == PANEL_SS_32_WT_1) || CC_MULTI_PANEL) // start add by stanley 080908
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    86000000,       // u4PixelClkMax
    72354836,       // u4PixelClk60Hz
    68530000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    2000,           // u2HTotalMax
    1527,           // u2HTotal60Hz
    1539,           // u2HTotal50Hz
    1450,           // u2HTotalMin

    1300,           // u2VTotalMax
    789,            // u2VTotal60Hz
    889,            // u2VTotal50Hz
    773,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    
    0xFF,         // (100%) u1BacklightHigh 0x00              //Wistron @sammi modified 080912
    0xB5,         // (67%) u1BacklightMiddle 0x8B
    0x34,         // (50%) u1BacklightLow    0xFF
    0,              // u1Reserved
    150,             // u2DimmingFrequency60Hz    //sammi modifed 080924
    150,             // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    40,             // u1BacklightOnDelay
    20,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif // end add by stanley 080908

#if ((PANEL_SELECT == PANEL_AUO_26_WT_1) || CC_MULTI_PANEL) // start add by stanley 080908
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    85000000,       // u4PixelClkMax
    75366234,       // u4PixelClk60Hz
    68510000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1699,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    1062,           // u2VTotalMax, Winton for Display Mode
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    75,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh        //Wistron @sammi modified 080912
    0xB4,           // (67%) u1BacklightMiddle
    0x35,           // (50%) u1BacklightLow
    0,              // u1Reserved
    160,              // u2DimmingFrequency60Hz    //sammi modifed 080924
    160,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    5,              // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    30,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif // end add by stanley 080908

//CMO V216B1-L01
#if ((PANEL_SELECT == PANEL_CMO_22_B1_L01) || CC_MULTI_PANEL)  // sammi modified 080912
    {
    1366,           // u2PanelWidth
    768,           // u2PanelHeight

    82000000,   // u4PixelClkMax
    76000000,      // u4PixelClk60Hz
    76000000,      // u4PixelClk50Hz
    60000000,       // u4PixelClkMin

    1936,           // u2HTotalMax
    1560,           // u2HTotal60Hz
    1560,           // u2HTotal50Hz
    1442,           // u2HTotalMin

    983,           // u2VTotalMax  (William for Display Mode)
//    1194,           // u2VTotalMax
    806,           // u2VTotal60Hz
    806,           // u2VTotal50Hz
    778,           // u2VTotalMin
    76,             // u1VClkMax
    50,             // u1VClkMin...spec not defined

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition

    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,


    0xFF,           // u1BacklightHigh           //Wistron @sammi modified 080912
    0xB5,           // u1BacklightMiddle
    0x34,           // u1BacklightLow
    0,              // u1Reserved
    150,              // u2DimmingFrequency60Hz    //sammi modifed 080924
    150,              // u2DimmingFrequency50Hz

    3,              // u1LvdsOnDalay
    60,             // u1BacklightOnDelay
    12,             // u1BacklightOffDelay
    3,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
//AUO M1850XW01
#if ((PANEL_SELECT == PANEL_AUO_19_XW1) || CC_MULTI_PANEL)   //sammi modifed 080912
      {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    90000000,   // u4PixelClkMax
    78000000,       // u4PixelClk60Hz
    78000000,       // u4PixelClk50Hz
    60000000,   // u4PixelClkMin...spec not defined

    2047,           // u2HTotalMax
    1606,           // u2HTotal60Hz
    1606,           // u2HTotal50Hz
    1416,           // u2HTotalMin

    1023,           // u2VTotalMax
    808,            // u2VTotal60Hz
    808,            // u2VTotal50Hz
    776,            // u2VTotalMin
    76,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,  

    0xFF,           // (100%) u1BacklightHigh //    Norman confirmed. //Wistron @sammi modified 080912
    0x80,           // (77%) u1BacklightMiddle
    0x34,           // (55%) u1BacklightLow 
    0,              // u1Reserved
    150,              // u2DimmingFrequency60Hz   //sammi modifed 080924
    150,              // u2DimmingFrequency50Hz

    4,              // u1LvdsOnDalay
    70,             // u1BacklightOnDelay
    2,              // u1BacklightOffDelay
    2,              // u1LvdsOffDalay

    50,             // u2MaxOverscan
    0,              // u2Reserved
    },
#endif
  //wistron @sammi add 080912	end
#if ((PANEL_SELECT == PANEL_SS_52_FHD) || CC_MULTI_PANEL)
    {
    1920,           // u2PanelWidth
    1080,           // u2PanelHeight

    155000000,      // u4PixelClkMax
    147029702,      // u4PixelClk60Hz
    123750000,      // u4PixelClk50Hz
    130000000,      // u4PixelClkMin

    2449,           // u2HTotalMax
    2199,           // u2HTotal60Hz
    2199,           // u2HTotal50Hz
    2153,           // u2HTotalMin

    1479,           // u2VTotalMax
    1124,           // u2VTotal60Hz
    1124,           // u2VTotal50Hz
    1099,           // u2VTotalMin
    62,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    5,              // u1VSyncWidth
    0x800,          // u1HPosition
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
#if ((PANEL_SELECT == PANEL_SS_32_WT_1_WXGA2) || CC_MULTI_PANEL) // start add by stanley 080908
    {
    1366,           // u2PanelWidth
    768,            // u2PanelHeight

    86000000,       // u4PixelClkMax
    72354836,       // u4PixelClk60Hz
    68530000,       // u4PixelClk50Hz
    65000000,       // u4PixelClkMin

    2000,           // u2HTotalMax
    1527,           // u2HTotal60Hz
    1539,           // u2HTotal50Hz
    1450,           // u2HTotalMin

    1300,           // u2VTotalMax
    789,            // u2VTotal60Hz
    889,            // u2VTotal50Hz
    773,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    
    0xFF,         // (100%) u1BacklightHigh 0x00              //Wistron @sammi modified 080912
    0xB5,         // (67%) u1BacklightMiddle 0x8B
    0x34,         // (50%) u1BacklightLow    0xFF
    0,              // u1Reserved
    150,             // u2DimmingFrequency60Hz    //sammi modifed 080924
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

    1599,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1495,           // u2HTotalMin

    979,            // u2VTotalMax
    779,            // u2VTotal60Hz
    959,            // u2VTotal50Hz
    775,            // u2VTotalMin
    121,             // u1VClkMax
    99,             // u1VClkMin
    //61,             // u1VClkMax
    //49,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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

    2000,           // u2HTotalMax
    1527,           // u2HTotal60Hz
    1539,           // u2HTotal50Hz
    1450,           // u2HTotalMin

    1300,           // u2VTotalMax
    789,            // u2VTotal60Hz
    889,            // u2VTotal50Hz
    773,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    
    0xFF,         // (100%) u1BacklightHigh 0x00              //Wistron @sammi modified 080912
    0xB5,         // (67%) u1BacklightMiddle 0x8B
    0x34,         // (50%) u1BacklightLow    0xFF
    0,              // u1Reserved
    150,             // u2DimmingFrequency60Hz    //sammi modifed 080924
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

    2000,           // u2HTotalMax
    1527,           // u2HTotal60Hz
    1539,           // u2HTotal50Hz
    1450,           // u2HTotalMin

    1300,           // u2VTotalMax
    789,            // u2VTotal60Hz
    889,            // u2VTotal50Hz
    773,            // u2VTotalMin
    63,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_OFF| LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,
    
    0xFF,         // (100%) u1BacklightHigh 0x00              //Wistron @sammi modified 080912
    0xB5,         // (67%) u1BacklightMiddle 0x8B
    0x34,         // (50%) u1BacklightLow    0xFF
    0,              // u1Reserved
    150,             // u2DimmingFrequency60Hz    //sammi modifed 080924
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

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    887,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh   // Norman confirmed.
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

    1721,           // u2HTotalMax
    1559,           // u2HTotal60Hz
    1559,           // u2HTotal50Hz
    1413,           // u2HTotalMin

    887,            // u2VTotalMax
    805,            // u2VTotal60Hz
    805,            // u2VTotal50Hz
    788,            // u2VTotalMin
    62,             // u1VClkMax
    48,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
    0x400,          // u1VPosition
    SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF |
    PWM_HIGH_PANEL_BRIGHT,

    0xFF,           // (100%) u1BacklightHigh   // Norman confirmed.
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

    2649,           // u2HTotalMax
    2183,           // u2HTotal60Hz
    2183,           // u2HTotal50Hz
    2159,           // u2HTotalMin

    1549,           // u2VTotalMax
    1133,           // u2VTotal60Hz
    1359,           // u2VTotal50Hz
    1117,           // u2VTotalMin
    62,             // u1VClkMax
    47,             // u1VClkMin

    30,             // u1HSyncWidth
    3,              // u1VSyncWidth
    0x800,          // u1HPosition
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
};

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
    case PANEL_SS_32NC_LBA_SESL_FHD: return "PANEL_SS_32NC_LBA_SESL_FHD";
    default: return "NO NAME";
    }
}
