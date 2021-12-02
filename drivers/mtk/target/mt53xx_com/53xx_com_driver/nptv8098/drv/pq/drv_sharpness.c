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
#include "hw_nr.h"
#include "vdo_misc.h"
#include "nptv_debug.h"
#include "drv_contrast.h"
#include "drv_meter.h"
#include "pe_if.h"
#include "drv_scaler.h"
// === HW INCLUDE =============================================================================
#include "hw_ycproc.h"
#include "hw_sw.h"
#include "hw_di_int.h"

// === DEFINE =============================================================================
#define CHROMACORING_OFFSET     0x200
#define HSHARP_OFFSET           0x00    //H Sharp has no sub path
// === FUNCTION PROTOTYPE =================================================================
void vDrvMoSharpInit(void);
void vDrvBNRSharpInit(void);
void vDrvSharp3DInit(void);

// === EXTERN FUNCTION ====================================================================
#ifdef CC_MT5396
EXTERN  UINT8 u1DrvMJCGetGmvX(void); // integer pixel
EXTERN  UINT8 u1DrvMJCGetGmvY(void); // integer pixel
#endif
// === GLOBAL VARIABLE ====================================================================

// === EXTERN VARIABLE ====================================================================

static BOOL fgAdapYLEV = SV_ON;
static UINT16 TDS_ADAP_YLEV[17];

UINT8 TDS_YLEV[17] =
{
	0x40,0x4D,0x5A,0x66,0x73,0x76,0x7A,0x7D,
	0x80,0x80,0x80,0x80,0x80,0x7D,0x7A,0x76,
	0x72  
};

UINT8 TDS_YLEV_APL[17] =
{
       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
       0x80       
};

#if defined(CC_MT5396) || defined(CC_MT5368)
static RRegDwrdTbl const REGTBL_HSHARP[] =
{
	{       HSHARP_MAIN_00, 0x804C4440, 0xF0FFFFFF},
	{       HSHARP_MAIN_01, 0x004C4440, 0x80FFFFFF},
	{       HSHARP_MAIN_02, 0x00020202, 0x70FFFFFF},
	{       HSHARP_MAIN_03, 0x00FFFFFF, 0x70FFFFFF},
	{       HSHARP_MAIN_04, 0x00FFFFFF, 0x70FFFFFF},
	{       HSHARP_MAIN_05, 0xF000FFFF, 0xFFC0FFFF},
	{       HSHARP_MAIN_06, 0x30000808, 0xF3FFFFFF},
	{       HSHARP_MAIN_07, 0x00444340, 0x00FFFFFF},
	{       HSHARP_MAIN_08, 0x00444340, 0x00FFFFFF},
	{       HSHARP_MAIN_09, 0x00010101, 0x70FFFFFF},
	{       HSHARP_MAIN_0A, 0x00FFFFFF, 0x70FFFFFF},
	{       HSHARP_MAIN_0B, 0x00FFFFFF, 0x70FFFFFF},
	{       HSHARP_MAIN_0E, 0x00000000, 0xFFFFFF00},
	{       HSHARP_MAIN_0F, 0x00000000, 0xFFFFFF0F},
	{       HSHARP_MAIN_10, 0x00800000, 0x1FFF0000},
	{         REG_END, 0x00000000, 0x00000000},
};
#endif 	

static RRegDwrdTbl const REGTBL_TDSHARP[] =
{
	{        TDSHP_00, 0x80000408, 0x80000408},
	{        TDSHP_01, 0xA0810010, 0xBFFFFFFF},
	{        TDSHP_02, 0x20810110, 0x3FFFFFFF},
	{        TDSHP_03, 0xFFFF4010, 0xFFFFFFFF},
	{        TDSHP_04, 0x40000000, 0xFF000000},
	{        TDSHP_05, 0xA300E800, 0xFFFFFFFF},
	{        TDSHP_06, 0x0A500A0A, 0x0FFFFFFF},
	{        TDSHP_07, 0x00FFFA10, 0x00FFFFBF},
	{        TDSHP_08, 0x02020002, 0xFFFFFFFF},
	{        TDSHP_09, 0x02000202, 0xFFFFFFFF},
	{        TDSHP_0A, 0x02020200, 0xFFFFFF00},
	{        TDSHP_0B, 0x00000000, 0xFFFFFFFF},
	{        TDSHP_0C, 0x00000000, 0xFFF00000},
	{        TDSHP_0D, 0x00000000, 0xFFFFFFFF},
	{        TDSHP_0E, 0x00000000, 0xFFFFFFFF},
	{        TDSHP_0F, 0x00000000, 0xFFFFFF00},
	{        TDSHP_10, 0x40600040, 0xFFFFFFFF},
	{        TDSHP_11, 0x60004060, 0xFFFFFFFF},
	{        TDSHP_12, 0x40604000, 0xFFFFFF00},
	{        TDSHP_13, 0x20202010, 0xFFFFFFFF},
	{        TDSHP_14, 0x2020FF20, 0xFFFFFFFF},
	{        TDSHP_15, 0x10FFFF00, 0xFFFFFF00},
	{        TDSHP_16, 0x20202010, 0xFFFFFFFF},
	{        TDSHP_17, 0x2020FF20, 0xFFFFFFFF},
	{        TDSHP_18, 0x10FFFF00, 0xFFFFFF00},
	{        TDSHP_19, 0x44000000, 0xFFFFFFFF},
	{        TDSHP_1A, 0x44400000, 0xFFF00000},
	{        TDSHP_1B, 0x0405FF05, 0xFFFFFFFF},
	{        TDSHP_1C, 0x28282828, 0xFFFFFFFF},
	{        TDSHP_1D, 0x20FFFF00, 0xFFFFFF00},
	{        TDSHP_1E, 0x0405FF28, 0xFFFFFFFF},
	{        TDSHP_1F, 0x28288028, 0xFFFFFFFF},
	{        TDSHP_20, 0x20FFFF00, 0xFFFFFF00},
	{        TDSHP_21, 0xD0FFFF00, 0xDFFFFF00},
	{        TDSHP_22, 0xD0FFFF00, 0xDFFFFF00},
	{        TDSHP_23, 0x88060600, 0xDFFFFF00},
	{        TDSHP_24, 0x8A060600, 0xDFFFFF00},
	{        TDSHP_25, 0x90FFFF00, 0xDFFFFF00},
	{        TDSHP_26, 0x90FFFF00, 0xDFFFFF00},
	{        TDSHP_27, 0x00000206, 0xFFFFFFFF},
	{        TDSHP_28, 0x1818FFFF, 0xFFFFFFFF},
	{        TDSHP_29, 0xD000FF05, 0xDFFFFF0F},
	{        TDSHP_2A, 0x404A5C5D, 0xFFFFFFFF},
	{        TDSHP_2B, 0x6568787D, 0xFFFFFFFF},
	{        TDSHP_2C, 0x807E7C79, 0xFFFFFFFF},
	{        TDSHP_2D, 0x78808070, 0xFFFFFFFF},
	{        TDSHP_2E, 0x4C0F4000, 0xFF3FC00C}, 
	{        TDSHP_30, 0x40200001, 0x70FFF001},
	{        TDSHP_31, 0x0008DC21, 0x007FFFFF},
	{        TDSHP_32, 0x00159031, 0x007FFFFF},
	{        TDSHP_33, 0x00012BB6, 0x007FFFFF},
	{        TDSHP_34, 0x00000000, 0x007FFFFF},
	{        TDSHP_35, 0x00000000, 0x007FFFFF},
	{        TDSHP_36, 0x00000000, 0x007FFFFF},
	{        TDSHP_37, 0x00000000, 0x007FFFFF},
	{        TDSHP_38, 0x00000000, 0x007FFFFF},
	{         REG_END, 0x00000000, 0x00000000}, 
};


#if defined(CC_MT5396) || defined(CC_MT5368)    
static RRegDwrdTbl const REGTBL_PBC[] =
{
	{     PBC_SHP1_01, 0x9010A0FF, 0x9FFFFFFF},
	{     PBC_SHP1_02, 0xA4110019, 0xFFFFFFFF},
	{     PBC_SHP1_03, 0x01003220, 0xFFF8FF3F},
	{     PBC_SHP1_04, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP1_05, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP1_06, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP1_07, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP1_08, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP1_09, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP2_01, 0x901060FF, 0x9FFFFFFF},
	{     PBC_SHP2_02, 0x6411002A, 0xFFFFFFFF},
	{     PBC_SHP2_03, 0x0108200C, 0xFFF8FF3F},
	{     PBC_SHP2_04, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP2_05, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP2_06, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP2_07, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP2_08, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP2_09, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP3_01, 0x901070FF, 0x9FFFFFFF},
	{     PBC_SHP3_02, 0xEE110030, 0xFFFFFFFF},
	{     PBC_SHP3_03, 0x0108200C, 0xFFF8FF3F},
	{     PBC_SHP3_04, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP3_05, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP3_06, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP3_07, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP3_08, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP3_09, 0x80808080, 0xFFFFFFFF},
	{     PBC_CTRL_00, 0x48008000, 0xEFFFF000},
	{         REG_END, 0x00000000, 0x00000000},
};	
#else
static RRegDwrdTbl const REGTBL_PBC[] =
{
	{     PBC_SHP1_01, 0x1010A0FF, 0x9FFFFFFF},
	{     PBC_SHP1_02, 0xA4110019, 0xFFFFFFFF},
	{     PBC_SHP1_03, 0x01083220, 0xFFF8FF3F},
	{     PBC_SHP1_04, 0x01800000, 0xFFFF8000},
	{     PBC_SHP1_05, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP1_06, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP1_07, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP1_08, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP1_09, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP1_0A, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP2_01, 0x101060FF, 0x9FFFFFFF},
	{     PBC_SHP2_02, 0x6411002A, 0xFFFFFFFF},
	{     PBC_SHP2_03, 0x0108200C, 0xFFF8FF3F},
	{     PBC_SHP2_04, 0x01800000, 0xFFFF8000},
	{     PBC_SHP2_05, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP2_06, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP2_07, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP2_08, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP2_09, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP2_0A, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP3_01, 0x101070FF, 0x9FFFFFFF},
	{     PBC_SHP3_02, 0xEE11001C, 0xFFFFFFFF},
	{     PBC_SHP3_03, 0x0108200C, 0xFFF8FF3F},
	{     PBC_SHP3_04, 0x01800000, 0xFFFF8000},
	{     PBC_SHP3_05, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP3_06, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP3_07, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP3_08, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP3_09, 0x80808080, 0xFFFFFFFF},
	{     PBC_SHP3_0A, 0x80808080, 0xFFFFFFFF},
	{     PBC_CTRL_00, 0x48008208, 0xEFFFFFF8},
	{         REG_END, 0x00000000, 0x00000000},
};
#endif

#if defined(CC_MT5396) || defined(CC_MT5368)
static RRegDwrdTbl const REGTBL_ECTIF[] =
{	
	{         ECTIF_00, 0x00001A11, 0xD0001A79},																																					
	{         ECTIF_01, 0x00033714, 0x07777FFF},																																					
	{         ECTIF_02, 0x1A0C0A01, 0xFFFFFF07},																																					
	{         ECTIF_03, 0x1A0C0A01, 0xFFFFFF07},																																					
	{         ECTIF_04, 0x0C080041, 0xFFFFFF77},																																					
	{         ECTIF_05, 0x02000000, 0xFF000000},
	{         REG_END, 0x00000000, 0x00000000},
};
#endif

static RRegDwrdTbl const REGTBL_ECTIR[] =
{
	{         ECTI_00, 0x00001A61, 0x10001A79},
	{         ECTI_01, 0x00003714, 0x07777FFF},
	{         ECTI_02, 0x1A0C0A05, 0xFFFFFF07},
	{         ECTI_03, 0x1A0C0A05, 0xFFFFFF07},
	{         ECTI_04, 0x40000001, 0xFFFFFF77},
	{         ECTI_05, 0x0000000B, 0x000000FF},
	{         REG_END, 0x00000000, 0x00000000},
};

#if defined(CC_MT5396)
static RRegDwrdTbl const REGTBL_CBE[] = 
{
	{	    CBE_00, 0x80004040, 0x8F7F7F7F},
	{   	    CBE_01, 0x00000416, 0x00007F7F},
	{         CBE_02, 0x2020083F, 0x00007F7F},
	{	    REG_END, 0x00000000, 0x00000000}
};
#endif

// 3Darpness Initial Define
UINT16 w3DWeightingH = 0x80;
UINT16 w3DWeightingV = 0x80;
UINT8 bSharp3dEn = SV_FALSE;
// MoAdaptiveSharpness Initial Define
UINT8 bMoShpEn = SV_TRUE;

// BNRSharpness Initial Define
UINT8 bBNRShpEn = SV_TRUE;
UINT8 vBNRLevel = 0;


//9 tap for chroma in main path, luma/chroma in sub path
static INT16 const ANTI_FILTER[4][5] =
{
	//480i
	{0, 0, 0, 1, 127},          //{ -1, -2, 6, 34, 52},

	//480p
	{0, 0, 0, 1, 127},          //{ -1, 3, -8, 14, 112},

	//720p
	{0, 0, 0, 1, 127},          //{ -1, 2, -7, 11, 118},

	//1080i
	{0, 0, 0, 1, 127},          //{ -1, -2, 6, 34, 52}
};

// === END OF STATIC VARIABLE =============================================================
/*****************************************************************************************/
/*********************************   Anti-Aliasing Filter   *************************************/
/*****************************************************************************************/


void vDrvAntiAliasingFilterSet(UINT8 bPath, UINT8 bFilterIndex)
{
    if (bPath == SV_VP_MAIN)
    {
        //  MAIN Y1/C1
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][0] < 0)? 1 : 0, LPF_MAIN_SIGN_Y1);
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][0] < 0)? 1 : 0, LPF_MAIN_SIGN_C1);
		vRegWriteFldAlign(LPF_MAIN_01, (UINT8) ABS(ANTI_FILTER[bFilterIndex][0]), LPF_MAIN_COF_Y1);
		vRegWriteFldAlign(LPF_MAIN_00, (UINT8) ABS(ANTI_FILTER[bFilterIndex][0]), LPF_MAIN_COF_C1);

        //  MAIN Y2/C2
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][1] < 0)? 1 : 0, LPF_MAIN_SIGN_Y2);
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][1] < 0)? 1 : 0, LPF_MAIN_SIGN_C2);
		vRegWriteFldAlign(LPF_MAIN_01, (UINT8) ABS(ANTI_FILTER[bFilterIndex][1]), LPF_MAIN_COF_Y2);
		vRegWriteFldAlign(LPF_MAIN_00, (UINT8) ABS(ANTI_FILTER[bFilterIndex][1]), LPF_MAIN_COF_C2);

        //  MAIN Y3/C3
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][2] < 0)? 1 : 0, LPF_MAIN_SIGN_Y3);
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][2] < 0)? 1 : 0, LPF_MAIN_SIGN_C3);
		vRegWriteFldAlign(LPF_MAIN_01, (UINT8) ABS(ANTI_FILTER[bFilterIndex][2]), LPF_MAIN_COF_Y3);
		vRegWriteFldAlign(LPF_MAIN_00, (UINT8) ABS(ANTI_FILTER[bFilterIndex][2]), LPF_MAIN_COF_C3);

        //  MAIN Y4/C4
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][3] < 0)? 1 : 0, LPF_MAIN_SIGN_Y4);
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][3] < 0)? 1 : 0, LPF_MAIN_SIGN_C4);
		vRegWriteFldAlign(LPF_MAIN_01, (UINT8) ABS(ANTI_FILTER[bFilterIndex][3]), LPF_MAIN_COF_Y4);
		vRegWriteFldAlign(LPF_MAIN_00, (UINT8) ABS(ANTI_FILTER[bFilterIndex][3]), LPF_MAIN_COF_C4);

        //  MAIN Y5/C5
		vRegWriteFldAlign(LPF_MAIN_02, (UINT8) ABS(ANTI_FILTER[bFilterIndex][4]), LPF_MAIN_COF_Y5);
		vRegWriteFldAlign(LPF_MAIN_02, (UINT8) (ANTI_FILTER[bFilterIndex][4]), LPF_MAIN_COF_C5);
    }
}

void vDrvAntiAliasingFilterOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bPath == SV_VP_MAIN)
    {
		vRegWriteFldAlign(LPF_MAIN_02, bOnOff, LPF_MAIN_Y_EN);
		vRegWriteFldAlign(LPF_MAIN_02, bOnOff, LPF_MAIN_C_EN);
    }
    else
    {
		vRegWriteFldAlign(LPF_MAIN_02, bOnOff, LPF_PIP_Y_EN);
		vRegWriteFldAlign(LPF_MAIN_02, bOnOff, LPF_PIP_C_EN);
    }
}

/*****************************************************************************************/
/*************************************   CTI   ******************************************/
/*****************************************************************************************/
/**
 * @brief CTIF on/off
 * @param bOnOff SV_ON/SV_OFF
 */

void vDrvCTIFOnOff(UINT8 bOnOff)
{
#if defined(CC_MT5396) || defined(CC_MT5368)
	vRegWriteFldAlign(ECTIF_00, bOnOff, ECTIF_ENA);
#endif
}

/**
 * @brief set main path CTIF initial parameters
 */
void vDrvCTIFInit(void)
{
#if defined(CC_MT5396) || defined(CC_MT5368)
	vDrvLoadRegDwrdTbl(REGTBL_ECTIF);
#endif
}

/**
 * @brief set CTIF parameters
 * @param prCTIParam CTIF parameters structure
 */
void vDrvCTIFParam(void)
{
#if defined(CC_MT5396) || defined(CC_MT5368)
	vRegWriteFldAlign(ECTIF_00, wReadQualityTable(QUALITY_CTIF_ENA),
	                  ECTIF_ENA);
	vRegWriteFldAlign(ECTIF_00, wReadQualityTable(QUALITY_CTIF_HD),
	                  ECTIF_HD);
	vRegWriteFldAlign(ECTIF_01, wReadQualityTable(QUALITY_CTIF_LPF1),
	                  ECTIF_LPF1);
	vRegWriteFldAlign(ECTIF_01, wReadQualityTable(QUALITY_CTIF_LPF1_SEL),
	                  ECTIF_LPF1_SEL);
	vRegWriteFldAlign(ECTIF_00, wReadQualityTable(QUALITY_CTIF_FLPF),
	                  ECTIF_FLPF);
	vRegWriteFldAlign(ECTIF_00, wReadQualityTable(QUALITY_CTIF_FLPF_SEL),
	                  ECTIF_FLPF_SEL);
	vRegWriteFldAlign(ECTIF_04, wReadQualityTable(QUALITY_CTIF_FLAT_GAIN),
	                  ECTIF_FLAT_GAIN);
	vRegWriteFldAlign(ECTIF_04, wReadQualityTable(QUALITY_CTIF_FLAT_OFST1),
	                  ECTIF_FLAT_OFST1);
	vRegWriteFldAlign(ECTIF_04, wReadQualityTable(QUALITY_CTIF_FLAT_OFST2),
	                  ECTIF_FLAT_OFST2);
	vRegWriteFldAlign(ECTIF_02, wReadQualityTable(QUALITY_CTIF_U_WND_SZ),
	                  ECTIF_U_WND_SZ);
	vRegWriteFldAlign(ECTIF_03, wReadQualityTable(QUALITY_CTIF_V_WND_SZ),
	                  ECTIF_V_WND_SZ);
	vRegWriteFldAlign(ECTIF_02, wReadQualityTable(QUALITY_CTIF_U_STB_GAIN),
	                  ECTIF_U_STB_GAIN);
	vRegWriteFldAlign(ECTIF_03, wReadQualityTable(QUALITY_CTIF_V_STB_GAIN),
	                  ECTIF_V_STB_GAIN);
	vRegWriteFldAlign(ECTIF_02, wReadQualityTable(QUALITY_CTIF_U_STB_OFST1),
	                  ECTIF_U_STB_OFST1);
	vRegWriteFldAlign(ECTIF_03, wReadQualityTable(QUALITY_CTIF_V_STB_OFST1),
	                  ECTIF_V_STB_OFST1);
	vRegWriteFldAlign(ECTIF_02, wReadQualityTable(QUALITY_CTIF_U_STB_OFST2),
	                  ECTIF_U_STB_OFST2);
	vRegWriteFldAlign(ECTIF_03, wReadQualityTable(QUALITY_CTIF_V_STB_OFST2),
	                  ECTIF_V_STB_OFST2);
	vRegWriteFldAlign(ECTIF_05, wReadQualityTable(QUALITY_CTIF_COR),
	                  ECTIF_COR);
#endif	                  
}

/**
 * @brief CTIR on/off
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvCTIROnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(ECTI_00, bOnOff, ECTI_ENA);
}

/**
 * @brief set main path CTIF initial parameters
 */
void vDrvCTIRInit(void)
{
	vDrvLoadRegDwrdTbl(REGTBL_ECTIR);
}

/**
 * @brief set CTIR parameters
 * @param prCTIParam CTIF parameters structure
 */
void vDrvCTIRParam(void)
{

	vRegWriteFldAlign(ECTI_00, wReadQualityTable(QUALITY_CTIR_ENA),
	                  ECTI_ENA);
	vRegWriteFldAlign(ECTI_00, wReadQualityTable(QUALITY_CTIR_HD),
	                  ECTI_HD);
	vRegWriteFldAlign(ECTI_01, wReadQualityTable(QUALITY_CTIR_LPF1),
	                  ECTI_LPF1);
	vRegWriteFldAlign(ECTI_01, wReadQualityTable(QUALITY_CTIR_LPF1_SEL),
	                  ECTI_LPF1_SEL);
	vRegWriteFldAlign(ECTI_00, wReadQualityTable(QUALITY_CTIR_FLPF),
	                  ECTI_FLPF);
	vRegWriteFldAlign(ECTI_00, wReadQualityTable(QUALITY_CTIR_FLPF_SEL),
	                  ECTI_FLPF_SEL);
	vRegWriteFldAlign(ECTI_04, wReadQualityTable(QUALITY_CTIR_FLAT_GAIN),
	                  ECTI_FLAT_GAIN);
	vRegWriteFldAlign(ECTI_04, wReadQualityTable(QUALITY_CTIR_FLAT_OFST1),
	                  ECTI_FLAT_OFST1);
	vRegWriteFldAlign(ECTI_04, wReadQualityTable(QUALITY_CTIR_FLAT_OFST2),
	                  ECTI_FLAT_OFST2);
	vRegWriteFldAlign(ECTI_02, wReadQualityTable(QUALITY_CTIR_U_WND_SZ),
	                  ECTI_U_WND_SZ);
	vRegWriteFldAlign(ECTI_03, wReadQualityTable(QUALITY_CTIR_V_WND_SZ),
	                  ECTI_V_WND_SZ);
	vRegWriteFldAlign(ECTI_02, wReadQualityTable(QUALITY_CTIR_U_STB_GAIN),
	                  ECTI_U_STB_GAIN);
	vRegWriteFldAlign(ECTI_03, wReadQualityTable(QUALITY_CTIR_V_STB_GAIN),
	                  ECTI_V_STB_GAIN);
	vRegWriteFldAlign(ECTI_02, wReadQualityTable(QUALITY_CTIR_U_STB_OFST1),
	                  ECTI_U_STB_OFST1);
	vRegWriteFldAlign(ECTI_03, wReadQualityTable(QUALITY_CTIR_V_STB_OFST1),
	                  ECTI_V_STB_OFST1);
	vRegWriteFldAlign(ECTI_02, wReadQualityTable(QUALITY_CTIR_U_STB_OFST2),
	                  ECTI_U_STB_OFST2);
	vRegWriteFldAlign(ECTI_03, wReadQualityTable(QUALITY_CTIR_V_STB_OFST2),
	                  ECTI_V_STB_OFST2);
	vRegWriteFldAlign(ECTI_05, wReadQualityTable(QUALITY_CTIR_COR),
	                  ECTI_COR);
	                  
}

/*****************************************************************************************/
/*************************************   CBE   ******************************************/
/*****************************************************************************************/
/**
 * @brief CTIF on/off
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvCBEOnOff(UINT8 bOnOff)
{
#if defined(CC_MT5396)
	vRegWriteFldAlign(CBE_00, bOnOff, CBEENABLE);
#endif
}

/**
 * @brief set main path CTIF initial parameters
 */
void vDrvCBEInit(void)
{
#if defined(CC_MT5396)
	vDrvLoadRegDwrdTbl(REGTBL_CBE);
#endif
}

/**
 * @brief set CBE parameters
 * @param void
 */
void vDrvCBELoadParam(void)
{
#if defined(CC_MT5396)
	vRegWriteFldAlign(CBE_00,wReadQualityTable(QUALITY_CBE_WEI_IT3),CBE_W_IT3);
	vRegWriteFldAlign(CBE_00,wReadQualityTable(QUALITY_CBE_WEI_IT5),CBE_W_IT5);                                               
#endif	
}
//**   Remove LTI
/*****************************************************************************************/
/*************************************   LTI  ***********************************************/
/*****************************************************************************************/
/**
 * @brief LTI on/off
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvLTIOnOff(UINT8 bOnOff)
{
#ifdef __MODEL_slt__
	bOnOff = SV_OFF;
#endif 

	if(bOnOff)
	{
	    vRegWriteFldAlign(TDSHP_12, wReadQualityTable(QUALITY_LTI_GAIN_LTI_H1), TDS_GAIN_LTI_H1);    
	    vRegWriteFldAlign(TDSHP_12, wReadQualityTable(QUALITY_LTI_GAIN_LTI_H2), TDS_GAIN_LTI_H2);    
	    vRegWriteFldAlign(TDSHP_12, wReadQualityTable(QUALITY_LTI_GAIN_LTI_V), TDS_GAIN_LTI_V);      
	}
	else
	{
	    vRegWriteFldAlign(TDSHP_12, 0, TDS_GAIN_LTI_H1);    
	    vRegWriteFldAlign(TDSHP_12, 0, TDS_GAIN_LTI_H2);    
	    vRegWriteFldAlign(TDSHP_12, 0, TDS_GAIN_LTI_V);    	
	}
}

/**
 * @brief set main path LTI initial parameters
 */
void vDrvLTIInit(void)
{
	// LTI init is integrated in TDSHARP
	return;
}

/**
 * @brief set LTI parameters
 * @param prLTIParam LTi parameters structure
 */
void vDrvLTIParam(void)
{
    vRegWriteFldAlign(TDSHP_00, wReadQualityTable(QUALITY_LTI_FLT_LTI_H1), TDS_FLT_LTI_H1);        
    vRegWriteFldAlign(TDSHP_0A, wReadQualityTable(QUALITY_LTI_COR_ZERO_LTI_H1), TDS_COR_ZERO_LTI_H1);   
    vRegWriteFldAlign(TDSHP_0C, wReadQualityTable(QUALITY_LTI_COR_RATIO_LTI_H1), TDS_COR_RATIO_LTI_H1);  
    vRegWriteFldAlign(TDSHP_0F, wReadQualityTable(QUALITY_LTI_COR_THR_LTI_H1), TDS_COR_THR_LTI_H1);    
    vRegWriteFldAlign(TDSHP_12, wReadQualityTable(QUALITY_LTI_GAIN_LTI_H1), TDS_GAIN_LTI_H1);       
    vIO32WriteFldAlign(TDSHP_LIMIT_02, wReadQualityTable(QUALITY_LTI_LIMIT_LTI_H1), LTI_LIMIT_LTI_H1);      
    vRegWriteFldAlign(TDSHP_1A, wReadQualityTable(QUALITY_LTI_LIMIT_RATIO_LTI_H1), TDS_LIMIT_RATIO_LTI_H1);
    vIO32WriteFldAlign(TDSHP_LIMIT_05, wReadQualityTable(QUALITY_LTI_BOUND_LTI_H1), LTI_BOUND_LTI_H1);      
    vRegWriteFldAlign(TDSHP_24, wReadQualityTable(QUALITY_LTI_CLIP_RATIO_LTI_H1), TDS_CLIP_RATIO_LTI_H1); 
    vRegWriteFldAlign(TDSHP_24, wReadQualityTable(QUALITY_LTI_CLIP_POS_LTI_H1), TDS_CLIP_POS_LTI_H1);   
    vRegWriteFldAlign(TDSHP_24, wReadQualityTable(QUALITY_LTI_CLIP_NEG_LTI_H1), TDS_CLIP_NEG_LTI_H1);   
    vRegWriteFldAlign(TDSHP_00, wReadQualityTable(QUALITY_LTI_FLT_LTI_H2), TDS_FLT_LTI_H2);        
    vRegWriteFldAlign(TDSHP_0A, wReadQualityTable(QUALITY_LTI_COR_ZERO_LTI_H2), TDS_COR_ZERO_LTI_H2);   
    vRegWriteFldAlign(TDSHP_0C, wReadQualityTable(QUALITY_LTI_COR_RATIO_LTI_H2), TDS_COR_RATIO_LTI_H2);  
    vRegWriteFldAlign(TDSHP_0F, wReadQualityTable(QUALITY_LTI_COR_THR_LTI_H2), TDS_COR_THR_LTI_H2);    
    vRegWriteFldAlign(TDSHP_12, wReadQualityTable(QUALITY_LTI_GAIN_LTI_H2), TDS_GAIN_LTI_H2);       
    vIO32WriteFldAlign(TDSHP_LIMIT_02, wReadQualityTable(QUALITY_LTI_LIMIT_LTI_H2), LTI_LIMIT_LTI_H2);      
    vRegWriteFldAlign(TDSHP_1A, wReadQualityTable(QUALITY_LTI_LIMIT_RATIO_LTI_H2), TDS_LIMIT_RATIO_LTI_H2);
    vIO32WriteFldAlign(TDSHP_LIMIT_05 , wReadQualityTable(QUALITY_LTI_BOUND_LTI_H2), LTI_BOUND_LTI_H2);      
    vRegWriteFldAlign(TDSHP_25, wReadQualityTable(QUALITY_LTI_CLIP_RATIO_LTI_H2), TDS_CLIP_RATIO_LTI_H2); 
    vRegWriteFldAlign(TDSHP_25, wReadQualityTable(QUALITY_LTI_CLIP_POS_LTI_H2), TDS_CLIP_POS_LTI_H2);   
    vRegWriteFldAlign(TDSHP_25, wReadQualityTable(QUALITY_LTI_CLIP_NEG_LTI_H2), TDS_CLIP_NEG_LTI_H2);   
    vRegWriteFldAlign(TDSHP_00, wReadQualityTable(QUALITY_LTI_FLT_LTI_V), TDS_FLT_LTI_V);         
    vRegWriteFldAlign(TDSHP_0A, wReadQualityTable(QUALITY_LTI_COR_ZERO_LTI_V), TDS_COR_ZERO_LTI_V);    
    vRegWriteFldAlign(TDSHP_0C, wReadQualityTable(QUALITY_LTI_COR_RATIO_LTI_V), TDS_COR_RATIO_LTI_V);   
    vRegWriteFldAlign(TDSHP_0F, wReadQualityTable(QUALITY_LTI_COR_THR_LTI_V), TDS_COR_THR_LTI_V);     
    vRegWriteFldAlign(TDSHP_12, wReadQualityTable(QUALITY_LTI_GAIN_LTI_V), TDS_GAIN_LTI_V);        
    vIO32WriteFldAlign(TDSHP_LIMIT_02 , wReadQualityTable(QUALITY_LTI_LIMIT_LTI_V), LTI_LIMIT_LTI_V);       
    vRegWriteFldAlign(TDSHP_1A, wReadQualityTable(QUALITY_LTI_LIMIT_RATIO_LTI_V), TDS_LIMIT_RATIO_LTI_V); 
    vIO32WriteFldAlign(TDSHP_LIMIT_05, wReadQualityTable(QUALITY_LTI_BOUND_LTI_V), LTI_BOUND_LTI_V);       
    vRegWriteFldAlign(TDSHP_26, wReadQualityTable(QUALITY_LTI_CLIP_RATIO_LTI_V), TDS_CLIP_RATIO_LTI_V);  
    vRegWriteFldAlign(TDSHP_26, wReadQualityTable(QUALITY_LTI_CLIP_POS_LTI_V), TDS_CLIP_POS_LTI_V);    
    vRegWriteFldAlign(TDSHP_26, wReadQualityTable(QUALITY_LTI_CLIP_NEG_LTI_V), TDS_CLIP_NEG_LTI_V);  
}

/*****************************************************************************************/
/***************************************  HSHARP *****************************************/
/****************************************************************************************/
/**
 * @brief enable/disable main path HSHARPF
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvPreSharpOnOff(UINT8 bPath, UINT8 bOnOff)
{
#if defined(CC_MT5396) || defined(CC_MT5368)
    bPath = SV_VP_MAIN;     //Protect sub path for H sharp.
    
    UINT8 bOffset = (bPath == SV_VP_MAIN) ? 0 : HSHARP_OFFSET;

    vRegWriteFldAlign(HSHARP_MAIN_00 + bOffset, bOnOff, HSP_SHARP_EN);
#endif
}

/**
 * @brief set main path HSHARP initial parameters
 */
void vDrvPreSharpInit(void)
{
#if defined(CC_MT5396) || defined(CC_MT5368)
    vDrvLoadRegDwrdTbl(REGTBL_HSHARP);
#endif
}

/**
 * @brief set main path HSHARPF parameters
 */
void vDrvPreSharpParam(UINT8 bPath)
{
#if defined(CC_MT5396) || defined(CC_MT5368)
//  Global Gain
	vRegWriteFldAlign(HSHARP_MAIN_00 ,
	                  wReadQualityTable(QUALITY_HSHARP_HIGH), HSP_SHARP_HIGH);
	vRegWriteFldAlign(HSHARP_MAIN_00 ,
	                  wReadQualityTable(QUALITY_HSHARP_MID), HSP_SHARP_MID);
	vRegWriteFldAlign(HSHARP_MAIN_00 ,
	                  wReadQualityTable(QUALITY_HSHARP_LOW), HSP_SHARP_LOW);
	vRegWriteFldAlign(HSHARP_MAIN_01 ,
	                  wReadQualityTable(QUALITY_HSHARP_HIGH_NEG), HSP_SHARP_HIGH_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_01 ,
	                  wReadQualityTable(QUALITY_HSHARP_MID_NEG), HSP_SHARP_MID_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_01 ,
	                  wReadQualityTable(QUALITY_HSHARP_LOW_NEG), HSP_SHARP_LOW_NEG);

//  Global Coring
	vRegWriteFldAlign(HSHARP_MAIN_02 ,
	                  wReadQualityTable(QUALITY_HSHARP_HIGH_CORING),
	                  HSP_SHARP_HIGH_CORING);
	vRegWriteFldAlign(HSHARP_MAIN_02 ,
	                  wReadQualityTable(QUALITY_HSHARP_MID_CORING),
	                  HSP_SHARP_MID_CORING);
	vRegWriteFldAlign(HSHARP_MAIN_02 ,
	                  wReadQualityTable(QUALITY_HSHARP_LOW_CORING),
	                  HSP_SHARP_LOW_CORING);

//  Global Limit
	vRegWriteFldAlign(HSHARP_MAIN_03 ,
	                  wReadQualityTable(QUALITY_HSHARP_HIGH_LIMIT_POS),
	                  HSP_SHARP_HIGH_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_03 ,
	                  wReadQualityTable(QUALITY_HSHARP_MID_LIMIT_POS),
	                  HSP_SHARP_MID_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_03 ,
	                  wReadQualityTable(QUALITY_HSHARP_LOW_LIMIT_POS),
	                  HSP_SHARP_LOW_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_04 ,
	                  wReadQualityTable(QUALITY_HSHARP_HIGH_LIMIT_NEG),
	                  HSP_SHARP_HIGH_LIMIT_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_04 ,
	                  wReadQualityTable(QUALITY_HSHARP_MID_LIMIT_NEG),
	                  HSP_SHARP_MID_LIMIT_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_04 ,
	                  wReadQualityTable(QUALITY_HSHARP_LOW_LIMIT_NEG),
	                  HSP_SHARP_LOW_LIMIT_NEG);

//  Global Clip
	vRegWriteFldAlign(HSHARP_MAIN_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_CLIP_POS_EN),
	                  HSP_CLIP_POS_EN);
	vRegWriteFldAlign(HSHARP_MAIN_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_CLIP_NEG_EN),
	                  HSP_CLIP_NEG_EN);
	vRegWriteFldAlign(HSHARP_MAIN_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_GB_CLIP_POS_EN_H),
	                  HSP_GB_CLIP_POS_EN_H);
	vRegWriteFldAlign(HSHARP_MAIN_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_GB_CLIP_NEG_EN_H),
	                  HSP_GB_CLIP_NEG_EN_H);
	vRegWriteFldAlign(HSHARP_MAIN_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_GB_CLIP_POS_EN_M),
	                  HSP_GB_CLIP_POS_EN_M);
	vRegWriteFldAlign(HSHARP_MAIN_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_GB_CLIP_NEG_EN_M),
	                  HSP_GB_CLIP_NEG_EN_M);
	vRegWriteFldAlign(HSHARP_MAIN_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_GB_CLIP_POS_EN_L),
	                  HSP_GB_CLIP_POS_EN_L);
	vRegWriteFldAlign(HSHARP_MAIN_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_GB_CLIP_NEG_EN_L),
	                  HSP_GB_CLIP_NEG_EN_L);
	vRegWriteFldAlign(HSHARP_MAIN_06 ,
	                  wReadQualityTable(QUALITY_HSHARP_CLIP_FRONT_THR_HIGH),
	                  HSP_CLIP_FRONT_THR_HIGH);
	vRegWriteFldAlign(HSHARP_MAIN_06 ,
	                  wReadQualityTable(QUALITY_HSHARP_CLIP_FRONT_THR_LOW),
	                  HSP_CLIP_FRONT_THR_LOW);
	vRegWriteFldAlign(HSHARP_MAIN_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_CLIP_POS),
	                  HSP_CLIP_POS_TH);
	vRegWriteFldAlign(HSHARP_MAIN_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_CLIP_NEG),
	                  HSP_CLIP_NEG_TH);
	vRegWriteFldAlign(HSHARP_MAIN_10 ,
	                  wReadQualityTable(QUALITY_HSHARP_SOFT_CLIP_GAIN),
	                  HSP_SHARP_SOFT_GAIN);
	

//  Local Gain
	vRegWriteFldAlign(HSHARP_MAIN_07 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_HIGH),
	                  HSP_LC_SHARP_HIGH);
	vRegWriteFldAlign(HSHARP_MAIN_07 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_MID), HSP_LC_SHARP_MID);
	vRegWriteFldAlign(HSHARP_MAIN_07 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_LOW), HSP_LC_SHARP_LOW);
	vRegWriteFldAlign(HSHARP_MAIN_08 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_HIGH_NEG),
	                  HSP_LC_SHARP_HIGH_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_08 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_MID_NEG), HSP_LC_SHARP_MID_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_08 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_LOW_NEG), HSP_LC_SHARP_LOW_NEG);
	
//  Local Coring
	vRegWriteFldAlign(HSHARP_MAIN_09 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_HIGH_CORING),
	                  HSP_LC_SHARP_HIGH_CORING);
	vRegWriteFldAlign(HSHARP_MAIN_09 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_MID_CORING),
	                  HSP_LC_SHARP_MID_CORING);
	vRegWriteFldAlign(HSHARP_MAIN_09 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_LOW_CORING),
	                  HSP_LC_SHARP_LOW_CORING);

//  Local Limit
	vRegWriteFldAlign(HSHARP_MAIN_0A,
	                  wReadQualityTable(QUALITY_HSHARP_LC_HIGH_LIMIT_POS),
	                  HSP_LC_SHARP_HIGH_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_0A ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_MID_LIMIT_POS),
	                  HSP_LC_SHARP_MID_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_0A ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_LOW_LIMIT_POS),
	                  HSP_LC_SHARP_LOW_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_0B ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_HIGH_LIMIT_NEG),
	                  HSP_LC_SHARP_HIGH_LIMIT_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_0B ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_MID_LIMIT_NEG),
	                  HSP_LC_SHARP_MID_LIMIT_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_0B ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_LOW_LIMIT_NEG),
	                  HSP_LC_SHARP_LOW_LIMIT_NEG);

//  Local Clip
	vRegWriteFldAlign(HSHARP_MAIN_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_CLIP_POS_EN),
	                  HSP_LC_CLIP_POS_EN);
	vRegWriteFldAlign(HSHARP_MAIN_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_CLIP_NEG_EN),
	                  HSP_LC_CLIP_NEG_EN);

//  Local Table Setting
	vRegWriteFldAlign(HSHARP_MAIN_06 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_TABLE_GAIN),
	                  HSP_LC_TABLE_GAIN);
	vRegWriteFldAlign(HSHARP_MAIN_06 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_TABLE_SEL),
	                  HSP_LC_TABLE_SEL);
	vRegWriteFldAlign(HSHARP_MAIN_06 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_TABLE_OFFSET),
	                  HSP_LC_TABLE_OFFSET);

//  FLPF
	vRegWriteFldAlign(HSHARP_MAIN_0F ,
	                  wReadQualityTable(QUALITY_HSHARP_AC_LPF_EN),
	                  HSP_SHARP_AC_EN);
	vRegWriteFldAlign(HSHARP_MAIN_10 ,
	                  wReadQualityTable(QUALITY_HSHARP_AC_LPF_COEF),
	                  HSP_SHARP_AC_COEF);
#endif
}

/*****************************************************************************************/
/************************************  TDSHARP *******************************************/
/*****************************************************************************************/
/**
 * @brief Post sharpness On/Off (Main path only)
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvPostSharpOnOff(UINT8 bOnOff)
{
    #ifdef CC_SCPOS_3DTV_SUPPORT
	if (IS_LVDS_DISP_3D_POLARIZED && (BSP_GetIcVersion() < IC_VER_5395_AB))
	{
        vRegWriteFldAlign(TDSHP_00, SV_OFF, TDS_EN);
	}
	else
    #endif
    #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    if (fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
        || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
        || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR)
        || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
    {
        vRegWriteFldAlign(TDSHP_00, SV_OFF, TDS_EN);
    }
    else
    #endif
	{
		vRegWriteFldAlign(TDSHP_00, bOnOff, TDS_EN);
	}

	if (!bOnOff)
	{
		vRegWriteFldAlign(PBC_SHP1_01, 0, PBC1_EN);
		vRegWriteFldAlign(PBC_SHP2_01, 0, PBC2_EN);
		vRegWriteFldAlign(PBC_SHP3_01, 0, PBC3_EN);
	}
}
/**
 * @brief Line Interleave Post sharpness On/Off (Main path only)
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvLIPostSharpOnOff(UINT8 bOnOff)
{
	UINT8 FltType=1;
/*
FltType : 
	2'h1: VLPF : [ 1 0 2 0 1 ]
	         VBPF : [-1 0 2 0 -1]
	         V2 and D3 band output will be zero
	2'h2: VLPF : [ 1 0 2 0 1 ]
	         VBPF : [ 0 0 0 0 0 ]
	         V1, D1 and D2 band output will be zero
*/	
	if(!bOnOff)
		{
			vRegWriteFldAlign(TDSHP_2E, 0, TDS_FLT_V_3D);	
		}
	else
		{
			vRegWriteFldAlign(TDSHP_2E, FltType, TDS_FLT_V_3D);	
		}
}


void vDrvPostSharpHorizontalOnOff(UINT8 bOnOff)
{
	if(bOnOff)
	{
		vDrvPostSharpParamHorizontal();
	}
	else
	{
		vRegWriteFldAlign(TDSHP_10, 0, TDS_GAIN_H1);       
		vRegWriteFldAlign(TDSHP_10, 0, TDS_GAIN_H2);       
		vRegWriteFldAlign(TDSHP_10, 0, TDS_GAIN_H3);       

		vRegWriteFldAlign(PBC_SHP1_04, 0x80, PBC1_GAIN_H1);     
		vRegWriteFldAlign(PBC_SHP1_04, 0x80, PBC1_GAIN_H2);     
		vRegWriteFldAlign(PBC_SHP1_04, 0x80, PBC1_GAIN_H3); 

		vRegWriteFldAlign(PBC_SHP2_04, 0x80, PBC2_GAIN_H1);    
		vRegWriteFldAlign(PBC_SHP2_04, 0x80, PBC2_GAIN_H2);    
		vRegWriteFldAlign(PBC_SHP2_04, 0x80, PBC2_GAIN_H3);    

		vRegWriteFldAlign(PBC_SHP3_04, 0x80, PBC3_GAIN_H1);    
		vRegWriteFldAlign(PBC_SHP3_04, 0x80, PBC3_GAIN_H2);    
		vRegWriteFldAlign(PBC_SHP3_04, 0x80, PBC3_GAIN_H3);   	
	}
}

void vDrvPostSharpVerticalOnOff(UINT8 bOnOff)
{
	if(bOnOff)
	{
		vDrvPostSharpParamVertical();
	}
	else
	{
		vRegWriteFldAlign(TDSHP_11, 0, TDS_GAIN_V1);       
		vRegWriteFldAlign(TDSHP_11, 0, TDS_GAIN_V2);

		vRegWriteFldAlign(PBC_SHP1_04, 0x80, PBC1_GAIN_V1);      
		vRegWriteFldAlign(PBC_SHP1_05, 0x80, PBC1_GAIN_V2);
                                                                   
		vRegWriteFldAlign(PBC_SHP2_04, 0x80, PBC2_GAIN_V1);
		vRegWriteFldAlign(PBC_SHP2_05, 0x80, PBC2_GAIN_V2);
                                                                   
		vRegWriteFldAlign(PBC_SHP3_04, 0x80, PBC3_GAIN_V1);
		vRegWriteFldAlign(PBC_SHP3_05, 0x80, PBC3_GAIN_V2);   	
	}
}


void vDrvPostSharpDiagonalOnOff(UINT8 bOnOff)
{
	if(bOnOff)
	{
		vDrvPostSharpParamDiagonal();
	}
	else
	{
		vRegWriteFldAlign(TDSHP_10, 0, TDS_GAIN_D1);       
		vRegWriteFldAlign(TDSHP_11, 0, TDS_GAIN_D2);       
		vRegWriteFldAlign(TDSHP_11, 0, TDS_GAIN_D3);       
		                                                   
		vRegWriteFldAlign(PBC_SHP1_05, 0x80, PBC1_GAIN_D1);
		vRegWriteFldAlign(PBC_SHP1_05, 0x80, PBC1_GAIN_D2);
		vRegWriteFldAlign(PBC_SHP1_05, 0x80, PBC1_GAIN_D3);
		                                                   
		vRegWriteFldAlign(PBC_SHP2_05, 0x80, PBC2_GAIN_D1);
		vRegWriteFldAlign(PBC_SHP2_05, 0x80, PBC2_GAIN_D2);
		vRegWriteFldAlign(PBC_SHP2_05, 0x80, PBC2_GAIN_D3);
		                                                   
		vRegWriteFldAlign(PBC_SHP3_05, 0x80, PBC3_GAIN_D1);
		vRegWriteFldAlign(PBC_SHP3_05, 0x80, PBC3_GAIN_D2);
		vRegWriteFldAlign(PBC_SHP3_05, 0x80, PBC3_GAIN_D3);	
	}
}



/**
 * @brief Post sharpness initial parameters
 * @param
 */
void vDrvPostSharpInit(void)
{
    vRegWriteFldAlign(TDPROC_MISC_00, 0x00, TDPROC_BYPASS_ALL);
    vDrvLoadRegDwrdTbl(REGTBL_TDSHARP);
    vDrvLoadRegDwrdTbl(REGTBL_PBC);    
    vDrvYlevSharpInit();
    vDrvMoSharpInit();    
    vDrvBNRSharpInit();
    //vDrvSharp3DInit();
}


/**
 * @brief Set Post sharpness parameters (Main path only)
 * @param
 * @dependency This function should be called by vApiVideoSharpness
 */
void vDrvPostSharpParam(void)
{
    // Coring Zero
    vRegWriteFldAlign(TDSHP_08, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_H1), TDS_COR_ZERO_H1);                                                                                       
    vRegWriteFldAlign(TDSHP_08, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_H2), TDS_COR_ZERO_H2);             
    vRegWriteFldAlign(TDSHP_08, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_H3), TDS_COR_ZERO_H3);             
    vRegWriteFldAlign(TDSHP_08, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_D1), TDS_COR_ZERO_D1);             
    vRegWriteFldAlign(TDSHP_09, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_D2), TDS_COR_ZERO_D2);             
    vRegWriteFldAlign(TDSHP_09, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_D3), TDS_COR_ZERO_D3);             
    vRegWriteFldAlign(TDSHP_09, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_V1), TDS_COR_ZERO_V1);             
    vRegWriteFldAlign(TDSHP_09, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_V2), TDS_COR_ZERO_V2);   
    // Coring Ratio             
    vRegWriteFldAlign(TDSHP_0B, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_H1), TDS_COR_RATIO_H1);            
    vRegWriteFldAlign(TDSHP_0B, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_H2), TDS_COR_RATIO_H2);            
    vRegWriteFldAlign(TDSHP_0B, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_H3), TDS_COR_RATIO_H3);            
    vRegWriteFldAlign(TDSHP_0B, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_D1), TDS_COR_RATIO_D1);            
    vRegWriteFldAlign(TDSHP_0B, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_D2), TDS_COR_RATIO_D2);            
    vRegWriteFldAlign(TDSHP_0B, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_D3), TDS_COR_RATIO_D3);            
    vRegWriteFldAlign(TDSHP_0B, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_V1), TDS_COR_RATIO_V1);            
    vRegWriteFldAlign(TDSHP_0B, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_V2), TDS_COR_RATIO_V2);  
    // Coring Threshold         
    vRegWriteFldAlign(TDSHP_0D, wReadQualityTable(QUALITY_TDSHARP_COR_THR_H1), TDS_COR_THR_H1);          
    vRegWriteFldAlign(TDSHP_0D, wReadQualityTable(QUALITY_TDSHARP_COR_THR_H2), TDS_COR_THR_H2);          
    vRegWriteFldAlign(TDSHP_0D, wReadQualityTable(QUALITY_TDSHARP_COR_THR_H3), TDS_COR_THR_H3);          
    vRegWriteFldAlign(TDSHP_0D, wReadQualityTable(QUALITY_TDSHARP_COR_THR_D1), TDS_COR_THR_D1);    
    vRegWriteFldAlign(TDSHP_0E, wReadQualityTable(QUALITY_TDSHARP_COR_THR_D2), TDS_COR_THR_D2);          
    vRegWriteFldAlign(TDSHP_0E, wReadQualityTable(QUALITY_TDSHARP_COR_THR_D3), TDS_COR_THR_D3);                
    vRegWriteFldAlign(TDSHP_0E, wReadQualityTable(QUALITY_TDSHARP_COR_THR_V1), TDS_COR_THR_V1);          
    vRegWriteFldAlign(TDSHP_0E, wReadQualityTable(QUALITY_TDSHARP_COR_THR_V2), TDS_COR_THR_V2); 

    // Gain    


    // Limit
    vIO32WriteFldAlign(TDSHP_LIMIT_00, wReadQualityTable(QUALITY_TDSHARP_LIMIT_H1), TDSHARP_LIMIT_H1);
    vIO32WriteFldAlign(TDSHP_LIMIT_00, wReadQualityTable(QUALITY_TDSHARP_LIMIT_H2), TDSHARP_LIMIT_H2);
    vIO32WriteFldAlign(TDSHP_LIMIT_00, wReadQualityTable(QUALITY_TDSHARP_LIMIT_H3), TDSHARP_LIMIT_H3);
    vIO32WriteFldAlign(TDSHP_LIMIT_00, wReadQualityTable(QUALITY_TDSHARP_LIMIT_D1), TDSHARP_LIMIT_D1);
    vIO32WriteFldAlign(TDSHP_LIMIT_01, wReadQualityTable(QUALITY_TDSHARP_LIMIT_D2), TDSHARP_LIMIT_D2);
    vIO32WriteFldAlign(TDSHP_LIMIT_01, wReadQualityTable(QUALITY_TDSHARP_LIMIT_D3), TDSHARP_LIMIT_D3);
    vIO32WriteFldAlign(TDSHP_LIMIT_01, wReadQualityTable(QUALITY_TDSHARP_LIMIT_V1), TDSHARP_LIMIT_V1);
    vIO32WriteFldAlign(TDSHP_LIMIT_01, wReadQualityTable(QUALITY_TDSHARP_LIMIT_V2), TDSHARP_LIMIT_V2);

    
    //Limit ratio                   
    vRegWriteFldAlign(TDSHP_19, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_H1), TDS_LIMIT_RATIO_H1);                 
    vRegWriteFldAlign(TDSHP_19, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_H2), TDS_LIMIT_RATIO_H2);                 
    vRegWriteFldAlign(TDSHP_19, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_H3), TDS_LIMIT_RATIO_H3);                 
    vRegWriteFldAlign(TDSHP_19, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_D1), TDS_LIMIT_RATIO_D1);                 
    vRegWriteFldAlign(TDSHP_19, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_D2), TDS_LIMIT_RATIO_D2);                 
    vRegWriteFldAlign(TDSHP_19, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_D3), TDS_LIMIT_RATIO_D3);                 
    vRegWriteFldAlign(TDSHP_19, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_V1), TDS_LIMIT_RATIO_V1);                 
    vRegWriteFldAlign(TDSHP_19, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_V2), TDS_LIMIT_RATIO_V2);     
    
    //Bound                                                                                           
    vIO32WriteFldAlign(TDSHP_LIMIT_03, wReadQualityTable(QUALITY_TDSHARP_BOUND_H1), TDSHARP_BOUND_H1);
    vIO32WriteFldAlign(TDSHP_LIMIT_03, wReadQualityTable(QUALITY_TDSHARP_BOUND_H2), TDSHARP_BOUND_H2);
    vIO32WriteFldAlign(TDSHP_LIMIT_03, wReadQualityTable(QUALITY_TDSHARP_BOUND_H3), TDSHARP_BOUND_H3);
    vIO32WriteFldAlign(TDSHP_LIMIT_03, wReadQualityTable(QUALITY_TDSHARP_BOUND_D1), TDSHARP_BOUND_D1);
    vIO32WriteFldAlign(TDSHP_LIMIT_04, wReadQualityTable(QUALITY_TDSHARP_BOUND_D2), TDSHARP_BOUND_D2);
    vIO32WriteFldAlign(TDSHP_LIMIT_04, wReadQualityTable(QUALITY_TDSHARP_BOUND_D3), TDSHARP_BOUND_D3); 
    vIO32WriteFldAlign(TDSHP_LIMIT_04, wReadQualityTable(QUALITY_TDSHARP_BOUND_V1), TDSHARP_BOUND_V1);
    vIO32WriteFldAlign(TDSHP_LIMIT_04, wReadQualityTable(QUALITY_TDSHARP_BOUND_V2), TDSHARP_BOUND_V2);                                                                                          

    // Clip       
    vRegWriteFldAlign(TDSHP_21, wReadQualityTable(QUALITY_TDSHARP_CLIP_RATIO_H), TDS_CLIP_RATIO_H);
    vRegWriteFldAlign(TDSHP_21, wReadQualityTable(QUALITY_TDSHARP_CLIP_POS_H), TDS_CLIP_POS_H);              
    vRegWriteFldAlign(TDSHP_21, wReadQualityTable(QUALITY_TDSHARP_CLIP_NEG_H), TDS_CLIP_NEG_H);     
    vRegWriteFldAlign(TDSHP_22, wReadQualityTable(QUALITY_TDSHARP_CLIP_RATIO_V), TDS_CLIP_RATIO_V);                  
    vRegWriteFldAlign(TDSHP_22, wReadQualityTable(QUALITY_TDSHARP_CLIP_POS_V), TDS_CLIP_POS_V);                    
    vRegWriteFldAlign(TDSHP_22, wReadQualityTable(QUALITY_TDSHARP_CLIP_NEG_V), TDS_CLIP_NEG_V); 
    vRegWriteFldAlign(TDSHP_23, wReadQualityTable(QUALITY_TDSHARP_CLIP_RATIO_D), TDS_CLIP_RATIO_D);                  
    vRegWriteFldAlign(TDSHP_23, wReadQualityTable(QUALITY_TDSHARP_CLIP_POS_D), TDS_CLIP_POS_D);                    
    vRegWriteFldAlign(TDSHP_23, wReadQualityTable(QUALITY_TDSHARP_CLIP_NEG_D), TDS_CLIP_NEG_D); 

    // Y Level 
    vRegWriteFldAlign(TDSHP_2E, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_EN), TDS_YLEV_EN); 
    vIO32WriteFldAlign(TDPROC_YLEV_00, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_ADAP_ENA), TDS_YLEV_ADAP_ENA);
    vIO32WriteFldAlign(TDPROC_YLEV_00, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_ADL_ENA), TDS_YLEV_ADL_ENA);
    vIO32WriteFldAlign(TDPROC_YLEV_00, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_APL_ENA), TDS_YLEV_APL_ENA);
    vIO32WriteFldAlign(TDPROC_YLEV_00, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_ADL_GAIN), TDS_YLEV_ADL_GAIN);
    vIO32WriteFldAlign(TDPROC_YLEV_00, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_APL_ZERO), TDS_YLEV_APL_ZERO);
    vIO32WriteFldAlign(TDPROC_YLEV_00, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_APL_THR), TDS_YLEV_APL_THR);	
    vIO32WriteFldAlign(TDPROC_YLEV_01, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_P1),  TDS_YLEV_P1);
    vIO32WriteFldAlign(TDPROC_YLEV_01, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_P2),  TDS_YLEV_P2);
    vIO32WriteFldAlign(TDPROC_YLEV_01, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_P3),  TDS_YLEV_P3);
    vIO32WriteFldAlign(TDPROC_YLEV_01, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_P4),  TDS_YLEV_P4);
    vIO32WriteFldAlign(TDPROC_YLEV_02, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_G1), TDS_YLEV_G1);
    vIO32WriteFldAlign(TDPROC_YLEV_02, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_G2), TDS_YLEV_G2);
    vIO32WriteFldAlign(TDPROC_YLEV_02, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_G3), TDS_YLEV_G3);
    vIO32WriteFldAlign(TDPROC_YLEV_02, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_G4), TDS_YLEV_G4);  

//PBC    
	#if defined(CC_MT5396) || defined(CC_MT5368)    
	    vRegWriteFldAlign(PBC_SHP1_01, wReadQualityTable(QUALITY_PBC_PBC1_EN), PBC1_EN);                               
	    vRegWriteFldAlign(PBC_SHP1_03, wReadQualityTable(QUALITY_PBC_PBC1_EDGE_EN), PBC1_EDGE_EN);                          
	    vRegWriteFldAlign(PBC_SHP1_04, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_H1), PBC1_GAIN_H1);     
	    vRegWriteFldAlign(PBC_SHP1_04, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_H2), PBC1_GAIN_H2);     
	    vRegWriteFldAlign(PBC_SHP1_04, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_H3), PBC1_GAIN_H3); 
	    vRegWriteFldAlign(PBC_SHP1_05, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_D1), PBC1_GAIN_D1);      
	    vRegWriteFldAlign(PBC_SHP1_05, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_D2), PBC1_GAIN_D2);      
	    vRegWriteFldAlign(PBC_SHP1_05, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_D3), PBC1_GAIN_D3);
	    vRegWriteFldAlign(PBC_SHP1_04, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_V1), PBC1_GAIN_V1);      
	    vRegWriteFldAlign(PBC_SHP1_05, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_V2), PBC1_GAIN_V2);      
	    vRegWriteFldAlign(PBC_SHP1_06, wReadQualityTable(QUALITY_PBC_PBC1_CORING_H1), PBC1_CORING_H1);     
	    vRegWriteFldAlign(PBC_SHP1_06, wReadQualityTable(QUALITY_PBC_PBC1_CORING_H2), PBC1_CORING_H2);     
	    vRegWriteFldAlign(PBC_SHP1_06, wReadQualityTable(QUALITY_PBC_PBC1_CORING_H3), PBC1_CORING_H3);    
	    vRegWriteFldAlign(PBC_SHP1_06, wReadQualityTable(QUALITY_PBC_PBC1_CORING_V1), PBC1_CORING_V1);    
	    vRegWriteFldAlign(PBC_SHP1_07, wReadQualityTable(QUALITY_PBC_PBC1_CORING_V2), PBC1_CORING_V2);    
	    vRegWriteFldAlign(PBC_SHP1_07, wReadQualityTable(QUALITY_PBC_PBC1_CORING_D1), PBC1_CORING_D1);    
	    vRegWriteFldAlign(PBC_SHP1_07, wReadQualityTable(QUALITY_PBC_PBC1_CORING_D2), PBC1_CORING_D2);    
	    vRegWriteFldAlign(PBC_SHP1_07, wReadQualityTable(QUALITY_PBC_PBC1_CORING_D3), PBC1_CORING_D3);   	      
	    vRegWriteFldAlign(PBC_SHP1_08, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_H1), PBC1_LIMIT_H1);     
	    vRegWriteFldAlign(PBC_SHP1_08, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_H2), PBC1_LIMIT_H2);     
	    vRegWriteFldAlign(PBC_SHP1_08, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_H3), PBC1_LIMIT_H3);     
	    vRegWriteFldAlign(PBC_SHP1_08, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_V1), PBC1_LIMIT_V1);     
	    vRegWriteFldAlign(PBC_SHP1_09, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_V2), PBC1_LIMIT_V2);     
	    vRegWriteFldAlign(PBC_SHP1_09, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_D1), PBC1_LIMIT_D1);     
	    vRegWriteFldAlign(PBC_SHP1_09, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_D2), PBC1_LIMIT_D2);     
	    vRegWriteFldAlign(PBC_SHP1_09, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_D3), PBC1_LIMIT_D3);     

	    
	    vRegWriteFldAlign(PBC_SHP2_01, wReadQualityTable(QUALITY_PBC_PBC2_EN), PBC2_EN);           
	    vRegWriteFldAlign(PBC_SHP2_03, wReadQualityTable(QUALITY_PBC_PBC2_EDGE_EN), PBC2_EDGE_EN);      
	    vRegWriteFldAlign(PBC_SHP2_04, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_H1), PBC2_GAIN_H1);    
	    vRegWriteFldAlign(PBC_SHP2_04, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_H2), PBC2_GAIN_H2);    
	    vRegWriteFldAlign(PBC_SHP2_04, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_H3), PBC2_GAIN_H3);  
	    vRegWriteFldAlign(PBC_SHP2_05, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_D1), PBC2_GAIN_D1);    
	    vRegWriteFldAlign(PBC_SHP2_05, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_D2), PBC2_GAIN_D2);    
	    vRegWriteFldAlign(PBC_SHP2_05, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_D3), PBC2_GAIN_D3);	    
	    vRegWriteFldAlign(PBC_SHP2_04, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_V1), PBC2_GAIN_V1);    
	    vRegWriteFldAlign(PBC_SHP2_05, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_V2), PBC2_GAIN_V2);    
	    vRegWriteFldAlign(PBC_SHP2_06, wReadQualityTable(QUALITY_PBC_PBC2_CORING_H1), PBC2_CORING_H1);    
	    vRegWriteFldAlign(PBC_SHP2_06, wReadQualityTable(QUALITY_PBC_PBC2_CORING_H2), PBC2_CORING_H2);    
	    vRegWriteFldAlign(PBC_SHP2_06, wReadQualityTable(QUALITY_PBC_PBC2_CORING_H3), PBC2_CORING_H3);    
	    vRegWriteFldAlign(PBC_SHP2_06, wReadQualityTable(QUALITY_PBC_PBC2_CORING_V1), PBC2_CORING_V1);    
	    vRegWriteFldAlign(PBC_SHP2_07, wReadQualityTable(QUALITY_PBC_PBC2_CORING_V2), PBC2_CORING_V2);    
	    vRegWriteFldAlign(PBC_SHP2_07, wReadQualityTable(QUALITY_PBC_PBC2_CORING_D1), PBC2_CORING_D1);    
	    vRegWriteFldAlign(PBC_SHP2_07, wReadQualityTable(QUALITY_PBC_PBC2_CORING_D2), PBC2_CORING_D2);    
	    vRegWriteFldAlign(PBC_SHP2_07, wReadQualityTable(QUALITY_PBC_PBC2_CORING_D3), PBC2_CORING_D3);    
	    vRegWriteFldAlign(PBC_SHP2_08, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_H1), PBC2_LIMIT_H1);     
	    vRegWriteFldAlign(PBC_SHP2_08, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_H2), PBC2_LIMIT_H2);     
	    vRegWriteFldAlign(PBC_SHP2_08, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_H3), PBC2_LIMIT_H3);     
	    vRegWriteFldAlign(PBC_SHP2_08, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_V1), PBC2_LIMIT_V1);     
	    vRegWriteFldAlign(PBC_SHP2_09, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_V2), PBC2_LIMIT_V2);     
	    vRegWriteFldAlign(PBC_SHP2_09, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_D1), PBC2_LIMIT_D1);     
	    vRegWriteFldAlign(PBC_SHP2_09, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_D2), PBC2_LIMIT_D2);     
	    vRegWriteFldAlign(PBC_SHP2_09, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_D3), PBC2_LIMIT_D3);     
	    vRegWriteFldAlign(PBC_SHP3_01, wReadQualityTable(QUALITY_PBC_PBC3_EN), PBC3_EN);           
	    vRegWriteFldAlign(PBC_SHP3_03, wReadQualityTable(QUALITY_PBC_PBC3_EDGE_EN), PBC3_EDGE_EN);      
	    vRegWriteFldAlign(PBC_SHP3_04, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_H1), PBC3_GAIN_H1);    
	    vRegWriteFldAlign(PBC_SHP3_04, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_H2), PBC3_GAIN_H2);    
	    vRegWriteFldAlign(PBC_SHP3_04, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_H3), PBC3_GAIN_H3);
	    vRegWriteFldAlign(PBC_SHP3_05, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_D1), PBC3_GAIN_D1);    
	    vRegWriteFldAlign(PBC_SHP3_05, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_D2), PBC3_GAIN_D2);    
	    vRegWriteFldAlign(PBC_SHP3_05, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_D3), PBC3_GAIN_D3); 	    
	    vRegWriteFldAlign(PBC_SHP3_04, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_V1), PBC3_GAIN_V1);    
	    vRegWriteFldAlign(PBC_SHP3_05, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_V2), PBC3_GAIN_V2);  
	    vRegWriteFldAlign(PBC_SHP3_06, wReadQualityTable(QUALITY_PBC_PBC3_CORING_H1), PBC3_CORING_H1);    
	    vRegWriteFldAlign(PBC_SHP3_06, wReadQualityTable(QUALITY_PBC_PBC3_CORING_H2), PBC3_CORING_H2);    
	    vRegWriteFldAlign(PBC_SHP3_06, wReadQualityTable(QUALITY_PBC_PBC3_CORING_H3), PBC3_CORING_H3);    
	    vRegWriteFldAlign(PBC_SHP3_06, wReadQualityTable(QUALITY_PBC_PBC3_CORING_V1), PBC3_CORING_V1);    
	    vRegWriteFldAlign(PBC_SHP3_07, wReadQualityTable(QUALITY_PBC_PBC3_CORING_V2), PBC3_CORING_V2);    
	    vRegWriteFldAlign(PBC_SHP3_07, wReadQualityTable(QUALITY_PBC_PBC3_CORING_D1), PBC3_CORING_D1);    
	    vRegWriteFldAlign(PBC_SHP3_07, wReadQualityTable(QUALITY_PBC_PBC3_CORING_D2), PBC3_CORING_D2);    
	    vRegWriteFldAlign(PBC_SHP3_07, wReadQualityTable(QUALITY_PBC_PBC3_CORING_D3), PBC3_CORING_D3);    
	    vRegWriteFldAlign(PBC_SHP3_08, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_H1), PBC3_LIMIT_H1);     
	    vRegWriteFldAlign(PBC_SHP3_08, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_H2), PBC3_LIMIT_H2);     
	    vRegWriteFldAlign(PBC_SHP3_08, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_H3), PBC3_LIMIT_H3);     
	    vRegWriteFldAlign(PBC_SHP3_08, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_V1), PBC3_LIMIT_V1);     
	    vRegWriteFldAlign(PBC_SHP3_09, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_V2), PBC3_LIMIT_V2);     
	    vRegWriteFldAlign(PBC_SHP3_09, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_D1), PBC3_LIMIT_D1);     
	    vRegWriteFldAlign(PBC_SHP3_09, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_D2), PBC3_LIMIT_D2);     
	    vRegWriteFldAlign(PBC_SHP3_09, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_D3), PBC3_LIMIT_D3);  
	#else
	    vRegWriteFldAlign(PBC_SHP1_01, wReadQualityTable(QUALITY_PBC_PBC1_EN), PBC1_EN);                                                                                                                    
	    vRegWriteFldAlign(PBC_SHP1_03, wReadQualityTable(QUALITY_PBC_PBC1_EDGE_EN), PBC1_EDGE_EN);                                                                                                          
	    vRegWriteFldAlign(PBC_SHP1_03, wReadQualityTable(QUALITY_PBC_PBC1_EDGE_SLOPE), PBC1_EDGE_SLOPE);
	    vRegWriteFldAlign(PBC_SHP1_04, wReadQualityTable(QUALITY_PBC_PBC1_LPF_EN), PBC1_LPF_EN);        
	    vRegWriteFldAlign(PBC_SHP1_04, wReadQualityTable(QUALITY_PBC_PBC1_LPF_LEVEL), PBC1_LPF_LEVEL);  	    
	    vRegWriteFldAlign(PBC_SHP1_05, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_H1), PBC1_GAIN_H1);     
	    vRegWriteFldAlign(PBC_SHP1_05, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_H2), PBC1_GAIN_H2);     
	    vRegWriteFldAlign(PBC_SHP1_05, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_H3), PBC1_GAIN_H3); 
	    vRegWriteFldAlign(PBC_SHP1_06, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_D1), PBC1_GAIN_D1);      
	    vRegWriteFldAlign(PBC_SHP1_06, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_D2), PBC1_GAIN_D2);      
	    vRegWriteFldAlign(PBC_SHP1_06, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_D3), PBC1_GAIN_D3);
	    vRegWriteFldAlign(PBC_SHP1_05, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_V1), PBC1_GAIN_V1);      
	    vRegWriteFldAlign(PBC_SHP1_06, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_V2), PBC1_GAIN_V2);      	    
	    vRegWriteFldAlign(PBC_SHP1_07, wReadQualityTable(QUALITY_PBC_PBC1_CORING_H1), PBC1_CORING_H1);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP1_07, wReadQualityTable(QUALITY_PBC_PBC1_CORING_H2), PBC1_CORING_H2);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP1_07, wReadQualityTable(QUALITY_PBC_PBC1_CORING_H3), PBC1_CORING_H3);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP1_07, wReadQualityTable(QUALITY_PBC_PBC1_CORING_V1), PBC1_CORING_V1);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP1_08, wReadQualityTable(QUALITY_PBC_PBC1_CORING_V2), PBC1_CORING_V2);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP1_08, wReadQualityTable(QUALITY_PBC_PBC1_CORING_D1), PBC1_CORING_D1);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP1_08, wReadQualityTable(QUALITY_PBC_PBC1_CORING_D2), PBC1_CORING_D2);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP1_08, wReadQualityTable(QUALITY_PBC_PBC1_CORING_D3), PBC1_CORING_D3);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP1_09, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_H1), PBC1_LIMIT_H1);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP1_09, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_H2), PBC1_LIMIT_H2);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP1_09, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_H3), PBC1_LIMIT_H3);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP1_09, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_V1), PBC1_LIMIT_V1);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP1_0A, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_V2), PBC1_LIMIT_V2);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP1_0A, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_D1), PBC1_LIMIT_D1);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP1_0A, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_D2), PBC1_LIMIT_D2);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP1_0A, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_D3), PBC1_LIMIT_D3);                                                                                                        
	                                                                                                                                                                                                        
	    vRegWriteFldAlign(PBC_SHP2_01, wReadQualityTable(QUALITY_PBC_PBC2_EN), PBC2_EN);                                                                                                                    
	    vRegWriteFldAlign(PBC_SHP2_03, wReadQualityTable(QUALITY_PBC_PBC2_EDGE_EN), PBC2_EDGE_EN);                                                                                                          
	    vRegWriteFldAlign(PBC_SHP2_03, wReadQualityTable(QUALITY_PBC_PBC2_EDGE_SLOPE), PBC2_EDGE_SLOPE); 
	    vRegWriteFldAlign(PBC_SHP2_04, wReadQualityTable(QUALITY_PBC_PBC2_LPF_EN), PBC2_LPF_EN);         
	    vRegWriteFldAlign(PBC_SHP2_04, wReadQualityTable(QUALITY_PBC_PBC2_LPF_LEVEL), PBC2_LPF_LEVEL);  
	    vRegWriteFldAlign(PBC_SHP2_05, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_H1), PBC2_GAIN_H1);    
	    vRegWriteFldAlign(PBC_SHP2_05, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_H2), PBC2_GAIN_H2);    
	    vRegWriteFldAlign(PBC_SHP2_05, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_H3), PBC2_GAIN_H3);    
	    vRegWriteFldAlign(PBC_SHP2_06, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_D1), PBC2_GAIN_D1);    
	    vRegWriteFldAlign(PBC_SHP2_06, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_D2), PBC2_GAIN_D2);    
	    vRegWriteFldAlign(PBC_SHP2_06, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_D3), PBC2_GAIN_D3);
	    vRegWriteFldAlign(PBC_SHP2_05, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_V1), PBC2_GAIN_V1);    
	    vRegWriteFldAlign(PBC_SHP2_06, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_V2), PBC2_GAIN_V2);    	    
	    vRegWriteFldAlign(PBC_SHP2_07, wReadQualityTable(QUALITY_PBC_PBC2_CORING_H1), PBC2_CORING_H1);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP2_07, wReadQualityTable(QUALITY_PBC_PBC2_CORING_H2), PBC2_CORING_H2);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP2_07, wReadQualityTable(QUALITY_PBC_PBC2_CORING_H3), PBC2_CORING_H3);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP2_07, wReadQualityTable(QUALITY_PBC_PBC2_CORING_V1), PBC2_CORING_V1);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP2_08, wReadQualityTable(QUALITY_PBC_PBC2_CORING_V2), PBC2_CORING_V2);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP2_08, wReadQualityTable(QUALITY_PBC_PBC2_CORING_D1), PBC2_CORING_D1);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP2_08, wReadQualityTable(QUALITY_PBC_PBC2_CORING_D2), PBC2_CORING_D2);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP2_08, wReadQualityTable(QUALITY_PBC_PBC2_CORING_D3), PBC2_CORING_D3);                                                                                                      
	                                                                                                                                                                                                        
	    vRegWriteFldAlign(PBC_SHP2_09, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_H1), PBC2_LIMIT_H1);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP2_09, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_H2), PBC2_LIMIT_H2);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP2_09, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_H3), PBC2_LIMIT_H3);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP2_09, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_V1), PBC2_LIMIT_V1);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP2_0A, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_V2), PBC2_LIMIT_V2);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP2_0A, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_D1), PBC2_LIMIT_D1);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP2_0A, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_D2), PBC2_LIMIT_D2);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP2_0A, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_D3), PBC2_LIMIT_D3);                                                                                                        
	                                                                                                                                                                                                        
	    vRegWriteFldAlign(PBC_SHP3_01, wReadQualityTable(QUALITY_PBC_PBC3_EN), PBC3_EN);                                                                                                                    
	    vRegWriteFldAlign(PBC_SHP3_03, wReadQualityTable(QUALITY_PBC_PBC3_EDGE_EN), PBC3_EDGE_EN);                                                                                                          
	    vRegWriteFldAlign(PBC_SHP3_03, wReadQualityTable(QUALITY_PBC_PBC3_EDGE_SLOPE), PBC3_EDGE_SLOPE);              
	    vRegWriteFldAlign(PBC_SHP3_04, wReadQualityTable(QUALITY_PBC_PBC3_LPF_EN), PBC3_LPF_EN);             
	    vRegWriteFldAlign(PBC_SHP3_04, wReadQualityTable(QUALITY_PBC_PBC3_LPF_LEVEL), PBC3_LPF_LEVEL);  
	    vRegWriteFldAlign(PBC_SHP3_05, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_H1), PBC3_GAIN_H1);    
	    vRegWriteFldAlign(PBC_SHP3_05, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_H2), PBC3_GAIN_H2);    
	    vRegWriteFldAlign(PBC_SHP3_05, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_H3), PBC3_GAIN_H3);  
	    vRegWriteFldAlign(PBC_SHP3_06, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_D1), PBC3_GAIN_D1);    
	    vRegWriteFldAlign(PBC_SHP3_06, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_D2), PBC3_GAIN_D2);    
	    vRegWriteFldAlign(PBC_SHP3_06, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_D3), PBC3_GAIN_D3);   
	    vRegWriteFldAlign(PBC_SHP3_05, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_V1), PBC3_GAIN_V1);    
	    vRegWriteFldAlign(PBC_SHP3_06, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_V2), PBC3_GAIN_V2);  	    
	    vRegWriteFldAlign(PBC_SHP3_07, wReadQualityTable(QUALITY_PBC_PBC3_CORING_H1), PBC3_CORING_H1);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP3_07, wReadQualityTable(QUALITY_PBC_PBC3_CORING_H2), PBC3_CORING_H2);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP3_07, wReadQualityTable(QUALITY_PBC_PBC3_CORING_H3), PBC3_CORING_H3);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP3_07, wReadQualityTable(QUALITY_PBC_PBC3_CORING_V1), PBC3_CORING_V1);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP3_08, wReadQualityTable(QUALITY_PBC_PBC3_CORING_V2), PBC3_CORING_V2);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP3_08, wReadQualityTable(QUALITY_PBC_PBC3_CORING_D1), PBC3_CORING_D1);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP3_08, wReadQualityTable(QUALITY_PBC_PBC3_CORING_D2), PBC3_CORING_D2);                                                                                                      
	    vRegWriteFldAlign(PBC_SHP3_08, wReadQualityTable(QUALITY_PBC_PBC3_CORING_D3), PBC3_CORING_D3);                                                                                                      
	                                                                                                                                                                                                        
	    vRegWriteFldAlign(PBC_SHP3_09, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_H1), PBC3_LIMIT_H1);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP3_09, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_H2), PBC3_LIMIT_H2);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP3_09, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_H3), PBC3_LIMIT_H3);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP3_09, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_V1), PBC3_LIMIT_V1);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP3_0A, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_V2), PBC3_LIMIT_V2);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP3_0A, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_D1), PBC3_LIMIT_D1);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP3_0A, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_D2), PBC3_LIMIT_D2);                                                                                                        
	    vRegWriteFldAlign(PBC_SHP3_0A, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_D3), PBC3_LIMIT_D3); 	
	#endif

	vDrvPostSharpParamHorizontal();
	vDrvPostSharpParamVertical();
	vDrvPostSharpParamDiagonal();
	
// TDSHARPALL
    vRegWriteFldAlign(TDSHP_27, wReadQualityTable(QUALITY_TDSHARPALL_COR_ZERO), TDS_COR_ZERO);	
    vRegWriteFldAlign(TDSHP_27, wReadQualityTable(QUALITY_TDSHARPALL_COR_RATIO), TDS_COR_RATIO);      
    vRegWriteFldAlign(TDSHP_27, wReadQualityTable(QUALITY_TDSHARPALL_COR_THR),TDS_COR_THR);        
    vRegWriteFldAlign(TDSHP_27, wReadQualityTable(QUALITY_TDSHARPALL_GAIN), TDS_GAIN);           
    vRegWriteFldAlign(TDSHP_27, wReadQualityTable(QUALITY_TDSHARPALL_LIMIT_RATIO), TDS_LIMIT_RATIO); 
    vIO32WriteFldAlign(TDSHP_LIMIT_02, wReadQualityTable(QUALITY_TDSHARPALL_LIMIT), TDSHARP_LIMIT);    
    vIO32WriteFldAlign(TDSHP_LIMIT_05, wReadQualityTable(QUALITY_TDSHARPALL_BOUND), TDSHARP_BOUND);            
    vRegWriteFldAlign(TDSHP_29, wReadQualityTable(QUALITY_TDSHARPALL_CLIP_RATIO), TDS_CLIP_RATIO);     
    vRegWriteFldAlign(TDSHP_29, wReadQualityTable(QUALITY_TDSHARPALL_CLIP_POS), TDS_CLIP_POS);       
    vRegWriteFldAlign(TDSHP_29, wReadQualityTable(QUALITY_TDSHARPALL_CLIP_NEG), TDS_CLIP_NEG);       
    vRegWriteFldAlign(TDSHP_07, wReadQualityTable(QUALITY_TDSHARPALL_SAT_PROC), TDS_SAT_PROC);       
    vRegWriteFldAlign(TDSHP_29, wReadQualityTable(QUALITY_TDSHARPALL_AC_LPF_COE), TDS_AC_LPF_COE);     
}


void vDrvPostSharpParamHorizontal(void)
{
    vRegWriteFldAlign(TDSHP_10, wReadQualityTable(QUALITY_TDSHARP_GAIN_H1), TDS_GAIN_H1);       
    vRegWriteFldAlign(TDSHP_10, wReadQualityTable(QUALITY_TDSHARP_GAIN_H2), TDS_GAIN_H2);       
    vRegWriteFldAlign(TDSHP_10, wReadQualityTable(QUALITY_TDSHARP_GAIN_H3), TDS_GAIN_H3);       
}

void vDrvPostSharpParamVertical(void)
{     
    vRegWriteFldAlign(TDSHP_11, wReadQualityTable(QUALITY_TDSHARP_GAIN_V1), TDS_GAIN_V1);       
    vRegWriteFldAlign(TDSHP_11, wReadQualityTable(QUALITY_TDSHARP_GAIN_V2), TDS_GAIN_V2);	    
}


void vDrvPostSharpParamDiagonal(void)
{   
    vRegWriteFldAlign(TDSHP_10, wReadQualityTable(QUALITY_TDSHARP_GAIN_D1), TDS_GAIN_D1);       
    vRegWriteFldAlign(TDSHP_11, wReadQualityTable(QUALITY_TDSHARP_GAIN_D2), TDS_GAIN_D2);       
    vRegWriteFldAlign(TDSHP_11, wReadQualityTable(QUALITY_TDSHARP_GAIN_D3), TDS_GAIN_D3);   	    
}

void vDrvPostSharpParamLimit(void)
{
        	vRegWriteFldAlign(TDSHP_13, IO32ReadFldAlign(TDSHP_LIMIT_00, TDSHARP_LIMIT_H1), TDS_LIMIT_POS_H1);  
        	vRegWriteFldAlign(TDSHP_13, IO32ReadFldAlign(TDSHP_LIMIT_00, TDSHARP_LIMIT_H2), TDS_LIMIT_POS_H2);  
        	vRegWriteFldAlign(TDSHP_13, IO32ReadFldAlign(TDSHP_LIMIT_00, TDSHARP_LIMIT_H3), TDS_LIMIT_POS_H3);  
        	vRegWriteFldAlign(TDSHP_13, IO32ReadFldAlign(TDSHP_LIMIT_00, TDSHARP_LIMIT_D1), TDS_LIMIT_POS_D1);  
        	vRegWriteFldAlign(TDSHP_14, IO32ReadFldAlign(TDSHP_LIMIT_01, TDSHARP_LIMIT_D2), TDS_LIMIT_POS_D2);  
        	vRegWriteFldAlign(TDSHP_14, IO32ReadFldAlign(TDSHP_LIMIT_01, TDSHARP_LIMIT_D3), TDS_LIMIT_POS_D3);  
        	vRegWriteFldAlign(TDSHP_14, IO32ReadFldAlign(TDSHP_LIMIT_01, TDSHARP_LIMIT_V1), TDS_LIMIT_POS_V1);  
        	vRegWriteFldAlign(TDSHP_14, IO32ReadFldAlign(TDSHP_LIMIT_01, TDSHARP_LIMIT_V2), TDS_LIMIT_POS_V2); 
        	vRegWriteFldAlign(TDSHP_15, IO32ReadFldAlign(TDSHP_LIMIT_02, LTI_LIMIT_LTI_H1), TDS_LIMIT_POS_LTI_H1);  
        	vRegWriteFldAlign(TDSHP_15, IO32ReadFldAlign(TDSHP_LIMIT_02, LTI_LIMIT_LTI_H2), TDS_LIMIT_POS_LTI_H2);  
        	vRegWriteFldAlign(TDSHP_15, IO32ReadFldAlign(TDSHP_LIMIT_02, LTI_LIMIT_LTI_V), TDS_LIMIT_POS_LTI_V);  
        	vRegWriteFldAlign(TDSHP_28, IO32ReadFldAlign(TDSHP_LIMIT_02, TDSHARP_LIMIT), TDS_LIMIT_POS);   
        	
        	vRegWriteFldAlign(TDSHP_16, IO32ReadFldAlign(TDSHP_LIMIT_00, TDSHARP_LIMIT_H1), TDS_LIMIT_NEG_H1);  
        	vRegWriteFldAlign(TDSHP_16, IO32ReadFldAlign(TDSHP_LIMIT_00, TDSHARP_LIMIT_H2), TDS_LIMIT_NEG_H2);  
        	vRegWriteFldAlign(TDSHP_16, IO32ReadFldAlign(TDSHP_LIMIT_00, TDSHARP_LIMIT_H3), TDS_LIMIT_NEG_H3);  
        	vRegWriteFldAlign(TDSHP_16, IO32ReadFldAlign(TDSHP_LIMIT_00, TDSHARP_LIMIT_D1), TDS_LIMIT_NEG_D1);  
        	vRegWriteFldAlign(TDSHP_17, IO32ReadFldAlign(TDSHP_LIMIT_01, TDSHARP_LIMIT_D2), TDS_LIMIT_NEG_D2);  
        	vRegWriteFldAlign(TDSHP_17, IO32ReadFldAlign(TDSHP_LIMIT_01, TDSHARP_LIMIT_D3), TDS_LIMIT_NEG_D3); 
        	vRegWriteFldAlign(TDSHP_17, IO32ReadFldAlign(TDSHP_LIMIT_01, TDSHARP_LIMIT_V1), TDS_LIMIT_NEG_V1);  
        	vRegWriteFldAlign(TDSHP_17, IO32ReadFldAlign(TDSHP_LIMIT_01, TDSHARP_LIMIT_V2), TDS_LIMIT_NEG_V2);  
        	vRegWriteFldAlign(TDSHP_18, IO32ReadFldAlign(TDSHP_LIMIT_02, LTI_LIMIT_LTI_H1), TDS_LIMIT_NEG_LTI_H1);  
        	vRegWriteFldAlign(TDSHP_18, IO32ReadFldAlign(TDSHP_LIMIT_02, LTI_LIMIT_LTI_H2), TDS_LIMIT_NEG_LTI_H2);  
        	vRegWriteFldAlign(TDSHP_18, IO32ReadFldAlign(TDSHP_LIMIT_02, LTI_LIMIT_LTI_V), TDS_LIMIT_NEG_LTI_V);  
        	vRegWriteFldAlign(TDSHP_28, IO32ReadFldAlign(TDSHP_LIMIT_02, TDSHARP_LIMIT), TDS_LIMIT_NEG);           	
        	
		vRegWriteFldAlign(TDSHP_1B, IO32ReadFldAlign(TDSHP_LIMIT_03, TDSHARP_BOUND_H1), TDS_BOUND_POS_H1);  
        	vRegWriteFldAlign(TDSHP_1B, IO32ReadFldAlign(TDSHP_LIMIT_03, TDSHARP_BOUND_H2), TDS_BOUND_POS_H2);  
        	vRegWriteFldAlign(TDSHP_1B, IO32ReadFldAlign(TDSHP_LIMIT_03, TDSHARP_BOUND_H3), TDS_BOUND_POS_H3);  
        	vRegWriteFldAlign(TDSHP_1B, IO32ReadFldAlign(TDSHP_LIMIT_03, TDSHARP_BOUND_D1), TDS_BOUND_POS_D1);  
        	vRegWriteFldAlign(TDSHP_1C, IO32ReadFldAlign(TDSHP_LIMIT_04, TDSHARP_BOUND_D2), TDS_BOUND_POS_D2);  
        	vRegWriteFldAlign(TDSHP_1C, IO32ReadFldAlign(TDSHP_LIMIT_04, TDSHARP_BOUND_D3), TDS_BOUND_POS_D3);  
        	vRegWriteFldAlign(TDSHP_1C, IO32ReadFldAlign(TDSHP_LIMIT_04, TDSHARP_BOUND_V1), TDS_BOUND_POS_V1);  
        	vRegWriteFldAlign(TDSHP_1C, IO32ReadFldAlign(TDSHP_LIMIT_04, TDSHARP_BOUND_V2), TDS_BOUND_POS_V2);  
        	vRegWriteFldAlign(TDSHP_1D, IO32ReadFldAlign(TDSHP_LIMIT_05, LTI_BOUND_LTI_H1), TDS_BOUND_POS_LTI_H1);  
        	vRegWriteFldAlign(TDSHP_1D, IO32ReadFldAlign(TDSHP_LIMIT_05, LTI_BOUND_LTI_H2), TDS_BOUND_POS_LTI_H2);  
        	vRegWriteFldAlign(TDSHP_1D, IO32ReadFldAlign(TDSHP_LIMIT_05, LTI_BOUND_LTI_V), TDS_BOUND_POS_LTI_V);  
        	vRegWriteFldAlign(TDSHP_28, IO32ReadFldAlign(TDSHP_LIMIT_05, TDSHARP_BOUND), TDS_BOUND_POS); 
        	
        	vRegWriteFldAlign(TDSHP_1E, IO32ReadFldAlign(TDSHP_LIMIT_03, TDSHARP_BOUND_H1), TDS_BOUND_NEG_H1);  
        	vRegWriteFldAlign(TDSHP_1E, IO32ReadFldAlign(TDSHP_LIMIT_03, TDSHARP_BOUND_H2), TDS_BOUND_NEG_H2);  
        	vRegWriteFldAlign(TDSHP_1E, IO32ReadFldAlign(TDSHP_LIMIT_03, TDSHARP_BOUND_H3), TDS_BOUND_NEG_H3);  
        	vRegWriteFldAlign(TDSHP_1E, IO32ReadFldAlign(TDSHP_LIMIT_03, TDSHARP_BOUND_D1), TDS_BOUND_NEG_D1);  
        	vRegWriteFldAlign(TDSHP_1F, IO32ReadFldAlign(TDSHP_LIMIT_04, TDSHARP_BOUND_D2), TDS_BOUND_NEG_D2);  
        	vRegWriteFldAlign(TDSHP_1F, IO32ReadFldAlign(TDSHP_LIMIT_04, TDSHARP_BOUND_D3), TDS_BOUND_NEG_D3); 
        	vRegWriteFldAlign(TDSHP_1F, IO32ReadFldAlign(TDSHP_LIMIT_04, TDSHARP_BOUND_V1), TDS_BOUND_NEG_V1);  
        	vRegWriteFldAlign(TDSHP_1F, IO32ReadFldAlign(TDSHP_LIMIT_04, TDSHARP_BOUND_V2), TDS_BOUND_NEG_V2);       
        	vRegWriteFldAlign(TDSHP_20, IO32ReadFldAlign(TDSHP_LIMIT_05, LTI_BOUND_LTI_H1), TDS_BOUND_NEG_LTI_H1);  
        	vRegWriteFldAlign(TDSHP_20, IO32ReadFldAlign(TDSHP_LIMIT_05, LTI_BOUND_LTI_H2), TDS_BOUND_NEG_LTI_H2);  
        	vRegWriteFldAlign(TDSHP_20, IO32ReadFldAlign(TDSHP_LIMIT_05, LTI_BOUND_LTI_V), TDS_BOUND_NEG_LTI_V);  
        	vRegWriteFldAlign(TDSHP_28, IO32ReadFldAlign(TDSHP_LIMIT_05, TDSHARP_BOUND), TDS_BOUND_NEG);     	
}

static void vDrvPBCProc(void)
{
    static UINT16 wRadius1 = 0xffff;
    static UINT16 wRange1 = 0xffff;
    static UINT16 wThRange1 = 0xffff;
    static UINT16 wRadius2 = 0xffff;
    static UINT16 wRange2 = 0xffff;
    static UINT16 wThRange2 = 0xffff;
    static UINT16 wRadius3 = 0xffff;
    static UINT16 wRange3 = 0xffff;
    static UINT16 wThRange3 = 0xffff;
    
    UINT16 wRadius, wRange, wThRange;

    // update PBC 1 register
    wRadius = RegReadFldAlign(PBC_SHP1_02, PBC1_RADIUS_C);
    wRange = RegReadFldAlign(PBC_SHP1_01, PBC1_RADIUS_R);
    wThRange = RegReadFldAlign(PBC_SHP1_01, PBC1_THETA_R);

    if ((wRadius1 != wRadius) || (wRange1 != wRange) || (wThRange1 != wThRange))
    {
        if ((wRadius !=0 ) && (wRange != 0) && (wThRange != 0))
        {
            vRegWriteFldAlign(PBC_SHP1_03, 4096 / wRange, PBC1_RSLOPE);
            vRegWriteFldAlign(PBC_SHP1_02, 4096 / wThRange, PBC1_TSLOPE);
            vRegWriteFldAlign(PBC_SHP1_01, 4096 / ((wRange > wRadius) ? wRadius : wRange), PBC1_RSLOPE_1);
        }
        wRadius1 = wRadius;
        wRange1 = wRange;
        wThRange1 = wThRange;
    }

    // update PBC 2 register
    wRadius = RegReadFldAlign(PBC_SHP2_02, PBC2_RADIUS_C);
    wRange = RegReadFldAlign(PBC_SHP2_01, PBC2_RADIUS_R);
    wThRange = RegReadFldAlign(PBC_SHP2_01, PBC2_THETA_R);

    if ((wRadius2 != wRadius) || (wRange2 != wRange) || (wThRange2 != wThRange))
    {
        if ((wRadius !=0 ) && (wRange != 0) && (wThRange != 0))
        {
            vRegWriteFldAlign(PBC_SHP2_03, 4096 / wRange, PBC2_RSLOPE);
            vRegWriteFldAlign(PBC_SHP2_02, 4096 / wThRange, PBC2_TSLOPE);
            vRegWriteFldAlign(PBC_SHP2_01, 4096 / ((wRange > wRadius) ? wRadius : wRange), PBC2_RSLOPE_1);
        }
        wRadius2 = wRadius;
        wRange2 = wRange;
        wThRange2 = wThRange;
    }

    // update PBC 3 register
    wRadius = RegReadFldAlign(PBC_SHP3_02, PBC3_RADIUS_C);
    wRange = RegReadFldAlign(PBC_SHP3_01, PBC3_RADIUS_R);
    wThRange = RegReadFldAlign(PBC_SHP3_01, PBC3_THETA_R);

    if ((wRadius3 != wRadius) || (wRange3 != wRange) || (wThRange3 != wThRange))
    {
        if ((wRadius !=0 ) && (wRange != 0) && (wThRange != 0))
        {
            vRegWriteFldAlign(PBC_SHP3_03, 4096 / wRange, PBC3_RSLOPE);
            vRegWriteFldAlign(PBC_SHP3_02, 4096 / wThRange, PBC2_TSLOPE);
            vRegWriteFldAlign(PBC_SHP3_01, 4096 / ((wRange > wRadius) ? wRadius : wRange), PBC3_RSLOPE_1);
        }
        wRadius3 = wRadius;
        wRange3 = wRange;
        wThRange3 = wThRange;
    }   
}

/*****************************************************************************************/
/**********************************   TDSHARP YLEV  ****************************************/
/*****************************************************************************************/
/**
 * @brief Enable/Disable adaptive Y level dependent sharpness
 * @param
 */
void vDrvSetAdapYLevOnOff(UINT8 bOnOff)
{
    fgAdapYLEV = bOnOff;
}

/**
 * @brief Y Level sharpness initial parameters
 * @param
 */
void vDrvYlevSharpInit(void)
{
    vDrvLoadYLevTable();   
    vRegWriteFldAlign(TDSHP_2E, 0x01, TDS_YLEV_EN);
    vDrvSetAdapYLevOnOff(SV_ON);
}

/**
 * @brief Post sharpness Y Level Table
 * @param
 */
void vDrvLoadYLevTable(void)
{  
		vRegWriteFldAlign(TDSHP_2A, TDS_YLEV[0], TDS_YLEV_P000);  
		vRegWriteFldAlign(TDSHP_2A, TDS_YLEV[1], TDS_YLEV_P064);  
		vRegWriteFldAlign(TDSHP_2A, TDS_YLEV[2], TDS_YLEV_P128);  
		vRegWriteFldAlign(TDSHP_2A, TDS_YLEV[3], TDS_YLEV_P192);
		vRegWriteFldAlign(TDSHP_2B, TDS_YLEV[4], TDS_YLEV_P256); 
		vRegWriteFldAlign(TDSHP_2B, TDS_YLEV[5], TDS_YLEV_P320);  
		vRegWriteFldAlign(TDSHP_2B, TDS_YLEV[6], TDS_YLEV_P384);  
		vRegWriteFldAlign(TDSHP_2B, TDS_YLEV[7], TDS_YLEV_P448);			
		vRegWriteFldAlign(TDSHP_2C, TDS_YLEV[8], TDS_YLEV_P512);  
		vRegWriteFldAlign(TDSHP_2C, TDS_YLEV[9], TDS_YLEV_P576);  
		vRegWriteFldAlign(TDSHP_2C, TDS_YLEV[10], TDS_YLEV_P640); 
		vRegWriteFldAlign(TDSHP_2C, TDS_YLEV[11], TDS_YLEV_P704); 
		vRegWriteFldAlign(TDSHP_2D, TDS_YLEV[12], TDS_YLEV_P768); 
		vRegWriteFldAlign(TDSHP_2D, TDS_YLEV[13], TDS_YLEV_P832); 
		vRegWriteFldAlign(TDSHP_2D, TDS_YLEV[14], TDS_YLEV_P896); 
		vRegWriteFldAlign(TDSHP_2D, TDS_YLEV[15], TDS_YLEV_P960); 
		vRegWriteFldAlign(TDSHP_2E, TDS_YLEV[16], TDS_YLEV_P1024);   
}

/**
 * @brief Read Post sharpness Y Level Table
 * @param
 */
BOOL bDrvReadYLevTable(void)
{
	UINT8 bIndex;
	for (bIndex = 0; bIndex < 17; bIndex++)
	{
		Printf("YLEV = %d, ", bIndex*64);
		Printf("Table = %d\n", TDS_YLEV[bIndex]);

    	}
	return SV_TRUE;
}

/**
 * @brief Calculate manual gain curve with 4 control points
 * @param
 */
void vDrvCalYLevTable(void)
{

    UINT8 i, bIndex, bPoint[4], bGain[4];

    bPoint[0] = IO32ReadFldAlign(TDPROC_YLEV_01, TDS_YLEV_P1);
    bPoint[1] = IO32ReadFldAlign(TDPROC_YLEV_01, TDS_YLEV_P2);
    bPoint[2] = IO32ReadFldAlign(TDPROC_YLEV_01, TDS_YLEV_P3);
    bPoint[3] = IO32ReadFldAlign(TDPROC_YLEV_01, TDS_YLEV_P4);
    bGain[0] = IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_G1);        
    bGain[1] = IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_G2);        
    bGain[2] = IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_G3);
    bGain[3] = IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_G4);


    for (i = 0; i < 3; i ++)    // Make P1-P4 monotonic increasement
    {
        if (bPoint[i] >= bPoint[i+1])
        {
            bPoint[i] = bPoint[i+1] - 1;
        }
    }

    for (bIndex = 0; bIndex < 17; bIndex ++)
    {
        if (bIndex <= bPoint[0])
        {
            TDS_YLEV[bIndex] = bGain[0];
        }
        else if (bIndex >= bPoint[3])
        {
            TDS_YLEV[bIndex] = bGain[3];
        }
        else
        {
            for (i = 0; i <= 2; i ++)
            {
                if ((bIndex >= bPoint[i]) && ((bIndex <= bPoint[i+1])))
                {
                    TDS_YLEV[bIndex] = (UINT16)((bGain[i+1] - bGain[i]) *
                                    ((UINT16)bIndex - bPoint[i]) / (bPoint[i+1] - bPoint[i])
                                    + bGain[i]);
                    break;
                }
            }
        }
    }

}

/**
 * @brief Calculate adaptive gain curve 
 * @param
 */

void vDrvCalYLevAdlTable(void)
{ 
    UINT8 i;
    UINT16 wTmp;
    UINT8 ADL_GAIN;
      
    ADL_GAIN = IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_ADL_GAIN); 
    
    // Calculate gain for even points
    for (i = 0; i < 16; i ++)
    {
        wTmp = waLumaArray[2*i + 1] - waLumaArray[2*i];

        // slope = 0x20/wTmp, Gain=((slope*0x80)-0x80)*(Adap_Gain/0x80)+0x80
        TDS_ADAP_YLEV[i ] = (wTmp == 0) ? 0xFF : 
            ((((0x1000/ wTmp) - 0x80) * ADL_GAIN)>> 7) + 0x80;
    }
    // Calculate gain for index 17
    TDS_ADAP_YLEV[16] = (2 * TDS_ADAP_YLEV[15] - TDS_ADAP_YLEV[14]);

    // Multiply adaptive gain curve by manual gain curve
    for (i = 0; i < 17; i ++)
    {
        wTmp = (TDS_ADAP_YLEV[i] * TDS_YLEV[i]) >> 7;
        TDS_YLEV[i] = (wTmp > 0x80) ? 0x80 : wTmp;
    }
}

void vDrvCalYLevAplTable(void)
{
    UINT8 bIndex;
    UINT8 bPoint[2];
    UINT16 wTmp;    	
    UINT8 bAPL = bDrvGetAPL();       
    // YLEV_APL_TBL 	
    bPoint[0] = IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_APL_ZERO);
    bPoint[1] = IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_APL_THR);  
    
    if ( bPoint[0]> bPoint[1])	// Make Table monotonic increasement
    {
         bPoint[0] =  bPoint[1];
    }
    
    for (bIndex = 0; bIndex < 17; bIndex ++)	// APLLUT Calculation 
    {
        if (bIndex <= bPoint[0])
        {
            TDS_YLEV_APL[bIndex] = 0x00;
        }
        else if (bIndex >= bPoint[1])
        {
            TDS_YLEV_APL[bIndex] = 0x80;
        }        
        else
        {
            TDS_YLEV_APL[bIndex] = (UINT16)(0x80 *((UINT16)bIndex - bPoint[0]) / (bPoint[1] - bPoint[0]));
        }    
    }
    	  
    for (bIndex = 0; bIndex < 17; bIndex++)	// YLEV Blending by YLEV_APL
    {
        wTmp = (TDS_YLEV[bIndex]*(0x80 - TDS_YLEV_APL[bAPL >> 4] ) + 0x80 * TDS_YLEV_APL[bAPL >> 4] ) >> 7;
        TDS_YLEV[bIndex] = (wTmp > 0x80) ? 0x80 : wTmp;   
    }   
}

/**
 * @brief Enable/Disable adaptive Y level dependent sharpness
 * @param
 */
void vDrvAdapYLev(void)
{
    if (fgAdapYLEV == SV_ON && IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_ADAP_ENA))
    {
       vDrvCalYLevTable();
       if (IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_ADL_ENA))
       {
	   vDrvCalYLevAdlTable(); 
       }
       if (IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_APL_ENA))
       {	
 	   vDrvCalYLevAplTable();        
       }
       vApiRegisterVideoEvent(PE_EVENT_SHARPNESS, SV_VP_MAIN, SV_ON);
    }  
}

/*****************************************************************************************/
/*********************************  Motion Adaptiv SHARP ************************************/
/*****************************************************************************************/
void vDrvMoSharpInit(void)
{
    vIO32WriteFldAlign(MOADAP_SHARP_00, bMoShpEn, MO_SHP_EN);    
    vIO32WriteFldAlign(MOADAP_SHARP_00, 0x04, MO_SHP_XLUT_GAIN);        
    vIO32WriteFldAlign(MOADAP_SHARP_00, 0x04, MO_SHP_YLUT_GAIN);  
    vIO32WriteFldAlign(MOADAP_SHARP_00, 0x08, MO_SHP_LEVEL);    
}

UINT8 vDrvMoSharpXLevelLUT(UINT8 LEVEL)
{
    UINT8 Gain;
    Gain = IO32ReadFldAlign(MOADAP_SHARP_00, MO_SHP_XLUT_GAIN); 
    LEVEL = (LEVEL*Gain)>>2;
    return (LEVEL<8)?LEVEL:8;
}

UINT8 vDrvMoSharpYLevelLUT(UINT8 LEVEL)
{
    UINT8 Gain;
    Gain = IO32ReadFldAlign(MOADAP_SHARP_00, MO_SHP_YLUT_GAIN); 
    LEVEL = (LEVEL*Gain)>>2;
    return (LEVEL<8)?LEVEL:8;
}

UINT8 vDrvGetMoSharpXLevel(void)
{
    UINT8 RM_LEVEL;		// RM_LEVEL 	: RandomMotionLEVEL
    UINT8 XLEVEL;			// XLEVEL 	: X-axis Motion LEVEL
    UINT8 MVX;			// MVX		: X-axis Motion Value

// Get RandomMotionLEVEL   
    RM_LEVEL = 0;			// TODO : Random Motion Detection
    
// Get X-axis Global Motion Status
	if (RegReadFldAlign(MCVP_CS_28,GMV_VALID))
	{
		MVX = RegReadFldAlign(MCVP_CS_28,GMV_MVX);
		XLEVEL = (MVX>=0x10)?(0x20-MVX):MVX;	 // 5 bit 2s compliment	
	       //LOG(1, "DI_MVX = %d\n", XLEVEL);
	}	
	else  
	{
		XLEVEL = RM_LEVEL;		
	}
       vIO32WriteFldAlign(MOADAP_SHARP_00, XLEVEL, MO_SHP_XLEVEL);   	
       //LOG(1, "XLEVEL = %d\n", XLEVEL);
   	return (vDrvMoSharpXLevelLUT(XLEVEL));
}

UINT8 vDrvGetMoSharpYLevel(void)
{
    UINT8 RM_LEVEL;		// RM_LEVEL 	: RandomMotionLEVEL
    UINT8 YLEVEL;			// YLEVEL 	: Y-axis Motion LEVEL 
    UINT8 MVY;			// MVY		: Y-axis Motion Value

// Get RandomMotionLEVEL   
    RM_LEVEL = 0;			// TODO : Random Motion Detection
    
// Get Y-axis Global Motion Status
	if (RegReadFldAlign(MCVP_CS_28,GMV_VALID))
	{
		MVY = RegReadFldAlign(MCVP_CS_28,GMV_MVY);
		YLEVEL = (MVY>=8)?(0x10-MVY):MVY; // 4 bit 2s compliment	
	       //LOG(1, "DI_MVY = %d\n", YLEVEL);
	}	
	else  
	{
		YLEVEL = RM_LEVEL;		
	}
       vIO32WriteFldAlign(MOADAP_SHARP_00, YLEVEL, MO_SHP_YLEVEL);   	
       //LOG(1, "YLEVEL = %d\n", YLEVEL);
   	return (vDrvMoSharpYLevelLUT(YLEVEL));
}


UINT16 vDrvGetMoSharpValue(UINT16 wInput, UINT16 wOutput, UINT8 LEVEL)
{
    if (LEVEL>8)
    {
    	LEVEL=8;    	
    }   	
    return (wInput >= wOutput) 
        ? (wInput - (((wInput - wOutput)*LEVEL)>>3)) 
        : (wInput  + (((wOutput - wInput)*LEVEL)>>3));
}

void vDrvSetMoSharpValue(void)
{
	//DRVCUST_MoSharpSetQtyTbl();
     	return;
}

void vDrvMoSharpProc(void)
{  
    if (IO32ReadFldAlign(MOADAP_SHARP_00, MO_SHP_EN))
    {    
        if (bMoShpEn == SV_OFF)
        {
            bMoShpEn = SV_ON;
            LOG(1, "Enable Motion Adaptive Sharp \n");
        }
       
        vDrvSetMoSharpValue();
    }
    else
    {
        if (bMoShpEn == SV_ON)
        {
            bMoShpEn = SV_OFF;
            vDrvSetMoSharpValue();
            LOG(1, "Disable Motion Adaptive Sharp\n");
        }
    }  
}

/*****************************************************************************************/
/*********************************  BNR SHARP ************************************/
/*****************************************************************************************/
void vDrvBNRSharpInit(void)
{
    vIO32WriteFldAlign(BNR_SHARP_00, 1, BNR_SHP_EN);    
    vIO32WriteFldAlign(BNR_SHARP_00, 20, BNR_SHP_LEVEL);     
    bBNRShpEn = IO32ReadFldAlign(BNR_SHARP_00, BNR_SHP_EN);
}

UINT8 vDrvGetBlockNoiseLevel(void)
{
	UINT8 VBLKLevel, HBLKLevel;
	HBLKLevel = RegReadFldAlign(NXNR_20,NX_STA_HBLK_CONFIDENCE);
	VBLKLevel = RegReadFldAlign(NXNR_20,NX_STA_VBLK_CONFIDENCE);
	vBNRLevel = MAX(HBLKLevel,VBLKLevel);
	// Redefine Range from 0~0x80
	return (vBNRLevel<<3);
}

void vDrvSetBNRSharpValue(void)
{
//	DRVCUST_BNRSharpSetQtyTbl();
	#if !(defined(CC_MT5396) || defined(CC_MT5368))
		UINT8 vBNRSharpLevel = (vDrvGetBlockNoiseLevel()*IO32ReadFldAlign(BNR_SHARP_00, BNR_SHP_LEVEL))>>5;
	#endif	
	if (bBNRShpEn)
	{
		#if !(defined(CC_MT5396) || defined(CC_MT5368))
			vRegWriteFldAlign(PBC_SHP1_04, SV_ON, PBC1_LPF_EN);        
			vRegWriteFldAlign(PBC_SHP1_04, vBNRSharpLevel, PBC1_LPF_LEVEL);  
			vRegWriteFldAlign(PBC_SHP2_04, SV_ON, PBC2_LPF_EN);        
			vRegWriteFldAlign(PBC_SHP2_04, vBNRSharpLevel, PBC2_LPF_LEVEL);  
			vRegWriteFldAlign(PBC_SHP3_04, SV_ON, PBC3_LPF_EN);        
			vRegWriteFldAlign(PBC_SHP3_04, vBNRSharpLevel, PBC3_LPF_LEVEL);     	
		#endif
	}
	else
	{
		#if !(defined(CC_MT5396) || defined(CC_MT5368))	
			vRegWriteFldAlign(PBC_SHP1_04, wReadQualityTable(QUALITY_PBC_PBC1_LPF_EN), PBC1_LPF_EN);        
			vRegWriteFldAlign(PBC_SHP1_04, wReadQualityTable(QUALITY_PBC_PBC1_LPF_LEVEL), PBC1_LPF_LEVEL);  
			vRegWriteFldAlign(PBC_SHP2_04, wReadQualityTable(QUALITY_PBC_PBC2_LPF_EN), PBC2_LPF_EN);        
			vRegWriteFldAlign(PBC_SHP2_04, wReadQualityTable(QUALITY_PBC_PBC2_LPF_LEVEL), PBC2_LPF_LEVEL);  
			vRegWriteFldAlign(PBC_SHP3_04, wReadQualityTable(QUALITY_PBC_PBC3_LPF_EN), PBC3_LPF_EN);        
			vRegWriteFldAlign(PBC_SHP3_04, wReadQualityTable(QUALITY_PBC_PBC3_LPF_LEVEL), PBC3_LPF_LEVEL);      
		#endif
	}
}

void vDrvBNRSharpProc(void)
{  
    if (IO32ReadFldAlign(BNR_SHARP_00, BNR_SHP_EN))
    {    
        if (bBNRShpEn == SV_OFF)
        {
            bBNRShpEn = SV_ON;
        }
        vDrvSetBNRSharpValue();
    }
    else
    {
        if (bBNRShpEn == SV_ON)
        {
            bBNRShpEn = SV_OFF;
            vDrvSetBNRSharpValue();
        } 
    }  
}
/*****************************************************************************************/
/************************************  3D SHARP ******************************************/
/*****************************************************************************************/

UINT16 wDrvGetTDShp3DGain(UINT16 wInputGain, UINT16 wWeighting)
{
    return (wInputGain >= 0x40) 
        ? (0x40 + (((wInputGain - 0x40)*wWeighting)>>7)) 
        : (0x40 - (((0x40 - wInputGain)*wWeighting)>>7));
}

void vDrvSharp3DInit(void)
{
    vIO32WriteFldAlign(SHARPNESS_3D_00, bSharp3dEn, SHARPNESS_3D_EN);    
    vIO32WriteFldAlign(SHARPNESS_3D_00, 0x00, SHARPNESS_FORCE_3D_MODE);    
    vIO32WriteFldAlign(SHARPNESS_3D_01, 0x80, SHARPNESS_FS_H); 
    vIO32WriteFldAlign(SHARPNESS_3D_01, 0x80, SHARPNESS_FS_V);    
    vIO32WriteFldAlign(SHARPNESS_3D_01, 0x80, SHARPNESS_TTD_H);    
    vIO32WriteFldAlign(SHARPNESS_3D_01, 0x80, SHARPNESS_TTD_V);    
    vIO32WriteFldAlign(SHARPNESS_3D_02, 0x80, SHARPNESS_SBS_H);    
    vIO32WriteFldAlign(SHARPNESS_3D_02, 0x80, SHARPNESS_SBS_V);    
    vIO32WriteFldAlign(SHARPNESS_3D_02, 0x80, SHARPNESS_TB_H);    
    vIO32WriteFldAlign(SHARPNESS_3D_02, 0x80, SHARPNESS_TB_V);
    vIO32WriteFldAlign(SHARPNESS_3D_03, 0x80, SHARPNESS_SS_H);    
    vIO32WriteFldAlign(SHARPNESS_3D_03, 0x80, SHARPNESS_SS_V);    
    vIO32WriteFldAlign(SHARPNESS_3D_03, 0x80, SHARPNESS_RD_H);    
    vIO32WriteFldAlign(SHARPNESS_3D_03, 0x80, SHARPNESS_RD_V);  
}

void vDrvSetSharp3DGain(void)
{     
	vRegWriteFldAlign(TDSHP_10, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_H1), w3DWeightingH), TDS_GAIN_H1);
	vRegWriteFldAlign(TDSHP_10, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_H2), w3DWeightingH), TDS_GAIN_H2);
	vRegWriteFldAlign(TDSHP_10, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_H3), w3DWeightingH), TDS_GAIN_H3);
	vRegWriteFldAlign(TDSHP_11, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_V1), w3DWeightingV), TDS_GAIN_V1);
	vRegWriteFldAlign(TDSHP_11, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_V2), w3DWeightingV), TDS_GAIN_V2);
}

void vDrvGetSharp3DWeighting(void)
{
    UINT8 bMode;
    
    if (IO32ReadFldAlign(SHARPNESS_3D_00, SHARPNESS_FORCE_3D_MODE) != 0)
    {
        bMode = IO32ReadFldAlign(SHARPNESS_3D_00, SHARPNESS_FORCE_3D_MODE);
    }
    else
    {
        bMode = u4DrvTDTV3DModeQuery();
    }
    
    if (bSharp3dEn == SV_ON)
    {
        switch (bMode)
        {    
            case E_TDTV_UI_3D_MODE_TTD:			// 2D to 3D
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_01, SHARPNESS_TTD_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_01, SHARPNESS_TTD_V); 
                break;                
            case E_TDTV_UI_3D_MODE_FS:			//Frame Seq & Frame Packing
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_01, SHARPNESS_FS_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_01, SHARPNESS_FS_V); 
                break;                
            case E_TDTV_UI_3D_MODE_TB:  			//Top and Bottom
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_02, SHARPNESS_TB_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_02, SHARPNESS_TB_V);             
                break;
            case E_TDTV_UI_3D_MODE_SBS:			//Side by Side
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_02, SHARPNESS_SBS_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_02, SHARPNESS_SBS_V); 
                break;
            case E_TDTV_UI_3D_MODE_RD:			//RealD
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_03, SHARPNESS_RD_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_03, SHARPNESS_RD_V); 
                break;
            case E_TDTV_UI_3D_MODE_SS:   			//Sensio
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_03, SHARPNESS_SS_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_03, SHARPNESS_SS_V); 
                break;
  	     case E_TDTV_UI_3D_MODE_LI:  			//Line Interleave         
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_04, SHARPNESS_LI_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_04, SHARPNESS_LI_V); 
                break;
            case E_TDTV_UI_3D_MODE_CB:   		//Checker Board
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_04, SHARPNESS_CB_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_04, SHARPNESS_CB_V); 
                break;
            case E_TDTV_UI_3D_MODE_DA:   		//Dot Alternative
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_05, SHARPNESS_DA_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_05, SHARPNESS_DA_V); 
                break;
            case E_TDTV_UI_3D_MODE_OFF:
            default:
                w3DWeightingH = 0x80; 
                w3DWeightingV = 0x80; 
                break;
        }
    }
    else
    {
        w3DWeightingH = 0x80; 
        w3DWeightingV = 0x80; 
    }

    vIO32WriteFldAlign(SHARPNESS_3D_00, w3DWeightingH, SHARPNESS_OUT_WEI_H);    
    vIO32WriteFldAlign(SHARPNESS_3D_00, w3DWeightingV, SHARPNESS_OUT_WEI_V);     
}

void vDrvSharp3DProc(void)
{   
    if (IO32ReadFldAlign(SHARPNESS_3D_00, SHARPNESS_3D_EN))
    {    
        if (bSharp3dEn == SV_OFF)
        {
            bSharp3dEn = SV_ON;
            LOG(3, "Enable 3D HSharp Gain\n");
        }

        vDrvGetSharp3DWeighting();
        vDrvSetSharp3DGain();
    }
    else
    {
        if (bSharp3dEn == SV_ON)
        {
            bSharp3dEn = SV_OFF;
            vDrvGetSharp3DWeighting();
            vDrvSetSharp3DGain();
            LOG(3, "Disable 3D Sharp Gain\n");
        }
    }  
}



// SHARPNESS adaptive control : Auto SHARPNESS
void vDrvSharpProc(void)
{
    // SHARPNESS Driving
    vDrvPostSharpParamLimit();
    vDrvPBCProc();   
    // SHARPNESS ADAPTIVE
    vDrvAdapYLev();
    vDrvMoSharpProc(); 
    vDrvBNRSharpProc();
    // 3D SHARPNESS Driving
    vDrvSharp3DProc();    
}



/*****************************************************************************************/
/**********************************   Pattern Gen   ****************************************/
/*****************************************************************************************/
void vDrvMainSharpPatGen(UINT8 bOnOff)
{
	if (bOnOff == SV_ON)
	{
        vRegWriteFldAlign(TDPROC_MISC_00, 1, TDPROC_PATGEN_DATA_EN);
		vRegWriteFldAlign(TDPROC_MISC_00, 0, TDPROC_PATGEN_MODE);
		vRegWriteFldAlign(TDPROC_MISC_00, 1, TDPROC_PATGEN_YSEL);
		vRegWriteFldAlign(TDPROC_MISC_00, 1, TDPROC_PATGEN_USEL);
		vRegWriteFldAlign(TDPROC_MISC_00, 1, TDPROC_PATGEN_VSEL);
	}
	else
	{
		vRegWriteFldAlign(TDPROC_MISC_00, 0, TDPROC_PATGEN_DATA_EN);
		vRegWriteFldAlign(TDPROC_MISC_00, 0, TDPROC_PATGEN_YSEL);
		vRegWriteFldAlign(TDPROC_MISC_00, 0, TDPROC_PATGEN_USEL);
		vRegWriteFldAlign(TDPROC_MISC_00, 0, TDPROC_PATGEN_VSEL);
	}
}


/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
void vDrvSetPostSharpDemoRange(Region rRegion)
{
        //Define demo region
        vRegWriteFldAlign(SP_MASK_00, rRegion.wHStart, CH1_H_DMWINDOW_START);
        vRegWriteFldAlign(SP_MASK_00, rRegion.wHEnd, CH1_H_DMWINDOW_END);// 2DSharp : start < active < End                                                                                                  
        vRegWriteFldAlign(SP_MASK_00, rRegion.wVStart, CH1_V_DMWINDOW_START); // 2D Sharp Demo mode Voffset
        vRegWriteFldAlign(SP_MASK_00, rRegion.wVEnd, CH1_V_DMWINDOW_END);// 2D Sharp Demo mode Voffset
}

void vDrv2DSharpDemo(UINT8 bOnOff, Region rRegion)
{
    vRegWriteFldAlign(TDPROC_MISC_00, ((rRegion.u1OnOff==SV_ON)?0:1), TDS_DEMO_SWAP);
    vRegWriteFldAlign(TDPROC_MISC_00, (bOnOff==SV_ON?1:0), TDS_DEMO_ENA);
  
}

void vDrvECTIDemo(UINT8 bOnOff, Region rRegion)
{
    vRegWriteFldAlign(TDPROC_MISC_00, ((rRegion.u1OnOff==SV_ON)?0:1), ECTI_DEMO_SWAP);
    vRegWriteFldAlign(TDPROC_MISC_00, (bOnOff==SV_ON?1:0), ECTI_DEMO_ENA);
}

