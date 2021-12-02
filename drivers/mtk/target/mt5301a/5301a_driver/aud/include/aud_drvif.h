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
 * $RCSfile: aud_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_drvif.h
 *  Brief of file aud_drvif.h.
 *  Details of file aud_drvif.h (optional).
 */
 
#ifndef AUD_DRV_IF_H
#define AUD_DRV_IF_H

#include "dmx_if.h"
#include "aud_if.h"

/****************************************************************************
** Type definitions
****************************************************************************/

/****************************************************************************
** Constant definitions
****************************************************************************/

#define DSP_BUSY  ((INT32)  1)
#define DSP_OK    ((INT32)  0)
#define DSP_FAIL  ((INT32) -1)
/****************************************************************************
** Export API
****************************************************************************/

extern void AUD_AtvAvInitialSync(BOOL fgStable);
extern void AUD_WaitDspFlush(UINT8 u1DecId);
extern void AUD_TriggerDspPlay(UINT8 u1DecId);
extern void Aud_SetTvSysMask(UINT8 u1DecId, UINT32 u4Mask);

extern void AUD_GetRWPtr(UINT8 u1DecId, UINT32 * pu4Rp, UINT32 * pu4Wp, UINT32 *pu4Size);
extern INT32 AUD_GetAudFifo(UINT32 * pu4Fifo1Start, UINT32 * pu4Fifo1SEnd,UINT32 * pu4Fifo2Start,UINT32 * pu4Fifo2End);
extern BOOL AUD_IsFifoEmpty(UINT8 u1DecId);

extern BOOL AUD_GetAinCfg(AUD_DEC_STREAM_FROM_T eStreamFrom, AIN_CFG_T* prAinCfg);
extern void AUD_HdmiModeMute(UINT8 u1DecId, BOOL fgMute);
extern void AUD_AoutDacFs(UINT8 u1DecId, SAMPLE_FREQ_T eFs);
extern void AUD_AinCfg(AUD_DEC_STREAM_FROM_T eStrSrc, const AIN_CFG_T* prAinCfg);
extern void AUD_AtunerSigStableNotify(BOOL fgSatble);
extern BOOL fgAudStatistics(void);
extern BOOL AUD_IsFifoFull(UINT8 u1DecId);
extern INT32 AUD_SetAudFifo(UINT8 u1DecId, UINT32  u4Address, UINT32 u4Length);
extern void AUD_MMAoutEnable(UINT8 u1DecId, BOOL fgEnable);
extern void AUD_ClearEOSFlag(UINT8 u1DecId);
extern BOOL _AudFMRadioDetection(void);
extern BOOL _AudPlayFMRadio(void);
extern UINT32  AUD_GetEmptyRetryThres(void);

#ifdef CC_ENABLE_AV_SYNC
extern void AUD_DrvSetChDelay(UINT8 u1Delay);
extern void AUD_VdpModeChange(void);
extern void AUD_VdpModeChangeDone(void);
#endif

#ifdef __MODEL_slt__
extern void AUD_SltLineInInit(void);
extern void SLT_FlashingAudInit(void);
extern INT32 SLT_SortingAud(void);
#endif

#endif /* AUD_DRV_IF_H */

