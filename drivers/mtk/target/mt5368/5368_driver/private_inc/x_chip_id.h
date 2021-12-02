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

#define BIM_CUST_KEY_ADDR   0x9f00
#define BIM_MTK_KEY_ADDR    0xf4003c00

#define REG_EFUSE_BONDING           (0x0670)
    #define BOND_QJY_BIT            (1U << 30)
    #define BOND_H264_BIT           (1U << 29)
    #define BOND_ANDRIOD_BIT        (1U << 28)
    #define BOND_CINEMA_BIT         (1U << 27)
    #define BOND_TSIN_BIT           (1U << 26)
    #define BOND_DIMMING_BIT        (1U << 25)
    #define BOND_120HZ_BIT          (1U << 24)
    #define BOND_3D_BIT             (1U << 23)
    #define BOND_DVB_BIT            (1U << 22)
    #define BOND_BRAZIL_BIT         (1U << 21)
    #define BOND_ATSC_BIT           (1U << 20)
    #define BOND_DMBT_BIT           (1U << 19)
    #define BOND_H264EN_BIT         (1U << 18)
    #define BOND_TCON_BIT           (1U << 17)
    #define BOND_NETWORK_BIT        (1U << 16)
    #define BOND_MHEGHD_BIT         (1U << 15)
    #define BOND_OPERA_BIT          (1U << 14)
    #define BOND_JVM_BIT            (1U << 13)
    #define BOND_ARIB_BIT           (1U << 12)
    #define BOND_GINGA_BIT          (1U << 11)
    #define BOND_MHP_BIT            (1U << 10)
    #define BOND_HBBTV_BIT          (1U << 9)
    #define BOND_RM_BIT             (1U << 8)
    #define BOND_AVS_BIT            (1U << 7)
    #define BOND_VC1_BIT            (1U << 6)
    #define BOND_DIVXHD_BIT         (1U << 5)
    #define BOND_VP8_BIT            (1U << 4)
    #define BOND_CPU1G_BIT          (1U << 3)
    #define BOND_3DGPU_BIT          (1U << 2)
    #define BOND_DTS_BIT            (1U << 1)
    #define BOND_DOLBY_BIT          (1U << 0)

    #define BONDING_MASK            (BOND_ANDRIOD_BIT | BOND_CINEMA_BIT | BOND_TSIN_BIT | BOND_DIMMING_BIT | BOND_120HZ_BIT | BOND_3D_BIT | BOND_DVB_BIT | BOND_BRAZIL_BIT | BOND_ATSC_BIT | BOND_DMBT_BIT \
                                     | BOND_H264EN_BIT | BOND_TCON_BIT | BOND_NETWORK_BIT | BOND_MHEGHD_BIT | BOND_OPERA_BIT \
                                     | BOND_JVM_BIT | BOND_ARIB_BIT | BOND_GINGA_BIT | BOND_MHP_BIT | BOND_HBBTV_BIT \
                                     | BOND_RM_BIT | BOND_AVS_BIT | BOND_VC1_BIT | BOND_DIVXHD_BIT | BOND_VP8_BIT \
                                     | BOND_CPU1G_BIT | BOND_3DGPU_BIT | BOND_DTS_BIT | BOND_DOLBY_BIT | BOND_H264_BIT | BOND_QJY_BIT)


EXTERN UINT32 GET_CHIPID(VOID);
EXTERN UINT32 IS_SupportQJY(VOID);
EXTERN UINT32 IS_SupportH264(VOID);
EXTERN UINT32 IS_SupportAndroid(VOID);
EXTERN UINT32 IS_SupportCINEMA(VOID);
EXTERN UINT32 IS_SupportTsIn(VOID);
EXTERN UINT32 IS_SupportDimming(VOID);
EXTERN UINT32 IS_Support120HZ(VOID);
EXTERN UINT32 IS_Support3D(VOID);
EXTERN UINT32 IS_SupportDVB(VOID);
EXTERN UINT32 IS_SupportBRAZIL(VOID);
EXTERN UINT32 IS_SupportATSC(VOID);
EXTERN UINT32 IS_SupportDMBT(VOID);
EXTERN UINT32 IS_SupportH264EN(VOID);
EXTERN UINT32 IS_SupportTCON(VOID);
EXTERN UINT32 IS_SupportCHINATV(VOID);
EXTERN UINT32 IS_SupportNETWORK(VOID);
EXTERN UINT32 IS_SupportMHEGHD(VOID);
EXTERN UINT32 IS_SupportOPERA(VOID);
EXTERN UINT32 IS_SupportJVM(VOID);
EXTERN UINT32 IS_SupportARIB(VOID);
EXTERN UINT32 IS_SupportGINGA(VOID);
EXTERN UINT32 IS_SupportMHP(VOID);
EXTERN UINT32 IS_SupportHBBTV(VOID);
EXTERN UINT32 IS_SupportRM(VOID);
EXTERN UINT32 IS_SupportAVS(VOID);
EXTERN UINT32 IS_SupportVC1(VOID);
EXTERN UINT32 IS_SupportDivXHD(VOID);
EXTERN UINT32 IS_SupportVP8(VOID);
EXTERN UINT32 IS_SupportCPU1G(VOID);
EXTERN UINT32 IS_Support3DGPU(VOID);
EXTERN UINT32 IS_SupportDTS(VOID);
EXTERN UINT32 IS_SupportDOLBY(VOID);

EXTERN UINT32 IS_SupportEAC3(VOID);
EXTERN UINT32 IS_SupportDivX(VOID);
EXTERN UINT32 IS_SupportAC3(VOID);
EXTERN UINT32 IS_SupportDDCO(VOID);
EXTERN UINT32 IS_SupportMS10(VOID);
EXTERN UINT32 IS_SupportCIPlus(VOID);

EXTERN UINT32 IS_MHEG5Support(VOID);
EXTERN UINT32 IS_MultiMediaSupport(VOID);
EXTERN UINT32 _IS_RDES(VOID);
EXTERN UINT32 GET_EFUSE_CHIPID(VOID);


EXTERN INT32 BIM_GetDeviceData(BYTE *pu1DeviceID, BYTE u1Size); //128byte device id from rom code 0x0
EXTERN INT32 BIM_GetDeviceID(BYTE *pu1DeviceID, BYTE u1Size); // 8byte device id from efuse 0x868C
EXTERN INT32 BIM_GetSecureID(BYTE *pu1SecureID, BYTE u1Size); // 128bit random key from efuse 0x8678

#endif /* X_CHIP_ID_H */

