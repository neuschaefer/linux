/**
 * Copyright 2002-4 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE definition of device number.
 *
 */

#ifndef CABOT_ICE_ICE_DEVICE_NUMBER_H_INCLUDED
#define CABOT_ICE_ICE_DEVICE_NUMBER_H_INCLUDED

#include "frost/frost_basictypes.h"
/**
 * A portable typedef for an instance of each supported ICE device.
 * Used extensively in the ICE interface.
 */
typedef frost_uint ICE_DeviceNumber;

/**
 * @brief Type definition for the source of a device's media stream.
 *
 * @note For platforms with a single tuner, the only valid value is
 * ICE_source_tuner0.  Other values should not be used and may be ignored.
 */
typedef enum ICE_DeviceSource
{
    /**
     * The device's first digital tuner.
     */
    ICE_source_tuner0 = 0,

    /**
     * The device's second digital tuner.
     */
    ICE_source_tuner1 = 1,

    /**
     * The device's first analogue tuner.
     */
    ICE_source_analogue_tuner0,

    /**
     * The device's second analogue tuner.
     */
    ICE_source_analogue_tuner1,

    /**
     * The device's hard disk drive.
     */
    ICE_source_hdd,

    /**
     * The ice_media_player implementation for the device.
     */
    ICE_source_media_player,

    /**
     * The ice_ts_player implementation for the device.
     */
    ICE_source_ts_player,

    /**
     * The device's video source index 0.
     */
    ICE_source_video0,

    /**
     * The device's video source index 1.
     */
    ICE_source_video1,

    /**
     * The device's video source index 2.
     */
    ICE_source_video2,

    /**
     * The device's video source index 3.
     */
    ICE_source_video3,

    /**
     * The device's video source index 4.
     */
    ICE_source_video4,

    /**
     * The device's vidoe source index 5.
     */
    ICE_source_video5,

    /**
     * The device's video source index 6.
     */
    ICE_source_video6,

    /**
     * The device's video source index 7.
     */
    ICE_source_video7,

    /**
     * The device's video source index 8.
     */
    ICE_source_video8,

    /**
     * The device's video source index 9.
     */
    ICE_source_video9,

    /**
     * The device's video source index 10.
     */
    ICE_source_video10,

    /**
     * The device's video source index 11.
     */
    ICE_source_video11,

    /**
     * The device's video source index 12.
     */
    ICE_source_video12,

    /**
     * The device's video source index 13.
     */
    ICE_source_video13,

    /**
     * The device's video source index 14.
     */
    ICE_source_video14,

    /**
     * The device's vidoe source index 15.
     */
    ICE_source_video15,

    /**
     * The device's video source index 16.
     */
    ICE_source_video16,

    /**
     * The device's video source index 17.
     */
    ICE_source_video17,

    /**
     * The device's video source index 18.
     */
    ICE_source_video18,

    /**
     * The device's video source index 19.
     */
    ICE_source_video19,

    /**
     * The device's audio source index 0.
     */
    ICE_source_audio0,

    /**
     * The device's audio source index 1.
     */
    ICE_source_audio1,

    /**
     * The device's audio source index 2.
     */
    ICE_source_audio2,

    /**
     * The device's audio source index 3.
     */
    ICE_source_audio3,

    /**
     * The device's audio source index 4.
     */
    ICE_source_audio4,

    /**
     * The device's audio source index 5.
     */
    ICE_source_audio5,

    /**
     * The device's audio source index 6.
     */
    ICE_source_audio6,

    /**
     * The device's audio source index 7.
     */
    ICE_source_audio7,

    /**
     * The device's audio source index 8.
     */
    ICE_source_audio8,

    /**
     * The device's audio source index 9.
     */
    ICE_source_audio9,

    /**
     * The device's audio source index 10.
     */
    ICE_source_audio10,

    /**
     * The device's audio source index 11.
     */
    ICE_source_audio11,

    /**
     * The device's audio source index 12.
     */
    ICE_source_audio12,

    /**
     * The device's audio source index 13.
     */
    ICE_source_audio13,

    /**
     * The device's audio source index 14.
     */
    ICE_source_audio14,

    /**
     * The device's audio source index 15.
     */
    ICE_source_audio15,

    /**
     * The device's audio source index 16.
     */
    ICE_source_audio16,

    /**
     * The device's audio source index 17.
     */
    ICE_source_audio17,

    /**
     * The device's audio source index 18.
     */
    ICE_source_audio18,

    /**
     * The device's audio source index 19.
     */
    ICE_source_audio19,

    /**
     * The device's Fm Tuner
     */
    ICE_source_fm_tuner,

    /**
     * None
     */
    ICE_source_unknown

} ICE_DeviceSource;


#endif /* CABOT_ICE_ICE_DEVICE_NUMBER_H_INCLUDED*/
