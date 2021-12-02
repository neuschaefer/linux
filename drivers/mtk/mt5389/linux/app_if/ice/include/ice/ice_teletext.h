/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE Interface to platform's teletext engine. 
 * 
 * This interface should be implemented when teletext decoding (for both analog
 * and digital services) will be handled by the platform's teletext engine. Eclipse 
 * will pass the teletext related commands to teletext engine with the help interface.
 * 
 * If it is decided to use TREX (Cabot's teletext engine), then there is no need
 * to implement this interface.
 *
 */
 
#ifndef CABOT_ICE_ICE_TELETEXT_H_INCLUDED
#define CABOT_ICE_ICE_TELETEXT_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_keypress_id.h"
#include "ice/ice_video_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The teletext page structure. Contains magazine, page, and subpage number.
 * 
 * @note See ETSI 300 706, ETSI TR 101 231
 */
typedef struct ICE_TeletextPage
{
    /**
     * Magazine number (0 to 7)
     */
    frost_uint8 magazine;

    /**
     * Page number (0x00 to 0xFF)
     */
    frost_uint8 page;

    /**
     * The high byte of the subpage (0x00 - 0x3F)
     */
    frost_uint8 subcode12;

    /**
     * The low byte of the subpage (0x00 - 0x7F)
     */
    frost_uint8 subcode34;
} ICE_TeletextPage;

/**
 * @brief The Packet 8/30 (PDC) Format 1 data structure.
 * 
 * @note See ETSI 300 706, ETSI TR 101 231
 */
typedef struct ICE_TeletextPacket_8_30_1
{
    /**
     * The initial page number.
     */
    ICE_TeletextPage initial_page;

    /**
     * The network id (16-bits)
     */
    frost_uint16 network_id;

    /**
     * The time-offset.
     */
    frost_uint8 time_offset;           

    /**
     * Modified Julian Date.
     */
    frost_uint32 MJD;               

    /**
     * Universal Time Coordinated. (Hours)
     */
    frost_uint8 UTC_Hours;               

    /**
     * Universal Time Coordinated. (Minutes)
     */
    frost_uint8 UTC_Minutes;               

    /**
     * Universal Time Coordinated. (Seconds)
     */
    frost_uint8 UTC_Seconds;               

    /**
     * Short program label (4 characters)
     */
    frost_uint8 short_prg_label[4]; 

    /**
     * The status display (20 characters)
     */
    frost_uint8 status_display[20];
} ICE_TeletextPacket_8_30_1;

/**
 * @brief The Packet 8/30 (PDC) Format 1 data structure.
 * 
 * @note See ETSI 300 706, ETSI TR 101 231
 */
typedef struct ICE_TeletextPacket_8_30_2
{
    /**
     * The initial page.
     */
    ICE_TeletextPage initial_page;

    /**
     * The country code.
     */
    frost_uint8 country;

    /**
     * The network identification.
     */
    frost_uint8 network_id;

    /**
     * The analog sound type.
     */
    frost_uint8 analog_snd_type;

    /**
     * The announced broadcast time (Day)
     */
    frost_uint8 day;

    /**
     * The announced broadcast time (Month)
     */
    frost_uint8 month;

    /**
     * The announced broadcast time (Hour)
     */
    frost_uint8 hour;

    /**
     * The announced broadcast time (Minute)
     */
    frost_uint8 minute;

    /**
     * The program type.
     */
    frost_uint8 prg_type;

    /**
     * The status display (20 characters)
     */
    frost_uint8 status_display[20];
} ICE_TeletextPacket_8_30_2;

/**
 * @brief VPS (Video Programming System) data structure.
 * 
 * @note See ETSI 300 706, ETSI TR 101 231
 */
typedef struct ICE_TeletextVPSData
{
    /**
     * The sound type of the program.
     */
    frost_uint8 sound; 

    /**
     * The ratings of the program.
     */
    frost_uint8 rating;

    /**
     * The time info (Day)
     */
    frost_uint8 day;

    /**
     * The time info (Month)
     */
    frost_uint8 month;

    /**
     * The time info (Hour)
     */
    frost_uint8 hour;

    /**
     * The time info (Minute)
     */
    frost_uint8 minute;

    /**
     * The nation (country) code.
     */
    frost_uint8 nation_code;

    /**
     * The source info.
     */
    frost_uint8 source;
} ICE_TeletextVPSData;


/**
 * @brief Possible teletext languages.
 */
typedef enum ICE_TeletextLanguage
{
    ice_txt_language_east,
    ice_txt_language_west,
    ice_txt_language_cyrillic,
    ice_txt_language_arabic,
    ice_txt_language_turkish,
    ice_txt_language_greek
}ICE_TeletextLanguage;  

/**
 * @brief Initialization parameters for teletext engine.
 */
typedef struct ICE_TeletextInitParams
{
    /**
    * tuner device to be init.
    */
    frost_int device_number;
    /**
    * cache size is number of page to be cached.
    */
    frost_int cache_size;

}ICE_TeletextInitParams;


/**
 * @brief Open parameters for teletext engine.
 */
typedef struct ICE_TeletextOpenParams
{
    /**
    * tuner device to be open.
    */
    frost_int device_number;

    /**
    * source for teletext data.
    */
    ICE_DeviceSource source;

}ICE_TeletextOpenParams;


/**
* @brief Type of the teletext service (analog, digital, etc.)
*/
typedef enum ICE_TeletextServiceType
{
    /**
    * teletext for analog service.
    */
    ice_teletext_analog_service,
    
    /**
    * teletext for digital service.
    */
    ice_teletext_digital_service
    
}ICE_TeletextServiceType;


/**
* @brief Start parameters for teletext engine.
*/
typedef struct ICE_TeletextStartParams
{
    /**
    * type of teletext service.
    */
    ICE_TeletextServiceType service_type;

    /**
    * pid value for digital teletext. Ignore this parameter for analog teletext.
    */
    frost_int pid;    
    
} ICE_TeletextStartParams;


/**
* @brief Show parameters for teletext engine.
*/
typedef struct ICE_TeletextShowParams
{
    /**
    * The size and position of the teletext screen.
    */
    ICE_DisplayWindow display_window;

    /**
    * To indicate that the displayed page is a teletext subtitle page. 
    */
    frost_bool teletext_subtitle;

    /**
    * Transpareny level for the teletext page. Transparency level can take values 
    * between 0-100 where 0 means full opaque and 100 means full transparent. 
    */
    frost_int transparency_level;
    
} ICE_TeletextShowParams;


/**
* @brief Opaque type to identify a handle to 3rd party teletext engine.
*/
typedef struct ICE_TeletextHandle ICE_TeletextHandle;


/**
 * @brief The types of status messages that can be sent by the ICE implementation
 * to Eclipse.
 */
typedef enum ICE_TeletextStatus 
{
    /**
     * Send when teletext engine retrieves packet 8/30 Format 1.
     */
    ICE_TeletextStatusPacket_8_30_1_Ready,

    /**
     * Send when teletext engine retrieves packet 8/30 Format 2.
     */
    ICE_TeletextStatusPacket_8_30_2_Ready,

    /**
     *  Send when teletext engine retrieves VPS data. 
     */
    ICE_TeletextStatusVPSDataReady,

    /**
     *  Send when teletext engine detects an available teletext subtitle page.
     */
    ICE_TeletextStatusSubtitleFound
    
} ICE_TeletextStatus;

/**
 * @brief Callback function to inform Eclipse of the teletext decoder status.
 *
 * @param user_data As supplied in the call to ICE_TeletextOpen().
 * @param status The teletext decoder status.
 */
typedef void (*ICE_TeletextStatusCallback)(void* user_data, ICE_TeletextStatus status);


/**
 * @brief Intialise platforms teletext engine.
 *
 * @param init_params The initialisation parameters for the teletext engine.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TeletextInit(ICE_TeletextInitParams* init_params);

/**
 * @brief Open platforms teletext engine.
 *
 * @param open_params The open parameters for the teletext engine.
 * @param callback The callback function that will be called when one of the 
 * events defined in #ICE_TeletextStatus has happened.
 * @param user_data User data that will be passed to callback.
 *
 * @return Returns a pointer to the handle to the teletext on success
 *         and a NULL pointer on failure.
 */
ICE_TeletextHandle* ICE_TeletextOpen(ICE_TeletextOpenParams* open_params,
                                          ICE_TeletextStatusCallback callback,
                                          void* user_data);

/**
 * @brief Close platforms teletext engine.
 *
 * @param handle The handle to the required teletext engine.
 *
 * @return None
 */
void ICE_TeletextClose(ICE_TeletextHandle* handle);

/**
 * @brief Reset the teletext engine.
 *
 * @param handle The handle to the required teletext engine.
 *
 * @return None
 */
void ICE_TeletextReset(ICE_TeletextHandle* handle);

/**
 * @brief Start platforms teletext engine for a DVB service.
 *
 * @param handle The handle to the required teletext engine.
 * @param start_params Start parameters for teletext decoding.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TeletextStart(ICE_TeletextHandle* handle, const ICE_TeletextStartParams* start_params);

/**
 * @brief Stop platforms teletext engine for an analog service.
 *
 * @param handle The handle to the required teletext engine.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TeletextStop(ICE_TeletextHandle* handle);

/**
 * @brief Start rendering teletext data to the screen.
 * 
 * If #ICE_TeletextSetPage is called before this function, then the page specified by
 * #ICE_TeletextSetPage will be displayed. Otherwise, teletext index page will be displayed.
 *
 * @param handle The handle to the required teletext engine.
 * @param show_params Properties of the teletext page that will be displayed.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TeletextShow(ICE_TeletextHandle* handle, const ICE_TeletextShowParams *show_params);

/**
 * @brief Stop teletext rendering.
 *
 * @param handle The handle to the required teletext engine.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TeletextHide(ICE_TeletextHandle* handle);

/**
 * @brief Send remote control key press to teletext engine.
 *
 * @param handle The handle to the required teletext engine.
 * @param key Remote control keypress that will be handled by teletext engine.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TeletextSetKey(ICE_TeletextHandle* handle, ICE_KeypressId key);

/**
 * @brief Set teletext page number to the specified value
 *
 * @param handle The handle to the required teletext engine.
 * @param page The page number to select.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TeletextSetPage(ICE_TeletextHandle* handle, ICE_TeletextPage page);

/**
 * @brief Set teletext language to the specified one.
 *
 * @param handle The handle to the required teletext engine.
 * @param language The teletext language to be set.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TeletextSetLanguage(ICE_TeletextHandle* handle, ICE_TeletextLanguage language);

/**
* @brief Get number of the teletext subtitle pages from the teletext engine.
* 
* For some of the services there may be more than one teletext subtitle page. 
* This function allows Eclipse to query number of those pages. 
* 
* @param handle The handle to the required teletext engine.
*
* @return Number of available teletext subtitle pages for selected service.
*/
frost_int ICE_TeletextGetNumberOfAvailableSubtPages(ICE_TeletextHandle* handle);

/**
* @brief Get the page number for the teletext subtitle page specified by the index
* 
* @param handle The handle to the required teletext engine.
* @param index The index value between 0 and number of teletext subtitle pages
* that is returned by #ICE_TeletextGetNumberOfAvailableSubtPages. 
* 
* @return Page number for the teletext subtitle data corresponding to index.
*/
ICE_TeletextPage ICE_TeletextGetSubtPageNumber(ICE_TeletextHandle* handle, frost_int index);

/**
 * @brief Retrieves teletext packet 8/30 (PDC) Format 1 data.
 * 
 * @param handle The handle to the required teletext engine.
 * @param packet_8_30_1 the data structure to be filled.
 *
 * @return #frost_true if data is available, #frost_false otherwise.
 */
frost_bool ICE_TeletextGetPacket_8_30_1(ICE_TeletextHandle* handle, ICE_TeletextPacket_8_30_1* packet_8_30_1);

/**
 * @brief Retrieves teletext packet 8/30 (PDC) Format 2 data.
 * 
 * @param handle The handle to the required teletext engine.
 * @param packet_8_30_2 the data structure to be filled.
 *
 * @return #frost_true if data is available, #frost_false otherwise.
 */
frost_bool ICE_TeletextGetPacket_8_30_2(ICE_TeletextHandle* handle, ICE_TeletextPacket_8_30_2* packet_8_30_2);

/**
 * @brief Retrieves the VPS data.
 * 
 * @param handle The handle to the required teletext engine.
 * @param VPSData the data structure to be filled.
 *
 * @return #frost_true if data is available, #frost_false otherwise.
 */
frost_bool ICE_TeletextGetVPSData(ICE_TeletextHandle* handle, ICE_TeletextVPSData* VPSData);


/**
 * @brief Returns whether teletext is available or not.
 * 
 * @param handle The handle to the required teletext engine.
 *
 * @return #frost_true  teletext is available, #frost_false  teletext is not availale.
 */
frost_bool ICE_TeletextAvailable(ICE_TeletextHandle* handle);

#ifdef __cplusplus
}
#endif

#endif /*CABOT_ICE_ICE_TELETEXT_H_INCLUDED*/
