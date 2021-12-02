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
 * $RCSfile: drv_di_cli.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#include "drv_di.h"
#include "drv_di_int.h"
#include "drv_nr.h"
#include "drv_scaler.h"
#include "drv_scpos.h"
#include "di_debug.h"
#include "eeprom_if.h"
#include "hw_di.h"
#include "hw_di_int.h"
#include "hw_scpos.h"
#include "hw_sys.h"
#include "hw_ckgen.h"
#include "hw_ycproc.h"
#include "vdo_misc.h"
#ifdef CC_FLIP_MIRROR_SUPPORT
#include "drv_display.h"
#endif

typedef struct
{
    UINT32 u4DataMode;
    const CHAR* szName;
} DATA_MODE_MAPPING_T;

static DATA_MODE_MAPPING_T _arDiDataModeList[] =
{
    { E_DI_FULL_1W3R,   "Full 1W3R" },
    { E_DI_FULL_1W4R,   "Full 1W4R" },
    { E_DI_CBOB_1W3R,   "C Bob 1W3R" },
    { E_DI_CBOB_1W4R,   "C Bob 1W4R" },
    { E_DI_YCBOB_0W0R,  "YC Bob 0W0R" },
    { E_DI_YCBOB_1W1R,  "YC Bob 1W1R" },
    { E_DI_FLIP_MIRROR_1W4R, "Flip Mirror 1W4R" },
    { E_DI_FLIP_MIRROR_CBOB_1W4R, "Flip Mirror CBOB_1W4R" },
    { E_DI_FLIP_MIRROR_YCBOB_1W1R, "Flip Mirror YCBOB_1W1R" },
    { E_DI_UNKNOWN,     "Unknown" },
};

static DATA_MODE_MAPPING_T _arNrDataModeList[] =
{
    { E_DI_NR_OFF,     "NR OFF" },
    { E_DI_NR_Y_ONLY,  "NR Y ONLY" },
    { E_DI_NR_YC_420,  "NR YC 420" },
    { E_DI_NR_YC_422,  "NR YC 422" },
    { E_DI_NR_RGB,     "NR RGB" },
    { E_DI_NR_UNKNOWN, "Unknown" },
};

typedef enum
{
    E_STA_PXL_OVERFLOW,     
    E_STA_PXL_UNDERFLOW,     
    E_STA_SC_UNDERFLOW,
    E_STA_NR_UNDERFLOW,     
    E_STA_FLG_OVERFLOW,     
    E_STA_FLG_UNDERFLOW,
    E_PSCAN_STA_NUM
} E_PSCAN_STATUS;


UINT32 u4FlagStaCnt[VDP_MAX_NS][E_PSCAN_STA_NUM] = {0, 0, 0, 0, 0};
#define DI_STATUS_CNT 0x20


#ifdef CC_FLIP_MIRROR_SUPPORT
extern UINT8 u1FlipMirrorConfig;
#endif

void vDrvDISetDebugDataMode(UINT8 u1VdpId, E_DI_DMODE bMode)
{
    _arMDDiPrm[u1VdpId].u1DebugDataMode = bMode;
    if (bMode == E_DI_UNKNOWN) // reset data mode to follow srm setting
    {
        _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay = DI_DISP_UNKNOWN;
    }
}

//TODO: remove this function if nobody else uses this?
UINT8 u1DrvDIGetDebugDataMode(UINT8 u1VdpId)
{
    return _arMDDiPrm[u1VdpId].u1DebugDataMode;
}

void vDrvDISetDispmodeOff(UINT8 u1VdpId)
{
    _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay = DI_DISP_UNKNOWN;
    _arMDDiPrm[u1VdpId].u1DebugDataMode = E_DI_UNKNOWN;
}

void vDrvDISetPSCANDisplayForDump(UINT8 u1VdpId, UINT8 u1OnOff)
{
    if (u1VdpId == VDP_2)
    {
        return;
    }

    // Enable dump -> disable dispmode, Disable dump -> enable dispmode
    _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay = ((u1OnOff) ? DI_DISP_OFF : DI_DISP_UNKNOWN);
    _arMDDiPrm[u1VdpId].u1DebugDataMode = E_DI_UNKNOWN;
}

void vDrvDISetDebugPSCANDisplay(UINT8 u1VdpId, UINT8 u1OnOff)
{
    UINT8 u1IsPScanFlipMirror;
    if (u1VdpId == VDP_2)
    {
        return;
    }

    if (!u1OnOff) // disable dispmode
    {
        _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay = DI_DISP_OFF;
        _arMDDiPrm[u1VdpId].u1DebugDataMode = E_DI_UNKNOWN;
        return;
    }

    #ifdef CC_FLIP_MIRROR_SUPPORT
        u1IsPScanFlipMirror = (u4GetFlipMirrorModule(u1VdpId)==FLIP_BY_PSCAN) ? 1 : 0;
    #else
        u1IsPScanFlipMirror = 0;
    #endif
    
    if (_arMDDiPrm[u1VdpId].u1Interlace)
    {
        _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay = DI_INTERLACE_DISP;
        if(u1IsPScanFlipMirror)
        {
            _arMDDiPrm[u1VdpId].u1DebugDataMode = E_DI_FLIP_MIRROR_1W4R;
        }
        else
        {
            _arMDDiPrm[u1VdpId].u1DebugDataMode = E_DI_FULL_1W4R;
        }
    }
    else
    {
        _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay = DI_PROGRESSIVE_DISP;
        if(u1IsPScanFlipMirror)
        {
            _arMDDiPrm[u1VdpId].u1DebugDataMode = E_DI_FLIP_MIRROR_YCBOB_1W1R;
        }
        else
        {
            _arMDDiPrm[u1VdpId].u1DebugDataMode = E_DI_YCBOB_1W1R;
        }
    }
}

UINT8 u1DrvDIGetDebugPSCANDisplay(UINT8 u1VdpId)
{
    return _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay;
}

void vDrvDIFWOnOff(UINT8 u1Item, UINT8 u1OnOff)
{
    switch (u1Item)
    {
        case 0:
            vIO32WriteFldAlign(ADAPTIVE_REG, u1OnOff, ADAPTIVE_PSCAN_MAIN);
            Printf("PSCAN main Turn %s FW control \n", u1OnOff? "on" : "off");
            break;
        case 1:
            vIO32WriteFldAlign(ADAPTIVE_REG, u1OnOff, ADAPTIVE_PSCAN_SUB);
            Printf("PSCAN sub Turn %s FW control \n", u1OnOff ? "on": "off");
            break;
        case 2:
            vIO32WriteFldAlign(ADAPTIVE_REG, u1OnOff, ADAPTIVE_PSCAN_QTY);
            Printf("PSCAN Turn %s FW quality \n", u1OnOff ? "on" : "off");
            break;
        default:
            Printf("Unknown item %d ", u1Item);
            break;
    }
}

static const CHAR* vDrvDataModeToString(E_DI_DMODE eMode)
{
    UINT32 u4Num, i;
    const CHAR* szName = "Unknown";

    u4Num = (sizeof (_arDiDataModeList) / sizeof (DATA_MODE_MAPPING_T)) - 1;

    for (i = 0; i < u4Num; i++)
    {
    	if (_arDiDataModeList[i].u4DataMode == eMode)
    	{
            szName = _arDiDataModeList[i].szName;
            break;
    	}
    }
    return szName;
}

static const CHAR* vDrvNRDataModeToString(E_NR_DMODE eMode)
{
    UINT32 u4Num, i;
    const CHAR* szName = "Unknown";

    u4Num = (sizeof (_arNrDataModeList) / sizeof (DATA_MODE_MAPPING_T)) - 1;

    for (i = 0; i < u4Num; i++)
    {
        if (_arNrDataModeList[i].u4DataMode == eMode)
        {
            szName = _arNrDataModeList[i].szName;
            break;
        }
    }
    return szName;
}

void vDrvDIGetInfo(UINT8 u1VdpId, UINT8 u1DbgIdx)
{
    switch (u1DbgIdx)
    {
        case 0:
            Printf("\nPSCAN %s Debug Information\n", u1VdpId ? "Sub" : "Main");
            Printf("PSCAN             %s\n",  _arMDDiPrm[u1VdpId].u1OnOff ? "On" :  "Off");
            Printf("Sorce             %s\n",  _arMDDiPrm[u1VdpId].u1Interlace ? "Interlace" : "Progressive");
            Printf("Data mode         %s\n", vDrvDataModeToString(_arMDDiPrm[u1VdpId].u1DataMode));
            Printf("NR Data mode      %s\n", vDrvNRDataModeToString(_arMDDiPrm[u1VdpId].u1NRDataMode));
            Printf("PSCAN Disp mode   %s\n", _arMDDiPrm[u1VdpId].u1PSCANDisplay ? "Yes" :  "No");
            Printf("Bob mode          %s\n", _arMDDiPrm[u1VdpId].u1Bob ? "Yes" :  "No");
            Printf("Bit Res           %s\n", (_arMDDiPrm[u1VdpId].u1BitRes == 10) ? "10 bit" : "8 bit");
            Printf("Color Mode        %s\n", (_arMDDiPrm[u1VdpId].u1420Mode) ? "420" : "422");
            Printf("DecType           %d\n\n", _arMDDiPrm[u1VdpId].u1DecType);
            Printf("HTotal            %d\n", _arMDDiPrm[u1VdpId].u2HTotal);
            Printf("VTotal            %d\n", _arMDDiPrm[u1VdpId].u2VTotal);
            Printf("Width             %d\n", _arMDDiPrm[u1VdpId].u2Width);
            Printf("Height            %d\n\n", _arMDDiPrm[u1VdpId].u2Height);
            Printf("HClip             %d\n", _arMDDiPrm[u1VdpId].u2HClip);
            Printf("VClip             %d\n", _arMDDiPrm[u1VdpId].u2VClip);
            Printf("DTVClip           %d\n\n", _arMDDiPrm[u1VdpId].u2DTVClip);

            Printf("\nFBM Infromation\n");
            Printf("Addr Start        %08x\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Addr);
            Printf("Addr End          %08x\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Addr+_arMDDiPrm[u1VdpId].rFbmPool.u4Size);
            Printf("Width             %d\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Width);
            Printf("Height            %d\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Height);
            Printf("Mode              %08x\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Mode);
            Printf("10bit             %d\n", _arMDDiPrm[u1VdpId].rFbmPool.fg10bit);
            Printf("422               %d\n\n", _arMDDiPrm[u1VdpId].rFbmPool.fg422);
            break;
            
        case 1:
            Printf("\nNR %s Debug Information\n", u1VdpId ? "Sub" : "Main");
            Printf("HDE:  %d\n", MDDI_READ_FLD(u1VdpId, MDDI_SC_0B, STA_NR_HDE));
            Printf("DATA_EN:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_SC_0B, STA_NR_DATA_EN) ? "Enable" : "Disable");
            Printf("READ_EN:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_SC_0B, STA_NR_READ_ENABLE) ?  "Enable" : "Disable");
            Printf("C_DELAY_SEL:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_SC_0B, STA_NR_C_DELAY_SEL) ? "C Delay 1 Line" : "Y/C Sync");
            Printf("PROGRESSIVE:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_SC_0B, STA_NR_PROGRESSIVE) ? "Progressive" : "Interlace");
            switch(MDDI_READ_FLD(u1VdpId, MDDI_SC_0B, STA_NR_YUV_MODE))
            {
                case 0:
                    Printf("YUV_MODE:  Y only\n");
                    break;
                case 1:
                    Printf("YUV_MODE:  420\n");
                    break;
                case 2:
                    Printf("YUV_MODE:  422\n");
                    break;
                default:
                    Printf("YUV_MODE:  444\n");
                    break;
            }
            Printf("MAIN_PIP_SEL:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_SC_0B, STA_NR_MAIN_PIP_SEL) ? "Sub" : "Main");
            Printf("BIT_SEL_Y:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_SC_0B, STA_NR_BIT_SEL_RY) ? "8 bit" : "10 bit");
            Printf("BIT_SEL_U:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_SC_0B, STA_NR_BIT_SEL_RU) ? "8 bit" : "10 bit");
            Printf("BIT_SEL_V:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_SC_0B, STA_NR_BIT_SEL_RV) ? "8 bit" : "10 bit");
            break;
            
        case 2:
            Printf("Pixel Overflow  Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_PXL_OVERFLOW]);
            Printf("Pixel Underflow Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_PXL_UNDERFLOW]);
            Printf("NR    Underflow Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_NR_UNDERFLOW]);
            Printf("Flag  Overflow  Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_FLG_OVERFLOW]);
            Printf("Flag  Underflow Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_FLG_UNDERFLOW]);
            Printf("SC    Underflow Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_SC_UNDERFLOW]);
            break;
        
        default:
            _arMDDiPrm[u1VdpId].u1DebugIndex = u1DbgIdx;
            break;
    }
}

void vDrvDISetOSD(UINT8 u1Item, UINT8 u1OnOff)
{
    switch (u1Item)
    {
    	case 0: // Probe
            vRegWriteFld(MDDI_OC_04, (u1OnOff ? 6 : 0), OSD_MODE);
            vRegWriteFldMulti(MDDI_OC_04,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 1: // Film
            vRegWriteFld(MDDI_OC_04, (u1OnOff ? 1 : 0), OSD_MODE);
            vRegWriteFldMulti(MDDI_OC_04,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 2: // CS
            vRegWriteFld(MDDI_OC_04, (u1OnOff ? 3 : 0), OSD_MODE);
            vRegWriteFldMulti(MDDI_OC_04,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 3: // MEMA
            vRegWriteFld(MDDI_OC_04, (u1OnOff ? 4 : 0), OSD_MODE);
            vRegWriteFldMulti(MDDI_OC_04,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 4: // KERNEL
            vRegWriteFld(MDDI_OC_04, (u1OnOff ? 5 : 0), OSD_MODE);
            vRegWriteFldMulti(MDDI_OC_04,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 5: // Input Border
            vRegWriteFld(MDDI_SC_01, u1OnOff, SRC_BOT_BORDER_EN);
            vRegWriteFld(MDDI_SC_01, u1OnOff, SRC_TOP_BORDER_EN);
            break;
    	case 6: // Output Border
            vRegWriteFld(MDDI_OC_01, u1OnOff, BORDER_EN);
            break;
    	default:
            break;
    }
}

void vDrvDIGetBufAddr(UINT8 u1VdpId, UINT32 *u4AddrStart, UINT32 *u4AddrEnd)
{
    *u4AddrStart = (MDDI_READ_FLD(u1VdpId, MDDI_MC_0F, DA_WADDR_LO_LIMIT) << 11);
    
    if (u1VdpId == VDP_1) // Sub path no flag dram access
    {
        *u4AddrEnd = (MDDI_READ_FLD(u1VdpId, MDDI_MC_10, DA_FLAG_WADDR_HI_LIMIT) << 4);
    }
    else
    {
        *u4AddrEnd = (MDDI_READ_FLD(u1VdpId, MDDI_MC_0E, DA_WADDR_HI_LIMIT) << 11);
    }    
}

void vDrvDIOnOff(UINT8 u1VdpId, UINT8 bOnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_00, !bOnOff, BYPASS_EN);
}

void vDrvDISetFreeze(UINT8 u1VdpId, UINT8 u1Field)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_01, u1Field, FREEZE_TOP_BOT_SEL);
}

void vDrvDISetColorMode(UINT8 u1VdpId, UINT8 u1ColorMode)
{
    if ((_arMDDiPrm[u1VdpId].u1PSCANDisplay == DI_PROGRESSIVE_DISP)
    && ((BSP_GetIcVersion() == IC_VER_5365_AA) || (BSP_GetIcVersion() == IC_VER_5395_AA)))
    {
        return;
    }
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_00, (u1ColorMode ? 0 : 1), M422);    
}

void vDrvDISetFieldInverse(UINT8 u1VdpId, UINT8 u1Inv)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_01, u1Inv, FLD_IN_P);
}

void vDrvDISetClkDiv(UINT8 u1VdpId, UINT16 u2N, UINT16 u2M)
{
    vIO32WriteFldAlign(CKGEN_PS_DIG_DIVCFG, u2N, FLD_IDEAL_PLL_N);
    vIO32WriteFldAlign(CKGEN_PS_DIG_DIVCFG, u2M, FLD_IDEAL_PLL_M);
}

void vDrvDIForceRead8bit(UINT8 u1VdpId, UINT8 b8bit)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_08, b8bit, DA_FORCE_8BIT_RY);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_08, b8bit, DA_FORCE_8BIT_RC);
}

void vDrvDISetWritePrecision(UINT8 u1VdpId, UINT8 b8bit)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, b8bit, DA_BIT_SEL_WY);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, b8bit, DA_BIT_SEL_WU);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, b8bit, DA_BIT_SEL_WV);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, b8bit, DA_SC_BIT_SEL_RY);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, b8bit, DA_SC_BIT_SEL_RU);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, b8bit, DA_SC_BIT_SEL_RV);
}

void vDrvDIClearStatus(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_09, u1OnOff, DA_OVERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_08, u1OnOff, DA_UNDERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_0E, u1OnOff, DA_SC_UNDERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_09, u1OnOff, DA_NR_UNDERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_07, u1OnOff, DA_FLAG_OVERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_07, u1OnOff, DA_FLAG_UNDERFLOW_CLR);
}

void vDrvDIChkStatus(UINT8 u1VdpId)
{   
    if (_arMDDiPrm[u1VdpId].u1StatusCnt == DI_STATUS_CNT)
    {
        _arMDDiPrm[u1VdpId].u1StatusCnt = 0;    
  
        if (MDDI_READ_FLD(u1VdpId, MDDI_SC_0B, STA_OVERFLOW))
        {
            u4FlagStaCnt[u1VdpId][E_STA_PXL_OVERFLOW]++;
        }
        
        if (MDDI_READ_FLD(u1VdpId, MDDI_KC_08, STA_UNDERFLOW))
        {
            u4FlagStaCnt[u1VdpId][E_STA_PXL_UNDERFLOW]++;
        }
        
        if (u1VdpId == bDrvNRGetCurrentPath())
        {
            if (MDDI_READ_FLD(u1VdpId, MDDI_SC_0B, STA_NR_UNDERFLOW))
            {
                u4FlagStaCnt[u1VdpId][E_STA_NR_UNDERFLOW]++;
            }    
        }
        
        if (u1VdpId == VDP_1)
        {        
            if (MDDI_READ_FLD(u1VdpId, MDDI_OC_0F, STA_SC_UNDERFLOW))
            {
                u4FlagStaCnt[u1VdpId][E_STA_SC_UNDERFLOW]++;
            }  
            
            if (MDDI_READ_FLD(u1VdpId, MDDI_KC_07, STA_FLAG_OVERFLOW))
            {
                u4FlagStaCnt[u1VdpId][E_STA_FLG_OVERFLOW]++;
            }  
            
            if (MDDI_READ_FLD(u1VdpId, MDDI_KC_07, STA_FLAG_UNDERFLOW))
            {
                u4FlagStaCnt[u1VdpId][E_STA_FLG_UNDERFLOW]++;
            }
        }
        vDrvDIClearStatus(u1VdpId, SV_ON);
        vDrvDIClearStatus(u1VdpId, SV_OFF);
    }   
}

UINT32 u4DrvDIGetHBlank(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MDDI_KC_04, H_BLANK);
}

void vDrvDISetPattern(UINT8 u1VdpId, UINT8 u1OnOff)
{
    UINT8 u1Val = ((u1OnOff == 1) ? 0x7 : 0x0);
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_01, u1Val, PAT_GEN_MODE_Y);
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_01, u1Val, PAT_GEN_MODE_CB);
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_01, u1Val, PAT_GEN_MODE_CR);
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_01, u1OnOff, BORDER_EN);
}

void vDrvDISetInputBorder(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_01, u1OnOff, SRC_TOP_BORDER_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_01, u1OnOff, SRC_BOT_BORDER_EN);
}

void vDrvDIConfigFlipMirror(void)
{
#ifdef CC_FLIP_MIRROR_SUPPORT
        UINT8 u1Config = u1GetFlipMirrorConfig();

        if ((u1Config & SYS_MIRROR_CONFIG_ON) 
        && (u1Config & SYS_FLIP_CONFIG_ON))
        {
            u1FlipMirrorConfig = DI_FLIP_MIRROR_ON_MODE;
        }
        else if (u1Config & SYS_MIRROR_CONFIG_ON)
        {
            u1FlipMirrorConfig = DI_MIRROR_ON_MODE;
        }
        else if (u1Config & SYS_FLIP_CONFIG_ON)
        {
            u1FlipMirrorConfig = DI_FLIP_ON_MODE;
        }
        else
        {
            u1FlipMirrorConfig = DI_NORMAL_MODE;
        }        
#endif
}

void vDrvDISetFlipMirror(UINT8 u1VdpId, UINT8 u1Mode)
{       
    // proressive 420 dispmode
    if (((!_arMDDiPrm[u1VdpId].u1Interlace) && _arMDDiPrm[u1VdpId].u1420Mode)
        || _arMDDiPrm[u1VdpId].u1Interlace)
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, DI_NORMAL_MODE, DA_SC_DISPLAY_MODE);
        MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, u1Mode, DA_DISPLAY_MODE); //420
        vDrvNRSetFlipMirror(u1VdpId, u1Mode);
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, DI_NORMAL_MODE, DA_DISPLAY_MODE);
        MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, u1Mode, DA_SC_DISPLAY_MODE); //422 & 444
        vDrvNRSetFlipMirror(u1VdpId, DI_NORMAL_MODE);
    }
}

void vDrvDISetROIPixel(UINT8 bPath, UINT32 u4RoiPixel)
{
    MDDI_WRITE_FLD(bPath, MDDI_FUSION_0E, u4RoiPixel, IF_ROI_PIXELS);
    MDDI_WRITE_FLD(bPath, MDDI_FUSION_0E, 0x0, IF_ROI_V);
    MDDI_WRITE_FLD(bPath, MDDI_FUSION_0E, 0x0, IF_ROI_H);
}

void vDrvDIInitCRC(UINT8 u1VdpId,UINT8 preprocMode)
{
    /* PSCAN CRC */
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_09, 0x1, SRCR_CRC_BOT_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_09, 0x1, SRCR_CRC_TOP_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_09, 0x1, SRCR_CRC_ODD_LINE_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_09, 0x1, SRCR_CRC_EVEN_LINE_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_09, 0x3ff, SRCR_CRC_Y_MASK);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_09, 0x3ff, SRCR_CRC_C_MASK);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_09, 0x0, SRCR_CRC_AUTO_FIELD_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_09, 0x0, SRCR_CRC_SEL_DET);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_09, 0x2, SRCR_CRC_SEL_FIELD);

    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_06, 0x1, SRCW_CRC_BOT_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_06, 0x1, SRCW_CRC_TOP_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_04, 0x1, SRCW_CRC_ODD_LINE_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_04, 0x1, SRCW_CRC_EVEN_LINE_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_06, 0x3ff, SRCW_CRC_Y_MASK);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_06, 0x3ff, SRCW_CRC_C_MASK);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_06, 0x0, SRCW_CRC_V_MASK);    

    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_09, 0x3ff, OUT_CRC_Y_MASK);
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_09, 0x3ff, OUT_CRC_C_MASK);
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_09, 0x0, OUT_CRC_V_MASK);

    /* OMUX CRC */
    vRegWriteFldAlign(SYS_04, (u1VdpId == VDP_1) ? 0 : 1, CRC_SRC_SEL);
    vRegWriteFldAlign(PQCRC_00, 0x3FF, PQCRC_Y_MSK);
    vRegWriteFldAlign(PQCRC_00, 0x3FF, PQCRC_C_MSK);
    vRegWriteFldAlign(PQCRC_00, 0x3FF, PQCRC_V_MSK);
    vRegWriteFldAlign(PQCRC_00, 0x1, PQCRC_TOP_EN);
    vRegWriteFldAlign(PQCRC_00, 0x1, PQCRC_BOT_EN);
    vRegWriteFldAlign(PQCRC_01, 0x1, PQCRC_EVEN_LINE_EN);
    vRegWriteFldAlign(PQCRC_01, 0x1, PQCRC_ODD_LINE_EN);

    /* PREPROC CRC */
    vRegWriteFldAlign(TOP_MAIN_00, preprocMode, C_CRC_SEL); /* 0: main_in, 1: main_out, 2: pip_in, 3: pip_out */
    vRegWriteFldAlign(CRC_00, 0x3FF, C_Y_MASK_IN);
    vRegWriteFldAlign(CRC_00, 0x3FF, C_C_MASK_IN);
    vRegWriteFldAlign(CRC_00, 0x3FF, C_V_MASK_IN);
    vRegWriteFldAlign(CRC_00, 0x1, C_TOP_EN);
    vRegWriteFldAlign(CRC_00, 0x1, C_BOT_EN);
}

UINT32 u4DrvDIGetInputCRC(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MDDI_SC_08, SRCW_CRC_OUT_V);
}

UINT32 u4DrvDIGetOutputCRC(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MDDI_OC_0B, OUT_CRC_OUT_V);
}

void vDrvDISetBgn(UINT8 u1VdpId, UINT8 u1S_V_Bgn, UINT8 u1K_H_Bgn, UINT8 u1K_V_Bgn)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_01, u1S_V_Bgn, SRCW_V_BGN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_01, u1K_V_Bgn, KERNEL_V_BGN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_01, u1K_H_Bgn, KERNEL_H_BGN);
}

void vDrvDISetFldPrediction(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_01, u1OnOff, FLD_PRED_EN);
}

void vDrvDISetReSyncOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
	vRegWriteFldAlign(PAT_MAIN_00, u1OnOff, C_MAIN_F_V_SHK);	 //Field resync by Vsync when vsize is reduced.
}

void vDrvDIEnableISR(UINT8 u1VdpId, UINT8 u1Enable)
{
    if (u1VdpId == VDP_1)
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, u1Enable, INT_MOVS_R_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, u1Enable, INT_MOVS_F_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, u1Enable, INT_MIVS_R_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, u1Enable, INT_MIVS_F_EN);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, u1Enable, INT_POVS_R_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, u1Enable, INT_POVS_F_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, u1Enable, INT_PIVS_R_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, u1Enable, INT_PIVS_F_EN);
    }
}

void vDrvDISetKDE(UINT8 u1VdpId, UINT16 u2N, UINT16 u2M)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_01, u2N, KDE_N);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_01, u2M, KDE_M);
}

void vDrvDISetSDE(UINT8 u1VdpId, UINT8 bOnOff, UINT16 u2N, UINT16 u2M)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_02, bOnOff, AUTO_SDE_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_02, bOnOff, AUTO_SDE_IIR_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_02, u2N, SDE_N);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_02, u2M, SDE_M);
}

void vDrvDISetODE(UINT8 u1VdpId, UINT8 bOnOff, UINT16 u2N, UINT16 u2M)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_00, bOnOff, AUTO_ODE_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_00, bOnOff, AUTO_ODE_IIR_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_00, u2N, ODE_N);
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_00, u2M, ODE_M);
}

void vDrvDISetBufferAddr(UINT8 u1VdpId, UINT32 u4AddrYMsb, UINT32 u4AddrYLsb, UINT32 u4AddrUMsb, 
                    UINT32 u4AddrULsb, UINT32 u4AddrVMsb, UINT32 u4AddrVLsb, UINT32 u4AddrMVFlg, UINT32 u4AddrLMFlg)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_06, u4AddrYMsb, DA_ADDR_BASE_MSB_Y);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_07, u4AddrYLsb, DA_ADDR_BASE_LSB_Y);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_08, u4AddrUMsb, DA_ADDR_BASE_MSB_U);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_09, u4AddrULsb, DA_ADDR_BASE_LSB_U);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_0A, u4AddrVMsb, DA_ADDR_BASE_MSB_V);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_0B, u4AddrVLsb, DA_ADDR_BASE_LSB_V);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_0C, u4AddrMVFlg, DA_FLAG_MV_ADDR);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_0D, u4AddrLMFlg, DA_FLAG_LM_ADDR);
}

void vDrvDISetBufferProtect(UINT8 u1VdpId, UINT32 u4Lo, UINT32 u4Hi, UINT32 u4FlgLo, UINT32 u4FlgHi)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_0F, u4Lo, DA_WADDR_LO_LIMIT);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_0E, u4Hi, DA_WADDR_HI_LIMIT);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_11, u4FlgLo, DA_FLAG_WADDR_LO_LIMIT);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_10, u4FlgHi, DA_FLAG_WADDR_HI_LIMIT);
}

void vDrvDISetFrmNum(UINT8 u1VdpId, UINT8 u1YNum, UINT8 u1UNum, UINT8 u1VNum)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, u1YNum, DA_FRAME_NUM_Y);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, u1UNum, DA_FRAME_NUM_U);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, u1VNum, DA_FRAME_NUM_V);	
}

void vDrvDISetDoubleBuffer(UINT8 u1VdpId, UINT8 u1OnOff)
{
    if (u1VdpId == VDP_1)
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, u1OnOff, MAIN_READ_V_PROTECT);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, u1OnOff, MAIN_WRITE_V_PROTECT);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, u1OnOff, PIP_READ_V_PROTECT);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, u1OnOff, PIP_WRITE_V_PROTECT);
    }    
}

void vDrvDISetClkReset(UINT8 u1VdpId, UINT8 u1OnOff)
{
    /* Note */
    /* Please follow the sequence when doing reset. */
    /* If the reset sequence is not correct, prefetch error may occur. */
    if (u1OnOff)
    {
        if (u1VdpId == VDP_1)
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_ON, DRAM_M_CK_RST);
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_ON, PS_IN_M_CK_RST);
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_ON, PS_K_M_CK_RST);
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_ON, PS_OUT_M_CK_RST);
        }
        else
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_ON, DRAM_P_CK_RST);
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_ON, PS_IN_P_CK_RST);
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_ON, PS_K_P_CK_RST);
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_ON, PS_OUT_P_CK_RST);
        }
    }
    else
    {
        if (u1VdpId == VDP_1)
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_OFF, PS_OUT_M_CK_RST);
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_OFF, PS_K_M_CK_RST);
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_OFF, PS_IN_M_CK_RST);
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_OFF, DRAM_M_CK_RST);
        }
        else
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_OFF, PS_OUT_P_CK_RST);
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_OFF, PS_K_P_CK_RST);
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_OFF, PS_IN_P_CK_RST);
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, SV_OFF, DRAM_P_CK_RST);
        }
    }
}

void vDrvDISRCWOutIOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_02, u1OnOff, SRCW_OUT_I);
}

void vDrvDIProgModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_00, u1OnOff, FRAME420);
}

void vDrvDISetFrcLine(UINT8 u1VdpId, UINT8 u1FrcLine)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_01, u1FrcLine, FRC_LINE);
}

void vDrvDISetStdVMask(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_01, u1OnOff, NON_STD_VMASK_EN);
}

void vDrvDISetStdVBlank(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_01, u1OnOff, NON_STD_VBLANK_SEL);
}

void vDrvDISetAutoStop(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_02, u1OnOff, SRCW_AUTO_STOP);
}

void vDrvDISetClip(UINT8 u1VdpId, UINT16 u2HClip, UINT16 u2VClip)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_02, u2HClip, X_SHIFT);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_02, u2VClip, Y_SHIFT);
}

void vDrvDISetClock(UINT8 u1VdpId, UINT8 u1OnOff)
{
    if (u1VdpId == VDP_1)
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, u1OnOff, DRAM_M_CK_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, u1OnOff, PS_OUT_M_CK_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, u1OnOff, PS_K_M_CK_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, u1OnOff, PS_IN_M_CK_EN);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, u1OnOff, DRAM_P_CK_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, u1OnOff, PS_OUT_P_CK_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, u1OnOff, PS_K_P_CK_EN);
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, u1OnOff, PS_IN_P_CK_EN);
    }

    MDDI_WRITE_FLD(VDP_1, MDDI_GC_00, u1OnOff, PS_K_M_PQ_CK_EN);
}

void vDrvDISetErrDefusion(UINT8 u1VdpId, UINT8 u18bit)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_00, u18bit, ERR_DFU);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_00, u18bit, ERR_DFU_RANDOM_INI);    
}
