/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot's low level support routines to allow Eclipse to
 *        ascertain whether a detected SSU or BBC-style software update
 *        is of interest to the ICE implementation.
 *
 * Callisto is Cabot's software module which loads off-air software updates.
 * If Eclipse is built without Cabot's Callisto, then Eclipse will be unable
 * to load off-air software updates.  However, it will still be capable of
 * detecting that a software update is available which the implementation may be able
 * to load using some proprietary software mechanism other than Callisto.  This
 * ice_software_update API allows Eclipse to query the implementation as to
 * whether a software update that has been detected is applicable to the
 * implementation.
 *
 * This interface should be implemented in all Eclipse products (regardless of
 * whether Callisto is integrated) if Eclipse is to be able to detect relevant
 * off-air software updates.  Detection of an update is not the same as the
 * downloading of the update.
 *
 * If off-air update detection is not required, then the functions described in
 * this API may be stubbed out with null implementations.
 */

#ifndef CABOT_ICE_ICE_SOFTWARE_UPDATE_H_INCLUDED
#define CABOT_ICE_ICE_SOFTWARE_UPDATE_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the current software version.
 *
 * Retrieves the version of the current software, in the range 0x0001 to 0xFFFF.
 *
 * The version should be incremented on each new production software
 * release.  Software downloads will only be accepted with a version number
 * that is greater than the value returned by this function.
 *
 * @note: returned value must not be zero.
 *
 * @return The implementation's 16-bit software version number
 */
frost_uint16 ICE_SoftwareVersion(void);

frost_uint16 ICE_SoftwareMinorVersion(void);


/**
 * @brief Get the primary Organisationally Unique Identifier (OUI)
 *
 * @return The 24-bit IEEE OUI to which this implementation responds.
 */
frost_long ICE_SoftwareUpdateGetOUI(void);

/**
 * @brief Get postfix for software version
 *
 * @return postfix .
 */
frost_uint8 ICE_SoftwareVersionPostfix(char* postfix);


/**
 * An enumeration to specify the behaviour of
 * ICE_SoftwareUpdateIsDescriptionApplicable()
 */
typedef enum ICE_SoftwareUpdateDescriptionType
{
    ice_test_oui_only   /** ICE_SoftwareUpdateIsDescriptionApplicable() should
                         *  ignore the model and version elements of the
                         *  ICE_SoftwareUpdateDescription */
   ,ice_test_hardware   /** ICE_SoftwareUpdateIsDescriptionApplicable() should
                         *  test the OUI, model and version elements of the
                         *  ICE_SoftwareUpdateDescription against the
                         *  implementation's OUI, hardware model and version numbers.
                         */
   ,ice_test_software   /** ICE_SoftwareUpdateIsDescriptionApplicable() should
                         * test the OUI, model and version elements of the
                         * ICE_SoftwareUpdateDescription against the
                         * implementation's OUI, software model and version numbers.
                         */
} ICE_SoftwareUpdateDescriptionType;

/**
 * A structure to carry the information obtained from potential download
 * signalling which is passed to ICE_SoftwareUpdateIsDescriptionApplicable().
 */
typedef struct ICE_SoftwareUpdateDescription
{
    /**
    * A 3-byte IEEE OUI (see http://www.ieee.org).
    */
    frost_long organizationally_unique_identifier;

    /**
    * A 2-byte Model number.
    */
    frost_uint16 model;

    /**
    * A 2-byte Version number.
    */
    frost_uint16 version;

    /**
     * A structure to carry the optional field customers
     */
    struct
    {
        char **customers;
        frost_uint16 length;
    } customers;

    /**
     * A structure to carry the optional field version range
     */
    struct
    {
        char **id;
        frost_uint16 length;
    } devices;

    /**
     * A structure to carry the optional field version range
     */
    struct
    {
        frost_uint16 min;
        frost_uint16 max;
    } versionRange;

} ICE_SoftwareUpdateDescription;


/**
 * @brief Test whether an ICE_SoftwareUpdateDescription is applicable to this
 * implementation.
 *
 * The implementation MUST contain:
 * -# a 3-byte IEEE manufacturer's Organizationally Unique Identifier
 * -# a 2-byte Hardware Model Number in the range 0x0001 to 0xFFFF.
 * -# a 2-byte Software Model number in the range 0x0001 to 0xFFFF.
 * -# a 2-byte Software Version number in the range 0x0001 to 0xFFFF.
 *
 * The implementation may optionally contain:
 * -# a 2-byte Hardware Version number in the range 0x0001 to 0xFFFF to
 * further differentiate between Hardware Models.
 *
 * A manufacturer could choose to use a single number for both the Hardware
 * and Software Model numbers,  Separate values may provide greater update
 * flexibility.
 *
 * The implementation should test the fields within the
 * ICE_SoftwareUpdateDescription structure to determine if a
 * potential software download may be destined for this implementation.
 *
 * @param description_type indicates the testing required (see below).
 * @param description an ICE_SoftwareUpdateDescription structure containing
 *        the fields to test.<br>
 *          <b><code>organizationally_unique_identifier</code></b><br>
 *                    This should match the implementation's Organizationally Unique Identifier.<br>
 *          <b><code>model</code></b><br>
 *                    If <i>description_type</i> is set to <i>ice_test_oui_only</i>, or if this
 *                    field is zero, then the implementation should ignore this field.<br>
 *                    If <i>description_type</i> is set to <i>ice_test_hardware</i> then
 *                    the implementation's Hardware Model number must match this field.<br>
 *                    If <i>description_type</i> is set to <i>ice_test_software</i> then
 *                    that the implementation's Software Model number must match this field.<br>
 *          <b><code>version</code></b><br>
 *                    If <i>description_type</i> is set to <i>ice_test_oui_only</i>, or if this
 *                    field is zero, then the implementation should ignore this field.<br>
 *                    If <i>description_type</i> is set to <i>ice_test_hardware</i> then
 *                    the implementation's Hardware Version number (if used) must match this field.<br>
 *                    If <i>description_type</i> is set to <i>ice_test_software</i> then
 *                    the implementation's Software Version number must be LESS THAN this field.<br>
 * @param is_enhanced_oad whether it is an enhanced oad or not
 * @param is_advertisement_oad whether it is an advertisement oad or not
 * @return #frost_true if applicable to this implementation or #frost_false if not applicable.
 */
frost_bool ICE_SoftwareUpdateIsDescriptionApplicable(ICE_SoftwareUpdateDescriptionType description_type,
                                                    const ICE_SoftwareUpdateDescription *description,
                                                    const frost_bool is_enhanced_oad,
                                                    const frost_bool is_advertisement_oad);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_SOFTWARE_UPDATE_H_INCLUDED*/
