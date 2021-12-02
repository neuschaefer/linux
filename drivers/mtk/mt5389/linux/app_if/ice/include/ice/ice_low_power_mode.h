/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot's interface between the Eclipse middleware running on the Main CPU
 * to a Low Power Microcontroller (MCU).
 *
 * The low power MCU acts a slave device to the main CPU.
 *
 * The MCU has no Real-Time Clock (RTC) just a counter. A periodic update event,
 * #ICE_low_power_exit_periodical_update, will occur every 1 hour i.e. 3600
 * seconds so that the low power MCU can synchronise with Eclipse.
 *
 * If there is a scheduled timer event less than one hour then this time in seconds
 * will be the value passed to the #ICE_LowPowerModeSleep(). 
 *
 * Otherwise if there is no scheduled timer then enter lower power mode
 * until the next periodic update.
 *
 */

#ifndef CABOT_ICE_ICE_LOW_POWER_MODE_H_INCLUDED
#define CABOT_ICE_ICE_LOW_POWER_MODE_H_INCLUDED

#include "config/enable_fast_standby.h"

#include "ice/ice_keypress_id.h"
#include "ice/ice_low_power_mode_types.h"
#include "frost/frost_basictypes.h"

#include "config/enable_fast_standby.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  ENABLE_FAST_STANDBY
typedef enum
{
    ICE_power_up_mode_standby,
    ICE_power_up_mode_last_state,
    ICE_power_up_mode_always_on
} ICE_LowPowerModePowerUpMode;

/**
* Structure of  Power up paqrameters in Low power mode.
*/
typedef struct
{
    ICE_LowPowerModePowerUpMode profile_power_up_mode;
    ICE_LowPowerModePowerUpMode hotel_mode_power_up_mode;   
    frost_bool fti_completed;
    frost_bool last_state_standby;
    frost_bool child_lock_enable;
} ICE_LowPowerModePowerUpParameters;

frost_bool ICE_LowPowerModeGetPowerUpParameters(ICE_LowPowerModePowerUpParameters * power_up_params);

frost_bool ICE_LowPowerModeSetPowerUpParameters(ICE_LowPowerModePowerUpParameters power_up_params);
#endif

/**
  * This will give control to the low power MCU which will turn off power to the
  * Main CPU until the specified number of seconds or another event has occurred.
  *
  * @warning This function must not return and block until power is cut off.
  *
  * @param seconds_in_low_power specifies the number of seconds to remain in low
  * power mode.
  *
  * @note Power might be cut before #ICE_LowPowerModeSleep could return.
  *
  * @return #frost_false if this operation failed, #frost_true otherwise.
  */
frost_bool ICE_LowPowerModeSleep(frost_uint32 seconds_in_low_power);

/**
  * This function queries the low power MCU for the most recent reason for exiting
  * out of low power mode. This will be called by Eclipse during initialisation to help it
  * to determine the correct course of action.
  *
  * @return #ICE_LowPowerModeExitType the reason why low power mode was exited,
  *         #ICE_low_power_exit_unknown_reason otherwise.
  */
ICE_LowPowerModeExitType ICE_LowPowerModeQueryExitType(void);

/**
  * This function queries the low power MCU for the most recent command for the related type of exiting
  * out of low power mode. This will be called by Eclipse during initialisation to help it
  * to determine the correct course of action.
  *
  * @return The command why low power mode was exited,
  *         -1 if no command.
  */
frost_uint16 ICE_LowPowerModeQueryExitCommand(ICE_LowPowerModeExitType exit_type);

/**
  * This function queries the low power MCU for the last keypress that
  * it responded to.
  *
  * This will be called by Eclipse during initialisation if
  * #ICE_LowPowerModeQueryExitType returns #ICE_low_power_exit_keypress,
  * to help it to determine the correct course of action.
  *
  * @param key A pointer to return ICE_keypressId (if any). It is set to
  *            ICE_keypress_invalid if the command fails.
  *
  * @return #frost_false if no keypress was recorded, #frost_true otherwise.
  */
frost_bool ICE_LowPowerModeQueryKeypress(ICE_KeypressId *key);

/**
 * @brief Get the current Firmware version of the Low Power MCU.
 *
 * Retrieves the version of the Firmware version in the range 0x0001 to 0xFFFF.
 *
 * @return The implementation's 16-bit Firmware version number or 0 
 *         in case of error.
 */
frost_uint16 ICE_LowPowerModeQueryVersion(void);

/**
 * @brief Enables or disables the pc standby functionality which is used to power up the tv if a pc signal is detected in standby.
 *
 * @param enabled the state of pc standby functionality. Defines if the tv will detect pc signal in standby or not. 
 */
void ICE_LowPowerSetPCStandby(frost_bool enabled);

/**
 * @brief Initializes the local keys threshold levels. Sets the low and high values of adc for each local key.
 */
void ICE_LowPowerLocalKeysInit(void);

 /**
 * @brief Get the current Touchpad version of the Low Power MCU.
 *
 * @return The implementation's 16-bit Touchpad version number or 0
 *         in case of error.
 */

frost_uint16 ICE_LowPowerModeQueryTouchpadVersion(void);

frost_bool ICE_LowPowerSetChildLock(frost_bool enabled);

void ICE_LowPowerSetPin8Standby(frost_bool enabled);

void ICE_LowPowerSetILogoEnabled(frost_bool enabled);

void ICE_LowPowerSetILogoValue(frost_uint8 value);

void ICE_LowPowerSetPowerUpMode(frost_int8 Mode);

frost_bool ICE_LowPowerSetBlinking(frost_bool enabled);

frost_bool ICE_LowPowerEnablePanelProtection(frost_bool);

frost_bool ICE_LowPowerEnableMainICCheck(frost_bool);

frost_bool ICE_LowPowerLedOnOff(frost_bool);

frost_bool ICE_LowPowerTouchpadConfigure(ICE_KeypressId, frost_uint8, frost_uint8, frost_uint8);

frost_bool ICE_LowPowerTouchpadGetConfiguration(ICE_KeypressId, frost_uint8*, frost_uint8*, frost_uint8*);

frost_bool ICE_LowPowerResetMainIC();

frost_bool ICE_LowPowerSelfProgram(frost_int8 * path);

frost_bool ICE_LowPowerModeInit(void);

/**
 * @brief Returns true if front panel menu key is hold pressed (used in Loewe for opening hotel menu)
 */
frost_bool ICE_LowPowerIsFrontMenuPressed(void);

/**
 * @brief Queries last RS232 command received while in standby.
 *
 * @param last_rs232_cmd A pointer to char array containing last received command
 * @param max_cmd_length maximum command length
 *
 * @return true if query was successfull
 *
 */
frost_bool ICE_LowPowerModeQueryLastRS232CtrlIfCommand(frost_int8* last_rs232_cmd, frost_uint8 max_cmd_length);
#ifdef __cplusplus
}
#endif

#endif /* defined CABOT_ICE_ICE_LOW_POWER_MODE_H_INCLUDED */
