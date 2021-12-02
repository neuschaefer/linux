/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for an additional plane that can display MPEG2 & H.264
 *         I-Frames.
 *
 * This file defines the ICE interface to allow manipulation of I-Frame still
 * images on the Image Plane.  Only systems that have an Image Plane (for
 * example, Freesat) need to implement this interface.
 *
 * All functions in this interface are implemented in STB-specific code and
 * called by Eclipse code.
 */

#ifndef CABOT_ICE_ICE_IMAGE_PLANE_H_INCLUDED
#define CABOT_ICE_ICE_IMAGE_PLANE_H_INCLUDED

#include "ice/ice_display_types.h"
#include "ice/ice_video_types.h"
#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialise the Image Plane.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_ImagePlaneInit(void);

/**
 * @brief Opaque type used to identify the open Image Plane.
 */
typedef struct ICE_ImagePlaneHandle ICE_ImagePlaneHandle;

/**
* @brief Open a connection to the Image Plane.
*
* Attempts to create an Image Plane and returns a handle for subsequent update.
*
* @param callback_func The callback function to be called.
* @param user_data Pointer to data that should be used as the parameter to
*        <code>callback_func</code> when the callback is raised.
*
* @return Returns a pointer to the handle to the Image Plane on success and a
*         NULL pointer on failure.
*/
ICE_ImagePlaneHandle* ICE_ImagePlaneOpen(ICE_VideoStatusCallback callback_func,
                                         void *user_data);

/**
* @brief Close a connection to the Image Plane.
*
* Closes a previously-opened connection to the Image Plane.
*
* @param handle The handle to the Image Plane.
*/
void ICE_ImagePlaneClose(ICE_ImagePlaneHandle *handle);

/**
* @brief Set the display properties of the decode.
*
* Specify the size and location on screen at which the decoded I-Frame should
* be shown.  If ICE_ImagePlanePosition() is called while an I-frame is
* showing, then the image should be resized and repositioned and should remain
* showing.  Any scaling and/or alterations to the aspect ratio in order for
* the input window to map onto the output window must be carried out by the
* underlying implementation.
*
* @param handle The handle to the Image Plane.
* @param input_window Pointer to a structure describing the input window, which
*        specifies the region of the source to be shown.
* @param output_window Pointer to a structure describing the output window,
*        which species the region of the display to be used.
*
* @return #frost_true on success, #frost_false on failure.
*
* @see ICE_VideoPosition() for more information on the \c input_window and \c
* output_window parameters.  ICE_ImagePlanePosition() is subject to the same
* conditions as ICE_VideoPosition() with the exception that the input window
* is specified relative to the decoded size returned by
* ICE_ImagePlaneDecodeHeight() and ICE_ImagePlaneDecodeWidth(), and only
* I-Frames (no video) will be positioned.
*/
frost_bool ICE_ImagePlanePosition(ICE_ImagePlaneHandle *handle,
                                  const ICE_DisplayWindow *input_window,
                                  const ICE_DisplayWindow *output_window);

/**
 * @brief Obtain the width of the latest decoded Image Plane I-Frame.
 *
 * A call to this function is based on the assumption that there is a currently
 * decoded Image Plane I-Frame.
 *
 * @param handle The handle to the Image Plane.
 *
 * @return The width, in pixels, of the latest decoded Image Plane I-Frame.
 */
frost_uint ICE_ImagePlaneDecodeWidth(ICE_ImagePlaneHandle* handle);

/**
 * @brief Obtain the height of the latest decoded Image Plane I-Frame.
 *
 * A call to this function is based on the assumption that there is a currently
 * decoded Image Plane I-Frame.
 *
 * @param handle The handle to the Image Plane.
 *
 * @return The height, in pixels, of the latest decoded Image Plane I-Frame.
 */
frost_uint ICE_ImagePlaneDecodeHeight(ICE_ImagePlaneHandle* handle);

/**
 * @brief Prepare platform to display an I-Frame on the Image Plane.
 *
 * Prepares the underlying platform to display an I-Frame on the Image Plane.
 * The ICE interface only supports preparing a single I-Frame at a time;
 * however, a different I-Frame can be displayed on the Video and Image Planes
 * at the same time.
 *
 * Subsequent calls to position, show, or hide the I-Frame will not pass the
 * raw data again, so the lower layer must either copy the data passed in by
 * the call to ICE_ImagePlaneIFrameLoad() or process it and ready it in AV
 * memory.  The data pointer must not be retained by the ICE implementation
 * after the call to ICE_ImagePlaneIFrameLoad() returns, as Eclipse is free to
 * re-use the memory.
 *
 * Loading an I-Frame does not immediately make it visible; it must first be
 * position/scaled - by calling ICE_ImagePlanePosition() - and finally
 * explicitly made visible by calling ICE_ImagePlaneIFrameShow().
 *
 * @param handle The handle to the Image Plane.
 * @param data Pointer to I-frame MPEG data.
 * @param number_of_bytes Length of I-frame data.
 * @param stream_type video stream type (either MPEG2 or H.264)
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_ImagePlaneIFrameLoad(ICE_ImagePlaneHandle *handle,
                                    const frost_uint8 *data,
                                    frost_ulong number_of_bytes,
                                    ICE_VideoStreamType stream_type);

/**
 * @brief Show the currently loaded I-Frame.
 *
 * Show the currently loaded I-Frame on the Image Plane at the currently
 * specified position and scale. The sequence of calls may be:
 * ICE_ImagePlaneInit(), ICE_ImagePlaneOpen(), ICE_ImagePlaneIFrameLoad()
 * ICE_ImagePlanePosition(), ICE_ImagePlaneIFrameShow(),
 * ICE_ImagePlanePosition(), ICE_ImagePlaneIFrameHide(),
 * ICE_ImagePlanePosition(), ICE_ImagePlaneIFrameShow(), etc.
 *
 * @param handle The handle to the Image Plane.
 *
 * @return #frost_true on success, #frost_false on failure (which
 *         typically indicates an invalid argument).
 */
frost_bool ICE_ImagePlaneIFrameShow(ICE_ImagePlaneHandle *handle);

/**
 * @brief Hide the I-Frame currently loaded on the Image Plane.
 *
 * Removes any currently visible Image Plane I-frame from the display, but
 * leaves it prepared in case of a future call to ICE_ImagePlaneIFrameShow().
 * A call to ICE_ImagePlaneIFrameHide() when no I-Frame is loaded is silently
 * ignored and returns #frost_true.
 *
 * @param handle The handle to the Image Plane.
 *
 * @return #frost_true on success, #frost_false on failure (which
 *         typically indicates an invalid argument).
 */
frost_bool ICE_ImagePlaneIFrameHide(ICE_ImagePlaneHandle *handle);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_IMAGE_PLANE_H_INCLUDED */
