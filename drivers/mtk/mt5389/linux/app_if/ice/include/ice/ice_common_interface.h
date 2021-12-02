/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file
 *
 * @brief Cabot portable interface to DVB Common Interface (CI) functionality.
 */

#ifndef CABOT_ICE_ICE_COMMON_INTERFACE_H_INCLUDED
#define CABOT_ICE_ICE_COMMON_INTERFACE_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Values used by <code>ICE_CICommand</code> */
typedef enum ICE_CICmd
{
     /** Enable inturrpts,
       * these bits can be added to the HostControl or None command
       */
    ICE_CICmd_Ints        = 0xC0,

    /** Reset the Card */
    ICE_CICmd_Reset       = 0x08,

    /** This command follows the reset command; requests the card maximum buffer size */
    ICE_CICmd_SizeRead    = 0x04,

    /** This command follows the size read command; sets the transfer buffer size */
    ICE_CICmd_SizeWrite   = 0x02,

    /** This command follows the size write or none command
      * It indicates that the host has data to send
      */
    ICE_CICmd_HostControl = 0x01,

    /** This command follows host control command */
    ICE_CICmd_None        = 0x00

} ICE_CICmd;

/**
 * The types of status messages that can be sent by the ICE implementation
 * to Eclipse.
 */
typedef enum ICE_CIStatus {
    /**
     * Send when an interrupt is received from the CI module. This is generally
     * used to indicate that either data is available to be read from a module
     * or the module is free to receive data.
     */
    ICE_CIStatusInterrupt = 0x01

} ICE_CIStatus;

/**
 * @brief Capabilities of a Common Interface slot
 */
typedef struct ICE_CICapability
{
    /**
     * The number of entries in the #data_rates array.
     */
    frost_uint num_bus_data_rates;

    /**
     * Array of supported bus data rates.
     *
     * Contains #num_bus_data_rates entries.
     *
     * Each data rate is in kilobits per seconds (kbps) e.g a data rate
     * of 72Mbps would be 72000.
     */
    const frost_uint* data_rates;

} ICE_CICapability;


/**
 * @brief Module type returned by ICE_CIModuleInitialise
 *
 * The Module type can be determined by the presence (or lack of) CI+
 * compatibility data in the Module's CIS structure (see CI+ Specification V1.2
 * (2009-04), Annex G.3.
 *
 * The CI+ stack will communicate differently with the inserted Module
 * depending on the value used.
 */
typedef enum ICE_CIModuleType
{
    /**
     * @brief Non-CI Module
     *
     * The CI+ stack will not communicate with the inserted Module.
     */
    ice_ci_module_type_unknown,

    /**
     * @brief Old CI Module - may not tolerate CI+ resource enquiries.
     *
     * The CI+ stack will not advertise the existence of CI+ resources to the
     * inserted Module and will prevent it from opening them.
     */
    ice_ci_module_type_ci,

    /**
     * @brief Old CI Module which tolerates CI+ resource enquiries.
     *
     * The CI+ stack will advertise the existence of CI+ resources to the
     * inserted module but will prevent it from opening them.
     */
    ice_ci_module_type_ci_ciplus_compliant,

    /**
     * @brief CI+ compliant module
     *
     * The CI+ stack will advertise the existence of CI+ resources to the
     * inserted module and allow it to open them.
     */
    ice_ci_module_type_ciplus

} ICE_CIModuleType;


/**
 * @brief Callback function to inform Eclipse of CI status changes.
 *
 * See the comments for ICE_CIStatus for when this callback should be called.
 *
 * This callback must not be called from a interrupt handling thread.
 *
 * @param user_data As supplied in the call to ICE_CIOpen().
 * @param status The CI status change.
 */
typedef void (*ICE_CIStatusCallback)(void* user_data, ICE_CIStatus status);

/**
 * Opaque type used to identify a CI device.
 */
typedef struct ICE_CIHandle ICE_CIHandle;

/**
 * @brief Initialise the CI system.
 *
 * @return #frost_true on success, #frost_false otherwise.
 */
frost_bool ICE_CIInit(void);

/**
 * @brief Obtain a handle to a PC Card slot, for the purposes of the CI
 * interface.
 *
 * @param slot_number slot number of the PC Card interface
 * @param callback_func The callback function to be called for status changes.
 * @param user_data Pointer to data that should be used as the parameter to
 *        <code>callback_func</code> when the callback is raised.
 *
 * @return Returns a pointer to the handle to the device on success and a NULL
 * pointer on failure.
 */
ICE_CIHandle * ICE_CIOpen(ICE_DeviceNumber slot_number,
                          ICE_CIStatusCallback callback_func,
                          void* user_data);

/**
 * @brief Release the handle to a previously opened PC Card slot.
 *
 * @param handle The handle to the required CI device.
 */
void ICE_CIClose(ICE_CIHandle * handle);

/**
 * @brief Determine whether there is any module present in a slot.
 *
 * @note This is just a test of physical insertion. There is no interaction
 * with the module expected.
 *
 * @param handle The handle to the required CI device.
 *
 * @return #frost_true if there is a CI Card present, #frost_false otherwise.
 *
 * @note This gets polled several times a second, and should not block.  It is
 * expected that a typical implementation will do actual CI card
 * insertion-detection and initialisation in a separate thread, and this
 * function will simply return a status flag.
 */
frost_bool ICE_CICardPresent(ICE_CIHandle * handle);

frost_bool    ICE_CIGetEmergencyStatus(void);

/**
 * @brief Read a message from a CI Card.
 *
 * This function implements the Module to Host Transfer documented in Appendix
 * A.2.2.1.3 of EN 50221, including setting the command register and checking
 * the status register as appropriate.
 *
 * @note The CI stack may make calls to set the command register and check the
 * status register at other times, for example during initialisation, but it
 * does not do so for normal message transfers.
 *
 * @param handle The handle to the required CI device.
 *
 * @param buffer A pre-allocated buffer into which the message bytes should be
 * written.
 *
 * @param buffer_size The size of \a buffer.  This is guaranteed to
 * be large enough for any single message, according to the buffer size
 * negotiation protocol, but implementations should ensure that the buffer
 * does not overflow even if a module erroneously sends an over-long message.
 *
 * @return The number of bytes actually read.  This should be 0 if any error
 * occured.
 *
 * @note If a module erroneously sends a message longer than \a buffer_size,
 * the implementation MUST NOT overflow \a buffer, and is recommended to
 * return 0.
 */
frost_uint ICE_CIRead(ICE_CIHandle * handle,
                      frost_uint8 * buffer,
                      frost_uint buffer_size);

/**
 * @brief Write a message to a CI Card.
 *
 * This function implements the Host to Module Transfer documented in Appendix
 * A.2.2.1.2 of EN 50221, including setting the command register and checking
 * the status register as appropriate.  If the function determines that the
 * interface is busy it should return #frost_false -- the CI stack will try
 * again later.
 *
 * @note The CI stack may make calls to set the command register and check the
 * status register at other times, for example during initialisation, but it
 * does not do so for normal message transfers.
 *
 * @param handle The handle to the required CI device.
 *
 * @param message_bytes Pointer to the data to write.
 *
 * @param number_of_bytes_to_write The number of bytes to write to the CI
 * module.
 *
 * @return #frost_true on success, #frost_false otherwise.
 */
frost_bool ICE_CIWrite(ICE_CIHandle * handle,
                       const frost_uint8 * message_bytes,
                       frost_uint number_of_bytes_to_write);

/**
 * @brief Read the status register from a CI Card.
 *
 * @param handle The handle to the required CI device.
 *
 * @return the value of the status register.
 */
frost_uint8 ICE_CIGetStatus(ICE_CIHandle * handle);

/**
 * @brief Set/clear bits in the command register on the CI Card.
 *
 * @param handle The handle to the required CI device.
 *
 * @param bit_mask Identifies the bits in the command register to set or
 * clear.
 *
 * @param value For each bit set in \a bit_mask, the corresponding bit in this
 * field determines whether the command register bit is set or cleared.
 */
void ICE_CISetCommand(ICE_CIHandle * handle,
                      frost_uint8 bit_mask,
                      frost_uint8 value);

/**
 * @brief Get whether a CI CAM supports interrupts
 *
 * @param handle The handle to the required CI device.
 *
 * @param supported Set by this function to #frost_true if interrupts are
 * supported by the CAM, set to #frost_false if interrupts are not supported.
 *
 * @return #frost_true if the call was successful, #frost_false otherwise.
 */
frost_bool ICE_CISupportsInterrupts(ICE_CIHandle * handle,
                                    frost_bool* supported);

/**
 * @brief Get the supported capabilities of the CI slot.
 *
 * @param handle The handle to the required CI device.
 *
 * @return The capabilities of the CI slot on success, or NULL on error.
 *         The returned pointer is owned by the ICE implementation. It must
 *         not be freed or modified before ICE_CIClose() is called on the
 *         handle.
 *
 * @note These are capabilities of the slot, independent of any CAM that
 *       may or may not be inserted.
 */
ICE_CICapability* ICE_CIGetCapability(ICE_CIHandle * handle);

/**
 * @brief Sets the data rate the CI device bus.
 *
 * Only data rates that were received by calling #ICE_CIGetCapability will be
 * passed to this function.
 *
 * @param handle The handle to the required CI device.
 * @param data_rate The data rate to set the bus to in kilobits per second
 * (kbps) e.g. 72Mbps would be 72000.
 *
 * @return #frost_true if data rate has been set, otherwise #frost_false.
 */
frost_bool ICE_CISetBusDataRate(ICE_CIHandle * handle, frost_uint data_rate);

frost_uint16 ICE_CIGetBufferSize(ICE_CIHandle *handle);


/**
 * @brief Reset the slot according to the PC Card Standard.
 *
 * This should perform a reset of the slot, by setting and then clearing the
 * PCMCIA RESET pin (see PC Card Standard).
 *
 * @param handle The handle to the required CI device (slot).
 */
void ICE_CIReset(ICE_CIHandle * handle);

/**
 * @brief Initialise a Module according to the PC Card Standard.
 *
 * The specified slot interface is initialised according to the PC Card
 * Standard (i.e. The Module's CIS information is read and the COR data is
 * written back to the Module)
 *
 * @param handle The handle to the required CI device (slot).
 * @param module_type pointer to the type of module found during initialisation
 *
 * @return #frost_true if initialise succeeds, otherwise #frost_false.
 */
frost_bool ICE_CIModuleInitialise(ICE_CIHandle * handle,
                                  ICE_CIModuleType * module_type);

/**
 * @brief Whether the transport stream is routed through the CAM.
 *
 * @param handle The handle to the required CI device (slot).
 *
 * @param status If #frost_true, turns the bypass ON, which means the CAM does
 * NOT see the transport stream (and therefore descrambling CAN NOT be done).
 * If #frost_false, the bypass is OFF, so the transport stream IS routed
 * through the CAM.
 *
 * @return the result of reading the status of the bypass back from the
 * device, after setting it.
 */
frost_bool ICE_CIBypass(ICE_CIHandle * handle, frost_bool status);


/**
 * @brief Configure the tuner device output to go via the CI slot
 *
 * This function should connect the output of the specified tuner device
 * to the CI slot.
 *
 * @param source The source of the transport stream, e.g. tuner 0, tuner 1
 *
 * @note For single tuner platforms or twin CI slots, this function may be ignored.
 */
void ICE_CISelectSource(ICE_DeviceSource source);

/**
 * @brief Configure the tuner device output to stop using the CI slot
 *
 * This function should disconnect the current tuner device's transport stream
 * from the CI slot.
 *
 * @note For single tuner platforms or twin CI slots, this function may be ignored.
 */
void ICE_CIDisconnectSource(void);

/**
 * @brief This function returns the fixed Canal Ready protocol response data in response to a Canal Ready CAM sending an Authentication object
 * with auth_req and auth_protocol_id 0x0001. See EN50221 Section B.1 .
 *
 * @param response A pre-allocated buffer to hold the 356 byte protocol_version_resp (returned in an auth_resp) into which the bytes should be
 * written.
 *
 */

void ICE_CICanalReadyProtocolVersionResponse(frost_uint8 *response);

/** Added to fix linker error on trunk build. */
frost_uint16 ICE_CIGetBufferSize(ICE_CIHandle *handle);

/**
 * @brief Read the manufacturer name from a CI Card.
 *
 * @param handle The handle to the required CI device.
 *
 */
char* ICE_CIGetManufacturerName(ICE_CIHandle * handle);

/**
 *
 * @param handle The handle to the required CI device.
 * @return whether compatible with ciplus or not
 */
frost_bool ICE_CIGetCiplusCompatibility(ICE_CIHandle *handle);


/**
 * @brief Notify fti status
 *
 * @param Current status of fti
 */
void ICE_CIIsFtiCompleted(frost_bool completed,frost_bool service_list_empty);

/**
 * @brief Notify FirstTimeSet after fti
 *
 */
frost_bool ICE_CIFirstTimeSet(frost_uint32 ctime);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_COMMON_INTERFACE_H_INCLUDED*/
