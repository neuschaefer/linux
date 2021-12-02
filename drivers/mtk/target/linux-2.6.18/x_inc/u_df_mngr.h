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
 * $RCSfile: u_df_mngr.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Download Function Manager specific 
 *      definitions, which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _U_DFM_H_
#define _U_DFM_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_handle.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 #define SIMP_LOGO_LEN     ((UINT8) 0x5)


/* Notify conditions. Note that this enumeration may only contain */
/* entries with a value '0' to '63' (inclusive). All other values */
/* are reserved for internal notify conditions.                   */
 typedef enum{
 
    DFM_COND_UNAVAILABLE = 0,
    DFM_COND_TRANSITION,
    DFM_COND_AVAILABLE,
    DFM_COND_UPDATE
 }  DFM_COND_T;

typedef UINT8      DFM_LOGO_TYPE_T;

#define DFM_LOGO_TYPE_SD4_3_SMAlL       ((DFM_LOGO_TYPE_T) 0x01)
#define DFM_LOGO_TYPE_SD16_9_SMALL      ((DFM_LOGO_TYPE_T) 0x02)
#define DFM_LOGO_TYPE_HD_SMALL          ((DFM_LOGO_TYPE_T) 0x04)
#define DFM_LOGO_TYPE_SD4_3_LARGE       ((DFM_LOGO_TYPE_T) 0x08)
#define DFM_LOGO_TYPE_SD16_9_LARGE      ((DFM_LOGO_TYPE_T) 0x10)
#define DFM_LOGO_TYPE_HD_LARGE          ((DFM_LOGO_TYPE_T) 0x20)
#define DFM_LOGO_TYPE_SIMP              ((DFM_LOGO_TYPE_T) 0x40)
#define DFM_LOGO_TYPE_UNKNOWN           ((DFM_LOGO_TYPE_T) 0x80)

 
/* Notify function. */
typedef VOID (*x_dfm_nfy_fct)(HANDLE_T       h_logo,
                           DFM_COND_T     e_nfy_cond,
                           VOID*          pv_nfy_tag,
                           UINT32         ui4_data);


/* Return values. */
#define DFMR_NO_DATA               ((INT32)   1)
#define DFMR_OK                    ((INT32)   0)
#define DFMR_NOT_INIT              ((INT32)  -1)
#define DFMR_ALREADY_INIT          ((INT32)  -2)
#define DFMR_INV_ARG               ((INT32)  -3)
#define DFMR_INV_HANDLE            ((INT32)  -4)
#define DFMR_OUT_OF_HANDLES        ((INT32)  -5)
#define DFMR_INV_NAME              ((INT32)  -6)
#define DFMR_UNKNOWN_NAME          ((INT32)  -7)
#define DFMR_OUT_OF_RESOURCES      ((INT32)  -8)
#define DFMR_NO_SUPPORT            ((INT32)  -9)
#define DFMR_INV_HANDLE_STATE      ((INT32) -10)
#define DFMR_NOT_ENOUGH_SPACE      ((INT32) -11)
#define DFMR_INV_LOAD_INFO         ((INT32) -12)
#define DFMR_INV_ENG_HANDLE        ((INT32) -13)
#define DFMR_FAIL                  ((INT32) -14)
#define DFMR_DUPLICATE_REC         ((INT32) -15)
#define DFMR_INV_COND              ((INT32) -16)
#define DFMR_INTERNAL_ERROR        ((INT32) -17)
#define DFMR_AEE_OUT_OF_RESOURCES  ((INT32) -18)
#define DFMR_AEE_NO_RIGHTS         ((INT32) -19)


#endif /* _U_DFM_H */


