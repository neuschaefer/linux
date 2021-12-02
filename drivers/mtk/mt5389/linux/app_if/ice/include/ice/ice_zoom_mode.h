/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for Video Zoom Modes
 *
 * This file defines the ICE interfaces to allow manipulation of video
 * zoom mode
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 *
 */

#ifndef CABOT_ICE_ICE_ZOOM_MODE_H_INCLUDED
#define CABOT_ICE_ICE_ZOOM_MODE_H_INCLUDED

#include "ice/ice_video_types.h"
#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************/
/**
 * @}
 * @name Zoom Mode Functions
 * @{
 */
/***************************************************************************/

/**
 * @brief Determine the zoom mode required to display video
 *
 * This function if it returns true will cause the middleware default zoom mode behaviour to be
 * overidden.
 *
 * @param handle [in] The handle to the required video device.
 * @param p_zoom_mode [out] The zoom mode to be used by the middleware
 * @param p_force_aspect_ratio [out] A flag to indicate whether to ignore auto user zoom mode
 *
 * @return #frost_true if zoom mode determined, #frost_false zoom mode cannot be determined
 */
frost_bool ICE_DetermineZoomMode( ICE_VideoHandle* handle,
                                  ICE_VideoTVZoomMode * p_zoom_mode,
                                  frost_bool * p_force_aspect_ratio);

/**
 * @brief Inform ICE layer about user_zoom_mode
 *
 * @param user_zoom_mode [in] The zoom mode selected by user
 *
 * @return #frost_true if it is set, #frost_false if not
 */
frost_bool ICE_InformUserZoomMode( ICE_VideoTVZoomMode user_zoom_mode);

/**
 * @}
 */





#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_ZOOM_MODE_H_INCLUDED*/
