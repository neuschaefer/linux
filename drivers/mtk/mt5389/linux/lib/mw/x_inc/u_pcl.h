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
/*----------------------------------------------------------------------------*/
/*! @file u_pcl.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains the exported constants and type
 *         definitions of the Power Control Library.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_PCL_H_
#define _U_PCL_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_handle.h"
#include "u_common.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_PCL  Power Control Library
 *
 *  @ingroup groupMW
 *  @brief Set and get the some information from system power control
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could set and get
 *   the some information from system power control.
 *   
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Power Control library API return values */
#define PCLR_OK                         ((INT32)    0)    /**<  The routine was successful.   */
#define PCLR_NOT_INIT                   ((INT32)   -1)    /**<  The Power Control Library is not initialized.      */
#define PCLR_ALREADY_INIT               ((INT32)   -2)    /**<  The Power Control Library has initialized already.      */
#define PCLR_INV_ARG                    ((INT32)   -3)    /**<  One or more of the arguments contain invalid data.      */
#define PCLR_HW_NOT_SUPPORT             ((INT32)   -4)    /**<  The hardware is not supported      */
#define PCLR_DEV_OPEN_FAILED            ((INT32)   -5)    /**<  Device open operation is failed     */
#define PCLR_DEV_GET_FAILED             ((INT32)   -6)    /**<  Device get operation is failed      */
#define PCLR_DEV_SET_FAILED             ((INT32)   -7)    /**<  Device set operation is failed      */
#define PCLR_MEM_ALLOC_FAILED           ((INT32)   -8)    /**<  Memory allocation is failed.      */
#define PCLR_TIMER_ERROR                ((INT32)   -9)    /**<  A timer error occurred.      */
#define PCLR_HANDLE_LIB_ERROR           ((INT32)  -10)    /**<  A handle library error occurred.      */
#define PCLR_ERROR                      ((INT32)  -11)    /**<  Operation error.      */
#define PCLR_CLI_FAILED                 ((INT32)  -12)    /**<  CLI command execute unsuccessfully.    */

/* Definition of GPIO type */
#define PCL_GPIO_TYPE_GPIO          ((UINT32) 0x00000000) /**<  GPIO type flag   */
#define PCL_GPIO_TYPE_OP_CTRL       ((UINT32) 0x10000000) /**<  Operation control GPIO type flag */

/* Wake-up reason */
/*------------------------------------------------------------------*/
/*! @enum PCL_WAKE_UP_REASON_T
 *  @brief This enumeration contains the system wake-up reasons
 *  @code
 *  typedef enum
 *  {
 *      PCL_WAKE_UP_REASON_UNKNOWN = 0,
 *      PCL_WAKE_UP_REASON_VGA,
 *      PCL_WAKE_UP_REASON_RTC,
 *      PCL_WAKE_UP_REASON_FP,
 *      PCL_WAKE_UP_REASON_IRRC,
 *      PCL_WAKE_UP_REASON_UART,
 *      PCL_WAKE_UP_REASON_AC_POWER,
 *      PCL_WAKE_UP_REASON_HDMI,
 *      PCL_WAKE_UP_REASON_DVD,
 *      PCL_WAKE_UP_REASON_UART_NORMAL,
 *      PCL_WAKE_UP_REASON_RC_DIGIT_0,  
 *      PCL_WAKE_UP_REASON_RC_DIGIT_1,
 *      PCL_WAKE_UP_REASON_RC_DIGIT_2,
 *      PCL_WAKE_UP_REASON_RC_DIGIT_3,
 *      PCL_WAKE_UP_REASON_RC_DIGIT_4,
 *      PCL_WAKE_UP_REASON_RC_DIGIT_5,
 *      PCL_WAKE_UP_REASON_RC_DIGIT_6,
 *      PCL_WAKE_UP_REASON_RC_DIGIT_7,
 *      PCL_WAKE_UP_REASON_RC_DIGIT_8,
 *      PCL_WAKE_UP_REASON_RC_DIGIT_9,
 *      PCL_WAKE_UP_REASON_RC_PRG_UP,
 *      PCL_WAKE_UP_REASON_RC_PRG_DOWN,
 *      PCL_WAKE_UP_REASON_RC_INP_SRC,
 *      PCL_WAKE_UP_REASON_RC_ANALOG,    
 *      PCL_WAKE_UP_REASON_RC_DIGITAL,    
 *      PCL_WAKE_UP_REASON_RC_DIGITAL_ANALOG,    
 *      PCL_WAKE_UP_REASON_FP_PRG_UP,   
 *      PCL_WAKE_UP_REASON_FP_PRG_DOWN,
 *      PCL_WAKE_UP_REASON_FP_INP_SRC,    
 *      PCL_WAKE_UP_REASON_RTC_SPECIAL, 
 *      PCL_WAKE_UP_REASON_CUSTOM_1 = 61, 
 *      PCL_WAKE_UP_REASON_CUSTOM_2 = 62, 
 *      PCL_WAKE_UP_REASON_CUSTOM_3 = 63, 
 *      PCL_WAKE_UP_REASON_CUSTOM_4 = 64
 *  }   PCL_WAKE_UP_REASON_T;
 *  @endcode
 *  @li@c  PCL_WAKE_UP_REASON_UNKNOWN            - The system was waked up by unknown reason
 *  @li@c  PCL_WAKE_UP_REASON_VGA                     - The system was waked up by VGA signal
 *  @li@c  PCL_WAKE_UP_REASON_RTC                     - The system was waked up by RTC
 *  @li@c  PCL_WAKE_UP_REASON_FP                        - The system was waked up by FP input
 *  @li@c  PCL_WAKE_UP_REASON_IRRC                    - The system was waked up by IRRC input
 *  @li@c  PCL_WAKE_UP_REASON_UART                    - The system was waked up by UART signal
 *  @li@c  PCL_WAKE_UP_REASON_AC_POWER           - The system was waked up by AC power-on
 *  @li@c  PCL_WAKE_UP_REASON_HDMI                    - The system was waked up by HDMI signal
 *  @li@c  PCL_WAKE_UP_REASON_DVD                     - The system was waked up by DVD signal
 *  @li@c  PCL_WAKE_UP_REASON_UART_NORMAL      - The system was waked up by UART input (normal mode).   
 *  @li@c  PCL_WAKE_UP_REASON_RC_DIGIT_0         - The system was waked up by IRRC digit 0
 *  @li@c  PCL_WAKE_UP_REASON_RC_DIGIT_1         - The system was waked up by IRRC digit 1
 *  @li@c  PCL_WAKE_UP_REASON_RC_DIGIT_2         - The system was waked up by IRRC digit 2
 *  @li@c  PCL_WAKE_UP_REASON_RC_DIGIT_3         - The system was waked up by IRRC digit 3
 *  @li@c  PCL_WAKE_UP_REASON_RC_DIGIT_4         - The system was waked up by IRRC digit 4
 *  @li@c  PCL_WAKE_UP_REASON_RC_DIGIT_5         - The system was waked up by IRRC digit 5
 *  @li@c  PCL_WAKE_UP_REASON_RC_DIGIT_6         - The system was waked up by IRRC digit 6
 *  @li@c  PCL_WAKE_UP_REASON_RC_DIGIT_7         - The system was waked up by IRRC digit 7
 *  @li@c  PCL_WAKE_UP_REASON_RC_DIGIT_8         - The system was waked up by IRRC digit 8
 *  @li@c  PCL_WAKE_UP_REASON_RC_DIGIT_9         - The system was waked up by IRRC digit 9
 *  @li@c  PCL_WAKE_UP_REASON_RC_PRG_UP          - The system was waked up by IRRC channel+
 *  @li@c  PCL_WAKE_UP_REASON_RC_PRG_DOWN    - The system was waked up by IRRC channel-
 *  @li@c  PCL_WAKE_UP_REASON_RC_INP_SRC        - The system was waked up by IRRC input source
 *  @li@c  PCL_WAKE_UP_REASON_RC_ANALOG          - The system was waked up by IRRC analog
 *  @li@c  PCL_WAKE_UP_REASON_RC_DIGITAL         - The system was waked up by IRRC digital
 *  @li@c  PCL_WAKE_UP_REASON_RC_DIGITAL_ANALOG  - The system was waked up by IRRC digital_analog
 *  @li@c  PCL_WAKE_UP_REASON_FP_PRG_UP           - The system was waked up by FP channel+
 *  @li@c  PCL_WAKE_UP_REASON_FP_PRG_DOWN     - The system was waked up by FP channel-
 *  @li@c  PCL_WAKE_UP_REASON_FP_INP_SRC         - The system was waked up by FP input source
 *  @li@c  PCL_WAKE_UP_REASON_RTC_SPECIAL        - to aviod enter standby  When AC instable 
 *  @li@c  PCL_WAKE_UP_REASON_CUSTOM_1           - The system was waked up by custom1
 *  @li@c  PCL_WAKE_UP_REASON_CUSTOM_2           - The system was waked up by custom2
 *  @li@c  PCL_WAKE_UP_REASON_CUSTOM_3           - The system was waked up by custom3
 *  @li@c  PCL_WAKE_UP_REASON_CUSTOM_4           - The system was waked up by custom4
 */
/*------------------------------------------------------------------*/
typedef enum
{
    PCL_WAKE_UP_REASON_UNKNOWN = 0,
    PCL_WAKE_UP_REASON_VGA,
    PCL_WAKE_UP_REASON_RTC,
    PCL_WAKE_UP_REASON_FP,
    PCL_WAKE_UP_REASON_IRRC,
    PCL_WAKE_UP_REASON_UART,
    PCL_WAKE_UP_REASON_AC_POWER,
    PCL_WAKE_UP_REASON_HDMI,
    PCL_WAKE_UP_REASON_DVD,
    PCL_WAKE_UP_REASON_UART_NORMAL,
    PCL_WAKE_UP_REASON_RC_DIGIT_0,  /* Remote controller */
    PCL_WAKE_UP_REASON_RC_DIGIT_1,
    PCL_WAKE_UP_REASON_RC_DIGIT_2,
    PCL_WAKE_UP_REASON_RC_DIGIT_3,
    PCL_WAKE_UP_REASON_RC_DIGIT_4,
    PCL_WAKE_UP_REASON_RC_DIGIT_5,
    PCL_WAKE_UP_REASON_RC_DIGIT_6,
    PCL_WAKE_UP_REASON_RC_DIGIT_7,
    PCL_WAKE_UP_REASON_RC_DIGIT_8,
    PCL_WAKE_UP_REASON_RC_DIGIT_9,
    PCL_WAKE_UP_REASON_RC_PRG_UP,
    PCL_WAKE_UP_REASON_RC_PRG_DOWN,
    PCL_WAKE_UP_REASON_RC_INP_SRC,
    PCL_WAKE_UP_REASON_RC_ANALOG,
    PCL_WAKE_UP_REASON_RC_DIGITAL,
    PCL_WAKE_UP_REASON_RC_DIGITAL_ANALOG,
    PCL_WAKE_UP_REASON_FP_PRG_UP,   /* Front panel */
    PCL_WAKE_UP_REASON_FP_PRG_DOWN,
    PCL_WAKE_UP_REASON_FP_INP_SRC,    
    PCL_WAKE_UP_REASON_RTC_SPECIAL, /* to aviod enter standby  When AC instable */
    PCL_WAKE_UP_REASON_CUSTOM_1 = 61, 
    PCL_WAKE_UP_REASON_CUSTOM_2 = 62, 
    PCL_WAKE_UP_REASON_CUSTOM_3 = 63, 
    PCL_WAKE_UP_REASON_CUSTOM_4 = 64
}   PCL_WAKE_UP_REASON_T;

/* Wake-up sub-reason mask */
#define PCL_WAKE_UP_SUB_REASON_QUIET_BOOT           MAKE_BIT_MASK_32(0)    /**<   Wake-up sub-reason mask indicates the quiet boot.   */

/* Power-down reason */
/*------------------------------------------------------------------*/
/*! @enum PCL_POWER_DOWN_REASON_T
 *  @brief This enumeration contains the power-down reason
 *  @code
 *  typedef enum
 *  {
 *      PCL_POWER_DOWN_REASON_UNKNOWN = 0,
 *      PCL_POWER_DOWN_REASON_FP,
 *      PCL_POWER_DOWN_REASON_IRRC,
 *      PCL_POWER_DOWN_REASON_CUSTOM_1 = 61, 
 *      PCL_POWER_DOWN_REASON_CUSTOM_2 = 62, 
 *      PCL_POWER_DOWN_REASON_CUSTOM_3 = 63, 
 *      PCL_POWER_DOWN_REASON_CUSTOM_4 = 64        
 *  }   PCL_POWER_DOWN_REASON_T;
 *  @endcode
 *  @li@c  PCL_POWER_DOWN_REASON_UNKNOWN  - This is power-down reason is unknown
 *  @li@c  PCL_POWER_DOWN_REASON_FP              - The power-down is caused by FP
 *  @li@c  PCL_POWER_DOWN_REASON_IRRC          - The power-down is caused by IRRC
 *  @li@c  PCL_POWER_DOWN_REASON_CUSTOM_1  -The power-down is caused by custom1
 *  @li@c  PCL_POWER_DOWN_REASON_CUSTOM_2  -The power-down is caused by custom2
 *  @li@c  PCL_POWER_DOWN_REASON_CUSTOM_3  -The power-down is caused by custom3
 *  @li@c  PCL_POWER_DOWN_REASON_CUSTOM_4  -The power-down is caused by custom4 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    PCL_POWER_DOWN_REASON_UNKNOWN = 0,
    PCL_POWER_DOWN_REASON_FP,
    PCL_POWER_DOWN_REASON_IRRC,
    PCL_POWER_DOWN_REASON_CUSTOM_1 = 61, 
    PCL_POWER_DOWN_REASON_CUSTOM_2 = 62, 
    PCL_POWER_DOWN_REASON_CUSTOM_3 = 63, 
    PCL_POWER_DOWN_REASON_CUSTOM_4 = 64        
}   PCL_POWER_DOWN_REASON_T;

/* Power state control */
/*------------------------------------------------------------------*/
/*! @enum PCL_PWR_STATE_CTRL_T
 *  @brief This enumeration contains the power state control
 *  @code
 *  typedef enum
 *  {
 *      PCL_PWR_STATE_CTRL_STANBY, 
 *      PCL_PWR_STATE_CTRL_STANBY_2,
 *      PCL_PWR_STATE_CTRL_SEMI_STANBY,
 *      PCL_PWR_STATE_CTRL_RUNNING,
 *      PCL_PWR_STATE_CTRL_NORMAL,
 *      PCL_PWR_STATE_CTRL_LAST_VALID_ENTRY
 *  }   PCL_PWR_STATE_CTRL_T;
 *  @endcode
 *  @li@c  PCL_PWR_STATE_CTRL_STANBY      - Only backlight is off but CPU is still running 
 *  @li@c  PCL_PWR_STATE_CTRL_STANBY_2    - Sets the power state to standby
 *  @li@c  PCL_PWR_STATE_CTRL_SEMI_STANBY - Sets the power state to semi-standby
 *  @li@c  PCL_PWR_STATE_CTRL_RUNNING     - Sets the power state to running
 *  @li@c  PCL_PWR_STATE_CTRL_NORMAL      - Sets the power state to normal
 */
/*------------------------------------------------------------------*/
typedef enum
{
    PCL_PWR_STATE_CTRL_STANBY = 0, /* Only backlight is off but CPU is still running */
    PCL_PWR_STATE_CTRL_STANBY_2,
    PCL_PWR_STATE_CTRL_SEMI_STANBY,
    PCL_PWR_STATE_CTRL_RUNNING,
    PCL_PWR_STATE_CTRL_NORMAL,
    PCL_PWR_STATE_CTRL_LAST_VALID_ENTRY
}   PCL_PWR_STATE_CTRL_T;
 
/* GPIO mode */
/*------------------------------------------------------------------*/
/*! @enum PCL_GPIO_MODE_T
 *  @brief This enumeration contains the GPIO mode
 *  @code
 *  typedef enum
 *  {
 *      PCL_GPIO_MODE_INPUT = 0,
 *      PCL_GPIO_MODE_OUTPUT
 *  }   PCL_GPIO_MODE_T;
 *  @endcode
 *  @li@c  PCL_GPIO_MODE_INPUT     - GPIO mode is input
 *  @li@c  PCL_GPIO_MODE_OUTPUT  - GPIO mode is output
 */
/*------------------------------------------------------------------*/
typedef enum
{
    PCL_GPIO_MODE_INPUT = 0,
    PCL_GPIO_MODE_OUTPUT
}   PCL_GPIO_MODE_T;

/* GPIO status */
/*------------------------------------------------------------------*/
/*! @enum PCL_GPIO_STATUS_T
 *  @brief This enumeration contains the GPIO status
 *  @code
 *  typedef enum
 *  {
 *      PCL_GPIO_STATUS_UNKNOWN = 0,
 *      PCL_GPIO_STATUS_LOW,
 *      PCL_GPIO_STATUS_HIGH
 *  }   PCL_GPIO_STATUS_T;
 *  @endcode
 *  @li@c  PCL_GPIO_STATUS_UNKNOWN - GPIO status is unknown
 *  @li@c  PCL_GPIO_STATUS_LOW          - GPIO status is low
 *  @li@c  PCL_GPIO_STATUS_HIGH         - GPIO status is high
 */
/*------------------------------------------------------------------*/
typedef enum
{
    PCL_GPIO_STATUS_UNKNOWN = 0,
    PCL_GPIO_STATUS_LOW,
    PCL_GPIO_STATUS_HIGH
}   PCL_GPIO_STATUS_T;

/* GPIO monitoring control */
/*------------------------------------------------------------------*/
/*! @struct PCL_GPIO_MONITOR_CTRL_T
 *  @brief This structure defines GPIO monitoring control data information
 *  @code
 *  typedef struct _PCL_GPIO_MONITOR_CTRL_T
 *  {
 *      UINT64      ui8_ctrl;
 *      UINT64      ui8_data;           
 *      BOOL        b_monitor_both_chg; 
 *  }   PCL_GPIO_MONITOR_CTRL_T;
 *  @endcode
 *  @li@c  ui8_ctrl                      - Contains a GPIO mask indicating which GPIOs to be monitored
 *  @li@c  ui8_data                    - Contains the GPIO status for matched monitoring
 *  @li@c  b_monitor_both_chg   - Indicates a pattern-matched or state-changed monitoring mechanism
 */
/*------------------------------------------------------------------*/
typedef struct _PCL_GPIO_MONITOR_CTRL_T
{
    UINT64      ui8_ctrl;
    UINT64      ui8_data;           /* Valid when b_monitor_both_chg = FALSE */
    BOOL        b_monitor_both_chg; /* If TRUE, monitor both low->high and high->low changes */
}   PCL_GPIO_MONITOR_CTRL_T;

/* GPIO monitoring control */
/*------------------------------------------------------------------*/
/*! @struct PCL_GPIO_MONITOR_CTRL_EX_T
 *  @brief This structure defines GPIO monitoring control data information
 *  @code
 *  typedef struct _PCL_GPIO_MONITOR_CTRL_EX_T
 *  {
 *      UINT32  ui4_gpio;
 *      BOOL    b_monitor; 
 *  }   PCL_GPIO_MONITOR_CTRL_EX_T;
 *  @endcode
 *  @li@c  ui4_gpio                      - Contains a GPIO mask indicating which GPIOs to be monitored
 *  @li@c  b_monitor                    - decide whether to monitor
 */
/*------------------------------------------------------------------*/
typedef struct _PCL_GPIO_MONITOR_CTRL_EX_T
{
    UINT32  ui4_gpio;
    BOOL    b_monitor;
} PCL_GPIO_MONITOR_CTRL_EX_T;

/* SIF info */
/*------------------------------------------------------------------*/
/*! @enum PCL_SIF_CH_ID_T
 *  @brief  This enumeration contains the SIF channel ID
 *  @code 
 *  typedef enum
 *  {
 *      PCL_SIF_CH_ID_SYS_BUS   = 0x00,
 *      PCL_SIF_CH_ID_TUNER_BUS = 0x01,
 *      PCL_SIF_CH_ID_EDID_BUS  = 0x10
 *  }   PCL_SIF_CH_ID_T;
 *  @endcode
 *  @li@c  PCL_SIF_CH_ID_SYS_BUS        - SIF channel ID is system bus
 *  @li@c  PCL_SIF_CH_ID_TUNER_BUS    - SIF channel ID is tuner bus
 *  @li@c  PCL_SIF_CH_ID_EDID_BUS      - SIF channel ID is EDID bus
 */
/*------------------------------------------------------------------*/
typedef enum
{
    PCL_SIF_CH_ID_SYS_BUS   = 0x00,
    PCL_SIF_CH_ID_TUNER_BUS = 0x01,
    PCL_SIF_CH_ID_EDID_BUS  = 0x10
}   PCL_SIF_CH_ID_T;

/*------------------------------------------------------------------*/
/*! @struct PCL_SIF_INFO_T
 *  @brief This structure defines SIF data information
 *  @code
 *  typedef struct _PCL_SIF_INFO_T
 *  {
 *      PCL_SIF_CH_ID_T   e_channel_id;
 *      UINT32                   ui4_add;
 *      UINT32                   ui4_offset;
 *      UINT8                     ui1_offset_len;
 *      UINT8*                   pui1_data;     
 *      UINT8                     ui1_data_len;     
 *  }   PCL_SIF_INFO_T;
 *  @endcode
 *  @li@c  e_channel_id     - Contains the channel ID as defined in PCL_SIF_CH_ID_T
 *  @li@c  ui4_addr           - Contains the SIF device address
 *  @li@c  ui4_offset          - Contains the SIF device sub-address
 *  @li@c  ui1_offset_len    - Contains the SIF offset length (0~3)
 *  @li@c  pui1_data          - References to the SIF data
 *  @li@c  ui1_data_len      - Contains the SIF data length
 */
/*------------------------------------------------------------------*/
typedef struct _PCL_SIF_INFO_T
{
    PCL_SIF_CH_ID_T     e_channel_id;
    UINT32              ui4_addr;       /* Device address */
    UINT32              ui4_offset;     /* Sub-address (or called offset) */
    UINT8               ui1_offset_len; /* Offset length: 0~3 */   
    UINT8*              pui1_data;     
    UINT8               ui1_data_len;     
}   PCL_SIF_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct PCL_SIF_INFO_BY_GPIO_T
 *  @brief This structure defines SIF operation via GPIO data information
 *  @code
 *  typedef struct _PCL_SIF_INFO_BY_GPIO_T
 *  {
 *      UINT32      ui4_addr_gpio_id;
 *      UINT32      ui4_data_gpio_id;
 *      UINT32      ui4_addr;
 *      UINT32      ui4_offset;
 *      UINT8        ui1_offset_len;        
 *      UINT8*      pui1_data;     
 *      UINT8       ui1_data_len;     
 *  }   PCL_SIF_INFO_BY_GPIO_T;
 *  @endcode
 *  @li@c  ui4_addr_gpio_id   - Contains the GPIO ID for controlling SIF address bus
 *  @li@c  ui4_data_gpio_id   - Contains the GPIO ID for controlling SIF data bus
 *  @li@c  ui4_addr               - Contains the SIF device address
 *  @li@c  ui4_offset             - Contains the SIF device sub-address
 *  @li@c  ui1_offset_len       - Contains the SIF offset length (0~3).
 *  @li@c  pui1_data             - References to the SIF data
 *  @li@c  ui1_data_len         - Contains the SIF data length
 */
/*------------------------------------------------------------------*/
typedef struct _PCL_SIF_INFO_BY_GPIO_T
{
    UINT32      ui4_addr_gpio_id;
    UINT32      ui4_data_gpio_id;
    UINT32      ui4_addr;        /* Device address */
    UINT32      ui4_offset;      /* Sub-address (or called offset) */
    UINT8       ui1_offset_len;  /* Offset length: 0~3 */   
    UINT8*      pui1_data;     
    UINT8       ui1_data_len;     
}   PCL_SIF_INFO_BY_GPIO_T;

/* Servo AD info */
/*------------------------------------------------------------------*/
/*! @struct PCL_SERVO_AD_INFO_T
 *  @brief This structure defines servo AD data information
 *  @code
 *  typedef struct _PCL_SERVO_AD_INFO_T
 *  {
 *      UINT32      ui4_id;
 *      UINT32      ui4_data;
 *  }   PCL_SERVO_AD_INFO_T;
 *  @endcode
 *  @li@c  ui4_id       - Contains the ID for servo AD operation
 *  @li@c  ui4_data   - Contains the data for servo AD operation
 */
/*------------------------------------------------------------------*/
typedef struct _PCL_SERVO_AD_INFO_T
{
    UINT32      ui4_id;
    UINT32      ui4_data;
}   PCL_SERVO_AD_INFO_T;

/* Notify condition */
/*------------------------------------------------------------------*/
/*! @enum PCL_COND_T
 *  @brief This enumeration contains the Power Control Library notify values which 
 *             are carried in argument e_nfy_cond of API (*x_pcl_nfy_fct). 
 *  @code
 *  typedef enum
 *  {
 *      PCL_COND_UNKNOWN = 0,
 *      PCL_COND_WAKE_UP_EXPIRED,
 *      PCL_COND_MONITORING_GPIO_MATCHED,
 *      PCL_COND_MONITORING_GPIO_CHANGED,
 *      PCL_COND_RES_CNT_UPDATE_PANEL,
 *      PCL_COND_RES_CNT_UPDATE_CPU,
 *      PCL_COND_RES_CNT_UPDATE_CUSTOM_1,
 *      PCL_COND_RES_CNT_UPDATE_CUSTOM_2,
 *      PCL_COND_RES_CNT_UPDATE_CUSTOM_3,
 *      PCL_COND_RES_CNT_UPDATE_CUSTOM_4
 *  }   PCL_COND_T;
 *  @endcode
 *  @li@c  PCL_COND_UNKNOWN                                - The condition is unknown
 *  @li@c  PCL_COND_WAKE_UP_EXPIRED                   - The setup of wake-up time has expired
 *  @li@c  PCL_COND_MONITORING_GPIO_MATCHED   - The state of monitored GPIO is changed
 *                                                                                and its value is the same as client's request
 *  @li@c  PCL_COND_MONITORING_GPIO_CHANGED   - The state of monitored GPIO is changed
 *  @li@c  PCL_COND_RES_CNT_UPDATE_PANEL          - The panel resource count is updated
 *  @li@c  PCL_COND_RES_CNT_UPDATE_CPU             - The CPU resource count is updated
 *  @li@c  PCL_COND_RES_CNT_UPDATE_CUSTOM_1   - The custom1 resource count is updated
 *  @li@c  PCL_COND_RES_CNT_UPDATE_CUSTOM_2   - The custom2 resource count is updated
 *  @li@c  PCL_COND_RES_CNT_UPDATE_CUSTOM_3   - The custom3 resource count is updated
 *  @li@c  PCL_COND_RES_CNT_UPDATE_CUSTOM_4   - The custom4 resource count is updated
 */
/*------------------------------------------------------------------*/
typedef enum
{
    PCL_COND_UNKNOWN = 0,
    PCL_COND_WAKE_UP_EXPIRED,
    PCL_COND_MONITORING_GPIO_MATCHED,
    PCL_COND_MONITORING_GPIO_CHANGED,
    PCL_COND_RES_CNT_UPDATE_PANEL,
    PCL_COND_RES_CNT_UPDATE_CPU,
    PCL_COND_RES_CNT_UPDATE_CUSTOM_1,
    PCL_COND_RES_CNT_UPDATE_CUSTOM_2,
    PCL_COND_RES_CNT_UPDATE_CUSTOM_3,
    PCL_COND_RES_CNT_UPDATE_CUSTOM_4
#ifdef LINUX_TURNKEY_SOLUTION_PHASE2
        ,
    PCL_COND_POWER_DOWN_TIMEOUT
#endif
}   PCL_COND_T;

/* Notify function */
/*------------------------------------------------------------------*/
/*! @brief This notify function is called by the Power Control Library to notifies
 *              the clients of the condition change of Power Control driver
 *
 *  @param [in]  h_nfy          - Contains the event code as defined in Event code table. 
 *  @param [in]  pv_nfy_tag  - References a tag value, which was obtained when 
 *                                           executing API x_pcl_reg_nfy_fct and c_pcl_reg_nfy_fct.
 *  @param [in]  e_nfy_cond  - Contains the notify condition as defined in enumeration
 *                                           PCL_ COND_T.
 *  @param [in]  ui4_data      - Contains additional information associated to the event. 
 *                                           It may be set to 0 if no additional information is available.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return VOID.
 *  @retval      - None
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_pcl_nfy_fct)(HANDLE_T        h_nfy,
                              VOID*            pv_nfy_tag,
                              PCL_COND_T    e_nfy_cond,
                              UINT32        ui4_data);

/* Wake-up capabilities (for wake-up setup & capability operations) */
#define PCL_WAKE_UP_CAP_VGA                 MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_VGA - 1))    /**<  Support wake-up source from VGA.     */
#define PCL_WAKE_UP_CAP_RTC                 MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RTC - 1))    /**<  Support wake-up source from RTC.      */
#define PCL_WAKE_UP_CAP_FP                  MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_FP - 1))    /**<  Support wake-up source from FP.       */
#define PCL_WAKE_UP_CAP_IRRC                MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_IRRC - 1))    /**<  Support wake-up source from IRRC.       */
#define PCL_WAKE_UP_CAP_UART                MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_UART - 1))    /**<  Support wake-up source from UART.       */
#define PCL_WAKE_UP_CAP_AC_POWER            MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_UART - 1))    /**<  Support wake-up source from AC power-on.      */
#define PCL_WAKE_UP_CAP_HDMI                MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_HDMI - 1))    /**<  Support wake-up source from HDMI.      */
#define PCL_WAKE_UP_CAP_UART_NORMAL         MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_UART_NORMAL - 1))    /**<  Support wake-up source from UART input(normal mode).    */
#define PCL_WAKE_UP_CAP_RC_DIGIT_0          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_DIGIT_0 - 1))    /**<  Support wake-up source from IRRC digit 0.   */
#define PCL_WAKE_UP_CAP_RC_DIGIT_1          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_DIGIT_1 - 1))    /**<  Support wake-up source from IRRC digit 1.     */
#define PCL_WAKE_UP_CAP_RC_DIGIT_2          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_DIGIT_2 - 1))    /**<  Support wake-up source from IRRC digit 2.     */
#define PCL_WAKE_UP_CAP_RC_DIGIT_3          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_DIGIT_3 - 1))    /**<  Support wake-up source from IRRC digit 3.    */
#define PCL_WAKE_UP_CAP_RC_DIGIT_4          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_DIGIT_4 - 1))    /**<  Support wake-up source from IRRC digit 4. */
#define PCL_WAKE_UP_CAP_RC_DIGIT_5          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_DIGIT_5 - 1))    /**<  Support wake-up source from IRRC digit 5.     */
#define PCL_WAKE_UP_CAP_RC_DIGIT_6          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_DIGIT_6 - 1))    /**<  Support wake-up source from IRRC digit 6.    */
#define PCL_WAKE_UP_CAP_RC_DIGIT_7          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_DIGIT_7 - 1))    /**<  Support wake-up source from IRRC digit 7.     */
#define PCL_WAKE_UP_CAP_RC_DIGIT_8          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_DIGIT_8 - 1))    /**<  Support wake-up source from IRRC digit 8.    */
#define PCL_WAKE_UP_CAP_RC_DIGIT_9          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_DIGIT_9 - 1))    /**<  Support wake-up source from IRRC digit 9.    */
#define PCL_WAKE_UP_CAP_RC_PRG_UP           MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_PRG_UP - 1))    /**<  Support wake-up source from IRRC CH+.      */
#define PCL_WAKE_UP_CAP_RC_PRG_DOWN         MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_PRG_DOWN - 1))    /**<  Support wake-up source from IRRC digit CH-.      */
#define PCL_WAKE_UP_CAP_RC_INP_SRC          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RC_INP_SRC - 1))    /**<  Support wake-up source from IRRC input source.      */
#define PCL_WAKE_UP_CAP_FP_PRG_UP           MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_FP_PRG_UP - 1))    /**<  Support wake-up source from FP CH+.     */
#define PCL_WAKE_UP_CAP_FP_PRG_DOWN         MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_FP_PRG_DOWN - 1))    /**<  Support wake-up source from FP CH1.     */
#define PCL_WAKE_UP_CAP_FP_INP_SRC          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_FP_INP_SRC - 1))    /**<  Support wake-up source from FP input source.     */
#define PCL_WAKE_UP_CAP_CUSTOM_1            MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_CUSTOM_1 - 1))    /**<  Support wake-up source from custom 1.      */
#define PCL_WAKE_UP_CAP_CUSTOM_2            MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_CUSTOM_2 - 1))    /**<  Support wake-up source from custom 2.      */
#define PCL_WAKE_UP_CAP_CUSTOM_3            MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_CUSTOM_3 - 1))    /**<  Support wake-up source from custom 3.     */
#define PCL_WAKE_UP_CAP_CUSTOM_4            MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_CUSTOM_4 - 1))    /**<  Support wake-up source from custom 4.     */

#define PCL_WKE_UP_CAP_RTC_SPECIAL          MAKE_BIT_MASK_32((PCL_WAKE_UP_REASON_RTC_SPECIAL - 1))    /**<  Support wake-up source from RTC, but should not enter standby when AC power is instable.      */
/* Miscellaneous controls */
/*
    Operation | PCL_MISC_CTRL_NO_STANDBY_2 PCL_MISC_CTRL_NO_STANDBY  
    ---------------------------------------------------------------
    Standby   |             0                        0
    On        |             0                        1
    Last sts  |             1                        x
*/
#define PCL_MISC_CTRL_LAST_STS                  PCL_MISC_CTRL_NO_STANDBY_2    /**<   Last staus of miscellaneous controls   */
#define PCL_MISC_CTRL_IGNORE                    PCL_MISC_CTRL_RESERVED    /**<   Ignore     */

#define PCL_MISC_CTRL_NO_STANDBY                MAKE_BIT_MASK_32(0)  /* This bit is valid when PCL_MISC_CTRL_NO_STANDBY_2 = 0 */    /**<  Controls whether the system should enter standby or not after startup.This bit is valid when PCL_MISC_CTRL_NO_STANDBY_2 = 0     */
#define PCL_MISC_CTRL_FACTORY_MODE              MAKE_BIT_MASK_32(1)    /**<  Controls whether the system should enter factory mode or not.   */
#define PCL_MISC_CTRL_POWER_LED_OFF             MAKE_BIT_MASK_32(2)    /**<  Contol oower LED on/off  */
#define PCL_MISC_CTRL_NO_STANDBY_2              MAKE_BIT_MASK_32(3)    /**<  Additional information for standby control.      */
#define PCL_MISC_CTRL_FIRMWARE_UPGRADE          MAKE_BIT_MASK_32(4)    /**<  Control Firmware upgrade miscellaneous controls      */
#define PCL_MISC_CTRL_COMPANION_CHIP_UPGRADE    MAKE_BIT_MASK_32(5)    /**<  Control companion chip upgrade operation.      */
#define PCL_MISC_CTRL_QUIET_BOOT                MAKE_BIT_MASK_32(6)  /* Backlight is off and power LED is unchanged */    /**<  Controls quiet boot operation (i.e. backlight is off and power LED is unchanged)     */
#define PCL_MISC_CTRL_DUAL_BANK                 MAKE_BIT_MASK_32(7)    /**<  Controls dual bank.    */
#define PCL_MISC_CTRL_NO_IR_STANDBY_BY_DRIVER   MAKE_BIT_MASK_32(8)    /**<  No standby miscellaneous controls      */
#define PCL_MISC_CTRL_DUAL_BANK_SYSTEM          MAKE_BIT_MASK_32(9)    /**<  Controls dual bank for system image and 8032 uP.    */
#define PCL_MISC_CTRL_DUAL_BANK_ROOTFS          MAKE_BIT_MASK_32(10)   /**<  Controls dual bank for root fs.    */
#define PCL_MISC_CTRL_DUAL_BANK_KERNEL          MAKE_BIT_MASK_32(11)   /**<  Controls dual bank for kernel.     */
#ifdef MW_FAST_INIT
#define PCL_MISC_CTRL_FAST_BOOT                 MAKE_BIT_MASK_32(12) /* index 0/1 for kernel */
#define PCL_MISC_CTRL_NORMAL_BOOT_WITH_SUSPEND  MAKE_BIT_MASK_32(13) /* index 0/1 for kernel */
#define PCL_MISC_CTRL_NORMAL_BOOT_WITHOUT_SUSPEND    MAKE_BIT_MASK_32(14) /* index 0/1 for kernel */
#endif
#define PCL_MISC_CTRL_RESERVED                  MAKE_BIT_MASK_32(31) /* Reserved for PCL internal use only */    /**<  Reserved for PCL internal use only.      */

/* Miscellaneous status */
#define PCL_MISC_STS_POWER_DOWN_BY_AC_CUT       MAKE_BIT_MASK_32(0)    /**<  Indicates the system was powered off by AC power cut.    */

/* Upgrade controls */
#define PCL_UPGRADE_CTRL_NOR_0                  MAKE_BIT_MASK_32(0)    /**<  Upgrades partition 0 of nor flash     */
#define PCL_UPGRADE_CTRL_NOR_1                  MAKE_BIT_MASK_32(1)    /**<  Upgrades Partition 1 of nor flash        */
#define PCL_UPGRADE_CTRL_NOR_2                  MAKE_BIT_MASK_32(2)    /**<  Upgrades Partition 2 of nor flash        */
#define PCL_UPGRADE_CTRL_NOR_3                  MAKE_BIT_MASK_32(3)    /**<  Upgrades Partition 3 of nor flash        */
#define PCL_UPGRADE_CTRL_NOR_4                  MAKE_BIT_MASK_32(4)    /**<  Upgrades Partition 4 of nor flash        */
#define PCL_UPGRADE_CTRL_NOR_5                  MAKE_BIT_MASK_32(5)    /**<  Upgrades Partition 5 of nor flash        */
#define PCL_UPGRADE_CTRL_NOR_6                  MAKE_BIT_MASK_32(6)    /**<  Upgrades Partition 6 of nor flash        */
#define PCL_UPGRADE_CTRL_NOR_7                  MAKE_BIT_MASK_32(7)    /**<  Upgrades Partition 7 of nor flash        */
#define PCL_UPGRADE_CTRL_NVM                    MAKE_BIT_MASK_32(8)    /**<  Upgrades NVM      */
#define PCL_UPGRADE_CTRL_STANDBY_MICRO_P        MAKE_BIT_MASK_32(9)    /**<  Upgrades standby uP     */
#define PCL_UPGRADE_CTRL_COMPANION_CHIP         MAKE_BIT_MASK_32(10)    /**<   Upgrades companion chip      */
#define PCL_UPGRADE_CTRL_NOR_8                  MAKE_BIT_MASK_32(11)    /**<  Upgrades Partition 8 of nor flash        */

/* Wake-up control */
/*------------------------------------------------------------------*/
/*! @struct PCL_WAKE_UP_CTRL_T
 *  @brief This structure defines wake-up control data information
 *  @code
 *  typedef struct _PCL_WAKE_UP_CTRL_T
 *  {
 *      TIME_T      t_time;
 *      UINT32      ui4_misc_ctrl;
 *  }   PCL_WAKE_UP_CTRL_T;
 *  @endcode
 *  @li@c  t_time              - Contains the wake-up time
 *  @li@c  ui4_misc_ctrl    - Contains the miscellaneous controls
 */
/*------------------------------------------------------------------*/
typedef struct _PCL_WAKE_UP_CTRL_T
{
    TIME_T      t_time;
    UINT32      ui4_misc_ctrl;
}   PCL_WAKE_UP_CTRL_T;

/* Resource type */
/*------------------------------------------------------------------*/
/*! @enum PCL_RES_TYPE_T
 *  @brief This enumeration contains the resource type
 *  @code
 *  typedef enum
 *  {
 *      PCL_RES_TYPE_PANEL = 0,
 *      PCL_RES_TYPE_CPU,
 *      PCL_RES_TYPE_CUSTOM_1,
 *      PCL_RES_TYPE_CUSTOM_2,
 *      PCL_RES_TYPE_CUSTOM_3,
 *      PCL_RES_TYPE_CUSTOM_4,
 *      PCL_RES_TYPE_MAX
 *  }   PCL_RES_TYPE_T;
 *  @endcode
 *  @li@c  PCL_RES_TYPE_PANEL           - The resource type is panel
 *  @li@c  PCL_RES_TYPE_CPU              - The resource type is CPU
 *  @li@c  PCL_RES_TYPE_CUSTOM_1    - The resource type is custom1
 *  @li@c  PCL_RES_TYPE_CUSTOM_2    - The resource type is custom2
 *  @li@c  PCL_RES_TYPE_CUSTOM_3    - The resource type is custom3
 *  @li@c  PCL_RES_TYPE_CUSTOM_4    - The resource type is custom4
 *  @li@c  PCL_RES_TYPE_MAX              - 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    PCL_RES_TYPE_PANEL = 0,
    PCL_RES_TYPE_CPU,
    PCL_RES_TYPE_CUSTOM_1,
    PCL_RES_TYPE_CUSTOM_2,
    PCL_RES_TYPE_CUSTOM_3,
    PCL_RES_TYPE_CUSTOM_4,
    PCL_RES_TYPE_MAX
}   PCL_RES_TYPE_T;

#ifdef MW_FAST_INIT
/* Boot type */
/*------------------------------------------------------------------*/
/*! @enum PCL_BOOT_TYPE_T
 *  @brief This enumeration contains the boot type of linux os.
 *  @code
 *  typedef enum
 *  {
 *      PCL_BOOT_TYPE_FAST_BOOT,
 *      PCL_BOOT_TYPE_NORMAL_BOOT_WITH_SUSPEND,
 *      PCL_BOOT_TYPE_NORMAL_BOOT_WO_SUSPEND
 *  }   PCL_DUAL_BANK_TYPE_T;
 *  @endcode
 *  @li@c  PCL_BOOT_TYPE_FAST_BOOT          - fast boot.
 *  @li@c  PCL_BOOT_TYPE_NORMAL_BOOT_WITH_SUSPEND  - Normal boot with suspend.
 *  @li@c  PCL_BOOT_TYPE_NORMAL_BOOT_WO_SUSPEND    - Normal bootout with suspend.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    PCL_BOOT_TYPE_FAST_BOOT = 0,
    PCL_BOOT_TYPE_NORMAL_BOOT_WITH_SUSPEND,
    PCL_BOOT_TYPE_NORMAL_BOOT_WO_SUSPEND
}   PCL_BOOT_TYPE_T;
#endif

/* Dual bank type */
/*------------------------------------------------------------------*/
/*! @enum PCL_DUAL_BANK_TYPE_T
 *  @brief This enumeration contains the dual bank type
 *  @code
 *  typedef enum
 *  {
 *      PCL_DUAL_BANK_SYSTEM,
 *      PCL_DUAL_BANK_ROOTFS,
 *      PCL_DUAL_BANK_SYSTEM_AND_ROOTFS,
 *      PCL_DUAL_BANK_KERNEL,
 *  }   PCL_DUAL_BANK_TYPE_T;
 *  @endcode
 *  @li@c  PCL_DUAL_BANK_SYSTEM     - Dual bank for system image and
 *                                    8032 uP.
 *  @li@c  PCL_DUAL_BANK_ROOTFS     - Dual bank for root FS.
 *  @li@c  PCL_DUAL_BANK_SYSTEM_AND_ROOTFS - Dual bank for both system & rootFS.
 *  @li@c  PCL_DUAL_BANK_KERNEL     - Dual bank for linux kernel.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    PCL_DUAL_BANK_SYSTEM,
    PCL_DUAL_BANK_ROOTFS,
    PCL_DUAL_BANK_SYSTEM_AND_ROOTFS,
    PCL_DUAL_BANK_KERNEL
}   PCL_DUAL_BANK_TYPE_T;

#endif /* _U_PCL_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                               /* end of groupMW_PCL */
/*----------------------------------------------------------------------------*/

