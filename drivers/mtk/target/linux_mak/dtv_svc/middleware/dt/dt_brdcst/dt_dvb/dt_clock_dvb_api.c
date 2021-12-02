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
 * $RCSfile: dt_dvb_api.c,v $
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
#include "dbg/x_dbg.h"
#include "dt/_dt.h"
#include "dt/dt_dbg.h"
#include "dt/dt_brdcst/dt_brdcst_api.h"
#include "dt/dt_brdcst/dt_dvb/dt_dvb_lto.h"

#include "dt/dt_clock.h"

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------
 * Name: dt_clock_parse_dvb_tdt_tot
 *
 * Description:  This function prases the DVB time info to get the
 *               current time and Day-Light Saving information.
 *
 *               DVB TDT section data:
 *
 *                    table_id                       8  uimsbf
 *                    section_syntax_indicator       1  bslbf
 *                    reserved for future            1  bslbf
 *                    reserved                       2  bslbf
 *                    section_length                 12 uimsbf
 *                    UTC_time                       40 bslbf
 *
 *               DVB TOT section data:
 *
 *                    table_id                       8  uimsbf
 *                    section_syntax_indicator       1  bslbf
 *                    reserved for future            1  bslbf
 *                    reserved                       2  bslbf
 *                    section_length                 12 uimsbf
 *                    UTC_time                       40 bslbf
 *                    reserved                       4  bslbf
 *                    descriptors_loop_len           12 bslbf
 *                    for (0:descriptors_loop_len )
 *                    {
 *                       descriptor()
 *                    }
 *                    CRC_32                         32 rpchof
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
 *   pui1_data    contains the address to the STT object.
 *
 * Outputs: -
 *   None.
 *
 * Returns:
 *   DTR_OK:         table section has been processed.
 *   DTR_WILL_NOTIFY table section has been process and we need to
 *                   notify the upper layer that system clock has
 *                   changed into sync running state.
 *   DTR_FAIL        SST table rejected.
 -------------------------------------------------------------------------*/
static INT32 dt_clock_parse_dvb_tdt_tot
(
    HANDLE_T       h_obj,
    HANDLE_TYPE_T  e_obj_type,
    VOID*          pv_nfy_tag,
    const UINT8*   pui1_data
)
{
    UINT8      ui1_tbl_id;
    UINT16     ui2_sect_len;
    UINT16     ui2_mjd;       /* for modified julian date. */
    STT_DATA_T t_stt;
    DTG_T      t_dtg;
    INT32      i4_rc;
    DVB_TOT_T  t_tot;
    BOOL       b_tot_rc;
    DT_CLOCK_CTRL_T*  pt_co;

    i4_rc = DTR_FAIL;

    /* clock control structure. */
    pt_co = (DT_CLOCK_CTRL_T*) pv_nfy_tag;
    
    /*
      Flag to check if the received table contains day-light-saving descriptor
      data.
    */
    t_stt.b_ds_desc = FALSE;

    /* Translate the raw header data into the section header structure */
    ui1_tbl_id = pui1_data[0];

    /* Get the section length of TDT data. */
    ui2_sect_len = GET_UINT16_FROM_PTR_BIG_END(&(pui1_data [1]))
                   & 0x0FFF;

    if ( ((ui1_tbl_id == DVB_TDT_TBL_ID) && (ui2_sect_len ==  5)) ||
         ((ui1_tbl_id == DVB_TOT_TBL_ID) && (ui2_sect_len >=  7)  )
        )
    {
        /* Get the MJD: byte 3 to byte 4 */
        ui2_mjd = GET_UINT16_FROM_PTR_BIG_END( &(pui1_data[3]) );

        /* Get the HR value, byte 5 */
        t_dtg.ui1_hr = (UINT8)BCD_2_DECIMAL(pui1_data[5]);

        /* Get the minute value, byte 6 */
        t_dtg.ui1_min = (UINT8)BCD_2_DECIMAL(pui1_data[6]);

        t_dtg.ui1_sec = (UINT8)BCD_2_DECIMAL(pui1_data[7]);

        /* Fill in the rest of DTG value from MJD */
        dt_mjd_to_dtg(ui2_mjd, &t_dtg);

        /*
          Convert DTG to UTC seconds.
        */
        t_stt.t_sec  = x_dt_dtg_to_sec(&t_dtg) ;
        t_stt.ui1_gps_utc_offset = 0;   /* In DVB, GPS-UTC offset
                                           not specified, so just
                                           use '0'.
                                        */
        
        t_stt.b_tz_offset_changed = FALSE;
        
        b_tot_rc = FALSE;
        t_tot.t_this_utc = t_stt.t_sec;
            
        if ( ui1_tbl_id == DVB_TOT_TBL_ID )
        {
            /* Process the 'local_time_offset' descriptors. */
            b_tot_rc = dt_dvb_lto_prc_lto_descrs (
                &(pui1_data [10]),
                (GET_UINT16_FROM_PTR_BIG_END (&(pui1_data [8])) & 0x0fff),
                &t_tot);
            
            if ( b_tot_rc == TRUE )
            {
                /* convert TOT data into internal data form for processing. */
                t_stt.b_ds_desc    = TRUE;
                t_stt.b_ds_status  = t_tot.b_dst;
                t_stt.t_ds_change  = t_tot.t_tm_of_change;
                t_stt.t_ds_offset  = t_tot.t_offset_for_dst;

                /*
                  Currently, for the custom clock we don't
                  Process timezone off-set data from the TOT table.
                */
            }
        }

        /*
          Sync the system time with the TDT/TOT.
        */
        t_stt.ui1_src_type = ui1_tbl_id;
        i4_rc=dt_clock_sync_with_stt(pt_co, &t_stt);
    }
    return (i4_rc);
}

/*-----------------------------------------------------------------------
 * Name: x_dt_reg_dvb
 *
 * Description:  This function registers DVB table processing function
 *               to the DT library.
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
 *   pui1_data    contains the address to the STT object.
 *
 * Outputs: -
 *   None.
 *
 * Returns:
 *   DTR_OK:         table section has been processed.
 *   DTR_WILL_NOTIFY table section has been process and we need to
 *                   notify the upper layer that system clock has
 *                   changed into sync running state.
 *   DTR_FAIL        SST table rejected.
 -------------------------------------------------------------------------*/
INT32 x_dt_clock_reg_dvb(VOID)
{
    static const DT_BRDCST_DVB_API_T  t_table =
        {
            dt_clock_parse_dvb_tdt_tot,
            dt_dvb_lto_set_sys_count_code,
            dt_dvb_lto_get_sys_count_code,
            dt_dvb_lto_get_lto,
            dt_dvb_lto_get_num_lto_entries,
            dt_dvb_set_config
        };

    static const DT_CLOCK_BRDCST_API_T     t_brdcst_table =
        {
            dt_clock_load_brdcst_time_info,
            dt_clock_unload_brdcst_time_info
        };
    
    dt_clock_reg_brdcst_functions(DT_DVB_API, &t_brdcst_table);
    
    return dt_clock_reg_dvb_functions(&t_table);
}



