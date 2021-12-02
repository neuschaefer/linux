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
 * $RCSfile: x_pwr_ctrl.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: $
 *
 * Description:
 *         This header file contains Power Control driver specific
 *         definitions, which are exported.
 *---------------------------------------------------------------------------*/
 
#ifndef _X_PC_H_
#define _X_PC_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_common.h"
#include "x_rm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Get operations */
#define PC_GET_TYPE_WAKE_UP_CAP             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  0))
#define PC_GET_TYPE_WAKE_UP_CTRL_CAP        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  1))
#define PC_GET_TYPE_WAKE_UP_SETUP           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  2))
#define PC_GET_TYPE_WAKE_UP_REASON          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  3))
#define PC_GET_TYPE_WAKE_UP_TIME            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  4))
#define PC_GET_TYPE_MISC_CTRL               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  5))
#define PC_GET_TYPE_MISC_STATUS             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  6))
#define PC_GET_TYPE_GPIO_MODE               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  7))
#define PC_GET_TYPE_GPIO_STATUS             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  8))
#define PC_GET_TYPE_GPIO_MONITOR_CTRL       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  9))
#define PC_GET_TYPE_SIF                     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 10))
#define PC_GET_TYPE_SIF_ADVANCED            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 11))
#define PC_GET_TYPE_SIF_BY_GPIO             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 12))
#define PC_GET_TYPE_UPGRADE_CTRL            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 13))
#define PC_GET_TYPE_SERVO_AD                (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 14))
#define PC_GET_TYPE_POWER_DOWN_REASON       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 15))
#define PC_GET_TYPE_DEV_ID                  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 16))
#define PC_GET_TYPE_POWER_STATE             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 17))
#define PC_GET_TYPE_GPIO_QUERY               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 18))


/* Set operations */
#define PC_SET_TYPE_WAKE_UP_SETUP          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  0)) | RM_SET_TYPE_ARG_NO_REF)
#define PC_SET_TYPE_WAKE_UP_TIME            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  1))
#define PC_SET_TYPE_NFY_FCT                 (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  2))
#define PC_SET_TYPE_MISC_CTRL              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  3)) | RM_SET_TYPE_ARG_NO_REF)
#define PC_SET_TYPE_POWER_DOWN              (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  4))
#define PC_SET_TYPE_REBOOT                  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  5))
#define PC_SET_TYPE_GPIO_MODE               (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  6))
#define PC_SET_TYPE_GPIO_STATUS             (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  7))
#define PC_SET_TYPE_GPIO_MONITOR_CTRL       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  8))
#define PC_SET_TYPE_SIF                     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  9))
#define PC_SET_TYPE_SIF_ADVANCED            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 10))
#define PC_SET_TYPE_SIF_BY_GPIO             (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 11))
#define PC_SET_TYPE_UPGRADE_CTRL           ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 12)) | RM_SET_TYPE_ARG_NO_REF)
#define PC_SET_TYPE_POWER_STATE             (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 13))
#define PC_SET_TYPE_WAKE_UP_REASON          (RM_GET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 14))
#define PC_SET_TYPE_POWER_DOWN_REASON       (RM_GET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 15))
#define PC_SET_TYPE_POWER_SUSPEND           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  16))
#define PC_SET_TYPE_GPIO_REG              (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  17))

/* Definition of GPIO type */
#define PC_GPIO_TYPE_GPIO           ((UINT32) 0x00000000)
#define PC_GPIO_TYPE_OP_CTRL        ((UINT32) 0x10000000)
          
/* Wake-up reason */    
typedef enum
{
    PC_WAKE_UP_REASON_UNKNOWN = 0,
    PC_WAKE_UP_REASON_VGA,
    PC_WAKE_UP_REASON_RTC,
    PC_WAKE_UP_REASON_FP,
    PC_WAKE_UP_REASON_IRRC,
    PC_WAKE_UP_REASON_UART,
    PC_WAKE_UP_REASON_AC_POWER,
    PC_WAKE_UP_REASON_HDMI,
    PC_WAKE_UP_REASON_DVD,
    PC_WAKE_UP_REASON_UART_NORMAL,
    PC_WAKE_UP_REASON_RC_DIGIT_0,  /* Remote controller */
    PC_WAKE_UP_REASON_RC_DIGIT_1,
    PC_WAKE_UP_REASON_RC_DIGIT_2,
    PC_WAKE_UP_REASON_RC_DIGIT_3,
    PC_WAKE_UP_REASON_RC_DIGIT_4,
    PC_WAKE_UP_REASON_RC_DIGIT_5,
    PC_WAKE_UP_REASON_RC_DIGIT_6,
    PC_WAKE_UP_REASON_RC_DIGIT_7,
    PC_WAKE_UP_REASON_RC_DIGIT_8,
    PC_WAKE_UP_REASON_RC_DIGIT_9,
    PC_WAKE_UP_REASON_RC_PRG_UP,
    PC_WAKE_UP_REASON_RC_PRG_DOWN,
    PC_WAKE_UP_REASON_RC_INP_SRC,
    PC_WAKE_UP_REASON_RC_ANALOG,
    PC_WAKE_UP_REASON_RC_DIGITAL,
    PC_WAKE_UP_REASON_RC_DIGITAL_ANALOG,
    PC_WAKE_UP_REASON_FP_PRG_UP,   /* Front panel */
    PC_WAKE_UP_REASON_FP_PRG_DOWN,
    PC_WAKE_UP_REASON_FP_INP_SRC,
    PC_WAKE_UP_REASON_CUSTOM_1 = 61, 
    PC_WAKE_UP_REASON_CUSTOM_2 = 62, 
    PC_WAKE_UP_REASON_CUSTOM_3 = 63, 
    PC_WAKE_UP_REASON_CUSTOM_4 = 64     
}   PC_WAKE_UP_REASON_T;

/* Wake-up sub-reason mask */
#define PC_WAKE_UP_SUB_REASON_QUIET_BOOT                MAKE_BIT_MASK_32(0)

/* Power-down reason */
typedef enum
{
    PC_POWER_DOWN_REASON_UNKNOWN = 0,
    PC_POWER_DOWN_REASON_FP,
    PC_POWER_DOWN_REASON_IRRC,
    PC_POWER_DOWN_REASON_CUSTOM_1 = 61, 
    PC_POWER_DOWN_REASON_CUSTOM_2 = 62, 
    PC_POWER_DOWN_REASON_CUSTOM_3 = 63, 
    PC_POWER_DOWN_REASON_CUSTOM_4 = 64        
}   PC_POWER_DOWN_REASON_T;

/* Reset mode */ 
typedef enum 
{ 
    PC_RESET_MODE_USER = 0, 
    PC_RESET_MODE_FACTORY, 
    PC_RESET_MODE_CUSTOM_1, 
    PC_RESET_MODE_CUSTOM_2, 
    PC_RESET_MODE_CUSTOM_3, 
    PC_RESET_MODE_CUSTOM_4 
}   PC_RESET_MODE_T; 

/* Power state control */
typedef enum
{
    PC_PWR_STATE_CTRL_STANBY = 0, /* Only backlight is off but CPU is still running */
    PC_PWR_STATE_CTRL_STANBY_2,
    PC_PWR_STATE_CTRL_SEMI_STANBY,
    PC_PWR_STATE_CTRL_RUNNING,
    PC_PWR_STATE_CTRL_NORMAL,
    PC_PWR_STATE_CTRL_LAST_VALID_ENTRY
}   PC_PWR_STATE_CTRL_T;

/* GPIO mode */
typedef enum
{
    PC_GPIO_MODE_INPUT = 0,
    PC_GPIO_MODE_OUTPUT
}   PC_GPIO_MODE_T;

/* GPIO status */
typedef enum
{
    PC_GPIO_STATUS_UNKNOWN = 0,
    PC_GPIO_STATUS_LOW,
    PC_GPIO_STATUS_HIGH
}   PC_GPIO_STATUS_T;

/* GPIO monitoring control */
/*
    For PC_GET_TYPE_GPIO_MONITOR_CTRL and PC_SET_TYPE_GPIO_MONITOR_CTRL operations, 
    it refers to an UINT64 variable where:
    [55:0] => GPIO 0~55 monitoring control
    [63:56] => OPCTRL 0~7 monitoring control
    
    As to the notification, ui4_data is used as follows when e_nfy_cond = PC_COND_GPIO_UPDATE
    [15:0] => GPIO/OPCTRL ID
    [31:16] => GPIO/OPCTRL status (0=LOW, 1=HIGH)
*/

/* Wake-up info */
typedef struct _PC_WAKE_UP_INFO_T
{
    PC_WAKE_UP_REASON_T     e_reason;
    VOID*                   pv_data;
}   PC_WAKE_UP_INFO_T;

/* Power-down info */
typedef struct _PC_POWER_DOWN_INFO_T
{
    PC_POWER_DOWN_REASON_T  e_reason;
    VOID*                   pv_data;
}   PC_POWER_DOWN_INFO_T;

/* GPIO mode info */
typedef struct _PC_GPIO_MODE_INFO_T
{
    UINT32              ui4_id;
    PC_GPIO_MODE_T      e_mode;
}   PC_GPIO_MODE_INFO_T;
/* GPIO mode info */
typedef struct _PC_GPIO_MONITOR_CTRL_EX_T
{
    UINT32    ui4_gpio;
    BOOL      b_monitor;
}   PC_GPIO_MONITOR_CTRL_EX_T;


/* GPIO status info */
typedef struct _PC_GPIO_STATUS_INFO_T
{
    UINT32              ui4_id;
    PC_GPIO_STATUS_T    e_status;
}   PC_GPIO_STATUS_INFO_T;

/* SIF info */
typedef struct _PC_SIF_INFO_T
{
    UINT32      ui4_addr;
    UINT32      ui4_offset;
    UINT8       ui1_data;
}   PC_SIF_INFO_T;

/* SIF info (advanced version) */
typedef enum
{
    PC_SIF_CH_ID_SYS_BUS    = 0x00,
    PC_SIF_CH_ID_TUNER_BUS  = 0x01,
    PC_SIF_CH_ID_EDID_BUS   = 0x10
}   PC_SIF_CH_ID_T;

typedef struct _PC_SIF_INFO_ADVANCED_T
{
    PC_SIF_CH_ID_T  e_channel_id;
    UINT32          ui4_addr;       /* Device address */
    UINT32          ui4_offset;     /* Sub-address (or called offset) */
    UINT8           ui1_offset_len; /* Offset length: 0~3 */    
    UINT8*          pui1_data;     
    UINT8           ui1_data_len;     
}   PC_SIF_INFO_ADVANCED_T;

/* SIF info by GPIO */
typedef struct _PC_SIF_INFO_BY_GPIO_T
{
    UINT32      ui4_addr_gpio_id;
    UINT32      ui4_data_gpio_id;
    UINT32      ui4_addr;       /* Device address */
    UINT32      ui4_offset;     /* Sub-address (or called offset) */
    UINT8       ui1_offset_len; /* Offset length: 0~3 */   
    UINT8*      pui1_data;     
    UINT8       ui1_data_len;     
}   PC_SIF_INFO_BY_GPIO_T;

/* Servo AD info */
typedef struct _PC_SERVO_AD_INFO_T
{
    UINT32      ui4_id;
    UINT32      ui4_data;
}   PC_SERVO_AD_INFO_T;

/* Notify condition */
typedef enum
{
    PC_COND_UNKNOWN = 0,    
    PC_COND_WAKE_UP_EXPIRED,
    PC_COND_GPIO_UPDATED
}   PC_COND_T;

/* Notify function */
typedef VOID (*x_pc_nfy_fct)(VOID*      pv_nfy_tag,
                             PC_COND_T  e_nfy_cond,
                             UINT32     ui4_data);
                             
/* Notify setting structure */
typedef struct _PC_NFY_INFO_T
{
    VOID*           pv_tag;
    x_pc_nfy_fct    pf_nfy;
}   PC_NFY_INFO_T;

/* Wake-up capabilities (for wake-up setup & capability operations) */
#define PC_WAKE_UP_CAP_VGA                  MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_VGA - 1)
#define PC_WAKE_UP_CAP_RTC                  MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RTC - 1)
#define PC_WAKE_UP_CAP_FP                   MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_FP - 1)
#define PC_WAKE_UP_CAP_IRRC                 MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_IRRC - 1)
#define PC_WAKE_UP_CAP_UART                 MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_UART - 1)
#define PC_WAKE_UP_CAP_AC_POWER             MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_UART - 1)
#define PC_WAKE_UP_CAP_HDMI                 MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_HDMI - 1)
#define PC_WAKE_UP_CAP_UART_NORMAL          MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_UART_NORMAL - 1)
#define PC_WAKE_UP_CAP_RC_DIGIT_0           MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_DIGIT_0 - 1)
#define PC_WAKE_UP_CAP_RC_DIGIT_1           MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_DIGIT_1 - 1)
#define PC_WAKE_UP_CAP_RC_DIGIT_2           MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_DIGIT_2 - 1)
#define PC_WAKE_UP_CAP_RC_DIGIT_3           MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_DIGIT_3 - 1)
#define PC_WAKE_UP_CAP_RC_DIGIT_4           MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_DIGIT_4 - 1)
#define PC_WAKE_UP_CAP_RC_DIGIT_5           MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_DIGIT_5 - 1)
#define PC_WAKE_UP_CAP_RC_DIGIT_6           MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_DIGIT_6 - 1)
#define PC_WAKE_UP_CAP_RC_DIGIT_7           MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_DIGIT_7 - 1)
#define PC_WAKE_UP_CAP_RC_DIGIT_8           MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_DIGIT_8 - 1)
#define PC_WAKE_UP_CAP_RC_DIGIT_9           MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_DIGIT_9 - 1)
#define PC_WAKE_UP_CAP_RC_PRG_UP            MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_PRG_UP - 1)
#define PC_WAKE_UP_CAP_RC_PRG_DOWN          MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_PRG_DOWN - 1)
#define PC_WAKE_UP_CAP_RC_INP_SRC           MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_RC_INP_SRC - 1)
#define PC_WAKE_UP_CAP_FP_PRG_UP            MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_FP_PRG_UP - 1)
#define PC_WAKE_UP_CAP_FP_PRG_DOWN          MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_FP_PRG_DOWN - 1)
#define PC_WAKE_UP_CAP_FP_INP_SRC           MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_FP_INP_SRC - 1)
#define PC_WAKE_UP_CAP_CUSTOM_1             MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_CUSTOM_1 - 1) 
#define PC_WAKE_UP_CAP_CUSTOM_2             MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_CUSTOM_2 - 1) 
#define PC_WAKE_UP_CAP_CUSTOM_3             MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_CUSTOM_3 - 1) 
#define PC_WAKE_UP_CAP_CUSTOM_4             MAKE_BIT_MASK_32(PC_WAKE_UP_REASON_CUSTOM_4 - 1)     

/* Miscellaneous controls */
/*
    Operation | PC_MISC_CTRL_NO_STANDBY_2 PC_MISC_CTRL_NO_STANDBY  
    ---------------------------------------------------------------
    Standby   |            0                       0
    On        |            0                       1
    Last sts  |            1                       x
*/
#define PC_MISC_CTRL_LAST_STS                   PCL_MISC_CTRL_NO_STANDBY_2
                                                
#define PC_MISC_CTRL_NO_STANDBY                 MAKE_BIT_MASK_32(0) /* This bit is valid when PC_MISC_CTRL_NO_STANDBY_2 = 0 */
#define PC_MISC_CTRL_FACTORY_MODE               MAKE_BIT_MASK_32(1)
#define PC_MISC_CTRL_POWER_LED_OFF              MAKE_BIT_MASK_32(2)
#define PC_MISC_CTRL_NO_STANDBY_2               MAKE_BIT_MASK_32(3)
#define PC_MISC_CTRL_FIRMWARE_UPGRADE           MAKE_BIT_MASK_32(4)
#define PC_MISC_CTRL_COMPANION_CHIP_UPGRADE     MAKE_BIT_MASK_32(5)
#define PC_MISC_CTRL_QUIET_BOOT                 MAKE_BIT_MASK_32(6) /* Backlight is off and power LED is unchanged */
#define PC_MISC_CTRL_DUAL_BANK                  MAKE_BIT_MASK_32(7)
#define PC_MISC_CTRL_NO_IR_STANDBY_BY_DRIVER    MAKE_BIT_MASK_32(8)
#define PC_MISC_CTRL_DUAL_BANK_SYSTEM           MAKE_BIT_MASK_32(9) /* index 0/1 for system image and 8032 up */
#define PC_MISC_CTRL_DUAL_BANK_ROOTFS           MAKE_BIT_MASK_32(10) /* index 0/1 for root file system */
#define PC_MISC_CTRL_DUAL_BANK_KERNEL           MAKE_BIT_MASK_32(11) /* index 0/1 for kernel */
#define PC_MISC_CTRL_FAST_BOOT                  MAKE_BIT_MASK_32(12) /* index 0/1 for kernel */
#define PC_MISC_CTRL_NORMAL_BOOT_WITH_SUSPEND   MAKE_BIT_MASK_32(13) /* index 0/1 for kernel */
#define PC_MISC_CTRL_NORMAL_BOOT_WITHOUT_SUSPEND    MAKE_BIT_MASK_32(14) /* index 0/1 for kernel */

/* Miscellaneous status */
#define PC_MISC_STS_POWER_DOWN_BY_AC_CUT        MAKE_BIT_MASK_32(0)

/* Upgrade controls */
#define PC_UPGRADE_CTRL_NOR_0                   MAKE_BIT_MASK_32(0)
#define PC_UPGRADE_CTRL_NOR_1                   MAKE_BIT_MASK_32(1)
#define PC_UPGRADE_CTRL_NOR_2                   MAKE_BIT_MASK_32(2)
#define PC_UPGRADE_CTRL_NOR_3                   MAKE_BIT_MASK_32(3)
#define PC_UPGRADE_CTRL_NOR_4                   MAKE_BIT_MASK_32(4)
#define PC_UPGRADE_CTRL_NOR_5                   MAKE_BIT_MASK_32(5)
#define PC_UPGRADE_CTRL_NOR_6                   MAKE_BIT_MASK_32(6)
#define PC_UPGRADE_CTRL_NOR_7                   MAKE_BIT_MASK_32(7)
#define PC_UPGRADE_CTRL_NVM                     MAKE_BIT_MASK_32(8)
#define PC_UPGRADE_CTRL_STANDBY_MICRO_P         MAKE_BIT_MASK_32(9)
#define PC_UPGRADE_CTRL_COMPANION_CHIP          MAKE_BIT_MASK_32(10)

/* Device ID */
typedef struct _PC_DEV_ID_T
{
    UINT8   aui1_dev_id[8];    
} PC_DEV_ID_T;
    
#endif /* _X_PC_H_ */

