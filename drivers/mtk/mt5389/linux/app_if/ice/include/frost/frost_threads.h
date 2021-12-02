/**
 * Copyright 2002-4, 2010 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief FROST portable interface to thread lifetime functionality.
 *
 * The threading assumptions of Cabot code are (almost) minimal.  We
 * assume the ability to create a small number of threads, and
 * assume the ability to dynamically create/destroy threads (but see
 * note below).  Cabot also assume a pre-emptive (as opposed to
 * cooperative) scheduler.  Threads are created and destroyed, and
 * synchronisation is achieved using semaphores and message queues.
 *
 * @note Thread synchronization is provided by facilities in the header
 *       files frost_semaphore.h, frost_mutex.h and frost_message_queue.h
 *
 * @internal The need for the platform to support dynamic thread creation and
 *       destruction could be avoided using a thread pool behind the scenes, as
 *       Eclipse thread functions never explicitly terminate the thread, but
 *       rely instead on the thread cleaning up when the thread function exits.
 */

#ifndef CABOT_FROST_FROST_THREADS_H_INCLUDED
#define CABOT_FROST_FROST_THREADS_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Thread priorities as accepted by FROST_ThreadCreate().
 */
typedef enum FROST_TaskPriority
{
    FROST_lowest_priority,
    FROST_low_priority,
    FROST_medium_priority,
    FROST_high_priority,
    FROST_highest_priority,
    FROST_realtime_priority
} FROST_TaskPriority;

/**
 * @brief Opaque type used to identify a FROST Thread.
 *
 * #FROST_Thread is an opaque type, to be manipulated only by the
 * functions specified in this header file.  Its implementation is specific
 * to the target platform, though this interface is portable.
 */
typedef struct FROST_Thread FROST_Thread;

/**
 * @brief Function pointer to be passed to FROST_ThreadCreate().
 */
typedef void (*FROST_ThreadFunction)(void *user_data);

/**
 * @brief Create a new thread and have it start execution.
 *
 * The thread exits when <code>function_to_execute</code> returns. No
 * method is supplied to terminate the thread prematurely, as that
 * behaviour cannot portably be implemented safely.
 *
 * Failure can only be caused by a shortage of OS resources (either
 * memory or threads/tasks), and will be signalled by returning a null pointer.
 *
 * @note Memory barriers: On SMP platforms (i.e. multiple-CPU or multi-core),
 *       this call implies a "release"-style memory barrier in this thread,
 *       and an "acquire"-style memory barrier in the created thread.  It has
 *       to guarantee that all memory reads and writes that are in the
 *       code before this call, will appear to have happened when viewed by
 *       the created thread.
 *
 * @param function_to_execute The function which will be executed by the
 *        thread, and will return to indicate that the thread can terminate.
 *        The argument passed to the <code>function_to_execute</code> will be
 *        the user_data passed to the FROST_ThreadCreate() call.
 *
 * @param user_data This is sufficient to allow clients to pass however much
 *        context the <code>function_to_execute</code> may need, by passing a
 *        pointer to that context information.
 *
 * @param stacksize_hint A recommendation for how much stack space should be
 *        allocated, in bytes.  Note that some platforms may not be able to
 *        respect this, and others such as certain RISC-based systems may
 *        adjust it upwards to reflect larger than typical memory usage.
 *
 * @param priority_hint Priorities can be used to tune performance of a
 *        system, but should not be relied upon to force execution order
 *        as this cannot be done sufficiently portably.
 *
 * @param name_for_thread For platforms which support thread names, this
 *        will be passed to the underlying platform; this can aid greatly
 *        in the use of debuggers etc.  If used, this name will be copied.
 *
 * @return On success, a handle to the thread, which should later be passed to
 *         FROST_ThreadWaitForExit(). On failure, a null pointer.
 */
FROST_Thread *FROST_ThreadCreate(FROST_ThreadFunction function_to_execute,
                             void *user_data,
                             frost_uint stacksize_hint,
                             FROST_TaskPriority priority_hint,
                             const char *name_for_thread);

/**
 * @brief Wait for a thread to finish and release all (FROST) resources
 * associated with that thread.
 *
 * This can legally be called many times for a particular thread handle.
 * Waiting for exit from multiple parallel threads blocks as expected.
 * Calling this function after the thread has exited will return immediately.
 * Eclipse will not call this function after the call to FROST_ThreadDelete() for
 * <code>thread_to_wait_for</code>.
 *
 * @note Memory barriers: On SMP platforms (i.e. multiple-CPU or multi-core),
 *       this call implies a "release"-style memory barrier in
 *       thread_to_wait_for just before it finishes executing,
 *       and an "acquire"-style memory barrier in the current thread.  It has
 *       to guarantee that all memory reads and writes done by
 *       thread_to_wait_for will appear to have happened when viewed by the
 *       calling thread following this function's return.  It also has to
 *       guarantee that all memory reads and writes done by the calling thread
 *       after the call to this function will never appear to have happened
 *       when viewed from thread_to_wait_for.
 *
 * @param thread_to_wait_for Pointer to the #FROST_Thread to wait
 *        for.  Passing a null pointer to this function has no effect.
 */
void FROST_ThreadWaitForExit(FROST_Thread *thread_to_wait_for);

/**
 * @brief Delete a terminated thread.
 *
 * Frees the memory used by a FROST_Thread handle.  After this call the
 * FROST_Thread handle is invalid and will not be used again.
 *
 * It is impossible to portably "kill" a running thread; that is NOT what
 * this function does.
 *
 * There are two cases:
 *
 * 1) If the thread_to_delete does not specify the current thread, then
 *    that thread must have already terminated (i.e. Cabot will have called 
 *    FROST_ThreadWaitForExit() on the thread).
 *
 * 2) If the thread_to_delete specifies the current thread, then obviously the
 *    thread is still running.  In this case it is platform-dependent whether
 *    or not this function returns.  (Many RTOSs allow a thread to delete
 *    itself, in which case this function will delete the thread and will not
 *    return.  Alternatively, this function may just free the memory used for
 *    the FROST_Thread, or request that memory be automatically freed when the
 *    thread exits - in that case, this function will return and the thread
 *    will continue to exist until the FROST_ThreadFunction returns).
 *
 * @param thread_to_delete Pointer to the #FROST_Thread to delete. Passing a null
 *        pointer to this function has no effect.
 */
void FROST_ThreadDelete(FROST_Thread *thread_to_delete);

/**
 * @brief Change the priority of the relevant thread.
 *
 * Set the priority of the thread identified by the thread_id
 * to the priority passed in by priority_hint. If the thread_id
 * is NULL then set the current threads priority.
 *
 * @param thread_id Pointer to the #FROST_Thread handle identifying the
 *        thread. Passing a null pointer shall mean the current thread is the
 *        target of this operation.
 *
 * @param priority_hint The required new thread priority.
 *
 */
void FROST_ThreadSetPriority(FROST_Thread* thread_id, FROST_TaskPriority priority_hint);


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
 * @brief Do platform dependent native initializations before using 
 *           any FROST_thread function.
 *
 * @return #frost_true On success,
 *             #frost_false On failure
 */
frost_bool FROST_ThreadInit(void);


/**
 * @brief Get the thread ID for the current thread.
 *
 * The thread ID is an opaque number that identifies the thread.  Each thread
 * will get a different ID.  If a single thread calls this function many times,
 * it will always get the same ID each time.
 *
 * When the #FROST_ThreadFunction function returns, the ID can be re-used.
 *
 * FROST_THREAD_ID_INVALID is not a valid thread ID and must not be returned by
 * this function.
 *
 * @return The ID of the calling thread, this ID should not be used for referencing
 * threads or calling functions on threads.
 */
frost_uintptr FROST_ThreadGetId(void);

/**
 * @brief Get the FROST_Thread structure that identifies the current thread.
 *
 *
 * @return The reference of the calling thread, this refence can be used for referencing
 * threads or calling functions on threads.
 */
FROST_Thread *FROST_ThreadSelf(void);


/**
 * @brief Get the given priority to the relevant thread.
 *
 * Get the priority of the thread identified by the thread_id.
 * If the thread_id is NULL then get the current threads priority.
 *
 * @param thread_id Pointer to the #FROST_Thread handle identifying the
 *        thread. Passing a null pointer shall mean the current thread is the
 *        target of this operation.
 *
 * @return The thread priority value of the thread identified by thread_id.
 */
FROST_TaskPriority FROST_ThreadGetPriority(FROST_Thread* thread_id);


/**
 * @brief Set current threads thread local variable value to the given new_value
 *
 * @param new_value Value to be set for current threads (calling thread) thread
 *                              local variable.
 *
 */
void FROST_ThreadLocalSet(void * new_value);


/**
 * @brief Get the value of current threads thread local variable.
 *
 * @return The value of current threads (calling thread) thread local variable.
 */
void * FROST_ThreadLocalGet(void);


/**
 * @brief Set the execution state of the thread identified by thread_id as suspended.
 *
 * @param thread_id Pointer to the #FROST_Thread handle identifying the
 *        thread. Passing a null pointer shall mean the current thread is the
 *        target of this operation.
 *
 * @return #frost_true, if succeded to suspend the thread.
 *              #frost_false, if fails in suspend operation.
 */
frost_bool FROST_ThreadSuspend(FROST_Thread *thread_id);


/**
 * @brief Resume the execution of the suspended thread identified by thread_id.
 *
 * @param thread_id Pointer to the #FROST_Thread handle identifying the
 *        thread. Passing a null pointer shall mean the current thread is the
 *        target of this operation.
 *
 * @return #frost_true, if succeded to resume the thread.
 *              #frost_false, if fails in resume operation.
 */
frost_bool FROST_ThreadResume(FROST_Thread *thread_id);


/**
 * @brief Current thread yields the ownership of CPU to the next thread
 *           in the waiting queue and goes to the end of the waiting queue.
 *
  */
void FROST_ThreadYield(void);

void list_all_thread(void);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_FROST_FROST_THREADS_H_INCLUDED*/
