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

#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif

#ifdef CC_MT5395
#include "drv_mjc_if.h"
#endif

// === DEFINE =============================================================================
#define R_DIS	  	0x1		// Special case, not default mapping
#define R_QTYMAP	0x2		// Use quality_table
#define R_UIMAP	 	0x4 	// UI Value = HW
#define R_MAIN 		0x8		// Main only
#define R_OFF	 	0x10	// 0 as off, 1 as min
#define R_MAX	 	0x20	// Re-map max as max-1
#define R_NOSIG	 	0x40 	// Also work in no singal
#define R_SCAN	 	0x80 	// Also work in TV scan
#define R_AUTONR      0x100      //TNR : Auto, High, Mid, Low, Off

// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================

// === GLOBAL VARIABLE ====================================================================

// === EXTERN VARIABLE ====================================================================

// === STATIC VARIABLE ====================================================================
static INT16 ai2Arg[VDP_NS][PE_ARG_NS];   // video quality item current value (UI)

static PE_UI_RANGE_T aUiVqMinMaxDft[EVDP_VIS_MAX][PE_ARG_NS];

static PE_UI_RANGE_T aUiVqMinMaxDftDefault[PE_ARG_NS] =
{
    // min   max   dft
    {   0,  100,    50},	// PE_ARG_BRIGHTNESS
    {   0,  100,    50},	// PE_ARG_CONTRAST
    { -50,  50,      0},	// PE_ARG_HUE
    {   0,  100,    50},	// PE_ARG_SATURATION
    {   0,    1,     1},    	// PE_ARG_CTI
    {   0,   20,    10},	// PE_ARG_SHARPNESS
    {   0,   20,    10},	// PE_ARG_SHARPNESS_H
    {	0,   20,    10},	// PE_ARG_SHARPNESS_L
    {   0,   20,    10},	// PE_ARG_LTI
    {   0,  255,   128},	// PE_ARG_R_GAIN
    {   0,  255,   128},	// PE_ARG_G_GAIN
    {   0,  255,   128},	// PE_ARG_B_GAIN    
    {   0,  255,   128},	// PE_ARG_R_OFFSET
    {   0,  255,   128},	// PE_ARG_G_OFFSET
    {   0,  255,   128},	// PE_ARG_B_OFFSET    
    {   0,    3,     2},	    // PE_ARG_NR
    {   0,    3,     2},	    // PE_ARG_GAMMA
    {   0,    1,     1},	    // PE_ARG_WHITE_PEAK_LMT
    {   0,    3,     2},	    // PE_ARG_SCE
    {   0,    3,     2},	    // PE_ARG_SKIN
    {   0,    3,     2},	    // PE_ARG_ADAPTIVE_LUMA
    {   0,   100,   50},	// PE_ARG_BACK_LIGHT_LVL
    {   0,    2,     0},	// PE_ARG_ADAPTIVE_BACK_LIGHT
    {   0,    4,     2},	// PE_ARG_3D_NR
    {   0,    3,     0},	// PE_ARG_DEMO
    {   0,    1,     1},	// PE_ARG_DI_FILM_MODE
    {   0,    1,     0},	// PE_ARG_DI_MA,
    {   0,    1,     0},	// PE_ARG_DI_EDGE_PRESERVE,
    {   0,    1,     0},	// PE_ARG_MJC_MODE,
    {   0,    3,     1},	// PE_ARG_MJC_EFFECT,
    {   0,    1,     0},	// PE_ARG_MJC_DEMO,
    {   0,    1,     0},	// PE_ARG_XVYCC,
    {   0,    1,     0},	// PE_ARG_WCG,
    {   0,    1,     0},	// PE_ARG_GAME_MODE,
    {   0,    1,     0},	// PE_ARG_BLUE_STRETCH,
    {   0,    3,     2},	// PE_ARG_MPEG_NR,
    {   0,    3,     2},	// PE_ARG_BLACK_STRETCH,
    {   0,    3,     2},	// PE_ARG_WHITE_STRETCH,
    {   0,    1,     0},	// PE_ARG_SHARPNESS_ON_OFF,     
    {   0,    1,     0},	// PE_ARG_OPC_CURVE,     
    {   0,    0,     0},    // PE_ARG_TDES,
    {   0,    3,     2},	// PE_ARG_LCIM
    {   0,    7,     1},    // PE_ARG_TDTV_PACKING,
    {   0,    1,     0},    // PE_ARG_TDTV_INVERSE,    
    {   0,    32,   16},    // PE_ARG_TDTV_DEPTH,
    {   0,    1,     0},    // PE_ARG_TDTV_3D_TO_2D
    {   0,    2,     0},    // PE_ARG_TDTV_FPR
    {   0,   32,    16},    // PE_ARG_TDTV_PROTRUDE,
    {   2,   18,    10},    // PE_ARG_TDTV_DISTANCE
    {   0,   32,    16}     // PE_ARG_TDTV_OSD_DEPTH
}; 
  
static PE_UI_RANGE_T aUiQtyItemMinMax[PE_ARG_NS] =
{
	// BEGIN   				END   					 	TYPE
	{QUALITY_BRIGHTNESS,	QUALITY_BRIGHTNESS,    		R_QTYMAP | R_SCAN},				// PE_ARG_BRIGHTNESS
	{QUALITY_CONTRAST,  	QUALITY_CONTRAST,    		R_QTYMAP | R_SCAN},				// PE_ARG_CONTRAST
	{QUALITY_HUE,  			QUALITY_HUE,      			R_QTYMAP | R_SCAN},				// PE_ARG_HUE
	{QUALITY_SATURATION,  	QUALITY_SATURATION,  		R_QTYMAP | R_SCAN},				// PE_ARG_SATURATION
	{QUALITY_CTI_BEGIN,    	QUALITY_CTI_END,     		R_QTYMAP | R_MAIN | R_OFF},		// PE_ARG_CTI
	{QUALITY_SHARP_BEGIN,   QUALITY_SHARP_END,    		R_QTYMAP | R_MAIN},				// PE_ARG_SHARPNESS
	{QUALITY_SHARP_H_BEGIN, QUALITY_SHARP_H_END,   		R_QTYMAP | R_MAIN},				// PE_ARG_SHARPNESS_H
	{QUALITY_SHARP_V_BEGIN, QUALITY_SHARP_V_END,   		R_QTYMAP | R_MAIN},				// PE_ARG_SHARPNESS_L
	{QUALITY_LTI_BEGIN,   	QUALITY_LTI_END,    		R_QTYMAP | R_MAIN | R_OFF},		// PE_ARG_LTI
	{QUALITY_R_GAIN,  		QUALITY_R_GAIN,   			R_QTYMAP | R_NOSIG | R_SCAN},   // PE_ARG_R_GAIN
	{QUALITY_G_GAIN,  		QUALITY_G_GAIN,   			R_QTYMAP | R_NOSIG | R_SCAN},   // PE_ARG_G_GAIN
	{QUALITY_B_GAIN,  		QUALITY_B_GAIN,  			R_QTYMAP | R_NOSIG | R_SCAN},   // PE_ARG_B_GAIN    
	{QUALITY_R_OFFSET,  	QUALITY_R_OFFSET,  			R_UIMAP  | R_NOSIG | R_SCAN},   // PE_ARG_R_OFFSET
	{QUALITY_G_OFFSET,  	QUALITY_G_OFFSET,  			R_UIMAP  | R_NOSIG | R_SCAN},   // PE_ARG_G_OFFSET
	{QUALITY_B_OFFSET,  	QUALITY_B_OFFSET,  		 	R_UIMAP  | R_NOSIG | R_SCAN},   // PE_ARG_B_OFFSET    
	{QUALITY_SNR_BEGIN,     QUALITY_SNR_END,     		R_QTYMAP | R_MAIN  | R_OFF},   	// PE_ARG_NR
	{QUALITY_GAMMA_BEGIN, 	QUALITY_GAMMA_END,			R_QTYMAP | R_MAIN | R_NOSIG},	// PE_ARG_GAMMA
	{0,    					0,     						R_DIS},							// PE_ARG_WHITE_PEAK_LMT
	{QUALITY_SCE_BEGIN, 	QUALITY_SCE_END,     		R_QTYMAP | R_MAIN},				// PE_ARG_SCE
	{QUALITY_SKIN_BEGIN, 	QUALITY_SKIN_END,     		R_QTYMAP | R_MAIN},				// PE_ARG_SKIN
	{QUALITY_AL_BEGIN, 		QUALITY_AL_END, 			R_QTYMAP | R_MAIN | R_OFF},		// PE_ARG_ADAPTIVE_LUMA
	{QUALITY_BACKLIGHT,   	QUALITY_BACKLIGHT,   		R_QTYMAP | R_MAIN | R_NOSIG | R_SCAN},	// PE_ARG_BACK_LIGHT_LVL
	{0,    					0,     						R_DIS    | R_MAIN | R_NOSIG},	// PE_ARG_ADAPTIVE_BACK_LIGHT
	{QUALITY_TNR_BEGIN,    QUALITY_TNR_END,     		R_QTYMAP | R_OFF | R_MAX | R_AUTONR},     	// PE_ARG_3D_NR
	{0,    					0,     						R_DIS    | R_MAIN},				// PE_ARG_DEMO
	{0,    					0,     						R_DIS},							// PE_ARG_DI_FILM_MODE
	{0,    					0,     						R_DIS},							// PE_ARG_DI_MA,
	{0,    					0,     						R_DIS},							// PE_ARG_DI_EDGE_PRESERVE,
	{0,    					0,     						R_DIS 	 | R_MAIN},				// PE_ARG_MJC_MODE,
	{0,    					0,     						R_DIS    | R_MAIN},				// PE_ARG_MJC_EFFECT,
	{0,    					0,     						R_DIS    | R_MAIN},				// PE_ARG_MJC_DEMO,
	{0,    					0,     						R_DIS    | R_MAIN},				// PE_ARG_XVYCC,
	{0,    					0,     						R_DIS    | R_MAIN},				// PE_ARG_WCG,
	{0,    					0,     						R_DIS    | R_MAIN},				// PE_ARG_GAME_MODE,
	{QUALITY_BS_BEGIN, 		QUALITY_BS_END, 			R_QTYMAP | R_MAIN},				// PE_ARG_BLUE_STRETCH,
	{QUALITY_MPEGNR_BEGIN,	QUALITY_MPEGNR_END,     	R_QTYMAP | R_OFF},				// PE_ARG_MPEG_NR,
	{QUALITY_AL_BS_BEGIN,	QUALITY_AL_BS_END, 		    R_QTYMAP | R_MAIN | R_OFF},		// PE_ARG_BLACK_STRETCH,
	{QUALITY_AL_WS_BEGIN,	QUALITY_AL_WS_END, 		    R_QTYMAP | R_MAIN | R_OFF},		// PE_ARG_WHITE_STRETCH,
	{0,    					0,     						R_DIS},							// PE_ARG_SHARPNESS_ON_OFF,     
	{0,    					0,     						R_DIS    | R_MAIN},				// PE_ARG_OPC_CURVE,  	
	{QUALITY_IRR_BEGIN,		QUALITY_IRR_END,			R_QTYMAP | R_MAIN},				// PE_ARG_IRR
	{QUALITY_CBE_BEGIN,		QUALITY_CBE_END,			R_QTYMAP | R_MAIN},				// PE_ARG_CBE
	{0,                     0,                          R_QTYMAP},						// PE_ARG_TDES,
    {0,                     0,                          R_MAIN},                        // PE_ARG_LCDIM,
	{0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},	// PE_ARG_TDTV_PACKING,
	{0,		                0,			                R_DIS    | R_MAIN | R_NOSIG},	// PE_ARG_TDTV_INVERSE, 
	{0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},	// PE_ARG_TDTV_DEPTH,
	{0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},	// PE_ARG_TDTV_3D_TO_2D	
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_TDTV_FPR 
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_TDTV_PROTRUDE,
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_TDTV_DISTANCE 
    {0,                     0,                          R_DIS    | R_MAIN | R_NOSIG},   // PE_ARG_TDTV_OSD_DEPTH     
};

static const UCHAR *ucPeUiType[PE_ARG_NS] = 
{
    "BRIGHTNESS",				// PE_ARG_BRIGHTNESS=0,         
    "CONTRAST",                 // PE_ARG_CONTRAST,             
    "HUE",                      // PE_ARG_HUE,                  
    "SATURATION",               // PE_ARG_SATURATION,           
    "CTI",                      // PE_ARG_CTI,                  
    "SHARPNESS",                // PE_ARG_SHARPNESS,            
    "SHARPNESS_H",              // PE_ARG_SHARPNESS_H,          
    "SHARPNESS_V",              // PE_ARG_SHARPNESS_L,          
    "LTI",                      // PE_ARG_LTI,                  
    "R_GAIN",                   // PE_ARG_R_GAIN,               
    "G_GAIN",                   // PE_ARG_G_GAIN,               
    "B_GAIN",                   // PE_ARG_B_GAIN,               
    "R_OFFSET",                 // PE_ARG_R_OFFSET,             
    "G_OFFSET",                 // PE_ARG_G_OFFSET,             
    "B_OFFSET",                 // PE_ARG_B_OFFSET,             
    "NR",                       // PE_ARG_NR,                   
    "GAMMA",                    // PE_ARG_GAMMA,                
    "WHITE_PEAK_LMT",           // PE_ARG_WHITE_PEAK_LMT,       
    "SCE", 						// PE_ARG_SCE,                
    "SKIN",                     	// PE_ARG_SKIN_COLOR,           
    "ADAPTIVE_LUMA",            // PE_ARG_ADAPTIVE_LUMA,        
    "BACK_LIGHT_LVL",           // PE_ARG_BACK_LIGHT_LVL,       
    "ADAPTIVE_BACK_LIGHT",      // PE_ARG_ADAPTIVE_BACK_LIGHT,  
    "3D_NR",                    // PE_ARG_3D_NR,                
    "DEMO",                     // PE_ARG_DEMO,                 
    "DI_FILM_MODE",             // PE_ARG_DI_FILM_MODE,         
    "DI_MA",                    // PE_ARG_DI_MA,                
    "DI_EDGE_PRESERVE",         // PE_ARG_DI_EDGE_PRESERVE,     
    "MJC_MODE",                 // PE_ARG_MJC_MODE,             
    "MJC_EFFECT",               // PE_ARG_MJC_EFFECT,           
    "MJC_DEMO",                 // PE_ARG_MJC_DEMO,             
    "XVYCC",                    // PE_ARG_XVYCC,                
    "WCG",                      // PE_ARG_WCG,                  
    "GAME_MODE",                // PE_ARG_GAME_MODE,            
    "BLUE_STRETCH",             // PE_ARG_BLUE_STRETCH,         
    "MPEG_NR",                  // PE_ARG_MPEG_NR,              
    "BLACK_STRETCH",            // PE_ARG_BLACK_STRETCH,        
    "WHITE_STRETCH",            // PE_ARG_WHITE_STRETCH,        
    "SHARPNESS_ON_OFF",         // PE_ARG_SHARPNESS_ON_OFF,     
    "OPC_CURVE",                // PE_ARG_OPC_CURVE,                          
    "TDES",                     // PE_ARG_TDES,                 
    "LCDIM",                    // PE_ARG_LCDIM,	            
    "TDTV_PACKING",             // PE_ARG_TDTV_PACKING,          
    "TDTV_INVERSE",             // PE_ARG_TDTV_INVERSE,         
    "TDTV_DEPTH",               // PE_ARG_TDTV_DEPTH,           
    "TDTV_2D_TO_3D",			// PE_ARG_TDTV_3D_TO_2D,   
    "TDTV_FPR",	                // PE_ARG_TDTV_FPR, 
    "PE_ARG_TDTV_PROTRUDE",     // PE_ARG_TDTV_PROTRUDE 
    "PE_ARG_TDTV_DISTANCE",     // PE_ARG_TDTV_DISTANCE
    "PE_ARG_TDTV_OSD_DEPTH",    // PE_ARG_TDTV_OSD_DEPTH    
};

UINT8 bSrcTimingInverseTbl[SOURCE_TYPE_TIMING_MAX+1];

#ifdef DRV_SUPPORT_EXTMJC
static UINT8 bMjcUiEffect;
static UINT8 bMjcUiMode;
static UINT8 bMjcUiDemo;
//static UINT8 bMjcUiDemoMode;
#endif
#ifdef CC_MT5395
#ifndef DRV_SUPPORT_EXTMJC
   static UINT8 fgMjcClearLcdDemo;
#endif
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
		case QUALITY_CONTRAST:
			wValue += ((UINT16)QUALITY_TBL[QUALITY_CONTRAST_MSB]) << 8;	
		break;
		
		case QUALITY_SATURATION:
			wValue += ((UINT16)QUALITY_TBL[QUALITY_SATURATION_MSB]) << 8;	
		break;

		case QUALITY_R_GAIN:
			wValue += ((UINT16)QUALITY_TBL[QUALITY_R_GAIN_MSB]) << 8;	
		break;
		
		case QUALITY_G_GAIN:
			wValue += ((UINT16)QUALITY_TBL[QUALITY_G_GAIN_MSB]) << 8;	
		break;
		
		case QUALITY_B_GAIN:
			wValue += ((UINT16)QUALITY_TBL[QUALITY_B_GAIN_MSB]) << 8;	
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
		case QUALITY_CONTRAST:
			QUALITY_TBL[QUALITY_CONTRAST_MSB] = MSB;	
		break;
		
		case QUALITY_SATURATION:
			QUALITY_TBL[QUALITY_SATURATION_MSB] = MSB;	
		break;

		case QUALITY_R_GAIN:
			QUALITY_TBL[QUALITY_R_GAIN_MSB] = MSB;
		break;
		
		case QUALITY_G_GAIN:
			QUALITY_TBL[QUALITY_G_GAIN_MSB] = MSB;
		break;
		
		case QUALITY_B_GAIN:
			QUALITY_TBL[QUALITY_B_GAIN_MSB] = MSB;
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
	}	
}

/**
 * @brief Mapping the bCustomSrcTimingTbl[]  and 
 *	bCustomSourceGroup[] table.
 *
 */
static void _vMappingSrcTiming(void)
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
		    ASSERT(bSrcTimingInverseTbl[j] == 0xFF); 
			bSrcTimingInverseTbl[j] = i;
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
UINT8 bApiParseCustomQtyItem()
{
    UINT16 DftIndex, CustIndex;

	_vMappingSrcTiming();

    for (DftIndex = 0; DftIndex < QUALITY_MAX; DftIndex++)
    {
        for (CustIndex = 0; CustIndex <  CUSTOM_QTY_TBL_DIM ; CustIndex++)
        {
            if (READ_CUST_QTY_ITEM(CustIndex) >= QUALITY_MAX)
            {
                // Can not find this item is CustomQtyItem table.
                // So this item will use default value in DefaultQtyTbl[].
                break;
            }

            else if (READ_CUST_QTY_ITEM(CustIndex) == DftIndex)
            {
                // This item is found in CustomQtyItem, mark its serial number.
                // CustomQtyTble for this item will be used, not use default value.
                WRITE_DEFUALT_QTY_TBL_REF(DftIndex, CustIndex);
                break;
            }
        }
    }
    return SV_SUCCESS;
}

/**
 * @brief Translate source type/timing index into customer defined index.
 * @param bSrcTypTmg SOURCE_TYPE_TIMING_RFTV ~ SOURCE_TYPE_TIMING_MAX
 *
 */
UINT8 bApiGetCustomSourceTypeTiming(UINT8 bSrcTypTmg)
{
    UINT8 i;
    UINT8 bCustomSrcTypTmg;

	if(bSrcTypTmg <= SOURCE_TYPE_TIMING_MAX)
	{
		bSrcTypTmg = bSrcTimingInverseTbl[bSrcTypTmg];
	}else
	{
		LOG(0, "API_PEUI Error: bApiGetCustomSourceTypeTiming bSrcTypTmg <= SOURCE_TYPE_TIMING_MAX");
		return 0;
	}
	
    //Search 2 Times, first time as original type , second time as parant type
    for (i = 2; i != 0; --i)
    {
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

        //Cast Children type to Parent type
        bSrcTypTmg = bSrcTypTmg & 0xF0;
    }

    // Can not find proper quality index.
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
		case QUALITY_CONTRAST:
			MSBQtyItem = QUALITY_CONTRAST_MSB;
			break;	
			
		case QUALITY_SATURATION:
	        MSBQtyItem = QUALITY_SATURATION_MSB;
	        break;
	        
		case QUALITY_R_GAIN:
			MSBQtyItem = QUALITY_R_GAIN_MSB;		
			break;
			
		case QUALITY_G_GAIN:
			MSBQtyItem = QUALITY_G_GAIN_MSB;		
			break;
			
		case QUALITY_B_GAIN:
			MSBQtyItem = QUALITY_B_GAIN_MSB;			
    	    break;
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
	
    // Some qty_item does not need mapping, return default HW value.
    // or UI_cur == UI_dft, return default HW value.
    if (((UiRange.i4Min == 0) && (UiRange.i4Max == 0)) || (i2Cur == i2Dft))
    {
        return UiRange.i4Dft;
    }

    // Some qty_item use 2-level setting as min/max/default, don't mapping.
    if (i2Max == 2)           // UI_max=2 means UI option is: off, low, high.
    {
        switch (i2Dft)
        {
        case 0:
            switch(i2Cur)            	
            {
           	case 0:
	    	    return UiRange.i4Dft;
	        case 1:
               	return UiRange.i4Max;
			case 2:
               	return UiRange.i4Max;
            }
		case 1:
            switch(i2Cur)            	
            {
            case 0:
	        	return UiRange.i4Min;
	        case 1:
              	return UiRange.i4Dft;
			case 2:
              	return UiRange.i4Max;
            }        
		case 2:
            switch(i2Cur)            	
            {
            case 0:
	        	return UiRange.i4Min;
	        case 1:
               	return UiRange.i4Min;
			case 2:
               	return UiRange.i4Dft;
            }
        default:
        	ASSERT(0);
        }
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

    LOG(5, "bApiQtyMapping: Value=%d, u1Min=%d u1Max=%d u1Dft=%d\n", ReturnVal, UiRange.i4Min, UiRange.i4Max, UiRange.i4Dft);
    
    return ReturnVal;
}

void PE_SetUiMinMaxDft(void)
{
    INT32 i4VisIdx;

    // Set all UI video quality item of input source's
    // min/max/default to default value.
    for (i4VisIdx = 0; i4VisIdx < (INT32)EVDP_VIS_MAX; i4VisIdx++)
    {
        x_memcpy(&aUiVqMinMaxDft[i4VisIdx],
                 aUiVqMinMaxDftDefault,
                 sizeof(aUiVqMinMaxDftDefault));
    } 

    // Set all UI video quality items' current to default value.
    PE_SetAllUiVqItemDftValues();
}

/**
 * u4ApiSetUiVqItemDftValue
 *
 * @param void
 * @return void
 */
UINT32 u4ApiSetUiVqItemDftValue(UCHAR u1VidPath, UCHAR ucArg, INT16 i2Value)
{

    if (ucArg >= (UCHAR) PE_ARG_NS)
    {
        return SV_FAIL;
    }

    ai2Arg[u1VidPath][ucArg] = i2Value;

    return SV_SUCCESS;
}
/**
 * u1ApiVidInSrcType2VisType
 *
 * @param void
 * @return void
 */
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
UINT32 PE_GetUiRange(UINT8 u1VidPath, PE_ARG_TYPE ucArgType,
    PE_UI_RANGE_T *prUiRange)
{
    INT32 i4Offset;
    UINT8 u1NptvSrcType, u1VdpVisType;
    PE_UI_RANGE_T *prVqTbl;

    if (ucArgType >= (UCHAR) PE_ARG_NS)
    {
        return SV_FAIL;
    }

    // get current video input source type
    u1NptvSrcType = bApiVideoGetSrcType(u1VidPath);
    u1VdpVisType = u1ApiVidInSrcType2VisType(u1NptvSrcType);

    if (u1VdpVisType >= (UINT8)EVDP_VIS_MAX)
    {
        u1VdpVisType = (UINT8)EVDP_VIS_TUNER_DIGITAL;
        prVqTbl = &aUiVqMinMaxDft[u1VdpVisType][ucArgType];
    }
    else
    {
        prVqTbl = &aUiVqMinMaxDft[u1VdpVisType][ucArgType];
    }

    i4Offset = ((prVqTbl->i4Min < 0) ? prVqTbl->i4Min : 0);

    prUiRange->i4Min = (INT16)(prVqTbl->i4Min - i4Offset);
    prUiRange->i4Max = (INT16)(prVqTbl->i4Max - i4Offset);
    prUiRange->i4Dft = (INT16)(prVqTbl->i4Dft - i4Offset);

    return SV_SUCCESS;
}
/*lint -restore */

/**
 * VDP set min/max/default values of UI video quality items.
 *
 * @param void
 * @return void
 */
/*lint -save -e613 -e661 -e662 -e818 -e830 -e831 */
void PE_SetUiRange(UINT8 u1VdpVisType, PE_ARG_TYPE ucArgType,
    PE_UI_RANGE_T *prUiRange)
{
    // check args
    VERIFY(u1VdpVisType < (UINT8)EVDP_VIS_MAX);
    VERIFY((UINT8)ucArgType < (UINT8)PE_ARG_NS);
    VERIFY(prUiRange != NULL);

    if ((u1VdpVisType < (UINT8)EVDP_VIS_MAX) &&
        ((UINT8)ucArgType < (UINT8)PE_ARG_NS) &&
        (prUiRange != NULL))
    {
        aUiVqMinMaxDft[u1VdpVisType][ucArgType].i4Min = prUiRange->i4Min;
        aUiVqMinMaxDft[u1VdpVisType][ucArgType].i4Max = prUiRange->i4Max;
        aUiVqMinMaxDft[u1VdpVisType][ucArgType].i4Dft = prUiRange->i4Dft;
    }
    else
    {
        Printf("aUiVqMinMaxDft range index error! %d,%d,%d\n",
            u1VdpVisType, ucArgType, prUiRange);
    }
}
/*lint -restore */

/**
 * VDP set min/max/default values of all UI video quality items.
 *
 * @param void
 * @return void
 */
void PE_SetAllUiVqItemDftValues(void)
{
    UCHAR ucVidPath;
    INT16 i2Default;
    INT32 i4UiVqItem, i4Offset, i4VdpVisType;
    PE_UI_RANGE_T *prVqTbl;

    // Use cvbs as default
    i4VdpVisType = (INT32)EVDP_VIS_AVC_COMP_VIDEO;

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

/**
 * VDP set video plane argument.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucArg specify the color item
 * @param i2Value specify the value of the color item
 * @return SV_FAIL or SV_SUCCESS
 */
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



/**
 * VDP query video plane argument.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucArg
 * @param pi2Value
 * @return SV_FAIL or SV_SUCCESS.
 */
//-----------------------------------------------------------------------------
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

void vUi_3D_NR(UINT8 u1VidPath, UINT8 i2Cur, UINT8 bAutoNROnOff)
{
        if (bDrvNRGetCurrentPath() != u1VidPath)
        {
            vDrvNRSetSystem();
            return;
        }

        if (i2Cur == 0) //TNR is off
        {
            vApiVideo3DNROnOff(SV_OFF);
#if ANM_ENABLE
            vDrvSetAutoNROnOff(SV_OFF);// Turn On Off ANR
#endif
        }
        else
        {
#if ANM_ENABLE
            vDrvSetAutoNROnOff(bAutoNROnOff);// Turn On Off ANR
            vDrvANRSetSWRegister();
#endif

            //vHalVideoNRTIIROnOff(SV_ON);
            vHalVideo3DNRParam();
            vApiVideo3DNROnOff(SV_ON);
        }

        vDrvNRSetSystem();
}

void vUi_NR(UINT8 u1VidPath, UINT8 bTurnOff)
{
    if(DRVCUST_OptGet(eFlagBind2DNRWithMpegNR))
    {
        return;
    }

    if (bDrvNRGetCurrentPath() != u1VidPath)
    {
        return;
    }

    vHalVideo2DNRParam(bTurnOff);
    vDrvNRSetSystem();
}

void vUi_MPEG_NR(UINT8 u1VidPath, UINT8 bTurnOff)
{
    if (bDrvNRGetCurrentPath() != u1VidPath)
    {
        return;
    }

    vHalVideoMPEGNRParam(bTurnOff);    // if i2Cur == 0, set blending = 0.
	LOG(0,"vUi_MPEG_NR\n");    
    vDrvNRSetSystem();
}

void vUi_SCE(UINT8 u1VidPath, UINT8 i2Cur)
{
    LOG(5, "Set SCE Level %d \n", i2Cur);
    
    vApiFlashPqSetSceId(i2Cur);   	
    bApiFlashPqUpdateSCE();   
    vDrvSetSCELevel((UINT8)wReadQualityTable(QUALITY_SCE_LUMA_GAIN), 
                    (UINT8)wReadQualityTable(QUALITY_SCE_SAT_GAIN), 
                    (UINT8)wReadQualityTable(QUALITY_SCE_HUE_GAIN));
    vHalVideoSCEOnOff(u1VidPath, SV_ON);
}

void vUi_SKIN_COLOR(UINT8 u1VidPath, UINT8 i2Cur)
{
    LOG(5, "Set Skin Level %d \n", i2Cur);
    
    vDrvSetSkinLevel((UINT8)wReadQualityTable(QUALITY_SKIN_LUMA_GAIN), 
                    (UINT8)wReadQualityTable(QUALITY_SKIN_SAT_GAIN), 
                    (UINT8)wReadQualityTable(QUALITY_SKIN_HUE_GAIN));
    vHalVideoSCEOnOff(u1VidPath, SV_ON);
}

void vUi_ADAPTIVE_LUMA(UINT8 u1VidPath, UINT8 bTurnOff)
{
    if(bTurnOff)
    {
        vApiRegisterVideoEvent(PE_EVENT_ADAPTIVE_LUMA, u1VidPath, SV_OFF);            
    }
    else
    {   
        vDrvADLUpdateParam();
        vApiRegisterVideoEvent(PE_EVENT_ADAPTIVE_LUMA, u1VidPath, SV_ON);
    }
}

void vUi_BLACK_STRETCH(UINT8 u1VidPath, UINT8 bTurnOff)
{
	wWriteQualityTable(QUALITY_AL_BWS_BLACK_ON_1_OFF_0, (UINT16)(!bTurnOff));
	vHalVideoAdaptiveLumaParam();
}

void vUi_WHITE_STRETCH(UINT8 u1VidPath, UINT8 bTurnOff)
{
	wWriteQualityTable(QUALITY_AL_BWS_WHITE_ON_1_OFF_0, (UINT16)(!bTurnOff));
	vHalVideoAdaptiveLumaParam();
}

void vUi_BLUE_STRETCH(UINT8 u1VidPath, UINT8 bTurnOff)
{
	vHalVideoBlueStretchSetDefault();
	vHalVideoBlueStretchOnOff(!bTurnOff);   //i2Cur==0 means turn off.
}

void vUi_ADAPTIVE_BACK_LIGHT(UINT8 u1VidPath, UINT8 i2Cur)
{
	//i2Cur==0 means turn off.
    vHalVideoAdaptiveBackLightParam(i2Cur); 
    vHalVideoSetAdaptiveBacklightMode(i2Cur);
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
        if (!GAMMA_IN_DRAM)
        {
        	vApiFlashPqSetGammaId(i2Cur);
            bApiFlashPqUpdateGamma();
        }
        
    	if(DRVCUST_OptGet(eNVMGamma))
    	{
    		vDrvSetGammaRemap(SV_ON);
    	}
    	else
    	{
    	    // i2Cur==0 means turn off Gamma
    		// vApiRegisterVideoEvent(PE_EVENT_GAMMA, u1VidPath, i2Cur);
            vHalVideoGammaLoadTable(i2Cur);
    	}
    }
}

void vUi_SHARPNESS_ON_OFF(UINT8 u1VidPath, UINT8 bTurnOff)
{
	vHalVideoSharpnessOnOff(u1VidPath, bTurnOff);
}

void vUi_TDES(UINT8 u1VidPath, UINT8 bTurnOff)
{
	vHalVideoTDESOnOff(u1VidPath, bTurnOff);
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
       vHalVideoIRRParam();   	
	vHalVideoCBEParam();	
}

void vUi_SHARPNESS_HORIZONTAL(UINT8 u1VidPath, UINT8 bTurnOff)
{
	vHalVideoSharpnessHorizontalOnOff(u1VidPath, SV_ON);
	vHalVideoSharpnessParamHorizontal(u1VidPath);
}

void vUi_SHARPNESS_VERTICAL(UINT8 u1VidPath, UINT8 bTurnOff)
{
	vHalVideoSharpnessVerticalOnOff(u1VidPath, SV_ON);
	vHalVideoSharpnessParamVertical(u1VidPath);
}

void vUi_BRIGHTNESS(UINT8 u1VidPath, UINT8 bTurnOff)
{
	vHalVideoBrightness(u1VidPath, wReadQualityTable(QUALITY_BRIGHTNESS));
}

void vUi_CONTRAST(UINT8 u1VidPath, UINT8 bTurnOff)
{
	vHalVideoContrast(u1VidPath, wReadQualityTable(QUALITY_CONTRAST) 
	                | (wReadQualityTable(QUALITY_CONTRAST_MSB) << 8));

	if(DRVCUST_OptGet(eFlagBindContrastSaturation))
	{
		bApiVideoProc(u1VidPath, PE_ARG_SATURATION);
	}
}

void vUi_HUE(UINT8 u1VidPath, UINT8 bTurnOff)
{
	vHalVideoHue(u1VidPath, wReadQualityTable(QUALITY_HUE));
}

void vUi_SATURATION(UINT8 u1VidPath, UINT8 u1SrcTypTmg)
{
    UINT32 u4Tmp;
    if(DRVCUST_OptGet(eFlagBindContrastSaturation))
    {
        UINT16 u2ContrastDft = bApiGetQtyDft(QUALITY_CONTRAST, u1SrcTypTmg) | ((UINT16)bApiGetQtyDft(QUALITY_CONTRAST_MSB, u1SrcTypTmg) << 8);
        UINT16 u2SatQty = wReadQualityTable(QUALITY_SATURATION) | ((UINT16)wReadQualityTable(QUALITY_SATURATION_MSB) << 8);
        UINT16 u2ContrastQty = wReadQualityTable(QUALITY_CONTRAST) | ((UINT16)wReadQualityTable(QUALITY_CONTRAST_MSB) << 8);

        if(u2ContrastDft)
        {
            u4Tmp = ((UINT32)u2SatQty * u2ContrastQty) / u2ContrastDft;
            wWriteQualityTable(QUALITY_SATURATION, u4Tmp & 0xFF);
            wWriteQualityTable(QUALITY_SATURATION_MSB, u4Tmp >> 8);
        }
    }
    vHalVideoSaturation(u1VidPath, wReadQualityTable(QUALITY_SATURATION) | (wReadQualityTable(QUALITY_SATURATION_MSB) << 8));
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

void vUi_BACK_LIGHT_LVL(UINT8 u1VidPath, UINT8 bTurnOff)
{
	DRVCUST_SetBacklightDuty(wReadQualityTable(QUALITY_BACKLIGHT));
}

void vUi_R_GAIN(UINT8 u1VidPath, UINT8 bTurnOff)
{
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_color_temperature(E_EXTMJC_CT_R_GAIN1, wReadQualityTable(QUALITY_R_GAIN));
    }
    else
#endif
    {
        vApiRegisterVideoEvent(PE_EVENT_R_GAIN, u1VidPath, SV_ON);
    }
}

void vUi_G_GAIN(UINT8 u1VidPath, UINT8 bTurnOff)
{
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_color_temperature(E_EXTMJC_CT_G_GAIN1, wReadQualityTable(QUALITY_G_GAIN));
    }
    else
#endif
    {
        vApiRegisterVideoEvent(PE_EVENT_G_GAIN, u1VidPath, SV_ON);
    }
}

void vUi_B_GAIN(UINT8 u1VidPath, UINT8 bTurnOff)
{
#ifdef DRV_SUPPORT_MT8283
    if (EXTMJC_SW_TO_FRONT == 0)
    {
        drv_extmjc_set_color_temperature(E_EXTMJC_CT_B_GAIN1, wReadQualityTable(QUALITY_B_GAIN));
    }
    else
#endif
    {
        vApiRegisterVideoEvent(PE_EVENT_B_GAIN, u1VidPath, SV_ON);
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
	    vApiRegisterVideoEvent(PE_EVENT_R_OFFSET, u1VidPath, SV_ON);
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
        vApiRegisterVideoEvent(PE_EVENT_G_OFFSET, u1VidPath, SV_ON);
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
        vApiRegisterVideoEvent(PE_EVENT_B_OFFSET, u1VidPath, SV_ON);
    }
}

void vUi_xvYCC(UINT8 i2Cur)
{
    vDrvxvYCCOnOff(i2Cur);
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
        vApiVideoDemoMode(i2Cur, rDummyRegion, 0xF0);
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
        #ifdef CC_MT5395
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
        #ifdef CC_MT5395
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
        #ifdef CC_MT5395
        if (u1VidPath == SV_VP_MAIN)
        {
            switch (i2Cur)
            {
            default:
            case VDP_ARG_MJC_DEMO_ALL:
                MJC_SetDemo(MJC_DEMO_OFF, NULL, NULL);
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




/**
 * @brief Interface between MW_if and NPTV Hal
 * @param u1VidPath SV_VP_MAIN/SV_VP_PIP
 * @param u2QtyItem PE_ARG_xxx
 */
UINT8 bApiVideoProc(UINT8 u1VidPath, PE_ARG_TYPE u1UiItem)
{
    PE_UI_RANGE_T UiRange;
    INT16 i2Min, i2Max, i2Dft, i2Cur;   		// current
    UINT8 u1SrcTypTmg;
    UINT16 u2QtyItem;
    UINT8 bTurnOff, bAutoNROnOff=SV_OFF;
    PE_UI_RANGE_T QtyRange;

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
    LOG(6, "bDrvVideoGetSourceTypeTiming %d %d\n", u1SrcTypTmg, SOURCE_TYPE_TIMING_MAX);

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
    LOG(6, "bDrvVideoGetSourceTypeTiming(Path:%d)=Timing:0x%X\n",
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
        LOG(6, "PE_GetUiRange %d FAIL\n", u1UiItem);
        return SV_FALSE;
    }
	i2Min = UiRange.i4Min;
	i2Max = UiRange.i4Max;
	i2Dft = UiRange.i4Dft;
	bTurnOff = (i2Cur == 0);
    LOG(5, "PE_UI Path:%d Item:%d Min:%d Max:%d Dft:%d Cur:%d\n", u1VidPath, u1UiItem, i2Min, i2Max, i2Dft, i2Cur);

    if (bApiGetPeUiDebugMode() == SV_ON)
    {
        if (u1UiItem <= PE_ARG_NS)
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
		i2Max = i2Max - 1;
	}

       if(QtyRange.i4Dft  & R_AUTONR)
       {
            bAutoNROnOff = ((i2Cur==4)? SV_ON : SV_OFF);
            if(i2Cur==4)
            {
                i2Cur = (INT16)DRVCUST_AutoNRDefSet();
            }        
       }

	// Fill Quality Table
	if(QtyRange.i4Dft & R_QTYMAP)
	{
        for (u2QtyItem=QtyRange.i4Min; u2QtyItem <= QtyRange.i4Max; u2QtyItem++)
        {
        	UINT16 u2Value = bApiQtyMapping(u1SrcTypTmg, u2QtyItem, i2Min, i2Max, i2Dft, i2Cur);
        	wWriteQualityTable(u2QtyItem, u2Value);             
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
    case PE_ARG_SHARPNESS_L:
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
		vUi_NR(u1VidPath, bTurnOff);
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
		vUi_3D_NR(u1VidPath, i2Cur, bAutoNROnOff);
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
		vUi_MPEG_NR(u1VidPath, bTurnOff);    	
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
    case PE_ARG_TDES:
    	vUi_TDES(u1VidPath, bTurnOff);
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
    default:
		LOG(5, "Quality Item (%d) Not Support Yet\n", u1UiItem);    	
        return SV_FALSE;
    }
    return SV_TRUE;
}

