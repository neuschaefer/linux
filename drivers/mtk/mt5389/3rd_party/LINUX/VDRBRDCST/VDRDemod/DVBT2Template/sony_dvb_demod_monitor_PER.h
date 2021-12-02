#ifndef SONY_DVB_DEMOD_MONITOR_PER_H
#define SONY_DVB_DEMOD_MONITOR_PER_H

#include "sony_dvb_demod.h"

/**
 @brief Monitor the PER (Packet Error Rate) parameters.

 @param pDemod The demod instance.
 @param pPER The estimated PER x 1e6.
 
 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_monitor_PER (sony_dvb_demod_t * pDemod, uint32_t * pPER);

#endif /* SONY_DEMOD_MONITOR_PER_H */
