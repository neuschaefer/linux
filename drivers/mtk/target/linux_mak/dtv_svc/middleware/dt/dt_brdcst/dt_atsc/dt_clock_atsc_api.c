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
 * $RCSfile: dt_atsc_api.c,v $
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

#include "dt/dt_clock.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------
 * Name: dt_clock_parse_ib_stt
 *
 * Description:  This function prases the in-band STT to get the
 *               current time and Day-Light Saving information.
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
static INT32 dt_clock_parse_ib_stt
(
    HANDLE_T          h_obj,
    HANDLE_TYPE_T     e_obj_type,
    VOID*             pv_nfy_tag,
    const UINT8*      pui1_data
)
{
    TIME_T            t_gps_sec;
    SECT_HDR_T        t_sect_hdr;
    const UINT8*      aui1_stt_data;
    STT_DATA_T        t_stt;
    INT32             i4_rc;
    DT_CLOCK_CTRL_T*  pt_co;

    /* clock control structure. */
    pt_co = (DT_CLOCK_CTRL_T*) pv_nfy_tag;

    /* Translate the raw header data into the section header structure */
    tm_xlat_sect_hdr((UINT8*)pui1_data, &t_sect_hdr);

    /* Set the pointer to the start of the protocol_version in the
       STT table.
    */
    aui1_stt_data=(UINT8*)(&(pui1_data[STT_IN_BAND_HEADER_LEN]));

    i4_rc = DTR_FAIL;

    if (t_sect_hdr.ui1_tbl_id == STT_IN_BAND_TBL_ID &&
        aui1_stt_data[0] == 0 /* protocol num is byte 0 */ )
    {
        /* Get the GPS seconds. byte 1 to byte 4 */
        t_gps_sec = (TIME_T) GET_UINT32_FROM_PTR_BIG_END( &(aui1_stt_data[1]) );

        /* Get the GPS UTC offset, byte 5 */
        t_stt.ui1_gps_utc_offset = aui1_stt_data[5];


        /* Get the Day-Light Saving statue, most significant bit of byte 6. */
        t_stt.b_ds_status =       ( (aui1_stt_data[6] & 0x80) != 0 ) ;

        /* Get the day_of_month, the last 5 significant bits of byte 6. */
        t_stt.ui1_ds_day_of_mon = ( (aui1_stt_data[6] & 0x1F) ) ;

        /* Get the hour of transition, byte 7. */
        t_stt.ui1_ds_hr = aui1_stt_data[7];

        /*
           This STT contains day-light-saving data.
        */
        t_stt.b_ds_desc = TRUE;
        
        /*
           Save the current offset second between GPS and UTC time
           scale.
        */
        dt_set_gps_utc_offset(t_stt.ui1_gps_utc_offset);

        /*
           Convert the STT GPS time to GMT time (second since
           Jan 01, 1970), and then set the system time.
        */
        t_stt.t_sec = x_dt_gps_sec_to_utc_sec(t_gps_sec);

        /*
          Sync the system time with the STT.
        */
        t_stt.ui1_src_type = t_sect_hdr.ui1_tbl_id;

        /* For ATSC, we currently, we don't support notification 
           to client when timezone offset is changed. */
        t_stt.b_tz_offset_changed = FALSE;
        
        i4_rc=dt_clock_sync_with_stt(pt_co, &t_stt);
    }
    return (i4_rc);
}


/*-----------------------------------------------------------------------
 * Name: dt_clock_parse_oob_stt
 *
 * Description:  This function prases the out-of-band STT to get the
 *               current time and Day-Light Saving information.
 *
 *               Out-of-band STT:
 *
 *               table_id            8
 *               zero                2
 *               reserved            2
 *               section_length     12
 *               zero                3
 *               protocol version    5
 *               zero                8
 *               system_time        32
 *               GPS_UTC_offset      8
 *               day-light-saving
 *               {
 *                   descriptor_tag     8
 *                   descriptor_len     8
 *                   DS_status          1
 *                   reserved           2
 *                   DS_day_of_month    5
 *                   DS_hour            8
 *               }
 *               CRC                 4
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
 *   pui1_data   contains the address to the first byte of the STT
 *               section data.
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
static INT32 dt_clock_parse_oob_stt
(
    HANDLE_T       h_obj,
    HANDLE_TYPE_T  e_obj_type,
    VOID*          pv_nfy_tag,
    const UINT8*   pui1_data
)
{
    const UINT8*   aui1_stt_data;
    UINT8          ui1_tbl_id;
    UINT16         ui2_sect_len;
    UINT8          ui1_protocol_ver;
    UINT8          ui1_dsave_desc_tag;
    UINT8          ui1_desc_len;
    UINT32         ui4_rd_ptr;
    TIME_T         t_gps_sec;
    STT_DATA_T     t_stt;
    INT32          i4_rc;

    DT_CLOCK_CTRL_T*  pt_co;

    /* clock control structure. */
    pt_co = (DT_CLOCK_CTRL_T*) pv_nfy_tag;

    i4_rc = DTR_FAIL;

    /*
      Flag to check if the OOB STT contain day-light-saving descriptor
      data.
    */
    t_stt.b_ds_desc = FALSE;

    /* Translate the raw header data into the section header structure */
    ui1_tbl_id = pui1_data[0];

    /* Get the section length of OOB STT. */
    ui2_sect_len = GET_UINT16_FROM_PTR_BIG_END(&(pui1_data [1]))
                   & 0x0FFF;

    /* Save the pointer to the beginning of the byte after
       the section_length.
    */
    aui1_stt_data=(UINT8*)(&(pui1_data[STT_OUT_OF_BAND_HEADER_LEN]));

    /* Get protocol version. */
    ui1_protocol_ver = aui1_stt_data[0] & 0x1F;

    if ( ui1_tbl_id        == STT_OUT_OF_BAND_TBL_ID   &&
         ui1_protocol_ver  == 0 /* protocol num is byte 0 */ )
    {
        DBG_INFO(("{DT} clock [%s] Processing Out-of-band STT.\n", pt_co->pt_clock->s_name));

        /* Get the GPS seconds. byte 2 to byte 6 */
        t_gps_sec = (TIME_T) GET_UINT32_FROM_PTR_BIG_END(&(aui1_stt_data[2]) );

        /* Get the GPS UTC offset, byte 6 */
        t_stt.ui1_gps_utc_offset = aui1_stt_data[6];

        /* byte location for the start of the first descriptor block. */
        ui4_rd_ptr = 7 ;

        /* check to ensure we are reading data within the section bounadary.

           We subtract off from the section len by 4 bytes (for CRC32).
           If the number of bytes left over is at least 2 bytes, then
           the processing of descriptor will contiune.

           Note: The reason for this logic being that a descriptor
           block must contain a minimum of 2 bytes (1 byte for TAG,
           and second byte for the length of the descriptor block.
        */
        while (  ((UINT32)(ui4_rd_ptr+2)) < ((UINT32)(ui2_sect_len-4)) )
        {

            /* Check if day-light-saving descriptor exist,
               if yes, then process optional descriptor.
            */
            ui1_dsave_desc_tag = aui1_stt_data[ui4_rd_ptr];
            ui4_rd_ptr += 1 ;

            ui1_desc_len = aui1_stt_data[ui4_rd_ptr];
            ui4_rd_ptr += 1;

            /*
              process the Day-light-saving descriptor
            */
            if ( ui1_dsave_desc_tag == 0x96 )
            {
                /* Get the Day-Light Saving statue, most significant bit
                   of byte 12. */
                t_stt.b_ds_status =    ( (aui1_stt_data[ui4_rd_ptr] & 0x80)
                                        != 0 ) ;

                /* Get the day_of_month, the last 5 significant bits of
                    byte 12. */
                t_stt.ui1_ds_day_of_mon = ( (aui1_stt_data[ui4_rd_ptr]
                                             & 0x1F) ) ;

                ui4_rd_ptr += 1 ;

                /* Get the hour of transition, byte 13. */
                t_stt.ui1_ds_hr = aui1_stt_data[ui4_rd_ptr];
                ui4_rd_ptr += 1 ;

                /* error check on the DS transition hours. This value
                   should be between 0 and 18. If the received value is
                   not valid, we will default to 2 (e.g., 2AM).
                */
                if ( t_stt.ui1_ds_hr > 18 )
                {
                    t_stt.ui1_ds_hr = 2;
                }

                /*
                  This STT contains day-light-saving descriptor.
                */
                t_stt.b_ds_desc = TRUE;

                break;
            }
            else
            {
                /* This descriptor is not Day Light Saving descriptor,
                   skip the following bytes and goto next descriptor.
                */
                ui4_rd_ptr += ui1_desc_len ;
            }
        }

        /*
           Save the current offset second between GPS and UTC time
           scale.
        */
        dt_set_gps_utc_offset(t_stt.ui1_gps_utc_offset);

        /*
           Convert the STT GPS time to GMT time (second since
           Jan 01, 1970), and then set the system time.
        */
        t_stt.t_sec = x_dt_gps_sec_to_utc_sec(t_gps_sec);
        
        /*
          Sync the system time with the STT.
        */
        t_stt.ui1_src_type = ui1_tbl_id;

        /* For ATSC, we currently, we don't support notification 
           to client when timezone offset is changed. */
        t_stt.b_tz_offset_changed = FALSE;
        
        i4_rc=dt_clock_sync_with_stt(pt_co, &t_stt);
    }
    return (i4_rc);
}

/*-----------------------------------------------------------------------
 * Name: x_dt_clock_reg_atsc
 *
 * Description:  This function register function table for parsing ATSC
 *   table.
 *               
 * Inputs:
 *   None.
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
INT32 x_dt_clock_reg_atsc(VOID)
{
    static const DT_BRDCST_ATSC_API_T  t_table =
        {
            dt_clock_parse_ib_stt,
            dt_clock_parse_oob_stt
        };
    
    static const DT_CLOCK_BRDCST_API_T     t_brdcst_table =
        {
            dt_clock_load_brdcst_time_info,
            dt_clock_unload_brdcst_time_info
        };
    
    dt_clock_reg_brdcst_functions(DT_ATSC_API, &t_brdcst_table);
    
    return dt_clock_reg_atsc_functions(&t_table);
}

