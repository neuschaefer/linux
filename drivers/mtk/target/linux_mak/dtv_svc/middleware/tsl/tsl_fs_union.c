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
 * $RCSfile: tsl_fs_union.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: This file contains endian conversion functions for
 *              the union data field in the TSL record.  In the TSL record,
 *              the data field is a UNION which contains currently 3 types
 *              of structure: TUNER_TER_DIG_TUNE_INFO_T,
 *              TUNER_CAB_DIG_TUNE_INFO_T, and TUNER_SAT_DIG_TUNE_INFO_T.
 *
 *              The conversion functions convert 1) structure fields into
 *              an array of bytes (record) for output to file, and 2)
 *              an array of bytes (record) to structures when reading
 *              from file.
 *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "cdb/cdb_api.h"
#include "tsl/tsl.h"
#include "tsl/_tsl.h"
#include "tsl/tsl_rec.h"
#include "util/x_end_cnvt.h"
#include "tsl/tsl_dbg.h"
/*-------------------------------------------------------------------------
            TSL data union field  endian conversion functions
 --------------------------------------------------------------------------*/
/****************************************************************
 * Name: tsl_union_dig_terr_to_file
 *
 * Description:  Convert TUNER_TER_DIG_TUNE_INFO_T structure into
 *               TSL record data field.
 *
 * Inputs: 
 *   pui1_src:    Pointer to an array of bytes representing
 *                TUNER_TER_DIG_TUNE_INFO_T structure.
 *   ui2_size:    size of buffer pointed to 'pui1_src' 
 *
 * Outputs:
 *   pui1_dest:   Points to an array of packed fields from
 *                TUNER_TER_DIG_TUNE_INFO_T
 *
 * Return:
 *   uint8*       Pointer location after last write operation into
 *                'pui1_dest'
 ****************************************************************/
static UINT8* tsl_union_dig_terr_to_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    INT32                       i4_enum;
    TUNER_TER_DIG_TUNE_INFO_T   t_tuner;
    UINT16                      ui2_dir;
    UINT16                      ui2_gain;
#if 1
    UINT16                      ui2_t2_sys_id;
#endif

    x_memcpy(&t_tuner, pui1_src, sizeof(TUNER_TER_DIG_TUNE_INFO_T));
    
    x_end_cnvt_to_big_32(&(t_tuner.ui4_freq),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_bandwidth;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;
    
    i4_enum=t_tuner.e_fec_outer;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_fec_inner;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_mod;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_hierarchy;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_hierarchy_priority;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_guard_int;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_trans_mode;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_timer_int;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;
    
    *((UINT8*)pui1_dest) = t_tuner.b_other_freq ;
    pui1_dest += 1;

    i4_enum=t_tuner.e_pol;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;
    
    i4_enum=t_tuner.e_conn_mode;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;
    
    ui2_dir=t_tuner.ui2_dir;
    x_end_cnvt_to_big_16(&(ui2_dir),1,pui1_dest);
    pui1_dest += 2;

    ui2_gain=t_tuner.ui2_gain;
    x_end_cnvt_to_big_16(&(ui2_gain),1,pui1_dest);
    pui1_dest += 2;

    *((UINT8*)pui1_dest) = t_tuner.b_no_disc_nfy ;
    pui1_dest += 1;
    
#ifdef DTMB_MANUAL_DEMOD_DETECT
	*pui1_dest++ = (UINT8) t_tuner.e_FecRate;
	*pui1_dest++ = (UINT8) t_tuner.e_CarrierMode;
	*pui1_dest++ = (UINT8) t_tuner.e_PnNumber;

    i4_enum=(INT32)t_tuner.ui4_NCO;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;
#endif

#if 1
    *pui1_dest++ = (UINT8) t_tuner.e_dvb_t_t2_mode;

    *((UINT8*)pui1_dest) = t_tuner.ui1_plp_id ;
    pui1_dest += 1;

    ui2_t2_sys_id = t_tuner.ui2_t2_sys_id;
    x_end_cnvt_to_big_16(&(ui2_t2_sys_id),1,pui1_dest);
    pui1_dest += 2;
#endif

    return pui1_dest ;
}

/**************************************************************
 * Name: tsl_union_dig_terr_from_file
 *
 * Description: Convert TSL record field into TUNER_TER_DIG_TUNE_INFO_T
 *              structure.
 ***************************************************************/
static UINT8* tsl_union_dig_terr_from_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    INT32                       i4_enum;
    UINT16                      ui2_dir;
    UINT16                      ui2_gain;
#if 1
    UINT16                      ui2_t2_sys_id;
#endif
    TUNER_TER_DIG_TUNE_INFO_T   t_tuner;
    x_memset(&t_tuner, 0, sizeof(t_tuner));

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_freq));
    pui1_src += 4;
    
    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_bandwidth = (TUNER_BANDWIDTH_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_fec_outer = (TUNER_FEC_OUTER_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_fec_inner = (TUNER_FEC_INNER_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_mod = (TUNER_MODULATION_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_hierarchy = (TUNER_HIERARCHY_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_hierarchy_priority = (TUNER_HIERARCHY_PRIORITY_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_guard_int = (TUNER_GUARD_INTERVAL_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_trans_mode = (TUNER_TRANSMISSION_MODE_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_timer_int = (TUNER_TIMER_INTERLEAVING_T) i4_enum;
    pui1_src += 4;
    
    t_tuner.b_other_freq = *((UINT8*)pui1_src);
    pui1_src += 1;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_pol = (TUNER_POLARIZATION_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_conn_mode = (TUNER_CONNECTION_MODE_T) i4_enum;
    pui1_src += 4;
    
    x_end_cnvt_from_big_16(pui1_src,1,&(ui2_dir));
    t_tuner.ui2_dir = ui2_dir ;
    pui1_src += 2;
    
    x_end_cnvt_from_big_16(pui1_src,1,&(ui2_gain));
    t_tuner.ui2_gain = ui2_gain;
    pui1_src += 2;

    t_tuner.b_no_disc_nfy = *((UINT8*)pui1_src);
    pui1_src += 1;
    
#ifdef DTMB_MANUAL_DEMOD_DETECT
	t_tuner.e_FecRate         = (TUNER_CODRRATE_T)      *pui1_src++;
	t_tuner.e_CarrierMode     = (TUNER_CARRIER_MODE_T)  *pui1_src++;
	t_tuner.e_PnNumber        = (TUNER_PN_SEQ_T)        *pui1_src++;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.ui4_NCO = (UINT32)i4_enum;
    pui1_src += 4;
#endif

#if 1
    t_tuner.e_dvb_t_t2_mode = (TUNER_DVB_T_T2_MODE_T)*pui1_src++;        

    t_tuner.ui1_plp_id = *((UINT8*)pui1_src);
    pui1_src += 1;

    x_end_cnvt_from_big_16(pui1_src,1,&(ui2_t2_sys_id));
    t_tuner.ui2_t2_sys_id = ui2_t2_sys_id;
    pui1_src += 2;
#endif

    x_memcpy(pui1_dest, &t_tuner, sizeof(TUNER_TER_DIG_TUNE_INFO_T) );
    
    return pui1_src;
}

#ifdef  MW_ISDB_SUPPORT
/****************************************************************
 * Name: tsl_union_dig_isdb_terr_to_file
 *
 * Description:  Convert ISDB_TER_DIG_INFO_T structure into
 *               TSL record data field.
 *
 * Inputs: 
 *   pui1_src:    Pointer to an array of bytes representing
 *                ISDB_TER_DIG_INFO_T structure.
 *   ui2_size:    size of buffer pointed to 'pui1_src' 
 *
 * Outputs:
 *   pui1_dest:   Points to an array of packed fields from
 *                TUNER_TER_DIG_TUNE_INFO_T
 *
 * Returns:
 *   UINT8*       address after the last processed input data from
 *                'pui1_src'.
 ****************************************************************/
static UINT8* tsl_union_dig_isdb_terr_to_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    ISDB_TER_DIG_INFO_T         t_isdb_dig;
    UINT16                      ui2_prim_svc;
    UINT32                      ui4_mask;

    x_memcpy(&t_isdb_dig, pui1_src, sizeof(ISDB_TER_DIG_INFO_T));

    /* ISDB_TER_DIG_INFO_T consist of TUNER_TER_DIG_TUNE_INFO_T
       component and ISDB specific fields. For TUNER_TER_DIG_TUNE_INFO_T
       part, we use the existing 'to_file' conversion function.
    */
    pui1_dest = tsl_union_dig_terr_to_file(
        ((UINT8*)&(t_isdb_dig.t_ter_dig)),
        sizeof(TUNER_TER_DIG_TUNE_INFO_T),
        pui1_dest);

    ui4_mask = t_isdb_dig.ui4_mask;
    x_end_cnvt_to_big_32(&(ui4_mask),1,pui1_dest);
    pui1_dest += 4;
    
    *((UINT8*)pui1_dest) = t_isdb_dig.ui1_rc_key_id;
    pui1_dest += 1;

    *((UINT8*)pui1_dest) = t_isdb_dig.ui1_num_layers;
    pui1_dest += 1;

    ui2_prim_svc =t_isdb_dig.ui2_prim_svc;
    x_end_cnvt_to_big_16(&(ui2_prim_svc),1,pui1_dest);
    pui1_dest += 2;

    /* convert INT32 array to big-endian. */
    x_end_cnvt_to_big_32(&(t_isdb_dig.ai4_pr_svc_id[0]), 8, pui1_dest);
    pui1_dest += (8*sizeof(INT32));

    x_memcpy(pui1_dest, &(t_isdb_dig.ac_ts_name[0]), ISDB_TSL_NAME_LEN+1);
    pui1_dest += (ISDB_TSL_NAME_LEN+1);
    
    return pui1_dest;
}

/**************************************************************
 * Name: tsl_union_dig_isdb_terr_from_file
 *
 * Description: Convert TSL record field into ISDB_TER_DIG_INFO_T
 *              structure.
 *
 * Inputs: 
 *   pui1_src:    Pointer to an array of bytes representing
 *                'ISDB_TER_DIG_INFO_T' data readedd from file.
 *
 *   ui2_size:    size of buffer pointed to 'pui1_src' 
 *
 * Outputs:
 *   pui1_dest:   Points to an 'ISDB_TER_DIG_INFO_T' structure.
 *
 * Returns:
 *   UINT8*       address after the last processed input data from
 *                'pui1_src'.
 ***************************************************************/
static UINT8* tsl_union_dig_isdb_terr_from_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    UINT16                      ui2_prim_svc;
    ISDB_TER_DIG_INFO_T         t_isdb_dig;
    UINT32                      ui4_mask;

    /* process the Digital tuner data from file. */
    pui1_src = tsl_union_dig_terr_from_file(pui1_src, ui2_size,
                                            ((UINT8*)&(t_isdb_dig.t_ter_dig))
        );

    /* process the ISDB specific fields. */
    x_end_cnvt_from_big_32(pui1_src,1,&(ui4_mask));
    t_isdb_dig.ui4_mask = ui4_mask;
    pui1_src += 4;
    
    t_isdb_dig.ui1_rc_key_id = *((UINT8*)pui1_src);
    pui1_src += 1;
    
    t_isdb_dig.ui1_num_layers = *((UINT8*)pui1_src);
    pui1_src += 1;
    
    x_end_cnvt_from_big_16(pui1_src,1,&(ui2_prim_svc));
    t_isdb_dig.ui2_prim_svc = ui2_prim_svc;
    pui1_src += 2;

    x_end_cnvt_from_big_32(pui1_src, 8, &(t_isdb_dig.ai4_pr_svc_id[0]));
    pui1_src += (8*sizeof(INT32));
    
    x_memcpy(&(t_isdb_dig.ac_ts_name[0]), pui1_src, ISDB_TSL_NAME_LEN+1);
    pui1_src += (ISDB_TSL_NAME_LEN+1);

    /*
       Now 't_isdb_dig' structure contain all the fields loaded from
       file, copy it to the output buffer.
    */
    x_memcpy(pui1_dest, &t_isdb_dig, sizeof(ISDB_TER_DIG_INFO_T) );
   
    return pui1_src;
}
#endif

/*********************************************************************
 * Name: tsl_union_dig_cable_to_file
 *
 * Description:  Convert TUNER_CAB_DIG_TUNE_INFO_T structure into
 *               TSL record data field.
 *
 * Inputs: 
 *   pui1_src:    Pointer to a TUNER_CAB_DIG_TUNE_INFO_T structure.
 *   ui2_size:    size of buffer pointed to 'pui1_src' 
 *
 *********************************************************************/
static VOID tsl_union_dig_cable_to_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    INT32                       i4_enum;
    TUNER_CAB_DIG_TUNE_INFO_T   t_tuner;

    x_memcpy(&t_tuner, pui1_src, sizeof(TUNER_CAB_DIG_TUNE_INFO_T));
    
    x_end_cnvt_to_big_32(&(t_tuner.ui4_freq),1,pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_sym_rate),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_fec_outer;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_fec_inner;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_mod;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_conn_mode;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    *((UINT8*)pui1_dest) = t_tuner.b_no_disc_nfy ;
    pui1_dest += 1;
    
    return;
}

/******************************************************************
 * Name: tsl_union_dig_cable_from_file
 *
 * Description:  Convert TSL record data field to
 *               TUNER_CAB_DIG_TUNE_INFO_T structure into
 *               .
 *
 * Inputs: 
 *   pui1_src:    Pointer to a TUNER_CAB_DIG_TUNE_INFO_T structure.
 *   ui2_size:    size of buffer pointed to 'pui1_src' 
 *
 ******************************************************************/
static VOID tsl_union_dig_cable_from_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    INT32                       i4_enum;
    TUNER_CAB_DIG_TUNE_INFO_T   t_tuner;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_freq));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_sym_rate));
    pui1_src += 4;
    
    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_fec_outer = (TUNER_FEC_OUTER_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_fec_inner = (TUNER_FEC_INNER_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_mod = (TUNER_MODULATION_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_conn_mode = (TUNER_CONNECTION_MODE_T) i4_enum;
    pui1_src += 4;
    
    t_tuner.b_no_disc_nfy = *((UINT8*)pui1_src) ;
    pui1_src += 1;
    
    x_memcpy(pui1_dest, &t_tuner, sizeof(TUNER_CAB_DIG_TUNE_INFO_T) );

    return;
}

/*********************************************************************
 * Name: tsl_union_dig_sat_to_file
 *
 * Description:  Convert TUNER_SAT_DIG_TUNE_INFO_T structure into
 *               TSL record data field.
 *
 * Inputs: 
 *   pui1_src:    Pointer to a TUNER_SAT_DIG_TUNE_INFO_T structure.
 *   ui2_size:    size of buffer pointed to 'pui1_src' 
 *
 *********************************************************************/
static VOID tsl_union_dig_sat_to_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    INT32                       i4_enum;
    TUNER_SAT_DIG_TUNE_INFO_T   t_tuner;

    x_memcpy(&t_tuner, pui1_src, sizeof(TUNER_SAT_DIG_TUNE_INFO_T));
    
    x_end_cnvt_to_big_32(&(t_tuner.ui4_freq),1,pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_sym_rate),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_fec_inner;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_pol;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_mod;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_conn_mode;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;
    
    x_end_cnvt_to_big_16(&(t_tuner.i2_orb_pos),1,pui1_dest);
    pui1_dest += 2;

    *((UINT8*)pui1_dest) = t_tuner.b_no_disc_nfy ;
    pui1_dest += 1;

    return;
}

/******************************************************************
 * Name: tsl_union_dig_sat_from_file
 *
 * Description:  Convert TSL record data field to
 *               TUNER_SAT_DIG_TUNE_INFO_T structure into
 *               .
 *
 * Inputs: 
 *   pui1_src:    Pointer to a TUNER_SAT_DIG_TUNE_INFO_T structure.
 *   ui2_size:    size of buffer pointed to 'pui1_src' 
 *
 ******************************************************************/
static VOID tsl_union_dig_sat_from_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    INT32                       i4_enum;
    TUNER_SAT_DIG_TUNE_INFO_T   t_tuner;
    x_memset(&t_tuner, 0, sizeof(t_tuner));
    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_freq));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_sym_rate));
    pui1_src += 4;
    
    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_fec_inner = (TUNER_FEC_INNER_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_pol = (TUNER_POLARIZATION_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_mod = (TUNER_MODULATION_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_conn_mode = (TUNER_CONNECTION_MODE_T) i4_enum;
    pui1_src += 4;
    
    x_end_cnvt_from_big_16(pui1_src,1,&(t_tuner.i2_orb_pos));
    pui1_src += 2;

    t_tuner.b_no_disc_nfy = *((UINT8*)pui1_src) ;
    pui1_src += 1;
    
    x_memcpy(pui1_dest, &t_tuner, sizeof(TUNER_SAT_DIG_TUNE_INFO_T) );

    return;
}

/****************************************************************
 * Name: tsl_union_ana_terr_to_file
 *
 * Description:  Convert TUNER_TER_ANA_TUNE_INFO_T structure into
 *               TSL record data field.
 *
 * Inputs: 
 *   pui1_src:    Pointer to a TUNER_TER_ANA_TUNE_INFO_T
 *   ui2_size:    size of buffer pointed to 'pui1_src' 
 *
 * Outputs:
 *   pui1_dest:   Points to an array of packed fields from
 *                TUNER_TER_ANA_TUNE_INFO_T
 ****************************************************************/
static VOID tsl_union_ana_terr_to_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    INT32                       i4_enum;
    TUNER_TER_ANA_TUNE_INFO_T   t_tuner;

    x_memcpy(&t_tuner, pui1_src, sizeof(TUNER_TER_ANA_TUNE_INFO_T));
    
    x_end_cnvt_to_big_32(&(t_tuner.ui4_freq),1,pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_freq_bound),1,pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_central_freq),1,pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_freq_offset),1,pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_tv_sys_mask),1,pui1_dest);
    pui1_dest += 4;
    
    i4_enum=t_tuner.e_pol;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_mod;
    x_end_cnvt_to_big_32(&(i4_enum),1,pui1_dest);
    pui1_dest += 4;
    
    x_end_cnvt_to_big_16(&(t_tuner.ui2_dir),1,pui1_dest);
    pui1_dest += 2;

    x_end_cnvt_to_big_16(&(t_tuner.ui2_gain),1,pui1_dest);
    pui1_dest += 2;

    *((UINT8*)pui1_dest) = t_tuner.b_fine_tune;
    pui1_dest += 1;
    
    *((UINT8*)pui1_dest) = t_tuner.b_no_disc_nfy ;
    pui1_dest += 1;
    
    return;
}

/**************************************************************
 * Name: tsl_union_ana_terr_from_file
 *
 * Description: Convert TSL record field into TUNER_TER_ANA_TUNE_INFO_T
 *              structure.
 * Inputs: 
 *   pui1_src:  Points to an array of packed fields from TSL record.
 *   ui2_size:  Size of buffer pointed to 'pui1_src' 
 *
 * Outputs:
 *   pui1_dest: Pointer to a TUNER_TER_ANA_TUNE_INFO_T structure. 
 ***************************************************************/
static VOID tsl_union_ana_terr_from_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    INT32                       i4_enum;
    TUNER_TER_ANA_TUNE_INFO_T   t_tuner;
    x_memset(&t_tuner, 0, sizeof(t_tuner));
    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_freq));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_freq_bound));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_central_freq));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_freq_offset));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_tv_sys_mask));
    pui1_src += 4;
    
    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_pol = (TUNER_POLARIZATION_T) i4_enum;
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(i4_enum));
    t_tuner.e_mod = (TUNER_MODULATION_T) i4_enum ;
    pui1_src += 4;

    x_end_cnvt_from_big_16(pui1_src,1,&(t_tuner.ui2_dir));
    pui1_src += 2;

    x_end_cnvt_from_big_16(pui1_src,1,&(t_tuner.ui2_gain));
    pui1_src += 2;

    t_tuner.b_fine_tune = *((UINT8*)pui1_src);
    pui1_src += 1;

    t_tuner.b_no_disc_nfy = *((UINT8*)pui1_src);
    pui1_src += 1;
    
    x_memcpy(pui1_dest, &t_tuner, sizeof(TUNER_TER_ANA_TUNE_INFO_T) );
    
    return;
}


/*********************************************************************
 * Name: tsl_union_ana_cable_to_file
 *
 * Description:  Convert TUNER_CAB_ANA_TUNE_INFO_T structure into
 *               TSL record data field.
 *
 * Inputs: 
 *   pui1_src:    Pointer to a TUNER_CAB_ANA_TUNE_INFO_T structure.
 *   ui2_size:    size of buffer pointed to 'pui1_src' 
 *
 * Outputs:
 *   pui1_dest:   Points to an array of packed fields from
 *                TUNER_CAB_ANA_TUNE_INFO_T
 *********************************************************************/
static VOID tsl_union_ana_cable_to_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    INT32                        i4_enum;
    TUNER_CAB_ANA_TUNE_INFO_T    t_tuner;
    
    x_memcpy(&t_tuner, pui1_src, sizeof(TUNER_CAB_ANA_TUNE_INFO_T));
    
    x_end_cnvt_to_big_32(&(t_tuner.ui4_freq), 1, pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_freq_bound), 1, pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_central_freq), 1, pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_freq_offset), 1, pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_tv_sys_mask), 1, pui1_dest);
    pui1_dest += 4;

    i4_enum=t_tuner.e_mod;
    x_end_cnvt_to_big_32(&(i4_enum), 1, pui1_dest);
    pui1_dest += 4 ;

    *((UINT8*)pui1_dest) = t_tuner.b_fine_tune;
    pui1_dest += 1;
    
    *((UINT8*)pui1_dest) = t_tuner.b_no_disc_nfy;
    pui1_dest += 1;
    
    return;
}

/******************************************************************
 * Name: tsl_union_ana_cable_from_file
 *
 * Description:  Convert TSL record data field to
 *               TUNER_CAB_ANA_TUNE_INFO_T structure.
 *
 * Inputs: 
 *   pui1_src:   Points to an array of packed fields from TSL record.
 *   ui2_size:   size of buffer pointed to 'pui1_src' 
 *
 * Outputs:
 *   pui1_dest:  Pointer to a TUNER_CAB_ANA_TUNE_INFO_T structure. 
 ******************************************************************/
static VOID tsl_union_ana_cable_from_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    INT32                       i4_enum;
    TUNER_CAB_ANA_TUNE_INFO_T   t_tuner;
    x_memset(&t_tuner, 0, sizeof(t_tuner));

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_freq));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_freq_bound));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_central_freq));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_freq_offset));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_tv_sys_mask));
    pui1_src += 4;
    
    x_end_cnvt_from_big_32(pui1_src, 1, &(i4_enum));
    t_tuner.e_mod = (TUNER_MODULATION_T) i4_enum;
    pui1_src += 4;

    t_tuner.b_fine_tune = *((UINT8*) pui1_src);
    pui1_src += 1;
    
    t_tuner.b_no_disc_nfy = *((UINT8*) pui1_src);
    pui1_src += 1;
    
    x_memcpy(pui1_dest, &t_tuner, sizeof(TUNER_CAB_ANA_TUNE_INFO_T) );

    return;
}


/*********************************************************************
 * Name: tsl_union_ana_sat_to_file
 *
 * Description:  Convert TUNER_SAT_ANA_TUNE_INFO_T structure into
 *               TSL record data field.
 *
 * Inputs: 
 *   pui1_src:    Pointer to a TUNER_SAT_ANA_TUNE_INFO_T structure.
 *   ui2_size:    size of buffer pointed to 'pui1_src' 
 *
* Outputs:
 *   pui1_dest:   Points to an array of packed fields from
 *                TUNER_CAB_ANA_TUNE_INFO_T
 *********************************************************************/
static VOID tsl_union_ana_sat_to_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    TUNER_SAT_ANA_TUNE_INFO_T    t_tuner;

    x_memcpy(&t_tuner, pui1_src, sizeof(TUNER_SAT_ANA_TUNE_INFO_T));
    
    x_end_cnvt_to_big_32(&(t_tuner.ui4_freq),1,pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_freq_bound),1,pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_central_freq),1,pui1_dest);
    pui1_dest += 4;

    x_end_cnvt_to_big_32(&(t_tuner.ui4_freq_offset),1,pui1_dest);
    pui1_dest += 4;

    *((UINT8*)pui1_dest) = t_tuner.b_no_disc_nfy ;
    pui1_dest += 1;

    return;
}

/******************************************************************
 * Name: tsl_union_ana_sat_from_file
 *
 * Description:  Convert TSL record data field to
 *               TUNER_SAT_ANA_TUNE_INFO_T structure into
 *  
 * Inputs: 
 *   pui1_src:  Points to an array of packed fields from TSL record.
 *   ui2_size:  size of buffer pointed to 'pui1_src'.
 *
 * Outputs:
 *   pui1_dest: Pointer to a TUNER_SAT_ANA_TUNE_INFO_T structure.
 ******************************************************************/
static VOID tsl_union_ana_sat_from_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    TUNER_SAT_ANA_TUNE_INFO_T    t_tuner;
    x_memset(&t_tuner, 0, sizeof(t_tuner));

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_freq));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_freq_bound));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_central_freq));
    pui1_src += 4;

    x_end_cnvt_from_big_32(pui1_src,1,&(t_tuner.ui4_freq_offset));
    pui1_src += 4;

    t_tuner.b_no_disc_nfy = *((UINT8*) pui1_src);
    pui1_src += 1;
    
    x_memcpy(pui1_dest, &t_tuner, sizeof(TUNER_SAT_ANA_TUNE_INFO_T) );
        
    return;
}


/*********************************************************************
 * Name: alt_tun_info_dig_terr_to_file
 *
 * Description:  Convert ALTERNATE_TUNE_INFO_T structure into
 *               TSL record data field.
 *
 * Inputs: 
 *   pui1_src:    Pointer to a ALTERNATE_TUNE_INFO_T structure.
 *   ui2_size:    size of buffer pointed to 'pui1_src' 
 *
* Outputs:
 *   pui1_dest:   Points to an array of packed fields from
 *                ALTERNATE_TUNE_INFO_T
 *********************************************************************/
static VOID alt_tun_info_dig_terr_to_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    INT32                     i4_i;
    UINT32                    ui4_freq;
    UINT32                    ui4_bandwidth;
    TIME_T                    t_loss_event_time;
    UINT8                     ui1_freq_updated;
    ALTERNATE_TUNE_INFO_T     t_alt_ter_dig;

    x_memcpy(&t_alt_ter_dig, pui1_src, sizeof(ALTERNATE_TUNE_INFO_T));

    for ( i4_i = 0 ;  i4_i < MAX_NB_ALTERNATE_FREQ ; i4_i++ )
    {
        ui4_freq = t_alt_ter_dig.t_all_freq[i4_i].ui4_freq;
        x_end_cnvt_to_big_32(&(ui4_freq),1,pui1_dest);
        pui1_dest += 4;

        /* To be consistent for size of enum variable, enum
           values are saved as UINT32.
        */
        ui4_bandwidth = t_alt_ter_dig.t_all_freq[i4_i].e_bandwidth;
        x_end_cnvt_to_big_32(&(ui4_bandwidth),1,pui1_dest);
        pui1_dest += 4;
    }

    ui4_freq = t_alt_ter_dig.t_orig_freq.ui4_freq;
    x_end_cnvt_to_big_32(&(ui4_freq),1,pui1_dest);
    pui1_dest += 4;

    /* To be consistent for size of enum variable, enum
       values are saved as UINT32.
    */
    ui4_bandwidth = t_alt_ter_dig.t_orig_freq.e_bandwidth;
    x_end_cnvt_to_big_32(&(ui4_bandwidth),1,pui1_dest);
    pui1_dest += 4;

    t_loss_event_time = t_alt_ter_dig.t_loss_event_time;
    x_end_cnvt_to_big_64(&(t_loss_event_time),1,pui1_dest);
    pui1_dest += 8;

    /* Boolean value is only one byte, so no endian conversion is needed */
    ui1_freq_updated = t_alt_ter_dig.b_freq_updated;
    *pui1_dest = ui1_freq_updated;
    pui1_dest += 1;

    return;
}

/******************************************************************
 * Name: alt_tun_info_dig_terr_from_file
 *
 * Description:  Convert TSL record data field to
 *               ALTERNATE_TUNE_INFO_T structure into
 *  
 * Inputs: 
 *   pui1_src:  Points to an array of packed fields from TSL record.
 *   ui2_size:  size of buffer pointed to 'pui1_src'.
 *
 * Outputs:
 *   pui1_dest: Pointer to a ALTERNATE_TUNE_INFO_T structure.
 ******************************************************************/
static VOID alt_tun_info_dig_terr_from_file
(
    UINT8* pui1_src,
    UINT16 ui2_size,
    UINT8* pui1_dest
)
{
    INT32                     i4_i;
    UINT32                    ui4_freq;
    UINT32                    ui4_bandwidth;
    TIME_T                    t_loss_event_time;
    UINT8                     ui1_freq_updated;
    ALTERNATE_TUNE_INFO_T     t_alt_ter_dig;

    x_memset(&t_alt_ter_dig,0,sizeof(ALTERNATE_TUNE_INFO_T));
    
    for (i4_i = 0 ;  i4_i < MAX_NB_ALTERNATE_FREQ ; i4_i++ )
    {
        x_end_cnvt_from_big_32(pui1_src,1,&(ui4_freq));
        pui1_src += 4;
        t_alt_ter_dig.t_all_freq[i4_i].ui4_freq = ui4_freq;

        /* Store bandwidth value is UINT32, read back into
           an enum value. 
        */
        x_end_cnvt_from_big_32(pui1_src,1,&(ui4_bandwidth));
        pui1_src += 4;
        t_alt_ter_dig.t_all_freq[i4_i].e_bandwidth = (TUNER_BANDWIDTH_T) ui4_bandwidth;
    }

    x_end_cnvt_from_big_32(pui1_src,1,&(ui4_freq));
    pui1_src += 4;
    t_alt_ter_dig.t_orig_freq.ui4_freq = ui4_freq;
   
    /* Store bandwidth value is UINT32, read back into
       an enum value.
    */ 
    x_end_cnvt_from_big_32(pui1_src,1,&(ui4_bandwidth));
    pui1_src += 4;
    t_alt_ter_dig.t_orig_freq.e_bandwidth = (TUNER_BANDWIDTH_T) ui4_bandwidth;
    
    x_end_cnvt_from_big_64(pui1_src,1,&(t_loss_event_time));
    pui1_src += 8;
    t_alt_ter_dig.t_loss_event_time = t_loss_event_time;

    /* Boolean value is only one byte, so no endian conversion is needed */
    ui1_freq_updated = *pui1_src;
    pui1_src += 1;
    t_alt_ter_dig.b_freq_updated = ui1_freq_updated;
    
    x_memcpy(pui1_dest, &(t_alt_ter_dig), sizeof(ALTERNATE_TUNE_INFO_T) );
        
    return;
}

/*----------------------------------------------------------------------
 * Name: tsl_union_end_conv_from_file
 *
 * Description: This API interpret the TSL record from persistent storage
 *              and perform the necessary conversion to the
 *              structure fields.
 *
 * Inputs: 
 *   
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           file handle is successfully closed. 
 *   
 -----------------------------------------------------------------------*/
VOID tsl_union_end_conv_from_file
(
    DB_FIELD_T       t_fld_type,
    DB_UNION_TAG_T   t_union_tag,
    const VOID*      pv_input,
    UINT16           ui2_size,
    VOID*            pv_output
)
{
    BRDCST_MEDIUM_T  t_bcst_medium;

    UINT8*           pui1_src;
    UINT8*           pui1_dest;

    pui1_src  = (UINT8*) pv_input;
    pui1_dest = (UINT8*)pv_output;

    /*
    DBG_INFO(("{TSL} File to TSL memory conversion\n"));
    */
    
    switch (t_fld_type)
    {
        case TSL_CDB_TYPE_HEADER:
        {
            /*
            DBG_INFO(("{TSL} TSL header: size: %d tag value: %d\n",
                       ui2_size, t_union_tag));
            */
            
            /*
              convert the TS_DESCR_T structure.
            */

            /*
              brdcast type
            */
            *((UINT8*)pui1_dest) = *((UINT8*)pui1_src) ;
            pui1_dest += 1;
            pui1_src  += 1;
            
            /*
              brdcast medium
            */
            *((UINT8*)pui1_dest) = *((UINT8*)pui1_src) ;
            pui1_dest += 1;
            pui1_src  += 1;

            /*
              This follows by 4 UINT16 fields:
              1) some form of network identifier.
              2) original network ID.
              3) transport stream ID.
              4) reserve for future use.
            */
            x_end_cnvt_from_big_16(pui1_src, 4, pui1_dest);
            pui1_dest += 8;
            pui1_src  += 8;
            
            /*
              one-byte, reserve for funture used.
            */
            *((UINT8*)pui1_dest) = *((UINT8*)pui1_src) ;
            pui1_dest += 1;
            pui1_src  += 1;

            /*
              one-byte, flag value.
            */
            *((UINT8*)pui1_dest) = *((UINT8*)pui1_src) ;
            pui1_dest += 1;
            pui1_src  += 1;
        }
        break;

        case TSL_CDB_TYPE_DATA:
        {
            /*
            DBG_INFO(("{TSL} TSL_data type (union tag): %x ", t_union_tag));
            */
            
            t_bcst_medium = (BRDCST_MEDIUM_T) TSL_GET_UTAG_BRDCST_MEDIUM(t_union_tag);

            if ( t_bcst_medium == BRDCST_MEDIUM_DIG_TERRESTRIAL )
            {
                #ifndef MW_ISDB_SUPPORT
                tsl_union_dig_terr_from_file(pui1_src, ui2_size, pui1_dest);
                #else
                tsl_union_dig_isdb_terr_from_file(pui1_src, ui2_size, pui1_dest);
                #endif
                
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_DIG_CABLE )
            {
                tsl_union_dig_cable_from_file(pui1_src, ui2_size, pui1_dest);
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_DIG_SATELLITE )
            {
                tsl_union_dig_sat_from_file(pui1_src, ui2_size, pui1_dest);
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_ANA_TERRESTRIAL )
            {
                tsl_union_ana_terr_from_file(pui1_src, ui2_size, pui1_dest);
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_ANA_CABLE )
            {
                tsl_union_ana_cable_from_file(pui1_src, ui2_size, pui1_dest);
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_ANA_SATELLITE )
            {
                tsl_union_ana_sat_from_file(pui1_src, ui2_size, pui1_dest);
            }
            else
            {
                /* no conversion. */
                x_memcpy(pui1_dest, pui1_src, ui2_size);
            }    
        }
        break;

        case TSL_CDB_TYPE_ALT_TUNE_INFO_DATA:
        {
            /*
            DBG_INFO(("{TSL} TSL_data type (union tag): %x ", t_union_tag));
            */
            
            t_bcst_medium = (BRDCST_MEDIUM_T) TSL_GET_UTAG_BRDCST_MEDIUM(t_union_tag);

            /* currently, alternate tuning support is only for Digital
               terrestrial broadcast. */
            if ( t_bcst_medium == BRDCST_MEDIUM_DIG_TERRESTRIAL )
            {
                alt_tun_info_dig_terr_from_file(pui1_src, ui2_size, pui1_dest);
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_DIG_CABLE )
            {
                
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_DIG_SATELLITE )
            {
                
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_ANA_TERRESTRIAL )
            {
                
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_ANA_CABLE )
            {
                
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_ANA_SATELLITE )
            {
                
            }
            else
            {
                /* no conversion. */
                x_memcpy(pui1_dest, pui1_src, ui2_size);
            }    
        }
        break;

        case TSL_CDB_TYPE_REC_REFERENCE_DATA:
        {
            /*
            DBG_INFO(("{TSL} TSL_data type (union tag): %x ", t_union_tag));
            */
            
            /*
              32-bit mask
            */
            x_end_cnvt_from_big_32(pui1_src, 1, pui1_dest);
            pui1_dest += 4;
            pui1_src  += 4;

            /*
              NWL/SATL ID/rec ID
            */
            x_end_cnvt_from_big_16(pui1_src, 4, pui1_dest);
            pui1_dest += 8;
            pui1_src  += 8;
        }
        break;

        default:
        {
            DBG_API(("{TSL} Error: TSL union field type: %d",t_fld_type));
        }
        break ;
    }
    return;
}

/*----------------------------------------------------------------------
 * Name: tsl_union_end_conv_to_file
 *
 * Description: This API interpret the TSL structures
 *              (TUNER_CAB_DIG_TUNE_INFO_T, TUNER_TER_DIG_TUNE_INFO_T,
 *               TUNER_SAT_DIG_TUNE_INFO_T) and perform the necessary
 *              conversion to data record field.
 *
 * Inputs: 
 *   
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           file handle is successfully closed. 
 *   
 -----------------------------------------------------------------------*/
VOID tsl_union_end_conv_to_file
(
    DB_FIELD_T       t_fld_type,
    DB_UNION_TAG_T   t_union_tag,
    const VOID*      pv_input,
    UINT16           ui2_size,
    VOID*            pv_output
)
{
    BRDCST_MEDIUM_T  t_bcst_medium;

    UINT8*           pui1_src;
    UINT8*           pui1_dest;

    pui1_src  = (UINT8*) pv_input;
    pui1_dest = (UINT8*)pv_output;
      
    
    /*   
    DBG_INFO(("{TSL} TSL memory to File conversion\n"));
    */
    
    
    switch (t_fld_type)
    {
        case TSL_CDB_TYPE_HEADER:
        {
            
            /*
            DBG_INFO(("{TSL} TSL header: size: %d tag value: %d\n",
                       ui2_size, t_union_tag));
            */             
            
            /*
              convert the TS_DESCR_T structure.
            */

            /*
              brdcast type
            */
            *((UINT8*)pui1_dest) = *((UINT8*)pui1_src) ;
            pui1_dest += 1;
            pui1_src  += 1;
            
            /*
              brdcast medium
            */
            *((UINT8*)pui1_dest) = *((UINT8*)pui1_src) ;
            pui1_dest += 1;
            pui1_src  += 1;

            /*
              This follows by 4 UINT16 fields:
              1) some form of network identifier.
              2) original network ID.
              3) transport stream ID.
              4) reserve for future use.
            */
            x_end_cnvt_to_big_16(pui1_src, 4, pui1_dest);
            pui1_dest += 8;
            pui1_src  += 8;
            
            /*
              one-byte, reserve for funture used.
            */
            *((UINT8*)pui1_dest) = *((UINT8*)pui1_src) ;
            pui1_dest += 1;
            pui1_src  += 1;

            /*
              one-byte, flag value.
            */
            *((UINT8*)pui1_dest) = *((UINT8*)pui1_src) ;
            pui1_dest += 1;
            pui1_src  += 1;
        }
        break;

        case TSL_CDB_TYPE_DATA:
        {
            
            /*  
            DBG_INFO(("{TSL} TSL_data type (union tag): %x ", t_union_tag));
            */
                 
            t_bcst_medium = (BRDCST_MEDIUM_T) TSL_GET_UTAG_BRDCST_MEDIUM(t_union_tag);

            if ( t_bcst_medium == BRDCST_MEDIUM_DIG_TERRESTRIAL )
            {
                #ifndef MW_ISDB_SUPPORT
                tsl_union_dig_terr_to_file(pui1_src, ui2_size, pui1_dest);
                #else
                tsl_union_dig_isdb_terr_to_file(pui1_src, ui2_size, pui1_dest);
                #endif
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_DIG_CABLE )
            {
                tsl_union_dig_cable_to_file(pui1_src, ui2_size, pui1_dest);
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_DIG_SATELLITE )
            {
                tsl_union_dig_sat_to_file(pui1_src, ui2_size, pui1_dest);
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_ANA_TERRESTRIAL )
            {
                tsl_union_ana_terr_to_file(pui1_src, ui2_size, pui1_dest);
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_ANA_CABLE )
            {
                tsl_union_ana_cable_to_file(pui1_src, ui2_size, pui1_dest);
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_ANA_SATELLITE )
            {
                tsl_union_ana_sat_to_file(pui1_src, ui2_size, pui1_dest);
            }
            else
            {
                /* no conversion. */
                x_memcpy(pui1_dest, pui1_src, ui2_size);
            }    
        }
        break;

        case TSL_CDB_TYPE_ALT_TUNE_INFO_DATA:
        {
            /*  
            DBG_INFO(("{TSL} TSL_data type (union tag): %x ", t_union_tag));
            */
                 
            t_bcst_medium = (BRDCST_MEDIUM_T) TSL_GET_UTAG_BRDCST_MEDIUM(t_union_tag);

            /* currently, alternate tuning support is only for Digital
               terrestrial broadcast. */
            if ( t_bcst_medium == BRDCST_MEDIUM_DIG_TERRESTRIAL )
            {
                alt_tun_info_dig_terr_to_file(pui1_src, ui2_size, pui1_dest);
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_DIG_CABLE )
            {
                
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_DIG_SATELLITE )
            {
                
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_ANA_TERRESTRIAL )
            {
                
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_ANA_CABLE )
            {
                
            }
            else if ( t_bcst_medium == BRDCST_MEDIUM_ANA_SATELLITE )
            {
                
            }
            else
            {
                /* no conversion. */
                x_memcpy(pui1_dest, pui1_src, ui2_size);
            }    
        }
        break;

        case TSL_CDB_TYPE_REC_REFERENCE_DATA:
        {
            /*
            DBG_INFO(("{TSL} TSL_data type (union tag): %x ", t_union_tag));
            */
            
            /*
              32-bit mask
            */
            x_end_cnvt_to_big_32(pui1_src, 1, pui1_dest);
            pui1_dest += 4;
            pui1_src  += 4;

            /*
              NWL/SATL ID/rec ID
            */
            x_end_cnvt_to_big_16(pui1_src, 4, pui1_dest);
            pui1_dest += 8;
            pui1_src  += 8;
        }
        break;

        default:
        {
            DBG_API(("{TSL} Error: TSL record union field type: %d",t_fld_type));
        }
        break ;
    }
    return;
}
