/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2006,2007,2008,2009,2010 Sony Corporation, All Rights Reserved.
 */

/**
 * @nsdepends
 * basic_types.h, npcommon_types.h and np_transportlayer.h
 *
 * @addtogroup dorado dorado
 * @ingroup nsec
 * @{ 
 *
 * @file
 * API for invoking action tokens for acquisition and deregistration
 *
 * @see API.reference.pdf
 */

#ifndef DOR_API_H
#define DOR_API_H

NP_BEGIN_EXTERN_C

/* Error code */
#define ERR_NP_NOTSETCALLBACK           (ERR_NP_DORADO|1)
#define ERR_NP_CANCEL                   (ERR_NP_DORADO|2)
#define ERR_NP_CRTSTDNOTSUPPORTED       (ERR_NP_DORADO|5)
#define ERR_NP_CALLBACKERROR            (ERR_NP_DORADO|6)

/* Dorado's status */
#define DOR_STAT_DORMANT                (0)
#define DOR_STAT_READY                  (1)
#define DOR_STAT_NEMOPERSONALIZATION    (2)
#define DOR_STAT_OCTOPUSPERSONALIZATION (3)
#define DOR_STAT_DUS                    (4)
#define DOR_STAT_DCS                    (5)
#define DOR_STAT_NODEACQ                (6)
#define DOR_STAT_LINKACQ                (7)
#define DOR_STAT_DEREG                  (8)
#define DOR_STAT_LICENSEACQ             (9)

/* types of HTTP-method */
#define DOR_HTTP_METHOD_GET             (1)
#define DOR_HTTP_METHOD_POST            (2)

/* types of notification */
#define DOR_NOTIFY_TYPE_PROGRESS        (1)

/* 
 * 1 round-trip : total number of steps is 1(include 1 dor_CHTTPSendRecv)
 * 2 round-trip : total number of steps is 2(include 2 dor_CHTTPSendRecv)
 */
typedef struct {
    int stat;
    int total;
    int step;
} dor_notify_progress;

typedef void* dor_handle_t;

typedef struct {
np_ret_t (*dor_CNotify)(
    dor_handle_t     in_handle,
    int              in_type,
    void            *in_notify);
np_ret_t (*dor_CHTTPCreateConnection)(
    dor_handle_t      in_handle,
    char             *in_url,
    void            **out_con);
np_ret_t (*dor_CHTTPDestroyConnection)(
    dor_handle_t     in_handle,
    void            *in_con);
np_ret_t (*dor_CHTTPSetExtraHeader)(
    dor_handle_t     in_handle,
    void            *in_con,
    const char      *in_name,
    const char      *in_value);
np_ret_t (*dor_CHTTPSendRecv)(
    dor_handle_t     in_handle,
    void            *in_con,
    int              method,
    u_int32_t        in_len,
    u_int8_t        *in_buf,
    u_int32_t       *out_len,
    u_int8_t       **out_buf);
np_ret_t (*dor_CFree)(void    *io_buf);
np_ret_t (*dor_CProcessSoapFault)(np_faultinfo_t  *in_faultinfo);

} dor_callback_t;

np_ret_t
dor_Init(np_handle_t in_target_info, dor_handle_t *out_handle);

np_ret_t
dor_Fin(dor_handle_t io_handle);

np_ret_t
dor_SetCallback(dor_handle_t in_handle, dor_callback_t *in_callback);

/** 
 * @brief Register a configuration token
 *
 * If no configuration token is registered one will be retreived during a call to @ref dor_InvokeActionToken.
 * Therefore calling this function is not mandatory.
 *
 * For a detailed description please see the section on @ref configtoken "Configuration Tokens"
 *
 * @note The handle supplied MUST be a handle returned by a call to @ref dor_Init. The result of passing
 *       any other value is undefined.
 *
 * @param[in] in_handle    Dorado handle
 * @param[in] in_ctkn_len  Length of the configuration token
 * @param[in] in_ctkn_buf  Pointer to buffer which contains the configuration token
 * 
 * @return ::ERR_NP_OK                            <br>Indicating success.
 * @return ::ERR_NP_CANCEL                        <br>Dorado was notified by the calling application to stop all operations.
 * @return ::ERR_NP_ILLEGALHANDLE                 <br>The handle passed to the function is invalid.
 * @return ::ERR_NP_ILLEGALARGS                   <br>The arguments to the function are incorrect.
 * @return ::ERR_NP_INTERNALERROR                 <br>An unrecoverable failure occurred in Nautilus.
 * @return ::ERR_NP_OUTOFMEMORY                   <br>Nautilus encountered resource problems.
 * @return ::ERR_NP_NOTSETCALLBACK                <br>Mandatory callbacks are missing.
 * @return ::ERR_NP_ILLEGALST                     <br>The service token from the server is invalid or corrupted.
 * @return ::ERR_NP_FILEERROR                     <br>An error occurred reading or writing to file.
 * @return ::ERR_NP_ALREADYREGISTERED             <br>Configuration token is already registered.
 * @return ::ERR_NP_CONNECTIONERROR               <br>Problems were encountered communicating with the server.
 */
np_ret_t
dor_RegConfigToken(
    dor_handle_t     in_handle,
    u_int32_t        in_ctkn_len,
    u_int8_t        *in_ctkn_buf);

np_ret_t
dor_InvokeActionToken(
    dor_handle_t      in_handle,
    u_int32_t         in_atkn_len,
    u_int8_t         *in_atkn_buf,
    np_str_t        **out_license);

NP_END_EXTERN_C

#endif /* DOR_API_H */
/** @} */
