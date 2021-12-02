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
 *
 * Description:
 *         The parser driver
 *
 *---------------------------------------------------------------------------*/

#ifndef PSR_HW_H
#define PSR_HW_H

#include "psr_reg.h"
#include "x_hal_5381.h"

#define HEADER_MASK             0xfffffff8
#define BUFF_OFFSET_MASK        0x0000ffff

//===========================================================================

#define _PSR_GET_ADDR(reg, ch, inc) \
        NONCACHE(PARSER_READ32((reg) + ((ch) * (inc))))

#define _PSR_SET_ADDR(reg, ch, inc, addr) \
        PARSER_WRITE32((reg) + ((ch) * (inc)), PHYSICAL(addr))

#define PSR_GET_ADDR(reg, ch)           _PSR_GET_ADDR(reg, ch, 4)

#define PSR_SET_ADDR(reg, ch, addr)     _PSR_SET_ADDR(reg, ch, 4, addr)

#define PSR_GET_ADDR_8(reg, ch)         _PSR_GET_ADDR(reg, ch, 8)

#define PSR_SET_ADDR_8(reg, ch, addr)   _PSR_SET_ADDR(reg, ch, 8, addr)

//---------------------------------------------------------------------

#define _PSR_GET_DATA(reg, ch, inc) \
        PARSER_READ32((reg) + ((ch) * (inc)))

#define _PSR_SET_DATA(reg, ch, inc, data) \
        PARSER_WRITE32((reg) + ((ch) * (inc)), (data))

#define PSR_GET_DATA(reg, ch)           _PSR_GET_DATA(reg, ch, 4)

#define PSR_SET_DATA(reg, ch, data)     _PSR_SET_DATA(reg, ch, 4, data)

//===========================================================================

#define PSR_GET_HEADER_ADDR(ch) \
        NONCACHE(PARSER_READ32(REG_HEAD0STA + ((ch) * 4)) & HEADER_MASK)

#define PSR_SET_HEADER_ADDR(ch, addr) \
        PARSER_WRITE32(REG_HEAD0STA + ((ch) * 4), \
            (PHYSICAL(addr) & HEADER_MASK) | \
            (PARSER_READ32(REG_HEAD0STA + ((ch) * 4)) & ~HEADER_MASK))

#define PSR_GET_PAYLOAD_ADDR_CHANNEL(dev) \
        (PARSER_READ32(REG_PSTASEL0 + ((dev) * 4)) & ~HEADER_MASK)

#define PSR_SET_PAYLOAD_ADDR_CHANNEL(dev, ch) \
        PARSER_WRITE32(REG_PSTASEL0 + ((dev) * 4), \
            (ch & ~HEADER_MASK) | \
            (PARSER_READ32(REG_PSTASEL0 + ((dev) * 4)) & HEADER_MASK))

#define PSR_GET_FIFO_START(ch)          PSR_GET_ADDR_8(REG_FIFO0STA, ch)
#define PSR_SET_FIFO_START(ch, addr)    PSR_SET_ADDR_8(REG_FIFO0STA, ch, addr)

#define PSR_GET_FIFO_END(ch)            PSR_GET_ADDR_8(REG_FIFO0END, ch)
#define PSR_SET_FIFO_END(ch, addr)      PSR_SET_ADDR_8(REG_FIFO0END, ch, addr)

#define PSR_GET_WP(ch)                  PSR_GET_ADDR(REG_FIFO0WP, ch)
#define PSR_SET_WP(ch, addr)            PSR_SET_ADDR(REG_FIFO0WP, ch, addr)

#define PSR_GET_RP(ch)                  PSR_GET_ADDR(REG_FIFO0RP, ch)
#define PSR_SET_RP(ch, addr)            PSR_SET_ADDR(REG_FIFO0RP, ch, addr)

#define PSR_GET_REAL_RP(ch)             PSR_GET_ADDR(REG_FIFO0REALRP, ch)

#define PSR_GET_VWPRISC(dev)            PSR_GET_ADDR(REG_VWPRISC0, dev)
#define PSR_SET_VMPRISC(dev, addr)      PSR_SET_ADDR(REG_VWPRISC0, dev, addr)

//---------------------------------------------------------------------

#define PSR_GET_THR(ch)                 PSR_GET_DATA(REG_FIFO0THR, ch)
#define PSR_SET_THR(ch, thr)            PSR_SET_DATA(REG_FIFO0THR, ch, thr)

#define PSR_GET_FIFOSEL(ch)             PSR_GET_DATA(REG_FIFO0SEL, ch)
#define PSR_SET_FIFOSEL(ch, sel)        PSR_SET_DATA(REG_FIFO0SEL, ch, sel)

#define PSR_GET_VWPSEL(dev)             PSR_GET_DATA(REG_VWP0SEL, dev)
#define PSR_SET_VWPSEL(dev, sel)        PSR_SET_DATA(REG_VWP0SEL, dev, sel)

#define PSR_GET_AWPSEL(dev)             PSR_GET_DATA(REG_AWP0SEL, dev)
#define PSR_SET_AWPSEL(dev, sel)        PSR_SET_DATA(REG_AWP0SEL, dev, sel)

#define PSR_SET_ARPSEL(dev)   PARSER_WRITE32(REG_ARPOFF, (PARSER_READ32(REG_ARPOFF) & BUFF_OFFSET_MASK) | (dev == 0) ? RP_SEL_AUD0 : RP_SEL_AUD1)                                                 
#define PSR_SET_VRPSEL(dev)   PARSER_WRITE32(REG_VRPOFF, (PARSER_READ32(REG_VRPOFF) & BUFF_OFFSET_MASK) | \
                                                  (dev == 0) ? RP_SEL_VLD0 : RP_SEL_VLD1)

//===========================================================================

#endif  // PSR_HW_H

