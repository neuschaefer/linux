/**
 * Copyright 2002-5 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Positive-negative section filtering and acquisition.
 *
 * This file describes part of the ICE interface to the section acquisition
 * module.  This file contains only the call to set up section filters; the
 * rest of the API is defined in ice_section_acquisition_common.h.  You must
 * implement the functions defined in ice_section_acquisition_common.h.
 *
 * This file describes the ICE interface for positive-negative section
 * acquisition.
 *
 * @note This file is not normally implemented.
 *       See ice_section_acquisition_common.h for details.
 */

#ifndef CABOT_ICE_ICE_SECTION_ACQUISITION_POSNEG_H_INCLUDED
#define CABOT_ICE_ICE_SECTION_ACQUISITION_POSNEG_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_section_acquisition_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Maximum length of a section filter.
 *
 * @note This value includes the table ID and the section length.  However,
 *       the 2 bytes used for the section length can be ignored by the
 *       implementation.
 */
#define ICE_FILTER_MAX_LENGTH 12

/**
 * @brief ICE_PosNegFilterDescription is used to describe a set of sections
 *        which are wanted.
 *
 * A section matches this filter if and only if:
 * <pre>
 * section.pid = filter.pid &&
 * &&   Matches(section, filter.positive_data, filter.positive_mask)
 * && ! Matches(section, filter.negative_data, filter.negative_mask)
 *
 * Where Matches(section, data, mask) is defined as:
 * {
 *    result = true
 *    for (i = 0; i < ICE_FILTER_MAX_LENGTH; i++) {
 *       if (i==1 || i == 2) {
 *          continue; // section[1] and section[2] are ignored
 *       }
 *       if (section[i] & mask[i] != data[i] & mask[i]) {
 *          result = false
 *       }
 *    }
 *    return result;
 * }
 * </pre>
 * I.e. (less formally) the filter matches if all the bits that are
 * positive-filtered match, and any of the bits that are
 * negative-filtered do not match.
 *
 * Note that the second and third bytes of the section are not used
 * for matching.
 *
 * @note Cabot guarantee that no bit is used for both positive and
 *       negative matching - i.e. <code>(filter.positive_mask[i] &
 *       filter.negative_mask[i]) == 0</code> holds for all legal
 *       values of i.
 */
typedef struct
{
    /**
     * The PID must always be specified; no wildcarding is possible.  Only
     * sections with a given PID match this filter.
     */
    frost_uint pid;

    /**
     * The section data to filter for.  Starts matching at the first byte
     * of the section, i.e. the 1-byte table ID.
     */
    frost_uint8 positive_data[ICE_FILTER_MAX_LENGTH];

    /**
     * Positive filter mask.  Note that positive_mask[1] and
     * positive_mask[2] must be zero.
     */
    frost_uint8 positive_mask[ICE_FILTER_MAX_LENGTH];

    /**
     * The section data to filter against.
     */
    frost_uint8 negative_data[ICE_FILTER_MAX_LENGTH];

    /**
     * Negative filter mask.
     */
    frost_uint8 negative_mask[ICE_FILTER_MAX_LENGTH];

    /**
     * Flag to indicate whether sections should be automatically discarded
     * if their CRC error check fails.
     */
    frost_bool discard_on_crc_error;

    /**
     * An optional pointer can be supplied (user_data) that will be
     * passed to all callbacks.  This can be used to provide context
     * information.  This is treated as an opaque pointer by the
     * section acquisition module.
     */
    void* user_data;

    /**
     * Callback used to obtain buffers into which to store sections matching
     * this filter.
     */
    ICE_RequestBufferCallback request_buffer;

    /**
     * Callback used to return sections matching this filter when
     * ICE_RequestBufferCallback() is able to supply a buffer.
     */
    ICE_SectionReadyCallback section_ready;

    /**
     * Callback used to release a buffer when the subsequent attempt to
     * acquire section data has failed.
     */
    ICE_ReleaseBufferCallback release_buffer;

} ICE_PosNegFilterDescription;

/**
 * @brief Set up a new positive-negative section filter.
 *
 * ICE_RequestPosNegFilter() is called by the client to tell the section
 * acquisition module to collect and pass on sections matching a given
 * #ICE_PosNegFilterDescription.
 * Sections matched by ICE filters must have valid CRC fields (but only when
 * the section_syntax_indicator field is set to '1') - Eclipse does not check
 * CRCs as it is assumed that the driver implementation has already done this.
 * A filter established with ICE_RequestPosNegFilter() remains active until
 * it is cancelled by a call to ICE_CancelFilter().
 *
 * It is implementation-dependent whether or not this function blocks if
 * filter callbacks are underway.
 *
 * @note The <code>new_filter</code> structure is only guaranteed to exist for
 *       the duration of this call.  I.e. the implementation must not try to
 *       dereference this pointer after ICE_RequestPosNegFilter() has
 *       returned.
 *
 * @note The ICE implementation must allow multiple simultaneous filters on
 *       the same PID.  Incoming sections that match against multiple filters
 *       must result in separate calls to the #ICE_RequestBufferCallback for
 *       each matching filter.
 *
 * @param filter_bank Identifies which filter bank to use.  Will be a
 *                    value returned by #ICE_FilterBankInit().
 * @param new_filter  The description of the set of wanted sections from the
 *                    new filter.
 *
 * @return On success, an #ICE_FilterHandle pointer which can be passed to
 *         ICE_CancelFilter() to cancel this filter.  On failure, a null
 *         pointer.
 */
ICE_FilterHandle* ICE_RequestPosNegFilter(
                        ICE_FilterBankHandle* filter_bank,
                        const ICE_PosNegFilterDescription* new_filter);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_SECTION_ACQUISITION_POSNEG_H_INCLUDED*/
