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
#ifndef  _L1_DVB_C_Demod_API_H_
#define _L1_DVB_C_Demod_API_H_

/* Layer 1 static defines */
#define DVB_C_ADDR_JUMP_REV_C                       0x00000000
#define DVB_C_ADDR_JUMP_REV_D                       0xF4000000
#define DEFAULT_DVB_C_CONSTELLATION constellation_qam256
#define DEFAULT_DVB_C_SYMBOL_RATE                    7
/* End of Layer 1 static defines */

typedef struct DVB_C_Status {
  long     symbol_rate;
  int      constellation;
  int      spectral_inversion;
} DVB_C_Status;

#ifdef __cplusplus
extern "C" {
#endif

    /* DVB-C re-initialisation function (to call after each HW reset or power cycle) */
    EMB_L1_DVB_C_DEMOD_API char   SLAB_WINAPI L1_DVB_C_Re_Init                       (L1_Context *demod);

    /* DVB-C reset function */
    EMB_L1_DVB_C_DEMOD_API char   SLAB_WINAPI L1_DVB_C_Reset                         (L1_Context *demod);

    /* DVB-C specific setup functions                                                */
    EMB_L1_DVB_C_DEMOD_API int    SLAB_WINAPI L1_DVB_C_set_constellation             (L1_Context *demod, int   constel);
    EMB_L1_DVB_C_DEMOD_API long   SLAB_WINAPI L1_DVB_C_set_symbol_rate               (L1_Context *demod, long symbol_rate);
    EMB_L1_DVB_C_DEMOD_API long   SLAB_WINAPI L1_DVB_C_Set_dvb_rate                  (L1_Context *demod);

    /* DVB-C global statusing functions                                              */
    EMB_L1_DVB_C_DEMOD_API char   SLAB_WINAPI L1_DVB_C_get_status                    (L1_Context *demod, DVB_C_Status  **status);
    EMB_L1_DVB_C_DEMOD_API long   SLAB_WINAPI L1_DVB_C_corrected_symbol_rate         (L1_Context *demod);
#ifdef    COMMAND_LINE_APPLICATION
    EMB_L1_DVB_C_DEMOD_API char   SLAB_WINAPI L1_DVB_C_get_text_status               (L1_Context *demod, char** formatted_status);
    EMB_L1_DVB_C_DEMOD_API char   SLAB_WINAPI L1_DVB_C_format_text_status            (DVB_C_Status *status, char** formatted_status);
    EMB_L1_DVB_C_DEMOD_API char   SLAB_WINAPI L1_DVB_C_zapping_info                  (DVB_C_Status *status, char** formatted_infos);
#endif /* COMMAND_LINE_APPLICATION */

    /* General status functions (present for each standard)                          */
    EMB_L1_DVB_C_DEMOD_API int    SLAB_WINAPI L1_DVB_C_get_C_N_dB                    (L1_Context *demod);
    EMB_L1_DVB_C_DEMOD_API int    SLAB_WINAPI L1_DVB_C_get_C_N_dB_int                (L1_Context *demod);
    EMB_L1_DVB_C_DEMOD_API char   SLAB_WINAPI L1_DVB_C_get_demod_lock                (L1_Context *demod);
    EMB_L1_DVB_C_DEMOD_API long   SLAB_WINAPI L1_DVB_C_get_timing_offset             (L1_Context *demod);
    EMB_L1_DVB_C_DEMOD_API long   SLAB_WINAPI L1_DVB_C_get_freq_offset               (L1_Context *demod);

    /* DVB-C specific status functions                                               */
    EMB_L1_DVB_C_DEMOD_API int    SLAB_WINAPI L1_DVB_C_get_constellation             (L1_Context *demod);
    EMB_L1_DVB_C_DEMOD_API int    SLAB_WINAPI L1_DVB_C_get_spectral_inversion        (L1_Context *demod);
    EMB_L1_DVB_C_DEMOD_API unsigned long SLAB_WINAPI L1_DVB_C_get_constellation_point(L1_Context *demod, int *i, int *q);

#ifdef __cplusplus
}
#endif

#endif /* end of _L1_DVB_C_Demod_API_H_ */
