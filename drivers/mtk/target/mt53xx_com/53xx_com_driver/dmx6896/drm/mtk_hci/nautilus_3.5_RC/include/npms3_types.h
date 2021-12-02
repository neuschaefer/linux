/** @addtogroup npms3 npms3
 *  @ingroup api
 *  @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2011 Sony Corporation, All Rights Reserved.
 */

#ifndef NPMS3_TYPES_H
#define NPMS3_TYPES_H

NP_BEGIN_EXTERN_C

/* -------------------------------------------------------------------------- */
/*                                                     PUBLIC DATA STRUCTURES */
/* -------------------------------------------------------------------------- */


/**
 * MS3 Error Result Data type
 *
 * Type to specify the error information returned if acquiring a SAS fails
 *
 **/
typedef struct {
    u_int32_t            http_err_code;   /** HTTP error code returned */
    np_str_t            *err_st;          /** Data returned in the case of a 
                                           *  failure to acuqire the SAS */
} np_ms3_err_info_t;


/**
 * MS3 Result Data type
 *
 * Type to specify the data returned by processing a Action Token, Compound
 * URI or a Manifest File
 **/

typedef enum {
    NP_MS3_SAS_SUCCESS_RESULTS            = 0, /** Sucessful SAS processing */
    NP_MS3_SAS_ACQ_ERR_RESULTS            = 1, /** SAS acquisition failed */
} np_ms3_results_type_t;


/**
 * HTTP extra header needed for MS3 
 *
 * Type to specify the HTTP extra header, which has a 
 * name<->value pair
 **/

typedef struct {
    char *name;                           /** HTTP extra header name  */
    char *value;                          /** HTTP extra header value */ 
}np_http_extra_hdr_t;

/** Use this callback when the nemo credentials should be used on the 
 *  connection with the ms3 service
 *
 * Below is the set of ERRORs that can be used by the callback implementation to notify
 * Nautilus of the status of the callback processing
 *
 * @return ::ERR_NP_OK               Indicating success.
 * @return ::ERR_NP_ILLEGALARGS      The callback detected errors in provided arguments
 * @return ::ERR_NP_INTERNALERROR    An unrecoverable failure occurred in the callback.
 * @return ::ERR_NP_HTTP_CALLBACK    An error occured for processing the HTTP request
 */

typedef np_ret_t
(*np_ms3_https_nemo_callback_t)
    ( u_int32_t             num_extra_headers,          /** Number of extra http headers */
      np_http_extra_hdr_t **in_extra_header,            /** Array of headers */
      char                 *in_surl,                    /** URL to acquire the SAS from */
      u_int32_t             in_cb_application_data_len, /** Length of opaque data */
      u_int8_t             *in_cb_application_data,     /** Opaque application data. Maybe
                                                            used to protect SAS and NEMO keys */
      u_int32_t             in_nemo_pub_certs_num,      /** Number of certs in in_nemo_pub_certs */
      np_str_t            **in_nemo_pub_certs,          /** Public NEMO certs array - format DER
                                                            First cert in the array is the device certificate */
      np_str_t             *in_nemo_priv_key,           /** Private NEMO key. Protection is the
                                                            porters responsibility */
      u_int32_t            *out_http_status,            /** HTTP status code return during the SAS acquisition */
      u_int32_t            *out_sas_mime_type_len,      /** Length of the Mime type of the SAS acquired */
      u_int8_t            **out_sas_mime_type,          /** Mime type of the SAS acquired. Only valid if the
                                                  HTTPs request is successful */
      u_int32_t            *out_resp_len,               /** Lenght of acquired data from service */
      u_int8_t            **out_resp );                 /** Acquired data from service. Protection is the
                                                  porters responsibility. In case of ERR_OK this
                                                  will be the SAS. Otherwise it is a status string
                                                  returned back to the Application */

/** Use this callback when an application specific key is used on the 
 *  connection with the ms3 service
 *
 * Below is the set of ERRORs that can be used by the callback implementation to notify
 * Nautilus of the status of the callback processing
 *
 * @return ::ERR_NP_OK               Indicating success.
 * @return ::ERR_NP_ILLEGALARGS      The callback detected errors in provided arguments
 * @return ::ERR_NP_INTERNALERROR    An unrecoverable failure occurred in the callback.
 * @return ::ERR_NP_HTTP_CALLBACK    An error occured for processing the HTTP request
 */

typedef np_ret_t
(*np_ms3_https_callback_t)
    ( u_int32_t             num_extra_headers,          /** Number of extra http headers */
      np_http_extra_hdr_t **in_extra_header,            /** Array of headers */
      char                 *in_surl,                    /** URL to acquire the SAS from */
      u_int32_t             in_cb_application_data_len, /** Length of opaque data */
      u_int8_t             *in_cb_application_data,     /** Opaque application data. Maybe
                                                          used to protect SAS and NEMO keys */
      u_int32_t            *out_http_status,            /** HTTP status code return during the SAS acquisition */
      u_int32_t            *out_sas_mime_type_len,      /** Length of the Mime type of the SAS acquired */
      u_int8_t            **out_sas_mime_type,          /** Mime type of the SAS acquired. Only valid if the
                                                  HTTPs request is successful */
      u_int32_t            *out_resp_len,               /** Lenght of acquired data from service */
      u_int8_t            **out_resp );                 /** Acquired data from service. Protection is the
                                                  porters responsibility. In case of ERR_OK this
                                                  will be the SAS. Otherwise it is a status string
                                                  returned back to the Application */

/** Use this callback to free data allocated by the Application callback */
typedef np_ret_t
(*np_ms3_free_callback_t)(void *io_buf);


/**
 * HTTPs callback types
 *
 * Type to specify the https callback registered with the ms3 session
 **/
typedef enum {
    NP_MS3_HTTPS_WITH_APP_KEY            = 0, /** Use an application specific key */
    NP_MS3_HTTPS_WITH_NEMO_KEY           = 1  /** Use the nemo key */
} np_ms3_callback_type_t;

/**
 * HTTPS callback
 *
 * Application supplied callback for establishing a https connection with the
 * MS3 service. The Application can choose to use its own keys or the nemo keys
 * on the connection based of the callback registered with the MS3 session
 *
 **/

typedef struct {
    np_ms3_callback_type_t            callback_type;       /** type of callback registered */
    union {
        np_ms3_https_nemo_callback_t  nemo_https_callback; /** use nautilus nemo keys */
        np_ms3_https_callback_t       app_https_callback;  /** use app keys */
    } cb;
    np_ms3_free_callback_t            free_callback;       /** use to free app allocated data */
} np_ms3_callback_t;

/**
 * MS3 SAS Extension data type
 *
 * Type to specify the an extension returned in a SAS
 **/
typedef struct {
    np_bool_t                is_critical;     /** Flag indicating if the extension is critical */
    u_int32_t                extension_type;  /** Type of the extension */
    np_str_t                *extension_data;  /** Extension data */
} np_ms3_extension_t;

/**
 * MS3 Content Retention Flag type
 *
 * Type to specify the control flag returned in a SAS. It indicates if the
 * streamed content can be retainted by the device or not.
 **/

typedef enum {
    NP_MS3_CONTENT_RETENTION_NOT_ALLOWED  = 0,  /** Content rentention not allowed */
    NP_MS3_CONTENT_RETENTION_ALLOWED      = 1   /** Content can be retained */
} np_retention_flag_t;


/**
 * MS3 SAS Result Data type
 *
 * Type to specify the information returned after parsing a SAS
 *
 * @note the CURL may not always be present. In some cases it is already known
 *       by the caller
 **/
typedef struct {
    np_output_control_t     *moc_info;        /** Marlin Output control info */
    np_bool_t                is_curl_present; /** Flag indicating is the CURL is present */
    np_str_t                *curl;            /** Content URL if there was a curit expanded */
    np_retention_flag_t      retention_flag;  /** Retention flag */
    u_int32_t                num_content_ids;
    np_str_t               **content_ids;     /** Array of hashed content ids.
                                               *  Useful for app integration and testing */
    u_int32_t                num_extensions;  /** Number of extentsion present */
    np_ms3_extension_t     **extensions;    /** The array of extension */
} np_ms3_sas_info_t;

/**
 * MS3 Result Data
 *
 * Result data returned after an attempt to process a Action Token, Compound
 * URI or Manifest file. If there is a failure if may be possible for the
 * service to return a html document to explain the failure. This is passed
 * as is back to the caller
 **/
typedef struct {
    np_ms3_results_type_t       results_type;    /** Type of results */
    /** Result data corresponding to the result_type */
    union {
        np_ms3_sas_info_t      *sas_info;        /** Data returned for NP_MS3_SAS_SUCCESS_RESULTS*/
        np_ms3_err_info_t      *acq_err_info;    /** Data returned for NP_MS3_SAS_ACQ_ERR_RESULTS*/
    } data;
} np_ms3_result_data_t;

NP_END_EXTERN_C

#endif /* NPMS3_API_H */
