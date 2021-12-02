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

#include "hw_di.h"

#include "drv_di.h"
#include "drv_di_int.h"
#include "drv_nr.h"
#include "drv_scaler.h"
#include "drv_scpos.h"
#include "di_debug.h"

#include "eeprom_if.h"
#include "hw_ckgen.h"
#include "hw_ycproc.h"
#include "hw_scpos.h"
#include "hw_sys.h"
#include "vdo_misc.h"
#include "drv_display.h"

#include "drv_scaler_drvif.h"
#include "fbm_drvif.h"
#include "srm_drvif.h"

EXTERN void vDrvDISetAutoStop(UINT8 u1VdpId, UINT8 u1OnOff);

/**
* @brief Set DI Debug Mode
* @param u1VdpId VDP_1/VDP_2
* @param bMode DI Mode
*/
void vDrvDISetDebugDataMode(UINT8 u1VdpId, E_MIB_DMODE bMode)
{
    _arMDDiPrm[u1VdpId].eDebugDataMode = bMode;
    
    if (bMode == E_MIB_DMODE_UNKNOWN) // reset data mode to follow srm setting
    {
        _arMDDiPrm[u1VdpId].u1DebugFrcMode = E_MIB_FRC_UNKNOWN;
    }
    else
    {
        _arMDDiPrm[u1VdpId].u1DebugFrcMode = (E_MIB_FRC_DMODE)((UINT8)bMode&0xF);
    }
}

/**
* @brief Disable display mode
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDISetDispmodeOff(UINT8 u1VdpId)
{
    LOG(0,"MT5396 / 68 New Flow Not Support Direct Setting FRC mode , Please Use Data Mode");
}

/**
* @brief Set DI Debug Display Mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff On/Off
*/
void vDrvDISetDebugPSCANDisplay(UINT8 u1VdpId, UINT8 u1OnOff)
{
    LOG(0,"MT5396 / 68 New Flow Not Support Direct Setting FRC mode , Please Use Data Mode");
    return;
}

/**
* @brief Get DI Debug Display Mode
* @param u1VdpId VDP_1/VDP_2
* @return DI Debug Display Mode
*/
UINT8 u1DrvDIGetDebugPSCANDisplay(UINT8 u1VdpId)
{
    return _arMDDiPrm[u1VdpId].u1DebugFrcMode;
}

/**
* @brief DI FW On/Off
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff On/Off
*/
void vDrvDIFWOnOff(UINT8 u1Item, UINT8 u1OnOff)
{
    switch (u1Item)
    {
        case 0:
            vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, u1OnOff, ADAPTIVE_PSCAN_MAIN);
            Printf("PSCAN main Turn %s FW control \n", u1OnOff? "on" : "off");
            break;
        case 1:
            vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, u1OnOff, ADAPTIVE_PSCAN_SUB);
            Printf("PSCAN sub Turn %s FW control \n", u1OnOff ? "on": "off");
            break;
        case 2:
            vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, u1OnOff, ADAPTIVE_PSCAN_QTY);
            Printf("PSCAN Turn %s FW quality \n", u1OnOff ? "on" : "off");
            break;
        default:
            Printf("Unknown item %d ", u1Item);
            break;
    }
}

/**
* @brief Translate data mode to string for log
* @param eMode DI Data Mode
*/
const CHAR* szDrvDataModeToString(UINT8 u1VdpId)
{
    if (u1VdpId >= VDP_NS)
    {
        u1VdpId = VDP_2;
    }    
    return aDataMode[_arMDDiPrm[u1VdpId].u1DataModeIdx].szModeName;
}

/**
* @brief Translate data mode to string for log
* @param eMode DI Data Mode
*/
const CHAR* szDrvBWDataModeToString(UINT8 u1VdpId)
{
    if (u1VdpId >= VDP_NS)
    {
        u1VdpId = VDP_2;
    }
    return aDataMode[_arMDDiPrm[u1VdpId].u1BandwidthDataModeIdx].szModeName;
}

/**
* @brief Get DI Status info
* @param u1VdpId VDP_1/VDP_2
* @param u1DbgIdx debug level
*/
void vDrvDIGetInfo(UINT8 u1VdpId, UINT8 u1DbgIdx)
{
    switch (u1DbgIdx)
    {
        case 0:
            Printf("\nPSCAN %s Debug Information\n", u1VdpId ? "Sub" : "Main");
            Printf("Sorce             %s\n",  _arMDDiPrm[u1VdpId].u1Interlace ? "Interlace" : "Progressive");
            Printf("Data mode         %s\n", szDrvDataModeToString(u1VdpId));  
            Printf("BandWidth Mode    %s\n", szDrvBWDataModeToString(u1VdpId));  
            Printf("Bit Res           %s\n", (_arMDDiPrm[u1VdpId].u1BitRes == 10) ? "10 bit" : "8 bit");
            Printf("Color Mode        %s\n", (_arMDDiPrm[u1VdpId].u1420Mode) ? "420" : "422");
            Printf("DecType           %d\n\n", _arMDDiPrm[u1VdpId].u1DecType);
            Printf("HTotal            %d\n", _arMDDiPrm[u1VdpId].u2HTotal);
            Printf("VTotal            %d\n", _arMDDiPrm[u1VdpId].u2VTotal);
            Printf("Width             %d\n", _arMDDiPrm[u1VdpId].u2Width);
            Printf("Height            %d\n\n", _arMDDiPrm[u1VdpId].u2Height);
            Printf("HClip             %d\n", _arMDDiPrm[u1VdpId].u2HClip);
            Printf("VClip             %d\n", _arMDDiPrm[u1VdpId].u2VClip);
            Printf("\nFBM Infromation\n");
            Printf("Addr Start        %08x\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Addr);
            Printf("Addr End          %08x\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Addr+_arMDDiPrm[u1VdpId].rFbmPool.u4Size);
            Printf("Width             %d\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Width);
            Printf("Height            %d\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Height);
            Printf("Mode              %08x\n", _arMDDiPrm[u1VdpId].rFbmPool.u4Mode);
            Printf("10bit             %d\n", _arMDDiPrm[u1VdpId].rFbmPool.fg10bit);
            Printf("422               %d\n\n", _arMDDiPrm[u1VdpId].rFbmPool.fg422);
            Printf("3D Input          %d\n\n", _arMDDiPrm[u1VdpId].eTDInput);
            Printf("3D Outout         %d\n\n", _arMDDiPrm[u1VdpId].eTDOutput);
            break;
                        
        case 1:
            Printf("Pixel Overflow  Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_PXL_OVERFLOW]);
            Printf("Pixel Underflow Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_PXL_UNDERFLOW]);
            Printf("Flag  Overflow  Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_FLG_OVERFLOW]);
            Printf("Flag  Underflow Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_FLG_UNDERFLOW]);
            Printf("SC    Underflow Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_SC_UNDERFLOW]);
            Printf("NR    Overflow Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_NR_OVERFLOW]);
            break;
        
        default:
            break;
    }
}

/**
* @brief Set DI OSD
* @param u1Item OSD type
* @param u1OnOff On/Off
*/
void vDrvDISetOSD(UINT8 u1Item, UINT8 u1OnOff)
{
    switch (u1Item)
    {
    	case 0: // Probe
            vRegWriteFld(MCVP_KC_21, (u1OnOff ? 6 : 0), OSD_MODE);
            vRegWriteFldMulti(MCVP_KC_21,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 1: // Film
            vRegWriteFld(MCVP_KC_21, (u1OnOff ? 1 : 0), OSD_MODE);
            vRegWriteFldMulti(MCVP_KC_21,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 2: // CS
            vRegWriteFld(MCVP_KC_21, (u1OnOff ? 3 : 0), OSD_MODE);
            vRegWriteFldMulti(MCVP_KC_21,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 3: // MEMA
            vRegWriteFld(MCVP_KC_21, (u1OnOff ? 4 : 0), OSD_MODE);
            vRegWriteFldMulti(MCVP_KC_21,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 4: // KERNEL
            vRegWriteFld(MCVP_KC_21, (u1OnOff ? 5 : 0), OSD_MODE);
            vRegWriteFldMulti(MCVP_KC_21,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 5: // Input Border
            vRegWriteFld(MCVP_KC_04, u1OnOff, IPTG_BORDER_EN);
            break;
    	case 6: // Output Border
            vRegWriteFld(MCVP_KC_1E, u1OnOff, BORDER_EN);
            vRegWriteFld(MCVP_BC_06, u1OnOff, FINAL_BORDER_EN);
            break;
    	default:
            break;
    }
}

/**
* @brief Get DI hblank
* @param u1VdpId VDP_1/VDP_2
* @return Hblank
*/
UINT32 u4DrvDIGetHBlank(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MCVP_KC_2B, I2P_H_BLANK);
}

/**
* @brief Get DI main width
* @return DI main width
*/
UINT16 u2DrvDIGetWidth(void)
{
    return MDDI_READ_FLD(SV_VP_MAIN, MCVP_KC_0A, HDEW);
}


/**
 * @brief Return the minimum horizontal width that PSCAN can support
 * @param u1VdpId VDP_1/VDP_2
 * @return u4MiniInputWidth The minimum horizontal width that PSCAN can support
 */
UINT32 u4DrvDIMiniInputWidth(UINT8 u1VdpId)
{
    return 180;
}

/**
 * @brief Return the maximum horizontal width that PSCAN can support
 * @param u1VdpId VDP_1/VDP_2
 * @return u4MaxInputWidth The maximum horizontal width that PSCAN can support
 */
UINT32 u4DrvDIMaxInputWidth(UINT8 u1VdpId)
{
    UINT32 u4MaxInputWidth;
    SRM_TV_MODE_T eMode = SRM_GetTvMode();

    if (eMode == SRM_TV_MODE_TYPE_POP)
    {
        u4MaxInputWidth = 960;
    }
    else if ((eMode == SRM_TV_MODE_TYPE_PIP) && (u1VdpId == VDP_2))
    {
    	u4MaxInputWidth = 720;
    }
    else
    {
    	u4MaxInputWidth = 1920;
    }

    return u4MaxInputWidth;
}

/**
* @brief Get PSCAN delay
* @param u1VdpId VDP_1/VDP_2
* @return PSCAN delay value
*/
UINT8 u1DrvDIGetFrameDelay(UINT8 u1VdpId)
{    
     return (
         MDDI_READ_FLD(u1VdpId,MCVP_KC_17,Fld(1, 0, AC_MSKB0))+
         MDDI_READ_FLD(u1VdpId,MCVP_KC_17,Fld(1, 2, AC_MSKB0))+
         MDDI_READ_FLD(u1VdpId,MCVP_KC_17,Fld(1, 4, AC_MSKB0)))*10;
}

/**
* @brief Get progressive display mode clip
* @param u1VdpId VDP_1/VDP_2
* @return progressive display mode clip
*/
UINT32 u4DrvDIGetProgDispmodeClip(UINT8 u1VdpId)
{
#if 0//MIB_PORTING_TODO
//This function is outdated and should be removed
#endif
    return 0;
}

/**
* @brief Get input/output sync distance
* @param u1VdpId VDP_1/VDP_2
*/
UINT16 u2DrvDIGetVcnt(UINT8 u1VdpId)
{
    return _arMDDiPrm[u1VdpId].u2Vcnt;
}

/**
 * @brief Set NR Data mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1DataMode datamode
 */
void vDrvDISetNRDebugDataMode(UINT8 u1VdpId, E_MIB_NR_DMODE u1DataMode)
{
    if ((u1VdpId != VDP_1) && (u1VdpId != VDP_2))
    {
        _arMDDiPrm[VDP_1].u1DebugNRDataMode = E_MIB_NR_UNKNOWN;        
        _arMDDiPrm[VDP_2].u1DebugNRDataMode = E_MIB_NR_UNKNOWN;                
    }
    else
    {    
        _arMDDiPrm[u1VdpId].u1DebugNRDataMode = u1DataMode;
    }
}

/**
 * @brief Set demo mode active region
 * @param u1OnOff SV_ON/SV_OFF
 * @param u1Region SV_LEFT/SV_RIGHT
 * @param u2Center Center line position
 */
void vDrvDIDemo(UINT8 u1OnOff, UINT8 u1Region, UINT16 u2Center)
{
    // Not support in MT5365
}

/**
 * @brief Set total number of lines to be clipped for ONLY DTV case
 * @param u1VdpId VDP_1/VDP_2
 * @param u2VClip Total number of lines to be clipped
 */
void vDrvDISetDtvClip(UINT8 u1VdpId, UINT16 u2DTVClip)
{
    //outdated FW Walkaround
}

/**
 * @brief Turn on/off trick mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 * @warning This should be invoked by TVD only
 */
void vDrvDITrickModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_17, (u1OnOff ? 1 : 0), AUTO_TRICK_ON);

    //Trick mode is an one-field-only mode. PSCAN has to work in 2D mode under this case
    vDrvDISetInterpMode(u1VdpId, (u1OnOff ? E(I,NR_OFF,DI_BOB ,FRC_SCL) : _arMDDiPrm[u1VdpId].eDataMode));
}

/**
 * @brief DI Frame Repeat
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 * @warning This should be invoked by B2R only
 */
void vDrvDIFrameRepeat(UINT8 u1VdpId, UINT8 u1OnOff)
{   
    if (u1VdpId >= VDP_NS)
    {
        return;
    }

    if(fgMainMuteOn())
	{
		u1OnOff=SV_OFF;
	}
    
    if(_arMDDiPrm[u1VdpId].u1SRCWAutoStopDisableCnt > 0)
    {
        _arMDDiPrm[u1VdpId].u1SRCWAutoStopDisableCnt-- ; //For H/V position adjustment after DTV pause(timeshift) when pscan disp mode.Temporery.
    }
    else
    {
        vDrvDISetAutoStop(u1VdpId, u1OnOff);
    }
}

/**
 * @brief Enable/disable PSCAN freeze mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 */
void vDrvDIFreezeOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
    UINT8 u1Disable = 0xFF;
    if(!IS_ECO_IC())
    {   //Before ECO , Should manually Stop MV LM Write
        if(u1OnOff)
        {
            MDDI_WRITE_FLD(u1VdpId,MCVP_MULTI_00,SV_OFF,EN_MVW);
            MDDI_WRITE_FLD(u1VdpId,MCVP_MULTI_00,SV_OFF,EN_LMW);        
        }
        else
        {
            MDDI_WRITE_FLD(u1VdpId,MCVP_MULTI_00,
                MDDI_READ_FLD(u1VdpId,MCVP_MULTI_00,EN_MVR)      
                ,EN_MVW);
            MDDI_WRITE_FLD(u1VdpId,MCVP_MULTI_00,
                MDDI_READ_FLD(u1VdpId,MCVP_MULTI_00,EN_LMR)
                ,EN_LMW);
        }
    }
    
    if(IS_PROGRESSIVE(u1VdpId))
    {
        if (bIsScalerInput444(u1VdpId))
        {
            u1Disable = ~E_SRC_W;
            MDDI_WRITE_FLD(u1VdpId, MCVP_MULTI_00, !u1OnOff, EN_SRCW); 
        }
        else
        {
            u1Disable = ~E_NR_W;
            MDDI_WRITE_FLD(u1VdpId, MCVP_MULTI_00, !u1OnOff, EN_NRW); 
        }
        _arMDDiPrm[u1VdpId].u1FreezeWriteFilter = (u1OnOff) ? u1Disable : 0xFF;
    }
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_0A, u1OnOff, FREEZE_ON);

    vDrvDIUserFilmModeFreeze(u1VdpId, !u1OnOff); //interlace


}

/**
 * @brief Check if freeze function works under the current configuration
 * @param u1VdpId VDP_1/VDP_2
 * @return SV_TRUE/SV_FALSE
 */
UINT8 u1DrvDIFreezable(UINT8 u1VdpId)
{
    return IS_MIB_FRC(u1VdpId) ? SV_TRUE : SV_FALSE;
}

/**
 * @brief DI info for capture logo
 * @param u1VdpId VDP_1/VDP_2
 * @param pDispInfo DI info
 */
void vDrvDIDispInfoForCapLog(UINT8 bPath, DI_DISP_INFO_T* pDispInfo)
{
	UINT32 u4YStartMSBAddr,u4YStartLSBAddr,u4UStartMSBAddr,u4UStartLSBAddr,u4VStartMSBAddr,u4VStartLSBAddr; 
	UINT8  u1BitRes;
	UINT16 u2Width, u2Height;
	UINT32 AF_Index;
	UINT8  AF_Bottom;
			
	if (bPath >= VDP_NS)
	{
		return;
	}
		
	u1BitRes = _arMDDiPrm[bPath].u1BitRes;
	u2Width = _arMDDiPrm[bPath].u2Width;
	u2Height = IS_INTERLACE(bPath) ? (_arMDDiPrm[bPath].u2Height/2) : _arMDDiPrm[bPath].u2Height;
	u2Height = IS_420TO422(bPath) ? (((u2Height+1)>>1)<<1) : u2Height;
		
	pDispInfo->u1BlockMode = 1;
	pDispInfo->u2Width = u2Width;
	pDispInfo->u2Height = u2Height;
	pDispInfo->u1BitRes = u1BitRes;
	pDispInfo->u1ColorMode = _arMDDiPrm[bPath].u1420Mode; //420 or 422
	pDispInfo->u2DramPitch = MDDI_READ_FLD(bPath, MCVP_DRAM_07, DA_DRAM_LINE_LENGTH)<< 4;
		
	u4YStartMSBAddr = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_ADDR_BASE_MSB_Y);
	u4YStartLSBAddr = MDDI_READ_FLD(bPath, MCVP_DRAM_09, DA_ADDR_BASE_LSB_Y);
	u4UStartMSBAddr = MDDI_READ_FLD(bPath, MCVP_DRAM_0A, DA_ADDR_BASE_MSB_U);
	u4UStartLSBAddr = MDDI_READ_FLD(bPath, MCVP_DRAM_0B, DA_ADDR_BASE_LSB_U);
	u4VStartMSBAddr = MDDI_READ_FLD(bPath, MCVP_DRAM_0C, DA_ADDR_BASE_MSB_V);
	u4VStartLSBAddr = MDDI_READ_FLD(bPath, MCVP_DRAM_0D, DA_ADDR_BASE_LSB_V);
		
	pDispInfo->u4YStartMSBAddr = u4YStartMSBAddr<<11;
	pDispInfo->u4YStartLSBAddr = u4YStartLSBAddr<<11;
	pDispInfo->u4UStartMSBAddr = u4UStartMSBAddr<<11;
	pDispInfo->u4UStartLSBAddr = u4UStartLSBAddr<<11;
	pDispInfo->u4VStartMSBAddr = u4VStartMSBAddr<<11;
	pDispInfo->u4VStartLSBAddr = u4VStartLSBAddr<<11;
				
	MDDI_WRITE_FLD(bPath, MCVP_DRAM_10, 1, DA_DEBUG); //A Field Index.
	AF_Index = MDDI_READ_FLD(bPath, MCVP_DRAM_11, STA_DEBUG);
	pDispInfo->u1AFYUVIndex = AF_Index & 0xFFF;
		
	AF_Bottom = MDDI_READ_FLD(bPath, MCVP_KC_19, CF_BOTTOM); // A=C  0:Top 1:Bottom
	pDispInfo->u1AFBottom = AF_Bottom;
		
	pDispInfo->u1YFrameNum = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_FRAME_NUM_Y) + 1;
	pDispInfo->u1CFrameNUm = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_FRAME_NUM_U) + 1;
		
	pDispInfo->u1CfgFlipMirror	= 0; //FlipMirror=Normal
	pDispInfo->u1DramFlipMirror = 0; //DramFlipMirror=Normal consequence.
			
	if(bIsScalerInput444(bPath))
	{
		pDispInfo->u1ColorMode = 2;//RGB444
		pDispInfo->u1OutMode = 2; //RGB444
				
	#ifdef CC_FLIP_MIRROR_SUPPORT
		if(u1FlipMirrorConfig != DI_NORMAL_MODE)
		{
			pDispInfo->u1CfgFlipMirror	= u1FlipMirrorConfig;
			pDispInfo->u1DramFlipMirror = 0; //SC Read Flip/Mirror
		}
	#endif		
	}
	else if(IS_INTERLACE(bPath)) //Interlace
	{
		pDispInfo->u1OutMode = 0; //Interlace based field
				
	#ifdef CC_FLIP_MIRROR_SUPPORT
		if(u1FlipMirrorConfig != DI_NORMAL_MODE)
		{
			pDispInfo->u1CfgFlipMirror	= u1FlipMirrorConfig;
			pDispInfo->u1DramFlipMirror = 1; //PSCAN Write Flip/Mirror
		}
	#endif
	}
	else //Progressive
	{
		pDispInfo->u1OutMode = 1; //Progressive based frame
				
	#ifdef CC_FLIP_MIRROR_SUPPORT
		if(u1FlipMirrorConfig != DI_NORMAL_MODE)
		{
			pDispInfo->u1CfgFlipMirror	= u1FlipMirrorConfig;
			if ((IS_PROGRESSIVE(bPath)) && IS_420TO422(bPath))
			{
				pDispInfo->u1DramFlipMirror = 1; //PSCAN Write Flip/Mirror
			}
			else
			{
				pDispInfo->u1DramFlipMirror = 0; //SC Read Flip/Mirror
			}
		}
	#endif		
	}
}

/**
* @brief  to tell whether overflow or underflow happen
* @param u1VdpId VDP_1/VDP_2
* @return 0 if no underflow or overflow happens else  total overflow/underflow number
*/
UINT32 u4DrvDIChkAllStatus(UINT8 u1VdpId)
{
    UINT32 u4Result = 0;

    u4Result += u4FlagStaCnt[u1VdpId][E_STA_PXL_OVERFLOW];
    u4Result += u4FlagStaCnt[u1VdpId][E_STA_PXL_UNDERFLOW];
    u4Result += u4FlagStaCnt[u1VdpId][E_STA_SC_UNDERFLOW];
    u4Result += u4FlagStaCnt[u1VdpId][E_STA_NR_OVERFLOW];
    u4Result += u4FlagStaCnt[u1VdpId][E_STA_FLG_OVERFLOW];
    u4Result += u4FlagStaCnt[u1VdpId][E_STA_FLG_UNDERFLOW];

    return u4Result;
}


/**
* @brief  Reset the overflow & underflow count
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIRstStatusCnt(UINT8 u1VdpId)
{
    u4FlagStaCnt[u1VdpId][E_STA_PXL_OVERFLOW] = 0;
    u4FlagStaCnt[u1VdpId][E_STA_PXL_UNDERFLOW] = 0;
    u4FlagStaCnt[u1VdpId][E_STA_NR_OVERFLOW] = 0;
    u4FlagStaCnt[u1VdpId][E_STA_SC_UNDERFLOW] = 0;
    u4FlagStaCnt[u1VdpId][E_STA_FLG_OVERFLOW] = 0;
    u4FlagStaCnt[u1VdpId][E_STA_FLG_UNDERFLOW] = 0;
}


/**
 * @brief Clear DI status
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff clear On/Off
 */
void vDrvDIClearStatus(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, u1OnOff, DA_OVERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, u1OnOff, DA_UNDERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, u1OnOff, DA_SC_UNDERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_01, u1OnOff, DA_FLAG_OVERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_01, u1OnOff, DA_FLAG_UNDERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, u1OnOff, DA_NR_OVERFLOW_CLR);
}


/**
* @brief Get DI vdp width
* @return DI vdp width
*/
UINT16 u2DrvDIGetVdpWidth(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MCVP_KC_0A, HDEW);
}

/**
* @brief Get DI vdp height
* @return DI vdp height
*/
UINT16 u2DrvDIGetVdpHeight(UINT8 u1VdpId)
{
    UINT32 u4Height = MDDI_READ_FLD(u1VdpId, MCVP_KC_0A, VDEW);

    if((VDP_2==u1VdpId)&&IS_PROGRESSIVE(u1VdpId))
    {
        u4Height = u4Height >> 1;
    }
    
    return u4Height;
}

UINT32 fgDrvDIIsMIBDramAvaliable(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId,MCVP_MULTI_00,EN_DRAM_RW)!=0;
}

/**
* @brief Get Debug Data Mode
* @param u1VdpId VDP_1/VDP_2
*/
UINT16  u2DrvDIGetDebugDataMode(UINT8 u1VdpId)
{
    return _arMDDiPrm[u1VdpId].eDataMode;
}

void vDrvDiSetDataMode(UINT8 u1VdpId, UINT8 index)
{
    if (u1VdpId >= VDP_NS)
    {
        u1VdpId = 0;
    }
    vDrvDISetDebugDataMode(u1VdpId, (aDataMode[index].eMIBMode));

    if(aDataMode[index].eMIBMode != E_MIB_DMODE_UNKNOWN)
    {
        VDP_SetDispmode(u1VdpId, ((((UINT32)aDataMode[index].eMIBMode)&0xF) == E_MIB_FRC_MIB) ? 3 : 5 );
    }
    else
    {
        if (_arMDDiPrm[u1VdpId].rFbmPool.u4Mode & FBM_POOL_MODE_MDDI_DISP)
        {
            VDP_SetDispmode(u1VdpId, 3);//FRC_MIB
        }
        else
        {
            VDP_SetDispmode(u1VdpId, 5);// FRC_SCL  96
        }
    }
}

UINT8 u1DrvDiIs10Bit(UINT8 u1VdpId)
{
    return !MDDI_READ_FLD(u1VdpId, MCVP_DRAM_08, DA_BIT_SEL_WY);
}

UINT8 u1DrvDiIs422Mode(UINT8 u1VdpId)
{
   	return !MDDI_READ_FLD(u1VdpId, MCVP_KC_1E, M444);
}

UINT8 u1DrvDiIsTopField(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MCVP_KC_0D, IN_FLD);
}

UINT32 u4DrvDIIsAutoStopped(UINT8 u1VdpId)
{
    PATH_CHECK(u1VdpId);
    return MDDI_READ_FLD(VDP_1, MCVP_KC_19, FRC_REPEAT_FIELD)&&IS_INTERLACE(u1VdpId)&&(!IS_FRAME_SEQ(u1VdpId));
}

/**
 * @brief Get DI output clock rate
 * @param u1VdpId VDP_1/VDP_2
 * @return u4ClkRate clock rate
 */

UINT32 u4DrvDIGetClkRate(UINT8 u1VdpId)
{
    UINT8 u1ClkType;
    u1ClkType = IO32ReadFldAlign(CKGEN_MIB_OCLK_CKCFG, FLD_MIB_OCLK_CK_SEL);    
    
    switch(u1ClkType)
    {        
        case MIB_CLK_TYPESYSPLL_D2_CK:  
            return MIB_SYSTEM_PLL/2;
        case MIB_CLK_TYPEENETPLL_D2_CK:
            return MIB_ETHERNET_PLL/2;
        case MIB_CLK_TYPEXTAL_CK:            
        case MIB_CLK_RSV:                   
        default:
            return MIB_XTAL_PLL;
    }
}


UINT8 u1DrvDIIsSupportedSignal(UINT8 u1VdpId)
{
    UINT32 u4InputClk = wDrvVideoGetVTotal(u1VdpId) * wDrvVideoGetHTotal(u1VdpId) * bDrvVideoGetRefreshRate(u1VdpId);        

    if (u4InputClk >= (u4DrvDIGetClkRate(u1VdpId))) 
    {
        return SV_OFF;
    }
    else
    {
        return SV_ON;
    }
}

/**
 * @brief Enable pscan to force 22 film mode for DTV/MMP
 * @param fgOnOff  Enable forcing 22 film mode
 * @param u1Mode 0->top first  1->bottom first
 * @return  0->OK  1->can't force film mode
 */
UINT8 u1vDrvDIMPEGForce22FilmMode(UINT8 fgOnOff, UINT8 u1Mode)
{
    return 0;
}

void vDrvDIStopCtrlFw(UINT8 u1VdpId,UINT8 fgOnOff)
{
    if (u1VdpId == VDP_2)
    {        
        vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, fgOnOff, ADAPTIVE_PSCAN_SUB); 
    }
    else
    {
        vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, fgOnOff, ADAPTIVE_PSCAN_MAIN); 
    }
}

