/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for common Audio/Video data types.
 *
 * This file defines the common ICE data structures to allow
 * manipulation of video streams, i-frame still images, audio streams
 * and samples, PCR and subtitles.
 */

#ifndef CABOT_ICE_ICE_AV_H_INCLUDED
#define CABOT_ICE_ICE_AV_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Type definition for freezing audio and video.
 *
 * @note Resuming a frozen audio/video stream will result in an
 * audio/video discontinuity but resuming playback of a frozen sample
 * file will continue playback from the point at which the sample was
 * frozen (i.e. no discontinuity shall be apparent).
 */
typedef enum ICE_FreezeMode
{
    /**
     * Indicates that freezing of the stream is required.
     */
    ICE_freeze_on,

    /**
     * Indicates that the stream is to be unfrozen.
     */
    ICE_freeze_off

} ICE_FreezeMode; 

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_AV_H_INCLUDED*/
