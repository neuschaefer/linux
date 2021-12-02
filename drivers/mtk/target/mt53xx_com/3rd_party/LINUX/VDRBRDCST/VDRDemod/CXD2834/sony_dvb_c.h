/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**

 @file    sony_dvb_c.h

          This file provides DVB-C related type definitions.

*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DVB_C_H_
#define SONY_DVB_C_H_

#include "sony_dvb.h"

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/
/* DVB-C definitions */
#define SONY_DVBC_BW_8_MHZ                     (8)     /**< DVBC BW 8MHZ in MHz. */
/**
 @brief DVB-C constellation.
*/
typedef enum {
    SONY_DVBC_CONSTELLATION_16QAM,           /**< 16-QAM */
    SONY_DVBC_CONSTELLATION_32QAM,           /**< 32-QAM */
    SONY_DVBC_CONSTELLATION_64QAM,           /**< 64-QAM */
    SONY_DVBC_CONSTELLATION_128QAM,          /**< 128-QAM */
    SONY_DVBC_CONSTELLATION_256QAM           /**< 256-QAM */
} sony_dvbc_constellation_t;

#endif
