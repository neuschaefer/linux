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
 * $RCSfile: fm_fat.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: a72575edb609b7b51f412d6003e7428c $
 *
 * Description: 
 *          This file implements the FAT12/16/32 file system.
 *          These codes are derived from Microsoft Extensible Firmware
 *          Initiative FAT32 File System Specification -
 *          FAT:General Overview of On-Disk Format, version 1.03, Dec. 6, 2000.          
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_os.h"
#include "x_dbg.h"
#include "x_end_cnvt.h"
#include "x_handle.h"
#include "x_rm_dev_types.h"

#include "../unicode/x_uc_str.h"
#include "../fm_util.h"
#include "../fm_ufs.h"
#include "../fm_blkdev.h"
#include "../fm_buf.h"
#include "../fm_rootfs.h"

#include "x_fm_fat.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#ifndef DEBUG_FM_FAT

#undef  DEBUG_TRACE
#define DEBUG_TRACE()

#endif /* DEBUG_FM_FAT */

#define FAT_EOC_MARK ((UINT32) 0x0FFFFFFF) /* End Of Cluster */
#define FAT32_EOC_MARK ((UINT32) 0x0FFFFFFF) /* End Of Cluster */
#define FAT16_EOC_MARK ((UINT16) 0xFFFF) /* End of Cluster */
#define FAT12_EOC_MARK ((UINT16) 0x0FFF) 

#define FAT_OFFSET(bs, off)         \
    (((UINT8 *)(bs)) + (off))
    
#define FAT_GET_VAL8(bs, off)       \
    (*(UINT8 *)FAT_OFFSET(bs, off))
    
#define FAT_SET_VAL8(bs, off, val)  \
    (*(UINT8 *)FAT_OFFSET(bs, off) = val)
    
#if defined _CPU_LITTLE_ENDIAN_

#define FAT_UINT8(ui1)  ((UINT8) ui1)
#define FAT_UINT16(ui2) ((UINT16)ui2)
#define FAT_UINT32(ui4) ((UINT32)ui4)
#define FAT_UINT64(ui8) ((UINT64)ui8)

#define FAT_GET_VAL16(bs, off)      \
    ((UINT16)(*FAT_OFFSET(bs, off)) +              \
    (((UINT16)(*FAT_OFFSET(bs, off + 1))) << 8))
    
#define FAT_GET_VAL32(bs, off)      \
    ((UINT32)(*FAT_OFFSET(bs, off)) +              \
    (((UINT32)(*FAT_OFFSET(bs, off + 1))) << 8) +  \
    (((UINT32)(*FAT_OFFSET(bs, off + 2))) << 16) + \
    (((UINT32)(*FAT_OFFSET(bs, off + 3))) << 24))
    
#define FAT_GET_VAL64(bs, off)      \
    ((UINT64)(*FAT_OFFSET(bs, off)) +              \
    (((UINT64)(*FAT_OFFSET(bs, off + 1))) << 8) +  \
    (((UINT64)(*FAT_OFFSET(bs, off + 2))) << 16) + \
    (((UINT64)(*FAT_OFFSET(bs, off + 3))) << 24) + \
    (((UINT64)(*FAT_OFFSET(bs, off + 4))) << 32) + \
    (((UINT64)(*FAT_OFFSET(bs, off + 5))) << 40) + \
    (((UINT64)(*FAT_OFFSET(bs, off + 6))) << 48) + \
    (((UINT64)(*FAT_OFFSET(bs, off + 7))) << 56))
    
#define FAT_SET_VAL16(bs, off, val) \
    (*FAT_OFFSET(bs, off)     = (UINT8)((UINT16)(val) & 0xff));           \
    (*FAT_OFFSET(bs, off + 1) = (UINT8)(((UINT16)(val) >> 8) & 0xff))
    
#define FAT_SET_VAL32(bs, off, val) \
    (*FAT_OFFSET(bs, off)     = (UINT8)((UINT32)(val) & 0xff));           \
    (*FAT_OFFSET(bs, off + 1) = (UINT8)(((UINT32)(val) >> 8) & 0xff));    \
    (*FAT_OFFSET(bs, off + 2) = (UINT8)(((UINT32)(val) >> 16) & 0xff));   \
    (*FAT_OFFSET(bs, off + 3) = (UINT8)(((UINT32)(val) >> 24) & 0xff))
    
#define FAT_SET_VAL64(bs, off, val) \
    (*FAT_OFFSET(bs, off)     = (UINT8)((UINT64)(val) & 0xff));           \
    (*FAT_OFFSET(bs, off + 1) = (UINT8)(((UINT64)(val) >> 8) & 0xff));    \
    (*FAT_OFFSET(bs, off + 2) = (UINT8)(((UINT64)(val) >> 16) & 0xff));   \
    (*FAT_OFFSET(bs, off + 3) = (UINT8)(((UINT64)(val) >> 24) & 0xff));   \
    (*FAT_OFFSET(bs, off + 4) = (UINT8)(((UINT64)(val) >> 32) & 0xff));   \
    (*FAT_OFFSET(bs, off + 5) = (UINT8)(((UINT64)(val) >> 40) & 0xff));   \
    (*FAT_OFFSET(bs, off + 6) = (UINT8)(((UINT64)(val) >> 48) & 0xff));   \
    (*FAT_OFFSET(bs, off + 7) = (UINT8)(((UINT64)(val) >> 56) & 0xff))
    
#elif defined _CPU_BIG_ENDIAN_

#define FAT_UINT8(ui1)  \
        (ui1)

#define FAT_UINT16(ui2) \
        (((ui2) >> 8) | ((ui2) << 8))
        
#define FAT_UINT32(ui4) \
        ((((ui4) & 0xFF000000) >> 24) |     \
         (((ui4) & 0x00FF0000) >> 8)  |     \
         (((ui4) & 0x0000FF00) << 8)  |     \
         (((ui4) & 0x000000FF) << 24))
         
#define FAT_UINT64(ui8) \
        ((((ui8) & 0xFF00000000000000) >> 56) |     \
         (((ui8) & 0x00FF000000000000) >> 40) |     \
         (((ui8) & 0x0000FF0000000000) >> 24) |     \
         (((ui8) & 0x000000FF00000000) >> 8)  |     \
         (((ui8) & 0x00000000FF000000) << 8)  |     \
         (((ui8) & 0x0000000000FF0000) << 24) |     \
         (((ui8) & 0x000000000000FF00) << 40) |     \
         (((ui8) & 0x00000000000000FF) << 56))

#define FAT_GET_VAL16(bs, off)      \
    ((UINT16)(*(UINT8 *)FAT_OFFSET(bs, off + 1)) +          \
    (((UINT16)(*(UINT8 *)FAT_OFFSET(bs, off))) << 8))
    
#define FAT_GET_VAL32(bs, off)      \
    ((UINT32)(*(UINT8 *)FAT_OFFSET(bs,  off + 3)) +         \
    (((UINT32)(*(UINT8 *)FAT_OFFSET(bs, off + 2))) << 8) +  \
    (((UINT32)(*(UINT8 *)FAT_OFFSET(bs, off + 1))) << 16) + \
    (((UINT32)(*(UINT8 *)FAT_OFFSET(bs, off))) << 24))
    
#define FAT_GET_VAL64(bs, off)      \
    ((UINT64)(*(UINT8 *)FAT_OFFSET(bs,  off + 7)) +         \
    (((UINT64)(*(UINT8 *)FAT_OFFSET(bs, off + 6)) << 8) +   \
    (((UINT64)(*(UINT8 *)FAT_OFFSET(bs, off + 5))) << 16) + \
    (((UINT64)(*(UINT8 *)FAT_OFFSET(bs, off + 4))) << 24) + \
    (((UINT64)(*(UINT8 *)FAT_OFFSET(bs, off + 3))) << 32) + \
    (((UINT64)(*(UINT8 *)FAT_OFFSET(bs, off + 2))) << 40) + \
    (((UINT64)(*(UINT8 *)FAT_OFFSET(bs, off + 1))) << 48) + \
    (((UINT64)(*(UINT8 *)FAT_OFFSET(bs, off))) << 56))
    
#define FAT_SET_VAL16(bs, off, val) \
    (*FAT_OFFSET(bs, off)     = (UINT8)(((UINT16)(val) >> 8) & 0xff));    \
    (*FAT_OFFSET(bs, off + 1) = (UINT8)((UINT16)(val) & 0xff))
    
#define FAT_SET_VAL32(bs, off, val) \
    (*FAT_OFFSET(bs, off)     = (UINT8)(((UINT32)(val) >> 24) & 0xff));   \
    (*FAT_OFFSET(bs, off + 1) = (UINT8)(((UINT32)(val) >> 16) & 0xff));   \
    (*FAT_OFFSET(bs, off + 2) = (UINT8)(((UINT32)(val) >> 8) & 0xff));    \
    (*FAT_OFFSET(bs, off + 3) = (UINT8)((UINT32)(val) & 0xff))
    
#define FAT_SET_VAL64(bs, off, val) \
    (*FAT_OFFSET(bs, off)     = (UINT8)(((UINT64)(val) >> 56) & 0xff));   \
    (*FAT_OFFSET(bs, off + 1) = (UINT8)(((UINT64)(val) >> 48) & 0xff));   \
    (*FAT_OFFSET(bs, off + 2) = (UINT8)(((UINT64)(val) >> 40) & 0xff));   \
    (*FAT_OFFSET(bs, off + 3) = (UINT8)(((UINT64)(val) >> 32) & 0xff));   \
    (*FAT_OFFSET(bs, off + 4) = (UINT8)(((UINT64)(val) >> 24) & 0xff));   \
    (*FAT_OFFSET(bs, off + 5) = (UINT8)(((UINT64)(val) >> 16) & 0xff));   \
    (*FAT_OFFSET(bs, off + 6) = (UINT8)(((UINT64)(val) >> 8) & 0xff));    \
    (*FAT_OFFSET(bs, off + 7) = (UINT8)((UINT64)(val) & 0xff))

#else
#error "_CPU_LITTLE_ENDIAN_ or _CPU_BIG_ENDIAN_ must be explictly defined"
#endif /* _CPU_LITTLE_ENDIAN_ */

/*
 *  Define FAT boot sector field offset
 */
#define FAT_BS_JMP0          (0)
#define FAT_BS_JMP1          (FAT_BS_JMP0 + 1)
#define FAT_BS_JMP2          (FAT_BS_JMP1 + 1)
#define FAT_BS_OEM_NAME      (FAT_BS_JMP2 + 1)
#define FAT_BS_BYTES_PER_SEC (FAT_BS_OEM_NAME + 8)
#define FAT_BS_SEC_PER_CLUS  (FAT_BS_BYTES_PER_SEC + 2)
#define FAT_BS_RSV_SEC_CNT   (FAT_BS_SEC_PER_CLUS  + 1)
#define FAT_BS_NUM_FAT       (FAT_BS_RSV_SEC_CNT + 2)
#define FAT_BS_ROOT_ENT_CNT  (FAT_BS_NUM_FAT + 1)
#define FAT_BS_TOTAL_SEC16   (FAT_BS_ROOT_ENT_CNT + 2)
#define FAT_BS_MEDIA         (FAT_BS_TOTAL_SEC16 + 2)
#define FAT_BS_FAT_SIZE16    (FAT_BS_MEDIA + 1)
#define FAT_BS_SEC_PER_TRK   (FAT_BS_FAT_SIZE16 + 2)
#define FAT_BS_NUM_HEADS     (FAT_BS_SEC_PER_TRK + 2)
#define FAT_BS_HIDDEN_SEC    (FAT_BS_NUM_HEADS + 2)
#define FAT_BS_TOTAL_SEC32   (FAT_BS_HIDDEN_SEC + 4)
#define FAT_BS_DRV_NUM       (FAT_BS_TOTAL_SEC32 + 4)
#define FAT_BS_RESERVED1     (FAT_BS_DRV_NUM + 1)
#define FAT_BS_BOOT_SIG      (FAT_BS_RESERVED1 + 1)
#define FAT_BS_VOL_ID        (FAT_BS_BOOT_SIG + 1)
#define FAT_BS_VOL_LAB       (FAT_BS_VOL_ID + 4)
#define FAT_BS_FS_TYPE       (FAT_BS_VOL_LAB + 11)
#define FAT_BS_FAT_SIZE32    (FAT_BS_DRV_NUM)
#define FAT_BS_EXT_FLAGS     (FAT_BS_FAT_SIZE32 + 4)
#define FAT_BS_FS_VERSION    (FAT_BS_EXT_FLAGS + 2)
#define FAT_BS_ROOT_CLUS     (FAT_BS_FS_VERSION + 2)
#define FAT_BS_FS_INFO       (FAT_BS_ROOT_CLUS + 4)
#define FAT_BS_BK_BOOT_SEC   (FAT_BS_FS_INFO + 2)
#define FAT_BS_RESERVED2     (FAT_BS_BK_BOOT_SEC + 2)
#define FAT_BS_DRV_NUM2      (FAT_BS_RESERVED2 + 12)
#define FAT_BS_RESERVED3     (FAT_BS_DRV_NUM2 + 1)
#define FAT_BS_BOOT_SIG32    (FAT_BS_RESERVED3 + 1)
#define FAT_BS_VOL_ID32      (FAT_BS_BOOT_SIG32 + 1)
#define FAT_BS_VOL_LAB32     (FAT_BS_VOL_ID32 + 4)
#define FAT_BS_FS_TYPE32     (FAT_BS_VOL_LAB32 + 11)

/*
 *  FAT32 fs information field offset
 */
#define FAT_FSI_LEAD_SIG     (0)
#define FAT_FSI_RESERVED1    (FAT_FSI_LEAD_SIG + 4)
#define FAT_FSI_STRUC_SIG    (FAT_FSI_RESERVED1 + 480)
#define FAT_FSI_FREE_COUNT   (FAT_FSI_STRUC_SIG + 4)
#define FAT_FSI_NXT_FREE     (FAT_FSI_FREE_COUNT + 4)
#define FAT_FSI_RESERVED2    (FAT_FSI_NXT_FREE + 4)
#define FAT_FSI_TRAIL_SIG    (FAT_FSI_RESERVED2 + 12)

/*
 *  FAT directory entry field offset
 */
#define FAT_DENTRY_NAME      (0)
#define FAT_DENTRY_ATTR      (FAT_DENTRY_NAME + 11)
#define FAT_DENTRY_NTRES     (FAT_DENTRY_ATTR + 1)
#define FAT_DENTRY_CRT_TIME_TENTH   (FAT_DENTRY_NTRES + 1)
#define FAT_DENTRY_CRT_TIME  (FAT_DENTRY_CRT_TIME_TENTH + 1)
#define FAT_DENTRY_CRT_DATE  (FAT_DENTRY_CRT_TIME + 2)
#define FAT_DENTRY_ACC_DATE  (FAT_DENTRY_CRT_DATE + 2)
#define FAT_DENTRY_CLUS_HI   (FAT_DENTRY_ACC_DATE + 2)
#define FAT_DENTRY_WRT_TIME  (FAT_DENTRY_CLUS_HI + 2)
#define FAT_DENTRY_WRT_DATE  (FAT_DENTRY_WRT_TIME + 2)
#define FAT_DENTRY_CLUS_LO   (FAT_DENTRY_WRT_DATE + 2)
#define FAT_DENTRY_FILE_SIZE (FAT_DENTRY_CLUS_LO + 2)

#define FAT_LDENTRY_ORDER    (0)
#define FAT_LDENTRY_NAME1    (FAT_LDENTRY_ORDER + 1)
#define FAT_LDENTRY_ATTR     (FAT_LDENTRY_NAME1 + 10)
#define FAT_LDENTRY_TYPE     (FAT_LDENTRY_ATTR + 1)
#define FAT_LDENTRY_CHKSUM   (FAT_LDENTRY_TYPE + 1)
#define FAT_LDENTRY_NAME2    (FAT_LDENTRY_CHKSUM + 1)
#define FAT_LDENTRY_FST_CLUS_LO (FAT_LDENTRY_NAME2 + 12)
#define FAT_LDENTRY_NAME3    (FAT_LDENTRY_FST_CLUS_LO + 2)

#define FAT_ATTR_READ_ONLY  (0x01)
#define FAT_ATTR_HIDDEN     (0x02)
#define FAT_ATTR_SYSTEM     (0x04)
#define FAT_ATTR_VOLUME_ID  (0x08)
#define FAT_ATTR_DIRECTORY  (0x10)
#define FAT_ATTR_ARCHIVE    (0x20)
#define FAT_ATTR_LONG_NAME  (FAT_ATTR_READ_ONLY | \
                             FAT_ATTR_HIDDEN | \
                             FAT_ATTR_SYSTEM | \
                             FAT_ATTR_VOLUME_ID)
#define FAT_ATTR_LONG_NAME_MASK (FAT_ATTR_READ_ONLY | \
                                 FAT_ATTR_HIDDEN | \
                                 FAT_ATTR_SYSTEM | \
                                 FAT_ATTR_VOLUME_ID | \
                                 FAT_ATTR_DIRECTORY | \
                                 FAT_ATTR_ARCHIVE)

#define FAT_CLUS_TO_DATA_SEC(clus, fat_info)   \
    ( ((fat_info)->ui8_data_off >> 9) +        \
      ((UINT64)((clus) - 2) * (UINT64)(fat_info)->ui1_sec_per_clus) )
    
#define FAT_CLUS_TO_DATA_ADDR(clus, fat_info)   \
    ( (fat_info)->ui8_data_off +                \
      (UINT64)(((clus) - 2) * (UINT64)((fat_info)->ui4_clus_size)) )
      
#define FAT_SIZE_TO_CLUS(size, xent) \
    ( (size == 0) ? 0 : ((size - 1) / xent->pt_mount->t_fs_info.ui8_blk_size)  )
      
#define FAT_GET_DENTRY(xent) ((FAT_DENTRY_T *)((xent)->pv_fs_node))

#define FAT_SECONDS_TO_1980 (((8 * 365) + (2 * 366)) * (24 * 60 * 60))

typedef enum
{
    FAT_TYPE_ANY = 0,
    FAT_TYPE_12,
    FAT_TYPE_16,
    FAT_TYPE_32
} FAT_TYPE_T;

typedef struct _FAT_DISKSIZE_TO_SECPERCLUS
{
    UINT32  ui4_disk_size;
    UINT8   ui1_sec_per_clus;
} FAT_DISKSIZE_TO_SECPERCLUS;

typedef struct _FAT_FSINFO_T
{
    FAT_TYPE_T  e_fat_type;
    UINT8       ui1_sec_per_clus;
    UINT32      ui4_clus_size;
    UINT32      ui4_valid_clus_cnt;
    UINT64      ui8_fat_off;    /* fat byte addr */
    UINT32      ui4_fat_size;
    UINT64      ui8_root_off;   /* FAT12/16 root directory byte addr */
    UINT32      ui4_root_size;  /* FAT12/16 root directory size */
    UINT32      ui4_root_clus;  /* FAT32 root directory cluster, typically 2 */
    UINT64      ui8_data_off;   /* data region byte addr */
    UINT32      ui4_data_size;
    UINT8       ui1_num_fat;

    HANDLE_T    h_lock;         /* FAT region lock */
} FAT_FSINFO_T;

#define FAT_DENTRY_CACHE_SIZE 128 /* must be power of 2 */

typedef struct _FAT_DENTRY_CACHE_T
{
    UINT32 aui4_next_clus[FAT_DENTRY_CACHE_SIZE];
    DLIST_ENTRY_T(_FAT_DENTRY_CACHE_T) t_link;
} FAT_DENTRY_CACHE_T;

typedef struct _FAT_CLUSTER_CACHE_ENTRY_T
{
    UINT32  ui4_lclus;
    UINT32   ui4_pclus;
} FAT_CLUSTER_CACHE_ENTRY_T;
#define FAT_CLUSTER_CACHE_ENTRY_NUM  64
typedef struct _FAT_DENTRY_CLUSTER_CACHE_T
{
    UINT32  ui4_head_lclus;
    UINT32  ui4_head_pclus;
    UINT32  ui4_tail_lclus;
    UINT32  ui4_tail_pclus;
    UINT32  ui4_last_valid_entry;

    FAT_CLUSTER_CACHE_ENTRY_T   at_cache[FAT_CLUSTER_CACHE_ENTRY_NUM];
} FAT_DENTRY_CLUSTER_CACHE_T;

typedef struct _FAT_DENTRY_T
{
    UINT8   s_name[11];
    UINT8   ui1_attr;
    UINT8   ui1_resv;
    UINT8   ui1_crt_time_tenth;
    UINT16  ui2_ctime;
    UINT16  ui2_cdate;
    UINT16  ui2_adate;
    UINT16  ui2_wtime;
    UINT16  ui2_wdate;
    UINT32  ui4_first_clus;
    UINT32  ui4_last_pclus;
    UINT32  ui4_size;

    UINT8   s_utf8[4 * (FM_MAX_FILE_LEN + 1)];    /* UTF8 string in CPU endian */
    UINT8   ui1_use_dent;
    UINT16  ui2_len;        /* number of characters */
    UINT32  ui4_name_off;   /* offset of the short name entry in the directory */

    FAT_DENTRY_CLUSTER_CACHE_T  t_clus_cache;
} FAT_DENTRY_T;
#if 0
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a: b)
#endif
/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static VOID _fat_utf8_to_upper(
    UINT8           *ps_uname,
    const UINT8     *ps_lname);

static BOOL _fat_utf8_to_oem(
    UINT8           *ps_oem,
    UINT8           *ps_utf8);

static VOID _fat_strip_space_and_period(
    UINT8           *ps_new,
    UINT8           *ps_org);

static VOID _fat_copy_dos_name(
    UINT8           *ps_dos_name,
    const UINT8     *ps_uname);

static INT32 _fat_generate_numeric_tail(
    FM_XENTRY_T *pt_xent,
    UINT8       *pui1_short_name);

static BOOL _fat_is_dos_format(
    const UINT8 *ps_utf8);

static VOID _fat_generate_dos_name(
    FM_XENTRY_T     *pt_prnt_xent,
    UINT8           *ps_dos_name,
    const UINT8     *ps_utf8);

static VOID _fat_trans_unicode_endian(
    UINT8           *ps_cpu,
    const UINT8     *ps_fat);

static UINT8 _fat_disksize_to_sectors_per_cluster(
    UINT64          ui4_disk_size,
    FAT_TYPE_T      *pe_fat_type);

static UINT8 _fat_longname_chksum(
    UINT8           *ps_name);

static INT32 _fat_rw_dev(
    FM_XENTRY_T     *pt_xent,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT64          ui8_offset,
    UINT32          *pui4_done,
    HANDLE_T        h_sync_lock,
    BOOL            b_read,
    BOOL            b_prefetch,
    UINT32          ui4_prefetch_limit);

#define _fat_dev_cache_read(xent, data, len, off, done, lock, b_prefetch, prefetch_limit)   \
    _fat_rw_dev((xent), (data), (len), (off), (done), (lock), TRUE, b_prefetch, prefetch_limit)
#define _fat_dev_cache_write(xent, data, len, off, done, lock, b_prefetch, prefetch_limit)  \
    _fat_rw_dev((xent), (data), (len), (off), (done), (lock), FALSE, b_prefetch, prefetch_limit)

static INT32 _fat_check_boot_sector(
    FM_XENTRY_T     *pt_dev_xent,
    UINT32          ui4_part_off,
    HANDLE_T        h_sync_lock,
    FAT_FSINFO_T    *pt_fat_info);

static VOID _fat_get_dentry(
    FAT_DENTRY_T    *pt_dent,
    UINT8           *pui1_dent);

static VOID _fat_set_dentry(
    UINT8           *pui1_dent,
    FAT_DENTRY_T    *pt_dent);

static VOID _fat_short_name_strcpy(
    UINT8           *pc_dst,
    UINT8           *pc_src,
    UINT8           ui1_nt_res);

static INT32 _fat_read_next_dentry(
    FM_XENTRY_T     *pt_xent,
    FAT_DENTRY_T    *pt_dent,
    UINT64          *pui8_off);

static FM_XENTRY_T *_fat_make_xentry(
    FM_XENTRY_T     *pt_prnt_xent,
    FAT_DENTRY_T    *pt_dent);

static UINT32 _fat_attr_to_ufs_mode(
    UINT8           ui1_attr);

static UINT32 _fat_conv_to_utc_time(
    UINT16          ui2_date,
    UINT16          ui2_time,
    UINT8           ui1_time_tenth);

static UINT32 _fat_conv_to_fat_time(
    UINT32  ui4_utc_sec);

static UINT32 _fat_read_fat_entry(
    FM_MOUNT_T      *pt_mount,
    UINT32          ui4_clus);

static INT32 _fat_extend_fat_chain(
    FM_MOUNT_T      *pt_mount,
    FAT_DENTRY_T    *pt_dent,
    UINT32          ui4_entry,
    UINT32          ui4_from_lclus,
    BOOL            b_zero_out);

static INT32 _fat_inval_fat_chain(
    FM_XENTRY_T     *pt_xent,
    UINT32          ui4_start_lclus);

static INT32 _fat_add_dir_entry(
    FM_XENTRY_T     *pt_prnt_xent,
    FAT_DENTRY_T    *pt_dent,
    const UINT8     *s_utf16,
    UINT16          ui2_len);

static INT32 _fat_del_dir_entry(
    FM_XENTRY_T     *pt_prnt_xent,
    FAT_DENTRY_T    *pt_dent);

static INT32 _fat_build_clus_cache2(
    FM_MOUNT_T      *pt_mount,
    FAT_DENTRY_T    *pt_dent,
    UINT32 ui4_want_lclus);
static INT32 _fat_get_pclus(
    UINT32  *pui4_pclus,
    UINT32  ui4_lclus,
    FM_MOUNT_T  *pt_mount,
    FAT_DENTRY_T    *pt_dent);

static VOID _fat_async_done(
    HANDLE_T    h_req,
    VOID            *pv_tag,
    FM_ASYNC_COND_T e_async_cond,
    UINT32          ui4_data);

static INT32 _fat_rw_buf(
    FM_RW_ARG_T     *pt_arg,
    BOOL             b_read,
    BOOL             b_prefetch);

static INT32 fat_format(VOID *pv_data);
static INT32 fat_mount(VOID *pv_data);
static INT32 fat_umount(VOID *pv_data);

static INT32 fat_lookup(VOID *pv_data);
static INT32 fat_read(VOID *pv_data);
static INT32 fat_write(VOID *pv_data);
static INT32 fat_lseek(VOID *pv_data);
static INT32 fat_seek_dir(VOID *pv_data);
static INT32 fat_get_info(VOID *pv_data);
static INT32 fat_set_info(VOID *pv_data);
static INT32 fat_get_dir_info(VOID *pv_data);
static INT32 fat_make_entry(VOID *pv_data);
static INT32 fat_read_dir(VOID *pv_data);
static INT32 fat_close(VOID *pv_data);
static INT32 fat_delete(VOID *pv_data);
static INT32 fat_rename(VOID *pv_data);
static INT32 fat_sync(VOID *pv_data);

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*
 * Days that have passed for each month in a regular year.
 */
static UINT16 aui2_reg_year[] = {
	31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

/*
 * Days that have passed for each month in a leap year.
 */
static UINT16 aui2_leap_year[] = {
	31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
};

static FAT_DISKSIZE_TO_SECPERCLUS t_fat16_disk_tbl[] =
{
    /* { 8400, 0 }, */
    { 8400, 1},
    { 32680, 2 },
    { 262144, 4 },
    { 524288, 8 },
    { 1048576, 16 },
    { 2097152, 32 },
    { 4194304, 64 },
    { 0xFFFFFFFF, 0 }
};

static FAT_DISKSIZE_TO_SECPERCLUS t_fat32_disk_tbl[] =
{
    { 66600, 0 },
    { 532480, 1 },
    { 16777216, 8 },
    { 33554432, 16 },
    { 67108864, 32 },
    { 0xFFFFFFFF, 64}
};

static FM_UFS_OP_T t_fat_ufs_ops =
{
    fat_format,
    NULL,
    fat_mount,
    fat_umount
};

static FM_XENTRY_OP_T t_fat_xent_ops =
{
    fat_lookup,     /* pf_lookup */
    fat_read,       /* pf_read */
    fat_write,      /* pf_write */
    fat_read,       /* pf_read_async */
    fat_write,      /* pf_write_async */
    fat_lseek,      /* pf_lseek */
    fat_close,      /* pf_close */
    fat_get_info,   /* pf_get_info */
    fat_set_info,   /* pf_set_info */
    fat_get_dir_info, /* pf_get_dir_info */
    fat_make_entry, /* pf_make_entry */
    NULL,
    NULL,
    fat_read_dir,   /* pf_read_dir */
    fat_delete,     /* pf_delete */
    fat_rename,     /* pf_rename */
    fat_sync,        /* pf_sync */
    fat_seek_dir,
    NULL
};

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _fat_strcasecmp
 *
 * Description: Compare two string in case insensitive case.
 *
 * Inputs:  ps_s1, ps_s2
 *              Null terminated strings to be compared.
 *
 * Outputs: -
 *
 * Returns: 0   ps_s1 is identical to ps_s2.
 *          1   ps_s1 is greater than ps_s2.
 *          -1  ps_s1 is less than ps_s2.
 ----------------------------------------------------------------------------*/
static INT32 _fat_strcasecmp(
    const CHAR *ps_s1,
    const CHAR *ps_s2)
{
    INT32 i4_ret;

    if (ps_s1 == NULL)
    {
        return (ps_s2 == NULL) ? 0 : -1;
    }
    else if (ps_s2 == NULL)
    {
        return 1;
    }

    do
    {
        CHAR pc1 = *ps_s1;
        CHAR pc2 = *ps_s2;

        if ((pc1 >= 'a') && (pc1 <= 'z'))
        {
            pc1 += ('A' - 'a');
        }
        if ((pc2 >= 'a') && (pc2 <= 'z'))
        {
            pc2 += ('A' - 'a');
        }
        i4_ret = pc1 - pc2;
        if (i4_ret != 0)
        {
            return (i4_ret > 0) ? 1 : -1;
        }
    } while ((*ps_s1++ != '\0') && (*ps_s2++ != '\0'));

    return 0;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_utf8_to_upper
 *
 * Description: Convert a string to upper case. Only characters from 'a' to 'z'
 *              will be converted.
 *
 * Inputs:      ps_lname
 *                  String to be converted.
 *
 * Outputs:     ps_uname
 *                  Buffer for new string. It must be as large as ps_lname or
 *                  larger than ps_lname.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fat_utf8_to_upper(
    UINT8       *ps_uname,
    const UINT8 *ps_lname)
{
    UINT8       *pc_uname;
    const UINT8 *pc_lname;

    for (pc_uname = ps_uname, pc_lname = ps_lname;
         *pc_lname != 0;
         pc_uname++, pc_lname++)
    {
        *pc_uname = ((*pc_lname >= 'a') && (*pc_lname <= 'z')) ?
                     *pc_lname + ('Z' - 'z') :
                     *pc_lname;
    }
    *pc_uname = 0;
}

/* iso8859-1 -> cp850 */

/*
 * 0 - character disallowed in long file name.
 * 1 - character should be replaced by '_' in DOS file name, 
 *     and generation number inserted.
 * 2 - character ('.' and ' ') should be skipped in DOS file name,
 *     and generation number inserted.
 */
static UINT8 ac_unicode_to_oem[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, '_',  0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, '_',  '_',  '_',  0x2D, 0x2E, '_', 
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, '_',  '_',  '_',  '_',  '_',  '_',
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5A, '_',  '_',  '_',  0x5E, 0x5F,
    0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5A, 0x7B, '_',  0x7D, 0x7E, 0x7F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
    0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
    0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
    0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
    0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
    0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};

static BOOL _fat_check_name_legality(CHAR *ps_name)
{
    UINT32 ui4_len;
    UINT32 ui4_cnt;

    ui4_len = x_strlen(ps_name);

    for (ui4_cnt = 0; ui4_cnt < ui4_len; ui4_cnt++)
    {
        if ((ps_name[ui4_cnt] == 0x5C) || /* '\' */
            (ps_name[ui4_cnt] == 0x2F) || /* '/' */
            (ps_name[ui4_cnt] == 0x3A) || /* ':' */
            (ps_name[ui4_cnt] == 0x2A) || /* '*' */
            (ps_name[ui4_cnt] == 0x3F) || /* '?' */
            (ps_name[ui4_cnt] == 0x22) || /* '"' */
            (ps_name[ui4_cnt] == 0x3C) || /* '<' */
            (ps_name[ui4_cnt] == 0x3E) || /* '>' */
            (ps_name[ui4_cnt] == 0x7C))
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _fat_utf8_to_oem
 *
 * Description: Convert illegal characters to '-'.
 *
 * Inputs:      ps_utf8
 *                  String to be converted.
 *
 * Outputs:     ps_oem
 *                  Buffer for new string. It must be as large as ps_utf8 or
 *                  larger than ps_utf8.
 *
 * Returns: TRUE
 *              The string is lossy conversion.
 *          FALSE
 *              The string is successful conversion.
 ----------------------------------------------------------------------------*/
static BOOL _fat_utf8_to_oem(
    UINT8 *ps_oem,
    UINT8 *ps_utf8)
{
    UINT8 *pc_oem, *pc_utf8;
    BOOL b_lossy = FALSE;

    for (pc_oem = ps_oem, pc_utf8 = ps_utf8;
         *pc_utf8 != 0;
         pc_oem++, pc_utf8++)
    {
        *pc_oem = ac_unicode_to_oem[*pc_utf8];
        if (*pc_oem == '_')
        {
            b_lossy = TRUE;
        }
    }

    return b_lossy;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_strip_space_and_period
 *
 * Description: Strip all leading and embedded spaces from the given string.
 *              And strip all leading periods from it, too.
 *
 * Inputs:      ps_org
 *                  Original string.
 *
 * Outputs:     ps_new
 *                  New string
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fat_strip_space_and_period(
    UINT8 *ps_new,
    UINT8 *ps_org)
{
    UINT8 *pc_new, *pc_org;

    for (pc_new = ps_new, pc_org = ps_org;
         *pc_org != 0;
         pc_org++)
    {
        if (*pc_org != ' ')
        {
            *pc_new++ = *pc_org;
        }
    }
    *pc_new = 0;

    for (pc_org = ps_new; *pc_org == '.'; pc_org++)
    {
        /* strip leading periods */
    }

    for (pc_new = ps_new; *pc_org != 0; pc_org++)
    {
        *pc_new++ = *pc_org;
    }
}


/*-----------------------------------------------------------------------------
 * Name: _fat_copy_dos_name
 *
 * Description: Put a 8.3 name to a 11 bytes of memory space in the directory
 *              entry format.
 *
 * Inputs:      ps_dos_name
 *                  11 bytes buffer.
 *
 * Outputs:     ps_uname
 *                  DOS name.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fat_copy_dos_name(
    UINT8       *ps_dos_name,
    const UINT8 *ps_uname)
{
    UINT8 *pc_dname;
    const UINT8 *pc_uname;
    const UINT8 *pc_fst_period = NULL;
    const UINT8 *pc_period = NULL;

    x_memset(ps_dos_name, ' ', 11);
    
    pc_dname = ps_dos_name;
    pc_uname = ps_uname;

    while ((*pc_uname != 0) && (*pc_uname != '.') && (pc_dname - ps_dos_name < 8))
    {
        *pc_dname++ = *pc_uname++;
    }
    while (*pc_uname != 0)
    {
        if (*pc_uname == '.')
        {
            if (pc_fst_period == NULL)
            {
                pc_fst_period = pc_uname;
            }
            pc_period = pc_uname;
        }
        pc_uname++;
    }
    if (pc_period != NULL)
    {
        pc_uname = pc_fst_period;
        while ((pc_uname != pc_period) && ((pc_dname - ps_dos_name) < 8))
        {
            if (*pc_uname != '.')
            {
                *pc_dname++ = *pc_uname;
            }
            pc_uname++;
        }

        pc_uname = pc_period + 1;
        pc_dname = ps_dos_name + 8;
        while ((*pc_uname != 0) && ((pc_dname - ps_dos_name) < 11))
        {
            *pc_dname++ = *pc_uname++;
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: _fat_trans_unicode_endian
 *
 * Description: The directory entry long name saved on device is two-byte
 *              unicode in little endian. We have to translate its byte
 *              order to whatever CPU order before convert to UTF8.
 *
 * Inputs:  ps_fat
 *              UCS-2 string in little endian.
 *
 * Outputs: ps_cpu
 *              UCS-2 string in CPU endian.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fat_trans_unicode_endian(
    UINT8           *ps_cpu,
    const UINT8     *ps_fat)
{
    UINT8       *pc_cpu;
    const UINT8 *pc_fat;

    for (pc_cpu = ps_cpu, pc_fat = ps_fat;
         (*pc_fat != 0) || (*(pc_fat + 1) != 0);
         pc_cpu += 2, pc_fat += 2)
    {
#if defined _CPU_LITTLE_ENDIAN_
        *pc_cpu = *pc_fat;
        *(pc_cpu + 1) = *(pc_fat + 1);
#else
        *pc_cpu = *(pc_fat + 1);
        *(pc_cpu + 1) = *pc_fat;
#endif /* _CPU_LITTLE_ENDIAN_ */
    }
    *pc_cpu = 0;
    *(pc_cpu + 1) = 0;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_disksize_to_sectors_per_cluster
 *
 * Description: According to the FAT document from Microsoft, the number of
 *              sectors per cluster can be determined by the device size.
 *              Though the number is a sugguested value, we just follow it.
 *              This function is used in format a drive (create a FAT file
 *              system).
 *
 * Inputs:  ui4_dev_size
 *              The size of device in bytes.
 *
 * Outputs: pe_fat_type
 *              The FAT type. It will be either FAT_TYPE_12 or FAT_TYPE_16
 *              or FAT_TYPE_32.
 *
 * Returns: Number of sectors per cluster.
 ----------------------------------------------------------------------------*/
static UINT8 _fat_disksize_to_sectors_per_cluster(
    UINT64      ui4_dev_size,
    FAT_TYPE_T *pe_fat_type)
{
    UINT8  ui1_ret = 0;
    INT32 i;

    switch (*pe_fat_type)
    {
        case FAT_TYPE_ANY:
        case FAT_TYPE_12:

            /* we do not format disc in FAT 12 */

        case FAT_TYPE_16:
        {
            *pe_fat_type = FAT_TYPE_16;

            for (i = 0; i < (INT32)(sizeof(t_fat16_disk_tbl)/sizeof(FAT_DISKSIZE_TO_SECPERCLUS)); i++)
            {
                if (ui4_dev_size <= t_fat16_disk_tbl[i].ui4_disk_size)
                {
                    ui1_ret = t_fat16_disk_tbl[i].ui1_sec_per_clus;
                    break;
                }
            }
            
            if (ui1_ret != 0)
            {
                return ui1_ret;
            }
        }   
            
            /* once the given space is not suitable for FAT 16, we try 
               to format it in FAT 32.*/

        case FAT_TYPE_32:
        {
            *pe_fat_type = FAT_TYPE_32;

            for (i = 0; i < (INT32)(sizeof(t_fat32_disk_tbl)/sizeof(FAT_DISKSIZE_TO_SECPERCLUS)); i++)
            {
                if (ui4_dev_size <= t_fat32_disk_tbl[i].ui4_disk_size)
                {
                    ui1_ret = t_fat32_disk_tbl[i].ui1_sec_per_clus;
                    break;
                }
            }
            return ui1_ret;
        }
    }

    return 0;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_longname_chksum
 *
 * Description: Calculate the checksum in the long directory entry (offset 13).
 *
 * Inputs:  ps_name
 *              The 8.3 format short name.
 *
 * Outputs: -
 *
 * Returns: The checksum in the long directory entry.
 ----------------------------------------------------------------------------*/
static UINT8 _fat_longname_chksum(
    UINT8       *ps_name)
{
    UINT8 ui1_sum = 0;
    UINT32 i;

    for (i = 11; i != 0; i--)
    {
        ui1_sum = (UINT8)(((ui1_sum & 1) ? 0x80 : 0) + (ui1_sum >> 1) + *ps_name++);
    }
    return ui1_sum;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_rw_dev
 *
 * Description: Read/Write data from/to the mounted device through buffer
 *              cache.
 *
 * Inputs:  pt_xent
 *              The reference to the xentry of mounted device.
 *          pv_data
 *              Buffer to hold read/write data.
 *          ui4_count
 *              Number of bytes to read/write.
 *          ui8_offset
 *              Read/write starting position in the device.
 *          h_sync_lock
 *              Handle of the semaphore to lock for synchronization.
 *          b_read
 *              Indicate to perform read or write operation.
 *
 * Outputs: pui4_done
 *              Number of done bytes.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              pt_xent is an invalid entry
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 _fat_rw_dev(
    FM_XENTRY_T     *pt_xent,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT64          ui8_offset,
    UINT32          *pui4_done,
    HANDLE_T        h_sync_lock,
    BOOL            b_read,
    BOOL            b_prefetch,
    UINT32          ui4_prefetch_limit)
{
    FM_RW_ARG_T t_io;
    INT32 i4_ret;
    ASSERT(pt_xent    != NULL);
    ASSERT(pv_data    != NULL);
    ASSERT(pui4_done  != NULL);
    ASSERT(h_sync_lock != NULL_HANDLE);
    ASSERT(pt_xent->pt_xent_ops != NULL);
    ASSERT(pt_xent->pt_xent_ops->pf_write != NULL);
    
    t_io.pt_xent        = pt_xent;
    t_io.pv_data        = pv_data;
    t_io.ui4_count      = ui4_count;
    t_io.ui8_offset     = ui8_offset;
    t_io.ui4_done_bytes = 0;
    t_io.ui2_flag       = FM_IO_SYNC;
    t_io.h_bin_sema     = h_sync_lock;

    i4_ret = fm_xentry_lock(pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }

    i4_ret = fm_buf_read_write(&t_io, b_read, b_prefetch, ui4_prefetch_limit);

    fm_xentry_unlock(pt_xent);
    
    if (i4_ret == FMR_OK)
    {
        ASSERT(t_io.ui4_done_bytes == ui4_count);
        *pui4_done = t_io.ui4_done_bytes;
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_check_boot_sector
 *
 * Description: Check the FAT boot sector.
 *
 * Inputs:  pt_dev_xent
 *              Reference to the device's xentry object.
 *          ui4_part_off
 *              Starting position of the boot sector on the device. It would
 *              be non-zero if there is partitions on the device.
 *          h_sync_lock
 *              Semophore to lock for synchronization.          
 *
 * Outputs: pt_fat_info
 *              Reference to the FAT file system data.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_FILE_SYSTEM_CRASH
 *              Not FAT or crashed.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 _fat_check_boot_sector(
    FM_XENTRY_T     *pt_dev_xent,
    UINT32          ui4_part_off,
    HANDLE_T        h_sync_lock,
    FAT_FSINFO_T    *pt_fat_info)
{
    FAT_TYPE_T  e_fat_type;
    UINT8  *pui1_bs;
    UINT8  ui1_num_fat;
    UINT16  ui2_bytes_per_sec;
    UINT8  ui1_sec_per_clus;
    UINT16 ui2_rsv_sec_cnt;
    UINT16 ui2_root_ent_cnt;
    UINT32 ui4_fat_secz;
    UINT32 ui4_total_sec;
    UINT32 ui4_data_sec;
    UINT32 ui4_root_dir_sec;
    UINT32 ui4_clus_cnt;
    UINT32 ui4_root_clus = 0;
    UINT32 ui4_done;
    INT32  i4_ret;

    UINT32 ui4_sec_sz;

    ASSERT(pt_dev_xent != NULL);
    ASSERT(h_sync_lock  != NULL_HANDLE);
    ASSERT(pt_dev_xent->pt_dev != NULL);

    pui1_bs = x_mem_alloc(512 * sizeof(UINT8));

    if (pui1_bs == NULL)
    {
        return FMR_CORE;
    }

    if (IS_XENTRY_BLK(pt_dev_xent))
    {
        ui4_sec_sz = (UINT32)pt_dev_xent->pt_dev->u_param.t_blkdev.ui4_sec_size;
    }
    else
    {
        ui4_sec_sz = (UINT32)512;
    }

    i4_ret = _fat_dev_cache_read(pt_dev_xent,
                                 pui1_bs,
                                 512,
                                 ui4_part_off,
                                 &ui4_done,
                                 h_sync_lock,
                                 FALSE,
                                 0);
    if (i4_ret != FMR_OK)
    {
        goto fail;
    }

    /*
     *  Figure out FAT parameters.
     */
    ui4_fat_secz = (UINT32)FAT_GET_VAL16(pui1_bs, FAT_BS_FAT_SIZE16);
    if (ui4_fat_secz == 0)
    {
        ui4_fat_secz = FAT_GET_VAL32(pui1_bs, FAT_BS_FAT_SIZE32);

        if ( ui4_fat_secz == 0)
        {
            i4_ret = FMR_FILE_SYSTEM_CRASH; /* shouldn't be both zero of FatSz16, and FatSz32 */
            goto fail;
        }
    }

    ui4_total_sec = (UINT32)FAT_GET_VAL16(pui1_bs, FAT_BS_TOTAL_SEC16);
    if (ui4_total_sec == 0)
    {
        ui4_total_sec = FAT_GET_VAL32(pui1_bs, FAT_BS_TOTAL_SEC32);

        if (ui4_total_sec == 0)
        {
            i4_ret = FMR_FILE_SYSTEM_CRASH; /* shouldn't be both zero of FatSz16, and FatSz32 */
            goto fail;
        }
    }

    ui2_rsv_sec_cnt = FAT_GET_VAL16(pui1_bs, FAT_BS_RSV_SEC_CNT);
    ui1_num_fat     = FAT_GET_VAL8(pui1_bs, FAT_BS_NUM_FAT);

    if (ui1_num_fat == 0)
    {
        i4_ret = FMR_FILE_SYSTEM_CRASH; /* shouldn't be both zero of FatSz16, and FatSz32 */
        goto fail;
    }

    ui2_root_ent_cnt = FAT_GET_VAL16(pui1_bs, FAT_BS_ROOT_ENT_CNT);
    ui2_bytes_per_sec = FAT_GET_VAL16(pui1_bs, FAT_BS_BYTES_PER_SEC);

    if ((ui2_bytes_per_sec != 512) && (ui2_bytes_per_sec != 1024) &&
        (ui2_bytes_per_sec != 2048) && (ui2_bytes_per_sec != 4096))
    {
        i4_ret = FMR_FILE_SYSTEM_CRASH; /* not FAT or crashed */
        goto fail;
    }

    ui4_root_dir_sec = (UINT32)(((ui2_root_ent_cnt * 32) + (ui2_bytes_per_sec - 1)) / ui2_bytes_per_sec);
    ui1_sec_per_clus = FAT_GET_VAL8(pui1_bs, FAT_BS_SEC_PER_CLUS);

    if ((ui1_sec_per_clus != 1) && (ui1_sec_per_clus != 2) &&
        (ui1_sec_per_clus != 4) && (ui1_sec_per_clus != 8) &&
        (ui1_sec_per_clus != 16) && (ui1_sec_per_clus != 32) &&
        (ui1_sec_per_clus != 64) && (ui1_sec_per_clus != 128))
    {
        i4_ret = FMR_FILE_SYSTEM_CRASH; /* not FAT or crashed */
        goto fail;
    }
    
    if ( ui1_sec_per_clus * ui2_bytes_per_sec > (512 * 1024)) /* looser M$'s spec. In M$'s spec, should be 32K */
    {
        i4_ret = FMR_FILE_SYSTEM_CRASH;
        goto fail;
    }
    
    ui4_data_sec = ui4_total_sec - ui2_rsv_sec_cnt -
                    (ui4_fat_secz * ui1_num_fat) - ui4_root_dir_sec;
    ui4_clus_cnt = ui4_data_sec / ui1_sec_per_clus;

    if (ui4_clus_cnt == 0)
    {
        i4_ret = FMR_FILE_SYSTEM_CRASH; /* shouldn't be both zero of FatSz16, and FatSz32 */
        goto fail;
    }

    if (ui4_clus_cnt < 4085)
    {
        e_fat_type = FAT_TYPE_12;
        /* 
          * hack by riccardo
          * if the device is nor flash, we violate the m$ FAT spec a little bit.
          * We access nor flash device in FAT16 mode whatever size it has.
          */
#if 0
        if  ((pt_dev_xent->pt_dev->e_drv_type == DRVT_NOR_FLASH) ||
        	(pt_dev_xent->pt_dev->e_drv_type == DRVT_NAND_FLASH))
        {
            e_fat_type = FAT_TYPE_16;
        }
#endif
    }
    else if (ui4_clus_cnt < 65525)
    {
        e_fat_type = FAT_TYPE_16;
    }
    else
    {
        e_fat_type = FAT_TYPE_32;
    }

    /* enforce the file system type as FAT32 */

    if (x_strncmp((const CHAR *)(&(pui1_bs[82])), "FAT32   ", 8) == 0)
    {
        e_fat_type = FAT_TYPE_32;
    }

    if ((pui1_bs[510] != 0x55) || (pui1_bs[511] != 0xAA))
    {    
        DBG_INFO(("This file system MIGHT BE Crashed! pui1_bs[510]: %x, pui1_bs[511]: %x\n", pui1_bs[510]&0xff, pui1_bs[511]&0xff));
    }

    if (e_fat_type != FAT_TYPE_32)
    {
        if ((ui2_root_ent_cnt == 0) ||
            (((ui2_root_ent_cnt * 32) % (ui2_bytes_per_sec * 2)) != 0) ||
            (FAT_GET_VAL8(pui1_bs, FAT_BS_BOOT_SIG) != 0x29))
        {   
            DBG_INFO(("[FM]  This file system MIGHT BE Crashed!\nui2_root_ent_cnt: %d \npui1_bs[FAT_BS_BOOT_SIG]: %x\n",
            ui2_root_ent_cnt, (FAT_GET_VAL8(pui1_bs, FAT_BS_BOOT_SIG)) ));
        }
    }
    else
    {
        if ((ui2_root_ent_cnt != 0) ||
            (FAT_GET_VAL16(pui1_bs, FAT_BS_TOTAL_SEC16) != 0) ||
            (FAT_GET_VAL16(pui1_bs, FAT_BS_FAT_SIZE16) != 0)  ||
            (FAT_GET_VAL16(pui1_bs, FAT_BS_FS_VERSION) != 0)  ||
            (FAT_GET_VAL16(pui1_bs, FAT_BS_FS_INFO) != 1)     ||
            (FAT_GET_VAL16(pui1_bs, FAT_BS_BK_BOOT_SEC) != 6) ||
            (FAT_GET_VAL8(pui1_bs, FAT_BS_BOOT_SIG32) != 0x29))
        {
            DBG_INFO(("[FM] This file system MIGHT BE Crashed!ui2_root_ent_cnt: %d pui1_bs[FAT_BS_BOOT_SIG]:%x\n",
                ui2_root_ent_cnt, (FAT_GET_VAL8(pui1_bs, FAT_BS_BOOT_SIG))));
        }

        ui4_root_clus = FAT_GET_VAL32(pui1_bs, FAT_BS_ROOT_CLUS);

        /*
         *  Check FAT32 FSInfo
         */

        i4_ret = _fat_dev_cache_read(pt_dev_xent,
                                     pui1_bs,
                                     512,
                                     512 + ui4_part_off,
                                     &ui4_done,
                                     h_sync_lock,
                                     FALSE,
                                     0);

        if (i4_ret != FMR_OK)
        {
            return i4_ret;
        }
        if ((pui1_bs[FAT_FSI_LEAD_SIG] != 0x52)      ||
            (pui1_bs[FAT_FSI_LEAD_SIG + 1] != 0x52)  ||
            (pui1_bs[FAT_FSI_LEAD_SIG + 2] != 0x61)  ||
            (pui1_bs[FAT_FSI_LEAD_SIG + 3] != 0x41)  ||
            (pui1_bs[FAT_FSI_STRUC_SIG] != 0x72)     ||
            (pui1_bs[FAT_FSI_STRUC_SIG + 1] != 0x72) ||
            (pui1_bs[FAT_FSI_STRUC_SIG + 2] != 0x41) ||
            (pui1_bs[FAT_FSI_STRUC_SIG + 3] != 0x61) ||
            (pui1_bs[FAT_FSI_TRAIL_SIG] != 0x00)     ||
            (pui1_bs[FAT_FSI_TRAIL_SIG + 1] != 0x00) ||
            (pui1_bs[FAT_FSI_TRAIL_SIG + 2] != 0x55) ||
            (pui1_bs[FAT_FSI_TRAIL_SIG + 3] != 0xAA))
        {

            DBG_INFO(("[FM] This file system MIGHT BE Crashed!FAT_FSI_LEAD_SIG, FAT_FSI_STRUC, FAT_FSI_TRAIL_SIG aren't correct\n"));
        }
    }

    if (pt_fat_info != NULL)
    {
        pt_fat_info->e_fat_type       = e_fat_type;
        pt_fat_info->ui1_sec_per_clus = ui1_sec_per_clus;
        pt_fat_info->ui4_clus_size    = (UINT32)ui2_bytes_per_sec * ui1_sec_per_clus;
        pt_fat_info->ui4_valid_clus_cnt  = ui4_clus_cnt;
        pt_fat_info->ui1_num_fat      = ui1_num_fat;
        pt_fat_info->ui8_fat_off      = ui2_rsv_sec_cnt  * ui2_bytes_per_sec;
        pt_fat_info->ui4_fat_size     = ui4_fat_secz * ui2_bytes_per_sec; /* size for one FAT */
        if (e_fat_type != FAT_TYPE_32)
        {
            pt_fat_info->ui8_root_off  = pt_fat_info->ui8_fat_off + 
                                         pt_fat_info->ui4_fat_size * ui1_num_fat;
            pt_fat_info->ui4_root_size = ui4_root_dir_sec * ui2_bytes_per_sec;
            pt_fat_info->ui8_data_off  = pt_fat_info->ui8_root_off +
                                         pt_fat_info->ui4_root_size;
        }
        else
        {
            UINT32 ui4_this_clus;
            UINT32 ui4_next_clus;
            UINT32 ui4_val;
            UINT32 ui4_done;

            UINT32 ui4_fat_sec_ns;
            UINT32 ui4_prefetch_limit;

            pt_fat_info->ui4_root_clus = ui4_root_clus;
            pt_fat_info->ui8_data_off = pt_fat_info->ui8_fat_off +
                                        pt_fat_info->ui4_fat_size * ui1_num_fat;
            pt_fat_info->ui4_root_size = 0;

            ui4_fat_sec_ns = (pt_fat_info->ui4_fat_size / ui4_sec_sz) * pt_fat_info->ui1_num_fat;
            ui4_prefetch_limit = (UINT32)(pt_fat_info->ui8_fat_off / (UINT64)ui4_sec_sz) + ui4_fat_sec_ns;

            /* For FAT32, the root directory can  be of variable size and is a cluster chain.*/
            /* 0x0FFFFFF7: BAD CLUSTER, 0x0FFFFFF8~0x0FFFFFFF: EOC*/
            for (ui4_this_clus = ui4_root_clus; ui4_this_clus < 0x0FFFFFF8; ui4_this_clus = ui4_next_clus)
            {
                i4_ret = _fat_dev_cache_read(pt_dev_xent,
                                             &ui4_val,
                                             4,
                                             pt_fat_info->ui8_fat_off + (ui4_this_clus * 4) + ui4_part_off,
                                             &ui4_done,
                                             pt_fat_info->h_lock,
                                             TRUE,
                                             ui4_prefetch_limit);
                if (i4_ret != FMR_OK)
                {
                    goto fail;
                }
                
                if (FAT_UINT32(ui4_val) <= ui4_root_clus) /* smaller than root cluster */
                {
                    i4_ret = FMR_FILE_SYSTEM_CRASH;
                    goto fail;
                }

                pt_fat_info->ui4_root_size += pt_fat_info->ui4_clus_size;

                ui4_next_clus = FAT_UINT32(ui4_val);
                /* What to do if ui4_next_clus is a BAD CLUSTER? */
            }
        }
    }

    x_mem_free(pui1_bs);

    return FMR_OK;

fail:

    x_mem_free(pui1_bs);

    return i4_ret;
}

static BOOL _fat_check_dentry(
    UINT8         *pui1_dent,
    FAT_FSINFO_T  *pt_fs_info)
{
    UINT32 ui4_first_clus;

    ASSERT(pui1_dent != NULL);

    if (FAT_GET_VAL8(pui1_dent, FAT_DENTRY_ATTR) > 0x3F)
    {
        return FALSE;
    }
#if 0
    if (FAT_GET_VAL8(pui1_dent, FAT_DENTRY_CRT_TIME_TENTH) > 199)
    {
        return FALSE;
    }
#endif
#if 0
    FAT_GET_VAL16(pui1_dent, FAT_DENTRY_CRT_TIME);
    FAT_GET_VAL16(pui1_dent, FAT_DENTRY_CRT_DATE);
    FAT_GET_VAL16(pui1_dent, FAT_DENTRY_ACC_DATE);
    FAT_GET_VAL16(pui1_dent, FAT_DENTRY_WRT_TIME);
    FAT_GET_VAL16(pui1_dent, FAT_DENTRY_WRT_DATE);
#endif

    ui4_first_clus = 
        (((UINT32)FAT_GET_VAL16(pui1_dent, FAT_DENTRY_CLUS_HI)) << 16) |
        (UINT32)FAT_GET_VAL16(pui1_dent, FAT_DENTRY_CLUS_LO);

    if (ui4_first_clus > (pt_fs_info->ui4_valid_clus_cnt + 1)) /* larger than max cluster # */
    {
        return FALSE;
    }

    return TRUE;

}

/*-----------------------------------------------------------------------------
 * Name: _fat_get_dentry
 *
 * Description: Convert from raw directory entry to FAT_DENTRY_T structure.
 *
 * Inputs:  pui1_dent
 *              Buffer of on disk raw data of a directory entry.
 *
 * Outputs: pt_dent
 *              Reference to a FAT_DENTRY_T object.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fat_get_dentry(
    FAT_DENTRY_T *pt_dent,
    UINT8        *pui1_dent)
{
    ASSERT(pt_dent != NULL);
    ASSERT(pui1_dent != NULL);

    x_memcpy(pt_dent->s_name, FAT_OFFSET(pui1_dent, FAT_DENTRY_NAME), 11);
    pt_dent->ui1_attr = FAT_GET_VAL8(pui1_dent, FAT_DENTRY_ATTR);
    pt_dent->ui1_crt_time_tenth = FAT_GET_VAL8(pui1_dent, FAT_DENTRY_CRT_TIME_TENTH);
    pt_dent->ui2_ctime = FAT_GET_VAL16(pui1_dent, FAT_DENTRY_CRT_TIME);
    pt_dent->ui2_cdate = FAT_GET_VAL16(pui1_dent, FAT_DENTRY_CRT_DATE);
    pt_dent->ui2_adate = FAT_GET_VAL16(pui1_dent, FAT_DENTRY_ACC_DATE);
    pt_dent->ui2_wtime = FAT_GET_VAL16(pui1_dent, FAT_DENTRY_WRT_TIME);
    pt_dent->ui2_wdate = FAT_GET_VAL16(pui1_dent, FAT_DENTRY_WRT_DATE);
    pt_dent->ui4_first_clus = 
        (((UINT32)FAT_GET_VAL16(pui1_dent, FAT_DENTRY_CLUS_HI)) << 16) |
        (UINT32)FAT_GET_VAL16(pui1_dent, FAT_DENTRY_CLUS_LO);
    pt_dent->ui4_size = FAT_GET_VAL32(pui1_dent, FAT_DENTRY_FILE_SIZE);

    pt_dent->t_clus_cache.ui4_head_pclus
        = pt_dent->t_clus_cache.ui4_tail_pclus
        = pt_dent->t_clus_cache.at_cache[0].ui4_pclus
        = pt_dent->ui4_first_clus;
    pt_dent->t_clus_cache.ui4_head_lclus
        = pt_dent->t_clus_cache.ui4_tail_lclus
        = pt_dent->t_clus_cache.at_cache[0].ui4_lclus
        = pt_dent->t_clus_cache.ui4_last_valid_entry
        = 0;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_set_dentry
 *
 * Description: Convert from FAT_DENTRY_T structure to on-disk format directory
 *              entry.
 *
 * Inputs:  pt_dent
 *              Reference to a FAT_DENTRY_T object.
 *
 * Outputs: pui1_dent
 *              Buffer of on disk raw data of a directory entry.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fat_set_dentry(
    UINT8           *pui1_dent,
    FAT_DENTRY_T    *pt_dent)
{
    ASSERT(pui1_dent != NULL);
    ASSERT(pt_dent != NULL);

    x_memcpy(FAT_OFFSET(pui1_dent, FAT_DENTRY_NAME), pt_dent->s_name, 11);
    FAT_SET_VAL8(pui1_dent, FAT_DENTRY_ATTR, pt_dent->ui1_attr);
    FAT_SET_VAL8(pui1_dent, FAT_DENTRY_NTRES, 0);
    FAT_SET_VAL8(pui1_dent, FAT_DENTRY_CRT_TIME_TENTH, pt_dent->ui1_crt_time_tenth);
    FAT_SET_VAL16(pui1_dent, FAT_DENTRY_CRT_TIME, pt_dent->ui2_ctime);
    FAT_SET_VAL16(pui1_dent, FAT_DENTRY_CRT_DATE, pt_dent->ui2_cdate);
    FAT_SET_VAL16(pui1_dent, FAT_DENTRY_ACC_DATE, pt_dent->ui2_adate);
    FAT_SET_VAL16(pui1_dent, FAT_DENTRY_WRT_TIME, pt_dent->ui2_wtime);
    FAT_SET_VAL16(pui1_dent, FAT_DENTRY_WRT_DATE, pt_dent->ui2_wdate);
    FAT_SET_VAL16(pui1_dent, FAT_DENTRY_CLUS_HI,  pt_dent->ui4_first_clus >> 16);
    FAT_SET_VAL16(pui1_dent, FAT_DENTRY_CLUS_LO,  pt_dent->ui4_first_clus & 0xFFFF);
    if (pt_dent->ui1_attr & FAT_ATTR_DIRECTORY)
    {
        FAT_SET_VAL32(pui1_dent, FAT_DENTRY_FILE_SIZE, 0);
    }
    else
    {
        FAT_SET_VAL32(pui1_dent, FAT_DENTRY_FILE_SIZE, pt_dent->ui4_size);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _fat_short_name_strcpy
 *
 * Description: Make FAT 8.3 short name from on-disk direcotry entry to
 *              a string buffer.
 *
 * Inputs:  pc_dent
 *              Reference to a FAT_DENTRY_T object.
 *
 * Outputs: pc_dst
 *              Buffer to hold the 8.3 format short name.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fat_short_name_strcpy(
    UINT8       *pc_dst,
    UINT8       *pc_dent,
    UINT8       ui1_nt_res)
{
    UINT8 *pc = pc_dst;
    INT32 i;

    ASSERT(pc_dst  != NULL);
    ASSERT(pc_dent != NULL);

    for (i = 0; i < 11; i++)
    {
        if (pc_dent[i] == 0x20)
        {
            if (i >= 8)
            {
                break;
            }
            continue;
        }
        if (i == 8)
        {
            *pc++ = (UINT8)('.');
        }

        if ((((i < 8) && (ui1_nt_res & 0x08)) || ((i >= 8) && (ui1_nt_res & 0x10))) &&
            ((pc_dent[i] >= 'A') && (pc_dent[i] <= 'Z')))
        {
            *pc++ = pc_dent[i] + 'a' - 'A';
            
        }
        else
        {
        *pc++ = pc_dent[i];
        }
    }
    *pc = 0;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_generate_numeric_tail
 *
 * Description: Generate file name in 8.3 format with numeric tail. To optimize
 *              the performance, we simply select the maximum number to append.
 *              That is, for example, if SAMPLE~1.TXT and SAMPLE~3.TXT exist,
 *              the new file name will be SAMPLE~4.TXT instead of SAMPLE~2.TXT.
 *              It does cause some problem, but it rarely happens since the 
 *              number ranges from 1 to 999999.
 *
 * Inputs:  pt_xent
 *              Reference to the xentry of the parent directory that will
 *              contain this new file.
 *          pui1_short_name
 *              The new file name string without numeric tail.
 *
 * Outputs: pui1_short_name
 *              The new file name with numeric tail.
 *
 * Returns: FMR_OK
 *              Success.
 ----------------------------------------------------------------------------*/
static INT32 _fat_generate_numeric_tail(
    FM_XENTRY_T *pt_xent,
    UINT8       *pui1_short_name)
{
    FM_RW_ARG_T t_io;
    UINT8       pui1_dent[32];
    UINT8       ui1_attr;
    UINT64      ui8_offset;
    INT32       i4_new_num;
    INT32       i4_can_num;
    INT32       i;
    INT32       j;
    INT32       i4_ret;

    ASSERT(pt_xent != NULL);
    ASSERT(pui1_short_name != NULL);

    ui8_offset = 0;
    i4_can_num = 1;
    while (1)
    {
        t_io.pt_xent        = pt_xent;
        t_io.pv_data        = pui1_dent;
        t_io.ui4_count      = 32;
        t_io.ui8_offset     = ui8_offset;
        t_io.ui4_done_bytes = 0;
        t_io.ui2_flag       = FM_IO_SYNC;
        t_io.h_bin_sema     = pt_xent->pt_mount->h_sync_sema;

        i4_ret = _fat_rw_buf(&t_io, TRUE, TRUE);
        if (i4_ret != FMR_OK)
        {
            break;
        }
        
        ASSERT(t_io.ui4_done_bytes == 32);
        ui8_offset += 32;

        if (pui1_dent[0] == 0x00)
        {
            break;
        }
        
        ui1_attr = FAT_GET_VAL8(pui1_dent, FAT_DENTRY_ATTR);

        if ((pui1_dent[0] == 0xE5) ||
            (ui1_attr == FAT_ATTR_VOLUME_ID) ||
            ((ui1_attr & FAT_ATTR_LONG_NAME_MASK) == (UINT8)(FAT_ATTR_LONG_NAME)))
        {
            continue;
        }

        if (pui1_dent[8] == pui1_short_name[8] &&
            pui1_dent[9] == pui1_short_name[9] &&
            pui1_dent[10] == pui1_short_name[10])
        {
            i4_new_num = 0;

            for (i = 0; i < 7; i++)
            {
                if (pui1_dent[i] == '~')
                {
                    for (j = i + 1; j < 8; j++)
                    {
                        i4_new_num = i4_new_num * 10 + (INT32)(pui1_dent[j] - '0');
                    }
                    if (i4_new_num >= i4_can_num)
                    {
                        i4_can_num = i4_new_num + 1;
                    }

                    break;
                }

                if (pui1_dent[i] != pui1_short_name[i])
                {
                    break;
                }
            }
        }
    }

    ASSERT((i4_can_num >= 1) && (i4_can_num <= 999999));
    if (i4_ret == 0)
    {
        for (i = 7; i > 1; i--)
        {
            pui1_short_name[i] = (UINT8)'0' + (UINT8)(i4_can_num % 10);
            i4_can_num /= 10;
            if (i4_can_num == 0)
            {
                i--;
                break;
            }
        }
        pui1_short_name[i] = (UINT8)'~';
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_is_not_dos_name
 *
 * Description: Check if the given file name is in dos 8.3 format.
 *
 * Inputs:  ps_utf8
 *              A null-terminated string.
 *
 * Outputs: -
 *
 * Returns: TRUE
 *              The string conforms to 8.3 format.
 *          FALSE
 *              The string does not conform to 8.3 format.
 ----------------------------------------------------------------------------*/
static BOOL _fat_is_dos_format(
    const UINT8 *ps_utf8)
{
    const UINT8 *pc;
    BOOL  b_has_dot   = FALSE;
    INT32 i4_name_len = 0;
    INT32 i4_ext_len  = 0;

    ASSERT(ps_utf8 != NULL);

    for (pc = ps_utf8; *pc != '\0'; pc++)
    {
        if (*pc == '.')
        {
            if (b_has_dot == TRUE)
            {
                return FALSE;
            }

            b_has_dot = TRUE;
        }
        else
        {
            if (b_has_dot == FALSE)
            {
                i4_name_len++;
            }
            else
            {
                i4_ext_len++;
            }
            if ((i4_name_len > 8) || (i4_ext_len > 3))
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_generate_dos_name
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static VOID _fat_generate_dos_name(
    FM_XENTRY_T     *pt_prnt_xent,
    UINT8           *ps_dos_name,
    const UINT8     *ps_utf8)
{
    UINT8 *ps_uname;
    BOOL b_lossy_conv;

    ASSERT(pt_prnt_xent != NULL);
    ASSERT(ps_dos_name != NULL);
    ASSERT(ps_utf8 != NULL);

    ps_uname = x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    if (ps_uname == NULL)
    {
        return ;
    }

    _fat_utf8_to_upper(ps_uname, ps_utf8);
    b_lossy_conv = _fat_utf8_to_oem(ps_uname, ps_uname);
    if (!b_lossy_conv)
    {
        b_lossy_conv = !_fat_is_dos_format(ps_utf8);
    }
    _fat_strip_space_and_period(ps_uname, ps_uname);
    _fat_copy_dos_name(ps_dos_name, ps_uname);

    if (b_lossy_conv == TRUE)
    {
        _fat_generate_numeric_tail(pt_prnt_xent, ps_dos_name);
    }

    x_mem_free(ps_uname);    
}


/*-----------------------------------------------------------------------------
 * Name: _fat_read_next_dentry
 *
 * Description: Read the next directory entry from the specified position in 
 *              the directory.          
 *
 * Inputs:  pt_xent
 *              Reference to the xentry of the directory to read.
 *          pui8_off
 *              Offset from the beginning of the directory.
 *
 * Outputs: pt_dent
 *              Reference to the FAT_DENTRY_T structure of this new entry.
 *          pui8_off
 *              The offset of next directory entry.
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 _fat_read_next_dentry(
    FM_XENTRY_T     *pt_xent,
    FAT_DENTRY_T    *pt_dent,
    UINT64          *pui8_off)
{
    UINT8   pui1_dent[32];
    UINT8   ui1_attr;
    UINT8   ui1_nt_res;
    UINT8   ui1_ent_num = 0;
    UINT8   ui1_chksum  = 0;
    INT32   i4_ret;

    FM_RW_ARG_T t_io;
    UINT8  *s_utf16 = NULL;
    UINT16 ui2_str_off;

    FAT_FSINFO_T *pt_fs_info = NULL;

    ASSERT(pt_xent != NULL);
    ASSERT(pt_dent  != NULL);
    ASSERT(pui8_off != NULL);
    ASSERT(((*pui8_off) & 31) == 0);
    ASSERT(pt_xent->pt_mount != NULL);
    ASSERT(pt_xent->pt_mount->pv_priv != NULL);

    pt_fs_info = (FAT_FSINFO_T *)pt_xent->pt_mount->pv_priv;
    /* [DTV139291] LongEntry number could be 20, the maxium space held by
       a dir entry'name could be 520(20*13*2) bytes; however, the maxium name
       length is still 255 chars */
    s_utf16 = (UINT8*)x_mem_alloc(2 * (FM_MAX_FILE_LEN + 6));

    if (s_utf16 == NULL)
    {
        return FMR_CORE;
    }

    while (1)
    {

        t_io.pt_xent        = pt_xent;
        t_io.pv_data        = pui1_dent;
        t_io.ui4_count      = 32;
        t_io.ui8_offset     = *pui8_off;
        t_io.ui4_done_bytes = 0;
        t_io.ui2_flag       = FM_IO_SYNC;
        t_io.h_bin_sema     = pt_xent->pt_mount->h_sync_sema;

        i4_ret = _fat_rw_buf(&t_io, TRUE, TRUE);
        if (i4_ret != FMR_OK)
        {
            goto EXIT;
        }
        
        ASSERT(t_io.ui4_done_bytes == 32);
        *pui8_off += 32;

        if (pui1_dent[0] == 0x00
            || (IS_XENTRY_MOUNT_ROOT(pt_xent) && pui1_dent[0] == '.'))
        {
            i4_ret = FMR_EOF;
            goto EXIT;
        }
        if (pui1_dent[0] == 0xE5)
        {
            ui1_ent_num = 0; /* reset long name entry */
            continue;
        }

        ui1_attr = FAT_GET_VAL8(pui1_dent, FAT_DENTRY_ATTR);
        ui1_nt_res = FAT_GET_VAL8(pui1_dent, FAT_DENTRY_NTRES);

#if 0
        if (ui1_attr == FAT_ATTR_VOLUME_ID)
        {
            ui1_ent_num = 0; /* reset long name entry */
            continue; /* skip this one */
        }
#endif        
        if (((ui1_attr & FAT_ATTR_LONG_NAME_MASK) == (UINT8)FAT_ATTR_LONG_NAME) ||
            (ui1_ent_num != 0))
        {
            if ((*pui1_dent & 0x40) &&
                ((ui1_attr & FAT_ATTR_LONG_NAME_MASK) == (UINT8)FAT_ATTR_LONG_NAME)) /*(ui1_ent_num == 0)*/
            {
                ui1_chksum  = FAT_GET_VAL8(pui1_dent, FAT_LDENTRY_CHKSUM);
                ui1_ent_num = (UINT8)((FAT_GET_VAL8(pui1_dent, FAT_LDENTRY_ORDER) & 0x3F) + 1);
                /* The maxium number of the long entries is 20.  */
                if (ui1_ent_num <= 21)
                {
                pt_dent->ui1_use_dent = ui1_ent_num; /* including short entry */
                    x_memset(s_utf16, 0, 2 * (FM_MAX_FILE_LEN + 6));
                }
                else
                {
                    ui1_ent_num = 0;
                    continue;
                }
            }

            if (ui1_ent_num <= 1)
            {
                /* By ChunZhou: Make sure it is short entry */
                if ((ui1_attr & FAT_ATTR_LONG_NAME_MASK) != (UINT8)FAT_ATTR_LONG_NAME)
                {
                    /* [DTV139934] invalid directory entry name */
                    if (((pui1_dent[0] <= 0x20) && (pui1_dent[0] != 0x05)) ||
                        (pui1_dent[0] == 0x22) || 
                        ((pui1_dent[0] >= 0x2A) && (pui1_dent[0] <= 0x2C)) ||
                        (pui1_dent[0] == 0x2E) ||
                        ((pui1_dent[0] >= 0x3A) && (pui1_dent[0] <= 0x3F)) ||
                        ((pui1_dent[0] >= 0x5B) && (pui1_dent[0] <= 0x5D)) ||
                        (pui1_dent[0] == 0x7C))
                    {
                        ui1_ent_num = 0; /* reset long name entry */
                        continue;        /* invalid directory entry name, skip it */
                    }

                /* short dir entry */
                if (ui1_chksum != _fat_longname_chksum(pui1_dent))
                {
                    /* Skip this long entry */
                        /* By ChunZhou,BUG here:[DTV00137979]
                           "An 8-bit checksum is computed on the name contained in the short directory entry
                           at the time the short and long directory entries are created. All 11 characters 
                           of the name in the short entry are used in the checksum calculation. The check 
                           sum is placed in every long entry. If any of the check sums in the set of long 
                           entries do not agree with the computed checksum of the name contained in the 
                           short entry, then the long entries are treated as orphans."
                           So I should "treat the long entries as orphans", but the short entry still makes
                           sense.
                        */
                        ui1_ent_num = 0;
    
                        /* Added by ChunZhou {*/
                        *pui8_off -= 32;
                        /* Added by ChunZhou }*/
                        continue;
                    }

                    if (_fat_check_dentry(pui1_dent, pt_fs_info) == FALSE)
                    {
                        /* Skip this long entry */
                    ui1_ent_num = 0;
                    continue;
                }

                _fat_trans_unicode_endian(s_utf16, s_utf16);
                pt_dent->ui2_len = (UINT16)x_uc_w2s_strlen((const UTF16_T *)s_utf16);
                i4_ret = x_uc_w2s_to_ps((const UTF16_T *)s_utf16, (CHAR *)pt_dent->s_utf8, 4 * (FM_MAX_FILE_LEN + 1));
                ASSERT(i4_ret == UCR_OK);
                pt_dent->ui4_name_off = (UINT32)*pui8_off - 32;
                break;  /* found a long name dir entry */
            }
            else
            {
                    /* Skip this long entry */
                    ui1_ent_num = 0;
                    continue;
                }
            }
            else
            {
                if (ui1_chksum != FAT_GET_VAL8(pui1_dent, FAT_LDENTRY_CHKSUM))
                {
                    /* Skip remaining long entries */
                    *pui8_off += ((ui1_ent_num - 1) * 32);
                    ui1_ent_num = 0;
                    continue;
                }

                ui2_str_off = (UINT16)((ui1_ent_num - 2) * 26); /* 13 unicode characters */
            
                /*
                 *  The name is currently based on little endian,
                 *  UCS2 encoding.
                 */
                x_memcpy(&s_utf16[ui2_str_off],
                         FAT_OFFSET(pui1_dent, FAT_LDENTRY_NAME1),
                         10);
                x_memcpy(&s_utf16[ui2_str_off + 10],
                         FAT_OFFSET(pui1_dent, FAT_LDENTRY_NAME2),
                         12);
                x_memcpy(&s_utf16[ui2_str_off + 22],
                         FAT_OFFSET(pui1_dent, FAT_LDENTRY_NAME3),
                         4);
            }

            ui1_ent_num--;
            continue;
        }
        else if ( (ui1_attr & FAT_ATTR_VOLUME_ID))
        {
            ui1_ent_num = 0;
            continue;
        }

        if ( (FALSE == IS_XENTRY_MOUNT_ROOT(pt_xent) )
            && pui1_dent[0] == 0x2E
            && pui1_dent[2] == 0x20 && pui1_dent[3] == 0x20
            && pui1_dent[4] == 0x20 && pui1_dent[5] == 0x20
            && pui1_dent[6] == 0x20 && pui1_dent[7] == 0x20
            && pui1_dent[8] == 0x20 && pui1_dent[9] == 0x20
            && pui1_dent[10] == 0x20)
        {
            /* "." or ".." in sub-directory */
            ui1_ent_num = 0;
        }
        else if (((pui1_dent[0] <= 0x20) && (pui1_dent[0] != 0x05)) ||
            (pui1_dent[0] == 0x22) || 
            ((pui1_dent[0] >= 0x2A) && (pui1_dent[0] <= 0x2C)) ||
            (pui1_dent[0] == 0x2E) ||
            ((pui1_dent[0] >= 0x3A) && (pui1_dent[0] <= 0x3F)) ||
            ((pui1_dent[0] >= 0x5B) && (pui1_dent[0] <= 0x5D)) ||
            (pui1_dent[0] == 0x7C))
        {
            ui1_ent_num = 0; /* reset long name entry */
            continue;        /* invalid directory entry name, skip it */
        }

        pt_dent->ui1_use_dent = 1;
        
        _fat_short_name_strcpy(pt_dent->s_utf8, pui1_dent, ui1_nt_res);
        pt_dent->ui2_len = (UINT16)x_strlen((const CHAR *)pt_dent->s_utf8);
        pt_dent->ui4_name_off = (UINT32)*pui8_off - 32;

        if (_fat_check_dentry(pui1_dent, pt_fs_info) == FALSE)
        {
            /* Skip this entry */
            ui1_ent_num = 0;
            continue;
        }

        break; /* found a short name dir entry */
    }

EXIT:

    if (i4_ret == FMR_OK)
    {
        _fat_get_dentry(pt_dent, pui1_dent);

        if (pt_dent->ui1_attr & FAT_ATTR_DIRECTORY)
        {
            UINT32 ui4_this_clus;
            UINT32 ui4_next_clus;

            UINT32 ui4_clus_size;
            FAT_TYPE_T e_fat_type;

            UINT32 ui4_eoc = 0;
            
            ui4_clus_size = ((FAT_FSINFO_T *)pt_xent->pt_mount->pv_priv)->ui4_clus_size;

#if 0
            if (ui4_clus_size == 4) 
                ui4_eoc = 0x0ffffff8;
            else
                ui4_eoc = 0xfff8;
#else
            e_fat_type = ((FAT_FSINFO_T *)pt_xent->pt_mount->pv_priv)->e_fat_type;

            if (e_fat_type == FAT_TYPE_12)
            {
                ui4_eoc = FAT12_EOC_MARK;
            }
            else if (e_fat_type == FAT_TYPE_16)
            {
                ui4_eoc = FAT16_EOC_MARK;
            }
            else if (e_fat_type == FAT_TYPE_32)
            {
                ui4_eoc = FAT32_EOC_MARK;
            }
            else
            {
                ASSERT(0);
            }
#endif
            
            pt_dent->ui4_size = 0;
 
            for (ui4_this_clus = pt_dent->ui4_first_clus;  ui4_this_clus < ui4_eoc; ui4_this_clus = ui4_next_clus)
            {
                ui4_next_clus = _fat_read_fat_entry(pt_xent->pt_mount, ui4_this_clus);
                /* <Fix for DTV00060197> */
                if(ui4_next_clus == 0)
                {
                    /* Fail to read the FAT entry. */
                    break;
                }

                pt_dent->ui4_size += ui4_clus_size;
            }
            
        }
    }

    x_mem_free( s_utf16);

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_lookup_entry
 *
 * Description: Look up an directory entry with name in the specified
 *              direcotry.
 *
 * Inputs:  pt_xent
 *              Reference to the xentry of the directory to be searched.
 *          ps_name
 *              The UTF-8 string of name to search.
 *
 * Outputs: -
 *
 * Returns: Reference to the FAT_DENTRY_T structure of the found entry. It
 *          returns NULL if the specified entry is not found.
 ----------------------------------------------------------------------------*/
static FAT_DENTRY_T *_fat_lookup_entry(
    FM_XENTRY_T     *pt_xent,
    const CHAR      *ps_name)
{
    FAT_DENTRY_T    *pt_dent = NULL;
    UINT64          ui8_off = 0;
    static UINT64   ui8_last_off = 0;
    INT32           i4_ret = FMR_OK;

    DEBUG_TRACE();
    
    ASSERT(pt_xent != NULL);
    ASSERT(ps_name != NULL);

    pt_dent = FAT_GET_DENTRY(pt_xent);

    if ((pt_dent->ui1_attr & (FAT_ATTR_DIRECTORY|FAT_ATTR_VOLUME_ID)) == 0)
    {
        return NULL;
    }

    pt_dent = x_mem_calloc(1, sizeof(FAT_DENTRY_T));
    
    /* Search from last offset first, and then from begining*/
    while (1)
    {
        i4_ret = _fat_read_next_dentry(pt_xent, pt_dent, &ui8_last_off);
        if (i4_ret != FMR_OK)
        {
            break;
        }

        if (!_fat_strcasecmp(ps_name, (const CHAR *)pt_dent->s_utf8))
        {
            return pt_dent;
        }
    }

    while (1)
    {
        i4_ret = _fat_read_next_dentry(pt_xent, pt_dent, &ui8_off);
        if (i4_ret != FMR_OK)
        {
            break;
        }

        if (!_fat_strcasecmp(ps_name, (const CHAR *)pt_dent->s_utf8))
        {
            ui8_last_off = ui8_off;
            return pt_dent;
        }
    }

    x_mem_free(pt_dent);

    return NULL;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_make_xentry
 *
 * Description: Make a new xentry for the given FAT node.
 *
 * Inputs:  pt_prnt_xent
 *              Reference to the xentry of the directory which the new node
 *              belongs to.
 *          pt_dent
 *              Reference to the new FAT node.
 *
 * Outputs: -
 *
 * Returns: Reference to the new xentry object. It returns NULL on fail.
 ----------------------------------------------------------------------------*/
static FM_XENTRY_T *_fat_make_xentry(
    FM_XENTRY_T     *pt_prnt_xent,
    FAT_DENTRY_T    *pt_dent)
{
    FM_XENTRY_T *pt_xent;
    
    ASSERT(pt_prnt_xent != NULL);
    ASSERT(pt_prnt_xent->pt_xent_ops != NULL);
    ASSERT(pt_prnt_xent->pt_mount != NULL);
    ASSERT(pt_prnt_xent->pt_mount->pt_dev_xent != NULL);
    ASSERT(pt_dent != NULL);

    pt_xent = fm_get_new_xentry((CHAR *)pt_dent->s_utf8);
    if (pt_xent != NULL)
    {
        pt_xent->ui4_mode     = _fat_attr_to_ufs_mode(pt_dent->ui1_attr);
        pt_xent->pt_mount     = pt_prnt_xent->pt_mount;
        pt_xent->pt_ufs_ops   = pt_prnt_xent->pt_ufs_ops;
        pt_xent->pv_fs_node   = pt_dent;
        pt_xent->pt_prnt_xent = pt_prnt_xent;
        pt_xent->pt_xent_ops  = pt_prnt_xent->pt_xent_ops;
        pt_xent->pt_dev       = pt_prnt_xent->pt_mount->pt_dev_xent->pt_dev;
    }

    return pt_xent;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_attr_to_ufs_mode
 *
 * Description:  Convert FAT attributes to UFS mode.
 *
 * Inputs:  ui1_attr
 *              FAT file attributes.
 *
 * Outputs: -
 *
 * Returns: UFS mode.
 ----------------------------------------------------------------------------*/
static UINT32 _fat_attr_to_ufs_mode(
    UINT8       ui1_attr)
{
    UINT32 ui4_mode;

    ui4_mode = (UINT32)((ui1_attr & FAT_ATTR_READ_ONLY) ? 0555 : 0777);

    if ((ui1_attr & FAT_ATTR_DIRECTORY) || (ui1_attr & FAT_ATTR_VOLUME_ID))
    {
        ui4_mode |= FM_MODE_TYPE_DIR;
    }
    else
    {
        ui4_mode |= FM_MODE_TYPE_FILE;
    }
    
    return ui4_mode;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_conv_to_utc_time
 *
 * Description: Convert FAT datetime to utc time.
 *
 * Inputs:  ui2_date
 *              FAT date.
 *          ui2_time
 *              FAT time.
 *
 * Outputs: -
 *
 * Returns: UTC time.
 ----------------------------------------------------------------------------*/
static UINT32 _fat_conv_to_utc_time(
    UINT16          ui2_date,
    UINT16          ui2_time,
    UINT8           ui1_time_tenth)
{
    UINT32 ui4_day;
    UINT32 ui4_month;
    UINT32 ui4_year;
    UINT32 ui4_sec;
    
    if (ui2_date == 0)
    {
        return 0;
    }

    ui4_sec = (UINT32)((((ui2_time & 0x1F) << 1) + ((ui1_time_tenth >= 100)?(1):(0))) + /* seconds */
              ((ui2_time & 0x7E0) >> 5) * 60 + /* minutes */
                       ((ui2_time & 0xF800) >> 11) * 3600); /* hours */
    
    ui4_day  = 0;
    ui4_year = (ui2_date & 0xFE00) >> 9;
    ui4_day  = ui4_year * 365;
    ui4_day += ui4_year / 4 + 1; /* add in leap days */
    if ((ui4_year & 0x03) == 0)
    {
        ui4_day--;  /* if year is a leap year */
    }
    ui4_month = (ui2_date & 0x1E0) >> 5;
    if (ui4_month < 1 || ui4_month > 12)
    {
        ui4_month = 1;
    }
    if (ui4_month > 1)
    {
        ui4_day += (ui4_year & 0x03) ?
                   aui2_reg_year[ui4_month - 2] :
                   aui2_leap_year[ui4_month - 2];
    }
    ui4_day += (ui2_date & 0x1F) - 1;
    
    return ui4_sec + (ui4_day * 24 * 60 * 60) + FAT_SECONDS_TO_1980;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_conv_to_fat_time
 *
 * Description: Convert UTC time to FAT datetime.
 *
 * Inputs:  ui4_utc_sec
 *              UTC datetime in seconds.
 *
 * Outputs: -
 *
 * Returns: The 32 bits integer, FAT date is in the high 16 bits and FAT time 
 *          is in the low 16 bits.
 ----------------------------------------------------------------------------*/
static UINT32 _fat_conv_to_fat_time(
    UINT32  ui4_utc_sec)
{
    UINT32 ui4_day;
    UINT32 ui4_month;
    UINT32 ui4_year;

    UINT32 ui4_date;
    UINT32 ui4_time;

    ui4_time = ((ui4_utc_sec / 2) % 30) +
               (((ui4_utc_sec / 60) % 60) << 5) +
               (((ui4_utc_sec / (60 * 60)) % 24) << 11);
    ui4_day = ui4_utc_sec / (60 * 60 * 24);
    for (ui4_year = 1970; ; ui4_year++)
    {
        UINT32 ui4_day_this_year;

        ui4_day_this_year = (UINT32)((ui4_year & 0x03) ? 365 : 366);
        if (ui4_day < ui4_day_this_year)
        {
            break;
        }
        ui4_day -= ui4_day_this_year;
    }
    if (ui4_year & 0x03)
    {
        for (ui4_month = 0; ui4_day >= aui2_reg_year[ui4_month]; ui4_month++) {}
        if (ui4_month > 0)
        {
            ui4_day -= aui2_reg_year[ui4_month - 1];
        }
    }
    else
    {
        for (ui4_month = 0; ui4_day >= aui2_leap_year[ui4_month]; ui4_month++) {}
        if (ui4_month > 0)
        {
            ui4_day -= aui2_leap_year[ui4_month - 1];
        }
    }

    ui4_date = (ui4_day + 1) + ((ui4_month + 1) << 5) + 
               ((ui4_year > 1980) ? ((ui4_year - 1980) << 9) : 0);
    
    return ((ui4_date << 16) | (ui4_time & 0xFFFF));
}


/*-----------------------------------------------------------------------------
 * Name: _fat_read_fat_entry
 *
 * Description: This function returns value of the specified cluster in
 *              FAT. This value is the next cluster where the following data
 *              locates.
 *
 * Inputs:  pt_mount
 *              Reference to the FM_MOUNT_T object.
 *          ui4_clus
 *              The cluster number in FAT. Normally, it should be greater
 *              than or equal to 2 because 0 and 1 is reserved.
 *
 * Outputs: -
 *
 * Returns: 0
 *              Fail to read the FAT entry.
 *          FAT_EOC_MARK
 *              No more clusters behind.
 *          Other values
 *              Next cluster number.
 ----------------------------------------------------------------------------*/
static UINT32 _fat_read_fat_entry(
    FM_MOUNT_T      *pt_mount,
    UINT32          ui4_clus)
{
    FAT_FSINFO_T *pt_fat_info;
    UINT32 ui4_next_clus;
    UINT32 ui4_ent_nib_len;
    UINT32 ui4_val;
    UINT32 ui4_done;
    INT32  i4_ret;
    UINT16 ui2_val;

    UINT32 ui4_sec_sz;
    UINT32 ui4_prefetch_limit;
    UINT32 ui4_fat_sec_ns;

    ASSERT(pt_mount != NULL);
    
    pt_fat_info = (FAT_FSINFO_T *)pt_mount->pv_priv;

    if (IS_XENTRY_BLK(pt_mount->pt_dev_xent))
    {
        ui4_sec_sz = (UINT32)pt_mount->pt_dev_xent->pt_dev->u_param.t_blkdev.ui4_sec_size;
    }
    else
    {
        ui4_sec_sz = (UINT32)512;
    }

    ASSERT(pt_fat_info != NULL);

    switch (pt_fat_info->e_fat_type)
    {
        case FAT_TYPE_12:
            ui4_ent_nib_len = 3;
            break;
        case FAT_TYPE_16:
            ui4_ent_nib_len = 4;
            break;
        case FAT_TYPE_32:
            ui4_ent_nib_len = 8;
            break;
        default:
            return 0;
    }

    ui4_fat_sec_ns = (pt_fat_info->ui4_fat_size / ui4_sec_sz) * pt_fat_info->ui1_num_fat;
    ui4_prefetch_limit = (UINT32)(pt_fat_info->ui8_fat_off / (UINT64)ui4_sec_sz) + ui4_fat_sec_ns;

    if (ui4_ent_nib_len == 8)
    {
        i4_ret = _fat_dev_cache_read(pt_mount->pt_dev_xent,
                                     &ui4_val,
                                     (ui4_ent_nib_len / 2),
                                     pt_fat_info->ui8_fat_off + (ui4_clus * (ui4_ent_nib_len / 2)),
                                     &ui4_done,
                                     pt_fat_info->h_lock,
                                     TRUE,
                                     ui4_prefetch_limit);

        if (i4_ret != FMR_OK)
    {
            return 0;
    }

        ui4_next_clus = FAT_UINT32(ui4_val);

        if (ui4_next_clus >= 0x0FFFFFF0)
        {
            ui4_next_clus = FAT32_EOC_MARK; 
        }
    }
    else if (ui4_ent_nib_len == 4)
    {
        i4_ret = _fat_dev_cache_read(pt_mount->pt_dev_xent,
                                     &ui2_val,
                                     (ui4_ent_nib_len / 2),
                                     pt_fat_info->ui8_fat_off + (ui4_clus * (ui4_ent_nib_len / 2)),
                                     &ui4_done,
                                     pt_fat_info->h_lock,
                                     TRUE,
                                     ui4_prefetch_limit);

        if ( i4_ret != FMR_OK)
        {
            return 0;
        }

        ui4_next_clus = (UINT32)FAT_UINT16(ui2_val);

        if (ui4_next_clus >= 0xFFF0)  
        {
            ui4_next_clus = FAT16_EOC_MARK;
        }
    }
    else
    {
        i4_ret = _fat_dev_cache_read(pt_mount->pt_dev_xent,
                                     &ui2_val,
                                     sizeof(UINT16),
                                     pt_fat_info->ui8_fat_off + ((ui4_clus * ui4_ent_nib_len) / 2),
                                     &ui4_done,
                                     pt_fat_info->h_lock,
                                     FALSE,
                                     0);

        if ( i4_ret != FMR_OK)
        {
            return 0;
        }

        if ((ui4_clus % 2) == 0)
        {
            ui2_val = ui2_val & 0x0FFF;
        }
        else
        {
            ui2_val = ui2_val >> 4;
        }

        ui4_next_clus = (UINT32)FAT_UINT16(ui2_val);

        if (ui4_next_clus >= 0x0FF0)  
        {
            ui4_next_clus = FAT12_EOC_MARK;
        }
    }
    
    return ui4_next_clus;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_extend_fat_chain
 *
 * Description: Find a free FAT entry and write EOC to it.
 *
 * Inputs:  pt_mount
 *              Reference to the FM_MOUNT_T object.
 *          pt_dent
 *              Reference to FAT_DENTRY_T object.
 *          ui4_entry
 *              Number of entires to extend.
 *          ui4_from_lclus
 *              The ID of last FAT entry in the cache.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 ----------------------------------------------------------------------------*/
static INT32 _fat_extend_fat_chain(
    FM_MOUNT_T      *pt_mount,
    FAT_DENTRY_T    *pt_dent,
    UINT32          ui4_entry,
    UINT32          ui4_from_lclus,
    BOOL            b_zero_out)
{
    FAT_FSINFO_T    *pt_fat_info;
    UINT32 ui4_this_pclus = 0;
    UINT32 ui4_ent_nib_len;
    UINT32 ui4_start_clus;
    UINT32 ui4_clus;
    UINT32 ui4_done;
    UINT32 ui4_cnt;
    UINT32 ui4_val;
    UINT16 ui2_val;
    UINT8  *pui1_zero = NULL;
    UINT8  *pui1_fat_ptr = NULL;
    UINT8  *pui1_fat_entries = NULL; /* reduce times of reading fat entry */
    UINT32 ui4_fat_entries_len = 0;
    UINT8  i;
    INT32  i4_ret;

    UINT64 ui8_extended_size;

    BOOL b_fat_entries_dirty = FALSE;
    UINT32 ui4_fat_entries_start_pclus = 0;
    UINT32 ui4_fat_entries_end_pclus = 0;

    ASSERT(pt_mount != NULL);
    ASSERT(pt_dent != NULL);
    
    pt_fat_info = (FAT_FSINFO_T *)pt_mount->pv_priv;

    ASSERT(pt_fat_info != NULL);

    if (ui4_entry == 0)
    {
        return FMR_OK;
    }

    if ((pt_mount->t_fs_info.ui8_avail_blk == 0)||(pt_mount->t_fs_info.ui8_free_blk == 0))
    {
        return FMR_FILE_SYSTEM_FULL;
    }

    switch (pt_fat_info->e_fat_type)
    {
        case FAT_TYPE_32:
            ui4_ent_nib_len = 8;
            ui4_fat_entries_len = 32*512;
            break;
        case FAT_TYPE_16:
            ui4_ent_nib_len = 4;
            ui4_fat_entries_len = 16*512;
            break;
        case FAT_TYPE_12:
            ui4_ent_nib_len = 3;
            ui4_fat_entries_len = 510;  //  dividable by 3
            break;
        default:
            return FMR_INVAL;
    }

    pui1_fat_entries = x_mem_alloc(ui4_fat_entries_len);

    if (pui1_fat_entries == NULL)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }

    x_memset(pui1_fat_entries, 0, ui4_fat_entries_len);

    ui8_extended_size = ((UINT64)(ui4_from_lclus + ui4_entry) * (UINT64)pt_fat_info->ui4_clus_size);

    if (ui8_extended_size > (UINT64)(0xffffffff))
    {
        i4_ret = FMR_FILE_TOO_LARGE;
        goto FAIL;
    }

    if (b_zero_out)
    {
        pui1_zero = x_mem_calloc(1, pt_fat_info->ui4_clus_size);

        if (pui1_zero == NULL)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }
    }

    if (ui4_from_lclus != 0)
    {
        i4_ret = _fat_get_pclus( &ui4_this_pclus, ui4_from_lclus, pt_mount, pt_dent);

        ASSERT((FMR_EOC != i4_ret) && (FMR_ABNORMAL_ENDED != i4_ret));

        if (FMR_OK != i4_ret)
        {
            goto FAIL;
        }

        ASSERT(ui4_this_pclus >= 2);
    }
    else
        {
        ui4_this_pclus = pt_dent->ui4_first_clus;
    }

    if (pt_dent->ui4_last_pclus != 0)
    {
        ui4_start_clus = pt_dent->ui4_last_pclus + 1;
        }
        else
        {
        ui4_start_clus = 2;
    }

    if (ui4_ent_nib_len == 3)
    {
        if ((ui4_start_clus % 2) != 0)
            {
            ui4_start_clus -= 1;
        }
    }

    ui4_clus = ui4_start_clus;
    pui1_fat_ptr = pui1_fat_entries;

    for (ui4_cnt = 0; ui4_cnt < ui4_entry;)
    {
#if 0 // new cache write method
        if (ui4_clus > (pt_fat_info->ui4_valid_clus_cnt + 1)) /* larger than max cluster number */
        {
            if (ui4_start_clus == 2)
            {
                i4_ret = FMR_FILE_SYSTEM_FULL; /* no more free FAT entry */
                goto FAIL;
            }
            else
            {
                /* search from the beginning */
                ui4_start_clus = 2;
                ui4_clus = 2;
                pui1_fat_ptr = pui1_fat_entries;

                continue;
            }
        }

        /*
         *  Read multiple FAT entries to reduce read times.
         */
        if (pui1_fat_ptr == pui1_fat_entries)
        {
            if (ui4_ent_nib_len == 3)
        {
                ASSERT((ui4_clus % 2) == 0);
            }
        
            i4_ret = _fat_dev_cache_read(pt_mount->pt_dev_xent,
                                         pui1_fat_entries,
                                         ui4_fat_entries_len,
                                         pt_fat_info->ui8_fat_off + ((ui4_clus * ui4_ent_nib_len) / 2),
                                         &ui4_done,
                                         pt_mount->h_sync_sema,
                                         FALSE,
                                         0);
                if (i4_ret != FMR_OK)
                {
                    goto FAIL;
                }
            }

        if (ui4_ent_nib_len == 8)
        {
            ui4_val = FAT_GET_VAL32(pui1_fat_ptr, 0);
            pui1_fat_ptr = ((pui1_fat_ptr + (ui4_ent_nib_len / 2) - pui1_fat_entries) % ui4_fat_entries_len) + pui1_fat_entries;
        }
        else if (ui4_ent_nib_len == 4)
        {
            ui4_val = (UINT32)FAT_GET_VAL16(pui1_fat_ptr, 0);
            pui1_fat_ptr = ((pui1_fat_ptr + (ui4_ent_nib_len / 2) - pui1_fat_entries) % ui4_fat_entries_len) + pui1_fat_entries;
    }
    else
    {
            if ((ui4_clus % 2) == 0)
            {
                ui4_val = (UINT32)(FAT_GET_VAL16(pui1_fat_ptr, 0) & 0x0FFF);
                ui2_val = FAT_GET_VAL16(pui1_fat_ptr, 0);
                pui1_fat_ptr = ((pui1_fat_ptr + 1 - pui1_fat_entries) % ui4_fat_entries_len) + pui1_fat_entries;
            }
            else
            {
                ui4_val = (UINT32)(FAT_GET_VAL16(pui1_fat_ptr, 0) >> 4);
                ui2_val = FAT_GET_VAL16(pui1_fat_ptr, 0);
                pui1_fat_ptr = ((pui1_fat_ptr + 2 - pui1_fat_entries) % ui4_fat_entries_len) + pui1_fat_entries;
            }
        }

        /*
         *  Check if it is a free entry
        */
        if (ui4_val == 0)
        {
            ui4_cnt++;

            if (ui4_cnt == ui4_entry)
            {
                if (ui4_ent_nib_len == 8)
                {
                    UINT32 ui4_fat_val;

                /* The last FAT entry, write EOC */
                    ui4_fat_val = FAT_UINT32(FAT32_EOC_MARK);

                for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                {
                    i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                      &ui4_fat_val,
                                                      (ui4_ent_nib_len / 2),
                                                      pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_clus * (ui4_ent_nib_len / 2)),
                                                      &ui4_done,
                                                      pt_mount->h_sync_sema,
                                                      FALSE,
                                                      0);
                    if (i4_ret != FMR_OK)
                    {
                        goto FAIL;
                    }
    }
                }
                else if (ui4_ent_nib_len == 4)
                {
                    UINT16 ui2_fat_val;

                    /* The last FAT entry, write EOC */
                    ui2_fat_val = FAT_UINT16(FAT16_EOC_MARK);
    
                    for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
    {
                        i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                      &ui2_fat_val,
                                                      (ui4_ent_nib_len / 2),
                                                      pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_clus * (ui4_ent_nib_len / 2)),
                                                      &ui4_done,
                                                      pt_mount->h_sync_sema,
                                                      FALSE,
                                                      0);
                        if (i4_ret != FMR_OK)
                {
                            goto FAIL;
                        }
                    }
                    
    }
    else
    {
                    UINT16 ui2_fat_val;

                    /* The last FAT entry, write EOC */
                    ui2_fat_val = FAT_UINT16(FAT12_EOC_MARK);

                    if ((ui4_clus % 2) == 0)
                    {
                        ui2_fat_val = ui2_fat_val & 0x0FFF;
                        ui2_val = ui2_val & 0xF000;
                        ui2_val = ui2_val | ui2_fat_val;
                    }
                    else
                    {
                        ui2_fat_val = ui2_fat_val << 4;
                        ui2_val = ui2_val & 0x000F;
                        ui2_val = ui2_val | ui2_fat_val;
    }

                    for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                    {
                        i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                      &ui2_val,
                                                      sizeof(UINT16),
                                                      pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + ((ui4_clus * ui4_ent_nib_len) / 2),
                                                      &ui4_done,
                                                      pt_mount->h_sync_sema,
                                                      FALSE,
                                                      0);
                        if (i4_ret != FMR_OK)
                        {
                            goto FAIL;
                        }
                    }
                }

                pt_dent->ui4_last_pclus = ui4_clus;
            }
            
            if (ui4_this_pclus == 0)
            {
                /* The whole new dentry */
                pt_dent->ui4_first_clus = ui4_clus;

                pt_dent->t_clus_cache.ui4_head_lclus
                    = pt_dent->t_clus_cache.ui4_tail_lclus
                    = pt_dent->t_clus_cache.ui4_last_valid_entry
                    = pt_dent->t_clus_cache.at_cache[0].ui4_lclus = 0;
                pt_dent->t_clus_cache.ui4_head_pclus
                    = pt_dent->t_clus_cache.ui4_tail_pclus
                    = pt_dent->t_clus_cache.at_cache[0].ui4_pclus 
                    = pt_dent->ui4_first_clus;
                    
                ASSERT(ui4_from_lclus == 0);
                /* ASSERT(ui4_cnt == 1); */
            }
            else
            {

                /* for a case: the found next entry (ui4_clus) is the same as the 
                 * previous entry.  It happens when there are only one free entry left
                 * and client needs more than one free entry.  The check routine checks
                 * from beginning again and found the same entry.  No entry available.
                 */

                if (ui4_this_pclus == ui4_clus)
                {
#if 0
                    ui4_cnt--;
                    ui4_clus++;

                    pt_mount->t_fs_info.ui8_avail_blk++;
                    pt_mount->t_fs_info.ui8_free_blk++;

                    continue;
#else
                    /* although there are no enough free cluster, we still need to enclose the
                       cluster chain. */

                    if (ui4_ent_nib_len == 8)
                    {
                        UINT32 ui4_fat_val;

                        /* The last FAT entry, write EOC */
                        ui4_fat_val = FAT_UINT32(FAT32_EOC_MARK);

                        for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                        {
                            i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                          &ui4_fat_val,
                                                          (ui4_ent_nib_len / 2),
                                                          pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_clus * (ui4_ent_nib_len / 2)),
                                                          &ui4_done,
                                                          pt_mount->h_sync_sema,
                                                          FALSE,
                                                          0);
                            if (i4_ret != FMR_OK)
                            {
                                goto FAIL;
                            }
                        }
                    }
                    else if (ui4_ent_nib_len == 4)
                    {
                        UINT16 ui2_fat_val;

                        /* The last FAT entry, write EOC */
                        ui2_fat_val = FAT_UINT16(FAT16_EOC_MARK);

                        for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                        {
                            i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                          &ui2_fat_val,
                                                          (ui4_ent_nib_len / 2),
                                                          pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_clus * (ui4_ent_nib_len / 2)),
                                                          &ui4_done,
                                                          pt_mount->h_sync_sema,
                                                          FALSE,
                                                          0);
                            if (i4_ret != FMR_OK)
                            {
                                goto FAIL;
                            }
                        }

                    }
                    else
                    {
                        UINT16 ui2_fat_val;

                        /* The last FAT entry, write EOC */
                        ui2_fat_val = FAT_UINT16(FAT12_EOC_MARK);

                        if ((ui4_clus % 2) == 0)
                        {
                            ui2_fat_val = ui2_fat_val & 0x0FFF;
                            ui2_val = ui2_val & 0xF000;
                            ui2_val = ui2_val | ui2_fat_val;
                        }
                        else
                        {
                            ui2_fat_val = ui2_fat_val << 4;
                            ui2_val = ui2_val & 0x000F;
                            ui2_val = ui2_val | ui2_fat_val;
                        }

                        for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                        {
                            i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                          &ui2_val,
                                                          sizeof(UINT16),
                                                          pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + ((ui4_clus * ui4_ent_nib_len) / 2),
                                                          &ui4_done,
                                                          pt_mount->h_sync_sema,
                                                          FALSE,
                                                          0);
                            if (i4_ret != FMR_OK)
                            {
                                goto FAIL;
                            }
                        }
                    }

                    pt_dent->ui4_last_pclus = ui4_clus;

                    ui4_clus++;

                    continue;
#endif
                }

                /* Concatenate new entry */

                if (ui4_ent_nib_len == 8)
                {
                    UINT32 ui4_fat_val;

                    /* The last FAT entry, write EOC */
                    ui4_fat_val = FAT_UINT32(ui4_clus);

                for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                {
                    i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                      &ui4_fat_val,
                                                      (ui4_ent_nib_len / 2),
                                                      pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_this_pclus * (ui4_ent_nib_len / 2)),
                                                      &ui4_done,
                                                      pt_mount->h_sync_sema,
                                                      FALSE,
                                                      0);
                    if (i4_ret != FMR_OK)
                    {
                        goto FAIL;
                    }
                }
                }
                else if (ui4_ent_nib_len == 4)
                {
                    UINT16 ui2_fat_val;

                    /* The last FAT entry, write EOC */
                    ui2_fat_val = FAT_UINT16(ui4_clus);

                    for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                    {
                        i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                      &ui2_fat_val,
                                                      (ui4_ent_nib_len / 2),
                                                      pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_this_pclus * (ui4_ent_nib_len / 2)),
                                                      &ui4_done,
                                                      pt_mount->h_sync_sema,
                                                      FALSE,
                                                      0);
                        if (i4_ret != FMR_OK)
                        {
                            goto FAIL;
                        }
                    }
                }
                else
                {
                    UINT16 ui2_fat_val;

                    /* The last FAT entry, write EOC */
                    ui2_fat_val = FAT_UINT16(ui4_clus);

                    if ((ui4_clus % 2) == 0)
                    {
                        ui2_fat_val = ui2_fat_val & 0x0FFF;
                        ui2_val = ui2_val & 0xF000;
                        ui2_val = ui2_val | ui2_fat_val;
                    }
                    else
                    {
                        ui2_fat_val = ui2_fat_val << 4;
                        ui2_val = ui2_val & 0x000F;
                        ui2_val = ui2_val | ui2_fat_val;
                    }

                    for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                    {
                        i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                      &ui2_val,
                                                      sizeof(UINT16),
                                                      pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + ((ui4_this_pclus * ui4_ent_nib_len) / 2),
                                                      &ui4_done,
                                                      pt_mount->h_sync_sema,
                                                      FALSE,
                                                      0);
                        if (i4_ret != FMR_OK)
                        {
                            goto FAIL;
                        }
                    }

                }

/* build cluster cache */
                if( pt_dent->t_clus_cache.ui4_last_valid_entry < (FAT_CLUSTER_CACHE_ENTRY_NUM - 1))
                {/* There is room for cache*/
                    /* logic: ui4_from_lclus + ui4_cnt */
                    /* physical: ui4_clus */
                    pt_dent->t_clus_cache.ui4_tail_lclus = ui4_from_lclus + ui4_cnt ;
                    if ( ui4_clus != pt_dent->t_clus_cache.ui4_tail_pclus + 1)
    {
                        pt_dent->t_clus_cache.ui4_last_valid_entry++;
                        pt_dent->t_clus_cache.at_cache[pt_dent->t_clus_cache.ui4_last_valid_entry].ui4_lclus
                            = pt_dent->t_clus_cache.ui4_tail_lclus;
                        pt_dent->t_clus_cache.at_cache[pt_dent->t_clus_cache.ui4_last_valid_entry].ui4_pclus
                            = ui4_clus;
                    }
                    pt_dent->t_clus_cache.ui4_tail_pclus = ui4_clus;                    
                    
                }
            }

            ui4_this_pclus = ui4_clus;

            if (b_zero_out)
            {
                _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                     pui1_zero,
                                     pt_fat_info->ui4_clus_size,
                                     FAT_CLUS_TO_DATA_ADDR(ui4_clus, pt_fat_info),
                                     &ui4_done,
                                     pt_mount->h_sync_sema,
                                     FALSE,
                                     0);
            }

            ASSERT(pt_mount->t_fs_info.ui8_avail_blk != 0)
            ASSERT(pt_mount->t_fs_info.ui8_free_blk != 0)

            /* update fat info */
            pt_mount->t_fs_info.ui8_avail_blk--;
            pt_mount->t_fs_info.ui8_free_blk--;
        }

        ui4_clus++;
#else
        if (ui4_clus > (pt_fat_info->ui4_valid_clus_cnt + 1)) /* larger than max cluster number */
        {
            if (ui4_start_clus == 2)
            {
                i4_ret = FMR_FILE_SYSTEM_FULL; /* no more free FAT entry */
                goto FAIL;
            }
            else
            {
                /* search from the beginning */
                ui4_start_clus = 2;
                ui4_clus = 2;
                pui1_fat_ptr = pui1_fat_entries;

                continue;
            }
        }

        /*
         *  Read multiple FAT entries to reduce read times.
         */
        if (pui1_fat_ptr == pui1_fat_entries)
        {
            if (ui4_ent_nib_len == 3)
            {
                ASSERT((ui4_clus % 2) == 0);
            }

            if ((b_fat_entries_dirty == TRUE) && (ui4_ent_nib_len != 3))
            {
                for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                {
                    i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                 pui1_fat_entries,
                                                 ui4_fat_entries_len,
                                                 pt_fat_info->ui8_fat_off + ((ui4_fat_entries_start_pclus * ui4_ent_nib_len) / 2),
                                                 &ui4_done,
                                                 pt_mount->h_sync_sema,
                                                 FALSE,
                                                 0);

                    if (i4_ret != FMR_OK)
        {
                        goto FAIL;
                    }
                }

                b_fat_entries_dirty = FALSE;
            }

            i4_ret = _fat_dev_cache_read(pt_mount->pt_dev_xent,
                                         pui1_fat_entries,
                                         ui4_fat_entries_len,
                                         pt_fat_info->ui8_fat_off + ((ui4_clus * ui4_ent_nib_len) / 2),
                                         &ui4_done,
                                         pt_mount->h_sync_sema,
                                         FALSE,
                                         0);

            if (i4_ret != FMR_OK)
            {
                goto FAIL;
            }

            if (ui4_ent_nib_len != 3)
            {
                ui4_fat_entries_start_pclus = ui4_clus;
                ui4_fat_entries_end_pclus = ui4_fat_entries_start_pclus + (ui4_fat_entries_len / (ui4_ent_nib_len / 2)) - 1;
            }
        }

        if (ui4_ent_nib_len == 8)
        {
            ui4_val = FAT_GET_VAL32(pui1_fat_ptr, 0);
            pui1_fat_ptr = ((pui1_fat_ptr + (ui4_ent_nib_len / 2) - pui1_fat_entries) % ui4_fat_entries_len) + pui1_fat_entries;
        }
        else if (ui4_ent_nib_len == 4)
        {
            ui4_val = (UINT32)FAT_GET_VAL16(pui1_fat_ptr, 0);
            pui1_fat_ptr = ((pui1_fat_ptr + (ui4_ent_nib_len / 2) - pui1_fat_entries) % ui4_fat_entries_len) + pui1_fat_entries;
        }
        else
        {
            if ((ui4_clus % 2) == 0)
            {
                ui4_val = (UINT32)(FAT_GET_VAL16(pui1_fat_ptr, 0) & 0x0FFF);
                ui2_val = FAT_GET_VAL16(pui1_fat_ptr, 0);
                pui1_fat_ptr = ((pui1_fat_ptr + 1 - pui1_fat_entries) % ui4_fat_entries_len) + pui1_fat_entries;
        }
        else
        {
                ui4_val = (UINT32)(FAT_GET_VAL16(pui1_fat_ptr, 0) >> 4);
                ui2_val = FAT_GET_VAL16(pui1_fat_ptr, 0);
                pui1_fat_ptr = ((pui1_fat_ptr + 2 - pui1_fat_entries) % ui4_fat_entries_len) + pui1_fat_entries;
            }
        }

        /*
         *  Check if it is a free entry
         */
        if (ui4_val == 0)
        {
            
#if 1

            /* for a case: the found next entry (ui4_clus) is the same as the 
             * previous entry.  It happens when there are only one free entry left
             * and client needs more than one free entry.  The check routine checks
             * from beginning again and found the same entry.  No entry available.
             */

            if (ui4_this_pclus != ui4_clus)
            {
                ui4_cnt++;
            }
#else
            ui4_cnt++;
#endif
                
            if (ui4_cnt == ui4_entry)
            {
                if (ui4_ent_nib_len == 8)
                {
                    UINT32 ui4_fat_val;

                /* The last FAT entry, write EOC */
                    ui4_fat_val = FAT_UINT32(FAT32_EOC_MARK);

                    if ((ui4_clus <= ui4_fat_entries_end_pclus) && (ui4_clus >= ui4_fat_entries_start_pclus))
                    {
                        FAT_SET_VAL32(pui1_fat_entries, (ui4_clus - ui4_fat_entries_start_pclus) * (ui4_ent_nib_len / 2), ui4_fat_val);
                        b_fat_entries_dirty = TRUE;
                    }
                    else
                    {
                for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                {
                    i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                          &ui4_fat_val,
                                                          (ui4_ent_nib_len / 2),
                                                          pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_clus * (ui4_ent_nib_len / 2)),
                                                  &ui4_done,
                                                          pt_mount->h_sync_sema,
                                                          FALSE,
                                                          0);

                    if (i4_ret != FMR_OK)
                    {
                        goto FAIL;
                    }
                }
            }
                }
                else if (ui4_ent_nib_len == 4)
                {
                    UINT16 ui2_fat_val;

                    /* The last FAT entry, write EOC */
                    ui2_fat_val = FAT_UINT16(FAT16_EOC_MARK);

                    if ((ui4_clus <= ui4_fat_entries_end_pclus) && (ui4_clus >= ui4_fat_entries_start_pclus))
                    {
                        FAT_SET_VAL16(pui1_fat_entries, (ui4_clus - ui4_fat_entries_start_pclus) * (ui4_ent_nib_len / 2), ui2_fat_val);
                        b_fat_entries_dirty = TRUE;
                    }
                    else
                {
                        for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                        {
                            i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                          &ui2_fat_val,
                                                          (ui4_ent_nib_len / 2),
                                                          pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_clus * (ui4_ent_nib_len / 2)),
                                                          &ui4_done,
                                                          pt_mount->h_sync_sema,
                                                          FALSE,
                                                          0);
                            if (i4_ret != FMR_OK)
                            {
                                goto FAIL;
                            }
                        }
                    }
                }
                else
                {
                    UINT16 ui2_fat_val;

                    /* The last FAT entry, write EOC */
                    ui2_fat_val = FAT_UINT16(FAT12_EOC_MARK);
                    
                    if ((ui4_clus % 2) == 0)
                    {
                        ui2_fat_val = ui2_fat_val & 0x0FFF;
                        ui2_val = ui2_val & 0xF000;
                        ui2_val = ui2_val | ui2_fat_val;
                }
                    else
                    {
                        ui2_fat_val = ui2_fat_val << 4;
                        ui2_val = ui2_val & 0x000F;
                        ui2_val = ui2_val | ui2_fat_val;
                    }

                    for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                    {
                        i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                      &ui2_val,
                                                      sizeof(UINT16),
                                                      pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + ((ui4_clus * ui4_ent_nib_len) / 2),
                                                      &ui4_done,
                                                      pt_mount->h_sync_sema,
                                                      FALSE,
                                                      0);
                        if (i4_ret != FMR_OK)
                        {
                            goto FAIL;
                        }
                    }
                }

                pt_dent->ui4_last_pclus = ui4_clus;
            }
            
            if (ui4_this_pclus == 0)
            {
                /* The whole new dentry */
                pt_dent->ui4_first_clus = ui4_clus;

                pt_dent->t_clus_cache.ui4_head_lclus
                    = pt_dent->t_clus_cache.ui4_tail_lclus
                    = pt_dent->t_clus_cache.ui4_last_valid_entry
                    = pt_dent->t_clus_cache.at_cache[0].ui4_lclus = 0;
                pt_dent->t_clus_cache.ui4_head_pclus
                    = pt_dent->t_clus_cache.ui4_tail_pclus
                    = pt_dent->t_clus_cache.at_cache[0].ui4_pclus 
                    = pt_dent->ui4_first_clus;
                    
                ASSERT(ui4_from_lclus == 0);
                /* ASSERT(ui4_cnt == 1); */
            }
            else
            {

                /* for a case: the found next entry (ui4_clus) is the same as the 
                 * previous entry.  It happens when there are only one free entry left
                 * and client needs more than one free entry.  The check routine checks
                 * from beginning again and found the same entry.  No entry available.
                 */

                if (ui4_this_pclus == ui4_clus)
                {
#if 0
                    ui4_cnt--;
                    ui4_clus++;

                    pt_mount->t_fs_info.ui8_avail_blk++;
                    pt_mount->t_fs_info.ui8_free_blk++;

                    continue;
#else
                    /* although there are no enough free cluster, we still need to enclose the
                       cluster chain. */

                    if (ui4_ent_nib_len == 8)
                    {
                        UINT32 ui4_fat_val;

                        /* The last FAT entry, write EOC */
                        ui4_fat_val = FAT_UINT32(FAT32_EOC_MARK);

                        if ((ui4_clus <= ui4_fat_entries_end_pclus) && (ui4_clus >= ui4_fat_entries_start_pclus))
                        {
                            FAT_SET_VAL32(pui1_fat_entries, (ui4_clus - ui4_fat_entries_start_pclus) * (ui4_ent_nib_len / 2), ui4_fat_val);
                            b_fat_entries_dirty = TRUE;
                        }
                        else
                        {
                            for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                            {
                                i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                              &ui4_fat_val,
                                                              (ui4_ent_nib_len / 2),
                                                              pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_clus * (ui4_ent_nib_len / 2)),
                                                              &ui4_done,
                                                              pt_mount->h_sync_sema,
                                                              FALSE,
                                                              0);
                                if (i4_ret != FMR_OK)
            {
                                    goto FAIL;
                                }
                            }
                        }
                    }
                    else if (ui4_ent_nib_len == 4)
                    {
                        UINT16 ui2_fat_val;

                        /* The last FAT entry, write EOC */
                        ui2_fat_val = FAT_UINT16(FAT16_EOC_MARK);

                        if ((ui4_clus <= ui4_fat_entries_end_pclus) && (ui4_clus >= ui4_fat_entries_start_pclus))
                        {
                            FAT_SET_VAL16(pui1_fat_entries, (ui4_clus - ui4_fat_entries_start_pclus) * (ui4_ent_nib_len / 2), ui2_fat_val);
                            b_fat_entries_dirty = TRUE;
            }
                        else
                        {
                            for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                            {
                                i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                              &ui2_fat_val,
                                                              (ui4_ent_nib_len / 2),
                                                              pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_clus * (ui4_ent_nib_len / 2)),
                                                              &ui4_done,
                                                              pt_mount->h_sync_sema,
                                                              FALSE,
                                                              0);
                                if (i4_ret != FMR_OK)
                                {
                                    goto FAIL;
                                }
                            }
                        }
                    }
                    else
                    {
                        UINT16 ui2_fat_val;

                        /* The last FAT entry, write EOC */
                        ui2_fat_val = FAT_UINT16(FAT12_EOC_MARK);

                        if ((ui4_clus % 2) == 0)
                        {
                            ui2_fat_val = ui2_fat_val & 0x0FFF;
                            ui2_val = ui2_val & 0xF000;
                            ui2_val = ui2_val | ui2_fat_val;
                        }
                        else
                        {
                            ui2_fat_val = ui2_fat_val << 4;
                            ui2_val = ui2_val & 0x000F;
                            ui2_val = ui2_val | ui2_fat_val;
                        }

                        for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                        {
                            i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                          &ui2_val,
                                                          sizeof(UINT16),
                                                          pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + ((ui4_clus * ui4_ent_nib_len) / 2),
                                                          &ui4_done,
                                                          pt_mount->h_sync_sema,
                                                          FALSE,
                                                          0);

                            if (i4_ret != FMR_OK)
                            {
                                goto FAIL;
                            }
                        }
                    }

                    pt_dent->ui4_last_pclus = ui4_clus;

                    ui4_clus++;

                    continue;
#endif            
                }

                /* Concatenate new entry */

                if (ui4_ent_nib_len == 8)
                {
                    UINT32 ui4_fat_val;

                    /* The last FAT entry, write EOC */
                    ui4_fat_val = FAT_UINT32(ui4_clus);

                    if ((ui4_this_pclus <= ui4_fat_entries_end_pclus) && (ui4_this_pclus >= ui4_fat_entries_start_pclus))
                    {
                        FAT_SET_VAL32(pui1_fat_entries, (ui4_this_pclus - ui4_fat_entries_start_pclus) * (ui4_ent_nib_len / 2), ui4_fat_val);
                        b_fat_entries_dirty = TRUE;
                    }
                    else
                    {
                for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                {
                    i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                          &ui4_fat_val,
                                                          (ui4_ent_nib_len / 2),
                                                          pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_this_pclus * (ui4_ent_nib_len / 2)),
                                                          &ui4_done,
                                                          pt_mount->h_sync_sema,
                                                          FALSE,
                                                          0);
                            if (i4_ret != FMR_OK)
                            {
                                goto FAIL;
                            }
                        }
                    }
                }
                else if (ui4_ent_nib_len == 4)
                {
                    UINT16 ui2_fat_val;

                    /* The last FAT entry, write EOC */
                    ui2_fat_val = FAT_UINT16(ui4_clus);

                    if ((ui4_this_pclus <= ui4_fat_entries_end_pclus) && (ui4_this_pclus >= ui4_fat_entries_start_pclus))
                    {
                        FAT_SET_VAL16(pui1_fat_entries, (ui4_this_pclus - ui4_fat_entries_start_pclus) * (ui4_ent_nib_len / 2), ui2_fat_val);
                        b_fat_entries_dirty = TRUE;
                    }
                    else
                    {
                        for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                        {
                            i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                          &ui2_fat_val,
                                                          (ui4_ent_nib_len / 2),
                                                          pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_this_pclus * (ui4_ent_nib_len / 2)),
                                                          &ui4_done,
                                                          pt_mount->h_sync_sema,
                                                          FALSE,
                                                          0);
                            if (i4_ret != FMR_OK)
                            {
                                goto FAIL;
                            }
                        }
                    }
                }
                else
                {
                    UINT16 ui2_fat_val;

                    /* The last FAT entry, write EOC */
                    ui2_fat_val = FAT_UINT16(ui4_clus);

                    if ((ui4_clus % 2) == 0)
                    {
                        ui2_fat_val = ui2_fat_val & 0x0FFF;
                        ui2_val = ui2_val & 0xF000;
                        ui2_val = ui2_val | ui2_fat_val;
                    }
                    else
                    {
                        ui2_fat_val = ui2_fat_val << 4;
                        ui2_val = ui2_val & 0x000F;
                        ui2_val = ui2_val | ui2_fat_val;
                    }

                    for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                    {
                        i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                      &ui2_val,
                                                      sizeof(UINT16),
                                                      pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + ((ui4_this_pclus * ui4_ent_nib_len) / 2),
                                                  &ui4_done,
                                                      pt_mount->h_sync_sema,
                                                      FALSE,
                                                      0);
                    if (i4_ret != FMR_OK)
                    {
                        goto FAIL;
                    }
                }

                }

/* build cluster cache */
                if( pt_dent->t_clus_cache.ui4_last_valid_entry < (FAT_CLUSTER_CACHE_ENTRY_NUM - 1))
                {/* There is room for cache*/
                    /* logic: ui4_from_lclus + ui4_cnt */
                    /* physical: ui4_clus */
                    pt_dent->t_clus_cache.ui4_tail_lclus = ui4_from_lclus + ui4_cnt ;
                    if ( ui4_clus != pt_dent->t_clus_cache.ui4_tail_pclus + 1)
                    {
                        pt_dent->t_clus_cache.ui4_last_valid_entry++;
                        pt_dent->t_clus_cache.at_cache[pt_dent->t_clus_cache.ui4_last_valid_entry].ui4_lclus
                            = pt_dent->t_clus_cache.ui4_tail_lclus;
                        pt_dent->t_clus_cache.at_cache[pt_dent->t_clus_cache.ui4_last_valid_entry].ui4_pclus
                            = ui4_clus;
                    }
                    pt_dent->t_clus_cache.ui4_tail_pclus = ui4_clus;                    
                    
                }
            }

            ui4_this_pclus = ui4_clus;

            if (b_zero_out)
            {
                _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                     pui1_zero,
                                     pt_fat_info->ui4_clus_size,
                                     FAT_CLUS_TO_DATA_ADDR(ui4_clus, pt_fat_info),
                                     &ui4_done,
                                     pt_mount->h_sync_sema,
                                     FALSE,
                                     0);
            }

            if ((pt_mount->t_fs_info.ui8_avail_blk >= 1)&&(pt_mount->t_fs_info.ui8_free_blk >= 1))
            {

            /* update fat info */
            pt_mount->t_fs_info.ui8_avail_blk --;
            pt_mount->t_fs_info.ui8_free_blk --;
        }
            else
            {
                i4_ret = FMR_FILE_SYSTEM_FULL;
                goto FAIL;
            }
        }
        
        ui4_clus++;
#endif
    }


#if 0 // new cache write method
#else
    if ((b_fat_entries_dirty == TRUE) && (ui4_ent_nib_len != 3))
    {
        for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
        {
            i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                         pui1_fat_entries,
                                         ui4_fat_entries_len,
                                         pt_fat_info->ui8_fat_off + ((ui4_fat_entries_start_pclus * ui4_ent_nib_len) / 2),
                                         &ui4_done,
                                         pt_mount->h_sync_sema,
                                         FALSE,
                                         0);

            if (i4_ret != FMR_OK)
            {
                goto FAIL;
            }
    }
    }
    
#endif

    i4_ret = FMR_OK;

FAIL:

    if (pui1_zero != NULL)
    {
        x_mem_free(pui1_zero);
    }

    if (pui1_fat_entries != NULL)
    {
        x_mem_free(pui1_fat_entries);
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_inval_fat_chain
 *
 * Description: Invalidate a FAT cluster chain of a file/directory from the
 *              specified cluster.
 *
 * Inputs:  pt_xent
 *              Reference to the xentry of a file/directory.
 *          ui4_start_lclus
 *              The starting cluster of the chain to discard. The first cluster
 *              of a file/directory is 0.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 ----------------------------------------------------------------------------*/
static INT32 _fat_inval_fat_chain(
    FM_XENTRY_T     *pt_xent,
    UINT32          ui4_start_lclus)
{
    FAT_FSINFO_T *pt_fat_info;
    FM_MOUNT_T   *pt_mount;
    FAT_DENTRY_T *pt_dent;
    UINT8  *pui1_fat_entries = NULL;
    UINT32 ui4_this_lclus = 0;
    UINT32 ui4_last_lclus;
    UINT32 ui4_this_pclus = 0;
    UINT32 ui4_next_pclus;
    UINT32 ui4_bgn_pclus;
    UINT32 ui4_end_pclus;
    UINT32 ui4_ent_nib_len;
    UINT32 ui4_fat_entries_len = 0;
    UINT32 ui4_done;
    UINT8  i;
    INT32  i4_ret = FMR_OK;
    
    ASSERT(pt_xent != NULL);
    ASSERT(pt_xent->pt_mount != NULL);
    ASSERT(pt_xent->pt_mount->pv_priv != NULL);
    ASSERT(pt_xent->pv_fs_node != NULL);
    
    pt_mount    = (FM_MOUNT_T *)pt_xent->pt_mount;
    pt_fat_info = (FAT_FSINFO_T *)pt_mount->pv_priv;
    pt_dent     = FAT_GET_DENTRY(pt_xent);

    switch (pt_fat_info->e_fat_type)
    {
        case FAT_TYPE_32:
            ui4_ent_nib_len = 8;
            ui4_fat_entries_len = 512;
            break;
        case FAT_TYPE_16:
            ui4_ent_nib_len = 4;
            ui4_fat_entries_len = 512;
            break;
        case FAT_TYPE_12:
            ui4_ent_nib_len = 3;
            ui4_fat_entries_len = 510;  //  dividable by 3
            break;
        default:
        return FMR_INVAL;
    }

    pui1_fat_entries = x_mem_alloc(ui4_fat_entries_len);

    if (pui1_fat_entries == NULL)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }

    ui4_last_lclus = (ui4_start_lclus > 0) ? ui4_start_lclus - 1 : 0;
    
    ui4_bgn_pclus = 0;
    ui4_end_pclus = 0;

    for (ui4_this_lclus = ui4_last_lclus; ; ui4_this_lclus++)
    {
        i4_ret = _fat_get_pclus( &ui4_this_pclus, ui4_this_lclus, pt_xent->pt_mount, pt_dent);

        /* perhaps the first cluster in directory entry is wrong  */

            if (i4_ret != FMR_OK)
            {
            goto FAIL;
        }

        if (ui4_this_pclus == 0)
        {
            break;
        }

        if ((ui4_this_pclus < ui4_bgn_pclus) || (ui4_this_pclus >= ui4_end_pclus))
        {
            if (ui4_end_pclus != 0)
            {
                /* write modified fat entries */
                for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                {
                    UINT32 ui4_write_len;

                    if (((ui4_bgn_pclus * ui4_ent_nib_len) / 2) + ui4_fat_entries_len > pt_fat_info->ui4_fat_size)
                    {
                        ui4_write_len = pt_fat_info->ui4_fat_size - ((ui4_bgn_pclus * ui4_ent_nib_len) / 2);
                    }
                    else
                    {
                        ui4_write_len = ui4_fat_entries_len;
                    }

                    i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                  pui1_fat_entries,
                                                  ui4_write_len,
                                                  pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + ((ui4_bgn_pclus * ui4_ent_nib_len) / 2),
                                                  &ui4_done,
                                                  pt_fat_info->h_lock,
                                                  FALSE,
                                                  0);
                    if (i4_ret != FMR_OK)
                    {
                        goto FAIL;
                    }
                }
            }
            
            /* read to-be-modified fat entries */
            ui4_bgn_pclus = ui4_this_pclus;

            i4_ret = _fat_dev_cache_read(pt_mount->pt_dev_xent,
                                         pui1_fat_entries,
                                         ui4_fat_entries_len,
                                         pt_fat_info->ui8_fat_off + ((ui4_bgn_pclus * ui4_ent_nib_len) / 2),
                                         &ui4_done,
                                         pt_fat_info->h_lock,
                                         FALSE,
                                         0);
            if (i4_ret != FMR_OK)
            {
                goto FAIL;
            }

            ui4_end_pclus = ui4_bgn_pclus + ((ui4_fat_entries_len * 2)/ ui4_ent_nib_len);

            if ((ui4_ent_nib_len == 3) && ((ui4_bgn_pclus % 2) != 0))
            {
                ui4_end_pclus -= 1;
            }
        }

        if (ui4_ent_nib_len == 8)
        {
            ui4_next_pclus = FAT_GET_VAL32(pui1_fat_entries, ((ui4_this_pclus - ui4_bgn_pclus) << 2));

            if (ui4_this_lclus < ui4_start_lclus)
            {
                pt_dent->ui4_last_pclus = ui4_this_pclus;
                FAT_SET_VAL32(pui1_fat_entries, ((ui4_this_pclus - ui4_bgn_pclus) << 2), FAT32_EOC_MARK);
            }
            else
            {
                FAT_SET_VAL32(pui1_fat_entries, ((ui4_this_pclus - ui4_bgn_pclus) << 2), 0);
            }

            pt_mount->t_fs_info.ui8_avail_blk++;
            pt_mount->t_fs_info.ui8_free_blk++;

            if (ui4_next_pclus >= 0x0FFFFFF8) /* FAT EOC MARK*/
            {
                break;
            }
        }
        else if (ui4_ent_nib_len == 4)
        {
            ui4_next_pclus = (UINT32)FAT_GET_VAL16(pui1_fat_entries, ((ui4_this_pclus - ui4_bgn_pclus) << 1));

            if (ui4_this_lclus < ui4_start_lclus)
            {
                pt_dent->ui4_last_pclus = ui4_this_pclus;
                FAT_SET_VAL16(pui1_fat_entries, ((ui4_this_pclus - ui4_bgn_pclus) << 1), FAT16_EOC_MARK);
            }
            else
            {
                FAT_SET_VAL16(pui1_fat_entries, ((ui4_this_pclus - ui4_bgn_pclus) << 1), 0);
            }

            pt_mount->t_fs_info.ui8_avail_blk++;
            pt_mount->t_fs_info.ui8_free_blk++;

            if (ui4_next_pclus >= 0x0000FFF8)  
            {
                break;
            }
        }
        else
        {
            UINT16 ui2_fat_val;

            if ((ui4_bgn_pclus % 2) == 0)
            {
                ui2_fat_val = FAT_GET_VAL16(pui1_fat_entries, (((ui4_this_pclus - ui4_bgn_pclus) * ui4_ent_nib_len) / 2));
            }
            else
            {
                ui2_fat_val = FAT_GET_VAL16(pui1_fat_entries, (((ui4_this_pclus - ui4_bgn_pclus) * ui4_ent_nib_len + 1) / 2));
            }
        
            if ((ui4_this_pclus % 2) == 0)
            {
                ui4_next_pclus = (UINT32)(ui2_fat_val & 0x0FFF);
            }
            else
            {
                ui4_next_pclus = (UINT32)(ui2_fat_val >> 4);
            }

            if (ui4_this_lclus < ui4_start_lclus)
            {
                UINT16 ui2_mod_val;

                pt_dent->ui4_last_pclus = ui4_this_pclus;

                if ((ui4_this_pclus % 2) == 0)
                {
                    ui2_mod_val = (ui2_fat_val & 0xF000) | (FAT12_EOC_MARK & 0x0FFF);
                }
                else
                {
                    ui2_mod_val = (ui2_fat_val & 0x000F) | (FAT12_EOC_MARK << 4);
                }

                if ((ui4_bgn_pclus % 2) == 0)
                {
                    FAT_SET_VAL16(pui1_fat_entries, (((ui4_this_pclus - ui4_bgn_pclus) * ui4_ent_nib_len) / 2), ui2_mod_val);
                }
                else
                {
                    FAT_SET_VAL16(pui1_fat_entries, (((ui4_this_pclus - ui4_bgn_pclus) * ui4_ent_nib_len + 1) / 2), ui2_mod_val);
                }
            }
            else
            {
                UINT16 ui2_mod_val;

                if ((ui4_this_pclus % 2) == 0)
                {
                    ui2_mod_val = (ui2_fat_val & 0xF000) | (0 & 0x0FFF);
                }
                else
                {
                    ui2_mod_val = (ui2_fat_val & 0x000F) | (0 << 4);
                }

                if ((ui4_bgn_pclus % 2) == 0)
                {
                    FAT_SET_VAL16(pui1_fat_entries, (((ui4_this_pclus - ui4_bgn_pclus) * ui4_ent_nib_len) / 2), ui2_mod_val);
                }
                else
                {
                    FAT_SET_VAL16(pui1_fat_entries, (((ui4_this_pclus - ui4_bgn_pclus) * ui4_ent_nib_len + 1) / 2), ui2_mod_val);
                }
            }

            pt_mount->t_fs_info.ui8_avail_blk++;
            pt_mount->t_fs_info.ui8_free_blk++;
        
            if (ui4_next_pclus >= 0x00000FF8)  
            {
                break;
            }
        }
    }

    if (ui4_end_pclus != 0)
    {
        /* write modified fat entries */
        for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
        {
            UINT32 ui4_write_len;

            if (((ui4_bgn_pclus * ui4_ent_nib_len) / 2) + ui4_fat_entries_len > pt_fat_info->ui4_fat_size)
            {
                ui4_write_len = pt_fat_info->ui4_fat_size - ((ui4_bgn_pclus * ui4_ent_nib_len) / 2);
            }
            else
            {
                ui4_write_len = ui4_fat_entries_len;
            }

            i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                          pui1_fat_entries,
                                          ui4_write_len,
                                          pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + ((ui4_bgn_pclus * ui4_ent_nib_len) / 2),
                                          &ui4_done,
                                          pt_fat_info->h_lock,
                                          FALSE,
                                          0);
            if (i4_ret != FMR_OK)
            {
                goto FAIL;
            }
        }
    }

    if ( ui4_start_lclus == 0)
    {
        pt_dent->ui4_last_pclus = pt_dent->ui4_first_clus;
    }

    /* reset cluster cache */
    pt_dent->t_clus_cache.ui4_head_lclus = 
    pt_dent->t_clus_cache.ui4_tail_lclus =
    pt_dent->t_clus_cache.ui4_last_valid_entry = 0;

    pt_dent->t_clus_cache.ui4_head_pclus =
    pt_dent->t_clus_cache.ui4_tail_pclus =
    pt_dent->ui4_first_clus;
    x_memset( pt_dent->t_clus_cache.at_cache, 0, sizeof(pt_dent->t_clus_cache.at_cache));

    pt_dent->t_clus_cache.at_cache[0].ui4_lclus = 0;
    pt_dent->t_clus_cache.at_cache[0].ui4_pclus = pt_dent->ui4_first_clus;

FAIL:

    if (pui1_fat_entries != NULL)
    {
        x_mem_free(pui1_fat_entries);
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_add_dir_entry
 *
 * Description: Add a new directory entry.
 *
 * Inputs:  pt_prnt_xent
 *              Reference to the parent directory xentry.
 *          pt_dent
 *              Reference to the new FAT node in memory structure.
 *          s_utf16
 *              UTF-16 file name.
 *          ui2_len
 *              String length of s_utf16
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 _fat_add_dir_entry(
    FM_XENTRY_T     *pt_prnt_xent,
    FAT_DENTRY_T    *pt_dent,
    const UINT8     *s_utf16,
    UINT16          ui2_len)
{
    FM_RW_ARG_T   t_io;
    FM_MOUNT_T   *pt_mount;
    FAT_DENTRY_T *pt_prnt_dent;
    FAT_FSINFO_T *pt_fat_info;
    UINT64  ui8_offset = 0;
    UINT8   ui1_val;
    UINT8   ui1_chksum;
    UINT8   ui1_free = 0;
    UINT8   pui1_dent[32];
    INT32   i4_ret;

    ASSERT(pt_prnt_xent != NULL);
    ASSERT(pt_prnt_xent->pt_mount != NULL);
    ASSERT(pt_prnt_xent->pt_mount->pv_priv != NULL);
    ASSERT(pt_prnt_xent->pt_mount->pt_dev_xent != NULL);
    ASSERT(pt_prnt_xent->pv_fs_node != NULL);
    ASSERT(IS_XENTRY_DIR(pt_prnt_xent));
    ASSERT(pt_dent != NULL);
    ASSERT(ui2_len > 0);

    pt_mount     = pt_prnt_xent->pt_mount;
    pt_fat_info  = (FAT_FSINFO_T *)pt_mount->pv_priv;
    pt_prnt_dent = FAT_GET_DENTRY(pt_prnt_xent);

    /* we always need the long name part even the name is shorter than 13 bytes */
    /* it is possible that we have a string have CJK characters but shorter than 13 bytes */

    if ((pt_fat_info->e_fat_type == FAT_TYPE_32) || (pt_fat_info->e_fat_type == FAT_TYPE_16))
    {
        pt_dent->ui1_use_dent = (UINT8)((ui2_len / 13) + 1 + (((ui2_len % 13) == 0) ? 0 : 1));
    }
    else
    {
        pt_dent->ui1_use_dent = 1;
    }

    t_io.pt_xent    = pt_prnt_xent;
    t_io.pv_data    = &ui1_val;
    t_io.ui4_count  = 1;
    t_io.ui2_flag   = FM_IO_SYNC;
    t_io.h_bin_sema = pt_mount->h_sync_sema;

    while (ui1_free < pt_dent->ui1_use_dent)
    {
        if (ui8_offset >= pt_prnt_dent->ui4_size)
        {
            if ((pt_fat_info->e_fat_type == FAT_TYPE_32) ||
                ((pt_prnt_dent->ui1_attr & FAT_ATTR_VOLUME_ID) == 0))
            {
                UINT32 ui4_old_clus;

                ui4_old_clus = (UINT32)(FAT_SIZE_TO_CLUS(pt_prnt_dent->ui4_size, pt_prnt_xent));
            
                i4_ret = _fat_extend_fat_chain(pt_mount,
                                               pt_prnt_dent,
                                               1,
                                               ui4_old_clus,
                                               TRUE);
                if (i4_ret != FMR_OK)
                {
                    return i4_ret;
                }
                pt_prnt_dent->ui4_size += pt_fat_info->ui4_clus_size;
            }
        }

        t_io.ui8_offset     = ui8_offset;
        t_io.ui4_done_bytes = 0;
        
        i4_ret = _fat_rw_buf(&t_io, TRUE, FALSE);
        if (i4_ret != FMR_OK)
        {
            return i4_ret;
        }

        ui8_offset += 32;

        if ((ui1_val == 0) || (ui1_val == 0xE5))
        {
            ui1_free++;
        }
        else
        {
            ui1_free = 0;
        }

    }

    ui8_offset -= ui1_free * 32;

    /*
     *  Write directory entries to the directory.
     */
    _fat_generate_dos_name(pt_prnt_xent,
                           pt_dent->s_name,
                           pt_dent->s_utf8);
    
    x_memset(pui1_dent, 0xFF, 32);
    FAT_SET_VAL8(pui1_dent, FAT_LDENTRY_ATTR, 0x0F);
    FAT_SET_VAL8(pui1_dent, FAT_LDENTRY_TYPE, 0);
    ui1_chksum = _fat_longname_chksum(pt_dent->s_name);
    FAT_SET_VAL8(pui1_dent, FAT_LDENTRY_CHKSUM, ui1_chksum);
    FAT_SET_VAL16(pui1_dent, FAT_LDENTRY_FST_CLUS_LO, 0);

    for (ui1_val = pt_dent->ui1_use_dent; ui1_val > 0; ui1_val--)
    {
        const UINT8 *pui1_src;
        UINT8 *pui1_dst;
        UINT16 i;

        if (ui1_val == 1)
        {
            /*
             *  Short entry.
             */
            _fat_set_dentry(pui1_dent, pt_dent);
            pt_dent->ui4_name_off = (UINT32)ui8_offset;
        }
        else
        {
            /*
             *  Long entries.
             */
            pui1_src = s_utf16 + (ui1_val - 2) * 26;

            if (ui1_val == pt_dent->ui1_use_dent)
            {
                FAT_SET_VAL8(pui1_dent, FAT_LDENTRY_ORDER, (UINT8)((ui1_val - 1) | 0x40));
                ui2_len = (UINT16)((ui2_len % 13) * 2);
                if (ui2_len == 0)
                {
                    ui2_len = 26;
                }
                else
                {
                    ui2_len += 2;
                }
            }
            else
            {
                FAT_SET_VAL8(pui1_dent, FAT_LDENTRY_ORDER, (UINT8)(ui1_val - 1));
                ui2_len = 26;
            }
        
            /*
             *  Note that ui2_len includes end zero.
             */
            pui1_dst = FAT_OFFSET(pui1_dent, FAT_LDENTRY_NAME1);
            for (i = 0; i < ui2_len; i++)
            {
                if (i == 10)
                {
                    pui1_dst = FAT_OFFSET(pui1_dent, FAT_LDENTRY_NAME2);
                }
                else if (i == 22)
                {
                    pui1_dst = FAT_OFFSET(pui1_dent, FAT_LDENTRY_NAME3);
                }

                *pui1_dst++ = *pui1_src++;
            }
        }

        t_io.pt_xent        = pt_prnt_xent;
        t_io.pv_data        = pui1_dent;
        t_io.ui4_count      = 32;
        t_io.ui8_offset     = ui8_offset;
        t_io.ui4_done_bytes = 0;
        t_io.ui2_flag       = FM_IO_SYNC;
        t_io.h_bin_sema     = pt_prnt_xent->pt_mount->h_sync_sema;

        i4_ret = _fat_rw_buf(&t_io, FALSE, FALSE);
        if (i4_ret != FMR_OK)
        {
            return i4_ret;
        }

        ui8_offset += 32;
    }

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_del_dir_entry
 *
 * Description: Erase a directory entry in the specified directory. It simply
 *              writes 0xE5 to the first byte of each 32-byte entries which
 *              belong to that entry.
 *
 * Inputs:  pt_prnt_xent
 *              Reference to the parent directory xentry.
 *          pt_dent
 *              Reference to the FAT node in memory structure that is going
 *              to be deleted.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 _fat_del_dir_entry(
    FM_XENTRY_T         *pt_prnt_xent,
    FAT_DENTRY_T        *pt_dent)
{
    FM_RW_ARG_T t_io;
    UINT64 ui8_offset;
    UINT8  ui1_e5 = 0xE5;
    INT32  i4_ret;
    INT32  i;

    t_io.pt_xent    = pt_prnt_xent;
    t_io.pv_data    = &ui1_e5;
    t_io.ui4_count  = 1;
    t_io.ui2_flag   = FM_IO_SYNC;
    t_io.h_bin_sema = pt_prnt_xent->pt_mount->h_sync_sema;

    for (ui8_offset = pt_dent->ui4_name_off - (pt_dent->ui1_use_dent - 1) * 32, i = 0;
         i < pt_dent->ui1_use_dent;
         ui8_offset += 32, i++)
    {
        t_io.ui8_offset     = ui8_offset;
        t_io.ui4_done_bytes = 0;

        i4_ret = _fat_rw_buf(&t_io, FALSE, FALSE);
        if (i4_ret != FMR_OK)
        {
            return i4_ret;
        }
    }

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _fat_build_clus_cache2
 *
 * Description: Build the cluster number cache of the specified file entry.
 *              It accelatrates the file access time because it can reduce
 *              storage access times. Note that it caches only a chunk of
 *              cluster numbers at one time, because it takes too much space
 *              to keep all of them in memory. It caches 128 continguous
 *              cluster numbers at present. This function use RLC to store cluster information.
 *
 * Inputs:  pt_mount
 *              Reference to the mount structure.
 *          pt_dent
 *              Reference to the FAT node in memory structure.
 *          ui4_want_lclus
 *              The cluster number we're interested to cache.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 _fat_build_clus_cache2( 
    FM_MOUNT_T *pt_mount,
    FAT_DENTRY_T *pt_dent,
    UINT32  ui4_target_lclus)
{
    INT32   i4_ret = FMR_OK;
    UINT32  ui4_i;
    UINT32  ui4_ent_nib_len;
    UINT32  ui4_done;
    UINT32  ui4_load_size;
    UINT32  ui4_start_search_pclus = 0;
    UINT32  ui4_end_search_pclus = 0;
    UINT32  ui4_this_pclus = 0;
    UINT32  ui4_next_pclus = 0;
    UINT32  ui4_this_lclus = 0;
    UINT8   *pui1_fat_entries = NULL;
    UINT32  ui4_fat_entries_len = 0;
    UINT32  ui4_eoc;

    FAT_FSINFO_T *pt_fat_info; 
    UINT64  ui8_partition_last_pclus;
    
    UINT32 ui4_sec_sz;
    UINT32 ui4_prefetch_limit;
    UINT32 ui4_fat_sec_ns;

/*
  * 0. Reset Cache.
  * 1. Get the last valid pclus, do search.
  * 2. if the target lclus is not within in the search window, go to 1.
  * 3. Cache them up!
  */
 
    pt_fat_info = (FAT_FSINFO_T *)pt_mount->pv_priv;

    switch(pt_fat_info->e_fat_type)
    {
        case FAT_TYPE_12:
            ui4_ent_nib_len = 3;
            ui4_fat_entries_len = 510;
            ui4_eoc = 0x0FF0;
            break;
        case FAT_TYPE_16:
            ui4_ent_nib_len = 4;
            ui4_fat_entries_len = 16*512;
            ui4_eoc = 0xFFF8;
            break;
        case FAT_TYPE_32:
            ui4_ent_nib_len = 8;
            ui4_fat_entries_len = 32*512;
            ui4_eoc = 0x0FFFFFF8;
            break;
        default:
            return FMR_INVAL;
    }
    
    if (IS_XENTRY_BLK(pt_mount->pt_dev_xent))
    {
        ui4_sec_sz = (UINT32)pt_mount->pt_dev_xent->pt_dev->u_param.t_blkdev.ui4_sec_size;
    }
    else
    {
        ui4_sec_sz = (UINT32)512;
    }

    pui1_fat_entries = x_mem_alloc(ui4_fat_entries_len);

    if (pui1_fat_entries == NULL)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }

    if ( ui4_target_lclus <= (pt_dent->t_clus_cache.ui4_head_lclus))
    { /* target logical cluster is out of cache*/
        ui4_this_pclus = ui4_start_search_pclus = pt_dent->ui4_first_clus;
        ui4_this_lclus  = 0;
    }
    else if ( ui4_target_lclus > pt_dent->t_clus_cache.ui4_tail_lclus)
    { /* target logical cluster is out of cache*/
        ui4_this_pclus = ui4_start_search_pclus = pt_dent->t_clus_cache.ui4_tail_pclus;
        ui4_this_lclus = pt_dent->t_clus_cache.ui4_tail_lclus;
    }
    else
    {
        ASSERT(0); /* It's impossible that the a lclus is neither less than head nor greater than tail*/
        i4_ret = FMR_INVAL;
        goto FAIL;
    }
    
#if 0
    /* some file system with error may have clusters more than total cluster number */
    ui8_partition_last_pclus = pt_fat_info->ui4_valid_clus_cnt + (pt_fat_info->ui1_num_fat * pt_fat_info->ui4_fat_size + pt_fat_info->ui8_fat_off) / pt_fat_info->ui4_clus_size;
#else
    /* the max cluster number should be the valid cluster count + 1 */
    ui8_partition_last_pclus = pt_fat_info->ui4_valid_clus_cnt + 1;
#endif

    ui4_fat_sec_ns = (pt_fat_info->ui4_fat_size / ui4_sec_sz) * pt_fat_info->ui1_num_fat;
    ui4_prefetch_limit = (UINT32)(pt_fat_info->ui8_fat_off / (UINT64)ui4_sec_sz) + ui4_fat_sec_ns;

    while( ui4_this_lclus < ui4_target_lclus)
    {
        ui4_start_search_pclus = ui4_this_pclus;

        if( ui4_start_search_pclus >= ui4_eoc)
        {
            i4_ret = FMR_EOC;
            goto FAIL;
        }

        /* load data from storage */

        if ((ui4_ent_nib_len == 8) || (ui4_ent_nib_len == 4))
        {
            if ((UINT64)(ui4_start_search_pclus + ((ui4_fat_entries_len * 2) / ui4_ent_nib_len)) > ui8_partition_last_pclus)
         {
                ui4_load_size = (UINT32)((ui8_partition_last_pclus - ui4_start_search_pclus + 1) * (ui4_ent_nib_len / 2));
         }
         else
            {
                ui4_load_size = ui4_fat_entries_len;
            }

            /* set search boundry */
            ui4_end_search_pclus = ui4_start_search_pclus + (ui4_load_size * 2) / ui4_ent_nib_len - 1;

        i4_ret = _fat_dev_cache_read(pt_mount->pt_dev_xent,
                                         pui1_fat_entries,
                                     ui4_load_size,
                                         pt_fat_info->ui8_fat_off + (ui4_start_search_pclus * (ui4_ent_nib_len / 2)),
                                     &ui4_done,
                                         pt_fat_info->h_lock,
                                         TRUE,
                                         ui4_prefetch_limit);
        if (i4_ret != FMR_OK)
        {
                goto FAIL;
        }
        }
        else
        {
            UINT32 ui4_fat_byte_ns;
            UINT32 ui4_start_byte_idx;

            if (((pt_fat_info->ui4_valid_clus_cnt + 2) % 2) == 0)
            {
                ui4_fat_byte_ns = (((pt_fat_info->ui4_valid_clus_cnt + 2) * ui4_ent_nib_len) / 2);
            }
            else
            {
                ui4_fat_byte_ns = (((pt_fat_info->ui4_valid_clus_cnt + 2) * ui4_ent_nib_len) / 2) + 1;
            }

            ui4_start_byte_idx = ((ui4_start_search_pclus * ui4_ent_nib_len) / 2);

            if ((ui4_start_byte_idx + ui4_fat_entries_len) > ui4_fat_byte_ns)
            {
                ui4_load_size = ui4_fat_byte_ns - ui4_start_byte_idx;
                ui4_end_search_pclus = (pt_fat_info->ui4_valid_clus_cnt + 2) - 1;
            }
            else
            {
                ui4_load_size = ui4_fat_entries_len;

                if ((ui4_start_search_pclus % 2) == 0)
         {
                    ui4_end_search_pclus = ui4_start_search_pclus + ((ui4_load_size * 2) / ui4_ent_nib_len) - 1;
         }
         else
                {
                    ui4_end_search_pclus = ui4_start_search_pclus + ((ui4_load_size * 2) / ui4_ent_nib_len) - 2;
                }
            }

        i4_ret = _fat_dev_cache_read(pt_mount->pt_dev_xent,
                                         pui1_fat_entries,
                                     ui4_load_size,
                                         pt_fat_info->ui8_fat_off + ui4_start_byte_idx,
                                     &ui4_done,
                                         pt_fat_info->h_lock,
                                         FALSE,
                                         0);
        if (i4_ret != FMR_OK)
        {
                goto FAIL;
        }
        }

        ui4_next_pclus = ui4_this_pclus;
        /* start search FAT */

        while( ui4_next_pclus <= ui4_end_search_pclus 
            && ui4_next_pclus >= ui4_start_search_pclus
            && ui4_this_lclus != ui4_target_lclus)
        {
            /* ui4_next_pclus is the next cluster in the file entry's cluser chain */
            if (ui4_ent_nib_len == 8)
            {
                ui4_next_pclus = (UINT32) FAT_GET_VAL32(pui1_fat_entries, (ui4_this_pclus - ui4_start_search_pclus) * (ui4_ent_nib_len / 2));
            }
            else if (ui4_ent_nib_len == 4)
            {
                ui4_next_pclus = (UINT32) FAT_GET_VAL16(pui1_fat_entries, (ui4_this_pclus - ui4_start_search_pclus) * (ui4_ent_nib_len / 2));
            }
            else
            {
                UINT16 ui2_fat_val;
                
                if ((ui4_start_search_pclus % 2) == 0)
                {
                    ui2_fat_val = FAT_GET_VAL16(pui1_fat_entries, (((ui4_this_pclus - ui4_start_search_pclus) * ui4_ent_nib_len) / 2));
                }
                else
                {
                    ui2_fat_val = FAT_GET_VAL16(pui1_fat_entries, (((ui4_this_pclus - ui4_start_search_pclus) * ui4_ent_nib_len + 1) / 2));
                }
                
                if ((ui4_this_pclus % 2) == 0)
            {
                    ui4_next_pclus = (UINT32)(ui2_fat_val & 0x0FFF);
            }
            else
            {
                    ui4_next_pclus = (UINT32)(ui2_fat_val >> 4);
                }
            }
            
            if ( ui4_next_pclus >= ui4_eoc)
            {
                i4_ret = FMR_EOC;
                goto FAIL;
            }
            
            ui4_this_lclus++;
            ui4_this_pclus = ui4_next_pclus;
        }

    }
    ASSERT(ui4_this_lclus == ui4_target_lclus);
    /* Now, we're going to cache cluster number */

    for(ui4_i = 0; ui4_i < FAT_CLUSTER_CACHE_ENTRY_NUM; ui4_i++)
    {/* reset to zero*/
        pt_dent->t_clus_cache.at_cache[ui4_i].ui4_lclus = 0;
        pt_dent->t_clus_cache.at_cache[ui4_i].ui4_pclus = 0;
    }
    /* 1st cache entry */
    pt_dent->t_clus_cache.ui4_tail_pclus 
        = pt_dent->t_clus_cache.ui4_head_pclus 
        = pt_dent->t_clus_cache.at_cache[0].ui4_pclus 
        = ui4_this_pclus;
    pt_dent->t_clus_cache.ui4_tail_lclus
        = pt_dent->t_clus_cache.ui4_head_lclus
        = pt_dent->t_clus_cache.at_cache[0].ui4_lclus 
        = ui4_this_lclus;
    pt_dent->t_clus_cache.ui4_last_valid_entry = 0;
    
    while( pt_dent->t_clus_cache.ui4_last_valid_entry < (FAT_CLUSTER_CACHE_ENTRY_NUM - 1))
    {
        ui4_start_search_pclus = ui4_this_pclus;

        /* load data from storage */
        if ((ui4_ent_nib_len == 8) || (ui4_ent_nib_len == 4))
         {
            if ((UINT64)(ui4_start_search_pclus + ((ui4_fat_entries_len * 2) / ui4_ent_nib_len)) > ui8_partition_last_pclus)
            {
                ui4_load_size = (UINT32)((ui8_partition_last_pclus - ui4_start_search_pclus + 1) * (ui4_ent_nib_len / 2));
         }
         else
            {
                ui4_load_size = ui4_fat_entries_len;
            }

            /* set search boundry */
            ui4_end_search_pclus = ui4_start_search_pclus + (ui4_load_size * 2) / ui4_ent_nib_len - 1;

        i4_ret = _fat_dev_cache_read(pt_mount->pt_dev_xent,
                                         pui1_fat_entries,
                                     ui4_load_size,
                                         pt_fat_info->ui8_fat_off + (ui4_start_search_pclus * (ui4_ent_nib_len / 2)),
                                     &ui4_done,
                                         pt_fat_info->h_lock,
                                         TRUE,
                                         ui4_prefetch_limit);
        if (i4_ret != FMR_OK)
        {
                goto FAIL;
        }            
        }
        else
        {
            UINT32 ui4_fat_byte_ns;
            UINT32 ui4_start_byte_idx;
        
            if (((pt_fat_info->ui4_valid_clus_cnt + 2)% 2) == 0)
            {
                ui4_fat_byte_ns = (((pt_fat_info->ui4_valid_clus_cnt + 2) * ui4_ent_nib_len) / 2);
            }
            else
            {
                ui4_fat_byte_ns = (((pt_fat_info->ui4_valid_clus_cnt + 2) * ui4_ent_nib_len) / 2) + 1;
            }

            ui4_start_byte_idx = ((ui4_start_search_pclus * ui4_ent_nib_len) / 2);

            if ((ui4_start_byte_idx + ui4_fat_entries_len) > ui4_fat_byte_ns)
            {
                ui4_load_size = ui4_fat_byte_ns - ui4_start_byte_idx;
                ui4_end_search_pclus = (pt_fat_info->ui4_valid_clus_cnt + 2) - 1;
            }
            else
            {
                ui4_load_size = ui4_fat_entries_len;

                if ((ui4_start_search_pclus % 2) == 0)
                {
                    ui4_end_search_pclus = ui4_start_search_pclus + ((ui4_load_size * 2) / ui4_ent_nib_len) - 1;
                }
                else
                {
                    ui4_end_search_pclus = ui4_start_search_pclus + ((ui4_load_size * 2) / ui4_ent_nib_len) - 2;
                }
            }

            i4_ret = _fat_dev_cache_read(pt_mount->pt_dev_xent,
                                         pui1_fat_entries,
                                         ui4_load_size,
                                         pt_fat_info->ui8_fat_off + ui4_start_byte_idx,
                                         &ui4_done,
                                         pt_fat_info->h_lock,
                                         FALSE,
                                         0);
            if (i4_ret != FMR_OK)
            {
                goto FAIL;
            }
        }            
        
        while( ui4_next_pclus <= ui4_end_search_pclus && ui4_this_pclus >= ui4_start_search_pclus
            &&  pt_dent->t_clus_cache.ui4_last_valid_entry < (FAT_CLUSTER_CACHE_ENTRY_NUM - 1))
        {
            if (ui4_ent_nib_len == 8)
            {
                ui4_next_pclus = (UINT32) FAT_GET_VAL32(pui1_fat_entries, (ui4_this_pclus - ui4_start_search_pclus) * (ui4_ent_nib_len / 2));
            }
            else if (ui4_ent_nib_len == 4)
            {
                ui4_next_pclus = (UINT32) FAT_GET_VAL16(pui1_fat_entries, (ui4_this_pclus - ui4_start_search_pclus) * (ui4_ent_nib_len / 2));
            }
            else
            {
                UINT16 ui2_fat_val;

                if ((ui4_start_search_pclus % 2) == 0)
                {
                    ui2_fat_val = FAT_GET_VAL16(pui1_fat_entries, (((ui4_this_pclus - ui4_start_search_pclus) * ui4_ent_nib_len) / 2));
            }
                else
                {
                    ui2_fat_val = FAT_GET_VAL16(pui1_fat_entries, (((ui4_this_pclus - ui4_start_search_pclus) * ui4_ent_nib_len + 1) / 2));
            }

                if ((ui4_this_pclus % 2) == 0)
                {
                    ui4_next_pclus = (UINT32)(ui2_fat_val & 0x0FFF);
                }
                else
                {
                    ui4_next_pclus = (UINT32)(ui2_fat_val >> 4);
                }
            }

            ui4_this_lclus++;
            
            if ( ui4_next_pclus != ui4_this_pclus + 1)
            {/* discontinuous cluster number*/

		            if (ui4_next_pclus >= ui4_eoc)
		            {
                    ui4_next_pclus = FAT_EOC_MARK;
		            }

                ASSERT( pt_dent->t_clus_cache.ui4_last_valid_entry < FAT_CLUSTER_CACHE_ENTRY_NUM - 1);

                pt_dent->t_clus_cache.ui4_last_valid_entry++;
                pt_dent->t_clus_cache.at_cache[pt_dent->t_clus_cache.ui4_last_valid_entry].ui4_pclus = ui4_next_pclus;
                pt_dent->t_clus_cache.at_cache[pt_dent->t_clus_cache.ui4_last_valid_entry].ui4_lclus = ui4_this_lclus;
            }

            pt_dent->t_clus_cache.ui4_tail_pclus = ui4_next_pclus;
            pt_dent->t_clus_cache.ui4_tail_lclus = ui4_this_lclus;
            ui4_this_pclus = ui4_next_pclus;
        }

        if (  ui4_this_pclus >= ui4_eoc)
        {
            ui4_this_pclus = FAT_EOC_MARK;
            break;
        }

        if (ui4_this_pclus == ui4_start_search_pclus)
        {
            i4_ret = FMR_FAT_ERROR;
            goto FAIL;
        }
    }

FAIL:

    if (pui1_fat_entries != NULL)
    {
        x_mem_free(pui1_fat_entries);
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: _fat_get_pclus
 *
 * Description:  Get physical cluster number.
 *
 * Inputs:  ui4_lclus_idx
 *              pt_mount
 *              pt_dent
 *
 * Outputs: pui4_pclus_idx
 *
 * Returns: FMR_OK
 *                  Success.
 *              FMR_EOC
 *                  This logical cluster number touches the end of file.
 *              Others
 *                  Error.
 ----------------------------------------------------------------------------*/

static INT32    _fat_get_pclus(
    UINT32 *pui4_pclus_idx, 
    UINT32   ui4_lclus_idx, 
    FM_MOUNT_T *pt_mount, 
    FAT_DENTRY_T *pt_dent)
{
    INT32   j;
    INT32 i4_ret;

    if ( ui4_lclus_idx < pt_dent->t_clus_cache.ui4_head_lclus
     || ui4_lclus_idx > pt_dent->t_clus_cache.ui4_tail_lclus
     || pt_dent->t_clus_cache.at_cache[0].ui4_pclus == 0/*not initial ever*/)
     { /* cache miss! */
        i4_ret = _fat_build_clus_cache2(pt_mount, pt_dent, ui4_lclus_idx);
        if (i4_ret != FMR_OK && i4_ret != FMR_EOC)
            return FMR_INVAL;
        if ( i4_ret == FMR_EOC)
        {
            *pui4_pclus_idx = FAT_EOC_MARK;
            return FMR_EOC;
        }
     }
     
    for (j = (INT32)pt_dent->t_clus_cache.ui4_last_valid_entry; j >= 0; j--)
    {
        if( ui4_lclus_idx >= pt_dent->t_clus_cache.at_cache[j].ui4_lclus)
        {
            *pui4_pclus_idx 
                = pt_dent->t_clus_cache.at_cache[j].ui4_pclus
                    + (ui4_lclus_idx - pt_dent->t_clus_cache.at_cache[j].ui4_lclus);
            break;
        }
    }

#if 0 //  it is possible that we are deleting an abnormal ended file
    ASSERT(*pui4_pclus_idx != 0);
#endif

    if (*pui4_pclus_idx >= FAT_EOC_MARK)
            {
        return FMR_EOC;
            }

    if (*pui4_pclus_idx == 0)
            {
        return FMR_ABNORMAL_ENDED;
    }

    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _fat_async_done
 *
 * Description: Asynchronous notification.
 *
 * Inputs:  pv_tag
 *              Reference to read/write argument.
 *          e_async_cond
 *              Asynchronous condition.
 *          ui4_data
 *              Read/write done bytes.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fat_async_done(
    HANDLE_T    h_req,
    VOID            *pv_tag,
    FM_ASYNC_COND_T e_async_cond,
    UINT32          ui4_data)
{
    FM_RW_ARG_T *pt_arg;
    
    ASSERT(pv_tag != NULL);

    pt_arg = (FM_RW_ARG_T *)pv_tag;
    pt_arg->ui4_done_bytes += ui4_data;

    switch ( e_async_cond)
    {
        case FM_ASYNC_COND_ABORT_OK:
            pt_arg->ui4_req_ok_cnt++;
            break;
        case FM_ASYNC_COND_READ_OK:
            pt_arg->ui4_req_ok_cnt++;
            break;
        case FM_ASYNC_COND_WRITE_OK:
            pt_arg->ui4_req_ok_cnt++;
            break;
        case FM_ASYNC_COND_FAIL:
            pt_arg->ui4_req_fail_cnt++;
            break;
        case FM_ASYNC_COND_ABORT_FAIL:
            pt_arg->ui4_req_fail_cnt++;
            break;
        default:
            ASSERT(0);
            break;

    }

    if ((pt_arg->ui1_all_req_sent == TRUE) &&
        (pt_arg->ui4_req_cnt == (pt_arg->ui4_req_ok_cnt + pt_arg->ui4_req_fail_cnt)))
    {
        if (pt_arg->ui4_req_fail_cnt == 0)
        {
            if (pt_arg->pf_nfy_fct)
            {
                if (pt_arg->ui4_done_bytes < pt_arg->ui4_count)
                {
                    pt_arg->pf_nfy_fct(pt_arg->h_rw_req, 
                                       pt_arg->pv_tag, 
                                       FM_ASYNC_COND_ABORT_OK, 
                                       pt_arg->ui4_done_bytes);
                }
                else
                {
                    if (pt_arg->ui1_aborted == TRUE)
                    {
                        pt_arg->pf_nfy_fct(pt_arg->h_rw_req, 
                                           pt_arg->pv_tag, 
                                           FM_ASYNC_COND_ABORT_FAIL, 
                                           pt_arg->ui4_done_bytes);
                    }
                    else
                    {
                        pt_arg->pf_nfy_fct(pt_arg->h_rw_req, 
                                           pt_arg->pv_tag, 
                                           e_async_cond, 
                                           pt_arg->ui4_done_bytes);
                    }
                }
            }

            x_handle_free(h_req);
            x_mem_free(pt_arg);
        }
        else
        {
            if (pt_arg->ui4_done_bytes < pt_arg->ui4_count)
            {
                pt_arg->pf_nfy_fct(pt_arg->h_rw_req, 
                                   pt_arg->pv_tag, 
                                   FM_ASYNC_COND_FAIL, 
                                   pt_arg->ui4_done_bytes);
            }

            x_handle_free(h_req);
            x_mem_free(pt_arg);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _fat_rw_buf
 *
 * Description: Perform read/write operation to a node(file/directory).
 *
 * Inputs:  pt_arg
 *              Reference to read/write argument.
 *          b_read
 *              TRUE for read operation, FALSE for write operation.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 _fat_rw_buf(
    FM_RW_ARG_T *pt_arg,
    BOOL         b_read,
    BOOL         b_prefetch)
{
    FM_XENTRY_T  *pt_xent;
    FAT_DENTRY_T *pt_dent;
    FAT_FSINFO_T *pt_fat_info;
    FM_MOUNT_T   *pt_mount;
    FM_RW_ARG_T  *pt_rw_arg = NULL;
    FM_RW_ARG_T   t_rw_arg;
    UINT8  *pui1_usr_data;
    UINT32 ui4_lclus; /* starting logical cluster of this operation */
    UINT32 ui4_rclus; /* endding logical cluster of this operation */
    UINT32 ui4_this_clus = 0;  /* physical cluster */
    UINT32 ui4_rw_bytes;
    UINT32 ui4_clus_size;
    UINT32 ui4_resid;
    UINT64 ui8_offset;
    UINT64 ui8_clus_off; /* logical cluster offset */
    UINT32 i;
    INT32   i4_ret;
    BOOL   b_sync;
    UINT32 ui4_prefetch_limit = 0;
    UINT32 ui4_sec_sz;
    
    DEBUG_TRACE();

    ASSERT(pt_arg != NULL);
    ASSERT(pt_arg->pt_xent != NULL);

    pt_xent = pt_arg->pt_xent;

    ASSERT(pt_xent->pt_mount != NULL);
    ASSERT(pt_xent->pt_mount->pt_dev_xent != NULL);
    ASSERT(pt_xent->pt_mount->pv_priv != NULL);
    ASSERT(pt_xent->pv_fs_node != NULL);

    pt_dent     = FAT_GET_DENTRY(pt_xent);
    pt_mount    = pt_xent->pt_mount;
    pt_fat_info = (FAT_FSINFO_T *)pt_mount->pv_priv;

    if (IS_XENTRY_BLK(pt_xent->pt_mount->pt_dev_xent))
    {
        ui4_sec_sz = (UINT32)pt_xent->pt_mount->pt_dev_xent->pt_dev->u_param.t_blkdev.ui4_sec_size;
    }
    else
    {
        ui4_sec_sz = (UINT32)512;
    }

    if ((pt_dent->ui1_attr & FAT_ATTR_VOLUME_ID) && (pt_fat_info->e_fat_type != FAT_TYPE_32)) 
    {
        /* FAT12/16 root directory has its own region */

        pt_arg->pt_xent = pt_mount->pt_dev_xent;

        if ( pt_arg->ui8_offset >= pt_fat_info->ui4_root_size)
        {
            return FMR_EOF;
        }

        pt_arg->ui8_offset += pt_fat_info->ui8_root_off;

        i4_ret = fm_xentry_lock(pt_mount->pt_dev_xent, X_SEMA_OPTION_WAIT);
        if (i4_ret != FMR_OK)
        {
            return i4_ret;
        }

        i4_ret = fm_buf_read_write(pt_arg, b_read, FALSE, 0);

        fm_xentry_unlock(pt_mount->pt_dev_xent);

        pt_arg->ui8_offset -= pt_fat_info->ui8_root_off;
        pt_arg->pt_xent = pt_xent;

        return i4_ret;
    }

    b_sync = (pt_arg->ui2_flag & FM_IO_SYNC) ? TRUE : FALSE;
    if (!b_sync)
    {
        pt_rw_arg = x_mem_alloc(sizeof(FM_RW_ARG_T));
        if (pt_rw_arg == NULL)
        {
            return FMR_CORE;
        }
        *pt_rw_arg = *pt_arg;
        
        pt_rw_arg->ui4_req_cnt = 0;
        pt_rw_arg->ui4_req_ok_cnt = 0;
        pt_rw_arg->ui4_req_fail_cnt = 0;
        pt_rw_arg->ui1_all_req_sent = FALSE;
        pt_rw_arg->ui1_aborted = FALSE;

    }
    
    /*
     *  Figure out the logic data clusters we're interested in.
     */
    ui4_lclus = (UINT32)(pt_arg->ui8_offset / pt_fat_info->ui4_clus_size);
    ui4_rclus = (UINT32)((pt_arg->ui8_offset + pt_arg->ui4_count - 1) / pt_fat_info->ui4_clus_size);

    /*
     *  Set fixed arguments.
     */
    t_rw_arg.pt_xent  = pt_mount->pt_dev_xent;
    t_rw_arg.ui2_flag = pt_arg->ui2_flag;
    if (b_sync)
    {
        t_rw_arg.h_bin_sema = pt_arg->h_bin_sema;
    }
    else
    {
        t_rw_arg.ui1_pri    = pt_arg->ui1_pri;
        t_rw_arg.pf_nfy_fct = _fat_async_done;
        t_rw_arg.pv_tag     = pt_rw_arg;
        t_rw_arg.e_async_cond = pt_arg->e_async_cond;
    }

    pui1_usr_data = pt_arg->pv_data;
    ui4_resid     = pt_arg->ui4_count;
    ui8_offset    = pt_arg->ui8_offset;
    ui4_clus_size = pt_fat_info->ui4_clus_size;
    ui8_clus_off  = (UINT64)pt_fat_info->ui4_clus_size * ui4_lclus;

    for (i = ui4_lclus; i <= ui4_rclus; i++)
    {
       i4_ret = _fat_get_pclus(&ui4_this_clus, i , pt_xent->pt_mount, pt_dent);

        if ((FMR_OK != i4_ret) && (FMR_EOC != i4_ret))
            {
                goto FAIL;
            }

        if (i4_ret == FMR_EOC)
        {
            i4_ret = FMR_EOF;
            goto FAIL;
        }

        if (i >= ui4_lclus)
        {
            ui4_rw_bytes = (UINT32)(ui8_clus_off + ui4_clus_size - ui8_offset);
            if (ui4_rw_bytes > ui4_resid)
            {
                ui4_rw_bytes = ui4_resid;
            }

            t_rw_arg.pv_data = pui1_usr_data;
            t_rw_arg.ui4_count = ui4_rw_bytes;
            t_rw_arg.ui8_offset = FAT_CLUS_TO_DATA_ADDR(ui4_this_clus, pt_fat_info) + ui8_offset - ui8_clus_off;
            t_rw_arg.ui4_done_bytes = 0;

            i4_ret = fm_xentry_lock(pt_mount->pt_dev_xent, X_SEMA_OPTION_WAIT);
            if (i4_ret != FMR_OK)
            {
                goto FAIL;
            }

            /* ui4_prefetch_limit should be calculated when b_prefetch is TRUE */

            if (b_prefetch == FALSE)
            {
                ui4_prefetch_limit = 0;
            }
            else
            {
                UINT32 ui4_cont_clus_num;
                UINT32 ui4_pclus_idx = 0;
                UINT32 ui4_last_pclus = ui4_this_clus;

                for (ui4_cont_clus_num = 1;; ui4_cont_clus_num++)
                {
                    i4_ret = _fat_get_pclus(&ui4_pclus_idx, i + ui4_cont_clus_num, pt_xent->pt_mount, pt_dent);

                    if (ui4_pclus_idx == ui4_this_clus + ui4_cont_clus_num)
                    {
                        ui4_last_pclus = ui4_pclus_idx;

#if 1
                        /* break the loop earlier */

                        if ((ui4_cont_clus_num * ui4_clus_size) >= FM_BUF_CHUNK_SIZE)
                        {
                            break;
                        }
#endif

                    }
                    else
                    {
                        break;
                    }            
                }

                ui4_prefetch_limit = (UINT32)((FAT_CLUS_TO_DATA_ADDR(ui4_last_pclus, pt_fat_info) + (UINT64)ui4_clus_size) / (UINT64)ui4_sec_sz);

            }
            if (b_sync == FALSE && pt_rw_arg != NULL)
            {
                pt_rw_arg->ui4_req_cnt++;
                if (i == ui4_rclus)
                {
                    pt_rw_arg->ui1_all_req_sent = TRUE;
                }
            }

            i4_ret = fm_buf_read_write(&t_rw_arg, b_read, b_prefetch, ui4_prefetch_limit);

            fm_xentry_unlock(pt_mount->pt_dev_xent);

            if (i4_ret != FMR_OK)
            {
                goto FAIL;
            }

            if (b_sync)
            {
                ASSERT(t_rw_arg.ui4_done_bytes == ui4_rw_bytes);
                pt_arg->ui4_done_bytes += ui4_rw_bytes;
            }

            ui4_resid     -= ui4_rw_bytes;
            pui1_usr_data += ui4_rw_bytes;
            ui8_offset    += ui4_rw_bytes;
            ui8_clus_off  += ui4_clus_size;
        }
    }

    return FMR_OK;

FAIL:

    if (!b_sync)
    {
        /*
         *  It's a bit tricky here.
         *  We don't know how many pending requests that
         *  had been issued. So we mark it invalid, and
         *  descrease the size. Upon all issued requests
         *  finish (ui4_done_bytes >= ui4_count),
         *  pt_rw_arg will be free. And since FM_IO_INVAL
         *  is asserted, we won't invoke the callback,
         *  either.
         */
        if (pui1_usr_data == pt_rw_arg->pv_data)
        {
            x_mem_free(pt_rw_arg);
        }
        else
        {
            pt_rw_arg->ui2_flag |= FM_IO_INVAL;
            pt_rw_arg->ui4_count -= ((UINT32)pui1_usr_data - (UINT32)pt_rw_arg->pv_data);
        }
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: x_fm_fat_init
 *
 * Description: Initialize and register the FAT file system.
 *
 * Inputs:  pv_data
 *              File system private init data.
 *
 * Outputs: -
 *
 * Returns: Reference to an object of FM_FS_DESC_T type on success.
 *          Otherwise NULL is returned.
 ----------------------------------------------------------------------------*/
INT32 x_fm_fat_init(VOID *pv_data)
{
    FM_FS_DESC_T *pt_fs_desc = NULL;
    INT32 i4_ret;

    DEBUG_TRACE();

    pt_fs_desc = x_mem_alloc(sizeof(FM_FS_DESC_T));
    if (pt_fs_desc == NULL)
    {
        return FMR_CORE;
    }

    x_strcpy(pt_fs_desc->ps_name, "fat");
    pt_fs_desc->e_fs_type  = FM_TYPE_FAT;
    pt_fs_desc->i4_ref_cnt = 0;
    pt_fs_desc->pt_ufs_op  = &t_fat_ufs_ops;

    i4_ret = fm_reg_fs(pt_fs_desc);
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pt_fs_desc);
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fat_format
 *
 * Description: Format a device to the FAT file system.
 *
 * Inputs:  pv_data
 *              Reference to FM_CREATE_FS_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_format(
    VOID        *pv_data)
{
    FM_CREATE_FS_ARG_T *pt_arg;
    FM_XENTRY_T *pt_xent;
    FM_DEVICE_T *pt_dev;
    FAT_TYPE_T  e_fat_type = FAT_TYPE_ANY;
    HANDLE_T    h_sync_lock;
    UINT8  *pui1_bs = NULL;
    UINT8  *pui1_fat = NULL;
    ROOTFS_ENTRY_T  *pt_rent = NULL;
    UINT8  ui1_sec_per_clus;
    UINT8  ui1_num_fat = 2;
    UINT16 ui2_root_ent_cnt;
    UINT16 ui2_rsv_sec_cnt;
    UINT32 ui4_root_dir_sec;
    UINT32 ui4_fat_size;
    UINT32 ui4_disk_sec;
    UINT32 ui4_sec_size;
    UINT32 ui4_free_cnt;
    UINT32 ui4_tmp1, ui4_tmp2;
    UINT64 ui8_offset;
    UINT32 ui4_done;
    UINT32  i, j;
    INT32  i4_ret = FMR_OK;

    if (pv_data == NULL)
    {
    ASSERT(pv_data != NULL);
        return FMR_ARG;
    }
    
    pt_arg = (FM_CREATE_FS_ARG_T *)pv_data;

    pt_xent    = pt_arg->pt_xent;
    h_sync_lock = pt_arg->h_sync_bin;

    if ((pt_xent == NULL) || (!IS_XENTRY_LOCKED(pt_xent)) || (h_sync_lock == NULL_HANDLE))
    {
    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_xent));
    ASSERT(h_sync_lock != NULL_HANDLE);
        return FMR_INVAL;
    }

    pt_dev = pt_xent->pt_dev;
    pt_rent = ROOTFS_GET_ENT(pt_xent);

    /* Determine some parameters. */
    /* Here should be a visual device */
    if (IS_XENTRY_VIRT(pt_xent))
    {
    if (IS_XENTRY_BLK(pt_xent))
    {
            /* Block device */
            ui4_sec_size = pt_dev->u_param.t_blkdev.ui4_sec_size;
            ASSERT(pt_rent != NULL);
            if (pt_rent->u.pt_part != NULL)
            {
                /* Multi partitions or single partition*/
                /* Here we should not use the "pt_dev->ui8_space" as it is the total size of the device, not the partition size. */
                ui4_disk_sec = pt_rent->u.pt_part->ui4_sectors;
                if ((UINT64)ui4_disk_sec > pt_dev->u_param.t_blkdev.ui8_sec_num)
                {
                    /* Just for case */
                    ui4_disk_sec = (UINT32)pt_dev->u_param.t_blkdev.ui8_sec_num;
                }
    }
    else
    {
                ASSERT(0);
                return FMR_INVAL;
            }
        }
        else
        {
            /* character device */
            ui4_sec_size = 512;
            
            ui4_disk_sec = (UINT32)(pt_dev->ui8_space >> 9);
        }
    }
    else
    {
        return FMR_INVAL;
    }

    if (ui4_sec_size == 512)
    {
        ui1_sec_per_clus = _fat_disksize_to_sectors_per_cluster(ui4_disk_sec, &e_fat_type);
    if ((ui1_sec_per_clus == 0) || (e_fat_type == FAT_TYPE_ANY))
    {
            return FMR_INVAL;
        }
    }
    else
    {
        if ((UINT64)(ui4_disk_sec * ui4_sec_size) < (UINT64)0x80000000)
        {
            e_fat_type = FAT_TYPE_16;
        }
        else
        {
            e_fat_type = FAT_TYPE_32;
        }
#if 0
        /* The disk formatted by this way will be most efficient, but the FAT size
           may be so large */
        ui1_sec_per_clus = 1;

        while(1)
        {
            if (e_fat_type == FAT_TYPE_16)
            {
                if (ui4_disk_sec / ui1_sec_per_clus <= 0xFFFF)
                {
                    break;
                }
                else
                {
                    ui1_sec_per_clus *= 2;
                }
            }
            else
            {
                if (ui4_disk_sec / ui1_sec_per_clus <= 0x0FFFFFFF)
                {
                    break;
                }
                else
                {
                    ui1_sec_per_clus *= 2;
                }
            }
        }
        if (ui1_sec_per_clus * ui4_sec_size >= 0x8000)
        {
            return FMR_INVAL;
        }
#else
        /* This is the simplest method, but the disk formatted by this method may not be so efficient */
        ui1_sec_per_clus = (UINT8)(((UINT32)0x8000) / ui4_sec_size);
#endif
    }

    ui2_root_ent_cnt = (UINT16)((e_fat_type == FAT_TYPE_32) ? 0 : 512);
    ui2_rsv_sec_cnt  = (UINT16)((e_fat_type == FAT_TYPE_32) ? 32 : 1);

    /*
     *  Setup boot sector
     */
    pui1_bs = x_mem_alloc(ui4_sec_size * sizeof(UINT8));

    if (pui1_bs == NULL)
    {
        return FMR_CORE;
    }
    
    x_memset(pui1_bs, 0, ui4_sec_size);
    
    FAT_SET_VAL8(pui1_bs, FAT_BS_JMP0, 0xEB);
    FAT_SET_VAL8(pui1_bs, FAT_BS_JMP1, (UINT8)((e_fat_type == FAT_TYPE_32) ? 0x58 : 0x3C));
    FAT_SET_VAL8(pui1_bs, FAT_BS_JMP2, 0x90);
    x_memcpy(FAT_OFFSET(pui1_bs, FAT_BS_OEM_NAME), "MSWIN4.1", 8);

    FAT_SET_VAL16(pui1_bs, FAT_BS_BYTES_PER_SEC, ui4_sec_size);
    FAT_SET_VAL8(pui1_bs, FAT_BS_SEC_PER_CLUS, ui1_sec_per_clus);
    FAT_SET_VAL16(pui1_bs, FAT_BS_RSV_SEC_CNT, ui2_rsv_sec_cnt);
    FAT_SET_VAL8(pui1_bs,  FAT_BS_NUM_FAT, ui1_num_fat);
    FAT_SET_VAL16(pui1_bs, FAT_BS_ROOT_ENT_CNT, ui2_root_ent_cnt);
    FAT_SET_VAL8(pui1_bs,  FAT_BS_MEDIA, 0xF8);
    FAT_SET_VAL16(pui1_bs, FAT_BS_SEC_PER_TRK, 63);
    FAT_SET_VAL16(pui1_bs, FAT_BS_NUM_HEADS, 255);
    FAT_SET_VAL32(pui1_bs, FAT_BS_HIDDEN_SEC, 0);
    
    if ((e_fat_type == FAT_TYPE_32) || (ui4_disk_sec > 0xFFFF))
    {
        FAT_SET_VAL32(pui1_bs, FAT_BS_TOTAL_SEC32, ui4_disk_sec);
    }
    else
    {
        FAT_SET_VAL16(pui1_bs, FAT_BS_TOTAL_SEC16, ui4_disk_sec);
    }
    
    /*
     *  Determine fat size.
     */
    if (e_fat_type == FAT_TYPE_32)
    {
        ui4_root_dir_sec = (UINT32)(ui1_sec_per_clus * 2) /* sec_per_clus * root_clus */;
    }
    else
    {
        ui4_root_dir_sec = ((ui2_root_ent_cnt * 32) + (ui4_sec_size - 1)) / ui4_sec_size;
    }

    ui4_tmp1 = ui4_disk_sec - (ui2_rsv_sec_cnt + ui4_root_dir_sec);
    
    if (e_fat_type == FAT_TYPE_32)
        {
        ui4_tmp2 = (UINT32)((ui4_sec_size * ui1_sec_per_clus / 4) + ui1_num_fat);
        }
        else
        {
        ui4_tmp2 = (UINT32)((ui4_sec_size * ui1_sec_per_clus / 2) + ui1_num_fat);
    }

    ui4_fat_size = (ui4_tmp1 + (ui4_tmp2 - 1)) / ui4_tmp2;
    
    if (e_fat_type == FAT_TYPE_32)
    {
        FAT_SET_VAL32(pui1_bs, FAT_BS_FAT_SIZE32, ui4_fat_size);
        FAT_SET_VAL32(pui1_bs, FAT_BS_ROOT_CLUS, 2);
        FAT_SET_VAL16(pui1_bs, FAT_BS_FS_INFO, 1);
        FAT_SET_VAL16(pui1_bs, FAT_BS_BK_BOOT_SEC, 6);
        FAT_SET_VAL8(pui1_bs,  FAT_BS_BOOT_SIG32, 0x29);
        x_memcpy(FAT_OFFSET(pui1_bs, FAT_BS_VOL_LAB32), "NO NAME    ", 11);
        x_memcpy(FAT_OFFSET(pui1_bs, FAT_BS_FS_TYPE32), "FAT32   ", 8);
    }
    else
    {
        FAT_SET_VAL16(pui1_bs, FAT_BS_FAT_SIZE16, ui4_fat_size);
        FAT_SET_VAL8(pui1_bs,  FAT_BS_BOOT_SIG, 0x29);
        x_memcpy(FAT_OFFSET(pui1_bs, FAT_BS_VOL_LAB), "NO NAME    ", 11);
        x_memcpy(FAT_OFFSET(pui1_bs, FAT_BS_FS_TYPE), "FAT16   ", 8);
    }

    pui1_bs[510] = 0x55;
    pui1_bs[511] = 0xAA;

    i4_ret = _fat_dev_cache_write(pt_xent, pui1_bs, ui4_sec_size, 0, &ui4_done, h_sync_lock, FALSE, 0);

    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }

    /*
     *  Construct FAT32 FSInfo Sector
     */
    if (e_fat_type == FAT_TYPE_32)
    {
        x_memset(pui1_bs, 0, ui4_sec_size);
    
        pui1_bs[FAT_FSI_LEAD_SIG] = 0x52;
        pui1_bs[FAT_FSI_LEAD_SIG + 1] = 0x52;
        pui1_bs[FAT_FSI_LEAD_SIG + 2] = 0x61;
        pui1_bs[FAT_FSI_LEAD_SIG + 3] = 0x41;
        pui1_bs[FAT_FSI_STRUC_SIG] = 0x72;
        pui1_bs[FAT_FSI_STRUC_SIG + 1] = 0x72;
        pui1_bs[FAT_FSI_STRUC_SIG + 2] = 0x41;
        pui1_bs[FAT_FSI_STRUC_SIG + 3] = 0x61;

        ui4_free_cnt = (ui4_disk_sec -                 /* total sectors */
                        ui2_rsv_sec_cnt -               /* reserved sectors */
                        ui4_fat_size * ui1_num_fat -    /* fat sectors */
                        ui4_root_dir_sec) /             /* root dir sectors = 2 */
                        ui1_sec_per_clus;               /* sectors per cluster */

        pui1_bs[FAT_FSI_FREE_COUNT] = (UINT8)(FAT_UINT32(ui4_free_cnt));
        pui1_bs[FAT_FSI_NXT_FREE] = 0x03;
        pui1_bs[FAT_FSI_TRAIL_SIG] = 0x00;
        pui1_bs[FAT_FSI_TRAIL_SIG + 1] = 0x00;
        pui1_bs[FAT_FSI_TRAIL_SIG + 2] = 0x55;
        pui1_bs[FAT_FSI_TRAIL_SIG + 3] = 0xAA;

        i4_ret = _fat_dev_cache_write(pt_xent, pui1_bs, ui4_sec_size, ui4_sec_size, &ui4_done, h_sync_lock, FALSE, 0);
        if (i4_ret != FMR_OK)
        {
            goto EXIT;
        }
    }

    /*
     *  Construct the FAT sectors
     */
    ui8_offset  = ui2_rsv_sec_cnt * ui4_sec_size;

    pui1_fat = x_mem_alloc(ui4_sec_size*1024);
    if(pui1_fat == NULL)
    {
        /* Allow the allocatation is NULL */
    }

    for (i = 0; i < ui1_num_fat; i++)
    {
        x_memset(pui1_bs, 0, ui4_sec_size);

        pui1_bs[0] = 0xF8;
        if (e_fat_type == FAT_TYPE_12)
        {
            pui1_bs[1] = pui1_bs[2] = 0xFF;
        }
        else if (e_fat_type == FAT_TYPE_16)
        {
            pui1_bs[1] = pui1_bs[2] = pui1_bs[3] = 0xFF;
        }
        else
        {
            pui1_bs[1] = pui1_bs[2] = 0xFF;
            pui1_bs[3] = 0x0f;
            FAT_SET_VAL32(pui1_bs, 4, 0xFFFFFFFF);
            pui1_bs[8] = pui1_bs[9] = pui1_bs[10] = 0xFF;
            pui1_bs[11] = 0x0f;
        }

        i4_ret = _fat_dev_cache_write(pt_xent, pui1_bs, ui4_sec_size, ui8_offset, &ui4_done, h_sync_lock, FALSE, 0);
        if (i4_ret != FMR_OK)
        {
            goto EXIT;
        }
        
        ui8_offset += ui4_done;

        if (pui1_fat == NULL)
        {
            x_memset(pui1_bs, 0, ui4_sec_size);

        for (j = 1; j < ui4_fat_size; j++)
        {
                i4_ret = _fat_dev_cache_write(pt_xent, pui1_bs, ui4_sec_size, ui8_offset, &ui4_done, h_sync_lock, FALSE, 0);
            if (i4_ret != FMR_OK)
            {
                goto EXIT;
            }
        
            ui8_offset += ui4_done;
        }
    }
        else
        {
            /* Bulk write */
            UINT32  ui4_n;

            x_memset(pui1_fat,0,ui4_sec_size*1024);

            for (j = 1; j < ui4_fat_size; j+=1024)
            {
                if(ui4_fat_size - j >= 1024)
                {
                    ui4_n = 1024;
                }
                else
                {
                    ui4_n = ui4_fat_size - j; 
                }
                
                i4_ret = _fat_dev_cache_write(pt_xent, pui1_fat, ui4_n * ui4_sec_size, ui8_offset, &ui4_done, h_sync_lock, FALSE, 0);
            if (i4_ret != FMR_OK)
            {
                goto EXIT;
            }
        
            ui8_offset += ui4_done;
        }
    }
    }

    /*
     *  Construct the root sectors
     */
    x_memset(pui1_bs, 0, ui4_sec_size);
    for (i = 0; i < ui4_root_dir_sec; i++)
    {
        i4_ret = _fat_dev_cache_write(pt_xent, pui1_bs, ui4_sec_size, ui8_offset, &ui4_done, h_sync_lock, FALSE, 0);
        if (i4_ret != FMR_OK)
        {
            goto EXIT;
        }
        
        ui8_offset += ui4_done;
    }

EXIT:

    if (pui1_bs != NULL)
    {
        x_mem_free(pui1_bs);
    }

    if (pui1_fat != NULL)
    {
        x_mem_free(pui1_fat);
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fat_mount
 *
 * Description: Mount to a FAT file system.
 *
 * Inputs:  pv_data
 *              Reference to FM_MOUNT_FS_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_mount(
    VOID        *pv_data)
{
    FM_MOUNT_FS_ARG_T *pt_mount_arg;
    FM_XENTRY_T       *pt_dev_xent;
    FM_MOUNT_T        *pt_mount;
    FAT_FSINFO_T      *pt_fat_info = NULL;
    FAT_DENTRY_T      *pt_dent = NULL;
    UINT32 ui4_offset;
    UINT32 ui4_done;
    INT32  i4_ret;
    
    UINT32 ui4_sec_sz;
    UINT32 ui4_prefetch_limit;
    UINT32 ui4_fat_sec_ns;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_mount_arg = (FM_MOUNT_FS_ARG_T *)pv_data;

    ASSERT(pt_mount_arg->pt_dev_xent != NULL);
    ASSERT(pt_mount_arg->pt_mount != NULL);

    pt_dev_xent = pt_mount_arg->pt_dev_xent;
    pt_mount    = pt_mount_arg->pt_mount;

    if (pt_dev_xent->pt_dev->e_drv_type == DRVT_OPTICAL_DISC)
    {
        return FMR_INVAL;
    }

    if (IS_XENTRY_BLK(pt_dev_xent))
    {
        ui4_sec_sz = (UINT32)pt_dev_xent->pt_dev->u_param.t_blkdev.ui4_sec_size;
    }
    else
    {
        ui4_sec_sz = (UINT32)512;
    }

    pt_fat_info = x_mem_calloc(1, sizeof(FAT_FSINFO_T));
    if (pt_fat_info == NULL)
    {
        return FMR_CORE;
    }

    if (x_sema_create(&pt_fat_info->h_lock,
                      X_SEMA_TYPE_BINARY,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        x_mem_free(pt_fat_info);
        return FMR_CORE;
    }

    pt_dent = x_mem_calloc(1, sizeof(FAT_DENTRY_T));
    if (pt_dent == NULL)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }

    /*
     *  Sanity check.
     */
     
    ui4_offset = 0;

    i4_ret = _fat_check_boot_sector(pt_dev_xent,
                                    ui4_offset,
                                    pt_mount->h_sync_sema,
                                    pt_fat_info);
    if (i4_ret != FMR_OK)
    {
        goto FAIL;
    }

    pt_fat_info->ui8_data_off += (UINT64)ui4_offset;
    pt_fat_info->ui8_fat_off  += (UINT64)ui4_offset;
    pt_fat_info->ui8_root_off += (UINT64)ui4_offset;

    /*
     *  Read the root directory entry.
     */
    if (pt_fat_info->e_fat_type != FAT_TYPE_32)
    {
        UINT8  *pui1_dent = NULL;

         pui1_dent = (UINT8*) x_mem_calloc(pt_fat_info->ui4_root_size, sizeof(UINT8) );

        if (pui1_dent == NULL)
        {
            goto FAIL;
        }
        
        i4_ret = _fat_dev_cache_read(pt_dev_xent,
                                     pui1_dent,
                                     pt_fat_info->ui4_root_size,
                                     pt_fat_info->ui8_root_off,
                                     &ui4_done,
                                     pt_mount->h_sync_sema,
                                     FALSE,
                                     0);
        if (i4_ret != FMR_OK)
        {
            x_mem_free(pui1_dent);
            goto FAIL;
        }

        _fat_get_dentry(pt_dent, pui1_dent);

        x_mem_free(pui1_dent);
        pui1_dent = NULL;
        if ((pt_dent->ui4_first_clus == 0) &&
            (pt_dent->ui1_attr & FAT_ATTR_VOLUME_ID))
        {
            /* Volume label file */

            /* XXX */
            /* What if it is not the first entry ??? */
        }
        pt_dent->ui1_attr |= FAT_ATTR_VOLUME_ID;
        pt_dent->ui4_first_clus = 0;
        pt_dent->ui4_size = pt_fat_info->ui4_root_size;
    }
    else
    {
        pt_dent->ui1_attr |= FAT_ATTR_VOLUME_ID;
        pt_dent->ui4_first_clus = pt_fat_info->ui4_root_clus;

        pt_dent->t_clus_cache.ui4_head_pclus
            = pt_dent->t_clus_cache.ui4_tail_pclus
            = pt_dent->t_clus_cache.at_cache[0].ui4_pclus
            = pt_dent->ui4_first_clus;
        pt_dent->t_clus_cache.ui4_head_lclus
            = pt_dent->t_clus_cache.ui4_tail_lclus
            = pt_dent->t_clus_cache.at_cache[0].ui4_lclus 
            = 0;
        pt_dent->t_clus_cache.ui4_last_valid_entry = 0;

        pt_dent->ui4_size = pt_fat_info->ui4_root_size;
    }

    /*
     *  Set mount structure.
     */
    pt_mount->t_fs_info.e_type = 
        (pt_fat_info->e_fat_type == FAT_TYPE_32) ? FM_TYPE_FAT32 :
        (pt_fat_info->e_fat_type == FAT_TYPE_16) ? FM_TYPE_FAT16 : FM_TYPE_FAT12;
    pt_mount->t_fs_info.ui2_max_name_len =
        (UINT16)((pt_fat_info->e_fat_type == FAT_TYPE_12) ? 11 : 255);
    pt_mount->t_fs_info.ui8_blk_size    = pt_fat_info->ui4_clus_size;
    pt_mount->t_fs_info.ui8_total_blk   = pt_fat_info->ui4_valid_clus_cnt;
    pt_mount->pv_priv                   = pt_fat_info;

    ASSERT(IS_XENTRY_LOCKED(pt_mount->pt_root_xent));
    pt_mount->pt_root_xent->pv_fs_node  = pt_dent;
    pt_mount->pt_root_xent->pt_ufs_ops  = &t_fat_ufs_ops;
    pt_mount->pt_root_xent->pt_xent_ops = &t_fat_xent_ops;

    ui4_fat_sec_ns = (pt_fat_info->ui4_fat_size / ui4_sec_sz) * pt_fat_info->ui1_num_fat;
    ui4_prefetch_limit = (UINT32)(pt_fat_info->ui8_fat_off / (UINT64)ui4_sec_sz) + ui4_fat_sec_ns;

    if ( pt_fat_info->e_fat_type == FAT_TYPE_32)
    {
        UINT32 *pui4_buf = NULL;
        UINT64 ui8_ofst = 0;
        UINT32 ui4_idx = 0;
        UINT32 ui4_total_valid_idx = 2;

        BOOL b_quit = FALSE;

        pui4_buf = (UINT32 *)x_mem_alloc(32 * 512); /* 16K buffer to scan FAT table */
        if (pui4_buf == NULL)
        {
            goto FAIL;
        }

        pt_mount->t_fs_info.ui8_free_blk = 0;
        pt_mount->t_fs_info.ui8_avail_blk = 0;

        for (;;)
        {
            i4_ret = _fat_dev_cache_read(pt_dev_xent,
                                         pui4_buf,
                                         32 * 512,
                                         pt_fat_info->ui8_fat_off + ui8_ofst,
                                         &ui4_done,
                                         pt_mount->h_sync_sema,
                                         TRUE,
                                         ui4_prefetch_limit);
            
            if (i4_ret != FMR_OK)
            {
                x_mem_free(pui4_buf);
                goto FAIL;
            }

            ui4_idx = (UINT32)((ui8_ofst == 0) ? 2 : 0);

            ui8_ofst += (32 * 512);

            for (;ui4_idx < ((32 * 512) / 4); ui4_idx++)
            {
                /* 
                   ui4_valid_clus_cnt is "valid cluster count" from cluster 2, not 
                   "max cluster number" beginnging from cluster 0.
                */

                if (ui4_total_valid_idx > (pt_fat_info->ui4_valid_clus_cnt + 1))
                {
                    b_quit = TRUE;
                    break;
                }

                if ((pui4_buf[ui4_idx] & 0x0FFFFFFF) == 0)
                {
                    pt_mount->t_fs_info.ui8_free_blk++;
                    pt_mount->t_fs_info.ui8_avail_blk++;
                }
 
                ui4_total_valid_idx++;
            }

            if (b_quit == TRUE)
            {
                break;
            }
        }

        x_mem_free(pui4_buf);
    } 
    else if (pt_fat_info->e_fat_type == FAT_TYPE_16)
    {
        UINT16 *pui2_buf = NULL;
        UINT64 ui8_ofst = 0;
        UINT32 ui4_idx = 0;
        UINT32 ui4_total_valid_idx = 2;

        BOOL b_quit = FALSE;

        pui2_buf = (UINT16 *)x_mem_alloc(32 * 512); /* 16K buffer to scan FAT table */
        if (pui2_buf == NULL)
        {
            goto FAIL;
        }

        pt_mount->t_fs_info.ui8_free_blk = 0;
        pt_mount->t_fs_info.ui8_avail_blk = 0;

        for (;;)
        {
            i4_ret = _fat_dev_cache_read(pt_dev_xent,
                                         pui2_buf,
                                         32 * 512,
                                         pt_fat_info->ui8_fat_off + ui8_ofst,
                                         &ui4_done,
                                         pt_mount->h_sync_sema,
                                         TRUE,
                                         ui4_prefetch_limit);
            
            if (i4_ret != FMR_OK)
            {
                x_mem_free(pui2_buf);
                goto FAIL;
            }

            ui4_idx = (UINT32)((ui8_ofst == 0) ? 2 : 0);

            ui8_ofst += (32 * 512);

            for (;ui4_idx < ((32 * 512) / 2); ui4_idx++)
            {
                /* 
                   ui4_valid_clus_cnt is "valid cluster count" from cluster 2, not 
                   "max cluster number" beginnging from cluster 0.
                */

                if (ui4_total_valid_idx > (pt_fat_info->ui4_valid_clus_cnt + 1))
                {
                    b_quit = TRUE;
                    break;
                }

                if ((pui2_buf[ui4_idx] & 0xFFFF) == 0)
                {
                    pt_mount->t_fs_info.ui8_free_blk++;
                    pt_mount->t_fs_info.ui8_avail_blk++;
                }
 
                ui4_total_valid_idx++;
            }

            if (b_quit == TRUE)
            {
                break;
            }
        }

        x_mem_free(pui2_buf);
    }
    else
    {
        UINT8 *pui1_buf = NULL;
        UINT64 ui8_ofst = 0;
        UINT32 ui4_idx = 0;
        UINT32 ui4_total_valid_idx = 2;

        BOOL b_quit = FALSE;

        pui1_buf = x_mem_alloc(510); /* 510 bytes for 510 * 2 / 3 entries */
        if (pui1_buf == NULL)
        {
            goto FAIL;
        }
        
        pt_mount->t_fs_info.ui8_free_blk = 0;
        pt_mount->t_fs_info.ui8_avail_blk = 0;

        for (;;)
        {
            i4_ret = _fat_dev_cache_read(pt_dev_xent,
                                         pui1_buf,
                                         510,
                                         pt_fat_info->ui8_fat_off + ui8_ofst,
                                         &ui4_done,
                                         pt_mount->h_sync_sema,
                                         FALSE,
                                         0);
            
            if (i4_ret != FMR_OK)
            {
                x_mem_free(pui1_buf);
                goto FAIL;
            }

            ui4_idx = (UINT32)((ui8_ofst == 0) ? 2 : 0);

            ui8_ofst += (510);

            for (;ui4_idx < (510 * 2) / 3; ui4_idx++)
            {
                UINT16 ui2_val;

                /* 
                   ui4_valid_clus_cnt is "valid cluster count" from cluster 2, not 
                   "max cluster number" beginnging from cluster 0.
                */

                if (ui4_total_valid_idx > (pt_fat_info->ui4_valid_clus_cnt + 1))
                {
                    b_quit = TRUE;
                    break;
                }

                ui2_val = FAT_GET_VAL16(pui1_buf, (ui4_idx * 3) / 2);

                if ((ui4_idx % 2) == 0)
                {
                    ui2_val = (ui2_val & 0x0FFF);
                }
                else
                {
                    ui2_val = (ui2_val >> 4);
                }

                if ((ui2_val & 0x0FFF) == 0)
                {
                    pt_mount->t_fs_info.ui8_free_blk++;
                    pt_mount->t_fs_info.ui8_avail_blk++;
                }
 
                ui4_total_valid_idx++;
            }

            if (b_quit == TRUE)
            {
                break;
            }
        }

        x_mem_free(pui1_buf);
    }
    
	  x_dbg_stmt("total: %llu, avail: %llu, free: %llu\n",

		pt_mount->t_fs_info.ui8_total_blk,
		pt_mount->t_fs_info.ui8_avail_blk,
		pt_mount->t_fs_info.ui8_free_blk);

    return FMR_OK;

FAIL:

    if (pt_dent != NULL)
    {
        x_mem_free(pt_dent);
	      pt_mount->pt_root_xent->pv_fs_node = NULL;
    }
    if (pt_fat_info != NULL)
    {
        if (pt_fat_info->h_lock != NULL_HANDLE)
        {
            x_sema_delete(pt_fat_info->h_lock);
        }
        
        x_mem_free(pt_fat_info);
	      pt_mount->pv_priv = NULL;
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fat_umount
 *
 * Description: Unmount a FAT file system.
 *
 * Inputs:  pv_data
 *              Reference to FM_UMOUNT_FS_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_umount(
    VOID        *pv_data)
{
    FM_UMOUNT_FS_ARG_T  *pt_umount_arg;
    FM_MOUNT_T          *pt_mount;
    FAT_FSINFO_T        *pt_fat_info;

    ASSERT(pv_data != NULL);
    
    pt_umount_arg = (FM_UMOUNT_FS_ARG_T *)pv_data;

    ASSERT(pt_umount_arg->pt_mount != NULL);

    pt_mount = pt_umount_arg->pt_mount;

    ASSERT(pt_mount->pt_root_xent != NULL);
    ASSERT(pt_mount->pt_root_xent->pv_fs_node != NULL);
    
    x_mem_free(pt_mount->pt_root_xent->pv_fs_node);
    pt_mount->pt_root_xent->pv_fs_node = NULL;

    ASSERT(pt_mount->pv_priv != NULL);

    pt_fat_info = (FAT_FSINFO_T *)pt_mount->pv_priv;
    x_sema_delete(pt_fat_info->h_lock);
    x_mem_free(pt_fat_info);
    
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: fat_lookup
 *
 * Description: Lookup an entry in the file system.
 *
 * Inputs:  pv_data
 *              Reference to FM_LOOKUP_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_lookup(
    VOID        *pv_data)
{
    FM_LOOKUP_ARG_T *pt_arg;
    FAT_DENTRY_T    *pt_new_dent;
    
    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_arg = (FM_LOOKUP_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_prnt_xent != NULL);
    ASSERT(pt_arg->ps_name      != NULL);
    
    pt_new_dent = _fat_lookup_entry(pt_arg->pt_prnt_xent, pt_arg->ps_name);
    if (pt_new_dent == NULL)
    {
        return FMR_NO_ENTRY;
    }

    pt_arg->pt_xent = _fat_make_xentry(pt_arg->pt_prnt_xent, pt_new_dent);
    if (pt_arg->pt_xent == NULL)
    {
        x_mem_free(pt_new_dent);
        return FMR_CORE;
    }

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: fat_read_dir
 *
 * Description: Read the FAT directory entry.
 *
 * Inputs:  pv_data
 *              Reference to FM_READ_DIR_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_read_dir(
    VOID        *pv_data)
{
    FM_READ_DIR_ARG_T   *pt_arg;
    FM_XENTRY_T         *pt_xent;
    FM_DIR_ENTRY_T      *pt_dir_ent;
    FAT_DENTRY_T        *pt_dent; /*t_dent;*/
    UINT32              ui4_read_cnt = 0;
    INT32               i4_ret = FMR_OK;

    FAT_FSINFO_T *pt_fat_info;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_arg = (FM_READ_DIR_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_arg->pt_xent));

    pt_xent = pt_arg->pt_xent;
    pt_dir_ent = pt_arg->pt_dir_ent;

    ASSERT(pt_dir_ent  != NULL);

    pt_dent = (FAT_DENTRY_T*) x_mem_alloc(sizeof(FAT_DENTRY_T));
    if (pt_dent == NULL)
    {
        return FMR_CORE;
    }

    if (pt_arg->ui4_ent_cnt == 0)
    {
        pt_arg->ui4_read_cnt = 0;
	      x_mem_free(pt_dent);
        return FMR_OK;
    }

    while (ui4_read_cnt < pt_arg->ui4_ent_cnt)
    {
        if (IS_XENTRY_MOUNT_ROOT(pt_xent) && (pt_arg->ui8_offset <= 32))
        {
            /*
             *  In FAT, there is no . and .. in the root directory.
             *  Create the virtual . and .. to make it consistent with
             *  general directories.
             */
            ASSERT((pt_arg->ui8_offset == 0) || (pt_arg->ui8_offset == 32));

            pt_dent->s_utf8[0] = '.';
            if (pt_arg->ui8_offset == 0)
            {
                pt_dent->s_utf8[1] = '\0';
                pt_dent->ui2_len = 1;
            }
            else
            {
                pt_dent->s_utf8[1] = '.';
                pt_dent->s_utf8[2] = '\0';
                pt_dent->ui2_len = 2;
            }

            pt_arg->ui8_offset += 32;
        }
        else
        {
            if (IS_XENTRY_MOUNT_ROOT(pt_xent))
            {
                pt_arg->ui8_offset -= 64;
                i4_ret = _fat_read_next_dentry(pt_xent, pt_dent, &pt_arg->ui8_offset);

                pt_arg->ui8_offset += 64;
            }
            else
            {
                i4_ret = _fat_read_next_dentry(pt_xent, pt_dent, &pt_arg->ui8_offset);
            }
            if (i4_ret != FMR_OK)
            {
                goto EXIT;
            }
        }

        if (_fat_check_name_legality((CHAR *)pt_dent->s_utf8) == FALSE)
        {
            continue;
        }

        x_strcpy(pt_dir_ent->s_name, (CHAR *)pt_dent->s_utf8);        
        ASSERT(pt_dent->ui2_len <= 255);
        pt_dir_ent->ui1_len = (UINT8)pt_dent->ui2_len;

        pt_dir_ent->t_file_info.ui4_inode    = pt_dent->ui4_first_clus;
        pt_dir_ent->t_file_info.ui4_mode     = _fat_attr_to_ufs_mode(pt_dent->ui1_attr);
        pt_dir_ent->t_file_info.ui4_uid      = 0;
        pt_dir_ent->t_file_info.ui4_gid      = 0;
        pt_dir_ent->t_file_info.ui8_size     = pt_dent->ui4_size;
        pt_dir_ent->t_file_info.ui8_offset   = 0;
        pt_dir_ent->t_file_info.ui4_blk_size = pt_arg->pt_xent->pt_mount->t_fs_info.ui8_blk_size;
        pt_dir_ent->t_file_info.ui8_blk_cnt  = 
            (pt_dent->ui4_size + pt_dir_ent->t_file_info.ui4_blk_size - 1) / pt_dir_ent->t_file_info.ui4_blk_size;
        pt_dir_ent->t_file_info.ui8_create_time =
            (TIME_T)_fat_conv_to_utc_time(pt_dent->ui2_cdate, pt_dent->ui2_ctime, pt_dent->ui1_crt_time_tenth);
        pt_dir_ent->t_file_info.ui8_access_time =
            (TIME_T)_fat_conv_to_utc_time(pt_dent->ui2_adate, 0, 0);
        pt_dir_ent->t_file_info.ui8_modify_time =
            (TIME_T)_fat_conv_to_utc_time(pt_dent->ui2_wdate, pt_dent->ui2_wtime, 0);

        pt_fat_info = (FAT_FSINFO_T *)pt_arg->pt_xent->pt_mount->pv_priv;
        
        ASSERT(pt_fat_info != NULL);

        pt_dir_ent->t_file_info.ui4_start_lba = FAT_CLUS_TO_DATA_SEC(
                                                    pt_dent->ui4_first_clus,
                                                    pt_fat_info);

        pt_dir_ent++;
        ui4_read_cnt++;
    }

EXIT:
    i4_ret = (ui4_read_cnt == 0) ? FMR_EOF : FMR_OK;

    pt_arg->ui4_read_cnt = ui4_read_cnt;

    x_mem_free(pt_dent);

    return i4_ret; 
}


/*-----------------------------------------------------------------------------
 * Name: fat_get_info
 *
 * Description: Get the information of an entry.
 *
 * Inputs:  pv_data
 *              Reference to FM_FILE_INFO_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_get_info(
    VOID        *pv_data)
{
    FM_FILE_INFO_ARG_T  *pt_arg;
    FAT_DENTRY_T        *pt_dent;

    FAT_FSINFO_T *pt_fat_info;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);
    
    pt_arg = (FM_FILE_INFO_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_xent != NULL);
    
    pt_dent = FAT_GET_DENTRY(pt_arg->pt_xent);

    ASSERT(pt_dent != NULL);

    pt_arg->pt_info->ui4_inode    = pt_dent->ui4_first_clus;
    pt_arg->pt_info->ui4_mode     = _fat_attr_to_ufs_mode(pt_dent->ui1_attr);
    pt_arg->pt_info->ui4_uid      = 0;
    pt_arg->pt_info->ui4_gid      = 0;
    pt_arg->pt_info->ui8_size     = pt_dent->ui4_size;
    pt_arg->pt_info->ui8_offset   = 0;
    pt_arg->pt_info->ui4_blk_size = pt_arg->pt_xent->pt_mount->t_fs_info.ui8_blk_size;
    pt_arg->pt_info->ui8_blk_cnt  = 
        (pt_dent->ui4_size + pt_arg->pt_info->ui4_blk_size - 1) / pt_arg->pt_info->ui4_blk_size;
    pt_arg->pt_info->ui8_create_time =
        (TIME_T)_fat_conv_to_utc_time(pt_dent->ui2_cdate, pt_dent->ui2_ctime, pt_dent->ui1_crt_time_tenth);
    pt_arg->pt_info->ui8_access_time =
        _fat_conv_to_utc_time(pt_dent->ui2_adate, 0, 0);
    pt_arg->pt_info->ui8_modify_time =
        (TIME_T)_fat_conv_to_utc_time(pt_dent->ui2_wdate, pt_dent->ui2_wtime, 0);

    pt_fat_info = (FAT_FSINFO_T *)pt_arg->pt_xent->pt_mount->pv_priv;
    
    ASSERT(pt_fat_info != NULL);

    pt_arg->pt_info->ui4_start_lba = FAT_CLUS_TO_DATA_SEC(
                                          pt_dent->ui4_first_clus,
                                          pt_fat_info);

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: fat_set_info
 *
 * Description: Set the attributes of an entry.
 *
 * Inputs:  pv_data
 *              Reference to FM_FILE_INFO_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_set_info(
    VOID        *pv_data)
{
    FM_FILE_INFO_ARG_T  *pt_arg;
    FM_FILE_INFO_T      *pt_info;
    FM_XENTRY_T         *pt_xent;
    FAT_DENTRY_T        *pt_dent;
    FM_RW_ARG_T         t_io;
    UINT8               pui1_dent[32];
    UINT32              ui4_date_time;
    INT32               i4_ret;
    UINT32              i;
    UINT32              ui4_ent_nib_len;
    UINT32              ui4_done;
    UINT32              ui4_clus;
    FM_MOUNT_T  *pt_mount; 
    FAT_FSINFO_T    *pt_fat_info;
                
    
    DEBUG_TRACE();

    ASSERT(pv_data != NULL);
    
    pt_arg = (FM_FILE_INFO_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_xent != NULL);
    ASSERT(pt_arg->pt_info != NULL);
    
    pt_info = pt_arg->pt_info;
    pt_xent = pt_arg->pt_xent;
    pt_dent = FAT_GET_DENTRY(pt_xent);
    pt_mount = pt_xent->pt_mount;    
    ASSERT(pt_dent != NULL);

    /* set mode */
    if (pt_info->ui4_mode != FM_SET_MODE_NOVAL)
    {
        if ((pt_info->ui4_mode & FM_MODE_USR_WRITE) == 0)
        {
            pt_dent->ui1_attr |= FAT_ATTR_READ_ONLY;
        }
    }
    
    /* handle truncate / extend */
    if (pt_info->ui8_size != FM_SET_SIZE_NOVAL)
    {
        UINT32 ui4_old_clus;
        UINT32 ui4_new_clus;
        
        if (pt_info->ui8_size > (UINT64)0xFFFFFFFF)
        {
            return FMR_FILE_TOO_LARGE;
        }

        ui4_old_clus = (UINT32)FAT_SIZE_TO_CLUS(pt_dent->ui4_size, pt_xent);
        ui4_new_clus = (UINT32)FAT_SIZE_TO_CLUS(pt_info->ui8_size, pt_xent);

        if (ui4_new_clus != ui4_old_clus)
        {
            if (ui4_new_clus > ui4_old_clus)
            {
                i4_ret = _fat_extend_fat_chain(pt_xent->pt_mount,
                                               pt_dent,
                                               ui4_new_clus - ui4_old_clus,
                                               ui4_old_clus,

               /* do not zero out next cluster with cache 
               since it may overwrite data tag has been written. */
#if 0
                                               TRUE);
#else
                                               FALSE);
#endif
                if (i4_ret != FMR_OK)
                {
                    return i4_ret;
                }
            }
            else if (ui4_new_clus < ui4_old_clus)
            {
                i4_ret = _fat_inval_fat_chain(pt_xent, ui4_new_clus + 1);

                if ((i4_ret == FMR_EOC) || (i4_ret == FMR_ABNORMAL_ENDED))
                {
                    i4_ret = FMR_OK;
                }

                if (i4_ret != FMR_OK)
                {
                    return i4_ret;
                }

                /* should write EOC on the ui4_new_clus */
                pt_fat_info = (FAT_FSINFO_T *)pt_mount->pv_priv;
                ASSERT(pt_fat_info != NULL);

                switch (pt_fat_info->e_fat_type)
                {
                    case FAT_TYPE_32:
                        ui4_ent_nib_len = 8;
                        break;
                    case FAT_TYPE_16:
                        ui4_ent_nib_len = 4;
                        break;
                    case FAT_TYPE_12:
                        ui4_ent_nib_len = 3;
                        break;
                    default:
                        ASSERT(0);
                        return FMR_INVAL;
                }

                i4_ret = _fat_get_pclus( &ui4_clus, ui4_new_clus, pt_mount, pt_dent);

                ASSERT((FMR_EOC != i4_ret) && (FMR_ABNORMAL_ENDED != i4_ret));

                if (FMR_OK != i4_ret)
                {
                    return i4_ret;
                }

                if (ui4_ent_nib_len == 8)
                {
                    UINT32 ui4_val;

                    ui4_val = FAT_UINT32(FAT32_EOC_MARK);
     
                for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                {
                    i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                  &ui4_val,
                                                      (ui4_ent_nib_len / 2),
                                                      pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_clus * (ui4_ent_nib_len / 2)),
                                                  &ui4_done,
                                                      pt_mount->h_sync_sema,
                                                      FALSE,
                                                      0);
                        if (i4_ret != FMR_OK)
                        {
                            return i4_ret;
                        }
                    } 
                }
                else if (ui4_ent_nib_len == 4)
                {
                    UINT16 ui2_val;

                    ui2_val = FAT_UINT16(FAT16_EOC_MARK);
     
                    for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                    {
                        i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                      &ui2_val,
                                                      (ui4_ent_nib_len / 2),
                                                      pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + (ui4_clus * (ui4_ent_nib_len / 2)),
                                                      &ui4_done,
                                                      pt_mount->h_sync_sema,
                                                      FALSE,
                                                      0);
                        if (i4_ret != FMR_OK)
                        {
                            return i4_ret;
                        }
                    } 
                }
                else
                {
                    UINT16 ui2_val, ui2_fat_val;

                    i4_ret = _fat_dev_cache_read(pt_mount->pt_dev_xent,
                                                 &ui2_val,
                                                 sizeof(UINT16),
                                                 pt_fat_info->ui8_fat_off + ((ui4_clus * ui4_ent_nib_len) / 2),
                                                 &ui4_done,
                                                 pt_mount->h_sync_sema,
                                                 FALSE,
                                                 0);
                    if (i4_ret != FMR_OK)
                    {
                        return i4_ret;
                    }

                    /* The last FAT entry, write EOC */
                    ui2_fat_val = FAT_UINT16(FAT12_EOC_MARK);

                    if ((ui4_clus % 2) == 0)
                    {
                        ui2_fat_val = ui2_fat_val & 0x0FFF;
                        ui2_val = ui2_val & 0xF000;
                        ui2_val = ui2_val | ui2_fat_val;
                    }
                    else
                    {
                        ui2_fat_val = ui2_fat_val << 4;
                        ui2_val = ui2_val & 0x000F;
                        ui2_val = ui2_val | ui2_fat_val;
                    }

                    for (i = 0; i < pt_fat_info->ui1_num_fat; i++)
                    {
                        i4_ret = _fat_dev_cache_write(pt_mount->pt_dev_xent,
                                                      &ui2_val,
                                                      sizeof(UINT16),
                                                      pt_fat_info->ui8_fat_off + (pt_fat_info->ui4_fat_size * i) + ((ui4_clus * ui4_ent_nib_len) / 2),
                                                  &ui4_done,
                                                      pt_mount->h_sync_sema,
                                                      FALSE,
                                                      0);
                    if (i4_ret != FMR_OK)
                    {
                        return i4_ret;
                    }
                }
                } 
            }            
        }

        pt_dent->ui4_size = (UINT32)pt_info->ui8_size;
    }

    if (pt_info->ui8_create_time != (TIME_T)FM_SET_TIME_NOVAL)
    {
        ui4_date_time = _fat_conv_to_fat_time((UINT32)pt_info->ui8_create_time);
        pt_dent->ui2_ctime = (UINT16)(ui4_date_time & 0xFFFF);
        pt_dent->ui2_cdate = (UINT16)(ui4_date_time >> 16);
    }
    
    if (pt_info->ui8_access_time != (TIME_T)FM_SET_TIME_NOVAL)
    {
        ui4_date_time = _fat_conv_to_fat_time((UINT32)pt_info->ui8_access_time);
        pt_dent->ui2_adate = (UINT16)(ui4_date_time >> 16);
    }

    if (pt_info->ui8_modify_time != (TIME_T)FM_SET_TIME_NOVAL)
    {
        ui4_date_time = _fat_conv_to_fat_time((UINT32)pt_info->ui8_modify_time);
        pt_dent->ui2_wtime = (UINT16)(ui4_date_time & 0xFFFF);
        pt_dent->ui2_wdate = (UINT16)(ui4_date_time >> 16);
    }

    _fat_set_dentry(pui1_dent, pt_dent);

    ASSERT(pt_arg->pt_xent->pt_mount != NULL);
    ASSERT(pt_arg->pt_xent->pt_mount->h_sync_sema != NULL_HANDLE);

    t_io.pt_xent        = pt_arg->pt_prnt_xent;
    t_io.pv_data        = pui1_dent;
    t_io.ui4_count      = 32;
    t_io.ui8_offset     = (UINT64)pt_dent->ui4_name_off;
    t_io.ui4_done_bytes = 0;
    t_io.ui2_flag       = FM_IO_SYNC;
    t_io.h_bin_sema     = pt_arg->pt_xent->pt_mount->h_sync_sema;

    i4_ret = _fat_rw_buf(&t_io, FALSE, FALSE);
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fat_get_dir_info
 *
 * Description: Get the information of an entry.
 *
 * Inputs:  pv_data
 *              Reference to FM_DIR_INFO_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_get_dir_info(
    VOID        *pv_data)
{
    FM_DIR_INFO_ARG_T  *pt_arg;
    INT32   i4_ret;
    FM_RW_ARG_T t_io;
    UINT8   pui1_dent[32], ui1_attr;
    UINT64 ui8_offset = 0;
    FAT_FSINFO_T *pt_fs_info = NULL;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);
    
    pt_arg = (FM_DIR_INFO_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_xent != NULL);
    
    if (!IS_XENTRY_DIR(pt_arg->pt_xent))
    {
        return FMR_ARG;
    }

    pt_fs_info = (FAT_FSINFO_T *)pt_arg->pt_xent->pt_mount->pv_priv;

    /* 1. looply read 32-byte long directory entry structure */
    /* 2. if DIR_Name[0] == 0x00, stop read it                  */
    
    t_io.pt_xent        = pt_arg->pt_xent;
    t_io.pv_data        = pui1_dent;
    t_io.ui4_count      = 32;
    t_io.ui4_done_bytes = 0;
    t_io.ui2_flag       = FM_IO_SYNC;
    t_io.h_bin_sema     = pt_arg->pt_xent->pt_mount->h_sync_sema;

    pt_arg->pt_info->ui4_dir_num = 0;
    pt_arg->pt_info->ui4_file_num = 0;

    while(TRUE)
    {
        t_io.ui8_offset = ui8_offset;
        t_io.ui4_done_bytes = 0;
        i4_ret = _fat_rw_buf(&t_io, TRUE, TRUE);
        if (i4_ret != FMR_OK)
        {
            break;
        }
        ui8_offset += 32;

        if ( pui1_dent[0] == 0x00
            || (pui1_dent[0] == '.' && IS_XENTRY_MOUNT_ROOT(pt_arg->pt_xent)))
            break;
        
        if ( pui1_dent[0] != 0xE5)
        {
            ui1_attr = FAT_GET_VAL8(pui1_dent, FAT_DENTRY_ATTR);

            if ( (ui1_attr & FAT_ATTR_LONG_NAME_MASK) == (UINT8)FAT_ATTR_LONG_NAME
                    || ((ui1_attr & FAT_ATTR_VOLUME_ID) != 0) )
            {
                continue;/* sub part of long directory entry or volume id*/
            }
            if (_fat_check_dentry(pui1_dent, pt_fs_info) == FALSE)
            {
                continue;
            }
            if ( ui1_attr & FAT_ATTR_DIRECTORY)
            {
                pt_arg->pt_info->ui4_dir_num++;
            }
            else if (!(((pui1_dent[0] <= 0x20) && (pui1_dent[0] != 0x05)) ||
                (pui1_dent[0] == 0x22) || 
                ((pui1_dent[0] >= 0x2A) && (pui1_dent[0] <= 0x2C)) ||
                (pui1_dent[0] == 0x2E) ||
                ((pui1_dent[0] >= 0x3A) && (pui1_dent[0] <= 0x3F)) ||
                ((pui1_dent[0] >= 0x5B) && (pui1_dent[0] <= 0x5D)) ||
                (pui1_dent[0] == 0x7C)))
            {
                pt_arg->pt_info->ui4_file_num++;
            }
        }
        
    }

    if ( IS_XENTRY_MOUNT_ROOT(pt_arg->pt_xent))
    {
        pt_arg->pt_info->ui4_dir_num += 2;
    }

    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: fat_lseek
 *
 * Description: Change the current position of an opened file.
 *
 * Inputs:  pv_data
 *              The reference to FM_LSEEK_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_FILE_TOO_LARGE
 *              Exceed the maximum file size.
 ----------------------------------------------------------------------------*/
static INT32 fat_lseek(VOID *pv_data)
{
    FM_LSEEK_ARG_T  *pt_arg;
    FAT_DENTRY_T    *pt_dent;
    
    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_arg = (FM_LSEEK_ARG_T *)pv_data;
    
    ASSERT(pt_arg->pt_xent != NULL);
    ASSERT(pt_arg->pt_xent->pt_dev != NULL);

    pt_dent = FAT_GET_DENTRY(pt_arg->pt_xent);
    
    ASSERT(pt_dent != NULL);
    
    if (pt_arg->i8_offset > 0)
    {
        if (pt_arg->ui1_whence == FM_SEEK_BGN)
        {
            if (pt_arg->i8_offset <= (INT64)0xffffffff)  /* max FAT size is 4G */
            {
                pt_arg->ui8_cur_pos = (UINT64)pt_arg->i8_offset;
            }
            else
            {
                return FMR_FILE_TOO_LARGE;
            }
        }
        else if (pt_arg->ui1_whence == FM_SEEK_CUR)
        {
            if ((pt_arg->i8_offset + pt_arg->ui8_cur_pos) <= (INT64)0xffffffff)  /* max FAT size is 4G */
            {
                pt_arg->ui8_cur_pos += pt_arg->i8_offset;
        }
        else
        {
                return FMR_FILE_TOO_LARGE;
        }
        }
        else
        {
            if ((pt_arg->i8_offset + (UINT64)(pt_dent->ui4_size)) <= (INT64)0xffffffff)  /* max FAT size is 4G */
            {
                pt_arg->ui8_cur_pos = (UINT64)(pt_arg->i8_offset + (INT64)(pt_dent->ui4_size));
            }
            else
        {
            return FMR_FILE_TOO_LARGE;
        }
        }
    }
    else
    {
        if (pt_arg->ui1_whence == FM_SEEK_BGN)
        {
            pt_arg->ui8_cur_pos = 0;
        }
        else if (pt_arg->ui1_whence == FM_SEEK_CUR)
        {
            if (pt_arg->ui8_cur_pos + pt_arg->i8_offset > 0)
            {
                pt_arg->ui8_cur_pos += pt_arg->i8_offset;
            }
            else
            {
                pt_arg->ui8_cur_pos = 0;
            }
        }
        else
        {
            if (pt_arg->i8_offset + (INT64)(pt_dent->ui4_size) > 0)
            {
                pt_arg->ui8_cur_pos = (UINT64)(pt_arg->i8_offset + (INT64)(pt_dent->ui4_size));
        }
        else
        {
            pt_arg->ui8_cur_pos = 0;
        }
    }
    }
    
    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: fat_seek_dir
 *
 * Description: Change the current position of an opened file.
 *
 * Inputs:  pv_data
 *              The reference to FM_SEEK_DIR_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_FILE_TOO_LARGE
 *              Exceed the maximum file size.
 ----------------------------------------------------------------------------*/
static INT32    fat_seek_dir(VOID *pv_data)
{
    FM_SEEK_DIR_ARG_T   *pt_arg;
    INT32   i4_ent_off, i4_ret;
    FM_RW_ARG_T t_io;
    UINT8   aui1_dent[32], ui1_attr = 0;
    HANDLE_T    h_sync_sema = 0;
    UINT64 ui8_curr_pos;
    FAT_FSINFO_T *pt_fs_info = NULL;
    ASSERT(pv_data != NULL);

    pt_arg = (FM_SEEK_DIR_ARG_T*) pv_data;
    ASSERT(pt_arg->pt_xent != NULL);
    
    if (!IS_XENTRY_DIR(pt_arg->pt_xent))
    {
        return FMR_NOT_DIR;
    }

    ui8_curr_pos = pt_arg->ui8_cur_pos;
    i4_ent_off = pt_arg->i4_ent_off;
    pt_fs_info = (FAT_FSINFO_T *)pt_arg->pt_xent->pt_mount->pv_priv;

    /*required resource */
    i4_ret = x_sema_create( &h_sync_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
    if( i4_ret != OSR_OK)
        return FMR_CORE;

    t_io.pv_data = aui1_dent;
    t_io.ui4_count = 32;
    t_io.ui4_done_bytes = 0;
    t_io.ui1_pri     = 128;
    t_io.ui2_flag = FM_IO_SYNC;
    t_io.h_bin_sema = h_sync_sema;
    t_io.pt_xent = pt_arg->pt_xent;

    if ( i4_ent_off >= 0)
    {
        /* search the next entry */
        while( i4_ent_off)
        {
            if ( IS_XENTRY_MOUNT_ROOT(pt_arg->pt_xent))
            {
                if ( pt_arg->ui8_cur_pos < 64)
                {/* virtual directories "." and ".." */
                    i4_ent_off--;
                    pt_arg->ui8_cur_pos += 32;
                    continue;
                }

                ui8_curr_pos = pt_arg->ui8_cur_pos - 64;
            }
            
            t_io.ui8_offset = ui8_curr_pos;
            t_io.ui4_done_bytes = 0;

                i4_ret = _fat_rw_buf(&t_io, TRUE, TRUE);
            if ( i4_ret != FMR_OK)
            {
                x_sema_delete(h_sync_sema);
                return i4_ret;
            }
            ui8_curr_pos += 32;
            pt_arg->ui8_cur_pos += 32;
            
                if ( aui1_dent[0] != 0xE5 && aui1_dent[0] != 0x00 )
                {
            ui1_attr = FAT_GET_VAL8(aui1_dent, FAT_DENTRY_ATTR);
                
                    if ( (ui1_attr & FAT_ATTR_LONG_NAME_MASK) == (UINT8)FAT_ATTR_LONG_NAME
                            || ((ui1_attr & FAT_ATTR_VOLUME_ID) != 0) )
                    {
                        continue;/* sub part of long directory entry or volume id*/
                    }
                    if (_fat_check_dentry(aui1_dent, pt_fs_info) == FALSE)
                    {
                        continue;
                    }
                    if ( ui1_attr & FAT_ATTR_DIRECTORY)
            {
                i4_ent_off--;
            }
                    else if (!(((aui1_dent[0] <= 0x20) && (aui1_dent[0] != 0x05)) ||
                        (aui1_dent[0] == 0x22) || 
                        ((aui1_dent[0] >= 0x2A) && (aui1_dent[0] <= 0x2C)) ||
                        (aui1_dent[0] == 0x2E) ||
                        ((aui1_dent[0] >= 0x3A) && (aui1_dent[0] <= 0x3F)) ||
                        ((aui1_dent[0] >= 0x5B) && (aui1_dent[0] <= 0x5D)) ||
                        (aui1_dent[0] == 0x7C)))
            {
                i4_ent_off--;
            }
        }

        }
    }
    else
    {
        /* search the previous entry */
        while( i4_ent_off )
        {
            if ( pt_arg->ui8_cur_pos == 0)
            {
                x_sema_delete( h_sync_sema);
                return FMR_ARG;
            }

            if ( IS_XENTRY_MOUNT_ROOT(pt_arg->pt_xent))
            {
                if ( pt_arg->ui8_cur_pos <= 64)
                {/* virtual directories "." and ".." */
                    i4_ent_off++;
                    pt_arg->ui8_cur_pos -= 32;
                    continue;
                }
                ui8_curr_pos = pt_arg->ui8_cur_pos - 64;
            }            
            
            t_io.ui8_offset = ui8_curr_pos - 32;
            t_io.ui4_done_bytes = 0;
                i4_ret = _fat_rw_buf(&t_io, TRUE, TRUE);
            if ( i4_ret != FMR_OK)
            {
                x_sema_delete(h_sync_sema);
                return i4_ret;
            }
            ui8_curr_pos -= 32;
            pt_arg->ui8_cur_pos -= 32;
            
                if ( aui1_dent[0] != 0xE5 && aui1_dent[0] != 0x00 )
                {
            ui1_attr = FAT_GET_VAL8(aui1_dent, FAT_DENTRY_ATTR);
                
                    if ( (ui1_attr & FAT_ATTR_LONG_NAME_MASK) == (UINT8)FAT_ATTR_LONG_NAME
                            || ((ui1_attr & FAT_ATTR_VOLUME_ID) != 0) )
                    {
                        continue;/* sub part of long directory entry or volume id*/
                    }
                    if (_fat_check_dentry(aui1_dent, pt_fs_info) == FALSE)
                    {
                        continue;
                    }
                    if ( ui1_attr & FAT_ATTR_DIRECTORY)
            {
                i4_ent_off++;
            }
                    else if (!(((aui1_dent[0] <= 0x20) && (aui1_dent[0] != 0x05)) ||
                        (aui1_dent[0] == 0x22) || 
                        ((aui1_dent[0] >= 0x2A) && (aui1_dent[0] <= 0x2C)) ||
                        (aui1_dent[0] == 0x2E) ||
                        ((aui1_dent[0] >= 0x3A) && (aui1_dent[0] <= 0x3F)) ||
                        ((aui1_dent[0] >= 0x5B) && (aui1_dent[0] <= 0x5D)) ||
                        (aui1_dent[0] == 0x7C)))
            {
                i4_ent_off++;
            }
        }

        }
        /* Now, the cur_pos is on the short entry. Move it to the long entry if there is long entries */
        do
        {
            if ( pt_arg->ui8_cur_pos == 0)
                break;

            if ( IS_XENTRY_MOUNT_ROOT(pt_arg->pt_xent))
            {
                if ( pt_arg->ui8_cur_pos <= 64)
                {/* virtual directories "." and ".." */
                    pt_arg->ui8_cur_pos -= 32;
                    break;
                }
            }            
            
            t_io.ui8_offset = ui8_curr_pos - 32;
            t_io.ui4_done_bytes = 0;

                i4_ret = _fat_rw_buf(&t_io, TRUE, TRUE);
            if ( i4_ret != FMR_OK)
            {
                x_sema_delete(h_sync_sema);
                return i4_ret;
            }
            
            ui1_attr = FAT_GET_VAL8(aui1_dent, FAT_DENTRY_ATTR);            
            if ( (ui1_attr & FAT_ATTR_LONG_NAME_MASK) == FAT_ATTR_LONG_NAME)          
            {
                ui8_curr_pos -= 32;
                pt_arg->ui8_cur_pos -= 32;
            }            
        } while( (ui1_attr & FAT_ATTR_LONG_NAME_MASK) == FAT_ATTR_LONG_NAME);
    }


    x_sema_delete( h_sync_sema);
    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: fat_close
 *
 * Description: Close a opened entry.
 *
 * Inputs:  pv_data
 *              Reference to FM_XENTRY_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_close(VOID *pv_data)
{
    FM_XENTRY_T     *pt_xent;

    ASSERT(pv_data != NULL);

    pt_xent = (FM_XENTRY_T *)pv_data;
    
    ASSERT(IS_XENTRY_LOCKED(pt_xent));

    if (pt_xent->ui4_ref_cnt == 1)
    {
    return fat_sync(pv_data);
}
    else
    {
        return FMR_OK;
    }
}
/*-----------------------------------------------------------------------------
 * Name: fat_read
 *
 * Description: Read an entry.
 *
 * Inputs:  pv_data
 *              Reference to FM_RW_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_read(VOID *pv_data)
{
    FM_RW_ARG_T     *pt_arg = NULL;
    FM_RW_ARG_T     t_io_arg;
    FM_RW_ARG_T     *pt_new_arg = NULL;
    FM_XENTRY_T     *pt_xent;
    FAT_DENTRY_T    *pt_dent;
    UINT32          ui4_valid_cnt;
    INT32           i4_ret = FMR_OK;
    INT32           i4_continue_clus_num;
    INT32           i4_max_num_trans_clus;
    INT32           i4_res_cnt;
    UINT32          ui4_read_cnt = 0; 
    UINT32          ui4_pbyte_offset = 0;
    UINT32          ui4_req_cnt = 0;
    UINT32          ui4_lclus_idx;
    UINT32          ui4_pclus_idx;
    UINT32          ui4_pclus_idx_tmp;
    UINT32          ui4_clus_size;
    UINT32          ui4_sec_size;
    ROOTFS_ENTRY_T  *pt_rent;
    
    ASSERT(pv_data != NULL);

    DEBUG_TRACE();
    
    pt_arg = (FM_RW_ARG_T *)pv_data;

    pt_xent = pt_arg->pt_xent;
    ASSERT(pt_xent != NULL);

    pt_dent = FAT_GET_DENTRY(pt_xent);
    ASSERT(pt_dent != NULL);

    pt_rent = ROOTFS_GET_ENT(pt_xent->pt_mount->pt_dev_xent);
    
    if (pt_arg->ui8_offset >= pt_dent->ui4_size)
    {
        return FMR_EOF;
    }

    /* We do not read over the EOF */
    if (pt_arg->ui8_offset + pt_arg->ui4_count > pt_dent->ui4_size)
    {
        pt_arg->ui4_count = pt_dent->ui4_size - (UINT32)pt_arg->ui8_offset;
    }

    ui4_clus_size = (UINT32)(pt_arg->pt_xent->pt_mount->t_fs_info.ui8_blk_size);
    if (IS_XENTRY_BLK(pt_xent->pt_mount->pt_dev_xent))
    {
        ui4_sec_size = (UINT32)pt_xent->pt_dev->t_blk.ui4_sec_size;
    }
    else
    {
        ui4_sec_size = (UINT32)512;
    }

    if ( pt_arg->ui2_flag & FM_IO_DIRECT)
    {
        /* Prevent driver from invalid parameter of read*/
        if ((pt_arg->ui8_offset % ui4_sec_size) != 0)
        {
            ASSERT(0);
            return FMR_ARG;
        }
    
        if( pt_arg->ui2_flag & FM_IO_SYNC)
        {
            t_io_arg.e_async_cond = pt_arg->e_async_cond;
            t_io_arg.h_bin_sema = pt_arg->h_bin_sema;
            t_io_arg.pf_nfy_fct = pt_arg->pf_nfy_fct;
            t_io_arg.pt_xent = pt_arg->pt_xent;
            t_io_arg.pv_data = pt_arg->pv_data;
            t_io_arg.pv_tag = pt_arg->pv_tag;
            t_io_arg.ui1_pri = pt_arg->ui1_pri;
            t_io_arg.ui2_flag = pt_arg->ui2_flag;
            t_io_arg.ui4_done_bytes = pt_arg->ui4_done_bytes;
            t_io_arg.ui8_offset = pt_arg->ui8_offset;
        }
        else
        {   /* ASYNC R/W */

            if ((pt_arg->ui8_offset + pt_arg->ui4_count) >
                  (((pt_dent->ui4_size / ui4_clus_size) + 1) * ui4_clus_size))
            {
                /* Async read, but read too much from the tail of the file.
                   the maximum length to read is the cluster aligned file size. */

                return FMR_OUT_OF_RANGE;
            }

            if (NULL == (pt_new_arg = (FM_RW_ARG_T*) x_mem_alloc(sizeof(FM_RW_ARG_T))))
            {
                return FMR_CORE;
            }

            *pt_new_arg = *pt_arg;

            t_io_arg.pt_xent = pt_arg->pt_xent;
            t_io_arg.pv_data = pt_arg->pv_data;
            t_io_arg.pv_tag     = pt_new_arg;
            t_io_arg.ui1_pri    = pt_arg->ui1_pri;
            t_io_arg.pf_nfy_fct = _fat_async_done;
            t_io_arg.e_async_cond = pt_arg->e_async_cond;
            t_io_arg.ui2_flag = pt_arg->ui2_flag;
            t_io_arg.ui4_done_bytes = pt_arg->ui4_done_bytes;
            t_io_arg.ui8_offset = pt_arg->ui8_offset;
            t_io_arg.h_rw_req = pt_arg->h_rw_req;

            pt_new_arg->ui4_req_cnt = 0;
            pt_new_arg->ui4_req_ok_cnt = 0;
            pt_new_arg->ui4_req_fail_cnt = 0;
            pt_new_arg->ui1_all_req_sent = FALSE;
            pt_new_arg->ui1_aborted = FALSE;

        }

        pt_dent = FAT_GET_DENTRY(t_io_arg.pt_xent);
        i4_res_cnt = (INT32)pt_arg->ui4_count;
        ui4_lclus_idx = (UINT32)t_io_arg.ui8_offset / ui4_clus_size;
        ui4_pbyte_offset = (UINT32)(t_io_arg.ui8_offset % ui4_clus_size);

        /* Translate logical address to physical address*/
            /*
              *  1. Lookup the cluster cache for pclus
              *  2. Use pclus + pbyte_offset to load data in multiple of block size
              *  3. Finish loading?
              *    3.1 No? goto 1.
              *    3.2 Yes? return.
              */
        
        ui4_valid_cnt = (pt_arg->ui4_count > (ui4_clus_size - ui4_pbyte_offset)) ? 
            (ui4_clus_size-ui4_pbyte_offset) :  pt_arg->ui4_count;

        /* Need to obey the alignment constraint */
        t_io_arg.ui4_count = ((ui4_valid_cnt / ui4_sec_size) + ((ui4_valid_cnt % ui4_sec_size) != 0)) * ui4_sec_size;
        
        while (i4_res_cnt > 0)
        {            
            i4_ret = _fat_get_pclus(&ui4_pclus_idx, ui4_lclus_idx, pt_xent->pt_mount, pt_dent);

            if (i4_ret == FMR_EOC)
            {
                i4_ret = FMR_EOF;
                break;
            }
            else if (i4_ret != FMR_OK)
            {
                return FMR_INVAL;
            }  

             /* Try to merge the continuous cluster as one io request */
             i4_max_num_trans_clus = i4_res_cnt / ui4_clus_size;

             for( i4_continue_clus_num = 1; i4_continue_clus_num < i4_max_num_trans_clus; i4_continue_clus_num++)
             {
                 i4_ret = _fat_get_pclus(&ui4_pclus_idx_tmp, ui4_lclus_idx + i4_continue_clus_num, pt_xent->pt_mount, pt_dent);

                 if( ui4_pclus_idx_tmp  == ui4_pclus_idx + i4_continue_clus_num)
                 {
                     ui4_valid_cnt += ui4_clus_size;
                    t_io_arg.ui4_count  += ui4_clus_size;
                 }
                 else
                 {
                     break;
                 }
             }
             
            t_io_arg.ui8_offset = 
                ui4_pbyte_offset + FAT_CLUS_TO_DATA_ADDR(ui4_pclus_idx,(FAT_FSINFO_T*) pt_xent->pt_mount->pv_priv);

            /* for multiple partition. Add partition offset to physical offset of file { */

            ASSERT(IS_XENTRY_VIRT(pt_xent->pt_mount->pt_dev_xent));  /* be sure this is a virtual device */

            if (IS_XENTRY_BLK(pt_xent->pt_mount->pt_dev_xent))
            {
                t_io_arg.ui8_offset += ((UINT64)pt_rent->u.pt_part->ui4_start_lba) * ui4_sec_size;
            }
            else
            {
                t_io_arg.ui8_offset += 
                    ((UINT64)pt_rent->u.pt_part->ui4_start_lba) * 1;
            }

            i4_ret = fm_xentry_lock( pt_xent->pt_mount->pt_dev_xent, X_SEMA_OPTION_WAIT);

            if( i4_ret != FMR_OK)
                break;
                
            if ((!(t_io_arg.ui2_flag & FM_IO_SYNC)) && (NULL != pt_new_arg))
            { /* async */
                ui4_req_cnt++;
                if (t_io_arg.ui4_count >= (UINT32)i4_res_cnt)
                {
                    pt_new_arg->ui4_req_cnt = ui4_req_cnt;
                }

                if (i4_res_cnt - (INT32)t_io_arg.ui4_count <= 0)
                {
                    pt_new_arg->ui1_all_req_sent = TRUE;
                }
            }
            
            i4_ret = XENTRY_OP_READ(pt_xent->pt_mount->pt_dev_xent)(&t_io_arg);

            fm_xentry_unlock(pt_xent->pt_mount->pt_dev_xent);

            if( i4_ret != FMR_OK)
            {
                break;
            }

            if (t_io_arg.ui2_flag & FM_IO_SYNC)
            {
                t_io_arg.pv_data = 
                    (VOID*) ((UINT32)t_io_arg.pv_data + t_io_arg.ui4_done_bytes);
                ui4_read_cnt += t_io_arg.ui4_done_bytes;
            }
            else
            {
                t_io_arg.pv_data =
                    (VOID*) ((UINT32)t_io_arg.pv_data + t_io_arg.ui4_count);
                ui4_read_cnt += t_io_arg.ui4_count;
            }

            i4_res_cnt -= (INT32)ui4_valid_cnt;                

            ui4_pbyte_offset = t_io_arg.ui4_done_bytes = 0;
            ui4_lclus_idx += i4_continue_clus_num;

            if ( i4_res_cnt > 0)
            {
                if (i4_res_cnt >= (INT32)ui4_clus_size)
                {
                    ui4_valid_cnt = ui4_clus_size;
                    t_io_arg.ui4_count = ui4_clus_size;
                }
                else
                {
                    ui4_valid_cnt = (UINT32)i4_res_cnt;
                    t_io_arg.ui4_count = (UINT32)(((i4_res_cnt + ui4_sec_size - 1) / ui4_sec_size) * ui4_sec_size);
                }
            }

        }

        if ( ui4_read_cnt >= pt_arg->ui4_count)
        {
            pt_arg->ui4_done_bytes = pt_arg->ui4_count;
        }
        else
        {
            pt_arg->ui4_done_bytes = ui4_read_cnt;
        }
    }
    else
    {
        UINT32  ui4_org_cnt = 0;

        ui4_org_cnt = pt_arg->ui4_count;

        /* in sync indirect read, we do not read over the EOF */

        if (pt_arg->ui8_offset + pt_arg->ui4_count > pt_dent->ui4_size)
        {
            pt_arg->ui4_count = pt_dent->ui4_size - (UINT32)pt_arg->ui8_offset;
        }

        i4_ret = _fat_rw_buf(pt_arg, TRUE, FALSE);

        pt_arg->ui4_count = ui4_org_cnt;  /* restore the original read count  */
    }

    /* just in case...  */
    if ((pt_arg->ui8_offset + (UINT64)pt_arg->ui4_done_bytes) > (UINT64)pt_dent->ui4_size)
    {
        pt_arg->ui4_done_bytes = pt_dent->ui4_size - (UINT32)pt_arg->ui8_offset;
    }
        
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fat_write
 *
 * Description: Write an entry.
 *
 * Inputs:  pv_data
 *              Reference to FM_RW_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_write(VOID *pv_data)
{
    FM_RW_ARG_T     *pt_arg, t_io_arg, *pt_new_arg = NULL;
    FM_XENTRY_T     *pt_xent;
    FAT_DENTRY_T    *pt_dent;
    UINT32          ui4_new_clus;
    UINT32          ui4_old_clus;
    INT32           i4_ret = FMR_OK;
    UINT32          ui4_req_cnt = 0;
    UINT32          ui4_write_cnt = 0; 
    UINT32          ui4_valid_cnt;

    INT32           i4_res_cnt;
    UINT32          ui4_clus_size;
    UINT32          ui4_sec_size;
    UINT32          ui4_lclus_idx;
    UINT32          ui4_pbyte_offset;
    UINT32          ui4_pclus_idx;
    INT32           i4_continue_clus_num;
    INT32           i4_max_num_trans_clus;
    UINT32          ui4_pclus_idx_tmp;
    ROOTFS_ENTRY_T  *pt_rent;
    
    ASSERT(pv_data != NULL);

    DEBUG_TRACE();
    
    pt_arg = (FM_RW_ARG_T *)pv_data;

    pt_xent = pt_arg->pt_xent;
    ASSERT(pt_xent != NULL);

    pt_dent = FAT_GET_DENTRY(pt_xent);
    ASSERT(pt_dent != NULL);

    ASSERT(pt_arg->ui4_count > 0);

    if (pt_dent->ui1_attr & FAT_ATTR_READ_ONLY)
    {
        return FMR_ACCESS;
    }

    ui4_old_clus = (UINT32)FAT_SIZE_TO_CLUS(pt_dent->ui4_size, pt_xent);
    ui4_new_clus = (UINT32)FAT_SIZE_TO_CLUS(pt_arg->ui8_offset + pt_arg->ui4_count, pt_xent);

    if (ui4_new_clus > ui4_old_clus)
    {
        i4_ret = _fat_extend_fat_chain(pt_xent->pt_mount,
                                       pt_dent,
                                       ui4_new_clus - ui4_old_clus,
                                       ui4_old_clus,
                                       (pt_arg->ui8_offset > pt_dent->ui4_size) ? TRUE : FALSE);
        if (i4_ret != FMR_OK)
        {
            return i4_ret;
        }
    }
        
    pt_rent = ROOTFS_GET_ENT(pt_xent->pt_mount->pt_dev_xent);

    ui4_clus_size = (UINT32)(pt_arg->pt_xent->pt_mount->t_fs_info.ui8_blk_size);
    if (IS_XENTRY_BLK(pt_xent->pt_mount->pt_dev_xent))
    {
        ui4_sec_size = (UINT32)pt_xent->pt_dev->t_blk.ui4_sec_size;
    }
    else
    {
        ui4_sec_size = (UINT32)512;
    }

    if (pt_arg->ui2_flag & FM_IO_DIRECT)
    {
        /* Prevent driver from invalid parameter of write*/
        if ((pt_arg->ui8_offset % ui4_sec_size) != 0)
        {
            ASSERT(0);
            return FMR_ARG;
        }

        if (pt_arg->ui2_flag & FM_IO_SYNC)
        {
            t_io_arg.e_async_cond = pt_arg->e_async_cond;
            t_io_arg.h_bin_sema = pt_arg->h_bin_sema;
            t_io_arg.pf_nfy_fct = pt_arg->pf_nfy_fct;
            t_io_arg.pt_xent = pt_arg->pt_xent;
            t_io_arg.pv_data = pt_arg->pv_data;
            t_io_arg.pv_tag = pt_arg->pv_tag;
            t_io_arg.ui1_pri = pt_arg->ui1_pri;
            t_io_arg.ui2_flag = pt_arg->ui2_flag;
            t_io_arg.ui4_done_bytes = pt_arg->ui4_done_bytes;
            t_io_arg.ui8_offset = pt_arg->ui8_offset;
        }
        else
        {
            if (NULL == (pt_new_arg = (FM_RW_ARG_T*) x_mem_alloc(sizeof(FM_RW_ARG_T))))
            {
                return FMR_CORE;
            }

            *pt_new_arg = *pt_arg;

            t_io_arg.pt_xent = pt_arg->pt_xent;
            t_io_arg.pv_data = pt_arg->pv_data;
            t_io_arg.pv_tag     = pt_new_arg;
            t_io_arg.ui1_pri    = pt_arg->ui1_pri;
            t_io_arg.pf_nfy_fct = _fat_async_done;
            t_io_arg.e_async_cond = pt_arg->e_async_cond;
            t_io_arg.ui2_flag = pt_arg->ui2_flag;
            t_io_arg.ui4_done_bytes = pt_arg->ui4_done_bytes;
            t_io_arg.ui8_offset = pt_arg->ui8_offset;
            t_io_arg.h_rw_req = pt_arg->h_rw_req;

            pt_new_arg->ui4_req_cnt = 0;
            pt_new_arg->ui4_req_ok_cnt = 0;
            pt_new_arg->ui4_req_fail_cnt = 0;
            pt_new_arg->ui1_all_req_sent = FALSE;
            pt_new_arg->ui1_aborted = FALSE;
        }

        pt_dent = FAT_GET_DENTRY(t_io_arg.pt_xent);
        i4_res_cnt = (INT32)pt_arg->ui4_count;
        ui4_lclus_idx = (UINT32)t_io_arg.ui8_offset / ui4_clus_size;
        ui4_pbyte_offset = (UINT32)(t_io_arg.ui8_offset % ui4_clus_size);

        /* Translate logical address to physical address*/
            /*
              *  1. Lookup the cluster cache for pclus
              *  2. Use pclus + pbyte_offset to load data in multiple of block size
              *  3. Finish loading?
              *    3.1 No? goto 1.
              *    3.2 Yes? return.
              */
        
        ui4_valid_cnt = (pt_arg->ui4_count > (ui4_clus_size - ui4_pbyte_offset)) ? 
            (ui4_clus_size - ui4_pbyte_offset) :  pt_arg->ui4_count;

        /* Need to obey the alignment constraint */
        t_io_arg.ui4_count = ((ui4_valid_cnt / ui4_sec_size) + ((ui4_valid_cnt % ui4_sec_size) != 0)) * ui4_sec_size;
       
        while (i4_res_cnt > 0)
        {
            i4_ret = _fat_get_pclus(&ui4_pclus_idx, ui4_lclus_idx, pt_xent->pt_mount, pt_dent);

#if 0
            if (i4_ret == FMR_EOC)
            {
                break;
            }
            else if (i4_ret != FMR_OK)
            {
                return FMR_INVAL;
            }  
#else
            ASSERT((FMR_EOC != i4_ret) && (FMR_ABNORMAL_ENDED != i4_ret));

            if (i4_ret != FMR_OK)
            {
                return FMR_INVAL;
            }  
#endif
            
             /* Try to merge the continuous cluster as one io request */
            i4_max_num_trans_clus = i4_res_cnt / ui4_clus_size;

            for (i4_continue_clus_num = 1; i4_continue_clus_num < i4_max_num_trans_clus; i4_continue_clus_num++)
            {
                i4_ret = _fat_get_pclus(&ui4_pclus_idx_tmp, ui4_lclus_idx + i4_continue_clus_num, pt_xent->pt_mount, pt_dent);

                if (ui4_pclus_idx_tmp == ui4_pclus_idx + i4_continue_clus_num)
                {
                    ui4_valid_cnt += ui4_clus_size;
                    t_io_arg.ui4_count  += ui4_clus_size;
                }
                else
                {
                    break;
                }            
            }

            t_io_arg.ui8_offset = 
                ui4_pbyte_offset + FAT_CLUS_TO_DATA_ADDR(ui4_pclus_idx,(FAT_FSINFO_T*) pt_xent->pt_mount->pv_priv);

            /* for multiple partition. Add partition offset to physical offset of file { */

            ASSERT(IS_XENTRY_VIRT(pt_xent->pt_mount->pt_dev_xent));  /* be sure this is a virtual device */

            if (IS_XENTRY_BLK(pt_xent->pt_mount->pt_dev_xent))
            {
                t_io_arg.ui8_offset += ((UINT64)pt_rent->u.pt_part->ui4_start_lba) * ui4_sec_size;
            }
            else
            {
                t_io_arg.ui8_offset += ((UINT64)pt_rent->u.pt_part->ui4_start_lba) * 1;
            }

            i4_ret = fm_xentry_lock(pt_xent->pt_mount->pt_dev_xent, X_SEMA_OPTION_WAIT);

            if (i4_ret != FMR_OK)
                break;
                
            if ((!(t_io_arg.ui2_flag & FM_IO_SYNC)) && (pt_new_arg != NULL))
            { /* async */
                ui4_req_cnt++;

                if (t_io_arg.ui4_count >= (UINT32)i4_res_cnt)
                {
                    pt_new_arg->ui4_req_cnt = ui4_req_cnt;
                }

                if (i4_res_cnt - (INT32)t_io_arg.ui4_count <= 0)
                {
                    pt_new_arg->ui1_all_req_sent = TRUE;
                }
            }
            
            i4_ret = XENTRY_OP_WRITE(pt_xent->pt_mount->pt_dev_xent)(&t_io_arg);

            fm_xentry_unlock(pt_xent->pt_mount->pt_dev_xent);

            if (i4_ret != FMR_OK)
            {
                break;
            }

            if (t_io_arg.ui2_flag & FM_IO_SYNC)
            {
                t_io_arg.pv_data = 
                    (VOID*) ((UINT32)t_io_arg.pv_data + t_io_arg.ui4_done_bytes);
                ui4_write_cnt += t_io_arg.ui4_done_bytes;
            }
            else
            {
                t_io_arg.pv_data =
                    (VOID*) ((UINT32)t_io_arg.pv_data + t_io_arg.ui4_count);
                ui4_write_cnt += t_io_arg.ui4_count;
            }

            i4_res_cnt -= (INT32)ui4_valid_cnt;                

            ui4_pbyte_offset = t_io_arg.ui4_done_bytes = 0;
            ui4_lclus_idx += i4_continue_clus_num;

            if (i4_res_cnt > 0)
            {
                if (i4_res_cnt >= (INT32)ui4_clus_size)
                {
                    ui4_valid_cnt = ui4_clus_size;
                    t_io_arg.ui4_count = ui4_clus_size;
                }
                else
                {
                    ui4_valid_cnt = (UINT32)i4_res_cnt;
                    t_io_arg.ui4_count = (UINT32)(((i4_res_cnt + ui4_sec_size - 1) / ui4_sec_size) * ui4_sec_size);
                }
            }

        }

        if (ui4_write_cnt >= pt_arg->ui4_count)
        {
            pt_arg->ui4_done_bytes = pt_arg->ui4_count;
        }
        else
        {
            pt_arg->ui4_done_bytes = ui4_write_cnt;
        }      
    }
    else
    {
        i4_ret = _fat_rw_buf(pt_arg, FALSE, FALSE);
    }

    if (i4_ret == FMR_OK)
    {
        FM_RW_ARG_T t_io;
        UINT8  pui1_dent[32];
        
        /* Update the file size in FAT entry */
        if (pt_arg->ui8_offset + pt_arg->ui4_done_bytes > pt_dent->ui4_size)
        {
            pt_dent->ui4_size = (UINT32)pt_arg->ui8_offset + pt_arg->ui4_done_bytes;
            
            _fat_set_dentry(pui1_dent, pt_dent);

            t_io.pt_xent        = pt_xent->pt_prnt_xent;
            t_io.pv_data        = pui1_dent;
            t_io.ui4_count      = 32;
            t_io.ui8_offset     = (UINT64)pt_dent->ui4_name_off;
            t_io.ui4_done_bytes = 0;
            t_io.ui2_flag       = FM_IO_SYNC;
            t_io.h_bin_sema     = pt_xent->pt_mount->h_sync_sema;

            i4_ret = _fat_rw_buf(&t_io, FALSE, FALSE);
        }
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fat_delete
 *
 * Description: Delete an entry.
 *
 * Inputs:  pv_data
 *              Reference to FM_DELETE_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_delete(VOID *pv_data)
{
    FM_DELETE_ARG_T *pt_del_arg = NULL;
    FM_XENTRY_T     *pt_prnt_xent = NULL;
    FM_XENTRY_T     *pt_xent = NULL;
    FAT_DENTRY_T    *pt_dent = NULL;
    FAT_DENTRY_T    *pt_dent_tmp = NULL;
    UINT64          ui8_offset;
    INT32           i4_ret;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);
    
    pt_del_arg   = (FM_DELETE_ARG_T *)pv_data;
    pt_prnt_xent = (FM_XENTRY_T *)pt_del_arg->pt_prnt_xent;
    pt_xent      = (FM_XENTRY_T *)pt_del_arg->pt_xent;
    
    ASSERT(pt_prnt_xent != NULL);
    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_prnt_xent));
    ASSERT(IS_XENTRY_LOCKED(pt_xent));

    pt_dent = FAT_GET_DENTRY(pt_xent);

    ASSERT(pt_dent != NULL);
    
    if (IS_XENTRY_DIR(pt_xent))
    {
        pt_dent_tmp = (FAT_DENTRY_T*) x_mem_alloc(sizeof(FAT_DENTRY_T));
        if (pt_dent_tmp == NULL)
        {
            return FMR_CORE;
        }
        
        ui8_offset = 0;

        while (1)
        {
            i4_ret = _fat_read_next_dentry(pt_xent, pt_dent_tmp, &ui8_offset);
            if (i4_ret == FMR_EOF)
            {
                x_mem_free(pt_dent_tmp);
                break;
            }
            if (i4_ret != FMR_OK)
            {
                x_mem_free(pt_dent_tmp);
                return i4_ret;
            }
/*
            if (!((t_dent.s_utf8[0] == '.')
                &&  ((t_dent.s_utf8[1] == '\0')
                        || ((t_dent.s_utf8[1] == '.') && (t_dent.s_utf8[2] == '\0')))))
            {
                x_mem_free(pt_dent_tmp);
                return FMR_DIR_NOT_EMPTY;
            } 
*/
            if (!((pt_dent_tmp->s_utf8[0] == '.')
                && ((pt_dent_tmp->s_utf8[1] == '\0')
                    || ((pt_dent_tmp->s_utf8[1] == '.') && (pt_dent_tmp->s_utf8[2] == '\0')))))
            {
                x_mem_free(pt_dent_tmp);
                return FMR_DIR_NOT_EMPTY;
            }
       }
    }

    i4_ret = _fat_del_dir_entry(pt_prnt_xent, pt_dent);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }
    
    i4_ret = _fat_inval_fat_chain(pt_xent, 0);

    /* perhaps the first cluster in directory entry is wrong  */
    /* or the file is abnormal ended  */

    if ((i4_ret == FMR_EOC) || (i4_ret == FMR_ABNORMAL_ENDED))
    {
        i4_ret = FMR_OK;
    }

    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }

    x_mem_free(pt_xent->pv_fs_node);
    pt_xent->pv_fs_node = NULL;

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fat_rename
 *
 * Description: Rename an entry.
 *
 * Inputs:  pv_data
 *              Reference to FM_RENAME_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_rename(VOID *pv_data)
{
    FM_RENAME_ARG_T *pt_rename_arg;
    FM_MOUNT_T      *pt_mount;
    FAT_FSINFO_T    *pt_fat_info;
    FAT_DENTRY_T    *pt_dent;
    UINT16          ui2_name_len;
    UINT8           *s_utf16 = NULL;
    const CHAR      *ps_name;
    INT32           i4_ret;
    
    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_rename_arg = (FM_RENAME_ARG_T *)pv_data;

    ASSERT(pt_rename_arg->pt_src_prnt_xent != NULL);
    ASSERT(pt_rename_arg->pt_dst_prnt_xent != NULL);
    ASSERT(pt_rename_arg->pt_xent          != NULL);
    ASSERT(pt_rename_arg->ps_name          != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_rename_arg->pt_src_prnt_xent));
    ASSERT(IS_XENTRY_LOCKED(pt_rename_arg->pt_dst_prnt_xent));
    ASSERT(IS_XENTRY_LOCKED(pt_rename_arg->pt_xent));

    s_utf16 = (UINT8*)x_mem_alloc(512 * sizeof(UINT8)); /*  utf16 is 2-byte encoding */

    if (s_utf16 == NULL)
    {
        return FMR_CORE;
    }
    
    pt_mount = (FM_MOUNT_T *)pt_rename_arg->pt_xent->pt_mount;
    pt_dent = FAT_GET_DENTRY(pt_rename_arg->pt_xent);
    ps_name = pt_rename_arg->ps_name;

    ASSERT(pt_mount != NULL);
    ASSERT(pt_dent != NULL);

    pt_fat_info = (FAT_FSINFO_T *)pt_mount->pv_priv;

    ASSERT(pt_fat_info != NULL);

    if (pt_fat_info->e_fat_type == FAT_TYPE_12)
    {

        /* check 8.3 format */
        /* do not use s_utf16 in FAT12 case */
        if (_fat_is_dos_format((const UINT8 *)ps_name) == FALSE)
        {
        x_mem_free(s_utf16);
        return FMR_INVAL;
    }

        ui2_name_len = (UINT16)x_strlen(ps_name);
    }
    else
    {
        i4_ret = x_uc_ps_to_w2s(ps_name, (UTF16_T *)s_utf16, 256);
        ASSERT(i4_ret == UCR_OK);
        ui2_name_len = (UINT16)(x_uc_w2s_strlen((const UTF16_T *)s_utf16));
        if (ui2_name_len > pt_mount->t_fs_info.ui2_max_name_len)
        {
            x_mem_free(s_utf16);
            return FMR_NAME_TOO_LONG;
        }
    }

    i4_ret = _fat_del_dir_entry(pt_rename_arg->pt_src_prnt_xent, pt_dent);
    if (i4_ret != FMR_OK)
    {
        x_mem_free(s_utf16);
        return i4_ret;
    }

    x_strcpy((CHAR *)pt_dent->s_utf8, ps_name);
    i4_ret = _fat_add_dir_entry(pt_rename_arg->pt_dst_prnt_xent,
                                pt_dent,
                                (const UINT8 *)s_utf16,
                                ui2_name_len);

    x_mem_free(s_utf16);

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fat_make_entry
 *
 * Description: Create a new entry.
 *
 * Inputs:  pv_data
 *              Reference to FM_MKENT_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_make_entry(VOID *pv_data)
{
    FM_MKENT_ARG_T  *pt_arg;
    FM_MOUNT_T      *pt_mount;
    FAT_FSINFO_T    *pt_fat_info;
    FM_XENTRY_T     *pt_prnt_xent;
    FAT_DENTRY_T    *pt_new_dent;
    FM_RW_ARG_T t_io;
    UINT32      ui4_date_time;
    UINT16      ui2_name_len;
    const CHAR  *ps_name;
    UINT8       *s_utf16 = NULL;
    BOOL        b_zero_out;
    INT32       i4_ret;

    UINT64      ui8_time;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_arg = (FM_MKENT_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_this_xent != NULL);
    ASSERT(pt_arg->ps_name      != NULL);
    ASSERT(IS_XENTRY_DIR(pt_arg->pt_this_xent));

    s_utf16 = (UINT8*)x_mem_alloc(512 * sizeof(UINT8)); /*  utf16 is 2-byte encoding */

    if (s_utf16 == NULL)
    {
        return FMR_CORE;
    }

    pt_prnt_xent = pt_arg->pt_this_xent;
    pt_mount = pt_prnt_xent->pt_mount;
    ps_name  = pt_arg->ps_name;

    ASSERT(pt_prnt_xent != NULL);
    ASSERT(pt_mount != NULL);
    ASSERT(ps_name != NULL);

    pt_fat_info = (FAT_FSINFO_T *)pt_mount->pv_priv;
    
    ASSERT(pt_fat_info != NULL);
    
    pt_new_dent = x_mem_calloc(1, sizeof(FAT_DENTRY_T));
    if (pt_new_dent == NULL)
    {
        x_mem_free(s_utf16);
        return FMR_CORE;
    }

    /*
     *  Check file name length
     */
    if (pt_fat_info->e_fat_type == FAT_TYPE_12)
    {
        /* check 8.3 format */
        /* do not use s_utf16 in FAT12 case */
        if (_fat_is_dos_format((const UINT8 *)ps_name) == FALSE)
        {
            x_mem_free(s_utf16);
        return FMR_INVAL;
    }

        ui2_name_len = (UINT16)x_strlen(ps_name);
    }
    else
    {
        i4_ret = x_uc_ps_to_w2s(ps_name, (UTF16_T *)s_utf16, 256);
        ASSERT(i4_ret == UCR_OK);
        ui2_name_len = (UINT16)x_uc_w2s_strlen((const UTF16_T *)s_utf16);
        if (ui2_name_len > 255)
        {
            i4_ret = FMR_NAME_TOO_LONG;
            goto FAIL;
        }
    }

    ASSERT(_fat_lookup_entry(pt_prnt_xent, ps_name) == NULL);
    
    switch (pt_arg->ui4_mode & FM_MODE_TYPE_MASK)
    {
        case FM_MODE_TYPE_DIR:
            pt_new_dent->ui1_attr = FAT_ATTR_DIRECTORY;
            pt_new_dent->ui4_size = pt_fat_info->ui4_clus_size;
            b_zero_out = TRUE;
            break;
        case FM_MODE_TYPE_FILE:
            pt_new_dent->ui1_attr = FAT_ATTR_ARCHIVE;
            pt_new_dent->ui4_size = 0;
            pt_new_dent->ui4_first_clus = 0;
            pt_new_dent->ui4_last_pclus = 0;
            b_zero_out = FALSE;
            break;
        default:
            DBG_ERROR(("[FM] ERR: Invalid file type\n"));
            i4_ret = FMR_INVAL;
            goto FAIL;
    }

    /*pt_new_dent->s_name;*/
    x_strcpy((CHAR *)pt_new_dent->s_utf8, ps_name);
    if (pt_new_dent->s_utf8[0] == 0xE5)
    {
        pt_new_dent->s_utf8[0] = 0x05;
    }
    pt_new_dent->ui1_crt_time_tenth = 0;

    RTC_GetTimeDate(&ui8_time);

    ui4_date_time = _fat_conv_to_fat_time((UINT32)ui8_time);
    pt_new_dent->ui2_ctime = (UINT16)(ui4_date_time & 0xFFFF);
    pt_new_dent->ui2_cdate = (UINT16)(ui4_date_time >> 16);
    pt_new_dent->ui2_adate = pt_new_dent->ui2_cdate;
    pt_new_dent->ui2_wtime = pt_new_dent->ui2_ctime;
    pt_new_dent->ui2_wdate = pt_new_dent->ui2_cdate;

    i4_ret = _fat_extend_fat_chain(pt_mount, pt_new_dent, 1, 0, b_zero_out);
    if (i4_ret != FMR_OK)
    {
        goto FAIL;
    }
    if (pt_new_dent->ui4_first_clus == 0)
    {
        i4_ret = FMR_FILE_SYSTEM_FULL;
        goto FAIL;
    }

    i4_ret = _fat_add_dir_entry(pt_prnt_xent, pt_new_dent, s_utf16, ui2_name_len);
    if (i4_ret != FMR_OK)
    {
        goto FAIL;
    }

    pt_arg->pt_new_xent = _fat_make_xentry(pt_prnt_xent, pt_new_dent);
    if (pt_arg->pt_new_xent == NULL)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }

    /* Add . and .. */
    if (pt_new_dent->ui1_attr & FAT_ATTR_DIRECTORY)
    {
        _fat_set_dentry(s_utf16, pt_new_dent);

        x_memset(s_utf16, 0x20, 11);
        s_utf16[0] = (UINT8)'.';
        x_memcpy(&s_utf16[32], s_utf16, 32);
        s_utf16[33] = (UINT8)'.';

        FAT_SET_VAL16(&s_utf16[32], FAT_DENTRY_CLUS_HI, 0);
        FAT_SET_VAL16(&s_utf16[32], FAT_DENTRY_CLUS_LO, 0);

        t_io.pt_xent        = pt_arg->pt_new_xent;
        t_io.pv_data        = s_utf16;
        t_io.ui4_count      = 64;
        t_io.ui8_offset     = 0;
        t_io.ui4_done_bytes = 0;
        t_io.ui2_flag       = FM_IO_SYNC;
        t_io.h_bin_sema     = pt_mount->h_sync_sema;

        i4_ret = _fat_rw_buf(&t_io, FALSE, FALSE);
        if (i4_ret != FMR_OK)
        {
            goto FAIL;
        }
    }

    i4_ret = FMR_OK;

FAIL:

    if (s_utf16 != NULL)
    {
        x_mem_free(s_utf16);
    }

    if (i4_ret != FMR_OK)
    {
        if (pt_new_dent != NULL)
        {
            x_mem_free(pt_new_dent);
        }
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fat_sync
 *
 * Description: Flush pending I/O of an entry.
 *
 * Inputs:  pv_data
 *              Reference to FM_XENTRY_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Error.
 ----------------------------------------------------------------------------*/
static INT32 fat_sync(VOID *pv_data)
{
    FM_XENTRY_T     *pt_xent;
    INT32           i4_ret;

    ASSERT(pv_data != NULL);

    DEBUG_TRACE();

    pt_xent = (FM_XENTRY_T *)pv_data;
    
    ASSERT(IS_XENTRY_LOCKED(pt_xent));
    ASSERT(pt_xent->pt_mount != NULL);
    ASSERT(pt_xent->pt_mount->pt_dev_xent != NULL);

    i4_ret = fm_xentry_lock(pt_xent->pt_mount->pt_dev_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }

    i4_ret = fm_buf_flush(pt_xent->pt_mount->pt_dev_xent, TRUE, TRUE);

    fm_xentry_unlock(pt_xent->pt_mount->pt_dev_xent);

    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }

    return FMR_OK;
}

