#ifndef SONY_DVB_DEMOD_MONITOR_SSI_H
#define SONY_DVB_DEMOD_MONITOR_SSI_H

#include "sony_dvb_demod.h"

/**
 @brief Monitor the SSI (Signal Strength Indicator).

 @param pDemod The demod instance.
 @param pSSI SSI value (0 - 100)
 
 @return SONY_RESULT_OK if successful.
*/

sony_result_t sony_dvb_demod_monitor_SSI (sony_dvb_demod_t * pDemod, uint8_t * pSSI);

#endif /* SONY_DVB_DEMOD_MONITOR_SSI_H */
