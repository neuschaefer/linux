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

/*
 * MUSBStack-S handling of architecture-specific means to
 * define MUSB_SWAP16, MUSB_SWAP32, MGC_Read8, MGC_Read16,
 * MGC_Read32, MGC_Write8, MGC_Write16, and MGC_Write32.
 * $Revision: #1 $
 */

#ifndef __MUSB_ARCH_H__
#define __MUSB_ARCH_H__

/**
 * Architecture Macros Introduction.
 * Architecture-dependent definitions of low-level primitives
 * to perform actual controller register/FIFO accesses.
 *
 * These are only "first approximations" and may be overridden 
 * in a platform-specific header (plat_arc.h) via #undef/#define,
 * since this really depends on how the controller is tied to
 * the processor.
 * <p>
 * For example, the core access macros assume a "straight" connection 
 * to the processor's address lines.
 * If this is not true (e.g. a 32-bit RISC design omits A[1:0] so 
 * 8-bit controller registers show up 4 bytes apart), override these
 * in your plat_arc.h to left-shift the offsets.
 */

/**
 * Read an 8-bit register from the core
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @return 8-bit datum
 */
#define MGC_Read8(_pBase, _offset) *((volatile uint8_t *)(((uint32_t)_pBase) + MUSB_COREBASE + _offset))

/**
 * Read a 16-bit register from the core
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @return 16-bit datum
 */
#define MGC_Read16(_pBase, _offset) *((volatile uint16_t *)(((uint32_t)_pBase) + MUSB_COREBASE + _offset))

/**
 * Read a 32-bit register from the core
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @return 32-bit datum
 */
#define MGC_Read32(_pBase, _offset) *((volatile uint32_t *)(((uint32_t)_pBase) + MUSB_COREBASE + _offset))

/**
 * Write an 8-bit core register
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @param _data 8-bit datum
 */
#define MGC_Write8(_pBase, _offset, _data) \
    {\
    volatile uint32_t u4TmpVar; \
    u4TmpVar = *((volatile uint32_t*)(((uint32_t)_pBase) + MUSB_COREBASE + ((_offset) & 0xFC)));\
    u4TmpVar &= ~(((uint32_t)0xFF) << (8*((_offset) & 0x03)));\
    u4TmpVar |= (uint32_t)(((_data) & 0xFF) << (8*((_offset) & 0x03)));\
    *((volatile uint32_t*)(((uint32_t)_pBase) + MUSB_COREBASE + ((_offset) & 0xFC))) = u4TmpVar;\
    }

/**
 * Write a 16-bit core register
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @param _data 16-bit datum
 */
#define MGC_Write16(_pBase, _offset, _data) \
    {\
    volatile uint32_t u4TmpVar; \
    u4TmpVar = *((volatile uint32_t*)(((uint32_t)_pBase) + MUSB_COREBASE + ((_offset) & 0xFC)));\
    u4TmpVar &= ~(((uint32_t)0xFFFF) << (8*((_offset) & 0x03)));\
    u4TmpVar |= (_data) << (8*((_offset) & 0x03));\
    *((volatile uint32_t*)(((uint32_t)_pBase) + MUSB_COREBASE + ((_offset) & 0xFC))) = u4TmpVar;\
    }

/**
 * Write a 32-bit core register
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @param _data 32-bit datum
 */
#define MGC_Write32(_pBase, _offset, _data) \
	(*((volatile uint32_t *)(((uint32_t)_pBase) + MUSB_COREBASE + _offset)) = _data)

/* allow overrides of the above */
#include "plat_arc.h"

#ifdef BIG_ENDIAN
/**
 * Swap the given 16-bit quantity from USB byte order (little-endian) 
 * to host byte order.
 * @param _data the 16-bit quantity
 * @return quantity in host byte order
 */
#define MUSB_SWAP16(_data) ((((_data) & 0xFF) << 8) | (((_data) & 0xFF00) >> 8))

/**
 * Swap the given 32-bit quantity from USB byte order (little-endian) 
 * to host byte order.
 * @param _data the 32-bit quantity
 * @return quantity in host byte order
 */
#define MUSB_SWAP32(_data) ( (((_data) & 0xFF) << 24) | (((_data) & 0xFF00) << 8) | (((_data) & 0xFF000000) >> 24) | (((_data) & 0xFF0000) >> 8) )

#else

/**
 * Swap the given 16-bit quantity from USB byte order (little-endian) 
 * to host byte order.
 * @param _data the 16-bit quantity
 * @return quantity in host byte order
 */
#define MUSB_SWAP16(_data) (_data)

/**
 * Swap the given 32-bit quantity from USB byte order (little-endian) 
 * to host byte order.
 * @param _data the 32-bit quantity
 * @return quantity in host byte order
 */
#define MUSB_SWAP32(_data) (_data)

#endif	/* BIG_ENDIAN or not */

/**
 * Swap the pointed-to 16-bit quantity from USB byte order (little-endian) 
 * to host byte order, in an alignment-safe manner.
 * @param _pData pointer (as uint8_t*)
 * @return quantity in host byte order
 */
#define MUSB_SWAP16P(_pData) (((*((_pData)+1)) << 8) | (*(_pData)))

/**
 * Swap the pointed-to 24-bit quantity from USB byte order (little-endian) 
 * to host byte order, in an alignment-safe manner.
 * @param _pData pointer (as uint8_t*)
 * @return quantity in host byte order
 */
#define MUSB_SWAP24P(_pData) (((*((_pData)+2)) << 16) | ((*((_pData)+1)) << 8) | (*(_pData)))

/**
 * Swap the pointed-to 32-bit quantity from USB byte order (little-endian) 
 * to host byte order, in an alignment-safe manner.
 * @param _pData pointer (as uint8_t*)
 * @return quantity in host byte order
 */
#define MUSB_SWAP32P(_pData) (((*((_pData)+3)) << 24) | ((*((_pData)+2)) << 16) | ((*((_pData)+1)) << 8) | (*(_pData)))

//  swap for CSW residue. Some device will return dCswDataResidue in big endian value !! 
#define MUSB_BIG_ENDIAN_SWAP32(_data) ( (((_data) & 0xFF) << 24) | (((_data) & 0xFF00) << 8) | (((_data) & 0xFF000000) >> 24) | (((_data) & 0xFF0000) >> 8) )

#define MUSB_BIG_ENDIAN_SWAP32P(_pData) \
        (((*(_pData)) << 24) | ((*((_pData)+1)) << 16) | ((*((_pData)+2)) << 8) | (*((_pData)+3)))

#endif	/* multiple inclusion protection */
