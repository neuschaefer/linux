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
#include "video_def.h"
#include "drv_tdtv_drvif.h"
#include "panel.h"
#ifdef CC_SUPPORT_TVE
#include "tve_if.h"
#endif

// === HW INCLUDE =============================================================================
#include "hw_ycproc.h"
#include "hw_ospe.h"
#include "hw_sw.h"

// === DEFINE =============================================================================
#define MATRIX3x3_OFFSET	    0x40

// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================
extern UINT8 bGetSignalType(UINT8 bPath) ;

// === GLOBAL VARIABLE ====================================================================
UINT16 u2PQItemMatrix[SV_VP_NA][PQ_MAX];
UINT8 _bMAIN_COLOR_SYS;
UINT8 _bPIP_COLOR_SYS;
INT32 COLOR_TRANSFORM_ADJ[15];
UINT32 _u4MatrixPedEn;
UINT16 pPedestalMatrix[15];

UINT16 const COLOR_TRANSFORM[SV_COLORTRANS_MAX][15] =
{
    {   // 0    RGB(Full) -> RGB (Full)
        0x0000, 0x0000, 0x0000,
        0x0200, 0x0000, 0x0000,
        0x0000, 0x0200, 0x0000,
        0x0000, 0x0000, 0x0200,
        0x0000, 0x0000, 0x0000
    }
    ,
    {   // 1    RGB(Limit) -> RGB (Full)
        0x07C0, 0x07C0, 0x07C0,
        0x0254, 0x0000, 0x0000,
        0x0000, 0x0254, 0x0000,
        0x0000, 0x0000, 0x0254,
        0x0000, 0x0000, 0x0000
    }
    ,
    {   // 2    RGB(Full) -> YCbCr601(Limit)
        0x0000, 0x0000, 0x0000,
        0x0102, 0x0032, 0x0084,
        0xFF6B, 0x00E1, 0xFFB4,
        0xFF44, 0xFFDC, 0x00E0,
        0x0040, 0x1800, 0x1800
    }
    ,
    {   // 3    RGB(Limit) -> YCbCr601(Limit)
        0x0000, 0x0000, 0x0000,
        0x012D, 0x003A, 0x0099,
        0xFF52, 0x0106, 0xFFA8,
        0xFF25, 0xFFD6, 0x0105,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 4    YCbCr601(Limit) -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x0200, 0x0000, 0x0000,
        0x0000, 0x0200, 0x0000,
        0x0000, 0x0000, 0x0200,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 5    YCbCr601(Full) -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x01B8, 0x0000, 0x0000,
        0x0000, 0x01C2, 0x0000,
        0x0000, 0x0000, 0x01C2,
        0x0040, 0x1800, 0x1800
    }
    ,
    {   // 6    YCbCr601(Limit) -> RGB(Full)
        0x07C0, 0x0600, 0x0600,
        0x0254, 0xFF38, 0xFE60,
        0x0254, 0x0409, 0x0000,
        0x0254, 0x0000, 0x0331,
        0x0000, 0x0000, 0x0000
    }
    ,
    {   // 7    YCbCr601(Full) -> RGB(Full)
        0x0000, 0x0600, 0x0600,
        0x0200, 0xFF54, 0xFE9B,
        0x0200, 0x0377, 0x0000,
        0x0200, 0x0000, 0x02BE,
        0x0000, 0x0000, 0x0000
    }
    ,
    {   // 8    YCbCr709(Limit) -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x0200, 0x0033, 0x0062,
        0x0000, 0x01FB, 0x0FC8,
        0x0000, 0x0FDB, 0x01F8,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 9    YCbCr709(Full) -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x01B8, 0x002C, 0x0054,
        0x0000, 0x01BE, 0xFFCF,
        0x0000, 0xFFDF, 0x01BB,
        0x0040, 0x1800, 0x1800
    }
    ,
    {   // 10   YCbCr709(Limit) -> RGB(Full)
        0x07C0, 0x0600, 0x0600,
        0x0254, 0xFF93, 0xFEEF,
        0x0254, 0x0396, 0x0000,
        0x0254, 0x0000, 0x043B,
        0x0000, 0x0000, 0x0000
    }
    ,
    {   // 11   YCbCr709(Full) -> RGB(Full)
        0x0000, 0x0600, 0x0600,
        0x0200, 0xFFA2, 0xFF15,
        0x0200, 0x03A2, 0x0000,
        0x0200, 0x0000, 0x0314,
        0x0000, 0x0000, 0x0000
    }
    ,
    {   // 12   TVD NTSC YUV -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x0362, 0x0000, 0x0000,
        0x0000, 0x03F8, 0x0000,
        0x0000, 0x0000, 0x02D0,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 13   TVD NTSC_J YUV -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x0321, 0x0000, 0x0000,
        0x0000, 0x03AC, 0x0000,
        0x0000, 0x0000, 0x029A,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 14   TVD PAL YUV -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x0333, 0x0000, 0x0000,
        0x0000, 0x03C1, 0x0000,
        0x0000, 0x0000, 0x02A9,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 15   TVD PAL YUV -> YCbCr601(Limit) //not support ultra black
        0x0000, 0x0600, 0x0600,
        0x0333, 0x0000, 0x0000,
        0x0000, 0x03C1, 0x0000,
        0x0000, 0x0000, 0x02A9,
        0x0040, 0x1800, 0x1800
    }
    ,
    {   // 16   SCART RGB, BGR->YCbCr(Limit)  (under fast blanking mode, TVD can only mix RGB signal as BGR order)
        0x0000, 0x0000, 0x0000,
        0x003A, 0x012D, 0x0099,
        0x0106, 0xFF52, 0xFFA8,
        0xFFD6, 0xFF25, 0x0106,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 17   SCART RGB, GBR -> YCbCr601(Limit)
        0x0000, 0x0000, 0x0000,
        0x012D, 0x003A, 0x0099,
        0xFF52, 0x0106, 0xFFA8,
        0xFF25, 0xFFD6, 0x0106,
        0x0000, 0x1800, 0x1800
    }
};

INT32 const YCBCR2RGBOS[3][15] =
{
    {
    	0, 0, 0,
    	1196, 0, 1640,      // 1.168, 0, 1.601
    	1196, -403, -835,   // 1.168, -0.394, -0.813
    	1196, 2072, 0,      // 1.168, 2.023, 0
    	-75, -75, -75
    }
    ,
    {
    	0, 0, 0,
    	1126, 0, 1549,      // 1.1, 0, 1.513
    	1126, -378, -785,   // 1.1, -0.37, -0.77
    	1126, 1959, 0,      // 1.1, 1.913, 0
    	-70, -70, -70
    }
    ,
    {
    	0, 0, 0,
    	1024, 0, 1404,     // 1, 0, 1.371
    	1024, -345, -715,  // 1, -0.336, -0.698
    	1024, 1774, 0,     // 1, 1.732, 0
    	0, 0, 0
    }
};

// === EXTERN VARIABLE ====================================================================

// === STATIC VARIABLE ====================================================================
//sin(0 deg ~ 90 deg) = 0 ~ (2 ^ 14 = 16384)
static INT16 const COLOR_TRANSFORM_HUE[129] =
{
    0,201,402,603,803,1004,1205,1405,               
    1605,1805,2005,2204,2404,2602,2801,2998,        
    3196,3393,3589,3785,3980,4175,4369,4563,        
    4756,4948,5139,5329,5519,5708,5896,6083,        
    6269,6455,6639,6822,7005,7186,7366,7545,        
    7723,7900,8075,8249,8423,8594,8765,8934,        
    9102,9268,9434,9597,9759,9920,10079,10237,      
    10393,10548,10701,10853,11002,11150,11297,11442,
    11585,11726,11866,12003,12139,12273,12406,12536,
    12665,12791,12916,13038,13159,13278,13395,13510,
    13622,13733,13842,13948,14053,14155,14255,14353,
    14449,14543,14634,14723,14810,14895,14978,15058,
    15136,15212,15286,15357,15426,15492,15557,15618,
    15678,15735,15790,15842,15892,15940,15985,16028,
    16069,16107,16142,16175,16206,16234,16260,16284,
    16305,16323,16339,16353,16364,16372,16379,16382,
    16384
};


//VGA Color Process
//Inorder to multiply another 16bit variable and plus 1 sign bit,
// All floating point variable need to shift 15 bit = 2^15 = 32768
static INT32 const RGB2YUV[15] =
{
	0, 512, 512,
	4809, 934, 2449,       // 0.587, 0.114, 0.299
	-2777, 4186, -1409,    // -0.339, 0.511, -0.172
	-3506, -680, 4186,     // -0.428, -0.083, 0.511
	-74, -74, -74
};

static INT32 const SRC_VGA_RGB_YUV2RGB[15] =
{
    -8, 504, 504,
    8323, 0, 11410,       // 1.0159, 0, 1.3928
    8323, -2796, -5809,   // 1, -0.3414, -0.7091
    8323, 14415, 0,       // 1.0159, 1.7596, 0
    0, 0, 0
};

static INT32 const SRC_RGB_YUV2RGB[15] =
{
	0, 512, 512,
	8192, 0, 11231,        // 1, 0, 1.371
	8192, -2753, -5718,    // 1, -0.336, -0.698
	8192, 14189, 0,        // 1, 1.732, 0
	0, 0, 0
};

static INT32 const SRC_YUV_YUV2RGB[15] =
{
    0, 0, 0,
    8192, 0, 11231,        // 1, 0, 1.371
    8192, -2753, -5718,    // 1, -0.336, -0.698
    8192, 14189, 0,        // 1, 1.732, 0
    0, 0, 0
};

static INT32 const SRC_YUV_YUV2RGBEXTLVL[15] =
{
    0, 0, 0,
    9568, 0, 13117,       // 1.168, 0, 1.601
    9568, -3215, -6678,   // 1.168, -0.394, -0.813
    9568, 16572, 0,       // 1.168, 2.023, 0
    -75, -75, -75
};

static INT32 const UNITRANSFROM[15] =
{
	0x0000,    0x0000,    0x0000,
	0x2000,    0x0000,    0x0000,
	0x0000,    0x2000,    0x0000,
	0x0000,    0x0000,    0x2000,
	0x0000,    0x0000,    0x0000
};

// === BODY ===============================================================================
/*****************************************************************************************/
/*********************************   3x3 Matrix   ****************************************/
/*****************************************************************************************/
void vSetForceLoadMatrix(UINT32 u4OnOff)
{
    vIO32WriteFldAlign(PEUI_00, u4OnOff, FORCE_LOAD_MATRIX);
}

UINT32 u4GetForceLoadMatrix(void)
{
    return IO32ReadFldAlign(PEUI_00, FORCE_LOAD_MATRIX);
}

/**
 * @brief load 3x3 color transform matrix
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param pwTable every 3x3 matrix has 15 TRANS_INDEX entries, see COLOR_TRANSFORM[]
 *        00h~02h		input sign and offset
 *        03h~0Bh		3x3 matrix coefficients
 *        0Ch~0Eh		clamp sign, output sign, and output offset
 *
 */
void vLoadColorMatrix(UINT8 bPath, const UINT16 * pwTable)
{
    CRIT_STATE_T csState;
    UINT16 wOffset = (bPath == SV_VP_MAIN) ? 0x0 : MATRIX3x3_OFFSET;

    if (pwTable == NULL)
    {
        return;
    }

    csState = x_crit_start();

    vRegWriteFldMulti(TRANS_MAIN_00 + wOffset,
                      P_Fld((pwTable[14] & 0x800) >> 11, OUT_S2P)
                      | P_Fld((pwTable[13] & 0x800) >> 11, OUT_S1P)
                      | P_Fld((pwTable[12] & 0x800) >> 11, OUT_S0P)
                      | P_Fld((pwTable[14] & 0x1000) >> 12, CLAMP_S2P)
                      | P_Fld((pwTable[13] & 0x1000) >> 12, CLAMP_S1P)
                      | P_Fld((pwTable[12] & 0x1000) >> 12, CLAMP_S0P)
                      | P_Fld((pwTable[2] & 0x800) >> 11, IN_S2P)
                      | P_Fld((pwTable[1] & 0x800) >> 11, IN_S1P)
                      | P_Fld((pwTable[0] & 0x800) >> 11, IN_S0P)
                      | P_Fld(pwTable[1] & 0x7FF, IN_OFFSET_1P)
                      | P_Fld(pwTable[0] & 0x7FF, IN_OFFSET_0P));
    vRegWriteFldMulti(TRANS_MAIN_01 + wOffset,
                      P_Fld(pwTable[12] & 0x7FF, OUT_OFFSET_0P)
                      | P_Fld(pwTable[2] & 0x7FF, IN_OFFSET_2P));
    vRegWriteFldMulti(TRANS_MAIN_02 + wOffset,
                      P_Fld(pwTable[14] & 0x7FF, OUT_OFFSET_2P)
                      | P_Fld(pwTable[13] & 0x7FF, OUT_OFFSET_1P));
    vRegWriteFldMulti(TRANS_MAIN_03 + wOffset,
                      P_Fld(pwTable[4], COEFF_01P)
                      | P_Fld(pwTable[3], COEFF_00P));
    vRegWriteFldMulti(TRANS_MAIN_04 + wOffset,
                      P_Fld(pwTable[6], COEFF_10P)
                      | P_Fld(pwTable[5], COEFF_02P));
    vRegWriteFldMulti(TRANS_MAIN_05 + wOffset,
                      P_Fld(pwTable[8], COEFF_12P)
                      | P_Fld(pwTable[7], COEFF_11P));
    vRegWriteFldMulti(TRANS_MAIN_06 + wOffset,
                      P_Fld(pwTable[10], COEFF_21P)
                      | P_Fld(pwTable[9], COEFF_20P));
    vRegWriteFldAlign(TRANS_MAIN_07 + wOffset, pwTable[11], COEFF_22P);

    vRegWriteFldAlign(TRANS_MAIN_07 + wOffset, 0, CONTROL_SEL);
    vRegWriteFldAlign(TRANS_MAIN_07 + wOffset, 6, INPUT_SELP);

    x_crit_end(csState);
}


/*****************************************************************************************/
/*********************************  Output Stage 3x3 Matrix   **********************************/
/*****************************************************************************************/
/**
 * @brief load output stage 3x3 color transform matrix
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param pwTable every 3x3 matrix has 15 TRANS_INDEX entries, see vDrvVideoColorMatrix()
 */
void vLoadOSColorMatrix(UINT8 bPath, const INT32 * pwTable)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OS_8D, pwTable[0], YIN_OFFSET_MAIN);
        vRegWriteFldAlign(OS_8D, pwTable[1], CIN_OFFSET_MAIN);
        vRegWriteFldAlign(OS_8E, pwTable[2], VIN_OFFSET_MAIN);

        vRegWriteFldAlign(OS_84, pwTable[3], Y2R_MATRIX00);
        vRegWriteFldAlign(OS_84, pwTable[4], Y2R_MATRIX01);
        vRegWriteFldAlign(OS_85, pwTable[5], Y2R_MATRIX02);
        vRegWriteFldAlign(OS_85, pwTable[6], Y2R_MATRIX10);
        vRegWriteFldAlign(OS_86, pwTable[7], Y2R_MATRIX11);
        vRegWriteFldAlign(OS_86, pwTable[8], Y2R_MATRIX12);
        vRegWriteFldAlign(OS_87, pwTable[9], Y2R_MATRIX20);
        vRegWriteFldAlign(OS_87, pwTable[10], Y2R_MATRIX21);
        vRegWriteFldAlign(OS_88, pwTable[11], Y2R_MATRIX22);

        vRegWriteFldAlign(OS_2D, pwTable[12], R_OFFSET_MAIN);
        vRegWriteFldAlign(OS_2E, pwTable[13], G_OFFSET_MAIN);
        vRegWriteFldAlign(OS_2F, pwTable[14], B_OFFSET_MAIN);
    }
    else
    {
        vRegWriteFldAlign(OS_8E, pwTable[0], YIN_OFFSET_PIP);
        vRegWriteFldAlign(OS_8F, pwTable[1], CIN_OFFSET_PIP);
        vRegWriteFldAlign(OS_8F, pwTable[2], VIN_OFFSET_PIP);

        vRegWriteFldAlign(OS_88, pwTable[3], Y2R_MATRIX00_PIP);
        vRegWriteFldAlign(OS_89, pwTable[4], Y2R_MATRIX01_PIP);
        vRegWriteFldAlign(OS_89, pwTable[5], Y2R_MATRIX02_PIP);
        vRegWriteFldAlign(OS_8A, pwTable[6], Y2R_MATRIX10_PIP);
        vRegWriteFldAlign(OS_8A, pwTable[7], Y2R_MATRIX11_PIP);
        vRegWriteFldAlign(OS_8B, pwTable[8], Y2R_MATRIX12_PIP);
        vRegWriteFldAlign(OS_8B, pwTable[9], Y2R_MATRIX20_PIP);
        vRegWriteFldAlign(OS_8C, pwTable[10], Y2R_MATRIX21_PIP);
        vRegWriteFldAlign(OS_8C, pwTable[11], Y2R_MATRIX22_PIP);

        vRegWriteFldAlign(OS_2D, pwTable[12], R_OFFSET_PIP);
        vRegWriteFldAlign(OS_2E, pwTable[13], G_OFFSET_PIP);
        vRegWriteFldAlign(OS_2F, pwTable[14], B_OFFSET_PIP);
    }
}


void vDrvSetPedestalMatrix(UINT8 bTransIndex)
{
    UINT8 i;
    INT32 i4Temp;
    INT32 i4Temp2;

    x_memcpy(pPedestalMatrix, COLOR_TRANSFORM[bTransIndex], sizeof(pPedestalMatrix));

    switch (bTransIndex)
    {
        case 4: // 4    YCbCr601(Limit) -> YCbCr601(Limit)
        case 5: // 5    YCbCr601(Full) -> YCbCr601(Limit)
        case 8: // 8    YCbCr709(Limit) -> YCbCr601(Limit)
        case 9: // 9    YCbCr709(Full) -> YCbCr601(Limit)
        case 12: // 12   TVD NTSC YUV -> YCbCr601(Limit)
        case 13: // 13   TVD NTSC_J YUV -> YCbCr601(Limit)
        case 14: // 14   TVD PAL YUV -> YCbCr601(Limit)
        case 15: // 15   TVD PAL YUV -> YCbCr601(Limit) //not support ultra black
            // Original Pre3x3: Yout = (Yin +In_offset[0])*Coef[3] +Out_offset[0] + (Uin+In_offset[1])*Coef[4] + (Vin +In_offset[2])*Coef[5]
            // 7.5 pedestal function: Yfinal = ((Yout -64) - (940-64)*7.5%)*100/(92.5) +64 = (Yout -129.7)*100/92.5 + 64
            // Yfinal =(Yin + In_offset[0] - 129.7/Coef[3]) * (Coef[3] *100/92.5) + Out_offset[0]*100/92.5 + 64 + (Uin+In_offset[1])*Coef[4]*100/92.5 + (Vin +In_offset[2])*Coef[5]*100/92.5
            // In_offset[0]' = In_offset[0] - 129.7/Coef[3]
            // Coef[3~5]' = Coef[3~5] *100/92.5
            // Out_offset[0]' = Out_offset[0]*100/92.5 + 64            
            i4Temp = pPedestalMatrix[0]&0x7FF;
            i4Temp = i4Temp&0x400 ? (i4Temp-0x800) : i4Temp;
            i4Temp2 = pPedestalMatrix[3]&0xFFF;
            i4Temp2 = i4Temp2&0x800 ? (i4Temp2-0x1000) : i4Temp2;        
            i4Temp = i4Temp - (INT32)((INT32)129*0x200)/i4Temp2;        
            i4Temp = i4Temp>0x3FF ? 0x3FF: i4Temp;
            i4Temp = i4Temp<-0x400 ? -0x400: i4Temp;
            i4Temp = i4Temp&0x7FF;
            pPedestalMatrix[0] = (pPedestalMatrix[0]&0xF800) | i4Temp;
            
            for(i=3;i<6;i++)
            {
                i4Temp = pPedestalMatrix[i]&0xFFF;
                i4Temp = i4Temp&0x800 ? (i4Temp-0x1000) : i4Temp;
            
                i4Temp =((i4Temp * 0x229)+0x100) >> 9;
                i4Temp = i4Temp>0x7FF ? 0x7FF: i4Temp;
                i4Temp = i4Temp<-0x800 ? -0x800: i4Temp;
                i4Temp = i4Temp&0xFFF;
                 
                pPedestalMatrix[i] = (pPedestalMatrix[i]&0xF000) | i4Temp;
            } 
            
            i4Temp = pPedestalMatrix[12]&0x7FF;
            i4Temp = i4Temp&0x400 ? (i4Temp-0x800) : i4Temp;
            i4Temp = (((i4Temp * 0x229)+0x100) >> 9) + 64;
            i4Temp = i4Temp>0x3FF ? 0x3FF: i4Temp;
            i4Temp = i4Temp<-0x400 ? -0x400: i4Temp;
            i4Temp = i4Temp&0x7FF;
            
            pPedestalMatrix[12] = (pPedestalMatrix[12]&0xF800) | i4Temp;
            break;
        case 0: // 0    RGB(Full) -> RGB (Full)
        case 1: // 1    RGB(Limit) -> RGB (Full)
        case 6: // 6    YCbCr601(Limit) -> RGB(Full)
        case 7: // 7    YCbCr601(Full) -> RGB(Full)
        case 10: // 10   YCbCr709(Limit) -> RGB(Full)
        case 11: // 11   YCbCr709(Full) -> RGB(Full)        
            // In_offset[0~2]' = In_offset[0~2]
            // Coef[3~11]' = Coef[3~5] *100/92.5
            // Out_offset[0~2]' = Out_offset[0~2]*100/92.5 -129.7*100/92.5 + 64
            for(i=3;i<12;i++)
            {
                i4Temp = pPedestalMatrix[i]&0xFFF;
                i4Temp = i4Temp&0x800 ? (i4Temp-0x1000) : i4Temp;

                i4Temp = ((i4Temp * 0x229)+ 0x100) >> 9;
                i4Temp = i4Temp>0x7FF ? 0x7FF: i4Temp;
                i4Temp = i4Temp<-0x800 ? -0x800: i4Temp;
                i4Temp = i4Temp&0xFFF;
                 
                pPedestalMatrix[i] = (pPedestalMatrix[i]&0xF000) | i4Temp;
            }
            pPedestalMatrix[12] = 0x7AD;
            pPedestalMatrix[13] = 0x7AD;
            pPedestalMatrix[14] = 0x7AD;
            break;

        case 2: // 2    RGB(Full) -> YCbCr601(Limit)
        case 3: // 3    RGB(Limit) -> YCbCr601(Limit)        
        case 16: // 16   SCART RGB, BGR->YCbCr(Limit)  (under fast blanking mode, TVD can only mix RGB signal as BGR order)
        case 17: // 17   SCART RGB, GBR -> YCbCr601(Limit)        
            // In_offset[0]' = In_offset[0]
            // Coef[3~5]' = Coef[3~5] *100/92.5
            // Out_offset[0]' = Out_offset[0]*100/92.5 -129.7*100/92.5 + 64
            for(i=3;i<6;i++)
            {
                i4Temp = pPedestalMatrix[i]&0xFFF;
                i4Temp = i4Temp&0x800 ? (i4Temp-0x1000) : i4Temp;

                i4Temp = ((i4Temp * 0x229) + 0x100) >> 9;
                i4Temp = i4Temp>0x7FF ? 0x7FF: i4Temp;
                i4Temp = i4Temp<-0x800 ? -0x800: i4Temp;
                i4Temp = i4Temp&0xFFF;
                 
                pPedestalMatrix[i] = (pPedestalMatrix[i]&0xF000) | i4Temp;
            }
            
            i4Temp = pPedestalMatrix[12]&0x7FF;
            i4Temp = i4Temp&0x400 ? (i4Temp-0x800) : i4Temp;
            i4Temp = (((i4Temp * 0x229) + 0x100) >> 9) - 76;
            i4Temp = i4Temp>0x3FF ? 0x3FF: i4Temp;
            i4Temp = i4Temp<-0x400 ? -0x400: i4Temp;
            i4Temp = i4Temp&0x7FF;
            pPedestalMatrix[12] = (pPedestalMatrix[12]&0xF800) | i4Temp;
            break;
        default:
            break;
    }
}


/**
 * @brief set color transform matrix
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bHdtv for 720p/1080i
 */
void vDrvColorTransform(UINT8 bPath)
{
    UINT8 bColorSys, bMonColorTransform, bHdtv, bHDMIRange;

    bColorSys = ((bPath == SV_VP_MAIN) ? _bMAIN_COLOR_SYS : _bPIP_COLOR_SYS);
    bHdtv = (wDrvVideoInputHeight(bPath) >= 720);
    DRVCUST_SetPedestalCtrl((UINT8)u4DrvGetPedOnOff());
    bHDMIRange = bHDMIRgbRange();

    switch (bColorSys)
    {
        case SV_COLOR_RGB_VGA:
            bMonColorTransform = SV_COLORTRANS_RGBF_2_RGBF;
            break;

        case SV_COLOR_RGB_YUV:      // if SUPPORT_EXTMJC_RGBTOYUV_OUTPUT
            bMonColorTransform = SV_COLORTRANS_RGBF_2_YCBCR601L;
            break;

        case SV_COLOR_YPBPR_DTV_SD:
            bMonColorTransform = SV_COLORTRANS_YCBCR601L_2_YCBCR601L;
            break;

        case SV_COLOR_YPBPR_DTV_HD:
            bMonColorTransform = SV_COLORTRANS_YCBCR709L_2_YCBCR601L;
            break;

        case SV_COLOR_YCBCR:
        case SV_COLOR_YCBCR_RGB:
        case SV_COLOR_YPBPR_ANALOG:
        case SV_COLOR_YPBPR_DIGITAL:
        case SV_COLOR_HDMI_422_601:
        case SV_COLOR_HDMI_444_601:
        case SV_COLOR_HDMI_422_709:
        case SV_COLOR_HDMI_444_709:
            if (bIsScalerInput444(bPath))
            {
                if (bHdtv == SV_FALSE)
                {   // SDTV
                    if (bHDMIRange == SV_HDMI_RANGE_FULL)   // full range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR601F_2_RGBF;
                    }
                    else if (bHDMIRange == SV_HDMI_RANGE_LIMIT)    // limit range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR601L_2_RGBF;
                    }
                    else    // default
                    {
                        bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
                    }
                }
                else    //HDTV
                {
                    if (bHDMIRange == SV_HDMI_RANGE_FULL)    // full range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR709F_2_RGBF;
                    }
                    else if (bHDMIRange == SV_HDMI_RANGE_LIMIT)     // limit range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR709L_2_RGBF;
                    }
                    else     // default
                    {
                        bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
                    }
                }
            }
            else
            {
                if (bHdtv == SV_FALSE)
                {   // SDTV
                    if (bHDMIRange == SV_HDMI_RANGE_FULL)    // full range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR601F_2_YCBCR601L;
                    }
                    else if (bHDMIRange == SV_HDMI_RANGE_LIMIT)     // limit range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR601L_2_YCBCR601L;
                    }
                    else    // default
                    {
                        bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
                    }
                }
                else    //HDTV
                {
                    if (bHDMIRange == SV_HDMI_RANGE_FULL)    // full range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR709F_2_YCBCR601L;
                    }
                    else if (bHDMIRange == SV_HDMI_RANGE_LIMIT)     // limit range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR709L_2_YCBCR601L;
                    }
                    else    // default
                    {
                        bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
                    }
                }
            }
            break;

        case SV_COLOR_HDMI_RGB:
            if (bIsScalerInput444(bPath))
            {
                if (bHDMIRange == SV_HDMI_RANGE_LIMIT)    // limit range
                {
                    bMonColorTransform = SV_COLORTRANS_RGBL_2_RGBF;
                }
                else if (bHDMIRange == SV_HDMI_RANGE_FULL)    // full range
                {
                    bMonColorTransform = SV_COLORTRANS_RGBF_2_RGBF;
                }
                else    // default
                {
                    bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
                }
            }
            else
            {
                if (bHDMIRange == SV_HDMI_RANGE_LIMIT) // limit range
                {
                    bMonColorTransform = SV_COLORTRANS_RGBL_2_YCBCR601L;
                }
                else if (bHDMIRange == SV_HDMI_RANGE_FULL)    // full range
                {
                    bMonColorTransform = SV_COLORTRANS_RGBF_2_YCBCR601L;
                }
                else    // default
                {
                    bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
                }
            }
            break;

        case SV_COLOR_BGR:
            // SDTV RGB
            bMonColorTransform = SV_COLORTRANS_SCARTBGR_2_YCBCR601L;
            break;

        case SV_COLOR_GBR:
            //SDTV RGB
            bMonColorTransform = SV_COLORTRANS_SCARTGBR_2_YCBCR601L;
            break;

        case SV_COLOR_YCBCR_NTSC:
            bMonColorTransform = SV_COLORTRANS_YUVNTSC_2_YCBCR601L;
            break;

        case SV_COLOR_YCBCR_NTSCJ:
            bMonColorTransform = SV_COLORTRANS_YUVNTSCJ_2_YCBCR601L;
            break;

        case SV_COLOR_YCBCR_PAL:
            bMonColorTransform = SV_COLORTRANS_YUVPAL_2_YCBCR601L;
            break;

        case SV_COLOR_RGB:
        default:
            if (bHDMIRgbRange() == SV_HDMI_RANGE_LIMIT)    // limit range
            {
                bMonColorTransform = SV_COLORTRANS_RGBL_2_RGBF;
            }
            else if (bHDMIRgbRange() == SV_HDMI_RANGE_FULL)    // full range
            {
                bMonColorTransform = SV_COLORTRANS_RGBF_2_RGBF;
            }
            else    // default
            {
                bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
            }
            break;
    }

    if (!u4GetForceLoadMatrix())
    {
        if(u4DrvGetPedOnOff()== SV_ON)
        {
            vDrvSetPedestalMatrix(bMonColorTransform);
            vLoadColorMatrix(bPath,pPedestalMatrix );
        }
        else
        {
            vLoadColorMatrix(bPath, COLOR_TRANSFORM[bMonColorTransform]);
        }
    }

    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(MATRIX_00, bMonColorTransform, MATRIXIDX_MAIN);
        vIO32WriteFldAlign(MATRIX_00, bIsScalerInput444(bPath), SCALER444_MAIN);
    }
    else
    {
        vIO32WriteFldAlign(MATRIX_00, bMonColorTransform, MATRIXIDX_PIP);
        vIO32WriteFldAlign(MATRIX_00, bIsScalerInput444(bPath), SCALER444_PIP);
    }
}

void vDrvMatrixCtrlSel(UINT8 bPath, UINT8 bMode)
{
    UINT16 wOffset = (bPath == SV_VP_MAIN) ? 0x0 : MATRIX3x3_OFFSET;
    vRegWriteFldAlign(TRANS_MAIN_07 + wOffset, bMode, CONTROL_SEL);
}

void vDrvOSMatrixOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if (bPath == SV_VP_MAIN)
	{
		vRegWriteFldAlign(OS_84, ~bOnOff, YUV2RGB_BYPASS_MAIN);
		vRegWriteFldAlign(OS_2D, ~bOnOff, MAIN_RGB_OFFSET_BYPASS);
	}
	else
	{
		vRegWriteFldAlign(OS_84, ~bOnOff, YUV2RGB_BYPASS_PIP);
		vRegWriteFldAlign(OS_2E, ~bOnOff, PIP_RGB_OFFSET_BYPASS);
	}
}

 /**
 * @brief enable/disable main path 422 mode
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvMain422(UINT8 bOnOff)
{
    vRegWriteFldAlign(YCBCR2YC_MAIN_00, bOnOff, C_ENA);
    vRegWriteFldAlign(YCBCR2YC_MAIN_00, bOnOff, C_FILTER_ON);
    vRegWriteFldAlign(YCBCR2YC_MAIN_00, (~bOnOff), C_NOSYNC_BLOCK);

     // OS 3x3matrix use standard Y2R for 422, and will be be filled by vDrvCalVideoColorMatrix() for RGB444
    if (bOnOff == SV_ON)
    {
        if (DRVCUST_OptGet(eFlagUseMatrixPQAdj))
        {
            vDrvVideoColorMatrix(SV_VP_MAIN);
        }
        else
        {
            x_memcpy(COLOR_TRANSFORM_ADJ, YCBCR2RGBOS[DRVCUST_Y2RMap()], sizeof(COLOR_TRANSFORM_ADJ));
            if (!u4GetForceLoadMatrix())
            {
            vLoadOSColorMatrix(SV_VP_MAIN, COLOR_TRANSFORM_ADJ);
        }
    }
    }
    else
    {
        vDrvVideoColorMatrix(SV_VP_MAIN);
    }
 }

/**
 * @brief enable/disable pip path 422 mode
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvPip422(UINT8 bOnOff)
{
    vRegWriteFldAlign(YCBCR2YC_PIP_00, bOnOff, C_ENA);
    vRegWriteFldAlign(YCBCR2YC_PIP_00, bOnOff, C_FILTER_ON);
    vRegWriteFldAlign(YCBCR2YC_PIP_00, (~bOnOff), C_NOSYNC_BLOCK);
    vDrvVideoColorMatrix(SV_VP_PIP);
 }


/**OS Matrix Color Process
 * @brief set split line for demo mode
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
void vDrvVideoColorMatrix(UINT8 bPath)
{
    UINT8 u1Offset = (bPath == SV_VP_MAIN) ? 0 : 0x4;

    u2PQItemMatrix[bPath][PQ_BRIGHTNESS] = IO32ReadFldAlign(GLOBAL_ADJ_00 + u1Offset, BRIGHT_DUMMY);
    u2PQItemMatrix[bPath][PQ_CONTRAST] = IO32ReadFldAlign(GLOBAL_ADJ_02 + u1Offset, CONT_DUMMY);
    u2PQItemMatrix[bPath][PQ_SATURATION] = IO32ReadFldAlign(GLOBAL_ADJ_02 + u1Offset, SAT_DUMMY);
    u2PQItemMatrix[bPath][PQ_HUE] = IO32ReadFldAlign(GLOBAL_ADJ_00 + u1Offset, HUE_DUMMY);

    vDrvCalVideoColorMatrix(bPath,
        u2PQItemMatrix[bPath][PQ_BRIGHTNESS],
        u2PQItemMatrix[bPath][PQ_CONTRAST],
        u2PQItemMatrix[bPath][PQ_SATURATION],
        u2PQItemMatrix[bPath][PQ_HUE]);
}


void vDrvCalVideoColorMatrix(UINT8 bPath, UINT16 wBriValue, UINT16 wContValue,
                          UINT16 wSatValue, UINT16 wHueValue)
{
    INT8 bi;
    INT16 wCos_v, wSin_v;
    INT32  dwTable[15], dwTableY2R[15];
    UINT32 u4ForceLmtOut = u4DrvTDTV3DModeQuery() !=E_TDTV_UI_3D_MODE_OFF;

    u4ForceLmtOut = IS_LVDS_DISP_3D_SHUTTER ? u4ForceLmtOut : 0;

    LOG(3, "vDrvVideoColorMatrix %d %d %d %d %d\n", bPath, wBriValue, wContValue, wSatValue, wHueValue);

    x_memcpy(dwTable, (bIsScalerInput444(bPath))? RGB2YUV : UNITRANSFROM, sizeof(dwTable));
    x_memcpy(COLOR_TRANSFORM_ADJ, dwTable, sizeof(COLOR_TRANSFORM_ADJ));

    if (wHueValue > 0x80)
    {
        wHueValue = (wHueValue > 0x100) ? 0x80 : (wHueValue - 0x80);
        wCos_v = COLOR_TRANSFORM_HUE[0x80 - wHueValue];                                                                                       
        wSin_v = COLOR_TRANSFORM_HUE[wHueValue];                                                                                        
    }
    else
    {
        wHueValue = (0x80 - wHueValue);
        wCos_v = COLOR_TRANSFORM_HUE[0x80 - wHueValue];                                                                                               
        wSin_v = -COLOR_TRANSFORM_HUE[wHueValue];
    }    


    //Contrast Adjustment
    for (bi = 3; bi < 6; bi++)
    {
        COLOR_TRANSFORM_ADJ[bi] = dwTable[bi] * wContValue >> 7;
    }

    //Hue Adjustment
    COLOR_TRANSFORM_ADJ[6] = (dwTable[6] * wCos_v + dwTable[9] * (-wSin_v) + (1<<13)) >> 14;
    COLOR_TRANSFORM_ADJ[7] = (dwTable[7] * wCos_v + dwTable[10] * (-wSin_v) + (1<<13)) >> 14;
    COLOR_TRANSFORM_ADJ[8] = (dwTable[8] * wCos_v + dwTable[11] * (-wSin_v) + (1<<13)) >> 14;
    COLOR_TRANSFORM_ADJ[9] = (dwTable[6] * wSin_v + dwTable[9] * wCos_v + (1<<13)) >> 14;
    COLOR_TRANSFORM_ADJ[10] = (dwTable[7] * wSin_v + dwTable[10] * wCos_v + (1<<13)) >> 14;
    COLOR_TRANSFORM_ADJ[11] = (dwTable[8] * wSin_v + dwTable[11] * wCos_v + (1<<13)) >> 14;

    //Saturation Adjustment
    for (bi = 6; bi < 12; bi++)
    {
        //Bind Contrast / Saturation Done in ApiVideoProc
        COLOR_TRANSFORM_ADJ[bi] = (COLOR_TRANSFORM_ADJ[bi] * wSatValue) >> 7;
    }

    x_memcpy(dwTable, COLOR_TRANSFORM_ADJ, sizeof(dwTable));

    if (bGetSignalType(bPath) == SV_ST_VGA)
    {
        x_memcpy(dwTableY2R, (bIsScalerInput444(bPath))? SRC_VGA_RGB_YUV2RGB : SRC_YUV_YUV2RGBEXTLVL, sizeof(dwTableY2R));
    }
    else
    {
        x_memcpy(dwTableY2R, (bIsScalerInput444(bPath))? SRC_RGB_YUV2RGB : 
                              (u4ForceLmtOut ? SRC_YUV_YUV2RGB : SRC_YUV_YUV2RGBEXTLVL), 
                              sizeof(dwTableY2R));
    }

    //YUV-> RGB
    COLOR_TRANSFORM_ADJ[3] =
        (((dwTableY2R[3]) * (dwTable[3])) +
        ((dwTableY2R[4]) * (dwTable[6])) +
        ((dwTableY2R[5]) * (dwTable[9])) + (1<<15)) >> 16;
    COLOR_TRANSFORM_ADJ[4] =
        (((dwTableY2R[3]) * (dwTable[4])) +
        ((dwTableY2R[4]) * (dwTable[7])) +
        ((dwTableY2R[5]) * (dwTable[10])) + (1<<15)) >> 16;
    COLOR_TRANSFORM_ADJ[5] =
        (((dwTableY2R[3]) * (dwTable[5])) +
        ((dwTableY2R[4]) * (dwTable[8])) +
        ((dwTableY2R[5]) * (dwTable[11])) + (1<<15)) >> 16;

    COLOR_TRANSFORM_ADJ[6] =
        (((dwTableY2R[6]) * (dwTable[3])) +
        ((dwTableY2R[7]) * (dwTable[6])) +
        ((dwTableY2R[8]) * (dwTable[9])) + (1<<15)) >> 16;
    COLOR_TRANSFORM_ADJ[7] =
        (((dwTableY2R[6]) * (dwTable[4])) +
        ((dwTableY2R[7]) * (dwTable[7])) +
        ((dwTableY2R[8]) * (dwTable[10])) + (1<<15)) >> 16;
    COLOR_TRANSFORM_ADJ[8] =
        (((dwTableY2R[6]) * (dwTable[5])) +
        ((dwTableY2R[7]) * (dwTable[8])) +
        ((dwTableY2R[8]) * (dwTable[11])) + (1<<15)) >> 16;

    COLOR_TRANSFORM_ADJ[9] =
        (((dwTableY2R[9]) * (dwTable[3])) +
        ((dwTableY2R[10]) * (dwTable[6])) +
        ((dwTableY2R[11]) * (dwTable[9])) + (1<<15)) >> 16;
    COLOR_TRANSFORM_ADJ[10] =
        (((dwTableY2R[9]) * (dwTable[4])) +
        ((dwTableY2R[10]) * (dwTable[7])) +
        ((dwTableY2R[11]) * (dwTable[10])) + (1<<15)) >> 16;
    COLOR_TRANSFORM_ADJ[11] =
        (((dwTableY2R[9]) * (dwTable[5])) +
        ((dwTableY2R[10]) * (dwTable[8])) +
        ((dwTableY2R[11]) * (dwTable[11])) + (1<<15)) >> 16;

    COLOR_TRANSFORM_ADJ[0] = dwTableY2R[0];
    COLOR_TRANSFORM_ADJ[1] = dwTableY2R[1];
    COLOR_TRANSFORM_ADJ[2] = dwTableY2R[2];

    //Brightness Adjustment
    COLOR_TRANSFORM_ADJ[12] = ((dwTableY2R[12]*(INT32)wContValue)>>7) + ((wBriValue - 0x80) << DRVCUST_OptGet(eBrightInterval));
    COLOR_TRANSFORM_ADJ[13] = COLOR_TRANSFORM_ADJ[12];
    COLOR_TRANSFORM_ADJ[14] = COLOR_TRANSFORM_ADJ[12];

    // Overflow protection: gain
    for (bi = 3; bi < 12; bi++)
    {
        if (COLOR_TRANSFORM_ADJ[bi] > 0x1FFF)
        {
            COLOR_TRANSFORM_ADJ[bi] = 0x1FFF;
        }
    }

    // Overflow protection: offset
    for (bi = 12; bi < 15; bi++)
    {
        if (COLOR_TRANSFORM_ADJ[bi] > 1023)
        {
            COLOR_TRANSFORM_ADJ[bi] = 1023;
        }
        if (COLOR_TRANSFORM_ADJ[bi] < -1024)
        {
            COLOR_TRANSFORM_ADJ[bi] = -1024;
        }
    }

    if (!u4GetForceLoadMatrix())
    {
        vLoadOSColorMatrix(bPath, COLOR_TRANSFORM_ADJ);
    }
}

void vDrvSetPedOnOff(UINT32 u4OnOff)
{
    _u4MatrixPedEn = u4OnOff;
}

UINT32 u4DrvGetPedOnOff(void)
{
    return _u4MatrixPedEn;
}

void vDrvSetUniColorDomain(UINT8 bColorDomain, UINT8 bLvdsMode)
{
#ifdef DRV_SUPPORT_EXTMJC
    if (!IS_COMPANION_CHIP_ON())
        return;

    if ((bLvdsMode == eLvdsVidNormal) || (bLvdsMode == eLvdsVidRgb444OnePort) || (bLvdsMode == eLvdsVidRgb444TwoPort))
    {
        vRegWriteFldAlign(OS_FF, 1, RGB2YUV_BYPASS);
    }
    else
    {
        vRegWriteFldAlign(OS_FF, 0, RGB2YUV_BYPASS);
    }
#endif
}

void vDrvSetPipColorDomain(void)
{
#ifdef DRV_SUPPORT_EXTMJC
#if SUPPORT_EXTMJC_RGBTOYUV_OUTPUT
	if (!IS_COMPANION_CHIP_ON())
		return;

	LOG(3, "fgApiVideoIsVgaTiming %d %d\n", fgApiVideoIsVgaTiming(SV_VP_MAIN),
	    fgApiVideoIsVgaTiming(SV_VP_PIP));
	if (fgApiVideoIsVgaTiming(SV_VP_MAIN) && (!bIsScalerInput444(SV_VP_PIP)))
	{
		vRegWriteFldAlign(OS_84, 0, YUV2RGB_BYPASS_PIP);
	}
	else
	{
		vRegWriteFldAlign(OS_84, 1, YUV2RGB_BYPASS_PIP);
	}

	if ((!fgApiVideoIsVgaTiming(SV_VP_MAIN)) && bIsScalerInput444(SV_VP_PIP))
	{
		vApiVideoSetFixColorSpaceMode(SV_VP_PIP, SV_FIXCOLORSPACE_RGB_TO_YCBCR);
		LOG(3,
		    "vDrvSetPipColorDomain=> SV_FIXCOLORSPACE_RGB_TO_YCBCR_ONCE\n");
	}
	else
	{
		vApiVideoSetFixColorSpaceMode(SV_VP_PIP, SV_FIXCOLORSPACE_OFF);
		LOG(3, "vDrvSetPipColorDomain=> SV_FIXCOLORSPCCE_OFF\n");
	}
#endif //SUPPORT_EXTMJC_RGBTOYUV_OUTPUT
#endif
#ifdef CC_SUPPORT_TVE
	UINT8 ucEnable;

	TVE_GetEnable(TVE_1, &ucEnable);
	if (ucEnable)
	{
		if (bIsScalerInput444(SV_VP_PIP))
		{
			if ((bGetVideoDecType(SV_VP_PIP) == SV_VD_DVI) && (bHDMIInputType()==0))
			{
				// YPcbr-> RGB for hdmi pc timing , force to ycbcr 422 in hdmi
				vApiVideoSetFixColorSpaceMode(SV_VP_PIP, SV_FIXCOLORSPACE_YCBCR);
				LOG(3,"vDrvSetPipColorDomain TVE=> SV_FIXCOLORSPACE_YCBCR_ONCE\n");
			}
			else
			{
				// RGB input , force to rgb-> ycbcr 422
				vApiVideoSetFixColorSpaceMode(SV_VP_PIP, SV_FIXCOLORSPACE_RGB_TO_YCBCR);
				LOG(3,"vDrvSetPipColorDomain TVE=> SV_FIXCOLORSPACE_RGB_TO_YCBCR_ONCE\n");
			}
		}
	}
#endif
}

