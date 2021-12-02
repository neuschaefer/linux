/**
 * Copyright 2005-6 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief   ICE Smart Card interface
 *
 * This file defines the ICE interfaces to a Smart Card slot and an inserted
 * smart card device conforming to ISO7816 standard.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code. This interface is only required for systems
 * that include an embedded CA stack. If an embedded CA stack is not present,
 * this interface need not be implemented.
 */

#ifndef CABOT_ICE_ICE_SMARTCARD_H_INCLUDED
#define CABOT_ICE_ICE_SMARTCARD_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C"
{
#endif


/***************************************************************************/
/**
 * @name Type Definitions
 * @{
 */
/***************************************************************************/

/**
 * @brief Opaque type used to identify an open connection to a smart card slot.
 */
typedef struct ICE_SmartCardHandle ICE_SmartCardHandle;


/**
 * @brief Enumeration of possible Smart Card states.
 */
typedef enum ICE_SmartCardState
{
    /**
     * Smart Card is inserted
     */
    ICE_smart_card_state_inserted,

    /**
     * Smart Card is not inserted
     */
    ICE_smart_card_state_removed

} ICE_SmartCardState;


/**
 * @brief Smart Card device open parameters.
 */
typedef struct ICE_SmartCardOpenParams
{
    /**
     * The number of the Smart Card slot.
     */
    frost_uint slot_number;

} ICE_SmartCardOpenParams;


/**
 * @brief Smart Card's Answer to Reset.
 */
typedef struct ICE_SmartCardAnswerToReset
{
    /**
     * ATR (Answer to Reset) string
     */
    frost_uint8 * string;

    /**
     * ATR string length
     */
    frost_uint32 length;

} ICE_SmartCardAnswerToReset;


/**
 * @brief Smart Card's Operating Parameters.
 */
typedef union ICE_SmartCardParams
{
    struct
    {
        frost_uint  current_frequency;
    }params;
    frost_uint  uint_array[16];
}ICE_SmartCardParams;


/**
 * Callback function to inform Eclipse of smart card state changes
 *
 * @param user_data As supplied in the call to #ICE_SmartCardOpen().
 * @param state The new state that the smart card is transitioning into.
 */
typedef void (*IceSmartCardStateChangedCallbackFunction)(void * user_data,
                                                         ICE_SmartCardState state);


/**
 * Callback function to inform Eclipse of the completion of a transfer request.
 *
 * @param user_data As supplied in the call to #ICE_SmartCardTransfer().
 * @param number_of_bytes_read The size of the Response APDU data that was read
 *        from the smart card (including the status words). Set to 0 if transfer failed.
 */
typedef void (*IceSmartCardTransferCompletedCallbackFunction)(void * user_data,
                                                              frost_uint number_of_bytes_read);


/**
 * Callback function to inform Eclipse of the completion of a reset request.
 *
 * @param user_data As supplied in the call to #ICE_SmartCardReset().
 * @param atr Pointer to Answer to Reset sent by the smart card, O if reset failed.
 *
 * @param status_bytes Status word representing the command processing status
 *        and qualifier returned by the smart card. Not required if success
 *        is #frost_false.
 */
typedef void (*IceSmartCardResetCompletedCallbackFunction)(void * user_data,
                                                           ICE_SmartCardAnswerToReset const * atr);

/***************************************************************************/
/**
 * @}
 * @name Smart Card Functions
 * @{
 */
/***************************************************************************/

/**
 * @brief Initialise the Smart Card slot(s).
 *
 * Initialise the interface to the Smart Card slot(s).
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_SmartCardInit(void);


/**
 * @brief Open a connection to a Smart Card slot.
 *
 * Attempts to open a connection to a smart card slot and return a handle
 * for subsequent communication with the device.
 *
 * @note A Smart Card does not have to be present in the slot for this function
 *       to succeed. If a Smart Card is present, this function should NOT reset
 *       the Smart Card, as that will be performed by Eclipse afterwards.
 *
 * @note During initialization, this function will be called successively,
 *       each time with a larger slot number. When the requested index is larger
 *       than the index for the last smart card slot, this function should return NULL.
 *
 * @param params The Open parameters indicate the number for the slot that should be
 *               opened with this callback.
 * @param user_data Context to be maintained and passed back through the
 *                  callback function.
 * @param callback Function to notify Eclipse that the smart card state has changed.
 *
 * @return Returns a pointer to the handle to the Smart Card device on success
 *         and a NULL pointer on failure.
 *
 * @note The /callback function should be called asynchronously, i.e. it should never
 *       be called from inside this function.
 */
ICE_SmartCardHandle * ICE_SmartCardOpen(const ICE_SmartCardOpenParams * params,
                                        void * user_data,
                                        IceSmartCardStateChangedCallbackFunction callback);


/**
 * @brief Close a connection to a Smart Card slot.
 *
 * Closes a previously Opened connection to the Smart Card slot identified.
 *
 * @param handle The handle to the required Smart Card slot.
 */
void ICE_SmartCardClose(ICE_SmartCardHandle * handle);


/**
 * @brief Write a command to a smart card, and read back the smart card's response.
 *
 * Write a Command APDU (see section 5.3.1 of ISO 7816-4) to the required
 * Smart Card and read back the Response APDU (see section 5.3.3 of ISO 7816-4) sent
 * by the Smart Card into the provided buffer.
 *
 * @param handle The handle to the required Smart Card device.
 * @param command_apdu Pointer to the Command APDU data to write.
 * @param command_apdu_length Length in bytes of data to be written.
 * @param response_apdu_buffer A pre-allocated buffer into which the Response APDU
 *        message bytes should be written (including the status words).
 * @param response_apdu_buffer_size Length in bytes of the read buffer, which matches
 *        the length of the expected response.
 * @param user_data Context to be maintained and passed back through the
 *        callback function.
 * @param callback Function to notify Eclipse that the write operation has been
 *        completed.
 *
 * @return #frost_true on success, #frost_false on failure (e.g. when a smart
 *         card is not present in the slot).
 *
 * @note This function starts an asynchronous operation whose result will be
 *       sent to Eclipse via a IceSmartCardTransferCompletedCallbackFunction call.
 *       A return value of #frost_true only indicates that the request has been
 *       successfully queued and not that the operation has been completed.
 *
 * @note If this function returns frost_true, /callback must always be called
 *       to notify Eclipse about the result of the operation. If this function
 *       returns frost_false, /callback should not be called. Eclipse will never
 *       make a second ICE_SmartCardTransfer call until the result of the first call
 *       is received.
 *
 * @note The /callback function should be called asynchronously, i.e. it should never
 *       be called from inside this function.
 */
frost_bool ICE_SmartCardTransfer(ICE_SmartCardHandle * handle,
                                 const frost_uint8 * command_apdu,
                                 frost_uint command_apdu_length,
                                 frost_uint8 * response_apdu_buffer,
                                 frost_uint response_apdu_buffer_size,
                                 void * user_data,
                                 IceSmartCardTransferCompletedCallbackFunction callback);

/**
 * @brief Reset the Smart Card device.
 *
 * Performs a hardware reset on the Smart Card device in the identified slot,
 * in accordance with ISO 7816-3 Section 3.2.
 *
 * @param handle The handle to the required Smart Card slot.
 * @param user_data Context to be maintained and passed back through
 *        callback function.
 * @param callback Function to notify Eclipse that the reset operation has been
 *        completed.
 *
 * @note This function starts an asynchronous reset operation whose result will be
 *       sent to Eclipse via a IceSmartCardResetCompletedCallbackFunction call.
 *       A return value of #frost_true only indicates that the reset request has been
 *       successfully queued and not that the reset operation has been completed.
 *
 * @return #frost_true on success, #frost_false on failure (e.g. when a smart
 *         card is not present in the slot).
 *
 * @note The /callback function should be called asynchronously, i.e. it should never
 *       be called from inside this function.
 *
 * @note When the reset is successful, and if baud rate negotiation (PPS/PTS) is required
 *       for the platform (PPS/PTS), it should be done before directly before calling
 *       /callback.
 */
frost_bool ICE_SmartCardReset(ICE_SmartCardHandle * handle,
                              void * user_data,
                              IceSmartCardResetCompletedCallbackFunction callback);



/**
 * @brief Get Smart Card Parameters.
 * Return frost_false if not supported
 */
frost_bool ICE_SmartCardGetParams(ICE_SmartCardHandle * handle,
                              ICE_SmartCardParams * params);



/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_SMARTCARD_H_INCLUDED*/
