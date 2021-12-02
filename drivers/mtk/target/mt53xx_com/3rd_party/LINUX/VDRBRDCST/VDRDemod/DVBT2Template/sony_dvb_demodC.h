/*------------------------------------------------------------------------------

 <dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
#ifndef SONY_DVBC_DEMOD_H
#define SONY_DVBC_DEMOD_H
#include "sony_dvb_demod.h"
/*
NOTE: The following functions should not be called directly. Use functions defined in sony_dvb_demod.h
        sony_dvb_demodC_ScanInitialize
        sony_dvb_demodC_ScanFinalize
        sony_dvb_demodC_Tune
        sony_dvb_demodC_Sleep
        
*/
sony_result_t sony_dvb_demodC_ScanInitialize (sony_dvb_demod_t * pDemod, sony_dvb_scan_params_t * pScanParams);

sony_result_t sony_dvb_demodC_ScanFinalize (sony_dvb_demod_t * pDemod, sony_dvb_scan_params_t * pScanParams);

sony_result_t sony_dvb_demodC_Tune (sony_dvb_demod_t * pDemod);

sony_result_t sony_dvb_demodC_Sleep (sony_dvb_demod_t * pDemod);

sony_result_t sony_dvb_demodC_CheckDemodLock (sony_dvb_demod_t * pDemod, sony_dvb_demod_lock_result_t * pLock);

sony_result_t sony_dvb_demodC_CheckTSLock (sony_dvb_demod_t * pDemod, sony_dvb_demod_lock_result_t * pLock);

sony_result_t sony_dvb_demodC_SetConfig (sony_dvb_demod_t * pDemod, sony_dvb_demod_config_id_t configId, int32_t value);

sony_result_t sony_dvb_demodC_CheckBandwidth(uint8_t bandWidth);

#endif
