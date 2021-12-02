 
#ifndef CABOT_ICE_ICE_VIDEO_FEEDER_H_INCLUDED
#define CABOT_ICE_ICE_VIDEO_FEEDER_H_INCLUDED


#include "ice/ice_video_types.h"
#include "frost/frost_basictypes.h"


 /* Enumeration of possible feeder events passed to the
 * ICE_VideoFeederStatusCallback.
 */
typedef enum ICE_VideoFeederEvent
{
    /**
     * The decoder has started processing raw h264 stream, or at least
     * it is ready to accept requests for decoding frames of h264 data.
     * Note that requests for data may have been made prior to this event
     * being generated.
     */
    ice_video_feeder_event_started,

    /**
     * The decoder is stopped.
     */
    ice_video_feeder_event_stopped,

    /**
     * The player has been stalled due to a lack of incoming data.
     * This may be caused by requests not being satisfied quickly enough to
     * ensure smooth playback. Streaming will continue if more data is received,
     * at which time an #ice_video_feeder_event_resumed event will occur.
     * This event must not be sent at end of stream, instead send
     * #ice_video_feeder_event_end_of_stream.
     */
    ice_video_feeder_event_underflow,

    /**
     * The player has resumed after an #ice_video_feeder_event_underflow event.
     */
    ice_video_feeder_event_resumed,

    /**
     * The player has encountered non-h264 stream format data and cannot
     * process it.
     *
     * #ICE_VideoStop() must be called to release resources.
     */
    ice_video_event_format_error,

    /**
     * The player has encountered an irrecoverable error.
     *
     * #ICE_VideoStop() must be called to release resources.
     */
    ice_video_feeder_event_error

} ICE_VideoFeederStatusEvent;
 /*
 * @param request_token Must be passed to #ICE_VideoFeederRequestCallback() when
 *                      the request can be satisfied.
 * @param buffer To be filled with RAW h264 data.
 * @param buffer_size The amount of h264 data required.
 * @param user_data The user_data parameter passed to ICE_VideoSetFeedder().
 *
 * @return #0 if Skype is not able to send data at this time, otherwise number
 * of bytes copied to buffer
 */
typedef frost_uint32 (*ICE_VideoFeederRequestCallback)(void * request_token,
                                                  frost_uint8 * buffer,
                                                  frost_uint32 buffer_size,
                                                  frost_uint32 *timestamp,
                                                  void * user_data);
/**
 * Callback function to inform Skype application of a videofeeder event.
 *
 * @param event         The event generated.
 * @param user_data     The user_data parameter passed to ICE_VideoSetFeedder().
 */
typedef void (*ICE_VideoFeederStatusCallback)(ICE_VideoFeederStatusEvent event,
                                           void * user_data);


/**
 * @brief Start injection of h264 raw data to video decoder.
 *
 * This function must return immediately and status updates must be signalled.
 *
 * Callbacks should only be called if playback starts successfully (i.e. this
 * function returns non-zero).  If errors are detected before the
 * function returns, they should be
 * signalled and the return value set to frost_true.
 *
 * After #ice_video_feeder_event_started has been received, calls will be made
 * to set the #ICE_DeviceSource to #ICE_source_feeder for video
 * (i.e. with ICE_VideoSelectSource()).  
 *
 * @param handle            video handle.
 * @param request_callback  The callback function to be called when more data
 *                          is required by the decoder. This function can be called with NULL CB, which means that
                            ICE_VideoFeedData() function will be called without wating for 
                            callback ICE_VideoFeederRequestCallback(frame rate of injected frames can be very low)
 * @param status_callback   The callback function to be called in the case of a
 *                          status update during data injection.
 * @param x                 Output window x position.
 * @param y                 Output window y position.
 * @param windowWidth       Required decoded video width.
 * @param width             Incoming video width.
 * @param height            Incoming video height.
 * @param user_data         Pointer to data that should be used as a parameter
 *                          to the callbacks.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VideoSetFeeder(ICE_VideoHandle * handle,
                             ICE_VideoFeederRequestCallback request_callback,
                             ICE_VideoFeederStatusCallback status_callback,
                             unsigned int x,
                             unsigned int y,
                             unsigned int windowWidth,
                             unsigned int width,
                             unsigned int height,
                             void * user_data);

frost_bool ICE_VideoStop_feeder(ICE_VideoHandle * handle);


/**
 * @brief Change output video decoder window size and position.
 *
 * @param handle            video handle.
 * @param x                 Output window x position.
 * @param y                 Output window y position.
 * @param windowWidth       Required decoded video width.
 * @param width             Incoming video width.
 * @param height            Incoming video height.
 * @param user_data         Pointer to data that should be used as a parameter
 *                          to the callbacks.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_VideoFeederChangePosition(ICE_VideoHandle * handle,
                             unsigned int x,
                             unsigned int y,
                             unsigned int windowWidth,
                             unsigned int width,
                             unsigned int height,
                             void * user_data);


#endif