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
* $Author: dtvbm11 $
* $Date: 2012/04/27 $
* $RCSfile: avl_reg.h,v $
* $Revision: #1 $
*
* Description:
*         AV.link register settings
*---------------------------------------------------------------------------*/
#ifndef _HW_AVLINK_H
#define _HW_AVLINK_H

#define REG_BASE                    0x20000
#define AVLINK_BASE			        0x8580
#define AVLINK_IO_BASE              (IO_BASE + REG_BASE + AVLINK_BASE)

#define AVL_OFFSET_TR_CONFIG        0x0000
#define AVL_OFFSET_CKGEN            0x0004
#define AVL_OFFSET_RX_T_START_R     0x0008
#define AVL_OFFSET_RX_T_START_F     0x000c
#define AVL_OFFSET_RX_T_DATA        0x0010
#define AVL_OFFSET_RX_T_ACK         0x0014
#define AVL_OFFSET_RX_T_ERROR       0x0018
#define AVL_OFFSET_TX_T_START       0x001c
#define AVL_OFFSET_TX_T_DATA_R      0x0020
#define AVL_OFFSET_TX_T_DATA_F      0x0024
#define AVL_OFFSET_TX_ARB           0x0028
#define AVL_OFFSET_RX_HEADER        0x002c
#define AVL_OFFSET_RX_DATA          0x0030
#define AVL_OFFSET_RX_HD_NEXT       0x0034
#define AVL_OFFSET_RX_DATA_NEXT     0x0038
#define AVL_OFFSET_RX_CAP           0x003c
#define AVL_OFFSET_RX_EVENT         0x0040
#define AVL_OFFSET_RX_FAIL          0x0044
#define AVL_OFFSET_RX_STATUS        0x0048
#define AVL_OFFSET_TX_HD_NEXT       0x004c
#define AVL_OFFSET_TX_DATA_NEXT     0x0050
#define AVL_OFFSET_TX_HEADER        0x0054
#define AVL_OFFSET_TX_DATA          0x0058
#define AVL_OFFSET_TX_CAP           0x005c
#define AVL_OFFSET_TX_EVENT         0x0060
#define AVL_OFFSET_TX_FAIL          0x0064
#define AVL_OFFSET_TX_STATUS        0x0068
#define AVL_OFFSET_TR_TEST          0x006c

#define AVL_TR_CONFIG               (AVLINK_BASE + AVL_OFFSET_TR_CONFIG)
#define AVL_CKGEN                   (AVLINK_BASE + AVL_OFFSET_CKGEN)
#define AVL_RX_T_START_R            (AVLINK_BASE + AVL_OFFSET_RX_T_START_R)
#define AVL_RX_T_START_F            (AVLINK_BASE + AVL_OFFSET_RX_T_START_F)
#define AVL_RX_T_DATA               (AVLINK_BASE + AVL_OFFSET_RX_T_DATA)
#define AVL_RX_T_ACK                (AVLINK_BASE + AVL_OFFSET_RX_T_ACK)
#define AVL_RX_T_ERROR              (AVLINK_BASE + AVL_OFFSET_RX_T_ERROR)
#define AVL_TX_T_START              (AVLINK_BASE + AVL_OFFSET_TX_T_START)
#define AVL_TX_T_DATA_R             (AVLINK_BASE + AVL_OFFSET_TX_T_DATA_R)
#define AVL_TX_T_DATA_F             (AVLINK_BASE + AVL_OFFSET_TX_T_DATA_F)
#define AVL_TX_ARB                  (AVLINK_BASE + AVL_OFFSET_TX_ARB)
#define AVL_RX_HEADER               (AVLINK_BASE + AVL_OFFSET_RX_HEADER)
#define AVL_RX_DATA                 (AVLINK_BASE + AVL_OFFSET_RX_DATA)
#define AVL_RX_HD_NEXT              (AVLINK_BASE + AVL_OFFSET_RX_HD_NEXT)
#define AVL_RX_DATA_NEXT            (AVLINK_BASE + AVL_OFFSET_RX_DATA_NEXT)
#define AVL_RX_CAP                  (AVLINK_BASE + AVL_OFFSET_RX_CAP)
#define AVL_RX_EVENT                (AVLINK_BASE + AVL_OFFSET_RX_EVENT)
#define AVL_RX_FAIL                 (AVLINK_BASE + AVL_OFFSET_RX_FAIL)
#define AVL_RX_STATUS               (AVLINK_BASE + AVL_OFFSET_RX_STATUS)
#define AVL_TX_HD_NEXT              (AVLINK_BASE + AVL_OFFSET_TX_HD_NEXT)
#define AVL_TX_DATA_NEXT            (AVLINK_BASE + AVL_OFFSET_TX_DATA_NEXT)
#define AVL_TX_HEADER               (AVLINK_BASE + AVL_OFFSET_TX_HEADER)
#define AVL_TX_DATA                 (AVLINK_BASE + AVL_OFFSET_TX_DATA)
#define AVL_TX_CAP                  (AVLINK_BASE + AVL_OFFSET_TX_CAP)
#define AVL_TX_EVENT                (AVLINK_BASE + AVL_OFFSET_TX_EVENT)
#define AVL_TX_FAIL                 (AVLINK_BASE + AVL_OFFSET_TX_FAIL)
#define AVL_TX_STATUS               (AVLINK_BASE + AVL_OFFSET_TX_STATUS)
#define AVL_TR_TEST                 (AVLINK_BASE + AVL_OFFSET_TR_TEST)

#define RXING_H_ACK                 Fld(1, 14, AC_MSKB1) //14
#define RXING_D_ACK                 Fld(1, 12, AC_MSKB1) //12
#define WTX_M3_ID                   Fld(3, 8, AC_MSKB1) //10:8
#define WTX_M1_DIR                  Fld(1, 7, AC_MSKB0) //7
#define WTX_M1_PAS                  Fld(1, 6, AC_MSKB0) //6
#define WTX_M1_NAS                  Fld(1, 5, AC_MSKB0) //5
#define WTX_M1_DES                  Fld(1, 4, AC_MSKB0) //4
#define WTX_MODE                    Fld(2, 0, AC_MSKB0) //1:0
#define I_EN_OV                     Fld(1, 4, AC_MSKB0) //4
#define I_EN_BR                     Fld(1, 0, AC_MSKB0) //0
#define I_EN_HEADER                 Fld(1, 6, AC_MSKB0) //6
#define I_EN_UN                     Fld(1, 4, AC_MSKB0) //4
#define I_EN_LOW                    Fld(1, 2, AC_MSKB0) //2
#define I_EN_FAIL                   Fld(1, 3, AC_MSKB0) //3
#define I_EN_BS                     Fld(1, 1, AC_MSKB0) //1
#define I_EN_RB                     Fld(1, 0, AC_MSKB0) //0
#define AVL_TX_INPUT_DELAY          Fld(5, 0, AC_MSKB0) //0:4
#define PDN                         Fld(1, 16, AC_MSKB2) //16
#define DIV_SEL                     Fld(16, 0, AC_FULLW10) //15:0
#define BCNT_NEW_INIT               Fld(4, 0, AC_MSKB0) //3:0
#define BCNT_NEW_MSG                Fld(4, 4, AC_MSKB0) //7:4
#define BCNT_RETRANSMIT             Fld(4, 8, AC_MSKB1) //11:8
#define MAX_RETRANSMIT              Fld(4, 12, AC_MSKB1) //15:12


#define AVL_ENABLE_INT_OV(onoff)        vRegWriteFldAlign(AVL_RX_EVENT, onoff, I_EN_OV)
#define AVL_ENABLE_INT_BR_RDY(onoff)    vRegWriteFldAlign(AVL_RX_EVENT, onoff, I_EN_BR)
#define AVL_ENABLE_INT_HEADER_RDY(onoff) vRegWriteFldAlign(AVL_RX_EVENT, onoff, I_EN_HEADER)
#define AVL_ENABLE_INT_UN(onoff)        vRegWriteFldAlign(AVL_TX_EVENT, onoff, I_EN_UN)
#define AVL_ENABLE_INT_LOW(onoff)       vRegWriteFldAlign(AVL_TX_EVENT, onoff, I_EN_LOW)
#define AVL_ENABLE_INT_FAIL(onoff)      vRegWriteFldAlign(AVL_TX_EVENT, onoff, I_EN_FAIL)
#define AVL_ENABLE_INT_BS(onoff)        vRegWriteFldAlign(AVL_TX_EVENT, onoff, I_EN_BS)
#define AVL_ENABLE_INT_RB(onoff)        vRegWriteFldAlign(AVL_TX_EVENT, onoff, I_EN_RB)

#define ENABLE_PINMUX_TO_AVLINK()   BSP_PinSet(PINMUX_OPCTRL_0, PINMUX_FUNCTION1)


#define AVL_FLOW_CONTROL_ACK(onoff) \
do {\
	vRegWriteFldAlign(AVL_RX_HD_NEXT, (!(onoff)), RXING_H_ACK);\
	vRegWriteFldAlign(AVL_RX_HD_NEXT, (!(onoff)), RXING_D_ACK);\
} while (0)



#endif /* _HW_AVLINK_H */

