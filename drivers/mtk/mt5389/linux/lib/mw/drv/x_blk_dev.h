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
 * $RCSfile: x_blk_dev.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains block device related definitions.
 *---------------------------------------------------------------------------*/

#ifndef _X_BLK_DEV_H_
#define _X_BLK_DEV_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "inc/x_common.h"

/*-----------------------------------------------------------------------------
 *  Constants and type definitions
 *---------------------------------------------------------------------------*/

typedef enum
{
    BLKDEV_COND_FAIL     = -1,
    BLKDEV_COND_READ_OK  =  1,
    BLKDEV_COND_WRITE_OK =  2,
    BLKDEV_COND_ERASE_OK =  3,
    BLKDEV_COND_FLUSH_OK =  4
} BLKDEV_COND_T;

typedef enum
{
    BLKDEV_STATUS_ABSENT = 0,
    BLKDEV_STATUS_FAIL   = 1,
    BLKDEV_STATUS_READY  = 2
} BLKDEV_STATUS_T;

typedef enum
{
    BLKDEV_MEMCARD_CF     = 0,
    BLKDEV_MEMCARD_SD     = 1,
    BLKDEV_MEMCARD_MMC    = 2,
    BLKDEV_MEMCARD_MS     = 3,
    BLKDEV_MEMCARD_MS_PRO = 4,
    BLKDEV_MEMCARD_SM     = 5,
    BLKDEV_MEMCARD_XD     = 6,
    BLKDEV_USB_MASS =   100
} BLKDEV_MEMCARD_VARIETY_T;


/*-----------------------------------------------------------------------------
 * Notify function
 *---------------------------------------------------------------------------*/

typedef VOID (*x_blkdev_nfy_fct)(VOID          *pv_nfy_tag,
                                 BLKDEV_COND_T e_nfy_cond,
                                 VOID          *pv_data);

typedef VOID (*x_blkdev_hotswap_nfy_fct)(VOID            *pv_nfy_tag,
                                         BLKDEV_STATUS_T e_nfy_cond,
                                         VOID            *pv_data);

typedef struct _BLKDEV_NFY_INFO_T
{
    VOID             *pv_tag;
    x_blkdev_nfy_fct  pf_blkdev_nfy;
} BLKDEV_NFY_INFO_T;

typedef struct _BLKDEV_HOTSWAP_NFY_INFO_T
{
    VOID            *pv_tag;
    x_blkdev_hotswap_nfy_fct pf_blkdev_nfy;
} BLKDEV_HOTSWAP_NFY_INFO_T;


/*-----------------------------------------------------------------------------
 * Parameter definitions of get/set types
 *---------------------------------------------------------------------------*/

typedef struct _BLKDEV_IO_DATA_T
{
    UINT64            ui8_blk_num;
    UINT32            ui4_count;
    UINT32            ui4_trans_count;
    BOOL              b_ext_buf;
    VOID              *pv_data;
    UINT32            ui4_handle;         /* for abort function, default is 0x00 */
    BLKDEV_NFY_INFO_T t_nfy_info;         /* for individual notification */
} BLKDEV_IO_DATA_T;

typedef struct _BLKDEV_SECTOR_RANGE_INFO_T
{
    UINT32            ui4_start_sector;
    UINT32            ui4_number_of_sectors;
    UINT32            ui4_handle;         /* for abort function, default is 0x00 */
    BLKDEV_NFY_INFO_T t_nfy_info;         /* for individual notification */
} BLKDEV_SECTOR_RANGE_INFO_T;

typedef struct _BLKDEV_SECTOR_SEG_INFO_T
{
    UINT32            ui4_num_sectors;
    UINT32            ui4_sector_size;
} BLKDEV_SECTOR_SEG_INFO_T;

typedef struct _BLKDEV_SECTOR_TBL_INFO_T
{
    UINT32            ui4_num_entries;
    BLKDEV_SECTOR_SEG_INFO_T *pt_sector_info;
} BLKDEV_SECTOR_TBL_INFO_T;

/* Parameter data structure for I/O control */
typedef struct _BLKDEV_IO_CTRL_T {
    UINT32            u4IoctrlType;       /* I/O control types */
    void              *pvIoData;          /* I/O parameter data pointer */
    UINT32            u4IoDataSz;         /* I/O parameter data size */
    UINT32            ui4_handle;         /* for abort function, default is 0x00 */
    BLKDEV_NFY_INFO_T t_nfy_info;         /* for individual notification */
} BLKDEV_IO_CTRL_T;

/* Parameter data structure for abort */
typedef struct _BLKDEV_ABORT_T {
    UINT32            u4Handle;           /* handle of I/O data */
    BOOL              fgEnable;           /* enable or disable driver abort state */
} BLKDEV_ABORT_T;

/* Parameter data structure for flush command */
typedef struct _BLKDEV_FLUSH_DATA_T
{
    UINT32            ui4_handle;         /* for abort function, default is 0x00 */
    BLKDEV_NFY_INFO_T t_nfy_info;         /* for individual notification */
} BLKDEV_FLUSH_DATA_T;

/* Parameter data structure for standby command */
typedef struct _BLKDEV_STANDBY_DATA_T
{
    UINT32            ui4_StandbyTimer;
    UINT32            ui4_handle;         /* for abort function, default is 0x00 */
    BLKDEV_NFY_INFO_T t_nfy_info;         /* for individual notification */
} BLKDEV_STANDBY_DATA_T;

/*-----------------------------------------------------------------------------
 * Block device flags
 *---------------------------------------------------------------------------*/
#define BLKDEV_FLAG_STANDBY ((UINT32) 0x00010000)
#define BLKDEV_FLAG_HOTSWAP ((UINT32) 0x00020000)
#define BLKDEV_FLAG_ASYNC_READ  ((UINT32) 0x00030000)
#define BLKDEV_FLAG_ASYNC_WRITE ((UINT32) 0x00040000)

/*-----------------------------------------------------------------------------
 * IO control types
 *---------------------------------------------------------------------------*/
#define BLK_DEV_IO_CTRL_TYPE_ENABLE_WP ((UINT32) 0x100)

/*-----------------------------------------------------------------------------
 * Get types
 *---------------------------------------------------------------------------*/
#define BLKDEV_GET_TYPE_SIZE \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define BLKDEV_GET_TYPE_NUMBER_OF_SECTORS \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define BLKDEV_GET_TYPE_SECTOR_SIZE \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define BLKDEV_GET_TYPE_MAX_TRANS_SECTORS \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define BLKDEV_GET_TYPE_MAX_IO_REQUEST \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))
#define BLKDEV_GET_TYPE_SECTOR_TBL_INFO \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))
#define BLKDEV_GET_TYPE_ALIGNMENT \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 6))
#define BLKDEV_GET_TYPE_STANDBY \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 7))
#define BLKDEV_GET_TYPE_STATUS \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 8))
#define BLKDEV_GET_TYPE_MEMCARD_VARIETY \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 9))
#define BLKDEV_GET_TYPE_MODEL \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 10))
#define BLKDEV_GET_TYPE_BEST_TRANS_SECTORS \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 11))    /* default value: 1 */
#define BLKDEV_GET_TYPE_MIN_READ_SECTORS \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 12))    /* default value: 1 */
#define BLKDEV_GET_TYPE_MIN_WRITE_SECTORS \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 13))    /* default value: 1 */
#define BLKDEV_GET_TYPE_WRITE_PROTECT \
     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 14))    
#define BLKDEV_GET_TYPE_PROPRIETARY_PROPERTY \
    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0x80))  /* get type value over 0x80 should be defined in device proprietary header file */

/*-----------------------------------------------------------------------------
 * Set types
 *---------------------------------------------------------------------------*/
#define BLKDEV_SET_TYPE_READ \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0))
#define BLKDEV_SET_TYPE_WRITE \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define BLKDEV_SET_TYPE_ERASE \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
#define BLKDEV_SET_TYPE_FLUSH \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))
#define BLKDEV_SET_TYPE_NFY_FCT \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
#define BLKDEV_SET_TYPE_STANDBY \
	  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5))
#define BLKDEV_SET_TYPE_HOTSWAP_NFY_FCT \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6))
#define BLKDEV_SET_TYPE_IO_CTRL \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7))
#define BLKDEV_SET_TYPE_ABORT \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 8))

#define BLKDEV_SET_TYPE_PROPRIETARY_PROPERTY \
    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0x80))  /* set type value over 0x80 should be defined in device proprietary header file */

#endif /* _X_BLK_DEV_H_ */
