/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Callisto API.
 *
 * Callisto is Cabot's software module providing Eclipse with over-air
 * software download capability (OAD).  Callisto can receive OADs which
 * are broadcast using the DVB System Software Update (SSU) standard, or
 * using the BBC's method on UK terrestrial broadcasts.  SSU is defined
 * in ETSI TS 102 006. The BBC's method is defined in the D-book chapter 24.
 *
 * Both standards convey the update data to the receiver in a DSM-CC carousel.
 * The UK standard uses an object carousel, while SSU uses a data carousel.
 * The carousel contains a textual "download control file" (DCF), and several
 * binary image parts.  Each specific ICE implementation will search for one
 * specific DCF file.
 *
 * The DCF file tells Eclipse, among other things:
 *
 * -# what version number of software is carried in the carousel;
 * -# a textual description of the software in the carousel;
 * -# which binary image parts should be downloaded from the carousel;
 * -# how to authenticate each image part to ensure it is error-free;
 * -# where each image part should be written in non-volatile memory.
 *
 * Using Cabot's SEA toolkit will prevent individuals or organisations from
 * constructing and broadcasting malicious code.  The DCF file is digitally
 * signed by the responsible authority.  Eclipse will validate the signature
 * using public keys which are be stored in the implementation's non-volatile
 * memory.
 *
 * <b> UK OAD MECHANISM </b>
 *
 * In the UK, Callisto's terrestrial over-air downloads (OADs) are broadcast
 * by the BBC using a DSM-CC object carousel.
 * The DCF file and all the image parts must exist as DSM-CC file objects
 * under a top-level DSM-CC directory object named with the organisationally
 * unique identifier (OUI) of the responsible authority.  The script
 * makeOADfs.pl can be used to generate a complete filesystem.  The script can
 * also create a configuration file allowing Softel's MediaSphere to generate a
 * transport stream containing the object carousel.
 *
 * <b> SSU OAD MECHANISM </b>
 *
 * Outside the UK (and for UK satellite), Callisto over-air downloads
 * are broadcast using a DSM-CC data carousel.  The DCF file is carried in
 * a DSM-CC module named with the DCF filename.  Each of the image parts are carried in
 * separate DSM-CC modules, the names of which are identified in the DCF.
 * The script makeOADfs.pl can be used to generate a complete filesystem.
 * The script can also generate a sequence of Intellibyte commands which will create a
 * transport stream containing the data carousel.
 *
 * ---------------------------------------------------------------------------------
 *
 * EXAMPLE USAGE CASE: assuming a download is available and each step is successful:-
 *
 * .................................... <===== Eclipse calls ICE_CallistoGetDCFName()<br>
 * .................................... ICE implementation returns OUI/DCF filename (case-sensitive).<br>
 * .................................... <===== Eclipse calls ICE_CallistoGetExpectedNumberOfSignatures()<br>
 * .................................... ICE implementation returns number of keys that require validating.<br>
 * foreach key that requires validating
 * {<br>
 * .................................... <===== Eclipse calls ICE_CallistoGetKey().<br>
 * .................................... Eclipse validates DCF file's signature(s) with the public key(s).<br>
 * .................................... <===== Eclipse calls ICE_CallistoReleaseKey()<br>
 * }<br>
 * .................................... <===== Eclipse may call ICE_CallistoInvalidateKeys() if necessary.<br>
 * foreach part image
 * {<br>
 * .................................... <===== Eclipse calls ICE_CallistoWriteBytesToFlash().<br>
 * }<br>
 * .................................... <===== Eclipse may call ICE_CallistoReadBytesFromFlash() repeatedly, if necessary.<br>
 * .................................... <===== Eclipse calls ICE_CallistoActivateNewCode()<br>
 *..................................... a subsequent call to ICE_HardwareReset() causes the new code to be exectuted.<br>
 *
 * ---------------------------------------------------------------------------------
 */

#ifndef CABOT_ICE_ICE_CALLISTO_H_INCLUDED
#define CABOT_ICE_ICE_CALLISTO_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Callisto API
 * This API allows Eclipse's Callisto module to locate a download
 * and store it to the receiver's flash memory.
 * @{
 */

/**
 * @brief Retrieve null-terminated download control file (DCF) name
 *
 * Cabot Communications' OADs are validated and controlled using a DCF file
 * which is delivered to the receiver either using the UK mechanism
 * (documented in chapter 24 of the D-book), or by DVB System Software Update
 * (SSU, documented in ETSI TS 102 006).
 *
 * <b> UK OAD MECHANISM </b>
 *
 * The DCF file is broadcast in the BBC Engineering Channel's DSM-CC object
 * carousel.  The file must reside in a directory off the SRG (root directory)
 * named with the OUI (Organisationally Unique Identifier) of the responsible
 * authority.  The OUI must be represented as six hex digits, using lower-
 * case if it contains any letters.  Using Cabot's OUI of 000a2d as an
 * example, a valid DCF filename might be something like
 * "/000a2d/st5518_dvb_receiver_type1.dcf".
 *
 * <b> SSU OAD MECHANISM </b>
 *
 * For SSU, rules governing the format of the DCF name are slightly more
 * stringent. The DCF name must be made up of the manufacturer's
 * OUI and the hardware model number.  The hardware model number must be four
 * hex digits, using lower-case if it contains any letters.  So a valid DCF
 * filename for an SSU download might be "/000a2d/89ab.dcf", where the
 * hardware model number is 0x89ab.
 *
 * @note The string must begin with "/xxxxxx/" where xxxxxx is the OUI in
 *       hex, using lower-case if it contains any letters.  There must be
 *       no other '/' characters in the string.
 *
 * @return pointer to static string containing name of download control file.
 */
const char* ICE_CallistoGetDCFName(void);

/**
 * @brief Write data to Flash memory, primarily for the purpose of
 *        updating code areas.
 *
 * Called by Eclipse to write a sequence of bytes to flash memory.  Eclipse
 * will call this function when instructed to write OAD data by the downloaded
 * DCF file.  This function is called once for each binary image part in the
 * download.
 *
 * When the OAD is being used to download a new software image the
 * <code>relative</code> parameter will usually be set to true, since in
 * general only the implementation will know the correct location to write the
 * new image.  This is because it is possible for different boxes in the field
 * to have received previous updates and to therefore be running images from
 * different flash locations.  Setting the <code>relative</code> parameter to
 * false allows an OAD to write arbitrary data to anywhere in flash. This is
 * not commonly used, but could be used to update the bootloader or to place
 * pictures in an unused area of flash, for example.
 *
 * @note Although this function gives Eclipse the ability to irreparably
 * damage the contents of flash memory, it should be noted that the function
 * will only be called as a direct consequence of instructions in the DCF file.
 * The DCF file contains authentication strings to ensure that the data to be
 * written is correct, and the DCF itself is signed to ensure that it is valid.
 * It is possible for the DCF to specify that data should be written as soon
 * as it is received.  In this case it is authenticated later using calls to
 * ICE_CallistoReadBytesFromFlash(), and ICE_CallistoActivateNewCode() will
 * only be called if the downloaded data passes authentication.
 *
 * @param data_to_write Start address of the data to be written.
 * @param data_size     Length of data to be written.
 * @param destination   Address in flash to start writing data
 *                      (can be absolute or relative depending upon the value
 *                      of the <code>relative</code> parameter).
 * @param relative      #frost_true indicates destination is relative
 *                      to the start of the image area that the implementation
 *                      is updating.
 *                      #frost_false indicates destination is an absolute
 *                      address in flash memory.
 * @param is_advertisement_oad      #frost_true indicates data is downloaded  as advertisement oad
 *
 * @return #frost_true if data was successfully written, #frost_false if
 *         arguments were rejected.
 */
frost_bool ICE_CallistoWriteBytesToFlash(const frost_uint8* data_to_write,
                                         frost_ulong data_size,
                                         frost_ulong destination,
                                         frost_bool relative,
                                         frost_bool is_advertisement_oad);
/**
 * @brief Read data from Flash memory for the purpose of authenticating an
 *        image written with ICE_CallistoWriteBytesToFlash().
 *
 * This function is only used when blocks of downloaded data are written
 * using ICE_CallistoWriteBytesToFlash() as they arrive, where there is no
 * internal cache.  It supplies the written data so it can be authenticated.
 *
 * Called by Eclipse to read a sequence of bytes from flash memory.  Eclipse
 * will call this function when the downloaded DCF requires that OAD data
 * is authenticated after is has been written.  This function is called to
 * read back the binary image in small sections.
 *
 * @param data_to_read  Start address of the data to be read.
 * @param data_size     Length of data to be read.
 * @param source        Address in flash to start reading data
 *                      (can be absolute or relative depending upon the value
 *                      of the <code>relative</code> parameter).
 * @param relative      #frost_true indicates source is relative
 *                      to the start of the image area that the implementation
 *                      is updating.
 *                      #frost_false indicates source is an absolute
 *                      address in flash memory.
 * @param is_advertisement_oad      #frost_true indicates data is downloaded  as advertisement oad
 *
 * @return #frost_true if data was successfully read, #frost_false if
 *         arguments were rejected or the function is not implemented.
 */
frost_bool ICE_CallistoReadBytesFromFlash(frost_uint8* data_to_read,
                                          frost_ulong data_size,
                                          frost_ulong source,
                                          frost_bool relative,
                                          frost_bool is_advertisement_oad);

/**
 * @brief Make the newly received code operational.
 *
 * Called by Eclipse to indicate that the code has been successfully written to
 * flash and that no further calls to ICE_CallistoWriteBytesToFlash() will be made.
 * The implementation of this function should ensure that a subsequent call to
 * ICE_HardwareReset() will cause the newly written code to be booted.
 *
 * @note This function should not cause the system to reboot.
 *
 * @return #frost_true if the new image will become operational following
 *         a subsequent reboot, otherwise #frost_false.
 */
frost_bool ICE_CallistoActivateNewCode(void);

/**
 * @}
 * @name SEA Toolkit API
 * This API allows the SEA Toolkit to verify the integrity and authenticity of
 * the download.  The functions in this API need not be implemented if Cabot's
 * SEA Toolkit is not being used for digitally signing downloads.
 * @{
 */

/**
 * @brief The structure used to convey security and authentication keys.
 *
 * Each set-top box or IDTV to which OADs are to be loaded must have a
 * set of keys held in non-volatile memory.  Ideally, this would be
 * read-only memory such that the keys cannot be tampered with.  There
 * would then need to be a separate set of "key validity" flags in
 * writeable non-volatile memory in order that keys can be marked as
 * invalid if the need arises.  Alternatively, writeable NVRam can be
 * used to store the keys, in which case "invalidity" can be indicated
 * by overwriting the key in question.
 *
 * These keys are "public key counterparts" to the private keys that will
 * be use for signing the DCF file in the OAD carousel.  In theory, a single
 * private key and a single public key will be all that is required.  In
 * practice, a private key might become compromised (by hacking, espionage,
 * etc.) and for this reason there is usually more than one private/public
 * key pairs.  The implementation might contain, say, 6 public keys.  There
 * would then be 6 private keys to keep secure - perhaps by keeping 3 in a safe
 * and 3 in a bank somewhere.  The DCF file will be signed with a key, and
 * contains the index number of the key that was used to sign it. (The first
 * public key is defined as an index of 1, the second 2, etc.)  Eclipse then
 * authenticates the signature using the correct public key by calling
 * ICE_CallistoGetKey() with the key index specified in the DCF file.
 *
 * If more than one signing authority is required, each authority can use a
 * different number of keys, but key ids must be numbered contiguously.
 * For example, if 6 keys are used for each of two authorities, the ids
 * must be numbered with 1-6 and 7-12 (but not 1-6 and 11-16).
 */
typedef struct ICE_CallistoKey
{
  /**
   * It is possible to have more than one signature on the DCF
   * file, so that OADs can be signed by both the hardware manufacturer and
   * the branding organisation (for example).
   * Each key therefore has an associated 'owner_id' in the form
   * of a 32-bit unsigned integer.  The owner_id of each signing authority
   * must be different, though the actual owner_id values are not important.
   * Eclipse ensures that the DCF file has been signed the correct number of
   * times, with a different 'owner' for each signature.
   */
   frost_uint32 owner_id;

  /**
   * <code>is_valid</code> will be #frost_true for any key which has
   * never been marked as invalid, and #frost_false for keys which have
   * been marked as invalid.  All keys are initially assumed to be
   * valid and are invalidated by ICE_CallistoInvalidateKeys().
   *
   * @see ICE_CallistoInvalidateKeys
   */
   frost_bool is_valid;

  /**
   * Specifies the size of the #key_data array in octets.
   * This is typically the same for all keys in a given
   * implementation, but is not required to be.
   */
   frost_uint data_length_in_octets;

  /**
   * The key data array of #data_length_in_octets unsigned chars
   * each holding one byte of the public key.
   */
   frost_uint8 * key_data;
} ICE_CallistoKey;


/**
 * @brief Look up the key with a given index and return its data.
 *
 * This function is used to map from a key index to that key and its
 * associated data.  Each call to ICE_CallistoGetKey() is followed by a call
 * to ICE_CallistoReleaseKey().
 *
 * @param index_of_key_to_return The index of the key being requested.
 * @note  The first key has an index of 1, the second 2, etc. -
 *        see ICE_CallistoKey.
 * @param returned_key Output parameter is a pointer to the OAD key data
 *        structure that is be populated by this function.
 *
 * @return #frost_true on success, #frost_false on failure, importantly
 *         including when no key exists with the given index.
 */
frost_bool ICE_CallistoGetKey(frost_uint index_of_key_to_return,
                       ICE_CallistoKey *returned_key);


/**
 * @brief Release any resources associated with a key obtained from
 *        ICE_CallistoGetKey().
 *
 * ICE_CallistoReleaseKey() must be called exactly once for each call to
 * ICE_CallistoGetKey(), once the data returned by ICE_CallistoGetKey() is no
 * longer needed.
 *
 * On some systems, ICE_CallistoGetKey() will need to allocate space in
 * directly addressable memory to hold the key data.  Calling
 * ICE_CallistoReleaseKey() gives the underlying software a chance to
 * reclaim this and any other associated resources.  On platforms
 * where ICE_CallistoGetKey() does not allocate resources this function
 * may do nothing.
 *
 * @param index_of_key_to_release The index of the key being released.
 * @note  The first key has an index of 1, the second 2, etc.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_CallistoReleaseKey(frost_uint index_of_key_to_release);


/**
 * @brief Invalidate all keys to reflect the fact that the key
 *        identified by index_of_used_key has been used.
 *
 * Invalidate all keys with lower index numbers that have the same owner_id.
 *
 * @note The key with the given index (index_of_used_key)
 *       should <b>not</b> be invalidated.  The fact that a key was used
 *       indicates that it is the "current" key - only previous keys
 *       <b>which have the same owner_id</b> should be invalidated.
 *
 * @param index_of_used_key The index of the key being used.
 *
 * @return #frost_true if the input key index is valid and all appropriate keys
 *         have been invalidated, #frost_false otherwise.
 */
frost_bool ICE_CallistoInvalidateKeys(frost_uint index_of_used_key);


/**
 * @brief Obtain the number of signatures to be present in the signature
 *        block of the DCF file.
 *
 * Each signature corresponds to a different signing authority, and each
 * signing authority will use keys with a different owner_id.  For example,
 * if OADs are to be signed by (i) the hardware manufacturer, (ii) the
 * branding organisation and (iii) Cabot Communications, then this function
 * must return 3.  If OADs will be validated using a single signature, then
 * this function must return 1.
 *
 * @note Important:  If this function returns zero, then Eclipse will not
 *       require the presence of any signatures before downloading and
 *       possibly activating an OAD.  In this case, the SEA toolkit is
 *       effectively disabled.
 *
 * @return The number of different signatures that must be present in a
 *         valid DCF file.
 */
frost_uint ICE_CallistoGetExpectedNumberOfSignatures(void);


/**
 * @brief Sets download type as advertisement oad
 *
 * @param is_advertisement_oad  whether it is advertisement oad or not
 *
 * @note It is used for advertisement download for inouttv.
 *       If inouttv feature is not supported, no need to implement this function
 */
void ICE_CallistoSetAdvertisementDownload(frost_bool is_advertisement_oad);


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_CALLISTO_H_INCLUDED*/
