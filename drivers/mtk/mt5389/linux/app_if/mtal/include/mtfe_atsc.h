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
 * $RCSfile: mtfe_atsc.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mtfe_atsc.h
 *  This header file declares exported APIs of frond end ATSC demod module.
 */

#ifndef MT_FE_ATSC_H
#define MT_FE_ATSC_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtfe_common.h"

   
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/**The modulation type
*/
typedef enum
{
    MTFE_MOD_TYPE_BEGIN,
    MTFE_MOD_ATSC_8VSB = MTFE_MOD_TYPE_BEGIN,
    MTFE_MOD_J83B_64QAM,
    MTFE_MOD_J83B_256QAM,
    MTFE_MOD_J83B_AUTO_QAM,
    MTFE_MOD_TYPE_END
}MTFE_MOD_T; 

/**The spectrum inverse format
*/
typedef enum
{
    MTFE_SPEC_NORM,        // Spectrum normal
    MTFE_SPEC_INV          // Spectrum inverse
}MTFE_SPEC_T;


/**This structure contains the atsc demod parameter for connection.
*/
typedef struct
{
    UINT32                      u4Freq;             // RF center frequency in KHz
    //UINT32                      u4IF; 				//IF  frequency in KHz
    MTFE_MOD_T                  rModulation;        // Modulation type
    MTFE_RF_MOD_T               rRfMode;            // RF mode
    MTFE_SURF_MOD_T             rSurfMode;          // Surf mode
    MTFE_DTV_STATUS_T           rLockStatus;        // Lock status
    UINT8           u1StatusInd; //EC_code_dB809: Add enum for Demod failed ID
    //UINT8					 m_aucPara[30];     //EC_debug_DDI: dB513 for ATSC DDI porting
}MTFE_ATSC_PARAMETER_T;



/**This callback function for atsc demod module.
*/
typedef INT32 (*MTFE_ATSC_PFN_CB) (MTAL_FE_US_TUNER_CALLBACK_INFO_T* pvArg);

/**This structure contains the atsc demod parameter for connection.
*/
typedef struct
{
    UINT16  u2IF_Freq;       // IF frequency of tuner in kHz, usually it's around 44kHz 
    UINT8   u1IFEN_TARGET_LEVEL;         // AD target level
    UINT8   u1IFTARGETLevel_MSB;         // AD target level MSB
    UINT8   u1IFTARGETLevel_LSB;         // AD target level LSB
    //UINT8   u1IFAGCLevelMax;                /* This is only for EU RF tuner signal strength */
    //UINT8   u1IFAGCLevelMin;
    UINT8   u1IFAGC_SLP;
    UINT8   u1IFAGC_INT;
    UINT8   u1IFAGC_MAX;
    UINT8   u1IFAGC_MIN;
    UINT8   fgRFTuner;
    BOOL   bSpecPolarity; // Tuner IF Spectrum polarity, for default spectrum polarity reference in NA, iven 20110706
    UINT8   uSLD_DefaultPGAIndex; //default fixed PGA gain index for tuner matching, only fixed for legacy. iven 20110706
}MTFE_ATSC_TUNER_CONFIG_T; 

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** Initialize front end ATSC demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_Open(VOID);

/** Terminate front end ATSC demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_Close(VOID);

/** Start demod module to acquire to specified frequency and modulation type.
*@param  rParameter         Acquisition parameter (RF freq , modulation type,..).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_Connect(MTFE_ATSC_PARAMETER_T* prParameter);

/**Stop acquisition and put Demod into sleep mode.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_DisConnect(VOID);

/** Get demod lock status.
*@param	 prStatus           Report demod status.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_GetSync(MTFE_ATSC_PARAMETER_T* prStatus);

/** Get Signal power level in dBm.
*@param pi2SigLeveldBm      Signal power level in dBm.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_GetSignalLeveldBm(INT16 *pi2SigLeveldBm); 

/**Get packet error rate.
*@param pu4TotalPkt         Received total packets.
*@param pu4ErrPkt           Received error packets.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_GetPER(UINT32 *pu4TotalPkt, UINT32 *pu4ErrPkt);

/**Get SNR in dB.
*@param  pu2SignalSNR       Signal SNR in dB
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_GetSNR(UINT16 *pu2SignalSNR);

/**Get IF AGC Voltage in mV.
*@param  pu2IfAgcVol        IF AGC voltage in mV.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_GetIfAgcVol(UINT16 *pu2IfAgcVol);

/**Get TCM BER.
*@param  pu4TotalBit        Received total bits.
*@param  pu4ErrBit          Received error bits.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_GetTcmBER(UINT32 *pu4TotalBit, UINT32 *pu4ErrBit);

/**Set demod start/stop (fw) 
*@param fgStart		1(demod fw start acq) / 0(demod fw stop acq, back to FAT_Rst and wait)
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_DTD_Acquire(BOOL fgStart);

/**Set spectrum polarity (normal or inverse)
*@param	 fgDebug            1 (Manual. For debug) / 0 (Auto. Normal flow use).
*@param	 rSpcPolarity       Spectrum polarity.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_SetSpcPolarity(BOOL fgDebug, MTFE_SPEC_T rSpcPolarity);

/**Set transport stream directly out
*@param	 fgOn               ON (directly out for debug) / OFF (normal).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_SetTSDirectOut(BOOL fgOn);

/** Check demod software version.
*@param  pszVersion         Software Version.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_GetVer(UCHAR *pszVersion);

/** Setup IF frequency to demod.
*@param  u4IfFreq       value of IF frequency in KHz
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_SetIFFreq(UINT32 u4IfFreq);

/** Setup tuner confiuration to demod.
*@param  pTunerConfig       Config of Tuner
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_Tuner_Config(MTFE_ATSC_TUNER_CONFIG_T *pTunerConfig);

/**Get frequency offset.
*@param  offset             ---.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_GetFreqOffset(INT32* pFreqOffset);  

/** To control digital demod TunerBypass function switch ON or OFF
*@param  bSwitchOn       TRUE(TunerBypass ON) or FALSE(TunerBypass OFF)
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_DigTunerBypassI2C(BOOL bSwitchOn);

/**Register demod callback function.
*@param  pfCb               Callback function.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ATSC_RegCb(MTFE_ATSC_PFN_CB pfCb);

#ifdef __cplusplus
}
#endif

#endif //MT_FE_ATSC_H
