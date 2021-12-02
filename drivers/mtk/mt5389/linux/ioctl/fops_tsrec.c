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
 * $RCSfile: $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fops_tsrec.c
 *  Module interface of TS record driver
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "ionodes.h"
#include "x_linux.h"
#include "fbm_drvif.h"
#include <linux/version.h>

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static int tsrec_mmap(struct file *, struct vm_area_struct *);

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** _TsRecOnMMAPAddr
 *
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------

UINT32 _TsRecOnMMAPAddr(void)
{
	FBM_POOL_T *prFbmPool;

    // Fixme, temp use PIP scaler
	//prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX_REC_TS);
	//prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_SCPOS_PIP);
	//prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_NR_1);
	prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_PVR);

	return PAGE_ALIGN(prFbmPool->u4Addr);	//jpg driver internal used

}

//-----------------------------------------------------------------------------
/** _TsRecOnMMAPSize
 *
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
UINT32 _TsRecOnMMAPSize(void)
{
	FBM_POOL_T *prFbmPool;

	//prepare input buffer from FBM, demo purpose
	//prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX_REC_TS);
	//prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_SCPOS_PIP);
	//prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_NR_1);
	prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_PVR);

	return (prFbmPool->u4Size & PAGE_MASK);
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

static int tsrec_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long off, start;
	u32 len;

	off = vma->vm_pgoff << PAGE_SHIFT;

	start = (unsigned long)_TsRecOnMMAPAddr(); //info->fix.smem_start;
	//info->fix.smem_len;
	len = PAGE_ALIGN(start & ~PAGE_MASK) + _TsRecOnMMAPSize();
	start &= PAGE_MASK;

	if ((vma->vm_end - vma->vm_start + off) > len)
	{
		return -EINVAL;
	}
	off += start;
	vma->vm_pgoff = off >> PAGE_SHIFT;

	/* This is an IO map - tell maydump to skip this VMA */
	vma->vm_flags |= VM_IO;

	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

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

struct file_operations tsrec_fops = {
	.owner   = THIS_MODULE,
	.mmap  = tsrec_mmap,
};

