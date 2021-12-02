/*------------------------------------------------------------------------------

 <dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
#ifndef SONY_DVBT2_DEMOD_H
#define SONY_DVBT2_DEMOD_H
#include "sony_dvb_demod.h"
#ifndef SONY_DVB_SUPPORT_DVBT2

#define sony_dvb_demodT2_ScanInitialize(pDemod, pScanParams)    SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_ScanFinalize(pDemod, pScanParams)      SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_TuneInitialize(pDemod,pTuneParams)     SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_TuneFinalize(pDemod,pTuneParams,tuneResult)       SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_Tune(pDemod)                           SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_Sleep(pDemod)                          SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_BandwidthChanged(pDemod)               SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_EnableBlindAcquisition(pDemod)         SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_CheckTSLock(pDemod, pLock)             SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_CheckDemodLock(pDemod,pLock)           SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_SetPLPConfig(pDemod,autoPLP,plpId)     SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_SetConfig(pDemod,configId,value)       SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_OptimizeMISO(pDemod)                   SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_InitializeMISOSetting(pDemod)          SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT2_CheckBandwidth(bandWidth)              SONY_RESULT_ERROR_NOSUPPORT

#else
/*
NOTE: The following functions should not be called directly. Use functions defined in sony_dvb_demod.h
        sony_dvb_demodT2_ScanInitialize
        sony_dvb_demodT2_ScanFinalize
        sony_dvb_demodT2_TuneInitialize
        sony_dvb_demodT2_TuneFinalize
        sony_dvb_demodT2_Tune
        sony_dvb_demodT2_Sleep
        sony_dvb_demodT2_BandwidthChanged
        sony_dvb_demodT2_EnableBlindAcquisition
*/
sony_result_t sony_dvb_demodT2_ScanInitialize (sony_dvb_demod_t * pDemod, sony_dvb_scan_params_t * pScanParams);

sony_result_t sony_dvb_demodT2_ScanFinalize (sony_dvb_demod_t * pDemod, sony_dvb_scan_params_t * pScanParams);

sony_result_t sony_dvb_demodT2_TuneInitialize (sony_dvb_demod_t * pDemod, sony_dvb_tune_params_t * pTuneParams);

sony_result_t sony_dvb_demodT2_TuneFinalize (sony_dvb_demod_t * pDemod, sony_dvb_tune_params_t * pTuneParams, sony_result_t tuneResult);

sony_result_t sony_dvb_demodT2_Tune (sony_dvb_demod_t * pDemod);

sony_result_t sony_dvb_demodT2_Sleep (sony_dvb_demod_t * pDemod);

sony_result_t sony_dvb_demodT2_BandwidthChanged (sony_dvb_demod_t * pDemod);

sony_result_t sony_dvb_demodT2_EnableBlindAcquisition (sony_dvb_demod_t * pDemod);

sony_result_t sony_dvb_demodT2_CheckDemodLock (sony_dvb_demod_t * pDemod, sony_dvb_demod_lock_result_t * pLock);


sony_result_t sony_dvb_demodT2_CheckTSLock (sony_dvb_demod_t * pDemod, sony_dvb_demod_lock_result_t * pLock);


/**
 @brief Setup the PLP configuration of the demodulator. Selecting both the device 
        PLP operation (automatic/manual PLP select) and the PLP to be selected
        in manual PLP mode.

 @param pDemod The demod instance.
 @param autoPLP The auto PLP setting.
        - 0x00: Fully automatic. The first PLP found during acquisition will be output.
        - 0x01: The data PLP ID set by plpId will be output.
               If the PLP with the ID is not found, then a PLP error is indicated
               (::dvb_demod_monitorT2_DataPLPError) but the 
               demod will still output the first found data PLP Id.
 @param plpId The PLP Id to set.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodT2_SetPLPConfig (sony_dvb_demod_t * pDemod, uint8_t autoPLP, uint8_t plpId);

sony_result_t sony_dvb_demodT2_SetConfig (sony_dvb_demod_t * pDemod, sony_dvb_demod_config_id_t configId, int32_t value);

sony_result_t sony_dvb_demodT2_OptimizeMISO (sony_dvb_demod_t * pDemod);

sony_result_t sony_dvb_demodT2_InitializeMISOSetting (sony_dvb_demod_t * pDemod);

sony_result_t sony_dvb_demodT2_CheckBandwidth(uint8_t bandWidth);

#endif
#endif
