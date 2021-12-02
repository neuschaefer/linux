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
#ifndef _X_HTTPC_API_H_
#define _X_HTTPC_API_H_

#include "u_httpc_api.h"

/***************************************************************************/
/* API to init HTTP module                                                 */
/***************************************************************************/
extern VOID    x_httpc_init_module(VOID);

/***************************************************************************/
/* Basic HTTP Client APIs                                                  */
/***************************************************************************/
extern VOID    x_httpc_data_init(HTTPC_DATA_T *pt_data);
extern VOID    x_httpc_data_free(HTTPC_DATA_T *pt_data);

extern HTTPCR_CODE_T   x_httpc_new_session(HANDLE_T *ph_handle);
extern HTTPCR_CODE_T   x_httpc_free_session(HANDLE_T h_file);
extern HTTPCR_CODE_T   x_httpc_get(HANDLE_T h_file, 
                                   const CHAR *ps_url, 
                                   HTTPC_DATA_T *pt_data);


extern HTTPCR_CODE_T   x_httpc_post(HANDLE_T      h_file,
                                    const CHAR   *ps_url,
                                    VOID         *pv_body,
                                    SIZE_T        z_body_size,
                                    HTTPC_DATA_T *pt_data);


extern HTTPCR_CODE_T   x_httpc_get_async(HANDLE_T                h_file,
                                         const CHAR             *ps_url,
                                         HTTPC_DATA_T           *pt_data, 
                                         x_httpc_async_fct    pf_nfy_fct,
                                         VOID                   *pv_tag,
                                         HANDLE_T               *ph_req);

extern HTTPCR_CODE_T   x_httpc_post_async(HANDLE_T             h_file,
                                          const CHAR          *ps_url,
                                          VOID                *pv_body,
                                          SIZE_T               z_body_size,
                                          HTTPC_DATA_T        *pt_data,
                                          x_httpc_async_fct pf_nfy_fct,
                                          VOID                *pv_tag,
                                          HANDLE_T            *ph_req);

extern HTTPCR_CODE_T    x_httpc_put(HANDLE_T      h_file,
                                    const CHAR   *ps_url,
                                    VOID         *pv_body,
                                    SIZE_T        z_body_size,
                                    HTTPC_DATA_T *pt_data);

extern HTTPCR_CODE_T    x_httpc_put_async(HANDLE_T            h_file,
                                          const CHAR         *ps_url,
                                          VOID               *pv_body,
                                          SIZE_T              z_body_size,
                                          HTTPC_DATA_T       *pt_data,
                                          x_httpc_async_fct   pf_nfy_fct,
                                          VOID               *pv_tag,
                                          HANDLE_T           *ph_req);

extern HTTPCR_CODE_T   x_httpc_append_header(HANDLE_T    h_file, const CHAR *ps_hdr);

extern HTTPCR_CODE_T   x_httpc_clear_append_header(HANDLE_T h_file);

extern HTTPCR_CODE_T   x_httpc_abort_async(HANDLE_T h_req);

extern HTTPCR_CODE_T   x_httpc_set_option(HANDLE_T h_handle, HTTP_HANDLE_OPT_T e_option, VOID* pv_val);

extern HTTPCR_CODE_T   x_httpc_get_info(HANDLE_T h_handle, HTTP_HANDLE_INFO_T e_option, VOID* pv_val);

extern HTTPCR_CODE_T   x_httpc_set_progress_callback(HANDLE_T h_handle, x_httpc_progress_fct pf_progrs_cb, VOID* pv_tag);

/***************************************************************************/
/* Basic HTTP streaming API                                                */
/***************************************************************************/
extern INT32 x_httpc_fm_open(const CHAR *ps_url, 
                             UINT32 ui4_buffer_size, 
                             HANDLE_T *ph_handle);

extern INT32 x_httpc_fm_open_proxy(const CHAR *ps_url, 
                            const CHAR *ps_proxy, 
                            UINT32 ui4_buffer_size, 
                            HANDLE_T *ph_handle);

extern INT32 x_httpc_fm_close(HANDLE_T h_file);

extern INT32 x_httpc_fm_read(HANDLE_T h_file, 
                             VOID *pv_data, 
                             UINT32 ui4_count, 
                             UINT32 *pui4_read);
#if 1/* MTK: Add by Jinlong */
extern INT32 x_httpc_fm_read_break(HANDLE_T h_file);
#endif

extern INT32 x_httpc_fm_read_async(HANDLE_T        h_file,
                                   VOID            *pv_data,
                                   UINT32          ui4_count,
                                   UINT8           ui1_pri,
                                   x_fm_async_fct  pf_nfy_fct,
                                   VOID            *pv_tag,
                                   HANDLE_T        *ph_req);

extern INT32 x_httpc_fm_abort_async(HANDLE_T h_file, HANDLE_T h_req);

extern INT32 x_httpc_fm_feof(HANDLE_T h_file, BOOL* pb_eof);

extern INT32 x_httpc_fm_lseek(HANDLE_T h_file, 
                              INT64 i8_offset, 
                              UINT8 ui1_whence, 
                              UINT64 *pui8_cur_pos);

extern INT32 x_httpc_fm_get_info_by_handle(HANDLE_T h_file, FM_FILE_INFO_T *pt_info);

extern INT32 x_httpc_fm_get_rsp_hdr(HANDLE_T h_file, CHAR *pc_hdr, UINT32 *pui4_rsp_hdr_len);

extern INT32 x_httpc_fm_set_progress_callback(HANDLE_T h_file, 
                                              x_httpc_fm_progress_fct pf_progrs_cb, VOID* pv_tag);
/*dont free the return value*/
extern CHAR* x_httpc_fm_get_content_type(HANDLE_T h_file);

extern INT32 x_httpc_fm_open_proxy_agent(const CHAR *ps_url, 
                            const CHAR *ps_proxy, 
                            const CHAR *ps_agent, 
                            UINT32 ui4_buffer_size, 
                            HANDLE_T *ph_handle);

extern INT32 x_httpc_fm_open_proxy_agent_timeout(const CHAR *ps_url, 
                            const CHAR *ps_proxy, 
                            const CHAR *ps_agent, 
                            UINT32 ui4_time_out,
                            UINT32 ui4_buffer_size, 
                            HANDLE_T *ph_handle);

extern INT32 x_httpc_fm_open_proxy_agent_timeout_cookie(const CHAR *ps_url, 
                            const CHAR *ps_proxy, 
                            const CHAR *ps_agent, 
                            UINT32 ui4_time_out,
                            UINT32 ui4_buffer_size, 
                            HANDLE_T *ph_handle,
                            const CHAR* ps_cookie);

extern INT32 x_httpc_fm_get_speed_download(HANDLE_T h_file, 
												 DOUBLE* d_speed);

#endif /*_X_HTTPC_API_H_*/

