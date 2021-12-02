/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**
 @file    sony_dvb_demod_monitorC.h

          This file provides the DVB-C demodulator monitor interface.

*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DVB_DEMOD_MONITORC_H
#define SONY_DVB_DEMOD_MONITORC_H

#include "sony_dvb_demod.h"

/**
 @brief Sync state monitor.

 @param pDemod The demodulator instance.
 @param pARLock Demod lock state (0: Not Lock 1: Lock ).
 @param pTSLockStat TS lock state (0: Not Lock, 1: Lock).
 @param pUnlockDetected Early unlock (1: Early Unlock detected).
 @return SONY_RESULT_OK on success and pARStat and pTSLockStat are valid.

*/
sony_result_t sony_dvb_demod_monitorC_SyncStat (sony_dvb_demod_t * pDemod, uint8_t * pARLock, uint8_t * pTSLockStat, uint8_t* pUnlockDetected);

/**
 @brief Monitor the IFAGC value. 
        Actual dB gain dependent on attached tuner.

 @param pDemod The demodulator instance.
 @param pIFAGCOut Value of IFAGC output register value. Unitless.
                  
 @return SONY_RESULT_OK if successful and pIFAGCOut valid.
*/
sony_result_t sony_dvb_demod_monitorC_IFAGCOut (sony_dvb_demod_t * pDemod, uint32_t * pIFAGCOut);

/**
 @brief Monitor detected constellation.

 @param pDemod The demodulator instance.
 @param pQAM The monitored constellation.

 @return SONY_RESULT_OK if successful and pQAM valid.
*/
sony_result_t sony_dvb_demod_monitorC_QAM (sony_dvb_demod_t * pDemod, sony_dvbc_constellation_t * pQAM);

/**
 @brief Monitor symbol rate.

 @param pDemod The demodulator instance.
 @param pSymRate Symbol rate (kSymbol/sec).

 @return SONY_RESULT_OK if successful and pSymRate valid.
*/
sony_result_t sony_dvb_demod_monitorC_SymbolRate (sony_dvb_demod_t * pDemod, uint32_t * pSymRate);

/**
 @brief Carrier offset monitor.        
        To get the estimated centre frequency of the current channel:
        Fest = Ftune + pOffset ;
        This function will compensate for an inverting tuner architecture if the 
        demodulator has been configured for the tuner architecture. 
        See ::sony_dvb_demod_config_id_t::DEMOD_CONFIG_SPECTRUM_INV config option for 
        ::dvb_demod_SetConfig.

 @param pDemod The demodulator instance.
 @param pOffset Carrier offset value(kHz).

 @return SONY_RESULT_OK if successful and pOffset valid.
*/
sony_result_t sony_dvb_demod_monitorC_CarrierOffset (sony_dvb_demod_t * pDemod, int32_t * pOffset);

/**
 @brief Monitors the spectrum sense.

 @param pDemod The demodulator instance.
 @param pSense Indicates the inversion sense. 

 @return SONY_RESULT_OK if successful and pSense valid. 
*/
sony_result_t sony_dvb_demod_monitorC_SpectrumSense (sony_dvb_demod_t * pDemod, sony_dvb_demod_spectrum_sense_t * pSense);

/**
 @brief Monitors the estimated SNR register value.

 @param pDemod The demodulator instance.
 @param pSNR The estimated SNR value in dB x 1000.

 @return SONY_RESULT_OK if successful and pSNR valid. 
*/
sony_result_t sony_dvb_demod_monitorC_SNR (sony_dvb_demod_t * pDemod, int32_t * pSNR);

/**
 @brief SNR monitor. Monitors the register value directly.

 @param pDemod The demodulator instance.
 @param pSNRReg IREG_SNR_ESTIMATE register value.

 @return SONY_RESULT_OK if successful and pSNRReg valid. 
*/
sony_result_t sony_dvb_demod_monitorC_SNRReg (sony_dvb_demod_t * pDemod, uint16_t * pSNRReg);

/**
 @brief Monitor the Pre-RS BER.

 @param pDemod The demodulator instance.
 @param pBER BER value (Pre reed solomon decoder) in 1e7.

 @return SONY_RESULT_OK if successful and pBER valid.
*/
sony_result_t sony_dvb_demod_monitorC_PreRSBER (sony_dvb_demod_t * pDemod, uint32_t * pBER);

/**
 @brief Monitors the number RS (Reed Solomon) errors detected by the 
        RS decoder over 1 second. Also known as the code word reject count.

 @param pDemod The demodulator instance.
 @param pPEN The number of RS errors detected over 1 second.

 @return SONY_RESULT_OK if successful and pPEN valid. 
*/
sony_result_t sony_dvb_demod_monitorC_PacketErrorNumber (sony_dvb_demod_t * pDemod, uint32_t * pPEN);

#endif /* SONY_DVB_DEMOD_MONITORC_H */
