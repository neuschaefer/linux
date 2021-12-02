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
/*-----------------------------------------------------------------------------
 * $RCSfile: x_msvctx.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/9 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 907020e011d0dbd7ca156869d249b742 $ Asa
 *
 * Description:
 *         This file exports all media service context API and constant for
 *         other module using.
 *---------------------------------------------------------------------------*/

#ifndef _X_MSVCTX_H_
#define _X_MSVCTX_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "x_common.h"
#include "u_handle.h"
#include "u_msvctx.h"
#include "u_mfmtrecg.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/* This API opens a media service context. All media playback can work after open an msvctx, and
MSVCTX handle can be open more than once. This function call is synchronous call. */
extern INT32 x_msvctx_open (
                        x_msvctx_nfy_fct            pf_nfy,    /* (I) specify the function point of notification function. */
                        VOID*                       pv_tag,    /* (I) specify the tag which will be send back to AP when MSVCTX notification. */
                        HANDLE_T*                   ph_msvctx);/* (O) The parameter will retrieve a MSVCTX handle after open MSVCTX. */

/* This API closes a media service context or an element handle. This function call is synchronous call. */
extern INT32 x_msvctx_close (
                        HANDLE_T                    h_handle); /* (I) specify the MSVCTX or element handle which will be closed. */

/* This API opens a memory buffer as an element. Application can use this element handle to get information. This function call is synchronous call. */
extern INT32 x_msvctx_elm_fopen (
                        HANDLE_T                    h_msvctx,       /* (I) specify the MSVCTX handle. */
                        CHAR*                       ps_path_name,   /* (I) specify the path string of file which will be queue in MSVCTX. */
                        BOOL                        b_auto_indexing,/* (I) specify if start asynchronous indexing automatically or not. If this parameter is true, and then MSVCTX send a message to index builder to start indexing. */
                        MEDIA_FORMAT_T*             pt_format,      /* (I/O) If the application specify the format (except MEDIA_TYPE_UNKNOWN), MSVCTX will use this information to select the right handler to deal with the media file./If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), MSVCTX will use format recognizer to look it up; then return it to AP. */
                        HANDLE_T*                   ph_q_elm);      /* (O) retrieve a handle of queue element to indicate the file in MSVCTX. */

/* This API opens a memory buffer as an element. Application can use this element handle to get information. This function call is synchronous call. */
extern INT32 x_msvctx_elm_mopen (
                        HANDLE_T                   h_msvctx,        /* (I) specify the MSVCTX handle. */
                        VOID*                      pv_mem_data,     /* (I) specify the memory address of media data buffer. */
                        SIZE_T                     z_mem_leng,      /* (I) specify the memory length of media data buffer */
                        BOOL                       b_auto_indexing, /* (I) specify if start asynchronous indexing automatically or not. If this parameter is true, and then MSVCTX send a message to index builder to start indexing. */
                        MEDIA_FORMAT_T*            pt_format,       /* (I/O) If the application specify the format (except MEDIA_TYPE_UNKNOWN), MSVCTX will use this information to select the right handler to deal with the media file./If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), MSVCTX will use format recognizer to look it up; then return it to AP. */
                        HANDLE_T*                  ph_q_elm);       /* (O) retrieve a handle of queue element to indicate the file in MSVCTX. */

/* This API queues an element handle for playback into MSVCTX. Because unqueue will not close the element handle, AP can reuse the handle and add into MSVCTX queue. This function call is synchronous call. */
extern INT32 x_msvctx_queue (
                        HANDLE_T                   h_msvctx,        /* (I) specify the MSVCTX handle which will queue the playback  */
                        HANDLE_T                   h_q_elm);        /* (I) specify a handle of element which will be queued in MSVCTX. */

/* This API removes a queue element form MSVCTX queue. After playback, the element will be removed
from queue automatically. Unqueue an element will not close the element handle. If unqueue a playing element,
it will call x_msvctx_next_svc() and then unqueue current element. This function call is synchronous call. */
extern INT32 x_msvctx_unqueue (
                        HANDLE_T                   h_msvctx,        /* (I) specify the MSVCTX handle which will unqueue the playback . */
                        UINT32                     ui4_idx);        /* (I) specify the index of unqueue element. */

/* This API clean up the MSVCTX msg queue*/
extern INT32 x_msvctx_empty_queue (
                        HANDLE_T                   h_msvctx);        /* (I) specify the MSVCTX handle */

/* This API gets the element by a index form MSVCTX queue. This function call is synchronous call. */
extern INT32 x_msvctx_get_queue_elm (
                        HANDLE_T                   h_msvctx,        /* (I) specify the MSVCTX handle which will queue the playback . */
                        UINT32                     ui4_idx,         /* (I) specify the index of the element which want to be retrieved. */
                        HANDLE_T*                  ph_q_elm);       /* (O) retrieve a handle of queue element to indicate the memory buffer  in MSVCTX */

/* This API plays the service which contain in the MSVCTX. There are two elements in the queue; when the first one reach end of file,
it will pay next element automatically, and remove the first one from queue. Unqueue an element will not close the element handle.
This function call is asynchronous call.*/
extern INT32 x_msvctx_play_svc (
                        HANDLE_T                  h_msvctx,        /* (I) specify the MSVCTX handle for playback. */
                        MSVCTX_PLAY_SPEED_T       e_speed);        /* (I) specify the speed of playback. */

/* This API stops the service which contain in the MSVCTX, and rewinds to the beginning of the playback. It will not delete the first element in the queue.
It only work when MSVCTX in play and pause mode, and return MSVCTX_OK when MSVCTX already in stop mode.
This function call is asynchronous call. */
extern INT32 x_msvctx_stop_svc (
                        HANDLE_T                  h_msvctx);      /* (I) specify the MSVCTX handle for playback.*/

/* This API pause the service which contains in the MSVCTX, and keep the position of the playback.
It only work when MSVCTX in play mode. This function call is asynchronous call. */
extern INT32 x_msvctx_pause_svc (
                        HANDLE_T                  h_msvctx);      /* (I) specify the MSVCTX handle for playback.*/

/*This API play the next service which contain in the MSVCTX, and it will also remove the previous element in the MSVCTX queue.
For example, call x_msvctx_next_svc() in play/pause mode, it will be in play mode to play the next. call x_msvctx_next _svc() in stop mode, it will be in stop mode.
Unqueue an element will not close the element handle.This function call is asynchronous call.*/
extern INT32 x_msvctx_next_svc (
                        HANDLE_T                  h_msvctx);       /* (I) specify the MSVCTX handle for playback.*/

/*This API play the next service which contain in the MSVCTX, and it will also remove the previous element in the MSVCTX queue.
For example, call x_msvctx_next_svc_ex() in play/pause mode, it will be in play mode to play the next. call x_msvctx_next _svc() in stop mode, it will be in stop mode.
Unqueue an element will not close the element handle.This function call is asynchronous call.*/
extern INT32 x_msvctx_next_svc_ex (
                        HANDLE_T                  h_msvctx,       /* (I) specify the MSVCTX handle for playback.*/
                        HANDLE_T                  h_q_elm);       /* (I) specify a handle of element which will be queued in MSVCTX. */

/* This API starts indexing of the media (h_q_elm). It is a aync API. When queue a memory or a file x_msvctx_queue_*(),
it can specify auto indexing or not. If not, this API provides a starting of indexing manually */
extern INT32 x_msvctx_start_indexing (
                        HANDLE_T                  h_msvctx,       /* (I) specify the MSVCTX handle which will queue the playback .*/
                        HANDLE_T                  h_q_elm);       /* (I) specify the handle of queue element which will start indexing.*/

/* This API stop indexing of the media (h_q_elm). It is a aync API.*/
extern INT32 x_msvctx_stop_indexing (
                        HANDLE_T                  h_msvctx,       /* (I) specify the MSVCTX handle which will queue the playback .*/
                        HANDLE_T                  h_q_elm);       /* (I) specify the handle of queue element which will start indexing.*/

/*This API retrieves the position of MSVCTX current media playback. This function call is synchronous call.*/
extern INT32 x_msvctx_get_position (
                        HANDLE_T                  h_msvctx,          /* (I) specify the MSVCTX handle which will queue the playback .*/
                        UINT32*                   pui4_time_offset); /* (O) retrieve the time position in sec of the current media playback.*/

/* This API set the position of MSVCTX current media playback. This function call is asynchronous call.*/
extern INT32 x_msvctx_set_position (
                        HANDLE_T                  h_msvctx,         /* (I) specify the MSVCTX handle which will queue the playback. */
                        UINT32                    ui4_time_offset); /* (I) set the time position in sec of the current media playback. */

/* This API retrieves the duration of the media. This function call is synchronous call. */
extern INT32 x_msvctx_get_duration (
                        HANDLE_T                  h_msvctx,            /* (I) specify the MSVCTX handle which will queue the memory playback. */
                        HANDLE_T                  h_q_elm,             /* (I) specify the element handle which want to retrieve the duration. */
                        UINT32*                   pui4_time_duration); /* (O) retrieve the time duration in sec of the current media playback. */

/* This API retrieves the information of media info handle. It will return MINFO_NOT_READY, before parse media complete.
This function call is synchronous call. */
extern INT32 x_msvctx_get_info (
                        HANDLE_T                 h_msvctx,      /* (I) specify the msvctx handle which contain the element. */
                        HANDLE_T                 h_q_elm,       /* (I) specify the element handle which will be retrieved information. */
                        MSVCTX_INFO_TYPE_T       e_info_type,   /* (I) specify the information type of the media */
                        VOID*                    pv_buf,        /* (I/O) specify the memory buffer which will contain the media information./specify the memory buffer which will contain the media information */
                        SIZE_T*                  pz_buf_leng);  /* (I/O) specify the length of the buffer./If it is smaller than required, it will return MSVCTX_INSUFFICENT_MEM, and pz_buf_leng will contain required length. */

#ifdef __cplusplus
}
#endif

#endif /* _X_MSVCTX_H_ */
