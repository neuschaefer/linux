/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @file u_http_firmware_upgrade.h
 * $RCSfile: $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * @par Description:
 *      http/firmware client
 */
/*----------------------------------------------------------------------------*/
/*!
* @addtogroup  groupMW_INET
* @{
*/

#ifndef U_HTTP_FIRMWARE_UPGRADE_H
#define U_HTTP_FIRMWARE_UPGRADE_H

#include "u_common.h"
#include "u_handle.h"

/*!
 * @enum HTTP_RESPONSE_STATUS_CODE_T
 * @brief HTTP response status/code
 * @code
 * typedef enum
 * {
 *     HTTP_RSP_STATUS_100                           = 100,
 *     HTTP_RSP_STATUS_CONTINUE                      = 100,
 *     HTTP_RSP_STATUS_SWITCHING_PROTOCOLS           = 101,
 *     HTTP_RSP_STATUS_200_OK                        = 200,
 *     HTTP_RSP_STATUS_CREATED                       = 201,
 *     HTTP_RSP_STATUS_ACCEPTED                      = 202,
 *     HTTP_RSP_STATUS_NO_AUTHORATATIVE_INFO         = 203,
 *     HTTP_RSP_STATUS_NO_CONTENT                    = 204,
 *     HTTP_RSP_STATUS_RESET_CONTENT                 = 205,
 *     HTTP_RSP_STATUS_PARTIAL_CONTENT               = 206,
 *     HTTP_RSP_STATUS_MULTIPLE_CHOICES              = 300,
 *     HTTP_RSP_STATUS_MOVED_PERMANENTLY             = 301,
 *     HTTP_RSP_STATUS_FOUND                         = 302,
 *     HTTP_RSP_STATUS_SEE_OTHER                     = 303,
 *     HTTP_RSP_STATUS_NOT_MODIFIED                  = 304,
 *     HTTP_RSP_STATUS_USE_PROXY                     = 305,
 *     HTTP_RSP_STATUS_TEMPORARY_REDIRECT            = 307,
 *     HTTP_RSP_STATUS_BAD_REQUEST                   = 400,
 *     HTTP_RSP_STATUS_UNAUTHORIZED                  = 401,
 *     HTTP_RSP_STATUS_PAYMENT_REQUIRED              = 402,
 *     HTTP_RSP_STATUS_FORBIDDEN                     = 403,
 *     HTTP_RSP_STATUS_NOT_FOUND                     = 404,
 *     HTTP_RSP_STATUS_METHOD_NOT_ALLOWED            = 405,
 *     HTTP_RSP_STATUS_NOT_ACCEPTABLE                = 406,
 *     HTTP_RSP_STATUS_PROXY_AUTH_REQUIRED           = 407,
 *     HTTP_RSP_STATUS_REQUEST_TIMEOUT               = 408,
 *     HTTP_RSP_STATUS_CONFLICT                      = 409,
 *     HTTP_RSP_STATUS_GONE                          = 410,
 *     HTTP_RSP_STATUS_LENGTH_REQUIRED               = 411,
 *     HTTP_RSP_STATUS_PRECONDITION_FAILED           = 412,
 *     HTTP_RSP_STATUS_REQ_ENTITY_TOO_LARGE          = 413,
 *     HTTP_RSP_STATUS_REQUEST_URI_TOO_LONG          = 414,
 *     HTTP_RSP_STATUS_UNSUPPORTED_MEDIA_TYPE        = 415,
 *     HTTP_RSP_STATUS_REQUEST_RANGE_NOT_SATISFIABLE = 416,
 *     HTTP_RSP_STATUS_EXPECTATION_FAILED            = 417,
 *     HTTP_RSP_STATUS_INTERNAL_SERVER_ERROR         = 500,
 *     HTTP_RSP_STATUS_NOT_IMPLEMENTED               = 501,
 *     HTTP_RSP_STATUS_BAD_GATEWAY                   = 502,
 *     HTTP_RSP_STATUS_SERVICE_UNAVAILABLE           = 503,
 *     HTTP_RSP_STATUS_GATEWAY_TIMEOUT               = 504,
 *     HTTP_RSP_STATUS_VERSION_NOT_SUPPORTED         = 505,
 *     HTTP_RSP_STATUS_CODE_MAX                      = 600,
 *     HTTP_RSP_STATUS_CODE_UNKNOWN                  = -1
 * } HTTP_RESPONSE_STATUS_CODE_T;
 * @endcode
 *
 * @li@c HTTP_RSP_STATUS_100                           - continue
 * @li@c HTTP_RSP_STATUS_CONTINUE                      - continue
 * @li@c HTTP_RSP_STATUS_SWITCHING_PROTOCOLS           - switching protocols
 * @li@c HTTP_RSP_STATUS_200_OK                        - OK
 * @li@c HTTP_RSP_STATUS_CREATED                       - created OK
 * @li@c HTTP_RSP_STATUS_ACCEPTED                      - accept OK
 * @li@c HTTP_RSP_STATUS_NO_AUTHORATATIVE_INFO         - no authoratative information
 * @li@c HTTP_RSP_STATUS_NO_CONTENT                    - no content
 * @li@c HTTP_RSP_STATUS_RESET_CONTENT                 - reset content
 * @li@c HTTP_RSP_STATUS_PARTIAL_CONTENT               - parital content
 * @li@c HTTP_RSP_STATUS_MULTIPLE_CHOICES              - multiple choices
 * @li@c HTTP_RSP_STATUS_MOVED_PERMANENTLY             - moved permanertly
 * @li@c HTTP_RSP_STATUS_FOUND                         - found
 * @li@c HTTP_RSP_STATUS_SEE_OTHER                     - see other
 * @li@c HTTP_RSP_STATUS_NOT_MODIFIED                  - not modified
 * @li@c HTTP_RSP_STATUS_USE_PROXY                     - use proxy
 * @li@c HTTP_RSP_STATUS_TEMPORARY_REDIRECT            - temporary redirect
 * @li@c HTTP_RSP_STATUS_BAD_REQUEST                   - bad request
 * @li@c HTTP_RSP_STATUS_UNAUTHORIZED                  - unauthorized
 * @li@c HTTP_RSP_STATUS_PAYMENT_REQUIRED              - payment requied
 * @li@c HTTP_RSP_STATUS_FORBIDDEN                     - forbidden
 * @li@c HTTP_RSP_STATUS_NOT_FOUND                     - not found
 * @li@c HTTP_RSP_STATUS_METHOD_NOT_ALLOWED            - method not allowed
 * @li@c HTTP_RSP_STATUS_NOT_ACCEPTABLE                - not acceptable
 * @li@c HTTP_RSP_STATUS_PROXY_AUTH_REQUIRED           - proxy auth required
 * @li@c HTTP_RSP_STATUS_REQUEST_TIMEOUT               - request timeout
 * @li@c HTTP_RSP_STATUS_CONFLICT                      - conflict
 * @li@c HTTP_RSP_STATUS_GONE                          - gone
 * @li@c HTTP_RSP_STATUS_LENGTH_REQUIRED               - length required
 * @li@c HTTP_RSP_STATUS_PRECONDITION_FAILED           - precondition failed
 * @li@c HTTP_RSP_STATUS_REQ_ENTITY_TOO_LARGE          - requst entry too large
 * @li@c HTTP_RSP_STATUS_REQUEST_URI_TOO_LONG          - request URI too long
 * @li@c HTTP_RSP_STATUS_UNSUPPORTED_MEDIA_TYPE        - unsupported media type
 * @li@c HTTP_RSP_STATUS_REQUEST_RANGE_NOT_SATISFIABLE - request range not satisfiable
 * @li@c HTTP_RSP_STATUS_EXPECTATION_FAILED            - expectation failed
 * @li@c HTTP_RSP_STATUS_INTERNAL_SERVER_ERROR         - internal server error
 * @li@c HTTP_RSP_STATUS_NOT_IMPLEMENTED               - not implemented
 * @li@c HTTP_RSP_STATUS_BAD_GATEWAY                   - bad gateway
 * @li@c HTTP_RSP_STATUS_SERVICE_UNAVAILABLE           - service unavalilable
 * @li@c HTTP_RSP_STATUS_GATEWAY_TIMEOUT               - gateway timeout
 * @li@c HTTP_RSP_STATUS_VERSION_NOT_SUPPORTED         - version not supported
 * @li@c HTTP_RSP_STATUS_CODE_MAX                      - max http response code value
 * @li@c HTTP_RSP_STATUS_CODE_UNKNOWN                  - unknown error
 */
typedef enum
{
    /* Informational.  */
    HTTP_RSP_STATUS_100                        = 100,
    HTTP_RSP_STATUS_CONTINUE                   = 100,
    HTTP_RSP_STATUS_SWITCHING_PROTOCOLS        = 101,

    /* Successful.  */
    HTTP_RSP_STATUS_200_OK                     = 200,
    HTTP_RSP_STATUS_CREATED                    = 201,
    HTTP_RSP_STATUS_ACCEPTED                   = 202,
    HTTP_RSP_STATUS_NO_AUTHORATATIVE_INFO      = 203,
    HTTP_RSP_STATUS_NO_CONTENT                 = 204,
    HTTP_RSP_STATUS_RESET_CONTENT              = 205,
    HTTP_RSP_STATUS_PARTIAL_CONTENT            = 206,

    /* Redirection.  */
    HTTP_RSP_STATUS_MULTIPLE_CHOICES            = 300,
    HTTP_RSP_STATUS_MOVED_PERMANENTLY           = 301,
    HTTP_RSP_STATUS_FOUND                       = 302,
    HTTP_RSP_STATUS_SEE_OTHER                   = 303,

    HTTP_RSP_STATUS_NOT_MODIFIED                = 304,
    HTTP_RSP_STATUS_USE_PROXY                   = 305,
    HTTP_RSP_STATUS_TEMPORARY_REDIRECT          = 307,

    /* Client error.  */
    HTTP_RSP_STATUS_BAD_REQUEST                   = 400,
    HTTP_RSP_STATUS_UNAUTHORIZED                  = 401,
    HTTP_RSP_STATUS_PAYMENT_REQUIRED              = 402,
    HTTP_RSP_STATUS_FORBIDDEN                     = 403,
    HTTP_RSP_STATUS_NOT_FOUND                     = 404,
    HTTP_RSP_STATUS_METHOD_NOT_ALLOWED            = 405,
    HTTP_RSP_STATUS_NOT_ACCEPTABLE                = 406,
    HTTP_RSP_STATUS_PROXY_AUTH_REQUIRED           = 407,
    HTTP_RSP_STATUS_REQUEST_TIMEOUT               = 408,
    HTTP_RSP_STATUS_CONFLICT                      = 409,
    HTTP_RSP_STATUS_GONE                          = 410,
    HTTP_RSP_STATUS_LENGTH_REQUIRED               = 411,
    HTTP_RSP_STATUS_PRECONDITION_FAILED           = 412,
    HTTP_RSP_STATUS_REQ_ENTITY_TOO_LARGE          = 413,
    HTTP_RSP_STATUS_REQUEST_URI_TOO_LONG          = 414,
    HTTP_RSP_STATUS_UNSUPPORTED_MEDIA_TYPE        = 415,
    HTTP_RSP_STATUS_REQUEST_RANGE_NOT_SATISFIABLE = 416,
    HTTP_RSP_STATUS_EXPECTATION_FAILED            = 417,

    /* Server error. */
    HTTP_RSP_STATUS_INTERNAL_SERVER_ERROR         = 500,
    HTTP_RSP_STATUS_NOT_IMPLEMENTED               = 501,
    HTTP_RSP_STATUS_BAD_GATEWAY                   = 502,
    HTTP_RSP_STATUS_SERVICE_UNAVAILABLE           = 503,
    HTTP_RSP_STATUS_GATEWAY_TIMEOUT               = 504,
    HTTP_RSP_STATUS_VERSION_NOT_SUPPORTED         = 505,

    HTTP_RSP_STATUS_CODE_MAX                      = 600,
    HTTP_RSP_STATUS_CODE_UNKNOWN                  = -1
} HTTP_RESPONSE_STATUS_CODE_T;

/*!
 * @enum CONTENT_TYPE_T
 * @brief HTTP content type
 * @code
 * typedef enum 
 * {
 *     CONTENT_TYPE_APPLICATION = 1,
 *     CONTENT_TYPE_AUDIO,
 *     CONTENT_TYPE_EXAMPLE,
 *     CONTENT_TYPE_IMAGE,
 *     CONTENT_TYPE_MESSAGE,
 *     CONTENT_TYPE_MODEL,
 *     CONTENT_TYPE_MULTIPART,
 *     CONTENT_TYPE_TEXT,
 *     CONTENT_TYPE_VIDEO
 * } CONTENT_TYPE_T;
 * @endcode
 *
 * @li@c CONTENT_TYPE_APPLICATION - application
 * @li@c CONTENT_TYPE_AUDIO       - audio
 * @li@c CONTENT_TYPE_EXAMPLE     - example
 * @li@c CONTENT_TYPE_IMAGE       - image
 * @li@c CONTENT_TYPE_MESSAGE     - message
 * @li@c CONTENT_TYPE_MODEL       - model
 * @li@c CONTENT_TYPE_MULTIPART   - multipart
 * @li@c CONTENT_TYPE_TEXT        - text
 * @li@c CONTENT_TYPE_VIDEO       - video
 */
typedef enum 
{
    CONTENT_TYPE_APPLICATION = 1,
    CONTENT_TYPE_AUDIO,
    CONTENT_TYPE_EXAMPLE,
    CONTENT_TYPE_IMAGE,
    CONTENT_TYPE_MESSAGE,
    CONTENT_TYPE_MODEL,
    CONTENT_TYPE_MULTIPART,
    CONTENT_TYPE_TEXT,
    CONTENT_TYPE_VIDEO
} CONTENT_TYPE_T;

/*!
 * @enum HTTP_DOWNLOAD_NOTIFY_STATE_T
 * @brief HTTP download notify state
 * @code
 * typedef enum
 * {
 *     HTTP_STATE_DATA_BLOCK       =  0,
 *     HTTP_STATE_DATA_LAST_BLOCK,
 *     HTTP_STATE_XML_RESPONSE,
 *     HTTP_STATE_DOWNLOADING,  
 *     HTTP_STATE_RECEIVE_DONE,
 * 
 *     HTTP_STATE_SESSION_ERROR     = -65535,
 *     HTTP_STATE_SESSION_TIME_OUT,  
 *     HTTP_STATE_RESOLVE_URL_ERROR,
 *     HTTP_STATE_DOWNLOAD_DATA_ERROR,
 *     HTTP_STATE_SERVER_DOWNLOAD_ERROR
 * } HTTP_DOWNLOAD_NOTIFY_STATE_T;
 * @endcode
 *
 * @li@c HTTP_STATE_DATA_BLOCK              - data block
 * @li@c HTTP_STATE_DATA_LAST_BLOCK         - data last block
 * @li@c HTTP_STATE_XML_RESPONSE            - xml response
 * @li@c HTTP_STATE_DOWNLOADING             - downloading
 * @li@c HTTP_STATE_RECEIVE_DONE            - receive done
 * @li@c HTTP_STATE_SESSION_ERROR           - session error
 * @li@c HTTP_STATE_SESSION_TIME_OUT        - session timeout
 * @li@c HTTP_STATE_RESOLVE_URL_ERROR       - resolve URL error
 * @li@c HTTP_STATE_DOWNLOAD_DATA_ERROR     - download data error
 * @li@c HTTP_STATE_SERVER_DOWNLOAD_ERROR   - server download error
 */
typedef enum
{
    HTTP_STATE_DATA_BLOCK       =  0,
    HTTP_STATE_DATA_LAST_BLOCK,
    HTTP_STATE_XML_RESPONSE,
    HTTP_STATE_DOWNLOADING,  
    HTTP_STATE_RECEIVE_DONE,

    HTTP_STATE_SESSION_ERROR     = -65535,
    HTTP_STATE_SESSION_TIME_OUT,  
    HTTP_STATE_RESOLVE_URL_ERROR,
    HTTP_STATE_DOWNLOAD_DATA_ERROR,
    HTTP_STATE_SERVER_DOWNLOAD_ERROR
} HTTP_DOWNLOAD_NOTIFY_STATE_T;

/*!
 * @enum HTTP_RET_CODE_T
 * @brief HTTP download notify state
 * @code
 * typedef enum 
 * {
 * 	HTTP_RET_OK = 0,
 *     HTTP_RET_OSAI_SEND_MSG_FAILED = -65535,
 *     HTTP_RET_OSAI_SEMA_LOCK_FAILED,
 *     HTTP_RET_WRONG_XML_FMT,
 *     HTTP_RET_CANNOT_CONNECT,
 *     HTTP_RET_RANGE_INVALID,
 *     HTTP_RET_INVALID_URL
 * } HTTP_RET_CODE_T;
 * @endcode
 *
 * @li@c HTTP_RET_OK                    - OK
 * @li@c HTTP_RET_OSAI_SEND_MSG_FAILED  - osai send message failed
 * @li@c HTTP_RET_OSAI_SEMA_LOCK_FAILED - osai sema lock failed
 * @li@c HTTP_RET_WRONG_XML_FMT         - wrong xml format
 * @li@c HTTP_RET_CANNOT_CONNECT        - cannot connect server
 * @li@c HTTP_RET_RANGE_INVALID         - invalid range (http header)
 * @li@c HTTP_RET_INVALID_URL           - invalid URL (internet address)
 */
typedef enum 
{
	HTTP_RET_OK = 0,
    HTTP_RET_OSAI_SEND_MSG_FAILED = -65535,
    HTTP_RET_OSAI_SEMA_LOCK_FAILED,
    HTTP_RET_WRONG_XML_FMT,
    HTTP_RET_CANNOT_CONNECT,
    HTTP_RET_RANGE_INVALID,
    HTTP_RET_INVALID_URL
} HTTP_RET_CODE_T;

typedef struct http_fw_download_ctx_s HTTP_CTX_T;

/*!
 * @brief http client callback function
 * @param in pt_ctx           - current http context
 * @param in pac_buf          - http response data (http body/content)
 * @param in i4_download_size - response data length 
 * @param in e_state          - download notify state
 * @return return the error code
 * @retval 0   - OK
 * @retval <0  - failed
 */
typedef INT32 (*x_http_fw_dn_read_nfy) (HTTP_CTX_T *pt_ctx, CHAR *pac_buf, INT32 i4_download_size, HTTP_DOWNLOAD_NOTIFY_STATE_T e_state);

/*!
 * @struct http_fw_download_ctx_s
 * @brief HTTP client struct
 * @code
 * struct http_fw_download_ctx_s
 * {
 *     VOID                            *pt_hm;            
 *     VOID                            *pt_hc;            
 *     VOID                            *pt_session;       
 *     CONTENT_TYPE_T                  e_dload_cnt_type;  
 *     HTTP_RET_CODE_T                 e_ret_code;        
 *     INT32                           i4_method;         
 *     HTTP_RESPONSE_STATUS_CODE_T     e_rsp_status;      
 *     CHAR                            *ps_url;           
 *     CHAR                            *ps_body;          
 *     x_http_fw_dn_read_nfy           pf_nfy;            
 *     CHAR                            *pac_buf;          
 *     INT32                           i4_buf_len;        
 *     INT32                           i4_buf_use_size;   
 *     UINT32                          ui4_received_size; 
 *     UINT32                          ui4_content_size;  
 *     INT32                           i4_abort;          
 *     INT32                           i4_done;
 *     CHAR                            s_host[256];       
 *     VOID                            *pv_rsp;
 *     HANDLE_T                        h_timer;           
 * };
 * @endcode
 *
 * @li@c pt_hm              - master buffer 
 * @li@c pt_hc              - http client  
 * @li@c pt_session         - connection session 
 * @li@c e_dload_cnt_type   - download CONTENT_TYPE_T 
 * @li@c e_ret_code         - return code 
 * @li@c i4_method          - http method 
 * @li@c e_rsp_status       - http response status code 
 * @li@c ps_url             - server's URL 
 * @li@c ps_body            - request body 
 * @li@c pf_nfy             - read nfy 
 * @li@c pac_buf            - buffer to store download data 
 * @li@c i4_buf_len         - buffer size 
 * @li@c i4_buf_use_size    - data has download 
 * @li@c ui4_received_size  - received size 
 * @li@c ui4_content_size   - content size 
 * @li@c i4_abort           - abort context flag 
 * @li@c i4_done            - done
 * @li@c s_host[256]        - host fqdn or ip address 
 * @li@c pv_rsp             - rsponse data temporary
 * @li@c h_timer            - delay close timer 
 */
struct http_fw_download_ctx_s
{
    VOID                            *pt_hm;            /* master buffer */
    VOID                            *pt_hc;            /* http client  */
    VOID                            *pt_session;       /* connection session */
    CONTENT_TYPE_T                  e_dload_cnt_type;  /* download CONTENT_TYPE_T */
    HTTP_RET_CODE_T                 e_ret_code;        /* return code */
    INT32                           i4_method;         /* http method */
    HTTP_RESPONSE_STATUS_CODE_T     e_rsp_status;      /* http response status code */
                                                 
    CHAR                            *ps_url;           /* server's URL */
    CHAR                            *ps_body;          /* request body */

    x_http_fw_dn_read_nfy           pf_nfy;            /* read nfy */
    CHAR                            *pac_buf;          /* buffer to store download data */
    INT32                           i4_buf_len;        /* buffer size */
    INT32                           i4_buf_use_size;   /* #of data has download */

    UINT32                          ui4_received_size; /* received size */
    UINT32                          ui4_content_size;  /* content size */
    INT32                           i4_abort;          /* abort context flag */
    INT32                           i4_done;

    CHAR                            s_host[256];       /* host fqdn or ip address */
    VOID                     *pv_rsp;
        HANDLE_T                        h_timer;           /* delay close timer */

} ;

#endif

/*! @} */

