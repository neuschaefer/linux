/**
 * Copyright 2002-4 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot portable interface to counting semaphores.
 *
 * @note If this interface is implemented using C++ and the platform is using
 * Cabot's new (as opposed to the system new), it is important that any
 * dynamic memory allocation done in the implementation of the frost_semaphore
 * interface must *not* be done with new.  This is because any call to new
 * will result in the use of a semaphore (since the memory pool access is
 * semaphore protected), resulting in recursive behaviour.
 */

#ifndef CABOT_FROST_FROST_SEMAPHORE_H_INCLUDED
#define CABOT_FROST_FROST_SEMAPHORE_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif
  
/**
 * @brief A semaphore.
 *
 * #FROST_Semaphore is an opaque type, to be manipulated only by the
 * functions specified in this header file.  Its implementation is specific
 * to the target platform, though this interface is portable.
 */
typedef struct FROST_Semaphore FROST_Semaphore;

/**
 * @brief Create a new counting semaphore.
 *
 * Create a new counting semaphore, which must eventually be released by calling
 * FROST_SemaphoreDestroy().
 *
 * @param count Initial count for the semaphore.  Note that support for an
 * unbounded count is not required.  Currently, all Eclipse semaphores have an
 * initial count of 1 or 0, although future extensions may require support for
 * a greater value of the initial count.
 *
 * @return pointer to #FROST_Semaphore on success, null on failure.
 */
FROST_Semaphore *FROST_SemaphoreCreate(frost_uint count);

/**
 * @brief Wait for a #FROST_Semaphore.
 *
 * @note Memory barriers: On SMP platforms (i.e. multiple-CPU or multi-core),
 *       this call implies an "acquire"-style memory barrier.  (Any decent OS
 *       implementation of semaphores will provide this for you, so you only
 *       need to worry about this if you're implementing semaphores from
 *       scratch). It has to guarantee that all memory reads and writes that
 *       are in the code after the semaphore is acquired, will appear to
 *       happen after the semaphore is acquired when viewed by another thread.
 *       (Unlike a full memory barrier, memory operations in the code before
 *       this call may be reordered to be executed after the semaphore is
 *       acquired).
 *
 * @param semaphore The #FROST_Semaphore instance to wait for.  May not be null.
 */
void FROST_SemaphoreWait(FROST_Semaphore *semaphore);

/**
 * @brief Signal a #FROST_Semaphore.
 *
 * @note Memory barriers: On SMP platforms (i.e. multiple-CPU or multi-core),
 *       this call implies a "release"-style memory barrier.  (Any decent OS
 *       implementation of semaphores will provide this for you, so you only
 *       need to worry about this if you're implementing semaphores from
 *       scratch).  It has to guarantee that all memory reads and writes that
 *       are in the code before the semaphore is signalled, will appear to
 *       happen before the semaphore is signalled when viewed by another
 *       thread.  (Unlike a full memory barrier, memory operations in the code
 *       after this call may be reordered to be executed before the semaphore
 *       is signalled).
 *
 * @param semaphore The #FROST_Semaphore instance to signal.  May not be null.
 */
void FROST_SemaphoreSignal(FROST_Semaphore *semaphore);

/**
 * @brief Destroy a #FROST_Semaphore.
 *
 * @param semaphore The #FROST_Semaphore instance to destroy.  May be
 * null in which case the call will have no effect.
 */
void FROST_SemaphoreDestroy(FROST_Semaphore *semaphore);


/***************************************************************************/
/**
 * @}
 * @name FROST extensions for other applications than Eclipse (Eg. MHP application).
 *
 * @note Eclipse does not need these functions. They must be implemented for applications 
 * which need extended FROST capabilities.
 * @{
 */
/***************************************************************************/

/**
 * @brief Wait for a #FROST_Semaphore.
 *
 * @note Memory barriers: On SMP platforms (i.e. multiple-CPU or multi-core),
 *       this call implies an "acquire"-style memory barrier.  (Any decent OS
 *       implementation of semaphores will provide this for you, so you only
 *       need to worry about this if you're implementing semaphores from
 *       scratch). It has to guarantee that all memory reads and writes that
 *       are in the code after the semaphore is acquired, will appear to
 *       happen after the semaphore is acquired when viewed by another thread.
 *       (Unlike a full memory barrier, memory operations in the code before
 *       this call may be reordered to be executed after the semaphore is
 *       acquired).
 *
 * @param semaphore The #FROST_Semaphore instance to wait for.  May not be null.
 */
/**
 * @brief Wait for a #FROST_Semaphore within the given timeout value.
 *
 * This routine will return immediately if the semaphore is free, or else 
 * block until the semaphore is signalled or until the given timeout value 
 * exceeds.  
 *
 * @note Memory barriers: On SMP platforms (i.e. multiple-CPU or multi-core),
 *       this call implies an "acquire"-style memory barrier.  (Any decent OS
 *       implementation of semaphores will provide this for you, so you only
 *       need to worry about this if you're implementing semaphores from
 *       scratch). It has to guarantee that all memory reads and writes that
 *       are in the code after the semaphore is acquired, will appear to
 *       happen after the semaphore is acquired when viewed by another thread.
 *       (Unlike a full memory barrier, memory operations in the code before
 *       this call may be reordered to be executed after the semaphore is
 *       acquired).
 *
 * @param semaphore The semaphore to wait for. If a null argument is passed 
 *        to this function, then #frost_false must be returned immediately. If 
 *        <code>semaphore</code> is signalled before timeout value exceeds 
 *        then #frost_true must be returned. Otherwise #frost_false should be returned.
 *
 * @param timeout Maximum timeout value to wait until the given <code>semaphore</code>
 *         is signalled. 
 *
 * @return On success, #frost_true. On failure, #frost_false.
 */
frost_bool FROST_SemaphoreWaitTimeout(FROST_Semaphore *semaphore, frost_uint32 timeout);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_FROST_FROST_SEMAPHORE_H_INCLUDED*/
