/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 *
 * @brief Interface for creating test menu.
 *
 */

#ifndef CABOT_ICE_ICE_TEST_MENU_H_INCLUDED
#define CABOT_ICE_ICE_TEST_MENU_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_keypress_id.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief An action call-back for menu item.
 *
 * Each menu item may be associated with the callback, 
 * that performs the required action.
 *
 * @param user_data pointer stored in the menu item.
 *
 */
typedef void (*ICE_TestMenuItemAction)(frost_uint32 item_index, ICE_KeypressId, void * user_data);

/**
 * @brief Actions for controlling the test menu.
 * @see ICE_TestMenuExecute.
 */
typedef enum ICE_TestMenuAction
{
    /**
     * @brief Update a menu item.
     *
     * This is used to tell Eclipse that a menu item value has changed and needs
     * updating, e.g. by re-displaying the value.
     *
     * The item_index in the ICE_TestMenuActionData must be set.
     */
    ICE_test_menu_item_update,

    /**
     * @brief Show the test menu.
     */
    ICE_test_menu_show,

    /**
     * @brief Hide the test menu.
     */
    ICE_test_menu_hide,

    /**
     * @brief Action currently-selected item.
     */
    ICE_test_menu_action,

    /**
     * @brief Trigger a timer.
     */
    ICE_test_menu_timer

} ICE_TestMenuAction;

/**
 * @brief Additional data that may be needed to execute an action.
 */
typedef struct ICE_TestMenuActionData
{
    /**
     * @brief Zero-based index of an item in the menu.
     */
    frost_uint32 item_index;

    /**
     * @brief Parameter association with action.
     */
    frost_uint32 param;

} ICE_TestMenuActionData;

/**
 * @brief An action callback for the test menu.
 *
 * This is used to tell Eclipse to perform an action on the test menu.
 *
 * @param action Indicates the action to perform.
 * @param action_data Data required to perform the action.
 * @param user_data Value passed via ICE_TestMenuOpenParams.
 */
typedef void (*ICE_TestMenuExecute)(ICE_TestMenuAction action,
                                    ICE_TestMenuActionData const * action_data,
                                    void * user_data);

/**
 * @brief Parameters passed to ICE_TestMenuOpen.
 */
typedef struct ICE_TestMenuOpenParams
{
    /**
     * @brief A callback for the ICE layer to control the presentation of the
     *        test menu.
     */
    ICE_TestMenuExecute    execute;

    /**
     * @brief Must be passed to callbacks.
     */
    void *                 user_data;

} ICE_TestMenuOpenParams;

/**
 * @brief ICE_KeyVerifier indicates whether a key can be sent to the ICE Test
 * Menu implementation.
 *
 * @param key the ID of the key selected by the user.
 *
 * @return true if the key can be passed to the ICE Test Meny implementation.
 */
typedef frost_bool (*ICE_KeyVerifier)(ICE_KeypressId key);

/**
 * @brief A single item within test menu.
 */
typedef struct ICE_TestMenuItem {
    /**
     * @brief Item name
     *
     * The name is displayed as a text label on the left side of the 
     * test menu. The pointed-to string must be UTF-8 encoded.
     * It's valid to use a NULL pointer but it will result in an
     * empty label. The name is read once when the test menu is created. 
     */
    const char * name;

    /**
     * @brief Value of the item.
     *
     * Value is displayed as a selectable button next to the name.
     * The pointed-to string must be UTF-8 encoded.
     * It's valid to use a NULL pointer but it will result in an
     * empty button. 
     *
     * Pointer or pointed-to data may change as a result of executing
     * an action. It's re-read at the initialisation and each time 
     * after an action is executed.
     */
    const char * value;

    /**
     * @brief Action performed by the menu item
     *
     * This callback is called each time user presses SELECT (OK)
     * button on the item. A 0-based item index and user_data pointer
     * is passed to the callback when called.
     *
     * If action is NULL an item is disabled (grayed out). This feature
     * can be used for displaying static information, for example device
     * model or software version number.
     */
    ICE_TestMenuItemAction action;

    /**
     * @brief Pointer to implementation specific data.
     *
     * It's intended to attach implementation specific data to any menu
     * item, if needed.Eclipse doesn't use the data in any way. 
     * The pointer is being passed to the action callback each time the 
     * action is executed.
     */
    void * user_data;

    /**
     * @brief Key verifier.
     *
     * Before passing a key to the ICE Test Menu implementation, it is intended
     * that this function shall be invoked to check whether the key is
     * permitted.
     */
    ICE_KeyVerifier key_verifier;
} ICE_TestMenuItem;

/**
 * @brief Test menu structure
 *
 * The test menu consist of a title and a list of 
 * ICE_TestMenuItem structures.
 */
typedef struct ICE_TestMenu {
    /**
     * @brief Title of the test menu.
     *
     * The title is an UTF-8 string to be displayed as a title 
     * of the test menu.
     */
    const char * title;

    /**
     * @brief Implementation specific data.
     *
     * The implemnation may which to attach some data to the menu structure.
     * The data is not used by Eclpise.
     */
    void * user_data;

    /**
     * @brief Number of items in the test menu.
     *
     * The number of items must not change while menu is open.
     */
    frost_uint32 num_items;

    /**
     * @brief Array of the test menu items.
     *
     * The pointer must not be NULL and must point to an array of
     * num_items menu items.
     */
    ICE_TestMenuItem const * items;
} ICE_TestMenu;

/**
 * @brief Test menu entry PIN.
 *
 * The PIN used to enter the test menu. The test menu is accessible
 * only from "First Time Installation" screen and PIN is not 
 * advertised in any way in order to minimise accidental access to the
 * test menu by the end users.
 *
 * User has to press sequence of numeric keys on the remote controll unit 
 * when the "First Time Installation" dialog is displayed.
 * For example sequence "1", "2", "3" is encoded as decimal integer 123.
 *
 * PINs with leading zeros are unsupported.
 *
 * @return PIN code encided as decimal integer or 0 to disable access
 * to test menu from "First Time Installation" screen.
 */
frost_uint32 ICE_TestMenuPin(void);

/**
 * @brief Open the test menu
 *
 * This is called when user enters valid PIN number in the "First Time
 * Installation" screen in order to show the test menu.
 *
 * @param params Configuration information for the test menu.
 * @return pointer to the test menu structure describing contents
 * and behaviour of the test menu or NULL if test menu is not available.
 */
ICE_TestMenu const * ICE_TestMenuOpen(ICE_TestMenuOpenParams const * params);

/**
 * @brief Close test menu.
 *
 * It's called after pressing MENU key on the remote from the test menu
 * dialog.
 *
 * @param menu pointer to the ICE_TestMenu structure 
 * returned by ICE_TestMenuOpen.
 *
 * @return false in order to continue with the first time installation,
 *         or true to clear NV memory and reset the device after exiting
 *         from the test menu.
 *
 */
frost_bool ICE_TestMenuClose(ICE_TestMenu const * menu);


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_TEST_MENU_H_INCLUDED */

