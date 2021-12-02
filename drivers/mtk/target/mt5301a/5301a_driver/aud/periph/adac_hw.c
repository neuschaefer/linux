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
 * $RCSfile: adac_hw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_hw.c
 *  Brief of file adac_hw.c.
 *  Note that MT537X support 4-channel interanl DAC
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "x_typedef.h"
#include "aud_if.h"
#include "adac_hw.h" // For environment register read & write
#include "drvcust_if.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Macro 
//---------------------------------------------------------------------------

/*
#define    REGISTER_ACCESS_TIME_BARRIER()      __asm { \
       		                                          nop  \
                                                      nop  \
                                                      nop  \
       		                                          nop  \
       		                                          nop  \
       		                                          nop  \
       		                                          nop  \
       		                                          nop  \
       		                                          nop  \
       		                                          nop  \
       	                                             }
*/
#define    REGISTER_ACCESS_TIME_BARRIER(u4Tick)      {  \
	                                                    volatile UINT32 i, j; \
                                                        for (i=0; i<u4Tick; i++)  \
                                                        {                     \
                                                            j = i;            \
                                                            UNUSED(j);  \
                                                        }                     \
                                                     }   

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define RESET_TIMES 4

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static ADAC_FUNCTBL_T _rInterDacFunTbl = 
{
    _ADAC_InterDacInit,     //pfnInit
    _ADAC_InterDacMute,     //pfnMute
    _ADAC_InterDacEnable,   //pfnEnable
    _ADAC_InterDacChlSel,   //pfnChlSelect
    NULL,                   //pfnSetDacInFmt;
    NULL,                   //pfnSetDacGain;
    NULL,                   //pfnWrite;
    NULL                    //pfnRead;
};

static ADAC_T _rInterAdac = 
{
    "INTERDAC",             // szName[32];
    256,                    // i2MaxGain
    -256,                   // i2MaxGain
    2,                      // u1DacNum
    TRUE,                   // fInterDac
    &_rInterDacFunTbl       // rAdacFuncTbl
};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _ADACReset(void)
{
/*
    INT32 i;
    for (i = 0; i < RESET_TIMES; i ++)
    {
        ADAC_WRITE32(REG_ADC_CFG_24, 0x3c);
        ADAC_WRITE32(REG_ADC_CFG_34, 0x14);	
        REGISTER_ACCESS_TIME_BARRIER(10);
        ADAC_WRITE32(REG_ADC_CFG_24, 0x3e);//ADAC_WRITE32(REG_ADC_CFG_24, 0x3f);
        ADAC_WRITE32(REG_ADC_CFG_34, 0x34);	
        ADAC_WRITE32(0x344, 0x0c);//ADAC_WRITE32(0x344, 0x00);
    }   
*/
/*
    ADAC_WRITE32(0x344, 0x0c);//ADAC_WRITE32(0x344, 0x00);
    ADAC_WRITE32(REG_ADC_CFG_34, 0x34);	
    ADAC_WRITE32(REG_ADC_CFG_24, 0x3c);
    REGISTER_ACCESS_TIME_BARRIER(10);
    ADAC_WRITE32(REG_ADC_CFG_24, 0x3e);    
    */
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _ADAC_InterDacCfg
 *  Internal ADAC channel select.
 *  Note that the 4 channel must be on the same decoder (cannot be on 
 *  different decoder (MAIN and AUX) simultaneously)
 *
 *  @param  u1DecId     Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eChl        Channel selection
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _ADAC_InterDacChlSel(UINT8 u1DacId, AUD_CHL_DEF_T eChl)
{
/*
    if (u1DacId == AUD_DEC_MAIN)
    {
        if (eChl == AUD_CHL_AUX)
        {
            ADAC_WRITE32(REG_ADC_CFG1, (ADAC_READ32(REG_ADC_CFG1) & (~CHL_SEL_MASK)) | CHL_SEL_CH0);
            ADAC_WRITE32(REG_AENV_CFG, (ADAC_READ32(REG_AENV_CFG) & (~INT_DAC1_SEL_MASK)) |
                                   (((UINT32)0) << 3) | INT_DAC_SEL_AUX);
        }
        else
        {
            ADAC_WRITE32(REG_ADC_CFG1, (ADAC_READ32(REG_ADC_CFG1) & (~CHL_SEL_MASK)) | CHL_SEL_CH0);
            ADAC_WRITE32(REG_AENV_CFG, (ADAC_READ32(REG_AENV_CFG) & (~INT_DAC1_SEL_MASK)) |
                                   (((UINT32)eChl) << 3) | INT_DAC_SEL_MAIN);
        }
    }
    else // AUD_DEC_AUX
    {
        if (eChl == AUD_CHL_AUX)
        {
            ADAC_WRITE32(REG_ADC_CFG1, (ADAC_READ32(REG_ADC_CFG1) & (~CHL_SEL_MASK)) | CHL_SEL_CH0);
            ADAC_WRITE32(REG_AENV_CFG, (ADAC_READ32(REG_AENV_CFG) & (~INT_DAC2_SEL_MASK)) |
                                   (((UINT32)0) << 6) | INT_DAC_SEL_AUX);
        }
        else
        {
            ADAC_WRITE32(REG_ADC_CFG1, (ADAC_READ32(REG_ADC_CFG1) & (~CHL_SEL_MASK)) | CHL_SEL_CH0);
            ADAC_WRITE32(REG_AENV_CFG, (ADAC_READ32(REG_AENV_CFG) & (~INT_DAC2_SEL_MASK)) |
                                   (((UINT32)eChl) << 6) | INT_DAC_SEL_MAIN);
        }
    }
    */
}

//-----------------------------------------------------------------------------
/** _ADAC_InterDacEnable
 *  Turn on/off internal ADAC. \n
 *  Note that 4 channel ADAC cannot be powered down individually.
 *
 *  @param  eChannel       ADAC_STEREO_CHANNEL_MAIN/ADAC_STEREO_CHANNEL_AUX
 *  @param  fgEnalbe       On/Off
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _ADAC_InterDacEnable(BOOL fgEnable)
{    
    if (fgEnable)
    {
	ADAC_WRITE32(0x01d0,0x44440e04);
    }
    else
    {
	ADAC_WRITE32(0x01d0,0x44440cff);
    }
}


//-----------------------------------------------------------------------------
/** _ADAC_InterDacInit
 *  Intial internal ADAC
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _ADAC_InterDacInit(void)
{
/*
    AUD_INTER_DAC_CFG_T* prInterDacUsage;
    
    ADAC_WRITE32(REG_ADC_CFG0, TRIG_SEL_RISING |
    	                       CLK_DIV_BY_2 |
    	                       CLK_SEL_INT |
    	                       NCLK_CTRL_350_PS);
    ADAC_WRITE32(REG_ADC_CFG1, CORE_POWER_DOWN_EN |
    	                       REF_POWER_DOWN_EN | 
    	                       CHL_SEL_CH0 | 
    	                       DAC1_CUR_X1 |
    	                       DAC2_CUR_X1 |
    	                       DAC3_CUR_X1 |
    	                       DAC4_CUR_X1);
    ADAC_WRITE32(REG_ADC_CFG2, VOL_CUR_ADJUST_DEFAULT);

    DRVCUST_OptQuery(eAudioInterDacChlConfig, (UINT32 *)&prInterDacUsage);
    if (prInterDacUsage != NULL)
    {
        _ADAC_InterDacChlSel(AUD_DEC_MAIN, prInterDacUsage->eDac0ChlSel);
        _ADAC_InterDacChlSel(AUD_DEC_AUX, prInterDacUsage->eDac1ChlSel);
    }

    // Missing registers  
    ADAC_WRITE32(0x300, 0xff);
    ADAC_WRITE32(0x304, 0xff);
    ADAC_WRITE32(0x308, 0x07);
    ADAC_WRITE32(0x30c, 0x14);
    ADAC_WRITE32(0x310, 0x0);
    ADAC_WRITE32(0x314, 0x0);
    ADAC_WRITE32(0x318, 0x0);
    //ADAC_WRITE32(0x31c, 0x4);
    ADAC_WRITE32(0x31c, 0x0);
    ADAC_WRITE32(0x320, 0x81);
    ADAC_WRITE32(0x324, 0x3e);
    ADAC_WRITE32(0x328, 0x07);
    ADAC_WRITE32(0x32c, 0xdb);
    ADAC_WRITE32(0x330, 0xe0);
    ADAC_WRITE32(0x334, 0x34);
    ADAC_WRITE32(0x338, 0x0);
    ADAC_WRITE32(0x33c, 0x0);
    ADAC_WRITE32(0x340, 0x0);
    ADAC_WRITE32(0x344, 0x0c);
    ADAC_WRITE32(0x348, 0x0);

    _ADACReset();
    */
ADAC_WRITE32(0x01c0,0x0087ffff);
ADAC_WRITE32(0x01c4,0x00000001);
ADAC_WRITE32(0x01c8,0x5087ffff);
ADAC_WRITE32(0x01d0,0x44440e04);
ADAC_WRITE32(0x01e0,0x00100084);
ADAC_WRITE32(0x01d4,0x00003f01);
ADAC_WRITE32(0x01d8,0x00003000);
ADAC_WRITE32(0x0204,0x0000001b);

}

//-----------------------------------------------------------------------------
/** _ADAC_InterDacInit
 *  Intial internal ADAC
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _ADAC_InterDacMute(BOOL fgEnable)
{
    if (fgEnable)
    {
        ADAC_WRITE32(0x01c0, 0x0187ffff);
    }
    else
    {
        ADAC_WRITE32(0x01c0, 0x0087ffff);
    }
}

//-----------------------------------------------------------------------------
/** _ADAC_InterDacInit
 *  Intial internal ADAC
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
ADAC_T* _ADAC_GetInterDacTbl(void)
{
    return &_rInterAdac;
}

