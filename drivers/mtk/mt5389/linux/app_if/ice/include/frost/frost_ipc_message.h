/**
 * Copyright 2011 Vestel.  All rights reserved.
 *
 * @file
 * @brief  Interface to inter process message queues.
 *
 * This file describes the FROST interface to inter process message queues.  
 *  Briefly, this interface consists of calls to manage the lifecycle of queues, and send
 *  and receive fixed-size messages on them, with the choice of blocking or
 *  non-blocking behaviour.
 */

#ifndef CABOT_FROST_FROST_IPC_MESSAGE_H_INCLUDED
#define CABOT_FROST_FROST_IPC_MESSAGE_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAXIMUM_IPC_MSG_SIZE 256

/**
 * @brief FROST representation of a IPC message queue.
 *
 * #FROST_IpcMessageQueue is an opaque type, to be manipulated only by the
 * functions specified in this header file.  Its implementation is specific
 * to the target platform, though this interface is portable.
 */
typedef struct FROST_IpcMessageQueue FROST_IpcMessageQueue;

/**
 * @brief The type of message.
 * 
 * Provides unique identification of the particular type of message.
 */
typedef enum FROST_IpcMessageType
{   
    /**
     * Message targeted is application
     */
    MQ_TYPE_SERVER_TO_APP = 1,

    /**
     * Message targeted is http server
     */
    MQ_TYPE_APP_TO_SERVER = 2
 }FROST_IpcMessageType;

/**
 * @brief The message queue creation flag that is platform independent  
*/
typedef enum FROST_IpcMessageCreationFlag
{
  /**
    creates message queue no matter if it exists or not
  */
  MQ_FLAG_CREATE_ANYWAY = 0, 
  /**
    creates message queue only if it dose not already exist
  */
  MQ_FLAG_CREATE_IF_NOT_EXIST 
}FROST_IpcMessageCreationFlag;

/**
 * @brief The permission flage of messages passed on the queues. 
 */
typedef enum FROST_IpcMessagePermissionFlag
{
   /**
    * USER permission R/W octal flags
    */
    MQ_FLAG_U_W  = 0200,
    MQ_FLAG_U_R  = 0400,
    MQ_FLAG_U_RW = 0600,
    
    /**
    * GROUP permission R/W flags
    */
    MQ_FLAG_G_W  = 020,
    MQ_FLAG_G_R  = 040,
    MQ_FLAG_G_RW = 060,
    
    /**
    * OTHERS permission flags
    */
    MQ_FLAG_O_W  = 02,
    MQ_FLAG_O_R  = 04,
    MQ_FLAG_O_RW = 06,
    MQ_FLAG_ALL  = MQ_FLAG_U_RW | MQ_FLAG_G_RW | MQ_FLAG_O_RW
}FROST_IpcMessagePermissionFlag;

/**
 * @brief The format of IPC message buffer
 *
 * Each #FROST_IpcMessage allows the user to pass a type message and one data buffer 
 * with MAXIMUM_IPC_MSG_SIZE maximum size
 */
typedef struct FROST_IpcMessage
{   
     /**
     * The type of the message
     */
    frost_long mtype;
    
     /**
     * The message content
     */
    frost_int8 msg[MAXIMUM_IPC_MSG_SIZE];
}FROST_IpcMessage;

/**
 * @brief Initialises/Create a #FROST_IpcMessageQueue
 *
 * Create a #FROST_IpcMessageQueue, which must eventually be released by calling
 * FROST_IpcMessageQueueDestroy().
 *
 * Failure can only be caused by a shortage of OS resources (either memory or
 * message queues).
 *
 * @param flag is requested creation | permition flag
 *
 * @return On success, a non-null pointer which can be used in calls to send
 *         and receive messages.  On failure, a null pointer.
 */
FROST_IpcMessageQueue * FROST_IpcMessageQueueCreate(frost_int flag);

/**
    * @brief Sets message queue id to id of an existing message queue without creating it
    * @param id Message queue id to be copied from
    * @return On success, a non-null pointer which can be used in calls to send
    *         and receive messages.  On failure, a null pointer.
*/
FROST_IpcMessageQueue * FROST_IpcMessageSetQueueId(frost_int id);

/**
  * @brief Gets meaningfull creation flag that is platform dependent
*/
frost_int FROST_IpcMessageGetCreationFlag(FROST_IpcMessageCreationFlag flag);

 /**
 * @brief Send a #FROST_IpcMessage to a #FROST_IpcMessageQueue, blocking if necessary.
 *
 * Send a #FROST_IpcMessage to a #FROST_IpcMessageQueue.  If the IPC message queue is
 * currently full (as is possible on some systems), this call will block if block flag is true.
 *
 * The FROST message layer makes no attempt to free any dynamically allocated
 * memory or other resources pointed to by the message; the receiver of the
 * message must do this in an appropriate manner.
 *
 * The implementation of this function must be thread
 * safe.  In particular, you must make sure that one thread, calling
 * FROST_IpcMessageSend() with a large or infinite timeout, cannot block another
 * thread calling FROST_IpcMessageSend() on the same message queue with a much
 * smaller (or zero) timeout.

 * @param queue_to_send_to Returned from a call to FROST_IpcMessageQueue() -
 *                         must not be null.
 * @param message_to_send  The message to be sent - must not be null.
 * @param block    Indicator whether is operation blocking(true) or none-blocking(false)
 *
 * @return #frost_true if the message was sent, otherwise #frost_false.
 */
 frost_bool FROST_IpcMessageSend(FROST_IpcMessageQueue *queue_to_send_to, 
                        FROST_IpcMessage *message_to_send , 
                        frost_bool block);

/**
 * @brief Read a message from a given #FROST_IpcMessageQueue, blocking if necessary.
 *
 * Read a message from a given #FROST_IpcMessageQueue, blocking the calling thread
 * if necessary.  When there is a message waiting, it is returned via the
 * <code>returned_message</code> argument.
 *
 * There may be two threads simultaneously calling FROST_IpcMessageRecv() on
 * the same message queue.  The implementation of this function must be thread
 * safe
 *
 *
 * @param queue_to_read Returned from a call to FROST_IpcMessageQueueCreate() -
 *                         must not be null.
 * @param returned_message  The message destination buffer - must not be null.
 * @param type      Type off the message
 *                   0 - Retrieve the next message on the queue, regardless of type,
 *                  >0 - Get the next message with an mtype equal to the specified type
 *                  <0 -Retrieve the first message on the queue whose mtype field is 
 *                  less than or equal to the absolute value of the type argument.
 * @param block     Indicator whether is operation blocking(true) or none-blocking(false)
 *
 * @return return -1 if error occured, or number of received bytes
 */
frost_int FROST_IpcMessageRecv(FROST_IpcMessageQueue *queue_to_read_from, 
                    FROST_IpcMessage *returned_message, 
                    FROST_IpcMessageType type, 
                    frost_bool block);

/**
 * Destroy a #FROST_IpcMessageQueueDestroy created with FROST_IpcMessageQueueCreate()
 * and release all associated resources.
 *
 * @note Any memory allocated dynamically by the client will not be released.
 *
 * @param queue_to_destroy Either a null pointer, or the result of a
 *        call to FROST_IpcMessageQueueCreate().  Calling this with a null argument
 *        is harmless, and does nothing.
 */
void FROST_IpcMessageQueueDestroy(FROST_IpcMessageQueue *queue_to_destroy);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_FROST_FROST_IPC_MESSAGE_H_INCLUDED*/
