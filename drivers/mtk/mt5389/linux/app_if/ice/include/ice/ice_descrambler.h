/**
 * Copyright 2005-9 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Interface to descrambler functionality.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code. This interface is only required for systems
 * that include an embedded CA stack. If an embedded CA stack is not present,
 * this interface need not be implemented.
 */

#ifndef CABOT_ICE_ICE_DESCRAMBLER_H_INCLUDED
#define CABOT_ICE_ICE_DESCRAMBLER_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"

/**
* A Cabot CA product will only require this number of descrambler channels
* for each descrambler device. The corresponding ICE descrambler implementation
* need only support this defined number of descrambler channels.
*/
#define ICE_MAX_DESCRAMBLER_CHANNELS (8)


#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************/
/**
 * @name Type Definitions
 * @{
 */
/***************************************************************************/

/**
 * @brief Opaque type used to identify an open connection.
 */
typedef struct ICE_DescramblerChannelHandle ICE_DescramblerChannelHandle;

/**
 * @brief Descrambler Algorithms
 */
typedef enum ICE_DescramblerAlgorithm {

    /**
     * The payload of Transport Stream packets are encrypted using CSA (Common
     * Scrambling Algorithm).
     *
     * The Key will be set using #ICE_DescramblerSetKeys
     */
    ICE_DescramblerAlgorithmCSA,

    /**
     * The payload of Transport Stream packets are encrypted using DES-56 in
     * ECB mode with residual blocks left in the clear. The DES descrambler
     * must adhere to SCTE41.
     *
     * The Key will be set using #ICE_DescramblerSetKeys
     */
    ICE_DescramblerAlgorithmDES_56_ECB,

    /**
     * The payload of transport packets are encrypted using AES-128 in CBC
     * (Cipher Block Chaining) mode requiring a Key and IV pair.
     *
     * Refer to FIPS 197 for AES-128 and NIST Special Publication 800-38A for
     * usage of AES-128 in CBC mode.
     *
     * The Key and IV will be set using #ICE_DescramblerSetKeysAndIVs
     *
     * Terminating and solitary short blocks (less than 16 bytes long) remain
     * in the clear, meaning that with a 188 byte transport packet the maximum
     * number of encrypted blocks is 11 (4 byte header - 184 byte payload, 176
     * payload bytes encrypted (11 * 16 byte blocks), 8 payload bytes in the
     * clear).
     *
     * Any transport packet with a payload less than 16 bytes long will be sent
     * in the clear.
     *
     * The IV is reset to the value given to #ICE_DescramblerSetKeysAndIVs for
     * each new Transport Packet.
     */
    ICE_DescramblerAlgorithmAES_128_CBC,

    /**
     * The payload of transport packets are encrypted using AES-128 in CBC
     * (Cipher Block Chaining) mode requiring a Key and IV pair.
     *
     * Refer to FIPS 197 for AES-128 and NIST Special Publication 800-38A for
     * usage of AES-128 in CBC mode.
     *
     * The Key and IV will be set using #ICE_DescramblerSetKeysAndIVs.
     *
     * Short blocks will not be in the clear (as with the
     * ICE_DescramblerAlgorithmAES_128_CBC mode) but will be terminated
     * according to parts 4.2 and 4.3 of ANSI/SCTE 52 2003. Although this
     * specification refers to DES, the termination algorithm can easily be
     * scaled for AES128.
     *
     * The IV is reset to the value given to #ICE_DescramblerSetKeysAndIVs for
     * each new Transport Packet.
     */
    ICE_DescramblerAlgorithmAES_128_CBC_SCTE52_Termination

} ICE_DescramblerAlgorithm;

/**
 * @brief Structure to hold descrambler key details.
 */
typedef struct ICE_DescramblerKey
{
    /** @brief Number of octets referenced by \a pointer. */
    frost_uint length;
    /** @brief Pointer to key data. */
    const frost_uint8 * pointer;

} ICE_DescramblerKey;

/**
 * @brief Structure to hold descrambler IV (Initialisation Vector) details.
 */
typedef struct ICE_DescramblerIV
{
    /** @brief Number of octets referenced by \a pointer. */
    frost_uint length;
    /** @brief Pointer to Initialisation Vector data. */
    const frost_uint8 * pointer;

} ICE_DescramblerIV;

/**
 * @brief Smart Card device open parameters.
 */
typedef struct ICE_DescramblerInitParams
{
    /**
     * The ID of the descrambler device.
     */
    ICE_DeviceNumber device_number;

} ICE_DescramblerInitParams;

/**
 * @brief Smart Card device open parameters.
 */
typedef struct ICE_DescramblerChannelOpenParams
{
    /**
     * The ID of the descrambler device.
     */
    ICE_DeviceNumber device_number;

    /**
     * The PID for which the descrambling channel should be opened.
     */
    frost_uint pid;

    /**
     * The Descrambler Algorithm that will be used to descramble transport
     * packets
     */
    ICE_DescramblerAlgorithm algorithm;

} ICE_DescramblerChannelOpenParams;

/***************************************************************************/
/**
 * @}
 * @name Descrambler Functions
 * @{
 */
/***************************************************************************/

/**
 * @brief Initialise the Descrambler device.
 *
 * Initialise the required descrambler device.
 *
 * @param params The parameters for the descrambling device to initialise
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DescramblerInit(const ICE_DescramblerInitParams* params);

/**
 * @brief Query whether a specific descrambler algorithm is supported
 *
 * @param algorithm The algorithm to check support for.
 *
 * @return #frost_true if the algorithm is supported, #frost_false otherwise.
 */
frost_bool ICE_DescramblerIsAlgorithmSupported(
                                            ICE_DescramblerAlgorithm algorithm);

/**
 * @brief specifies the descrambler type
 *
 * This function is called during the processing of the
 * certificates so the receiver stack can know which
 * descrambling algorithm will be used.
 *
 * @param algorithm The algorithm that will be used to descramble transport
 * packets
 *
 * @note The implementation of this function is optional since the algorithm is
 * also included in the ICE_DescramblerChannelOpenParams
 *
 */
void ICE_DescramblerSetAlgorithm(ICE_DescramblerAlgorithm algorithm);

/**
 * @brief Configure the transport stream source for the descrambler.
 *
 * @param source The source of the transport stream, e.g. tuner 0, tuner 1 or
 *               hard disk.
 *
 * This function will only be called when all the channels are closed on the
 * descrambler.
 *
 * @note For MHEG encrypted streaming, the source will be ICE_source_ts_player.
 *       For CI+, this function is not currently called. In this case, the
 *       platform should respond to the ICE_CISelectSource() function in order
 *       to change the source of the descrambler.
 */
void ICE_DescramblerSelectSource(ICE_DeviceSource source);

/**
 * @brief Open a connection to a descrambling channel.
 *
 * Attempts to open a connection to a descrambling channel and
 * returns a handle for subsequent communication on the channel.
 *
 * This will only be called with an algorithm that has been confirmed as
 * supported by a call to #ICE_DescramblerIsAlgorithmSupported
 *
 * This will be called for each PID that requires descrambling. This is
 * usually all active PIDs on the current service.
 *
 * Each call to this function should open the next free descrambler channel. If
 * no descrambler channels are free then it should return NULL.
 *
 * Every call to #ICE_DescramblerChannelOpen will be matched by a call to
 * #ICE_DescramblerChannelClose for the same #ICE_DescramblerChannelHandle.
 *
 * Any channels closed by a call to #ICE_DescramblerChannelClose can be
 * re-used by #ICE_DescramblerChannelOpen
 *
 * @note After this call, the descrambler channel must be ready to descramble
 *       content as soon as #ICE_DescramblerSetKeys or
 *       #ICE_DescramblerSetKeysAndIVs is called.
 *
 * @param params The open parameters indicate the required descrambler device,
 *               and the PID it needs to be associated with.
 *
 * @return Returns a pointer to the channel handle on success,
 *         and a NULL pointer on failure or if there are no free channels
 */
ICE_DescramblerChannelHandle * ICE_DescramblerChannelOpen(
                                  const ICE_DescramblerChannelOpenParams* params);

/**
 * @brief Close an open connection to the descrambling channel.
 *
 * Every call to #ICE_DescramblerChannelOpen will be matched by a call to
 * #ICE_DescramblerChannelClose for the same \a handle.
 *
 * @param handle The handle to the required descrambler channel obtained from
 * #ICE_DescramblerChannelOpen.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DescramblerChannelClose(ICE_DescramblerChannelHandle* handle);

/**
 * @brief Close an open connection to the descrambling channel.
 *
 * @return #frost_true on success, #frost_false on failure.
 */

frost_bool ICE_DescramblerChannelClose_Biss(void);


/**
 * @brief Open a connection to a descrambling channel.
 *
 * This will be called for each PID that requires descrambling. This is
 * usually all active PIDs on the current service.
 *
 * Set the Even and Odd keys that make up the Control Word for the descrambler
 * channel.
 */

void ICE_DescramblerChannelOpen_Biss(frost_uint32 biss_pid, const ICE_DescramblerKey* biss_key );

/**
 * @brief Set the control word keys for a descrambler channel.
 *
 * Set the Even and Odd keys that make up the Control Word for the descrambler
 * channel.
 *
 * This function will be called periodically (typically every 10 seconds) to
 * change the Key once a descrambler channel is open.
 *
 * If \a even_key or \a odd_key is NULL then that key should remain the same as
 * the last call to #ICE_DescramblerSetKeys.
 *
 * This function will not be called for an algorithm that requires an IV,
 * instead #ICE_DescramblerSetKeysAndIVs will be used.
 *
 * This function will only be called after a successful call to
 * #ICE_DescramblerChannelOpen.
 *
 * This function only needs to be implemented if an algorithm that doesn't
 * require IV is supported, otherwise it can be implemented as a stub that
 * returns #frost_false
 *
 * For DES-56-ECB the even and odd keys will be 8 bytes long.
 *
 * @param handle The handle to the required descrambler channel.
 * @param odd_key Odd Key component of the Control Word or NULL if the odd key
 * isn't being set.
 * @param even_key Even Key component of the Control Word or NULL if the even
 * key isn't being set.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DescramblerSetKeys(ICE_DescramblerChannelHandle* handle,
                                  const ICE_DescramblerKey* even_key,
                                  const ICE_DescramblerKey* odd_key);

/**
 * @brief Set the keys and IVs for a descrambler channel.
 *
 * Set the Even and Odd keys that make up the Control Word for the descrambler
 * channel.
 *
 * This function will not be called for an algorithm that doesn't require an
 * IV, instead #ICE_DescramblerSetKeys will be used.
 *
 * This function will only be called after a successful call to
 * #ICE_DescramblerChannelOpen.
 *
 * This function will be called periodically (typically every 10 seconds) to
 * change the Key and IV once a descrambler channel is open.
 *
 * If \a even_key and \a even_iv or \a odd_key and \a odd_iv are NULL then that
 * key and IV should remain the same as the last call to
 * #ICE_DescramblerSetKeysAndIVs.
 *
 * Keys and IVs will always be updated in pairs.
 *
 * This function only needs to be implemented if an algorithm that requires
 * an IV is supported, otherwise it can be implemented as a stub that returns
 * #frost_false
 *
 * For AES-128-CBC the even and odd keys and the IVs will be 16 bytes long.
 *
 * @param handle The handle to the required descrambler channel.
 * @param even_key Even Key component of the Control Word or NULL if the even
 * key isn't being set.
 * @param even_iv Even IV component of the Control Word or NULL if the even IV
 * isn't being set.
 * @param odd_key Odd Key component of the Control Word or NULL if the odd key
 * isn't being set.
 * @param odd_iv Odd IV component of the Control Word or NULL if the odd IV
 * isn't being set.
 *
 * @return #frost_true on success, #frost_false on failure or if not supported.
 */
frost_bool ICE_DescramblerSetKeysAndIVs(ICE_DescramblerChannelHandle* handle,
                                  const ICE_DescramblerKey* even_key,
                                  const ICE_DescramblerIV* even_iv,
                                  const ICE_DescramblerKey* odd_key,
                                  const ICE_DescramblerIV* odd_iv);

/**
 * @brief Set the encrypted control word keys for the descrambler channel.
 *
 * Set the encrypted Even and Odd keys that make up the Control Word
 * for the descrambler channel identified.
 *
 * This function is only required for embedded CA builds and can be implemented
 * as a stub that returns #frost_false for CI Plus.
 *
 * @param handle The handle to the required descrambler channel.
 * @param protecting_key The encrypted key which is when decrypted is used to
 *                       decrypt the encrypted even and odd keys
 * @param protected_even_key  Protected Even Key component of the Control Word
 * @param protected_odd_key Protected Odd Key component of the Control Word
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DescramblerSetProtectedKeys(ICE_DescramblerChannelHandle* handle,
                                  const ICE_DescramblerKey* protected_even_key,
                                  const ICE_DescramblerKey* protected_odd_key,
                                  const ICE_DescramblerKey* protecting_key);



/**
 * @brief Set the PID for the descrambler channel
 *
 * This function is only required for embedded CA builds and can be implemented
 * as a stub that returns #frost_false for CI Plus.
 *
 * @param handle The handle to the required descrambler channel.
 * @param pid The PID for the descrambler channel
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DescramblerSetPid(ICE_DescramblerChannelHandle* handle,
                                  frost_uint pid);


/**
 * @brief Set the stream type for the descrambler channel
 *
 * This function is only required for embedded CA builds and can be implemented
 * as a stub that returns #frost_false for CI Plus.
 *
 * @param handle The handle to the required descrambler channel.
 * @param stream_type
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DescramblerSetStreamType(ICE_DescramblerChannelHandle* handle,
                                  frost_uint stream_type);



/**
 * @brief Set the metadata for the descrambler channel
 *
 * This function is only required for embedded CA builds and can be implemented
 * as a stub that returns #frost_false for CI Plus.
 *
 * @param handle The handle to the required descrambler channel.
 * @param length The length of the metadata
 * @param pointer A pointer to the metadata
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DescramblerSetMetadata(ICE_DescramblerChannelHandle* handle,
                                  frost_uint length, const frost_uint8 *pointer);




/**
 * @brief Get The Nagra Unique Chipset Identifier (NUID)
 *
 * This function is only required for embedded CA builds and can be implemented
 * as a stub that returns #frost_false for CI Plus.
 *
 * Note that the data returned will be in big-endian order (MSB first).
 *
 * @param nuid Set to the Nagra Unique Chipset Identifier (NUID) value
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DescramblerGetNUID( ICE_DescramblerKey* nuid );


/**
 * @brief Encrypt Data using current algorithm
 *
 * This function is only required for embedded CA builds and can be implemented
 * as a stub that returns #frost_false for CI Plus.
 *
 * @param src pointer to data to be encrypted
 * @param dst pointer to encrypted data
 * @param len length of data to be encrypted
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DescramblerEncrypt( frost_uint8 *src, frost_uint8 *dst, frost_uint len );


/**
 * @brief Decrypt Data using current algorithm
 *
 * This function is only required for embedded CA builds and can be implemented
 * as a stub that returns #frost_false for CI Plus.
 *
 * @param src pointer to data to be decrypted
 * @param dst pointer to decrypted data
 * @param len length of data to be decrypted
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_DescramblerDecrypt( frost_uint8 *src, frost_uint8 *dst, frost_uint len );


/**
 * @brief Returns Descrambler status
 *
 * This function is only required for embedded CA builds and can be implemented
 * as a stub that returns #frost_false for CI Plus.
 *
 * @return #frost_true when Descrambler is opened, #frost_false on it is closed.
 */

frost_bool ICE_DescramblerGetStatus(void);


/**
 * @brief Changes Descrambler status
 *
 * This function is only required for embedded CA builds and can be implemented
 * as a stub that returns #frost_false for CI Plus.
 *
 * @param active indicates the status to be set
 *
 * @return #frost_true when Descrambler is opened, #frost_false on it is closed.
 */


void ICE_DescramblerSetStatus( frost_bool active );


/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_DESCRAMBLER_H_INCLUDED*/
