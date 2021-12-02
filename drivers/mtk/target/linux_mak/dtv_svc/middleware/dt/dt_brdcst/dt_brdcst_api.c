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
 * $Revision: #2 $
 * $Date: 2012/06/20 $
 * $Author: weihua.chen $
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
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

/*
  Handle to the root object of the ATSC table tree.
*/
static HANDLE_T     h_src_root = NULL_HANDLE;

/*
  Handle to the STT
*/
static HANDLE_T     h_stt      = NULL_HANDLE;

/*
  Handle to the DVB TDT engine
*/
static HANDLE_T     h_dvb_tdt  = NULL_HANDLE ;

/*
  Handle to the DVB TOT engine.
*/
static HANDLE_T     h_dvb_tot  = NULL_HANDLE ;

static UINT32       ui4_seq    = 0;

/*---------------------------------------------------------------------
                     data declaraions
 ---------------------------------------------------------------------*/
static const DT_BRDCST_ATSC_API_T*   pt_atsc_brdcst_api = NULL;

static const DT_BRDCST_DVB_API_T*    pt_dvb_brdcst_api = NULL;

static const DT_ANALOG_BRDCST_VBI_TELETEXT_API_T* pt_analog_brdcst_api = NULL;


/*----------------------------------------------------------------------
 *  analog synchronization information.
 *----------------------------------------------------------------------*/
static  DT_SYNC_SRC_TYPE_T _t_analog_sync_src_type=DT_SYNC_SRC_NONE ;

static  DT_SRC_TYPE_DESC_T _t_analog_src_type_desc=DT_SRC_TYPE_FORMAT_UNKNOWN;

static  CHAR               _s_analog_tuner_name[24];

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

static  UINT32   ui4_internal_src_state        = DT_SRC_ACCEPT_ANY   ;

static  DT_SYNC_SRC_TYPE_T  _t_sync_src_type   = DT_SYNC_SRC_NONE    ;

static INT32 dt_tb_src_set_sel_order(DT_SYNC_SRC_TYPE_T t_src_type)
{
    switch (t_src_type)
    {
        case DT_SYNC_WITH_DVB_TDT:
        {
            ui4_internal_src_state =  DT_SRC_TDT_ONLY ;
        }
        break;

        case DT_SYNC_WITH_DVB_TOT:
        {
            ui4_internal_src_state =  DT_SRC_TOT_ONLY ;
        }
        break;

        case DT_SYNC_WITH_DVB_TOT_OR_TDT:
        {
            ui4_internal_src_state =  DT_SRC_TOT_OR_TDT ;
        }
        break;

        case DT_SYNC_WITH_DVB_TDT_OR_TOT:
        {
            ui4_internal_src_state =  DT_SRC_TDT_OR_TOT ;
        }
        break;
        
        default:
        {
            ui4_internal_src_state = DT_SRC_ACCEPT_ANY  ;
        }
        break;
    }

    /* save the sync source specified by the application. */
    _t_sync_src_type = t_src_type;
    
    return DTR_OK;
}

/****************************************************************
*  Name dt_accept_systm_from_src
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

static BOOL dt_accept_systm_from_src(UINT8 t_src_type)
{
    UINT32   ui4_state ;
    BOOL     b_rc;

    ui4_state = ui4_internal_src_state ;
    
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
                ui4_internal_src_state =  DT_SRC_TOT_ONLY ;
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
                ui4_internal_src_state =  DT_SRC_TDT_ONLY ;
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
        if ( _t_analog_sync_src_type == DT_SYNC_SRC_NONE )
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
 * Name: dt_tm_root_nfy_fct
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
 *   pv_nfy_tag  contains the notification tag.
 *   ui4_data    contains the address to the STT object.
 *
 * Outputs: -
 *   None.
 *
 * Returns:
 *   NFY_RET_PROCESSED:  table section has been processed.
 *
 -------------------------------------------------------------------------*/
static NFY_RET_T dt_tm_root_nfy_fct
(
    HANDLE_T       h_obj,
    HANDLE_TYPE_T  e_obj_type,
    TM_COND_T      e_nfy_cond,
    VOID*          pv_nfy_tag,
    UINT32         ui4_data
)
{
    NFY_RET_T      t_return;
    INT32          i4_rc;

    /* set the return code to data processed. */
    t_return = NFY_RET_PROCESSED;
    
    switch (e_nfy_cond)
    {
        case TM_COND_SRC_DISC:
        {
            ui4_seq++;

            /*
              Set the new condition.
            */

            DBG_INFO(("{DT} TM ROOT Notification => TM_COND_SRC_DISC: Set system clock to DT_FREE_RUNNING.\n"));

            i4_rc = dt_set_cond((INT32)DT_CLOCK_BRDCST_UTC, DT_FREE_RUNNING);

            dt_reset_last_stt_time();

            if ( dt_user_clock_sync_with_brdcst_clock() == TRUE )
            {
                dt_set_cond((INT32)DT_CLOCK_USER_UTC, DT_FREE_RUNNING);
            }

            /*
              Notify the register client(s) of change in
              the system clock running state.
            */
            if ( i4_rc == DTR_WILL_NOTIFY )
            {
                DBG_INFO(("{DT} Notify clients with DT_FREE_RUNNING cond.\n"));
                dt_nfy_all_clients(DT_FREE_RUNNING, 0);
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
            if ( _t_sync_src_type == DT_SYNC_WITH_DVB_TDT_OR_TOT )
            {
                ui4_internal_src_state = DT_SRC_TDT_OR_TOT ;
            }
            
            if ( _t_sync_src_type == DT_SYNC_WITH_DVB_TOT_OR_TDT )
            {
                ui4_internal_src_state = DT_SRC_TOT_OR_TDT ;
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
    return t_return;
}

/*-----------------------------------------------------------------------
 * Name: dt_brdcst_tm_nfy
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
 *   pv_nfy_tag  contains the notification tag.
 *   ui4_data    contains the address to the STT object.
 *
 * Outputs: -
 *   None.
 *
 * Returns:
 *   NFY_RET_PROCESSED:  table section has been processed.
 *
 -------------------------------------------------------------------------*/
static NFY_RET_T dt_brdcst_tm_nfy
(
    HANDLE_T       h_obj,
    HANDLE_TYPE_T  e_obj_type,
    TM_COND_T      e_nfy_cond,
    VOID*          pv_nfy_tag,
    UINT32         ui4_data
)
{
    NFY_RET_T      t_return;
    const UINT8*   aui1_stt_data;
    INT32          i4_rc;

    /* set the return code to data processed. */
    t_return = NFY_RET_PROCESSED;

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
            DBG_INFO(("{DT} sys time table section received, table id: 0x%x\n",
                     (UINT8)aui1_stt_data[0]));

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
                        pt_atsc_brdcst_api->pf_parse_ib_stt(h_obj,
                                                            e_obj_type,
                                                            pv_nfy_tag,
                                                            aui1_stt_data);
                    }
                }
                break;

                case STT_OUT_OF_BAND_TBL_ID:
                {
                    if ( pt_atsc_brdcst_api != NULL )
                    {
                        pt_atsc_brdcst_api->pf_parse_oob_stt(h_obj,
                                                             e_obj_type,
                                                             pv_nfy_tag,
                                                             aui1_stt_data);
                    }
                }
                break;

                case DVB_TDT_TBL_ID:
                {
                    if ( pt_dvb_brdcst_api != NULL )
                    {
                        pt_dvb_brdcst_api->pf_parse_dvb_tdt_tot(h_obj,
                                                                e_obj_type,
                                                                pv_nfy_tag,
                                                                aui1_stt_data);
                    }
                }
                break;

                case DVB_TOT_TBL_ID:
                {
                    if ( pt_dvb_brdcst_api != NULL )
                    {
                        pt_dvb_brdcst_api->pf_parse_dvb_tdt_tot(h_obj,
                                                                e_obj_type,
                                                                pv_nfy_tag,
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
            DBG_INFO(("{DT} STT Unavailable.\n"));

            /*
              STT is not available, so we set the condition to free running.
            */

            DBG_INFO(("{DT} STT Notification => TM_COND_UNAVAILABLE Set system clock to DT_FREE_RUNNING.\n"));

            i4_rc = dt_set_cond((INT32)DT_CLOCK_BRDCST_UTC, DT_FREE_RUNNING);

            dt_reset_last_stt_time();

            if ( dt_user_clock_sync_with_brdcst_clock() == TRUE )
            {
                dt_set_cond((INT32)DT_CLOCK_USER_UTC, DT_FREE_RUNNING);
            }

            /*
              Notify the register client(s) of change in
              the system clock running state.
            */
            if ( i4_rc == DTR_WILL_NOTIFY )
            {
                DBG_INFO(("{DT} Notify clients with DT_FREE_RUNNING cond.\n"));
                dt_nfy_all_clients(DT_FREE_RUNNING, 0);
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

/*--------------------------------------------------------------------------
 * Name: dt_rm_comp_sel_fct
 *
 * Description - Select the tuner component when multiple tuner of same type
 *               has been registered with resouce manager.
 *
 * Input  - 
 *    e_type:  Tuner driver type.
 *    ui2_comp_id:  component id for this driver.
 *    pv_comp_sel_data: client data specified in rm_comp_open() that is
 *                      passed to the select function .
 *    pv_comp_data:  Driver data for this component.
 *
 * Output - None
 *
 * Return - Void
 *
 --------------------------------------------------------------------------*/
SELECT_T dt_rm_comp_sel_fct
(
    DRV_TYPE_T   e_type,
    UINT16       ui2_comp_id,
    VOID*        pv_comp_sel_data,
    const VOID*  pv_comp_data,
    SIZE_T       z_comp_data_len,
    UINT16       ui2_num_active,
    UINT16       ui2_num_wait,
    CTRL_TYPE_T  e_ctrl,
    BOOL         b_would_conflict
)
{
    SELECT_T     t_select_rc;

    /* pick the first one found. */
    t_select_rc = SEL_COMP_BREAK;

    /* t_select_rc = SEL_COMP_NEXT; */

    DBG_INFO(("{DT} RM component [comp id %d] selected \n",ui2_comp_id));
    
    return t_select_rc;
}

#if 0
/*--------------------------------------------------------------------------
 * Name: dt_rm_cond_util
 *
 * Description - Interpret the RM condition and print out its meaning.
 *
 * Input  - 
 *    e_cond:   RM condition
 *
 * Output - None
 *
 * Return - Void
 *
 --------------------------------------------------------------------------*/
VOID  dt_rm_cond_util(RM_COND_T   e_cond)
{
    /*
      Debug print to interpret the states of RM notify condition
    */
    x_dbg_stmt("\ne_cond: [%d]\n", e_cond); 
    switch (rm_cond_chg (e_cond, RM_COND_IGNORE, COND_GRP_OBJ_STATE))
    {
        case RM_COND_IGNORE:
            break;

        case RM_COND_CLOSED:
            x_dbg_stmt ("      RM_COND_CLOSED\n");
            break;

        case RM_COND_OPENING:
            x_dbg_stmt ("      RM_COND_OPENING\n");
            break;

        case RM_COND_OPENED:
            x_dbg_stmt ("      RM_COND_OPENED\n");
            break;

        case RM_COND_CLOSING:
            x_dbg_stmt ("      RM_COND_CLOSING\n");
            break;

        default:
            x_dbg_stmt ("      Unknown COND_GRP_OBJ_STATE\n");
            break;
    }

    switch (rm_cond_chg (e_cond, RM_COND_IGNORE, COND_GRP_CONN_STATE))
    {
        case RM_COND_IGNORE:
            break;

        case RM_COND_DISCONNECTED:
            x_dbg_stmt ("      RM_COND_DISCONNECTED\n");
            break;

        case RM_COND_CONNECTING:
            x_dbg_stmt ("      RM_COND_CONNECTING\n");
            break;

        case RM_COND_CONNECTED:
            x_dbg_stmt ("      RM_COND_CONNECTED\n");
            break;

        case RM_COND_DISCONNECTING:
            x_dbg_stmt ("      RM_COND_DISCONNECTING\n");
            break;

        case RM_COND_HARD_WIRED:
            x_dbg_stmt ("      RM_COND_HARD_WIRED\n");
            break;

        case RM_COND_MULTI_PORT:
            x_dbg_stmt ("      RM_COND_MULTI_PORT\n");
            break;

        default:
            x_dbg_stmt ("      Unknown COND_GRP_CONN_STATE\n");
            break;
    }

    switch (rm_cond_chg (e_cond, RM_COND_IGNORE, COND_GRP_SHARE_STATE))
    {
        case RM_COND_IGNORE:
            break;

        case RM_COND_SLAVE:
            x_dbg_stmt ("      RM_COND_SLAVE\n");
            break;

        case RM_COND_MASTER:
            x_dbg_stmt ("      RM_COND_MASTER\n");
            break;

        case RM_COND_SHARED:
            x_dbg_stmt ("      RM_COND_SHARED\n");
            break;

        default:
            x_dbg_stmt ("      Unknown COND_GRP_SHARE_STATE\n");
            break;
    }

    switch (rm_cond_chg (e_cond, RM_COND_IGNORE, COND_GRP_ARBITRATION))
    {
        case RM_COND_IGNORE:
            break;

        case RM_COND_REQ_OPEN:
            x_dbg_stmt ("      RM_COND_REQ_OPEN\n");
            break;

        case RM_COND_REQ_CONN:
            x_dbg_stmt ("      RM_COND_REQ_CONN\n");
            break;

        default:
            x_dbg_stmt ("      Unknown COND_GRP_ARBITRATION\n");
            break;
    }

    switch (rm_cond_chg (e_cond, RM_COND_IGNORE, COND_GRP_REASON))
    {
        case RM_COND_IGNORE:
            break;

        case RM_COND_REASON_RIGHTS:
            x_dbg_stmt ("      RM_COND_REASON_RIGHTS\n");
            break;

        case RM_COND_REASON_MULTI_COMPS:
            x_dbg_stmt ("      RM_COND_REASON_MULTI_COMPS\n");
            break;

        case RM_COND_REASON_CONF_CTRL_MODE:
            x_dbg_stmt ("      RM_COND_REASON_CONF_CTRL_MODE\n");
            break;

        case RM_COND_REASON_CONF_COMP:
            x_dbg_stmt ("      RM_COND_REASON_CONF_COMP\n");
            break;

        case RM_COND_REASON_ARBITRATION:
            x_dbg_stmt ("      RM_COND_REASON_ARBITRATION\n");
            break;

        default:
            x_dbg_stmt ("      Unknown COND_GRP_REASON\n");
            break;
    }

    switch (rm_cond_chg (e_cond, RM_COND_IGNORE, COND_GRP_CONN_ORIG))
    {
        case RM_COND_IGNORE:
            break;

        case RM_COND_CONN_ORIGINATOR:
            x_dbg_stmt ("      RM_COND_CONN_ORIGINATOR\n");
            break;

        case RM_COND_CONN_PENDING:
            x_dbg_stmt ("      RM_COND_CONN_PENDING\n");
            break;

        default:
            x_dbg_stmt ("      Unknown COND_GRP_CONN_ORIG\n");
            break;
    }

    switch (rm_cond_chg (e_cond, RM_COND_IGNORE, COND_GRP_OPERATION))
    {
        case RM_COND_IGNORE:
            break;

        case RM_COND_SET_OPERATION:
            x_dbg_stmt ("      RM_COND_SET_OPERATION\n");
            break;

        case RM_COND_CLIENT_SET_OPERATION:
            x_dbg_stmt ("      RM_COND_CLIENT_SET_OPERATION\n");
            break;

        case RM_COND_CLIENT_GET_OPERATION:
            x_dbg_stmt ("      RM_COND_CLIENT_GET_OPERATION\n");
            break;

        case RM_COND_CLIENT_GET_LEN_OPERATION:
            x_dbg_stmt ("      RM_COND_CLIENT_GET_LEN_OPERATION\n");
            break;

        case RM_COND_CONNECT_TRANS:
            x_dbg_stmt ("      RM_COND_CONNECT_TRANS\n");
            break;

        default:
            x_dbg_stmt ("      Unknown COND_GRP_OPERATION\n");
            break;
    }

    switch (rm_cond_chg (e_cond, RM_COND_IGNORE, COND_GRP_MON_COMP_STATE))
    {
        case RM_COND_IGNORE:
            break;

        case RM_COND_COMP_ACTIVE:
            x_dbg_stmt ("      RM_COND_COMP_ACTIVE\n");
            break;

        default:
            x_dbg_stmt ("      Unknown COND_GRP_MON_COMP_STATE\n");
            break;
    }
}
#endif


/*--------------------------------------------------------------------------
 * Name: dt_get_rm_comp_cond
 *
 * Description - Interpret the RM condition.  For monitoring the
 *    Analog tuner, we are interesting in two different state:
 *
 *    {RM_COND_OPENED, RM_COND_DISCONNECTED}   [When current analog
 *    channel stop playing].
 *
 *    {RM_COND_OPENED, RM_COND_CONNECTED, RM_COND_COMP_ACTIVE} [When
 *    analog channel is playing out]
 *
 * Input  - 
 *    e_cond:   RM condition
 *
 * Output - None
 *
 * Return - Void
 *
 --------------------------------------------------------------------------*/
#define    RM_ANALOG_TUNER_STATE_UNKNOWN           -1
#define    RM_ANALOG_TUNER_DISCONNECTED             0
#define    RM_ANALOG_TUNER_CONNECTED_AND_PLAYING    1

static INT32  dt_get_rm_comp_cond(RM_COND_T   e_cond)
{
    /*
      RM component status

      -1 -> unknown
      0 -> false
      1 -> true
    */    
    INT32     comp_opened   = -1 ;
    INT32     comp_connected = -1 ;
    INT32     comp_active    = -1 ;
    INT32     comp_state     = RM_ANALOG_TUNER_STATE_UNKNOWN;

    /* x_dbg_stmt("e_cond: [%d]\n", e_cond); */
    /*
      Debug print to interpret the states of RM notify condition
    */
    switch (rm_cond_chg (e_cond, RM_COND_IGNORE, COND_GRP_OBJ_STATE))
    {
        case RM_COND_IGNORE:
            break;

        case RM_COND_CLOSED:
            /* x_dbg_stmt("       RM_COND_CLOSED\n"); */
            comp_opened = 0;
            break;

        case RM_COND_OPENED:
            /* x_dbg_stmt ("      RM_COND_OPENED\n"); */
            comp_opened = 1 ;
            break;
            
        default:
            break;
    }

    switch (rm_cond_chg (e_cond, RM_COND_IGNORE, COND_GRP_CONN_STATE))
    {
        case RM_COND_IGNORE:
            break;

        case RM_COND_DISCONNECTED:
            /* x_dbg_stmt ("      RM_COND_DISCONNECTED\n"); */
            comp_connected = 0 ;
            break;

        case RM_COND_CONNECTED:
            /* x_dbg_stmt ("      RM_COND_CONNECTED\n"); */
            comp_connected = 1 ;
            break;

        default:
            break;
    }

    switch (rm_cond_chg (e_cond, RM_COND_IGNORE, COND_GRP_MON_COMP_STATE))
    {
        case RM_COND_IGNORE:
            break;

        case RM_COND_COMP_ACTIVE:
            /* x_dbg_stmt ("      RM_COND_COMP_ACTIVE\n");  */
            comp_active  = 1;
            break;

        default:
            break;
    }

    if ( (comp_opened == 1) && (comp_connected == 1) && (comp_active == 1) )
    {
        /* analog channel is playing. */
        comp_state = RM_ANALOG_TUNER_CONNECTED_AND_PLAYING ;
    }

    if ( (comp_opened == 1) && (comp_connected == 0) && (comp_active == -1) )
    {
        /* ananlog channel stops playing. */
        comp_state = RM_ANALOG_TUNER_DISCONNECTED ;
    }

    /* All other cases is ignored. */
    return comp_state;
}


/*-----------------------------------------------------------------------
 * Name: dt_rm_analog_tuner_nfy
 *
 * Description:  This function is called by the RM analog tuner component
 *               when its status is changed.
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
 *   pv_nfy_tag  contains the notification tag.
 *   ui4_data    contains the address to the STT object.
 *
 * Outputs: -
 *   None.
 *
 * Returns:
 *   NFY_RET_PROCESSED:  table section has been processed.
 *
 -------------------------------------------------------------------------*/
static VOID dt_rm_analog_tuner_nfy
(
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_cond,
    VOID*      pv_tag,
    UINT32     ui4_data
)
{

    DT_NFY_MSG_T    t_msg;
    INT32           i4_rc;

    /* send notification to the DT notification processing thread,
       which will invoke 'dt_analog_tuner_nfy_handler' function.
    */
    
    t_msg.t_msg_type                     = DT_ANALOG_TUNER_NOTIFICATION;
    
    t_msg.u.t_anal_tuner_info.h_obj      = h_obj;
    t_msg.u.t_anal_tuner_info.ui1_port   = ui1_port;
    t_msg.u.t_anal_tuner_info.e_nfy_cond = e_cond ;
    t_msg.u.t_anal_tuner_info.pv_tag     = pv_tag;
    t_msg.u.t_anal_tuner_info.ui4_data   = ui4_data;
    
    
    i4_rc = dt_send_nfy_msg (&t_msg);
    
    if (i4_rc != DTR_OK)
    {
        DBG_ERROR(("{DT} cannot send msg to DT notification processing thread (ANALOG_TUNER_NOTIFICATION), i4_rc = %d.\n", DTR_OK));  
    }
    
    
    
#if 0    
    INT32      i4_rc;
    INT32      i4_cond;

    i4_cond = dt_get_rm_comp_cond(e_cond);
    
    switch (i4_cond)
    {
        /*  Analog tuner is dis-connected and not active.
            { RM_COND_OPENED, RM_COND_DISCONNECTED }
        */
        case RM_ANALOG_TUNER_DISCONNECTED:  
        {
            DBG_INFO(("{DT} analog tuner dis-connected\n"));
            if ( _t_analog_sync_src_type == DT_SYNC_WITH_ANALOG_TELETEXT )
            {
                if ( (pt_analog_brdcst_api != NULL) )
                {
                    i4_rc = pt_analog_brdcst_api->pf_close_analog_source(FALSE);
                    
                    if ( (i4_rc == DTR_OK) || (i4_rc == DTR_WILL_NOTIFY) )
                    {
                        if ( i4_rc == DTR_WILL_NOTIFY )
                        {
                            i4_rc = DTR_OK;
                            /*
                              VBI filter source has been deleted, set the sync source
                              condition and determine if it is necessary to notify
                              client of the condition change..
                            */   
                            if ( dt_set_cond(DT_CLOCK_BRDCST_UTC, DT_NO_SYNC_SRC) ==
                                 DTR_WILL_NOTIFY )
                            {
                                /*
                                  Notify the client(s) that system clock is
                                  now in the no sync source state.
                                */
                                DBG_INFO(("{DT} Notify clients with DT_NO_SYNC_SRC cond.\n"));
                                dt_nfy_all_clients(DT_NO_SYNC_SRC, 0);
                            }
                        }
                    }

                    /*
                       If the user clock is linked with broadcast clock, then
                       set the condition and for the user clock.
                    */
                    if ( dt_user_clock_sync_with_brdcst_clock() == TRUE )
                    {
                        dt_set_cond(DT_CLOCK_USER_UTC, DT_NO_SYNC_SRC);
                    }
                }
            }
        }
        break;

        /* Analog tuner is connected, active, and playing
           { RM_COND_OPENED, RM_COND_CONNECTED, RM_COND_COMP_ACTIVE}
        */
        case RM_ANALOG_TUNER_CONNECTED_AND_PLAYING: 
        {
            DBG_INFO(("{DT} analog tuner connected\n"));
            
            if ( _t_analog_sync_src_type == DT_SYNC_WITH_ANALOG_TELETEXT )
            {
                if ( (pt_analog_brdcst_api != NULL) )
                {
                    i4_rc = pt_analog_brdcst_api->pf_open_analog_source(
                        _t_analog_sync_src_type,
                        _t_analog_src_type_desc,
                        _s_analog_tuner_name,
                        h_obj);

                    if ( i4_rc != DTR_OK )
                    {
                        _t_analog_sync_src_type = DT_SYNC_SRC_NONE ;
                        _t_analog_src_type_desc = DT_SRC_TYPE_FORMAT_UNKNOWN ;
                        x_memset(_s_analog_tuner_name,0,
                                 sizeof(_s_analog_tuner_name));
                    }
                }
            }
        }
        break;

        default:
        {
        }
        break;
    }
    
    return ;
#endif    
}

/*-----------------------------------------------------------------------
 * Name: dt_analog_tuner_nfy_handler
 *
 * Description:  This function process the RM analog tuner component
 *               notification when its status is changed.  This
 *               function is invoked in DT notification processing
 *               thread, NOT in the RM notification thread context.
 *
 * Inputs:
 *   h_obj       handle to the analog tuner component..
 *
 *   e_obj_type 
 *
 *   e_cond  contains the RM condition of the analog component.
 *
 *   pv_tag  contains the notification tag.
 *
 *   ui4_data   
 *
 * Outputs: -
 *   None.
 *
 * Returns:
 *   NFY_RET_PROCESSED:  table section has been processed.
 *
 -------------------------------------------------------------------------*/
INT32 dt_analog_tuner_nfy_handler
(
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_cond,
    VOID*      pv_tag,
    UINT32     ui4_data
)
{
    INT32      i4_rc = DTR_OK;
    INT32      i4_cond;

    i4_cond = dt_get_rm_comp_cond(e_cond);
    
    switch (i4_cond)
    {
        /*  Analog tuner is dis-connected and not active.
            { RM_COND_OPENED, RM_COND_DISCONNECTED }
        */
        case RM_ANALOG_TUNER_DISCONNECTED:  
        {
            DBG_INFO(("{DT} analog tuner dis-connected\n"));
            if ( _t_analog_sync_src_type == DT_SYNC_WITH_ANALOG_TELETEXT )
            {
                if ( (pt_analog_brdcst_api != NULL) )
                {
                    i4_rc = pt_analog_brdcst_api->pf_close_analog_source(FALSE);
                    
                    if ( (i4_rc == DTR_OK) || (i4_rc == DTR_WILL_NOTIFY) )
                    {
                        if ( i4_rc == DTR_WILL_NOTIFY )
                        {
                            i4_rc = DTR_OK;
                            /*
                              VBI filter source has been deleted, set the sync source
                              condition and determine if it is necessary to notify
                              client of the condition change..
                            */   
                            if ( dt_set_cond((INT32)DT_CLOCK_BRDCST_UTC, DT_NO_SYNC_SRC) ==
                                 DTR_WILL_NOTIFY )
                            {
                                /*
                                  Notify the client(s) that system clock is
                                  now in the no sync source state.
                                */
                                DBG_INFO(("{DT} Notify clients with DT_NO_SYNC_SRC cond.\n"));
                                dt_nfy_all_clients(DT_NO_SYNC_SRC, 0);
                            }
                        }
                    }

                    /*
                       If the user clock is linked with broadcast clock, then
                       set the condition and for the user clock.
                    */
                    if ( dt_user_clock_sync_with_brdcst_clock() == TRUE )
                    {
                        dt_set_cond((INT32)DT_CLOCK_USER_UTC, DT_NO_SYNC_SRC);
                    }
                }
            }
        }
        break;

        /* Analog tuner is connected, active, and playing
           { RM_COND_OPENED, RM_COND_CONNECTED, RM_COND_COMP_ACTIVE}
        */
        case RM_ANALOG_TUNER_CONNECTED_AND_PLAYING: 
        {
            DBG_INFO(("{DT} analog tuner connected\n"));
            
            if ( _t_analog_sync_src_type == DT_SYNC_WITH_ANALOG_TELETEXT )
            {
                if ( (pt_analog_brdcst_api != NULL) )
                {
                    i4_rc = pt_analog_brdcst_api->pf_open_analog_source(
                        _t_analog_sync_src_type,
                        _t_analog_src_type_desc,
                        _s_analog_tuner_name,
                        h_obj);

                    if ( i4_rc != DTR_OK )
                    {
                        _t_analog_sync_src_type = DT_SYNC_SRC_NONE ;
                        _t_analog_src_type_desc = DT_SRC_TYPE_FORMAT_UNKNOWN ;
                        x_memset(_s_analog_tuner_name,0,
                                 sizeof(_s_analog_tuner_name));
                    }
                }
            }
        }
        break;

        default:
        {
        }
        break;
    }
    
    return i4_rc;
}

/*-----------------------------------------------------------------------
 * Name: dt_analog_get_connection_handle
 *
 * Description:
 *   This function get the RM handle for the Analog Tuner.
 *   APIs.
 *
 * Inputs:
 *   None
 *
 * Outputs: 
 *   ph_connection
 *
 * Returns:
 *   NFY_RET_PROCESSED:  table section has been processed.
 *
 -------------------------------------------------------------------------*/

static HANDLE_T   _g_h_rm_analog_tuner = NULL_HANDLE;

static INT32 dt_open_analog_tuner
(
    DT_SRC_TYPE_DESC_T t_src_type_desc,
    const VOID*        pv_src_info,
    HANDLE_T*          ph_rm_analog_tuner
)
{
    INT32              i4_rc       = DTR_OK;
    HANDLE_T           h_rm_pipe   = NULL_HANDLE;
    DRV_TYPE_T         t_drv_type  = DRVT_TUNER_TER_ANA;
    CHAR*              ps_name     = NULL;
    RM_COND_T          e_rm_cond;

    switch (t_src_type_desc)
    {
        case DT_SRC_TYPE_VBI_ANA_TER:
        {
            t_drv_type = DRVT_TUNER_TER_ANA;
        }
        break;

        case DT_SRC_TYPE_VBI_ANA_CAB:
        {
            t_drv_type = DRVT_TUNER_CAB_ANA;
        }
        break;

        case DT_SRC_TYPE_VBI_ANA_SAT:
        {
            t_drv_type = DRVT_TUNER_SAT_ANA;
        }
        break;

        default:
        {
            DBG_ERROR(("{DT} Invalid analog tuner type [%d].\n",t_src_type_desc));
            i4_rc = DTR_FAIL;
        }
        break;
    }

    if ( i4_rc == DTR_FAIL )
    {
        return i4_rc;
    }
    
    if ( pv_src_info != NULL )
    {
        ps_name = (CHAR*) pv_src_info;
    }

    *ph_rm_analog_tuner = NULL_HANDLE ;
    
    if ( _g_h_rm_analog_tuner == NULL_HANDLE )
    {
        i4_rc = rm_open_comp(h_rm_pipe,
                             OBJ_COMP_REL_IGNORE,
                             t_drv_type,
                             ANY_COMP_ID,
                             ps_name,
                             CTRL_TYPE_MONITOR,
                             NULL,
                             dt_rm_comp_sel_fct, 
                             (VOID*) NULL,        /* index */
                             dt_rm_analog_tuner_nfy,
                             &(_g_h_rm_analog_tuner),
                             &e_rm_cond);

        if ( (i4_rc == RMR_OK)                 ||
             (i4_rc == RMR_ASYNC_NFY)          ||
             (i4_rc == RMR_ARBITRATION)
            )
        {
            if ( dt_get_rm_comp_cond(e_rm_cond) ==
                 RM_ANALOG_TUNER_CONNECTED_AND_PLAYING
                )
            {
                DBG_INFO(("{DT} Analog tuner [%d] opened.\n", _g_h_rm_analog_tuner));
                i4_rc = DTR_OK;
            }
            else
            {
                DBG_INFO(("{DT} Analog tuner [%d] opened but not currently playing.\n",_g_h_rm_analog_tuner));
                /*
                  We will get notify when analog tuner is connected and
                  playing later on, at that time the VBIF filter will
                  be open.
                */
                i4_rc = DTR_WILL_NOTIFY ;
            }
        }
        else
        {
            DBG_ERROR(("{DT} Error: can not analog tuner [%d].\n",i4_rc));
            i4_rc = DTR_FAIL;
        }
    }

    *ph_rm_analog_tuner = _g_h_rm_analog_tuner;
    
    return i4_rc;
}

/*-----------------------------------------------------------------------
 * Name: dt_close_analog_tuner
 *
 * Description:
 *   This function get the current connection handle from Service Context
 *   APIs.
 *
 * Inputs:
 *   None
 *
 * Outputs: 
 *   ph_connection
 *
 * Returns:
 *   NFY_RET_PROCESSED:  table section has been processed.
 *
 -------------------------------------------------------------------------*/
static INT32 dt_close_analog_tuner(VOID)
{
    RM_COND_T  e_cond;
    
    if ( _g_h_rm_analog_tuner != NULL_HANDLE )
    {
        rm_close(_g_h_rm_analog_tuner, FALSE, &e_cond);
        DBG_INFO(("{DT} close analog tuner. [%d]\n",_g_h_rm_analog_tuner));
        _g_h_rm_analog_tuner = NULL_HANDLE;
    }
    return DTR_OK;
}

/*-----------------------------------------------------------------------
 * Name: dt_sync_with_stt
 *
 * Description:  This function set the system time library with
 *               the STT data that has arrived.
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
INT32 dt_sync_with_stt
(
    STT_DATA_T*     pt_stt
)
{
    TIME_T          t_delta = 0;
    INT32           i4_rc  = DTR_OK;

    TIME_T          t_sys_tm;
    DT_COND_T       t_sys_cond;
    DT_COND_T       t_new_cond = DT_FREE_RUNNING;
    BOOL            b_set_sys_tm;

    /*
      Determine if we should use this time to set the system time.
      This is to handle the DVB broadcast where we can receive system
      time from TOT or TDT.
    */
    b_set_sys_tm = dt_accept_systm_from_src(pt_stt->ui1_src_type );

    if ( (b_set_sys_tm == TRUE) &&
         (dt_gross_error_check_on_input_time(pt_stt->t_sec) == TRUE)
        )
    {  
        t_delta = 0;

        DBG_INFO(("{DT} Sync system time with => src tbl id: 0x%x, UTC: %lld sec\n",
                 pt_stt->ui1_src_type,
                 pt_stt->t_sec));

        /* The default new system clock state. */
        t_new_cond = DT_SYNC_RUNNING ;

        /*
          Check the new current time with the current time
          to see how much delta there is between the current
          time of system clock and current time from the STT.
        */
        t_sys_tm=dt_get_utc_and_cond((INT32)DT_CLOCK_BRDCST_UTC, NULL, &t_sys_cond);
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
          synchronize the broadcast clock with system time received.
        */
        i4_rc=dt_set_utc_and_cond((INT32)DT_CLOCK_BRDCST_UTC,
                                  pt_stt->t_sec, 0, t_new_cond);

        /*
          If the user clock is linked with broadcast clock, then
          set the condition and UTC for the user clock.
        */
        if ( dt_user_clock_sync_with_brdcst_clock() == TRUE )
        {
            DBG_INFO(("{DT} user clock is sync with brdcst clock"));
            dt_set_utc_and_cond((INT32)DT_CLOCK_USER_UTC,
                                pt_stt->t_sec, 0, t_new_cond);
        }
        else
        {
            DBG_INFO(("{DT} user clock is not sync with brdcst clock"));
            /*
              Notify the RTC of the latest time received from
              broadcast source For RTC drift adjustment.  This
              is only performed if RTC is not synced with
              broadcast source.
            */
            /*dt_rtc_set_sync_time (pt_stt->t_sec, ui4_seq); */
        }
        
    }
    

    /*
      If table contains DST info, then set the DST info in the DT library.
    */
    if ( pt_stt->b_ds_desc == TRUE )
    {        
        if ( pt_stt->ui1_src_type == STT_IN_BAND_TBL_ID      ||
             pt_stt->ui1_src_type == STT_OUT_OF_BAND_TBL_ID  )
        {
            if ( x_dt_get_dst_ctrl() == TRUE )
            {
                /*
                  Set the Day-Light Saving time from STT.
                */
                dt_set_day_sav_state(pt_stt->t_sec,
                                     pt_stt->b_ds_status,
                                     pt_stt->ui1_ds_day_of_mon,
                                     pt_stt->ui1_ds_hr);

                DBG_INFO(("{DT} Set day-light-saving state => src tbl id: 0x%x, "
                         "current dst status: %d "
                         "DST change time => day-of-mon: %d  hrs: %d \n",
                         pt_stt->ui1_src_type,
                         pt_stt->b_ds_status,
                         pt_stt->ui1_ds_day_of_mon,
                         pt_stt->ui1_ds_hr));
            }
            
        }
        else if (  pt_stt->ui1_src_type == DVB_TOT_TBL_ID )
        {
            if ( x_dt_get_dst_ctrl() == TRUE )
            {
                /*
                  Set the Day-Light-Saving Info from TOT.
                */
                dt_set_day_sav_state_from_tot(pt_stt->t_sec,
                                              pt_stt->b_ds_status,
                                              pt_stt->t_ds_change,
                                              pt_stt->t_ds_offset,
                                              t_new_cond);

                
                
                DBG_INFO(("{DT} Set day-light-saving state => src tbl id: 0x%x, "
                         "current dst status: %d "
                         "DST change time => %lld \n",
                         pt_stt->ui1_src_type,
                         pt_stt->b_ds_status,
                         pt_stt->t_ds_change));
            }
        }
    }
    else
    {
        /* Handle the case where we can get time jump (for example, after channel
           change).
           If the new brdcst time setting has a large jump (+2 sec) from the
           existing broadcast time.  This 'discontinous' check is to catch the
           case where after a channel change, we receive a slight different
           broadcast time than previous channel, but we still have the old DST
           setting, so we re-compute the delta T to the notification time
           and re-set the timer.
        */
        if ( (t_new_cond == DT_SYNC_DISCONT)   &&
             (pt_stt->ui1_src_type == DVB_TOT_TBL_ID)  &&
             (x_dt_get_dst_ctrl() == TRUE)
            )
        {
            dt_reset_dst_timer_nfy(pt_stt->t_sec,t_new_cond);
        }
    }

    if ( b_set_sys_tm == TRUE )
    {
        dt_set_last_sync_tbl_id(pt_stt->ui1_src_type);
        /*
          Notify the client only if this is change of system clock condition
          from DT_FREE_RUNNING to DT_SYNC_RUNNING, or from DT_SYNC_RUNNING to
          DT_SYNC_DISCONT (due to positive or negative jump in the received
          STT time.)
        */
        if ( i4_rc == DTR_WILL_NOTIFY )
        {
            DBG_INFO(("{DT} Notify clients with DT_SYNC_RUNNING cond.\n"));
            dt_nfy_all_clients(t_new_cond, t_delta);
        }

        /*
          Notify the client for timezone offset change.
        */
        if ( pt_stt->b_tz_offset_changed == TRUE )
        {
            DT_NFY_MSG_T       t_msg;

            t_msg.t_msg_type = DT_TZ_OFFSET_CHANGED_NOTIFICATION;
            t_msg.ui32_val = (UINT32) 0;

            /* Send msg to the DT notification thread. */
            dt_send_nfy_msg(&t_msg); 
        }
    }
    
    return i4_rc;
}

/*
  Semaphore Handle.
*/
static HANDLE_T      h_dt_sema = NULL_HANDLE;

/* --------------------------------------------------------------
  Initialize broadcast source synchronization.
---------------------------------------------------------------- */

VOID   dt_init_sync_src(VOID)
{
    /*
      Initialize the semaphore for protecting the change
      of synchronization source.
    */
    if ( h_dt_sema == NULL_HANDLE                       &&
         x_sema_create(&(h_dt_sema), X_SEMA_TYPE_MUTEX,
                       X_SEMA_STATE_UNLOCK) != OSR_OK
        )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
    }
}

static VOID dt_grab_sema_stt(VOID)
{
    if ( x_sema_lock( h_dt_sema, X_SEMA_OPTION_WAIT) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
    return;
}

static VOID dt_ungrab_sema_stt(VOID)
{
    if ( x_sema_unlock( h_dt_sema) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE);
    }
    return;
}

/*-----------------------------------------------------------------------
 * Name: _dt_free_sync_src_hdle_unprotected
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
static INT32 _dt_free_sync_src_hdle_unprotected
(
    VOID
)
{
    INT32       i4_rc;
    
    i4_rc = DTR_OK;
    
    /*
      Free the current source of the time table.
    */
    if ( h_src_root != NULL_HANDLE )
    {
        /*
          Delete the Table Manager Root
        */
        if (  x_tm_free(h_src_root) == HR_OK )
        {
            if ( x_handle_valid(h_src_root) == FALSE )
            {
                DBG_INFO(("{DT}: TM Root deleted.\n"));
                h_src_root = NULL_HANDLE ;
                /*
                  If the root is delete, the child will
                  become invalid as well.
                */
                if ( x_handle_valid(h_dvb_tdt) == FALSE   &&
                     x_handle_valid(h_dvb_tot) == FALSE   &&
                     x_handle_valid(h_stt)     == FALSE
                    )
                {
                    DBG_INFO(("{DT}: Handle(s) to engine (ATSC/DVB) deleted.\n"));
                    h_dvb_tdt = NULL_HANDLE ;
                    h_dvb_tot = NULL_HANDLE ;
                    h_stt     = NULL_HANDLE ;
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
 * Name: dt_unload_brdcst_time_info
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
INT32 dt_unload_brdcst_time_info
(
    VOID
)
{
    INT32       i4_rc;
    
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
    dt_grab_sema_stt();

    /*
      Free the current source of the time table.
    */
    i4_rc = _dt_free_sync_src_hdle_unprotected();
    
    if ( i4_rc == DTR_OK )
    {
        /*
          SI handle source has been deleted, set the sync source
          condition and determine if it is necessary to notify
          client of the condition change..
        */
        
        if ( dt_set_cond((INT32)DT_CLOCK_BRDCST_UTC, DT_NO_SYNC_SRC) ==
             DTR_WILL_NOTIFY )
        {
            /*
               Notify the client(s) that system clock is
               now in the no sync source state.
            */
            DBG_INFO(("{DT} Notify clients with DT_NO_SYNC_SRC cond.\n"));
            dt_nfy_all_clients(DT_NO_SYNC_SRC, 0);
        }

        /*
          If the user clock is linked with broadcast clock, then
          set the condition and for the user clock.
        */
        if ( dt_user_clock_sync_with_brdcst_clock() == TRUE )
        {
            dt_set_cond((INT32)DT_CLOCK_USER_UTC, DT_NO_SYNC_SRC);
        }
    }
    else
    {
        DBG_ERROR(("{DT} Error: can not delete sync source handle(s) to SI engine.\n"));
    }
    
    dt_ungrab_sema_stt();
    
    return i4_rc;
}

/*-----------------------------------------------------------------------
 * Name: dt_load_brdcst_time_info
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
INT32 dt_load_brdcst_time_info
(
    DT_SYNC_SRC_TYPE_T          t_sync_src_type,
    DT_SRC_TYPE_DESC_T          t_src_type_desc,
    const VOID*                 pv_src_info
)
{
    INT32                       i4_rc;
    TM_COND_T                   e_cond;
    TM_SRC_TYPE_T               t_tbm_src_type;

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
      then call x_dt_set_sync_src()

      2. From application thread, where it change sources of the STT
      (c_dt_set_sync_src() which calles x_dt_set_sync_src()).

      For example: to change source from Terrestrial STT to
                   Cable in-band STT.
    */
    dt_grab_sema_stt();
    
    /*
      Free the current source of the time table.
    */
    i4_rc = _dt_free_sync_src_hdle_unprotected();
    
    if ( i4_rc == DTR_OK )
    {
        /*
           STT source has been deleted, set the system clock
           condition to free running state.
        */
        DBG_INFO(("{DT} broadcast time source changed: => Set broadcast clock to DT_FREE_RUNNING.\n"));
        
        if ( dt_set_cond((INT32)DT_CLOCK_BRDCST_UTC, DT_FREE_RUNNING) ==
             DTR_WILL_NOTIFY )
        {
            /*
              Notify the client(s) that system clock is
              now in the free running state.
            */
            DBG_INFO(("{DT} Notify clients with DT_FREE_RUNNING cond.\n"));
            dt_nfy_all_clients(DT_FREE_RUNNING, 0);
        }

        dt_reset_last_stt_time();

        /*
          If the user clock is linked with broadcast clock, then
          set the condition and for the user clock.
        */
        if ( dt_user_clock_sync_with_brdcst_clock() == TRUE )
        {
            dt_set_cond((INT32)DT_CLOCK_USER_UTC, DT_FREE_RUNNING);
        }
    }
    else
    {
        DBG_ERROR(("{DT} Error: can not delete sync source handle(s) to SI engine.\n"));
        dt_ungrab_sema_stt();
        return i4_rc;
    }

    i4_rc =  TMR_NOT_INIT ;
    
    if (h_src_root == NULL_HANDLE)
    {
        i4_rc = x_tm_open_root("ROOT1",
                               t_tbm_src_type,
                               (VOID*)pv_src_info,
                               (VOID*)NULL,
                               dt_tm_root_nfy_fct,
                               &h_src_root,
                               &e_cond ) ;
    }

    if ( i4_rc == TMR_OK )
    {
        DBG_INFO(("{DT}: TM Root created successful.\n"));

        /*
          Enable notification from TM Root.
        */
        if ( tm_set_src_conn_nfy_cond(h_src_root,
                                      TRUE,
                                      &e_cond) == TMR_OK
            )
        {

            DBG_INFO(("{DT}: TM Root notification enabled.\n"));

            switch (t_sync_src_type)
            {
                case DT_SYNC_WITH_DVB_TDT:
                {
                    /* Load only TDT table. */
                    if ( x_dvb_si_load_tdt(h_src_root,
                                           (VOID*)NULL,
                                           dt_brdcst_tm_nfy,
                                           &h_dvb_tdt,
                                           &e_cond) == TMR_OK
                        )
                    {
                        DBG_INFO(("{DT}: load DVB TDT table successful.\n"));
                        dt_tb_src_set_sel_order(t_sync_src_type);
                        i4_rc = DTR_OK;
                    }
                    else
                    {
                        DBG_ERROR(("{DT} Failed to load DVB TDT table.\n"));
                        i4_rc = DTR_FAIL;
                    }
                }
                break;

                case DT_SYNC_WITH_DVB_TOT:
                {
                    /* Load only TOT table. */
                    if ( x_dvb_si_load_tot(h_src_root,
                                           (VOID*)NULL,
                                           dt_brdcst_tm_nfy,
                                           &h_dvb_tot,
                                           &e_cond) == TMR_OK
                        )
                    {
                        DBG_INFO(("{DT}: load DVB TOT table successful.\n"));
                        dt_tb_src_set_sel_order(t_sync_src_type);
                        i4_rc = DTR_OK;
                    }
                    else
                    {
                        DBG_ERROR(("{DT} Failed to load DVB TOT table.\n"));
                        i4_rc = DTR_FAIL;
                    }
                }
                break;

                case DT_SYNC_WITH_DVB_TDT_OR_TOT:
                {
                    /* Load TDT table and then TOT table. */
                    if ( x_dvb_si_load_tdt(h_src_root,
                                           (VOID*)NULL,
                                           dt_brdcst_tm_nfy,
                                           &h_dvb_tdt,
                                           &e_cond) == TMR_OK
                        )
                    {
                        DBG_INFO(("{DT}: load DVB TDT table successful.\n"));
                        if ( x_dvb_si_load_tot(h_src_root,
                                               (VOID*)NULL,
                                               dt_brdcst_tm_nfy,
                                               &h_dvb_tot,
                                               &e_cond) == TMR_OK
                            )
                        {
                            DBG_INFO(("{DT}: load DVB TOT table successful.\n"));
                            dt_tb_src_set_sel_order(t_sync_src_type);
                            i4_rc = DTR_OK;
                        }
                        else
                        {
                            DBG_ERROR(("{DT} Failed to load DVB TOT table.\n"));
                            i4_rc = DTR_FAIL;
                        }
                    }
                    else
                    {
                        DBG_ERROR(("{DT} Failed to load DVB TDT table.\n"));
                        i4_rc = DTR_FAIL;
                    }
                }
                break;

                case DT_SYNC_WITH_DVB_TOT_OR_TDT:
                {
                    /* Load TOT table and then TDT table. */
                    if ( x_dvb_si_load_tot(h_src_root,
                                           (VOID*)NULL,
                                           dt_brdcst_tm_nfy,
                                           &h_dvb_tot,
                                           &e_cond) == TMR_OK
                        )
                    {
                        DBG_INFO(("{DT}: load DVB TOT table successful.\n"));
                        if ( x_dvb_si_load_tdt(h_src_root,
                                               (VOID*)NULL,
                                               dt_brdcst_tm_nfy,
                                               &h_dvb_tdt,
                                               &e_cond) == TMR_OK
                            )
                        {
                            DBG_INFO(("{DT}: load DVB TDT table successful.\n"));
                            dt_tb_src_set_sel_order(t_sync_src_type);
                            i4_rc = DTR_OK;
                        }
                        else
                        {
                            DBG_ERROR(("{DT} Failed to load DVB TDT table.\n"));
                            i4_rc = DTR_FAIL;
                        }
                    }
                    else
                    {
                        DBG_ERROR(("{DT} Failed to load DVB TOT table.\n"));
                        i4_rc = DTR_FAIL;
                    }
                }
                break;

                case DT_SYNC_WITH_SCTE_STT:
                {
                    if ( x_atsc_si_load_stt(h_src_root,
                                            (VOID*)NULL,
                                            dt_brdcst_tm_nfy,
                                            &h_stt,
                                            &e_cond) == TMR_OK
                        )
                    {
                        DBG_INFO(("{DT}: load SCTE STT table successful.\n"));
                        dt_tb_src_set_sel_order(t_sync_src_type);
                        i4_rc = DTR_OK;
                    }
                    else
                    {
                        DBG_ERROR(("{DT} Failed to load SCTE STT table.\n"));
                        i4_rc = DTR_FAIL;
                    }
                }
                break;
                
                case DT_SYNC_WITH_ATSC_STT:
                {
                    if ( x_atsc_si_load_stt(h_src_root,
                                            (VOID*)NULL,
                                            dt_brdcst_tm_nfy,
                                            &h_stt,
                                            &e_cond) == TMR_OK
                        )
                    {
                        DBG_INFO(("{DT}: load ATSC STT table successful.\n"));
                        dt_tb_src_set_sel_order(t_sync_src_type);
                        i4_rc = DTR_OK;
                    }
                    else
                    {
                        DBG_ERROR(("{DT} Failed to load ATSC STT table.\n"));
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
            DBG_ERROR(("{DT} TM root notification function reg failed.\n"));
            i4_rc = DTR_FAIL;
        }
    }
    else
    {
        DBG_ERROR(("{DT} Failed to open TM root.\n"));
        i4_rc = DTR_FAIL;
    }

    dt_ungrab_sema_stt();

    return i4_rc;
}



/*-----------------------------------------------------------------------
 * Name: dt_unload_analog_brdcst_time_info
 *
 * Description:  This function frees the analog synchronization source
 *               handle(s) to the SI engine and set the DT condition to
 *               DT_NO_SYNC_SRC.
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
INT32 dt_unload_analog_brdcst_time_info
(
    VOID
)
{
    INT32       i4_rc = DTR_FAIL;
    
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
    dt_grab_sema_stt();
    
    /*
      Free the current analog source of the time table.
    */
    if ( pt_analog_brdcst_api != NULL )
    {
        i4_rc = pt_analog_brdcst_api->pf_close_analog_source(TRUE);
        dt_close_analog_tuner();
        _t_analog_sync_src_type = DT_SYNC_SRC_NONE ;
        _t_analog_src_type_desc = DT_SRC_TYPE_FORMAT_UNKNOWN ;
        x_memset(_s_analog_tuner_name,0,sizeof(_s_analog_tuner_name));
    }
    
    if ( (i4_rc == DTR_OK) || (i4_rc == DTR_WILL_NOTIFY) )
    {
        DBG_INFO(("{DT} analog brdcst source closed.\n"));

        if ( i4_rc == DTR_WILL_NOTIFY )
        {
            i4_rc = DTR_OK;
            /*
              SI handle source has been deleted, set the sync source
              condition and determine if it is necessary to notify
              client of the condition change..
            */   
            if ( dt_set_cond((INT32)DT_CLOCK_BRDCST_UTC, DT_NO_SYNC_SRC) ==
                 DTR_WILL_NOTIFY )
            {
                /*
                  Notify the client(s) that system clock is
                  now in the no sync source state.
                */
                DBG_INFO(("{DT} Notify clients with DT_NO_SYNC_SRC cond.\n"));
                dt_nfy_all_clients(DT_NO_SYNC_SRC, 0);
            }
        }
         
        /*
          If the user clock is linked with broadcast clock, then
          set the condition and for the user clock.
        */
        if ( dt_user_clock_sync_with_brdcst_clock() == TRUE )
        {
            dt_set_cond((INT32)DT_CLOCK_USER_UTC, DT_NO_SYNC_SRC);
        }
    }
    else
    {
        DBG_ERROR(("{DT} Error: can not delete analog sync source handle(s) to VBI engine.\n"));
    }
    
    dt_ungrab_sema_stt();
    
    return i4_rc;
}

/*-----------------------------------------------------------------------
 * Name: dt_load_analog_brdcst_time_info
 *
 * Description:  This function creates a handle to the table manager, and
 *               request loading of system time info tables (STT, TOT,
 *               TDT) from specific source engine. If an exist source 
 *               already in used, calling this API will change the source 
 *               of the time info.
 *
 *               If the 't_sync_src_type' is DT_SYNC_SRC_NONE, then
 *               this API free the existing table manager handle and
 *               handle to the source engine, and then return.
 
 *               A callback function is registered with the specific
 *               source engine to receive time table data.
 *
 *               Currently, only VBI filter engine is accepted.
 *               
 *               The callback function is called everytime when a 
 *               current time and date packet is received by the
 *               VBI filter engine.
 *
 * Inputs:
 *
 *   t_sync_src_type:  Specifies the types of synchronization source.
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
INT32 dt_load_analog_brdcst_time_info
(
    DT_SYNC_SRC_TYPE_T          t_sync_src_type,
    DT_SRC_TYPE_DESC_T          t_src_type_desc,
    const VOID*                 pv_src_info
)
{
    INT32                       i4_rc;
    
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
    dt_grab_sema_stt();

    /*
      Free current analog source.
    */
    if ( (pt_analog_brdcst_api != NULL) )
    {
        i4_rc = pt_analog_brdcst_api->pf_close_analog_source(TRUE);
        
        if ( (i4_rc == DTR_OK) || (i4_rc == DTR_WILL_NOTIFY) )
        {
            if ( i4_rc == DTR_WILL_NOTIFY )
            {
                /*
                  Analog Teletext source has been deleted, set the 
                  system clock condition to free running state.
                */
                DBG_INFO(("{DT} analog broadcast time source changed: => Set broadcast clock to DT_FREE_RUNNING.\n"));

                i4_rc = DTR_OK;
                
                if ( dt_set_cond((INT32)DT_CLOCK_BRDCST_UTC, DT_FREE_RUNNING) ==
                     DTR_WILL_NOTIFY )
                {
                    /*
                      Notify the client(s) that system clock is
                      now in the free running state.
                    */
                    DBG_INFO(("{DT} Notify clients with DT_FREE_RUNNING cond.\n"));
                    dt_nfy_all_clients(DT_FREE_RUNNING, 0);
                }

                dt_reset_last_stt_time();
            
            }
            
            /*
              If the user clock is linked with broadcast clock, then
              set the condition and for the user clock.
            */
            if ( dt_user_clock_sync_with_brdcst_clock() == TRUE )
            {
                dt_set_cond((INT32)DT_CLOCK_USER_UTC, DT_FREE_RUNNING);
            }
        }
        else
        {
            DBG_ERROR(("{DT} Error: can not delete analog sync source handle(s) to SI engine.\n"));
            i4_rc = DTR_FAIL;
            dt_ungrab_sema_stt();
            return i4_rc;
        }
    }
    
    if ( (pt_analog_brdcst_api != NULL) )
    {
        HANDLE_T      h_rm_analog_tuner = NULL_HANDLE;

        i4_rc = dt_open_analog_tuner(t_src_type_desc ,
                                     pv_src_info,
                                     &h_rm_analog_tuner);
        
        if ( (i4_rc == DTR_OK) || (i4_rc == DTR_WILL_NOTIFY) )
        {
            _t_analog_sync_src_type = t_sync_src_type;
            _t_analog_src_type_desc = t_src_type_desc;
            x_memset(_s_analog_tuner_name, 0, sizeof(_s_analog_tuner_name));
			if(pv_src_info != NULL)
			{
				x_strncpy(_s_analog_tuner_name,(CHAR*)pv_src_info,
                      	 (sizeof(_s_analog_tuner_name)-1));
			}
            if ( i4_rc == DTR_OK )
            {
                /*
                  Analog tuner is connected and active, therefore open a
                  VBIF filter and monitor teletext.
                */
                i4_rc = pt_analog_brdcst_api->pf_open_analog_source(
                    t_sync_src_type,
                    t_src_type_desc,
                    pv_src_info, h_rm_analog_tuner) ;

                if ( i4_rc != DTR_OK )
                {
                    _t_analog_sync_src_type = DT_SYNC_SRC_NONE ;
                    _t_analog_src_type_desc = DT_SRC_TYPE_FORMAT_UNKNOWN ;
                    x_memset(_s_analog_tuner_name,0,sizeof(_s_analog_tuner_name));
                }
            }
        }
    }
    else
    {
        DBG_ERROR(("{DT} Failed to open VBI filter for receiving teletext data.\n"));
        i4_rc = DTR_FAIL;
    }

    dt_ungrab_sema_stt();

    return i4_rc;
}


/*****************************************************************
 * Name: dt_reg_atsc_functions
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
INT32 dt_reg_atsc_functions(const DT_BRDCST_ATSC_API_T* pt_table)
{
    INT32   i4_rc = DTR_OK;
    
    pt_atsc_brdcst_api = pt_table;
    
    return i4_rc;
}

/*****************************************************************
 * Name: dt_reg_dvb_functions
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
INT32 dt_reg_dvb_functions(const DT_BRDCST_DVB_API_T* pt_table)
{
    INT32   i4_rc = DTR_OK;
    
    pt_dvb_brdcst_api = pt_table;

    /* register DVB function up one-level to the x_dt_* API's */
    dt_reg_dvb_functions_2(pt_table);
    
    return i4_rc;
}

/*****************************************************************
 * Name: dt_reg_teletext_functions
 *
 * Description:  This function registers teletext function table.
 *
 * Inputs:
 *   pt_table    Teletext function table processing API's.
 *
 * Outputs: -
 *   VOID
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
INT32 dt_reg_teletext_functions
(
    const DT_ANALOG_BRDCST_VBI_TELETEXT_API_T* pt_table
)
{
    INT32   i4_rc = DTR_OK;
    
    pt_analog_brdcst_api = pt_table;

    /* register VBI teletext function up one-level to the x_dt_* API's */
    dt_reg_teletext_functions_2(pt_table);
    
    return i4_rc;
}


