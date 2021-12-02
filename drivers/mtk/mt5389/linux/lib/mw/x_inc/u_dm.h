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
/*! @file u_dm.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file contains all the common macros, defines, typedefs, enums 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_DM_H_
#define _U_DM_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"

/*!
 * @addtogroup    groupMW_DM
 * @brief    This file contains all the common macros, defines, typedefs, enums for DM. 
 * 
 * @see groupMW_DM
 *
 * @{
 * */


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MNT_ROOT "/mnt"                         /**<Root device mount point. */
#define MNT_CATEGORY "/mnt/usb"                 /**<USB device mount point. */

#define DM_DEV_SKT_MAX_SLOT_NS        6         /**<Maximum slot number of a socket device        */
#define DM_DEV_MED_DISC_MAX_LAYER_NS  5         /**<Maximum layer number of a medium disc        */
#define DM_DEV_MAX_MODEL_NAME_LEN     40        /**<Maximum length of the device's model name        */
#define DM_MAX_LABEL_NAME_LEN         32        /**<Maximum length of device's path name        */
#define DM_DEV_ROOT_DEV_LABEL         "/dev"    /**<The string lable for the root device        */

/*
 *  Return values.
 */
#define	DMR_OK                  ((INT32)    0)    /**<Everything is OK        */
#define DMR_ERR                 ((INT32)   -1)    /**<Some error occured        */
#define DMR_INVALID_PARM        ((INT32)   -2)    /**<Invalid parameters        */
#define DMR_INVALID_DEV_TYPE    ((INT32)   -3)    /**<Invalid device type        */
#define DMR_INVALID_DEV_CHAR    ((INT32)   -4)    /**<Invalid device characteristic     */
#define DMR_INVALID_HANDLE      ((INT32)   -5)    /**<Invalid input handle        */
#define DMR_NO_MEM              ((INT32)   -6)    /**<System run out of memory        */
#define DMR_WRONG_DEV_STATUS    ((INT32)   -7)    /**<Wrong device status        */
#define DMR_INTERNAL            ((INT32)   -8)    /**<Internal error        */
#define DMR_ALREADY_INIT        ((INT32)   -9)    /**<DM is already initialized        */
#define DMR_NOT_IMPLEMETED      ((INT32)  -10)    /**<This function is not implemented        */
#define DMR_HANDLE              ((INT32)  -11)    /**<System run out of handle        */
#define DMR_OS                  ((INT32)  -12)    /**<OS error        */
#define DMR_DEVICE_ERR          ((INT32)  -13)    /**<Device error        */
#define DMR_FM_ERR              ((INT32)  -14)    /**<File manager error        */
#define DMR_RM_ERR              ((INT32)  -15)    /**<Resource manager error       */
#define DMR_HW_INIT_ERR         ((INT32)  -16)    /**<Init HW component failed        */
#define DMR_CLI_ERR             ((INT32)  -17)    /**<Attached CLI commands table failed        */
#define DMR_NOT_INIT            ((INT32)  -18)    /**<DM is not initialized yet        */
#define DMR_IOC_FAIL            ((INT32)  -32)    /**<IO control failed        */
#define DMR_IOC_ERR_DEV_BUSY    ((INT32)  -33)    /**<IO control failed as device is busy        */
#define DMR_IOC_ERR_ILL_PARM    ((INT32)  -34)    /**<IO control failed as ?        */
#define DMR_IOC_ERR_CMD_TIMEOUT ((INT32)  -35)    /**<IO control failed as command executed timeout        */
#define DMR_IOC_ERR_CMD_ABORT   ((INT32)  -36)    /**<IO control failed as aborted        */
#define DMR_IOC_ERR_NOT_SUPPORT ((INT32)  -37)    /**<IO control failed as comamnd is not supported        */
#define DMR_IOC_ERR_PROPRIETARY ((INT32)  -38)    /**<IO control failed as no permission        */
#define DMR_USB_DRV_NOT_INIT    ((INT32)  -39)
#define DMR_INVALID_PHY_PATH    ((INT32)  -40)

/*------------------------------------------------------------------*/
/*! @enum   DM_HW_TYPE_T
 *  @brief  Supported types for root hardware.
 *  @code
 *  typedef enum _DM_HW_TYPE_T  
 *  {
 *      DM_HW_TYPE_UNKNOWN  = 0,
 *      DM_HW_TYPE_EEPROM   = 1,
 *      DM_HW_TYPE_NAND     = 2,
 *      DM_HW_TYPE_NOR      = 3,
 *      DM_HW_TYPE_USB      = 4,
 *      DM_HW_TYPE_IDE      = 5,
 *      DM_HW_TYPE_FCI      = 6,
 *      DM_HW_TYPE_1394     = 7,
 *      DM_HW_TYPE_ETHERNET = 8
 *  } DM_HW_TYPE_T;
 *  @endcode
 *  @li@c  DM_HW_TYPE_UNKNOWN - Unknown HW
 *  @li@c  DM_HW_TYPE_EEPROM - EEPROM
 *  @li@c  DM_HW_TYPE_NAND - Nand flash
 *  @li@c  DM_HW_TYPE_NOR - Nor flash
 *  @li@c  DM_HW_TYPE_USB - USB
 *  @li@c  DM_HW_TYPE_IDE - IDE
 *  @li@c  DM_HW_TYPE_FCI - FCI
 *  @li@c  DM_HW_TYPE_1394 - 1394
 *  @li@c  DM_HW_TYPE_ETHERNET -  Ethernet
 */
/*------------------------------------------------------------------*/
typedef enum _DM_HW_TYPE_T  
{
    DM_HW_TYPE_UNKNOWN  = 0,
    DM_HW_TYPE_EEPROM   = 1,
    DM_HW_TYPE_NAND     = 2,
    DM_HW_TYPE_NOR      = 3,
    DM_HW_TYPE_USB      = 4,
    DM_HW_TYPE_IDE      = 5,
    DM_HW_TYPE_FCI      = 6,
    DM_HW_TYPE_1394     = 7,
    DM_HW_TYPE_ETHERNET = 8
} DM_HW_TYPE_T;


/*------------------------------------------------------------------*/
/*! @enum   DM_DEV_CHR_T
 *  @brief  Device characteristic (socket device or medium device)
 *  @code
 *  typedef enum _DM_DEV_CHR_T
 *  {
 *      DM_DEV_CHR_UNKNOWN  = 0x0,
 *      DM_DEV_CHR_SOCKET   = 0x100,
 *      DM_DEV_CHR_MEDIUM   = 0x200,
 *      DM_DEV_CHR_MISC     = 0x400
 *  } DM_DEV_CHR_T;
 *  @endcode
 *  @li@c  DM_DEV_CHR_UNKNOWN   - Unknow Device characteristic 
 *  @li@c  DM_DEV_CHR_SOCKET       - Socket devices
 *  @li@c  DM_DEV_CHR_MEDIUM       - Medium devices
 *  @li@c  DM_DEV_CHR_MISC           - Other devices
 */
/*------------------------------------------------------------------*/
typedef enum _DM_DEV_CHR_T
{
    DM_DEV_CHR_UNKNOWN  = 0x0,
    DM_DEV_CHR_SOCKET   = 0x100,
    DM_DEV_CHR_MEDIUM   = 0x200,
    /*zink CSR BT*/
    DM_DEV_CHR_MISC     = 0x400
} DM_DEV_CHR_T;


/*------------------------------------------------------------------*/
/*! @enum   DM_DEV_TYPE_T
 *  @brief  Supported device types.
 *  @code
 *  typedef enum _DM_DEV_TYPE_T
 *  {
 *      DM_DEV_UKW_TYPE_UNKNOWN     = DM_DEV_CHR_UNKNOWN,
 *      DM_DEV_SKT_TYPE_UNKNOWN     = DM_DEV_CHR_SOCKET,
 *      DM_DEV_SKT_TYPE_EEPROM_HW,
 *      DM_DEV_SKT_TYPE_NAND_HW,
 *      DM_DEV_SKT_TYPE_NOR_HW,
 *      DM_DEV_SKT_TYPE_USB_HW,
 *      DM_DEV_SKT_TYPE_IDE_HW,
 *      DM_DEV_SKT_TYPE_FCI_HW,
 *      DM_DEV_SKT_TYPE_1394_HW,    
 *      DM_DEV_SKT_TYPE_HUB,
 *      DM_DEV_SKT_TYPE_CARD_READER,
 *      DM_DEV_SKT_TYPE_OPTI_DRV,
 *      DM_DEV_SKT_TYPE_ETHERNET,
 *      DM_DEV_MED_TYPE_UNKNOWN     = DM_DEV_CHR_MEDIUM,
 *      DM_DEV_MED_TYPE_EEPROM,
 *      DM_DEV_MED_TYPE_NAND,
 *      DM_DEV_MED_TYPE_NOR,
 *      DM_DEV_MED_TYPE_HDD,
 *      DM_DEV_MED_TYPE_DISC,
 *      DM_DEV_MED_TYPE_MEM_CARD,
 *      DM_DEV_MED_TYPE_MASS_STRG,
 *      DM_DEV_MED_TYPE_PTP_MTP,
 *      DM_DEV_MED_TYPE_DLNA_DMS,
        DM_DEV_MED_TYPE_SMB,
 *      DM_DEV_MISC_TYPE_UNKNOWN     = DM_DEV_CHR_MISC,
 *      DM_DEV_MISC_TYPE_USB_BT
 *  } DM_DEV_TYPE_T;
 *  @endcode
 *  @li@c  DM_DEV_UKW_TYPE_UNKNOWN          - Unknown device
 *  @li@c  DM_DEV_SKT_TYPE_UNKNOWN           -  Unknown socket device
 *  @li@c  DM_DEV_SKT_TYPE_EEPROM_HW       - EEPROM socket device
 *  @li@c  DM_DEV_SKT_TYPE_NAND_HW          - Nand socket device
 *  @li@c  DM_DEV_SKT_TYPE_NOR_HW            - Nor socket device
 *  @li@c  DM_DEV_SKT_TYPE_USB_HW            - USB socket device
 *  @li@c  DM_DEV_SKT_TYPE_IDE_HW             - IDE socket device
 *  @li@c  DM_DEV_SKT_TYPE_FCI_HW             - FCI socket device
 *  @li@c  DM_DEV_SKT_TYPE_1394_HW           - 1394 socket device
 *  @li@c  DM_DEV_SKT_TYPE_HUB                   - HUB
 *  @li@c  DM_DEV_SKT_TYPE_CARD_READER   - Card reader
 *  @li@c  DM_DEV_SKT_TYPE_OPTI_DRV          - Optical driver
 *  @li@c  DM_DEV_SKT_TYPE_ETHERNET         - Ethernet socked device
 *  @li@c  DM_DEV_MED_TYPE_UNKNOWN        - Unknown medium device
 *  @li@c  DM_DEV_MED_TYPE_EEPROM           - EEPROM medium device
 *  @li@c  DM_DEV_MED_TYPE_NAND              - Nand medium device
 *  @li@c  DM_DEV_MED_TYPE_NOR                - Nor medium device
 *  @li@c  DM_DEV_MED_TYPE_HDD                - HDD medium device
 *  @li@c  DM_DEV_MED_TYPE_DISC               - Disc medium device
 *  @li@c  DM_DEV_MED_TYPE_MEM_CARD      - memory card medium device
 *  @li@c  DM_DEV_MED_TYPE_MASS_STRG    - Mass-storage medium device
 *  @li@c  DM_DEV_MED_TYPE_PTP_MTP          - MTP medium device
 *  @li@c  DM_DEV_MED_TYPE_DLNA_DMS      - DLNA medium device
 *  @li@c  DM_DEV_MED_TYPE_SMB           - MyNetworkPlaces device
 *  @li@c  DM_DEV_MISC_TYPE_UNKNOWN     - Unknown device
 *  @li@c  DM_DEV_MISC_TYPE_USB_BT         - USB Bluetooth
 */
/*------------------------------------------------------------------*/
typedef enum _DM_DEV_TYPE_T
{
    DM_DEV_UKW_TYPE_UNKNOWN     = DM_DEV_CHR_UNKNOWN,

    DM_DEV_SKT_TYPE_UNKNOWN     = DM_DEV_CHR_SOCKET,
    DM_DEV_SKT_TYPE_EEPROM_HW,
    DM_DEV_SKT_TYPE_NAND_HW,
    DM_DEV_SKT_TYPE_NOR_HW,
    DM_DEV_SKT_TYPE_USB_HW,
    DM_DEV_SKT_TYPE_IDE_HW,
    DM_DEV_SKT_TYPE_FCI_HW,
    DM_DEV_SKT_TYPE_1394_HW,    
    DM_DEV_SKT_TYPE_HUB,
    DM_DEV_SKT_TYPE_CARD_READER,
    DM_DEV_SKT_TYPE_OPTI_DRV,
    DM_DEV_SKT_TYPE_ETHERNET,

    DM_DEV_MED_TYPE_UNKNOWN     = DM_DEV_CHR_MEDIUM,
    DM_DEV_MED_TYPE_EEPROM,
    DM_DEV_MED_TYPE_NAND,
    DM_DEV_MED_TYPE_NOR,
    DM_DEV_MED_TYPE_HDD,
    DM_DEV_MED_TYPE_DISC,
    DM_DEV_MED_TYPE_MEM_CARD,
    DM_DEV_MED_TYPE_MASS_STRG,
    DM_DEV_MED_TYPE_PTP_MTP,
    DM_DEV_MED_TYPE_DLNA_DMS,
    DM_DEV_MED_TYPE_SMB,
    /*zink CSR BT*/
    DM_DEV_MISC_TYPE_UNKNOWN     = DM_DEV_CHR_MISC,
    DM_DEV_MISC_TYPE_USB_BT
} DM_DEV_TYPE_T;

#define DM_DEV_CHR(x) ((DM_DEV_CHR_T)(x & 0xFF00))    /**< Device characteristic mask       */


/*------------------------------------------------------------------*/
/*! @struct DM_COND_T
 *  @brief  Condition type for specifying filters.
 *  @code
 *  typedef struct _DM_COND_T
 *  {
 *      DM_HW_TYPE_T  t_hw_type;
 *      DM_DEV_TYPE_T t_dev_type;
 *  } DM_COND_T;
 *  @endcode
 *  @li@c  t_hw_type    -   If the t_dev_type is specified as DM_HW_TYPE_UNKNOWN, all 
 *                          hardware type match the condition.
 *  @li@c  t_dev_type   -   If the t_dev_type is specified as DM_DEV_UKW_TYPE_UNKNOWN, all device
 *                          match the condition.  If the value is DM_DEV_SKT_TYPE_UNKNOWN, all socket
 *                          device match the condition.  If the value is DM_DEV_MED_TYPE_UNKNWON,
 *                          all medium device match the condition.
 */
/*------------------------------------------------------------------*/
typedef struct _DM_COND_T
{
    DM_HW_TYPE_T  t_hw_type;
    DM_DEV_TYPE_T t_dev_type;
} DM_COND_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_STAT_T
 *  @brief  Type for simplified device information
 *  @code
 *  typedef struct _DM_DEV_STAT_T
 *  {
 *      DM_HW_TYPE_T  t_hw_type;
 *      DM_DEV_TYPE_T t_dev_type;
 *      UINT32        ui4_unit_id;
 *      UINT32        ui4_skt_no;
 *      BOOL          b_avail;
 *      HANDLE_T      h_dev;
 *  } DM_DEV_STAT_T;
 *  @endcode
 *  @li@c  t_hw_type    -   No such device if the value is DM_HW_TYPE_UNKNOWN
 *  @li@c  t_dev_type   -   Un-supported device if the value is DM_DEV_UKW_TYPE_UNKNOWN
 *  @li@c  ui4_unit_id  -   HW component ID
 *  @li@c  ui4_skt_no   -   Attached socket number on the parent device
 *  @li@c  b_avail      -   FALSE if the device is unsupported or failed
 *  @li@c  h_dev        -   valid if the value is not DM_INVALID_HANDLE
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_STAT_T
{
    DM_HW_TYPE_T    t_hw_type;
    DM_DEV_TYPE_T   t_dev_type;
    UINT32          ui4_unit_id;
    UINT32          ui4_skt_no;
    BOOL            b_avail;
    HANDLE_T        h_dev;
} DM_DEV_STAT_T;


/*------------------------------------------------------------------*/
/*! @struct DM_INIT_PARM_T
 *  @brief  Type for Device Manager initialization
 *  @code
 *  typedef struct _DM_INIT_PARM_T
 *  {
 *      DM_COND_T  *pt_automnt_cond;
 *      UINT32     ui4_automnt_cond_ns;
 *  } DM_INIT_PARM_T;
 *  @endcode
 *  @li@c  *pt_automnt_cond         -   Auto mount condition array
 *  @li@c  ui4_automnt_cond_ns      -   Number of the conditions
 */
/*------------------------------------------------------------------*/
typedef struct _DM_INIT_PARM_T
{
    DM_COND_T  *pt_automnt_cond;
    UINT32     ui4_automnt_cond_ns;
} DM_INIT_PARM_T;


/*------------------------------------------------------------------*/
/*! @enum   DM_EVT_CAT_T
 *  @brief  Supported device event categories.
 *  @code
 *  typedef enum _DM_EVT_CAT_T
 *  {
 *      DM_EVT_CAT_DEV      = 0x100,
 *      DM_EVT_CAT_FS       = 0x200,
 *      DM_EVT_CAT_LDR      = 0x400,
 *      DM_EVT_CAT_BOARD    = 0x800
 *  } DM_EVT_CAT_T;
 *  @endcode
 *  @li@c  DM_EVT_CAT_DEV         - Device event categories
 *  @li@c  DM_EVT_CAT_FS           - File system event categories
 *  @li@c  DM_EVT_CAT_LDR         - LDR event categories
 *  @li@c  DM_EVT_CAT_BOARD    - Board event categories
 */
/*------------------------------------------------------------------*/
typedef enum _DM_EVT_CAT_T
{
    DM_EVT_CAT_DEV      = 0x100,
    DM_EVT_CAT_FS       = 0x200,
    DM_EVT_CAT_LDR      = 0x400,
    DM_EVT_CAT_BOARD    = 0x800
} DM_EVT_CAT_T;


/*------------------------------------------------------------------*/
/*! @enum   DM_EVT_T
 *  @brief  Supported device event types.
 *  @code
 *  typedef enum _DM_EVT_T
 *  {
 *      DM_DEV_EVT_UNSUPPORT     = DM_EVT_CAT_DEV,
 *      DM_DEV_EVT_FAIL,    
 *      DM_DEV_EVT_ATTACH,
 *      DM_DEV_EVT_DETACH,
 *      DM_FS_EVT_MOUNTFAIL      = DM_EVT_CAT_FS,
 *      DM_FS_EVT_MOUNTED,
 *      DM_FS_EVT_UNMOUNTED,
 *      DM_LDR_EVT_TRAY_ERROR    = DM_EVT_CAT_LDR,
 *      DM_LDR_EVT_TRAY_OPEN,
 *      DM_LDR_EVT_TRAY_CLOSE,
 *      DM_LDR_EVT_TRAY_OPENING,
 *      DM_LDR_EVT_TRAY_CLOSING,
 *      DM_LDR_EVT_DISC_LOADING,
 *      DM_LDR_EVT_DISC_ABSENT,
 *      DM_LDR_EVT_DISC_PRESENT,
 *      DM_BOARD_EVT_ERROR    = DM_EVT_CAT_BOARD,
 *      DM_BOARD_EVT_USB_ST_NORMAL,
 *      DM_BOARD_EVT_USB_ST_OVER_CURRENT
 *  } DM_EVT_T;
 *  @endcode
 *  @li@c  DM_DEV_EVT_UNSUPPORT                        - Unsupport device
 *  @li@c  DM_DEV_EVT_FAIL                                   - Device failed
 *  @li@c  DM_DEV_EVT_ATTACH                              - Device attached
 *  @li@c  DM_DEV_EVT_DETACH                              - Device detached
 *  @li@c  DM_FS_EVT_MOUNTFAIL                           - Device mounted failed
 *  @li@c  DM_FS_EVT_MOUNTED                              - Device mounted OK
 *  @li@c  DM_FS_EVT_UNMOUNTED                          - Device umounted OK
 *  @li@c  DM_LDR_EVT_TRAY_ERROR                       - Tray error
 *  @li@c  DM_LDR_EVT_TRAY_OPEN                         - Tray opened
 *  @li@c  DM_LDR_EVT_TRAY_CLOSE                        - Tray closed
 *  @li@c  DM_LDR_EVT_TRAY_OPENING                    - Tray is opening
 *  @li@c  DM_LDR_EVT_TRAY_CLOSING                    - Tray is closing
 *  @li@c  DM_LDR_EVT_DISC_LOADING                    - Disc is loading
 *  @li@c  DM_LDR_EVT_DISC_ABSENT                      - Disc is absent
 *  @li@c  DM_LDR_EVT_DISC_PRESENT                     - Disc is present
 *  @li@c  DM_BOARD_EVT_ERROR                             - Board error
 *  @li@c  DM_BOARD_EVT_USB_ST_NORMAL              - USB port status is normal
 *  @li@c  DM_BOARD_EVT_USB_ST_OVER_CURRENT   - USB port status is over current
 */
/*------------------------------------------------------------------*/
typedef enum _DM_EVT_T
{
    DM_DEV_EVT_UNSUPPORT     = DM_EVT_CAT_DEV,
    DM_DEV_EVT_FAIL,    
    DM_DEV_EVT_ATTACH,
    DM_DEV_EVT_DETACH,

    DM_FS_EVT_MOUNTFAIL      = DM_EVT_CAT_FS,
    DM_FS_EVT_MOUNTED,
    DM_FS_EVT_UNMOUNTED,
    DM_FS_EVT_MOUNT_START,
    DM_FS_EVT_MOUNT_END,

    DM_LDR_EVT_TRAY_ERROR    = DM_EVT_CAT_LDR,
    DM_LDR_EVT_TRAY_OPEN,
    DM_LDR_EVT_TRAY_CLOSE,
    DM_LDR_EVT_TRAY_OPENING,
    DM_LDR_EVT_TRAY_CLOSING,
    DM_LDR_EVT_DISC_LOADING,
    DM_LDR_EVT_DISC_ABSENT,
    DM_LDR_EVT_DISC_PRESENT,

    DM_BOARD_EVT_ERROR    = DM_EVT_CAT_BOARD,
    DM_BOARD_EVT_USB_ST_NORMAL,
    DM_BOARD_EVT_USB_ST_OVER_CURRENT
} DM_EVT_T;

#define DM_EVT_CAT(x) ((DM_EVT_CAT_T)(x & 0xFF00))    /**< DM event mask       */


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_EVT_DATA_T
 *  @brief  Device event data structure.
 *  @code
 *  typedef struct _DM_DEV_EVT_DATA_T
 *  {
 *      DM_HW_TYPE_T   t_hw_type;
 *      DM_DEV_TYPE_T  t_dev_type;
 *      UINT32         ui4_unit_id;
 *      UINT32         ui4_root_dev_unit_id;
 *      UINT32         ui4_skt_no;
 *      BOOL           b_avail;
 *      HANDLE_T       h_dev;
 *      HANDLE_T       h_dev_root_lbl;  
 *      CHAR           ps_dev_name[DM_MAX_LABEL_NAME_LEN];
 *  } DM_DEV_EVT_DATA_T;
 *  @endcode
 *  @li@c  t_hw_type                            -   HW type, no such device if the value is DM_HW_TYPE_UNKNOWN
 *  @li@c  t_dev_type                           -   Device type, un-supported device if the value is DM_DEV_UKW_TYPE_UNKNOWN
 *  @li@c  ui4_unit_id                          -   HW component ID
 *  @li@c  ui4_root_dev_unit_id                 -   Root HW component ID
 *  @li@c  ui4_skt_no                           -   Attached socket number on the parent device
 *  @li@c  b_avail                              -   FALSE if the device is unsupported or failed
 *  @li@c  h_dev                                -   Handle of the device
 *  @li@c  h_dev_root_lbl                       -   "dev" entry in FM
 *  @li@c  ps_dev_name[DM_MAX_LABEL_NAME_LEN]   -   Device name
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_EVT_DATA_T
{
    DM_HW_TYPE_T    t_hw_type;
    DM_DEV_TYPE_T   t_dev_type;
    UINT32          ui4_unit_id;
    UINT32          ui4_root_dev_unit_id;
    UINT32          ui4_skt_no;
    BOOL            b_avail;
    HANDLE_T        h_dev;
    HANDLE_T        h_dev_root_lbl;
    CHAR            ps_dev_name[DM_MAX_LABEL_NAME_LEN];
} DM_DEV_EVT_DATA_T;


/*------------------------------------------------------------------*/
/*! @struct DM_FS_EVT_DATA_T
 *  @brief  File system event data structure.
 *  @code
 *  typedef struct _DM_FS_EVT_DATA_T
 *  {
 *      HANDLE_T  h_dev;
 *      HANDLE_T  h_dev_root_lbl;
 *      CHAR      ps_dev_name[DM_MAX_LABEL_NAME_LEN];
 *      CHAR      ps_part_name[DM_MAX_LABEL_NAME_LEN];
 *      UINT32    ui4_part_idx;
 *      CHAR      ps_mnt_path[DM_MAX_LABEL_NAME_LEN];
 *  } DM_FS_EVT_DATA_T;
 *  @endcode
 *  @li@c  h_dev                                -   Valid if the value is not DM_INVALID_HANDLE, the device is opened through RM
 *  @li@c  h_dev_root_lbl                       -   "dev" entry in FM
 *  @li@c  ps_dev_name[DM_MAX_LABEL_NAME_LEN]   -   Name of true device, "/dev" is excluded
 *  @li@c  ps_part_name[DM_MAX_LABEL_NAME_LEN]  -   Name of partition name, "/dev" is excluded
 *  @li@c  ui4_part_idx                         -   0-based index value for the order of the partitions
 *  @li@c  ps_mnt_path[DM_MAX_LABEL_NAME_LEN]   -   The full path of the mount point
 */
/*------------------------------------------------------------------*/
typedef struct _DM_FS_EVT_DATA_T
{
    HANDLE_T  h_dev;
    HANDLE_T  h_dev_root_lbl;
    CHAR      ps_dev_name[DM_MAX_LABEL_NAME_LEN];
    CHAR      ps_part_name[DM_MAX_LABEL_NAME_LEN];
    UINT32    ui4_part_idx;
    CHAR      ps_mnt_path[DM_MAX_LABEL_NAME_LEN];
#ifdef LINUX_TURNKEY_SOLUTION
    /* When event is mounted and unmounted. This var indicate
     * Which port send the event.  
     */
    UINT8      ui1_usb_port;
#endif
} DM_FS_EVT_DATA_T;


/*------------------------------------------------------------------*/
/*! @struct DM_LDR_EVT_DATA_T
 *  @brief  Loader event data structure.
 *  @code
 *  typedef struct _DM_LDR_EVT_DATA_T
 *  {
 *      HANDLE_T  h_dev;
 *  } DM_LDR_EVT_DATA_T;
 *  @endcode
 *  @li@c  h_dev        -   Valid if the value is not DM_INVALID_HANDLE: the device is opened through RM
 */
/*------------------------------------------------------------------*/
typedef struct _DM_LDR_EVT_DATA_T
{
    HANDLE_T    h_dev;
} DM_LDR_EVT_DATA_T;


/*------------------------------------------------------------------*/
/*! @enum   DM_USB_STATUS_TYPE_T
 *  @brief  USB port status
 *  @code
 *  typedef enum _DM_USB_STATUS_TYPE_T
 *  {
 *      DM_USB_ST_NORMAL = 0,
 *      DM_USB_ST_OVER_CURRENT
 *  } DM_USB_STATUS_TYPE_T;
 *  @endcode
 *  @li@c  DM_USB_ST_NORMAL         -   Normal status
 *  @li@c  DM_USB_ST_OVER_CURRENT   -   Over-current status
 */
/*------------------------------------------------------------------*/
typedef enum _DM_USB_STATUS_TYPE_T
{
    DM_USB_ST_NORMAL         = 0,
    DM_USB_ST_OVER_CURRENT
} DM_USB_STATUS_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct DM_USB_STATUS_T
 *  @brief  USB port status
 *  @code
 *  typedef struct _DM_USB_STATUS_T
 *  {
 *      UINT32                ui4_port_no; 
 *      DM_USB_STATUS_TYPE_T  e_st;        
 *  } DM_USB_STATUS_T;
 *  @endcode
 *  @li@c  ui4_port_no      -   USB port ID
 *  @li@c  e_st             -   Status
 */
/*------------------------------------------------------------------*/
typedef struct _DM_USB_STATUS_T
{
    UINT32                ui4_port_no;
    DM_USB_STATUS_TYPE_T  e_st;
} DM_USB_STATUS_T;


/*------------------------------------------------------------------*/
/*! @brief  Event callback prototypes which would be registered in c_dm_reg_nfy_fct().
 *  @param [in] t_evt - Event type.
 *  @param [in] pv_data - Event data.
 *  @param [in] pv_tag - Proprietary information set by event listener.
 *  @param [out] None.
 *  @see c_dm_reg_nfy_fct
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_dm_nfy_fct)(
    DM_EVT_T t_evt, 
    VOID*    pv_data, 
    VOID*    pv_tag);


/*------------------------------------------------------------------*/
/*! @struct DM_EVT_FILTR_T
 *  @brief  Event filter data structure.
 *  @code
 *  typedef struct _DM_EVT_FILTR_T
 *  {
 *      DM_COND_T* pt_evt_filtr;
 *      UINT32     ui4_evt_filtr_ns;
 *  } DM_EVT_FILTR_T;
 *  @endcode
 *  @li@c  pt_evt_filtr         -   Filter conditions array
 *  @li@c  ui4_evt_filtr_ns     -   Number of the conditions
 */
/*------------------------------------------------------------------*/
typedef struct _DM_EVT_FILTR_T
{
    DM_COND_T* pt_evt_filtr;
    UINT32     ui4_evt_filtr_ns;
} DM_EVT_FILTR_T;


/*------------------------------------------------------------------*/
/*! @enum   DM_DEV_UNS_TYPE_T
 *  @brief  Unsupport device type.
 *  @code
 *  typedef enum _DM_DEV_UNS_TYPE_T
 *  {
 *      DM_DEV_UNS_TYPE_DEV = 0,
 *      DM_DEV_UNS_TYPE_HUB = 1
 *  } DM_DEV_UNS_TYPE_T;
 *  @endcode
 *  @li@c  DM_DEV_UNS_TYPE_DEV      - Unsupport device
 *  @li@c  DM_DEV_UNS_TYPE_HUB      - Unsupport HUB
 */
/*------------------------------------------------------------------*/
typedef enum _DM_DEV_UNS_TYPE_T
{
    DM_DEV_UNS_TYPE_DEV = 0,
    DM_DEV_UNS_TYPE_HUB = 1
} DM_DEV_UNS_TYPE_T;


/*------------------------------------------------------------------*/
/*! @enum   DM_DEV_MED_TX_TYPE_T
 *  @brief  Medium device trasfer type (block device or character device).
 *  @code
 *  typedef enum _DM_DEV_MED_TX_TYPE_T
 *  {
 *      DM_DEV_MED_TX_TYPE_BLKDEV = 0,
 *      DM_DEV_MED_TX_TYPE_CHRDEV = 1
 *  } DM_DEV_MED_TX_TYPE_T;
 *  @endcode
 *  @li@c  DM_DEV_MED_TX_TYPE_BLKDEV    -   Block device
 *  @li@c  DM_DEV_MED_TX_TYPE_CHRDEV    -   Character device
 */
/*------------------------------------------------------------------*/
typedef enum _DM_DEV_MED_TX_TYPE_T
{
    DM_DEV_MED_TX_TYPE_BLKDEV = 0,
    DM_DEV_MED_TX_TYPE_CHRDEV = 1
} DM_DEV_MED_TX_TYPE_T;


/*------------------------------------------------------------------*/
/*! @enum   DM_DEV_MED_CARD_TYPE_T
 *  @brief  Supported memory card types.
 *  @code
 *  typedef enum _DM_DEV_MED_CARD_TYPE_T
 *  {
 *      DM_DEV_MED_CARD_TYPE_UNKNOWN  = 0,
 *      DM_DEV_MED_CARD_TYPE_CF       = 1,
 *      DM_DEV_MED_CARD_TYPE_SD       = 2,
 *      DM_DEV_MED_CARD_TYPE_MMC      = 3,
 *      DM_DEV_MED_CARD_TYPE_MS       = 4,
 *      DM_DEV_MED_CARD_TYPE_MS_PRO   = 5,
 *      DM_DEV_MED_CARD_TYPE_SM       = 6,
 *      DM_DEV_MED_CARD_TYPE_XD       = 7,
 *      DM_DEV_MED_CARD_TYPE_USB      = 100    
 *  } DM_DEV_MED_CARD_TYPE_T;
 *  @endcode
 *  @li@c  DM_DEV_MED_CARD_TYPE_UNKNOWN               - Unknown medium card
 *  @li@c  DM_DEV_MED_CARD_TYPE_CF                           - CF card
 *  @li@c  DM_DEV_MED_CARD_TYPE_SD                           - SD card
 *  @li@c  DM_DEV_MED_CARD_TYPE_MMC                         - MMC card
 *  @li@c  DM_DEV_MED_CARD_TYPE_MS                           - MS card
 *  @li@c  DM_DEV_MED_CARD_TYPE_MS_PRO                    - MS Professionalcard
 *  @li@c  DM_DEV_MED_CARD_TYPE_SM                           - SM card
 *  @li@c  DM_DEV_MED_CARD_TYPE_XD                           - XD card
 *  @li@c  DM_DEV_MED_CARD_TYPE_USB                          - USB card
 */
/*------------------------------------------------------------------*/
typedef enum _DM_DEV_MED_CARD_TYPE_T
{
    DM_DEV_MED_CARD_TYPE_UNKNOWN  = 0,
    DM_DEV_MED_CARD_TYPE_CF       = 1,
    DM_DEV_MED_CARD_TYPE_SD       = 2,
    DM_DEV_MED_CARD_TYPE_MMC      = 3,
    DM_DEV_MED_CARD_TYPE_MS       = 4,
    DM_DEV_MED_CARD_TYPE_MS_PRO   = 5,
    DM_DEV_MED_CARD_TYPE_SM       = 6,
    DM_DEV_MED_CARD_TYPE_XD       = 7,
    DM_DEV_MED_CARD_TYPE_USB      = 100    
} DM_DEV_MED_CARD_TYPE_T;


/*------------------------------------------------------------------*/
/*! @enum   DM_DEV_MED_DISC_TYPE_T
 *  @brief  Supported optical disc types.
 *  @code
 *  typedef enum _DM_DEV_MED_DISC_TYPE_T
 *  {
 *      DM_DEV_MED_DISC_TYPE_UNKNOWN = 0,
 *      DM_DEV_MED_DISC_TYPE_CD_ROM  = 1,
 *      DM_DEV_MED_DISC_TYPE_CD_R    = 2,
 *      DM_DEV_MED_DISC_TYPE_CD_RW   = 3,
 *      DM_DEV_MED_DISC_TYPE_DVD_ROM = 4,
 *      DM_DEV_MED_DISC_TYPE_DVD_R   = 5,
 *      DM_DEV_MED_DISC_TYPE_DVD_RW  = 6,
 *      DM_DEV_MED_DISC_TYPE_DVD_PR  = 7,
 *      DM_DEV_MED_DISC_TYPE_DVD_PRW = 8,
 *      DM_DEV_MED_DISC_TYPE_DVD_RAM = 9,
 *      DM_DEV_MED_DISC_TYPE_BD_ROM  = 10,
 *      DM_DEV_MED_DISC_TYPE_BD_RE   = 11,
 *      DM_DEV_MED_DISC_TYPE_BD_RW   = 12
 *  } DM_DEV_MED_DISC_TYPE_T;
 *  @endcode
 *  @li@c  DM_DEV_MED_DISC_TYPE_UNKNOWN                 - Unknown disc
 *  @li@c  DM_DEV_MED_DISC_TYPE_CD_ROM                    - CD ROM
 *  @li@c  DM_DEV_MED_DISC_TYPE_CD_R                         - CD R
 *  @li@c  DM_DEV_MED_DISC_TYPE_CD_RW                      - CD RW
 *  @li@c  DM_DEV_MED_DISC_TYPE_DVD_ROM                  - DVR ROM
 *  @li@c  DM_DEV_MED_DISC_TYPE_DVD_R                      - DVD R
 *  @li@c  DM_DEV_MED_DISC_TYPE_DVD_RW                    - DVR RW
 *  @li@c  DM_DEV_MED_DISC_TYPE_DVD_PR                     - DVD PR
 *  @li@c  DM_DEV_MED_DISC_TYPE_DVD_PRW                  - DVR PRW
 *  @li@c  DM_DEV_MED_DISC_TYPE_DVD_RAM                  - DVR RAM
 *  @li@c  DM_DEV_MED_DISC_TYPE_BD_ROM                    - BD ROM
 *  @li@c  DM_DEV_MED_DISC_TYPE_BD_RE                       - BD RE
 *  @li@c  DM_DEV_MED_DISC_TYPE_BD_RW                      - BD RW
 */
/*------------------------------------------------------------------*/
typedef enum _DM_DEV_MED_DISC_TYPE_T
{
    DM_DEV_MED_DISC_TYPE_UNKNOWN = 0,
    DM_DEV_MED_DISC_TYPE_CD_ROM  = 1,
    DM_DEV_MED_DISC_TYPE_CD_R    = 2,
    DM_DEV_MED_DISC_TYPE_CD_RW   = 3,
    DM_DEV_MED_DISC_TYPE_DVD_ROM = 4,
    DM_DEV_MED_DISC_TYPE_DVD_R   = 5,
    DM_DEV_MED_DISC_TYPE_DVD_RW  = 6,
    DM_DEV_MED_DISC_TYPE_DVD_PR  = 7,
    DM_DEV_MED_DISC_TYPE_DVD_PRW = 8,
    DM_DEV_MED_DISC_TYPE_DVD_RAM = 9,
    DM_DEV_MED_DISC_TYPE_BD_ROM  = 10,
    DM_DEV_MED_DISC_TYPE_BD_RE   = 11,
    DM_DEV_MED_DISC_TYPE_BD_RW   = 12
} DM_DEV_MED_DISC_TYPE_T;


/*------------------------------------------------------------------*/
/*! @enum   DM_DEV_MED_DISC_STATUS_T 
 *  @brief  Optical disc status.
 *  @code
 *  typedef enum _DM_DEV_MED_DISC_STATUS_T 
 *  {
 *      DM_DEV_MED_DISC_STATUS_INVALID     = 0,
 *      DM_DEV_MED_DISC_STATUS_RDONLY      = 1,
 *      DM_DEV_MED_DISC_STATUS_BLANK       = 2,
 *      DM_DEV_MED_DISC_STATUS_FORMATTED   = 3,
 *      DM_DEV_MED_DISC_STATUS_APPENDABLE  = 4
 *  } DM_DEV_MED_DISC_STATUS_T;
 *  @endcode
 *  @li@c  DM_DEV_MED_DISC_STATUS_INVALID                    - Invalid
 *  @li@c  DM_DEV_MED_DISC_STATUS_RDONLY                     - Read only
 *  @li@c  DM_DEV_MED_DISC_STATUS_BLANK                      - Blank sector
 *  @li@c  DM_DEV_MED_DISC_STATUS_FORMATTED                  - Formatted
 *  @li@c  DM_DEV_MED_DISC_STATUS_APPENDABLE                 - Appendable
 */
/*------------------------------------------------------------------*/
typedef enum _DM_DEV_MED_DISC_STATUS_T 
{
    DM_DEV_MED_DISC_STATUS_INVALID     = 0,
    DM_DEV_MED_DISC_STATUS_RDONLY      = 1,
    DM_DEV_MED_DISC_STATUS_BLANK       = 2,
    DM_DEV_MED_DISC_STATUS_FORMATTED   = 3,
    DM_DEV_MED_DISC_STATUS_APPENDABLE  = 4
} DM_DEV_MED_DISC_STATUS_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_MED_DISC_LAYER_SIZE_T 
 *  @brief  Optical disc layer information.
 *  @code
 *  typedef struct _DM_DEV_MED_DISC_LAYER_SIZE_T 
 *  { 
 *      UINT32 ui4_layer_ns;
 *      UINT32 aui4_layer_sec_ns[DM_DEV_MED_DISC_MAX_LAYER_NS];
 *  } DM_DEV_MED_DISC_LAYER_SIZE_T;
 *  @endcode
 *  @li@c  ui4_layer_ns                                      - layer number
 *  @li@c  aui4_layer_sec_ns[DM_DEV_MED_DISC_MAX_LAYER_NS]   - sector number
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_MED_DISC_LAYER_SIZE_T 
{ 
    UINT32 ui4_layer_ns;
    UINT32 aui4_layer_sec_ns[DM_DEV_MED_DISC_MAX_LAYER_NS];
} DM_DEV_MED_DISC_LAYER_SIZE_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_MED_DISC_INF_T
 *  @brief  Optical disc proprietary information.
 *  @code
 *  typedef struct _DM_DEV_MED_DISC_INF_T
 *  {
 *      DM_DEV_MED_DISC_TYPE_T        t_disc_type;
 *      DM_DEV_MED_DISC_STATUS_T      t_disc_status;
 *      DM_DEV_MED_DISC_LAYER_SIZE_T  t_disc_layer;
 *  } DM_DEV_MED_DISC_INF_T;
 *  @endcode
 *  @li@c  t_disc_type                                       - Disc type
 *  @li@c  t_disc_status                                     - Disc status
 *  @li@c  t_disc_layer                                      - Disc layer
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_MED_DISC_INF_T
{
    DM_DEV_MED_DISC_TYPE_T        t_disc_type;
    DM_DEV_MED_DISC_STATUS_T      t_disc_status;
    DM_DEV_MED_DISC_LAYER_SIZE_T  t_disc_layer;
} DM_DEV_MED_DISC_INF_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_MED_CARD_INF_T
 *  @brief  Memory card proprietary information.
 *  @code
 *  typedef struct _DM_DEV_MED_CARD_INF_T
 *  {
 *      DM_DEV_MED_CARD_TYPE_T t_card_type;
 *  } DM_DEV_MED_CARD_INF_T;
 *  @endcode
 *  @li@c  t_card_type                                       - Medium card type
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_MED_CARD_INF_T
{
    DM_DEV_MED_CARD_TYPE_T t_card_type;
} DM_DEV_MED_CARD_INF_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_SLOT_INF_T
 *  @brief  Slot information for socket device.
 *  @code
 *  typedef struct _DM_DEV_SLOT_INF_T
 *  {
 *      UINT32 ui4_slot_no;
 *      DM_DEV_STAT_T t_dev_stat;
 *  } DM_DEV_SLOT_INF_T;
 *  @endcode
 *  @li@c  ui4_slot_no                                       - Slot NO.
 *  @li@c  t_dev_stat                                        - Device status
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_SLOT_INF_T
{
    UINT32        ui4_slot_no;
    DM_DEV_STAT_T t_dev_stat;
} DM_DEV_SLOT_INF_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_SKT_INF_T
 *  @brief  Socket device information.
 *  @code
 *  typedef struct _DM_DEV_SKT_INF_T
 *  {
 *      UINT32  ui4_total_slot_ns;
 *      UINT32  ui4_used_slot_ns;
 *      DM_DEV_SLOT_INF_T at_slot_inf[DM_DEV_SKT_MAX_SLOT_NS];
 *  } DM_DEV_SKT_INF_T;
 *  @endcode
 *  @li@c  ui4_total_slot_ns                                 - Total slot number
 *  @li@c  ui4_used_slot_ns                                  - Used slot number
 *  @li@c  at_slot_inf[DM_DEV_SKT_MAX_SLOT_NS]              - Slot info
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_SKT_INF_T
{
    UINT32  ui4_total_slot_ns;
    UINT32  ui4_used_slot_ns;

    DM_DEV_SLOT_INF_T at_slot_inf[DM_DEV_SKT_MAX_SLOT_NS];
} DM_DEV_SKT_INF_T;

/*------------------------------------------------------------------*/
/*! @struct DM_DEV_UNS_INF_T
 *  @brief  Un-supported device info
 *  @code
 *  typedef struct _DM_DEV_UNS_INF_T
 *  {
 *      DM_DEV_UNS_TYPE_T t_uns_type;
 *  } DM_DEV_UNS_INF_T;
 *  @endcode
 *  @li@c  t_uns_type                                        - Unsupport device type
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_UNS_INF_T
{
    DM_DEV_UNS_TYPE_T t_uns_type;
} DM_DEV_UNS_INF_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_BLK_INF_T
 *  @brief  Block device information.
 *  @code
 *  typedef struct _DM_DEV_BLK_INF_T
 *  {
 *      UINT64 ui8_sec_ns;
 *      UINT32 ui4_sec_size;
 *      UINT32 ui4_max_tx_sec_ns;
 *      UINT32 ui4_best_tx_sec_ns;
 *      union
 *      {
 *          DM_DEV_MED_DISC_INF_T t_disc_inf;
 *          DM_DEV_MED_CARD_INF_T t_card_inf;
 *      } u;
 *  } DM_DEV_BLK_INF_T;
 *  @endcode
 *  @li@c  ui8_sec_ns                                        - Sector number
 *  @li@c  ui4_sec_size                                      - Sector size
 *  @li@c  ui4_max_tx_sec_ns                                 - Max transition sector number
 *  @li@c  ui4_best_tx_sec_ns                                - Best transition sector number
 *  @li@c  t_disc_inf                                        - Disc info
 *  @li@c  t_card_inf                                        - Card info
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_BLK_INF_T
{
    UINT64 ui8_sec_ns;
    UINT32 ui4_sec_size;

    UINT32 ui4_max_tx_sec_ns;
    UINT32 ui4_best_tx_sec_ns;

    union
    {
        DM_DEV_MED_DISC_INF_T t_disc_inf;
        DM_DEV_MED_CARD_INF_T t_card_inf;
    } u;
} DM_DEV_BLK_INF_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_CHR_INF_T
 *  @brief  Character device information.
 *  @code
 *  typedef struct _DM_DEV_CHR_INF_T
 *  {
 *      UINT32 ui4_max_sync_read_sz;
 *      UINT32 ui4_max_sync_write_sz;
 *      UINT32 ui4_sec_ns;
 *  } DM_DEV_CHR_INF_T;
 *  @endcode
 *  @li@c  ui4_max_sync_read_sz                              - Max sync read size
 *  @li@c  ui4_max_sync_write_sz                             - Max sync write size
 *  @li@c  ui4_sec_ns                                               - Sector number
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_CHR_INF_T
{
    UINT32 ui4_max_sync_read_sz;
    UINT32 ui4_max_sync_write_sz;
    UINT32 ui4_sec_ns;
} DM_DEV_CHR_INF_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_MED_INF_T
 *  @brief  Medium device information.
 *  @code
 *  typedef struct _DM_DEV_MED_INF_T
 *  {
 *      DM_DEV_MED_TX_TYPE_T t_tx_type;
 *      UINT64 ui8_total_size;  
 *      union
 *      {
 *          DM_DEV_BLK_INF_T t_blk_inf;
 *          DM_DEV_CHR_INF_T t_chr_inf;
 *      } u;
 *      HANDLE_T  h_dev_root_lbl;  
 *      CHAR ps_dev_name[DM_MAX_LABEL_NAME_LEN];
 *  } DM_DEV_MED_INF_T;
 *  @endcode
 *  @li@c  t_tx_type                            -  Medium device trasfer type (block device or character device)
 *  @li@c  ui8_total_size                       -  General for medium type
 *  @li@c  t_blk_inf                            -  Block device info
 *  @li@c  t_chr_inf                            -  Character device info
 *  @li@c  h_dev_root_lbl                       -  "dev" entry in FM 
 *  @li@c  ps_dev_name[DM_MAX_LABEL_NAME_LEN]   -  Device name
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_MED_INF_T
{
    DM_DEV_MED_TX_TYPE_T t_tx_type;

    UINT64 ui8_total_size;

    union
    {
        DM_DEV_BLK_INF_T t_blk_inf;
        DM_DEV_CHR_INF_T t_chr_inf;
    } u;

    HANDLE_T  h_dev_root_lbl;

    CHAR ps_dev_name[DM_MAX_LABEL_NAME_LEN];
} DM_DEV_MED_INF_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_INF_T
 *  @brief  Device information.
 *  @code
 *  typedef struct _DM_DEV_INF_T
 *  {
 *      DM_HW_TYPE_T   t_hw_type;
 *      DM_DEV_TYPE_T  t_dev_type;
 *      DM_DEV_STAT_T  t_root_stat;
 *      DM_DEV_STAT_T  t_prnt_stat;
 *      UINT32         ui4_unit_id;
 *      UINT32         ui4_skt_no;
 *      VOID*          pv_data;
 *      UINT32         ui4_data_size;
 *      UINT32         ui4_max_io_request;
 *      UINT8          aui1_model_str[DM_DEV_MAX_MODEL_NAME_LEN];
 *      UINT32         ui4_mem_align;
 *      BOOL           b_avail;
 *      union
 *      {
 *          DM_DEV_UNS_INF_T  t_uns_inf;
 *          DM_DEV_SKT_INF_T  t_skt_inf;
 *          DM_DEV_MED_INF_T  t_med_inf;
 *      } u;
 *  } DM_DEV_INF_T;
 *  @endcode
 *  @li@c  t_hw_type                                         - HW type
 *  @li@c  t_dev_type                                        - Device type
 *  @li@c  t_root_stat                                       - Root status
 *  @li@c  t_prnt_stat                                       - Parent status
 *  @li@c  ui4_unit_id                                       - Unit ID
 *  @li@c  ui4_skt_no                                        - Socket NO.
 *  @li@c  pv_data                                           - Private data
 *  @li@c  ui4_data_size                                     - Data size
 *  @li@c  ui4_max_io_request                                - Max IO requests
 *  @li@c  aui1_model_str[DM_DEV_MAX_MODEL_NAME_LEN]         - Model name
 *  @li@c  ui4_mem_align                                     - Memory align bytes
 *  @li@c  b_avail                                           - Available or not?
 *  @li@c  t_uns_inf                                         - Unsupport device info
 *  @li@c  t_skt_inf                                         - Socket device info
 *  @li@c  t_med_inf                                         - Medium device info
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_INF_T
{
    DM_HW_TYPE_T   t_hw_type;
    DM_DEV_TYPE_T  t_dev_type;
    DM_DEV_STAT_T  t_root_stat;
    DM_DEV_STAT_T  t_prnt_stat;
    UINT32         ui4_unit_id;
    UINT32         ui4_skt_no;
    VOID*          pv_data;
    UINT32         ui4_data_size;
    UINT32         ui4_max_io_request;
    UINT8          aui1_model_str[DM_DEV_MAX_MODEL_NAME_LEN];
    UINT32         ui4_mem_align;
    BOOL           b_avail;

    union
    {
        DM_DEV_UNS_INF_T  t_uns_inf;
        DM_DEV_SKT_INF_T  t_skt_inf;
        DM_DEV_MED_INF_T  t_med_inf;
    } u;
} DM_DEV_INF_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_FILTR_T
 *  @brief  Device filter for device status query function.
 *  @code
 *  typedef struct _DM_DEV_FILTR_T
 *  {
 *      DM_COND_T* pt_dev_filtr;
 *      UINT32     ui4_dev_filtr_ns;
 *  } DM_DEV_FILTR_T;
 *  @endcode
 *  @li@c  pt_dev_filtr                                     - Device filters array
 *  @li@c  ui4_dev_filtr_ns                              - Length of the device filters array
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_FILTR_T
{
    DM_COND_T* pt_dev_filtr;
    UINT32     ui4_dev_filtr_ns;
} DM_DEV_FILTR_T;

/*------------------------------------------------------------------*/
/*! @struct DM_DEV_SPEC_T
 *  @brief  Indicates a specific device
 *  @code
 *  typedef struct _DM_DEV_SPEC_T
 *  {
 *      DM_HW_TYPE_T  t_hw_type;
 *      DM_DEV_TYPE_T t_dev_type;
 *      UINT32        ui4_unit_id;
 *  } DM_DEV_SPEC_T;
 *  @endcode
 *  @li@c  t_hw_type                                         - HW type
 *  @li@c  t_dev_type                                        -Device type
 *  @li@c  ui4_unit_id                                       - Unit ID
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_SPEC_T
{
    DM_HW_TYPE_T  t_hw_type;
    DM_DEV_TYPE_T t_dev_type;
    UINT32  ui4_unit_id;
} DM_DEV_SPEC_T;

/*------------------------------------------------------------------*/
/*! @enum   DM_IOC_COND_T
 *  @brief  IO control conditions
 *  @code
 *  typedef enum {
 *      DM_IOC_OK               = 0,
 *      DM_IOC_FAIL             = -1,
 *      DM_IOC_ERR_DEV_BUSY     = -2,
 *      DM_IOC_ERR_ILL_PARM     = -3,
 *      DM_IOC_ERR_CMD_TIMEOUT  = -4,
 *      DM_IOC_ERR_CMD_ABORT    = -5,
 *      DM_IOC_ERR_NOT_SUPPORT  = -6,
 *      DM_IOC_ERR_PROPRIETARY  = -32
 *  } DM_IOC_COND_T;
 *  @endcode
 *  @li@c  DM_IOC_OK                                         - IO control OK,  Not used
 *  @li@c  DM_IOC_FAIL                                       - IO control Failed,  Not used
 *  @li@c  DM_IOC_ERR_DEV_BUSY                               - Device is busy,  Not used
 *  @li@c  DM_IOC_ERR_ILL_PARM                               - Invalid parameters,  Not used
 *  @li@c  DM_IOC_ERR_CMD_TIMEOUT                            - Time out,  Not used
 *  @li@c  DM_IOC_ERR_CMD_ABORT                              - Aborted,  Not used
 *  @li@c  DM_IOC_ERR_NOT_SUPPORT                            - Not supported,  Not used
 *  @li@c  DM_IOC_ERR_PROPRIETARY                            - Permission deny,  Not used
 */
/*------------------------------------------------------------------*/
typedef enum {
    DM_IOC_OK               = 0,
    DM_IOC_FAIL             = -1,
    DM_IOC_ERR_DEV_BUSY     = -2,
    DM_IOC_ERR_ILL_PARM     = -3,
    DM_IOC_ERR_CMD_TIMEOUT  = -4,
    DM_IOC_ERR_CMD_ABORT    = -5,
    DM_IOC_ERR_NOT_SUPPORT  = -6,
    DM_IOC_ERR_PROPRIETARY  = -32
} DM_IOC_COND_T;

/*------------------------------------------------------------------*/
/*! @brief  IOC event callback prototypes.
 *  @note Not used in DTV platform
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_dm_ioc_fct)(
    HANDLE_T      h_ioc,
    VOID*         pv_tag,
    VOID*         pv_data,
    UINT32        ui4_data_sz,
    DM_IOC_COND_T t_ioc_cond);


/*------------------------------------------------------------------*/
/*! @enum   DM_DEV_IO_SPIN_CTRL_T
 *  @brief  Spindle control.
 *  @code
 *  typedef enum {
 *      DM_DEV_IO_SPIN_ON,
 *      DM_DEV_IO_SPIN_OFF
 *  } DM_DEV_IO_SPIN_CTRL_T;
 *  @endcode
 *  @li@c  DM_DEV_IO_SPIN_ON                                 - Not used
 *  @li@c  DM_DEV_IO_SPIN_OFF                                - Not used
 */
/*------------------------------------------------------------------*/
typedef enum {
    DM_DEV_IO_SPIN_ON,
    DM_DEV_IO_SPIN_OFF
} DM_DEV_IO_SPIN_CTRL_T;


/*------------------------------------------------------------------*/
/*! @enum   DM_DEV_IO_TRAY_CTRL_T
 *  @brief  Tray control.
 *  @code
 *  typedef enum {
 *      DM_DEV_IO_TRAY_OPEN,
 *      DM_DEV_IO_TRAY_CLOSE
 *  } DM_DEV_IO_TRAY_CTRL_T;
 *  @endcode
 *  @li@c  DM_DEV_IO_TRAY_OPEN                               - Not used
 *  @li@c  DM_DEV_IO_TRAY_CLOSE                              - Not used
 */
/*------------------------------------------------------------------*/
typedef enum {
    DM_DEV_IO_TRAY_OPEN,
    DM_DEV_IO_TRAY_CLOSE
} DM_DEV_IO_TRAY_CTRL_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_MED_DISC_TOC_HEADER_T
 *  @brief  TOC header information for CD.
 *  @code
 *  typedef struct _DM_DEV_MED_DISC_TOC_HEADER_T
 *  {
 *      UINT8 ui1_disc_type;
 *      UINT8 ui1_total_trk;
 *      UINT8 ui1_total_sen;
 *      UINT8 ui1_flag;
 *      UINT8 ui1_trk_1st;
 *      UINT8 ui1_reserve;
 *  } DM_DEV_MED_DISC_TOC_HEADER_T;
 *  @endcode
 *  @li@c  ui1_disc_type                                     - Not used
 *  @li@c  ui1_total_trk                                     - Not used
 *  @li@c  ui1_total_sen                                     - Not used
 *  @li@c  ui1_flag                                          - Not used
 *  @li@c  ui1_trk_1st                                       - Not used
 *  @li@c  ui1_reserve                                       - Not used
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_MED_DISC_TOC_HEADER_T
{
    UINT8 ui1_disc_type;
    UINT8 ui1_total_trk;
    UINT8 ui1_total_sen;
    UINT8 ui1_flag;
    UINT8 ui1_trk_1st;
    UINT8 ui1_reserve;
} DM_DEV_MED_DISC_TOC_HEADER_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_MED_DISC_TOC_ENTRY_T
 *  @brief  TOC entry information for CD.
 *  @code
 *  typedef struct _DM_DEV_MED_DISC_TOC_ENTRY_T
 *  {
 *      UINT8 ui1_trk_number;
 *      UINT8 ui1_ctrl_addr;
 *      UINT8 ui1_reserve;
 *      UINT8 ui1_a_min;
 *      UINT8 ui1_a_sec;
 *      UINT8 ui1_a_blk;
 *  } DM_DEV_MED_DISC_TOC_ENTRY_T;
 *  @endcode
 *  @li@c  ui1_trk_number                                 - Not used
 *  @li@c  ui1_ctrl_addr                                     - Not used
 *  @li@c  ui1_reserve                                       - Not used
 *  @li@c  ui1_a_min                                         - Not used
 *  @li@c  ui1_a_sec                                         - Not used
 *  @li@c  ui1_a_blk                                         - Not used
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_MED_DISC_TOC_ENTRY_T
{
    UINT8 ui1_trk_number;
    UINT8 ui1_ctrl_addr;
    UINT8 ui1_reserve;
    UINT8 ui1_a_min;
    UINT8 ui1_a_sec;
    UINT8 ui1_a_blk;
} DM_DEV_MED_DISC_TOC_ENTRY_T;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_MED_DISC_SESS_INFO_T
 *  @brief  CD session information.
 *  @code
 *  typedef struct _DM_DEV_MED_DISC_SESS_INFO_T {
 *      UINT32 ui4_start_trk_no;
 *      UINT32 ui4_end_trk_no;
 *  } DM_DEV_MED_DISC_SESS_INFO_T ;
 *  @endcode
 *  @li@c  ui4_start_trk_no                                  - Not used
 *  @li@c  ui4_end_trk_no                                    - Not used
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_MED_DISC_SESS_INFO_T {
    UINT32 ui4_start_trk_no;
    UINT32 ui4_end_trk_no;
} DM_DEV_MED_DISC_SESS_INFO_T ;


/*------------------------------------------------------------------*/
/*! @struct DM_DEV_MED_DISC_TRK_INFO_T
 *  @brief  CD track information.
 *  @code
 *  typedef struct _DM_DEV_MED_DISC_TRK_INFO_T {
 *      BOOL    b_audio_trk;
 *      BOOL    b_Complete;
 *      UINT32  ui4_start_LBA;
 *      UINT32  ui4_sector_ns;
 *  } DM_DEV_MED_DISC_TRK_INFO_T;
 *  @endcode
 *  @li@c  b_audio_trk                                       - Not used
 *  @li@c  b_Complete                                        - Not used
 *  @li@c  ui4_start_LBA                                     - Not used
 *  @li@c  ui4_sector_ns                                     - Not used
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_MED_DISC_TRK_INFO_T {
    BOOL    b_audio_trk;
    BOOL    b_Complete;
    UINT32  ui4_start_LBA;
    UINT32  ui4_sector_ns;
} DM_DEV_MED_DISC_TRK_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum   DM_DEV_MED_DISC_SECTOR_SIZE_TYPE_T
 *  @brief Enumerate stctor size for disc mediums.
 *  @code
 *  typedef enum {
 *      DM_MED_DISC_SEC_SZ_2448,
 *      DM_MED_DISC_SEC_SZ_2336,
 *      DM_MED_DISC_SEC_SZ_2048,
 *      DM_MED_DISC_SEC_SZ_2324,
 *      DM_MED_DISC_SEC_SZ_2328
 *  } DM_DEV_MED_DISC_SECTOR_SIZE_TYPE_T;
 *  @endcode
 *  @li@c  DM_MED_DISC_SEC_SZ_2448                           - Not used
 *  @li@c  DM_MED_DISC_SEC_SZ_2336                           - Not used
 *  @li@c  DM_MED_DISC_SEC_SZ_2048                           - Not used
 *  @li@c  DM_MED_DISC_SEC_SZ_2324                           - Not used
 *  @li@c  DM_MED_DISC_SEC_SZ_2328                           - Not used
 */
/*------------------------------------------------------------------*/
typedef enum {
    DM_MED_DISC_SEC_SZ_2448,
    DM_MED_DISC_SEC_SZ_2336,
    DM_MED_DISC_SEC_SZ_2048,
    DM_MED_DISC_SEC_SZ_2324,
    DM_MED_DISC_SEC_SZ_2328
} DM_DEV_MED_DISC_SECTOR_SIZE_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct DM_DEV_MED_DISC_SECTOR_SIZE_T
 *  @brief Sector size of disc medium.
 *  @code
 *  typedef struct _DM_DEV_MED_DISC_SECTOR_SIZE_T {
 *      UINT32                              ui4_lba;
 *      DM_DEV_MED_DISC_SECTOR_SIZE_TYPE_T  t_sec_sz_type;
 *  } DM_DEV_MED_DISC_SECTOR_SIZE_T;
 *  @endcode
 *  @li@c  ui4_lba                                           - Not used
 *  @li@c  t_sec_sz_type                                     - Not used
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_MED_DISC_SECTOR_SIZE_T {
    UINT32                              ui4_lba;
    DM_DEV_MED_DISC_SECTOR_SIZE_TYPE_T  t_sec_sz_type;
} DM_DEV_MED_DISC_SECTOR_SIZE_T;

/*------------------------------------------------------------------*/
/*! @struct DM_DEV_IO_MED_READ_DVD_STRUCT_T
 *  @brief Not used in DTV platform.
 *  @code
 *  typedef struct _DM_DEV_IO_MED_READ_DVD_STRUCT_T {
 *      UINT32  u4_address;
 *      UINT8   u1_med_type;
 *      UINT8   u1_layer;
 *      UINT8   u1_fmt_cd;
 *      UINT8   u1_agid;
 *      VOID*   pv_buf;
 *      UINT32  u4_buf_len;
 *  } DM_DEV_IO_MED_READ_DVD_STRUCT_T;
 *  @endcode
 *  @li@c  u4_address                                        - Not used
 *  @li@c  u1_med_type                                       - Not used
 *  @li@c  u1_layer                                          - Not used
 *  @li@c  u1_fmt_cd                                         - Not used
 *  @li@c  u1_agid                                           - Not used
 *  @li@c  pv_buf                                           - Not used
 *  @li@c  u4_buf_len                                        - Not used
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_IO_MED_READ_DVD_STRUCT_T {
    UINT32  u4_address;
    UINT8   u1_med_type;
    UINT8   u1_layer;
    UINT8   u1_fmt_cd;
    UINT8   u1_agid;
    VOID*   pv_buf;
    UINT32  u4_buf_len;
} DM_DEV_IO_MED_READ_DVD_STRUCT_T;

/*------------------------------------------------------------------*/
/*! @struct DM_DEV_IO_MED_READ_KCD_T
 *  @brief Not used in DTV platform.
 *  @code
 *  typedef struct _DM_DEV_IO_MED_READ_KCD_T {
 *      UINT8   u1_agid;
 *      VOID*   pv_buf;
 *      UINT32  u4_buf_len;
 *  } DM_DEV_IO_MED_READ_KCD_T;
 *  @endcode
 *  @li@c  u1_agid                                           - Not used
 *  @li@c  pv_buf                                           - Not used
 *  @li@c  u4_buf_len                                        - Not used
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_IO_MED_READ_KCD_T {
    UINT8   u1_agid;
    VOID*   pv_buf;
    UINT32  u4_buf_len;
} DM_DEV_IO_MED_READ_KCD_T;

/*------------------------------------------------------------------*/
/*! @struct DM_DEV_IO_COM_REPORT_KEY_T
 *  @brief Not used in DTV platform.
 *  @code
 *  typedef struct _DM_DEV_IO_COM_REPORT_KEY_T {
 *      UINT32  u4_lba;
 *      UINT8   u1_func;
 *      UINT8   u1_key_class;
 *      UINT8   u1_agid;
 *      UINT8   u1_key_fmt;
 *      VOID*   pv_buf;
 *      UINT32  u4_buf_len;
 *  } DM_DEV_IO_COM_REPORT_KEY_T;
 *  @endcode
 *  @li@c  u4_lba                                            - Not used
 *  @li@c  u1_func                                           - Not used
 *  @li@c  u1_key_class                                      - Not used
 *  @li@c  u1_agid                                           - Not used
 *  @li@c  u1_key_fmt                                        - Not used
 *  @li@c  pv_buf                                           - Not used
 *  @li@c  u4_buf_len                                        - Not used
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_IO_COM_REPORT_KEY_T {
    UINT32  u4_lba;
    UINT8   u1_func;
    UINT8   u1_key_class;
    UINT8   u1_agid;
    UINT8   u1_key_fmt;
    VOID*   pv_buf;
    UINT32  u4_buf_len;
} DM_DEV_IO_COM_REPORT_KEY_T;

/*------------------------------------------------------------------*/
/*! @struct DM_DEV_IO_COM_SEND_KEY_T
 *  @brief Not used in DTV platform.
 *  @code
 *  typedef struct _DM_DEV_IO_COM_SEND_KEY_T {
 *      UINT8   u1_func;
 *      UINT8   u1_key_class;
 *      UINT8   u1_agid;
 *      UINT8   u1_key_fmt;
 *      VOID*   pv_buf;
 *      UINT32  u4_buf_len;
 *  } DM_DEV_IO_COM_SEND_KEY_T;
 *  @endcode
 *  @li@c  u1_func                                           - Not used
 *  @li@c  u1_key_class                                      - Not used
 *  @li@c  u1_agid                                           - Not used
 *  @li@c  u1_key_fmt                                        - Not used
 *  @li@c  pv_buf                                           - Not used
 *  @li@c  u4_buf_len                                        - Not used
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_IO_COM_SEND_KEY_T {
    UINT8   u1_func;
    UINT8   u1_key_class;
    UINT8   u1_agid;
    UINT8   u1_key_fmt;
    VOID*   pv_buf;
    UINT32  u4_buf_len;
} DM_DEV_IO_COM_SEND_KEY_T;

/*------------------------------------------------------------------*/
/*! @struct DM_DEV_IO_COM_GET_FEATURE_T
 *  @brief Not used in DTV platform.
 *  @code
 *  typedef struct _DM_DEV_IO_COM_GET_FEATURE_T {
 *      UINT32  u4_feature_num;
 *      Void*   pv_buf;
 *      UINT32  u4_buf_len;
 *  } DM_DEV_IO_COM_GET_FEATURE_T;
 *  @endcode
 *  @li@c  u4_feature_num                                    - Not used
 *  @li@c  pv_buf                                           - Not used
 *  @li@c  u4_buf_len                                        - Not used
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_IO_COM_GET_FEATURE_T {
    UINT32  u4_feature_num;
    VOID*   pv_buf;
    UINT32  u4_buf_len;
} DM_DEV_IO_COM_GET_FEATURE_T;

/*------------------------------------------------------------------*/
/*! @struct DM_DEV_MED_DISC_READ_CD_T
 *  @brief Not used in DTV platform.
 *  @code
 *  typedef struct _DM_DEV_MED_DISC_READ_CD_T {
 *      UINT32                              ui4_offset;
 *      UINT32                              ui4_count;
 *      DM_DEV_MED_DISC_SECTOR_SIZE_TYPE_T  t_sec_sz;
 *      VOID*                               pv_buf;
 *      UINT32*                             pui4_xfer_cnt;
 *  } DM_DEV_MED_DISC_READ_CD_T;
 *  @endcode
 *  @li@c  ui4_offset                                        - Not used
 *  @li@c  ui4_count                                         - Not used
 *  @li@c  t_sec_sz                                          - Not used
 *  @li@c  pv_buf                                           - Not used
 *  @li@c  pui4_xfer_cnt                                    - Not used
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_MED_DISC_READ_CD_T {
    UINT32                              ui4_offset;
    UINT32                              ui4_count;
    DM_DEV_MED_DISC_SECTOR_SIZE_TYPE_T  t_sec_sz;
    VOID*                              pv_buf;
    UINT32*                             pui4_xfer_cnt;
} DM_DEV_MED_DISC_READ_CD_T;

/*------------------------------------------------------------------*/
/*! @struct DM_DEV_MED_READ_CD_TEXT_T
 *  @brief Not used in DTV platform.
 *  @code
 *  typedef struct _DM_DEV_MED_READ_CD_TEXT_T {
 *      VOID*               pv_buf;
 *      UINT32              u4_buf_len;
 *  } DM_DEV_MED_READ_CD_TEXT_T;
 *  @endcode
 *  @li@c  pv_buf                                           - Not used
 *  @li@c  u4_buf_len                                        - Not used
 */
/*------------------------------------------------------------------*/
typedef struct _DM_DEV_MED_READ_CD_TEXT_T {
    VOID*               pv_buf;
    UINT32              u4_buf_len;
} DM_DEV_MED_READ_CD_TEXT_T;


/*zink CSR BT*/
/* Traffic types in ui1_traffic_type */
#define DM_USB_BT_XFER_CONTROL	    0    /**< USB Control transfer   */
#define DM_USB_BT_XFER_ISOC	        1    /**< USB ISOC transfer      */
#define DM_USB_BT_XFER_BULK	        2    /**< USB Bulk transfer      */
#define DM_USB_BT_XFER_INT        	3    /**< USB Interrupt transfer */

typedef struct _DM_USB_BT_IO_REQ_T DM_USB_BT_IO_REQ_T;

/*------------------------------------------------------------------*/
/*! @brief Callback function for USB bluetooth IO completed.
 *  @param [in] pv_tag - Proprietary information set by IO requester.
 *  @param [in] pt_bt_io - Pointer to the USB bluetooth ISO structure.
 *  @param [out] None.
 *  @see _DM_USB_BT_IO_REQ_T
 */
/*------------------------------------------------------------------*/
typedef void (*dm_usb_bt_io_complete_fnc)(VOID*                    pv_tag, 
                                          DM_USB_BT_IO_REQ_T*      pt_bt_io);


/*------------------------------------------------------------------*/
/*! @struct DM_USB_BT_ISO_T
 *  @brief USB bluetooth ISO structure.
 *  @code
 *  typedef struct _DM_USB_BT_ISO_T 
 *  {
 *      UINT32      ui4_data_length;
 *      UINT32      ui4_actual_data_length;
 *      UINT32      ui4_status;
 *  }DM_USB_BT_ISO_T;
 *  @endcode
 *  @li@c  ui4_data_length          -   Data length in transaction
 *  @li@c  ui4_actual_data_length   -   Actual transfer data length
 *  @li@c  ui4_status               -   Transfer result
 */
/*------------------------------------------------------------------*/
typedef struct _DM_USB_BT_ISO_T 
{
    UINT32      ui4_data_length;
    UINT32      ui4_actual_data_length;
    UINT32      ui4_status;
}DM_USB_BT_ISO_T;

/*------------------------------------------------------------------*/
/*! @struct _DM_USB_BT_IO_REQ_T
 *  @brief USB bluetooth IO request structure.
 *  @code
 *  struct _DM_USB_BT_IO_REQ_T
 *  {
 *      UINT8                       ui1_traffic_type;           
 *      BOOL                        b_is_tx;                    
 *      UINT8*                      aui1_setup_packet;          
 *      UINT8*                      aui1_data_buffer;           
 *      UINT32                      ui4_data_length;            
 *      UINT32                      ui4_actual_data_length;     
 *      UINT32                      ui4_status;                  
 *      UINT32                      ui4_packets_number;
 *      DM_USB_BT_ISO_T*            at_iso_packet; 
 *      dm_usb_bt_io_complete_fnc   pf_bt_io_complete;          
 *      VOID*                       pv_tag;
 *  };
 *  @endcode
 *  @li@c  ui1_traffic_type         -   I: 0 ~ 3: select transfer type.
 *  @li@c  b_is_tx                  -   I: 1: Tx, 0: Rx
 *  @li@c  aui1_setup_packet        -   I: Only for ui1_traffic_type == DM_USB_BT_XFER_CONTROL 8 bytes
 *  @li@c  aui1_data_buffer         -   I/O: data phase data buffer; Tx:data to dev, Rx:dev to client
 *  @li@c  ui4_data_length          -   I: If ui1_traffic_type == MUSB_BT_XFER_CONTROL, ui4_data_length = the size of data at data phase. It does not include setup packet.
 *  @li@c  ui4_actual_data_length   -   O: actual data length gotton
 *  @li@c  ui4_status               -   O: 0: ok, others: fail.
 *  @li@c  ui4_packets_number       -   ISOC transfers packet count
 *  @li@c  at_iso_packet            -   ISOC transfers packet array
 *  @li@c  pf_bt_io_complete        -   callback function
 *  @li@c  pv_tag                   -  Tag
 */
/*------------------------------------------------------------------*/
struct _DM_USB_BT_IO_REQ_T
{
    UINT8                       ui1_traffic_type;
    BOOL                        b_is_tx;

    UINT8*                      aui1_setup_packet;

    UINT8*                      aui1_data_buffer;

    UINT32                      ui4_data_length;

    UINT32                      ui4_actual_data_length;
    UINT32                      ui4_status;

    UINT32                      ui4_packets_number;
    DM_USB_BT_ISO_T*            at_iso_packet; 
    
    dm_usb_bt_io_complete_fnc   pf_bt_io_complete;
    VOID*                       pv_tag;
};

/*------------------------------------------------------------------*/
/*! @struct DM_USB_BT_IO_REQ_NUM_T
 *  @brief USB bluetooth IO request number structure.
 *  @code
 *  typedef struct _DM_USB_BT_IO_REQ_NUM_T
 *  {
 *      BOOL                    b_is_tx;
 *      UINT8                   ui1_traffic_type;           
 *      UINT32                  ui4_free_slot_in_queue;     
 *  }DM_USB_BT_IO_REQ_NUM_T;
 *  @endcode
 *  @li@c  b_is_tx                                           -  Transfer direction, 0:RX, 1:TX
 *  @li@c  ui1_traffic_type                                  -  I: 0 ~ 3: select transfer type
 *  @li@c  ui4_free_slot_in_queue                            -  Free slot in queue
 */
/*------------------------------------------------------------------*/
typedef struct _DM_USB_BT_IO_REQ_NUM_T
{
    BOOL                    b_is_tx;
    UINT8                   ui1_traffic_type;
    UINT32                  ui4_free_slot_in_queue;
}DM_USB_BT_IO_REQ_NUM_T;

/*! @}  */


#endif


