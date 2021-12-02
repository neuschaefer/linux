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
#ifndef  _L1_S1216x_Defines_H_
#define _L1_S1216x_Defines_H_

//#define RWTRACES
/*#ifdef RWTRACES     // 16-bit version
//    #warning "register-level traces activated (RWTRACES defined)" 
    #define L1_READ(ptr, register)      L1_ReadRegisterTrace  (ptr, #register,     register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_SIGNED)
    #define L1_WRITE(ptr, register, v ) L1_WriteRegisterTrace (ptr, #register, #v, register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_ALONE, v)
#else
    #define L1_READ(ptr, register)      L1_ReadRegister  (ptr, register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_SIGNED)
    #define L1_WRITE(ptr, register, v ) L1_WriteRegister (ptr, register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_ALONE, v)
#endif */

#ifdef RWTRACES       // 8-bit version
/*    #warning "register-level traces activated (RWTRACES defined)" */
  #ifdef   INDEXING_MODE_8_BITS
    #define L1_READ(ptr, register)      L1_ReadRegisterTrace8Bits  (ptr, #register,     register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_SIGNED)
    #define L1_WRITE(ptr, register, v ) L1_WriteRegisterTrace8Bits (ptr, #register, #v, register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_ALONE, v)
  #else
    #define L1_READ(ptr, register)      L1_ReadRegisterTrace  (ptr, #register,     register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_SIGNED)
    #define L1_WRITE(ptr, register, v ) L1_WriteRegisterTrace (ptr, #register, #v, register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_ALONE, v)
  #endif /* INDEXING_MODE_8_BITS  */
#else
  #ifdef   INDEXING_MODE_8_BITS
    #define L1_READ(ptr, register)      L1_ReadRegister8Bits  (ptr, register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_SIGNED)
    #define L1_WRITE(ptr, register, v ) L1_WriteRegister8Bits (ptr, register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_ALONE, v)
  #else
    #define L1_READ(ptr, register)      L1_ReadRegister  (ptr, register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_SIGNED)
    #define L1_WRITE(ptr, register, v ) L1_WriteRegister (ptr, register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_ALONE, v)
  #endif /* INDEXING_MODE_8_BITS  */
#endif

#define DBG TRACES_TRACE();system("pause");

 /* i2c_passthru                   */
 #define    i2c_passthru_ADDRESS               1
 #define    i2c_passthru_OFFSET                0
 #define    i2c_passthru_NBBIT                 1
 #define    i2c_passthru_ALONE                 1
 #define    i2c_passthru_SIGNED                0
  #define           i2c_passthru_disabled                      0
  #define           i2c_passthru_enabled                       1
 /* revcode                        */
 #define    revcode_ADDRESS                   35
 #define    revcode_OFFSET                     0
 #define    revcode_NBBIT                      8
 #define    revcode_ALONE                      1
 #define    revcode_SIGNED                     0
  #define           revcode_rev_a                              0
  #define           revcode_rev_b                              1
  #define           revcode_rev_c                              2
  #define           revcode_rev_d                              3
 /* chip_init                      */
 #define    chip_init_ADDRESS                 80
 #define    chip_init_OFFSET                   0
 #define    chip_init_NBBIT                    1
 #define    chip_init_ALONE                    1
 #define    chip_init_SIGNED                   0
  #define           chip_init_func                             0
  #define           chip_init_init                             1
 /* init_done                      */
 #define    init_done_ADDRESS                 84
 #define    init_done_OFFSET                   0
 #define    init_done_NBBIT                    1
 #define    init_done_ALONE                    1
 #define    init_done_SIGNED                   0
  #define           init_done_pending                          0
  #define           init_done_completed                        1
 /* start_init                     */
 #define    start_init_ADDRESS               150
 #define    start_init_OFFSET                  0
 #define    start_init_NBBIT                   1
 #define    start_init_ALONE                   1
 #define    start_init_SIGNED                  0
  #define           start_init_reset                           0
  #define           start_init_run                             1
 /* pll_divl                       */
 #define    pll_divl_ADDRESS                 160
 #define    pll_divl_OFFSET                    0
 #define    pll_divl_NBBIT                     5
 #define    pll_divl_ALONE                     1
 #define    pll_divl_SIGNED                    0
 /* pll_divm                       */
 #define    pll_divm_ADDRESS                 161
 #define    pll_divm_OFFSET                    0
 #define    pll_divm_NBBIT                     5
 #define    pll_divm_ALONE                     1
 #define    pll_divm_SIGNED                    0
 /* pll_divn                       */
 #define    pll_divn_ADDRESS                 162
 #define    pll_divn_OFFSET                    0
 #define    pll_divn_NBBIT                     6
 #define    pll_divn_ALONE                     0
 #define    pll_divn_SIGNED                    0
 /* pll_divp                       */
 #define    pll_divp_ADDRESS                 162
 #define    pll_divp_OFFSET                    6
 #define    pll_divp_NBBIT                     1
 #define    pll_divp_ALONE                     0
 #define    pll_divp_SIGNED                    0
  #define           pll_divp_prescaler_4                       0
  #define           pll_divp_prescaler_1                       1
 /* pll_enable                     */
 #define    pll_enable_ADDRESS               162
 #define    pll_enable_OFFSET                  7
 #define    pll_enable_NBBIT                   1
 #define    pll_enable_ALONE                   0
 #define    pll_enable_SIGNED                  0
  #define           pll_enable_disable                         0
  #define           pll_enable_enable                          1
 /* pll_divr                       */
 #define    pll_divr_ADDRESS                 163
 #define    pll_divr_OFFSET                    0
 #define    pll_divr_NBBIT                     3
 #define    pll_divr_ALONE                     1
 #define    pll_divr_SIGNED                    0
 /* rst_all                        */
 #define    rst_all_ADDRESS                  192
 #define    rst_all_OFFSET                     0
 #define    rst_all_NBBIT                      1
 #define    rst_all_ALONE                      1
 #define    rst_all_SIGNED                     0
  #define           rst_all_reset                              0
  #define           rst_all_run                                1
 /* lock_timeout                   */
 #define    lock_timeout_ADDRESS             196
 #define    lock_timeout_OFFSET                0
 #define    lock_timeout_NBBIT                26
 #define    lock_timeout_ALONE                 1
 #define    lock_timeout_SIGNED                0
 /* auto_reset                     */
 #define    auto_reset_ADDRESS               203
 #define    auto_reset_OFFSET                  0
 #define    auto_reset_NBBIT                   1
 #define    auto_reset_ALONE                   1
 #define    auto_reset_SIGNED                  0
  #define           auto_reset_off                             0
  #define           auto_reset_on                              1
 /* adc_sampling_mode              */
 #define    adc_sampling_mode_ADDRESS        224
 #define    adc_sampling_mode_OFFSET           0
 #define    adc_sampling_mode_NBBIT            2
 #define    adc_sampling_mode_ALONE            1
 #define    adc_sampling_mode_SIGNED           0
  #define           adc_sampling_mode_if_ovr4                  0
  #define           adc_sampling_mode_if_ovr2                  1
  #define           adc_sampling_mode_zif_ovr4                 2
  #define           adc_sampling_mode_zif_ovr2                 3
 /* oversamp                       */
 #define    oversamp_ADDRESS                 228
 #define    oversamp_OFFSET                    0
 #define    oversamp_NBBIT                    30
 #define    oversamp_ALONE                     1
 #define    oversamp_SIGNED                    0
 /* if_freq_shift                  */
 #define    if_freq_shift_ADDRESS            232
 #define    if_freq_shift_OFFSET               0
 #define    if_freq_shift_NBBIT               29
 #define    if_freq_shift_ALONE                1
 #define    if_freq_shift_SIGNED               1
 /* standard                       */
 #define    standard_ADDRESS                 236
 #define    standard_OFFSET                    0
 #define    standard_NBBIT                     6
 #define    standard_ALONE                     1
 #define    standard_SIGNED                    0
  #define           standard_dvb_t                             1
  #define           standard_dvb_c                             5
 /* constellation                  */
 #define    constellation_ADDRESS            248
 #define    constellation_OFFSET               0
 #define    constellation_NBBIT                6
 #define    constellation_ALONE                1
 #define    constellation_SIGNED               0
  #define           constellation_qpsk                         3
  #define           constellation_qam16                        7
  #define           constellation_qam32                        8
  #define           constellation_qam64                        9
  #define           constellation_qam128                      10
  #define           constellation_qam256                      11
 /* dsp_clock                      */
 #define    dsp_clock_ADDRESS                260
 #define    dsp_clock_OFFSET                   0
 #define    dsp_clock_NBBIT                    1
 #define    dsp_clock_ALONE                    1
 #define    dsp_clock_SIGNED                   0
  #define           dsp_clock_disable                          0
  #define           dsp_clock_enable                           1
 /* adc_ri8                        */
 #define    adc_ri8_ADDRESS                  291
 #define    adc_ri8_OFFSET                     0
 #define    adc_ri8_NBBIT                      8
 #define    adc_ri8_ALONE                      1
 #define    adc_ri8_SIGNED                     0
 /* adc_ri1                        */
 #define    adc_ri1_ADDRESS                  298
 #define    adc_ri1_OFFSET                     0
 #define    adc_ri1_NBBIT                      8
 #define    adc_ri1_ALONE                      1
 #define    adc_ri1_SIGNED                     0
 /* adc_ri3                        */
 #define    adc_ri3_ADDRESS                  300
 #define    adc_ri3_OFFSET                     0
 #define    adc_ri3_NBBIT                      8
 #define    adc_ri3_ALONE                      1
 #define    adc_ri3_SIGNED                     0
 /* adc_ri5                        */
 #define    adc_ri5_ADDRESS                  302
 #define    adc_ri5_OFFSET                     0
 #define    adc_ri5_NBBIT                      8
 #define    adc_ri5_ALONE                      1
 #define    adc_ri5_SIGNED                     0
 /* adc_ri6                        */
 #define    adc_ri6_ADDRESS                  303
 #define    adc_ri6_OFFSET                     0
 #define    adc_ri6_NBBIT                      8
 #define    adc_ri6_ALONE                      1
 #define    adc_ri6_SIGNED                     0
 /* agc_crestf_dbx8                */
 #define    agc_crestf_dbx8_ADDRESS          336
 #define    agc_crestf_dbx8_OFFSET             0
 #define    agc_crestf_dbx8_NBBIT              8
 #define    agc_crestf_dbx8_ALONE              1
 #define    agc_crestf_dbx8_SIGNED             0
 /* agc_unfreeze_thr               */
 #define    agc_unfreeze_thr_ADDRESS         347
 #define    agc_unfreeze_thr_OFFSET            0
 #define    agc_unfreeze_thr_NBBIT             4
 #define    agc_unfreeze_thr_ALONE             1
 #define    agc_unfreeze_thr_SIGNED            0
 /* agc1_kacq                      */
 #define    agc1_kacq_ADDRESS                348
 #define    agc1_kacq_OFFSET                   0
 #define    agc1_kacq_NBBIT                    5
 #define    agc1_kacq_ALONE                    1
 #define    agc1_kacq_SIGNED                   0
 /* agc1_kloc                      */
 #define    agc1_kloc_ADDRESS                349
 #define    agc1_kloc_OFFSET                   0
 #define    agc1_kloc_NBBIT                    5
 #define    agc1_kloc_ALONE                    1
 #define    agc1_kloc_SIGNED                   0
 /* agc1_pola                      */
 #define    agc1_pola_ADDRESS                352
 #define    agc1_pola_OFFSET                   1
 #define    agc1_pola_NBBIT                    1
 #define    agc1_pola_ALONE                    0
 #define    agc1_pola_SIGNED                   0
  #define           agc1_pola_non_inverted                     0
  #define           agc1_pola_inverted                         1
 /* agc1_clkdiv                    */
 #define    agc1_clkdiv_ADDRESS              353
 #define    agc1_clkdiv_OFFSET                 0
 #define    agc1_clkdiv_NBBIT                  3
 #define    agc1_clkdiv_ALONE                  1
 #define    agc1_clkdiv_SIGNED                 0
 /* agc1_cmd                       */
 #define    agc1_cmd_ADDRESS                 360
 #define    agc1_cmd_OFFSET                    0
 #define    agc1_cmd_NBBIT                     8
 #define    agc1_cmd_ALONE                     1
 #define    agc1_cmd_SIGNED                    0
 /* agc2_kacq                      */
 #define    agc2_kacq_ADDRESS                364
 #define    agc2_kacq_OFFSET                   0
 #define    agc2_kacq_NBBIT                    5
 #define    agc2_kacq_ALONE                    1
 #define    agc2_kacq_SIGNED                   0
 /* agc2_kloc                      */
 #define    agc2_kloc_ADDRESS                365
 #define    agc2_kloc_OFFSET                   0
 #define    agc2_kloc_NBBIT                    5
 #define    agc2_kloc_ALONE                    1
 #define    agc2_kloc_SIGNED                   0
 /* agc2_min                       */
 #define    agc2_min_ADDRESS                 366
 #define    agc2_min_OFFSET                    0
 #define    agc2_min_NBBIT                     8
 #define    agc2_min_ALONE                     1
 #define    agc2_min_SIGNED                    0
 /* agc2_pola                      */
 #define    agc2_pola_ADDRESS                368
 #define    agc2_pola_OFFSET                   1
 #define    agc2_pola_NBBIT                    1
 #define    agc2_pola_ALONE                    0
 #define    agc2_pola_SIGNED                   0
  #define           agc2_pola_non_inverted                     0
  #define           agc2_pola_inverted                         1
 /* agc2_buftype                   */
 #define    agc2_buftype_ADDRESS             368
 #define    agc2_buftype_OFFSET                2
 #define    agc2_buftype_NBBIT                 1
 #define    agc2_buftype_ALONE                 0
 #define    agc2_buftype_SIGNED                0
  #define           agc2_buftype_push_pull                     0
  #define           agc2_buftype_open_drain                    1
 /* agc2_clkdiv                    */
 #define    agc2_clkdiv_ADDRESS              369
 #define    agc2_clkdiv_OFFSET                 0
 #define    agc2_clkdiv_NBBIT                  3
 #define    agc2_clkdiv_ALONE                  1
 #define    agc2_clkdiv_SIGNED                 0
 /* agc2_cmd                       */
 #define    agc2_cmd_ADDRESS                 376
 #define    agc2_cmd_OFFSET                    0
 #define    agc2_cmd_NBBIT                     8
 #define    agc2_cmd_ALONE                     1
 #define    agc2_cmd_SIGNED                    0
 /* agc_if_tri                     */
 #define    agc_if_tri_ADDRESS               395
 #define    agc_if_tri_OFFSET                  0
 #define    agc_if_tri_NBBIT                   1
 #define    agc_if_tri_ALONE                   1
 #define    agc_if_tri_SIGNED                  0
  #define           agc_if_tri_normal                          0
  #define           agc_if_tri_tristate                        1
 /* agc_rf_tri                     */
 #define    agc_rf_tri_ADDRESS               397
 #define    agc_rf_tri_OFFSET                  0
 #define    agc_rf_tri_NBBIT                   1
 #define    agc_rf_tri_ALONE                   1
 #define    agc_rf_tri_SIGNED                  0
  #define           agc_rf_tri_normal                          0
  #define           agc_rf_tri_tristate                        1
 /* agc_if_slr                     */
 #define    agc_if_slr_ADDRESS               400
 #define    agc_if_slr_OFFSET                  0
 #define    agc_if_slr_NBBIT                   2
 #define    agc_if_slr_ALONE                   1
 #define    agc_if_slr_SIGNED                  0
  #define           agc_if_slr_fastest_edges                   0
  #define           agc_if_slr_slowest_edges                   1
  #define           agc_if_slr_moderate_edges                  2
  #define           agc_if_slr_fast_edges                      3
 /* agc_rf_slr                     */
 #define    agc_rf_slr_ADDRESS               402
 #define    agc_rf_slr_OFFSET                  0
 #define    agc_rf_slr_NBBIT                   2
 #define    agc_rf_slr_ALONE                   1
 #define    agc_rf_slr_SIGNED                  0
  #define           agc_rf_slr_fastest_edges                   0
  #define           agc_rf_slr_slowest_edges                   1
  #define           agc_rf_slr_moderate_edges                  2
  #define           agc_rf_slr_fast_edges                      3
 /* aaf_crestf_dbx8                */
 #define    aaf_crestf_dbx8_ADDRESS          416
 #define    aaf_crestf_dbx8_OFFSET             0
 #define    aaf_crestf_dbx8_NBBIT              8
 #define    aaf_crestf_dbx8_ALONE              1
 #define    aaf_crestf_dbx8_SIGNED             0
 /* aci_crestf_dbx8                */
 #define    aci_crestf_dbx8_ADDRESS          456
 #define    aci_crestf_dbx8_OFFSET             0
 #define    aci_crestf_dbx8_NBBIT              8
 #define    aci_crestf_dbx8_ALONE              1
 #define    aci_crestf_dbx8_SIGNED             0
 /* timing_corr_c                  */
 #define    timing_corr_c_ADDRESS            520
 #define    timing_corr_c_OFFSET               0
 #define    timing_corr_c_NBBIT               18
 #define    timing_corr_c_ALONE                1
 #define    timing_corr_c_SIGNED               1
 /* sweep_step                     */
 #define    sweep_step_ADDRESS               562
 #define    sweep_step_OFFSET                  0
 #define    sweep_step_NBBIT                   6
 #define    sweep_step_ALONE                   1
 #define    sweep_step_SIGNED                  1
 /* freq_corr_c                    */
 #define    freq_corr_c_ADDRESS              564
 #define    freq_corr_c_OFFSET                 0
 #define    freq_corr_c_NBBIT                 16
 #define    freq_corr_c_ALONE                  1
 #define    freq_corr_c_SIGNED                 1
 /* kp_lock                        */
 #define    kp_lock_ADDRESS                  570
 #define    kp_lock_OFFSET                     0
 #define    kp_lock_NBBIT                      4
 #define    kp_lock_ALONE                      1
 #define    kp_lock_SIGNED                     1
 /* demod_lock_c                   */
 #define    demod_lock_c_ADDRESS             574
 #define    demod_lock_c_OFFSET                0
 #define    demod_lock_c_NBBIT                 1
 #define    demod_lock_c_ALONE                 1
 #define    demod_lock_c_SIGNED                0
  #define           demod_lock_c_unlocked                      0
  #define           demod_lock_c_locked                        1
 /* phase_cor_c                    */
 #define    phase_cor_c_ADDRESS              576
 #define    phase_cor_c_OFFSET                 0
 #define    phase_cor_c_NBBIT                 16
 #define    phase_cor_c_ALONE                  1
 #define    phase_cor_c_SIGNED                 1
 /* phase_lock_thr                 */
 #define    phase_lock_thr_ADDRESS           588
 #define    phase_lock_thr_OFFSET              0
 #define    phase_lock_thr_NBBIT              16
 #define    phase_lock_thr_ALONE               1
 #define    phase_lock_thr_SIGNED              0
 /* central_tap                    */
 #define    central_tap_ADDRESS              609
 #define    central_tap_OFFSET                 0
 #define    central_tap_NBBIT                  8
 #define    central_tap_ALONE                  1
 #define    central_tap_SIGNED                 0
 /* c_n                            */
 #define    c_n_ADDRESS                      620
 #define    c_n_OFFSET                         0
 #define    c_n_NBBIT                         24
 #define    c_n_ALONE                          1
 #define    c_n_SIGNED                         0
 /* auto_algo                      */
 #define    auto_algo_ADDRESS                632
 #define    auto_algo_OFFSET                   1
 #define    auto_algo_NBBIT                    1
 #define    auto_algo_ALONE                    0
 #define    auto_algo_SIGNED                   0
  #define           auto_algo_dfe_init                         0
  #define           auto_algo_dfe_freeze                       1
 /* cma2dd_thr                     */
 #define    cma2dd_thr_ADDRESS               636
 #define    cma2dd_thr_OFFSET                  0
 #define    cma2dd_thr_NBBIT                  16
 #define    cma2dd_thr_ALONE                   1
 #define    cma2dd_thr_SIGNED                  0
 /* req_constellation              */
 #define    req_constellation_ADDRESS        756
 #define    req_constellation_OFFSET           0
 #define    req_constellation_NBBIT            6
 #define    req_constellation_ALONE            1
 #define    req_constellation_SIGNED           0
  #define           req_constellation_qpsk                     3
  #define           req_constellation_qam16                    7
  #define           req_constellation_qam64                    9
 /* boot_done                      */
 #define    boot_done_ADDRESS                833
 #define    boot_done_OFFSET                   0
 #define    boot_done_NBBIT                    1
 #define    boot_done_ALONE                    0
 #define    boot_done_SIGNED                   0
  #define           boot_done_in_progress                      0
  #define           boot_done_done                             1
 /* rst_wdog_error                 */
 #define    rst_wdog_error_ADDRESS           833
 #define    rst_wdog_error_OFFSET              1
 #define    rst_wdog_error_NBBIT               1
 #define    rst_wdog_error_ALONE               0
 #define    rst_wdog_error_SIGNED              0
  #define           rst_wdog_error_run                         0
  #define           rst_wdog_error_reset                       1
 /* patch_version                  */
 #define    patch_version_ADDRESS            836
 #define    patch_version_OFFSET               0
 #define    patch_version_NBBIT                8
 #define    patch_version_ALONE                1
 #define    patch_version_SIGNED               0
 /* addr_jump                      */
 #define    addr_jump_ADDRESS                840
 #define    addr_jump_OFFSET                   0
 #define    addr_jump_NBBIT                   32
 #define    addr_jump_ALONE                    1
 #define    addr_jump_SIGNED                   0
 /* rst_crc                        */
 #define    rst_crc_ADDRESS                  889
 #define    rst_crc_OFFSET                     0
 #define    rst_crc_NBBIT                      1
 #define    rst_crc_ALONE                      1
 #define    rst_crc_SIGNED                     0
  #define           rst_crc_run                                0
  #define           rst_crc_reset                              1
 /* crc                            */
 #define    crc_ADDRESS                      890
 #define    crc_OFFSET                         0
 #define    crc_NBBIT                         16
 #define    crc_ALONE                          1
 #define    crc_SIGNED                         0
 /* timing_corr_t                  */
 #define    timing_corr_t_ADDRESS            944
 #define    timing_corr_t_OFFSET               0
 #define    timing_corr_t_NBBIT               20
 #define    timing_corr_t_ALONE                1
 #define    timing_corr_t_SIGNED               1
 /* ps_ambig_out                   */
 #define    ps_ambig_out_ADDRESS             1092
 #define    ps_ambig_out_OFFSET                0
 #define    ps_ambig_out_NBBIT                 1
 #define    ps_ambig_out_ALONE                 1
 #define    ps_ambig_out_SIGNED                0
 /* uncor_rst                      */
 #define    uncor_rst_ADDRESS                1124
 #define    uncor_rst_OFFSET                   0
 #define    uncor_rst_NBBIT                    1
 #define    uncor_rst_ALONE                    1
 #define    uncor_rst_SIGNED                   0
  #define           uncor_rst_reset                            1
  #define           uncor_rst_run                              0
 /* uncor_cnt                      */
 #define    uncor_cnt_ADDRESS                1128
 #define    uncor_cnt_OFFSET                   0
 #define    uncor_cnt_NBBIT                    8
 #define    uncor_cnt_ALONE                    1
 #define    uncor_cnt_SIGNED                   0
 /* ber_pkt                        */
 #define    ber_pkt_ADDRESS                  1136
 #define    ber_pkt_OFFSET                     0
 #define    ber_pkt_NBBIT                     16
 #define    ber_pkt_ALONE                      1
 #define    ber_pkt_SIGNED                     0
 /* ber_bit                        */
 #define    ber_bit_ADDRESS                  1144
 #define    ber_bit_OFFSET                     0
 #define    ber_bit_NBBIT                     24
 #define    ber_bit_ALONE                      1
 #define    ber_bit_SIGNED                     0
 /* fec_lock                       */
 #define    fec_lock_ADDRESS                 1248
 #define    fec_lock_OFFSET                    0
 #define    fec_lock_NBBIT                     1
 #define    fec_lock_ALONE                     1
 #define    fec_lock_SIGNED                    0
  #define           fec_lock_unlocked                          0
  #define           fec_lock_locked                            1
 /* ts_data_mode                   */
 #define    ts_data_mode_ADDRESS             1252
 #define    ts_data_mode_OFFSET                0
 #define    ts_data_mode_NBBIT                 1
 #define    ts_data_mode_ALONE                 0
 #define    ts_data_mode_SIGNED                0
  #define           ts_data_mode_serial                        0
  #define           ts_data_mode_parallel                      1
 /* sel_gpio_ts_err                */
 #define    sel_gpio_ts_err_ADDRESS          1259
 #define    sel_gpio_ts_err_OFFSET             0
 #define    sel_gpio_ts_err_NBBIT              1
 #define    sel_gpio_ts_err_ALONE              0
 #define    sel_gpio_ts_err_SIGNED             0
  #define           sel_gpio_ts_err_ts_err                     0
  #define           sel_gpio_ts_err_gpio_2                     1
 /* ts_data0_tri                   */
 #define    ts_data0_tri_ADDRESS             1263
 #define    ts_data0_tri_OFFSET                0
 #define    ts_data0_tri_NBBIT                 1
 #define    ts_data0_tri_ALONE                 0
 #define    ts_data0_tri_SIGNED                0
  #define           ts_data0_tri_normal                        0
  #define           ts_data0_tri_tristate                      1
 /* ts_data1_tri                   */
 #define    ts_data1_tri_ADDRESS             1263
 #define    ts_data1_tri_OFFSET                1
 #define    ts_data1_tri_NBBIT                 1
 #define    ts_data1_tri_ALONE                 0
 #define    ts_data1_tri_SIGNED                0
  #define           ts_data1_tri_normal                        0
  #define           ts_data1_tri_tristate                      1
 /* ts_data2_tri                   */
 #define    ts_data2_tri_ADDRESS             1263
 #define    ts_data2_tri_OFFSET                2
 #define    ts_data2_tri_NBBIT                 1
 #define    ts_data2_tri_ALONE                 0
 #define    ts_data2_tri_SIGNED                0
  #define           ts_data2_tri_normal                        0
  #define           ts_data2_tri_tristate                      1
 /* ts_data3_tri                   */
 #define    ts_data3_tri_ADDRESS             1263
 #define    ts_data3_tri_OFFSET                3
 #define    ts_data3_tri_NBBIT                 1
 #define    ts_data3_tri_ALONE                 0
 #define    ts_data3_tri_SIGNED                0
  #define           ts_data3_tri_normal                        0
  #define           ts_data3_tri_tristate                      1
 /* ts_data4_tri                   */
 #define    ts_data4_tri_ADDRESS             1263
 #define    ts_data4_tri_OFFSET                4
 #define    ts_data4_tri_NBBIT                 1
 #define    ts_data4_tri_ALONE                 0
 #define    ts_data4_tri_SIGNED                0
  #define           ts_data4_tri_normal                        0
  #define           ts_data4_tri_tristate                      1
 /* ts_data5_tri                   */
 #define    ts_data5_tri_ADDRESS             1263
 #define    ts_data5_tri_OFFSET                5
 #define    ts_data5_tri_NBBIT                 1
 #define    ts_data5_tri_ALONE                 0
 #define    ts_data5_tri_SIGNED                0
  #define           ts_data5_tri_normal                        0
  #define           ts_data5_tri_tristate                      1
 /* ts_data6_tri                   */
 #define    ts_data6_tri_ADDRESS             1263
 #define    ts_data6_tri_OFFSET                6
 #define    ts_data6_tri_NBBIT                 1
 #define    ts_data6_tri_ALONE                 0
 #define    ts_data6_tri_SIGNED                0
  #define           ts_data6_tri_normal                        0
  #define           ts_data6_tri_tristate                      1
 /* ts_data7_tri                   */
 #define    ts_data7_tri_ADDRESS             1263
 #define    ts_data7_tri_OFFSET                7
 #define    ts_data7_tri_NBBIT                 1
 #define    ts_data7_tri_ALONE                 0
 #define    ts_data7_tri_SIGNED                0
  #define           ts_data7_tri_normal                        0
  #define           ts_data7_tri_tristate                      1
 /* ts_val_tri                     */
 #define    ts_val_tri_ADDRESS               1264
 #define    ts_val_tri_OFFSET                  0
 #define    ts_val_tri_NBBIT                   1
 #define    ts_val_tri_ALONE                   0
 #define    ts_val_tri_SIGNED                  0
  #define           ts_val_tri_normal                          0
  #define           ts_val_tri_tristate                        1
 /* ts_sync_tri                    */
 #define    ts_sync_tri_ADDRESS              1264
 #define    ts_sync_tri_OFFSET                 1
 #define    ts_sync_tri_NBBIT                  1
 #define    ts_sync_tri_ALONE                  0
 #define    ts_sync_tri_SIGNED                 0
  #define           ts_sync_tri_normal                         0
  #define           ts_sync_tri_tristate                       1
 /* ts_err_tri                     */
 #define    ts_err_tri_ADDRESS               1264
 #define    ts_err_tri_OFFSET                  2
 #define    ts_err_tri_NBBIT                   1
 #define    ts_err_tri_ALONE                   0
 #define    ts_err_tri_SIGNED                  0
  #define           ts_err_tri_normal                          0
  #define           ts_err_tri_tristate                        1
 /* ts_clk_tri                     */
 #define    ts_clk_tri_ADDRESS               1264
 #define    ts_clk_tri_OFFSET                  3
 #define    ts_clk_tri_NBBIT                   1
 #define    ts_clk_tri_ALONE                   0
 #define    ts_clk_tri_SIGNED                  0
  #define           ts_clk_tri_normal                          0
  #define           ts_clk_tri_tristate                        1
 /* ts_data0_slr                   */
 #define    ts_data0_slr_ADDRESS             1268
 #define    ts_data0_slr_OFFSET                0
 #define    ts_data0_slr_NBBIT                 2
 #define    ts_data0_slr_ALONE                 0
 #define    ts_data0_slr_SIGNED                0
  #define           ts_data0_slr_fastest_edges                 0
  #define           ts_data0_slr_slowest_edges                 1
  #define           ts_data0_slr_moderate_edges                2
  #define           ts_data0_slr_fast_edges                    3
 /* ts_data1_slr                   */
 #define    ts_data1_slr_ADDRESS             1268
 #define    ts_data1_slr_OFFSET                2
 #define    ts_data1_slr_NBBIT                 2
 #define    ts_data1_slr_ALONE                 0
 #define    ts_data1_slr_SIGNED                0
  #define           ts_data1_slr_fastest_edges                 0
  #define           ts_data1_slr_slowest_edges                 1
  #define           ts_data1_slr_moderate_edges                2
  #define           ts_data1_slr_fast_edges                    3
 /* ts_data2_slr                   */
 #define    ts_data2_slr_ADDRESS             1268
 #define    ts_data2_slr_OFFSET                4
 #define    ts_data2_slr_NBBIT                 2
 #define    ts_data2_slr_ALONE                 0
 #define    ts_data2_slr_SIGNED                0
  #define           ts_data2_slr_fastest_edges                 0
  #define           ts_data2_slr_slowest_edges                 1
  #define           ts_data2_slr_moderate_edges                2
  #define           ts_data2_slr_fast_edges                    3
 /* ts_data3_slr                   */
 #define    ts_data3_slr_ADDRESS             1268
 #define    ts_data3_slr_OFFSET                6
 #define    ts_data3_slr_NBBIT                 2
 #define    ts_data3_slr_ALONE                 0
 #define    ts_data3_slr_SIGNED                0
  #define           ts_data3_slr_fastest_edges                 0
  #define           ts_data3_slr_slowest_edges                 1
  #define           ts_data3_slr_moderate_edges                2
  #define           ts_data3_slr_fast_edges                    3
 /* ts_data4_slr                   */
 #define    ts_data4_slr_ADDRESS             1269
 #define    ts_data4_slr_OFFSET                0
 #define    ts_data4_slr_NBBIT                 2
 #define    ts_data4_slr_ALONE                 0
 #define    ts_data4_slr_SIGNED                0
  #define           ts_data4_slr_fastest_edges                 0
  #define           ts_data4_slr_slowest_edges                 1
  #define           ts_data4_slr_moderate_edges                2
  #define           ts_data4_slr_fast_edges                    3
 /* ts_data5_slr                   */
 #define    ts_data5_slr_ADDRESS             1269
 #define    ts_data5_slr_OFFSET                2
 #define    ts_data5_slr_NBBIT                 2
 #define    ts_data5_slr_ALONE                 0
 #define    ts_data5_slr_SIGNED                0
  #define           ts_data5_slr_fastest_edges                 0
  #define           ts_data5_slr_slowest_edges                 1
  #define           ts_data5_slr_moderate_edges                2
  #define           ts_data5_slr_fast_edges                    3
 /* ts_data6_slr                   */
 #define    ts_data6_slr_ADDRESS             1269
 #define    ts_data6_slr_OFFSET                4
 #define    ts_data6_slr_NBBIT                 2
 #define    ts_data6_slr_ALONE                 0
 #define    ts_data6_slr_SIGNED                0
  #define           ts_data6_slr_fastest_edges                 0
  #define           ts_data6_slr_slowest_edges                 1
  #define           ts_data6_slr_moderate_edges                2
  #define           ts_data6_slr_fast_edges                    3
 /* ts_data7_slr                   */
 #define    ts_data7_slr_ADDRESS             1269
 #define    ts_data7_slr_OFFSET                6
 #define    ts_data7_slr_NBBIT                 2
 #define    ts_data7_slr_ALONE                 0
 #define    ts_data7_slr_SIGNED                0
  #define           ts_data7_slr_fastest_edges                 0
  #define           ts_data7_slr_slowest_edges                 1
  #define           ts_data7_slr_moderate_edges                2
  #define           ts_data7_slr_fast_edges                    3
 /* ts_val_slr                     */
 #define    ts_val_slr_ADDRESS               1270
 #define    ts_val_slr_OFFSET                  0
 #define    ts_val_slr_NBBIT                   2
 #define    ts_val_slr_ALONE                   0
 #define    ts_val_slr_SIGNED                  0
  #define           ts_val_slr_fastest_edges                   0
  #define           ts_val_slr_slowest_edges                   1
  #define           ts_val_slr_moderate_edges                  2
  #define           ts_val_slr_fast_edges                      3
 /* ts_sync_slr                    */
 #define    ts_sync_slr_ADDRESS              1270
 #define    ts_sync_slr_OFFSET                 2
 #define    ts_sync_slr_NBBIT                  2
 #define    ts_sync_slr_ALONE                  0
 #define    ts_sync_slr_SIGNED                 0
  #define           ts_sync_slr_fastest_edges                  0
  #define           ts_sync_slr_slowest_edges                  1
  #define           ts_sync_slr_moderate_edges                 2
  #define           ts_sync_slr_fast_edges                     3
 /* ts_err_slr                     */
 #define    ts_err_slr_ADDRESS               1270
 #define    ts_err_slr_OFFSET                  4
 #define    ts_err_slr_NBBIT                   2
 #define    ts_err_slr_ALONE                   0
 #define    ts_err_slr_SIGNED                  0
  #define           ts_err_slr_fastest_edges                   0
  #define           ts_err_slr_slowest_edges                   1
  #define           ts_err_slr_moderate_edges                  2
  #define           ts_err_slr_fast_edges                      3
 /* ts_clk_slr                     */
 #define    ts_clk_slr_ADDRESS               1270
 #define    ts_clk_slr_OFFSET                  6
 #define    ts_clk_slr_NBBIT                   2
 #define    ts_clk_slr_ALONE                   0
 #define    ts_clk_slr_SIGNED                  0
  #define           ts_clk_slr_fastest_edges                   0
  #define           ts_clk_slr_slowest_edges                   1
  #define           ts_clk_slr_moderate_edges                  2
  #define           ts_clk_slr_fast_edges                      3
 /* gp0_sel                        */
 #define    gp0_sel_ADDRESS                  1457
 #define    gp0_sel_OFFSET                     0
 #define    gp0_sel_NBBIT                      2
 #define    gp0_sel_ALONE                      0
 #define    gp0_sel_SIGNED                     0
  #define           gp0_sel_gp_o                               0
  #define           gp0_sel_interrupt                          1
  #define           gp0_sel_deltasigma                         2
  #define           gp0_sel_clock                              3
 /* gp0_p                          */
 #define    gp0_p_ADDRESS                    1457
 #define    gp0_p_OFFSET                       2
 #define    gp0_p_NBBIT                        1
 #define    gp0_p_ALONE                        0
 #define    gp0_p_SIGNED                       0
  #define           gp0_p_non_inverted                         0
  #define           gp0_p_inverted                             1
 /* gp0_t                          */
 #define    gp0_t_ADDRESS                    1457
 #define    gp0_t_OFFSET                       3
 #define    gp0_t_NBBIT                        1
 #define    gp0_t_ALONE                        0
 #define    gp0_t_SIGNED                       0
  #define           gp0_t_cmos                                 0
  #define           gp0_t_open_drain                           1
 /* gp0_en                         */
 #define    gp0_en_ADDRESS                   1457
 #define    gp0_en_OFFSET                      4
 #define    gp0_en_NBBIT                       1
 #define    gp0_en_ALONE                       0
 #define    gp0_en_SIGNED                      0
  #define           gp0_en_disable                             0
  #define           gp0_en_enable                              1
 /* fecl0_e                        */
 #define    fecl0_e_ADDRESS                  1462
 #define    fecl0_e_OFFSET                     0
 #define    fecl0_e_NBBIT                      1
 #define    fecl0_e_ALONE                      1
 #define    fecl0_e_SIGNED                     0
  #define           fecl0_e_disable                            0
  #define           fecl0_e_enable                             1
 /* gpio0_tri                      */
 #define    gpio0_tri_ADDRESS                1473
 #define    gpio0_tri_OFFSET                   0
 #define    gpio0_tri_NBBIT                    1
 #define    gpio0_tri_ALONE                    1
 #define    gpio0_tri_SIGNED                   0
  #define           gpio0_tri_normal                           0
  #define           gpio0_tri_tristate                         1
 /* gpio0_slr                      */
 #define    gpio0_slr_ADDRESS                1482
 #define    gpio0_slr_OFFSET                   0
 #define    gpio0_slr_NBBIT                    2
 #define    gpio0_slr_ALONE                    1
 #define    gpio0_slr_SIGNED                   0
  #define           gpio0_slr_fastest_edges                    0
  #define           gpio0_slr_slowest_edges                    1
  #define           gpio0_slr_moderate_edges                   2
  #define           gpio0_slr_fast_edges                       3
 /* ts_parallel_mode               */
 #define    ts_parallel_mode_ADDRESS         2296
 #define    ts_parallel_mode_OFFSET          0
 #define    ts_parallel_mode_NBBIT           3
 #define    ts_parallel_mode_ALONE           1
 #define    ts_parallel_mode_SIGNED          0
  #define           ts_parallel_mode_mode1                     0
  #define           ts_parallel_mode_mode2_div6                4
  #define           ts_parallel_mode_mode2_div8                5
  #define           ts_parallel_mode_mode2_div12               6
  #define           ts_parallel_mode_mode2_div16               7 
  
 /* chip_mode                      */
 #define    chip_mode_ADDRESS                  0
 #define    chip_mode_OFFSET                   0
 #define    chip_mode_NBBIT                    7
 #define    chip_mode_ALONE                    1
 #define    chip_mode_SIGNED                   0
  #define           chip_mode_off                              0
  #define           chip_mode_pll_ext                         32
  #define           chip_mode_pll_xtal                        33

 /* symb_iq_2                      */
 #define    symb_iq_2_ADDRESS                660
 #define    symb_iq_2_OFFSET                   0
 #define    symb_iq_2_NBBIT                   32
 #define    symb_iq_2_ALONE                    1
 #define    symb_iq_2_SIGNED                   0

 /* en_rst_error                   */
 #define    en_rst_error_ADDRESS             860
 #define    en_rst_error_OFFSET                0
 #define    en_rst_error_NBBIT                 1
 #define    en_rst_error_ALONE                 0
 #define    en_rst_error_SIGNED                0
  #define           en_rst_error_no_reset_on_errorn            0
  #define           en_rst_error_reset_on_errorn               1

 /* dcom_control_byte              */
 #define    dcom_control_byte_ADDRESS        868
 #define    dcom_control_byte_OFFSET           0
 #define    dcom_control_byte_NBBIT           32
 #define    dcom_control_byte_ALONE            1
 #define    dcom_control_byte_SIGNED           0
 /* dcom_addr                      */
 #define    dcom_addr_ADDRESS                872
 #define    dcom_addr_OFFSET                   0
 #define    dcom_addr_NBBIT                   32
 #define    dcom_addr_ALONE                    1
 #define    dcom_addr_SIGNED                   0
 /* dcom_data                      */
 #define    dcom_data_ADDRESS                876
 #define    dcom_data_OFFSET                   0
 #define    dcom_data_NBBIT                   32
 #define    dcom_data_ALONE                    1
 #define    dcom_data_SIGNED                   0

/* addition to speed up ts pins setup and reduce L1_Demod_TS_mode code size */
 #define    ts_tri_ADDRESS                 ts_data0_tri_ADDRESS
 #define    ts_tri_OFFSET                0
 #define    ts_tri_NBBIT                12
 #define    ts_tri_ALONE                 1
 #define    ts_tri_SIGNED                0
  #define           ts_tri_serial                        0x00fe
  #define           ts_tri_all_tristate                  0xff0f
  #define           ts_tri_parallel                      0x0000

 #define    ts_slr_ADDRESS                 ts_data0_slr_ADDRESS
 #define    ts_slr_OFFSET                0
 #define    ts_slr_NBBIT                24
 #define    ts_slr_ALONE                 1
 #define    ts_slr_SIGNED                0
  #define           ts_slr_all_slowest_edges             0x555555
/* end of to speed up ts pins setup and reduce L1_Demod_TS_mode code size */

#endif /* end of _L1_S1216x_Defines_H_ */

