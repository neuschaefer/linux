/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2009 Sony Corporation.

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

#include <sony_dvb_demod.h>

/*------------------------------------------------------------------------------
 Functions: Monitoring
------------------------------------------------------------------------------*/
/**
 @brief Monitor the carrier offset of the currently tuned channel.
        Calls the appropriate T/T2/C monitor function based on the current
        driver operating mode.
        To get the estimated centre frequency of the current channel:
        Fest = Ftune + pOffset ;

 @param pDemod The demodulator instance.
 @param pOffset The detected carrier offset in kHz.

 @return SONY_DVB_OK if pOffset is valid.
*/
sony_dvb_result_t dvb_demod_monitor_CarrierOffset (sony_dvb_demod_t * pDemod, 
                                                   int32 * pOffset);

/**
 @brief Monitor the signal quality, expressed as a percentage.

        <b>Example Implementation Only</b>
        
        - DVBC: No SQI available.
        - DVBT: Implements SQI according to Nordig V2.01 Section 3.4.4.7.
        - DVBT2: <b>Example Implementation Only</b>
        Empirical algorithm based on the MER, CNR and Post BCH FER.
        MER is scaled between measured knee point (picture fail) 
        and near maximum MER and returned as a percentage. 
        MER knee point is dependent on active PLP modcod.

        CNR is scaled between Nordig knee point and maximum value.
        CNR knee point is dependent on active PLP modcod.

        \verbatim
        If Post BCH FER is > 0, then the returned SQI indicator
        is 0%.
        
        FER != 0: SQI = 0.
        FER == 0: SQI = 
            (((MER - MERmin) / (MERmax - MERmin)) * 100)/2  +
            (((CNR - (CNrel-CNmarg)) / (CNmax - (CNrel-CNmarg))) * 100)/2 ;

        Where:
        MER     = Monitored MER.
        MERmin  = The value of the MER - 0.5dB at picture fail point 
                 of active PLP modcod.
        MERmax  = "Maximum" MER from device.

        CN     = Monitored CNR.
        CNrel  = Nordig CNR point for active PLP modcod.
        CNmarg = CNR margin for scaling.
        \endverbatim
        Notes:
        MERmax could be made a fixed margin above picture fail. 
        However, this *could* lead to installation issues if the installer happens to 
        choose a robust PLP to use while installing which may be misleading for the 
        SQI on less robust PLPs.

        Issues:
        - Precision of the output decreases with higher modulation 
          and code rates as the MER and CNR range are reduced.
        - Very high margin (20dB) on low modulation and code rate (e.g QPSK 1/2)
          would show relatively low SQI (~70%).
        - Recommended to average the DVB-T2 SQI over 1 second at 100ms intervals.
    
 @param pDemod The demodulator instance.
 @param pQuality The quality as a percentage (0-100).

 @return SONY_DVB_OK if pQuality is valid.
*/
sony_dvb_result_t dvb_demod_monitor_Quality (sony_dvb_demod_t* pDemod, 
                                             uint8* pQuality);

/**
 @brief Monitor the RFAGC value provided by the demodulator.
        Calls the appropriate T/T2/C monitor function based on the current
        driver operating mode.

 @param pDemod The demodulator instance.
 @param pRFAGC The RFAGC value.
 
 @return SONY_DVB_OK if pRFAGC is valid.
*/
sony_dvb_result_t dvb_demod_monitor_RFAGCOut (sony_dvb_demod_t * pDemod,
                                              uint32 * pRFAGC);

/**
 @brief Monitor the IFAGC value provided by the demodulator,
        independent of the current operating mode.
        
        Calls the appropriate T/T2/C monitor function based on the current
        driver operating mode.

        Normalises the DVB-C IFAGC value to the DVB-T/T2 IFAGC values.

 @param pDemod The demodulator instance.
 @param pIFAGC The IFAGC value.
 
 @return SONY_DVB_OK if pIFAGC is valid.
*/
sony_dvb_result_t dvb_demod_monitor_IFAGCOut (sony_dvb_demod_t * pDemod, 
                                              uint32 * pIFAGC);

/**
 @brief Monitor the TS lock condition.

        Calls the appropriate T/T2/C monitor function based on the current
        driver operating mode.

 @param pDemod The demodulator instance.
 @param pLock The TS lock status.

 @return SONY_DVB_OK if pLock is valid.
*/
sony_dvb_result_t dvb_demod_monitor_TSLock (sony_dvb_demod_t * pDemod, 
                                            sony_dvb_demod_lock_result_t * pLock);

/**
 @brief Get the IP ID of the connected demodulator.

 @param pDemod The demodulator instance.
 @param pIPID Pointer to receive the IP ID into.

 @return SONY_DVB_OK if pIPID is valid.
*/
sony_dvb_result_t dvb_demod_monitor_IPID (sony_dvb_demod_t * pDemod, 
                                          uint8 * pIPID);

/**
 @brief Get the Chip ID of the connected demodulator.
 
 @param pDemod The demodulator instance.
 @param pChipId Pointer to receive the IP ID into.

 @return SONY_DVB_OK if pChipId is valid.
*/
sony_dvb_result_t dvb_demod_monitor_ChipID (sony_dvb_demod_t * pDemod, 
                                            sony_dvb_demod_chip_id_t * pChipId);


#endif /* SONY_DVB_DEMOD_MONITOR_H */
