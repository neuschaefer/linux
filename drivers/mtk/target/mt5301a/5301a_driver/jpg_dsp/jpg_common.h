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
#if ! defined(JPEG_COMMON_H)
#define JPEG_COMMON_H

#include "x_lint.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"

typedef enum _ENUM_JPEG_RET_CODE_T
{
	E_JPG_OK,
	E_JPG_ERR_NONFATAL,
	E_JPG_ERR_FATAL,
	E_JPG_ERR_PARAM,
	E_JPG_ERR_NOT_INITED,
	E_JPG_ERR_ALREADY, //5
	/* markers */
	E_JPG_ERR_NOT_JPEG_IMAGE,
	E_JPG_ERR_NOT_A_MARKER,
	E_JPG_ERR_PASS_END,
	E_JPG_ERR_MULTI_SOI,
	E_JPG_ERR_UNSUPPORT_SOF, //10
	E_JPG_ERR_MULTI_SOF,
	E_JPG_ERR_INVALID_SOF,
	E_JPG_ERR_SOS_BEFORE_SOF,
	E_JPG_ERR_INVALID_SOS,
	E_JPG_ERR_INVALID_DHT, //15
	E_JPG_ERR_INVALID_DRI,
	E_JPG_ERR_MULTI_APP1,
	E_JPG_ERR_INVALID_APP0,
	E_JPG_ERR_INVALID_APP1,
	E_JPG_ERR_PARSE_FAILED, //20
	E_JPG_ERR_NOT_PARSED,
	E_JPG_ERR_MULTI_EOI,
	/* instances */
	E_JPG_ERR_ALLOC_PARSER,
	E_JPG_ERR_ALLOC_DECODER,
	E_JPG_ERR_ALLOC_MEM, //25
	E_JPG_ERR_ALLOC_FLOW,
	/* general */
	E_JPG_ERR_UNSUPPORT,
	E_JPG_ERR_GENERAL,
	E_JPG_ERR_LAST,
	E_JPG_ERR_DISPLAY_ADDR //30
} ENUIM_JPEG_RET_CODE_T;

/* MISRA32 enum assign at first only or all assign */
LINT_SUPPRESS_NEXT_STATEMENT(960)
typedef enum _ENUM_HWJPG_RET_CODE
{
	E_HWJPG_OK = 0,
	E_HWJPG_BUSY,

	E_HWJPG_ERR_FETCH_TIMEOIT = E_JPG_ERR_LAST + 1,
	E_HWJPG_ERR_SET_BS,
	E_HWJPG_ERR_LOAD_Q,
	E_HWJPG_ERR_QTBL_INDEX,
	E_HWJPG_ERR_QTBL_NUM,

	E_HWJPG_ERR_PARAM,
	E_HWJPG_ERR_TIMEOUT,

	E_HWJPG_ERR_COMP_RANGE,
	E_HWJPG_ERR_TBL_RANGE,
	E_HWJPG_ERR_BLOCK,
	E_HWJPG_ERR_SIZE,
	E_HWJPG_ERR_OUTBUF,

	E_HWJPG_ERR_NULL_SCAN,

	E_HWJPG_ERR_GLOB_NOT_READY,
	E_HWJPG_ERR_SCAN_NOT_READY,
	E_HWJPG_ERR_ROW_NOT_READY,
	E_HWJPG_ERR_MCU_NOT_READY,
	E_HWJPG_ERR_MISC_NOT_READY,
	E_HWJPG_ERR_HUFF_ADDR,
	/* last */
	E_HWJPG_ERR_GENERAL
} ENUM_HWJPG_RET_CODE;


#define D_BIT(X)			(1 << (X))
#define ALIGN_MASK(BIT)		(((UINT32)(BIT) >> 3) - 1)


#endif //JPEG_COMMON_H
