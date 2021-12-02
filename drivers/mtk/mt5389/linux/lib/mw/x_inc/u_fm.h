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
/*! @file u_fm.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains File Manager exported constants, macros,
 *         and types.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_FM_H_
#define _U_FM_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"

/*!
 * @defgroup    groupMW_FM File Manager
 * @ingroup     groupMW
 * @brief    This header file contains File Manager exported constants, macros, and types.
 * 
 * @see groupMW
 *
 * @{
 * */



/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define FM_MAX_FILE_LEN         (256 - 1)     /**< Maximum length of a file's name, unicode not including trailing NULL */
#define FM_MAX_PATH_LEN         (1152 - 1)    /**< Maximum length of a file path       */
#define FM_ROOT_HANDLE          NULL_HANDLE   /**< Root handle used in FM       */
#define FM_MAX_VOLUME_LABEL_LEN (128)         /**< FAT:11bytes, NTFS:32 unicode chracters? >  */

/*
 *  Handle types supported by the File Manager.
 */
#define FMT_FILE_DESC   (HT_GROUP_FM + ((HANDLE_TYPE_T) 0))    /**< FD handle       */
#define FMT_DIR_LABEL   (HT_GROUP_FM + ((HANDLE_TYPE_T) 1))    /**< Dir label handle       */
#define FMT_ASYNC_READ  (HT_GROUP_FM + ((HANDLE_TYPE_T) 2))    /**< Async read request handle       */
#define FMT_ASYNC_WRITE (HT_GROUP_FM + ((HANDLE_TYPE_T) 3))    /**< Async write request handle       */
#define FMT_PART_TBL    (HT_GROUP_FM + ((HANDLE_TYPE_T) 4))    /**< Partition table handle       */
#define FMT_ASYNC_COPY  (HT_GROUP_FM + ((HANDLE_TYPE_T) 5))

#define FMT_SPECIAL_START   (HT_GROUP_FM + ((HANDLE_TYPE_T) 6))    /**< Used by sub-system in FM       */

/*
 *  Device types.
 */
#define FM_DRVT_EEPROM            ((UINT16)  96)    /**<EEPROM        */
#define FM_DRVT_NOR_FLASH         ((UINT16)  97)    /**<Nor flash        */
#define FM_DRVT_NAND_FLASH        ((UINT16)  98)    /**<Nand flash        */
#define FM_DRVT_MEM_CARD          ((UINT16)  99)    /**<Memory card        */
#define FM_DRVT_HARD_DISK         ((UINT16) 100)    /**<Hard disk        */
#define FM_DRVT_USB_MASS_STORAGE  ((UINT16) 101)    /**<USB mass stroage        */
#define FM_DRVT_OPTICAL_DISC      ((UINT16) 102)    /**<Optical disk        */
#define FM_DRVT_PTP_MTP_DEVICE    ((UINT16) 103)    /**<MTP device        */
#define FM_DRVT_DLNA_DMS          ((UINT16) 104)    /**<DLNA DMS        */
#define FM_DRVT_SMB               ((UINT16) 105)    /**<My Network Places */

/*
 *  Return values.
 */
#define FMR_OK                  ((INT32)  0)    /**<Everything is OK        */
#define FMR_ARG                 ((INT32) -1)    /**<Invalid parameters        */
#define FMR_HANDLE              ((INT32) -2)    /**<Invalid handle        */
#define FMR_INVAL               ((INT32) -3)    /**<Invalid states        */
#define FMR_CORE                ((INT32) -4)    /**<Internal error        */
#define FMR_EXIST               ((INT32) -5)    /**<File/dir/device is existed        */
#define FMR_NO_ENTRY            ((INT32) -6)    /**<File/dir/device is not existed        */
#define FMR_NOT_DIR             ((INT32) -7)    /**<Path is not a dir        */
#define FMR_IS_DIR              ((INT32) -8)    /**<Path is a dir        */
#define FMR_DIR_NOT_EMPTY       ((INT32) -9)    /**<Dir is not empty        */
#define FMR_NAME_TOO_LONG       ((INT32) -10)    /**<File name is too long        */
#define FMR_FILE_TOO_LARGE      ((INT32) -11)    /**<File size is too large        */
#define FMR_BUSY                ((INT32) -12)    /**<File/dir/device is busy      */
#define FMR_EOF                 ((INT32) -13)    /**<End of file        */
#define FMR_LOCK_FAIL           ((INT32) -14)    /**<Failed to lock        */
#define FMR_WOULD_BLOCK         ((INT32) -15)    /**<Would be blocked if wait when lock       */
#define FMR_PERM_DENY           ((INT32) -16)    /**<Permission denied        */
#define FMR_ACCESS              ((INT32) -17)    /**<No right to access        */
#define FMR_NOT_INIT            ((INT32) -18)    /**<Not inited        */
#define FMR_ALIGNMENT           ((INT32) -19)    /**<Alignment error        */
#define FMR_ASYNC_NOT_SUPPORT   ((INT32) -20)    /**<Async IO is not supported        */
#define FMR_DEVICE_ERROR        ((INT32) -21)    /**<Device error        */
#define FMR_NO_SUCH_DEVICE      ((INT32) -22)    /**<No such device        */
#define FMR_NOT_ENOUGH_SPACE    ((INT32) -23)    /**<Not enough space        */
#define FMR_FILE_SYSTEM_FULL    ((INT32) -24)    /**<File system is full, no free space        */
#define FMR_FILE_SYSTEM_CRASH   ((INT32) -25)    /**<File system is crashed        */
#define FMR_FILE_SYSTEM_OTHER   ((INT32) -26)    /**<Source and destination are mounted on different file system        */
#define FMR_CLI_ERROR           ((INT32) -27)    /**<CLI commands excuted error        */
#define FMR_NO_MBR              ((INT32) -28)    /**<Device doesn't have MBR        */
#define FMR_EOC                 ((INT32) -29)    /**<End of cluster        */
#define FMR_FAT_ERROR           ((INT32) -30)    /**<FAT error        */
#define FMR_DELETING_ENTRY      ((INT32) -31)    /**<Access a deleting entry        */
#define FMR_OUT_OF_RANGE        ((INT32) -32)    /**<IO is out of range        */
#define FMR_OVER_VALID_RANGE    ((INT32) -33)    /**<IO is over valid range        */
#define FMR_ABNORMAL_ENDED      ((INT32) -34)    /**<Abnormal ended        */
#define FMR_BLANK_SECTOR        ((INT32) -35)    /**<Blank sector in disc        */
#define FMR_CMD_TIMEOUT         ((INT32) -36)    /**<Command executed timeout        */
#define FMR_ENTRY_RECLAIMED     ((INT32) -37)    /**<Entry is reclaimed        */
#define FMR_AUTH_FAIL           ((INT32) -38)    /**<Juest for MyNetworkPlaces: need usr/passwd */
#define FMR_SRC_SAME_AS_DEST    ((INT32) -39)    /**<SRC same as DEST */
#define FMR_ABORTED             ((INT32) -40)    /**<SRC same as DEST */
#define FMR_NFY_SETTED          ((INT32) -41)    /**<Notiyf function is already setted        */
#define FMR_ERROR               ((INT32) -99)    /**<Other error        */
#define FMR_NOT_IMPLEMENT       ((INT32) -100)    /**<This function is not implemented        */


/*
 *  Flags for file descriptor.
 */ 
#define FM_ACCESS_MODE          ((UINT32) 0003)    /**<Access mode mask        */
#define FM_READ_ONLY            ((UINT32) 0000)    /**<read only        */
#define FM_WRITE_ONLY           ((UINT32) 0001)    /**<write only        */
#define FM_READ_WRITE           ((UINT32) 0002)    /**<read write        */
#define FM_OPEN_CREATE          ((UINT32) 0100)    /**<open create        */
#define FM_OPEN_EXCLUDE         ((UINT32) 0200)    /**<open exclude        */
#define FM_OPEN_TRUNC           ((UINT32) 01000)    /**<open truncate        */
#define FM_OPEN_APPEND          ((UINT32) 02000)    /**<open append        */
#define FM_NO_CACHE             ((UINT32) 010000)    /**<Not cached        */

/*
 *  lseek starting position
 */
#define FM_SEEK_BGN             ((UINT8) 1)    /**<Seek from begin of the file/dir        */
#define FM_SEEK_CUR             ((UINT8) 2)    /**<Seek from current position        */
#define FM_SEEK_END             ((UINT8) 3)    /**<Seek from end of the file/dir       */
#define FM_SEEK_CHK             ((UINT8) 0x80) /**<Check if seek in file is slow or not        */

/*
 *  File lock
 */
#define FM_LOCK_READ            ((UINT32) 1)    /**<Locked to read        */
#define FM_LOCK_WRITE           ((UINT32) 2)    /**<Locked to write        */
#define FM_LOCK_NO_WAIT         ((UINT32) 4)    /**<No wait to lock        */
#define FM_LOCK_RELEASE         ((UINT32) 8)    /**<Release lock        */

/*
 *  File mode.
 */
#define FM_MODE_USR_READ        ((UINT32) 0400)    /**<User can read        */
#define FM_MODE_USR_WRITE       ((UINT32) 0200)    /**<User can write        */
#define FM_MODE_USR_EXEC        ((UINT32) 0100)    /**<User can execute        */
#define FM_MODE_GRP_READ        ((UINT32) 0040)    /**<Group can read        */
#define FM_MODE_GRP_WRITE       ((UINT32) 0020)    /**<Group can write        */
#define FM_MODE_GRP_EXEC        ((UINT32) 0010)    /**<Group can execute        */
#define FM_MODE_OTH_READ        ((UINT32) 0004)    /**<Others can read       */
#define FM_MODE_OTH_WRITE       ((UINT32) 0002)    /**<Others can write        */
#define FM_MODE_OTH_EXEC        ((UINT32) 0001)    /**<Others can execute        */
#define FM_MODE_PERM_MASK       ((UINT32) 0777)    /**<RW permission mask        */
#define FM_MODE_TYPE_FILE       ((UINT32) 0x00010000)    /**<File        */
#define FM_MODE_TYPE_DIR        ((UINT32) 0x00020000)    /**<DIR        */
#define FM_MODE_TYPE_BLK        ((UINT32) 0x00040000)    /**<Block device        */
#define FM_MODE_TYPE_CHR        ((UINT32) 0x00080000)    /**<Character device        */
#define FM_MODE_TYPE_LINK       ((UINT32) 0x00100000)    /**<Link file        */
#define FM_MODE_TYPE_PTP_MTP    ((UINT32) 0x00200000)    /**<MTP        */
#define FM_MODE_TYPE_DLNA_DMS   ((UINT32) 0x00400000)    /**<DLNA DMS        */
#define FM_MODE_TYPE_HIDDEN     ((UINT32) 0x00800000)    /**<Hidden file/dir        */
#define FM_MODE_TYPE_SMB        ((UINT32) 0x01000000)    /**<My Network Places        */
#define FM_MODE_TYPE_MASK       ((UINT32) 0x017F0000)    /**<File mode mask        */
#define FM_MODE_DEV_TRUE        ((UINT32) 0x02000000)    /**<True device        */
#define FM_MODE_DEV_VIRT        ((UINT32) 0x04000000)    /**<Virtual device        */
#define FM_MODE_DEV_MASK        ((UINT32) 0x06000000)    /**<Device mode mask        */

#ifndef SN_FLASH_USE_NAND
#define FM_MODE_TYPE            (FM_MODE_TYPE_CHR)  /* for NOR flash */
#else
#define FM_MODE_TYPE            (FM_MODE_TYPE_BLK)  /* for NAND flash */
#endif

#define FM_IS_FILE(mode) \
    (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_FILE)    /**<Is file?        */
#define FM_IS_DIR(mode) \
    (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_DIR)    /**<Is dir?        */
#define FM_IS_BLK(mode) \
    (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_BLK)    /**<Is block device?        */
#define FM_IS_CHR(mode) \
    (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_CHR)    /**<Is character device?        */
#define FM_IS_LINK(mode) \
    (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_LINK)    /**<Is link file/dir?        */
#define FM_IS_PTP_MTP(mode) \
    (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_PTP_MTP)    /**<Is MTP?        */
#define FM_IS_DLNA_DMS(mode) \
        (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_DLNA_DMS)    /**<Is DLNA DMS?        */
#define FM_IS_SMB(mode) \
        (((mode) & FM_MODE_TYPE_MASK) == FM_MODE_TYPE_SMB)    /**<Is MyNetworkPlaces?        */
#define FM_IS_TRUE(mode) \
    (((mode) & FM_MODE_DEV_MASK) == FM_MODE_DEV_TRUE)    /**<Is true device?        */
#define FM_IS_VIRT(mode) \
    (((mode) & FM_MODE_DEV_MASK) == FM_MODE_DEV_VIRT)    /**<Is virtual device?        */
#define FM_IS_HIDDEN(mode) \
    (((mode) & FM_MODE_TYPE_HIDDEN) == FM_MODE_TYPE_HIDDEN)    /**<Is hidden file/dir?        */


/*------------------------------------------------------------------*/
/*! @enum   FM_IO_CTRL_COMMAND
 *  @brief  FM IO Control Type
 *  @code
 *  typedef enum 
 *  {
 *      FM_IO_CTRL_ENABLE_FLASH_WRITE_PROTECTION = 0,
 *      FM_IO_CTRL_TYPE_END
 *  } FM_IO_CTRL_COMMAND;
 *  @endcode
 *  @li@c  FM_IO_CTRL_ENABLE_FLASH_WRITE_PROTECTION          -  For write protection, DATA TYPE: BOOL, 
 *                                                                                                     True means Enable, while FALSE means DISABLE
 *  @li@c  FM_IO_CTRL_TYPE_END                               -
 */
/*------------------------------------------------------------------*/
typedef enum 
{
    FM_IO_CTRL_ENABLE_FLASH_WRITE_PROTECTION = 0,

    FM_IO_CTRL_FLASH_BULK_WRITE = 1,

    FM_IO_CTRL_TYPE_END
} FM_IO_CTRL_COMMAND;

/* Flash bulk write */
typedef struct _FM_FLASH_PART_BULK_WRITE_T
{
    UINT32    ui4_offset;
    UINT32    ui4_size;
    UINT8     ui1_value;
} FM_FLASH_PART_BULK_WRITE_T;



/*------------------------------------------------------------------*/
/*! @struct FM_DIR_INFO_T
 *  @brief  Directory information
 *  @code
 *   typedef struct _FM_DIR_INFO_T
 *   {
 *      UINT32  ui4_dir_num;
 *      UINT32  ui4_file_num;
 *   } FM_DIR_INFO_T;
 *  @endcode
 *  @li@c  ui4_dir_num                                       - Number of child directories
 *  @li@c  ui4_file_num                                      - Number of child files
 */
/*------------------------------------------------------------------*/
 typedef struct _FM_DIR_INFO_T
 {
    UINT32  ui4_dir_num;
    UINT32  ui4_file_num;
 } FM_DIR_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct FM_FILE_INFO_T
 *  @brief  File information.
 *  @code
 *  typedef struct _FM_FILE_INFO_T
 *  {
 *      UINT32      ui4_inode;
 *      UINT32      ui4_mode;
 *      UINT32      ui4_uid;
 *      UINT32      ui4_gid;
 *      UINT64      ui8_size;
 *      UINT64      ui8_offset; 
 *      UINT32      ui4_blk_size;
 *      UINT64      ui8_blk_cnt;
 *      TIME_T      ui8_create_time;
 *      TIME_T      ui8_access_time;
 *      TIME_T      ui8_modify_time;
 *      UINT32      ui4_start_lba;
 *      BOOL        b_copy_protected;
 *  } FM_FILE_INFO_T;
 *  @endcode
 *  @li@c  ui4_inode                                        - Inode
 *  @li@c  ui4_mode                                        - Mode
 *  @li@c  ui4_uid                                           - UID
 *  @li@c  ui4_gid                                           - GID
 *  @li@c  ui8_size                                          - Size
 *  @li@c  ui8_offset                                        - Offset used for chop
 *  @li@c  ui4_blk_size                                    - Block size
 *  @li@c  ui8_blk_cnt                                     - Block count
 *  @li@c  ui8_create_time                               - Created time
 *  @li@c  ui8_access_time                              - Last accessed time
 *  @li@c  ui8_modify_time                             - Last modified time
 *  @li@c  ui4_start_lba                                   - Start LBA
 *  @li@c  b_copy_protected                            - Copy protected or not
 */
/*------------------------------------------------------------------*/
typedef struct _FM_FILE_INFO_T
{
    UINT32      ui4_inode;
    UINT32      ui4_mode;
    UINT32      ui4_uid;
    UINT32      ui4_gid;
    UINT64      ui8_size;
    UINT64      ui8_offset;
    UINT32      ui4_blk_size;
    UINT64      ui8_blk_cnt;
    TIME_T      ui8_create_time;
    TIME_T      ui8_access_time;
    TIME_T      ui8_modify_time;

    UINT32      ui4_start_lba;
    BOOL        b_copy_protected;
} FM_FILE_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct FM_DIR_ENTRY_T
 *  @brief  Directory entry.
 *  @code
 *  typedef struct _FM_DIR_ENTRY_T
 *  {
 *      UINT8   ui1_len;                            
 *      CHAR    s_name[(FM_MAX_FILE_LEN + 1) * 4];  
 *      UINT64  ui8_dirent_ofst;
 *      UINT32  ui4_dirent_len;
 *      FM_FILE_INFO_T t_file_info;
 *  } FM_DIR_ENTRY_T;
 *  @endcode
 *  @li@c  ui1_len                                           - number of characters 
 *  @li@c  s_name[(FM_MAX_FILE_LEN + 1) * 4]    - Dir entry's name, for UTF-8, enough???
 *  @li@c  ui8_dirent_ofst                                   - Offset of the dir entry
 *  @li@c  ui4_dirent_len                                    - Length of the dir entry
 *  @li@c  t_file_info                                       - File info
 */
/*------------------------------------------------------------------*/
typedef struct _FM_DIR_ENTRY_T
{
    UINT8   ui1_len;
    CHAR    s_name[(FM_MAX_FILE_LEN + 1) * 4];

    UINT64  ui8_dirent_ofst;
    UINT32  ui4_dirent_len;

    FM_FILE_INFO_T t_file_info;
} FM_DIR_ENTRY_T;


/*------------------------------------------------------------------*/
/*! @enum   FM_FS_TYPE_T
 *  @brief  File system types.
 *  @code
 *  typedef enum
 *  {
 *      FM_TYPE_INVAL  = 0,
 *      FM_TYPE_ROOTFS,
 *      FM_TYPE_FAT,
 *      FM_TYPE_FAT12,
 *      FM_TYPE_FAT16,
 *      FM_TYPE_FAT32,
 *      FM_TYPE_UDF,
 *      FM_TYPE_ISO9660,
 *      FM_TYPE_SFS,
 *      FM_TYPE_MTP,
 *      FM_TYPE_NTFS,
 *      FM_TYPE_DLNA_DMS,
 *      FM_TYPE_SMB
 *  } FM_FS_TYPE_T;
 *  @endcode
 *  @li@c  FM_TYPE_INVAL                                    - Invalid file system
 *  @li@c  FM_TYPE_ROOTFS                                 - Rootfs in FM
 *  @li@c  FM_TYPE_FAT                                       - FAT
 *  @li@c  FM_TYPE_FAT12                                   - FAT12
 *  @li@c  FM_TYPE_FAT16                                   - FAT16
 *  @li@c  FM_TYPE_FAT32                                   - FAT32
 *  @li@c  FM_TYPE_UDF                                      - UDF
 *  @li@c  FM_TYPE_ISO9660                                - ISO9660
 *  @li@c  FM_TYPE_SFS                                       - SFS
 *  @li@c  FM_TYPE_MTP                                       - MTP
 *  @li@c  FM_TYPE_NTFS                                      - NTFS
 *  @li@c  FM_TYPE_DLNA_DMS                             - DLNA DMS
 *  @li@c  FM_TYPE_SMB                                  - MyNetworkPlaces
 *  @li@c  FM_TYPE_LINUX                                    - Linux FS
 */
/*------------------------------------------------------------------*/
typedef enum
{
    FM_TYPE_INVAL  = 0,
    FM_TYPE_ROOTFS,
    FM_TYPE_FAT,
    FM_TYPE_FAT12,
    FM_TYPE_FAT16,
    FM_TYPE_FAT32,
    FM_TYPE_FAT64,
    FM_TYPE_UDF,
    FM_TYPE_ISO9660,
    FM_TYPE_SFS,
    FM_TYPE_MTP,
    FM_TYPE_NTFS,
    FM_TYPE_DLNA_DMS,
    FM_TYPE_LINUX,
    FM_TYPE_SMB
} FM_FS_TYPE_T;

typedef enum FM_FS_RW_ATTR_T
{
    FM_FS_RW_ATTR_READ_ONLY,
    FM_FS_RW_ATTR_WRITE_ONLY,
    FM_FS_RW_ATTR_READ_WRITE,
    FM_FS_RW_ATTR_RW_DENY
} FM_FS_RW_ATTR_T;

/*------------------------------------------------------------------*/
/*! @struct FM_FS_INFO_T
 *  @brief  File system information.
 *  @code
 *  typedef struct _FM_FS_INFO_T
 *  {
 *      FM_FS_TYPE_T    e_type;
 *      UINT64          ui8_blk_size;
 *      UINT64          ui8_total_blk;
 *      UINT64          ui8_free_blk;
 *      UINT64          ui8_avail_blk;
 *      UINT64          ui8_files;
 *      UINT16          ui2_max_name_len;
 *      UINT32          ui4_alignment;
 *      UINT32          ui4_min_blk_size;
 *  } FM_FS_INFO_T;
 *  @endcode
 *  @li@c  e_type                                           - File system type
 *  @li@c  ui8_blk_size                                   - Block size
 *  @li@c  ui8_total_blk                                  - Total blocks
 *  @li@c  ui8_free_blk                                   - Free blocks
 *  @li@c  ui8_avail_blk                                  - Available blocks
 *  @li@c  ui8_files                                         - Files, only valid for ROOTFS
 *  @li@c  ui2_max_name_len                         - Maximum name length 
 *  @li@c  ui4_alignment                                - Alignment bytes
 *  @li@c  ui4_min_blk_size                            - Minimum block size
 */
/*------------------------------------------------------------------*/
typedef struct _FM_FS_INFO_T
{
    FM_FS_TYPE_T    e_type;
    UINT64          ui8_blk_size;
    UINT64          ui8_total_blk;
    UINT64          ui8_free_blk;
    UINT64          ui8_avail_blk;
    UINT64          ui8_files;
    UINT16          ui2_max_name_len;

    UINT32          ui4_alignment;
    UINT32          ui4_min_blk_size;

        FM_FS_RW_ATTR_T e_rw_attr;
} FM_FS_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum   FM_MNT_TYPE_T
 *  @brief  File system selection types.
 *  @code
 *  typedef enum
 *  {
 *      FM_MNT_TYPE_AUTO  = 0,
 *      FM_MNT_TYPE_FAT,
 *      FM_MNT_TYPE_UDF,
 *      FM_MNT_TYPE_ISO9660,
 *      FM_MNT_TYPE_MTP,
 *      FM_MNT_TYPE_NTFS,
 *      FM_MNT_TYPE_DLNA_DMS,
 *      FM_MNT_TYPE_SMB
 *  } FM_MNT_TYPE_T;
 *  @endcode
 *  @li@c  FM_MNT_TYPE_AUTO                                  - Auto mount
 *  @li@c  FM_MNT_TYPE_FAT                                   - Use FAT to mount
 *  @li@c  FM_MNT_TYPE_UDF                                   -Use UDF to mount
 *  @li@c  FM_MNT_TYPE_ISO9660                            - Use ISO9660 to mount
 *  @li@c  FM_MNT_TYPE_MTP                                   - Use MTP to mount
 *  @li@c  FM_MNT_TYPE_NTFS                                  - Use NTFS to mount
 *  @li@c  FM_MNT_TYPE_DLNA_DMS                         - Use dlna to mount
 *  @li@c  FM_MNT_TYPE_SMB                              - Use MyNetworkPlaces to mount
 */
/*------------------------------------------------------------------*/
typedef enum
{
    FM_MNT_TYPE_AUTO  = 0,
    FM_MNT_TYPE_FAT,
    FM_MNT_TYPE_UDF,
    FM_MNT_TYPE_ISO9660,
    FM_MNT_TYPE_MTP,
    FM_MNT_TYPE_NTFS,
    FM_MNT_TYPE_DLNA_DMS,
    FM_MNT_TYPE_SMB
} FM_MNT_TYPE_T;


/*------------------------------------------------------------------*/
/*! @struct FM_MNT_PARM_T
 *  @brief   File system mount parameter.
 *  @code
 *  typedef struct _FM_MNT_PARM_T
 *  {
 *      FM_MNT_TYPE_T     e_type;
 *      UINT32            ui4_sess_no;
 *    BOOL              b_attach_mounted;
 *  } FM_MNT_PARM_T;
 *  @endcode
 *  @li@c  e_type                                            - Mount type
 *  @li@c  ui4_sess_no                                    - Session NO.
 *  @li@c  b_attach_mounted                          - Used for linux, attach a mounted device
 */
/*------------------------------------------------------------------*/
typedef struct _FM_MNT_PARM_T
{
    FM_MNT_TYPE_T     e_type;
    UINT32            ui4_sess_no;

    BOOL              b_attach_mounted;
    
} FM_MNT_PARM_T;

/*------------------------------------------------------------------*/
/*! @enum   FM_ASYNC_COND_T
 *  @brief  Asynchronous notification conditions.
 *  @code
 *  typedef enum
 *  {
 *      FM_ASYNC_COND_BLANK_SECTOR  = -8,
 *      FM_ASYNC_COND_CMD_TIMEOUT   = -4,
 *      FM_ASYNC_COND_READ_TIMEOUT  = -3,   
 *      FM_ASYNC_COND_ABORT_FAIL    = -2,
 *      FM_ASYNC_COND_FAIL          = -1,
 *      FM_ASYNC_COND_READ_OK       =  1,
 *      FM_ASYNC_COND_WRITE_OK      =  2,
 *      FM_ASYNC_COND_ABORT_OK      =  3,
 *      FM_ASYNC_COND_EOF           =  4,
 *      FM_ASYNC_COND_DLNA_NO_RESPONSE = 5,
 *      FM_ASYNC_COND_SMB_NO_RESPONSE = 6
 *  } FM_ASYNC_COND_T;
 *  @endcode
 *  @li@c  FM_ASYNC_COND_BLANK_SECTOR                       - BLANK_SECTOR
 *  @li@c  FM_ASYNC_COND_CMD_TIMEOUT                         - CMD_TIMEOUT
 *  @li@c  FM_ASYNC_COND_READ_TIMEOUT                        - READ_TIMEOUT
 *  @li@c  FM_ASYNC_COND_ABORT_FAIL                             - ABORT_FAIL
 *  @li@c  FM_ASYNC_COND_FAIL                                        - FAIL  
 *  @li@c  FM_ASYNC_COND_READ_OK                                 - READ OK
 *  @li@c  FM_ASYNC_COND_WRITE_OK                               - WRITE_OK
 *  @li@c  FM_ASYNC_COND_ABORT_OK                               - ABORT_OK
 *  @li@c  FM_ASYNC_COND_EOF                                         - EOF
 *  @li@c  FM_ASYNC_COND_DLNA_NO_RESPONSE               - DLNA no response
 *  @li@c  FM_ASYNC_COND_SMB_NO_RESPONSE               - MyNetworkPlaces no response
 */
/*------------------------------------------------------------------*/
typedef enum
{
    FM_ASYNC_COND_BLANK_SECTOR  = -8,
    FM_ASYNC_COND_CMD_TIMEOUT   = -4,
    FM_ASYNC_COND_READ_TIMEOUT  = -3,   /* to notify network http timeout */
    FM_ASYNC_COND_ABORT_FAIL    = -2,
    FM_ASYNC_COND_FAIL          = -1,
    FM_ASYNC_COND_READ_OK       =  1,
    FM_ASYNC_COND_WRITE_OK      =  2,
    FM_ASYNC_COND_ABORT_OK      =  3,
    FM_ASYNC_COND_EOF           =  4,
    FM_ASYNC_COND_DLNA_NO_RESPONSE = 5,
    FM_ASYNC_COND_SMB_NO_RESPONSE = 6
} FM_ASYNC_COND_T;


/*------------------------------------------------------------------*/
/*! @struct FM_PART_INFO_T
 *  @brief  Partition info
 *  @code
 *  typedef struct _FM_PART_INFO_T
 *  {
 *      BOOL    b_mnt;
 *      CHAR    ps_part_name[32];
 *      CHAR    ps_part_path[32];
 *      CHAR    ps_mnt_path[32];
 *  } FM_PART_INFO_T;
 *  @endcode
 *  @li@c  b_mnt                                             - Mounted or not
 *  @li@c  ps_part_name[32]                           - partition name
 *  @li@c  ps_part_path[32]                             - partition path
 *  @li@c  ps_mnt_path[32]                             - Mount point path
 */
/*------------------------------------------------------------------*/
typedef struct _FM_PART_INFO_T
{
    BOOL    b_mnt;

    CHAR    ps_part_name[32];
    CHAR    ps_part_path[32];
    CHAR    ps_mnt_path[32];
} FM_PART_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum   FM_MNT_COND_T
 *  @brief  Mount conditions
 *  @code
 *  typedef enum
 *  {
 *      FM_MNT_OK,
 *      FM_MNT_UMNT,
 *      FM_MNT_FAIL
 *  } FM_MNT_COND_T;
 *  @endcode
 *  @li@c  FM_MNT_OK                                         - Mounted OK
 *  @li@c  FM_MNT_UMNT                                     - Umounted
 *  @li@c  FM_MNT_FAIL                                       - Mount failed
 */
/*------------------------------------------------------------------*/
typedef enum
{
    FM_MNT_OK,
    FM_MNT_UMNT,
    FM_MNT_FAIL
} FM_MNT_COND_T;

/*------------------------------------------------------------------*/
/*! @brief  callback of mount/umount
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_fm_mnt_fct)(
    FM_MNT_COND_T   t_cond,
    CHAR            *ps_part_name,
    UINT32          ui4_part_idx,
    CHAR            *ps_mnt_path,
    VOID            *pv_tag);

/*------------------------------------------------------------------*/
/*! @enum   FS_PARTITION_T
 *  @brief  Partition ID.
 *  @code
 *  typedef enum
 *  {
 *      FS_PARTITION_BASIC = 0,
 *      FS_PARTITION_3RD,
 *      FS_PARTITION_PERM,
 *      FS_PARTITION_ALL
 *  } FS_PARTITION_T;
 *  @endcode
 *  @li@c  FS_PARTITION_BASIC                              - Basic partition
 *  @li@c  FS_PARTITION_3RD                                - 3rd partition
 *  @li@c  FS_PARTITION_PERM                               - Perm partition
 *  @li@c  FS_PARTITION_ALL                                - All partitions
 */
/*------------------------------------------------------------------*/
typedef enum
{
    FS_PARTITION_BASIC = 0,
    FS_PARTITION_3RD,
    FS_PARTITION_PERM,
    FS_PARTITION_ALL
} FS_PARTITION_T;

/*------------------------------------------------------------------*/
/*! @brief  callback of partition status change.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_fm_partition_status_nfy)(
        UINT32 ui4_tag,
        FS_PARTITION_T e_partition);

/*------------------------------------------------------------------*/
/*! @struct FM_MNT_CB_T
 *  @brief Callback structure for mount and umount operation
 *  @code
 *  typedef struct _FM_MNT_CB_T
 *  {
 *      x_fm_mnt_fct    pf_mnt_fct;
 *      VOID            *pv_tag;
 *  } FM_MNT_CB_T;
 *  @endcode
 *  @li@c  pf_mnt_fct   - Callback function
 *  @li@c  *pv_tag       - Tag
 */
/*------------------------------------------------------------------*/
typedef struct _FM_MNT_CB_T
{
    x_fm_mnt_fct    pf_mnt_fct;
    VOID            *pv_tag;
} FM_MNT_CB_T;

/*------------------------------------------------------------------*/
/*! @brief Async RW callback function
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_fm_async_fct)(
    HANDLE_T        h_req,
    VOID            *pv_tag,
    FM_ASYNC_COND_T e_async_cond,
    UINT32          ui4_data);



/*------------------------------------------------------------------*/
/*! @enum   FM_ATTR_ON_DISK_SET_TYPE_T
 *  @brief  Set file attribute on disk, such like HIDDEN, READ_ONLY 
 *  @code
 *  typedef enum _FM_ATTR_ON_DISK_SET_TYPE_T
 *  {
 *     FM_ATTR_SET_TYPE_HIDDEN,
 *      FM_ATTR_SET_TYPE_UNHIDDEN,
 *      FM_ATTR_SET_TYPE_READ_ONLY,
 *      FM_ATTR_SET_TYPE_READ_WRITE
 *      
 *   } FM_ATTR_ON_DISK_SET_TYPE_T;
 *
 *  @endcode
 *  @li@c  FM_ATTR_SET_TYPE_HIDDEN                                         - Hide
 *  @li@c  FM_ATTR_SET_TYPE_UNHIDDEN                                     - Unhide
 *  @li@c  FM_ATTR_SET_TYPE_READ_ONLY                                    - Read only
 *  @li@c FM_ATTR_SET_TYPE_READ_WRITE                                   - Read write
 */
/*------------------------------------------------------------------*/
typedef enum _FM_ATTR_ON_DISK_SET_TYPE_T
{
    FM_ATTR_SET_TYPE_HIDDEN,
    FM_ATTR_SET_TYPE_UNHIDDEN,
    FM_ATTR_SET_TYPE_READ_ONLY,
    FM_ATTR_SET_TYPE_READ_WRITE
    
} FM_ATTR_ON_DISK_SET_TYPE_T;



/* Confirm with user about the file replace */
typedef struct _FM_CONFIRM_FILE_REPLACE_T
{
    /* ps_exist_file is provided by FM, and it should be copied to user's own space before using. */
    CHAR* ps_exist_file;
    
    /* b_replace_existing_file should be filled by user in the (*x_fm_copy_nfy) */
    BOOL b_replace_existing_file;

} FM_CONFIRM_FILE_REPLACE_T;

/* Confirm with user about the floder replace */
typedef struct _FM_CONFIRM_DIR_REPLACE_T
{
    /* ps_exist_dir is provided by FM, and it should be copied to user's own space before using. */
    CHAR* ps_exist_dir;
    
    /* b_abort should be filled by user in the (*x_fm_copy_nfy);
           only when b_abort is FALSE, b_replace_existing_sub_files and b_recursively are valid.
           if b_abort is TRUE, the copy/paste operation would be cancled. */
    BOOL b_abort;

    /* b_replace_existing_sub_files should be filled by user in the (*x_fm_copy_nfy).
           if b_replace_existing_sub_files is TRUE, all existing child files of the parent dir would be replaced; */
    BOOL b_replace_existing_sub_files;
    
    /* b_recursively should be filled by user in the (*x_fm_copy_nfy).
          if b_recursively is TRUE, b_replace_existing_sub_files would be applied to all child dirs of the parent dir recursively,
          and FM would not send the notification again. */
    BOOL b_recursively;
    
} FM_CONFIRM_DIR_REPLACE_T;

/* Progress of the copy/paste operation */
typedef struct _FM_COPY_FILE_PRGS_T
{
    /* Provided by FM */
    CHAR*               ps_copying;
    CHAR*               ps_from;
    CHAR*               ps_to;
    
    UINT64              ui8_file_size;
    UINT64              ui8_transferred_bytes;

    /* Should be filled by user */
    BOOL                b_abort;
} FM_COPY_FILE_PRGS_T;

#define FM_NFY_COPY_FAIL_CORE_ERROR                 ((UINT32) 1)   /* Internal error or OSAI error */
#define FM_NFY_COPY_FAIL_FILE_SYSTEM_ERROR          ((UINT32) 2)   /* Internal error or OSAI error */
#define FM_NFY_COPY_FAIL_DEVICE_ERROR               ((UINT32) 3)   /* IO error */
#define FM_NFY_COPY_FAIL_DEST_EXIST                 ((UINT32) 4)   /* The dest is existing */
#define FM_NFY_COPY_FAIL_NOT_EXIST                  ((UINT32) 5)   /* The src is not existed; or the parent dir of the dest is not existed */
#define FM_NFY_COPY_FAIL_DEST_SAME_AS_SRC           ((UINT32) 6)   /* The dest file/floder is same as the source file/floder */


typedef enum _FM_COPY_NFY_COND_T
{
    FM_COPY_NFY_COND_ABORT_FAIL            = -2,
    FM_COPY_NFY_COND_FAIL                  = -1,
    FM_COPY_NFY_COND_CONFIRM_FILE_REPLACE  = 1,
    FM_COPY_NFY_COND_CONFIRM_DIR_REPLACE   = 2,
    FM_COPY_NFY_COND_OK                    = 3,
    FM_COPY_NFY_COND_ABORT_OK              = 4,
    FM_COPY_NFY_COND_PRGS                  = 5
} FM_COPY_NFY_COND_T;


/*
    When start a sync copy/paste operration, only the following two conditions may be notified:
    FM_COPY_NFY_COND_CONFIRM_FILE_REPLACE, ui4_data: (FM_CONFIRM_FILE_REPLACE_T *), b_replace must be filled;
    FM_COPY_NFY_COND_CONFIRM_DIR_REPLACE, ui4_data: (FM_CONFIRM_DIR_REPLACE_T *), b_replace&b_replace must be filled;
    FM_COPY_NFY_COND_PRGS, ui4_data is the progress of the copy/paste operation(ui4_data: (FM_ASYNC_COPY_PRGS_T *));
    NOTE:h_req and pv_tag are NULL in this case.

    When start a async copy/paste operration, the following conditions may be notified:
    FM_COPY_NFY_COND_CONFIRM_FILE_REPLACE, ui4_data: (FM_CONFIRM_FILE_REPLACE_T *), b_replace must be filled;
    FM_COPY_NFY_COND_CONFIRM_DIR_REPLACE, ui4_data: (FM_CONFIRM_DIR_REPLACE_T *), b_replace&b_replace must be filled;
    FM_COPY_NFY_COND_OK, ui4_data is reserved;
    FM_COPY_NFY_COND_PRGS, ui4_data is the progress of the copy/paste operation(ui4_data: (FM_ASYNC_COPY_PRGS_T *));
    FM_COPY_NFY_COND_FAIL, ui4_data is the error code;
    FM_COPY_NFY_COND_ABORT_OK,  ui4_data is reserved;
    FM_COPY_NFY_COND_ABORT_FAIL, ui4_data is the error code;
*/
typedef VOID (*x_fm_copy_nfy)(
    HANDLE_T           h_req,
    VOID*              pv_tag,
    FM_COPY_NFY_COND_T e_copy_cond,
    UINT32             ui4_data);


typedef struct _FM_SEARCH_RESULT_T
{
    CHAR    **pps_pathv;            /* List of matched path name */
    UINT32  ui4_matched_pathc;      /* Count of paths matched */
} FM_SEARCH_RESULT_T;

typedef VOID (*x_fm_search_nfy)(FM_SEARCH_RESULT_T* pt_result);




















/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*! @}  */

/*! @page pageMW_FM File Manager Detailed Description
 *  @par
 *  @section secMW_FM_1  1. Introduction
 *  @par
 *      As the name implies, FM is always used to manage files in a device mounted with special file system. FM also provides the functionality to access RAW
 *      devices. FM also support some network functions now.
 *  @par
 *      The following functionalities provided by FM are frequently used: <b>parse</b> / <b>mount</b> / <b>umount</b> device, <b>open</b> / <b>close</b> / <b>create</b>
 *      / <b>delete</b> / <b>rename</b> /<b>copy</b> file or directory, <b>sync</b> or <b>async</b> <b>read</b> / <b>write</b> file, <b>dir</b> directory.
 *  @par
 *  @section secMW_FM_2  2. Brief Design
 *  @par
 *      FM integrates the native file systems internally and then provides an uniform file system(UFS) externally. User can simply access files/directories via
 *      the UFS, regardless of the actual file system.
 *  @par
 *      FM always works with DeviceManger(DM), DM is responsible for detecting device plugging and unplugging, and notifying such events to user. DM can auto
 *      mount/umount device if it is required.
 *  @par
 *      There are two individual FM in Nucleus and Linux solution. The main differnece is that FM in Linux solution doesn't integrate any native file systems,
 *      but integrate a dummy file system which is just a wrapper of the linux native vfs.
 *  @par
 *  @section secMW_FM_3  3. Frequently Asked Questions
 *  @subsection ssecMW_FM_3_1  3.1. Why and how to make entry for a device?
 *  @par
 *      For accessing the data from the device, FM must have the capability to establish a connection with driver. That is just <b>making entry</b> means to do.
 *  @par
 *      APP user can use <b><i>c_fm_make_entry</i></b> to make a special entry for a specific block or character device. It can let FM has an way to establish a
 *      connection with driver. There are two modes, namely <b>true</b> and <b>virtual</b> mode. If you want to access the RAW device directly, please make entry
 *      for the device with <b>true</b> mode; Otherwise, if you want to access the device via special file system on the device, please make entry for the device
 *      with <b>virtual</b> mode. Making entry with <b>true</b> mode always aims to a real physical device, which is drived by an individual and unique driver
 *      component; Making entry with <b>virtual</b> mode always aims to logical device, i.e. device partitions, which are always formatted or mounted with special
 *      file system, and shared the same driver component. FM can only mount <b>virtual</b> devices, i.e. you have to make <b>virtual</b> entry for the device
 *      before mounting it.
 *  @par
 *      In the Nucleus solution, the device name is just a symbol, <b>device type</b> and <b>unit id</b> are used to indicate an unique device. So you can make
 *      more than one entries for a single device with different name but same device type and unit id. However, in the Linux solution, the <b>device name</b>
 *      is used to indicate a device, but once you have made an entry with the device name successfully, you can also make other entries for that single device
 *      with different name but still same device type and unit id.
 *  @par
 *  @subsection ssecMW_FM_3_2  3.2. How to parse device partitions?
 *  @par
 *      <b>Note: This question only exists in the Nucleus solution.</b>
 *      When a device is pluged, we always want to know partition info. APP user can use <b><i>c_fm_parse_drive</i></b> to <b>parse</b> the device, then use
 *      <b><i>x_fm_get_part_ns</i></b> to get the number of the partitions, and then use <b><i>x_fm_get_part_info</i></b> to get the specified partition info.
 *  @par
 *  @subsection ssecMW_FM_3_3  3.3. How to mount a device?
 *  @par
 *      As mentioned above, you have to make a virtual entry for the device(partition) first. And then you have to create a directory as the <b>mount point</b>.
 *      When they are ready, you can use <b><i>c_fm_mount</i></b> to mount that specified device to the specified mount point.
 *  @par
 *      In Nucleus solution, FM only support FAT(FAT12, FAT16 and FAT32) and NTFS; In Linux solution, FM would only try to mount device wih vfat and ntfs also,
 *      but user may need to mount device with other more native file systems, so we allow user <b>mount</b> the device first and then <b>attach</b> the mounted
 *      device. The restrictions mentioned above still need to be obeyed. In brief, you can regard <b><i>linux native mount + c_fm_attach_file_system</i></b> equal
 *      to <b><i>c_fm_mount</i></b>.
 *  @par
 *  @subsection ssecMW_FM_3_4  3.4. How to umount a mounted device?
 *  @par
 *      User can call <b><i>x_fm_umount</i></b> with the mount point path to umount the mounted device.
 *  @par
 *  @subsection ssecMW_FM_3_5  3.5. What is the difference of dir handle and dir label?
 *  @par
 *      A <b>dir handle</b> is got from a successful call of <b><i>c_fm_open_dir</i></b>, user can then use the handle to get the directory info, or to read directory
 *      entries.
 *  @par
 *      However a <b>dir label</b> is got from a successful call of <b><i>c_fm_set_dir_path</i></b>. It is a label to the specified directory. One can use this
 *      function to set a reference to a directory, so that the underneath UFS can start finding a file from this directory without re-searching from the root.
 *      It hence provides the similar ability to the current working directory, but it's more flexible.
 *  @par
 *  @subsection ssecMW_FM_3_6  3.6. What are the normal operations on a directory?
 *  @par
 *      User can use <b><i>c_fm_open_dir</i></b>' to open a directory, then with the handle, user can use <b><i>c_fm_get_dir_info</i></b> to know how many sub-directories
 *      and sub-files in the directory, or user can use <b><i>c_fm_seek_sir</i></b> to change the current entry postion of the opened dir, or user can use
 *      <b><i>c_fm_read_dir_entries</i></b> to get the info of the current directory entry. And finally, user should use <b><i>c_fm_close</i></b> to close the directory.
 *  @par
 *  @subsection ssecMW_FM_3_7  3.7. What are the normal operations on a file?
 *  @par
 *      Use can use <b><i>c_fm_open</i></b> to open a existing file. If user wants to create a file, user should also use <b><i>c_fm_open</i></b> with a special flag
 *      <b>FM_OPEN_CREATE</b>. If the call is successful, user would get a file handle. With the handle, user can call <b><i>c_fm_lseek</i></b> to change the current
 *      file pointer, and then user can call <b><i>c_fm_read</i></b> to read data from the file, and call <b><i>c_fm_write</i></b> to write data to the file, both of the
 *      two operations are synchronous. User can also call <b><i>c_fm_read_async</i></b> and <b><i>c_fm_write_async</i></b> to start a asynchronous read and write. And
 *      finally, user should use <b><i>c_fm_close</i></b> to close the file.
 */

#endif /* _U_FM_H */

