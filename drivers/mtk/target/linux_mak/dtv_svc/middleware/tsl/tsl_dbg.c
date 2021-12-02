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
 * $RCSfile: tsl_dbg.c,v $
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

#include "tsl/x_tsl.h"
#include "svl/x_svl.h"
/*
  internal variable for the debug level.
*/
static UINT16  ui2_dbg_level = DBG_INIT_LEVEL_MW_TSL;

/*----------------------------------------------
  Public Function for DBG support.
------------------------------------------------*/
UINT16 tsl_get_dbg_level(VOID)
{
    return ui2_dbg_level;
}

VOID   tsl_set_dbg_level(UINT16  ui2_dbg)
{
    ui2_dbg_level = ui2_dbg;
}

/*-------------------------------------------------------------------------
 * Name: tsl_print_info
 *
 * Description: This API prints information for the TSL object.
 *
 * Inputs:  
 *    ui2_tsl_id:     TSL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID tsl_print_info(UINT16    ui2_tsl_id)
{
    HANDLE_T   h_tsl;

    if ( x_tsl_open(ui2_tsl_id, NULL, NULL, &h_tsl) != TSLR_OK )
    {
        x_dbg_stmt("Error: TSL ID %d is not valid.\n", ui2_tsl_id);
        return;
    }

    x_tsl_get_info(h_tsl);
    
    x_tsl_close(h_tsl);
    
    return;
}

/*-------------------------------------------------------------------------
 * Name: e_brdcst_type_name
 *
 * Description: This API converts BRDCST_TYPE_T enum to a string name. 
 *
 * Inputs:  
 *    e_brdcst_type   specifies BRDCST_TYPE_T enum
 *
 * Outputs: -
 *
 * Returns: brdcst type name.
 --------------------------------------------------------------------------*/
static CHAR*  e_brdcst_type_name(BRDCST_TYPE_T   e_brdcst_type)
{
    CHAR*    s_name;
    
    switch (e_brdcst_type)
    {
        case BRDCST_TYPE_UNKNOWN:
        {
            s_name="UNKNOWN";
        }
        break;

        case BRDCST_TYPE_ANALOG:
        {
            s_name="ANALOG";
        }
        break;

        case BRDCST_TYPE_DVB:
        {
            s_name="DVB";
        }
        break;

        case BRDCST_TYPE_ATSC:
        {
            s_name="ATSC";
        }
        break;

        case BRDCST_TYPE_SCTE:
        {
            s_name="SCTE";
        }
        break;

        default:
        {
            s_name=NULL;
        }
    }
    return s_name;
}


/*-------------------------------------------------------------------------
 * Name: e_brdcst_medium_name
 *
 * Description: This API converts BRDCST_MEDIUM_T enum to a string name. 
 *
 * Inputs:  
 *    e_brdcst_medium   specifies BRDCST_MEDIUM_T enum
 *
 * Outputs: -
 *
 * Returns: brdcst medium name.
 --------------------------------------------------------------------------*/
static CHAR*  e_brdcst_medium_name(BRDCST_MEDIUM_T   e_brdcst_medium)
{
    CHAR*    s_name;
    
    switch (e_brdcst_medium)
    {
        case BRDCST_MEDIUM_UNKNOWN:
        {
            s_name="UNKNOWN";
        }
        break;

        case BRDCST_MEDIUM_DIG_TERRESTRIAL:
        {
            s_name="DIG_TERRESTRIAL";
        }
        break;

        case BRDCST_MEDIUM_DIG_CABLE:
        {
            s_name="DIG_CABLE";
        }
        break;

        case BRDCST_MEDIUM_DIG_SATELLITE:
        {
            s_name="DIG_SATELLITE";
        }
        break;

        case BRDCST_MEDIUM_ANA_TERRESTRIAL:
        {
            s_name="ANA_TERRESTRIAL";
        }
        break;

        case BRDCST_MEDIUM_ANA_CABLE:
        {
            s_name="ANA_CABLE";
        }
        break;

        case BRDCST_MEDIUM_ANA_SATELLITE:
        {
            s_name="ANA_SATELLITE";
        }
        break;

        case BRDCST_MEDIUM_1394:
        {
            s_name="1934";
        }
        break;
        
        default:
        {
            s_name=NULL;
        }
        break;
    }
    return s_name;
}

extern VOID dt_print_utc(TIME_T t_utc_sec);
/*-------------------------------------------------------------------------
 * Name: e_tuner_modulation_name
 *
 * Description: This API converts TUNER_MODULATION_T enum to a string name. 
 *
 * Inputs:  
 *    e_tuner_mod   specifies TUNER_MODULATION enum
 *
 * Outputs: -
 *
 * Returns: brdcst medium name.
 --------------------------------------------------------------------------*/
static CHAR*  e_tuner_modulation_name(TUNER_MODULATION_T e_tuner_mod)
{
    CHAR*    s_name;
    
    switch (e_tuner_mod)
    {
        case MOD_UNKNOWN:
        {
            s_name="UNKNOWN";
        }
        break;

        case MOD_PSK_8:
        {
            s_name="MOD_PSK_8";
        }
        break;

        case MOD_VSB_8:
        {
            s_name="MOD_VSB_8";
        }
        break;

        case MOD_VSB_16:
        {
            s_name="MOD_VSB_16";
        }
        break;

        case MOD_QAM_16:
        {
            s_name="MOD_QAM_16";
        }
        break;

        case MOD_QAM_32:
        {
            s_name="MOD_QAM_32";
        }
        break;

        case MOD_QAM_64:
        {
            s_name="MOD_QAM_64";
        }
        break;

        case MOD_QAM_80:
        {
            s_name="MOD_QAM_80";
        }
        break;

        case MOD_QAM_96:
        {
            s_name="MOD_QAM_96";
        }
        break;

        case MOD_QAM_112:
        {
            s_name="MOD_QAM_112";
        }
        break;

        case MOD_QAM_128:
        {
            s_name="MOD_QAM_128";
        }
        break;

        case MOD_QAM_160:
        {
            s_name="MOD_QAM_160";
        }
        break;

        case MOD_QAM_192:
        {
            s_name="MOD_QAM_192";
        }
        break;

        case MOD_QAM_224:
        {
            s_name="MOD_QAM_224";
        }
        break;

        case MOD_QAM_256:
        {
            s_name="MOD_QAM_256";
        }
        break;

        case MOD_QAM_320:
        {
            s_name="MOD_QAM_320";
        }
        break;

        case MOD_QAM_384:
        {
            s_name="MOD_QAM_384";
        }
        break;

        case MOD_QAM_448:
        {
            s_name="MOD_QAM_448";
        }
        break;

        case MOD_QAM_512:
        {
            s_name="MOD_QAM_512";
        }
        break;

        case MOD_QAM_640:
        {
            s_name="MOD_QAM_640";
        }
        break;

        case MOD_QAM_768:
        {
            s_name="MOD_QAM_768";
        }
        break;

        case MOD_QAM_896:
        {
            s_name="MOD_QAM_896";
        }
        break;

        case MOD_QAM_1024:
        {
            s_name="MOD_QAM_1024";
        }
        break;

        case MOD_QPSK:
        {
            s_name="MOD_QPSK";
        }
        break;

        case MOD_OQPSK:
        {
            s_name="MOD_OQPSK";
        }
        break;

        case MOD_BPSK:
        {
            s_name="MOD_BPSK";
        }
        break;

        case MOD_VSB_AM:
        {
            s_name="MOD_VSB_AM";
        }
        break;

        case MOD_QAM_4_NR:
        {
            s_name="MOD_QAM_4_NR";
        }
        break;
        
        default:
        {
            s_name=NULL;
        }
        break;
    }
    return s_name;
}

/*-------------------------------------------------------------------------
 * Name: tsl_print_rec
 *
 * Description: This API prints out TSL record entries for the
 *    specified TSL ID.
 *
 * Inputs:  
 *    ui2_tsl_id:     TSL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   tsl_print_rec(const TSL_REC_T*  pt_tsl_rec, UINT16  ui2_idx)
{
    UINT16     ui2_nwl_id;
    UINT16     ui2_nwl_rec_id;

#ifndef MW_DVBS_DEV_ENABLE
    ui2_nwl_id     = TS_DESCR_GET_NWL_ID(pt_tsl_rec->uheader.t_desc);
    ui2_nwl_rec_id = TS_DESCR_GET_NWL_REC_ID(pt_tsl_rec->uheader.t_desc);
#else
    ui2_nwl_id     = pt_tsl_rec->uref.t_ref.ui2_nwl_id;
    ui2_nwl_rec_id = pt_tsl_rec->uref.t_ref.ui2_nwl_rec_id;
#endif
    
    x_dbg_stmt("%d => "
               "tsl_rec id: [%d] "
               "brdcst type: [%s] "
               "brdcst medium: [%s] "
               "nw id: [%d] "
               "orig nw id: [%d] "
               "ts: [%d] "
               "nwl_id: [%d] "
               "nwl_rec_id: [%d] "
               "nw_name:[%s] "
               "satl_id: [%d] "
               "satl_rec_id: [%d] "
               "mask: [0x%x] "
               "\n",
               ui2_idx,
               pt_tsl_rec->ui2_tsl_rec_id,
               e_brdcst_type_name(pt_tsl_rec->uheader.t_desc.e_bcst_type),
               e_brdcst_medium_name(pt_tsl_rec->uheader.t_desc.e_bcst_medium),
               pt_tsl_rec->uheader.t_desc.ui2_nw_id,
               pt_tsl_rec->uheader.t_desc.ui2_on_id,
               pt_tsl_rec->uheader.t_desc.ui2_ts_id,
               ui2_nwl_id,
               ui2_nwl_rec_id,
               pt_tsl_rec->s_nw_name,
               pt_tsl_rec->uref.t_ref.ui2_satl_id,
               pt_tsl_rec->uref.t_ref.ui2_satl_rec_id,
               pt_tsl_rec->uref.t_ref.ui4_mask);

    switch(pt_tsl_rec->uheader.t_desc.e_bcst_medium)
    {
        case BRDCST_MEDIUM_DIG_TERRESTRIAL :
        {
            INT32 i4_i;

            x_dbg_stmt(" tuner freq: %d ", pt_tsl_rec->udata.t_ter_dig.ui4_freq);
            x_dbg_stmt(" bandwidth: %d ", pt_tsl_rec->udata.t_ter_dig.e_bandwidth);
            
            x_dbg_stmt(" e_fec_outer: %d ", pt_tsl_rec->udata.t_ter_dig.e_fec_outer);
            x_dbg_stmt(" e_fec_inner: %d ", pt_tsl_rec->udata.t_ter_dig.e_fec_inner);
            x_dbg_stmt(" e_mod: %d ", pt_tsl_rec->udata.t_ter_dig.e_mod);
            x_dbg_stmt(" e_hierarchy: %d ", pt_tsl_rec->udata.t_ter_dig.e_hierarchy);
            x_dbg_stmt(" e_hierarchy_priority: %d ", pt_tsl_rec->udata.t_ter_dig.e_hierarchy_priority);
            x_dbg_stmt(" e_guard_int: %d ", pt_tsl_rec->udata.t_ter_dig.e_guard_int);
            x_dbg_stmt(" e_trans_mode: %d ", pt_tsl_rec->udata.t_ter_dig.e_trans_mode);
            x_dbg_stmt(" e_timer_int: %d ", pt_tsl_rec->udata.t_ter_dig.e_timer_int);
            x_dbg_stmt(" b_other_freq: %d ", pt_tsl_rec->udata.t_ter_dig.b_other_freq);
            x_dbg_stmt(" e_pol: %d ", pt_tsl_rec->udata.t_ter_dig.e_pol);
            x_dbg_stmt(" e_conn_mode: %d ", pt_tsl_rec->udata.t_ter_dig.e_conn_mode);
            x_dbg_stmt(" ui2_dir : %d ", pt_tsl_rec->udata.t_ter_dig.ui2_dir );
            x_dbg_stmt(" ui2_gain: %d ", pt_tsl_rec->udata.t_ter_dig.ui2_gain);
            x_dbg_stmt(" b_no_disc_nfy: %d ", pt_tsl_rec->udata.t_ter_dig.b_no_disc_nfy);
            
#ifdef DTMB_MANUAL_DEMOD_DETECT
            x_dbg_stmt(" e_FecRate"  ": %d ", pt_tsl_rec->udata.t_ter_dig.e_FecRate    );
            x_dbg_stmt(" e_CarrierMode: %d ", pt_tsl_rec->udata.t_ter_dig.e_CarrierMode);
            x_dbg_stmt(" e_PnNumber" ": %d ", pt_tsl_rec->udata.t_ter_dig.e_PnNumber   );
            x_dbg_stmt(" ui4_NCO"    ": %d ", pt_tsl_rec->udata.t_ter_dig.ui4_NCO      );
#endif
            
            /* print out additional alternate tune info */
            x_dbg_stmt(" Alternate freq info [freq bandwidth]: ");
            for (i4_i=0; i4_i < MAX_NB_ALTERNATE_FREQ ; i4_i++ )
            {
                x_dbg_stmt(" [%d %d] ",
                           pt_tsl_rec->utune.t_alt_ter_dig.t_all_freq[i4_i].ui4_freq,
                           pt_tsl_rec->utune.t_alt_ter_dig.t_all_freq[i4_i].e_bandwidth);
            }
            x_dbg_stmt(" Orig freq info [freq bandwidth]: ");
            x_dbg_stmt(" [%d %d] ",
                       pt_tsl_rec->utune.t_alt_ter_dig.t_orig_freq.ui4_freq,
                       pt_tsl_rec->utune.t_alt_ter_dig.t_orig_freq.e_bandwidth);

            x_dbg_stmt(" Freq lost time: %lld ", pt_tsl_rec->utune.t_alt_ter_dig.t_loss_event_time);
            dt_print_utc(pt_tsl_rec->utune.t_alt_ter_dig.t_loss_event_time);

            x_dbg_stmt(" First tune: %d ", pt_tsl_rec->utune.t_alt_ter_dig.ui1_first_tune);
            
            x_dbg_stmt(" Freq updated: %d ", pt_tsl_rec->utune.t_alt_ter_dig.b_freq_updated);
            
        }
        break;

        case BRDCST_MEDIUM_DIG_CABLE :
        {
            x_dbg_stmt(" tuner freq: %d ", pt_tsl_rec->udata.t_cab_dig.ui4_freq);
            x_dbg_stmt(" symbol rate: %d ", pt_tsl_rec->udata.t_cab_dig.ui4_sym_rate);
            
            x_dbg_stmt(" e_fec_outer: %d ", pt_tsl_rec->udata.t_cab_dig.e_fec_outer);
            x_dbg_stmt(" e_fec_inner: %d ", pt_tsl_rec->udata.t_cab_dig.e_fec_inner);
            x_dbg_stmt(" e_mod: %d ", pt_tsl_rec->udata.t_cab_dig.e_mod);
            x_dbg_stmt(" e_conn_mode: %d ", pt_tsl_rec->udata.t_cab_dig.e_conn_mode);
            x_dbg_stmt(" b_no_disc_nfy: %d ", pt_tsl_rec->udata.t_cab_dig.b_no_disc_nfy);
        }
        break;

        case BRDCST_MEDIUM_DIG_SATELLITE :
        {
            x_dbg_stmt(" tuner freq: %d ", pt_tsl_rec->udata.t_sat_dig.ui4_freq);
            x_dbg_stmt(" symbol rate: %d ", pt_tsl_rec->udata.t_sat_dig.ui4_sym_rate);
            
            x_dbg_stmt(" e_fec_inner: %d ", pt_tsl_rec->udata.t_sat_dig.e_fec_inner);
            x_dbg_stmt(" e_mod: %d ", pt_tsl_rec->udata.t_sat_dig.e_mod);
            x_dbg_stmt(" e_pol: %d ", pt_tsl_rec->udata.t_sat_dig.e_pol);
            x_dbg_stmt(" e_conn_mode: %d ", pt_tsl_rec->udata.t_sat_dig.e_conn_mode);
            x_dbg_stmt(" i2_orb_pos: %d ", pt_tsl_rec->udata.t_sat_dig.i2_orb_pos);
            x_dbg_stmt(" b_no_disc_nfy: %d ", pt_tsl_rec->udata.t_sat_dig.b_no_disc_nfy);
        }
        break;

        default:
        {
        }
        break;
    }
    
    switch(pt_tsl_rec->uheader.t_desc.e_bcst_type)
    {
        case BRDCST_TYPE_ANALOG :
        {
            if ( pt_tsl_rec->uheader.t_desc.e_bcst_medium ==
                 BRDCST_MEDIUM_ANA_CABLE )
            {
                const TUNER_CAB_ANA_TUNE_INFO_T*  pt_tuner;

                pt_tuner = &(pt_tsl_rec->udata.t_cab_ana);

                x_dbg_stmt("<Analog cable> freq: %d freq_bound: %d central_freq: %d freq_offset: %d tv_sys_mask: %d e_mod: %s b_fine_tune: %d b_no_disc_nfy %d\n",
                           pt_tuner->ui4_freq,
                           pt_tuner->ui4_freq_bound,
                           pt_tuner->ui4_central_freq,
                           pt_tuner->ui4_freq_offset,
                           pt_tuner->ui4_tv_sys_mask,
                           e_tuner_modulation_name(pt_tuner->e_mod),
                           pt_tuner->b_fine_tune,
                           pt_tuner->b_no_disc_nfy);
            }
            else if ( pt_tsl_rec->uheader.t_desc.e_bcst_medium ==
                      BRDCST_MEDIUM_ANA_TERRESTRIAL )
            {
                const TUNER_TER_ANA_TUNE_INFO_T*  pt_tuner;

                pt_tuner = &(pt_tsl_rec->udata.t_ter_ana);

                x_dbg_stmt("<Analog terrestrial> freq: %d freq_bound: %d central_freq: %d freq_offset: %d tv_sys_mask: %d e_policy: %d e_mod: %s dirction: %d gain: %d b_fine_tune: %d b_no_disc_nfy %d\n",
                           pt_tuner->ui4_freq,
                           pt_tuner->ui4_freq_bound,
                           pt_tuner->ui4_central_freq,
                           pt_tuner->ui4_freq_offset,
                           pt_tuner->ui4_tv_sys_mask,
                           pt_tuner->e_pol,
                           e_tuner_modulation_name(pt_tuner->e_mod),
                           pt_tuner->ui2_dir,
                           pt_tuner->ui2_gain,
                           pt_tuner->b_fine_tune,
                           pt_tuner->b_no_disc_nfy);
            }
        }
        break;

        case BRDCST_TYPE_DVB :
        {
            if ( pt_tsl_rec->uheader.t_desc.e_bcst_medium ==
                 BRDCST_MEDIUM_DIG_TERRESTRIAL )
            {
                const TUNER_TER_DIG_TUNE_INFO_T*   pt_ter_dig;
                
                pt_ter_dig=&(pt_tsl_rec->udata.t_ter_dig);
                
                x_dbg_stmt("<DVB terr> freq: %d band_width: %d fec_outer: %d fec_inner: %d e_mod: %s e_hierarchy: %d e_hier_priority: %d guard: %d trans_mode: %d timer_int: %d other_freq: %d e_pol: %d conn_mode: %d dir: %d gain: %d b_no_disc_nfy: %d \n",
                           pt_ter_dig->ui4_freq,
                           pt_ter_dig->e_bandwidth,
                           pt_ter_dig->e_fec_outer,
                           pt_ter_dig->e_fec_inner,
                           e_tuner_modulation_name(pt_ter_dig->e_mod),
                           pt_ter_dig->e_hierarchy,
                           pt_ter_dig->e_hierarchy_priority,
                           pt_ter_dig->e_guard_int,
                           pt_ter_dig->e_trans_mode,
                           pt_ter_dig->e_timer_int,
                           pt_ter_dig->b_other_freq,
                           pt_ter_dig->e_pol,
                           pt_ter_dig->e_conn_mode,
                           pt_ter_dig->ui2_dir,
                           pt_ter_dig->ui2_gain,
                           pt_ter_dig->b_no_disc_nfy
                    );
            }
        }
        break;

        case BRDCST_TYPE_ATSC :
        {
            if ( pt_tsl_rec->uheader.t_desc.e_bcst_medium ==
                 BRDCST_MEDIUM_DIG_TERRESTRIAL )
            {
                const TUNER_TER_DIG_TUNE_INFO_T*   pt_ter_dig;
                
                pt_ter_dig=&(pt_tsl_rec->udata.t_ter_dig);
                
                x_dbg_stmt("<ATSC terr> freq: [%d]\n",pt_ter_dig->ui4_freq);
            }
        }
        break;
        #ifdef MW_ISDB_SUPPORT
        case BRDCST_TYPE_ISDB :
        {
            if ( pt_tsl_rec->uheader.t_desc.e_bcst_medium ==
                 BRDCST_MEDIUM_DIG_TERRESTRIAL )
            {
                const TUNER_TER_DIG_TUNE_INFO_T*   pt_ter_dig;
                INT32   i4_pos = 0;
                pt_ter_dig=&(pt_tsl_rec->udata.t_isdbt_dig.t_ter_dig);
                x_dbg_stmt("<ISDBT terr> freq: [%d]\n",pt_ter_dig->ui4_freq);                
                x_dbg_stmt("<Remote control> key id: [%d]\n", pt_tsl_rec->udata.t_isdbt_dig.ui1_rc_key_id);
                x_dbg_stmt("<Layer ctrl> layer count: [%d]\n", pt_tsl_rec->udata.t_isdbt_dig.ui1_num_layers);
                x_dbg_stmt("<TS name> tsname: [%s]\n", pt_tsl_rec->udata.t_isdbt_dig.ac_ts_name);
                x_dbg_stmt("data:");
                while (i4_pos< 41)
                {
                    x_dbg_stmt("%d,",*(pt_tsl_rec->udata.t_isdbt_dig.ac_ts_name+i4_pos));
                    i4_pos++;
                }
                x_dbg_stmt("\n");
            }
        }
        break;
        #endif
        case BRDCST_TYPE_SCTE :
        {
        }
        break;

        case BRDCST_TYPE_UNKNOWN :
        {
            
        }
        break;
    }

    return;
}

/*-------------------------------------------------------------------------
 * Name: tsl_print_all_rec
 *
 * Description: This API prints out TSL record entries for the
 *    specified tsl ID.
 *
 * Inputs:  
 *    ui2_TSL_id:     TSL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   tsl_print_all_rec(UINT16    ui2_tsl_id)
{
    HANDLE_T   h_tsl;
    UINT32     ui4_ver_id ;
    UINT32     i4_num_rec = 0;
    INT32      i4_rc;
    TSL_REC_T  t_tsl_rec = {0}; /* try to fix Klocwork issue */

    if ( x_tsl_open(ui2_tsl_id, NULL, NULL, &h_tsl) != TSLR_OK )
    {
        x_dbg_stmt("Error: TSL ID %d is not valid.\n", ui2_tsl_id);
        return;
    }
    
    ui4_ver_id = TSL_NULL_VER_ID;

    i4_rc = x_tsl_read_lock(h_tsl);
    
    i4_rc=x_tsl_sort_rec(h_tsl, &ui4_ver_id);

    if ( i4_rc == TSLR_OK )
    {
        i4_rc=x_tsl_set_cursor(h_tsl,TSL_FIRST_REC);

        while (i4_rc == TSLR_OK)
        {
            i4_rc = x_tsl_read_cursor(h_tsl, &t_tsl_rec, &ui4_ver_id);
            if ( i4_rc == TSLR_OK )
            {
                i4_num_rec++;
                /* print out record. */
                tsl_print_rec(&t_tsl_rec,(UINT16)i4_num_rec);
            }
            else
            {
                x_dbg_stmt("Read error: %d\n", i4_rc);
                break;
            }

            /* goto next record. */
            i4_rc = x_tsl_set_cursor(h_tsl, TSL_NEXT_REC);
            if ( i4_rc == TSLR_END_OF_ITERATION )
            {
                /* done. */
                break;
            }
        }
    }

    x_tsl_read_unlock(h_tsl);
    x_tsl_close(h_tsl);
 
    x_dbg_stmt("Total number of TSL recrods in handle %d are: %d\n",
               h_tsl, i4_num_rec);
    
    return;
}


/*------------------------------------------------------------------------
 * Name: tsl_find_rec_id_by_freq
 *
 * Description: This API search for TSL record id for a specific freq.
 *
 * Inputs:  
 *    ui2_TSL_id:     TSL Database ID 
 *
 *    ui4_freq        Specifies freq to look for
 *
 *    e_brdcst_type   Specifies type of broadcast for this freq,
 *                    BRDCST_TYPE_DVB, BRDCST_TYPE_ATSC, BRDCST_TYPE_Analog,
 *                    etc.
 *
 *    e_brdcst_type   Specifies the broadcast medium:
 *                    BRDCST_MEDIUM_DIG_TERRESTRIAL,
 *                    BRDCST_MEDIUM_DIG_CABLE,  etc.
 *
 * Outputs: -
 *
 *    pui2_tsl_rec_id
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
INT32   tsl_find_tsl_rec_id_by_freq
(
    UINT16             ui2_tsl_id,
    UINT32             ui4_freq,
    BRDCST_TYPE_T      e_brdcst_type,
    BRDCST_MEDIUM_T    e_brdcst_medium,
    UINT16*            pui2_tsl_rec_id
    
)
{
    HANDLE_T   h_tsl;
    UINT32     ui4_ver_id ;
    UINT32     i4_num_rec = 0;
    INT32      i4_rc;
    TSL_REC_T  t_tsl_rec;
    TSL_REC_T* pt_tsl_rec;
    

    /* open an handle to the TSL database. */
    if ( x_tsl_open(ui2_tsl_id, NULL, NULL, &h_tsl) != TSLR_OK )
    {
        x_dbg_stmt("Error: TSL Data ID %d is not valid.\n", ui2_tsl_id);
        return TSLR_FAIL;
    }
    
    ui4_ver_id = TSL_NULL_VER_ID;

    i4_rc = x_tsl_read_lock(h_tsl);
    
    i4_rc=x_tsl_sort_rec(h_tsl, &ui4_ver_id);

    if ( i4_rc == TSLR_OK )
    {
        i4_rc=x_tsl_set_cursor(h_tsl,TSL_FIRST_REC);

        while (i4_rc == TSLR_OK)
        {
            i4_rc = x_tsl_read_cursor(h_tsl, &t_tsl_rec, &ui4_ver_id);
            
            if ( i4_rc == TSLR_OK )
            {
                i4_num_rec++;

                pt_tsl_rec = &t_tsl_rec;
                

                /* compare frequency value. */
                switch(pt_tsl_rec->uheader.t_desc.e_bcst_type)
                {
                    case BRDCST_TYPE_ANALOG :
                    {

                    }
                    break;

                    case BRDCST_TYPE_DVB :
                    {
                        if ( pt_tsl_rec->uheader.t_desc.e_bcst_medium ==
                             BRDCST_MEDIUM_DIG_TERRESTRIAL )
                        {
                            TUNER_TER_DIG_TUNE_INFO_T*   pt_ter_dig;
                            pt_ter_dig=&(pt_tsl_rec->udata.t_ter_dig);
                            if ( (pt_ter_dig->ui4_freq/1000) == ui4_freq )
                            {
                                x_dbg_stmt("<DVB terr> freq: [%d]\n",pt_ter_dig->ui4_freq);
                                *pui2_tsl_rec_id=pt_tsl_rec->ui2_tsl_rec_id;
                                i4_rc=TSLR_OK;
                                break;
                            }
                        }
                    }
                    break;

                    case BRDCST_TYPE_ATSC :
                    {
                        if ( pt_tsl_rec->uheader.t_desc.e_bcst_medium ==
                             BRDCST_MEDIUM_DIG_TERRESTRIAL )
                        {
                            TUNER_TER_DIG_TUNE_INFO_T*   pt_ter_dig;
                            pt_ter_dig=&(pt_tsl_rec->udata.t_ter_dig);
                            if ( (pt_ter_dig->ui4_freq/1000) == ui4_freq )
                            {
                                x_dbg_stmt("<ATSC terr> freq: [%d]\n",pt_ter_dig->ui4_freq);
                                *pui2_tsl_rec_id=pt_tsl_rec->ui2_tsl_rec_id;
                                i4_rc=TSLR_OK;
                                break;
                            }
                        }
                    }
                    break;

                    case BRDCST_TYPE_UNKNOWN :
                    {
            
                    }
                    break;

                    default:
                    {
                        
                    }
                    break;
                }
            }
            else
            {
                x_dbg_stmt("Read error: %d\n", i4_rc);
                break;
            }

            /* goto next record. */
            i4_rc = x_tsl_set_cursor(h_tsl, TSL_NEXT_REC);
            if ( i4_rc == TSLR_END_OF_ITERATION )
            {
                /* done. */
                break;
            }
        }
    }

    x_tsl_read_unlock(h_tsl);
    x_tsl_close(h_tsl);
 
    x_dbg_stmt("Total number of TSL recrods in handle %d are: %d\n",
               h_tsl, i4_num_rec);
    
    return i4_rc;
}



/*------------------------------------------------------------------------
 * Name: svl_find_svc_rec_by_tsl_info
 *
 * Description: This find SVL record from TS info.
 *
 * Inputs:  
 *    ui2_svl_id:     SVL Database ID 
 *
 *    ui2_tsl_id:     TSL Database ID
 *
 *    ui2_tsl_rec_id:  TSL record id in that database.
 *
 *
 *
 * Outputs: -
 *
 *    pui2_tsl_rec_id
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
extern VOID   svl_print_rec(SVL_REC_T*  pt_svl_rec, UINT16  ui2_idx);

INT32   svl_find_svc_rec_by_tsl_info
(
    UINT16       ui2_svl_id,
    UINT16       ui2_tsl_id,
    UINT16       ui2_tsl_rec_id,
    SVL_REC_T*   pt_svl_rec
)
{
    INT32         i4_rc;
    UINT32        ui4_ver_id;
    HANDLE_T      h_svl;
    
    /* open a handle to the SVL database. */
    if ( x_svl_open(ui2_svl_id, NULL, NULL, &h_svl) != SVLR_OK )
    {
        x_dbg_stmt("Error: SVL ID %d is not valid.\n", ui2_svl_id);
        return SVLR_FAIL;
    }
    else
    {
        i4_rc = x_svl_get_rec_by_ts(h_svl,ui2_tsl_id,
                                    ui2_tsl_rec_id,
                                    0xFFFFFF,
                                    0,
                                    pt_svl_rec,
                                    &ui4_ver_id);
        if ( i4_rc == SVLR_OK )
        {
            svl_print_rec(pt_svl_rec, 0);
        }
        else
        {
            x_dbg_stmt("<SVL> no service record found for TSL info: %d %d\n",
                       ui2_tsl_id,ui2_tsl_rec_id);
        }
        x_svl_close(h_svl);
    }
    return i4_rc;
}

    
    
/*-------------------------------------------------------------------------
 * Name: tsl_modify_rec_value
 *
 * Description: This API modifies a TSL record entry for the
 *    specified tsl ID.
 *
 * Inputs:  
 *    ui2_TSL_id:     TSL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   tsl_modify_rec_value(UINT16    ui2_tsl_id,
                            UINT16 ui2_tsl_rec_id,
                            UINT16 ui2_nwl_id,
                            UINT16 ui2_nwl_rec_id)
{
    HANDLE_T   h_tsl;
    UINT32     ui4_ver_id ;
    TSL_REC_T  t_tsl_rec = {0}; /* try to fix Klocwork issue */

    if ( x_tsl_open(ui2_tsl_id, NULL, NULL, &h_tsl) != TSLR_OK )
    {
        x_dbg_stmt("Error: TSL ID %d is not valid.\n", ui2_tsl_id);
        return;
    }

    x_dbg_stmt("modified TSL record [%d] with new NWL data value: [%d %d]\n",
               ui2_tsl_rec_id, ui2_nwl_id, ui2_nwl_rec_id);
        
    ui4_ver_id = TSL_NULL_VER_ID;

    x_tsl_lock(h_tsl);

    x_tsl_get_rec(h_tsl,
                  ui2_tsl_rec_id,
                  &t_tsl_rec,
                  &ui4_ver_id);

    TS_DESCR_SET_NWL_ID(t_tsl_rec.uheader.t_desc, ui2_nwl_id);
    TS_DESCR_SET_NWL_REC_ID(t_tsl_rec.uheader.t_desc, ui2_nwl_rec_id);

    tsl_print_rec(&t_tsl_rec,1);

    x_tsl_update_rec(h_tsl, &t_tsl_rec, TRUE);
    

    x_tsl_unlock(h_tsl);
    x_tsl_close(h_tsl);
 
    x_dbg_stmt("TSL record %d updated. \n", ui2_tsl_rec_id);
    
    return;
}


/*-------------------------------------------------------------------------
 * Name: tsl_modify_rec_value
 *
 * Description: This API deletes a TSL record entry for the
 *    specified tsl ID.
 *
 * Inputs:  
 *    ui2_TSL_id:     TSL ID 
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   tsl_delete_rec(UINT16    ui2_tsl_id,
                      UINT16 ui2_tsl_rec_id)
{
    HANDLE_T   h_tsl;

    if ( x_tsl_open(ui2_tsl_id, NULL, NULL, &h_tsl) != TSLR_OK )
    {
        x_dbg_stmt("Error: TSL ID %d is not valid.\n", ui2_tsl_id);
        return;
    }

    x_dbg_stmt("deleted TSL record [%d] \n", ui2_tsl_rec_id);

    x_tsl_lock(h_tsl);

    x_tsl_del_rec(h_tsl,
                  ui2_tsl_rec_id);
    

    x_tsl_unlock(h_tsl);
    x_tsl_close(h_tsl);
 
    x_dbg_stmt("TSL record %d deleted. \n", ui2_tsl_rec_id);
    
    return;
}
