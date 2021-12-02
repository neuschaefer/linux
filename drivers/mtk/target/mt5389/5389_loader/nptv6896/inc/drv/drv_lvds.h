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

/**
 * @file LVDS driver interface
 *
 * @author dragon_chen@mtk.com.tw
 */

#ifndef _DRV_LVDS_H_
#define _DRV_LVDS_H_

#define LVDSA_EN_REST_DELAYTIME 20

extern void vDrvLVDSInit(void);
extern void vDrvLVDSOff(void) ;
extern void vDrvLVDSOn(void) ;
extern void vDrvLVDSConfig(void) ;
extern void vDrvLVDSACDRst_Enable(void) ;
extern void vDrvLVDSACDRst_Disable(void) ;
extern void LVDS_SwitchDataEnable(BOOL fEnable);
extern void vSetPI_GROUP(UINT8 u1AEven, UINT8 u1AODD, UINT8 u1BEven, UINT8 u1BOdd);
extern UINT8 vGetPI_GROUP(UINT8 u1Link);


extern UINT8 vDrvLVDSGetDriving(void);
extern void vDrvLVDSSetDriving(UINT8 bDriving);
extern void vDrvLVDSSetVCM(UINT8 bVCM);
extern UINT8 vDrvLVDSGetVCM(void);
extern void vDrvSetCurLvdsMode(UINT32 u4LvdsMode);
extern UINT32 vDrvGetCurLvdsMode(void);
extern void vDrvLVDSChgMod(UINT32 u4Mode);
extern void vDrvSetLvdsColorDomain(UINT32 u4ColorDomain);
extern UINT32 vDrvGetLvdsColorDomain(void);
extern void vDrvLVDS7To4FifoEnable(void);
//automation cli
extern void LVDS_ATERM_ATVO_Setting(UINT8 mode);
extern void LVDS_ATERM_ATVO_Restore(UINT8 ATERN, UINT8 ATVO);
extern void LVDS_ATVO_Set(UINT8 mode);
extern void LVDS_APSRC_Set(UINT8 mode);
extern void LVDS_ANSRC_Set(UINT8 mode);
extern void LVDS_PADPD_Set(UINT8 mode);
extern void LVDS_RESET_Set(UINT8 mode);
extern void MLVDS_LS_Set(UINT8 mode);
extern void MLVDS_TestMODE_Set(UINT8 mode);
extern void vDrvVCOPhase_SEL(UINT8 phase);
extern void SET_DDDSFreeRun(void);
extern void u1TCONIFSourceSet(UINT8 val);
extern UINT8 u1TCONIFSourceGet(void);
extern void vTXCRCStatus(void);
extern void u1TCONIFLineBufferSet(UINT8 val);
extern UINT8 u1TCONIFLineBufferGet(void);

#ifdef CC_VESTEL_ESD_TEST
extern void u1LVDSClockSkew_Recover(void);
extern void u1LVDSClockSkew_Reset(void);
extern void u1CheckLVDSClockSkew_Enable(UINT8 u1OnOff);
extern UINT8 u1ChkLVDSClockSkewStatus(void);
#endif

#endif
