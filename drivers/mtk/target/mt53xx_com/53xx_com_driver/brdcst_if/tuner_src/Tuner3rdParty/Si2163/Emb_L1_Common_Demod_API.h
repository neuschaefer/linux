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
#ifndef  _L1_Common_Demod_API_H_
#define _L1_Common_Demod_API_H_

#define   SYSTEM_WAIT
#ifdef    SYSTEM_WAIT
    int system_wait(int time_ms);
#endif /* #ifdef SYSTEM_WAIT */

#ifdef __cplusplus
extern "C" {
#endif

    /****************************************************************/
    /*  DLL init function : MUST be called FIRST and ONCE ONLY!     */
    /****************************************************************/
    EMB_L1_COMMON_DEMOD_API char   SLAB_WINAPI L1_Demod_SW_Init           (L1_Context **ppDemod, unsigned int add, int standard);
    EMB_L1_COMMON_DEMOD_API char   SLAB_WINAPI L1_Demod_SW_Destroy        (L1_Context *demod);
    EMB_L1_COMMON_DEMOD_API void   SLAB_WINAPI L1_DEMOD_Fix               (L1_Context *demod);

    /****************************************************************/
    /*  Reset functions                                             */
    /****************************************************************/
    EMB_L1_COMMON_DEMOD_API void   SLAB_WINAPI L1_Demod_reset_uncorrs     (L1_Context *demod);

    /****************************************************************/
    /*  Demod setup functions                                       */
    /****************************************************************/
    EMB_L1_COMMON_DEMOD_API int    SLAB_WINAPI L1_Demod_set_ber_depth     (L1_Context *demod, int ber_depth);
    EMB_L1_COMMON_DEMOD_API void   SLAB_WINAPI L1_Demod_set_adc_regs      (L1_Context *demod);

    /****************************************************************/
    /*  Registers setup functions                                   */
    /****************************************************************/
    EMB_L1_COMMON_DEMOD_API long   SLAB_WINAPI L1_Demod_Set_dvb_rate       (L1_Context *demod);
    EMB_L1_COMMON_DEMOD_API long   SLAB_WINAPI L1_Demod_Set_FE_clk         (L1_Context *demod);
    EMB_L1_COMMON_DEMOD_API long   SLAB_WINAPI L1_Demod_Set_oversamp       (L1_Context *demod);
    EMB_L1_COMMON_DEMOD_API long   SLAB_WINAPI L1_Demod_Set_Digital_IF_Hz  (L1_Context *demod, long digital_if);
    EMB_L1_COMMON_DEMOD_API long   SLAB_WINAPI L1_Demod_Set_if_freq_shift  (L1_Context *demod);
    EMB_L1_COMMON_DEMOD_API void   SLAB_WINAPI L1_Demod_mode               (L1_Context *demod, int mode);
    EMB_L1_COMMON_DEMOD_API int    SLAB_WINAPI L1_Demod_TS_mode            (L1_Context *demod, int ts_mode);
    EMB_L1_COMMON_DEMOD_API void   SLAB_WINAPI L1_Demod_SetTSInterface     (L1_Context *demod, unsigned char fgParallel);

    /****************************************************************/
    /*  Demod specific parameters readout                           */
    /****************************************************************/
    EMB_L1_COMMON_DEMOD_API int    SLAB_WINAPI L1_Demod_current_standard   (L1_Context *demod);
    EMB_L1_COMMON_DEMOD_API int    SLAB_WINAPI L1_Demod_current_bandwidth  (L1_Context *demod);
    EMB_L1_COMMON_DEMOD_API long   SLAB_WINAPI L1_Demod_current_symbol_rate(L1_Context *demod);

    /****************************************************************/
    /*  Demod monitoring functions                                  */
    /****************************************************************/
    EMB_L1_COMMON_DEMOD_API char   SLAB_WINAPI L1_Demod_get_fec_lock      (L1_Context *demod);
    EMB_L1_COMMON_DEMOD_API int    SLAB_WINAPI L1_Demod_get_ber           (L1_Context *demod);
    EMB_L1_COMMON_DEMOD_API int    SLAB_WINAPI L1_Demod_get_uncorrs       (L1_Context *demod);
    EMB_L1_COMMON_DEMOD_API int    SLAB_WINAPI L1_Demod_get_RFagc         (L1_Context *demod);
    EMB_L1_COMMON_DEMOD_API int    SLAB_WINAPI L1_Demod_get_IFagc         (L1_Context *demod);
#ifdef    INPUT_LEVEL
    EMB_L1_COMMON_DEMOD_API void   SLAB_WINAPI L1_Demod_get_InputLevel    (L1_Context *demod, float *input_level);
#endif /* INPUT_LEVEL */
    EMB_L1_COMMON_DEMOD_API int    SLAB_WINAPI L1_Demod_GetSignalLevel    (L1_Context *demod);
    EMB_L1_COMMON_DEMOD_API int    SLAB_WINAPI L1_Demod_GetSignalPER      (L1_Context *demod);
#ifdef    COMMAND_LINE_APPLICATION
    EMB_L1_COMMON_DEMOD_API char*  SLAB_WINAPI L1_Context_info            (L1_Context *demod);
#endif /* COMMAND_LINE_APPLICATION */

#define   NO_MATH_H_APPLICATION
#ifdef    NO_MATH_H_APPLICATION
    EMB_L1_COMMON_DEMOD_API int  SLAB_WINAPI L1_Log10_int(int x);
#endif /* NO_MATH_H_APPLICATION */

#ifdef __cplusplus
}
#endif

#endif /* end of _L1_Common_Demod_API_H_ */
