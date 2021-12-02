
/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface to retrieve dynamic ui data. 
 *
 */

#ifndef CABOT_ICE_ICE_UI_H_INCLUDED
#define CABOT_ICE_ICE_UI_H_INCLUDED

#include "frost/frost_basictypes.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    ICE_UIModuleState_scan_mode = 0,
    ICE_UIModuleState_standby_mode,
    ICE_UIModuleState_nosignal_mode,
    ICE_UIModuleState_password_mode,
    ICE_UIModuleState_oad_search_mode,
    ICE_UIModuleState_web_search_mode,
    ICE_UIModuleState_fti_mode,
    ICE_UIModuleState_CA_mode,
    ICE_UIModuleState_special_msg_mode,
    ICE_UIModuleState_EPG_mode,
    ICE_UIModuleState_EPG_search_mode,
    ICE_UIModuleState_sleep_timer_warning_mode,
    ICE_UIModuleState_media_browser_mode,
    ICE_UIModuleState_service_menu_mode,
    ICE_UIModuleState_hotel_menu_mode,
    ICE_UIModuleState_teletext_mode,
    ICE_UIModuleState_init_mode,
    ICE_UIModuleState_library_mode,
    ICE_UIModuleState_test_mode,
    ICE_UIModuleState_backlight_off_mode,
    ICE_UIModuleState_portal_mode,
    ICE_UIModuleState_pre_portal_mode,
    ICE_UIModuleState_home_portal_mode,
    ICE_UIModuleState_remote_alarm_mode,
    ICE_UIModuleState_diagnostic_menu_mode,
    ICE_UIModuleState_other_mode
} ICE_UIModuleState;

/**
 * @brief Clear all dynamic ui files.
 *
 */
void ICE_UIClearDynamicUIs();

/**
 * @brief Clear only selected dynamic ui files.
 *
 */
void ICE_UIClearSelectedDynamicUI(const char * selected_ui_name);

/**
 * @brief Gets dynamic ui directory path.
 *
 * @param ui_path_string dynamic ui directory path string
 */
void ICE_UIGetDynamicUIPath(char * ui_path_string);

/**
 * @brief Gets default ui directory path.
 *
 * @param ui_path_string default ui directory path string
 */
void ICE_UIGetDefaultUIPath(char * ui_path_string);

/**
 * @brief Copies source to destination.
 *
 * @param src source file
 * @param dst destination file
 * @return #frost_true on success, #frost_false on failure
 */
frost_bool ICE_UICopyFile(char * src, char * dst);

/**
 * @brief Set UI module state for informational purposes.
 *
 * @param module_state Module state of UI
 */
void ICE_UISetModuleState(ICE_UIModuleState module_state);

#define DYNAMIC_UI_PATH "/basic/user_dynamic_ui"
#define DEFAULT_UI_PATH "/basic/dynamic_ui"


#ifdef __cplusplus
}
#endif

#endif /*CABOT_ICE_ICE_UI_H_INCLUDED*/

