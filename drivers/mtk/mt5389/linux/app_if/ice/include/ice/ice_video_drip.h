/**
 * Copyright 2005 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for video drips
 *
 * This file defines the ICE interfaces to allow decoding of video
 * drips.
 *
 * This API only needs to be implemented if you want to support MHP.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 *
 * For more information about video drips, see the MHP specification.
 *
 * @see ice_video.h
 * @see ice_video_types.h
 */

#ifndef CABOT_ICE_ICE_VIDEO_DRIP_H_INCLUDED
#define CABOT_ICE_ICE_VIDEO_DRIP_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_video_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Place the decoder in video drip mode.
 *
 * Prepares the underlying platform to display video drips.
 *
 * The decoder will stay in video drip mode until a call to
 * ICE_IFrameLoad(), ICE_VideoStart() or ICE_VideoClose().
 *
 * Note that ICE_VideoPosition(), ICE_DecodeWidth() and
 * ICE_DecodeHeight() all apply to video drips.  The
 * ICE_VideoAttributesChangedCallbackFunction passed to
 * ICE_VideoOpen() must be called if the size of the decoded
 * frame changes - either due to starting playing a video drip,
 * or due to recieving an I-frame with a different size in the
 * middle of a video drip sequence.
 *
 * The AFD and aspect ratio of video drips is ignored.  Hence
 * in video drip mode ICE_DecodeAspectRatio() and ICE_DecodeAFD()
 * do not have to return the correct values (but they must return
 * some legal value and must not crash).  There is no need to call
 * the ICE_VideoAttributesChangedCallbackFunction passed to
 * ICE_VideoOpen() when the video drip aspect ratio or AFD
 * changes, but it is OK to call it if that is easier to implement.
 *
 * @param handle The handle to the required video device.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VideoDripStart(ICE_VideoHandle* handle);

/**
 * @brief Display a video drip.
 *
 * Decodes part of a video drip.
 *
 * @param handle The handle to the required video device.
 * @param data Pointer to video drip MPEG data.
 * @param number_of_bytes Length of video drip data.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VideoDripData(ICE_VideoHandle* handle,
                              const frost_uint8 *data,
                              frost_ulong number_of_bytes);

/**
 * @brief Stop the currently video drip.
 *
 * Releases all the resources allocated for video drip mode.
 *
 * @param handle The handle to the required video device.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VideoDripStop(ICE_VideoHandle* handle);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_VIDEO_DRIP_H_INCLUDED*/
