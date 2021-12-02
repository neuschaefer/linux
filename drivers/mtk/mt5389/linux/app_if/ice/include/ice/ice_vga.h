/**
 * Copyright 2002-5 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot's interface for controlling VGA / PC interfaces.
 */

#ifndef CABOT_ICE_ICE_VGA_H_INCLUDED
#define CABOT_ICE_ICE_VGA_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief List of possible gamma levels
 */
typedef enum ICE_VgaGammaLevel
{
    ice_vga_gamma_level_2_0,    /** Gamma level = 2.0 */
    ice_vga_gamma_level_2_2,    /** Gamma level = 2.2 */
    ice_vga_gamma_level_2_5     /** Gamma level = 2.5 */    
}ICE_VgaGammaLevel;


/**
 * @brief List of possible sync methods
 */
typedef enum ICE_VgaSyncMethod
{
    ice_vga_sync_horizontal_and_vertical,   /** Sync on horizontal and vertical */
    ice_vga_sync_on_green                   /** Sync on green */
} ICE_VgaSyncMethod;

/**
 * @brief Starts VGA/PC input auto mode detection and position process
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaAutoposition(void);

/**
 * @brief Sets a VGA/PC input's phase shift of the sampling clock
 *
 * @param percent_phase [i] New phase shift level as a percentage of
 *        the maximum allowed range. Values over 100 will be treated as 100.
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaSetPhase(frost_uint percent_phase);

/**
 * @brief Gets a VGA/PC input's phase shift of the sampling clock
 *
 * @param *percent_phase [o] Current phase shift level as a percentage of
 *        the maximum allowed range.
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaGetPhase(frost_uint *percent_phase);

/**
 * @brief Sets a VGA/PC input's sync method
 *
 * @param sync - see typedef ICE_VgaSyncMethod
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaSetSync(ICE_VgaSyncMethod sync);


/**
 * @brief Gets a VGA/PC input's current frequency shift of the sampling clock
 *
 * @param *sync - see typedef ICE_VgaSyncMethod
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaGetSync(ICE_VgaSyncMethod *sync);

/**
 * @brief Sets a VGA/PC input's frequency shift of the sampling clock
 *
 * @param percent_clock [i] New clock shift level as a percentage of
 *        the maximum allowed range.  Values over 100 will be treated as 100.
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaSetClock(frost_uint percent_clock);


/**
 * @brief Gets a VGA/PC input's current frequency shift of the sampling clock
 *
 * @param *percent_clock [o] Current clock shift level as a percentage of
 *        the maximum allowed range.
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaGetClock(frost_uint *percent_clock);


/**
 * @brief Sets the VGA/PC input's Red Gain Level
 *
 * @param percent_gain [i] - New gain shift level as a percentage of
 *        the maximum allowed range.  Values over 100 will be treated as 100.
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaSetRedGain(frost_uint percent_gain);


/**
 * @brief Gets a VGA/PC input's Red Gain Level
 *
 * @param *percent_gain [o] Current gain shift level as a percentage of
 *        the maximum allowed range.
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaGetRedGain(frost_uint *percent_gain);


/**
 * @brief Sets the VGA/PC input's Green Gain Level
 *
 * @param percent_gain [i] - New gain shift level as a percentage of
 *        the maximum allowed range.  Values over 100 will be treated as 100.
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaSetGreenGain(frost_uint percent_gain);

/**
 * @brief Gets a VGA/PC input's Green Gain Level
 *
 * @param *percent_gain [o] Current gain shift level as a percentage of
 *        the maximum allowed range.
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaGetGreenGain(frost_uint *percent_gain);

/**
 * @brief Sets the VGA/PC input's Blue Gain Level
 *
 * @param percent_gain [i] - New gain shift level as a percentage of
 *        the maximum allowed range.  Values over 100 will be treated as 100.
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaSetBlueGain(frost_uint percent_gain);


/**
 * @brief Gets a VGA/PC input's Blue Gain Level
 *
 * @param *percent_gain [o] Current gain shift level as a percentage of
 *        the maximum allowed range.
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaGetBlueGain(frost_uint *percent_gain);


/**
 * @brief Sets the VGA/PC input's Gamma Level
 *
 * @param level [i] - allowable gamma levels
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaSetGammaLevel(ICE_VgaGammaLevel level);


/**
 * @brief Gets the VGA/PC input's Gamma Level
 *
 * @param *level [o] current gamma level
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_VgaGetGammaLevel(ICE_VgaGammaLevel *level);


/**
 * @brief Sets a VGA/PC input's vertical position shift of the PC Image
 *
 * @param percent_vshift [i] New vertical position shift level as a percentage of
 *        the maximum allowed range. Values over 100 will be treated as 100
 *
 * @return frost_true on success, frost_false otherwise.
 */
frost_bool ICE_VgaSetVShift(frost_uint percent_vshift);

/**
 * @brief Gets a VGA/PC input's vertical position shift of the PC Image
 *
 * @param *percent_vshift [o] Current vertical position shift level as a percentage of
 *        the maximum allowed range.
 *
 * @return frost_true on success, frost_false otherwise.
 */
frost_bool ICE_VgaGetVShift(frost_uint *percent_vshift);

/**
 * @brief Sets a VGA/PC input's horizontal position shift of the PC Image
 *
 * @param percent_hshift [i] New horizontal position shift level as a percentage of
 *        the maximum allowed range. Values over 100 will be treated as 100
 *
 * @return frost_true on success, frost_false otherwise.
 */
frost_bool ICE_VgaSetHShift(frost_uint percent_hshift);

/**
 * @brief Gets a VGA/PC input's horizontal position shift of the PC Image
 *
 * @param *percent_hshift [o] Current horizontal position shift level as a percentage of
 *        the maximum allowed range.
 *
 * @return frost_true on success, frost_false otherwise.
 */
frost_bool ICE_VgaGetHShift(frost_uint *percent_hshift);

#ifdef __cplusplus
} /* end of extern "C" block */
#endif

#endif /* CABOT_ICE_ICE_VGA_H_INCLUDED*/
