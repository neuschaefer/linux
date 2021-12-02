/**
 * Copyright 2002-4 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Interface to message queues.
 *
 * This file describes the FROST interface to message queues.  Briefly, this
 * interface consists of calls to manage the lifecycle of queues, and send
 * and receive fixed-size messages on them, with the choice of blocking or
 * non-blocking behaviour.
 */

#ifndef CABOT_FROST_FROST_MESSAGE_QUEUE_H_INCLUDED
#define CABOT_FROST_FROST_MESSAGE_QUEUE_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif
  
/**
 * @brief FROST representation of a message queue.
 *
 * #FROST_MessageQueue is an opaque type, to be manipulated only by the
 * functions specified in this header file.  Its implementation is specific
 * to the target platform, though this interface is portable.
 */
typedef struct FROST_MessageQueue FROST_MessageQueue;

/**
 * @brief The type of message.
 * 
 * Provides unique identification of the particular type of message.
 */
typedef frost_uint32 FROST_MessageType;
    
/**
 * @brief Message payload.
 *
 * The payload of the message, which can contain either a pointer
 * or an unsigned 32-bit integer value.  Which field of the union is
 * used will be dependent upon the particular message.
 */
union FROST_MessageField
{
    /**
     * A pointer to message payload - used where the message carries a
     * struct or array etc;
     */
    void *pointer;
    
    /**
     * The message payload, where this is an integral value.
     */
    frost_uint32 integer;
};

/**
 * @brief The format of messages passed on the queues.
 *
 * Each #FROST_Message allows the user to pass a type code and two data fields,
 * each of which is a #FROST_MessageField so that it can contain either a pointer
 * or an unsigned 32-bit integral value.
 */
typedef struct FROST_Message
{
    /**
     * The type of the message
     */
    FROST_MessageType type;
    
    /**
     * First message data field
     */
    union FROST_MessageField payload1;
    
    /**
     * Second message data field
     */
    union FROST_MessageField payload2;

} FROST_Message;

/**
 * @brief Create a #FROST_MessageQueue.
 *
 * Create a #FROST_MessageQueue, which must eventually be released by calling
 * FROST_MessageQueueDestroy().
 *
 * Failure can only be caused by a shortage of OS resources (either memory or
 * message queues).
 *
 * @param max_queue_size A hint giving the number of queued messages that the
 *                       queue is to support.  Total capacity in bytes is,
 *                       therefore, <code>sizeof(FROST_Message) * 
 *                       max_queue_size</code>.
 *
 * @return On success, a non-null pointer which can be used in calls to send
 *         and receive messages.  On failure, a null pointer.
 */
FROST_MessageQueue *FROST_MessageQueueCreate(frost_uint max_queue_size);


/**
 * @brief Send a #FROST_Message to a #FROST_MessageQueue, blocking if necessary.
 *
 * Send a #FROST_Message to a #FROST_MessageQueue.  If the message queue is
 * currently full (as is possible on some systems), this call will block for
 * the duration given by <code>msecs_to_wait</code>.
 *
 * The FROST message layer makes no attempt to free any dynamically allocated
 * memory or other resources pointed to by the message; the receiver of the
 * message must do this in an appropriate manner.
 *
 * There may be multiple threads simultaneously calling FROST_MessageSend() on
 * the same message queue.  The implementation of this function must be thread
 * safe.  In particular, you must make sure that one thread, calling
 * FROST_MessageSend() with a large or infinite timeout, cannot block another
 * thread calling FROST_MessageSend() on the same message queue with a much
 * smaller (or zero) timeout.
 *
 * @note Memory barriers: On SMP platforms (i.e. multiple-CPU or multi-core),
 *       this call implies a "release"-style memory barrier.  It has to
 *       guarantee that all memory reads and writes that are in the
 *       code before the message is sent, will appear to happen before the
 *       message is received when viewed by the receiving thread.
 *
 * @param queue_to_send_to Returned from a call to FROST_MessageQueueCreate() -
 *                         must not be null.
 * @param message_to_send  The message to be sent - must not be null.
 * @param msecs_to_wait    Indicates the maximum number of milliseconds to
 *                         block for.  A value of -1 indicates an infinite
 *                         wait, and 0 indicates no wait.
 *
 * @return #frost_true if the message was sent, otherwise #frost_false.
 */
frost_bool FROST_MessageSend(FROST_MessageQueue *queue_to_send_to, 
                         const FROST_Message *message_to_send,
                         frost_int msecs_to_wait);


/**
 * @brief Read a message from a given #FROST_MessageQueue, blocking if necessary.
 *
 * Read a message from a given #FROST_MessageQueue, blocking the calling thread
 * if necessary.  When there is a message waiting, it is returned via the
 * <code>returned_message</code> argument.
 *
 * There may be two threads simultaneously calling FROST_MessageGet() on
 * the same message queue.  The implementation of this function must be thread
 * safe.  In particular, you must make sure that one thread calling
 * FROST_MessageGet() with a large timeout cannot block another thread calling
 * FROST_MessageGet() on the same message queue with a timeout of 0.
 *
 * @note Memory barriers: On SMP platforms (i.e. multiple-CPU or multi-core),
 *       this call implies an "acquire"-style memory barrier.  It has to
 *       guarantee that all memory reads and writes that are in the
 *       code after the message is received, will appear to happen after the
 *       message was sent when viewed by the sending thread.
 *
 * @param queue_to_read_from Returned from a call to FROST_MessageQueueCreate() -
 *                           must not be null.
 * @param returned_message   Output parameter that points to storage for the
 *                           received message - must not be null.
 * @param msecs_to_wait      Indicates the maximum number of milliseconds to
 *                           block for.  A value of -1 indicates an infinite
 *                           wait, and 0 indicates no wait.
 *
 * @return #frost_true if a message was received successfully, otherwise
 *         #frost_false if no message was available or an error occurred.
 */
frost_bool FROST_MessageGet(FROST_MessageQueue *queue_to_read_from,
                        FROST_Message *returned_message,
                        frost_int msecs_to_wait);


/**
 * Destroy a #FROST_MessageQueue created with FROST_MessageQueueCreate()
 * and release all associated resources.
 *
 * @note Any memory allocated dynamically by the client will not be released.
 *
 * @param queue_to_destroy Either a null pointer, or the result of a
 *        call to FROST_MessageQueueCreate().  Calling this with a null argument
 *        is harmless, and does nothing.
 */
void FROST_MessageQueueDestroy(FROST_MessageQueue *queue_to_destroy);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_FROST_FROST_MESSAGE_QUEUE_H_INCLUDED*/
