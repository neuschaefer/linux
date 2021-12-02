/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot's interface from the Eclipse system to hardware.
 */

#ifndef CABOT_ICE_ICE_HARDWARE_H_INCLUDED
#define CABOT_ICE_ICE_HARDWARE_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_video_types.h"
#include "ice/ice_tuner.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Hardware initialisation.
 *
 * ICE_HardwareInit() shall be called before any driver specific
 * initialisation code.
 * 
 * HD MHEG used in Freesat and T2 profiles uses CURL library provided 
 * by the platform. The curl_global_init() function is NOT thread safe 
 * so it must be called by the platform in the sigle-threaded 
 * initialisation phase, before this function is called or inside it.
 *
 * See documentation of curl_global_init() for details.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_HardwareInit(void);


/**
 * @brief Reset hardware.
 *
 * ICE_HardwareReset() should be called to reset the system as if a power-cycle
 * had been performed.  Typically this will run the Flash boot loader, or other
 * initial program loader code.
 *
 * @note This function should not return.
 */
void ICE_HardwareReset(void);


/**
 * @brief Enable or disable the hardware watchdog (if available).
 *
 * Eclipse will call ICE_HardwareWatchdog() to enable or disable the watchdog.
 * The ICE implementation should set appropriate timings and ensure the
 * watchdog is being kicked often enough.
 *
 * A watchdog-initiated reset should have the same effect as a call to
 * ICE_HardwareReset() - i.e. it should run the Flash boot loader, or other
 * initial program loader code.
 *
 * @param  enable #frost_true shall enable watchdog, #frost_false shall
 *                disable it.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_HardwareWatchdog(frost_bool enable);

/**
 * @brief Set the hardware watchdog timeout period(if watchdog available).
 *
 * @param  timeout_period Watchdog timeout period to be kicked.
 */
void ICE_HardwareSetWatchdogTimeout(frost_int timeout_period);

/**
 * @brief Get the hardware watchdog timeout period(if watchdog available).
 *
 * @return watchdog timeout period value.
 */
frost_int ICE_HardwareGetWatchdogTimeout(void);


/**
 * @brief Enumeration of reasons that "standby mode" has been exited.
 */
typedef enum ICE_HardwareStandbyModeExitType {

    /**
     * Exited because a call to ICE_HardwareStandby() failed,
     * indicating that hardware standby mode cannot be entered.  In this case
     * Eclipse will 'simulate' standby mode by switching off all AV outputs.
     */
    ICE_standby_failed = 0,

    /**
     * Exited because of a remote-control keypress.
     */
    ICE_standby_keypress,

    /**
     * Exited because of a schedule timer event.
     */
    ICE_standby_timer,

    /**
     * Exited because a CI/CAM module or an embedded CA stack
     * does not allow entering standby mode.
     */
    ICE_standby_ca_failed,

    /**
     * Exited because an HDMI connector has requested 
     * to route AV to user. e.g. DVD One touch play.
     */
    ICE_standby_hdmi,
    
    /**
     * Exited because the vga PC connector has requested 
     * to route AV to user.
     */
    ICE_standby_pcinput

} ICE_HardwareStandbyModeExitType;


/**
 * @brief Enter passive low-power standby state.
 *
 * ICE_HardwareStandby() shall be called by Eclipse to set the hardware into
 * passive standby mode.  The implementation should set the appropriate low 
 * power mode of all components.  This function does not return until standby 
 * mode is exited.
 *
 * @note If the implementation wishes a remote control (or front panel)
 * keypress to cause the standby mode to exit, it must implement the
 * appropriate code to cause the ICE_HardwareStandby() function to return
 * as a result of this keypress.  The corresponding #ICE_KeypressId (usually
 * ICE_keypress_power) should not be passed on to the application.
 *
 * @note The implementation must not go into standby mode unless this function
 * is called.  I.e. it is incorrect for the implementation to intercept the
 * power key when the system is not in standby mode and to then enter passive
 * standby mode.  In this case, the ICE_keypress_power keypress ID should be
 * passed on using the ICE_KeypressFunction() (just as any other key), and
 * the UI application determines what steps should be taken to enter standby
 * mode.  Typically, this might involve saving any settings and finishing with
 * a call to ICE_HardwareStandby().
 *
 * @note The implementation should not manipulate scart signals. The UI
 * Application will ensure that the scart is switched off and on at the
 * appropriate time.
 *
 * @param seconds Number of seconds after which box should leave passive
 *        standby mode. Zero value indicates that there is no scheduled
 *        "wake-up" time and that standby mode should only be exited upon a
 *        remote control (or front panel) keypress.
 *
 * @return An #ICE_HardwareStandbyModeExitType indicating why passive standby
 *         mode was exited.
 */
ICE_HardwareStandbyModeExitType ICE_HardwareStandby(frost_ulong seconds);

/**
 * @brief Leave powered off state.
 *
 * On some systems ICE_HardwareStandby will power down the main board
 * and leave a secondary low power processor monitoring RC and other 
 * wake up sources, so ICE_HardwarePowerUpReason is called during the
 * initialiation to determine why the device is leaving power down state.
 *
 */
ICE_HardwareStandbyModeExitType ICE_HardwarePowerUpReason(void);

/**
 * @brief inform the implementation about the state of active standby mode.
 *
 * For UI applications where passive low-power standby mode is not used, this
 * function may be called to inform the ICE implementation that a "fake standby"
 * mode has been entered.  EACEM refers to this mode as "active standby".
 * The implementation should not block when this function is called, but may use
 * knowledge of the active standby state to power-down unnecessary components for 
 * standby mode.  For example, the fan may not be required in active standby mode.
 *
 * @note This is for eclipse use only.
 *       There is an EASI interface alternative. 
 *
 * @param active frost_true indicates that active standby mode is being entered;
 *               frost_false indicates that active standby mode is being exited.
 */
void ICE_HardwareActiveStandby(frost_bool active);

/**
 * @brief Set the state of a front panel LED indicator.
 *
 * @param number    LED to change state of.
 * @param state     New LED state.
 *                  0 = Off
 *                  1 = On
 *                  2 = Flashing.
 *
 * @note Due to the wide range of platforms below ICE it is not possible to
 * enumerate all the different LEDs and their possible states.  This
 * interface is intentionally flexible and should be implemented in the way
 * that is most appropriate for the platform.  The corresponding UI
 * application for the platform will invoke this function appropriately
 * via the EASIControlInterface.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_HardwareLed(frost_uint number, frost_uint state);


/**
 * @brief Structure defining data to be displayed on any front panel display.
 *
 * @note Due to the wide range of platforms below ICE it is not possible to
 * define parameters that specify all types of displayed information.
 * However, an 'id' of value zero is reserved to mean that 'data' points to
 * frost_int containing the channel number.
 * This structure provides a flexible mechanism by which such information can
 * be passed.
 */
typedef struct ICE_FrontPanelData
{
    /**
     * Identifies which part of the front panel display is to be updated.
     * 'id' = 0, means 'data' points to channel number - held in an frost_int.
     * 'id' = 1, means 'data' points to a string for use with seven 16 segment display or better.
     *                        An empty string must always clear the display and leave it blank.
     *                        "SCAN", "init" or "boot" may be displayed on a 7 segment display. 
     * 'id' = 2, means 'data' points to a string with "HHMM" or "----" if time not known.
     *                        This is used when the main cpu is in active standby or is about to be powered off.
     *
     * various states like AV and Rec are indicated by ICE_HardwareLed, these may be used to display messages if no
     * led is available.
     * ICE_HardwareLed is also used to control the dots between the digits of the segments.
     *  
     * For other information, the 'id' values are undefined (e.g. for time,
     * other status information, etc.).
     */
    frost_uint id;

    /**
     * The data that the front panel should be set to.
     */
    const void* data;

} ICE_FrontPanelData;


/**
 * @brief Show information on front panel display (LCD, 7-segment etc.).
 *
 * @param front_panel_data  Information to be displayed.  The precise meaning
 * of the data pointer within this structure should be defined by the ICE
 * implementer.  This information should be made available to the UI
 * application writer so that the data can be set correctly by the application.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_HardwareFrontPanelDisplay(
                    const ICE_FrontPanelData* front_panel_data);


/**
 * @brief Set channel of the RF modulator.
 *
 * This function sets up the RF modulator.  The function can also be used
 * to switch off the modulator.
 *
 * @param encoding_mode The video encoding mode to use.  It is anticipated
 *        that any given ICE implementation will only support a subset of
 *        modes.
 * @param frequency_khz The RF centre frequency in kHz (not the vision or
 *        sound sub-carrier frequencies), or special value zero.  Zero
 *        indicates that the modulator should be switched off or disabled.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note The parameters are provided for modulators that can be configured.
 * If a modulator is capable of different sound carrier separations
 * (e.g. 5.5MHz and 6.0MHz) then this can be determined from the encoding_mode.
 * The modulator might also conceivably need to know whether the signal is PAL,
 * NTSC or SECAM e.g. for setting bandwidth. If an implementation cannot
 * utilise a parameter, then the parameter should be ignored.
 */
frost_bool ICE_HardwareModulator(ICE_VideoEncodingMode encoding_mode,
                                 ICE_Frequency frequency_khz);

/**
 * @brief Structure defining the properties of the video panel.
 */
typedef struct ICE_HardwarePanelInfo
{
    /**
     *  The width of the panel in pixels.
     */
    frost_uint width;

    /**
     * The height of the panel in pixels.
     */
    frost_uint height;

} ICE_HardwarePanelInfo;

/**
 * @brief Get video panel's properties.
 *
 * @param panel_info The pointer to panel info to be filled in.
 * 
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_HardwareGetPanelInfo(ICE_HardwarePanelInfo * panel_info);


/**
 * @brief Get hardware state.
 *
 * Generic function to get platform-specific hardware parameters.
 * The purpose of this function is to provide a standard interface
 * to any platform-specific hardware modules.
 *
 * The precise meaning parameters should be defined by the ICE
 * implementer.  This information should be made available to the UI
 * application writer so that the data can be set correctly by the application.
 *
 * @param module hardware module to retrieve the parameter(s) from
 * @param state  pointer to the buffer to receive current module state
 * 
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_HardwareGetState(frost_uint module, void * state);

/**
 * @brief Change hardware state.
 *
 * Generic function to set platform-specific hardware parameters.
 * The purpose of this function is to provide a standard interface
 * to any platform-specific hardware modules.
 *
 * The precise meaning parameters should be defined by the ICE
 * implementer.  This information should be made available to the UI
 * application writer so that the data can be set correctly by the application.
 *
 * @param module hardware module to set the parameter(s) for
 * @param state  pointer to the new state for the module
 * 
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_HardwareSetState(frost_uint module, const void * state);

/**
 * @brief Enable / Disable the child lock option for front panel keys.
 *
 * If child lock is enabled, then front panel buttons will not function when
 * they are pressed.
 *
 * @param enable If #frost_true, enable child lock, if #frost_false disable child lock.
 */
frost_bool ICE_HardwareFrontPanelChildLock(frost_bool enable);

/**
 * @brief Mute / Demute audio DACs to avoid pop noise.
 *
 * @param demute mute or demute state for dacs.
 */
void ICE_HardwareAudioMute(frost_bool demute);

/**
*@brief Definition of application type for Illuminated Logo Mode. 
*/
typedef enum ICE_IlluminatedLogoMode
{
    /**
     * Illuminated logo is always on.
     */
    ICE_illuminated_logo_on,

    /**
     * Illuminated logo is always off.
     */
    ICE_illuminated_logo_off,

    /**
     * Illuminated logo is on only in standby.
     */
    ICE_illuminated_logo_standby_on,

    /**
     * Illuminated logo is on only in working mode.
     */
    ICE_illuminated_logo_standby_off
    
} ICE_IlluminatedLogoMode;


/**
 * @brief Sets the mode of the illuminated logo.
 *
 * @param mode Illuminated logo mode to define at which states of the TV, logo will be on and off.
 */
void ICE_HardwareSetIlluminatedLogo(ICE_IlluminatedLogoMode mode);

/**
 * @brief Set the Standby Led mode.
 * 
 * @param mode standby Led mode to define at standby states of the TV, led will be on and off.
 * 
 */
void ICE_HardwareSetStandbyLed(frost_bool mode);

/**
 * @brief to get touchpad sw version.
 * 
 * @return Version Number of Touchpad SW
 * 
 */
frost_int8 ICE_HardwareGetTouchPadVer(void);

/**
 * @brief Update and store last tuning, audio & video parameters.
 *
 * Generic function to set platform-specific hardware parameters
 * that are required for a successful tune and audio/video start.
 * The purpose of this function is to sync UI and ICE level in terms of
 * last stored channel parameters.
 *
 * The precise meaning parameters should be defined by the ICE
 * implementer.
 *
 */
void ICE_HardwareWriteFastTurnOnParams(frost_bool locked);



typedef enum ICE_HotelExternalOutputVoltage
{
    ICE_HotelExternalOutputVoltage_0V,
    ICE_HotelExternalOutputVoltage_5V,
    ICE_HotelExternalOutputVoltage_12V
} ICE_HotelExternalOutputVoltage;


/**
* Structure of Tuner information that includes firmware version, product id and build number.
*/
typedef struct {

    /**
     * Firmware version of tuner.
     */    
    frost_uint16 TunerFirmwareVer;

    /**
     * Product ID of tuner.
     */
    frost_uint16 TunerProductId;

    /**
     * Build Number of tuner.
     */
    frost_uint16 TunerBuildNum;
    
}TunerInfo;

/**
*@brief Definition for selection type of Tuner Information.
*/
typedef enum{

    /**
     * to select tuner firmware.
     */
    ICE_tuner_firmware,

    /**
     * to select tuner product id.
     */
    ICE_tuner_product_id,

    /**
     * to select tuner build number.
     */
    ICE_tuner_build_num,  
    
}ICE_TunerInfoType;

/**

 * @brief to get tuner firmware version.
 * 
 * @param info describe type of tuner information
 * 
 * @param variable store selected tuner information
 *  
 */
void ICE_HardwareGetTunerInfo(ICE_TunerInfoType info, frost_uint16 *variable);

/**
 * @brief Sets voltage level from hotel card's external output
 *
 * @return frost_true if successfull
 */
frost_bool ICE_HardwareSetHotelExternalOutputVoltage(ICE_HotelExternalOutputVoltage output_voltage);


/**
 * @brief Enables or disables audio output from hotel card used for driving bathroom speakers
 *
 * @return frost_true if successfull
 */
frost_bool ICE_HardwareSetHotelBathroomSpeakerOutEnable(frost_bool enable);

/**

 * @brief Disable FastTurnOn property.
 *
 * @param disable Next bootup will be handled in a traditional way, if set to 'true'
 *
 */
void ICE_HardwareDisableFastTurnOn(frost_bool disable);

typedef struct ICE_RfRemoteInfo
{
    frost_uint8 discovery_lqi_threshold;
    frost_uint8 tv_power_mode;
    frost_uint8 paging_mode;
    frost_uint8 pairing_mode;
    frost_uint8 mac_address[8];
    frost_uint8 rf_board_major_sw_version;
    frost_uint8 rf_board_minor_sw_version;
    frost_uint8 rf_remote_major_sw_version;
    frost_uint8 rf_remote_minor_sw_version;
    frost_uint8 checksum;
} ICE_RfRemoteInfo;

typedef enum ICE_RfRemotePagingStates
{
    ICE_RF_REMOTE_PAGING_SERVICE_NOT_ACTIVATED,
    ICE_RF_REMOTE_PAGING_SERVICE_ACTIVATED,
    ICE_RF_REMOTE_PAGING_SUCCESSFULL,
    ICE_RF_REMOTE_PAGING_NOT_SUCCESSFULL,
}ICE_RfRemotePagingStates;

typedef enum ICE_RfRemotePairingStates
{
    ICE_RF_REMOTE_NO_NOTIFICATION_PAIRING_SUCCESSFULL,
    ICE_RF_REMOTE_PAIRING_SUCCESSFULL,
}ICE_RfRemotePairingStates;

typedef enum ICE_RfRemotePowerModes
{
    ICE_RF_REMOTE_POWER_ON,
    ICE_RF_REMOTE_STANDBY,
}ICE_RfRemotePowerModes;

frost_bool ICE_GetRfRemoteInfo(ICE_RfRemoteInfo* StructBuff);
frost_bool ICE_SetRfRemoteDiscoveryLqiThreshold(frost_uint8 Value);
frost_bool ICE_SetRfRemotePowerMode(ICE_RfRemotePowerModes Mode);
frost_bool ICE_SetRfRemotePagingMode(ICE_RfRemotePagingStates Mode);
frost_bool ICE_SetRfRemoteMacAddress(ICE_RfRemoteInfo* StructBuff);
frost_bool ICE_GetRfRemotePagingStatus(ICE_RfRemotePagingStates* PagingStatus, frost_bool* PagingRequestReceived);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_HARDWARE_H_INCLUDED*/
