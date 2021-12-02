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
 * $RCSfile: drv_lcdim.c,v $
 *
 *---------------------------------------------------------------------------*/

// *-----------------------------------------------------------------------------
// * 0. include file
// *-----------------------------------------------------------------------------
#include "general.h"
#include "vdo_misc.h"
#include "drv_lcdim.h"
#include "hw_ycproc.h"
#include "panel.h"
#include "drv_meter.h"
#include "nptv_debug.h"
#include "hw_sw.h"

extern UINT8 bLcDimMappingCurve[256];

//UINT8 bLcDimInfo[4][LCDIM_CONTENT_INFO_MAX]; // 0 : max    1 : average    2 : content index (blending)   3 : dimming
UINT8  *bLcDimInfo; // 0 : max    1 : average    2 : content index (blending)   3 : dimming

#ifdef CC_MT5395 // add local data stretch support
UINT16 u2LcDSGainCurve[LCDS_CURVE_IDX] = 
{
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
};
#endif

UINT32 _u4BlockNumH, _u4BlockNumV;
UINT32 _u4BlockPxlH, _u4BlockPxlV;
UINT32 _u4BlockPxlHLast, _u4BlockPxlVLast;
// *-----------------------------------------------------------------------------
// * 1. initial table
// *-----------------------------------------------------------------------------
RRegDwrdTbl const REGTBL_LCDIM[] =
{
#ifdef CC_MT5365 // local dimming only, no data stretch related quality
    {    LOCAL_DIM_00, 0x01000000, 0x01007000},
    {    LOCAL_DIM_04, 0x80604020, 0xFFFFFFFF},
    {    LOCAL_DIM_05, 0x02E0C0A0, 0x03FFFFFF},
    {    LOCAL_DIM_07, 0x00200384, 0x00FFFFFF},
    {    LOCAL_DIM_08, 0x01002008, 0x0FFFFFFF},        
    {         REG_END, 0x00000000, 0x00000000},
#endif

#ifdef CC_MT5395 // add local data stretch support
    {    LOCAL_DIM_00, 0x01008180, 0x0100FFFF},
    {    LOCAL_DIM_04, 0x80604020, 0xFFFFFFFF},
    {    LOCAL_DIM_05, 0x0AE0C0A0, 0x0FFFFFFF},
    {    LOCAL_DIM_07, 0x00600450, 0x00FFFFFF},
    {    LOCAL_DIM_08, 0x0B002008, 0x0FFFFFFF},
    {    LOCAL_DIM_0D, 0x0C2334D5, 0x3FFFFFFF},
    {    LOCAL_DIM_0E, 0x090274AC, 0x3FFFFFFF},
    {    LOCAL_DIM_0F, 0xC8020084, 0xFFFFFFFF},
    {         REG_END, 0x00000000, 0x00000000},
#endif
};

// *-----------------------------------------------------------------------------
// * 2. functions
// *-----------------------------------------------------------------------------
#ifdef CC_MT5395 // add local data stretch support
void vDrvSetLcDSGainCurve()
{
    vRegWriteFldAlign(LOCAL_DIM_0D, u2LcDSGainCurve[0], C_DS_MULT_0);
    vRegWriteFldAlign(LOCAL_DIM_0D, u2LcDSGainCurve[1], C_DS_MULT_1);
    vRegWriteFldAlign(LOCAL_DIM_0D, u2LcDSGainCurve[2], C_DS_MULT_2);
    vRegWriteFldAlign(LOCAL_DIM_0E, u2LcDSGainCurve[3], C_DS_MULT_3);
    vRegWriteFldAlign(LOCAL_DIM_0E, u2LcDSGainCurve[4], C_DS_MULT_4);
    vRegWriteFldAlign(LOCAL_DIM_0E, u2LcDSGainCurve[5], C_DS_MULT_5);
    vRegWriteFldAlign(LOCAL_DIM_0F, u2LcDSGainCurve[6], C_DS_MULT_6);
    vRegWriteFldAlign(LOCAL_DIM_0F, u2LcDSGainCurve[7], C_DS_MULT_7);
    vRegWriteFldAlign(LOCAL_DIM_0F, u2LcDSGainCurve[8], C_DS_MULT_8);
}
#endif

void vDrvGetLcDimDimmingResult(UINT8 *bDimmingResult)
{
    UINT32 i;
    UINT32 u4InfoSize;

    u4InfoSize = _u4BlockNumH * _u4BlockNumV;
    
    //while (RegReadFldAlign(LOCAL_DIM_0A, DIM_RDY)==0);
    //avoid infinite wait
    for(i = 0; i < LCDIM_LOOP_WAIT; i++)
    {
        if(RegReadFldAlign(LOCAL_DIM_0A, DIM_RDY)==SV_TRUE)
        {
            break;
        }
        else
        {
            //LOG(0, "Local Dimming Retrieve Data Error.\n");
        }
    }

    vRegWriteFldAlign(LOCAL_DIM_0A, 1, C_FRAME_DLY_CPU);         
    vRegWriteFldAlign(LOCAL_DIM_0A, 0, C_FRAME_DLY_CPU_RADDR);         

    for(i = 0; i < u4InfoSize; i++)
    {
        bDimmingResult[i] = RegReadFldAlign(LOCAL_DIM_0A, DIM_DATA);
    }
    
    vRegWriteFldAlign(LOCAL_DIM_0A, 0, C_FRAME_DLY_CPU); 
	vRegWriteFldAlign(LOCAL_DIM_0A, 0, C_FRAME_DLY_CPU_RADDR); 
    vRegWriteFldAlign(LOCAL_DIM_0A, 0, DIM_RDY);
}

void vDrvUpdateLcDimContentInfo(UINT8 bContentSel)
{
    UINT32 i;
    UINT32 u4InfoSize;
    UINT32 u4LcDimDimmingInfo;

    u4InfoSize = _u4BlockNumH * _u4BlockNumV;

    switch(bContentSel)
    {
        case 0:     //retrieve dimming
//            while (RegReadFldAlign(LOCAL_DIM_0A, DIM_RDY)==0);
           // avoid infinite wait
            for(i = 0; i < LCDIM_LOOP_WAIT; i++)
            {
                if(RegReadFldAlign(LOCAL_DIM_0A, DIM_RDY)==SV_TRUE)
                {
                    break;
                }
                else
                {
                    //LOG(0, "Local Dimming Retrieve Data Error.\n");
                }
            }

            vRegWriteFldAlign(LOCAL_DIM_0A, 1, C_FRAME_DLY_CPU);         
            vRegWriteFldAlign(LOCAL_DIM_0A, 0, C_FRAME_DLY_CPU_RADDR);         

            for(i = 0; i < u4InfoSize; i++)
            {
                *(bLcDimInfo + (3*u4InfoSize) + i) = RegReadFldAlign(LOCAL_DIM_0A, DIM_DATA);
//                bLcDimInfo[3][i] = RegReadFldAlign(LOCAL_DIM_0A, DIM_DATA);
            }
            
            vRegWriteFldAlign(LOCAL_DIM_0A, 0, C_FRAME_DLY_CPU);         
            vRegWriteFldAlign(LOCAL_DIM_0A, 0, DIM_RDY);
            break;
        case 1:     //retrieve content max / average / blending
//            while (RegReadFldAlign(LOCAL_DIM_0A, DIM_RDY)==0);
           	vRegWriteFldMulti(LOCAL_DIM_0B, P_Fld(1, C_BLK_INFO_CS) |
	                                        P_Fld(0, C_BLK_INFO_CPU_RADDR));
            
            for(i = 0; i < u4InfoSize; i++)
            {
                u4LcDimDimmingInfo = u4RegRead4B(LOCAL_DIM_0C);
                *(bLcDimInfo + (0*u4InfoSize) + i) = (UINT8)((u4LcDimDimmingInfo >> 8) & 0xFF);    // max
//                bLcDimInfo[0][i] = (UINT8)((u4LcDimDimmingInfo >> 8) & 0xFF);    // max                
                *(bLcDimInfo + (1*u4InfoSize) + i) = (UINT8)( u4LcDimDimmingInfo & 0xFF);          // average
//                bLcDimInfo[1][i] = (UINT8)( u4LcDimDimmingInfo & 0xFF);          // average
                *(bLcDimInfo + (2*u4InfoSize) + i) = (UINT8)((u4LcDimDimmingInfo >> 16) & 0xFF);   // content index (blending)
//                bLcDimInfo[2][i] = (UINT8)((u4LcDimDimmingInfo >> 16) & 0xFF);   // content index (blending)
            }

            vRegWriteFldAlign(LOCAL_DIM_0B, 0, C_BLK_INFO_CS);               
            vRegWriteFldAlign(LOCAL_DIM_0A, 0, DIM_RDY);       
            break;            
        default:
            break;
    }
}

void vDrvLcDimLog(UINT8 bLogSel)
{
    UINT32 h, v;

    for(v = 0; v < _u4BlockNumV; v++)
    {
        for(h = 0; h < _u4BlockNumH; h++)
        {
            Printf("%3d\t", *(bLcDimInfo + (bLogSel*_u4BlockNumH*_u4BlockNumV) + (h+v*_u4BlockNumH)));
//            Printf("%3d\t", bLcDimInfo[bLogSel][h+v*_u4BlockNumH]);
        }
        Printf("\n");
    }

}

void vDrvSetLcDimOnOff(UINT8 bOnOff)
{
    vDrvSetLcDimOnOffInt(bOnOff);
}

void vDrvSetLcDimMappingCurve(UINT8 *bMappingCurve)
{
    UINT32 i;

    vRegWriteFldAlign(LOCAL_DIM_00, 0, C_MAPPING_EN);
    vRegWriteFldAlign(LOCAL_DIM_00, 1, C_MAPPING_CPU);

    for(i = 0; i < LCDIM_CURVE_IDX; i++)
    {
        vRegWrite4B(LOCAL_DIM_09, ((UINT32)bMappingCurve[i] << 16) | (i));
    	
        vRegWriteFldAlign(LOCAL_DIM_00, 1, C_MAPPING_WR);
        vRegWriteFldAlign(LOCAL_DIM_00, 0, C_MAPPING_WR);
    }

    vRegWriteFldAlign(LOCAL_DIM_00, 1, C_MAPPING_EN);
    vRegWriteFldAlign(LOCAL_DIM_00, 0, C_MAPPING_CPU);
    vRegWriteFldAlign(LOCAL_DIM_00, 0, C_MAPPING_WR);
    
}

BOOL vDrvVerifyLcDimMappingCurve()
{
    
    UINT32 i;
    UINT8 bLcDimCurveRead;
    BOOL bRet = SV_TRUE;

	vRegWriteFldAlign(LOCAL_DIM_00, 1, C_MAPPING_EN);
	vRegWriteFldAlign(LOCAL_DIM_00, 1, C_MAPPING_CPU);
	vRegWriteFldAlign(LOCAL_DIM_00, 0, C_MAPPING_WR);
    
    for(i = 0; i < LCDIM_CURVE_IDX; i++)
    {
        vRegWriteFldAlign(LOCAL_DIM_09, i, C_RAM_MAPPING_ADDR);
        bLcDimCurveRead = RegReadFldAlign(LOCAL_DIM_09, C_RAM_MAPPING_DI);

        if(bLcDimCurveRead != bLcDimMappingCurve[i])
        {
            Printf("[Index %d]bLcDimCurveRead = %d, bLcDimMappingCurve = %d\n", i, bLcDimCurveRead, bLcDimMappingCurve[i]);
            bRet = SV_FALSE;
        }
    }

	vRegWriteFldAlign(LOCAL_DIM_00, 1, C_MAPPING_EN);
	vRegWriteFldAlign(LOCAL_DIM_00, 0, C_MAPPING_CPU);
	vRegWriteFldAlign(LOCAL_DIM_00, 0, C_MAPPING_WR);

    return bRet;
}

void vDrvLcDimCalDivider()
{
	UINT32 u4BlockPxlCnt = _u4BlockPxlH * _u4BlockPxlV;
	UINT32 u4BlockLumaSumMax = _u4BlockPxlHLast * _u4BlockPxlVLast * 0xFF;
	UINT32 u4ShiftA=0, u4MultiplyB=0, u4ShiftC=0;
	UINT32 u4Shift_A_C, u4MultiplyB_Residue;

	for(u4Shift_A_C = 1; u4Shift_A_C < 100; u4Shift_A_C++)	
	{
		u4MultiplyB = ((UINT32)0x1 << u4Shift_A_C) / u4BlockPxlCnt;		
		
		u4MultiplyB_Residue = (((UINT32)0x1 << u4Shift_A_C)*100) / u4BlockPxlCnt;
		
//		if(u4MultiplyB >= 4 && (u4MultiplyB_Residue - u4MultiplyB * 100 < LCDIM_DIV_RES))
		if(u4MultiplyB >= 4 && ((u4MultiplyB_Residue - u4MultiplyB * 100) < u4MultiplyB * LCDIM_DIV_RES))
		{
			u4ShiftC = u4Shift_A_C >> 1;
			u4ShiftA = u4Shift_A_C - u4ShiftC;

            // u4BlockLumaSumMax >> u4ShiftA must be less than 12 bits
			while((u4BlockLumaSumMax >> u4ShiftA) > 0xFFF)
			{
				u4ShiftA++;
				u4ShiftC--;
			}
			break;
		}
	}

	if(u4Shift_A_C == 100)
	{
		Printf("panel out of spec.\n");
	}	
	else
	{
        vRegWriteFldAlign(LOCAL_DIM_06, u4ShiftA, C_SHIFT_A);
        vRegWriteFldAlign(LOCAL_DIM_06, u4MultiplyB, C_MULT_B);
        vRegWriteFldAlign(LOCAL_DIM_06, u4ShiftC, C_SHIFT_C);
        
		Printf("-> u4ShiftA = %d\n-> u4MultiplyB = %d\n-> u4ShiftC = %d\n", u4ShiftA, u4MultiplyB, u4ShiftC);
	}
}

void vDrvLcDimSetMaxThd(UINT32 u4LcDimMaxThd)
{
    UINT32 u4BlockPxlSum = _u4BlockPxlH * _u4BlockPxlV;
    UINT32 u4MaxThd = (u4BlockPxlSum * (1000 - u4LcDimMaxThd)) / 1000;
    
    vRegWriteFldAlign(LOCAL_DIM_07, u4MaxThd, C_MAX_THD);
    Printf("-> u4LcDimMaxThd = %d (Total Pixel of Block = %d)\n\n", u4MaxThd, u4BlockPxlSum);
}

void vDrvLcDimSetAveWeight(UINT8 bAveWeight)
{
    vRegWriteFldAlign(LOCAL_DIM_07, bAveWeight, C_AVE_W);
}

void vDrvLcDimSetTPFParam(UINT8 bTPFCurWeight)
{
	vRegWriteFldAlign(LOCAL_DIM_08, bTPFCurWeight, C_CUR_W_MIN);
}


void vDrvLcDimSetPanelInfo(UINT32 u4BlockNumH, UINT32 u4BlockNumV)
{
	UINT32 u4BlockPxlH, u4BlockPxlV;
	UINT32 u4BlockPxlHLast, u4BlockPxlVLast;
	UINT32 u4PanelWidth, u4PanelHeight; 
    UINT8 bAdapLcDimOnOff;
    
    _u4BlockNumH = u4BlockNumH;
    _u4BlockNumV = u4BlockNumV;    

	u4PanelWidth = PANEL_GetPanelWidth();
	u4PanelHeight = PANEL_GetPanelHeight();
	
	u4BlockPxlH = u4PanelWidth / u4BlockNumH;
	u4BlockPxlV = u4PanelHeight / u4BlockNumV;
	u4BlockPxlHLast = (u4PanelWidth / u4BlockNumH) + (u4PanelWidth % u4BlockNumH);
	u4BlockPxlVLast = (u4PanelHeight / u4BlockNumV) + (u4PanelHeight % u4BlockNumV);

    _u4BlockPxlH = u4BlockPxlH;
    _u4BlockPxlV = u4BlockPxlV;
    _u4BlockPxlHLast = u4BlockPxlHLast;
    _u4BlockPxlVLast = u4BlockPxlVLast;    

    vRegWriteFldAlign(LOCAL_DIM_03, u4BlockNumH-1, C_BLK_H);
    vRegWriteFldAlign(LOCAL_DIM_03, u4BlockNumV-1, C_BLK_V);  
    vRegWriteFldAlign(LOCAL_DIM_01, u4BlockPxlH-1, C_PXL_H);
    vRegWriteFldAlign(LOCAL_DIM_01, u4BlockPxlV-1, C_PXL_V);
    vRegWriteFldAlign(LOCAL_DIM_02, u4BlockPxlHLast-1, C_PXL_H_LAST);
    vRegWriteFldAlign(LOCAL_DIM_02, u4BlockPxlVLast-1, C_PXL_V_LAST);

	Printf("[LcDim Panel]\nu4BlockNumH = %d\nu4BlockNumV = %d\n-> u4BlockPxlH = %d\n-> u4BlockPxlV = %d\n-> u4BlockPxlHLast = %d\n-> u4BlockPxlVLast = %d\n", 
		u4BlockNumH, u4BlockNumV, u4BlockPxlH, u4BlockPxlV, u4BlockPxlHLast, u4BlockPxlVLast);


    // ============ create local dimming info array memory space
    bAdapLcDimOnOff = bDrvGetAdaptiveLcDimOnOff();
    
    vDrvSetAdaptiveLcDimOnOff(SV_FALSE);

    x_mem_free(bLcDimInfo);        

    if ((bLcDimInfo = (UINT8*)x_mem_alloc(4*u4BlockNumH*u4BlockNumV)) == NULL)
    {
        Printf("Local Dimming Info Array Initialization Error!\n");
        return;
    }        

    vDrvSetAdaptiveLcDimOnOff(bAdapLcDimOnOff);        
    // ============ end of create local dimming info array memory space
    
	vDrvLcDimCalDivider();
}

void vDrvLcDinQualityInit()
{
    // Quality Initialization
    vDrvLoadRegDwrdTbl(REGTBL_LCDIM);
    vDrvSetLcDimMappingCurve(bLcDimMappingCurve);

    // Adaptive Local Dimming Parameter Init
    vDrvAdaptiveLcDimSetParam();
	vDrvLcDimSetMaxThd(LCDIM_MAX_THD);    
}

void vDrvLcDimPanelInit()
{
    PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam;

  	UINT32 u4BlockNumH = 8, u4BlockNumV = 2;

    // Get Local Dimming information here  	
    if (PANEL_GetLocalDimmingParam(&pParam) == SV_TRUE)
    {
        if(pParam!=NULL)
        {
            u4BlockNumH = pParam->u1BlockNumM;
            u4BlockNumV = pParam->u1BlockNumN;
        }
        else
        {
            u4BlockNumH = 1;
            u4BlockNumV = 1;
        }

        _u4BlockNumH = u4BlockNumH;
        _u4BlockNumV = u4BlockNumV;        
    }    
    else
    {
        Printf("panel not support local dimming");
        // not support local dimming
        return;
    }    
  	
    // Panel Initialization
    vDrvLcDimSetPanelInfo(u4BlockNumH, u4BlockNumV);
}


