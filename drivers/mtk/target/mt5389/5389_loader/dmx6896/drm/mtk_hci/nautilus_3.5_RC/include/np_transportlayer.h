/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010 Sony Corporation, All Rights Reserved.
 *
 */

/**
 * @file
 * Definitions for transport layers over which Nautilus communicates with a secure core
 *
 * @see API.reference.pdf
 */

#ifndef NP_TRANSPORTLAYER_H
#define NP_TRANSPORTLAYER_H

NP_BEGIN_EXTERN_C

/** @brief Valid callback return values */
typedef enum {
    ERR_TL_OK            = 0,   /**< Indicates success */
    ERR_TL_ILLEGALARGS   = 1,   /**< The user passed unexpected arguments to the callback */
    ERR_TL_OUTOFMEMORY   = 2,   /**< The transport callback has no resources available to it */
    ERR_TL_INTERNALERROR = 3,   /**< The transport encountered an error and is unrecoverable */
    ERR_TL_DROPPED       = 4,   /**< The transport session has gone down, or the remote closed the connection */
    ERR_TL_TIMEOUT       = 5,   /**< The transport connection failed after a period of time */
    ERR_TL_ACCESS        = 6,   /**< Access to transport is not allowed (e.g. a TCP/IP TL trying
                                     a low numbered port without sufficient permissions) */
    ERR_TL_MSGSIZE       = 7    /**< A request or response message is too big for the transport */
} tl_ret_t;

/**
 * @brief Structure encapsulating Transport Layer connection information
 *        and operation callbacks.
 */
typedef struct np_tl_target_info {
    /** @brief Transport Layer connection info passed in each callback */
    void *target_conn;

    /**
     * @brief Sends an SMI message to a secure core through TL connection
     *
     * @param[in]  in_target_conn Connection info
     * @param[in]  in_req_msg     SMI message sent to secure core
     * @param[out] out_resp_msg   Recieved response from secure core
     *
     * @return ::ERR_TL_OK
     * @return ::ERR_TL_ILLEGALARGS
     * @return ::ERR_TL_OUTOFMEMORY
     * @return ::ERR_TL_INTERNALERROR
     * @return ::ERR_TL_DROPPED
     * @return ::ERR_TL_TIMEOUT
     * @return ::ERR_TL_ACCESS
     * @return ::ERR_TL_MSGSIZE
     */
    tl_ret_t (*tl_SendRecv)(void *in_target_conn, np_str_t *in_req_msg, np_str_t **out_resp_msg);

    /**
     * @brief Frees message returned by tl_SendRecv.  
     * 
     * Tells Transport Layer to free message it has allocated during tl_SendRecv.  
     *
     * @param[in] in_resp_msg   Recieved response from secure core
     *
     * @return ::ERR_TL_OK
     */
    tl_ret_t (*tl_MsgFree)(np_str_t *in_resp_msg);
} np_target_info_t;

typedef struct np_handle {
    np_protocol_t      protocol_info;
    np_target_info_t  *target_info;
} np_transporthandle_t;

typedef np_transporthandle_t* np_handle_t;

#define TL_PROTO(hndl)           (hndl)->protocol_info
#define TL_TARGET(hndl)          (hndl)->target_info

NP_END_EXTERN_C

#endif /* NP_TRANSPORTLAYER_H */
