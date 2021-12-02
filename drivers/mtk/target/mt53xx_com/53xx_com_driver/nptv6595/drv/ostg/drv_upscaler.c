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
#include "hw_ycproc.h"
#include "vdo_misc.h"
#include "x_rand.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/**************************************************************************
 * EdgeSmooth and Dering and IRR Initial
 *************************************************************************/

RRegDwrdTbl const CODE REGTBL_EDGE_SMOOTH[] =
{
{    SCPIP_US1_0C, 0x00000F04, 0x00000F3D},
{    SCPIP_US1_0D, 0x0F0F0655, 0xFFFFFFFF},
{    SCPIP_US1_0E, 0xDE0F1003, 0xFEFFFFFF},
{    SCPIP_US1_12, 0x95101510, 0xFF3F7F3F},
{    SCPIP_US1_13, 0x02A80908, 0x03FFFF3F},
{    SCPIP_US1_14, 0x00078010, 0x000FFFFF},
{         REG_END, 0x00000000, 0x00000000},
};

RRegDwrdTbl const CODE REGTBL_DE_RING_VDO[] =
{
{    SCPIP_US1_01, 0x00600000, 0x00E00000},
{    SCPIP_US1_02, 0xB3010100, 0xf3ffffff},
{    SCPIP_US1_03, 0xB3010100, 0xf3ffff0f},
{         REG_END, 0x00000000, 0x00000000},
};

RRegDwrdTbl const CODE REGTBL_DE_RING_PC[] =
{
{    SCPIP_US1_01, 0x00E00000, 0x00E00000},
{    SCPIP_US1_02, 0xF3FFFF7F, 0xf3ffffff},
{    SCPIP_US1_03, 0xF3FFFF00, 0xf3ffff0f},
{         REG_END, 0x00000000, 0x00000000},
};

#ifdef CC_MT5365
//5365 IRR initial table
RRegDwrdTbl const CODE REGTBL_IRR[] =
{
{          IRR_00, 0x800C1903, 0xBFFFFFFF},
{          IRR_01, 0x00800400, 0xFFEFF7FF},
{          IRR_02, 0x8F017523, 0xEFFFFF7F},
{          IRR_03, 0x00440C50, 0xFFEFFEFF},
{          IRR_04, 0x10004040, 0xFFFFF3FF},
{         REG_END, 0x00000000, 0x00000000},
};
#else
//5395 IRR initial table
RRegDwrdTbl const CODE REGTBL_IRR[] =
{
{          IRR_00, 0xB00C1902, 0xBFFFFFFF},
{          IRR_01, 0x00800400, 0xFFEFF7FF},
{          IRR_02, 0x8F014063, 0xEF0FFFFF},
{          IRR_03, 0x00440000, 0xFFEFFFFF},
{          IRR_04, 0x07004040, 0xFF78F3FF},
{          IRR_05, 0x2020F000, 0xFFFFFFFF},
{         REG_END, 0x00000000, 0x00000000},
};
#endif

UINT32 u4VScalerTaps[VDP_MAX_NS] = {UPSCALER_8TAP, UPSCALER_4TAP};
UINT8  bIRREnalbed = SV_FALSE;

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
    // Set ContinuousPhase
    vRegWriteFldAlign(SCPIP_US1_04, 1, US1_04_CP_H_EN);			// Set Hor En 
    vRegWriteFldAlign(SCPIP_US1_04, 1, US1_04_CP_V_EN);			// Set Ver En
  
    // Set Vertical Taps 
    vRegWriteFldAlign(SCPIP_US1_04, 1, US1_04_VTAP_SEL_1);			// Set 6 Tap
    
    // Set UpScaler Coefficient from tbl
    vDrvSetUpScalerFilter(SV_VP_MAIN, UPSCALER_HORI, 0);
    vDrvSetUpScalerFilter(SV_VP_MAIN, UPSCALER_VERT, 0);
    
    // Enable SRAM Coefficient
    vDrvSetUpScalerProgOnOff(SV_VP_MAIN, UPSCALER_HORI, SV_ON);
    vDrvSetUpScalerProgOnOff(SV_VP_MAIN, UPSCALER_VERT, SV_ON);
    
    // SCPIP_PQ Initialize
    vDrv2DEdgeSmoothInit();
    vDrvDeRingInit();
    vDrvIRRInit();
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
            vRegWriteFldAlign(SCPIP_US1_04, bOnOff, US1_04_SRAM_COE_CSV_CBCR_1);
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
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_CBCR_1);


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
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_SRAM_COE_CSV_CBCR_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_CBCR_1);
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
        vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_CBCR_1);        
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
                    Printf("[H][3] Read [%d]  Write [%d]\n", u4SramReadData1, u4FilterCoeff[HORI_Index + 3]);
                    Printf("[H][4] Read [%d]  Write [%d]\n", u4SramReadData2, u4FilterCoeff[HORI_Index + 4]);
                    Printf("[H][5] Read [%d]  Write [%d]\n", u4SramReadData3, u4FilterCoeff[HORI_Index + 5]);                    
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
                    Printf("[H][0] Read [%d]  Write [%d]\n", u4SramReadData1, u4FilterCoeff[HORI_Index + 0]);
                    Printf("[H][1] Read [%d]  Write [%d]\n", u4SramReadData2, u4FilterCoeff[HORI_Index + 1]);
                    Printf("[H][2] Read [%d]  Write [%d]\n", u4SramReadData3, u4FilterCoeff[HORI_Index + 2]);                                        
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
                    Printf("[Y][0] Read [%d]  Write [%d]\n", u4SramReadData1, u4FilterCoeff[VER_Y_Index + 0]);
                    Printf("[Y][1] Read [%d]  Write [%d]\n", u4SramReadData2, u4FilterCoeff[VER_Y_Index + 1]);
                    Printf("[Y][2] Read [%d]  Write [%d]\n", u4SramReadData3, u4FilterCoeff[VER_Y_Index + 2]);                          
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
                vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_SRAM_COE_CSV_CBCR_1);

                u4SramReadData1 = RegReadFldAlign(SCPIP_US1_08, US1_08_SRAM_COE_RDATA1_1);
                u4SramReadData2 = RegReadFldAlign(SCPIP_US1_09, US1_09_SRAM_COE_RDATA2_1);
                u4SramReadData3 = RegReadFldAlign(SCPIP_US1_0A, US1_0A_SRAM_COE_RDATA3_1);

                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_CSV_CBCR_1);
                vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_REN_1);

                if (u4SramReadData1 != u4FilterCoeff[VER_C_Index + 0] ||
                    u4SramReadData2 != u4FilterCoeff[VER_C_Index + 1] ||
                    u4SramReadData3 != u4FilterCoeff[VER_C_Index + 2])
                {
                    Printf("[C][0] Read [%d]  Write [%d]\n", u4SramReadData1, u4FilterCoeff[VER_C_Index + 0]);
                    Printf("[C][1] Read [%d]  Write [%d]\n", u4SramReadData2, u4FilterCoeff[VER_C_Index + 1]);
                    Printf("[C][2] Read [%d]  Write [%d]\n", u4SramReadData3, u4FilterCoeff[VER_C_Index + 2]);                      
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
}

void vDrv2DEdgeSmoothOnOff(UINT8 bOnOff)
{
    vRegWriteFldAlign(SCPIP_US1_0C, bOnOff, US1_0C_TDES_EN);
}

/*****************************************************************************************/
/*************************************   Dering   *******************************************/
/*****************************************************************************************/

void vDrvDeRingInit(void)
{

    if(bIsScalerInput444(SV_VP_MAIN))
    {	// load REGTBL_DE_RING_PC
    	vDrvLoadRegDwrdTbl(REGTBL_DE_RING_PC);    	
    }
    else
    {
    	//load REGTBL_DE_RING_VDO
	vDrvLoadRegDwrdTbl(REGTBL_DE_RING_VDO);		
    }
}


void vDrvDeRingOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(SCPIP_US1_02, bOnOff, US1_02_DE_RING_EN_H_1);
        vRegWriteFldAlign(SCPIP_US1_03, bOnOff, US1_03_DE_RING_EN_V_1);
    }
}

/*****************************************************************************************/
/*************************************      IRR     **************************************/
/*****************************************************************************************/
void vDrvIRRInit(void)
{
    vDrvLoadRegDwrdTbl(REGTBL_IRR);
}

void vDrvIRRSetOnOff(UINT8 bOnOff)
{
	bIRREnalbed = bOnOff;
    vRegWriteFldAlign(IRR_00, bOnOff, IRR_EN);
}

UINT32 bDrvIsIRREnabled(void)
{
    return bIRREnalbed;
}

void vDrvIRRLoadParam(void)
{
	vRegWriteFldAlign(IRR_00,wReadQualityTable(QUALITY_IRR_EN),IRR_EN);
	//write to s/w register first
	#ifdef CC_MT5365
	vIO32WriteFldAlign(IRR_QTY_ITEM_00,wReadQualityTable(QUALITY_IRR_FILTER)&0x3,IRR_SWREG_FILTER);	
	#else
	vIO32WriteFldAlign(IRR_QTY_ITEM_00,wReadQualityTable(QUALITY_IRR_FILTER),IRR_SWREG_FILTER);	
	#endif
	vRegWriteFldAlign(IRR_04,wReadQualityTable(QUALITY_IRR_FILTER_CLIP),IRR_FILTER_CLIP);                                             
	vRegWriteFldAlign(IRR_04,wReadQualityTable(QUALITY_IRR_LEVEL),IRR_LEVEL);                                               
	vRegWriteFldAlign(IRR_00,wReadQualityTable(QUALITY_IRR_TAP),IRR_TAP);                                               
	vRegWriteFldAlign(IRR_00,wReadQualityTable(QUALITY_IRR_EDGE_TH),IRR_EDGE_TH);                                               
	vRegWriteFldAlign(IRR_00,wReadQualityTable(QUALITY_IRR_EDGE_SLOPE),IRR_EDGE_SLOPE);                                               
	vRegWriteFldAlign(IRR_01,wReadQualityTable(QUALITY_IRR_LF_TH),IRR_LF_L_TH);                                               
	vRegWriteFldAlign(IRR_01,wReadQualityTable(QUALITY_IRR_LF_SLOPE),IRR_LF_L_SLOPE);                                               
	vRegWriteFldAlign(IRR_00,wReadQualityTable(QUALITY_IRR_HF_TH),IRR_HF_L_TH);                                               
	vRegWriteFldAlign(IRR_00,wReadQualityTable(QUALITY_IRR_HF_SLOPE),IRR_HF_L_SLOPE);                                               
	vRegWriteFldAlign(IRR_02,wReadQualityTable(QUALITY_IRR_RIPPLE_TH),IRR_RIPPLE_TH);                                               
	vRegWriteFldAlign(IRR_02,wReadQualityTable(QUALITY_IRR_DEACY_ER),IRR_DEACY_ER);                                               
	vRegWriteFldAlign(IRR_02,wReadQualityTable(QUALITY_IRR_DEACY_TH),IRR_DEACY_TH);                                               
	vRegWriteFldAlign(IRR_03,wReadQualityTable(QUALITY_IRR_FLAT_TH),IRR_FLAT_TH);                                               
	vRegWriteFldAlign(IRR_03,wReadQualityTable(QUALITY_IRR_FLAT_SLOPE),IRR_FLAT_SLOPE);                                               
	vRegWriteFldAlign(IRR_04,wReadQualityTable(QUALITY_IRR_FLAT_LEVEL),IRR_FLAT_LEVEL);                                               	
}

void vDrvIRRUpdateFilter(void)
{
	#ifdef CC_MT5365
	if(RegReadFldAlign(IRR_03, IRR_FILTER) != (IO32ReadFldAlign(IRR_QTY_ITEM_00,IRR_SWREG_FILTER) & 0x3))
	#else
	if(RegReadFldAlign(IRR_03, IRR_FILTER) != IO32ReadFldAlign(IRR_QTY_ITEM_00,IRR_SWREG_FILTER))
	#endif
	{
		vRegWriteFldAlign(IRR_03,IO32ReadFldAlign(IRR_QTY_ITEM_00,IRR_SWREG_FILTER),IRR_FILTER);	
	}
}

void vDrvScalerPQProc(void)
{
	//1. update IRR Filter. for Hiding 5365/5395 IRR register difference.
	vDrvIRRUpdateFilter();
}
