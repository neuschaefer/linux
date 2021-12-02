/**
 * Copyright 2002-5 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Common part of interface for section filtering and acquisition.
 *
 * This file describes the common part of the ICE interface to the
 * section acquisition module.  In brief, this interface consists of calls
 * to set up and release section filters, and callbacks used by the section
 * acquisition module to pass data back to its client.
 *
 * This file contains only the common part, which includes initialisation,
 * termination, cancelling filters, and some common types.  All ICE
 * implementers must implement the functions defined in this file.
 *
 * The actual definition of a filter and the function to create a filter is
 * specified in a different header file.  There are two alternative APIs to
 * create filters:
 *   - ice_section_acquisition_simple.h describes a simple API which is
 *     sufficient for most products, and is easier to implement.
 *   - ice_section_acquisition_posneg.h describes an API for positive/negative
 *     section filtering, intended for enhanced platforms where Eclipse is
 *     integrated with non-Cabot software, and the non-Cabot software requires
 *     a more complex section filtering API.
 * You can implent either or both of the APIs.
 *
 * If you choose to implement ice_section_acquisition_posneg.h only, Cabot will
 * use our implementation of ice_section_acquisition_simple.h that just calls
 * the ice_section_acquisition_posneg.h functions.
 */

#ifndef CABOT_ICE_ICE_SECTION_ACQUISITION_COMMON_H_INCLUDED
#define CABOT_ICE_ICE_SECTION_ACQUISITION_COMMON_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Identifies a specific filter bank.
 *
 * #ICE_FilterBankHandle is an opaque type specific to the target
 * platform.  It is used to identify the demux from which to filter
 * sections from.  This allows multiple transport streams to be
 * processed simultaneously.
 */
typedef struct ICE_FilterBankHandle ICE_FilterBankHandle;

/**
 * @brief Identifies a specific filter.
 *
 * #ICE_FilterHandle is an opaque type specific to the target
 * platform.  It is used to identify the specific filter within the
 * demux that a filter is established.
 */
typedef struct ICE_FilterHandle ICE_FilterHandle;

/**
 * @brief Acquire a buffer from the client to store a received section.
 *
 * This callback is used by the section acquisition subsystem to acquire
 * a buffer (a contiguous block of memory) in which to store sections to
 * be passed back to the client code.
 *
 * If memory is not available at the time this request is made, the
 * callback should return a null pointer.  In this case, the implementation
 * should discard the section (unless required by another filter).
 *
 * @note In practice, after a channel change to a service containing high data
 * rate DSMCC PIDs the rate of null pointer returns from this callback will be
 * quite high, so it makes sense for the implementation to minimise the amount
 * of processing carried out on the section prior to invoking the
 * ICE_RequestBufferCallback().
 *
 * For each successful call to ICE_RequestBufferCallback() the implementation
 * must make exactly one subsequent call to either ICE_SectionReadyCallback()
 * or ICE_ReleaseBufferCallback().
 *
 * @note This callback will not call ICE_RequestFilter() or
 *       ICE_CancelFilter().  ICE implementations may deadlock if this
 *       rule is broken.  (It is common for an ICE implementation to have
 *       a non-recursive mutex that is held for the duration of this callback,
 *       and is also used to protect those functions).
 *
 * @param user_data      The user_data as passed in the ICE_FilterDescription
 *                       in the ICE_RequestFilter() call which established the
 *                       filter which was matched by this section.
 * @param section_length The minimum amount of buffer space required, in bytes.
 *
 * @return On success, a non-null pointer to a buffer of at least
 *         <code>section_length</code> bytes.  On failure, a null pointer.
 */
typedef frost_uint8* (*ICE_RequestBufferCallback)(void* user_data,
                                                  frost_uint section_length);

/**
 * @brief Pass data from the section acquisition module to a client.
 *
 * This callback is used to pass sections from the section acquisition
 * module to a client which has requested them by calling ICE_RequestFilter().
 * The complete section is passed, including its header.
 *
 * @note This callback will not call ICE_RequestFilter() or
 *       ICE_CancelFilter().  ICE implementations may deadlock if this
 *       rule is broken.  (It is common for an ICE implementation to have
 *       a non-recursive mutex that is held for the duration of this callback,
 *       and is also used to protect those functions).
 *
 * @param user_data The user_data as passed in the ICE_FilterDescription
 *                  in the ICE_RequestFilter() call which established the
 *                  filter which was matched by this section.
 * @param pid       The PID on which this section was received.
 * @param data      The originally returned pointer value from
 *                  ICE_RequestBufferCallback(), which now points to the
 *                  raw section data.
 */
typedef void (*ICE_SectionReadyCallback)(void* user_data, frost_uint pid,
                                         frost_uint8* data);

/**
 * @brief Release buffer provided for a section on acquisition failure.
 *
 * This callback is used to release the buffer provided by
 * ICE_RequestBufferCallback() on a section acquisition failure.  It
 * should NOT be used when section acquisition has been successful.
 *
 * @note This callback will not call ICE_RequestFilter() or
 *       ICE_CancelFilter().  ICE implementations may deadlock if this
 *       rule is broken.  (It is common for an ICE implementation to have
 *       a non-recursive mutex that is held for the duration of this callback,
 *       and is also used to protect those functions).
 *
 * @param user_data The user_data as passed in the ICE_FilterDescription
 *                  in the ICE_RequestFilter call which established the
 *                  filter which was matched by this section.
 * @param data      The originally returned pointer value from
 *                  ICE_RequestBufferCallback().
 */
typedef void (*ICE_ReleaseBufferCallback)(void* user_data, frost_uint8* data);


/**
 * @brief Cancel a filter previously established by ICE_RequestFilter().
 *
 * ICE_CancelFilter() is called by the client to cancel a filter
 * established by a previous call to ICE_RequestFilter().
 *
 * If one of the section filter callbacks is in progress for the specified
 * filter, this function must block until that callback returns.  If a buffer
 * has been allocated for the specified filter (or if a call to
 * ICE_RequestBufferCallback() for that filter is in progress) then this
 * function must block until that buffer has been returned using a call to
 * ICE_ReleaseBufferCallback() or ICE_SectionReadyCallback().  (It is
 * implementation-dependent whether or not this function blocks if callbacks
 * for a different filter are underway).
 *
 * There must be no calls to any of the callbacks for the filter after this
 * function returns.
 *
 * @param filter A filter handle resulting from a call to ICE_RequestFilter().
 *        This parameter may be null, in which case the function call has no
 *        effect.
 */
void ICE_CancelFilter(ICE_FilterHandle* filter);

/**
 * @brief Initialise a specific filter bank.
 *
 * ICE_FilterBankInit() is called by the client to initialise an ICE
 * filter bank.
 *
 * This function will not be called twice for the same source,
 * unless there is a call to #ICE_FilterBankReset() in between.
 *
 * @param source Identifies the source transport stream.
 *
 * @return On success a pointer to an #ICE_FilterBankHandle used to
 *         reference the FilterBank in subsequent calls. On failure a null
 *         pointer.
 */
ICE_FilterBankHandle* ICE_FilterBankInit(ICE_DeviceSource source);

/**
 * @brief Reset a specific filter bank.
 *
 * ICE_FilterBankReset() is called by the client to reset an ICE
 * filter bank and release all resources.  After the call, ICE_FilterBankInit()
 * can be called again.
 *
 * After a call to this function, the #ICE_FilterBankHandle returned by the
 * call to #ICE_FilterBankInit() with the same source is invalid and
 * will not be used again.
 *
 * Will not be called unless #ICE_FilterBankInit() has been previously
 * (and successfully) called with the same device_number.  Will not be called
 * twice for the same device number, unless there is a call to
 * #ICE_FilterBankInit() in between.
 *
 * Will not be called if any filters are active (i.e. if any #ICE_FilterHandle
 * returned by ICE_RequestFilter() has not been cancelled by a call to
 * #ICE_CancelFilter()).
 *
 * @param source Identifies the source transport stream.
 */
void ICE_FilterBankReset(ICE_DeviceSource source);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_SECTION_ACQUISITION_COMMON_H_INCLUDED*/
