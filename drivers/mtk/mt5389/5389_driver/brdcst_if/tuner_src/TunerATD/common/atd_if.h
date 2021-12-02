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
 * $RCSfile: atd_if.h,v $ 
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file atd_if.h 
 *  NA tuner analog pi primitive API 
 */

#ifndef ATD_IF_H
#define ATD_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "fe_tuner_common_if.h"
#include "tuner_interface_if.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef VOID *PATD_SPECIFIC_CTX;


#ifndef tuner_break_fct
#define tuner_break_fct
typedef INT32     (*x_break_fct)(void *pArg);
#endif

typedef struct _ATD_CTX_T
{
// Hardware Configure
    UINT8           I2cAddress;
    UINT32         u4Frequency;             /* carrier frequency (in KHz)   */
    UINT8           u1SubSysIdx;
// Status
    UINT16	    u2VOPCpo;
    INT32	    s4Cfo;
    UINT8           u1ControlStatus;
    UINT8           u1VIFLock;
    BOOL            fgPRALock;
    BOOL            fgCRLock;
    BOOL            fgCheckCR;
    BOOL            fgDemodReset;
    BOOL            fgAutoSearch;
    BOOL            fgAFT;
    BOOL            fgAudScan;
    UINT8           u1PF;
    UINT8           u1UpChkCnt;
    UINT8           u1UpNakCnt;
    UINT8           u1UpMisCnt;
    UINT8           u1TunerType;

    INT8    s1RFmin;
    INT8    s1IFmin;

    //GEN_TUNER_CTX_T sTunerCtx;
    //void            *psI2cCtx;
    HANDLE_T          hHostCmdLock;  // Semaphore
    HANDLE_T          hRegLock;      // Semaphore
    HANDLE_T          hLogLock;      // Semaphore
    BOOL            fgDemodInitFlag;
    HANDLE_T          hDemodAcq;     // Semaphore

    VOID        *pDigiTunerCtx;
    x_break_fct isBreak;
//    VOID        *isBreak;
    VOID        *pvBreakArg;
    //THREAD_T        t_monitor_thread;

    ITUNER_CTX_T *pTCtx;

    
} ATD_CTX_T;


typedef struct _ATD_PD_CTX_T
{
// ATD PI context
    void*        sAtdPiCtx;
// Hardware Configure
    UINT32      Frequency;                  /* Carrier frequency (in Hz) */
    UINT8       u1VIFLock;
    UINT8       u1TunerAcqType;
    UINT8       u1SubSysIdx;
    UINT32      u4TvSubSysMask;
    UINT8       ucTvSysGroupNum;
    UINT32      *pu4TvSysGroup;
    VOID        *psTunerAnaAttr;
// Glue-layer variables
    HANDLE_T    hHalLock;  //sema
    UINT32      u4DrvCustTunerCtrl;
    UINT32      u4AcqFrequency;
    BOOL        fgBreakMonitor;
    BOOL        fgPIMoniStatus; //1: doing monitor 0:not doing monitor
    BOOL        fgDisStatus;
} ATD_PD_CTX_T, *PATD_PD_CTX_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


enum
{
    ATV_CONN_MODE_CHAN_CHANGE = 0,
    ATV_CONN_MODE_CHAN_SCAN,
    ATV_CONN_MODE_FINE_TUNE,
    ATV_CONN_MODE_FM_RADIO,
    ATV_CONN_MODE_NUM
};        




#endif /* ATD_IF_H */

