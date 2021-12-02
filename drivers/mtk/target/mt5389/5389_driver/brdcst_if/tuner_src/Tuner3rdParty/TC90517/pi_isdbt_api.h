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
 * $RCSfile: pi_isdbt_api.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef PI_DEMOD_ISDBT_IF
#define PI_DEMOD_ISDBT_IF

#include "tuner_interface_if.h"
#include "PI_Def.h"
#include "pi_demod.h"

//----------------------------------------------------------------------------- 
/** ISDBT_Init
 *  Initialization of ISDB-T Demod.
 *  @param   pSBTVD_DEMOD_CTX_T  Pointer of Demod context.
 *  @retval  TRUE : success.
 *  @retval  FALSE : fail.   
 */
//-----------------------------------------------------------------------------
EXTERN BOOL ISDBT_Init(SBTVD_DEMOD_CTX_T * pSBTVD_DEMOD_CTX_T);

//----------------------------------------------------------------------------- 
/** ISDBT_Reset
 *  Demodulation reset that resets the demodulation synchronization sequence.
 *  @retval  TRUE : success.
 *  @retval  FALSE : fail.    
 */
//-----------------------------------------------------------------------------
EXTERN BOOL ISDBT_Reset(VOID);

//----------------------------------------------------------------------------- 
/** ISDBT_ScanInit
 *  Channel scan init, fast chnnel search by SP.
 *  @retval  TRUE : success.
 *  @retval  FALSE : fail.    
 */
//-----------------------------------------------------------------------------
EXTERN BOOL ISDBT_ScanInit(VOID);

//----------------------------------------------------------------------------- 
/** ISDBT_TuningInit
 *  Channel change init, fast chnnel search by SP.
 *  @retval  TRUE : success.
 *  @retval  FALSE : fail.    
 */
//-----------------------------------------------------------------------------
EXTERN BOOL ISDBT_TuningInit(VOID);

//----------------------------------------------------------------------------- 
/** ISDBT_Connect
 *  Wake up the ISDB-T Demodulation.
 *  @retval  TRUE : success.
 *  @retval  FALSE : fail.    
 */
//-----------------------------------------------------------------------------
EXTERN  BOOL ISDBT_Connect(VOID);

//----------------------------------------------------------------------------- 
/** ISDBT_Sleep
 *  Sleep the ISDB-T Demodulation.
 *  @retval  TRUE : success.
 *  @retval  FALSE : fail.    
 */
//-----------------------------------------------------------------------------
EXTERN BOOL ISDBT_Sleep(VOID);

//----------------------------------------------------------------------------- 
/** ISDBT_GetLockSts
 *  Get the ISDB-T Demod Lock Status.
 *  @retval  1 : Lock.
 *  @retval  other : Unlock.    
 */
//-----------------------------------------------------------------------------
EXTERN INT8 ISDBT_GetLockSts(VOID);

//----------------------------------------------------------------------------- 
/** ISDBT_GetTuningSts
 *  Get the ISDB-T Demod channel change Status.
 *  @retval  1 : Lock.
 *  @retval  other : Unlock.    
 */
//-----------------------------------------------------------------------------
EXTERN INT8 ISDBT_GetTuningSts(VOID);

//----------------------------------------------------------------------------- 
/** ISDBT_TimeOut
 *  Check if the retry count is greater than the setting when channel scan
 *  @retval  1 : Time out.
 *  @retval  other : Not time out.    
 */
//-----------------------------------------------------------------------------
EXTERN INT8 ISDBT_TimeOut(VOID);

//----------------------------------------------------------------------------- 
/** ISDBT_GetSignalLevel
 *  Get the signal level, computed by the IF AGC value
 *  @retval  value [0~100]
 */
//-----------------------------------------------------------------------------
EXTERN INT8 ISDBT_GetSignalLevel(VOID);

//----------------------------------------------------------------------------- 
/** ISDBT_GetSigCnr
 *  Get signal Carrier to Noise Ratio (CNR), Units: 0.01 .
 *  @param   psDemodCtx  Pointer of Demod context.
 *  @retval  CNR
 *   if return value is 0, the Demod unsync or I2c Failed
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 ISDBT_GetSigCnr(VOID);

//----------------------------------------------------------------------------- 
/** ISDBT_SetFreq
 *  Set the tuner freq to the specified freq
 *  @param  param, Pointer of tuner param context.
 *  @retval  1: Success
 *  @retval  0: unlock
 *  @retval  -1: I2c fail
 */
//-----------------------------------------------------------------------------
EXTERN INT8 ISDBT_SetFreq(PARAM_SETFREQ_T  * param);

//----------------------------------------------------------------------------- 
/** ISDBT_GetBer
 *  Get the signal BER/PER measurement
 *  @param   pSBTVD_DEMOD_CTX_T  Pointer of Demod context.
 *  @param  fgBer, 0:PER measurement, 1:BER measurement.
 *  @retval  TRUE: Success
 *  @retval  FALSE: Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL ISDBT_GetBer(SBTVD_DEMOD_CTX_T * pSBTVD_DEMOD_CTX_T,BOOL fgBer);
#ifdef ENABLE_DEMOD_PRESET

//----------------------------------------------------------------------------- 
/** StoreTMCCData
 *  Get and save the TMCC status data from the demod
 *  @param   pSBTVD_DEMOD_CTX_T  Pointer of Demod context.
 *  @retval  TRUE: Success
 *  @retval  FALSE: Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL StoreTMCCData(SBTVD_DEMOD_CTX_T * pSBTVD_DEMOD_CTX_T);

//----------------------------------------------------------------------------- 
/** LoadTMCCData
 *  Write the TMCC status data to the demod
 *  @param   pSBTVD_DEMOD_CTX_T  Pointer of Demod context.
 *  @retval  TRUE: Success
 *  @retval  FALSE: Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL LoadTMCCData(SBTVD_DEMOD_CTX_T * pSBTVD_DEMOD_CTX_T);

//----------------------------------------------------------------------------- 
/** vCleanTMCCData
 *  Clean the TMCC Data, Set the fgValid to FALSE
 *  @param   pSBTVD_DEMOD_CTX_T  Pointer of Demod context.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
EXTERN VOID vCleanTMCCData(SBTVD_DEMOD_CTX_T * pSBTVD_DEMOD_CTX_T);

//----------------------------------------------------------------------------- 
/** fgTMCCDataAval
 *  Get  the TMCC status data if is valid
 *  @param   pSBTVD_DEMOD_CTX_T  Pointer of Demod context.
 *  @retval  TRUE: Valid
 *  @retval  FALSE: Unvalid
 */
//-----------------------------------------------------------------------------
EXTERN BOOL fgTMCCDataAval(SBTVD_DEMOD_CTX_T * pSBTVD_DEMOD_CTX_T);

//----------------------------------------------------------------------------- 
/** vShowTMCCData
 *  Show  the TMCC status data
 *  @param   pSBTVD_DEMOD_CTX_T  Pointer of Demod context.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
EXTERN VOID vShowTMCCData(SBTVD_DEMOD_CTX_T * pSBTVD_DEMOD_CTX_T);
#endif

//----------------------------------------------------------------------------- 
/** ISDBT_GetEWSInfo
 *  Get the EWS(Emergency Warning System) flag
 *  @param   pEwsInfo Pointer of EWS information context.
 *  @retval  TRUE: Success
 *  @retval  FALSE: Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL ISDBT_GetEWSInfo(EWS_INFO* pEwsInfo);

//----------------------------------------------------------------------------- 
/** ISDBT_GetTSFMT
 *	Get the TS format
 *	@param pTsfmt Pointer of TS format context.
 *	@retval  TRUE: Success
 *	@retval  FALSE: Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL ISDBT_GetTSFMT(TS_FMT_T * pTsfmt);
#endif


