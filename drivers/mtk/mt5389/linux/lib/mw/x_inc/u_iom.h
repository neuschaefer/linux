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
/*! @file u_iom.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains IO Manager related definitions, which
 *         are known to applications and middleware.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_IOM_H_
#define _U_IOM_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_IO_MNGR  IO Manager
 *
 *  @ingroup groupMW
 *  @brief Control the I/O from Application
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could control
 *   system input and output.
 *   
 *  @par See:
 *  @ref pageMW_IO_MNGR "IO Manager Detailed Description"
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* I/O Manager API return values */
#define IOMR_OK                         ((INT32)    0) /**< Routine successful  */
#define IOMR_NOT_INIT                   ((INT32)   -1) /**< The IO Manager is not initialized  */
#define IOMR_ALREADY_INIT               ((INT32)   -2) /**< The IO Manager has already been  initialized  */
#define IOMR_INV_ARG                    ((INT32)   -3) /**< One or more of the arguments contain invalid data  */
#define IOMR_INV_EVT_GRP_MASK           ((INT32)   -4) /**< Event group mask is invalid  */
#define IOMR_INV_HANDLE                 ((INT32)   -5) /**< The handle is invalid  */
#define IOMR_MEM_ALLOC_FAILED           ((INT32)   -6) /**< Memory allocation is failed  */
#define IOMR_NOT_SUPPORT                ((INT32)   -7) /**< IOM Manager  does not support the operation  */
#define IOMR_DEV_FULL                   ((INT32)   -8) /**< Device array is full  */
#define IOMR_EVT_FILTR_FULL             ((INT32)   -9) /**< Reach the maximum of event filters associated to a device  */
#define IOMR_DEV_OPEN_FAILED            ((INT32)  -10) /**< Device open operation is failed  */
#define IOMR_DEV_GET_FAILED             ((INT32)  -11) /**< Device get operation is failed  */
#define IOMR_DEV_SET_FAILED             ((INT32)  -12) /**< Device set operation is failed  */
#define IOMR_DEV_QUERY_FAILED           ((INT32)  -13) /**< Device info query operation is failed  */
#define IOMR_DEV_CLOSE_FAILED           ((INT32)  -14) /**< Device close operation is failed  */
#define IOMR_DEV_BUSY                   ((INT32)  -15) /**< Device is busy  */
#define IOMR_MSGQ_FULL                  ((INT32)  -16) /**< Message Queue is full  */
#define IOMR_HANDLE_LIB_ERROR           ((INT32)  -17) /**< A handle library error occurred  */
#define IOMR_TIMER_ERROR                ((INT32)  -18) /**< A timer error occurred  */
#define IOMR_CLI_ERROR                  ((INT32)  -19) /**< CLI operation is failed  */
#define IOMR_ERROR                      ((INT32)  -20) /**< IOM error  */

/* Definition of handle type */
#define IOM_HANDLE_TYPE_IN              (HT_GROUP_IOM + ((HANDLE_TYPE_T) 0)) /**< IO Manager handle type is IN  */
#define IOM_HANDLE_TYPE_OUT             (HT_GROUP_IOM + ((HANDLE_TYPE_T) 1)) /**< IO Manager handle type is OUT */
#define IOM_HANDLE_TYPE_IO              (HT_GROUP_IOM + ((HANDLE_TYPE_T) 2)) /**< IO Manager handle type is IN-OUT */

#define IOM_REMOVE_APDEMON_IRRC         1    /**< Support APDEMON IRRC or not  */
#define IOM_REMOVE_DBG                  1    /**< Support REMOVE DBG or not   */

/* Definition for debug disable keys for IRRC */
#define IOM_SUPPORT_IRRC_DISABLE_KEYS   1    /**< Support IRRC Disable Keys or not */

/* Definition of repeat event interval */
#define IOM_ZERO_RPT_EVT_ITVL           ((UINT16) 0x0)  /**<  No repeat events are required   */

/* Definition of maximum data size */
#define IOM_MAX_IRRC_DATA_SIZE          ((UINT16)  11)  /**< byte   */
#define IOM_MAX_DBG_DATA_SIZE           ((UINT16) 128)  /**< byte   */

/* Definition of IO Manager device ID */
#define IOM_DEV_ID_ANY                  ((UINT16) 0xFFFF)    /**<  Definition of IO Manager device ID   */

/* Macros for event code handling ********************************************/
/*
    31-----------28+27-----------24+23-------------16+15----------0
       event_src      event_attr       event_group       event_id
*/
#define IOM_GET_EVT_SRC(_val)                   (((UINT32)_val) & 0xF0000000) /**< IOM get event source  */
#define IOM_GET_EVT_ATTR(_val)                  (((UINT32)_val) & 0x0F000000) /**< IOM get event attribute */
#define IOM_GET_EVT_GRP(_val)                   (((UINT32)_val) & 0x00FF0000) /**< IOM get event group   */
#define IOM_GET_EVT_ID(_val)                    (((UINT32)_val) & 0x0000FFFF) /**< IOM get event ID  */
#define IOM_GET_EVT_GRP_ID(_val)                (((UINT32)_val) & 0x00FFFFFF) /**< IOM get event group ID  */

#define IOM_GEN_EVT_CODE(_src, _grp_id)         (((UINT32)_src) | ((UINT32)_grp_id)) /* event_src + event_group + event_id */    /**<        */

#define IOM_EVT_SRC_TO_MASK(_val)               ((UINT64)(1 << (((UINT32)_val) >> 28)))    /**<        */
#define IOM_EVT_ATTR_TO_MASK(_val)              ((UINT64)(1 << (((((UINT32)_val) >> 24) & 0xF) - 1)))    /**<        */
#define IOM_EVT_GRP_TO_MASK(_val)               ((UINT64)(1 << (((((UINT32)_val) >> 16) & 0xFF) - 1)))    /**<        */

#define IOM_REMOVE_EVT_SRC(_val)                (((UINT32)_val) & 0x0FFFFFFF)    /**<        */
#define IOM_REMOVE_EVT_ATTR(_val)               (((UINT32)_val) & 0xF0FFFFFF)    /**<        */
#define IOM_REMOVE_EVT_SRC_ATTR(_val)           (((UINT32)_val) & 0x00FFFFFF)    /**<        */

/* Macros for IRRC raw data handling *****************************************/
/* (type, addr, cmd) => raw_bits transformation macro */
/* Type definition: 0x1 = RC5, 0x2 = RC6 */ 
#define IOM_RC5_TO_RAW_BITS(type, addr, cmd)    \
((cmd < 64) ? (0x2 | ((type & 0xF) << 28) | ((addr & 0x1F) << 3) | ((cmd & 0x3F) << 8)) : (((type & 0xF) << 28) | ((addr & 0x1F) << 3) | ((cmd & 0x3F) << 8))) /**<  convert RC5 to raw bits   */

#define IOM_RC6_TO_RAW_BITS(type, addr, cmd)    \
(((type & 0xF) << 28) | ((addr & 0xFF) << 7) | ((cmd & 0xFF) << 15)) /**< convert RC6 to raw bits   */

#define IOM_RC56_TO_RAW_BITS(type, addr, cmd)   \
((type == 0x1) ? IOM_RC5_TO_RAW_BITS(type, addr, cmd) : IOM_RC6_TO_RAW_BITS(type, addr, cmd)) /**<  convert RC56 to raw bits  */

/* IOM device type */
/*------------------------------------------------------------------*/
/*! @enum IOM_DEV_TYPE_T
 *  @brief  This enumeration contains the device types
 *  @code
 *  typedef enum
 *  {
 *      IOM_DEV_TYPE_NONE = 0,
 *      IOM_DEV_TYPE_IN_IRRC,
 *      IOM_DEV_TYPE_IN_IRRK,
 *      IOM_DEV_TYPE_IN_IRRM,
 *      IOM_DEV_TYPE_IN_DBG,
 *      IOM_DEV_TYPE_OUT_IND_POWER,
 *      IOM_DEV_TYPE_OUT_FPD,
 *      IOM_DEV_TYPE_OUT_IRB,
 *      IOM_DEV_TYPE_IO_UART,
 *      IOM_DEV_TYPE_IO_I2C
 *  }   IOM_DEV_TYPE_T;
 *  @endcode
 *  @li@c  IOM_DEV_TYPE_NONE                       - This is an unknown device type
 *  @li@c  IOM_DEV_TYPE_IN_IRRC                   -  This is IR remote controller device type
 *  @li@c  IOM_DEV_TYPE_IN_IRRK                  -  This is IR remote keyboard device type
 *  @li@c  IOM_DEV_TYPE_IN_IRRM                  -  This is IR remote mouse device type
 *  @li@c  IOM_DEV_TYPE_IN_DBG                   -  This device type is utilized for character
                                                                         reception via DBG library.
 *  @li@c  IOM_DEV_TYPE_OUT_IND_POWER     - This is power indicator device type
 *  @li@c  IOM_DEV_TYPE_OUT_FPD                 - This is FP display device type
 *  @li@c  IOM_DEV_TYPE_OUT_IRB                 - This is IR blaster device type
 *  @li@c  IOM_DEV_TYPE_IO_UART                 - This is UART device type
 *  @li@c  IOM_DEV_TYPE_IO_I2C                   - This is I2C device type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IOM_DEV_TYPE_NONE = 0,
    IOM_DEV_TYPE_IN_IRRC,
    IOM_DEV_TYPE_IN_IRRK,
    IOM_DEV_TYPE_IN_IRRM,
    IOM_DEV_TYPE_IN_DBG,
    IOM_DEV_TYPE_OUT_IND_POWER,
    IOM_DEV_TYPE_OUT_FPD,
    IOM_DEV_TYPE_OUT_IRB,
    IOM_DEV_TYPE_IO_UART,
    IOM_DEV_TYPE_IO_I2C
}   IOM_DEV_TYPE_T;

/* Get type */
/*------------------------------------------------------------------*/
/*! @enum IOM_GET_T
 *  @brief  This enumeration defines the get operations
 *  @code
 *  typedef enum
 *  {
 *      IOM_GET_IRRC_EVT_SRC_MASK = 0,
 *      IOM_GET_IRRC_EVT_GRP_MASK,
 *      IOM_GET_IRRC_RPT_EVT_ITVL,
 *      IOM_GET_IRRC_SYS_CODE_FILTER,
 *      IOM_GET_IRRC_PROTOCOL,
 *      IOM_GET_UART_SETTING,
 *      IOM_GET_UART_OPERATION_MODE
 *  #if IOM_SUPPORT_IRRC_DISABLE_KEYS
 *      ,
 *      IOM_GET_IRRC_DISABLE_EVTCODE_CNT,
 *      IOM_GET_IRRC_DISABLE_EVTCODE_INFO
 *  #endif
 *  }   IOM_GET_T;
 *  @endcode
 *  @li@c  IOM_GET_IRRC_EVT_SRC_MASK                -  Gets IR remote controller event source mask
 *  @li@c  IOM_GET_IRRC_EVT_GRP_MASK                - Gets IR remote controller event group mask
 *  @li@c  IOM_GET_IRRC_RPT_EVT_ITVL                  - Gets IR remote controller repeat event interval
 *  @li@c  IOM_GET_IRRC_SYS_CODE_FILTER           - Gets the configuration of system code filter
 *  @li@c  IOM_GET_IRRC_PROTOCOL                       - Gets current used IRRC protocol
 *  @li@c  IOM_GET_UART_SETTING                          - Gets UART configuration
 *  @li@c  IOM_GET_UART_OPERATION_MODE            - Gets current used UART operation mode
 *  @li@c  IOM_GET_IRRC_DISABLE_EVTCODE_CNT   - Gets IR remote controller disable event code count
 *  @li@c  IOM_GET_IRRC_DISABLE_EVTCODE_INFO  - Gets IR remote controller disable event code
 *                                                                               information. Before gets the setting of disable 
 *                                                                               event code information, Please first gets IR
 *                                                                               remote controller disable event code  count to 
 *                                                                               allocate enough memory to store the disable
 *                                                                               event code information.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IOM_GET_IRRC_EVT_SRC_MASK = 0,
    IOM_GET_IRRC_EVT_GRP_MASK,
    IOM_GET_IRRC_RPT_EVT_ITVL,
    IOM_GET_IRRC_SYS_CODE_FILTER,
    IOM_GET_IRRC_PROTOCOL,
    IOM_GET_UART_SETTING,
    IOM_GET_UART_OPERATION_MODE
#if IOM_SUPPORT_IRRC_DISABLE_KEYS
    ,
    IOM_GET_IRRC_DISABLE_EVTCODE_CNT,
    IOM_GET_IRRC_DISABLE_EVTCODE_INFO
#endif
}   IOM_GET_T;

/* Set type */
/*------------------------------------------------------------------*/
/*! @enum IOM_SET_T 
 *  @brief This enumeration defines the set operations
 *  @code
 *  typedef enum
 *  {
 *      IOM_SET_IRRC_EVT_SRC_MASK = 0,
 *      IOM_SET_IRRC_EVT_GRP_MASK,
 *      IOM_SET_IRRC_RPT_EVT_ITVL,
 *      IOM_SET_IRRC_SYS_CODE_FILTER,
 *      IOM_SET_IRRC_PROTOCOL,
 *      IOM_SET_UART_SETTING,
 *      IOM_SET_UART_OPERATION_MODE    
 *  #if IOM_SUPPORT_IRRC_DISABLE_KEYS
 *      ,
 *      IOM_SET_IRRC_DISABLE_EVTCODE_INFO
 *  #endif
 *	  , IOM_SET_UART_MAGIC_CHAR
 *      , IOM_SET_IRRC_LED_CTRL
 *  }   IOM_SET_T;
 *  @endcode
 *  @li@c  IOM_SET_IRRC_EVT_SRC_MASK                - Sets IR remote controller event source mask
 *  @li@c  IOM_SET_IRRC_EVT_GRP_MASK                - Sets IR remote controller event group mask
 *  @li@c  IOM_SET_IRRC_RPT_EVT_ITVL                  - Sets IR remote controller repeat event interval
 *  @li@c  IOM_SET_IRRC_SYS_CODE_FILTER            - Sets the configuration of system code filter
 *  @li@c  IOM_SET_IRRC_PROTOCOL                       - Sets current used IRRC protocol.
 *  @li@c  IOM_SET_UART_SETTING                          - Sets UART configuration
 *  @li@c  IOM_SET_UART_OPERATION_MODE            - Sets current used UART operation mode
 *  @li@c  IOM_SET_IRRC_DISABLE_EVTCODE_INFO  - Sets IR remote controller event code information
 *  @li@c  IOM_SET_UART_MAGIC_CHAR                   - Sets uart factory mode magic char
 *  @li@c  IOM_SET_IRRC_LED_CTRL              - Sets IR LED blink control information
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IOM_SET_IRRC_EVT_SRC_MASK = 0,
    IOM_SET_IRRC_EVT_GRP_MASK,
    IOM_SET_IRRC_RPT_EVT_ITVL,
    IOM_SET_IRRC_SYS_CODE_FILTER,
    IOM_SET_IRRC_PROTOCOL,
    IOM_SET_UART_SETTING,
    IOM_SET_UART_OPERATION_MODE    
#if IOM_SUPPORT_IRRC_DISABLE_KEYS
    ,
    IOM_SET_IRRC_DISABLE_EVTCODE_INFO
#endif
    , IOM_SET_UART_MAGIC_CHAR
    , IOM_SET_IRRC_LED_CTRL
}   IOM_SET_T;

/* IRRC related setup ********************************************************/
/* Definition of IRRC event attribute */
#define IOM_EVT_ATTR_NONE                   ((UINT32) 0)    /**< IOM event attribute is NONE   */
#define IOM_EVT_ATTR_LONG_PRESS             MAKE_BIT_MASK_32(24)  /**<  IOM event attribute is long press  */

/* Repeat event interval structure */
/*------------------------------------------------------------------*/
/*! @struct RPT_EVT_ITVL_T
 *  @brief This structure defines repeat event interval data information.
 *  @code
 *  typedef struct _RPT_EVT_ITVL_T
 *  {
 *      UINT16      ui2_1st;    
 *      UINT16      ui2_other;  
 *  }   RPT_EVT_ITVL_T;
 *  @endcode
 *  @li@c  ui2_1st      -Contains the first repeat event interval information. 
 *                               This value shall be implemented with a resolution as defined 
 *                               in IOM_TIMER_RESOLUTION. IOM_ZERO_RPT_EVT_ITVL is 
 *                               used when the first repeat event is not required
 *  @li@c  ui2_other   - Contains the other repeat event interval information. This value
 *                                shall be implemented with a resolution as defined in
 *                                IOM_TIMER_RESOLUTION. IOM_ZERO_RPT_EVT_ITVL is used
 *                                when the other repeat events are not required
 */
/*------------------------------------------------------------------*/
typedef struct _RPT_EVT_ITVL_T
{
    UINT16      ui2_1st;    /* 10 ms */
    UINT16      ui2_other;  /* 10 ms */
}   RPT_EVT_ITVL_T;

#if IOM_SUPPORT_IRRC_DISABLE_KEYS
/* Disable key structure */
/*------------------------------------------------------------------*/
/*! @struct DISABLE_EVTCODE_T
 *  @brief  This structure defines disable IRRC event code information
 *  @code
 *  typedef struct _DISABLE_EVTCODE_T
 *  {
 *     BOOL         b_enable;           
 *     BOOL         b_positive_filter;  
 *     SIZE_T       z_evtcode_cnt;     
 *     UINT32*      pui4_evtcode_arr;  
 *  }DISABLE_EVTCODE_T;
 *  @endcode
 *  @li@c  b_enable                  - Enables/Disables disable event code feature, 
 *                                              TRUE: Enable; FALSE: disable
 *  @li@c  b_positive_filter        - Indicates a positive or negative filtering 
 *                                              TRUE: Disable the event code in the  pui4_evtcode_arr;   
 *                                              FALSE:  enable the event code in the pui4_evtcode_arr
 *  @li@c  z_evtcode_cnt          - Indicates event code count in pui4_evtcode_arr
 *  @li@c  pui4_evtcode_arr      - Contains the event code data
 */
/*------------------------------------------------------------------*/
typedef struct _DISABLE_EVTCODE_T
{
   BOOL         b_enable;           
   BOOL         b_positive_filter;                                                                
   SIZE_T       z_evtcode_cnt;     
   UINT32*      pui4_evtcode_arr;  
}DISABLE_EVTCODE_T;
#endif
/* Magic unit structure */
/*------------------------------------------------------------------*/
/*! @struct MAGIC_UNIT_T
 *  @brief  This structure defines the magic string information
 *  @code
 *  typedef struct _MAGIC_UNIT_T
 *  {
 *     UINT8*   pui1_magic;  
 *     SIZE_T   z_magic_len; 
 *  }MAGIC_UNIT_T;
 *  @endcode
 *  @li@c  pui1_magic        - Contains the magic string information 
 *  @li@c  z_magic_len      - Indicates the mgic string length in pui1_magic 
 */
/*------------------------------------------------------------------*/
typedef struct _MAGIC_UNIT_T
{
    UINT8*   pui1_magic;
    SIZE_T   z_magic_len;
}MAGIC_UNIT_T;
/*MAGIC  structure */
/*------------------------------------------------------------------*/
/*! @struct MAGIC_T
 *  @brief  This structure defines the magic setting information
 *  @code
 *  typedef struct _MAGIC_T
 *  {
 *     MAGIC_UNIT_T*    pt_magic_unit;         
 *     UINT8                   ui1_magic_units_count;
 *  }MAGIC_T;
 *  @endcode
 *  @li@c  pt_magic_unit          - Contains the magic unit data
 *  @li@c  ui1_magic_units_count      - Indicates magic unit count in pt_magic_unit
 */
/*------------------------------------------------------------------*/
typedef struct _MAGIC_T
{   
    MAGIC_UNIT_T*    pt_magic_unit;
    UINT8            ui1_magic_units_count;
}MAGIC_T;
/* IRRC setting structure */
/*------------------------------------------------------------------*/
/*! @struct IRRC_SETTING_T
 *  @brief This structure defines IRRC setting data information
 *  @code
 *  typedef struct _IRRC_SETTING_T
 *  {
 *      UINT64          ui8_evt_grp_mask;
 *      RPT_EVT_ITVL_T  t_rpt_evt_itvl;
 *      BOOL            b_notify_raw_data;
 *      UINT16          ui2_long_press_itvl; 
 *  }   IRRC_SETTING_T;
 *  @endcode
 *  @li@c  ui8_evt_grp_mask   - Contains the event group mask information
 *  @li@c  t_rpt_evt_itvl           - Contain the data structure as define in RPT_EVT_ITVL_T
 *  @li@c  b_notify_raw_data   - Indicates if raw data notification is required
 *  @li@c  ui2_long_press_itvl  - Contains long-pressed event interval in millisecond
 */
/*------------------------------------------------------------------*/
typedef struct _IRRC_SETTING_T
{
    UINT64          ui8_evt_grp_mask;
    RPT_EVT_ITVL_T  t_rpt_evt_itvl;
    BOOL            b_notify_raw_data;
#ifdef IOM_CUSTOM_KEY_REPEAT_INTERVAL_SUPPORT
    BOOL            b_disable_custom_key_rep_itvl;
#endif
    UINT16          ui2_long_press_itvl; /* 10 ms */
}   IRRC_SETTING_T;

/* Definition of IRRC LED CONTROL */
/*------------------------------------------------------------------*/
/*! @enum IOM_IRRC_LED_CTRL_T
 *  @brief This enumeration defines IOM IRRC LED control.
 *  @code
 *  typedef enum
 *  {
 *      IOM_IRRC_LED_CTRL_OFF = 0,
 *      IOM_IRRC_LED_CTRL_ON
 *  }   IOM_IRRC_LED_CTRL_T;
 *  @endcode
 *  @li@c  IOM_IRRC_LED_CTRL_OFF                     - LED Blink off
 *  @li@c  IOM_IRRC_LED_CTRL_ON                      - LED Blink on
 */
typedef enum
{
    IOM_IRRC_LED_CTRL_OFF = 0,
    IOM_IRRC_LED_CTRL_ON
}   IOM_IRRC_LED_CTRL_T;
#ifdef IOM_CUSTOM_KEY_REPEAT_INTERVAL_SUPPORT
typedef UINT32 IOM_CUST_KEY_REP_ITVL_ITEM_T[3];  /**<  Custom key repeat interval setting. (event code, first interval units, other interval units)  */
#endif

/* Definition of IRRC protocol */
/*------------------------------------------------------------------*/
/*! @enum IOM_IRRC_PROTOCOL_T
 *  @brief This enumeration defines IOM IRRC protocol.
 *  @code
 *  typedef enum
 *  {
 *      IOM_IRRC_PROTOCOL_NEC = 0,
 *      IOM_IRRC_PROTOCOL_RC5,
 *      IOM_IRRC_PROTOCOL_RC6,
 *      IOM_IRRC_PROTOCOL_FUNAI_FACTORY
 *  }   IOM_IRRC_PROTOCOL_T;
 *  @endcode
 *  @li@c  IOM_IRRC_PROTOCOL_NEC                     - IRRC protocol is NEC
 *  @li@c  IOM_IRRC_PROTOCOL_RC5                     - IRRC protocol is RC5
 *  @li@c  IOM_IRRC_PROTOCOL_RC6                     - IRRC protocol is RC6
 *  @li@c  IOM_IRRC_PROTOCOL_FUNAI_FACTORY   - IRRC protocol is FUNAI Factory
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IOM_IRRC_PROTOCOL_NEC = 0,
    IOM_IRRC_PROTOCOL_RC5,
    IOM_IRRC_PROTOCOL_RC6,
    IOM_IRRC_PROTOCOL_FUNAI_FACTORY
}   IOM_IRRC_PROTOCOL_T;

/*
    31---------------28+27---------------0
       irrc protocol       irrc raw data
*/
#define IOM_GET_IRRC_PROTOCOL(_val)         ((IOM_IRRC_PROTOCOL_T)(((UINT32)_val) >> 28)) /**< Get IRRC Protocol  */

/* Definition for raw data to event code conversion */
/*------------------------------------------------------------------*/
/*! @struct IOM_RAW_DATA_TO_EVT_CODE_T
 *  @brief This structure defines raw data to event code information
 *  @code
 *  typedef struct _IOM_RAW_DATA_TO_EVT_CODE_T
 *  {
 *      UINT8       ui1_sys_code; 
 *      UINT8       ui1_cmd_code; 
 *      UINT32      ui4_evt_code; 
 *  }   IOM_RAW_DATA_TO_EVT_CODE_T;
 *  @endcode
 *  @li@c  ui1_sys_code      -Input,  Contains the IRRC system code
 *  @li@c  ui1_cmd_code     -Input,  Contains the IRRC command code
 *  @li@c  ui4_evt_code      -Output, Contain the IRRC event code, If input data is invalid, this field is 
 *                                       set to BTN_INVALID
 */
/*------------------------------------------------------------------*/
typedef struct _IOM_RAW_DATA_TO_EVT_CODE_T
{
    UINT8       ui1_sys_code; /* Input */
    UINT8       ui1_cmd_code; /* Input */
    UINT32      ui4_evt_code; /* Output => If input data is invalid, this field is set to BTN_INVALID */
}   IOM_RAW_DATA_TO_EVT_CODE_T;

/* Definition for raw bits to event code conversion */
/*------------------------------------------------------------------*/
/*! @struct IOM_RAW_BITS_TO_EVT_CODE_T
 *  @brief This structure defines raw bits to event code information
 *  @code
 *  typedef struct _IOM_RAW_BITS_TO_EVT_CODE_T
 *  {
 *      UINT32      ui4_raw_bits; 
 *      UINT32      ui4_evt_code; 
 *  }   IOM_RAW_BITS_TO_EVT_CODE_T;
 *  @endcode
 *  @li@c  ui4_raw_bits      - Input , Contains the IRRC raw bits
 *  @li@c  ui4_evt_code     - Output => If input data is invalid, this field is set to BTN_INVALID,
 *                                      Contain the IRRC event code
 */
/*------------------------------------------------------------------*/
typedef struct _IOM_RAW_BITS_TO_EVT_CODE_T
{
    UINT32      ui4_raw_bits; 
    UINT32      ui4_evt_code;
}   IOM_RAW_BITS_TO_EVT_CODE_T;

/* System code filter */
/*------------------------------------------------------------------*/
/*! @struct IOM_SYS_CODE_FILTER_T
 *  @brief This structure defines system code filter information
 *  @code
 *  typedef struct _IOM_SYS_CODE_FILTER_T
 *  {
 *      BOOL        b_enable;
 *      BOOL        b_positive_filter;
 *      SIZE_T      z_filter_arr_size;
 *      UINT32*     pui4_filter_arr;
 *  }   IOM_SYS_CODE_FILTER_T;
 *  @endcode
 *  @li@c  b_enable              - Enables/disables system code filter
 *  @li@c  b_positive_filter    - Indicates a positive or negative filtering
 *  @li@c  z_filter_arr_size    - Contain the number of system code
 *  @li@c  pui4_filter_arr       - Contain the system code data
 */
/*------------------------------------------------------------------*/
typedef struct _IOM_SYS_CODE_FILTER_T
{
    BOOL        b_enable;
    BOOL        b_positive_filter;
    SIZE_T      z_filter_arr_size;
    UINT32*     pui4_filter_arr;
}   IOM_SYS_CODE_FILTER_T;

/* IRRC data structure */
/*------------------------------------------------------------------*/
/*! @struct  IRRC_DATA_T
 *  @brief  This structure defines IRRC data information
 *  @code
 *  typedef struct _IRRC_DATA_T
 *  {
 *      IOM_IRRC_PROTOCOL_T     e_protocol;
 *      SIZE_T                               z_data_len; 
 *      UINT8                                 aui1_data[IOM_MAX_IRRC_DATA_SIZE];        
 *  }   IRRC_DATA_T;
 *  @endcode
 *  @li@c  e_protocol                                              - Contains the IRRC protocol as 
 *                                                                            defined in IOM_IRRC_PROTOCOL_T
 *  @li@c  z_data_len                                             -  Contains the IRRC data length
 *  @li@c  aui1_data[IOM_MAX_IRRC_DATA_SIZE]   - Contain the IRRC data. The maximum 
 *                                                                            size is defined in IOM_MAX_IRRC_DATA_SIZE
 */
/*------------------------------------------------------------------*/
typedef struct _IRRC_DATA_T
{
    IOM_IRRC_PROTOCOL_T     e_protocol;
    SIZE_T                  z_data_len; /* byte */
    UINT8                   aui1_data[IOM_MAX_IRRC_DATA_SIZE];        
}   IRRC_DATA_T;

/* Debug data related setup **************************************************/
/* Debug data structure */
/*------------------------------------------------------------------*/
/*! @struct DBG_DATA_T  
 *  @brief  This structure defines debug data information
 *  @code
 *  typedef struct _DBG_DATA_T
 *  {
 *      UINT32      ui4_data_len;
 *      UINT8       aui1_data[IOM_MAX_DBG_DATA_SIZE];
 *  }   DBG_DATA_T;
 *  @endcode
 *  @li@c  ui4_data_len                                          - Contains the debug data length
 *  @li@c  aui1_data[IOM_MAX_DBG_DATA_SIZE]   - Contain the debug data. The maximum
 *                                                                          size is defined in IOM_MAX_DBG_DATA_SIZE
 */
/*------------------------------------------------------------------*/
typedef struct _DBG_DATA_T
{
    UINT32      ui4_data_len;
    UINT8       aui1_data[IOM_MAX_DBG_DATA_SIZE];
}   DBG_DATA_T;

/* IND related setup *********************************************************/
/* Definition of indicator state */
/*------------------------------------------------------------------*/
/*! @enum IOM_IND_STATE_T
 *  @brief This enumeration defines the state of indicator driver
 *  @code 
 *  typedef enum
 *  {
 *      IOM_IND_STATE_ENABLE_STEADY = 0,
 *      IOM_IND_STATE_ENABLE_BLINK,
 *      IOM_IND_STATE_DISABLE
 *  }   IOM_IND_STATE_T;
 *  @endcode
 *  @li@c  IOM_IND_STATE_ENABLE_STEADY  - The Indicator driver is enabled and shall
 *                                                                    present a steady indication
 *  @li@c  IOM_IND_STATE_ENABLE_BLINK     - The Indicator driver is enabled and shall 
 *                                                                    present a blinking presentation
 *  @li@c  IOM_IND_STATE_DISABLE              - The Indicator driver is disabled
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IOM_IND_STATE_ENABLE_STEADY = 0,
    IOM_IND_STATE_ENABLE_BLINK,
    IOM_IND_STATE_DISABLE
}   IOM_IND_STATE_T;

/* Definition of indicator color */
/*------------------------------------------------------------------*/
/*! @enum  IOM_IND_COLOR_T
 *  @brief  This enumeration defines the color of indicator driver
 *  @code
 *  typedef enum
 *  {
 *      IOM_IND_COLOR_SINGLE = 0,
 *      IOM_IND_COLOR_RED,
 *      IOM_IND_COLOR_GREEN,
 *      IOM_IND_COLOR_YELLOW,
 *      IOM_IND_COLOR_BLUE,
 *      IOM_IND_COLOR_AMBER,
 *      IOM_IND_COLOR_WHITE
 *  }   IOM_IND_COLOR_T;
 *  @endcode
 *  @li@c  IOM_IND_COLOR_SINGLE   - The Indicator driver cannot present multiple colors
 *  @li@c  IOM_IND_COLOR_RED        - The Indicator driver can present multiple colors and
 *                                                        is set to show the color red.
 *  @li@c  IOM_IND_COLOR_GREEN    - The Indicator driver can present multiple colors and
 *                                                        is set to show the color green
 *  @li@c  IOM_IND_COLOR_YELLOW  - The Indicator driver can present multiple colors 
 *                                                        and is set to show the color yellow
 *  @li@c  IOM_IND_COLOR_BLUE       - The Indicator driver can present multiple colors and
 *                                                        is set to show the color blue
 *  @li@c  IOM_IND_COLOR_AMBER    - The Indicator driver can present multiple colors and 
 *                                                        is set to show the color amber
 *  @li@c  IOM_IND_COLOR_WHITE    - The Indicator driver can present multiple colors and
 *                                                        is set to show the color white
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IOM_IND_COLOR_SINGLE = 0,
    IOM_IND_COLOR_RED,
    IOM_IND_COLOR_GREEN,
    IOM_IND_COLOR_YELLOW,
    IOM_IND_COLOR_BLUE,
    IOM_IND_COLOR_AMBER,
    IOM_IND_COLOR_WHITE
}   IOM_IND_COLOR_T;

/* IND event info */
/*------------------------------------------------------------------*/
/*! @struct IOM_IND_T
 *  @brief This structure defines indicator information
 *  @code
 *  typedef struct _IOM_IND_T
 *  {
 *      IOM_IND_STATE_T     e_ind_state;
 *      IOM_IND_COLOR_T    e_ind_color;
 *  }   IOM_IND_T;
 *  @endcode
 *  @li@c  e_ind_state    - Contains indicator state as defined in IOM_IND_STATE_T
 *  @li@c  e_ind_color    - Contains indicator color as defined in IOM_IND_COLOR_T
 */
/*------------------------------------------------------------------*/
typedef struct _IOM_IND_T
{
    IOM_IND_STATE_T     e_ind_state;
    IOM_IND_COLOR_T     e_ind_color;
}   IOM_IND_T;

/* FPD related setup *********************************************************/
/* FPD event info */
/*------------------------------------------------------------------*/
/*! @struct  IOM_FPD_T
 *  @brief This structure defines FP display information
 *  @code
 *  typedef struct _IOM_FPD_T
 *  {
 *      UINT16      ui2_line_num;
 *      SIZE_T      z_offset;
 *      SIZE_T      z_len;
 *      CHAR*       ps_content;
 *  }   IOM_FPD_T;
 *  @endcode
 *  @li@c  ui2_line_num  - Contains FP display's line number
 *  @li@c  z_offset          - Contains the character position at which the content 
 *                                    shall be set. The first character of a line is always
 *                                    referenced with a value of '0'
 *  @li@c  z_len             - Contain the number of characters which shall be presented
 *  @li@c  ps_content     - References a zero terminated string for display
 */
/*------------------------------------------------------------------*/
typedef struct _IOM_FPD_T
{
    UINT16      ui2_line_num;
    SIZE_T      z_offset;
    SIZE_T      z_len;
    CHAR*       ps_content;
}   IOM_FPD_T;

/* UART related setup ********************************************************/
/* UART data structure */
/*------------------------------------------------------------------*/
/*! @struct IOM_UART_T
 *  @brief This structure defines UART data packet information
 *  @code
 *  typedef struct _IOM_UART_T
 *  {
 *      SIZE_T      z_len;
 *      UINT8*      pui1_data;
 *  }   IOM_UART_T;
 *  @endcode
 *  @li@c  z_len          - Contains the data packet length
 *  @li@c  pui1_data    - Points to the data packet
 */
/*------------------------------------------------------------------*/
typedef struct _IOM_UART_T
{
    SIZE_T      z_len;
    UINT8*      pui1_data;
}   IOM_UART_T;

/* Definition of UART data speed */
/*------------------------------------------------------------------*/
/*! @enum  IOM_UART_SPEED_T
 *  @brief This enumeration contains the speed with which the UART driver
 *             communicates with the external device
 *  @code
 *  typedef enum
 *  {
 *      IOM_UART_SPEED_75 = 0,
 *      IOM_UART_SPEED_110,
 *      IOM_UART_SPEED_134,
 *      IOM_UART_SPEED_150,
 *      IOM_UART_SPEED_300,
 *      IOM_UART_SPEED_600,
 *      IOM_UART_SPEED_1200,
 *      IOM_UART_SPEED_1800,
 *      IOM_UART_SPEED_2400,
 *      IOM_UART_SPEED_4800,
 *      IOM_UART_SPEED_9600,
 *      IOM_UART_SPEED_14400,
 *      IOM_UART_SPEED_19200,
 *      IOM_UART_SPEED_38400,
 *      IOM_UART_SPEED_57600,
 *      IOM_UART_SPEED_115200,
 *      IOM_UART_SPEED_128000,
 *      IOM_UART_SPEED_230400,
 *      IOM_UART_SPEED_460800,
 *      IOM_UART_SPEED_921600
 *  }   IOM_UART_SPEED_T;
 *  @endcode
 *  @li@c  IOM_UART_SPEED_*              - Indicates that the speed with which the UART
 *                                                           driver can receive and transmit data. The
 *                                                           trailing number of the definition indicates the
 *                                                           speed in bits per second
 *  @li@c  IOM_UART_SPEED_75            -
 *  @li@c  IOM_UART_SPEED_110          -
 *  @li@c  IOM_UART_SPEED_134          -
 *  @li@c  IOM_UART_SPEED_150          -
 *  @li@c  IOM_UART_SPEED_300          -
 *  @li@c  IOM_UART_SPEED_600          -
 *  @li@c  IOM_UART_SPEED_1200        -
 *  @li@c  IOM_UART_SPEED_1800        -
 *  @li@c  IOM_UART_SPEED_2400        -
 *  @li@c  IOM_UART_SPEED_4800        -
 *  @li@c  IOM_UART_SPEED_9600        -
 *  @li@c  IOM_UART_SPEED_14400      -
 *  @li@c  IOM_UART_SPEED_19200      -
 *  @li@c  IOM_UART_SPEED_38400      -
 *  @li@c  IOM_UART_SPEED_57600      -
 *  @li@c  IOM_UART_SPEED_115200    -
 *  @li@c  IOM_UART_SPEED_128000    -
 *  @li@c  IOM_UART_SPEED_230400    -
 *  @li@c  IOM_UART_SPEED_460800    -
 *  @li@c  IOM_UART_SPEED_921600    -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IOM_UART_SPEED_75 = 0,
    IOM_UART_SPEED_110,
    IOM_UART_SPEED_134,
    IOM_UART_SPEED_150,
    IOM_UART_SPEED_300,
    IOM_UART_SPEED_600,
    IOM_UART_SPEED_1200,
    IOM_UART_SPEED_1800,
    IOM_UART_SPEED_2400,
    IOM_UART_SPEED_4800,
    IOM_UART_SPEED_9600,
    IOM_UART_SPEED_14400,
    IOM_UART_SPEED_19200,
    IOM_UART_SPEED_38400,
    IOM_UART_SPEED_57600,
    IOM_UART_SPEED_115200,
    IOM_UART_SPEED_128000,
    IOM_UART_SPEED_230400,
    IOM_UART_SPEED_460800,
    IOM_UART_SPEED_921600
}   IOM_UART_SPEED_T;

/* Definition of UART data length */
/*------------------------------------------------------------------*/
/*! @enum IOM_UART_DATA_LEN_T
 *  @brief This enumeration contains the length in number of bits of a received or 
 *             transmitted data packet
 *  @code
 *  typedef enum
 *  {
 *      IOM_UART_DATA_LEN_4 = 0,
 *      IOM_UART_DATA_LEN_5,
 *      IOM_UART_DATA_LEN_6,
 *      IOM_UART_DATA_LEN_7,
 *      IOM_UART_DATA_LEN_8,
 *      IOM_UART_DATA_LEN_9
 *  }   IOM_UART_DATA_LEN_T;
 *  @endcode
 *  @li@c   IOM_UART_DATA_LEN_*  - Indicates that the length in number of bits of 
 *                                                     a packet received or transmitted. The trailing 
 *                                                     number of the definition indicates the number 
 *                                                     of bits per packet.
 *  @li@c  IOM_UART_DATA_LEN_4   -
 *  @li@c  IOM_UART_DATA_LEN_5   -
 *  @li@c  IOM_UART_DATA_LEN_6   -
 *  @li@c  IOM_UART_DATA_LEN_7   -
 *  @li@c  IOM_UART_DATA_LEN_8   -
 *  @li@c  IOM_UART_DATA_LEN_9   -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IOM_UART_DATA_LEN_4 = 0,
    IOM_UART_DATA_LEN_5,
    IOM_UART_DATA_LEN_6,
    IOM_UART_DATA_LEN_7,
    IOM_UART_DATA_LEN_8,
    IOM_UART_DATA_LEN_9
}   IOM_UART_DATA_LEN_T;

/* Definition of UART parity */
/*------------------------------------------------------------------*/
/*! @enum IOM_UART_PARITY_T
 *  @brief This enumeration contains the parity of a packet received or transmitted
 *  @code
 *  typedef enum
 *  {
 *      IOM_UART_PARITY_NONE = 0,
 *      IOM_UART_PARITY_EVEN,
 *      IOM_UART_PARITY_ODD,
 *      IOM_UART_PARITY_MARK,
 *      IOM_UART_PARITY_SPACE
 *  }   IOM_UART_PARITY_T;
 *  @endcode
 *  @li@c  IOM_UART_PARITY_NONE     - Indicates that the received or transmitted 
 *                                                         packet does not contain a parity bit
 *  @li@c  IOM_UART_PARITY_EVEN     - Indicates that the received or transmitted 
 *                                                         packet does contain an even parity bit
 *  @li@c  IOM_UART_PARITY_ODD      - Indicates that the received or transmitted 
 *                                                         packet does contain an odd parity bit
 *  @li@c  IOM_UART_PARITY_MARK    - Indicates that the received or transmitted
 *                                                         packet does contain a mark parity bit
 *  @li@c  IOM_UART_PARITY_SPACE   - Indicates that the received or transmitted 
 *                                                         packet does contain a space parity bit
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IOM_UART_PARITY_NONE = 0,
    IOM_UART_PARITY_EVEN,
    IOM_UART_PARITY_ODD,
    IOM_UART_PARITY_MARK,
    IOM_UART_PARITY_SPACE
}   IOM_UART_PARITY_T;

/* Definition of UART stop bit */
/*------------------------------------------------------------------*/
/*! @enum IOM_UART_STOP_BIT_T
 *  @brief This enumeration contains the number of stop bits inserted at the end of a 
 *            received or transmitted packet
 *  @code
 *  typedef enum
 *  {
 *      IOM_UART_STOP_BIT_1 = 0,
 *      IOM_UART_STOP_BIT_1_5,
 *      IOM_UART_STOP_BIT_2
 *  }   IOM_UART_STOP_BIT_T;
 *  @endcode
 *  @li@c  IOM_UART_STOP_BIT_*       - Indicates that the number of stop bits inserted
 *                                                         at the end of a received or transmitted packet. 
 *                                                         The trailing number of the definition indicates
 *                                                          the number of stop bits
 *  @li@c  IOM_UART_STOP_BIT_1       - 
 *  @li@c  IOM_UART_STOP_BIT_1_5    -
 *  @li@c  IOM_UART_STOP_BIT_2       -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IOM_UART_STOP_BIT_1 = 0,
    IOM_UART_STOP_BIT_1_5,
    IOM_UART_STOP_BIT_2
}   IOM_UART_STOP_BIT_T;


/* UART setting structure */
/*------------------------------------------------------------------*/
/*! @struct UART_SETTING_T
 *  @brief  This structure defines UART setting data information
 *  @code
 *  typedef struct _UART_SETTING_T
 *  {
 *      IOM_UART_SPEED_T       e_speed;
 *      IOM_UART_DATA_LEN_T  e_data_len;
 *      IOM_UART_PARITY_T      e_parity;
 *      IOM_UART_STOP_BIT_T  e_stop_bit;
 *  }   UART_SETTING_T;
 *  @endcode
 *  @li@c  e_speed      - Contains the UART speed as defined in IOM_UART_SPEED_T
 *  @li@c  e_data_len  - Contains the UART packet length as defined in IOM_UART_DATA_LEN_T
 *  @li@c  e_parity      - Contains the UART packet parity as defined in IOM_UART_PARITY_T
 *  @li@c  e_stop_bit   - Contains the UART packet stop bit as defined in IOM_UART_STOP_BIT_T
 */
/*------------------------------------------------------------------*/
typedef struct _UART_SETTING_T
{
    IOM_UART_SPEED_T        e_speed;
    IOM_UART_DATA_LEN_T     e_data_len;
    IOM_UART_PARITY_T       e_parity;
    IOM_UART_STOP_BIT_T     e_stop_bit;
}   UART_SETTING_T;

/* UART operation mode */
#define IOM_UART_OPERATION_MODE_NORMAL          MAKE_BIT_MASK_32(0) /**< URAT normal operation  mode */
#define IOM_UART_OPERATION_MODE_TRANSPARENT     MAKE_BIT_MASK_32(1) /**< URAT tranparent operation mode */
#define IOM_UART_OPERATION_MODE_CLI             MAKE_BIT_MASK_32(2) /**< URAT CLI operantion mode */
#define IOM_UART_OPERATION_MODE_FACTORY         MAKE_BIT_MASK_32(3) /**< URAT factory operation mode   */

/* Notify function definition ************************************************/
/* Definition of notify condition */
/*------------------------------------------------------------------*/
/*! @enum IOM_NFY_COND_T
 *  @brief This enumeration contains the IO Manager notify values which are carried
 *             in argument e_nfy_cond of API (*x_iom_nfy_fct). 
 *  @code
 *  typedef enum
 *  {
 *      IOM_NFY_COND_NONE = 0,
 *      IOM_NFY_COND_BTN_DOWN,
 *      IOM_NFY_COND_BTN_UP,
 *      IOM_NFY_COND_BTN_REPEAT,
 *      IOM_NFY_COND_REC_DBG_DATA,
 *      IOM_NFY_COND_REC_UART_DATA,
 *      IOM_NFY_COND_XMT_COMPLETE,
 *      IOM_NFY_COND_AUTO_SLEEP_TIMER_RESET,
 *      IOM_NFY_COND_AUTO_SLEEP_PROMPT,
 *      IOM_NFY_COND_AUTO_SLEEP_TIMEOUT
 *  }   IOM_NFY_COND_T;
 *  @endcode
 *  @li@c  IOM_NFY_COND_NONE                                -
 *  @li@c  IOM_NFY_COND_BTN_DOWN                       - The IO Manager has received a new
 *                                                                                event that indicates a button is pressed 
 *                                                                                down
 *  @li@c  IOM_NFY_COND_BTN_UP                             - The IO Manager has received a new event
 *                                                                                 that indicates a button is released
 *  @li@c  IOM_NFY_COND_BTN_REPEAT                      - This notification is not caused by a physical 
 *                                                                                button position change such as a button is 
 *                                                                                just pressed down but generated by the IO
 *                                                                                Manager when a button that is continuously 
 *                                                                                held down after an IOM_NFY_COND_BTN_DOWN
 *                                                                                notification is issued. The IO manager will keep 
 *                                                                                notifying applications with the condition before
 *                                                                                IOM_NFY_COND_BTN_UP is received. The
 *                                                                                repeat event interval can be configured through
 *                                                                                x_iom_open and x_iom_set
 *  @li@c  IOM_NFY_COND_REC_DBG_DATA                - The IO Manager has received a character string 
 *                                                                                input from DBG library
 *  @li@c  IOM_NFY_COND_REC_UART_DATA               - The IO Manager has received UART data
 *  @li@c  IOM_NFY_COND_XMT_COMPLETE                  - The UART transmission has been complete
 *  @li@c  IOM_NFY_COND_AUTO_SLEEP_TIMER_RESET -Auto sleep prompt timer is reset
 *  @li@c  IOM_NFY_COND_AUTO_SLEEP_PROMPT         - Auto sleep prompt timer is expired
 *  @li@c  IOM_NFY_COND_AUTO_SLEEP_TIMEOUT        - Auto sleep timer is expired
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IOM_NFY_COND_NONE = 0,
    IOM_NFY_COND_BTN_DOWN,
    IOM_NFY_COND_BTN_UP,
    IOM_NFY_COND_BTN_REPEAT,
    IOM_NFY_COND_REC_DBG_DATA,
    IOM_NFY_COND_REC_UART_DATA,
    IOM_NFY_COND_XMT_COMPLETE,
    IOM_NFY_COND_AUTO_SLEEP_TIMER_RESET,
    IOM_NFY_COND_AUTO_SLEEP_PROMPT,
    IOM_NFY_COND_AUTO_SLEEP_TIMEOUT
}   IOM_NFY_COND_T;

/* Notify function */
/*------------------------------------------------------------------*/
/*! @brief  This notify function is called by the IO Manager to notifies the 
 *                applications of the receipt ion of events. The interpretation of
 *                the argument ui4_data depends on the notify condition specified
 *                in argument e_nfy_cond
 *
 *  @param [in]  pv_nfy_tag    -Contains a tag value, which was obtained when
 *                                            executing API x_iom_open. 
 *  @param [in]  e_nfy_cond   - Contains the notify condition as defined in
 *                                            enumeration IOM_NFY_COND_T. 
 *  @param [in]  ui4_evt_code - Contains the event code as defined in Event code table. 
 *  @param [in]  ui4_data        - Contains additional information associated to the event. 
 *                                             It may be set to 0 if no additional information is available. 
 *
 *  @param [out] None
 *
 *  @note [INP_EVT_TYPE_IRRC] If IOM_EXTENDABLE_RAW_DATA_SUPPORT is defined, 
 *                                                 ui4_data = the address of IRRC_DATA_T,
 *                                              If IOM_EXTENDABLE_RAW_DATA_SUPPORT is not defined, 
 *                                                 ui4_data = 32-bit raw data
 *  @see
 *
 *  @return None.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_iom_nfy_fct)(VOID*          pv_nfy_tag,
                              IOM_NFY_COND_T e_nfy_cond,
                              UINT32         ui4_evt_code,
                              UINT32         ui4_data); 
/* Auto sleep timer related setup ********************************************/
/* Auto sleep callback function */
/*------------------------------------------------------------------*/
/*! @brief This notify function is called by the IO Manager to notifies the applications 
 *              of the receipt ion of events. The interpretation of the argument ui4_data 
 *              depends on the notify condition specified in argument e_nfy_cond 
 *
 *  @param [in] e_nfy_cond  - Contains the notify condition as defined in enumeration 
 *                                          IOM_NFY_COND_T. 
 *  @param [in] pv_nfy_tag  - Contains a tag value, which was obtained when executing 
 *                                          API x_iom_set_auto_sleep_timer_ex.
 *
 *  @param [out] None.
 *
 *  @note
 *  @see
 *
 *  @return None.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_iom_auto_sleep_cb)(IOM_NFY_COND_T e_nfy_cond,  
                                    VOID*          pv_nfy_tag);

/* Auto sleep timer info */
/*------------------------------------------------------------------*/
/*! @struct AUTO_SLEEP_TIMER_T
 *  @brief This structure defines auto sleep timer information
 *  @code
 *  typedef struct _AUTO_SLEEP_TIMER_T
 *  {
 *      UINT32                        ui4_prompt_timeout;
 *      UINT32                        ui4_sleep_timeout;
 *      x_iom_auto_sleep_cb   pf_nfy; 
 *      VOID*                         pv_nfy_tag;
 *  }   AUTO_SLEEP_TIMER_T;
 *  @endcode
 *  @li@c  ui4_prompt_timeout      - Contains the prompt timeout
 *  @li@c  ui4_sleep_timeout         - Contains the sleep timeout
 *  @li@c  pf_nfy                          - References a notify function 
 *  @li@c  pv_nfy_tag                   - Contains a tag value from the client
 */
/*------------------------------------------------------------------*/
typedef struct _AUTO_SLEEP_TIMER_T
{
    UINT32              ui4_prompt_timeout;
    UINT32              ui4_sleep_timeout;
    x_iom_auto_sleep_cb pf_nfy; 
    VOID*               pv_nfy_tag;
}   AUTO_SLEEP_TIMER_T;
                                                           
#endif /* _U_IOM_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                           /* end of groupMW_IO_MNGR */
/*----------------------------------------------------------------------------*/

/*! @page pageMW_IO_MNGR IO Manager Detailed Description
 *  @par
 *  @section secMW_IO_MNGR_1  1. Introduction
 *  @par
 *      As the module name says, the IO Manager handles
 *      -   events from the input drivers / Middleware components to the applications and
 *      -   events from the applications to the output drivers.
 *      It is designed as a message manager and dispatcher under MediaTek's Middleware.
 *  
 *  @par
 *  @section secMW_IO_MNGR_2  2. Concept
 *  @par
 *      Input devices
 *      -   Serial port
 *      -   IR remote controller
 *      -   IR keyboard
 *      -   IR mouse
 *      -   etc
 *  
 *  @par
 *      Output devices
 *      -   IR blaster
 *      -   Front panel display
 *      -   LED indicator
 *      -   etc
 *  
 *  @par
 *      Symbols and abbreviations
 *      <table>
 *      <tr>
 *      <td>FIFO</td><td>
 *      <b>F</b>irst <b>I</b>n <b>F</b>irst <b>O</b>ut
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>FP</td><td>
 *      <b>F</b>ront <b>P</b>anel
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>IND</td><td>
 *      <b>I</b>ndicator
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>IR</td><td>
 *      <b>I</b>nfra<b>R</b>ed
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>LED</td><td>
 *      <b>L</b>ight <b>E</b>mitting <b>D</b>iode
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>RC</td><td>
 *      <b>R</b>emote <b>C</b>ontroller
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>UART</td><td>
 *      <b>U</b>niversal <b>A</b>synchronous <b>R</b>eceiver-<b>T</b>ransmitter
 *      </td>
 *      </tr>
 *      </table>
 *  @par
 *  @section secMW_IO_MNGR_3  3. Brief Design
 *  @par
 *      IO manager consists of three parts, database, input dispatcher, and output
 *      dispatcher, as is shown in the following figure which also describes
 *      relationship between IO manager and other components.
 *  @par
 *      @image html iom_relation.gif "Figure 1: Relationship between IO Manager and other components"
 *  @par
 *  @subsection ssecMW_IO_MNGR_3_1  3.1. Database
 *  @par
 *      IO manager holds an array of device info data objects describing input, output
 *      and input/output devices; each object then contains a link list of event
 *      filter data objects, which will be used for input event dispatch. An
 *      application must open at least one event filter in order to receive events
 *      coming from the input drivers / Middleware component.
 *  @par
 *      @image html iom_database.gif "Figure 2: Database"
 *  @par
 *  @subsection ssecMW_IO_MNGR_3_2  3.2. Input dispatcher
 *  @par
 *      The input dispatcher establishes input event flow from the drivers to the
 *      applications, performing the following tasks:
 *      -   open input drivers via the Resource Manager or open Middleware components
 *      via their APIs per application request
 *      -   receive events sent from the input drivers via the Resource Manager or the
 *      Middleware components
 *      -   dispatch events to the applications that have the right of receiving such
 *      events (by checking event group mask)
 *  
 *  @par
 *      @image html iom_inpdptr.gif "Figure 3: Input Event Flow"
 *  @par
 *      Note that
 *      -#  driver needs to map its physical event code to Middleware's event code when
 *      notifying the IO Manager of an event
 *      -#  There is no priority of processing input events. It is based on the sequence
 *      when entering the input dispatcher
 *  
 *  @par
 *  @subsection ssecMW_IO_MNGR_3_3  3.3. Output dispatcher
 *  @par
 *      The output dispatcher has the following responsibilities.
 *      -   Open output drivers via the Resource Manager as the applications require
 *      -   Receive events sent from the applications
 *      -   Dispatch events to the output drivers
 *  
 *  @par
 *      Unlike the input dispatcher, the output dispatcher does not require to invoke
 *      the database for event processing. Based on FIFO rule, the events will simply
 *      be processed and transmitted to the responsible output driver directly.
 *  @par
 *      @image html iom_opdptr.gif "Figure 4: Output Event Flow"
 *  @par
 *  @subsection ssecMW_IO_MNGR_3_4  3.4. Repeat event intervals
 *  @par
 *      When a button of IRRC is pressed and held, signals will be transmitted to the
 *      IR driver periodically (110 ms for NEC, 114 for RC-5, etc). However in current
 *      design, only button-down and button-up events will be notified to the IO
 *      Manager. Button-repeat events are filter out by the IR driver, and the ones
 *      sent to application are generated by IO manager by starting a timer after
 *      button-down is received, which will be stopped on button-up. The timeout of
 *      button-repeat timer is controled by <b>ui2_1st</b> and <b>ui2_other</b> in
 *      <b>t_rpt_evt_itvl</b> when opening IOM IRRC device, or by setting
 *      @ref IOM_SET_IRRC_RPT_EVT_ITVL .
 *  @par
 *      @image html iom_repeat.gif "Figure 5: Repeat Event Intervals"
 *  @par
 *  @subsection ssecMW_IO_MNGR_3_5  3.5. Event Code Table
 *  @par
 *      An event code table is defined in this section and the drivers are responsible
 *      to map or translate its specific physical key code to the event code defined
 *      in this table. In case a mismatch is identified, either an event code
 *      expansion or a set of user defined event codes should be provided by MediaTek
 *      Inc. or the system manufacturer respectively.
 *  @par
 *      Event code is defined through a 32-bit UINT32 data type divided into two
 *      fields as specified in Table 1:
 *  @par
 *      <table align = "center">
 *      <tr>
 *      <th>Bit</th><th>
 *      Description
 *      </th>
 *      </tr>
 *      <tr>
 *      <td>31:16</td><td>
 *      Event Group Field
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>15:0</td><td>
 *      Event ID Field
 *      </td>
 *      </tr>
 *      </table>
 *      <center><b>Table 1: Event code fields</b></center>
 *  @par
 *      The following table defines an event code table that should be used together
 *      with current specification. It is important to note that this table is not
 *      meant to list all possible events for the drivers but to cover most popular
 *      events used in DTV applications. Further expansion and extension are possible
 *      and should be done through MediaTek Inc.
 *  @par
 *      <table width="80%" align="center">
 *      <tr>
 *      <th width="20%">
 *      Event Group
 *      </th>
 *      <th width="80%" colspan="3">
 *      Event ID
 *      </th>
 *      </tr>
 *      <tr>
 *      <td align="center" rowspan="14">
 *      0x0001    <br>(Digit)
 *      </td>
 *      <td>ID</td><td>
 *      Description
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0x0030</td><td>
 *      Number 0
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0x0031</td><td>
 *      Number 1
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0x0032</td><td>
 *      Number 2
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0x0033</td><td>
 *      Number 3
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0x0034</td><td>
 *      Number 4
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0x0035</td><td>
 *      Number 5
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0x0036</td><td>
 *      Number 6
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0x0037</td><td>
 *      Number 7
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0x0038</td><td>
 *      Number 8
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0x0039</td><td>
 *      Number 9
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF000</td><td>
 *      Plus 100
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF001</td><td>
 *      Dot
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>&nbsp;</td><td>
 *      &nbsp;
 *      </td>
 *      </tr>
 *      <tr>
 *      <td align="center" rowspan="2">
 *      0x0002    <br>(Alpha)
 *      </td>
 *      <td>ID</td><td>
 *      Description
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>&nbsp;</td><td>
 *      &nbsp;
 *      </td>
 *      </tr>
 *      <tr>
 *      <td align="center" rowspan="6">
 *      0x0003        <br>(Cursor)
 *      </td>
 *      <td>ID</td><td>
 *      Description
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF000</td><td>
 *      Cursor Step Left
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF001</td><td>
 *      Cursor Step Right
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF002</td><td>
 *      Cursor Step Up
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF003</td><td>
 *      Cursor Step Down
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>&nbsp;</td><td>
 *      &nbsp;
 *      </td>
 *      </tr>
 *      <tr>
 *      <td align="center" rowspan="4">
 *      0x0004       <br>(Selection control)
 *      </td>
 *      <td>ID</td><td>
 *      Description
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF100</td><td>
 *      Select / OK
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF101</td><td>
 *      Exit
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>&nbsp;</td><td>
 *      &nbsp;
 *      </td>
 *      </tr>
 *      <tr>
 *      <td align="center" rowspan="14">
 *      0x0005    <br>(Program control)
 *      </td>
 *      <td>ID</td><td>
 *      Description
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF000</td><td>
 *      Channel Up
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF001</td><td>
 *      Channel Down
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF002</td><td>
 *      Previous Channel
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF003</td><td>
 *      POP
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF004</td><td>
 *      Zoom
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF005</td><td>
 *      Swap
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF006</td><td>
 *      Favorite
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF007</td><td>
 *      Favorite Channel
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF008</td><td>
 *      Channel Info
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF009</td><td>
 *      Add/Erase Channel
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF00A</td><td>
 *      Closed Caption
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF00B</td><td>
 *      Input Source
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>&nbsp;</td><td>
 *      &nbsp;
 *      </td>
 *      </tr>
 *      <tr>
 *      <td align="center" rowspan="6">
 *      0x0006      <br>(Audio control)
 *      </td>
 *      <td>ID</td><td>
 *      Description
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF000</td><td>
 *      Volume Up
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF001</td><td>
 *      Volume Down
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF002</td><td>
 *      Mute/Demute
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF003</td><td>
 *      MTS
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>&nbsp;</td><td>
 *      &nbsp;
 *      </td>
 *      </tr>
 *      <tr>
 *      <td align="center" rowspan="6">
 *      0x0007     <br>(Function control)
 *      </td>
 *      <td>ID</td><td>
 *      Description
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF000</td><td>
 *      Power
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF001</td><td>
 *      Menu
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF002</td><td>
 *      EPG
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF003</td><td>
 *      Memory Card
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>&nbsp;</td><td>
 *      &nbsp;
 *      </td>
 *      </tr>
 *      <tr>
 *      <td align="center" rowspan="6">
 *      0x0008     <br>(User define)
 *      </td>
 *      <td>ID</td><td>
 *      Description
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF000</td><td>
 *      Red
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF001</td><td>
 *      Green
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF002</td><td>
 *      Yellow
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>0xF003</td><td>
 *      Blue
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>&nbsp;</td><td>
 *      &nbsp;
 *      </td>
 *      </tr>
 *      </table>
 *      <center><b>Table 2: Event code table</b></center>
 *  @par
 *      The definitions of event code could be found in @ref u_irrc_btn_def.h .
 *  @par
 *  @section secMW_IO_MNGR_4  4. Typical Case
 *  @par
 *      @dontinclude iomsample.c
 *  @subsection ssecMW_IO_MNGR_4_1  4.1. Open IRRC device
 *      @skip open IRRC
 *      @skip {
 *      @until }
 *  @par
 *  @subsection ssecMW_IO_MNGR_4_2  4.2. Open UART device
 *      @skip open UART
 *      @skip {
 *      @until }
 *  @par
 *      &nbsp;
 *      @par Note
 *      <b>ui2_uart_id</b> is the ID of the UART port. There would be two physical
 *      ports (0 and 1) for use.  Currently, the TV system uses the port (0) for
 *      debug. If you want to use the port (0), you may need to set the uart_id to 7;
 *      it's a virtual number of port (0). Then the driver would know it's used for
 *      factory/hotel mode.
 *  @par
 *  @subsection ssecMW_IO_MNGR_4_3  4.3. Handle IRRC / UART input event
 *      @skip handle event
 *      @skip static
 *      @until return
 *      @until }
 *  @par
 *      &nbsp;
 *      @par Note
 *      After you get the callback from the IO manager, please remember to switch to
 *      your own thread context to do the remaining process. The reasons are;
 *      -#  A deadlock may arise if invoking Middleware API's in callback
 *      function.
 *      -#  Allocate resources (memory, handles) in a callback function will fail.
 *  
 *  @par
 *  @subsection ssecMW_IO_MNGR_4_4  4.4. Send fake button down/up event
 *      @skip send event
 *      @skip {
 *      @until }
 *  @par
 *  @subsection ssecMW_IO_MNGR_4_5  4.5. Output data to UART
 *      @skip output UART
 *      @skip {
 *      @until }
 *  @par
 *      &nbsp;
 *      @par Note
 *      c_iom_output() is an asynchronous function. Therefore, IO manager would notify
 *      the condition @ref IOM_NFY_COND_XMT_COMPLETE when it completes the sending process.
 *  @par
 *  @subsection ssecMW_IO_MNGR_4_6  4.6. Set UART operation mode
 *  @par
 *      Because the TV system uses the port (0) to be the debug port, if you use the
 *      port (0) to send the factory/hotel data, these data may be mixed together and
 *      the data pattern may be wrong. Therefore you need set the operation mode to
 *      tell the IO manager, and the system would help to filter the data.
 *  @par
 *      @skip set_oper
 *      @skip {
 *      @until }
 *  @par
 *      When you want to terminate the factory/hotel mode, you need to change the
 *      operation mode to normal.
 *  @par
 *      @skip set_oper
 *      @skip {
 *      @until }
 *  @par
 *  @subsection ssecMW_IO_MNGR_4_7  4.7. Change UART baudrate
 *      @skip change UART
 *      @skip {
 *      @until }
 */
