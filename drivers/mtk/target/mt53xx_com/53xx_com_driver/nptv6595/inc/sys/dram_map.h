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
 * $Author: dtvbm11 $
 * $Date  $
 * $RCSfile: dram_map.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRAM_MAP_H_
#define _DRAM_MAP_H_

#include "typedef.h"
#include "vendor.h"
#include "feature.h"
#include "debug.h"

/*****************************************************************************
  Dram Allocation Mapping
  ***************************************************************************/
#define Macro8Align(value)  ((value + 7) & 0xFFFFFFF8)
#define Macro256Align(value)  ((value + 255) & 0xFFFFFF00)
#define Macro8KAlign(value) ((value + 0x2000 - 0x01) & 0xFFFFE000)
#define Macro128KAlign(value) ((value + 131071L) & 0xFFFE0000)

/* start */
//#define UP_DRAM_BASE 0x06000000

/* SI DRAM */
//#define SI_DRAM_BASE UP_DRAM_BASE
//#define SI_DRAM_SIZE 0x1000	//4096

/* EEP DRAM Allocate and Size */
// #define EEP_DRAM_BASE (SI_DRAM_BASE + SI_DRAM_SIZE)
// #define EEP_DRAM_SIZE 0x2000 // 8192

#if 0
/* VGA Table */
#define VGA_TABLE_BASE (SI_DRAM_BASE + SI_DRAM_SIZE)
#define MAX_VGA_TABLE_SIZE 0x1000 // ToDo
#endif

#if 0
/* VBI Working Size */
#define VBI_DRAM_BASE (SI_DRAM_BASE+SI_DRAM_SIZE)
#define VBI_DRAM_SIZE 0x1200
#endif

#if 0
/* TTX DRAM Allocate and Size */
#define TTX_DRAM_BASE (VGA_TABLE_BASE + MAX_VGA_TABLE_SIZE)
#if SUPPORT_TELETEXT
	#if TTX_1KPAGE_DEC
	#define TTX_DRAM_SIZE 0x12c640//0x12C000 // (1200*1024L)
	#else
	#define TTX_DRAM_SIZE 0x1E000	// (120*1024L)
	#endif
#else
	#define TTX_DRAM_SIZE 0
#endif

#define PAGEINFO_DRAM_BASE Macro8KAlign(VBI_DRAM_BASE + VBI_DRAM_SIZE)
#if SUPPORT_TELETEXT
#define PAGEINFO_DRAM_SIZE 0x10000
#else
#define PAGEINFO_DRAM_SIZE 0
#endif
#endif

/* Video Module DRAM Allocate */
// #define _VIDEO_DRAM_BASE	(PAGEINFO_DRAM_BASE + PAGEINFO_DRAM_SIZE)	// 5371 0x06000000//(OSD_DRAM_BASE+OSD_DRAM_SIZE)
//#define _VIDEO_DRAM_BASE	(SI_DRAM_BASE+SI_DRAM_SIZE)

/*****************************************************************************
  The following are VIDEO dram allocation
****************************************************************************/
#if 0
#define DI_DRAM_BASE VIDEO_DRAM_BASE

#define DI_MBA_SIZE 13312L //13*1024
#define DI_720_576I_SIZE 414720L //720*576
#define DI_720_1080I_SIZE 777600L //720*1080
#define DI_1440_1080I_SIZE 1555200L //1440*1080

#if SUPPORT_POP
	#define DI_MBUF_SIZE DI_1440_1080I_SIZE
	#define DI_PBUF_SIZE DI_720_1080I_SIZE
#else
		#define DI_MBUF_SIZE DI_1440_1080I_SIZE       
#endif //SUPPORT_POP

//Main MDDi
#define DI_MBUF0_Y_DRAM_BASE Macro8Align(DI_DRAM_BASE)
#define DI_MBUF0_C_DRAM_BASE Macro8Align(DI_MBUF0_Y_DRAM_BASE + DI_MBUF_SIZE)
#define DI_MBUF1_Y_DRAM_BASE Macro8Align(DI_MBUF0_C_DRAM_BASE + DI_MBUF_SIZE)
#define DI_MBUF1_C_DRAM_BASE Macro8Align(DI_MBUF1_Y_DRAM_BASE + DI_MBUF_SIZE)

#define DI_6FBUF0_DRAM_BASE Macro8Align(DI_MBUF1_C_DRAM_BASE + DI_720_576I_SIZE)
#define DI_6FBUF1_DRAM_BASE Macro8Align(DI_6FBUF0_DRAM_BASE + DI_MBA_SIZE)
#define DI_6FBUF2_DRAM_BASE Macro8Align(DI_6FBUF1_DRAM_BASE + DI_MBA_SIZE)
#define DI_6FBUF3_DRAM_BASE Macro8Align(DI_6FBUF2_DRAM_BASE + DI_MBA_SIZE)

#if SUPPORT_POP
//Pip MDDi
#define P_DI_MBUF0_Y_DRAM_BASE Macro8Align(DI_MBUF0_Y_DRAM_BASE + DI_PBUF_SIZE)
#define P_DI_MBUF0_C_DRAM_BASE Macro8Align(DI_MBUF0_C_DRAM_BASE + DI_PBUF_SIZE)
#define P_DI_MBUF1_Y_DRAM_BASE Macro8Align(DI_MBUF1_Y_DRAM_BASE + DI_PBUF_SIZE)
#define P_DI_MBUF1_C_DRAM_BASE Macro8Align(DI_MBUF1_C_DRAM_BASE + DI_PBUF_SIZE)

#define P_DI_6FBUF0_DRAM_BASE Macro8Align(P_DI_MBUF1_C_DRAM_BASE + DI_720_576I_SIZE)
#define P_DI_6FBUF1_DRAM_BASE Macro8Align(P_DI_6FBUF0_DRAM_BASE + DI_MBA_SIZE)
#define P_DI_6FBUF2_DRAM_BASE Macro8Align(P_DI_6FBUF1_DRAM_BASE + DI_MBA_SIZE)
#define P_DI_6FBUF3_DRAM_BASE Macro8Align(P_DI_6FBUF2_DRAM_BASE + DI_MBA_SIZE)
#endif //SUPPORT_POP
#endif

#if 0
// SCPOS
#if (DI_MBUF_SIZE == DI_720_576I_SIZE)
	#define SCPOS_DRAM_BASE Macro8Align(DI_MBUF0_Y_DRAM_BASE + ((DI_MBUF_SIZE + DI_MBA_SIZE) * 4))
#else
	#define SCPOS_DRAM_BASE Macro8Align(DI_MBUF0_Y_DRAM_BASE + (DI_MBUF_SIZE * 4))
#endif //(DI_MBUF_SIZE == DI_720_576I_SIZE)

	#define SCPOS_MAIN_SIZE Macro8KAlign((TV_WIDTH+4) * TV_HEIGHT * 3 * 2 + 64)

#if SUPPORT_POP
#define SCPOS_PIP_SIZE Macro8KAlign(724 * TV_HEIGHT * 2 * 3 + 64)
#else
	#define SCPOS_PIP_SIZE 0
	#endif
#endif


#define TDC_USE_FBM_POOL 1

#if !TDC_USE_FBM_POOL
#define TDC_BUF_SIZE 0x251ca0	//(304020*8) 
#endif

#if 0
//TDC
#define TDC_DRAM_BASE Macro8Align(SCPOS_DRAM_BASE + SCPOS_MAIN_SIZE + SCPOS_PIP_SIZE)

#if SUPPORT_3DCOMB
#define TDC_DRAM_SIZE 0x251ca0	//(304020*8) 
#else
#define TDC_DRAM_SIZE 0        
#endif
#endif

#if 0
//NR
#define NR_DRAM_BASE Macro8Align(TDC_DRAM_SIZE + TDC_DRAM_BASE)
#if SUPPORT_TDN
#define NR_DRAM_SIZE 0x90000	// (1024*576)
#else
#define NR_DRAM_SIZE 0x0
#endif //SUPPORT_TDN
#endif

extern UINT32 NR_DRAM_BASE;
extern UINT32 NR_DRAM_SIZE;
extern UINT32 TDC_DRAM_BASE;
extern UINT32 TDC_DRAM_SIZE;
extern UINT32 VBI_DRAM_BASE;
extern UINT32 VBI_DRAM_SIZE;
extern UINT32 SW_DRAM_BASE;
extern UINT32 SW_DRAM_SIZE;

/*****************************************************************************
  Dram Allocation Variable Declaration
  ***************************************************************************/
// Audio
//extern UINT32 code DSP_MOD_BASE;
// Video
//extern UINT32 code VIDEO_DRAM_BASE;

#endif
