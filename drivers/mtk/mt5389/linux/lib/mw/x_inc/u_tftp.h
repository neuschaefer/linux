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

/*!
 * @file u_tftp.h
 * @brief tftp interface
 */

#ifndef U_TFTP_H
#define U_TFTP_H

#include "u_common.h"

#define TFTP_DATA_RECEIVE_TIMEOUT 1     /* second */        /**< tftp data receive timeout */
#define TFTP_BACK_OFF_START       1     /* second */        /**< back off start */
#define TFTP_BACK_OFF_END         16    /* seconds */       /**< back off end */
#define TFTP_REQUEST_RETRIES      16                        /**< request retry count */
#define TFTP_DOWNLOAD_RETRIES     3                         /**< download retry count */
#define TFTP_WAIT                 600   /* 10 minutes, the wait between TFTP retry sequences */     /**< tftp wait time */

/* configable item */
#define TFTP_CLIENT_RECEIVE_TIME_OUT            1           /**< tftp client receive timeout */
#define TFTP_CLIENT_BACKOFF_START               2           /**< tftp client backoff start */
#define TFTP_CLIENT_BACKOFF_END                 3           /**< tftp client backoff end */
#define TFTP_CLIENT_REQUEST_RETRY               4           /**< tftp client request retry */
#define TFTP_CLIENT_DOWNLOAD_RETRY              5           /**< tftp client download retry */
#define TFTP_CLEINT_DOWNLOAD_RETRY_WAIT         6           /**< tftp client download retry wait */
#define TFTP_CLIENT_NOTIFY                      7           /**< tftp client notify */

/*!
 * @brief tftp error code
 * @code
 * enum
 * {
 *     eTftp_ndf               = 0,  
 *     eTftp_file_not_found    = 1,  
 *     eTftp_access_violation  = 2,  
 *     eTftp_full              = 3,  
 *     eTftp_bad_op            = 4,  
 *     eTftp_bad_id            = 5,  
 *     eTftp_exist             = 6,  
 *     eTftp_no_user           = 7,  
 *     eTftp_socket_failed     = 100,
 *     eTftp_server_not_exist  = 101,
 *     eTftp_request_time_out  = 102,
 *     eTftp_read_abort        = 103,
 *     eTftp_read_retry        = 104,
 *     eTftp_ctx_null          = 105,
 *     eTftp_no_error          = 200,
 *     eTftp_continue_block    = 300,
 *     eTftp_last_block        = 301
 * };
 * @endcode
 * @li@c eTftp_ndf               -    Not defined, see error message (if any) 
 * @li@c eTftp_file_not_found    -    File not found 
 * @li@c eTftp_access_violation  -    Access violation 
 * @li@c eTftp_full              -    Disk full or allocation exceeded 
 * @li@c eTftp_bad_op            -    Illegal TFTP operation 
 * @li@c eTftp_bad_id            -    Unknow teansfer ID 
 * @li@c eTftp_exist             -    File already exists 
 * @li@c eTftp_no_user           -    No such user 
 * @li@c eTftp_socket_failed     -    Open socket failed 
 * @li@c eTftp_server_not_exist  -    TFTP Server not exist 
 * @li@c eTftp_request_time_out  -    TFTP request timeout 
 * @li@c eTftp_read_abort        -    Abort TFTP read operation 
 * @li@c eTftp_read_retry        -    do retry 
 * @li@c eTftp_ctx_null          -    tftp client context is null 
 * @li@c eTftp_no_error          -    no error 
 * @li@c eTftp_continue_block    -    download process need continue till last block 
 * @li@c eTftp_last_block        -    tftp last block 
 */
enum {
    /* error code in RFC 1350 */
    eTftp_ndf = 0,                   /* Not defined, see error message (if any) */
    eTftp_file_not_found = 1,        /* File not found */
    eTftp_access_violation = 2,      /* Access violation */
    eTftp_full = 3,                  /* Disk full or allocation exceeded */
    eTftp_bad_op = 4,                /* Illegal TFTP operation */
    eTftp_bad_id = 5,                /* Unknow teansfer ID */
    eTftp_exist = 6,                 /* File already exists */
    eTftp_no_user = 7,               /* No such user */

    /* internal error code */
    eTftp_socket_failed = 100,       /* Open socket failed */
    eTftp_server_not_exist = 101,    /* TFTP Server not exist */
    eTftp_request_time_out = 102,    /* TFTP request timeout */
    eTftp_read_abort = 103,          /* Abort TFTP read operation */
    eTftp_read_retry = 104,          /* do retry */
    eTftp_ctx_null = 105,            /* tftp client context is null */
    eTftp_no_error = 200,            /* no error */

    /* data transfer */
    eTftp_continue_block = 300,           /* download process need continue till last block */
    eTftp_last_block = 301
};

struct sockaddr_in;

/*!
 * @struct tftpCTX_T
 * @brief tftp context structure
 * @code
 * typedef struct tftp_ctx_s
 * {
 *     INT32 h_socket;
 *     INT32 i4_time_out;
 *     INT32 i4_backoff_start;
 *     INT32 i4_backoff_end; 
 *     INT32 i4_request_retry;
 *     INT32 i4_download_retry;
 *     INT32 i4_retry_wait;
 *     INT32 (*notify) (INT32 i4_code, CHAR* s_filename, UINT32 ui4_ip);
 *     UINT32 ui4_ip;
 *     INT32  i4_cur_request_retry;
 *     INT32  i4_cur_download_retry;
 *     UINT16 ui2_last_good_block;
 *     CHAR  *ps_file;
 *     CHAR   ac_pkt[516];
 *     INT32  i4_pkt_len;
 *     INT32  i4_quit;
 *     struct sockaddr_in t_tftp_server;
 *     struct sockaddr_in t_data_session;
 * }tftpCTX_T;
 * @endcode
 *
 * @li@c h_socket                 -   socket
 * @li@c i4_time_out              -   receive time out
 * @li@c i4_backoff_start         -   retry backoff start 
 * @li@c i4_backoff_end           -   retry backoff end 
 * @li@c i4_request_retry         -   maximum request retry number 
 * @li@c i4_download_retry        -   maximum download retry number 
 * @li@c i4_retry_wait            -   the gap between each download retry
 * @li@c notify                   -   event call back
 * @li@c ui4_ip                   -   server ip, now only ipv4 supported 
 * @li@c i4_cur_request_retry     -   current request retry 
 * @li@c i4_cur_download_retry    -   current download retry
 * @li@c ui2_last_good_block      -   last good block 
 * @li@c ps_file                  -   file name 
 * @li@c ac_pkt                   -   packet
 * @li@c i4_pkt_len               -   packet length
 * @li@c i4_quit                  -   quit
 * @li@c t_tftp_server            -   tftp server IP
 * @li@c t_data_session           -   tftp data session
 */
typedef struct tftp_ctx_s
{
    INT32 h_socket;                             /* socket */

    INT32 i4_time_out;                          /* receive time out */
    INT32 i4_backoff_start;                     /* retry backoff start */
    INT32 i4_backoff_end;                       /* retry backoff end */
    INT32 i4_request_retry;                     /* maximum request retry number */
    INT32 i4_download_retry;                    /* maximum download retry number */
    INT32 i4_retry_wait;                        /* the gap between each download retry */

    INT32 (*notify) (INT32 i4_code, CHAR* s_filename, UINT32 ui4_ip); /* event call back */

    /* internal */
    UINT32 ui4_ip;                              /* server ip, now only ipv4 supported */
    INT32  i4_cur_request_retry;
    INT32  i4_cur_download_retry;
    UINT16 ui2_last_good_block;                 /* last good block */
    CHAR  *ps_file;                             /* file name */
    CHAR   ac_pkt[516];
    INT32  i4_pkt_len;
    INT32  i4_quit;
    struct sockaddr_in t_tftp_server;
    struct sockaddr_in t_data_session;
}tftpCTX_T;

#endif

