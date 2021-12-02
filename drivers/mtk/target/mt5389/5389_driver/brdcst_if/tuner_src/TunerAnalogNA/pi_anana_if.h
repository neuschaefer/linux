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
 * $RCSfile: pi_anana_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_anana_if.h 
 *  NA tuner analog pi primitive API 
 */

#ifndef PI_ANANA_IF_H
#define PI_ANANA_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "fe_tuner_common_if.h"
#include "tuner_interface_if.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define mcMSG_IN_SEARCHNEARBY_AUTO_TEST(LO_STEPSIZE) if(1==fgEnableATP){mcDELAY_MS(200);mcSHOW_DBG_MSG(("##Start\n##Freq : 0x%X\n##Noise : 0x0%X\n##ChromaGain : %d\n##Video : %s\n##CHScanTime : %3u\n##End\n",((*wFreqInKHz+45750) * 100)/(LO_STEPSIZE),IO_READ8(0x20022000, 0x80),IO_READ8(0x20022000, 0x9D) & 0x01,(((IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) >> 4)?"On":"Off"),(mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));}
#define mcMSG_IN_MONITOR_AUTO_TEST(LO_STEPSIZE) if(1==fgEnableATP){if((IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) >> 4){mcSHOW_DBG_MSG2(("##Start \n##Freq : 0x%X\n##Video : On\n##ChromaGain : %d\n##End\n",((Freq+45750) * 100)/(LO_STEPSIZE),IO_READ8(0x20022000, 0x9D) & 0x01)); }}


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
//#ifndef PATD_SPECIFIC_CTX
typedef VOID *PATD_SPECIFIC_CTX;
//#endif

#ifndef tuner_break_fct
#define tuner_break_fct
typedef INT32     (*x_break_fct)(void *pArg);
#endif

typedef struct _ATV_PI_CTX_T
{
// Hardware Configure
    UINT8       u1TunerType;
    UINT8       u1RF_I2cAddr;
    UINT8       u1IF_I2cAddr;
    UINT16      u2IF_Freq;
    UINT16      u2LO_Step;
    UINT32      u4Freq;
    UINT8       u1DEMOD_state;
    UINT8       u1AnaMode;
    UINT8       u1VIFLock;
    UINT8       u1AnaDemoTop;//for MW set to 9886 top tune
//    CHAR        aSW_Ver_HW_Ver[24];
// Access Link
    VOID        *pDigiTunerCtx;
    x_break_fct isBreak;
//    VOID        *isBreak;
    VOID        *pvBreakArg;

//jackson move this, do not need this anymore
//    struct _ANA_TUNER_FUNC_T    *psTunerFunc;

    // TV Color / Sound Sub-System
//#if defined(TUNER_SUPPORT_MULTI_SYS_TV) || 1
//    ATV_SUB_SYS_CTX_T   sAtvSubSysCtx;
    UINT8       u1SubSysIdx;
    UINT32      u4TvSubSysMask;
    UINT8       ucTvSysGroupNum;
    UINT32      *pu4TvSysGroup;
    VOID        *psTunerAnaAttr;
//#endif
// ATD PI Specific context
//#if fcNEW_ANA_PIPD_ARCH
    PATD_SPECIFIC_CTX   ptATDSpecificCtx;
//#endif
} ATV_PI_CTX_T, *PATV_PI_CTX_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
/** fgDrvTunerCheckTVDLock
 *  check whether tv decoder can  decode  IF signal or not 
 *  @param  bTryLoopCnt  Re-check TVD times if TVD unlock
 *  @retval TRUE         TVD lock signal(CVBS)
 *  @retval FALSE        TVD can't lock signal(CVBS)
 */
 //----------------------------------------------------------------------------
EXTERN BOOL fgDrvTunerCheckTVDLock( UINT16 bTryLoopCnt );
//----------------------------------------------------------------------------
/** AnalogPINA_TunerSetFreq
 *  Set Tuner LO tune to specified frequency
 *  @param  psAtvPiCtx     pointer of tuner device context
 *  @param  wFreqInKHz   RF frequency in KHz.
 *  @param  u1AnaMode   Modulation Type
 *  @retval  0            Set tuner successfully
 *  @retval  1             Set tuner set fail  
 */ 
 //----------------------------------------------------------------------------
EXTERN BOOL  AnalogPINA_TunerSetFreq(ATV_PI_CTX_T *psAtvPiCtx,PARAM_SETFREQ_T * param);
//----------------------------------------------------------------------------- 
/**AnalogPINA_TunerCheckVIFLock
 *  Check if programmed frequency has strong VIF signal nearby
 *  @param  psAtvPiCtx    	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  wFreqInKHz          The frequenccy in KHz need to check the VIF status
 *  @retval   	 2  => Current frequency is very close to the nominal VIF.
           		 1  => Current frequency is is near the nominal VIF.
           		 0  => Current frequency is not close to the nominal VIF yet.
 */
//-----------------------------------------------------------------------------
EXTERN UINT8 AnalogPINA_TunerCheckVIFLock(ATV_PI_CTX_T *psAtvPiCtx,PARAM_SETFREQ_T * param);

//AnalogPINA_TunerSearchNearbyFreq return value, bit 
#define cpANA_LOCK_STS_VIF      0 //bit 0
#define cpANA_LOCK_STS_TVD      1 //bit 1
#define cpANA_LOCK_STS_BREAK    2 //bit 2
//----------------------------------------------------------------------------- 
/** AnalogPINA_TunerSearchNearbyFreq
 *  Find the freq. value that near center frequency of the channel.
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  *wFreqInKHz  	THe point of frequenccy in KHz need to check the VIF status
 *  @param  _BreakFct  	 	The break function
 *  @param  *pvArg  		The argument of break function        
 *  @retval  bit0           1:VIF lock, 0: VIF unlock
 *  @retval  bit1           1:TVD lock, 0: TVD unlock
 *  @retval  bit2           1:break happen, 0: no break
 */
//-----------------------------------------------------------------------------
EXTERN UINT8 AnalogPINA_TunerSearchNearbyFreq(ATV_PI_CTX_T *psAtvPiCtx,  UINT32 *wFreqInKHz, x_break_fct _BreakFct, VOID *pvArg);
//------------------------------------------------------------------------------
/**  AnalogPINA_TunerMonitor
 *  Find the freq. value that near center frequency of the channel.
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  *wFreqInKHz  	THe point of frequenccy in KHz need to check the VIF status
 *  @param  _BreakFct  	 	The break function
 *  @param  *pvArg  		The argument of break function
 *  @retval   TRUE  =>  center freq. found
            	 FALSE =>  center freq. not found (no signal on this channel)
 */
//------------------------------------------------------------------------------
EXTERN UINT8 AnalogPINA_TunerMonitor(ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, VOID *pvArg);
#endif /* PI_ANANA_IF_H */

