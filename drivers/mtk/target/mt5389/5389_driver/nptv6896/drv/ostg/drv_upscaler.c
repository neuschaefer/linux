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
#include "drv_scaler_psc.h"
#include "hw_ycproc.h"
#include "vdo_misc.h"
#include "x_rand.h"
#include "nptv_debug.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/**************************************************************************
 * PEI and Dering and Initial
 *************************************************************************/
#if defined(CC_MT5396) || defined(CC_MT5368)
RRegDwrdTbl const CODE REGTBL_PEI[] =
{
	{        US1_PEI_00, 0x00000005, 0xE7F00005},    
	{        US1_PEI_01, 0x00F04912, 0x01F0FF1F},    
	{        US1_PEI_02, 0x76007040, 0xFF81FFFF},    
	{        US1_PEI_03, 0x00054444, 0x000FFFFF},    
	{        US1_PEI_04, 0x00064864, 0x000FFFFF},    
	{        US1_PEI_05, 0x04244842, 0x0FFFFFFF},    
	{        US1_PEI_06, 0x00000000, 0x0001F000},    
	{        US1_PEI_08, 0x101002BF, 0x77FF07FF},    
	{        US1_PEI_09, 0x001001CF, 0x07FF07FF},    
	{        US1_PEI_0A, 0x0E0337B3, 0xFFFFFFFF},    
	{        US1_PEI_0B, 0x01DE0162, 0xFFFFFFFF},    
	{        US1_PEI_0C, 0x0080008A, 0xFFFFFFFF},    
	{        US1_PEI_0D, 0x005D0074, 0xFFFFFFFF},    
	{        US1_PEI_0E, 0x000F0010, 0xFFFFFFFF},    
	{        US1_PEI_10, 0x101002BF, 0x77FF07FF},    
	{        US1_PEI_11, 0x001001CF, 0x07FF07FF},    
	{        US1_PEI_12, 0xC0000146, 0xFFF0FFFF},    
	{        US1_PEI_13, 0xC198380A, 0xFFFFFFFF},    
	{        US1_PEI_20, 0x00000000, 0x80000000},    
	{        US1_PEI_0F, 0x00020003, 0xFFFFFFFF},    
	{         REG_END, 0x00000000, 0x00000000},  
};
#endif

RRegDwrdTbl const CODE REGTBL_DE_RING[] =
{
	{    SCPIP_US1_02, 0xB0301300, 0xb0ffffff},		// HOR_Y_DERING
	{    SCPIP_US1_03, 0xB0310000, 0xb0fff000},		// VER_DERING
	{    SCPIP_US1_1E, 0x02060060, 0x0ffffff0},		// HOR_UV_DERING
	{         REG_END, 0x00000000, 0x00000000},
};

UINT32 u4VScalerTaps[VDP_MAX_NS] = {UPSCALER_8TAP, UPSCALER_4TAP};

/**************************************************************************
 * Filter Coefficients Definition
 *************************************************************************/

UINT32 u4RegReadData;
UINT16 const CODE bPhaseNum = 129;
UINT16 const CODE bCoefLength = 3;
UINT16 const CODE bHorYCoefTblOffset = 0;			//bHorYCoefTotal = 0;
UINT16 const CODE bHorYCoefTotal = 387;				//bHorYCoefTotal = bCoefLength*bPhaseNum;   
UINT16 const CODE bHorCbCrCoefTblOffset = 387 ;			//bHorCbCrCoefTblOffset = bHorYCoefTblOffset + bHorYCoefTotal;    
UINT16 const CODE bHorCbCrCoefTotal = 387;			//bHorCbCrCoefTotal = bCoefLength*bPhaseNum;   
UINT16 const CODE bVerCoefTblOffset = 774 ;			//bVerCoefTblOffset = bHorCoefTblOffset + bHorCoefTotal;    
UINT16 const CODE bVerCoefTotal = 387;				//bVerYCoefTotal= bCoefLength*bPhaseNum;
UINT16 const CODE bUPSCoefTotal = 1161;				//bUPSCoefTotal= bHorYCoefTotal + bHorCbCrCoefTotal + bVerCoefTotal;
UINT8 const ModeNum=2;
UINT32 u4FilterCoeff[2322] =							//u4FilterCoeff size = ModeNum*bUPSCoefTotal;
{
	#include "UPS_COEF_422.coef"        // mode 0 : 422 mode
	#include "UPS_COEF_444.coef"        // mode 1 : 444 mode	
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
    vRegWriteFldAlign(SCPIP_US1_04, 1, US1_04_VTAP_SEL_1);			// Set 6 Tap
    
    // Set UpScaler Coefficient from tbl
    vDrvSetUpScalerFilter(SV_VP_MAIN, UPSCALER_HORI, 0);
    vDrvSetUpScalerFilter(SV_VP_MAIN, UPSCALER_VERT, 0);

    // Enable SRAM Coefficient
    vDrvSetUpScalerProgOnOff(SV_VP_MAIN, UPSCALER_HORI, SV_ON);
    vDrvSetUpScalerProgOnOff(SV_VP_MAIN, UPSCALER_VERT, SV_ON);

    // SCPIP_PQ Initialize
    vDrvUsPeiInit();
    vDrvDeRingInit();
    
#if defined(CC_MT5396)
    // Set UpScaler Coefficient from tbl
    vDrvSetPostUpScalerFilter(SV_VP_MAIN, UPSCALER_HORI, 0);
    // Enable SRAM Coefficient
    vDrvSetPostUpScalerProgOnOff(SV_VP_MAIN, UPSCALER_HORI, SV_ON);
    // SCPIP_PQ Initialize    
    vDrvPostDeRingInit();
#endif

}

//--------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------  FRONT UPSCALER ------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//

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
    UINT8 bSel;
    switch(bTap)
    {
        case UPSCALER_4TAP:
            bSel = 0;
        break;
        case UPSCALER_6TAP:
            bSel = 1;
        break;    
        case UPSCALER_8TAP:
            bSel = 2;
        break;    
        case UPSCALER_6PLUS_TAP:
            bSel = 3;
        break;
        default:
            bSel = 1;   //@6896 default up vtap is 6tap
        break;
    };

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
        }
        else
        {
            vRegWriteFldAlign(SCPIP_US1_04, bOnOff, US1_04_SRAM_COE_ENV_1);
        }
    }
    return;
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
    UINT16 bSramAddr;
    UINT16 u2Index;  

    if (bMode>ModeNum)
	{
	    bMode = 0;
	}

    vRegWriteFldAlign(SCPIP_US1_05, 0, US1_05_R_PIO_MODE);
    vRegWriteFldAlign(SCPIP_US1_05, 0, US1_05_R_FORCE_WR_NO_WAIT);   
    
    if (bType == UPSCALER_HORI)
    {
        // HORI_Y
        vRegWriteFldAlign(SCPIP_US1_06, 0, US1_06_COE_SEL);			
        vRegWriteFldAlign(SCPIP_US1_05, 1, US1_05_R_WR_AUTO_INC_ADDR);
        vRegWriteFldAlign(SCPIP_US1_0D, 1, US1_0D_R_CPURW_ACTIVE);	 
        vRegWriteFldAlign(SCPIP_US1_06, 0, US1_06_CPU_SRAM_ADDR);	

        for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)
        {
            u2Index = bSramAddr*bCoefLength+bHorYCoefTblOffset + bMode*bUPSCoefTotal;                 
            vRegWriteFldAlign(SCPIP_US1_09, u4FilterCoeff[u2Index + 0], US1_09_R_WDATA_02);
            vRegWriteFldAlign(SCPIP_US1_08, u4FilterCoeff[u2Index + 1], US1_08_R_WDATA_01);
            vRegWriteFldAlign(SCPIP_US1_07, u4FilterCoeff[u2Index + 2], US1_07_R_WDATA_00);
        }
        
        #if defined(CC_MT5396) || defined(CC_MT5368)
        // HORI_CbCr  
        vRegWriteFldAlign(SCPIP_US1_06, 1, US1_06_COE_SEL);	
        vRegWriteFldAlign(SCPIP_US1_05, 1, US1_05_R_WR_AUTO_INC_ADDR);	
        vRegWriteFldAlign(SCPIP_US1_0D, 1, US1_0D_R_CPURW_ACTIVE);	 
        vRegWriteFldAlign(SCPIP_US1_06, 0, US1_06_CPU_SRAM_ADDR);
        
        for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)
        {      
            u2Index = bSramAddr*bCoefLength+bHorCbCrCoefTblOffset + bMode*bUPSCoefTotal;     
            vRegWriteFldAlign(SCPIP_US1_09, u4FilterCoeff[u2Index + 0], US1_09_R_WDATA_02);
            vRegWriteFldAlign(SCPIP_US1_08, u4FilterCoeff[u2Index + 1], US1_08_R_WDATA_01);
            vRegWriteFldAlign(SCPIP_US1_07, u4FilterCoeff[u2Index + 2], US1_07_R_WDATA_00);                
        }
        #endif	    
    }
    else if (bType == UPSCALER_VERT)
    {
        // VER
        vRegWriteFldAlign(SCPIP_US1_06, 2, US1_06_COE_SEL);	
        vRegWriteFldAlign(SCPIP_US1_05, 1, US1_05_R_WR_AUTO_INC_ADDR);	
        vRegWriteFldAlign(SCPIP_US1_0D, 1, US1_0D_R_CPURW_ACTIVE);	 
        vRegWriteFldAlign(SCPIP_US1_06, 0, US1_06_CPU_SRAM_ADDR);
        
        for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)
        {
            u2Index = bSramAddr*bCoefLength+bVerCoefTblOffset + bMode*bUPSCoefTotal;  
            vRegWriteFldAlign(SCPIP_US1_09, u4FilterCoeff[u2Index + 0], US1_09_R_WDATA_02);
            vRegWriteFldAlign(SCPIP_US1_08, u4FilterCoeff[u2Index + 1], US1_08_R_WDATA_01);
            vRegWriteFldAlign(SCPIP_US1_07, u4FilterCoeff[u2Index + 2], US1_07_R_WDATA_00);                                
        }
    }

    vRegWriteFldAlign(SCPIP_US1_0D, 0, US1_0D_R_CPURW_ACTIVE);
    vRegWriteFldAlign(SCPIP_US1_05, 0, US1_05_R_WR_AUTO_INC_ADDR);	   
    vRegWriteFldAlign(SCPIP_US1_06, 0, US1_06_CPU_SRAM_ADDR);    
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
    UINT32 u4SramReadData1, u4SramReadData2, u4SramReadData3;
    UINT16 bSramAddr;
    UINT16 u2Index;   
    
    if (bMode>ModeNum)
	{
	    bMode = 0;
	}
    
    vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_ENH_1);
    vRegWriteFldAlign(SCPIP_US1_04, 0x0, US1_04_SRAM_COE_ENV_1);

    vRegWriteFldAlign(SCPIP_US1_05, 0, US1_05_R_PIO_MODE);
    vRegWriteFldAlign(SCPIP_US1_05, 0, US1_05_R_FORCE_WR_NO_WAIT);       

    if (bType == UPSCALER_HORI)
    {
        // HORI_Y
        vRegWriteFldAlign(SCPIP_US1_06, 0, US1_06_COE_SEL);				
        vRegWriteFldAlign(SCPIP_US1_05, 1, US1_05_R_RD_AUTO_INC_ADDR); 
        vRegWriteFldAlign(SCPIP_US1_0D, 1, US1_0D_R_CPURW_ACTIVE);	 
        vRegWriteFldAlign(SCPIP_US1_06, 0, US1_06_CPU_SRAM_ADDR);
        
        for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)                                                  
        {
            u2Index = bSramAddr*bCoefLength+bHorYCoefTblOffset + bMode*bUPSCoefTotal;            
            // Get Data
            u4SramReadData3 = RegReadFldAlign(SCPIP_US1_0A, US1_0A_R_RDATA_00);
            u4SramReadData2 = RegReadFldAlign(SCPIP_US1_0B, US1_0B_R_RDATA_01);
            u4SramReadData1 = RegReadFldAlign(SCPIP_US1_0C, US1_0C_R_RDATA_02);
            
            // Compare Data		                               
            if (u4SramReadData1 != u4FilterCoeff[u2Index + 0] ||
                u4SramReadData2 != u4FilterCoeff[u2Index + 1] ||
                u4SramReadData3 != u4FilterCoeff[u2Index + 2])
            {
                Printf("[H_Y][0] Read [%d]  Write [%d]\n", u4SramReadData1, u4FilterCoeff[u2Index + 0]);
                Printf("[H_Y][1] Read [%d]  Write [%d]\n", u4SramReadData2, u4FilterCoeff[u2Index + 1]);
                Printf("[H_Y][2] Read [%d]  Write [%d]\n", u4SramReadData3, u4FilterCoeff[u2Index + 2]);                    
                return FALSE;
            }
        }
        
        #if defined(CC_MT5396) || defined(CC_MT5368)	        
        vRegWriteFldAlign(SCPIP_US1_06, 1, US1_06_COE_SEL);				
        vRegWriteFldAlign(SCPIP_US1_05, 1, US1_05_R_RD_AUTO_INC_ADDR);
        vRegWriteFldAlign(SCPIP_US1_05, 0, US1_05_R_PIO_MODE);
        vRegWriteFldAlign(SCPIP_US1_05, 0, US1_05_R_FORCE_WR_NO_WAIT);        
        vRegWriteFldAlign(SCPIP_US1_0D, 1, US1_0D_R_CPURW_ACTIVE);	 
        vRegWriteFldAlign(SCPIP_US1_06, 0, US1_06_CPU_SRAM_ADDR);	              
        for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)
        {
            u2Index = bSramAddr*bCoefLength+bHorCbCrCoefTblOffset + bMode*bUPSCoefTotal;
            // Get Data
            u4SramReadData3 = RegReadFldAlign(SCPIP_US1_0A, US1_0A_R_RDATA_00);
            u4SramReadData2 = RegReadFldAlign(SCPIP_US1_0B, US1_0B_R_RDATA_01);
            u4SramReadData1 = RegReadFldAlign(SCPIP_US1_0C, US1_0C_R_RDATA_02);
            
            // Compare Data		                               
            if (u4SramReadData1 != u4FilterCoeff[u2Index + 0] ||
                u4SramReadData2 != u4FilterCoeff[u2Index + 1] ||
                u4SramReadData3 != u4FilterCoeff[u2Index + 2])
            {
                Printf("bSramAddr = %d\n", u2Index);            
                Printf("[H_C][0] Read [%d]  Write [%d]\n", u4SramReadData1, u4FilterCoeff[u2Index + 0]);
                Printf("[H_C][1] Read [%d]  Write [%d]\n", u4SramReadData2, u4FilterCoeff[u2Index + 1]);
                Printf("[H_C][2] Read [%d]  Write [%d]\n", u4SramReadData3, u4FilterCoeff[u2Index + 2]);                    
                return FALSE;
            }
        }
        #endif		
    }
    else if (bType == UPSCALER_VERT)
    {
        // VER
        vRegWriteFldAlign(SCPIP_US1_06, 2, US1_06_COE_SEL);				
        vRegWriteFldAlign(SCPIP_US1_05, 1, US1_05_R_RD_AUTO_INC_ADDR);
        vRegWriteFldAlign(SCPIP_US1_05, 0, US1_05_R_PIO_MODE);
        vRegWriteFldAlign(SCPIP_US1_05, 0, US1_05_R_FORCE_WR_NO_WAIT);        
        vRegWriteFldAlign(SCPIP_US1_0D, 1, US1_0D_R_CPURW_ACTIVE);	 
        vRegWriteFldAlign(SCPIP_US1_06, 0, US1_06_CPU_SRAM_ADDR);	 	            

        for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)
        {
            u2Index = bSramAddr*bCoefLength+bVerCoefTblOffset + bMode*bUPSCoefTotal; 
            // Get Data
            u4SramReadData3 = RegReadFldAlign(SCPIP_US1_0A, US1_0A_R_RDATA_00);
            u4SramReadData2 = RegReadFldAlign(SCPIP_US1_0B, US1_0B_R_RDATA_01);
            u4SramReadData1 = RegReadFldAlign(SCPIP_US1_0C, US1_0C_R_RDATA_02);
            
            // Compare Data		                               
            if (u4SramReadData1 != u4FilterCoeff[u2Index + 0] ||
                u4SramReadData2 != u4FilterCoeff[u2Index + 1] ||
                u4SramReadData3 != u4FilterCoeff[u2Index + 2])
            {
                Printf("bSramAddr = %d\n", u2Index);              
                Printf("[V][0] Read [%d]  Write [%d]\n", u4SramReadData1, u4FilterCoeff[u2Index + 0]);
                Printf("[V][1] Read [%d]  Write [%d]\n", u4SramReadData2, u4FilterCoeff[u2Index + 1]);
                Printf("[V][2] Read [%d]  Write [%d]\n", u4SramReadData3, u4FilterCoeff[u2Index + 2]);                    
                return FALSE;
            }
        }
    }
    else
    {
        return FALSE;
    }

    vRegWriteFldAlign(SCPIP_US1_0D, 0, US1_0D_R_CPURW_ACTIVE);            
    vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_SRAM_COE_ENH_1);
    vRegWriteFldAlign(SCPIP_US1_04, 0x1, US1_04_SRAM_COE_ENV_1);  
    
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
/*****************************    PureEdgeInterpolation   ********************************/
/*****************************************************************************************/
void vDrvUsPeiInit(void)
{
#if defined(CC_MT5396) || defined(CC_MT5368)
	vDrvLoadRegDwrdTbl(REGTBL_PEI);
	vDrvUsPeiOnOff(SV_ON);
	vRegWriteFldAlign(SCPIP_US1_10,wDrvVideoInputWidth(SV_VP_MAIN),US1_10_PURE_EDGE_SRC_WIDTH_H);
	vRegWriteFldAlign(SCPIP_US1_10,PANEL_GetPanelHeight(),US1_10_PURE_EDGE_SRC_WIDTH_V);
#endif
}

void vDrvUsPeiOnOff(UINT8 bOnOff)
{
#if defined(CC_MT5396) || defined(CC_MT5368)
	vRegWriteFldAlign(US1_PEI_13, bOnOff, US1_PEI_EN);
#endif
}

void vDrvUsPeiBypass(UINT8 bOnOff)
{
	vRegWriteFldAlign(SCPIP_US1_10, bOnOff, US1_10_BYPASS_PURE_EDGE);
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


#if defined(CC_MT5396) 
//--------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------  POST UPSCALER -----------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------
/**
 * @brief vDrvSetPostUpScalerFilterOnOff
 * @param bType bOnOff
 * @retval UINT32
 */
//-----------------------------------------------------------------------------
void vDrvSetPostUpScalerProgOnOff(UINT8 bPath, UINT8 bType, UINT8 bOnOff)
{
    if (bPath == SV_VP_MAIN)
    {
        if (bType == UPSCALER_HORI)
        {
            PSC_WRITE_FLD(PSUS1_04, bOnOff, PSUS1_04_SRAM_COE_ENH_1);
        }
        else
        {
            PSC_WRITE_FLD(PSUS1_04, bOnOff, PSUS1_04_SRAM_COE_ENV_1);
        }
    }
}

//-----------------------------------------------------------------------------
/**
 * @brief vDrvSetPostUpScalerFilter
 * @param bTap
 * @retval UINT32
 */
//-----------------------------------------------------------------------------
void vDrvSetPostUpScalerFilter(UINT8 bPath, UINT8 bType, UINT8 bMode)
{
    UINT16 bSramAddr;
    UINT16 HORI_Y_Index;
    if (bMode>=ModeNum)
	{
	    bMode = 0;
	}
    
    PSC_WRITE_FLD(PSUS1_04, 0x0, PSUS1_04_SRAM_COE_ENH_1);

    PSC_WRITE_FLD(PSUS1_05, 0, PSUS1_05_R_PIO_MODE);
    PSC_WRITE_FLD(PSUS1_05, 0, PSUS1_05_R_FORCE_WR_NO_WAIT);        
    if (bType == UPSCALER_HORI)
    	{
	    // HORI_Y
	    PSC_WRITE_FLD(PSUS1_06, 0, PSUS1_06_COE_SEL);			
	    PSC_WRITE_FLD(PSUS1_05, 1, PSUS1_05_R_WR_AUTO_INC_ADDR);
	    PSC_WRITE_FLD(PSUS1_0D, 1, PSUS1_0D_R_CPURW_ACTIVE);	 
	    PSC_WRITE_FLD(PSUS1_06, 0, PSUS1_06_CPU_SRAM_ADDR);	

	    for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)
	    {
	    HORI_Y_Index = bSramAddr*bCoefLength+bHorYCoefTblOffset + bMode*bUPSCoefTotal;                 
	    PSC_WRITE_FLD(PSUS1_09, u4FilterCoeff[HORI_Y_Index + 0], PSUS1_09_R_WDATA_02);
	    PSC_WRITE_FLD(PSUS1_08, u4FilterCoeff[HORI_Y_Index + 1], PSUS1_08_R_WDATA_01);
	    PSC_WRITE_FLD(PSUS1_07, u4FilterCoeff[HORI_Y_Index + 2], PSUS1_07_R_WDATA_00);
	    }
    	}

    PSC_WRITE_FLD(PSUS1_0D, 0, PSUS1_0D_R_CPURW_ACTIVE);
    PSC_WRITE_FLD(PSUS1_05, 0, PSUS1_05_R_WR_AUTO_INC_ADDR);	   
    PSC_WRITE_FLD(PSUS1_06, 0, PSUS1_06_CPU_SRAM_ADDR);    
    PSC_WRITE_FLD(PSUS1_04, 0x1, PSUS1_04_SRAM_COE_ENH_1);
}

//-----------------------------------------------------------------------------
/**
 * @brief vDrvCheckPostUpScalerFilter

 * @param bTap
 * @retval UINT8
 */
//-----------------------------------------------------------------------------
UINT8 vDrvCheckPostUpScalerFilter(UINT8 bPath, UINT8 bType, UINT8 bMode)
{
    UINT32 u4SramReadData1, u4SramReadData2, u4SramReadData3;
    UINT16 bSramAddr;
    UINT16 HORI_Y_Index;
    
    if (bMode>=ModeNum)
	{
	    bMode = 0;
	}

    PSC_WRITE_FLD(PSUS1_04, 0x0, PSUS1_04_SRAM_COE_ENH_1);
    PSC_WRITE_FLD(PSUS1_05, 0, PSUS1_05_R_PIO_MODE);
    PSC_WRITE_FLD(PSUS1_05, 0, PSUS1_05_R_FORCE_WR_NO_WAIT);       

    if (bType == UPSCALER_HORI)
    	{
	    // HORI_Y
	    PSC_WRITE_FLD(PSUS1_06, 0, PSUS1_06_COE_SEL);				
	    PSC_WRITE_FLD(PSUS1_05, 1, PSUS1_05_R_RD_AUTO_INC_ADDR); 
	    PSC_WRITE_FLD(PSUS1_0D, 1, PSUS1_0D_R_CPURW_ACTIVE);	 
	    PSC_WRITE_FLD(PSUS1_06, 0, PSUS1_06_CPU_SRAM_ADDR);
	    for (bSramAddr = 0; bSramAddr < bPhaseNum; bSramAddr++)                                                  
	    {
		    HORI_Y_Index = bSramAddr*bCoefLength+bHorYCoefTblOffset + bMode*bUPSCoefTotal;            
		    // Get Data
		    u4SramReadData3 = PSC_READ_FLD(PSUS1_0A, PSUS1_0A_R_RDATA_00);
		    u4SramReadData2 = PSC_READ_FLD(PSUS1_0B, PSUS1_0B_R_RDATA_01);
		    u4SramReadData1 = PSC_READ_FLD(PSUS1_0C, PSUS1_0C_R_RDATA_02);
		    // Compare Data		                               
		    if (u4SramReadData1 != u4FilterCoeff[HORI_Y_Index + 0] ||
		        u4SramReadData2 != u4FilterCoeff[HORI_Y_Index + 1] ||
		        u4SramReadData3 != u4FilterCoeff[HORI_Y_Index + 2])
		    {
			Printf("[H_Y][0] Read [%d]  Write [%d]\n", u4SramReadData1, u4FilterCoeff[HORI_Y_Index + 0]);
			Printf("[H_Y][1] Read [%d]  Write [%d]\n", u4SramReadData2, u4FilterCoeff[HORI_Y_Index + 1]);
			Printf("[H_Y][2] Read [%d]  Write [%d]\n", u4SramReadData3, u4FilterCoeff[HORI_Y_Index + 2]);                    
			return FALSE;
		    }

	    }	     
    	}
    else
    	{
    		return FALSE;
	}  
    PSC_WRITE_FLD(PSUS1_0D, 0, PSUS1_0D_R_CPURW_ACTIVE);            
    PSC_WRITE_FLD(PSUS1_04, 0x1, PSUS1_04_SRAM_COE_ENH_1);
    return TRUE;
}

//-----------------------------------------------------------------------------
/**
 * @brief vDrvTestPostUpScalerFilter

 * @param void
 * @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL vDrvTestPostUpScalerFilter(void)
{
    vDrvSetPostUpScalerFilter(SV_VP_MAIN, UPSCALER_HORI, 0);
    if (vDrvCheckPostUpScalerFilter(SV_VP_MAIN, UPSCALER_HORI, 0) == FALSE)
    {
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************************/
/*********************************   Post Dering   *******************************************/
/*****************************************************************************************/

void vDrvPostDeRingInit(void)
{
	// HOR_Y
   	PSC_WRITE_FLD(PSUS1_02, 0x01, PSUS1_02_DE_RING_EN_H_1);        
   	PSC_WRITE_FLD(PSUS1_02, 0x03, PSUS1_02_DE_RING_SEL_H_1);
   	PSC_WRITE_FLD(PSUS1_02, 0x03, PSUS1_02_DE_RING_SLOPE_H_1);
   	PSC_WRITE_FLD(PSUS1_02, 0x10, PSUS1_02_DE_RING_MAX_THRESHOLD_H_1);
   	PSC_WRITE_FLD(PSUS1_02, 0x03, PSUS1_02_DE_RING_FIRST_SLOPE_H_1);   	    	    	    	 
   	PSC_WRITE_FLD(PSUS1_02, 0x00, PSUS1_02_DE_RING_FIRST_THRESHOLD_H_1);      	 
   	// HOR_UV
   	PSC_WRITE_FLD(PSUS1_1E, 0x20, PSUS1_1E_RING_THR_H_UV);           	 
   	PSC_WRITE_FLD(PSUS1_1E, 0x06, PSUS1_1E_SLOPE_SEL_H_UV);      
   	PSC_WRITE_FLD(PSUS1_1E, 0x00, PSUS1_1E_RING_1ST_THR_H_UV);                                                                       
   	PSC_WRITE_FLD(PSUS1_1E, 0x03, PSUS1_1E_SLOPE_1ST_SEL_H_UV); 
}

void vDrvPostDeRingOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bPath == SV_VP_MAIN)
    {
        PSC_WRITE_FLD(PSUS1_02, bOnOff, PSUS1_02_DE_RING_EN_H_1);
    }
}
#endif

