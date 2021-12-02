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
//UINT16 _uDisableFunction=0;
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


void vScpipSetDSPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4HTotal, UINT32 u4Width, UINT32 u4VTotal, UINT32 u4Height)
{
    UINT8 bPath;
    UINT8 dsOffset;

    if (u4VdpId >= VDP_NS) 
    { 
        return;
    }
   
    bPath = getScalerPath(u4VdpId);
    dsOffset = getDSOffset(bPath);

    if (u4OnOff == VDP_DS_PTN_AUTO)
    {
        UINT32 u4SrcWidth, u4SrcHeight, u4SrcXOffset, u4SrcYOffset;
        UINT32 u4YUV;
        
        u4SrcWidth = _pPicInfo[u4VdpId]->wTargetW;
        u4SrcHeight = _pPicInfo[u4VdpId]->wTargetH;
        u4SrcXOffset = getMixedHPorch(u4VdpId);
        u4SrcYOffset = getMixedVPorch(u4VdpId);

        u4SrcWidth = DIVIDE_WITH_CARRIER(u4SrcWidth * u4ScpipGetHorizontalPreScaleDownFactor(u4VdpId),  DFT_HPD_FACTOR);
        u4SrcXOffset = (u4SrcXOffset * u4ScpipGetHorizontalPreScaleDownFactor(u4VdpId)) / DFT_HPD_FACTOR;

        LOG(3, "VDP(%d), SRC after HPS (%d, %d, %d, %d)\n",  u4VdpId, u4SrcXOffset, u4SrcYOffset, u4SrcWidth, u4SrcHeight);

        u4HTotal = wDrvVideoGetHTotal(bPath);
        u4VTotal = wDrvVideoGetVTotal(bPath);
            
        LOG(3, "VDP(%d), HTotal/VTotal (%d, %d)\n",  u4VdpId, u4HTotal, u4VTotal);

        u4Width =  u4SrcWidth + u4SrcXOffset;
        u4Height =  u4SrcHeight + u4SrcYOffset;

        u4YUV = (u1ScpipGet444Mode(bPath)? 0: 1);
        
        vRegWriteFldAlign(SCPIP_DS1_03+dsOffset, u4Width, DS1_03_PATGEN_H_ACTIVE_1);
        vRegWriteFldAlign(SCPIP_DS1_03+dsOffset, u4HTotal, DS1_03_PATGEN_H_TOTAL_1);
        vRegWriteFldAlign(SCPIP_DS1_04+dsOffset, u4Height, DS1_04_PATGEN_V_ACTIVE_1);
        vRegWriteFldAlign(SCPIP_DS1_04+dsOffset, u4VTotal, DS1_04_PATGEN_V_TOTAL_1);

        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, u4YUV, DS1_02_PTCOLOR_1);

        // disable frame track
        u4ForceFrameTrackCtrl |= SCPIP_FRAME_TRACK_OFF_DS_PATTERN;

        // turn on pattern
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_PTGEN_1);
    }
    else if (u4OnOff == VDP_DS_PTN_MANUAL)
    {
        vRegWriteFldAlign(SCPIP_DS1_03+dsOffset, u4Width, DS1_03_PATGEN_H_ACTIVE_1);
        vRegWriteFldAlign(SCPIP_DS1_03+dsOffset, u4HTotal, DS1_03_PATGEN_H_TOTAL_1);
        vRegWriteFldAlign(SCPIP_DS1_04+dsOffset, u4Height, DS1_04_PATGEN_V_ACTIVE_1);
        vRegWriteFldAlign(SCPIP_DS1_04+dsOffset, u4VTotal, DS1_04_PATGEN_V_TOTAL_1);

        // disable frame track
         u4ForceFrameTrackCtrl |= SCPIP_FRAME_TRACK_OFF_DS_PATTERN;

        // turn on pattern
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_PTGEN_1);
    }
    else // off
    {
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_PTGEN_1);
        u4ForceFrameTrackCtrl &= (~SCPIP_FRAME_TRACK_OFF_DS_PATTERN);    
    }
}


void vScpipSetUSPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4Width, UINT32 u4Height)
{
    UINT8 bPath;
    UINT32 u4PipOffset;
    UINT32 u4Offset;
    UINT32 u4Mode;
    
    if (u4VdpId >= VDP_NS) 
    { 
        return;
    }

    if (u4VdpId == VDP_1)
    {
        u4Offset = 0;
    }
    else
    {
        u4Offset = 4;
    }
    bPath = getScalerPath(u4VdpId);
    u4PipOffset = getPIPOffset(bPath);

    if ((u4OnOff == VDP_US_PTN_AUTO) || (u4OnOff == VDP_US_PTN_MANUAL))
    {
        UINT32 u4PreLen; //, u4YOfst;

        if (u4OnOff == VDP_US_PTN_AUTO)        
        {
            u4Width = _pPicInfo[u4VdpId]->wTargetW;
            u4Height = _pPicInfo[u4VdpId]->wTargetH;

            u4Width = DIVIDE_WITH_CARRIER(u4Width * u4ScpipGetHorizontalPreScaleDownFactor(u4VdpId),  DFT_HPD_FACTOR);
            u4Width = RSHIFT_WITH_CARRIER((u4Width * _pDispPrm[u4VdpId]->u4DSclH), DFT_DS_BIT_NUM);
            u4Height = RSHIFT_WITH_CARRIER((u4Height * _pDispPrm[u4VdpId]->u4DSclV), DFT_DS_BIT_NUM);

            LOG(3, "VDP(%d), Src after DS(%d, %d)\n", u4Width, u4Height);
        }

        u4PreLen = RegReadFldAlign(SCPIP_PIP1_0B+u4PipOffset, PIP1_0B_VS_PSCAN_PRELEN_10_8_1);
        u4PreLen = u4PreLen << 8;
        u4PreLen += RegReadFldAlign(SCPIP_PIP1_0C+u4PipOffset, PIP1_0C_VS_PSCAN_PRELEN_7_0_1);

        // set different prelen by mode
        u4Mode = u1ScpipGetDispMode(bPath);

        if (u4Mode == VDP_SCPOS_PSCAN_DISPMODE)
        {
            UINT32 u4PreVofst;
            if(RegReadFldAlign(SCPIP_US1_01, US1_01_VUS_BYPASS_1) == 0)
            {
                u4PreVofst = RegReadFldAlign(SCPIP_PIP_OA_03, PIP_OA_03_PRE_VOFST1);
            }
            else
            {
                u4PreVofst = RegReadFldAlign(SCPIP_PIP_OA_03, PIP_OA_03_PRE_VOFST_BYPASS1);                
            }
            u4PreLen += u4PreVofst;
        }
        else
        {
            u4PreLen += 1;
        }

        #ifdef CC_SCPOS_3DTV_SUPPORT        
        if(u4VdpId == VDP_1)
        {
            if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D))
            {
                if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_LINE_INTERLEAVE)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_FRAME_INTERLEAVE))
                {                    
                    u4Height <<= 1;
                    if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_LINE_INTERLEAVE))
                    {
                        u4Width >>= 1;                        
                    }
                }
            }
        }        
        #endif   
   
        vRegWriteFldAlign(SCPIP_PIP_OA_04+u4Offset, u4Width, PIP_OA_04_MAIN_PAT_HTOTAL);
        vRegWriteFldAlign(SCPIP_PIP_OA_04+u4Offset, u4Height, PIP_OA_04_MAIN_PAT_VTOTAL);
        vRegWriteFldAlign(SCPIP_PIP_OA_04+u4Offset, u4PreLen, PIP_OA_04_MAIN_PAT_V_START);

        // turn on pattern
        if (u4VdpId == VDP_1)
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_MAIN_PAT_EN);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_SUB_PAT_EN);
        }
    }
    else // off
    {
        if (u4VdpId == VDP_1)
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_MAIN_PAT_EN);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_SUB_PAT_EN);
        }
    }
}

#if SUPPORT_DOT_TO_DOT_PATTERN
 void VDP_GetPattern(UINT8 bIndex)
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
            case 10: //one dot on off
                G0PixelValue = 0xff;
                B0PixelValue = 0xff;
                R0PixelValue = 0xff;
                G1PixelValue = 0x00;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 1;
               // SpecialRevLine = 1;
                break;            
            case 11: //1sub dot on off
                G0PixelValue = 0x00;
                B0PixelValue = 0xff;
                R0PixelValue = 0xff;
                G1PixelValue = 0xff;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 1;
              //  SpecialRevLine = 1;
                break;
            case 12: //1v on off
                G0PixelValue = 0xff;
                B0PixelValue = 0xff;
                R0PixelValue = 0xff;
                G1PixelValue = 0x00;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 0;
               // SpecialRevLine = 1;
                break;
            case 13: // 1v sub on off
                G0PixelValue = 0x00;
                B0PixelValue = 0xff;
                R0PixelValue = 0xff;
                G1PixelValue = 0xff;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 0;
              //  SpecialRevLine = 1;
                break;   
            case 14: //two dot on off
                G0PixelValue = 0xff;
                B0PixelValue = 0xff;
                R0PixelValue = 0xff;
                G1PixelValue = 0x00;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 2;
               // SpecialRevLine = 1;
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
void vScpipDotToDotPattern(UINT8 ucOnOff,UINT32 bIndex)
    #if 1
{	     
	static UINT8 _uDisableFunction=0;
	UINT8 GPixelValue;
	UINT8 BPixelValue;
	UINT8 RPixelValue;
	UINT16 uPattern_Width;
	UINT16 uPattern_Height;
	UINT16 RevLine = 0;    
	
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
	    vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_W_EN_1);
		  
		vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_R_EN_1);
		vRegWriteFldAlign(OUTSTG_OS_17, 1, OS_17_MUTE_MAIN);
		
		BaseAddress= (RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1))<<4;
		
		if(bCount==1)
		{
		    bOriEN3D = RegReadFldAlign(C_COMB_2A, EN3D); 
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
			bOriYUV2RGB = RegReadFldAlign(OS_84, YUV2RGB_BYPASS_MAIN);	
			wOriHBoundary = RegReadFldAlign(SCPIP_PIP1_0F,PIP1_0F_BOUNDARY_H_POS_1);
			dwPreBaseAddress = (RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1));
			bOriDramBurstMode = (RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_DRAM_BURST_MODE_1));
		}
	       vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_FSEL_1);
		vScpipSetScalerDispmode(SV_VP_MAIN,SV_OFF);
		vScpipSetPSCANDispmode(SV_VP_MAIN,SV_OFF);
		vRegWriteFldAlign(C_COMB_2A, SV_OFF, EN3D);
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
		vRegWriteFldAlign(OS_84, 1, YUV2RGB_BYPASS_MAIN);
		vRegWriteFldAlign(SCPIP_PIP_OA_03, 0, PIP_OA_03_US_VOFST_BYPASS1);
		vRegWriteFldAlign(SCPIP_PIP1_0F, uPattern_Width, PIP1_0F_BOUNDARY_H_POS_1);
        vRegWriteFldAlign(SCPIP_US1_14, 1, US1_14_UP_VBOUND_EN1);
        vRegWriteFldAlign(SCPIP_US1_14, uPattern_Height, US1_14_UP_VBOUND_Y1);
		vRegWriteFldAlign(SCPIP_PIP1_05, 0, PIP1_05_DRAM_BURST_MODE_1);
		if(BaseAddress < 0x1000)
		{
			BaseAddress = (0x229c00)<<4; 
			vRegWriteFldAlign(SCPIP_PIP1_05, 0x229c00,PIP1_05_WSTARTP_1);
			vRegWriteFldAlign(SCPIP_PIP1_06, 0x229c00,PIP1_06_RSTARTP_1);
		}
		#if 1
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
		#else
		for(VerticalCounter = 0; VerticalCounter < PANEL_GetPanelHeight()*2; VerticalCounter ++)//Write Dram Data
		{
			bTemp = 15;
			for(HorizontalConuter = 0; HorizontalConuter < bDate_Length; HorizontalConuter ++)
			{
				switch(HorizontalConuter%3)
				{				
				       case 0: //left one byte
					for(bPackCount=0;bPackCount<5;bPackCount++)
					{
						vWriteDramB(TempAddress+(bTemp--),uGValue);
						vWriteDramB(TempAddress+(bTemp--),uBValue);
						vWriteDramB(TempAddress+(bTemp--),uRValue);
					}
					vWriteDramB(TempAddress+(bTemp--),uGValue);
					TempAddress += 16;
					bTemp = 15;	
					break;
					
					case 1://left two byte
					for(bPackCount=0;bPackCount<5;bPackCount++)
					{
						vWriteDramB(TempAddress+(bTemp--),uBValue);
						vWriteDramB(TempAddress+(bTemp--),uRValue);
						vWriteDramB(TempAddress+(bTemp--),uGValue);
					}					
					vWriteDramB(TempAddress+(bTemp--),uBValue);
					TempAddress += 16;
					bTemp = 15;					
					break;
					
					case 2:
					for(bPackCount=0;bPackCount<5;bPackCount++)
					{
						vWriteDramB(TempAddress+(bTemp--),uRValue);
						vWriteDramB(TempAddress+(bTemp--),uGValue);
						vWriteDramB(TempAddress+(bTemp--),uBValue);
					}					
					vWriteDramB(TempAddress+(bTemp--),uRValue);
					TempAddress += 16;
					bTemp = 15;
					break;
					
					default:
					    break;
				}
			}
		}
		#endif
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
			
			vScpipImportOnOff(SV_VP_MAIN,SV_OFF);
			vRegWriteFldAlign(C_COMB_2A, bOriEN3D, EN3D);
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
			vRegWriteFldAlign(OS_84, bOriYUV2RGB, YUV2RGB_BYPASS_MAIN);
			vRegWriteFldAlign(SCPIP_PIP1_0F, wOriHBoundary,PIP1_0F_BOUNDARY_H_POS_1);
			vRegWriteFldAlign(SCPIP_PIP1_05, dwPreBaseAddress,PIP1_05_WSTARTP_1);
			vRegWriteFldAlign(SCPIP_PIP1_05, dwPreBaseAddress,PIP1_06_RSTARTP_1);
			vRegWriteFldAlign(SCPIP_PIP1_05, bOriDramBurstMode, PIP1_05_DRAM_BURST_MODE_1);
			vScpipImportOnOff(SV_VP_MAIN,SV_OFF);
		}
	}
 }
#else
{	     
	static UINT8 _uDisableFunction=0;

       if((_uDisableFunction ==0) && (ucOnOff==0))
        return;

        _uDisableFunction = ucOnOff;
        
       if(ucOnOff == 1)
	{
        	UINT32 TempAddress;
		UINT16 bDate_Length;
		UINT16 VerticalCounter=0;
		UINT16 HorizontalConuter=0;
		
		INT8 bTemp = 15;
		UINT8 uGValue;
		UINT8 uBValue;
		UINT8 uRValue;
		UINT8 bPackCount=0;
		
		uGValue=(UINT8)(u4RGBColor&0x000000ff);
		uBValue=(UINT8)((u4RGBColor>>8)&0x000000ff);
		uRValue=(UINT8)((u4RGBColor>>16)&0x000000ff);
		bDate_Length=(PANEL_GetPanelWidth()*3%16)?(PANEL_GetPanelWidth()*3/16+ 1):(PANEL_GetPanelWidth()*3/16+0);
		_rMChannel.bIsFreeze=1;
		bCount++;
		_vDrvVideoSetMute(MUTE_MODULE_SCPOS_PATTERN, VDP_1, FOREVER_MUTE, TRUE);
	      vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_W_EN_1);
		TempAddress= (RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1))<<4;
		
		if(bCount==1)
		{
		    bOriEN3D = RegReadFldAlign(C_COMB_2A, EN3D); 
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
			bOriYUV2RGB = RegReadFldAlign(OS_84, YUV2RGB_BYPASS_MAIN);	
		}
	       vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_FSEL_1);
		vScposSetScalerDispmode(SV_VP_MAIN,SV_OFF);
		vScposSetPSCANDispmode(SV_VP_MAIN,SV_OFF);
		vRegWriteFldAlign(C_COMB_2A, SV_OFF, EN3D);
		vRegWriteFldAlign(SCPIP_DS1_02, 1, DS1_02_SEL444_1);
		vRegWriteFldAlign(SCPIP_DS1_02, 0, DS1_02_VD10BSEL_1);
		vRegWriteFldAlign(SCPIP_US1_00, 0x8000, US1_00_USCALE_H_1);
		vRegWriteFldAlign(SCPIP_US1_00, 0x8000, US1_00_USCALE_V_1); 
		vRegWriteFldAlign(SCPIP_US1_01, 1, US1_01_VUS_BYPASS_1);
		vRegWriteFldAlign(SCPIP_PIP1_08, bDate_Length, PIP1_08_DATA_LENGTH_1);
		vRegWriteFldAlign(SCPIP_PIP1_01, bDate_Length, PIP1_01_HL_1);
		vRegWriteFldAlign(SCPIP_PIP1_00, PANEL_GetPanelHeight(), PIP1_00_VL_1);
		vRegWriteFldAlign(SCPIP_PIP1_03, 0, PIP1_03_RHP_1);
		vRegWriteFldAlign(SCPIP_PIP1_03, PANEL_GetPanelWidth(), PIP1_03_RHL_1);
		vRegWriteFldAlign(SCPIP_PIP1_02, 0, PIP1_02_RVP_1);
		vRegWriteFldAlign(SCPIP_PIP1_02, PANEL_GetPanelHeight(), PIP1_02_RVL_1);
		vRegWriteFldAlign(SCE_CFG_MAIN, 1, ALLBP);// bypass SCE
		vRegWriteFldAlign(OS_84, 1, YUV2RGB_BYPASS_MAIN);
		vRegWriteFldAlign(SCPIP_PIP_OA_03, 0, PIP_OA_03_US_VOFST_BYPASS1);

		for(VerticalCounter = 0; VerticalCounter < PANEL_GetPanelHeight()*2; VerticalCounter ++)//Write Dram Data
		{
			bTemp = 15;
			for(HorizontalConuter = 0; HorizontalConuter < bDate_Length; HorizontalConuter ++)
			{
				switch(HorizontalConuter%3)
				{				
				       case 0: //left one byte
					for(bPackCount=0;bPackCount<5;bPackCount++)
					{
						vWriteDramB(TempAddress+(bTemp--),uGValue);
						vWriteDramB(TempAddress+(bTemp--),uBValue);
						vWriteDramB(TempAddress+(bTemp--),uRValue);
					}
					vWriteDramB(TempAddress+(bTemp--),uGValue);
					TempAddress += 16;
					bTemp = 15;	
					break;
					
					case 1://left two byte
					for(bPackCount=0;bPackCount<5;bPackCount++)
					{
						vWriteDramB(TempAddress+(bTemp--),uBValue);
						vWriteDramB(TempAddress+(bTemp--),uRValue);
						vWriteDramB(TempAddress+(bTemp--),uGValue);
					}					
					vWriteDramB(TempAddress+(bTemp--),uBValue);
					TempAddress += 16;
					bTemp = 15;					
					break;
					
					case 2:
					for(bPackCount=0;bPackCount<5;bPackCount++)
					{
						vWriteDramB(TempAddress+(bTemp--),uRValue);
						vWriteDramB(TempAddress+(bTemp--),uGValue);
						vWriteDramB(TempAddress+(bTemp--),uBValue);
					}					
					vWriteDramB(TempAddress+(bTemp--),uRValue);
					TempAddress += 16;
					bTemp = 15;
					break;
					
					default:
					    break;
				}
			}
		}
		_vDrvVideoSetMute(MUTE_MODULE_SCPOS_PATTERN, VDP_1,10, FALSE);  
	   }	 
	   else
	   { 	  
	   	//if(_uDisableFunction==1)
		{ 
		    bCount=0;
			//_uDisableFunction=0;	
			_rMChannel.bIsFreeze=0;
			_vDrvVideoSetMute(MUTE_MODULE_SCPOS_PATTERN, VDP_1, 10, TRUE);
			vDrvMainImportOnOff(SV_OFF);
			vRegWriteFldAlign(C_COMB_2A, bOriEN3D, EN3D);
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
			vRegWriteFldAlign(OS_84, bOriYUV2RGB, YUV2RGB_BYPASS_MAIN);
			vDrvMainImportOnOff(SV_ON);
		}
	}
 }
#endif


#endif

