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
 * $RCSfile: dtd_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dtd_if.h
 *  This file list DTD primitive APIs.
 */

#ifndef DTD_IF_H
#define DTD_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "fe_tuner_common_if.h"
#include "dtd_tuner_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#if 0
#define TUNER_FUNC_EN_MAX_SIZE    16
#define TUNER_PARA_MAX_SIZE       32
#endif

enum
{
   DEMOD_CONN_TYPE_UNKNOWN = 0,
   DEMOD_CONN_TYPE_SCAN,
   DEMOD_CONN_TYPE_SURF   /* Channel Change */
};

//-----------------------------------------------------------------------------
// Type definitions

//-----------------------------------------------------------------------------
#if 0
/** tuner device driver RF tuner context.
 *  This data structure is used to store tuner device driver RF tuner context.
*/
typedef struct
{
    UINT8   u1TunerType;
    UINT8   I2cAddress;
    UINT8   u1IF_I2cAddr;
    UINT16  u2IF_Freq;
    UINT16  u2LO_Step;
    UINT16  u2Ana_LO_Step;
    UINT32  u4LO_Freq;
    UINT32  u4RF_Freq;
    UINT8   m_aucFunc[TUNER_FUNC_EN_MAX_SIZE];    
    UINT8   m_aucPara[TUNER_PARA_MAX_SIZE];    
// Access Link
    VOID    *psDemodCtx;
    VOID                    *psI2cCtx;
} RF_TUNER_CTX_T;
#endif

/***********************************************************************/
/** tuner device driver demod context.
 *  This data structure is used to store tuner device driver demod context.
 */

typedef struct _DEMOD_CTX_T
{
// Hardware Configure
    UINT8           u1ChipGen;
    UINT8           u1ChipRev;
    UINT8           I2cAddress;
    INT32           Frequency ;             /* carrier frequency (in Hz)   */
    UINT32          SymbolRate;             /* Symbol Rate (in Baud)       */
    UINT8           Modulation;             /* Modulation (8VSB, 64QAM, 256QAM) */
    UINT8           ConnectionType;
    INT32           PreFrequency;
// Status
    BOOL            fgLockSignal;
    UINT8           ucLockStatus;
    UINT8           ucLockState;
    UINT8           ucSigCnt;
    UINT16          u2SigLeveldBm;
    BOOL            fgAcqStart;
    UINT16          u2AcqTime;
    UINT8           u1ReAcqCnt;
    UINT16          u2AccReAcqCnt;
    UINT8           u1StatusInd;
    UINT8           u1QSpecInv;
    BOOL            fgAutoQam;
    BOOL            fgMosaicUnLock;
//    float           SNR;
    UINT8           u1SNR;
    UINT16          u2SNRRaw;
    UINT16          u2SNRRaw_HighRes;
    UINT8           u1SNR_FS;
    UINT16          u2SNRRaw_FS;
//    FLOAT           fErrorRate;
    UINT16          u2ErrorRate;
    UINT16          u2PktErr;
    UINT16          u2PktCnt;
    UINT16          u2PrePktErr;
    UINT32          u4AccErrorRate;
    UINT32          u4AccPktErr;
    UINT32          u4AccPktCnt;
    UINT8           u1ErrClass;
    UINT8           u1PERRaw1;
    UINT8           u1PERRaw2;
    UINT8           u1ErrRateIdx;
    UINT8           u1LossRateIdx;
    INT8            i1MosaicCnt;
    UINT8           u1CurTSIFCnt;
    BOOL            fgResetFlag;
    UINT8           ucConnFlag;
    BOOL            fgInitFlag;
    UINT8           u1PecFlag;
    //BOOL            fgFreqGapEn;
    UINT16          u2PecRefreshTime;
    UINT32          u4PecObserveTime;
    UINT8           u1PecErrSec;
//    UINT8           u1I2cNakCnt;
    UINT8           u1UpChkCnt;
    UINT8           u1UpNakCnt;
    UINT8           u1UpMisCnt;
    BOOL            fgStatusReady;
    BOOL            fgInfoReady;
    BOOL            fgPecReady;
    UINT8           u1TunerAcqType;
    UINT16          u2FwAcqTime;
    BOOL            fgUserGetInfo;
    UINT8           u1TransPhase;
    UINT8           u1TunerType;
    
// Output Format Parameters
//    MPEG_FMT_T      sTS_Format;
// Access Link
    ITUNER_CTX_T*  sTunerCtx;
    void            *psI2cCtx;
    HANDLE_T          hHostCmdLock;      // Semaphore
    HANDLE_T          hRiscIfLock;       // Semaphore
    HANDLE_T          hRiscIfAtdLock;    // Semaphore  
    HANDLE_T          hLogLock;          
    BOOL            fgDemodInitFlag;
    HANDLE_T        hDemodAcq;           // Semaphore  
    UINT32          u4DrvCustTunerCtrl;
    UINT8           u1DrvCustTunerDefType;
} DEMOD_CTX_T;


/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
//-------------------------------------------------------------------------
/** DTD_SetReg
 *  Set consecutive demod registers. The length is at most 4 bytes.
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @param  u2RegAddr   16-bit register address.
 *  @param  *pu1Buffer  Pointer of data buffer values to be set.
 *  @param  u2ByteCount Length to be read. u2ByteCount <= 4.
 *  @retval 0: Fail, Others: Success.
 */
//-------------------------------------------------------------------------
UINT16 DTD_SetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);

//-------------------------------------------------------------------------
/** DTD_GetReg
 *  Get consecutive demod registers. The length is at most 4 bytes.
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @param  u2RegAddr   16-bit register address.
 *  @param  *pu1Buffer  Pointer of data buffer to be stored read values.
 *  @param  u2ByteCount Length to be set. u2ByteCount <= 4.
 *  @retval 0: Fail, Others: Success.
 */
//-------------------------------------------------------------------------
UINT16 DTD_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);

/***********************************************************************/
/*              Interface APIs                                         */
/***********************************************************************/
//-------------------------------------------------------------------------
/** DTD_CheckTimeout
 *  Check if No-signal case is detected.
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @retval TRUE    No-signal case is detected.
 *  @retval FALSE   Normal case.
 */
//-------------------------------------------------------------------------
UCHAR DTD_CheckTimeout(DEMOD_CTX_T *psDemodCtx);

//-------------------------------------------------------------------------
/** DTD_Connected
 *  Set Demod connection flag to be connected.
 *  @param  psDemodCtx  Pointer of DTD device driver context.
 *  @retval None
 */
//-------------------------------------------------------------------------
VOID DTD_Connected(DEMOD_CTX_T *psDemodCtx);

//-------------------------------------------------------------------------
/** DTD_AcqEnd
 *  Update some parameters which need to be updated when the acquisition is finished.
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @retval None
 */
//-------------------------------------------------------------------------
VOID DTD_AcqEnd(DEMOD_CTX_T *psDemodCtx, BOOL fgBreak);

//-------------------------------------------------------------------------
/** DTD_DisConnect
 *  Stop acquisition and put Demod into sleep mode.
 *  This API should be invoked when the middleware disconnects the channel.
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @retval None
 */
//-------------------------------------------------------------------------
VOID DTD_DisConnect(DEMOD_CTX_T *psDemodCtx);

//-------------------------------------------------------------------------
/** DTD_Initialize
 *  Initialize tuner device driver.
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @param  u1I2cAddress    Demod I2C address. Default is 0x92
 *  @param  fgHardwareInit  Init demod hardware
 *  @retval 0           Success
 *  @retval -1          Fail
 */
//-------------------------------------------------------------------------
INT32 DTD_Initialize(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddress,BOOL fgHardwareInit);

//-------------------------------------------------------------------------
/** DTD_Close
 *  Cleanup system resource, and put demod into sleep mode
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @retval 0           Success
 *          Others      System error
 */
//-------------------------------------------------------------------------
INT32 DTD_Close(DEMOD_CTX_T *psDemodCtx);

//-------------------------------------------------------------------------
/** DTD_Connect
 *  Start acquisition to specified frequency and modulation type
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @param  u4Freq      RF center frequency in kHz
 *  @param  u1Modulation    Modulation type: MOD_ATSC_8VSB, MOD_J83B_64QAM & MOD_J83B_256QAM
 *  @param  u1ConnType      Connection type: DEMOD_CONN_TYPE_SCAN & DEMOD_CONN_TYPE_SURF
 *  @retval FALSE       Hardware error
 */
//-------------------------------------------------------------------------
//BOOL DTD_Connect(DEMOD_CTX_T *psDemodCtx, UINT32 u4Freq, UINT8 u1Modulation);
BOOL DTD_Connect(DEMOD_CTX_T *psDemodCtx, UINT32 u4Freq, UINT8 u1Modulation,UINT8 u1ConnType);

//-------------------------------------------------------------------------
/** DTD_GetSync
 *  Get demod and tuner lock status
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @retval TRUE        Locked
 */
//-------------------------------------------------------------------------
INT16 DTD_GetSync(DEMOD_CTX_T *psDemodCtx);

//-------------------------------------------------------------------------
/** DTD_SetSpcPolarity
 *  Set spectrum polarity.
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @param  fgDebug       1 (Manual. For debug) / 0 (Auto. Normal flow use).
 *  @param  u1SpecInv   
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_SetSpcPolarity(DEMOD_CTX_T *psDemodCtx, BOOL fgDebug, UINT8 u1SpecInv);

//-------------------------------------------------------------------------
/** DTD_GetTcmBER
 *  Set spectrum polarity.
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @param  pu4TotalPkt        Received total packets.
 *  @param  pu4ErrPkt          Received error symbols. 
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_GetTcmBER(DEMOD_CTX_T *psDemodCtx, UINT32 *pu4TotalPkt, UINT32 *pu4ErrSym);

//-------------------------------------------------------------------------
/** DTD_GetIfAgcVol
 *  Get IF AGC Voltage in mV.
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @param  pu2IfAgcVol   IF AGC voltage in mV.
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_GetIfAgcVol(DEMOD_CTX_T *psDemodCtx, UINT16 *pu2IfAgcVol);

//-------------------------------------------------------------------------
/** DTD_GetMod
 *  Get modulation type.
 *  @param  psDemodCtx  Pointer of demod device driver context.
 *  @param  peModType   Modulation type.
 *  @retval INT32
 */
//-------------------------------------------------------------------------
INT32 DTD_GetMod(DEMOD_CTX_T *psDemodCtx, UINT8 *peModType);

#endif /* DTD_IF_H */

