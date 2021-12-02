/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/05/27
 Revision: 1.0.0.1

------------------------------------------------------------------------------*/
#ifndef CXD2828_ISDB_T_DEMOD_MONITOR_H
#define CXD2828_ISDB_T_DEMOD_MONITOR_H

#include "cxd2828_common.h"
#include "cxd2828_i2c.h"
#include "cxd2828_stdlib.h"
#include "cxd2828_isdb_t.h"
#include "cxd2828_isdb_t_demod.h"

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  Monitor target
--------------------------------------------------------------------*/
typedef enum cxd2828_isdb_t_demod_monitor_target_t {
    CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_A, /* A layer */
    CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_B, /* B layer */
    CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_C  /* C layer */
} cxd2828_isdb_t_demod_monitor_target_t;

/*------------------------------------------------------------------------------
  Functions
------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  Monitor sync status

  pDemod     : Address of ISDB-T part instance.
  pDmdLock   : Address of demodulator lock flag
               - 0: Not lock
               - 1: Lock
  pTSLock    : Address of TS lock flag
               - 0: Not lock
               - 1: Lock
  pUnlock    : Address of unlock detection flag
               - 0: No ISDB-T signal in the receiving channel.
               - 1: Detection underway or ISDB-T signal may exist.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorSyncStat(cxd2828_isdb_t_demod_t* pDemod,
                                                UINT8* pDmdLock,
                                                UINT8* pTSLock,
                                                UINT8* pUnlock);

/*--------------------------------------------------------------------
  Monitor IFAGC output level

  pDemod : Address of ISDB-T part instance.
  pIFAGC : Address of IFAGC output level (0 - 4095).
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorIFAGCOut(cxd2828_isdb_t_demod_t* pDemod,
                                                UINT32* pIFAGC);

/*--------------------------------------------------------------------
  Monitor mode and guard interval

  pDemod : Address of ISDB-T part instance.
  pMode  : Address of mode
  pGuard : Address guard interval
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorModeGuard(cxd2828_isdb_t_demod_t* pDemod,
                                                 cxd2828_isdb_t_mode_t* pMode,
                                                 cxd2828_isdb_t_guard_t* pGuard);

/*--------------------------------------------------------------------
  Monitor spectrum inversion

  This API return the spectrum polarity of input to demodulator.

  pDemod    : Address of ISDB-T part instance.
  pIsInvert : Address of spectrum polarity
              - 0: Not inverted
              - 1: Inverted
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorSpectrumSense(cxd2828_isdb_t_demod_t* pDemod,
                                                     UINT8* pIsInvert);

/*--------------------------------------------------------------------
  Monitor carrier offset

  The relation of "Actual broadcasting frequency(X)","Current tuning frequency(Y)"
  and "Carrier offset(Z)" is following.
    X = Y + Z
  The unit is "KHz".

  pDemod  : Address of ISDB-T part instance.
  pOffset : Address of carrier offset(KHz).
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorCarrierOffset(cxd2828_isdb_t_demod_t* pDemod,
                                                     INT32* pOffset);

#ifdef CXD2828_FP_ENABLE
/*--------------------------------------------------------------------
  Monitor MER(Modulation Error Ratio)

  pDemod : Address of ISDB-T part instance.
  pMER   : Address of MER(dB).
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorMER(cxd2828_isdb_t_demod_t* pDemod,
                                           double* pMER);
#endif /* CXD2828_FP_ENABLE */

/*--------------------------------------------------------------------
  Monitor MERDT

  For the environment of not use floating point calculation,
  this function provides the monitor the register IREG_MER_DT.
  If you can use floating pointer calculation, you can use
  cxd2828_isdb_t_demod_MonitorMER function for monitor MER.

  pDemod : Address of ISDB-T part instance.
  pMER   : Address of IREG_MER_DT register value.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorMERDT(cxd2828_isdb_t_demod_t* pDemod,
                                             UINT32* pMERDT);

#ifdef CXD2828_FP_ENABLE
/*--------------------------------------------------------------------
  Monitor BER(Bit Error Ratio) after Viterbi decoding

  pDemod : Address of ISDB-T part instance.
  target : Monitor target
  pBER   : Address of BER.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorPostViterbiBER(cxd2828_isdb_t_demod_t* pDemod,
                                                      cxd2828_isdb_t_demod_monitor_target_t target,
                                                      double* pBER);
#endif /* CXD2828_FP_ENABLE */

/*--------------------------------------------------------------------
  Monitor bit error count after Viterbi decoding

  For the environment of not use floating point calculation,
  this function provides monitor the bit error count.
  If you can use floating pointer calculation, you can use
  cxd2828_isdb_t_demod_MonitorPostViterbiBER function for monitor BER.

  pDemod     : Address of ISDB-T part instance.
  target     : Monitor target
  pBitError  : Address of bit error count.
  pPacketCnt : Address of packet count.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorPostViterbiBitError(cxd2828_isdb_t_demod_t* pDemod,
                                                           cxd2828_isdb_t_demod_monitor_target_t target,
                                                           UINT32* pBitError,
                                                           UINT32* pPacketCnt);

#ifdef CXD2828_FP_ENABLE
/*--------------------------------------------------------------------
  Monitor PER(Packet Error Ratio) after Reed-Solomon decoding

  pDemod    : Address of ISDB-T part instance.
  target    : Monitor target
  pPostRSPER : Address of PER.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorPostRSPER(cxd2828_isdb_t_demod_t* pDemod,
                                                 cxd2828_isdb_t_demod_monitor_target_t target,
                                                 double* pPostRSPER);
#endif /* CXD2828_FP_ENABLE */

/*--------------------------------------------------------------------
  Monitor packet error count after Reed-Solomon decoding

  For the environment of not use floating point calculation,
  this function provides monitor the bit error count.
  If you can use floating pointer calculation, you can use
  cxd2828_isdb_t_demod_MonitorPostRSPER function for monitor PER.

  pDemod       : Address of ISDB-T part instance.
  target       : Monitor target
  pPacketError : Address of packet error count.
  pPacketCnt   : Address of packet count.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorPostRSPacketError(cxd2828_isdb_t_demod_t* pDemod,
                                                         cxd2828_isdb_t_demod_monitor_target_t target,
                                                         UINT32* pPacketError,
                                                         UINT32* pPacketCnt);

/*--------------------------------------------------------------------
  Monitor TMCC information

  pDemod    : Address of ISDB-T part instance.
  pTMCCInfo : Address of TMCC information.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorTMCCInfo(cxd2828_isdb_t_demod_t* pDemod,
                                                cxd2828_isdb_t_tmcc_info_t* pTMCCInfo);

/*--------------------------------------------------------------------
  Monitor preset information

  The preset information of current tuning channel can get by this function.
  And next tuning to this channel, you can shorten the tuning time to set it.

  pDemod      : Address of ISDB-T part instance.
  pPresetInfo : Address of preset information.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorPresetInfo(cxd2828_isdb_t_demod_t* pDemod,
                                                  cxd2828_isdb_t_demod_preset_info_t* pPresetInfo);

#endif /* CXD2828_ISDB_T_DEMOD_MONITOR_H */
