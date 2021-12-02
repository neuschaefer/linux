/*------------------------------------------------------------------------------

 <dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
#ifndef SONY_DVBT_DEMOD_H
#define SONY_DVBT_DEMOD_H
#include "sony_dvb_demod.h"
#ifndef SONY_DVB_SUPPORT_DVBT

#define sony_dvb_demodT_ScanInitialize(pDemod, pScanParams)         SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT_ScanFinalize(pDemod, pScanParams)           SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT_TuneInitialize(pDemod,pTuneParams)          SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT_Tune(pDemod)                                SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT_Sleep(pDemod)                               SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT_BandwidthChanged(pDemod)                    SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT_EnableBlindAcquisition(a)                   SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT_CheckDemodLock(a,b)                         SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT_CheckTSLock(a,b)                            SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT_SetConfig(a,b,c)                            SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodT_CheckBandwidth(bandWidth)                   SONY_RESULT_ERROR_NOSUPPORT

#else
/*
NOTE: The following functions should not be called directly. Use functions defined in sony_dvb_demod.h
        sony_dvb_demodT_ScanInitialize
        sony_dvb_demodT_ScanFinalize
        sony_dvb_demodT_TuneInitialize
        sony_dvb_demodT_Tune
        sony_dvb_demodT_Sleep
        sony_dvb_demodT_BandwidthChanged
        sony_dvb_demodT_EnableBlindAcquisition
*/
sony_result_t sony_dvb_demodT_ScanInitialize (sony_dvb_demod_t * pDemod, sony_dvb_scan_params_t * pScanParams);

sony_result_t sony_dvb_demodT_ScanFinalize (sony_dvb_demod_t * pDemod, sony_dvb_scan_params_t * pScanParams);

sony_result_t sony_dvb_demodT_TuneInitialize (sony_dvb_demod_t * pDemod, sony_dvb_tune_params_t * pTuneParams);

sony_result_t sony_dvb_demodT_Tune (sony_dvb_demod_t * pDemod);

sony_result_t sony_dvb_demodT_Sleep (sony_dvb_demod_t * pDemod);

sony_result_t sony_dvb_demodT_BandwidthChanged (sony_dvb_demod_t * pDemod);

sony_result_t sony_dvb_demodT_EnableBlindAcquisition (sony_dvb_demod_t * pDemod);

sony_result_t sony_dvb_demodT_CheckDemodLock (sony_dvb_demod_t * pDemod, sony_dvb_demod_lock_result_t * pLock);

sony_result_t sony_dvb_demodT_CheckTSLock (sony_dvb_demod_t * pDemod, sony_dvb_demod_lock_result_t * pLock);

sony_result_t sony_dvb_demodT_SetConfig (sony_dvb_demod_t * pDemod, sony_dvb_demod_config_id_t configId, int32_t value);

sony_result_t sony_dvb_demodT_CheckBandwidth(uint8_t bandWidth);

#endif
#endif
