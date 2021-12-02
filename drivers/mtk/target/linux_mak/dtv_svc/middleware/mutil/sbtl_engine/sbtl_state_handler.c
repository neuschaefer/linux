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
 * $RCSfile: sm_state.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/10 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: d908c026a37c5be69fa4ecb1520bc593 $
 *
 * Description:
 *         This header file contains Stream Manager state machine specific
 *         definitions.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "os/inc/x_os.h"
#include "handle/x_handle.h"
#include "mutil/sbtl_engine/sbtl_state_handler.h"

#else /*__KERNEL__*/
#include "x_os.h"
#include "x_handle.h"
#include "sbtl_state_handler.h"

#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static SBTL_STATE_COND_T   t_cond_null = SBTL_STATE_COND_NULL;
static SBTL_STATE_DESCR_T  t_state_descr_null = SBTL_STATE_DESCR_NULL;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
-----------------------------------------------------------------------------*/

static SBTL_STATE_COND_T* _sbtl_state_get_matched_cond(
    P_SBTL_STATE_T              pt_state,
    UINT32                      ui4_event )
{
    SBTL_STATE_COND_T*  pt_cond_iter = pt_state->pt_cond_lst;

    while( x_memcmp( pt_cond_iter, &t_cond_null, sizeof(SBTL_STATE_COND_T) ) != 0 )
    {
        if ( ( pt_cond_iter->ui4_curr_state == pt_state->ui4_curr_state ||
               pt_cond_iter->ui4_curr_state == SBTL_STATE_IGNORE ) &&
             pt_cond_iter->ui4_event == ui4_event )
        {
            /* matched */
            return pt_cond_iter;
        }
        ++pt_cond_iter;
    }

    return NULL;
}

static SBTL_STATE_DESCR_T* _sbtl_state_get_descr(
    P_SBTL_STATE_T                pt_state,
    UINT32                      ui4_state )
{
    SBTL_STATE_DESCR_T* pt_state_iter = pt_state->pt_state_lst;

    while( x_memcmp( pt_state_iter, &t_state_descr_null, sizeof(SBTL_STATE_DESCR_T) ) != 0 )
    {
        if ( pt_state_iter->ui4_state == ui4_state )
        {
            /* matched */
            return pt_state_iter;
        }
        ++pt_state_iter;
    }

    return NULL;
}

static BOOL _sbtl_state_is_allowed_pending_event(
    P_SBTL_STATE_T                pt_state,
    UINT32                      ui4_event )
{
    INT32      i4_i = 0;

    while( pt_state->pui4_allowed_pending_events[i4_i] != EVN_MM_SBTL_IGNORE )
    {
        if ( pt_state->pui4_allowed_pending_events[i4_i] == ui4_event )
        {
            return TRUE;
        }

        ++i4_i;
    }

    return FALSE;
}

static INT32 _sbtl_state_add_pending_event(
    P_SBTL_STATE_T                pt_state,
    UINT32                      ui4_event,
    VOID*                       pv_tag1,
    VOID*                       pv_tag2 )
{
    UINT32 ui4_i = 0;

    for( ui4_i = 0; ui4_i < SBTL_MAX_NUM_PENDING_EVN; ++ui4_i )
    {
        if ( pt_state->aui4_pending_event[ui4_i] == EVN_MM_SBTL_IGNORE )
        {
            pt_state->aui4_pending_event[ui4_i] = ui4_event;
            pt_state->apv_pending_event_tag1[ui4_i] = pv_tag1;
            pt_state->apv_pending_event_tag2[ui4_i] = pv_tag2;
            return MMSBTLR_PENDING;
        }
    }

    return MMSBTLR_NOT_ALLOW;
}

static INT32 _sbtl_state_remove_pending_event(
    P_SBTL_STATE_T                pt_state,
    UINT32*                     pui4_event,
    VOID**                      ppv_tag1,
    VOID**                      ppv_tag2 )
{
    if ( pt_state->aui4_pending_event[0] != EVN_MM_SBTL_IGNORE )
    {
        *pui4_event = pt_state->aui4_pending_event[0];
        *ppv_tag1 = pt_state->apv_pending_event_tag1[0];
        *ppv_tag2 = pt_state->apv_pending_event_tag2[0];

        x_memmove( &pt_state->aui4_pending_event[0],
                   &pt_state->aui4_pending_event[1],
                   sizeof(UINT32) * (SBTL_MAX_NUM_PENDING_EVN - 1) );
        x_memmove( &pt_state->apv_pending_event_tag1[0],
                   &pt_state->apv_pending_event_tag1[1],
                   sizeof(VOID*) * (SBTL_MAX_NUM_PENDING_EVN - 1) );
        x_memmove( &pt_state->apv_pending_event_tag2[0],
                   &pt_state->apv_pending_event_tag2[1],
                   sizeof(VOID*) * (SBTL_MAX_NUM_PENDING_EVN - 1) );

        pt_state->aui4_pending_event[SBTL_MAX_NUM_PENDING_EVN-1] = EVN_MM_SBTL_IGNORE;
        pt_state->apv_pending_event_tag1[SBTL_MAX_NUM_PENDING_EVN-1] = NULL;
        pt_state->apv_pending_event_tag2[SBTL_MAX_NUM_PENDING_EVN-1] = NULL;

        return MMSBTLR_OK;
    }

    *pui4_event = EVN_MM_SBTL_IGNORE;
    *ppv_tag1 = NULL;
    *ppv_tag2 = NULL;

    return MMSBTLR_NOT_ALLOW;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
INT32 sbtl_state_init(
    const SBTL_STATE_COND_T*        pt_state_cond_lst,
    const SBTL_STATE_DESCR_T*       pt_state_lst,
    const UINT32*                   pui4_allowed_pending_events,
    sbtl_state_notify_fct           pf_state_nfy,
    VOID*                           pv_state_nfy_tag,
    P_SBTL_STATE_T                  pt_state )
{
    UINT32   ui4_i;

    if ( !pt_state_cond_lst           ||
         !pt_state_lst                ||
         !pui4_allowed_pending_events ||
         !pt_state )
    {
        return MMSBTLR_INV_ARG;
    }

    pt_state->pt_cond_lst = (SBTL_STATE_COND_T*)pt_state_cond_lst;
    pt_state->pt_state_lst = (SBTL_STATE_DESCR_T*)pt_state_lst;
    pt_state->pui4_allowed_pending_events = (UINT32*)pui4_allowed_pending_events;
    pt_state->ui4_curr_state = SBTL_STATE_IGNORE;

    for( ui4_i = 0; ui4_i < SBTL_MAX_NUM_PENDING_EVN; ++ui4_i )
    {
        pt_state->aui4_pending_event[ui4_i] = EVN_MM_SBTL_IGNORE;
        pt_state->apv_pending_event_tag1[ui4_i] = NULL;
        pt_state->apv_pending_event_tag2[ui4_i] = NULL;
    }
    pt_state->pf_state_nfy = pf_state_nfy;
    pt_state->pv_state_nfy_tag = pv_state_nfy_tag;

    return MMSBTLR_OK;
}

INT32 sbtl_state_on_event(
    P_SBTL_STATE_T          pt_state,
    UINT32                  ui4_event,
    VOID*                   pv_tag1,
    VOID*                   pv_tag2,
    UINT32*                 pui4_next_state )
{
    INT32              i4_ret = MMSBTLR_OK, i4_ret_2 = MMSBTLR_INV_ARG;
    UINT32             ui4_previous_state = 0;
    SBTL_STATE_COND_T*   pt_cond = NULL;
    SBTL_STATE_DESCR_T*  pt_state_descr = NULL;

    if ( !pt_state || ui4_event == EVN_MM_SBTL_IGNORE )
    {
        return MMSBTLR_INV_ARG;
    }

    pt_state_descr = _sbtl_state_get_descr( pt_state, pt_state->ui4_curr_state );
    if ( !pt_state_descr )
    {
        return MMSBTLR_CAN_NOT_OP_IN_THIS_STATE;
    }


    pt_cond = _sbtl_state_get_matched_cond( pt_state, ui4_event );
    if ( !pt_cond )
    {
        if ( pt_state_descr->pf_state_do_exception )
        {
            i4_ret = pt_state_descr->pf_state_do_exception(
                ui4_event,
                pt_state->ui4_curr_state,
                pv_tag1,
                pv_tag2 );

            return i4_ret;
        }

        return MMSBTLR_CAN_NOT_OP_IN_THIS_STATE;
    }

    if ( pt_cond->pf_event_do )
    {
        pt_cond->pf_event_do( pt_state->ui4_curr_state,
                              ui4_event,
                              pt_cond->ui4_next_state,
                              pv_tag1,
                              pv_tag2 );
    }

    if ( pt_state->ui4_curr_state != pt_cond->ui4_next_state &&
         pt_cond->ui4_next_state != SBTL_STATE_IGNORE )
    {
        ui4_previous_state = pt_state->ui4_curr_state;
        pt_state->ui4_curr_state = pt_cond->ui4_next_state;
        if ( pui4_next_state )
        {
            *pui4_next_state = pt_cond->ui4_next_state;
        }

        /* Get new state's descriptor */
        pt_state_descr = _sbtl_state_get_descr( pt_state, pt_state->ui4_curr_state );
        if ( !pt_state_descr )
        {
            return MMSBTLR_CAN_NOT_OP_IN_THIS_STATE;
        }

        if ( pt_state->pf_state_nfy && (pt_state->ui4_curr_state != SBTL_COND_CLOSED))
        {
            pt_state->pf_state_nfy( pt_state->pv_state_nfy_tag,
                                    pt_state->ui4_curr_state,
                                    ui4_event,
                                    0 );
        }

        if ( pt_state_descr->pf_state_do )
        {
            pt_state_descr->pf_state_do( ui4_previous_state,
                                         ui4_event,
                                         pt_state->ui4_curr_state,
                                         pv_tag1,
                                         pv_tag2 );
        }

        if ( pt_state->pf_state_nfy && (pt_state->ui4_curr_state == SBTL_COND_CLOSED))
        {
            pt_state->pf_state_nfy( pt_state->pv_state_nfy_tag,
                                    pt_state->ui4_curr_state,
                                    ui4_event,
                                    0);
        }        
    }

    if ( pt_cond->ui4_pending_event != EVN_MM_SBTL_IGNORE )
    {
        if ( pt_state_descr->b_allow_pending &&
             _sbtl_state_is_allowed_pending_event( pt_state, ui4_event ) )
        {
            return _sbtl_state_add_pending_event(
                        pt_state,
                        pt_cond->ui4_pending_event,
                        pv_tag1,
                        pv_tag2 );
        }
        else
        {
            return MMSBTLR_CAN_NOT_OP_IN_THIS_STATE;
        }
    }

    while ( pt_state->aui4_pending_event[0] != EVN_MM_SBTL_IGNORE &&
            pt_state_descr->b_prcss_pending &&
            pt_cond->ui4_next_state != SBTL_STATE_IGNORE &&
            i4_ret_2 < MMSBTLR_OK )
    {
        UINT32 ui4_pending_event;
        VOID*  pv_pending_event_tag1;
        VOID*  pv_pending_event_tag2;

        i4_ret = _sbtl_state_remove_pending_event(
            pt_state,
            &ui4_pending_event,
            &pv_pending_event_tag1,
            &pv_pending_event_tag2 );
        if ( i4_ret != MMSBTLR_OK )
        {
            break;
        }

        i4_ret_2 = sbtl_state_on_event( pt_state,
                                        ui4_pending_event,
                                        pv_pending_event_tag1,
                                        pv_pending_event_tag2,
                                        pui4_next_state );
        i4_ret = i4_ret_2;
    }

    return i4_ret;
}
