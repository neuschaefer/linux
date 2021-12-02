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
 * $Author: shijiang.feng $
 * $Date: 2012/11/13 $
 * $RCSfile: drv_di_int.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////////////
#include "general.h"
#include "typedef.h"
#include "sv_const.h"
#include "vdo_misc.h"
#include "x_bim.h"
#include "di_debug.h"
#include "x_assert.h"
#include "drv_di.h"
#include "drv_di_int.h"
#include "drv_tdtv_drvif.h"
#include "hw_di_int.h"
#include "hw_di.h"
#include "video_timing.h"
#include "source_select.h"
#include "drv_meter.h"
#include "x_hal_5381.h"
#include "video_def.h"

//Disable CS when signal level lower than this threshold
extern UINT8 bTvdSignalStrengthLevel(UINT8 bMode);
#define CS_DISABLE_WEAK_SIGNAL            50       

typedef enum
{
    E_DI_FILM_NONE,
    E_DI_FILM_32,
    E_DI_FILM_22, 
    E_DI_FILM_2224,
    E_DI_FILM_2332,
    E_DI_FILM_32322,
    E_DI_FILM_55,
    E_DI_FILM_64,
    E_DI_FILM_87,
    E_DI_FILM_KALEIDOSCOPE,
} E_DI_FILM_MODE;

#define MDDI_PD_SW_FW_FILM_PATCH            1       //Patch for Some Scenario HW Can not detect
#define MDDI_PD_ADAPTIVE_THRESHOLD          1       //Adaptive Film Mode Seqence Threshold
#define MDDI_CS_FW_FILM_DISABLE_CS          1
#define MDDI_DAR_ADAPTIVE_VERTICAL_FILTER   1
#define CS_HD_WIDTH_LIMIT                   1792    //MV Dram bug, wait for ECO

#define DI_DEBUG_EVENT_NUM      6
#define FW_FILM_FR_SEQ_32       0x210   //b1000010000
#define FW_FILM_MASK            0x1F    //b11111
#define FW_FILM_CB              5

#define FW_FILM_FI_SEQ_2224     0x055    //b0001010101
#define FW_FILM_FI_SEQ_2332     0x095    //b0010010101
#define FW_FILM_FI_SEQ_64       0x104    //b0100000100

#define MDDI_FILM_ACC_MAX_SEQ_CNT    0x60
#define MDDI_FILM_MAX_SEQ_CNT        0xFF

#define FILM_MASK(bit)      ((1<<bit)-1)
#define MASK_NUM             6
UINT32 u4MaskBit[MASK_NUM] = {15, 12, 10, 6, 5, 4};

#define MV_HIST_IND(x) (((x)%2) ? (DiSta.CsSta.u4MVHist[(x)>>1]>>16) : (DiSta.CsSta.u4MVHist[(x)>>1]&0x3FFF))
#define MV_HIST(x, y)  (MV_HIST_IND(31*(4-(y))+(15-(x)))) 

//Status Filter
#define DILOOPNUM               8   
#define DILOOPMASK              (DILOOPNUM-1) 

#define DI_BE_HIST_BIN          12
#define DI_CS_LINE_BIN          22
#define DI_CS_MV_HIST_BIN       280

#define SET_USER_OSD(u4Value, u4Flag)   (MDDI_WRITE_FLD(VDP_1, MDDI_OC_0D, u4Value, u4Flag))
#define GET_USER_OSD(u4Flag)            (MDDI_READ_FLD(VDP_1, MDDI_OC_0D, u4Flag))  

UINT8 fgDIStatusPolling;
UINT8 u1YFixMotion[VDP_NS] = {0, 0};
UINT8 u1CFixMotion[VDP_NS] = {0, 0};
UINT8 u1YMotionLevel[VDP_NS] = {0, 0};
UINT8 u1CMotionLevel[VDP_NS] = {0, 0};

typedef struct
{
    UINT16 u2StartLine;
    UINT16 u2EndLine;
    UINT16 u2ContiCnt;
    UINT16 u2TotalCnt; 
} TEXT_INFO;

typedef struct
{
    UINT32 u4HeterUp;
    UINT32 u4HeterDn;
    UINT32 u4Heter;
    UINT32 u4HeterDiff;
    UINT32 u4HomoUp;
    UINT32 u4HomoDn;
    UINT32 u4Homo;
    UINT32 u4HomoDiff;
#ifndef NDEBUG
    UINT32 u4HEdgeCnt;
    UINT32 u4VEdgeCnt;
    UINT32 u4EdgeSum;    
#endif
    UINT32 u4AvgCurY;
    UINT32 u4AvgPreY;
    UINT32 u4AvgCurCb;
    UINT32 u4AvgPreCb;
    UINT32 u4AvgCurCr;
    UINT32 u4AvgPreCr;
} DI_IF_STA;

typedef struct
{
    UINT32 u4AliasCnt;
    UINT32 u4EdgeSum;
    UINT32 u4EdgeHis[DI_BE_HIST_BIN];
} DI_PE_STA;

typedef struct
{
    UINT8 u1GmvX; 
    UINT8 u1GmvY;     
    UINT16 u2HistMax;
    UINT16 u2HistCnt;  
    UINT16 u2EdgeCnt;    
    UINT32 u4FieldSawRank;
#ifndef NDEBUG
    UINT8 bAllBurst;     
    UINT8 u1SmtDeconf;
    UINT8 u1MvDeconf;
    UINT16 u2SmtCnt;
    UINT16 u2SmtCntROI;
    UINT32 u4SawAB;
    UINT32 u4SawBC;
    UINT32 u4LineValid[DI_CS_LINE_BIN];
#endif
    UINT32 u4MVHist[DI_CS_MV_HIST_BIN/2];       // 1 Index Contain 2 Hist To Save ISR Run Time
} DI_CS_STA;

typedef struct
{
    UINT8 bBlkMotion;
    UINT8 bCurFilmMode;     //Non-Film, 32_Film, 22_Film
    UINT32 u4FrmMotion;
    UINT32 u4EdgMotion;
    UINT32 u4FldMotion;
} DI_PD_STA;

typedef struct
{
    UINT8 bStableCnt;
    UINT8 bMovingCzp;
} DI_IF_PARAM;

typedef struct
{   
    UINT8 bCsOnOff;
    UINT8 bFilmCnt;    
} DI_CS_PARAM;

typedef struct
{    
} DI_DAR_PARAM;

typedef struct
{  
    UINT8 bFWPatch32FrSeq;  //Sequence to change 32 PD Detection
    UINT8 bFWPatchCB32;     //Current Combination of 32 PD Detection
    UINT8 bFWPatchFi32Cnt;  //Sequence to change 32 PD Detection
    UINT8 bFWPatchFr32Cnt;  //Sequence to change 32 PD Detection
    UINT8 bFldCnt;
    UINT8 bFldCB;
    UINT8 bFwAccCnt;    
    UINT8 bFwBadEdit;
    UINT8 bFwWeave;
    UINT8 bUnknowCnt;
    UINT8 bFrmCnt;
    UINT8 bFrmCB;
    UINT32 u4FrmSeq;
    UINT32 u4FrmThld;
    UINT32 u4FrmMask;
    UINT32 u4FrmBit;
    UINT32 u4FldSeq;    
    UINT32 u4FldThld;
    UINT32 u4FldMask;
    UINT32 u4FldBit;
    UINT32 u4AdaptiveFrMoTh;
    UINT32 u4AdaptiveEgMo32Th;    
    UINT32 u4AdaptiveEgMo22Th;  
    UINT32 u4AdaptivePxlMotionTh;  
    UINT32 u4AdaptiveEgMotionTh;          
    UINT16 u2FilmMode;
} DI_PD_PARAM;

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
} DI_PQ_DBG_PARAM;

typedef struct
{
    E_DI_QUALITY_MODE eIfQty;
    E_DI_QUALITY_MODE ePeQty;
    E_DI_QUALITY_MODE eFilmQty;
    UINT8 bCfgFilmOnOff; //Film Mode ON/OFF of UI Setting
    UINT8 bFilmOnOff_Y; //Chroma Film Blending Enable
    UINT8 bFilmOnOff_C; //Luma Film Blending Enable
    UINT8 b32Film; //32 Film Mode Detection Engine Enable
    UINT8 b22Film; //22 Film Mode Detection Engine Enable
} DI_PQ_MODE;

typedef struct
{
     DI_IF_STA IfSta;
     DI_PE_STA PeSta;
     DI_CS_STA CsSta;
     DI_PD_STA PdSta;
     UINT32 u4IsrIndex;
} DI_PQ_STA;

typedef struct
{
    DI_IF_PARAM IfPar;
    DI_CS_PARAM CsPar;
    DI_DAR_PARAM DarPar;
    DI_PD_PARAM PdPar;
#ifndef NDEBUG
    DI_PQ_DBG_PARAM Event[DI_DEBUG_EVENT_NUM];
#endif
} DI_PQ_PARAM;

DI_PQ_MODE  DiPQMode;
DI_PQ_STA   DiSta;
DI_PQ_PARAM DiPar;

////////////////////////////////////////////////////////////////////////////////
// Register table
////////////////////////////////////////////////////////////////////////////////
const MDDI_REGTBL_T CODE MDDI_QTY_INIT_MAIN[] =
{
    //iFution
    {MDDI_FUSION_01     , 0x04840A08, 0xFFFFFFFF},  //Increase 2D Flicker cost 
    {MDDI_FUSION_04     , 0x88442084, 0xFFFFFFFF},  //Increase 3D Sawtooth cost  
    {MDDI_FUSION_05     , 0x01010101, 0xFFFFFFFF},  //Decrease Blending ratio      
    {MDDI_FUSION_0C     , 0x00011010, 0xFFFFFFFF},  //HOMO DIFF NOISE CORING
    {MDDI_FUSION_0D     , 0x031C1004, 0xFFFFFFFF},  //Decrease the flicker cost clip setting  
    {MDDI_FUSION_22     , 0x00000F00, 0x00000F00},  //Disable Bounadary Saw
    {MDDI_FUSION_24     , 0xFFFF0000, 0xFFFFFFFF},  //Disable Film Active Window
    //CS
    {MDDI_CS_03         , 0x41822902, 0xFFFFFFFF},  //more exact valid MV condition    
    {MDDI_CS_25         , 0x00000000, 0x00000080},  //Disable border detection
    //DAR
    {MDDI_DARE_06       , 0x80000000, 0xFFFFFFFF},  //CUE depend on C motion level
    {MDDI_DARE_10       , 0x3410104B, 0xFFFFFFFF},  //Content Adaptive Filter      
    //Film
    {MDDI_FILM_00       , 0xA3020013, 0xFB3A70F3},
    {MDDI_FILM_01       , 0x80000400, 0xF8000C00},  //32 Film Mask 
    {MDDI_FILM_15       , 0x00140004, 0x03FF03FF},
    {MDDI_FILM_16       , 0x00000010, 0x000003FF},
    {MDDI_FILM_24       , 0x20300010, 0x73FF83FF},    
    {MDDI_FILM_25       , 0x70000000, 0xF01FF1FF},
    {MDDI_FILM_26       , 0x00000000, 0x000001FF},    
    {MDDI_FILM_27       , 0xE8050001, 0xFF1F0001},    
    {MDDI_FILM_28       , 0x00000000, 0x000000FF},
    {MDDI_FILM_3B       , 0xFFFFFFFF, 0xFFFFFFFF},  //FI_MO_32 MIN MAX
    {MDDI_FILM_3C       , 0xFFFFFFFF, 0xFFFFFFFF},    
    {MDDI_FILM_3D       , 0xFFFFFFFF, 0xFFFFFFFF},  //FI_MO_22 MIN MAX    
    {MDDI_FILM_3E       , 0x00000000, 0xFFFFFFFF},  //ED_MO always zero
    
    {MDDI_REGTBL_END    , 0x00000000, 0x00000000}
};

const MDDI_REGTBL_T CODE MDDI_QTY_INIT_SUB[] =
{   
    {MDDI_SB_01         , 0x00002000, 0x00002000},  //Enable Chroma Line Avg
    {MDDI_REGTBL_END    , 0x00000000, 0x00000000}
};

const MDDI_REGTBL_T CODE MDDI_PDMA_PARAM[] =
{
    {MA4F_R_FDIFF_TH2   , 0x00000000, 0x01000000},  //Fix Last Row Region Motion Problem  
    {MDDI_REGTBL_END    , 0x00000000, 0x00000000}
};

const MDDI_REGTBL_T CODE MDDI_FUSION_PARAM[3][1] =
{
    {   //RF
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //SD
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //1080i
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};

const MDDI_REGTBL_T CODE MDDI_FUSION_UI_PARAM[E_DI_QUALITY_NUM_MODE][1] = 
{
    {   //AGGRESIVE
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //DEFENSIVE
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};

const MDDI_REGTBL_T CODE MDDI_DAR_PARAM[3][4] =
{    
    {   //RF
        {MDDI_DARE_00       , 0x87000030, 0xFFFFFFFF},  //   
        {MDDI_DARE_11       , 0x21084000, 0xFFFFFFFF},  //Vertical Filter V-Variation Table
        {MDDI_DARE_12       , 0x22084000, 0xFFFFFFFF},  //Vertical Filter H-Variation Table    
        {MDDI_REGTBL_END    , 0x00000000, 0x00000000}
    },    
    {   //SD
        {MDDI_DARE_00       , 0x87000030, 0xFFFFFFFF},  //   
        {MDDI_DARE_11       , 0x21084000, 0xFFFFFFFF},  //Vertical Filter V-Variation Table
        {MDDI_DARE_12       , 0x22084000, 0xFFFFFFFF},  //Vertical Filter H-Variation Table       
        {MDDI_REGTBL_END    , 0x00000000, 0x00000000}
    },    
    {   //HD
        {MDDI_DARE_00       , 0x87000030, 0xFFFFFFFF},  //
        {MDDI_DARE_11       , 0x21084000, 0xFFFFFFFF},  //Vertical Filter V-Variation Table
        {MDDI_DARE_12       , 0x22084000, 0xFFFFFFFF},  //Vertical Filter H-Variation Table       
        {MDDI_REGTBL_END    , 0x00000000, 0x00000000}
    }
};


const MDDI_REGTBL_T CODE MDDI_PE_PARAM[3][1] =
{    
    {   //RF
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },    
    {   //SD
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },    
    {   //HD
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};

const MDDI_REGTBL_T CODE MDDI_PE_UI_PARAM[E_DI_QUALITY_NUM_MODE][1] =
{
    {   //AGGRESIVE
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //DEFENSIVE
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};

const MDDI_REGTBL_T CODE MDDI_PD_PARAM[3][14] =
{    
    {   //480i
        {MDDI_FILM_02, 0x10003020, 0xFF0F3F3F},
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
        {MDDI_FILM_18, 0x00000200, 0x003FFFFF},
        {MDDI_FILM_23, 0x00080008, 0x08FF00FF},
        {MDDI_REGTBL_END, 0x00000000, 0x00000000},
    },
    {   //576i
        {MDDI_FILM_02, 0x06003010, 0xFF0F3F3F},
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
        {MDDI_FILM_18, 0x00000200, 0x003FFFFF},
        {MDDI_FILM_23, 0x00080008, 0x08FF00FF},
        {MDDI_REGTBL_END, 0x00000000, 0x00000000},
    },
    {   //1080i
        {MDDI_FILM_02, 0x20003010, 0xFFFFFFFF},//Rate
        {MDDI_FILM_07, 0x00200200, 0xFFFFFFFF},//FR_ST MIN MAX
        {MDDI_FILM_08, 0x00200200, 0xFFFFFFFF},//FR_MO MIN MAX
        {MDDI_FILM_09, 0x04008000, 0xFFFFFFFF},//FR_SI MIN MAX
        {MDDI_FILM_0A, 0x04008000, 0xFFFFFFFF},//FI_SIM MIN MAX
        {MDDI_FILM_0B, 0x00802000, 0xFFFFFFFF},//FI_ST_32 MIN MAX
        {MDDI_FILM_0C, 0x00000100, 0xFFFFFFFF},//ED_ST_32 MIN MAX
        {MDDI_FILM_0D, 0x00801000, 0xFFFFFFFF},//FI_ST_22 MIN MAX
        {MDDI_FILM_0E, 0x00200100, 0xFFFFFFFF},//ED_ST_22 MIN MAX
        {MDDI_FILM_13, 0x00003010, 0x0000FFFF},//Saw Quant Thresh
        {MDDI_FILM_14, 0x02000000, 0x3FFFFF00},//SCENE_CHANGE_FI
        {MDDI_FILM_18, 0x00000800, 0x003FFFFF},//SCENE_CHANGE_EDGE
        {MDDI_FILM_23, 0x00080008, 0x08FF00FF},//Mixsaw ,FR_TH,Quantization Enable
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

const MDDI_REGTBL_T CODE MDDI_CS_PARAM[1][2] =
{
    {   
        {MDDI_CS_22, 0x00000000, 0xC00FFFFF},       // reset histogram clip setting
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};

static UINT8 _u1DrvPScanQualityOnOff(UINT8 bPath) 
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

////////////////////////////////////////////////////////////////////////////////
// Basic On/Off Function
////////////////////////////////////////////////////////////////////////////////
static void _vDrvFilmDefaultOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    if (bPath == VDP_1)
    {
        DiPQMode.bFilmOnOff_Y = (u1OnOff & DiPQMode.bCfgFilmOnOff & _u1DrvPScanQualityOnOff(bPath));

        if (((_arMDDiPrm[VDP_1].u1DataMode == E_DI_FULL_1W3R) 
            || (_arMDDiPrm[VDP_1].u1DataMode == E_DI_FULL_1W4R))
            && (((BSP_GetIcVersion() >= IC_VER_5365_AB) && IS_IC_5365())
            || ((BSP_GetIcVersion() >= IC_VER_5395_AB) && IS_IC_5395())))            
        {
            DiPQMode.bFilmOnOff_C = DiPQMode.bFilmOnOff_Y;
        }
        else
        {
            DiPQMode.bFilmOnOff_C = SV_OFF;
        }

        // default film setting for different source or timing
        switch (bDrvVideoGetTiming(bPath))
        {
            case MODE_625I:
            case MODE_625I_OVERSAMPLE:
            case MODE_1080i_48:
            case MODE_1080i_50:
                DiPQMode.b22Film = u1OnOff;
                DiPQMode.b32Film = SV_OFF;
                break;
            default:
                DiPQMode.b22Film = u1OnOff;
                DiPQMode.b32Film = u1OnOff;
                break;
        }
    }    
}

static void _vDrvSetIfOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_00, u1OnOff, IFUSION_EN);
}

static void _vDrvSetDarOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(VDP_1, MDDI_DARE_00, u1OnOff, DARE_EN);
}

static void _vDrvSetCsOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    UINT32 u4IsHD = IS_SD_HEIGHT(VDP_1) ? 0 : 1;
    
    LOG(6, "[%d]  CS turn %d\n", bPath, u1OnOff);
    
    MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_00, u1OnOff, IF_CS_EN);
    MDDI_WRITE_FLD(VDP_1, MDDI_CS_00, u4IsHD, CS_HD_MODE);
//    MDDI_WRITE_FLD(VDP_1, MDDI_CS_00, u4IsHD, CS_HD_GMV_MC_EN);
    MDDI_WRITE_FLD(VDP_1, MDDI_CS_08, u4IsHD, MV_TXT_HD_MODE_EN);
    MDDI_WRITE_FLD(VDP_1, MDDI_KC_00, ((_arMDDiPrm[bPath].u2Width <= 1440) && (IS_SD_HEIGHT(VDP_1)))
                                        ? u1OnOff : 0, SD_5_LINE);    
}

////////////////////////////////////////////////////////////////////////////////
// Quality mainloop function
////////////////////////////////////////////////////////////////////////////////
static void _vDrvDISetPDMAQuality(UINT8 bPath)
{
    vDrvDILoadRegTbl(VDP_2, MDDI_PDMA_PARAM);
    
    switch (bDrvVideoGetTiming(VDP_2))
    {
        case MODE_1080i_48:
        case MODE_1080i_50:
        case MODE_1080i:
            MDDI_WRITE_FLD(VDP_2, VDO_MODE, SV_ON, HD_EN);           
            break;
        default:
            MDDI_WRITE_FLD(VDP_2, VDO_MODE, SV_OFF, HD_EN);                       
            break;
    }    
}

static void _vDrvDISetIFQuality(UINT8 bPath)
{
    switch (bDrvVideoGetTiming(bPath))
    {
        case MODE_1080i_48:
        case MODE_1080i_50:
        case MODE_1080i:
            vDrvDILoadRegTbl(VDP_1, MDDI_FUSION_PARAM[2]);
            break;
        default:
            if ((bGetSignalType(VDP_1) == SV_ST_TV) 
                || (bGetSignalType(VDP_1) == SV_ST_MPEG))
            {
                vDrvDILoadRegTbl(VDP_1, MDDI_FUSION_PARAM[0]);
            }
            else
            {
                vDrvDILoadRegTbl(VDP_1, MDDI_FUSION_PARAM[1]);
            }            
            break;
    }

    if (DiPQMode.eIfQty < E_DI_QUALITY_NUM_MODE)
    {
        vDrvDILoadRegTbl(VDP_1, MDDI_FUSION_UI_PARAM[DiPQMode.eIfQty]);   
    }    

    //Set ROI
#ifndef PSCAN_AUTO_VERIFY
    MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_0E, 0x4, IF_ROI_V);
    MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_0E, 0x4, IF_ROI_H);  
    MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_0E, 
        (MDDI_READ_FLD(VDP_1, MDDI_SC_00, HDEW) - 4*4*2) * (MDDI_READ_FLD(VDP_1, MDDI_MULTI_00, VDEW) - 4*2*2) / 16   
        , IF_ROI_PIXELS);
#endif

    //CBOB Mode , Turn off chroma reference
    if((_arMDDiPrm[VDP_1].u1DataMode == E_DI_CBOB_1W3R) ||
        (_arMDDiPrm[VDP_1].u1DataMode == E_DI_CBOB_1W4R) ||
        (_arMDDiPrm[VDP_1].u1DataMode == E_DI_FLIP_MIRROR_CBOB_1W4R))       
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_08, 0x0, IF_YC_C_TO_Y_W);
    }
}

static void _vDrvDISetPEQuality(UINT8 bPath)
{
    switch (bDrvVideoGetTiming(bPath))
    {
        case MODE_1080i_48:
        case MODE_1080i_50:
        case MODE_1080i:
            vDrvDILoadRegTbl(VDP_1, MDDI_PE_PARAM[2]);
            break;
        default:
            if ((bGetSignalType(VDP_1) == SV_ST_TV) 
                || (bGetSignalType(VDP_1) == SV_ST_MPEG))
            {
                vDrvDILoadRegTbl(VDP_1, MDDI_PE_PARAM[0]);
            }
            else
            {
                vDrvDILoadRegTbl(VDP_1, MDDI_PE_PARAM[1]);
            }            
            break;
    }
    
    if (DiPQMode.ePeQty < E_DI_QUALITY_NUM_MODE)
    {
        vDrvDILoadRegTbl(VDP_1, MDDI_PE_UI_PARAM[DiPQMode.ePeQty]);    
    }
}

static void _vDrvDISetPDQuality(UINT8 bPath)
{
    switch (bDrvVideoGetTiming(bPath))
    {
        //HD
        case MODE_1080i_48:
        case MODE_1080i_50:       
        case MODE_1080i:                    
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x40, FR_MIN);  
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x40, EG_MIN);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x600, EG_MAX_END_32);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x600, EG_MAX_END_22);            
            vDrvDILoadRegTbl(bPath, MDDI_PD_PARAM[2]);
            break;
        //NTSC
        case MODE_625I:
        case MODE_625I_OVERSAMPLE:
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, FR_MIN);  
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, EG_MIN);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x100, EG_MAX_END_32);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x80, EG_MAX_END_22);            
            
            vDrvDILoadRegTbl(bPath, MDDI_PD_PARAM[1]);
            break;
        //PAL            
        default:
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, FR_MIN);  
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, EG_MIN);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x100, EG_MAX_END_32);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x80, EG_MAX_END_22);            
            vDrvDILoadRegTbl(bPath, MDDI_PD_PARAM[0]);
            break;
    }

    if (DiPQMode.eFilmQty < E_DI_QUALITY_NUM_MODE)
    {
        vDrvDILoadRegTbl(bPath, MDDI_PD_UI_PARAM[DiPQMode.eFilmQty]);   
    }     
}

static void _vDrvDISetCSQuality(UINT8 bPath)
{
    vDrvDILoadRegTbl(bPath, MDDI_CS_PARAM[0]);    
    
    switch (bDrvVideoGetTiming(bPath))
    {
        case MODE_1080i_48:
        case MODE_1080i_50:
        case MODE_1080i:
            vIO32WriteFldAlign(PSCAN_FWCS_00, 0x1C, GMV_EDGE_RATIO);     
            break;
        default:
            vIO32WriteFldAlign(PSCAN_FWCS_00, 0x30, GMV_EDGE_RATIO);             
            break;
    }
}

static void _vDrvDISetDARQuality(UINT8 bPath)
{
    switch (bDrvVideoGetTiming(bPath))
    {
        case MODE_1080i_48:
        case MODE_1080i_50:
        case MODE_1080i:
            vDrvDILoadRegTbl(VDP_1, MDDI_DAR_PARAM[2]);
            break;
        default:
            if ((bGetSignalType(VDP_1) == SV_ST_TV) 
                || (bGetSignalType(VDP_1) == SV_ST_MPEG))
            {
                vDrvDILoadRegTbl(VDP_1, MDDI_DAR_PARAM[0]);
            }
            else
            {
                vDrvDILoadRegTbl(VDP_1, MDDI_DAR_PARAM[1]);
            }            
            break;
    }
}

static void _vDrvDISetAdatpive(UINT8 bPath)
{
    UINT32 u4TotalBlk = ((_arMDDiPrm[VDP_1].u2Width>>4)-2)*((_arMDDiPrm[VDP_1].u2Height>>1)-4);

    u4TotalBlk = MDDI_READ_FLD(VDP_1, MDDI_MULTI_00, SD_1440_MODE) ? (u4TotalBlk>>1) : u4TotalBlk;

    vIO32WriteFldAlign(PSCAN_FWCS_00, u4TotalBlk, TOTAL_BLK); 
    #ifdef PSCAN_AUTO_VERIFY
    vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, 0x7, ADAPTIVE_PSCAN);
    #else
    vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, 0x1F, ADAPTIVE_PSCAN);
    #endif
}

////////////////////////////////////////////////////////////////////////////////
// Quality ISR 
////////////////////////////////////////////////////////////////////////////////
static void _vDrvDIMutliCadence(void)
{
    UINT16 i, wTest = 0;    

    if (DiPar.PdPar.u4FldBit == 4)
    {
        DiPar.PdPar.u2FilmMode = E_DI_FILM_22;
    }    
    else if ((DiPar.PdPar.u4FldBit == 5) && (DiPar.PdPar.u4FrmBit == 5))
    {
        for (i=0; i<4; i++)
        {
            wTest += ((DiPar.PdPar.u4FrmMask>>i)&0x1); 
        }
        // FILM_32 : 11110   FILM_55 : 11000 
        DiPar.PdPar.u2FilmMode = (wTest>2) ? E_DI_FILM_32 : E_DI_FILM_55;
    }    
    else if (DiPar.PdPar.u4FldBit == 6) 
    {
        DiPar.PdPar.u2FilmMode = E_DI_FILM_KALEIDOSCOPE;
    }
    else if ((DiPar.PdPar.u4FldBit == 10) && (DiPar.PdPar.u4FrmBit == 10))
    {
        for (i=0; i<10; i++)
        {
            wTest = (DiPar.PdPar.u4FldMask>>i)&0x3FF;
            
            if (wTest == FW_FILM_FI_SEQ_2224)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_2224;
                break;
            }

            if (wTest == FW_FILM_FI_SEQ_2332)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_2332;
                break;
            }

            if (wTest == FW_FILM_FI_SEQ_64)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_64;
                break;
            }
        }
    }
    else if ((DiPar.PdPar.u4FldBit == 12) && (DiPar.PdPar.u4FrmBit == 12))
    {
        DiPar.PdPar.u2FilmMode = E_DI_FILM_32322;
    }
    else if ((DiPar.PdPar.u4FldBit == 15) && (DiPar.PdPar.u4FrmBit == 15))
    {
        DiPar.PdPar.u2FilmMode = E_DI_FILM_87;
    }
    else
    {
        DiPar.PdPar.u2FilmMode = E_DI_FILM_NONE;
    }
}

static void _vDrvDISetOSD(void)
{
    UINT8 bInfoData = 0;
    
    bInfoData |= (DiPar.PdPar.u4FldBit ? 0x01 : 0x00);    
    bInfoData |= (DiPar.PdPar.u4FrmBit ? 0x02 : 0x00);
    bInfoData |= (DiPar.PdPar.bUnknowCnt ? 0x04 : 0x00);
    bInfoData |= ((DiSta.PeSta.u4EdgeSum==0) ? 0x08 : 0x00);
    bInfoData |= ((DiPar.CsPar.bFilmCnt==8) ? 0x10 : 0x00);
    bInfoData |= ((DiPar.PdPar.bFwWeave==1) ? 0x20 : 0x00);
    bInfoData |= ((DiPar.IfPar.bMovingCzp==1) ? 0x40 : 0x00);
    
    SET_USER_OSD(bInfoData, FW_FILM_INFO);
    SET_USER_OSD(DiPar.PdPar.u2FilmMode ? (1<<(DiPar.PdPar.u2FilmMode-1)) : 0, FW_FILM_MODE);    
}

static void _vDrvDIUpdateISRSetting(void)
{    
    _vDrvDIMutliCadence();

    DiPar.IfPar.bMovingCzp = 0;
    DiPar.PdPar.bFwWeave = 0;

    //Weave field 
    if (IO32ReadFldAlign(PSCAN_FWFILM_06, FILED_WEAVE_EN)
        && (DiPar.PdPar.u2FilmMode != E_DI_FILM_22) 
        && (DiPar.PdPar.u2FilmMode != E_DI_FILM_32) 
        && (DiPar.PdPar.u2FilmMode != E_DI_FILM_NONE)
        && (DiPQMode.bFilmOnOff_Y == SV_ON))
    {
        DiPar.PdPar.bFwWeave = 1;
        vRegWriteFldMulti(MDDI_FUSION_20, P_Fld(1, IF_FIX_MOTION_Y) | 
            P_Fld(0, IF_FORCE_MOTION_Y) | P_Fld(1, IF_FIX_B_WEIGHT) | 
            P_Fld((DiPar.PdPar.u4FldSeq&1) ? 0 : 8, IF_FORCE_B_WEIGHT));

        // Chroma film ??
        //vRegWriteFldMulti(MDDI_FUSION_20, P_Fld(1, IF_FIX_MOTION_C) | P_Fld(0, IF_FORCE_MOTION_C));
    }
    else if (IO32ReadFldAlign(PSCAN_FWFILM_06, MOVING_CZP_BOB_EN)
        && IS_SD_HEIGHT(VDP_1) && (DiSta.PeSta.u4AliasCnt > 4000)
        && MDDI_READ_FLD(VDP_1, MDDI_CS_28, GMV_VALID))
    {
        DiPar.IfPar.bMovingCzp = 1;
        vDrvDISetInterpMode(VDP_1, E_DI_YCBOB_0W0R);
        MDDI_WRITE_FLD(VDP_1, MDDI_PE_00, 1, PE_FORCE_USER_DIR);  
    }
    else      
    {
        //FIX B_WIGHT ALSO FORCE FILM MODE LUMA WAEVING DIRECTION, AND NO EFFECT CHROMA,
        //SHOULD NOT USE FIX_B_WEIGHT UNDER FILM MODE IN MT5395/65
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_20, (DiSta.PdSta.bCurFilmMode&0x3)!=0 ? 0 : 1, IF_FIX_B_WEIGHT);
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_20, 4, IF_FORCE_B_WEIGHT);  
        MDDI_WRITE_FLD(VDP_1, MDDI_PE_00, 0, PE_FORCE_USER_DIR);  
        vDrvDISetInterpMode(VDP_1, _arMDDiPrm[VDP_1].u1DataMode);
    }

    vIO32Write4B(PSCAN_FW_ADAPTIVE_FILM_0A, DiPar.PdPar.u4FldSeq);
    vIO32Write4B(PSCAN_FW_ADAPTIVE_FILM_0B, DiPar.PdPar.u4FrmSeq);

    _vDrvDISetOSD();
}

static void _vDrvDIGetPQStatusISR()
{
    UINT32 i, u4Heter, u4Homo;

    DiSta.u4IsrIndex = (DiSta.u4IsrIndex+1)&DILOOPMASK;
    
    //============= MA Status ====================
    #ifndef NDEBUG
    DiSta.IfSta.u4HEdgeCnt = MDDI_READ_FLD(VDP_1, MDDI_FUSION_19, IF_HOR_EDGE_CNT);
    DiSta.IfSta.u4VEdgeCnt = MDDI_READ_FLD(VDP_1, MDDI_FUSION_19, IF_VER_EDGE_CNT);
    DiSta.IfSta.u4EdgeSum = DiSta.IfSta.u4VEdgeCnt + DiSta.IfSta.u4HEdgeCnt;
    #endif
    DiSta.IfSta.u4HeterUp = MDDI_READ_FLD(VDP_1, MDDI_FUSION_12, IF_PD_SUM_HETER_UP);
    DiSta.IfSta.u4HomoUp = MDDI_READ_FLD(VDP_1, MDDI_FUSION_13, IF_PD_SUM_HOMO_UP);
    DiSta.IfSta.u4HeterDn = MDDI_READ_FLD(VDP_1, MDDI_FUSION_14, IF_PD_SUM_HETER_DN);
    DiSta.IfSta.u4HomoDn = MDDI_READ_FLD(VDP_1, MDDI_FUSION_15, IF_PD_SUM_HOMO_DN);
    u4Heter = DiSta.IfSta.u4HeterUp + DiSta.IfSta.u4HeterDn;
    u4Homo = DiSta.IfSta.u4HomoUp + DiSta.IfSta.u4HomoDn;
    DiSta.IfSta.u4HeterDiff = DIFF(u4Heter, DiSta.IfSta.u4Heter);
    DiSta.IfSta.u4HomoDiff = DIFF(u4Homo, DiSta.IfSta.u4Homo);
    DiSta.IfSta.u4Heter = u4Heter;
    DiSta.IfSta.u4Homo = u4Homo;    
    DiSta.IfSta.u4AvgCurY = MDDI_READ_FLD(VDP_1, MDDI_FUSION_16, IF_AVG_Y);
    DiSta.IfSta.u4AvgCurCb = MDDI_READ_FLD(VDP_1, MDDI_FUSION_16, IF_AVG_CB);
    DiSta.IfSta.u4AvgCurCr = MDDI_READ_FLD(VDP_1, MDDI_FUSION_16, IF_AVG_CR);
    DiSta.IfSta.u4AvgPreY = MDDI_READ_FLD(VDP_1, MDDI_FUSION_17, IF_AVG_Y_PREV);
    DiSta.IfSta.u4AvgPreCb = MDDI_READ_FLD(VDP_1, MDDI_FUSION_17, IF_AVG_CB_PREV);
    DiSta.IfSta.u4AvgPreCr = MDDI_READ_FLD(VDP_1, MDDI_FUSION_17, IF_AVG_CR_PREV);

    //============= BE Status ====================
    DiSta.PeSta.u4AliasCnt = MDDI_READ_FLD(VDP_1, MDDI_PE_12, ALIAS_CNT);
    DiSta.PeSta.u4EdgeSum = 0;
    DiSta.PeSta.u4EdgeHis[0] = u2RegRead2B(MDDI_PE_0A);
    DiSta.PeSta.u4EdgeHis[1] = u2RegRead2B(MDDI_PE_0A+2);
    
    for (i = 2; i < DI_BE_HIST_BIN; i++)
    {        
        DiSta.PeSta.u4EdgeHis[i] = u2RegRead2B(MDDI_PE_0A+2*i);
        DiSta.PeSta.u4EdgeSum += DiSta.PeSta.u4EdgeHis[i];    
    }
    
    //============= CS Status ====================    
    DiSta.CsSta.u1GmvX = MDDI_READ_FLD(VDP_1, MDDI_CS_28, GMV_MVX);
    DiSta.CsSta.u1GmvY = MDDI_READ_FLD(VDP_1, MDDI_CS_28, GMV_MVY);
    DiSta.CsSta.u2HistCnt = MDDI_READ_FLD(VDP_1, MDDI_CS_27, HIST_CNT);
    DiSta.CsSta.u2HistMax = MDDI_READ_FLD(VDP_1, MDDI_CS_27, STA_MAX_CNT1);
    DiSta.CsSta.u2EdgeCnt = MDDI_READ_FLD(VDP_1, MDDI_CS_18, STA_EDGE_CNT);
    DiSta.CsSta.u4FieldSawRank = (DiSta.CsSta.u4FieldSawRank<<4) | MDDI_READ_FLD(VDP_1, MDDI_CS_0C, FIELD_SAW_RANK);

	MDDI_WRITE_FLD(VDP_1, MDDI_CS_11, 1, HIST_STA_AUTO_READ);
	MDDI_WRITE_FLD(VDP_1, MDDI_CS_11, 0, HIST_STA_RADDR_INI);
	MDDI_WRITE_FLD(VDP_1, MDDI_CS_11, SV_ON, SET_HIST_STA_RADDR);
	
    for (i = 0; i < DI_CS_MV_HIST_BIN/2; i++)
    {
        DiSta.CsSta.u4MVHist[i] = u4RegRead4B(MDDI_CS_12)&0x3FFF3FFF;
    }

    #ifndef NDEBUG
    DiSta.CsSta.bAllBurst = MDDI_READ_FLD(VDP_1, MDDI_CS_11, PRE_WHOLE_MULTIBURST);
    DiSta.CsSta.u2SmtCnt = MDDI_READ_FLD(VDP_1, MDDI_CS_26, STA_SMT_CNT);
    DiSta.CsSta.u2SmtCntROI = MDDI_READ_FLD(VDP_1, MDDI_CS_26, STA_SMT_CNT_ROI);
    DiSta.CsSta.u1SmtDeconf = MDDI_READ_FLD(VDP_1, MDDI_CS_28, STA_SMT_DECONF);
    DiSta.CsSta.u1MvDeconf = MDDI_READ_FLD(VDP_1, MDDI_CS_28, STA_MV_DECONF);
    DiSta.CsSta.u4SawAB = MDDI_READ_FLD(VDP_1, MDDI_CS_1D, STA_SAW_AB_CNT);
    DiSta.CsSta.u4SawBC = MDDI_READ_FLD(VDP_1, MDDI_CS_1E, STA_SAW_BC_CNT);
    
    MDDI_WRITE_FLD(VDP_1, MDDI_CS_11, 1, HIST_STA_AUTO_READ);
    MDDI_WRITE_FLD(VDP_1, MDDI_CS_11, 280, HIST_STA_RADDR_INI);
    MDDI_WRITE_FLD(VDP_1, MDDI_CS_11, SV_ON, SET_HIST_STA_RADDR);

    for (i = 0; i < DI_CS_LINE_BIN; i++)
    {            
        DiSta.CsSta.u4LineValid[i] = u4RegRead4B(MDDI_CS_12);
    }
    #endif

    //============= FILM Status ====================
    DiSta.PdSta.u4FrmMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
    DiSta.PdSta.u4FldMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_10, FD_32_SAW_FI_A_B_SAW_Q);
    DiSta.PdSta.u4EdgMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
    DiSta.PdSta.bBlkMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
    DiSta.PdSta.bCurFilmMode = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0F, FILM_STATUS);
}

static void _vDrvDISetIFQualityISR()
{
    
}

static void _vDrvDISetPEQualityISR()
{
    
}

static void _vDrvDISetCSQualityISR()
{
    //FORCE SCENE CHANGE TO PREVENT MV Remains
    MDDI_WRITE_FLD(VDP_1, MDDI_CS_13, (DiSta.IfSta.u4Homo == 0) ? SV_ON : SV_OFF, SC_FORCE_ON);
}

static void _vDrvDISetDARQualityISR()
{
  
}

static void _vDrvDISetPDQualityISR()
{
    UINT16 i, j;
    UINT32 u4CurFilmMaskFr32;

    //ON/OFF for 32/22 Film Mode
    UINT8 u1Weighting = (bGetSignalType(VDP_1) == SV_ST_TV) ? 6 : 8;
    
    if (((DiSta.PdSta.bCurFilmMode&0x1) && (DiPQMode.b32Film)) 
        || ((DiSta.PdSta.bCurFilmMode&0x2) && (DiPQMode.b22Film)))
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_23, DiPQMode.bFilmOnOff_Y ? u1Weighting : 0, IF_FILM_Y_W);
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_23, DiPQMode.bFilmOnOff_C ? u1Weighting : 0, IF_FILM_C_W);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_23, 0, IF_FILM_Y_W);
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_23, 0, IF_FILM_C_W);
    }       
    
#if MDDI_PD_ADAPTIVE_THRESHOLD

    MDDI_WRITE_FLD(VDP_1, MDDI_FILM_23, DiPar.PdPar.u4AdaptivePxlMotionTh>>1, FR_MO_PXL_DIFF_THL);
    MDDI_WRITE_FLD(VDP_1, MDDI_FILM_02, DiPar.PdPar.u4AdaptivePxlMotionTh, FR_MO_CUR_THL_2);
    MDDI_WRITE_FLD(VDP_1, MDDI_FILM_23, DiPar.PdPar.u4AdaptiveEgMotionTh>>1, R_MIXSAW);
    MDDI_WRITE_FLD(VDP_1, MDDI_FILM_13, DiPar.PdPar.u4AdaptiveEgMotionTh, FI_MO_CUR_THL_2);    
    MDDI_WRITE_FLD(VDP_1, MDDI_FILM_07, DiPar.PdPar.u4AdaptiveFrMoTh, FR_ST_MIN_TH);
    MDDI_WRITE_FLD(VDP_1, MDDI_FILM_08, DiPar.PdPar.u4AdaptiveFrMoTh, FR_MO_MIN_TH); 
    MDDI_WRITE_FLD(VDP_1, MDDI_FILM_07, DiPar.PdPar.u4AdaptiveFrMoTh, FR_ST_MAX_TH);
    MDDI_WRITE_FLD(VDP_1, MDDI_FILM_08, DiPar.PdPar.u4AdaptiveFrMoTh, FR_MO_MAX_TH);     
    MDDI_WRITE_FLD(VDP_1, MDDI_FILM_0C, DiPar.PdPar.u4AdaptiveEgMo32Th, NEW_FI_ST_MAX_TH);
    MDDI_WRITE_FLD(VDP_1, MDDI_FILM_0E, DiPar.PdPar.u4AdaptiveEgMo22Th, NEW_FI_ST_MAX_TH_22);

#endif

#if MDDI_PD_SW_FW_FILM_PATCH //Patch for pulldown.trp
    
    DiPar.PdPar.bFWPatch32FrSeq = (DiPar.PdPar.bFWPatch32FrSeq<<1)|(DiSta.PdSta.u4FrmMotion==0);

    u4CurFilmMaskFr32 = (FW_FILM_FR_SEQ_32 >> ((FW_FILM_CB-1)-DiPar.PdPar.bFWPatchCB32)) & FW_FILM_MASK;

    if ((DiPar.PdPar.bFWPatch32FrSeq & FW_FILM_MASK) == u4CurFilmMaskFr32)
    {           
        DiPar.PdPar.bFWPatchCB32 = (DiPar.PdPar.bFWPatchCB32 == (FW_FILM_CB-1)) ? 0 : DiPar.PdPar.bFWPatchCB32 + 1;
        DiPar.PdPar.bFWPatchFr32Cnt = MIN(DiPar.PdPar.bFWPatchFr32Cnt+1, MDDI_FILM_ACC_MAX_SEQ_CNT);  
    }
    else
    {
        DiPar.PdPar.bFWPatchCB32 = 0;
        DiPar.PdPar.bFWPatchFr32Cnt = 0;        
    }

    if (DiPar.PdPar.bFWPatchCB32 == 3)
    {
        if (DiSta.PdSta.u4EdgMotion == 0)
        {
            DiPar.PdPar.bFWPatchFi32Cnt = MIN(DiPar.PdPar.bFWPatchFi32Cnt+1, MDDI_FILM_ACC_MAX_SEQ_CNT);
        }
        else
        {
            DiPar.PdPar.bFWPatchFi32Cnt = 0;
        }
    }    
    
    if ((DiPar.PdPar.bFWPatchFi32Cnt > 0x4) 
        && (DiPar.PdPar.bFWPatchFr32Cnt > 0x10)
        && (IS_MPEG(VDP_1))
        && (u1DrvGetASL() < 0x10))
    {    
        MDDI_WRITE_FLD(VDP_1, MDDI_FILM_01, 0, FILM_DET_TYPE);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_FILM_01, 4, FILM_DET_TYPE);
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
    
    UINT8 u1FldGain, u1FrmGain;
    UINT16 u2FrmMaskCur, u2FldMaskCur, u2Mask;    
    UINT32 u4FrmSaw = DiSta.PdSta.u4FrmMotion;   
    UINT32 u4FldSaw = DiSta.PdSta.u4EdgMotion; 
    UINT32 u4FrmSeq, u4FrmSeqNew;
    UINT32 u4FldSeq, u4FldSeqNew;    
   
    //Field Sequence
    u4FldMin = (DiSta.PdSta.bBlkMotion < u1MoBlkSt) ? (u4FldMin>>2) : u4FldMin;
    u4FrmMin = (DiSta.PdSta.bBlkMotion < u1MoBlkSt) ? (u4FrmMin>>2) : u4FrmMin;       
    u4FldMax = (DiSta.PdSta.bBlkMotion < u1MoBlkMo) ? u4FldMax : (u4FldMax<<2);
    u4FrmMax = (DiSta.PdSta.bBlkMotion < u1MoBlkMo) ? u4FrmMax : (u4FrmMax<<2);       

    if (u4FldSaw > DiPar.PdPar.u4FldThld)                                                                                     
    {          
        DiPar.PdPar.u4FldSeq = (DiPar.PdPar.u4FldSeq<<1) | 1;   
        u1FldGain = u1FldGain2;
    }                                                                                                                         
    else                                                                                                                      
    {           
        DiPar.PdPar.u4FldSeq = (DiPar.PdPar.u4FldSeq<<1) | 0;    
        u1FldGain = u1FldGain1;
    }

    //Frame Sequence
    if (u4FrmSaw > DiPar.PdPar.u4FrmThld)                                                                                     
    {                    
        DiPar.PdPar.u4FrmSeq = (DiPar.PdPar.u4FrmSeq<<1) | 1;   
        u1FrmGain = u1FrmGain2;
    }                                                                                                                         
    else                                                                                                                      
    {                               
        DiPar.PdPar.u4FrmSeq = (DiPar.PdPar.u4FrmSeq<<1) | 0;  
        u1FrmGain = u1FrmGain1;
    } 

    u4FldSeq = DiPar.PdPar.u4FldSeq;                                                                                        
    u4FrmSeq = DiPar.PdPar.u4FrmSeq;        
    DiPar.PdPar.u4FldThld = MIN(MAX(u4FldMin, (u4FldSaw*u1FldGain)>>4), u4FldMax); 
    DiPar.PdPar.u4FrmThld = MIN(MAX(u4FrmMin, (u4FrmSaw*u1FrmGain)>>4), u4FrmMax);  
    
    //Field Mask matching                                                                                                           
    if (DiPar.PdPar.u4FldBit)                                                                                                     
    {                                                                                                                         
        DiPar.PdPar.bFldCB = (DiPar.PdPar.bFldCB == (DiPar.PdPar.u4FldBit-1)) ? 0 : (DiPar.PdPar.bFldCB+1);                                   
                                                                                                                              
        u2Mask = FILM_MASK(DiPar.PdPar.u4FldBit);                                                                             
        u2FldMaskCur = (DiPar.PdPar.u4FldMask>>(DiPar.PdPar.u4FldBit-DiPar.PdPar.bFldCB))&u2Mask;  
        u4FldSeq = u4FldSeq&u2Mask;
        
        if (DiPar.PdPar.bFldCnt > 0x10) 
        {
            u4FldSeqNew = u4FldSeq|u2FldMaskCur;
        }
        else
        {            
            u4FldSeqNew = u4FldSeq;   
        }
        
        if (u2FldMaskCur == u4FldSeqNew)   
        {                                 
            DiPar.PdPar.bFldCnt = MIN(DiPar.PdPar.bFldCnt+1, MDDI_FILM_MAX_SEQ_CNT);   
        }                                                                                                                     
        else                                                                                                                  
        {                                                                                                                     
            DiPar.PdPar.bFldCB = 0;                                                                                               
            DiPar.PdPar.u4FldBit = 0;   
            DiPar.PdPar.u4FldMask = 0; 
            DiPar.PdPar.bFwBadEdit = 0;  
            DiPar.PdPar.bFldCnt = 0;            

            //LOG(5, "Field Mask %4x  Seq %4x %4x \n", u2FldMaskCur, u4FldSeq, u4FldSeqNew);
        }        
    }       

    //Frame Mask matching                                                                                                           
    if (DiPar.PdPar.u4FrmBit)                                                                                                     
    {                                                                                                                         
        DiPar.PdPar.bFrmCB = (DiPar.PdPar.bFrmCB == (DiPar.PdPar.u4FrmBit-1)) ? 0 : (DiPar.PdPar.bFrmCB+1);                                   
                                                                                                                              
        u2Mask = FILM_MASK(DiPar.PdPar.u4FrmBit);                                                                             
        u2FrmMaskCur = (DiPar.PdPar.u4FrmMask>>(DiPar.PdPar.u4FrmBit-DiPar.PdPar.bFrmCB))&u2Mask;        
        u4FrmSeq = u4FrmSeq&u2Mask;
        
        if (DiPar.PdPar.bFrmCnt > 0x10) 
        {
            u4FrmSeqNew = u4FrmSeq|u2FrmMaskCur;
        }
        else
        {
            u4FrmSeqNew = u4FrmSeq;
        }
        
        if (u2FrmMaskCur == u4FrmSeqNew)   
        {    
            DiPar.PdPar.bFrmCnt = MIN(DiPar.PdPar.bFrmCnt+1, MDDI_FILM_MAX_SEQ_CNT); 
        }                                                                                                                     
        else                                                                                                                  
        {                                                                                                                     
            DiPar.PdPar.bFrmCB = 0;                                                                                               
            DiPar.PdPar.u4FrmBit = 0;  
            DiPar.PdPar.u4FrmMask = 0; 
            DiPar.PdPar.bFrmCnt = 0;            

            //LOG(5, "Frame Mask %4x  Seq %4x %4x \n", u2FrmMaskCur, u4FrmSeq, u4FrmSeqNew);   
        }                                                                                                                     
    }      

    //CS filed sequence match, now olny 22
    if ((((DiSta.CsSta.u4FieldSawRank+0x8080)&0x8888) == 0x0000) 
        || (((DiSta.CsSta.u4FieldSawRank+0x8080)&0x8888) == 0x8888))
    {
        DiPar.CsPar.bFilmCnt = (DiPar.CsPar.bFilmCnt) < 8 ? (DiPar.CsPar.bFilmCnt+1) : 8;
    }
    else
    {
        DiPar.CsPar.bFilmCnt = (DiPar.CsPar.bFilmCnt) > 0 ? (DiPar.CsPar.bFilmCnt-1) : 0;
    }
    
    //Special case for TG35 vertical moving pattern or random moving pattern
    //Field saw detection is wrong under this condition    
    if ((DiPar.PdPar.u4FldBit == 4 || DiPar.CsPar.bFilmCnt == 8) 
        && (DiSta.PeSta.u4EdgeSum == 0) && (GET_USER_OSD(FW_LVL_STABLE)))
    {                
        DiPar.PdPar.bUnknowCnt = MIN(DiPar.PdPar.bUnknowCnt+0x8, 0x80); 
        //LOG(6, "Unknown OK:  %2x   %2x   %2x  \n", DiSta.PD.u4FldBit, DiSta.PD.bCurFilmMode, DiSta.BE[DiSta.u4IsrIndex].u4EdgeSum);   
    }
    else
    {
        DiPar.PdPar.bUnknowCnt = (DiPar.PdPar.bUnknowCnt) ? (DiPar.PdPar.bUnknowCnt-1) : 0;  
        //LOG(6, "Unknown OK:  %2x   %2x   %2x  \n", DiSta.PD.u4FldBit, DiSta.PD.bCurFilmMode, DiSta.BE[DiSta.u4IsrIndex].u4EdgeSum);   
    }
    
    //Search Field random mask                                                                                                      
    if (DiPar.PdPar.u4FldBit == 0)                                                                                                
    {                                                                                                                         
        for (i=0; i < MASK_NUM; i++)                                                                                           
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
                        DiPar.PdPar.bFwBadEdit = 1;                                                                              
                        break;                                                                                                
                    }                                                                                                         
                }                                                                                                             
                                                                                                                              
                DiPar.PdPar.u4FldMask = u4FldSeq&FILM_MASK(2*u4MaskBit[i]);
                DiPar.PdPar.u4FldBit = u4MaskBit[i];                                                                              
                break;                                                                                                        
            }                                                                                                                 
        }                                                                                                          
    } 

    //Search Frame random mask
    if (DiPar.PdPar.u4FrmBit == 0)                                                                                                
    {                                                                                                                         
        for (i=0; i < MASK_NUM; i++)                                                                                           
        {                                                                                                                     
            u2Mask = FILM_MASK(u4MaskBit[i]);                                                                             
            u2FrmMaskCur = u4FrmSeq&u2Mask;                                                                            
                                                                                                                              
            if ((u2FrmMaskCur != 0) && (u2FrmMaskCur != u2Mask) &&                                                      
                (u2FrmMaskCur == ((u4FrmSeq>>u4MaskBit[i])&u2Mask)))                                                   
            {                                                                                                             
                DiPar.PdPar.u4FrmMask = u4FrmSeq&FILM_MASK(2*u4MaskBit[i]);
                DiPar.PdPar.u4FrmBit = u4MaskBit[i];                                                                              
                break;                                                                                                        
            }                                                                                                                 
        }                                                                                                           
    }     

    if ((DiPar.PdPar.u4FldBit || DiPar.PdPar.u4FrmBit) && (DiPar.PdPar.bUnknowCnt == 0))
    {
        DiPar.PdPar.bFwAccCnt = MIN(DiPar.PdPar.bFwAccCnt+1, MDDI_FILM_ACC_MAX_SEQ_CNT);    
    }
    else
    {
        DiPar.PdPar.bFwAccCnt = (DiPar.PdPar.bFwAccCnt >= 0x8) ? (DiPar.PdPar.bFwAccCnt-2) : 0;                                 
    }   
}

#ifndef NDEBUG
static void _vDrvDIEventISR()
{
    UINT32 i, u4Value;
    
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
}

static void _vDrvGetTextInfo(TEXT_INFO* pTextInfo)
{
    UINT8 u1ValidLine, u1Conti = 0;
    UINT32 i, j, u4ValidLine, u4Acc = 0;
    UINT16 u2StartLine = 0, u2EndLine = 0, u2ContiCnt = 0;
    
    pTextInfo->u2StartLine = 0;
    pTextInfo->u2EndLine = 0;
    pTextInfo->u2TotalCnt = 0;
    pTextInfo->u2ContiCnt = 0;    
    
    for (i=0; i < DI_CS_LINE_BIN; i++)
    {   
        u4ValidLine = DiSta.CsSta.u4LineValid[i];
        u4ValidLine = (((u4ValidLine&0x3FFF0000)>>2)|(u4ValidLine&0x3FFF));
            
        for (j=0; j < 7; j++)
        {
            u1ValidLine = (UINT8)((u4ValidLine>>(4*j))&0xA);
            
            if (u1ValidLine)
            {    
                u4Acc++;
                pTextInfo->u2TotalCnt = (u1ValidLine == 0xA) ? (pTextInfo->u2TotalCnt+2) : (pTextInfo->u2TotalCnt+1);
            }
            else
            {
                u4Acc = 0;
            }
            
            if (u4Acc == 2)
            {
                u2StartLine = i*14+2*j-4;
                u1Conti = 1;
            }

            if ((u4Acc == 0) && (u1Conti == 1))
            {
                u2EndLine = i*14+2*j+2;
                u2ContiCnt = u2EndLine-u2StartLine;
                u1Conti = 0;

                if (u2ContiCnt >= pTextInfo->u2ContiCnt)
                {
                    pTextInfo->u2StartLine = u2StartLine;
                    pTextInfo->u2EndLine = u2EndLine;
                    pTextInfo->u2ContiCnt = u2ContiCnt;
                }
            }
        }
    }        
}
#endif

static void _vDrvIFAdaptive(UINT8 bPath)
{
    UINT32 u4StableTdh = IO32ReadFldAlign(PSCAN_MISC_00, STABLE_THLD);
    UINT32 u4StableCnt = IO32ReadFldAlign(PSCAN_MISC_00, STABLE_CNT);
    
    if ((DIFF(DiSta.IfSta.u4AvgCurY, DiSta.IfSta.u4AvgPreY) < u4StableTdh)
        && (DIFF(DiSta.IfSta.u4AvgCurCb, DiSta.IfSta.u4AvgPreCb) < u4StableTdh)
        && (DIFF(DiSta.IfSta.u4AvgCurCr, DiSta.IfSta.u4AvgPreCr) < u4StableTdh))
    {
        DiPar.IfPar.bStableCnt = (DiPar.IfPar.bStableCnt < u4StableCnt) ? (DiPar.IfPar.bStableCnt+1) : u4StableCnt;
    }
    else
    {
        DiPar.IfPar.bStableCnt = 0;
    }

    SET_USER_OSD((DiPar.IfPar.bStableCnt == u4StableCnt) ? 0x1 : 0x0, FW_LVL_STABLE);
}

static void _vDrvPEAdaptive(UINT8 bPath)
{

}

static void _vDrvDARAdaptive(UINT8 bPath)
{
#if MDDI_DAR_ADAPTIVE_VERTICAL_FILTER

    UINT8 u1MaxStrMin = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, DAR_V_MAX_MIN);  
    UINT8 u1MaxStrMax = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, DAR_V_MAX_MAX);  
    UINT16 u2MoPxlCoring = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_01, DAR_V_MOTION_PXL_CORING);  
    UINT16 u2MoSumCoring = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_01, DAR_V_MOTION_SUM_CORING);  
    UINT8 u1MoPxlShift = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, DAR_V_MOTION_PXL_SHIFT);  
    UINT8 u1MoSumShift = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, DAR_V_MOTION_SUM_SHIFT);  
    UINT8 u1FilterStrDelay = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, DAR_V_FILTER_STR_DELAY_SHIFT);
    UINT8 u1VTBLInputForNoise = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_03, DAR_V_TBL_IN_FOR_NOISE);  
    UINT8 u1VTBLOutputForNoise = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_03, DAR_V_TBL_OUT_FOR_NOISE);  

    static UINT8 u1MotionIdx, u1TargetValue, u1TargetFilter; 
    static UINT16 u2PreAlpha;
    static UINT32 u4MoPxl[5], u4MoSum[5];
    static INT32 i4PreNoiseLevel = 0xFFFFFFFF;

    u1MotionIdx = (u1MotionIdx < 4) ? (u1MotionIdx + 1) : 0;
    u4MoPxl[u1MotionIdx] = DiSta.PdSta.u4FrmMotion;
    u4MoSum[u1MotionIdx] = DiSta.IfSta.u4Homo;
    
    INT32 i4MoPxlLevel, i4MoSumLevel;
    UINT32 u4Min, u4Max, u1CurNoiseLvl; 
           
    //Filter Level Decide by Motion Pixel Number
    u4Min = MIN(u4MoPxl[0], MIN(u4MoPxl[1], MIN(u4MoPxl[2], MIN(u4MoPxl[3], u4MoPxl[4]))));        
    u4Max = MAX(u4MoPxl[0], MAX(u4MoPxl[1], MAX(u4MoPxl[2], MAX(u4MoPxl[3], u4MoPxl[4]))));
    
    i4MoPxlLevel=(u4MoPxl[0]+u4MoPxl[1]+u4MoPxl[2]+u4MoPxl[3]+u4MoPxl[4]-u4Min-u4Max)/3;
         
    i4MoPxlLevel = MAX(i4MoPxlLevel-u2MoPxlCoring, 0x0);
    i4MoPxlLevel = MIN(((u1MaxStrMax-u1MaxStrMin)*i4MoPxlLevel >> u1MoPxlShift),
                    (u1MaxStrMax-u1MaxStrMin)) + u1MaxStrMin;

    //Filter Level Decide by Total Motion Diff
    u4Min = MIN(u4MoSum[0], MIN(u4MoSum[1], MIN(u4MoSum[2], MIN(u4MoSum[3], u4MoSum[4]))));        
    u4Max = MAX(u4MoSum[0], MAX(u4MoSum[1], MAX(u4MoSum[2], MAX(u4MoSum[3], u4MoSum[4]))));
    
    i4MoSumLevel = (u4MoSum[0]+u4MoSum[1]+u4MoSum[2]+u4MoSum[3]+u4MoSum[4]-u4Min-u4Max)/3;

    i4MoSumLevel = MAX(i4MoSumLevel-u2MoSumCoring, 0x0);
    i4MoSumLevel = MIN(((u1MaxStrMax-u1MaxStrMin)*i4MoSumLevel >> u1MoSumShift),
                    (u1MaxStrMax-u1MaxStrMin)) + u1MaxStrMin;
   
    //Chose Maximun of Neccesary Filter
    i4MoSumLevel = MAX(i4MoSumLevel, i4MoPxlLevel)<<u1FilterStrDelay;

    //Gradually Change The Alpha Ratio
    if (i4MoSumLevel > u2PreAlpha)
    {
        u2PreAlpha++;
    }
    else if (i4MoSumLevel < u2PreAlpha)
    {
        u2PreAlpha--;
    }

    if (ABS(u2PreAlpha - (u2PreAlpha>>u1FilterStrDelay<<u1FilterStrDelay)) < 0x1)
    {
        u1TargetValue = (u2PreAlpha>>u1FilterStrDelay);
    }
    else if(ABS(u2PreAlpha - (((u2PreAlpha>>u1FilterStrDelay)+1)<<u1FilterStrDelay)) < 0x1)
    {
        u1TargetValue = (u2PreAlpha>>u1FilterStrDelay)+1;
    }

    u1TargetValue = MIN(u1TargetValue, 0x10);
    
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_DARE_10, u1TargetValue, DAR_V_MAX_ALPHA);

    if (bGetSignalType(VDP_1) == SV_ST_TV) 
    {
        //Front End Ralative Noise Adaptive Filter Strength
        u1CurNoiseLvl = DRVCUST_NoiseLevel()<<u1FilterStrDelay;

        if (i4PreNoiseLevel == 0xFFFFFFFF)
        {
            i4PreNoiseLevel = u1CurNoiseLvl;
        }
        
        if (u1CurNoiseLvl > i4PreNoiseLevel)
        {
            i4PreNoiseLevel++;
        }
        else if (u1CurNoiseLvl < i4PreNoiseLevel)
        {
            i4PreNoiseLevel--;
        }

        if (ABS(i4PreNoiseLevel - (i4PreNoiseLevel>>u1FilterStrDelay<<u1FilterStrDelay)) < 0x1)
        {
            u1TargetFilter = i4PreNoiseLevel>>u1FilterStrDelay;
        }
        else if (ABS(i4PreNoiseLevel - ((i4PreNoiseLevel>>u1FilterStrDelay+1)<<u1FilterStrDelay)) < 0x1)
        {
            u1TargetFilter = (i4PreNoiseLevel>>u1FilterStrDelay)+1;
        }
        
        //Filter Input Control
        u1CurNoiseLvl = MIN(
            (u1TargetFilter - (INT32)NM_LEVEL1) * u1VTBLInputForNoise/(NM_LEVEL8-NM_LEVEL1)
            , u1VTBLInputForNoise);        
        
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_DARE_11, 0x08 + u1CurNoiseLvl, DAR_V_VARIATION_I1);
        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_DARE_11, 0x10 + u1CurNoiseLvl, DAR_V_VARIATION_I2);

        //Filter Output 
        u1CurNoiseLvl = MIN(
            (u1TargetFilter - (INT32)NM_LEVEL1) * u1VTBLOutputForNoise/(NM_LEVEL8-NM_LEVEL1)
            , u1VTBLOutputForNoise);        

        MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_DARE_11, u1CurNoiseLvl, DAR_V_VARIATION_O2);   
    }    
    
#endif    
}

static void _vDrvCSAdaptive(UINT8 bPath)
{
    UINT32 i, j;
    UINT32 u4WholeY4, u4NonZeroCnt = 0, u4ZeroRatio;
    UINT32 u4GmvCnt = 0x08, u4GmvRatio = 0x14, u4FbGmvRatio = 0x13, u4MVHistHd;
    UINT32 u4TotalBlk = IO32ReadFldAlign(PSCAN_FWCS_00, TOTAL_BLK);  
    UINT32 u4Y4Ratio = IO32ReadFldAlign(PSCAN_FWCS_00, Y4_RATIO);  
    UINT32 u4EdgeRatio = IO32ReadFldAlign(PSCAN_FWCS_00, GMV_EDGE_RATIO);
    UINT32 u4ReduceRatio = IO32ReadFldAlign(PSCAN_FWCS_01, REDUCE_RATIO);
    UINT32 u4HistCnt = DiSta.CsSta.u2HistCnt;
      
        // for RF no signal
    if (((bGetSignalType(VDP_1) == SV_ST_TV) 
        && (bDrvVideoSignalStatus(VDP_1) == SV_VDO_NOSIGNAL))
        // for RF & CVBS weak signal
		|| ((bGetVideoDecType(VDP_1) == SV_VD_TVD3D) 
        && (bTvdSignalStrengthLevel(1) < CS_DISABLE_WEAK_SIGNAL)))
    {
        _vDrvSetCsOnOff(VDP_1, SV_OFF);
    }
    else
    {
        _vDrvSetCsOnOff(VDP_1, DiPar.CsPar.bCsOnOff);
    }
        
    if (IS_SD_HEIGHT(VDP_1))
    {
        for (i = 0; i < 279; i++)
        { 
            u4NonZeroCnt = (MV_HIST_IND(i) > 1) ? (u4NonZeroCnt+1) : u4NonZeroCnt; 
        }

        // turn off MvV4 check while really MvV4    
        u4WholeY4 = ((MV_HIST(0, 4)+MV_HIST(0, -4)) > ((u4HistCnt*u4Y4Ratio)>>8)) ? 1 : 0;
        SET_USER_OSD(u4WholeY4, FW_WHOLE_MVY4);
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_10, !u4WholeY4, MVY4_TEXTURE_CHK);          
    }
    else
    {            
        for (i = 0; i < 31; i++)
        {   
            u4MVHistHd = 0;
            for (j = 0; j < 9; j++)
            {                       
                u4MVHistHd += MV_HIST_IND(31*j+i); 
            }
            u4NonZeroCnt = (u4MVHistHd > 1) ? (u4NonZeroCnt+1) : u4NonZeroCnt; 
        }
    }

    if ((u4TotalBlk != 0) && (u4HistCnt != 0))
    {    
        u4GmvRatio = 0x14;
        u4FbGmvRatio = 0x13;
        u4GmvCnt = (DiSta.CsSta.u2EdgeCnt*u4EdgeRatio)/u4TotalBlk;
        u4ZeroRatio = (MV_HIST(0, 0)*256)/u4HistCnt;
                
        if ((u4ZeroRatio > IO32ReadFldAlign(PSCAN_FWCS_01, ZERO_RATIO))
            && (u4NonZeroCnt < IO32ReadFldAlign(PSCAN_FWCS_01, NON_ZERO_CNT)))
        {
            u4GmvCnt = (u4GmvCnt*u4ReduceRatio)>>8;
            u4GmvRatio = (u4GmvRatio*u4ReduceRatio)>>8;
            u4FbGmvRatio = (u4FbGmvRatio*u4ReduceRatio)>>8;
            SET_USER_OSD(1, FW_GMV_LOOSE);
        }
        else
        {
            SET_USER_OSD(0, FW_GMV_LOOSE);
        }

        SET_USER_OSD(0, FW_ERROR);

        //LOG(6, "NonZero %d  ZeroRatio %d  GmvCnt %d  GmvRatio %d  FbGmvRatio %d  \n", 
        //        u4NonZeroCnt, u4ZeroRatio, u4GmvCnt, u4GmvRatio, u4FbGmvRatio);
    }
    else
    {
        SET_USER_OSD(1, FW_ERROR);
    }
    
    MDDI_WRITE_FLD(VDP_1, MDDI_CS_16, u4GmvCnt, GMV_CNT_THD);  
    MDDI_WRITE_FLD(VDP_1, MDDI_CS_0C, u4GmvRatio, GMV_CNT_RATIO); 
    MDDI_WRITE_FLD(VDP_1, MDDI_CS_23, u4FbGmvRatio, NON_FB_GMV_RATIO); 

#if MDDI_CS_FW_FILM_DISABLE_CS    
    // Special case for moving cross hatch  MVY = 2
    if ((DiSta.PeSta.u4EdgeSum == 0) && (u1DrvGetASL() < 0x10) 
        && ((DiSta.CsSta.u1GmvY == 0x2) || (DiSta.CsSta.u1GmvY == 0xE)))
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_17, 0x2, FORCE_CS_FILMLIKE);
    }
    else if (DiPar.PdPar.bFwAccCnt > 0x10) //If current FW Film Detected, Disable CS
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_17, 0x3, FORCE_CS_FILMLIKE);//FORCE CURRENT FILM FOR CS
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_CS_17, 0x0, FORCE_CS_FILMLIKE);
    }
#endif
}

static void _vDrvPDAdaptive(UINT8 bPath)
{
#if MDDI_PD_ADAPTIVE_THRESHOLD
    
    static UINT32 u4FrMo[4], u4EgMo[4];
    static UINT32 u4Index = 0;

    //Adaptive Film Mode Parameter        
    u4FrMo[u4Index] = DiSta.PdSta.u4FrmMotion;
    u4EgMo[u4Index] = DiSta.PdSta.u4EdgMotion;
        
    UINT32 u4FrMinTh = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, FR_MIN);  
    UINT32 u4EgMinTh = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, EG_MIN);  
    UINT32 u4FrRate = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, FR_RATE);                                                       
    UINT32 u4EgRate32 = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, EG_RATE_32);    
    UINT32 u4EgRate22 = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, EG_RATE_22);
    UINT32 u4FrMaxTh = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_02, FR_MAX);        
    UINT32 u4EgMaxTh32 = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, EG_MAX_START_32);              
    UINT32 u4EgMaxEndTh32 = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, EG_MAX_END_32);  
    UINT32 u4EgMaxTh22 = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, EG_MAX_START_22);              
    UINT32 u4EgMaxEndTh22 = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, EG_MAX_END_22);        
    UINT32 u4EgMaxEndBitTh = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_06, EG_MAX_END_BIT);              
    UINT32 u4SubTitleRate =  IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_03, SUBTITLE_RATE);          
    UINT32 u4FrMoFilterThLow= IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, FR_MO_FILTER_L);
    UINT32 u4FrMoFilterThHigh= IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, FR_MO_FILTER_H);
    UINT32 u4FrMoFilterBit = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, FR_MO_LARGE_BIT);
    UINT32 u4EgMoFilterThLow= IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, EG_MO_FILTER_L);
    UINT32 u4EgMoFilterThHigh= IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, EG_MO_FILTER_H);
    UINT32 u4EgMoFilterBit = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, EG_MO_LARGE_BIT);
           
    UINT32 u4MaxFrMo = MAX(u4FrMo[(u4Index)&3], MAX(u4FrMo[(u4Index-1)&3], u4FrMo[(u4Index-2)&3]));
    UINT32 u4MaxEgMo = MAX(u4EgMo[(u4Index)&3], MAX(u4EgMo[(u4Index-1)&3], u4EgMo[(u4Index-2)&3]));

    // Motion Depenent Eg Max
    if( u4MaxFrMo < (1<<u4EgMaxEndBitTh))
    {
        u4EgMaxTh32 += (u4EgMaxEndTh32 - u4EgMaxTh32) * u4MaxFrMo >> u4EgMaxEndBitTh;
        u4EgMaxTh22 += (u4EgMaxEndTh22 - u4EgMaxTh22) * u4MaxFrMo >> u4EgMaxEndBitTh;            
    }
    else
    {
        u4EgMaxTh32 = u4EgMaxEndTh32;
        u4EgMaxTh22 = u4EgMaxEndTh22;            
    }
    
    //Subtitle Detection
    static UINT32 u4EgMin[2];
    static UINT32 u4EgMinIndex = 0;
    
    if ((u4EgMo[(u4Index-1)&3] <= (u4EgMo[(u4Index-0)&3]>>1))
        || (u4EgMo[(u4Index-1)&3] <= (u4EgMo[(u4Index-2)&3]>>1)))
    {
        u4EgMin[u4EgMinIndex] = u4EgMo[(u4Index-1)&3];
        u4EgMinIndex = (u4EgMinIndex+1)&1;
    }
    
    UINT32 u4EgMaxMinMo = MAX(u4EgMin[0], u4EgMin[1]);          
    u4EgMaxTh32 = MIN(MAX((u4EgMaxMinMo*u4SubTitleRate)>>2, u4EgMinTh), u4EgMaxTh32);
    u4EgMaxTh22 = MIN(MAX((u4EgMaxMinMo*u4SubTitleRate)>>2, u4EgMinTh), u4EgMaxTh22);

    //Frame Static Max
    UINT32 u4FrPreMin = MIN(u4FrMo[(u4Index)&3],MIN(u4FrMo[(u4Index-1)&3], u4FrMo[(u4Index-2)&3]));

    u4FrMaxTh = MIN(MAX((u4FrPreMin*u4SubTitleRate)>>2,u4FrMinTh), u4FrMaxTh);

    // Adaptive Motion Filter to remove Noise        
    UINT32 u4FrMoFilterTh;
    if (u4MaxFrMo <= (1<<u4FrMoFilterBit)) 
    {
        u4FrMoFilterTh = u4FrMoFilterThLow +((u4FrMoFilterThHigh - u4FrMoFilterThLow) * u4MaxFrMo >>u4FrMoFilterBit);
    }
    else
    {
        u4FrMoFilterTh = u4FrMoFilterThHigh;
    }               

    if (DiPar.PdPar.u4AdaptivePxlMotionTh > u4FrMoFilterTh)
    {
        DiPar.PdPar.u4AdaptivePxlMotionTh--;            
    }
    else if(DiPar.PdPar.u4AdaptivePxlMotionTh < u4FrMoFilterTh)
    {
        DiPar.PdPar.u4AdaptivePxlMotionTh++;
    }            

    UINT32 u4EgMoFilterTh;
    if( u4MaxEgMo <= (1<<u4EgMoFilterBit)) 
    {
        u4EgMoFilterTh = u4EgMoFilterThLow +((u4EgMoFilterThHigh - u4EgMoFilterThLow) * u4MaxEgMo >>u4EgMoFilterBit);
    }
    else
    {
        u4EgMoFilterTh = u4EgMoFilterThHigh;
    }
           
    if( DiPar.PdPar.u4AdaptiveEgMotionTh > u4EgMoFilterTh)
    {
        DiPar.PdPar.u4AdaptiveEgMotionTh--;
    }
    else if ( DiPar.PdPar.u4AdaptiveEgMotionTh < u4EgMoFilterTh)
    {
        DiPar.PdPar.u4AdaptiveEgMotionTh++;
    }        

    //Sequence Deicsion Threshold
    DiPar.PdPar.u4AdaptiveFrMoTh = MIN(MAX(u4MaxFrMo>>u4FrRate, u4FrMinTh), u4FrMaxTh);
    DiPar.PdPar.u4AdaptiveEgMo32Th = MIN(MAX(u4MaxEgMo>>u4EgRate32, u4EgMinTh), u4EgMaxTh32);

    //22 Film Mode use more strict minimun threshold
    u4EgMinTh = MIN(u4EgMinTh, u4MaxEgMo>>1);
    DiPar.PdPar.u4AdaptiveEgMo22Th = MIN(MAX(u4MaxEgMo>>u4EgRate22, u4EgMinTh), u4EgMaxTh22);            

    u4Index= (u4Index+1)&3;        
#endif
}

static void _vDrvDIInitSwRegister()
{
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
    vIO32WriteFldAlign(PSCAN_FWFILM_06, 0x00, FILED_WEAVE_EN);   

    //Adaptive CS    
    vIO32WriteFldAlign(PSCAN_FWCS_00, 0xF0, Y4_RATIO); 
    vIO32WriteFldAlign(PSCAN_FWCS_00, 0x30, GMV_EDGE_RATIO);   
    vIO32WriteFldAlign(PSCAN_FWCS_01, 0xA0, REDUCE_RATIO);   
    vIO32WriteFldAlign(PSCAN_FWCS_01, 0x04, NON_ZERO_CNT);  
    vIO32WriteFldAlign(PSCAN_FWCS_01, 0x40, ZERO_RATIO);  

    //Misc
    vIO32WriteFldAlign(PSCAN_MISC_00, 0x02, STABLE_THLD); 
    vIO32WriteFldAlign(PSCAN_MISC_00, 0x40, STABLE_CNT);

    //DYNAMIC FILM MODE DETECTION
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, FR_MIN);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x18, EG_MIN);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x03, FR_RATE);                                                       
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x02, EG_RATE_32);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x03, EG_RATE_22);    
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_02, 0x800, FR_MAX);   
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_03, 0x20, SUBTITLE_RATE);    
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x60, EG_MAX_START_32);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x600, EG_MAX_END_32);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x60, EG_MAX_START_22);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x600, EG_MAX_END_22);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_06, 0x11, EG_MAX_END_BIT); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x08, FR_MO_FILTER_L);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x28, FR_MO_FILTER_H);        
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x10, FR_MO_LARGE_BIT); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x08, EG_MO_FILTER_L);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x28, EG_MO_FILTER_H);        
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x0A, EG_MO_LARGE_BIT); 
    
    //Motion Adaptive DAR
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, 0x04, DAR_V_MAX_MIN);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, 0x10, DAR_V_MAX_MAX); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_01, 0x20, DAR_V_MOTION_PXL_CORING);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_01, 0x2000, DAR_V_MOTION_SUM_CORING);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, 0x07, DAR_V_MOTION_PXL_SHIFT);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, 0x0C, DAR_V_MOTION_SUM_SHIFT); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, 0x05, DAR_V_FILTER_STR_DELAY_SHIFT); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_03, 0x10 ,DAR_V_TBL_IN_FOR_NOISE); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_03, 0x10 ,DAR_V_TBL_OUT_FOR_NOISE);             
}

/**
 * @brief Turn on/off film mode for UI
 * @param bPath VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 * @warning This should be invoked by TVD only
 */
void vDrvDIFilmModeOnOff(UINT8 bPath, UINT8 u1OnOff)
{   
    if (bPath == VDP_1)
    {
        DiPQMode.bCfgFilmOnOff = u1OnOff;
        _vDrvFilmDefaultOnOff(VDP_1, u1OnOff);
    }    
}

UINT8 bDrvGetFilmStatus(void)
{
    return ((UINT8)DiPar.PdPar.u2FilmMode);
}

void vDrvDISetFilmModeOnOff(UINT8 u1Path, UINT8 u1FilmType, UINT8 u1OnOff)
{
    if (u1Path == VDP_1)
    {
        switch (u1FilmType)
        {
    	    case 0:
                DiPQMode.b22Film = u1OnOff;
                break;
    	    case 1:
                DiPQMode.b32Film = u1OnOff;
                break;
            case 2:
                vIO32WriteFldAlign(PSCAN_FWFILM_06, (u1OnOff ? 1 : 0), FILED_WEAVE_EN);
                break;
            default:
                break;
        }
    }    
}

UINT8 u1DrvDIGetFilmOnOff(UINT8 bPath)
{
    return DiPQMode.bCfgFilmOnOff;
}

/**
 * @brief Turn on/off film mode Freeze Fro B2R Source
 * @param bPath VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 * @warning This should be invoked by TVD only
 */
void vDrvDIFilmModeFreeze(UINT8 bPath, UINT8 u1OnOff)
{
     MDDI_WRITE_FLD(VDP_1, MDDI_FILM_27, u1OnOff, R_22ENABLE);  
     MDDI_WRITE_FLD(VDP_1, MDDI_FILM_27, u1OnOff, R_32ENABLE); 
}

void vDrvDISetMotionLevel(UINT8 u1VdpId, UINT8 u1YOnOff, UINT8 u1COnOff, UINT8 u1YLevel, UINT8 u1CLevel)
{
    u1YFixMotion[u1VdpId] = u1YOnOff;
    u1CFixMotion[u1VdpId] = u1COnOff;
    u1YMotionLevel[u1VdpId] = u1YLevel;
    u1CMotionLevel[u1VdpId] = u1CLevel;
}

void vDrvDISetInterpMode(UINT8 u1VdpId, E_DI_DMODE bMode)
{
    UINT8 u1FixMotionY, u1FixMotionC;
    UINT8 u1ForceMotionY, u1ForceMotionC;

    switch (bMode)
    {
        case E_DI_FULL_1W3R:
        case E_DI_FULL_1W4R:
        case E_DI_FLIP_MIRROR_1W4R:
            u1FixMotionY = 0x0;
            u1FixMotionC = 0x0;
            u1ForceMotionY = 0x0;
            u1ForceMotionC = 0x0;
            #ifdef __MODEL_slt__
            if (IS_MPEG(u1VdpId))
            {
                u1FixMotionY = 0x1;
                u1FixMotionC = 0x1;
                u1ForceMotionY = 0x8;
                u1ForceMotionC = 0x8;
            }
            #endif
            break;
            
        case E_DI_CBOB_1W3R:	
        case E_DI_CBOB_1W4R:
        case E_DI_FLIP_MIRROR_CBOB_1W4R:
            u1FixMotionY = 0x0;
            u1FixMotionC = 0x1;
            u1ForceMotionY = 0x0;
            u1ForceMotionC = 0x8;
            break;
            
        case E_DI_YCBOB_0W0R:
        case E_DI_YCBOB_1W1R:
        case E_DI_FLIP_MIRROR_YCBOB_1W1R:
            default:
            u1FixMotionY = 0x1;
            u1FixMotionC = 0x1;
            u1ForceMotionY = 0x8;
            u1ForceMotionC = 0x8;
            break;
    }

    #ifdef PSCAN_AUTO_VERIFY
    u1FixMotionY = u1YFixMotion[u1VdpId] ? u1YFixMotion[u1VdpId] : u1FixMotionY;
    u1FixMotionC = u1CFixMotion[u1VdpId] ? u1CFixMotion[u1VdpId] : u1FixMotionC;
    u1ForceMotionY = u1YFixMotion[u1VdpId] ? u1YMotionLevel[u1VdpId] : u1ForceMotionY;
    u1ForceMotionC = u1CFixMotion[u1VdpId] ? u1CMotionLevel[u1VdpId] : u1ForceMotionC;    
    #endif
    
    if (u1VdpId == VDP_1)
    {
        if ((_arMDDiPrm[u1VdpId].u4TdTvDIType == E_TDTV_DI_3D_I) || // sbs/realD Interlace
        (_arMDDiPrm[u1VdpId].u4TdTvDIType == E_TDTV_DI_3D_Q)) //Quincunx Interlace
        {
            u1FixMotionY = 0x1;
            u1FixMotionC = 0x1;
            u1ForceMotionY = 0x8;
            u1ForceMotionC = 0x8;
        }
        
        // Fix Bob when Quincunx Interlace
        if (_arMDDiPrm[u1VdpId].u4TdTvDIType == E_TDTV_DI_3D_Q)
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_20, 0x3, IF_FIX_BOB);  
        }
        else
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_20, 0x0, IF_FIX_BOB);  
        }
        
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_20, u1FixMotionY, IF_FIX_MOTION_Y);  
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_20, u1FixMotionC, IF_FIX_MOTION_C); 
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_20, u1ForceMotionY, IF_FORCE_MOTION_Y);
        MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_20, u1ForceMotionC, IF_FORCE_MOTION_C);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_SB_04, u1FixMotionY, SB_FIX_MOTION_Y);  
        MDDI_WRITE_FLD(VDP_1, MDDI_SB_04, u1FixMotionC, SB_FIX_MOTION_C);  
        MDDI_WRITE_FLD(VDP_1, MDDI_SB_04, u1ForceMotionY, SB_FORCE_MOTION_Y);
        MDDI_WRITE_FLD(VDP_1, MDDI_SB_04, u1ForceMotionC, SB_FORCE_MOTION_C);
    }
}

void vDrvDIInitQuality(UINT8 bPath)
{
    //initial PSCAN HW
    vDrvDILoadRegTbl(VDP_1, MDDI_QTY_INIT_MAIN);
    vDrvDILoadRegTbl(VDP_2, MDDI_QTY_INIT_SUB);
    _vDrvDIInitSwRegister();
    
    //initial FW
    DiPQMode.eIfQty = E_DI_QUALITY_AGGRESSIVE;
    DiPQMode.ePeQty = E_DI_QUALITY_AGGRESSIVE;
    DiPQMode.eFilmQty = E_DI_QUALITY_AGGRESSIVE; 
}

void vDrvDISetQuality(UINT8 bPath)
{
    UINT8 bOnOff = _u1DrvPScanQualityOnOff(bPath);
    
    if (bPath == VDP_1)
    {            
        _vDrvDISetIFQuality(VDP_1);
        _vDrvDISetDARQuality(VDP_1);        
        _vDrvDISetPEQuality(VDP_1);
        _vDrvDISetPDQuality(VDP_1);
        _vDrvDISetCSQuality(VDP_1);
        _vDrvDISetAdatpive(VDP_1);

        // force SD dispmode turn off CS to remind the CS quality is uncertain 
        if (((IS_SD_HEIGHT(VDP_1)) && (_arMDDiPrm[VDP_1].u1PSCANDisplay))
        // ECO item for HD CS issue    
		|| (((BSP_GetIcVersion() == IC_VER_5365_AA) || (BSP_GetIcVersion() == IC_VER_5395_AA))
		&& (_arMDDiPrm[VDP_1].u2Width > CS_HD_WIDTH_LIMIT)))
        {            
            DiPar.CsPar.bCsOnOff = SV_OFF;
        }
        else
        {
            DiPar.CsPar.bCsOnOff = bOnOff;
        }

        _vDrvSetCsOnOff(VDP_1, DiPar.CsPar.bCsOnOff);        
        _vDrvSetIfOnOff(VDP_1, bOnOff);
        _vDrvSetDarOnOff(VDP_1, bOnOff);    
        _vDrvFilmDefaultOnOff(VDP_1, bOnOff);
    }
    else
    {
        _vDrvDISetPDMAQuality(VDP_2);
    }
}

void vDrvDISetQualityISR()
{  
    if (_u1DrvPScanQualityOnOff(VDP_1))
    {
        _vDrvDIGetPQStatusISR();

        if ((IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_QTY))
            && (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_QTY_ISR)))
        {
            _vDrvDISetIFQualityISR();
            _vDrvDISetPEQualityISR();
            _vDrvDISetCSQualityISR();
            _vDrvDISetPDQualityISR();   
            _vDrvDISetDARQualityISR();      
            _vDrvDIUpdateISRSetting();
        }        
    }

    #ifndef NDEBUG
    _vDrvDIEventISR();
    #endif
}

void vDrvDIQtyMainLoop(UINT8 bPath)
{
    if (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_QTY) 
        && IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_QTY_MLOOP)
        && (_u1DrvPScanQualityOnOff(bPath)))
    {   
        _vDrvIFAdaptive(VDP_1);
        _vDrvCSAdaptive(VDP_1);
        _vDrvPEAdaptive(VDP_1);
        _vDrvPDAdaptive(VDP_1);
        _vDrvDARAdaptive(VDP_1);
    }
}

/**
* @brief Set DI Quality Mode
* @param eDIEngineType E_DI_ENGINE_MA/E_DI_ENGINE_EDGEP/E_DI_ENGINE_FILM
* @param eDIQualityMode E_DI_QUALITY_AGGRESSIVE / E_DI_QUALITY_DEFENSIVE
*/
void vDrvDISetQualityMode(E_DI_ENGINE_TYPE eDIEngineType, E_DI_QUALITY_MODE eDIQualityMode)
{
    switch (eDIEngineType)
    {
        case E_DI_ENGINE_IF:
            if (eDIQualityMode != DiPQMode.eIfQty)
            {
                DiPQMode.eIfQty = eDIQualityMode;
                _vDrvDISetIFQuality(VDP_1);
            }
            break;
        case E_DI_ENGINE_PE:
            if (eDIQualityMode != DiPQMode.ePeQty)
            {
                DiPQMode.ePeQty = eDIQualityMode;
                _vDrvDISetPEQuality(VDP_1);
            }
            break;
        case E_DI_ENGINE_FILM:
            if (eDIQualityMode != DiPQMode.eFilmQty)
            {
                DiPQMode.eFilmQty = eDIQualityMode;
                _vDrvDISetPDQuality(VDP_1);
            }
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//Debug Utility
////////////////////////////////////////////////////////////////////////////////

EXTERN void vDrvWaitDIVSyncSema(void);

void vDrvDISetInk(UINT8 bMode, UINT16 u2Type)
{    
    #ifndef NDEBUG
    LOG(3,"Ink Mode: 0) Ink Off , 1) CS , 2) PE , 3) iFusion, 4) DARE, 5) SB\n");

    //Reset All ink Off
    MDDI_WRITE_FLD(VDP_1, MDDI_DARE_07, SV_OFF, DARE_INK_EN);
    MDDI_WRITE_FLD(VDP_1, MDDI_SB_06, SV_OFF, SB_INK_ENABLE);
    MDDI_WRITE_FLD(VDP_2, MDDI_SB_06, SV_OFF, SB_INK_ENABLE);
    MDDI_WRITE_FLD(VDP_1, MDDI_OC_04, 0x10, OSD_Y_POS);
    MDDI_WRITE_FLD(VDP_1, MDDI_OC_04, 0x10, OSD_X_POS);
    MDDI_WRITE_FLD(VDP_1, MDDI_DARE_07, (bMode) ? (bMode-1) : bMode, INK_SEL);

    switch (bMode)
    {
        case 0:
            MDDI_WRITE_FLD(VDP_1, MDDI_OC_04, SV_OFF, HIST_INK_ON);
            MDDI_WRITE_FLD(VDP_1, MDDI_OC_04, SV_OFF, OSD_MODE);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_07, SV_OFF, DARE_INK_EN);
            MDDI_WRITE_FLD(VDP_1, MDDI_CS_17, 0, CS_INK_SEL);
            MDDI_WRITE_FLD(VDP_1, MDDI_PE_00, 0, PE_INK_SEL);
            MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_1A, 0, IF_INK_SEL);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_07, 0, DARE_INK_SEL);
            break;
            
        case 1: //CS ink            
            LOG(6, "0 : FullSearch MV\n");
            LOG(6, "1 : ME MV Before Write To Dram\n");
            LOG(6, "2 : MC RAW MV From Dram\n");
            LOG(6, "3 : Proc MV\n");
            LOG(6, "4 : Erosioned 2x1 MV\n");
            LOG(6, "5 : MC Input 2x1 MV\n");
            LOG(6, "6 : Edge Type\n");
            LOG(6, "7 : Gradient Type\n");
            LOG(6, "8 : Alias Found\n");
            LOG(6, "9 : Multi-Burst\n");
            LOG(6, "10 : Brailing\n");
            LOG(6, "11 : Instable Region\n");
            LOG(6, "12 : Text Line\n");
            LOG(6, "13 : Local De-Confidence\n");
            LOG(6, "14 : Confidence_Ini\n");
            LOG(6, "15 : Confidence\n");
            
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_07, SV_ON, DARE_INK_EN);
            MDDI_WRITE_FLD(VDP_1, MDDI_CS_17, u2Type, CS_INK_SEL);

            if (u2Type == 5)
            {
                MDDI_WRITE_FLD(VDP_1, MDDI_DARE_08, 0xdf38bdf0, INK_COLOR_Y_0);
                MDDI_WRITE_FLD(VDP_1, MDDI_DARE_09, 0x38bdf38b, INK_COLOR_Y_1);
                MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0A, 0x50fb8508, INK_COLOR_CB_0);
                MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0B, 0xfb850fb8, INK_COLOR_CB_1);
                MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0C, 0x88fffff8, INK_COLOR_CR_0);
                MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0D, 0x888, INK_COLOR_CR_1);
            }
            else if (u2Type == 15)
            {
                MDDI_WRITE_FLD(VDP_1, MDDI_DARE_08, 0x88888888, INK_COLOR_Y_0);
                MDDI_WRITE_FLD(VDP_1, MDDI_DARE_09, 0x8888888f, INK_COLOR_Y_1);
                MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0A, 0xeca86428, INK_COLOR_CB_0);
                MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0B, 0xf, INK_COLOR_CB_1);
                MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0C, 0xeca86428, INK_COLOR_CR_0);
                MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0D, 0xf, INK_COLOR_CR_1);
            }            
            break;
            
        case 2: //PE ink            
            LOG(6, "0 : Final_Dir\n");
            LOG(6, "1 : Final_Spec_Dir\n");
            LOG(6, "2 : Final_Confidence\n");
            LOG(6, "3 : Final_Coeff\n");
            LOG(6, "4 : Bob_Density\n");

            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_07, SV_ON, DARE_INK_EN);
            MDDI_WRITE_FLD(VDP_1, MDDI_PE_00, u2Type, PE_INK_SEL);

            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_08, 0x999999a8, INK_COLOR_Y_0);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_09, 0x66666688, INK_COLOR_Y_1);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0A, 0x333333a8, INK_COLOR_CB_0);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0B, 0xcccccc88, INK_COLOR_CB_1);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0C, 0x22222218, INK_COLOR_CR_0);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0D, 0xdddddd88, INK_COLOR_CR_1);
            break;

        case 3: // FUSION Ink
            LOG(6, "0 : Boundary saw type [0, 4, 8]\n");
            LOG(6, "1 : Boundary saw run [0~8]\n");
            LOG(6, "2 : Block motion level [0~15]\n");
            LOG(6, "3 : Region motion level ini [0~15]\n");
            LOG(6, "4 : Still [0~8]\n");
            LOG(6, "5 : Local motion level prev [0~15]\n");
            LOG(6, "6 : Local motion level [0~15]\n");
            LOG(6, "7 : Region motion level final [0~15]\n");
            LOG(6, "8 : Sawtooth [0~15]\n");
            LOG(6, "9 : Edge Type [0~3]\n");
            LOG(6, "10 : Y internal cost 2D [0~15]\n");
            LOG(6, "11 : C internal cost 2D [0~15]\n");
            LOG(6, "12 : Texture cost [0~15]\n");
            LOG(6, "13 : Flicker cost [0~15]\n");
            LOG(6, "14 : Y internal cost 3D [0~15]\n");
            LOG(6, "15 : C internal cost 3D [0~15]\n");
            LOG(6, "16 : MC cost [0~15]\n");
            LOG(6, "17 : Smooth cost [0~15]\n");
            LOG(6, "18 : Y sawtooth cost [0~15]\n");
            LOG(6, "19 : C sawtooth cost [0~15]\n");
            LOG(6, "20 : 2D total cost Y [0~15]\n");
            LOG(6, "21 : 2D total cost C [0~15]\n");
            LOG(6, "22 : 3D total cost Y [0~15]\n");
            LOG(6, "23 : 3D total cost C [0~15]\n");    
            LOG(6, "24 : Current Pixel Weight [2~7]\n");
            LOG(6, "25 : 2D final cost Y [0~15]\n");
            LOG(6, "26 : 2D final cost C [0~15]\n");
            LOG(6, "27 : 3D final cost Y [0~15]\n");
            LOG(6, "28 : 3D final cost C [0~15]\n");                
            LOG(6, "29 :  Y blending factor [0: still, 1-7: still blend with motion, 8: motion, 9-11: MC blend with motion, 15: MC]\n");
            LOG(6, "30 : C blending factor [0~8]\n");

            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_07, SV_ON, DARE_INK_EN);
            MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_1A, u2Type, IF_INK_SEL);
            
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_08, 0x999999a8, INK_COLOR_Y_0);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_09, 0x66666688, INK_COLOR_Y_1);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0A, 0x333333a8, INK_COLOR_CB_0);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0B, 0xcccccc88, INK_COLOR_CB_1);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0C, 0x22222218, INK_COLOR_CR_0);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0D, 0xdddddd88, INK_COLOR_CR_1);
            break;
            
        case 4: //DARE ink            
            LOG(6, "0 : Cont_Saw_Strength\n");
            LOG(6, "1 : Luma_Jaggy_Strength\n");
            LOG(6, "2 : Line_Flicker\n");
            LOG(6, "3 : Single_Saw_Strength\n");
            LOG(6, "4 : Final_Strength\n");

            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_07, SV_ON, DARE_INK_EN);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_07, u2Type, DARE_INK_SEL);            
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_08, 0x88888888, INK_COLOR_Y_0);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_09, 0x8888888f, INK_COLOR_Y_1);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0A, 0xeca86428, INK_COLOR_CB_0);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0B, 0xf, INK_COLOR_CB_1);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0C, 0xeca86428, INK_COLOR_CR_0);
            MDDI_WRITE_FLD(VDP_1, MDDI_DARE_0D, 0xf, INK_COLOR_CR_1);
            break;

        case 5: //SB ink            
            LOG(6, "0: LumaMotion\n");
            LOG(6, "1: ChromaMotion\n");
            LOG(6, "2: BlkMotion\n");
            LOG(6, "3: Saw\n");
            LOG(6, "4: PDSaw\n");
            LOG(6, "5: HMagLv\n");
            LOG(6, "6: HEdgeLv\n");
            LOG(6, "7: VEdgeLv\n");
            LOG(6, "8: motion_level_y\n");
            LOG(6, "9: mema_ink_out\n");
            LOG(6, "10: be_ink_out\n");
            LOG(6, "11: cs ink\n");
            LOG(6, "12: final_motion_level_y\n");
            LOG(6, "13: final_motion_level_c\n");
            LOG(6, "14: mc_wei_y\n");
            LOG(6, "15: mc_wei_c\n");

            MDDI_WRITE_FLD(VDP_1, MDDI_SB_06, SV_ON, SB_INK_ENABLE);
            MDDI_WRITE_FLD(VDP_2, MDDI_SB_06, SV_ON, SB_INK_ENABLE);
            MDDI_WRITE_FLD(VDP_1, MDDI_SB_06, u2Type, SB_INK_SELECT);
            MDDI_WRITE_FLD(VDP_2, MDDI_SB_06, u2Type, SB_INK_SELECT);
            break;
            
        default:
            break;
    }
#else
    UNUSED(bMode);
    UNUSED(u2Type);
#endif    
}

void vDrvDISetEvent(UINT8 bEventSel, UINT8 bParam, UINT32 u4Value)
{
#ifndef NDEBUG
    if (bEventSel >= DI_DEBUG_EVENT_NUM)
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
#else
    UNUSED(bEventSel);
    UNUSED(bParam);
    UNUSED(u4Value);
#endif    
}

#ifndef NDEBUG
/*
 * @brief MDDi Film Mode Status polling loop
 * @param pvArgv Interrupt vector
 */
void vDrvDIFilmStatusPolling(void *pvArgv)
{
    INT32 i4Cnt;
    UINT32 pu4FilmModeFlag;
    UINT32 pu4FrameStaticSeq, pu4pFrameMotionSeq, pu4FrameSimilarSeq;
    UINT32 pu4FieldStatic22Seq, pu4FieldStatic32Seq;
    UINT32 pu4FieldMotion22Seq, pu4FieldMotion32Seq;
    UINT32 pu4FieldSimilarSeq;
    UINT32 pu4EdgeStatic22Seq, pu4EdgeStatic32Seq;
    UINT32 pu4EdgeMotion22Seq, pu4EdgeMotion32Seq;
    UINT32 pu4FrameMotion, pu4FieldMotion, pu4EdgeFieldMotion ;
    UINT32 pu4BlockMotion, pu4FilmFrameCB;
    UINT32 testloop = *(UINT32*)pvArgv;
    
    Printf("22 32 2E 3E  FR_MO  FI_MO  ED_MO FR_ST FR_MO FR_SI 32_FI_ST 32_FI_MO 32_ED_ST 32_ED_MO FI_SI 22_FI_ST 22_FI_MO 22_ED_ST 22_ED_MO  BL CB\n");
    Printf("== == == ==  =====  =====  ===== ===== ===== ===== ======== ======== ======== ======== ===== ======== ======== ======== ========  ==== ==");
    while ((fgDIStatusPolling) && (testloop-- != 0))
    {
        vDrvWaitDIVSyncSema();

        pu4FilmModeFlag = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0F, FD_FLAG);
        pu4FrameMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
        pu4FieldMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_10, FD_32_SAW_FI_A_B_SAW_Q);
        pu4EdgeFieldMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
        pu4FrameStaticSeq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_08, FRAME_ST_SEQ_STATUS);
        pu4pFrameMotionSeq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_09, FRAME_ST_SEQ_STATUS);
        pu4FrameSimilarSeq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0A, FRAME_SIM_SEQ_STATUS);
        pu4FieldStatic22Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0C, FIELD_ST_SEQ_22_STATUS);
        pu4FieldStatic32Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0C, FIELD_ST_SEQ_STATUS);
        pu4FieldMotion22Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_29, FIELD_MO_SEQ_22_STATUS);
        pu4FieldMotion32Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_27, FIELD_MO_SEQ_STATUS);
        pu4EdgeStatic22Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0E, NEW_FIELD_ST_SEQ_22_STATUS);
        pu4EdgeStatic32Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0E, NEW_FIELD_ST_SEQ_STATUS);
        pu4EdgeMotion22Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_2A, NEW_FIELD_MO_SEQ_22_STATUS);
        pu4EdgeMotion32Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_28, NEW_FIELD_MO_SEQ_STATUS);
        pu4FieldSimilarSeq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0D, FIELD_SIM_SEQ_STATUS);
        pu4BlockMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
        pu4FilmFrameCB = MDDI_READ_FLD(VDP_1, MDDI_STATUS_21, FILM_CB_STATUS);

        //22 detection
        Printf("\n%d  ", (pu4FilmModeFlag>>8) & 0x1);

        //32 detection
        Printf("%d  ", (pu4FilmModeFlag>>7) & 0x1);

        //22 exit flag
        Printf("%d  ", (pu4FilmModeFlag>>0) & 0x7);

        //32 exit flag
        Printf("%d  ", (pu4FilmModeFlag>>3) & 0x7);

        Printf("%6x %6x %6x ", pu4FrameMotion & 0x3fffff, pu4FieldMotion & 0x3fffff, pu4EdgeFieldMotion & 0x3fffff);

        //frame static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameStaticSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //frame motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4pFrameMotionSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //frame similar
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameSimilarSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Field Static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Edge field static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Edge field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //Field Similar
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldSimilarSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Field Static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Edge field static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Edge field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion22Seq>>(i4Cnt - 1)) & 0x1);
        }

        Printf(" %4X  %d", (pu4BlockMotion) & 0xFFFF, (pu4FilmFrameCB) & 0x7);
    }

    Printf("\nPSCAN status polling thread END\n");
}

/*
 * @brief MDDi Film Mode Status polling loop
 * @param pvArgv Interrupt vector
 */
void vDrvDIHWFilmStatusPolling(void *pvArgv)
{
    INT32 i4Cnt;
    UINT32 pu4FilmModeFlag;
    UINT32 pu4FrameStaticSeq, pu4pFrameMotionSeq, pu4FrameSimilarSeq;
    UINT32 pu4FieldStatic22Seq, pu4FieldStatic32Seq;
    UINT32 pu4FieldMotion22Seq, pu4FieldMotion32Seq;
    UINT32 pu4FieldSimilarSeq;
    UINT32 pu4EdgeStatic22Seq, pu4EdgeStatic32Seq;
    UINT32 pu4EdgeMotion22Seq, pu4EdgeMotion32Seq;
    UINT32 pu4FrameMotion, pu4FieldMotion, pu4EdgeFieldMotion ;
    UINT32 pu4BlockMotion, pu4FilmFrameCB;
    UINT32 testloop = *(UINT32*)pvArgv;
    
    Printf("22 32 2E 3E  FR_MO  EG_TH  ED_MO FR_ST FR_MO FR_SI 32_FI_ST 32_FI_MO 32_ED_ST 32_ED_MO FI_SI 22_FI_ST 22_FI_MO 22_ED_ST 22_ED_MO  BL CB\n");
    Printf("== == == ==  =====  =====  ===== ===== ===== ===== ======== ======== ======== ======== ===== ======== ======== ======== ========  ==== ==");
    while ((fgDIStatusPolling) && (testloop-- != 0))
    {
        vDrvWaitDIVSyncSema();

        pu4FilmModeFlag = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0F, FD_FLAG);
        pu4FrameMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
        pu4FieldMotion = DiPar.PdPar.u4AdaptiveEgMo32Th;
        pu4EdgeFieldMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
        pu4FrameStaticSeq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_08, FRAME_ST_SEQ_STATUS);
        pu4pFrameMotionSeq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_09, FRAME_ST_SEQ_STATUS);
        pu4FrameSimilarSeq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0A, FRAME_SIM_SEQ_STATUS);
        pu4FieldStatic22Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0C, FIELD_ST_SEQ_22_STATUS);
        pu4FieldStatic32Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0C, FIELD_ST_SEQ_STATUS);
        pu4FieldMotion22Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_29, FIELD_MO_SEQ_22_STATUS);
        pu4FieldMotion32Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_27, FIELD_MO_SEQ_STATUS);
        pu4EdgeStatic22Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0E, NEW_FIELD_ST_SEQ_22_STATUS);
        pu4EdgeStatic32Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0E, NEW_FIELD_ST_SEQ_STATUS);
        pu4EdgeMotion22Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_2A, NEW_FIELD_MO_SEQ_22_STATUS);
        pu4EdgeMotion32Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_28, NEW_FIELD_MO_SEQ_STATUS);
        pu4FieldSimilarSeq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0D, FIELD_SIM_SEQ_STATUS);
        pu4BlockMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
        pu4FilmFrameCB = MDDI_READ_FLD(VDP_1, MDDI_STATUS_21, FILM_CB_STATUS);

        //22 detection
        Printf("\n%d  ", (pu4FilmModeFlag>>8) & 0x1);

        //32 detection
        Printf("%d  ", (pu4FilmModeFlag>>7) & 0x1);

        //22 exit flag
        Printf("%d  ", (pu4FilmModeFlag>>0) & 0x7);

        //32 exit flag
        Printf("%d  ", (pu4FilmModeFlag>>3) & 0x7);

        Printf("%6x %6x %6x ", pu4FrameMotion & 0x3fffff, pu4FieldMotion & 0x3fffff, pu4EdgeFieldMotion & 0x3fffff);

        //frame static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameStaticSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //frame motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4pFrameMotionSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //frame similar
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameSimilarSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Field Static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Edge field static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Edge field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //Field Similar
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldSimilarSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Field Static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Edge field static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Edge field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion22Seq>>(i4Cnt - 1)) & 0x1);
        }

        Printf(" %4X  %d", (pu4BlockMotion) & 0xFFFF, (pu4FilmFrameCB) & 0x7);
    }

    Printf("\nPSCAN status polling thread END\n");
}


/*
 * @brief MDDi Film Mode Status polling loop
 * @param pvArgv Interrupt vector
 */
void vDrvDIFilmChk(void *pvArgv)
{
    INT32 i4Cnt;
    UINT32 pu4FilmModeFlag;
    UINT32 pu4FrameStaticSeq, pu4pFrameMotionSeq, pu4FrameSimilarSeq;
    UINT32 pu4FieldStatic22Seq, pu4FieldStatic32Seq;
    UINT32 pu4FieldMotion22Seq, pu4FieldMotion32Seq;
    UINT32 pu4FieldSimilarSeq;
    UINT32 pu4EdgeStatic22Seq, pu4EdgeStatic32Seq;
    UINT32 pu4EdgeMotion22Seq, pu4EdgeMotion32Seq;
    UINT32 pu4FrameMotion, pu4FieldMotion, pu4EdgeFieldMotion ;
    UINT32 pu4BlockMotion, pu4FilmFrameCB;

    UINT32 u4LumaValue,u4CbValue,u4CrValue,u4PreLumaValue;
    static UINT32 u4KeyCnt;
    
    UINT32 testloop = *(UINT32*)pvArgv;                                                                          
    UINT32 u4FiMaskCur, u4FrMaskCur;
    
    Printf("KEY FR FI 22 32 2E 3E  FR_MO  FI_MO  ED_MO FR_ST FR_MO FR_SI 32_FI_ST 32_FI_MO 32_ED_ST 32_ED_MO FI_SI 22_FI_ST 22_FI_MO 22_ED_ST 22_ED_MO  BL CB\n");
    Printf("=== == == == == == ==  =====  =====  ===== ===== ===== ===== ======== ======== ======== ======== ===== ======== ======== ======== ========  ==== ==");
    while ((fgDIStatusPolling) && (testloop-- != 0))
    {
        vDrvWaitDIVSyncSema();

        u4LumaValue = MDDI_READ_FLD(VDP_1, MDDI_FUSION_16, IF_AVG_Y);
        u4CbValue = MDDI_READ_FLD(VDP_1, MDDI_FUSION_16, IF_AVG_CB);
        u4CrValue = MDDI_READ_FLD(VDP_1, MDDI_FUSION_16, IF_AVG_CR);
        u4PreLumaValue = MDDI_READ_FLD(VDP_1, MDDI_FUSION_17, IF_AVG_Y_PREV);

        if((u4CbValue==0x100)&&(u4CrValue==0x100)&&(u4LumaValue > 0x1A0)&&(u4LumaValue==u4PreLumaValue))
        {
            u4KeyCnt=MIN(u4KeyCnt+1,0xF);
        }
        else
        {
            u4KeyCnt=0;
        }

        u4FiMaskCur = (DiPar.PdPar.u4FldMask>>(DiPar.PdPar.u4FldBit-DiPar.PdPar.bFldCB))&FILM_MASK(DiPar.PdPar.u4FldBit);  
        u4FrMaskCur = (DiPar.PdPar.u4FrmMask>>(DiPar.PdPar.u4FldBit-DiPar.PdPar.bFrmCB))&FILM_MASK(DiPar.PdPar.u4FldBit);  

        //KEY 
        Printf("\n%3d  %c  %c ", u4KeyCnt>6,DiPar.PdPar.u4FrmBit?(u4FrMaskCur&1?'M':'S'):'N',
            DiPar.PdPar.u4FldBit?(u4FiMaskCur&1 ?'M':'S'):'N');
        
        pu4FilmModeFlag = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0F, FD_FLAG);
        pu4FrameMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
        pu4FieldMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_10, FD_32_SAW_FI_A_B_SAW_Q);
        pu4EdgeFieldMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
        pu4FrameStaticSeq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_08, FRAME_ST_SEQ_STATUS);
        pu4pFrameMotionSeq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_09, FRAME_ST_SEQ_STATUS);
        pu4FrameSimilarSeq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0A, FRAME_SIM_SEQ_STATUS);
        pu4FieldStatic22Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0C, FIELD_ST_SEQ_22_STATUS);
        pu4FieldStatic32Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0C, FIELD_ST_SEQ_STATUS);
        pu4FieldMotion22Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_29, FIELD_MO_SEQ_22_STATUS);
        pu4FieldMotion32Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_27, FIELD_MO_SEQ_STATUS);
        pu4EdgeStatic22Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0E, NEW_FIELD_ST_SEQ_22_STATUS);
        pu4EdgeStatic32Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0E, NEW_FIELD_ST_SEQ_STATUS);
        pu4EdgeMotion22Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_2A, NEW_FIELD_MO_SEQ_22_STATUS);
        pu4EdgeMotion32Seq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_28, NEW_FIELD_MO_SEQ_STATUS);
        pu4FieldSimilarSeq = MDDI_READ_FLD(VDP_1, MDDI_STATUS_0D, FIELD_SIM_SEQ_STATUS);
        pu4BlockMotion = MDDI_READ_FLD(VDP_1, MDDI_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
        pu4FilmFrameCB = MDDI_READ_FLD(VDP_1, MDDI_STATUS_21, FILM_CB_STATUS);

        //22 detection
        Printf("%d  ", (pu4FilmModeFlag>>8) & 0x1);

        //32 detection
        Printf("%d  ", (pu4FilmModeFlag>>7) & 0x1);

        //22 exit flag
        Printf("%d  ", (pu4FilmModeFlag>>0) & 0x7);

        //32 exit flag
        Printf("%d  ", (pu4FilmModeFlag>>3) & 0x7);

        Printf("%6x %6x %6x ", pu4FrameMotion & 0x3fffff, pu4FieldMotion & 0x3fffff, pu4EdgeFieldMotion & 0x3fffff);

        //frame static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameStaticSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //frame motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4pFrameMotionSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //frame similar
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameSimilarSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Field Static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Edge field static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Edge field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //Field Similar
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldSimilarSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Field Static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Edge field static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Edge field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion22Seq>>(i4Cnt - 1)) & 0x1);
        }

        Printf(" %4X  %d", (pu4BlockMotion) & 0xFFFF, (pu4FilmFrameCB) & 0x7);
    }

    Printf("\nPSCAN status polling thread END\n");
}

void vDrvDIIFStatusPolling(void *pvArgv)
{
    UINT32 u4Ratio, testloop = *(UINT32*)pvArgv;

    Printf("N HETER  HOMO   RATIO  DIFF_HE   DIFF_HO   Hedge   Vedge \n");
    Printf("= ====== ====== ====== ======== ========= ======= =======\n");

    while ((fgDIStatusPolling) && (testloop-- != 0))
    {
        vDrvWaitDIVSyncSema();        
        
        u4Ratio = (DiSta.IfSta.u4Homo>>8) ? (DiSta.IfSta.u4Heter/(DiSta.IfSta.u4Homo>>8)) : 0xFFFFFFFF;
        
        Printf("\n%1x %6x %6x %6x %8x %8x %6x %6x  Y %d %d  Cb %d %d  Cr %d %d",
               DiSta.u4IsrIndex, DiSta.IfSta.u4Heter, DiSta.IfSta.u4Homo, u4Ratio,
               DiSta.IfSta.u4HeterDiff, DiSta.IfSta.u4HomoDiff,               
               DiSta.IfSta.u4HEdgeCnt, DiSta.IfSta.u4VEdgeCnt,
               DiSta.IfSta.u4AvgCurY, DiSta.IfSta.u4AvgPreY,
               DiSta.IfSta.u4AvgCurCb, DiSta.IfSta.u4AvgPreCb,
               DiSta.IfSta.u4AvgCurCr, DiSta.IfSta.u4AvgPreCr);
    }
}

void vDrvDIFwFilmStatusPolling(void *pvArgv)
{
    UINT32 i, testloop = *(UINT32*)pvArgv;
    DI_PD_PARAM *pPD_Par = &DiPar.PdPar;
    DI_PD_STA *pPD_Sta = &DiSta.PdSta;

    Printf("\nIdx [ FmMo Thld    Seq    N CB Cnt ][ FdMo Thld    Seq    B CB Cnt ][Blk BE We Unknown Acc] FmMask  FldMask ", 
        DiSta.u4IsrIndex, pPD_Sta->u4FrmMotion, pPD_Par->u4FrmThld);

    while ((fgDIStatusPolling) && (testloop-- != 0))
    {
        vDrvWaitDIVSyncSema();

        Printf("\n%x   [%5x %4x ", DiSta.u4IsrIndex, pPD_Sta->u4FrmMotion, pPD_Par->u4FrmThld);

        for (i = 8; i > 0; i--)
        {
            Printf("%d", (pPD_Par->u4FrmSeq>>(i - 1)) & 0x1);
        } 
        Printf("  %x  %x  %2x ]", pPD_Par->u4FrmBit, pPD_Par->bFrmCB, pPD_Par->bFrmCnt);
        Printf("[%5x %4x ", pPD_Sta->u4EdgMotion, pPD_Par->u4FldThld);

        for (i = 8; i > 0; i--)
        {
            Printf("%d", (pPD_Par->u4FldSeq>>(i - 1)) & 0x1);
        } 
        Printf("  %x  %x  %2x ]", pPD_Par->u4FldBit, pPD_Par->bFldCB, pPD_Par->bFldCnt);
        
        Printf("[ %2x %x %x  %2x  %2x ]  ", pPD_Sta->bBlkMotion, pPD_Par->bFwBadEdit, 
            pPD_Par->bFwWeave, pPD_Par->bUnknowCnt, pPD_Par->bFwAccCnt);
        
        for (i = MAX(pPD_Par->u4FrmBit, pPD_Par->u4FldBit); i > 0; i--)
        {
            Printf("%d", (pPD_Par->u4FrmMask>>(i - 1)) & 0x1);
        }  
        Printf(" ");        
        for (i = MAX(pPD_Par->u4FrmBit, pPD_Par->u4FldBit); i > 0; i--)
        {
            Printf("%d", (pPD_Par->u4FldMask>>(i - 1)) & 0x1);
        }           
    }
}

void vDrvDITextLineStatusPolling(void *pvArgv)
{
    TEXT_INFO TextInfo; 
    UINT32 testloop = *(UINT32*)pvArgv;
        
    while ((fgDIStatusPolling) && (testloop-- != 0))
    {
        vDrvWaitDIVSyncSema();  
        _vDrvGetTextInfo(&TextInfo);

        if (IS_SD_HEIGHT(VDP_1))
        {
            Printf("Text Line From %3d to %3d conti %3d  Total  %3d  \n",
                TextInfo.u2StartLine, TextInfo.u2EndLine, TextInfo.u2ContiCnt, TextInfo.u2TotalCnt);  
        }
        else
        {
            Printf("Text Line From %3d to %3d conti %3d  Total  %3d  \n",
                TextInfo.u2StartLine*2, TextInfo.u2EndLine*2, TextInfo.u2ContiCnt*2, TextInfo.u2TotalCnt*2);  
        }
    }
}

void vDrvDIMVStatusPolling(void *pvArgv)
{
    UINT32 testloop = *(UINT32*)pvArgv;
    UINT32 i, j, u4MVHistHd;
    
    while ((fgDIStatusPolling) && (testloop-- !=0))
    {
        vDrvWaitDIVSyncSema();  
    
        Printf("\n ======== MV[%d] Status ========", DiSta.u4IsrIndex);        
           
        if (IS_SD_HEIGHT(VDP_1))
        {
            for (i = 0; i < 31; i++)
            {
                for (j = 0; j < 9; j++)
                {
                    if (MV_HIST_IND(31*j+i))
                    {
                         Printf("\n MV[%d, %d] = %d", i-15, 4-j, MV_HIST_IND(31*j+i));
                    }
                }   
            }
        }
        else
        { 
            for (i = 0; i < 31; i++)
            {
                u4MVHistHd = 0;
                for (j = 0; j < 9; j++)
                {
                    u4MVHistHd += MV_HIST_IND(31*j+i);                    
                }   

                if (u4MVHistHd)
                {
                     Printf("\n MV[%d] = %d", i-15, u4MVHistHd);
                }
            }
        }        

        Printf("\n Hist %d / %d \n", DiSta.CsSta.u2HistMax, DiSta.CsSta.u2HistCnt);
    }
}        

void vDrvDIPEPolling(void *pvArgv)
{
    UINT32 i;    
    UINT32 testloop = *(UINT32*)pvArgv;
    
    Printf("N ALIAS EdgeSum   0    V   MH   PH   M1   P1   M2   P2   M4   P4   M7   P7   \n");
    Printf("= ===== ======= ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ");

    while ((fgDIStatusPolling) && (testloop-- !=0))
    {
        vDrvWaitDIVSyncSema();

        Printf("\n%1x %4x %8x ", DiSta.u4IsrIndex, 
            DiSta.PeSta.u4AliasCnt, 
            DiSta.PeSta.u4EdgeSum);

        for (i = 0; i < DI_BE_HIST_BIN; i++)
        {
            Printf("%4x ", DiSta.PeSta.u4EdgeHis[i]);
        }
    }
}

void vDrvDIEventStatusPolling(void * pvArgv)
{
    UINT32 i;
    UINT32 testloop = *(UINT32*)pvArgv;
   
    Printf("[RegValue True/False]");
    while ((fgDIStatusPolling) && (testloop-- !=0))
    {
        vDrvWaitDIVSyncSema();

        Printf("\n");
        for (i=0; i < DI_DEBUG_EVENT_NUM; ++i)
        {
            Printf("[0x%8x 0x%1x] ",(int)u4RegRead4B(DiPar.Event[i].u4Addr)&
                   (DiPar.Event[i].u4Mask),(int)DiPar.Event[i].fgTrue);
        }
    }
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
    UINT32 testloop = *(UINT32*)pvArgv;
    
    Printf("N AllBurst EdgCnt HisCnt HisMax SMT SMT_ROI  SMT_Deconf MV_Deconf\n");
    Printf("= ======== ====== ====== ====== === =======  ========== =========\n");

    while ((fgDIStatusPolling) && (testloop-- !=0))
    {
        vDrvWaitDIVSyncSema();        
        
        Printf("%1x   %1x    %4x  [%4x  %4x]  [%4x  %4x]  [%1x  %1x] \n", 
            DiSta.u4IsrIndex,
            DiSta.CsSta.bAllBurst,
            DiSta.CsSta.u2EdgeCnt,
            DiSta.CsSta.u2HistCnt,
            DiSta.CsSta.u2HistMax,
            DiSta.CsSta.u2SmtCnt,
            DiSta.CsSta.u2SmtCntROI,
            DiSta.CsSta.u1SmtDeconf,
            DiSta.CsSta.u1MvDeconf);
    }
}

void vDrvDIAdaptivePolling(void *pvArgv)
{    
    UINT32 testloop = *(UINT32*)pvArgv;
    
    Printf("N u4SawAB  u4SawBC SawRank \n");
    Printf("= ======== ======= =======\n");
    
    while ((fgDIStatusPolling) && (testloop-- !=0))
    {
        vDrvWaitDIVSyncSema();

        Printf("%1x %8x %8x   %8x \n", 
            DiSta.u4IsrIndex,
            DiSta.CsSta.u4SawAB,
            DiSta.CsSta.u4SawBC,
            DiSta.CsSta.u4FieldSawRank);
    }
}
#endif

DI_PQ_THREAD DIPQThread[] =
{
#ifndef NDEBUG
    {vDrvDIIFStatusPolling,         "Fusion Status"},
    {vDrvDITextLineStatusPolling,   "Text Line Status"},
    {vDrvDIFwFilmStatusPolling,     "FW FILM"},
    {vDrvDIFilmStatusPolling,       "HW FILM"},
    {vDrvDIHWFilmStatusPolling,     "NEW HW FILM"},    
    {vDrvDIFilmChk,                 "FILM Check"},    
    {vDrvDIMVStatusPolling,         "MV Hist"},
    {vDrvDIPEPolling,               "PE Status"},
    {vDrvDIEventStatusPolling,      "Event Status"},
    {vDrvDIEventProfileStatusPolling,   "Event Profile Status"},
    {vDrvDICSPolling,                   "CS Status"},
    {vDrvDIAdaptivePolling,             "Adaptive FW Status"},
#endif
    {NULL                       ,NULL}//End Of Function Table
};

