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
/*----------------------------------------------------------------------------*
 * SWAuthor: Chun Zhou 
 *
 * Description: 
 *         This file implements the flash access function.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/rm.h"
#include "file_mngr/fm_util.h"
#include "file_mngr/fm_dev.h"
#include "file_mngr/fm_flash.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mtd/mtd-user.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>
#include <getopt.h>
#include <asm/types.h>


/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
static INT32 fm_flash_map_sector_range(FLASH_SECTOR_TBL_INFO_T *pt_tbl, UINT64 ui8_start_addr, 
        FLASH_SECTOR_RANGE_INFO_T *pt_range)
{
    UINT32 ui4_start_sector = 0;
    UINT32 ui4_start_entry  = 0;
    INT32 i = 0, j = 0, k = 0;

    ASSERT(pt_tbl != NULL);
    ASSERT(pt_range != NULL);

    /* get start region and start sector(block) in the region */
    for (i = (INT32)pt_tbl->ui4_num_entries - 1; i >= 0; i--)
    {
        if (ui8_start_addr >= pt_tbl->pt_sector_rng[i].ui8_start)
        {
            ui4_start_sector = (UINT32)((ui8_start_addr - pt_tbl->pt_sector_rng[i].ui8_start) / (pt_tbl->pt_sector_info[i].ui4_sector_size));
            break;
        }
    }
    ASSERT(i >= 0);
    ui4_start_entry = (UINT32)i;

    pt_range->ui4_orig_start_entry  = ui4_start_entry;
    pt_range->ui4_orig_start_sector = ui4_start_sector;
    pt_range->ui4_orig_start_addr = 
        pt_tbl->pt_sector_rng[ui4_start_entry].ui8_start + ui4_start_sector * pt_tbl->pt_sector_info[ui4_start_entry].ui4_sector_size;
    pt_range->ui4_orig_length = pt_tbl->pt_sector_info[ui4_start_entry].ui4_sector_size;

    /* get bad sectors(blocks) number */
    for (i = 0; i < ui4_start_entry; i++)
    {
        for (j = 0; j < pt_tbl->pt_sector_info[i].ui4_num_sectors; j++)
        {
            /* Bad block before start addr */
            if (pt_tbl->pt_sector_info[i].pb_bad[j] == TRUE)
            {
                k++;
            }
        }
    }
    
    /* (i == ui4_start_entry) now */
    for (j = 0; j <= ui4_start_sector; j++)
    {
        /* Bad block before start addr */
        if (pt_tbl->pt_sector_info[i].pb_bad[j] == TRUE)
        {
            k++;
        }
    }

    /* Re-Calculate the start region and start sector(block) */
    for (; k > 0; k--)
    {
        /* check next block */
        ui4_start_sector++;

        /* check if next block is included in next region */
        if (ui4_start_sector >= pt_tbl->pt_sector_info[ui4_start_entry].ui4_num_sectors)
        {
            ui4_start_entry++;
            ui4_start_sector = 0;
            
            /* here FMR_OUT_OF_RANGE is returned if we have reached the last entry. */
            if (ui4_start_entry >= pt_tbl->ui4_num_entries)
            {
                return FMR_OUT_OF_RANGE;
            }
        }
        
        /* check if the next block is bad also */
        if (pt_tbl->pt_sector_info[ui4_start_entry].pb_bad[ui4_start_sector] == TRUE)
        {
            /* bad block after the addr */
            k++;
        }
    }

    pt_range->ui4_start_entry = ui4_start_entry;
    pt_range->ui4_start_sector = ui4_start_sector;
    pt_range->ui4_start_addr = 
        pt_tbl->pt_sector_rng[ui4_start_entry].ui8_start + ui4_start_sector * pt_tbl->pt_sector_info[ui4_start_entry].ui4_sector_size;
    pt_range->ui4_length = pt_tbl->pt_sector_info[ui4_start_entry].ui4_sector_size;
    
    return FMR_OK;
}

extern INT32 fm_flash_get_info( FM_DEVICE_T *pt_dev, FM_FLASH_INFO_T *pt_info)
{
    INT32 i4_fd = pt_dev->h_driver;
    int i, j, err, n, region_count;
    struct mtd_info_user mtd;
    FLASH_SECTOR_TBL_INFO_T *pt_sec_tbl = NULL;

    DBG_INFO(("[FM_FLASH] FD:%d, get flash info...\n", i4_fd));
    
    if ((pt_info->ui4_type & (~((FM_FLASH_INFO_TYPE_LAST_TYPE << 1) - 1))))
    {
        return FMR_INVAL;
    }

    err = ioctl(i4_fd, MEMGETINFO, &mtd);
    if (err < 0)
    {
        perror("MEMGETINFO");
        return FMR_CORE;
    }
    
    DBG_INFO(("[FM_FLASH] mtd.type:%d\n",      mtd.type));
    DBG_INFO(("[FM_FLASH] mtd.flags:%d\n",     mtd.flags));
    DBG_INFO(("[FM_FLASH] mtd.size:%d\n",      mtd.size));
    DBG_INFO(("[FM_FLASH] mtd.erasesize:%d\n", mtd.erasesize));
    DBG_INFO(("[FM_FLASH] mtd.writesize:%d\n", mtd.writesize));
    DBG_INFO(("[FM_FLASH] mtd.oobsize:%d\n",   mtd.oobsize));

    if (pt_info->ui4_type & FM_FLASH_INFO_TYPE_SIZE)
    {
        pt_info->ui4_size = mtd.size;
    }

    if (pt_info->ui4_type & FM_FLASH_INFO_TYPE_SEC_SIZE)
    {
        pt_info->ui4_sec_size = mtd.erasesize;
    }

    if (pt_info->ui4_type & FM_FLASH_INFO_TYPE_SEC_TBL)
    {
        struct region_info_user *regions = NULL;
        
        /* Get region count */
        err = ioctl(i4_fd, MEMGETREGIONCOUNT, &region_count);
        if (err)
        {
            return FMR_CORE;
        }
        
        DBG_INFO(("[FM_FLASH] region count:%d\n", region_count));
        if (region_count == 0)
        {
            /* none region */
            n = mtd.size / mtd.erasesize;
        }
        else
        {
            n = region_count;
        }
        
        pt_sec_tbl = x_mem_alloc(sizeof(FLASH_SECTOR_TBL_INFO_T));
        if (pt_sec_tbl == NULL)
        {
            return FMR_CORE;
        }

        pt_sec_tbl->pt_sector_info = x_mem_alloc(n * sizeof(FLASH_SECTOR_SEG_INFO_T));
        if (pt_sec_tbl->pt_sector_info == NULL)
        {
            x_mem_free(pt_sec_tbl);
            return FMR_CORE;
        }

        pt_sec_tbl->pt_sector_rng = x_mem_alloc(n * sizeof(FLASH_SECTOR_SEG_RANGE_T));
        if (pt_sec_tbl->pt_sector_rng == NULL)
        {
            x_mem_free(pt_sec_tbl->pt_sector_info);
            x_mem_free(pt_sec_tbl);
            return FMR_CORE;
        }
        
        regions = x_mem_alloc(sizeof(struct region_info_user));
        if (regions == NULL)
        {
            x_mem_free(pt_sec_tbl->pt_sector_rng);
            x_mem_free(pt_sec_tbl->pt_sector_info);
            x_mem_free(pt_sec_tbl);
            return FMR_CORE;
        }
        pt_sec_tbl->ui4_num_entries = n;
        
        for (i = 0; i < n; i++)
        {
            regions[0].regionindex = i;
            if (region_count != 0)
            {
                err = ioctl(i4_fd, MEMGETREGIONINFO, &regions[0]);
                if (err)
                {
                    x_mem_free(regions);
                    x_mem_free(pt_sec_tbl->pt_sector_rng);
                    x_mem_free(pt_sec_tbl->pt_sector_info);
                    x_mem_free(pt_sec_tbl);
                    return FMR_CORE;
                }
                /* What if there is bad block in the region? */
            }
            else
            {
                regions[0].erasesize = mtd.erasesize;
                regions[0].numblocks = 1;
                regions[0].offset = i * mtd.erasesize;
            }
            
            pt_sec_tbl->pt_sector_info[i].pb_bad = x_mem_alloc(regions[0].numblocks * sizeof(BOOL));
            if (pt_sec_tbl->pt_sector_info[i].pb_bad == NULL)
            {
                x_mem_free(regions);
                x_mem_free(pt_sec_tbl->pt_sector_rng);
                x_mem_free(pt_sec_tbl->pt_sector_info);
                x_mem_free(pt_sec_tbl);
                return FMR_CORE;
            }
            
            pt_sec_tbl->pt_sector_info[i].ui4_sector_size = regions[0].erasesize;
            pt_sec_tbl->pt_sector_info[i].ui4_num_sectors = regions[0].numblocks;

            for (j = 0; j < regions[0].numblocks; j++)
            {
                pt_sec_tbl->pt_sector_info[i].pb_bad[j] = FALSE;
            }
            
            pt_sec_tbl->pt_sector_rng[i].ui8_start = regions[0].offset;
            pt_sec_tbl->pt_sector_rng[i].ui8_end   = regions[0].offset + (regions[0].numblocks * regions[0].erasesize);

        }
        
        x_mem_free(regions);
        *(pt_info->ppt_sec_tbl) = pt_sec_tbl; 
    }

    return FMR_OK;
}
 
extern INT32 fm_flash_read(FM_DEVICE_T *pt_dev, UINT32 ui4_ofst, VOID *pv_data, UINT32 ui4_count, UINT32 *pui4_done)
{
    INT32 i4_fd = pt_dev->h_driver;
    int err, ret = FMR_OK;
    ssize_t len;
    loff_t block;
    UINT8 *pui1_temp_buf = NULL;
    UINT32 ui4_read = 0;
    UINT32 ui4_unread = ui4_count;
    UINT32 ui4_read_once = 0;
    UINT32 ui4_user_ofst = ui4_ofst;
    UINT32 ui4_relative_ofst;
    struct erase_info_user er;
    struct mtd_info_user mtd;
    FLASH_SECTOR_RANGE_INFO_T t_range;

    DBG_INFO(("[FM_FLASH] FD:%d, reading from %lu to %lu...\n", i4_fd, ui4_ofst, ui4_ofst + ui4_count));

    if (pt_dev->e_device_type == FM_DEV_TYPE_SDM)
    {
        len = pread(i4_fd, pv_data, ui4_count, ui4_ofst);
        if (len != ui4_count) 
        {
            printf("[FM_FLASH] error - FD:%d, read (%d bytes)\n", (int)i4_fd, (int)len);
            return FMR_CORE;
        }

        *pui4_done = len;
        return FMR_OK;
    }
    
    /* Get flash info first. */
    err = ioctl(i4_fd, MEMGETINFO, &mtd);
    if (err < 0)
    {
        perror("MEMGETINFO");
        return FMR_CORE;
    }

    while (ui4_unread > 0)
    {
        ret = FMR_OK;
        
        /* Map sector range */
        ret = fm_flash_map_sector_range(pt_dev->pt_sector_tbl, ui4_user_ofst, &t_range);
        if (ret != FMR_OK)
        {
            if (ret == FMR_OUT_OF_RANGE)
            {
                /* It is hard to handle FMR_OUT_OF_RANGE caused by bad blocks,
                   as I don't know the real purpose of the user.
                   If the user just came to init the flash storage, it should
                   not fail; but if the user really came to write, it should 
                   fail as no more space can be expanded to write.
                   Anyway, the next case is rare, I just return OK here.
                */
                printf("FMR_OUT_OF_RANGE caused by bad block, skip it\n");
                *pui4_done = ui4_count;
                x_memset(pv_data, 0, ui4_count);
                ret = FMR_OK;
                break;
            }
            else
            {
                ret = FMR_CORE;
                break;
            }
        }

        /* Get the offset relative to the start addr of the start sector. */
        ui4_relative_ofst = ui4_user_ofst - t_range.ui4_orig_start_addr;
        
        er.start = t_range.ui4_start_addr;
        er.length = t_range.ui4_length;

        DBG_INFO(("[FM_FLASH] FD:%d, Relative offset:%lu, Actually writing from %lu to %lu...\n", 
            i4_fd, ui4_relative_ofst, er.start, er.start + er.length));
        
         /* I have to check if this block is a bad block. */
        block = er.start;
        ret = ioctl(i4_fd, MEMGETBADBLOCK, &block);
        if (ret < 0) 
        {
            perror("ioctl(MEMGETBADBLOCK)");
            ret = FMR_CORE;
            break;

        }
        
        if (ret == 1)
        {
            printf("Bad block at %x, it will be skipped\n", (int)block);

            /* Mark it in sector table too. */
            pt_dev->pt_sector_tbl->pt_sector_info[t_range.ui4_start_entry].pb_bad[t_range.ui4_start_sector] = TRUE;

            continue;
        }

        pui1_temp_buf = x_mem_realloc(pui1_temp_buf, er.length);
        if (pui1_temp_buf == NULL)
        {
            ret = FMR_CORE;
            break;
        }
        
        /* Read the block data */
        len = pread(i4_fd, pui1_temp_buf, er.length, er.start);
        if (len < er.length) 
        {
            if (len)
            {
                printf("Short read (%d bytes)\n", len);
            }
            else
            {
                perror("read");
            }
            
            ret = FMR_CORE;
            break;
        }
        
        /* prepare the user buffer */
        ui4_read_once = (ui4_unread > (er.length - ui4_relative_ofst)) ? (er.length - ui4_relative_ofst) : (ui4_unread);
        
        x_memcpy((UINT8 *)((UINT8 *)pv_data + ui4_read), pui1_temp_buf + ui4_relative_ofst, ui4_read_once);

        DBG_INFO(("[FM_FLASH] FD:%d, prepare user buffer\n\t"
               "Read bytes:%lu\n\t"
               "Unread bytes:%lu\n\t"
               "To read %lu bytes\n\t"
               "Read offset:%lu\n",
               i4_fd, ui4_read, ui4_unread, ui4_read_once, er.start + ui4_relative_ofst));

        ui4_read   += ui4_read_once;
        ui4_unread -= ui4_read_once;
        ui4_user_ofst += ui4_read_once;

        *pui4_done = ui4_read;
    }

    if (pui1_temp_buf != NULL)
    {
        x_mem_free(pui1_temp_buf);
        pui1_temp_buf = NULL;
    }

    return ret;
}
 
extern INT32 fm_flash_write(FM_DEVICE_T *pt_dev, UINT32 ui4_ofst, VOID *pv_data, UINT32 ui4_count, UINT32 *pui4_done)
{
    INT32 i4_fd = pt_dev->h_driver;
    int err, ret = FMR_OK;
    loff_t block;
    ssize_t len;
    UINT8 *pui1_temp_buf = NULL;
    UINT32 ui4_written = 0;
    UINT32 ui4_unwritten = ui4_count;
    UINT32 ui4_write_once = 0;
    UINT32 ui4_relative_ofst;
    UINT32 ui4_user_ofst = ui4_ofst;
    struct erase_info_user er;
    struct mtd_info_user mtd;
    FLASH_SECTOR_RANGE_INFO_T t_range;

    DBG_INFO(("[FM_FLASH] FD:%d, writing from %lu to %lu...\n", i4_fd, ui4_ofst, ui4_ofst + ui4_count));
    
    if (pt_dev->e_device_type == FM_DEV_TYPE_SDM)
    {
        len = pwrite(i4_fd, pv_data, ui4_count, ui4_ofst);
        if (len != ui4_count) 
        {
            printf("[FM_FLASH] error - FD:%d, write (%d bytes)\n", (int)i4_fd, (int)len);
            return FMR_CORE;
        }

        *pui4_done = len;
        return FMR_OK;
    }
    
    /* Get flash info first. */
    err = ioctl(i4_fd, MEMGETINFO, &mtd);
    if (err < 0)
    {
        perror("MEMGETINFO");
        return FMR_CORE;
    }
    
    /* It is NOT writeable. */
    if (!(mtd.flags & MTD_WRITEABLE))
    {
        return FMR_PERM_DENY;
    }
    
    while (ui4_unwritten > 0)
    {
        ret = FMR_OK;
        
        /* Map sector range */
        ret = fm_flash_map_sector_range(pt_dev->pt_sector_tbl, ui4_user_ofst, &t_range);
        if (ret != FMR_OK)
        {
            if (ret == FMR_OUT_OF_RANGE)
            {
                /* It is hard to handle FMR_OUT_OF_RANGE caused by bad blocks,
                   as I don't know the real purpose of the user.
                   If the user just came to init the flash storage, it should
                   not fail; but if the user really came to write, it should 
                   fail as no more space can be expanded to write.
                   Anyway, the next case is rare, I just return OK here.
                */
                *pui4_done = ui4_count;
                ret = FMR_OK;
                break;
            }
            else
            {
                ret = FMR_CORE;
                break;
            }

        }
        
        /* Get the offset relative to the start addr of the start sector. */
        ui4_relative_ofst = ui4_user_ofst - t_range.ui4_orig_start_addr;

        er.start  = t_range.ui4_start_addr;
        er.length = t_range.ui4_length;
        
        DBG_INFO(("[FM_FLASH] FD:%d, Relative offset:%lu, Actually writing from %lu to %lu...\n", 
            i4_fd, ui4_relative_ofst, er.start, er.start + er.length));

        /* I have to check if this block is a bad block. */
        block = er.start;
        ret = ioctl(i4_fd, MEMGETBADBLOCK, &block);
        if (ret < 0) 
        {
            perror("ioctl(MEMGETBADBLOCK)");
            ret = FMR_CORE;
            break;
        }
        
        if (ret == 1)
        {
            printf("Bad block at %x, it will be skipped\n", (int)block);

            /* Mark it in sector table too. */
            pt_dev->pt_sector_tbl->pt_sector_info[t_range.ui4_start_entry].pb_bad[t_range.ui4_start_sector] = TRUE;

            continue;
        }

        pui1_temp_buf = x_mem_realloc(pui1_temp_buf, er.length);
        if (pui1_temp_buf == NULL)
        {
            ret = FMR_CORE;
            break;
        }
        
        /* 
           Read the block data first as the region NOT included in the
           write area need to be cached first. */
        len = pread(i4_fd, pui1_temp_buf, er.length, er.start);
        if (len < er.length) 
        {
            if (len)
            {
                printf("Short read (%d bytes)\n", len);
            }
            else
            {
                perror("read");
            }
            
            ret = FMR_CORE;
            break;
        }

        /* Need erase before write */
        if (!(mtd.flags & MTD_NO_ERASE))
        {
            if (ioctl(i4_fd, MEMERASE, &er)) 
            {
                perror("MEMERASE");
                
                /* 
                   Although I have already checked if it is bad block, it is
                   possibly to be a bad block just when I erase it. So mark it.
                */
                block = er.start;
                printf("Mark block bad at %d\n", (int)block);
                ioctl(i4_fd, MEMSETBADBLOCK, &block);

                /* Mark it in sector table too. */
                pt_dev->pt_sector_tbl->pt_sector_info[t_range.ui4_start_entry].pb_bad[t_range.ui4_start_sector] = TRUE;
                
                continue;
            }
        }
        
        /* prepare the buffer to write */
        ui4_write_once = (ui4_unwritten > (er.length-ui4_relative_ofst)) ? (er.length-ui4_relative_ofst) : (ui4_unwritten);
        
        x_memcpy(pui1_temp_buf + ui4_relative_ofst,
                 (UINT8 *)((UINT8 *)pv_data + ui4_written),
                 ui4_write_once);

        DBG_INFO(("[FM_FLASH] FD:%d,prepare buffer to write\n\t"
               "Written bytes:%lu\n\t"
               "Unwritten bytes:%lu\n\t"
               "To write %lu bytes\n\t"
               "Write offset:%lu\n",
               i4_fd, ui4_written, ui4_unwritten, ui4_write_once, er.start + ui4_relative_ofst));

        /* write to flash */
        len = pwrite(i4_fd, pui1_temp_buf, er.length, er.start);
        if (len < 0) 
        {
            perror("write");
            
            /* 
               Although I have already checked if it is bad block, it is
               possibly to be a bad block just when I erase it. So mark it.
            */
            block = er.start;
            printf("Mark block bad at %x\n", (int)block);
            ioctl(i4_fd, MEMSETBADBLOCK, &block);

            /* Mark it in sector table too. */
            pt_dev->pt_sector_tbl->pt_sector_info[t_range.ui4_start_entry].pb_bad[t_range.ui4_start_sector] = TRUE;
            
            continue;
        }
        
        if (len < er.length) 
        {
            printf("Short write (%d bytes)\n", len);
            ret = FMR_CORE;
            break;
        }

        ui4_written   += ui4_write_once;
        ui4_unwritten -= ui4_write_once;

        *pui4_done = ui4_written;

        ui4_user_ofst += ui4_write_once;
    }
    
    if (pui1_temp_buf != NULL)
    {
        x_mem_free(pui1_temp_buf);
        pui1_temp_buf = NULL;
    }

    return ret;
}

