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
 * $RCSfile: drv_scaler_ptgen.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler.h
 *  Brief of file drv_scaler_ptgen.c.
 *  Details of file drv_scaler_ptgen.c (optional).
 */

#include "hw_tdc.h"
#include "hw_ycproc.h"

#include "drv_video.h"
#include "drv_scaler.h"
#include "drv_scaler_drvif.h"
#include "frametrack_drvif.h"
#include "drv_scpos.h"

#include "vdo_misc.h"
#include "general.h"
#include "video_def.h"
#include "mute_if.h"
#include "scpos_debug.h"

#include "x_assert.h"

#if SUPPORT_DOT_TO_DOT_PATTERN
UINT32 bOri10BitMode; 
UINT32 bOriUsH; 
UINT32 bOriUsV; 
UINT32 bCount=0;
UINT16 bOriDataLen;
UINT16  bOriHL;
UINT16  bOriVL;
UINT16 bOriRHP;
UINT16 bOriRHL;
UINT16 bOriRVP;
UINT16 bOriRVL;
UINT8 bOriEN3D; 
UINT8 bOriDSIs444Mode; 
UINT8 bOriVUSisBypass; 
UINT8 bOriFSEL_1;
UINT8 bOriVOFST_BYPASS;
UINT8 bOriALLBP;
UINT8 bOriYUV2RGB;
UINT16 wOriHBoundary;
UINT32 dwPreBaseAddress; 
UINT8 bOriDramBurstMode;

extern void vWriteDramB(UINT32 u4Addr, UINT8 bData);

UINT8 G0PixelValue;   //the pixel0 R G B
UINT8 B0PixelValue;
UINT8 R0PixelValue;

UINT8 G1PixelValue;   //the pixel1 R G B
UINT8 B1PixelValue;
UINT8 R1PixelValue;

UINT8 BeginRevLine;//begin reverse the Revline's head of a line's pixel. 
UINT8 RevLine = 0;    
UINT8 SpecialRevLine = 0;
#endif

/**
 * @6896 porting done 
 */
void vScpipSetDSPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4HTotal, UINT32 u4Width, UINT32 u4VTotal, UINT32 u4Height)
{
    UINT8 bPath;
    UINT8 patOffset;
    RPicInfo* picInfo;

    if (u4VdpId >= VDP_NS) 
    { 
        return;
    }
   
    bPath = getScalerPath(u4VdpId);
    patOffset = getDSPatOffset(bPath);
    picInfo = getPicInfo(bPath);

    
    if (u4OnOff == VDP_DS_PTN_AUTO) // auto mode
    {
        UINT32 u4SrcWidth, u4SrcHeight, u4SrcXOffset, u4SrcYOffset;
        UINT32 u4YUV;
        //get width, height, overscan
        u4SrcWidth = picInfo->wTargetW;
        u4SrcHeight = picInfo->wTargetH;
        u4SrcXOffset = getMixedHPorch(u4VdpId);
        u4SrcYOffset = getMixedVPorch(u4VdpId);
        //cal width, overscan after H-predown
        u4SrcWidth = DIVIDE_WITH_CARRIER(u4SrcWidth * u4ScpipGetHorizontalPreScaleDownFactor(u4VdpId),  DFT_HPD_FACTOR);
        u4SrcXOffset = (u4SrcXOffset * u4ScpipGetHorizontalPreScaleDownFactor(u4VdpId)) / DFT_HPD_FACTOR;

        LOG(3, "VDP(%d), SRC after HPS (%d, %d, %d, %d)\n",  u4VdpId, u4SrcXOffset, u4SrcYOffset, u4SrcWidth, u4SrcHeight);

        u4HTotal = wDrvVideoGetHTotal(bPath);
        u4VTotal = wDrvVideoGetVTotal(bPath);
            
        LOG(3, "VDP(%d), HTotal/VTotal (%d, %d)\n",  u4VdpId, u4HTotal, u4VTotal);

        u4Width =  u4SrcWidth + u4SrcXOffset;
        u4Height =  u4SrcHeight + u4SrcYOffset;

        u4YUV = (bIsScalerInput444(bPath)? 0: 1);
        
        vRegWriteFldAlign(SCPIP_IPTGEN1_01+patOffset, u4Width, IPTGEN1_01_IPT1_H_ACTIVE);
        vRegWriteFldAlign(SCPIP_IPTGEN1_01+patOffset, u4HTotal, IPTGEN1_01_IPT1_H_TOTAL);
        vRegWriteFldAlign(SCPIP_IPTGEN1_02+patOffset, u4Height, IPTGEN1_02_IPT1_V_ACTIVE);
        vRegWriteFldAlign(SCPIP_IPTGEN1_02+patOffset, u4VTotal, IPTGEN1_02_IPT1_V_TOTAL);
        
        vRegWriteFldAlign(SCPIP_IPTGEN1_00+patOffset, u4YUV, IPTGEN1_00_IPT1_COLOR);
        // disable frame track
        u4ForceFrameTrackCtrl |= SCPIP_FRAME_TRACK_OFF_DS_PATTERN;
        // turn on pattern
        vRegWriteFldAlign(SCPIP_IPTGEN1_00+patOffset, 1, IPTGEN1_00_IPT1_EN);
    }
    else if (u4OnOff == VDP_DS_PTN_MANUAL)  //manual mode
    {
        vRegWriteFldAlign(SCPIP_IPTGEN1_01+patOffset, u4Width, IPTGEN1_01_IPT1_H_ACTIVE);
        vRegWriteFldAlign(SCPIP_IPTGEN1_01+patOffset, u4HTotal, IPTGEN1_01_IPT1_H_TOTAL);
        vRegWriteFldAlign(SCPIP_IPTGEN1_02+patOffset, u4Height, IPTGEN1_02_IPT1_V_ACTIVE);
        vRegWriteFldAlign(SCPIP_IPTGEN1_02+patOffset, u4VTotal, IPTGEN1_02_IPT1_V_TOTAL);
        // disable frame track
        u4ForceFrameTrackCtrl |= SCPIP_FRAME_TRACK_OFF_DS_PATTERN;
        // turn on pattern
        vRegWriteFldAlign(SCPIP_IPTGEN1_00+patOffset, 1, IPTGEN1_00_IPT1_EN);
    }
    else // off
    {
        vRegWriteFldAlign(SCPIP_IPTGEN1_00+patOffset, 0, IPTGEN1_00_IPT1_EN);
        u4ForceFrameTrackCtrl &= (~SCPIP_FRAME_TRACK_OFF_DS_PATTERN);    
    }
}

/**
 * @6896 porting done 
 */
void vScpipSetUSPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4Width, UINT32 u4Height)
{
    UINT8 bPath;    
    UINT8 usPatOffset;
    
    if (u4VdpId >= VDP_NS) 
    { 
        return;
    }

    bPath = getScalerPath(u4VdpId);
    usPatOffset = getUSPatOffset(bPath);
    
    if ((u4OnOff == VDP_US_PTN_AUTO) || (u4OnOff == VDP_US_PTN_MANUAL))
    {
        UINT32 u4PreLen; 
        if (u4OnOff == VDP_US_PTN_AUTO)        
        {
            //width after down
			u4Width = _arScalePrm[u4VdpId].u4DownScaledWidth;
            //height after down
			u4Height = _arScalePrm[u4VdpId].u4DownScaledHeight;
            LOG(3, "VDP(%d), Src after DS(%d, %d)\n", u4VdpId,u4Width, u4Height);
        }
                                
        //In mt5368/96, v start should always set to 1
        u4PreLen = 1;
        //set pattern size 
        vRegWriteFldAlign(SCPIP_SYSTEM_15+usPatOffset, u4Width, SYSTEM_15_MAIN_PAT_HTOTAL);
        vRegWriteFldAlign(SCPIP_SYSTEM_15+usPatOffset, u4Height, SYSTEM_15_MAIN_PAT_VTOTAL);
        vRegWriteFldAlign(SCPIP_SYSTEM_15+usPatOffset, u4PreLen, SYSTEM_15_MAIN_PAT_V_START);

        // turn on pattern
        if (u4VdpId == VDP_1)   // for main
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_14, 1, SYSTEM_14_MAIN_PAT_EN);
        }
        else                    // for sub 
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_14, 1, SYSTEM_14_SUB_PAT_EN);
        }
    }
    else // off
    {
        if (u4VdpId == VDP_1)   // for main
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_14, 0, SYSTEM_14_MAIN_PAT_EN);
        }
        else                    // for sub 
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_14, 0, SYSTEM_14_SUB_PAT_EN);
        }
    }
}

#ifdef CC_MT5396
void vScpipSetPostScalerPattern(UINT32 u4InOut, UINT32 u4OnOff, UINT32 u4HTotal, UINT32 u4Width, UINT32 u4VTotal, UINT32 u4Height)
{
	if (u4InOut == 0) // post scaler in
	{
        if (u4OnOff == SCPIP_PTGEN_OFF)
        {
            vIO32WriteFldAlign(PSSYSTEM_12 , SV_OFF, PSSYSTEM_12_PTGEN_OUT_ENABLE);
            return;
        }

        if (u4OnOff == SCPIP_PTGEN_AUTO)
        {
            u4Width  = wDISPLAY_WIDTH;
            u4Height = wDISPLAY_HEIGHT;
            u4HTotal = HLEN_TOTAL_50HZ;
            u4VTotal = VLEN_TOTAL_50HZ;
        }

        vIO32WriteFldAlign(PSSYSTEM_11, u4Width,  PSSYSTEM_11_PTGEN_ACTIVE_WIDTH);
        vIO32WriteFldAlign(PSSYSTEM_11, u4Height, PSSYSTEM_11_PTGEN_ACTIVE_HEIGHT);
        vIO32WriteFldAlign(PSSYSTEM_13, u4HTotal, PSSYSTEM_13_PTGEN_OUT_HTOTAL);
        vIO32WriteFldAlign(PSSYSTEM_13, u4VTotal, PSSYSTEM_13_PTGEN_OUT_VTOTAL);

        vIO32WriteFldAlign(PSSYSTEM_11, 7, PSSYSTEM_11_PTGEN_MODE_Y);
        vIO32WriteFldAlign(PSSYSTEM_11, 7, PSSYSTEM_11_PTGEN_MODE_CB);
        vIO32WriteFldAlign(PSSYSTEM_11, 7, PSSYSTEM_11_PTGEN_MODE_CR);

        vIO32WriteFldAlign(PSSYSTEM_14, 1, PSSYSTEM_14_PTGEN_BORDER_EN);
        vIO32WriteFldAlign(PSSYSTEM_14, 1, PSSYSTEM_14_PTGEN_PRB_X_POS);
        vIO32WriteFldAlign(PSSYSTEM_14, 1, PSSYSTEM_14_PTGEN_PRB_Y_POS);
        
        vIO32WriteFldAlign(PSSYSTEM_12, SV_ON, PSSYSTEM_12_PTGEN_OUT_TG_MODE);
	    vIO32WriteFldAlign(PSSYSTEM_12 , SV_ON, PSSYSTEM_12_PTGEN_OUT_ENABLE);
	}
	else // post scaler out
	{
        if (u4OnOff == SCPIP_PTGEN_OFF)
        {
            vIO32WriteFldAlign(PSSYSTEM_16 , SV_OFF, PSSYSTEM_16_MAIN_PAT_EN);
            return;
        }

        if (u4OnOff == SCPIP_PTGEN_AUTO)
        {
            u4Width  = wDISPLAY_WIDTH;
            u4Height = wDISPLAY_HEIGHT;
        }

        vIO32WriteFldAlign(PSSYSTEM_17, u4Width, PSSYSTEM_17_MAIN_PAT_HTOTAL);
        vIO32WriteFldAlign(PSSYSTEM_17, u4Height, PSSYSTEM_17_MAIN_PAT_VTOTAL);
        vIO32WriteFldAlign(PSSYSTEM_17, SV_ON, PSSYSTEM_17_MAIN_PAT_V_START);
        vIO32WriteFldAlign(PSSYSTEM_17, SV_OFF, PSSYSTEM_17_MAIN_PAT_YUV_EN);

	    vIO32WriteFldAlign(PSSYSTEM_16 , SV_ON, PSSYSTEM_16_MAIN_PAT_EN);
	}
}
#endif

#if SUPPORT_DOT_TO_DOT_PATTERN
/*
 *  @review this
 */
static void VDP_GetPattern(UINT8 bIndex)
{
	    switch(bIndex)
        {
            case 0: //black-white dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0xff;
                B1PixelValue = 0xff;
                R1PixelValue = 0xff;
                BeginRevLine = 1;
                break;
            case 1: //black-green dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0xff;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 1;
                break;

            case 2: //black-blue dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x00;
                B1PixelValue = 0xff;
                R1PixelValue = 0x00;
                BeginRevLine = 1;
                break;
            case 3: //black-red dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x00;
                B1PixelValue = 0x00;
                R1PixelValue = 0xff;
                BeginRevLine = 1;
                break;
            case 4: //black-green dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x7f;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 2;
                break;
            case 5: //black-bule dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x00;
                B1PixelValue = 0x7f;
                R1PixelValue = 0x00;
                BeginRevLine = 2;
                break;
            case 6: //black-red dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x00;
                B1PixelValue = 0x00;
                R1PixelValue = 0x7f;
                BeginRevLine = 2;
                break;
            case 7: //special black-green dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x7f;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 2;
                SpecialRevLine = 1;
                break;
            case 8: //special black-blue dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x00;
                B1PixelValue = 0x7f;
                R1PixelValue = 0x00;
                BeginRevLine = 2;
                SpecialRevLine = 1;
                break;
            case 9: //special black-red dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x00;
                B1PixelValue = 0x00;
                R1PixelValue = 0x7f;
                BeginRevLine = 2;
                SpecialRevLine = 1;
                break;
            default:
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x7f;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 2;
                SpecialRevLine = 1;
                break;
        }
}

/**
 *  @review this
 */
void vScpipDotToDotPattern(UINT8 ucOnOff,UINT32 bIndex)
{	     
#if 0
    static UINT8 _uDisableFunction=0;
    UINT8 GPixelValue;
    UINT8 BPixelValue;
    UINT8 RPixelValue;
    UINT16 uPattern_Width;
    UINT16 uPattern_Height;
    UINT8 RevLine = 0;    

    if((_uDisableFunction ==0) && (ucOnOff==0))
        return;
    _uDisableFunction = ucOnOff;

    if(ucOnOff == 1)
    {
        UINT32 TempAddress;
        UINT32 BaseAddress;
        UINT16 bDate_Length;
        UINT16 VerticalCounter=0;
        UINT16 HorizontalConuter=0;

        INT8 bTemp = 15;
        //UINT8 bPackCount=0;
        uPattern_Width =PANEL_GetPanelWidth();
        uPattern_Height=PANEL_GetPanelHeight()+1;		
        //_u4PreMuteMask  =  _u4MainMuteMask;
        //_u4MainMuteMask = 0;
        bDate_Length=(PANEL_GetPanelWidth()*3%16)?(PANEL_GetPanelWidth()*3/16+ 1):(PANEL_GetPanelWidth()*3/16+0);
        _rMChannel.bIsFreeze=1;
        bCount++;
        //_vDrvVideoSetMute(MUTE_MODULE_SCPOS_PATTERN, VDP_1, FOREVER_MUTE, TRUE);
        vScpipSetDramWrite(VDP_1, SV_OFF);;
        vScpipSetDramRead(VDP_1,SV_ON);
        vRegWriteFldAlign(OUTSTG_OS_17, 1, OS_17_MUTE_MAIN);

        BaseAddress= (RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1))<<4;

        if(bCount==1)
        {
            bOriEN3D = RegReadFldAlign(COMB_3D_01, CB_3D_EN); 
            bOriDSIs444Mode=RegReadFldAlign(SCPIP_DS1_02, DS1_02_SEL444_1); 
            bOri10BitMode = RegReadFldAlign(SCPIP_DS1_02, DS1_02_VD10BSEL_1);
            bOriUsH= RegReadFldAlign(SCPIP_US1_00, US1_00_USCALE_H_1);
            bOriUsV= RegReadFldAlign(SCPIP_US1_00, US1_00_USCALE_V_1);
            bOriVUSisBypass= RegReadFldAlign(SCPIP_US1_01, US1_01_VUS_BYPASS_1);
            bOriDataLen= RegReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1);
            bOriHL  = RegReadFldAlign(SCPIP_PIP1_01, PIP1_01_HL_1);
            bOriVL= RegReadFldAlign(SCPIP_PIP1_00, PIP1_00_VL_1);
            bOriRHP  = RegReadFldAlign(SCPIP_PIP1_03, PIP1_03_RHP_1);
            bOriRHL  = RegReadFldAlign(SCPIP_PIP1_03, PIP1_03_RHL_1);
            bOriRVP  = RegReadFldAlign(SCPIP_PIP1_02, PIP1_02_RVP_1);	
            bOriRVL  = RegReadFldAlign(SCPIP_PIP1_02, PIP1_02_RVL_1);
            bOriFSEL_1 = RegReadFldAlign(SCPIP_PIP1_09, PIP1_09_FSEL_1);
            bOriVOFST_BYPASS = RegReadFldAlign(SCPIP_PIP_OA_03, PIP_OA_03_US_VOFST_BYPASS1);
            bOriALLBP = RegReadFldAlign(SCE_CFG_MAIN, ALLBP);
            bOriYUV2RGB = RegReadFldAlign(YCBCR2RGB_00, YUV2RGB_BYPASS_MAIN);	
            wOriHBoundary = RegReadFldAlign(SCPIP_PIP1_0F,PIP1_0F_BOUNDARY_H_POS_1);
            dwPreBaseAddress = (RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1));
            bOriDramBurstMode = 0; //(RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_DRAM_BURST_MODE_1));
        }
        vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_FSEL_1);
        vScpipSetScalerDispmode(SV_VP_MAIN,SV_OFF);
        vScpipSetPSCANDispmode(SV_VP_MAIN,SV_OFF);
        vRegWriteFldAlign(COMB_3D_01, SV_OFF, CB_3D_EN);
        vRegWriteFldAlign(SCPIP_DS1_02, 1, DS1_02_SEL444_1);
        vRegWriteFldAlign(SCPIP_DS1_02, 0, DS1_02_VD10BSEL_1);
        vRegWriteFldAlign(SCPIP_US1_00, 0x8000, US1_00_USCALE_H_1);
        vRegWriteFldAlign(SCPIP_US1_00, 0x8000, US1_00_USCALE_V_1); 
        vRegWriteFldAlign(SCPIP_US1_01, 1, US1_01_VUS_BYPASS_1);
        vRegWriteFldAlign(SCPIP_PIP1_08, bDate_Length, PIP1_08_DATA_LENGTH_1);
        vRegWriteFldAlign(SCPIP_PIP1_01, bDate_Length, PIP1_01_HL_1);
        vRegWriteFldAlign(SCPIP_PIP1_00, uPattern_Height, PIP1_00_VL_1);
        vRegWriteFldAlign(SCPIP_PIP1_03, 0, PIP1_03_RHP_1);
        vRegWriteFldAlign(SCPIP_PIP1_03, uPattern_Width, PIP1_03_RHL_1);
        vRegWriteFldAlign(SCPIP_PIP1_02, 0, PIP1_02_RVP_1);
        vRegWriteFldAlign(SCPIP_PIP1_02, uPattern_Height, PIP1_02_RVL_1);
        vRegWriteFldAlign(SCE_CFG_MAIN, 1, ALLBP);// bypass SCE
        vRegWriteFldAlign(YCBCR2RGB_00, 1, YUV2RGB_BYPASS_MAIN);
        vRegWriteFldAlign(SCPIP_PIP_OA_03, 0, PIP_OA_03_US_VOFST_BYPASS1);
        vRegWriteFldAlign(SCPIP_PIP1_0F, uPattern_Width, PIP1_0F_BOUNDARY_H_POS_1);
        if(BaseAddress < 0x1000)
        {
            BaseAddress = (0x229c00)<<4; 
            vRegWriteFldAlign(SCPIP_PIP1_05, 0x229c00,PIP1_05_WSTARTP_1);
            vRegWriteFldAlign(SCPIP_PIP1_06, 0x229c00,PIP1_06_RSTARTP_1);
        }

        //_vDrvVideoSetMute(MUTE_MODULE_SCPOS_PATTERN, VDP_1,10, FALSE); 
        VDP_GetPattern(bIndex);
        GPixelValue = G0PixelValue;
        BPixelValue = B0PixelValue;
        RPixelValue = R0PixelValue;
        for(VerticalCounter = 0; VerticalCounter < uPattern_Height*2; VerticalCounter ++)//////Write Dram Data 
        {
            for(HorizontalConuter = 0; HorizontalConuter < uPattern_Width; HorizontalConuter ++)
            {
                switch(bTemp)
                {
                    case 0:   //left 1 pixel
                        TempAddress = BaseAddress;
                        vWriteDramB(TempAddress ,  GPixelValue);
                        TempAddress = BaseAddress + 31;
                        vWriteDramB(TempAddress ,  BPixelValue);
                        TempAddress = BaseAddress + 30;
                        vWriteDramB(TempAddress ,  RPixelValue);
                        bTemp = 13;
                        BaseAddress = BaseAddress + 16;

                        break;
                    case 1:  //left 2 pixels
                        TempAddress = BaseAddress;
                        vWriteDramB(TempAddress ,  BPixelValue);
                        TempAddress = BaseAddress + 1;
                        vWriteDramB(TempAddress , GPixelValue);
                        TempAddress = BaseAddress + 31;
                        vWriteDramB(TempAddress ,  RPixelValue);
                        bTemp = 14;
                        BaseAddress = BaseAddress + 16;

                        break;
                    default:	
                        if(-1 == bTemp)  //no left	
                        {
                            bTemp = 15;
                            BaseAddress = BaseAddress+16;
                        }
                        vWriteDramB((BaseAddress+bTemp) , GPixelValue);
                        bTemp--;
                        vWriteDramB((BaseAddress+bTemp), BPixelValue);
                        bTemp--;
                        vWriteDramB((BaseAddress+bTemp) , RPixelValue);
                        bTemp--;
                        break;					
                }
                GPixelValue = (GPixelValue == G0PixelValue) ? G1PixelValue : G0PixelValue;
                BPixelValue = (BPixelValue == B0PixelValue) ? B1PixelValue : B0PixelValue;
                RPixelValue = (RPixelValue == R0PixelValue) ? R1PixelValue : R0PixelValue;

            }

            RevLine++;
            if(SpecialRevLine == 1)
            {
                RevLine++;
                SpecialRevLine = 0;
            }			
            if(RevLine == BeginRevLine)
            {
                GPixelValue = (GPixelValue == G0PixelValue) ? G1PixelValue : G0PixelValue;
                BPixelValue = (BPixelValue == B0PixelValue) ? B1PixelValue : B0PixelValue;
                RPixelValue = (RPixelValue == R0PixelValue) ? R1PixelValue : R0PixelValue;
                RevLine = 0;
            }

            bTemp = 15;
            TempAddress = BaseAddress + 16;
            BaseAddress = TempAddress;
            //TempAddress = BaseAddress + 16*360;  //VerticalCounter * 16 * HL
        }
        vRegWriteFldAlign(OUTSTG_OS_17, 0, OS_17_MUTE_MAIN);
        _vDrvVideoForceUnMute(SV_VP_MAIN,1);
        //_rVideoMuteInfo[SV_VP_MAIN][MUTE_MODULE_SCPOS_PATTERN].bInvalid = FALSE;
        //_vDrvVideoSetMute(MUTE_MODULE_SCPOS_PATTERN, VDP_1,10, FALSE);  
    }	 
    else
    { 	  
        //if(_uDisableFunction==1)
        { 
            bCount=0;
            //_uDisableFunction=0;	
            _rMChannel.bIsFreeze=0;
            _vDrvVideoForceUnMute(SV_VP_MAIN,0);
            _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PATTERN, VDP_1, 10, TRUE);

            vScpipSetImportOnOff(SV_VP_MAIN,SV_OFF);
            vRegWriteFldAlign(COMB_3D_01, bOriEN3D, CB_3D_EN);
            vRegWriteFldAlign(SCPIP_DS1_02, bOriDSIs444Mode, DS1_02_SEL444_1);
            vRegWriteFldAlign(SCPIP_DS1_02, bOri10BitMode, DS1_02_VD10BSEL_1);
            vRegWriteFldAlign(SCPIP_US1_00, bOriUsH, US1_00_USCALE_H_1);
            vRegWriteFldAlign(SCPIP_US1_00, bOriUsV, US1_00_USCALE_V_1);
            vRegWriteFldAlign(SCPIP_US1_01, bOriVUSisBypass, US1_01_VUS_BYPASS_1);
            vRegWriteFldAlign(SCPIP_PIP1_08, bOriDataLen, PIP1_08_DATA_LENGTH_1);
            vRegWriteFldAlign(SCPIP_PIP1_01, bOriHL, PIP1_01_HL_1);
            vRegWriteFldAlign(SCPIP_PIP1_00, bOriVL, PIP1_00_VL_1);
            vRegWriteFldAlign(SCPIP_PIP1_03, bOriRHP, PIP1_03_RHP_1);
            vRegWriteFldAlign(SCPIP_PIP1_03, bOriRHL, PIP1_03_RHL_1);
            vRegWriteFldAlign(SCPIP_PIP1_02, bOriRVP, PIP1_02_RVP_1);
            vRegWriteFldAlign(SCPIP_PIP1_02, bOriRVL, PIP1_02_RVL_1);
            vRegWriteFldAlign(SCPIP_PIP1_09, bOriFSEL_1, PIP1_09_FSEL_1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03, bOriVOFST_BYPASS, PIP_OA_03_US_VOFST_BYPASS1);
            vRegWriteFldAlign(SCE_CFG_MAIN, bOriALLBP, ALLBP);
            vRegWriteFldAlign(YCBCR2RGB_00, bOriYUV2RGB, YUV2RGB_BYPASS_MAIN);
            vRegWriteFldAlign(SCPIP_PIP1_0F, wOriHBoundary,PIP1_0F_BOUNDARY_H_POS_1);
            vRegWriteFldAlign(SCPIP_PIP1_05, dwPreBaseAddress,PIP1_05_WSTARTP_1);
            vRegWriteFldAlign(SCPIP_PIP1_05, dwPreBaseAddress,PIP1_06_RSTARTP_1);
            vScpipSetImportOnOff(SV_VP_MAIN,SV_OFF);
        }
    }
#endif   
}
#endif

