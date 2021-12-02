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

// === INCLUDE =============================================================================
#include "x_assert.h"
#include "sv_const.h"
#include "nptv_debug.h"
#include "pe_if.h"
#include "drv_ycproc.h"
#include "vdp_if.h"
#include "vdp_drvif.h"
#include "vdo_misc.h"
#include "source_select.h"
#include "drv_video.h"
#include "drv_pq_cust.h"
#include "drv_tdtv_drvif.h"
#include "drv_contrast.h"
#include "drv_di.h"
#include "drv_ttd.h"
#include "pe_table.h"    //add for vestel --- pingan 0606
#include "drv_ycproc.h"  //add for vestel --- pingan 0606
#ifdef CC_FLIP_MIRROR_SUPPORT
#include "drv_display.h"
#endif

#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif

#ifdef CC_MT5396
#include "drv_mjc_if.h"
#endif

//add for vestel --- pingan 0606
#include "drv_tvd.h"
#include "drv_tdc.h"
// === DEFINE =============================================================================
#define R_DIS       0x1     // Special case, not default mapping
#define R_QTYMAP    0x2     // Use quality_table
#define R_UIMAP     0x4     // UI Value = HW
#define R_MAIN      0x8     // Main only
#define R_OFF       0x10    // 0 as off, 1 as min
#define R_MAX       0x20    // Re-map max as max-1
#define R_NOSIG     0x40    // Also work in no singal
#define R_SCAN      0x80    // Also work in TV scan
#define R_AUTONR    0x100   //TNR : Auto, High, Mid, Low, Off   //add for vestel --- pingan 0606

//Caihua Add 20120709
#define BACKLIGHT_ITEMS_IN_PCF 14 //BACKLIGHT page in PCF xml
// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================

// === GLOBAL VARIABLE ====================================================================

// === EXTERN VARIABLE ====================================================================
EXTERN FLASH_PQ_FILE_HEAD aFlashPqFileHead;
EXTERN UINT8 bBLMode;

// === STATIC VARIABLE ====================================================================
static INT16 ai2Arg[VDP_NS][PE_ARG_NS];   // video quality item current value (UI)

static PE_UI_RANGE_T aUiVqMinMaxDft[FLASHPQ_TOTAL_TIMING][PE_ARG_NS];

static UINT16 aUiVqMinMaxDftSmartPic[PICTURE_MODE_MAX][FLASHPQ_TOTAL_TIMING][UI_SMART_PIC_UI_DEF_MAX];
static UINT16 aUiVqMinMaxDftSmartPic3D[PICTURE_MODE_MAX][FLASHPQ_TOTAL_TIMING][UI_SMART_PIC_UI_DEF_MAX];

//Caihua Add 20120709
UINT16 aUiVqDftBacklight[PICTURE_MODE_MAX][BACKLIGHT_ITEMS_IN_PCF];

static PE_UI_RANGE_T aUiVqMinMaxDftDefault[PE_ARG_NS] =
{
    // min   max   dft
    {   0,  100,    50},    // PE_ARG_BRIGHTNESS
    {   0,  100,    50},    // PE_ARG_CONTRAST
    { -50,  50,      0},    // PE_ARG_HUE
    {   0,  100,    50},    // PE_ARG_SATURATION
    {   0,    1,     1},    // PE_ARG_CTI
    {   0,   20,    10},    // PE_ARG_SHARPNESS    
    {   0,   20,    10},    // PE_ARG_SHARPNESS_H
    {   0,   20,    10},    // PE_ARG_SHARPNESS_V
    {   0,   20,    10},    // PE_ARG_LTI
    {   0,  255,   128},    // PE_ARG_R_GAIN
    {   0,  255,   128},    // PE_ARG_G_GAIN
    {   0,  255,   128},    // PE_ARG_B_GAIN    
    {   0,  255,   128},    // PE_ARG_R_OFFSET
    {   0,  255,   128},    // PE_ARG_G_OFFSET
    {   0,  255,   128},    // PE_ARG_B_OFFSET    
    {   0,    3,     2},    // PE_ARG_NR
    {   0,    3,     2},    // PE_ARG_GAMMA
    {   0,    1,     1},    // PE_ARG_WHITE_PEAK_LMT
    {   0,    3,     2},    // PE_ARG_SCE
    {   0,    3,     2},    // PE_ARG_SKIN_COLOR
    {   0,    3,     2},    // PE_ARG_ADAPTIVE_LUMA
    {   0,   100,   50},    // PE_ARG_BACK_LIGHT_LVL
    #if 0
    {   0,    2,     0},    // PE_ARG_ADAPTIVE_BACK_LIGHT
    #else//vestel
    {   0,    0,     0},    // PE_ARG_ADAPTIVE_BACK_LIGHT
    #endif
    {   0,    3,     2},    // PE_ARG_3D_NR
    {   0,    3,     0},    // PE_ARG_DEMO
    {   0,    1,     1},    // PE_ARG_DI_FILM_MODE
    {   0,    1,     0},    // PE_ARG_DI_MA,
    {   0,    1,     0},    // PE_ARG_DI_EDGE_PRESERVE,
    {   0,    1,     0},    // PE_ARG_MJC_MODE,
    {   0,    3,     1},    // PE_ARG_MJC_EFFECT,
    {   0,    1,     0},    // PE_ARG_MJC_DEMO,
    {   0,    1,     0},    // PE_ARG_XVYCC,
    {   0,    1,     0},    // PE_ARG_WCG,
    {   0,    1,     0},    // PE_ARG_GAME_MODE,
    {   0,    1,     0},    // PE_ARG_BLUE_STRETCH,
    {   0,    3,     2},    // PE_ARG_MPEG_NR,
    {   0,    3,     2},    // PE_ARG_BLACK_STRETCH,
    {   0,    3,     2},    // PE_ARG_WHITE_STRETCH,
    {   0,    1,     1},    // PE_ARG_SHARPNESS_ON_OFF,   
    {   0,    1,     1},    // PE_ARG_SUPERRESOLUTION_ON_OFF,    
    {   0,    1,     0},    // PE_ARG_OPC_CURVE,    
    {   0,    0,     0},    // PE_ARG_CBE,    
    {   0,    0,     0},    // PE_ARG_USPEI,
    {   0,    3,     2},    // PE_ARG_LCDIM
    {   0,  256,     1},    // PE_ARG_TDTV_PACKING,
    {   0,    1,     0},    // PE_ARG_TDTV_INVERSE,    
    {   0,   32,    16},    // PE_ARG_TDTV_DEPTH,
    {   0,    1,     0},    // PE_ARG_TDTV_3D_TO_2D
    {   0,    2,     0},    // PE_ARG_TDTV_FPR
    {   0,   32,    16},    // PE_ARG_TDTV_PROTRUDE,
    {   2,   18,    10},    // PE_ARG_TDTV_DISTANCE
    {   0,   32,    16},     // PE_ARG_TDTV_OSD_DEPTH
    
    //Begin add for Vestel --- pingan 0606
    {   0,    0,     0},     // PE_ARG_TVD_COMB
    {   0,    0,     0},     // PE_ARG_PIC_UI_DFT //Caihua Add 20110901
    {   0,    0,     0},     //PE_ARG_SIX_COLOR
    {   0,    0,     0},     //PE_ARG_TDTV_SIX_COLOR
    {   0,    3,     2},     // PE_ARG_TDTV_SCE
    {   0,  255,    128},    // PE_ARG_TDTV_BRIGHTNESS
    {   0,  255,    128},    // PE_ARG_TDTV_CONTRAST
    {   0,  255,    128},    // PE_ARG_TDTV_HUE
    {   0,  255,    128},    // PE_ARG_TDTV_SATURATION
    {   0,  31,   16},       // PE_ARG_TDTV_R_GAIN
    {   0,  31,   16},       // PE_ARG_TDTV_G_GAIN
    {   0,  31,   16},       // PE_ARG_TDTV_B_GAIN  
    {   0,  255,   128}      // PE_ARG_TDTV_SKIN
    //end
};

static PE_UI_RANGE_T aUiQtyItemMinMax[PE_ARG_NS] =
{
    // BEGIN                END                         TYPE
    {QUALITY_OSD_MAPPING_BRIGHTNESS,    QUALITY_OSD_MAPPING_BRIGHTNESS,            R_QTYMAP | R_SCAN},    // PE_ARG_BRIGHTNESS
    {QUALITY_OSD_MAPPING_CONTRAST,      QUALITY_OSD_MAPPING_CONTRAST,            R_QTYMAP | R_SCAN},      // PE_ARG_CONTRAST
    {QUALITY_OSD_MAPPING_HUE,           QUALITY_OSD_MAPPING_HUE,                  R_QTYMAP | R_SCAN},     // PE_ARG_HUE
    {QUALITY_OSD_MAPPING_SATURATION,    QUALITY_OSD_MAPPING_SATURATION,          R_QTYMAP | R_SCAN},      // PE_ARG_SATURATION
    {QUALITY_CTI_BEGIN,     QUALITY_CTI_END,            R_QTYMAP | R_MAIN | R_OFF},     // PE_ARG_CTI
    {QUALITY_SHARP_BEGIN,   QUALITY_SHARP_END,          R_QTYMAP | R_MAIN},             // PE_ARG_SHARPNESS
    {QUALITY_SHARP_H_BEGIN, QUALITY_SHARP_H_END,        R_QTYMAP | R_MAIN},             // PE_ARG_SHARPNESS_H
    {QUALITY_SHARP_V_BEGIN, QUALITY_SHARP_V_END,        R_QTYMAP | R_MAIN},             // PE_ARG_SHARPNESS_V
    {QUALITY_LTI_BEGIN,     QUALITY_LTI_END,            R_QTYMAP | R_MAIN | R_OFF},     // PE_ARG_LTI
    {QUALITY_OSD_MAPPING_R_GAIN,        QUALITY_OSD_MAPPING_R_GAIN,              R_QTYMAP | R_NOSIG | R_SCAN},   // PE_ARG_R_GAIN
    {QUALITY_OSD_MAPPING_G_GAIN,        QUALITY_OSD_MAPPING_G_GAIN,              R_QTYMAP | R_NOSIG | R_SCAN},   // PE_ARG_G_GAIN
    {QUALITY_OSD_MAPPING_B_GAIN,        QUALITY_OSD_MAPPING_B_GAIN,              R_QTYMAP | R_NOSIG | R_SCAN},   // PE_ARG_B_GAIN    
    {QUALITY_R_OFFSET,      QUALITY_R_OFFSET,           R_UIMAP  | R_NOSIG | R_SCAN},   // PE_ARG_R_OFFSET
    {QUALITY_G_OFFSET,      QUALITY_G_OFFSET,           R_UIMAP  | R_NOSIG | R_SCAN},   // PE_ARG_G_OFFSET
    {QUALITY_B_OFFSET,      QUALITY_B_OFFSET,           R_UIMAP  | R_NOSIG | R_SCAN},   // PE_ARG_B_OFFSET    
    {QUALITY_SNR_BEGIN,     QUALITY_SNR_END,            R_QTYMAP | R_MAIN | R_OFF},     // PE_ARG_NR   //remove R_MAX,add R_OFF for vestel
    {QUALITY_GAMMA_BEGIN,   QUALITY_GAMMA_END,          R_QTYMAP | R_MAIN | R_NOSIG},   // PE_ARG_GAMMA
    {0,                     0,                          R_DIS},                         // PE_ARG_WHITE_PEAK_LMT
    {QUALITY_SCE_BEGIN,     QUALITY_SCE_END,            R_QTYMAP | R_MAIN},             // PE_ARG_SCE
    {QUALITY_SKIN_BEGIN,    QUALITY_SKIN_END,           R_QTYMAP | R_MAIN},             // PE_ARG_SKIN_COLOR
    {QUALITY_AL_BEGIN,      QUALITY_AL_END,             R_QTYMAP | R_MAIN | R_OFF},     // PE_ARG_ADAPTIVE_LUMA
    {QUALITY_BACKLIGHT,     QUALITY_BACKLIGHT,          R_QTYMAP | R_MAIN | R_NOSIG | R_SCAN},  // PE_ARG_BACK_LIGHT_LVL
#if 0
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_ADAPTIVE_BACK_LIGHT
#else
    {QUALITY_ADAP_BKLT_BEGIN,QUALITY_ADAP_BKLT_END, R_MAIN | R_NOSIG | R_QTYMAP|R_SCAN},        // PE_ARG_ADAPTIVE_BACK_LIGHT
#endif
    {QUALITY_TNR_BEGIN,     QUALITY_TNR_END,            R_QTYMAP | R_MAIN | R_OFF},     // PE_ARG_3D_NR
    {0,                     0,                          R_DIS    | R_MAIN},                     // PE_ARG_DEMO
#if 0
    {0,                     0,                          R_DIS},                         // PE_ARG_DI_FILM_MODE
#else
    {QUALITY_DI_FILM_BEGIN,QUALITY_DI_FILM_END,         R_QTYMAP | R_MAIN},            // PE_ARG_DI_FILM_MODE
#endif
    {0,                     0,                          R_DIS},                         // PE_ARG_DI_MA,
    {0,                     0,                          R_DIS},                         // PE_ARG_DI_EDGE_PRESERVE,
    {0,                     0,                          R_DIS    | R_MAIN},             // PE_ARG_MJC_MODE,
    {0,                     0,                          R_DIS    | R_MAIN},             // PE_ARG_MJC_EFFECT,
    {0,                     0,                          R_DIS    | R_MAIN},             // PE_ARG_MJC_DEMO,
    {0,                     0,                          R_DIS    | R_MAIN},             // PE_ARG_XVYCC,
    {0,                     0,                          R_DIS    | R_MAIN},             // PE_ARG_WCG,
    {0,                     0,                          R_DIS    | R_MAIN},             // PE_ARG_GAME_MODE,
    {QUALITY_BS_BEGIN,      QUALITY_BS_END,             R_QTYMAP | R_MAIN | R_NOSIG},   // PE_ARG_BLUE_STRETCH,
    {QUALITY_MPEGNR_BEGIN,  QUALITY_MPEGNR_END,         R_QTYMAP | R_MAIN},             // PE_ARG_MPEG_NR,
    {QUALITY_AL_BS_BEGIN,   QUALITY_AL_BS_END,          R_QTYMAP | R_MAIN | R_OFF},     // PE_ARG_BLACK_STRETCH,
    {QUALITY_AL_WS_BEGIN,   QUALITY_AL_WS_END,          R_QTYMAP | R_MAIN | R_OFF},     // PE_ARG_WHITE_STRETCH,
    {0,                     0,                          R_DIS},                         // PE_ARG_SHARPNESS_ON_OFF, 
    {0,                     0,                          R_DIS},                         // PE_ARG_SUPERRESOLUTION_ON_OFF,   
    {0,                     0,                          R_DIS    | R_MAIN},             // PE_ARG_OPC_CURVE,    
    {QUALITY_CBE_BEGIN,     QUALITY_CBE_END,            R_QTYMAP | R_MAIN},             // PE_ARG_CBE
    {0,                     0,                          R_QTYMAP},                      // PE_ARG_USPEI,
    {0,                     0,                          R_MAIN},                        // PE_ARG_LCDIM,
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_TDTV_PACKING,
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_TDTV_INVERSE, 
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_TDTV_DEPTH,
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_TDTV_3D_TO_2D 
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_TDTV_FPR 
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_TDTV_PROTRUDE,
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_TDTV_DISTANCE 
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_TDTV_OSD_DEPTH
    //Begin Add for Vestel --- pingan 0606
    {QUALITY_TVD_COMB_BEGIN,  QUALITY_TVD_COMB_END,                 R_QTYMAP},          // PE_ARG_TVD_COMB
    {QUALITY_UI_DEFAULT_BEGIN, QUALITY_UI_DEFAULT_END,              R_QTYMAP},          // PE_ARG_PIC_UI_DFT
    {QUALITY_SIXCOLOR_BEGIN,  QUALITY_SIXCOLOR_END,                 R_QTYMAP| R_MAIN},  //PE_ARG_SIX_COLOR
    {QUALITY_SIXCOLOR_TDTV_BEGIN,       QUALITY_SIXCOLOR_TDTV_END,  R_QTYMAP| R_MAIN},  //PE_ARG_TDTV_SIX_COLOR
    {QUALITY_SCE_TDTV_BEGIN,        QUALITY_SCE_TDTV_END,           R_QTYMAP| R_MAIN},  //PE_ARG_TDTV_SCE 
    {QUALITY_OSD_MAPPING_3D_BRIGHTNESS,    QUALITY_OSD_MAPPING_3D_BRIGHTNESS,   R_QTYMAP | R_SCAN},             // PE_ARG_TDTV_BRIGHTNESS
    {QUALITY_OSD_MAPPING_3D_CONTRAST,      QUALITY_OSD_MAPPING_3D_CONTRAST,     R_QTYMAP | R_SCAN},             // PE_ARG_TDTV_CONTRAST
    {QUALITY_OSD_MAPPING_3D_HUE,           QUALITY_OSD_MAPPING_3D_HUE,          R_QTYMAP | R_SCAN},             // PE_ARG_TDTV_HUE
    {QUALITY_OSD_MAPPING_3D_SATURATION,    QUALITY_OSD_MAPPING_3D_SATURATION,   R_QTYMAP | R_SCAN},             // PE_ARG_TDTV_SATURATION
    {QUALITY_OSD_MAPPING_3D_R_GAIN,        QUALITY_OSD_MAPPING_3D_R_GAIN,       R_QTYMAP | R_NOSIG | R_SCAN},   // PE_ARG_TDTV_R_GAIN
    {QUALITY_OSD_MAPPING_3D_G_GAIN,        QUALITY_OSD_MAPPING_3D_G_GAIN,       R_QTYMAP | R_NOSIG | R_SCAN},   // PE_ARG_TDTV_G_GAIN
    {QUALITY_OSD_MAPPING_3D_B_GAIN,        QUALITY_OSD_MAPPING_3D_B_GAIN,       R_QTYMAP | R_NOSIG | R_SCAN},   // PE_ARG_TDTV_B_GAIN    
    {QUALITY_SKIN_TDTV_BEGIN,    QUALITY_SKIN_TDTV_END,                         R_QTYMAP | R_MAIN},             // PE_ARG_TDTV_SKIN
    //End Add for Vestel --- pingan 0606
};

static const UCHAR *ucPeUiType[PE_ARG_NS] = 
{
    "BRIGHTNESS",
    "CONTRAST",
    "HUE",
    "SATURATION",
    "CTI",
    "SHARPNESS",
    "SHARPNESS_H",
    "SHARPNESS_V", 
    "LTI",
    "R_GAIN",
    "G_GAIN",
    "B_GAIN",
    "R_OFFSET",
    "G_OFFSET",
    "B_OFFSET",
    "NR",
    "GAMMA",
    "WHITE_PEAK_LMT",
    "SCE",
    "SKIN_COLOR",
    "ADAPTIVE_LUMA",
    "BACK_LIGHT_LVL",
    "ADAPTIVE_BACK_LIGHT",
    "3D_NR",
    "DEMO",
    "DI_FILM_MODE",
    "DI_MA",
    "DI_EDGE_PRESERVE",
    "MJC_MODE",
    "MJC_EFFECT",
    "MJC_DEMO",
    "XVYCC",
    "WCG",
    "GAME_MODE",
    "BLUE_STRETCH",
    "MPEG_NR",
    "BLACK_STRETCH",
    "WHITE_STRETCH",
    "SHARPNESS_ON_OFF",
    "SUPERRESOLUTION_ON_OFF",
    "OPC_CURVE",
    "CBE",
    "USPEI",
    "LCDIM",
    "TDTV_PACKING",
    "TDTV_INVERSE",
    "TDTV_DEPTH",
    "TDTV_3D_TO_2D",
    "TDTV_FPR",
    "PE_ARG_TDTV_PROTRUDE",  
    "PE_ARG_TDTV_DISTANCE",  
    "PE_ARG_TDTV_OSD_DEPTH",
    
    //Begin Add for Vestel --- pingan 0606
    "TVD_COMB",             // PE_ARG_TVD_COMB,
    "UI_DFT",               // PE_ARG_PIC_UI_DFT,
    "SIX_COLOR" ,           // PE_ARG_SIX_COLOR,
    "TDTV_SIX_COLOR",       //PE_ARG_TDTV_SIX_COLOR
    "TDTV_SCE",             // PE_ARG_TDTV_SCE
    "TDTV_BRIGHTNESS",      // PE_ARG_TDTV_BRIGHTNESS
    "TDTV_CONTRAST",        // PE_ARG_TDTV_CONTRAST
    "TDTV_HUE",             // PE_ARG_TDTV_HUE
    "TDTV_SATURATION",      // PE_ARG_TDTV_SATURATION
    "TDTV_R_GAIN",          // PE_ARG_TDTV_R_GAIN
    "TDTV_G_GAIN",          // PE_ARG_TDTV_G_GAIN
    "TDTV_B_GAIN",          // PE_ARG_TDTV_B_GAIN
    "TDTV_SKIN_COLOR"       // PE_ARG_TDTV_SKIN
    //End Add for Vestel --- pingan 0606
};

UINT8 bSrcTimingInverseTbl[SOURCE_TYPE_TIMING_MAX+1];

#ifdef DRV_SUPPORT_EXTMJC
static UINT8 bMjcUiEffect;
static UINT8 bMjcUiMode;
static UINT8 bMjcUiDemo;
//static UINT8 bMjcUiDemoMode;
#endif
#ifdef CC_MT5396
static UINT8 fgMjcClearLcdDemo;
#endif

// === END OF STATIC VARIABLE =============================================================
// === BODY ===============================================================================

UINT16 wReadQualityTable(UINT16 wAddr)
{
    UINT16 wValue;

    if(wAddr >= QUALITY_MAX)
    {
        LOG(0, "API_PEUI Error: wReadQualityTable[%d]=%d\n", wAddr);    
        return 0;
    }
    else
    {
        wValue = QUALITY_TBL[wAddr];        
    }   
    
    switch(wAddr)
    {
        case QUALITY_OSD_MAPPING_CONTRAST:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_OSD_MAPPING_CONTRAST_MSB]) << 8; 
        break;
        
        case QUALITY_OSD_MAPPING_SATURATION:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_OSD_MAPPING_SATURATION_MSB]) << 8;   
        break;

        case QUALITY_OSD_MAPPING_R_GAIN:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_OSD_MAPPING_R_GAIN_MSB]) << 8;   
        break;
        
        case QUALITY_OSD_MAPPING_G_GAIN:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_OSD_MAPPING_G_GAIN_MSB]) << 8;   
        break;
        
        case QUALITY_OSD_MAPPING_B_GAIN:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_OSD_MAPPING_B_GAIN_MSB]) << 8;   
        break;
        
        case QUALITY_R_OFFSET:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_R_OFFSET_MSB]) << 8; 
        break;
        
        case QUALITY_G_OFFSET:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_G_OFFSET_MSB]) << 8; 
        break;
        
        case QUALITY_B_OFFSET:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_B_OFFSET_MSB]) << 8; 
        break;
        
        //Begin Add (DI Film) for Vestel --- pingan 0606
        case QUALITY_DI_FILM_FR_MIN:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_DI_FILM_FR_MIN_HIBYTE])<<8;
        break;

        case QUALITY_DI_FILM_FR_MAX:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_DI_FILM_FR_MAX_HIBYTE])<<8;
        break;    

        case QUALITY_DI_FILM_EG_MIN:  
            wValue += ((UINT16)QUALITY_TBL[QUALITY_DI_FILM_EG_MIN_HIBYTE])<<8;
        break;

        case QUALITY_DI_FILM_EG_MAX_START_32:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_DI_FILM_EG_MAX_START_32_HIBYTE])<<8;
        break;

        case QUALITY_DI_FILM_EG_MAX_END_32:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_DI_FILM_EG_MAX_END_32_HIBYTE])<<8;
        break;

        case QUALITY_DI_FILM_EG_MAX_START_22:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_DI_FILM_EG_MAX_START_22_HIBYTE])<<8;
        break;

        case QUALITY_DI_FILM_EG_MAX_END_22:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_DI_FILM_EG_MAX_END_22_HIBYTE])<<8;
        break;
        //End Add (DI Film) for Vestel --- pingan 0606

        //Add (TVD) for Vestel --- pingan 0606
        case QUALITY_DECODER_AGC2_MANUAL_DCODE:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_DECODER_AGC2_MANUAL_DCODE_HIBYTE]) <<8;
        
        //Begin Add (3D) for Vestel --- pingan 0606   
        case QUALITY_OSD_MAPPING_3D_CONTRAST:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_OSD_MAPPING_3D_CONTRAST_MSB]) << 8;    
        break;
        
        case QUALITY_OSD_MAPPING_3D_SATURATION:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_OSD_MAPPING_3D_SATURATION_MSB]) << 8;    
        break;
        
        case QUALITY_OSD_MAPPING_3D_R_GAIN:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_OSD_MAPPING_3D_R_GAIN_MSB]) << 8;    
        break;
        
        case QUALITY_OSD_MAPPING_3D_G_GAIN:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_OSD_MAPPING_3D_G_GAIN_MSB]) << 8;    
        break;
        
        case QUALITY_OSD_MAPPING_3D_B_GAIN:
            wValue += ((UINT16)QUALITY_TBL[QUALITY_OSD_MAPPING_3D_B_GAIN_MSB]) << 8;    
        break;
        //End Add (3D) for Vestel --- pingan 0606   
    }
    
    return wValue;
}

void wWriteQualityTable(UINT16 wAddr, UINT16 wData)
{
    UINT8 MSB = (UINT8)(wData >> 8);
    UINT8 LSB = (UINT8)(wData & 0xFF);

    if(wAddr >= QUALITY_MAX)
    {
        LOG(0, "API_PEUI Error: wWriteQualityTable[%d]=%d\n", wAddr, wData);    
        return;
    }
    else
    {
        QUALITY_TBL[wAddr] = LSB;       
    }
    
    switch(wAddr)
    {
        case QUALITY_OSD_MAPPING_CONTRAST:
            QUALITY_TBL[QUALITY_OSD_MAPPING_CONTRAST_MSB] = MSB;    
        break;
        
        case QUALITY_OSD_MAPPING_SATURATION:
            QUALITY_TBL[QUALITY_OSD_MAPPING_SATURATION_MSB] = MSB;    
        break;

        case QUALITY_OSD_MAPPING_R_GAIN:
            QUALITY_TBL[QUALITY_OSD_MAPPING_R_GAIN_MSB] = MSB;
        break;
        
        case QUALITY_OSD_MAPPING_G_GAIN:
            QUALITY_TBL[QUALITY_OSD_MAPPING_G_GAIN_MSB] = MSB;
        break;
        
        case QUALITY_OSD_MAPPING_B_GAIN:
            QUALITY_TBL[QUALITY_OSD_MAPPING_B_GAIN_MSB] = MSB;
        break;  

        case QUALITY_R_OFFSET:
            QUALITY_TBL[QUALITY_R_OFFSET_MSB] = MSB;
        break;
        
        case QUALITY_G_OFFSET:
            QUALITY_TBL[QUALITY_G_OFFSET_MSB] = MSB;
        break;
        
        case QUALITY_B_OFFSET:
            QUALITY_TBL[QUALITY_B_OFFSET_MSB] = MSB;
        break;
        
        //Begin Add (DI Film) for Vestel --- pingan 0606
        case QUALITY_DI_FILM_FR_MIN:
            QUALITY_TBL[QUALITY_DI_FILM_FR_MIN_HIBYTE] = MSB;
        break;

        case QUALITY_DI_FILM_FR_MAX:
            QUALITY_TBL[QUALITY_DI_FILM_FR_MAX_HIBYTE] = MSB;
        break;    

        case QUALITY_DI_FILM_EG_MIN:  
            QUALITY_TBL[QUALITY_DI_FILM_EG_MIN_HIBYTE] = MSB;
        break;

        case QUALITY_DI_FILM_EG_MAX_START_32:
            QUALITY_TBL[QUALITY_DI_FILM_EG_MAX_START_32_HIBYTE] = MSB;
        break;

        case QUALITY_DI_FILM_EG_MAX_END_32:
            QUALITY_TBL[QUALITY_DI_FILM_EG_MAX_END_32_HIBYTE] = MSB;
        break;

        case QUALITY_DI_FILM_EG_MAX_START_22:
            QUALITY_TBL[QUALITY_DI_FILM_EG_MAX_START_22_HIBYTE] = MSB;
        break;

        case QUALITY_DI_FILM_EG_MAX_END_22:
            QUALITY_TBL[QUALITY_DI_FILM_EG_MAX_END_22_HIBYTE] = MSB;
        break;
        //End Add (DI Film) for Vestel --- pingan 0606
        
        //Add (TVD) for Vestel --- pingan 0606
        case QUALITY_DECODER_AGC2_MANUAL_DCODE:
            QUALITY_TBL[QUALITY_DECODER_AGC2_MANUAL_DCODE_HIBYTE] = MSB;
        break;

        //Begin Add (3D) for Vestel --- pingan 0606
        case QUALITY_OSD_MAPPING_3D_CONTRAST:
            QUALITY_TBL[QUALITY_OSD_MAPPING_3D_CONTRAST_MSB] = MSB;    
        break;
        
        case QUALITY_OSD_MAPPING_3D_SATURATION:
            QUALITY_TBL[QUALITY_OSD_MAPPING_3D_SATURATION_MSB] = MSB;    
        break;

        case QUALITY_OSD_MAPPING_3D_R_GAIN:
            QUALITY_TBL[QUALITY_OSD_MAPPING_3D_R_GAIN_MSB] = MSB;
        break;
        
        case QUALITY_OSD_MAPPING_3D_G_GAIN:
            QUALITY_TBL[QUALITY_OSD_MAPPING_3D_G_GAIN_MSB] = MSB;
        break;
        
        case QUALITY_OSD_MAPPING_3D_B_GAIN:
            QUALITY_TBL[QUALITY_OSD_MAPPING_3D_B_GAIN_MSB] = MSB;
        break;
        //End Add (3D) for Vestel --- pingan 0606
    }   
}

void vApiApplyPQSetting(UINT8 bPath)
{
    UINT8 i,j;
    UINT8 u1PicMode;        //vestel
    UINT16 u2SrcTypTmg;     //vestel

    for (j=0; j<FLASHPQ_NUM; j++)
    {
        bApiFlashPqUpdateQtyMinMax(bPath, j);
    }

    //Add for vestel --- pingan 0606
    u1PicMode = vApiFlashPqGetSmartPicId();
    u2SrcTypTmg = bDrvVideoGetSourceTypeTiming(bPath);
    u2SrcTypTmg = bApiGetCustomSourceTypeTiming(u2SrcTypTmg);
    LOG(0,"\n==IN vApiApplyPQSetting() !!!==\n"); 
    vApiUpdateUIDefaultRange(u1PicMode, u2SrcTypTmg);
    
    for (i=0; i < (UINT8)PE_ARG_NS; i++)
    {
        switch (i)
        {
            // List all the quality items that do not update after mode change done.
            //  the back light is not relative to video timing, nor main/pip change.
            //case PE_ARG_ADAPTIVE_BACK_LIGHT:
            //case PE_ARG_BACK_LIGHT_LVL:
            case PE_ARG_BLACK_STRETCH:      //vestel
            case PE_ARG_WHITE_STRETCH:      //vestel
            case PE_ARG_GAMMA:              //vestel
            case PE_ARG_TDTV_PACKING:       //vestel
                break;                    
            default:
                UNUSED(bApiVideoProc(bPath, (PE_ARG_TYPE)i));
                break;
        }
    }
    // Flush all of setting
    vApiApplyVideoEvent(PEQTY_APPLY_TIME_OUTPUT_VSYNC_ISR);
    vApiApplyVideoEvent(PEQTY_APPLY_TIME_MJC_INPUT_VSYNC_ISR);  
}

/**
 * @brief Mapping the bCustomSrcTimingTbl[]  and 
 *	bCustomSourceGroup[] table.
 *
 */
static void _vMappingSrcTiming(void) //from 89 cn pro  
{
    UINT8 i, j;
    UINT8 bCustomSrcTypTmg, bBegin, bEnd;
    
    x_memset((void*)bSrcTimingInverseTbl, 0xFF, (SOURCE_TYPE_TIMING_MAX+1));

    // Setup bCustomSrcTimingTbl Inversion Table (bSrcTimingInverseTbl)
    // bCustomSrcTimingTbl should be one-on-one mapping to 
    for(i = 0; i <= SOURCE_TYPE_TIMING_MAX; i++)
    {
        j = bCustomSrcTimingTbl[i];

        if(j <= SOURCE_TYPE_TIMING_MAX)
        {       
            if(bSrcTimingInverseTbl[j] == 0xFF)
            {
                bSrcTimingInverseTbl[j] = i;
            }
            else
            {
                LOG(0,"API_PEUI Error : bCustomSrcTimingTbl not one-on-one mapping");
                ASSERT(0);
            }

        }else
        {
            LOG(0, "API_PEUI Error: _vMappingSrcTiming on j <= SOURCE_TYPE_TIMING_MAX");
            return;
        }
    }

    bCustomSrcTypTmg = 0;
    
    do      
    {
        bBegin = READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg, QTY_IDX_RANGE_BEGIN);
        bEnd = READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg, QTY_IDX_RANGE_END);

        if(bBegin <= SOURCE_TYPE_TIMING_MAX && bEnd <= SOURCE_TYPE_TIMING_MAX)
        {
            WRITE_CUSTOM_SRC_GROUP(bCustomSrcTypTmg, QTY_IDX_RANGE_BEGIN,
               bSrcTimingInverseTbl[bBegin]);

            WRITE_CUSTOM_SRC_GROUP(bCustomSrcTypTmg, QTY_IDX_RANGE_END,
               bSrcTimingInverseTbl[bEnd]); 
        
            bCustomSrcTypTmg++ ;
        }else
        {
            LOG(0, "API_PEUI Error: _vMappingSrcTiming on bBegin/bEnd <= SOURCE_TYPE_TIMING_MAX");
            return;
        }
        
    //End of the bCustomSourceGroup Table
    }while((bBegin != SOURCE_TYPE_TIMING_MAX) && (bEnd != SOURCE_TYPE_TIMING_MAX));
}


/**
 * @brief Parse the bCustomQtyItem[] table.
 *
 */
void bApiParseCustomQtyItem()
{
    UINT16 DftIndex, CustIndex;

    UINT8 j;

    _vMappingSrcTiming();

    for(j = 0; j < FLASHPQ_NUM; j++)
    {
        for (DftIndex = 0; DftIndex < QUALITY_MAX; DftIndex++)
        {
            for (CustIndex = 0; CustIndex <  CUSTOM_QTY_TBL_DIM ; CustIndex++)
            {
                if (READ_CUST_QTY_ITEM(j, CustIndex) >= QUALITY_MAX)
                {
                    // Can not find this item is CustomQtyItem table.
                    // So this item will use default value in DefaultQtyTbl[].
                    break;
                }

                else if (READ_CUST_QTY_ITEM(j, CustIndex) == DftIndex)
                {
                    // This item is found in CustomQtyItem, mark its serial number.
                    // CustomQtyTble for this item will be used, not use default value.
                    WRITE_DEFUALT_QTY_TBL_REF(DftIndex, (CustIndex | (j<<QTY_TBL_REF_INDEX_SHIFT)));
                    break;
                }
            }
        }
    }
}

/**
 * @brief Translate source type/timing index into customer defined index.
 * @param bSrcTypTmg SOURCE_TYPE_TIMING_RFTV ~ SOURCE_TYPE_TIMING_MAX
 *
 */
UINT8 bApiGetCustomSourceTypeTiming(UINT8 bSrcTypTmg)
{
    UINT8 bCustomSrcTypTmg;

    if(bSrcTypTmg == SOURCE_TYPE_TIMING_MAX)
    {
        return SOURCE_TYPE_TIMING_MAX;
    }
    else if(bSrcTypTmg < SOURCE_TYPE_TIMING_MAX)
    {
        bSrcTypTmg = bSrcTimingInverseTbl[bSrcTypTmg];
    }
    else
    {
        LOG(0, "API_PEUI Error: bApiGetCustomSourceTypeTiming bSrcTypTmg <= SOURCE_TYPE_TIMING_MAX");
        return SOURCE_TYPE_TIMING_MAX;
    }

        for (bCustomSrcTypTmg = 0;
             //End of the bCustomSourceGroup Table
             (READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN) !=
              SOURCE_TYPE_TIMING_MAX)
             && (READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END) !=
                 SOURCE_TYPE_TIMING_MAX); bCustomSrcTypTmg++)
        {

            if (READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN) >
                READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END))
            {
                // Fix QTY_IDX_RANGE_END.
                WRITE_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END,
                    READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN));
            }

            if ((bSrcTypTmg >=
                 READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN))
                && (bSrcTypTmg <=
                    READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END)))
            {
                return bCustomSrcTypTmg;
            }
        }

    // Can not find proper quality index.
    LOG(0,"API_PEUI Error: can not find customer source group ");
    return SOURCE_TYPE_TIMING_MAX;
}

/**
 * @brief Search the default value from DefaultQtyTbl or CustomQtyTbl.
 * @param wSearchIndex
 * @param bQtyIdx
 */
UINT8 bApiGetQtyDft(UINT16 wSearchItem, UINT8 bQtyIdx)
{
    UINT8 Ret;
    // After run bApiParseCustomQtyItem, if reference is FROM_DFT, use default table.
    // Or input source is un-defined in customer source group, use default table.
    if ((READ_DEFUALT_QTY_TBL_REF(wSearchItem) == FROM_DFT) ||
        (bQtyIdx >= SOURCE_TYPE_TIMING_MAX))
    {
        // Directly use the default quality table.
        Ret = READ_DEFUALT_QTY_TBL_DFT(wSearchItem);
        LOG(7, "READ_DEFUALT_QTY_TBL_DFT(%d)=%d\n", wSearchItem, Ret);        
    }
    else
    {
    // This quality item can be found in custom table.
        Ret =
        READ_CUSTOM_QTY_TBL(bQtyIdx,READ_DEFUALT_QTY_TBL_REF(wSearchItem));
        LOG(5, "READ_CUSTOM_QTY_TBL(%d, %d)=%d wSearchItem=%d\n", 
            bQtyIdx,READ_DEFUALT_QTY_TBL_REF(wSearchItem), Ret, wSearchItem);
    }

    return Ret;
}

/**
 * @brief Search the min value from DefaultQtyTbl.
 * @param wSearchIndex
 */
UINT8 bApiGetQtyMin(UINT16 u2QtyItem)
{
    return READ_DEFUALT_QTY_TBL_MIN(u2QtyItem);
}

/**
 * @brief Search the min value from DefaultQtyTbl.
 * @param wSearchIndex
 */
UINT8 bApiGetQtyMax(UINT16 u2QtyItem)
{
    return READ_DEFUALT_QTY_TBL_MAX(u2QtyItem);
}

PE_UI_RANGE_T GetQtyRange(UINT8 u1SrcTypTmg, UINT16 u2QtyItem)
{
    UINT16 MSBQtyItem;
    PE_UI_RANGE_T UiRange;
    
    if(u2QtyItem < QUALITY_MAX)
    {
        UiRange.i4Min = READ_DEFUALT_QTY_TBL_MIN(u2QtyItem);
        UiRange.i4Max = READ_DEFUALT_QTY_TBL_MAX(u2QtyItem);
        UiRange.i4Dft = bApiGetQtyDft(u2QtyItem, u1SrcTypTmg);
    }
    else
    {
        LOG(0, "API_PEUI Error: GetQtyRange u2QtyItem < QUALITY_MAX");
        UiRange.i4Min = 0;
        UiRange.i4Max = 0;
        UiRange.i4Dft = 0;      
        return UiRange;
    }

    // Special Case Handing for MSB Byte 
    switch(u2QtyItem)
    {
        case QUALITY_OSD_MAPPING_CONTRAST:
            MSBQtyItem = QUALITY_OSD_MAPPING_CONTRAST_MSB;
            break;    
            
        case QUALITY_OSD_MAPPING_SATURATION:
            MSBQtyItem = QUALITY_OSD_MAPPING_SATURATION_MSB;
            break;
            
        case QUALITY_OSD_MAPPING_R_GAIN:
            MSBQtyItem = QUALITY_OSD_MAPPING_R_GAIN_MSB;        
            break;
            
        case QUALITY_OSD_MAPPING_G_GAIN:
            MSBQtyItem = QUALITY_OSD_MAPPING_G_GAIN_MSB;        
            break;
            
        case QUALITY_OSD_MAPPING_B_GAIN:
            MSBQtyItem = QUALITY_OSD_MAPPING_B_GAIN_MSB;            
            break;
        
        //Begin Add for Vestel --- pingan 0606
        case QUALITY_OSD_MAPPING_3D_CONTRAST:
            MSBQtyItem = QUALITY_OSD_MAPPING_3D_CONTRAST_MSB;
            break;    
            
        case QUALITY_OSD_MAPPING_3D_SATURATION:
            MSBQtyItem = QUALITY_OSD_MAPPING_3D_SATURATION_MSB;
            break;
            
        case QUALITY_OSD_MAPPING_3D_R_GAIN:
            MSBQtyItem = QUALITY_OSD_MAPPING_3D_R_GAIN_MSB;        
            break;
            
        case QUALITY_OSD_MAPPING_3D_G_GAIN:
            MSBQtyItem = QUALITY_OSD_MAPPING_3D_G_GAIN_MSB;        
            break;
            
        case QUALITY_OSD_MAPPING_3D_B_GAIN:
            MSBQtyItem = QUALITY_OSD_MAPPING_3D_B_GAIN_MSB;            
            break;
        //End Add for Vestel --- pingan 0606
        default:
            MSBQtyItem = 0;
    }

    if(MSBQtyItem != 0)
    {
        UiRange.i4Min += ((UINT16)READ_DEFUALT_QTY_TBL_MIN(MSBQtyItem)) << 8;
        UiRange.i4Max += ((UINT16)READ_DEFUALT_QTY_TBL_MAX(MSBQtyItem)) << 8;
        UiRange.i4Dft += ((UINT16)bApiGetQtyDft(MSBQtyItem, u1SrcTypTmg)) << 8; 
    }
    
    return UiRange;
}


// Begin add for Vestel --- pingna 0606
void vApiSetUiDftRange(UINT8 u1PicMode)
{
    UINT8 i,j;
    INT16 Ret;

    for(i=0; i<aFlashPqFileHead.bNumOfTiming ;i++)
    {
        for(j=0; j<UI_SMART_PIC_UI_DEF_MAX; j++)
        {
            //3D
            Ret = vApiFlashPqUpdateUi3DRange(i,j); 

            if(Ret >= 0)
            {
                aUiVqMinMaxDftSmartPic3D[u1PicMode][i][j] = Ret;
            }
            //end

            Ret = vApiFlashPqUpdateUiRange(i,j); 
            
            if(Ret >= 0)
            {
                aUiVqMinMaxDftSmartPic[u1PicMode][i][j] = Ret;
            }
        } 
    }    
}

void vApiSetUiMinMaxRange(UINT8 u1SrcTypTmg)
{    

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_CONTRAST].i4Min 
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_CONTRASTDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_CONTRAST].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_CONTRASTDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_BRIGHTNESS].i4Min  
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_BRIGHTNESSDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_BRIGHTNESS].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_BRIGHTNESSDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SATURATION].i4Min 
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_SATURATIONDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SATURATION].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_SATURATIONDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_HUE].i4Min 
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_HUEDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_HUE].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_HUEDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SHARPNESS].i4Min 
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_SHARPNESSDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SHARPNESS].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_SHARPNESSDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SKIN_COLOR].i4Min 
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_SKINTONEDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SKIN_COLOR].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_SKINTONEDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_R_GAIN].i4Min 
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_REDGAINDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_R_GAIN].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_REDGAINDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_G_GAIN].i4Min  
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_GREENGAINDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_G_GAIN].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_GREENGAINDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_B_GAIN].i4Min  
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_BLUEGAINDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_B_GAIN].i4Max 
        =READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_BLUEGAINDFT);

    // 3D 20120423
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_CONTRAST].i4Min 
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_3D_CONTRASTDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_CONTRAST].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_3D_CONTRASTDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_BRIGHTNESS].i4Min  
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_3D_BRIGHTNESSDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_BRIGHTNESS].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_3D_BRIGHTNESSDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_SATURATION].i4Min 
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_3D_SATURATIONDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_SATURATION].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_3D_SATURATIONDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_HUE].i4Min 
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_3D_HUEDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_HUE].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_3D_HUEDFT);


    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_SKIN].i4Min 
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_3D_SKINTONEDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_SKIN].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_3D_SKINTONEDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_R_GAIN].i4Min 
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_3D_REDGAINDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_R_GAIN].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_3D_REDGAINDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_G_GAIN].i4Min  
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_3D_GREENGAINDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_G_GAIN].i4Max 
        = READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_3D_GREENGAINDFT);

    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_B_GAIN].i4Min  
        = READ_DEFUALT_QTY_TBL_MIN(QUALITY_OSD_MAPPING_3D_BLUEGAINDFT);
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_B_GAIN].i4Max 
        =READ_DEFUALT_QTY_TBL_MAX(QUALITY_OSD_MAPPING_3D_BLUEGAINDFT);

    
    if (bApiGetPeUiDebugMode() == SV_ON)
    {
        printf("Set Timing Min Source=%d : B %d  C %d  H %d  S %d  Sharp %d Skin Tone %d R Gain %d G Gain %d B Gain %d\n", u1SrcTypTmg,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_BRIGHTNESS].i4Min,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_CONTRAST].i4Min,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_HUE].i4Min,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SATURATION].i4Min,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SHARPNESS].i4Min,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SKIN_COLOR].i4Min,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_R_GAIN].i4Min,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_G_GAIN].i4Min,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_B_GAIN].i4Min);
        printf("Set Timing Max Source=%d : B %d  C %d  H %d  S %d  Sharp %d Skin Tone %d R Gain %d G Gain %d B Gain %d\n", u1SrcTypTmg,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_BRIGHTNESS].i4Max,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_CONTRAST].i4Max,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_HUE].i4Max,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SATURATION].i4Max,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SHARPNESS].i4Max,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SKIN_COLOR].i4Max,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_R_GAIN].i4Max,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_G_GAIN].i4Max,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_B_GAIN].i4Max);
    }
}

//Caihua 20120709
void vApiSetBacklightDft(UINT8 u1PicMode)
{
    UINT8 i,j;
    INT16 Ret;

    i=0;//the backlight settings of all the timing is the same
    for(j=0; j<BACKLIGHT_ITEMS_IN_PCF; j++)
    {
        Ret = vApiFlashPqUpdateBacklightDft(i,j); 
        
        if(Ret >= 0)
        {
            aUiVqDftBacklight[u1PicMode][j] = Ret;
        }
    }
}

void vApiUiDftRangeVerify(void)
{
    UINT8 i,j,k;

    for(i=0; i<PICTURE_MODE_MAX;i++)
    {
        printf("\n== Picture mode %d == ", i);
        for(j=0; j<FLASHPQ_TOTAL_TIMING; j++)
        {    
            printf("\nTiming %d == ", j);
            for(k=0; k<UI_SMART_PIC_UI_DEF_MAX; k++)
            {
                printf("%d ", aUiVqMinMaxDftSmartPic[i][j][k]);
                printf("%d ", aUiVqMinMaxDftSmartPic3D[i][j][k]);
            }
        } 
    }    
}


//Caihua Add 20110908
void vApiUpdateUIDefaultRange(UINT8 u1PicMode, UINT8 u1SrcTypTmg)
{        
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_BRIGHTNESS].i4Dft 
        = aUiVqMinMaxDftSmartPic[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_BRIGHTNESS];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_CONTRAST].i4Dft 
        = aUiVqMinMaxDftSmartPic[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_CONTRAST];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_HUE].i4Dft 
        = aUiVqMinMaxDftSmartPic[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_HUE];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SATURATION].i4Dft 
        = aUiVqMinMaxDftSmartPic[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_SATURATION];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SHARPNESS].i4Dft 
        = aUiVqMinMaxDftSmartPic[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_SHAPRNESS];

    //Caihua 20111106 Add
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SKIN_COLOR].i4Dft 
        = aUiVqMinMaxDftSmartPic[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_SKIN_TONE];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_R_GAIN].i4Dft 
        = aUiVqMinMaxDftSmartPic[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_R_GAIN];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_G_GAIN].i4Dft 
        = aUiVqMinMaxDftSmartPic[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_G_GAIN];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_B_GAIN].i4Dft 
        = aUiVqMinMaxDftSmartPic[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_B_GAIN];


    //3D 20120423
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_BRIGHTNESS].i4Dft 
        = aUiVqMinMaxDftSmartPic3D[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_BRIGHTNESS];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_CONTRAST].i4Dft 
        = aUiVqMinMaxDftSmartPic3D[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_CONTRAST];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_HUE].i4Dft 
        = aUiVqMinMaxDftSmartPic3D[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_HUE];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_SATURATION].i4Dft 
        = aUiVqMinMaxDftSmartPic3D[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_SATURATION];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_SKIN].i4Dft 
        = aUiVqMinMaxDftSmartPic3D[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_SKIN_TONE];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_R_GAIN].i4Dft 
        = aUiVqMinMaxDftSmartPic3D[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_R_GAIN];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_G_GAIN].i4Dft 
        = aUiVqMinMaxDftSmartPic3D[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_G_GAIN];
    aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_B_GAIN].i4Dft 
        = aUiVqMinMaxDftSmartPic3D[u1PicMode][u1SrcTypTmg][UI_SMART_PIC_B_GAIN];
    

    if (bApiGetPeUiDebugMode() == SV_ON)  //Caihua mark for debug
    {
        printf(">>>>>vApiUpdateUIDefaultRange:Set Pic %d Timing %d : B %d  C %d  H %d  S %d  Sharp %d Skin Tone %d R Gain %d G Gain %d B Gain %d\n", u1PicMode, u1SrcTypTmg,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_BRIGHTNESS].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_CONTRAST].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_HUE].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SATURATION].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SHARPNESS].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SKIN_COLOR].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_R_GAIN].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_G_GAIN].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_B_GAIN].i4Dft);
        printf(">>>>>3D vApiUpdateUIDefaultRange:Set Pic %d Timing %d : B %d  C %d  H %d  S %d  Sharp %d Skin Tone %d R Gain %d G Gain %d B Gain %d\n", u1PicMode, u1SrcTypTmg,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_BRIGHTNESS].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_CONTRAST].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_HUE].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_SATURATION].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SHARPNESS].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_SKIN].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_R_GAIN].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_G_GAIN].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_B_GAIN].i4Dft);
    }
}

void vApiSetPicModeDef(UINT8 ucVdpId, UINT8 u1SrcTypTmg)
{    
    PE_SetArg(ucVdpId, PE_ARG_BRIGHTNESS, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_BRIGHTNESS].i4Dft);
    PE_SetArg(ucVdpId, PE_ARG_CONTRAST, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_CONTRAST].i4Dft);
    PE_SetArg(ucVdpId, PE_ARG_HUE, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_HUE].i4Dft); 
    PE_SetArg(ucVdpId, PE_ARG_SATURATION, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SATURATION].i4Dft); 
    PE_SetArg(ucVdpId, PE_ARG_SHARPNESS, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SHARPNESS].i4Dft); 
    PE_SetArg(ucVdpId, PE_ARG_ADAPTIVE_BACK_LIGHT, 0);   

    //Caihua Add 20111105
    PE_SetArg(ucVdpId, PE_ARG_SKIN_COLOR, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SKIN_COLOR].i4Dft); 
    PE_SetArg(ucVdpId, PE_ARG_R_GAIN, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_R_GAIN].i4Dft); 
    PE_SetArg(ucVdpId, PE_ARG_G_GAIN, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_G_GAIN].i4Dft); 
    PE_SetArg(ucVdpId, PE_ARG_B_GAIN, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_B_GAIN].i4Dft); 
    PE_SetArg(ucVdpId, PE_ARG_PIC_UI_DFT, 0);

    //3D 20120423
    PE_SetArg(ucVdpId, PE_ARG_TDTV_BRIGHTNESS, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_BRIGHTNESS].i4Dft);
    PE_SetArg(ucVdpId, PE_ARG_TDTV_CONTRAST, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_CONTRAST].i4Dft);
    PE_SetArg(ucVdpId, PE_ARG_TDTV_HUE, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_HUE].i4Dft); 
    PE_SetArg(ucVdpId, PE_ARG_TDTV_SATURATION, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_SATURATION].i4Dft);
    PE_SetArg(ucVdpId, PE_ARG_TDTV_SKIN, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_SKIN].i4Dft); 
    PE_SetArg(ucVdpId, PE_ARG_TDTV_R_GAIN, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_R_GAIN].i4Dft); 
    PE_SetArg(ucVdpId, PE_ARG_TDTV_G_GAIN, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_G_GAIN].i4Dft); 
    PE_SetArg(ucVdpId, PE_ARG_TDTV_B_GAIN, aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_B_GAIN].i4Dft); 

    if (bApiGetPeUiDebugMode() == SV_ON)  //Caihua mark for debug
    {
        printf(">>>>>vApiSetPicModeDef:Set Timing %d : B %d  C %d  H %d  S %d  Sharp %d Skin Tone %d R Gain %d G Gain %d B Gain %d\n", u1SrcTypTmg,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_BRIGHTNESS].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_CONTRAST].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_HUE].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SATURATION].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SHARPNESS].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_SKIN_COLOR].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_R_GAIN].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_G_GAIN].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_B_GAIN].i4Dft);
        printf(">>>>>vApiSetPicModeDef:Set 3D Timing %d : B %d  C %d  H %d  S %d  Skin Tone %d R Gain %d G Gain %d B Gain %d\n", u1SrcTypTmg,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_BRIGHTNESS].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_CONTRAST].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_HUE].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_SATURATION].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_SKIN].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_R_GAIN].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_G_GAIN].i4Dft,
            aUiVqMinMaxDft[u1SrcTypTmg][PE_ARG_TDTV_B_GAIN].i4Dft);
    }
}
// End add for Vestel --- pingna 0606

/**
 * @brief Mapping from UI_min, UI_max, UI_dft, UI_cur, HW_min, HW_max, HW_dft, to HW cur.
 * @param u1VidPath SV_VP_MAIN/SV_VP_PIP
 * @param u2QtyItem VDP_ARG_xxx
 * @param pi2Min, pi2Max, pi2Dft, pi2Cur: UI values.
 */
UINT16 bApiQtyMapping(UINT8 u1SrcTypTmg, UINT16 u2QtyItem,
                     INT16 const i2Min, INT16 const i2Max,
                     INT16 const i2Dft, INT16 const i2Cur)
{
    PE_UI_RANGE_T UiRange;
    UINT16 ReturnVal = 0;

    if (u2QtyItem >= QUALITY_MAX)
    {
        LOG(0, "API_PEUI Error: DEFUALT_QTY_TBL range index error! %d\n", u2QtyItem);
        return 0;
    }

    UiRange = GetQtyRange(u1SrcTypTmg, u2QtyItem);
    LOG(2," $$$ min max dft [%d] [%d] [%d] \n",UiRange.i4Min,UiRange.i4Max,UiRange.i4Dft); //temp 
    
    // Some qty_item does not need mapping, return default HW value.
    // or UI_cur == UI_dft, return default HW value.
    if (((UiRange.i4Min == 0) && (UiRange.i4Max == 0)) || (i2Cur == i2Dft))
    {
        return UiRange.i4Dft;
    }

    // qty_item use 2-level setting as min/max/default, 
    // when dft=1 : don't mapping.
    // when dft=0 or 2 : use interpolation to prevent UI no effect.
    if (i2Max == 2)           
    {
        switch (i2Dft)
        {
        case 0:
            switch(i2Cur)               
            {
            case 0:
                ReturnVal = UiRange.i4Dft;
            break;
            case 1:
                ReturnVal = (UiRange.i4Max+UiRange.i4Dft)/2;
            break;
            case 2:
                ReturnVal = UiRange.i4Max;
            break;                
            }
        break;
        case 1:
            switch(i2Cur)               
            {
            case 0:
                ReturnVal = UiRange.i4Min;
            break;                
            case 1:
                ReturnVal = UiRange.i4Dft;
            break;                
            case 2:
                ReturnVal = UiRange.i4Max;
            break;                
            }        
        break;            
        case 2:
            switch(i2Cur)               
            {
            case 0:
                ReturnVal = UiRange.i4Min;
            break;                
            case 1:
                ReturnVal = (UiRange.i4Min+UiRange.i4Dft)/2;
            break;                
            case 2:
                ReturnVal = UiRange.i4Dft;
            break;                
            }
        break;            
        default:
            ASSERT(0);
        break;            
        }
        return ReturnVal;
    }

    // Some qty_item use 3-level setting as min/max/default, don't mapping.
    // eg. NR registers don't fit linear mapping, use 3-level LUT instead.
    // ==> UI_max=3 means UI option is: off, low, mid, hi.
    // ==> Use "R_OFF"
/*
    if(!DRVCUST_OptGet(eFlagUIQTYLinearMap))
    {
    if (i2Max == 3)           
    {
        if (i2Cur == 1)
        {
            return UiRange.i4Min;
        }
        else if (i2Cur == 2)
        {
            return UiRange.i4Dft;
        }
        else if (i2Cur == 3)
        {
            return UiRange.i4Max;
        }
    }
    }
*/    
    // Get HW value for UI_current > UI_default.
    if (i2Cur > i2Dft)
    {
        // Prevent UI_max, UI_dft miss setting, and div=0.
        if ((i2Max - i2Dft) <= 0)
        {
            ReturnVal = UiRange.i4Dft;
        }
        else
        {
            ReturnVal = (UINT16) ((i2Cur - i2Dft) * (UiRange.i4Max - UiRange.i4Dft) /
                        (i2Max - i2Dft) + UiRange.i4Dft);
        }
    }
    // Get HW value for UI_current < UI_default.
    else
    {
        // Prevent UI_dft, UI_min miss setting, and div=0.
        if ((i2Dft - i2Min) <= 0)
        {
            ReturnVal = UiRange.i4Dft;
        }
        else
        {
            ReturnVal = (UINT16) ((i2Cur -i2Min) * (UiRange.i4Dft - UiRange.i4Min) /
                            (i2Dft - i2Min) + UiRange.i4Min);
        }
    }

    //LOG(1, "bApiQtyMapping: Value=%d, u1Min=%d u1Max=%d u1Dft=%d\n", ReturnVal, UiRange.i4Min, UiRange.i4Max, UiRange.i4Dft);
    
    return ReturnVal;
}

void PE_SetUiMinMaxDft(void)
{
    INT32 i4VisIdx;
    static UINT8 bUiInit = SV_FALSE;

    if (bUiInit != SV_FALSE)
    {
        LOG(5, "PE UI aleady Init !\n");
        return;
    }

    // Set all UI video quality item of input source's
    // min/max/default to default value.
    for (i4VisIdx = 0; i4VisIdx < (INT32)FLASHPQ_TOTAL_TIMING; i4VisIdx++)
    {
        x_memcpy(&aUiVqMinMaxDft[i4VisIdx],
                 aUiVqMinMaxDftDefault,
                 sizeof(aUiVqMinMaxDftDefault));
    } 

    // Set all UI video quality items' current to default value.
    PE_SetAllUiVqItemDftValues();
    bUiInit = SV_TRUE;
}

UINT32 u4ApiSetUiVqItemDftValue(UCHAR u1VidPath, UCHAR ucArg, INT16 i2Value)
{

    if (ucArg >= (UCHAR) PE_ARG_NS)
    {
        return SV_FAIL;
    }

    ai2Arg[u1VidPath][ucArg] = i2Value;

    return SV_SUCCESS;
}

UINT8 u1ApiVidInSrcType2VisType(UINT8 u1SrcTypTmg)
{
LINT_SAVE_AND_DISABLE
    switch (u1SrcTypTmg)
    {
    case SV_VS_TUNER1:
        return (UINT8)EVDP_VIS_TUNER_ANALOG;

    case SV_VS_CVBS1:
    case SV_VS_CVBS2:
    case SV_VS_CVBS3:
        return (UINT8)EVDP_VIS_AVC_COMP_VIDEO;

    case SV_VS_S1:
    case SV_VS_S2:
    case SV_VS_S3:
        return (UINT8)EVDP_VIS_AVC_S_VIDEO;

    case SV_VS_YPbPr1:
    case SV_VS_YPbPr2:
    case SV_VS_YPbPr3:
        return (UINT8)EVDP_VIS_AVC_Y_PB_PR;

    case SV_VS_HDMI1:
    case SV_VS_HDMI2:
        return (UINT8)EVDP_VIS_AVC_HDMI;

    case SV_VS_VGA:
        return (UINT8)EVDP_VIS_AVC_VGA;

    case SV_VS_DVI:
        return (UINT8)EVDP_VIS_AVC_DVI;

    case SV_VS_DT1:     // DTV
//    case SV_VS_DT2:     // DTV
        return (UINT8)EVDP_VIS_TUNER_DIGITAL;

//    case SV_VS_SCART:
//    case SV_VS_SCART1:
//    case SV_VS_SCART2:
//    case SV_VS_SCART3:
//        return (UINT8)EVDP_VIS_AVC_SCART;

    default:
        return (UINT8)EVDP_VIS_MAX;
    }
LINT_RESTORE
}

/**
 * VDP get min, max, default, and current values of UI video quality items.
 *
 * @param void
 * @return void
 */
/*lint -save -e818 -e830 */
UINT32 PE_GetUiRange(UINT8 u1VidPath, PE_ARG_TYPE ucArgType, PE_UI_RANGE_T *prUiRange)        //Add for vestel --- pingan 0608
{
    INT32 i4Offset = 0;
    UINT8 u1SrcTypTmg, temptimming = 0;
    PE_UI_RANGE_T *prVqTbl;

    if (ucArgType >= (UCHAR) PE_ARG_NS)
    {
        return SV_FAIL;
    }
    u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(u1VidPath);
    temptimming = u1SrcTypTmg; //temporary add
    u1SrcTypTmg = bApiGetCustomSourceTypeTiming(u1SrcTypTmg);
  
    if(u1SrcTypTmg > 17) //temporary add
    {
        //printf("\nPE_GetUiRange  sourcetimming=%d,mappingTmg=%d !!!\n",temptimming,u1SrcTypTmg);
        u1SrcTypTmg = 0;
    }
    
    prVqTbl = &aUiVqMinMaxDft[u1SrcTypTmg][ucArgType];

    i4Offset = ((prVqTbl->i4Min < 0) ? prVqTbl->i4Min : 0);

    prUiRange->i4Min = (INT16)(prVqTbl->i4Min - i4Offset);
    prUiRange->i4Max = (INT16)(prVqTbl->i4Max - i4Offset);
    prUiRange->i4Dft = (INT16)(prVqTbl->i4Dft - i4Offset);
    return SV_SUCCESS;
}
/*lint -restore */

void PE_SetUiRange(UINT8 u1VdpVisType, PE_ARG_TYPE ucArgType, PE_UI_RANGE_T *prUiRange)
{
    if ((u1VdpVisType < (UINT8)FLASHPQ_TOTAL_TIMING) &&
        ((UINT8)ucArgType < (UINT8)PE_ARG_NS) &&
        (prUiRange != NULL))
    {
        aUiVqMinMaxDft[u1VdpVisType][ucArgType].i4Min = prUiRange->i4Min;
        aUiVqMinMaxDft[u1VdpVisType][ucArgType].i4Max = prUiRange->i4Max;
        aUiVqMinMaxDft[u1VdpVisType][ucArgType].i4Dft = prUiRange->i4Dft;
    }
    else
    {
        Printf("PE_SetUiRange error! %d, %d, %ld\n",
            u1VdpVisType, ucArgType, (UINT32)prUiRange);
    }
}

void PE_SetAllUiVqItemDftValues(void)
{
    UCHAR ucVidPath;
    INT16 i2Default;
    INT32 i4UiVqItem, i4Offset, i4VdpVisType;
    PE_UI_RANGE_T *prVqTbl;

    // Use cvbs as default
    i4VdpVisType = 0;//(INT32)EVDP_VIS_AVC_COMP_VIDEO;

    for (ucVidPath = 0; ucVidPath < VDP_NS; ucVidPath++)
    {
        for (i4UiVqItem = 0; i4UiVqItem < ((INT32)PE_ARG_NS); i4UiVqItem++)
        {
            prVqTbl = &aUiVqMinMaxDft[i4VdpVisType][i4UiVqItem];
            i4Offset = ((prVqTbl->i4Min < 0) ? prVqTbl->i4Min : 0);
            i2Default = (INT16)(prVqTbl->i4Dft - i4Offset);

            VERIFY(u4ApiSetUiVqItemDftValue(ucVidPath, i4UiVqItem,
                            i2Default) == SV_SUCCESS);
        }
    }
}

UINT32 PE_SetArg(UCHAR ucVdpId, PE_ARG_TYPE ucArg, INT16 i2Value)
{
    if ((ucVdpId >= (UCHAR) VDP_NS) || (ucArg >= (UCHAR) PE_ARG_NS))
    {
        return SV_FAIL;
    }

    ai2Arg[ucVdpId][ucArg] = i2Value;

    if (bApiVideoProc(ucVdpId, ucArg) != SV_TRUE)
    {
        return SV_FAIL;
    }

    return SV_SUCCESS;
}

UINT32 PE_GetArg(UCHAR ucVdpId, PE_ARG_TYPE ucArg, INT16* pi2Value)
{
    if (pi2Value==NULL)
    {
        return SV_FAIL;
    }

    if ((ucVdpId >= (UCHAR) VDP_NS) || (ucArg >= (UCHAR) PE_ARG_NS))
    {
        return SV_FAIL;
    }

    *pi2Value = ai2Arg[ucVdpId][ucArg];

    return SV_SUCCESS;
}

UINT8 bApiGetPeUiDebugMode(void)
{
    return bDrvGetPeUiDebugMode();
}

void vApiSetPeUiDebugMode(UINT8 bOnOff)
{
    vDrvSetPeUiDebugMode(bOnOff);
}

// ============================================================================================================
void vUi_CTI(UINT8 u1VidPath, UINT8 bTurnOff)
{
    if (bTurnOff)
    {
        vHalVideoCTIOnOff(u1VidPath, SV_OFF);
    }
    else
    {
        vHalVideoCTIOnOff(u1VidPath, SV_ON);
        vHalVideoCTIParam(u1VidPath);
    }
}

void vUi_CDS(UINT8 u1VidPath, UINT8 bTurnOff)
{
}

void vUi_LTI(UINT8 u1VidPath, UINT8 bTurnOff)
{
    if (bTurnOff)
    {
        vHalVideoLTIOnOff(u1VidPath, SV_OFF);
    }
    else
    {
        vHalVideoLTIOnOff(u1VidPath, SV_ON);
        vHalVideoLTIParam(u1VidPath);
    }
}

void vUi_3D_NR(UINT8 u1VidPath, UINT8 u1Cur, UINT8 u1Auto)
{
    if (u1VidPath == SV_VP_MAIN)
    {
        if(u1Cur == 0)    //vestel, not close NR in any case,pingan 120712
        {
            wWriteQualityTable(QUALITY_3DNR_TNR_ACNR_STRENGTH, 0x4);
            u1Cur = 1;
        }
        vHalVideo3DNRParam(u1Cur);
        vHalVideo3DAutoNROnOff(u1Auto);
    }
}

void vUi_NR(UINT8 u1VidPath, UINT8 u1Cur, UINT8 u1Auto)
{
    if (u1VidPath == SV_VP_MAIN)
    {
        if(u1Cur == 0)    //vestel, not close NR in any case,pingan 120712
        {
            u1Cur = 1;
        }
        //u1Auto = 1;   //open auto nr
        vHalVideo2DNRParam(u1Cur);
        vHalVideo2DAutoNROnOff(u1Auto);
    }
}

void vUi_MPEG_NR(UINT8 u1VidPath, UINT8 u1Cur)
{
    if (u1VidPath == SV_VP_MAIN)
        vHalVideoMPEGNRParam(u1Cur);
}

void vUi_CBE(UINT8 u1VidPath,UINT8 bTurnOff)
{   
    if (bTurnOff)
    {
        vHalVideoCBEOnOff(u1VidPath, SV_OFF);
    }
    else
    {
        vHalVideoCBEOnOff(u1VidPath, SV_ON);
        vHalVideoCBEParam(u1VidPath);
    }   
}

void vUi_SCE(UINT8 u1VidPath, UINT8 i2Cur)
{
    LOG(5, "Set SCE Level %d \n", i2Cur);
    
    vApiFlashPqSetSceId(i2Cur);     
    bApiFlashPqUpdateSCE();   
    vHalVideoSCEOnOff(u1VidPath);
    vDrvSCEParam();
    vDrvSetSCELevel((UINT8)wReadQualityTable(QUALITY_SCE_LUMA_GAIN), 
                    (UINT8)wReadQualityTable(QUALITY_SCE_SAT_GAIN), 
                    (UINT8)wReadQualityTable(QUALITY_SCE_HUE_GAIN));
    
    vDrvSetSceRegFromQtyTable(); // add for vestel
}

void vUi_SKIN_COLOR(UINT8 u1VidPath, UINT8 i2Cur)
{
    LOG(5, "Set Skin Level %d \n", i2Cur);
    
    vDrvSetSkinLevel((UINT8)wReadQualityTable(QUALITY_COLOR_SKIN_L), 
                    (UINT8)wReadQualityTable(QUALITY_COLOR_SKIN_S), 
                    (UINT8)wReadQualityTable(QUALITY_COLOR_SKIN_H));
    
    vDrvSetSkinToneRegFromQtyTable(); // add for vestel
}

void vUi_TDTV_SKIN_COLOR(UINT8 u1VidPath, UINT8 i2Cur)
{
    LOG(5, "Set Skin Level %d \n", i2Cur);

    //3D
    vDrvSetTdtvSkinToneRegFromQtyTable();

    if(u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)
    {
        vDrvSetSkinLevel((UINT8)wReadQualityTable(QUALITY_COLOR_3D_SKIN_L), 
                        (UINT8)wReadQualityTable(QUALITY_COLOR_3D_SKIN_S), 
                            (UINT8)wReadQualityTable(QUALITY_COLOR_3D_SKIN_H));
    }
    //end
    
    vHalVideoSCEOnOff(u1VidPath);
}


void vUi_ADAPTIVE_LUMA(UINT8 u1VidPath, UINT8 bTurnOff)
{
    if(bTurnOff)
    {
        vHalVideoAdaptiveLumaOnOff(u1VidPath, SV_OFF);    
    }
    else
    {   
        vHalVideoAdaptiveLumaOnOff(u1VidPath, SV_ON);      
        vDrvADLUpdateParam();
        vDrvChromaBoostParam();  //for vestel
    }
}

void vUi_BLACK_STRETCH(UINT8 u1VidPath, UINT8 bTurnOff)
{
    wWriteQualityTable(QUALITY_AL_BWS_BLACK_ON_1_OFF_0, (UINT16)(!bTurnOff));
    wWriteQualityTable(QUALITY_AL_BWS_BK_EN, (UINT16)(!bTurnOff));
    vHalVideoAdaptiveLumaParam();
}

void vUi_WHITE_STRETCH(UINT8 u1VidPath, UINT8 bTurnOff)
{
    wWriteQualityTable(QUALITY_AL_BWS_WHITE_ON_1_OFF_0, (UINT16)(!bTurnOff));
    wWriteQualityTable(QUALITY_AL_BWS_WT_EN, (UINT16)(!bTurnOff));
    vHalVideoAdaptiveLumaParam();
}

void vUi_BLUE_STRETCH(UINT8 u1VidPath, UINT8 bTurnOff)
{
#ifdef CC_MT5396    
        vApiRegisterVideoEvent(PE_EVENT_BLUE_STRETCH, u1VidPath, !bTurnOff);
#else
        vHalVideoBlueStretchOnOff(u1VidPath, !bTurnOff);
        vDrvSetBlueStretchParam();
#endif
}

void vUi_ADAPTIVE_BACK_LIGHT(UINT8 u1VidPath, UINT8 i2Cur)
{
    //i2Cur==0 means turn off.
    DRVCUST_AdaptiveBacklightParam(bBLMode);
}

void vUi_BACK_LIGHT_LVL(UINT8 u1VidPath, UINT8 bTurnOff)
{
    DRVCUST_SetBacklightDuty(VDP_ARG_BACKLT_UI, wReadQualityTable(QUALITY_BACKLIGHT));
}

void vUi_WHITE_PEAK_LMT(UINT8 u1VidPath, UINT8 bTurnOff)
{
    vHalVideoWhitePeakLimit(!bTurnOff);
}

void vUi_GAMMA(UINT8 u1VidPath, UINT8 i2Cur)
{       
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_gamma(i2Cur, NULL);
    }
    else
#endif
    {
        vHalVideoGammaTblUpdate(i2Cur);        
        vHalVideoGammaLoadTable();
    }
}

void vUi_SHARPNESS_ON_OFF(UINT8 u1VidPath, UINT8 bTurnOff)
{
    vHalVideoSharpnessOnOff(u1VidPath, !bTurnOff);
}

void vUi_USPEI(UINT8 u1VidPath, UINT8 bTurnOff)
{
    vHalVideoUSPEIOnOff(u1VidPath, !bTurnOff);
}

void vUI_LCDIM(UINT8 u1VidPath, UINT8 i2Cur)
{
    vHalVideoLcDimOnOff(u1VidPath, i2Cur);
}

void vUi_SHARPNESS(UINT8 u1VidPath, UINT8 bTurnOff)
{
    vHalVideoLTIOnOff(u1VidPath, SV_ON);
    vHalVideoSharpnessOnOff(u1VidPath, SV_ON);

    vHalVideoLTIParam(u1VidPath);
    vHalVideoSharpnessParam(u1VidPath);
}

void vUi_SHARPNESS_HORIZONTAL(UINT8 u1VidPath, UINT8 bTurnOff)
{
    vHalVideoSharpnessHorizontalOnOff(u1VidPath, !bTurnOff);
    vHalVideoSharpnessParamHorizontal(u1VidPath);
}

void vUi_SHARPNESS_VERTICAL(UINT8 u1VidPath, UINT8 bTurnOff)
{
    vHalVideoSharpnessVerticalOnOff(u1VidPath, !bTurnOff);
    vHalVideoSharpnessParamVertical(u1VidPath);
}

void vUi_SHARPNESS_DIAGONAL(UINT8 u1VidPath, UINT8 bTurnOff)
{
    vHalVideoSharpnessDiagonalOnOff(u1VidPath, !bTurnOff);
    vHalVideoSharpnessParamDiagonal(u1VidPath);
}

void vUi_SUPERRESOLUTION_ON_OFF(UINT8 u1VidPath, UINT8 bTurnOff)
{
    vHalVideoSuperResolutionOnOff(u1VidPath, !bTurnOff);
}

void vUi_BRIGHTNESS(UINT8 u1VidPath, UINT8 bTurnOff)
{
    vHalVideoBrightness(u1VidPath, wReadQualityTable(QUALITY_OSD_MAPPING_BRIGHTNESS));
}

void vUi_TDTV_BRIGHTNESS(UINT8 u1VidPath, UINT8 bTurnOff)  // add for vestel
{
    vHalTdtvVideoBrightness(u1VidPath, wReadQualityTable(QUALITY_OSD_MAPPING_3D_BRIGHTNESS));
}

void vUi_CONTRAST(UINT8 u1VidPath, UINT8 bTurnOff)
{
    vHalVideoContrast(u1VidPath, wReadQualityTable(QUALITY_OSD_MAPPING_CONTRAST) 
                    | (wReadQualityTable(QUALITY_OSD_MAPPING_CONTRAST_MSB) << 8));

    if(DRVCUST_OptGet(eFlagBindContrastSaturation))
    {
        bApiVideoProc(u1VidPath, PE_ARG_SATURATION);
    }
}

void vUi_TDTV_CONTRAST(UINT8 u1VidPath, UINT8 bTurnOff)  // add for vestel
{
    vHalTdtvVideoContrast(u1VidPath, wReadQualityTable(QUALITY_OSD_MAPPING_3D_CONTRAST) 
                    | (wReadQualityTable(QUALITY_OSD_MAPPING_3D_CONTRAST_MSB) << 8));

    if(DRVCUST_OptGet(eFlagBindContrastSaturation))
    {
        bApiVideoProc(u1VidPath, PE_ARG_TDTV_SATURATION);
    }
}

void vUi_HUE(UINT8 u1VidPath, UINT8 bTurnOff)
{
    vHalVideoHue(u1VidPath, wReadQualityTable(QUALITY_OSD_MAPPING_HUE));
}

void vUi_TDTV_HUE(UINT8 u1VidPath, UINT8 bTurnOff) // add for vestel
{
    vHalTdtvVideoHue(u1VidPath, wReadQualityTable(QUALITY_OSD_MAPPING_3D_HUE));
}

void vUi_SATURATION(UINT8 u1VidPath, UINT8 u1SrcTypTmg)
{
    UINT32 u4Tmp;
    if(DRVCUST_OptGet(eFlagBindContrastSaturation))
    {
        UINT16 u2ContrastDft = bApiGetQtyDft(QUALITY_OSD_MAPPING_CONTRAST, u1SrcTypTmg) | ((UINT16)bApiGetQtyDft(QUALITY_OSD_MAPPING_CONTRAST_MSB, u1SrcTypTmg) << 8);
        UINT16 u2SatQty = wReadQualityTable(QUALITY_OSD_MAPPING_SATURATION) | ((UINT16)wReadQualityTable(QUALITY_OSD_MAPPING_SATURATION_MSB) << 8);
        UINT16 u2ContrastQty = wReadQualityTable(QUALITY_OSD_MAPPING_CONTRAST) | ((UINT16)wReadQualityTable(QUALITY_OSD_MAPPING_CONTRAST_MSB) << 8);

        if(u2ContrastDft)
        {
            u4Tmp = ((UINT32)u2SatQty * u2ContrastQty) / u2ContrastDft; 
            //LOG(0, "IN  --vUi_SATURATION u4temp:[%d]   \n",u4Tmp);
            wWriteQualityTable(QUALITY_OSD_MAPPING_SATURATION, u4Tmp & 0xFF);
            wWriteQualityTable(QUALITY_OSD_MAPPING_SATURATION_MSB, u4Tmp >> 8);
        }
    }
    vHalVideoSaturation(u1VidPath, wReadQualityTable(QUALITY_OSD_MAPPING_SATURATION) | (wReadQualityTable(QUALITY_OSD_MAPPING_SATURATION_MSB) << 8));
}

void vUi_TDTV_SATURATION(UINT8 u1VidPath, UINT8 u1SrcTypTmg) // add for vestel
{
    UINT32 u4Tmp;

    if(DRVCUST_OptGet(eFlagBindContrastSaturation))
    {
        UINT16 u2ContrastDft3D = bApiGetQtyDft(QUALITY_OSD_MAPPING_3D_CONTRAST, u1SrcTypTmg) | ((UINT16)bApiGetQtyDft(QUALITY_OSD_MAPPING_3D_CONTRAST_MSB, u1SrcTypTmg) << 8);
        UINT16 u2SatQty3D = wReadQualityTable(QUALITY_OSD_MAPPING_3D_SATURATION) | ((UINT16)wReadQualityTable(QUALITY_OSD_MAPPING_3D_SATURATION_MSB) << 8);
        UINT16 u2ContrastQty3D = wReadQualityTable(QUALITY_OSD_MAPPING_3D_CONTRAST) | ((UINT16)wReadQualityTable(QUALITY_OSD_MAPPING_3D_CONTRAST_MSB) << 8);

        if(u2ContrastDft3D)
        {
            u4Tmp = ((UINT32)u2SatQty3D * u2ContrastQty3D) / u2ContrastDft3D;
            wWriteQualityTable(QUALITY_OSD_MAPPING_3D_SATURATION, u4Tmp & 0xFF);
            wWriteQualityTable(QUALITY_OSD_MAPPING_3D_SATURATION_MSB, u4Tmp >> 8);
        }
    }
    vHalTdtvVideoSaturation(u1VidPath, wReadQualityTable(QUALITY_OSD_MAPPING_3D_SATURATION) | (wReadQualityTable(QUALITY_OSD_MAPPING_3D_SATURATION_MSB) << 8));
}


void vUi_DI_FILM_MODE(UINT8 u1VidPath, UINT8 bTurnOff, UINT8 i2Cur)
{
    if(bTurnOff)
    {
        vDrvDIFilmModeOnOff(u1VidPath, SV_OFF);
    }
    else
    {
        vDrvDIFilmModeOnOff(u1VidPath, SV_ON);

        //Slow Motion (1) Map to Aggressive ,Big Motion (2) Map to Defensive
        vDrvDISetQualityMode(E_DI_ENGINE_FILM, i2Cur==SV_CN_MOVIE ?
        E_DI_QUALITY_AGGRESSIVE : E_DI_QUALITY_DEFENSIVE);
    }
    vDrvDISetFilmModeParam();
}

void vUi_DI_MA(UINT8 u1VidPath, UINT8 i2Cur)
{
    vDrvDISetQualityMode(E_DI_ENGINE_IF, i2Cur==SV_MA_SLOW ?
        E_DI_QUALITY_AGGRESSIVE : E_DI_QUALITY_DEFENSIVE);
}

void vUi_DI_EDGE_PRESERVE(UINT8 u1VidPath, UINT8 i2Cur)
{
    vDrvDISetQualityMode(E_DI_ENGINE_PE, i2Cur==SV_V_EDGE_LOW ?
        E_DI_QUALITY_DEFENSIVE : E_DI_QUALITY_AGGRESSIVE);
}

void vUi_R_GAIN(UINT8 u1VidPath, UINT8 bTurnOff)
{
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_color_temperature(E_EXTMJC_CT_R_GAIN1, wReadQualityTable(QUALITY_OSD_MAPPING_R_GAIN));
    }
    else
#endif
    {
#ifdef CC_MT5396    
        vApiRegisterVideoEvent(PE_EVENT_R_GAIN, u1VidPath, SV_ON);
#else
        vDrvSetRGain1FromQtyTable();                               //add for vestel
        vHalVideoRGBGain(u1VidPath, SV_RGB_R, wReadQualityTable(QUALITY_OSD_MAPPING_R_GAIN));
#endif        
    }
}

void vUi_TDTV_R_GAIN(UINT8 u1VidPath, UINT8 bTurnOff) //add for vestel
{
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_color_temperature(E_EXTMJC_CT_R_GAIN1, wReadQualityTable(QUALITY_OSD_MAPPING_3D_R_GAIN));
    }
    else
#endif
    {
#ifdef CC_MT5396    
        vApiRegisterVideoEvent(PE_EVENT_R_GAIN, u1VidPath, SV_ON);
#else
        vDrvSetTDTVRGain1FromQtyTable();  
        vHalVideoRGBGain(u1VidPath, SV_RGB_R, wReadQualityTable(QUALITY_OSD_MAPPING_3D_R_GAIN));
#endif
    }
}

void vUi_G_GAIN(UINT8 u1VidPath, UINT8 bTurnOff)
{
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_color_temperature(E_EXTMJC_CT_G_GAIN1, wReadQualityTable(QUALITY_OSD_MAPPING_G_GAIN));
    }
    else
#endif
    {
#ifdef CC_MT5396    
        vApiRegisterVideoEvent(PE_EVENT_G_GAIN, u1VidPath, SV_ON);
#else
        vDrvSetGGain1FromQtyTable();   //add for vestel 
        vHalVideoRGBGain(u1VidPath, SV_RGB_G, wReadQualityTable(QUALITY_OSD_MAPPING_G_GAIN));
#endif
    }
}

void vUi_TDTV_G_GAIN(UINT8 u1VidPath, UINT8 bTurnOff)  //add for vestel 
{
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_color_temperature(E_EXTMJC_CT_G_GAIN1, wReadQualityTable(QUALITY_OSD_MAPPING_3D_G_GAIN));
    }
    else
#endif
    {
#ifdef CC_MT5396    
            vApiRegisterVideoEvent(PE_EVENT_G_GAIN, u1VidPath, SV_ON);
#else
            vDrvSetTDTVGGain1FromQtyTable();//Caihua Add 20120408
            vHalVideoRGBGain(u1VidPath, SV_RGB_G, wReadQualityTable(QUALITY_OSD_MAPPING_3D_G_GAIN));
#endif
    }
}

void vUi_B_GAIN(UINT8 u1VidPath, UINT8 bTurnOff)
{
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_color_temperature(E_EXTMJC_CT_B_GAIN1, wReadQualityTable(QUALITY_OSD_MAPPING_B_GAIN));
    }
    else
#endif
    {
#ifdef CC_MT5396    
        vApiRegisterVideoEvent(PE_EVENT_B_GAIN, u1VidPath, SV_ON);
#else
        vDrvSetBGain1FromQtyTable();   //add for vestel 
        vHalVideoRGBGain(u1VidPath, SV_RGB_B, wReadQualityTable(QUALITY_OSD_MAPPING_B_GAIN));
#endif
    }
}

void vUi_TDTV_B_GAIN(UINT8 u1VidPath, UINT8 bTurnOff) //add for vestel 
{
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_color_temperature(E_EXTMJC_CT_B_GAIN1, wReadQualityTable(QUALITY_OSD_MAPPING_3D_B_GAIN));
    }
    else
#endif
    {
#ifdef CC_MT5396    
        vApiRegisterVideoEvent(PE_EVENT_B_GAIN, u1VidPath, SV_ON);
#else
        vDrvSetTDTVBGain1FromQtyTable();//Caihua Add 20120408
        vHalVideoRGBGain(u1VidPath, SV_RGB_B, wReadQualityTable(QUALITY_OSD_MAPPING_3D_B_GAIN));
#endif
    }
}

void vUi_R_OFFSET(UINT8 u1VidPath, UINT8 bTurnOff)
{
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_color_temperature(E_EXTMJC_CT_R_OFST1, wReadQualityTable(QUALITY_R_OFFSET));
    }
    else
#endif
    {
#ifdef CC_MT5396    
        vApiRegisterVideoEvent(PE_EVENT_R_OFFSET, u1VidPath, SV_ON);
#else
        vHalVideoRGBOffset(u1VidPath, SV_RGB_R, wReadQualityTable(QUALITY_R_OFFSET));
#endif  
    }
}

void vUi_G_OFFSET(UINT8 u1VidPath, UINT8 bTurnOff)
{
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_color_temperature(E_EXTMJC_CT_G_OFST1, wReadQualityTable(QUALITY_G_OFFSET));
    }
    else
#endif
    {
#ifdef CC_MT5396    
        vApiRegisterVideoEvent(PE_EVENT_G_OFFSET, u1VidPath, SV_ON);
#else
        vHalVideoRGBOffset(u1VidPath, SV_RGB_G, wReadQualityTable(QUALITY_G_OFFSET));
#endif  
    }    
}

void vUi_B_OFFSET(UINT8 u1VidPath, UINT8 bTurnOff)
{
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_color_temperature(E_EXTMJC_CT_B_OFST1, wReadQualityTable(QUALITY_B_OFFSET));
    }
    else
#endif
    {
#ifdef CC_MT5396    
        vApiRegisterVideoEvent(PE_EVENT_B_OFFSET, u1VidPath, SV_ON);
#else
        vHalVideoRGBOffset(u1VidPath, SV_RGB_B, wReadQualityTable(QUALITY_B_OFFSET));
#endif  
    }
}

void vUi_xvYCC(UINT8 i2Cur)
{
// only 5396 support xvycc, 5368 does not support
#ifdef CC_MT5396    
    vDrvxvYCCOnOff(i2Cur);
#else
    LOG(0, "NOT support xvYCC\n");
#endif
}
       
void vUi_DEMO(UINT8 u1VidPath, UINT8 i2Cur)
{
    Region rDummyRegion;
    //support only Split demo.
    rDummyRegion.u1OnOff = SV_ON;
    rDummyRegion.wHEnd = 200;
    rDummyRegion.wHStart=0;
    rDummyRegion.wVEnd = 200;
    rDummyRegion.wVStart = 0;
    //vApiVideoDemoMode(i2Cur, rDummyRegion, 0xF0);
    #if 0
    vApiVideoDemoMode(((i2Cur == SV_ON) ? DRVCUST_DemoRegion() : VDP_PQ_DEMO_OFF), rDummyRegion, 0xF0);  
    #else
    vApiVideoDemoMode(i2Cur,  rDummyRegion, 0xFC);
    #ifdef CC_FLIP_MIRROR_SUPPORT
        UINT8 u1FlipMirrorEnable= u1GetFlipMirrorConfig() ;
        if((i2Cur == 1)&&(u1FlipMirrorEnable & SYS_MIRROR_CONFIG_ON))
        {
            vApiVideoDemoMode(VDP_PQ_DEMO_ON_LEFT,  rDummyRegion, 0xFC);
        }
    #endif
    #endif
}

void vUi_MJC_EFFECT(UINT8 u1VidPath, UINT8 i2Cur)
{
    #ifdef DRV_SUPPORT_EXTMJC
    if (i2Cur == VDP_ARG_MJC_EFFECT_OFF)
    {
        bMjcUiEffect = MJC_CTRL_EFCT_OFF;
    }
    else if (i2Cur == VDP_ARG_MJC_EFFECT_LOW)
    {
        bMjcUiEffect = MJC_CTRL_EFCT_LOW;
    }
    else if (i2Cur == VDP_ARG_MJC_EFFECT_MIDDLE)
    {
        bMjcUiEffect = MJC_CTRL_EFCT_MID;
    }
    else
    {
        bMjcUiEffect = MJC_CTRL_EFCT_HIGH;
    }
    #else
        #ifdef CC_MT5396
        UINT32 u4Effect;

        switch (i2Cur)
        {
        default:
        case VDP_ARG_MJC_EFFECT_OFF:
            u4Effect = MJC_CTRL_EFCT_OFF;
            break;
        case VDP_ARG_MJC_EFFECT_LOW:
            u4Effect = MJC_CTRL_EFCT_LOW;
            break;
        case VDP_ARG_MJC_EFFECT_MIDDLE:
            u4Effect = MJC_CTRL_EFCT_MID;
            break;
        case VDP_ARG_MJC_EFFECT_HIGH:
            u4Effect = MJC_CTRL_EFCT_HIGH;
            break;
        }

        if (u1VidPath == SV_VP_MAIN)
        {
            MJC_SetEffect(u4Effect);
        }

        #endif
    #endif
}

void vUi_MJC_MODE(UINT8 u1VidPath, UINT8 i2Cur)
{
    #ifdef DRV_SUPPORT_EXTMJC
    bMjcUiMode = i2Cur;
    #else
        #ifdef CC_MT5396
        if (u1VidPath == SV_VP_MAIN)
        {
            switch (i2Cur)
            {
            default:
            case VDP_ARG_MJC_MODE_NORM:
                MJC_SetClearLCD(SV_ON);
                MJC_SetFilmMode(SV_ON);
                break;
            case VDP_ARG_MJC_MODE_NO_DEBLUR:
                MJC_SetClearLCD(SV_OFF);
                MJC_SetFilmMode(SV_ON);
                break;
            case VDP_ARG_MJC_MODE_NO_FILM:
                MJC_SetClearLCD(SV_ON);
                MJC_SetFilmMode(SV_OFF);
                break;
            }
            fgMjcClearLcdDemo = i2Cur;
        }
        #endif
    #endif
}
           
void vUi_MJC_DEMO(UINT8 u1VidPath, UINT8 i2Cur)
{
    #ifdef DRV_SUPPORT_EXTMJC
    bMjcUiDemo = i2Cur;
    vMJCSetDemoMode(bMjcUiEffect, bMjcUiMode, bMjcUiDemo);
    #else
        #ifdef CC_MT5396
        if (u1VidPath == SV_VP_MAIN)
        {
            switch (i2Cur)
            {
            default:
            case VDP_ARG_MJC_DEMO_ALL:
                if (fgMjcClearLcdDemo == VDP_ARG_MJC_MODE_NO_DEBLUR)
                {
                    MJC_SetDemo(MJC_DEMO_OFF, MJC_INTP_HALF, MJC_INTP_HALF);
                }
                else
                {
                    MJC_SetDemo(MJC_DEMO_OFF, MJC_INTP_FULL, MJC_INTP_FULL);
                }
                break;
            case VDP_ARG_MJC_DEMO_RIGHT:
                if (fgMjcClearLcdDemo == VDP_ARG_MJC_MODE_NO_DEBLUR)
                {
                    MJC_SetDemo(MJC_DEMO_SIDEBYSIDE, MJC_INTP_OFF, MJC_INTP_HALF);
                }
                else
                {
                    MJC_SetDemo(MJC_DEMO_SIDEBYSIDE, MJC_INTP_OFF, MJC_INTP_FULL);
                }
                break;
            case VDP_ARG_MJC_DEMO_LEFT:
                if (fgMjcClearLcdDemo == VDP_ARG_MJC_MODE_NO_DEBLUR)
                {
                    MJC_SetDemo(MJC_DEMO_SIDEBYSIDE, MJC_INTP_HALF, MJC_INTP_OFF);
                }
                else
                {
                    MJC_SetDemo(MJC_DEMO_SIDEBYSIDE, MJC_INTP_FULL, MJC_INTP_OFF);                    
                }
                break;
            }
        }
        #endif
    #endif
}
    

void vUi_TDTV_PACKING(UINT8 u1VidPath, UINT8 i2Cur)
{
#ifdef CC_SCPOS_3DTV_SUPPORT
    if(u1VidPath==SV_VP_MAIN)
    {
        vDrvTDTVPacking((E_TDTV_UI_3D_PACKING)i2Cur);
    }
#endif    
}
 
void vUi_TDTV_LRINVERSE(UINT8 u1VidPath, UINT8 i2Cur)
{
#ifdef CC_SCPOS_3DTV_SUPPORT
    if(u1VidPath==SV_VP_MAIN)
    {
        vDrvTDTVLRInverse((E_TDTV_UI_LR)i2Cur);
    }
#endif    
}

void vUi_TDTV_DEPTH(UINT8 u1VidPath, UINT8 i2Cur)
{
#ifdef CC_SCPOS_3DTV_SUPPORT
    if(u1VidPath==SV_VP_MAIN)
    {
        vDrvTDTVDepth(i2Cur);
    }
#endif    
}

void vUi_TDTV_3D22D(UINT8 u1VidPath, UINT8 i2Cur)
{
#ifdef CC_SCPOS_3DTV_SUPPORT
    if(u1VidPath==SV_VP_MAIN)
    {
        vDrvTDTV3D22D((E_TDTV_UI_3D_2_2D)i2Cur);
    }
#endif    
}

void vUi_TDTV_FPR(UINT8 u1VidPath, UINT8 i2Cur)
{
#ifdef CC_SCPOS_3DTV_SUPPORT
    if(u1VidPath==SV_VP_MAIN)
    {
        vDrvTDTV3DFPR((E_TDTV_UI_3D_FPR)i2Cur);
    }
#endif    
}

void vUi_TDTV_Protrude(UINT8 u1VidPath, UINT8 i2Cur)
{
#ifdef CC_SCPOS_3DTV_SUPPORT
    if(u1VidPath==SV_VP_MAIN)
    {
        vDrvTDTVProtrude(i2Cur);
    }
#endif    
}

void vUi_TDTV_DISTANCE(UINT8 u1VidPath, UINT8 i2Cur)
{
#ifdef CC_SCPOS_3DTV_SUPPORT
    if(u1VidPath==SV_VP_MAIN)
    {
        vDrvTDTVDistanceFromTV(i2Cur);
    }
#endif    
}

void vUi_TDTV_OSD_DEPTH(UINT8 u1VidPath, UINT8 i2Cur)
{
#ifdef CC_SCPOS_3DTV_SUPPORT
    if(u1VidPath==SV_VP_MAIN)
    {
        vDrvTDTVOSDDepth(i2Cur);
    }
#endif    
}



//Begin Add for Vestel --- pingan 0606
void vUi_TVD_COMB(UINT8 u1VidPath, UINT8 i2Cur)
{
    vDrvTvdParam();
    vDrvTdcParam();
}

//Caihua Add 20110901
void vUi_SMARTPIC_UIDFT(UINT8 u1VidPath, UINT8 i2Cur)
{
    vDrvSetSmartPictureUiDft();
    vDrvSetTdtvSmartPictureUiDft();
}

//Caihua Add 20110916
void vUi_SIX_COLOR(UINT8 u1VidPath, UINT8 i2Cur)
{
    vDrvSetSixColorRegFromQtyTable();
}

//Caihua Add 20120326 All below
void vUi_TDTV_SIX_COLOR(UINT8 u1VidPath, UINT8 i2Cur)
{
    vDrvSetTdtvSixColorRegFromQtyTable();
}

void vUi_TDTV_SCE(UINT8 u1VidPath, UINT8 i2Cur)
{
    LOG(5, "Set 3D SCE Level %d \n", i2Cur);
    
    vApiFlashPqSetSceId(i2Cur);     
    bApiFlashPqUpdateSCE();   
    vHalVideoSCEOnOff(u1VidPath);
    vDrvSCEParam();
    vDrvSetSCELevel((UINT8)wReadQualityTable(QUALITY_COLOR_3D_SCE_L), 
                (UINT8)wReadQualityTable(QUALITY_COLOR_3D_SCE_S), 
                (UINT8)wReadQualityTable(QUALITY_COLOR_3D_SCE_H));
    vDrvSetTdtvSceRegFromQtyTable();
}
//End Add for Vestel --- pingan 0606


/**
 * @brief Interface between MW_if and NPTV Hal
 * @param u1VidPath SV_VP_MAIN/SV_VP_PIP
 * @param u2QtyItem PE_ARG_xxx
 */
UINT8 bApiVideoProc(UINT8 u1VidPath, PE_ARG_TYPE u1UiItem)
{
    PE_UI_RANGE_T UiRange;
    INT16 i2Min, i2Max, i2Dft, i2Cur;           // current
    UINT8 u1SrcTypTmg;
    UINT16 u2QtyItem;
    UINT8 bTurnOff;
    UINT8 bIsMax;
    PE_UI_RANGE_T QtyRange;
    i2Cur = 0;
    
    if(u1UiItem < PE_ARG_NS)
    {
        QtyRange = aUiQtyItemMinMax[u1UiItem];  
    }
    else
    {
        LOG(0, "API_PEUI Error: bApiVideoProc u1UiItem < PE_ARG_NS");
        return SV_FALSE;
    }

    // Get Timing
    u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(u1VidPath);
    LOG(1, "bDrvVideoGetSourceTypeTiming %d %d\n", u1SrcTypTmg, SOURCE_TYPE_TIMING_MAX); 

    // Bypass while TV Channal Auto Searching
    // During auto search, do not check no signal status
    if ((bGetSignalType(u1VidPath) == SV_ST_TV) && (_fgAutoSearch == TRUE))
    {
        if((QtyRange.i4Dft & R_SCAN) == 0)
        {
            LOG(6, "Search Chaneel, Do Not Update PQ Setting.\n");
            return SV_FALSE;
        }
    }
    else if (u1SrcTypTmg == SOURCE_TYPE_TIMING_MAX)
    {
        // Bypass while input path has no signal
        // But only backlight relative functions can work.  
        if((QtyRange.i4Dft & R_NOSIG) == 0)
        {
            LOG(6, "API_PEUI Error: bApiVideoProc No-Signal\n");
            return SV_TRUE;
        }
    }
    
    // Find a match SourceTypeTiming in CustomQtyIdx[].    
    u1SrcTypTmg = bApiGetCustomSourceTypeTiming(u1SrcTypTmg);    
    LOG(1, "bDrvVideoGetSourceTypeTiming(Path:%d)=Timing:0x%X\n",  //temp modify
        u1VidPath, u1SrcTypTmg);

    // Bypass after CLI "nptv.fw"   
    if ((_u4BypassNptvMloop & BYPASS_COLOR_PROC) != 0)
    {
        return SV_FALSE;
    }

    // Bypass while u1UiItem doesn't support PIP        
    if(QtyRange.i4Dft & R_MAIN)
    {
        if (u1VidPath != SV_VP_MAIN)
        {
            return SV_FALSE;
        }
    }
    
    // Get UI Setting
    PE_GetArg(u1VidPath, u1UiItem, &i2Cur);    
    
    if (PE_GetUiRange(u1VidPath, u1UiItem, &UiRange) == SV_FAIL)
    {
        LOG(0, "PE_GetUiRange %d FAIL\n", u1UiItem);
        return SV_FALSE;
    }
    i2Min = UiRange.i4Min;
    i2Max = UiRange.i4Max;
    i2Dft = UiRange.i4Dft;
    bTurnOff = (i2Cur == 0);
    bIsMax = 0;
    LOG(1, "\nPE_UI:  %s, Path: %d Item:%d Min:%d Max:%d Dft:%d Cur:%d\n",      //add log
        ucPeUiType[u1UiItem],u1VidPath, u1UiItem, i2Min, i2Max, i2Dft, i2Cur);

    if ((bApiGetPeUiDebugMode() == SV_ON) && (u1UiItem <= PE_ARG_NS))
    {
        LOG(0, "PE UI: %s, Path(%d), Min(%d), Max(%d), Dft(%d), Cur(%d)\n", 
                ucPeUiType[u1UiItem], u1VidPath, i2Min, i2Max, i2Dft, i2Cur);
    }

    // UI Remapping, 0(off), 1(min)
    // workaround for AP's UI limitation
    if(QtyRange.i4Dft & R_OFF)
    {
        i2Min = 1;
    }

    // UI Remapping, 
    // workaround for AP's UI limitation
    if(QtyRange.i4Dft & R_MAX)
    {
        if (i2Cur == i2Max)
        {   
            i2Cur = i2Max/2;
            bIsMax = 1;
        }
        i2Max = i2Max - 1;
    }

    // Fill Quality Table
    if(QtyRange.i4Dft & R_QTYMAP)
    {
        for (u2QtyItem=QtyRange.i4Min; u2QtyItem <= QtyRange.i4Max; u2QtyItem++)
        {
            UINT16 u2Value = bApiQtyMapping(u1SrcTypTmg, u2QtyItem, i2Min, i2Max, i2Dft,
                ((QtyRange.i4Dft & (R_OFF)) == (R_OFF)) ? MIN(MAX(i2Cur, i2Min), i2Max) : i2Cur);
            wWriteQualityTable(u2QtyItem, u2Value);
            LOG(2, "===value =[%d] item=[%d]  \n",u2Value,u2QtyItem);
        }
    }
    
    if(QtyRange.i4Dft & R_UIMAP)
    {
        // Special Case: Only for RGB Offset
        // u2Value is 2's complement
        for (u2QtyItem=QtyRange.i4Min; u2QtyItem <= QtyRange.i4Max; u2QtyItem++)
        {
            UINT16 u2Value = ((UINT16) i2Cur - (UINT16) ((i2Min + i2Max + 1) / 2));
            wWriteQualityTable(u2QtyItem, u2Value);             
        }
    }

    switch (u1UiItem)
    {
        case PE_ARG_BRIGHTNESS:
            vUi_BRIGHTNESS(u1VidPath, bTurnOff);        
            break;        
        case PE_ARG_CONTRAST:
            vUi_CONTRAST(u1VidPath, bTurnOff);              
            break;
        case PE_ARG_HUE:
            vUi_HUE(u1VidPath, bTurnOff);           
            break;        
        case PE_ARG_SATURATION:   
            vUi_SATURATION(u1VidPath, u1SrcTypTmg);         
            break;
        case PE_ARG_CTI:
            vUi_CTI(u1VidPath, bTurnOff);
            break;        
        case PE_ARG_SHARPNESS:
            vUi_SHARPNESS(u1VidPath, bTurnOff);         
            break;        
        case PE_ARG_SHARPNESS_H:
            vUi_SHARPNESS_HORIZONTAL(u1VidPath, bTurnOff);          
            break;
        case PE_ARG_SHARPNESS_V:
            vUi_SHARPNESS_VERTICAL(u1VidPath, bTurnOff);        
            break;      
        case PE_ARG_LTI:
            //vUi_LTI(u1VidPath, bTurnOff);     // This function is applied by above function: PE_ARG_SHARPNESS
            break;
        case PE_ARG_R_GAIN:
            vUi_R_GAIN(u1VidPath, bTurnOff);        
            break;        
        case PE_ARG_G_GAIN:
            vUi_G_GAIN(u1VidPath, bTurnOff);            
            break;        
        case PE_ARG_B_GAIN: 
            vUi_B_GAIN(u1VidPath, bTurnOff);            
            break;        
        case PE_ARG_R_OFFSET:
            vUi_R_OFFSET(u1VidPath, bTurnOff);          
            break;        
        case PE_ARG_G_OFFSET:
            vUi_G_OFFSET(u1VidPath, bTurnOff);          
            break;
        case PE_ARG_B_OFFSET:
            vUi_B_OFFSET(u1VidPath, bTurnOff);          
            break;  
        case PE_ARG_NR:
            bIsMax = (u1SrcTypTmg == 0);    //for vestel (enable 2D AUTO NR just in RF)
            vUi_NR(u1VidPath, i2Cur, bIsMax);
            break;
        case PE_ARG_GAMMA:
            vUi_GAMMA(u1VidPath, i2Cur);
            break;
        case PE_ARG_WHITE_PEAK_LMT:
            vUi_WHITE_PEAK_LMT(u1VidPath, bTurnOff);        
            break;
        case PE_ARG_SCE:
            vUi_SCE(u1VidPath, i2Cur);    
            break;
        case PE_ARG_SKIN_COLOR:
            vUi_SKIN_COLOR(u1VidPath, i2Cur);
            break;              
        case PE_ARG_ADAPTIVE_LUMA:
            vUi_ADAPTIVE_LUMA(u1VidPath, bTurnOff);    
            break;
        case PE_ARG_BACK_LIGHT_LVL:
            vUi_BACK_LIGHT_LVL(u1VidPath, bTurnOff);        
            break;
        case PE_ARG_ADAPTIVE_BACK_LIGHT:
            vUi_ADAPTIVE_BACK_LIGHT(u1VidPath, i2Cur);          
            break;                
        case PE_ARG_3D_NR:
            vUi_3D_NR(u1VidPath, i2Cur, bIsMax);
            break;
       case PE_ARG_DEMO:
            vUi_DEMO(u1VidPath, i2Cur);         
            break;
        case PE_ARG_DI_FILM_MODE:
            vUi_DI_FILM_MODE(u1VidPath, bTurnOff, i2Cur);       
            break;
        case PE_ARG_DI_MA:
            vUi_DI_MA(u1VidPath,i2Cur);         
            break;
        case PE_ARG_DI_EDGE_PRESERVE:
            vUi_DI_EDGE_PRESERVE(u1VidPath,i2Cur);          
            break;
        case PE_ARG_MJC_EFFECT:
            vUi_MJC_EFFECT(u1VidPath, i2Cur);       
            break;
        case PE_ARG_MJC_MODE:
            vUi_MJC_MODE(u1VidPath, i2Cur);  
            break;
        case PE_ARG_MJC_DEMO:
            vUi_MJC_DEMO(u1VidPath, i2Cur);         
            break;     
        case PE_ARG_XVYCC:
            vUi_xvYCC(i2Cur);       
            break;      
        case PE_ARG_BLUE_STRETCH:
            vUi_BLUE_STRETCH(u1VidPath, bTurnOff);      
            break;
        case PE_ARG_MPEG_NR:
            vUi_MPEG_NR(u1VidPath, i2Cur);      
            break;
        case PE_ARG_BLACK_STRETCH:
            vUi_BLACK_STRETCH(u1VidPath, bTurnOff);         
            break;
        case PE_ARG_WHITE_STRETCH:
            vUi_WHITE_STRETCH(u1VidPath, bTurnOff);         
            break;        
        case PE_ARG_SHARPNESS_ON_OFF:
            vUi_SHARPNESS_ON_OFF(u1VidPath, bTurnOff);      
            break;
        case PE_ARG_SUPERRESOLUTION_ON_OFF:
            vUi_SUPERRESOLUTION_ON_OFF(u1VidPath,  bTurnOff);       
            LOG(1, "vUi_SUPERRESOLUTION_ON_OFF=%d\n",!bTurnOff);
            break;
        case PE_ARG_CBE:
            vUi_CBE(u1VidPath,bTurnOff);
            break;
        case PE_ARG_USPEI:
            vUi_USPEI(u1VidPath, bTurnOff);
            break;
        case PE_ARG_LCDIM:
            vUI_LCDIM(u1VidPath, i2Cur);
            break;        
        case PE_ARG_TDTV_PACKING:
            vUi_TDTV_PACKING(u1VidPath,i2Cur);
            break;    
        case PE_ARG_TDTV_INVERSE:
            vUi_TDTV_LRINVERSE(u1VidPath,i2Cur);
            break;    
        case PE_ARG_TDTV_DEPTH:
            vUi_TDTV_DEPTH(u1VidPath, i2Cur);
            break;
        case PE_ARG_TDTV_3D_TO_2D:
            vUi_TDTV_3D22D(u1VidPath, i2Cur);
            break;        
        case PE_ARG_TDTV_FPR:
            vUi_TDTV_FPR(u1VidPath, i2Cur);
            break;
        case PE_ARG_TDTV_PROTRUDE:
            vUi_TDTV_Protrude(u1VidPath, i2Cur);
            break;
        case PE_ARG_TDTV_DISTANCE:
            vUi_TDTV_DISTANCE(u1VidPath, i2Cur);
            break;        
        case PE_ARG_TDTV_OSD_DEPTH:
            vUi_TDTV_OSD_DEPTH(u1VidPath, i2Cur);
            break;

        //Begin Add for Vestel --- pingan 0606
        case PE_ARG_TVD_COMB:
            vUi_TVD_COMB(u1VidPath, i2Cur);
            break;
        //Caihua Add 20110901
        case PE_ARG_PIC_UI_DFT:
            vUi_SMARTPIC_UIDFT(u1VidPath, i2Cur);
            break;     
        //Caihua Add 20110916    
        case PE_ARG_SIX_COLOR:
            vUi_SIX_COLOR(u1VidPath, i2Cur);
            break;
        //Caihua Add 20120326
        case PE_ARG_TDTV_SIX_COLOR:
            vUi_TDTV_SIX_COLOR(u1VidPath, i2Cur);
            break;
        case PE_ARG_TDTV_SCE:
            vUi_TDTV_SCE(u1VidPath, i2Cur);  
            break;
        case PE_ARG_TDTV_BRIGHTNESS:   
            vUi_TDTV_BRIGHTNESS(u1VidPath, bTurnOff);           
            break;
        case PE_ARG_TDTV_CONTRAST:   
            vUi_TDTV_CONTRAST(u1VidPath, bTurnOff);          
            break;
        case PE_ARG_TDTV_HUE:   
            vUi_TDTV_HUE(u1VidPath, bTurnOff);         
            break;
        case PE_ARG_TDTV_SATURATION:   
            vUi_TDTV_SATURATION(u1VidPath, u1SrcTypTmg);          
            break;
        case PE_ARG_TDTV_R_GAIN:   
            vUi_TDTV_R_GAIN(u1VidPath, bTurnOff);         
            break;
        case PE_ARG_TDTV_G_GAIN:   
            vUi_TDTV_G_GAIN(u1VidPath, bTurnOff);           
            break;
        case PE_ARG_TDTV_B_GAIN:   
            vUi_TDTV_B_GAIN(u1VidPath, bTurnOff);           
            break;
        case PE_ARG_TDTV_SKIN:   
            vUi_TDTV_SKIN_COLOR(u1VidPath, i2Cur);          
            break;
        //End Add for Vestel --- pingan 0606
        default:
            LOG(5, "Quality Item (%d) Not Support Yet\n", u1UiItem);        
            return SV_FALSE;
    }

    return SV_TRUE;
}
