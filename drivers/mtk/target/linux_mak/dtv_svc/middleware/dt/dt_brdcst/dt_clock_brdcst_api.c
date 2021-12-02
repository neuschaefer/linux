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
 * $RCSfile: dt_brdcst_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains the APIs for processing broadcast tables and
 *         synchronization with broadcast time source such ATSC's
 *         System Time Table (STT), DVB's TOT (Time Offset Table) and TDT.
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "os/inc/x_os.h"
#include "tbl_mngr/tm.h"
#include "tbl_mngr/tm_hndlr.h"
#include "tbl_mngr/atsc_si_eng/x_atsc_si_eng.h"
#include "tbl_mngr/dvb_si_eng/x_dvb_si_eng.h"

#include "res_mngr/x_rm.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_tuner.h"

#include "dbg/x_dbg.h"
#include "dt/_dt.h"
#include "dt/dt_dbg.h"
#include "dt/dt_brdcst/dt_brdcst_api.h"

#include "dt/dt_clock.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------
                     data declaraions
 ---------------------------------------------------------------------*/
static const DT_BRDCST_ATSC_API_T*   pt_atsc_brdcst_api = NULL;

static const DT_BRDCST_DVB_API_T*    pt_dvb_brdcst_api = NULL;

/*---------------------------------------------------------------------
                    functions implementations
 ----------------------------------------------------------------------*/

/****************************************************************
*  Name dt_tb_src_set_sel_order
*
*  Desc: For DVB, the system time can be received via TOT or TDT. To
*        avoid possible conflict between the current system time
*        in TDT and TOT, the user can specified precedent order to
*        tell the DT library to ignore one of the table.
*
*        The possible order are specified by the source specification.
*
*            DT_SYNC_WITH_DVB_TDT:  only accept system time from TDT
*
*            DT_SYNC_WITH_DVB_TOT:  only accept system time from TOT
*
*            DT_SYNC_WITH_DVB_TDT_OR_TOT:  initially accept TDT or TOT, but
*                                          once TDT is received, then no
*                                          TOT is accepted.
*
*            DT_SYNC_WITH_DVB_TOT_OR_TDT:  initially accept TDT or TOT, but
*                                          once TOT is received, then no
*                                          TDT is accepted. 
*  Input:
*     t_src_type:   source type 
*
*  Return:
*     DTR_OK
*
******************************************************************/

#define    DT_SRC_ACCEPT_ANY          0
#define    DT_SRC_TOT_ONLY            1
#define    DT_SRC_TDT_ONLY            2
#define    DT_SRC_TOT_OR_TDT          3
#define    DT_SRC_TDT_OR_TOT          4

static INT32 dt_tb_src_set_sel_order
(
    DT_CLOCK_CTRL_T*            pt_co,
    DT_SYNC_SRC_TYPE_T          t_src_type
)
{
    switch (t_src_type)
    {
        case DT_SYNC_WITH_DVB_TDT:
        {
            pt_co->pt_clock->ui4_internal_src_state =  DT_SRC_TDT_ONLY ;
        }
        break;

        case DT_SYNC_WITH_DVB_TOT:
        {
            pt_co->pt_clock->ui4_internal_src_state =  DT_SRC_TOT_ONLY ;
        }
        break;

        case DT_SYNC_WITH_DVB_TOT_OR_TDT:
        {
            pt_co->pt_clock->ui4_internal_src_state =  DT_SRC_TOT_OR_TDT ;
        }
        break;

        case DT_SYNC_WITH_DVB_TDT_OR_TOT:
        {
            pt_co->pt_clock->ui4_internal_src_state =  DT_SRC_TDT_OR_TOT ;
        }
        break;
        
        default:
        {
            pt_co->pt_clock->ui4_internal_src_state = DT_SRC_ACCEPT_ANY  ;
        }
        break;
    }

    /* save the sync source specified by the application. */
    pt_co->pt_clock->t_sync_src_type = t_src_type;
    
    return DTR_OK;
}

/****************************************************************
*  Name dt_clock_accept_systm_from_src
*
*  Desc: Evaluate the system time received from the specified source,
*        and determine if the system time should be accepted (e.g.,
*        use to set the DT library current time.)
*
*        Depending on the current internal state, accepting the system
*        time might cause state transition.  For example:
*
*        1). If the current state is DT_SRC_TOT_OR_TDT, accepting
*            TOT time will change the state to accept TOT only (subsequent
*            TDT received will be rejected). But accepting TDT time will
*            not change the state.
*
*        2). If the current state is DT_SRC_TDT_OR_TOT, accepting
*            TDT time will change the state to accept TDT only (subsequent
*            TOT received will be rejected). But accepting TOT time will
*            not change the state.
*        
*  Input:
*     t_src_type:   Table ID of the system time/DST info source. 
*
*  Return:
*     TRUE:  Accept system time from this source type.
*     FALSE: Don't accept system time from this source type.
*
******************************************************************/
static BOOL dt_clock_accept_systm_from_src
(
    DT_CLOCK_CTRL_T* pt_co,
    UINT8 t_src_type
)
{
    UINT32   ui4_state ;
    BOOL     b_rc;

    ui4_state = pt_co->pt_clock->ui4_internal_src_state ;
    
    switch (ui4_state)
    {
        case DT_SRC_TDT_ONLY:
        {
            b_rc = ( t_src_type == DVB_TDT_TBL_ID );
        }
        break;

        case DT_SRC_TOT_ONLY :
        {
            b_rc = ( t_src_type == DVB_TOT_TBL_ID ) ;
        }
        break;

        case DT_SRC_TOT_OR_TDT :
        {
            if ( t_src_type == DVB_TOT_TBL_ID )
            {
                pt_co->pt_clock->ui4_internal_src_state =  DT_SRC_TOT_ONLY ;
                b_rc = TRUE;
            }
            else 
            {
                b_rc = ( t_src_type == DVB_TDT_TBL_ID ) ;
            }
        }
        break;

        case DT_SRC_TDT_OR_TOT:
        {
            if ( t_src_type == DVB_TDT_TBL_ID )
            {
                pt_co->pt_clock->ui4_internal_src_state =  DT_SRC_TDT_ONLY ;
                b_rc = TRUE;
            }
            else 
            {
                b_rc = ( t_src_type == DVB_TOT_TBL_ID ) ;
            }
        }
        break;

        default:
        {
            b_rc = TRUE ;
        }
        break;
    }

    /* Check if the current time data is from VBI teletext. */
    if ( t_src_type == VBI_TELETEXT_TBL_ID )
    {
        if ( pt_co->pt_clock->t_analog_sync_src_type == DT_SYNC_SRC_NONE )
        {
            b_rc = FALSE;
        }
        else
        {
            b_rc = TRUE;
        }
    }
    
    return b_rc;
}

/*-----------------------------------------------------------------------
 * Name: dt_clock_tm_root_nfy_fct
 *
 * Description:  This function is called by the table manager
 *               when the source for the Table Root is changed.
 *
 * Inputs:
 *   h_obj       handle to the table manager.
 *
 *   e_obj_type  contains the object type.
 *
 *   e_nfy_cond  contains the state (condition) of the table object.
 *               The states can be: TM_COND_AVAILABLE,
 *               TM_COND_UPDATE, TM_COND_TRANSITION.
 *
 *   pv_nfy_tag  contains the notification tag, in this case it would be a
 *               handle to this clock.
 *
 *   ui4_data    contains the address to the STT object.
 *
 * Outputs: -
 *   None.
 *
 * Returns:
 *   NFY_RET_PROCESSED:  table section has been processed.
 *
 -------------------------------------------------------------------------*/
static NFY_RET_T dt_clock_tm_root_nfy_fct
(
    HANDLE_T         h_obj,
    HANDLE_TYPE_T    e_obj_type,
    TM_COND_T        e_nfy_cond,
    VOID*            pv_nfy_tag,
    UINT32           ui4_data
)
{
    NFY_RET_T        t_return;
    INT32            i4_rc;
    HANDLE_T         h_clock;

    CRIT_STATE_T     t_state;
    HANDLE_TYPE_T    e_type;
    DT_CLOCK_CTRL_T* pt_co;

    h_clock = (HANDLE_T) pv_nfy_tag ;

    /* set the return code to data processed. */
    t_return = NFY_RET_PROCESSED;

    /* get the control structure for this clock. */
    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_clock, &e_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (e_type == DTT_CLOCK_T)                            &&
         (pt_co->pt_clock != NULL )
        )     
    {
        x_crit_end(t_state);
        
        switch (e_nfy_cond)
        {
            case TM_COND_SRC_DISC:
            {
                /*
                  Set the new condition.
                */

                DBG_INFO(("{DT} TM ROOT Notification => TM_COND_SRC_DISC: clock [%s] set to DT_FREE_RUNNING.\n", pt_co->pt_clock->s_name));

                i4_rc = dt_clock_set_cond(pt_co, DT_FREE_RUNNING);

                /*
                  Notify the register client(s) of change in
                  the system clock running state.
                */
                if ( i4_rc == DTR_WILL_NOTIFY )
                {
                    DBG_INFO(("{DT} Notify clients of clock [%s] with DT_FREE_RUNNING cond.\n",pt_co->pt_clock->s_name));
                    
                    dt_clock_notify(pt_co,DT_FREE_RUNNING, 0);
                }

                /*----------------------------------------------------
                  If the current system sync source is DT_SYNC_WITH_DVB_TDT_OR_TOT
                  or DT_SYNC_WITH_DVB_TOT_OR_TDT, then we need to reset it.
                  Because we could have already receive 'TDT' or 'TOT' table.
                  In such case, we will be in the 'accept TDT' or 'accept
                  TOT' only state.  We need to reset the internal state
                  to process 'TOT' or TDT' state so that the logic to
                  accpet only 'TOT' or 'TDT' is re-execute again in the
                  event of t.s change. (Pedro Tsai)
                  -------------------------------------------------------------*/
                if ( pt_co->pt_clock->t_sync_src_type == DT_SYNC_WITH_DVB_TDT_OR_TOT )
                {
                    pt_co->pt_clock->ui4_internal_src_state = DT_SRC_TDT_OR_TOT ;
                }
            
                if ( pt_co->pt_clock->t_sync_src_type == DT_SYNC_WITH_DVB_TOT_OR_TDT )
                {
                    pt_co->pt_clock->ui4_internal_src_state = DT_SRC_TOT_OR_TDT ;
                }
            }
            break;

            default:
            {

                /*
                  Ignore all other TM Root condition received.
                */
            }
            break;
        }
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
#endif
    }
    
    return t_return;
}

/*-----------------------------------------------------------------------
 * Name: dt_clock_brdcst_tm_nfy
 *
 * Description:  This function is called by the Section engine when a time table
 *               (STT, TDT, or TOT) is received.  The prototype of this API
 *               is specified by (*x_tm_nfy_fct).
 *               This function calls the respective parse function to obtain
 *               the current time and day-light-saving information. It sets
 *               the broadcast clock and/or system clock with the data received
 *               from the broadcast.
 *
 * Inputs:
 *   h_obj       handle to the table manager.
 *
 *   e_obj_type  contains the object type.
 *
 *   e_nfy_cond  contains the state (condition) of the table object.
 *               The states can be: TM_COND_AVAILABLE,
 *               TM_COND_UPDATE, TM_COND_TRANSITION, TM_COND_UPDATE,
 *               TM_COND_SECTION, TM_COND_SRC_CHANGED.
 *
 *   pv_nfy_tag  contains the notification tag. In this case it would be a
 *               handle to this clock.
 *   ui4_data    contains the address to the STT object.
 *
 * Outputs: -
 *   None.
 *
 * Returns:
 *   NFY_RET_PROCESSED:  table section has been processed.
 *
 -------------------------------------------------------------------------*/
static NFY_RET_T dt_clock_brdcst_tm_nfy
(
    HANDLE_T       h_obj,
    HANDLE_TYPE_T  e_obj_type,
    TM_COND_T      e_nfy_cond,
    VOID*          pv_nfy_tag,
    UINT32         ui4_data
)
{
    NFY_RET_T        t_return;
    const UINT8*     aui1_stt_data;
    INT32            i4_rc;
    HANDLE_T         h_clock;

    CRIT_STATE_T     t_state;
    HANDLE_TYPE_T    e_type;
    DT_CLOCK_CTRL_T* pt_co;

    h_clock = (HANDLE_T) pv_nfy_tag ;
    
    /* set the return code to data processed. */
    t_return = NFY_RET_PROCESSED;

    /* get the control structure for this clock. */
    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_clock, &e_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (e_type == DTT_CLOCK_T)                            &&
         (pt_co->pt_clock != NULL )
        )     
    {
        x_crit_end(t_state);
    }
    else
    {
        /* No access to clock object, just return. */
        x_crit_end(t_state);
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
#endif
        return t_return;
    }
    
    aui1_stt_data=(UINT8*)ui4_data;
    /*
      debug statements: Check the notification condition.
    */
    switch (e_nfy_cond)
    {
        case TM_COND_SECTION:
        {
            /*
              Receive a table section contain time info.
              Table section can be STT, TDT, or TOT.
            */
            DBG_INFO(("{DT} clock [%s] sys time table section received, table id: 0x%x\n",
                      pt_co->pt_clock->s_name, (UINT8)aui1_stt_data[0]));

            /*
               Process the STT section, check if this is In-Band or
               Out-Of-Band data.
            */
            switch ( (UINT8)aui1_stt_data[0] )
            {
                case STT_IN_BAND_TBL_ID:
                {
                    if ( pt_atsc_brdcst_api != NULL )
                    {
                        DBG_INFO(("{DT} clock [%s] call parse_ib_stt function\n",pt_co->pt_clock->s_name));
                        pt_atsc_brdcst_api->pf_parse_ib_stt(h_obj,
                                                            e_obj_type,
                                                            (VOID*) pt_co,
                                                            aui1_stt_data);
                    }
                }
                break;

                case STT_OUT_OF_BAND_TBL_ID:
                {
                    if ( pt_atsc_brdcst_api != NULL )
                    {
                        DBG_INFO(("{DT} clock [%s] call parse_oob_stt function\n",pt_co->pt_clock->s_name));
                        pt_atsc_brdcst_api->pf_parse_oob_stt(h_obj,
                                                             e_obj_type,
                                                             (VOID*) pt_co,
                                                             aui1_stt_data);
                    }
                }
                break;

                case DVB_TDT_TBL_ID:
                {
                    if ( pt_dvb_brdcst_api != NULL )
                    {
                        DBG_INFO(("{DT} clock [%s] call parse_dvb_tdt_tot function\n",pt_co->pt_clock->s_name));
                        pt_dvb_brdcst_api->pf_parse_dvb_tdt_tot(h_obj,
                                                                e_obj_type,
                                                                (VOID*) pt_co,
                                                                aui1_stt_data);
                    }
                }
                break;

                case DVB_TOT_TBL_ID:
                {
                    if ( pt_dvb_brdcst_api != NULL )
                    {
                        DBG_INFO(("{DT} clock [%s] call parse_dvb_tdt_tot function\n",pt_co->pt_clock->s_name));
                        pt_dvb_brdcst_api->pf_parse_dvb_tdt_tot(h_obj,
                                                                e_obj_type,
                                                                (VOID*) pt_co,
                                                                aui1_stt_data);
                    }
                }
                break;

                default:
                {
                    /*
                      Unknown STT section received.
                    */
                    t_return = NFY_RET_RETRY;
                }
                break;
            }
        }
        break;

        case TM_COND_UNAVAILABLE:
        {
            DBG_INFO(("{DT} clock [%s] STT Unavailable, set clock to DT_FREE_RUNNING.\n", pt_co->pt_clock->s_name));

            /*
              STT is not available, so we set the condition to free running.
            */
            i4_rc = dt_clock_set_cond(pt_co, DT_FREE_RUNNING);

            /*
              Notify the register client(s) of change in
              the system clock running state.
            */
            if ( i4_rc == DTR_WILL_NOTIFY )
            {
                DBG_INFO(("{DT} clock [%s] Notify clients with DT_FREE_RUNNING cond.\n", pt_co->pt_clock->s_name));
                dt_clock_notify(pt_co, DT_FREE_RUNNING, 0);
            }
        }
        break;

        case TM_COND_AVAILABLE:
        {
            DBG_INFO(("{DT} brdcst time available.\n"));
        }
        break;

        case TM_COND_TRANSITION:
        {
            DBG_INFO(("{DT} brdcst source in transition.\n"));
        }
        break;

        case TM_COND_UPDATE:
        {
            DBG_INFO(("{DT} brdcst Time updated.\n"));
        }
        break;

        case TM_COND_SRC_CHANGED:
        {
            DBG_INFO(("{DT} brdcst source changed.\n"));
        }
        break;

        default:
            DBG_INFO(("{DT} unknown condition.\n"));
            break;
    }

    return t_return;
}

/*-----------------------------------------------------------------------
 * Name: dt_clock_sync_with_stt
 *
 * Description:  This function set the system time library with
 *               the time table (ATSC STT, DVB TDT/TOT) data that has
 *               been received.
 * Inputs:
 *   h_obj       handle to the table manager.
 *
 *   e_obj_type  contains the object type.
 *
 *   e_nfy_cond  contains the state (condition) of the table object.
 *               The states can be: TM_COND_OBJ_AVAILABLE,
 *               TM_COND_OBJ_UPDATE, TM_COND_OBJ_TRANSITION.
 *
 *   pv_nfy_tag  contains the notification tag.
 *   pui1_data    contains the address to the STT object.
 *
 * Outputs: -
 *   None.
 *
 * Returns:
 *   DTR_WILL_NOTIFY:  After setting the system clock, the internal system
 *                     clock condition has changed, therefore we will need
 *                     to notify the client.
 *   DTR_OK:           Setting of system time is success, but no need
 *                     to notify the client of condition change.
 *
 -------------------------------------------------------------------------*/
INT32 dt_clock_sync_with_stt
(
    DT_CLOCK_CTRL_T* pt_co,
    STT_DATA_T*      pt_stt
)
{
    TIME_T           t_delta = 0;
    INT32            i4_rc  = DTR_OK;

    TIME_T           t_sys_tm;
    DT_COND_T        t_sys_cond;
    DT_COND_T        t_new_cond = DT_FREE_RUNNING;
    BOOL             b_set_sys_tm;

    /*
      Determine if we should use this time to set the system time.
      This is to handle the DVB broadcast where we can receive system
      time from TOT or TDT.
    */
    b_set_sys_tm = dt_clock_accept_systm_from_src(pt_co, pt_stt->ui1_src_type );

    if ( (b_set_sys_tm == TRUE) &&
         (dt_gross_error_check_on_input_time(pt_stt->t_sec) == TRUE)
        )
    {  
        t_delta = 0;

        DBG_INFO(("{DT} clock [%s] Sync system time with => src tbl id: 0x%x, UTC: %lld sec\n",
                  pt_co->pt_clock->s_name,pt_stt->ui1_src_type,pt_stt->t_sec));

        /* The default new system clock state. */
        t_new_cond = DT_SYNC_RUNNING ;

        /*
          Check the new current time with the current time
          to see how much delta there is between the current
          time of system clock and current time from the STT.
        */
        t_sys_tm=dt_clock_get_utc_and_cond(pt_co, NULL, &t_sys_cond);
        if ( t_sys_cond == DT_SYNC_RUNNING )
        {
            /*
              In the sync state, check if there is a discontinuity in
              value of current time. The discontinuity could be
              due to re-winding of the stream or too much different
              between the system time and time provided by the STT.
            */
            if (  DT_IS_DISCONT(t_sys_tm, pt_stt->t_sec ,DT_DELTA_SEC) )
            {
                /*
                  Set the new system clock condition to DT_SYNC_DISCONT
                  to indicate possible re-winding of stream.
                */
                t_new_cond = DT_SYNC_DISCONT;

                /*
                  compute the jump of system time.
                */
                t_delta = pt_stt->t_sec - t_sys_tm ;
            }
        }

        /*
          synchronize the broadcast clock with UTC time received.
        */
        i4_rc=dt_clock_set_utc_and_cond(pt_co,
                                        pt_stt->t_sec, 0, t_new_cond);

        /*
          Notify the client only if this is change of this clock condition
          from DT_FREE_RUNNING to DT_SYNC_RUNNING, or from DT_SYNC_RUNNING to
          DT_SYNC_DISCONT (due to positive or negative jump in the received
          STT time.)
        */
        if ( i4_rc == DTR_WILL_NOTIFY )
        {
            UINT32   ui4_val;
            
            DBG_INFO(("{DT} clock [%s] Notify clients with DT_SYNC_RUNNING cond.\n",pt_co->pt_clock->s_name));
            ui4_val = (UINT32) t_delta;
            
            dt_clock_notify(pt_co, t_new_cond, (VOID*)ui4_val);
        }
    }
    
    /*
       Note: Currently for the custom create clock, we don't send notification to
       clients for for DST or timezone offset change.,  this might
       needs to be revisited later.
    */    
    
    return i4_rc;
}





/*-----------------------------------------------------------------------
 * Name: _dt_clock_free_sync_src_hdle_unprotected
 *
 * Description:  This function frees the handle to the SI engine(s)
 *
 *               This function should be called with semaphore for
 *               proctection.
 * Inputs:
 *   None.
 *
 * Outputs:
 *   None.
 *
 * Returns:
 *   DTR_OK:      Handle to the SI engine are free.
 *
 *   DTR_FAIL     Can not allocate handle to table manager or handle to the
 *                ATSC engine.
 -------------------------------------------------------------------------*/
static INT32 _dt_clock_free_sync_src_hdle_unprotected
(
    DT_CLOCK_CTRL_T*            pt_clock_ctrl
)
{
    INT32       i4_rc;
    
    i4_rc = DTR_OK;
    
    /*
      Free the current source of the time table.
    */
    if ( pt_clock_ctrl->pt_clock->h_src_root != NULL_HANDLE )
    {   
        /*
          Delete the Table Manager Root
        */
        if (  x_tm_free(pt_clock_ctrl->pt_clock->h_src_root) == TMR_OK )
        {
            if ( x_handle_valid(pt_clock_ctrl->pt_clock->h_src_root) == FALSE )
            {
                DBG_INFO(("{DT} clock [%s] TM Root deleted.\n", pt_clock_ctrl->pt_clock->s_name));
                pt_clock_ctrl->pt_clock->h_src_root = NULL_HANDLE ;
                /*
                  If the root is delete, the child will
                  become invalid as well.
                */
                if ( x_handle_valid(pt_clock_ctrl->pt_clock->h_dvb_tdt)  == FALSE   &&
                     x_handle_valid(pt_clock_ctrl->pt_clock->h_dvb_tot)  == FALSE   &&
                     x_handle_valid(pt_clock_ctrl->pt_clock->h_atsc_stt) == FALSE
                    )
                {
                    DBG_INFO(("{DT} clock [%s] Handle(s) to engine (ATSC/DVB) deleted.\n", pt_clock_ctrl->pt_clock->s_name));
                    pt_clock_ctrl->pt_clock->h_dvb_tdt  = NULL_HANDLE ;
                    pt_clock_ctrl->pt_clock->h_dvb_tot  = NULL_HANDLE ;
                    pt_clock_ctrl->pt_clock->h_atsc_stt = NULL_HANDLE ;
                    i4_rc = DTR_OK;
                }
                else
                {
                    i4_rc = DTR_FAIL;
                }
            }
            else
            {
                i4_rc = DTR_FAIL;
            }
        }
        else
        {
            i4_rc = DTR_FAIL;
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------
 * Name: dt_clock_unload_brdcst_time_info
 *
 * Description:  This function frees the sync source handle(s) to the SI engine
 *               and set the DT condition to DT_NO_SYNC_SRC.
 *
 * Inputs:
 *   None
 *
 * Outputs:
 *   None.
 *
 * Returns:
 *   DTR_OK:      Handle to the table manager and callback function to
 *                process STT has been registered.
 *   DTR_FAIL     Can not allocate handle to table manager or handle to the
 *                ATSC engine.
 -------------------------------------------------------------------------*/
INT32 dt_clock_unload_brdcst_time_info
(
    DT_CLOCK_CTRL_T*            pt_clock_ctrl
)
{
    INT32       i4_rc;

    if ( pt_clock_ctrl == NULL )
    {
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_NULL_PTR);
    }
    
    /*
      Need to protect this API, because this API can be called from
      two different threads:

      1. From DT initialization thread, when dt_init() is called which
      then call x_dt_set_sync_src()

      2. From application thread, where it change sources of the STT
      (c_dt_set_sync_src() which calles x_dt_set_sync_src()).

      For example: to change source from Terrestrial STT to
                   Cable in-band STT.
    */
    dt_clock_grab_sema(pt_clock_ctrl->pt_clock->h_sema);

    /*
      Free the current source of the time table.
    */
    i4_rc = _dt_clock_free_sync_src_hdle_unprotected(pt_clock_ctrl);
    
    if ( i4_rc == DTR_OK )
    {
        /*
          SI handle source has been deleted, set the sync source
          condition and determine if it is necessary to notify
          client of the condition change..
        */
        
        if ( dt_clock_set_cond(pt_clock_ctrl, DT_NO_SYNC_SRC) ==
             DTR_WILL_NOTIFY )
        {
            /*
               Notify the client(s) of this clock that it is 
               now in the no sync source state.
            */
            DBG_INFO(("{DT} clock [%s] notify clients with DT_NO_SYNC_SRC cond.\n",pt_clock_ctrl->pt_clock->s_name));
            dt_clock_notify(pt_clock_ctrl, DT_NO_SYNC_SRC, 0);
        }
    }
    else
    {
        DBG_ERROR(("{DT} Error: clock [%s] can not delete sync source handle(s) to SI engine.\n",pt_clock_ctrl->pt_clock->s_name));
    }
    
    dt_clock_ungrab_sema(pt_clock_ctrl->pt_clock->h_sema);
    
    return i4_rc;
}

/*-----------------------------------------------------------------------
 * Name: dt_clock_load_brdcst_time_info
 *
 * Description:  This function creates a handle to the table manager, and
 *               request loading of system time info tables (STT, TOT,
 *               TDT) from specific SI engine. If an exist source already
 *               in used, calling this API will change the source of the
 *               time info.
 *
 *               If the 't_sync_src_type' is DT_SYNC_SRC_NONE, then
 *               this API free the existing table manager handle and
 *               handle to the SI engine, and then return.
 
 *               A callback function is registered with the specific
 *               SI engine to receive time table data.
 *
 *               SI engine can be ATSC, SCTE, or DVB engines.
 *               
 *               The callback function is called everytime when a TOT
 *               and/or TDT, or STT table section is received by the
 *                SI engine.
 *
 * Inputs:
 *
 *   t_sync_src_type:  Specifies the types of synchronization source.
 *
 *   t_tbm_src_type  Specified table manager root node source type.
 *
 *   pv_src_info     pointer to source info description. For example,
 *                   if 't_tbm_src_type' is TM_SRC_TYPE_MPEG_2_BRDCST,
 *                   then 'pv_src_info' points to a char string containing
 *                   the name of tuner which table manager used to
 *                   retrieve the STT from.
 * Outputs:
 *   None.
 *
 * Returns:
 *   DTR_OK:      Handle to the table manager and callback function to
 *                process STT has been registered.
 *   DTR_FAIL     Can not allocate handle to table manager or handle to the
 *                ATSC engine.
 -------------------------------------------------------------------------*/
INT32 dt_clock_load_brdcst_time_info
(
    DT_CLOCK_CTRL_T*            pt_clock_ctrl,
    DT_SYNC_SRC_TYPE_T          t_sync_src_type,
    DT_SRC_TYPE_DESC_T          t_src_type_desc,
    const VOID*                 pv_src_info
)
{
    INT32                       i4_rc;
    TM_COND_T                   e_cond;
    TM_SRC_TYPE_T               t_tbm_src_type;

    if (  pt_clock_ctrl == NULL )
    {
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_NULL_PTR);
    }
        
    switch (t_src_type_desc)
    {
        case DT_SRC_TYPE_MPEG_2_BRDCST:
        {
            t_tbm_src_type = TM_SRC_TYPE_MPEG_2_BRDCST;
        }
        break;

        case DT_SRC_TYPE_FORMAT_UNKNOWN:
        {
            t_tbm_src_type = TM_SRC_TYPE_MPEG_2_OOB;
        }
        break;

        case DT_SRC_TYPE_CONN_HANDLE:
        {
            t_tbm_src_type = TM_SRC_TYPE_CONN_HANDLE;
        }
        break;

        default:
        {
            x_dbg_stmt("{DT} Error: unknown table source type.\n");
            return  DTR_FAIL;
        }
    }
    
    /*
      Need to protect this API, because this API can be called from
      two different threads:

      1. From DT initialization thread, when dt_init() is called which
      then call x_dt_clock_set_sync_src()

      2. From application thread, where it change sources of the STT
      (c_dt_set_sync_src() which calles x_dt_clock_set_sync_src()).

      For example: to change source from Terrestrial STT to
                   Cable in-band STT.
    */
    dt_clock_grab_sema(pt_clock_ctrl->pt_clock->h_sema);
    
    /*
      Free the current source of the time table.
    */
    i4_rc = _dt_clock_free_sync_src_hdle_unprotected(pt_clock_ctrl);
    
    if ( i4_rc == DTR_OK )
    {
        /*
           Time source has been deleted, set the clock
           condition to free running state.
        */
        DBG_INFO(("{DT} [clock: %s] time source changed: set clock cond to DT_FREE_RUNNING.\n", pt_clock_ctrl->pt_clock->s_name));
        
        if ( dt_clock_set_cond(pt_clock_ctrl, DT_FREE_RUNNING) ==
             DTR_WILL_NOTIFY )
        {
            /*
              Notify the client(s) that this clock is
              now in the free running state.
            */
            DBG_INFO(("{DT} Notify clients of [clock: %s] with DT_FREE_RUNNING cond.\n",pt_clock_ctrl->pt_clock->s_name));
            
            dt_clock_notify(pt_clock_ctrl, DT_FREE_RUNNING, 0);
        }
    }
    else
    {
        DBG_ERROR(("{DT} Error: [clock: %s] can not delete sync source handle(s) to SI engine.\n",pt_clock_ctrl->pt_clock->s_name));
        dt_clock_ungrab_sema(pt_clock_ctrl->pt_clock->h_sema);
        return i4_rc;
    }

    i4_rc =  TMR_NOT_INIT ;
    
    if ( pt_clock_ctrl->pt_clock->h_src_root == NULL_HANDLE)
    {
        i4_rc = x_tm_open_root(pt_clock_ctrl->pt_clock->s_name, /* use name of clock as
                                                                   TM root name. */
                               t_tbm_src_type,
                               (VOID*)pv_src_info,
                               (VOID*)pt_clock_ctrl->h_clock,   /* pass clock handle
                                                                   to the notify fct. */
                               dt_clock_tm_root_nfy_fct,
                               &(pt_clock_ctrl->pt_clock->h_src_root),
                               &e_cond ) ;
    }

    if ( i4_rc == TMR_OK )
    {
        DBG_INFO(("{DT} clock [%s] TM Root created successful.\n",pt_clock_ctrl->pt_clock->s_name));                       

        /*
          Enable notification from TM Root.
        */
        if ( tm_set_src_conn_nfy_cond(pt_clock_ctrl->pt_clock->h_src_root,
                                      TRUE,
                                      &e_cond) == TMR_OK
            )
        {

            DBG_INFO(("{DT} clock [%s] TM Root notification enabled.\n",pt_clock_ctrl->pt_clock->s_name));

            switch (t_sync_src_type)
            {
                case DT_SYNC_WITH_DVB_TDT:
                {
                    /* Load only TDT table. */
                    if ( x_dvb_si_load_tdt(pt_clock_ctrl->pt_clock->h_src_root,
                                           (VOID*)pt_clock_ctrl->h_clock,   /* pass clock handle
                                                                               to the notify fct. */
                                           dt_clock_brdcst_tm_nfy,
                                           &(pt_clock_ctrl->pt_clock->h_dvb_tdt),
                                           &e_cond) == TMR_OK
                        )
                    {
                        DBG_INFO(("{DT} clock [%s] load DVB TDT table successful.\n",pt_clock_ctrl->pt_clock->s_name));
                        dt_tb_src_set_sel_order(pt_clock_ctrl, t_sync_src_type);
                        i4_rc = DTR_OK;
                    }
                    else
                    {
                        DBG_ERROR(("{DT} clock [%s] Failed to load DVB TDT table.\n",pt_clock_ctrl->pt_clock->s_name));
                        i4_rc = DTR_FAIL;
                    }
                }
                break;

                case DT_SYNC_WITH_DVB_TOT:
                {
                    /* Load only TOT table. */
                    if ( x_dvb_si_load_tot(pt_clock_ctrl->pt_clock->h_src_root,
                                           (VOID*)pt_clock_ctrl->h_clock,   /* pass clock handle
                                                                               to the notify fct. */
                                           dt_clock_brdcst_tm_nfy,
                                           &(pt_clock_ctrl->pt_clock->h_dvb_tot),
                                           &e_cond) == TMR_OK
                        )
                    {
                        DBG_INFO(("{DT} clock [%s] load DVB TOT table successful.\n",pt_clock_ctrl->pt_clock->s_name));
                        dt_tb_src_set_sel_order(pt_clock_ctrl, t_sync_src_type);
                        i4_rc = DTR_OK;
                    }
                    else
                    {
                        DBG_ERROR(("{DT} clock [%s] Failed to load DVB TOT table.\n",pt_clock_ctrl->pt_clock->s_name));
                        i4_rc = DTR_FAIL;
                    }
                }
                break;

                case DT_SYNC_WITH_DVB_TDT_OR_TOT:
                {
                    /* Load TDT table and then TOT table. */
                    if ( x_dvb_si_load_tdt(pt_clock_ctrl->pt_clock->h_src_root,
                                           (VOID*)pt_clock_ctrl->h_clock,   /* pass clock handle
                                                                               to the notify fct. */
                                           dt_clock_brdcst_tm_nfy,
                                           &(pt_clock_ctrl->pt_clock->h_dvb_tdt),
                                           &e_cond) == TMR_OK
                        )
                    {
                        DBG_INFO(("{DT} clock [%s] load DVB TDT table successful.\n",pt_clock_ctrl->pt_clock->s_name));
                        if ( x_dvb_si_load_tot(pt_clock_ctrl->pt_clock->h_src_root,
                                               (VOID*)pt_clock_ctrl->h_clock,   /* pass clock handle
                                                                                   to the notify fct. */
                                               dt_clock_brdcst_tm_nfy,
                                               &(pt_clock_ctrl->pt_clock->h_dvb_tot),
                                               &e_cond) == TMR_OK
                            )
                        {
                            DBG_INFO(("{DT} clock [%s] load DVB TOT table successful.\n",pt_clock_ctrl->pt_clock->s_name));
                            dt_tb_src_set_sel_order(pt_clock_ctrl, t_sync_src_type);
                            i4_rc = DTR_OK;
                        }
                        else
                        {
                            DBG_ERROR(("{DT} clock [%s] Failed to load DVB TOT table.\n",pt_clock_ctrl->pt_clock->s_name));
                            i4_rc = DTR_FAIL;
                        }
                    }
                    else
                    {
                        DBG_ERROR(("{DT} clock [%s] Failed to load DVB TDT table.\n",pt_clock_ctrl->pt_clock->s_name));
                        i4_rc = DTR_FAIL;
                    }
                }
                break;

                case DT_SYNC_WITH_DVB_TOT_OR_TDT:
                {
                    /* Load TOT table and then TDT table. */
                    if ( x_dvb_si_load_tot(pt_clock_ctrl->pt_clock->h_src_root,
                                           (VOID*)pt_clock_ctrl->h_clock,   /* pass clock handle
                                                                               to the notify fct. */
                                           dt_clock_brdcst_tm_nfy,
                                           &(pt_clock_ctrl->pt_clock->h_dvb_tot),
                                           &e_cond) == TMR_OK
                        )
                    {
                        DBG_INFO(("{DT} clock [%s] load DVB TOT table successful.\n",pt_clock_ctrl->pt_clock->s_name));
                        if ( x_dvb_si_load_tdt(pt_clock_ctrl->pt_clock->h_src_root,
                                               (VOID*)pt_clock_ctrl->h_clock,   /* pass clock handle
                                                                                   to the notify fct. */
                                               dt_clock_brdcst_tm_nfy,
                                               &(pt_clock_ctrl->pt_clock->h_dvb_tdt),
                                               &e_cond) == TMR_OK
                            )
                        {
                            DBG_INFO(("{DT} clock [%s] load DVB TDT table successful.\n",pt_clock_ctrl->pt_clock->s_name));
                            dt_tb_src_set_sel_order(pt_clock_ctrl, t_sync_src_type);
                            i4_rc = DTR_OK;
                        }
                        else
                        {
                            DBG_ERROR(("{DT} clock [%s] Failed to load DVB TDT table.\n",pt_clock_ctrl->pt_clock->s_name));
                            i4_rc = DTR_FAIL;
                        }
                    }
                    else
                    {
                        DBG_ERROR(("{DT} clock [%s] Failed to load DVB TOT table.\n",pt_clock_ctrl->pt_clock->s_name));
                        i4_rc = DTR_FAIL;
                    }
                }
                break;

                case DT_SYNC_WITH_SCTE_STT:
                {
                    if ( x_atsc_si_load_stt(pt_clock_ctrl->pt_clock->h_src_root,
                                            (VOID*)pt_clock_ctrl->h_clock,   /* pass clock handle
                                                                                to the notify fct. */
                                            dt_clock_brdcst_tm_nfy,
                                            &(pt_clock_ctrl->pt_clock->h_atsc_stt),
                                            &e_cond) == TMR_OK
                        )
                    {
                        DBG_INFO(("{DT} clock [%s] load SCTE STT table successful.\n",pt_clock_ctrl->pt_clock->s_name));
                        dt_tb_src_set_sel_order(pt_clock_ctrl, t_sync_src_type);
                        i4_rc = DTR_OK;
                    }
                    else
                    {
                        DBG_ERROR(("{DT} clock [%s] Failed to load SCTE STT table.\n",pt_clock_ctrl->pt_clock->s_name));
                        i4_rc = DTR_FAIL;
                    }
                }
                break;
                
                case DT_SYNC_WITH_ATSC_STT:
                {
                    if ( x_atsc_si_load_stt(pt_clock_ctrl->pt_clock->h_src_root,
                                            (VOID*)pt_clock_ctrl->h_clock,   /* pass clock handle
                                                                                to the notify fct. */
                                            dt_clock_brdcst_tm_nfy,
                                            &(pt_clock_ctrl->pt_clock->h_atsc_stt),
                                            &e_cond) == TMR_OK
                        )
                    {
                        DBG_INFO(("{DT} clock [%s] load ATSC STT table successful.\n",pt_clock_ctrl->pt_clock->s_name));
                        dt_tb_src_set_sel_order(pt_clock_ctrl, t_sync_src_type);
                        i4_rc = DTR_OK;
                    }
                    else
                    {
                        DBG_ERROR(("{DT} clock [%s] Failed to load ATSC STT table.\n",pt_clock_ctrl->pt_clock->s_name));
                        i4_rc = DTR_FAIL;
                    }
                }
                break;

                default:
                {
                    i4_rc = DTR_FAIL;
                }
                break;
            }
        }
        else
        {
            DBG_ERROR(("{DT} clock [%s] TM root notification function reg failed.\n",pt_clock_ctrl->pt_clock->s_name));
            i4_rc = DTR_FAIL;
        }
    }
    else
    {
        DBG_ERROR(("{DT} clock [%s] Failed to open TM root.\n",pt_clock_ctrl->pt_clock->s_name));
        i4_rc = DTR_FAIL;
    }

    
    dt_clock_ungrab_sema(pt_clock_ctrl->pt_clock->h_sema);

    return i4_rc;
}



/*****************************************************************
 * Name: dt_clock_reg_atsc_functions
 *
 * Description:  This function registers ATSC function table.
 *
 * Inputs:
 *   pt_table     ATSC function table processing API's.
 *
 * Outputs: -
 *   VOID
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
INT32 dt_clock_reg_atsc_functions(const DT_BRDCST_ATSC_API_T* pt_table)
{
    INT32   i4_rc = DTR_OK;
    
    pt_atsc_brdcst_api = pt_table;
    
    return i4_rc;
}

/*****************************************************************
 * Name: dt_clock_reg_dvb_functions
 *
 * Description:  This function registers DVB function table.
 *
 * Inputs:
 *   pt_table     DVB function table processing API's.
 *
 * Outputs: -
 *   VOID
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
INT32 dt_clock_reg_dvb_functions(const DT_BRDCST_DVB_API_T* pt_table)
{
    INT32   i4_rc = DTR_OK;
    
    pt_dvb_brdcst_api = pt_table;
    
    return i4_rc;
}



