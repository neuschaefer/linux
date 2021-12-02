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
 * $RCSfile: pi_dvbt_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_dvbt_if.h
 *  DVB-T Demod pi primitive API.
 */

#ifndef PI_8G80_DTMB_H
#define PI_8G80_DTMB_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "u_handle.h"
#include "dtmb_tuner_if.h"
#include "tuner_interface_if.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
enum
{
    DVBT_HIR_PRIORITY_UNKNOWN = 0,
    DVBT_HIR_PRIORITY_NONE,
    DVBT_HIR_PRIORITY_HIGH,
    DVBT_HIR_PRIORITY_LOW
};

enum
{
    DTMB_DEMOD_STATE_UNLOCK = 0,
    DTMB_DEMOD_STATE_LOCK,
    DTMB_DEMOD_STATE_UNKNOWN
};

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


typedef struct _DEMOD_CTX_T
{
// Hardware Configure
    UINT8           I2cAddress;
    INT32           Frequency ;             /* carrier frequency (in Hz)   */
    UINT8           fgEn24MClk;
	UINT8			ucEnFpga;
	UINT8           u1Bw;
	UCHAR           ucConnType;
	BOOL            fgAutoScan;
	UINT8           u1MWIntf;
// Status
	UINT8           u1Ver1;
	UINT8           u1Ver0;
	UINT8           u1Fft;
	UINT8           u1GI;
	UINT8           u1CR;
	UINT8           u1Mod;
	UINT8           u1FecRate;
	UINT8           u1CarrierMode;
	UINT8           u1TimDeInterl;
	UINT8           u1PnNumber;
	UINT8           u1Hier;
	UINT8           u1HpLp;
	UINT8           u1LpCR;
	UINT8           u1HpCR;
	UINT8           u1Indepth;
	INT16			s2Ifo;
	INT16			s2Ffo;
	INT32			s4Cfo;
	UINT8			u1Si;
	UINT8           m_TunerType;
	UCHAR           m_TunerMode; // momo, 061116, for silicon tuner
	UINT16          u2CellId;
    INT8            s1IfAgc;
	UINT8           u1ControlStatus;
	BOOL            fgAgcLock;
	UINT8           ucMdFftMode;
	UINT8			ucLastDemodStatus;
	BOOL            fgTpsLock;
	BOOL            fgTsLock;
	UINT8           u1Rssi;
    INT16           s2Snr;   // SNR*512
    UINT16          u2ErrorPkt;
    UINT16          u2TotalPkt;
	UINT32          u4CorrBit;
    UINT16          u2VdTotBit;
    UINT16          u2VdErrBit;
    BOOL            fgDemodReset;
    UINT8           u1UpChkCnt;
    UINT8           u1UpNakCnt;
    UINT8           u1UpMisCnt;
	// RF AGC
	INT8            m_s1AciPow;
	INT8            m_s1InbPow;
	UINT8			ucTsIfFormat;
	UINT8			ucTsIfCtrl;
	UINT8			ucTsIfRate;

	UINT8           u1ChSearchMode;
	BOOL            fgSelectANIR;
	UINT8           u1MdMode;
	UINT8           u1TpsCr;
	UINT8           u1TpsCons;
	UINT8           u1TpsHier;
	INT8            s1FreqBias40khz;

    UINT8           u1TunerAcqType; // for SDAL, tune freq only or tune Demod only
    ITUNER_CTX_T*   psTunerCtx;

    //void            *psI2cCtx;
    HANDLE_T          hHostCmdLock;     // Semaphore
	HANDLE_T          hRegLock;         // Semaphore
    HANDLE_T          hLogLock;         // Semaphore
    BOOL              fgDemodInitFlag;
    HANDLE_T          hDemodAcq;        // Semaphore
    //THREAD_T        t_monitor_thread;
    BOOL            fgMonitorThreadRunning;
    INT32           i4BerTh;
} DEMOD_CTX_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------- 
/** vDTMBGetInfo
 *  Get demod information and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void   vDTMBGetInfo(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** vDTMBGetStatus
 *  Get demod status and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void   vDTMBGetStatus(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** vDTMBGetPara
 *  Get demod system parameters and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void   vDTMBGetPara(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBT_SetReg
 *  This function set demod register.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8  DVBT_SetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);

//-----------------------------------------------------------------------------
/** DVBT_GetReg
 *  This function get demod register.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8  DVBT_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);

//----------------------------------------------------------------------------- 
/** DTMB_Initialize
 *  Initialize demod context.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  ucI2cAddress  Demod I2C address.
 *  @param  fgLoadCode  Flag to check if need to downlaod firmware code. Unused.
 *  @retval   DRVAPI_TUNER_OK : OK.
 *  @retval   DRVAPI_TUNER_ERROR : Fail.   
 */
//-----------------------------------------------------------------------------
INT32  DTMB_Initialize(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddress, BOOL fgLoadCode);

//----------------------------------------------------------------------------- 
/** DVBT_DemodReset
 *  Demod Reset.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   DRVAPI_TUNER_OK : OK.
 *  @retval   DRVAPI_TUNER_ERROR : Fail.   
 */
//-----------------------------------------------------------------------------
//INT32  DVBT_DemodReset(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DTMB_UpdateInfo
 *  Get demod status, information and system parameters 
 *  and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void   DTMB_UpdateInfo(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBT_DriverStart
 *  Start demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u1Mon1Sel  Monitor1 driver select. (0: None, 1: Auto-Reacquisition).
 *  @param  u1Mon2Sel  Monitor2 driver select. (0: None, 1: TPS sync, 2: SPS sync auto, 3: SPS sync manual
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void   DVBT_DriverStart(DEMOD_CTX_T *psDemodCtx, UINT8 u1Mon1Sel, UINT8 u1Mon2Sel);

//----------------------------------------------------------------------------- 
/** DVBT_DriverStop
 *  Stop demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void   DVBT_DriverStop(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBT_SetBW
 *  Set channel bandwidth.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  ucBW  Channel bandwidth.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void   DVBT_SetBW(DEMOD_CTX_T *psDemodCtx, UINT8 ucBW);

//----------------------------------------------------------------------------- 
/** DVBT_SetTsIf
 *  Set TS interface parameters.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  tsif_format  TS interface format.
 *  TsIfFormat:
 *  b7  Reserve
 *  b6	VALID	Active polarity selection of tsif_val
 *  	0 -> tsif_val is high active
 *  	1 -> tsif_val is low active
 *  b5	ERR	Active polarity selection of tsif_err
 *  	0 -> tsif_err is high active
 *  	1 -> tsif_err is low active
 *  b4	SYNC	Active polarity selection of tsif_sync
 *  	0 -> tsif_sync is high active
 *  	1 -> tsif_sync is low active
 *  b3	TEI	Transport stream error indicator
 *  	0 -> TEI bit will not be changed by tsif
 *  	1 -> TEI bit will automatic set by tsif 
 *  b2	SP	Parrallel / Serial mode selection
 *  	0 -> Serial mode
 *  	1 -> Parralle mode
 *  b1	CLK_GATED	Output clock gated selection of tsif_clk
 *  	0 -> tsif_clk is gated
 *  	1 -> tsif_clk is non-gated
 *  b0	CLK_INV	Output clock edge selection of tsif_clk
 *  	0 -> tsif_data is valid at positive edge clock
 *  	1 -> tsif_data is valid at negative edge clock
 *  @param  tsif_ctrl  TS interface control.
 *  TsIfControl:
 *  b7  Reserve
 *  b6	FRACTIONAL_PKT	Allow MPEG fractional packet generated
 *  	0 -> Output packet is always 188 bytes
 *  	1 -> Output packet may not be 188 bytes when channel changed
 *  b5	SERIAL_SYNC  Control the width of TSIF_SYNC signal. Only valid in serial mode
 *  	0 -> SYNC duration = first 8 bit
 *  	1 -> SYNC duration = first 1 bit
 *  b4	DISABLE_HDR  Disable output header 'h47
 *  	0 -> Normal
 *  	1 -> Disable header 'h47 in TS
 *  b3	DATA_FLIP  Control data flip of TSIF_DATA[7:0]
 *  	0 -> Normal
 *  	1 -> Data flip of TSIF_DATA[7:0]
 *  b2	ERR_DROP_PKT  Drop packet if error packet occur
 *  	0 -> Normal
 *  	1 -> Drop packet if TSIF_ERR = 1'b1
 *  b1	ERR_SHORT  Define TSIF_ERR duration
 *  	0 -> Normal, TSIF_ERR duration is 188 bytes
 *  	1 -> TSIF_ERR duration is 1 byte
 *  b0	ERR_GATED  Define TSIF_ERR gated
 *  	0 -> TSIF_ERR is gated by data enable
 *  	1 -> TSIF_ERR is not gated between packet
 *  @param  tsif_rate  TS interface clock rate. Output data rate = (48MHz/(TSIF_RATE+1)). The value should be odd number.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void   DVBT_SetTsIf(DEMOD_CTX_T *psDemodCtx, UCHAR tsif_format, UCHAR tsif_ctrl, UCHAR tsif_rate);

//----------------------------------------------------------------------------- 
/** DVBT_SetHpLp
 *  Set HP/LP.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  ucHpLpSel  HP/LP selection. (EN_HP_DECODE/EN_LP_DECODE).
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void   DVBT_SetHpLp(DEMOD_CTX_T *psDemodCtx, UCHAR ucHpLpSel);

//----------------------------------------------------------------------------- 
/** DVBT_SetSeachParam
 *  Set system parameters of channel scan or channel up/down mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  search_mode  search mode select. (CH_SEARCH_BLIND : search mode, CH_SEARCH_UPDOWN : channel up/down mode).
 *  @param  mdMode  MD mode select.
 *  b・7: Enable 2k mode
 *  b・6: Enable 4k mode
 *  b・5: Enable 8k mode
 *  [b・4:b・2] GI
 *   000: 1/32
 *   001: 1/16
 *   010: 1/8
 *   011: 1/4
 *  [b・1:b・0] Spectrum inversion mode
 *   01: inverted
 *   10: non-inverted
 *  @param  tpsCr  TPS Code Rate.
 *  b・7: 0
 *  b・6: 0
 *  [b・5:b・3] TPS LP code rate info
 *   000: 1/2
 *   001: 2/3
 *   010: 3/4
 *   011: 5/6
 *   100: 7/8
 *   101~111: Reserved
 *  [b・2:b・0] TPS HP code rate info
 *   000: 1/2
 *   001: 2/3
 *   010: 3/4
 *   011: 5/6
 *   100: 7/8
 *   101~111: Reserved
 *  @param  tpsCons  TPS constellation.
 *  0: QPSK
 *  1: 16QAM
 *  2: 64QAM
 *  Other values: Reserved
 *  @param  tpsHier  TPS hierarchy.
 *  0: native, non-hierarchy
 *  1: native, alpha = 1
 *  2: native, alpha = 2
 *  3: native, alpha = 4
 *  4: indepth, non-hierarchy
 *  5: indepth, alpha = 1
 *  6: indepth, alpha = 2
 *  7: indepth, alpha = 4
 *  Other values: Reserved
 *  @param  freqBias_40kHz  Frequency bias in 40kHz. (-18~+18).
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void   DVBT_SetSearchParam(DEMOD_CTX_T *psDemodCtx, UCHAR search_mode, UCHAR mdMode, 
						    UCHAR tpsCr, UCHAR tpsCons, UCHAR tpsHier, INT8 freqBias_40kHz);

//----------------------------------------------------------------------------- 
/** DTMB_GetSignalLevel
 *  Get signal level from 0 (lowest) to 100 (highest).
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   (UINT8) 0~100.   
 */
//-----------------------------------------------------------------------------
UINT8  DTMB_GetSignalLevel(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DTMB_GetSignalSNR
 *  Get signal SNR.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   (UINT8) SNR in 0.1dB.   
 */
//-----------------------------------------------------------------------------
UINT16  DTMB_GetSignalSNR(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBT_GetPER
 *  Get TS packet error rate.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   (INT32) PER*10^5.   
 */
//-----------------------------------------------------------------------------
INT32  DVBT_GetPER(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBT_GetTpsLock
 *  Get TPS lock.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   TRUE : Lock.
 *  @retval   FALSE : Unlock.   
 */
//-----------------------------------------------------------------------------
BOOL   DVBT_GetTpsLock(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBT_GetTsLock
 *  Get TS lock.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   TRUE : Lock.
 *  @retval   FALSE : Unlock.   
 */
//-----------------------------------------------------------------------------
BOOL   DVBT_GetTsLock(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBT_GetBW
 *  Get channel bandwidth.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   0 : 6M.
 *  @retval   1 : 7M.
 *  @retval   2 : 8M.
 *  @retval   3 : 5M.
 */
//-----------------------------------------------------------------------------
UINT8  DVBT_GetBW(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBT_GetFreqOffset_kHz
 *  Get frequency offset in kHz.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   (INT32) freq_offset * 256.   
 */
//-----------------------------------------------------------------------------
INT32  DVBT_GetFreqOffset_kHz(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetIndepth
 *  Get inner interleaver mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   0 : native mode.
 *  @retval   1 : indepth mode.
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_GetIndepth(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBT_GetHierMode
 *  Get hierarchical mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   0 : non-hierarchical.
 *  @retval   1 : hierarchical, alpha = 1.
 *  @retval   2 : hierarchical, alpha = 2.
 *  @retval   3 : hierarchical, alpha = 4.
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_GetHierMode(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetHier
 *  Get hierarchical mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   DVBT_HIR_PRIORITY_UNKNOWN : unknown type.
 *  @retval   DVBT_HIR_PRIORITY_NONE : non-hierarchical.
 *  @retval   DVBT_HIR_PRIORITY_HIGH : hierarchical, HP stream.
 *  @retval   DVBT_HIR_PRIORITY_LOW  : hierarchical, LP stream.
 */
//-----------------------------------------------------------------------------
UINT8  DVBT_GetHier(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBT_GetHpLp
 *  Get the mode of current decoding stream.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   0 : HP(high priority) stream or non-hierarchical mode.
 *  @retval   1 : LP(low  priority) stream.
 */
//-----------------------------------------------------------------------------
UINT8  DVBT_GetHpLp(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBT_GetDvbtNotExist
 *  Check if really no DVB-T signal exists.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   TRUE  : It's confirmed that no DVB-T signal exists.   
 *  @retval   FALSE : DVB-T signal exists or it's not confirmed yet.
 */
//-----------------------------------------------------------------------------
BOOL   DVBT_GetDvbtNotExist(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetPreVBer
 *  Get pre Viterbi BER.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   Pre Viterbi BER * 10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DVBT_GetPreVBer(DEMOD_CTX_T *psDemodCtx);

// ----------------------------------------------------------------------------- 
/** DVBT_GetPostVBer
 *  Get post Viterbi BER.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   Post Viterbi BER * 10^5.   
 */
//-----------------------------------------------------------------------------
INT32  DVBT_GetPostVBer(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBT_GetSearchParam
 *  Get and update system parameters of channel scan or channel up/down mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  mdMode  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  tpsCr  Pointer of code rate value.
 *  @param  tpsCons  Pointer of TPS value.
 *  @param  tpsHier  Pointer of hierarchical mode value.
 *  @param  freqBias_40kHz  Pointer of frequency offset in 40kHz .
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void   DVBT_GetSearchParam(DEMOD_CTX_T *psDemodCtx, UCHAR *mdMode, UCHAR *tpsCr, 
						   UCHAR *tpsCons, UCHAR *tpsHier, INT8 *freqBias_40kHz);


//----------------------------------------------------------------------------- 
/** DTMB_Connect
 *  Start demod module to acquire to a channel.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  ucBW  Channel bandwidth.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_Connect(DEMOD_CTX_T *psDemodCtx, UINT8 ucBW);

//----------------------------------------------------------------------------- 
/** DTMB_GetTotPkt
 *  Get total TS packets.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval  (UINT32)total TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DTMB_GetTotPkt(DEMOD_CTX_T *psDemodCtx);


//----------------------------------------------------------------------------- 
/** DTMB_GetErrPkt
 *  Get error TS packets.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval  (UINT32)error TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DTMB_GetErrPkt(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DTMB_GetIfAgcVol
 *  Get IF AGC voltage.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   (UINT16) IF AGC voltage in mV.   
 */
//-----------------------------------------------------------------------------
UINT16 DTMB_GetIfAgcVol(DEMOD_CTX_T *psDemodCtx);


//----------------------------------------------------------------------------- 
/** DVBT_GetSwVer
 *  Get SW version.
 *  @retval   string of SW version.
 */
//-----------------------------------------------------------------------------
CHAR*  DVBT_GetSwVer(void);

//----------------------------------------------------------------------------- 
/** DVBT_SetTSInterface
 *  Set TS interface
 *  @param  fgParallel  set interface to parallel or serial mode. TRUE: Parallel, FALSE: serial
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBT_SetTSInterface(BOOL fgParallel);

//----------------------------------------------------------------------------- 
/** DVBT_SetTSPath
 *  Set TS routing path
 *  @param  fgEnable  set TS path to tri-state or pass-through. TRUE: pass-through, FALSE: tri-state
 *  @retval  demod lock status, 0: unlock, 1: lock.
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_SetTSPath(BOOL fgEnable);

//liuqu,AUTO ATP,20090302
#ifdef EU_AUTO_ATP
extern UINT8 gATP_TEST_ITEM_BIT ;
extern UINT8 gATPFailBitMsk ;
#endif
#endif /* PI_DVBT_IF_H */

