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
/*! @file u_socket_dev.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Socket
 *         definitions, which are exported to middleware and applications.
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_SOCKET_DEV_H_
#define _U_SOCKET_DEV_H_
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_RES_MNGR_SOCKET_DEV Socket Device
 *  @ingroup groupMW_RES_MNGR
 *  @brief The u_socket_dev.h module defined Socket Device is exported for AP to set driver parameters.
 *   
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#include "x_common.h"

/*! The status type used by socket device to notify middleware.
 *! - When a child device is attached into the socket device,
 *!   the socket device start to recognize and identify the child device.
 *!   - If the child device cannot be recognized,
 *!     notifies SKTDEV_STATUS_DEV_UNSUPPORT.
 *!   - If the child device can be recognized and also can be identified,
 *!     notifies SKTDEV_STATUS_DEV_ATTACH.
 *!   - If the child device can be recognized but cannot be identified,
 *!     notifies SKTDEV_STATUS_DEV_FAIL.
 *!
 *! - When a child device is detached from the socket device,
 *!   the socket device notifies SKTDEV_STATUS_DEV_DETACH.
 *!
 *! - SKTDEV_STATUS_DEV_PROPRIETARY means if the status value is more than 0x80,
 *!   the status belongs to device proprietray status. And, the detail should be
 *!   enumerated in device related header file.
 */
/*------------------------------------------------------------------*/
/*! @enum SKTDEV_STATUS_T
 *  @brief Socket device status.
 *  @code
 *  typedef enum
 *  {
 *      SKTDEV_STATUS_UNKNOWN,                
 *      SKTDEV_STATUS_DEV_ATTACH,             
 *      SKTDEV_STATUS_DEV_DETACH,             
 *      SKTDEV_STATUS_DEV_FAIL,               
 *      SKTDEV_STATUS_DEV_UNSUPPORT,          
 *      SKTDEV_STATUS_DEV_PROPRIETARY = 0x80  
 *  } SKTDEV_STATUS_T;
 *  @endcode
 *  @li@c  SKTDEV_STATUS_UNKNOWN                             - status initial value
 *  @li@c  SKTDEV_STATUS_DEV_ATTACH                          - device is attached & recognized, and identification succeeds.
 *  @li@c  SKTDEV_STATUS_DEV_DETACH                          - device is detached
 *  @li@c  SKTDEV_STATUS_DEV_FAIL                            - device is detached & recognized, but identification is failed.
 *  @li@c  SKTDEV_STATUS_DEV_UNSUPPORT                       - device is attached but not recognized
 *  @li@c  SKTDEV_STATUS_DEV_PROPRIETARY                     - device proprietary status
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SKTDEV_STATUS_UNKNOWN,                /*!< status initial value */
    SKTDEV_STATUS_DEV_ATTACH,             /*!< device is attached & recognized, and identification succeeds. */
    SKTDEV_STATUS_DEV_DETACH,             /*!< device is detached */
    SKTDEV_STATUS_DEV_FAIL,               /*!< device is attached & recognized, but identification is failed. */
    SKTDEV_STATUS_DEV_UNSUPPORT,          /*!< device is attached but not recognized */
    SKTDEV_STATUS_DEV_PROPRIETARY = 0x80  /*!< device proprietary status */
} SKTDEV_STATUS_T;


/*! Socket device I/O command result types */
/*------------------------------------------------------------------*/
/*! @enum SKTDEV_RESULT_T
 *  @brief Socket device I/O command result types 
 *  @code
 *  typedef enum {
 *      SKTDEV_OK               = 0,          
 *      SKTDEV_FAIL             = -1,         
 *      SKTDEV_ERR_DEV_BUSY     = -2,         
 *      SKTDEV_ERR_ILL_PARM     = -3,         
 *      SKTDEV_ERR_CMD_TIMEOUT  = -4,         
 *      SKTDEV_ERR_CMD_ABORT    = -5,         
 *      SKTDEV_ERR_NOT_SUPPORT  = -6,         
 *      SKTDEV_ERR_PROPRIETARY  = -32         
 *  } SKTDEV_RESULT_T;
 *  @endcode
 *  @li@c  SKTDEV_OK                                         - command succeeds
 *  @li@c  SKTDEV_FAIL                                       - command is failed, and used for BLKDEV_COND_FAIL
 *  @li@c  SKTDEV_ERR_DEV_BUSY                               - used when device cannot accept more commands
 *  @li@c  SKTDEV_ERR_ILL_PARM                               - used when one of the parameters is invaild
 *  @li@c  SKTDEV_ERR_CMD_TIMEOUT                            - used when command is timeout
 *  @li@c  SKTDEV_ERR_CMD_ABORT                              - used when command is aborted by other task
 *  @li@c  SKTDEV_ERR_NOT_SUPPORT                            - used when command is not supported
 *  @li@c  SKTDEV_ERR_PROPRIETARY                            - device priopritary error codes
 */
/*------------------------------------------------------------------*/
typedef enum {
    SKTDEV_OK               = 0,          /*!< command succeeds */
    SKTDEV_FAIL             = -1,         /*!< command is failed, and used for BLKDEV_COND_FAIL */
    SKTDEV_ERR_DEV_BUSY     = -2,         /*!< used when device cannot accept more commands */
    SKTDEV_ERR_ILL_PARM     = -3,         /*!< used when one of the parameters is invaild */
    SKTDEV_ERR_CMD_TIMEOUT  = -4,         /*!< used when command is timeout */
    SKTDEV_ERR_CMD_ABORT    = -5,         /*!< used when command is aborted by other task */
    SKTDEV_ERR_NOT_SUPPORT  = -6,         /*!< used when command is not supported */
    SKTDEV_ERR_PROPRIETARY  = -32         /*!< device priopritary error codes */
} SKTDEV_RESULT_T;


/*! \name Device Flag Defines
 *! The device flag constants used to indicate the attached device information.
 *! - The following flags are exclusive flags:.
 *!   - SKTDEV_FLAG_BLOCK_DEV, SKTDEV_FLAG_CHAR_DEV, and SKTDEV_FLAG_SOCKET_DEV.
 *!
 *! - The following flags can be only used for block device:.
 *!   - SKTDEV_FLAG_STANDY.
 *!
 *! - The following flags can be only used for character device:.
 *!   - SKTDEV_FLAG_ASYNC_WRITE, and SKTDEV_FLAG_ASYNC_READ.
 *!
 *! - The following flags can be only used for socket device:.
 *!   - SKTDEV_FLAG_HOTSWAP_INTERRUPT, SKTDEV_FLAG_HOTSWAP_POLLING, and
 *!     SKTDEV_FLAG_POLLING_IN_IDLE
 *! @{*/
#define SKTDEV_FLAG_BLOCK_DEV         ((UINT32)1 << 0)  /*!< it is a block device */    /**<        */
#define SKTDEV_FLAG_CHAR_DEV          ((UINT32)1 << 1)  /*!< it is a character device */    /**<        */
#define SKTDEV_FLAG_SOCKET_DEV        ((UINT32)1 << 2)  /*!< it is a socket device */    /**<        */
#define SKTDEV_FLAG_STANDY            ((UINT32)1 << 3)  /*!< (block device only)device supports standby command */    /**<        */
#define SKTDEV_FLAG_ASYNC_WRITE       ((UINT32)1 << 4)  /*!< (character device only)device supports asynchronous write command */    /**<        */
#define SKTDEV_FLAG_ASYNC_READ        ((UINT32)1 << 5)  /*!< (character device only)device supports asynchronous read command */    /**<        */
#define SKTDEV_FLAG_HOTSWAP_INTERRUPT ((UINT32)1 << 6)  /*!< (socket device only)device supports hardware interrupt hotswap */    /**<        */
#define SKTDEV_FLAG_HOTSWAP_POLLING   ((UINT32)1 << 7)  /*!< (socket device only)device supports software polling hotswap */    /**<        */
#define SKTDEV_FLAG_POLLING_IN_IDLE   ((UINT32)1 << 8)  /*!< (socket device only)used with SKTDEV_FLAG_HOTSWAP_POLLING, if set, only polling when not command received */    /**<        */
#define SKTDEV_FLAG_MISC_DEV          ((UINT32)1 << 9)  /*!< it is a misc device */    /**<        */
/*! @}*/


/*! The data structure of support device information */
/*------------------------------------------------------------------*/
/*! @struct SKTDEV_SUPPORT_DEV_T
 *  @brief The data structure of support device information
 *  @code
 *  typedef struct _SKTDEV_SUPPORT_DEV_T {
 *      UINT32          u4DevType;    
 *      UINT32          u4CompId;     
 *      UINT32          u4DevFlag;    
 *  } SKTDEV_SUPPORT_DEV_T;
 *  @endcode
 *  @li@c  u4DevType                                         - [OUT]the child device type defined in x_rm_dev_types.h
 *  @li@c  u4CompId                                          - [OUT]the component ID used to register the child device into resource manager
 *  @li@c  u4DevFlag                                         - [OUT]the child device information defined in Device Flag Defines
 */
/*------------------------------------------------------------------*/
typedef struct _SKTDEV_SUPPORT_DEV_T {
    UINT32          u4DevType;    /*!< [OUT]the child device type defined in x_rm_dev_types.h */
    UINT32          u4CompId;     /*!< [OUT]the component ID used to register the child device into resource manager */
    UINT32          u4DevFlag;    /*!< [OUT]the child device information defined in Device Flag Defines */
} SKTDEV_SUPPORT_DEV_T;

/*! The data structure of unsupport device information
 *! - The data structure describes the information of unsupport child device
 *!   information. Becuase the information content depends on different device
 *!   types, the actual structure should be defined in device related header file. */
/*------------------------------------------------------------------*/
/*! @struct SKTDEV_UNSUPPORT_DEV_T
 *  @brief The data structure of unsupport device information
 *  @code
 *  typedef struct _SKTDEV_UNSUPPORT_DEV_T {
 *      VOID            *pvData;      
 *      UINT32          u4DataSz;     
 *  } SKTDEV_UNSUPPORT_DEV_T;
 *  @endcode
 *  @li@c  *pvData                                           - [IN]proprietary unsupport device information structure pointer
 *  @li@c  u4DataSz                                          - [IN]size of unsupport device information structure
 */
/*------------------------------------------------------------------*/
typedef struct _SKTDEV_UNSUPPORT_DEV_T {
    VOID            *pvData;      /*!< [IN]proprietary unsupport device information structure pointer */
    UINT32          u4DataSz;     /*!< [IN]size of unsupport device information structure */
} SKTDEV_UNSUPPORT_DEV_T;

/*! Unsupport device types */
/*------------------------------------------------------------------*/
/*! @enum SKTDEV_UNSUPPORT_TYPE_T
 *  @brief Unsupport device types
 *  @code
 *  typedef enum _SKTDEV_UNSUPPORT_TYPE_T
 *  {
 *      SKTDEV_UNSUPPORT_TYPE_DEV = 0,
 *      SKTDEV_UNSUPPORT_TYPE_HUB = 1
 *  } SKTDEV_UNSUPPORT_TYPE_T;
 *  @endcode
 *  @li@c  SKTDEV_UNSUPPORT_TYPE_DEV                         - Unsupport device types
 *  @li@c  SKTDEV_UNSUPPORT_TYPE_HUB                         - Unsupport hub
 */
/*------------------------------------------------------------------*/
typedef enum _SKTDEV_UNSUPPORT_TYPE_T
{
    SKTDEV_UNSUPPORT_TYPE_DEV = 0,
    SKTDEV_UNSUPPORT_TYPE_HUB = 1
} SKTDEV_UNSUPPORT_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct SKTDEV_UNSUPPORT_DESC_T
 *  @brief Unsupport device description
 *  @code
 *  typedef struct _SKTDEV_UNSUPPORT_DESC_T
 *  {
 *      SKTDEV_UNSUPPORT_TYPE_T t_uns_type;
 *  } SKTDEV_UNSUPPORT_DESC_T;
 *  @endcode
 *  @li@c  t_uns_type                                        -
 */
/*------------------------------------------------------------------*/
typedef struct _SKTDEV_UNSUPPORT_DESC_T
{
    SKTDEV_UNSUPPORT_TYPE_T t_uns_type;
} SKTDEV_UNSUPPORT_DESC_T;

/*! The data structure of device information
 *! - The data structure describes the information of child device,
 *!   and middleware could use the information to open child device component
 *!   from resource manager.
 *!
 *! - There are 2 conditions to use this data structure:
 *!   - 1.when device notifies status to middleware by x_sktdev_st_nfy_fct,
 *!       the data structure is passed in 3rd parameter *pv_data.
 *!     - the status includes SKTDEV_STATUS_DEV_ATTACH, SKTDEV_STATUS_DEV_DETACH,
 *!       SKTDEV_STATUS_DEV_FAIL, and SKTDEV_STATUS_DEV_UNSUPPORT
 *!     - [NOTE]u4SocketNo is OUTPUT from device driver in this condition
 *!
 *!   - 2.when middleware uses SKTDEV_GET_TYPE_CHILD_DEV_INFO to get child
 *!       device information.
 *!     - [NOTE]u4SocketNo is INPUT from middlware in this condition
 */
/*------------------------------------------------------------------*/
/*! @struct SKTDEV_DEV_INFO_T
 *  @brief The data structure of device information
 *  @code
 *  typedef struct _SKTDEV_DEV_INFO_T {
 *      UINT32          u4SocketNo;   
 *      SKTDEV_STATUS_T eDevSt;       
 *      union {
 *          SKTDEV_SUPPORT_DEV_T     rSupDev;   
 *          SKTDEV_UNSUPPORT_DEV_T   rUnsupDev; 
 *      } u_info;
 *  } SKTDEV_DEV_INFO_T;
 *  @endcode
 *  @li@c  u4SocketNo                                        - [IN/OUT]the socket number which the child device is attached to 
 *  @li@c  eDevSt                                            - [OUT]the child device status
 *  @li@c  rSupDev                                           - used when eDevSt is SKTDEV_STATUS_DEV_ATTACH or SKTDEV_STATUS_DEV_DETACH
 *  @li@c  rUnsupDev                                         - used when eDevSt is SKTDEV_STATUS_DEV_FAIL or SKTDEV_STATUS_DEV_UNSUPPORT
 */
/*------------------------------------------------------------------*/
typedef struct _SKTDEV_DEV_INFO_T {
    UINT32          u4SocketNo;   /*!< [IN/OUT]the socket number which the child device is attached to */
    SKTDEV_STATUS_T eDevSt;       /*!< [OUT]the child device status */
    union {
        SKTDEV_SUPPORT_DEV_T     rSupDev;   /* used when eDevSt is SKTDEV_STATUS_DEV_ATTACH or SKTDEV_STATUS_DEV_DETACH */
        SKTDEV_UNSUPPORT_DEV_T   rUnsupDev; /* used when eDevSt is SKTDEV_STATUS_DEV_FAIL or SKTDEV_STATUS_DEV_UNSUPPORT */
    } u_info;
} SKTDEV_DEV_INFO_T;


/*! \name Get Type Defines
 *! @{*/

/*! get socket number (UINT32) */
#define SKTDEV_GET_TYPE_SOCKET_NUM            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))    /**<get socket number (UINT32)        */

/*! get device information of specified socket no. (SKTDEV_DEV_INFO_T) */
#define SKTDEV_GET_TYPE_CHILD_DEV_INFO        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))    /**<get device information of specified socket no. (SKTDEV_DEV_INFO_T)        */

/*! get current device status (SKTDEV_STATUS_T) */
#define SKTDEV_GET_TYPE_STATUS                (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))    /**<get current device status (SKTDEV_STATUS_T)        */

/*! get current device model name (CHAR *) */
#define SKTDEV_GET_TYPE_MODEL                 (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))    /**<get current device model name (CHAR *)        */

/*! get device I/O control buffer memory alignment (UINT32) */
#define SKTDEV_GET_TYPE_ALIGNMENT             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))    /**<get device I/O control buffer memory alignment (UINT32)        */

/*! get maximum number of I/O requests that can be issued to driver simultaneously  (UINT32) */
#define SKTDEV_GET_TYPE_MAX_IO_REQUEST        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 6))    /**<get maximum number of I/O requests that can be issued to driver simultaneously  (UINT32)        */

/*! get device proprietary property */
#define SKTDEV_GET_TYPE_PROPRIETARY_PROPERTY  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0x80))    /**<get device proprietary property        */
/*! @}*/



/*! I/O control result notification function type
 *! - Socket device notifies I/O control result to middleware in this function type.
 */
/*------------------------------------------------------------------*/
/*! @brief I/O control result notification function type
 *  @param [in]  - pv_nfy_tag       private data set from middleware 
 *  @param [in]  - e_nfy_result     command result
 *  @param [in]  - pv_data          command data set from middleware
 *  @return      VOID
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sktdev_cmd_nfy_fct)(
    VOID              *pv_nfy_tag,          /*!< private data set from middleware */
    SKTDEV_RESULT_T   e_nfy_result,         /*!< command result */
    VOID              *pv_data              /*!< command data set from middleware */
);


/*! I/O control result notification data structure
 *! - Middleware prepares the following data for I/O control commands:.
 *!   - *pv_tag: middleware could pass private data to itself
 *!              when it receives notification.
 *!   - pf_sktdev_cmd_nfy: notification function pointer used by socket device
 *!                        to notify the result of I/O control command.
 *!
 *! - Then, middleware could set the notification data to socket device by SKTDEV_SET_TYPE_DEV_ST_NFY.
 */
/*------------------------------------------------------------------*/
/*! @struct SKTDEV_DEV_CMD_NFY_T
 *  @brief I/O control result notification data structure
 *  @code
 *  typedef struct _SKTDEV_DEV_CMD_NFY_T
 *  {
 *      VOID                 *pv_tag;           
 *      x_sktdev_cmd_nfy_fct pf_sktdev_cmd_nfy; 
 *  } SKTDEV_DEV_CMD_NFY_T;
 *  @endcode
 *  @li@c  *pv_tag                                           - [IN]private data
 *  @li@c  pf_sktdev_cmd_nfy                                 - [IN]notification function pointer
 */
/*------------------------------------------------------------------*/
typedef struct _SKTDEV_DEV_CMD_NFY_T
{
    VOID                 *pv_tag;           /*!< [IN]private data */
    x_sktdev_cmd_nfy_fct pf_sktdev_cmd_nfy; /*!< [IN]notification function pointer */
} SKTDEV_DEV_CMD_NFY_T;


/*! Device status notification function type
 *! - Socket device notifies device status to middleware in this function type.
 */
/*------------------------------------------------------------------*/
/*! @brief Device status notification function type
 *  @param [in]   -pv_nfy_tag         private data set from middleware
 *  @param [in]   -e_nfy_st           device status 
 *  @param [in]   -pv_data            notification data sent from socket device
 *  @return       VOID
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sktdev_st_nfy_fct)(
    VOID              *pv_nfy_tag,          /*!< private data set from middleware */
    SKTDEV_STATUS_T   e_nfy_st,             /*!< device status */
    VOID              *pv_data              /*!< notification data sent from socket device */
);


/*! Device status notification data structure
 *! - Middleware prepares the following data for device status notification:.
 *!   - *pv_tag: middleware could pass private data to itself
 *!              when it receives notification.
 *!   - pf_sktdev_st_nfy: notification function pointer used by socket device
 *!                       to notify device status.
 *!
 *! - Then, middleware could set the notification data to socket device by SKTDEV_SET_TYPE_DEV_ST_NFY.
 */
/*------------------------------------------------------------------*/
/*! @struct SKTDEV_DEV_ST_NFY_T
 *  @brief Device status notification data structure
 *  @code
 *  typedef struct _SKTDEV_DEV_ST_NFY_T
 *  {
 *      VOID                *pv_tag;            
 *      x_sktdev_st_nfy_fct pf_sktdev_st_nfy;   
 *  } SKTDEV_DEV_ST_NFY_T;
 *  @endcode
 *  @li@c  *pv_tag                                           - [IN]private data
 *  @li@c  pf_sktdev_st_nfy                                  - [IN]notification function pointer
 */
/*------------------------------------------------------------------*/
typedef struct _SKTDEV_DEV_ST_NFY_T
{
    VOID                *pv_tag;            /*!< [IN]private data */
    x_sktdev_st_nfy_fct pf_sktdev_st_nfy;   /*!< [IN]notification function pointer */
} SKTDEV_DEV_ST_NFY_T;

/*! USB status type */
/*------------------------------------------------------------------*/
/*! @enum SKTDEV_USB_STATUS_T
 *  @brief USB status type
 *  @code
 *  typedef enum _SKTDEV_USB_STATUS_T
 *  {
 *  	SKTDEV_USB_STATUS_NORMAL      = 0,
 *  	SKTDEV_USB_STATUS_OVERCURRENT
 *  } SKTDEV_USB_STATUS_T;
 *  @endcode
 *  @li@c  SKTDEV_USB_STATUS_NORMAL                          - status is ok
 *  @li@c  SKTDEV_USB_STATUS_OVERCURRENT                     - the current is overloaded
 */
/*------------------------------------------------------------------*/
typedef enum _SKTDEV_USB_STATUS_T
{
	SKTDEV_USB_STATUS_NORMAL      = 0,
	SKTDEV_USB_STATUS_OVERCURRENT
} SKTDEV_USB_STATUS_T;

/*! USB status notification function type
 *! - Socket device notifies USB status to middleware in this function type.
 */
/*------------------------------------------------------------------*/
/*! @brief USB status notification function type
 *  @param [in]  -pv_nfy_tag   private data set from middleware
 *  @param [in]  -e_nfy_st     usb status
 *  @param [in]  -pv_data      notification data sent from socket device
 *  @return      VOID
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sktdev_usb_st_nfy_fct)(
    VOID                  *pv_nfy_tag,       /*!< private data set from middleware */
    SKTDEV_USB_STATUS_T   e_nfy_st,          /*!< usb status */
    VOID                  *pv_data           /*!< notification data sent from socket device */
);

/*! USB over current notification data structure
 *! - Middleware prepares the following data for device status notification:.
 *!   - *pv_tag: middleware could pass private data to itself
 *!              when it receives notification.
 *!   - pf_sktdev_usb_st_nfy: notification function pointer used by socket device
 *!                       to notify usb status.
 *!
 *! - Then, middleware could set the notification data to socket device by SKTDEV_SET_TYPE_USB_ST_NFY.
 */
/*------------------------------------------------------------------*/
/*! @struct SKTDEV_USB_ST_NFY_T
 *  @brief USB over current notification data structure
 *  @code
 *  typedef struct _SKTDEV_USB_ST_NFY_T
 *  {
 *      VOID                    *pv_tag;                
 *      x_sktdev_usb_st_nfy_fct pf_sktdev_usb_st_nfy;   
 *  } SKTDEV_USB_ST_NFY_T;
 *  @endcode
 *  @li@c  *pv_tag                                           -[IN]private data
 *  @li@c  pf_sktdev_usb_st_nfy                              -[IN]notification function pointer
 */
/*------------------------------------------------------------------*/
typedef struct _SKTDEV_USB_ST_NFY_T
{
    VOID                    *pv_tag;                /*!< [IN]private data */
    x_sktdev_usb_st_nfy_fct pf_sktdev_usb_st_nfy;   /*!< [IN]notification function pointer */
} SKTDEV_USB_ST_NFY_T;


/*! I/O control data structure
 *! - The data structure describes the device proprietary I/O control function
 *!   data structure. The I/O control types and the data structure of *pvIoData
 *!   should be defined in device related header file.
 */
/*------------------------------------------------------------------*/
/*! @struct SKTDEV_IO_CTRL_T
 *  @brief I/O control data structure
 *  @code
 *  typedef struct _SKTDEV_IO_CTRL_T {
 *      UINT32                u4IoctrlType;      
 *      void                  *pvIoData;         
 *      UINT32                u4IoDataSz;        
 *      UINT32                u4Handle;          
 *      SKTDEV_DEV_CMD_NFY_T  rCmdNfyInfo;       
 *  } SKTDEV_IO_CTRL_T;
 *  @endcode
 *  @li@c  u4IoctrlType                                      - [IN]I/O control types
 *  @li@c  *pvIoData                                         - [IN]I/O parameter data pointer 
 *  @li@c  u4IoDataSz                                        - [IN]I/O parameter data size
 *  @li@c  u4Handle                                          - [IN]handle for abort function
 *  @li@c  rCmdNfyInfo                                       - [IN]command notification information
 */
/*------------------------------------------------------------------*/
typedef struct _SKTDEV_IO_CTRL_T {
    UINT32                u4IoctrlType;      /*!< [IN]I/O control types */
    void                  *pvIoData;         /*!< [IN]I/O parameter data pointer */
    UINT32                u4IoDataSz;        /*!< [IN]I/O parameter data size */
    UINT32                u4Handle;          /*!< [IN]handle for abort function */
    SKTDEV_DEV_CMD_NFY_T  rCmdNfyInfo;       /*!< [IN]command notification information */
} SKTDEV_IO_CTRL_T;


/*! Parameter data structure for abort */
/*------------------------------------------------------------------*/
/*! @struct SKTDEV_ABORT_T
 *  @brief Parameter data structure for abort
 *  @code
 *  typedef struct _SKTDEV_ABORT_T {
 *      UINT32            u4Handle;           
 *      BOOL              fgEnable;           
 *  } SKTDEV_ABORT_T;
 *  @endcode
 *  @li@c  u4Handle                                          - handle of I/O data
 *  @li@c  fgEnable                                          - enable or disable driver abort state
 */
/*------------------------------------------------------------------*/
typedef struct _SKTDEV_ABORT_T {
    UINT32            u4Handle;           /* handle of I/O data */
    BOOL              fgEnable;           /* enable or disable driver abort state */
} SKTDEV_ABORT_T;


/*! \name Set Type Defines
 *! @{ */

/*! set device status notification data (SKTDEV_DEV_ST_NFY_T) */
#define SKTDEV_SET_TYPE_DEV_ST_NFY            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0))    /**<set device status notification data (SKTDEV_DEV_ST_NFY_T)        */

/*! set device status notification data (SKTDEV_DEV_CMD_NFY_T) */
#define SKTDEV_SET_TYPE_DEV_CMD_NFY           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))    /**<set device status notification data (SKTDEV_DEV_CMD_NFY_T)        */

/*! device I/O control function (SKTDEV_IO_CTRL_T) */
#define SKTDEV_SET_TYPE_IO_CTRL               (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))    /**<device I/O control function (SKTDEV_IO_CTRL_T)        */

/*! device I/O control function (SKTDEV_ABORT_T) */
#define SKTDEV_SET_TYPE_ABORT                 (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))    /**<device I/O control function (SKTDEV_ABORT_T)         */

/*! set usb status notification data (SKTDEV_USB_ST_NFY_T) */
#define SKTDEV_SET_TYPE_USB_ST_NFY            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))    /**<set usb status notification data (SKTDEV_USB_ST_NFY_T)        */

/*! set device proprietary property */
#define SKTDEV_SET_TYPE_PROPRIETARY_PROPERTY  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0x80))    /**<        */
/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_RES_MNGR */
/*----------------------------------------------------------------------------*/


#endif /* _U_SOCKET_DEV_H_ */
