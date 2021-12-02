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
 * $RCSfile: fm_chrdev.c,v $
 * $Revision: #2 $
 * $Date: 2012/05/23 $
 * $Author: hongjun.chu $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: d27b6aee077771f93713790ce359dfcf $
 *
 * Description: 
 *         This file implements the character device access methods.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_chr_dev.h"
#include "file_mngr/fm_util.h"
#include "file_mngr/fm_ufs.h"
#include "file_mngr/fm_chrdev.h"
#include "file_mngr/fm_buf.h"
#include "file_mngr/fm_rootfs.h"
#include "file_mngr/fm_flash.h"
#include "file_mngr/linuxfs/fm_linux.h"
#include <sys/vfs.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#if !defined(ANDROID) && !defined(__UCLIBC__)
#include <aio.h>
#include <stropts.h>
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#ifndef DEBUG_FM_CHRDEV

#undef  DEBUG_TRACE
#define DEBUG_TRACE()

#endif /* DEBUG_FM_CHRDEV */

/*-----------------------------------------------------------------------------
                    data declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
  
/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
#if !defined(ANDROID) && !defined(__UCLIBC__)
void _chr_aio_callback(sigval_t sigval)
{
    INT32 i4_ret;
    FM_RW_ARG_T  *pt_rw_arg;
    struct aiocb *pt_aiocb;
    AIO_CB_TAG_T *pt_aiocb_tag;

    pt_aiocb_tag = (AIO_CB_TAG_T *)sigval.sival_ptr;
    pt_rw_arg = pt_aiocb_tag->pt_rw_arg;
    pt_aiocb  = pt_aiocb_tag->pt_aiocb;

    /* Did the request complete? */
    DBG_INFO(("_fm_aio_callback:paio_cb:%ld\n", (UINT32)pt_aiocb));
    i4_ret = aio_error( pt_aiocb );
    if (i4_ret != 0)
    {
        if (i4_ret == ECANCELED)
        {
            DBG_INFO(("_fm_aio_callback:FM_ASYNC_COND_ABORT_OK.paio_cb:%ld, %s\n",(UINT32)pt_aiocb));
            pt_rw_arg->pf_nfy_fct(pt_rw_arg->h_rw_req, pt_rw_arg->pv_tag, FM_ASYNC_COND_ABORT_OK, 0);
        }
        else
        {
            DBG_INFO(("_fm_aio_callback:FM_ASYNC_COND_FAIL.paio_cb:%ld, %s\n",(UINT32)pt_aiocb, strerror(errno)));
            pt_rw_arg->pf_nfy_fct(pt_rw_arg->h_rw_req, pt_rw_arg->pv_tag, FM_ASYNC_COND_FAIL, 0);
        }
    }
    else
    {
        /* Request completed successfully, get the return status */
        i4_ret = aio_return( pt_aiocb );
        if (pt_aiocb_tag->b_read)
        {
            DBG_INFO(("_fm_aio_callback:FM_ASYNC_COND_READ_OK,read_count:%d\n", i4_ret));
            pt_rw_arg->pf_nfy_fct(pt_rw_arg->h_rw_req, pt_rw_arg->pv_tag, FM_ASYNC_COND_READ_OK, i4_ret);
        }
        else
        {
            DBG_INFO(("_fm_aio_callback:FM_ASYNC_COND_WRITE_OK,read_count:%d\n", i4_ret));
            pt_rw_arg->pf_nfy_fct(pt_rw_arg->h_rw_req, pt_rw_arg->pv_tag, FM_ASYNC_COND_WRITE_OK, i4_ret);
        }
    }
    x_mem_free(pt_rw_arg);
    x_mem_free(pt_aiocb);
    x_mem_free(pt_aiocb_tag);
}
#endif /* ANDROID */

#define EEPROM_IOCTL_GET_SIZE 0x200

unsigned long _chrdev_get_eeprom_size(FM_DEVICE_T *pt_dev)
{
    unsigned long eeprom_size;
    if(ioctl(pt_dev->h_driver, EEPROM_IOCTL_GET_SIZE, &eeprom_size) < 0)
    {
        x_dbg_stmt("[FakeDM]Error to ioctl get eeprom size.\n");
        return 0x7fffffff;
    }
    x_dbg_stmt("EEPROM size is 0x%x.\n", eeprom_size);
    return eeprom_size;
    
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: chrdev_open
 *
 * Description: Open a character device 
 *
 * Inputs:  pt_xent
 *              The xentry of the character device.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_NO_SUCH_DEVICE
 *              Cannot open the specified driver component.
 *          FMR_DEVICE_ERROR
 *              Set or get operation error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 chrdev_open(
    FM_XENTRY_T         *pt_xent)
{
    FM_DEVICE_T         *pt_dev = NULL;
    UINT16              ui2_drv_type;
    UINT16              ui2_drv_unit;
    INT32               i4_ret;
    ROOTFS_ENTRY_T      *pt_rent = NULL;
    FM_FLASH_INFO_T      t_info;
    FLASH_SECTOR_TBL_INFO_T* pt_sec_tbl=NULL;

    ASSERT(pt_xent != NULL);
    DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));

    pt_rent = ROOTFS_GET_ENT(pt_xent);
    ui2_drv_type = (UINT16)((pt_rent->ui4_ent_id) & 0xffff);
    ui2_drv_unit = (UINT16)((pt_rent->ui4_ent_id) >> 16);
    
    i4_ret = fm_dev_open(pt_xent,
                         ui2_drv_type,
                         ui2_drv_unit,
                         &pt_dev);
    if (i4_ret == FMR_EXIST)
    {
        goto DONE;
    }
    else if (i4_ret != FMR_OK)
    {
        goto FAIL;
    }

    if (pt_dev->e_drv_type == DRVT_NAND_FLASH ||
        pt_dev->e_drv_type == DRVT_NOR_FLASH)
    {
        t_info.ui4_type = (UINT32)(FM_FLASH_INFO_TYPE_SIZE | FM_FLASH_INFO_TYPE_SEC_SIZE | FM_FLASH_INFO_TYPE_SEC_TBL);
        t_info.ppt_sec_tbl = &pt_sec_tbl;

        i4_ret = fm_flash_get_info(
            pt_dev, 
            &t_info);
        
        if (i4_ret != FMR_OK)
        {
            goto FAIL;
        }
        pt_dev->pt_sector_tbl = pt_sec_tbl;
    }
    else if(pt_dev->e_drv_type == DRVT_EEPROM)
    {
        t_info.ppt_sec_tbl = &pt_sec_tbl;/*NULL*/
        t_info.ui4_size = _chrdev_get_eeprom_size(pt_dev);
        t_info.ui4_sec_size = 512;
        pt_dev->pt_sector_tbl = pt_sec_tbl;/*NULL*/
    }
    else
    {
        t_info.ppt_sec_tbl = &pt_sec_tbl;/*NULL*/
        t_info.ui4_size = 0x7FFFFFFF;
        t_info.ui4_sec_size = 512;
        pt_dev->pt_sector_tbl = pt_sec_tbl;/*NULL*/
    }
    
    pt_dev->t_chr.ui4_dev_size = t_info.ui4_size;

    pt_dev->ui8_space = (UINT64)t_info.ui4_size;
    if (pt_dev->ui8_space == 0)
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }

    pt_dev->t_chr.ui4_max_rsync = t_info.ui4_sec_size;
    pt_dev->t_chr.ui4_max_wsync = t_info.ui4_sec_size;
    
    DBG_INFO(("[FM] Device:%s (%d bytes)\n",
               pt_xent->ps_name,
               (int)pt_dev->t_chr.ui4_dev_size));

DONE:

    pt_xent->pt_dev = pt_dev;

    return FMR_OK;

FAIL:
    
    if (pt_dev != NULL)
    {
        fm_dev_close(pt_dev);
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: chrdev_read
 *
 * Description: Read data from the character device.
 *
 * Inputs:  pv_data
 *              Reference to the read argument.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              Invaid device.
 *          FMR_EOF
 *              Cannot read over the device size.
 *          FMR_DEVICE_ERROR
 *              Get operation error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 chrdev_read(VOID *pv_data)
{
    FM_RW_ARG_T     *pt_arg = NULL;
    INT32           i4_ret = 0;
    INT32           i4_fd;
    DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));

    if (pv_data == NULL)
    {
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    /* Get corresponding inode */
    pt_arg = (FM_RW_ARG_T *)pv_data;
    if (pt_arg->pt_xent == NULL)
    {
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    /* quick return if read 0 bytes */
    if (pt_arg->ui4_count == 0)
    {
        pt_arg->ui4_done_bytes = 0;
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
        return FMR_OK;
    }
    i4_fd = pt_arg->pt_xent->pt_dev->h_driver;

    DBG_INFO(("[FM] %s: lseek %d to %llu\n ", __FUNCTION__, i4_fd, pt_arg->ui8_offset));
    lseek(i4_fd, pt_arg->ui8_offset, SEEK_SET);
    DBG_INFO(("[FM] %s: begin to read.Fd:%d, ofst:%llu,count:%lu\n ", __FUNCTION__, i4_fd, pt_arg->ui8_offset, pt_arg->ui4_count));

    if (pt_arg->pt_xent->pt_dev->e_drv_type == DRVT_NAND_FLASH ||
        pt_arg->pt_xent->pt_dev->e_drv_type == DRVT_NOR_FLASH)
    {
        return fm_flash_read(
             pt_arg->pt_xent->pt_dev, 
             (UINT32)(pt_arg->ui8_offset), 
             pt_arg->pv_data, 
             pt_arg->ui4_count, 
             &pt_arg->ui4_done_bytes);
    }

    /* read */
    i4_ret = read(i4_fd, pt_arg->pv_data, pt_arg->ui4_count);
    if (i4_ret == -1)
    {
        switch (errno)
        {
            case EINVAL:
                
                DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_EOF));
                return FMR_EOF;
            case EIO:
                
                DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_DEVICE_ERROR));
                return FMR_DEVICE_ERROR;
            default:
                
                DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
                return FMR_CORE;
        }
    }
    else
    {
        pt_arg->ui4_done_bytes = i4_ret;
        if (i4_ret == 0)
        {
            DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_EOF));
            return FMR_EOF;
        }
        else
        {
            DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
            return FMR_OK;
        }
    }
}



/*-----------------------------------------------------------------------------
 * Name: chrdev_write
 *
 * Description: Write data to the character device.
 *
 * Inputs:  pv_data
 *              Reference to the write argument.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              Invaid device.
 *          FMR_EOF
 *              Cannot read over the device size.
 *          FMR_DEVICE_ERROR
 *              Get operation error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 chrdev_write(VOID *pv_data)
{
    FM_RW_ARG_T     *pt_arg = NULL;
    INT32           i4_ret = 0;
    INT32           i4_fd;
    DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));

    if (pv_data == NULL)
    {
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    /* Get corresponding inode */
    pt_arg = (FM_RW_ARG_T *)pv_data;
    if (pt_arg->pt_xent == NULL)
    {
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    /* quick return if write 0 bytes */
    if (pt_arg->ui4_count == 0)
    {
        pt_arg->ui4_done_bytes = 0;
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
        return FMR_OK;
    }
    i4_fd = pt_arg->pt_xent->pt_dev->h_driver;

    DBG_INFO(("[FM] %s: lseek %d to %llu\n ", __FUNCTION__,i4_fd, pt_arg->ui8_offset));
    lseek(i4_fd, pt_arg->ui8_offset, SEEK_SET);
    DBG_INFO(("[FM] %s: begin to write.Fd:%d, ofst:%llu, count:%lu\n ", __FUNCTION__,i4_fd,pt_arg->ui8_offset,pt_arg->ui4_count));

    if (pt_arg->pt_xent->pt_dev->e_drv_type == DRVT_NAND_FLASH ||
        pt_arg->pt_xent->pt_dev->e_drv_type == DRVT_NOR_FLASH)
    {
        return fm_flash_write(
             pt_arg->pt_xent->pt_dev, 
             (UINT32)(pt_arg->ui8_offset), 
             pt_arg->pv_data, 
             pt_arg->ui4_count, 
             &pt_arg->ui4_done_bytes);
    }

    /* write */
    i4_ret = write(i4_fd, pt_arg->pv_data, pt_arg->ui4_count);
    if (i4_ret == -1)
    {
        switch (errno)
        {
            case EFAULT:
                
                DBG_INFO(("[FM] %s: line:%d, return %d(%s)\n", __FUNCTION__,__LINE__, FMR_EOF, strerror(errno)));
                return FMR_EOF;
            case EIO:
                
                DBG_INFO(("[FM] %s: line:%d, return %d(%s)\n", __FUNCTION__,__LINE__, FMR_DEVICE_ERROR, strerror(errno)));
                return FMR_DEVICE_ERROR;
            default:
                
                DBG_INFO(("[FM] %s: line:%d, return %d(%s)\n", __FUNCTION__,__LINE__, FMR_CORE, strerror(errno)));
                return FMR_CORE;
        }
    }
    else
    {
        pt_arg->ui4_done_bytes = i4_ret;
        if (i4_ret == 0)
        {
            DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_EOF));
            return FMR_EOF;
        }
        else
        {
            DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
            return FMR_OK;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: chrdev_aread
 *
 * Description: Async Read data from the character device.
 *
 * Inputs:  pv_data
 *              Reference to the read argument.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              Invaid device.
 *          FMR_EOF
 *              Cannot read over the device size.
 *          FMR_DEVICE_ERROR
 *              Get operation error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
#if !defined(ANDROID) && !defined(__UCLIBC__)
INT32 chrdev_aread(VOID *pv_data)
{
    FM_RW_ARG_T     *pt_arg = NULL;
    FM_RW_ARG_T     *pt_new_arg = NULL;
    struct aiocb    *pt_aiocb = NULL;
    AIO_CB_TAG_T    *pt_aiocb_tag = NULL;
    INT32           i4_fd;
    INT32           i4_ret = 0;
    DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));

    if (pv_data == NULL)
    {
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    /* Get corresponding inode */
    pt_arg = (FM_RW_ARG_T *)pv_data;
    if (pt_arg->pt_xent == NULL)
    {
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    /* quick return if read 0 bytes */
    if (pt_arg->ui4_count == 0)
    {
        pt_arg->ui4_done_bytes = 0;
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
        return FMR_OK;
    }

    i4_fd = pt_arg->pt_xent->pt_dev->h_driver;
    if (pt_arg->pt_xent->pt_dev->e_drv_type == DRVT_NAND_FLASH ||
        pt_arg->pt_xent->pt_dev->e_drv_type == DRVT_NOR_FLASH)
    {
        return FMR_ASYNC_NOT_SUPPORT;
    }

    pt_aiocb = (struct aiocb *)x_mem_alloc(sizeof(struct aiocb));
    if (pt_aiocb == NULL)
    {
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    x_memset(pt_aiocb, 0, sizeof(struct aiocb));

    pt_new_arg = (FM_RW_ARG_T *)x_mem_alloc(sizeof(FM_RW_ARG_T));
    if (pt_new_arg == NULL)
    {
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        x_mem_free(pt_aiocb);
        return FMR_CORE;
    }
    x_memcpy(pt_new_arg, pt_arg, sizeof(FM_RW_ARG_T));

    pt_aiocb_tag = (AIO_CB_TAG_T *)x_mem_alloc(sizeof(AIO_CB_TAG_T));
    if (pt_aiocb_tag == NULL)
    {
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        x_mem_free(pt_aiocb);
        x_mem_free(pt_new_arg);
        return FMR_CORE;
    }

  /*DBG_INFO(("[FM] %s: lseek %d to %d\n ", __FUNCTION__,i4_fd, pt_arg->ui8_offset));
    lseek(i4_fd, pt_arg->ui8_offset, SEEK_SET);
    DBG_INFO(("[FM] %s: begin to aread.Fd:%d, ofst:%d,count:%d\n ", __FUNCTION__,i4_fd,pt_arg->ui8_offset,pt_arg->ui4_count));*/

    /* Setup aiocb structure */
    pt_aiocb->aio_fildes = i4_fd;
    pt_aiocb->aio_buf = pt_arg->pv_data;
    pt_aiocb->aio_nbytes = pt_arg->ui4_count;
    pt_aiocb->aio_reqprio = 0;
    pt_aiocb->aio_offset = /*lseek(i4_fd, 0, SEEK_CUR);*/pt_arg->ui8_offset;
    pt_aiocb->aio_sigevent.sigev_notify = SIGEV_THREAD;
    pt_aiocb->aio_sigevent.sigev_notify_function = _chr_aio_callback;
    pt_aiocb->aio_sigevent.sigev_notify_attributes = NULL;
    pt_aiocb->aio_sigevent.sigev_value.sival_ptr = pt_aiocb_tag;

    pt_aiocb_tag->b_read = TRUE;
    pt_aiocb_tag->pt_rw_arg = pt_new_arg;
    pt_aiocb_tag->pt_aiocb = pt_aiocb;

    i4_ret = aio_read(pt_aiocb);
    if (i4_ret != 0)
    {
        DBG_INFO(("aio read:paio_cb:%ld init fail %s\n",(UINT32)pt_aiocb, strerror(errno)));
        x_mem_free(pt_aiocb);
        x_mem_free(pt_new_arg);
        x_mem_free(pt_aiocb_tag);
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    /*lseek(i4_fd, pt_arg->ui4_count, SEEK_CUR);*/
    DBG_INFO(("send aio read:paio_cb:%ld\n",(UINT32)pt_aiocb));
    DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
    return FMR_OK;
}



/*-----------------------------------------------------------------------------
 * Name: chrdev_awrite
 *
 * Description: Async Write data to the character device.
 *
 * Inputs:  pv_data
 *              Reference to the write argument.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              Invaid device.
 *          FMR_EOF
 *              Cannot read over the device size.
 *          FMR_DEVICE_ERROR
 *              Get operation error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 chrdev_awrite(VOID *pv_data)
{
    FM_RW_ARG_T     *pt_arg = NULL;
    FM_RW_ARG_T     *pt_new_arg = NULL;
    struct aiocb    *pt_aiocb = NULL;
    AIO_CB_TAG_T    *pt_aiocb_tag = NULL;
    INT32           i4_fd;
    INT32           i4_ret = 0;
    DBG_INFO(("[FM] %s: enter \n", __FUNCTION__));

    if (pv_data == NULL)
    {
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    /* Get corresponding inode */
    pt_arg = (FM_RW_ARG_T *)pv_data;
    if (pt_arg->pt_xent == NULL)
    {
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    /* quick return if read 0 bytes */
    if (pt_arg->ui4_count == 0)
    {
        pt_arg->ui4_done_bytes = 0;
        
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
        return FMR_OK;
    }

    i4_fd = pt_arg->pt_xent->pt_dev->h_driver;
    if (pt_arg->pt_xent->pt_dev->e_drv_type == DRVT_NAND_FLASH ||
        pt_arg->pt_xent->pt_dev->e_drv_type == DRVT_NOR_FLASH)
    {
        return FMR_ASYNC_NOT_SUPPORT;
    }

    pt_aiocb = (struct aiocb *)x_mem_alloc(sizeof(struct aiocb));
    if (pt_aiocb == NULL)
    {
        
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    x_memset(pt_aiocb, 0, sizeof(struct aiocb));

    pt_new_arg = (FM_RW_ARG_T *)x_mem_alloc(sizeof(FM_RW_ARG_T));
    if (pt_new_arg == NULL)
    {
        x_mem_free(pt_aiocb);
        
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    x_memcpy(pt_new_arg, pt_arg, sizeof(FM_RW_ARG_T));

    pt_aiocb_tag = (AIO_CB_TAG_T *)x_mem_alloc(sizeof(AIO_CB_TAG_T));
    if (pt_aiocb_tag == NULL)
    {
        x_mem_free(pt_aiocb);
        x_mem_free(pt_new_arg);
        
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
  /*DBG_INFO(("[FM] %s: lseek %d to %d\n ", __FUNCTION__,i4_fd, pt_arg->ui8_offset));
    lseek(i4_fd, pt_arg->ui8_offset, SEEK_SET);
    DBG_INFO(("[FM] %s: begin to awrite.Fd:%d, ofst:%d,count:%d\n ", __FUNCTION__,i4_fd,pt_arg->ui8_offset,pt_arg->ui4_count));*/

    /* Setup aiocb structure */
    pt_aiocb->aio_fildes = i4_fd;
    pt_aiocb->aio_buf = pt_arg->pv_data;
    pt_aiocb->aio_nbytes = pt_arg->ui4_count;
    pt_aiocb->aio_reqprio = 0;
    pt_aiocb->aio_offset = /*lseek(i4_fd, 0, SEEK_CUR);*/pt_arg->ui8_offset;
    pt_aiocb->aio_sigevent.sigev_notify = SIGEV_THREAD;
    pt_aiocb->aio_sigevent.sigev_notify_function = _chr_aio_callback;
    pt_aiocb->aio_sigevent.sigev_notify_attributes = NULL;
    pt_aiocb->aio_sigevent.sigev_value.sival_ptr = pt_aiocb_tag;

    pt_aiocb_tag->b_read = FALSE;
    pt_aiocb_tag->pt_rw_arg = pt_new_arg;
    pt_aiocb_tag->pt_aiocb = pt_aiocb;

    i4_ret = aio_write(pt_aiocb);
    if (i4_ret != 0)
    {
        DBG_INFO(("aio write:paio_cb:%ld init fail %s\n",(UINT32)pt_aiocb, strerror(errno)));
        x_mem_free(pt_aiocb);
        x_mem_free(pt_new_arg);
        x_mem_free(pt_aiocb_tag);
        
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    /*lseek(i4_fd, pt_arg->ui4_count, SEEK_CUR);*/
    DBG_INFO(("send aio write:paio_cb:%ld\n",(UINT32)pt_aiocb));
    
    DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
    return FMR_OK;

}
#endif /* ANDROID */


/*-----------------------------------------------------------------------------
 * Name: chrdev_lseek
 *
 * Description: Change the current position of an opened character device.
 *
 * Inputs:  pv_data
 *              The reference to FM_LSEEK_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 ----------------------------------------------------------------------------*/
INT32 chrdev_lseek(VOID *pv_data)
{
    FM_LSEEK_ARG_T  *pt_arg;
    UINT64 ui8_dev_size;
    
    DEBUG_TRACE();
    DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));

    ASSERT(pv_data != NULL);

    pt_arg = (FM_LSEEK_ARG_T *)pv_data;
    
    ASSERT(pt_arg->pt_xent != NULL);
    ASSERT(pt_arg->pt_xent->pt_dev != NULL);

    ui8_dev_size = pt_arg->pt_xent->pt_dev->ui8_space;
    
    if (pt_arg->i8_offset > 0)
    {
        if (pt_arg->ui1_whence == FM_SEEK_BGN)
        {
            if (pt_arg->i8_offset <= (INT64)ui8_dev_size)
            {
                pt_arg->ui8_cur_pos = (UINT64)pt_arg->i8_offset;
            }
            else
            {
                pt_arg->ui8_cur_pos = ui8_dev_size;
            }
        }
        else if (pt_arg->ui1_whence == FM_SEEK_CUR)
        {
            if ((pt_arg->i8_offset + pt_arg->ui8_cur_pos) <= (INT64)ui8_dev_size)
            {
                pt_arg->ui8_cur_pos += pt_arg->i8_offset;
            }
            else
            {
                pt_arg->ui8_cur_pos = ui8_dev_size;
            }
        }
        else
        {
            pt_arg->ui8_cur_pos = ui8_dev_size;
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
            if (pt_arg->i8_offset + ui8_dev_size > 0)
            {
                pt_arg->ui8_cur_pos = (UINT64)pt_arg->i8_offset + ui8_dev_size;
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
 * Name: chrdev_sync
 *
 * Description: Sync pending I/O.
 *
 * Inputs:  pv_data
 *              Reference to xentry of the character device.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Fail.
 ----------------------------------------------------------------------------*/
INT32 chrdev_sync(VOID *pv_data)
{  
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: chrdev_close
 *
 * Description: Close an opened character device.
 *
 * Inputs:  pv_data
 *              Reference to xentry of the character device.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              Invalid pt_dev
 ----------------------------------------------------------------------------*/
INT32 chrdev_close(VOID *pv_data)
{
    FM_XENTRY_T     *pt_xent;
    
    ASSERT(pv_data != NULL);
    DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));

    pt_xent = (FM_XENTRY_T *)pv_data;

    ASSERT(IS_XENTRY_LOCKED(pt_xent));
    ASSERT(pt_xent->pt_dev != NULL);

    return fm_dev_close(pt_xent->pt_dev);
}

