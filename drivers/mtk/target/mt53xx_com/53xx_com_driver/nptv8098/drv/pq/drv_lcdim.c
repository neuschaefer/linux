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
#include "hw_ospe.h"
#include "panel.h"
#include "drv_meter.h"
#include "nptv_debug.h"
#include "hw_sw.h"

#define SW_CTRL_PT_NUM 9
#define SW_CTRL_PT_SHIFT 5  // NOTE: 256 / (SW_CTRL_PT_NUM - 1) == 2 ^ SW_CTRL_PT_SHIFT

extern PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam;

typedef enum{
    E_LDM_INFO_MAX              =   0x0,
    E_LDM_INFO_AVE              =   0x1,
    E_LDM_INFO_CONTENT_INDEX    =   0x2,
    E_LDM_INFO_DIM              =   0x3,
    E_LDM_INFO_NUM              =   0x4,
} E_LDM_CONTENT_INFO;

EXTERN UINT8 bSupportLocalDimming;
EXTERN UINT16 u2LcDimAveW;    
EXTERN UINT16 u2LcDimCurW;

UINT8  *bLcDimInfo = NULL; // 0 : max    1 : average    2 : content index (blending)   3 : dimming

UINT8 u1LcdimDftCurve[LCDIM_CURVE_NUM];
UINT8 u1LcdimStrCurve[LCDIM_CURVE_NUM];

UINT16 u2LcDSGainCurve[LCDS_CURVE_IDX] =  {0xAA, 0xA9, 0xA0, 0x97, 0x8E, 0x87, 0x84, 0x80, 0x80};   // data stretch gain curve
UINT8 u1LcdimDftSwCurve[SW_CTRL_PT_NUM] = {0x18, 0x30, 0x68, 0xA2, 0xCF, 0xEC, 0xF5, 0xF9, 0xFF};   // default dimming curve
UINT8 u1LcdimStrSwCurve[SW_CTRL_PT_NUM] = {0x00, 0x10, 0x53, 0x9A, 0xCF, 0xEC, 0xF5, 0xF9, 0xFF};   // strong  dimming curve
UINT8 u1LcdimSWCtrlPtPre[2][SW_CTRL_PT_NUM];  // recording dimming curve control point of previous frame

UINT16 const PXL_H_TBL[30][5] = 
{
/*block*/  //H0, H1, H2, H3, H_LAST
/*01*/    {1920, 1920, 1920, 1920, 1920},  
/*02*/    {960, 960, 960, 960, 960},  
/*03*/    {640, 640, 640, 640, 640}, 
/*04*/    {480, 480, 480, 480, 480}, 
/*05*/    {384, 384, 384, 384, 384}, 
/*06*/    {320, 320, 320, 320, 320}, 
/*07*/    {276, 272, 276, 272, 276}, 
/*08*/    {240, 240, 240, 240, 240}, 
/*09*/    {216, 212, 212, 212, 216}, 
/*10*/    {192, 192, 192, 192, 192}, 
/*11*/    {172, 176, 176, 176, 172}, 
/*12*/    {160, 160, 160, 160, 160}, 
/*13*/    {148, 148, 148, 148, 144}, 
/*14*/    {140, 136, 136, 136, 136}, 
/*15*/    {128, 128, 128, 128, 128}, 
/*16*/    {120, 120, 120, 120, 120}, 
/*17*/    {116, 112, 112, 112, 112}, 
/*18*/    {104, 108, 108, 108, 104}, 
/*19*/    {104, 100, 100, 100, 100}, 
/*20*/    {96, 96, 96, 96, 96}, 
/*21*/    {92, 92, 92, 92, 80}, 
/*22*/    {88, 88, 88, 84, 92}, 
/*23*/    {84, 84, 84, 84, 72}, 
/*24*/    {80, 80, 80, 80, 80}, 
/*25*/    {80, 76, 76, 76, 72}, 
/*26*/    {72, 72, 76, 76, 72}, 
/*27*/    {72, 72, 72, 68, 72}, 
/*28*/    {68, 68, 68, 72, 60}, 
/*29*/    {64, 68, 64, 68, 72}, 
/*30*/    {64, 64, 64, 64, 64} 
};

UINT32 _u4BlockNumH, _u4BlockNumV;
UINT32 _u4BlockPxlH, _u4BlockPxlV;
// *-----------------------------------------------------------------------------
// * 1. initial table
// *-----------------------------------------------------------------------------
RRegDwrdTbl const REGTBL_LCDIM[] =
{
    {    LOCAL_DIM_00, 0x01318180, 0x0131FFFF},
    {    LOCAL_DIM_04, 0x80604020, 0xFFFFFFFF},
    {    LOCAL_DIM_05, 0x0AE0C0A0, 0x0FFFFFFF},
    {    LOCAL_DIM_07, 0x00600450, 0x00FFFFFF},
    {    LOCAL_DIM_08, 0x0B002008, 0x0FFFFFFF},
    {    LOCAL_DIM_0D, 0x0C2334D5, 0x3FFFFFFF},
    {    LOCAL_DIM_0E, 0x090274AC, 0x3FFFFFFF},
    {    LOCAL_DIM_0F, 0xC8020084, 0xFFFFFFFF},
    {         REG_END, 0x00000000, 0x00000000},
};

// *-----------------------------------------------------------------------------
// * 2. functions
// *-----------------------------------------------------------------------------
void vDrvSetLcDSGainCurve(void)
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

void vDrvGetLcDimDimmingResult(UINT8 *bDimmingResult)
{
    UINT32 i;
    UINT32 u4InfoSize;

    u4InfoSize = _u4BlockNumH * _u4BlockNumV;
    
    //avoid infinite wait
    for(i = 0; i < LCDIM_LOOP_WAIT; i++)
    {
        if(RegReadFldAlign(LOCAL_DIM_0A, DIM_RDY)==SV_TRUE)
        {
            vRegWriteFldAlign(LOCAL_DIM_0A, 1, C_FRAME_DLY_CPU);         
            vRegWriteFldAlign(LOCAL_DIM_0A, 0, C_FRAME_DLY_CPU_RADDR);         

            for(i = 0; i < u4InfoSize; i++)
            {
                bDimmingResult[i] = RegReadFldAlign(LOCAL_DIM_0A, DIM_DATA);
            }
            
            vRegWriteFldAlign(LOCAL_DIM_0A, 0, C_FRAME_DLY_CPU); 
        	vRegWriteFldAlign(LOCAL_DIM_0A, 0, C_FRAME_DLY_CPU_RADDR); 
            vRegWriteFldAlign(LOCAL_DIM_0A, 0, DIM_RDY);            
            break;
        }
        else
        {
            //LOG(0, "Local Dimming Retrieve Data Error.\n");
        }
    }


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
            // avoid infinite wait
            for(i = 0; i < LCDIM_LOOP_WAIT; i++)
            {
                if(RegReadFldAlign(LOCAL_DIM_0A, DIM_RDY)==SV_TRUE)
                {
                    vRegWriteFldAlign(LOCAL_DIM_0A, 1, C_FRAME_DLY_CPU);         
                    vRegWriteFldAlign(LOCAL_DIM_0A, 0, C_FRAME_DLY_CPU_RADDR);         

                    for(i = 0; i < u4InfoSize; i++)
                    {
                        *(bLcDimInfo + (E_LDM_INFO_DIM*u4InfoSize) + i) = RegReadFldAlign(LOCAL_DIM_0A, DIM_DATA);
                    }            
                    vRegWriteFldAlign(LOCAL_DIM_0A, 0, C_FRAME_DLY_CPU);         
                    vRegWriteFldAlign(LOCAL_DIM_0A, 0, DIM_RDY);                    
                    break;
                }
                else
                {
                    //LOG(0, "Local Dimming Retrieve Data Error.\n");
                }
            }

            break;
        case 1:     //retrieve content max / average / blending
           	vRegWriteFldMulti(LOCAL_DIM_0B, P_Fld(1, C_BLK_INFO_CPU) |
	                                        P_Fld(0, C_BLK_INFO_CPU_RADDR));
            
            for(i = 0; i < u4InfoSize; i++)
            {
                u4LcDimDimmingInfo = u4RegRead4B(LOCAL_DIM_0C);
                *(bLcDimInfo + (E_LDM_INFO_MAX*u4InfoSize) + i) = (UINT8)((u4LcDimDimmingInfo >> 8) & 0xFF);    // max
                *(bLcDimInfo + (E_LDM_INFO_AVE*u4InfoSize) + i) = (UINT8)( u4LcDimDimmingInfo & 0xFF);          // average
                *(bLcDimInfo + (E_LDM_INFO_CONTENT_INDEX*u4InfoSize) + i) = (UINT8)((u4LcDimDimmingInfo >> 16) & 0xFF);   // content index (blending)
            }

            vRegWriteFldAlign(LOCAL_DIM_0B, 0, C_BLK_INFO_CPU);               
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
        }
        Printf("\n");
    }
}

void vDrvSetLcDimOnOff(UINT8 bOnOff)
{
    vDrvSetLcDimOnOffInt(bOnOff);
}

void vDrvLcDimKickPingPong(void)
{
    vRegWriteFldAlign(LOCAL_DIM_15, 1, R_MAPPING_FILL_TABLE_READY);         // set ping-pong tick
    vRegWriteFldAlign(LOCAL_DIM_15, 0, R_MAPPING_FILL_TABLE_READY);         // set ping-pong tick

//    while(RegReadFldAlign(LOCAL_DIM_15, MAPPING_TABLE_WAIT_VSYNC));
}

void vDrvSetLcDimMappingCurve(UINT8 *bMappingCurve)
{
    UINT32 u4Index;

    vRegWriteFldAlign(LOCAL_DIM_15, 1, R_MAPPING_WR_AUTO_INC_ADDR);         // set write auto-incremental
    vRegWriteFldAlign(LOCAL_DIM_09, 0, R_MAPPING_ADDR);                     // set initial address to 0

    for (u4Index = 0; u4Index < 256; u4Index++)
    {
        vRegWrite4B(LOCAL_DIM_16, bMappingCurve[u4Index]);                  // R_MAPPING_WDATA
    }                                           

    vRegWriteFldAlign(LOCAL_DIM_15, 0, R_MAPPING_WR_AUTO_INC_ADDR);         // set write auto-incremental   
    vRegWriteFldAlign(LOCAL_DIM_09, 0, R_MAPPING_ADDR);                     // set initial address to 0
}

#ifndef NDEBUG    // PQ SRAM Stress Test Only Take Effect in Debug Build
BOOL vDrvVerifyLcDimMappingCurve(UINT8 *bLcDimMappingCurve)
{
    UINT8 bLcDimMappingCurveReadBack[256];
    UINT16 index;
    UINT8 bRet = SV_TRUE;

    vRegWriteFldAlign(LOCAL_DIM_15, 1, R_MAPPING_RD_AUTO_INC_ADDR);       // set write auto-incremental
    vRegWriteFldAlign(LOCAL_DIM_09, 0, R_MAPPING_ADDR);                   // set initial address to 0

    for (index = 0; index < 256; index++)
    {
        bLcDimMappingCurveReadBack[index] = (u4RegRead4B(LOCAL_DIM_16) >> 16) & 0xFF;
    }

    for (index = 0; index < 256; index++)
    {
        if(bLcDimMappingCurveReadBack[index] != bLcDimMappingCurve[index])
        {
            Printf("[FAIL] LCDIM [%d] : [W] = %d, [R] = %d\n", index, bLcDimMappingCurve[index], 
                                                               bLcDimMappingCurveReadBack[index]);
            bRet = SV_FALSE;
        }
    }
    
    vRegWriteFldAlign(LOCAL_DIM_15, 0, R_MAPPING_RD_AUTO_INC_ADDR);         // set write auto-incremental   
    vRegWriteFldAlign(LOCAL_DIM_09, 0, R_MAPPING_ADDR);                     // set initial address to 0

    return bRet;
}
#endif // NDEBUG    
void vDrvLcDimCalDivider(UINT32 u4BlockPxlHMax, UINT32 u4BlockPxlVMax)
{
	UINT32 u4BlockPxlCnt = _u4BlockPxlH * _u4BlockPxlV;
	UINT32 u4BlockLumaSumMax = u4BlockPxlHMax * u4BlockPxlVMax * 0xFF;
	UINT32 u4ShiftA=0, u4MultiplyB=0, u4ShiftC=0;
	UINT32 u4Shift_A_C, u4MultiplyB_Residue;

	for(u4Shift_A_C = 1; u4Shift_A_C < 100; u4Shift_A_C++)	
	{
		u4MultiplyB = ((UINT32)0x1 << u4Shift_A_C) / u4BlockPxlCnt;		
		
		u4MultiplyB_Residue = (((UINT32)0x1 << u4Shift_A_C)*100) / u4BlockPxlCnt;
		
		if(u4MultiplyB >= 4 && ((u4MultiplyB_Residue - u4MultiplyB * 100) < u4MultiplyB * LCDIM_DIV_RES))
		{
			u4ShiftC = u4Shift_A_C >> 1;
			u4ShiftA = u4Shift_A_C - u4ShiftC;

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


BOOL bDrvLcDimSetPanelInfo(UINT32 u4BlockNumH, UINT32 u4BlockNumV)
{
	UINT32 u4BlockPxlH0, u4BlockPxlH1, u4BlockPxlH2, u4BlockPxlH3, u4BlockPxlV;
	UINT32 u4BlockPxlHLast, u4BlockPxlVLast;
	UINT32 u4PanelWidth, u4PanelHeight; 
    UINT8 bAdapLcDimOnOff;
    
    _u4BlockNumH = u4BlockNumH;
    _u4BlockNumV = u4BlockNumV;    

	u4PanelWidth = PANEL_GetPanelWidth();
	u4PanelHeight = PANEL_GetPanelHeight();
	
    if (u4BlockNumH <= 30)
    {
        u4BlockPxlH0 = PXL_H_TBL[u4BlockNumH-1][0];
        u4BlockPxlH1 = PXL_H_TBL[u4BlockNumH-1][1];
        u4BlockPxlH2 = PXL_H_TBL[u4BlockNumH-1][2];
        u4BlockPxlH3 = PXL_H_TBL[u4BlockNumH-1][3];
        u4BlockPxlHLast = PXL_H_TBL[u4BlockNumH-1][4];
    }
    else
    {
        Printf("LocalDimming H Block Num Error: %d\n", u4BlockNumH);
        return SV_FAIL;
    }

	u4BlockPxlV = u4PanelHeight / u4BlockNumV;
	u4BlockPxlVLast = (u4PanelHeight / u4BlockNumV) + (u4PanelHeight % u4BlockNumV);

    _u4BlockPxlH = MAX(MAX(MAX(MAX(u4BlockPxlH0, u4BlockPxlH1), u4BlockPxlH2), u4BlockPxlH3), u4BlockPxlHLast);
    _u4BlockPxlV = u4BlockPxlV;

    vRegWriteFldAlign(LOCAL_DIM_03, u4BlockNumH-1, C_BLK_H);
    vRegWriteFldAlign(LOCAL_DIM_03, u4BlockNumV-1, C_BLK_V); 
 
    vRegWriteFldAlign(LOCAL_DIM_18, u4BlockPxlH0-1, C_PXL_H_0);
    vRegWriteFldAlign(LOCAL_DIM_18, u4BlockPxlH1-1, C_PXL_H_1);
    vRegWriteFldAlign(LOCAL_DIM_19, u4BlockPxlH2-1, C_PXL_H_2);
    vRegWriteFldAlign(LOCAL_DIM_19, u4BlockPxlH3-1, C_PXL_H_3);  
    vRegWriteFldAlign(LOCAL_DIM_02, u4BlockPxlHLast-1, C_PXL_H_LAST);

    vRegWriteFldAlign(LOCAL_DIM_01, u4BlockPxlV-1, C_PXL_V);
    vRegWriteFldAlign(LOCAL_DIM_02, u4BlockPxlVLast-1, C_PXL_V_LAST);

	Printf("[LcDim Panel]\n-> Panel = %d x %d\n-> BlockNum = %d x %d\n-> u4BlockPxlH = %d %d %d %d (%d)\n-> u4BlockPxlV = %d\n-> u4BlockPxlVLast = %d\n", 
            u4PanelWidth, u4PanelHeight,
            _u4BlockNumH, 
            _u4BlockNumV, 
    		u4BlockPxlH0, u4BlockPxlH1, u4BlockPxlH2, u4BlockPxlH3, u4BlockPxlHLast,
    		_u4BlockPxlV, u4BlockPxlVLast);


    // ============ create local dimming info array memory space
    bAdapLcDimOnOff = bDrvGetAdaptiveLcDimOnOff();
    
    vDrvSetAdaptiveLcDimOnOff(SV_FALSE);

    if(bLcDimInfo!=NULL)
    {
        x_mem_free(bLcDimInfo);        
    }
    
    if ((bLcDimInfo = (UINT8*)x_mem_alloc(E_LDM_INFO_NUM*u4BlockNumH*u4BlockNumV)) == NULL)
    {
        Printf("Local Dimming Info Array Initialization Error!\n");
        return SV_FAIL;
    }        
    else
    {
        Printf("Memory Allocation for Local Dimming OK with Size = %d Byte.\n", 4*u4BlockNumH*u4BlockNumV);
    }
    
	vDrvLcDimCalDivider(_u4BlockPxlH, u4BlockPxlVLast);
	vDrvLcDimSetMaxThd(pParam->u2HistMaxThd);

    vDrvSetAdaptiveLcDimOnOff(bAdapLcDimOnOff);        
    return SV_SUCCESS;
}

UINT8 bDrvLcDimSetSWCurve(UINT8 *u1InterpolatedCurve, UINT32 u4SWAddrBase, UINT8 u1Index)
{
    UINT8 u1LcdimSWCtrlPt[SW_CTRL_PT_NUM];
    UINT8 CtrlPt = 0;
    UINT32 i;

    // 1. read out control point from sw register
    for(CtrlPt = 0; CtrlPt < SW_CTRL_PT_NUM; CtrlPt++)
    {
        u1LcdimSWCtrlPt[CtrlPt] = u1IO32Read1B(u4SWAddrBase+CtrlPt);
    }

    // 2. compare read-out with exist one, if the same, do nothing, else re-interpolate
    if (x_memcmp((VOID*)&u1LcdimSWCtrlPtPre[u1Index][0], (VOID*)u1LcdimSWCtrlPt, SW_CTRL_PT_NUM) != 0)
    {
        // 3. re-interpolate
        for(i = 0; i < 256; i++)
        {
            u1InterpolatedCurve[i] = (UINT8)((UINT16)u1LcdimSWCtrlPt[i>>SW_CTRL_PT_SHIFT] + (UINT16)((UINT16)(u1LcdimSWCtrlPt[(i>>SW_CTRL_PT_SHIFT)+1] - u1LcdimSWCtrlPt[i>>SW_CTRL_PT_SHIFT])*(UINT16)(i-((i>>SW_CTRL_PT_SHIFT)<<SW_CTRL_PT_SHIFT))>>SW_CTRL_PT_SHIFT));
        }
        x_memcpy(&u1LcdimSWCtrlPtPre[u1Index][0], u1LcdimSWCtrlPt, SW_CTRL_PT_NUM); 
        return SV_TRUE;
    }
    else
    {
        x_memcpy(&u1LcdimSWCtrlPtPre[u1Index][0], u1LcdimSWCtrlPt, SW_CTRL_PT_NUM);         
        return SV_FALSE;
    }
}


void vDrvLcDinQualityInit(void)
{
    UINT8 i;
    
    // Quality Initialization
    vDrvLoadRegDwrdTbl(REGTBL_LCDIM);
    
    // Init Dimming Curve
    for(i = 0; i < SW_CTRL_PT_NUM; i++)
    {
        vIO32Write1B(LCDIM_SW_CTRL_PT_DFT_0+i, u1LcdimDftSwCurve[i]);
        vIO32Write1B(LCDIM_SW_CTRL_PT_STR_0+i, u1LcdimStrSwCurve[i]);        
    }

    bDrvLcDimSetSWCurve(u1LcdimDftCurve, LCDIM_SW_CTRL_PT_DFT_0, 0);

    vDrvSetLcDimMappingCurve(u1LcdimDftCurve);    
    vDrvLcDimKickPingPong();

    // Init DS Gain Curve
    vDrvSetLcDSGainCurve();

    // SW Register Initialization
    vIO32WriteFldAlign(ADAP_LCDIM_00, SV_TRUE, ADAP_LD_CUR_CTRL);
    vIO32WriteFldAlign(ADAP_LCDIM_00, SV_FALSE, ADAP_LCDIM_MOTION_PENALTY_EN);
    vIO32WriteFldAlign(ADAP_LCDIM_00, SV_TRUE, ADAP_LCDIM_BLACK_PENALTY_EN);    
    
    vIO32WriteFldAlign(ADAP_LCDIM_00, SV_FALSE, ADAP_LD_SENS_CTRL);
    
    vIO32WriteFldAlign(ADAP_LCDIM_01, 0x20, LCDIM_QUE_SIZE_AVE);    
    vIO32WriteFldAlign(ADAP_LCDIM_01, 0x10, LCDIM_QUE_SIZE_BLACK);        
    
    vIO32WriteFldAlign(ADAP_LCDIM_01, 8, LCDIM_BLACK_THD);

    vIO32WriteFldAlign(ADAP_LCDIM_03, 0x20, LCDIM_AVE_W_MIN);
    vIO32WriteFldAlign(ADAP_LCDIM_03, 0x60, LCDIM_AVE_W_MAX);    

    vIO32WriteFldAlign(ADAP_LCDIM_03, 0x4,  LCDIM_CUR_W_MIN);
    vIO32WriteFldAlign(ADAP_LCDIM_03, 0x20, LCDIM_CUR_W_MAX);        

    vIO32WriteFldAlign(ADAP_LCDIM_02, 85, LCDIM_NR_MOTION_THD);
    vIO32WriteFldAlign(ADAP_LCDIM_01, 42, LCDIM_APL_THD);    

    vIO32WriteFldAlign(ADAP_LCDIM_04, 2, LCDIM_MOTION_PENALTY_STEP);       
    vIO32WriteFldAlign(ADAP_LCDIM_04, 2, LCDIM_BLACK_PENALTY_STEP);           

    u2LcDimAveW = 0x60;    
    u2LcDimCurW = 0x20;
}

BOOL bDrvLcDimPanelInit(void)
{    
    // Panel Initialization
    if(bDrvLcDimSetPanelInfo(pParam->u1BlockNumM, pParam->u1BlockNumN) == SV_FAIL)        
    {
        Printf("[LDM] Panel Block Initialization Fail\b");
        return SV_FAIL;
    }
    return SV_SUCCESS;
}

void vDrvLcdimProc(void)
{    
    if(bSupportLocalDimming)
    {
        // interpolate dimming curve from sw register control points
        bDrvLcDimSetSWCurve(u1LcdimDftCurve, LCDIM_SW_CTRL_PT_DFT_0, 0);
        bDrvLcDimSetSWCurve(u1LcdimStrCurve, LCDIM_SW_CTRL_PT_STR_0, 1);        

        vDrvAdaptiveLcDim(u1LcdimDftCurve, u1LcdimStrCurve);

        vDrvSetLcDimMappingCurve(u1LcdimDftCurve);
        vDrvLcDimKickPingPong();               
    }        
}
