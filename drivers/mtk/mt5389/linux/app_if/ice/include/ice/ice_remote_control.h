/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief   ICE remote control interface
 *
 * This file defines the ICE interfaces to the remote control.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 */

#ifndef CABOT_ICE_ICE_REMOTE_CONTROL_H_INCLUDED
#define CABOT_ICE_ICE_REMOTE_CONTROL_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_keypress_id.h"
#include "config/include_rs232_ctrl_if.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Pass a keypress to the Eclipse core for processing.
 *
 * This is a call made by the ICE-implementation into the Eclipse core.
 * The ICE-implementation must ensure that thread synchronisation and
 * inter-thread messaging can be used from inside the ICE_KeypressFunction().
 *
 * @param key The ID of the key that has been pressed.
 * @param userdata Pointer to data that was passed in when the callback
 *        was registered.
 */
typedef void (*ICE_KeyEventFunction)(ICE_KeypressId key, void* userdata);

/**
 * @brief Initialises remote controller.
 *
 * ICE_RemoteInit() is called by the client to initialise an ICE
 * Remote Controller.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_RemoteInit(void);

/**
 * @brief Set the callback function to handle remote control key presses.
 *
 * Set the function to be used for handling key presses from the remote
 * control.  Once the callback function is set, keypresses passed to the
 * callback will be processed, either by Eclipse or by being passed to the
 * UI application.  Until ICE_RemoteRegisterKeypressFunction() is called,
 * remote control key presses should be discarded.
 *
 * @param new_function The function to be called to handle remote
 *        control key presses.
 * @param userdata Data to be passed in a callback alongside key code.
 */
void ICE_RemoteRegisterKeypressFunction(ICE_KeyEventFunction new_function,
                                        void* userdata);

/**
 * @brief Set the callback function to handle remote control key presses.
 *
 * Set the function to be used for handling key presses from the remote
 * control.  Once the callback function is set, keypresses passed to the
 * callback will be processed, either by Eclipse or by being passed to the
 * UI application.  Until ICE_RemoteRegisterKeypressFunction() is called,
 * remote control key presses should be discarded.
 *
 * @param new_function The function to be called to handle remote
 *        control key presses.
 * @param userdata Data to be passed in a callback alongside key code.
 */
void ICE_RemoteRegisterKeyreleaseFunction(ICE_KeyEventFunction new_function,
                                        void* userdata);

/**
* @brief Query function to check if auto repeat is enabled for the required
*        key.
*
* @param key The required #ICE_KeypressId key
*
* @return #frost_true if auto repeat is enabled for the key,
*         #frost_false otherwise
*/
frost_bool ICE_IsKeyRepeatOn(ICE_KeypressId key);

/**
* @brief Enable auto repeat for the required key
*
* @param key The required #ICE_KeypressId key
* @return #frost_true if the request could be satisfied,
*         #frost_false otherwise.
*/
frost_bool ICE_KeyRepeatOn(ICE_KeypressId key);

/**
* @brief Disable auto repeat for the required key
*
* @param key The required #ICE_KeypressId key
* @return #frost_true if the request could be satisfied,
*         #frost_false otherwise.
*/
frost_bool ICE_KeyRepeatOff(ICE_KeypressId key);

/**
* @brief Set the auto repeat key interval in milliseconds.
*
* @note not all platforms may be able to implement this function.
*
* @param initial_delay_ms The delay after the key is pressed before repeating
*                         should start, in milliseconds.
* @param repeat_interval_ms The time between key repeats, in milliseconds.
*
* @return #frost_true if the request could be satisfied,
*         #frost_false otherwise.
*/
frost_bool ICE_SetKeyRepeatParameters(frost_uint initial_delay_ms,
                                      frost_uint repeat_interval_ms);

/**
 * @brief Determines if the remote control supports all of the optional
 *        non-linear playback keys (NLK).
 *
 * These keys are:
 * - ICE_keypress_stop.
 * - Either ICE_keypress_play and ICE_keypress_pause or a combined key
 *   ICE_keypress_playpause.
 * - Either ICE_keypress_skip_forward or ICE_keypress_fforward or both.
 * - Either ICE_keypress_skip_backward or ICE_keypress_rewind or both.
 *
 * @return frost_true if the NLK are supported, frost_false otherwise.
 * @remark Implemented by Receiver
 *
 * @note Must only be implemented if the Cabot MHEG-5 engine is integrated.
 */
frost_bool ICE_RemoteNonLinearPlaybackKeysSupported(void);

/**
 * @brief Determines if the remote control supports the specified key.
 *
 * @param keypress_id The key for which support is to be tested.
 *
 * @return frost_true if the key is supported, frost_false otherwise.
 * @remark Implemented by Receiver
 *
 * @note Must only be implemented if the Cabot MHEG-5 engine is integrated and
 * the CIPlus profile v1.3 is enabled.
 */
frost_bool ICE_IsKeySupported(ICE_KeypressId keypress_id);

/**
* @brief Set the system code
*
* @note not all IR protocols may be able to implement this function.
*
* @param enable Use the new system code or use the hardcoded one
* @param system_code use new rc system code.
*
* @return #frost_true if no error,
*         #frost_false otherwise.
*/
frost_bool ICE_SetRCSystemCode(frost_bool enable, frost_uint8 system_code);

frost_bool ICE_VirtualRemoteInit(void);

frost_bool ICE_VirtualRemoteClose(void);

frost_bool ICE_VirtualRemoteDisable(void);

frost_bool ICE_VirtualRemoteEnable(void);

void ICE_CallKeyPressFunction(ICE_KeypressId KeyPressId);

#ifdef INCLUDE_RS232_CTRL_IF

/**
 * @brief Disables IR key presses
 */
frost_bool ICE_EnableIRKeypresses(frost_bool enable);

typedef enum
{
    notify_ir_keys,
    notify_local_keys
} ICE_RemoteRS232NotifyKeys;

frost_bool ICE_EnableRS232KeyPressNotifications(ICE_RemoteRS232NotifyKeys keys, frost_bool enable);

frost_bool ICE_HandleRs232IrCommand(char system, char subsystem, char command, char status);

#endif

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_REMOTE_CONTROL_H_INCLUDED*/
