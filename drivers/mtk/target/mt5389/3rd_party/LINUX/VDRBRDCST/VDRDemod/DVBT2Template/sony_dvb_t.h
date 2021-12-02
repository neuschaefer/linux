/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**

 @file    sony_dvb_t.h

          This file provides DVB-T related type definitions.

*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DVB_T_H_
#define SONY_DVB_T_H_

#include "sony_dvb.h"

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/
/* DVB-T definitions */
/**
 @brief DVB-T bandwidths
*/
#define SONY_DVBT_BW_5_MHZ                            (5)     /**< DVBT BW 5MHZ in MHz. */
#define SONY_DVBT_BW_6_MHZ                            (6)     /**< DVBT BW 6MHZ in MHz. */
#define SONY_DVBT_BW_7_MHZ                            (7)     /**< DVBT BW 7MHZ in MHz. */
#define SONY_DVBT_BW_8_MHZ                            (8)     /**< DVBT BW 8MHZ in MHz. */

/**
 @brief DVB-T constellation.
*/
typedef enum {
    SONY_DVBT_CONSTELLATION_QPSK,            /**< QPSK */
    SONY_DVBT_CONSTELLATION_16QAM,           /**< 16-QAM */
    SONY_DVBT_CONSTELLATION_64QAM,           /**< 64-QAM */
    SONY_DVBT_CONSTELLATION_RESERVED_3       /**< Reserved. */
} sony_dvbt_constellation_t;

/**
 @brief DVB-T hierarchy.
*/
typedef enum {
    SONY_DVBT_HIERARCHY_NON,                 /**< Non-hierarchical service. */
    SONY_DVBT_HIERARCHY_1,                   /**< a = 1. */
    SONY_DVBT_HIERARCHY_2,                   /**< a = 2. */
    SONY_DVBT_HIERARCHY_4                    /**< a = 4. */
} sony_dvbt_hierarchy_t;

/**
 @brief DVB-T code rate.
*/
typedef enum {
    SONY_DVBT_CODERATE_1_2,                  /**< 1/2 */
    SONY_DVBT_CODERATE_2_3,                  /**< 2/3 */
    SONY_DVBT_CODERATE_3_4,                  /**< 3/4 */
    SONY_DVBT_CODERATE_5_6,                  /**< 5/6 */
    SONY_DVBT_CODERATE_7_8,                  /**< 7/8 */
    SONY_DVBT_CODERATE_RESERVED_5,           /**< CR reserved 5 */
    SONY_DVBT_CODERATE_RESERVED_6,           /**< CR reserved 6 */
    SONY_DVBT_CODERATE_RESERVED_7            /**< CR reserved 7 */
} sony_dvbt_coderate_t;

/**
 @brief DVB-T guard interval.
*/
typedef enum {
    SONY_DVBT_GUARD_1_32,                    /**< 1/32 */
    SONY_DVBT_GUARD_1_16,                    /**< 1/16 */
    SONY_DVBT_GUARD_1_8,                     /**< 1/8  */
    SONY_DVBT_GUARD_1_4                      /**< 1/4  */
} sony_dvbt_guard_t;

/**
 @brief DVB-T mode.
*/
typedef enum {
    SONY_DVBT_MODE_2K,                       /**< 2k mode */
    SONY_DVBT_MODE_8K,                       /**< 8k mode */
    SONY_DVBT_MODE_RESERVED_2,               /**< Mode Reserved 2 */
    SONY_DVBT_MODE_RESERVED_3                /**< Mode Reserved 3 */
} sony_dvbt_mode_t;

/**
 @brief DVB-T HP/LP profile.
*/
typedef enum {
    SONY_DVBT_PROFILE_HP = 0,                /**< HP */
    SONY_DVBT_PROFILE_LP                     /**< LP */
} sony_dvbt_profile_t;

/**
 @brief The TPS information structure.
*/
typedef struct {

    /**
     @brief DVB-T constellation.
    */
    sony_dvbt_constellation_t constellation;

    /**
     @brief DVB-T hierarchy.
    */
    sony_dvbt_hierarchy_t hierarchy;

    /**
     @brief The HP code rate.
    */
    sony_dvbt_coderate_t rateHP;

    /**
     @brief The LP code rate.
    */
    sony_dvbt_coderate_t rateLP;

    /**
     @brief The guard interval.
    */
    sony_dvbt_guard_t guard;

    /**
     @brief The mode.
    */
    sony_dvbt_mode_t mode;

    /**
     @brief The f number.
    */
    uint8_t fnum;

    /**
     @brief The Length indicator.
    */
    uint8_t lengthIndicator;

    /**
     @brief The Cell ID.
    */
    uint16_t cellID;

    /**
     @brief Reserved (Even).
    */
    uint8_t reservedEven;

    /**
     @brief Reserved (Odd).
    */
    uint8_t reservedOdd;

} sony_dvbt_tpsinfo_t;

/** 
 @brief Tuning parameters specific to DVB-T.
*/
typedef struct sony_dvbt_tune_params_t {

    /**
     @brief Indicates the HP/LP profile to be selected.
    */
    sony_dvbt_profile_t profile;

    /**
     @brief Indicates whether the following (mode and gi) preset 
            information should be used (1) or not (0).
            Using preset tuning can help speed up acquisition and help in difficult 
            channels.

            - 1: sony_dvb_demod_t_tune_params_t::mode and sony_dvb_demod_t_tune_params_t::gi are 
               forced in the demodulator. The mode and guard can be retrieved from 
               ::dvb_demod_monitorT_TPSInfo.
            - 0: sony_dvb_demod_t_tune_params_t::mode and sony_dvb_demod_t_tune_params_t::gi are 
               ignored.
    */
    uint8_t usePresets;

    /**
     @brief If using presets, force the demodulator to acquire with the given mode.
    */
    sony_dvbt_mode_t mode;

    /**
     @brief If using presets, force the demodulator to acquire with the given gi.
    */
    sony_dvbt_guard_t gi;


} sony_dvbt_tune_params_t;

#endif
