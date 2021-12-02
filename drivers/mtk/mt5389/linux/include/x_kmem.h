/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#ifndef _X_KMEM_H_
#define _X_KMEM_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#ifdef __KERNEL__
#include <linux/compiler.h>
#endif /* __KERNEL__ */
#include "u_handle2.h"

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __KERNEL__

extern VOID *x_kmem_alloc(SIZE_T z_size);
extern VOID *x_kmem_calloc(UINT32 ui4_num_element, SIZE_T z_size_element);
extern VOID *x_kmem_realloc(VOID *pv_mem_block, SIZE_T z_new_size);
extern VOID x_kmem_free(VOID *pv_mem_block);
extern VOID *x_kmem_aligned_alloc(SIZE_T z_size, UINT32 u4Align);
extern VOID x_kmem_aligned_free(VOID *pv_mem);

#endif /* __KERNEL__ */

extern HANDLE_T x_kmem_block_alloc(SIZE_T z_size);
extern HANDLE_T x_kmem_block_sliced_alloc(UINT32 z_size, SIZE_T z_size_element);
extern HANDLE_T x_kmem_block_sliced_aligned_alloc(UINT32 z_size, SIZE_T z_size_element, UINT32 u4Align);
extern VOID x_kmem_block_free(HANDLE_T handle);
extern HANDLE_T x_kmem_block_aligned_alloc(SIZE_T z_size, UINT32 u4Align);
extern VOID x_kmem_block_aligned_free(HANDLE_T handle);
extern VOID *x_kmem_block_map(HANDLE_T handle);
extern VOID *x_kmem_block_map_ext(void *handle);
extern VOID *x_kmem_block_multimap(int n, HANDLE_T handles[]);
extern VOID x_kmem_block_unmap(VOID *pv_mem_block);
extern VOID x_kmem_block_unmap_ext(VOID *addr, size_t size);


extern SIZE_T x_kmem_calc_slice_size(UINT32 z_size);

#define x_kmem_block_auto_sliced_alloc(z_size) \
    x_kmem_block_sliced_alloc(z_size, x_kmem_calc_slice_size(z_size))
#define x_kmem_block_auto_sliced_aligned_alloc(z_size, u4Align) \
    x_kmem_block_sliced_aligned_alloc(z_size, x_kmem_calc_slice_size(z_size), u4Align)

#ifdef __KERNEL__

typedef struct
{
    int n;
    size_t size, last_size;
} KMEM_BLK_INFO;

extern INT32 x_kmem_block_info(HANDLE_T handle, KMEM_BLK_INFO *kmem_blk_info);
extern HANDLE_T x_kmem_block_handle(VOID *pv_mem);
extern VOID *x_kmem_addr_user_to_kernel(VOID __user *pv_user);
extern VOID *x_kmem_kernel_address(HANDLE_T handle, UINT32 ui4_ofst);
extern HANDLE_T x_kmem_create_handle(VOID *pv_mem, SIZE_T z_size);
extern VOID x_kmem_destroy_handle(HANDLE_T handle);
extern VOID *x_mem_aligned_alloc(SIZE_T z_size, UINT32 u4Align);
extern VOID x_mem_aligned_free(VOID *pv_mem);
extern VOID *x_mem_valloc(UINT32 u4Size);
extern VOID x_mem_vfree(VOID *pv_mem);

#endif /* __KERNEL__ */

#ifdef __cplusplus
}
#endif

#endif /* _X_KMEM_H_ */
