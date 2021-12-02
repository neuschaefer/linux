/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**
 @file    sony_dvb_demod_monitorT.h

          This file provides the DVB-T demodulator monitor interface.

*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DVB_DEMOD_MONITORT_H
#define SONY_DVB_DEMOD_MONITORT_H

#include "sony_dvb_demod.h"

#ifndef SONY_DVB_SUPPORT_DVBT

#define  sony_dvb_demod_monitorT_IPID(pDemod,pID)                           SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_SyncStat( pDemod,pSyncStat,pTSLockStat,pUnlockDetected)   SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_IFAGCOut( pDemod,pIFAGCOut)                 SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_ModeGuard( pDemod,pMode,pGuard)             SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_CarrierOffset( pDemod, pOffset)             SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_MER( pDemod, pMER)                          SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_PreViterbiBER( pDemod, pBER)                SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_PreRSBER( pDemod, pBER)                     SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_TPSInfo( pDemod, pInfo)                     SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_PacketErrorNumber( pDemod, pPEN)            SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_SpectrumSense( pDemod, pSense)              SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_SNR( pDemod, pSNR)                          SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_SamplingOffset( pDemod, pPPM)               SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorT_Quality(pDemod, pQuality)                  SONY_RESULT_ERROR_NOSUPPORT

#else

/**
 @brief IP ID monitor.

 @param pDemod The demodulator instance.
 @param pID The ID of the IP.

 @return SONY_RESULT_OK if successful and pID valid.
*/
sony_result_t sony_dvb_demod_monitorT_IPID (sony_dvb_demod_t * pDemod, uint8_t * pID);

/**
 @brief Sync state monitor.

 @param pDemod The demodulator instance.
 @param pSyncStat Sync state (6: Lock).
 @param pTSLockStat TS lock state (0: Not Lock, 1: Lock).
 @param pUnlockDetected Early unlock (1: Early Unlock detected).
 @return SONY_RESULT_OK if successful and pSyncStat, pTSLockStat valid.
*/
sony_result_t sony_dvb_demod_monitorT_SyncStat (sony_dvb_demod_t * pDemod, uint8_t * pSyncStat, uint8_t * pTSLockStat, uint8_t* pUnlockDetected);

/**
 @brief Monitor the IFAGC value. 
        Actual dB gain dependent on attached tuner.

 @param pDemod The demodulator instance.
 @param pIFAGCOut The IFAGC output register value. Range 0 - 0xFFF. Unitless.

 @return SONY_RESULT_OK if successful and pIFAGCOut valid.
*/
sony_result_t sony_dvb_demod_monitorT_IFAGCOut (sony_dvb_demod_t * pDemod, uint32_t * pIFAGCOut);

/**
 @brief Monitor the detected mode/guard (not TPS mode/guard).

 @param pDemod The demodulator instance.
 @param pMode Mode estimation result.
 @param pGuard Guard interval estimation result.

 @return SONY_RESULT_OK if successful and pMode, pGuard valid.
*/
sony_result_t sony_dvb_demod_monitorT_ModeGuard (sony_dvb_demod_t * pDemod,
                                                     sony_dvbt_mode_t * pMode, sony_dvbt_guard_t * pGuard);

/**
 @brief Monitor the estimated carrier offset. To get the estimated
        centre frequency of the current channel:
        Fest = Ftune + pOffset ;

 @param pDemod The demodulator instance.
 @param pOffset Carrier offset value (kHz).

 @return SONY_RESULT_OK if successful and pOffset valid.
*/
sony_result_t sony_dvb_demod_monitorT_CarrierOffset (sony_dvb_demod_t * pDemod, int32_t * pOffset);

/**
 @brief Monitor the estimated MER.

 @param pDemod The demodulator instance.
 @param pMER MER estimation value in dB x 1000. Deprecated. Maps to ::sony_dvb_demod_monitorT_SNR.

 @return SONY_RESULT_OK if successful and pMER valid.
*/
sony_result_t sony_dvb_demod_monitorT_MER (sony_dvb_demod_t * pDemod, int32_t * pMER);

/**
 @brief Monitor the Pre-Viterbi BER.

 @param pDemod The demodulator instance.
 @param pBER BER value (Pre viterbi decoder) in 1e7.

 @return SONY_RESULT_OK if successful and pBER valid.
*/
sony_result_t sony_dvb_demod_monitorT_PreViterbiBER (sony_dvb_demod_t * pDemod, uint32_t * pBER);

/**
 @brief Monitor the Pre-RS BER.

 @param pDemod The demodulator instance.
 @param pBER BER value (Pre reed solomon decoder) in 1e7.

 @return SONY_RESULT_OK if successful and pBER valid.
*/
sony_result_t sony_dvb_demod_monitorT_PreRSBER (sony_dvb_demod_t * pDemod, uint32_t * pBER);

/**
 @brief Monitor the TPS information.

 @param pDemod The demodulator instance.
 @param pInfo The TPS information.

 @return SONY_RESULT_OK if successful and pInfo valid.
*/
sony_result_t sony_dvb_demod_monitorT_TPSInfo (sony_dvb_demod_t * pDemod, sony_dvbt_tpsinfo_t * pInfo);

/**
 @brief Monitors the number RS (Reed Solomon) errors detected by the 
        RS decoder over 1 second. Also known as the code word reject count.

 @param pDemod The demodulator instance.
 @param pPEN The number of RS errors detected over 1 second.

 @return SONY_RESULT_OK if successful and pPEN valid. 
*/
sony_result_t sony_dvb_demod_monitorT_PacketErrorNumber (sony_dvb_demod_t * pDemod, uint32_t * pPEN);

/**
 @brief Monitors the spectrum sense detected by the demodulator with respect to 
        the configured spectrum sense.

 @param pDemod The demodulator instance.
 @param pSense The detected spectrum sense.

 @return SONY_RESULT_OK if successful and pSense is valid.
*/
sony_result_t sony_dvb_demod_monitorT_SpectrumSense (sony_dvb_demod_t * pDemod, sony_dvb_demod_spectrum_sense_t * pSense);

/**
 @brief Monitors the estimated SNR register value.

 @param pDemod The demodulator instance.
 @param pSNR The estimated SNR in dBx1000.

 @return SONY_RESULT_OK if successful and pSNR is valid.
*/
sony_result_t sony_dvb_demod_monitorT_SNR (sony_dvb_demod_t * pDemod, int32_t * pSNR);

/**
 @brief Monitor the sampling frequency offset value.
        PPM = (( IREG_TRL_CTLVAL_S - OREG_TRCG_NOMINALRATE ) / OREG_TRCG_NOMINALRATE ) * 1e6
        
        @note Limitation: This function assumes the values of TRL_NOMINALRATE
              applied in src/sony_dvb_demod.c are true and correct. 
              This limitation was done to avoid 64-bit math or floating 
              point operations.

 @param pDemod The demodulator instance.
 @param pPPM The sampling frequency offset in ppm x 100.
             Range: +/- 220ppm. 
             Accuracy: +/- 0.2ppm.

 @return SONY_RESULT_OK if pPPM is valid.
*/
sony_result_t sony_dvb_demod_monitorT_SamplingOffset (sony_dvb_demod_t * pDemod, int32_t * pPPM);


/**
 @brief Monitor the DVB-T quality metric. Based on Nordig SQI 
        equation.
 
 @param pDemod The demodulator instance.
 @param pQuality The quality as a percentage (0-100).

 @return SONY_RESULT_OK if successful and pQuality valid.
*/
sony_result_t sony_dvb_demod_monitorT_Quality (sony_dvb_demod_t * pDemod, uint8_t * pQuality);

#endif /* SONY_DVB_SUPPORT_DVBT */

#endif /* SONY_DVB_DEMOD_MONITORT_H */
