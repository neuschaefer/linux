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
 * $RCSfile: pd_anana_picommonapi.h,v $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_commonAPI.h
 *  This ltdis api declaration.
 */

#ifndef PD_ANANA_PICOMMONAPI_H
#define PD_ANANA_PICOMMONAPI_H
//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
//#include "ctx_tuner.h"
#include "pi_anana_if.h"
#include "pd_tuner_type.h"
#include "tuner_interface_if.h"
//typedef void (*Tuner_Initialize_fct) (GEN_TUNER_CTX_T* pTCtx);
//typedef void (*Tuner_SetSawBw_fct)   (GEN_TUNER_CTX_T *pTCtx, U8 sawbw);
//typedef S16  (*Tuner_SetFreq_fct)    (GEN_TUNER_CTX_T *pTCtx, U32 Freq, U8 Mode, U16 AutoSearch);
//typedef S16  (*Tuner_SetIFDemod_fct) (GEN_TUNER_CTX_T* pTCtx, U8 u1SubSysIdx);

//typedef S32 (*LTDIS_TunerClose_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx);

typedef BOOL  (*AnalogPINA_TunerInit_fct)         (ATV_PI_CTX_T *psAtvPiCtx, UINT8 IFDEMOD_I2c_Addr, UINT8 RF_I2c_Addr);
typedef VOID  (*AnalogPINA_TunerClose_fct)        (ATV_PI_CTX_T *psAtvPiCtx);
typedef BOOL  (*AnalogPINA_TunerSetFreq_fct)      (ATV_PI_CTX_T *psAtvPiCtx, PARAM_SETFREQ_T * param);
typedef UINT8 (*AnalogPINA_TunerGetSystem_fct)    (ATV_PI_CTX_T *psAtvPiCtx);
typedef BOOL  (*AnalogPINA_TunerSetSystem_fct)    (ATV_PI_CTX_T *psAtvPiCtx, UINT8 u1SubSysIdx);
typedef UINT8 (*AnalogPINA_TunerCheckVIFLock_fct) (ATV_PI_CTX_T *psAtvPiCtx,PARAM_SETFREQ_T * param);
typedef UINT8 (*AnalogPINA_TunerSearchNearbyFreq_fct) (ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, VOID *pvArg);
typedef UINT8 (*AnalogPINA_TunerMonitor_fct)      (ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, VOID *pvArg);
typedef UINT8 (*AnalogPINA_TunerGetVer_fct)       (ATV_PI_CTX_T *psAtvPiCtx);


typedef struct _ANANA_PICOMMONAPI_FCT_TBL_T
{
    AnalogPINA_TunerInit_fct            pf_AnalogPINA_TunerInit;
    AnalogPINA_TunerClose_fct           pf_AnalogPINA_TunerClose;
    AnalogPINA_TunerSetFreq_fct         pf_AnalogPINA_TunerSetFreq;
    AnalogPINA_TunerGetSystem_fct       pf_AnalogPINA_TunerGetSystem;
    AnalogPINA_TunerSetSystem_fct       pf_AnalogPINA_TunerSetSystem;
    AnalogPINA_TunerCheckVIFLock_fct    pf_AnalogPINA_TunerCheckVIFLock;
    AnalogPINA_TunerSearchNearbyFreq_fct  pf_AnalogPINA_TunerSearchNearbyFreq;
    AnalogPINA_TunerMonitor_fct         pf_AnalogPINA_TunerMonitor;
    AnalogPINA_TunerGetVer_fct          pf_AnalogPINA_TunerGetVer;
} ANANA_PICOMMONAPI_FCT_TBL_T;

//void Tuner_Initialize(GEN_TUNER_CTX_T *pTCtx);
//void Tuner_SetSawBw  (GEN_TUNER_CTX_T *pTCtx, U8 sawbw);
//S16  Tuner_SetFreq   (GEN_TUNER_CTX_T *pTCtx, U32 Freq, U8 Mode, U16 AutoSearch);
//S16  Tuner_SetIFDemod(GEN_TUNER_CTX_T* pTCtx, U8 u1SubSysIdx);

BOOL  AnalogPINA_TunerInit(ATV_PI_CTX_T *psAtvPiCtx, UINT8 IFDEMOD_I2c_Addr, UINT8 RF_I2c_Addr);
VOID  AnalogPINA_TunerClose(ATV_PI_CTX_T *psAtvPiCtx);
BOOL  AnalogPINA_TunerSetFreq(ATV_PI_CTX_T *psAtvPiCtx,PARAM_SETFREQ_T * param);
UINT8 AnalogPINA_TunerGetSystem(ATV_PI_CTX_T *psAtvPiCtx);
BOOL  AnalogPINA_TunerSetSystem(ATV_PI_CTX_T *psAtvPiCtx, UINT8 u1SubSysIdx);
UINT8 AnalogPINA_TunerCheckVIFLock(ATV_PI_CTX_T *psAtvPiCtx,PARAM_SETFREQ_T * param);
UINT8 AnalogPINA_TunerSearchNearbyFreq(ATV_PI_CTX_T *psAtvPiCtx,  UINT32 *wFreqInKHz, x_break_fct _BreakFct, VOID *pvArg);
UINT8 AnalogPINA_TunerMonitor(ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, VOID *pvArg);
UINT8 *AnalogPINA_TunerGetVer(ATV_PI_CTX_T *psAtvPiCtx);

#endif /* PD_ANAEU_PICOMMONAPI_H */
