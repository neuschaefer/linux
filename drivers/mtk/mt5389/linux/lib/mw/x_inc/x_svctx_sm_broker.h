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
 * $RCSfile: x_svctx_sm_broker.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/6 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 29fe902dc7b0175781aa7c8566cead6f $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#ifndef _X_SVCTX_SM_BROKER_H_
#define _X_SVCTX_SM_BROKER_H_

/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_device_type.h"
#include "u_handle.h"
#include "u_svctx.h"
#include "u_sm_video_hdlr.h"
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
typedef UINT32 SVCTX_SM_BROKER_CMD_CODE_T;

#define SVCTX_SM_BROKER_CMD_CODE_END                   ((SVCTX_SM_BROKER_CMD_CODE_T) 0)
#define SVCTX_SM_BROKER_CMD_CODE_DEVICE_TYPE           ((SVCTX_SM_BROKER_CMD_CODE_T) 1)
#define SVCTX_SM_BROKER_CMD_CODE_SRC_NAME              ((SVCTX_SM_BROKER_CMD_CODE_T) 2)
#define SVCTX_SM_BROKER_CMD_CODE_SVC_LST_ID            ((SVCTX_SM_BROKER_CMD_CODE_T) 3)
#define SVCTX_SM_BROKER_CMD_CODE_SVL_REC_ID            ((SVCTX_SM_BROKER_CMD_CODE_T) 4)

typedef struct
{
    SVCTX_SM_BROKER_CMD_CODE_T  e_code;

    union
    {
        DEVICE_TYPE_T   e_device_type; /* DEV_TUNER or DEV_AVC defined u_device_type.h*/
        UINT16          ui2_data;
        CHAR*           ps_text;
        VOID*           pv_data;       /* SM_VSH_VBI_NFY_T* for SVCTX_SM_BROKER_CMD_CODE_VSH_VBI_NTY */
    } u;
} SVCTX_SM_BROKER_CMD_T;

typedef enum
{
    SVCTX_SM_BROKER_COND_UNAVAILABLE = 0,
    SVCTX_SM_BROKER_COND_TRANSITION,
    SVCTX_SM_BROKER_COND_AVAILABLE
}   SVCTX_SM_BROKER_COND_T;

typedef VOID (*x_svctx_vbi_broker_nfy_fct) (
                    HANDLE_T                        h_vbi_broker,
                    SVCTX_SM_BROKER_COND_T          e_cond,
                    VOID*                           pv_tag,
                    UINT32                          ui4_data
                    );
/*-----------------------------------------------------------------------------
 * functions declarations
 *---------------------------------------------------------------------------*/
extern INT32 x_svctx_vbi_broker_open (
                    const SVCTX_SM_BROKER_CMD_T*    pt_cmd_sets,    /* in  */
                    SM_VSH_VBI_NFY_T*               pt_vbi,         /* in  */
                    x_svctx_vbi_broker_nfy_fct      pf_nfy,         /* in  */
                    VOID*                           pv_tag,         /* in  */
                    HANDLE_T*                       ph_vbi_broker,  /* out */
                    SVCTX_SM_BROKER_COND_T*         pe_cond		    /* out */
                    );
extern INT32 x_svctx_vbi_broker_close (
                    HANDLE_T                        h_vbi_broker    /* in  */
                    );
/* The difference between this API with Stream Manager's x_sm_get()
 * is the object handle. In this API, it is VBI broker handle. In Stream
 * Manager, it is video stream handle. The Details about how to use this 
 * API, please reference Stream Manager's spec for e_get_type, pv_get_info
 * and pz_get_info_size.
 */
extern INT32 x_svctx_vbi_broker_video_sm_get(
                    HANDLE_T                        h_vbi_broker,   /* in  */
                    SM_GET_TYPE_T                   e_get_type,     /* in  */
                    VOID*                           pv_get_info,    /* in/out */
                    SIZE_T*                         pz_get_info_size/* in/out */
                    );
#endif  /* _X_SVCTX_SM_BROKER_H_ */
