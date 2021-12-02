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
 * $RCSfile: sbtl_state_handler.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 65e24ec94ef31ad2a397ad6a1b5a8b73 $
 *
 * Description: 
 *         This header file contains Stream Manager state machine specific
 *         definitions.
 *---------------------------------------------------------------------------*/

#ifndef _SBTL_STATE_HANDLER_H_
#define _SBTL_STATE_HANDLER_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "mutil/sbtl_engine/u_mm_sbtl_engine.h"
#else /*__KERNEL__*/
#include "x_common.h"
#include "x_mm_common.h"
#include "u_mm_sbtl_engine.h"
#endif /*__KERNEL__*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define SBTL_MAX_NUM_PENDING_EVN  ((UINT32) 8)
#define SBTL_STATE_IGNORE         ((UINT32) SBTL_COND_IGNORE)

typedef INT32 (*sbtl_event_do_fct)(     
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

typedef INT32 (*sbtl_state_do_fct)(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

typedef INT32 (*sbtl_state_do_exception_fct)(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

typedef struct _SBTL_STATE_COND_T
{
    UINT32              ui4_curr_state;
    UINT32              ui4_event;
    UINT32              ui4_next_state;
    UINT32              ui4_pending_event;
    sbtl_event_do_fct   pf_event_do;
} SBTL_STATE_COND_T;

#define SBTL_STATE_COND_NULL       { 0, 0, 0, 0, NULL }

typedef struct _SBTL_STATE_DESCR_T
{
    UINT32                          ui4_state;
    BOOL                            b_prcss_pending;
    BOOL                            b_allow_pending;
    sbtl_state_do_fct               pf_state_do;
    sbtl_state_do_exception_fct     pf_state_do_exception;
} SBTL_STATE_DESCR_T;

#define SBTL_STATE_DESCR_NULL      { 0, FALSE, FALSE, NULL, NULL }

typedef struct _SBTL_STATE_T
{
    SBTL_STATE_COND_T*      pt_cond_lst;
    SBTL_STATE_DESCR_T*     pt_state_lst;
    UINT32*                 pui4_allowed_pending_events;
    UINT32                  ui4_curr_state;
    UINT32                  aui4_pending_event[SBTL_MAX_NUM_PENDING_EVN];
    VOID*                   apv_pending_event_tag1[SBTL_MAX_NUM_PENDING_EVN];
    VOID*                   apv_pending_event_tag2[SBTL_MAX_NUM_PENDING_EVN];
    sbtl_state_notify_fct   pf_state_nfy;
    VOID*                   pv_state_nfy_tag;
} SBTL_STATE_T;

typedef SBTL_STATE_T*   P_SBTL_STATE_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

extern INT32 sbtl_state_init( 
    const SBTL_STATE_COND_T*    pt_state_cond_lst,
    const SBTL_STATE_DESCR_T*   pt_state_lst,
    const UINT32*               pui4_allowed_pending_events,
    sbtl_state_notify_fct       pf_state_nfy,
    VOID*                       pv_state_nfy_tag,
    P_SBTL_STATE_T              pt_state );

extern INT32 sbtl_state_on_event( 
    P_SBTL_STATE_T              pt_state,
    UINT32                      ui4_event,
    VOID*                       pv_tag1,
    VOID*                       pv_tag2,
    UINT32*                     pui4_next_state );

extern CHAR* sbtl_state_event_to_str( UINT32 ui4_event );

#endif /* _SBTL_STATE_HANDLER_H_ */
