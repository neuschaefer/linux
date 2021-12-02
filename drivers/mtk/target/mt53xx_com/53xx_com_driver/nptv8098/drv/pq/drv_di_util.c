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
#include "hw_ckgen.h"
#include "hw_ycproc.h"
#include "hw_scpos.h"
#include "hw_sys.h"
#include "vdo_misc.h"
#include "drv_display.h"
#include "drv_tdnavi.h"
#include "source_select.h"
#include "hw_scpos.h"
#include "panel.h"

UINT32 u4FlagStaCnt[VDP_MAX_NS][E_PSCAN_STA_NUM];

////////////////////////////////////////////////////////////////////////////////
//MDDI Debug Register Lor
////////////////////////////////////////////////////////////////////////////////

#ifdef LOG_MDDI_REG

#define DI_LOG_NUM 1024
#define DI_LOG_MSK (DI_LOG_NUM-1)
MDDI_REGTBL_T MDDI_REG_LOG[DI_LOG_NUM]; 
UINT32 u4LogReg = 0;

/**
 * @brief DI Write Field Macro
 * @param u1VdpId VDP_1/VDP_2
 * @param u2Addr Address
 * @param u4Value Value
 * @param u4Fld Field
 */
void MDDI_WRITE_FLD(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Fld)
{
    vRegWriteFldAlign((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Fld);
    //Avoid interupt clear bit access
    if (u2Addr == 0x402C) return;
    MDDI_REG_LOG[u4LogReg].u2Addr = (u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM))|0x1;
    MDDI_REG_LOG[u4LogReg].u4Value = u4Value;
    MDDI_REG_LOG[u4LogReg].u4Mask = u4Fld;
    u4LogReg = (u4LogReg+1)&DI_LOG_MSK;
}

/**
 * @brief DI Write Mask Macro
 * @param u1VdpId VDP_1/VDP_2
 * @param u2Addr Address
 * @param u4Value Value
 * @param u4Msk Mask
 */
void MDDI_WRITE32_MSK(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Msk) 
{
    vRegWrite4BMsk((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Msk);
    MDDI_REG_LOG[u4LogReg].u2Addr = (u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM));
    MDDI_REG_LOG[u4LogReg].u4Value = u4Value;
    MDDI_REG_LOG[u4LogReg].u4Mask = u4Msk;
    u4LogReg = (u4LogReg+1)&DI_LOG_MSK;
}

/**
 * @brief DI Print Register Log
 */
void MDDI_PRINT_REG_LOG(void)
{
    UINT16 i, index, j = u4LogReg;

    Printf("\nIndex    u2Addr         u4Value        u4Mask\n");    

    for (i = j; i < (1024 + j); i++)
    {
        index = i&DI_LOG_MSK;
        if ((index&0xFF) == 0xFF)
        {
            x_thread_delay(20);
        }
        Printf("%4d    %6x        %8x        %08x\n", index,
                MDDI_REG_LOG[index].u2Addr&0xFFFC, MDDI_REG_LOG[index].u4Value,
                (MDDI_REG_LOG[u4LogReg].u2Addr & 0x1) 
                ? (Fld2Msk32(MDDI_REG_LOG[index].u4Mask))
                : MDDI_REG_LOG[index].u4Mask);
    }
}
#endif

/**
 * @brief DI Load Register Table
 * @param u1VdpId VDP_1/VDP_2
 * @param prRegTbl Register Table
 */
void vDrvDILoadRegTbl(UINT8 u1VdpId, const MDDI_REGTBL_T * prRegTbl)
{
    UNUSED(u1VdpId);
    
    while (prRegTbl->u2Addr != MDDI_REGTBL_END)
    {
        vRegWrite4BMsk(prRegTbl->u2Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
        prRegTbl++;
    }
}

/**
* @brief Check DI status
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIChkStatus(UINT8 u1VdpId)
{   
    if (_arMDDiPrm[u1VdpId].u1StatusCnt == DI_STATUS_CNT)
    {
        _arMDDiPrm[u1VdpId].u1StatusCnt = 0;    
  
        if(MDDI_READ_FLD(u1VdpId, MCVP_KC_17, ABCD_YC_EN))  	
        {
            if (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_06, STA_OVERFLOW))
            {
                u4FlagStaCnt[u1VdpId][E_STA_PXL_OVERFLOW]++;
            }
        
            if (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_06, STA_UNDERFLOW))
            {
                u4FlagStaCnt[u1VdpId][E_STA_PXL_UNDERFLOW]++;
            }
        }    

        if (u1VdpId == VDP_1)
        {        
            if (MDDI_READ_FLD(u1VdpId,MCVP_DRAM_18, DA_SC_READ_ENABLE) && MDDI_READ_FLD(u1VdpId, MCVP_DRAM_1A, STA_SC_UNDERFLOW))
            {
                u4FlagStaCnt[u1VdpId][E_STA_SC_UNDERFLOW]++;
            }

            if ((MDDI_READ_FLD(u1VdpId, MCVP_MULTI_00, EN_DRAM_RW) & E_NR_W) && MDDI_READ_FLD(u1VdpId, MCVP_DRAM_1A, STA_NR_OVERFLOW))
            {
                u4FlagStaCnt[u1VdpId][E_STA_NR_OVERFLOW]++;
            }	

            if (MDDI_READ_FLD(u1VdpId, MCVP_MULTI_00, EN_DRAM_RW) & (E_MV_W | E_MV_R | E_LM_W | E_LM_R))
            {
	            if (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_05, STA_FLAG_OVERFLOW))
	            {
	                u4FlagStaCnt[u1VdpId][E_STA_FLG_OVERFLOW]++;
	            }  
	            
	            if (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_05, STA_FLAG_UNDERFLOW))
	            {
	                u4FlagStaCnt[u1VdpId][E_STA_FLG_UNDERFLOW]++;
	            }
            }
        }
        vDrvDIClearStatus(u1VdpId, SV_ON);
        vDrvDIClearStatus(u1VdpId, SV_OFF);
    }

    _arMDDiPrm[u1VdpId].u1StatusCnt++;
    
}


/**
 * @brief Get DI Buffer Address
 * @param u1VdpId VDP_1/VDP_2
 * @param u4AddrStart start address
 * @param u4AddrEnd end address
 */
void vDrvDIGetBufAddr(UINT8 u1VdpId, UINT32 *u4AddrStart, UINT32 *u4AddrEnd)
{
    *u4AddrStart = (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_0F, DA_WADDR_LO_LIMIT) << 11);

    if (u1VdpId == VDP_1) // Sub path no flag dram access
    {
        *u4AddrEnd = (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_03, DA_FLAG_WADDR_HI_LIMIT) << 4);
    }
    else
    {
        *u4AddrEnd = (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_0E, DA_WADDR_HI_LIMIT) << 11);
    }    
}

/**
 * @brief Set DI On/Off
 * @param u1VdpId VDP_1/VDP_2
 * @param bOnOff On/Off
 */
void vDrvDIOnOff(UINT8 u1VdpId, UINT8 bOnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, !bOnOff, BYPASS_EN);
}

/**
 * @brief Set DI Color Mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1ColorMode Color Format 0: normal , 1: Force 420 Mode;
 */
void vDrvDISetColorMode(UINT8 u1VdpId, UINT8 u1ColorMode)
{
    _arMDDiPrm[u1VdpId].u1Force420Mode=u1ColorMode;
}

/**
 * @brief Set DI Field
 * @param u1VdpId VDP_1/VDP_2
 * @param u1Inv Normal/Inverse
 */
void vDrvDISetFieldInverse(UINT8 u1VdpId, UINT8 u1Inv)
{
    UINT8 u1Mode = MDDI_READ_FLD(u1VdpId, MCVP_DRAM_06, DA_DISPLAY_MODE);    
    if (IS_FLIP_MIRROR(u1VdpId) && IS_INTERLACE(u1VdpId))
    {
        if ((u1Mode == DI_FLIP_ON_MODE) || (u1Mode == DI_FLIP_MIRROR_ON_MODE))
        {
            u1Inv = !u1Inv;
        }   
    }

    if(vDrvTDIsFldInv(u1VdpId)==SV_ON)
    {
        u1Inv = !u1Inv;
    }

    if(IS_PROGRESSIVE(u1VdpId))
    {
        u1Inv=0;
    }
    
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, u1Inv, FLD_IN_P);
}

/**
 * @brief Set DI write bit resolution
 * @param u1VdpId VDP_1/VDP_2
 * @param b8bit 1: 8bit/0: 10bit
 */
void vDrvDISetWritePrecision(UINT8 u1VdpId, UINT8 b8bit)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_06, b8bit, DA_BIT_SEL_WY);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_06, b8bit, DA_BIT_SEL_WU);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_06, b8bit, DA_BIT_SEL_WV);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, b8bit, DA_SC_BIT_SEL_RY);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, b8bit, DA_SC_BIT_SEL_RU);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, b8bit, DA_SC_BIT_SEL_RV);
}



/**
 * @brief Set DI Output Pattern
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 */
void vDrvDISetPattern(UINT8 u1VdpId, UINT8 u1OnOff)
{
    UINT8 u1Val = ((u1OnOff == 1) ? 0x7 : 0x0);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u1Val, PAT_GEN_MODE_Y);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u1Val, PAT_GEN_MODE_CB);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u1Val, PAT_GEN_MODE_CR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u1OnOff, BORDER_EN);
}

/**
 * @brief Set DI Flip-Mirror
 * @param u1VdpId VDP_1/VDP_2
 * @param u1Mode Flip-Mirror Mode
 */
void vDrvDISetFlipMirror(UINT8 u1VdpId, UINT8 u1Mode)
{    
    UINT16 u2Height = MDDI_READ_FLD(u1VdpId, MCVP_KC_0A, VDEW);     
    UINT8 u1FrameRate = bDrvVideoGetRefreshRate(u1VdpId);
    
    u2Height = IS_INTERLACE(u1VdpId) ? u2Height>>1 : u2Height;

    if (u1VdpId >= VDP_NS)
    {
        u1VdpId = VDP_1;
    }
    
    if (IS_INTERLACE(u1VdpId))
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_06, u1Mode, DA_DISPLAY_MODE); //420            
        MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, DI_NORMAL_MODE, DA_SC_DISPLAY_MODE);
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_06, DI_NORMAL_MODE, DA_DISPLAY_MODE);
        MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, u1Mode, DA_SC_DISPLAY_MODE); //422 & 444
    }


    // Adjest FRC Line for Flip
    if ((u1Mode == DI_FLIP_ON_MODE) || (u1Mode == DI_FLIP_MIRROR_ON_MODE))
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, u2Height, FRC_LINE);    
    }
    else
    {
        if((60==u1FrameRate)||(50==u1FrameRate))
        {   //Input Frame Rate = output Frame Rate, Read Pointer will never faster than write pointer
            //FRC Line can be smaller
            MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, u2Height*1/4, FRC_LINE);//Default Value
        }
        else
        {
            MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, u2Height*3/4, FRC_LINE);//Default Value
        }    
    }    

    vDrvDISetFieldInverse(u1VdpId, SV_OFF);
}

/**
 * @brief Set Force Flip-Mirror
 * @param u1VdpId VDP_1/VDP_2
 * @param u1Mode Flip-Mirror Mode
 */
void vDrvDISetDbgFlipMirror(UINT8 u1VdpId, UINT8 u1Mode)
{   
    if(u1Mode != DI_FLIP_MIRROR_UNKOWN)
    {           
        _arMDDiPrm[u1VdpId].u1DebugFlipMorrorByMIB=SV_ON;    

        u1FlipMirrorConfig = u1Mode; 
    }
    else
    {
        _arMDDiPrm[u1VdpId].u1DebugFlipMorrorByMIB=SV_OFF;    
    }
}


/**
 * @brief Set DI ROI Pixel
 * @param u1VdpId VDP_1/VDP_2
 * @param u4RoiPixel pixel value
 */
void vDrvDISetROIPixel(UINT8 u1VdpId, UINT32 u4RoiPixel)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_FUSION_0E, u4RoiPixel, IF_ROI_PIXELS);
    MDDI_WRITE_FLD(u1VdpId, MCVP_FUSION_0E, 0x0, IF_ROI_V);
    MDDI_WRITE_FLD(u1VdpId, MCVP_FUSION_0E, 0x0, IF_ROI_H);
}

/**
 * @brief Init DI CRC
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDIInitCRC(UINT8 u1VdpId)
{
    /* PSCAN CRC */
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x1, SRCRW_CRC_BOT_L_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x1, SRCRW_CRC_TOP_L_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x0, SRCRW_CRC_BOT_R_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x0, SRCRW_CRC_TOP_R_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_11, 0x3ff, SRCW_CRC_Y_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_11, 0x3ff, SRCW_CRC_C_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_11, 0x0, SRCW_CRC_V_MASK);    
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_13, 0x0, SRCW_CRC_CLIP_H_START);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_13, _arMDDiPrm[u1VdpId].u2Width, SRCW_CRC_CLIP_H_END);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_12, 0x0, SRCW_CRC_CLIP_V_START);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_12, (_arMDDiPrm[u1VdpId].u2Height/2), SRCW_CRC_CLIP_V_END);

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x3ff, SRCR_CRC_Y_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x3ff, SRCR_CRC_C_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x0, SRCR_CRC_SEL_FIELD);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x1, SRCR_CRC_AUTO_FIELD_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1C, 0x0, SRCR_CRC_CLIP_H_START);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1C, _arMDDiPrm[u1VdpId].u2Width, SRCR_CRC_CLIP_H_END);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1B, 0x0, SRCR_CRC_CLIP_V_START);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1B, (_arMDDiPrm[u1VdpId].u2Height/2), SRCR_CRC_CLIP_V_END);

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_24, 0x3ff, OUT_CRC_Y_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_24, 0x3ff, OUT_CRC_C_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_24, 0x0, OUT_CRC_V_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_26, 0x1, OUT_CRC_L_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_26, 0x1, OUT_CRC_R_EN);
    //MDDI_WRITE_FLD(u1VdpId, MCVP_KC_26, 0x0, OUT_CRC_R_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_26, 0x0, OUT_CRC_CLIP_H_START);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_26, _arMDDiPrm[u1VdpId].u2Width, OUT_CRC_CLIP_H_END);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_25, 0x0, OUT_CRC_CLIP_V_START);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_25, _arMDDiPrm[u1VdpId].u2Height, OUT_CRC_CLIP_V_END);

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
    vRegWriteFldAlign(TOP_MAIN_00, 0x1, C_CRC_SEL); /* 0: main_in, 1: main_out, 2: pip_in, 3: pip_out */
    vRegWriteFldAlign(CRC_00, 0x3FF, C_Y_IN_MASK_MAIN);
    vRegWriteFldAlign(CRC_00, 0x3FF, C_C_IN_MASK_MAIN);
    vRegWriteFldAlign(CRC_00, 0x3FF, C_V_IN_MASK_MAIN);
    vRegWriteFldAlign(CRC_00, 0x1, C_TOP_L_EN_MAIN);
    vRegWriteFldAlign(CRC_00, 0x1, C_BOT_L_EN_MAIN);
    vRegWriteFldAlign(CRC_01, 0x1, C_TOP_R_EN_MAIN);
    vRegWriteFldAlign(CRC_01, 0x1, C_BOT_R_EN_MAIN);
}

/**
 * @brief Init DI Input CRC
 * @param u1VdpId VDP_1/VDP_2
 */
UINT32 u4DrvDIGetInputCRC(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MCVP_KC_15, SRCW_CRC_OUT_V);
}

/**
 * @brief Init DI Output CRC
 * @param u1VdpId VDP_1/VDP_2
 */
UINT32 u4DrvDIGetOutputCRC(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MCVP_KC_28, OUT_CRC_OUT_V);
}

/**
 * @brief Set DI ISR Enable
 * @param u1Enable On/Off
 */
void vDrvDIEnableISR(UINT8 u1VdpId, UINT8 u1Enable)
{
    if (u1VdpId == VDP_1)
    {

        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_MIVS_R_EN);
#ifdef PSCAN_AUTO_VERIFY                
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_MIVS_F_EN);
#endif        
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_MOVS_R_EN);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_MOVS_F_EN);        
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_MO_ADJ_EN);
    }
    else
    {

        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_PIVS_R_EN);
        
#ifdef CC_SUPPORT_VENC                        
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_PIVS_F_EN);
#endif
        //MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_POVS_R_EN);
#ifdef PSCAN_AUTO_VERIFY                
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_POVS_F_EN);
#endif


    }
}

/**
 * @brief Set DI Buffer Address
 * @param u1VdpId VDP_1/VDP_2
 * @param u4AddrYMsb/u4AddrYLsb/u4AddrUMsb/u4AddrULsb/u4AddrVMsb/u4AddrVLsb/u4AddrMVFlg/u4AddrLMFlg
 */
void vDrvDISetBufferAddr(UINT8 u1VdpId, UINT32 u4AddrYMsb, UINT32 u4AddrYLsb, UINT32 u4AddrUMsb, 
        UINT32 u4AddrULsb, UINT32 u4AddrVMsb, UINT32 u4AddrVLsb, UINT32 u4AddrMVFlg, UINT32 u4AddrLMFlg)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_0A, u4AddrYMsb, DA_ADDR_BASE_MSB_Y);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_0B, u4AddrYLsb, DA_ADDR_BASE_LSB_Y);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_0C, u4AddrUMsb, DA_ADDR_BASE_MSB_U);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_0D, u4AddrULsb, DA_ADDR_BASE_LSB_U);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_0E, u4AddrVMsb, DA_ADDR_BASE_MSB_V);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_0F, u4AddrVLsb, DA_ADDR_BASE_LSB_V);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_02, u4AddrMVFlg, DA_FLAG_MV_ADDR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_03, u4AddrLMFlg, DA_FLAG_LM_ADDR);
}

/**
 * @brief Set DI Buffer Protect
 * @param u1VdpId VDP_1/VDP_2
 * @param u4Lo/u4Hi/u4FlgLo/u4FlgHi limit
 */
void vDrvDISetBufferProtect(UINT8 u1VdpId, UINT32 u4Lo, UINT32 u4Hi, UINT32 u4FlgLo, UINT32 u4FlgHi)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_10, u4Hi, DA_WADDR_HI_LIMIT);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_11, u4Lo, DA_WADDR_LO_LIMIT);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_05, u4FlgHi, DA_FLAG_WADDR_HI_LIMIT);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_06, u4FlgLo, DA_FLAG_WADDR_LO_LIMIT);    
}

/**
 * @brief Set DI Frame Number
 * @param u1VdpId VDP_1/VDP_2
 * @param u1YNum/u1UNum/u1VNum
 */
void vDrvDISetFrmNum(UINT8 u1VdpId, UINT8 u1YNum, UINT8 u1UNum, UINT8 u1VNum)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_06, u1YNum, DA_FRAME_NUM_Y);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_06, u1UNum, DA_FRAME_NUM_U);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_06, u1VNum, DA_FRAME_NUM_V);	
}

/**
 * @brief Set DI Double Buffer
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 */
void vDrvDISetDoubleBuffer(UINT8 u1VdpId, UINT8 u1OnOff)
{
    if (u1VdpId == VDP_1)
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MAIN_RD_LOCAL_VP);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MAIN_WR_LOCAL_VP);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, PIP_RD_LOCAL_VP);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, PIP_WR_LOCAL_VP);
    }
}

/**
 * @brief Set DI Clock Reset
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 */
void vDrvDISetClkReset(UINT8 u1VdpId, UINT8 u1OnOff)
{
    /* Note */
    /* Please follow the sequence when doing reset. */
    /* If the reset sequence is not correct, prefetch error may occur. */

    if (u1VdpId == VDP_1)
    {    
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, DRAM_M_CK_RST);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MIB_IN_M_CK_RST);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MCVP_K_M_CK_RST);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, DRAM_P_CK_RST);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MIB_IN_P_CK_RST);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MCVP_K_P_CK_RST);
    }
   
}

/**
 * @brief Set DI FRC line
 * @param u1VdpId VDP_1/VDP_2
 * @param u1FrcLine
 */
void vDrvDISetFrcLine(UINT8 u1VdpId, UINT8 u1FrcLine)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, u1FrcLine, FRC_LINE);
}

/**
 * @brief Set DI standard vmask
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 */
void vDrvDISetStdVMask(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u1OnOff, NON_STD_VMASK_EN);
}

/**
 * @brief Set DI auto stop
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 */
void vDrvDISetAutoStop(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, IS_INTERLACE(u1VdpId) ? u1OnOff : SV_OFF, SRCW_AUTO_STOP);
}

/**
 * @brief Set DI Clock Enable
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 */
void vDrvDISetClockEnable(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, DRAM_CK_EN);
    MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, (u1VdpId == VDP_1) ? MCVP_K_M_CK_EN : MCVP_K_P_CK_EN);
    MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MCVP_K_M_PQ_CK_EN);
}

/**
 * @brief Set pscan output clock
 * @param bPath VDP_1/VDP_2
 * @param eType clock type
 */
void _vDrvDISetClock(UINT8 u1VdpId, HW_MIB_OUTPUT_CLK_TYPE eType)
{
    vIO32WriteFldAlign(CKGEN_MIB_OCLK_CKCFG, eType, FLD_MIB_OCLK_CK_SEL);
}

/**
 * @brief Set DI Error Defusion
 * @param u1VdpId VDP_1/VDP_2
 * @param u18bit 1: 8bit/ 0: 10bit
 */
void vDrvDISetErrDefusion(UINT8 u1VdpId, UINT8 u18bit)
{
    //Always turn off because of performance issue
    u18bit =SV_OFF;
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, u18bit, ERR_DFU_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, u18bit, ERR_DFU_RAND_INI_EN);    
}

/**
 * @brief Set bit resolution
 * @param u1VdpId VDP_1/VDP_2
 * @param u1BitRes Bit resolution (8 or 10)
 */

void vDrvDISetBitRes(UINT8 u1VdpId, UINT8 u18bit)
{
    vDrvDISetWritePrecision(u1VdpId, u18bit);
    vDrvDISetErrDefusion(u1VdpId, u18bit);

    if (u1VdpId == VDP_1)
        vDrvNRSetBitRes(u18bit);
}

/**
 * @brief Set DI DISPMODE_H_BLANK
 * @param u1VdpId VDP_1/VDP_2
 * @param u2Value set the value to DISPMODE_H_BLANK
 */
void vDrvDiSetDispmodeHBlank(UINT8 u1VdpId, UINT16 u2Value)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2B, u2Value, DISPMODE_H_BLANK); 
}

UINT32 u4DrvDiGetDispHBlank(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MCVP_KC_2B, DISPMODE_H_BLANK); 
}

#ifdef DYNAMIC_RES_VERIFY
void vSetDiDynRes(UINT8 fgOnOff)
{
    MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, fgOnOff, PIP_RD_SET_RES_VP);
    MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, fgOnOff, PIP_WR_SET_RES_VP);
    MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, fgOnOff, MAIN_RD_SET_RES_VP);
    MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, fgOnOff, MAIN_WR_SET_RES_VP);

    MDDI_WRITE_FLD(VDP_1, MCVP_KC_1E, !fgOnOff, NON_STD_VMASK_EN);
    if (!IS_MIB_FRC(VDP_1))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_33, fgOnOff, RSV_DYNRES_M_INSYNC);
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_33, fgOnOff, RSV_DYNRES_P_INSYNC);
    }
    u1EnDynRes = fgOnOff;
}

void vDiForceClipEn(UINT8 fgOnOff)
{
    u1ForceClipEn = fgOnOff;
    MDDI_WRITE_FLD(VDP_1, MCVP_KC_09, SV_ON, IPTG_CLIP_EN);
}
#endif

//**********prelen**************************/

#if defined(CC_MT5389)

inline UINT16 _u2Get60HzPrelen(UINT8 u1VdpID,UINT32 u4HDEW,UINT32 u4VDEW)
{

        // type A
	    if (u4VDEW >= 1080)
	    {
	    	if (u4HDEW > 1592)
	    	{
	    	    return 41;
	    	}
	    	else
	    	{
	    	    return 25;
	    	}
	    }
	     // type B
	    else if ((u4VDEW <= 1078 && u4VDEW > 1072) ||
                 (u4VDEW <= 952 && u4VDEW > 872)   ||
                 (u4VDEW <= 836 && u4VDEW > 796))
	    {
	    	if (u4HDEW > 1868)
	    	{
	    	    return 31;
	    	}
	    	else if (u4HDEW <= 1868 && u4HDEW > 1512)
	    	{
	    	    return 25;
	    	}
	    	else
	    	{
	    	    return 41;
	    	}
	    }
	    // type C
	    else if (u4VDEW <= 1072 && u4VDEW > 968)
	    {
	    	if (u4HDEW > 1724)
	    	{
	    	    return 40;
	    	}
	    	else
	    	{
	    	    return 22;
	    	}
	    }
	    // type D
	    else if ((u4VDEW <= 968 && u4VDEW > 952))
	    {
	    	if (u4HDEW > 1872)
	    	{
	    	    return 40;
	    	}
	    	else
	    	{
	    	    return 33;
	    	}
	    }
	    // type E
	    else if (u4VDEW <= 872 && u4VDEW > 836)
	    {
	    	if (u4HDEW > 1836)
	    	{
	    	    return 22;
	    	}
	    	else if (u4HDEW > 1836 && u4HDEW > 1728)
	    	{
	    	    return 38;
	        }	
	    	else
	    	{
	    	    return 29;
	    	}
	    }
	    // type F
	    else if (u4VDEW <= 852 && u4VDEW > 836)
	    {
	    	if (u4HDEW > 1860)
	    	{
	    	    return 42;
	    	}
	    	else if (u4HDEW <= 1860 && u4HDEW > 1640)
	    	{
	    	    return 48;
	    	}
	    	else if (u4HDEW <= 1640 && u4HDEW > 1520)
	    	{
	    	    return 26;
	    	}
	    	else
	    	{
	    	    return 44;
	    	}
	    }
	    // type G
	    else if (u4VDEW <= 734 && u4VDEW > 704)
	    {
	    	if (u4HDEW > 1836)
	    	{
	    	    return 35;
	    	}
	    	else if (u4HDEW <= 1836 && u4HDEW > 1636)
	    	{
	    	    return 49;
	    	}
	    	else if (u4HDEW <= 1636 && u4HDEW > 1596)
	    	{
	    	    return 20;
	    	}
	    	else
	    	{
	    	    return 41;
	    	}
	    }
	    // type H
	    else if ((u4VDEW <= 702 && u4VDEW > 684) ||
	    	     (u4VDEW <= 646 && u4VDEW > 598) ||
	    	     (u4VDEW <= 578 && u4VDEW > 558))
	    {
	    	if (u4HDEW > 1584)
	    	{
	    	    return 39;
	    	}
	    	else
	    	{
	    	    return 24;
	    	}    
	    }
	    // type I
	    else if (u4VDEW == 704 || (u4VDEW <= 684 && u4VDEW > 646))
	    {
	    	if (u4HDEW > 1708)
	    	{
	    	    return 22;
	    	}
	    	else if (u4HDEW <= 1708 && u4HDEW > 1608)
	    	{
	    	    return 46;
	    	}
	    	else
	    	{
	    	    return 36;
	    	}    
	    }//////////////
	    // type J
	    else if (u4VDEW <= 796 && u4VDEW > 770)
	    {
	    	if (u4HDEW <= 1820 && u4HDEW > 1756)
	    	{
	    	    return 36;
	    	}
	    	else
	    	{
	    	    return 21;
	    	}    
	    }
	    // type K => no K
	    // type L
	    else if ((u4VDEW <= 598 && u4VDEW > 578) ||
	    	     (u4VDEW <= 558 && u4VDEW > 520) || 
	    	     (u4VDEW <= 502))
	    {
	    	if (u4HDEW <= 1864 || (u4HDEW <= 1808 && u4HDEW > 1476))
	    	{
	    	    return 27;
	    	}
	    	else
	    	{
	    	    return 48;
	    	}    
	    }
	    // type M
	    else if ((u4VDEW <= 598 && u4VDEW > 578) ||
	    	     (u4VDEW <= 558 && u4VDEW > 520) || 
	    	     (u4VDEW <= 502))
	    {
	    	if (u4HDEW >= 1768 || (u4HDEW <= 1700 && u4HDEW > 1660) ||
	    		u4HDEW <= 1516)
	    	{
	    	    return 42;
	    	}
	    	else
	    	{
	    	    return 24;
	    	}    
	    }
	    
	    
	    return 0x15;
	    
}

inline UINT16 _u2Get50HzPrelen(UINT8 u1VdpID,UINT32 u4HDEW,UINT32 u4VDEW)
{
        // type B
	    if (u4VDEW >= 1080)
	    {
	    	if ( (u4HDEW <= 1880 && u4HDEW > 1840) ||
	    		 (u4HDEW <= 1680 && u4HDEW > 1572) 
	    		)
	    	{
	    	    return 44;
	    	}
	    	else
	    	{
	    	    return 24;
	    	}
	    }
	     // type A  ///////
	    else if ((u4VDEW <= 1078 && u4VDEW > 1072) ||
	    	     (u4VDEW <= 952 && u4VDEW > 872)   ||
	    	     (u4VDEW <= 836 && u4VDEW > 796) 
	    	     )
	    {
	    	if ((u4HDEW <= 1600 && u4HDEW > 1536))
	    	{
	    	    return 40;
	    	}
	    	else
	    	{
	    	    return 22;
	    	}
	    }
	    // type C
	    else if (u4VDEW <= 1072 && u4VDEW > 966)
	    {
	        if (u4HDEW <= 1848 && u4HDEW > 1696)
	    	{
	    	    return 38;
	    	}
	    	else
	    	{
	    	    return 25;
	    	}
	    }
	    // type D
	    else if ((u4VDEW <= 966 && u4VDEW > 952))
	    {
	    	if ( (u4HDEW <= 1820 && u4HDEW > 1720) ||
	    		  u4HDEW <= 1516
	    		)
	    	{
	    	    return 43;
	    	}
	    	else
	    	{
	    	    return 26;
	    	}
	    }
	    // type E   //////////////
	    else if ((u4VDEW <= 872 && u4VDEW > 852)||
	    	     (u4VDEW <= 796 && u4VDEW > 770)
	    	    )
	    {
	    	if ( (u4HDEW <= 1680 && u4HDEW > 1636) ||
	    		 (u4HDEW <= 1588 && u4HDEW > 1532) ||
	    		 (u4HDEW <= 1492)
	    		)
	    	{
	    	    return 38;
	    	}	
	    	else
	    	{
	    	    return 22;
	    	}
	    }
	    // type F
	    else if ((u4VDEW <= 852 && u4VDEW > 836) ||
	    	     (u4VDEW <= 770 && u4VDEW > 734) 	             )
	    {
	    	if (u4HDEW <= 1760 && u4HDEW > 1680)
	    	{
	    	    return 28;
	    	}
	    	else if (u4HDEW <= 1680 && u4HDEW > 1624)
	    	{
	    	    return 43;
	    	}
	    	else
	    	{
	    	    return 20;
	    	}
	    }
	    // type G
	    else if ((u4VDEW <= 702 && u4VDEW > 684) ||
	    	     (u4VDEW <= 646 && u4VDEW > 598) ||
	    	     (u4VDEW <= 578 && u4VDEW > 558)  
	    	     )
	    {
	        if ( (u4HDEW <= 1808) ||
	    		 (u4HDEW <= 1560 && u4HDEW > 1500) 
	    		)
	    	{
	    	    return 47;
	    	}
	    	else
	    	{
	    	    return 28;
	    	}
	    }
	    // type H
	    else if ((u4VDEW <= 598 && u4VDEW > 578) ||
	    	     (u4VDEW <= 558 && u4VDEW > 522) ||
	    	     (u4VDEW <= 502) 
	    	     )
	    {
	        if ( (u4HDEW <= 1820 && u4HDEW > 1752) ||
	    		 (u4HDEW <= 1500 && u4HDEW > 1480) 
	    		)
	    	{
	    	    return 45;
	    	}
	    	else
	    	{
	    	    return 24;
	    	}    
	    }
	    // type I
	    else if ( u4VDEW == 704 || 
	    	     (u4VDEW <= 684 && u4VDEW > 646)
	    	    )
	    	
	    {
	    	if (u4HDEW > 1604)
	    	{
	    	    return 44;
	    	}
	    	else
	    	{
	    	    return 22;
	    	}    
	    }
	    // type J/////////
	    else if (u4VDEW <= 522 && u4VDEW > 502)
	    {
	    	if (u4HDEW <= 1724 && u4HDEW > 1672)
	    	{
	    	    return 44;
	    	}
	    	else
	    	{
	    	    return 22;
	    	}    
	    }
	    // type M
	    else if ((u4VDEW <= 734 && u4VDEW > 704))
	    {
	    	if (u4HDEW > 1676)
	    	{
	    	    return 34;
	    	}
	    	else if ((u4HDEW <= 1676 && u4HDEW > 1552))
	    	{
	    	    return 47;
	    	}
	    	else
	    	{
	    	    return 21;
	    	}      
	    }
	    
	    
	    return 0x15;
}

#else
///// for 68/96
inline UINT16 _u2Get60HzPrelen(UINT8 u1VdpID,UINT32 u4HDEW,UINT32 u4VDEW)
{

         if (u4VDEW >= 1080)
	    {
	    	if (u4HDEW > 1732)
	    	{
	    	    return 41;
	    	}
	    	else
	    	{
	    	    return 25;
	    	}
	    }
	     // type B
	    else if (u4VDEW <= 967 && u4VDEW > 953)
	    {
	    	if (u4HDEW > 1576)
	    	{
	    	    return 41;
	    	}
	    	else
	    	{
	    	    return 21;
	    	}
	    }
	    // type C
	    else if (u4VDEW <= 1073 && u4VDEW > 970)
	    {
	    	if (u4HDEW > 1756)
	    	{
	    	    return 43;
	    	}
	    	else if (u4HDEW <= 1756 && u4HDEW >1592)
	    	{
	    	    return 38;
	    	}
	    	else
	    	{
	    	    return 23;
	    	}
	    }
	    // type D
	    else if ((u4VDEW <= 1079 && u4VDEW > 1073) || 
	    	     (u4VDEW <= 953 && u4VDEW > 873) ||
	             (u4VDEW <= 837 && u4VDEW > 797))
	    {
	    	if (u4HDEW > 1576)
	    	{
	    	    return 34;
	    	}
	    	else
	    	{
	    	    return 21;
	    	}
	    }
	    // type E
	    else if ((u4VDEW <= 863 && u4VDEW > 853) || 
	    	         (u4VDEW <= 797 && u4VDEW > 771))
	    {
	    	if (u4HDEW > 1844)
	    	{
	    	    return 35;
	    	}
	    	else
	    	{
	    	    return 28;
	    	}
	    }
	    // type F
	    else if ((u4VDEW <= 735 && u4VDEW > 713))
	    {
	    	if (u4HDEW > 1748)
	    	{
	    	    return 50;
	    	}
	    	else if (u4HDEW <= 1748 && u4HDEW > 1684)
	    	{
	    	    return 29;
	    	}
	    	else if (u4HDEW <= 1684 && u4HDEW > 1500)
	    	{
	    	    return 22;
	    	}
	    	else
	    	{
	    	    return 44;
	    	}
	    }
	    // type G
	    else if ((u4VDEW <= 599 && u4VDEW > 579) || 
	                (u4VDEW <= 559 && u4VDEW > 521) ||
	                u4VDEW <= 503)
	    {
	    	if (u4HDEW > 1884)
	    	{
	    	    return 49;
	    	}
	    	else if (u4HDEW <= 1840 && u4HDEW > 1556)
	    	{
	    	    return 44;
	    	}
	    	else
	    	{
	    	    return 23;
	    	}
	    }
	    // type H
	    else if ((u4VDEW <= 703 && u4VDEW > 685) || 
	                (u4VDEW <= 647 && u4VDEW > 599) ||
	                (u4VDEW <= 577 && u4VDEW > 559))
	    {
	    	if (u4HDEW > 1712)
	    	{
	    	    return 40;
	    	}
	    	else if (u4HDEW <= 1712 && u4HDEW > 1592)
	    	{
	    	    return 23;
	    	}
	    	else if (u4HDEW <= 1592 && u4HDEW > 1548)
	    	{
	    	    return 46;
	    	}
	    	else
	    	{
	    	    return 21;
	    	}    
	    }
	    // type I
	    else if ((u4VDEW <= 521 && u4VDEW > 503))
	    {
	    	if (u4HDEW > 1800)
	    	{
	    	    return 25;
	    	}
	    	else if (u4HDEW <= 1800 && u4HDEW > 1744)
	    	{
	    	    return 43;
	    	}
	    	else if (u4HDEW <= 1744 && u4HDEW > 1492)
	    	{
	    	    return 22;
	    	}
	    	else
	    	{
	    	    return 45;
	    	}    
	    }
	    // type J
	    else if ((u4VDEW <= 713 && u4VDEW > 703) || 
	                (u4VDEW <= 685 && u4VDEW > 547))
	    {
	    	if (u4HDEW > 1820)
	    	{
	    	    return 29;
	    	}
	    	else if (u4HDEW <= 1820 && u4HDEW > 1720)
	    	{
	    	    return 45;
	    	}
	    	else
	    	{
	    	    return 21;
	    	}    
	    }
	    // type K
	    else if ((u4VDEW <= 853 && u4VDEW > 837) || 
	                (u4VDEW <= 771 && u4VDEW > 735))
	    {
	    	if (u4HDEW > 1752)
	    	{
	    	    return 48;
	    	}
	    	else
	    	{
	    	    return 24;
	    	}    
	    }
	    
	    return 0x15;
}

inline UINT16 _u2Get50HzPrelen(UINT8 u1VdpID,UINT32 u4HDEW,UINT32 u4VDEW)
{	    
	    return 0x25;
}
#endif
/**
* @brief  68/89 Get right prelen value for HD interlace output sync mode
*/
UINT16 u2GetMainPrelenValue(UINT8 u1VdpID)
{

    UINT8 u1fps = bDrvVideoGetRefreshRate(u1VdpID);
    UINT32 u4Prelen = 23;
    INT32 i4Adj;
    UINT32 u4VDEW = MDDI_READ_FLD(VDP_1, MCVP_KC_0A, VDEW);
	UINT32 u4HDEW = MDDI_READ_FLD(VDP_1, MCVP_KC_0A, HDEW);  

    UINT32 u4VBlank;

    u4VBlank = RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_PANEL_VTOTAL) - TV_HEIGHT;
    if (MODE_525I == bDrvVideoGetTiming(u1VdpID) || MODE_625I == bDrvVideoGetTiming(u1VdpID)) // interlace: 480i, 576i
    {
        return MAX((u4VBlank-7), 0x14);
    }

    if (IS_PROGRESSIVE(VDP_1) || IS_SD_HEIGHT(VDP_1) || 
        FRC_MODE(VDP_1) != E_MIB_FRC_MIB || u1VdpID == VDP_2)
    {
        return 0x15;
    }
#if defined(CC_MT5389)    
    if (IS_ECO_2_IC())
    {
        return 35;
    }
#endif

    if (u1fps == 50)
    {
        u4Prelen = _u2Get50HzPrelen(VDP_1 ,u4HDEW, u4VDEW);
        i4Adj = PANEL_GetVTotal50Hz() - 1125; 
    }
    else  // 60
    {
        u4Prelen = _u2Get60HzPrelen(VDP_1 ,u4HDEW, u4VDEW);
        i4Adj = PANEL_GetVTotal60Hz() - 1125; 
    }
    i4Adj = (i4Adj > 0) ? i4Adj : 0;

    return (UINT16)(u4Prelen + (UINT32)i4Adj);
}

