/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file ice_external_types.h
 * @brief Definition of external input/output interface types.
 *
 */

#ifndef CABOT_ICE_ICE_EXTERNAL_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_EXTERNAL_TYPES_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"
#include "ice/ice_video_types.h"

/**
 * @brief The group of an external input/output interface.
 * 
 *  As returned by ICE_ExternalGetVideoGroupID().
 *
 */
typedef enum ICE_ExternalGroup
{
    ICE_ExternalGroupUnknown = 0,

    /**
     * SCART 1 group
     */
    ICE_ExternalGroupSCART1,

    /**
     * SCART 2 group
     */
    ICE_ExternalGroupSCART2,

    /**
     * HDMI 1 group
     */
    ICE_ExternalGroupHDMI1,

    /**
     * HDMI 2 group
     */
    ICE_ExternalGroupHDMI2,

    /**
     * YPBPR group
     */
    ICE_ExternalGroupYPBPR,

    /**
     * VGA group
     */
    ICE_ExternalGroupVGA,

    /**
     * SVideo group
     */
    ICE_ExternalGroupSVideo,

    /**
     * CVBS group
     */
    ICE_ExternalGroupCVBS,

    /**
     * HDMI 3 group
     */
    ICE_ExternalGroupHDMI3,

    /**
     * SCART 3 group
     */
    ICE_ExternalGroupSCART3,

    /**
     * SCART 4 group
     */
    ICE_ExternalGroupSCART4,

    /**
     * HDMI 4 group
     */
    ICE_ExternalGroupHDMI4,

    /**
     * BluRay group
     */
    ICE_ExternalGroupBluRay,
    
    /**
     * DVD group
     */
    ICE_ExternalGroupDVD,

    /**
      * WiDi group
      */
    ICE_ExternalGroupWiDi,

    /**
      * APM group
      */
    ICE_ExternalGroupAPM

} ICE_ExternalGroup;

/**
 * @brief The type of external input/output interface.
 *
 *        A SCART connector might be represented by three separate interfaces,
 *        one ICE_ExternalTypeAuto, one ICE_ExternalTypeRGB, and one
 *        ICE_ExternalTypeSVideo according to the hardware structure.
 *        ICE_ExternalTypeAuto type will be used when the device is in auto
 *        detection mode.  ICE_ExternalTypeRGB is used to force the device
 *        to RGB mode, and ICE_ExternalTypeSVideo would be used to force
 *        the device to SVideo mode.
 */
typedef enum ICE_ExternalType
{
    /**
     * Unknown
     */
    ICE_ExternalTypeNone,

    /**
     * Auto detection (between RGB and CVBS in SCART)
     */
    ICE_ExternalTypeAuto,

    /**
     * Composite interface
     */
    ICE_ExternalTypeComposite,

    /**
     * YPbPr component interface
     */
    ICE_ExternalTypeYPbPr,

    /**
     * YUV component interface
     */
    ICE_ExternalTypeYUV,

    /**
     * RGB component interface
     */
    ICE_ExternalTypeRGB,

    /**
     * S-Video interface
     */
    ICE_ExternalTypeSVideo,

    /**
     * HDMI interface
     */
    ICE_ExternalTypeHDMI,

    /**
     * VGA interface
     */
    ICE_ExternalTypeVGA,

    /**
     * DVI interface
     */
    ICE_ExternalTypeDVI,

     /**
     * DVD interface
     */
    ICE_ExternalTypeDVD,
        
    /**
     * Analogue two-channel audio connector
     */
    ICE_ExternalTypeAnalogueTwoChannelAudio,

    /**
     * S/PDIF connector
     */
    ICE_ExternalTypeSPDIF,

    /**
     * WiDi
     */
    ICE_ExternalTypeWiDi,

    /**
     * APM
     */
    ICE_ExternalTypeAPM

} ICE_ExternalType;


/**
 * @brief The physical connector type of external input/output interface.
 *
 */
typedef enum ICE_ExternalConnectorType
{
    /**
     * Unknown
     */
    ICE_ExternalConnectorTypeNone,

    /**
     * SCART connector
     */
    ICE_ExternalConnectorTypeSCART,

    /**
     * Composite connector
     */
    ICE_ExternalConnectorTypeComposite,

    /**
     * YPbPr component connector
     */
    ICE_ExternalConnectorTypeYPbPr,

    /**
     * YUV component connector
     */
    ICE_ExternalConnectorTypeYUV,

    /**
     * RGB component connector
     */
    ICE_ExternalConnectorTypeRGB,

    /**
     * S-Video connector
     */
    ICE_ExternalConnectorTypeSVideo,

    /**
     * HDMI connector
     */
    ICE_ExternalConnectorTypeHDMI,

    /**
     * VGA connector
     */
    ICE_ExternalConnectorTypeVGA,

    /**
     * DVI connector
     */
    ICE_ExternalConnectorTypeDVI,

    /**
     * DVD connector
     */
    ICE_ExternalConnectorTypeDVD,
    
    /**
     * Analogue two-channel audio connector
     */
    ICE_ExternalConnectorTypeAnalogueTwoChannelAudio,

    /**
     * S/PDIF connector
     */
    ICE_ExternalConnectorTypeSPDIF,
    
    /**
         * WiDi
         */
    ICE_ExternalConnectorTypeWiDi,

    /**
         * APM
         */
    ICE_ExternalConnectorTypeAPM

} ICE_ExternalConnectorType;


/**
 * @brief The video signal that comes from the interface.
 */
typedef enum ICE_ExternalVideoSignalFormat
{
    /**
     * No signal.
     */
    ICE_ExternalVideoFormatNoSignal,

    /**
     * Composite.
     */
    ICE_ExternalVideoFormatComposite,

    /**
     * Component YPbPr.
     */
    ICE_ExternalVideoFormatYPbPr,

    /**
     * Component YUV.
     */
    ICE_ExternalVideoFormatYUV,

    /**
     * Component RGB.
     */
    ICE_ExternalVideoFormatRGB,

    /**
     * S-Video.
     */
    ICE_ExternalVideoFormatSVideo,

    /**
     * HDMI
     */
    ICE_ExternalVideoFormatHDMI,

    /**
     * VGA signal
     */
    ICE_ExternalVideoFormatVGA,

    /**
     * DVI signal
     */
    ICE_ExternalVideoFormatDVI

} ICE_ExternalVideoFormat;


/**
 * @brief The audio signal that comes from the interface.
 */
typedef enum ICE_ExternalAudioSignalFormat
{
    /**
     * No signal.
     */
    ICE_ExternalAudioFormatNoSignal,

    /**
     * Two-channel analogue
     */
    ICE_ExternalAudioFormatTwoChAnalog,

    /**
     * HDMI
     */
    ICE_ExternalAudioFormatHDMI,

    /**
     * DVI interface
     */
    ICE_ExternalAudioFormatDVI

} ICE_ExternalAudioFormat;


/**
 * @brief External input/output's mode. It can be in input mode,
 *        output mode, input/output mode or it can be disabled.
 */
typedef enum ICE_ExternalMode
{
    /**
     * None.
     */
    ICE_ExternalModeNone,

    /**
     * Disabled.
     */
    ICE_ExternalModeDisabled,

    /**
     * Input mode.
     */
    ICE_ExternalModeInput,

    /**
     * Output mode.
     */
    ICE_ExternalModeOutput,

    /**
     * Input/output mode.
     */
    ICE_ExternalModeInputOutput

} ICE_ExternalMode;


/**
 * @brief Representation of external interface's capabilities.
 */
typedef struct ICE_ExternalCapability
{
    /**
     * Can output video to external devices.
     */
    frost_bool ICE_ExternalCapabilityCanOutputVideo;

    /**
     * Can output audio to external devices.
     */
    frost_bool ICE_ExternalCapabilityCanOutputAudio;

    /**
     * Can take video from external devices.
     */
    frost_bool ICE_ExternalCapabilityCanInputVideo;

    /**
     * Can take audio from external devices.
     */
    frost_bool ICE_ExternalCapabilityCanInputAudio;

    /**
     * Can be outputted from scart2(ext2) interface.
     */
    frost_bool ICE_ExternalCapabilityCanBeOutputtedFromExt2;

    /**
     * Its video can appear on main window.
     */
    frost_bool ICE_ExternalCapabilityCanUseMainWindow;

    /**
     * Its video can appear on PIP window.
     */
    frost_bool ICE_ExternalCapabilityCanUsePIPWindow;

    /**
     * Its video can appear on left PAP window.
     */
    frost_bool ICE_ExternalCapabilityCanUseLeftPAPWindow;

    /**
     * Its video can appear on right PAP window.
     */
    frost_bool ICE_ExternalCapabilityCanUseRightPAPWindow;

    /**
     * Its video can be zoomed/scaled to 4:3
     */
    frost_bool ICE_ExternalCapabilityCanBeScaled4_3;
 
    /**
     * Its video can be zoomed/scaled to 16:9
     */
    frost_bool ICE_ExternalCapabilityCanBeScaled16_9;
 
    /**
     * Its video can be zoomed/scaled to 16:9 Zoom mode
     */
    frost_bool ICE_ExternalCapabilityCanBeScaled16_9Zoom;
 
    /**
     * Its video can be zoomed/scaled to 16:9 Subtitle mode
     */
    frost_bool ICE_ExternalCapabilityCanBeScaled16_9Subtitle;
 
    /**
     * Its video can be zoomed/scaled to 14:9
     */
    frost_bool ICE_ExternalCapabilityCanBeScaled14_9;
 
    /**
     * Its video can be zoomed/scaled to panoramic mode
     */
    frost_bool ICE_ExternalCapabilityCanBeScaledPanoramic;
    
} ICE_ExternalCapability;



/**
 * The types of status messages that can be sent by the ICE implementation
 * to Eclipse.
 */
typedef enum ICE_ExternalStatus {

    /**
     * Send when a device has been connected through an external interface
     *   or the connected device started to provide audio and/or video.
     */
    ICE_ExternalStatusDeviceConnected,

    /**
     * Send when a device has been disconnected from an external interface
     *   or the connected device was turned off.
     */
    ICE_ExternalStatusDeviceDisconnected,

    /**
     * Send when the external interface attributes change.
     *
     * External interface's attributes are format, (any other?).
     *
     * The driver level software should call the status callback and pass
     * this code on detection of a change in format. 
     * Following a call to this callback, Eclipse will determine
     * the external interface attributes via calls to
     * #ICE_ExternalGetDetectedAudioSignalFormat,
     * #ICE_ExternalGetDetectedVideoSignalFormat, #ICE_ExternalGetVideoType,
     * #ICE_ExternalGetAudioType
     * #ICE_ExternalGetMode.
     */
    ICE_ExternalStatusAttributesChanged

} ICE_ExternalStatus;



/**
 * @brief Callback function to inform Eclipse of the external interface status.
 *
 * See the comments in #ICE_ExternalStatus for when this callback should be
 * called.
 *
 * @param interface_index The index of the external interface for which this instance of the call is valid.
 * @param status The external interface status.
 * @param user_data As supplied in the call to #ICE_ExternalOpen.
 */
typedef void (*ICE_ExternalStatusCallback)(frost_int interface_index, ICE_ExternalStatus status, void* user_data);

/**
 * @brief Structure to store current sources on the main and secondary displays.
 */
typedef struct ICE_ExternalStateInfo
{
    /**
     * Main (Primary) video device source
     */
    ICE_DeviceSource video_on_main;
    /**
     * Secondary video device source
     */
    ICE_DeviceSource video_on_secondary;
}ICE_ExternalStateInfo;

/**
 * @brief External Device display aspect ratios.
 */
typedef enum ICE_ExternalAspectRatio
{
    ICE_external_aspect_ratio_none,
    ICE_external_4_by_3,
    ICE_external_16_by_9

} ICE_ExternalAspectRatio;
    
#ifdef __cplusplus
} /* end of extern "C" block */
#endif

#endif /* CABOT_ICE_ICE_EXTERNAL_TYPES_H_INCLUDED*/
