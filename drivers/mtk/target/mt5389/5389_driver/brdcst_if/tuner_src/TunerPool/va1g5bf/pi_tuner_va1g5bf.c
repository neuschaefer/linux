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
 * $RCSfile: pi_tuner_va1g5bf.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_va1g5bf.c
 *  Tuner control for SHARP VA1G5BF.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "demod_tuner_interface.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"
#include "fe_tuner_common_if.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SHARP_VA1G5BF_BASE            ((UINT16) 0xC2)
#define SHARP_VA1G5BF_IF_FREQUENCY    ((UINT16) 44000)   /* KHz */
#define SHARP_VA1G5BF_IF_FREQ_ANA   ((U32)  45750)//45750) /* kHz */
#define SHARP_VA1G5BF_LO_STEP         ((UINT32) 142857)  /* Hz */
#define SHARP_VA1G5BF_LO_STEP_ANA     ((UINT16) 50000)  /* 10Hz */
#define SHARP_VA1G5BF_STR             "SHARP_VA1G5BF 2010-01-01\n 2011-04-09\n v1.0"

//-----------------------------------------------------------------------------
// Extern definitions
//-----------------------------------------------------------------------------


//----------------------------------------------------------------------------- 
/** VA1G5BF_TunerInit
 *  Tuner initialzation forVA1G5BF.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  i2cAddr     Tuner I2C address.
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID VA1G5BF_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;

    /* Common part: All tuners need to do the following procedures */
    //Tuner_ParaInit(psTunerCtx);   // move to InitializeTunerCtx() (tuner_interface.c)
    psTunerCtx->I2cAddress = SHARP_VA1G5BF_BASE;
    psTunerCtx->u4RF_Freq = 0;

    /* Specific part: Only for this tuner model */
    psTunerCtx->u2IF_Freq = SHARP_VA1G5BF_IF_FREQUENCY;
    
    
    mcSHOW_DBG_MSG(("Init " SHARP_VA1G5BF_STR "\n"));
}

STATIC INT16 OFDM_calculate_divider_byte(U32 freq)
{
	U16  data;
	U8   bs321 = 0x04, cp210=0xA0;
	U32  Lo;
    U8   byte_[7];

    Lo = freq + SHARP_VA1G5BF_IF_FREQUENCY;
    Lo = (Lo*1000) + (SHARP_VA1G5BF_LO_STEP/2); //Hz
    data = (U16) (Lo / SHARP_VA1G5BF_LO_STEP);

    byte_[0] = 0xFE;
    byte_[1] = SHARP_VA1G5BF_BASE;
	byte_[2] = (U8)((data>>8)&0x7F);
	byte_[3] = (U8)(data&0xFF);
	byte_[4] = 0xC9;  //0xCA //TODO, Justin
	byte_[5] = 0x00;  //b3 is A/D switch //TODO, Justin
	byte_[6] = 0x80;

	//U/V
	if(freq < 161000)						bs321 = 0x01;
	else if(freq>=161000 && freq<426000)	bs321 = 0x02;
	else if(freq>=426000)					bs321 = 0x04;
	byte_[5] &= 0xF8;
	byte_[5] |= bs321;

	//Charge Pump
	if(freq<153000)							cp210 = 0x80;
	else if(freq>=153000 && freq<161000)	cp210 = 0xA0;
	else if(freq>=161000 && freq<233000)	cp210 = 0x60;
	else if(freq>=233000 && freq<343000)	cp210 = 0xA0;
	else if(freq>=343000 && freq<426000)	cp210 = 0xE0;
	else if(freq>=426000 && freq<463000)	cp210 = 0x60;
	else if(freq>=463000 && freq<563000)	cp210 = 0x80;
	else if(freq>=563000 && freq<613000)	cp210 = 0xA0;
	else if(freq>=613000 && freq<713000)	cp210 = 0xC0;
	else if(freq>=713000)					cp210 = 0xE0;
	byte_[5] &= 0x1F;
	byte_[5] |= cp210;

//    if (ucI2cWriteOnly(0xC2, &byte_[1], 5) < 0)
    if (ICtrlBus_I2cTunerWrite(270,0x30, byte_, 7))
    {
        mcSHOW_USER_MSG(("\nTunerSet() failed!\n"));
        return (-1);
    }

    return (0);
}
STATIC U8 VA1G5BF_SetIFDemod()
{
    U8 fgRet = 0;
    U8 uc_data[8];

    uc_data[0] = 0x80; 
    fgRet |= ICtrlBus_I2cDemodWrite(270,0x32, 0x13, uc_data, 1);
    uc_data[0] = 0xFF; 
    fgRet |= ICtrlBus_I2cDemodWrite(270,0x32, 0x17, uc_data, 1);

/*
    Register_[0x1E] = 0x08;
    Register_[0x1F] = 0xA8;
    fgRet |= ICtrlBus_I2cDemodWrite(I2cAddress, 0x1E, (Register_+0x1E), 2);
*/
    uc_data[0] = 0x08;
    uc_data[1] = 0xA8;
    fgRet |= ICtrlBus_I2cDemodWrite(270,0x30, 0x1E, uc_data, 2);

    // power down ofdm, set to sleep mode
    uc_data[0] = 0x70; 
    fgRet |= ICtrlBus_I2cDemodWrite(270,0x30, 0x03, uc_data, 1);

    return fgRet;
}
STATIC INT8 tuner_set_analog(UINT32 freq)
{
    U8   byte_[7], CP210, B321;
    U16  data;
    U32  Lo;

    byte_[0] = 0xFE; // switch on bypass function
    byte_[1] = SHARP_VA1G5BF_BASE; 

    Lo = freq + SHARP_VA1G5BF_IF_FREQ_ANA;
    Lo = (Lo*1000) + (SHARP_VA1G5BF_LO_STEP_ANA/2); //Hz
    data = (U16) (Lo / SHARP_VA1G5BF_LO_STEP_ANA);
    byte_[2] = (U8)((data>>8)&0x7F);
    byte_[3] = (U8)(data&0xFF);

    byte_[4] = 0xCB; // step = 50kHz

//    set_cp_data();
	if (freq < 161000)  // VHF Low
	{
		if (freq < 153000)
			CP210=4;
		else 
			CP210=5;
	}
	else if (freq < 426000) // VHF High
	{
		if (freq < 233000)
			CP210=3;
		else if (freq < 343000)
			CP210=5;
		else
			CP210=7;
	}
	else  //UHF
	{
		if(freq < 463000)
			CP210=3;
		else if (freq < 563000)
			CP210=4;
		else if (freq < 613000)
			CP210=5;
		else if ( freq < 713000)
			CP210=6;
		else
			CP210=7;
	}
	byte_[5] = ((CP210<<5)&0xE0);

//    set_band_data();
	if (freq < 161000){
		B321=1;	//VHF-L
	}
	else if (freq < 426000){
		B321=2;	//VHF-h
	}
	else{
		B321=4;	//UHF
	}
    byte_[5] |= (B321&0x07);
    byte_[5] |= 0x08;

	byte_[6] = 0x86;

/*
    if (analog_link_set_all() != 0)
    {
        mcSHOW_USER_MSG(("\nATD Set() failed!\n"));
        return (-1);
    }
*/
    if (ICtrlBus_I2cTunerWrite(270,0x30, byte_, 7))
    {
        mcSHOW_USER_MSG(("\nTunerSet() failed!\n"));
        return (-1);
    }

    return (0);
}
//----------------------------------------------------------------------------- 
/** VA1G5BF_TunerSetFreq
 *  Set Tuner PLL forVA1G5BF/VA1G5BF to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 VA1G5BF_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
ITUNER_CTX_T *psTunerCtx = pTCtx; 
INT16 Ret;
UINT32 freq=param->Freq;
   mcSHOW_DBG_MSG(("SHARP VA1G5BF_TunerSetFreq %dKHz Mod=%d\n",freq,param->Modulation));
    psTunerCtx->u4RF_Freq=freq;
    #ifdef USE_ATD_IF_DEMOD
    mcSHOW_DBG_MSG(("USE_ATD_IF_DEMOD BUT modify source table\n"));
    #else
    mcSHOW_DBG_MSG(("not define USE_ATD_IF_DEMOD\n"));
    #endif
    if (param->Modulation== MOD_ISDBT) {//ISDBT
        Ret = OFDM_calculate_divider_byte(freq);	
    }
    else {
        Ret = VA1G5BF_SetIFDemod();
        Ret |= tuner_set_analog(freq);
    }
   return (Ret);
}

//----------------------------------------------------------------------------- 
/**  VA1G5BF_TunerGetStatus
 *  Set Tuner PLL staus forVA1G5BF/VA1G5BF
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
STATIC BOOL VA1G5BF_TunerGetStatus(ITUNER_CTX_T *pTCtx)
{
    return TRUE;
}

//----------------------------------------------------------------------------- 
/** VA1G5BF_TunerGetStatus
 *  Set Tuner PLL staus forVA1G5BF/VA1G5BF
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
STATIC CHAR *VA1G5BF_TunerGetVer(VOID)
{
    return (SHARP_VA1G5BF_STR);
}
/***********************************************************************/
//----------------------------------------------------------------------------- 
/** VA1G5BF_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 VA1G5BF_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
         switch(eOperation){
              case itGetStatus:
                {
                    BOOL * Retval=(BOOL *)pInOutVal;
                    *Retval=VA1G5BF_TunerGetStatus(pTCtx);
                }
                break;
              case itGetVer:
                 {
                    CHAR ** RetStr=(CHAR **)pInOutVal;
                    *RetStr=VA1G5BF_TunerGetVer();
                 }
                 break;
              default:
                 return ITUNER_NOT_SUPPORTED;
        }
         return ITUNER_OK;
}
