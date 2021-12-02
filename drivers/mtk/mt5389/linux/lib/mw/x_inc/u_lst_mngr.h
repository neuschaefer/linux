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
 * $RCSfile: u_lst_mngr.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/20 $
 * $SWAuthor: Pedro Tsai $
 * $MD5HEX: aa3109d410702d35a82d86c6386592c7 $
 *
 * Description:
 *         This header file contains Large Object list related API definitions,
 *         which are exported to other Middleware components.
 *
 *---------------------------------------------------------------------------*/

#ifndef _U_LST_MNGR_H_
#define _U_LST_MNGR_H_

#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/

/* Return code status from API */
#define         LSTMR_OK                    ((INT32)  0)
#define         LSTMR_FAIL                  ((INT32) -1)
#define         LSTMR_OUT_OF_HANDLE         ((INT32) -2)
#define         LSTMR_OUT_OF_MEM            ((INT32) -3)
#define         LSTMR_INV_HANDLE            ((INT32) -4)
#define         LSTMR_INV_ARG               ((INT32) -5)
#define         LSTMR_REC_NOT_FOUND         ((INT32) -6)
#define         LSTMR_NO_WRITE_LOCK         ((INT32) -7)
#define         LSTMR_NOT_FOUND             ((INT32) -8)
#define         LSTMR_END_OF_ITERATION      ((INT32) -9)
#define         LSTMR_MODIFIED              ((INT32) -10)
#define         LSTMR_DUPLICATE_REC         ((INT32) -11)
#define         LSTMR_INV_FILE_PATH         ((INT32) -12)
#define         LSTMR_OUT_OF_SPACE          ((INT32) -13)
#define         LSTMR_AEE_OUT_OF_RESOURCES  ((INT32) -14)
#define         LSTMR_AEE_NO_RIGHTS         ((INT32) -15)
#define         LSTMR_FILE_READ_FAIL        ((INT32) -16)
#define         LSTMR_FILE_WRITE_FAIL       ((INT32) -17)
#define         LSTMR_INV_FILE_VER          ((INT32) -18)
#define         LSTMR_ALREADY_INITED        ((INT32) -19)

/* Empty list ID */
#define LST_NULL_ID                         0

/* Bitmasks for individual lists */
#define LST_MASK_SVL                    MAKE_BIT_MASK_32 (LST_TYPE_SVL)
#define LST_MASK_TSL                    MAKE_BIT_MASK_32 (LST_TYPE_TSL)
#define LST_MASK_NWL                    MAKE_BIT_MASK_32 (LST_TYPE_NWL)
#define LST_MASK_LOL                    MAKE_BIT_MASK_32 (LST_TYPE_LOL)
#define LST_MASK_SATL                   MAKE_BIT_MASK_32 (LST_TYPE_SATL)

#define LST_MASK_ALL                    ((UINT32) ~0)

/*------------------------------------------------------
  data structure and typedef.
------------------------------------------------------*/
typedef enum _LST_MNGR_LST_TYPE_ {
    LST_TYPE_SVL = 0,
    LST_TYPE_TSL,
    LST_TYPE_NWL,
    LST_TYPE_LOL,
    LST_TYPE_SATL,
    LST_MNGR_NUM_LIST_TYPES         /* Number of list types under management */
} LST_MNGR_LST_TYPE_T;

/* Dual bank */
typedef enum
{
    LST_MNGR_DUAL_BANK_AS_MODE_BY_NVM = 0,
    LST_MNGR_DUAL_BANK_AS_MODE_TO_MAIN_SUB,
    LST_MNGR_DUAL_BANK_AS_MODE_TO_MAIN,
    LST_MNGR_DUAL_BANK_AS_MODE_TO_SUB,
    LST_MNGR_DUAL_BANK_AS_MODE_LAST_VALID_ENTRY  /* only for counting purpose */
} LST_MNGR_DUAL_BANK_AS_MODE_T;

typedef INT32 (*x_lst_mngr_get_fct) (UINT8* pui1_svl_tsl_dual_bank);

typedef INT32 (*x_lst_mngr_set_fct) (UINT8 ui1_svl_tsl_dual_bank);

#ifdef LINUX_TURNKEY_SOLUTION
typedef INT32 (*lst_mngr_get_fct) (UINT8* pui1_svl_tsl_dual_bank, VOID* pv_tag);

typedef INT32 (*lst_mngr_set_fct) (UINT8 ui1_svl_tsl_dual_bank, VOID* pv_tag);
#endif

typedef struct _LST_MNGR_CLEAN_ {
    BRDCST_TYPE_T   e_brdcst_type;
} LST_MNGR_CLEAN_T;

#endif  /* _U_LST_MNGR_H_ */
