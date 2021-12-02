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
 * $RCSfile: x_scte_si_eng.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: 0d036d6863c3fb51c7e6cad0171240c6 $
 *
 * Description:
 *         This header file contains SCTE-SI engine specific definitions,
 *         which are exported.
 *----------------------------------------------------------------------------*/

#ifndef _X_SCTE_SI_H_
#define _X_SCTE_SI_H_

/*-----------------------------------------------------------------------------
                    include files
 -----------------------------------------------------------------------------*/
#include "u_scte_si_eng.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 -----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 -----------------------------------------------------------------------------*/
extern INT32 x_scte_si_get_tbl_version(HANDLE_T   h_obj,
                                       UINT8*     pui1_tbl_version,
                                       TM_COND_T* pe_cond);

extern INT32 x_scte_si_get_prot_version(HANDLE_T   h_obj,
                                        UINT8*     pui1_prot_version,
                                        TM_COND_T* pe_cond);

extern INT32 x_scte_si_get_num_nit_entries(HANDLE_T   h_nit,
                                           UINT16*    pui2_num_entries,
                                           TM_COND_T* pe_cond);

extern INT32 x_scte_si_get_nit_cds_entry(HANDLE_T                h_nit_cds,
                                         UINT16                  ui2_idx,
                                         SCTE_SI_NIT_CDS_INFO_T* pt_nit_cds_info,
                                         TM_COND_T*              pe_cond);

extern INT32 x_scte_si_get_nit_mms_entry(HANDLE_T                h_nit_mms,
                                         UINT16                  ui2_idx,
                                         SCTE_SI_NIT_MMS_INFO_T* pt_nit_mms_info,
                                         TM_COND_T*              pe_cond);

extern INT32 x_scte_si_get_num_ntt_entries(HANDLE_T   h_ntt_sns,
                                           UINT16*    pui2_num_entries,
                                           TM_COND_T* pe_cond);
                                        
extern INT32 x_scte_si_get_ntt_sns_entry(HANDLE_T                h_ntt_sns,
                                         UINT16                  ui2_idx,
                                         SCTE_SI_NTT_SNS_INFO_T* pt_ntt_sns_info,
                                         TM_COND_T*              pe_cond);

extern INT32 x_scte_si_get_num_svct_entries(HANDLE_T   h_svct,
                                            UINT16*    pui2_num_entries,
                                            TM_COND_T* pe_cond);
                                     
extern INT32 x_scte_si_get_svct_vcm_entry(HANDLE_T                 h_svct_vcm,
                                          UINT16                   ui2_idx,
                                          SCTE_SI_SVCT_VCM_INFO_T* pt_svct_vcm_info,
                                          TM_COND_T*               pe_cond);

extern INT32 x_scte_si_get_svct_dcm_entry(HANDLE_T                 h_svct_dcm,
                                          UINT16                   ui2_idx,
                                          SCTE_SI_SVCT_DCM_INFO_T* pt_svct_dcm_info,
                                          TM_COND_T*               pe_cond);

extern INT32 x_scte_si_get_svct_icm_entry(HANDLE_T                 h_svct_icm,
                                          UINT16                   ui2_idx,
                                          SCTE_SI_SVCT_ICM_INFO_T* pt_svct_icm_info,
                                          TM_COND_T*               pe_cond);

extern INT32 x_scte_si_get_svct_vc_record(HANDLE_T                        h_svct_vcm,
                                          SCTE_SI_SVCT_SECOND_LOOP_IDX_T* pt_second_loop_idx,
                                          SCTE_SI_SVCT_VC_RECORD_T*       pt_svct_vc_record,
                                          TM_COND_T*                      pe_cond);

extern INT32 x_scte_si_get_svct_dcm_data(HANDLE_T                        h_svct_dcm,
                                         SCTE_SI_SVCT_SECOND_LOOP_IDX_T* pt_second_loop_idx,
                                         SCTE_SI_SVCT_DCM_DATA_T*        pt_svct_dcm_data_obj,
                                         TM_COND_T*                      pe_cond);
                                  
extern INT32 x_scte_si_get_svct_icm_record(HANDLE_T                        h_svct_icm,
                                           SCTE_SI_SVCT_SECOND_LOOP_IDX_T* pt_second_loop_idx,
                                           SCTE_SI_SVCT_ICM_RECORD_T*      pt_svct_icm_record,
                                           TM_COND_T*                      pe_cond);

extern INT32 x_scte_si_get_num_aeit_entries(HANDLE_T   h_aeit_src,
                                            UINT16*    pui2_num_entries,
                                            TM_COND_T* pe_cond);
                                     
extern INT32 x_scte_si_get_aeit_src_entry(HANDLE_T                 h_aeit_src,
                                          UINT16                   ui2_idx,
                                          SCTE_SI_AEIT_SRC_INFO_T* pt_aeit_src_info,
                                          TM_COND_T*               pe_cond);
                                   
extern INT32 x_scte_si_get_aeit_evt_entry(HANDLE_T                        h_aeit_src,
                                          SCTE_SI_AEIT_SECOND_LOOP_IDX_T* pt_second_loop_idx,
                                          SCTE_SI_AEIT_EVT_INFO_T*        pt_aeit_evt_info,
                                          TM_COND_T*                      pe_cond);                                   

extern INT32 x_scte_si_get_num_aett_entries(HANDLE_T   h_aett_blk,
                                            UINT16*    pui2_num_entries,
                                            TM_COND_T* pe_cond);
                                     
extern INT32 x_scte_si_get_ntt_lang_code(HANDLE_T        h_ntt_sns,
                                         ISO_639_LANG_T* pt_lang_code,
                                         TM_COND_T*      pe_cond);                              

extern INT32 x_scte_si_get_svct_vct_id(HANDLE_T   h_svct,
                                       UINT16     ui2_idx,  
                                       UINT16*    pui2_vct_id,
                                       TM_COND_T* pe_cond);
                                
extern INT32 x_scte_si_get_aeit_mgt_tag(HANDLE_T   h_aeit_src,
                                        UINT8*     pui1_mgt_tag,
                                        TM_COND_T* pe_cond);
                                 
extern INT32 x_scte_si_get_aett_mgt_tag(HANDLE_T   h_aett_blk,
                                        UINT8*     pui1_mgt_tag,
                                        TM_COND_T* pe_cond);
                                                                                                  
extern INT32 x_scte_si_get_aett_etm_id(HANDLE_T   h_aett_blk,
                                       UINT16     ui2_idx,
                                       UINT32*    pui4_etm_id,
                                       TM_COND_T* pe_cond);

extern INT32 x_scte_si_get_txt_num_lang(HANDLE_T h_txt,
                                        UINT16*  pui2_num_lang);

extern INT32 x_scte_si_get_txt_lang_info(HANDLE_T                 h_txt,
                                         UINT16                   ui2_idx,
                                         SCTE_SI_TXT_LANG_INFO_T* pt_lang_info);

extern INT32 x_scte_si_get_txt_seg_info(HANDLE_T                     h_txt,
                                        SCTE_SI_LANG_SEG_IDX_INFO_T* pt_lang_seg_idx,
                                        SCTE_SI_TXT_SEG_INFO_T*      pt_seg_info);

extern INT32 x_scte_si_get_txt_len(HANDLE_T h_txt,
                                   SIZE_T*  pz_txt_len);
                                   
extern INT32 x_scte_si_get_txt(HANDLE_T                     h_txt,
                               SCTE_SI_LANG_SEG_IDX_INFO_T* pt_lang_seg_idx,
                               CHAR*                        ps_txt,
                               SIZE_T*                      pz_txt_len);

extern INT32 x_scte_si_txt_loc_ntt_src_name(HANDLE_T  h_ntt_sns,
                                            UINT16    ui2_idx,
                                            HANDLE_T* ph_txt);
                                                  
extern INT32 x_scte_si_txt_loc_ntt_src_name_by_id(HANDLE_T               h_ntt_sns,
                                                  SCTE_SI_NTT_ID_INFO_T* pt_id_info,
                                                  HANDLE_T*              ph_txt);

extern INT32 x_scte_si_txt_loc_aeit_title_txt(HANDLE_T                        h_aeit_src,
                                              SCTE_SI_AEIT_SECOND_LOOP_IDX_T* pt_second_loop_idx,
                                              HANDLE_T*                       ph_txt);

extern INT32 x_scte_si_txt_loc_aett_etm(HANDLE_T  h_aett_blk,
                                        UINT16    ui2_idx,
                                        HANDLE_T* ph_txt);

extern INT32 x_scte_si_txt_loc_descr(HANDLE_T     h_obj,
                                     const UINT8* pui1_multi_lang_txt,
                                     HANDLE_T*    ph_txt);

extern INT32 x_scte_si_init(const CHAR*             ps_name,
                            const SCTE_SI_CONFIG_T* pt_config);

extern INT32 x_scte_si_load_nit_cds(HANDLE_T      h_obj,
                                    TM_SRC_TYPE_T e_src_type,
                                    VOID*         pv_src_info,
                                    VOID*         pv_nfy_tag,
                                    x_tm_nfy_fct  pf_nfy,
                                    HANDLE_T*     ph_nit_cds,
                                    TM_COND_T*    pe_cond);

extern INT32 x_scte_si_load_nit_mms(HANDLE_T      h_obj,
                                    TM_SRC_TYPE_T e_src_type,
                                    VOID*         pv_src_info,
                                    VOID*         pv_nfy_tag,
                                    x_tm_nfy_fct  pf_nfy,
                                    HANDLE_T*     ph_nit_mms,
                                    TM_COND_T*    pe_cond);

extern INT32 x_scte_si_load_ntt_sns(HANDLE_T      h_obj,
                                    TM_SRC_TYPE_T e_src_type,
                                    VOID*         pv_src_info,
                                    VOID*         pv_nfy_tag,
                                    x_tm_nfy_fct  pf_nfy,
                                    HANDLE_T*     ph_ntt_sns,
                                    TM_COND_T*    pe_cond);

extern INT32 x_scte_si_load_svct_vcm(HANDLE_T      h_obj,
                                     TM_SRC_TYPE_T e_src_type,
                                     VOID*         pv_src_info,
                                     VOID*         pv_nfy_tag,
                                     x_tm_nfy_fct  pf_nfy,
                                     HANDLE_T*     ph_svct_vcm,
                                     TM_COND_T*    pe_cond);

extern INT32 x_scte_si_load_svct_dcm(HANDLE_T      h_obj,
                                     TM_SRC_TYPE_T e_src_type,
                                     VOID*         pv_src_info,
                                     VOID*         pv_nfy_tag,
                                     x_tm_nfy_fct  pf_nfy,
                                     HANDLE_T*     ph_svct_dcm,
                                     TM_COND_T*    pe_cond);

extern INT32 x_scte_si_load_svct_icm(HANDLE_T      h_obj,
                                     TM_SRC_TYPE_T e_src_type,
                                     VOID*         pv_src_info,
                                     VOID*         pv_nfy_tag,
                                     x_tm_nfy_fct  pf_nfy,
                                     HANDLE_T*     ph_svct_icm,
                                     TM_COND_T*    pe_cond);

extern INT32 x_scte_si_load_aeit_src_by_time(HANDLE_T     h_mgt,
                                             TIME_T       t_time, 
                                             VOID*        pv_nfy_tag,
                                             x_tm_nfy_fct pf_nfy,
                                             HANDLE_T*    ph_aeit_src,
                                             TM_COND_T*   pe_cond);

extern INT32 x_scte_si_load_aett_blk_by_time(HANDLE_T     h_mgt,
                                             TIME_T       t_time, 
                                             VOID*        pv_nfy_tag,
                                             x_tm_nfy_fct pf_nfy,
                                             HANDLE_T*    ph_aett_blk,
                                             TM_COND_T*   pe_cond);

#endif /* _X_SCTE_SI_H_ */

