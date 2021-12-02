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
 * $RCSfile: mtvdo.h,v $
 * $Revision: #7 $
 *---------------------------------------------------------------------------*/ 

/** @file mtvdo.h
 *  This header file declares exported APIs of Video (VDO) module.
 */

#ifndef MT_VDO_H
#define MT_VDO_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"
      
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/**The video plane mode
*/
#define VDO_MODE_NORMAL             0		///< normal mode, video out
#define VDO_MODE_BG                 1		///< background mode, mute
#define VDO_MODE_UNKNOWN            2

#define PANEL_FILE_HEAD_VER_SIZE                16
#define PANEL_TBL_HEAD_SCRIPT_SIZE           48
#define VER_SIZE	10
#define SHORT_NAME_SIZE	24
#define LONG_NAME_SIZE	48
#define VDO_CHROMA_HIST_NUM         8
#define VDO_LUMA_HIST_NUM           32

//Ptf Version info
typedef struct 
{
    UINT8 bPtfVer[PANEL_FILE_HEAD_VER_SIZE];
    UINT8 bPtfPanelName[PANEL_TBL_HEAD_SCRIPT_SIZE];
} PTF_INFO;

typedef enum
{
    MTVDO_SRM_TV_MODE_TYPE_NORMAL = 0,
    MTVDO_SRM_TV_MODE_TYPE_PIP,
    MTVDO_SRM_TV_MODE_TYPE_POP,
    MTVDO_SRM_TV_MODE_TYPE_SCART_OUT_ON,
    MTVDO_SRM_TV_MODE_TYPE_SCART_OUT_OFF,
    MTVDO_SRM_TV_MODE_TYPE_LAST_VALID_ENTRY      /* only for counting purpose */
} MTVDO_SRM_TV_MODE_T;


typedef enum
{
    VDO_PQ_RGB_GAIN_OFST = 0,
    VDO_PQ_RGB_GAIN,
    VDO_PQ_RGB_OFST1,
    VDO_PQ_RGB_OFST2,
    VDO_PQ_EXP_COLOR,
    VDO_PQ_XVYCC,
    VDO_PQ_XVRGB_GAIN,
    VDO_PQ_ADAP_LUMA_CURVE_MANUAL,
    VDO_PQ_ADAP_LUMA_CURVE,
    VDO_PQ_MATRIX_INPUT,
    VDO_PQ_MATRIX_OUTPUT,
    VDO_PQ_GAMMA_CURVE,
    VDO_PQ_SCE,
    VDO_PQ_SCE_PARTIAL_START_IDX,
    VDO_PQ_SCE_PARTIAL,
    VDO_PQ_YLEV_SHARP,
    VDO_PQ_BOB_MODE,
    VDO_PQ_FILM_ON_OFF,
    VDO_PQ_CHROMA_CORING,
    VDO_PQ_RGB_PATTERN,
    VDO_PQ_SCE_PATTERN,
    VDO_PQ_MJC_FBCK_CURVE,
    VDO_PQ_DIR_SET,
    VDO_PQ_DIR_GET,
    VDO_PQ_LUMA_INFO_GET,
    VDO_PQ_CHROMA_INFO_GET,
    VDO_PQ_FILM_STATUS_GET,
    VDO_PQ_SCE_INPUT_CAP_GET,
    VDO_PQ_SCE_OUTPUT_CAP_GET,
    VDO_PQ_OSTG_CAP_GET,
    VDO_PQ_CONTRAST_SKIN_CURVE,
    VDO_PQ_COLOR_CURVE,
    VDO_PQ_MOTION_LEVEL_GET,
    VDO_PQ_PICTURE_MODE,
    VDO_PQ_COLOR_TEMPERATURE,
    VDO_PQ_ECO_SWITCH,
    VDO_PQ_DBL_MM_SOURCE_SELECT,
    VDO_PQ_HDMI_TRICK_MODE,
    VDO_PQ_PICTURE_MODE_UI,
    VDO_PQ_VGA_COLOR_SPACE_SET,
    VDO_PQ_NS           // total pq items
} MTVDO_PQ_TYPE_T;

typedef enum
{
    MTVDO_TAG3D_2D      = 0,
    MTVDO_TAG3D_MVC,          //MVC = Multi-View Codec 
    MTVDO_TAG3D_FP,           //FP = Frame Packing
    MTVDO_TAG3D_FS,           //FS = Frame Sequential 
    MTVDO_TAG3D_TB,           //TB = Top-and-Bottom 
    MTVDO_TAG3D_SBS,          //SBS = Side-by-Side
    MTVDO_TAG3D_REALD,        //RealD
    MTVDO_TAG3D_SENSIO,       //Sensio
    MTVDO_TAG3D_LI,           //LI = Line Interleave 
    MTVDO_TAG3D_CB,           //CB = Checker Board
    MTVDO_TAG3D_DA,           //DA = Dot Alternative
    MTVDO_TAG3D_TTDO,         //TTD only
    MTVDO_TAG3D_NOT_SUPPORT
} MTVDO_TAG3D_TYPE_T;

typedef struct 
{
    UINT16 u2RGain;
    UINT16 u2GGain;
    UINT16 u2BGain;
    UINT16 u2ROffset;
    UINT16 u2GOffset;
    UINT16 u2BOffset;
} MTVDO_PQ_RGB_GAIN_OFST_T;

typedef struct 
{
    UINT16 u2RGain;
    UINT16 u2GGain;
    UINT16 u2BGain;
} MTVDO_PQ_RGB_GAIN_T;

typedef struct
{
    UINT16 u2ROffset;
    UINT16 u2GOffset;
    UINT16 u2BOffset;
} MTVDO_PQ_RGB_OFST_T;

typedef struct 
{
    UINT8 u1Enable;
    UINT16 u2R;
    UINT16 u2G;
    UINT16 u2B;
} MTVDO_PQ_RGB_PATTERN_T;

typedef struct 
{
    UINT8 u1Enable;
    UINT16 u2Y;
    UINT16 u2Cb;
    UINT16 u2Cr;
} MTVDO_PQ_SCE_PATTERN_T;

enum
{ 
    ENUM_DI_FILM = 0, 
    ENUM_MJC_FILM = 1,       
};

enum
{ 
    ENUM_22_FILM = 0, 
    ENUM_32_FILM = 1,       
    ENUM_UC_FILM = 2
};

enum
{ 
    ENUM_COLORSPACE_FORCE_RGB = 0,
    ENUM_COLORSPACE_FORCE_YCBCR = 1,
    ENUM_COLORSPACE_AUTO = 2,
};

typedef struct 
{
    UINT8 u1Module;
    UINT8 u1FilmType;
    UINT8 u1OnOff;  
} MTVDO_PQ_FILM_ONOFF_T;

typedef struct 
{
    UINT8 u1Enable;
    UINT8 u1YBound;
    UINT8 u1CbTbl[16];
    UINT8 u1CrTbl[16];
} MTVDO_PQ_CHROMA_CORING_T;

typedef struct 
{
    UINT8 u1LumaMaxL;
    UINT8 u1LumaMaxR;
    UINT8 u1LumaMinL;
    UINT8 u1LumaMinR;
    UINT8 u1APLL;
    UINT8 u1APLR;
    UINT16 u2LumaHistL[32];
    UINT16 u2LumaHistR[32];
} MTVDO_PQ_LUMA_INFO_T;

typedef struct 
{
    UINT16 u2ChromaHistL[8];
    UINT16 u2ChromaHistR[8];
    UINT16 u2HueHistL[8];
    UINT16 u2HueHistR[8];
    UINT16 u2WindowL[3];
    UINT16 u2WindowR[3];
} MTVDO_PQ_CHROMA_INFO_T;

typedef struct 
{
    UINT16 u2PosX;        // AP prepared position. API will not modify it.
    UINT16 u2PosY;        // AP prepared position. API will not modify it.
    UINT16 u2ValCh[3];    // API will return the pixel value
} MTVDO_PQ_CAPTURE_T;

typedef struct 
{
    UINT8 u1Module;        // AP prepared type: 0 (DI) or 1(MJC)
    UINT8 u1Result;        // return result: 0 (Video), 1 (3:2), 2 (2:2) 
} MTVDO_PQ_FILM_STATUS_T;

typedef struct 
{
    UINT8 u1OnOff;        
    UINT8 u1GammaOnOff;   
    UINT16 u2Matrix[9];
} MTVDO_PQ_XVYCC_T;

typedef struct 
{
    UINT8 u1Color;          // M, R, Y, G, C, B
    UINT8 u1Module;         // Luma, Sat, Hue
    UINT8 u1Value;          // 0 ~ 0xFF
} MTVDO_PQ_EXP_COLOR_T;

typedef struct
{
    UINT8 arMJCQMap[2][16];
    UINT16 arMJCQTbl[7][16];    // arMJCQTbl[5] is reserved
} MTVDO_PQ_MJC_FBCK_CURVE_T;

typedef struct
{
    UINT8 u1Index;
    UINT8 u1SCETbl[336];
} MTVDO_PQ_COLOR_CURVE_T;

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/**Video plane region (source/output).
*/
typedef struct
{
    UINT32 u4X;        	///< start position in x direction (u4X/1000 = start pixel/total pixel)
    UINT32 u4Y;        	///< start position in y direction (u4Y/1000 = start pixel/total pixel)
    UINT32 u4Width;  	///< width in percentage (u4Width/1000 = region width/total width)
    UINT32 u4Height; 	///< height in percentage (u4Width/1000 = region height/total width)
}MTVDO_REGION_T;

/**Video plane overscan region.
*/
typedef struct
{
    UINT32 u4Top;      	///<overscan percentage on top side (u4Top%)
    UINT32 u4Bottom;	///<overscan percentage on top side (u4Bottom%)
    UINT32 u4Left;     	///<overscan percentage on top side (u4Left%)
    UINT32 u4Right;   	///<overscan percentage on top side (u4Right%)
}MTVDO_OVERSCAN_REGION_T;

/** Brief of VDP_NONLINEAR_PRM_T
 *  Video plane nonlinear scaler factors.
 */
typedef struct
{
    UINT32 u4Start;
    UINT32 u4End;
    UINT32 u4Slope;
    UINT32 u4MiddlePoint;
} MTVDO_NONLINEAR_PRM_T;


/** Brief of MTVDO_NONLINEAR_MODE_ENUM_T
 *  Video plane nonlinear scaler factors.
 */
	typedef enum
	{
		MTVDO_NONLINEAR_OFF,
		MTVDO_NONLINEAR_AUTO,
		MTVDO_NONLINEAR_AUTO_1,
		MTVDO_NONLINEAR_NEW,
		MTVDO_NONLINEAR_MANUAL
	} MTVDO_NONLINEAR_MODE_ENUM_T;


/**  Scalar dram data format
*/
typedef struct
{
	UINT32 u4VR;
	UINT32 u4UB;
	UINT32 u4YG;
}MTVDO_SCLAR_BUFFER_DATA;

/**Video callback structure
*/
typedef struct
{
    UINT32 u4Arg1;
    UINT32 u4Arg2;
    UINT32 u4Arg3;
    UINT32 u4Arg4;
    UINT32 u4Arg5;
    UINT32 u4Arg6;
    UINT32 u4Arg7;    
}MTVDO_CB_T;


/**Source change notify function type
*/
typedef void (*MTVDO_SRC_CHG_NOTIFY)(
    MTVDO_PATH_T eVdpId, 
    UINT32 u4SrcWidth, 
    UINT32 u4SrcHeight, 
    UINT32 u4OutWidth, 
    UINT32 u4OutHeight,
    UINT32 u4FrameRate,
    UINT32 u4Interlace
    );

/**PTS reach notify function type
*/
typedef void (*MTVDO_PTS_NOTIFY)(
    MTVDO_PATH_T eVdpId, 
    UINT32 u4Pts, 
    UINT32 u4Arg
    );

/**AFD/Aspect Ratio change notify function type
*/
typedef void (*MTVDO_AFD_NOTIFY)(
    MTVDO_PATH_T eVdpId, 
    UINT32 u4Afd, 
    UINT32 u4AspectRatio
    );

/**UnMute status change notify function type
*/
typedef void (*MTVDO_UNMUTE_NOTIFY)(
    MTVDO_PATH_T eVdpId, 
    BOOL fgEnable
    );


/**Lip sync notify function type
*/
typedef void (*MTVDO_LIPSYNC_NOTIFY)(
void
);

/**output V sync notify function type
*/
typedef void (*MTVDO_OUT_VSYNC_NOTIFY)(
void
);

typedef void (*MTVDO_VSS_NOTIFY)
(
void
);

/**TDNAVI notify function type
*/
typedef void (*MTVDO_TDNAVI_NOTIFY)
    (
        MTVDO_PATH_T eVdpId,
        MTVDO_TAG3D_TYPE_T e3DTag    
    );

/**Video plane call back function type id.
*/
typedef enum
{
    VDO_CB_FUNC_SRC_CHG_IND,    ///< Source Change Notify
    VDO_CB_FUNC_DISP_SUPPORT_IND,   //Scaler display mode not support notify
    VDO_CB_FUNC_PTS_IND,        ///< PTS Reach Notify
    VDO_CB_FUNC_AFD_IND,        ///< AFD/Aspect Ratio Change Notify
    VDO_CB_FUNC_UNMUTE_IND,     ///< UnMute Status Change Notify
    VDO_CB_FUNC_LIPSYNC_IND,     ///< Lip Sync Notify    
    VDO_CB_FUNC_MM_COND_IND,        ///< Vdp Condition Notify for MM
    VDO_CB_FUNC_MM_STEP_FIN_IND,        ///< Vdp Step Forward finish for MM
    VDO_CB_FUNC_MM_SEEK_FIN_IND,        ///< Vdp Seek finish for MM
    VDO_CB_FUNC_REPEAT_DROP_IND,        ///< Vdp repeat or drop    
    VDO_CB_FUNC_LIPSYNC_OK_TO_NG_IND,   //< Vdp Lip Sync OK to NG notify
    VDO_CB_FUNC_LIPSYNC_NG_TO_OK_IND,   //< Vdp Lip Sync NG to OK notify   
    VDO_CB_FUNC_OUT_VSYNC_IND,  /// < output v-sync. Notify
    VDO_CB_FUNC_VSS_FIN_IND,    /// VSS finish notify DEBUG
    VDO_CB_FUNC_TDNAVI_IND,     // TDNavi New content detected notify
    VDO_CB_FUNC_NS
}MTVDO_CB_FUNC_ENUM_T;

/**Video plane argument.
*/
typedef enum
{
	MTVDO_ARG_BRIGHTNESS = 0,
	MTVDO_ARG_CONTRAST,
	MTVDO_ARG_HUE,
	MTVDO_ARG_SATURATION,
	MTVDO_ARG_CTI,
	MTVDO_ARG_SHARPNESS,	
	MTVDO_ARG_SHARPNESS_H,
	MTVDO_ARG_SHARPNESS_L,
	MTVDO_ARG_LTI,
	MTVDO_ARG_R_GAIN,
	MTVDO_ARG_G_GAIN,
	MTVDO_ARG_B_GAIN,
	MTVDO_ARG_R_OFFSET,
	MTVDO_ARG_G_OFFSET,
	MTVDO_ARG_B_OFFSET,
	MTVDO_ARG_NR,
	MTVDO_ARG_GAMMA,
	MTVDO_ARG_WHITE_PEAK_LMT,
	MTVDO_ARG_SCE,
	MTVDO_ARG_SKIN_COLOR,
	MTVDO_ARG_ADAPTIVE_LUMA,
	MTVDO_ARG_BACK_LIGHT_LVL,
	MTVDO_ARG_ADAPTIVE_BACK_LIGHT,
	MTVDO_ARG_3D_NR,
	MTVDO_ARG_DEMO,
	MTVDO_ARG_DI_FILM_MODE,
	MTVDO_ARG_DI_MA,
	MTVDO_ARG_DI_EDGE_PRESERVE,
	MTVDO_ARG_MJC_MODE,
	MTVDO_ARG_MJC_EFFECT,
	MTVDO_ARG_MJC_DEMO,
	MTVDO_ARG_XVYCC,
	MTVDO_ARG_WCG,
	MTVDO_ARG_GAME_MODE,
	MTVDO_ARG_BLUE_STRETCH,
	MTVDO_ARG_MPEG_NR,
	MTVDO_ARG_BLACK_STRETCH,
	MTVDO_ARG_WHITE_STRETCH,
	MTVDO_ARG_SHARPNESS_ON_OFF,     
	MTVDO_ARG_OPC_CURVE,
    MTVDO_ARG_CBE,
    MTVDO_ARG_USPEI,
    MTVDO_ARG_LCDIM,   
    MTVDO_ARG_TDTV_PACKING,
    MTVDO_ARG_TDTV_INVERSE,
    MTVDO_ARG_TDTV_DEPTH,
    MTVDO_ARG_TDTV_3D_TO_2D,
    MTVDO_ARG_TDTV_FPR,
    MTVDO_ARG_TDTV_PROTRUDE,
    MTVDO_ARG_TDTV_DISTANCE,
    MTVDO_ARG_TDTV_OSD_DEPTH,     
    //Caihua Add for Vestel
    MTVDO_ARG_TVD_COMB,
    //Caihua Add 20110901
    MTVDO_ARG_PIC_UI_DFT,
    //Caihua Add 20110916
    MTVDO_ARG_SIX_COLOR, 
    //Caihua Add 20120326
    MTVDO_ARG_TDTV_SIX_COLOR,
    MTVDO_ARG_TDTV_SCE,
    MTVDO_ARG_TDTV_BRIGHTNESS,
    MTVDO_ARG_TDTV_CONTRAST,
    MTVDO_ARG_TDTV_HUE,
    MTVDO_ARG_TDTV_SATURATION,
    MTVDO_ARG_TDTV_R_GAIN,
    MTVDO_ARG_TDTV_G_GAIN,
    MTVDO_ARG_TDTV_B_GAIN,
    MTVDO_ARG_TDTV_SKIN,
    //end
    MTVDO_ARG_NS
} MTVDO_ARG_TYPE_T;

typedef enum {
    MTVDO_3D_MODE_OFF  =0,
    MTVDO_3D_MODE_AUTO =1,
    MTVDO_3D_MODE_TTD  =2,//2D -> 3D Conversion
    MTVDO_3D_MODE_FS   =3,//Frame Seq
    MTVDO_3D_MODE_SBS  = 4,//Side by Side    
    MTVDO_3D_MODE_TB   = 5,//Top Bottom    
    MTVDO_3D_MODE_RD   = 6,//Reald
    MTVDO_3D_MODE_SS   = 7,//Sensio    
    MTVDO_3D_MODE_LI   = 8,//Line Interleave
    MTVDO_3D_MODE_DA   = 9,//Dot Alternative    
    MTVDO_3D_MODE_CB   =10,//Checker Board
    MTVDO_3D_MODE_END    
}MTVDO_3D_PACKING;

typedef enum {
    MTVDO_LR_NORMAL  =0,
    MTVDO_LR_INVERSE =1
}MTVDO_LR;

typedef enum {
    MTVDO_3D_2_2D_OFF =0,
    MTVDO_3D_2_2D_ON  =1
}MTVDO_3D_2_2D;


//UI VQ MIN MAX DEFAULT 
typedef struct 
{
    INT32 i4Min;
    INT32 i4Max;
    INT32 i4Dft;
} MTVDO_UI_RANGE_T;

typedef struct structMatrix{
    INT16 i2Num[15];
}MTVDO_MATRIX_PARAM_T;
#ifdef CC_MT5392B
typedef struct structGammaTable{
    UINT8 u8Table[3][1024];
}MTVDO_GAMMA_TABLE_T;
#else
typedef struct structGammaTable{
    UINT8 u8Table[3][257];
}MTVDO_GAMMA_TABLE_T;
#endif // CC_MT5392B
typedef struct 
{
    UINT32 dwChromaHist[VDO_CHROMA_HIST_NUM];
} MTVDO_CHROMA_HIST_T;
typedef struct 
{
    UINT16 wLumaHist[VDO_LUMA_HIST_NUM];
} MTVDO_LUMA_HIST_T;

typedef struct 
{
    UINT8 bPath;
    UINT8 bType;
    UINT8 bMode;
} MTVDO_SCALING_ARG_T;
#define VER_SIZE	10
#define SHORT_NAME_SIZE	24
#define LONG_NAME_SIZE	48

#define PANEL_FILE_HEAD_VER_SIZE                16
#define PANEL_TBL_HEAD_SCRIPT_SIZE           48

//Pcf/Pqf Version info
typedef struct 
{
    UINT8 bPcfVer[VER_SIZE];
    UINT8 bPcfPanelName[LONG_NAME_SIZE];
    UINT8 bPqfVer[VER_SIZE];
    UINT8 bPqfPanelName[LONG_NAME_SIZE]; //[SHORT_NAME_SIZE];
} MTVDO_PCF_PQF_INFO_T;

/**Video Timing Info.
*/
typedef struct
{
    UINT32 u4HTotal;      	///< HTotal
    UINT32 u4VTotal;		///< VTotal
    UINT32 u4FrameRate;     	///< Frame Rate
    UINT32 u4HStart;                  ///< H start position
    UINT32 u4VStart;                  ///< V start position
    UINT32 u4ActiveWidth;   	///< Active Width
    UINT32 u4ActiveHeight;        ///< Active Height
    UINT32 u4Interlace;             ///< Is interlace or progressive
    UINT32 u4Phase;             ///< sampling phase
}MTVDO_TIMING_INFO_T;

/*
*  data format of frame or field 
*/
typedef struct
{
    BOOL fgIs10bit;
    BOOL fgIs422;
    BOOL fgIsInterlace;
    BOOL fgIsTopField;
    BOOL fgIsYUV; 
}MTVDO_FRAME_FORMAT_DATA;

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size; 
}MTVDO_VIDEO_BUFFER;

/*
*  for LG dump dram
*/
typedef struct
{
	UINT16 u2UB;
	UINT16 u2YG;
	UINT16 u2VR;
}MTVDO_DDI_PIXEL_DATA;

/*
*  Pscan data mode
*/
typedef enum
{
    MTVDO_PSCAN_DEFAULT,
    MTVDO_PSCAN_INPUT_SYNC_YC3D,
    MTVDO_PSCAN_OUTPUT_SYNC_YC3D,
    MTVDO_PSCAN_INPUT_SYNC_BOB,
    MTVDO_PSCAN_OUTPUT_SYNC_BOB,
    MTVDO_PSCAN_UNKNOWN,
}MTVDO_PSCAN_MODE_T;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/**Initialize VDO module.
*@param void
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_Init(void);

/**Stop VDO module.
*@param void
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_Stop(void); 

/**Reset VDO module.
*@param eVdpId                     Video plane id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_Reset(MTVDO_PATH_T eVdpId);

/**VDO set video plane enable.
*@param eVdpId                     Video plane id.
*@param fgEnable                   Enable/Disable.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetEnable(MTVDO_PATH_T eVdpId, BOOL fgEnable);

/**VDO query video plane enable/disable status.
*@param eVdpId                     Video plane id.
*@param pucEnable                  0:disable, 1:enable.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_GetEnable(MTVDO_PATH_T eVdpId, UCHAR *pucEnable);

/**VDO set video plane mode.
*@param eVdpId                     Video plane id.
*@param ucMode                     VDO_MODE_NORMAL or VDO_MODE_BG.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetMode(MTVDO_PATH_T eVdpId, UCHAR ucMode);

/**VDO query video plane mode.
*@param eVdpId                     Video plane id.
*@param pucMode                    VDO_MODE_NORMAL or VDO_MODE_BG.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_GetMode(MTVDO_PATH_T eVdpId, UCHAR *pucMode);


/**VDO set video plane backlight mode.
*@param eVdpId                     Video plane id.
*@param ucMode                     ice_backlight_dimming_low or ice_backlight_dimming_auto.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetBacklightMode(MTVDO_PATH_T eVdpId, UCHAR ucMode); 

/**VDO set video plane power save mode.
*@param eVdpId                     Video plane id.
*@param ucMode                     ice_backlight_dimming_off or ice_backlight_dimming_power_save.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetPowerSaveMode(MTVDO_PATH_T eVdpId, UCHAR ucMode); 

/**VDO set video plane with freeze feature.
*@param eVdpId                     Video plane id.
*@param fgEnable                   Enable/disable freeze function.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetFreeze(MTVDO_PATH_T eVdpId, BOOL fgEnable);

/**VDO query video plane with freeze feature.
*@param eVdpId                     Video plane id.
*@param pucFreeze                  0:disable, 1:freeze feature enable.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  MTVDO_GetFreeze(MTVDO_PATH_T eVdpId, UCHAR *pucFreeze);

/**VDO set nonlinear parameters.
*@param eVdpId                     Video plane id.
*@param eMode                      MTVDO_NONLINEAR_OFF/ MTVDO_NONLINEAR_AUTO/MTVDO_NONLINEAR_MANUAL
*@param ePrm			    OFF: NULL, AUTO:slope and end factor, MANUAL: begin, end, middle, slope should be specified.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetNonLinearMode(MTVDO_PATH_T eVdpId,MTVDO_NONLINEAR_MODE_ENUM_T eMode, MTVDO_NONLINEAR_PRM_T *ePrm);

/**VDO set video plane dot by dot.
*@param eVdpId                     Video plane id.
*@param fgEnable                   Enable/disable dot by dot function.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetDotByDot(MTVDO_PATH_T eVdpId, BOOL fgEnable);

/**VDO set video quality item.
*@param eVdpId                     Video plane id.
*@param ucArg                      The color item.
*@param i2Value                    The value of the color item.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetArg(MTVDO_PATH_T eVdpId, MTVDO_ARG_TYPE_T eArgType, INT16 i2Value);

/**VDO query video quality item setting.
*@param eVdpId                     Video plane id.
*@param ucArg                      The color item.
*@param pi2Value                   The value of the color item.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_GetArg(MTVDO_PATH_T eVdpId, MTVDO_ARG_TYPE_T eArgType, INT16 *pi2Value);

/**VDO set overscan region.
*@param eVdpId                     Video plane id.
*@param rRegion                    Overscan percentage setting.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetOverScan(MTVDO_PATH_T eVdpId, MTVDO_OVERSCAN_REGION_T *prRegion);

/**VDO get overscan region.
*@param eVdpId                     Video plane id.
*@param prRegion                   Overscan percentage setting.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_GetOverScan(MTVDO_PATH_T eVdpId, MTVDO_OVERSCAN_REGION_T *prRegion);

/**VDO set source region.
*@param eVdpId                     Video plane id.
*@param rSrcRegion                 Source region description.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetSrcRegion(MTVDO_PATH_T eVdpId, MTVDO_REGION_T* prSrcRegion);

/**VDO query source region.
*@param eVdpId                     Video plane id.
*@param prSrcRegion                Source region description.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_GetSrcRegion(MTVDO_PATH_T eVdpId, MTVDO_REGION_T *prSrcRegion);

/**VDO set output region.
*@param eVdpId                     Video plane id.
*@param rOutRegion                 Output region description.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetOutRegion(MTVDO_PATH_T eVdpId, MTVDO_REGION_T* prOutRegion);

/**VDO query output region.
*@param eVdpId                     Video plane id.
*@param rOutRegion                 Output region description.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_GetOutRegion(MTVDO_PATH_T eVdpId, MTVDO_REGION_T *prOutRegion);

EXTERN MT_RESULT_T MTVDO_SetMuteOnOff(UINT32 u4VdpId, UCHAR fgEnable); //Caihua 20120601

/**VDO set background color.
*@param eVdpId                     Video plane id.
*@param u4BgColor                  The color in RGB color mode, 1 byte per color component.(e.g. Red: 0xFF0000).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetBg(MTVDO_PATH_T eVdpId, UINT32 u4BgColor);

/**VDO query background color.
*@param eVdpId                     Video plane id.
*@param u4BgColor                  The color in RGB color mode, 1 byte per color component.(e.g. Red: 0xFF0000).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_GetBg(MTVDO_PATH_T eVdpId, UINT32 *pu4BgColor);

/**VDO call back function register.
*@param eFuncType                  The call back function type.
*@param u4FuncPtr                  The call back function pointer.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_RegCbFunc(MTVDO_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr, UINT32 u4Arg1, UINT32 u4Arg2);

/**VDO set game mode on/off.
*@param eVdpId                     Video plane id.
*@param fgEnable                   Enable/disable game mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetGameMode(UINT32 u4VdpId, BOOL fgEnable);

/**VDO set HDMI True Black on/off.
*@param eVdpId                     Video plane id.
*@param fgEnable                   Enable/disable HDMI True Black.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetHDMITrueBlack(UINT32 u4VdpId, BOOL fgEnable);

/**VDO set 3D game mode on/off.
*@param eVdpId                     Video plane id.
*@param fgOnOff                    Enable/disable 3D game mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_Set3DGameMode(UINT32 u4VdpId, BOOL fgEnable);

/**VDO get DTV playback unmute status.
*@param pfgEnable                  TRUE: Unmute, FALSE: Mute.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_GetUnMuteStatus (MTVDO_PATH_T eVdpId, BOOL *pfgEnable);

/**VDO MTVDO_Scalar_EnableGetBuffer
* calling sequence: Enable GetBuffer -> GetBufferAvg -> Disable GetBuffer
*@param fgEnable                    TRUE/FALSE.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_Scalar_EnableGetBuffer(BOOL fgEnable);

/**VDO scalar : Get the average data of scalar frame buffer.
* calling sequence: GetBufferOn -> GetBufferAvg -> GetBufferOff
*@param prRegion            the region of frame Buffer.
*@param prData                     the average data of prRegion in frame buffer.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_Scalar_GetBufferAvg(MTVDO_REGION_T *prRegion, MTVDO_SCLAR_BUFFER_DATA *prData);

/**VDO get timing information.
*@param eVdpId                     Video plane id.
*@param prInfo                    Timing information.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_GetTimingInfo(MTVDO_PATH_T eVdpId, MTVDO_TIMING_INFO_T *prInfo);

/**VDO Enable/Disable Factory mode (CVBS output with OSD) 
*@param fgEnable                     Factory mode Enable/Disable.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetFactoryMode(BOOL fgEnable);

/**VDO Set Auto Mute On or Off
*@param fgAutoMuteOff : TRUE: Auto Mute On; FALSE: Auto Mute Off
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetAutoMuteOnOff(BOOL fgAutoMuteOff);

/**VDO Set Scaler Pixel Based control On or Off
*@param fgEnable : TRUE: Pixel Based Control On; FALSE: Pixel Based Control Off
*@retval MTR_OK 				   Success.
*@retval MTR_NOT_OK 			   Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetScalerPixelBased(BOOL fgEnable); // drmyung 081226

/**VDO Set xvYcc Inner Pattern Gen
*@param bOnOff : on / off xvycc pattern gen
*@param wPatGenR : xvycc pattern gen r value
*@param wPatGenG : xvycc pattern gen g value
*@param wPatGenB : xvycc pattern gen b value
*@retval MTR_OK 				   Success.
*@retval MTR_NOT_OK 			   Fail.
*/
EXTERN MT_RESULT_T MTVDO_SetxvYccInnerPatternGen(UINT8 bOnOff, UINT16 wPatGenR, UINT16 wPatGenG, UINT16 wPatGenB);

/**VDO Set PE_UI_VQ_MIN_MAX_DFT Parameters
*@param  prUIParam		   UI parameters
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/    
EXTERN MT_RESULT_T MTVDO_SetPeUiRangeParam(MTVDO_PATH_T eVdpId, MTVDO_ARG_TYPE_T eArgType, const MTVDO_UI_RANGE_T *prUIParam);

/**VDO Get PE_UI_VQ_MIN_MAX_DFT Parameters
*@param  prUIParam		   UI parameters
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/    
EXTERN MT_RESULT_T MTVDO_GetPeUiRangeParam(MTVDO_PATH_T eVdpId, MTVDO_ARG_TYPE_T eArgType, MTVDO_UI_RANGE_T *prUIParam);


EXTERN MT_RESULT_T MTVDO_SetScanMode(UINT32 *pscan);

/**VDO Set pre-down scaling factor
*@param  eVdpId		   path
*@param  u4Factor	   pre-down scaling factor to set
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/    
EXTERN MT_RESULT_T MTVDO_SetDSFactor(MTVDO_PATH_T eVdpId, UINT32 u4Factor);

/**VDO Set PQ interface
*@param  eVdpId                 path
*@param  ePqType                PQ type to set
*@param  pvPqSetInfo            PQ data structure to set
*@param  u4Size                 size of the passing PQ data structure
*@retval MTR_OK                 Success.
*@retval MTR_NOT_OK             Set fail.
*@retval MTR_NOT_SUPPORTED      Not supported function
*@retval MTR_PARAMETER_ERROR    read-only type
*/
EXTERN MT_RESULT_T MTVDO_SetPQ(MTVDO_PATH_T eVdpId, MTVDO_PQ_TYPE_T ePqType, const void* pvPqSetInfo, UINT32 u4Size);

/**VDO Get PQ interface
*@param  eVdpId                 path
*@param  ePqType                PQ type to get
*@param  pvPqGetInfo            PQ data structure to get
*@param  u4Size                 size of the PQ data structure for retrieving
*@retval MTR_OK                 Success.
*@retval MTR_NOT_OK             Get fail.
*@retval MTR_NOT_SUPPORTED      Not supported function.
*@retval MTR_PARAMETER_ERROR    Not support for read.
*/
EXTERN MT_RESULT_T MTVDO_GetPQ(MTVDO_PATH_T eVdpId, MTVDO_PQ_TYPE_T ePqType, void* pvPqGetInfo, UINT32 u4Size);
EXTERN MT_RESULT_T MTVDO_SetMJCMotionCompensationOnOff(UINT8 fgOnOff);
EXTERN MT_RESULT_T MTVDO_SetMJCDemoBar(UINT8 fgOnOff, UINT8 r, UINT8 g, UINT8 b);
EXTERN MT_RESULT_T MTVDO_SetMJCInitialize(void);
EXTERN MT_RESULT_T MTVDO_SetMJCEffectLevel(UINT8 bLevel);
EXTERN MT_RESULT_T MTVDO_SetMJCDemoMode(BOOL fgOnOff, UINT8 bMode, UINT16 wCenter);
EXTERN MT_RESULT_T MTVDO_SetMJCBypassWindow(UINT8 bWindow, MTVDO_REGION_T rRegion);

EXTERN MT_RESULT_T MTVDO_2DOnlySetLR(MTVDO_2DLRMode e2DMode);
EXTERN MT_RESULT_T MTVDO_Set3dMode(MTVDO_PATH_T eVdpId, MTVDO_3DMode eVdo3dMode);
EXTERN MT_RESULT_T MTVDO_3DSetDepth(MTVDO_3DDepth  e3DDepth);
EXTERN MT_RESULT_T MTVDO_3DSetInverse(MTVDO_3DInverse e3DInverse);

EXTERN MT_RESULT_T MTVDO_SetOutputWindow(MTVDO_PATH_T eVdpId, MTVDO_REGION_T *prOutRegion);
EXTERN MT_RESULT_T MTVDO_SRM_SetTvMode(MTVDO_SRM_TV_MODE_T eMode);
EXTERN MT_RESULT_T MTVDO_GetPlaneWH(UINT32 *pWidth,UINT32 *pHeight);


EXTERN MT_RESULT_T MTVDO_SetTDTVPacking(MTVDO_PATH_T eVdpId ,INT16 i2Value);
EXTERN MT_RESULT_T MTVDO_GetTDTVPacking(MTVDO_PATH_T eVdpId, INT16 *pi2Value);

EXTERN MT_RESULT_T MTVDO_SetTDTV3D22D(MTVDO_PATH_T eVdpId,MTVDO_3D_2_2D e3D22DMode);
EXTERN MT_RESULT_T MTVDO_GetTDTV3D22D(MTVDO_PATH_T eVdpId, INT16 *pi2Value);

EXTERN MT_RESULT_T MTVDO_GetTDTVGetTag3D(MTVDO_PATH_T eVdpId,INT16 *pi2Value);
EXTERN MT_RESULT_T MTVDO_GetTDTVGetNaviTag3D(MTVDO_PATH_T eVdpId,INT16 *pi2Value);
EXTERN MT_RESULT_T MTVDO_SetTDTVNaviMode(INT16 i2NaviMode);
EXTERN MT_RESULT_T MTVDO_SetTDTVDCOnOff(UINT8 u1OnOff);

EXTERN MT_RESULT_T MTVDO_SetTDTVProtrude(MTVDO_PATH_T eVdpId, INT16 i2Protrude);
EXTERN MT_RESULT_T MTVDO_GetTDTVProtrude(MTVDO_PATH_T eVdpId, INT16 *pi2Value);
EXTERN MT_RESULT_T MTVDO_SetTDTVDistance(MTVDO_PATH_T eVdpId, INT16 i2Distance);
EXTERN MT_RESULT_T MTVDO_GetTDTVDistance(MTVDO_PATH_T eVdpId, INT16 *pi2Value);
EXTERN MT_RESULT_T MTVDO_SetTDTVOSDDepth(MTVDO_PATH_T eVdpId, INT16 i2OSDDepth);
EXTERN MT_RESULT_T MTVDO_GetTDTVOSDDepth(MTVDO_PATH_T eVdpId, INT16 *pi2Value);
EXTERN MT_RESULT_T MTVDO_SetTDTVPanel(UINT8 type);
EXTERN MT_RESULT_T MTVDO_SetBacklightMode(MTVDO_PATH_T eVdpId, UCHAR ucMode);
EXTERN MT_RESULT_T MTVDO_GetPcfPqfInfo(MTVDO_PATH_T eVdpId,  MTVDO_PCF_PQF_INFO_T *pPcfPqfInfo);

#ifdef __cplusplus
}
#endif

#endif //MT_VDO_H
