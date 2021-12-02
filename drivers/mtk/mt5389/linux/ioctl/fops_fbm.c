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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: fops_jpg.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fops_fbm.c
 *  Module interface for FBM memory
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "ionodes.h"
#include "x_linux.h"
#include "fbm_drvif.h"
#include "x_hal_5381.h"
#include <linux/mt53xx_fbm.h>
#include <linux/version.h>
#include <linux/blkdev.h>


static int fbm_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long off, start;
	u32 len;
	dev_t dev;
	FBM_POOL_T *prFbmPool;
	unsigned int poolNum;

	dev = file->f_dentry->d_inode->i_rdev;

	off = vma->vm_pgoff << PAGE_SHIFT;

	poolNum = MINOR(dev) - IO_NODE_MINOR_FBM;

	// Special handling for total/total2
	// They are fixed to last 2 entries.
	if (poolNum == IO_NODE_NUMBER_FBM - 2)
			poolNum = FBM_POOL_TYPE_TOTAL;
	if (poolNum == IO_NODE_NUMBER_FBM - 1)
			poolNum = FBM_POOL_TYPE_TOTAL2;

	if (poolNum >= FBM_POOL_TYPE_NS)
			return -EINVAL;

	prFbmPool = FBM_GetPoolInfo((UINT8)poolNum);

	start = prFbmPool->u4Addr & PAGE_MASK;
	len = PAGE_ALIGN(prFbmPool->u4Addr + prFbmPool->u4Size - start);

	if ((vma->vm_end - vma->vm_start + off) > len)
	{
		return -EINVAL;
	}
	off += start;
	vma->vm_pgoff = off >> PAGE_SHIFT;

	/* This is an IO map - tell maydump to skip this VMA */
	vma->vm_flags |= VM_IO;

	/*
	 * Don't alter the page protection flags; we want to keep the area
	 * cached for better performance.  This does mean that we may miss
	 * some updates to the screen occasionally, but process switches
	 * should cause the caches and buffers to be flushed often enough.
	 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)	
	if (io_remap_pfn_range(vma, vma->vm_start, (off >> PAGE_SHIFT),
				vma->vm_end - vma->vm_start,
				vma->vm_page_prot))
#else
	if (io_remap_page_range(vma, vma->vm_start, off,
		vma->vm_end - vma->vm_start,
		vma->vm_page_prot))
#endif   
	{
		return -EAGAIN;
	}

	return 0;
}

static long
fbm_gen_ioctl(dev_t dev, unsigned int cmd, unsigned long arg)
{
	UINT32 start, len;
	FBM_POOL_T *prFbmPool;
	unsigned int poolNum;
	INT32 i4Ret=0;

	poolNum = MINOR(dev) - IO_NODE_MINOR_FBM;

	// Special handling for total/total2
	// They are fixed to last 2 entries.
	if (poolNum == IO_NODE_NUMBER_FBM - 2)
		poolNum = FBM_POOL_TYPE_TOTAL;
	if (poolNum == IO_NODE_NUMBER_FBM - 1)
		poolNum = FBM_POOL_TYPE_TOTAL2;

	if (poolNum >= FBM_POOL_TYPE_NS)
		return -EINVAL;

	prFbmPool = FBM_GetPoolInfo((UINT8)poolNum);

	start = prFbmPool->u4Addr & PAGE_MASK;
	len = PAGE_ALIGN(prFbmPool->u4Addr + prFbmPool->u4Size - start);

        switch (cmd) 
        {
            case FBM_CMD_GET_BASE:
                if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(UINT32)))
                {
                    i4Ret = -EFAULT;
                    goto _FBM_IOCTL_EXIT;
                }
                if (copy_to_user((void __user *)arg, &start, sizeof(UINT32)))
                {
                    i4Ret = -EFAULT;
                    goto _FBM_IOCTL_EXIT;
                }
                break;
            case FBM_CMD_GET_SIZE:
                if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(UINT32)))
                {
                    i4Ret = -EFAULT;
                    goto _FBM_IOCTL_EXIT;
                }
                if (copy_to_user((void __user *)arg, &len, sizeof(UINT32)))
                {
                    i4Ret = -EFAULT;
                    goto _FBM_IOCTL_EXIT;
                }
                break;
            default:
                i4Ret = -EFAULT;
        }

_FBM_IOCTL_EXIT:

	return i4Ret;
}

static long 
fbm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	return fbm_gen_ioctl(file->f_dentry->d_inode->i_rdev, cmd, arg);
}

struct file_operations fbm_fops = {
	.owner   = THIS_MODULE,
	.mmap  = fbm_mmap,
	//.ioctl = fbm_ioctl,
	.unlocked_ioctl = fbm_ioctl,
};


//////////////////////////////////////////////////////////////////////
///	FBM block device support.
//////////////////////////////////////////////////////////////////////

struct fbm_disk
{
	unsigned int poolNum;
	char *pBase;
	UINT32 u4Len;
	struct gendisk *disk;
};

static DEFINE_MUTEX(fbm_probe_mutex);
static DEFINE_SPINLOCK(fbm_blk_lock);
static struct fbm_disk *fbm_disks[IO_NODE_NUMBER_FBM];

static void
do_fbm_blk_request(struct request_queue *q)
{
	struct request *req;

	req = blk_fetch_request(q);
	while (req) {
		unsigned long start = blk_rq_pos(req) << 9;
		unsigned long len  = blk_rq_cur_bytes(req);
		int err = 0;
		struct gendisk *disk = req->rq_disk;
		struct fbm_disk *diskInfo = (struct fbm_disk*)disk->private_data;

		if (start + len > diskInfo->u4Len) {
			err = -EIO;
			goto done;
		}
		if (rq_data_dir(req) == READ)
			memcpy(req->buffer, diskInfo->pBase+start, len);
		else
			memcpy(diskInfo->pBase+start, req->buffer, len);
	done:
		if (!__blk_end_request_cur(req, err))
			req = blk_fetch_request(q);
	}
}

static int
fbm_blk_ioctl (struct block_device *bdev, fmode_t mode, unsigned cmd, unsigned long arg)
{
	return fbm_gen_ioctl(bdev->bd_dev, cmd, arg);
}

static const struct block_device_operations fbm_blk_fops = {
	.owner =		THIS_MODULE,
	.ioctl =		fbm_blk_ioctl,
};

/* Create disk on demand. So we won't create lots of disk for un-used devices. */
static struct kobject *
fbm_blk_probe(dev_t dev, int *part, void *data)
{
	UINT32 start, len;
	FBM_POOL_T *prFbmPool, *prTotalPool;
	unsigned int poolNum;
	struct fbm_disk *diskInfo;
	struct gendisk *disk;
	struct kobject *kobj;
	struct request_queue *queue;

	poolNum = MINOR(dev) - IO_NODE_MINOR_FBM;

	// Special handling for total/total2
	// They are fixed to last 2 entries.
	if (poolNum == IO_NODE_NUMBER_FBM - 2)
		poolNum = FBM_POOL_TYPE_TOTAL;
	if (poolNum == IO_NODE_NUMBER_FBM - 1)
		poolNum = FBM_POOL_TYPE_TOTAL2;

	if (poolNum >= FBM_POOL_TYPE_NS)
		return ERR_PTR(-EINVAL);

	mutex_lock(&fbm_probe_mutex);
	
	diskInfo = fbm_disks[poolNum];
	if (!diskInfo)
	{
		diskInfo = kmalloc(GFP_KERNEL, sizeof(struct fbm_disk));
		if (!diskInfo)
			goto out_info;
		
		disk = alloc_disk(1);
		if (!disk)
			goto out_disk;

		queue = blk_init_queue(do_fbm_blk_request, &fbm_blk_lock);
		if (!queue)
			goto out_queue;
		blk_queue_max_hw_sectors(queue, 1024);
		blk_queue_bounce_limit(queue, BLK_BOUNCE_ANY);

		prFbmPool = FBM_GetPoolInfo((UINT8)poolNum);

		start = prFbmPool->u4Addr & PAGE_MASK;
		len = PAGE_ALIGN(prFbmPool->u4Addr + prFbmPool->u4Size - start);

		// Map FBM area if not already mapped.
		// Assume FBM in channel 0 is already mapped, and all others are not.
		prTotalPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TOTAL);
		if (start >= prTotalPool->u4Addr && start < (prTotalPool->u4Addr + prTotalPool->u4Size))
		{
			diskInfo->pBase = (char*)VIRTUAL(start);
		}
		else
		{
			diskInfo->pBase = ioremap_cached(start, len);
		}
		if (!diskInfo->pBase)
			goto out_ioremap;

		disk->major = IO_NODE_MAJOR_FBM;
		disk->first_minor = MINOR(dev);
		disk->fops = &fbm_blk_fops;
		disk->private_data = diskInfo;
		snprintf(disk->disk_name, sizeof(disk->disk_name), "fbmblk%d", MINOR(dev));
		disk->queue = queue;
		set_capacity(disk, len / 512);
		add_disk(disk);

		diskInfo->poolNum = poolNum;
		diskInfo->u4Len = len;
		diskInfo->disk = disk;
		fbm_disks[poolNum] = diskInfo;
	}

	*part = 0;
	kobj = diskInfo ? get_disk(diskInfo->disk) : ERR_PTR(-ENOMEM);
	mutex_unlock(&fbm_probe_mutex);

	return kobj;

out_ioremap:
	blk_cleanup_queue(queue);
out_queue:
	put_disk(disk);
out_disk:
	kfree(diskInfo);
out_info:
	mutex_unlock(&fbm_probe_mutex);
	return ERR_PTR(-ENOMEM);
}

// Register FBM block device.
void __init fbm_blkdev_init(void)
{
	if (register_blkdev(IO_NODE_MAJOR_FBM, "fbm"))
		return;

	blk_register_region(MKDEV(IO_NODE_MAJOR_FBM, IO_NODE_MINOR_FBM), IO_NODE_NUMBER_FBM,
		THIS_MODULE, fbm_blk_probe, NULL, NULL);
}

