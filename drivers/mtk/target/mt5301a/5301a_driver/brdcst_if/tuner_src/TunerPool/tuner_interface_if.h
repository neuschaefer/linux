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
 * $RCSfile: tuner_interface.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file tuner_interface_if.h
 *  Export tuner driver interface to demod.
 */

#ifndef _TUNER_INTERFACE_IF_H
#define _TUNER_INTERFACE_IF_H

#include "dtd_tuner_if.h"
#include "dvbt_tuner_if.h"
#include "fe_tuner_common_if.h"
//#include "pd_tuner_type.h"

//---------------------------------------------------------------------------
// Type defininitions
//---------------------------------------------------------------------------

/** Tuner function list
*/
typedef struct
{
    UINT8 AgcRfBias;
    UINT8 AgcRfMin;
    UINT8 AgcIfMin;
    UINT8 AgcDrSel;
} ATD_AGC_CTX;


typedef struct
{
    UINT8   m_aucPara[TUNER_PARA_SIZE];//AD target level Parameter
    UINT16  u2LO_Step;
	#ifdef CC_DALI2K10 
    UINT8           u1InputSrc;//Air or Cable
    #endif
} SPECIFIC_MEMBER_US_CTX;


typedef struct{
    UCHAR     m_aucPara[cMAX_PARA_CNT];
    UINT8     m_SAW_BW;
    UINT8     m_Ana_Top;            // JC, 071227
    INT32     m_s4FreqBoundUpper;
    INT32     m_s4FreqBoundLower;
    INT32     m_s4AnaFreqBoundUpper;
    INT32     m_s4AnaFreqBoundLower;	
    INT8      m_SigLvScan;
    UINT8     m_SigLvTh;

//add by liuqu,20081111
    UINT16     m_Small_Step;
    UINT16     m_Middle_Step_Up;
    UINT16     m_Middle_Step_Down;

// add by JC, 081215
    UCHAR      m_ifAgc_lvl_max;
    UCHAR      m_ifAgc_lvl_min;
 
}SPECIFIC_MEMBER_EU_CTX;


typedef struct _ITUNER_CTX_T {
    UINT8   I2cAddress;
    UINT16  u2IF_Freq;//IF Freq (KHz))
    UINT32  u4RF_Freq;
    UINT8   u1AtdPatchSwitch;
    UINT8   fgLnaOn;
    UINT8   fgSunBoard;
    UINT8   fgRFAGC;  //RFAGC control by ATD or tuner, LC 090720
	UINT16  u2RetryTVDTm;//Re-check TVD lock interval if it is 0,disable TVD re-check mechanism  
    ATD_AGC_CTX sAtdAgcPara;

    union {
        SPECIFIC_MEMBER_US_CTX us_ctx;
        SPECIFIC_MEMBER_EU_CTX eu_ctx;
    }specific_member;
}ITUNER_CTX_T;

//Set Freq Parameter Structure
typedef struct {
    UINT32 Freq;		//KHz
    UINT8  Modulation;
    UINT8  fgAutoSearch;
} PARAM_SETFREQ_T;
typedef enum  {
	/* OP name		effective prototype */
	// Supported OP
	itSetSawBw,		// void SetSawBw(UINT8 sawBw)
	itSetIfDemod,		// void SetIfDemod(UINT8 subSysIdx)
	itGetStatus,		// BOOL GetStatus()
	itGetVer,			// char * GetVer()
	itGetSignalLevel,
    itGetEqScriptNormal,
    itGetEqScriptWeak,
    itGetEqScriptStrongChroma,
	itSetLNA	,
	itGetLNA,
	itSetTop,
	itSetReg,
	itGetReg,
	itSetIfMin,
	itMonitorLNA
} ITUNEROP_T;

typedef VOID  (*ITuner_Init_fct) (ITUNER_CTX_T *pTCtx);
typedef INT16  (*ITuner_SetFreq_fct)(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T *param);
typedef INT16  (*ITuner_OP_fct)(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal);


//! PI tuner function table declaration
typedef struct _ITUNER_FCT_TBL_T
{
	ITuner_Init_fct		Init;
	ITuner_SetFreq_fct	SetFreq;
	ITuner_OP_fct		OP;
} ITUNER_FCT_T;


//---------------------------------------------------------------------------
// Exported functions
//---------------------------------------------------------------------------

//! Primitive tuner functions
EXTERN ITUNER_CTX_T *ITunerGetCtx(VOID);
EXTERN INT32 ITunerRegistration(UINT16 id);

//	ret >= 0 , success
//	ret <0 , fail
#define ITUNER_OK 				0
#define ITUNER_NOT_OK 			-1	//  print error msg in tuner_interface layer 
#define ITUNER_NOT_SUPPORTED	-2	//  print error msg in tuner_interface layer



EXTERN VOID  ITuner_Init(ITUNER_CTX_T *pTCtx);
EXTERN INT16 ITuner_SetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T *param);
EXTERN INT16 ITuner_OP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal);
#endif
