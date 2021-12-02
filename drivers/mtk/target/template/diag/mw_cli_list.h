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

extern CLI_EXEC_T *CLI_Gfx_function(void);
extern CLI_EXEC_T *CLI_GfxEmu_function(void);
extern CLI_EXEC_T *CLI_GfxSc_function(void);
extern CLI_EXEC_T *CLI_GfxScEmu_function(void);
extern CLI_EXEC_T *CLI_Osd_function(void);
extern CLI_EXEC_T *CLI_OsdDiag_function(void);
extern CLI_EXEC_T *CLI_Pmx_function(void);
extern CLI_EXEC_T *CLI_rMUSB_function(void);
extern CLI_EXEC_T *CLI_Fci_function(void);
extern CLI_EXEC_T *CLI_Uart_function(void);
extern CLI_EXEC_T *CLI_Psipsr_function(void);
extern CLI_EXEC_T *CLI_Nav_function(void);
extern CLI_EXEC_T *CLI_Sif_function(void);
extern CLI_EXEC_T *CLI_Eeprom_function(void);
extern CLI_EXEC_T *CLI_Stg_function(void);
extern CLI_EXEC_T *CLI_Nim_function(void);
extern CLI_EXEC_T *CLI_Ir_function(void);
extern CLI_EXEC_T *CLI_Rtc_function(void);
extern CLI_EXEC_T *CLI_Aud_function(void);
extern CLI_EXEC_T *CLI_Nptv_function(void);
extern CLI_EXEC_T *CLI_Av_function(void);
extern CLI_EXEC_T *CLI_Vdp_function(void);
extern CLI_EXEC_T *CLI_Fbm_function(void);
extern CLI_EXEC_T *CLI_Dbs_function(void);
extern CLI_EXEC_T *CLI_Mpv_function(void);
extern CLI_EXEC_T *CLI_LZHS_function(void);
extern CLI_EXEC_T *CLI_Nand_function(void);
extern CLI_EXEC_T *CLI_Dmx_function(void);
extern CLI_EXEC_T *CLI_MemTest_function(void);
extern CLI_EXEC_T *CLI_Pdwnc_function(void);
extern CLI_EXEC_T *CLI_Bwt_function(void);
extern CLI_EXEC_T *CLI_Jpg_function(void);
extern CLI_EXEC_T *CLI_Gpio_function(void);
extern CLI_EXEC_T *CLI_Nor_function(void);
extern CLI_EXEC_T *CLI_Mid_function(void);
#ifdef DRV_SUPPORT_EXTMJC
extern CLI_EXEC_T *CLI_Extmjc_function(void);
#endif

static const CLI_GET_CMD_TBL_FUNC _pfCliGetTbl[] = {
#if !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE)
CLI_Gfx_function,
// CLI_GfxEmu_function,
// CLI_GfxSc_function,
// CLI_GfxScEmu_function,
CLI_Osd_function,
#endif /* !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE) */
CLI_Pmx_function,
#if !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE)
/*
CLI_OsdDiag_function,
CLI_Fci_function,
CLI_Uart_function,
CLI_Psipsr_function,
CLI_Nav_function,
*/
CLI_rMUSB_function,
#endif /* !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE) */
// CLI_Sif_function,
#if !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE)
// CLI_Eeprom_function,
// CLI_Stg_function,
#endif /* !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE) */
// CLI_Nim_function,
#if !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE)
// CLI_Ir_function,
// CLI_Rtc_function,
// CLI_Aud_function,
// CLI_Nptv_function,
// CLI_Av_function,
// CLI_Vdp_function,
// CLI_Fbm_function,
// CLI_Dbs_function,
CLI_Mpv_function,
CLI_LZHS_function,
CLI_Nand_function,
CLI_Dmx_function,
CLI_MemTest_function,
// CLI_Pdwnc_function,
// CLI_Bwt_function,
// CLI_Jpg_function,
// CLI_Gpio_function,
// CLI_Nor_function,
// CLI_Mid_function,
#ifdef DRV_SUPPORT_EXTMJC
CLI_Extmjc_function,
#endif
#endif /* !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE) */
};
