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
 * $RCSfile: x_cec.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: $
 *
 * Description:
 *         This header file contains HDMI CEC driver specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_CEC_H_
#define _X_CEC_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "res_mngr/x_rm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Set operations */
#define CEC_SET_TYPE_SEND_MSG               (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0))
#define CEC_SET_TYPE_NFY_FCT                (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define CEC_SET_TYPE_WAKE_UP_CTRL           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
#define CEC_SET_TYPE_SIGNAL_CEC_READY      ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3)) | RM_SET_TYPE_ARG_NO_REF)
#define CEC_SET_TYPE_LOG_ADDR               (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
#define CEC_SET_TYPE_GLOBAL_CEC_CTRL       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5)) | RM_SET_TYPE_ARG_NO_REF)
#define CEC_SET_TYPE_ARC_CTRL                (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6))

#define CEC_GET_TYPE_MSG_ENQUEUE            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define CEC_GET_TYPE_IS_ACT_SRC_RECEIVED    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))

/* 
    CEC_SET_TYPE_SIGNAL_CEC_READY is used to switch CEC control between CEC driver and uP
    - TRUE: CEC driver
    - FALSE: uP
*/

#define CEC_DEV_VNDR_ID_SIZE                ((SIZE_T)   3) 
#define CEC_MAX_OPERAND_SIZE                ((SIZE_T)  14) 

/* CEC notify type */
typedef enum
{
    CEC_NFY_TYPE_ACK = 0,
    CEC_NFY_TYPE_FRAME,
    CEC_NFY_TYPE_5V_POWER
}   CEC_NFY_TYPE_T;

/* ACK condition */
typedef enum
{
    CEC_ACK_COND_OK = 0,
    CEC_ACK_COND_NO_RESPONSE   
}   CEC_ACK_COND_T;

/* ACK info */
typedef struct _CEC_ACK_INFO_T
{
    VOID*               pv_tag;
    CEC_ACK_COND_T      e_ack_cond;
}   CEC_ACK_INFO_T;

/* Frame info */
typedef struct _CEC_FRAME_INFO_T
{
    UINT8       ui1_init_addr;
    UINT8       ui1_dest_addr;
    UINT16      ui2_opcode;
    UINT8       aui1_operand[CEC_MAX_OPERAND_SIZE];
    SIZE_T      z_operand_size;
}   CEC_FRAME_INFO_T;

typedef struct _CEC_5V_POWER_INFO_T
{
    BOOL      b_5v_power;
    UINT16    ui2_comp_id;
} CEC_5V_POWER_INFO_T;

/* CEC notification */
typedef struct _CEC_NFY_T
{
    CEC_NFY_TYPE_T      e_nfy_type;

    union
    {
        CEC_ACK_INFO_T      t_ack_info;
        CEC_FRAME_INFO_T    t_frame_info;
        CEC_5V_POWER_INFO_T      t_5v_power_info;
    }   u;
}   CEC_NFY_T;

/* Notify function */
typedef VOID (*x_cec_nfy_fct)(CEC_NFY_T* pt_nfy_data,
                              VOID*      pv_nfy_tag,
                              UINT32	 ui4_data);

/* CEC_SET_TYPE_SEND_MSG *****************************************************/
typedef struct _CEC_SEND_MSG_T
{
    VOID*                   pv_tag;
    CEC_FRAME_INFO_T        t_frame_info;
    BOOL                    b_enqueue_ok;
}   CEC_SEND_MSG_T;

/* CEC_SET_TYPE_NFY_FCT ******************************************************/
typedef struct _CEC_NFY_INFO_T
{
    VOID*               pv_nfy_tag;
    x_cec_nfy_fct       pf_cec_nfy;
}   CEC_NFY_INFO_T;

/* CEC_SET_TYPE_WAKE_UP_CTRL *************************************************/
typedef UINT8 CEC_DEV_VNDR_ID_T [CEC_DEV_VNDR_ID_SIZE];           

typedef enum
{
    CEC_FCT_CTRL_OFF = 0,
    CEC_FCT_CTRL_ON
}   CEC_FCT_CTRL_T;

/* CEC version */
typedef enum
{
    CEC_VER_1_1    = 0x00,
    CEC_VER_1_2    = 0x01,
    CEC_VER_1_2A   = 0x02,
    CEC_VER_1_3    = 0x03,
    CEC_VER_1_3A   = 0x04
}   CEC_VER_T;

/* uP wake-up control */
/* e_cec_fct_ctrl is used to determine whether CEC function is supported or not when uP is active */
typedef struct _CEC_WAKE_UP_CTRL_T
{
    CEC_FCT_CTRL_T          e_cec_fct_ctrl;
    CEC_VER_T               e_cec_ver;
    ISO_639_LANG_T          t_menu_lang;
    CEC_DEV_VNDR_ID_T       t_dev_vndr_id; 
    BOOL                    b_tv_auto_pwr_on;   
}   CEC_WAKE_UP_CTRL_T;

/* CEC logical address */
typedef enum
{
    CEC_LOG_ADDR_TV = 0,
    CEC_LOG_ADDR_REC_DEV_1,
    CEC_LOG_ADDR_REC_DEV_2,
    CEC_LOG_ADDR_TUNER_1,
    CEC_LOG_ADDR_PLAYBACK_DEV_1,
    CEC_LOG_ADDR_AUD_SYS,
    CEC_LOG_ADDR_TUNER_2,
    CEC_LOG_ADDR_TUNER_3,
    CEC_LOG_ADDR_PLAYBACK_DEV_2,
    CEC_LOG_ADDR_REC_DEV_3,
    CEC_LOG_ADDR_TUNER_4,
    CEC_LOG_ADDR_PLAYBACK_DEV_3,
    CEC_LOG_ADDR_RESERVED_1,
    CEC_LOG_ADDR_RESERVED_2,
    CEC_LOG_ADDR_FREE_USE,
    CEC_LOG_ADDR_UNREGED_BRDCST,
    CEC_LOG_ADDR_MAX
}   CEC_LOG_ADDR_T;

/* CEC_GET_TYPE_MSG_ENQUEUE **************************************************/
typedef struct _CEC_GET_MSG_ENQUEUE_T
{
    BOOL    b_enqueue_ok;
    
}   CEC_GET_MSG_ENQUEUE_T;

/* CEC_GET_TYPE_IS_ACT_SRC_RECEIVED ******************************************/
typedef struct _CEC_ACT_SRC_REVED_T
{
    BOOL        b_act_src_reved;
    UINT16      ui2_pa;
}   CEC_ACT_SRC_REVED_T;

typedef struct _CEC_ARC_CTRL_T
{
    BOOL        b_enable;
}   CEC_ARC_CTRL_T;


#endif /* _X_CEC_H_ */

