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
 * $RCSfile: pi_tuner_tdqu6.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_tdqu6.c
 *  Tuner control for ALPS TDQU6.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "demod_tuner_interface.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define ALPS_TDQU6_BASE          ((UINT16) 0xC2)
#define ALPS_TDQU6_IF_FREQUENCY  ((UINT16) 44)   /* MHz */
#define ALPS_TDQU6_LO_STEP       ((UINT16) 625)  /* 100Hz */
#define ALPS_TDQU6_STR           "ALPS_TDQU6"

//-----------------------------------------------------------------------------
// Extern definitions
//-----------------------------------------------------------------------------
EXTERN VOID Tuner_ParaInit(RF_TUNER_CTX_T *psTunerCtx);
EXTERN UINT8 u1I2cWriteTuner(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
EXTERN UINT8 u1I2cReadTuner(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);

//----------------------------------------------------------------------------- 
/** TDQU6_TunerInit
 *  Tuner initialzation for TDQU6.
 *  @param  psTunerCtx  Pointer of RF tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID TDQU6_TunerInit(RF_TUNER_CTX_T *psTunerCtx, UINT8 I2cAddress)
{

   //UINT8   u1Value[3];
    /* Common part: All tuners need to do the following procedures */
    Tuner_ParaInit(psTunerCtx);
    psTunerCtx->I2cAddress = 0xC0;
    psTunerCtx->u1IF_I2cAddr = 0xC0;
    psTunerCtx->u4RF_Freq = 0;

    /* Specific part: Only for this tuner model */
    psTunerCtx->u1TunerType = cALPS_TDQU6;
    psTunerCtx->u2IF_Freq = ALPS_TDQU6_IF_FREQUENCY;
    psTunerCtx->u2LO_Step = ALPS_TDQU6_LO_STEP;
    psTunerCtx->u2Ana_LO_Step = psTunerCtx->u2LO_Step;

 
    psTunerCtx->m_aucFunc[TUNER_FUNC_EN_TARGET_LEVEL] = TRUE;
    psTunerCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x18;
    psTunerCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = 0x00;   
   
    

    mcSHOW_DBG_MSG(("Init " ALPS_TDQU6_STR "\n"));
}


//----------------------------------------------------------------------------- 
/** TDQU6_TunerSetFreq
 *  Set Tuner PLL for TDQU6 /DTVS205EH to tune RF frequency.
 *  @param  psTunerCtx  Pointer of RF tuner driver context.
 *  @param  Freq	    RF center frequency in KHz.
 *  @param  u1Mod	  		
 *  @param  u2Step	  		
 *  @retval 
 */
//-----------------------------------------------------------------------------
/* Reference Divider Ratio Select */
// R2 - R0: Bit_2 - 0
//#define cwTDQU6_RS              3
#define cpTDQU6_RS              0
//#define cmTDQU8_RSK01A              mcSET_MASKS(TDQU6_RS)
#define cTDQU6_RSA_RSB          0x03    // for 62.5 kHz step size
/* Band Select */
// BS4 - BS1: Bit_3 - 0
//#define cwTDQU6_BS              4
//#define cpTDQU6_BS              0
//#define cmTDQU6_BS              mcSET_MASKS(TDQU6_BS)


INT16 TDQU6_TunerSetFreq(RF_TUNER_CTX_T *psTunerCtx, UINT32 Freq, UINT8 u1Mod, UINT16 u2Step)
{
UINT32  Lo;
UINT8   Nband;
UINT16  Ndivider;
UINT8   TunerS[5], TunerDataCnt;

    mcSHOW_DBG_MSG(("ALPS TDQU6_SetFreq\n"));

    psTunerCtx->u4RF_Freq = Freq;

    TunerDataCnt = 5;
    
    Lo = Freq + psTunerCtx->u2IF_Freq * 1000;
    Ndivider = (UINT16) ((Lo * 10) / psTunerCtx->u2LO_Step);

    psTunerCtx->u4LO_Freq = Lo;
    Freq = Freq / 1000;

 /* Assign Control word for specific tuner */
   if (Freq < 53 || Freq > 865)         
    {
        mcSHOW_HW_MSG(("Out of range for LO!\n"));
        return (+1);
    }

    if (Freq < 132)
        Nband = 0x64;
    else if (Freq < 366)
        Nband = 0x66;
    else if (Freq < 782)
        Nband = 0xAE;
    else
        Nband = 0x2E;


/* LO settings */
    TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UCHAR) 0xFF & Ndivider;
    TunerS[2] = 0x98 | (cTDQU6_RSA_RSB << cpTDQU6_RS); 
    TunerS[3] = Nband;
    TunerS[4] = 0xE6;


 
/* Write to tuner device */
    if (u1I2cWriteTuner(psTunerCtx->psI2cCtx, psTunerCtx->I2cAddress, TunerS, TunerDataCnt))
    {
        mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
        return (-1);
    }
    else
    {
        mcSHOW_DBG_MSG(("LO_%02X: (%d) %02X-%02X-%02X-%02X-%02X\n",
                       psTunerCtx->I2cAddress, TunerDataCnt, TunerS[0], TunerS[1], TunerS[2], TunerS[3], TunerS[4]));
    }

    mcDELAY_MS(10);

    return (0);
}

//----------------------------------------------------------------------------- 
/**  TDQU6_TunerGetStatus
 *  Set Tuner PLL staus for TDQU6 
 *  @param  psTunerCtx  Pointer of RF tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
BOOL TDQU6_TunerGetStatus(RF_TUNER_CTX_T *psTunerCtx)
{
UINT8 TunerS[4];

    if (u1I2cReadTuner(psTunerCtx->psI2cCtx, psTunerCtx->I2cAddress, TunerS, 1))
    {
        mcSHOW_HW_MSG(("Tuner read failed!\n"));
        return FALSE;
    }

    if (mcTEST_BIT(TunerS[0], 6))
        return TRUE;

    return FALSE;

}

//----------------------------------------------------------------------------- 
/** TDQU6_TunerGetStatus
 *  Set Tuner PLL staus for TDQU6
 *  @param  psTunerCtx  Pointer of RF tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
CHAR *TDQU6_TunerGetVer(RF_TUNER_CTX_T *psTunerCtx)
{
    return (ALPS_TDQU6_STR);
}
/***********************************************************************/

