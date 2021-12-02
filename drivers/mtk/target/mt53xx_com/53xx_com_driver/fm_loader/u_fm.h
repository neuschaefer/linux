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
 * $RCSfile: u_fm.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 748ca9ac1805fcf33d028a9d2b82d57a $
 *
 * Description: 
 *         This header file contains File Manager exported constants, macros,
 *         and types.
 *---------------------------------------------------------------------------*/

#ifndef _U_FM_H_
#define _U_FM_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define	FM_MAX_FILE_LEN 		(256 - 1) /* Unicode not including trailing NULL */
#define	FM_MAX_PATH_LEN 		(1152 - 1)
#define FM_ROOT_HANDLE      NULL_HANDLE

/*
 *  Handle types supported by the File Manager.
 */
#define FMT_FILE_DESC   (HT_GROUP_FM + ((HANDLE_TYPE_T) 0))
#define FMT_DIR_LABEL   (HT_GROUP_FM + ((HANDLE_TYPE_T) 1))
#define FMT_ASYNC_READ  (HT_GROUP_FM + ((HANDLE_TYPE_T) 2))
#define FMT_ASYNC_WRITE (HT_GROUP_FM + ((HANDLE_TYPE_T) 3))
#define FMT_PART_TBL    (HT_GROUP_FM + ((HANDLE_TYPE_T) 4))

/*
 *  Device types.
 */
#define FM_DRVT_EEPROM            ((UINT16)  96)
#define FM_DRVT_NOR_FLASH         ((UINT16)  97)
#define FM_DRVT_NAND_FLASH        ((UINT16)  98)
#define FM_DRVT_MEM_CARD          ((UINT16)  99)
#define FM_DRVT_HARD_DISK         ((UINT16) 100)
#define FM_DRVT_USB_MASS_STORAGE  ((UINT16) 101)
#define FM_DRVT_OPTICAL_DISC      ((UINT16) 102)

/*
 *  Return values.
 */
#define	FMR_OK                  ((INT32)  0)
#define	FMR_ARG                 ((INT32) -1)
#define	FMR_HANDLE              ((INT32) -2)
#define	FMR_INVAL               ((INT32) -3)
#define FMR_CORE                ((INT32) -4)
#define	FMR_EXIST               ((INT32) -5)
#define	FMR_NO_ENTRY            ((INT32) -6)
#define	FMR_NOT_DIR             ((INT32) -7)
#define	FMR_IS_DIR              ((INT32) -8)
#define	FMR_DIR_NOT_EMPTY       ((INT32) -9)
#define	FMR_NAME_TOO_LONG       ((INT32) -10)
#define	FMR_FILE_TOO_LARGE      ((INT32) -11)
#define	FMR_BUSY                ((INT32) -12)
#define FMR_EOF                 ((INT32) -13)
#define FMR_LOCK_FAIL           ((INT32) -14)
#define FMR_WOULD_BLOCK         ((INT32) -15)
#define FMR_PERM_DENY           ((INT32) -16)
#define FMR_ACCESS              ((INT32) -17)
#define FMR_NOT_INIT            ((INT32) -18)
#define FMR_ALIGNMENT           ((INT32) -19)
#define FMR_ASYNC_NOT_SUPPORT   ((INT32) -20)
#define FMR_DEVICE_ERROR        ((INT32) -21)
#define	FMR_NO_SUCH_DEVICE      ((INT32) -22)
#define	FMR_NOT_ENOUGH_SPACE    ((INT32) -23)
#define	FMR_FILE_SYSTEM_FULL    ((INT32) -24)
#define	FMR_FILE_SYSTEM_CRASH   ((INT32) -25)
#define FMR_FILE_SYSTEM_OTHER   ((INT32) -26)
#define FMR_CLI_ERROR           ((INT32) -27)
#define FMR_NO_MBR              ((INT32) -28)
#define FMR_EOC                 ((INT32) -29)
#define FMR_FAT_ERROR           ((INT32) -30)
#define FMR_DELETING_ENTRY      ((INT32) -31)
#define FMR_OUT_OF_RANGE        ((INT32) -32)
#define FMR_NOT_IMPLEMENT       ((INT32) -100)

/*
 *  Flags for file descriptor.
 */ 
#define	FM_ACCESS_MODE          ((UINT32) 0003)
#define	FM_READ_ONLY 	        ((UINT32) 0000)
#define	FM_WRITE_ONLY 	        ((UINT32) 0001)
#define	FM_READ_WRITE           ((UINT32) 0002)
#define	FM_OPEN_CREATE          ((UINT32) 0100)
#define FM_OPEN_EXCLUDE         ((UINT32) 0200)
#define	FM_OPEN_TRUNC           ((UINT32) 01000)
#define	FM_OPEN_APPEND          ((UINT32) 02000)
#define FM_NO_CACHE             ((UINT32) 010000)

/*
 *  lseek starting position
 */
#define FM_SEEK_BGN             ((UINT8) 1)
#define FM_SEEK_CUR             ((UINT8) 2)
#define FM_SEEK_END             ((UINT8) 3)

/*
 *  File lock
 */
#define FM_LOCK_READ            ((UINT32) 1)
#define FM_LOCK_WRITE           ((UINT32) 2)
#define FM_LOCK_NO_WAIT         ((UINT32) 4)
#define FM_LOCK_RELEASE         ((UINT32) 8)

/*
 *  File mode.
 */
#define FM_MODE_USR_READ        ((UINT32) 0400)
#define FM_MODE_USR_WRITE       ((UINT32) 0200)
#define FM_MODE_USR_EXEC        ((UINT32) 0100)
#define FM_MODE_GRP_READ        ((UINT32) 0040)
#define FM_MODE_GRP_WRITE       ((UINT32) 0020)
#define FM_MODE_GRP_EXEC        ((UINT32) 0010)
#define FM_MODE_OTH_READ        ((UINT32) 0004)
#define FM_MODE_OTH_WRITE       ((UINT32) 0002)
#define FM_MODE_OTH_EXEC        ((UINT32) 0001)
#define FM_MODE_PERM_MASK       ((UINT32) 0777)
#define FM_MODE_TYPE_FILE       ((UINT32) 0x00010000)
#define FM_MODE_TYPE_DIR        ((UINT32) 0x00020000)
#define FM_MODE_TYPE_BLK        ((UINT32) 0x00040000)
#define FM_MODE_TYPE_CHR        ((UINT32) 0x00080000)
#define FM_MODE_TYPE_LINK       ((UINT32) 0x00100000)
#define FM_MODE_TYPE_MASK       ((UINT32) 0x001f0000)
#define FM_MODE_DEV_TRUE        ((UINT32) 0x00200000)
#define FM_MODE_DEV_VIRT        ((UINT32) 0x00400000)
#define FM_MODE_DEV_MASK        ((UINT32) 0x00600000)

#define FM_IS_FILE(mode) \
    (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_FILE)
#define FM_IS_DIR(mode) \
    (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_DIR)
#define FM_IS_BLK(mode) \
    (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_BLK)
#define FM_IS_CHR(mode) \
    (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_CHR)
#define FM_IS_LINK(mode) \
    (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_LINK)
#define FM_IS_TRUE(mode) \
    (((mode) & FM_MODE_DEV_MASK) == FM_MODE_DEV_TRUE)
#define FM_IS_VIRT(mode) \
    (((mode) & FM_MODE_DEV_MASK) == FM_MODE_DEV_VIRT)
/*
 * Directory information
 */
 typedef struct _FM_DIR_INFO_T
 {
    UINT32  ui4_dir_num;
    UINT32  ui4_file_num;
 } FM_DIR_INFO_T;
/*
 *  File information.
 */
typedef struct _FM_FILE_INFO_T
{
    UINT32      ui4_inode;
    UINT32      ui4_mode;
    UINT32      ui4_uid;
    UINT32      ui4_gid;
    UINT64      ui8_size;
    UINT64      ui8_offset; /* used for chop */
    UINT32      ui4_blk_size;
    UINT64      ui8_blk_cnt;
    TIME_T      ui8_create_time;
    TIME_T      ui8_access_time;
    TIME_T      ui8_modify_time;

    UINT32      ui4_start_lba;
} FM_FILE_INFO_T;

/*
 *  Directory entry.
 */
typedef struct _FM_DIR_ENTRY_T
{
    UINT8   ui1_len;                            /* number of characters */
    CHAR    s_name[(FM_MAX_FILE_LEN + 1) * 4];  /* for UTF-8, enough??? */

    FM_FILE_INFO_T t_file_info;
} FM_DIR_ENTRY_T;

/*
 *  File system types.
 */
typedef enum
{
    FM_TYPE_INVAL  = 0,
    FM_TYPE_ROOTFS,
    FM_TYPE_FAT,
    FM_TYPE_FAT12,
    FM_TYPE_FAT16,
    FM_TYPE_FAT32,
    FM_TYPE_UDF,
    FM_TYPE_ISO9660,
    FM_TYPE_SFS
} FM_FS_TYPE_T;
 
/*
 *  File system information.
 */
typedef struct _FM_FS_INFO_T
{
	FM_FS_TYPE_T    e_type;
	UINT64	        ui8_blk_size;
	UINT64	        ui8_total_blk;
	UINT64	        ui8_free_blk;
	UINT64	        ui8_avail_blk;
	UINT64	        ui8_files;
	UINT16	        ui2_max_name_len;

	UINT32              ui4_alignment;
	UINT32              ui4_min_blk_size;
} FM_FS_INFO_T;

/*
 *  Asynchronous notification conditions.
 */
typedef enum
{
    FM_ASYNC_COND_ABORT_FAIL = -2,
    FM_ASYNC_COND_FAIL      = -1,
    FM_ASYNC_COND_READ_OK   =  1,
    FM_ASYNC_COND_WRITE_OK  =  2,
    FM_ASYNC_COND_ABORT_OK  =   3
} FM_ASYNC_COND_T;

typedef struct _FM_PART_INFO_T
{
    BOOL    b_mnt;

    CHAR    ps_part_name[32];
    CHAR    ps_part_path[32];
    CHAR    ps_mnt_path[32];
} FM_PART_INFO_T;

typedef enum
{
    FM_MNT_OK,
    FM_MNT_UMNT,
    FM_MNT_FAIL
} FM_MNT_COND_T;

typedef VOID (*x_fm_mnt_fct)(
    FM_MNT_COND_T t_cond,
    CHAR  *ps_part_name,
    UINT32  ui4_part_idx,
    CHAR *ps_mnt_path,
    VOID *pv_tag);

typedef struct _FM_MNT_CB_T
{
    x_fm_mnt_fct pf_mnt_fct;
    VOID *pv_tag;
} FM_MNT_CB_T;

typedef VOID (*x_fm_async_fct)(
    HANDLE_T    h_req,
    VOID            *pv_tag,
    FM_ASYNC_COND_T e_async_cond,
    UINT32          ui4_data);
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#endif /* _U_FM_H */
