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
/*! @file u_httpc_api.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains http client specific
 *         definitions, which are exported.
 */
/*----------------------------------------------------------------------------*/

/*!
* @defgroup    groupMW_INET_HTTP HTTP API
* @ingroup     groupMW_INET
* @brief       Provide an external API about HTTP.
* %INET_HTTP API would be used by user who wants to download by http protocol.
* 
*
* @{
* */

#ifndef _U_HTTPC_API_H_
#define _U_HTTPC_API_H_

#include "u_lnk_list.h"
#include "u_fm.h"

typedef struct _HTTPC_DATA_T HTTPC_DATA_T;                  /**<        */

/*------------------------------------------------------------------*/
/*! @struct _HTTPC_DATA_T
 *  @brief Save return data 
 *  @code
 *  typedef struct _HTTPC_DATA_T
 *  { 
 *	    CHAR                                              *pac_data;
 *	    UINT32                                             ui4_len;
 *	    HANDLE_T                                        h_session;
 *	    DLIST_ENTRY_T(_HTTPC_DATA_T)     t_link;
 *  };
 *  @endcode
 *  @li@c  pac_data                    -save return http body
 *  @li@c  ui4_len                       -size of pac_data
 */
/*------------------------------------------------------------------*/
struct _HTTPC_DATA_T
{
    CHAR                            *pac_data;
    UINT32                           ui4_len;
};

/*------------------------------------------------------------------*/
/*! @enum HTTPCR_CODE_T
 *  @brief Http error code, mapping from libcurl
 *  @code
 *  typedef enum _HTTPCR_CODE_T
 *  { 
 *	    HTTPR_OK = 0,          
 *	    HTTPR_UNSUPPORTED_PROTOCOL        = -1,
 *      HTTPR_FAILED_INIT                             = -2,
 *	    HTTPR_URL_MALFORMAT                      = -3,
 *	    HTTPR_OBSOLETE4                              = -4,
 *	    HTTPR_COULDNT_RESOLVE_PROXY      = -5,
 *	    HTTPR_COULDNT_RESOLVE_HOST        = -6,
 *	    HTTPR_COULDNT_CONNECT                 = -7,
 *	    HTTPR_FTP_WEIRD_SERVER_REPLY      = -8,
 *	    HTTPR_REMOTE_ACCESS_DENIED        = -9,
 *	    HTTPR_OBSOLETE10                            = -10,
 *	    HTTPR_FTP_WEIRD_PASS_REPLY          = -11,
 *	    HTTPR_OBSOLETE12                            = -12,
 *	    HTTPR_FTP_WEIRD_PASV_REPLY          = -13,
 *	    HTTPR_FTP_WEIRD_227_FORMAT          = -14,
 *	    HTTPR_FTP_CANT_GET_HOST               = -15,
 *	    HTTPR_OBSOLETE16                            = -16,
 *	    HTTPR_FTP_COULDNT_SET_TYPE          = -17,
 *	    HTTPR_PARTIAL_FILE                           = -18,
 *	    HTTPR_FTP_COULDNT_RETR_FILE         = -19,
 *	    HTTPR_OBSOLETE20                            = -20,
 *	    HTTPR_QUOTE_ERROR                         = -21,
 *	    HTTPR_HTTP_RETURNED_ERROR           = -22,
 *	    HTTPR_WRITE_ERROR                          = -23,
 *	    HTTPR_OBSOLETE24                             = -24,
 *	    HTTPR_UPLOAD_FAILED                        = -25,
 *	    HTTPR_READ_ERROR                            = -26,
 *	    HTTPR_OUT_OF_MEMORY                      = -27,
 *	    HTTPR_OPERATION_TIMEDOUT              = -28,
 *	    HTTPR_OBSOLETE29                              = -29,
 *	    HTTPR_FTP_PORT_FAILED                      = -30,
 *	    HTTPR_FTP_COULDNT_USE_REST           = -31,
 *	    HTTPR_OBSOLETE32                              = -32, 
 *	    HTTPR_RANGE_ERROR                           = -33,
 *	    HTTPR_HTTP_POST_ERROR                    = -34,
 *	    HTTPR_SSL_CONNECT_ERROR               = -35,
 *	    HTTPR_BAD_DOWNLOAD_RESUME         = -36,
 *	    HTTPR_FILE_COULDNT_READ_FILE         = -37,
 *	    HTTPR_LDAP_CANNOT_BIND                  = -38,
 *	    HTTPR_LDAP_SEARCH_FAILED               = -39,
 *	    HTTPR_OBSOLETE40                              = -40,
 *	    HTTPR_FUNCTION_NOT_FOUND              = -41,
 *	    HTTPR_ABORTED_BY_CALLBACK            = -42,
 *	    HTTPR_BAD_FUNCTION_ARGUMENT        = -43,
 *	    HTTPR_OBSOLETE44                              = -44,
 *	    HTTPR_INTERFACE_FAILED                    = -45,
 *	    HTTPR_OBSOLETE46                              = -46,
 *	    HTTPR_TOO_MANY_REDIRECTS               = -47,
 *	    HTTPR_UNKNOWN_TELNET_OPTION         = -48,
 *	    HTTPR_TELNET_OPTION_SYNTAX             = -49,
 *	    HTTPR_OBSOLETE50                               = -50,
 *	    HTTPR_PEER_FAILED_VERIFICATION        = -51,
 *	    HTTPR_GOT_NOTHING                             = -52,
 *	    HTTPR_SSL_ENGINE_NOTFOUND             = -53,
 *	    HTTPR_SSL_ENGINE_SETFAILED            = -54,
 *	    HTTPR_SEND_ERROR                             = -55,
 *	    HTTPR_RECV_ERROR                             = -56,
 *	    HTTPR_OBSOLETE57                              = -57,
 *	    HTTPR_SSL_CERTPROBLEM                    = -58,
 *	    HTTPR_SSL_CIPHER                              = -59,
 *	    HTTPR_SSL_CACERT                             = -60,
 *	    HTTPR_BAD_CONTENT_ENCODING         = -61,
 *	    HTTPR_LDAP_INVALID_URL                    = -62,
 *	    HTTPR_FILESIZE_EXCEEDED                 = -63,
 *	    HTTPR_USE_SSL_FAILED                       = -64,
 *	    HTTPR_SEND_FAIL_REWIND                   = -65,
 *	    HTTPR_SSL_ENGINE_INITFAILED             = -66,
 *	    HTTPR_LOGIN_DENIED                          = -67,
 *	    HTTPR_TFTP_NOTFOUND                       = -68,
 *	    HTTPR_TFTP_PERM                               = -69,
 *	    HTTPR_REMOTE_DISK_FULL                   = -70,
 *	    HTTPR_TFTP_ILLEGAL                             = -71,
 *	    HTTPR_TFTP_UNKNOWNID                       = -72,
 *	    HTTPR_REMOTE_FILE_EXISTS                  = -73,
 *	    HTTPR_TFTP_NOSUCHUSER                     = -74,
 *	    HTTPR_CONV_FAILED                           = -75,
 *	    HTTPR_CONV_REQD                             = -76,
 *	    HTTPR_SSL_CACERT_BADFILE              = -77,
 *	    HTTPR_REMOTE_FILE_NOT_FOUND           = -78,
 *	    HTTPR_SSH                                           = -79,
 *	    HTTPR_SSL_SHUTDOWN_FAILED            = -80,
 *	    HTTPR_AGAIN                                       = -81,
 *	    HTTPR_SSL_CRL_BADFILE                     = -82,
 *	    HTTPR_SSL_ISSUER_ERROR                 = -83,
 *      HTTPR_INVALID_HANDLE                          = -500,  
 *      HTTPR_SEND_MSG_FAILED                        = -501
 *  } HTTPCR_CODE_T;
 *  @endcode
 *  @li@c  HTTPR_OK                    -Success
 *  @li@c  HTTPR_UNSUPPORTED_PROTOCOL        -the url you passed to libcurl used a protocol that this libcurl does not support, the support might be a compile-time option that you didn't use, it can be a misspelled protocol string or just a protocol libcurl has no code for
 *  @li@c  HTTPR_FAILED_INIT                 -very early initialization code failed, this is likely to be an internal error or problem
 *  @li@c  HTTPR_URL_MALFORMAT               -the rul was not properly formatted
 *  @li@c  HTTPR_OBSOLETE4                   -
 *  @li@c  HTTPR_COULDNT_RESOLVE_PROXY       -couldn't resolve proxy, the given proxy host could not be resolved
 *  @li@c  HTTPR_COULDNT_RESOLVE_HOST        -couldn't resolve host, the given remote host was not resolved
 *  @li@c  HTTPR_COULDNT_CONNECT             -failed to connect() to host or proxy
 *  @li@c  HTTPR_FTP_WEIRD_SERVER_REPLY      -after connecting to a ftp server, libcurl expects to get a certain reply back, this error code implies that it  got a strange or bad reply, the given remote server is probably not an ok ftp server
 *  @li@c  HTTPR_REMOTE_ACCESS_DENIED        -we were denied access to the resource given in the url, for ftp, this occurs while trying to change to the remote directory
 *  @li@c  HTTPR_OBSOLETE10                  -
 *  @li@c  HTTPR_FTP_WEIRD_PASS_REPLY        -after having sent the ftp password to the server, libcurl expects a proper reply, this error code indicates that an unexpected code was returned
 *  @li@c  HTTPR_OBSOLETE12                  -
 *  @li@c  HTTPR_FTP_WEIRD_PASV_REPLY        -libcurl failed to get a sensible result back from the server as a response to the either a PASV or a EPSV command, the server is flawed
 *  @li@c  HTTPR_FTP_WEIRD_227_FORMAT        -ftp servers return a 227-line as a response to a PASV connand, if libcurl fails to parse that line, this return code is passed back
 *  @li@c  HTTPR_FTP_CANT_GET_HOST           -an internal failure to lookup the host used for the new connection
 *  @li@c  HTTPR_OBSOLETE16                  -
 *  @li@c  HTTPR_FTP_COULDNT_SET_TYPE        -received an error when trying to set the transfer mode to binary or ASCII
 *  @li@c  HTTPR_PARTIAL_FILE                -a file transfer was shorter or larger than expected, this happens when the server first reports an expected transfer size, and then delivers data that doesn't match the previously given size
 *  @li@c  HTTPR_FTP_COULDNT_RETR_FILE       -this was either a weird reply to a "RETR" command or a zero byte transfer complete
 *  @li@c  HTTPR_OBSOLETE20                  -
 *  @li@c  HTTPR_QUOTE_ERROR                 -when sending custom "QUOTE" commands to the remote server, one of the commands returned an error code that was 400 or higher(for ftp) or otherwise indicated unsuccessful completion of the command
 *  @li@c  HTTPR_HTTP_RETURNED_ERROR         -this is returned if CURLOPT_FAILONERROR is set TRUE and http server returns an error code that is >=400, this error code was formely known as CURLE_HTTP_NOT_FOUND
 *  @li@c  HTTPR_WRITE_ERROR                 -an error occurred when writing received data to a local file, of an error was returned to libcurl from a write callback
 *  @li@c  HTTPR_OBSOLETE24                  -
 *  @li@c  HTTPR_UPLOAD_FAILED               -failed starting the upload, for ftp, the server typically denied the STOR command, the error buffer usually contains the server's explanation for this, this error code was formerly known as CURLE_COULDNT_STOR_FILE
 *  @li@c  HTTPR_READ_ERROR                  -there was a problem reading a local file or an error returned by the read callback
 *  @li@c  HTTPR_OUT_OF_MEMORY               -a memory allocation request failed, this is serious badness and things are severely screwed up if this ever occurs
 *  @li@c  HTTPR_OPERATION_TIMEDOUT          -operation timeout, the specified time-out period was reached according to the conditions
 *  @li@c  HTTPR_OBSOLETE29                  -
 *  @li@c  HTTPR_FTP_PORT_FAILED             -the FTP PORT command returned error, this mostly happens when you haven't specified a good enough address for libcurl to use
 *  @li@c  HTTPR_FTP_COULDNT_USE_REST        -the FTP REST command returned error, this should never happen if the server is sane
 *  @li@c  HTTPR_OBSOLETE32                  -
 *  @li@c  HTTPR_RANGE_ERROR                 -the server does not support or accept range requests
 *  @li@c  HTTPR_HTTP_POST_ERROR             -this is an odd error that mainly occurs due to internal confusion
 *  @li@c  HTTPR_SSL_CONNECT_ERROR           -a problem occurred somewhere in the SSL/TLS handshake, you realy want the error buffer and read the message there as it pinpoints the problem slightly more, could be certificates(file formats, paths,permissions),passwords,and others
 *  @li@c  HTTPR_BAD_DOWNLOAD_RESUME         -attemping ftp resume beyond file size
 *  @li@c  HTTPR_FILE_COULDNT_READ_FILE      -a file given with file:// couldn't be opened, most likely because the file path doesn'tidentify an existing file
 *  @li@c  HTTPR_LDAP_CANNOT_BIND            -LDAP cannot bind, LDAP bind operation failed
 *  @li@c  HTTPR_LDAP_SEARCH_FAILED          -LDAP search filed
 *  @li@c  HTTPR_OBSOLETE40                  -
 *  @li@c  HTTPR_FUNCTION_NOT_FOUND          -function not found, a required zlib function was not found
 *  @li@c  HTTPR_ABORTED_BY_CALLBACK         -aborted by callback, a callback returned "abort" to libcurl
 *  @li@c  HTTPR_BAD_FUNCTION_ARGUMENT       -internal error, a function was called with a bad parameter
 *  @li@c  HTTPR_OBSOLETE44                  -
 *  @li@c  HTTPR_INTERFACE_FAILED            -interface error, a specified outgoing interface could not be used, set which interface to use for outgoing connections' source ip address with CURLOPT_INTERFACE
 *  @li@c  HTTPR_OBSOLETE46                  -
 *  @li@c  HTTPR_TOO_MANY_REDIRECTS          -too many redirects, when following redirects, libcurl hit the maximum amount
 *  @li@c  HTTPR_UNKNOWN_TELNET_OPTION       -an option set with CURLOPT_TELNETOPTIONS was not recognized
 *  @li@c  HTTPR_TELNET_OPTION_SYNTAX        -a telnet option string was illegally formatted
 *  @li@c  HTTPR_OBSOLETE50                  -
 *  @li@c  HTTPR_PEER_FAILED_VERIFICATION    -the remote server's SSL certificate or SSH md5 fingerprint was deemed not ok
 *  @li@c  HTTPR_GOT_NOTHING                 -nothing was returned from the server, and under the circumstances, getting nothing is considered an error 
 *  @li@c  HTTPR_SSL_ENGINE_NOTFOUND         -the specified crypto engine wasn't found
 *  @li@c  HTTPR_SSL_ENGINE_SETFAILED        -failed setting the selected SSL crypto engine as default
 *  @li@c  HTTPR_SEND_ERROR                  -failed sending network data
 *  @li@c  HTTPR_RECV_ERROR                  -failure with receiving network data
 *  @li@c  HTTPR_OBSOLETE57                  -
 *  @li@c  HTTPR_SSL_CERTPROBLEM             -problem with the local client certificate
 *  @li@c  HTTPR_SSL_CIPHER                  -couldn't use specified cipher
 *  @li@c  HTTPR_SSL_CACERT                  -peer certificate cannot be authenticated with known CA certificates
 *  @li@c  HTTPR_BAD_CONTENT_ENCODING        -unrecognized transfer encoding
 *  @li@c  HTTPR_LDAP_INVALID_URL            -invalid LDAP url
 *  @li@c  HTTPR_FILESIZE_EXCEEDED           -maximum file size exceeded
 *  @li@c  HTTPR_USE_SSL_FAILED              -requested ftp SSL level failed
 *  @li@c  HTTPR_SEND_FAIL_REWIND            -when doing a send operation curl had to rewind the data to retransmit, but the rewinding operation failed
 *  @li@c  HTTPR_SSL_ENGINE_INITFAILED       -initiating the SSL engine failed
 *  @li@c  HTTPR_LOGIN_DENIED                -the remote server denied curl to login
 *  @li@c  HTTPR_TFTP_NOTFOUND               -file not found on tftp server
 *  @li@c  HTTPR_TFTP_PERM                   -permission problem on tftp server
 *  @li@c  HTTPR_REMOTE_DISK_FULL            -out of disk space on the server
 *  @li@c  HTTPR_TFTP_ILLEGAL                -illegal tftp operation
 *  @li@c  HTTPR_TFTP_UNKNOWNID              -unknown tftp transfer id
 *  @li@c  HTTPR_REMOTE_FILE_EXISTS          -file already exists and will not be overwritten
 *  @li@c  HTTPR_TFTP_NOSUCHUSER             -this error should never be returned by a properly functioning tftp server
 *  @li@c  HTTPR_CONV_FAILED                 -character conversion failed
 *  @li@c  HTTPR_CONV_REQD                   -caller must register conversion callbacks
 *  @li@c  HTTPR_SSL_CACERT_BADFILE          -problem with reading the SSL CA cert
 *  @li@c  HTTPR_REMOTE_FILE_NOT_FOUND       -the resource referenced in the url does not exist
 *  @li@c  HTTPR_SSH                         -an unspecified error occurred during the SSH session
 *  @li@c  HTTPR_SSL_SHUTDOWN_FAILED         -failed to shut down the SSL connection
 *  @li@c  HTTPR_AGAIN                       -socket is not ready for send/recv wait till it's ready and try again, this return code is only returned from curl_easy_recv() and curl_easy_send()
 *  @li@c  HTTPR_SSL_CRL_BADFILE             -failed to load CRL file
 *  @li@c  HTTPR_SSL_ISSUER_ERROR            -issuer check failed
 *  @li@c  HTTPR_INVALID_HANDLE              -
 *  @li@c  HTTPR_SEND_MSG_FAILED             -
 */
/*------------------------------------------------------------------*/
typedef enum _HTTPCR_CODE_T
{
    HTTPR_OK = 0,

    HTTPR_UNSUPPORTED_PROTOCOL        = -1,
    HTTPR_FAILED_INIT                 = -2,
    HTTPR_URL_MALFORMAT               = -3,
    HTTPR_OBSOLETE4                   = -4,
    HTTPR_COULDNT_RESOLVE_PROXY       = -5,
    HTTPR_COULDNT_RESOLVE_HOST        = -6,
    HTTPR_COULDNT_CONNECT             = -7,
    HTTPR_FTP_WEIRD_SERVER_REPLY      = -8,
    HTTPR_REMOTE_ACCESS_DENIED        = -9,
    HTTPR_OBSOLETE10                  = -10,
    HTTPR_FTP_WEIRD_PASS_REPLY        = -11,
    HTTPR_OBSOLETE12                  = -12,
    HTTPR_FTP_WEIRD_PASV_REPLY        = -13,
    HTTPR_FTP_WEIRD_227_FORMAT        = -14,
    HTTPR_FTP_CANT_GET_HOST           = -15,
    HTTPR_OBSOLETE16                  = -16,
    HTTPR_FTP_COULDNT_SET_TYPE        = -17,
    HTTPR_PARTIAL_FILE                = -18,
    HTTPR_FTP_COULDNT_RETR_FILE       = -19,
    HTTPR_OBSOLETE20                  = -20,
    HTTPR_QUOTE_ERROR                 = -21,
    HTTPR_HTTP_RETURNED_ERROR         = -22,
    HTTPR_WRITE_ERROR                 = -23,
    HTTPR_OBSOLETE24                  = -24,
    HTTPR_UPLOAD_FAILED               = -25,
    HTTPR_READ_ERROR                  = -26,
    HTTPR_OUT_OF_MEMORY               = -27,
    HTTPR_OPERATION_TIMEDOUT          = -28,
    HTTPR_OBSOLETE29                  = -29,
    HTTPR_FTP_PORT_FAILED             = -30,
    HTTPR_FTP_COULDNT_USE_REST        = -31,
    HTTPR_OBSOLETE32                  = -32, 
    HTTPR_RANGE_ERROR                 = -33,
    HTTPR_HTTP_POST_ERROR             = -34,
    HTTPR_SSL_CONNECT_ERROR           = -35,
    HTTPR_BAD_DOWNLOAD_RESUME         = -36,
    HTTPR_FILE_COULDNT_READ_FILE      = -37,
    HTTPR_LDAP_CANNOT_BIND            = -38,
    HTTPR_LDAP_SEARCH_FAILED          = -39,
    HTTPR_OBSOLETE40                  = -40,
    HTTPR_FUNCTION_NOT_FOUND          = -41,
    HTTPR_ABORTED_BY_CALLBACK         = -42,
    HTTPR_BAD_FUNCTION_ARGUMENT       = -43,
    HTTPR_OBSOLETE44                  = -44,
    HTTPR_INTERFACE_FAILED            = -45,
    HTTPR_OBSOLETE46                  = -46,
    HTTPR_TOO_MANY_REDIRECTS          = -47,
    HTTPR_UNKNOWN_TELNET_OPTION       = -48,
    HTTPR_TELNET_OPTION_SYNTAX        = -49,
    HTTPR_OBSOLETE50                  = -50,
    HTTPR_PEER_FAILED_VERIFICATION    = -51,
    HTTPR_GOT_NOTHING                 = -52,
    HTTPR_SSL_ENGINE_NOTFOUND         = -53,
    HTTPR_SSL_ENGINE_SETFAILED        = -54,
    HTTPR_SEND_ERROR                  = -55,
    HTTPR_RECV_ERROR                  = -56,
    HTTPR_OBSOLETE57                  = -57,
    HTTPR_SSL_CERTPROBLEM             = -58,
    HTTPR_SSL_CIPHER                  = -59,
    HTTPR_SSL_CACERT                  = -60,
    HTTPR_BAD_CONTENT_ENCODING        = -61,
    HTTPR_LDAP_INVALID_URL            = -62,
    HTTPR_FILESIZE_EXCEEDED           = -63,
    HTTPR_USE_SSL_FAILED              = -64,
    HTTPR_SEND_FAIL_REWIND            = -65,
    HTTPR_SSL_ENGINE_INITFAILED       = -66,
    HTTPR_LOGIN_DENIED                = -67,
    HTTPR_TFTP_NOTFOUND               = -68,
    HTTPR_TFTP_PERM                   = -69,
    HTTPR_REMOTE_DISK_FULL            = -70,
    HTTPR_TFTP_ILLEGAL                = -71,
    HTTPR_TFTP_UNKNOWNID              = -72,
    HTTPR_REMOTE_FILE_EXISTS          = -73,
    HTTPR_TFTP_NOSUCHUSER             = -74,
    HTTPR_CONV_FAILED                 = -75,
    HTTPR_CONV_REQD                   = -76,
    HTTPR_SSL_CACERT_BADFILE          = -77,
    HTTPR_REMOTE_FILE_NOT_FOUND       = -78,
    HTTPR_SSH                         = -79,
    HTTPR_SSL_SHUTDOWN_FAILED         = -80,
    HTTPR_AGAIN                       = -81,
    HTTPR_SSL_CRL_BADFILE             = -82,
    HTTPR_SSL_ISSUER_ERROR            = -83,

    HTTPR_INVALID_HANDLE              = -500,  
    HTTPR_SEND_MSG_FAILED             = -501
} HTTPCR_CODE_T;

/*------------------------------------------------------------------*/
/*! @enum HTTPC_ASYNC_COND_T
 *  @brief Http error code, mapping from libcurl, used in callback
 *  @code
 *  typedef enum _HTTPC_ASYNC_COND_T
 *  { 
 *	    HTTPC_ASYNC_COND_OK = 0,          
 *	    HTTPC_ASYNC_COND_UNSUPPORTED_PROTOCOL               = -1,
 *      HTTPC_ASYNC_COND_FAILED_INIT                                       = -2,
 *	    HTTPC_ASYNC_COND_MALFORMAT                                       = -3,
 *	    HTTPC_ASYNC_COND_COULDNT_RESOLVE_PROXY                 = -5,
 *	    HTTPC_ASYNC_COND_COULDNT_RESOLVE_HOST                  = -6,
 *	    HTTPC_ASYNC_COND_COULDNT_CONNECT                          = -7,
 *	    HTTPC_ASYNC_COND_FTP_WEIRD_SERVER_REPLY               = -8,
 *	    HTTPC_ASYNC_COND_REMOTE_ACCESS_DENIED                 = -9,
 *	    HTTPC_ASYNC_COND_OBSOLETE10                                     = -10,
 *	    HTTPC_ASYNC_COND_FTP_WEIRD_PASS_REPLY                     = -11,
 *	    HTTPC_ASYNC_COND_FTP_WEIRD_PASV_REPLY                     = -13,
 *	    HTTPC_ASYNC_COND_FTP_WEIRD_227_FORMAT                     = -14,
 *	    HTTPC_ASYNC_COND_FTP_CANT_GET_HOST                          = -15,
 *	    HTTPC_ASYNC_COND_OBSOLETE16                                     = -16,
 *	    HTTPC_ASYNC_COND_FTP_COULDNT_SET_TYPE                   = -17,
 *	    HTTPC_ASYNC_COND_PARTIAL_FILE                                   = -18,
 *	    HTTPC_ASYNC_COND_FTP_COULDNT_RETR_FILE                  = -19,
 *	    HTTPC_ASYNC_COND_OBSOLETE20                                     = -20,
 *	    HTTPC_ASYNC_COND_QUOTE_ERROR                                    = -21,
 *	    HTTPC_ASYNC_COND_HTTP_RETURNED_ERROR                    = -22,
 *	    HTTPC_ASYNC_COND_WRITE_ERROR                                    = -23,
 *	    HTTPC_ASYNC_COND_OBSOLETE24                                     = -24,
 *	    HTTPC_ASYNC_COND_UPLOAD_FAILED                                  = -25,
 *	    HTTPC_ASYNC_COND_READ_ERROR                                     = -26,
 *	    HTTPC_ASYNC_COND_OUT_OF_MEMORY                              = -27,
 *	    HTTPC_ASYNC_COND_OPERATION_TIMEDOUT                     = -28,
 *	    HTTPC_ASYNC_COND_FTP_PORT_FAILED                            = -30,
 *	    HTTPC_ASYNC_COND_FTP_COULDNT_USE_REST                   = -31,
 *	    HTTPC_ASYNC_COND_OBSOLETE32                                     = -32, 
 *	    HTTPC_ASYNC_COND_RANGE_ERROR                                    = -33,
 *	    HTTPC_ASYNC_COND_HTTP_POST_ERROR                            = -34,
 *	    HTTPC_ASYNC_COND_SSL_CONNECT_ERROR                        = -35,
 *	    HTTPC_ASYNC_COND_BAD_DOWNLOAD_RESUME                    = -36,
 *	    HTTPC_ASYNC_COND_FILE_COULDNT_READ_FILE                 = -37,
 *	    HTTPC_ASYNC_COND_LDAP_CANNOT_BIND                       = -38,
 *	    HTTPC_ASYNC_COND_LDAP_SEARCH_FAILED                     = -39,
 *	    HTTPC_ASYNC_COND_OBSOLETE40                                     = -40,
 *	    HTTPC_ASYNC_COND_FUNCTION_NOT_FOUND                     = -41,
 *	    HTTPC_ASYNC_COND_ABORTED_BY_CALLBACK                    = -42,
 *	    HTTPC_ASYNC_COND_BAD_FUNCTION_ARGUMENT                  = -43,
 *	    HTTPC_ASYNC_COND_OBSOLETE44                                     = -44,
 *	    HTTPC_ASYNC_COND_INTERFACE_FAILED                           = -45,
 *	    HTTPC_ASYNC_COND_OBSOLETE46                                     = -46,
 *	    HTTPC_ASYNC_COND_TOO_MANY_REDIRECTS                     = -47,
 *	    HTTPC_ASYNC_COND_UNKNOWN_TELNET_OPTION                = -48,
 *	    HTTPC_ASYNC_COND_TELNET_OPTION_SYNTAX                   = -49,
 *	    HTTPC_ASYNC_COND_OBSOLETE50                                     = -50,
 *	    HTTPC_ASYNC_COND_PEER_FAILED_VERIFICATION              = -51,
 *	    HTTPC_ASYNC_COND_GOT_NOTHING                                   = -52,
 *	    HTTPC_ASYNC_COND_SSL_ENGINE_NOTFOUND                    = -53,
 *	    HTTPC_ASYNC_COND_SSL_ENGINE_SETFAILED                    = -54,
 *	    HTTPC_ASYNC_COND_SEND_ERROR                                    = -55,
 *	    HTTPC_ASYNC_COND_RECV_ERROR                                     = -56,
 *	    HTTPC_ASYNC_COND_SSL_CERTPROBLEM                            = -58,
 *	    HTTPC_ASYNC_COND_SSL_CIPHER                                      = -59,
 *	    HTTPC_ASYNC_COND_SSL_CACERT                                     = -60,
 *	    HTTPC_ASYNC_COND_BAD_CONTENT_ENCODING                 = -61,
 *	    HTTPC_ASYNC_COND_LDAP_INVALID_URL                            = -62,
 *	    HTTPC_ASYNC_COND_FILESIZE_EXCEEDED                          = -63,
 *	    HTTPC_ASYNC_COND_USE_SSL_FAILED                               = -64,
 *	    HTTPC_ASYNC_COND_SEND_FAIL_REWIND                           = -65,
 *	    HTTPC_ASYNC_COND_SSL_ENGINE_INITFAILED                     = -66,
 *	    HTTPC_ASYNC_COND_LOGIN_DENIED                                  = -67,
 *	    HTTPC_ASYNC_COND_TFTP_NOTFOUND                                = -68,
 *	    HTTPC_ASYNC_COND_TFTP_PERM                                        = -69,
 *	    HTTPC_ASYNC_COND_REMOTE_DISK_FULL                           = -70,
 *	    HTTPC_ASYNC_COND_TFTP_ILLEGAL                                    = -71,
 *	    HTTPC_ASYNC_COND_TFTP_UNKNOWNID                             = -72,
 *	    HTTPC_ASYNC_COND_REMOTE_FILE_EXISTS                        = -73,
 *	    HTTPC_ASYNC_COND_TFTP_NOSUCHUSER                            = -74,
 *	    HTTPC_ASYNC_COND_CONV_FAILED                                    = -75,
 *	    HTTPC_ASYNC_COND_CONV_REQD                                      = -76,
 *	    HTTPC_ASYNC_COND_SSL_CACERT_BADFILE                       = -77,
 *	    HTTPC_ASYNC_COND_REMOTE_FILE_NOT_FOUND                 = -78,
 *	    HTTPC_ASYNC_COND_SSH                                                  = -79,
 *	    HTTPC_ASYNC_COND_SSL_SHUTDOWN_FAILED                    = -80,
 *	    HTTPC_ASYNC_COND_AGAIN                                               = -81,
 *	    HTTPC_ASYNC_COND_SSL_CRL_BADFILE                              = -82,
 *	    HTTPC_ASYNC_COND_SSL_ISSUER_ERROR                           = -83,
 *      HTTPC_ASYNC_COND_ABORT                                                 = -200,  
 *  } HTTPC_ASYNC_COND_T;
 *  @endcode
 *  @li@c  HTTPC_ASYNC_COND_OK                    -Success
 *  @li@c  HTTPC_ASYNC_COND_UNSUPPORTED_PROTOCOL        -the url you passed to libcurl used a protocol that this libcurl does not support, the support might be a compile-time option that you didn't use, it can be a misspelled protocol string or just a protocol libcurl has no code for
 *  @li@c  HTTPC_ASYNC_COND_FAILED_INIT                 -very early initialization code failed, this is likely to be an internal error or problem
 *  @li@c  HTTPC_ASYNC_COND_MALFORMAT               -the rul was not properly formatted
 *  @li@c  HTTPC_ASYNC_COND_COULDNT_RESOLVE_PROXY       -couldn't resolve proxy, the given proxy host could not be resolved
 *  @li@c  HTTPC_ASYNC_COND_COULDNT_RESOLVE_HOST        -couldn't resolve host, the given remote host was not resolved
 *  @li@c  HTTPC_ASYNC_COND_COULDNT_CONNECT             -failed to connect() to host or proxy
 *  @li@c  HTTPC_ASYNC_COND_FTP_WEIRD_SERVER_REPLY      -after connecting to a ftp server, libcurl expects to get a certain reply back, this error code implies that it  got a strange or bad reply, the given remote server is probably not an ok ftp server
 *  @li@c  HTTPC_ASYNC_COND_REMOTE_ACCESS_DENIED        -we were denied access to the resource given in the url, for ftp, this occurs while trying to change to the remote directory
 *  @li@c  HTTPC_ASYNC_COND_OBSOLETE10                  -
 *  @li@c  HTTPC_ASYNC_COND_FTP_WEIRD_PASS_REPLY        -after having sent the ftp password to the server, libcurl expects a proper reply, this error code indicates that an unexpected code was returned
 *  @li@c  HTTPC_ASYNC_COND_FTP_WEIRD_PASV_REPLY        -libcurl failed to get a sensible result back from the server as a response to the either a PASV or a EPSV command, the server is flawed
 *  @li@c  HTTPC_ASYNC_COND_FTP_WEIRD_227_FORMAT        -ftp servers return a 227-line as a response to a PASV connand, if libcurl fails to parse that line, this return code is passed back
 *  @li@c  HTTPC_ASYNC_COND_FTP_CANT_GET_HOST           -an internal failure to lookup the host used for the new connection
 *  @li@c  HTTPC_ASYNC_COND_OBSOLETE16                  -
 *  @li@c  HTTPC_ASYNC_COND_FTP_COULDNT_SET_TYPE        -received an error when trying to set the transfer mode to binary or ASCII
 *  @li@c  HTTPC_ASYNC_COND_PARTIAL_FILE                -a file transfer was shorter or larger than expected, this happens when the server first reports an expected transfer size, and then delivers data that doesn't match the previously given size
 *  @li@c  HTTPC_ASYNC_COND_FTP_COULDNT_RETR_FILE       -this was either a weird reply to a "RETR" command or a zero byte transfer complete
 *  @li@c  HTTPC_ASYNC_COND_OBSOLETE20                  -
 *  @li@c  HTTPC_ASYNC_COND_QUOTE_ERROR                 -when sending custom "QUOTE" commands to the remote server, one of the commands returned an error code that was 400 or higher(for ftp) or otherwise indicated unsuccessful completion of the command
 *  @li@c  HTTPC_ASYNC_COND_HTTP_RETURNED_ERROR         -this is returned if CURLOPT_FAILONERROR is set TRUE and http server returns an error code that is >=400, this error code was formely known as CURLE_HTTP_NOT_FOUND
 *  @li@c  HTTPC_ASYNC_COND_WRITE_ERROR                 -an error occurred when writing received data to a local file, of an error was returned to libcurl from a write callback
 *  @li@c  HTTPC_ASYNC_COND_OBSOLETE24                  -
 *  @li@c  HTTPC_ASYNC_COND_UPLOAD_FAILED               -failed starting the upload, for ftp, the server typically denied the STOR command, the error buffer usually contains the server's explanation for this, this error code was formerly known as CURLE_COULDNT_STOR_FILE
 *  @li@c  HTTPC_ASYNC_COND_READ_ERROR                  -there was a problem reading a local file or an error returned by the read callback
 *  @li@c  HTTPC_ASYNC_COND_OUT_OF_MEMORY               -a memory allocation request failed, this is serious badness and things are severely screwed up if this ever occurs
 *  @li@c  HTTPC_ASYNC_COND_OPERATION_TIMEDOUT          -operation timeout, the specified time-out period was reached according to the conditions
 *  @li@c  HTTPC_ASYNC_COND_FTP_PORT_FAILED             -the FTP PORT command returned error, this mostly happens when you haven't specified a good enough address for libcurl to use
 *  @li@c  HTTPC_ASYNC_COND_FTP_COULDNT_USE_REST        -the FTP REST command returned error, this should never happen if the server is sane
 *  @li@c  HTTPC_ASYNC_COND_OBSOLETE32                  -
 *  @li@c  HTTPC_ASYNC_COND_RANGE_ERROR                 -the server does not support or accept range requests
 *  @li@c  HTTPC_ASYNC_COND_HTTP_POST_ERROR             -this is an odd error that mainly occurs due to internal confusion
 *  @li@c  HTTPC_ASYNC_COND_SSL_CONNECT_ERROR           -a problem occurred somewhere in the SSL/TLS handshake, you realy want the error buffer and read the message there as it pinpoints the problem slightly more, could be certificates(file formats, paths,permissions),passwords,and others
 *  @li@c  HTTPC_ASYNC_COND_BAD_DOWNLOAD_RESUME         -attemping ftp resume beyond file size
 *  @li@c  HTTPC_ASYNC_COND_FILE_COULDNT_READ_FILE      -a file given with file:// couldn't be opened, most likely because the file path doesn'tidentify an existing file
 *  @li@c  HTTPC_ASYNC_COND_LDAP_CANNOT_BIND            -LDAP cannot bind, LDAP bind operation failed
 *  @li@c  HTTPC_ASYNC_COND_LDAP_SEARCH_FAILED          -LDAP search filed
 *  @li@c  HTTPC_ASYNC_COND_OBSOLETE40                  -
 *  @li@c  HTTPC_ASYNC_COND_FUNCTION_NOT_FOUND          -function not found, a required zlib function was not found
 *  @li@c  HTTPC_ASYNC_COND_ABORTED_BY_CALLBACK         -aborted by callback, a callback returned "abort" to libcurl
 *  @li@c  HTTPC_ASYNC_COND_BAD_FUNCTION_ARGUMENT       -internal error, a function was called with a bad parameter
 *  @li@c  HTTPC_ASYNC_COND_OBSOLETE44                  -
 *  @li@c  HTTPC_ASYNC_COND_INTERFACE_FAILED            -interface error, a specified outgoing interface could not be used, set which interface to use for outgoing connections' source ip address with CURLOPT_INTERFACE
 *  @li@c  HTTPC_ASYNC_COND_OBSOLETE46                  -
 *  @li@c  HTTPC_ASYNC_COND_TOO_MANY_REDIRECTS          -too many redirects, when following redirects, libcurl hit the maximum amount
 *  @li@c  HTTPC_ASYNC_COND_UNKNOWN_TELNET_OPTION       -an option set with CURLOPT_TELNETOPTIONS was not recognized
 *  @li@c  HTTPC_ASYNC_COND_TELNET_OPTION_SYNTAX        -a telnet option string was illegally formatted
 *  @li@c  HTTPC_ASYNC_COND_OBSOLETE50                  -
 *  @li@c  HTTPC_ASYNC_COND_PEER_FAILED_VERIFICATION    -the remote server's SSL certificate or SSH md5 fingerprint was deemed not ok
 *  @li@c  HTTPC_ASYNC_COND_GOT_NOTHING                 -nothing was returned from the server, and under the circumstances, getting nothing is considered an error 
 *  @li@c  HTTPC_ASYNC_COND_SSL_ENGINE_NOTFOUND         -the specified crypto engine wasn't found
 *  @li@c  HTTPC_ASYNC_COND_SSL_ENGINE_SETFAILED        -failed setting the selected SSL crypto engine as default
 *  @li@c  HTTPC_ASYNC_COND_SEND_ERROR                  -failed sending network data
 *  @li@c  HTTPC_ASYNC_COND_RECV_ERROR                  -failure with receiving network data
 *  @li@c  HTTPC_ASYNC_COND_SSL_CERTPROBLEM             -problem with the local client certificate
 *  @li@c  HTTPC_ASYNC_COND_SSL_CIPHER                  -couldn't use specified cipher
 *  @li@c  HTTPC_ASYNC_COND_SSL_CACERT                  -peer certificate cannot be authenticated with known CA certificates
 *  @li@c  HTTPC_ASYNC_COND_BAD_CONTENT_ENCODING        -unrecognized transfer encoding
 *  @li@c  HTTPC_ASYNC_COND_LDAP_INVALID_URL            -invalid LDAP url
 *  @li@c  HTTPC_ASYNC_COND_FILESIZE_EXCEEDED           -maximum file size exceeded
 *  @li@c  HTTPC_ASYNC_COND_USE_SSL_FAILED              -requested ftp SSL level failed
 *  @li@c  HTTPC_ASYNC_COND_SEND_FAIL_REWIND            -when doing a send operation curl had to rewind the data to retransmit, but the rewinding operation failed
 *  @li@c  HTTPC_ASYNC_COND_SSL_ENGINE_INITFAILED       -initiating the SSL engine failed
 *  @li@c  HTTPC_ASYNC_COND_LOGIN_DENIED                -the remote server denied curl to login
 *  @li@c  HTTPC_ASYNC_COND_TFTP_NOTFOUND               -file not found on tftp server
 *  @li@c  HTTPC_ASYNC_COND_TFTP_PERM                   -permission problem on tftp server
 *  @li@c  HTTPC_ASYNC_COND_REMOTE_DISK_FULL            -out of disk space on the server
 *  @li@c  HTTPC_ASYNC_COND_TFTP_ILLEGAL                -illegal tftp operation
 *  @li@c  HTTPC_ASYNC_COND_TFTP_UNKNOWNID              -unknown tftp transfer id
 *  @li@c  HTTPC_ASYNC_COND_REMOTE_FILE_EXISTS          -file already exists and will not be overwritten
 *  @li@c  HTTPC_ASYNC_COND_TFTP_NOSUCHUSER             -this error should never be returned by a properly functioning tftp server
 *  @li@c  HTTPC_ASYNC_COND_CONV_FAILED                 -character conversion failed
 *  @li@c  HTTPC_ASYNC_COND_CONV_REQD                   -caller must register conversion callbacks
 *  @li@c  HTTPC_ASYNC_COND_SSL_CACERT_BADFILE          -problem with reading the SSL CA cert
 *  @li@c  HTTPC_ASYNC_COND_REMOTE_FILE_NOT_FOUND       -the resource referenced in the url does not exist
 *  @li@c  HTTPC_ASYNC_COND_SSH                         -an unspecified error occurred during the SSH session
 *  @li@c  HTTPC_ASYNC_COND_SSL_SHUTDOWN_FAILED         -failed to shut down the SSL connection
 *  @li@c  HTTPC_ASYNC_COND_AGAIN                       -socket is not ready for send/recv wait till it's ready and try again, this return code is only returned from curl_easy_recv() and curl_easy_send()
 *  @li@c  HTTPC_ASYNC_COND_SSL_CRL_BADFILE             -failed to load CRL file
 *  @li@c  HTTPC_ASYNC_COND_SSL_ISSUER_ERROR            -issuer check failed
 *  @li@c  HTTPC_ASYNC_COND_ABORT              -
 */
/*------------------------------------------------------------------*/
typedef enum _HTTPC_ASYNC_COND_T
{
    HTTPC_ASYNC_COND_OK                          = 0,

    HTTPC_ASYNC_COND_UNSUPPORTED_PROTOCOL        = -1,
    HTTPC_ASYNC_COND_FAILED_INIT                 = -2,
    HTTPC_ASYNC_COND_MALFORMAT                   = -3,
    HTTPC_ASYNC_COND_COULDNT_RESOLVE_PROXY       = -5,
    HTTPC_ASYNC_COND_COULDNT_RESOLVE_HOST        = -6,
    HTTPC_ASYNC_COND_COULDNT_CONNECT             = -7,
    HTTPC_ASYNC_COND_FTP_WEIRD_SERVER_REPLY      = -8,
    HTTPC_ASYNC_COND_REMOTE_ACCESS_DENIED        = -9,
    HTTPC_ASYNC_COND_OBSOLETE10                  = -10,
    HTTPC_ASYNC_COND_FTP_WEIRD_PASS_REPLY        = -11,
    HTTPC_ASYNC_COND_FTP_WEIRD_PASV_REPLY        = -13,
    HTTPC_ASYNC_COND_FTP_WEIRD_227_FORMAT        = -14,
    HTTPC_ASYNC_COND_FTP_CANT_GET_HOST           = -15,
    HTTPC_ASYNC_COND_OBSOLETE16                  = -16,
    HTTPC_ASYNC_COND_FTP_COULDNT_SET_TYPE        = -17,
    HTTPC_ASYNC_COND_PARTIAL_FILE                = -18,
    HTTPC_ASYNC_COND_FTP_COULDNT_RETR_FILE       = -19,
    HTTPC_ASYNC_COND_OBSOLETE20                  = -20,
    HTTPC_ASYNC_COND_QUOTE_ERROR                 = -21,
    HTTPC_ASYNC_COND_HTTP_RETURNED_ERROR         = -22,
    HTTPC_ASYNC_COND_WRITE_ERROR                 = -23,
    HTTPC_ASYNC_COND_OBSOLETE24                  = -24,
    HTTPC_ASYNC_COND_UPLOAD_FAILED               = -25,
    HTTPC_ASYNC_COND_READ_ERROR                  = -26,
    HTTPC_ASYNC_COND_OUT_OF_MEMORY               = -27,
    HTTPC_ASYNC_COND_OPERATION_TIMEDOUT          = -28,
    HTTPC_ASYNC_COND_FTP_PORT_FAILED             = -30,
    HTTPC_ASYNC_COND_FTP_COULDNT_USE_REST        = -31,
    HTTPC_ASYNC_COND_OBSOLETE32                  = -32, 
    HTTPC_ASYNC_COND_RANGE_ERROR                 = -33,
    HTTPC_ASYNC_COND_HTTP_POST_ERROR             = -34,
    HTTPC_ASYNC_COND_SSL_CONNECT_ERROR           = -35,
    HTTPC_ASYNC_COND_BAD_DOWNLOAD_RESUME         = -36,
    HTTPC_ASYNC_COND_FILE_COULDNT_READ_FILE      = -37,
    HTTPC_ASYNC_COND_LDAP_CANNOT_BIND            = -38,
    HTTPC_ASYNC_COND_LDAP_SEARCH_FAILED          = -39,
    HTTPC_ASYNC_COND_OBSOLETE40                  = -40,
    HTTPC_ASYNC_COND_FUNCTION_NOT_FOUND          = -41,
    HTTPC_ASYNC_COND_ABORTED_BY_CALLBACK         = -42,
    HTTPC_ASYNC_COND_BAD_FUNCTION_ARGUMENT       = -43,
    HTTPC_ASYNC_COND_OBSOLETE44                  = -44,
    HTTPC_ASYNC_COND_INTERFACE_FAILED            = -45,
    HTTPC_ASYNC_COND_OBSOLETE46                  = -46,
    HTTPC_ASYNC_COND_TOO_MANY_REDIRECTS          = -47,
    HTTPC_ASYNC_COND_UNKNOWN_TELNET_OPTION       = -48,
    HTTPC_ASYNC_COND_TELNET_OPTION_SYNTAX        = -49,
    HTTPC_ASYNC_COND_OBSOLETE50                  = -50,
    HTTPC_ASYNC_COND_PEER_FAILED_VERIFICATION    = -51,
    HTTPC_ASYNC_COND_GOT_NOTHING                 = -52,
    HTTPC_ASYNC_COND_SSL_ENGINE_NOTFOUND         = -53,
    HTTPC_ASYNC_COND_SSL_ENGINE_SETFAILED        = -54,
    HTTPC_ASYNC_COND_SEND_ERROR                  = -55,
    HTTPC_ASYNC_COND_RECV_ERROR                  = -56,
    HTTPC_ASYNC_COND_SSL_CERTPROBLEM             = -58,
    HTTPC_ASYNC_COND_SSL_CIPHER                  = -59,
    HTTPC_ASYNC_COND_SSL_CACERT                  = -60,
    HTTPC_ASYNC_COND_BAD_CONTENT_ENCODING        = -61,
    HTTPC_ASYNC_COND_LDAP_INVALID_URL            = -62,
    HTTPC_ASYNC_COND_FILESIZE_EXCEEDED           = -63,
    HTTPC_ASYNC_COND_USE_SSL_FAILED              = -64,
    HTTPC_ASYNC_COND_SEND_FAIL_REWIND            = -65,
    HTTPC_ASYNC_COND_SSL_ENGINE_INITFAILED       = -66,
    HTTPC_ASYNC_COND_LOGIN_DENIED                = -67,
    HTTPC_ASYNC_COND_TFTP_NOTFOUND               = -68,
    HTTPC_ASYNC_COND_TFTP_PERM                   = -69,
    HTTPC_ASYNC_COND_REMOTE_DISK_FULL            = -70,
    HTTPC_ASYNC_COND_TFTP_ILLEGAL                = -71,
    HTTPC_ASYNC_COND_TFTP_UNKNOWNID              = -72,
    HTTPC_ASYNC_COND_REMOTE_FILE_EXISTS          = -73,
    HTTPC_ASYNC_COND_TFTP_NOSUCHUSER             = -74,
    HTTPC_ASYNC_COND_CONV_FAILED                 = -75,
    HTTPC_ASYNC_COND_CONV_REQD                   = -76,
    HTTPC_ASYNC_COND_SSL_CACERT_BADFILE          = -77,
    HTTPC_ASYNC_COND_REMOTE_FILE_NOT_FOUND       = -78,
    HTTPC_ASYNC_COND_SSH                         = -79,
    HTTPC_ASYNC_COND_SSL_SHUTDOWN_FAILED         = -80,
    HTTPC_ASYNC_COND_AGAIN                       = -81,
    HTTPC_ASYNC_COND_SSL_CRL_BADFILE             = -82,
    HTTPC_ASYNC_COND_SSL_ISSUER_ERROR            = -83,

    HTTPC_ASYNC_COND_ABORT                       = -200
} HTTPC_ASYNC_COND_T;

/*------------------------------------------------------------------*/
/*! @brief This is a callback function for get/post asynchronized function
 *  @param [in] h_req   -The http async request handle
 *  @param [in] pt_data   -The data save the memory block pointer which save the download data
 *  @param [in] pv_tag   -The data pass to callback function when call the async function
 *  @param [in] e_async_cond   -The http flow condition when call the callback
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_httpc_async_fct)(HANDLE_T            h_req,
                                     HTTPC_DATA_T       *pt_data,
                                     VOID               *pv_tag,
                                     HTTPC_ASYNC_COND_T  e_async_cond);

/*------------------------------------------------------------------*/
/*! @brief This is a callback function for progress callback function
 *  @param [in] pv_tag   -The data pass to callback function
 *  @param [in] ui8_down_total   -download total data
 *  @param [in] ui8_down_now   -download now data
 *  @param [in] ui8_up_total   -upload total data
 *  @param [in] ui8_up_now   -upload now data
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_httpc_progress_fct)(VOID* pv_tag,
                                     UINT64 ui8_down_total,
                                     UINT64 ui8_down_now,
                                     UINT64 ui8_up_total,
                                     UINT64 ui8_up_now);

/*------------------------------------------------------------------*/
/*! @brief This is a callback function for progress callback function
 *  @param [in] pv_tag   -The data pass to callback function
 *  @param [in] ui8_down_total   -download total data, if content length is not valid,
 *                                  this would be ~(0L)
 *  @param [in] ui8_down_now   -download now data
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_httpc_fm_progress_fct)(VOID* pv_tag,
                                     UINT64 ui8_down_total,
                                     UINT64 ui8_down_now);

/*------------------------------------------------------------------*/
/*! @enum HTTP_HANDLE_OPT_T
 *  @brief Argument for x_httpc_set_option
 *  @code
 *  typedef enum _HTTP_HANDLE_OPT_T
 *  { 
 *	    HTTP_HANDLE_OPT_PROXY,          
 *	    HTTP_HANDLE_OPT_TIMEOUT,
 *	    HTTP_HANDLE_OPT_INTERVAL_TIMEOUT,
 *	    HTTP_HANDLE_OPT_CONNECT_TIMEOUT,
 *        HTTP_HANDLE_OPT_FOLLOWLOCATION,          
 *	    HTTP_HANDLE_OPT_MAXREDIRS,
 *	    HTTP_HANDLE_OPT_SSL_VERIFYPEER,
 *	    HTTP_HANDLE_OPT_SSL_CERT,
 *	    HTTP_HANDLE_OPT_SSL_CERT_PATH,
 *	    HTTP_HANDLE_OPT_SSL_CERT_FILE,
 *  } HTTP_HANDLE_OPT_T;
 *  @endcode
 *  @li@c  HTTP_HANDLE_OPT_PROXY                           -char* string of proxy address
 *  @li@c  HTTP_HANDLE_OPT_TIMEOUT                        -int32 seconds, total timeout
 *  @li@c  HTTP_HANDLE_OPT_INTERVAL_TIMEOUT        -int32 seconds, interval timeout
 *  @li@c  HTTP_HANDLE_OPT_CONNECT_TIMEOUT        -int32 seconds, connect timeout
 *  @li@c  HTTP_HANDLE_OPT_FOLLOWLOCATION          -1 or 0
 *  @li@c  HTTP_HANDLE_OPT_MAXREDIRS                    -int32 max redir times
 *  @li@c  HTTP_HANDLE_OPT_SSL_VERIFYPEER             -1 or 0
 *  @li@c  HTTP_HANDLE_OPT_SSL_CERT                       -"OPENSSL PEM string", only use in httpc api
                                                                                       "-----BEGIN CERTIFICATE-----\n"\
                                                                                       "MIIHPTCCBSWgAwIBAgIBADANBgkqhkiG9w0BAQQFADB5MRAwDgYDVQQKEwdSb290\n"\
                                                                                       "omTxJBzcoTWcFbLUvFUufQb1nA5V9FrWk9p2rSVzTMVD\n"\
                                                                                       "-----END CERTIFICATE-----\n";
 *  @li@c  HTTP_HANDLE_OPT_SSL_CERT_PATH              -cert dir which contain certs
 *  @li@c  HTTP_HANDLE_OPT_SSL_CERT_FILE                -cert file full path
 */
/*------------------------------------------------------------------*/
typedef enum _HTTP_HANDLE_OPT_T
{
    HTTP_HANDLE_OPT_PROXY,
    HTTP_HANDLE_OPT_TIMEOUT,
    HTTP_HANDLE_OPT_INTERVAL_TIMEOUT,
    HTTP_HANDLE_OPT_CONNECT_TIMEOUT,
    HTTP_HANDLE_OPT_FOLLOWLOCATION,
    HTTP_HANDLE_OPT_MAXREDIRS,

    HTTP_HANDLE_OPT_SSL_VERIFYPEER,
    HTTP_HANDLE_OPT_SSL_CERT,
        
    HTTP_HANDLE_OPT_SSL_CERT_DIR,
    HTTP_HANDLE_OPT_SSL_CERT_FILE
}HTTP_HANDLE_OPT_T;


/*------------------------------------------------------------------*/
/*! @enum HTTP_HANDLE_INFO_T
 *  @brief Argument for x_httpc_get_info 
 *  @code
 *  typedef enum _HTTP_HANDLE_INFO_T
 *  { 
 *	    HTTP_HANDLE_INFO_RESPONSE_CODE,          
 *	    HTTP_HANDLE_INFO_CONTENT_LENGTH,
 *  } HTTP_HANDLE_INFO_T;
 *  @endcode
 *  @li@c  HTTP_HANDLE_INFO_RESPONSE_CODE                    -int 32, get response code information
 *  @li@c  HTTP_HANDLE_INFO_CONTENT_LENGTH                   -uint64, get content lenth information
 */
/*------------------------------------------------------------------*/
typedef enum _HTTP_HANDLE_INFO_T
{
    HTTP_HANDLE_INFO_RESPONSE_CODE,
    HTTP_HANDLE_INFO_CONTENT_LENGTH
}HTTP_HANDLE_INFO_T;

#endif

/*! @} */
