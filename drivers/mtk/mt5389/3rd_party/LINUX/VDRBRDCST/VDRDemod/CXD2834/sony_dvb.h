/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**

 @file    sony_dvb.h

          This file provides low level type definitions and trace definitions
          used across the driver.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DVB_H
#define SONY_DVB_H

#include "sony_common.h"

#if   (SONY_DVB_DEVICE_CXD2834 == 1)
#define SONY_DVB_SUPPORT_DVBC           /**< Driver supports DVBC. */
#define SONY_DVB_SUPPORT_DVBT           /**< Driver supports DVBT. */
#define SONY_DVB_SUPPORT_DVBT2          /**< Driver supports DVBT2. */
#define SONY_DVB_DEVICE_CXD2835 0       /**< C/C2 Demodulator */
#define SONY_DVB_DEVICE_CXD2836 0       /**< T/T2/C/C2 Demodulator */
#elif (SONY_DVB_DEVICE_CXD2835 == 1)
#define SONY_DVB_SUPPORT_DVBC           /**< Driver supports DVBC. */
#define SONY_DVB_SUPPORT_DVBC2          /**< Driver supports DVBC2. */
#define SONY_DVB_DEVICE_CXD2834 0       /**< T/T2/C Demodulator */
#define SONY_DVB_DEVICE_CXD2836 0       /**< T/T2/C/C2 Demodulator */
#elif (SONY_DVB_DEVICE_CXD2836 == 1)
#define SONY_DVB_SUPPORT_DVBC           /**< Driver supports DVBC. */
#define SONY_DVB_SUPPORT_DVBT           /**< Driver supports DVBT. */
#define SONY_DVB_SUPPORT_DVBT2          /**< Driver supports DVBT2. */
#define SONY_DVB_SUPPORT_DVBC2          /**< Driver supports DVBC2. */
#define SONY_DVB_DEVICE_CXD2835 0       /**< C/C2 Demodulator */
#define SONY_DVB_DEVICE_CXD2834 0       /**< T/T2/C Demodulator */
#else
#error SONY_DVB_DEVICE value not recognised
#endif

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/
/**
 @brief System(DVB-T/C/T2/C2) 
*/
typedef enum {
    SONY_DVB_SYSTEM_UNKNOWN,        /**< Unknown. */
    SONY_DVB_SYSTEM_DVBC,           /**< DVB-C. */
    SONY_DVB_SYSTEM_DVBT,           /**< DVB-T. */
    SONY_DVB_SYSTEM_DVBT2,          /**< DVB-T2. */
    SONY_DVB_SYSTEM_DVBC2           /**< DVB-C2. */
} sony_dvb_system_t;

/**
 @brief Crystal Frequency  
*/
typedef enum {
    SONY_DVB_41000kHz,      /**< 41 MHz   */
    SONY_DVB_20500kHz       /**< 20.5 MHz */
} sony_dvb_xtal_t;

#endif /* SONY_DVB_H */
