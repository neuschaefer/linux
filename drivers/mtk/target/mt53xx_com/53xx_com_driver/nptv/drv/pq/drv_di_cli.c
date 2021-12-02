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


////////////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////////////

#include "drv_di.h"
#include "drv_di_int.h"
#include "drv_scaler.h"
#include "hw_di.h"
#include "hw_nr.h"
#include "hw_di_int.h"
#include "hw_ycproc.h"
#include "hw_vdoin.h"
#include "hw_ckgen.h"
#include "hw_scpos.h"
#include "vdo_misc.h"
#include "drv_display.h"

//#include "nptv_debug.h"
#include "di_debug.h"

////////////////////////////////////////////////////////////////////////////////
// External function prototypes
////////////////////////////////////////////////////////////////////////////////
extern UINT8 bDrvNRGetCurrentPath(void);

////////////////////////////////////////////////////////////////////////////////
// Parameters
////////////////////////////////////////////////////////////////////////////////
static DI_MAPPING_T _arDiDataModeList[] =
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

static NR_MAPPING_T _arNrDataModeList[] =
{
    { E_DI_NR_OFF,      "NR OFF" },
    { E_DI_NR_Y_ONLY,  "NR Y ONLY" },
    { E_DI_NR_YC_420,  "NR YC 420" },
    { E_DI_NR_YC_422,  "NR YC 422" },
    { E_DI_NR_RGB,     "NR RGB" },
    { E_DI_NR_UNKNOWN, "Unknown" },
};

const CHAR* szPscanStatus[] = 
{   "Pixel overlfow",
    "Pixel underlfow",
    "NR    underlfow",
    "Flag  overlfow",
    "Flag  underlfow",
};

UINT32 u4FlagStaCnt[] = {0, 0, 0, 0, 0};

#define DI_STATUS_CNT 0x20

#ifdef __MODEL_slt__
UINT8 u1Finish = 0;
UINT8 u1Done = 0;
UINT32 u4DICRC = 0;
UINT32 u4DISrcCRC = 0;
#endif

#ifdef CC_FLIP_MIRROR_SUPPORT
extern UINT8 u1FlipMirrorConfig;
#endif

////////////////////////////////////////////////////////////////////////////////
// Control part CLI functions
////////////////////////////////////////////////////////////////////////////////
/**
* @brief Set debug data mode
* @param u1VdpId VDP_1/VDP_2
* @param bMode E_DI_NOBOB/E_DI_YCBOB/E_DI_CBOB/E_DI_FLIP_MIRROR
*/
void vDrvDISetDebugDataMode(UINT8 u1VdpId, E_DI_DMODE bMode)
{
    _arMDDiPrm[u1VdpId].u1DebugDataMode = bMode;
}

/**
* @brief Get debug data mode
* @param u1VdpId VDP_1/VDP_2
* @return E_DI_NOBOB/E_DI_YCBOB/E_DI_CBOB/E_DI_FLIP_MIRROR
*/
//TODO: remove this function if nobody else uses this?
UINT8 u1DrvDIGetDebugDataMode(UINT8 u1VdpId)
{
    return _arMDDiPrm[u1VdpId].u1DebugDataMode;
}

/**
* @brief Enable/disable debug PSCAN display mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
*/
void vDrvDISetDebugPSCANDisplay(UINT8 u1VdpId, UINT8 u1OnOff)
{
    _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay = u1OnOff;
}

/**
* @brief Return debug PSCAN display mode status
* @param u1VdpId VDP_1/VDP_2
* @return SV_ON/SV_OFF
*/
//TODO: remove this function if nobody else uses this?
UINT8 u1DrvDIGetDebugPSCANDisplay(UINT8 u1VdpId)
{
    return _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay;
}

/**
* @brief PSCAN firmware on/off
* @param u1Item main/sub/quality
* @param u1OnOff SV_ON/SV_OFF
*/
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
            vIO32WriteFldAlign(ADAPTIVE_REG, u1OnOff, ADAPTIVE_MA);
            Printf("PSCAN Turn %s FW quality \n", u1OnOff ? "on" : "off");
            break;
        default:
            Printf("Unknown item %d ", u1Item);
            break;
    }
}

/**
* @brief Convert PSCAN data mode to string
* @param E_DI_DMODE PSCAN data mode
* @return string
*/
const CHAR* vDrvDataModeToString(E_DI_DMODE eMode)
{
    UINT32 u4Num, i;
    const CHAR* szName = "Unknown";

    u4Num = (sizeof (_arDiDataModeList) / sizeof (DI_MAPPING_T)) - 1;

    for (i = 0; i < u4Num; i++)
    {
    	if (_arDiDataModeList[i].eDataMode == eMode)
    	{
            szName = _arDiDataModeList[i].szName;
            break;
    	}
    }

    return szName;
}

/**
* @brief Convert NR data mode to string
* @param E_DI_DMODE NR data mode
* @return string
*/
const CHAR* vDrvNRDataModeToString(E_NR_DMODE eMode)
{
    UINT32 u4Num, i;
    const CHAR* szName = "Unknown";

    u4Num = (sizeof (_arNrDataModeList) / sizeof (NR_MAPPING_T)) - 1;

    for (i = 0; i < u4Num; i++)
    {
        if (_arNrDataModeList[i].eDataMode == eMode)
        {
            szName = _arNrDataModeList[i].szName;
            break;
        }
    }
    return szName;
}

/**
* @brief Get PSCAN information
* @param u1VdpId VDP_1/VDP_2
* @param u1DbgIdx debug index
*/
UINT8 vDrvDIGetInfo(UINT8 u1VdpId, UINT8 u1DbgIdx)
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

            Printf("BufAddr           %08x\n", _arMDDiPrm[u1VdpId].u4BufAddr);
            Printf("BufWidth          %d\n", _arMDDiPrm[u1VdpId].u2BufWidth);
            Printf("BufHeight         %d\n", _arMDDiPrm[u1VdpId].u2BufHeight);

            Printf("\nFBM Infromation\n");
            Printf("Addr              %08x\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Addr);
            Printf("Width             %d\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Width);
            Printf("Height            %d\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Height);
            Printf("Mode              %08x\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Mode);
            Printf("10bit             %d\n", _arMDDiPrm[u1VdpId].rFbmPool.fg10bit);
            Printf("422               %d\n\n", _arMDDiPrm[u1VdpId].rFbmPool.fg422);

            Printf("Pixel Overflow  Cnt               %d\n", u4FlagStaCnt[0]);
            Printf("Pixel Underflow Cnt               %d\n", u4FlagStaCnt[1]);
            Printf("NR    Underflow Cnt               %d\n", u4FlagStaCnt[2]);
            Printf("Flag  Overflow  Cnt               %d\n", u4FlagStaCnt[3]);
            Printf("Flag  Underflow Cnt               %d\n", u4FlagStaCnt[4]);
            break;
            
        case 1:
            Printf("\nNR %s Debug Information\n", u1VdpId ? "Sub" : "Main");
            Printf("HDE:  %d\n", MDDI_READ_FLD(u1VdpId, MDDI_DA_16, STA_NR_HDE));
            Printf("DATA_EN:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_DA_16, STA_NR_DATA_EN) ? "Enable" : "Disable");
            Printf("READ_EN:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_DA_16, STA_NR_READ_ENABLE) ?  "Enable" : "Disable");
            Printf("C_DELAY_SEL:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_DA_16, STA_NR_C_DELAY_SEL) ? "C Delay 1 Line" : "Y/C Sync");
            Printf("PROGRESSIVE:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_DA_16, STA_NR_PROGRESSIVE) ? "Progressive" : "Interlace");
            switch(MDDI_READ_FLD(u1VdpId, MDDI_DA_16, STA_NR_YUV_MODE))
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
            Printf("MAIN_PIP_SEL:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_DA_16, STA_NR_MAIN_PIP_SEL) ? "Sub" : "Main");
            Printf("BIT_SEL_Y:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_DA_16, STA_NR_BIT_SEL_RY) ? "8 bit" : "10 bit");
            Printf("BIT_SEL_U:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_DA_16, STA_NR_BIT_SEL_RU) ? "8 bit" : "10 bit");
            Printf("BIT_SEL_V:  %s\n", MDDI_READ_FLD(u1VdpId, MDDI_DA_16, STA_NR_BIT_SEL_RV) ? "8 bit" : "10 bit");
            break;
            
        default:
            _arMDDiPrm[u1VdpId].u1DebugIndex = u1DbgIdx;
            break;
    }

    return 0;
}

/**
* @brief Set PSCAN ink information
* @param u1Item ink type
* @param u1OnOff on/off
*/
UINT8 vDrvDISetOSD(UINT8 u1Item, UINT8 u1OnOff)
{
    switch (u1Item)
    {
    	case 0: // Probe
            vRegWriteFld(MDDI_CTRL_15, (u1OnOff ? 6 : 0), OSD_MODE);
            vRegWriteFldMulti(MDDI_CTRL_14,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 1: // Film
            vRegWriteFld(MDDI_CTRL_15, (u1OnOff ? 1 : 0), OSD_MODE);
            vRegWriteFldMulti(MDDI_CTRL_14,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 2: // CS
            vRegWriteFld(MDDI_CTRL_15, (u1OnOff ? 3 : 0), OSD_MODE);
            vRegWriteFldMulti(MDDI_CTRL_14,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 3: // MEMA
            vRegWriteFld(MDDI_CTRL_15, (u1OnOff ? 4 : 0), OSD_MODE);
            vRegWriteFldMulti(MDDI_CTRL_14,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 4: // KERNEL
            vRegWriteFld(MDDI_CTRL_15, (u1OnOff ? 5 : 0), OSD_MODE);
            vRegWriteFldMulti(MDDI_CTRL_14,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 5: // Input Border
            vRegWriteFld(MDDI_CTRL_01, u1OnOff, SRC_BOT_BORDER_EN);
            vRegWriteFld(MDDI_CTRL_01, u1OnOff, SRC_BOT_BORDER_EN);
            break;
    	case 6: // Output Border
            vRegWriteFld(MDDI_CTRL_13, u1OnOff, BORDER_EN);
            break;
    	default:
            break;
    }
    return 0;
}


/**
* @brief Bypass PSCAN HW if SV_TRUE
* @param u1VdpId VDP_1/VDP_2
* @param u1Bypass SV_TRUE/SV_FALSE
*/
void vDrvDIBypass(UINT8 u1VdpId, UINT8 u1Bypass)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, ((u1Bypass == SV_TRUE) ? 1 : 0), BYPASS_PSCAN);
}

void vDrvDISetDoubleBuffer(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, u1OnOff, READ_V_PROTECT);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, u1OnOff, WRITE_V_PROTECT);
}

/**
* @brief Reset PSCAN hardware state machine
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIReset(UINT8 u1VdpId, UINT8 bStart)
{
    if (bStart)
    {
        vDrvDISetDoubleBuffer(u1VdpId, SV_OFF);
        vDrvDISetDramCtrl(u1VdpId, SV_OFF);
        vDrvDISetDoubleBuffer(u1VdpId, SV_ON);

        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, 1, SEQ_RST_DRAM);
        x_thread_delay(1);
        vRegWriteFldMulti((MDDI_CTRL_00 + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), 
        P_Fld(1, SOFT_RST_PSOUT)|P_Fld(1, SOFT_RST_SRC)|
        P_Fld(1, SOFT_RST_DISP)|P_Fld(0, READ_V_PROTECT));
    }
    else
    {
        vRegWriteFldMulti((MDDI_CTRL_00 + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), 
        P_Fld(0, SOFT_RST_PSOUT)|P_Fld(0, SOFT_RST_SRC)|
        P_Fld(0, SOFT_RST_DISP)|P_Fld(1, READ_V_PROTECT));
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, 0, SEQ_RST_DRAM);
    }
}


/**
* @brief Get PSCAN buffer address
* @param u1VdpId VDP_1/VDP_2
* @param u4AddrStart start address
* @param u4AddrEnd end address
* @return pointer
*/
UINT8 vDrvDIGetBufAddr(UINT8 u1VdpId, UINT32 *u4AddrStart, UINT32 *u4AddrEnd)
{
    *u4AddrStart = (MDDI_READ_FLD(u1VdpId, MDDI_DA_08, RG_WADDR_LO_LIMIT)<<11);
    *u4AddrEnd = (MDDI_READ_FLD(u1VdpId, MDDI_DA_0C, RG_FLAG_WADDR_HI_LIMIT)<<11);
    return 0;
}

/**
* @brief Get PSCAN buffer address
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIOnOff(UINT8 u1VdpId, UINT8 bOnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, bOnOff, BYPASS_PSCAN);
    vRegWriteFldAlign(SCPIP_PIP1_06, 0x0, PIP1_06_WSTARTP_OF1ST_1);
}

/**
* @brief Set PSCAN Freeze
* @param u1VdpId VDP_1/VDP_2
* @param u1Field freeze field selection
*/
void vDrvDISetFreeze(UINT8 u1VdpId, UINT8 u1Field)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, u1Field, FREEZE_TOP_BOT_SEL);
}

/**
* @brief Set PSCAN Freeze Delay
* @param u1VdpId VDP_1/VDP_2
* @param u1MVW freeze delay for clear shape flag write
* @param u1Film freeze delay for film mode detection
* @param u1LTMW freeze delay for MEMA long term motion flag write
* @param u1MFFW freeze delay for MEMA multi-field flag write
* @param u1PXLW freeze delay for pixel write
* @param u1PXLR freeze delay for pixel read
*/
void vDrvDISetFreezeDelay(UINT8 u1VdpId, UINT8 u1MVW, UINT8 u1Film, UINT8 u1LTMW, 
                                                             UINT8 u1MFFW, UINT8 u1PXLW, UINT8 u1PXLR)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, u1MVW, FREEZE_SEL_MVW);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, u1Film, FREEZE_SEL_FILM);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, u1LTMW, FREEZE_SEL_LTMW);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, u1MFFW, FREEZE_SEL_MFFW);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, u1PXLW, FREEZE_SEL_PXLW);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, u1PXLR, FREEZE_SEL_PXLR);
}


/**
* @brief Set Kernel DE ratio
* @param u1VdpId VDP_1/VDP_2
* @param u1N numerator
* @param u1Mdenominator
*/
void vDrvDISetKDE(UINT8 u1VdpId, UINT16 u2N, UINT16 u2M)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_08, u2N, KDE_N);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_08, u2M, KDE_M);
}

/**
* @brief Set Source DE ratio
* @param u1VdpId VDP_1/VDP_2
* @param bOnOff enable auto source de ratio
* @param u1N numerator
* @param u1Mdenominator
*/
void vDrvDISetSDE(UINT8 u1VdpId, UINT8 bOnOff, UINT16 u2N, UINT16 u2M)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_09, bOnOff, AUTO_SDE_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_09, u2N, SDE_N);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_09, u2M, SDE_M);
}

/**
* @brief Set Output DE ratio
* @param u1VdpId VDP_1/VDP_2
* @param bOnOff enable auto source de ratio
* @param u1N numerator
* @param u1Mdenominator
*/
void vDrvDISetODE(UINT8 u1VdpId, UINT8 bOnOff, UINT16 u2N, UINT16 u2M)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0A, bOnOff, AUTO_ODE_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0A, u2N, ODE_N);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0A, u2M, ODE_M);
}

/**
* @brief Turn on/off SRCW_OUT_I
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
*/
void vDrvDISRCWOutIOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_10, u1OnOff, SRCW_OUT_I);
}

/**
* @brief Turn on/off progressive mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
*/
void vDrvDIProgModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_01, u1OnOff, FRAME420);
}


/**
* @brief Switch between 420/422 color mode
* @param u1VdpId VDP_1/VDP_2
* @param u1ColorMode MDDI_422_MODE/MDDI_420_MODE
*/
void vDrvDISetColorMode(UINT8 u1VdpId, UINT8 u1ColorMode)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_01, (u1ColorMode ? 0 : 1), M422);    
}


#ifdef MDDI_WA1
UINT16 vDrvDICalWAWidth(UINT16 u2Width)
{
    UINT16 ret, u2Mod, u2Div;
    
    u2Div = u2Width / 128;
    u2Mod = u2Width % 128;
    ret = u2Width;
    
    if ((u2Mod > 0) && (u2Mod <5))
    {
        ret = u2Width + 6;
    }
    else if ((u2Mod > 117) && (u2Mod < 127))
    {
        ret = 128 * (u2Div + 1);
    }
    return ret;
}

/**
* @brief workaround for ink error
* @param u2Width input width
* @param workaround width
*/
UINT16 vDrvDISetWAWidth(UINT16 u2Width)
{
    UINT16 ret;

    if (u2Width >= 262)
    {
        ret = vDrvDICalWAWidth(u2Width);
    }
    else if ((u2Width >= 252) && (u2Width <= 261))
    {
        ret = 262;
    }
    else if ((u2Width >= 247) && (u2Width <= 251))
    {
        ret = 246;
    }
    else
    {
        if ((u2Width % 4) == 0)
        {
            ret = u2Width + 1;
        }
        else if ((u2Width % 4) == 3)
        {
            ret = u2Width - 1;
        }
        else
        {
            ret = u2Width;
        }
    }
    return ret;
}
#endif


/**
* @brief Set field inverse
* @param u1VdpId VDP_1/VDP_2
* @param u1Inv 
*/
void vDrvDISetFieldInverse(UINT8 u1VdpId, UINT8 u1Inv)
{
    LOG(2, "DI[%d] Set Field Inverse %d\n", u1VdpId, u1Inv);

    #if defined(CC_MT5387) && !defined(CC_MT5363)
    if (BSP_GetIcVersion() <= IC_VER_5387_AA) // Before ECO
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, u1Inv, FLD_IN_P);
    }
    #endif
}

/**
* @brief Set pscan kernel clock
* @param bPath VDP_1/VDP_2
* @param eType clock type
*/
void vDrvDISetKernelClock(UINT8 u1VdpId, DI_KERNEL_CLOCK_TYPE eType)
{
#ifdef CC_MT5363
    vIO32WriteFldAlign(CKGEN_DISP_CKCFG1, eType, ((u1VdpId == VDP_1) ? FLD_PS_K_M_CK_SEL : FLD_PS_K_P_CK_SEL));        
#else // MT5387
    vIO32WriteFldAlign(CKGEN_VOPROCCFG, eType, ((u1VdpId == VDP_1) ? FLD_PS_DISP_CLK_SEL : FLD_PIP_PS_DISP_CLK_SEL));        
#endif
}

/**
* @brief Set pscan output clock
* @param bPath VDP_1/VDP_2
* @param eType clock type
*/
void vDrvDISetOutputClock(UINT8 u1VdpId, DI_OUTPUT_CLOCK_TYPE eType)
{
#ifdef CC_MT5363
    vIO32WriteFldAlign(CKGEN_DISP_CKCFG1, eType, ((u1VdpId == VDP_1) ? FLD_PS_OUT_CLK_SEL : FLD_PS_O_P_CK));        
#else // MT5387
    vIO32WriteFldAlign(CKGEN_VOPROCCFG, eType, ((u1VdpId == VDP_1) ? FLD_PS_OUT_CLK_SEL : FLD_PIP_PS_OUT_CLK_SEL));        
#endif
}

/**
* @brief Init pscan  clock
* @param bPath VDP_1/VDP_2
*/
void vDrvDIInitClock(UINT8 u1VdpId)
{
#ifdef CC_MT5363
    vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0x0, ((u1VdpId == VDP_1) ? FLD_PS_DFD_M_EN : FLD_PS_DFD_P_EN));        
    vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0x1, ((u1VdpId == VDP_1) ? FLD_PS_DFD_M_SEL : FLD_PS_DFD_P_SEL)); 
    vIO32WriteFldAlign(((u1VdpId == VDP_1) ? CKGEN_PSOUT_DIVCFG : CKGEN_PSOUT2_DIVCFG), 0x2, FLD_IDEAL_PLL_N);        
    vIO32WriteFldAlign(((u1VdpId == VDP_1) ? CKGEN_PSOUT_DIVCFG : CKGEN_PSOUT2_DIVCFG), 0x1, FLD_IDEAL_PLL_M);        
#else // MT5387
    vIO32WriteFldAlign(((u1VdpId == VDP_1) ? CKGEN_PS_DIG_CKCFG : CKGEN_PIP_PS_DIG_CKCFG), 0x1, FLD_PS_DIG_CK_SEL);        
    vIO32WriteFldAlign(((u1VdpId == VDP_1) ? CKGEN_PS_DIG_CKCFG : CKGEN_PIP_PS_DIG_CKCFG), 0x0, FLD_PS_DIG_CK_TST);        
    vIO32WriteFldAlign(((u1VdpId == VDP_1) ? CKGEN_PS_DIG_CKCFG : CKGEN_PIP_PS_DIG_CKCFG), 0x1, FLD_PS_DIG_CK_SRC);        
    vIO32WriteFldAlign(((u1VdpId == VDP_1) ? CKGEN_PS_DIG_DIVCFG : CKGEN_PIP_PS_DIG_DIVCFG), 0x2, FLD_PIP_PS_DIG_CK_IDEAL_PLL_N);        
    vIO32WriteFldAlign(((u1VdpId == VDP_1) ? CKGEN_PS_DIG_DIVCFG : CKGEN_PIP_PS_DIG_DIVCFG), 0x1, FLD_PIP_PS_DIG_CK_IDEAL_PLL_M);        
#endif
}


/**
* @brief Set pscan output clock div
* @param bPath VDP_1/VDP_2
* @param N numerator
* @param M denominator
*/
void vDrvDISetClkDiv(UINT8 u1VdpId, UINT16 u2M, UINT16 u2N)
{
#ifdef CC_MT5363
    vIO32WriteFldAlign(((u1VdpId == VDP_1) ? CKGEN_PSOUT_DIVCFG : CKGEN_PSOUT2_DIVCFG), u2N, FLD_IDEAL_PLL_N);        
    vIO32WriteFldAlign(((u1VdpId == VDP_1) ? CKGEN_PSOUT_DIVCFG : CKGEN_PSOUT2_DIVCFG), u2M, FLD_IDEAL_PLL_M);        
#else // MT5387
    vIO32WriteFldAlign(((u1VdpId == VDP_1) ? CKGEN_PS_DIG_DIVCFG : CKGEN_PIP_PS_DIG_DIVCFG), u2M, FLD_PIP_PS_DIG_CK_IDEAL_PLL_M);        
    vIO32WriteFldAlign(((u1VdpId == VDP_1) ? CKGEN_PS_DIG_DIVCFG : CKGEN_PIP_PS_DIG_DIVCFG), u2N, FLD_PIP_PS_DIG_CK_IDEAL_PLL_N);        
#endif
}

/**
* @brief Get pscan output clock
* @param u1VdpId VDP_1/VDP_2
* @return Clock rate
*/
UINT32 u4DrvDIGetClkRate(UINT8 u1VdpId)
{
    UINT8 u1ClkType;
    UINT32 u4ClkRate = 0xffff;

#ifdef CC_MT5363
    u1ClkType = IO32ReadFldAlign(CKGEN_DISP_CKCFG1, ((u1VdpId == VDP_1) ? FLD_PS_OUT_CLK_SEL : FLD_PS_O_P_CK));     
#else // MT5387
    u1ClkType = IO32ReadFldAlign(CKGEN_VOPROCCFG, ((u1VdpId == VDP_1) ? FLD_PS_OUT_CLK_SEL : FLD_PIP_PS_OUT_CLK_SEL));     
#endif

    switch(u1ClkType)
    {
        case 0: // vdoin_mp_ck
            u4ClkRate = ((RegReadFldAlign(OMUX_00, VDOIN_MPCLK_SEL) == 0) ? 16200 : 18000);
            break;
        case 2: // ps_dig_div_ck
            u4ClkRate = 16200;
            break;
        default: // oclk or clock off
            break;
    }
    return u4ClkRate;
}

/**
* @brief Set Read Precision Selection
* @param u1VdpId VDP_1/VDP_2
* @param b8bit 8bit/10bit
*/
void vDrvDIForceRead8bit(UINT8 u1VdpId, UINT8 b8bit)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, b8bit, RG_FORCE_8BIT_RY);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, b8bit, RG_FORCE_8BIT_RC);
}

/**
* @brief Set Write Precision Selection
* @param u1VdpId VDP_1/VDP_2
* @param b8bit 8bit/10bit
*/
void vDrvDISetWritePrecision(UINT8 u1VdpId, UINT8 b8bit)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, b8bit, RG_BIT_SEL_WY);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, b8bit, RG_BIT_SEL_WU);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, b8bit, RG_BIT_SEL_WV);
}

/**
* @brief Set PSCAN buffer address
* @param u1VdpId VDP_1/VDP_2
* @param u4AddrYMSB
* @param u4AddrYLSB
* @param u4AddrUMSB
* @param u4AddrULSB
* @param u4AddrVMSB
* @param u4AddrVLSB
* @param u4AddrFlag
*/
void vDrvDISetBufferAddr(UINT8 u1VdpId, UINT32 u4AddrYMsb, UINT32 u4AddrYLsb, 
        UINT32 u4AddrUMsb, UINT32 u4AddrULsb, UINT32 u4AddrVMsb, UINT32 u4AddrVLsb, UINT32 u4AddrFlg)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_01, u4AddrYMsb, RG_ADDR_BASE_MSB_Y);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_02, u4AddrYLsb, RG_ADDR_BASE_LSB_Y);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_03, u4AddrUMsb, RG_ADDR_BASE_MSB_U);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_04, u4AddrULsb, RG_ADDR_BASE_LSB_U);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_05, u4AddrVMsb, RG_ADDR_BASE_MSB_V);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_06, u4AddrVLsb, RG_ADDR_BASE_LSB_V);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_0B, u4AddrFlg, RG_FLAG_ADDR_BASE);
}

/**
* @brief Set PSCAN buffer protection
* @param u1VdpId VDP_1/VDP_2
* @param u4AddrYMsb
* @param u4AddrFlg
* @param u4AddrEnd
*/
void vDrvDISetBufferProtect(UINT8 u1VdpId, UINT32 u4AddrYMsb, UINT32 u4AddrFlg, UINT32 u4AddrEnd)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_08, u4AddrYMsb, RG_WADDR_LO_LIMIT);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_07, u4AddrFlg, RG_WADDR_HI_LIMIT);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_0D, u4AddrFlg, RG_FLAG_WADDR_LO_LIMIT);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_0C, u4AddrEnd, RG_FLAG_WADDR_HI_LIMIT);
}

/**
* @brief Set SRCW_V_BGN, KERNEL_H_BGN, KERNEL_V_BGN
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDISetBgn(UINT8 u1VdpId, UINT8 u1S_V_Bgn, UINT8 u1K_H_Bgn, UINT8 u1K_V_Bgn)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0F, u1S_V_Bgn, SRCW_V_BGN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0F, u1K_V_Bgn, KERNEL_V_BGN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0F, u1K_H_Bgn, KERNEL_H_BGN);
}

/**
* @brief Log FIFO overflow/underflow
* @param u1VdpId VDP_1/VDP_2
* @param u1Sta 
*/
void vDrvDILogStatus(UINT8 u1VdpId, UINT8 u1Sta)
{
    LOG(6, "DI[%d] %s\n", u1VdpId, szPscanStatus[u1Sta]);
    u4FlagStaCnt[u1Sta]++;
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, 1, RG_STATUS_CLR);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, 0, RG_STATUS_CLR);
}

/**
* @brief Check FIFO overflow/underflow
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIChkStatus(UINT8 u1VdpId)
{   
    if (_arMDDiPrm[u1VdpId].u1StatusCnt == DI_STATUS_CNT)
    {
        _arMDDiPrm[u1VdpId].u1StatusCnt = 0;    
  
        if (MDDI_READ_FLD(u1VdpId, MDDI_DA_13, STA_OVERFLOW))
        {
            vDrvDILogStatus(u1VdpId, 0);
        }
        
        if (MDDI_READ_FLD(u1VdpId, MDDI_DA_13, STA_UNDERFLOW))
        {
            vDrvDILogStatus(u1VdpId, 1);
        }
        
        if (u1VdpId == bDrvNRGetCurrentPath())
        {
            if (MDDI_READ_FLD(u1VdpId, MDDI_DA_13, STA_NR_UNDERFLOW))
            {
                vDrvDILogStatus(u1VdpId, 2);
            }    
        }
        
        if (u1VdpId == VDP_1)
        {        
            if (MDDI_READ_FLD(u1VdpId, MDDI_DA_13, STA_FLAG_OVERFLOW))
            {
                vDrvDILogStatus(u1VdpId, 3);
            }  
            
            if (MDDI_READ_FLD(u1VdpId, MDDI_DA_13, STA_FLAG_UNDERFLOW))
            {
                vDrvDILogStatus(u1VdpId, 4);
            }
        }
    }   
}


/**
* @brief Get H_blank information
* @param bPath VDP_1/VDP_2
* @return H_blank
*/
UINT32 dwDrvDIGetHBlank(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MDDI_CTRL_0E, H_BLANK);
}


/**
* @brief Set Output Pattern Gen
* @param bPath VDP_1/VDP_2
* @param bOnOff On/Off
*/
void vDrvDISetPattern(UINT8 u1VdpId, UINT8 u1OnOff)
{
    UINT8 u1Val = ((u1OnOff == 1) ? 0x7 : 0x0);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_12, u1Val, PAT_GEN_MODE_Y);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_12, u1Val, PAT_GEN_MODE_CB);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_12, u1Val, PAT_GEN_MODE_CR);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_13, u1OnOff, BORDER_EN);
}


/**
* @brief Set Input Border
* @param bPath VDP_1/VDP_2
* @param bTopBot Top/Bot
*/
void vDrvDISetInputBorder(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_01, u1OnOff, SRC_TOP_BORDER_EN);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_01, u1OnOff, SRC_BOT_BORDER_EN);
}


/**
* @brief Configure Flip Mirror Mode
* @param bPath VDP_1/VDP_2
* @param bMode
*/
void vDrvDIConfigFlipMirror(void)
{
#ifdef CC_FLIP_MIRROR_SUPPORT
	UINT8 u1Config;
	u1Config= u1GetFlipMirrorConfig(); 
	
        /* move check to u1GetFlipMirrorConfig
	if(IS_IC_5387() || (IS_IC_5363() && (BSP_GetIcVersion() < IC_VER_5363_AB)))
	{
		// 5387, skip mirror
		// 5363 before ECO, skip mirror
		u1Config &= ~(SYS_MIRROR_CONFIG_ON);
	}
        */
	if((u1Config & SYS_FLIP_CONFIG_ON) && 
	(u1Config& SYS_MIRROR_CONFIG_ON))
	{
		u1FlipMirrorConfig = DI_FLIP_MIRROR_ON_MODE;
	}
	else if(u1Config & SYS_FLIP_CONFIG_ON)
	{
		u1FlipMirrorConfig = DI_FLIP_ON_MODE;
	}
	else if(u1Config & SYS_MIRROR_CONFIG_ON)
	{
		u1FlipMirrorConfig = DI_MIRROR_ON_MODE;
	}
	else
	{
		u1FlipMirrorConfig = DI_NORMAL_MODE;
	}
#endif
}

/**
* @brief Set Flip Mirror Mode
* @param bPath VDP_1/VDP_2
* @param bMode
*/
void vDrvDISetFlipMirror(UINT8 u1VdpId, UINT8 u1Mode)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0F, (u1Mode == 0) ? 
            0 : MDDI_READ_FLD(u1VdpId, MDDI_CTRL_0F, SRCW_V_BGN) + 1, FRC_LINE);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, u1Mode, RG_DISPLAY_MODE);
    vRegWriteFldAlign(NR_3DNR00, u1Mode, nr_display_mode);

#if PSCAN_DISPMODE_MIRROR_WA
    if ((u1Mode == DI_MIRROR_ON_MODE) || (u1Mode == DI_FLIP_MIRROR_ON_MODE))
    {
        bScpipPScanDispMirrorWA(u1VdpId, SV_TRUE);
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_1F, SV_TRUE, CROMA_DELAY);
    }
    else
    {
        bScpipPScanDispMirrorWA(u1VdpId, SV_FALSE);
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_1F, SV_FALSE, CROMA_DELAY);
    }
#endif
}


#ifdef __MODEL_slt__
/**
* @brief Set PSCAN CRC test
*/
void vDrvDICRCTest(void)
{
    if (RegReadFldAlign(MDDI_CTRL_17, CRC_ON) != u1Finish)
    {
        if (RegReadFldAlign(MDDI_CTRL_17, CRC_ON) == 1) // from 0 ro 1
        {
            u1Done = 0;
            LOG(2, "[CRC TEST] Do CRC: %d\n", RegReadFldAlign(MDDI_CTRL_17, CRC_ON));
        }
        else // from 1 ro 0
        {
            u4DICRC = RegReadFldAlign(MDDI_STA_07, MDDI_CRC);
            u4DISrcCRC = RegReadFldAlign(MDDI_STA_08, SRCW_CRC);
            u1Done = 1;
            LOG(2, "[CRC TEST] Finish CRC: %d\n", RegReadFldAlign(MDDI_CTRL_17, CRC_ON));
            LOG(2, "[CRC TEST] u4DICRC (%08X)\n", u4DICRC);
        }
        u1Finish = RegReadFldAlign(MDDI_CTRL_17, CRC_ON);
    }
}

/**
* @brief Set PSCAN CRC clear
* @bOnOff clear on/off
*/
void vDrvDISetCRCClr(UINT8 bOnOff)
{
    vRegWriteFldAlign(MDDI_CTRL_17, bOnOff, CRC_CLEAR);
}

/**
* @brief Set PSCAN CRC trigger
* @bOnOff clear on/off
*/
void vDrvDISetCRCTrigger(UINT8 bOnOff)
{
    vRegWriteFldAlign(MDDI_CTRL_17, bOnOff, CRC_TRIGGER);
}

/**
* @brief Get PSCAN CRC output pixel result
* @return output pixel result
*/
UINT32 vDrvDIGetCRCResult(void)
{
    if (u1Done)
    {
        return u4DICRC;
    }
    else
    {
        return 0; // CRC not finished
    }
}

/**
* @brief Get PSCAN Source CRC output pixel result
* @return output pixel result
*/
UINT32 vDrvDIGetSrcCRC(void)
{
    return u4DISrcCRC;
}

/**
* @brief CRC pre-processing
*/
void vDrvDICRCPreProcess(void)
{
    // Do Reset
    u1Done = 0;
    u1Finish = 0;
    u4DICRC = 0;
    u4DISrcCRC = 0;
    vDrvDIReset(VDP_1, SV_ON);
    vDrvDIReset(VDP_1, SV_OFF);

    // Delay B2R send field to PSCAN
    #if defined(CC_MT5387) && !defined(CC_MT5363)
    if (BSP_GetIcVersion() >= IC_VER_5387_AB) // After ECO
    {
        vRegWriteFldAlign(0x6020, 0x0, Fld(4, 28, AC_MSKB3));
    }
    else
    #endif
    {
        #ifdef CC_MT5363
        if (BSP_GetIcVersion() >= IC_VER_5363_AB) // After ECO
        {
            vRegWriteFldAlign(0x6020, 0x0, Fld(4, 28, AC_MSKB3));
        }
        else
        #endif
        {
            vRegWriteFldAlign(0x6020, 0xf, Fld(4, 28, AC_MSKB3));
        }
    }
    
    // Bypass NR
    vRegWriteFldAlign(NR_TOP_MAIN_00, 0x0, c_main_nr_en);
    vRegWriteFldAlign(NR_TOP_PIP_00, 0X0, C_PIP_NR_EN);
    
    // Bypass DI Quailiy
    vRegWriteFldAlign(MDDI_CTRL_00, 0x0, EN_QLYRW);
    vRegWriteFldAlign(MDDI_MEMA_00, 0x0, MEMA_EN);
    vRegWriteFldAlign(MDDI_SB_00, 0x0, SMART_BLENDING_EN);
    vRegWriteFldAlign(MDDI_DAR_00, 0x0, DAR_EN);
    vRegWriteFldAlign(MDDI_CS_00, 0x0, CS_EN);
    vRegWriteFldAlign(MDDI_CTRL_01, 0x0, SD_5_LINE);
    vRegWriteFldAlign(MDDI_FILM_27, 0x0, R_22ENABLE);
    vRegWriteFldAlign(MDDI_FILM_27, 0x0, R_32ENABLE);
}
#endif
