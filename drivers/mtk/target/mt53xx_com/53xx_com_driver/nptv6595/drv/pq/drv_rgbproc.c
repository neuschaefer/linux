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
#include "x_assert.h"
#include "x_os.h"	
#include "sv_const.h"
#include "hw_vdoin.h"
#include "nptv_debug.h"
#include "drv_pq_cust.h"

// === HW INCLUDE =============================================================================
#include "hw_ycproc.h"
#include "hw_sw.h"
#include "hw_ospe.h"

// === DEFINE =============================================================================
#define AMBILIGHT_OFFSET1       0xC
#define AMBILIGHT_OFFSET2       0x8
#define USE_POST_GAMMA          SV_FALSE
// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================


// === GLOBAL VARIABLE ====================================================================
UINT16 u2GammaDecodedTable[3][GAMMA_INDEX];
// === EXTERN VARIABLE ====================================================================


// === STATIC VARIABLE ====================================================================
static UINT8 _FLG_SW_GAMMA = SV_OFF;
static UINT8 bGammaOn;
static UINT16 wGammaZeroPtR, wGammaZeroPtG, wGammaZeroPtB;
static UINT16 wHwGammaCurve[3][257];
static UINT32 dwGammaR1024[257], dwGammaG1024[257], dwGammaB1024[257];

// === BODY ===============================================================================

/*****************************************************************************************/
/*************************************   DITHER   ****************************************/
/*****************************************************************************************/
void vDrvDitherInit(UINT8 bDitherMode)
{
#ifdef CC_MT5365    
    switch(bDitherMode)
    {
        case R_DITHER:
    		vRegWriteFldAlign(OS_15, 0x1, RDITHER_EN);            
    		vRegWriteFldAlign(OS_15, 0x1, REG_FPHASE_EN);            
    		vRegWriteFldAlign(OS_15, 0x5, FPHASE);                		

    		// todo : add magic table here for r-dither

    		vRegWriteFldAlign(OS_15, 0x0, EDITHER_EN);

    		vRegWriteFldAlign(OS_15, 0x0, DITHER_LFSR_EN);
    		vRegWriteFldAlign(OS_15, 0x0, LFSR_TIMING_EN);    		    		

    		vRegWriteFldAlign(OS_15, 0x0, ROUND_EN);    		

            // Magic Table For R-Dither
    		vRegWriteFldAlign(REG_DITHER_MAGIC_TABLE_0, 0x082AC4E6, REG_MAGIC_VALUE_0);
    		vRegWriteFldAlign(REG_DITHER_MAGIC_TABLE_1, 0x193BD5F7, REG_MAGIC_VALUE_1);
            break;
        case E_DITHER:
    		vRegWriteFldAlign(OS_15, 0x0, RDITHER_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, REG_FPHASE_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, FPHASE);                		

    		vRegWriteFldAlign(OS_15, 0x1, EDITHER_EN);

    		vRegWriteFldAlign(OS_15, 0x0, DITHER_LFSR_EN);
    		vRegWriteFldAlign(OS_15, 0x0, LFSR_TIMING_EN);    		    		

    		vRegWriteFldAlign(OS_15, 0x0, ROUND_EN);    		
            break;
        case E_R_DITHER:
    		vRegWriteFldAlign(OS_15, 0x1, RDITHER_EN);            
    		vRegWriteFldAlign(OS_15, 0x1, REG_FPHASE_EN);            
    		vRegWriteFldAlign(OS_15, 0x5, FPHASE);                		

    		vRegWriteFldAlign(OS_15, 0x1, EDITHER_EN);

    		vRegWriteFldAlign(OS_15, 0x0, DITHER_LFSR_EN);
    		vRegWriteFldAlign(OS_15, 0x0, LFSR_TIMING_EN);    		    		

    		vRegWriteFldAlign(OS_15, 0x0, ROUND_EN);    	

            // Magic Table For R-Dither
    		vRegWriteFldAlign(REG_DITHER_MAGIC_TABLE_0, 0x082AC4E6, REG_MAGIC_VALUE_0);
    		vRegWriteFldAlign(REG_DITHER_MAGIC_TABLE_1, 0x193BD5F7, REG_MAGIC_VALUE_1);    		
            break;
        case LFSR_DITHER:
    		vRegWriteFldAlign(OS_15, 0x0, RDITHER_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, REG_FPHASE_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, FPHASE);                		

    		vRegWriteFldAlign(OS_15, 0x0, EDITHER_EN);

    		vRegWriteFldAlign(OS_15, 0x1, DITHER_LFSR_EN);
    		vRegWriteFldAlign(OS_15, 0x0, LFSR_TIMING_EN);    		    		

    		vRegWriteFldAlign(OS_15, 0x0, ROUND_EN);    		            
            break;
        case ROUNDING:
    		vRegWriteFldAlign(OS_15, 0x0, RDITHER_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, REG_FPHASE_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, FPHASE);                		

    		vRegWriteFldAlign(OS_15, 0x0, EDITHER_EN);

    		vRegWriteFldAlign(OS_15, 0x0, DITHER_LFSR_EN);
    		vRegWriteFldAlign(OS_15, 0x0, LFSR_TIMING_EN);    		    		

    		vRegWriteFldAlign(OS_15, 0x1, ROUND_EN);    		            
            break;
        case DITHER_OFF:
    		vRegWriteFldAlign(OS_15, 0x0, RDITHER_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, REG_FPHASE_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, FPHASE);                		

    		vRegWriteFldAlign(OS_15, 0x0, EDITHER_EN);

    		vRegWriteFldAlign(OS_15, 0x0, DITHER_LFSR_EN);
    		vRegWriteFldAlign(OS_15, 0x0, LFSR_TIMING_EN);    		    		

    		vRegWriteFldAlign(OS_15, 0x0, ROUND_EN);    		            
            break;            
    }
#else   // 5395 Dither
    switch(bDitherMode)
    {        
        case R_DITHER:
            
    		vRegWriteFldAlign(OS_15, 0x1, RDITHER_EN);            
    		vRegWriteFldAlign(OS_15, 0x1, REG_FPHASE_EN);            
    		vRegWriteFldAlign(OS_15, 0x5, FPHASE);                		

    		// todo : add magic table here for r-dither

    		vRegWriteFldAlign(OS_15, 0x0, EDITHER_EN);

    		vRegWriteFldAlign(OS_15, 0x0, DITHER_LFSR_EN);
    		vRegWriteFldAlign(OS_15, 0x0, LFSR_TIMING_EN);    		    		

    		vRegWriteFldAlign(OS_15, 0x0, ROUND_EN);    		

            // 5395 New Feature
    		vRegWriteFldAlign(OS_F7, 0x1, REG_SUBPIX_EN);            
    		vRegWriteFldAlign(OS_F7, 0x0, REG_FPHASE_R);            
    		vRegWriteFldAlign(OS_F7, 0x1, FPHASE);                		
    		vRegWriteFldAlign(OS_F7, 0x0, REG_FPHASE_BIT);                		    		
    		vRegWriteFldAlign(OS_F7, 0x0, REG_FPHASE_SEL);   
    		
            break;
        case E_DITHER:
    		vRegWriteFldAlign(OS_15, 0x0, RDITHER_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, REG_FPHASE_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, FPHASE);                		

    		vRegWriteFldAlign(OS_15, 0x1, EDITHER_EN);

    		vRegWriteFldAlign(OS_15, 0x0, DITHER_LFSR_EN);
    		vRegWriteFldAlign(OS_15, 0x0, LFSR_TIMING_EN);    		    		

    		vRegWriteFldAlign(OS_15, 0x0, ROUND_EN);    		

            // 5395 New Feature
    		vRegWriteFldAlign(OS_F7, 0x1, REG_SUBPIX_EN);   
    		vRegWriteFldAlign(OS_F7, 0x0, REG_FPHASE_R);                        
            
            break;
        case E_R_DITHER:
    		vRegWriteFldAlign(OS_15, 0x1, RDITHER_EN);            
    		vRegWriteFldAlign(OS_15, 0x1, REG_FPHASE_EN);            
    		vRegWriteFldAlign(OS_15, 0x5, FPHASE);                		

    		vRegWriteFldAlign(OS_15, 0x1, EDITHER_EN);

    		vRegWriteFldAlign(OS_15, 0x0, DITHER_LFSR_EN);
    		vRegWriteFldAlign(OS_15, 0x0, LFSR_TIMING_EN);    		    		

    		vRegWriteFldAlign(OS_15, 0x0, ROUND_EN);    	
    		
            // 5395 New Feature
    		vRegWriteFldAlign(OS_F7, 0x1, REG_SUBPIX_EN);        
    		vRegWriteFldAlign(OS_F7, 0x0, REG_FPHASE_R);                        
    		vRegWriteFldAlign(OS_F7, 0x1, FPHASE);                		
    		vRegWriteFldAlign(OS_F7, 0x0, REG_FPHASE_BIT);                		    		
    		vRegWriteFldAlign(OS_F7, 0x0, REG_FPHASE_SEL);   
    		
            break;
        case LFSR_DITHER:
    		vRegWriteFldAlign(OS_15, 0x0, RDITHER_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, REG_FPHASE_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, FPHASE);                		

    		vRegWriteFldAlign(OS_15, 0x0, EDITHER_EN);

    		vRegWriteFldAlign(OS_15, 0x1, DITHER_LFSR_EN);
    		vRegWriteFldAlign(OS_15, 0x0, LFSR_TIMING_EN);    		    		

    		vRegWriteFldAlign(OS_15, 0x0, ROUND_EN);    		         

            // 5395 New Feature
    		vRegWriteFldAlign(OS_F7, 0x1, REG_SUBPIX_EN);            
    		vRegWriteFldAlign(OS_F7, 0x0, REG_FPHASE_R);        
    		
            break;
        case ROUNDING:
    		vRegWriteFldAlign(OS_15, 0x0, RDITHER_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, REG_FPHASE_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, FPHASE);                		

    		vRegWriteFldAlign(OS_15, 0x0, EDITHER_EN);

    		vRegWriteFldAlign(OS_15, 0x0, DITHER_LFSR_EN);
    		vRegWriteFldAlign(OS_15, 0x0, LFSR_TIMING_EN);    		    		

    		vRegWriteFldAlign(OS_15, 0x1, ROUND_EN);    		 

            // 5395 New Feature
    		vRegWriteFldAlign(OS_F7, 0x1, REG_SUBPIX_EN);            
    		vRegWriteFldAlign(OS_F7, 0x0, REG_FPHASE_R);            
    		
            break;
        case DITHER_OFF:
    		vRegWriteFldAlign(OS_15, 0x0, RDITHER_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, REG_FPHASE_EN);            
    		vRegWriteFldAlign(OS_15, 0x0, FPHASE);                		

    		vRegWriteFldAlign(OS_15, 0x0, EDITHER_EN);

    		vRegWriteFldAlign(OS_15, 0x0, DITHER_LFSR_EN);
    		vRegWriteFldAlign(OS_15, 0x0, LFSR_TIMING_EN);    		    		

    		vRegWriteFldAlign(OS_15, 0x0, ROUND_EN);    	

            // 5395 New Feature
    		vRegWriteFldAlign(OS_F7, 0x0, REG_SUBPIX_EN);            
    		vRegWriteFldAlign(OS_F7, 0x0, REG_FPHASE_R);   
    		vRegWriteFldAlign(OS_F7, 0x0, FPHASE);                		    		

    		vRegWriteFldAlign(OS_F7, 0x0, REG_FPHASE_BIT);                		    		
    		vRegWriteFldAlign(OS_F7, 0x0, REG_FPHASE_SEL);                		    		    		

    		break;            
    }
#endif //CC_MT5365    

    // todo : 5395 dither here
}


/*****************************************************************************************/
/************************************* Gamma Mute ****************************************/
/*****************************************************************************************/


void vDrvGammaMute(UINT16 wGammaMuteR, UINT16 wGammaMuteG, UINT16 wGammaMuteB)
{
#if !USE_POST_GAMMA        
	bGammaOn = RegReadFldAlign(OS_26, GM_EN);

	// 1. if gamma not on, turn it on to enable gamma mute
	if (!bGammaOn)
	{
		vRegWriteFldAlign(OS_26, 0x1, GM_EN);
	}
	vRegWriteFldAlign(OS_26, 0, GMT_WEN);

	// 2. read zero point
	// 2.1 gamma table read only
	vRegWriteFldAlign(OS_26, 1, GM_R_ONLY);

	// 2.2 select RGB mode, read all
	vRegWriteFldAlign(OS_26, 1, GMTWMODE);

	// 2.3 write address / read data
	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);

	wGammaZeroPtR = RegReadFldAlign(RO_21, GM_TABLE_R);
	wGammaZeroPtG = RegReadFldAlign(RO_20, GM_TABLE_G);
	wGammaZeroPtB = RegReadFldAlign(RO_20, GM_TABLE_B);

	Printf("%d, %d, %d\n", wGammaZeroPtR, wGammaZeroPtG, wGammaZeroPtB);

	// 2.4 all off
	vRegWriteFldAlign(OS_26, 0, GMTWMODE);

	// 2.5 gamma table read only off
	vRegWriteFldAlign(OS_26, 0, GM_R_ONLY);

	// 2.6 reset address
	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);

	// 3. turn on write mode
	vRegWriteFldAlign(OS_26, 1, GMTWMODE);

	// 4. use gamma write temporal as mute
	vRegWriteFldMulti(OS_22, P_Fld(0,     N_GM_WADD) |
	                  P_Fld(wGammaMuteR,     N_GM_WDATA_R) );
	vRegWriteFldMulti(OS_23, P_Fld(wGammaMuteG,     N_GM_WDATA_G) |
	                  P_Fld(wGammaMuteB,     N_GM_WDATA_B) );
#else

	bGammaOn = !RegReadFldAlign(OS_38, C_GM_BYPASS);

	// 1. if gamma not on, turn it on to enable gamma mute
	if (!bGammaOn)
	{
		vRegWriteFldAlign(OS_38, 0x0, C_GM_BYPASS);
	}

	vRegWriteFldAlign(OS_38, 0x1, C_GM_CPU);

	// 2.3 write address / read data
	vRegWriteFldAlign(OS_36, 0, C_CPU_ADDR);

	wGammaZeroPtR = RegReadFldAlign(OS_3A, R_READ);
	wGammaZeroPtG = RegReadFldAlign(OS_3B, G_READ);
	wGammaZeroPtB = RegReadFldAlign(OS_3B, B_READ);

	Printf("%d, %d, %d\n", wGammaZeroPtR, wGammaZeroPtG, wGammaZeroPtB);

	// 4. use gamma write temporal as mute
	vRegWriteFldMulti(OS_36, P_Fld(0,     C_CPU_ADDR) |
      	                     P_Fld(wGammaMuteR,     C_CPU_DATA_R) );
	vRegWriteFldMulti(OS_37, P_Fld(wGammaMuteG,     C_CPU_DATA_G) |
	                         P_Fld(wGammaMuteB,     C_CPU_DATA_B) );

#endif
}

void vDrvGammaUnmute()
{
#if !USE_POST_GAMMA    
	vRegWriteFldMulti(OS_26, P_Fld(7, GAMMA_CS) |
	                  P_Fld(0, GM_EN) |
	                  P_Fld(1, GMT_WEN) |
	                  P_Fld(1, N_GM_W_PULSE) |
	                  P_Fld(0, GM_R_ONLY) |
	                  P_Fld(1, GMTWMODE) |
	                  P_Fld(0, GM_AUTOINC) |
	                  P_Fld(0, GMT_WEN_CONTROL));

	//
	vRegWriteFldMulti(OS_22, P_Fld(0,   N_GM_WADD) |
	                  P_Fld(wGammaZeroPtR,     N_GM_WDATA_R) );
	vRegWriteFldMulti(OS_23, P_Fld(wGammaZeroPtG,     N_GM_WDATA_G) |
	                  P_Fld(wGammaZeroPtB,     N_GM_WDATA_B) );
	//
	vRegWriteFldMulti(OS_26, P_Fld(1, GM_R_ONLY) |
	                  P_Fld(1, GM_EN) |
	                  P_Fld(0, GMTWMODE) |
	                  P_Fld(0, N_GM_W_PULSE) |
	                  P_Fld(0, GMT_WEN) |
	                  P_Fld(0, GAMMA_CS));

	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWriteFldAlign(OS_26, 7, GAMMA_CS);
	//
	vRegWriteFldAlign(OS_26, bGammaOn, GM_EN);
#else

	vRegWriteFldMulti(OS_36, P_Fld(0,     C_CPU_ADDR) |
      	                     P_Fld(wGammaZeroPtR,     C_CPU_DATA_R) );
	vRegWriteFldMulti(OS_37, P_Fld(wGammaZeroPtG,     C_CPU_DATA_G) |
	                         P_Fld(wGammaZeroPtB,     C_CPU_DATA_B) );

	vRegWriteFldAlign(OS_38, 0x0, C_GM_CPU);

	vRegWriteFldAlign(OS_38, !bGammaOn, C_GM_BYPASS);

#endif
}

/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/
void vDrvSetOstgOptPos(UINT8 bOpt)
{
    vRegWriteFldAlign(OS_70, bOpt, REG_OP2_SEL);
}

void vDrvSetSWGammaFlg(UINT8 bFlagOnOff)
{
	_FLG_SW_GAMMA = bFlagOnOff;
}

UINT8 bDrvGetSWGammaFlg()
{
	return _FLG_SW_GAMMA;
}


void vDrvGammaOnOff(UINT8 bOnOff)
{
#if !USE_POST_GAMMA        
	vRegWriteFldAlign(OS_26, bOnOff, GM_EN);
	bGammaOn = bOnOff;
#else
	vRegWriteFldAlign(OS_38, !bOnOff, C_GM_BYPASS);
	bGammaOn = bOnOff;
#endif
}

UINT8 bDrvGammaReadTable(UINT8 bMode)
{
#if !USE_POST_GAMMA        
	UINT8 bValOld, bFetch;
	UINT32 index;
	UINT16 wValCur, wReadVal;
	UINT8 bOk;

	bFetch = 0;
	bOk = 1;
	bValOld = 0;
	bHiByte(wValCur) = 0;

	vRegWriteFldAlign(OS_26, 1, GM_EN);
	vRegWriteFldAlign(OS_26, 1, GM_R_ONLY);
	vRegWriteFldAlign(OS_26, 1, GMTWMODE);

	for (index = 0; index < 256; index++)
	{
        bFetch = GAMMA_256[bMode][index];

		if (bValOld > bFetch)
		{
			++bHiByte(wValCur);
		}

		bLoByte(wValCur) = bFetch;
		bValOld = bFetch;

		vRegWriteFldAlign(OS_22, index<<2, N_GM_WADD);
		wReadVal = 0;

		if (bMode == SV_RGB_R)
		{
			wReadVal = RegReadFldAlign(RO_21, GM_TABLE_R);
		}
		else if (bMode == SV_RGB_G)
		{
			wReadVal = RegReadFldAlign(RO_20, GM_TABLE_G) ;
		}
		else
		{
			wReadVal = RegReadFldAlign(RO_20, GM_TABLE_B) ;
		}

		if (wValCur != wReadVal)
		{
			bOk = 0;
			Printf("Error : Index %d Load %4x Read %4x in bMode %d\n", index, wValCur,
			       wReadVal, bMode);
		}
	}

	vRegWriteFldAlign(OS_26, 0, GM_R_ONLY);
	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWriteFldAlign(OS_26, 0, GMTWMODE);

	return bOk;

#else

	UINT8 bValOld, bFetch;
	UINT32 index;
	UINT16 wValCur, wReadVal;
	UINT8 bOk;

	bFetch = 0;
	bOk = 1;
	bValOld = 0;
	bHiByte(wValCur) = 0;

    vRegWriteFldAlign(OS_38, 1, C_GM_CPU);    	

	for (index = 0; index < 256; index++)
	{
        bFetch = GAMMA_256[bMode][index];

		if (bValOld > bFetch)
		{
			++bHiByte(wValCur);
		}

		bLoByte(wValCur) = bFetch;
		bValOld = bFetch;

    	vRegWriteFldAlign(OS_36, index+768, C_CPU_ADDR);

		wReadVal = 0;

		if (bMode == SV_RGB_R)
		{
			wReadVal = RegReadFldAlign(OS_3A, R_READ);
		}
		else if (bMode == SV_RGB_G)
		{
			wReadVal = RegReadFldAlign(OS_3B, G_READ) ;
		}
		else
		{
			wReadVal = RegReadFldAlign(OS_3B, B_READ) ;
		}

		if (wValCur != wReadVal)
		{
			bOk = 0;
			Printf("Error : Index %d Load %4x Read %4x in bMode %d\n", index, wValCur,
			       wReadVal, bMode);
		}
	}

    vRegWriteFldAlign(OS_38, 0, C_GM_CPU);    	

	return bOk;    
	
#endif //!USE_POST_GAMMA        
}

/**
 * @brief load gamma table
 * @param bMode gamma table loading mode
 * @param pbTable gamma table
 */
void vDrvSWGammaTruncate(UINT32 * dwOSDGammaRCurve, UINT32 * dwOSDGammaGCurve, UINT32 * dwOSDGammaBCurve)
{
	UINT32 i;
	for (i=0; i<256; i++)
	{
		if ((dwOSDGammaRCurve[i] & 0xFFFF) > 0x3FF)
		{
			if ((dwOSDGammaRCurve[i] >> 15) && 0x1 == 1) // minus number
			{
				dwOSDGammaRCurve[i] = 0;
			}
			else
			{
				dwOSDGammaRCurve[i] = 0x3FF;
			}
		}

		if ((dwOSDGammaGCurve[i] & 0xFFFF) > 0x3FF)
		{
			if ((dwOSDGammaGCurve[i] >> 15) && 0x1 == 1) // minus number
			{
				dwOSDGammaGCurve[i] = 0;
			}
			else
			{
				dwOSDGammaGCurve[i] = 0x3FF;
			}
		}

		if ((dwOSDGammaBCurve[i] & 0xFFFF) > 0x3FF)
		{
			if ((dwOSDGammaBCurve[i] >> 15) && 0x1 == 1) // minus number
			{
				dwOSDGammaBCurve[i] = 0;
			}
			else
			{
				dwOSDGammaBCurve[i] = 0x3FF;
			}
		}
	}
}

void vDrvSWGammaInit(UINT32 * dwOSDGammaRCurve, UINT32 * dwOSDGammaGCurve, UINT32 * dwOSDGammaBCurve)
{
	UINT32 i;

	for (i=0; i<256; i++)
	{
		dwOSDGammaRCurve[i] = i << 2;     // SW Gamma R
		dwOSDGammaGCurve[i] = i << 2;     // SW Gamma G
		dwOSDGammaBCurve[i] = i << 2;     // SW Gamma B
	}
}

void vDrvSWGammaSetGain(UINT32 * dwOSDGammaRCurve, UINT32 * dwOSDGammaGCurve, UINT32 * dwOSDGammaBCurve)
{
	UINT32 i;
	UINT16 wGainR, wGainG, wGainB;
	wGainR = wDrvGetRGain1(SV_VP_MAIN);
	wGainG = wDrvGetGGain1(SV_VP_MAIN);
	wGainB = wDrvGetBGain1(SV_VP_MAIN);

	for (i=0; i<256; i++)
	{
		if (RegReadFldAlign(OS_7A, M_RGB_GAIN_SEL) == 0x0)
		{
			// 0 ~ 2.0
			dwOSDGammaRCurve[i] = (dwOSDGammaRCurve[i] * wGainR) >> 9;     // SW Gamma R
			dwOSDGammaGCurve[i] = (dwOSDGammaGCurve[i] * wGainG) >> 9;     // SW Gamma G
			dwOSDGammaBCurve[i] = (dwOSDGammaBCurve[i] * wGainB) >> 9;     // SW Gamma B
		}
		else
		{
			// 0.5 ~ 1.5
			dwOSDGammaRCurve[i] = (dwOSDGammaRCurve[i] >> 1) + ((dwOSDGammaRCurve[i] * wGainR) >> 10);     // SW Gamma R
			dwOSDGammaGCurve[i] = (dwOSDGammaGCurve[i] >> 1) + ((dwOSDGammaGCurve[i] * wGainG) >> 10);     // SW Gamma R
			dwOSDGammaBCurve[i] = (dwOSDGammaBCurve[i] >> 1) + ((dwOSDGammaBCurve[i] * wGainB) >> 10);     // SW Gamma R
		}
	}
}

void vDrvSWGammaSetOffset1(UINT32 * dwOSDGammaRCurve, UINT32 * dwOSDGammaGCurve, UINT32 * dwOSDGammaBCurve)
{
	UINT32 i;
	UINT32 wOffsetR, wOffsetG, wOffsetB;
	wOffsetR = dwDrvGetROffset11(SV_VP_MAIN);
	wOffsetG = dwDrvGetGOffset11(SV_VP_MAIN);
	wOffsetB = dwDrvGetBOffset11(SV_VP_MAIN);

	for (i=0; i<256; i++)
	{
		dwOSDGammaRCurve[i] = (dwOSDGammaRCurve[i] + wOffsetR);     // SW Gamma R
		dwOSDGammaGCurve[i] = (dwOSDGammaGCurve[i] + wOffsetG);     // SW Gamma G
		dwOSDGammaBCurve[i] = (dwOSDGammaBCurve[i] + wOffsetB);     // SW Gamma B
	}
}

void vDrvSWGammaSetOffset2(UINT32 * dwOSDGammaRCurve, UINT32 * dwOSDGammaGCurve, UINT32 * dwOSDGammaBCurve)
{
	UINT32 i;
	UINT32 wOffsetR, wOffsetG, wOffsetB;
	wOffsetR = dwDrvGetROffset12(SV_VP_MAIN);
	wOffsetG = dwDrvGetGOffset12(SV_VP_MAIN);
	wOffsetB = dwDrvGetBOffset12(SV_VP_MAIN);

	for (i=0; i<256; i++)
	{
		dwOSDGammaRCurve[i] = (dwOSDGammaRCurve[i] + wOffsetR);     // SW Gamma R
		dwOSDGammaGCurve[i] = (dwOSDGammaGCurve[i] + wOffsetG);     // SW Gamma G
		dwOSDGammaBCurve[i] = (dwOSDGammaBCurve[i] + wOffsetB);     // SW Gamma B
	}
}

void vDrvGetOSDGammaCurve(UINT16 * bOSDGammaRCurve, UINT16 * bOSDGammaGCurve, UINT16 * bOSDGammaBCurve)
{
	UINT32 i;

	vDrvSWGammaInit(dwGammaR1024, dwGammaG1024, dwGammaB1024);         // generate a 10-bit linear curve
	vDrvSWGammaSetOffset1(dwGammaR1024, dwGammaG1024, dwGammaB1024);   // offset 1
	vDrvSWGammaSetGain(dwGammaR1024, dwGammaG1024, dwGammaB1024);      // gain 1
	vDrvSWGammaSetOffset2(dwGammaR1024, dwGammaG1024, dwGammaB1024);   // offset 2
	vDrvSWGammaTruncate(dwGammaR1024, dwGammaG1024, dwGammaB1024);     // truncation

	for (i= 0; i < 256; i++)   //12-bits index
	{
		bOSDGammaRCurve[i] = wHwGammaCurve[0][(dwGammaR1024[i]>>2)];
		bOSDGammaGCurve[i] = wHwGammaCurve[1][(dwGammaG1024[i]>>2)];
		bOSDGammaBCurve[i] = wHwGammaCurve[2][(dwGammaB1024[i]>>2)];
	}
}

void vDrvGammaSetDefaultTable(const UINT8 bTable, UINT8 * bGammaTable)
{
    UINT16 u2Index = 0;
    
    for(u2Index = 0; u2Index < GAMMA_INDEX; u2Index++)
    {
        GAMMA_256[bTable][u2Index] = bGammaTable[u2Index];
    }
}

void vDrvGammaDecodeTable()
{
	UINT8 bValOld_R, bValOld_G, bValOld_B;
	UINT8  bFetch_R, bFetch_G, bFetch_B;
	UINT16 wValCur_R, wValCur_G, wValCur_B;
	UINT16 wIndex;

	bFetch_R = 0;
	bFetch_G = 0;
	bFetch_B = 0;

	wValCur_R = 0;
	wValCur_G = 0;
	wValCur_B = 0;

	bValOld_R = 0;
	bValOld_G = 0;
	bValOld_B = 0;

	bHiByte(wValCur_R) = 0;
	bHiByte(wValCur_G) = 0;
	bHiByte(wValCur_B) = 0;

	for (wIndex = 0; wIndex < GAMMA_INDEX; wIndex++)   //12-bits index
	{
		bFetch_R = GAMMA_256[0][wIndex];
		bFetch_G = GAMMA_256[1][wIndex];
		bFetch_B = GAMMA_256[2][wIndex];

		// decompress R value
		if (bValOld_R > bFetch_R)
		{
			++bHiByte(wValCur_R);
		}

		bLoByte(wValCur_R) = bFetch_R;
		bValOld_R= bFetch_R;

		// decompress G value
		if (bValOld_G > bFetch_G)
		{
			++bHiByte(wValCur_G);
		}

		bLoByte(wValCur_G) = bFetch_G;
		bValOld_G= bFetch_G;

		// decompress B value
		if (bValOld_B > bFetch_B)
		{
			++bHiByte(wValCur_B);
		}

		bLoByte(wValCur_B) = bFetch_B;
		bValOld_B= bFetch_B;
		
		u2GammaDecodedTable[0][wIndex] = wValCur_R;
		u2GammaDecodedTable[1][wIndex] = wValCur_G;
		u2GammaDecodedTable[2][wIndex] = wValCur_B;		
	}
}

void vDrvGammaWriteTable()
{
#if !USE_POST_GAMMA    
	UINT16 wIndex;    
	vRegWriteFldMulti(OS_26, P_Fld(7, GAMMA_CS) |
	                  P_Fld(0, GM_EN) |
	                  P_Fld(1, GMT_WEN) |
	                  P_Fld(1, N_GM_W_PULSE) |
	                  P_Fld(0, GM_R_ONLY) |
	                  P_Fld(1, GMTWMODE) |
	                  P_Fld(0, GM_AUTOINC) |
	                  P_Fld(0, GMT_WEN_CONTROL));

	for (wIndex = 0; wIndex < GAMMA_INDEX; wIndex++)   //12-bits index
	{
        if(wIndex != 256)
        {
            if(wIndex > 256)    // fix klockwork
            {
                wIndex = 256;
            }
            
		    // write data into gamma
	    	vRegWrite4B(OS_22, ((UINT32)u2GammaDecodedTable[0][wIndex] << 16) | (wIndex<<2));
    		vRegWrite4B(OS_23, ((UINT32)u2GammaDecodedTable[1][wIndex] << 16) | (u2GammaDecodedTable[2][wIndex]));
		    wHwGammaCurve[0][wIndex] = (u2GammaDecodedTable[0][wIndex] ); // 12 bit
	    	wHwGammaCurve[1][wIndex] = (u2GammaDecodedTable[1][wIndex] ); // 12 bit
    		wHwGammaCurve[2][wIndex] = (u2GammaDecodedTable[2][wIndex] ); // 12 bit
        }
        else
        {
            // top-most gamma data
        	vRegWriteFldAlign(OS_24,       MIN(0xFFF, u2GammaDecodedTable[0][wIndex]), REG_OS_R_SRAM_1024);
	        vRegWriteFldMulti(OS_25, P_Fld(MIN(0xFFF, u2GammaDecodedTable[1][wIndex]), REG_OS_G_SRAM_1024) |
    	                             P_Fld(MIN(0xFFF, u2GammaDecodedTable[2][wIndex]), REG_OS_B_SRAM_1024) );		
        }
    }

	vRegWriteFldMulti(OS_26, P_Fld(1, GM_R_ONLY) |
	                  P_Fld(1, GM_EN) |
	                  P_Fld(0, GMTWMODE) |
	                  P_Fld(0, N_GM_W_PULSE) |
	                  P_Fld(0, GMT_WEN) |
	                  P_Fld(0, GAMMA_CS));

	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWriteFldAlign(OS_26, 7, GAMMA_CS);	
	
#else   // use gamma after local dimming

    UINT16 u2GammaIdx = 0;

    vRegWriteFldAlign(OS_38, 1, C_GM_CPU);    

    // gamma table address, [255:0] for R, [511:256] for G, [767:512] for B, [1023:768] for RGB all
    for (u2GammaIdx = 0; u2GammaIdx < 256; u2GammaIdx++)
    {
    	vRegWriteFldMulti(OS_36, 
    	                P_Fld(u2GammaIdx+768, C_CPU_ADDR) |
	                    P_Fld(u2GammaDecodedTable[0][u2GammaIdx], C_CPU_DATA_R));

    	vRegWriteFldMulti(OS_37, 
    	                P_Fld(u2GammaDecodedTable[1][u2GammaIdx], C_CPU_DATA_G) |
	                    P_Fld(u2GammaDecodedTable[2][u2GammaIdx], C_CPU_DATA_B));        
    }

    vRegWriteFldAlign(OS_38, MIN(0xFFF, u2GammaDecodedTable[0][u2GammaIdx]), C_GM_256_R);    
    vRegWriteFldAlign(OS_39, MIN(0xFFF, u2GammaDecodedTable[1][u2GammaIdx]), C_GM_256_G);    
    vRegWriteFldAlign(OS_39, MIN(0xFFF, u2GammaDecodedTable[2][u2GammaIdx]), C_GM_256_B);    

    vRegWriteFldAlign(OS_38, 0, C_GM_CPU);            
    
#endif //!USE_POST_GAMMA    
}

BOOL bDrvGammaVerifyTable()
{
#if !USE_POST_GAMMA    
	UINT8 bRet = SV_TRUE;    
	UINT32 wIndex;
	UINT16 wReadValR, wReadValG, wReadValB;
	UINT32 dwReadValGB;

	vRegWriteFldAlign(OS_26, 1, GM_R_ONLY);
	vRegWriteFldAlign(OS_26, 1, GMTWMODE);
	vRegWriteFldAlign(OS_26, 7, GAMMA_CS);
	vRegWriteFldAlign(OS_26, 0, GM_AUTOINC);
	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWrite4B(OS_22, 0);

	for (wIndex = 0; wIndex < 256; wIndex++)
	{
		vRegWriteFldAlign(OS_22, wIndex<<2, N_GM_WADD);

        vUtDelay1ms(1);

		wReadValR = 0;
		wReadValG = 0;
		wReadValB = 0;	
		dwReadValGB = 0;

		wReadValR = RegReadFldAlign(RO_21, GM_TABLE_R);
		dwReadValGB = u4RegRead4B(RO_20);
		wReadValG = (dwReadValGB >> 12) & 0xFFF;
		wReadValB = dwReadValGB & 0xFFF;

		if (wReadValR != u2GammaDecodedTable[0][wIndex])
		{
			bRet = SV_FALSE;
			Printf("[R] Index %d Load %4x Read %4x\n", wIndex, u2GammaDecodedTable[0][wIndex],
			       wReadValR);
		}
		if (wReadValG != u2GammaDecodedTable[1][wIndex])
		{
			bRet = SV_FALSE;
			Printf("[G] Index %d Load %4x Read %4x\n", wIndex, u2GammaDecodedTable[1][wIndex],
			       wReadValG);
		}
		if (wReadValB != u2GammaDecodedTable[2][wIndex])
		{
			bRet = SV_FALSE;
			Printf("[B] Index %d Load %4x Read %4x\n", wIndex, u2GammaDecodedTable[2][wIndex],
			       wReadValB);
		}		
	}

	u2GammaDecodedTable[0][256] = RegReadFldAlign(OS_24, REG_OS_R_SRAM_1024);
	u2GammaDecodedTable[1][256] = RegReadFldAlign(OS_25, REG_OS_G_SRAM_1024);
	u2GammaDecodedTable[2][256] = RegReadFldAlign(OS_25, REG_OS_B_SRAM_1024);

	vRegWriteFldAlign(OS_26, 0, GM_R_ONLY);
	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWriteFldAlign(OS_26, 0, GMTWMODE);

	return bRet;
	
#else

	UINT8 bRet = SV_TRUE;    
	UINT32 wIndex;
	UINT16 wReadValR, wReadValG, wReadValB;
	UINT32 dwReadValGB;

    vRegWriteFldAlign(OS_38, 1, C_GM_CPU); 

	for (wIndex = 0; wIndex < 256; wIndex++)
	{
    	vRegWriteFldAlign(OS_36, wIndex+768, C_CPU_ADDR);

        vUtDelay1ms(1);

		wReadValR = 0;
		wReadValG = 0;
		wReadValB = 0;	

		wReadValR = RegReadFldAlign(OS_3A, R_READ);
		wReadValG = RegReadFldAlign(OS_3B, G_READ);
		wReadValB = RegReadFldAlign(OS_3B, B_READ);

		if (wReadValR != u2GammaDecodedTable[0][wIndex])
		{
			bRet = SV_FALSE;
			Printf("[R] Index %d Load %4x Read %4x\n", wIndex, u2GammaDecodedTable[0][wIndex],
			       wReadValR);
		}
		if (wReadValG != u2GammaDecodedTable[1][wIndex])
		{
			bRet = SV_FALSE;
			Printf("[G] Index %d Load %4x Read %4x\n", wIndex, u2GammaDecodedTable[1][wIndex],
			       wReadValG);
		}
		if (wReadValB != u2GammaDecodedTable[2][wIndex])
		{
			bRet = SV_FALSE;
			Printf("[B] Index %d Load %4x Read %4x\n", wIndex, u2GammaDecodedTable[2][wIndex],
			       wReadValB);
		}		
	}

	u2GammaDecodedTable[0][256] = RegReadFldAlign(OS_38, C_GM_256_R);
	u2GammaDecodedTable[1][256] = RegReadFldAlign(OS_39, C_GM_256_G);
	u2GammaDecodedTable[2][256] = RegReadFldAlign(OS_39, C_GM_256_B);

    vRegWriteFldAlign(OS_38, 0, C_GM_CPU); 

	return bRet;

#endif
}


/**
* @brief Gamma Main Loop
* @param void
*/
void vDrvSetGammaRemap(UINT8 bForceLoadGamma)
{
	vIO32WriteFldAlign(ADAPTIVE_RGB_GAMMA_REG,wReadQualityTable(QUALITY_GAMMA_CURVE_SEL),ADAPTIVE_RGB_GAMMA_SEL);
	vIO32WriteFldAlign(ADAPTIVE_RGB_GAMMA_REG,bForceLoadGamma,ADAPTIVE_RGB_GAMMA_FORCE_LOAD);
}

void vDrvSpline(UINT32 u4Count, INT32 *xx, INT32 *yy, INT32 *y)
{
    UINT32  i, j, jk;
    static INT32 h[64], v[64], d[64], l[64], z[64], u[64];
    static INT32 aa[64], bb[64], cc[64], dd[64];
    static INT32 a[64], b[64], c[64];
    INT32	x;
    
    for (i = 0; i <= u4Count; i++)
    {
        yy[i] = (yy[i]<<14);
    }
    
    for (i = 0; i < u4Count; i++)
    {
        h[i] = xx[i+1] - xx[i] ;
    }
    
    for (i = 1; i < u4Count; i++) 
    {
        if (h[i]*h[i-1] != 0)
        {
            v[i] = 6*(yy[i+1]-yy[i])/h[i]-6*(yy[i]-yy[i-1])/h[i-1];
        }

        a[i] = 2 * (h[i-1] + h[i]);
        b[i] = h[i-1];
        c[i] = h[i];
    }

    d[1] = a[1];    
    for (i = 2; i < u4Count; i++) 
    {
        if (d[i-1] != 0)
        {
            l[i] = (b[i]<<8)/d[i-1];
        }
        d[i] = a[i] - ((l[i] * c[i-1]+128)>>8);
    }

    z[1] = v[1];
    for (i = 2; i < u4Count; i++)
    {
        z[i] = v[i] - ((l[i] * z[i-1]+128)>>8);
    }
    
	u[0]  = 0;
	u[u4Count] = 0;
    if (d[u4Count-1] != 0)
    {
        u[u4Count-1] = z[u4Count-1]/d[u4Count-1] ;
    }
    
    for (i = (u4Count-2); i >= 1 ; i--) 
    { 
        if (d[i] != 0)
        {
            u[i] = (z[i] - c[i] * u[i+1]) / d[i] ;
        }
    }

    for (i = 0; i < u4Count; i++)
    {
        bb[i] = (u[i] / 2);
        dd[i] = yy[i] ;
        if ((xx[i + 1] - xx[i]) != 0) 
        {
            aa[i] = ((u[i+1]-u[i])/(xx[i+1]-xx[i]) / 6);
            cc[i] = ((yy[i+1]-yy[i])/(xx[i+1]-xx[i])-(xx[i+1]-xx[i])*(2*u[i]+u[i+1])/6);
        }
    }
    dd[u4Count] = yy[u4Count];
    
    for (i = 0; i < 256; i++)
    {
        x = i;
        jk = 0 ;

        for (j = 0; j <= (u4Count-1); j++) 
        {
            if ( (x >= xx[j]) && (x < xx[j+1]))
            {
                jk = j ;
            }
        }

        if (x >= xx[u4Count])
        {
            jk = u4Count;
        }

        y[i] = ((aa[jk] * ((x - xx[jk])*(x - xx[jk])*(x - xx[jk]))
               + bb[jk] * ((x - xx[jk])*(x - xx[jk]))
               + cc[jk] * (x - xx[jk]) + dd[jk] + 8192)>>14);
    }
}

void vDrvGammaInterpolation(UINT32 u4Count, INT32* iCtlPos, INT32* iCtlValue, INT32* OutputLut)
{
    UINT32 i;

    if (u4Count > 64)
    {
        Printf("Too many Control Point : %d > (Max 64 points)\n", u4Count);  
        return;
    }

    if (OutputLut == NULL)
    {
        Printf("Null output array\n");  
        return;
    }

#if 0   
    Printf("\n Control Point %d\n", u4Count);  
    for (i=0; i<u4Count; i++)
    {
        Printf(" %d\t %d \n", iCtlPos[i], iCtlValue[i]);  
    }
#endif
  
    vDrvSpline(u4Count-1, iCtlPos, iCtlValue, OutputLut);
          
    OutputLut[256] = (OutputLut[255]<<1) - OutputLut[254];

    for (i=0; i<=256; i++)
    {      
        OutputLut[i] = (OutputLut[i] < 0) ? 0 : (OutputLut[i]>4095) ? 4095 : OutputLut[i]; 
        
        if (i>0 && (OutputLut[i] < OutputLut[i-1]))
        {
            Printf("Value reverse !! Index : %d => %d < %d \n", i, OutputLut[i], OutputLut[i-1]);  
        }
    }                   

#if 0    
    Printf("\n Gamma Table \n", OutputLut[i]);  
    for (i=0; i<=256; i++)
    {
        Printf(" %d \n", OutputLut[i]);  
    }
#endif    
}

/*****************************************************************************************/
/***************************************  xvYCC   ****************************************/
/*****************************************************************************************/
void vDrvxvYCCOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(OS_60, !bOnOff, REG_BYPASS_XVYCC);
}

void vDrvxvYCCGammaOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(OS_60, !bOnOff, REG_BYPASS_G);
}

void vDrvxvYCCAntiCGammaOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(OS_60, !bOnOff, REG_BYPASS_AG);
}

void vDrvxvYCC3x3OnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(OS_60, !bOnOff, REG_3X3_BYPASS);
}

void vDrvxvYCCSetPath(UINT8 bPath)
{
	vRegWriteFldAlign(OS_60, (bPath == SV_VP_MAIN) ? 0 : 1, REG_XVYCC_M_P_SW);
}

void vDrvxvYCCLoad3x3Matrix(UINT16* pwTable)
{
	vRegWriteFldAlign(OS_65, pwTable[0], REG_XV_MATRIX00);
	vRegWriteFldAlign(OS_66, pwTable[1], REG_XV_MATRIX01);
	vRegWriteFldAlign(OS_66, pwTable[2], REG_XV_MATRIX02);
	vRegWriteFldAlign(OS_67, pwTable[3], REG_XV_MATRIX10);
	vRegWriteFldAlign(OS_67, pwTable[4], REG_XV_MATRIX11);
	vRegWriteFldAlign(OS_68, pwTable[5], REG_XV_MATRIX12);
	vRegWriteFldAlign(OS_68, pwTable[6], REG_XV_MATRIX20);
	vRegWriteFldAlign(OS_69, pwTable[7], REG_XV_MATRIX21);
	vRegWriteFldAlign(OS_69, pwTable[8], REG_XV_MATRIX22);
}

void vDrvxvYCCSetOffset1(UINT16* u2RgbOffset)
{
	vRegWriteFldMulti(OS_61, P_Fld(u2RgbOffset[0], REG_XV_OFFSET1_R) |
	                  P_Fld(u2RgbOffset[1], REG_XV_OFFSET1_G));
	vRegWriteFldMulti(OS_62, P_Fld(u2RgbOffset[2], REG_XV_OFFSET1_B));
}

void vDrvxvYCCSetGain(UINT16* u2RgbGain)
{
	vRegWriteFld(OS_62, u2RgbGain[0], REG_XV_GAIN1_R);
	vRegWriteFldMulti(OS_63, P_Fld(u2RgbGain[1], REG_XV_GAIN1_G) |
	                  P_Fld(u2RgbGain[2], REG_XV_GAIN1_B));
}

void vDrvxvYCCSetOffset2(UINT16* u2RgbOffset)
{
	vRegWriteFldMulti(OS_64, P_Fld(u2RgbOffset[0], REG_XV_OFFSET2_R) |
	                  P_Fld(u2RgbOffset[1], REG_XV_OFFSET2_G));
	vRegWriteFldAlign(OS_65, u2RgbOffset[2], REG_XV_OFFSET2_B);
}

BOOL fgDrvGetxvYCCOnOff(void)
{
	return (!RegReadFldAlign(OS_60, REG_BYPASS_XVYCC));
}

void vDrvxvYCCSetClip(UINT8 bOnOff)
{
    vRegWriteFldAlign(OS_60, bOnOff, REG_XVYCC_CLIP);
}

/*****************************************************************************************/
/************************************ xvYCC Inner PatGen ***************************************/
/*****************************************************************************************/
UINT8 bxvYCCStatus=SV_OFF;
UINT8 bxvYCCInnerPatGenStatus = SV_OFF;

void vDrvSetxvYCCPatGenOnOff(UINT8 bOnOff)
{
    if(bOnOff == SV_ON)
    {
  //      bxvYCCStatus = RegReadFldAlign(OS_60, REG_BYPASS_XVYCC);
        vRegWriteFldAlign(OS_60, 0x0, REG_BYPASS_XVYCC);    
        vRegWriteFldAlign(OS_60, 0xA, REG_XVYCC_PTGEN_SEL);            
    }
    else
    {
        vRegWriteFldAlign(OS_60, !bxvYCCStatus, REG_BYPASS_XVYCC);   
        vRegWriteFldAlign(OS_60, 0x0, REG_XVYCC_PTGEN_SEL);    
    }
    bxvYCCInnerPatGenStatus = bOnOff;
}

// Express RGB Mute in 10 bit resolution
void vDrvSetxvYCCPatGenRGB(UINT16 wMuteR, UINT16 wMuteG, UINT16 wMuteB)
{
    vRegWriteFldAlign(OS_6F, wMuteB, REG_XVYCC_PTGEN_B);
    vRegWriteFldAlign(OS_6E, wMuteG, REG_XVYCC_PTGEN_G);
    vRegWriteFldAlign(OS_6E, wMuteR, REG_XVYCC_PTGEN_R);
}

/*****************************************************************************************/
/***************************************  Ambilight **************************************/
/*****************************************************************************************/
void vDrvAmbilightDemoMode(UINT8 bOnOff)
{
	vRegWriteFldAlign(AMBI_CFG, bOnOff, DEMO_MODE);
}

void vDrvAmbilightSetWindow(UINT8 bWindowNum,
                            const structAmbilightWindow * pAmbiWindow)
{
	vRegWriteFldAlign(W0_X_RANGE + bWindowNum * AMBILIGHT_OFFSET1,
	                  pAmbiWindow->wX_Start, W0_X_START);
	vRegWriteFldAlign(W0_X_RANGE + bWindowNum * AMBILIGHT_OFFSET1,
	                  pAmbiWindow->wX_End, W0_X_END);
	vRegWriteFldAlign(W0_Y_RANGE + bWindowNum * AMBILIGHT_OFFSET1,
	                  pAmbiWindow->wY_Start, W0_Y_START);
	vRegWriteFldAlign(W0_Y_RANGE + bWindowNum * AMBILIGHT_OFFSET1,
	                  pAmbiWindow->wY_End, W0_Y_END);
	vRegWriteFldAlign(W0_ATTEN + bWindowNum * AMBILIGHT_OFFSET1,
	                  pAmbiWindow->bAttenuate, W0_ATTEN_VALUE);
}

UINT16 wDrvAmbilightGetRGB(UINT8 bWindowNum, UINT8 bRGBSel)
{
	UINT16 wRet = 0;

	switch (bRGBSel)
	{
	case AmbilightRSel:
		wRet =
		    RegReadFldAlign(W0_RESULT_1 + bWindowNum * AMBILIGHT_OFFSET2,
		                    W0_R);
		break;
	case AmbilightGSel:
		wRet =
		    RegReadFldAlign(W0_RESULT_1 + bWindowNum * AMBILIGHT_OFFSET2,
		                    W0_G);
		break;
	case AmbilightBSel:
		wRet =
		    RegReadFldAlign(W0_RESULT_2 + bWindowNum * AMBILIGHT_OFFSET2,
		                    W0_B);
		break;
	default:
		break;
	}

	return wRet;
}

#ifdef GAMMA_DEBUG
void vDrvGammaRead()
{
	UINT32 j;
	UINT16 wGammaTable[3][1024];

	vRegWriteFldAlign(OS_26, 1, GM_EN);
	vRegWriteFldAlign(OS_26, 1, GM_R_ONLY);
	vRegWriteFldAlign(OS_26, 1, GMTWMODE);
	vRegWriteFldAlign(OS_26, 7, GAMMA_CS);
	vRegWriteFldAlign(OS_26, 0, N_GM_W_PULSE);
	vRegWriteFldAlign(OS_26, 0, GM_AUTOINC);

	for (j=0; j < 1024; j++)
	{
		vRegWriteFldAlign(OS_22, j, N_GM_WADD);
		wGammaTable[0][j] = RegReadFldAlign(RO_21, GM_TABLE_R);
		wGammaTable[1][j] = RegReadFldAlign(RO_20, GM_TABLE_G);
		wGammaTable[2][j] = RegReadFldAlign(RO_20, GM_TABLE_B);
	}

	vRegWriteFldAlign(OS_26, 0, GM_R_ONLY);
	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWriteFldAlign(OS_26, 0, GMTWMODE);
	vRegWriteFldAlign(OS_26, 7, GAMMA_CS);

}
#endif //GAMMA_DEBUG


#define GAMMA_PROFILING 0


#ifdef GAMMA_DEBUG
void vDrvGammaInit()
{
	UINT32 j;
#if GAMMA_PROFILING
	HAL_RAW_TIME_T rtime_front, rtime_end;
	HAL_RAW_TIME_T rest_time;
	HAL_TIME_T est_time;
	HAL_GetRawTime(&rtime_front);
#endif

	vRegWriteFldMulti(OS_26, P_Fld(7, GAMMA_CS) |
	                  P_Fld(0, GM_EN) |
	                  P_Fld(1, GMT_WEN) |
	                  P_Fld(1, N_GM_W_PULSE) |
	                  P_Fld(0, GM_R_ONLY) |
	                  P_Fld(1, GMTWMODE) |
	                  P_Fld(0, GM_AUTOINC) |
	                  P_Fld(0, GMT_WEN_CONTROL));


	for (j=0; j < 1023; j+=4)
	{
		vRegWriteFldMulti(OS_22, P_Fld(j, N_GM_WADD) |
		                  P_Fld(j, N_GM_WDATA_R) );
		vRegWriteFldMulti(OS_23, P_Fld(j, N_GM_WDATA_G) |
		                  P_Fld(j, N_GM_WDATA_B) );
	}

	vRegWriteFldAlign(OS_24, 0xFFF, REG_OS_R_SRAM_1024);
	vRegWriteFldMulti(OS_25, P_Fld(0xFFF, REG_OS_G_SRAM_1024) |
	                  P_Fld(0xFFF, REG_OS_B_SRAM_1024) );

	vRegWriteFldMulti(OS_26, P_Fld(1, GM_R_ONLY) |
	                  P_Fld(1, GM_EN) |
	                  P_Fld(0, GMTWMODE) |
	                  P_Fld(0, N_GM_W_PULSE) |
	                  P_Fld(0, GMT_WEN) |
	                  P_Fld(0, GAMMA_CS));

	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWriteFldAlign(OS_26, 7, GAMMA_CS);

#if GAMMA_PROFILING
	HAL_GetRawTime(&rtime_end);
	HAL_GetDeltaRawTime(&rest_time, &rtime_front, &rtime_end);
	HAL_RawToTime(&rest_time, &est_time);
//	Printf("Gamma Timing Profile %d\n", est_time.u4Micros);
#endif
}
#endif //GAMMA_DEBUG


/*****************************************************************************************/
/*********************************** GAIN / OFFSET ***************************************/
/*****************************************************************************************/
void vDrvSetRGain1(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OS_7A, wValue, R_GAIN_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        vRegWriteFldAlign(OS_7B, wValue, R_GAIN_PIP);
    }
    else
    {
        LOG(0, "vDrvSetRGain1 Path Error!");
        ASSERT(0);
    }
}

void vDrvSetGGain1(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OS_7A, wValue, G_GAIN_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        vRegWriteFldAlign(OS_7B, wValue, G_GAIN_PIP);
    }
    else
    {
        LOG(0, "vDrvSetGGain1 Path Error!");
        ASSERT(0);
    }
}

void vDrvSetBGain1(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OS_7A, wValue, B_GAIN_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        vRegWriteFldAlign(OS_7B, wValue, B_GAIN_PIP);
    }
    else
    {
        LOG(0, "vDrvSetBGain1 Path Error!");
        ASSERT(0);
    }
}

void vDrvSetROffset11(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OS_7C, wValue, R_OFST1_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        vRegWriteFldAlign(OS_7E, wValue, R_OFST1_PIP);
    }
    else
    {
        LOG(0, "vDrvSetROffset11 Path Error!");
        ASSERT(0);
    }
}

void vDrvSetGOffset11(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OS_7C, wValue, G_OFST1_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        vRegWriteFldAlign(OS_7E, wValue, G_OFST1_PIP);
    }
    else
    {
        LOG(0, "vDrvSetGOffset11 Path Error!");
        ASSERT(0);
    }
}

void vDrvSetBOffset11(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OS_7C, wValue, B_OFST1_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        vRegWriteFldAlign(OS_7E, wValue, B_OFST1_PIP);
    }
    else
    {
        LOG(0, "vDrvSetBOffset11 Path Error!");
        ASSERT(0);
    }
}

void vDrvSetROffset12(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OS_7D, wValue, R_OFST2_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        vRegWriteFldAlign(OS_7F, wValue, R_OFST2_PIP);
    }
    else
    {
        LOG(0, "vDrvSetROffset12 Path Error!");
        ASSERT(0);
    }
}

void vDrvSetGOffset12(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OS_7D, wValue, G_OFST2_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        vRegWriteFldAlign(OS_7F, wValue, G_OFST2_PIP);
    }
    else
    {
        LOG(0, "vDrvSetROffset12 Path Error!");
        ASSERT(0);
    }
}

void vDrvSetBOffset12(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OS_7D, wValue, B_OFST2_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        vRegWriteFldAlign(OS_7F, wValue, B_OFST2_PIP);
    }
    else
    {
        LOG(0, "vDrvSetBOffset12 Path Error!");
        ASSERT(0);
    }
}
UINT16 wDrvGetRGain1(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = RegReadFldAlign(OS_7A, R_GAIN_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = RegReadFldAlign(OS_7B, R_GAIN_PIP);
    }
    else
    {
        LOG(0, "wDrvGetRGain1 Path Error!");
        ASSERT(0);
    }

    return dwReadOutVal;
}

UINT16 wDrvGetGGain1(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = RegReadFldAlign(OS_7A, G_GAIN_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = RegReadFldAlign(OS_7B, G_GAIN_PIP);
    }
    else
    {
        LOG(0, "wDrvGetGGain1 Path Error!");
        ASSERT(0);
    }

    return dwReadOutVal;
}

UINT16 wDrvGetBGain1(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = RegReadFldAlign(OS_7A, B_GAIN_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = RegReadFldAlign(OS_7B, B_GAIN_PIP);
    }
    else
    {
        LOG(0, "wDrvGetBGain1 Path Error!");
        ASSERT(0);
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetROffset11(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = RegReadFldAlign(OS_7C, R_OFST1_MAIN) ;
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = RegReadFldAlign(OS_7E, R_OFST1_PIP) ;
    }
    else
    {
        LOG(0, "dwDrvGetROffset11 Path Error!");
        ASSERT(0);
    }

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetGOffset11(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = RegReadFldAlign(OS_7C, G_OFST1_MAIN) ;
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = RegReadFldAlign(OS_7E, G_OFST1_PIP) ;
    }
    else
    {
        LOG(0, "dwDrvGetROffset11 Path Error!");
        ASSERT(0);
    }

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetBOffset11(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = RegReadFldAlign(OS_7C, B_OFST1_MAIN) ;
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = RegReadFldAlign(OS_7E, B_OFST1_PIP) ;
    }
    else
    {
        LOG(0, "dwDrvGetBOffset11 Path Error!");
        ASSERT(0);
    }

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetROffset12(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = RegReadFldAlign(OS_7D, R_OFST2_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = RegReadFldAlign(OS_7F, R_OFST2_PIP);
    }
    else
    {
        LOG(0, "dwDrvGetROffset12 Path Error!");
        ASSERT(0);
    }

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetGOffset12(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = RegReadFldAlign(OS_7D, G_OFST2_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = RegReadFldAlign(OS_7F, G_OFST2_PIP);
    }
    else
    {
        LOG(0, "dwDrvGetGOffset12 Path Error!");
        ASSERT(0);
    }

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}
UINT32 dwDrvGetBOffset12(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = RegReadFldAlign(OS_7D, B_OFST2_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = RegReadFldAlign(OS_7F, B_OFST2_PIP);
    }
    else
    {
        LOG(0, "dwDrvGetBOffset12 Path Error!");
        ASSERT(0);
    }

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

/*****************************************************************************************/
/************************************ BLUE STRETCH ***************************************/
/*****************************************************************************************/
void vDrvSetBlueStretchOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(BS_CFG, bOnOff, BS_ENABLE);
}

void vDrvSetBlueStretchParam(BSParam bsParam)
{
	vRegWriteFldAlign(BS_GAIN, bsParam.bRGain, BS_GAIN_R);
	vRegWriteFldAlign(BS_GAIN, bsParam.bGGain, BS_GAIN_G);
	vRegWriteFldAlign(BS_GAIN, bsParam.bBGain, BS_GAIN_B);
	vRegWriteFldAlign(BS_CFG, bsParam.bLumaThr, BS_LUMA_THD);
	vRegWriteFldAlign(BS_CFG, bsParam.bChromaThr, BS_SAT_THD);
}

BOOL fgDrvGetBlueStretchOnOff(void)
{
	return (RegReadFldAlign(BS_CFG, BS_ENABLE));
}


/**************************************************************************************/
/********************************* OPC              ***********************************/
/**************************************************************************************/

void vHalVideoOPCResultUpdate()
{
    // write opc curve
	vRegWriteFldMulti(DBL_CFG1, 
	                    P_Fld(IO32ReadFldAlign(OPC_0B, OPC_SW_P1), DBL_P1) |
	                    P_Fld(IO32ReadFldAlign(OPC_0B, OPC_SW_P2), DBL_P2) |
	                    P_Fld(IO32ReadFldAlign(OPC_0B, OPC_SW_G1), DBL_G1) |
	                    P_Fld(IO32ReadFldAlign(OPC_0B, OPC_SW_G2), DBL_G2));    
	vRegWriteFldAlign(DBL_CFG2, 
	                          IO32ReadFldAlign(OPC_0C, OPC_SW_G_SLOPE), DBL_G_SLOPE);


    // write backlight
	if (PANEL_BRIGHT_SETTING == PWM_HIGH_PANEL_BRIGHT)
  	{
       	vApiSetPanelBright(IO32ReadFldAlign(OPC_0C, OPC_SW_PWM));
	}
	else
	{
       	vApiSetPanelBright(255-IO32ReadFldAlign(OPC_0C, OPC_SW_PWM));
	}    
}


