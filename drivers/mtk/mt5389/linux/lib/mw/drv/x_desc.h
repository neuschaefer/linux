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
 * $RCSfile: x_desc.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains descrambler driver specific definitions,
 *         which are exported.
 *----------------------------------------------------------------------------*/

#ifndef _X_DESC_H_
#define _X_DESC_H_

/*-----------------------------------------------------------------------------
                    include files
 -----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "res_mngr/x_rm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Get operations */
#define DESC_GET_TYPE_CTRL          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define DESC_GET_TYPE_ALGORITHM	    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define DESC_GET_TYPE_PID           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))

/* Set operations */
#define DESC_SET_TYPE_CTRL         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0)) | RM_SET_TYPE_ARG_NO_REF)
#define DESC_SET_TYPE_ALGORITHM     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define DESC_SET_TYPE_PID          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2)) | RM_SET_TYPE_ARG_NO_REF)
#define DESC_SET_TYPE_ODD_KEY       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))
#define DESC_SET_TYPE_EVEN_KEY      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))

/* Descrambler algorithms */
typedef enum
{
    DESC_ALG_NONE = 0,          /* None scramble */
    DESC_ALG_DES_ECB,           /* DES-56-ECB*/
    DESC_ALG_DES_CBC,           /* DES-56-CCB*/
    DESC_ALG_3DES_ECB,          /* 3DES-ECB*/
    DESC_ALG_3DES_CBC,          /* 3DES-CCB*/
    DESC_ALG_DVB,               /* DVB */
    DESC_ALG_AES_128_ECB,       /* AES-128-ECB*/
    DESC_ALG_AES_128_CBC        /* AES-128-CCB*/
}   DESC_ALG_T;

/*
   Descrambler algorithms in bit-mask manner used in
   the argument ui4_comp_flags of the API x_rm_reg_comp()
*/
#define DESC_DRV_FLAG_CAP_DES_ECB   MAKE_BIT_MASK_32(DESC_ALG_DES_ECB  + ((DESC_ALG_T) 16))
#define DESC_DRV_FLAG_CAP_DES_CBC   MAKE_BIT_MASK_32(DESC_ALG_DES_CBC  + ((DESC_ALG_T) 16))
#define DESC_DRV_FLAG_CAP_3DES_ECB  MAKE_BIT_MASK_32(DESC_ALG_3DES_ECB + ((DESC_ALG_T) 16))
#define DESC_DRV_FLAG_CAP_3DES_CBC  MAKE_BIT_MASK_32(DESC_ALG_3DES_CBC + ((DESC_ALG_T) 16))
#define DESC_DRV_FLAG_CAP_DVB       MAKE_BIT_MASK_32(DESC_ALG_DVB      + ((DESC_ALG_T) 16))

/* DES EBC Key information */
typedef struct _DES_ECB_KEY_INFO_T
{
    SIZE_T          z_key_len;    /* size of key in byte */
    const UINT8*    pui1_key;     /* pointer of key buffer */
} DES_ECB_KEY_INFO_T;

/* AES CBC Key information */
typedef struct _AES_128_CBC_KEY_INFO_T
{
    SIZE_T          z_key_len;    /* size of key in byte */
    const UINT8*    pui1_key;     /* pointer of key buffer */
    SIZE_T          z_iv_len;     /* size of iv in byte */
    const UINT8*    pui1_iv;      /* pointer of iv buffer */
} AES_128_CBC_KEY_INFO_T;

/* Definition of descrambler key information */
typedef struct _DESC_KEY_INFO_T
{
    DESC_ALG_T e_descrambler_alg;                   /* descrambler algorithm */
    union
    {
        DES_ECB_KEY_INFO_T t_des_ecb_key;          /*DES-56-ECB*/
        AES_128_CBC_KEY_INFO_T t_aes_128_cbc_key;  /*AES-128-CBC*/
    }u_key_info;
}   DESC_KEY_INFO_T;

#endif /* _X_DESC_H_ */

