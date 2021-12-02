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
 * $RCSfile: mem_linux_drv.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor: Jun Zuo $
 *
 * Description:
 *         
 *---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>

#ifndef ANDROID
#include <sys/shm.h>
#else
#include <linux/shm.h>
#endif

#include <signal.h>
#include "x_dbg_drv.h"
#include "ioctl_param.h"

#include "x_common.h"
#include <stdlib.h>
#include "os.h"
#include "u_handle.h"
#include "mem_lib.h"


VOID* os_linux_mem_alloc (VOID*     pt_part,
                       SIZE_T       z_size)
{
#ifndef MEM_WITH_OSAI_HDR
    return malloc ((size_t) z_size);
#else
    UINT8* pv_mem;
    
    z_size = (((z_size + 3) >> 2) << 2);
    z_size += sizeof(MEM_OBJ_T);
    
    pv_mem = (UINT8*) malloc ((size_t) z_size);
    ((MEM_OBJ_T*)pv_mem)->pt_bkend = (MEM_FREEOBJ_T *)(pv_mem + z_size);
    ((MEM_OBJ_T*)pv_mem)->ui1_part_index = OS_MEM_PART_NUM;
    ((MEM_OBJ_T*)pv_mem)->ui2_padding3 = 0xFFFF;
    ((MEM_OBJ_T*)pv_mem)->ui2_flags = OS_MEM_FLAG_ALLOCATED;

    return ((VOID*)(pv_mem + sizeof(MEM_OBJ_T)));
#endif
}

VOID* os_linux_mem_realloc (VOID*     pt_part,
                         VOID*        pv_ptr,
                         SIZE_T       z_size)
{
#ifndef MEM_WITH_OSAI_HDR
    return realloc ((void*)pv_ptr, (size_t) z_size);
#else
    UINT8*      pv_mem;
    MEM_OBJ_T*  pt_mem;
    
    pt_mem = ((MEM_OBJ_T*) pv_ptr) - 1;
    
    if (pt_mem->ui2_flags != OS_MEM_FLAG_ALLOCATED)
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_FREED);
    }

    z_size = (((z_size + 3) >> 2) << 2);
    z_size += sizeof(MEM_OBJ_T);

    pv_mem = (UINT8*) realloc ((void*) pt_mem, (size_t) z_size);
    ((MEM_OBJ_T*)pv_mem)->pt_bkend = (MEM_FREEOBJ_T *)(pv_mem + z_size);
    ((MEM_OBJ_T*)pv_mem)->ui1_part_index = OS_MEM_PART_NUM;
    ((MEM_OBJ_T*)pv_mem)->ui2_padding3 = 0xFFFF;
    ((MEM_OBJ_T*)pv_mem)->ui2_flags = OS_MEM_FLAG_ALLOCATED;

    return ((VOID*)(pv_mem + sizeof(MEM_OBJ_T)));
#endif
}

VOID* os_linux_mem_calloc (VOID*     pt_part,
                        UINT32       ui4_count,
                        SIZE_T       z_size)
{
#ifndef MEM_WITH_OSAI_HDR
    return calloc ((size_t) ui4_count, (size_t) z_size);
#else
    UINT8* pv_mem;
    
    z_size = (((z_size + 3) >> 2) << 2);
    z_size += sizeof(MEM_OBJ_T);
    
    pv_mem = (UINT8*) calloc ((size_t) ui4_count, (size_t) z_size);
    ((MEM_OBJ_T*)pv_mem)->pt_bkend = (MEM_FREEOBJ_T *)(pv_mem + (ui4_count * z_size));
    ((MEM_OBJ_T*)pv_mem)->ui1_part_index = OS_MEM_PART_NUM;
    ((MEM_OBJ_T*)pv_mem)->ui2_padding3 = 0xFFFF;
    ((MEM_OBJ_T*)pv_mem)->ui2_flags = OS_MEM_FLAG_ALLOCATED;

    return ((VOID*)(pv_mem + sizeof(MEM_OBJ_T)));
#endif
}

VOID os_linux_mem_free (VOID*     pt_part,
                     VOID*        pv_ptr)
{
#ifndef MEM_WITH_OSAI_HDR
    free ((void*)pv_ptr);
    return;
#else
    MEM_OBJ_T*  pt_mem;
    
    pt_mem = ((MEM_OBJ_T*) pv_ptr) - 1;
    
    if (pt_mem->ui2_flags != OS_MEM_FLAG_ALLOCATED)
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_FREED);
    }

    pt_mem->ui2_flags = OS_MEM_FLAG_FREE;
    
    free ((void*) pt_mem);
    return;
#endif
}

#define PAGE_OFFSET  0xc0000000 
static INT32    i4_fdioctrl = 0;
static INT32    i4_mem_fd = 0;

VOID* os_linux_share_mem_alloc(SIZE_T z_size)
{
    VOID*               pv_phy_addr;
    UINT32              ui4_tmp;

    ui4_tmp = z_size;
    ui4_tmp = ((ui4_tmp+4095)/4096)*4096;

    if (0 == i4_fdioctrl)
    {
        i4_fdioctrl=open("/dev/rmmgr", O_RDWR);
    }

    ioctl(i4_fdioctrl,IOCTRL_MALLOC_SHM2,&ui4_tmp);
    
    if (ui4_tmp != 0)
    {
        pv_phy_addr = (VOID *)(ui4_tmp - PAGE_OFFSET);
    }
    else
    {
        pv_phy_addr = NULL;
    }

    return pv_phy_addr;


}

VOID os_linux_share_mem_free(VOID* pv_phy_addr, SIZE_T z_size)
{
    UINT64              ui8_tmp;
    UINT32              ui4_size_4k;

    ui4_size_4k = ((((UINT32) z_size)+4095)/4096)*4096;
    ui8_tmp = ((UINT64)((UINT32)pv_phy_addr))<<32;
    ui8_tmp |= ((UINT64)ui4_size_4k);
    
    ioctl(i4_fdioctrl,IOCTRL_FREE_SHM2,&ui8_tmp);
}

VOID* os_linux_mmap_share_mem_rmmgr(VOID* pv_phy_addr, SIZE_T z_size)
{
    if (0 == i4_fdioctrl)
    {
        i4_fdioctrl = open("/dev/rmmgr", O_RDWR|O_SYNC);
    }

    return  mmap(0, 
                 z_size, 
                 PROT_READ|PROT_WRITE, 
                 MAP_SHARED, i4_fdioctrl, 
                 (UINT32)pv_phy_addr);
}


VOID* os_linux_mmap_share_mem(VOID* pv_phy_addr, SIZE_T z_size)
{
    if (0 == i4_mem_fd)
    {
        i4_mem_fd = open("/dev/mem", O_RDWR);
    }

    return  mmap(0, 
                 z_size, 
                 PROT_READ|PROT_WRITE, 
                 MAP_SHARED, i4_mem_fd, 
                 (UINT32)pv_phy_addr);
}

VOID os_linux_munmap_share_mem(VOID* pv_virt_addr, SIZE_T z_size)
{
    munmap(pv_virt_addr, z_size);
}

static UINT32 ui4_fbm_usr_address_start = 0xFFFFFFFF;
static UINT32 ui4_fbm_phy_address_start = 0;
static UINT32 ui4_fbm_usr_mem_size = 0;
static BOOL b_fbm_init = FALSE;

VOID os_linux_set_fbm_info(UINT32 ui4_virt_addr, UINT32 ui4_size)
{
    ui4_fbm_usr_address_start = ui4_virt_addr;
    ui4_fbm_usr_mem_size = ui4_size;
}

BOOL os_linux_get_fbm_info(UINT32* pui4_phy_addr, UINT32* pui4_virt_addr, UINT32* pui4_size)
{
    if (!b_fbm_init)
    {
        b_fbm_init = TRUE;
        if (0 == i4_fdioctrl)
        {
            i4_fdioctrl=open("/dev/rmmgr", O_RDWR|O_SYNC);
        }

        if (0 == i4_mem_fd)
        {
            i4_mem_fd = open("/dev/mem", O_RDWR);
        }

        ui4_fbm_phy_address_start = ioctl(i4_fdioctrl, IOCTL_GET_FBM_PHY_ADDR, 0);
        ui4_fbm_usr_mem_size = ioctl(i4_fdioctrl, IOCTL_GET_FBM_SIZE, 0);
        
        if (0xFFFFFFFF==ui4_fbm_usr_address_start)
        {
            ui4_fbm_usr_address_start = (UINT32)mmap(0, 
                                             ui4_fbm_usr_mem_size, 
                                             PROT_READ|PROT_WRITE, 
                                             MAP_SHARED, i4_mem_fd, 
                                             ui4_fbm_phy_address_start);
        }
    }

    if ((ui4_fbm_usr_address_start!=0xFFFFFFFF) && ui4_fbm_phy_address_start && ui4_fbm_usr_mem_size)
    {
        *pui4_phy_addr = ui4_fbm_phy_address_start;
        *pui4_virt_addr = ui4_fbm_usr_address_start;
        *pui4_size = ui4_fbm_usr_mem_size;
        return TRUE;
    }
    return FALSE;
}
