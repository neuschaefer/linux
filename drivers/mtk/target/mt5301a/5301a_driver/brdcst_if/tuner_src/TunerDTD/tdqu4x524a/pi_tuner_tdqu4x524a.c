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
 * $RCSfile: pi_tuner_tdqu4x524a.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_tdqu4x524a.c
 *  Tuner control for SS DTVS205EH / DTVS203EH.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "demod_tuner_interface.h"
//#include "ctx_tuner.h"
//#include "PD_Def.h"


//EXTERN UINT8 u1I2cWriteTuner(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
//EXTERN UINT8 u1I2cReadTuner(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define ALPS_TDQU4X524A_BASE            ((UINT16) 0xC2)
#define ALPS_TDQU4X524A_IF_FREQUENCY    ((UINT16) 44)   /* MHz */
#define ALPS_TDQU4X524A_LO_STEP         ((UINT16) 625)  /* 100Hz */
#define ALPS_TDQU4X524A_STR             "ALPS_TDQU4X524A"

/* Reference Divider Ratio Select */
// R2 - R0: Bit_2 - 0
#define cwTDQU4_RS              3
#define cpTDQU4_RS              0
#define cmTDQU4_RS              mcSET_MASKS(TDQU4_RS)
#define cTDQU4_RSA_RSB          0x03    // for 62.5 kHz step size

//----------------------------------------------------------------------------- 
/** TDQU4X524A_TunerInit
 *  Tuner initialzation for TDQU4X524A.
 *  @param  psTunerCtx  Pointer of RF tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID TDQU4X524A_TunerInit(RF_TUNER_CTX_T *psTunerCtx, UINT8 I2cAddress)
{
#if 0	
UINT8   u1Value = 0;
UINT8   u1Value[3];
#endif

    psTunerCtx->u1TunerType = cALPS_TDQU4X524A;
    psTunerCtx->I2cAddress = I2cAddress;
    psTunerCtx->u1IF_I2cAddr = I2cAddress;
    psTunerCtx->u2IF_Freq = ALPS_TDQU4X524A_IF_FREQUENCY;
    psTunerCtx->u2LO_Step = ALPS_TDQU4X524A_LO_STEP;
    psTunerCtx->u2Ana_LO_Step = psTunerCtx->u2LO_Step;
    psTunerCtx->u4RF_Freq = 0;

#if 0    
    u1Value[0] = 0x23; // AD_Target_Level_0
    u1Value[1] = 0x00; // AD_Target_Level_1
    u1Value[2] = 0x01; // Enable fgMaunal_AD_Target_Level
    MT5112_u1SetFwVars(psTunerCtx, eVAR_UCAD_TARGET_LEVEL_0, (UINT8 *) &u1Value, 3);
#endif

    mcSHOW_DBG_MSG(("Init " ALPS_TDQU4X524A_STR "\n"));
}


//----------------------------------------------------------------------------- 
/** TDQU4X524A_TunerSetFreq
 *  Set Tuner PLL for DTVS203EH /DTVS205EH to tune RF frequency.
 *  @param  psTunerCtx  Pointer of RF tuner driver context.
 *  @param  Freq	    RF center frequency in KHz.
 *  @param  u1Mod	  		
 *  @param  u2Step	  		
 *  @retval 
 */
//-----------------------------------------------------------------------------
INT16 TDQU4X524A_TunerSetFreq(RF_TUNER_CTX_T *psTunerCtx, UINT32 Freq, UINT8 u1Mod, UINT16 u2Step)
{
UINT32  Lo;
UINT8   Nband;
UINT16  Ndivider;
BOOL 		fgAnalogSignal = FALSE;
UINT8   TunerS[6], TunerDataCnt;

    mcSHOW_DBG_MSG(("ALPS TDQU4X524A_TunerSetFreq\n"));

    psTunerCtx->u4RF_Freq = Freq;
    if (u1Mod >= MOD_ANA_TYPE_BEGIN )
        fgAnalogSignal = TRUE;

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
    if (Freq < 82)
        Nband = 0x00;
    else if (Freq < 88)
        Nband = 0x00;
    else if (Freq < 162)
        Nband = 0x00;
    else if (Freq < 426)
        Nband = 0x02;
    else
        Nband = 0x0A;

    if (Freq < 426)
        Nband |= 0x60;
    else if (Freq < 782)
        Nband |= 0xA0;
    else
        Nband |= 0x20;

/* LO settings */
    TunerS[0] = (UINT8) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UINT8) 0xFF & Ndivider;
    TunerS[2] = 0x88 | (cTDQU4_RSA_RSB << cpTDQU4_RS);
    TunerS[3] = Nband;
    TunerS[4] = 0xC6;
    if (!fgAnalogSignal)
        TunerS[4] = 0xF6;
 
/* Write to tuner device */
    if (u1I2cWriteTuner(psTunerCtx->psI2cCtx, psTunerCtx->I2cAddress, TunerS, TunerDataCnt))
    {
        mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
        return (-1);
    }
    else
    {
        mcSHOW_DBG_MSG(("LO_%02X: (%d) %02X-%02X-%02X-%02X-%02X (DR = %d)\n",
                       psTunerCtx->I2cAddress, TunerDataCnt, TunerS[0], TunerS[1], TunerS[2], TunerS[3], TunerS[4], Ndivider));
    }

    mcDELAY_MS(10);

    return (0);
}

//----------------------------------------------------------------------------- 
/**  TDQU4X524A_TunerGetStatus
 *  Set Tuner PLL staus for TDQU4X524A
 *  @param  psTunerCtx  Pointer of RF tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
BOOL TDQU4X524A_TunerGetStatus(RF_TUNER_CTX_T *psTunerCtx)
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
/** TDQU4X524A_TunerGetVer
 *  Set Tuner PLL staus for TDQU4X524A.
 *  @param  psTunerCtx  Pointer of RF tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
CHAR *TDQU4X524A_TunerGetVer(RF_TUNER_CTX_T *psTunerCtx)
{
    return (ALPS_TDQU4X524A_STR);
}
/***********************************************************************/

