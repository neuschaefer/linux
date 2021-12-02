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
#ifndef PI_ATBM_COMMON_API_EXP_H
#define PI_ATBM_COMMON_API_EXP_H

#include "PI_Def.h"
#include "pi_atbm8846e_common_def.h"
#include "tuner_interface_if.h"

EXTERN BOOL ATBMWrite(UINT8 u1I2cAddr,UINT16 u2RegAddr, UINT8 data);
EXTERN BOOL ATBMRead(UINT8 u1I2cAddr,UINT16 u2RegAddr, UINT8 * data);
EXTERN BOOL ATBMWriteDemodData(UINT8 u1I2cAddr,pDEMOD_I2C_DATA pData,UINT8 u1Len);
EXTERN BOOL ATBMGetSync(UINT8 u1I2cAddr,BOOL * fgLock);
EXTERN BOOL ATBM_Standby(UINT8 u1I2cAddr);
EXTERN BOOL ATBM_WakeUp(UINT8 u1I2cAddr);
EXTERN BOOL ATBM_BypassI2C(UINT8 u1I2cAddr,BOOL fgBypass);
EXTERN BOOL ATBM_Init(UINT8 u1I2cAddr);
EXTERN BOOL ATBMGetSigLvl(UINT8 u1I2cAddr,MOD_TYPE_T mode,UINT8 * pu1SigLvl);
//EXTERN BOOL ATBM_GetTPSIndex(UINT8 u1I2cAddr,UINT8* u1TPSIndex);
EXTERN BOOL ATMB_SetFreq(UINT8 u1I2cAddr,PARAM_SETFREQ_T * pPara);
//EXTERN UINT16 log2(UINT32 u4Data);
EXTERN UINT32 Log10_Convert(UINT32 InValue);
EXTERN BOOL ATBM_TimeOut(UINT8 u1I2cAddr,BOOL* fgTimeOut);
EXTERN BOOL ATBMGetTsFMT(UINT8 u1I2cAddr,TS_FMT_T * pTsFmt);
EXTERN BOOL ATBMIsDemodOk(UINT8 u1I2cAddr,BOOL * fgNeedReset);
EXTERN VOID ATBMDoHWReset(VOID);
EXTERN VOID ATBM_SwitchIfAGC(UINT8 u1I2cAddr,BOOL fgAnalog);
#endif
