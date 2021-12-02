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
 * $Author: caihua.lei $
 * $Date: 2012/10/17 $
 * $RCSfile: drv_di_int.c,v $
 * $Revision: #5 $
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
#ifdef CC_MT5396
#include "hw_mjcfd.h"
#endif
#include "hw_ycproc.h"
#include "video_timing.h"
#include "source_select.h"
#include "drv_meter.h"
#include "x_hal_5381.h"
#include "video_def.h"
#include "fbm_drvif.h"

//Disable CS when signal level lower than this threshold
extern UINT8 bTvdSignalStrengthLevel(UINT8 bMode);
#define CS_DISABLE_WEAK_SIGNAL            50       
#define SUPPORT_MULTI_CADENCE             1

#define MDDI_PD_SW_FW_FILM_PATCH            1       //Patch for Some Scenario HW Can not detect
#define MDDI_PD_ADAPTIVE_THRESHOLD          1       //Adaptive Film Mode Seqence Threshold
#define MDDI_CS_FW_FILM_DISABLE_CS          1
#define MDDI_DAR_ADAPTIVE_VERTICAL_FILTER   1

#define DI_DEBUG_EVENT_NUM      6
#define FW_FILM_FI_SEQ_32       0x210   //b1000010000
#define FW_FILM_MASK            0x1F    //b11111
#define FW_FILM_CB              5

#define FW_FILM_FR_SEQ_32       0x1EF   //b0111101111
#define FW_FILM_FR_SEQ_55       0x063   //b0001100011
#define FW_FILM_FR_SEQ_2224     0x0FF   //b0011111111
#define FW_FILM_FR_SEQ_2332     0x3F6   //b1111110110
#define FW_FILM_FR_SEQ_64       0x198   //b0110011000
#define FW_FILM_FR_SEQ_87       0x303   //b000001100000011         

#define MDDI_FILM_ACC_MAX_SEQ_CNT    0x60
#define MDDI_FILM_MAX_SEQ_CNT        0xFF

#define FILM_MASK(bit)      ((1<<bit)-1)
#define MASK_NUM             5
UINT32 u4MaskBit[MASK_NUM] = {15, 12, 10, 6, 4};

//Status Filter
#define DILOOPNUM               8   
#define DILOOPMASK              (DILOOPNUM-1) 

UINT8 u1FreezeState = 3; // bit 0 --> user freeze , bit 1 film freeze
UINT8 fgDIStatusPolling;
UINT8 u1YFixMotion[VDP_NS] = {0, 0};
UINT8 u1CFixMotion[VDP_NS] = {0, 0};
UINT8 u1YMotionLevel[VDP_NS] = {0, 0};
UINT8 u1CMotionLevel[VDP_NS] = {0, 0};

typedef struct
{
    UINT8 bMovingCzp;    
    UINT8 bFldInv;  
    UINT8 bCzpState;
} DI_IF_PARAM;

typedef struct
{   
    UINT8 bCsOnOff;
    UINT8 bFilmCnt;    
    UINT8 bFavorCS;        
} DI_CS_PARAM;

typedef struct
{      
    UINT32 u4DarCfg;
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
    UINT8 bFilm24To50;
    UINT8 bSmallMo;    
    UINT8 bFilmPatch;
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
    UINT8 bFilmOnOff_Y; //Luma Film Blending Enable
    UINT8 bFilmOnOff_C; //Chroma Film Blending Enable    
    UINT8 bFilmOnOff_MultiCadence; //MultiCadence Enable
    UINT8 b32Film; //32 Film Mode Detection Engine Enable
    UINT8 b22Film; //22 Film Mode Detection Engine Enable
} DI_PQ_MODE;

typedef struct
{
    DI_IF_PARAM IfPar;
    DI_CS_PARAM CsPar;
    DI_DAR_PARAM DarPar;
    DI_PD_PARAM PdPar;
    UINT8 bForceSceneChange;
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
    {MCVP_FUSION_01     , 0x04840A08, 0xFFFFFFFF},  //Increase 2D Flicker cost 
    {MCVP_FUSION_04     , 0x88442084, 0xFFFFFFFF},  //Increase 3D Sawtooth cost  
    {MCVP_FUSION_05     , 0x01010101, 0xFFFFFFFF},  //Decrease Blending ratio      
    {MCVP_FUSION_0C     , 0x00011010, 0xFFFFFFFF},  //HOMO DIFF NOISE CORING
    {MCVP_FUSION_0D     , 0x031C1004, 0xFFFFFFFF},  //Decrease the flicker cost clip setting  
    {MCVP_FUSION_22     , 0x00000F00, 0x00000F00},  //Disable Bounadary Saw
    {MCVP_FUSION_24     , 0xFFFF0000, 0xFFFFFFFF},  //Disable Film Active Window
    //CS
    {MCVP_CS_03         , 0x41822902, 0xFFFFFFFF},  //more exact valid MV condition
    {MCVP_CS_1F         , 0x00040000, 0x00078000},  // more pre_flt_coef (8)
    {MCVP_CS_25         , 0x00000000, 0x00000080},  //Disable border detection
    //DAR
    {MCVP_DARE_06       , 0x80001020, 0xFFFFFFFF},  //CUE depend on C motion level
    {MCVP_DARE_10       , 0x3410104B, 0xFFFFFFFF},  //Content Adaptive Filter      
    //Film
    {MCVP_FILM_00       , 0xA3020013, 0xFB3A70F3},
    {MCVP_FILM_01       , 0x80000400, 0xF8000C00},  //32 Film Mask 
    {MCVP_FILM_15       , 0x00140004, 0x03FF03FF},
    {MCVP_FILM_16       , 0x00000010, 0x000003FF},
    {MCVP_FILM_24       , 0x20300010, 0x73FF83FF},    
    {MCVP_FILM_25       , 0x70000000, 0xF01FF1FF},
    {MCVP_FILM_26       , 0x00000000, 0x000001FF},    
    {MCVP_FILM_27       , 0xE8050001, 0xFF1F0001},    
    {MCVP_FILM_28       , 0x00000000, 0x000000FF},
    {MCVP_FILM_3B       , 0xFFFFFFFF, 0xFFFFFFFF},  //FI_MO_32 MIN MAX
    {MCVP_FILM_3C       , 0xFFFFFFFF, 0xFFFFFFFF},    
    {MCVP_FILM_3D       , 0xFFFFFFFF, 0xFFFFFFFF},  //FI_MO_22 MIN MAX    
    {MCVP_FILM_3E       , 0x00000000, 0xFFFFFFFF},  //ED_MO always zero
    
    {MDDI_REGTBL_END    , 0x00000000, 0x00000000}
};

const MDDI_REGTBL_T CODE MDDI_QTY_INIT_SUB[] =
{   
    {MCVP_SB_01         , 0x00002000, 0x00002000},  //Enable Chroma Line Avg
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
        {MCVP_DARE_00       , 0x87000130, 0xFFFFFFFF},  //   
        {MCVP_DARE_11       , 0x41884000, 0xFFFFFFFF},  //Vertical Filter V-Variation Table
        {MCVP_DARE_12       , 0x22084000, 0xFFFFFFFF},  //Vertical Filter H-Variation Table    
        {MDDI_REGTBL_END    , 0x00000000, 0x00000000}
    },    
    {   //SD
        {MCVP_DARE_00       , 0x87000130, 0xFFFFFFFF},  //   
        {MCVP_DARE_11       , 0x21024200, 0xFFFFFFFF},  //Vertical Filter V-Variation Table
        {MCVP_DARE_12       , 0x43012210, 0xFFFFFFFF},  //Vertical Filter H-Variation Table       
        {MDDI_REGTBL_END    , 0x00000000, 0x00000000}
    },    
    {   //HD
        {MCVP_DARE_00       , 0x87000030, 0xFFFFFFFF},  //
        {MCVP_DARE_11       , 0x41884000, 0xFFFFFFFF},  //Vertical Filter V-Variation Table
        {MCVP_DARE_12       , 0x22084000, 0xFFFFFFFF},  //Vertical Filter H-Variation Table       
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
        {MCVP_FILM_02, 0x10003020, 0xFF0F3F3F},
        {MCVP_FILM_07, 0x00100200, 0xFFFFFFFF},
        {MCVP_FILM_08, 0x00140280, 0xFFFFFFFF},
        {MCVP_FILM_09, 0x00100FA0, 0xFFFFFFFF},
        {MCVP_FILM_0A, 0x000A0FA0, 0xFFFFFFFF},
        {MCVP_FILM_0B, 0x00050300, 0xFFFFFFFF},
        {MCVP_FILM_0C, 0x000600E0, 0xFFFFFFFF},
        {MCVP_FILM_0D, 0x00050280, 0xFFFFFFFF},
        {MCVP_FILM_0E, 0x00000040, 0xFFFFFFFF},
        {MCVP_FILM_13, 0x00003010, 0x0000FFFF},
        {MCVP_FILM_14, 0x00400000, 0x3FFFFF00},
        {MCVP_FILM_18, 0x00000200, 0x003FFFFF},
        {MCVP_FILM_23, 0x00080008, 0x08FF00FF},
        {MDDI_REGTBL_END, 0x00000000, 0x00000000},
    },
    {   //576i
        {MCVP_FILM_02, 0x06003010, 0xFF0F3F3F},
        {MCVP_FILM_07, 0x00100200, 0xFFFFFFFF},
        {MCVP_FILM_08, 0x00140280, 0xFFFFFFFF},
        {MCVP_FILM_09, 0x00100FA0, 0xFFFFFFFF},
        {MCVP_FILM_0A, 0x000A0FA0, 0xFFFFFFFF},
        {MCVP_FILM_0B, 0x00050300, 0xFFFFFFFF},
        {MCVP_FILM_0C, 0x000600E0, 0xFFFFFFFF},
        {MCVP_FILM_0D, 0x00050280, 0xFFFFFFFF},
        {MCVP_FILM_0E, 0x00000040, 0xFFFFFFFF},
        {MCVP_FILM_13, 0x00003010, 0x0000FFFF},
        {MCVP_FILM_14, 0x00400000, 0x3FFFFF00},
        {MCVP_FILM_18, 0x00000200, 0x003FFFFF},
        {MCVP_FILM_23, 0x00080008, 0x08FF00FF},
        {MDDI_REGTBL_END, 0x00000000, 0x00000000},
    },
    {   //1080i
        {MCVP_FILM_02, 0x20003010, 0xFFFFFFFF},//Rate
        {MCVP_FILM_07, 0x00200200, 0xFFFFFFFF},//FR_ST MIN MAX
        {MCVP_FILM_08, 0x00200200, 0xFFFFFFFF},//FR_MO MIN MAX
        {MCVP_FILM_09, 0x04008000, 0xFFFFFFFF},//FR_SI MIN MAX
        {MCVP_FILM_0A, 0x04008000, 0xFFFFFFFF},//FI_SIM MIN MAX
        {MCVP_FILM_0B, 0x00802000, 0xFFFFFFFF},//FI_ST_32 MIN MAX
        {MCVP_FILM_0C, 0x00000100, 0xFFFFFFFF},//ED_ST_32 MIN MAX
        {MCVP_FILM_0D, 0x00801000, 0xFFFFFFFF},//FI_ST_22 MIN MAX
        {MCVP_FILM_0E, 0x00200100, 0xFFFFFFFF},//ED_ST_22 MIN MAX
        {MCVP_FILM_13, 0x00003010, 0x0000FFFF},//Saw Quant Thresh
        {MCVP_FILM_14, 0x02000000, 0x3FFFFF00},//SCENE_CHANGE_FI
        {MCVP_FILM_18, 0x00000800, 0x003FFFFF},//SCENE_CHANGE_EDGE
        {MCVP_FILM_23, 0x00080008, 0x08FF00FF},//Mixsaw ,FR_TH,Quantization Enable
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

const MDDI_REGTBL_T CODE MDDI_CS_PARAM[3][8] =
{
    {   //RF (ATV,DTV)
		{MCVP_CS_04, 0x081D1C04, 0x00FFFF00},
		{MCVP_CS_05, 0x02080802, 0x00FFFF00},
		{MCVP_CS_06, 0x270F0F02, 0x00FFFF00},
		{MCVP_CS_07, 0x02140C04, 0x00FFFF00},
		{MCVP_CS_09, 0x20120E08, 0x00FFFF00},
        {MCVP_CS_20, 0x01D04200, 0x01F00000},       
        {MCVP_CS_22, 0x00000000, 0xC00FFFFF},  // reset histogram clip setting
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //PAL
		{MCVP_CS_04, 0x08100804, 0x00FFFF00},
		{MCVP_CS_05, 0x02040402, 0x00FFFF00},
		{MCVP_CS_06, 0x27040402, 0x00FFFF00},
		{MCVP_CS_07, 0x02100804, 0x00FFFF00},
		{MCVP_CS_09, 0x20100C08, 0x00FFFF00},
        {MCVP_CS_20, 0x00D04200, 0x01F00000},
        {MCVP_CS_22, 0x00000000, 0xC00FFFFF}, 
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //NTSC
		{MCVP_CS_04, 0x08100804, 0x00FFFF00},
		{MCVP_CS_05, 0x02040402, 0x00FFFF00},
		{MCVP_CS_06, 0x27040402, 0x00FFFF00},
		{MCVP_CS_07, 0x02100804, 0x00FFFF00},
		{MCVP_CS_09, 0x20100C08, 0x00FFFF00},
        {MCVP_CS_20, 0x01E04200, 0x01F00000},
        {MCVP_CS_22, 0x00000000, 0xC00FFFFF}, 
        {MDDI_REGTBL_END, 0x00000000, 0x00000000}
    }
};

static UINT8 _u1DrvPScanQualityOnOff(UINT8 bPath) 
{
    if ((bPath != VDP_1) || IS_DI_BOB(VDP_1))
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
        DiPQMode.bFilmOnOff_MultiCadence = (DiPQMode.bFilmOnOff_Y & SUPPORT_MULTI_CADENCE);

        if (IS_DI_3D(VDP_1))
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

        //Caihua Add 2011-11-24
        if(IS_DTV_SD_I(VDP_1) || IS_DTV_HD_I(VDP_1))
        {
            DiPQMode.b22Film = SV_OFF;
        }
    }    
}

static void _vDrvSetIfOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_00, u1OnOff, IFUSION_EN);
}

static void _vDrvSetDarOnOff(UINT8 bPath, UINT8 u1OnOff)
{
#ifdef __MODEL_slt__
    MDDI_WRITE_FLD(VDP_1, MCVP_DARE_00, SV_OFF, DARE_EN);
#else
    MDDI_WRITE_FLD(VDP_1, MCVP_DARE_00, u1OnOff, DARE_EN);
#endif    
}

static void _vDrvSetCsOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    UINT32 u4IsHD = IS_SD_HEIGHT(VDP_1) ? 0 : 1;
    
    LOG(6, "[%d]  CS turn %d\n", bPath, u1OnOff);
    
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_00, u1OnOff, IF_CS_EN);
    //MDDI_WRITE_FLD(VDP_1, MCVP_CS_00, 0, CS_HD_GMV_MC_EN);
    MDDI_WRITE_FLD(VDP_1, MCVP_CS_08, u4IsHD, MV_TXT_HD_MODE_EN);  
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
            vRegWriteFldAlign(VDO_MODE, SV_ON, HD_EN);           
            break;
        default:
            vRegWriteFldAlign(VDO_MODE, SV_OFF, HD_EN);                       
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
#ifdef PSCAN_AUTO_VERIFY
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_0E, 0x4, IF_ROI_V);
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_0E, 0x4, IF_ROI_H);  
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_0E, 
        (MDDI_READ_FLD(VDP_1, MCVP_KC_0A, HDEW) - 4*4*2) * (MDDI_READ_FLD(VDP_1, MCVP_KC_0A, VDEW) - 4*2*2) / 16   
        , IF_ROI_PIXELS);
#endif

    if(bGetSignalType(VDP_1) == SV_ST_TV || bGetSignalType(VDP_1) == SV_ST_AV)
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_01, 0x4, IF_W_FLICKER);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, 0x0, IF_W_SAW_SIM_CHK_TV);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_01, 0x8, IF_W_FLICKER);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, 0x1, IF_W_SAW_SIM_CHK_TV);
    }

    //CBOB Mode , Turn off chroma reference
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_08, IS_DI_3D(VDP_1) ? 1 : 0, IF_YC_C_TO_Y_W);
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
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x60, FR_MIN);  
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x40, EG_MIN);            
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x02, EG_RATE_22);    
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x600, EG_MAX_END_32);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x600, EG_MAX_END_22);              
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x0A, EG_MO_FILTER_L);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x34, EG_MO_FILTER_H);                
            vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x40, FLD_MAX);            
            vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x68, FLD_MIN);            
            vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x50, FRM_MAX);
            vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x50, FRM_MIN);            
            vDrvDILoadRegTbl(bPath, MDDI_PD_PARAM[2]);
            break;
        //NTSC
        case MODE_625I:
        case MODE_625I_OVERSAMPLE:
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, FR_MIN);  
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, EG_MIN);            
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x03, EG_RATE_22);    
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x100, EG_MAX_END_32);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x80, EG_MAX_END_22);             
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x06, EG_MO_FILTER_L);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x24, EG_MO_FILTER_H);        
            vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x10, FLD_MAX);
            vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x20, FLD_MIN);
            vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MAX);
            vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MIN);            
            vDrvDILoadRegTbl(bPath, MDDI_PD_PARAM[1]);
            break;
        //PAL            
        default:
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, FR_MIN);  
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, EG_MIN);            
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x03, EG_RATE_22);    
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x100, EG_MAX_END_32);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x80, EG_MAX_END_22);               
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x06, EG_MO_FILTER_L);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x24, EG_MO_FILTER_H);        
            vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x10, FLD_MAX);
            vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x20, FLD_MIN);
            vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MAX);
            vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MIN);            
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
    UINT8 bTimingType;

    bTimingType = bDrvVideoGetTiming(bPath);
    
    switch (bGetSignalType(bPath))
    {
        case SV_ST_TV:
        case SV_ST_MPEG:
            vDrvDILoadRegTbl(VDP_1, MDDI_CS_PARAM[0]);
            break;
        default:
            if((bTimingType == MODE_625I) || (bTimingType == MODE_625I_OVERSAMPLE))
            {
                vDrvDILoadRegTbl(VDP_1, MDDI_CS_PARAM[1]);
            }
            else
            {
                vDrvDILoadRegTbl(VDP_1, MDDI_CS_PARAM[2]);
            }
            break;
    }

    if((_arMDDiPrm[bPath].u2Width <=720) || IS_OVERSAMPLE(bPath)) //SD Timing
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_00, 0, ME_MODE);
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_03, 0x1C, SAD_GRA_RATIO1);
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_03, 0x1E, SAD_GRA_RATIO2);
        vIO32WriteFldAlign(PSCAN_FWCS_00, 0x30, GMV_EDGE_RATIO); 
    }
    else if(_arMDDiPrm[bPath].u2Width <=1280)//720P
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_00, 1, ME_MODE);
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_03, 0xA, SAD_GRA_RATIO1);
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_03, 0x10, SAD_GRA_RATIO2);
        vIO32WriteFldAlign(PSCAN_FWCS_00, 0x1C, GMV_EDGE_RATIO);     
    }
    else if(IS_INTERLACE(bPath))//1080I
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_00, 2, ME_MODE);
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_03, 0xA, SAD_GRA_RATIO1);
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_03, 0x10, SAD_GRA_RATIO2);
        vIO32WriteFldAlign(PSCAN_FWCS_00, 0x1C, GMV_EDGE_RATIO);     
    }
    else//1080P
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_00, 3, ME_MODE);
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_03, 0xA, SAD_GRA_RATIO1);
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_03, 0x10, SAD_GRA_RATIO2);
        vIO32WriteFldAlign(PSCAN_FWCS_00, 0x1C, GMV_EDGE_RATIO);     
    }
    
    if(bGetSignalType(VDP_1) == SV_ST_TV || bGetSignalType(VDP_1) == SV_ST_AV)
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_03, 0xA, SAD_GRA_RATIO1);
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_03, 0x10, SAD_GRA_RATIO2);
    }
}

static void _vDrvDISetDARQuality(UINT8 bPath)
{
    UINT8 u1Index;
    
    switch (bDrvVideoGetTiming(bPath))
    {
        case MODE_1080i_48:
        case MODE_1080i_50:
        case MODE_1080i:
            u1Index = 2;
            break;
        default:
            if ((bGetSignalType(VDP_1) == SV_ST_TV) 
                || (bGetSignalType(VDP_1) == SV_ST_MPEG))
            {
                u1Index = 0;
            }
            else
            {
                u1Index = 1;
            }            
            break;
    }
    
    vDrvDILoadRegTbl(VDP_1, MDDI_DAR_PARAM[u1Index]);
    DiPar.DarPar.u4DarCfg = u4RegRead4B(MCVP_DARE_00);
}

static void _vDrvDISetAdatpive(UINT8 bPath)
{
    UINT32 u4TotalBlk = ((_arMDDiPrm[VDP_1].u2Width>>4)-2)*((_arMDDiPrm[VDP_1].u2Height>>1)-4);

    u4TotalBlk = IS_INTERLACE(VDP_1) ? u4TotalBlk : u4TotalBlk*2;
    u4TotalBlk = MDDI_READ_FLD(VDP_1, MCVP_MULTI_00, DET_SUB) ? (u4TotalBlk>>1) : u4TotalBlk;

    vIO32WriteFldAlign(PSCAN_FWCS_00, u4TotalBlk, TOTAL_BLK); 
}

////////////////////////////////////////////////////////////////////////////////
// Quality ISR 
////////////////////////////////////////////////////////////////////////////////
static void _vDrvDIMutliCadence(void)
{
    UINT16 i, wTest; 

    if ((DiPar.PdPar.u4FldBit == 4) && (DiPar.PdPar.u4FrmBit == 0)) 
    {
        DiPar.PdPar.u2FilmMode = E_DI_FILM_22;
    }    
    else if ((DiPar.PdPar.u4FrmBit == 6) && (DiPar.PdPar.u4FldBit == 6))
    {
        DiPar.PdPar.u2FilmMode = E_DI_FILM_KALEIDOSCOPE;
    }
    else if ((DiPar.PdPar.u4FldBit == 10) && (DiPar.PdPar.u4FrmBit == 10))
    {
        for (i=0; i<10; i++)
        {
            wTest = (DiPar.PdPar.u4FrmMask>>i)&0x3FF;
        
            if (wTest == FW_FILM_FR_SEQ_32)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_32;
                break;
            }
            
            if (wTest == FW_FILM_FR_SEQ_55)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_55;
                break;
            } 
            
            if (wTest == FW_FILM_FR_SEQ_2224)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_2224;
                break;
            }

            if (wTest == FW_FILM_FR_SEQ_2332)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_2332;
                break;
            }

            if (wTest == FW_FILM_FR_SEQ_64)
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
        for (i=0; i<10; i++)
        {
            wTest = (DiPar.PdPar.u4FrmMask>>i)&0x3FF;
        
            if (wTest == FW_FILM_FR_SEQ_32)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_32;
                break;
            }
        }
        
        for (i=0; i<15; i++)
        {
            wTest = (DiPar.PdPar.u4FrmMask>>i)&0x7FFF;
        
            if (wTest == FW_FILM_FR_SEQ_87)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_87;
                break;
            }            
        }
    }
    else
    {
        DiPar.PdPar.u2FilmMode = E_DI_FILM_NONE;
    }
}

static void _vDrvDISetOSD(void)
{
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.u4FldBit ? 1 : 0, DI_DBG_FLDBIT);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.u4FldBit ? 1 : 0, DI_DBG_FRMBIT);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.bUnknowCnt ? 1 : 0, DI_DBG_UNKNOWN);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiSta.PeSta.u4EdgeSum ? 0 : 1, DI_DBG_NOEDGE);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, (DiPar.CsPar.bFilmCnt==8) ? 1 : 0, DI_DBG_CSFILM);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.bFwWeave ? 1 : 0, DI_DBG_FWWEAVE);    
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.IfPar.bMovingCzp ? 1 : 0, DI_DBG_MOVINGCZP);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.IfPar.bFldInv, DI_DBG_FLDINV);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.u2FilmMode, DI_DBG_FILMMODE);    
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.CsPar.bFavorCS, DI_DBG_FAVORCS);    
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.IfPar.bCzpState, DI_DBG_CZPSTATE);     
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.bFilm24To50, DI_DBG_FILM24TO50);    
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.bSmallMo, DI_DBG_SMALL_MO);     
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.bFilmPatch, DI_DBG_FILM_PATCH); 
}

static void _vDrvDIPDSetAdapThld(void)
{
#if MDDI_PD_ADAPTIVE_THRESHOLD

    MDDI_WRITE_FLD(VDP_1, MCVP_FILM_23, DiPar.PdPar.u4AdaptivePxlMotionTh>>1, FR_MO_PXL_DIFF_THL);
    MDDI_WRITE_FLD(VDP_1, MCVP_FILM_02, DiPar.PdPar.u4AdaptivePxlMotionTh, FR_MO_CUR_THL_2);
    MDDI_WRITE_FLD(VDP_1, MCVP_FILM_23, DiPar.PdPar.u4AdaptiveEgMotionTh>>1, R_MIXSAW);
    MDDI_WRITE_FLD(VDP_1, MCVP_FILM_13, DiPar.PdPar.u4AdaptiveEgMotionTh, FI_MO_CUR_THL_2);    
    MDDI_WRITE_FLD(VDP_1, MCVP_FILM_07, DiPar.PdPar.u4AdaptiveFrMoTh, FR_ST_MIN_TH);
    MDDI_WRITE_FLD(VDP_1, MCVP_FILM_08, DiPar.PdPar.u4AdaptiveFrMoTh, FR_MO_MIN_TH); 
    MDDI_WRITE_FLD(VDP_1, MCVP_FILM_07, DiPar.PdPar.u4AdaptiveFrMoTh, FR_ST_MAX_TH);
    MDDI_WRITE_FLD(VDP_1, MCVP_FILM_08, DiPar.PdPar.u4AdaptiveFrMoTh, FR_MO_MAX_TH);     
    MDDI_WRITE_FLD(VDP_1, MCVP_FILM_0C, DiPar.PdPar.u4AdaptiveEgMo32Th, NEW_FI_ST_MAX_TH);
    MDDI_WRITE_FLD(VDP_1, MCVP_FILM_0E, DiPar.PdPar.u4AdaptiveEgMo22Th, NEW_FI_ST_MAX_TH_22);

#endif
}

static void _vDrvDIPDFwPatch(void)
{
#if MDDI_PD_SW_FW_FILM_PATCH //Patch for pulldown.trp
    
    UINT32 u4CurFilmMaskFr32;
    static UINT32 u4PeCount = 0;
    UINT32 u4ASL = u1DrvGetASL();
    UINT32 u4APL = bDrvGetAPL();

    DiPar.PdPar.bFWPatch32FrSeq = (DiPar.PdPar.bFWPatch32FrSeq<<1)|(DiSta.PdSta.u4FrmMotion==0);

    u4CurFilmMaskFr32 = (FW_FILM_FI_SEQ_32 >> ((FW_FILM_CB-1)-DiPar.PdPar.bFWPatchCB32)) & FW_FILM_MASK;

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

    if (((DiSta.PeSta.u4EdgeHis[0] > 0x2100) && (DiSta.PeSta.u4EdgeHis[0] < 0x2600))
        && (bGetSignalType(VDP_1) != SV_ST_TV) 
        && (bGetSignalType(VDP_1) != SV_ST_MPEG)
        && (u4APL > 165) && (u4APL < 190) 
        && (u4ASL > 18) && (u4ASL < 26) 
        && (DiSta.PdSta.bCurFilmMode&E_DI_FILM_32))
    {
        u4PeCount = (u4PeCount < 0x08) ? (u4PeCount+1) : 0x08;
    }
    else
    {
        u4PeCount = 0;
    }

    //LOG(3, "Count %d : APL %d ASL %d Edge0 %x  \n", u4PeCount, u4APL, u4ASL, DiSta.PeSta.u4EdgeHis[0]);
    
    if (((DiPar.PdPar.bFWPatchFi32Cnt > 0x4) 
        && (DiPar.PdPar.bFWPatchFr32Cnt > 0x10)
        && (IS_MPEG(VDP_1))
        && (u4ASL < 0x10))
        // for HQV2.0 Text vertical moving
        || (((IS_SD_HEIGHT(VDP_1)) && (!IS_MPEG(VDP_1)) && (u4PeCount == 8))))
    {    
        MDDI_WRITE_FLD(VDP_1, MCVP_FILM_01, 0, FILM_DET_TYPE);
        DiPar.PdPar.bFilmPatch = 1;
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_FILM_01, 4, FILM_DET_TYPE);
        DiPar.PdPar.bFilmPatch = 0;
    }
#endif
}

static void _vDrvDIPDFwFilmDetetion(void)
{
    UINT16 i, j;
    UINT8 u1FldGain, u1FrmGain;
    UINT16 u2FrmMaskCur, u2FldMaskCur, u2Mask;    
    UINT32 u4FrmSaw = DiSta.PdSta.u4FrmMotion;   
    UINT32 u4FldSaw = DiSta.PdSta.u4EdgMotion; 
    UINT32 u4FrmSeq, u4FrmSeqNew;
    UINT32 u4FldSeq, u4FldSeqNew;    
    static UINT32 u4FrmMoThld = 0, u4FldMoThld = 0; 
    UINT8 u1Film24To50Det = IO32ReadFldAlign(PSCAN_FWFILM_06, DET_24TO50_EN);

    //FW FILM DETECTION 
    UINT8 u1FldGain1 = IO32ReadFldAlign(PSCAN_FWFILM_00, FLD_GAIN1);  
    UINT8 u1FldGain2 = IO32ReadFldAlign(PSCAN_FWFILM_00, FLD_GAIN2);  
    UINT32 u4FldMin = IO32ReadFldAlign(PSCAN_FWFILM_00, FLD_MIN);                                                       
    UINT32 u4FldMax = IO32ReadFldAlign(PSCAN_FWFILM_00, FLD_MAX)<<4;    
    UINT8 u1FrmGain1 = IO32ReadFldAlign(PSCAN_FWFILM_02, FRM_GAIN1);  
    UINT8 u1FrmGain2 = IO32ReadFldAlign(PSCAN_FWFILM_02, FRM_GAIN2);  
    UINT32 u4FrmMin = IO32ReadFldAlign(PSCAN_FWFILM_02, FRM_MIN);                                                       
    UINT32 u4FrmMax = IO32ReadFldAlign(PSCAN_FWFILM_02, FRM_MAX)<<8; 

    //Force exit 
    if (DiPar.PdPar.u4FldBit && DiPar.PdPar.u4FrmBit && (DiPar.PdPar.u4FldBit != DiPar.PdPar.u4FrmBit))
    {
        DiPar.PdPar.u4FldBit = 0;
        DiPar.PdPar.u4FrmBit = 0;   
    }

    if (u4FldSaw > DiPar.PdPar.u4FldThld)                                                                                     
    {          
        DiPar.PdPar.u4FldSeq = (DiPar.PdPar.u4FldSeq<<1) | 1;   
        u4FldMoThld = ((DiPar.PdPar.u4FldBit)&&(DiPar.PdPar.u4FldSeq)) ? (u4FldSaw>>3) : 0;
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
        u4FrmMoThld = ((DiPar.PdPar.u4FrmBit)&&(DiPar.PdPar.u4FrmSeq)) ? (u4FrmSaw>>3) : 0;
        u1FrmGain = u1FrmGain2;
    }                                                                                                                         
    else                                                                                                                      
    {                               
        DiPar.PdPar.u4FrmSeq = (DiPar.PdPar.u4FrmSeq<<1) | 0;  
        u1FrmGain = u1FrmGain1;
    } 

    //Avoid 
    u4FrmMoThld = ((DiPar.PdPar.u4FrmSeq&0xFFFF) == 0) ? 0 : u4FrmMoThld;
    u4FldMoThld = ((DiPar.PdPar.u4FldSeq&0xFFFF) == 0) ? 0 : u4FldMoThld;

    u4FldSeq = DiPar.PdPar.u4FldSeq;                                                                                        
    u4FrmSeq = DiPar.PdPar.u4FrmSeq;        
    DiPar.PdPar.u4FldThld = MIN(u4FldMax, MAX(u4FldMin, MAX((u4FldSaw*u1FldGain)>>4, u4FldMoThld))); 
    DiPar.PdPar.u4FrmThld = MIN(u4FrmMax, MAX(u4FrmMin, MAX((u4FrmSaw*u1FrmGain)>>4, u4FrmMoThld)));  

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
            
            if ((DiPar.PdPar.bFilm24To50 == SV_TRUE) && 				
                ((DiPar.PdPar.bFldCnt > 0x12) || (DiPar.PdPar.u4FldBit != 4)))          
            {               
                DiPar.PdPar.bFilm24To50 = SV_FALSE;             
                LOG(2, "Other Film sequence\n");            
            }
        }                                                                                                                     
        else                                                                                                                  
        {   
            if ((DiPar.PdPar.u4FldBit == 4) && (DiPar.PdPar.bFldCnt == 0x12))
			{
				DiPar.PdPar.bFilm24To50 = u1Film24To50Det ? SV_TRUE : SV_FALSE;
				LOG(2, "This 24 to 50 sequence\n");
			}
			else if ((DiPar.PdPar.u4FldBit != 4) && (DiPar.PdPar.bFldCnt > 0x4))
			{
				DiPar.PdPar.bFilm24To50 = SV_FALSE;
				LOG(2, "Other Film sequence jump\n");
			}
			else
			{
				LOG(2, "Not sure \n");
			}
            
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
        && (DiSta.PeSta.u4EdgeSum == 0))
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

    _vDrvDIMutliCadence();
}

static void _vDrvDIGetPQStatusISR(void)
{
    UINT32 i, u4Heter, u4Homo;

    DiSta.u4IsrIndex = (DiSta.u4IsrIndex+1)&DILOOPMASK;
    
    //============= MA Status ====================
    DiSta.IfSta.u4HEdgeCnt = MDDI_READ_FLD(VDP_1, MCVP_FUSION_19, IF_HOR_EDGE_CNT);
    DiSta.IfSta.u4VEdgeCnt = MDDI_READ_FLD(VDP_1, MCVP_FUSION_19, IF_VER_EDGE_CNT);
    DiSta.IfSta.u4EdgeSum = DiSta.IfSta.u4VEdgeCnt + DiSta.IfSta.u4HEdgeCnt;          
    DiSta.IfSta.u4AvgCurY = MDDI_READ_FLD(VDP_1, MCVP_FUSION_16, IF_AVG_Y);
    DiSta.IfSta.u4AvgCurCb = MDDI_READ_FLD(VDP_1, MCVP_FUSION_16, IF_AVG_CB);
    DiSta.IfSta.u4AvgCurCr = MDDI_READ_FLD(VDP_1, MCVP_FUSION_16, IF_AVG_CR);
    DiSta.IfSta.u4AvgPreY = MDDI_READ_FLD(VDP_1, MCVP_FUSION_17, IF_AVG_Y_PREV);
    DiSta.IfSta.u4AvgPreCb = MDDI_READ_FLD(VDP_1, MCVP_FUSION_17, IF_AVG_CB_PREV);
    DiSta.IfSta.u4AvgPreCr = MDDI_READ_FLD(VDP_1, MCVP_FUSION_17, IF_AVG_CR_PREV);
    DiSta.IfSta.u4HeterUp = MDDI_READ_FLD(VDP_1, MCVP_FUSION_12, IF_PD_SUM_HETER_UP);
    DiSta.IfSta.u4HomoUp = MDDI_READ_FLD(VDP_1, MCVP_FUSION_13, IF_PD_SUM_HOMO_UP);
    DiSta.IfSta.u4HeterDn = MDDI_READ_FLD(VDP_1, MCVP_FUSION_14, IF_PD_SUM_HETER_DN);
    DiSta.IfSta.u4HomoDn = MDDI_READ_FLD(VDP_1, MCVP_FUSION_15, IF_PD_SUM_HOMO_DN);
    DiSta.IfSta.u4SawInv = MDDI_READ_FLD(VDP_1, MCVP_FUSION_0B, IF_SIMPLE_SAW_INVERT);
    DiSta.IfSta.u4SawNom = MDDI_READ_FLD(VDP_1, MCVP_FUSION_0B, IF_SIMPLE_SAW_NORMAL);    
    
    u4Heter = DiSta.IfSta.u4HeterUp + DiSta.IfSta.u4HeterDn;
    u4Homo = DiSta.IfSta.u4HomoUp + DiSta.IfSta.u4HomoDn;
    DiSta.IfSta.u4HeterDiff = DIFF(u4Heter, DiSta.IfSta.u4Heter);
    DiSta.IfSta.u4HomoDiff = DIFF(u4Homo, DiSta.IfSta.u4Homo);
    DiSta.IfSta.u4Heter = u4Heter;
    DiSta.IfSta.u4Homo = u4Homo;    

    //============= BE Status ====================
    DiSta.PeSta.u4AliasCnt = MDDI_READ_FLD(VDP_1, MCVP_PE_12, ALIAS_CNT);
    DiSta.PeSta.u4EdgeSum = 0;
    DiSta.PeSta.u4EdgeHis[0] = u2RegRead2B(MCVP_PE_0A);
    DiSta.PeSta.u4EdgeHis[1] = u2RegRead2B(MCVP_PE_0A+2);
    
    for (i = 2; i < DI_BE_HIST_BIN; i++)
    {        
        DiSta.PeSta.u4EdgeHis[i] = u2RegRead2B(MCVP_PE_0A+2*i);
        DiSta.PeSta.u4EdgeSum += DiSta.PeSta.u4EdgeHis[i];    
    }

    //============= FILM Status ====================
    DiSta.PdSta.u4FrmMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
    DiSta.PdSta.u4FldMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_10, FD_32_SAW_FI_A_B_SAW_Q);
    DiSta.PdSta.u4EdgMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
    DiSta.PdSta.bBlkMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
    DiSta.PdSta.bCurFilmMode = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0F, FILM_STATUS);
}

static void _vDrvDISetIFQualityISR(void)
{        
    UINT8 u1Weighting = (bGetSignalType(VDP_1) == SV_ST_TV) ? 6 : 8;
    
    u1Weighting = DiPar.PdPar.bSmallMo ? 0 : u1Weighting;
    
    if ((((DiSta.PdSta.bCurFilmMode&E_DI_FILM_32) && (DiPQMode.b32Film)) 
        || ((DiSta.PdSta.bCurFilmMode&E_DI_FILM_22) && (DiPQMode.b22Film))) 
        && (DiPar.PdPar.bFilm24To50 == SV_FALSE)
        && (DiPar.PdPar.u2FilmMode <= E_DI_FILM_22))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_23, DiPQMode.bFilmOnOff_Y ? u1Weighting : 0, IF_FILM_Y_W);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_23, DiPQMode.bFilmOnOff_C ? u1Weighting : 0, IF_FILM_C_W);

        // Film mode CUE should not depend on Chroma motion level : 5396/89 not fixed, 8227 fixed
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_21, DiPQMode.bFilmOnOff_C ? 0 : 8, IF_MAX_MOTION_C);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_23, 0, IF_FILM_Y_W);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_23, 0, IF_FILM_C_W);        
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_21, 8, IF_MAX_MOTION_C);
    }       
    
    //Weave field 
    if (IO32ReadFldAlign(PSCAN_FWFILM_06, FILED_WEAVE_EN)
        && (DiPar.PdPar.u2FilmMode > E_DI_FILM_22) 
        && (DiPar.PdPar.bFwBadEdit == 0)
        && (DiPQMode.bFilmOnOff_MultiCadence == SV_ON))
    {
        DiPar.PdPar.bFwWeave = 1;
        vRegWriteFldMulti(MCVP_FUSION_20, P_Fld(1, IF_FIX_MOTION_Y) | 
            P_Fld(0, IF_FORCE_MOTION_Y) | P_Fld(1, IF_FIX_B_WEIGHT) | 
            P_Fld((DiPar.PdPar.u4FldSeq&1) ? 0 : 8, IF_FORCE_B_WEIGHT));
    }
    else      
    {
        DiPar.PdPar.bFwWeave = 0;
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_20, 0, IF_FIX_MOTION_Y);   
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_20, (DiSta.PdSta.bCurFilmMode&0x3)!=0 ? 0 : 1, IF_FIX_B_WEIGHT);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_20, 4, IF_FORCE_B_WEIGHT);          
    }

    vIO32Write4B(PSCAN_FW_ADAPTIVE_FILM_0A, DiPar.PdPar.u4FldSeq);
    vIO32Write4B(PSCAN_FW_ADAPTIVE_FILM_0B, DiPar.PdPar.u4FrmSeq);
}

static void _vDrvDISetIFQualityISR_Ex(void)
{        
    // Turn off weave and use fusion result to fix multicadence top line problem. 
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_20, 0, IF_FIX_MOTION_Y);   
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_20, (DiSta.PdSta.bCurFilmMode&0x3)!=0 ? 0 : 1, IF_FIX_B_WEIGHT);
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_20, 4, IF_FORCE_B_WEIGHT); 
}

static void _vDrvDISetPEQualityISR(void)
{
    
}

static void _vDrvDISetCSQualityISR(void)
{
    //FORCE MV = 0 TO PREVENT MV Remains
    MDDI_WRITE_FLD(VDP_1, MCVP_CS_21, 
        ((DiSta.IfSta.u4Homo == 0) && (DiSta.IfSta.u4HomoDiff == 0)) ? SV_ON : SV_OFF, FORCE_ME_IN_EN);
}

static void _vDrvDISetDARQualityISR(void)
{
        //Hw Film mode turn off Dare 
    if (((DiPQMode.bFilmOnOff_Y == SV_ON)
        && (((DiSta.PdSta.bCurFilmMode&E_DI_FILM_32) && (DiPQMode.b32Film)) 
        || ((DiSta.PdSta.bCurFilmMode&E_DI_FILM_22) && (DiPQMode.b22Film))))
        //MultiCadense Film mode turn off Dare 
        || ((DiPQMode.bFilmOnOff_MultiCadence == SV_ON) 
        && ((DiPar.PdPar.u2FilmMode > E_DI_FILM_22) && (DiPar.PdPar.u2FilmMode < E_DI_FILM_KALEIDOSCOPE))))  
    {       
        //Motion level should be 0 when film mode 
        MDDI_WRITE_FLD(VDP_1, MCVP_DARE_00, 0, DARE_LINE_FLICKER_EN);                

        if (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, DAR_OFF_IN_FILM))
        {
            vRegWriteFldMulti(MCVP_DARE_00, P_Fld(0, DARE_VD_SMOOTH_EN) | P_Fld(0, DARE_VS_SMOOTH_EN) 
                | P_Fld(0, DARE_CONT_SAW_EN) | P_Fld(0, DARE_SINGLE_SAW_EN) | P_Fld(0, DARE_LUMA_JAGGY_EN));   
        }
    }
    else
    {
        vRegWrite4B(MCVP_DARE_00, DiPar.DarPar.u4DarCfg);
    }
}

static void _vDrvDISetPDQualityISR(void)
{
    _vDrvDIPDSetAdapThld();
    _vDrvDIPDFwPatch();
    _vDrvDIPDFwFilmDetetion();
}
    
#ifndef NDEBUG
static void _vDrvDIEventISR(void)
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
#endif

static void _vDrvDIFieldInverse(void)
{
    static UINT32 u4Count = 0;    
    UINT32 u4StaSawInv, u4StaSawNom;        
    UINT32 u4Max = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_01, FLD_INV_MAX_THD);
    
    if (DiPar.IfPar.bFldInv == 0)
    {        
        u4StaSawInv = DiSta.IfSta.u4SawInv;
        u4StaSawNom = DiSta.IfSta.u4SawNom;    
    }
    else
    {        
        u4StaSawInv = DiSta.IfSta.u4SawNom;  
        u4StaSawNom = DiSta.IfSta.u4SawInv;  
    }
      
    if ((u4StaSawInv < 0x4f) && (u4StaSawNom > 0x4f)
        && ((u4StaSawNom*2) > (u4StaSawInv*5)))
    {
        u4Count = (u4Count < u4Max) ? (u4Count + 1) : u4Max;
    }
    else
    {
        u4Count = (u4Count > 0) ? (u4Count - 1) : 0;
    }

    if (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_00, FLD_INV_EN))
    {
        if (u4Count > IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_01, FLD_INV_IN_THD))
        {
            DiPar.IfPar.bFldInv = 1;
        }        
        else if (u4Count < IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_01, FLD_INV_OUT_THD))
        {
            DiPar.IfPar.bFldInv = 0;
        }

        //LOG(2, "u4Count %d  =>  %d \n", u4Count, DiPar.IfPar.bFldInv);
    }
    else 
    {
        DiPar.IfPar.bFldInv = 0;
    } 
    
    vDrvDISetFieldInverse(VDP_1, DiPar.IfPar.bFldInv);    
}

static void _vDrvDICzpAdaptive(void)
{
    UINT32 u4State;    
    UINT32 u4HorSm, u4VerSm, u4Base3D;
    UINT32 u4HorCostEN, u4HorCostTH, u4Peak;    
    static UINT32 u4Count = 0;        
    UINT32 u4Min = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_01, CZP_AD_MIN_THD);
    
    if ((DiSta.CsSta.u4MultibusrtCnt > 0x1800)
        && (DiSta.IfSta.u4SawNom < 0x4f)
        && (DiSta.IfSta.u4SawNom > 0x28)
        && (DiSta.IfSta.u4SawInv > 0x4f))
    {
        u4Count = (u4Count < 15) ? (u4Count + 1) : 15;
    }
    else
    {
        u4Count = (u4Count > 0) ? (u4Count - 1) : 0;
    }

    u4State = (u4Count < u4Min) ? 0 : ((u4Count > (u4Min + 8)) ? 8 : (u4Count - u4Min));

    // Still CZP vertical horazontal line
    if (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_00, CZP_AD_EN) == 0)
    {
        u4State = 0;
    }
    DiPar.IfPar.bCzpState = (UINT8)u4State;
    
    u4HorSm = (0x3 * ( 8 - u4State ) + 17 * u4State +4) >> 3;
    u4VerSm = (0x14 * ( 8 - u4State ) + 82 * u4State + 4) >> 3;
    u4Base3D = (0x8 * ( 8 - u4State ) + 91 * u4State + 4) >> 3;
    u4HorCostEN = (u4State > 0 ) ? 1 : 0;
    u4HorCostTH = (0 * ( 8 - u4State ) + 63 * u4State + 4) >> 3;
    u4Peak = ( 0x4 * ( 8 - u4State ) + 0 * u4State + 4) >> 3;

    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_09, u4HorSm, IF_HOR_SMALL_VAR_TH);
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_09, u4VerSm, IF_VER_SMALL_VAR_TH);
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_03, u4Base3D, IF_BASE_COST_3D);
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_01, u4HorCostEN, IF_HOR_EDGE_COST_ADD);
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_02, u4HorCostTH, IF_HOR_EDGE_COST_TH);
    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_07, u4Peak, IF_RA_PEAK_MULT);

    // Moving CZP using bob mode 
    if (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_00, MOVING_CZP_BOB_EN)
        && (DiSta.PeSta.u4AliasCnt > 0x1000)
        && ((DiSta.CsSta.u1GmvX != 0) || (DiSta.CsSta.u1GmvY != 0)))
    {
        DiPar.IfPar.bMovingCzp = 1;
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_21, 8, IF_MIN_MOTION_C);         
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_21, 8, IF_MIN_MOTION_Y);  
        MDDI_WRITE_FLD(VDP_1, MCVP_PE_00, 1, PE_FORCE_USER_DIR);  
    }
    else
    {
        DiPar.IfPar.bMovingCzp = 0;
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_21, 0, IF_MIN_MOTION_C);         
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_21, 0, IF_MIN_MOTION_Y);  
        MDDI_WRITE_FLD(VDP_1, MCVP_PE_00, 0, PE_FORCE_USER_DIR);  
    }
}

static void _vDrvDIFavorCS(void)
{
    static UINT16 _u2NonZeroCnt[5] = {0, 0, 0, 0, 0};
    static UINT32 u4Index = 0;
    static UINT32 u4Count = 0;
    UINT32 u4Cost, u4Stable = 0;

    UINT32 u4HistThd = ((UINT32)DiSta.CsSta.u2HistCnt*IO32ReadFldAlign(PSCAN_FWCS_03, HIST_DIFF_RATIO))>>8;
    UINT32 u4Max = IO32ReadFldAlign(PSCAN_FWCS_03, COUNT_MAX_BND);
    UINT32 u4ExeThd = IO32ReadFldAlign(PSCAN_FWCS_03, COUNT_EXE_THD);

    _u2NonZeroCnt[u4Index] = DiSta.CsSta.u2NonZeroCnt;

    u4Stable |= ((_u2NonZeroCnt[0] == _u2NonZeroCnt[1])
                    && (_u2NonZeroCnt[0] == _u2NonZeroCnt[2])
                    && (_u2NonZeroCnt[0] == _u2NonZeroCnt[3])) ? 1 : 0;

    u4Stable |= ((_u2NonZeroCnt[1] == _u2NonZeroCnt[2])
                    && (_u2NonZeroCnt[1] == _u2NonZeroCnt[3])
                    && (_u2NonZeroCnt[1] == _u2NonZeroCnt[4])) ? 1 : 0;

    u4Stable |= ((_u2NonZeroCnt[2] == _u2NonZeroCnt[0])
                    && (_u2NonZeroCnt[2] == _u2NonZeroCnt[3])
                    && (_u2NonZeroCnt[2] == _u2NonZeroCnt[4])) ? 1 : 0;

    u4Stable |= ((_u2NonZeroCnt[3] == _u2NonZeroCnt[0])
                    && (_u2NonZeroCnt[3] == _u2NonZeroCnt[1])
                    && (_u2NonZeroCnt[3] == _u2NonZeroCnt[4])) ? 1 : 0;

    u4Stable |= ((_u2NonZeroCnt[4] == _u2NonZeroCnt[0])
                    && (_u2NonZeroCnt[4] == _u2NonZeroCnt[1])
                    && (_u2NonZeroCnt[4] == _u2NonZeroCnt[2])) ? 1 : 0;

    if (((u4Stable && !(DiSta.CsSta.u1GmvValid && (DiSta.CsSta.u1GmvX == 0) && (DiSta.CsSta.u1GmvY == 0))) 
        || (DiSta.CsSta.u1GmvValid && ((DiSta.CsSta.u1GmvX != 0) || (DiSta.CsSta.u1GmvY != 0))))
        && (DiSta.CsSta.u4HistDiffAcc < u4HistThd) 
        && (bDrvGetFilmStatus() == E_DI_FILM_NONE)
        && ((_u2NonZeroCnt[u4Index] > 1) || DiSta.CsSta.u1GmvValid)
        && (_u2NonZeroCnt[u4Index] < 13))
    {
        u4Count = (u4Count < u4Max) ? (u4Count + 1) : u4Max;
    }
    else
    {
        u4Count = (u4Count > 0) ? (u4Count - 1) : 0;
    }
    
    DiPar.CsPar.bFavorCS = (u4Count > u4ExeThd) ? (((u4Count - u4ExeThd) > 8) ? 8 : (u4Count - u4ExeThd)) : 0;

    u4Cost = MAX(MIN((9-DiPar.CsPar.bFavorCS), 8), 2);

    if (IO32ReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_EN))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, 8-DiPar.CsPar.bFavorCS, IF_W_SAWTOOTH_MC);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, u4Cost, IF_W_SAWTOOTH_STILL);
    }
    else     
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, 8, IF_W_SAWTOOTH_MC);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, 8, IF_W_SAWTOOTH_STILL);
    }
    
    vIO32WriteFldAlign(PSCAN_FWCS_02, DiPar.CsPar.bFavorCS, FAVOR_CS_STATE);
    
    u4Index = (u4Index < 4) ? (u4Index+1) : 0;
}

static void _vDrvIFAdaptive(UINT8 bPath)
{
    if (IS_SD_HEIGHT(bPath) && (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF))
    {
        _vDrvDIFieldInverse();
        _vDrvDICzpAdaptive();          
    }
}

static void _vDrvPEAdaptive(UINT8 bPath)
{

}

static void _vDrvDARAdaptive(UINT8 bPath)
{
#if MDDI_DAR_ADAPTIVE_VERTICAL_FILTER
    INT32 i4MoPxlLevel, i4MoSumLevel;
    UINT32 u4Min, u4Max; 
    static UINT8 u1MotionIdx, u1TargetValue; 
    static UINT16 u2PreAlpha;
    static UINT32 u4MoPxl[5], u4MoSum[5];

    UINT8 u1MaxStrMin = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, DAR_V_MAX_MIN);  
    UINT8 u1MaxStrMax = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, DAR_V_MAX_MAX);  
    UINT16 u2MoPxlCoring = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_01, DAR_V_MOTION_PXL_CORING);  
    UINT16 u2MoSumCoring = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_01, DAR_V_MOTION_SUM_CORING);  
    UINT8 u1MoPxlShift = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, DAR_V_MOTION_PXL_SHIFT);  
    UINT8 u1MoSumShift = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, DAR_V_MOTION_SUM_SHIFT);  
    UINT8 u1FilterStrDelay = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, DAR_V_FILTER_STR_DELAY_SHIFT);

    u1MotionIdx = (u1MotionIdx < 4) ? (u1MotionIdx + 1) : 0;
    u4MoPxl[u1MotionIdx] = DiSta.PdSta.u4FrmMotion;
    u4MoSum[u1MotionIdx] = DiSta.IfSta.u4Homo;
               
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

    DiPar.PdPar.bSmallMo = (i4MoSumLevel < IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_0C, SMALL_MO_THLD)) ? 1 : 0;

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
    
    MDDI_WRITE_FLD(SV_VP_MAIN, MCVP_DARE_10, u1TargetValue, DAR_V_MAX_ALPHA);

#if 0    // disable since the noise level is not correct now
    UINT8 u1VTBLInputForNoise = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_03, DAR_V_TBL_IN_FOR_NOISE);  
    UINT8 u1VTBLOutputForNoise = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_03, DAR_V_TBL_OUT_FOR_NOISE);  
    static INT32 i4PreNoiseLevel = 0xFFFFFFFF;
    static UINT8 u1TargetFilter; 
    UINT32 u1CurNoiseLvl; 

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
            (u1TargetFilter - (INT32)NM_LEVEL1_3) * u1VTBLInputForNoise/(NM_LEVEL8-NM_LEVEL1_3)
            , u1VTBLInputForNoise);        
        
        MDDI_WRITE_FLD(SV_VP_MAIN, MCVP_DARE_11, 0x08 + u1CurNoiseLvl, DAR_V_VARIATION_I1);
        MDDI_WRITE_FLD(SV_VP_MAIN, MCVP_DARE_11, 0x10 + u1CurNoiseLvl, DAR_V_VARIATION_I2);

        //Filter Output 
        u1CurNoiseLvl = MIN(
            (u1TargetFilter - (INT32)NM_LEVEL1_3) * u1VTBLOutputForNoise/(NM_LEVEL8-NM_LEVEL1_3)
            , u1VTBLOutputForNoise);        

        MDDI_WRITE_FLD(SV_VP_MAIN, MCVP_DARE_11, u1CurNoiseLvl, DAR_V_VARIATION_O2);   
    }    
#endif      
#endif    
}

static void _vDrvCSAdaptive(UINT8 bPath)
{
    UINT32 u4ZeroRatio;
    UINT32 u4GmvCnt = 0x08, u4GmvRatio = 0x14, u4FbGmvRatio = 0x13;
    UINT32 u4TotalBlk = IO32ReadFldAlign(PSCAN_FWCS_00, TOTAL_BLK);  
    UINT32 u4EdgeRatio = IO32ReadFldAlign(PSCAN_FWCS_00, GMV_EDGE_RATIO);
    UINT32 u4ReduceRatio = IO32ReadFldAlign(PSCAN_FWCS_01, REDUCE_RATIO);    
    UINT32 u4HistCnt = DiSta.CsSta.u2HistCnt;
    UINT32 u4EdgeDiff = 0x90;
    UINT32 u4LumaAvg = (DiSta.IfSta.u4AvgCurY + DiSta.IfSta.u4AvgPreY)>>3;
    
        // for RF no signal
    if (((bGetSignalType(VDP_1) == SV_ST_TV) 
        && (bDrvVideoSignalStatus(VDP_1) == SV_VDO_NOSIGNAL))
        // for RF & CVBS weak signal
		|| ((bGetVideoDecType(VDP_1) == SV_VD_TVD3D) 
        && (bTvdSignalStrengthLevel(1) < CS_DISABLE_WEAK_SIGNAL))
        // for HQV 2.0 SD 
        || (IS_SD_HEIGHT(VDP_1) && (u1DrvGetASL() < 0x10) &&
            ((DiSta.PeSta.u4EdgeSum > 0x280) && (DiSta.PeSta.u4EdgeSum < 0x3C0)) &&
            ((DiSta.IfSta.u4HEdgeCnt>>2) > DiSta.IfSta.u4VEdgeCnt)))
    {
        _vDrvSetCsOnOff(VDP_1, SV_OFF);
    }
    else
    {
        _vDrvSetCsOnOff(VDP_1, DiPar.CsPar.bCsOnOff&&(!DiPar.bForceSceneChange));
    }
        
    if ((u4TotalBlk != 0) && (u4HistCnt != 0))
    {    
        u4GmvRatio = 0x14;
        u4FbGmvRatio = 0x13;
        u4GmvCnt = (DiSta.CsSta.u2EdgeCnt*u4EdgeRatio)/u4TotalBlk;
        u4ZeroRatio = (MV_HIST(0, 0)*256)/u4HistCnt;
                
        if ((u4ZeroRatio > IO32ReadFldAlign(PSCAN_FWCS_01, ZERO_RATIO))
            && (DiSta.CsSta.u2NonZeroCnt < IO32ReadFldAlign(PSCAN_FWCS_01, NON_ZERO_CNT)))
        {
            u4GmvCnt = (u4GmvCnt*u4ReduceRatio)>>8;
            u4GmvRatio = (u4GmvRatio*u4ReduceRatio)>>8;
            u4FbGmvRatio = (u4FbGmvRatio*u4ReduceRatio)>>8;
        }
        
        //LOG(6, "NonZero %d  ZeroRatio %d  GmvCnt %d  GmvRatio %d  FbGmvRatio %d  \n", 
        //        DiSta.CsSta.u2NonZeroCnt, u4ZeroRatio, u4GmvCnt, u4GmvRatio, u4FbGmvRatio);
    }    
    
    MDDI_WRITE_FLD(VDP_1, MCVP_CS_16, u4GmvCnt, GMV_CNT_THD);  
    MDDI_WRITE_FLD(VDP_1, MCVP_CS_0C, u4GmvRatio, GMV_CNT_RATIO); 
    MDDI_WRITE_FLD(VDP_1, MCVP_CS_23, u4FbGmvRatio, NON_FB_GMV_RATIO); 

    // Level dependent threshold    
    u4LumaAvg = (u4LumaAvg<0x20) ? 0 : (u4LumaAvg-0x20);
    u4EdgeDiff = 0x8 + u4LumaAvg*2;
    u4EdgeDiff = (u4EdgeDiff > 0x90) ? 0x90 : u4EdgeDiff;
    MDDI_WRITE_FLD(VDP_1, MCVP_CS_02, u4EdgeDiff, VER_DIFF_THD);  
    MDDI_WRITE_FLD(VDP_1, MCVP_CS_02, u4EdgeDiff, HOR_DIFF_THD);     
    
#if MDDI_CS_FW_FILM_DISABLE_CS    
    // Special case for moving cross hatch  MVY = 2
    if (((DiSta.PeSta.u4EdgeSum == 0) && (u1DrvGetASL() < 0x10) 
        && ((DiSta.CsSta.u1GmvY == 0x2) || (DiSta.CsSta.u1GmvY == 0xE)))
        || (!IS_INTERLACE(VDP_1)))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_17, 0x2, FORCE_CS_FILMLIKE);
    }
    else if (DiPar.PdPar.bFwAccCnt > 0x10) //If current FW Film Detected, Disable CS
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_17, 0x3, FORCE_CS_FILMLIKE);//FORCE CURRENT FILM FOR CS
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_CS_17, 0x0, FORCE_CS_FILMLIKE);
    }
#endif

    _vDrvDIFavorCS();
}

static void _vDrvPDAdaptive(UINT8 bPath)
{
#if MDDI_PD_ADAPTIVE_THRESHOLD

    UINT32 u4MaxFrMo, u4MaxEgMo;
    UINT32 u4FrPreMin, u4EgMaxMinMo;
    UINT32 u4FrMoFilterTh, u4EgMoFilterTh;
    static UINT32 u4FrMo[4], u4EgMo[4];
    static UINT32 u4Index = 0;
    
    //Subtitle Detection
    static UINT32 u4EgMin[2];
    static UINT32 u4EgMinIndex = 0;
        
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
    
    //Adaptive Film Mode Parameter        
    u4FrMo[u4Index] = DiSta.PdSta.u4FrmMotion;
    u4EgMo[u4Index] = DiSta.PdSta.u4EdgMotion;
    
    u4MaxFrMo = MAX(u4FrMo[(u4Index)&3], MAX(u4FrMo[(u4Index-1)&3], u4FrMo[(u4Index-2)&3]));
    u4MaxEgMo = MAX(u4EgMo[(u4Index)&3], MAX(u4EgMo[(u4Index-1)&3], u4EgMo[(u4Index-2)&3]));    

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

    
    if ((u4EgMo[(u4Index-1)&3] <= (u4EgMo[(u4Index-0)&3]>>1))
        || (u4EgMo[(u4Index-1)&3] <= (u4EgMo[(u4Index-2)&3]>>1)))
    {
        u4EgMin[u4EgMinIndex] = u4EgMo[(u4Index-1)&3];
        u4EgMinIndex = (u4EgMinIndex+1)&1;
    }
    
    u4EgMaxMinMo = MAX(u4EgMin[0], u4EgMin[1]);          
    u4EgMaxTh32 = MIN(MAX((u4EgMaxMinMo*u4SubTitleRate)>>2, u4EgMinTh), u4EgMaxTh32);
    u4EgMaxTh22 = MIN(MAX((u4EgMaxMinMo*u4SubTitleRate)>>2, u4EgMinTh), u4EgMaxTh22);

    //Frame Static Max
    u4FrPreMin = MIN(u4FrMo[(u4Index)&3],MIN(u4FrMo[(u4Index-1)&3], u4FrMo[(u4Index-2)&3]));

    u4FrMaxTh = MIN(MAX((u4FrPreMin*u4SubTitleRate)>>2,u4FrMinTh), u4FrMaxTh);

    // Adaptive Motion Filter to remove Noise        
    if( u4MaxFrMo <= (1<<u4FrMoFilterBit)) 
    {
        u4FrMoFilterTh = u4FrMoFilterThLow +((u4FrMoFilterThHigh - u4FrMoFilterThLow) * u4MaxFrMo >>u4FrMoFilterBit);
    }
    else
    {
        u4FrMoFilterTh = u4FrMoFilterThHigh;
    }               

    if( DiPar.PdPar.u4AdaptivePxlMotionTh > u4FrMoFilterTh)
    {
        DiPar.PdPar.u4AdaptivePxlMotionTh--;            
    }
    else if( DiPar.PdPar.u4AdaptivePxlMotionTh < u4FrMoFilterTh)
    {
        DiPar.PdPar.u4AdaptivePxlMotionTh++;
    }            

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
    u4EgMinTh = MIN(u4EgMinTh,u4MaxEgMo>>1);
    DiPar.PdPar.u4AdaptiveEgMo22Th = MIN(MAX(u4MaxEgMo>>u4EgRate22, u4EgMinTh), u4EgMaxTh22);            

    u4Index= (u4Index+1)&3;        
#endif
}

static void _vDrvDIInitSwRegister(void)
{      
    //FW Film    
    vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x10, FLD_MAX);
    vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x20, FLD_MIN);
    vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x30, FLD_GAIN1);
    vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x08, FLD_GAIN2);    
    vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MAX);
    vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MIN);
    vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x28, FRM_GAIN1);
    vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x06, FRM_GAIN2);
    vIO32WriteFldAlign(PSCAN_FWFILM_06, 0x01, FILED_WEAVE_EN);   
    vIO32WriteFldAlign(PSCAN_FWFILM_06, 0x01, DET_24TO50_EN);   

    //Adaptive IF     
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 0x00, MOVING_CZP_BOB_EN);       
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 0x01, FLD_INV_EN);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 0x01, CZP_AD_EN);    
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_01, 0x08, FLD_INV_IN_THD);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_01, 0x04, FLD_INV_OUT_THD);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_01, 0x0C, FLD_INV_MAX_THD);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_01, 0x04, CZP_AD_MIN_THD);

    //Adaptive CS    
    vIO32WriteFldAlign(PSCAN_FWCS_00, 0xF0, Y4_RATIO); 
    vIO32WriteFldAlign(PSCAN_FWCS_00, 0x30, GMV_EDGE_RATIO);   
    vIO32WriteFldAlign(PSCAN_FWCS_01, 0xA0, REDUCE_RATIO);   
    vIO32WriteFldAlign(PSCAN_FWCS_01, 0x04, NON_ZERO_CNT);  
    vIO32WriteFldAlign(PSCAN_FWCS_01, 0x40, ZERO_RATIO);  
    vIO32WriteFldAlign(PSCAN_FWCS_01, 0x01, ZERO_THLD);  
    vIO32WriteFldAlign(PSCAN_FWCS_02, 0x01, FAVOR_CS_EN);
    vIO32WriteFldAlign(PSCAN_FWCS_03, 0x30, HIST_DIFF_RATIO); 
    vIO32WriteFldAlign(PSCAN_FWCS_03, 0x02, COUNT_EXE_THD); 
    vIO32WriteFldAlign(PSCAN_FWCS_03, 0x0C, COUNT_MAX_BND); 

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
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x06, EG_MO_FILTER_L);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x20, EG_MO_FILTER_H);        
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x0A, EG_MO_LARGE_BIT); 
    
    //Motion Adaptive DAR
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, 0x04, DAR_V_MAX_MIN);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, 0x10, DAR_V_MAX_MAX);     
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, 0x00, DAR_OFF_IN_FILM); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_01, 0x20, DAR_V_MOTION_PXL_CORING);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_01, 0x2000, DAR_V_MOTION_SUM_CORING);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, 0x07, DAR_V_MOTION_PXL_SHIFT);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, 0x0C, DAR_V_MOTION_SUM_SHIFT); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, 0x05, DAR_V_FILTER_STR_DELAY_SHIFT); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_03, 0x10 ,DAR_V_TBL_IN_FOR_NOISE); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_03, 0x10 ,DAR_V_TBL_OUT_FOR_NOISE);             
}

void vDrvDIGetCSStatusISR(void)
{
    UINT32 i, u4DirtyCnt = 0;
    
    //============= CS Status ====================
    DiSta.CsSta.u1GmvValid = MDDI_READ_FLD(VDP_1, MCVP_CS_28, GMV_VALID);
    DiSta.CsSta.u1GmvX = MDDI_READ_FLD(VDP_1, MCVP_CS_28, GMV_MVX);
    DiSta.CsSta.u1GmvY = MDDI_READ_FLD(VDP_1, MCVP_CS_28, GMV_MVY);
    DiSta.CsSta.u2HistCnt = MDDI_READ_FLD(VDP_1, MCVP_CS_27, HIST_CNT);
    DiSta.CsSta.u2HistMax = MDDI_READ_FLD(VDP_1, MCVP_CS_27, STA_MAX_CNT1);
    DiSta.CsSta.u2EdgeCnt = MDDI_READ_FLD(VDP_1, MCVP_CS_18, STA_EDGE_CNT);
    DiSta.CsSta.u2NonZeroCnt = MDDI_READ_FLD(VDP_1, MCVP_CS_29, CS_NON_ZERO_CNT);
    DiSta.CsSta.u4HistDiffAcc = MDDI_READ_FLD(VDP_1, MCVP_CS_2A, HIST_DIFF_ACC);
    DiSta.CsSta.u4FieldSawRank = (DiSta.CsSta.u4FieldSawRank<<4) | MDDI_READ_FLD(VDP_1, MCVP_CS_0C, FIELD_SAW_RANK);
    DiSta.CsSta.u4MultibusrtCnt = MDDI_READ_FLD(VDP_1, MCVP_CS_18, STA_MULTIBURST_CNT);

	MDDI_WRITE_FLD(VDP_1, MCVP_CS_11, 1, HIST_STA_AUTO_READ);
	MDDI_WRITE_FLD(VDP_1, MCVP_CS_11, 0, HIST_STA_RADDR_INI);
	MDDI_WRITE_FLD(VDP_1, MCVP_CS_11, SV_ON, SET_HIST_STA_RADDR);
    
    for (i = 0; i < DI_CS_MV_HIST_BIN/2; i++)
    {
        while (u4RegRead4B(MCVP_CS_13)&0x80000000)  
        {
            if (u4DirtyCnt++ > DI_CS_MV_HIST_BIN)
            {
                DiSta.CsSta.u1MvReadValid = 0;
                LOG(4, "MV read error %d \n", u4DirtyCnt);
                return;
            }
        }
        
        DiSta.CsSta.u4MVHist[i] = u4RegRead4B(MCVP_CS_12);
    }
    DiSta.CsSta.u1MvReadValid = 1;
    
    DiSta.CsSta.bAllBurst = MDDI_READ_FLD(VDP_1, MCVP_CS_11, PRE_WHOLE_MULTIBURST);
    DiSta.CsSta.u2SmtCnt = MDDI_READ_FLD(VDP_1, MCVP_CS_26, STA_SMT_CNT);
    DiSta.CsSta.u2SmtCntROI = MDDI_READ_FLD(VDP_1, MCVP_CS_26, STA_SMT_CNT_ROI);
    DiSta.CsSta.u1SmtDeconf = MDDI_READ_FLD(VDP_1, MCVP_CS_28, STA_SMT_DECONF);
    DiSta.CsSta.u1MvDeconf = MDDI_READ_FLD(VDP_1, MCVP_CS_28, STA_MV_DECONF);
    DiSta.CsSta.u4SawAB = MDDI_READ_FLD(VDP_1, MCVP_CS_1D, STA_SAW_AB_CNT);
    DiSta.CsSta.u4SawBC = MDDI_READ_FLD(VDP_1, MCVP_CS_1E, STA_SAW_BC_CNT);    
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

// Add for Vestel --- pingan 0606
void vDrvDISetFilmModeParam(void)
{
    //FrontEnd Filter
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, wReadQualityTable(QUALITY_DI_FILM_FR_MO_FILTER_L), FR_MO_FILTER_L);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, wReadQualityTable(QUALITY_DI_FILM_FR_MO_FILTER_H), FR_MO_FILTER_H);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, wReadQualityTable(QUALITY_DI_FILM_FR_MO_LARGE_BIT), FR_MO_LARGE_BIT);

    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, wReadQualityTable(QUALITY_DI_FILM_EG_MO_FILTER_L), EG_MO_FILTER_L);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, wReadQualityTable(QUALITY_DI_FILM_EG_MO_FILTER_H), EG_MO_FILTER_H);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, wReadQualityTable(QUALITY_DI_FILM_EG_MO_LARGE_BIT), EG_MO_LARGE_BIT);

    //Thredhold Min/Max
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, wReadQualityTable(QUALITY_DI_FILM_FR_MIN), FR_MIN);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_02, wReadQualityTable(QUALITY_DI_FILM_FR_MAX), FR_MAX);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, wReadQualityTable(QUALITY_DI_FILM_EG_MIN), EG_MIN); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, wReadQualityTable(QUALITY_DI_FILM_EG_MAX_START_32), EG_MAX_START_32);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, wReadQualityTable(QUALITY_DI_FILM_EG_MAX_END_32), EG_MAX_END_32);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, wReadQualityTable(QUALITY_DI_FILM_EG_MAX_START_22), EG_MAX_START_22);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, wReadQualityTable(QUALITY_DI_FILM_EG_MAX_END_22), EG_MAX_END_22);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_06, wReadQualityTable(QUALITY_DI_FILM_EG_MAX_END_BIT), EG_MAX_END_BIT); 

    //Rate
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, wReadQualityTable(QUALITY_DI_FILM_FR_RATE), FR_RATE);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, wReadQualityTable(QUALITY_DI_FILM_EG_RATE_32), EG_RATE_32);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, wReadQualityTable(QUALITY_DI_FILM_EG_RATE_22), EG_RATE_22);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_03, wReadQualityTable(QUALITY_DI_FILM_SUBTITLE_RATE), SUBTITLE_RATE);
}

UINT8 bDrvGetFilmStatus(void)
{
    return ((UINT8)DiPar.PdPar.u2FilmMode);
}

void vDrvDISetBob(UINT8 u1VdpId, UINT8 u1OnOff)
{       
    if (u1VdpId >= VDP_NS)
    {
        return;
    }
    
    vDrvDISetInterpMode(u1VdpId, (u1OnOff ? E(I, NR_OFF, DI_BOB, FRC_SCL) : _arMDDiPrm[u1VdpId].eDataMode));
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
                DiPQMode.bFilmOnOff_MultiCadence = u1OnOff;
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
void vDrvDIB2RFilmModeFreeze(UINT8 bPath, UINT8 u1OnOff)
{//// bit 1
     if (u1OnOff)
     {
          u1FreezeState |= 0x2;
     }
     else
     {
          u1FreezeState &= (~0x2);
     }
     MDDI_WRITE_FLD(VDP_1, MCVP_FILM_27, (u1FreezeState == 3), R_22ENABLE);  
     MDDI_WRITE_FLD(VDP_1, MCVP_FILM_27, (u1FreezeState == 3), R_32ENABLE); 
     vIO32WriteFldAlign(PSCAN_FWFILM_06, (u1FreezeState == 3), FILED_WEAVE_EN);
}


/**
 * @brief Turn on/off film mode Freeze for user freeze
 * @param bPath VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 */
void vDrvDIUserFilmModeFreeze(UINT8 bPath, UINT8 u1OnOff)
{//// bit 0
    

     if (u1OnOff)
     {
          u1FreezeState |= 0x1;
     }
     else
     {
          u1FreezeState &= (~0x1);
     }
     MDDI_WRITE_FLD(VDP_1, MCVP_FILM_27, (u1FreezeState == 3), R_22ENABLE);  
     MDDI_WRITE_FLD(VDP_1, MCVP_FILM_27, (u1FreezeState == 3), R_32ENABLE);      
     vIO32WriteFldAlign(PSCAN_FWFILM_06, (u1FreezeState == 3), FILED_WEAVE_EN);
}

void vDrvDISetMotionLevel(UINT8 u1VdpId, UINT8 u1YOnOff, UINT8 u1COnOff, UINT8 u1YLevel, UINT8 u1CLevel)
{
    u1YFixMotion[u1VdpId] = u1YOnOff;
    u1CFixMotion[u1VdpId] = u1COnOff;
    u1YMotionLevel[u1VdpId] = u1YLevel;
    u1CMotionLevel[u1VdpId] = u1CLevel;
}

void vDrvDISetRegionMotionOnOff(UINT8 u1VdpId,UINT8 u1OnOff)
{
    if(VDP_1==u1VdpId)
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_06, u1OnOff ? 0x2 : 0  , IF_RA_MOTION_LEVEL_INI);        
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_21, u1OnOff ? 0x0 : 0x3, IF_FORCE_SCENE_CHANGE);
        DiPar.bForceSceneChange = !u1OnOff;
    }    
}

void vDrvDISetInterpMode(UINT8 u1VdpId, E_MIB_DMODE bMode)
{
    UINT8 u1FixMotionY, u1FixMotionC;
    UINT8 u1ForceMotionY, u1ForceMotionC;

    if(IS_DI_3D(u1VdpId))
    {
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
    }
    else if(IS_DI_CBOB(u1VdpId))
    {
        u1FixMotionY = 0x0;
        u1FixMotionC = 0x1;
        u1ForceMotionY = 0x0;
        u1ForceMotionC = 0x8;
    }        
    else
    {
	    u1FixMotionY = 0x1;
        u1FixMotionC = 0x1;
        u1ForceMotionY = 0x8;
        u1ForceMotionC = 0x8;
    }        


    #ifdef PSCAN_AUTO_VERIFY
    u1FixMotionY = u1YFixMotion[u1VdpId] ? u1YFixMotion[u1VdpId] : u1FixMotionY;
    u1FixMotionC = u1CFixMotion[u1VdpId] ? u1CFixMotion[u1VdpId] : u1FixMotionC;
    u1ForceMotionY = u1YFixMotion[u1VdpId] ? u1YMotionLevel[u1VdpId] : u1ForceMotionY;
    u1ForceMotionC = u1CFixMotion[u1VdpId] ? u1CMotionLevel[u1VdpId] : u1ForceMotionC;    
    #endif
    
    if (u1VdpId == VDP_1)
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_20, 0x0, IF_FIX_BOB);  
        
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_20, u1FixMotionY, IF_FIX_MOTION_Y);  
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_20, u1FixMotionC, IF_FIX_MOTION_C); 
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_20, u1ForceMotionY, IF_FORCE_MOTION_Y);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_20, u1ForceMotionC, IF_FORCE_MOTION_C);
        
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_20, u1FixMotionY ? 0x3:0x0, IF_FIX_BOB);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_SB_04, u1FixMotionY, SB_FIX_MOTION_Y);  
        MDDI_WRITE_FLD(VDP_1, MCVP_SB_04, u1FixMotionC, SB_FIX_MOTION_C);  
        MDDI_WRITE_FLD(VDP_1, MCVP_SB_04, u1ForceMotionY, SB_FORCE_MOTION_Y);
        MDDI_WRITE_FLD(VDP_1, MCVP_SB_04, u1ForceMotionC, SB_FORCE_MOTION_C);
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

#if 0
        // force SD dispmode turn off CS to remind the CS quality is uncertain 
        if (((IS_SD_HEIGHT(VDP_1)) && (_arMDDiPrm[VDP_1].u1FrcMode)))
        {            
            DiPar.CsPar.bCsOnOff = SV_OFF;
        }
        else
        {
            DiPar.CsPar.bCsOnOff = bOnOff;
        }
#else
        DiPar.CsPar.bCsOnOff = bOnOff;
#endif

        _vDrvSetCsOnOff(VDP_1, DiPar.CsPar.bCsOnOff&&(!DiPar.bForceSceneChange));        
        _vDrvSetIfOnOff(VDP_1, bOnOff);
        _vDrvSetDarOnOff(VDP_1, bOnOff);    
        _vDrvFilmDefaultOnOff(VDP_1, bOnOff);
    }
    else
    {
        _vDrvDISetPDMAQuality(VDP_2);
    }
}

void vDrvDISetQualityISR(void)
{  
#ifndef __MODEL_slt__

    if (_u1DrvPScanQualityOnOff(VDP_1))
    {
        _vDrvDIGetPQStatusISR();

        if ((IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_QTY))
            && (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_QTY_ISR)))
        {
            _vDrvDISetPEQualityISR();
            _vDrvDISetCSQualityISR();
            _vDrvDISetPDQualityISR();   
            _vDrvDISetDARQualityISR();                
            _vDrvDISetIFQualityISR();            
        }        
    }

    #ifndef NDEBUG
    _vDrvDIEventISR();
    #endif
#endif
}

void vDrvDISetQualityISR_Ex(void)
{  
#ifndef __MODEL_slt__
    if (_u1DrvPScanQualityOnOff(VDP_1))
    {
        if ((IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_QTY))
            && (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_QTY_ISR)))
        {
            _vDrvDISetIFQualityISR_Ex();   
        }        
    }
#endif
}


void vDrvDIQtyMainLoop(UINT8 bPath)
{
    if (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_QTY) 
        && IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_QTY_MLOOP))
    {   
        if ((_u1DrvPScanQualityOnOff(bPath)))
        {
            _vDrvIFAdaptive(VDP_1);
            _vDrvPEAdaptive(VDP_1);
            _vDrvPDAdaptive(VDP_1);
            _vDrvDARAdaptive(VDP_1);
            _vDrvDISetOSD();
        }       

        if ((eDrvNRGet3DNRMode() == MCNR_MODE_MC) || (eDrvNRGet3DNRMode() == MCNR_MODE_MV))
        {
            _vDrvCSAdaptive(VDP_1);
        }
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
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//Debug Utility
////////////////////////////////////////////////////////////////////////////////
EXTERN void vDrvDIWaitDIVSyncSema(void);

void vDrvDISetDebugOSDModule(E_OSD_DEBUG_IDX u4Module)
{
    vRegWrite4B(SWRW_00, (UINT32)u4Module);
}

void vDrvDISetDebugOSD(E_OSD_DEBUG_IDX u4Module, UINT32 u4Value)
{
    //LOG(4, "Module %d = %d  Value 0x%08x\n", u4RegRead4B(SWRW_00), u4Module, u4Value);

    if (u4RegRead4B(SWRW_00) == u4Module)
    {
        vRegWrite4B(MCVP_KC_2A, u4Value);
    }
}

void vDrvDISetDebugOSDMask(E_OSD_DEBUG_IDX u4Module, UINT32 u4Value, UINT32 Fld)
{
    //LOG(4, "Module %d = %d  Value 0x%08x\n", u4RegRead4B(SWRW_00), u4Module, u4Value);

    if (u4RegRead4B(SWRW_00) == u4Module)
    {
        vRegWriteFldAlign(MCVP_KC_2A, u4Value, Fld);
    }
}

void vDrvDISetInkColor(UINT32 u4Idx, UINT32 u4Color)
{
    switch (u4Idx)
    {
    case 0:
        MDDI_WRITE_FLD(VDP_1, MCVP_DARE_08, u4Color, INK_COLOR_Y_0);
        break;
    case 1:
        MDDI_WRITE_FLD(VDP_1, MCVP_DARE_09, u4Color, INK_COLOR_Y_1);
        break;
    case 2:
        MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0A, u4Color, INK_COLOR_CB_0);
        break;
    case 3:
        MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0B, u4Color, INK_COLOR_CB_1);
        break;
    case 4:
        MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0C, u4Color, INK_COLOR_CR_0);
        break;
    case 5:
        MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0D, u4Color, INK_COLOR_CR_1);
        break;
    }
}

void vDrvDISetInk(UINT8 bMode, UINT16 u2Type)
{    
    #ifndef NDEBUG
    LOG(3,"Ink Mode: 0) Ink Off , 1) CS , 2) PE , 3) iFusion, 4) DARE, 5) SB\n");

    //Reset All ink Off
    MDDI_WRITE_FLD(VDP_1, MCVP_DARE_07, SV_OFF, DI_INK_EN);
    MDDI_WRITE_FLD(VDP_1, MCVP_SB_06, SV_OFF, SB_INK_ENABLE);
    MDDI_WRITE_FLD(VDP_1, MCVP_KC_21, 0x10, OSD_Y_POS);
    MDDI_WRITE_FLD(VDP_1, MCVP_KC_21, 0x10, OSD_X_POS);
    MDDI_WRITE_FLD(VDP_1, MCVP_DARE_07, (bMode) ? (bMode-1) : bMode, INK_SEL);

    switch (bMode)
    {
        case 0:
            MDDI_WRITE_FLD(VDP_1, MCVP_KC_21, SV_OFF, OSD_MODE);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_07, SV_OFF, DI_INK_EN);
            MDDI_WRITE_FLD(VDP_1, MCVP_CS_17, 0, CS_INK_SEL);
            MDDI_WRITE_FLD(VDP_1, MCVP_PE_00, 0, PE_INK_SEL);
            MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_1A, 0, IF_INK_SEL);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_07, 0, DARE_INK_SEL);
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
            
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_07, SV_ON, DI_INK_EN);
            MDDI_WRITE_FLD(VDP_1, MCVP_CS_17, u2Type, CS_INK_SEL);

            if (u2Type == 5)
            {
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_08, 0xdf38bdf0, INK_COLOR_Y_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_09, 0x38bdf38b, INK_COLOR_Y_1);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0A, 0x50fb8508, INK_COLOR_CB_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0B, 0xfb850fb8, INK_COLOR_CB_1);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0C, 0x88fffff8, INK_COLOR_CR_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0D, 0x888, INK_COLOR_CR_1);
            }
            else if (u2Type == 15)
            {
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_08, 0x88888888, INK_COLOR_Y_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_09, 0x8888888f, INK_COLOR_Y_1);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0A, 0xeca86428, INK_COLOR_CB_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0B, 0xf, INK_COLOR_CB_1);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0C, 0xeca86428, INK_COLOR_CR_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0D, 0xf, INK_COLOR_CR_1);
            }            
	     else if  (u2Type == 2)
	     {
	         MDDI_WRITE_FLD(VDP_1, MCVP_DARE_08, 0xaaaaaaaa, INK_COLOR_Y_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_09, 0xaaaaaaaa, INK_COLOR_Y_1);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0A, 0xffffffff, INK_COLOR_CB_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0B, 0x00000008, INK_COLOR_CB_1);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0C, 0xeca86420, INK_COLOR_CR_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0D, 0xeca86428, INK_COLOR_CR_1);
	     }
	     else
	     {  
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_08, 0x88888888, INK_COLOR_Y_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_09, 0x88888888, INK_COLOR_Y_1);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0A, 0xffffffff, INK_COLOR_CB_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0B, 0x00000000, INK_COLOR_CB_1);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0C, 0xff884422, INK_COLOR_CR_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0D, 0xff884422, INK_COLOR_CR_1);
	     }
            break;
            
        case 2: //PE ink            
            LOG(6, "0 : Final_Dir\n");
            LOG(6, "1 : Final_Spec_Dir\n");
            LOG(6, "2 : Final_Confidence\n");
            LOG(6, "3 : Final_Coeff\n");
            LOG(6, "4 : Bob_Density\n");

            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_07, SV_ON, DI_INK_EN);
            MDDI_WRITE_FLD(VDP_1, MCVP_PE_00, u2Type, PE_INK_SEL);

            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_08, 0x999999a8, INK_COLOR_Y_0);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_09, 0x66666688, INK_COLOR_Y_1);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0A, 0x333333a8, INK_COLOR_CB_0);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0B, 0xcccccc88, INK_COLOR_CB_1);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0C, 0x22222218, INK_COLOR_CR_0);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0D, 0xdddddd88, INK_COLOR_CR_1);
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

            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_07, SV_ON, DI_INK_EN);
            MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_1A, u2Type, IF_INK_SEL);
            
            /*MDDI_WRITE_FLD(VDP_1, MCVP_DARE_08, 0x999999a8, INK_COLOR_Y_0);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_09, 0x66666688, INK_COLOR_Y_1);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0A, 0x333333a8, INK_COLOR_CB_0);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0B, 0xcccccc88, INK_COLOR_CB_1);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0C, 0x22222218, INK_COLOR_CR_0);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0D, 0xdddddd88, INK_COLOR_CR_1);*/


            if (u2Type == 29 )
            {
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_08, 0x88888888, INK_COLOR_Y_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_09, 0x88888888, INK_COLOR_Y_1);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0A, 0xfffffff8, INK_COLOR_CB_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0B, 0x0000000f, INK_COLOR_CB_1);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0C, 0xfffffff8, INK_COLOR_CR_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0D, 0xfaaaaaa0, INK_COLOR_CR_1);
            }
            else
            {
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_08, 0x88888888, INK_COLOR_Y_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_09, 0x88888888, INK_COLOR_Y_1);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0A, 0xffffffff, INK_COLOR_CB_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0B, 0x00000000, INK_COLOR_CB_1);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0C, 0xff884422, INK_COLOR_CR_0);
                MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0D, 0xff884422, INK_COLOR_CR_1);
            }

            break;
            
        case 4: //DARE ink            
            LOG(6, "0 : Cont_Saw_Strength\n");
            LOG(6, "1 : Luma_Jaggy_Strength\n");
            LOG(6, "2 : Line_Flicker\n");
            LOG(6, "3 : Single_Saw_Strength\n");
            LOG(6, "4 : Final_Strength\n");

            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_07, SV_ON, DI_INK_EN);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_07, u2Type, DARE_INK_SEL);            
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_08, 0x88888888, INK_COLOR_Y_0);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_09, 0x8888888f, INK_COLOR_Y_1);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0A, 0xeca86428, INK_COLOR_CB_0);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0B, 0xf, INK_COLOR_CB_1);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0C, 0xeca86428, INK_COLOR_CR_0);
            MDDI_WRITE_FLD(VDP_1, MCVP_DARE_0D, 0xf, INK_COLOR_CR_1);
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

            vRegWriteFldAlign(MCVP_SB_06, SV_ON, SB_INK_ENABLE);
            vRegWriteFldAlign(MCVP_SB_06, u2Type, SB_INK_SELECT);
            break;
            
        default:
            break;
    }
#else
    UNUSED(bMode);
    UNUSED(u2Type);
#endif    
}

UINT32 u4DrvDIGetMotionLevel(void)
{
    UINT32 u4Motion = 0;

#ifdef CC_MT5396
    if (IS_INTERLACE(VDP_1))
    {
        u4Motion = DiSta.PdSta.u4FrmMotion;
    }
    else
    {
        u4Motion = RegReadFldAlign(MJCFD_0B, MJCFD_FD_DET_MOV_INFO);
    }

    if (DiSta.u4IsrIndex == 0)
    {
        LOG(3, "u4Motion = %d\n", u4Motion);
    }
#endif

    return u4Motion;
}

UINT32 u4DrvDIGetFusionMotionLevel(void)
{    
    if (MDDI_READ_FLD(VDP_1, MCVP_KC_19, FRC_REPEAT_FIELD)&&IS_INTERLACE(VDP_1)&&(!IS_FRAME_SEQ(VDP_1)))
    {
        return 0;        
    }
    else
    {
        return DiSta.IfSta.u4Homo;
    }
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
        vDrvDIWaitDIVSyncSema();

        pu4FilmModeFlag = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0F, FD_FLAG);
        pu4FrameMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
        pu4FieldMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_10, FD_32_SAW_FI_A_B_SAW_Q);
        pu4EdgeFieldMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
        pu4FrameStaticSeq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_08, FRAME_ST_SEQ_STATUS);
        pu4pFrameMotionSeq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_09, FRAME_ST_SEQ_STATUS);
        pu4FrameSimilarSeq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0A, FRAME_SIM_SEQ_STATUS);
        pu4FieldStatic22Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0C, FIELD_ST_SEQ_22_STATUS);
        pu4FieldStatic32Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0C, FIELD_ST_SEQ_STATUS);
        pu4FieldMotion22Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_29, FIELD_MO_SEQ_22_STATUS);
        pu4FieldMotion32Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_27, FIELD_MO_SEQ_STATUS);
        pu4EdgeStatic22Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0E, NEW_FIELD_ST_SEQ_22_STATUS);
        pu4EdgeStatic32Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0E, NEW_FIELD_ST_SEQ_STATUS);
        pu4EdgeMotion22Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_2A, NEW_FIELD_MO_SEQ_22_STATUS);
        pu4EdgeMotion32Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_28, NEW_FIELD_MO_SEQ_STATUS);
        pu4FieldSimilarSeq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0D, FIELD_SIM_SEQ_STATUS);
        pu4BlockMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
        pu4FilmFrameCB = MDDI_READ_FLD(VDP_1, MCVP_STATUS_21, FILM_CB_STATUS);

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
        vDrvDIWaitDIVSyncSema();

        pu4FilmModeFlag = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0F, FD_FLAG);
        pu4FrameMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
        pu4FieldMotion = DiPar.PdPar.u4AdaptiveEgMo32Th;
        pu4EdgeFieldMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
        pu4FrameStaticSeq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_08, FRAME_ST_SEQ_STATUS);
        pu4pFrameMotionSeq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_09, FRAME_ST_SEQ_STATUS);
        pu4FrameSimilarSeq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0A, FRAME_SIM_SEQ_STATUS);
        pu4FieldStatic22Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0C, FIELD_ST_SEQ_22_STATUS);
        pu4FieldStatic32Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0C, FIELD_ST_SEQ_STATUS);
        pu4FieldMotion22Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_29, FIELD_MO_SEQ_22_STATUS);
        pu4FieldMotion32Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_27, FIELD_MO_SEQ_STATUS);
        pu4EdgeStatic22Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0E, NEW_FIELD_ST_SEQ_22_STATUS);
        pu4EdgeStatic32Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0E, NEW_FIELD_ST_SEQ_STATUS);
        pu4EdgeMotion22Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_2A, NEW_FIELD_MO_SEQ_22_STATUS);
        pu4EdgeMotion32Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_28, NEW_FIELD_MO_SEQ_STATUS);
        pu4FieldSimilarSeq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0D, FIELD_SIM_SEQ_STATUS);
        pu4BlockMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
        pu4FilmFrameCB = MDDI_READ_FLD(VDP_1, MCVP_STATUS_21, FILM_CB_STATUS);

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
        vDrvDIWaitDIVSyncSema();

        u4LumaValue = MDDI_READ_FLD(VDP_1, MCVP_FUSION_16, IF_AVG_Y);
        u4CbValue = MDDI_READ_FLD(VDP_1, MCVP_FUSION_16, IF_AVG_CB);
        u4CrValue = MDDI_READ_FLD(VDP_1, MCVP_FUSION_16, IF_AVG_CR);
        u4PreLumaValue = MDDI_READ_FLD(VDP_1, MCVP_FUSION_17, IF_AVG_Y_PREV);

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
        
        pu4FilmModeFlag = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0F, FD_FLAG);
        pu4FrameMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
        pu4FieldMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_10, FD_32_SAW_FI_A_B_SAW_Q);
        pu4EdgeFieldMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
        pu4FrameStaticSeq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_08, FRAME_ST_SEQ_STATUS);
        pu4pFrameMotionSeq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_09, FRAME_ST_SEQ_STATUS);
        pu4FrameSimilarSeq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0A, FRAME_SIM_SEQ_STATUS);
        pu4FieldStatic22Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0C, FIELD_ST_SEQ_22_STATUS);
        pu4FieldStatic32Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0C, FIELD_ST_SEQ_STATUS);
        pu4FieldMotion22Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_29, FIELD_MO_SEQ_22_STATUS);
        pu4FieldMotion32Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_27, FIELD_MO_SEQ_STATUS);
        pu4EdgeStatic22Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0E, NEW_FIELD_ST_SEQ_22_STATUS);
        pu4EdgeStatic32Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0E, NEW_FIELD_ST_SEQ_STATUS);
        pu4EdgeMotion22Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_2A, NEW_FIELD_MO_SEQ_22_STATUS);
        pu4EdgeMotion32Seq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_28, NEW_FIELD_MO_SEQ_STATUS);
        pu4FieldSimilarSeq = MDDI_READ_FLD(VDP_1, MCVP_STATUS_0D, FIELD_SIM_SEQ_STATUS);
        pu4BlockMotion = MDDI_READ_FLD(VDP_1, MCVP_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
        pu4FilmFrameCB = MDDI_READ_FLD(VDP_1, MCVP_STATUS_21, FILM_CB_STATUS);

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
    UINT32 testloop = *(UINT32*)pvArgv;

    Printf("\nN HETER  DIF_HE  HOMO   DIF_HO  Hedge  Vedge \n");
    Printf("= ====== ======  ====== ======  ====== ======\n");

    while ((fgDIStatusPolling) && (testloop-- != 0))
    {
        vDrvDIWaitDIVSyncSema();        
        
        Printf("\n%1x %6x %6x %6x %6x %6x %6x ",
               DiSta.u4IsrIndex, 
               DiSta.IfSta.u4Heter, DiSta.IfSta.u4HeterDiff,
               DiSta.IfSta.u4Homo, DiSta.IfSta.u4HomoDiff,               
               DiSta.IfSta.u4HEdgeCnt, DiSta.IfSta.u4VEdgeCnt);
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
        vDrvDIWaitDIVSyncSema();

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


void vDrvDIMVStatusPolling(void *pvArgv)
{
    UINT32 testloop = *(UINT32*)pvArgv;
    INT32 i, j;
    
    while ((fgDIStatusPolling) && (testloop-- !=0))
    {
        vDrvDIWaitDIVSyncSema();  
    
        Printf("\n ======== MV[%d] Status ========\n", DiSta.u4IsrIndex);        

        if (DiSta.CsSta.u1MvReadValid)
        {
            for (i = -DI_CS_SEARCH_RANGE_X; i <= DI_CS_SEARCH_RANGE_X; i++)
            {
                for (j = -DI_CS_SEARCH_RANGE_Y; j <= DI_CS_SEARCH_RANGE_Y; j++)
                {
                    if (MV_HIST(i, j))
                    {
                         Printf("MV[%d, %d] = %d \n", i, j, MV_HIST(i, j));
                    }
                }   
            }
        }
        else
        {
            Printf("\nMV Hist Read fail \n");
        }

        Printf("NonZero  %d / HistDiff  %d  \n", DiSta.CsSta.u2NonZeroCnt, DiSta.CsSta.u4HistDiffAcc);
        Printf("Hist max %d / Total     %d  \n", DiSta.CsSta.u2HistMax, DiSta.CsSta.u2HistCnt);
        Printf("Edge     %d   CS state  %d  \n", DiSta.CsSta.u2EdgeCnt, DiPar.CsPar.bFavorCS);
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
        vDrvDIWaitDIVSyncSema();

        Printf("\n%1x %4x %8x ", DiSta.u4IsrIndex, 
            DiSta.PeSta.u4AliasCnt, 
            DiSta.PeSta.u4EdgeSum);

        for (i = 0; i < DI_BE_HIST_BIN; i++)
        {
            Printf("%4x ", DiSta.PeSta.u4EdgeHis[i]);
        }
    }
}

#ifndef NDEBUG

void vDrvDIEventStatusPolling(void * pvArgv)
{
    UINT32 i;
    UINT32 testloop = *(UINT32*)pvArgv;
   
    Printf("[RegValue True/False]");
    while ((fgDIStatusPolling) && (testloop-- !=0))
    {
        vDrvDIWaitDIVSyncSema();

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
        Printf("\nu4SrcAddr=0x%x",(int)(DiPar.Event[bEventSel].u4Addr|0xf0020000));
        Printf("\nu4Mask=0x%x",(int)(DiPar.Event[bEventSel].u4Mask));
        Printf("\nu4Low=0x%x",(int)(DiPar.Event[bEventSel].u4Low));
        Printf("\nu4High=0x%x",(int)(DiPar.Event[bEventSel].u4High));
        Printf("\nu4DstAddr=0x%x",(int)(DiPar.Event[bEventSel].u4DstAddr|0xf0020000));
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
        vDrvDIWaitDIVSyncSema();        
        
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
    
    Printf("N u4SawAB  u4SawBC SawRank SawNorm SawInv  Ratio\n");
    Printf("= ======== ======= ======= ======= ======= ======= \n");
    
    while ((fgDIStatusPolling) && (testloop-- !=0))
    {
        vDrvDIWaitDIVSyncSema();

        Printf("\n%1x %08x %08x %08x ", 
            DiSta.u4IsrIndex,
            DiSta.CsSta.u4SawAB,
            DiSta.CsSta.u4SawBC,
            DiSta.CsSta.u4FieldSawRank);
        
        Printf("%d  %d  ", DiSta.IfSta.u4SawNom, DiSta.IfSta.u4SawInv);

        if (DiSta.IfSta.u4SawNom)
        {
            Printf(" %4x", (DiSta.IfSta.u4SawNom<<8)/DiSta.IfSta.u4SawInv);
        }
    }
}
#endif

DI_PQ_THREAD DIPQThread[] =
{
    {vDrvDIIFStatusPolling,         "Fusion Status"},
    {vDrvDIFwFilmStatusPolling,     "FW FILM"},
    {vDrvDIFilmStatusPolling,       "HW FILM"},
    {vDrvDIHWFilmStatusPolling,     "NEW HW FILM"},    
    {vDrvDIFilmChk,                 "FILM Check"},    
    {vDrvDIMVStatusPolling,         "MV Hist"},
    {vDrvDIPEPolling,               "PE Status"},
#ifndef NDEBUG
    {vDrvDIEventStatusPolling,      "Event Status"},
    {vDrvDIEventProfileStatusPolling,   "Event Profile Status"},
    {vDrvDICSPolling,                   "CS Status"},
    {vDrvDIAdaptivePolling,             "Adaptive FW Status"},
#endif
    {NULL                       ,NULL}//End Of Function Table
};



