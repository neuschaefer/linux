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

#ifdef CC_MT5395
#include "drv_mjc_if.h"
#endif

//-----------------------------------------------------------------------------
//MTAL VDO PQ related stuff
//-----------------------------------------------------------------------------
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
    VDO_PQ_YLEV_SHARP,
    VDO_PQ_BOB_MODE,
    VDO_PQ_FILM_ON_OFF,
    VDO_PQ_CHROMA_CORING,
    VDO_PQ_RGB_PATTERN,
    VDO_PQ_MJC_FBCK_CURVE,
    VDO_PQ_DIR_SET,
    VDO_PQ_DIR_GET,
    VDO_PQ_LUMA_INFO_GET,
    VDO_PQ_CHROMA_INFO_GET,
    VDO_PQ_FILM_STATUS_GET,
    VDO_PQ_SCE_INPUT_CAP_GET,
    VDO_PQ_SCE_OUTPUT_CAP_GET,
    VDO_PQ_OSTG_CAP_GET,
    VDO_PQ_NS           // total pq items
} VDO_PQ_TYPE_T;

typedef struct 
{
    UINT16 u2RGain;
    UINT16 u2GGain;
    UINT16 u2BGain;
    UINT16 u2ROffset;
    UINT16 u2GOffset;
    UINT16 u2BOffset;
} VDO_PQ_RGB_GAIN_OFST_T;

typedef struct 
{
    UINT16 u2RGain;
    UINT16 u2GGain;
    UINT16 u2BGain;
} VDO_PQ_RGB_GAIN_T;

typedef struct
{
    UINT16 u2ROffset;
    UINT16 u2GOffset;
    UINT16 u2BOffset;
} VDO_PQ_RGB_OFST_T;

typedef struct 
{
    UINT8 u1Enable;
    UINT16 u2R;
    UINT16 u2G;
    UINT16 u2B;
} VDO_PQ_RGB_PATTERN_T;

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

typedef struct 
{
    UINT8 u1Module;
    UINT8 u1FilmType;
    UINT8 u1OnOff;  
} VDO_PQ_FILM_ONOFF_T;

typedef struct 
{
    UINT8 u1Enable;
    UINT8 u1YBound;
    UINT8 u1CbTbl[16];
    UINT8 u1CrTbl[16];
} VDO_PQ_CHROMA_CORING_T;

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
} VDO_PQ_LUMA_INFO_T;

typedef struct 
{
    UINT16 u2ChromaHistL[8];
    UINT16 u2ChromaHistR[8];
    UINT16 u2HueHistL[8];
    UINT16 u2HueHistR[8];
    UINT16 u2WindowL[3];
    UINT16 u2WindowR[3];
} VDO_PQ_CHROMA_INFO_T;

typedef struct 
{
    UINT16 u2PosX;        // AP prepared position. API will not modify it.
    UINT16 u2PosY;        // AP prepared position. API will not modify it.
    UINT16 u2ValCh[3];    // API will return the pixel value
} VDO_PQ_CAPTURE_T;

typedef struct 
{
    UINT8 u1Module;        // AP prepared type: 0 (DI) or 1(MJC)
    UINT8 u1Result;        // return result: 0 (Video), 1 (3:2), 2 (2:2) 
} VDO_PQ_FILM_STATUS_T;

typedef struct 
{
    UINT8 u1OnOff;        
    UINT8 u1GammaOnOff;   
    UINT16 u2Matrix[9];
} VDO_PQ_XVYCC_T;

typedef struct 
{
    UINT8 u1Color;         // M, R, Y, G, C, B
    UINT8 u1Module;        // Luma, Sat, Hue
    UINT8 u1Value;         // 0 ~ 0xFF
} VDO_PQ_EXP_COLOR_T;

typedef struct
{
    UINT8 arMJCQMap[2][16];
    UINT16 arMJCQTbl[7][16];    // arMJCQTbl[5] is reserved
} VDO_PQ_MJC_FBCK_CURVE_T;

////////////////////////////////////////////////////////////////////////////////////////////////////////

static INT32 _PE_SetVdoPqRgbGainOfst(UCHAR ucVdpId, const VDO_PQ_RGB_GAIN_OFST_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_RGB_GAIN_OFST_T))
        return PQ_PARAMETER_ERROR;
    
    wWriteQualityTable(QUALITY_R_GAIN, prData->u2RGain);
    wWriteQualityTable(QUALITY_G_GAIN, prData->u2GGain);
    wWriteQualityTable(QUALITY_B_GAIN, prData->u2BGain);
    wWriteQualityTable(QUALITY_R_OFFSET, prData->u2ROffset);
    wWriteQualityTable(QUALITY_G_OFFSET, prData->u2GOffset);
    wWriteQualityTable(QUALITY_B_OFFSET, prData->u2BOffset);

    vApiRegisterVideoEvent(PE_EVENT_RGB_GAIN_OFST, ucVdpId, SV_ON);

    return PQ_OK;
}

static INT32 _PE_SetVdoPqRgbGain(UCHAR ucVdpId, const VDO_PQ_RGB_GAIN_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_RGB_GAIN_T))
        return PQ_PARAMETER_ERROR;
    
    vHalVideoSetRGBGain(ucVdpId, prData->u2RGain, prData->u2GGain, prData->u2BGain);

    return PQ_OK;
}

static INT32 _PE_SetVdoPqRgbOfst1(UCHAR ucVdpId, const VDO_PQ_RGB_OFST_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_RGB_OFST_T))
        return PQ_PARAMETER_ERROR;
    
    vHalVideoSetRGBOfst1(ucVdpId, prData->u2ROffset, prData->u2GOffset, prData->u2BOffset);
    
    return PQ_OK;
}

static INT32 _PE_SetVdoPqRgbOfst2(UCHAR ucVdpId, const VDO_PQ_RGB_OFST_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_RGB_OFST_T))
        return PQ_PARAMETER_ERROR;
    
    vHalVideoSetRGBOfst2(ucVdpId, prData->u2ROffset, prData->u2GOffset, prData->u2BOffset);
    
    return PQ_OK;
}

static INT32 _PE_SetExpColor(UCHAR ucVdpId, const VDO_PQ_EXP_COLOR_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_EXP_COLOR_T))
        return PQ_PARAMETER_ERROR;
    
    vHalSetSCEAngular(prData->u1Module, prData->u1Color, prData->u1Value);
    
    return PQ_OK;
}

static INT32 _PE_SetVdoPqXvYcc(UCHAR ucVdpId, const VDO_PQ_XVYCC_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_XVYCC_T))
        return PQ_PARAMETER_ERROR;

    vHalVideoSetXVYCC(ucVdpId, prData->u1OnOff, prData->u1GammaOnOff, prData->u2Matrix);
    
    return PQ_OK;
}

static INT32 _PE_SetVdoPqXvRgbGain(UCHAR ucVdpId, const VDO_PQ_RGB_GAIN_T* prData, UINT32 u4Size)
{
    UINT16 u2RgbGain[3];
    
    if (u4Size != sizeof(VDO_PQ_RGB_GAIN_T))
        return PQ_PARAMETER_ERROR;

    u2RgbGain[0] = prData->u2RGain;
    u2RgbGain[1] = prData->u2GGain;
    u2RgbGain[2] = prData->u2BGain;

    vHalVideoSetXVRGBGain(ucVdpId, u2RgbGain);
    
    return PQ_OK;
}

static INT32 _PE_SetVdoPqAdapLumaCurveManual(UCHAR ucVdpId, const UINT8* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(UINT8))
        return PQ_PARAMETER_ERROR;

    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;

    vDrvADLManualLumaCurveOnOff(*prData);
    
    return PQ_OK;
}

static INT32 _PE_SetVdoPqAdapLumaCurve(UCHAR ucVdpId, const UINT16* prData, UINT32 u4Size)
{
    if (u4Size != (LUMA_CURVE_NUM*sizeof(UINT16)))
        return PQ_PARAMETER_ERROR;

    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;
    
    vDrvADLSetLumaCurve(prData);    
    vApiRegisterVideoEvent(PE_EVENT_LUMA_CURVE, VDP_1, SV_ON);

    return PQ_OK;
}

static INT32 _PE_SetVdoPqMatrixInput(UCHAR ucVdpId, const UINT16* prData, UINT32 u4Size)
{
    if (u4Size != (15*sizeof(UINT16)))
            return PQ_PARAMETER_ERROR;

    vSetForceLoadMatrix(SV_ON);
    vLoadColorMatrix(ucVdpId, prData);

    return PQ_OK;
}

static INT32 _PE_SetVdoPqMatrixOutput(UCHAR ucVdpId, const INT32* prData, UINT32 u4Size)
{
    if (u4Size != (15*sizeof(INT32)))
            return PQ_PARAMETER_ERROR;

    vSetForceLoadMatrix(SV_ON);
    vLoadOSColorMatrix(ucVdpId, prData);

    return PQ_OK;
}

static INT32 _PE_SetVdoPqGammaCurve(UCHAR ucVdpId, const UINT8* prData, UINT32 u4Size)
{
    if (u4Size != (GAMMA_INDEX*sizeof(UINT8)*3))
        return PQ_PARAMETER_ERROR;

    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;

    UINT8 u1PrevGammaInDram = GAMMA_IN_DRAM;
    
    SET_GAMMA_IN_DRAM(0);
    
    x_memcpy(GAMMA_256, (UINT8*) prData, u4Size);
    vHalVideoGammaLoadTable(SV_ON);

    SET_GAMMA_IN_DRAM(u1PrevGammaInDram);
    
    return PQ_OK;
}

static INT32 _PE_SetVdoPqSce(UCHAR ucVdpId, const UINT8* prData, UINT32 u4Size)
{
    if (u4Size != SCE_INDEX*sizeof(UINT8))
        return PQ_PARAMETER_ERROR;

    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;
    
    x_memcpy(&DEFAULT_SCE_TBL, (UINT8*) prData, u4Size);
    vApiRegisterVideoEvent(PE_EVENT_SCE, VDP_1, SV_ON);

    return PQ_OK;
}

static INT32 _PE_SetVdoPqYlevSharp(UCHAR ucVdpId, const UINT8* prData, UINT32 u4Size)
{
    if (u4Size != 65*sizeof(UINT8))
        return PQ_PARAMETER_ERROR;

    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;

    vHalVideoYLevSharpness(VDP_1, prData);
    
    return PQ_OK;
}

static INT32 _PE_SetVdoPqBobMode(UCHAR ucVdpId, const UINT8* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(UINT8))
        return PQ_PARAMETER_ERROR;
   
    vDrvDISetBob(ucVdpId, *prData);

    return PQ_OK;
}

static INT32 _PE_SetVdoPqRgbPattern(UCHAR ucVdpId, const VDO_PQ_RGB_PATTERN_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_RGB_PATTERN_T))
        return PQ_PARAMETER_ERROR;

    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;

    if (prData->u1Enable)
    {
        vDrvGammaMute(prData->u2R, prData->u2G, prData->u2B);
    }
    else
    {
        vDrvGammaUnmute();
    }

    return PQ_OK;
}

static INT32 _PE_SetVdoPqFilmOnOff(UCHAR ucVdpId, const VDO_PQ_FILM_ONOFF_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_FILM_ONOFF_T))
        return PQ_PARAMETER_ERROR;
    
    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;

    if (prData->u1Module == ENUM_DI_FILM)
    {
        vDrvDISetFilmModeOnOff(ucVdpId, prData->u1FilmType, prData->u1OnOff);
    }
    else if (prData->u1Module == ENUM_MJC_FILM)
    {
        #ifdef CC_MT5395
        MJC_SetFilmModeTypeOnOff(ucVdpId, prData->u1FilmType, prData->u1OnOff);
        MJC_SetFilmMode(SV_ON);
        #else
        return PQ_NOT_SUPPORTED;
        #endif
    }
    else
    {
        return PQ_PARAMETER_ERROR;
    }

    return PQ_OK;
}

static INT32 _PE_SetVdoPqChromaCoring(UCHAR ucVdpId, const VDO_PQ_CHROMA_CORING_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_CHROMA_CORING_T))
            return PQ_PARAMETER_ERROR;
    
    vDrvChromaCoringOnOff(ucVdpId, prData->u1Enable);
    vDrvChromaCoringYbound(ucVdpId, prData->u1YBound);
    vDrvSetChromaCoringTbl((UINT32)ucVdpId, prData->u1CbTbl, prData->u1CrTbl);

    return PQ_OK;
}

static INT32 _PE_SetVdoPqMjcFbckCurve(UCHAR ucVdpId, const VDO_PQ_MJC_FBCK_CURVE_T* prData, UINT32 u4Size)
{
    #ifdef CC_MT5395
    
    if (u4Size != sizeof(VDO_PQ_MJC_FBCK_CURVE_T))
        return PQ_PARAMETER_ERROR;

    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;

    CRIT_STATE_T csState = x_crit_start();

    MJC_Set_FbckQMap(0, prData->arMJCQMap[0]);
    MJC_Set_FbckQMap(1, prData->arMJCQMap[1]);
    MJC_Set_FbckQTable(0, prData->arMJCQTbl[0]);
    MJC_Set_FbckQTable(1, prData->arMJCQTbl[1]);
    MJC_Set_FbckQTable(2, prData->arMJCQTbl[2]);
    MJC_Set_FbckQTable(3, prData->arMJCQTbl[3]);
    MJC_Set_FbckQTable(4, prData->arMJCQTbl[4]);
    MJC_Set_FbckQTable(6, prData->arMJCQTbl[6]);
    
    x_crit_end(csState);

    return PQ_OK;
    #else
    return PQ_NOT_SUPPORTED;
    #endif
}

static INT32 _PE_GetVdoPqLumaInfo(UCHAR ucVdpId, VDO_PQ_LUMA_INFO_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_LUMA_INFO_T))
        return PQ_PARAMETER_ERROR;
    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;

    vDrvGetAPLLR(&prData->u1APLL, &prData->u1APLR);
    vDrvGetInputLumaMaxLR(&prData->u1LumaMaxL, &prData->u1LumaMaxR);
    vDrvGetInputLumaMinLR(&prData->u1LumaMinL, &prData->u1LumaMinR);
    vDrvGetLumaHistLR(prData->u2LumaHistL, prData->u2LumaHistR, 4096);
    
    return PQ_OK;
}

static INT32 _PE_GetVdoPqChromaInfo(UCHAR ucVdpId, VDO_PQ_CHROMA_INFO_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_CHROMA_INFO_T))
        return PQ_PARAMETER_ERROR;
    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;

    vDrvGetHueHistLR(prData->u2HueHistL, prData->u2HueHistR, 4096);
    vDrvGetSatHistLR(prData->u2ChromaHistL, prData->u2ChromaHistR, 4096);
    vDrvGet2DWindowLR(prData->u2WindowL, prData->u2WindowR, 4096);
    
    return PQ_OK;
}

static INT32 _PE_GetVdoPqSceInputCapture(UCHAR ucVdpId, VDO_PQ_CAPTURE_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_CAPTURE_T))
        return PQ_PARAMETER_ERROR;
    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;

    vDrvSceDbgInfo(prData->u2PosX, prData->u2PosY, prData->u2ValCh, NULL);
    
    return PQ_OK;
}

static INT32 _PE_GetVdoPqSceOutputCapture(UCHAR ucVdpId, VDO_PQ_CAPTURE_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_CAPTURE_T))
        return PQ_PARAMETER_ERROR;
    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;

    vDrvSceDbgInfo(prData->u2PosX, prData->u2PosY, NULL, prData->u2ValCh);
    
    return PQ_OK;
}

static INT32 _PE_GetVdoPqOstgCapture(UCHAR ucVdpId, VDO_PQ_CAPTURE_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_CAPTURE_T))
        return PQ_PARAMETER_ERROR;
    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;

    vDrvOsDbgInfo(prData->u2PosX, prData->u2PosY, prData->u2ValCh);
    
    return PQ_OK;
}

static INT32 _PE_GetVdoPqFilmStatus(UCHAR ucVdpId, VDO_PQ_FILM_STATUS_T* prData, UINT32 u4Size)
{
    if (u4Size != sizeof(VDO_PQ_FILM_STATUS_T))
        return PQ_PARAMETER_ERROR;
    if (ucVdpId != VDP_1)
        return PQ_NOT_SUPPORTED;

    if (prData->u1Module == ENUM_DI_FILM)
    {
        prData->u1Result = bDrvGetFilmStatus();
    }
    else if (prData->u1Module == ENUM_MJC_FILM)
    {
        #ifdef CC_MT5395
        prData->u1Result = u4DrvMJCGetFilmModeStatus();
        #else
        return PQ_NOT_SUPPORTED;
        #endif
    }
    else
    {
        return PQ_PARAMETER_ERROR;
    }
    
    return PQ_OK;
}

static INT32 _PE_SetPqReg(UCHAR ucVdpId, UINT32* prData, UINT32 u4Size)
{
    if (bDrvCheckPqReg(prData, u4Size) == 0)
    {
        return PQ_PARAMETER_ERROR;
    }
    else
    {
        vDrvSetPqReg(prData, u4Size);
        return PQ_OK;
    }
}

static INT32 _PE_GetPqReg(UCHAR ucVdpId, UINT32* prData, UINT32 u4Size)
{
    if (bDrvCheckPqReg(prData, u4Size) == 0)
    {
        return PQ_PARAMETER_ERROR;
    }
    else
    {
        vDrvGetPqReg(prData, u4Size);
        return PQ_OK;
    }
}

INT32 PE_SetPqInfo(UCHAR ucVdpId, UINT32 u4PqType, const void* pkvPqSetInfo, UINT32 u4Size)
{
    INT32 i4Ret = PQ_NOT_SUPPORTED;
    
    switch ((VDO_PQ_TYPE_T) u4PqType)
    {
        case VDO_PQ_RGB_GAIN_OFST:
            i4Ret = _PE_SetVdoPqRgbGainOfst(ucVdpId, (VDO_PQ_RGB_GAIN_OFST_T*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_RGB_GAIN:
            i4Ret = _PE_SetVdoPqRgbGain(ucVdpId, (VDO_PQ_RGB_GAIN_T*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_RGB_OFST1:
            i4Ret = _PE_SetVdoPqRgbOfst1(ucVdpId, (VDO_PQ_RGB_OFST_T*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_RGB_OFST2:
            i4Ret = _PE_SetVdoPqRgbOfst2(ucVdpId, (VDO_PQ_RGB_OFST_T*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_EXP_COLOR:
            i4Ret = _PE_SetExpColor(ucVdpId, (VDO_PQ_EXP_COLOR_T*) pkvPqSetInfo, u4Size);
            break;                
        case VDO_PQ_XVYCC:
            i4Ret = _PE_SetVdoPqXvYcc(ucVdpId, (VDO_PQ_XVYCC_T*) pkvPqSetInfo, u4Size);
            break;    
        case VDO_PQ_XVRGB_GAIN:
            i4Ret = _PE_SetVdoPqXvRgbGain(ucVdpId, (VDO_PQ_RGB_GAIN_T*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_ADAP_LUMA_CURVE_MANUAL:
            i4Ret = _PE_SetVdoPqAdapLumaCurveManual(ucVdpId, (UINT8*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_ADAP_LUMA_CURVE:
            i4Ret = _PE_SetVdoPqAdapLumaCurve(ucVdpId, (UINT16*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_MATRIX_INPUT:
            i4Ret = _PE_SetVdoPqMatrixInput(ucVdpId, (UINT16*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_MATRIX_OUTPUT:
            i4Ret = _PE_SetVdoPqMatrixOutput(ucVdpId, (INT32*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_GAMMA_CURVE:
            i4Ret = _PE_SetVdoPqGammaCurve(ucVdpId, (UINT8*) pkvPqSetInfo, u4Size);
            break;     
        case VDO_PQ_SCE:
            i4Ret = _PE_SetVdoPqSce(ucVdpId, (UINT8*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_YLEV_SHARP:
            i4Ret = _PE_SetVdoPqYlevSharp(ucVdpId, (UINT8*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_BOB_MODE:
            i4Ret = _PE_SetVdoPqBobMode(ucVdpId, (UINT8*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_RGB_PATTERN:
            i4Ret = _PE_SetVdoPqRgbPattern(ucVdpId, (VDO_PQ_RGB_PATTERN_T*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_FILM_ON_OFF:
            i4Ret = _PE_SetVdoPqFilmOnOff(ucVdpId, (VDO_PQ_FILM_ONOFF_T*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_CHROMA_CORING:
            i4Ret = _PE_SetVdoPqChromaCoring(ucVdpId, (VDO_PQ_CHROMA_CORING_T*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_MJC_FBCK_CURVE:
            i4Ret = _PE_SetVdoPqMjcFbckCurve(ucVdpId, (VDO_PQ_MJC_FBCK_CURVE_T*) pkvPqSetInfo, u4Size);
            break;
        case VDO_PQ_DIR_SET:
            i4Ret = _PE_SetPqReg(ucVdpId, (UINT32*) pkvPqSetInfo, u4Size);  
            break;
        default:
            i4Ret = PQ_NOT_SUPPORTED;
            break;
    }
    
    return i4Ret;
}

INT32 PE_GetPqInfo(UCHAR ucVdpId, UINT32 u4PqType, void* pkvPqGetInfo, UINT32 u4Size)
{
    INT32 i4Ret = PQ_NOT_SUPPORTED;
    
    switch ((VDO_PQ_TYPE_T) u4PqType)
    {
        case VDO_PQ_LUMA_INFO_GET:
            i4Ret = _PE_GetVdoPqLumaInfo(ucVdpId, (VDO_PQ_LUMA_INFO_T*) pkvPqGetInfo, u4Size);
            break;
        case VDO_PQ_CHROMA_INFO_GET:
            i4Ret = _PE_GetVdoPqChromaInfo(ucVdpId, (VDO_PQ_CHROMA_INFO_T*) pkvPqGetInfo, u4Size);
            break;
        case VDO_PQ_SCE_INPUT_CAP_GET:
            i4Ret = _PE_GetVdoPqSceInputCapture(ucVdpId, (VDO_PQ_CAPTURE_T*) pkvPqGetInfo, u4Size);
            break;
        case VDO_PQ_SCE_OUTPUT_CAP_GET:
            i4Ret = _PE_GetVdoPqSceOutputCapture(ucVdpId, (VDO_PQ_CAPTURE_T*) pkvPqGetInfo, u4Size);
            break;
        case VDO_PQ_OSTG_CAP_GET:
            i4Ret = _PE_GetVdoPqOstgCapture(ucVdpId, (VDO_PQ_CAPTURE_T*) pkvPqGetInfo, u4Size);
            break;
        case VDO_PQ_FILM_STATUS_GET:
            i4Ret = _PE_GetVdoPqFilmStatus(ucVdpId, (VDO_PQ_FILM_STATUS_T*) pkvPqGetInfo, u4Size);
            break;
        case VDO_PQ_DIR_GET:
            i4Ret = _PE_GetPqReg(ucVdpId, (UINT32*) pkvPqGetInfo, u4Size);  
            break;        
        default:
            i4Ret = PQ_NOT_SUPPORTED;
            break;
    }
    
    return i4Ret;
}


