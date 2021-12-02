/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for Video
 *
 * This file defines the ICE interfaces to allow manipulation of video
 * streams and I-frame still images.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 *
 * @see ice_stc.h
 */

#ifndef CABOT_ICE_ICE_VIDEO_H_INCLUDED
#define CABOT_ICE_ICE_VIDEO_H_INCLUDED

#include "ice/ice_video_types.h"
#include "frost/frost_basictypes.h"

#include "ice/ice_av.h"

#ifdef WIN32
#include "config/include_3d_support.h"
#endif

#include "config/include_project_median.h"
#include "config/include_project_phoenix.h"
#include "config/include_freeze.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************/
/**
 * @}
 * @name Video Functions
 * @{
 */
/***************************************************************************/


/**
 * @brief Get the number of video devices supported by the platform.
 *
 * Eclipse will call this function during initialisation.
 *
 * @return The number of video devices supported by the platform.
 */
frost_uint ICE_VideoGetNumberOfVideoDevices(void);


/**
 * @brief Initialise the Video device.
 *
 * Initialise the required video device and it's connection to the DEMUX.
 *
 * @param device_number The ID of the video device.
 * @param encoding_mode The initial video encoding mode.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VideoInit(ICE_DeviceNumber device_number,
                         ICE_VideoEncodingMode encoding_mode);


/**
 * @brief Open a connection to the Video device.
 *
 * Attempts to open a connection to the video device identified and
 * return a handle for subsequent communication with the device.
 *
 * @param device_number The ID of the video device.
 * @param callback_func The callback function to be called.
 * @param user_data Pointer to data that should be used as the parameter to
 *        <code>callback_func</code> when the callback is raised.
 *
 * @return Returns a pointer to the handle to the device on success
 *         and a NULL pointer on failure.
 */
ICE_VideoHandle* ICE_VideoOpen(ICE_DeviceNumber device_number,
                   ICE_VideoStatusCallback callback_func,
                   void* user_data);


/**
 * @brief Close a connection to the Video device.
 *
 * Closes a previously Opened connection to the video device identified.
 *
 * @param handle The handle to the required video device.
 */
void ICE_VideoClose(ICE_VideoHandle* handle);


/**
 * @brief Configure the transport stream source for subsequent calls to
 * ICE_VideoStart.
 *
 * @param handle The handle to the required video device.
 * @param source The source of the transport stream, e.g. tuner 0, tuner 1 or
 * hard disk.
 *
 * This function will only be called when the video decoder is stopped.
 *
 * @note For single tuner platforms with no HDD, this function may be ignored.
 */
void ICE_VideoSelectSource(ICE_VideoHandle* handle,
                           ICE_DeviceSource source);


/**
 * @brief Provides a query interface to determine the video encoding modes
 * supported by the implementation.
 *
 * @param supported_modes An output parameter to be filled with a bitwise OR
 * of the #ICE_VideoEncodingMode enumerated values that are supported by the
 * implementation.
 */
void ICE_VideoEncodingCapability(frost_uint32* supported_modes);


/**
 * @brief Sets the required video output encoding mode.
 *
 * Set the video output encoding mode required - e.g. PAL, NTSC.
 *
 * @param reqd_mode A #ICE_VideoEncodingMode enumerated value describing the
 * required mode.
 *
 * @note This function is not called by Eclipse except as a direct result of
 * the UI application calling the EASIControlInterface method
 * setVideoEncodingMode().
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SetVideoEncodingFormat(ICE_VideoEncodingMode reqd_mode);


/**
 * @brief Obtain the width of the latest decoded video.
 *
 * Access function returning the width of the latest decoded video. A
 * call to this function is based on the assumption that there is a
 * currently decoded video stream.
 *
 * @param handle The handle to the required video device.
 *
 * @return The width, in pixels, of the latest decoded video.
 *
 * @note This applies equally to I-frames.
 */
frost_uint ICE_DecodeWidth(ICE_VideoHandle* handle);


/**
 * @brief Obtain the height of the latest decoded video.
 *
 * Access function returning the height of the latest decoded video.  A
 * call to this function is based on the assumption that there is a
 * currently decoded video stream.
 *
 * @param handle The handle to the required video device.
 *
 * @return The height, in pixels, of the latest decoded video.
 *
 * @note This applies equally to I-frames.
 */
frost_uint ICE_DecodeHeight(ICE_VideoHandle* handle);


/**
 * @brief Gets field scan frequency of the video
 *
 * @param handle The handle to the required video device.
 *
 * @return The frequency in Hz
 *
 */
frost_uint ICE_VideoGetFrequency(ICE_VideoHandle* handle);

/**
 * @brief Gets whether the video is progressive
 *
 * @param handle The handle to the required video device.
 *
 * @return frost_true if video is progressive
 *
 */
frost_bool ICE_VideoIsProgressive(ICE_VideoHandle* handle);

/**
 * @brief Get the co-ordinate system for the video scaling output window.
 *
 * Full screen output will be indicated to ICE_VideoPosition() as an output
 * window (0, 0, width, height) where width and height are the values returned
 * by this function.
 * Eclipse will call ICE_VideoGetFullScreenOutputResolution() prior to every
 * call of ICE_VideoPosition() to get the current width and height of the
 * platform's video output window (it may have changed - for example, if the
 * HDMI output resolution has changed).
 *
 * @param width     The maximum width of the output window.
 * @param height    The maximum height of the output window.
 *
 * @return frost_true if width and height were both set correctly.
 */
frost_bool ICE_VideoGetFullScreenOutputResolution(frost_uint32 *width,
                                                  frost_uint32 *height);

/**
 * @brief Checks the stream type will change by ci cam
 */
frost_bool ICE_VideoIsStreamTypeChange(void);

/**
 * @brief Checks whether the video is scrambled
 */
frost_bool ICE_VideoIsScrambled(void);

/**
 * @brief Set the display properties of the decode.
 *
 * Specify the size and location on screen at which decode (video or
 * I-frame) should be shown.  If ICE_VideoPosition() is called while
 * video or I-frame is showing, then the video should be resized and
 * repositioned and should remain showing.  Any scaling and/or
 * alterations to the aspect ratio in order for the input window to
 * map onto the output window must be carried out by the underlying
 * implementation.
 *
 * @param handle The handle to the required video device.
 * @param input_window Pointer to a structure describing the input
 * window, which specifies the region of the source to be shown.
 * @param output_window Pointer to a structure describing the output
 * window, which species the region of the display to be used.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * Passing in windows containing widths or heights of zero indicate that
 * video should not be displayed, and so should be handled gracefully by
 * the ICE implementation.
 *
 * The <code>output_window</code> is specified relative to the screen size
 * returned by ICE_VideoGetFullScreenOutputResolution().
 * Eclipse will not set an output window outside the screen size.  I.e,
 * output_window.x_position and output_window.y_position will never be
 * negative, output_window.x_position + output_window.width will never
 * exceed 720 (for an SD PAL STB), and similarly output_window.y_position
 * + output_window.height will not exceed 576 (for an SD PAL STB).
 *
 * The <code>input_window</code> is specified relative to the decoded
 * video size returned by ICE_DecodeWidth() and ICE_DecodeHeight().
 * Eclipse will not deliberately set an input window outside the bounds
 * of the decoded video frame.  However, receivers should be aware that
 * there is a potential race condition between the arrival of a new video
 * frame with a different size and the call to this function to set the
 * new scaling.  Therefore implementers should make sure that they don't
 * crash when the input window is larger than the decoded video.
 * input_window.x_position and input_window.y_position may be negative
 * for re-positioning the input window (example: for VGA/PC input vertical /
 * horizontal shifting)
 *
 * The following examples illustrate the results of calling this function
 * when the source is full scale 720x576 and the display size returned from
 * ICE_VideoGetFullScreenOutputResolution is also 720x576. The chequerboard
 * pattern illustrates the areas of the screen where video output should not
 * be present. (The chequerboard pattern is for the purposes of illustration
 * only: these areas should appear black on a hardware implementation.)
 *
 * The parameters are,
 *   - Example 1
 *     - input window   (0,0,720,576)
 *     - output window  (0,0,720,576)
 *   - Example 2
 *     - input window   (180,144,360,288)
 *     - output window  (180,144,360,288)
 *   - Example 3 :
 *     - input window   (0,0,360,288)
 *     - output window  (180,144,360,288)
 *   - Example 4
 *     - input window   (90,72,630,504)
 *     - output window  (0,0,630,504)
 *   - Example 5
 *     - input window   (0,0,360,288)
 *     - output window  (0,0,720,576)
 *   - Example 6
 *     - input window   (360,288,720,576)
 *     - output window  (0,0,720,576)
 *   - Example 7 :
 *     - input window   (0,0,720,576)
 *     - output window  (180,144,360,288)
 *
 * @image html ICE_VideoPosition1.png "Example 1"
 * @image html ICE_VideoPosition2.png "Example 2"
 * @image html ICE_VideoPosition3.png "Example 3"
 * @image html ICE_VideoPosition4.png "Example 4"
 * @image html ICE_VideoPosition5.png "Example 5"
 * @image html ICE_VideoPosition6.png "Example 6"
 * @image html ICE_VideoPosition7.png "Example 7"
 *
 * @note If the width or height of the output window is set to zero, then
 *       no video should be presented.
 */
frost_bool ICE_VideoPosition(ICE_VideoHandle* handle,
                           const ICE_DisplayWindow *input_window,
                           const ICE_DisplayWindow *output_window);

/**
 * @brief Check whether the video stream type is supported
 *
 * This function is called to determine if ICE is able to provide
 * support for the specified stream type. That is, if requested to
 * start decoding video of this type (eg MPEG-2/H.264) that the
 * platform has a suitable decoder.
 * It must be called before a call to ICE_VideoStart, as if this
 * function indicates that the given stream type is not supported
 * then it is expected that Eclipse will not call ICE_VideoStart.
 *
 * @param video_stream_type Stream type to be checked.
 *
 * @return #frost_true on type supported, #frost_false not supported
 */
frost_bool ICE_IsVideoStreamTypeSupported(ICE_VideoStreamType video_stream_type);

/**
 * @brief Start decoding the video stream specified by the PID.
 *
 * PID is ignored if the video comes from non-DVB sources, like external
 * sources (SCART etc) or analogue boradcast.
 *
 * Likewise, video_stream_type is ignored in non-DVB cases.
 *
 * Start decoding the input video elementary stream specified by
 * <code>video_pid</code>.  If another call is made to this function prior to
 * a call to ICE_VideoStop() the behaviour is undefined.
 *
 * This function should not show the video that is produced by the decoders
 * synchronising to the video stream before the video is ready to be displayed.
 * The callback function (as passed to ICE_VideoOpen()) should be called with
 * status ICE_VideoStatusFirstFrameDecoded when the first video frame is
 * decoded in the case of digital broadcast, with status
 * ICE_AnalogVideoStatusVideoReady when the video is ready in the case of
 * analogue broadcast, and with status ICE_ExternalVideoStatusVideoReady when
 * the video is ready in the case of video that comes from external sources.
 *
 * If the video has been hidden by a call to ICE_VideoHide(), this function
 * does not unhide it.  The callback should still be made when the first
 * frame has been decoded, even if that frame is not displayed because
 * the video is hidden.
 *
 * Note that this function will be proceeded by a call to
 * ICE_IsVideoStreamTypeSupported, to ensure that the platform is able to
 * decode the supplied video stream. If this function is called with a
 * stream type that is not supported it will return frost_false, but
 * provide no further information on why the failure occurred.
 *
 * @param handle The handle to the required video device.
 * @param video_pid Video PID to decode.
 * @param video_stream_type The type of the video stream (MPEG-2/H.264)
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VideoStart(ICE_VideoHandle* handle, frost_uint video_pid,
                          ICE_VideoStreamType video_stream_type);


/**
 * @brief Show the currently decoded video.
 *
 * Show the currently decoding video at the position specified by
 * ICE_VideoPosition().  (The minimal sequence of calls required to
 * display video is ICE_VideoInit(), ICE_VideoOpen(), ICE_VideoPosition(),
 * ICE_VideoStart(), ICE_VideoShow().)  Calling ICE_VideoShow() for a
 * video that is already being shown will be silently ignored and
 * return #frost_true.
 *
 * After Eclipse calls ICE_IFrameLoad(), it will not call this function until
 * after it has called ICE_VideoStart() again.
 *
 * @param handle The handle to the required video device.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VideoShow(ICE_VideoHandle* handle);


/**
 * @brief Bring the video to the front of other video windows
 *
 * @param handle The handle to the required video device.
 *
 * This function may be called when Eclipse is using multiple video decoders,
 * for example to exhibit "picture-in-picture" behaviour.  When this function
 * is called, the output of the video device specified should be brought in
 * front of any other active video decoders.
 *
 * @note On platforms which do not support picture-in-picture, this function
 * may return frost_false.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VideoToFront(ICE_VideoHandle* handle);


/**
 * @brief Send the video to the back of other video windows.
 *
 * @param handle The handle to the required video device.
 *
 * This function may be called when Eclipse is using multiple video decoders,
 * for example to exhibit "picture-in-picture" behaviour.  When this function
 * is called, the output of the video device specified should be displayed
 * behind any other active video decoders.
 *
 * @note On platforms which do not support picture-in-picture, this function
 * may return frost_false.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VideoToBack(ICE_VideoHandle* handle);


/**
 * @brief Hides the display of the current video.
 *
 * This function hides the currently showing video (if any). Resume
 * video with ICE_VideoShow().  If there is no currently showing video
 * stream, then this function will have no effect and return #frost_true.
 *
 * @param handle The handle to the required video device.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VideoHide(ICE_VideoHandle* handle);


/**
 * @brief Stop decoding the current video stream and leave the last frame
 *        on screen.
 *
 * This function stops the current video stream (if any) from being
 * decoded and leaves the last decoded frame on screen.  If Eclipse wishes
 * to hide the last decoded frame so that there is no video element showing,
 * it will call ICE_VideoHide() either before or after this call.  The
 * implementation must release the video PID.
 *
 * If there is no video stream being decoded, then this function will
 * have no effect and will return #frost_true.
 *
 * @param handle The handle to the required video device.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VideoStop(ICE_VideoHandle* handle);

/***************************************************************************/
/**
 * @}
 * @name Aspect ratio, AFD and WSS functions
 * @{
 */
/***************************************************************************/

/**
* @brief Reset the current AFD
*
* This function is called when tuning to a new channel to reset to the default
* AFD, as not all services have a transmitted AFD.  As a result of calling
* this function subsequent calls to #ICE_DecodeAFD will return
* #ICE_afd_not_present (until a different AFD is detected for the
* service).
*
* @param handle The handle to the required video device.
*/
void ICE_ResetAFD(ICE_VideoHandle* handle);


/**
 * @brief Obtains the aspect ratio of the latest decoded video.
 *
 * Access function returning the aspect ratio of the latest decoded
 * video.  A call to this function is based on the assumption that
 * there is a currently decoded video stream.
 *
 * @param handle The handle to the required video device.
 *
 * @return The aspect ratio of the latest decoded video.
 *
 * @note This applies equally to Iframes.
 */
ICE_VideoAspectRatio ICE_DecodeAspectRatio(ICE_VideoHandle* handle);

/**
 * @brief Obtains the exact value of aspect ratio of the latest decoded video.
 *
 * Access function returning the exact value for the aspect ratio of the latest decoded
 * video.  A call to this function is based on the assumption that
 * there is a currently decoded video stream.
 *
 * @param handle The handle to the required video device.
 *
 * @return The exact value of aspect ratio of the latest decoded video.
 *
 * @note This applies equally to Iframes.
 */
float ICE_DecodeExactAspectRatio(ICE_VideoHandle* handle);

/**
 * @brief Obtains the AFD of the latest decoded video.
 *
 * Access function returning the AFD of the latest decoded video.  A
 * call to this function is based on the assumption that there is a
 * currently decoded video stream.
 *
 * @param handle The handle to the required video device.
 *
 * @return The AFD of the latest decoded video.
 *
 * @note This applies equally to Iframes.
 */
ICE_VideoAFD ICE_DecodeAFD(ICE_VideoHandle* handle);


/**
 * @brief Sets the Wide Screen Signalling Aspect Ratio code to output.
 *
 * Wide Screen Signalling (WSS) is transmitted as a data burst
 * in the first part of line 23, and is used to signal the aspect
 * ratio and signal of the transmitted analogue signal.
 *
 * This function is used to control WSS output that depends upon the MPEG
 * aspect ratio, AFD and the user decoder option.  Widescreen TVs make use
 * of the WSS code to ensure automatic selection of the most appropriate
 * display mode for 4:3 ratio video.
 *
 * @note See ETS 300 294 and D-book version 3.2 section 3.4. for more details.
 *
 * @param handle The handle to the required video device.
 * @param wss_aspect_ratio The WSS aspect ratio code to output.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SetWSSAspectRatio(ICE_VideoHandle* handle,
                                 ICE_WSS_AspectRatioCode wss_aspect_ratio);


/**
 * @brief Sets the Wide Screen Signalling for subtitles.
 *
 * Wide Screen Signalling (WSS) is transmitted as a data burst
 * in the first part of line 23, the rest is other information
 * such as subtitle modes, surround sound mode, and copyright etc.
 *
 * This function is used to set the state of bits 9 and 10, so that
 * the TV may decide not to scale the video if it would cause the
 * subtitles to be displayed off screen.
 *
 * @note See ETS 300 294 section 4.2.3 titled "Data group 3, subtitles"
 *
 * @param handle The handle to the required video device.
 * @param value  The value.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SetWSSSubtitles(ICE_VideoHandle* handle,
                               ICE_WSS_Subtitles value);

/***************************************************************************/
/**
 * @}
 * @name Iframe Functions
 * @{
 */
/***************************************************************************/

/**
 * @brief Prepare platform to display an I-frame.
 *
 * Prepares the underlying platform to display an I-frame.  The ICE interface
 * only supports preparing a single I-Frame at a time.
 *
 * Subsequent calls to position, show, or hide the I-frame will not pass the
 * raw data again, so the lower layer must either copy the data passed in by
 * the call to ICE_IFrameLoad() or process it and ready it in AV memory.  The
 * data pointer must not be retained by the ICE implementation after the call
 * to ICE_IFrameLoad() returns, as Eclipse is free to re-use the memory.
 *
 * Loading an I-frame does not immediately make it visible; it must first be
 * position/scaled - by calling ICE_VideoPosition() - and finally explicitly
 * made visible by calling ICE_IFrameShow().  When appropriate, the callback
 * function (as passed to ICE_VideoOpen()) must be called with
 * status ICE_VideoStatusAttributesChanged to ensure the positioning and
 * scaling are correct.  The implementation of ICE_IFrameLoad() should
 * initially stop the decoding of any active video.
 *
 * @note If the implementation uses resources to prepare the I-frame, these
 * resources may be released by the implementation when calls to
 * ICE_VideoStart() are made.  Thus, Eclipse does not rely
 * on an I-Frame still being loaded after intervening video calls are made,
 * and instead has to reload the I-Frame.
 *
 * @param handle The handle to the required video device.
 * @param data Pointer to I-frame MPEG data.
 * @param number_of_bytes Length of I-frame data.
 * @param stream_type video stream type (either MPEG2 or H.264)
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_IFrameLoad(ICE_VideoHandle* handle, const frost_uint8 *data,
                          frost_ulong number_of_bytes,
                          ICE_VideoStreamType stream_type);


/**
 * @brief Show the currently loaded I-frame.
 *
 * Show the currently loaded I-frame at the currently specified
 * position and scale. The sequence of calls may be: ICE_IFrameLoad(),
 * ICE_VideoPosition(), ICE_IFrameShow(), ICE_VideoPosition(),
 * ICE_IFrameHide(), ICE_VideoPosition(), ICE_IFrameShow(), etc.
 *
 * @param handle The handle to the required video device.
 *
 * @return #frost_true on success, #frost_false on failure (which
 *         typically indicates an invalid argument).
 */
frost_bool ICE_IFrameShow(ICE_VideoHandle* handle);


/**
 * @brief Hide the currently loaded I-frame.
 *
 * Removes any currently visible I-frame from the display, but leaves
 * it prepared in case of a future call to ICE_IFrameShow().  A call to
 * ICE_IFrameHide() when no I-frame is loaded is silently ignored and
 * returns #frost_true.
 *
 * @param handle The handle to the required video device.
 *
 * @return #frost_true on success, #frost_false on failure (which
 *         typically indicates an invalid argument).
 */
frost_bool ICE_IFrameHide(ICE_VideoHandle* handle);


/***************************************************************************/
/**
 * @}
 * @name Video functions for IDTV extensions.
 * @{
 */
/***************************************************************************/

/**
 * @brief Obtains the WSS of the latest decoded video.
 *
 * Access function returning the WSS of the latest decoded video.  A
 * call to this function is based on the assumption that there is a
 * currently decoded video stream.
 *
 * @param handle The handle to the required video device.
 *
 * @return The WSS of the latest decoded video.
 */
ICE_WSS_AspectRatioCode ICE_DecodeWSS(ICE_VideoHandle* handle);


/**
 * @brief Queries the capability of the platform to display video on PIP
 *        window when there is a video in the main window.
 *
 *  In some cases, opening a high-resolution video on PIP window has
 *  unsatisfactory results when there is already a high-resolution video
 *  on the main window due to bandwidth limitations.  Thus, the implementation
 *  might want to disable opening PIP window in such cases.  The implementation
 *  of upper layers thus will call this function before opening a PIP window.
 *  Likewise, when a video format change occurs in the main window (from standard
 *  resolution to high-resolution) the upper layer might want to close the
 *  PIP window if it already exists.
 *
 *  This function must be called:
 *   - Before a PIP window is opened,
 *   - Before the source in the PIP window is changed,
 *   - Before the source in the main window is changed,
 *   - When the status of the source that's being displayed in
 *     PIP window changes,
 *   - When the status of the source that's being displayed in
 *     main window changes.
 *
 *
 * @param main_display_source The source that provides  (or will provide) the
 *                            video in the main window.
 * @param pip_display_source The source that provides (or will provide) the
 *                           video in the PIP window.
 *
 * @return #frost_true if connection is possible, #frost_false if connection
 *         is not possible.
 *
 */
frost_bool ICE_VideoIsPIPConnectionPossible(ICE_DeviceSource main_display_source,
                                            ICE_DeviceSource pip_display_source);

/**
 * @brief This function is called at the last stage of zooming calculations.
 *    The calculated input and output windows are provided to this function to
 *    allow manuplation of these parameters whenever required. Ideally, this
 *    function should be left as empty.
 *
 * @param handle The handle to the required video device.
 * @param zoom_params Input and output windows calculated by Eclipse.
 *
 * @return #frost_true if this function is implemented #frost_true if it is left
 *       empty.
 */
frost_bool ICE_VideoZoomModeOverride(ICE_VideoHandle* handle,
                                     ICE_VideoTVZoomParams* zoom_params);

/**
 * @brief This function is called to retrive colour standard of an analog video.
 *
 * @param handle [in] The handle to the required video device.
 * @param colour_standard [out] The colour standard (also known as "video
 *                    encoding mode" in Eclipse code) that's used by the given
 *                    video device.
 *
 * @note Platforms that do not support analogue video inputs
 *       do not need to implement this method.
 *
 * @return #frost_true if colour standard is detected, #frost_false if colour
 *         standard could not be detected or if an error occured.
 */
frost_bool ICE_VideoGetDetectedColourStandard(ICE_VideoHandle* handle,
                                              ICE_VideoEncodingMode* colour_standard);


/**
 * @brief This function is called to enable or disable nonlinear scaling
 * @param handle [in] The handle to the required video device.
 * @param horizontal_nonlinear_scaling [in] if frost_true, enable horizontal nonlinear scaling.
 *
 *
 * @note This is essentially for IDTVs, not STB or DVR based platforms, so they
 *       do not need to implement this method.
 *
 * @return #frost_true if successful #frost_false otherwise
 *
 */
frost_bool ICE_VideoSetNonLinearScaling(ICE_VideoHandle* handle, frost_bool horizontal_nonlinear_scaling);

/**
 * @brief Check whether the video decoding is presented
 *
 * This function is called to determine if ICE has started to decoding video from an external source.
 *
 * @param handle [in] The handle to the required video device.
 *
 * @return #frost_true on decode presented, #frost_false not decoding presented
 */
frost_bool ICE_IsVideoDecodingPresent(ICE_VideoHandle* handle);

#if (defined(INCLUDE_PROJECT_MEDIAN) || defined(INCLUDE_PROJECT_PHOENIX))
/**
 * @brief Check whether the video decoding is presented
 *
 * This function is called to signal  nondvbstart_semaphore.
 *
 * @return #frost_true on if semaphore signaled, #frost_false if semaphore not created
 */
frost_bool ICE_VideoStartMessage(void);

#ifdef FAST_TURN_ON 
frost_bool ICE_VideoShowMessage(void);
#endif

void ICE_VideoForceDrvSrc(ICE_DeviceSource src);

#endif

frost_bool ICE_SetVideoWall(ICE_VideoWall input_video_wall);

#if (defined INCLUDE_3D_SUPPORT)  || (defined ENABLE_3D)
/**
*@brief structure of parametrs of Latest Display that incluses x,y positions, width, height, vfreq,and is_interlaced. 
*/
typedef struct ICE_LatestDisplayInfo
{
    frost_int x_position;
    frost_int y_position;
    frost_uint width;
    frost_uint height;
    frost_uint16 vfreq;
    frost_bool is_interlaced;
}ICE_LatestDisplayInfo;

/**
 * @brief This function is called to retrive latest Display resolution and position information.
 *
 * @param info [out] to get struct of video information.
 *
 * @return nothing
 */
void  ICE_VideoGetLatestDisplayInfo(ICE_LatestDisplayInfo* info );
frost_uint ICE_DecodeWidth3D(void);
frost_uint ICE_DecodeHeight3D(void);
#endif

void ICE_VideoSetMacrovision(frost_bool macrovision_enable);

#ifdef INCLUDE_FREEZE
void ICE_VideoSetFreezed(void);
#endif

/**
 * @brief This function is called to change colour pattern.
 *
 * @param colormode [in] defines the colour of the applied pattern.
 *      0:  WHITE
 *      1:  RED
 *      2:  GREEN
 *      3:  BLUE
 *      4:  BLACK
 *      5:  GREY
 *
 * @return #frost_true if successful #frost_false otherwise
 */
frost_int ICE_VideoSetColorMode(frost_ulong colormode);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_VIDEO_H_INCLUDED*/
