/**
 * Copyright 2002-5 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Simple interface for section filtering and acquisition.
 *
 * This file describes part of the ICE interface to the section acquisition
 * module.  This file contains only the call to set up section filters; the
 * rest of the API is defined in ice_section_acquisition_common.h.  You must
 * implement the functions defined in ice_section_acquisition_common.h.
 *
 * @note Most products will implement this API.
 *       See ice_section_acquisition_common.h for details.
 */

#ifndef CABOT_ICE_ICE_SECTION_ACQUISITION_SIMPLE_H_INCLUDED
#define CABOT_ICE_ICE_SECTION_ACQUISITION_SIMPLE_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_section_acquisition_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Describes a set of sections which are wanted.
 *
 * A PID must always be present. TID and TIDEXT specifications are
 * optional; if absent, any TID or TIDEXT should be matched by the
 * filter.  The version is typically used to exclude the current
 * version, to avoid being informed about the same version of a
 * section repeatedly.
 */
typedef struct
{
    /**
     * The PID must always be specified; no wildcarding is possible.  Only
     * sections with a given PID match this filter.
     */
    frost_uint pid;

    /**
     * If <code>accept_any_tid</code> is set to a #frost_true value then
     * sections with any TID may match this filter.  If
     * <code>accept_any_tid</code> is #frost_false then only sections
     * with a TID equal to <code>specific_tid</code> will be matched
     * by this filter.
     */
    frost_bool accept_any_tid;

    /**
     * The specific TID (if required).
     */
    frost_uint specific_tid;

    /**
     * If <code>accept_any_tid_ext</code> is #frost_true, the
     * <code>specific_tid_ext</code> field is ignored and any TIDEXT is
     * considered a match for this filter.  If
     * <code>accept_any_tid_ext</code> is #frost_false, a section must
     * have a TIDEXT matching the value given in
     * <code>specific_tid_ext</code> in order to match the filter.
     */
    frost_bool accept_any_tid_ext;

    /**
     * The specific TIDEXT (if required).
     */
    frost_uint specific_tid_ext;

    /**
     * If <code>accept_any_version</code> is #frost_true, the
     * <code>version_to_exclude</code> field is ignored and sections
     * with any version number are permitted to match this filter.  If
     * <code>accept_any_version</code> is #frost_false, only sections
     * with a version number <em>not equal to</em>
     * <code>version_to_exclude</code> will be matched.
     */
    frost_bool accept_any_version;

    /**
     * The version to exclude (if required).
     */
    frost_uint version_to_exclude;

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

} ICE_FilterDescription;

/**
 * @brief Set up a new section filter.
 *
 * ICE_RequestFilter() is called by the client to tell the section
 * acquisition module to collect and pass on sections matching a given
 * #ICE_FilterDescription.
 * Sections matched by ICE filters must have valid CRC fields (but only when
 * the section_syntax_indicator field is set to '1') - Eclipse does not check
 * CRCs as it is assumed that the driver implementation has already done this.
 * A filter established with ICE_RequestFilter() remains active until it is
 * cancelled by a call to ICE_CancelFilter().
 *
 * It is implementation-dependent whether or not this function blocks if
 * filter callbacks are underway.
 *
 * @note The <code>new_filter</code> structure is only guaranteed to exist for
 *       the duration of this call.  I.e. the implementation must not try to
 *       dereference this pointer after ICE_RequestFilter() has returned.
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
ICE_FilterHandle* ICE_RequestFilter(ICE_FilterBankHandle* filter_bank,
                                    const ICE_FilterDescription* new_filter);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_SECTION_ACQUISITION_SIMPLE_H_INCLUDED*/
