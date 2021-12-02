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

// === INCLUDE =============================================================================
#include "x_os.h"	
#include "sv_const.h"
#include "hw_vdoin.h"
#include "nptv_debug.h"
#include "drv_pq_cust.h"
#include "drv_ycproc.h"
// === HW INCLUDE =============================================================================
#include "hw_ycproc.h"
#include "hw_sw.h"

// === DEFINE =============================================================================
#define SCE_OFFSET	            0x400
#define SUPPORT_NEW_C_BOOST     SV_TRUE
#define SUPPORT_NEW_SAT_GAIN_Y  SV_TRUE

#define UINT8_CLIP(val, min, max) ((UINT8)((val>=max) ? max : ((val<=min) ? min : val)))  

// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================

// === GLOBAL VARIABLE ====================================================================
UINT8 bSceGain[3]  = {0x80, 0x80, 0x80};
UINT8 bSkinTable[3] = {0x80, 0x80, 0x80};

// === EXTERN VARIABLE ====================================================================

// === STATIC VARIABLE ====================================================================
static UINT8 bPrevPhase = 0;
static UINT32 dwSCECounter = 0;
static UINT8 bFindColorOnOff = SV_OFF;

// === END OF STATIC VARIABLE =============================================================

void vDrvSCEFindColor()
{
	UINT8 bPhase;

	if ((bFindColorOnOff == SV_ON) && (RegReadFldAlign(DUMMY_02, SCE_FINDCOLOR_ONOFF) == SV_OFF))
	{
		if (bPrevPhase < 28)
		{
			vRegWrite4B(LOCAL_SAT_1, 0x30000 + bPrevPhase);

			vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1,    bPrevPhase) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_P2,    bPrevPhase)<<8) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_GAIN1, bPrevPhase)<<16) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_GAIN2, bPrevPhase)<<24));
			vRegWrite4B(LOCAL_SAT_3, READ_SCE_TBL(ENUM_S_GAIN3, bPrevPhase) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_Y000,  bPrevPhase)<<8) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_Y064,  bPrevPhase)<<16) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_Y128,  bPrevPhase)<<24));
			vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192,  bPrevPhase) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_Y256,  bPrevPhase)<<8));

			vRegWrite4B(LOCAL_SAT_1, 0x10000 + bPrevPhase);

			bFindColorOnOff = SV_OFF;

			return;
		}
	}

	bFindColorOnOff = RegReadFldAlign(DUMMY_02, SCE_FINDCOLOR_ONOFF);

	if (bFindColorOnOff == SV_ON)
	{
		bPhase = RegReadFldAlign(DUMMY_02, SCE_FINDCOLOR_PHASE);

		if ((bPhase < 28) && (bPrevPhase < 28))
		{
			if (bPhase != bPrevPhase)
			{
				vRegWrite4B(LOCAL_SAT_1, 0x30000 + bPrevPhase);

				vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1,    bPrevPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_P2,    bPrevPhase)<<8) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_GAIN1, bPrevPhase)<<16) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_GAIN2, bPrevPhase)<<24));
				vRegWrite4B(LOCAL_SAT_3, READ_SCE_TBL(ENUM_S_GAIN3, bPrevPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y000,  bPrevPhase)<<8) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y064,  bPrevPhase)<<16) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y128,  bPrevPhase)<<24));
				vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192,  bPrevPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y256,  bPrevPhase)<<8));

				vRegWrite4B(LOCAL_SAT_1, 0x10000 + bPrevPhase);
			}


			if (dwSCECounter % 120 == 60)
			{
				vRegWrite4B(LOCAL_SAT_1, 0x30000 + bPhase);

				vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1,    bPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_P2,    bPhase)<<8) +
				            ((UINT32)0xFF<<16) +
				            ((UINT32)0xFF<<24));
				vRegWrite4B(LOCAL_SAT_3, 0xFF +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y000,  bPhase)<<8) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y064,  bPhase)<<16) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y128,  bPhase)<<24));
				vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192,  bPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y256,  bPhase)<<8));

				vRegWrite4B(LOCAL_SAT_1, 0x10000 + bPhase);
			}

			if (dwSCECounter % 120 == 0)
			{
				vRegWrite4B(LOCAL_SAT_1, 0x30000 + bPhase);

				vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1,    bPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_P2,    bPhase)<<8) +
				            (0x0<<16) +
				            (0x0<<24));
				vRegWrite4B(LOCAL_SAT_3, 0x0 +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y000,  bPhase)<<8) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y064,  bPhase)<<16) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y128,  bPhase)<<24));
				vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192,  bPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y256,  bPhase)<<8));

				vRegWrite4B(LOCAL_SAT_1, 0x10000 + bPhase);
			}
		}
		bPrevPhase = bPhase;
		dwSCECounter++;
	}

}

/*****************************************************************************************/
/********************************** ANGULAR SCE MODEL ************************************/
/*****************************************************************************************/
#define SCE_LUMA_NUM        16
#define SCE_SAT_NUM         28
#define SCE_HUE_NUM         28

// Skin tone configuration note: Start phase must be larger than end phase
#define SKIN_START_28       10  // SKIN_START_28 phase can't be 0
#define SKIN_END_28         12  // SKIN_END_28 phase can't be 28 

#define SKIN_START_16       6   // SKIN_START_28 phase can't be 0   
#define SKIN_END_16         6   // SKIN_END_16 phase can't be 16

#define UINT8_CLIP(val, min, max) ((UINT8)((val>=max) ? max : ((val<=min) ? min : val)))  

UINT16 wAngularHue[BASE_COLOR_MAX]; // hue : 128 as 0
UINT16 wAngularSat[BASE_COLOR_MAX]; // sat : 128 as 1.0
UINT16 wAngularBri[BASE_COLOR_MAX]; // bri : 128 as 0

UINT8 bSCE_Y_Pos[BASE_COLOR_MAX] = {2, 5, 8, 10, 13, 15};
UINT8 bSCE_S_Pos[BASE_COLOR_MAX] = {2, 8, 16, 22, 24, 27};
UINT8 bSCE_H_Pos[BASE_COLOR_MAX] = {2, 8, 16, 22, 24, 27};

void vDrvSCEInterpolation(UINT16* inval, UINT8* inpos, UINT8 insize, UINT16* outval, UINT8 outsize)
{
    UINT32 i, j;
    INT32 diff;

    if ((inval == NULL) || (outval == NULL))
    {
        return;
    }
    
    for (i=0; i<insize; i++)
    {
        if (i == (insize-1))
        {
            diff = ((INT32)inval[0] - inval[i])/((inpos[0] + outsize - inpos[i]));
            
            for (j=inpos[i]; j<(inpos[0]+outsize); j++)
            {   
                if (j < outsize)
                {
                    outval[j] = inval[i]+diff*(j-inpos[i]);
                }
                else
                {
                    outval[j-outsize] = inval[i]+diff*(j-inpos[i]);
                }
            }
        }
        else
        {
            diff = ((INT32)inval[i+1] - inval[i])/((inpos[i+1] - inpos[i]));
            for (j=inpos[i]; j<inpos[i+1]; j++)
            {   
                outval[j] = inval[i]+diff*(j-inpos[i]);
            }
        }        
    }

    LOG(5, "input size %d \n", insize);
    for (i=0; i<insize; i++)
    {
        LOG(5, "[%d] = %d \n", inpos[i], inval[i]);
    }
    
    LOG(5, "output size %d \n", outsize);
    for (i=0; i<outsize; i++)
    {
        LOG(5, "[%d] = %d \n", i, outval[i]);
    }
}


void vDrvSetSixColorReg(void)
{
    vIO32Write4B(SIXCOLOR_REG_00, wAngularBri[0]|(wAngularBri[1]<<8)|
                                (wAngularBri[2]<<16)|(wAngularBri[3]<<24));
    vIO32Write4B(SIXCOLOR_REG_01, wAngularBri[4]|(wAngularBri[5]<<8)|
                                (wAngularSat[0]<<16)|(wAngularSat[1]<<24));
    vIO32Write4B(SIXCOLOR_REG_02, wAngularSat[2]|(wAngularSat[3]<<8)|
                                (wAngularSat[4]<<16)|(wAngularSat[5]<<24));
    vIO32Write4B(SIXCOLOR_REG_03, wAngularHue[0]|(wAngularHue[1]<<8)|
                                (wAngularHue[2]<<16)|(wAngularHue[3]<<24));
    vIO32Write4B(SIXCOLOR_REG_04, wAngularHue[4]|(wAngularHue[5]<<8));
    vIO32WriteFldAlign(ADAPTIVE_SCE_REG, SV_ON, ADAPTIVE_SCE_FORCE_LOAD);

    LOG(5, "\n=== Six Color ====\n", wAngularBri[0], wAngularBri[1], 
        wAngularBri[2], wAngularBri[3], wAngularBri[4], wAngularBri[5]);
    LOG(5, "Six Color Y %d %d %d %d %d %d\n", wAngularBri[0], wAngularBri[1], 
        wAngularBri[2], wAngularBri[3], wAngularBri[4], wAngularBri[5]);
    LOG(5, "Six Color S %d %d %d %d %d %d\n", wAngularSat[0], wAngularSat[1], 
        wAngularSat[2], wAngularSat[3], wAngularSat[4], wAngularSat[5]);
    LOG(5, "Six Color H %d %d %d %d %d %d\n", wAngularHue[0], wAngularHue[1], 
        wAngularHue[2], wAngularHue[3], wAngularHue[4], wAngularHue[5]);    
}

void vDrvGetSixColorReg(void)
{
    wAngularBri[0] = IO32ReadFldAlign(SIXCOLOR_REG_00, SIXCOLOR_Y_M);
    wAngularBri[1] = IO32ReadFldAlign(SIXCOLOR_REG_00, SIXCOLOR_Y_R);
    wAngularBri[2] = IO32ReadFldAlign(SIXCOLOR_REG_00, SIXCOLOR_Y_Y);
    wAngularBri[3] = IO32ReadFldAlign(SIXCOLOR_REG_00, SIXCOLOR_Y_G);
    wAngularBri[4] = IO32ReadFldAlign(SIXCOLOR_REG_01, SIXCOLOR_Y_C);
    wAngularBri[5] = IO32ReadFldAlign(SIXCOLOR_REG_01, SIXCOLOR_Y_B);
    wAngularSat[0] = IO32ReadFldAlign(SIXCOLOR_REG_01, SIXCOLOR_S_M);
    wAngularSat[1] = IO32ReadFldAlign(SIXCOLOR_REG_01, SIXCOLOR_S_R);
    wAngularSat[2] = IO32ReadFldAlign(SIXCOLOR_REG_02, SIXCOLOR_S_Y);
    wAngularSat[3] = IO32ReadFldAlign(SIXCOLOR_REG_02, SIXCOLOR_S_G);
    wAngularSat[4] = IO32ReadFldAlign(SIXCOLOR_REG_02, SIXCOLOR_S_C);
    wAngularSat[5] = IO32ReadFldAlign(SIXCOLOR_REG_02, SIXCOLOR_S_B);
    wAngularHue[0] = IO32ReadFldAlign(SIXCOLOR_REG_03, SIXCOLOR_H_M);
    wAngularHue[1] = IO32ReadFldAlign(SIXCOLOR_REG_03, SIXCOLOR_H_R);
    wAngularHue[2] = IO32ReadFldAlign(SIXCOLOR_REG_03, SIXCOLOR_H_Y);
    wAngularHue[3] = IO32ReadFldAlign(SIXCOLOR_REG_03, SIXCOLOR_H_G);
    wAngularHue[4] = IO32ReadFldAlign(SIXCOLOR_REG_04, SIXCOLOR_H_C);
    wAngularHue[5] = IO32ReadFldAlign(SIXCOLOR_REG_04, SIXCOLOR_H_B);
}

void vDrvMainSCEAngularReset(void)
{
	UINT16 i;

	for (i = 0; i < BASE_COLOR_MAX; i++)
	{
		wAngularHue[i] = 0x80;
		wAngularSat[i] = 0x80;
		wAngularBri[i] = 0x80;
	}

	vDrvSetSixColorReg();
}

void vDrvMainSCEAngularSetHueVal(UINT8 bColor, UINT8 bHueOffset)
{
    if (bColor >= BASE_COLOR_MAX)
    {
        return;
    }
    
	wAngularHue[bColor] = bHueOffset;
	vDrvSetSixColorReg();
}

void vDrvMainSCEAngularSetSatVal(UINT8 bColor, UINT8 bSatGain)
{
    if (bColor >= BASE_COLOR_MAX)
    {
        return;
    }
    
	wAngularSat[bColor] = bSatGain;
	vDrvSetSixColorReg();
}

void vDrvMainSCEAngularSetBriVal(UINT8 bColor, UINT8 bBriOffset)
{
    if (bColor >= BASE_COLOR_MAX)
    {
        return;
    }
    
	wAngularBri[bColor] = bBriOffset;
	vDrvSetSixColorReg();
}

/*****************************************************************************************/
/************************************ CHROMA BOOST ***************************************/
/*****************************************************************************************/
void vDrvChromaBoostParam(void)
{
#if SUPPORT_NEW_C_BOOST   
    // should only take effect when chroma boost is on
    // therefore shares the same setting with cboost_onoff
    vRegWriteFldAlign(C_BOOST, wReadQualityTable(QUALITY_CBOOST_ONOFF), NEW_BOOST_EN);  

	vRegWriteFldAlign(C_BOOST, wReadQualityTable(QUALITY_CBOOST_ONOFF), C_BOOST_ENABLE);
	vRegWriteFldAlign(C_BOOST, wReadQualityTable(QUALITY_CBOOST_GAIN), BOOST_GAIN);
	vRegWriteFldAlign(C_BOOST, wReadQualityTable(QUALITY_CBOOST_LMT_U), NEW_BOOST_LMT_U);
	vRegWriteFldAlign(C_BOOST, wReadQualityTable(QUALITY_CBOOST_LMT_L), NEW_BOOST_LMT_L);
#else
	vRegWriteFldAlign(C_BOOST, wReadQualityTable(QUALITY_CBOOST_ONOFF), C_BOOST_ENABLE);
	vRegWriteFldAlign(C_BOOST, wReadQualityTable(QUALITY_CBOOST_MINUS_EN), MINUS_BOOST_EN);
	vRegWriteFldAlign(C_BOOST, wReadQualityTable(QUALITY_CBOOST_GAIN), BOOST_GAIN);
	vRegWriteFldAlign(C_BOOST, wReadQualityTable(QUALITY_CBOOST_RNG_SEL), RANGE_SEL);
#endif	
} 

/*****************************************************************************************/
/**********************************   CONTRAST   ***************************************/
/*****************************************************************************************/
/**
 * @brief set main/pip path contrast value
 * @param bValue contrast value
 */
void vDrvSetContrast(UINT8 bPath, UINT16 u2Value)
{
    UINT32 u4Addr = GLOBAL_ADJ_02 + ((bPath == SV_VP_MAIN) ? 0 : 0x4);
    vIO32WriteFldAlign(u4Addr, u2Value, CONT_DUMMY);
}

/*****************************************************************************************/
/**********************************   BRIGHTNESS   ***************************************/
/*****************************************************************************************/
/**
 * @brief set main/pip path brightness value
 * @param bValue brightness value
 */
void vDrvSetBrightness(UINT8 bPath, UINT16 wValue)
{
    UINT32 u4Addr = GLOBAL_ADJ_00 + ((bPath == SV_VP_MAIN) ? 0 : 0x4);
    vIO32WriteFldAlign(u4Addr, wValue, BRIGHT_DUMMY);
}

/*****************************************************************************************/
/**********************************   SATURATION   ***************************************/
/*****************************************************************************************/
/**
 * @brief set main/pip path saturation value
 * @param bValue saturation value
 */
void vDrvSetSaturation(UINT8 bPath, UINT16 u2Value)
{
    UINT32 u4Addr = GLOBAL_ADJ_02 + ((bPath == SV_VP_MAIN) ? 0 : 0x4);
    vIO32WriteFldAlign(u4Addr, u2Value, SAT_DUMMY);
}

/*****************************************************************************************/
/*************************************   HUE   *******************************************/
/*****************************************************************************************/
/**
 * @brief set main/pip path hue value
 * @param bValue hue value degree
 */
void vDrvSetHue(UINT8 bPath, UINT16 wValue)
{
    UINT32 u4Addr = GLOBAL_ADJ_00 + ((bPath == SV_VP_MAIN) ? 0 : 0x4);
    vIO32WriteFldAlign(u4Addr, wValue, HUE_DUMMY);
}

/*****************************************************************************************/
/*************************************   SCE   *******************************************/
/*****************************************************************************************/
void vDrvMainSCEOnOff(UINT8 bOnOff)
{
	UINT16 bi;
	UINT16 wOffset = 0;

	for (bi = 0; bi < 16; bi++)
	{
		if (bOnOff)
		{
			vRegWrite1B(Y_SLOPE_1_0_MAIN + wOffset, READ_SCE_TBL(0,bi)); //Y_Slope
		}
		else
		{
			vRegWrite1B(Y_SLOPE_1_0_MAIN + wOffset, 0x80);   //Y_Slope
		}
		wOffset += 2;
	}
	vRegWriteFldAlign(SCE_CFG_MAIN, (bOnOff == SV_ON) ? 0x00 : 1, SEBP);
	vRegWriteFldAlign(SCE_CFG_MAIN, (bOnOff == SV_ON) ? 0x00 : 1, HEBP);

	if (bOnOff == SV_ON)
	{
        vRegWriteFldAlign(SCE_CFG_MAIN, ~bOnOff, ALLBP);
	}
}

void vDrvSCEAllOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(SCE_CFG_MAIN, ~bOnOff, ALLBP);
}

void vDrvSCEPatGen(UINT8 bMode, UINT16 Ydata)
{
	if (bMode == 0)
	{
		vRegWriteFldAlign(DBG_CFG, SV_OFF, INK_EN);
	}
	else
	{
		vRegWriteFldAlign(DBG_CFG, SV_ON, INK_EN);

		vRegWriteFldAlign(DBG_CFG, 1, Y_REPLACE);
		vRegWriteFldAlign(DBG_CFG, 1, CB_REPLACE);
		vRegWriteFldAlign(DBG_CFG, 1, CR_REPLACE);

		vRegWriteFldAlign(DBG_CFG, bMode, Y_INK_MODE);
		vRegWriteFldAlign(DBG_CFG, bMode, CB_INK_MODE);
		vRegWriteFldAlign(DBG_CFG, bMode, CR_INK_MODE);

		vRegWriteFldAlign(INK_DATA_MAIN, Ydata, INK_DATA_Y);
		vRegWriteFldAlign(INK_DATA_MAIN, 0x200, INK_DATA_CB);
		vRegWriteFldAlign(INK_DATA_MAIN_CR, 0x200, INK_DATA_CR);
	}
}

void vDrvSetSCELevel(UINT8 bLumaGain, UINT8 bSatGain, UINT8 bHueGain)
{
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG, bLumaGain, ADAPTIVE_SCE_L);
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG, bSatGain, ADAPTIVE_SCE_S);
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG, bHueGain, ADAPTIVE_SCE_H);
    vIO32WriteFldAlign(ADAPTIVE_SCE_REG, SV_ON, ADAPTIVE_SCE_FORCE_LOAD);
}

void vDrvSetSkinLevel(UINT8 bLumaGain, UINT8 bSatGain, UINT8 bHueGain)
{
	vIO32WriteFldAlign(ADAPTIVE_SKIN_REG, bLumaGain, ADAPTIVE_SKIN_L);
	vIO32WriteFldAlign(ADAPTIVE_SKIN_REG, bSatGain, ADAPTIVE_SKIN_S);
	vIO32WriteFldAlign(ADAPTIVE_SKIN_REG, bHueGain, ADAPTIVE_SKIN_H);
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG, SV_ON, ADAPTIVE_SCE_FORCE_LOAD);
}

void vDrvSCECalculate(void)
{
	UINT32 i, j, u4Idx;
	INT32 i4Tmp;
    UINT16 wSceYTable[SCE_LUMA_NUM];
    UINT16 wSceSTable[SCE_SAT_NUM];
    UINT16 wSceHTable[SCE_HUE_NUM];
    
    vDrvGetSixColorReg();
    
    vDrvSCEInterpolation(wAngularBri, bSCE_Y_Pos, BASE_COLOR_MAX, wSceYTable, SCE_LUMA_NUM);
    vDrvSCEInterpolation(wAngularSat, bSCE_S_Pos, BASE_COLOR_MAX, wSceSTable, SCE_SAT_NUM);
    vDrvSCEInterpolation(wAngularHue, bSCE_H_Pos, BASE_COLOR_MAX, wSceHTable, SCE_HUE_NUM);

    LOG(5, "\n bSceGain[0] = %d  bSkinTable[0] %d\n", bSceGain[0], bSkinTable[0]);
	for (i = 0; i < SCE_LUMA_NUM; i++)
	{   	    
        i4Tmp = ((((INT32)BASE_SCE_TBL[i]-MIN_SCE_TBL[i])*bSceGain[0])/128)+MIN_SCE_TBL[i]; 	

        // Six color 
        i4Tmp = wSceYTable[i]+i4Tmp-0x80;

        // Skin 
        if ((i >= SKIN_START_16) && (i <= SKIN_END_16))
        {
            i4Tmp += ((INT32)bSkinTable[0]-0x80);
        }    
        else if ((i == (SKIN_START_16-1)) || (i == (SKIN_END_16+1)))
        {
            i4Tmp += ((INT32)bSkinTable[0]-0x80)/2;
        }
        
        LOG(5, "[%d]  %d   %d  => %d \n", i, BASE_SCE_TBL[i], MIN_SCE_TBL[i], i4Tmp);
        
	    DEFAULT_SCE_TBL[i] = UINT8_CLIP(i4Tmp, 1, 255);
	}

	LOG(5, "\n bSceGain[1] = %d  bSkinTable[1] = %d \n", bSceGain[1], bSkinTable[1]);  
    for (j=ENUM_S_GAIN1; j<ENUM_S_GAIN3+1; j++)
    {
    	for (i = 0; i < SCE_SAT_NUM; i++)
        {
            u4Idx = j*SCE_SAT_NUM+i;
            i4Tmp = ((((INT32)BASE_SCE_TBL[u4Idx]-MIN_SCE_TBL[u4Idx])*bSceGain[1])/128)+MIN_SCE_TBL[u4Idx]; 
            
            // Six color 
            i4Tmp = i4Tmp*wSceSTable[i]/128;

            // Skin 
            if ((i >= SKIN_START_28) && (i <= SKIN_END_28))
            {
                i4Tmp = (i4Tmp*bSkinTable[1])>>7;
            }
            else if ((i == (SKIN_START_28-1)) ||  (i == (SKIN_END_28+1)))
            {
                i4Tmp = (i4Tmp*(((INT32)bSkinTable[1]-0x80)/2+0x80))>>7;
            }
            
            LOG(5, "[%d]  %d   %d  => %d \n", i, BASE_SCE_TBL[u4Idx], MIN_SCE_TBL[u4Idx], i4Tmp);
            
            DEFAULT_SCE_TBL[u4Idx] = UINT8_CLIP(i4Tmp, 0, 255);
        }
    }

    LOG(5, "\n bSceGain[2] = %d  bSkinTable[2] = %d\n", bSceGain[2], bSkinTable[2]);  
    for (j=ENUM_H_FTN; j<ENUM_END; j++)
    {        
    	for (i = 0; i < SCE_HUE_NUM; i++)
        {
            u4Idx = j*SCE_HUE_NUM+i;
            i4Tmp = ((((INT32)BASE_SCE_TBL[u4Idx]-MIN_SCE_TBL[u4Idx])*bSceGain[2])/128)+MIN_SCE_TBL[u4Idx]; 

            // Six color and Skin only works on Hue primary effect
            if (j == ENUM_H_FTN) 
            {                
                // Six color 
                i4Tmp = wSceHTable[i]+i4Tmp-0x80;

                // Skin
                if ((i >= SKIN_START_28) && (i <= SKIN_END_28))
                {
                    i4Tmp += ((INT32)bSkinTable[2]-0x80);
                }
                else if ((i == (SKIN_START_28-1)) ||  (i == (SKIN_END_28+1)))
                {
                    i4Tmp += ((INT32)bSkinTable[2]-0x80)/2;
                }
            }

            LOG(5, "[%d]  %d   %d  => %d \n", i, BASE_SCE_TBL[u4Idx], MIN_SCE_TBL[u4Idx], i4Tmp);
            DEFAULT_SCE_TBL[u4Idx] = UINT8_CLIP(i4Tmp, 1, 255);
        }
    }
}

/**
 * @brief load SCE table
 * @param pdwTable SCE table
 */
void vDrvSCELoadTable(UINT8 bPath)
{
	UINT8 i;	

	for (i = 0; i < 8; i++)
	{
		// y slope
		vRegWrite4B(Y_SLOPE_1_0_MAIN + 4 * i,
		            ((UINT32)READ_SCE_TBL(ENUM_Y_SLOPE,2*i)) |
		            ((UINT32)READ_SCE_TBL(ENUM_Y_SLOPE,2*i+1)<<16));
	}

    //ccmp
    vRegWrite4B(CCMP_1, (READ_SCE_TBL(ENUM_Y_SLOPE, 16)) | 
                                        (READ_SCE_TBL(ENUM_Y_SLOPE, 17) << 8) | 
                                        (READ_SCE_TBL(ENUM_Y_SLOPE, 18) << 16) |
                                        (READ_SCE_TBL(ENUM_Y_SLOPE, 19) << 24));
    vRegWrite4B(CCMP_2, (READ_SCE_TBL(ENUM_Y_SLOPE, 20)) | 
                                        (READ_SCE_TBL(ENUM_Y_SLOPE, 21) << 8) | 
                                        (READ_SCE_TBL(ENUM_Y_SLOPE, 22) << 16) |
                                        (READ_SCE_TBL(ENUM_Y_SLOPE, 23) << 24));
    vRegWrite1B(CCMP_3, READ_SCE_TBL(ENUM_Y_SLOPE, 24));

	for (i = 0; i < 28; i++)
	{
		// saturation related function
		vRegWrite4B(LOCAL_SAT_1, 0x30000 + i);

		vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1,    i) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_P2,    i))<<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_GAIN1, i))<<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_GAIN2, i))<<24));

		vRegWrite4B(LOCAL_SAT_3, (UINT32)READ_SCE_TBL(ENUM_S_GAIN3, i) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y000,  i))<<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y064,  i))<<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y128,  i))<<24));

		vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192,  i) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y256,  i))<<8));

		vRegWrite4B(LOCAL_SAT_1, 0x10000 + i);

		// hue related function
		vRegWrite4B(LOCAL_HUE_1, 0x30000 + i);

		vRegWrite4B(LOCAL_HUE_2, (UINT32)READ_SCE_TBL(ENUM_H_FTN, i) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y000, i)) <<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y064, i)) <<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y128, i)) <<24));
		vRegWrite4B(LOCAL_HUE_3, (UINT32)READ_SCE_TBL(ENUM_H_Y192, i)  +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y256, i)) <<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S000, i)) <<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S032, i)) <<24));
		vRegWrite4B(LOCAL_HUE_4, (UINT32)READ_SCE_TBL(ENUM_H_S064, i)  +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S096, i)) <<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S128, i)) <<16));

		vRegWrite4B(LOCAL_HUE_1, 0x10000 + i);
	}
}

UINT32 vDrvSCELoadIdx(UINT32 u4FunIdx, UINT32 u4HueIdx, UINT32 u4Value)
{
    if ((u4FunIdx >= ENUM_END) || (u4HueIdx >= 28))
    {
        LOG(0, "SCE index out of range: function:%d, hue:%d\n", u4FunIdx, u4HueIdx);
        return SV_FAIL;
    }

    WRITE_SCE_TBL(u4FunIdx, u4HueIdx, (UINT8)(u4Value & 0xFF));

    if (u4FunIdx == ENUM_Y_SLOPE)
    {
        UINT32 u4Temp;

        if (u4HueIdx <= 15)
        {
            u4Temp = (u4HueIdx >> 1);
            vRegWrite4B(Y_SLOPE_1_0_MAIN + 4 * u4Temp,
                        ((UINT32)READ_SCE_TBL(ENUM_Y_SLOPE, 2 * u4Temp)) |
                        ((UINT32)READ_SCE_TBL(ENUM_Y_SLOPE, 2 * u4Temp + 1) << 16));
        }
        else if ((u4HueIdx >= 16) && (u4HueIdx <= 24))
        {
            UINT32 u4Temp = 4 * ((u4HueIdx-16) >> 2);
            vRegWrite4B(CCMP_1 + u4Temp,
                        (READ_SCE_TBL(ENUM_Y_SLOPE, 16 + u4Temp)) | 
                        (READ_SCE_TBL(ENUM_Y_SLOPE, 17 + u4Temp) << 8) | 
                        (READ_SCE_TBL(ENUM_Y_SLOPE, 18 + u4Temp) << 16) |
                        (READ_SCE_TBL(ENUM_Y_SLOPE, 19 + u4Temp) << 24));
        }
        else
        {
            LOG(0, "SCE index out of range: function:%d, hue:%d\n", u4FunIdx, u4HueIdx);
            return SV_FAIL;
        }
    }
    else if ((u4FunIdx >= ENUM_S_GAIN1) && (u4FunIdx <= ENUM_S_Y256))
    {
		// saturation related function
		vRegWrite4B(LOCAL_SAT_1, 0x30000 + u4HueIdx);

		vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1, u4HueIdx) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_P2, u4HueIdx))<<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_GAIN1, u4HueIdx))<<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_GAIN2, u4HueIdx))<<24));

		vRegWrite4B(LOCAL_SAT_3, (UINT32)READ_SCE_TBL(ENUM_S_GAIN3, u4HueIdx) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y000, u4HueIdx))<<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y064, u4HueIdx))<<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y128, u4HueIdx))<<24));

		vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192, u4HueIdx) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y256, u4HueIdx))<<8));

		vRegWrite4B(LOCAL_SAT_1, 0x10000 + u4HueIdx);
    }
    else
    {
		// hue related function
		vRegWrite4B(LOCAL_HUE_1, 0x30000 + u4HueIdx);

		vRegWrite4B(LOCAL_HUE_2, (UINT32)READ_SCE_TBL(ENUM_H_FTN, u4HueIdx) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y000, u4HueIdx)) <<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y064, u4HueIdx)) <<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y128, u4HueIdx)) <<24));
		vRegWrite4B(LOCAL_HUE_3, (UINT32)READ_SCE_TBL(ENUM_H_Y192, u4HueIdx)  +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y256, u4HueIdx)) <<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S000, u4HueIdx)) <<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S032, u4HueIdx)) <<24));
		vRegWrite4B(LOCAL_HUE_4, (UINT32)READ_SCE_TBL(ENUM_H_S064, u4HueIdx)  +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S096, u4HueIdx)) <<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S128, u4HueIdx)) <<16));

		vRegWrite4B(LOCAL_HUE_1, 0x10000 + u4HueIdx);
    }
 
    return SV_TRUE;
}



BOOL bDrvSCEVerifyTable()
{
    UINT8 i;
	UINT32 u4LocalSat_2_Read, u4LocalSat_2_Write;
	UINT32 u4LocalSat_3_Read, u4LocalSat_3_Write;
	UINT32 u4LocalSat_4_Read, u4LocalSat_4_Write;
	UINT32 u4LocalHue_2_Read, u4LocalHue_2_Write;
	UINT32 u4LocalHue_3_Read, u4LocalHue_3_Write;
    UINT32 u4LocalHue_4_Read, u4LocalHue_4_Write;    
    
	// 2. sce saturation
	vRegWrite4B(LOCAL_SAT_1, 0x30000);
	vRegWrite4B(LOCAL_HUE_1, 0x30000);

	for (i = 0; i < 28; i++)
	{
		u4LocalSat_2_Read = u4RegRead4B(LOCAL_SAT_2);		
        u4LocalSat_2_Write = READ_SCE_TBL(ENUM_S_P1,    i) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_P2,    i))<<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_GAIN1, i))<<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_GAIN2, i))<<24);
        if(u4LocalSat_2_Read != u4LocalSat_2_Write)
        {
            return SV_FALSE;
        }
        
		u4LocalSat_3_Read = u4RegRead4B(LOCAL_SAT_3);
        u4LocalSat_3_Write = (UINT32)READ_SCE_TBL(ENUM_S_GAIN3, i) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y000,  i))<<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y064,  i))<<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y128,  i))<<24);
        if(u4LocalSat_3_Read != u4LocalSat_3_Write)
        {
            return SV_FALSE;
        }
        
        u4LocalSat_4_Read = u4RegRead4B(LOCAL_SAT_4);
        u4LocalSat_4_Write = READ_SCE_TBL(ENUM_S_Y192,  i) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y256,  i))<<8);
        if(u4LocalSat_4_Read != u4LocalSat_4_Write)
        {
            return SV_FALSE;
        }
        
        u4LocalHue_2_Read = u4RegRead4B(LOCAL_HUE_2);
        u4LocalHue_2_Write = (UINT32)READ_SCE_TBL(ENUM_H_FTN, i) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y000, i)) <<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y064, i)) <<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y128, i)) <<24);
        if(u4LocalHue_2_Read != u4LocalHue_2_Write)
        {
            return SV_FALSE;
        }
        
		u4LocalHue_3_Read = u4RegRead4B(LOCAL_HUE_3);
        u4LocalHue_3_Write = (UINT32)READ_SCE_TBL(ENUM_H_Y192, i)  +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y256, i)) <<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S000, i)) <<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S032, i)) <<24);
        if(u4LocalHue_3_Read != u4LocalHue_3_Write)
        {
            return SV_FALSE;
        }
        
		u4LocalHue_4_Read = u4RegRead4B(LOCAL_HUE_4);
        u4LocalHue_4_Write = (UINT32)READ_SCE_TBL(ENUM_H_S064, i)  +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S096, i)) <<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S128, i)) <<16);
        if(u4LocalHue_4_Read != u4LocalHue_4_Write)
        {
            return SV_FALSE;
        }
        
	}
	vRegWrite4B(LOCAL_SAT_1, 0x10000);
	vRegWrite4B(LOCAL_HUE_1, 0x10000);

	return SV_TRUE;
}

void vDrvSCEInit(UINT8 bPath)
{
    UINT8 i;

    vRegWriteFldAlign(LUMA_ADJ, 0x40, Y_LEV_ADJ);
    vDrvSCEAllOnOff(SV_ON);
    vDrvMainSCEAngularReset();
    vDrvChromaBoostParam();

	if (bPath == SV_VP_MAIN)
	{
        // Global function
        vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 0x400, BRIGHT);
        vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 0x80, CONT);
        vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 0x200, HUE);
        vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 0x80, SAT);

		// y function (for adaptive luma)
		for (i = 0; i < 16; i++)
		{
			vRegWrite4B(Y_FTN_1_0_MAIN+ 4 * i,
			            (((UINT32)i * 2) << 5) +
			            ((UINT32)(((UINT32)i * 2 + 1) << 5) << 16));
		}
		vRegWriteFldAlign(Y_FTN_32_MAIN, 0x3FF, Y_FTN_32);

		// y slope
		for (i = 0; i < 8; i++)
		{
			vRegWriteFldMulti(Y_SLOPE_1_0_MAIN + 4 * i,
			                  P_Fld(0x80, YSlope0) |        // initialized as 0.0 (128)
			                  P_Fld(0x80, YSlope1));
		}

        //ccmp
        vRegWriteFldAlign(SCE_CFG_MAIN, 0, CCMP_BP);
        vRegWrite4B(CCMP_1, 0x80808080);
        vRegWrite4B(CCMP_2, 0x80808080);
        vRegWrite1B(CCMP_3, 0x80);

		for (i = 0; i < 28; i++)
		{
			// saturation related function
			vRegWrite4B(LOCAL_SAT_1, 0x30000 + i);

			vRegWrite4B(LOCAL_SAT_2, 0x80804010);           // G2, G1, P2, P1
			vRegWrite4B(LOCAL_SAT_3, 0x80808080);           // SY128, SY64, SY0, G3
			vRegWrite4B(LOCAL_SAT_4, 0x00008080);           // SY255, SY192

			vRegWrite4B(LOCAL_SAT_1, 0x10000 + i);

			// hue related function
			vRegWrite4B(LOCAL_HUE_1, 0x30000 + i);

			vRegWrite4B(LOCAL_HUE_2, 0x80808080);           // HY128, HY64, HY0, HFTN
			vRegWrite4B(LOCAL_HUE_3, 0x80808080);           // HS32, HS00, HY255, HY 192
			vRegWrite4B(LOCAL_HUE_4, 0x00808080);           // HS128, HS96, HS64

			vRegWrite4B(LOCAL_HUE_1, 0x10000 + i);
		}
	}

#if SUPPORT_NEW_SAT_GAIN_Y
    // enable new saturation gain on y
    vRegWriteFldAlign(SCE_CFG_MAIN, 0x1, S_BY_Y_MODE);  
#endif
}

/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
#if SUPPORT_SEAMLESS_MODE

void vDrvColorDemo(UINT8 bOnOff, Region rRegion)
{
    if (bOnOff == SV_ON) 
    {
        //Set region or off
        vRegWriteFldAlign(DBG_CFG, ((rRegion.u1OnOff== SV_ON)?0:1), SPLIT_SWAP);
        //Define demo region
        vRegWriteFldAlign(WIN_X_MAIN, rRegion.wHStart, WIN_X_START);
        vRegWriteFldAlign(WIN_X_MAIN, rRegion.wHEnd, WIN_X_END);
        vRegWriteFldAlign(WIN_Y_MAIN, rRegion.wVStart, WIN_Y_START);
        vRegWriteFldAlign(WIN_Y_MAIN, rRegion.wVEnd, WIN_Y_END);
    }
    //Turn on/off demo mode
    vRegWriteFldAlign(DBG_CFG, (bOnOff==SV_ON?1:0), SPLIT_EN);
    vRegWriteFldAlign(0x781C, 0x80, AC_FULLB1); // Set Demo mode brightness in demo window.
}

#endif //SUPPORT_SEAMLESS_MODE

