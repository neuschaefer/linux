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
 * $Author: kai.wang $
 * $Date: 2012/06/28 $
 * $RCSfile: aud_cfg.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_pll.c
 *  Brief of file aud_pll.c.
 *  Details of file aud_pll.c (optional).
 */

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
// KERNEL
LINT_EXT_HEADER_BEGIN

#include "x_util.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "x_os.h"
#include "x_ckgen.h"

#include "aud_debug.h"
#include "aud_if.h"
#include "aud_drv.h"
#include "aud_pll.h"

#include "hw_vdoin.h"
#include "hw_ckgen.h"
#include "aud_hw.h"
 
LINT_EXT_HEADER_END

//#define SRC_SELF_TEST
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define K2_VALID_VALUE_MIN      2
#define K2_VALID_VALUE_MAX      48

#define APLL_K2_MASK            (0x3f << 24)
#define APLL_K1_MASK            ((UINT32)1 << 31)

#define HDMI_CLK_TOGGLE_TIMES  10

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)) //CC_AUD_PLL_HALF
#define APLL_FS_48K     (147456000)
#define APLL_FS_44K     (135475200)
#else
#define APLL_FS_48K     (294912000)
#define APLL_FS_44K     (270950400)
#endif

#define RETRY_CNT 3

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro 
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern MCLK_FREQUENCY_T AUD_GetMclkFs(UINT8 u1ApllId);
extern void bHDMIAudioSigmaDeltaSetting(UINT32,UINT8,UINT8);

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
/*static*/ void _AudPllInitial(UINT8 u1ApllId, SAMPLE_FREQ_T eFs)
{
    UINT32 u4Tmp;
    UINT8 u1Loop = 0;

    if(u1ApllId == APLL_ID_MAIN)
    {
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        //HADDS1: 294.912
        //HADDS1 setting (analog)
        //REG(RG_HADDS1_BIAS_PWD)=0b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 0, FLD_RG_HADDS1_BIAS_PWD);
        //REG(RG_HADDS1_PI_C[2:0])=111b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x7, FLD_RG_HADDS1_PI_C);
        //REG(RG_HADDS1_PRE_DIV2)=0b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0, FLD_RG_HADDS1_PRE_DIV2);
        //REG(RG_HADDS1_HF)=0b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 0, FLD_RG_HADDS1_HF);
        #if 0 //CC_AUD_PLL_HALF
        //REG(RG_HADDS1_CKSEL[1:0])=01b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 1, FLD_RG_HADDS1_CKSEL);
        #else
        //REG(RG_HADDS1_CKSEL[1:0])=00b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 0, FLD_RG_HADDS1_CKSEL);
        #endif
        //REG(RG_HADDS1_MONEN)=1b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 1, FLD_RG_HADDS1_MONEN);
    #endif 

        do
        {
        #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
            //HADDS1: 294.912
            //HADDS1 setting (digital) 

            //Reset Again
            //REG(RG_HADDS1_PWDB)=0b
            AUD_WRITE32(REG_HADDS1_CFG1, 0x0);
            //vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 0, FLD_RG_HADDS1_PWDB);
            vIO32WriteFldAlign(CKGEN_HADDS1_CKCFG, 0x0, FLD_HADDS1_ALL);
            x_thread_delay(1);
            //Reset End 

            //REG(0xf000D340) = 0x1
            vIO32WriteFldAlign(CKGEN_HADDS1_CKCFG, 0x1, FLD_HADDS1_ALL); 
         
            //REG(0xf00053D4) = 0xc2328000 HADDS1: 294.912, 0xc4650000  HADDS1: 147.456
            AUD_WRITE32(REG_HADDS1_CFG0, 0xc2328000);

            //REG(RG_HADDS1_PWDB)=1b
            //vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 1, FLD_RG_HADDS1_PWDB); 
            //REG(0xf00053D8) = 0x1
            AUD_WRITE32(REG_HADDS1_CFG1, 0x1);
        #else
            //HAPLL1: 294.912
            #if defined(CC_MT5395)
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG20, 1, FLD_RG_PLLGP_BAG_EN);
            u4Tmp = IO32ReadFldAlign(CKGEN_AFEPLLCFG20, FLD_RG_PLLGP_TOP) | 0x30;
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG20, u4Tmp, FLD_RG_PLLGP_TOP);
            #else // CC_MT5365
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG18, 1, FLD_RG_PLLGP_BAG_EN);
            u4Tmp = IO32ReadFldAlign(CKGEN_AFEPLLCFG18, FLD_RG_PLLGP_TOP) | 0x30;
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG18, u4Tmp, FLD_RG_PLLGP_TOP);
            #endif
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG10, 1, FLD_RG_HA1_CMDIV1); 
            //vIO32WriteFldAlign(CKGEN_AFEPLLCFG10, 7, FLD_RG_HA1_CP_P); 
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG10, 1, FLD_RG_HA1_FB_POSTDIVSEL);
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG10, 0, FLD_RG_HA1_PWD); 
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG11, 0x2, FLD_RG_HA1_FB_POSTDIV);
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG12, 0x539C000, FLD_RG_HA1_PCW);
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG12, 0, FLD_RG_HA1_PCW_NCPO_SYNC);
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG12, 0, FLD_RG_HA1_FIFO_START_MAN);
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG12, 1, FLD_RG_HA1_PCW_NCPO_SYNC);
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG12, 1, FLD_RG_HA1_FIFO_START_MAN);
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG10, 1, FLD_RG_HA1_RSTB);
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG11, 1, FLD_RG_HA1_MONCLKEN);
            //u4Tmp = IO32ReadFldAlign(CKGEN_AFEPLLCFG11, FLD_RG_HA1_PCW) | 0x0080000;
            //vIO32WriteFldAlign(CKGEN_AFEPLLCFG11, u4Tmp, FLD_RG_HA1_PCW); 
        #endif 
            // Check APLL result
            u4Tmp = BSP_GetClock(CAL_SRC_APLL1, 0, 0, 0, 0); 
            u1Loop++;
        }
        while (((u4Tmp > (APLL_FS_48K + 300000)) || (u4Tmp < (APLL_FS_48K - 300000))) && (u1Loop < RETRY_CNT));
        if ((u4Tmp > (APLL_FS_48K + 300000)) || (u4Tmp < (APLL_FS_48K - 300000)))
        {
            Printf("APLL clock = %d. Calibration FAIL!!\n", u4Tmp);
        }
    }
    else
    {
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        //TODO
        //HADDS2 setting (analog)
        //REG(RG_HADDS2_PWD) = 1b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 1, FLD_RG_HADDS2_PWD);
        //REG(RG_HADDS2_27M_SEL) = 1b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 1, FLD_RG_HADDS2_27M_SEL);
        //REG(RG_HADDS2_CKREF_SEL[1:0]) = 00b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0, FLD_RG_HADDS2_CKREF_SEL);
        //REG(RG_HADDS2_RESERVE[0]) =1b		
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 1, FLD_RG_HADDS2_RESERVE);
        #if 0//CC_AUD_PLL_HALF
        //REG(RG_HADDS2_POSDIV[1:0]) = 01b for 294.912/270.9504
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 1, FLD_RG_HADDS2_POSDIV);
        #else
        //REG(RG_HADDS2_POSDIV[1:0]) = 10b for 147.456/135.4752
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 2, FLD_RG_HADDS2_POSDIV); 
        #endif
        //REG(RG_HADDS2_FBSEL[1:0]) = 00b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 1, FLD_RG_HADDS2_FBSEL); //TODO_ES 0
        //REG(RG_HADDS2_FPEN) = 1b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_FPEN);
        //REG(RG_HADDS2_MONCKEN) = 0b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0, FLD_RG_HADDS2_MONCKEN);
        //REG(RG_HADDS2_HF) = 0b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0, FLD_RG_HADDS2_HF);
        //REG(RG_HADDS2_LF) = 0b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0, FLD_RG_HADDS2_LF);
        //REG(RG_HADDS2_VODEN) = 1b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 1, FLD_RG_HADDS2_VODEN);
        //REG(RG_HADDS2_DDSEN) = 0b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0, FLD_RG_HADDS2_DDSEN);
        //REG(RG_HADDS2_DDS_DMY[15:0]) = 0x0067
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0x0067, FLD_RG_HADDS2_DDS_DMY);
        //REG(RG_HADDS2_DDS_HF_EN)= 0b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0, FLD_RG_HADDS2_DDS_HF_EN);
        //REG(RG_HADDS2_PWD) = 0b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0, FLD_RG_HADDS2_PWD);
        //REG(RG_HADDS2_FMEN) = 1b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_FMEN); //TODO_ES 0
        //REG(RG_HADDS2_CK_98M_RSTB) = 0b
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 1, FLD_RG_HADDS2_CK_98M_RSTB); //TODO_ES 0
    #endif 

        switch (eFs)
        { 
        case FS_44K:
            do
            {
            #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
                //Reset Again
                //RG_APLL_DDS_RSTB (0=>1)
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0, FLD_RG_HADDS2_DDS_RSTB);                
                //RG_APLL_PCW_NCPO_CHG
                //RG_APLL_FIFO_START_MAN (0=>1)
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0, FLD_RG_HADDS2_FIFO_START_MAN);                
                //RG_APLL_NCPO_EN (0=>1)
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0, FLD_RG_HADDS2_RG_NCPO_EN);                
                //RG_APLL_DDSEN (0=>1)
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0, FLD_RG_HADDS2_DDSEN);                
                x_thread_delay(1);
                //Reset End
                
                //delay 100us
                //REG(RG_HADDS2_DDS_PWDB) = 1b
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_DDS_PWDB);
                //REG(RG_HADDS2_DDS_RSTB) = 1b
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_DDS_RSTB);
                
                //HADDS2 setting (digital)
                bHDMIAudioSigmaDeltaSetting(0x205bc01a, 0x21,0x1);

                //REG(RG_HADDS2_FIFO_START_MAN) = 1b
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_FIFO_START_MAN);
                //REG(RG_HADDS2_DDS_DMY[4]) = 1b
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_RG_NCPO_EN);
                //REG(RG_HADDS2_DDSEN) = 1b
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 1, FLD_RG_HADDS2_DDSEN);
            #else
                bHDMIAudioSigmaDeltaSetting(0x205bc01a, 0x21,0x1);
            #endif
	            // Check APLL result
    	        u4Tmp = BSP_GetClock(CAL_SRC_APLL2, 0, 0, 0, 0);
    	        u1Loop++;
            }
            while (((u4Tmp > (APLL_FS_44K + 300000)) || (u4Tmp < (APLL_FS_44K - 300000))) && (u1Loop < RETRY_CNT));
            if ((u4Tmp > (APLL_FS_44K + 300000)) || (u4Tmp < (APLL_FS_44K - 300000)))
            {
                Printf("APLL2 clock = %d. Calibration FAIL!!\n", u4Tmp);
            } 
            break;
            
        default:
            do
            {
            #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
                //Reset Again
                //RG_APLL_DDS_RSTB (0=>1)
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0, FLD_RG_HADDS2_DDS_RSTB);                
                //RG_APLL_PCW_NCPO_CHG
                //RG_APLL_FIFO_START_MAN (0=>1)
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0, FLD_RG_HADDS2_FIFO_START_MAN);                
                //RG_APLL_NCPO_EN (0=>1)
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0, FLD_RG_HADDS2_RG_NCPO_EN);                
                //RG_APLL_DDSEN (0=>1)
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0, FLD_RG_HADDS2_DDSEN);                
                x_thread_delay(1);
                //Reset End
              
                //delay 100us
                //REG(RG_HADDS2_DDS_PWDB) = 1b
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_DDS_PWDB);
                //REG(RG_HADDS2_DDS_RSTB) = 1b
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_DDS_RSTB);

                bHDMIAudioSigmaDeltaSetting(0x867c3ece, 0x3d,0x1);

                //REG(RG_HADDS2_FIFO_START_MAN) = 1b
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_FIFO_START_MAN);
                //REG(RG_HADDS2_DDS_DMY[4]) = 1b
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_RG_NCPO_EN);
                //REG(RG_HADDS2_DDSEN) = 1b
                vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 1, FLD_RG_HADDS2_DDSEN);
            #else
                bHDMIAudioSigmaDeltaSetting(0x867c3ece, 0x3d,0x1);
            #endif
                // Check APLL result
                u4Tmp = BSP_GetClock(CAL_SRC_APLL2, 0, 0, 0, 0);
                u1Loop++;
            }
            while (((u4Tmp > (APLL_FS_48K + 300000)) || (u4Tmp < (APLL_FS_48K - 300000))) && (u1Loop < RETRY_CNT));
            if ((u4Tmp > (APLL_FS_48K + 300000)) || (u4Tmp < (APLL_FS_48K - 300000)))
            {
                Printf("APLL2 clock = %d. Calibration FAIL!!!\n", u4Tmp);
            } 
            break;
        }
    }
}


//-----------------------------------------------------------------------------
/** _ConfigAPLLK1K2
 *  Config APLL K1 K2 value
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  K1        (2,3)
 *  @param  K2        (2 ~ 48)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static BOOL  _AUD_PLL_ConfigDivider(UINT8 u1DecId, UINT8 K1, UINT8 K2)
{
    UINT16  u2K1;
    UINT16  u2K2;

    AUD_DEC_ID_VALIDATE(u1DecId);
    

    if (K1 == 2)
    {
        u2K1 = 0;
    } 
    else if (K1 == 3)
    {
        u2K1 = 1;
    }
    else
    {
        return FALSE; 
    }
  
    if ((K2 >= K2_VALID_VALUE_MIN) && (K2 <= K2_VALID_VALUE_MAX))
    {
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)) //CC_AUD_PLL_HALF
        //for APLL 147.456/135.4752
        u2K2 = K2/2;     
#else
        u2K2 = K2; 
#endif
    } 
    else
    {
        return FALSE;
    }

  
    if (u1DecId == AUD_DEC_MAIN)
    {
        vIO32WriteFldAlign(CKGEN_APLLCFG, u2K2, FLD_APLL_K2);          
        vIO32WriteFldAlign(CKGEN_APLLCFG, u2K1, FLD_APLL_K1);
    }
    else // APLL1
    {
        vIO32WriteFldAlign(CKGEN_APLL2CFG, u2K2, FLD_APLL2_K2);          
        vIO32WriteFldAlign(CKGEN_APLL2CFG, u2K1, FLD_APLL2_K1);
    }

    return TRUE;
}

void _AudPllFsDivSetting(UINT8 u1ApllId, SAMPLE_FREQ_T eFs)
{
    MCLK_FREQUENCY_T eMclk = AUD_GetMclkFs(u1ApllId);//_arAudOutCfg[u1ApllId].eSampleFreq;

	if(u1ApllId == APLL_ID_MAIN)
	{
	    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 8));    	
       	LOG(5, "Set APLL[MAIN] K1&K2 = FS_48K\n");
	}
	else
	{
	    // Setup APLL divider
	    switch (eFs)
	    {   
	    case FS_32K:
	        switch (eMclk)
	        {
	        case MCLK_128FS:
	            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 24)); // Set K1 = 3, K2 = 24, MCLK = 4.096 MHz
	            break;
	        case MCLK_256FS:
	            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 12)); // Set K1 = 3, K2 = 12, MCLK = 8.192 MHz
	            break;
	        case MCLK_384FS:
	            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 2, 12)); // Set K1 = 2, K2 = 12, MCLK = 12.288 MHz
	            break;
	        case MCLK_512FS:
	            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 6)); // Set K1 = 3, K2 = 6, MCLK = 16.384 MHz
	            break;
	        case MCLK_768FS:
	            //to do
	            break;
	        default:
	        	//256Fs
	            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 12)); // Set K1 = 3, K2 = 12, MCLK = 8.192 MHz	        	
	            break;
	        }
           	LOG(5, "Set APLL[AUX] K1&K2 = FS_32K\n");
	        break;
	    default:
	        // Default set to 48kHz
	        switch (eMclk)
	        {
	        case MCLK_128FS:
	            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 16)); // Set K1 = 3, K2 = 16, MCLK = 6.144 MHz
	            break;
	        case MCLK_256FS:
	            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 8)); // Set K1 = 3, K2 = 8, MCLK = 12.288 MHz
	            break;
	        case MCLK_384FS:
	            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 2, 8)); // Set K1 = 2, K2 = 8, MCLK = 18.432 MHz
	            break;
	        case MCLK_512FS:
	            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 4)); // Set K1 = 3, K2 = 4, MCLK = 24.576 MHz
	            break;
	        case MCLK_768FS:
	            //to do
	            break;
	        default:
	            //256FS
	            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 8)); // Set K1 = 3, K2 = 8, MCLK = 12.288 MHz
	            break;
        	}
        	LOG(5, "Set APLL[AUX] K1&K2 = FS_48K/FS_44K\n");
        	break;
		}
    }
}

static void _AudPllFsSetting(UINT8 u1ApllId, SAMPLE_FREQ_T eFs)
{
	_AudPllInitial(u1ApllId, eFs);
	_AudPllFsDivSetting(u1ApllId, eFs);
}

static void _AudDelay(UINT32 u4Value)
{
    volatile UINT32 u4Cnt;

    volatile UINT32 u4Loop;

    for (u4Cnt = 0; u4Cnt < u4Value; u4Cnt++)
    {
        for (u4Loop = 0; u4Loop < 25/*10*/; u4Loop++)
        {
            ;
        }
    }
}

BOOL AUD_IsHdmiClkExist(void)
{
    BOOL fgRet = TRUE;
    UINT32 u4Reg = 0;
    CRIT_STATE_T crit;
    
    // Step 0. Reset HDMI COUNTER
    crit = x_crit_start();	
    
    vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_RST_HDMI_CNT);
    // Step 1. Check COUNTER
    
    _AudDelay(30);// HDMI_MCLK = 12.288MHz, CPU_CLK = x00MHz,
                  // needs to wait for some CPU cycles
    
    u4Reg = IO32ReadFld(CKGEN_AUD_CKCFG, FLD_HDMI_CNT);

    x_crit_end(crit);
    
    if (u4Reg != 0) 
    {
        fgRet = TRUE;
    }
    else
    {
        fgRet = FALSE;
    }

    return fgRet;
}

void AudPll1Setting(void)
{
    _AudPllFsSetting(APLL_ID_MAIN, FS_48K);
}

void AudPll2Setting(SAMPLE_FREQ_T eFs)
{
    _AudPllFsSetting(APLL_ID_AUX, eFs);
}
void AudPllPowerDown(UINT8 PLL_ID, BOOL fgPD)
{
 if( PLL_ID == APLL_ID_MAIN )
 {
    if (fgPD)
  	  vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1 , FLD_AUD_CK_PD);
    else
 	  vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 0 , FLD_AUD_CK_PD);
 }
 else
 {
    if (fgPD)
  	  vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 1 , FLD_AUD2_CK_PD);
    else
	  vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 0 , FLD_AUD2_CK_PD);
 }	
}

//-----------------------------------------------------------------------------
/** _AudPllInit
 *  Audio PLL initial
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AudPllInit(void)
{
    // Initial set sampling freqnecy MAIN/AUX: 48K
    _AudPllFsSetting(APLL_ID_MAIN, FS_48K);
    _AudPllFsSetting(APLL_ID_AUX, FS_48K);

#if defined(CC_MT5368) || defined(CC_MT5396) 
    //AUD3_CK_SEL from apll_ck
    vIO32WriteFldAlign(CKGEN_AUD3_CKCFG, 1, FLD_AUD3_CK_SEL);
#if 0//CC_AUD_PLL_HALF
    // 294.912MHz/(23+1) = 12.288MHz, 12.288/256 = 48KHz (TODO) 
    vIO32WriteFldAlign(CKGEN_AUD3_CKCFG, 23, FLD_AUD3_CK_DIVIDE);
#else
    // 147.456MHz/(11+1) = 12.288MHz, 12.288/256 = 48KHz (TODO) 
    vIO32WriteFldAlign(CKGEN_AUD3_CKCFG, 11, FLD_AUD3_CK_DIVIDE); 
#endif
#else
#if defined(CC_MT5389)
    vIO32WriteFldAlign(CKGEN_AUD3_CKCFG, 3, FLD_AUD3_CK_SEL);
    // 432MHz/(29+1) = 14.4MHz, 14.4/256 = 56.25KHz
    vIO32WriteFldAlign(CKGEN_AUD3_CKCFG, 29, FLD_AUD3_CK_DIVIDE);
#else
    // Set AUD3 clock for Line-in
  #if 1 //Change AUD3 to APLL1 for 50Hz video in, audio have 15.8KHz noise tone issue.
    vIO32WriteFldAlign(CKGEN_AUD3_CKCFG, 1, FLD_AUD3_CK_SEL);
    // 294.912MHz/(17+1) = 16.384MHz, 16.384/256 = 64KHz
    vIO32WriteFldAlign(CKGEN_AUD3_CKCFG, 17, FLD_AUD3_CK_DIVIDE);
  #else
    vIO32WriteFldAlign(CKGEN_AUD3_CKCFG, 3, FLD_AUD3_CK_SEL);
    // 432MHz/(31+1) = 13.5MHz, 13.5/256 = 52.734375KHz
    //vIO32WriteFldAlign(CKGEN_AUD3_CKCFG, 31, FLD_AUD3_CK_DIVIDE);
    // 432MHz/(26+1) = 16MHz,   16/256 = 62.5KHz
    vIO32WriteFldAlign(CKGEN_AUD3_CKCFG, 26, FLD_AUD3_CK_DIVIDE);
  #endif
  #endif
#endif

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    // Setup clock source as apll
    vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_AUD_CK_SEL);   //apll_k2_ck
    vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 2, FLD_AUD2_CK_SEL); //apll2_k2_ck
    // Set ASRC reference clock to APLL1 (294.912MHz)
    vIO32WriteFldAlign(CKGEN_ASRC_CKCFG, 5, FLD_ASRC_CK_SEL); //apll_ck
#else
	#if defined(CC_MT5395)
	// Setup clock source as apll
    vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_AUD_CK_SEL);   //apll_k2_ck
    vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 3, FLD_AUD2_CK_SEL); //apll2_k2_ck
    // Set ASRC reference clock to APLL1 (294.912MHz)
    vIO32WriteFldAlign(CKGEN_ASRC_CKCFG, 1, FLD_ASRC_CK_SEL); //apll_ck
	#else
    // Setup clock source as apll
    vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_AUD_CK_SEL);   //apll_k2_ck
    vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 1, FLD_AUD2_CK_SEL); //apll2_k2_ck
    // Set ASRC reference clock to APLL1 (294.912MHz)
    vIO32WriteFldAlign(CKGEN_ASRC_CKCFG, 1, FLD_ASRC_CK_SEL); //apll_ck
    #endif
#endif
}

//-----------------------------------------------------------------------------
/** AUD_DspClkSel
 *  ADSP Clk cource selection
 *
 *  @param  u1ADSPclkSel    
    DSP_CK_SEL	dsp_ck clock source selection

    //5365/95
    0: xtal_ck 
    1: usbpll_d1_ck
    2: syspll_d1_ck ==> 459MHz (default setting)
    3: sawlesspll_d1_ck
    4: M_CK
    5: usbpll_d2_ck
    6: syspll_d2_ck
    7: dmpll_ck
    8: apll_ck
    9: apll2_ck
    10: fractional divider
    11~13: syspll_d2_ck
    14: usbpll_d1_ck
    15: bist_ck

    //5368/96
    0: xtal_ck
    1: sawlesspll_d1_ck ==> 432MHz (default setting)
    2: usbpll_d1p5_ck
    3: syspll_d2_ck
    4: enetpll_d1p5_ck  ==> 450MHz (may try)
    5: apll_ck
    6: apll2_ck
    7: vdecpll_ck
    8: M_CK
    9: usbpll_ck
    10~13: syspll_d2_ck
    14: usbpll_d2_ck
    15: bist_ck
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspClkSel(UINT8 u1ADSPclkSel)
{
    vIO32WriteFldAlign(CKGEN_DSP_CKCFG, (UINT32)u1ADSPclkSel, FLD_DSP_CK_SEL);  
}


//-----------------------------------------------------------------------------
/** AudPllSrc
 *  Audio PLL source select
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------

void AudPllSrcSel(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    static AUD_DEC_STREAM_FROM_T eSourceOrg = AUD_STREAM_FROM_OTHERS;

    switch (eStreamFrom)
    {
        case AUD_STREAM_FROM_SPDIF:
          #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
            vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 7, FLD_AUD2_CK_SEL); //external_hdmi_ck
          #else
            vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 4, FLD_AUD2_CK_SEL);
          #endif
            break;        
        case AUD_STREAM_FROM_HDMI:
          #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
            vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 2, FLD_AUD2_CK_SEL); // apll2 synced to HDMI-Rx
          #else            
            vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 3, FLD_AUD2_CK_SEL);
          #endif
            break;        
        case AUD_STREAM_FROM_LINE_IN:
        case AUD_STREAM_FROM_DIGITAL_TUNER:
        case AUD_STREAM_FROM_ANALOG_TUNER:
        case AUD_STREAM_FROM_MEMORY:
        case AUD_STREAM_FROM_MULTI_MEDIA:                
        case AUD_STREAM_FROM_FEEDER:
          #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
            vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_AUD_CK_SEL);   //apll_k2_ck
            vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 2, FLD_AUD2_CK_SEL); //apll2_k2_ck
          #else            
		  	#if defined(CC_MT5395)
            vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_AUD_CK_SEL); //apll_k2_ck
            vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 3, FLD_AUD2_CK_SEL); //apll2_k2_ck
            #else
			vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_AUD_CK_SEL); //apll_k2_ck
            vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 1, FLD_AUD2_CK_SEL); //apll2_k2_ck
			#endif
          #endif            
            break;
        default:
            break;
    }

    // HDMI Clock Switch Verify
    // If original source is HDMI, and if there is no clock on HDMI, then mux switch may fail
    // Below function do the confirmation of mux switch from HDMI
    if ((u1DecId == AUD_DEC_MAIN) &&
        (eSourceOrg == AUD_STREAM_FROM_HDMI) && 
        (eStreamFrom != AUD_STREAM_FROM_HDMI))
    {
        if (AUD_IsHdmiClkExist()) 
        {
            // Clock Exit. No problem
            // LOG(3, "####### HDMI Exist ######\n");
        }
        else  // HDMI clock doesn't exit, may need to toggle clock
        {
            INT32 i4Cnt = 0;
            for (i4Cnt = 0; i4Cnt <= HDMI_CLK_TOGGLE_TIMES;  i4Cnt++)
            {
                vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 0, FLD_TOG_HDMI_CLK);        
                vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_TOG_HDMI_CLK);        
            }    
            LOG(7, "####### HDMI NOT Exist, toggle HDMI clock ######\n");
        }
            
    }
    if (u1DecId == AUD_DEC_MAIN)
    {
        eSourceOrg = eStreamFrom;    
    }
}

