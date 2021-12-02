/**
 * @file
 * @brief Implementation of FROST portable interface to condition variables.
 *
 * Copyright 2002 Cabot Communications.  All rights reserved.
 *
 */
#ifndef CABOT_FROST_FROST_CONDVAR_H_INCLUDED
#define CABOT_FROST_FROST_CONDVAR_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "frost/frost_mutex.h"

#ifdef __cplusplus
extern "C" {
#endif

 /**
 * @brief A conditional variable.
 *
 * #FROST_CondVar is an opaque type, to be manipulated only by the
 * functions specified in this header file.  Its implementation is specific
 * to the target platform, though this interface is portable.
 */
 typedef struct FROST_CondVar FROST_CondVar;
 

/**
 * @brief Create a new conditional variable.
 *
 * Create a new conditional variable, which must eventually be released by calling
 * FROST_CondVarDestroy().
 *
 * @return pointer to #FROST_CondVar on success, null on failure.
 */
FROST_CondVar* FROST_CondVarCreate();


/**
 * @brief Destroy a #FROST_CondVar.
 *
 * @param condvar The #FROST_CondVar instance to destroy.  May be
 * null in which case the call will have no effect.
 */
frost_bool FROST_CondVarDestroy(FROST_CondVar *condvar);

/**
 * @brief Wake at least one thread waiting on the specified conditional variable.
 *
 * This function will signal the given conditional variable thus will unblock a thread
 * which is waiting for the conditional variable to be signalled.
 *
 * @param condvar The #FROST_CondVar instance to be signalled.  If value is null
 *                           then function returns #frost_false immediately.
 * @return #frost_true, On success.
 *             #frost_false, On failure.
 */
frost_bool FROST_CondVarSignal(FROST_CondVar * condvar);


/**
 * @brief Wake all threads waiting for the conditional variable to be signalled.
 *
 * This function will signal the given conditional variable thus will unblock all threads
 * waiting for the conditional variable to be signalled.
 *
 * @param condvar The #FROST_CondVar instance to be signalled.  If value is null
 *                           then function returns #frost_false immediately.
 * @return #frost_true, On success.
 *             #frost_false, On failure.
 */
frost_bool FROST_CondVarBroadcast(FROST_CondVar *condvar);


/**
 * @brief Causes the calling thread to wait for the specified condition variable to be signaled
 *           or broadcasted.
 *
 * Calling thread must lock mutex before calling this function, if not #frost_false will be returned.
 * This function will unlock given mutex until the conditional variable is signalled. After that it will
 * wait until mutex lock is acquired again.
 *
 * @param condvar The #FROST_CondVar instance to wait for.  If value is null
 *                           then function returns #frost_false immediately.
 * @param mutex The #FROST_CondVar instance to wait for.  If value is null
 *                           then function returns #frost_false immediately.
 * @return #frost_true, On success
 *              #frost_false, On failure or invalid parameters.
 */
frost_bool FROST_CondVarWait(FROST_CondVar *condvar, FROST_Mutex * mutex);


/**
 * @brief Causes the calling thread to wait for the specified condition variable to be signaled
 *           or broadcasted in the given time period.
 *
 * Calling thread must lock mutex before calling this function, if not #frost_false will be returned.
 * This function will unlock given mutex until the conditional variable is signalled. After that it will
 * wait until mutex lock is acquired again.
 *
 * @param condvar The #FROST_CondVar instance to wait for.  If value is null
 *                           then function returns #frost_false immediately.
 * @param mutex The #FROST_CondVar instance to wait for.  If value is null
 *                           then function returns #frost_false immediately.
 * @param timeout The maximum waiting time in millisecond. If the timeout value is 0, 
 *                          it means INFINITE waiting.
  * @return #frost_true, On success
 *              #frost_false, On failure or invalid parameters or timeout value exceeds.
 */
frost_bool FROST_CondVarWaitTimeout(FROST_CondVar *condvar,
                                   FROST_Mutex * mutex,
                                   frost_uint32 timeout);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_FROST_FROST_CONDVAR_H_INCLUDED*/

