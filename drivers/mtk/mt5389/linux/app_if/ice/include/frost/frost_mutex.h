/**
 * Copyright 2002-10 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot portable interface to mutual exclusion semaphores.
 *
 * On platforms where mutexes (as distinct from counting semaphores) are
 * not supported directly, a mutex can be implemented using a counting
 * semaphore.  On other platforms, mutual exclusion semaphores can be
 * provided more efficiently than general counting semaphores.  One further
 * difference of this mutex interface over the FROST counting semaphore interface
 * is that while operations on counting semaphores may be mixed freely between
 * threads, a mutex must only be unlocked by the thread which locks it.  
 *
 * @internal A high-quality implementation of this interface would
 *           check for that condition in debug builds and raise a diagnostic if
 *           it was violated.  This checking is likely to hurt performance if
 *           used in release builds.
 */

#ifndef CABOT_FROST_FROST_MUTEX_H_INCLUDED
#define CABOT_FROST_FROST_MUTEX_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif
  
/**
 * @brief #FROST_Mutex abstracts the concept of a mutual
 * exclusion semaphore.
 *
 * This is an opaque type, to be manipulated only by the functions specified 
 * in this header file.  Its implementation is specific to the target
 * platform, though this interface is portable.
 */
typedef struct FROST_Mutex FROST_Mutex;


/**
 * @brief Create a new mutual exclusion semaphore.
 *
 * Creates a new mutual exclusion semaphore, which must eventually be
 * released by calling FROST_MutexDestroy().
 *
 * Failure can only be caused by a shortage of OS resources (either
 * memory or semaphores), and will be signalled by returning a null pointer.
 *
 * @return A new mutual exclusion semaphore.
 */
FROST_Mutex *FROST_MutexCreate(void);

/**
 * @brief Lock a #FROST_Mutex.
 *
 * Lock a #FROST_Mutex created with FROST_MutexCreate().  This
 * routine will return immediately if the mutex is free, or else block
 * until the mutex is released by the current holder.  After calling
 * FROST_MutexLock(), a task should later call FROST_MutexUnlock().
 * (In C++ code, this can be arranged using the RAII idiom.)
 *
 * Note that if the mutex is already locked by the current thread then
 * the behaviour is undefined, and deadlock is possible.
 *
 * @note Memory barriers: On SMP platforms (i.e. multiple-CPU or multi-core),
 *       this call implies an "acquire"-style memory barrier.  (Any decent OS
 *       implementation of mutexes will provide this for you, so you only need
 *       to worry about this if you're implementing mutexes from scratch).
 *       It has to guarantee that all memory reads and writes that are in the
 *       code after the mutex is taken, will appear to happen after the mutex
 *       is taken when viewed by another thread.  (Unlike a full memory
 *       barrier, memory operations in the code before this call may be
 *       reordered to be executed after the mutex is taken).
 *
 * @param mutex The mutex to lock. A null argument must not be passed to
 *        this function.
 */
void FROST_MutexLock(FROST_Mutex *mutex);

/**
 * @brief Unlock a #FROST_Mutex.
 *
 * Unlock a #FROST_Mutex which has been locked by a previous call to
 * FROST_MutexLock() in the current thread.
 *
 * @note Memory barriers: On SMP platforms (i.e. multiple-CPU or multi-core),
 *       this call implies a "release"-style memory barrier.  (Any decent OS
 *       implementation of mutexes will provide this for you, so you only need
 *       to worry about this if you're implementing mutexes from scratch).  It
 *       has to guarantee that all memory reads and writes that are in the
 *       code before the mutex is released, will appear to happen before the
 *       mutex is released when viewed by another thread.  (Unlike a full
 *       memory barrier, memory operations in the code after this call may be
 *       reordered to be executed before the mutex is released).
 *
 * @param mutex The mutex to unlock.  A null argument must not be passed
 *        to this function.
 */
void FROST_MutexUnlock(FROST_Mutex *mutex);

/**
 * @brief Destroy a #FROST_Mutex.
 *
 * Destroy a #FROST_Mutex created with FROST_MutexCreate() and release
 * all associated resources.
 *
 * The caller must ensure the mutex is unlocked before calling this function.
 *
 * @param mutex The mutex to destroy.  Calling this with a null argument is
 *        harmless, and does nothing.
 */
void FROST_MutexDestroy(FROST_Mutex *mutex);


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
 * @brief Check a #FROST_Mutex is held by the current thread.
 *
 * If properly implemented, this function should check that the specified
 * mutex is owned by the calling thread.  If so, it should return #frost_true.
 * If not, it should return #frost_false, and Cabot's assertion mechanism will
 * report the error to the programmer.
 *
 * (If you do not record the owner of the mutex, then just checking if the
 * mutex is locked or not is helpful, and will catch some bugs).
 *
 * @param mutex The mutex to check.  May be NULL (in which case this function
 *              should return #frost_false).
 * @return #frost_true if not implemented, or if the mutex may be owned by the
 *         calling thread.  #frost_false only if you are certain that the
 *         mutex is not owned by the calling thread.
 */
frost_bool FROST_MutexCheckOwner(FROST_Mutex *mutex);


/**
 * @brief Create a new recursive mutual exclusion semaphore.
 *
 * Creates a new recursive mutual exclusion semaphore, which must eventually be
 * released by calling FROST_MutexDestroy().
 *
 * Failure can only be caused by a shortage of OS resources (either
 * memory or semaphores), and will be signalled by returning a null pointer.
 *
 * Note: This function is provided for integrating with customers and is 
 * not for general use in Cabot.
 *
 * @return A new recursive mutual exclusion semaphore.
 */
FROST_Mutex *FROST_MutexCreateRecursive(void);


/**
 * @brief Try to lock a #FROST_Mutex within the given timeout value.
 *
 * Lock a #FROST_Mutex created with FROST_MutexCreate().  This
 * routine will return immediately if the mutex is free, or else block
 * until the mutex is released by the current holder or until the given 
 * timeout value exceeds.  After calling FROST_MutexLock(), a task 
 * should later call FROST_MutexUnlock().(In C++ code, this can be 
 * arranged using the RAII idiom.)
 *
 * Note that if the mutex is already locked by the current thread then
 * the behaviour is undefined, and deadlock is possible.
 *
 * @note Memory barriers: On SMP platforms (i.e. multiple-CPU or multi-core),
 *       this call implies an "acquire"-style memory barrier.  (Any decent OS
 *       implementation of mutexes will provide this for you, so you only need
 *       to worry about this if you're implementing mutexes from scratch).
 *       It has to guarantee that all memory reads and writes that are in the
 *       code after the mutex is taken, will appear to happen after the mutex
 *       is taken when viewed by another thread.  (Unlike a full memory
 *       barrier, memory operations in the code before this call may be
 *       reordered to be executed after the mutex is taken).
 *
 * @param mutex The mutex to lock. If a null argument is passed to this function,
 *        then #frost_false must be returned immediately. If lock is acquired for the
 *        <code>mutex</code> before timeout value exceeds then #frost_true
 *        must be returned. Otherwise #frost_false should be returned.
 *
 * @param timeout Maximum timeout value to wait until the given <code>mutex</code>
 *         lock is acquired. 
 *
 * @return On success, #frost_true. On failure, #frost_false.
 */
frost_bool FROST_MutexLockTimeout(FROST_Mutex *mutex, frost_uint32 timeout);


/**
 * @brief Check a #FROST_Mutex is locked or not.
 *
 * This function should check that the specified mutex is locked or not.
 * If so, it should return #frost_true. If not, it should return #frost_false.
 *
 * @param mutex The mutex to check.  May be NULL (in which case this function
 *              should return #frost_false).
 *
 * @return #frost_true if not implemented, or if the mutex is really locked.
 *             #frost_false if mutex is not locked.
 */
frost_bool FROST_MutexIsLocked(FROST_Mutex *mutex);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_FROST_FROST_MUTEX_H_INCLUDED*/
