/**
 * @addtogroup nsnp_ms3 MS3 functionality
 * @ingroup nsnp
 * @{ 
 *
 * @brief nsnp layer APIs for MS3
 *
 * @file
 * MS3 APIs
 */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2011 Sony Corporation, All Rights Reserved.
 */


#ifndef NSNP_MS3_API_H
#define NSNP_MS3_API_H

NP_BEGIN_EXTERN_C

/* -------------------------------------------------------------------------- */
/*                                                                 PUBLIC API */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/**
 * @brief Open a ms3 session 
 *
 * @param[in,out] io_handle           Handle for session
 *
 * @note Only one processing request ActionToken|CompoundURI|Manifest can be
 *       issued persion ssession
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_OUTOFMEMORY
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_ms3InitSession(np_handle_t  io_handle);


/* -------------------------------------------------------------------------- */
/** 
 * @brief Close a ms3 session
 *
 * @param[in,out] io_handle            Handle for the session to close
 *
 * @return ::ERR_NP_OK
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_ms3FinSession(np_handle_t  io_handle);


/* -------------------------------------------------------------------------- */
/**
 * @brief Process a MS3 Action Token 
 *
 * @param[in]     in_handle                  MS3 session handle
 * @param[in]     in_callback                HTTPS callback to retrieve the S-URL
 * @param[in]     in_application_data_len    Length of the application data
 * @param[in]     in_application_data        Opaque application data. Passed to the
 *                                           export keys routine and the https callback
 * @param[in]     in_action_token_len        Length of the action token
 * @param[in]     in_action_token            MS3 Action token to process
 * @param[out]    out_results                Results of the processing request
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_ILLEGALMS3AT
 * @return ::ERR_NP_ILLEGALCURIT
 * @return ::ERR_NP_SASALREADYSET
 * @return ::ERR_NP_ILLEGALSAS
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_ms3ActionTokenProcess(
    np_handle_t            in_handle,
    np_ms3_callback_t     *in_callback,
    np_size_t              in_application_data_len,
    u_int8_t              *in_application_data,
    np_size_t              in_action_token_len,
    u_int8_t              *in_action_token,
    np_ms3_result_data_t **out_results);


/* -------------------------------------------------------------------------- */
/** 
 * @brief Process a Compound URI 
 *
 * @param[in]     in_handle                  MS3 session handle
 * @param[in]     in_callback                HTTPS callback to retrieve the S-URL
 * @param[in]     in_application_data_len    Length of the application data
 * @param[in]     in_application_data        Opaque application data. Passed to the
 *                                           export keys routine and the https callback
 * @param[in]     in_uri_len                 Length of the Compound URI
 * @param[in]     in_uri                     Compound URI to process
 * @param[out]    out_results                Results of the processing request
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_ILLEGALCOMPOUNDURI
 * @return ::ERR_NP_ILLEGALCURIT
 * @return ::ERR_NP_SASALREADYSET
 * @return ::ERR_NP_ILLEGALSAS
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_ms3CompoundURIProcess(
    np_handle_t            in_handle,
    np_ms3_callback_t     *in_callback,
    np_size_t              in_application_data_len,
    u_int8_t              *in_application_data,
    np_size_t              in_uri_len,
    u_int8_t              *in_uri,
    np_ms3_result_data_t **out_results);


/* -------------------------------------------------------------------------- */
/** 
 * @brief Process a Manifest file 
 *
 * @param[in]     in_handle                  MS3 session handle
 * @param[in]     in_callback                HTTPS callback to retrieve the S-URL
 * @param[in]     in_application_data_len    Length of the application data
 * @param[in]     in_application_data        Opaque application data. Passed to the
 *                                           export keys routine and the https callback
 * @param[in]     in_manifest_file_len       Length of the MS3 Manifest file
 * @param[in]     in_manifest_file           MS3 Manifest file to process
 * @param[out]    out_results                Results of the processing request
 * @param[out]    out_content_type           Content Mime Type extracted from the 
 *                                           Manifest file
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_ILLEGALMANIFESTFILE
 * @return ::ERR_NP_ILLEGALCURIT
 * @return ::ERR_NP_SASALREADYSET
 * @return ::ERR_NP_ILLEGALSAS
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_ms3ManifestFileProcess(
    np_handle_t            in_handle,
    np_ms3_callback_t     *in_callback,
    np_size_t              in_application_data_len,
    u_int8_t              *in_application_data,
    np_size_t              in_manifest_file_len,
    u_int8_t              *in_manifest_file,
    np_ms3_result_data_t **out_results,
    np_str_t             **out_content_type);


/* -------------------------------------------------------------------------- */
/** 
 * @brief Expand a curit using the SAS previously acquired with an AT, compound
 *        URI or Manifest file
 *
 * @note the supplied curit can be in string format or encoded in a compound URI
 *       If the curit is in a compound URI then the SURL part of the compound
 *       URI MUST match to the SURL used to acquire the SAS which is stored in
 *       the MS3 handle.
 *
 * @note This API can be called multiple times for a single session.
 *
 * @param[in]     in_handle           MS3 session handle
 * @param[in]     in_curit_len        Curit string or curit in a Compound URI length
 * @param[in]     in_curit            Curit string or curit in a Compound URI
 * @param[out]    out_curl            Expanded Content URL
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_ILLEGALCURIT
 * @return ::ERR_NP_NOSASFOUND
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_ms3CuritExpand(
    np_handle_t         in_handle,
    np_size_t           in_curit_len,
    u_int8_t           *in_curit,
    np_str_t          **out_curl);


/* -------------------------------------------------------------------------- */
/** 
 * @brief Notification that SAS information is valid and understood by the
 *        Application
 *
 * @param[in] in_handle              MS3 Session
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_SASALREADYPROCESSED
 * @return ::ERR_NP_NOSASFOUND
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_ms3SasInfoProcessingComplete(np_handle_t  in_handle);


/* -------------------------------------------------------------------------- */
/**
 * @brief Free the returned results data
 *
 * @param[in,out] io_results             Result data to be freed
 *
 * @return ::ERR_NP_OK
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_ms3FreeResultData(np_ms3_result_data_t  *io_results);

NP_END_EXTERN_C

#endif /* NSNP_MS3_API_H */
/** @} */
