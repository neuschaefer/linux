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
 * $RCSfile: dt_teletext_api.c,v $
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

#include "vbi_fltr/x_vbi_fltr.h"
#include "res_mngr/rm.h"

#include "dbg/x_dbg.h"
#include "dt/_dt.h"
#include "dt/dt_dbg.h"
#include "dt/dt_brdcst/dt_brdcst_api.h"
#include "dt/dt_brdcst/dt_dvb/dt_dvb_lto.h"

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

static  UINT32      ui4_vbi_teltxt_config_flag = 0;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------
 * Name: dt_get_analog_tuner
 *
 * Description:
 *   This function get the analog tuner component for the 
 *   current connection pipe. 
 *
 * Inputs:
 *   
 *   h_rm_analog_tuner   Handle to the RM analog tuner.
 *
 * Outputs:
 *   ph_obj:   handle to an instance of VBI filter.
 *
 * Returns:
 *   DTR_OK:      Handle to the analog source is closed.
 *                
 *   DTR_FAIL     Can not free the analog source handle to VBI filter.
 *                
 -------------------------------------------------------------------------*/
static INT32 dt_get_analog_tuner
(
    HANDLE_T           h_analog_tuner,
    HANDLE_T*          ph_pipe
)
{
    INT32              i4_rc = DTR_FAIL;
    SIZE_T             z_size;
    RM_COND_T          e_cond;
    
    z_size = sizeof(HANDLE_T);

    DBG_INFO(("{DT} Get analog tuner source component\n"));
    i4_rc=rm_get(h_analog_tuner,
                 FALSE,
                 ANY_PORT_NUM,
                 (RM_GET_TYPE_NFY_COMP_CONN_ORIG | RM_GET_TYPE_CONN_ORIG_HANDLE),
                 (VOID *) ph_pipe,
                 (SIZE_T *)&z_size,
                 &e_cond);
    
    if ( i4_rc == RMR_OK )
    {
        DBG_INFO(("{DT} Analog tuner source component [%d].\n", *ph_pipe));
        i4_rc=DTR_OK;
    }
    else
    {
        DBG_ERROR(("{DT} Error [%d] RM_GET on analog tuner failed.\n",i4_rc));
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------
 * Name: dt_analog_brdcst_nfy_fct
 *
 * Description:
 *   This function is called by the VBI filter engine when a
 *   current time/date packet is received.  The prototype of this API
 *   is specified by (*x_vbif_nfy_fct).
 *
 *   This function calls the respective parse function to obtain
 *   the current time and day-light-saving information. It sets
 *   the broadcast clock and/or system clock with the data received
 *   from the broadcast.
 *
 * Inputs:
 *   h_obj       handle to the VBI filter
 *
 *   e_nfy_cond  contains the state (condition) of the VBI filter..
 *
 *   pv_tag      contains the notification tag.
 *
 *   ui4_data    contains the address to the Teletext data object.
 *
 * Outputs: 
 *   None.
 *
 * Returns:
 *   NFY_RET_PROCESSED:  table section has been processed.
 *
 -------------------------------------------------------------------------*/
static VOID dt_analog_brdcst_nfy_fct
(
    HANDLE_T                    h_filter,
    VBIF_COND_T                 e_cond,
    VBIF_NFY_REASON_T           e_reason,
    VOID*                       pv_tag,
    UINT32                      ui4_data
)
{
    UINT8                       aui1_pkt_data[7];
    TIME_T                      t_utc;
    BOOL                        b_accept;
    
    switch (e_reason)
    {
        case NFY_REASON_DATA_AVAIL:
        {
            UINT32               ui4_mjd;  /* for modified julian date. */
            STT_DATA_T           t_stt;
            DTG_T                t_dtg;
            FLTR_NFY_NI_T*       pt_nfy_ni = NULL;
            INT32                i4_offset_value = 0;
        
            /* copy the VBI current time and date data.
               Format for the encoding of Teletext packet 8/30, per
               ETSI EN 300 706  byte 15 to byte 21.

               ui4_data is adddress to FLTR_NFY_NI_T data structure
               containing FLTR_NI_NFY_TTX_DATA_TIME_T structure.

               Teletext Date Time data: [ 0 16 55 52 24 6A 18 ]
            */
            pt_nfy_ni=(FLTR_NFY_NI_T*) ui4_data ;
            x_memcpy(aui1_pkt_data, pt_nfy_ni->u.t_ttx_date_time.aui1_data, 7);

            /* 
               Byte 0 (e.g., byte 15)  2:6  local time offset
               bit 2:  0.5 hrs.
               bit 3:  1.0 hrs.
               bit 4:  2.0 hrs.
               bit 5:  4.0 hrs.
               bit 5:  8.0 hrs.
                                        
               7    0: positive
               1: negative
            */
            
            if ( ((aui1_pkt_data[0] & 0x02) >> 1 ) == 1 )
            {
                i4_offset_value += 1800 ;  /* 0.5 hrs.  */
            }

            if ( ((aui1_pkt_data[0] & 0x04) >> 2 ) == 1 )
            {
                i4_offset_value += 3600 ;  /* 1 hrs.  */
            }

            if ( ((aui1_pkt_data[0] & 0x08) >> 3 ) == 1 )
            {
                i4_offset_value += 7200 ;  /* 2 hrs.  */
            }

            if ( ((aui1_pkt_data[0] & 0x10) >> 4 ) == 1 )
            {
                i4_offset_value += 14400 ;  /* 4 hrs.  */
            }

            if ( ((aui1_pkt_data[0] & 0x20) >> 5 ) == 1 )
            {
                i4_offset_value += 28800 ;  /* 8 hrs.  */
            }
            
            if ( ((aui1_pkt_data[0] & 0x40) >> 6) != 0 )
            {
                i4_offset_value = i4_offset_value * (-1) ;
            }
                
            /* Next 3 bytes contains Modified Julian Day.  */
            ui4_mjd = (UINT32) (
                ( (aui1_pkt_data[1]  & 0x0F)       - 1 )  * 10000  +
                ( ((aui1_pkt_data[2] & 0xF0) >> 4) - 1 )  * 1000   +
                ( (aui1_pkt_data[2]  & 0x0F)       - 1 )  * 100    +
                ( ((aui1_pkt_data[3] & 0xF0) >> 4) - 1 )  * 10     +
                ( (aui1_pkt_data[3]  & 0x0F)       - 1 ) );

            x_dt_mjd_to_dtg(ui4_mjd, &t_dtg);

            /* Next 3 bytes contains HH, MM, SEC values. */
            t_dtg.ui1_hr = (UINT8) (
                ( ((aui1_pkt_data[4] & 0xF0) >> 4) - 1 ) * 10  +
                ( (aui1_pkt_data[4] & 0x0F)        - 1 ) );

            t_dtg.ui1_min = (UINT8) (
                ( ((aui1_pkt_data[5] & 0xF0) >> 4) - 1 ) * 10  +
                ( (aui1_pkt_data[5] & 0x0F)        - 1 ) );

            t_dtg.ui1_sec = (UINT8) (
                ( ((aui1_pkt_data[6] & 0xF0) >> 4) - 1 ) * 10  +
                ( (aui1_pkt_data[6] & 0x0F)        - 1 ) );
            
            /* convert the DTG_T to seconds. */
            t_stt.t_sec  = x_dt_dtg_to_sec(&t_dtg) ;

            
            DBG_INFO(("{DT} Recv UTC data from analog teletext stream: [%lld] sec.\n", t_stt.t_sec));
            DBG_INFO(("{DT} Teletext time (YYYYMMDDHHSS): %d %d %d %d %d %d \n",t_dtg.ui2_yr,t_dtg.ui1_mo,t_dtg.ui1_day,t_dtg.ui1_hr,t_dtg.ui1_min,t_dtg.ui1_sec));
            
            /* Now we have the UTC value received from the teletxt
               stream, we will check if this teletext UTC data packet
               contain reasonable (e.g., not-corrupted) data.
            */
            t_utc=x_dt_get_utc(NULL,NULL);

            /* check time. */
            b_accept = dt_check_input_utc_time(t_stt.t_sec,t_utc);
            
            if ( b_accept == TRUE )
            {
                DBG_INFO(("{DT} Teletext UTC time pass gross error check, it is accepted.\n"));
                
                t_stt.b_ds_desc = FALSE;

                t_stt.ui1_gps_utc_offset = 0;

                t_stt.ui1_src_type = VBI_TELETEXT_TBL_ID;

                t_stt.b_tz_offset_changed = FALSE;

                if ( ui4_vbi_teltxt_config_flag & DT_ANALOG_USE_TELETEXT_LOCAL_TIME_OFFSET )
                {
                    
                    /* since we only have the current offset of the
                       local time from GMT, we have to see if we need to
                       adjust for the current Day-Light-Saving

                       The reason is that the current offset consist of
                       timezone offset + Day-Light-saving offset:

                       local_offset = TZ_offset + DST_offset

                       From local_offset, we determine the TZ_offset.

                       TZ_offset = local_offset - DST_offset
                    */
                
                    if ( x_dt_get_tz_ctrl() == TRUE )
                    {
                        TIME_T   curr_tz_offset =   0  ;
                        
                        if ( x_dt_get_dst() == TRUE )
                        {
                            i4_offset_value = (INT32) ( i4_offset_value -
                                                        dt_get_dst_offset() ) ;
                        }

                        curr_tz_offset = x_dt_get_tz();

                        /* determine if the timezone offset has changed. */
                        if ( curr_tz_offset !=  (TIME_T) i4_offset_value )
                        {
                            x_dt_set_tz((TIME_T)i4_offset_value);
                            t_stt.b_tz_offset_changed = TRUE;
                        }
                    }
                }

                
                dt_sync_with_stt(&t_stt);
            }
        }
        break;

        case NFY_REASON_ERROR:
        {
            DBG_ERROR(("{DT} VBI nfy [%d] NFY_REASON_ERROR\n", e_reason));
        }
        break;

        default:
        {
            DBG_ERROR(("{DT} VBI nfy reason [%d]\n",e_reason));
        }
        break;
    }
    return ;
}

/*
  Handle to the VBI filter engine.
*/
static HANDLE_T     h_vbif_filter = NULL_HANDLE ;

/*-----------------------------------------------------------------------
 * Name: dt_open_analog_source
 *
 * Description:
 *   This function open a handle to the VBI filter engine and
 *   register a callback function to process VBI teletext data
 *   when it is received.
 *
 * Inputs:
 *   
 *   h_rm_analog_tuner   Handle to the RM analog tuner.
 *
 * Outputs:
 *   ph_obj:   handle to an instance of VBI filter.
 *
 * Returns:
 *   DTR_OK:      Handle to the analog source is closed.
 *                
 *   DTR_FAIL     Can not free the analog source handle to VBI filter.
 *                
 -------------------------------------------------------------------------*/
static INT32 dt_open_analog_source
(
    DT_SYNC_SRC_TYPE_T t_sync_src_type,
    DT_SRC_TYPE_DESC_T t_src_type_desc,
    const VOID*        pv_src_info,
    HANDLE_T           h_rm_analog_tuner
)
{
    INT32          i4_rc = DTR_OK;
    VBIF_COND_T    e_cond;
    
    if ( h_vbif_filter == NULL_HANDLE )
    {
        HANDLE_T            h_tuner     = NULL_HANDLE;
        VBIF_FLTR_TYPE_T    e_fltr_type = FLTR_TYPE_NI ;
        FLTR_INIT_NI_T      t_fltr_info;
        VBIF_SRC_TYPE_T     e_src_type = SRC_TYPE_ANA_VBI;
        VBIF_SRC_ANA_VBI_T  t_src_info ;
        VBIF_FLTR_MODE_T    e_fltr_mode = FLTR_MODE_PUMP;
        
        t_fltr_info.e_src_mask = FLTR_NI_SRC_TYPE_TTX_DATE_TIME ;

        if ( dt_get_analog_tuner(h_rm_analog_tuner, &h_tuner) == DTR_OK )
        {
            t_src_info.b_sel_src_or_conn_handle = TRUE;
            t_src_info.h_ref = h_tuner;

            DBG_INFO(("{DT} open VBI filter with analog tuner [%d]\n",h_tuner));

            i4_rc = x_vbif_open(e_fltr_type,
                                &t_fltr_info,
                                e_src_type,
                                &t_src_info,
                                e_fltr_mode,
                                (const VOID*) t_src_type_desc,
                                dt_analog_brdcst_nfy_fct,
                                &h_vbif_filter,
                                NULL) ;
            if ( i4_rc == VBIFR_OK )
            {    
                DBG_INFO(("{DT} VBI Filter [%d] created.\n", h_vbif_filter));

                i4_rc=x_vbif_start(h_vbif_filter, &e_cond);
            
                if ( i4_rc == VBIFR_OK)
                {
                    DBG_INFO(("{DT} VBI filter [d%] started\n", h_vbif_filter));
                    i4_rc = DTR_OK;
                }
                else
                {
                    DBG_ERROR(("{DT} Error [%d]: VBI filter not started\n",i4_rc));
                    i4_rc = DTR_FAIL;
                }
            }
            else
            {
                DBG_ERROR(("{DT} Error [%d]: can not create VBI Filter.\n",i4_rc));
                h_vbif_filter = NULL_HANDLE;
                i4_rc = DTR_FAIL;
            }
        }
    }

    /* Reset the check engine. */
    dt_reset_check_input_time();
    
    return i4_rc;
}

/*-----------------------------------------------------------------------
 * Name: dt_close_analog_source
 *
 * Description:
 *   This function frees the analog source handle for the current time
 *   and date. If VBI filter was NULL or has already been closed,
 *   then this function just return OK.  If the VBIF filter is
 *   closed, then return DTR_WILL_NOTIFY, so the caller can notify
 *   upper level that the broadcast clock is in FREE_RUNNING state.
 *
 * Inputs:
 *   b_is_tuner_connected:
 *      Flag to indicate if the tuner source for the VBI
 *      filter is connected. When the analog tuner is disconnected,
 *      the VBI filter is automatically closed, so we don't
 *      dont need to call x_vbif_close().
 *
 * Outputs:
 *   None.
 *
 * Returns:
 *   DTR_OK:      Handle to the analog source was already closed or NULL.
 *
 *   DTR_WILL_NOTIFY:  Handle to the analog source is closed.  Return
 *                     WILL_NOTIFY status so the caller can notify clients
 *                     of DT library a change in broadcast clock condition.
 *
 *   DTR_FAIL     Can not free the analog source handle to VBI filter.
 *                
 -------------------------------------------------------------------------*/
static INT32 dt_close_analog_source(BOOL b_is_tuner_connected)
{
    INT32          i4_rc = DTR_OK;

    if ( h_vbif_filter != NULL_HANDLE )
    {
        if ( b_is_tuner_connected )
        {
            i4_rc = x_vbif_close(h_vbif_filter, NULL);
            if ( i4_rc != VBIFR_OK )
            {
                DBG_ERROR(("{DT} Error [%d], VBIF close failed.\n",i4_rc));
                i4_rc = DTR_FAIL;
            }
        }
        
        h_vbif_filter = NULL_HANDLE;
        i4_rc = DTR_WILL_NOTIFY;
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------
 * Name: dt_vbi_teletext_set_config
 *
 * Description:
 *   This function sets the internal configuration for processing
 *   VBI teletext table.
 *
 *   Currently, the only option supported is DT_ANALOG_USE_TELETEXT_LOCAL_TIME_OFFSET.
 *   When this value is set, then the local time adjustment will use the
 *   the time offset specified in the VBI teletext table.  Otherwise, the the local
 *   time offset will be set by the Digital broadcast table (e.g., DOT's country code
 *   information) and the user specified country code.
 *
 * Inputs:
 *   ui4_flag       Specifies the config flag for processing VBI table.
 *
 * Outputs:
 *   None.
 *
 * Returns:
 *   DTR_OK:      configuration flag is set.
 *
 *   DTR_FAIL     Invalid configuration flag.
 *                
 -------------------------------------------------------------------------*/
static INT32 dt_vbi_teletext_set_config(UINT32 ui4_flag)
{
    if ( ui4_flag & DT_ANALOG_USE_TELETEXT_LOCAL_TIME_OFFSET )
    {
        ui4_vbi_teltxt_config_flag =
            ui4_vbi_teltxt_config_flag | DT_ANALOG_USE_TELETEXT_LOCAL_TIME_OFFSET ;
    }

    if ( ui4_flag & DT_ANALOG_UNSET_CONFIG_SETTING )
    {
        ui4_vbi_teltxt_config_flag = 0;
    }
    
    return DTR_OK;
}

/*-----------------------------------------------------------------------
 * Name: x_dt_reg_teletext
 *
 * Description:  This function registers teletext table processing function
 *               to the DT library.
 *               
 * Inputs:
 *   None.
 *
 * Outputs: 
 *   None.
 *
 * Returns:
 *   DTR_OK:         function table has been registered.
 *   DTR_WILL_NOTIFY table section has been process and we need to
 *                   notify the upper layer that system clock has
 *                   changed into sync running state.
 *   DTR_FAIL        SST table rejected.
 -------------------------------------------------------------------------*/
INT32 x_dt_reg_teletext(VOID)
{
    static const DT_ANALOG_BRDCST_VBI_TELETEXT_API_T t_table =
        {
            dt_open_analog_source,
            dt_close_analog_source,
            dt_vbi_teletext_set_config
        };

    static const DT_ANALOG_BRDCST_API_T     t_brdcst_table =
        {
            dt_load_analog_brdcst_time_info,
            dt_unload_analog_brdcst_time_info,
            dt_analog_tuner_nfy_handler
        };

    /*
      Initialize system time source synchronization mechanism.
    */
    dt_init_sync_src();

    x_dt_check_input_time(TRUE);
    
    dt_reg_analog_brdcst_functions(&t_brdcst_table);
    
    return dt_reg_teletext_functions(&t_table); 
}



