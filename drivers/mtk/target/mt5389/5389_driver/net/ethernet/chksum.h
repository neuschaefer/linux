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
/*****************************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
* AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN
* "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER
* DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF
* ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE
* MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR
* ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION OR TO CONFORM TO
* A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
* LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
* AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR
* REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK
* FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH
* THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS
* PRINCIPLES.
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: chksum.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   checksum register header file
 *
 * Author:
 * -------
 *   HsHuang
 *
 * Last changed:
 * -------------
 * $Author:
 *
 * $Modtime:
 *
 * $Revision:
****************************************************************************/

#ifndef _CHKSUM_H_
#define _CHKSUM_H_

#include "x_hal_5381.h"
#include "ether_com.h"

/****************************************************************************
** Config definitions
****************************************************************************/
#define HW_CHKSUM_ON_THE_FLY  0 /* apply hw to calculate checksum */
//need to set CONFIG_ETHER_HW_CHKSUM  1 in Ether_priv.h

// *********************************************************************
// checksum Registers offset
// *********************************************************************
#define CHECKSUM_REG_OFFSET     (IO_VIRT + 0x29700)

// *********************************************************************
// 1's complement checksum registers map
// *********************************************************************
#if 0
#define RW_CHKSUM_ADDR   0x00     //	UDP/TCP/IP Checksum Start Address										
  #define CHECKSUM_SADR_MASK 	 ((UINT32)0xffffFFFF)	  //Checksum Start Address (Byte Address)

#define RW_CHKSUM_LENGTH  0x04
  #define CHECKSUM_LENGTH_MASK 	 ((UINT32)0xffffFFFF)	  // Checksum Length (Byte)										

#define RW_CHKSUM_CTRL     0x08
  #define  OUT_INV		((UINT32)0x01 << 7)  // 1's complement of UDP/TCP/IP Checksum Result Data
  #define  OUT_REV		((UINT32)0x01 << 6)  //Reverse Byte of Checksum Result Data
  #define  BYTE_REV         ((UINT32)0x01 << 5)  //Reverse Byte of Checksum Input Data (Word)
  #define  DW_REV	      ((UINT32)0x01 << 4)  //Reverse Byte of Checksum Input Data (Double Word)	
  #define  INTR_EN		((UINT32)0x01 << 1) //Checksum Interrupt Enable
  #define  CHECKSUM_EN	((UINT32)0x01 << 0)  //Checksum Enable. When Checksum is finish, this bit will be clear to 0.	

#define RW_CHKSUM_RESULT     0x0C
  #define  CHECKSUM_RESULT_MASK	((UINT32)0x0FFFF)  //Checksum Result Data Mask

#define RW_CHKSUM_DMA_CTRL     0x10
  #define  CHECKSUM_DMA_IDLE_TIME_MASK	((UINT32)0xFFFF0000)  //Checksum DMA Idle Time (dram clock cycle)
  #define  CHECKSUM_DMA_ALE_NUM_MASK	((UINT32)0x0000FFFF)  //Checksum Burst Length (64 bytes). If these registers set to 0, burst length is CHECKSUM_LENGTH.
#endif
// *********************************************************************
// Macros
// *********************************************************************
#define vWriteChkSum(dAddr, dVal)  IO_WRITE32((CHECKSUM_REG_OFFSET), (dAddr), (dVal))
#define dReadChkSum(dAddr)         IO_READ32((CHECKSUM_REG_OFFSET), (dAddr))
#define SetBitChkSum(Reg, Bit)     vWriteChkSum(Reg, dReadChkSum(Reg) |   (Bit) )
#define ClrBitChkSum(Reg, Bit)     vWriteChkSum(Reg, dReadChkSum(Reg) & (~(Bit)))


/****************************************************************************
** Function prototype
****************************************************************************/
void  vChksumDeQueue(void);
void vCheckSumInit(void);
INT32 _i4EtherTxChkSumVerify(UINT32 u4Size, UINT32 u4Count, BOOL fgSingleSizeTest);
INT32 _i4EtherRxChkSumVerify(UINT32 u4Size, UINT32 u4Count, BOOL fgSingleSizeTest);
#endif // _CHKSUM_H_
