/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file ice_external.h
 * @brief  Interface to the television set's external input/outputs
 *
 * This file defines the ICE interfaces to the external input and outputs like
 *  SCART, Front-AV, HDMI etc.
 *
 * The main idea in this interface is that, a logical external interface
 * consists of two components: A video component and an audio component.
 * Two or more logical external interfaces may share a video component
 * and/or an audio component.  The link between an interface and its
 * video and audio components is provided by #ICE_DeviceSource enumeration.
 *
 * However, a logical external interface may consist of a video component only
 * or an audio component only.
 *
 * As an example, consider the following hardware configuration with
 * a SCART connector, a VGA connector and a 2-channel analog audio
 * connector:
 <PRE>
  __________________               ________
 |                 /              /  VGA   \               L / R
 |   SCART 1      /              /__________\              O   O
 |_______________/                    |                      |
    |   |   |   |                     |                      |
    |   |   |   |                     |                      |
    |   |   |   |                #ICE_source_video3    #ICE_source_audio1
    |   |   |   '
    |   |   |    \
    |   |   '     \
    |   '    \     \
    |    \    \     \
    |     \    \     \
    |      \    \     \ #ICE_source_video0 (#ICE_ExternalTypeComposite, or probably #ICE_ExternalTypeAuto)
    |       \    \ #ICE_source_video1 (#ICE_ExternalTypeRGB)
    |        \ #ICE_source_video2 (#ICE_ExternalTypeSVideo)
    #ICE_source_audio0

    The following logical interfaces may then be defined:

       Audio components     Logical Ext Interfaces      Video Components
                                                       __________________
     __________________  <---------- 0 -------------> |                  |
    |                  | <---.                        |ICE_source_video0 |
    |ICE_source_audio0 | <--. \                       |__________________|
    |__________________| <-. \ \                       __________________
     __________________     \ \ '--- 1 --.            |                  |
    |                  |     \ \          '---------> |ICE_source_video1 |
    |ICE_source_audio1 |      \ \                     |__________________|
    |__________________| <--.  \ '-- 2 ----.           __________________
                             \  \           \         |                  |
                              \  \           '------> |ICE_source_video2 |
                               \  '- 3 --.            |__________________|
                                \         \            __________________
                                 \         '--------> |                  |
                                  '- 4 -------------> |ICE_source_video3 |
                                             .------> |__________________|
                NO AUDIO COMPONENT<- 5 -----'                
 </PRE>
 *
 * 
 */

#ifndef CABOT_ICE_ICE_EXTERNAL_H_INCLUDED
#define CABOT_ICE_ICE_EXTERNAL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"
#include "ice/ice_external_types.h"

/***************************************************************************/
/**
 * @name Functions to query and set properties of external interfaces, and to open and close them
 * @{
 */
/***************************************************************************/

/**
 * @brief Initialises the external interfaces.
 *
 * @return #frost_true if successful, #frost_false if unsuccessful.
 */
frost_bool ICE_ExternalInit(void);


/**
 * @brief Returns the number of external interfaces in the system.
 *        This is the total number of them, not the number of active
 *        ones.  If the upper layers need to filter out the disabled
 *        interfaces, they need to do it themselves.
 *
 * @return The number of external interfaces.
 */
frost_int ICE_ExternalNumberOfInterfaces(void);

/**
 * @brief Gets the video source device for the given interface.
 *
 * @param ext_index The index of the external interface.
 *
 * @return The video source device for this external interface, if exists.
 *         #ICE_source_unknown if this interface does not have a video
 *         component.
 */
ICE_DeviceSource ICE_ExternalGetVideoDevice(frost_int ext_index);

/**
 * @brief Gets the audio source device for the given interface.
 *
 * @param ext_index The index of the external interface.
 *
 * @return The audio source device for this external interface, if exists.
 *         #ICE_source_unknown if this interface does not have an audio
 *         component.
 */
ICE_DeviceSource ICE_ExternalGetAudioDevice(frost_int ext_index);

/**
 * @brief Initiate/activate and terminate/deactivate HDMI Audio Return Channel (ARC) for given HDMI input.
 *
 * @param activate True Initiate False Terminate
 * @param hdmi_group HDMI group ARC to be initiated or terminated
 *
 * @return frost_true on success, frost_false otherwise.
 */
frost_bool ICE_ExternalSetHDMIAudioReturnChannel(frost_bool activate, ICE_ExternalGroup hdmi_group);

/**
 * @brief Gets the type of the aux video input/output.
 *        See #ICE_ExternalType for more information.
 *
 * This is not the physical connector type, but
 * rather how the video signal is carried.  For example, an interface with
 * physical connector type #ICE_ExternalConnectorTypeSCART can have a type
 * of #ICE_ExternalTypeAuto, #ICE_ExternalTypeComposite, #ICE_ExternalTypeRGB
 * or #ICE_ExternalTypeSVideo.
 *
 * In the case where the type is #ICE_ExternalTypeAuto, the current signal
 * format can be accessed using #ICE_ExternalGetDetectedVideoSignalFormat. 
 *
 * @param ext_index The index of the external interface.
 *
 * @return Video type of the external interface.
 */
ICE_ExternalType ICE_ExternalGetVideoType(frost_int ext_index);

/**
 * @brief Gets the type of the aux audio input/output.
 *        See #ICE_ExternalType for more information.
 *
 * This is not the physical connector type, but
 * rather how the audio signal is carried.
 *
 * @param ext_index The index of the external interface.
 *
 * @return Audio type of the external interface.
 */
ICE_ExternalType ICE_ExternalGetAudioType(frost_int ext_index);

/**
 * @brief Gets the physical connector type of the aux video input/output.
 *        See #ICE_ExternalConnectorType for more information.
 *
 * @param ext_index The index of the external interface.
 *
 * @return Type of the external interface.
 *
 */
ICE_ExternalConnectorType ICE_ExternalGetVideoConnectorType(frost_int ext_index);

/**
 * @brief Gets the physical connector type of the aux audio input/output.
 *        See #ICE_ExternalConnectorType for more information.
 *
 * @param ext_index The index of the external interface.
 *
 * @return Audio type of the external interface.
 *
 */
ICE_ExternalConnectorType ICE_ExternalGetAudioConnectorType(frost_int ext_index);

/**
 * @brief Gets the name of the aux interface. This is a short string that
 *        identifies the interface to the user.  Two interfaces may have
 *        the same name but this is not necessarily a good idea. Example
 *        names could be "SC1", "AUX 1", "EXT 2", "EXT-2S", "VGA" etc.
 * 
 *        The interfaces with names "EXT1" and "EXT2" are special.  Usually,
 *        the interface with the name "EXT1" has the AV of the main window,
 *        while the interface with the name "EXT2" can be configured to
 *        output the TV, or another external interface's input.
 *
 *        The upper layers will identify the EXT1 and EXT2 by the name.
 *
 *        The returned pointer must NOT be free'ed.
 *
 * @param ext_index The index of the external interface.
 * 
 * @return The name of the interface.  NULL if the name cannot be obtained.
 *
 */ 
char * ICE_ExternalGetName(const frost_int ext_index);
 

/**
 * @brief Get the capabilities of the external interface.
 *
 * @param aux_capability The capabilities of the given interface.
 * @param ext_index The index of the external interface.
 *
 * @return #frost_true if capabilities are known, otherwise 
 *         #frost_false.
 */
frost_bool ICE_ExternalGetCapability(ICE_ExternalCapability * aux_capability,
                                      const frost_int ext_index);




/**
 * @brief Get the video signal format which the external interface currently detects.
 *        For example, an interface with format #ICE_ExternalTypeAuto
 *        (which might be a SCART) might return #ICE_ExternalVideoFormatComposite
 *        or #ICE_ExternalVideoFormatRGB, or #ICE_ExternalVideoFormatSVideo
 *
 * @param aux_format The detected signal format of the given interface.
 * @param ext_index The index of the external interface.
 *
 * @return #frost_true if format can be obtained, otherwise 
 *         #frost_false.
 */
frost_bool ICE_ExternalGetDetectedVideoSignalFormat(ICE_ExternalVideoFormat * aux_format, const frost_int ext_index);


/**
 * @brief Get the audio signal format which the external interface currently detects.
 *
 * @param aux_format The detected signal format of the given interface.
 * @param ext_index The index of the external interface.
 *
 * @return #frost_true if format can be obtained, otherwise 
 *         #frost_false.
 */
frost_bool ICE_ExternalGetDetectedAudioSignalFormat(ICE_ExternalAudioFormat * aux_format, const frost_int ext_index);


/**
 * @brief Get the mode of the external interface. It can be in output mode,
 *        in input mode, in input/output mode or it could be disabled.
 *
 * @param ext_index The index of the external interface.
 *
 * @return Mode if mode can be obtained, otherwise 
 *         #ICE_ExternalModeNone.
 */
ICE_ExternalMode ICE_ExternalGetMode(const frost_int ext_index);


/**
 * @brief Set the mode of the external interface. It can be set to output mode,
 *        input mode, or it could be disabled.
 *
 * @param aux_mode The requested mode for the given interface.
 * @param ext_index The index of the external interface.
 *
 * @return #frost_true if mode could be set, otherwise 
 *         #frost_false.
 */
frost_bool ICE_ExternalSetMode(const ICE_ExternalMode aux_mode, const frost_int ext_index);

/**
 * @brief Get the group ID of the given interface's video source.
 *        A group of interfaces is a collection of interfaces that constitutes
 *        one physical connector.  For example, an interface of type
 *        #ICE_ExternalTypeComposite, an interface of type #ICE_ExternalTypeRGB
 *        and another interface of type #ICE_ExternalTypeSVideo could form a
 *        group of interfaces, which is a SCART connector.
 *
 *        The #ICE_ExternalConnectorType of the interfaces that have the same
 *        groupID must be the same.
 *
 *        GroupIDs start from 1.  0 is not a valid groupID.
 *
 * @param ext_index The index of the external interface.
 *
 * @return Video source's groupID if successful, 0 if unsuccessful.
 *
 */
ICE_ExternalGroup ICE_ExternalGetVideoGroupID(const frost_int ext_index);

/**
 * @brief Get the group ID of the given interface's audio source.
 *        See #ICE_ExternalGetVideoGroupID for more info.
 *
 *        The #ICE_ExternalConnectorType of the interfaces that have the same
 *        groupID must be the same.
 *
 *        GroupIDs start from 1.  0 is not a valid groupID.
 *
 * @param ext_index The index of the external interface.
 *
 * @return Audio source's groupID if successful, 0 if unsuccessful.
 *
 */
ICE_ExternalGroup ICE_ExternalGetAudioGroupID(const frost_int ext_index);



/**
 * @brief Set the audio mode of the external HDMI input to be digital (i.e. derived from the HDMI input) or
 *        analogue (derived from a customer specified analogue input port)
 *
 *        HDMI is capable of inputting a DVI signal, but in this case there is no associated audio in the date stream.
 *        Therefore it must be sourced from another input, typically one of the AV cinches.  This function is called
 *        to instruct ICE to perform what ever routing is necessary to select the correct audio when the external input is selected.
 *
 *        For non HDMI external inputs, this function has no use.
 *
 * @param is_analogue - set to #frost_false when using the HDMI audio stream, #frost_true when using an analogue audio input
 *
 * @param ext_index The index of the external interface.
 *
 * @return #frost_true if mode could be set, otherwise #frost_false.
 */
frost_bool ICE_ExternalSetHDMIAudioModeAnalogue(frost_bool is_analogue, const frost_int ext_index);


/**
 * @brief Open an external interface.  This could be used to set a
 *        call-back to the aux interface so that upper layers can
 *        be notified when a device is connected to our TV set through
 *        SCART or HDMI (or any other connector) and this device
 *        starts providing input to our set.
 *
 * @param ext_index The index of the external interface.
 * @param callback_func The callback function that is called when status
 *        of the aux interface changes.
 * @param user_data Generic void pointer that can be used to pass extra parameters.
 *
 * @return #frost_true if aux interface could be opened, otherwise 
 *         #frost_false.
 */
frost_bool ICE_ExternalOpen(const frost_int ext_index, 
                                ICE_ExternalStatusCallback callback_func,
                                void* user_data);


/**
 * @brief Close an external interface.  Release any resources that were
 *        kept for this aux interface.
 *
 * @param ext_index The index of the external interface.
 *
 * @return #frost_true if it could be closed successfully, otherwise 
 *         #frost_false.
 */
frost_bool ICE_ExternalClose(const frost_int ext_index);

/***************************************************************************/
/**
 * @}
 * @name Functions for forwarding AV to external interfaces
 * @{
 */
/***************************************************************************/


/**
 * @brief Dynamically check if outputting audio and video from the given source
 *        device to the given target interface is possible at the time of
 *        the function call.  The result may change according to the timing
 *        (ie, the current state of the system).
 *
 * @param source Source device
 * @param target_interface Target external interface
 * @param state Current state of the main and secondary displays.
 *
 * @return #frost_true if outputting this source from the given target interface is
 *         possible.  #frost_false otherwise.
 */
frost_bool ICE_ExternalIsOutputPossible(ICE_DeviceSource source, 
                                           const frost_int target_interface,
                                           const ICE_ExternalStateInfo* state);

/**
 * @brief Set the video output of a given external interface.
 *
 * @param source Source device.
 * @param target_interface Target external interface.
 *
 * @return #frost_true if operation is succesful. #frost_false otherwise.
 */
frost_bool ICE_ExternalSetVideoOutput(ICE_DeviceSource source, const frost_int target_interface);

/**
 * @brief Disconnect the last connected video source from the given external interface.
 *
 * @param target_interface Target external interface.
 *
 * @return #frost_true if operation is succesful. #frost_false otherwise.
 */
frost_bool ICE_ExternalResetVideoOutput(const frost_int target_interface);

/**
 * @brief Set the audio output of a given external interface.
 *
 * @param source Source device.
 * @param target_interface Target external interface.
 *
 * @return #frost_true if operation is succesful. #frost_false otherwise.
 */
frost_bool ICE_ExternalSetAudioOutput(ICE_DeviceSource source, const frost_int target_interface);

/**
 * @brief Disconnect the last connected audio source from the given external interface.
 *
 * @param target_interface Target external interface.
 *
 * @return #frost_true if operation is succesful. #frost_false otherwise.
 */
frost_bool ICE_ExternalResetAudioOutput(const frost_int target_interface);

/**
 * @brief Set aspect ratio of an external device (mainly SCART).
 * 
 * @param target_interface Target external interface. 
 * @param aspect_ratio Aspect ratio that will be applied.
 * 
 * @return #frost_true on success, #frost_false otherwise.
 */
frost_bool ICE_ExternalSetAspectRatio(const frost_int target_interface, 
                                         const ICE_ExternalAspectRatio aspect_ratio);

/**
 * @brief Get aspect ratio of an external device (mainly SCART).
 *
 * @param target_interface Target external interface.
 *
 * @return Aspect ratio fo specified device.
 */
ICE_ExternalAspectRatio ICE_ExternalGetAspectRatio(const frost_int target_interface);

/**
 * @brief Enable or disable pass the IR keys through DV loader
 *
 * @param enable enable or disable the output
 * @param forced whether to check previous state or not.
 */
void ICE_ExternalSetDVDIREnable(const frost_bool enable, const frost_bool forced);

/**
 * @brief Reset the state to previous, set current forced flag to false
 */
void ICE_ExternalRevertDVDIREnable();

/**
 * @brief Return current state
 *
 * @return #frost_true if current state is enabled, #frost_false otherwise
 */
frost_bool ICE_ExternalGetDVDIREnable();


/**
 * @}
 */
#ifdef __cplusplus
} /* end of extern "C" block */
#endif

#endif /* CABOT_ICE_ICE_EXTERNAL_H_INCLUDED*/
