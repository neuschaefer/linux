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
 * $RCSfile: drv_di_int.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////////////
#include "general.h"
#include "typedef.h"
#include "sv_const.h"
#include "vdo_misc.h"
#include "hw_vdoin.h"
#include "x_bim.h"
#include "di_debug.h"
#include "x_assert.h"
#include "drv_di.h"
#include "hw_di_int.h"
#include "hw_di.h"
#include "video_def.h"
#include "source_select.h"
#include "video_timing.h"
#include "hw_ycproc.h"

#ifdef CC_DRV_SUPPORT_SMICRO
#include "SMicro/SMicro_if.h"
#endif

#ifdef CC_UP8032_ATV
#undef CC_DEBUG
#define MDDI_PD_SW_FW_FILM_PATCH    0
#else
#define MDDI_PD_SW_FW_FILM_PATCH    1   //Patch for Some Scenario HW Can not detect
#endif

#if defined(CC_DEBUG)
#define DI_DEBUG_EVENT_NUM 6
#else
#define DI_DEBUG_EVENT_NUM 0
#endif

//HW SPEC
#define MEMA_LRG_BLK_H_SIZE     1024
#define MEMA_LRG_BLK_V_SIZE     576
#define CS_LIMIT_H_SIZE         720

#define FW_FILM_FR_SEQ_32       0x210   //b1000010000
#define FW_FILM_SEQ_32          0x94    //b010010100
#define FW_FILM_SEQ_22          0xAA    //b10101010
#define FW_FILM_MASK            0x1F    //b11111
#define FW_FILM_CB              5

#define MDDI_FILM_ACC_MAX_SEQ_CNT    0x60
#define MDDI_FILM_MAX_SEQ_CNT        0xFF

#define FILM_MASK(bit)      ((1<<bit)-1)
#define FILM_HIGHBIT(bit)   (1<<(bit-1))
#define FILMMASKNUM             5
UINT32 u4MaskBit[FILMMASKNUM] = {15, 12, 10, 5, 4};

//Status Filter
#define DILOOPNUM               8   // Must Be Multiple Of 2 For Calculation
#define DILOOPMASK              (DILOOPNUM-1) 

#define DI_MOTION_HIST_BIN      16
#define DI_BE_HIST_BIN          12
#define DI_CS_LINE_BIN          17
#define DI_CS_MV_HIST_BIN       38

#define CONTAIN_LESS_THAN_ONE(x) ((x)-((x)&(~((x)-1))))

//For Status Polling Register
UINT8 fgDIStatusPolling;
UINT32 u4aSatSumSta;

void vDrvDIMEMAStatusPolling(void *pvArgv);
void vDrvDITextLineStatusPolling(void *pvArgv);
void vDrvDIMotionStatusPolling(void *pvArgv);
void vDrvDICMotionStatusPolling(void *pvArgv);
void vDrvDIFWFILMStatusPolling(void *pvArgv);
void vDrvDIFilmStatusPolling(void *pvArgv);
void vDrvDIMVStatusPolling(void *pvArgv);
void vDrvDIMVDiffStatusPolling(void *pvArgv);
void vDrvDIBEPolling(void *pvArgv);
void vDrvDIEventStatusPolling(void *pvArgv);
void vDrvDIEventProfileStatusPolling(void *pvArgv);
void vDrvDICSPolling(void *pvArgv);
void vDrvDIAdaptivePolling(void *pvArgv);

DI_PQ_THREAD DIPQThread[]=
{
#if defined(CC_DEBUG)
    {vDrvDIMotionStatusPolling,     "Luma Motion Histogram"},
    {vDrvDICMotionStatusPolling,    "Chroma Motion Histogram"},
    {vDrvDIMEMAStatusPolling,       "MEMA Status"},
    {vDrvDITextLineStatusPolling,   "Text Line Status"},
    {vDrvDIFWFILMStatusPolling,     "FW FILM"},
    {vDrvDIFilmStatusPolling,       "HW FILM"},
    {vDrvDIMVStatusPolling,         "MV Hist"},
    {vDrvDIMVDiffStatusPolling,     "MVDiff Hist"},
    {vDrvDIBEPolling,               "BE Hist"},
    {vDrvDIEventStatusPolling,      "Event Status"},
    {vDrvDIEventProfileStatusPolling,   "Event Profile Status"},
    {vDrvDICSPolling,                   "CS Status"},
    {vDrvDIAdaptivePolling,             "AdaptiveFW Status"},
#endif
    {NULL                       ,NULL}//End Of Function Table
};

//MEMA Status
typedef struct
{
    UINT32 u4Heter;
    UINT32 u4Homo;
    UINT32 u4HEdgeCnt;
    UINT32 u4VEdgeCnt;
    UINT32 u4EdgeSum;
    UINT32 u4Saw;
    UINT8 u1FMDType;    
    UINT32 u4HeterUp;
    UINT32 u4HomoUp;
    UINT32 u4HeterDn;
    UINT32 u4HomoDn;
    
#if defined(CC_DEBUG)
    UINT32 u4HeterPre;
    UINT32 u4HomoPre;
    UINT32 u4ML;
    UINT32 u4AML;
    UINT32 u4CurFieldLumaAvg;
    UINT32 u4PreFieldLumaAvg;
    UINT16 u2MotionHist[DI_MOTION_HIST_BIN];    
    UINT32 u4MotionPxl;
    UINT32 u4LargeMotionPxl;
#endif
}DI_PQ_MA_STA;

//BE Status
typedef struct
{
    UINT32 u4AliasCnt;
    UINT32 u4EdgeSum;
    UINT32 u4EdgeHis[DI_BE_HIST_BIN];
#if defined(CC_DEBUG)
    UINT32 u4GridPos;
    UINT32 u4GMVC;
    UINT32 u4GMVX;
#endif
} DI_PQ_BE_STA;

typedef enum
{
    DETECTSTAGE,
    TEXTSTAGE        
} LINE_CS_STATE;


typedef struct
{
    UINT16 u2StartLine;
    UINT16 u2EndLine;
    UINT16 u2ContiCnt;
    UINT16 u2TotalCnt; 
} TEXT_INFO;


//CS Status
typedef struct
{
    UINT8 bAllBurst;     
    UINT8 bHIST_MAX_I2;
    UINT8 bHIST_MAX_I1;
    UINT32 bHIST_MAX_I2_Value;
    UINT32 bHIST_MAX_I1_Value;   
    UINT32 u4MVHist[DI_CS_MV_HIST_BIN/2]; // 1 Index Contain 2 Hist To Save ISR Run Time
    UINT32 u4LineValid[DI_CS_LINE_BIN];
#if defined(CC_DEBUG)
    UINT16 u2MVHistDiff[DI_CS_MV_HIST_BIN];
    INT32 u4MVHistDiffSum;
    UINT8 bFALLBACK_CHANGE_CNT;
    UINT8 bFALLBACK_STATUS;
    UINT8 bFALLBACK_STEP;
    UINT32 u4VSI_ACCUM;
    UINT32 u4MC_WEI_Y_ACCUM;
    UINT32 u4MV_RAW_INVALID_ACCUM;
    UINT32 u4CS_HIST_GMVX;
    UINT32 u4SC_LUMA_DIFF_VALUE;
    UINT32 u4SC_SAD_VALUE;
    UINT32 u4MOTION_MV_CNT;
    UINT32 u4SB_NONEDGE_CNT;
    UINT32 u4SB_TEXTURE_CNT;
 #endif   
} DI_PQ_CS_STA;

//FILM Status
typedef struct
{
    UINT32 u4FrmMotion;
    UINT32 u4EdgMotion;
    UINT32 u4FldMotion;
    UINT8 bBlkMotion;
    UINT8 bFWPatch32FrSeq;  //Sequence to change 32 PD Detection
    UINT8 bFWPatchCB32;     //Current Combination of 32 PD Detection
    UINT8 bFWPatchFi32Cnt;  //Sequence to change 32 PD Detection
    UINT8 bFWPatchFr32Cnt;  //Sequence to change 32 PD Detection
    UINT8 bCurFilmMode;     //Non-Film, 32_Film, 22_Film     
    UINT32 u4FrmSeq;
    UINT32 u4FrmThld;
    UINT32 u4FrmMask;
    UINT32 u4FrmBit;
    UINT8 bFrmCnt;
    UINT8 bFrmCB;
    UINT32 u4FldSeq;    
    UINT32 u4FldThld;
    UINT32 u4FldMask;
    UINT32 u4FldBit;
    UINT8 bFldCnt;
    UINT8 bFldCB;
    UINT8 bFwAccCnt;    
    UINT8 bFwBadEdit;
    UINT8 bFwWeave;
    UINT8 bUnknowCnt;
} DI_PQ_PD_STA;


typedef struct
{
    DI_PQ_MA_STA MA;
    DI_PQ_BE_STA BE;
    DI_PQ_CS_STA CS;
    DI_PQ_PD_STA PD;
    UINT32 u4IsrIndex;
} DI_PQ_STA;

//MEMA Parameter
typedef struct
{
    UINT32 u4HeterMax;
    UINT32 u4HeterMin;
    UINT32 u4HomoMax;
    UINT32 u4HomoMin;
    UINT32 u4RatioMax;
    UINT32 u4RatioMin;
    UINT8 bHeterRate;
    UINT32 u4CZPAlias;
} DI_PQ_FW_MA_PARAM;

//CS Param
typedef struct
{   
    LINE_CS_STATE LineCSSta;
    TEXT_INFO TextInfo;
    UINT8 u1TextMode;
    UINT8 u1MixMode;    
} DI_PQ_FW_CS_PARAM;

#if 0
typedef struct
{    
} DI_PQ_FW_DAR_PARAM;
#endif

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Mask;
    UINT32 u4High;
    UINT32 u4Low;
    UINT32 u4DstAddr;
    UINT32 u4DstMask;
    UINT32 u4DstValue;
    UINT32 fgTrue;  //The Event Condition True
    UINT32 bOnOff;
} DI_PQ_FW_DBG_PARAM;

typedef struct
{
    DI_PQ_FW_MA_PARAM MA;
    DI_PQ_FW_CS_PARAM CS;
    //DI_PQ_FW_DAR_PARAM DAR;

#if DI_DEBUG_EVENT_NUM
    DI_PQ_FW_DBG_PARAM Event[DI_DEBUG_EVENT_NUM];
#endif
} DI_PQ_FW_PARAM;

typedef struct
{
    E_DI_QUALITY_MODE eMAQuality;
    E_DI_QUALITY_MODE eEdgePQuality;
    E_DI_QUALITY_MODE eFilmQuality;
    UINT8 bCfgFilmModeOnOff;
    UINT8 bCfgCSOnOff;
} DI_PQ_MODE;

DI_PQ_MODE DiPQMode;
DI_PQ_STA DiSta;
DI_PQ_FW_PARAM DiPar;

////////////////////////////////////////////////////////////////////////////////
// Register table
////////////////////////////////////////////////////////////////////////////////
const MDDI_REGTBL_T CODE MDDI_PDMA_INIT[] =
{
    {MDDI_REGTBL_END, 0x00000000, 0x00000000}
};

const MDDI_REGTBL_T CODE MDDI_PDMA_PARAM[] =
{
    {MDDI_REGTBL_END, 0x00000000, 0x00000000}
};

const MDDI_REGTBL_T CODE MDDI_MEMA_INIT[] =
{
    {MDDI_MEMA_14, 0x00000000, 0x0000FF00},
    {MDDI_REGTBL_END, 0x00000000, 0x00000000}
};

const MDDI_REGTBL_T CODE MDDI_MEMA_PARAM[4][12] =
{
    {   //RF
        //MEMA Parameter
        {MDDI_MEMA_00, 0x80037AAF, 0x80FFFFFF}, //Enable MAMF
        {MDDI_MEMA_01, 0x30868C6D, 0xFFFFFFFF},
        {MDDI_MEMA_02, 0xE0206240, 0xFFFFFFFF}, // edge adaptive motion threshold        
        {MDDI_MEMA_04, 0x00002000, 0x0000FF00},
        {MDDI_MEMA_09, 0x0000000F, 0x000000FF}, //MD_VER_MOTION_TH
        {MDDI_MEMA_0D, 0x00000500, 0x0000FFFF}, //MBURST_VAR_TH
        {MDDI_MEMA_12, 0x06150000, 0xFFFFFFFF}, //Still Threshold
        {MDDI_MEMA_13, 0x19020844, 0xFFFFFFFF}, 
        {MDDI_MEMA_17, 0x00000000, 0x0F000000}, //MF_ML_C_TH :Chroma offset to motion level. 
        //SB Parameter
        {MDDI_SB_00, 0x111BD00D, 0xFFFFFFFF},
        {MDDI_SB_04, 0x10040000, 0x100F0000}, // turn off B/D adaptive weighting        
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //SD
        //MEMA Parameter
        {MDDI_MEMA_00, 0x80037AAF, 0x80FFFFFF}, //Enable MAMF
        {MDDI_MEMA_01, 0x30868C6D, 0xFFFFFFFF},
        {MDDI_MEMA_02, 0xE0206240, 0xFFFFFFFF}, // edge adaptive motion threshold        
        {MDDI_MEMA_04, 0x00002000, 0x0000FF00},
        {MDDI_MEMA_09, 0x0000000F, 0x000000FF}, //MD_VER_MOTION_TH
        {MDDI_MEMA_0D, 0x00000500, 0x0000FFFF}, //MBURST_VAR_TH
        {MDDI_MEMA_12, 0x06150000, 0xFFFFFFFF}, //Still Threshold
        {MDDI_MEMA_13, 0x19020814, 0xFFFFFFFF}, 
        {MDDI_MEMA_17, 0x00000000, 0x0F000000}, //MF_ML_C_TH :Chroma offset to motion level. 
        //SB Parameter
        {MDDI_SB_00, 0x111BD00D, 0xFFFFFFFF},
        {MDDI_SB_04, 0x10040000, 0x100F0000}, // turn off B/D adaptive weighting        
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //1080i
        //MEMA Parameter
        {MDDI_MEMA_00, 0x92037AAD, 0x80FFFFFF}, 
        {MDDI_MEMA_01, 0xF08C8C6D, 0xFFFFFFFF},
        {MDDI_MEMA_02, 0x20206240, 0xFFFFFFFF}, 
        {MDDI_MEMA_04, 0x0000B000, 0x0000FF00},
        {MDDI_MEMA_09, 0x00000018, 0x000000FF}, 
        {MDDI_MEMA_0D, 0x00000C00, 0x0000FFFF}, 
        {MDDI_MEMA_12, 0x060A0000, 0xFFFFFFFF},
        {MDDI_MEMA_13, 0x19020814, 0xFFFFFFFF},  
        {MDDI_MEMA_17, 0x04000000, 0x0F000000},
        //SB Parameter
        {MDDI_SB_00, 0x091BD00D, 0xFFFFFFFF},                 
        {MDDI_SB_04, 0x10048080, 0x100F0000}, // Force CS weighting =0 &  turn off B/D adaptive weighting
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //SCEAM for Fluke multiburst, color unstable problem
        //MEMA Parameter
        {MDDI_MEMA_00, 0x80037AAF, 0x80FFFFFF}, //Enable MAMF
        {MDDI_MEMA_01, 0x30868C6D, 0xFFFFFFFF},
        {MDDI_MEMA_02, 0xE0206240, 0xFFFFFFFF}, // edge adaptive motion threshold        
        {MDDI_MEMA_04, 0x00002000, 0x0000FF00},
        {MDDI_MEMA_09, 0x0000000F, 0x000000FF}, //MD_VER_MOTION_TH
        {MDDI_MEMA_0D, 0x00000500, 0x0000FFFF}, //MBURST_VAR_TH
        {MDDI_MEMA_12, 0x06150000, 0xFFFFFFFF}, //Still Threshold
        {MDDI_MEMA_13, 0x19020864, 0xFFFFFFFF}, 
        {MDDI_MEMA_17, 0x00000000, 0x0F000000}, //MF_ML_C_TH :Chroma offset to motion level. 
        //SB Parameter
        {MDDI_SB_00, 0x111BD00D, 0xFFFFFFFF},
        {MDDI_SB_04, 0x10040000, 0x100F0000}, // turn off B/D adaptive weighting        
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};

const MDDI_REGTBL_T CODE MDDI_MEMA_UI_PARAM[E_DI_QUALITY_NUM_MODE][1] = 
{
    {   //AGGRESIVE
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //DEFENSIVE
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};

const MDDI_REGTBL_T CODE MDDI_EP_INIT[] =
{
    {MDDI_REGTBL_END, 0x00000000, 0x00000000}
};

const MDDI_REGTBL_T CODE MDDI_EP_PARAM[3][17] =
{
    //RF
    {    //BE Parameter    
        {MDDI_BE_09, 0xCA400001, 0xFFFFFFFF},
        {MDDI_BE_1B, 0x00020020, 0xFFFFFFFF},
        //DAR Parameter
        {MDDI_DAR_00, 0xF0802000, 0xFFFFFFFF},
        {MDDI_DAR_02, 0x900AC014, 0xFFFFFFFF},
        {MDDI_DAR_03, 0x29900210, 0xFFFFFFFF},
        {MDDI_DAR_04, 0x66300210, 0xFFFFFFFF},
        {MDDI_DAR_05, 0x3DE84210, 0xFFFFFFFF},
        {MDDI_DAR_07, 0x00010A2F, 0xFFFFFFFF},
        {MDDI_DAR_09, 0x2A084000, 0xFFFFFFFF},
        {MDDI_DAR_0A, 0x22084210, 0xFFFFFFFF},
        {MDDI_DAR_0C, 0xCDF00005, 0xFFFFFFFF},
        {MDDI_DAR_10, 0x00ABC550, 0xFFFFFFFF},
        {MDDI_DAR_11, 0xF0FF8000, 0xFFFFFFFF},
        {MDDI_DAR_12, 0x0006F320, 0xFFFFFFFF},
        {MDDI_DAR_13, 0xE0FF8000, 0xFFFFFFFF},
        {MDDI_DAR_16, 0x092AF412, 0xFFFFFFFF},        
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    //SD
    {    //BE Parameter    
        {MDDI_BE_09, 0xCA400001, 0xFFFFFFFF},
        {MDDI_BE_1B, 0x00020020, 0xFFFFFFFF},
        //DAR Parameter
        {MDDI_DAR_00, 0xF0802000, 0xFFFFFFFF},
        {MDDI_DAR_02, 0x900AC014, 0xFFFFFFFF},
        {MDDI_DAR_03, 0x29900210, 0xFFFFFFFF},
        {MDDI_DAR_04, 0x66300210, 0xFFFFFFFF},
        {MDDI_DAR_05, 0x3DE84210, 0xFFFFFFFF},
        {MDDI_DAR_07, 0x00010A2F, 0xFFFFFFFF},
        //{MDDI_DAR_09, 0x14F02C00, 0xFFFFFFFF},
        {MDDI_DAR_09, 0x14F02E00, 0xFFFFFFFF}, //DAR_V_VARIATION_O2 [9:5]: 0->10
        //{MDDI_DAR_0A, 0x22002860, 0xFFFFFFFF},
        {MDDI_DAR_0A, 0x2200287A, 0xFFFFFFFF}, //DAR_V_DIVERSITY_O3 [5:0]: 0->1A
        {MDDI_DAR_0C, 0xCDF00005, 0xFFFFFFFF},
        {MDDI_DAR_10, 0x00ABC550, 0xFFFFFFFF},
        {MDDI_DAR_11, 0xF0FF8000, 0xFFFFFFFF},
        {MDDI_DAR_12, 0x0006F320, 0xFFFFFFFF},
        {MDDI_DAR_13, 0xE0FF8000, 0xFFFFFFFF},
        {MDDI_DAR_16, 0x092AF412, 0xFFFFFFFF},        
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    //HD
    {   //BE Parameter
        {MDDI_BE_09, 0xCA500001, 0xFFFFFFFF},
        {MDDI_BE_1B, 0x00000020, 0xFFFFFFFF},        
        //DAR Parameter
        {MDDI_DAR_00, 0xD0802000, 0xFFFFFFFF},
        {MDDI_DAR_02, 0x900AC014, 0xFFFFFFFF},
        {MDDI_DAR_03, 0x0C5043FF, 0xFFFFFFFF},
        {MDDI_DAR_04, 0x03F003FF, 0xFFFFFFFF},
        {MDDI_DAR_05, 0x3DE84210, 0xFFFFFFFF},
        {MDDI_DAR_07, 0x00010040, 0xFFFFFFFF},
        {MDDI_DAR_09, 0x14FF8400, 0xFFFFFFFF},
        {MDDI_DAR_0A, 0x22084000, 0xFFFFFFFF},
        {MDDI_DAR_0C, 0xCD000000, 0xFFFFFFFF},
        {MDDI_DAR_10, 0x00ABC080, 0xFFFFFFFF},
        {MDDI_DAR_11, 0xE0FF8000, 0xFFFFFFFF},
        {MDDI_DAR_12, 0x0D567B20, 0xFFFFFFFF},
        {MDDI_DAR_13, 0x40FF8000, 0xFFFFFFFF},
        {MDDI_DAR_16, 0x092AF412, 0xFFFFFFFF},        
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};

const MDDI_REGTBL_T CODE MDDI_EP_UI_PARAM[E_DI_QUALITY_NUM_MODE][1] =
{
    {   //AGGRESIVE
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },

    {   //DEFENSIVE
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};

const MDDI_REGTBL_T CODE MDDI_PD_INIT[] =
{    
    { MDDI_SB_01     , 0x00C00000, 0x00C00000},//Disable CS When Film Detected
    { MDDI_SB_05     , 0x00000000, 0x00000020},//SB_FORCE_FILM_SEL_BF==1
    { MDDI_REGTBL_END, 0x00000000, 0x00000000}
};

const MDDI_REGTBL_T CODE MDDI_PD_PARAM[3][36] =
{    
    {   //480i
        {MDDI_FILM_00, 0xA3000003, 0xFB3A70F3},
        {MDDI_FILM_01, 0x80104400, 0xFBFF7FFF},
        {MDDI_FILM_02, 0x10003020, 0xFF0F3F3F},
        {MDDI_FILM_03, 0x00000028, 0x000000FF},
        {MDDI_FILM_04, 0x00000004, 0x000003FF},
        {MDDI_FILM_05, 0x00078000, 0x000FFFFF},
        {MDDI_FILM_06, 0x00000000, 0x03FF03FF},
        {MDDI_FILM_07, 0x00100200, 0xFFFFFFFF},
        {MDDI_FILM_08, 0x00140280, 0xFFFFFFFF},
        {MDDI_FILM_09, 0x00100FA0, 0xFFFFFFFF},
        {MDDI_FILM_0A, 0x000A0FA0, 0xFFFFFFFF},
        {MDDI_FILM_0B, 0x00050300, 0xFFFFFFFF},
        {MDDI_FILM_0C, 0x000600E0, 0xFFFFFFFF},
        {MDDI_FILM_0D, 0x00050280, 0xFFFFFFFF},
        {MDDI_FILM_0E, 0x00000040, 0xFFFFFFFF},
        {MDDI_FILM_13, 0x00003010, 0x0000FFFF},
        {MDDI_FILM_14, 0x00400000, 0x3FFFFF00},
        {MDDI_FILM_15, 0x00140004, 0x03FF03FF},
        {MDDI_FILM_16, 0x00000010, 0x000003FF},
        {MDDI_FILM_18, 0x00000200, 0x003FFFFF},
        {MDDI_FILM_23, 0x08080008, 0x38FF00FF},
        {MDDI_FILM_24, 0x20300010, 0x73FF83FF},
        {MDDI_FILM_25, 0x70008003, 0xF01FF1FF},
        {MDDI_FILM_26, 0x02000018, 0x0F0001FF},
        {MDDI_FILM_27, 0xE5050001, 0xFF1F0001},
        {MDDI_FILM_28, 0x00000080, 0x000000FF},
        {MDDI_FILM_29, 0x00000200, 0x0000FFFF},
        {MDDI_FILM_2A, 0x00100FA0, 0xFFFFFFFF},
        {MDDI_FILM_2B, 0x01001000, 0xFFFFFFFF},
        {MDDI_FILM_2C, 0x00E00300, 0xFFFFFFFF},
        {MDDI_FILM_2D, 0x00400280, 0xFFFFFFFF},
        {MDDI_FILM_3B, 0x00050300, 0xFFFFFFFF},
        {MDDI_FILM_3C, 0x00080100, 0xFFFFFFFF},
        {MDDI_FILM_3D, 0x000501F4, 0xFFFFFFFF},
        {MDDI_FILM_3E, 0x00000040, 0xFFFFFFFF},
        {MDDI_REGTBL_END, 0x00000000, 0x00000000},
    },
    {   //576i
        {MDDI_FILM_00, 0xA3000003, 0xFB3A70F3},
        {MDDI_FILM_01, 0x80104400, 0xFBFF7FFF},
        {MDDI_FILM_02, 0x06003010, 0xFF0F3F3F},
        {MDDI_FILM_03, 0x00000028, 0x000000FF},
        {MDDI_FILM_04, 0x00000004, 0x000003FF},
        {MDDI_FILM_05, 0x00078000, 0x000FFFFF},
        {MDDI_FILM_06, 0x00000000, 0x03FF03FF},
        {MDDI_FILM_07, 0x00100200, 0xFFFFFFFF},
        {MDDI_FILM_08, 0x00140280, 0xFFFFFFFF},
        {MDDI_FILM_09, 0x00100FA0, 0xFFFFFFFF},
        {MDDI_FILM_0A, 0x000A0FA0, 0xFFFFFFFF},
        {MDDI_FILM_0B, 0x00050300, 0xFFFFFFFF},
        {MDDI_FILM_0C, 0x000600E0, 0xFFFFFFFF},
        {MDDI_FILM_0D, 0x00050280, 0xFFFFFFFF},
        {MDDI_FILM_0E, 0x00000500, 0xFFFFFFFF},
        //{MDDI_FILM_0E, 0x00000040, 0xFFFFFFFF},
        //{MDDI_FILM_13, 0x00003010, 0x0000FFFF},
        {MDDI_FILM_13, 0x00003006, 0x0000FFFF},
        {MDDI_FILM_14, 0x00400000, 0x3FFFFF00},
        {MDDI_FILM_15, 0x00140004, 0x03FF03FF},
        {MDDI_FILM_16, 0x00000010, 0x000003FF},
        {MDDI_FILM_18, 0x00000200, 0x003FFFFF},
        {MDDI_FILM_23, 0x28040008, 0x38FF00FF},
        //{MDDI_FILM_23, 0x28080008, 0x38FF00FF},
        {MDDI_FILM_24, 0x20500030, 0x73FF83FF},
        //{MDDI_FILM_24, 0x20300010, 0x73FF83FF},
        {MDDI_FILM_25, 0x70010003, 0xF01FF1FF},
        {MDDI_FILM_26, 0x02000020, 0x0F0001FF},
        {MDDI_FILM_27, 0xE5050001, 0xFF1F0001},
        {MDDI_FILM_28, 0x00000080, 0x000000FF},
        {MDDI_FILM_29, 0x00000200, 0x0000FFFF},
        {MDDI_FILM_2A, 0x00100FA0, 0xFFFFFFFF},
        {MDDI_FILM_2B, 0x01001000, 0xFFFFFFFF},
        {MDDI_FILM_2C, 0x00E00300, 0xFFFFFFFF},
        {MDDI_FILM_2D, 0x00400280, 0xFFFFFFFF},
        {MDDI_FILM_3B, 0x00050300, 0xFFFFFFFF},
        {MDDI_FILM_3C, 0x00080100, 0xFFFFFFFF},
        {MDDI_FILM_3D, 0x000501F4, 0xFFFFFFFF},
        {MDDI_FILM_3E, 0x00000040, 0xFFFFFFFF},
        {MDDI_REGTBL_END, 0x00000000, 0x00000000},
    },
    {   //1080i
        {MDDI_FILM_00, 0xA3000003, 0xFB3A70F3},
        {MDDI_FILM_01, 0x80000400, 0xF8000C00},//32 Film Mask
        {MDDI_FILM_02, 0x10003010, 0x00FFFFFF},//Rate
        {MDDI_FILM_03, 0x00000030, 0x000000FF},//FR_MO_QUA_THRESH
        {MDDI_FILM_04, 0x00000002, 0x000003FF},//FR QUANT
        {MDDI_FILM_05, 0x000E8000, 0xFFFFFFFF},//Quan DN END
        {MDDI_FILM_06, 0x00000000, 0xFFFFFFFF},//Quan DN START
        {MDDI_FILM_07, 0x00400800, 0xFFFFFFFF},//FR_ST MIN MAX
        {MDDI_FILM_08, 0x00400800, 0xFFFFFFFF},//FR_MO MIN MAX
        {MDDI_FILM_09, 0x04008000, 0xFFFFFFFF},//FR_SI MIN MAX
        {MDDI_FILM_0A, 0x04008000, 0xFFFFFFFF},//FI_SIM MIN MAX
        {MDDI_FILM_0B, 0x00802000, 0xFFFFFFFF},//FI_ST_32 MIN MAX
        {MDDI_FILM_0C, 0x00000800, 0xFFFFFFFF},//ED_ST MIN MAX
        {MDDI_FILM_0D, 0x00801000, 0xFFFFFFFF},//FI_ST_22 MIN MAX
        {MDDI_FILM_0E, 0x00200200, 0xFFFFFFFF},//ED_ST_22 MIN MAX
        {MDDI_FILM_13, 0x00002010, 0x0000FFFF},//Saw Quant Thresh
        {MDDI_FILM_14, 0x02000000, 0x3FFFFF00},//SCENE_CHANGE_FI
        {MDDI_FILM_15, 0x00000002, 0x000003FF},//Saw Quant
        {MDDI_FILM_16, 0x00000010, 0x000003FF},
        {MDDI_FILM_18, 0x00000800, 0x008FFFFF},//SCENE_CHANGE_EDGE
        {MDDI_FILM_23, 0x08080008, 0x08FFFFFF},//Mixsaw ,FR_TH,Quantization Enable
        {MDDI_FILM_24, 0x20300010, 0x73FF83FF},
        {MDDI_FILM_25, 0x00040020, 0x001FF1FF},//DynamicBlock
        {MDDI_FILM_26, 0x0A000050, 0x0F0001FF},//DynamicBlock FIELD_ST_RATE
        {MDDI_FILM_27, 0x20000000, 0x20000000},//FIELD_ST_RATE
        {MDDI_FILM_28, 0x00000080, 0x000000FF},//Rate
        {MDDI_FILM_29, 0x00000800, 0x0000FFFF},//FI_ST_Q
        {MDDI_FILM_2A, 0x04008000, 0xFFFFFFFF},//FR_SIM_Q
        {MDDI_FILM_2B, 0x04008000, 0xFFFFFFFF},//FI_SIM_Q
        {MDDI_FILM_2C, 0x08002000, 0xFFFFFFFF},//ED_SAW_Q_22
        {MDDI_FILM_2D, 0x08002000, 0xFFFFFFFF},//ED_SAW_Q_32
        {MDDI_FILM_3B, 0x00802000, 0xFFFFFFFF},//FI_MO_32 MIN MAX
        {MDDI_FILM_3C, 0x00000300, 0xFFFFFFFF},//ED_MO MIN MAX
        {MDDI_FILM_3D, 0x00801000, 0xFFFFFFFF},//FI_MO_22 MIN MAX
        {MDDI_FILM_3E, 0x00000200, 0xFFFFFFFF},//ED_MO_22 MIN MAX
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};

const MDDI_REGTBL_T CODE MDDI_PD_UI_PARAM[E_DI_QUALITY_NUM_MODE][1] =
{
    {   //AGGRESIVE
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //DEFENSIVE
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};


const MDDI_REGTBL_T CODE MDDI_CS_INIT[] =
{
    {MDDI_CS_0B, 0x07133182, 0xFFFFFFFF},   //flfb_discard_ncn_large_ratio
    {MDDI_REGTBL_END, 0x00000000, 0x00000000}
};


const MDDI_REGTBL_T CODE MDDI_CS_PARAM[3][5] =
{
    {   //RF (ATV,DTV)
        //flfb_discard_ncn_large_ratio=1, FLFB_MOTION_MV_THD =0xF, FLFB_SB_NCN_CNT_THD = 0x30
        {MDDI_CS_0B, 0x0F303182, 0xFFFFFFFF},
        {MDDI_CS_0B, 0x07133182, 0xFFFFFFFF},
        {MDDI_CS_0D, 0x000A0A08, 0x00FFFFFF},
        {MDDI_CS_19, 0x06000000, 0xFF000000},   
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //PAL
        //flfb_discard_ncn_large_ratio=1, FLFB_MOTION_MV_THD =0x7, FLFB_SB_NCN_CNT_THD = 0x13
        {MDDI_CS_03, 0x79827102, 0xFFFFFFFF},
        {MDDI_CS_0B, 0x07133182, 0xFFFFFFFF},
        {MDDI_CS_0D, 0x00090C0A, 0x00FFFFFF},
        {MDDI_CS_19, 0x00000000, 0xFF000000}, 
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //NTSC
        //flfb_discard_ncn_large_ratio=1, FLFB_MOTION_MV_THD =0x7, FLFB_SB_NCN_CNT_THD = 0x13
        {MDDI_CS_03, 0xAA527102, 0xFFFFFFFF},
        {MDDI_CS_0B, 0x07133182, 0xFFFFFFFF},
        {MDDI_CS_0D, 0x000A0A08, 0x00FFFFFF},
        {MDDI_CS_19, 0x00000000, 0xFF000000}, 
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};

////////////////////////////////////////////////////////////////////////////////
// Basic On/Off Function
////////////////////////////////////////////////////////////////////////////////
static void _vDrvDI22FilmOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(bPath, MDDI_SB_01, (!u1OnOff), SB_FILM_22_DIS);
}

static void _vDrvDI32FilmOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(bPath, MDDI_SB_01, (!u1OnOff), SB_FILM_32_DIS);
}

static void _vDrvSetSBOnOff(UINT8 bPath, UINT8 u1OnOff)
{
	MDDI_WRITE_FLD(bPath, MDDI_SB_00, u1OnOff, SMART_BLENDING_EN);
}

static void _vDrvSetMEMAOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(bPath, MDDI_MEMA_00, u1OnOff, MEMA_EN);
}

static void _vDrvSetDAROnOff(UINT8 bPath, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(bPath, MDDI_DAR_00, u1OnOff, DAR_EN);
}

static void _vDrvFilmDefaultOnOff(UINT8 bPath)
{
#ifndef __MODEL_slt__
    switch (bDrvVideoGetTiming(bPath))
    {
        case MODE_625I:
        case MODE_625I_OVERSAMPLE:
        case MODE_1080i_48:
        case MODE_1080i_50:
            _vDrvDI22FilmOnOff(bPath, DiPQMode.bCfgFilmModeOnOff ? SV_ON : SV_OFF); // check UI status
            _vDrvDI32FilmOnOff(bPath, SV_OFF);
            break;
        default:
            _vDrvDI32FilmOnOff(bPath, DiPQMode.bCfgFilmModeOnOff ? SV_ON : SV_OFF);
            _vDrvDI22FilmOnOff(bPath, DiPQMode.bCfgFilmModeOnOff ? SV_ON : SV_OFF);
            break;
    }
#endif
}

UINT8 _u1DrvPScanQualityOnOff(UINT8 bPath) 
{
    if ((bPath != VDP_1) || 
        (!_arMDDiPrm[VDP_1].u1Interlace) ||
        (_arMDDiPrm[VDP_1].u1DataMode == E_DI_YCBOB_0W0R) ||
        (_arMDDiPrm[VDP_1].u1DataMode == E_DI_YCBOB_1W1R) ||
        (_arMDDiPrm[VDP_1].u1DataMode == E_DI_FLIP_MIRROR_YCBOB_1W1R) ||
        (_arMDDiPrm[VDP_1].u1DataMode == E_DI_UNKNOWN))
    {
        return SV_FALSE;
    }
    else
    {
        return SV_TRUE;
    }
}

/**
 * @brief Turn on/off film mode for UI
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param u1OnOff SV_ON/SV_OFF
 * @warning This should be invoked by TVD only
 */
void vDrvDIFilmModeOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    DiPQMode.bCfgFilmModeOnOff = u1OnOff;

    if (u1OnOff)
    {
        _vDrvFilmDefaultOnOff(bPath);
    }
    else
    {
        _vDrvDI32FilmOnOff(bPath, SV_OFF);
        _vDrvDI22FilmOnOff(bPath, SV_OFF);
    }
}

/**
 * @brief Return film mode status
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @return SV_TRUE/SV_FALSE
 */
UINT8 u1DrvGetFilmOnOff(UINT8 bPath)
{
    return DiPQMode.bCfgFilmModeOnOff;
}

/**
 * @brief Turn on/off CS in VSync
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param u1OnOff SV_ON/SV_OFF
 * @warning This should be invoked by TVD only
 */
void vDrvCSOnOffEx(UINT8 bPath, UINT8 u1OnOff)
{
#ifndef __MODEL_slt__
    UINT16 u2MainInputWidth = bDrvDIIsOversampleOver720(VDP_1) 
        ? (_arMDDiPrm[VDP_1].u2Width/2) : wDrvVideoInputWidth(bPath);

    if (bPath != VDP_1)
    {
        return;
    }
       
    if ( (bPath == VDP_1) && (u2MainInputWidth <= (CS_LIMIT_H_SIZE + DECODER_ADD_WIDTH))
            && (_arMDDiPrm[bPath].u1DataMode != E_DI_YCBOB_0W0R)
            && (_arMDDiPrm[bPath].u1DataMode != E_DI_YCBOB_1W1R)
            && (!_arMDDiPrm[bPath].u1PSCANDisplay) )
    {
        _arMDDiPrm[VDP_1].u1CsEnable = u1OnOff;          
    }
    else
    {
        _arMDDiPrm[VDP_1].u1CsEnable = SV_OFF;  
    }

    if (DiPQMode.bCfgCSOnOff != _arMDDiPrm[VDP_1].u1CsEnable)
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_CTRL_0B, E_DI_OUT_V_R, INT_TYPE);
    }
#endif
}


/**
 * @brief Turn on/off CS directly
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param u1OnOff SV_ON/SV_OFF
 * @warning This should be invoked by TVD only
 */
void vDrvCSOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    if (DiPQMode.bCfgCSOnOff != u1OnOff)
    {
        DiPQMode.bCfgCSOnOff = u1OnOff;

        LOG(6, "[%d]  CS turn %d\n", bPath, u1OnOff);

        MDDI_WRITE_FLD(bPath, MDDI_CS_00, u1OnOff, CS_EN);
        MDDI_WRITE_FLD(bPath, MDDI_CTRL_01, u1OnOff, SD_5_LINE);
        MDDI_WRITE_FLD(bPath, MDDI_CTRL_00, u1OnOff, EN_MVR);
        MDDI_WRITE_FLD(bPath, MDDI_CTRL_00, u1OnOff, EN_MVW);
        MDDI_WRITE_FLD(VDP_1, MDDI_CTRL_0B, E_DI_OUT_V_F, INT_TYPE);
    }
}

/**
 * @brief Turn on/off CS directly
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param u1OnOff SV_ON/SV_OFF
 * @warning This should be invoked by TVD only
 */
void vDrvCSForceOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    u1OnOff = (bPath == VDP_1) ? u1OnOff : SV_OFF;   
    
    DiPQMode.bCfgCSOnOff = u1OnOff;
    _arMDDiPrm[VDP_1].u1CsEnable = u1OnOff;

    LOG(6, "[%d]  CS force turn %d\n", bPath, u1OnOff);
    
    MDDI_WRITE_FLD(bPath, MDDI_CS_00, u1OnOff, CS_EN);
    MDDI_WRITE_FLD(bPath, MDDI_CTRL_01, u1OnOff, SD_5_LINE);
    MDDI_WRITE_FLD(bPath, MDDI_CTRL_00, u1OnOff, EN_MVR);
    MDDI_WRITE_FLD(bPath, MDDI_CTRL_00, u1OnOff, EN_MVW);
}


/**
 * @brief Turn on/off film mode Freeze Fro B2R Source
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param u1OnOff SV_ON/SV_OFF
 * @warning This should be invoked by TVD only
 */
void vDrvDIFilmModeFreeze(UINT8 bPath, UINT8 u1OnOff)
{
    if (DiPQMode.bCfgFilmModeOnOff&&u1OnOff)
    {
        _vDrvFilmDefaultOnOff(bPath);
    }
    else // When UI OFf, Always Off
    {
        _vDrvDI32FilmOnOff(bPath, SV_OFF);
        _vDrvDI22FilmOnOff(bPath, SV_OFF);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Initialization
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Initialize quality related register values
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
void vDrvDIInitQuality(UINT8 bPath)
{
    //initial PSCAN HW
    vDrvDILoadRegTbl(bPath, MDDI_PDMA_INIT);
    vDrvDILoadRegTbl(bPath, MDDI_MEMA_INIT);
    vDrvDILoadRegTbl(bPath, MDDI_EP_INIT);
    vDrvDILoadRegTbl(bPath, MDDI_PD_INIT);
    vDrvDILoadRegTbl(bPath, MDDI_CS_INIT);

    //initial FW
    DiPQMode.eMAQuality = E_DI_QUALITY_AGGRESSIVE;
    DiPQMode.eEdgePQuality = E_DI_QUALITY_AGGRESSIVE;
    DiPQMode.eFilmQuality = E_DI_QUALITY_AGGRESSIVE;

    //Mix mode FW
    vIO32WriteFldAlign(PSCAN_MIXMODE, 0x40, MIXMODE_TOP);
    vIO32WriteFldAlign(PSCAN_MIXMODE, 0xB0, MIXMODE_BOTTOM);

    //Text mode FW
    vIO32WriteFldAlign(PSCAN_TEXTMODE_00, 0x06, TEXTMODE_COUNT);
    vIO32WriteFldAlign(PSCAN_TEXTMODE_00, 0x10, TEXTMODE_RATIO);
    vIO32WriteFldAlign(PSCAN_TEXTMODE_00, 0x04, TEXTMODE_VAR);
    vIO32WriteFldAlign(PSCAN_TEXTMODE_01, 0x32, TEXTMODE_LIMITTOP);
    vIO32WriteFldAlign(PSCAN_TEXTMODE_01, 0xB4, TEXTMODE_LIMITBOTTOM);
    vIO32WriteFldAlign(PSCAN_TEXTMODE_01, 0x02, TEXTMODE_EXTLINE);

    //FW Film    
    vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x0d, FLD_MAX);
    vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x40, FLD_MIN);
    vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x28, FLD_GAIN1);
    vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x08, FLD_GAIN2);    

    vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MAX);
    vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MIN);
    vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x28, FRM_GAIN1);
    vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x06, FRM_GAIN2);

    vIO32WriteFldAlign(PSCAN_FWFILM_04, 0x10, HOMO_GAIN);

    vIO32WriteFldAlign(PSCAN_FWFILM_05, 0x20, BLK_STILL);
    vIO32WriteFldAlign(PSCAN_FWFILM_05, 0x40, BLK_MOTION);    

    //MEMA
    DiPar.MA.bHeterRate = 0x4;
    DiPar.MA.u4CZPAlias = 0x800;        

    //LineCS Status Initialize
    DiPar.CS.TextInfo.u2TotalCnt = 0;
    DiPar.CS.TextInfo.u2StartLine = 0;
    DiPar.CS.TextInfo.u2EndLine = _arMDDiPrm[VDP_1].u2Height>>1;
    DiPar.CS.TextInfo.u2ContiCnt = 0;
}


////////////////////////////////////////////////////////////////////////////////
// MA/EP/SD quality parameter related
////////////////////////////////////////////////////////////////////////////////
static void _vDrvDISetPDMAQuality(UINT8 bPath)
{
    vDrvDILoadRegTbl(bPath, MDDI_PDMA_PARAM);
}

static void _vDrvDISetMEMAQuality(UINT8 bPath)
{
    UINT16 u2InputWidth = MDDI_READ_FLD(bPath, MDDI_CTRL_02, HDEW);
    UINT16 u2InputHeight = MDDI_READ_FLD(bPath, MDDI_CTRL_02, VDEW);
    
    u2InputWidth = bDrvDIIsOversampleOver720(bPath) ? (u2InputWidth/2) : u2InputWidth;

    switch (bDrvVideoGetSourceTypeTiming(bPath))
    {
        case SOURCE_TYPE_TIMING_DIGI_1080I_50:
        case SOURCE_TYPE_TIMING_DIGI_1080I_60:
        case SOURCE_TYPE_TIMING_DTV_1080I_50:
        case SOURCE_TYPE_TIMING_DTV_1080I_60:
        case SOURCE_TYPE_TIMING_YPBPR_1080I_50:
        case SOURCE_TYPE_TIMING_YPBPR_1080I_60:
            vDrvDILoadRegTbl(bPath, MDDI_MEMA_PARAM[2]);
            break;
        case SOURCE_TYPE_TIMING_SECAM_RFTV:
        case SOURCE_TYPE_TIMING_SECAM_CVBS:
        case SOURCE_TYPE_TIMING_SECAM_SV:
        case SOURCE_TYPE_TIMING_SCART_AV_SECAM:
        case SOURCE_TYPE_TIMING_SCART_SV_SECAM:
            vDrvDILoadRegTbl(bPath, MDDI_MEMA_PARAM[3]);
            break;
        case SOURCE_TYPE_TIMING_RFTV_NTSC_358:
        case SOURCE_TYPE_TIMING_RFTV_NTSC_443:
        case SOURCE_TYPE_TIMING_RFTV_PAL:
        case SOURCE_TYPE_TIMING_RFTV_PAL_M:
        case SOURCE_TYPE_TIMING_RFTV_PAL_N:
        case SOURCE_TYPE_TIMING_RFTV_PAL_60:
        case SOURCE_TYPE_TIMING_DTV_SD_I_50:
        case SOURCE_TYPE_TIMING_DTV_SD_I_60:
            vDrvDILoadRegTbl(bPath, MDDI_MEMA_PARAM[0]);
            break;
        default:
            vDrvDILoadRegTbl(bPath, MDDI_MEMA_PARAM[1]);
            break;
    }

    if (DiPQMode.eMAQuality < E_DI_QUALITY_NUM_MODE)
    {
        vDrvDILoadRegTbl(bPath, MDDI_MEMA_UI_PARAM[DiPQMode.eMAQuality]);   
    }
    
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_MEMA_01, (u2InputWidth > MEMA_LRG_BLK_H_SIZE), RA_USE_LARGE_HOR_BLK);
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_MEMA_01, (u2InputHeight > MEMA_LRG_BLK_V_SIZE), RA_USE_LARGE_VER_BLK);

    //Enable chroma line average function(only sub)
    MDDI_WRITE_FLD(SV_VP_PIP, MDDI_SB_01, 0x1, SB_NR_PAL_C_FILTER);
}

static void _vDrvDISetEPQuality(UINT8 bPath)
{
    switch (bDrvVideoGetTiming(bPath))
    {
        case MODE_1080i_48:
        case MODE_1080i_50:
        case MODE_1080i:
            vDrvDILoadRegTbl(bPath, MDDI_EP_PARAM[2]);
            break;
        default:
            if ((bGetSignalType(bPath) == SV_ST_TV) 
                || (bGetSignalType(bPath) == SV_ST_MPEG))
            {
                    vDrvDILoadRegTbl(bPath, MDDI_EP_PARAM[0]);
            }
            else
            {
                    vDrvDILoadRegTbl(bPath, MDDI_EP_PARAM[1]);
            }            
            break;
    }
    
    if (DiPQMode.eEdgePQuality < E_DI_QUALITY_NUM_MODE)
    {
        vDrvDILoadRegTbl(bPath, MDDI_EP_UI_PARAM[DiPQMode.eEdgePQuality]);    
    }
}

static void _vDrvDISetPDQuality(UINT8 bPath)
{
    switch (bDrvVideoGetTiming(bPath))
    {
        case MODE_1080i_48:
        case MODE_1080i_50:
        case MODE_1080i:
            vDrvDILoadRegTbl(bPath, MDDI_PD_PARAM[2]);
            break;
        case MODE_625I:
        case MODE_625I_OVERSAMPLE:
            vDrvDILoadRegTbl(bPath, MDDI_PD_PARAM[1]);
            break;
        default:
            vDrvDILoadRegTbl(bPath, MDDI_PD_PARAM[0]);
            break;
    }

    if (DiPQMode.eFilmQuality < E_DI_QUALITY_NUM_MODE)
    {
        vDrvDILoadRegTbl(bPath, MDDI_PD_UI_PARAM[DiPQMode.eFilmQuality]);   
    }
    
    _vDrvFilmDefaultOnOff(bPath);
}

static void _vDrvDISetCSQuality(UINT8 bPath)
{
    UINT8 bTimingType;

    bTimingType = bDrvVideoGetTiming(bPath);
    
    switch (bGetSignalType(bPath))
    {
        case SV_ST_TV:
        case SV_ST_MPEG:
            vDrvDILoadRegTbl(bPath, MDDI_CS_PARAM[0]);
            break;
        default:
            if((bTimingType == MODE_625I) || (bTimingType == MODE_625I_OVERSAMPLE))
            {
            vDrvDILoadRegTbl(bPath, MDDI_CS_PARAM[1]);
            }
            else
            {
                vDrvDILoadRegTbl(bPath, MDDI_CS_PARAM[2]);
            }
            break;
    }    
}

void vDrvDISetQuality(UINT8 bPath)
{
    if (bPath == VDP_1)
    {
        if (_u1DrvPScanQualityOnOff(bPath))
        {
            _vDrvSetMEMAOnOff(bPath, SV_ON);
            _vDrvSetDAROnOff(bPath, SV_ON);
            _vDrvSetSBOnOff(bPath, SV_ON);  
        }
        else
        {
            _vDrvSetMEMAOnOff(bPath, SV_OFF);
            _vDrvSetDAROnOff(bPath, SV_OFF);
            _vDrvSetSBOnOff(bPath, SV_OFF);
            return;
        }
        
        _vDrvDISetMEMAQuality(VDP_1);
        _vDrvDISetEPQuality(VDP_1);
        _vDrvDISetPDQuality(VDP_1);
        _vDrvDISetCSQuality(VDP_1);

        //Set Adaptive FW Register
        if(bGetSignalType(SV_VP_MAIN) == SV_ST_TV)
        {
            vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, 0x37F, ADAPTIVE_PSCAN);
        }
        else
        {
            vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, 0x1FF, ADAPTIVE_PSCAN);
        }
    }
    else
    {
        _vDrvDISetPDMAQuality(VDP_2);
    }
}

////////////////////////////////////////////////////////////////////////////////
// MA/EP/SD quality parameter related (Interrupt Service Routine)
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Set MA quality parameters (Interrupt Service Routine)
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
static void _vDrvDIGetPQStatusISR()
{
    UINT32 i;
    
    DiSta.u4IsrIndex = (DiSta.u4IsrIndex+1)&DILOOPMASK;

    //============= MA Status ====================
    DiSta.MA.u4HEdgeCnt = MDDI_READ_FLD(SV_VP_MAIN, MDDI_MEMA_1F, HOR_EDGE_CNT);
    DiSta.MA.u4VEdgeCnt = MDDI_READ_FLD(SV_VP_MAIN, MDDI_MEMA_1F, VER_EDGE_CNT);
    DiSta.MA.u4EdgeSum = DiSta.MA.u4VEdgeCnt + DiSta.MA.u4HEdgeCnt;
    DiSta.MA.u4Saw = MDDI_READ_FLD(SV_VP_MAIN, MDDI_MEMA_1C, FIELD_SAW_SUM);
    DiSta.MA.u1FMDType = MDDI_READ_FLD(SV_VP_MAIN, MDDI_MEMA_1C, FIELD_MD_TYPE);
    DiSta.MA.u4HeterUp = MDDI_READ_FLD(SV_VP_MAIN, MDDI_MEMA_18, PD_SUM_HETER_UP);
    DiSta.MA.u4HomoUp = MDDI_READ_FLD(SV_VP_MAIN, MDDI_MEMA_19, PD_SUM_HOMO_UP);
    DiSta.MA.u4HeterDn = MDDI_READ_FLD(SV_VP_MAIN, MDDI_MEMA_1A, PD_SUM_HETER_DN);
    DiSta.MA.u4HomoDn = MDDI_READ_FLD(SV_VP_MAIN, MDDI_MEMA_1B, PD_SUM_HOMO_DN);
    
#if defined(CC_DEBUG)
    DiSta.MA.u4AML = MDDI_READ_FLD(SV_VP_MAIN, MDDI_MEMA_1C, AVG_MOTION_LEVEL);
    DiSta.MA.u4ML = MDDI_READ_FLD(SV_VP_MAIN, MDDI_MEMA_1C, MOTION_LEVEL_VAR);
    DiSta.MA.u4CurFieldLumaAvg = MDDI_READ_FLD(SV_VP_MAIN, MDDI_MEMA_1D, LUMA_AVG_0);
    DiSta.MA.u4PreFieldLumaAvg = MDDI_READ_FLD(SV_VP_MAIN, MDDI_MEMA_1D, LUMA_AVG_1);
    DiSta.MA.u4HeterPre = DiSta.MA.u4Heter;
    DiSta.MA.u4HomoPre = DiSta.MA.u4Homo;
    DiSta.MA.u4Heter = DiSta.MA.u4HeterUp + DiSta.MA.u4HeterDn;
    DiSta.MA.u4Homo = DiSta.MA.u4HomoUp + DiSta.MA.u4HomoDn;

    //Motion Histogram
    for (i = 0; i < DI_MOTION_HIST_BIN; i++)
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CTRL_16, i, HISTOGRAM_SEL);
        DiSta.MA.u2MotionHist[i] = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STA_06, Y_MOTION_HISTOGRAM);
    }
#else
    DiSta.MA.u4Heter = DiSta.MA.u4HeterUp + DiSta.MA.u4HeterDn;
    DiSta.MA.u4Homo = DiSta.MA.u4HomoUp + DiSta.MA.u4HomoDn;    
#endif   

    //============= BE Status ====================
    DiSta.BE.u4AliasCnt = MDDI_READ_FLD(SV_VP_MAIN, MDDI_BE_19, ALIAS_CNT);
    DiSta.BE.u4EdgeSum = 0;
    DiSta.BE.u4EdgeHis[0] = u2RegRead2B(MDDI_BE_13);
    DiSta.BE.u4EdgeHis[1] = u2RegRead2B(MDDI_BE_13+2);
    
    for (i = 2; i < DI_BE_HIST_BIN; i++)
    {        
        DiSta.BE.u4EdgeHis[i] = u2RegRead2B(MDDI_BE_13+2*i);
        DiSta.BE.u4EdgeSum += DiSta.BE.u4EdgeHis[i];    
    }
    
#if defined(CC_DEBUG)    
    DiSta.BE.u4GridPos = MDDI_READ_FLD(SV_VP_MAIN, MDDI_BE_1A, GRID_POS);
    DiSta.BE.u4GMVC = MDDI_READ_FLD(SV_VP_MAIN, MDDI_BE_1A, GRID_POS_ORIG);
    DiSta.BE.u4GMVX = MDDI_READ_FLD(SV_VP_MAIN, MDDI_BE_1A, GMV);
#endif

    //============= CS Status ====================
    DiSta.CS.bAllBurst = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_12, PRE_WHOLE_MULTIBURST);
    DiSta.CS.bHIST_MAX_I2 = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_12, HIST_MAX_I2);
    DiSta.CS.bHIST_MAX_I1 = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_12, HIST_MAX_I1);

#if defined(CC_DEBUG)
    DiSta.CS.u4MVHistDiffSum = 0;
    DiSta.CS.bFALLBACK_CHANGE_CNT = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_10, FALLBACK_CHANGE_CNT);
    DiSta.CS.bFALLBACK_STATUS = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_10, FALLBACK_STATUS);
    DiSta.CS.bFALLBACK_STEP = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_10, FALLBACK_STEP);
    DiSta.CS.u4VSI_ACCUM = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_25, VSI_ACCUM);
    DiSta.CS.u4MC_WEI_Y_ACCUM = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_26, MC_WEI_Y_ACCUM);
    DiSta.CS.u4MV_RAW_INVALID_ACCUM = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_29, MV_RAW_INVALID_ACCUM);
    DiSta.CS.u4CS_HIST_GMVX = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_2A, CS_HIST_GMVX);
    DiSta.CS.u4SC_LUMA_DIFF_VALUE = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_2E, SC_LUMA_DIFF_VALUE);
    DiSta.CS.u4SC_SAD_VALUE = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_2F, SC_SAD_VALUE);
    DiSta.CS.u4MOTION_MV_CNT = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_30, MOTION_MV_CNT);
    DiSta.CS.u4SB_NONEDGE_CNT = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_31, SB_NONEDGE_CNT);
    DiSta.CS.u4SB_TEXTURE_CNT = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_32, SB_TEXTURE_CNT);
#endif
    
    for (i = 0; i < DI_CS_MV_HIST_BIN/2; i++)
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_10, i<<1, HIST_STA_RADDR_INI);
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_10, SV_ON, SET_HIST_STA_RADDR);
        
#if defined(CC_DEBUG)
        UINT32 u4MVHistPre = DiSta.CS.u4MVHist[i];        
        DiSta.CS.u4MVHist[i] = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_11, Fld(32, 0, AC_FULLDW));
        DiSta.CS.u2MVHistDiff[2*i] = DIFF((DiSta.CS.u4MVHist[i]&0x3FFF), u4MVHistPre&0x3FFF);
        DiSta.CS.u2MVHistDiff[2*i+1] = DIFF((DiSta.CS.u4MVHist[i]>>16), u4MVHistPre>>16);
        DiSta.CS.u4MVHistDiffSum = DiSta.CS.u4MVHistDiffSum + DiSta.CS.u2MVHistDiff[2*i] + DiSta.CS.u2MVHistDiff[2*i+1];
#else
        DiSta.CS.u4MVHist[i] = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_11, Fld(32, 0, AC_FULLDW));
#endif

        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_10, SV_OFF, SET_HIST_STA_RADDR); 
    }    

    if (DiSta.CS.bHIST_MAX_I1 < DI_CS_MV_HIST_BIN)
    {
        DiSta.CS.bHIST_MAX_I1_Value = ((DiSta.CS.bHIST_MAX_I1%2)             
            ? (DiSta.CS.u4MVHist[DiSta.CS.bHIST_MAX_I1/2]>>16)
            : (DiSta.CS.u4MVHist[DiSta.CS.bHIST_MAX_I1/2]))&0x3FFF;
    }

    if (DiSta.CS.bHIST_MAX_I2 < DI_CS_MV_HIST_BIN)
    {
        DiSta.CS.bHIST_MAX_I2_Value = ((DiSta.CS.bHIST_MAX_I2%2)             
            ? (DiSta.CS.u4MVHist[DiSta.CS.bHIST_MAX_I2/2]>>16)
            : (DiSta.CS.u4MVHist[DiSta.CS.bHIST_MAX_I2/2]))&0x3FFF;
    }

    //=========== LineCS Status ===================
    for (i = 0; i < DI_CS_LINE_BIN; i++)
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_27, i, TEXT_LINE_READ_SEL);
        DiSta.CS.u4LineValid[i] = MDDI_READ_FLD(SV_VP_MAIN, MDDI_CS_28, TEXT_LINE_PRE);
    }    

    //============= FILM Status ====================
    DiSta.PD.u4FrmMotion = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
    DiSta.PD.u4FldMotion = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_10, FD_32_SAW_FI_A_B_SAW_Q);
    DiSta.PD.u4EdgMotion = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
    DiSta.PD.bBlkMotion = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
    DiSta.PD.bCurFilmMode = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_0F, Fld(2, 30, AC_MSKB3));
}


/**
 * @brief Set MA quality parameters (Interrupt Service Routine)
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
#if 0
static void _vDrvDISetMAQualityISR()
{

}
#endif

/**
 * @brief Set EP quality parameters (Interrupt Service Routine)
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
#if 0
static void _vDrvDISetEPQualityISR()
{

}
#endif


/**
 * @brief Set SD quality parameters (Interrupt Service Routine)
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
static void _vDrvDISetPDQualityISR()
{
    UINT16 i, j;
    UINT32 u4CurFilmMaskFr32;
    UINT32 u4Timing = bDrvVideoGetTiming(VDP_1);   

#if MDDI_PD_SW_FW_FILM_PATCH //Patch for pulldown.trp
    
    DiSta.PD.bFWPatch32FrSeq = (DiSta.PD.bFWPatch32FrSeq<<1)|(DiSta.PD.u4FrmMotion==0);

    u4CurFilmMaskFr32 = (FW_FILM_FR_SEQ_32 >> ((FW_FILM_CB-1)-DiSta.PD.bFWPatchCB32))& FW_FILM_MASK;

    if ((DiSta.PD.bFWPatch32FrSeq & FW_FILM_MASK) == u4CurFilmMaskFr32)
    {           
        DiSta.PD.bFWPatchCB32 = (DiSta.PD.bFWPatchCB32 == (FW_FILM_CB-1)) ? 0 : DiSta.PD.bFWPatchCB32 + 1;
        DiSta.PD.bFWPatchFr32Cnt = MIN(DiSta.PD.bFWPatchFr32Cnt+1, MDDI_FILM_ACC_MAX_SEQ_CNT);  
    }
    else
    {
        DiSta.PD.bFWPatchCB32 = 0;
        DiSta.PD.bFWPatchFr32Cnt = 0;        
    }

    if (DiSta.PD.bFWPatchCB32 == 3)
    {
        if (DiSta.PD.u4EdgMotion == 0)
        {
            DiSta.PD.bFWPatchFi32Cnt = MIN(DiSta.PD.bFWPatchFi32Cnt+1, MDDI_FILM_ACC_MAX_SEQ_CNT);
        }
        else
        {
            DiSta.PD.bFWPatchFi32Cnt = 0;
        }
    }    
    
    if ((DiSta.PD.bFWPatchFi32Cnt > 0x4) && (DiSta.PD.bFWPatchFr32Cnt > 0x10))
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_FILM_23, SV_OFF, R_QUAN_EN);
    
        if ((u4Timing == MODE_1080i) || (u4Timing == MODE_1080i_50))
        { 
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_FILM_0C, 0x300, NEW_FI_ST_MAX_TH);    
        }
        else
        {
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_FILM_0C, 0x200, NEW_FI_ST_MAX_TH);
        }
    }
    else
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_FILM_23, SV_ON, R_QUAN_EN);

        if ((u4Timing == MODE_1080i) || (u4Timing == MODE_1080i_50))
        { 
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_FILM_0C, 0x800, NEW_FI_ST_MAX_TH); 
        }
        else
        {
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_FILM_0C, 0x0E0, NEW_FI_ST_MAX_TH);        
        }
    }
#endif

    //FW FILM DETECTION 
    UINT8 u1FldGain1 = IO32ReadFldAlign(PSCAN_FWFILM_00, FLD_GAIN1);  
    UINT8 u1FldGain2 = IO32ReadFldAlign(PSCAN_FWFILM_00, FLD_GAIN2);  
    UINT32 u4FldMin = IO32ReadFldAlign(PSCAN_FWFILM_00, FLD_MIN);                                                       
    UINT32 u4FldMax = IO32ReadFldAlign(PSCAN_FWFILM_00, FLD_MAX)<<8;
    
    UINT8 u1FrmGain1 = IO32ReadFldAlign(PSCAN_FWFILM_02, FRM_GAIN1);  
    UINT8 u1FrmGain2 = IO32ReadFldAlign(PSCAN_FWFILM_02, FRM_GAIN2);  
    UINT32 u4FrmMin = IO32ReadFldAlign(PSCAN_FWFILM_02, FRM_MIN);                                                       
    UINT32 u4FrmMax = IO32ReadFldAlign(PSCAN_FWFILM_02, FRM_MAX)<<8;     

    UINT8 u1MoBlkSt = IO32ReadFldAlign(PSCAN_FWFILM_05, BLK_STILL);  
    UINT8 u1MoBlkMo = IO32ReadFldAlign(PSCAN_FWFILM_05, BLK_MOTION);          
    
    UINT8 u1FldGain, u1FrmGain, u1FldWeave = 0, u1FrmWeave = 0;
    UINT16 u2FrmMaskCur, u2FldMaskCur, u2Mask;    
    UINT32 u4FrmSaw = DiSta.PD.u4FrmMotion;   
    UINT32 u4FldSaw = DiSta.PD.u4EdgMotion; 
    UINT32 u4FrmSeq, u4FrmSeqNew;
    UINT32 u4FldSeq, u4FldSeqNew;    
    
    //Field Sequence
    u4FldMin = (DiSta.PD.bBlkMotion < u1MoBlkSt) ? (u4FldMin>>2) : u4FldMin;
    u4FrmMin = (DiSta.PD.bBlkMotion < u1MoBlkSt) ? (u4FrmMin>>2) : u4FrmMin;       
    u4FldMax = (DiSta.PD.bBlkMotion < u1MoBlkMo) ? u4FldMax : (u4FldMax<<2);
    u4FrmMax = (DiSta.PD.bBlkMotion < u1MoBlkMo) ? u4FrmMax : (u4FrmMax<<2);       

    if (u4FldSaw > DiSta.PD.u4FldThld)                                                                                     
    {          
        DiSta.PD.u4FldSeq = (DiSta.PD.u4FldSeq<<1) | 1;   
        u1FldGain = u1FldGain2;
    }                                                                                                                         
    else                                                                                                                      
    {           
        DiSta.PD.u4FldSeq = (DiSta.PD.u4FldSeq<<1) | 0;    
        u1FldGain = u1FldGain1;
    }

    //Frame Sequence
    if (u4FrmSaw > DiSta.PD.u4FrmThld)                                                                                     
    {                    
        DiSta.PD.u4FrmSeq = (DiSta.PD.u4FrmSeq<<1) | 1;   
        u1FrmGain = u1FrmGain2;
    }                                                                                                                         
    else                                                                                                                      
    {                               
        DiSta.PD.u4FrmSeq = (DiSta.PD.u4FrmSeq<<1) | 0;  
        u1FrmGain = u1FrmGain1;
    } 

    u4FldSeq = DiSta.PD.u4FldSeq;                                                                                        
    u4FrmSeq = DiSta.PD.u4FrmSeq;        
    DiSta.PD.u4FldThld = MIN(MAX(u4FldMin, (u4FldSaw*u1FldGain)>>4), u4FldMax); 
    DiSta.PD.u4FrmThld = MIN(MAX(u4FrmMin, (u4FrmSaw*u1FrmGain)>>4), u4FrmMax);  
    
    //Field Mask matching                                                                                                           
    if (DiSta.PD.u4FldBit)                                                                                                     
    {                                                                                                                         
        DiSta.PD.bFldCB = (DiSta.PD.bFldCB == (DiSta.PD.u4FldBit-1)) ? 0 : (DiSta.PD.bFldCB+1);                                   
                                                                                                                              
        u2Mask = FILM_MASK(DiSta.PD.u4FldBit);                                                                             
        u2FldMaskCur = (DiSta.PD.u4FldMask>>(DiSta.PD.u4FldBit-DiSta.PD.bFldCB))&u2Mask;  
        u4FldSeq = u4FldSeq&u2Mask;
        
        if (DiSta.PD.bFldCnt > 0x10) 
        {
            u4FldSeqNew = u4FldSeq|u2FldMaskCur;
        }
        else
        {            
            u4FldSeqNew = u4FldSeq;   
        }
        
        if (u2FldMaskCur == u4FldSeqNew)   
        {                                 
            u1FldWeave = (u2FldMaskCur != u4FldSeq) ? 0 : 1;     
            DiSta.PD.bFldCnt = MIN(DiSta.PD.bFldCnt+1, MDDI_FILM_MAX_SEQ_CNT);   
        }                                                                                                                     
        else                                                                                                                  
        {                                                                                                                     
            DiSta.PD.bFldCB = 0;                                                                                               
            DiSta.PD.u4FldBit = 0;   
            DiSta.PD.u4FldMask = 0; 
            DiSta.PD.bFwBadEdit = 0;  
            DiSta.PD.bFldCnt = 0;            

            //LOG(5, "Field Mask %4x  Seq %4x %4x \n", u2FldMaskCur, u4FldSeq, u4FldSeqNew);
        }        
    }       

    //Frame Mask matching                                                                                                           
    if (DiSta.PD.u4FrmBit)                                                                                                     
    {                                                                                                                         
        DiSta.PD.bFrmCB = (DiSta.PD.bFrmCB == (DiSta.PD.u4FrmBit-1)) ? 0 : (DiSta.PD.bFrmCB+1);                                   
                                                                                                                              
        u2Mask = FILM_MASK(DiSta.PD.u4FrmBit);                                                                             
        u2FrmMaskCur = (DiSta.PD.u4FrmMask>>(DiSta.PD.u4FrmBit-DiSta.PD.bFrmCB))&u2Mask;        
        u4FrmSeq = u4FrmSeq&u2Mask;
        
        if (DiSta.PD.bFrmCnt > 0x10) 
        {
            u4FrmSeqNew = u4FrmSeq|u2FrmMaskCur;
        }
        else
        {
            u4FrmSeqNew = u4FrmSeq;
        }
        
        if (u2FrmMaskCur == u4FrmSeqNew)   
        {    
            u1FrmWeave = (u2FrmMaskCur != u4FrmSeq) ? 0 : 1;
            DiSta.PD.bFrmCnt = MIN(DiSta.PD.bFrmCnt+1, MDDI_FILM_MAX_SEQ_CNT); 
        }                                                                                                                     
        else                                                                                                                  
        {                                                                                                                     
            DiSta.PD.bFrmCB = 0;                                                                                               
            DiSta.PD.u4FrmBit = 0;  
            DiSta.PD.u4FrmMask = 0; 
            DiSta.PD.bFrmCnt = 0;            

            LOG(5, "Frame Mask %4x  Seq %4x %4x \n", u2FrmMaskCur, u4FrmSeq, u4FrmSeqNew);   
        }                                                                                                                     
    }      

    //Special case for TG35 vertical moving pattern or random moving pattern
    //Field saw detection is wrong under this condition    
    if (((DiSta.PD.u4FldBit == 4) || (DiSta.PD.bCurFilmMode != 0)) 
        && (DiSta.BE.u4EdgeSum == 0))
    {                
        u1FldWeave = 0;
        DiSta.PD.bUnknowCnt = MIN(DiSta.PD.bUnknowCnt+0x8, 0x80); 
        //LOG(6, "Unknown OK:  %2x   %2x   %2x  \n", DiSta.PD.u4FldBit, DiSta.PD.bCurFilmMode, DiSta.BE[DiSta.u4IsrIndex].u4EdgeSum);   
    }
    else
    {
        u1FldWeave = 1;
        DiSta.PD.bUnknowCnt = (DiSta.PD.bUnknowCnt) ? (DiSta.PD.bUnknowCnt-1) : 0;  
        //LOG(6, "Unknown OK:  %2x   %2x   %2x  \n", DiSta.PD.u4FldBit, DiSta.PD.bCurFilmMode, DiSta.BE[DiSta.u4IsrIndex].u4EdgeSum);   
    }
    
    //Search Field random mask                                                                                                      
    if (DiSta.PD.u4FldBit == 0)                                                                                                
    {                                                                                                                         
        for (i=0; i < FILMMASKNUM; i++)                                                                                           
        {                                                                                                                     
            u2Mask = FILM_MASK(u4MaskBit[i]);                                                                             
            u2FldMaskCur = u4FldSeq&u2Mask;                                                                            
                                                                                                                              
            if ((u2FldMaskCur != 0) && (u2FldMaskCur != u2Mask) &&                                                      
                (u2FldMaskCur == ((u4FldSeq>>u4MaskBit[i])&u2Mask)))                                                   
            {                                                                                                                 
                for (j=0; j <= u4MaskBit[i]; j++)                                                                             
                {                                                                                                             
                    if (((u4FldSeq>>j) & 0x3) == 0x3)                                                                       
                    {                                                                                                         
                        DiSta.PD.bFwBadEdit = 1;                                                                              
                        break;                                                                                                
                    }                                                                                                         
                }                                                                                                             
                                                                                                                              
                DiSta.PD.u4FldMask = u4FldSeq&FILM_MASK(2*u4MaskBit[i]);
                DiSta.PD.u4FldBit = u4MaskBit[i];                                                                              
                break;                                                                                                        
            }                                                                                                                 
        }                                                                                                          
    } 

    //Search Frame random mask
    if (DiSta.PD.u4FrmBit == 0)                                                                                                
    {                                                                                                                         
        for (i=0; i < FILMMASKNUM; i++)                                                                                           
        {                                                                                                                     
            u2Mask = FILM_MASK(u4MaskBit[i]);                                                                             
            u2FrmMaskCur = u4FrmSeq&u2Mask;                                                                            
                                                                                                                              
            if ((u2FrmMaskCur != 0) && (u2FrmMaskCur != u2Mask) &&                                                      
                (u2FrmMaskCur == ((u4FrmSeq>>u4MaskBit[i])&u2Mask)))                                                   
            {                                                                                                             
                DiSta.PD.u4FrmMask = u4FrmSeq&FILM_MASK(2*u4MaskBit[i]);
                DiSta.PD.u4FrmBit = u4MaskBit[i];                                                                              
                break;                                                                                                        
            }                                                                                                                 
        }                                                                                                           
    }     

    if ((DiSta.PD.u4FldBit || DiSta.PD.u4FrmBit) && (DiSta.PD.bUnknowCnt == 0))
    {
        DiSta.PD.bFwAccCnt = MIN(DiSta.PD.bFwAccCnt+1, MDDI_FILM_ACC_MAX_SEQ_CNT);    
    }
    else
    {
        DiSta.PD.bFwAccCnt = (DiSta.PD.bFwAccCnt >= 0x8) ? (DiSta.PD.bFwAccCnt-2) : 0;                                 
    }                                  

    //Weave field 
    if ((!DiSta.PD.bFwBadEdit) && u1FldWeave && ((DiSta.PD.u4FldBit == 4) || u1FrmWeave))
    {
        DiSta.PD.bFwWeave = 1;
        //MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_05, 1, SB_FIX_FILM_MODE);  
        //MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_05, 1, SB_FIX_FILM_SEL_BF);
        //MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_05, (DiSta.PD.u4FldSeq&1) ? 0 : 1, SB_FORCE_FILM_SEL_BF);  
    }
    else
    {
        DiSta.PD.bFwWeave = 0;
        //MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_05, 0, SB_FIX_FILM_MODE);  
        //MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_05, 0, SB_FIX_FILM_SEL_BF);
    }

#if 1   
    //Debug Ink
    vRegWriteFldMulti(MDDI_CTRL_15,
        P_Fld((DiSta.PD.bFwAccCnt > 8), LARGE_OSD) |
        P_Fld(DiSta.PD.u4FldBit ? 0x1A : 0x08, OSD_X_POS) |
        P_Fld(DiSta.PD.u4FrmBit ? 0x1A : 0x08, OSD_Y_POS));
#endif

}


/**
 * @brief Set CS quality parameters (Interrupt Service Routine)
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
static void _vDrvDISetCSQualityISR()
{
    UINT8 u1CsW;
    
    //a. In ATV/DTV source, it reduces the CS weighting.
    //b. Detect not really 22 film - TG35 moving pattern
    if ((bGetSignalType(SV_VP_MAIN) == SV_ST_TV)
        ||(bGetSignalType(SV_VP_MAIN) == SV_ST_MPEG))
    {
        u1CsW = (((DiSta.PD.bFwAccCnt >= 0x8) && (DiSta.PD.bUnknowCnt == 0)) ? 0 : 4);
    }
    else
    {
        u1CsW = (((DiSta.PD.bFwAccCnt >= 0x8) && (DiSta.PD.bUnknowCnt == 0)) ? 0 : 8);
    }

    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_1D, u1CsW, MC_WEI_Y_MAX);    
}


#if 0
static void _vDrvDIEventISR()
{
#if defined(CC_DEBUG)

    UINT32 u4Value;
    UINT32 i;

    for (i=0; i<DI_DEBUG_EVENT_NUM; ++i)
    {
        if (DiPar.Event[i].bOnOff)
        {
            u4Value=u4RegRead4B(DiPar.Event[i].u4Addr)&DiPar.Event[i].u4Mask;

            if ( (u4Value>DiPar.Event[i].u4Low)&&(u4Value<DiPar.Event[i].u4High))
            {
                vRegWrite4BMsk(DiPar.Event[i].u4DstAddr, DiPar.Event[i].u4DstValue,DiPar.Event[i].u4DstMask);
                DiPar.Event[i].fgTrue=SV_TRUE;
            }
            else
            {
                DiPar.Event[i].fgTrue=SV_FALSE;
            }
        }
    }
#endif
}
#endif

/**
 * @brief Set video quality parameters (Interrupt Service Routine)
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
void vDrvDISetQualityISR()
{  
    _vDrvDIGetPQStatusISR();

    if (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_QUALITY))
    {
        //_vDrvDISetMAQualityISR();
        //_vDrvDISetEPQualityISR();
        
        if (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_SWFILM))
        {
            //Get SWFilm 22/32 Info.
            _vDrvDISetPDQualityISR();
                
            //According SWFilm Info to disable CS
            _vDrvDISetCSQualityISR();
        }
        
        //_vDrvDIEventISR();
    }
}

////////////////////////////////////////////////////////////////////////////////
// MA Infomation Collector
////////////////////////////////////////////////////////////////////////////////

static void _vDrvGetTextInfo(TEXT_INFO* TextInfo_temp)
{
    UINT8 u1ValidLine, u1Conti = 0;
    UINT32 i, j, u4ValidLine, u4Acc = 0;
    UINT16 u2StartLine = 0, u2EndLine = 0, u2ContiCnt = 0;
    
    TextInfo_temp->u2StartLine = 0;
    TextInfo_temp->u2EndLine = 0;
    TextInfo_temp->u2TotalCnt = 0;
    TextInfo_temp->u2ContiCnt = 0;    
    
    for (i=(DiPar.CS.TextInfo.u2StartLine>>4); i < (_arMDDiPrm[VDP_1].u2Height>>5); i++)
    {
        u4ValidLine = (i < DI_CS_LINE_BIN) ? DiSta.CS.u4LineValid[i] : 0;
        
        for (j=0; j < 8; j++)
        {   
            u1ValidLine = (UINT8)((u4ValidLine>>(4*j))&0xA);
            
            if (u1ValidLine)
            {    
                u4Acc++;
                TextInfo_temp->u2TotalCnt = (u1ValidLine == 0xA) ? (TextInfo_temp->u2TotalCnt+2) : (TextInfo_temp->u2TotalCnt+1);
            }
            else
            {
                u4Acc = 0;
            }
            
            if (u4Acc == 2)
            {
                u2StartLine = i*16+2*j-4;
                u1Conti = 1;
            }

            if ((u4Acc == 0) && (u1Conti == 1))
            {
                u2EndLine = i*16+2*j+2;
                u2ContiCnt = u2EndLine-u2StartLine;
                u1Conti = 0;

                if (u2ContiCnt >= TextInfo_temp->u2ContiCnt)
                {
                    TextInfo_temp->u2StartLine = u2StartLine;
                    TextInfo_temp->u2EndLine = u2EndLine;
                    TextInfo_temp->u2ContiCnt = u2ContiCnt;
                }
            }
        }
    }    
}

static void _vDrvMEMAAdaptive(UINT8 bPath)
{
    UINT16 FSawSum_th = 0x490;
    UINT16 u2MdCntMin;
    
    //CZP Scene Handling
    if ((DiPar.MA.u4CZPAlias*3/2 < DiSta.BE.u4AliasCnt) && (_arMDDiPrm[bPath].u2Height <720))
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_MEMA_00, SV_OFF, SAWTOOTH_ADAPTIVE_EN);
    }
    else if ((DiPar.MA.u4CZPAlias > DiSta.BE.u4AliasCnt) && (_arMDDiPrm[bPath].u2Height <720))
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_MEMA_00, SV_ON, SAWTOOTH_ADAPTIVE_EN);
    }
    else
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_MEMA_00, SV_ON, SAWTOOTH_ADAPTIVE_EN);
    }

    //Overcome the chroma feathering in the color bar edge
    if (DiSta.MA.u4Saw > FSawSum_th)
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_MEMA_02, 0x20, Y_TO_C_TH);
    }
    else
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_MEMA_02, 0x0, Y_TO_C_TH);
    }    

    u2MdCntMin = MIN(DiSta.MA.u4HEdgeCnt, DiSta.MA.u4VEdgeCnt);

    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_MEMA_0A, (u2MdCntMin>>8), MD_TANGENT_CNT_TH);
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_MEMA_0A, (u2MdCntMin>>8), MD_NORMAL_CNT_TH);
}

static void _vDrvDARAdaptive(UINT8 bPath)
{
    if ((DiSta.CS.bAllBurst)
        || ((DiSta.CS.bHIST_MAX_I1 > 8) 
        && (DiSta.CS.bHIST_MAX_I1_Value > 0x4A0))
        || ((DiSta.MA.u1FMDType == 0x2) 
        && (DiSta.MA.u4HEdgeCnt > 0x5000))
        || (DiSta.PD.bCurFilmMode))
    {
        if ((bGetSignalType(SV_VP_MAIN) == SV_ST_TV) 
            || (bGetSignalType(SV_VP_MAIN) == SV_ST_MPEG))
        {
            _vDrvSetDAROnOff(bPath, SV_ON);
        }
        else
        {
            _vDrvSetDAROnOff(bPath, SV_OFF);
        }
        _vDrvSetSBOnOff(bPath, SV_OFF);
    }
    else if (DiSta.MA.u1FMDType == 0x2)        // Vertical Moving
    {
        _vDrvSetDAROnOff(bPath, SV_ON);
        _vDrvSetSBOnOff(bPath, SV_OFF);
    }
    else
    {
        _vDrvSetDAROnOff(bPath, SV_ON);
        _vDrvSetSBOnOff(bPath, SV_ON);
    }
}


static void _vDrvCSAdaptive(UINT8 bPath)
{
    UINT32 u4SBNcnCnt_th1 = 0x450;
    UINT32 u4SBNcnCnt_th2 = 0x2000;         
    UINT32 u4aSatSum_th = 0x3000;    
    
    //CSAdaptive_Allburst mode for Multiburst/V Sweep pattern
    if (DiSta.CS.bAllBurst)
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_0B, 0x1, FLFB_MOTION_MV_THD);
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_05, 0x0, MULTIBURST_LOW_WEIGHT);
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_06, 0x3f, MULTIBURST_HIGH_WEIGHT);
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_07, 0x15, VAR_COST_S);
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_07, 0x20, VAR_COST_M);
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_07, 0x30, VAR_COST_L);
    }
    else
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_0B, 0x7, FLFB_MOTION_MV_THD);
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_05, 0x8, MULTIBURST_LOW_WEIGHT);
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_06, 0x20, MULTIBURST_HIGH_WEIGHT);
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_07, 0x4, VAR_COST_S);
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_07, 0x8, VAR_COST_M);
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_07, 0x30, VAR_COST_L);
    }

    //Adjust SB_NCN_CNT_TH according to content meter
    u4aSatSumSta = RegReadFldAlign(SAT_HIST_1_0_MAIN + 12, SAT_HIST_0) +
                    RegReadFldAlign(SAT_HIST_1_0_MAIN + 12, SAT_HIST_1);
        
    if ((DiSta.MA.u4EdgeSum < u4SBNcnCnt_th1)
        &&(u4aSatSumSta < u4aSatSum_th)
        &&(DiSta.MA.u1FMDType == 0x2))
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_0B, 0x3, FLFB_SB_NCN_CNT_THD);
    }
    else if ((DiSta.MA.u4EdgeSum < u4SBNcnCnt_th2)
        &&(u4aSatSumSta < u4aSatSum_th)
        &&(DiSta.MA.u1FMDType == 0x2))
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_0B, 0x8, FLFB_SB_NCN_CNT_THD);
    }
    else
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_0B, 0x13, FLFB_SB_NCN_CNT_THD);
    }

    //Reduce the time to enter CS in CG pattern
    if (DiSta.BE.u4EdgeSum == 0)
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_0E, 0x2, FLFB_STATUS_UPSTEP);
    }
    else
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_0E, 0x1, FLFB_STATUS_UPSTEP);
    }
}


static void _vDrvFilmModeAdaptive(UINT8 bPath)
{
    static UINT8 bPreMode;
    
    if(bPreMode != DiSta.PD.bCurFilmMode)
    {
        bPreMode = DiSta.PD.bCurFilmMode;
#ifdef CC_DRV_SUPPORT_SMICRO
            extern void vSMicroSetFilmMode(SMICRO_FILM_MODE eMode);
        
            SMICRO_FILM_MODE eFilmMode;
            switch(bPreMode)
            {
                case 1:
                    eFilmMode = FILM_32;
                    break;
                case 2:
                    eFilmMode = FILM_22;
                    break;            
                default:
                    eFilmMode = FILM_OFF;
                    break;
        
            }
            vSMicroSetFilmMode(eFilmMode);
#endif 
    }

    if (DiPQMode.bCfgFilmModeOnOff == SV_ON)
    {
        //To overcome TG35 motion case
        //Vertical moving and rainbow random moving 
        if ((DiSta.PD.bUnknowCnt) 
            || (DiSta.BE.u4EdgeSum == 0)
            || (DiSta.MA.u4EdgeSum == 0))  
        {
            MDDI_WRITE_FLD(bPath, MDDI_SB_01, SV_OFF, SB_FILM_22_DIS_CS);
            MDDI_WRITE_FLD(bPath, MDDI_SB_01, SV_ON, SB_FILM_22_DIS);
        }
        else
        {
            MDDI_WRITE_FLD(bPath, MDDI_SB_01, SV_ON, SB_FILM_22_DIS_CS);
            MDDI_WRITE_FLD(bPath, MDDI_SB_01, SV_OFF, SB_FILM_22_DIS);
        }
    }
    else
    {
        MDDI_WRITE_FLD(bPath, MDDI_SB_01, SV_OFF, SB_FILM_22_DIS_CS);
        MDDI_WRITE_FLD(bPath, MDDI_SB_01,SV_ON, SB_FILM_22_DIS);
    }
}

static void _vDrvTextDecision(TEXT_INFO* pTextSta)
{    
    if (bGetSignalType(SV_VP_MAIN) == SV_ST_TV)
    {
        if (((pTextSta->u2StartLine > IO32ReadFldAlign(PSCAN_TEXTMODE_01, TEXTMODE_LIMITBOTTOM))
            || (pTextSta->u2EndLine < IO32ReadFldAlign(PSCAN_TEXTMODE_01, TEXTMODE_LIMITTOP)))
            && (pTextSta->u2ContiCnt > IO32ReadFldAlign(PSCAN_TEXTMODE_00, TEXTMODE_COUNT)) 
            && ((pTextSta->u2ContiCnt<<1) > pTextSta->u2TotalCnt))
        {
            DiPar.CS.u1TextMode = 1;
        } 
        else
        {
            DiPar.CS.u1TextMode = 0;
        }
    }
    else
    {
        if ((pTextSta->u2ContiCnt > IO32ReadFldAlign(PSCAN_TEXTMODE_00, TEXTMODE_COUNT)) 
            && (((pTextSta->u2ContiCnt<<4)/pTextSta->u2TotalCnt) > IO32ReadFldAlign(PSCAN_TEXTMODE_00, TEXTMODE_RATIO)))
        {
            DiPar.CS.u1TextMode = 1;
        } 
        else
        {
            DiPar.CS.u1TextMode = 0;
        }
    }  
}

static void _vDrvTextCSSetting(UINT8 u1IsEnter)
{
    UINT16 u2Top, u2Bottom;
    UINT8 u1ExtLine = IO32ReadFldAlign(PSCAN_TEXTMODE_01, TEXTMODE_EXTLINE);
    
    if (u1IsEnter)
    {
        u2Top = (DiPar.CS.TextInfo.u2StartLine < u1ExtLine) ? 0 : (DiPar.CS.TextInfo.u2StartLine-u1ExtLine);

        if (ABS((_arMDDiPrm[VDP_1].u2Height>>1)-DiPar.CS.TextInfo.u2EndLine) < u1ExtLine)
        {
            u2Bottom = 0;
        }
        else
        {
            u2Bottom = (_arMDDiPrm[VDP_1].u2Height>>1)-DiPar.CS.TextInfo.u2EndLine-u1ExtLine;
        }

        //Change CS Active Window
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_21, u2Top, HIST_TOP_CLIP);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_21, u2Bottom, HIST_BOTTOM_CLIP);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_1E, (u2Top/4), FLBK_BLK_NUM_UP);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_1E, (u2Bottom/4), FLBK_BLK_NUM_DOWN);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_0B, 0x0, FLFB_MOTION_MV_THD);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_0B, 0x1, FLFB_SB_NCN_CNT_THD);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_17, 0x0, SOFTFLBK_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_17, 0x1, FB_IF_ANY_EDGE);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_13, 0xFFFFFF, SCD_LUMA_DIFF_THD);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_14, 0xFFFFFFF, SCD_SAD_DIFF_THD);
    }
    else
    {
         //Change CS Active Window
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_21, 0, HIST_TOP_CLIP);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_21, 0, HIST_BOTTOM_CLIP);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_1E, 0, FLBK_BLK_NUM_UP);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_1E, 0, FLBK_BLK_NUM_DOWN);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_0B, 0x7, FLFB_MOTION_MV_THD);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_0B, 0x13, FLFB_SB_NCN_CNT_THD);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_17, 0x1, SOFTFLBK_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_17, 0x0, FB_IF_ANY_EDGE);        
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_13, 0xA8D00, SCD_LUMA_DIFF_THD);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_14, 0x278D00, SCD_SAD_DIFF_THD);
    }
}


static UINT8 _u1DrvTextSceneChange(TEXT_INFO* pTextSta)
{     
    UINT8 u1LineVar = IO32ReadFldAlign(PSCAN_TEXTMODE_00, TEXTMODE_VAR);

    if((DIFF(DiPar.CS.TextInfo.u2StartLine, pTextSta->u2StartLine) > u1LineVar) 
        || (DIFF(DiPar.CS.TextInfo.u2EndLine, pTextSta->u2EndLine) > u1LineVar) 
        || (DIFF(DiPar.CS.TextInfo.u2ContiCnt, pTextSta->u2ContiCnt) > u1LineVar))
    {
        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }    
}

static void _vDrvTextAdaptive(UINT8 bPath)
{  
    TEXT_INFO TextInfo_temp;

    _vDrvGetTextInfo(&TextInfo_temp);
    
    switch (DiPar.CS.LineCSSta)
    {
        case DETECTSTAGE:            
            _vDrvTextDecision(&TextInfo_temp);
            
            if (DiPar.CS.u1TextMode)
            {
                DiPar.CS.LineCSSta = TEXTSTAGE;
                DiPar.CS.TextInfo.u2TotalCnt = TextInfo_temp.u2TotalCnt;
                DiPar.CS.TextInfo.u2StartLine = TextInfo_temp.u2StartLine;
                DiPar.CS.TextInfo.u2EndLine = TextInfo_temp.u2EndLine;
                DiPar.CS.TextInfo.u2ContiCnt = TextInfo_temp.u2ContiCnt;
                _vDrvTextCSSetting(SV_ON);
            }
            break;
        case TEXTSTAGE:           
            if (_u1DrvTextSceneChange(&TextInfo_temp))
            {
                DiPar.CS.LineCSSta = DETECTSTAGE;
                DiPar.CS.TextInfo.u2TotalCnt = 0;
                DiPar.CS.TextInfo.u2StartLine = 0;
                DiPar.CS.TextInfo.u2EndLine = _arMDDiPrm[VDP_1].u2Height>>1;
                DiPar.CS.TextInfo.u2ContiCnt = 0;
                _vDrvTextCSSetting(SV_OFF);
            }
            break;
        default:
            DiPar.CS.LineCSSta = DETECTSTAGE;
            DiPar.CS.TextInfo.u2TotalCnt = 0;
            DiPar.CS.TextInfo.u2StartLine = 0;
            DiPar.CS.TextInfo.u2EndLine = _arMDDiPrm[VDP_1].u2Height>>1;
            DiPar.CS.TextInfo.u2ContiCnt = 0;
            _vDrvTextCSSetting(SV_OFF);
            break;
    }    
}


static void _vDrvMixModeAdaptive(UINT8 bPath)
{
    if ((DiPar.CS.u1TextMode == 1) 
        && (DiPar.CS.TextInfo.u2EndLine < IO32ReadFldAlign(PSCAN_MIXMODE, MIXMODE_TOP) 
        || DiPar.CS.TextInfo.u2StartLine > IO32ReadFldAlign(PSCAN_MIXMODE, MIXMODE_BOTTOM)) 
        && (DiSta.PD.bFwAccCnt >= 0x8)) 
    {
        DiPar.CS.u1MixMode = 1;
    }
    else
    {
        DiPar.CS.u1MixMode = 0;
    }

    if (DiPar.CS.u1MixMode)
    {
        // Moving text on the top    
        if (DiPar.CS.TextInfo.u2EndLine < IO32ReadFldAlign(PSCAN_MIXMODE, MIXMODE_TOP))
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_FILM_16, (DiPar.CS.TextInfo.u2EndLine+4)<<1, UP_BOUND); 
            MDDI_WRITE_FLD(VDP_1, MDDI_FILM_01, 0x10, DOWN_BOUND); 
            MDDI_WRITE_FLD(VDP_1, MDDI_SB_0E, DiPar.CS.TextInfo.u2EndLine+4, SB_FILM_BND_UP); 
            MDDI_WRITE_FLD(VDP_1, MDDI_SB_0E, 0x3FF, SB_FILM_BND_DN);         
        }
        else
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_FILM_16, 0x10, UP_BOUND); 
            MDDI_WRITE_FLD(VDP_1, MDDI_FILM_01, (240-DiPar.CS.TextInfo.u2StartLine+4)<<1, DOWN_BOUND);
            MDDI_WRITE_FLD(VDP_1, MDDI_SB_0E, 0x000, SB_FILM_BND_UP); 
            MDDI_WRITE_FLD(VDP_1, MDDI_SB_0E, DiPar.CS.TextInfo.u2StartLine-4, SB_FILM_BND_DN);         
        }    
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_FILM_16, 0x10, UP_BOUND); 
        MDDI_WRITE_FLD(VDP_1, MDDI_FILM_01, 0x10, DOWN_BOUND);         
        MDDI_WRITE_FLD(VDP_1, MDDI_SB_0E, 0x000, SB_FILM_BND_UP); 
        MDDI_WRITE_FLD(VDP_1, MDDI_SB_0E, 0x3FF, SB_FILM_BND_DN);  
    }    
}

////////////////////////////////////////////////////////////////////////////////
// MDDi quality main loop
////////////////////////////////////////////////////////////////////////////////
void vDrvDIQtyMainLoop(UINT8 bPath)
{
    if (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_QUALITY) 
        && (_u1DrvPScanQualityOnOff(bPath)))
    {
        _vDrvFilmModeAdaptive(VDP_1);

        if (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_MA))
        {
            _vDrvMEMAAdaptive(VDP_1);
        }
        
        if (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_DAR))
        {
            _vDrvDARAdaptive(VDP_1);
        }
        
        if (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_CS))
        {
            _vDrvCSAdaptive(VDP_1);
        }

        if (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_TEXTMMODE))
        {
            _vDrvTextAdaptive(VDP_1);
        }

        if (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_MIXMMODE))
        {
            _vDrvMixModeAdaptive(VDP_1);
        }
    }
}


/**
* @brief Set DI Quality Mode
* @param eDIEngineType E_DI_ENGINE_MA/E_DI_ENGINE_EDGEP/E_DI_ENGINE_FILM
* @param eDIQualityMode E_DI_QUALITY_AGGRESSIVE / E_DI_QUALITY_DEFENSIVE
*/
void vDrvDISetQualityMode(E_DI_ENGINE_TYPE eDIEngineType,E_DI_QUALITY_MODE eDIQualityMode)
{
    switch (eDIEngineType)
    {
        case E_DI_ENGINE_MA:
            if (eDIQualityMode != DiPQMode.eMAQuality)
            {
                DiPQMode.eMAQuality = eDIQualityMode;
                _vDrvDISetMEMAQuality(VDP_1);
                _vDrvDISetMEMAQuality(VDP_2);
            }
            break;
        case E_DI_ENGINE_EDGEP:
            if (eDIQualityMode != DiPQMode.eEdgePQuality)
            {
                DiPQMode.eEdgePQuality = eDIQualityMode;
                _vDrvDISetEPQuality(VDP_1);
                _vDrvDISetEPQuality(VDP_2);
            }
            break;
        case E_DI_ENGINE_FILM:
            if (eDIQualityMode != DiPQMode.eFilmQuality)
            {
                DiPQMode.eFilmQuality = eDIQualityMode;
                _vDrvDISetPDQuality(VDP_1);
                _vDrvDISetPDQuality(VDP_2);
            }
            break;
    }
    return;
}

////////////////////////////////////////////////////////////////////////////////
//Debug Utility
////////////////////////////////////////////////////////////////////////////////
void vDrvDISetInk(UINT8 bMode,UINT16 u2Type)
{
    LOG(3,"\nbMode: 0)Ink Off , 1)Final Motion , 2)MEMA , 3)BE , 4)CS , 5)DAR , 6)FILM");

    //Reset All ink Off
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_06, SV_OFF, SB_INK_ENABLE);
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_16, SV_OFF, MV_INK_EN);
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_16, SV_OFF, MV_VALID_INK_EN);
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_16, SV_OFF, VSI_INK_EN);
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CTRL_15, 0x10, OSD_Y_POS);
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CTRL_15, 0x10, OSD_X_POS);

    switch (bMode)
    {
        case 0:
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CTRL_15, SV_OFF, HIST_INK_ON);
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CTRL_15, SV_OFF, OSD_MODE);
            break;

        case 1: //Motion Ink 
            LOG(1, "\n0)final_motion_level_y");
            LOG(1, "\n1)final_motion_level_c");
            LOG(1, "\n2)MEMA motion_level_y");
            LOG(1, "\n3)mc_wei_y");
            LOG(1, "\n4)mc_wei_c");

            u2Type = (u2Type==0) ? 12 : (u2Type==1) ? 13 : (u2Type==2) ? 8 : (u2Type==3) ? 14 : (u2Type==5) ? 14 : 12;

            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_06, SV_ON, SB_INK_ENABLE);
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_06, u2Type, SB_INK_SELECT);
            break;

        case 2: //MEME ink
            LOG(1, "0 : motion_level_y\n");
            LOG(1, "1 : motion_y\n");
            LOG(1, "2 : motion_level_c \n");
            LOG(1, "3 : motion_c\n");
            LOG(1, "4 : mvy\n");
            LOG(1, "5 : blk_motion_smooth\n");
            LOG(1, "6 : saw_tooth_found\n");
            LOG(1, "7 : mburst_found\n");
            LOG(1, "8 : possible_aliasing\n");
            LOG(1, "9 : pixel_sawtooth_found_2 \n");
            LOG(1, "10: pixel_sawtooth_found_1\n");
            LOG(1, "11: pixel_sawtooth_found_0\n");
            LOG(1, "12: edge_type[2:0]\n");
            LOG(1, "13: long_term_motion[1:0]\n");
            LOG(1, "14: pixel_saw_sum[2:0]\n");
            LOG(1, "15: flicker_found\n");
            LOG(1, "16: vburst_found\n");
            LOG(1, "17: region_motion_level[2:0]\n");
            LOG(1, "18: block_motion_level[2:0]\n");
            LOG(1, "19: cont_saw[2:0]\n");
            LOG(1, "20: long_term_motion_nxt_line[1:0]\n");
            LOG(1, "21: field_mv_type[1:0]\n");
            LOG(1, "22: motion_cost[2:0]\n");
            LOG(1, "23: ad_still_th[2:0]\n");
            LOG(1, "24: hor_thin_line\n");
            LOG(1, "25: motion_type[2:0]\n");
            LOG(1, "26: diffusion_flag[1:0]\n");

            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_06, SV_ON, SB_INK_ENABLE);
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_06, 0x9, SB_INK_SELECT);
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_MEMA_00, u2Type, MEMA_INK_SEL);
            break;

        case 3: //BE ink            
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_06, SV_ON, SB_INK_ENABLE);
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_06, 0xA, SB_INK_SELECT);
            break;

        case 4: //CS ink            
            LOG(1, "\nu2Type: 0)MV 1)VALID 2)VSI\n");
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CTRL_15, SV_ON, HIST_INK_ON);
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CTRL_15, 3, OSD_MODE);

            switch (u2Type)
            {
                case 0:
                    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_16, SV_ON, MV_INK_EN);
                    break;
                case 1:
                    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_16, SV_ON, MV_VALID_INK_EN);
                    break;
                case 2:
                    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CS_16, SV_ON, VSI_INK_EN);
                    break;
            }
            break;

        case 5: //DAR Ink
            break;
            
        case 6: //Film Ink 
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CTRL_15, 1, OSD_MODE);
            break;

        default:
            break;
    }
}


void vDrvDISetEvent(UINT8 bEventSel, UINT8 bParam, UINT32 u4Value)
{
#if defined(CC_DEBUG)

    if (bEventSel > DI_DEBUG_EVENT_NUM)
    {
        Printf("Event Handle Should Less Than %d", (int)DI_DEBUG_EVENT_NUM);
        return;
    }

    switch (bParam)
    {
        case 0:        //u4Addr
            Printf("u4SrcAddr=0x%x",(int)(u4Value&(~3)&0xFFFF)|0x20020000);
            DiPar.Event[bEventSel].u4Addr=u4Value&(~3)&0xFFFF;
            break;
        case 1:        //u4Mask
            Printf("u4Mask=0x%x",(int)u4Value);
            DiPar.Event[bEventSel].u4Mask=u4Value;
            break;
        case 2:        //u4Low
            Printf("u4Low=0x%x",(int)u4Value);
            DiPar.Event[bEventSel].u4Low=u4Value;
            break;
        case 3:        //u4High
            Printf("u4High=0x%x",(int)u4Value);
            DiPar.Event[bEventSel].u4High=u4Value;
            break;
        case 4:        //u4Value
            Printf("u4DstAddr=%x",(int)(u4Value&(~3)&0xFFFF)|0x20020000);
            DiPar.Event[bEventSel].u4DstAddr=u4Value&(~3)&0xFFFF;
            break;
        case 5:        //u4Value
            Printf("u4DstMask=0x%x",(int)u4Value);
            DiPar.Event[bEventSel].u4DstMask=u4Value;
            break;
        case 6:        //u4Value
            Printf("u4DstValue=0x%x",(int)u4Value);
            DiPar.Event[bEventSel].u4DstValue=u4Value;
            break;
        case 7:        //OnOff
            Printf("OnOff=0x%x",(int)u4Value);
            DiPar.Event[bEventSel].bOnOff=u4Value;
            break;            
        case 8:
            Printf("Copy Paramater From %d to %d",(int)u4Value,(int)bEventSel);
            DiPar.Event[bEventSel].u4Addr=DiPar.Event[u4Value].u4Addr;
            DiPar.Event[bEventSel].u4Mask=DiPar.Event[u4Value].u4Mask;
            DiPar.Event[bEventSel].u4Low=DiPar.Event[u4Value].u4Low;
            DiPar.Event[bEventSel].u4High=DiPar.Event[u4Value].u4High;
            DiPar.Event[bEventSel].u4DstAddr=DiPar.Event[u4Value].u4DstAddr;
            DiPar.Event[bEventSel].u4DstMask=DiPar.Event[u4Value].u4DstMask;
            DiPar.Event[bEventSel].u4DstValue=DiPar.Event[u4Value].u4DstValue;
            DiPar.Event[bEventSel].bOnOff = SV_OFF;
            break;
        default:
            Printf("\nFORMAT: bEventSel bParam u4Value");
            Printf("\nbEventSel: Handler Select (From 0 to %d",(int)DI_DEBUG_EVENT_NUM-1);
            Printf("\nbParam: 0)u4Addr 1)u4Mask 2)u4Low 3)u4High 4)u4DstAddr 5)u4DstMask 6)u4DstValue 7)OnOff 8)Copy All Param From Handler N");
            break;
    }
#endif
}

#if defined(CC_DEBUG)

EXTERN void vDrvWaitDIVSyncSema(void);

/*
 * @brief MDDi Film Mode Status polling loop
 * @param pvArgv Interrupt vector
 */
void vDrvDIFilmStatusPolling(void *pvArgv)
{
    INT32 i4Cnt;
    UINT32 pu4FilmModeFlag, pu4FilmFrameCB;
    UINT32 pu4FrameStaticSeq, pu4pFrameMotionSeq;
    UINT32 pu4FrameSimilarSeq, pu4FieldSimilarSeq;
    UINT32 pu4FieldStatic22Seq, pu4FieldStatic32Seq;
    UINT32 pu4FieldMotion22Seq, pu4FieldMotion32Seq;
    UINT32 pu4EdgeStatic22Seq, pu4EdgeStatic32Seq;
    UINT32 pu4EdgeMotion22Seq, pu4EdgeMotion32Seq;
    UINT32 pu4FrameMotion, pu4FieldMotion, pu4EdgeFieldMotion, pu4BlockMotion;

    Printf("PSCAN status polling thread START\n");
    Printf("22 32 2E 3E  FR_MO  FI_MO  ED_MO FR_ST FR_MO FR_SI 32_FI_ST 32_FI_MO 32_ED_ST 32_ED_MO FI_SI 22_FI_ST 22_FI_MO 22_ED_ST 22_ED_MO  BL CB\n");
    Printf("== == == ==  =====  =====  ===== ===== ===== ===== ======== ======== ======== ======== ===== ======== ======== ======== ========  ==== ==");
    while (fgDIStatusPolling)
    {
        vDrvWaitDIVSyncSema();

        pu4FilmModeFlag = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_0F, FD_FLAG);
        pu4FrameMotion = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
        pu4FieldMotion = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_10, FD_32_SAW_FI_A_B_SAW_Q);
        pu4EdgeFieldMotion = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);

        pu4FrameStaticSeq = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_08, FRAME_ST_SEQ_STATUS);
        pu4pFrameMotionSeq = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_09, FRAME_ST_SEQ_STATUS);
        pu4FrameSimilarSeq = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_0A, FRAME_SIM_SEQ_STATUS);

        pu4FieldStatic22Seq = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_0C, FIELD_ST_SEQ_22_STATUS);
        pu4FieldStatic32Seq = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_0C, FIELD_ST_SEQ_STATUS);
        pu4FieldMotion22Seq = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_29, FIELD_MO_SEQ_22_STATUS);
        pu4FieldMotion32Seq = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_27, FIELD_MO_SEQ_STATUS);

        pu4EdgeStatic22Seq = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_0E, NEW_FIELD_ST_SEQ_22_STATUS);
        pu4EdgeStatic32Seq = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_0E, NEW_FIELD_ST_SEQ_STATUS);
        pu4EdgeMotion22Seq = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_2A, NEW_FIELD_MO_SEQ_22_STATUS);
        pu4EdgeMotion32Seq = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_28, NEW_FIELD_MO_SEQ_STATUS);

        pu4FieldSimilarSeq = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_0D, FIELD_SIM_SEQ_STATUS);

        pu4BlockMotion = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
        pu4FilmFrameCB = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STATUS_21, FILM_CB_STATUS);

        Printf("\n%d  ", (pu4FilmModeFlag>>8) & 0x1); //53c[31] 22 detection
        Printf("%d  ", (pu4FilmModeFlag>>7) & 0x1); //53c[30] 32 detection        
        Printf("%d  ", (pu4FilmModeFlag>>0) & 0x7); //53c[25:23] 22 exit flag        
        Printf("%d  ", (pu4FilmModeFlag>>3) & 0x7); //53c[28:26] 32 exit flag
        Printf("%6x ", pu4FrameMotion & 0x3fffff); //53c[21:0] frame motion
        Printf("%6x ", pu4FieldMotion & 0x3fffff); //540[21:0] field saw
        Printf("%6x ", pu4EdgeFieldMotion & 0x3fffff); // Edge Field Motion
        
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameStaticSeq>>(i4Cnt - 1)) & 0x1); //520[4:0] frame static
        }
        Printf(" ");
        
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4pFrameMotionSeq>>(i4Cnt - 1)) & 0x1); //524[4:0] frame motion
        }
        Printf(" ");
        
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameSimilarSeq>>(i4Cnt - 1)) & 0x1); //528[4:0] frame similar
        }
        Printf("    ");

        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic32Seq>>(i4Cnt - 1)) & 0x1); //530[4:0] 32 Field Static
        }
        Printf("    ");
        
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion32Seq>>(i4Cnt - 1)) & 0x1); //530[4:0] 32 Field Motion
        }
        Printf("    ");
        
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic32Seq>>(i4Cnt - 1)) & 0x1); //538[21:17] 32 Edge field static
        }
        Printf("    ");
        
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion32Seq>>(i4Cnt - 1)) & 0x1); //538[21:17] 32 Edge field Motion
        }
        Printf(" ");
        
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldSimilarSeq>>(i4Cnt - 1)) & 0x1); //534[4:0] Field Similar
        }
        Printf("    ");
        
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic22Seq>>(i4Cnt - 1)) & 0x1); //530[21:17] 22 Field Static
        }
        Printf("    ");
        
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion22Seq>>(i4Cnt - 1)) & 0x1); //530[21:17] 22 Field Motion
        }
        Printf("    ");
        
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic22Seq>>(i4Cnt - 1)) & 0x1); //538[4:0] 22 Edge field static
        }
        Printf("    ");
        
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion22Seq>>(i4Cnt - 1)) & 0x1); //538[4:0] 22 Edge field Motion
        }
        Printf(" ");        
        Printf("%4X  ", (pu4BlockMotion) & 0xFFFF); //55c[8:0] Block Motion Sum       
        Printf("%d ", (pu4FilmFrameCB) & 0x7); //584[22:20] FILM_CB
    }

    Printf("\nPSCAN status polling thread END\n");
}


void vDrvDIMotionStatusPolling(void *pvArgv)
{
    UINT32 u4HistSel;;

    UNUSED(pvArgv);

    Printf("\nPSCAN Motion status polling thread START");
    Printf("\nN LOW                                                                        HIGH   MO_PXL L_MO_PXL");
    Printf("\n= =============================================================================== ======== ========");

    while (fgDIStatusPolling)
    {
        vDrvWaitDIVSyncSema();

        Printf("\n%1x ", DiSta.u4IsrIndex);
        for (u4HistSel = 0; u4HistSel < DI_MOTION_HIST_BIN ; u4HistSel++)
        {
            Printf("%4x ", DiSta.MA.u2MotionHist[u4HistSel]);
        }

        Printf("%8x %8x", DiSta.MA.u4MotionPxl, DiSta.MA.u4LargeMotionPxl);
    }

    Printf("PSCAN Motion status polling thread END\n");
}


void vDrvDICMotionStatusPolling(void *pvArgv)
{
    UINT32 u4HistSel, u4Motion;

    UNUSED(pvArgv);

    Printf("PSCAN C Motion status polling thread START\n");

    while (fgDIStatusPolling)
    {
        vDrvWaitDIVSyncSema();

        Printf("\n");

        for (u4HistSel = 0; u4HistSel < DI_MOTION_HIST_BIN ; u4HistSel++)
        {
            MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_CTRL_16, u4HistSel, HISTOGRAM_SEL);
            u4Motion = MDDI_READ_FLD(SV_VP_MAIN, MDDI_STA_06, C_MOTION_HISTOGRAM);
            Printf("%4x ", u4Motion);
        }
    }

    Printf("PSCAN C Motion status polling thread END\n");
}

void vDrvDIMEMAStatusPolling(void *pvArgv)
{
    UINT32 u4Ratio;
    
    UNUSED(pvArgv);

    Printf("PSCAN  MEMA status polling thread START\n");
    Printf("N HETER    HOMO      RATIO  DIFF_HE  DIFF_HO      SAW    Hedge    YEdge    MLv AvgMLv S  C S  C LU\n");
    Printf("= ======== ======== ====== ======== ======== ======== ======== ======== ====== ====== = == = == ==\n");

    while (fgDIStatusPolling)
    {
        vDrvWaitDIVSyncSema(); 
        
        u4Ratio = (DiSta.MA.u4Homo>>8) ? (DiSta.MA.u4Heter/(DiSta.MA.u4Homo>>8)) : 0xFFFFFFFF;
        
        Printf("\n%1x %8x %8x %6x %8x %8x %8x %8x %8x %6x %6x %2x",
               DiSta.u4IsrIndex,
               DiSta.MA.u4Heter,
               DiSta.MA.u4Homo,
               u4Ratio,
               DIFF(DiSta.MA.u4Heter, DiSta.MA.u4HeterPre),
               DIFF(DiSta.MA.u4Homo, DiSta.MA.u4HomoPre),              
               DiSta.MA.u4Saw,
               DiSta.MA.u4HEdgeCnt,
               DiSta.MA.u4VEdgeCnt,
               DiSta.MA.u4ML,
               DiSta.MA.u4AML,
               DiSta.MA.u4CurFieldLumaAvg);
    }

    Printf("\nPSCAN MEMA Motion status polling thread END");
}


void vDrvDIFWFILMStatusPolling(void *pvArgv)
{
    UNUSED(pvArgv);

    UINT8 i;
    DI_PQ_PD_STA *pPD_Sta = &DiSta.PD;

    Printf("PSCAN  FW FILM status polling thread START\n");
    Printf("= ====== ====== = == ====== ====== = == ====== ====== = ==\n");

    while (fgDIStatusPolling)
    {
        vDrvWaitDIVSyncSema();

        Printf("\n%1x  %2x    ", DiSta.u4IsrIndex, pPD_Sta->bBlkMotion);
        Printf("%5x %4x ", pPD_Sta->u4FrmMotion, pPD_Sta->u4FrmThld);

        for (i = 12; i > 0; i--)
        {
            Printf("%d", (pPD_Sta->u4FrmSeq>>(i - 1)) & 0x1);
        } 
        Printf("  ");
        Printf("%x %x %2x     ", pPD_Sta->u4FrmBit, pPD_Sta->bFrmCB, pPD_Sta->bFrmCnt);
        Printf("%5x %5x %4x ", pPD_Sta->u4FldMotion, pPD_Sta->u4EdgMotion, pPD_Sta->u4FldThld);

        for (i = 12; i > 0; i--)
        {
            Printf("%d", (pPD_Sta->u4FldSeq>>(i - 1)) & 0x1);
        } 
        Printf("    ");
        Printf("%x %x %2x     ", pPD_Sta->u4FldBit, pPD_Sta->bFldCB, pPD_Sta->bFldCnt);
        Printf("%x %x  %2x %2x     ", pPD_Sta->bFwBadEdit, pPD_Sta->bFwWeave, pPD_Sta->bUnknowCnt, pPD_Sta->bFwAccCnt);
        
        for (i = MAX(pPD_Sta->u4FrmBit, pPD_Sta->u4FldBit); i > 0; i--)
        {
            Printf("%d", (pPD_Sta->u4FrmMask>>(i - 1)) & 0x1);
        }  
        Printf("  ");
        
        for (i = MAX(pPD_Sta->u4FrmBit, pPD_Sta->u4FldBit); i > 0; i--)
        {
            Printf("%d", (pPD_Sta->u4FldMask>>(i - 1)) & 0x1);
        }           
    }

    Printf("\nPSCAN  FW FILM status polling thread END\n");
}

void vDrvDITextLineStatusPolling(void *pvArgv)
{
    UNUSED(pvArgv);

    Printf("PSCAN  CS status polling thread START\n");

    while (fgDIStatusPolling)
    {
        vDrvWaitDIVSyncSema();        
       
        Printf("LineCSSta %d [%3d - %3d] conti %3d  diff %3d  Total  %3d  Ratio  %2x  Text %d Mix %d\n",
            DiPar.CS.LineCSSta,
            DiPar.CS.TextInfo.u2StartLine,
            DiPar.CS.TextInfo.u2EndLine,
            DiPar.CS.TextInfo.u2ContiCnt,
            DiPar.CS.TextInfo.u2TotalCnt,        
            DiPar.CS.TextInfo.u2TotalCnt ? ((DiPar.CS.TextInfo.u2ContiCnt<<4)/DiPar.CS.TextInfo.u2TotalCnt) : 0,
            DiPar.CS.u1TextMode,
            DiPar.CS.u1MixMode);        
    }

    Printf("\nPSCAN  CS status polling thread END\n");
}

void vDrvDIMVStatusPolling(void *pvArgv)
{
    UINT32 i;
    UINT32 u4CurMVHist;

    UNUSED(pvArgv);

    Printf("\nPSCAN MV status polling thread START");
    Printf("\nN");
    Printf("\n= =============================================================================== ======== ========");

    while (fgDIStatusPolling)
    {
        vDrvWaitDIVSyncSema();
        
        Printf("\n%1x ", DiSta.u4IsrIndex);
        
        for (i = 0; i < 13 ; i++)
        {
            u4CurMVHist = DiSta.CS.u4MVHist[i/2];
            u4CurMVHist = ((i%2) ? (u4CurMVHist>>16) : u4CurMVHist)&0x3FFF;

            Printf("%6x ",(int)u4CurMVHist);
        }
    }

    Printf("PSCAN MV status polling thread END\n");
}

void vDrvDIMVDiffStatusPolling(void *pvArgv)
{
    UINT32 i;

    UNUSED(pvArgv);

    Printf("\nPSCAN MVDiff status polling thread START");
    Printf("\nN  DiffSum");
    Printf("\n= ======  ============================================================================");

    while (fgDIStatusPolling)
    {
        vDrvWaitDIVSyncSema();
         
        Printf("\n%1x %8x    ", DiSta.u4IsrIndex, DiSta.CS.u4MVHistDiffSum);

        for (i = 0; i < DI_CS_MV_HIST_BIN ; i++)
        {
            Printf("%4x ", DiSta.CS.u2MVHistDiff[i]);
        }
    }

    Printf("PSCAN MVDiff status polling thread END\n");
}

void vDrvDIBEPolling(void *pvArgv)
{
    UINT32 i;
    
    UNUSED(pvArgv);

    Printf("\nPSCAN BE status polling thread START");
    Printf("\nN   ALIAS2 GRID_POS GMVC GMVX    0    V   MH   PH   M1   P1   M2   P2   M4   P4   M7   P7");
    Printf("\n= ======== ======== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====");

    while (fgDIStatusPolling)
    {
        vDrvWaitDIVSyncSema();

        Printf("\n%1x %8x %8x %4d ", 
            DiSta.u4IsrIndex, 
            DiSta.BE.u4AliasCnt, 
            DiSta.BE.u4GridPos, 
            DiSta.BE.u4GMVC);

        if ((DiSta.BE.u4GMVX&0x10) != 0)
        {
            Printf("%-3d ", (((~DiSta.BE.u4GMVX)&0xF)+1));
        }
        else
        {
            Printf(" %3d ", (DiSta.BE.u4GMVX&0xF));
        }

        for (i = 0; i < DI_BE_HIST_BIN; i++)
        {
            Printf("%4x ", DiSta.BE.u4EdgeHis[i]);
        }
    }
    Printf("PSCAN BE status polling thread END\n");
}

void vDrvDIEventStatusPolling(void * pvArgv)
{
    UINT8 i;

    UNUSED(pvArgv);

    Printf("[RegValue True/False]");
    while (fgDIStatusPolling)
    {
        vDrvWaitDIVSyncSema();

        Printf("\n");
        for (i=0; i< DI_DEBUG_EVENT_NUM; ++i)
        {
            Printf("[0x%8x 0x%1x] ",(int)u4RegRead4B(DiPar.Event[i].u4Addr)&
                   (DiPar.Event[i].u4Mask),(int)DiPar.Event[i].fgTrue);
        }
    }

    Printf("\nPSCAN  Event status polling thread END\n");
}

void vDrvDIEventProfileStatusPolling(void *pvArgv)
{
    UINT8 bEventSel;

    for (bEventSel=0; bEventSel < DI_DEBUG_EVENT_NUM; ++bEventSel)
    {
        Printf("\n==========================");
        Printf("\nEvent Handler %d",(int)bEventSel);
        Printf("\nu4SrcAddr=0x%x",(int)(DiPar.Event[bEventSel].u4Addr|0x20020000));
        Printf("\nu4Mask=0x%x",(int)(DiPar.Event[bEventSel].u4Mask));
        Printf("\nu4Low=0x%x",(int)(DiPar.Event[bEventSel].u4Low));
        Printf("\nu4High=0x%x",(int)(DiPar.Event[bEventSel].u4High));
        Printf("\nu4DstAddr=0x%x",(int)(DiPar.Event[bEventSel].u4DstAddr|0x20020000));
        Printf("\nu4DstMask=0x%x",(int)(DiPar.Event[bEventSel].u4DstMask));
        Printf("\nu4DstValue=0x%x",(int)(DiPar.Event[bEventSel].u4DstValue));
        Printf("\nOnOff=0x%x\n",(int)(DiPar.Event[bEventSel].bOnOff));
    }
}

void vDrvDICSPolling(void *pvArgv)
{    
    UNUSED(pvArgv);

    Printf("\nPSCAN CS status polling thread START\n");
    Printf("N  FbSta  FbStep  HisMax1  HisMax1v  HisMax2  HisMax2v     VSI     INVALID  GMVX  ASatSum \n");
    Printf("=  =====  ======  =======  ========  =======  ========  =========  ======== ========\n");

    while (fgDIStatusPolling)
    {
        vDrvWaitDIVSyncSema();        
        
        Printf("%1x  %2x  %6x  %8x  %8x  %8x  %8x  %8x  %8x  %8x %8x\n", 
            DiSta.u4IsrIndex,
            DiSta.CS.bFALLBACK_STATUS,
            DiSta.CS.bFALLBACK_STEP,
            DiSta.CS.bHIST_MAX_I1,
            DiSta.CS.bHIST_MAX_I1_Value,
            DiSta.CS.bHIST_MAX_I2,
            DiSta.CS.bHIST_MAX_I2_Value,
            DiSta.CS.u4VSI_ACCUM,
            DiSta.CS.u4MV_RAW_INVALID_ACCUM,
            DiSta.CS.u4CS_HIST_GMVX,
            u4aSatSumSta);
    }
    Printf("PSCAN CS status polling thread END\n");
}

void vDrvDIAdaptivePolling(void *pvArgv)
{    
    UNUSED(pvArgv);

    UINT8 i;
    UINT32 u4HomoDiff, u4HomoMoTh, u4HomoThld, u4HomoSeq = 0; 

    Printf("  Homo    HomoDiff    Thld         Seq \n");
    Printf("========  ========  ========  ============\n");

    while (fgDIStatusPolling)
    {
        vDrvWaitDIVSyncSema();

        u4HomoDiff = DIFF(DiSta.MA.u4Homo, DiSta.MA.u4HomoPre);
        u4HomoMoTh = MAX(DiSta.MA.u4Homo, DiSta.MA.u4HomoPre); 
        u4HomoThld = (u4HomoMoTh*IO32ReadFldAlign(PSCAN_FWFILM_04, HOMO_GAIN))>>8; 
        u4HomoSeq = (u4HomoSeq<<1) | (u4HomoDiff > u4HomoThld); 
        
        Printf("%8x %8x %8x ", DiSta.MA.u4Homo, u4HomoDiff, u4HomoThld);

        for (i = 12; i > 0; i--)
        {
            Printf("%d", (u4HomoSeq>>(i - 1)) & 0x1);
        } 

        Printf(" \n");
    }

    Printf("PSCAN CS status polling thread END\n");
}
#endif

