/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Definition of low power mode types.
 *
 */

#ifndef CABOT_ICE_ICE_LOW_POWER_MODE_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_LOW_POWER_MODE_TYPES_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration of possible power modes.
 */
typedef enum ICE_PowerMode
{
    /**
     * Low power mode.
     */
    ICE_power_mode_low_power,

    /**
     * Normal power mode.
     */
    ICE_power_mode_normal
} ICE_PowerMode;

/**
  * @brief Enumeration of reasons for exiting low power mode.
  */
typedef enum ICE_LowPowerModeExitType
{
    /**
    * Exited due to normal power up e.g. Factory state
    */
    ICE_low_power_exit_power_up = 0,

    /**
    * Exited due to a remote-control keypress
    * button.
    */
    ICE_low_power_exit_keypress,

    /**
    * Exited due to a scheduled Timer i.e. recording, OAD, or reset event.
    */
    ICE_low_power_exit_timer,

    /**
    * Exited due to a periodical update event occurred for time synchronisation.
    */
    ICE_low_power_exit_periodical_update,

    /**
    * Exited due to a CEC command.
    */
    ICE_low_power_exit_cec_line,

    /**
    * Exited due to a front panel keypress
    * button.
    */
    ICE_low_power_exit_localkeypress,

    /**
     * Exited due to PC input
     */
    ICE_low_power_exit_pcinput,

    /**
     * Exited due to PIN 8 of EXT1
     */
    ICE_low_power_exit_pin_8_ext1,

    /**
     * Exited due to PIN 8 of EXT2
     */
    ICE_low_power_exit_pin_8_ext2,

    /**
     * Exited due to DVD
     */
    ICE_low_power_exit_dvd,

    /**
     * Exited due to a command from 2M ISB device
     */
    ICE_low_power_exit_2m_isb_device,
        
    /**
     * Exited due to an RS232 ctrl IF command
     */
    ICE_low_power_exit_rs232_ctrl_if,

    /**
    * Exited due to an unknown reason.
    */
    ICE_low_power_exit_unknown_reason
} ICE_LowPowerModeExitType;

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_LOW_POWER_MODE_TYPES_H_INCLUDED */
