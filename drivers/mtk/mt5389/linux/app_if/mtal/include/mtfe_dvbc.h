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
 * $RCSfile: mtfe_dvbc.h,v $
 * $Revision: #3 $
 *---------------------------------------------------------------------------*/

/** @file mtfe_dvbc.h
 *  This header file declares exported APIs of DVBC module.
 */

#ifndef MT_FE_DVBC_H
#define MT_FE_DVBC_H

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
    MTFE_MOD_QAM_16 =0,
    MTFE_MOD_QAM_32,
    MTFE_MOD_QAM_64,    
    MTFE_MOD_QAM_128,    
    MTFE_MOD_QAM_256,
    MTFE_MOD_QAM_AUTOQAM,
    MTFE_MTFE_DVBT_QAM_RESERVED
}   MTFE_DVBC_CONSTELLATION_T;

/**This structure contains the dvbt demod parameter for connection.
*/
typedef struct
{
    UINT32                   ui4_freq;             // RF center frequency in KHz 
    UINT32                   ui4_sym_rate;
	UINT32                   ui4_IF;			  // IF setting in KHz
	MTFE_DVBC_CONSTELLATION_T       e_mod;	
    MTFE_SURF_MOD_T             eSurfMode;          // Surf mode    
    UINT8					 m_aucPara[30];
    UINT8                   ui4_QSpecInv;    // spectrum inverse flag
}MTFE_DVBC_PARAMETER_T;

/**This callback function for dvbc demod module.
*/
typedef VOID (*MTFE_DVBC_PFN_CB) (UINT8 u1CbId, VOID* pvArg);

/**This structure contains the dvbc demod parameter for connection.
*/
typedef struct
{
    UINT16  u2IF_Freq;
    UINT8   u1IFAGCLevelMax;
    UINT8   u1IFAGCLevelMin;
    UINT8   u1IFAGC_SLP;
    UINT8   u1IFAGC_INT;
    UINT8   u1IFAGC_MAX;
    UINT8   u1IFAGC_MIN;
//    UINT8   u1IFAGCTargetLevel;
}MTFE_DVBC_TUNER_CONFIG_T;



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** Initialize front end DVB-T demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_Open(VOID);

/** Terminate front end DVB-T demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_Close(VOID);

/** Start demod module to acquire to specified frequency and bandwidth...
*@param  rParameter         Acquisition parameter (RF freq , bandwidth,..).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_Connect(MTFE_DVBC_PARAMETER_T *prParameter);

/**Stop acquisition and put Demod into sleep mode.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_DisConnect(VOID);

/** Get demod lock status.
*@param	 pi2LockStatus      Lock / unlock.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_GetSync(INT32 *pi2LockStatus);

/** Get Signal power level in percentage.
*@param pi2SigLevel         Signal power level in percentage.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_GetSignalLevel(INT32 *pi2SigLevel); 

/**Get packet error rate.
*@param pu4TotalPkt         Received total packets.
*@param pu4ErrPkt           Received error packets.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_GetPER(UINT32 *pu4TotalPkt, UINT32 *pu4ErrPkt);

/**Get SNR in dB.
*@param u2SignalSNR         Signal SNR
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_GetSNR(UINT16 *pu2SignalSNR);

/**Get IF AGC Voltage in mV.
*@param  pu2IfAgcVol        IF AGC voltage in mV.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_GetIfAgcVol(UINT16 *pu2IfAgcVol);

/**Get Post Viterbi BER and Pre Viterbi BER.
*@param  pi4PostVBer        Post Viterbi BER * 10^5.
*@param  pi4PreVBer         Pre  Viterbi BER * 10^5.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_GetBER(INT32 *pi4PostVBer, INT32 *pi4PreVBer);

/**Get frequency offset.
*@param  offset             ---.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_GetFreqOffset(UINT32* offset);

/** Check demod software version.
*@param  u1Version          Software Version.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_GetVer(UCHAR *pszVersion);


/**Register demod callback function.
*@param  CbId               Callback function ID.
*@param  pfCb               Callback function.
*@param  vArg               The argument of callback function.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
//EXTERN MT_RESULT_T MTFE_DVBC_RegCb(UINT8 u1CbId, MTFE_DVBC_PFN_CB pfCb, VOID* pvArg);

EXTERN MT_RESULT_T MTFE_DVBC_RegCb(MTFE_CB_FCT pfCb);

/** Setup tuner confiuration to demod.
*@param  pTunerConfig       Config of Tuner
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_DVBC_Tuner_Config(MTFE_DVBC_TUNER_CONFIG_T *pTunerConfig);


/**set demod TS out mode.
*@param  fgParallel         TRUE:Parallel;   FALSE:serial
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
//MT_RESULT_T MTFE_DVBC_RegCb(UINT8 u1CbId, MTFE_DVBC_PFN_CB pfCb, VOID* pvArg)
EXTERN MT_RESULT_T MTFE_DVBC_SetTSInterface(BOOL *fgParallel);


/**set demod TS Path.
*@param  fgEnable            TRUE:Enable;   FALSE:Disable
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/

EXTERN MT_RESULT_T MTFE_DVBC_SetTSPath(BOOL *fgEnable);

EXTERN MT_RESULT_T MTFE_DVBC_GetTPSInfo(MTFE_DVBC_PARAMETER_T *prParameter);


#ifdef __cplusplus
}
#endif

#endif //MT_FE_DVBC_H
