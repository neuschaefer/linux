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
 * $RCSfile: mtfe_isdb.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mtfe_isdb.h
 *  This header file declares exported APIs of frond end ISDB demod module.
 */

#ifndef MT_FE_ISDB_H
#define MT_FE_ISDB_H

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
    MTFE_MOD_ISDB_TYPE_BEGIN,
    MTFE_MOD_ISDBT_QPSK = MTFE_MOD_ISDB_TYPE_BEGIN,
    MTFE_MOD_ISDBT_16QAM,
    MTFE_MOD_ISDBT_64QAM,
    MTFE_MOD_ISDBT_DQPSK,
    MTFE_MOD_ISDBS_8PSK,
    MTFE_MOD_ISDBS_QPSK,
    MTFE_MOD_ISDBS_BPSK,
    MTFE_MOD_ISDB_TYPE_END
}MTFE_ISDB_MOD_T; 

/**The spectrum inverse format
*/
typedef enum
{
    MTFE_ISDB_SPEC_NORM,        // Spectrum normal
    MTFE_ISDB_SPEC_INV          // Spectrum inverse
}MTFE_ISDB_SPEC_T;

/**The TunerID Type
*/
typedef enum
{
    MTFE_TUNER_OFDM1 = 0,        // Tuner + Demod OFDM1
    MTFE_TUNER_OFDM2,              // Tuner + Demod OFDM2
    MTFE_TUNER_PSK1,                  // Tuner + Demod PSK1
    MTFE_TUNER_PSK2                   // Tuner + Demod PSK2
}MTFE_TUNER_ID_T;

typedef enum
{
    MTFE_TS_SERIAL_IO_B = 0,     // System B TS pins
    MTFE_TS_SERIAL_IO_C            // System C TS pins
}MTFE_TS_IO_T;

/**********************************************
**                ISDB-T Tuner LNA Status                       **
**********************************************/
typedef enum
{
    MTFE_LNA_OFF = 0,                // LNA disable (Default)
    MTFE_LNA_ON                         // LNA enable
}MTFE_LNA_STS_T;

/**********************************************
**                ISDB-S Tuner LNB Status                       **
**********************************************/
typedef enum
{
    MTFE_LNB_OFF = 0,                // LNB disable (Default)
    MTFE_LNB_VOLT_11V,            // LNB Voltage is 11V
    MTFE_LNB_VOLT_15V             // LNB Voltage is 15V 
}MTFE_LNB_STS_T;

/**This structure contains the isdb demod parameter for connection.
*/
typedef struct
{
    UINT32                         u4Freq;              // RF center frequency in KHz
    UINT16                         u2TsID;              // TS ID
    MTFE_TUNER_ID_T        rTunerID;           // Tuner ID for Demod+Tuner
    MTFE_TS_IO_T              rTsIO;                // Set the TS Serial Output System (B or C)
    UINT8                           u1IFAGC;           // IF AGC Control Word   
    UINT32                         u4PER;               // Packet Error Rate,  Unit:1e-8
    UINT32                         u4BER;               // Bit Error Rate,       Unit:1e-8
    UINT32                         u4CNR;               // Carrier to Noise Ratio, Unit:1e-2
    UINT32                         u4SigLevel;        // Signal Level, Calculate from the IF AGC
    MTFE_ISDB_MOD_T       rModulation;      // Modulation type
    MTFE_RF_MOD_T           rRfMode;            // RF mode
    MTFE_SURF_MOD_T       rSurfMode;         // Surf mode
    MTFE_DTV_STATUS_T    rLockStatus;       // Lock status
    MTFE_LNA_STS_T          rLNASts;              // ISDB-T Tuner LNA Status (Default: disable) 
    MTFE_LNB_STS_T          rLNBSts;              // ISDB-S Tuner LNB Status (Defualt: disable) 
}MTFE_ISDB_PARAMETER_T, MTFE_ISDBT_PARAMETER_T, MTFE_ISDBS_PARAMETER_T;

/**This callback function for isdb demod module.
*/
typedef INT32 (*MTFE_ISDB_PFN_CB) (MTAL_FE_JAPAN_TUNER_CALLBACK_INFO_T* pvArg);


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** Initialize front end ISDB demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_Open(VOID);

/** Terminate front end ISDB demod module.
*@param VOID
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_Close(VOID);

/** Start demod module to acquire to specified transport ID.
*@param  prPara        Acquisition parameter (RF freq , modulation type,..).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_SetTSID(MTFE_ISDB_PARAMETER_T *prPara);

/** Start ISDB-T demod module to set the Tuner LNA Status.
*@param  prPara        Parameter (LNA Status, ... ).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_SetLNA(MTFE_ISDB_PARAMETER_T *prPara);

/** Start ISDB-S demod module to set the Tuner LNB Status.
*@param  prPara        Parameter (LNB Status, ... ).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_SetLNB(MTFE_ISDB_PARAMETER_T *prPara);

/** Start demod module to acquire to specified frequency and modulation type.
*@param  prPara         Acquisition parameter (RF freq , modulation type,..).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_Connect(MTFE_ISDB_PARAMETER_T *prPara);

/**Stop acquisition and put Demod into sleep mode.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_DisConnect(MTFE_ISDB_PARAMETER_T *prPara);

/** Start demod module to acquire to specified frequency.
*@param  prPara        Acquisition parameter (RF freq).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_SetFreq(MTFE_ISDB_PARAMETER_T *prPara);

/**Get packet error rate.
*@param  prPara        Acquisition parameter (PER).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_GetPER(MTFE_ISDB_PARAMETER_T *prPara);

/**Get BER.
*@param  prPara        Acquisition parameter (BER).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_GetBER(MTFE_ISDB_PARAMETER_T *prPara);

/**Get CNR in dB.
*@param  prPara        Acquisition parameter (CNR).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_GetCNR(MTFE_ISDB_PARAMETER_T *prPara);

/** Get demod lock status.
*@param	 prPara           Report demod status.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_GetSync(MTFE_ISDB_PARAMETER_T *prPara);

/**Get IF AGC control word from the Demod.
*@param  prPara        Acquisition parameter (IF AGC).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_GetIfAgc(MTFE_ISDB_PARAMETER_T *prPara);

/** Get Signal power level in dBm.
*@param  prPara        Acquisition parameter (IF AGC dBm).
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_GetSignalLeveldBm(MTFE_ISDB_PARAMETER_T *prPara); 

/** Check demod software version.
*@param  prPara        Acquisition parameter (IF AGC dBm).
*@param  pszVersion         Software Version.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_GetVer(MTFE_ISDB_PARAMETER_T *prPara, UCHAR *pszVersion);

/**Register demod callback function.
*@param  pfCb               Callback function.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_ISDB_RegCb(MTFE_ISDB_PFN_CB pfCb);

#ifdef __cplusplus
}
#endif

#endif //MT_FE_ISDB_H
