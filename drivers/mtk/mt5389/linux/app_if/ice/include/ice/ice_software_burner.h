/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Interface to burn the located software image to flash.
 *
 */

#ifndef CABOT_ICE_ICE_SOFTWARE_BURNER_H_INCLUDED
#define CABOT_ICE_ICE_SOFTWARE_BURNER_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Write data to Flash memory, primarily for the purpose of
 *        updating code areas.
 *
 * @param data_to_write Start address of the data to be written.
 * @param data_size     Length of data to be written.
 * @param destination   Address in flash to start writing data
 *                      (can be absolute or relative depending upon the value
 *                      of the relative parameter).
 * @param relative      #frost_true indicates destination is relative
 *                      to the start of the image area that the implementation
 *                      is updating.
 *                      #frost_false indicates destination is an absolute
 *                      address in flash memory.
 * @param write_to_unused_slot      #frost_true indicates software will
 *                      be written to unused slot.
 *                      #frost_false indicates software will
 *                      be written to used slot.
 *
 * @return #frost_true if data was successfully written, #frost_false if
 *         arguments were rejected.
 */
frost_bool ICE_WriteSoftwareToFlash(const frost_uint8* data_to_write,
                                         frost_ulong data_size,
                                         frost_ulong destination,
                                         frost_bool relative,
                                         frost_bool write_to_unused_slot);

/**
 * @brief Gets the dcf name and version of the found software.
 *
 * @param software_header Header information of the found software.
 * @param image_dcf_name Dcf name of the found software.
 * @param image_version Version of the found software.
 */
void ICE_GetSoftwareDcfAndVersion(unsigned char* software_header, char* image_dcf_name, frost_long* image_version);

/**
 * @brief Makes CRC on the specified software image.
 *
 * @param software_image Software image to make CRC on.
 *
 * @return #frost_true if CRC succeeds, #frost_false if otherwise.
 */
frost_bool ICE_MakeCRC(unsigned char* software_image);

/**
 * @brief Gets the size of the software image header.
 *
 * @return the size of the software image header.
 */
frost_uint32 ICE_GetHeaderSize(void);

/**
 * @brief Activates the newly burned software.
 *
 * @return #frost_true if activation succeeds, #frost_false if otherwise.
 */
frost_bool ICE_ActivateNewSoftware(void);

/**
 * @brief Deactivates the current software.
 *
 * @return #frost_true if deactivation succeeds, #frost_false if otherwise.
 */
frost_bool ICE_DeactivateCurrentSoftware(void);

/**
 * @brief Gets the address and the size of the unused flash slot
 *
 * @param slot_address address of the unused flash slot
 * @param slot_size     size of the unused flash slot
 */
void ICE_GetUnusedSlot(frost_uint* slot_address, frost_uint* slot_size);

/**
 * @brief Gets the address and the size of the flash slot that the software runs from
 *
 * @param slot_address address of the used flash slot
 * @param slot_size     size of the used flash slot
 */

void ICE_GetUsedSlot(frost_uint* slot_address, frost_uint* slot_size);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_SOFTWARE_BURNER_H_INCLUDED */

