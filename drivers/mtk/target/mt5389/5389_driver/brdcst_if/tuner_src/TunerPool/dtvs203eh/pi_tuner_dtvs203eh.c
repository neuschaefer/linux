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
 * $RCSfile: pi_tuner_dtvs203eh.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_dtvs205eh.c
 *  Tuner control for SS DTVS205EH / DTVS203EH.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "demod_tuner_interface.h"
//#include "pd_i2capi.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"
#include "fe_tuner_common_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SAMSUNG_DTVS203EH_BASE          ((UINT16) 0xC2)
#define SAMSUNG_DTVS203EH_IF_FREQUENCY  ((UINT16) 44)   /* MHz */
#define SAMSUNG_DTVS203EH_LO_STEP       ((UINT16) 625)  /* 100Hz */
#define SAMSUNG_DTVS203EH_STR           "SAMSUNG_DTVS205EH 2010-01-01\n 2011-04-06\n v1.0"

//-----------------------------------------------------------------------------
// Extern definitions
//-----------------------------------------------------------------------------
//EXTERN VOID Tuner_ParaInit(RF_TUNER_CTX_T *psTunerCtx);
//EXTERN UINT8 u1I2cReadTuner(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
//EXTERN UINT8 u1I2cWriteTuner(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);


//----------------------------------------------------------------------------- 
/** DTVS203_TunerInit
 *  Tuner initialzation for DTVS203EH /DTVS205EH.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  i2cAddr     Tuner I2C address.
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID DTVS203EH_TunerInit(
    ITUNER_CTX_T *pTCtx,
    UINT8 i2cAddr
    )
{
    RF_TUNER_CTX_T *psTunerCtx = &pTCtx->dtdTunerCtx;

    /* Common part: All tuners need to do the following procedures */
    //Tuner_ParaInit(psTunerCtx);   // move to InitializeTunerCtx() (tuner_interface.c)
    psTunerCtx->I2cAddress = i2cAddr;
    psTunerCtx->u1IF_I2cAddr = i2cAddr;
    psTunerCtx->u4RF_Freq = 0;

    /* Specific part: Only for this tuner model */
//    psTunerCtx->u1TunerType = MT5112_TUNER_SAMSUNG_DTVS203EH;
    psTunerCtx->u1TunerType = 8;
    psTunerCtx->u2IF_Freq = SAMSUNG_DTVS203EH_IF_FREQUENCY;
    psTunerCtx->u2LO_Step = SAMSUNG_DTVS203EH_LO_STEP;
    psTunerCtx->u2Ana_LO_Step = psTunerCtx->u2LO_Step;

    psTunerCtx->m_aucFunc[TUNER_FUNC_EN_TARGET_LEVEL] = TRUE;
    psTunerCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x23;
    psTunerCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = 0x00;   

    mcSHOW_DBG_MSG(("Init " SAMSUNG_DTVS203EH_STR "\n"));
}


//----------------------------------------------------------------------------- 
/** DTVS203EH_TunerSetFreq
 *  Set Tuner PLL for DTVS203EH /DTVS205EH to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  freq	    RF center frequency in KHz.
 *  @param  mode	  		  		
 *  @param  autoSearch	
 *  @param  step	  		
 *  @retval 
 */
//-----------------------------------------------------------------------------
INT16 DTVS203EH_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    UINT32 freq,
    UINT8 mode,
    UINT16 autoSearch,
    UINT16 step
    )
{
    RF_TUNER_CTX_T *psTunerCtx = &pTCtx->dtdTunerCtx;

    UINT32  Lo;
    UINT8   Nband, CByte;
    UINT16  Ndivider;
    BOOL fgAnalogSignal = FALSE;
    UINT8   TunerS[6], TunerDataCnt;

    mcSHOW_DBG_MSG(("SAMSUNG DTVS205EH_SetFreq\n"));

    psTunerCtx->u4RF_Freq = freq;
    if (mode >= MOD_ANA_TYPE_BEGIN )
        fgAnalogSignal = TRUE;

    TunerDataCnt = 4;
    
    Lo = freq + psTunerCtx->u2IF_Freq * 1000;

    Ndivider = (UINT16) ((Lo * 10) / psTunerCtx->u2LO_Step);

    psTunerCtx->u4LO_Freq = Lo;
    freq = freq / 1000;

 /* Assign Control word for specific tuner */
   if (freq < 53 || freq > 865)         // For HRC issue & Chan135
    {
        mcSHOW_HW_MSG(("Out of range for LO!\n"));
        return (+1);
    }

    CByte = 0x86;
    if ( (freq >= 100 && freq < 168) || (freq >= 700) )
        CByte |= 0x40;

    if (freq < 168)
        Nband = 0x01;
    else if (freq < 470)
        Nband = 0x02;
    else
        Nband = 0x08;
    Nband |= 0x04;

    if (fgAnalogSignal)                     // For NTSC
        Nband &= ~0x04;

    /* LO settings */
    TunerS[0] = (UINT8) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UINT8) 0xFF & Ndivider;
    TunerS[2] = (UINT8) CByte;               // Must be set first for init
    TunerS[3] = (UINT8) Nband;
 
/* Write to tuner device */
    if (ICtrlBus_I2cTunerWrite(psTunerCtx->I2cAddress, TunerS, TunerDataCnt))
    {
        mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
        return (-1);
    }
    else
    {
        mcSHOW_DBG_MSG(("LO_%02X: (%d) %02X-%02X-%02X-%02X (DR = %d)\n",
                       psTunerCtx->I2cAddress, TunerDataCnt, TunerS[0], TunerS[1], TunerS[2], TunerS[3], Ndivider));
    }

    mcDELAY_MS(10);

    return (0);
}

//----------------------------------------------------------------------------- 
/**  DTVS203EH_TunerGetStatus
 *  Set Tuner PLL staus for DTVS203EH /DTVS205EH
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
BOOL DTVS203EH_TunerGetStatus(
    ITUNER_CTX_T *pTCtx
    )
{
    RF_TUNER_CTX_T *psTunerCtx = &pTCtx->dtdTunerCtx;

    UINT8 TunerS[4];

    if (ICtrlBus_I2cTunerRead(psTunerCtx->I2cAddress, TunerS, 1))
    {
        mcSHOW_HW_MSG(("Tuner read failed!\n"));
        return FALSE;
    }

    if (mcTEST_BIT(TunerS[0], 6))
        return TRUE;

    return FALSE;

}

//----------------------------------------------------------------------------- 
/** DTVS203EH_TunerGetStatus
 *  Set Tuner PLL staus for DTVS203EH /DTVS205EH
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
CHAR *DTVS203EH_TunerGetVer(
    ITUNER_CTX_T *pTCtx
    )
{
    return (SAMSUNG_DTVS203EH_STR);
}
/***********************************************************************/

