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

/** @file pi_demod.h
 *  ISDBT tuner PI file
 */
#ifndef _ISDBT_PI_DEMOD_H_
#define _ISDBT_PI_DEMOD_H_

//#include "PD_Def.h"
#include "x_typedef.h"
#include "dtd_tuner_if.h"
#include "tuner_interface_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define TUNER_FUNC_EN_MAX_SIZE    16
#define TUNER_PARA_MAX_SIZE       32

#define cMAX_READ_NUM           8

#define ISDBT_TS_MASK_LAYERA    0x04
#define ISDBT_TS_MASK_LAYERB    0x02
#define ISDBT_TS_MASK_LAYERC    0x01

#define ISDBT_LOCK_LEVEL        0x40
#define ISDBT_LOCK_TMCC         0x20
#define ISDBT_LOCK_FRAME        0x08

enum
{
    ISDBT_LAYER_A = 0,
    ISDBT_LAYER_B,
    ISDBT_LAYER_C
};


enum
{
    ISDBT_FFT_SIZE_2K = 0,
    ISDBT_FFT_SIZE_4K,
    ISDBT_FFT_SIZE_8K,
    ISDBT_FFT_SIZE_UNKNOWN
};

enum
{
    ISDBT_GI_SIZE_1_32 = 0,
    ISDBT_GI_SIZE_1_16,
    ISDBT_GI_SIZE_1_8,
    ISDBT_GI_SIZE_1_4,
    ISDBT_GI_SIZE_UNKNOWN
};

enum
{
    ISDBT_MOD_DQPSK = 0,
    ISDBT_MOD_QPSK,
    ISDBT_MOD_16QAM,
    ISDBT_MOD_64QAM,
    ISDBT_MOD_NOLAYER,
    ISDBT_MOD_RESERVED
};

enum
{
    ISDBT_CR_1_2 = 0,
    ISDBT_CR_2_3,
    ISDBT_CR_3_4,
    ISDBT_CR_5_6,
    ISDBT_CR_7_8,
    ISDBT_CR_NOLAYER,
    ISDBT_CR_RESERVED
};


/** tuner device driver RF tuner context.
 *  This data structure is used to store tuner device driver RF tuner context.
*/
#if 0
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

typedef struct _DEMOD_CTX_T   
{
    UINT8       I2cAddress;
    UINT32         m_Freq_KHz;
    BOOL        m_bHighSampling;
    UINT32         uSysClk;
    UCHAR       region;
    UCHAR       m_iQamsize;
    UCHAR       uQamSize;
    UINT32         uSymbolRate;
    UINT32      BER;
    UINT32      PER;
    UINT32      SNR;
    UINT32      m_u4FreqBoundLower;
    UINT32      m_u4FreqBoundUpper;
    UINT32      m_u4MaxLockRange;
    ITUNER_CTX_T*  sTunerCtx;
    UINT8      m_I2cSts;
} DEMOD_CTX_T;


//! [TODO:PI] add PI API declarations
/////BEGIN
/////END

//----------------------------------------------------------------------------- 
/** psDemodCtxInit
 *  Create and initialize Demod context.
 *  @param   fgDemodCtxInit  Pointer of flag which record whether Demod context is initialized or not.
 *  @retval  DEMOD_CTX_T*  Pointer of Demod context.   
 */
//-----------------------------------------------------------------------------
DEMOD_CTX_T *psDemodCtxInit(BOOL *fgDemodCtxInit);

//----------------------------------------------------------------------------- 
/** vDemodCtxDestroy
 *  Destory Demod context.
 *  @param   psDemodCtx  Pointer of Demod context.
 *  @param   fgDemodCtxInit  Pointer of flag which record whether Demod context is initialized or not.
 *  @retval  void.   
 */
//-----------------------------------------------------------------------------
VOID vDemodCtxDestroy(DEMOD_CTX_T *psDemodCtx, BOOL *fgDemodCtxInit);

//----------------------------------------------------------------------------- 
/** ISDBT_Initialize
 *  Initialization of ISDB-T Demod.
 *  @param   psDemodCtx  Pointer of Demod context.
 *  @retval  1 : error.
 *  @retval  0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_Initialize(DEMOD_CTX_T *psDemodCtx);
//-------------------------------------------------------------------------
/**
 *  This API is driver fe initialize. It performs
 *  1. HW Demod resetr.
 *  2. Set LNA on
 *  3. witch input to AIR
 *  4.power off psk demod
 */
//-------------------------------------------------------------------------
BOOL ISDBT_FEInitialize(DEMOD_CTX_T *psDemodCtx);
//----------------------------------------------------------------------------- 
/** ISDBT_Connect
 *  Set ISDB-T Demod to start to acquire to a channel.
 *  @param   psDemodCtx  Pointer of Demod context.
 *  @retval  1 : error.
 *  @retval  0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_Connect(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** ISDBT_GetSync
 *  Get ISDB-T Demod lock status.
 *  @param   psDemodCtx  Pointer of Demod context.
 *  @retval  FALSE : unlock.
 *  @retval  TRUE  : locked.   
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_GetSync(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** ISDBT_GetVBer
 *  Get post-viterbi bit error rate.
 *  @param   psDemodCtx  Pointer of Demod context.
 *  @param   layer  specify the layer. 
 *  @retval  (UINT32) post-viterbi BER *10^5.
 */
//-----------------------------------------------------------------------------
UINT32 ISDBT_GetVBer(DEMOD_CTX_T *psDemodCtx, UINT8 layer);

//----------------------------------------------------------------------------- 
/** ISDBT_GetPer
 *  Get packet error rate.
 *  @param   psDemodCtx  Pointer of Demod context.
 *  @param   layer  specify the layer. 
 *  @retval  (UINT32) PER *10^5.
 */
//-----------------------------------------------------------------------------
UINT32 ISDBT_GetPer(DEMOD_CTX_T *psDemodCtx, UINT8 layer);

//----------------------------------------------------------------------------- 
/** ISDBT_GetSnr
 *  Get signal SNR.
 *  @param   psDemodCtx  Pointer of Demod context.
 *  @retval  (UINT16) SNR *10.
 */
//-----------------------------------------------------------------------------
UINT16 ISDBT_GetSnr(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** ISDBT_GetSignalLevel
 *  Get signal level in percentage.
 *  @param   psDemodCtx  Pointer of Demod context.
 *  @retval  (UINT16) normalized signal level in percentage.
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_GetSignalLevel(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** ISDBT_GetEws
 *  Get emergency warning system flag.
 *  @param   psDemodCtx  Pointer of Demod context.
 *  @retval  (UINT8) the value of emergency warning system flag
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_GetEws(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** ISDBT_UpdateModPara
 *  Update modulation parameter from ISDB-T Demod.
 *  Call this function before using getting modulation function, like ISDBT_GetFftSize(), ISDBT_GetMod()....
 *  @param   psDemodCtx  Pointer of Demod context.
 *  @retval  1 : error.
 *  @retval  0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_UpdateModPara(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** ISDBT_GetFftSize
 *  Get FFT size.
 *  @retval  ISDBT_FFT_SIZE_2K : 2k.
 *  @retval  ISDBT_FFT_SIZE_4K : 4k.
 *  @retval  ISDBT_FFT_SIZE_8K : 8k.
 *  @retval  ISDBT_FFT_SIZE_UNKNOWN : unknown.
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_GetFftSize(VOID);

//----------------------------------------------------------------------------- 
/** ISDBT_GetGiSize
 *  Get guard interval size.
 *  @retval  ISDBT_GI_SIZE_1_32 : 1/32.
 *  @retval  ISDBT_GI_SIZE_1_16 : 1/16.
 *  @retval  ISDBT_GI_SIZE_1_8  : 1/8.
 *  @retval  ISDBT_GI_SIZE_1_4  : 1/4.
 *  @retval  ISDBT_GI_SIZE_UNKNOWN : unknown.
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_GetGiSize(VOID);

//----------------------------------------------------------------------------- 
/** ISDBT_GetMod
 *  Get constellation modulation.
 *  @param   layer  specify the layer.
 *  @retval  ISDBT_MOD_DQPSK;   : DQPSK
 *  @retval  ISDBT_MOD_QPSK;    : QPSK
 *  @retval  ISDBT_MOD_16QAM;   : 16QAM
 *  @retval  ISDBT_MOD_64QAM;   : 64QAM
 *  @retval  ISDBT_MOD_NOLAYER  : this layer is not exist
 *  @retval  ISDBT_MOD_RESERVED : reserved
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_GetMod(UINT8 layer);

//----------------------------------------------------------------------------- 
/** ISDBT_GetCr
 *  Get code rate.
 *  @param   layer  specify the layer.
 *  @retval  ISDBT_CR_1_2     : 1/2
 *  @retval  ISDBT_CR_2_3     : 2/3
 *  @retval  ISDBT_CR_3_4     : 3/4
 *  @retval  ISDBT_CR_5_6     : 5/6
 *  @retval  ISDBT_CR_7_8     : 7/8
 *  @retval  ISDBT_CR_NOLAYER : this layer is not exist
 *  @retval  ISDBT_CR_RESERVED: reserved
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_GetCr(UINT8 layer);

//----------------------------------------------------------------------------- 
/** ISDBT_GetInterleave
 *  Get time interleave type.
 *  @param   layer  specify the layer.
 *  @retval  (UINT8) time interleave length
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_GetInterleave(UINT8 layer);

//----------------------------------------------------------------------------- 
/** ISDBT_GetSegment
 *  Get segment number.
 *  @param   layer  specify the layer.
 *  @retval  (UINT8) segment number
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_GetSegment(UINT8 layer);

//----------------------------------------------------------------------------- 
/** ISDBT_SetLayer
 *  Select output layer from TS.
 *  @param   layer  specify the layer.
 *  @param   u1TsMAsk  bit mask used to enable layer output.
 *  @retval  1 : error.
 *  @retval  0 : OK. 
 */
//-----------------------------------------------------------------------------
UINT8 ISDBT_SetLayer(DEMOD_CTX_T *psDemodCtx, UINT8 u1TsMAsk);

//----------------------------------------------------------------------------- 
/** ISDBT_GetBer
 *Get BER Before RS decoding or After RS decoding
 when fgBer is true,the retval is BER(After Viterbi decoding)
 otherwise,the retval is PER(After RS decoding)
 *  @param   psDemodCtx  demod driver context
 *  @param   fgBer       To indicate get BER or PER
*/
UINT32 ISDBT_GetBer(DEMOD_CTX_T *psDemodCtx,BOOL fgBer);

BOOL ISDBT_IsNoSignal(DEMOD_CTX_T *psDemodCtx);

BOOL ISDBT_IsDemodSleep(VOID);

UINT8 ISDBT_GetLayer(DEMOD_CTX_T *psDemodCtx);

UINT8 ISDBT_SetGiSize(DEMOD_CTX_T *psDemodCtx, UINT8 gi);

UINT8 ISDBT_SetCr(DEMOD_CTX_T *psDemodCtx, UINT8 layer, UINT8 cr);

UINT8  ISDBT_SetMod(DEMOD_CTX_T *psDemodCtx,UINT8  layer, UINT8  mod);

#endif // _ISDBTN_PI_DEMOD_H_
