/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**
 @file    sony_dvb_demod_monitor.h

          This file provides the common demodulator monitor interface.

*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DVB_DEMOD_MONITOR_H
#define SONY_DVB_DEMOD_MONITOR_H

#include "sony_dvb_demod.h"

/*------------------------------------------------------------------------------
 Functions: Monitoring
------------------------------------------------------------------------------*/
/**
 @brief Monitor the TS lock condition.

        Calls the appropriate T/T2/C monitor function based on the current
        driver operating mode.

 @param pDemod The demodulator instance.
 @param pLock The TS lock status.

 @return SONY_RESULT_OK if pLock is valid.
*/
sony_result_t sony_dvb_demod_monitor_TSLock (sony_dvb_demod_t * pDemod, sony_dvb_demod_lock_result_t * pLock);

/**
 @brief Monitor the carrier offset of the currently tuned channel.
        Calls the appropriate T/T2/C monitor function based on the current
        driver operating mode.
        To get the estimated centre frequency of the current channel:
        Fest = Ftune + pOffset ;

 @param pDemod The demodulator instance.
 @param pOffset The detected carrier offset in kHz.

 @return SONY_RESULT_OK if pOffset is valid.
*/
sony_result_t sony_dvb_demod_monitor_CarrierOffset (sony_dvb_demod_t * pDemod, int32_t * pOffset);

/**
 @brief Monitor the RFAGC value provided by the demodulator.
        Calls the appropriate T/T2/C monitor function based on the current
        driver operating mode.

 @param pDemod The demodulator instance.
 @param pRFAGC The RFAGC value.
 
 @return SONY_RESULT_OK if pRFAGC is valid.
*/
sony_result_t sony_dvb_demod_monitor_RFAGCOut (sony_dvb_demod_t * pDemod, uint32_t * pRFAGC);

/**
 @brief Monitor the IFAGC value provided by the demodulator,
        independent of the current operating mode.
        
        Calls the appropriate T/T2/C monitor function based on the current
        driver operating mode.

        Normalises the DVB-C IFAGC value to the DVB-T/T2 IFAGC values.

 @param pDemod The demodulator instance.
 @param pIFAGC The IFAGC value.
 
 @return SONY_RESULT_OK if write was succesful.
*/
sony_result_t sony_dvb_demod_monitor_IFAGCOut (sony_dvb_demod_t * pDemod, uint32_t * pIFAGC);

/**
 @brief Get the Chip ID of the connected demodulator.
 
 @param pDemod The demodulator instance.
 @param pChipId Pointer to receive the IP ID into.

 @return SONY_RESULT_OK if pChipId is valid.
*/
sony_result_t sony_dvb_demod_monitor_ChipID (sony_dvb_demod_t * pDemod, sony_dvb_demod_chip_id_t * pChipId);

/**
 @brief Monitor the signal quality, expressed as a percentage.

        <b>Example Implementation Only</b>
        
        - DVBC: No SQI available.
        - DVBT: Implements SQI according to Nordig V2.01 Section 3.4.4.7.
        - DVBT2: Implements SQI according to Nordig V2.2.1
    
 @param pDemod The demodulator instance.
 @param pQuality The quality as a percentage (0-100). Output.

 @return SONY_RESULT_OK if pQuality is valid.
*/
sony_result_t sony_dvb_demod_monitor_Quality (sony_dvb_demod_t * pDemod, uint8_t * pQuality);

#endif /* SONY_DVB_DEMOD_MONITOR_H */
