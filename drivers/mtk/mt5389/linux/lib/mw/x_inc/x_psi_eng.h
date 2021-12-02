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
/*----------------------------------------------------------------------------*/
/*! @file x_psi_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains PSI engine specific definitions, which
 *         are exported.
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_PSI_H_
#define _X_PSI_H_    /**<        */


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_handle_grp.h"
#include "x_common.h"
#include "x_tm.h"
#include "u_psi_eng.h"

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @brief This API returns a copy of a raw section for a PSI table.
 *
 *  @param [in] h_tbl              - References the PSI table object.
 *  @param [in] ui1_sec_num  - Specifies the section to get by index.
 *  @param [in] pui1_buffer    - Pointer to buffer to hold copy
 *  @param [in] pz_len           - Holds length of buffer
 *
 *  @param [out]  pz_len        - Holds length of section
 *  @param [out]  pe_cond     - Contains the condition of the table object.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                           -  Routine successful.
 *  @retval TMR_NOT_INIT                 - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                  - One or more of the arguments contain invalid
 *                                                        data.
 *  @retval TMR_INV_HANDLE             - Invalid handle.
 *  @retval TMR_NO_SUPPORT            - No support.
 *  @retval TMR_INV_HANDLE_STATE  - Invalid handle state.
 *  @retval TMR_INV_GET_INFO          - Invalid get operation data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_get_raw_section (HANDLE_T   h_tbl,
                                    UINT8      ui1_sec_num,
                                    UINT8*     pui1_buffer,
                                    SIZE_T*    pz_len,
                                    TM_COND_T* pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API returns an entry (NIT or PMT) from a PAT table.
 *
 *  @param [in]  h_pat                 - References the PAT table object.
  *  @param [in] ui2_idx               - Specifies the entry number by index.
 *
 *  @param [out] pt_pid_svc_info  - Contains the entry info.
 *  @param [out] pe_cond            - Contains the condition of the table object.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                           -  Routine successful.
 *  @retval TMR_NOT_INIT                 - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                  - One or more of the arguments contain invalid
 *                                                        data.
 *  @retval TMR_INV_HANDLE             - Invalid handle.
 *  @retval TMR_NO_SUPPORT            - No support.
 *  @retval TMR_INV_HANDLE_STATE  - Invalid handle state.
 *  @retval TMR_INV_GET_INFO          - Invalid get operation data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_get_pat_entry (HANDLE_T                h_pat,
                                  UINT16                  ui2_idx,
                                  PSI_PID_SVC_ID_INFO_T*  pt_pid_svc_info,
                                  TM_COND_T*              pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API returns a service entry from a PAT table.
 *
 *  @param [in] h_pat                   - References the PAT table object.
 *  @param [in] ui2_idx                 -  Specifies the program entry number by index.
 *
 *  @param [out]  pt_pid_svc_info  - Contains the service entry info.
 *  @param [out]  pe_cond            - Contains the condition of the table object.
 *
 *  @return Return the error code.
 *  @retval  TMR_OK                           - Routine successful.
 *  @retval  TMR_NOT_INIT                 - The Table Manager is not initialized.
 *  @retval  TMR_INV_ARG                  - One or more of the arguments contain invalid
 *                                                         data.
 *  @retval  TMR_INV_HANDLE             - Invalid handle.
 *  @retval  TMR_NO_SUPPORT            - No support.
 *  @retval  TMR_INV_HANDLE_STATE  - Invalid handle state.
 *  @retval  TMR_INV_GET_INFO          - Invalid get operation data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_get_svc_entry (HANDLE_T                h_pat,
                                  UINT16                  ui2_idx,
                                  PSI_PID_SVC_ID_INFO_T*  pt_pid_svc_info,
                                  TM_COND_T*              pe_cond);
/*------------------------------------------------------------------*/
/*! @brief  This API returns the transport stream id of a PAT table.
 *
 *  @param [in]  h_pat          - References the PAT table object.
 *
 *  @param [out] pui2_ts_id  - Contains the transport stream id.
 *  @param [out] pe_cond     - Contains the condition of the table object.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                            - Routine successful.
 *  @retval TMR_NOT_INIT                  - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                   - One or more of the arguments contain invalid
 *                                                         data.
 *  @retval TMR_INV_HANDLE             - Invalid handle.
 *  @retval TMR_NO_SUPPORT            - No support.
 *  @retval TMR_INV_HANDLE_STATE  - Invalid handle state.
 *  @retval TMR_INV_GET_INFO          - Invalid get operation data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_get_ts_id (HANDLE_T    h_pat,
                              UINT16*     pui2_ts_id,
                              TM_COND_T*  pe_cond);
/*------------------------------------------------------------------*/
/*! @brief  This API returns the PCR PID value of a PMT table.
 *
 *  @param [in] h_pmt          - References the PMT table object.
 *
 *  @param [out] pt_pcr_pid  - Contains the PCR PID.
 *  @param [out] pe_cond    - Contains the condition of the table object.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                           - Routine successful.
 *  @retval TMR_NOT_INIT                 - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                  - One or more of the arguments contain invalid
 *                                                        data.
 *  @retval TMR_INV_HANDLE             - Invalid handle.
 *  @retval TMR_NO_SUPPORT            - No support.
 *  @retval TMR_INV_HANDLE_STATE  - Invalid handle state.
 *  @retval TMR_INV_GET_INFO          - Invalid get operation data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_get_pcr_pid (HANDLE_T       h_pmt,
                                MPEG_2_PID_T*  pt_pcr_pid,
                                TM_COND_T*     pe_cond);
/*------------------------------------------------------------------*/
/*! @brief This API returns the stream enty information in a PMT.
 *
 *  @param [in] h_pmt                   - References the PMT table object.
 *  @param [in] ui2_idx                  - Specifies the stream entry number by index.
 *
 *  @param [out] pt_stream_entry  - Contains the stream entry information.
 *  @param [out] pe_cond              - Contains the condition of the table object.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                           - Routine successful.
 *  @retval TMR_NOT_INIT                 - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                  - One or more of the arguments contain invalid
 *                                                        data.
 *  @retval TMR_INV_HANDLE             - Invalid handle.
 *  @retval TMR_NO_SUPPORT            - No support.
 *  @retval TMR_INV_HANDLE_STATE  - Invalid handle state.
 *  @retval TMR_INV_GET_INFO          - Invalid get operation data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_get_stream_entry (HANDLE_T            h_pmt,
                                     UINT16              ui2_idx,
                                     PSI_STREAM_INFO_T*  pt_stream_entry,
                                     TM_COND_T*          pe_cond);
/*------------------------------------------------------------------*/
/*! @brief This API returns the program or service id of a PMT table.
 *
 *  @param [in] h_pmt            - References the PMT table object.
 *
 *  @param [out] pui2_svc_id  - Contains the program or service id.
 *  @param [out] pe_cond       - Contains the condition of the table object.
 *
 *  @return Return the error code.
 *  @retval    TMR_OK                         - Routine successful.
 *  @retval TMR_NOT_INIT                  - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                   - One or more of the arguments contain invalid
 *                                                        data.
 *  @retval  TMR_INV_HANDLE             - Invalid handle.
 *  @retval   TMR_NO_SUPPORT           - No support.
 *  @retval  TMR_INV_HANDLE_STATE  - Invalid handle state.
 *  @retval    TMR_INV_GET_INFO        - Invalid get operation data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_get_svc_id (HANDLE_T    h_pmt,
                               UINT16*     pui2_svc_id,
                               TM_COND_T*  pe_cond);
/*------------------------------------------------------------------*/
/*! @brief This API returns the number of ts ids of a monitor PAT table object.
 *
 *  @param [in] h_mon_pat          - References the monitor PAT table object.
 *
 *  @param [out] pui2_num_ts_id  -Contains the number of ts ids.
 *  @param [out] pe_cond            - Contains the condition of the table object.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                           - Routine successful.
 *  @retval TMR_NOT_INIT                 - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                  - One or more of the arguments contain invalid
 *                                                        data.
 *  @retval TMR_INV_HANDLE             - Invalid handle.
 *  @retval TMR_NO_SUPPORT            - No support.
 *  @retval TMR_INV_HANDLE_STATE  - Invalid handle state.
 *  @retval TMR_INV_GET_INFO          - Invalid get operation data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_mon_get_num_ts_id (HANDLE_T    h_mon_pat,
                                      UINT16*     pui2_num_ts_id,
                                      TM_COND_T*  pe_cond);
/*------------------------------------------------------------------*/
/*! @brief This API returns the an entry, referenced by an index, of the
 *              monitor PAT table object.
 *
 *  @param [in] h_mon_pat  - References the monitor PAT table object.
 *  @param [in] ui2_idx        - Contains the index value.
 *
 *  @param [out] pui2_ts_id  - Contains the number of ts ids.
 *  @param [out] pe_cond    - Contains the condition of the table object.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                           - Routine successful.
 *  @retval TMR_NOT_INIT                 - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                  - One or more of the arguments contain invalid
 *                                                        data.
 *  @retval TMR_INV_HANDLE             - Invalid handle.
 *  @retval TMR_NO_SUPPORT            - No support.
 *  @retval TMR_INV_HANDLE_STATE  - Invalid handle state.
 *  @retval TMR_INV_GET_INFO          - Invalid get operation data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_mon_get_ts_id (HANDLE_T    h_mon_pat,
                                  UINT16      ui2_idx,
                                  UINT16*     pui2_ts_id,
                                  TM_COND_T*  pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API initializes the PSI engine and registers it with the
 *              Table Manager.
 *
 *  @param [in] ps_name   - Contains the PSI engine name.
 *  @param [in] pt_config   - References the PSI engine configuration control
 *                                       structure.
 *
 *  @param [out] None.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                           - Routine successful.
 *  @retval TMR_NOT_INIT                 - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                  - One or more of the arguments contain invalid
 *                                                        data.
 *  @retval TMR_INV_HANDLE             - Invalid handle.
 *  @retval TMR_NO_SUPPORT            - No support.
 *  @retval TMR_INV_HANDLE_STATE  - Invalid handle state.
 *  @retval TMR_INV_GET_INFO          - Invalid get operation data.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_init (const CHAR*    ps_name,
                         PSI_CONFIG_T*  pt_config);

/*------------------------------------------------------------------*/
/*! @brief This API loads a TSDT table.
 *
 *  @param [in] h_root           - References the root table object.
 *  @param [in] e_src_type    - Specifies the source type for loading the requested
 *                                            table.
 *  @param [in] pv_src_info    - Contains additional information describing the source.
 *  @param [in] pv_nfy_tag     - Contains a notify tag value.
 *  @param [in]  pf_nfy           - References the notify function.
 *
 *  @param [out]  ph_tsdt        - Contains a handle which references the TSDT table.
 *  @param [out]   pe_cond      -Contains the condition of the TSDT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                            -  Routine successful.
 *  @retval TMR_NOT_INIT                  - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                   - One or more of the arguments contain invalid
 *  @retval                                           data.
 *  @retval TMR_INV_HANDLE              - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES      - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_load_tsdt (HANDLE_T       h_root,
                              TM_SRC_TYPE_T  e_src_type,
                              VOID*          pv_src_info,
                              VOID*          pv_nfy_tag,
                              x_tm_nfy_fct   pf_nfy,
                              HANDLE_T*      ph_tsdt,
                              TM_COND_T*     pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API loads a CAT table.
 *
 *  @param [in] h_root          - References the root table object.
 *  @param [in] e_src_type    - Specifies the source type for loading the requested
 *                                            table.
 *  @param [in]  pv_src_info  -Contains additional information describing the source.
 *  @param [in]  pv_nfy_tag  - Contains a notify tag value.
 *  @param [in]  pf_nfy         - References the notify function.
 *
 *  @param [out] ph_cat       - Contains a handle which references the CAT table.
 *  @param [out] pe_cond     - Contains the condition of the CAT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                             -  Routine successful.
 *  @retval TMR_NOT_INIT                   - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                    - One or more of the arguments contain invalid
 *                                                          data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    -  Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_load_cat (HANDLE_T       h_root,
                             TM_SRC_TYPE_T  e_src_type,
                             VOID*          pv_src_info,
                             VOID*          pv_nfy_tag,
                             x_tm_nfy_fct   pf_nfy,
                             HANDLE_T*      ph_cat,
                             TM_COND_T*     pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API loads a PAT table
 *
 *  @param [in] h_root         -  References the root table object.
 *  @param [in] e_src_type   - Specifies the source type for loading the requested
 *                                          table.
 *  @param [in] pv_src_info  - Contains additional information describing the source.
 *  @param [in] pv_nfy_tag  - Contains a notify tag value.
 *  @param [in] pf_nfy         - References the notify function.
 *
 *  @param [out] ph_pat      - Contains a handle which references the PAT table.
 *  @param [out] pe_cond    - Contains the condition of the PAT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    -  The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               -  Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES   - Out of resources.
 *  @retval TMR_NO_SUPPORT               - No support.
 *  @retval TMR_INV_HANDLE_STATE     -  Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_load_pat(HANDLE_T       h_root,
                            TM_SRC_TYPE_T  e_src_type,
                            VOID*          pv_src_info,
                            VOID*          pv_nfy_tag,
                            x_tm_nfy_fct   pf_nfy,
                            HANDLE_T*      ph_pat,
                            TM_COND_T*     pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API loads a PAT table given a transport stream id.
 *
 *  @param [in] h_root           - References the root table object.
 *  @param [in] ui2_ts_id       - Contains the PAT's transport stream id.
 *  @param [in] e_src_type    - Specifies the source type for loading the requested
 *                                            table.
 *  @param [in] pv_src_info    - Contains additional information describing the source.
 *  @param [in] pv_nfy_tag    - Contains a notify tag value.
 *  @param [in] pf_nfy           - References the notify function.
 *
 *  @param [out]  ph_pat        - Contains a handle which references the PAT table.
 *  @param [out]  pe_cond      - Contains the condition of the PAT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                            - Routine successful.
 *  @retval TMR_NOT_INIT                  - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                   - One or more of the arguments contain invalid
 *                                                         data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_load_pat_by_ts_id (HANDLE_T       h_root,
                                      UINT16         ui2_ts_id,
                                      TM_SRC_TYPE_T  e_src_type,
                                      VOID*          pv_src_info,
                                      VOID*          pv_nfy_tag,
                                      x_tm_nfy_fct   pf_nfy,
                                      HANDLE_T*      ph_pat,
                                      TM_COND_T*     pe_cond);
/*------------------------------------------------------------------*/
/*! @brief This API loads a PAT table given a transport stream descriptor.
 *
 *  @param [in] h_root          - References the root table object.
 *  @param [in] ui2_ts_id      - Contains the PAT's transport stream id.
 *  @param [in] e_src_type   - Specifies the source type for loading the requested
 *                                           table.
 *  @param [in] pv_src_info  - Contains additional information describing the source.
 *  @param [in] pv_nfy_tag  - Contains a notify tag value.
 *  @param [in] pf_nfy         - References the notify function.
 *
 *  @param [out]  ph_pat     - Contains a handle which references the PAT table.
 *  @param [out]  pe_cond   - Contains the condition of the PAT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_load_pat_by_ts_descr (HANDLE_T       h_root,
                                         TS_DESCR_T*    pt_ts_descr,
                                         TM_SRC_TYPE_T  e_src_type,
                                         VOID*          pv_src_info,
                                         VOID*          pv_nfy_tag,
                                         x_tm_nfy_fct   pf_nfy,
                                         HANDLE_T*      ph_pat,
                                         TM_COND_T*     pe_cond);
/*------------------------------------------------------------------*/
/*! @brief This API loads a PMT table given a service id and PAT table
 *              handle.
 *
 *  @param [in] h_pat           - References the PAT table object.
 *  @param [in] ui2_svc_id    - Contains the PMT's service id.
 *  @param [in] e_src_type   - Specifies the source type for loading the requested
 *                                           table.
 *  @param [in] pv_src_info  - Contains additional information describing the source.
 *  @param [in] pv_nfy_tag  - Contains a notify tag value.
 *  @param [in] pf_nfy         - References the notify function.
 *
 *  @param [out] ph_pmt     - Contains a handle which references the PMT table.
 *  @param [out] pe_cond    - Contains the condition of the PMT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_load_pmt_by_svc_id (HANDLE_T       h_pat,
                                       UINT16         ui2_svc_id,
                                       TM_SRC_TYPE_T  e_src_type,
                                       VOID*          pv_src_info,
                                       VOID*          pv_nfy_tag,
                                       x_tm_nfy_fct   pf_nfy,
                                       HANDLE_T*      ph_pmt,
                                       TM_COND_T*     pe_cond);
/*------------------------------------------------------------------*/
/*! @brief  This API loads a PMT table given an index and PAT table handle.
 *
 *  @param [in] h_pat          - References the PAT table object.
 *  @param [in] ui2_idx        - Contains the index value.
 *  @param [in] e_src_type   - Specifies the source type for loading the requested
 *                                            table.
 *  @param [in] pv_src_info  - Contains additional information describing the source.
 *  @param [in] pv_nfy_tag  - Contains a notify tag value.
 *  @param [in] pf_nfy         -  References the notify function.
 *
 *
 *  @param [out] ph_pmt     - Contains a handle which references the PMT table.
 *  @param [out] pe_cond    -Contains the condition of the PMT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_load_pmt_by_idx (HANDLE_T       h_pat,
                                    UINT16         ui2_idx,
                                    TM_SRC_TYPE_T  e_src_type,
                                    VOID*          pv_src_info,
                                    VOID*          pv_nfy_tag,
                                    x_tm_nfy_fct   pf_nfy,
                                    HANDLE_T*      ph_pmt,
                                    TM_COND_T*     pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API loads a PMT table given a PID value and a service id.
 *
 *  @param [in] h_root           - References the TM root object.
 *  @param [in] t_pid             - Contains the PMT's PID value.
 *  @param [in] ui2_svc_id     - Contains the PMT's service id.
 *  @param [in] e_src_type    - Specifies the source type for loading the requested
 *                                            table.
 *  @param [in] pv_src_info   - Contains additional information describing the source.
 *  @param [in] pv_nfy_tag   - Contains a notify tag value.
 *  @param [in] pf_nfy          - References the notify function.
 *
 *  @param [out] ph_pmt       -Contains a handle which references the PMT table.
 *  @param [out] pe_cond      -Contains the condition of the PMT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_load_pmt_by_pid_svc_id (HANDLE_T        h_root,
                                           MPEG_2_PID_T    t_pid,
                                           UINT16          ui2_svc_id,
                                           TM_SRC_TYPE_T   e_src_type,
                                           VOID*           pv_src_info,
                                           VOID*           pv_nfy_tag,
                                           x_tm_nfy_fct    pf_nfy,
                                           HANDLE_T*       ph_pmt,
                                           TM_COND_T*      pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API monitors PAT tables.
 *
 *  @param [in] h_root              - References the root object.
 *  @param [in] e_src_type       - Specifies the source type for loading the requested
 *                                               table.
 *  @param [in] pv_src_info      - Contains additional information describing the source.
 *  @param [in] pv_nfy_tag      -  Contains a notify tag value.
 *  @param [in] pf_nfy             - References the notify function.
 *
 *  @param [out] ph_mon_pat  - Contains a handle which references the PAT monitor
 *                                             object.
 *  @param [out] pe_cond       - Contains the condition of the PMT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_mon_pat (HANDLE_T       h_root,
                            TM_SRC_TYPE_T  e_src_type,
                            VOID*          pv_src_info,
                            VOID*          pv_nfy_tag,
                            x_tm_nfy_fct   pf_nfy,
                            HANDLE_T*      ph_mon_pat,
                            TM_COND_T*     pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API creates a PMT entry in the specified PAT
 *
 *  @param [in] h_pat                - References the PAT object
 *  @param [in] pt_pid_svc_info  -Contains the PMT's service id and PID.
 *
 *  @param [out] pe_cond          - Contains the condition of the PMT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_set_pat_pmt_info (HANDLE_T               h_pat,
                                     PSI_PID_SVC_ID_INFO_T* pt_pid_svc_info,
                                     TM_COND_T*             pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API sets whether a table is available
 *
 *  @param [in] h_tbl           - References the PSI object
 *  @param [in] b_available  - Specifies the state
 *
 *  @param [out] pe_cond    - Contains the condition of the PMT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_set_available (HANDLE_T   h_tbl,
                                  BOOL       b_available,
                                  TM_COND_T* pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API sets a table's version number
 *
 *  @param [in] h_tbl            - References the PSI object
 *  @param [in] ui1_version   - Contains the version number
 *
 *  @param [out] pe_cond     -  Contains the condition of the table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_set_version_num (HANDLE_T   h_tbl,
                                    UINT8      ui1_version,
                                    TM_COND_T* pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API sets a PMT's PCR PID
 *
 *  @param [in] h_pmt       -  References the PMT object
 *  @param [in] t_pid         - Contains the PCR PID
 *
 *  @param [out] pe_cond  - Contains the condition of the PMT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_set_pmt_pcr_pid (HANDLE_T     h_pmt,
                                    MPEG_2_PID_T t_pid,
                                    TM_COND_T*   pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API inserts a stream entry in the specified PMT
 *
 *  @param [in] h_pmt                -References the PMT object
 *  @param [in] pt_stream_info   - Points to the stream's info
 *
 *  @param [out] pe_cond           - Contains the condition of the PMT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_set_pmt_stream_entry (HANDLE_T              h_pmt,
                                         PSI_STREAM_INFO_T*    pt_stream_info,
                                         TM_COND_T*            pe_cond);

/*------------------------------------------------------------------*/
/*! @brief  This API inserts a stream descriptor  in the specified PMT
 *
 *  @param [in] h_pmt          - References the PMT object
 *  @param [in] pui1_descr    - Points to the descriptor
 *  @param [in] z_descr_len  - Length of descriptor
 *
 *  @param [out]  pe_cond    -  Contains the condition of the PMT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_set_pmt_stream_descr (HANDLE_T              h_pmt,
                                         UINT8*                pui1_descr,
                                         SIZE_T                z_descr_len,
                                         TM_COND_T*            pe_cond);

/*------------------------------------------------------------------*/
/*! @brief This API sets the main descriptors in the specified PMT
 *
 *  @param [in] h_pmt             - References the PMT object
 *  @param [in] pui1_descr       - Points to the stream's descriptors
 *  @param [in] z_descr_len     - Length of the descriptors
 *
 *  @param [out] pe_cond        -  Contains the condition of the PMT table.
 *
 *  @return Return the error code.
 *  @retval TMR_OK                              - Routine successful.
 *  @retval TMR_NOT_INIT                    - The Table Manager is not initialized.
 *  @retval TMR_INV_ARG                     - One or more of the arguments contain invalid
 *                                                           data.
 *  @retval TMR_INV_HANDLE               - Invalid handle.
 *  @retval TMR_OUT_OF_HANDLES       - No handles available.
 *  @retval TMR_OUT_OF_RESOURCES  - Out of resources.
 *  @retval TMR_NO_SUPPORT              - No support.
 *  @retval TMR_INV_HANDLE_STATE    - Invalid handle state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_psi_set_pmt_main_descr (HANDLE_T              h_pmt,
                                       UINT8*                pui1_descr,
                                       SIZE_T                z_descr_len,
                                       TM_COND_T*            pe_cond);
#endif /* _X_PSI_H_ */
