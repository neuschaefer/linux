#ifndef CABOT_ICE_ICE_AUDIO_FEEDER_H_INCLUDED
#define CABOT_ICE_ICE_AUDIO_FEEDER_H_INCLUDED


#include "frost/frost_basictypes.h"
#include "ice/ice_audio_types.h"

 /* Enumeration of possible feeder events passed to the
 * ICE_AudioFeederStatusCallback.
 */
typedef enum ICE_AudioFeederEvent
{
    /**
     * The decoder has started processing PCM data stream, or at least
     * it is ready to accept requests for palyback of PCM data.
     * Note that requests for data may have been made prior to this event
     * being generated.
     */
    ice_audio_feeder_event_started,

    /**
     * The decoder is stopped.
     */
    ice_audio_feeder_event_stopped,

    /**
     * The last of the media from the stream has been presented as a result
     * of end_of_stream being passed to #ICE_AudioFeederRequestCallback.
     */
    ice_audio_feeder_event_end_of_stream,

    /**
     * The decoder has been stalled due to a lack of incoming data.
     * This may be caused by requests not being satisfied quickly enough to
     * ensure smooth playback. Streaming will continue if more data is received,
     * at which time an #ice_audio_feeder_event_resumed event will occur.
     * This event must not be sent at end of stream, instead send
     * #ice_audio_feeder_event_end_of_stream.
     */
    ice_audio_feeder_event_underflow,

    /**
     * The decoder has resumed after an #ice_audio_feeder_event_underflow event.
     */
    ice_audio_feeder_event_resumed,

    /**
     * The decoder has encountered an irrecoverable error.
     *
     * #ICE_AudioStop() must be called to release resources.
     */
    ice_audio_feeder_event_error

} ICE_AudioFeederStatusEvent;

/**
 * @brief Representation of PCM parameters for a streamed audio file.
 */
typedef struct ICE_AudioFeederInitParams
{
    /**
     * Identifies the number of input channels of a streamed audio file.
     */
    ICE_AudioChannelMode input_channels;

    /**
     * Identifies the number of output channels.
     */
    ICE_AudioChannelMode output_channels;

    /**
     * Identifies the PCM sampling frequency of a streamed audio file.
     */
    frost_uint16 sampling_frequency;

    /**
     * Identifies the PCM sample number of bits of a streamed audio file.
     */
    frost_uint8 sample_width;
   
    /**
     * Identifies if audio file should be reaped=ted after end_of_file is reached.
     */
    frost_bool loop_requested;

    /**
     * Identifies if streamed audio file should be downmixed eith main audio.
     */
    frost_bool downmix_requested;    
} ICE_AudioFeederInitParams;

 /*
 * @param request_token Must be passed to #ICE_AudioFeederRequestCallback() when
 *                      the request can be satisfied.
 * @param buffer To be filled with PCM data.
 * @param buffer_size The amount of PCM data required.
 * @param user_data The user_data parameter passed to ICE_AudioSetFeedder().
 *
 * @return #0 if Skype is not able to send data at this time, otherwise number
 * of bytes copied to buffer
 * 
 */
typedef frost_uint32 (*ICE_AudioFeederRequestCallback)(void * request_token,
                                                  frost_uint8 * buffer,
                                                  frost_uint32 buffer_size,
                                                  void * user_data);
/**
 * Callback function to inform Skype application of a audiofeeder event.
 *
 * @param event         The event generated.
 * @param user_data     The user_data parameter passed to ICE_AudioSetFeedder().
 */
typedef void (*ICE_AudioFeederStatusCallback)(ICE_AudioFeederStatusEvent event,
                                           void * user_data);


/**
 * @brief Start injection of PCM data to audio decoder.
 *
 * This function must return immediately and status updates must be signalled.
 *
 * Callbacks should only be called if playback starts successfully (i.e. this
 * function returns non-zero).  If errors are detected before the
 * function returns, they should be
 * signalled and the return value set to frost_true.
 *
 * After #ice_audio_feeder_event_started has been received, calls will be made
 * to set the #ICE_DeviceSource to #ICE_source_feeder for audio (i.e. with ICE_AudioSelectSource()). 
 *
 * @param handle            audio handle.
 * @param request_callback  The callback function to be called when more data
 *                          is required by the decoder. This function can be called with NULL CB, which means that
                            ICE_AudioFeedData() function will be called without wating for 
                            callback ICE_AudioFeederRequestCallback.
 * @param status_callback   The callback function to be called in the case of a
 *                          status update during data injection.
 * @param audio_params      Description of PCM streaming audio type
 * @param user_data         Pointer to data that should be used as a parameter
 *                          to the callbacks.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_AudioSetFeeder(ICE_AudioHandle * handle,
                             ICE_AudioFeederRequestCallback request_callback,
                             ICE_AudioFeederStatusCallback status_callback,
                             ICE_AudioFeederInitParams audio_params,
                             void * user_data);



// temporary solution for stoping audio feeder thread
// until ICE_AudioStop() is updated to support AV feeder
frost_bool ICE_AudioStop_feeder(ICE_AudioHandle * handle);


/**
 * @brief Change LiveTV/Skype volume ratio.
 *
 * Function changes LiveTV/Skype volume ratio
 * 
 * @param handle            audio handle.
 * @param volume_percentage Skype volume portion in main volume
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_AudioSetVolumePercentage(ICE_AudioHandle * handle,
                             frost_uint32 volume_percentage);


#endif