/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**
 @file    sony_dvb_demod_monitorC2.h

          This file provides the DVB-C2 demodulator monitor interface.

*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DVB_DEMOD_MONITORC2_H
#define SONY_DVB_DEMOD_MONITORC2_H

#include "sony_dvb_demod.h"

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/
#define c2_FreezeReg(pDemod) ((pDemod)->pI2c->WriteOneRegister ((pDemod)->pI2c, (pDemod)->i2cAddress, 0x01, 0x01)) /**< */
#define c2_UnFreezeReg(pDemod) ((void)((pDemod)->pI2c->WriteOneRegister ((pDemod)->pI2c, (pDemod)->i2cAddress, 0x01, 0x00)))/**< */

#define SONY_DVBC2_L1_DATA_NOT_READY    0       /**< L1 data is not currently valid */
#define SONY_DVBC2_L1_DATA_READY        1       /**< L1 data is valid and can be read from the appropriate registers */

/*------------------------------------------------------------------------------
 Types
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
 Monitor Functions
------------------------------------------------------------------------------*/
#ifndef SONY_DVB_SUPPORT_DVBC2

#define sony_dvb_demod_monitorC2_IFAGCOut(pDemod, pIFAGC)                                    SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_SpectrumSense(pDemod, pSense)                                SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_CarrierOffset( pDemod, pOffset)                            SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_MER(pDemod, pMER)                                          SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_PreLDPCBER(pDemod,pBER)                                    SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_PreBCHBER(pDemod, pBER)                                    SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_PostBCHFER(pDemod, pFER)                                   SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_PacketErrorNumber(pDemod, pPEN)                            SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_SyncStat(pDemod, pSyncStat, pTSLockStat, pUnlockDetected)  SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_TsRate(pDemod, pTsRateKbps)                                SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_L1Data(pDemod, l1Data)                                     SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_QAM(pDemod, type, pQAM)                                    SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_LDPCCodeLength(pDemod, type, pCodeLength)                  SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_CodeRate(pDemod, type, pCodeRate)                          SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demod_monitorC2_SamplingOffset(pDemod,pPPM)                SONY_RESULT_ERROR_NOSUPPORT

#else
/**
 @brief Monitor the IFAGC value of the C2 demodulator.
        Actual dB gain dependent on attached tuner.

 @param pDemod The demodulator instance.
 @param pIFAGC The IFAGC output register value. Range 0 - 0xFFF. Unitless.

 @return SONY_RESULT_OK if pIFAGC is valid.
*/
sony_result_t sony_dvb_demod_monitorC2_IFAGCOut (sony_dvb_demod_t * pDemod, uint32_t * pIFAGC);

/**
 @brief Monitors the spectrum sense used by the demodulator.
        DVB-C2 supports automatic spectrum sense detection.

 @param pDemod The demodulator instance.
 @param pSense The detected spectrum sense.

 @return SONY_RESULT_OK if successful and pSense is valid.
*/
sony_result_t sony_dvb_demod_monitorC2_SpectrumSense (sony_dvb_demod_t * pDemod, sony_dvb_demod_spectrum_sense_t * pSense);

/**
 @brief Monitor the carrier offset of the currently tuned channel.
        This uses the continual pilot (CP) carrier offset estimation
        from the device.
        To get the estimated centre frequency of the current channel:
        Fest = Ftune (KHz) + pOffset (KHz) ;

 @param pDemod The demodulator instance.
 @param pOffset The detected carrier offset in KHz.

 @return SONY_RESULT_OK if pOffset is valid.
*/
sony_result_t sony_dvb_demod_monitorC2_CarrierOffset (sony_dvb_demod_t * pDemod, int32_t * pOffset);

/**
 @brief Monitor the MER (Modulation Error Ratio) of the DVB-C2
        demodulator.

 @param pDemod The demodulator instance.
 @param pMER The returned MER in dB x 1000.

 @return SONY_RESULT_OK if pMER is valid.
*/
sony_result_t sony_dvb_demod_monitorC2_MER (sony_dvb_demod_t * pDemod, int32_t * pMER);

/**
 @brief Monitor the pre-LDPC BER. This provides the
        data PLP BER in multiple PLP case.

 @param pDemod The demodulator instance.
 @param pBER The returned BER in 1e7.

 @return SONY_RESULT_OK if the pBER is valid.
*/
sony_result_t sony_dvb_demod_monitorC2_PreLDPCBER (sony_dvb_demod_t * pDemod, uint32_t * pBER);

/**
 @brief Monitor the pre-BCH BER.

 @param pDemod The demodulator instance.
 @param pBER  The returned BER in 1e9.

 @return SONY_RESULT_OK if pBER is valid.
*/
sony_result_t sony_dvb_demod_monitorC2_PreBCHBER (sony_dvb_demod_t * pDemod, uint32_t * pBER);

/**
 @brief Monitor the post BCH FER (FEC block error rate) parameters.

 @param pDemod The demod instance.
 @param pFER The estimated FER x 1e6.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_monitorC2_PostBCHFER (sony_dvb_demod_t * pDemod, uint32_t * pFER);

/**
 @brief Monitor the Packet Error Number.

 @param pDemod The demodulator instance.
 @param pPEN  The returned Packet Error Number.

 @return SONY_RESULT_OK if pPEN is valid.
*/
sony_result_t sony_dvb_demod_monitorC2_PacketErrorNumber (sony_dvb_demod_t * pDemod, uint32_t * pPEN);

/**
 @brief Monitors the synchronisation state of the C2 demodulator.

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
 @param pUnlockDetected Indicates an early unlock condition
        - 0: No early unlock.
        - 1: Early unlock detected.

 @return SONY_RESULT_OK if pSyncStat, pTSLockStat is valid, otherwise an error is returned.
*/
sony_result_t sony_dvb_demod_monitorC2_SyncStat (sony_dvb_demod_t * pDemod, uint8_t * pSyncStat, uint8_t * pTSLockStat, uint8_t * pUnlockDetected);

/**
 @brief Monitor the demodulator *estimated* DVB-C2 TS (Transport Stream) rate.

 @param pDemod The demodulator instance.
 @param pTsRateKbps The estimated TS rate in kbps.

 @return SONY_RESULT_OK if pTsRateKbps is valid.
*/
sony_result_t sony_dvb_demod_monitorC2_TsRate (sony_dvb_demod_t * pDemod, uint32_t * pTsRateKbps);

/**
 @brief Monitor the DVB-C2 L1 ignalling information. L1 signalling
        information is available when TS is locked, and earlier in the
        acquisition sequence, after the P1 symbols have been decoded.
        In the case of being tuned to a Data Slice with a broadband nothc
        then no L1 signalling is availabe.

 @param pDemod The demodulator instance.
 @param l1Data Pointer to receive the L1signalling information.

 @return SONY_RESULT_OK if the l1Data is valid.
*/
sony_result_t sony_dvb_demod_monitorC2_L1Data (sony_dvb_demod_t * pDemod, sony_dvbc2_l1_t * l1Data);

/**
 @brief Monitor the DVB-C2 active PLP QAM
        For multiple PLP systems, then able to monitor both the data
        and common PLP.
        For single PLP systems, only able to monitor the data PLP.

 @param pDemod The demodulator instance.
 @param type The type of the PLP to monitor.
 @param pQAM To receive the PLP QAM information into.

 @return SONY_RESULT_OK if pPLPInfo is valid, otherwise an error is returned.
*/
sony_result_t sony_dvb_demod_monitorC2_QAM (sony_dvb_demod_t * pDemod, sony_dvbc2_plp_btype_t type,
                                                sony_dvbc2_constellation_t * pQAM);

/**
 @brief Monitor the DVB-C2 active PLP Code Length
        For multiple PLP systems, then able to monitor both the data
        and common PLP.
        For single PLP systems, only able to monitor the data PLP.

 @param pDemod The demodulator instance.
 @param type The type of the PLP to monitor.
 @param pCodeLength To receive the PLP code length information into.

 @return SONY_RESULT_OK if pPLPInfo is valid, otherwise an error is returned.
*/
sony_result_t sony_dvb_demod_monitorC2_LDPCCodeLength (sony_dvb_demod_t * pDemod, sony_dvbc2_plp_btype_t type,
                                                           sony_dvbc2_ldpc_code_length_t * pCodeLength);

/**
 @brief Monitor the DVB-C2 active PLP Code Rate
        For multiple PLP systems, then able to monitor both the data
        and common PLP.
        For single PLP systems, only able to monitor the data PLP.

 @param pDemod The demodulator instance.
 @param type The type of the PLP to monitor.
 @param pCodeRate To receive the PLP code rate information into.

 @return SONY_RESULT_OK if pPLPInfo is valid, otherwise an error is returned.
*/
sony_result_t sony_dvb_demod_monitorC2_CodeRate (sony_dvb_demod_t * pDemod, sony_dvbc2_plp_btype_t type,
                                                     sony_dvbc2_plp_code_rate_t * pCodeRate);

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

 @return SONY_RESULT_OK if pPPM is valid.
*/
sony_result_t sony_dvb_demod_monitorC2_SamplingOffset (sony_dvb_demod_t * pDemod, int32_t * pPPM);


#endif /* SONY_DVB_SUPPORT_DVBC2 */

#endif /* SONY_DVB_DEMOD_MONITORC2_H */
