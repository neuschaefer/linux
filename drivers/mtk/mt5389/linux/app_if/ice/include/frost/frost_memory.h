/**
 * Copyright 2002-4 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Interface to FROST memory allocation functionality.
 *
 * This file describes the FROST interface from/to the memory allocation
 * functionality.  In brief, this interface consists of a means to acquire 
 * a memory partition and allocate memory from it.
 *
 * @note This interface is only required to be implemented if Cabot's new is
 * being used, as opposed to the system new.  A consequence of this is that
 * any dynamic memory allocation used in this implementation (or the
 * implementation of frost_semaphore) must *not* use new.
 */

#ifndef CABOT_FROST_FROST_MEMORY_H_INCLUDED
#define CABOT_FROST_FROST_MEMORY_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A handle to a Partition.
 *
 * #FROST_PartitionHandle is a handle to the memory partition from which memory is
 * allocated by FROST_PartitionAlloc().  This is an opaque type.
 */
typedef struct FROST_PartitionHandle FROST_PartitionHandle;


/**
 * Create a memory heap partition of a given size.
 *
 * @param size_in_bytes Desired size of partition.
 *
 * @return A pointer to the new handle on success, else null on failure
 */
FROST_PartitionHandle* FROST_PartitionCreate(frost_ulong size_in_bytes);


/**
 * Allocate memory from a partition previously created by a successful
 * call to FROST_PartitionCreate().
 *
 * @param partition Partition from which to allocate memory
 *
 * @param size_in_bytes Size of the buffer to allocate
 *
 * @return a pointer to the allocated memory on success, else null on failure.
 */
void* FROST_PartitionAlloc(FROST_PartitionHandle* partition,
                         frost_ulong size_in_bytes);


/**
 * Free allocated memory back to a partition.
 *
 * @note The implementation of FROST_PartitionAlloc() is expected to manage any
 * information that will be needed by FROST_PartitionFree().  For example, it may
 * need to allocate a small amount of extra memory to store the size of the
 * buffer (though some small memory allocators can avoid this overhead).
 *
 * @param partition The partition from which the memory was allocated (and
 *        hence will be freed to).
 *
 * @param buffer A pointer to a memory buffer previously allocated by
 *        FROST_PartitionAlloc(), or null.
 */
void FROST_PartitionFree(FROST_PartitionHandle* partition, void* buffer);


/**
 * Destroy a memory partition, returning all memory assigned to that partition
 * to the system.
 *
 * @param partition pointer to #FROST_PartitionHandle returned by a
 *        successful call to FROST_PartitionCreate().
 */
void FROST_PartitionDestroy(FROST_PartitionHandle* partition);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_FROST_FROST_MEMORY_H_INCLUDED*/
