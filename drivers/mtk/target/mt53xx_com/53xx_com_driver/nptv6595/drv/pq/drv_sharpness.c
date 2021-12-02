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
#include "vdo_misc.h"
#include "nptv_debug.h"
#include "drv_contrast.h"
#include "drv_meter.h"
#include "pe_if.h"
#include "drv_scaler.h"
#include "drv_tdtv_drvif.h"

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
#include "drv_scaler_drvif.h"
#endif
// === HW INCLUDE =============================================================================
#include "hw_ycproc.h"
#include "hw_sw.h"
#include "hw_di_int.h"
// === DEFINE =============================================================================
#define CHROMACORING_OFFSET     0x200

// === FUNCTION PROTOTYPE =================================================================
void vDrvAdapHShpInit(void);
void vDrvSharp3DInit(void);
void vDrvMoSharpInit(void);

// === EXTERN FUNCTION ====================================================================
#ifdef CC_MT5395
EXTERN  UINT8 u1DrvMJCGetGmvX(void); // integer pixel
EXTERN  UINT8 u1DrvMJCGetGmvY(void); // integer pixel
#endif
// === GLOBAL VARIABLE ====================================================================

// === EXTERN VARIABLE ====================================================================
static BOOL fgAdapYLEV = SV_ON;
static UINT16 TDS_ADAP_YLEV[65];

UINT8 TDS_LC_H[65] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x03, 0x05, 0x06, 0x08, 0x0A, 0x0B,
    0x0D, 0x0E, 0x10, 0x12, 0x13, 0x15, 0x16, 0x18,
    0x1A, 0x20, 0x26, 0x2D, 0x33, 0x3A, 0x40, 0x46,
    0x4D, 0x53, 0x5A, 0x60, 0x66, 0x6D, 0x73, 0x7A,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80	
};

UINT8 TDS_YLEV[65] =
{
	0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
	0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
	0x60, 0x60, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65,
	0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d,
	0x6e, 0x6f, 0x70, 0x71, 0x74, 0x77, 0x7a, 0x7d,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80
};

UINT8 TDS_YLEV_APL[65] =
{
       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
       0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
       0x80       
};

static RRegDwrdTbl const REGTBL_HSHARP[] =
{
	{  HSHARP_MAIN_00, 0x80434040, 0xF0FFFFFF},
	{  HSHARP_MAIN_01, 0x80434040, 0x80FFFFFF},
	{  HSHARP_MAIN_02, 0x00020202, 0x70FFFFFF},
	{  HSHARP_MAIN_03, 0x00FFFFFF, 0x70FFFFFF},
	{  HSHARP_MAIN_04, 0x00FFFFFF, 0x70FFFFFF},
	{  HSHARP_MAIN_05, 0xC0C0FFFF, 0xFFC0FFFF},
	{  HSHARP_MAIN_06, 0x01000000, 0xF3FFFFFF},
	{  HSHARP_MAIN_07, 0x004A4340, 0x00FFFFFF},
	{  HSHARP_MAIN_08, 0x004A4340, 0x00FFFFFF},
	{  HSHARP_MAIN_09, 0x00020202, 0x70FFFFFF},
	{  HSHARP_MAIN_0A, 0x00FFFFFF, 0x70FFFFFF},
	{  HSHARP_MAIN_0B, 0x00FFFFFF, 0x70FFFFFF},
	{  HSHARP_MAIN_0C, 0x00000005, 0x0000080F},
	{  HSHARP_MAIN_0D, 0x40100000, 0xE0100003},
	{  HSHARP_MAIN_0E, 0x00000000, 0xFFFFFFFF},
	{  HSHARP_MAIN_0F, 0x00000780, 0x9FFF1FFF},
	{  HSHARP_MAIN_12, 0x00000000, 0x0000000F},
	{  HSHARP_MAIN_13, 0x02400000, 0x1FFF0000},
	{  REG_END, 0x00000000, 0x00000000}	
};

static RRegDwrdTbl const REGTBL_TDSHARP[] =
{
	{  TDPROC_00, 0x59FFFF09, 0xFFFFFFFF},
	{  TDPROC_01, 0x06061080, 0xFFFF17BF},
	{  TDPROC_02, 0x00002040, 0x0000F3FF},
	{  TDPROC_03, 0x59000000, 0xFF000000},
	{  TDPROC_04, 0x47FFFF09, 0xFFFFFFFF},
	{  TDPROC_05, 0x06061090, 0xFFFF17BF},
	{  TDPROC_06, 0x00002040, 0x0000F3FF},
	{  TDPROC_07, 0x47000000, 0xFF000000},
	{  TDPROC_08, 0x51FFFF09, 0xFFFFFFFF},
	{  TDPROC_09, 0x06060082, 0xFFFF178F},
	{  TDPROC_0A, 0x00002040, 0x0000F3FF},
	{  TDPROC_0B, 0x51000000, 0xFF000000},
	{  TDPROC_0C, 0x44FFFF09, 0xFFFFFFFF},
	{  TDPROC_0D, 0x06060082, 0xFFFF178F},
	{  TDPROC_0E, 0x00000040, 0x0000F3FF},
	{  TDPROC_0F, 0x44000000, 0xFF000000},
	{  TDPROC_10, 0x51FFFF09, 0xFFFFFFFF},
	{  TDPROC_11, 0x06060084, 0xFFFF178F},
	{  TDPROC_12, 0x00000040, 0x0000F3FF},
	{  TDPROC_13, 0x51000000, 0xFF000000},
	{  TDPROC_14, 0x44FFFF09, 0xFFFFFFFF},
	{  TDPROC_15, 0x06060084, 0xFFFF178F},
	{  TDPROC_16, 0x00000040, 0x0000F3FF},
	{  TDPROC_17, 0x44000000, 0xFF000000},
	{  TDPROC_20, 0x68FFFF04, 0xFFFFFFFF},
	{  TDPROC_21, 0x00000080, 0xFFFF00B0},
	{  TDPROC_22, 0x00000040, 0x0000F0FF},
	{  TDPROC_23, 0x40000000, 0xFF000000},
	{  TDPROC_24, 0xB63FC3FC, 0xFFBFF3FF},
	{  TDPROC_26, 0x38063210, 0xFFF400FF},
	{  TDPROC_27, 0x0000000E, 0x0000000F},
	{  TDPROC_28, 0x00008040, 0x0000FF7F},
	{  TDPROC_29, 0x00800060, 0x00FF00FF},
	{  TDPROC_2C, 0x18060400, 0x1FFFF7FF},
	{  TDPROC_34, 0x55FFFF00, 0xFFFFFFFF},
	{  TDPROC_35, 0x00000180, 0xFFFF03B0},
	{  TDPROC_36, 0x00000080, 0x0000F0FF},
	{  TDPROC_37, 0x55000000, 0xFF000000},
	{  TDPROC_6B, 0x00000103, 0x000001FF},
	{  TDPROC_6C, 0x04030201, 0xFFFFFFFF},
	{  TDPROC_6D, 0x08070605, 0xFFFFFFFF},
	{  TDPROC_6E, 0x0D0C0804, 0xFFFFFFFF},
	{  TDPROC_6F, 0x11100F0E, 0xFFFFFFFF},
	{  TDPROC_B0, 0x42FFA048, 0x67FFFFFF},
	{  TDPROC_B1, 0x00000089, 0x00FF03FF},
	{  TDPROC_B2, 0x0200B080, 0x03FFFFFF},
	{  TDPROC_B3, 0x02600330, 0x03FF03FF},
	{  TDPROC_C0, 0x04080005, 0x8708000F},
	{  TDPROC_C2, 0x00C0140A, 0x03FFFFFF},
	{  TDPROC_C3, 0x3FC3FC80, 0x3FF3FFFF},
	{  PBC_SHP1_01, 0x08083030, 0xFFFFFFFF},// Green Grass
	{  PBC_SHP1_02, 0x01FF01FF, 0x01FF01FF},
	{  PBC_SHP1_03, 0x01FC0068, 0x03FF03FF},
	{  PBC_SHP1_04, 0x00000000, 0x0FFF0FFF},
	{  PBC_SHP1_05, 0x00800000, 0x03FF0FFF},
	{  PBC_SHP2_01, 0x08046060, 0xFFFFFFFF},// Skin Tone
	{  PBC_SHP2_02, 0x01FF01FF, 0x01FF01FF},
	{  PBC_SHP2_03, 0x01800080, 0x03FF03FF},
	{  PBC_SHP2_04, 0x002A002A, 0x0FFF0FFF},
	{  PBC_SHP2_05, 0x0020002A, 0x03FF0FFF},
	{  PBC_SHP3_01, 0x00004080, 0xFFFFFFFF},// Ocean/Sky Blue
	{  PBC_SHP3_02, 0x01FF01FF, 0x01FF01FF},
	{  PBC_SHP3_03, 0x038000F0, 0x03FF03FF},
	{  PBC_SHP3_04, 0x00000000, 0x0FFF0FFF},
	{  PBC_SHP3_05, 0x00800000, 0x03FF0FFF},
	{  TDPROC_B4, 0x02008080, 0x03FFFFFF},
	{  TDPROC_B5, 0x02000200, 0x03FF03FF},
	{  TDPROC_B6, 0x02008080, 0x03FFFFFF},
	{  TDPROC_B7, 0x02000200, 0x03FF03FF},
//	{  SP_MASK_01, 0x00000000, 0x3FFFFFFF},
	{  REG_END, 0x00000000, 0x00000000}
};

static RRegDwrdTbl const REGTBL_ECTIF[] =
{	
	{  ECTIF_00, 0x08800200, 0xDFF01AF9},
	{  ECTIF_01, 0x00000000, 0x07777FFF},
	{  ECTIF_02, 0x00100707, 0xFFFFFFF7},
	{  ECTIF_03, 0x00100707, 0xFFFFFFF7},
	{  ECTIF_04, 0x00200701, 0xFFFFFF77},
	{  ECTIF_05, 0x003FF3FF, 0xFF7FF7FF},
	{  ECTIF_06, 0x00000000, 0x72000000},
	{  ECTIF_07, 0x00000000, 0x03FFFF00},
	{  ECTIF_08, 0x00000000, 0x0001FFFF},
	{   REG_END, 0x00000000, 0x00000000},	
};

static RRegDwrdTbl const REGTBL_ECTIR[] =
{
	{  ECTI_00, 0xE8801BF1, 0xFFF03BF9},
	{  ECTI_01, 0x00043714, 0x07777FFF},
	{  ECTI_02, 0x10081006, 0xFFFFFFF7},
	{  ECTI_03, 0x10081006, 0xFFFFFFF7},
	{  ECTI_04, 0x60000007, 0xFFFFFF77},
	{  ECTI_05, 0x0000000B, 0x03FFFFFF},
	{  ECTI_06, 0x003FF3FF, 0x007FF7FF},
	{  ECTI_07, 0x00FFF000, 0x00FFFFFF},
	{  ECTI_08, 0x00FFF000, 0x00FFFFFF},
	{  ECTI_09, 0x00FFF000, 0x00FFFFFF},
	{  ECTI_0A, 0x00FFF000, 0x03FFFFFF},
	{  ECTI_0C, 0x00000000, 0x0001FFFF},
	{	REG_END, 0x00000000, 0x00000000},

};

#ifdef CC_MT5395
static RRegDwrdTbl const REGTBL_CBE[] = 
{
	{  CBE_00, 0x8008203F, 0xFF7F7F7F},
	{  CBE_01, 0x20200416, 0x7F7F7F7F},
	{  CBE_02, 0x2020083F, 0x7F7F7F7F},
	{  CBE_03, 0x00FFF000, 0xC0FFFFFF},
	{  CBE_04, 0x00FFF000, 0x00FFFFFF},
	{  REG_END, 0x00000000, 0x00000000},
};
#endif

static RRegDwrdTbl const REGTBL_LTI[] =
{
	{	LTI_00, 0x4CFFFF08, 0xFFFFFFFF}, 
	{	LTI_01, 0x00001080, 0xFFFF17BF}, 
	{	LTI_02, 0x00000040, 0x0000F3FF}, 
	{	LTI_03, 0x4C000000, 0xFF000000}, 
	{	LTI_04, 0x40FFFF04, 0xFFFFFFFF}, 
	{	LTI_05, 0x00001080, 0xFFFF17BF}, 
	{	LTI_06, 0x00000040, 0x0000F3FF}, 
	{	LTI_07, 0x40000000, 0xFF000000}, 
	{	HLTI_01, 0x2E0F5B0F, 0xFFFFFFFF},
	{   HLTI_00, 0x00000003, 0x00000003},
	{   HLTI_02, 0x03FF0000, 0x03FF03FF},
	{   HLTI_03, 0x00000020, 0x00007FFF},
	{   HLTI_04, 0x3FFB0000, 0x3FFFFFFF},
	{   HLTI_05, 0x00000020, 0x7FFF7FFF},
	{	REG_END, 0x00000000, 0x00000000},	
};

static UINT8 const CCORING_TBL[16] =
{
	0x00, 0x01, 0x02, 0x05, 0x08, 0x0D, 0x12, 0x19,
	0x20, 0x27, 0x2E, 0x33, 0x38, 0x3B, 0x3E, 0x3F
};

#define ADAP_SHP_MAX_POINT 10
static UINT8 bAdapShpEn = SV_FALSE;
static UINT8 bAdapShpPoints = 8;
static UINT8 bAdapShpWeighting = 255;
static UINT8 bAdapShpTable[ADAP_SHP_MAX_POINT][2] = 
{
    {16,128},
    {24,160},
    {32,192},
    {64,232},
    {96,248},
    {144,255},
    {192,255},
    {235,255},
    {0,0},
    {0,0}
};

UINT16 w3DWeightingH = 0x80;
UINT16 w3DWeightingV = 0x80;
UINT8 bSharp3dEn = SV_FALSE;
UINT8 bMoShpEn = SV_TRUE;

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
	vRegWriteFldAlign(ECTIF_00, bOnOff, ECTIF_ENA);
}

/**
 * @brief set main path CTIF initial parameters
 */
void vDrvCTIFInit(void)
{
	vDrvLoadRegDwrdTbl(REGTBL_ECTIF);
}

/**
 * @brief set CTIF parameters
 * @param prCTIParam CTIF parameters structure
 */
void vDrvCTIFParam(void)
{
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
#ifdef CC_MT5395
/**
 * @brief CTIF on/off
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvCBEOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(CBE_00, bOnOff, CBEENABLE);
}

/**
 * @brief set main path CTIF initial parameters
 */
void vDrvCBEInit(void)
{
	vDrvLoadRegDwrdTbl(REGTBL_CBE);
}

/**
 * @brief set CBE parameters
 * @param void
 */
void vDrvCBELoadParam(void)
{
	vRegWriteFldAlign(CBE_00,wReadQualityTable(QUALITY_CBE_WEI_IT3),CBE_W_IT3);
	vRegWriteFldAlign(CBE_00,wReadQualityTable(QUALITY_CBE_WEI_IT5),CBE_W_IT5);                                               
}
#endif	

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

    #ifdef CC_SCPOS_3DTV_SUPPORT
	if (IS_LVDS_DISP_3D_POLARIZED && (BSP_GetIcVersion() < IC_VER_5395_AB))
	{
		vRegWriteFldAlign(TDPROC_24, bOnOff, TDS_LTI_EN); // Horizontal LTI
		vRegWriteFldAlign(HLTI_00, 0, HLTI_EN); // 2D LTI
	}
	else
    #endif
    #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    if (fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
        || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
        || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR)
        || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
    {
        vRegWriteFldAlign(TDPROC_24, SV_OFF, TDS_EN);
    }
    else
    #endif    
	{
    	vRegWriteFldAlign(TDPROC_24, bOnOff, TDS_LTI_EN);
    	vRegWriteFldAlign(HLTI_00, bOnOff, HLTI_EN);
    }
}

/**
 * @brief set main path LTI initial parameters
 */
void vDrvLTIInit(void)
{
	vDrvLoadRegDwrdTbl(REGTBL_LTI);
}

/**
 * @brief set LTI parameters
 * @param prLTIParam LTi parameters structure
 */
void vDrvLTIParam(void)
{
	vRegWriteFldAlign(LTI_00, wReadQualityTable(QUALITY_LTI_GAIN1), LTI_GAIN1);
	vRegWriteFldAlign(LTI_03, wReadQualityTable(QUALITY_LTI_GAIN_NEG1), LTI_GAIN_NEG1);
	vRegWriteFldAlign(LTI_00, wReadQualityTable(QUALITY_LTI_LIMIT_POS1), LTI_LIMIT_POS1);
	vRegWriteFldAlign(LTI_00, wReadQualityTable(QUALITY_LTI_LIMIT_NEG1), LTI_LIMIT_NEG1);
	vRegWriteFldAlign(LTI_00, wReadQualityTable(QUALITY_LTI_CORING1), LTI_CORING1);
	vRegWriteFldAlign(LTI_02, wReadQualityTable(QUALITY_LTI_SOFT_CLIP_GAIN1), LTI_SOFT_CLIP_GAIN1);
	vRegWriteFldAlign(LTI_04, wReadQualityTable(QUALITY_LTI_GAIN2), LTI_GAIN2);
	vRegWriteFldAlign(LTI_07, wReadQualityTable(QUALITY_LTI_GAIN_NEG2), LTI_GAIN_NEG2);
	vRegWriteFldAlign(LTI_04, wReadQualityTable(QUALITY_LTI_LIMIT_POS2), LTI_LIMIT_POS2);
	vRegWriteFldAlign(LTI_04, wReadQualityTable(QUALITY_LTI_LIMIT_NEG2), LTI_LIMIT_NEG2);
	vRegWriteFldAlign(LTI_04, wReadQualityTable(QUALITY_LTI_CORING2), LTI_CORING2);
	vRegWriteFldAlign(LTI_06, wReadQualityTable(QUALITY_LTI_SOFT_CLIP_GAIN2), LTI_SOFT_CLIP_GAIN2);
	vRegWriteFldAlign(HLTI_01, wReadQualityTable(QUALITY_HLTI_HDEG_GAIN), HLTI_HDEG_GAIN);
	vRegWriteFldAlign(HLTI_01, wReadQualityTable(QUALITY_HLTI_HDIFF_OFFSET), HLTI_HDIFF_OFFSET);
	vRegWriteFldAlign(HLTI_01, wReadQualityTable(QUALITY_HLTI_VDEG_GAIN), HLTI_VDEG_GAIN);
	vRegWriteFldAlign(HLTI_01, wReadQualityTable(QUALITY_HLTI_VDIFF_OFFSET), HLTI_VDIFF_OFFSET);
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
	if(bPath!=SV_VP_MAIN)	//skip sub path selection
		return;

	vRegWriteFldAlign(ECTIF_00, ((bPath==SV_VP_MAIN)?0:1), ECTIF_SRC_SEL);	
	vRegWriteFldAlign(HSHARP_MAIN_00, bOnOff, HSP_SHARP_EN);
}

/**
 * @brief set main path HSHARP initial parameters
 */
void vDrvPreSharpInit(void)
{
    vDrvLoadRegDwrdTbl(REGTBL_HSHARP);
    vDrvAdapHShpInit();
}

/**
 * @brief set main path HSHARPF parameters
 */
void vDrvPreSharpParam(UINT8 bPath)
{
    UNUSED(bPath);

    //  Global Gain
	vRegWriteFldAlign(HSHARP_MAIN_00,
        wReadQualityTable(QUALITY_HSHARP_HIGH), HSP_SHARP_HIGH);
	vRegWriteFldAlign(HSHARP_MAIN_00,
        wReadQualityTable(QUALITY_HSHARP_MID), HSP_SHARP_MID);
	vRegWriteFldAlign(HSHARP_MAIN_00,
        wReadQualityTable(QUALITY_HSHARP_LOW), HSP_SHARP_LOW);
	vRegWriteFldAlign(HSHARP_MAIN_01,
        wReadQualityTable(QUALITY_HSHARP_HIGH_NEG), HSP_SHARP_HIGH_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_01,
        wReadQualityTable(QUALITY_HSHARP_MID_NEG), HSP_SHARP_MID_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_01,
        wReadQualityTable(QUALITY_HSHARP_LOW_NEG), HSP_SHARP_LOW_NEG);

    //  Global Coring
	vRegWriteFldAlign(HSHARP_MAIN_02,
        wReadQualityTable(QUALITY_HSHARP_HIGH_CORING), HSP_SHARP_HIGH_CORING);
	vRegWriteFldAlign(HSHARP_MAIN_02,
        wReadQualityTable(QUALITY_HSHARP_MID_CORING), HSP_SHARP_MID_CORING);
	vRegWriteFldAlign(HSHARP_MAIN_02,
        wReadQualityTable(QUALITY_HSHARP_LOW_CORING), HSP_SHARP_LOW_CORING);

    //  Global Limit
	vRegWriteFldAlign(HSHARP_MAIN_03,
        wReadQualityTable(QUALITY_HSHARP_HIGH_LIMIT_POS), HSP_SHARP_HIGH_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_03,
        wReadQualityTable(QUALITY_HSHARP_MID_LIMIT_POS), HSP_SHARP_MID_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_03,
        wReadQualityTable(QUALITY_HSHARP_LOW_LIMIT_POS), HSP_SHARP_LOW_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_04,
        wReadQualityTable(QUALITY_HSHARP_HIGH_LIMIT_NEG), HSP_SHARP_HIGH_LIMIT_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_04,
        wReadQualityTable(QUALITY_HSHARP_MID_LIMIT_NEG), HSP_SHARP_MID_LIMIT_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_04,
        wReadQualityTable(QUALITY_HSHARP_LOW_LIMIT_NEG), HSP_SHARP_LOW_LIMIT_NEG);

    //  Global Clip
	vRegWriteFldAlign(HSHARP_MAIN_05,
        wReadQualityTable(QUALITY_HSHARP_CLIP_POS_EN), HSP_CLIP_POS_EN);
	vRegWriteFldAlign(HSHARP_MAIN_05,
        wReadQualityTable(QUALITY_HSHARP_CLIP_NEG_EN), HSP_CLIP_NEG_EN);
	vRegWriteFldAlign(HSHARP_MAIN_05,
        wReadQualityTable(QUALITY_HSHARP_GB_CLIP_POS_EN_H), HSP_GB_CLIP_POS_EN_H);
	vRegWriteFldAlign(HSHARP_MAIN_05,
        wReadQualityTable(QUALITY_HSHARP_GB_CLIP_NEG_EN_H), HSP_GB_CLIP_NEG_EN_H);
	vRegWriteFldAlign(HSHARP_MAIN_05,
        wReadQualityTable(QUALITY_HSHARP_GB_CLIP_POS_EN_M), HSP_GB_CLIP_POS_EN_M);
	vRegWriteFldAlign(HSHARP_MAIN_05,
        wReadQualityTable(QUALITY_HSHARP_GB_CLIP_NEG_EN_M), HSP_GB_CLIP_NEG_EN_M);
	vRegWriteFldAlign(HSHARP_MAIN_05,
        wReadQualityTable(QUALITY_HSHARP_GB_CLIP_POS_EN_L), HSP_GB_CLIP_POS_EN_L);
	vRegWriteFldAlign(HSHARP_MAIN_05,
        wReadQualityTable(QUALITY_HSHARP_GB_CLIP_NEG_EN_L), HSP_GB_CLIP_NEG_EN_L);
	vRegWriteFldAlign(HSHARP_MAIN_06,
        wReadQualityTable(QUALITY_HSHARP_CLIP_FRONT_THR_HIGH), HSP_CLIP_FRONT_THR_HIGH);
	vRegWriteFldAlign(HSHARP_MAIN_06,
        wReadQualityTable(QUALITY_HSHARP_CLIP_FRONT_THR_LOW), HSP_CLIP_FRONT_THR_LOW);
	vRegWriteFldAlign(HSHARP_MAIN_05,
        wReadQualityTable(QUALITY_HSHARP_CLIP_POS), HSP_CLIP_POS_TH);
	vRegWriteFldAlign(HSHARP_MAIN_05,
        wReadQualityTable(QUALITY_HSHARP_CLIP_NEG), HSP_CLIP_NEG_TH);
	vRegWriteFldAlign(HSHARP_MAIN_13,
        wReadQualityTable(QUALITY_HSHARP_SOFT_CLIP_GAIN), HSP_C_HSHP_SOFT_GAIN);	

    //  Local Gain
	vRegWriteFldAlign(HSHARP_MAIN_07,
        wReadQualityTable(QUALITY_HSHARP_LC_HIGH), HSP_LC_SHARP_HIGH);
	vRegWriteFldAlign(HSHARP_MAIN_07,
        wReadQualityTable(QUALITY_HSHARP_LC_MID), HSP_LC_SHARP_MID);
	vRegWriteFldAlign(HSHARP_MAIN_07,
        wReadQualityTable(QUALITY_HSHARP_LC_LOW), HSP_LC_SHARP_LOW);
	vRegWriteFldAlign(HSHARP_MAIN_08,
        wReadQualityTable(QUALITY_HSHARP_LC_HIGH_NEG), HSP_LC_SHARP_HIGH_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_08,
        wReadQualityTable(QUALITY_HSHARP_LC_MID_NEG), HSP_LC_SHARP_MID_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_08,
        wReadQualityTable(QUALITY_HSHARP_LC_LOW_NEG), HSP_LC_SHARP_LOW_NEG);
	
    //  Local Coring
	vRegWriteFldAlign(HSHARP_MAIN_09,
        wReadQualityTable(QUALITY_HSHARP_LC_HIGH_CORING), HSP_LC_SHARP_HIGH_CORING);
	vRegWriteFldAlign(HSHARP_MAIN_09,
        wReadQualityTable(QUALITY_HSHARP_LC_MID_CORING), HSP_LC_SHARP_MID_CORING);
	vRegWriteFldAlign(HSHARP_MAIN_09,
        wReadQualityTable(QUALITY_HSHARP_LC_LOW_CORING), HSP_LC_SHARP_LOW_CORING);

    //  Local Limit
	vRegWriteFldAlign(HSHARP_MAIN_0A,
        wReadQualityTable(QUALITY_HSHARP_LC_HIGH_LIMIT_POS), HSP_LC_SHARP_HIGH_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_0A,
        wReadQualityTable(QUALITY_HSHARP_LC_MID_LIMIT_POS), HSP_LC_SHARP_MID_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_0A,
        wReadQualityTable(QUALITY_HSHARP_LC_LOW_LIMIT_POS), HSP_LC_SHARP_LOW_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_0B,
        wReadQualityTable(QUALITY_HSHARP_LC_HIGH_LIMIT_NEG), HSP_LC_SHARP_HIGH_LIMIT_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_0B,
        wReadQualityTable(QUALITY_HSHARP_LC_MID_LIMIT_NEG), HSP_LC_SHARP_MID_LIMIT_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_0B,
        wReadQualityTable(QUALITY_HSHARP_LC_LOW_LIMIT_NEG), HSP_LC_SHARP_LOW_LIMIT_NEG);

    //  Local Clip
	vRegWriteFldAlign(HSHARP_MAIN_05,
        wReadQualityTable(QUALITY_HSHARP_LC_CLIP_POS_EN), HSP_LC_CLIP_POS_EN);
	vRegWriteFldAlign(HSHARP_MAIN_05,
        wReadQualityTable(QUALITY_HSHARP_LC_CLIP_NEG_EN), HSP_LC_CLIP_NEG_EN);

    //  Local Table Setting
	vRegWriteFldAlign(HSHARP_MAIN_06,
        wReadQualityTable(QUALITY_HSHARP_LC_TABLE_GAIN), HSP_LC_TABLE_GAIN);
	vRegWriteFldAlign(HSHARP_MAIN_06,
        wReadQualityTable(QUALITY_HSHARP_LC_TABLE_SEL), HSP_LC_TABLE_SEL);
	vRegWriteFldAlign(HSHARP_MAIN_06,
        wReadQualityTable(QUALITY_HSHARP_LC_TABLE_OFFSET), HSP_LC_TABLE_OFFSET);

    //  FLPF
	vRegWriteFldAlign(HSHARP_MAIN_12,
        wReadQualityTable(QUALITY_HSHARP_AC_LPF_EN), HSP_C_HSHP_AC_EN);
	vRegWriteFldAlign(HSHARP_MAIN_13,
        wReadQualityTable(QUALITY_HSHARP_AC_LPF_EN), HSP_C_HSHP_AC_COEF);
}

/*****************************************************************************************/
/************************************  2DSHARP *******************************************/
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
        vRegWriteFldAlign(TDPROC_24, SV_OFF, TDS_EN);
	}
	else
    #endif
    #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    if (fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
        || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
        || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR)
        || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
    {
        vRegWriteFldAlign(TDPROC_24, SV_OFF, TDS_EN);
    }
    else
    #endif
	{
		vRegWriteFldAlign(TDPROC_24, bOnOff, TDS_EN);
	}

	if (!bOnOff)
	{
		vRegWriteFldAlign(PBC_SHP1_01, 0, PBC_EN_1);
		vRegWriteFldAlign(PBC_SHP2_01, 0, PBC_EN_2);
		vRegWriteFldAlign(PBC_SHP3_01, 0, PBC_EN_3);
	}
}

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
	}
}

void vDrvLoadLocalTable(void)
{
	UINT8 bIndex;

	vRegWrite4B(TDPROC_27, 0x9);   //switch sram control to CPU
	for (bIndex = 0; bIndex < 65; bIndex++)
	{
		vRegWrite4B(TDPROC_28, ((UINT16)(TDS_LC_H[bIndex]) <<8) | bIndex);
		vRegWrite4B(TDPROC_27, 0xb);   //triggle write
		vRegWrite4B(TDPROC_27, 0x9);   //reset write
	}
	vRegWrite1B(TDPROC_27, 0x0);      //release sram control
}

BOOL bDrvReadLocalTable(void)    
{
	UINT8 bIndex;
	UINT8 u1ReadData;
	UINT8 i;
	UINT8 dummy = 0;	
	BOOL bResult = SV_TRUE;	
	
	vRegWrite4B(TDPROC_27, 0x1);     //switch sram control to CPU
	vUtDelay2us(1);
	for (bIndex = 0; bIndex < 65; bIndex++)
	{
		vRegWrite4B(TDPROC_28, bIndex);    //set read address

        // delay to ensure a safe read
        for(i = 0; i < 16; i++)
        {
                dummy += IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_P1);        
        }
        
		u1ReadData = u1RegRead1B(TDPROC_29);		
	    if(u1ReadData != TDS_LC_H[bIndex])
        {
            Printf("LocalTable index = %d, ", bIndex);
            Printf("LocalTable read data = %d, TDS_LC_H[i] = %d\n", u1ReadData, TDS_LC_H[bIndex]);
            bResult = SV_FALSE;	
        }
    }
    vRegWrite1B(TDPROC_27, 0x0);   //release SRAM control		

    return bResult;
}

/**
 * @brief Post sharpness Y Level Table
 * @param
 */
void vDrvLoadYLevTable(void)
{  
	UINT8 bIndex;

	vRegWriteFldMulti(TDPROC_2C, 
		P_Fld(0x80, TDS_YLEV_ALPHA) | 
		P_Fld(0, TDS_YLEV_LOAD) | 
		P_Fld(0xFF, TDS_YLEV_IND) | 
		P_Fld(1, TDS_YLEV_TBL_LD) | 
		P_Fld(0, TDS_YLEV_LOAD_BURST) | 
		P_Fld(0, TDS_YLEV_LOAD_ENA) | 
		P_Fld(1, TDS_YLEV_READ_ENA));

	for (bIndex = 0; bIndex < 65 ; bIndex++)
	{
		vRegWriteFldMulti(TDPROC_2C,
		                  P_Fld(TDS_YLEV[bIndex], TDS_YLEV_LOAD) |
		                  P_Fld(bIndex, TDS_YLEV_IND));
	
	    vRegWriteFldAlign(TDPROC_2C, 1, TDS_YLEV_LOAD_ENA);    //triggle write
	    vRegWriteFldAlign(TDPROC_2C, 0, TDS_YLEV_LOAD_ENA);    //reset write
	}
	
    vRegWriteFldMulti(TDPROC_2C,
	            P_Fld(0, TDS_YLEV_TBL_LD) |
	            P_Fld(0, TDS_YLEV_LOAD_BURST) |
	            P_Fld(0, TDS_YLEV_LOAD_ENA) |
	            P_Fld(0, TDS_YLEV_READ_ENA));	     //release sram control	
}

/**
 * @brief Read Post sharpness Y Level Table
 * @param
 */
BOOL bDrvReadYLevTable(void)
{
	UINT8 bIndex;
	UINT8 u1ReadData;
	UINT8 dummy;	
	BOOL bResult = SV_TRUE;
	
    vRegWriteFldMulti(TDPROC_2C,
        P_Fld(0, TDS_YLEV_TBL_LD) |
        P_Fld(0, TDS_YLEV_LOAD_BURST) |
        P_Fld(0, TDS_YLEV_LOAD_ENA) |
        P_Fld(1, TDS_YLEV_READ_ENA));

    for (bIndex = 0; bIndex < 65; bIndex++)
    {
        vRegWriteFldAlign(TDPROC_2C, bIndex, TDS_YLEV_IND);

        dummy = IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_P1);        
        dummy = IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_P1);        
        dummy = IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_P1);        
        dummy = IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_P1);        
        UNUSED(dummy);
        
        u1ReadData = u1RegRead1B(TDPROC_29+2);
        
        //if(u1ReadData != TDS_YLEV[bIndex])
	    //{
			Printf("LocalTable index = %d, ", bIndex);
			Printf("LocalTable read data = %d, TDS_YLEV[i] = %d\n", u1ReadData, TDS_LC_H[bIndex]);

			//bResult = SV_FALSE;
    	//}
    }
	vRegWriteFldMulti(TDPROC_2C,
				 P_Fld(0, TDS_YLEV_TBL_LD) |
				 P_Fld(0, TDS_YLEV_LOAD_BURST) |
				 P_Fld(0, TDS_YLEV_LOAD_ENA) |
				 P_Fld(0, TDS_YLEV_READ_ENA));		  //release sram control

    return bResult;
}

/**
 * @brief Calculate manual gain curve with 4 control points
 * @param
 */
void vDrvCalYLevTable(void)
{
    UINT8 i, bIndex, bPoint[4], bGain[4];

    // YLEV_TBL
    for (i = 0; i < 4; i ++)
    {
        bPoint[i] = IO32ReadFldAlign(TDPROC_YLEV_00 + i, TDS_YLEV_P1);
        bGain[i] = IO32ReadFldAlign(TDPROC_YLEV_01 + i, TDS_YLEV_G1);
    }

    for (i = 0; i < 3; i ++)    // Make P1-P4 monotonic increasement
    {
        if (bPoint[i] >= bPoint[i+1])
        {
            bPoint[i] = bPoint[i+1] - 1;
        }
    }

    for (bIndex = 0; bIndex < 65; bIndex ++)
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
 void vDrvCalYLevAdalumaTable(void)
    {
    UINT8 i;
    UINT16 wTmp; 
    UINT16 u2LumaArray[LUMA_CURVE_NUM]; //luma curve
     
    vDrvADLGetLumaCurve(u2LumaArray);
    // Calculate gain for even points
    for (i = 0; i < 32; i ++)
        {
        wTmp = u2LumaArray[i + 1] - u2LumaArray[i];

        // slope = 0x20/wTmp, Gain=((slope*0x80)-0x80)*(Adap_Gain/0x80)+0x80
        TDS_ADAP_YLEV[i * 2] = (wTmp == 0) ? 0xFF : 
            (((0x1000/ wTmp) - 0x80) * 
            IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_ADL_GAIN) >> 7) + 0x80;
        }
    // Calculate gain for index 64
    TDS_ADAP_YLEV[64] = (2 * TDS_ADAP_YLEV[62] - TDS_ADAP_YLEV[60]);

    // Interpolate gain for odd points
    for (i = 1; i < 64; i += 2)
        {
        TDS_ADAP_YLEV[i] = (TDS_ADAP_YLEV[i -1] + TDS_ADAP_YLEV[i + 1]) >> 1;
        }        

    // Multiply adaptive gain curve by manual gain curve
    for (i = 0; i < 65; i ++)
        {
        wTmp = (TDS_ADAP_YLEV[i] * TDS_YLEV[i]) >> 7;
        TDS_YLEV[i] = (wTmp > 0xFF) ? 0xFF : wTmp;
    }
}

/**
 * @brief Calculate manual APL gain curve with 2 control points
 * @param
 */
void vDrvCalYLevAplTable(void)
{
    UINT8 i, bIndex, bPoint[2];
    UINT16 wTmp;    	
    UINT8 bAPL = bDrvGetAPL();       
    // YLEV_APL_TBL
    bPoint[0] = IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_APL_ZERO);
    bPoint[1] = IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_APL_THR);

    if ( bPoint[0]> bPoint[1])	// Make Table monotonic increasement
    {
         bPoint[0] =  bPoint[1];
    }
    
    for (bIndex = 0; bIndex < 65; bIndex ++)	// APLLUT Calculation 
    {
        if (bIndex <= bPoint[0])
        {
            TDS_YLEV_APL[bIndex] = 0x00;
        }
        else if (bIndex >= bPoint[1])
        {
            TDS_YLEV_APL[bIndex] = 0xFF;
        }        
        else
        {
            TDS_YLEV_APL[bIndex] = (UINT16)(0xFF *((UINT16)bIndex - bPoint[0]) / (bPoint[1] - bPoint[0]));
        }   
    }
        
    for (i = 0; i < 65; i ++)	// YLEV Blending by YLEV_APL
    {
        wTmp = (TDS_YLEV[i]*(0x80 - TDS_YLEV_APL[bAPL >> 2] ) + 0x80 * TDS_YLEV_APL[bAPL >> 2] ) >> 7;
        TDS_YLEV[i] = (wTmp > 0xFF) ? 0xFF : wTmp;
    }   
}

/**
 * @brief Enable/Disable adaptive Y level dependent sharpness
 * @param
 */
void vDrvSetAdapYLevOnOff(UINT8 bOnOff)
    {
    fgAdapYLEV = bOnOff;
}

/**
 * @brief Enable/Disable adaptive Y level dependent sharpness
 * @param
 */
void vDrvAdapYLev(void)
{
    if (fgAdapYLEV == SV_ON)
    {
       vDrvCalYLevTable();
	vDrvCalYLevAdalumaTable();   	
 	vDrvCalYLevAplTable();        
       vApiRegisterVideoEvent(PE_EVENT_SHARPNESS, SV_VP_MAIN, SV_ON);
    }  
}

/**
 * @brief Post sharpness initial parameters
 * @param
 */
void vDrvPostSharpInit(void)
{
    vDrvLoadRegDwrdTbl(REGTBL_TDSHARP);

    vRegWriteFldAlign(TDPROC_B0, 0x01, TDS_ADAP_SHP_EN);
    vRegWriteFldAlign(TDPROC_2C, 0x01, TDS_YLEV_ENA);
    vRegWriteFldAlign(TDPROC_24, 0x01, TDS_NEG_GAIN_EN); 
    vDrvLoadLocalTable();
    vDrvLoadYLevTable();   
    vDrvSetAdapYLevOnOff(SV_ON);
    vDrvSharp3DInit();
    vDrvMoSharpInit();
}

/**
 * @brief Set Post sharpness parameters (Main path only)
 * @param
 * @dependency This function should be called by vApiVideoSharpness
 */
void vDrvPostSharpParam(void)
{
//  Gain
	vRegWriteFldAlign(TDPROC_00, wReadQualityTable(QUALITY_TDSHARP_H1_GAIN),
	                  TDS_H1_GAIN);
	vRegWriteFldAlign(TDPROC_03, wReadQualityTable(QUALITY_TDSHARP_H1_GAIN_NEG),
	                  TDS_H1_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_20, wReadQualityTable(QUALITY_TDSHARP_H2_GAIN),
	                  TDS_H2_GAIN);
	vRegWriteFldAlign(TDPROC_23, wReadQualityTable(QUALITY_TDSHARP_H2_GAIN_NEG),
	                  TDS_H2_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_04, wReadQualityTable(QUALITY_TDSHARP_H3_GAIN),
	                  TDS_H3_GAIN);
	vRegWriteFldAlign(TDPROC_07, wReadQualityTable(QUALITY_TDSHARP_H3_GAIN_NEG),
	                  TDS_H3_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_34, wReadQualityTable(QUALITY_TDSHARP_H4_GAIN),
	                  TDS_H4_GAIN);
	vRegWriteFldAlign(TDPROC_37, wReadQualityTable(QUALITY_TDSHARP_H4_GAIN_NEG),
	                  TDS_H4_GAIN_NEG);	
	vRegWriteFldAlign(TDPROC_08, wReadQualityTable(QUALITY_TDSHARP_V1_GAIN),
	                  TDS_V1_GAIN);
	vRegWriteFldAlign(TDPROC_0B, wReadQualityTable(QUALITY_TDSHARP_V1_GAIN_NEG),
	                  TDS_V1_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_0C, wReadQualityTable(QUALITY_TDSHARP_V2_GAIN),
	                  TDS_V2_GAIN);
	vRegWriteFldAlign(TDPROC_0F, wReadQualityTable(QUALITY_TDSHARP_V2_GAIN_NEG),
	                  TDS_V2_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_10, wReadQualityTable(QUALITY_TDSHARP_X1_GAIN),
	                  TDS_X1_GAIN);
	vRegWriteFldAlign(TDPROC_13, wReadQualityTable(QUALITY_TDSHARP_X1_GAIN_NEG),
	                  TDS_X1_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_14, wReadQualityTable(QUALITY_TDSHARP_X2_GAIN),
	                  TDS_X2_GAIN);
	vRegWriteFldAlign(TDPROC_17, wReadQualityTable(QUALITY_TDSHARP_X2_GAIN_NEG),
	                  TDS_X2_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_26, wReadQualityTable(QUALITY_TDSHARP_H3_SEL),
	                  TDS_H3_FLT_SEL);
	vRegWriteFldAlign(TDPROC_26, wReadQualityTable(QUALITY_TDSHARP_V4_SEL),
	                  TDS_V2_FLT_SEL);

//  Coring
	vRegWriteFldAlign(TDPROC_00, wReadQualityTable(QUALITY_TDSHARP_H1_CORING),
	                  TDS_H1_CORING);
	vRegWriteFldAlign(TDPROC_02, wReadQualityTable(QUALITY_TDSHARP_H1_SOFT_COR_GAIN),
	                  TDS_H1_SOFT_COR_GAIN);
	vRegWriteFldAlign(TDPROC_20, wReadQualityTable(QUALITY_TDSHARP_H2_CORING),
	                  TDS_H2_CORING);
	vRegWriteFldAlign(TDPROC_22, wReadQualityTable(QUALITY_TDSHARP_H2_SOFT_COR_GAIN),
	                  TDS_H2_SOFT_COR_GAIN);	
	vRegWriteFldAlign(TDPROC_04, wReadQualityTable(QUALITY_TDSHARP_H3_CORING),
	                  TDS_H3_CORING);
	vRegWriteFldAlign(TDPROC_06, wReadQualityTable(QUALITY_TDSHARP_H3_SOFT_COR_GAIN),
	                  TDS_H3_SOFT_COR_GAIN);	
	vRegWriteFldAlign(TDPROC_34, wReadQualityTable(QUALITY_TDSHARP_H4_CORING),
	                  TDS_H4_CORING);
	vRegWriteFldAlign(TDPROC_36, wReadQualityTable(QUALITY_TDSHARP_H4_SOFT_COR_GAIN),
	                  TDS_H4_SOFT_COR_GAIN);
	vRegWriteFldAlign(TDPROC_08, wReadQualityTable(QUALITY_TDSHARP_V1_CORING),
	                  TDS_V1_CORING);
	vRegWriteFldAlign(TDPROC_0A, wReadQualityTable(QUALITY_TDSHARP_V1_SOFT_COR_GAIN),
	                  TDS_V1_SOFT_COR_GAIN);	
	vRegWriteFldAlign(TDPROC_0C, wReadQualityTable(QUALITY_TDSHARP_V2_CORING),
	                  TDS_V2_CORING);
	vRegWriteFldAlign(TDPROC_0E, wReadQualityTable(QUALITY_TDSHARP_V2_SOFT_COR_GAIN),
	                  TDS_V2_SOFT_COR_GAIN);		
	vRegWriteFldAlign(TDPROC_10, wReadQualityTable(QUALITY_TDSHARP_X1_CORING),
	                  TDS_X1_CORING);
	vRegWriteFldAlign(TDPROC_12, wReadQualityTable(QUALITY_TDSHARP_X1_SOFT_COR_GAIN),
	                  TDS_X1_SOFT_COR_GAIN);		
	vRegWriteFldAlign(TDPROC_14, wReadQualityTable(QUALITY_TDSHARP_X2_CORING),
	                  TDS_X2_CORING);
	vRegWriteFldAlign(TDPROC_16, wReadQualityTable(QUALITY_TDSHARP_X2_SOFT_COR_GAIN),
	                  TDS_X2_SOFT_COR_GAIN);		

    //  Limit
	vRegWriteFldAlign(TDPROC_00,
        wReadQualityTable(QUALITY_TDSHARP_H1_LIMIT_POS), TDS_H1_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_00,
        wReadQualityTable(QUALITY_TDSHARP_H1_LIMIT_NEG), TDS_H1_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_20,
        wReadQualityTable(QUALITY_TDSHARP_H2_LIMIT_POS), TDS_H2_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_20,
        wReadQualityTable(QUALITY_TDSHARP_H2_LIMIT_NEG), TDS_H2_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_04,
        wReadQualityTable(QUALITY_TDSHARP_H3_LIMIT_POS), TDS_H3_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_04,
        wReadQualityTable(QUALITY_TDSHARP_H3_LIMIT_NEG), TDS_H3_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_34,
        wReadQualityTable(QUALITY_TDSHARP_H4_LIMIT_POS), TDS_H4_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_34,
        wReadQualityTable(QUALITY_TDSHARP_H4_LIMIT_NEG), TDS_H4_LIMIT_NEG);	
	vRegWriteFldAlign(TDPROC_08,
        wReadQualityTable(QUALITY_TDSHARP_V1_LIMIT_POS), TDS_V1_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_08,
        wReadQualityTable(QUALITY_TDSHARP_V1_LIMIT_NEG), TDS_V1_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_0C,
        wReadQualityTable(QUALITY_TDSHARP_V2_LIMIT_POS), TDS_V2_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_0C,
        wReadQualityTable(QUALITY_TDSHARP_V2_LIMIT_NEG), TDS_V2_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_10,
        wReadQualityTable(QUALITY_TDSHARP_X1_LIMIT_POS), TDS_X1_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_10,
        wReadQualityTable(QUALITY_TDSHARP_X1_LIMIT_NEG), TDS_X1_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_14,
        wReadQualityTable(QUALITY_TDSHARP_X2_LIMIT_POS), TDS_X2_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_14,
        wReadQualityTable(QUALITY_TDSHARP_X2_LIMIT_NEG), TDS_X2_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_24,
        wReadQualityTable(QUALITY_TDSHARP_LIMIT_POS_ALL)<<2, TDS_LIMIT_POS_ALL);
	vRegWriteFldAlign(TDPROC_24,
        wReadQualityTable(QUALITY_TDSHARP_LIMIT_NEG_ALL)<<2, TDS_LIMIT_NEG_ALL);

// Clip
	vRegWriteFldAlign(TDPROC_01,
        wReadQualityTable(QUALITY_TDSHARP_H1_CLIP_THPOS), TDS_H1_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_01,
        wReadQualityTable(QUALITY_TDSHARP_H1_CLIP_THNEG), TDS_H1_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_02,
        wReadQualityTable(QUALITY_TDSHARP_H1_SOFT_CLIP_GAIN), TDS_H1_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_21,
        wReadQualityTable(QUALITY_TDSHARP_H2_CLIP_THPOS), TDS_H2_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_21,
        wReadQualityTable(QUALITY_TDSHARP_H2_CLIP_THNEG), TDS_H2_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_22,
        wReadQualityTable(QUALITY_TDSHARP_H2_SOFT_CLIP_GAIN), TDS_H2_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_05,
        wReadQualityTable(QUALITY_TDSHARP_H3_CLIP_THPOS), TDS_H3_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_05,
        wReadQualityTable(QUALITY_TDSHARP_H3_CLIP_THNEG), TDS_H3_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_06,
        wReadQualityTable(QUALITY_TDSHARP_H3_SOFT_CLIP_GAIN), TDS_H3_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_35,
        wReadQualityTable(QUALITY_TDSHARP_H4_CLIP_THPOS), TDS_H4_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_35,
        wReadQualityTable(QUALITY_TDSHARP_H4_CLIP_THNEG), TDS_H4_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_36,
        wReadQualityTable(QUALITY_TDSHARP_H4_SOFT_CLIP_GAIN), TDS_H4_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_09,
        wReadQualityTable(QUALITY_TDSHARP_V1_CLIP_THPOS), TDS_V1_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_09,
        wReadQualityTable(QUALITY_TDSHARP_V1_CLIP_THNEG), TDS_V1_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_0A,
        wReadQualityTable(QUALITY_TDSHARP_V1_SOFT_CLIP_GAIN), TDS_V1_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_0D,
        wReadQualityTable(QUALITY_TDSHARP_V2_CLIP_THPOS), TDS_V2_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_0D,
        wReadQualityTable(QUALITY_TDSHARP_V2_CLIP_THNEG), TDS_V2_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_0E,
        wReadQualityTable(QUALITY_TDSHARP_V2_SOFT_CLIP_GAIN), TDS_V2_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_11,
        wReadQualityTable(QUALITY_TDSHARP_X1_CLIP_THPOS), TDS_X1_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_11,
        wReadQualityTable(QUALITY_TDSHARP_X1_CLIP_THNEG), TDS_X1_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_12,
        wReadQualityTable(QUALITY_TDSHARP_X1_SOFT_CLIP_GAIN), TDS_X1_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_15,
        wReadQualityTable(QUALITY_TDSHARP_X2_CLIP_THPOS), TDS_X2_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_15,
        wReadQualityTable(QUALITY_TDSHARP_X2_CLIP_THNEG), TDS_X2_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_16,
        wReadQualityTable(QUALITY_TDSHARP_X2_SOFT_CLIP_GAIN), TDS_X2_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_C3,
        wReadQualityTable(QUALITY_TDSHARP_POS_CLIP)<<2, TDS_POS_CLIP);
	vRegWriteFldAlign(TDPROC_C3,
        wReadQualityTable(QUALITY_TDSHARP_NEG_CLIP)<<2, TDS_NEG_CLIP);
	vRegWriteFldAlign(TDPROC_C3,
        wReadQualityTable(QUALITY_TDSHARP_CLIP_GAIN), TDS_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_26,
        wReadQualityTable(QUALITY_TDSHARP_AC_LPF_EN), TDS_AC_LPF_EN);
	vRegWriteFldAlign(TDPROC_C0,
        wReadQualityTable(QUALITY_TDSHARP_AC_LPF_COE), TDS_AC_LPF_COE);
	vRegWriteFldAlign(TDPROC_01, 
	    wReadQualityTable(QUALITY_TDSHARP_H1_LPF_SEL), TDS_H1_LPF_SEL);
	vRegWriteFldAlign(TDPROC_21, 
	    wReadQualityTable(QUALITY_TDSHARP_H2_LPF_SEL), TDS_H2_LPF_SEL);
	vRegWriteFldAlign(TDPROC_05, 
	    wReadQualityTable(QUALITY_TDSHARP_H3_LPF_SEL), TDS_H3_LPF_SEL);
	vRegWriteFldAlign(TDPROC_35, 
	    wReadQualityTable(QUALITY_TDSHARP_H4_LPF_SEL), TDS_H4_LPF_SEL);
	vRegWriteFldAlign(TDPROC_C0,
        wReadQualityTable(QUALITY_TDSHARP_ADSHP_LC_EN), TDS_LC_EN);
	vRegWriteFldAlign(TDPROC_2C,
        wReadQualityTable(QUALITY_TDSHARP_ADSHP_YLEV_EN), TDS_YLEV_ENA);
	// Ylev Adaptive link to QTYtbl
	// vDrvSetAdapYLevOnOff(wReadQualityTable(QUALITY_TDSHARP_ADSHP_YLEV_EN));
	
    // Adaptive Sharpness
	vRegWriteFldAlign(TDPROC_B0,
        wReadQualityTable(QUALITY_TDSHARP_ADSHP_P1), TDS_ADAP_SHP_P1);
	vRegWriteFldAlign(TDPROC_B0,
        wReadQualityTable(QUALITY_TDSHARP_ADSHP_P2), TDS_ADAP_SHP_P2);
	vRegWriteFldAlign(TDPROC_B0,
        wReadQualityTable(QUALITY_TDSHARP_ADSHP_P3), TDS_ADAP_SHP_P3);
	vRegWriteFldAlign(TDPROC_B2,
        wReadQualityTable(QUALITY_TDSHARP_ADSHP_G1) << 2, TDS_ADAP_SHP_G1);
	vRegWriteFldAlign(TDPROC_B3,
        wReadQualityTable(QUALITY_TDSHARP_ADSHP_G2) << 2, TDS_ADAP_SHP_G2);
	vRegWriteFldAlign(TDPROC_B3,
        wReadQualityTable(QUALITY_TDSHARP_ADSHP_G3) << 2, TDS_ADAP_SHP_G3);
	vRegWriteFldAlign(TDPROC_B2,
        wReadQualityTable(QUALITY_TDSHARP_ADSHP_OFFSET), TDS_ADAP_SHP_OFFSET);

    // Y Levle Table
	vIO32WriteFldAlign(TDPROC_YLEV_00,
        wReadQualityTable(QUALITY_TDSHARP_YLEV_P1), TDS_YLEV_P1);
	vIO32WriteFldAlign(TDPROC_YLEV_00,
        wReadQualityTable(QUALITY_TDSHARP_YLEV_P2), TDS_YLEV_P2);
	vIO32WriteFldAlign(TDPROC_YLEV_00,
        wReadQualityTable(QUALITY_TDSHARP_YLEV_P3), TDS_YLEV_P3);
	vIO32WriteFldAlign(TDPROC_YLEV_00,
        wReadQualityTable(QUALITY_TDSHARP_YLEV_P4), TDS_YLEV_P4);
	vIO32WriteFldAlign(TDPROC_YLEV_01,
        wReadQualityTable(QUALITY_TDSHARP_YLEV_G1), TDS_YLEV_G1);
	vIO32WriteFldAlign(TDPROC_YLEV_01,
        wReadQualityTable(QUALITY_TDSHARP_YLEV_G2), TDS_YLEV_G2);
	vIO32WriteFldAlign(TDPROC_YLEV_01,
        wReadQualityTable(QUALITY_TDSHARP_YLEV_G3), TDS_YLEV_G3);
	vIO32WriteFldAlign(TDPROC_YLEV_01,
        wReadQualityTable(QUALITY_TDSHARP_YLEV_G4), TDS_YLEV_G4);
	vIO32WriteFldAlign(TDPROC_YLEV_02,
        wReadQualityTable(QUALITY_TDSHARP_YLEV_APL_ZERO), TDS_YLEV_APL_ZERO);
	vIO32WriteFldAlign(TDPROC_YLEV_02,
        wReadQualityTable(QUALITY_TDSHARP_YLEV_APL_THR), TDS_YLEV_APL_THR);
	vIO32WriteFldAlign(TDPROC_YLEV_02,
        wReadQualityTable(QUALITY_TDSHARP_YLEV_ADL_GAIN), TDS_YLEV_ADL_GAIN);

    // Mo Sharp
	vIO32WriteFldAlign(MOADAP_SHARP_00, wReadQualityTable(QUALITY_MO_SHARP_EN), MO_SHP_EN);    
	vIO32WriteFldAlign(MOADAP_SHARP_00, wReadQualityTable(QUALITY_MO_SHP_XLUT_GAIN), MO_SHP_XLUT_GAIN);        
	vIO32WriteFldAlign(MOADAP_SHARP_00, wReadQualityTable(QUALITY_MO_SHP_YLUT_GAIN), MO_SHP_YLUT_GAIN);            
	vIO32WriteFldAlign(MOADAP_SHARP_00, wReadQualityTable(QUALITY_MO_SHP_LEVEL), MO_SHP_LEVEL);        
    
    //PBC_1
	vRegWriteFldAlign(PBC_SHP1_01, 
	    wReadQualityTable(QUALITY_PBC_EN_1), PBC_EN_1);
	vRegWriteFldAlign(PBC_SHP1_01, 
	    wReadQualityTable(QUALITY_PBC_BAND_SEL_1), PBC_BAND_SEL_1);
	vRegWriteFldAlign(PBC_SHP1_01, 
	    wReadQualityTable(QUALITY_PBC_GAIN_SIGN_1), PBC_GAIN_SIGN_1);
	vRegWriteFldAlign(PBC_SHP1_05,
        wReadQualityTable(QUALITY_PBC_GAIN_1), PBC_GAIN_1);
	vRegWriteFldAlign(PBC_SHP1_01, 
	    wReadQualityTable(QUALITY_PBC_CORING_1), PBC_CORING_1);
	vRegWriteFldAlign(PBC_SHP1_03,
        (wReadQualityTable(QUALITY_PBC_THETA_C_1)<<2), PBC_THETA_C_1);
	vRegWriteFldAlign(PBC_SHP1_03,
        (wReadQualityTable(QUALITY_PBC_RADIUS_C_1)<<2), PBC_RADIUS_C_1);
	vRegWriteFldAlign(PBC_SHP1_01,
        wReadQualityTable(QUALITY_PBC_THETA_RANGE_1), PBC_THETA_RANGE_1);
	vRegWriteFldAlign(PBC_SHP1_01,
        wReadQualityTable(QUALITY_PBC_RADIUS_RANGE_1), PBC_RADIUS_RANGE_1);
	vRegWriteFldAlign(PBC_SHP1_02,
        (wReadQualityTable(QUALITY_PBC_LOWER_BOUND_1)<<1), PBC_LOWER_BOUND_1);
	vRegWriteFldAlign(PBC_SHP1_02,
        (wReadQualityTable(QUALITY_PBC_UPPER_BOUND_1)<<1), PBC_UPPER_BOUND_1);
    //PBC_2
	vRegWriteFldAlign(PBC_SHP2_01, 
	    wReadQualityTable(QUALITY_PBC_EN_2), PBC_EN_2);
	vRegWriteFldAlign(PBC_SHP2_01, 
	    wReadQualityTable(QUALITY_PBC_BAND_SEL_2), PBC_BAND_SEL_2);
	vRegWriteFldAlign(PBC_SHP2_01, 
	    wReadQualityTable(QUALITY_PBC_GAIN_SIGN_2), PBC_GAIN_SIGN_2);
	vRegWriteFldAlign(PBC_SHP2_05,
        wReadQualityTable(QUALITY_PBC_GAIN_2), PBC_GAIN_2);
	vRegWriteFldAlign(PBC_SHP2_01, 
	    wReadQualityTable(QUALITY_PBC_CORING_2), PBC_CORING_2);
	vRegWriteFldAlign(PBC_SHP2_03,
        (wReadQualityTable(QUALITY_PBC_THETA_C_2)<<2), PBC_THETA_C_2);
	vRegWriteFldAlign(PBC_SHP2_03,
        (wReadQualityTable(QUALITY_PBC_RADIUS_C_2)<<2), PBC_RADIUS_C_2);
	vRegWriteFldAlign(PBC_SHP2_01,
        wReadQualityTable(QUALITY_PBC_THETA_RANGE_2), PBC_THETA_RANGE_2);
	vRegWriteFldAlign(PBC_SHP2_01,
        wReadQualityTable(QUALITY_PBC_RADIUS_RANGE_2), PBC_RADIUS_RANGE_2);
	vRegWriteFldAlign(PBC_SHP2_02,
        (wReadQualityTable(QUALITY_PBC_LOWER_BOUND_2)<<1), PBC_LOWER_BOUND_2);
	vRegWriteFldAlign(PBC_SHP2_02,
        (wReadQualityTable(QUALITY_PBC_UPPER_BOUND_2)<<1), PBC_UPPER_BOUND_2);

    //PBC_3
	vRegWriteFldAlign(PBC_SHP3_01, 
	    wReadQualityTable(QUALITY_PBC_EN_3), PBC_EN_3);
	vRegWriteFldAlign(PBC_SHP3_01, 
	    wReadQualityTable(QUALITY_PBC_BAND_SEL_3), PBC_BAND_SEL_3);
	vRegWriteFldAlign(PBC_SHP3_01, 
	    wReadQualityTable(QUALITY_PBC_GAIN_SIGN_3), PBC_GAIN_SIGN_3);
	vRegWriteFldAlign(PBC_SHP3_05,
        wReadQualityTable(QUALITY_PBC_GAIN_3), PBC_GAIN_3);
	vRegWriteFldAlign(PBC_SHP3_01, 
	    wReadQualityTable(QUALITY_PBC_CORING_3), PBC_CORING_3);
	vRegWriteFldAlign(PBC_SHP3_03,
        (wReadQualityTable(QUALITY_PBC_THETA_C_3)<<2), PBC_THETA_C_3);
	vRegWriteFldAlign(PBC_SHP3_03,
        (wReadQualityTable(QUALITY_PBC_RADIUS_C_3)<<2), PBC_RADIUS_C_3);
	vRegWriteFldAlign(PBC_SHP3_01,
        wReadQualityTable(QUALITY_PBC_THETA_RANGE_3), PBC_THETA_RANGE_3);
	vRegWriteFldAlign(PBC_SHP3_01,
        wReadQualityTable(QUALITY_PBC_RADIUS_RANGE_3), PBC_RADIUS_RANGE_3);
	vRegWriteFldAlign(PBC_SHP3_02,
        (wReadQualityTable(QUALITY_PBC_LOWER_BOUND_3)<<1), PBC_LOWER_BOUND_3);
	vRegWriteFldAlign(PBC_SHP3_02,
        (wReadQualityTable(QUALITY_PBC_UPPER_BOUND_3)<<1), PBC_UPPER_BOUND_3);

    vIO32WriteFldAlign(SHARPNESS_3D_00, 
        (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF) ? 0 : 1, SHARPNESS_3D_EN); 
    vIO32WriteFldAlign(SHARPNESS_3D_01, wReadQualityTable(QUALITY_SHARP_3D_FS_H), SHARPNESS_FS_H); 
    vIO32WriteFldAlign(SHARPNESS_3D_01, wReadQualityTable(QUALITY_SHARP_3D_FS_V), SHARPNESS_FS_V);    
    vIO32WriteFldAlign(SHARPNESS_3D_01, wReadQualityTable(QUALITY_SHARP_3D_TTD_H), SHARPNESS_TTD_H);    
    vIO32WriteFldAlign(SHARPNESS_3D_01, wReadQualityTable(QUALITY_SHARP_3D_TTD_V), SHARPNESS_TTD_V);    
    vIO32WriteFldAlign(SHARPNESS_3D_02, wReadQualityTable(QUALITY_SHARP_3D_SBS_H), SHARPNESS_SBS_H);    
    vIO32WriteFldAlign(SHARPNESS_3D_02, wReadQualityTable(QUALITY_SHARP_3D_SBS_V), SHARPNESS_SBS_V);    
    vIO32WriteFldAlign(SHARPNESS_3D_02, wReadQualityTable(QUALITY_SHARP_3D_TB_H), SHARPNESS_TB_H);    
    vIO32WriteFldAlign(SHARPNESS_3D_02, wReadQualityTable(QUALITY_SHARP_3D_TB_V), SHARPNESS_TB_V);
    vIO32WriteFldAlign(SHARPNESS_3D_03, wReadQualityTable(QUALITY_SHARP_3D_SS_H), SHARPNESS_SS_H);    
    vIO32WriteFldAlign(SHARPNESS_3D_03, wReadQualityTable(QUALITY_SHARP_3D_SS_V), SHARPNESS_SS_V);    
    vIO32WriteFldAlign(SHARPNESS_3D_03, wReadQualityTable(QUALITY_SHARP_3D_RD_H), SHARPNESS_RD_H);    
    vIO32WriteFldAlign(SHARPNESS_3D_03, wReadQualityTable(QUALITY_SHARP_3D_RD_V), SHARPNESS_RD_V);  
}

void vDrvPostSharpParamHorizontal(void)
{
//  Gain
	vRegWriteFldAlign(TDPROC_00, wReadQualityTable(QUALITY_TDSHARP_H1_GAIN),
	                  TDS_H1_GAIN);
	vRegWriteFldAlign(TDPROC_03, wReadQualityTable(QUALITY_TDSHARP_H1_GAIN_NEG),
	                  TDS_H1_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_20, wReadQualityTable(QUALITY_TDSHARP_H2_GAIN),
	                  TDS_H2_GAIN);
	vRegWriteFldAlign(TDPROC_23, wReadQualityTable(QUALITY_TDSHARP_H2_GAIN_NEG),
	                  TDS_H2_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_04, wReadQualityTable(QUALITY_TDSHARP_H3_GAIN),
	                  TDS_H3_GAIN);
	vRegWriteFldAlign(TDPROC_07, wReadQualityTable(QUALITY_TDSHARP_H3_GAIN_NEG),
	                  TDS_H3_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_34, wReadQualityTable(QUALITY_TDSHARP_H4_GAIN),
	                  TDS_H4_GAIN);
	vRegWriteFldAlign(TDPROC_37, wReadQualityTable(QUALITY_TDSHARP_H4_GAIN_NEG),
	                  TDS_H4_GAIN_NEG);	   
}

void vDrvPostSharpParamVertical(void)
{
//  Gain
	vRegWriteFldAlign(TDPROC_08, wReadQualityTable(QUALITY_TDSHARP_V1_GAIN),
	                  TDS_V1_GAIN);
	vRegWriteFldAlign(TDPROC_0B, wReadQualityTable(QUALITY_TDSHARP_V1_GAIN_NEG),
	                  TDS_V1_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_0C, wReadQualityTable(QUALITY_TDSHARP_V2_GAIN),
	                  TDS_V2_GAIN);
	vRegWriteFldAlign(TDPROC_0F, wReadQualityTable(QUALITY_TDSHARP_V2_GAIN_NEG),
	                  TDS_V2_GAIN_NEG);
}

/*****************************************************************************************/
/********************************  Adaptive H SHARP **************************************/
/*****************************************************************************************/


void vDrvAdapHShpInit(void)
{
    UINT8 index;
    
    vIO32WriteFldAlign(ADAPTIVE_SHARP_GAIN_00, bAdapShpEn, ADAPTIVE_SHARP_GAIN_EN);    
    vIO32WriteFldAlign(ADAPTIVE_SHARP_GAIN_00, bAdapShpPoints, ADAPTIVE_SHARP_GAIN_P_NUMBER);     
    
    bAdapShpPoints = (bAdapShpPoints > ADAP_SHP_MAX_POINT) ? ADAP_SHP_MAX_POINT : bAdapShpPoints;
        
    for (index=0; index<bAdapShpPoints; index++)
    {
        vIO32WriteFldAlign(ADAPTIVE_SHARP_GAIN_01+(4*index), 
            bAdapShpTable[index][0], ADAPTIVE_SHARP_GAIN_P1);
        vIO32WriteFldAlign(ADAPTIVE_SHARP_GAIN_01+(4*index), 
            bAdapShpTable[index][1], ADAPTIVE_SHARP_GAIN_W1);
    }
}

void vDrvUpdateAdapHSharpWeighting(void)
{
    UINT8 index = 0;
    UINT8 lPoint=0, rPoint=0;
    UINT8 lWeight=0, rWeight=0;
    UINT8 maxWeight=0, minWeight=0;
    UINT8 bAPL = bDrvGetAPL();

    //update current APL to sw reg
    vIO32WriteFldAlign(ADAPTIVE_SHARP_GAIN_00, bAPL, APAPTIVE_SHARP_GAIN_CURRENT_APL);
    
    //update table first
    bAdapShpPoints = IO32ReadFldAlign(ADAPTIVE_SHARP_GAIN_00, ADAPTIVE_SHARP_GAIN_P_NUMBER);
    if (bAdapShpPoints > ADAP_SHP_MAX_POINT)
    {
        bAdapShpPoints = ADAP_SHP_MAX_POINT;
    }
    
    for (index=0;index<bAdapShpPoints;index++)
    {
        bAdapShpTable[index][0] = IO32ReadFldAlign(ADAPTIVE_SHARP_GAIN_01+(4*index), ADAPTIVE_SHARP_GAIN_P1); 
        bAdapShpTable[index][1] = IO32ReadFldAlign(ADAPTIVE_SHARP_GAIN_01+(4*index), ADAPTIVE_SHARP_GAIN_W1); 
    }

    //search table
    for(index = 0; index<bAdapShpPoints; index++)
    {
        if(index==0)
        {
            if(bAPL < bAdapShpTable[index][0])
            {
                lPoint = 0;
                rPoint = bAdapShpTable[index][0];
                lWeight = 0;
                rWeight = bAdapShpTable[index][1];                
            }
        }
        else
        {
            if(bAPL >= bAdapShpTable[index-1][0] && bAPL < bAdapShpTable[index][0])
            {
                lPoint = bAdapShpTable[index-1][0];
                rPoint = bAdapShpTable[index][0];
                lWeight = bAdapShpTable[index-1][1];
                rWeight = bAdapShpTable[index][1];                
            }
        }
    }

    minWeight = MIN(lWeight, rWeight);
    maxWeight = MAX(lWeight, rWeight);

    if (rPoint !=0)  //for divide to 0 protection
    {
        bAdapShpWeighting = minWeight + (((bAPL - lPoint) * (maxWeight - minWeight))/ (rPoint - lPoint));
    }        
    
    if (bAdapShpPoints > 0)
    {
        if (bAPL > bAdapShpTable[bAdapShpPoints-1][0])
        {
            bAdapShpWeighting = 255;
        }
    }

    if (bAdapShpEn == SV_OFF)
    {
        bAdapShpWeighting = 255;
    }
    
    vIO32WriteFldAlign(ADAPTIVE_SHARP_GAIN_00, bAdapShpWeighting, APAPTIVE_SHARP_GAIN_FINAL_WEIGHTING);
    LOG(6, "[Adap Shp] (L,R,Min,Max,APL)=(%d,%d,%d,%d,%d) Adap Sharp Weighting(%d)\n",
        lPoint, rPoint, minWeight, maxWeight, bAPL, bAdapShpWeighting);
}

UINT16 wDrvGetAdpHShpGain(UINT16 wInputGain)
{
    UINT16 wRet = wInputGain;

    if(wInputGain >= 0x40)
    {
        wRet = 0x40 + ((wInputGain - 0x40) * bAdapShpWeighting)/ 0xFF;
    }
    else
    {
        wRet = 0x40 - ((0x40 - wInputGain) * bAdapShpWeighting)/ 0xFF;
    }

    return wRet;
}

void vDrvSetHSahrpAdapGain(void)
{
    vRegWriteFldAlign(HSHARP_MAIN_00,
        wDrvGetAdpHShpGain(wReadQualityTable(QUALITY_HSHARP_HIGH)), HSP_SHARP_HIGH);
    vRegWriteFldAlign(HSHARP_MAIN_00,
        wDrvGetAdpHShpGain(wReadQualityTable(QUALITY_HSHARP_MID)), HSP_SHARP_MID);
    vRegWriteFldAlign(HSHARP_MAIN_00,
        wDrvGetAdpHShpGain(wReadQualityTable(QUALITY_HSHARP_LOW)), HSP_SHARP_LOW);
    vRegWriteFldAlign(HSHARP_MAIN_01,
        wDrvGetAdpHShpGain(wReadQualityTable(QUALITY_HSHARP_HIGH_NEG)), HSP_SHARP_HIGH_NEG);
    vRegWriteFldAlign(HSHARP_MAIN_01,
        wDrvGetAdpHShpGain(wReadQualityTable(QUALITY_HSHARP_MID_NEG)), HSP_SHARP_MID_NEG);
    vRegWriteFldAlign(HSHARP_MAIN_01,
        wDrvGetAdpHShpGain(wReadQualityTable(QUALITY_HSHARP_LOW_NEG)), HSP_SHARP_LOW_NEG);

    vRegWriteFldAlign(HSHARP_MAIN_07,
        wDrvGetAdpHShpGain(wReadQualityTable(QUALITY_HSHARP_LC_HIGH)), HSP_LC_SHARP_HIGH);
    vRegWriteFldAlign(HSHARP_MAIN_07,
        wDrvGetAdpHShpGain(wReadQualityTable(QUALITY_HSHARP_LC_MID)), HSP_LC_SHARP_MID);
    vRegWriteFldAlign(HSHARP_MAIN_07,
        wDrvGetAdpHShpGain(wReadQualityTable(QUALITY_HSHARP_LC_LOW)), HSP_LC_SHARP_LOW);
    vRegWriteFldAlign(HSHARP_MAIN_08,
        wDrvGetAdpHShpGain(wReadQualityTable(QUALITY_HSHARP_LC_HIGH_NEG)), HSP_LC_SHARP_HIGH_NEG);
    vRegWriteFldAlign(HSHARP_MAIN_08,
        wDrvGetAdpHShpGain(wReadQualityTable(QUALITY_HSHARP_LC_MID_NEG)), HSP_LC_SHARP_MID_NEG);
    vRegWriteFldAlign(HSHARP_MAIN_08,
        wDrvGetAdpHShpGain(wReadQualityTable(QUALITY_HSHARP_LC_LOW_NEG)), HSP_LC_SHARP_LOW_NEG);	
}

void vDrvHSharpProc(void)
{
    if (IO32ReadFldAlign(ADAPTIVE_SHARP_GAIN_00, ADAPTIVE_SHARP_GAIN_EN))
    {    
        if (bAdapShpEn == SV_OFF)
        {
            bAdapShpEn = SV_ON;
            LOG(3, "Enable Adaptive HSharp Gain\n");
        }

        vDrvUpdateAdapHSharpWeighting();
        vDrvSetHSahrpAdapGain();   
    }
    else
    {
        if (bAdapShpEn == SV_ON)
        {
            bAdapShpEn = SV_OFF;
            vDrvUpdateAdapHSharpWeighting();
            vDrvSetHSahrpAdapGain();
            LOG(3, "Disable Adaptive HSharp Gain\n");
        }
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
#ifdef CC_MT5395		// MT5395 refer to mjc when mjc_Gmvx!=0
	MVX = u1DrvMJCGetGmvX();
	if  (MVX!= 0)
	{		
		XLEVEL = MVX;	
	       vIO32WriteFldAlign(MOADAP_SHARP_00, XLEVEL, MO_SHP_XLEVEL);   
              //LOG(1, "MJC_GmvX = %d\n", MVX);
	}
	else	
#endif
	if (RegReadFldAlign(MDDI_CS_28,GMV_VALID))
	{
		MVX = RegReadFldAlign(MDDI_CS_28,GMV_MVX);
		XLEVEL = (MVX>10)?(0x20-MVX):MVX;	
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
#ifdef CC_MT5395		// MT5395 refer to mjc when mjc_Gmvx!=0

	MVY = u1DrvMJCGetGmvY();
	if  (MVY!= 0)
	{		
		YLEVEL = MVY;	
	       vIO32WriteFldAlign(MOADAP_SHARP_00, YLEVEL, MO_SHP_YLEVEL);   
              //LOG(1, "MJC_GmvY = %d\n", MVY);
	}
	else	
#endif
	if (RegReadFldAlign(MDDI_CS_28,GMV_VALID))
	{
		MVY = RegReadFldAlign(MDDI_CS_28,GMV_MVY);
		YLEVEL = (MVY>4)?(0x10-MVY):MVY;	
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


UINT16 wDrvGetMoSharpValue(UINT16 wInput, UINT16 wOutput, UINT8 LEVEL)
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

    UINT8 UI_LEVEL, XLEVEL,YLEVEL,DLEVEL;

// Set Total Level
    UI_LEVEL =IO32ReadFldAlign(MOADAP_SHARP_00, MO_SHP_LEVEL);  
    // Get SW UI Level	
    if (UI_LEVEL>8)			// Protect UI_LEVEL <=8
    {
       vIO32WriteFldAlign(MOADAP_SHARP_00, 0x08, MO_SHP_LEVEL);   
    	UI_LEVEL=8;    	
    } 
// Get Mo Level                
    if (IO32ReadFldAlign(MOADAP_SHARP_00, MO_SHP_EN))
    {      
	XLEVEL = (vDrvGetMoSharpXLevel()*UI_LEVEL)>>3;    
	YLEVEL = (vDrvGetMoSharpYLevel()*UI_LEVEL)>>3;
    }
    else
    {
    	XLEVEL = 0;
    	YLEVEL = 0;
    }
    DLEVEL = (XLEVEL<YLEVEL)?XLEVEL:YLEVEL;  
	
    // Edge Clear : TDSHARP ACLPF
	vRegWriteFldAlign(TDPROC_C0, 
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_TDSHARP_AC_LPF_COE), 0x00, XLEVEL), TDS_AC_LPF_COE);  	
    // Detail Reveal : HSAHRP Coring
	vRegWriteFldAlign(HSHARP_MAIN_02, 
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_HSHARP_HIGH_CORING), 0x00, XLEVEL), HSP_SHARP_HIGH_CORING);      
	vRegWriteFldAlign(HSHARP_MAIN_02, 
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_HSHARP_HIGH_CORING), 0x00, XLEVEL), HSP_SHARP_MID_CORING);      
	vRegWriteFldAlign(HSHARP_MAIN_02, 
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_HSHARP_HIGH_CORING), 0x00, XLEVEL), HSP_SHARP_LOW_CORING);      
	vRegWriteFldAlign(HSHARP_MAIN_09, 
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_HSHARP_HIGH_CORING), 0x00, XLEVEL), HSP_LC_SHARP_HIGH_CORING);      
	vRegWriteFldAlign(HSHARP_MAIN_09, 
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_HSHARP_HIGH_CORING), 0x00, XLEVEL), HSP_LC_SHARP_MID_CORING);      
	vRegWriteFldAlign(HSHARP_MAIN_09, 
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_HSHARP_HIGH_CORING), 0x00, XLEVEL), HSP_LC_SHARP_LOW_CORING);     
    // Clarity Keeping : TDSAHRP Clip Soft Gain
	vRegWriteFldAlign(TDPROC_02,                                                                                                    
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_TDSHARP_H1_SOFT_CLIP_GAIN), 0x80, XLEVEL), TDS_H1_SOFT_CLIP_GAIN);
    	vRegWriteFldAlign(TDPROC_22,                                                                                                    
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_TDSHARP_H2_SOFT_CLIP_GAIN), 0x80, XLEVEL), TDS_H2_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_06,                                                                                                    
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_TDSHARP_H3_SOFT_CLIP_GAIN), 0x80, XLEVEL), TDS_H3_SOFT_CLIP_GAIN);
 	vRegWriteFldAlign(TDPROC_36,                                                                                                    
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_TDSHARP_H4_SOFT_CLIP_GAIN), 0x80, XLEVEL), TDS_H4_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_0A,                                                                                                    
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_TDSHARP_V1_SOFT_CLIP_GAIN), 0x80, YLEVEL), TDS_V1_SOFT_CLIP_GAIN);
    	vRegWriteFldAlign(TDPROC_0E,                                                                                                    
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_TDSHARP_V2_SOFT_CLIP_GAIN), 0x80, YLEVEL), TDS_V2_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_12,                                                                                                    
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_TDSHARP_X1_SOFT_CLIP_GAIN), 0x80, DLEVEL), TDS_X1_SOFT_CLIP_GAIN);
 	vRegWriteFldAlign(TDPROC_16,                                                                                                    
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_TDSHARP_X2_SOFT_CLIP_GAIN), 0x80, DLEVEL), TDS_X2_SOFT_CLIP_GAIN);
 	vRegWriteFldAlign(TDPROC_C3,                                                                                                    
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_TDSHARP_CLIP_GAIN), 0x80, DLEVEL), TDS_CLIP_GAIN);
 	vRegWriteFldAlign(LTI_02,                                                                                                    
	    wDrvGetMoSharpValue(wReadQualityTable(QUALITY_LTI_SOFT_CLIP_GAIN1), 0x80, DLEVEL), LTI_SOFT_CLIP_GAIN1);
 	
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
    //vApiRegisterVideoEvent(PE_EVENT_SHARPNESS, SV_VP_MAIN, SV_ON);    

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
	vRegWriteFldAlign(TDPROC_00, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_H1_GAIN), w3DWeightingH), TDS_H1_GAIN);
	vRegWriteFldAlign(TDPROC_03, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_H1_GAIN_NEG), w3DWeightingH), TDS_H1_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_20, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_H2_GAIN), w3DWeightingH), TDS_H2_GAIN);
	vRegWriteFldAlign(TDPROC_23, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_H2_GAIN_NEG), w3DWeightingH), TDS_H2_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_04, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_H3_GAIN), w3DWeightingH), TDS_H3_GAIN);
	vRegWriteFldAlign(TDPROC_07, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_H3_GAIN_NEG), w3DWeightingH), TDS_H3_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_34, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_H4_GAIN), w3DWeightingH), TDS_H4_GAIN);
	vRegWriteFldAlign(TDPROC_37, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_H4_GAIN_NEG), w3DWeightingH), TDS_H4_GAIN_NEG);	
	vRegWriteFldAlign(TDPROC_08, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_V1_GAIN), w3DWeightingV), TDS_V1_GAIN);
	vRegWriteFldAlign(TDPROC_0B, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_V1_GAIN_NEG), w3DWeightingV), TDS_V1_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_0C, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_V2_GAIN), w3DWeightingV), TDS_V2_GAIN);
	vRegWriteFldAlign(TDPROC_0F, 
	    wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_V2_GAIN_NEG), w3DWeightingV), TDS_V2_GAIN_NEG);
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
            case E_TDTV_UI_3D_MODE_FS:
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_01, SHARPNESS_FS_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_01, SHARPNESS_FS_V); 
                break;
            case E_TDTV_UI_3D_MODE_RD:
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_03, SHARPNESS_RD_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_03, SHARPNESS_RD_V); 
                break;
            case E_TDTV_UI_3D_MODE_SBS:
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_02, SHARPNESS_SBS_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_02, SHARPNESS_SBS_V); 
                break;
            case E_TDTV_UI_3D_MODE_TTD:
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_01, SHARPNESS_TTD_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_01, SHARPNESS_TTD_V); 
                break;
            case E_TDTV_UI_3D_MODE_TB:  
            case E_TDTV_UI_3D_MODE_LI:              
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_02, SHARPNESS_TB_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_02, SHARPNESS_TB_V); 
                break;
            case E_TDTV_UI_3D_MODE_SS:   
                w3DWeightingH = IO32ReadFldAlign(SHARPNESS_3D_03, SHARPNESS_SS_H); 
                w3DWeightingV = IO32ReadFldAlign(SHARPNESS_3D_03, SHARPNESS_SS_V); 
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

/*****************************************************************************************/
/************************************  PBC Proc ******************************************/
/*****************************************************************************************/
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
    wRadius = RegReadFldAlign(PBC_SHP1_03, PBC_RADIUS_C_1);
    wRange = RegReadFldAlign(PBC_SHP1_01, PBC_RADIUS_RANGE_1);
    wThRange = RegReadFldAlign(PBC_SHP1_01, PBC_THETA_RANGE_1);

    if ((wRadius1 != wRadius) || (wRange1 != wRange) || (wThRange1 != wThRange))
    {
        if ((wRadius !=0 ) && (wRange != 0) && (wThRange != 0))
        {
            vRegWriteFldAlign(PBC_SHP1_04, 4096 / wRange, PBC_ALPHA_1);
            vRegWriteFldAlign(PBC_SHP1_04, 4096 / wThRange, PBC_BETA_1);
            vRegWriteFldAlign(PBC_SHP1_05, 4096 / ((wRange > wRadius) ? wRadius : wRange), PBC_ALPHA_1_1);
        }
        wRadius1 = wRadius;
        wRange1 = wRange;
        wThRange1 = wThRange;
    }

    // update PBC 2 register
    wRadius = RegReadFldAlign(PBC_SHP2_03, PBC_RADIUS_C_2);
    wRange = RegReadFldAlign(PBC_SHP2_01, PBC_RADIUS_RANGE_2);
    wThRange = RegReadFldAlign(PBC_SHP2_01, PBC_THETA_RANGE_2);

    if ((wRadius2 != wRadius) || (wRange2 != wRange) || (wThRange2 != wThRange))
    {
        if ((wRadius !=0 ) && (wRange != 0) && (wThRange != 0))
        {
            vRegWriteFldAlign(PBC_SHP2_04, 4096 / wRange, PBC_ALPHA_2);
            vRegWriteFldAlign(PBC_SHP2_04, 4096 / wThRange, PBC_BETA_2);
            vRegWriteFldAlign(PBC_SHP2_05, 4096 / ((wRange > wRadius) ? wRadius : wRange), PBC_ALPHA_1_2);
        }
        wRadius2 = wRadius;
        wRange2 = wRange;
        wThRange2 = wThRange;
    }

    // update PBC 3 register
    wRadius = RegReadFldAlign(PBC_SHP3_03, PBC_RADIUS_C_3);
    wRange = RegReadFldAlign(PBC_SHP3_01, PBC_RADIUS_RANGE_3);
    wThRange = RegReadFldAlign(PBC_SHP3_01, PBC_THETA_RANGE_3);

    if ((wRadius3 != wRadius) || (wRange3 != wRange) || (wThRange3 != wThRange))
    {
        if ((wRadius !=0 ) && (wRange != 0) && (wThRange != 0))
        {
            vRegWriteFldAlign(PBC_SHP3_04, 4096 / wRange, PBC_ALPHA_3);
            vRegWriteFldAlign(PBC_SHP3_04, 4096 / wThRange, PBC_BETA_3);
            vRegWriteFldAlign(PBC_SHP3_05, 4096 / ((wRange > wRadius) ? wRadius : wRange), PBC_ALPHA_1_3);
        }
        wRadius3 = wRadius;
        wRange3 = wRange;
        wThRange3 = wThRange;
    }
}

void vDrvSharpProc(void)
{
    UINT32 dwWidth;

    dwWidth = u4ScpipGetPreScaleWidth(SV_VP_MAIN);

    if (dwWidth != RegReadFldAlign(HSHARP_MAIN_0F, HSP_BOUNDARY_H_POS_IN_CTI))
    {
        vRegWriteFldAlign(HSHARP_MAIN_0F, dwWidth, HSP_BOUNDARY_H_POS_IN_CTI);
    }

    vDrvAdapYLev();
    vDrvHSharpProc();
    vDrvSharp3DProc();
    vDrvMoSharpProc();    
    vDrvPBCProc();
}


/*****************************************************************************************/
/************************************   CHROMA CORING*************************************/
/*****************************************************************************************/
void vDrvChromaCoringInit(void)
{
    UINT8 i, bPath;
    UINT16 wOffset;

    for (bPath = SV_VP_MAIN; bPath < SV_VP_NA; bPath ++)
    {
        wOffset = (bPath == SV_VP_MAIN)? 0x0 : CHROMACORING_OFFSET;
        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0x64, MAIN_Y_BOUND);

        for (i = 0; i < 16; i++)
        {
            vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0, LOAD_CB);
            vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0, LOAD_CR);
            vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, i, CBCR_TABLE_ADDR);
            vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, CCORING_TBL[i], CBCR_TABLE_DATA);
            vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 1, LOAD_CB);
            vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 1, LOAD_CR);
        }
        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0, LOAD_CB);
        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0, LOAD_CR);
    }
}

void vDrvChromaCoringOnOff(UINT8 bPath, UINT8 bOnOff)
{
	UINT16 wOffset = (bPath == SV_VP_MAIN) ? 0x0 : CHROMACORING_OFFSET;

	vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, bOnOff, MAIN_CHROMA_TABLE_EN);
}

void vDrvChromaCoringYbound(UINT8 bPath, UINT8 bYbound)
{
	UINT16 wOffset = (bPath == SV_VP_MAIN) ? 0x0 : CHROMACORING_OFFSET;

    vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, bYbound, MAIN_Y_BOUND);
}

void vDrvSetChromaCoringTbl(UINT32 u4Path, const UINT8 * CbCoringTbl, const UINT8 * CrCoringTbl)
{
    UINT32 i;
    UINT16 wOffset;

    wOffset = (u4Path == SV_VP_MAIN)? 0x0 : CHROMACORING_OFFSET;

    for (i = 0; i < 16; i++)
    {
        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, i, CBCR_TABLE_ADDR);

        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0, LOAD_CB);
        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, CbCoringTbl[i], CBCR_TABLE_DATA);
        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 1, LOAD_CB);

        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0, LOAD_CR);
        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, CrCoringTbl[i], CBCR_TABLE_DATA);
        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 1, LOAD_CR);
    }
    vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0, LOAD_CB);
    vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0, LOAD_CR);

}

/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/

void vDrvHSharpDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter)
{
    if (bOnOff == SV_ON)
    {
        vRegWriteFldAlign(HSHARP_MAIN_0E, 0, HSP_DEMO_LBOUND);
        vRegWriteFldAlign(HSHARP_MAIN_0E, wXCenter, HSP_DEMO_RBOUND);
        vRegWriteFldAlign(HSHARP_MAIN_0E, 0x0, HSP_DEMO_TBOUND);
        vRegWriteFldAlign(HSHARP_MAIN_0E, 0xFF, HSP_DEMO_BBOUND);    
        vRegWriteFldAlign(HSHARP_MAIN_0D, (bRegion == SV_LEFT)?1:0, HSP_HBOUND_INV);
    }

    vRegWriteFldAlign(HSHARP_MAIN_0D, bOnOff, HSP_DEMO_EN); 
}

void vDrv2DSharpDemo(UINT8 bOnOff, Region rRegion)
{
    if(bOnOff==SV_ON)
    {
        //Set region or off
        vRegWriteFldAlign(SP_MASK_03, ((rRegion.u1OnOff==SV_ON)?0:1), DMWINDOW_EXCHANGE);
        //Define demo region
        vRegWriteFldAlign(SP_MASK_02, rRegion.wHStart, H_DMWINDOW_START);
        vRegWriteFldAlign(SP_MASK_02, rRegion.wHEnd+1, H_DMWINDOW_END);// 2DSharp : start < active < End
        vRegWriteFldAlign(SP_MASK_03, rRegion.wVStart+3, V_DMWINDOW_START); // 2D Sharp Demo mode Voffset
        vRegWriteFldAlign(SP_MASK_03, rRegion.wVEnd+2, V_DMWINDOW_END);// 2D Sharp Demo mode Voffset
    }
    //Turn on/off demo mode
    vRegWriteFldAlign(SP_MASK_03, (bOnOff==SV_ON?1:0), DMWINDOW_EN);
}

void vDrvECTIDemo(UINT8 bOnOff, Region rRegion)
{
    if (bOnOff == SV_ON) 
    {
        //Set region or off
        vRegWriteFldAlign(ECTI_0A, ((rRegion.u1OnOff== SV_ON)?0:1), ECTI_DEMO_SX);
        //Define demo region
        vRegWriteFldAlign(ECTI_09, rRegion.wHStart, ECTI_HDEMO_START);
        vRegWriteFldAlign(ECTI_09, rRegion.wHEnd, ECTI_HDEMO_END);
        vRegWriteFldAlign(ECTI_0A, rRegion.wVStart, ECTI_VDEMO_START);
        vRegWriteFldAlign(ECTI_0A, rRegion.wVEnd, ECTI_VDEMO_END);
    }
    //Turn on/off demo mode
    vRegWriteFldAlign(ECTI_0A, (bOnOff==SV_ON?1:0), ECTI_DEMO_ENA);
}

