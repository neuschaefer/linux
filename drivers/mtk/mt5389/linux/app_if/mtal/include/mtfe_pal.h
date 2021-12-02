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
 * $RCSfile: mtfe_pal.h,v $
 * $Revision: #3 $
 *---------------------------------------------------------------------------*/

/** @file mtfe_pal.h
 *  This header file declares exported APIs of FE PAL/SECAM module.
 */

#ifndef MTFE_PAL_H
#define MTFE_PAL_H

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

/**ATV mode.
*/

#ifndef FE_TUNER_COMMON_IF_H

typedef enum //liuqu,20091224
{
    MOD_TYPE_BEGIN = 0,
    MOD_ATSC_8VSB = MOD_TYPE_BEGIN,
    MOD_J83B_64QAM,
    MOD_J83B_256QAM,
    MOD_J83B_AUTO_QAM,
    MOD_DVBT,
	MOD_DVBT2,   //add by liuqu    
    MOD_ISDBT,
    MOD_DVBC,
	MOD_DTMB,
    MOD_TYPE_END,
    MOD_ANA_TYPE_BEGIN,
    MOD_PAL_BG = MOD_ANA_TYPE_BEGIN,
    MOD_PAL_DK,
    MOD_PAL_I,
    MOD_SECAM_L,
    MOD_SECAM_L1,
    MOD_NTSC_M,
    MOD_TYPE_UNKNOWN,
    MOD_ANA_TERR,
    MOD_ANA_CABLE,
    MOD_ANA_TYPE_END
}MOD_TYPE_T;

#endif

/*liuqu marked,change to new enum*/
typedef MOD_TYPE_T MTFE_PAL_MOD_T;

/**This structure contains the pal module parameter.
*/
typedef struct
{
    UINT32                  u4StartVFreqInKHz;      // Visual frequency searching start boundary in kHz, or desired visual frequency 
    UINT32                  u4StopVFreqInKHz;       // Visual frequency searching stop boundary in kHz
    MTFE_PAL_MOD_T          eAtvMode;               // Analog TV mode
    MTFE_SURF_MOD_T         eSurfMode;              // Surf mode
}MTFE_PAL_PARAMETER_T;

/**The pal module callback function type ID
*/
enum
{
    MTFE_PAL_CB_ID_SEARCH_NEARBY_NFY = 0,
    MTFE_PAL_CB_ID_TVD_LOCK_STATUS_NFY = 1,
    MTFE_PAL_CB_ID_MAX_SIZE
};


/**This structure contains the search nearby result parameter.
*/
typedef struct
{
    UINT32                  u4VFreqInKHz;            //RF visual frequency in KHz
    MTFE_ATV_STATUS_T       rVIFStatus;              //VIF Status
    MTFE_ATV_TVD_STATUS_T   rTVDStatus;              //TVD Status
}MTFE_PAL_SEARCH_PARAMETER_T;


/**The pal module callback function.
*/
typedef INT32     (*MTFE_PAL_PFN_CB)(UINT8 u1CbId, VOID *pvArg);


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** Initialize frond end pal module.
*@param VOID
*@retval MT_OK                  Success.
*@retval MT_NOT_OK              Fail.
*/
EXTERN MT_RESULT_T MTFE_PAL_Open(VOID);

/** Terminate frond end pal module.
*@param VOID
*@retval MT_OK                  Success.
*@retval MT_NOT_OK              Fail.
*/
EXTERN MT_RESULT_T MTFE_PAL_Close(VOID);

/** Connect frond end pal module.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_PAL_Connect(VOID); 

/** Disconnet frond end pal module.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
EXTERN MT_RESULT_T MTFE_PAL_DisConnect(VOID); 

/**Check TVD lock statu.
*@retval MT_OK                  Success.
*@retval MT_NOT_OK              Fail.
*/
EXTERN MT_RESULT_T MTFE_PAL_TunerCheckTVDLock(VOID); 



EXTERN MT_RESULT_T MTFE_PAL_TunerAutoSearch(UINT32 IsAutoSearch); 


/**Set visual carrier frequency.
*@param  rParameter             The input paramter (Visual carrier freq...)
*@retval MT_OK                  Success.
*@retval MT_NOT_OK              Fail.
*/
EXTERN MT_RESULT_T MTFE_PAL_TunerSetFreq(MTFE_PAL_PARAMETER_T *prParameter); 

/**Search visual carrier frequency. 
* Registration a notify function (ID = MTFE_PAL_CB_ID_SEARCH_NEARBY_NFY) before using this API.
*@param  rParameter             The input paramter (Visual carrier freq...)
*@retval MT_OK                  Success.
*@retval MT_NOT_OK              Fail.
*/
EXTERN MT_RESULT_T  MTFE_PAL_StartSearchNearbyFreq(MTFE_PAL_PARAMETER_T *prParameter);

/**Break the search visual carrier frequency process.
*@retval MT_OK                  Success.
*@retval MT_NOT_OK              Fail.
*/
EXTERN MT_RESULT_T  MTFE_PAL_BreakSearchNearbyFreq(VOID);

/**Register pal module callback function.
*@param  CbId               Callback function ID.
*@param  pfCb               Callback function.
*@param  vArg               The argument of callback function.
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*/
//EXTERN MT_RESULT_T MTFE_PAL_RegCb(UINT8 u1CbId, MTFE_PAL_PFN_CB pfCb, VOID *pvArg);
EXTERN MT_RESULT_T MTFE_PAL_RegCb(MTFE_CB_FCT pfCb);

/**Get Tuner boundary frequency.
*@param  pu4UpperFreq   Upper frequency boundary
*@param pu4LowerFreq   Lower frequency boundary
*@retval MT_OK              Success.
*@retval MT_NOT_OK          Fail.
*@retval MTR_NOT_SUPPORTED      Not supported 
*/
EXTERN MT_RESULT_T MTFE_PAL_GetTunerFreqBound(UINT32 *pu4UpperFreq,UINT32 *pu4LowerFreq);
/** Bypass  audio SIF signal  to audio DSP via Demod ADC
*@param u4AudSys        current audio system.For ATV,such as MOD_PAL_BG,MOD_PAL_DK,...etc.Please refer enumuration type MTFE_PAL_MOD_T in this file
*@retval MTR_OK         Success.
*@retval MTR_NOT_OK          Fail.
*@retval MTR_NOT_SUPPORTED      Not supported 
*/
EXTERN MT_RESULT_T MTFE_PAL_BypassSIF(UINT32 u4AudSys);
EXTERN MT_RESULT_T MTFE_PAL_GetTunerLockStatus(INT16* plockstatus);

EXTERN MT_RESULT_T MTFE_PAL_GetTVsystem(MTFE_PAL_MOD_T *pSystem);

EXTERN MT_RESULT_T MTFE_PAL_SetTVsystem(MTFE_PAL_MOD_T *pSystem);

EXTERN MT_RESULT_T MTFE_PAL_SetAFT(BOOL *pfgEnable);   /*wenming 2011-10-11:for UT to enable or disable AFT*/

#ifdef __cplusplus
}
#endif

#endif //MTFE_PAL_H
