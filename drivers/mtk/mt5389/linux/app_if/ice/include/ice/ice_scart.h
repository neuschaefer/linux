/**
 * Copyright 2002-5 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot's interface for controlling SCART interfaces.
 */

#ifndef CABOT_ICE_ICE_SCART_H_INCLUDED
#define CABOT_ICE_ICE_SCART_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief SCART video output modes.
 */
typedef enum ICE_ScartVideoOutputMode
{
    ICE_scart_rgb,
    ICE_scart_cvbs,
    ICE_scart_svideo,
    ICE_scart_ypbpr

} ICE_ScartVideoOutputMode;


/**
 * @brief SCART display aspect ratios.
 */
typedef enum ICE_ScartAspectRatio
{
    ICE_scart_4_by_3,
    ICE_scart_16_by_9

} ICE_ScartAspectRatio;

/**
 * @brief SCART status.
 *
 * Determines status of SCART connectors with regards to content control.
 * If a SCART connector has been disabled via usage restrictions, it must not 
 * be re-enabled from another system call.
 *
 * ICE_scart_protected == SCART on/off cannot be modified by other ICE api's.
 * ICE_scart_clear == SCART on/off can be modified by other ICE api's.
 */
typedef enum ICE_ScartStatus
{
    ICE_scart_protected,
    ICE_scart_clear

} ICE_ScartStatus;

/**
 * Callback function to pass a SCART event to the Eclipse core for processing.
 *
 * This is a call made by the ICE-implementation into the Eclipse core.  The
 * ICE-implementation must ensure that thread synchronisation and inter-thread
 * messaging can be used from inside the ICE_ScartCallback().
 *
 * A proper ICE implementation should call registered callback function when
 * anything changes in any scart's status. This must include the scart number
 * (<i>scart_number</i>) and the status of scart (<i>external_video_active</i>) with
 * userdata received from #ICE_ScartRegisterFunction.
 *
 * Example:
 * If a device connected to the scart number 1 (VCR) is activated, then ICE
 * implementation should call the callback function with parameters
 *
 *      <i>scart_number</i> = 1<br>
 *      <i>external_video_active</i> = #frost_true<br>
 *      <i>userdata</i> = registered user data<br>
 *
 * @note Callback functions can be registered using #ICE_ScartRegisterFunction.
 *       If no callback function has been registered, this has nothing to do.
 *
 * @param scart_number SCART connector number.
 * @param external_video_active Set to #frost_true when external video starts,
 *                              #frost_false when it stops.
 * @param userdata Data to be passed to the callback.
 */
typedef void (*ICE_ScartCallback)(frost_uint scart_number,
                                  frost_bool external_video_active,
                                  void* userdata);

/**
 * @brief Set the callback to handle SCART connector notifications
 *
 * Set the function to be used for handling external video events from SCART
 * connector. Before this function is called, events should be discarded.
 *
 * @note During initialisation, Eclipse will register a call-back which may be
 *       called if a mechanism for handling these events is required. In this
 *       mechanism, a UIExternalVideoStatusChangedMessage will be sent to
 *       the UI Application.
 *
 *       However, the UI Application can override this behaviour and register
 *       its own call-back function. A UIExternalVideoStatusChangedMessage
 *       will not be sent in this case and the UI Application will have to
 *       handle states manually.
 *
 * @param scart_number SCART connector number.
 * @param callback The function to be called to handle SCART changes.
 * @param userdata Data to be passed to the callback.
 *
 * @return #frost_true on success or if not supported,
 *         #frost_false on failure.
 */
frost_bool ICE_ScartRegisterFunction(frost_uint scart_number,
                                     ICE_ScartCallback callback,
                                     void* userdata);

/**
 * @brief Switches a SCART interface on or off.
 *
 * The meaning of 'on' and 'off' will depend on the hardware configuration.
 * E.g. when turning off the TV SCART output, one configuration might be
 * to turn off all signalling on this SCART, and another might be to
 * route VCR SCART input to TV SCART output.
 *
 * @param scart_number Identifies the required SCART:
 *                     0 = primary SCART, 1 = secondary SCART.
 * @param on #frost_true means on, #frost_false means off.
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_ScartOnOff(frost_uint scart_number,
                          frost_bool on);
/**
 * @brief Switches a SCART interface on or off.
 *
 * Cannot be overridden by ICE_ScartOnOff. 
 * The meaning of 'on' and 'off' will depend on the hardware configuration.
 * E.g. when turning off the TV SCART output, one configuration might be
 * to turn off all signalling on this SCART, and another might be to
 * route VCR SCART input to TV SCART output.
 *
 * @param scart_number Identifies the required SCART:
 *                     0 = primary SCART, 1 = secondary SCART.
 * @param on #frost_true means on, #frost_false means off.
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_ScartForceOnOff(frost_uint scart_number,
                          frost_bool on);
/**
 * @brief Sets a SCART video output mode (e.g. rgb, cvbs ...).
 *
 * @param scart_number Identifies the required SCART:
 *                     0 = primary SCART, 1 = secondary SCART.
 * @param video_output_mode the new video output mode.
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_ScartSetVideoOutputMode(frost_uint scart_number,
                               ICE_ScartVideoOutputMode video_output_mode);


/**
 * @brief Configures the aspect ratio output on pin 8 of a SCART interface.
 *
 * Pin 8 of a SCART interface is used to signal the aspect ratio for
 * widescreen displays. The action of widescreen TVs on seeing a voltage of 6V
 * will be to ignore any WSS signalling received by the TV and cause the TV to
 * switch to 16:9 mode.  Some 4:3 TVs also respond to pin 8 signalling. A
 * value of 12V means that widescreen TVs should make use of WSS aspect ratio
 * signalling in order to apply the correct display action.
 *
 * This function does not control whether pin 8 is high or low (0V), but
 * merely controls what voltage pin 8 is at (6V or 12V) if it is high.
 *
 * @param scart_number Identifies the required SCART:
 *                     0 = primary SCART, 1 = secondary SCART.
 * @param aspect_ratio the new aspect ratio.
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_ScartSetAspectRatio(frost_uint scart_number,
                                   ICE_ScartAspectRatio aspect_ratio);


#ifdef __cplusplus
} /* end of extern "C" block */
#endif

#endif /* CABOT_ICE_ICE_SCART_H_INCLUDED*/
