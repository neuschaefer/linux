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
 * $RCSfile: svl_dbg.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains functions and internal variables to support
 *         CLI control of the DT library.
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "dbg/def_dbg_level_mw.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"

#include "svl/x_svl.h"
#include "svl_bldr/u_sb.h"
#include "os/inc/x_os.h"
#include "libc/ctype.h"
#include "dt/x_dt.h"


/*
  internal variable for the debug level.
*/
static UINT16  ui2_dbg_level = DBG_INIT_LEVEL_MW_SVL;

/*----------------------------------------------
  Public Function for DBG support.
------------------------------------------------*/
UINT16 svl_get_dbg_level(VOID)
{
    return ui2_dbg_level;
}

VOID   svl_set_dbg_level(UINT16  ui2_dbg)
{
    ui2_dbg_level = ui2_dbg;
}

static CHAR* svl_service_type_to_string(SVL_SERVICE_TYPE_T  t_servc)
{
    CHAR*   s_txt = " " ;
    
    switch (t_servc)
    {
        case SVL_SERVICE_TYPE_UNKNOWN:
        {
            s_txt = "SVL_SERVICE_TYPE_UNKNOWN";
        }
        break;

        case SVL_SERVICE_TYPE_TV:
        {
            s_txt = "SVL_SERVICE_TYPE_TV";
        }
        break;

        case SVL_SERVICE_TYPE_RADIO:
        {
            s_txt = "SVL_SERVICE_TYPE_RADIO";
        }
        break;

        case SVL_SERVICE_TYPE_APP:
        {
            s_txt = "SVL_SERVICE_TYPE_APP";
        }
        break;
    }
    
    return s_txt;
}

/*-------------------------------------------------------------------------
 * Name: svl_print _rec
 *
 * Description: This API prints out SVL record entries for the
 *    specified SVL ID.
 *
 * Inputs:  
 *    ui2_svl_id:     SVL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   svl_print_rec(SVL_REC_T*  pt_svl_rec, UINT16  ui2_idx)
{
    x_dbg_stmt("[%d] "
               "svl_rec id: %d "
               "channel: %d "
               "mask: %x "
               "opt mask: %x "
               "prgm id: %d "
               "TSL id: %d "
               "TSL rec id: %d "
               "NWL id: %d "
               "NWL rec id: %d "
               "SATL id: %d "
               "SATL rec id: %d "
               "servc type: %s "
               "name: [%s] ",
               ui2_idx,
               pt_svl_rec->ui2_svl_rec_id,
               pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id,
               pt_svl_rec->uheader.t_rec_hdr.ui4_nw_mask,
               pt_svl_rec->uheader.t_rec_hdr.ui4_option_mask,
               pt_svl_rec->uheader.t_rec_hdr.ui2_prog_id,
               pt_svl_rec->uheader.t_rec_hdr.ui2_tsl_id,
               pt_svl_rec->uheader.t_rec_hdr.ui2_tsl_rec_id,
               pt_svl_rec->uheader.t_rec_hdr.ui2_nwl_id,
               pt_svl_rec->uheader.t_rec_hdr.ui2_nwl_rec_id,
               pt_svl_rec->uheader.t_rec_hdr.ui2_satl_id,
               pt_svl_rec->uheader.t_rec_hdr.ui2_satl_rec_id,
               svl_service_type_to_string(pt_svl_rec->uheader.t_rec_hdr.e_serv_type),
               pt_svl_rec->uheader.t_rec_hdr.ac_name);

    if ( SB_ATSC_IS_ONE_PART_CHANNEL_ID(
             pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id)
        )
    {
        x_dbg_stmt("1-part channel id, major: %d  minor: %d ",
                   SB_ATSC_GET_MAJOR_CHANNEL_NUM(pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id),
                   SB_ATSC_GET_MINOR_CHANNEL_NUM(pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id));
    }
    else
    {
        x_dbg_stmt("2-part channel id, major: %d  minor: %d ",
                   SB_ATSC_GET_MAJOR_CHANNEL_NUM(pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id),
                   SB_ATSC_GET_MINOR_CHANNEL_NUM(pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id));
    }

    x_dbg_stmt("Brdcst type: %d ",
               pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type);

    switch(pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type)
    {
        case BRDCST_TYPE_ANALOG :
        {
            x_dbg_stmt(" BRDCST_TYPE_ANALOG ");                    
        }
        break;

        case BRDCST_TYPE_DVB :
        {
            x_dbg_stmt(" BRDCST_TYPE_DVB ");
        }
        break;

        case BRDCST_TYPE_ISDB :
        {
            x_dbg_stmt(" BRDCST_TYPE_ISDB ");
        }
        break;

        case BRDCST_TYPE_ATSC :
        {
            x_dbg_stmt(" BRDCST_TYPE_ATSC ");
        }
        break;

        case BRDCST_TYPE_SCTE :
        {
            x_dbg_stmt(" BRDCST_TYPE_SCTE ");
        }
        break;
                    
        case BRDCST_TYPE_DTMB :
        {
            x_dbg_stmt(" BRDCST_TYPE_DTMB ");
        }
        break;

        case BRDCST_TYPE_FMRDO:
        {
            x_dbg_stmt(" BRDCST_TYPE_FMRDO ");
        }
        break;

        case BRDCST_TYPE_UNKNOWN :
        {
            x_dbg_stmt(" BRDCST_TYPE_UNKNOWN ");
        }
        break;
    }

            
    switch(pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type)
    {
        case BRDCST_TYPE_ANALOG :
        {
            x_dbg_stmt("comp mask: %x ",
                       pt_svl_rec->u_data.t_analog.ui4_s_comp_mask);
                    
            if ( pt_svl_rec->u_data.t_analog.b_valid_src_id == TRUE )
            {
                x_dbg_stmt("source id: %d [flag %d]",
                           pt_svl_rec->u_data.t_analog.ui2_src_id,
                           pt_svl_rec->u_data.t_analog.b_valid_src_id );
            }
            else
            {
                x_dbg_stmt(" source id is not set. ");
            }

            if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys == TV_SYS_MASK_NONE )
            {
                x_dbg_stmt("TV system unknown. ");
            }
            else
            {
                x_dbg_stmt("TV sys: ");
                
                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_A
                    )
                {
                    x_dbg_stmt(" TV_SYS_A ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_B
                    )
                {
                    x_dbg_stmt(" TV_SYS_B ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_C
                    )
                {
                    x_dbg_stmt(" TV_SYS_C ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_D
                    )
                {
                    x_dbg_stmt(" TV_SYS_D ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_E
                    )
                {
                    x_dbg_stmt(" TV_SYS_E ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_F
                    )
                {
                    x_dbg_stmt(" TV_SYS_F ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_G
                    )
                {
                    x_dbg_stmt(" TV_SYS_G ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_H
                    )
                {
                    x_dbg_stmt(" TV_SYS_H ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_I
                    )
                {
                    x_dbg_stmt(" TV_SYS_I ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_J
                    )
                {
                    x_dbg_stmt(" TV_SYS_J ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_K
                    )
                {
                    x_dbg_stmt(" TV_SYS_K ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_K_PRIME
                    )
                {
                    x_dbg_stmt(" TV_SYS_K_PRIME ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_L
                    )
                {
                    x_dbg_stmt(" TV_SYS_L ");
                }
                
                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_L_PRIME
                    )
                {
                    x_dbg_stmt(" TV_SYS_L_PRIME ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_M
                    )
                {
                    x_dbg_stmt(" TV_SYS_M ");
                }

                if ( pt_svl_rec->u_data.t_analog.ui4_tv_sys &
                     TV_SYS_MASK_N
                    )
                {
                    x_dbg_stmt(" TV_SYS_N ");
                }
                
                x_dbg_stmt("\n");
            }

            x_dbg_stmt(" EU Data:  vps_ni_code: %d ttx_fmt_1_ni_code: %d ttx_fmt_2_cni_code: %d ",
                       pt_svl_rec->u_data.t_analog.ui2_vps_ni_code,
                       pt_svl_rec->u_data.t_analog.ui2_ttx_fmt_1_ni_code,
                       pt_svl_rec->u_data.t_analog.ui2_ttx_fmt_2_cni_code);

            x_dbg_stmt(" eu_data_mask: ");
            
            if ( (SVL_ANALOG_EU_DATA_MASK_NI_TTX_8_30_FMT_1 & pt_svl_rec->u_data.t_analog.t_eu_data_mask) > 0  )
            {
                x_dbg_stmt(" SVL_ANALOG_EU_DATA_MASK_NI_TTX_8_30_FMT_1 ") ;
            }

            if ( ( SVL_ANALOG_EU_DATA_MASK_CNI_TTX_8_30_FMT_2 & pt_svl_rec->u_data.t_analog.t_eu_data_mask) > 0  )
            {
                x_dbg_stmt(" SVL_ANALOG_EU_DATA_MASK_CNI_TTX_8_30_FMT_2 ") ;
            }

            if ( ( SVL_ANALOG_EU_DATA_MASK_CNI_VPS & pt_svl_rec->u_data.t_analog.t_eu_data_mask) > 0  )
            {
                x_dbg_stmt(" SVL_ANALOG_EU_DATA_MASK_CNI_VPS ") ;
            }

            if ( ( SVL_ANALOG_EU_DATA_MASK_VPS_DATA_BYTE_5 & pt_svl_rec->u_data.t_analog.t_eu_data_mask) > 0  )
            {
                x_dbg_stmt(" SVL_ANALOG_EU_DATA_MASK_VPS_DATA_BYTE_5 ") ;
            }

            x_dbg_stmt(" vps_byte_5: %d ", pt_svl_rec->u_data.t_analog.ui1_vps_data_byte_5);

            x_dbg_stmt(" Vid color sys: ");
            
            
            switch (pt_svl_rec->u_data.t_analog.e_vid_color_sys)
            {
                case COLOR_SYS_NTSC :
                {
                    x_dbg_stmt(" COLOR_SYS_NTSC ");
                }
                break;

                case COLOR_SYS_PAL:
                {
                    x_dbg_stmt(" COLOR_SYS_PAL ");
                }
                break;

                case COLOR_SYS_SECAM:
                {
                    x_dbg_stmt(" COLOR_SYS_SECAM ");
                }
                break;

                case COLOR_SYS_NTSC_443 :
                {
                    x_dbg_stmt(" COLOR_SYS_NTSC_443 ");
                }
                break;

                case COLOR_SYS_PAL_M:
                {
                    x_dbg_stmt(" COLOR_SYS_PAL_M ");
                }
                break;

                case COLOR_SYS_PAL_N:
                {
                    x_dbg_stmt(" COLOR_SYS_PAL_N ");
                }
                break;

                case COLOR_SYS_PAL_60:
                {
                    x_dbg_stmt(" COLOR_SYS_PAL_60 ");
                }
                break;
                
                default:
                {
                    x_dbg_stmt(" COLOR_SYS_T: %d ",
                               pt_svl_rec->u_data.t_analog.e_vid_color_sys);
                }
                break;
            }

            if ( pt_svl_rec->u_data.t_analog.b_no_aft == TRUE )
            {
                if ( pt_svl_rec->u_data.t_analog.i2_aft_offset == 0 )
                {
                    x_dbg_stmt(" AFT: auto. ");
                }
                else
                {
                    x_dbg_stmt(" AFT: manual offset %d. ",
                               pt_svl_rec->u_data.t_analog.i2_aft_offset);
                }
            }
            else
            {
                x_dbg_stmt(" AFT: disabled. ");
            }

#ifdef MW_ISDB_SUPPORT
            if (pt_svl_rec->u_data.t_analog.ui2_schedule_num > 0)
            {
                DTG_T   t_dtg = {0};
                UINT16  ui2_i;
                x_dbg_stmt("Schedule num: %d ",
                           pt_svl_rec->u_data.t_analog.ui2_schedule_num);
                for (ui2_i = 0; ui2_i < SVL_ANALOG_MAX_NUM_SCHEDULE; ui2_i++)
                {
                    if (pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][0].ui2_mjd == 0 &&
                        pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][0].ui2_bcd == 0 &&
                        pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][1].ui2_mjd == 0 &&
                        pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][1].ui2_bcd == 0)
                    {
                        continue;
                    }
                    if (pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][0].ui2_mjd == 0)
                    {
                        x_dbg_stmt("daily: %02x:%02x to %02x:%02x ",
                                   (pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][0].ui2_bcd >> 8),
                                   (pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][0].ui2_bcd & 0xFF),
                                   (pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][1].ui2_bcd >> 8),
                                   (pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][1].ui2_bcd & 0xFF));
                    }
                    else
                    {
                        x_dbg_stmt("once: ");
                        x_dt_mjd_to_dtg(pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][0].ui2_mjd,
                                        &t_dtg);
                        x_dbg_stmt("%d.%d.%d. ", t_dtg.ui2_yr, t_dtg.ui1_mo, t_dtg.ui1_day);
                        x_dbg_stmt("%02x:%02x to ",
                                   (pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][0].ui2_bcd >> 8),
                                   (pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][0].ui2_bcd & 0xFF));
                        x_dt_mjd_to_dtg(pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][1].ui2_mjd,
                                        &t_dtg);
                        x_dbg_stmt("%d.%d.%d. ", t_dtg.ui2_yr, t_dtg.ui1_mo, t_dtg.ui1_day);
                        x_dbg_stmt("%02x:%02x ",
                                   (pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][1].ui2_bcd >> 8),
                                   (pt_svl_rec->u_data.t_analog.at_schedule[ui2_i][1].ui2_bcd & 0xFF));
                    }
                }
            }
#endif

            x_dbg_stmt(" Audio MTS: %d ", pt_svl_rec->u_data.t_analog.ui1_aud_mts);

            x_dbg_stmt("\n");
        }
        break;

        case BRDCST_TYPE_DVB :
        {
            x_dbg_stmt("DVB record: reseved %d ",
                       pt_svl_rec->u_data.t_dvb.ui4_reserve);
            x_dbg_stmt("service name: %s ",
                       pt_svl_rec->u_data.t_dvb.s_svc_name);
            x_dbg_stmt("CA sys id: %d ",
                       pt_svl_rec->u_data.t_dvb.ui2_ca_system_id);
            
            x_dbg_stmt("running status: ");
            switch (pt_svl_rec->u_data.t_dvb.e_running_status)
            {
                case DVB_SVC_RUNNING_STATUS_UNDEFINED:
                {
                    x_dbg_stmt("DVB_SVC_RUNNING_STATUS_UNDEFINED ");
                }
                break;

                case DVB_SVC_NOT_RUNNING:
                {
                    x_dbg_stmt("DVB_SVC_NOT_RUNNING ");
                }
                break;

                case DVB_SVC_START_IN_SECONDS:
                {
                    x_dbg_stmt("DVB_SVC_START_IN_SECONDS ");
                }
                break;
                
                case DVB_SVC_PAUSING:
                {
                    x_dbg_stmt("DVB_SVC_PAUSING ");
                }
                break;
                
                case DVB_SVC_RUNNING:
                {
                    x_dbg_stmt("DVB_SVC_RUNNING ");
                }
                break;

                case DVB_SVC_OFF_AIR:
                {
                    x_dbg_stmt("DVB_SVC_OFF_AIR ");
                }
                break;

                default:
                {
                    x_dbg_stmt("unknown. ");
                }
                break;
            }
            
            x_dbg_stmt("linkage descr mask: %d ",
                       pt_svl_rec->u_data.t_dvb.ui4_linkage_descr_mask);
            
            x_dbg_stmt("CA replmnt on id: %d ",
                       pt_svl_rec->u_data.t_dvb.t_ca_replmnt_on_ts_svc_id_info.ui2_on_id);
            x_dbg_stmt("CA replmnt ts id: %d ",
                       pt_svl_rec->u_data.t_dvb.t_ca_replmnt_on_ts_svc_id_info.ui2_ts_id);
            x_dbg_stmt("CA replmnt svc id: %d ",
                       pt_svl_rec->u_data.t_dvb.t_ca_replmnt_on_ts_svc_id_info.ui2_svc_id);

            x_dbg_stmt("SVC replmnt on id: %d ",
                       pt_svl_rec->u_data.t_dvb.t_svc_replmnt_on_ts_svc_id_info.ui2_on_id);
            x_dbg_stmt("SVC replmnt ts id: %d ",
                       pt_svl_rec->u_data.t_dvb.t_svc_replmnt_on_ts_svc_id_info.ui2_ts_id);
            x_dbg_stmt("SVC replmnt svc id: %d ",
                       pt_svl_rec->u_data.t_dvb.t_svc_replmnt_on_ts_svc_id_info.ui2_svc_id);
            
            x_dbg_stmt("Completion EIT Schd on id: %d ",
                       pt_svl_rec->u_data.t_dvb.t_cmplt_eit_sch_on_ts_svc_id_info.ui2_on_id);
            x_dbg_stmt("Completion EIT schedule ts id: %d ",
                       pt_svl_rec->u_data.t_dvb.t_cmplt_eit_sch_on_ts_svc_id_info.ui2_ts_id);
            x_dbg_stmt("Completion EIT schedule svc id: %d ",
                       pt_svl_rec->u_data.t_dvb.t_cmplt_eit_sch_on_ts_svc_id_info.ui2_svc_id);
            x_dbg_stmt("Free CA mode: %s ",
                       ((pt_svl_rec->u_data.t_dvb.b_free_ca_mode)?"true":"false"));

            x_dbg_stmt("CI host shunning mode: %d ",
                       (pt_svl_rec->u_data.t_dvb.t_ci_host_shunning_mode));
            
            x_dbg_stmt("Original Service Info: ");
            x_dbg_stmt("On id: %d ",
                       pt_svl_rec->u_data.t_dvb.t_orig_svc_info.ui2_on_id);
            x_dbg_stmt("Ts id: %d ",
                       pt_svl_rec->u_data.t_dvb.t_orig_svc_info.ui2_ts_id);
            x_dbg_stmt("Svc id: %d ",
                       pt_svl_rec->u_data.t_dvb.t_orig_svc_info.ui2_svc_id);

            x_dbg_stmt("Subtitle preference: ");
            x_dbg_stmt("Language 1: %s ",
                       pt_svl_rec->u_data.t_dvb.t_sbtl_pref.t_lang_1st);
            x_dbg_stmt("Language 2: %s ",
                       pt_svl_rec->u_data.t_dvb.t_sbtl_pref.t_lang_2nd);
            
            x_dbg_stmt("Downmix mode: %d ",
                       pt_svl_rec->u_data.t_dvb.ui1_downmix_mode);

            x_dbg_stmt("Ref SVL rec ID: %d "
                       "TS svc time start: %lld "
                       "TS svc duration: %lld "
                       "Last view SVL ID: %d "
                       "Last view SVL rec ID: %d "
                       "MF type: %d ",
                       pt_svl_rec->u_data.t_dvb.ui2_ref_svl_rec_id,
                       pt_svl_rec->u_data.t_dvb.t_ts_svc_time_start,
                       pt_svl_rec->u_data.t_dvb.t_ts_svc_duration,
                       pt_svl_rec->u_data.t_dvb.ui2_last_view_svl_id,
                       pt_svl_rec->u_data.t_dvb.ui2_last_view_svl_rec_id,
                       pt_svl_rec->u_data.t_dvb.ui1_mf_type
                       );
            x_dbg_stmt("LCN: %d ",pt_svl_rec->u_data.t_dvb.ui2_lcn);

			x_dbg_stmt("ts_id: %d ",pt_svl_rec->u_data.t_dvb.ui2_ts_id);
			x_dbg_stmt("on_id: %d ",pt_svl_rec->u_data.t_dvb.ui2_on_id);
            x_dbg_stmt("\n");
        }
        break;
        case BRDCST_TYPE_DTMB :
        {
            x_dbg_stmt("DTMB record: reseved %d ",
                       pt_svl_rec->u_data.t_dtmb.ui4_reserve);
            x_dbg_stmt("service name: %s ",
                       pt_svl_rec->u_data.t_dtmb.s_svc_name);
            x_dbg_stmt("CA sys id: %d ",
                       pt_svl_rec->u_data.t_dtmb.ui2_ca_system_id);
            
            x_dbg_stmt("running status: ");
            switch (pt_svl_rec->u_data.t_dtmb.e_running_status)
            {
                case DTMB_SVC_RUNNING_STATUS_UNDEFINED:
                {
                    x_dbg_stmt("DTMB_SVC_RUNNING_STATUS_UNDEFINED ");
                }
                break;

                case DTMB_SVC_NOT_RUNNING:
                {
                    x_dbg_stmt("DTMB_SVC_NOT_RUNNING ");
                }
                break;

                case DTMB_SVC_START_IN_SECONDS:
                {
                    x_dbg_stmt("DTMB_SVC_START_IN_SECONDS ");
                }
                break;
                
                case DTMB_SVC_PAUSING:
                {
                    x_dbg_stmt("DTMB_SVC_PAUSING ");
                }
                break;
                
                case DTMB_SVC_RUNNING:
                {
                    x_dbg_stmt("DTMB_SVC_RUNNING ");
                }
                break;

                case DTMB_SVC_OFF_AIR:
                {
                    x_dbg_stmt("DTMB_SVC_OFF_AIR ");
                }
                break;

                default:
                {
                    x_dbg_stmt("unknown. ");
                }
                break;
            }
            
            x_dbg_stmt("linkage descr mask: %d ",
                       pt_svl_rec->u_data.t_dtmb.ui4_linkage_descr_mask);
            
            x_dbg_stmt("CA replmnt on id: %d ",
                       pt_svl_rec->u_data.t_dtmb.t_ca_replmnt_on_ts_svc_id_info.ui2_on_id);
            x_dbg_stmt("CA replmnt ts id: %d ",
                       pt_svl_rec->u_data.t_dtmb.t_ca_replmnt_on_ts_svc_id_info.ui2_ts_id);
            x_dbg_stmt("CA replmnt svc id: %d ",
                       pt_svl_rec->u_data.t_dtmb.t_ca_replmnt_on_ts_svc_id_info.ui2_svc_id);

            x_dbg_stmt("SVC replmnt on id: %d ",
                       pt_svl_rec->u_data.t_dtmb.t_svc_replmnt_on_ts_svc_id_info.ui2_on_id);
            x_dbg_stmt("SVC replmnt ts id: %d ",
                       pt_svl_rec->u_data.t_dtmb.t_svc_replmnt_on_ts_svc_id_info.ui2_ts_id);
            x_dbg_stmt("SVC replmnt svc id: %d ",
                       pt_svl_rec->u_data.t_dtmb.t_svc_replmnt_on_ts_svc_id_info.ui2_svc_id);
            
            x_dbg_stmt("Completion EIT Schd on id: %d ",
                       pt_svl_rec->u_data.t_dtmb.t_cmplt_eit_sch_on_ts_svc_id_info.ui2_on_id);
            x_dbg_stmt("Completion EIT schedule ts id: %d ",
                       pt_svl_rec->u_data.t_dtmb.t_cmplt_eit_sch_on_ts_svc_id_info.ui2_ts_id);
            x_dbg_stmt("Completion EIT schedule svc id: %d ",
                       pt_svl_rec->u_data.t_dtmb.t_cmplt_eit_sch_on_ts_svc_id_info.ui2_svc_id);
            x_dbg_stmt("Free CA mode: %s ",
                       ((pt_svl_rec->u_data.t_dtmb.b_free_ca_mode)?"true":"false"));

            
            x_dbg_stmt("Original Service Info: ");
            x_dbg_stmt("On id: %d ",
                       pt_svl_rec->u_data.t_dtmb.t_orig_svc_info.ui2_on_id);
            x_dbg_stmt("Ts id: %d ",
                       pt_svl_rec->u_data.t_dtmb.t_orig_svc_info.ui2_ts_id);
            x_dbg_stmt("Svc id: %d ",
                       pt_svl_rec->u_data.t_dtmb.t_orig_svc_info.ui2_svc_id);

            x_dbg_stmt("Downmix mode: %d ",
                       pt_svl_rec->u_data.t_dtmb.ui1_downmix_mode);

            x_dbg_stmt("\n");
        }
        break;
#ifdef MW_ISDB_SUPPORT
		case BRDCST_TYPE_ISDB :
        {
            x_dbg_stmt("ISDB record: ui1_reg_key: %d e_svc_layer: %d ",
                       pt_svl_rec->u_data.t_isdb.ui1_reg_key,
                       pt_svl_rec->u_data.t_isdb.e_svc_layer);
            x_dbg_stmt("Downmix mode: %d ",
                       pt_svl_rec->u_data.t_isdb.ui1_downmix_mode);

            if (pt_svl_rec->u_data.t_isdb.ui2_schedule_num > 0)
            {
                DTG_T   t_dtg = {0};
                UINT16  ui2_i;
                x_dbg_stmt("Schedule num: %d ",
                           pt_svl_rec->u_data.t_isdb.ui2_schedule_num);
                for (ui2_i = 0; ui2_i < SVL_ISDB_MAX_NUM_SCHEDULE; ui2_i++)
                {
                    if (pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][0].ui2_mjd == 0 &&
                        pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][0].ui2_bcd == 0 &&
                        pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][1].ui2_mjd == 0 &&
                        pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][1].ui2_bcd == 0)
                    {
                        continue;
                    }
                    if (pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][0].ui2_mjd == 0)
                    {
                        x_dbg_stmt("daily: %02x:%02x to %02x:%02x ",
                                   (pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][0].ui2_bcd >> 8),
                                   (pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][0].ui2_bcd & 0xFF),
                                   (pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][1].ui2_bcd >> 8),
                                   (pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][1].ui2_bcd & 0xFF));
                    }
                    else
                    {
                        x_dbg_stmt("once: ");
                        x_dt_mjd_to_dtg(pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][0].ui2_mjd,
                                        &t_dtg);
                        x_dbg_stmt("%d.%d.%d. ", t_dtg.ui2_yr, t_dtg.ui1_mo, t_dtg.ui1_day);
                        x_dbg_stmt("%02x:%02x to ",
                                   (pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][0].ui2_bcd >> 8),
                                   (pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][0].ui2_bcd & 0xFF));
                        x_dt_mjd_to_dtg(pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][1].ui2_mjd,
                                        &t_dtg);
                        x_dbg_stmt("%d.%d.%d. ", t_dtg.ui2_yr, t_dtg.ui1_mo, t_dtg.ui1_day);
                        x_dbg_stmt("%02x:%02x ",
                                   (pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][1].ui2_bcd >> 8),
                                   (pt_svl_rec->u_data.t_isdb.at_schedule[ui2_i][1].ui2_bcd & 0xFF));
                    }
                }
            }
            x_dbg_stmt("\n");
        }
        break;
#endif
        case BRDCST_TYPE_ATSC :
        {
            x_dbg_stmt("ATSC record: source id %d ",
                       pt_svl_rec->u_data.t_atsc.ui2_src_id);
            x_dbg_stmt("Downmix mode: %d ",
                       pt_svl_rec->u_data.t_atsc.ui1_downmix_mode);
            x_dbg_stmt("\n");
        }
        break;

        case BRDCST_TYPE_SCTE :
        {
            x_dbg_stmt("SCTE record: source id %d ",
                       pt_svl_rec->u_data.t_atsc.ui2_src_id);
            x_dbg_stmt("Downmix mode: %d ",
                       pt_svl_rec->u_data.t_atsc.ui1_downmix_mode);
            x_dbg_stmt("\n");
        }
        break;

        case BRDCST_TYPE_FMRDO:
        {
            x_dbg_stmt("FM Radio record: signal level %d ",
                       pt_svl_rec->u_data.t_fmrdo.ui1_sgl_lvl);
            x_dbg_stmt("\n");
        }
        break;

        case BRDCST_TYPE_UNKNOWN :
        {
            x_dbg_stmt("BRDCST_TYPE_UNKNOWN\n");
        }
        break;
    }
}

/*-------------------------------------------------------------------------
 * Name: svl_dump_rec
 *
 * Description: This API prints out SVL record entries for the
 *    specified SVL ID.
 *
 * Inputs:  
 *    ui2_svl_id:     SVL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   svl_dump_rec(UINT16    ui2_svl_id, BRDCST_TYPE_T e_brdcst_type)
{
    HANDLE_T   h_svl;
    UINT32     ui4_ver_id ;
    UINT16     i2_num_rec;
    UINT16     ui2_idx;
    INT32      i4_rc;
    SVL_REC_T  t_svl_rec;


    if ( x_svl_open(ui2_svl_id, NULL, NULL, &h_svl) != SVLR_OK )
    {
        x_dbg_stmt("Error: SVL ID %d is not valid.\n", ui2_svl_id);
        return;
    }
    
    ui4_ver_id = SVL_NULL_VER_ID;
    /*
      Get the total number of record.
    */
    i2_num_rec = 0;

    x_svl_get_num_rec_by_brdcst_type(h_svl, e_brdcst_type,
                                     0xFFFFFFFF,
                                     &i2_num_rec,
                                     &ui4_ver_id);
    
    x_dbg_stmt("Total number of SVL recrods in handle %d are: %d\n",
               h_svl, i2_num_rec);
    
    ui2_idx = 0;
    do
    {
        i4_rc = x_svl_get_rec_by_brdcst_type(
            h_svl, e_brdcst_type, 0xFFFFFFFF,
            ui2_idx, &t_svl_rec, &ui4_ver_id);

        if ( i4_rc == SVLR_OK )
        {
            svl_print_rec(&t_svl_rec, ui2_idx);    
        }
        else
        {
            break;
        }
        /*
          increment the idx to get the next record with
          same broadcast type.
        */
        ui2_idx++;
            
    }  while ( 1 ) ;

    x_svl_close(h_svl);
    
    return;
}


/*-------------------------------------------------------------------------
 * Name: svl_print_info
 *
 * Description: This API prints information for the SVL object.
 *
 * Inputs:  
 *    ui2_svl_id:     SVL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   svl_print_info(UINT16    ui2_svl_id)
{
    HANDLE_T   h_svl;

    if ( x_svl_open(ui2_svl_id, NULL, NULL, &h_svl) != SVLR_OK )
    {
        x_dbg_stmt("Error: SVL ID %d is not valid.\n", ui2_svl_id);
        return;
    }

    x_svl_get_info(h_svl);
    
    x_svl_close(h_svl);
    
    return;
}


    

    
