/**
 * Copyright 2002-4 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Allow Eclipse to query the implementation for various settings.
 */

#ifndef CABOT_ICE_ICE_SETTINGS_H_INCLUDED
#define CABOT_ICE_ICE_SETTINGS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"

/**
 * @brief Get the maximum number of services to be supported.
 *
 * Eclipse requires (97 + u) x n bytes (plus about 3% overhead) for
 * non-volatile storage of service information, where u is the maximum
 * (UI) application user data size, and n is the maximum number of services.
 *
 * @return The maximum number of services to be supported.
 */
frost_int ICE_SettingsMaxNumberOfServices(void);

/**
 * @brief Get the maximum number of multiplexes to be supported.
 *
 * Eclipse requires 42 x n bytes (plus about 75% overhead) for
 * non-volatile storage of terrestrial multiplex information, where n is the
 * maximum number of multiplexes.
 *
 * @return The maximum number of multiplexes to be supported.
 */
frost_int ICE_SettingsMaxNumberOfMultiplexes(void);

/**
 * @brief Get the maximum amount of heap memory to be used for DSMCC data.
 *
 * DSMCC data is used to convey MHEG-5 applications and Callisto updates.
 * Eclipse will allocate memory from the heap in amounts of up to 4096
 * bytes to store DSMCC data.  This function allows the implementation
 * to specify the aggregate heap memory to be used for this purpose.
 * Once the DSMCC's heap usage exceeds this amount, the cache will be
 * trimmed to occupy 90% of maximum.
 *
 * For implementations including the MHEG-5 engine (with or without Callisto)
 * Cabot recommends that this function returns a value of at least 6MB.  For
 * implementations with Callisto but without MHEG-5, we recommend at least
 * 500kB.
 *
 * @return The the maximum amount of heap memory to be used for DSMCC data.
 */
frost_int32 ICE_SettingsDsmccCacheLimit(void);

/**
 * @brief Get the maximum amount of heap memory to be used for EIT data.
 *
 * EIT data is used to convey programme schedule information.
 * Eclipse will allocate memory from the heap in amounts of up to 4096
 * bytes to store EIT data.  This function allows the implementation
 * to specify the aggregate heap memory to be used for this purpose.
 * Once EIT heap usage exceeds this amount, the EIT cache will be
 * trimmed to occupy 90% of maximum.
 *
 * The value returned by this function will depend on where and how Eclipse is
 * to be used. In the UK, for example, a value of 3MB will ensure all
 * broadcast EIT data can be cached for viewing the next 8 days. 
 *
 * @return The the maximum amount of heap memory to be used for EIT data.
 */
frost_int32 ICE_SettingsEitCacheLimit(void);

/**
 * @brief Get the size of persistent storage to be used for reminders.
 *
 * The EASIReminder structure (defined in eclipse/easi/EASIReminderManager.hpp) holds
 * details of service events which the UIapp requires to be reminded about.
 * Each reminder holds the service ID, start and end time, a "repeat" flag, and
 * may also include the event name and description (synopsis).  The size returned
 * by this function affects the total number of reminders that can be stored
 * persistently.  A reminder with a 32 character name and 256 byte synopsis will
 * require 312 bytes.  A reminder with no name/synopsis will require 24 bytes.
 * 
 * For UIapps that do not use reminders, this function may return 0.
 *
 * @return The number of bytes of persistent storage to be used for reminders.
 */
frost_int32 ICE_SettingsMaxRemindersStorage(void);

/**
 * @brief Determine the amount of persistent storage available for Freesat MHEG.
 *
 * @note Only needed for Freesat support.
 *
 * This is the amount of non-volatile storage available to Freesat MHEG
 * applications for storage of files (and associated metadata).  The Freesat
 * specification requires this to be a minimum of 256kB.
 *
 * For files of the minimum size, Eclipse overheads account for about 75% of
 * the total storage space, and for much larger files (e.g. around 10kB), these
 * overheads are about 3%.
 * Therefore, to handle the worst case of all files being of minimum size and
 * fully satisfy the minimum requirement, the implementation must allocate
 * approximately 1MB of non-volatile memory and return the value 256kB.
 * 
 * @return The number of bytes of available persistent storage.
 */
frost_int32 ICE_SettingsFreesatMaxMhegNvmSize(void);

/**
 * @brief Get the number of section filters that can be used.
 *
 * The ice_section_acquisition API must support at least this many section
 * filters.  Aurora will not attempt to use more section filters than the
 * number specified here.
 *
 * @note Currently this function is only called in MHP builds.
 *
 * @return the number of section filters that can be used.
 */
frost_uint32 ICE_SettingsMaxNumberOfSectionFilters(void);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_SETTINGS_H_INCLUDED*/
