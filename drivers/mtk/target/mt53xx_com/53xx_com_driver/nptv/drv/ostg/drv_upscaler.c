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
 * $RCSfile: drv_upscaler.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_ospe.c
 *  Brief of file drv_ospe.c.
 *  Details of file drv_ospe.c (optional).
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "general.h"
#include "hw_scpos.h"
#include "drv_upscaler.h"
#include "drv_scaler.h"
#include "drv_scaler_table.h"
#include "vdo_misc.h"
#include "x_rand.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/**************************************************************************
 * EdgeSmooth and Dering Initial
 *************************************************************************/

RRegDwrdTbl const CODE REGTBL_EDGE_SMOOTH[] =
{
{    SCPIP_US1_0C, 0x00000F00, 0x00000F01},
{    SCPIP_US1_0D, 0x0F0F0655, 0xFFFFFFFF},
{    SCPIP_US1_0E, 0xDF0F1003, 0xFFFFFFFF},
{    SCPIP_US1_12, 0x89081510, 0xFF3F7F3F},
{    SCPIP_US1_13, 0x00001510, 0x00007F3F},
{    	  REG_END, 0x00000000, 0x00000000},
};

RRegDwrdTbl const CODE REGTBL_DE_RING[] =
{
{    SCPIP_US1_01, 0x20400080, 0x00E00000},
{    SCPIP_US1_02, 0xA300007F, 0xf3ffffff},
{    SCPIP_US1_03, 0xF3FFFF00, 0xf3ffff0f},
{         REG_END, 0x00000000, 0x00000000},
};


UINT32 u4VScalerTaps[VDP_MAX_NS] = {UPSCALER_8TAP, UPSCALER_4TAP};
UINT32 u42DEdgeSmoothEnabled = SV_FALSE;

/**************************************************************************
 * Filter Coefficients Definition
 *************************************************************************/

UINT32 u4RegReadData;
UINT8 const CODE bPhaseNum = 17;
UINT8 const CODE bHorCoefTblOffset = 0;
UINT8 const CODE bHorCoefLength = 6;
UINT8 const CODE bHorCoefTotal = 102;					//HorCoefTotal = bHorCoefLength*bPhaseNum;   
UINT8 const CODE bVerYCoefTblOffset = 102 ;			//bVerYCoefTblOffset = bHorCoefTblOffset + bHorCoefTotal;    
UINT8 const CODE bVerYCoefLength = 3;
UINT8 const CODE bVerYCoefTotal = 51;					//bVerYCoefTotal= bVerYCoefLength*bPhaseNum;
UINT8 const CODE bVerCbCrCoefTblOffset =153;  			//bVerCbCrCoefTblOffset = bVerYCoefTblOffset + bVerYCoefTotal; 
UINT8 const CODE bVerCbCrCoefLength = 3;
UINT8 const CODE bVerCbCrCoefTotal = 51;				//bVerCbCrCoefTotal = bVerCbCrCoefLength*bPhaseNum;

UINT32 u4FilterCoeff[204] =
{
	#include "UPS_COEF.coef"
};

//-----------------------------------------------------------------------------
/**
 * @brief vDrvUpScalerInit

 * @param void
 * @retval void
 */
//-----------------------------------------------------------------------------
void vDrvUpScalerInit(void)
{
    
    // Set Vertical Taps 
    vRegWriteFldAlign(SCPIP_US1_04, 0, US1_04_VTAP_SEL_1);			// Set 4 Tap
    
    // Set UpScaler Coefficient from tbl
    vDrvSetUpScalerFilter(SV_VP_MAIN, UPSCALER_HORI, 0);
    vDrvSetUpScalerFilter(SV_VP_MAIN, UPSCALER_VERT, 0);
    
    // Enable SRAM Coefficient
    vDrvSetUpScalerProgOnOff(SV_VP_MAIN, UPSCALER_HORI, SV_OFF);
    vDrvSetUpScalerProgOnOff(SV_VP_MAIN, UPSCALER_VERT, SV_ON);
    
    // SCPIP_PQ Initialize
    vDrv2DEdgeSmoothInit();
    vDrvDeRingInit();
}

//-----------------------------------------------------------------------------
/**
* @brief vDrvGetUpScalerVTap

 * @param bPath
 * @retval UINT32 up-scaler tap setting
 */
//-----------------------------------------------------------------------------
UINT32 vDrvGetUpScalerVTap(UINT8 bPath)
{    
    if(bPath >= VDP_MAX_NS)
    {
        return UPSCALER_8TAP;
    }
    else
    {
        return u4VScalerTaps[bPath];
    }
}

//-----------------------------------------------------------------------------
/**
 * @brief vDrvSetUpScalerVTap

 * @param bType, bTap
 * @retval UINT32
 */
//-----------------------------------------------------------------------------
void vDrvSetUpScalerVTap(UINT8 bPath, UINT8 bTap)
{
    UINT8 bSel = (bTap == UPSCALER_8TAP)? 2 : (bTap == UPSCALER_6TAP)? 1 : 0;
    u4VScalerTaps[bPath] = bTap;
    vRegWriteFldAlign(SCPIP_US1_04, bSel, US1_04_VTAP_SEL_1);    
}

//-----------------------------------------------------------------------------
/**
 * @brief vDrvSetUpScalerFilterOnOff

 * @param bType bOnOff
 * @retval UINT32
 */
//-----------------------------------------------------------------------------
void vDrvSetUpScalerProgOnOff(UINT8 bPath, UINT8 bType, UINT8 bOnOff)
{
    if (bPath == SV_VP_MAIN)
    {
        if (bType == UPSCALER_HORI)
        {
            vRegWriteFldAlign(SCPIP_US1_04, bOnOff, US1_04_SRAM_COE_ENH_1);
            vRegWriteFldAlign(SCPIP_US1_04, bOnOff, US1_04_SRAM_COE_CSH_1);
            vRegWriteFldAlign(SCPIP_US1_04, bOnOff, US1_04_SRAM_COE_CSH2_1);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_US1_04, bOnOff, US1_04_SRAM_COE_ENV_1);
            vRegWriteFldAlign(SCPIP_US1_04, bOnOff, US1_04_SRAM_COE_CSV_Y_1);
            vRegWriteFldAlign(SCPIP_US1_04, bOnOff, US1_04_SRAM_COE_CSV_CbCr_1);
        }
    }
}

//-----------------------------------------------------------------------------
/**
 * @brief vDrvSetUpScalerFilter

 * @param bTap
 * @retval UINT32
 */
//-----------------------------------------------------------------------------
void vDrvSetUpScalerFilter(UINT8 bPath, UINT8 bType, UINT8 bMode)
{
    UINT8 bSramAddr;
    UINT8 HORI_Index;
    UINT8 VER_Y_Index;
    UINT8 VER_C_Index;
    if (bPath == SV_VP_MAIN)
    {
        u4RegReadData = RegReadFldAlign(SCPIP_US1_04, Fld(32, 0, AC_FULLDW));

        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_ENV_1);
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_ENH_1);
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_Y_1);
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_CbCr_1);


        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSH_1);
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSH2_1);
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_REN_1);
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_WEN_1);

        if (bType == UPSCALER_HORI)
        {
            for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)
            {
                HORI_Index = bSramAddr*bHorCoefLength+bHorCoefTblOffset;
                if (HORI_Index > 198) 		// for clock check
                	{
                		HORI_Index = 198;
                	}
                vRegWriteFldAlign(SCPIP_US1_04, bSramAddr,US1_04_SRAM_COE_WADD_1);
                vRegWriteFldAlign(SCPIP_US1_05, u4FilterCoeff[HORI_Index + 3], US1_05_SRAM_COE_WDATA1_1);
                vRegWriteFldAlign(SCPIP_US1_06, u4FilterCoeff[HORI_Index + 4], US1_06_SRAM_COE_WDATA2_1);
                vRegWriteFldAlign(SCPIP_US1_07, u4FilterCoeff[HORI_Index + 5], US1_07_SRAM_COE_WDATA3_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_WEN_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_SRAM_COE_CSH_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSH_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_WEN_1);

                vRegWriteFldAlign(SCPIP_US1_05, u4FilterCoeff[HORI_Index + 0], US1_05_SRAM_COE_WDATA1_1);
                vRegWriteFldAlign(SCPIP_US1_06, u4FilterCoeff[HORI_Index + 1], US1_06_SRAM_COE_WDATA2_1);
                vRegWriteFldAlign(SCPIP_US1_07, u4FilterCoeff[HORI_Index + 2], US1_07_SRAM_COE_WDATA3_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_WEN_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_SRAM_COE_CSH2_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSH2_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_WEN_1);
            }
        }
        else if (bType == UPSCALER_VERT)
        {
            for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)
            {      
                VER_Y_Index = bSramAddr*bVerYCoefLength+bVerYCoefTblOffset;            
                VER_C_Index = bSramAddr*bVerCbCrCoefLength+bVerCbCrCoefTblOffset;                      
                if (VER_Y_Index > 201) 		// for clock check
                	{
                		VER_Y_Index = 201;
                	}  
                if (VER_C_Index > 201) 		// for clock check
                	{
                		VER_C_Index = 201;
                	}                
                vRegWriteFldAlign(SCPIP_US1_04, bSramAddr, US1_04_SRAM_COE_WADD_1);
                vRegWriteFldAlign(SCPIP_US1_05, u4FilterCoeff[VER_Y_Index + 0], US1_05_SRAM_COE_WDATA1_1);
                vRegWriteFldAlign(SCPIP_US1_06, u4FilterCoeff[VER_Y_Index + 1], US1_06_SRAM_COE_WDATA2_1);
                vRegWriteFldAlign(SCPIP_US1_07, u4FilterCoeff[VER_Y_Index + 2], US1_07_SRAM_COE_WDATA3_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_WEN_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_SRAM_COE_CSV_Y_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_Y_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_WEN_1);

                vRegWriteFldAlign(SCPIP_US1_04, bSramAddr, US1_04_SRAM_COE_WADD_1);
                vRegWriteFldAlign(SCPIP_US1_05, u4FilterCoeff[VER_C_Index + 0], US1_05_SRAM_COE_WDATA1_1);
                vRegWriteFldAlign(SCPIP_US1_06, u4FilterCoeff[VER_C_Index + 1], US1_06_SRAM_COE_WDATA2_1);
                vRegWriteFldAlign(SCPIP_US1_07, u4FilterCoeff[VER_C_Index + 2], US1_07_SRAM_COE_WDATA3_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_WEN_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_SRAM_COE_CSV_CbCr_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_CbCr_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_WEN_1);               
            }
        }

        vRegWrite4B(SCPIP_US1_04, u4RegReadData);
        // warning clean
    }
}

//-----------------------------------------------------------------------------
/**
 * @brief vDrvCheckUpScalerFilter

 * @param bTap
 * @retval UINT8
 */
//-----------------------------------------------------------------------------
UINT8 vDrvCheckUpScalerFilter(UINT8 bPath, UINT8 bType, UINT8 bMode)
{
    UINT8 bSramAddr;
    UINT32 u4RegReadData;
    UINT32 u4SramReadData1, u4SramReadData2, u4SramReadData3;
    UINT8 HORI_Index;
    UINT8 VER_Y_Index;
    UINT8 VER_C_Index;
    
    if (bPath == SV_VP_MAIN)
    {
        u4RegReadData = RegReadFldAlign(SCPIP_US1_04, Fld(32, 0, AC_FULLDW));

        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_ENV_1);
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_ENH_1);
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_Y_1);
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_CbCr_1);        
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSH_1);
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSH2_1);
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_REN_1);
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_WEN_1);

        if (bType == UPSCALER_HORI)
        {
            for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)                                                  
            {
                HORI_Index = bSramAddr*bHorCoefLength+bHorCoefTblOffset;
                if (HORI_Index > 198) 		// for clock check
                	{
                		HORI_Index = 198;
                	}
                vRegWriteFldAlign(SCPIP_US1_04, bSramAddr, US1_04_SRAM_COE_WADD_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_REN_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_SRAM_COE_CSH_1);
                u4SramReadData1 = RegReadFldAlign(SCPIP_US1_08, US1_08_SRAM_COE_RDATA1_1);
                u4SramReadData2 = RegReadFldAlign(SCPIP_US1_09, US1_09_SRAM_COE_RDATA2_1);
                u4SramReadData3 = RegReadFldAlign(SCPIP_US1_0A, US1_0A_SRAM_COE_RDATA3_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSH_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_REN_1);

                if (u4SramReadData1 != u4FilterCoeff[HORI_Index + 3] ||
                    u4SramReadData2 != u4FilterCoeff[HORI_Index + 4] ||
                    u4SramReadData3 != u4FilterCoeff[HORI_Index + 5])
                {
                    return FALSE;
                }

                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_REN_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_SRAM_COE_CSH2_1);
                u4SramReadData1 = RegReadFldAlign(SCPIP_US1_08, US1_08_SRAM_COE_RDATA1_1);
                u4SramReadData2 = RegReadFldAlign(SCPIP_US1_09, US1_09_SRAM_COE_RDATA2_1);
                u4SramReadData3 = RegReadFldAlign(SCPIP_US1_0A, US1_0A_SRAM_COE_RDATA3_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSH2_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_REN_1);

                if (u4SramReadData1 != u4FilterCoeff[HORI_Index + 0] ||
                    u4SramReadData2 != u4FilterCoeff[HORI_Index + 1] ||
                    u4SramReadData3 != u4FilterCoeff[HORI_Index + 2])
                {
                    return FALSE;
                }
            }
        }
        else if (bType == UPSCALER_VERT)
        {
            for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)
            {
                VER_Y_Index = bSramAddr*bVerYCoefLength+bVerYCoefTblOffset;    
                if (VER_Y_Index > 201) 		// for clock check
                	{
                		VER_Y_Index = 201;
                	}             
                vRegWriteFldAlign(SCPIP_US1_04, bSramAddr, US1_04_SRAM_COE_WADD_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_REN_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_SRAM_COE_CSV_Y_1);

                u4SramReadData1 = RegReadFldAlign(SCPIP_US1_08, US1_08_SRAM_COE_RDATA1_1);
                u4SramReadData2 = RegReadFldAlign(SCPIP_US1_09, US1_09_SRAM_COE_RDATA2_1);
                u4SramReadData3 = RegReadFldAlign(SCPIP_US1_0A, US1_0A_SRAM_COE_RDATA3_1);

                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_Y_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_REN_1);

                if (u4SramReadData1 != u4FilterCoeff[VER_Y_Index + 0] ||
                    u4SramReadData2 != u4FilterCoeff[VER_Y_Index + 1] ||
                    u4SramReadData3 != u4FilterCoeff[VER_Y_Index + 2])
                {
                    return FALSE;
                }
            }
            for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)
            {
                VER_C_Index = bSramAddr*bVerCbCrCoefLength+bVerCbCrCoefTblOffset;                      
                if (VER_C_Index > 201) 		// for clock check
                	{
                		VER_C_Index = 201;
                	}               
                vRegWriteFldAlign(SCPIP_US1_04, bSramAddr, US1_04_SRAM_COE_WADD_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_REN_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_SRAM_COE_CSV_CbCr_1);

                u4SramReadData1 = RegReadFldAlign(SCPIP_US1_08, US1_08_SRAM_COE_RDATA1_1);
                u4SramReadData2 = RegReadFldAlign(SCPIP_US1_09, US1_09_SRAM_COE_RDATA2_1);
                u4SramReadData3 = RegReadFldAlign(SCPIP_US1_0A, US1_0A_SRAM_COE_RDATA3_1);

                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_CbCr_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_REN_1);

                if (u4SramReadData1 != u4FilterCoeff[VER_C_Index + 0] ||
                    u4SramReadData2 != u4FilterCoeff[VER_C_Index + 1] ||
                    u4SramReadData3 != u4FilterCoeff[VER_C_Index + 2])
                {
                    return FALSE;
                }
            }
        }

        vRegWrite4B(SCPIP_US1_04, u4RegReadData);
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/**
 * @brief vDrvTestUpScalerFilter

 * @param void
 * @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL vDrvTestUpScalerFilter(void)
{
    vDrvSetUpScalerFilter(SV_VP_MAIN, UPSCALER_HORI, 0);
    if (vDrvCheckUpScalerFilter(SV_VP_MAIN, UPSCALER_HORI, 0) == FALSE)
    {
        return FALSE;
    }

    vDrvSetUpScalerFilter(SV_VP_MAIN, UPSCALER_VERT, 0);
    if (vDrvCheckUpScalerFilter(SV_VP_MAIN, UPSCALER_VERT, 0) == FALSE)
    {
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************************/
/*************************************   Edge Smooth   **************************************/
/*****************************************************************************************/
void vDrv2DEdgeSmoothInit(void)
{
    vDrvLoadRegDwrdTbl(REGTBL_EDGE_SMOOTH);
    #ifdef CC_FLIP_MIRROR_SUPPORT
        vDrv2DEdgeSmoothOnOff(SV_OFF);
    #endif
}

void vDrv2DEdgeSmoothOnOff(UINT8 bOnOff)
{
    UINT32 u4RegReadData;
    u42DEdgeSmoothEnabled = bOnOff;
    u4RegReadData = RegReadFldAlign(SCPIP_US1_04, US1_04_SRAM_COE_CSV_Y_1);
    vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_Y_1);
    vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_CbCr_1);    
    vRegWriteFldAlign(SCPIP_US1_0C, bOnOff, US1_0C_2D_SmoothEdge_En);
    vRegWriteFldAlign(SCPIP_US1_04, u4RegReadData, US1_04_SRAM_COE_CSV_Y_1);
    vRegWriteFldAlign(SCPIP_US1_04, u4RegReadData, US1_04_SRAM_COE_CSV_CbCr_1);    
}

UINT32 bDrvIs2DEdgeSmoothEnabled(void)
{
    return u42DEdgeSmoothEnabled;
}

/*****************************************************************************************/
/*************************************   Dering   *******************************************/
/*****************************************************************************************/

void vDrvDeRingInit(void)
{
    vDrvLoadRegDwrdTbl(REGTBL_DE_RING);
}


void vDrvDeRingOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(SCPIP_US1_02, bOnOff, US1_02_DE_RING_EN_H_1);
        vRegWriteFldAlign(SCPIP_US1_03, bOnOff, US1_03_DE_RING_EN_V_1);
    }
}

