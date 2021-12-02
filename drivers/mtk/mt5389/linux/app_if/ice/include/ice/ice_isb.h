/**
 * Copyright 2010 Vestel/RT-RK.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for 2M ISB communication
 *
 * This file defines the ICE interfaces to support M2 ISB protocol
 * All functions in this interface are
 * implemented in STB-specific code and called by Cabot code.
 */


#ifndef CABOT_ICE_ICE_ISB_H_INCLUDED
#define CABOT_ICE_ICE_ISB_H_INCLUDED

#include "ice/ice_isb_types.h"
#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Update TV status structure used to generate TV status message
 *
 * @return  frost_true
 *
 */
frost_bool ICE_ISBUpdateTVStatusField( ICE_ISBHandle* handle, ICEISB_change_status_field status_field, frost_int32 status_field_value );

frost_bool ICE_ISBUpdateCompleteTVStatus( ICE_ISBHandle* handle, ICEISB_tv_status_t* new_tv_status);

frost_bool ICE_ISBGetTVStatus( ICE_ISBHandle* handle, ICEISB_tv_status_t* tv_status );

ICEISB_connection_state ICE_ISBGetConnectionState(ICE_ISBHandle* handle);

/**
 * @brief Initialise the ICE ISB component.
 *
 * This function must be called prior to ICE_ISBOpen() function.
 * If called again the call is silently ignored.
 *
 * @return  frost_true     if initialisation is successful or if the ISB is already initialised,
 *              frost_false    if initialisation is not successfull
 *
 */ 
frost_bool ICE_ISBInit();

/**
 * @brief Open a connection to ISB device
 *
 * This function must be called prior to any calls to any other ICE ISB functions.
 * If called again the call is silently ignored.
 *
 * @return a pointer to the handle to the device on succes,
 *             or NULL on failure
 *
 */
ICE_ISBHandle* ICE_ISBOpen();

/**
 * @brief Closes the connection to the ISB device and releases any resources.
 *
 * @param handle    The handle to the ISB connection to be closed.
 * @return Returns   #frost_true on success, #frost_false on failure
 *
 */
frost_bool ICE_ISBClose(ICE_ISBHandle* handle);

/**
 * @brief Registers callback function to be called when command is received from ISB device
 *
 * @param handle                      The handle to the ISB component.
 * @param callback_function        function to be called
 * @param user_data   Interface file for ISB functionality 
 *
 */
void ICE_ISBRegisterReceivedCommandCallback
    ( ICE_ISBHandle* handle, ICEISB_ReceivedCmdCallbackFunction callback_function, void* user_data );

/**
 * @brief Registers callback function to be called when connection state changes
 *
 * @param handle                       The handle to the ISB component.
 * @param callback_function        function to be called
 * @param user_data   Interface file for ISB functionality 
 *
 */
void ICE_ISBRegisterConnectionStateCallback
    ( ICE_ISBHandle* handle, ICEISB_ConnectionStateCallbackFunction callback_function, void* user_data );


/**
 * @brief Creates and sends a message to ISB device as a response from Middleware
 *
 * @param handle                      The handle to the ISB component.
 * @param struct_with_options    struct holding options, used for creating ISB message
 * @param final_response Defines if it is final response or not
 *
 * @return frost_true if ISB component is open, else frost_false
 */
frost_bool ICE_ISBResponse( ICE_ISBHandle* handle, void* struct_with_options, frost_bool final_response );

frost_bool ICE_ISBIsOpened();



#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_ISB_H_INCLUDED*/
