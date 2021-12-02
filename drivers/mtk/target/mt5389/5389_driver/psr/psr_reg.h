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

#ifndef PSR_REG_H
#define PSR_REG_H

#include "x_hal_5381.h"

//===========================================================================
// Macros for register read/write

#define PARSER_BASE						PARSER0_BASE
#define PSR_BASE                        PARSER0_BASE  

#define PARSER_READ32(offset)			IO_READ32(PARSER_BASE, (offset))
#define PARSER_WRITE32(offset, value)	IO_WRITE32(PARSER_BASE, (offset), (value))
#define PARSER_REG32(offset)			IO_REG32(PARSER_BASE, (offset))

//===========================================================================
// System parser register and bitmap defintions

// Hardware constants

#define GBUF_MAX_DMA_SIZE		0xf7f
#define PBUF_MAX_DMA_SIZE		0xf7ffff

// Shared registers

#define REG_PARCTL				0x1f0		// Parser control

	#define PARCTL_RST			(1 << 0)	// Reset parser hardware
	#define PARCTL_PCKE			(1 << 1)	// Parser clock enable
	#define PARCTL_APD			(1 << 2)	// Automatic power down control
	
// PBUF registers

#define REG_PLENG				0x4			// Transfer length
#define REG_PWP					0x170		// Write destination adderss

#define REG_PADDR				0x8 		// Transfer initial read address

#define REG_BITTYPE				0x3c		// Bitstream type
	#define BITTYPE_AUDIO		(0x1)
	#define BITTYPE_GENERAL		(0x7)

#define REG_PBUF				0x200		// PBUF data (- 0x23f, 64bytes)

#define REG_PCMD				0x28		// P command
	#define PCMD_GO				(1 << 0)    // Data transfer trigger

	#define PCMD_ABT			(1 << 6)    // Abort current PBUF transfer
	#define PCMD_LOCK_P			(1 << 10)   // Lock PBUF to avoid ADSP DMA
	#define PCMD_RDY			(1 << 16)   // RUBF ready to accept new transfer command
	#define PCMD_OK				(1 << 17)   // PBUF complete

// Output buffer ranges

#define REG_A0STA				0x14		// Audio buffer 0 start address
#define REG_A0END				0x18		// Audio buffer 0 end address

// Output buffer read/write pointers

#define REG_ARP					0x1d4		// Audio buffer read pointer
#define REG_AWP					0x5c		// Audio buffer write pointer
#define REG_ARPOFF				0x1d8		// Audio read pointer offset and source selection

    #define RP_SEL_AUD0         (0 << 17)   // Read pointer is from Audio 0
    #define RP_SEL_AUD1         (1 << 17)   // Read pointer is from Audio 1

#define REG_INTEN               0x184       // Interrupt enable
#define REG_INTCLR              0x188       // Interrupt clear
#define REG_INTSTS              0x18c       // Interrupt status

    #define INT_PDMA            (1 << 1)    // P DMA interrupt

//=====================================================================
// Type definitions

typedef enum BITTYPE
{
	// The definition here should be identical to hardware spec.
	BTYPE_VIDEO = 0,
	BTYPE_AUDIO = 1,
	BTYPE_SP0 = 2,
	BTYPE_SP1 = 3,
	BTYPE_NAVI = 4,
	BTYPE_GENERAL = 5
} BITTYPE_T;


//=====================================================================
// Constant defintions

#define DMA_SIZE					4096

#define DTYPE_SHIFT					8
#define PTYPE_CHANNEL_SHIFT			16
#define PTYPE_STREAM_SHIFT			19
#define END_SHIFT					21

#define PES_HEADER_FIRST_BYTE		0
#define PES_HEADER					1
#define PES_PAYLOAD_FIRST_BYTE		2
#define PES_PAYLOAD					3
#define PES_INVALID_DTYPE           4

#define TS_LIMIT_SIZE				180
#define TS_STRICT_LIMIT_SIZE        (TS_LIMIT_SIZE + 4)

#define FIFO_THRESHOLD              0x1ff


#endif	// PSR_REG_H

