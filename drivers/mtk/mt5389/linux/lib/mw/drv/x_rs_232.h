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
 * $RCSfile: x_rs_232.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains RS-232 serial port driver specific
 *         definitions, which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_RS_232_H_
#define _X_RS_232_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "res_mngr/x_rm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Get operations */
#define RS_232_GET_TYPE_CAPABILITY          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define RS_232_GET_TYPE_SETUP               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define RS_232_GET_TYPE_CTRL_LINE           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define RS_232_GET_TYPE_REC_DATA            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))
#define RS_232_GET_TYPE_REC_MULTI_DATA      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 6))
#define RS_232_GET_TYPE_REC_DATA_LEN        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 7))
#define RS_232_GET_TYPE_XMT_DATA_LEN        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 8))
#define RS_232_GET_TYPE_OPERATION_MODE      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 9))

#define RS_232_GET_TYPE_REC_DATA_NFY        (RM_GET_TYPE_ISR_CONTEXT | RS_232_GET_TYPE_REC_DATA)
#define RS_232_GET_TYPE_REC_MULTI_DATA_NFY  (RM_GET_TYPE_ISR_CONTEXT | RS_232_GET_TYPE_REC_MULTI_DATA)
#define RS_232_GET_TYPE_XMT_DATA_LEN_NFY    (RM_GET_TYPE_ISR_CONTEXT | RS_232_GET_TYPE_XMT_DATA_LEN)

/* Set operations */
#define RS_232_SET_TYPE_SETUP               (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define RS_232_SET_TYPE_CTRL_LINE           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
#define RS_232_SET_TYPE_NFY_FCT             (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))
#define RS_232_SET_TYPE_CTRL_NFY_COND       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
#define RS_232_SET_TYPE_XMT_DATA            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5))
#define RS_232_SET_TYPE_XMT_MULTI_DATA      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6))
#define RS_232_SET_TYPE_OPERATION_MODE      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7))

#define RS_232_SET_TYPE_XMT_DATA_NFY        (RM_SET_TYPE_ISR_CONTEXT | RS_232_SET_TYPE_XMT_DATA)
#define RS_232_SET_TYPE_XMT_MULTI_DATA_NFY  (RM_SET_TYPE_ISR_CONTEXT | RS_232_SET_TYPE_XMT_MULTI_DATA)

/* Flags used during registration */
#define RS_232_FLAG_REC_DATA_BUFFER         ((UINT32) 0x00010000)
#define RS_232_FLAG_XMT_DATA_BUFFER         ((UINT32) 0x00020000)
#define RS_232_FLAG_CTRL_LINE               ((UINT32) 0x00040000)
#define RS_232_FLAG_CTRL_LINE_NFY           ((UINT32) 0x00080000)

/* Control line flags */
#define RS_232_CTRL_LINE_CTS                ((UINT8) 0x01)
#define RS_232_CTRL_LINE_RTS                ((UINT8) 0x02)
#define RS_232_CTRL_LINE_DCD                ((UINT8) 0x04)
#define RS_232_CTRL_LINE_DSR                ((UINT8) 0x08)
#define RS_232_CTRL_LINE_DTR                ((UINT8) 0x10)
#define RS_232_CTRL_LINE_RI                 ((UINT8) 0x20)

/* Data length */
typedef enum
{
    RS_232_DATA_LEN_4 = 0,
    RS_232_DATA_LEN_5,
    RS_232_DATA_LEN_6,
    RS_232_DATA_LEN_7,
    RS_232_DATA_LEN_8,
    RS_232_DATA_LEN_9
}   RS_232_DATA_LEN_T;

/* Parity */
typedef enum
{
    RS_232_PARITY_NONE = 0,
    RS_232_PARITY_EVEN,
    RS_232_PARITY_ODD,
    RS_232_PARITY_MARK,
    RS_232_PARITY_SPACE
}   RS_232_PARITY_T;

/* Data speed */
typedef enum
{
    RS_232_SPEED_75 = 0,
    RS_232_SPEED_110,
    RS_232_SPEED_134,
    RS_232_SPEED_150,
    RS_232_SPEED_300,
    RS_232_SPEED_600,
    RS_232_SPEED_1200,
    RS_232_SPEED_1800,
    RS_232_SPEED_2400,
    RS_232_SPEED_4800,
    RS_232_SPEED_9600,
    RS_232_SPEED_14400,
    RS_232_SPEED_19200,
    RS_232_SPEED_38400,
    RS_232_SPEED_57600,
    RS_232_SPEED_115200,
    RS_232_SPEED_128000,
    RS_232_SPEED_230400,
    RS_232_SPEED_460800,
    RS_232_SPEED_921600
}   RS_232_SPEED_T;

/* Stop bit */
typedef enum
{
    RS_232_STOP_BIT_1 = 0,
    RS_232_STOP_BIT_1_5,
    RS_232_STOP_BIT_2
}   RS_232_STOP_BIT_T;

/* Capabilities */
#define RS_232_CAP_DATA_LEN_4           (((UINT32) 1) << RS_232_DATA_LEN_4)
#define RS_232_CAP_DATA_LEN_5           (((UINT32) 1) << RS_232_DATA_LEN_5)
#define RS_232_CAP_DATA_LEN_6           (((UINT32) 1) << RS_232_DATA_LEN_6)
#define RS_232_CAP_DATA_LEN_7           (((UINT32) 1) << RS_232_DATA_LEN_7)
#define RS_232_CAP_DATA_LEN_8           (((UINT32) 1) << RS_232_DATA_LEN_8)
#define RS_232_CAP_DATA_LEN_9           (((UINT32) 1) << RS_232_DATA_LEN_9)

#define RS_232_CAP_PARITY_NONE          (((UINT32) 1) << RS_232_PARITY_NONE)
#define RS_232_CAP_PARITY_EVEN          (((UINT32) 1) << RS_232_PARITY_EVEN)
#define RS_232_CAP_PARITY_ODD           (((UINT32) 1) << RS_232_PARITY_ODD)
#define RS_232_CAP_PARITY_MARK          (((UINT32) 1) << RS_232_PARITY_MARK)
#define RS_232_CAP_PARITY_SPACE         (((UINT32) 1) << RS_232_PARITY_SPACE)

#define RS_232_CAP_SPEED_75             (((UINT32) 1) << RS_232_SPEED_75)
#define RS_232_CAP_SPEED_110            (((UINT32) 1) << RS_232_SPEED_110)
#define RS_232_CAP_SPEED_134            (((UINT32) 1) << RS_232_SPEED_134)
#define RS_232_CAP_SPEED_150            (((UINT32) 1) << RS_232_SPEED_150)
#define RS_232_CAP_SPEED_300            (((UINT32) 1) << RS_232_SPEED_300)
#define RS_232_CAP_SPEED_600            (((UINT32) 1) << RS_232_SPEED_600)
#define RS_232_CAP_SPEED_1200           (((UINT32) 1) << RS_232_SPEED_1200)
#define RS_232_CAP_SPEED_1800           (((UINT32) 1) << RS_232_SPEED_1800)
#define RS_232_CAP_SPEED_2400           (((UINT32) 1) << RS_232_SPEED_2400)
#define RS_232_CAP_SPEED_4800           (((UINT32) 1) << RS_232_SPEED_4800)
#define RS_232_CAP_SPEED_9600           (((UINT32) 1) << RS_232_SPEED_9600)
#define RS_232_CAP_SPEED_14400          (((UINT32) 1) << RS_232_SPEED_14400)
#define RS_232_CAP_SPEED_19200          (((UINT32) 1) << RS_232_SPEED_19200)
#define RS_232_CAP_SPEED_38400          (((UINT32) 1) << RS_232_SPEED_38400)
#define RS_232_CAP_SPEED_57600          (((UINT32) 1) << RS_232_SPEED_57600)
#define RS_232_CAP_SPEED_115200         (((UINT32) 1) << RS_232_SPEED_115200)
#define RS_232_CAP_SPEED_128000         (((UINT32) 1) << RS_232_SPEED_128000)
#define RS_232_CAP_SPEED_921600         (((UINT32) 1) << RS_232_SPEED_921600)

#define RS_232_CAP_STOP_BIT_1           (((UINT32) 1) << RS_232_STOP_BIT_1)
#define RS_232_CAP_STOP_BIT_1_5         (((UINT32) 1) << RS_232_STOP_BIT_1_5)
#define RS_232_CAP_STOP_BIT_2           (((UINT32) 1) << RS_232_STOP_BIT_2)

/* UART operation mode */
#define RS_232_OPERATION_MODE_NORMAL        MAKE_BIT_MASK_32(0)
#define RS_232_OPERATION_MODE_TRANSPARENT   MAKE_BIT_MASK_32(1)
#define RS_232_OPERATION_MODE_CLI           MAKE_BIT_MASK_32(2)
#define RS_232_OPERATION_MODE_FACTORY       MAKE_BIT_MASK_32(3)


/* Capability structure */
typedef struct _RS_232_CAPABILITY_INFO_T
{
    UINT32  ui4_speed;
    UINT32  ui4_data_len;
    UINT32  ui4_parity;
    UINT32  ui4_stop_bit;
}   RS_232_CAPABILITY_INFO_T;

/* Notify condition */
typedef enum
{
    RS_232_COND_REC_DATA = 0,
    RS_232_COND_REC_BUFFER,
    RS_232_COND_REC_FRAME_ERROR,
    RS_232_COND_REC_OVERFLOW,
    RS_232_COND_XMT,
    RS_232_COND_XMT_EMPTY,
    RS_232_COND_CTRL_CHG
}   RS_232_COND_T;

/* Notify function */
typedef VOID (*x_rs_232_nfy_fct) (VOID*          pv_nfy_tag,
                                  RS_232_COND_T  e_nfy_cond,
                                  UINT32         ui4_data);

/* Data xfer structure */
typedef struct _RS_232_MULTI_DATA_INFO_T
{
    SIZE_T      z_data_len;
    UINT8*      pui1_data;
    VOID*       pv_tag;    
}   RS_232_MULTI_DATA_INFO_T;

/* Notify setting structure */
typedef struct _RS_232_NFY_INFO_T
{
    VOID*               pv_tag;
    x_rs_232_nfy_fct    pf_rs_232_nfy;
}   RS_232_NFY_INFO_T;

/* Setup structure */
typedef struct _RS_232_SETUP_INFO_T
{
    RS_232_SPEED_T     e_speed;
    RS_232_DATA_LEN_T  e_data_len;
    RS_232_PARITY_T    e_parity;
    RS_232_STOP_BIT_T  e_stop_bit;
}   RS_232_SETUP_INFO_T;

#endif /* _X_RS_232_H_ */

