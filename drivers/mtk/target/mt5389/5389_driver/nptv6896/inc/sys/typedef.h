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
 * $RCSfile: typedef.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include "x_typedef.h"

/*=====================================================================
  Common macro definition
=====================================================================*/
#ifndef CC_UP8032_ATV
#define CONST   const
#ifndef CODE
/* 8032  C language extenstions removal */
#define CODE    code
#define code
#define large
#define reentrant
#define REMOVE_UNCALLED_FUNCTION 0
#endif
#endif
#define PUBLIC  extern
#define PRIVATE static

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL  0
#endif

#define ON   1
#define OFF  0

#ifndef CC_UP8032_ATV
#define _nop_()	Error_please_remove_all_NOP_use_UtDelayXX_instead
#endif

#ifndef CC_UP8032_ATV
/*=====================================================================
  MACRO : UINT8/UINT16 access (HIGH UINT8 FIRST)
=====================================================================*/
#define bLoByte(arg)      (*(UINT8*)&arg)
#define bHiByte(arg)      (*((UINT8*)&arg + 1))

#define wLoWord(arg)      (*(UINT16*)&arg)
#define wHiWord(arg)      (*((UINT16*)&arg + 1))

#define BYTE3(arg)        (*((UINT8 *)&(arg) + 3))
#define BYTE2(arg)        (*((UINT8 *)&(arg) + 2))
#define BYTE1(arg)        (*((UINT8 *)&(arg) + 1))
#define BYTE0(arg)        (* (UINT8 *)&(arg))

#define BYTE21(arg)      (*(UINT16*)((UINT8*)&arg))

#ifndef LOADL_DWRD
#define LOADL_DWRD(arg1, arg2)  BYTE3(arg2) = *((UINT8 *)(arg1) + 3); \
                                  BYTE2(arg2) = *((UINT8 *)(arg1) + 2); \
                                  BYTE1(arg2) = *((UINT8 *)(arg1) + 1); \
                                  BYTE0(arg2) = *((UINT8 *)(arg1))
#endif
#endif
/*=====================================================================
  MACRO : Number Comparison Macro
=====================================================================*/
#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif


/*=====================================================================
  MACRO : Expand Value to Meet Special Request
=====================================================================*/
#define MAKE_EVEN(val) ((((val) + 1) >> 1) << 1)	/* to be even */
#define MAKE_ODD(val) ((val) | 0x01)	/* to be odd */

  /* Expand given value to be multiple of 8 in upper alignment */
#define MAKE_8X(val) ((((val) + 0x07) >> 3) << 3)

  /* Expand given value to be multiple of 16 in upper alignment */
#define MAKE_16X(val) ((((val) + 0x0F) >> 4) << 4)

  /* Make given value to be multiple of 16 in center alignment */
#define MAKE_16X_EX(val) ((((val) + 0x08) >> 4) << 4)

  /* Expand given value to be multiple of 4 in center alignment */
#define  MAKE_4X(val)  ((((val) + 0x01) >> 2) << 2)

#ifndef CC_UP8032_ATV
/*=====================================================================
 *  Description:
 *    Make a item position with same step
 *  Parameters:
 *    start		base start position of all items
 *    index		index of the item to be calculated
 *    step		step of all items
=====================================================================*/
#define MAKE_ITEM_POS(start, index, step) ((start) + (index) * (step))
#endif

/*=====================================================================
  MACRO : Swap Integers Macro
=====================================================================*/
#define SWAP(a, b)     {(a) ^= (b); (b) ^= (a); (a) ^= (b);}

/*=====================================================================
  MACRO : Retrieve Value from Pointer
=====================================================================*/
#define dwGetPtrVal(ptr)  (*((UINT32 *) (ptr)))	/* Get DWORD value from a pointer */
#define wGetPtrVal(ptr)   (*((UINT16 *) (ptr)))	/* Get  UINT16 value from a pointer */
#define vSetPtrValDW(ptr, dwVal)  (*((UINT32 *)ptr) = dwVal)
#define vSetPtrValW(ptr, wVal)  (*((UINT16 *)ptr) = wVal)

#ifdef CC_COPLAT_MT82
#define CRIT_STATE_T UINT8
#ifdef CC_UP8032_ATV
#define x_crit_start() irq_save()
#define x_crit_end(x) irq_restore(x)
#endif
#endif
#endif
