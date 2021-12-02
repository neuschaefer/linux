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

#ifndef X_CHIP_ID_H
#define X_CHIP_ID_H

#include "x_typedef.h"

// MT538x & 5335~7
#define REG_BONDING             0x0000
    #define BOND_BIM_BD0        (1U << 0)
    #define BOND_BGA_BD1        (1U << 1)
    #define BOND_KEY1_BD2       (1U << 2)
    #define BOND_KEY2_BD3       (1U << 3)
    #define BOND_MULTIMEDIA     (1U << 9)
    #define BOND_MHEG5          (1U << 10)
    #define ALL_IC_BONDINGS     (0x7U)
    #define ALL_P_IC_BONDINGS   (0xfU)
#define REG_KEY_BONDING         0x0010

#define MT5335_BOND_ID          (BOND_BIM_BD0)
#define MT5336_BOND_ID          (BOND_BIM_BD0 | BOND_BGA_BD1)
#define MT5337_BOND_ID          (BOND_BIM_BD0 | BOND_BGA_BD1 | BOND_KEY1_BD2)
#define MT5380C_BOND_ID         (BOND_KEY1_BD2)
#define MT5380_BOND_ID          (0)
#define MT5381_BOND_ID          (BOND_BGA_BD1)
#define MT5382_BOND_ID          (BOND_BGA_BD1 | BOND_KEY1_BD2)

#define MT5335P_BOND_ID         (BOND_KEY2_BD3 | BOND_BIM_BD0)
#define MT5336P_BOND_ID         (BOND_KEY2_BD3 | BOND_BIM_BD0 | BOND_BGA_BD1)
#define MT5337P_BOND_ID         (BOND_KEY2_BD3 | BOND_BIM_BD0 | BOND_BGA_BD1 | BOND_KEY1_BD2)
#define MT5380CP_BOND_ID        (BOND_KEY2_BD3 | BOND_KEY1_BD2)
#define MT5380P_BOND_ID         (BOND_KEY2_BD3 | 0)
#define MT5381P_BOND_ID         (BOND_KEY2_BD3 | BOND_BGA_BD1)
#define MT5382P_BOND_ID         (BOND_KEY2_BD3 | BOND_BGA_BD1 | BOND_KEY1_BD2)

// MT537x/MT5330~3
#define IS_MT5330()             (0)
#define IS_MT5331()             (0)
#define IS_MT5332()             (0)
#define IS_MT5333()             (0)
#define IS_MT5370()             (0)
#define IS_MT5371()             (0)
#define IS_MT5372()             (0)
#define IS_MT5373()             (0)

// MT538x
EXTERN UINT32 GET_CHIPID(void);
EXTERN UINT32 IS_MT5380C(void);
EXTERN UINT32 IS_MT5380(void);
EXTERN UINT32 IS_MT5381(void);
EXTERN UINT32 IS_MT5382(void);
EXTERN UINT32 IS_MT5335(void);
EXTERN UINT32 IS_MT5336(void);
EXTERN UINT32 IS_MT5337(void);
EXTERN UINT32 IS_MT5380CP(void);
EXTERN UINT32 IS_MT5380P(void);
EXTERN UINT32 IS_MT5381P(void);
EXTERN UINT32 IS_MT5382P(void);
EXTERN UINT32 IS_MT5335P(void);
EXTERN UINT32 IS_MT5336P(void);
EXTERN UINT32 IS_MT5337P(void);
EXTERN INT32 IS_MultiMediaSupport(void);
EXTERN INT32 IS_SurroundSupport(void);
EXTERN INT32 IS_MHEG5Support(void);
EXTERN UINT32 CHIP_GetSwChipId(void);
EXTERN void CHIP_CheckBoard(void);

#endif /* X_CHIP_ID_H */

