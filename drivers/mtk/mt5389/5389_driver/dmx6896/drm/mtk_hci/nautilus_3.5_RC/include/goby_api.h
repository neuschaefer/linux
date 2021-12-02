/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2010,2011 Sony Corporation, All Rights Reserved.
 */

/**
 * @addtogroup goby goby
 * @ingroup nsec
 * @{
 *
 * @file
 * @brief Public API for Marlin Protocol specification
 */
#ifndef GOBY_API_H
#define GOBY_API_H

NP_BEGIN_EXTERN_C

#define GOBY_HTTP_RET_OK                   0
#define GOBY_HTTP_RET_ERROR                1

#define GOBY_HTTP_METHOD_GET               0
#define GOBY_HTTP_METHOD_POST              1

#define GOBY_CLIENT_PROTOCOL_LOCAL         0
#define GOBY_CLIENT_PROTOCOL_USB           1

#define GOBY_RESULT_TYPE_REGISTER_NODE     1
#define GOBY_RESULT_TYPE_REGISTER_LINK     2
#define GOBY_RESULT_TYPE_DEREGISTER_LINK   3
#define GOBY_RESULT_TYPE_LICENSE           4
#define GOBY_RESULT_TYPE_WRITE_FILE        5
#define GOBY_RESULT_TYPE_PROTOCOL_INFO     6
#define GOBY_RESULT_TYPE_SOAPFAULT         16

#define GOBY_RESULT_WRITE_FILE_LICENSE     1

#define GOBY_TRANSFERMODE_MOVE             1

#define GOBY_INFO_PLAY                     0

#define GOBY_DATE_IS_ZERO(dt) \
    (dt.year==0 && dt.mon==0 && dt.mday==0 && dt.hour==0 && dt.min==0 && dt.sec==0 && dt.msec==0 ? 1 : 0)

typedef u_int32_t http_ret_t;

typedef struct goby_http {
    http_ret_t (*cb_CreateConnection)(
        char             *in_url,
        void            **out_con);
    http_ret_t (*cb_DestroyConnection)(
        void            *in_con);
    http_ret_t (*cb_SetExtraHeader)(
        void            *in_con,
        const char      *in_name,
        const char      *in_value);
    http_ret_t (*cb_SendRecv)(
        void            *in_con,
        int              in_method,
        u_int32_t        in_len,
        u_int8_t        *in_buf,
        u_int32_t       *out_len,
        u_int8_t       **out_buf);
    void (*cb_FreeResponse)(
        void            *io_buf);
} goby_http_t;

typedef struct goby_protocol_usb {
    np_target_info_t*    callbacks;
    np_str_t*            general_info;
} goby_protocol_usb_t;

typedef struct goby_client {
    u_int32_t       protocol;
    union {
        goby_protocol_usb_t    usb;
    } driver;
} goby_client_t;

typedef struct goby_musb_hello {
    u_int16_t  version_major;
    u_int16_t  version_minor;
    np_str_t  *client_info;
    np_str_t  *vendor_ext;
    u_int16_t  supported_options;
} goby_musb_hello_t;

typedef struct goby_remote_info {
    u_int32_t protocol;
    union {
        goby_musb_hello_t hello;
    } remote;
} goby_remote_info_t;

typedef struct goby_link_list {
    struct goby_link_list *next_link;
    np_str_t              *from_id;
    np_str_t              *to_id;
} goby_link_list_t;

typedef struct goby_node_list {
    struct goby_node_list  *next_node;
    np_str_t               *node_id;
} goby_node_list_t;


typedef void* goby_handle_t;

typedef struct goby_action_results {
    struct goby_action_results *next_results;
    int                         result_type;
    np_str_t                   *srv_id;
    np_str_t                   *srv_info;
    union {
        np_str_t            *node_id;
        struct {
            np_str_t        *from_id;
            np_str_t        *to_id;
        } link;
        np_str_t            *license;
        struct {
            u_int32_t        type;
            np_str_t        *file;
        } write;
        np_faultinfo_t      *faultinfo;
        np_protocol_info_t   protocol_info;
    } result;
} goby_action_results_t;

typedef struct goby_license_info {
    goby_node_list_t      *target_nodes;
    np_date_t              not_before;
    np_date_t              not_after;
    u_int32_t              remaining_time;
    u_int32_t              first_use_remaining_time;
} goby_license_info_t;

np_ret_t
goby_Init(
    goby_client_t        *in_client,
    goby_handle_t        *out_handle,
    goby_remote_info_t  **out_remote_info);

np_ret_t
goby_Fin(
    goby_handle_t  io_handle);
    
np_ret_t
goby_FreeRemoteInfo(
    goby_remote_info_t *io_remote_info);

np_ret_t
goby_ManageRights(
    goby_handle_t           in_handle,
    np_str_t               *in_action_token,
    np_str_t               *in_service_id,
    np_str_t               *in_service_info,
    goby_http_t            *in_http,
    goby_action_results_t **out_results);

np_ret_t
goby_FreeActionResults(
    goby_action_results_t *io_results);

np_ret_t
goby_ProcessMessage(
    goby_handle_t            in_handle,
    np_str_t                *in_request,
    goby_action_results_t  **out_result,
    np_str_t               **out_response);

np_ret_t
goby_GetAvailableLinks(
    goby_handle_t            in_handle,
    goby_link_list_t       **out_links);

np_ret_t
goby_GetLinksForServiceID(
    goby_handle_t             in_handle,
    np_str_t                 *in_service_id,
    np_str_t                 *in_service_info,
    goby_link_list_t        **out_links);

np_ret_t
goby_FreeLinkList(
    goby_link_list_t *io_links);

np_ret_t
goby_GeneratePersonalizationRequest(
    goby_handle_t   in_handle,
    np_str_t      **out_request);

np_ret_t
goby_ProcessPersonalizationResponse(
    goby_handle_t   in_handle,
    np_str_t       *in_personality);

np_ret_t
goby_DeleteLink(
    goby_handle_t            in_handle,
    np_str_t                *in_from,
    np_str_t                *in_to);

np_ret_t
goby_CheckPersonality(
    goby_handle_t   in_handle);

np_ret_t
goby_DescribeActionPlay(
    goby_handle_t   in_handle,
    np_str_t       *in_license,
    np_esb_t      **out_esb);

np_ret_t
goby_GetDUIDParameters(
    goby_handle_t    in_handle,
    u_int32_t        in_cinfo,
    np_str_t       **out_duid);

np_ret_t
goby_GetReachableNodes(
    goby_link_list_t     *in_links,
    np_str_t             *in_node,
    goby_node_list_t     **out_nodes);
    
np_ret_t 
goby_FreeNodeList(
    goby_node_list_t  *io_nodes);    

np_ret_t
goby_GetLinkPeriod(
    goby_handle_t            in_handle,
    np_str_t                *in_from,
    np_str_t                *in_to,
    np_date_t               *out_validity_start,
    np_date_t               *out_validity_end);
    
np_ret_t
goby_CheckLicense(
    goby_handle_t            in_handle,
    np_str_t                *in_license,
    goby_license_info_t    **out_info);

np_ret_t
goby_FreeLicenseInfo(
    goby_license_info_t     *io_info);
    
np_ret_t
goby_TransferLicense(
    goby_handle_t            in_source,
    goby_handle_t            in_sink,
    np_str_t                *in_license,
    u_int32_t                in_transfer_mode,
    goby_action_results_t  **out_result);

np_ret_t 
goby_UpdateTT(
    goby_handle_t    in_handle,
    goby_http_t     *in_http);

np_ret_t
goby_EnableWriteData(
    goby_handle_t    in_handle,
    np_bool_t        in_enable);

NP_END_EXTERN_C

#endif /* GOBY_API_H */
/** @} */
