#ifndef CABOT_ICE_ICE_HARDWARE_CONFIG_H_INCLUDED
#define CABOT_ICE_ICE_HARDWARE_CONFIG_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Version number of the hardware configuration record.
   Note: Consider incrementing it when the layout of the record changes. */
#define HARDWARE_CONFIGURATION_VERSION 0x01

typedef struct PersistentHardwareConfigurationRecord
{
    frost_int8 version; 
    
    frost_int8 m_front_type;
    frost_int8 m_remote_control;
    frost_int8 m_oad_available;
    frost_int8 m_usb_available;
    frost_int8 m_ignore_epg_patents;
    frost_int8 m_low_power_option_available;
    frost_int8 m_video_playback_available;
    frost_int8 m_favourites_option_available;
    frost_int8 m_dvr_available;
    frost_int8 m_background_search_available;
    char       m_pin[4];
} PersistentHardwareConfigurationRecord;

frost_bool ICE_LoadHardwareConfig(PersistentHardwareConfigurationRecord * hw_config);
frost_bool ICE_StoreHardwareConfig(PersistentHardwareConfigurationRecord const * hw_config);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_HARDWARE_CONFIG_H_INCLUDED */


