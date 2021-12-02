/**
 * Copyright 2008 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface to retrieve CEC data.
 *
 * CEC is a communications protocol for HDMI - for instance, an Xbox will use
 * it to query what resolutions a TV can support.
 */

#ifndef CABOT_ICE_ICE_CEC_H_INCLUDED
#define CABOT_ICE_ICE_CEC_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

 /*
  * Eclipse expects only to process complete messages that
  * consist of header + opcode + optional data[0..nn]
  */


/**
 * @brief This callback function will be used to inform the middleware
 *        when a data has been recieve at an HDMI connector. A callback
 *        may be in an ISR, a 1K stack size is recommended.
 *
 * @param client Data to be passed back when the callback is raised.
 * @param size   Number of bytes of data.
 * @param data   Pointer to data another device has transmitted, the
 *               data must remind valid for the duration of the ISR.
 */
typedef void (*ICE_CECRecieveCallback)(void* client, frost_uint16 size,  frost_uint8* data);

/**
 * @brief Opaque type used to identify an open connection to CEC module.
 */
typedef struct ICE_CECHandle ICE_CECHandle;

/**
 * @brief Init paramaters structure which will be passed to the Init function
 *        during initialization.
 */
typedef struct ICE_CECInitParams
{
    /**
     * Identifies this devices preferred logic CEC device number.
     * Currently always 0 as middleware only supports a TV mode.
     */
    frost_uint8 logical_number;

}ICE_CECInitParams;

/**
 * @brief Open paramaters structure which will be passed to the Open function
 *        during opening CEC object.
 */
typedef struct ICE_CECOpenParams
{
    /**
     * client Data to be passed back when the callback is raised.
     */
    void* client;

    /**
     * recieve_callback The function that processes data when recieved.
     */
    ICE_CECRecieveCallback   recieve_callback;

}ICE_CECOpenParams;

/**
 * @brief Enumeration for CEC status events. Status events will be passed to the ICE_CECStatusCallback
 *        function when the callback is raised.
 */
typedef enum ICE_CECStatus
{
    /**
     * CEC status event that will be raises when a CEC message is received.
     */
    ICE_CECDataReceived,

    /**
     * CEC status event that will be raises when a CEC message is received.
     */
    ICE_CECDataSent,
}ICE_CECStatus;

/**
 * @brief The callback function that will be used to inform middleware when
 *        the status of the CEC changed.
 *
 * @param user_data Data to be passed back when the callback is raised.
 * @param status Type of the CEC status event.
 */
typedef void (*ICE_CECStatusCallback)(void* user_data, ICE_CECStatus status);

/**
 * @brief Maximum number of operands for CEC
 */
#define ICE_CEC_MAX_NUMBER_OF_OPERANDS 15

/**
 * @brief Structure to represent a CEC messge.
 */
typedef struct ICE_CECParams
{
    /**
     * The opcode of the CEC command.
     */
    frost_uint8   Opcode;

    /**
     * The number of operands of the CEC command.
     */
    frost_uint8   NumberOfOperands;

    /**
     * The operands of the CEC command. See #ICE_CEC_MAX_NUMBER_OF_OPERANDS
     */
    frost_uint8   Operands[ICE_CEC_MAX_NUMBER_OF_OPERANDS];

    /**
     * The ID of the device which is the initiator of the CEC command.
     */
    frost_uint8   InitiatorDeviceID;

    /**
     * The ID of the device which is the destination of the CEC command.
     */
    frost_uint8   DestinationDeviceID;
} ICE_CECParams;

/**
 * @brief Initialise CEC module.
 *
 * @param params Initialisation parameters.
 *
 * @return #frost_true on success, #frost_false otherwise.
 */
frost_bool ICE_CECInit(ICE_CECInitParams* params);

/**
 * @brief Open a connection CEC module.
 *
 * @param open_params Open parameters.

 * @return Returns a pointer to the handle to the device on success,
 *         and a NULL pointer on failure..
 */
ICE_CECHandle* ICE_CECOpen(ICE_CECOpenParams* open_params);


/**
 * @brief Close a connection CEC module.
 *        Middleware is shuting down.
 */
void ICE_CECClose(ICE_CECHandle* handle);


/**
 * @brief Send CEC data to another device.
 *
 * @param handle   The handle to CEC module from which the data will be sent.
 * @param size     Number of bytes of data.
 * @param data     CEC data that will be sent. The data is valid for the
 *                 duration of the function.
 *
 * @note cec_data will always contain header, opcode and length bytes.
 *       If the length byte is zero then it is not transmitted.
 *
 * @return #frost_true on success, #frost_false otherwise.
 */
frost_bool ICE_CECSendData(ICE_CECHandle* handle, frost_uint16 size, frost_uint8* data);

/**
 * @brief This is a pointer to the cec vendor device identifier.
 * The first byte is the length of the identifer.
 * e.g. ICE_CecDeviceId = { 3, 0x00, 0x80, 0x45 };
 */
extern const frost_uint8* const ICE_CecVendorId;

#ifdef __cplusplus
}
#endif

#endif /*CABOT_ICE_ICE_CEC_H_INCLUDED*/

