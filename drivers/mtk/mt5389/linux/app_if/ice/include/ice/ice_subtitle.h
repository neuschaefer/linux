/**
 * Copyright 2002 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for Subtitles
 *
 * This file defines the ICE interfaces to allow manipulation of a
 * platform's subtitle decoder.  This interface should only be used
 * if Cabot's subtitle decoder is NOT integrated.  If Cabot's
 * subtitle decoder is integrated, subtitles will be decoded and
 * rendered within the Eclipse core.  (Cabot's subtitle decoder uses
 * the ice_tpdata.h API to retrieve subtitle data from the transport
 * stream.)
 *
 * All functions in this interface are implemented in STB-specific
 * code and called by Eclipse code.
 *
 */

#ifndef CABOT_ICE_ICE_SUBTITLE_H_INCLUDED
#define CABOT_ICE_ICE_SUBTITLE_H_INCLUDED

#include "ice/ice_device_number.h"
#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialise underlying STB subtitle driver.
 *
 * Called once (and only once), before any other subtitle functions
 * are called.  This hook is provided as a convenience to implementers
 * of this interface to allow one-time initialization to be performed.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SubtitleInit(void);


/**
 * @brief Configure the transport stream source for subsequent calls to 
 * ICE_SubtitleStart.
 *
 * @param source The source of the transport stream, e.g. tuner 0, tuner 1 or 
 * hard disk.
 *
 * This function will only be called when the subtitle decoder is stopped.
 * 
 * @note For single tuner platforms with no HDD, this function may be ignored.
 */
void ICE_SubtitleSelectSource(ICE_DeviceSource source);

/**
 * @brief Start decoding of subtitles.
 *
 * Requests that subtitles from the specified elementary stream should be
 * decoded.  The subtitles are not to be shown until ICE_SubtitleShow() is
 * called.
 *
 * @param subtitle_pid Subtitle PID.
 * @param composition_page_id Composition page.
 * @param ancillary_page_id Ancillary page.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SubtitleStart(frost_uint subtitle_pid,
                             frost_uint composition_page_id,
                             frost_uint ancillary_page_id);


/**
 * @brief Shows the currently selected subtitles (if any).
 *
 * Calling ICE_SubtitleShow() when subtitles are already shown will be
 * silently ignored and return #frost_true.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SubtitleShow(void);


/**
 * @brief Hides the currently selected subtitles (if any).
 *
 * Calling ICE_SubtitleHide() when subtitles are already hidden will
 * be silently ignored and return #frost_true.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SubtitleHide(void);


/**
 * @brief Stops decoding of the currently selected subtitle stream.
 */
void ICE_SubtitleStop(void);


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_SUBTITLE_H_INCLUDED*/
