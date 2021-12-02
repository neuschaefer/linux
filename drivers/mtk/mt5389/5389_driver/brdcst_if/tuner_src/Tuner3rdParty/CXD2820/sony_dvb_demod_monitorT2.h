/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2009 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**
 @file    sony_dvb_demod_monitorT2.h

          This file provides the DVB-T2 demodulator monitor interface.

*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DVB_DEMOD_MONITORT2_H
#define SONY_DVB_DEMOD_MONITORT2_H

#include <sony_dvb_demod.h>
#include <sony_dvb_t2.h>

/*------------------------------------------------------------------------------
 Monitor Functions
------------------------------------------------------------------------------*/

/**
 @brief Monitor the carrier offset of the currently tuned channel.
        This uses the continual pilot (CP) carrier offset estimation 
        from the device.
        To get the estimated centre frequency of the current channel:
        Fest = Ftune (Hz) + pOffset (Hz) ;

 @param pDemod The demodulator instance.
 @param pOffset The detected carrier offset in Hz.

 @return SONY_DVB_OK if pOffset is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_CarrierOffset (sony_dvb_demod_t * pDemod, 
                                                     int32 * pOffset);

/**
 @brief Monitor the IFAGC value of the T2 demodulator.
        Actual dB gain dependent on attached tuner.
 
 @param pDemod The demodulator instance.
 @param pIFAGC The IFAGC output register value. Range 0 - 0xFFF. Unitless.

 @return SONY_DVB_OK if pIFAGC is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_IFAGCOut (sony_dvb_demod_t * pDemod, 
                                                uint32 * pIFAGC);

/**
 @brief Monitor the RFAGC value of the T2 demodulator.
        Actual dB gain dependent on attached tuner.
 
 @param pDemod The demodulator instance.
 @param pRFAGC The returned RFAGC. Unitless. Range: 0 - 1023. 

 @return SONY_DVB_OK if pRFAGC is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_RFAGCOut (sony_dvb_demod_t * pDemod, 
                                                uint32 * pRFAGC);

/**
 @brief Monitor the SNR estimation made by the demodulator.
 
 @param pDemod The demodulator instance.
 @param pSNR The returned SNR in dB x 1000.

 @return SONY_DVB_OK if pSNR is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_SNR (sony_dvb_demod_t * pDemod, 
                                           int32 * pSNR);

/**
 @brief Monitor the MER (Modulation Error Ratio) of the T2 
        demodulator.
 
 @param pDemod The demodulator instance.
 @param pMER The returned MER in dB x 1000.

 @return SONY_DVB_OK if pMER is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_MER (sony_dvb_demod_t * pDemod, 
                                           int32 * pMER);

/**
 @brief Monitor the pre-LDPC BER. This provides the 
        data PLP BER in multiple PLP case.

 @param pDemod The demodulator instance.
 @param pBER The returned BER in 1e7.

 @return SONY_DVB_OK if the pBER is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_PreLDPCBER (sony_dvb_demod_t * pDemod, 
                                                  uint32 * pBER);

/**
 @brief Monitor the pre-LDPC BER params. This provides the 
        data PLP in multiple PLP case.

 @param pDemod The demodulator instance.
 @param pBitError The number of bit errors detected.
 @param pPeriod The number of bits over which the error was measured.

 @return SONY_DVB_OK if the pBitError, pPeriod is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_PreLDPCBERParam (sony_dvb_demod_t * pDemod, 
                                                       uint32* pBitError,
                                                       uint32* pPeriod);

/**
 @brief Monitor the post BCH FER (FEC block error rate) parameters.

 @param pDemod The demod instance.
 @param pFER The estimated FER x 1e6.
 
 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_monitorT2_PostBCHFER(sony_dvb_demod_t* pDemod,
                                                 uint32* pFER);
/**
 @brief Monitor the post BCH FER (FEC block error rate) parameters.

 @param pDemod The demod instance.
 @param pFECError The FEC block error count.
 @param pPeriod The number of FEC blocks that the errors were accumulated over.
 
 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_monitorT2_PostBCHFERParam(sony_dvb_demod_t* pDemod,
                                                      uint32* pFECError,
                                                      uint32* pPeriod);

/**
 @brief Monitor the L1-pre signalling information. L1-pre signalling 
        information is available when TS is locked, and earlier in the
        acquisition sequence, after the P1 symbols have been decoded.

 @param pDemod The demodulator instance.
 @param pL1Pre Pointer to receive the L1-pre signalling information.

 @return SONY_DVB_OK if the pL1Pre is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_L1Pre (sony_dvb_demod_t * pDemod, 
                                             sony_dvb_dvbt2_l1pre_t * pL1Pre);

/**
 @brief Monitor the tuning information on the active channel.

 @param pDemod The demodulator instance.
 @param pPresetInfo Pointer to receive the tuning information.

 @return SONY_DVB_OK if the pPresetInfo is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_Preset (sony_dvb_demod_t * pDemod, 
                                              sony_dvb_demod_t2_preset_t * pPresetInfo);

/**
 @brief Monitor the data PLPs that the demodulator has detected.
        If a single PLP service is in use, then numberPlps = 1
        and the plpIds[0] shall contain the signalled PLP Id.

 @param pDemod The demodulator instance.
 @param pPLPIds Pointer to an array of at least 256 bytes in length 
        that can receive the list of data PLPs carried.
 @param pNumPLPs The number of data PLPs detected (signalled in L1-post).

 @return SONY_DVB_OK if the pPLPIds and pNumPLPs are valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_DataPLPs (sony_dvb_demod_t * pDemod, 
                                                uint8 * pPLPIds, 
                                                uint8 * pNumPLPs);

/**
 @brief Monitor the active PLP information. 
        For multiple PLP systems, then able to monitor both the data 
        and common PLP.  
        For single PLP systems, only able to monitor the data PLP.

 @param pDemod The demodulator instance.
 @param type The type of the PLP to monitor.
 @param pPLPInfo The PLP structure to receive the PLP information into.

 @return SONY_DVB_OK if pPLPInfo is valid, otherwise an error is returned.
*/
sony_dvb_result_t dvb_demod_monitorT2_ActivePLP (sony_dvb_demod_t * pDemod,
                                                 sony_dvb_dvbt2_plp_btype_t type, 
                                                 sony_dvb_dvbt2_plp_t * pPLPInfo);

/**
 @brief Monitor the data PLP error indicator. A data PLP error is indicated
        when the selected PLP was not found in the channel.

 @param pDemod The demodulator instance.
 @param pPLPError The error indicated.
        - 0: No data PLP error.
        - 1: Data PLP error detected.

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_monitorT2_DataPLPError (sony_dvb_demod_t * pDemod, 
                                                    uint8 * pPLPError);

/**
 @brief Monitor the L1 change indicator. 
 
 @param pDemod The demodulator instance.
 @param pL1Change Indicator of L1 change.
        - 0: L1 has not changed.
        - 1: L1 changed.

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_monitorT2_L1Change (sony_dvb_demod_t * pDemod, 
                                                uint8 * pL1Change);

/**
 @brief Monitors the synchronisation state of the T2 demodulator.
 
 @param pDemod The demodulator instance.
 @param pSyncStat The demodulator state.
        - 0: WAIT_GO, 
        - 1: WAIT_AGC, 
        - 2: WAIT_P1DET, 
        - 3: WAIT_L1PRE, 
        - 4: WAIT_L1POST, 
        - 5: WAIT_DMD_OK, 
        - 6: DMD_OK
 @param pTSLockStat Indicates the TS lock condition.
        - 0: TS not locked.
        - 1: TS locked.

 @return SONY_DVB_OK if pSyncStat, pTSLockStat is valid, otherwise an error is returned.
*/
sony_dvb_result_t dvb_demod_monitorT2_SyncStat (sony_dvb_demod_t * pDemod, 
                                                uint8 * pSyncStat, 
                                                uint8 * pTSLockStat) ;

/**
 @brief Monitors the spectrum sense used by the demodulator.
        DVB-T2 does not support automatic spectrum sense detection.

 @param pDemod The demodulator instance.
 @param pSense The detected spectrum sense.

 @return SONY_DVB_OK if successful and pSense is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_SpectrumSense (sony_dvb_demod_t* pDemod,
                                                     sony_dvb_demod_spectrum_sense_t* pSense);

/**
 @brief Monitors the basic L1-post data.
        The device must have L1-post lock before calling, 
        if L1 post lock is not detected and error is returned.

 @param pDemod The demodulator instance.
 @param pL1Post The L1 post data monitored.

 @return SONY_DVB_OK if successful and pL1Post is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_L1Post (sony_dvb_demod_t* pDemod,
                                              sony_dvb_dvbt2_l1post_t* pL1Post);

/**
 @brief Monitor the pre-BCH BER params. This provides the 
        data PLP in multiple PLP case.

 @param pDemod The demodulator instance.
 @param pBitError The number of bit errors detected.
 @param pPeriod The number of bits over which the bit errors were measured.

 @return SONY_DVB_OK if the pBitError, pPeriod is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_PreBCHBERParam(sony_dvb_demod_t* pDemod,
                                                     uint32* pBitError,
                                                     uint32* pPeriod);

/**
 @brief Monitor the pre-BCH BER.

 @param pDemod The demodulator instance.
 @param pBER  The returned BER in 1e9.

 @return SONY_DVB_OK if pBER is valid.
*/
sony_dvb_result_t dvb_demod_monitorT2_PreBCHBER(sony_dvb_demod_t* pDemod,
                                                uint32* pBER);

/**
 @brief Monitor the sampling frequency offset value.
        PPM = ( (TRL_MON - TRL_NOMINALRATE) / TRL_NOMINALRATE) * 1e6
        
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
sony_dvb_result_t dvb_demod_monitorT2_SamplingOffset (sony_dvb_demod_t * pDemod, 
                                                      int32* pPPM);

#endif /* SONY_DVB_DEMOD_MONITORT2_H */
