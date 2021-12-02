/**
 * Copyright 2010 Cabot Communications.  All rights reserved.
 *
 * @file
 * @version 
 * @brief  ICE Interface for background colour
 *
 * This file defines the ICE interface to change the colour that appears
 * on the background plane.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 */

#ifndef CABOT_ICE_ICE_BACKGROUND_H_INCLUDED
#define CABOT_ICE_ICE_BACKGROUND_H_INCLUDED

#include "ice_osd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set the background colour.
 *
 * Set the solid background colour that appears behind transparent areas in
 * the OSD that are not covered by video or I-frames.
 *
 * @param colour The color to use to fill the entire background.  The
 *               transparency field will be ignored as this fill is always
 *               opaque.  Must not be NULL.
 */
void ICE_BackgroundSetColour(frost_uint32 colour);


/**
 * @brief Display the specified image on background plane.
 *
 * @param data_ptr Background image raw data (as an I-frame).
 * @param size Size of the data_ptr.
 * @param x X coordinate of image on background plane.
 * @param y Y coordinate of image on background plane.
 * @param width Width of image.
 * @param height Height of image. 
 *
 * @return frost_true on success, frost_false otherwise.
 */
frost_bool ICE_BackgroundDisplayImage (void *data_ptr, 
                                       frost_uint32 size, 
                                       frost_int32 x, 
                                       frost_int32 y, 
                                       frost_int32 width, 
                                       frost_int32 height);

/**
 * @brief Hide the background image.
 */
void ICE_BackgroundHideImage(void);


/**
 * @brief Create a video preview window on background plane for the 
 *        given coordinates. This function needs to be implemented for
 *        platforms that cannot support a background image plane behind the video.
 *        In such a situation, implementation can use a graphics plane that is
 *        actually in front of the video plane. In order to see video through 
 *        this pseudo background plane, it is required to create a transparent
 *        video preview window.
 * 
 *
 * @param x X coordinate of the video position.
 * @param y Y coordinate of the video position
 * @param width Width of the video position. 0 means there is no video on the 
 *        screen right now.
 * @param height: Height of the video position. 0 means there is no video on the
 *        screen right now.
 */
void ICE_BackgroundSetVideoPosition(frost_int32 x, 
                                    frost_int32 y, 
                                    frost_int32 width, 
                                    frost_int32 height);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_BACKGROUND_H_INCLUDED */
