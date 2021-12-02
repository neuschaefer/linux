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
 * $Author: pingan.du $
 * $Date: 2013/01/15 $
 * $RCSfile: drv_nr.c,v $
 * $Revision: #7 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file NR driver
 *
 * @author mf_tien@mtk.com.tw
 * @function entry point
 *  vDrvNRInit
 *  vDrvNRSetMode
 * @user case
 *  power on -> vDrvNRInit
 *  mode change done -> vDrvNRSetMode
 *  user menu -> vDrvNRSetMode
 */

/**************************************************************************
 * Inclusions
 *************************************************************************/
#include "sv_const.h"
#include "vdo_misc.h"
#include "video_def.h"
#include "video_timing.h"

#include "hw_vdoin.h"
#include "hw_nr.h"

#include "nr_debug.h"

#include "drv_nr.h"
#include "drv_di.h"
#include "hw_di.h"

#include "pe_if.h"

#define _P2D(item) P_Fld(wReadQualityTable(QUALITY_2DNR_##item), item)
#define _P3D(item) P_Fld(wReadQualityTable(QUALITY_3DNR_##item), item)

#define NR_REGTBL_END 0xFFFF

#ifdef IS_ECO_IC
#undef IS_ECO_IC
#endif

#define IS_ECO_IC(void)          (((BSP_GetIcVersion() >= IC_VER_5368_AB) && IS_IC_5368()) \
                                    || ((BSP_GetIcVersion() >= IC_VER_5396_AB) && IS_IC_5396()))

#define IS_MCNR_STR(void)        (IS_ECO_IC() || IS_IC_5389())

////////////////////////////////////////////////////////////////////////////////
// Register table
////////////////////////////////////////////////////////////////////////////////
enum
{
    E_MCNR_SD_I         = 0,
    E_MCNR_SD_P         = 1,
    E_MCNR_HD           = 2,
    E_MCNR_FHD_I        = 3,
    E_MCNR_FHD_P        = 4,
    E_MCNR_NUM_TIMINGS  = 5
};

enum
{
    E_NXNR_SD           = 0,
    E_NXNR_HD           = 1,
    E_NXNR_FHD          = 2,
    E_NXNR_NUM_TIMINGS  = 3
};

const NR_REGTBL_T CODE _MCNR_PQ_COMMON[] =
{
    {MCNR_00, 0x0000043C, 0x00000FFC},//{MCNR_00, 0x00000420, 0x00000FFC},
    {MCNR_03, 0x01FF0000, 0xFFFFFFFF},
    {MCNR_09, 0xF83E0F80, 0xFFFFFFFF}, //{MCNR_09, 0xF83E0B80, 0xFFFFFFFF}, //{MCNR_09, 0xD8761B80, 0xFFFFFFFF},
    {MCNR_0A, 0x4AD06188, 0xFFFFFFFF},
    {MCNR_0B, 0x0852B418, 0xFFFFFFFF},
    {MCNR_0C, 0x188214AC, 0xFFFFFFFF},
    {MCNR_0D, 0x41862084, 0xFFFFFFFF},
    {MCNR_11, 0x00000000, 0x00000078},
    {MCNR_16, 0x82255000, 0xFFFFFFFF},
    {MCNR_26, 0x0312A160, 0xFFFFFFFF},
    {MCNR_27, 0xAC522100, 0xFFFFFFFF},

    {NR_REGTBL_END, 0x00000000, 0x00000000}
};
    
const NR_REGTBL_T CODE _MCNR_PQ_PARAM[E_MCNR_NUM_TIMINGS][6] =
{
    {   // SD interlaced (480i/576i)
        {MCNR_03, 0x007E0000, 0x007E0000},
        {MCNR_05, 0xC1404410, 0xFFFFFFFF},
        {MCNR_0E, 0x060C1218, 0xFFFFFFFF},
        {MCNR_0F, 0x1E242A30, 0xFFFFFFFF},
        {MCNR_27, 0x00000100, 0x000003C0},
        {NR_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   // SD progressive (480p/576p)
        {MCNR_03, 0x007E0000, 0x007E0000},
        {MCNR_05, 0xC2408420, 0xFFFFFFFF},
        {MCNR_0E, 0x060C1218, 0xFFFFFFFF},
        {MCNR_0F, 0x1E242A30, 0xFFFFFFFF},
        {MCNR_27, 0x00000100, 0x000003C0},
        {NR_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   // HD 720p
        {MCNR_03, 0x00400000, 0x007E0000},
        {MCNR_05, 0xC2408420, 0xFFFFFFFF},
        {MCNR_0E, 0x070E151C, 0xFFFFFFFF},
        {MCNR_0F, 0x232A3138, 0xFFFFFFFF},
        {MCNR_27, 0x00000200, 0x000003C0},
        {NR_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   // FHD interlaced (1080i)
        {MCNR_03, 0x00400000, 0x007E0000},
        {MCNR_05, 0xC2408420, 0xFFFFFFFF},
        {MCNR_0E, 0x070E151C, 0xFFFFFFFF},
        {MCNR_0F, 0x232A3138, 0xFFFFFFFF},
        {MCNR_27, 0x00000200, 0x000003C0},
        {NR_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   // FHD progressive (1080p)
        {MCNR_03, 0x00400000, 0x007E0000},
        {MCNR_05, 0xC2408420, 0xFFFFFFFF},
        {MCNR_0E, 0x070E151C, 0xFFFFFFFF},
        {MCNR_0F, 0x232A3138, 0xFFFFFFFF},
        {MCNR_27, 0x00000200, 0x000003C0},
        {NR_REGTBL_END, 0x00000000, 0x00000000}
    }
};

const NR_REGTBL_T CODE _NXNR_PQ_COMMON[] =
{
    {NXNR_03, 0x00000200, 0xFFFFFFFF},
    {NXNR_04, 0x20002068, 0xFFFFFFFF},
    {NXNR_05, 0x08800333, 0xFFFFFFFF},
    {NXNR_06, 0x00007020, 0xFFFFFFFF},  // only detect size = 8, 15, 16, 17
    {NXNR_07, 0x00007020, 0xFFFFFFFF},
    {NXNR_08, 0x00000000, 0xFFFFFFFF},
    {NXNR_09, 0x04040066, 0xFFFFFFFF},
    {NXNR_0A, 0x40402020, 0xFFFFFFFF},
    {NXNR_0B, 0xA2202D2D, 0xFFFFFFFF},
    {NXNR_0C, 0x00000000, 0xFFFFFFFF},
    {NXNR_0D, 0x00000000, 0xFFFFFFFF},
    {NXNR_0E, 0x88000864, 0xFFFFFFFF},
    {NXNR_0F, 0x00444444, 0xFFFFFFFF},
    {NXNR_10, 0x00044433, 0xFFFFFFFF},
    {NXNR_11, 0xC0010000, 0xFFFFFFFF},
    {NXNR_12, 0x7F0041A9, 0xFFFFFFFF},    
    {NR_REGTBL_END, 0x00000000, 0x00000000}
};

const NR_REGTBL_T CODE _NXNR_PQ_PARAM[E_NXNR_NUM_TIMINGS][1] =
{
    {   // SD
        {NR_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   // HD 720p
        {NR_REGTBL_END, 0x00000000, 0x00000000}
    },
    {   // FHD
        {NR_REGTBL_END, 0x00000000, 0x00000000}
    }
};

//NR global parameter
NR_PARAM_T _rNrPrm;

////////////////////////////////////////////////////////////////////////////////
// external functions
////////////////////////////////////////////////////////////////////////////////
EXTERN void vDrvDISetInkColor(UINT32 u4Idx, UINT32 u4Color);
EXTERN void vDrvNRNoiseLevelProc(void);
EXTERN void vDrvNRPrintNMStatus(void);
EXTERN void vDrvNRUpdateParam(void);

////////////////////////////////////////////////////////////////////////////////
// static function for internal use
////////////////////////////////////////////////////////////////////////////////
static void _vDrvNRLoadRegTbl(const NR_REGTBL_T* prRegTbl)
{
	while (prRegTbl->u2Addr != NR_REGTBL_END)
	{
		vRegWrite4BMsk(prRegTbl->u2Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
		prRegTbl++;
	}
}

static void _vDrvNRSetBypass(BOOL fgBypass)
{
    NR_W(MCNR_00, fgBypass, MCNR_BYPASS);
    NR_W(NXNR_00, fgBypass, NX_BYPASS);
}

////////////////////////////////////////////////////////////////////////////////
//NR Process
////////////////////////////////////////////////////////////////////////////////
void vDrvNRSet3DNRMode(E_3DNR_MODE e3DMode)
{
    switch (e3DMode)
    {
        case MCNR_MODE_MC:
        case MCNR_MODE_MV:
        case MCNR_MODE_NM:
        case MCNR_MODE_MA:
            NR_WM(MCNR_00, P_Fld(0, MCNR_BYPASS) | P_Fld(((UINT8)e3DMode-1), MCNR_MODE));
            break;
        case MCNR_MODE_OFF:
            default:        
            NR_WM(MCNR_00, P_Fld(1, MCNR_BYPASS) | P_Fld(0, MCNR_MODE));
            //NR_WM(MCNR_11, P_Fld(0, MCNR_FORCE_ALPHA_MC_EN)|P_Fld(0, MCNR_FORCE_ALPHA_MC));
            //NR_WM(MCNR_10, P_Fld(0, MCNR_FORCE_NOISE_LVL_EN)|P_Fld(0, MCNR_FORCE_NOISE_LVL));
            break;
            
    }

    _rNrPrm.eMCNRMode = e3DMode;
}

E_3DNR_MODE eDrvNRGet3DNRMode(void)
{
    return _rNrPrm.eMCNRMode;
}

void vDrvNRSet2DNRMode(E_2DNR_MODE e2DMode)
{
    UINT32 u4NXNROn = (e2DMode == NXNR_MODE_ON) ? 1 : 0;
    NR_WM(NXNR_00, P_Fld(u4NXNROn, NX_NR_EN)|P_Fld((!u4NXNROn), NX_BYPASS));
    
    _rNrPrm.eNXNRMode = e2DMode;
}

void vDrvNRSetChromaUpSample(BOOL fgOnOff)
{
    // can be enabled only for progressive format
    //if (!IS_SD_HEIGHT(VDP_1))
    if (!IS_SUPPORT_CUS(VDP_1)) //change by yuehao.qiu 2012-10-23 (ecn)
    {
        fgOnOff = SV_OFF;
    }
    NR_W(NXNR_00, fgOnOff, NX_CHROMA_UP_EN);
}

void vDrvNRSetInk(UINT8 u1Mode, UINT8 u1Type)
{
    UINT32 u4InkLvlShf = 0;
    
    LOG(3, "MCNR Ink Mode: 0) Ink Off , 1) MCNR MV, 2) 64 lvl, 3) 16 lvl, 4) 8 lvl\n");

    if (u1Type > 22 && u1Type != 0xFF)
    {
        LOG(3, "NR Ink type error: u1Type = %d\n", u1Type);    
        return;
    }

    switch (u1Mode)
    {
    default:
    case 0:
        NR_WM(MCNR_15, P_Fld(0, MCNR_INK_EN)|P_Fld(0, MCNR_INK_SEL)|P_Fld(u4InkLvlShf, MCNR_INK_LEVEL_SHIFT));
        return;
    case 1: // MV ink
        LOG(3, "11: raw_mv\n");
        LOG(3, "12: flt_refined_mv\n");
        LOG(3, "13: pmv\n");
        vDrvDISetInkColor(0, 0x88888888);
        vDrvDISetInkColor(1, 0x88888888);
        vDrvDISetInkColor(2, 0x40FC8408);
        vDrvDISetInkColor(3, 0xFC840FC8);
        vDrvDISetInkColor(4, 0x77CCCCC7);
        vDrvDISetInkColor(5, 0x00000777);
        break;
    case 2: // 0~64 level ink
        LOG(3, "1: weighting_y_mc_proc\n");
        LOG(3, "2: weighting_y_ma_proc\n");
        LOG(3, "3: weighting_y_blending\n");
        LOG(3, "4: weighting_c_adj\n");
        LOG(3, "10: motion_txt_lvl\n");
        vDrvDISetInkColor(0, 0x88888888);
        vDrvDISetInkColor(1, 0x88888888);
        vDrvDISetInkColor(2, 0xAA998877);
        vDrvDISetInkColor(3, 0xFEDDCCBB);
        vDrvDISetInkColor(4, 0xAA998877);
        vDrvDISetInkColor(5, 0xFEDDCCBB);
        u4InkLvlShf = 2;
        break;
    case 3: // 0~15 level ink
        LOG(3, "14: pmv_max_cnt1\n");
        LOG(3, "15: pmv_max_cnt2\n");
        LOG(3, "19: ma_noise_txtur_lvl\n");
        LOG(3, "20: ma_prtrbtn_lvl\n");
        LOG(3, "21: mc_noise_txtur_lvl\n");
        LOG(3, "22: mc_lmv_lvl\n");        
        vDrvDISetInkColor(0, 0x88888888);
        vDrvDISetInkColor(1, 0x88888888);
        vDrvDISetInkColor(2, 0xAA998877);
        vDrvDISetInkColor(3, 0xFEDDCCBB);
        vDrvDISetInkColor(4, 0xAA998877);
        vDrvDISetInkColor(5, 0xFEDDCCBB);
        break;
    case 4: // 0~8 level ink
        LOG(3, "0: alpha_mc\n");
        LOG(3, "5: ts_ratio_y\n");
        LOG(3, "6: ts_ratio_c\n");
        LOG(3, "7: ts_ratio\n");
        LOG(3, "8: still_level\n");
        LOG(3, "9: non_smooth_level\n");
        LOG(3, "16: rd_still_level\n");
        LOG(3, "17: mv_still_level\n");
        LOG(3, "18: mv_confidence\n");
        
        vDrvDISetInkColor(0, 0x88888888);
        vDrvDISetInkColor(1, 0x88888888);
        vDrvDISetInkColor(2, 0xEDCBA987);
        vDrvDISetInkColor(3, 0xFFFFFFFF);
        vDrvDISetInkColor(4, 0xEDCBA987);
        vDrvDISetInkColor(5, 0xFFFFFFFF);
        break;
    }

    if (u1Type == 0xFF)
    {
        NR_WM(MCNR_15, P_Fld(0, MCNR_INK_EN)|P_Fld(0, MCNR_INK_SEL)|P_Fld(0, MCNR_INK_LEVEL_SHIFT));
    }
    else
    {
        NR_WM(MCNR_15, P_Fld(1, MCNR_INK_EN)|P_Fld(u1Type, MCNR_INK_SEL)|P_Fld(u4InkLvlShf, MCNR_INK_LEVEL_SHIFT));
    }
}

/**
* @brief Set chroma line average mode register.
* @param bPath VDP_1/ VDP_2
* @param bMode 	NR_CHROMA_LINE_AVERAGE_MODE_OFF/ NR_CHROMA_LINE_AVERAGE_MODE_UP/
*           	NR_CHROMA_LINE_AVERAGE_MODE_DOWN/ NR_CHROMA_LINE_AVERAGE_MODE_121
*/
void vDrvNRSetChromaLineAvgModeReg(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode)
{
    UNUSED(bPath);
    
    //Only turn on chroma line average under TVD source
    if(!fgIsMainTvd3d())
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, SV_OFF, PAL_C_AVG_EN);
        return;
    }
    
    //5365 use only 121 chroma line average.  
    if(bMode == NR_CHROMA_LINE_AVERAGE_MODE_121)
    {
        //121   	        
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, SV_ON, PAL_C_AVG_EN);
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_2F, SV_ON, PAL_C_AVG_HALF);   
    }
    else if(bMode == NR_CHROMA_LINE_AVERAGE_MODE_DYN)
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, SV_ON, PAL_C_AVG_EN);
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_2F, SV_OFF, PAL_C_AVG_HALF); 

    }
    else//NR_CHROMA_LINE_AVERAGE_MODE_OFF
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, SV_OFF, PAL_C_AVG_EN);
    }
}

UINT8 u1DrvNRGetNoiseMeterSupport(void)
{
    UINT32 u4VdoTiming = bDrvVideoGetTiming(SV_VP_MAIN);
    UINT8 u1Ret = 0;

    switch (u4VdoTiming)
    {
    case MODE_525I_OVERSAMPLE:
    case MODE_625I_OVERSAMPLE:
    case MODE_480P_OVERSAMPLE:
    case MODE_576P_OVERSAMPLE:
    case MODE_525I:
    case MODE_625I:
    case MODE_480P:
    case MODE_576P:
    case MODE_480P_24:
    case MODE_480P_30:
    case MODE_576P_25:
        u1Ret = NR_NM_SAD_HIST;
        break;
    case MODE_720p_24:
    case MODE_720p_25:
    case MODE_720p_30:
    case MODE_720p_50:
    case MODE_720p_60:
        u1Ret = (IS_ECO_IC() && (!IS_IC_5389())) ? NR_NM_SAD_HIST : 0;
        break;
    case MODE_1080i_48:
    case MODE_1080i_50:
    case MODE_1080i:
        u1Ret = (IS_ECO_IC() && (!IS_IC_5389())) ? NR_NM_SAD_HIST : (IS_IC_5389() ? NR_NM_TDF_BASE : 0);
        break;
    case MODE_1080p_24:
    case MODE_1080p_25:
    case MODE_1080p_30:
    case MODE_1080p_50:
    case MODE_1080p_60:
        u1Ret = (IS_ECO_IC() && (!IS_IC_5389())) ? NR_NM_SAD_HIST : 0;
        break;
    default:
        u1Ret = 0;
    }

    return u1Ret;
}

UINT8 u1DrvNRGetBlockMeterSupport(void)
{
#ifdef DEF_DISABLE_NR_BLK_METER
    return 0;
#else
    return (_rNrPrm.u1Ver & NR_SUPPORT_BKMTR) ? 1 : 0;
#endif
}

void vDrvNRSetQuality(UINT8 u1Interlace, UINT16 u2Width, UINT16 u2Height)
{
    UINT32 u4DecType = bGetVideoDecType(VDP_1);
    UINT32 u4VdoTiming = bDrvVideoGetTiming(SV_VP_MAIN);
    UINT32 u4HistTmpIIRWei = 0; //0xA;
    UINT32 u4GMVValidChk = 1;
    UINT32 u4StillTexEn = 0;
    UINT32 u4NonSmoothThdNlGain = 3;
    UINT32 u4SupportHBlkMeter = 0;
    UINT32 u4Thd8 = 0x30;
    UINT32 u4SADHistWinH = 4;
    UINT32 u4SADHistWinV = 16;
    UINT32 u4BKNLMaxStr = 0;
    UINT32 u4BkNLGain = 0;
    
    _vDrvNRLoadRegTbl(_MCNR_PQ_COMMON);
    _vDrvNRLoadRegTbl(_NXNR_PQ_COMMON);
    
    switch (u4VdoTiming)
    {
    case MODE_525I_OVERSAMPLE:
    case MODE_625I_OVERSAMPLE:
    case MODE_480P_OVERSAMPLE:
    case MODE_576P_OVERSAMPLE:
    case MODE_525I:
    case MODE_625I:
        _vDrvNRLoadRegTbl(_MCNR_PQ_PARAM[E_MCNR_SD_I]);
        _vDrvNRLoadRegTbl(_NXNR_PQ_PARAM[E_NXNR_SD]);
        u4StillTexEn = 1;
        u4NonSmoothThdNlGain = 5;
        u4SADHistWinH = 4;
        u4SADHistWinV = 16;

        if (u4DecType == SV_VD_TVD3D)
        {
            NR_W(MCNR_26, 2, MCNR_NM_RPRSNT_PEAK_RTO);
            NR_W(MCNR_27, 7, MCNR_NM_FLT_COEF_CUR);
            NR_WMSK(NXNR_0A, 0x20201010, 0xFFFFFFFF);
            NR_WMSK(NXNR_0B, 0x00001010, 0x0000FFFF);
            NR_W(NXNR_10, 7, NX_MNR_SYM_LINE_GAIN);
            u4HistTmpIIRWei = 0; //4;
            u4GMVValidChk = 0;
        }
                
        break;
    case MODE_480P:
    case MODE_576P:
    case MODE_480P_24:
    case MODE_480P_30:
    case MODE_576P_25:
        _vDrvNRLoadRegTbl(_MCNR_PQ_PARAM[E_MCNR_SD_P]);
        _vDrvNRLoadRegTbl(_NXNR_PQ_PARAM[E_NXNR_SD]);
        u4StillTexEn = 1;
        u4SADHistWinH = 4;
        u4SADHistWinV = 16;
        break;
    case MODE_720p_24:
    case MODE_720p_25:
    case MODE_720p_30:
    case MODE_720p_50:
    case MODE_720p_60:
    case MODE_3D_720p_50_FP:
    case MODE_3D_720p_60_FP:
        _vDrvNRLoadRegTbl(_MCNR_PQ_PARAM[E_MCNR_HD]);
        _vDrvNRLoadRegTbl(_NXNR_PQ_PARAM[E_NXNR_HD]);
        u4Thd8 = 0x38;
        u4SADHistWinH = 7;
        u4SADHistWinV = 24;
        break;
    case MODE_1080i_48:
    case MODE_1080i_50:
    case MODE_1080i:
    case MODE_3D_1080I_60_FP:
        _vDrvNRLoadRegTbl(_MCNR_PQ_PARAM[E_MCNR_FHD_I]);
        _vDrvNRLoadRegTbl(_NXNR_PQ_PARAM[E_NXNR_FHD]);
        u4Thd8 = 0x38;
        u4NonSmoothThdNlGain = 6;
        u4SupportHBlkMeter = 0;
        u4SADHistWinH = 10;
        u4SADHistWinV = 40;
        u4BkNLGain = 3;
        u4BKNLMaxStr = 1000;
        break;
    case MODE_1080p_24:
    case MODE_1080p_25:
    case MODE_1080p_30:
    case MODE_1080p_50:
    case MODE_1080p_60:
    case MODE_3D_1080p_24_FP:
        _vDrvNRLoadRegTbl(_MCNR_PQ_PARAM[E_MCNR_FHD_P]);
        _vDrvNRLoadRegTbl(_NXNR_PQ_PARAM[E_NXNR_FHD]);
        u4Thd8 = 0x38;
        u4NonSmoothThdNlGain = 6;
        u4SupportHBlkMeter = 0;
        u4SADHistWinH = 10;
        u4SADHistWinV = 40;
        break;
    default:
        _vDrvNRLoadRegTbl(_MCNR_PQ_PARAM[E_MCNR_SD_P]);
        _vDrvNRLoadRegTbl(_NXNR_PQ_PARAM[E_NXNR_SD]);
        u4SupportHBlkMeter = 0;
        u4SADHistWinH = 4;
        u4SADHistWinV = 16;
        break;
    }

    if (_SWR(FLASH_PQ_00, FLASH_PQ_READY))
    {
        vDrvNRSet3DNRParam();
        vDrvNRSetSNRParam();
        vDrvNRSetMpegNRParam();
    }

    _SWWM(NR_NM_00,
        P_Fld(u4HistTmpIIRWei, HIST_TEMP_IIR_WEI)|
        P_Fld(u4GMVValidChk, CHK_GMV_RDCNL)|
        P_Fld(u4StillTexEn, STILL_TEXTURE));
    _SWW(NR_NM_03, u4StillTexEn, FAVOR_MCNR_MODE);
   // _SWW(NR_NM_06, (u4DecType == SV_VD_TVD3D ? 1 : 0), NM_EXT_NL_MODE);
   _SWW(NR_NM_06, 0, NM_EXT_NL_MODE);
    _SWW(NR_NM_0A, u4NonSmoothThdNlGain, NM_ZOOM_TEX_NON_SMOOTH_THD_NL_GAIN);
    _SWW(NR_NM_09, u4NonSmoothThdNlGain, NM_ZOOM_TEX_NON_SMOOTH_THD_NL_GAIN_DFT);
    _SWW(NR_NM_0F, (u1Interlace ? 0x24 : 0x32), NM_ANTI_NL_GAIN_THD_HI);
    _SWW(NR_PQ_09, u4Thd8, NR_TBL_THD8);
    _SWW(NR_PQ_09, u4Thd8, NR_TBL_THD8_HI);
    _SWWM(NR_BK_METER_01, P_Fld(u4BKNLMaxStr, NR_BK_NL_MAX_STR)|P_Fld(u4BkNLGain, NR_BK_NL_GAIN));

    _SWW(NR_NM_06, (NR_NM_TDF_BASE == u1DrvNRGetNoiseMeterSupport() ? 3 : 0), NM_SC_Q_NL_MODE);

    _SWW(NR_CTRL_00, u4SupportHBlkMeter, EN_SUPPORT_HBLK);

    NR_W(MCNR_00, (u1Interlace?6:5), MCNR_R2C_INTERVAL);

    if (_rNrPrm.u1Ver&NR_VER_ECO)
    {
        NR_WM(MCNR_2C, P_Fld(u4SADHistWinH, MCNR_SAD_HIST_HS)|P_Fld(u4SADHistWinH, MCNR_SAD_HIST_HE));
        NR_WM(MCNR_2D,
            P_Fld((u1Interlace ? (u4SADHistWinV/2) : u4SADHistWinV), MCNR_SAD_HIST_VS)|
            P_Fld((u1Interlace ? (u4SADHistWinV/2) : u4SADHistWinV), MCNR_SAD_HIST_VE));
    }

    vDrvNRUpdateParam();

    _rNrPrm.u1Timing = u4VdoTiming;
    _rNrPrm.u1NoiseMeterEn = u1DrvNRGetNoiseMeterSupport();
    _rNrPrm.u1Interlace = u1Interlace;
    _rNrPrm.u2Width = u2Width;
    _rNrPrm.u2Height = u2Height;
    _rNrPrm.u4TolBlks = (_rNrPrm.u1Ver&NR_VER_ECO)?
        ((((u2Width>>3)-(u4SADHistWinH*2)) * (u2Height-(u4SADHistWinV*2))) >> (u1Interlace ? 1 : 0)) :        
        (((u2Width>>3) * u2Height) >> (u1Interlace ? 1 : 0)); 
}

void vDrvNRSet3DNRParam(void)
{
    // Strength
    NR_W(MCNR_00, wReadQualityTable(QUALITY_3DNR_TNR_ACNR_STRENGTH), MCNR_ACNR_STRENGTH);
    // LUT
    _SWWM(NR_PQ_00, _P3D(NR_TBL_MA_0)|_P3D(NR_TBL_MA_1)|_P3D(NR_TBL_MA_2)|_P3D(NR_TBL_MA_3)|_P3D(NR_TBL_MA_4)|_P3D(NR_TBL_MA_5));
    _SWWM(NR_PQ_01, _P3D(NR_TBL_MA_6)|_P3D(NR_TBL_MA_7));
    _SWWM(NR_PQ_02, _P3D(NR_TBL_C_0)|_P3D(NR_TBL_C_1));
    _SWWM(NR_PQ_03, _P3D(NR_TBL_C_2)|_P3D(NR_TBL_C_3)|_P3D(NR_TBL_C_4)|_P3D(NR_TBL_C_5)|_P3D(NR_TBL_C_6)|_P3D(NR_TBL_C_7));
    _SWWM(NR_PQ_04, _P3D(NR_TBL_MA_E_0)|_P3D(NR_TBL_MA_E_1)|_P3D(NR_TBL_MA_E_2)|_P3D(NR_TBL_MA_E_3)|_P3D(NR_TBL_MA_E_4)|_P3D(NR_TBL_MA_E_5));
    _SWWM(NR_PQ_05, _P3D(NR_TBL_MA_E_6)|_P3D(NR_TBL_MA_E_7)|_P3D(NR_IIR_MAX_WEI)|_P3D(NR_IIR_MAX_WEI_E));
    // Pixel Adaptive
    _SWWM(NR_PQ_08, _P3D(NR_MOVINGTXR_SUB_WEI_BASE_DFT)|_P3D(NR_MOVINGTXR_SUB_WEI_NL_NAG_GAIN_DFT));
    NR_WM(MCNR_04,
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_MA_NOISE_ADD_WEI_BASE), MCNR_MA_NOISE_ADD_WEI_BASE)|
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_MA_NOISE_ADD_WEI_NL_GAIN), MCNR_MA_NOISE_ADD_WEI_NL_GAIN));
    NR_WM(MCNR_05,
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_MA_PRTRBTN_ADD_WEI_BASE), MCNR_MA_PRTRBTN_ADD_WEI_BASE)|
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_MA_PRTRBTN_ADD_WEI_STILL_GAIN), MCNR_MA_PRTRBTN_ADD_WEI_STILL_GAIN)|
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_STILL_GMV_ASSIST_DE_PRTRBTN), MCNR_STILL_GMV_ASSIST_DE_PRTRBTN));
    NR_WM(MCNR_06,
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_CS_NOISE_ADD_WEI_BASE), MCNR_MC_NOISE_ADD_WEI_BASE)|
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_CS_NOISE_ADD_WEI_NL_GAIN), MCNR_MC_NOISE_ADD_WEI_NL_GAIN));
    NR_WM(MCNR_07,
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_CHROMA_MOVINGTXR_SUB_WEI_MAX), MCNR_CHROMA_MOVINGTXR_SUB_WEI_MAX)|
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_CHROMA_DC_ADD_WEI_MAX), MCNR_CHROMA_DC_ADD_WEI_MAX)|
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_CHROMA_DC_DIFF_BND), MCNR_CHROMA_DC_DIFF_BND));

    vDrvNRUpdateParam();
}

void vDrvNRSetSNRParam(void)
{
    // SNR gain
    NR_WM(NXNR_04, _P2D(NX_SNR_GAIN_C));
    _SWW(NR_PQ_09, wReadQualityTable(QUALITY_2DNR_NX_SNR_GAIN_Y), NR_SNR_GAIN);
    // SNR threshold setting
    NR_WM(NXNR_11, _P2D(NX_SNR_COST_TH_DIR_ADAP)|_P2D(NX_SNR_NOISE_COST_MODE)|_P2D(NX_SNR_NOISE_REMOVAL_AUTO_TH_EDGE_STR));
    _SWWM(NR_PQ_08,
        P_Fld(wReadQualityTable(QUALITY_2DNR_NX_SNR_NOISE_REMOVAL_AUTO_TH_GENERAL_STR), NR_SNR_AUTO_TH_GENERAL_STR)|
        P_Fld(wReadQualityTable(QUALITY_2DNR_NR_SNR_MANUAL_AUTO_RATIO), NR_SNR_MANUAL_AUTO_RATIO));
    _SWWM(NR_NM_07, _P2D(SNR_THM_SEL));
    _SWWM(NR_NM_03, _P2D(SNR_THM_GAIN));
    _SWWM(NR_NM_04, _P2D(SNR_THM_OFST));
    // SNR max ratio
    _SWWM(NR_PQ_06, _P2D(SNR_MAX_RTO_LVL_0)|_P2D(SNR_MAX_RTO_LVL_1)|_P2D(SNR_MAX_RTO_LVL_2)|_P2D(SNR_MAX_RTO_LVL_3)|_P2D(SNR_MAX_RTO_LVL_4)|_P2D(SNR_MAX_RTO_LVL_5)|_P2D(SNR_MAX_RTO_LVL_6)|_P2D(SNR_MAX_RTO_LVL_7));
    _SWWM(NR_PQ_05, _P2D(SNR_MAX_RTO_LVL_8)|_P2D(SNR_MAX_RTO_LVL_9));
    // SNR C
    NR_WM(NXNR_12, _P2D(NX_SNR_CHROMA_PRESERVATION)|_P2D(NX_SNR_DEAD_ZONE_TRANSITION)|_P2D(NX_SNR_DEAD_ZONE_DARK)|_P2D(NX_SNR_DEAD_ZONE_BRIGHT));
}

void vDrvNRSetMpegNRParam(void)
{
    NR_WM(NXNR_04, _P2D(NX_BNR_GAIN)|_P2D(NX_MNR_GAIN));
    // BNR
    NR_WM(NXNR_09, _P2D(NX_HBLK_MIN_LINE_SUM)|_P2D(NX_VBLK_MIN_LINE_SUM));
    // MNR
    NR_WM(NXNR_0E, _P2D(NX_MNR_EDGE_TH)|_P2D(NX_MNR_GROUPING_STR));
    NR_WM(NXNR_0F, _P2D(NX_MNR_LERE_GAIN)|_P2D(NX_MNR_LERF_GAIN)|_P2D(NX_MNR_LFRE_GAIN)|_P2D(NX_MNR_UEDE_GAIN)|_P2D(NX_MNR_UEDF_GAIN)|_P2D(NX_MNR_UFDE_GAIN));
    NR_WM(NXNR_10, _P2D(NX_MNR_STR_GAIN));
}

void vDrvNRSet2DNRBNROnOff(UINT8 u1OnOff)
{
    NR_W(NXNR_00, (u1OnOff ? 1 : 0), NX_BNR);
    _SWW(NR_CTRL_00, u1OnOff&&u1DrvNRGetBlockMeterSupport(), EN_ADAP_SNR_BLK);

    if (u1OnOff&&u1DrvNRGetBlockMeterSupport())
        vDrvNRSet2DNRSNROnOff(SV_ON);
}
void vDrvNRSet2DNRMNROnOff(UINT8 u1OnOff)
{
    NR_W(NXNR_00, (u1OnOff ? 1 : 0), NX_MNR);
}
void vDrvNRSet2DNRSNROnOff(UINT8 u1OnOff)
{
    NR_W(NXNR_00, (u1OnOff ? 1 : (_SWR(NR_CTRL_00, EN_ADAP_SNR_BLK) ? 1 : 0)), NX_SNR_Y);
}
void vDrvNRSet3DNROnOff(UINT8 u1OnOff)
{
    NR_W(MCNR_00, (u1OnOff ? 1 : 0), MCNR_ENABLE);
}

void vDrvNRSet3DNRStrength(UINT8 u1Strength)
{
    _SWW(NR_PQ_05, MIN(u1Strength, 31), NR_IIR_MAX_WEI_E);
}

void vDrvNRSet2DNRAuto(UINT8 u1OnOff)
{
    u1OnOff = (u1OnOff && (NR_NM_SAD_HIST == u1DrvNRGetNoiseMeterSupport()) ? 1 : 0);
    _SWW(NR_CTRL_00, u1OnOff, EN_ADAP_SNR_THM);
}

void vDrvNRSet3DNRAuto(UINT8 u1OnOff)
{
    u1OnOff = (u1OnOff && (NR_NM_SAD_HIST == u1DrvNRGetNoiseMeterSupport()) ? 1 : 0);
    _SWWM(NR_CTRL_00,
        P_Fld((((bGetVideoDecType(SV_VP_MAIN)==SV_VD_TVD3D)&&
        (bGetSignalType(SV_VP_MAIN)==SV_ST_TV)) ? 0x01 : u1OnOff), EN_AUTO_XNR)|
        P_Fld(u1OnOff || (NR_NM_TDF_BASE == u1DrvNRGetNoiseMeterSupport()), EN_ADAP_WEIGHT));
    vDrvNRUpdateParam();
}

void vDrvNRSetBitRes(UINT8 u18bit)
{
    NR_W(MCNR_01, (u18bit ? 10 : 8), MCNR_RND_PRECISION);
    _SWW(NR_CTRL_00, (u18bit ? 1 : 0), EN_PATCH_8BIT);
}

void vDrvNRSetForceBypass(UINT8 u1OnOff)
{
    _vDrvNRSetBypass(u1OnOff);
}

void vDrvNRSet3DNRChromaPath(UINT8 u1OnOff)
{
    if (u1OnOff)
    {
        NR_W(MCNR_00, 0, MCNR_C_TEX_OFF);
        vDrvNRSet3DNRParam();
    }
    else
    {
        NR_W(MCNR_00, 1, MCNR_C_TEX_OFF);
        _SWWM(NR_PQ_02, P_Fld(0, NR_TBL_C_0)|P_Fld(0, NR_TBL_C_1));
        _SWWM(NR_PQ_03, P_Fld(0, NR_TBL_C_2)|P_Fld(0, NR_TBL_C_3)|P_Fld(0, NR_TBL_C_4)|P_Fld(0, NR_TBL_C_5)|P_Fld(0, NR_TBL_C_6)|P_Fld(0, NR_TBL_C_7));
        NR_WM(MCNR_07, P_Fld(0, MCNR_CHROMA_MOVINGTXR_SUB_WEI_MAX)|P_Fld(0, MCNR_CHROMA_DC_ADD_WEI_MAX));
    }
}

void vDrvNRGetNRStatus(void)
{
    LOG(0, "(3DNR Mode, 2DNR Mode) = (%d, %d)\n", _rNrPrm.eMCNRMode, _rNrPrm.eNXNRMode);
    LOG(0, "Timing: %s\n", strDrvVideoGetTimingString(_rNrPrm.u1Timing));
    LOG(3, "ConflictCnt = %d\n", _rNrPrm.u4ConflictCnt);

    // noise meter query
    vDrvNRPrintNMStatus();
}

/**
 * @brief Initialize NR
 */
void vDrvNRInit(void)
{
    _rNrPrm.eMCNRMode = MCNR_MODE_OFF;
    _rNrPrm.eNXNRMode = NXNR_MODE_OFF;
    _rNrPrm.u4ConflictCnt = 0;
    _rNrPrm.u1Interlace = 1;
    _rNrPrm.u2Width = 720;
    _rNrPrm.u2Height = 480;
    _rNrPrm.u4TolBlks = 21600;
    _rNrPrm.u1Ver = (IS_ECO_IC() ? NR_SUPPORT_6BIT_NL : 0) | (IS_MCNR_STR() ? NR_SUPPORT_STRENTH : 0);

    // temporally bypass for initial verification
    _vDrvNRSetBypass(SV_TRUE);

    vDrvNRPQInit();
}

void vDrvNRProc(void)
{  
    
    //Vestel for weak RF signal ,increase NR strength; pingan
    static BOOL fg_RF_NR = SV_FALSE;
    if((IO32ReadFldAlign(ADAPTIVE_REG, ADAPTIVE_NR))&&
        (bGetVideoDecType(SV_VP_MAIN)==SV_VD_TVD3D)&&
        (bGetSignalType(SV_VP_MAIN)==SV_ST_TV)&&
        (bTvdSignalStrengthLevel(0) < 85))// TVD RF source
    {
        //if(fg_RF_NR == SV_FALSE)
        {
            fg_RF_NR = SV_TRUE;
            _SWWM(NR_PQ_04, P_Fld(0x0F, NR_TBL_MA_E_0) | P_Fld(0x0F,NR_TBL_MA_E_1)|P_Fld(0x0F,NR_TBL_MA_E_2)|P_Fld(0x0E,NR_TBL_MA_E_3)|P_Fld(0x0C,NR_TBL_MA_E_4)|P_Fld(0xA,NR_TBL_MA_E_5));
            _SWWM(NR_PQ_05, P_Fld(0x9,NR_TBL_MA_E_6)|P_Fld(0x8,NR_TBL_MA_E_7)|P_Fld(0x1F,NR_IIR_MAX_WEI)|P_Fld(0x1F,NR_IIR_MAX_WEI_E));
            NR_WM(MCNR_04,
                    P_Fld(0x0F, MCNR_MA_NOISE_ADD_WEI_BASE)|
                    P_Fld(0x0, MCNR_MA_NOISE_ADD_WEI_NL_GAIN));
            vDrvNRSet3DNRAuto(SV_ON);
        }
    }
    else
    {
        if(fg_RF_NR == SV_TRUE)
        {
            fg_RF_NR = SV_FALSE;
            vDrvNRSet3DNRParam();
            vDrvNRSet3DNRAuto(SV_OFF);
        }
    }
    //End ---

    vDrvNRNoiseLevelProc();
}

/**
 * @brief Set split line for demo mode
 * @param bOnOff SV_ON/SV_OFF
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param wXCenter Split line position
 * @warning bPath does not have any effect for now
 */
void vDrvNRSet2DNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter)
{
    // set demo side window left
    NR_WM(NXNR_01, P_Fld(0, NX_DEMO_XS)|P_Fld(wXCenter, NX_DEMO_XE));
    NR_WM(NXNR_02, P_Fld(0, NX_DEMO_YS)|P_Fld(_rNrPrm.u2Height, NX_DEMO_YE));
    NR_WM(NXNR_00, P_Fld((bOnOff?1:0), NX_DEMO_EN)|P_Fld(((bRegion==SV_LEFT)?0:1), NX_DEMO_INSIDE));
}

void vDrvNRSet3DNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter)
{
    // set demo side window left
    NR_WM(MCNR_13, P_Fld(0, MCNR_DEMO_HS)|P_Fld(wXCenter, MCNR_DEMO_HE));
    NR_WM(MCNR_14, P_Fld(0, MCNR_DEMO_VS)|P_Fld(_rNrPrm.u2Height, MCNR_DEMO_VE));
    NR_WM(MCNR_00, P_Fld((bOnOff?1:0), MCNR_DEMO_MODE)|P_Fld(((bRegion==SV_LEFT)?1:0), MCNR_DEMO_INSIDE));
}

void vDrvNRSet2DNRDemoRgn(UINT8 bOnOff, Region bRegion)
{
    NR_WM(NXNR_01,
        P_Fld(bRegion.wHStart, NX_DEMO_XS)|
        P_Fld(bRegion.wHEnd-1, NX_DEMO_XE));
    NR_WM(NXNR_02,
        P_Fld(bRegion.wVStart, NX_DEMO_YS)|
        P_Fld(bRegion.wVEnd, NX_DEMO_YE));
    NR_WM(NXNR_00, P_Fld((bOnOff?1:0), NX_DEMO_EN)|P_Fld(0, NX_DEMO_INSIDE));
}

void vDrvNRSet3DNRDemoRgn(UINT8 bOnOff, Region bRegion)
{
    UINT8 bSft = (_rNrPrm.u1Interlace)?1:0;

    NR_WM(MCNR_13,
        P_Fld(bRegion.wHStart, MCNR_DEMO_HS)|
        P_Fld(bRegion.wHEnd-1, MCNR_DEMO_HE));
    NR_WM(MCNR_14,
        P_Fld((bRegion.wVStart>>bSft)+1, MCNR_DEMO_VS)|
        P_Fld((bRegion.wVEnd>>bSft)+1, MCNR_DEMO_VE));
    NR_WM(MCNR_00, P_Fld((bOnOff?1:0), MCNR_DEMO_MODE)|P_Fld(1, MCNR_DEMO_INSIDE));
}


