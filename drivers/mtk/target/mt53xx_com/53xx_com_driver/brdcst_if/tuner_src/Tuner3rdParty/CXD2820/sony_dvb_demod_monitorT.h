/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2009 Sony Corporation.

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

#include <sony_dvb_demod.h>

/**
 @brief IP ID monitor.

 @param pDemod The demodulator instance.
 @param pID The ID of the IP.

 @return SONY_DVB_OK if successful and pID valid.
*/
sony_dvb_result_t dvb_demod_monitorT_IPID (sony_dvb_demod_t * pDemod, 
                                           uint8 * pID);

/**
 @brief Sync state monitor.

 @param pDemod The demodulator instance.
 @param pSyncStat Sync state (6: Lock).
 @param pTSLockStat TS lock state (0: Not Lock, 1: Lock).

 @return SONY_DVB_OK if successful and pSyncStat, pTSLockStat valid.
*/
sony_dvb_result_t dvb_demod_monitorT_SyncStat (sony_dvb_demod_t * pDemod, 
                                               uint8 * pSyncStat, 
                                               uint8 * pTSLockStat);

/**
 @brief Monitor the IFAGC value. 
        Actual dB gain dependent on attached tuner.

 @param pDemod The demodulator instance.
 @param pIFAGCOut The IFAGC output register value. Range 0 - 0xFFF. Unitless.

 @return SONY_DVB_OK if successful and pIFAGCOut valid.
*/
sony_dvb_result_t dvb_demod_monitorT_IFAGCOut (sony_dvb_demod_t * pDemod, 
                                               uint32 * pIFAGCOut);

/**
 @brief RF AGC output monitor.
        Actual dB gain dependent on attached tuner.

 @param pDemod The demodulator instance.
 @param pRFAGCOut The RFAGC output register value. Unitless. Range: 0 - 1023. 

 @return SONY_DVB_OK if successful and pRFAGCOut valid.
*/
sony_dvb_result_t dvb_demod_monitorT_RFAGCOut (sony_dvb_demod_t * pDemod, 
                                               uint32 * pRFAGCOut);

/**
 @brief Monitor the detected mode/guard (not TPS mode/guard).

 @param pDemod The demodulator instance.
 @param pMode Mode estimation result.
 @param pGuard Guard interval estimation result.

 @return SONY_DVB_OK if successful and pMode, pGuard valid.
*/
sony_dvb_result_t dvb_demod_monitorT_ModeGuard (sony_dvb_demod_t * pDemod,
                                                sony_dvb_demod_dvbt_mode_t * pMode, 
                                                sony_dvb_demod_dvbt_guard_t * pGuard);

/**
 @brief Monitor the estimated carrier offset. To get the estimated
        centre frequency of the current channel:
        Fest = Ftune + pOffset ;

 @param pDemod The demodulator instance.
 @param pOffset Carrier offset value (kHz).

 @return SONY_DVB_OK if successful and pOffset valid.
*/
sony_dvb_result_t dvb_demod_monitorT_CarrierOffset (sony_dvb_demod_t * pDemod, 
                                                    int32 * pOffset);

/**
 @brief Monitor the estimated C/N.

 @param pDemod The demodulator instance.
 @param pCNValue C/N estimation value in dB x 1000.

 @return SONY_DVB_OK if successful and pCNValue valid.
*/
sony_dvb_result_t dvb_demod_monitorT_CNValue (sony_dvb_demod_t * pDemod, 
                                              int32 *pCNValue);

/**
 @brief Monitor the estimated MER.

 @param pDemod The demodulator instance.
 @param pMER MER estimation value in dB x 1000.

 @return SONY_DVB_OK if successful and pMER valid.
*/
sony_dvb_result_t dvb_demod_monitorT_MER (sony_dvb_demod_t * pDemod, 
                                          int32 *pMER);

/**
 @brief Monitor the MER estimate data value.

 @param pDemod The demodulator instance.
 @param pMERDT MER_DT register value.

 @return SONY_DVB_OK if successful and pMERDT valid.
*/
sony_dvb_result_t dvb_demod_monitorT_MERDT (sony_dvb_demod_t * pDemod, 
                                            uint32 * pMERDT);
/**
 @brief Monitor the Pre-Viterbi BER.

 @param pDemod The demodulator instance.
 @param pBER BER value (Pre viterbi decoder) in 1e7.

 @return SONY_DVB_OK if successful and pBER valid.
*/
sony_dvb_result_t dvb_demod_monitorT_PreViterbiBER (sony_dvb_demod_t * pDemod, 
                                                    uint32 *pBER);

/**
 @brief Monitor the Pre-Viterbi BER parameters.

        BER = pBitError / pPeriod ;

 @param pDemod The demodulator instance.
 @param pBitError The number of bit errors detected by the demodulator.
 @param pPeriod The number of bits that the bit errors were measured over.

 @return SONY_DVB_OK if successful and pBitError, pPeriod valid.
*/
sony_dvb_result_t dvb_demod_monitorT_PreViterbiBitError (sony_dvb_demod_t * pDemod, 
                                                         uint32 * pBitError, 
                                                         uint32 * pPeriod);

/**
 @brief Monitor the Pre-RS BER.

 @param pDemod The demodulator instance.
 @param pBER BER value (Pre reed solomon decoder) in 1e7.

 @return SONY_DVB_OK if successful and pBER valid.
*/
sony_dvb_result_t dvb_demod_monitorT_PreRSBER (sony_dvb_demod_t * pDemod, 
                                               uint32 *pBER);

/**
 @brief Monitor the Pre-RS BER parameters.

        BER = pBitError / pPeriod ;

 @param pDemod The demodulator instance.
 @param pBitError The number of bit errors detected by the demodulator.
 @param pPeriod The number of bits that the bit errors were measured over.

 @return SONY_DVB_OK if successful and pBitError, pPeriod valid.
*/
sony_dvb_result_t dvb_demod_monitorT_PreRSBitError (sony_dvb_demod_t * pDemod, 
                                                    uint32 * pBitError, 
                                                    uint32 * pPeriod);

/**
 @brief The TPS information structure.
*/
typedef struct {

    /**
     @brief DVB-T constellation.
    */
    sony_dvb_demod_dvbt_constellation_t constellation;

    /**
     @brief DVB-T hierarchy.
    */
    sony_dvb_demod_dvbt_hierarchy_t hierarchy;

    /**
     @brief The HP code rate.
    */
    sony_dvb_demod_dvbt_coderate_t rateHP;

    /**
     @brief The LP code rate.
    */
    sony_dvb_demod_dvbt_coderate_t rateLP;

    /**
     @brief The guard interval.
    */
    sony_dvb_demod_dvbt_guard_t guard;

    /**
     @brief The mode.
    */
    sony_dvb_demod_dvbt_mode_t mode;

    /**
     @brief The Cell ID.
    */
    uint16 cellID;
} sony_dvb_demod_tpsinfo_t;

/**
 @brief Monitor the TPS information.

 @param pDemod The demodulator instance.
 @param pInfo The TPS information.

 @return SONY_DVB_OK if successful and pInfo valid.
*/
sony_dvb_result_t dvb_demod_monitorT_TPSInfo (sony_dvb_demod_t * pDemod, 
                                              sony_dvb_demod_tpsinfo_t * pInfo);

/**
 @brief Monitors the number RS (Reed Solomon) errors detected by the 
        RS decoder over 1 second. Also known as the code word reject count.

 @param pDemod The demodulator instance.
 @param pRSError The number of RS errors detected over 1 second.

 @return SONY_DVB_OK if successful and pRSError valid. 
*/
sony_dvb_result_t dvb_demod_monitorT_RSError (sony_dvb_demod_t * pDemod, 
                                              uint32 * pRSError);

/**
 @brief Monitors the spectrum sense detected by the demodulator with respect to 
        the configured spectrum sense.

 @param pDemod The demodulator instance.
 @param pSense The detected spectrum sense.

 @return SONY_DVB_OK if successful and pSense is valid.
*/
sony_dvb_result_t dvb_demod_monitorT_SpectrumSense (sony_dvb_demod_t* pDemod,
                                                    sony_dvb_demod_spectrum_sense_t* pSense);

/**
 @brief Monitors the estimated SNR register value.

 @param pDemod The demodulator instance.
 @param pSNR The estimated SNR in dBx1000.

 @return SONY_DVB_OK if successful and pSNR is valid.
*/
sony_dvb_result_t dvb_demod_monitorT_SNR (sony_dvb_demod_t * pDemod, int32 *pSNR);

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

 @return SONY_DVB_OK if pPPM is valid.
*/
sony_dvb_result_t dvb_demod_monitorT_SamplingOffset (sony_dvb_demod_t * pDemod, 
                                                     int32* pPPM);

#endif /* SONY_DVB_DEMOD_MONITORT_H */
