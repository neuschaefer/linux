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

#if defined(LINUX_TURNKEY_SOLUTION)
#if defined(CC_ENABLE_VOMX) || defined(CC_ENABLE_AOMX)

#include <asm/cacheflush.h>
#include <asm/tlbflush.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include "ionodes.h"
#include "x_kmem.h"
#include "x_typedef.h"
//fix89 #include "x_major.h"
//fix89 #include "x_module.h"
//fix89 #include "x_printf.h"
#include "kmem.h"
//fix89 #include "sys_config.h"
#ifdef CONFIG_SYS_MEM_WASTE_WARNING
#define SLICED_ALLOC(n, size, last_size) sliced_alloc(n, size, last_size, __builtin_return_address(0))
#else
#define SLICED_ALLOC(n, size, last_size) sliced_alloc(n, size, last_size)
#endif


#define GFP_MEM     (GFP_KERNEL | __GFP_REPEAT)
#define GFP_KMEM    (GFP_KERNEL | __GFP_REPEAT)

#define HANDLE_TO_ITEM(X) ((PKMEM_BLOCK)(X))
#define ITEM_TO_HANDLE(X) ((HANDLE_T)(X))

#define CACHELINE_ALIGNED(X) ((X) >= 32 ? (X) : 32)


//static DEFINE_SPINLOCK(kmem_list_lock);
static DEFINE_RWLOCK(kmem_list_lock);
static LIST_HEAD(kmem_list);

typedef struct
{
    struct list_head list;
    KMEM_BLK_INFO info;
    void *addr[1];
} KMEM_BLOCK, *PKMEM_BLOCK;


static HANDLE_T sliced_alloc(int n, size_t size, size_t last_size
#ifdef CONFIG_SYS_MEM_WASTE_WARNING
    , void *caller
#endif
    )
{
    int i, m = last_size != 0 ? n + 1 : n;
    PKMEM_BLOCK item = (PKMEM_BLOCK)(kzalloc(
        sizeof(KMEM_BLOCK) - sizeof(void *) + m * sizeof(void *),
        GFP_KMEM));
    if (item == NULL)
        goto err;
    for (i = 0; i < n; i++)
    {
        void *addr = kmalloc(CACHELINE_ALIGNED(size), GFP_KMEM);
#ifdef CONFIG_SYS_MEM_WASTE_WARNING
        if (size >= PAGE_SIZE)
        {
            if ((size * 4) < (((1 << get_order(size)) << PAGE_SHIFT) * 3))
                printk("[%s] size = %d, usage less than 3/4, caller address = %p\n", __FUNCTION__, size, caller);
            if ((size + (256*1024)) < ((1 << get_order(size)) << PAGE_SHIFT))
                printk("[%s] size = %d, over 256K bytes is unused, caller address = %p\n", __FUNCTION__, size, caller);
        }
#endif
        if (addr == NULL)
            goto err;
        item->addr[i] = addr;
    }
    if (last_size != 0)
    {
        void *addr = kmalloc(PAGE_ALIGN(last_size), GFP_KMEM);
#ifdef CONFIG_SYS_MEM_WASTE_WARNING
        if (last_size >= PAGE_SIZE)
        {
            if((last_size * 4) < (((1 << get_order(last_size)) << PAGE_SHIFT) * 3))
                printk("[%s] size = %d, usage less than 3/4, caller address = %p\n", __FUNCTION__, last_size, caller);
            if((last_size + (256*1024)) < ((1 << get_order(last_size)) << PAGE_SHIFT))
                printk("[%s] size = %d, over 256K bytes is unused, caller address = %p\n", __FUNCTION__, last_size, caller);
        }
#endif
        if (addr == NULL)
            goto err;
        item->addr[n] = addr;
    }
    item->info.n = n;
    item->info.size = size;
    item->info.last_size = last_size;
    write_lock(&kmem_list_lock);
    list_add_tail(&item->list, &kmem_list);
    write_unlock(&kmem_list_lock);
    return ITEM_TO_HANDLE(item);

err:
    if (item != NULL)
    {
        for (i = 0; i < m; i++)
        {
            void *addr = item->addr[i];
            if (addr != NULL)
                kfree(addr);
            else
                break;
        }
        kfree(item);
    }
    return NULL_HANDLE;
}

static void sliced_free(HANDLE_T handle)
{
    PKMEM_BLOCK item = HANDLE_TO_ITEM(handle);
    int i, m;

    write_lock(&kmem_list_lock);
    list_del(&item->list);
    write_unlock(&kmem_list_lock);
    m = item->info.n;
    if (item->info.last_size != 0) m++;
    for (i = 0; i < m; i++)
    {
        void *addr = item->addr[i];
        kfree(addr);
    }
    kfree(item);
}

HANDLE_T x_kmem_block_alloc(SIZE_T z_size)
{
    return SLICED_ALLOC(1, z_size, 0);
}

HANDLE_T x_kmem_block_sliced_alloc(UINT32 z_size, SIZE_T z_size_element)
{
    int m = z_size / z_size_element, n = z_size % z_size_element;
    if (m != 0)
        return SLICED_ALLOC(m, z_size_element, n);
    else
        return SLICED_ALLOC(1, z_size, 0);
}

HANDLE_T x_kmem_block_sliced_aligned_alloc(UINT32 z_size, SIZE_T z_size_element, UINT32 u4Align)
{
    int m = z_size / z_size_element, n = z_size % z_size_element;
    if (m != 0)
        return SLICED_ALLOC(m, z_size_element, n >= u4Align ? n : u4Align);
    else
        return SLICED_ALLOC(1, z_size >= u4Align ? z_size : u4Align, 0);
}

VOID x_kmem_block_free(HANDLE_T handle)
{
    sliced_free(handle);
}

HANDLE_T x_kmem_block_aligned_alloc(SIZE_T z_size, UINT32 u4Align)
{
    return SLICED_ALLOC(1, z_size >= u4Align ? z_size : u4Align, 0);
}

VOID x_kmem_block_aligned_free(HANDLE_T handle)
{
    sliced_free(handle);
}

INT32 x_kmem_block_info(HANDLE_T handle, KMEM_BLK_INFO *kmem_blk_info)
{
    PKMEM_BLOCK item = HANDLE_TO_ITEM(handle);
    *kmem_blk_info = item->info;
    if (kmem_blk_info->n == 0 && kmem_blk_info->size != 0 && kmem_blk_info->last_size == 0)
        kmem_blk_info->n = 1;
    return 0;
}

HANDLE_T x_kmem_block_handle(VOID *pv_mem)
{
    PKMEM_BLOCK item;
    read_lock(&kmem_list_lock);
    list_for_each_entry(item, &kmem_list, list)
    {
        size_t size = item->info.size;
        int i, n = item->info.n ?: 1;
        char *addr;
        for (i = 0; i < n; i++)
        {
            addr = (char *)(item->addr[i]);
            if ((char *)(pv_mem) >= addr && (char *)(pv_mem) < addr + size)
            {
                read_unlock(&kmem_list_lock);
                return ITEM_TO_HANDLE(item);
            }
        }
        size = item->info.last_size;
        if (size != 0)
        {
            addr = (char *)(item->addr[n]);
            if ((char *)(pv_mem) >= addr && (char *)(pv_mem) < addr + size)
            {
                read_unlock(&kmem_list_lock);
                return ITEM_TO_HANDLE(item);
            }
        }
    }
    read_unlock(&kmem_list_lock);
    return NULL_HANDLE;
}

EXPORT_SYMBOL(x_kmem_block_alloc);
EXPORT_SYMBOL(x_kmem_block_sliced_alloc);
EXPORT_SYMBOL(x_kmem_block_sliced_aligned_alloc);
EXPORT_SYMBOL(x_kmem_block_free);
EXPORT_SYMBOL(x_kmem_block_aligned_alloc);
EXPORT_SYMBOL(x_kmem_block_aligned_free);
EXPORT_SYMBOL(x_kmem_block_info);
EXPORT_SYMBOL(x_kmem_block_handle);

static inline unsigned int __clz(unsigned int val)
{
    register unsigned int ret;
    asm ("clz\t%0, %1" : "=r" (ret) : "r" (val));
    return ret;
}

SIZE_T x_kmem_calc_slice_size(UINT32 z_size)
{
    SIZE_T i, j, k;
    for (i = 2 * 1024 * 1024; i > 256 * 1024; i >>= 1)
    {
        if (i > z_size) continue;
        j = z_size & (i - 1);
        k = 0x80000000u >> __clz(j);
        if (k < j) k <<= 1;
        if (k - j < 128 * 1024)
            break;
    }
    return i;
}

EXPORT_SYMBOL(x_kmem_calc_slice_size);

static inline bool _addr_to_phys(void *x, unsigned long *y)
{
    register unsigned long i, j;
    asm volatile (
    "mrs	%1, cpsr\n"
    "	cpsid	if\n"
    "	mcr	p15, 0, %2, c7, c8, 0\n"
    "	mrc	p15, 0, %0, c7, c4, 0\n"
    "	msr	cpsr_c, %1"
    : "=r" (i), "=r" (j) : "r" (x) : "cc");
    if (i & 1)
        return false;
    *y = (i & PAGE_MASK) | ((unsigned long)(x) & ~PAGE_MASK);
    return true;
}

static inline bool _addr_user_to_phys(void __user *x, unsigned long *y)
{
    register unsigned long i, j;
    asm volatile (
    "mrs	%1, cpsr\n"
    "	cpsid	if\n"
    "	mcr	p15, 0, %2, c7, c8, 2\n"
    "	mrc	p15, 0, %0, c7, c4, 0\n"
    "	msr	cpsr_c, %1"
    : "=r" (i), "=r" (j) : "r" (x) : "cc");
    if (i & 1)
        return false;
    *y = (i & PAGE_MASK) | ((unsigned long)(x) & ~PAGE_MASK);
    return true;
}

static inline void _sync_pmd(unsigned long addr)
{
    unsigned int index = pgd_index(addr);
    register unsigned long ttb;
    pgd_t *pgd_k, *pgd;
    pmd_t *pmd_k, *pmd;
    asm volatile ("mrc	p15, 0, %0, c2, c0, 1" : "=r" (ttb));
    pgd_k = /*init_mm.pgd*/ (pgd_t *)__va(ttb & 0xFFFFC000) + index;
    pgd = cpu_get_pgd() + index;
    pmd_k = pmd_offset(pgd_k, addr);
    pmd = pmd_offset(pgd, addr);
    if (pmd_present(*pmd_k))
        copy_pmd(pmd, pmd_k);
}

VOID *x_kmem_addr_user_to_kernel(VOID __user *pv_user)
{
    unsigned long i;
    if (!_addr_user_to_phys(pv_user, &i))
        return NULL;
    return (VOID *)__va(i);
}

VOID *x_kmem_kernel_address(HANDLE_T handle, UINT32 ui4_ofst)
{
    PKMEM_BLOCK item = HANDLE_TO_ITEM(handle);
    int m = ui4_ofst / item->info.size, n = ui4_ofst % item->info.size;
    if (m < (item->info.n ?: 1) ||
        (m == item->info.n && n < item->info.last_size))
        return (VOID *)((char *)(item->addr[m]) + n);
    return NULL;
}

HANDLE_T x_kmem_create_handle(VOID *pv_mem, SIZE_T z_size)
{
    PKMEM_BLOCK item = (PKMEM_BLOCK)(kzalloc(sizeof(KMEM_BLOCK), GFP_KMEM));
    if (item == NULL)
        return NULL_HANDLE;
    item->info.size = z_size;
    item->addr[0] = pv_mem;
    write_lock(&kmem_list_lock);
    list_add_tail(&item->list, &kmem_list);
    write_unlock(&kmem_list_lock);
    return ITEM_TO_HANDLE(item);
}

VOID x_kmem_destroy_handle(HANDLE_T handle)
{
    PKMEM_BLOCK item = HANDLE_TO_ITEM(handle);
    write_lock(&kmem_list_lock);
    list_del(&item->list);
    write_unlock(&kmem_list_lock);
    kfree(item);
}

EXPORT_SYMBOL(x_kmem_addr_user_to_kernel);
EXPORT_SYMBOL(x_kmem_kernel_address);
EXPORT_SYMBOL(x_kmem_create_handle);
EXPORT_SYMBOL(x_kmem_destroy_handle);

VOID *x_mem_aligned_alloc(SIZE_T z_size, UINT32 u4Align)
{
    if (z_size < u4Align) z_size = u4Align;
#ifdef CONFIG_SYS_MEM_WASTE_WARNING
    if (z_size >= PAGE_SIZE)
    {
        if((z_size * 4) < (((1 << get_order(z_size)) << PAGE_SHIFT) * 3))
            printk("[%s] size = %d, usage less than 3/4, caller address = %p\n", __FUNCTION__, z_size, __builtin_return_address(0));
        if((z_size + (256*1024)) < ((1 << get_order(z_size)) << PAGE_SHIFT))
            printk("[%s] size = %d, over 256K bytes is unused, caller address = %p\n", __FUNCTION__, z_size, __builtin_return_address(0));
    }
#endif
    return kmalloc(CACHELINE_ALIGNED(z_size), GFP_MEM);
}

VOID x_mem_aligned_free(VOID *pv_mem)
{
    kfree(pv_mem);
}

VOID *x_mem_valloc(UINT32 u4Size)
{
    return vmalloc(u4Size);
}

VOID x_mem_vfree(VOID *pv_mem)
{
    vfree(pv_mem);
}

EXPORT_SYMBOL(x_mem_aligned_alloc);
EXPORT_SYMBOL(x_mem_aligned_free);
EXPORT_SYMBOL(x_mem_valloc);
EXPORT_SYMBOL(x_mem_vfree);

static PKMEM_BLOCK k_curr_handle;

static long kmem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    HANDLE_T handle;
    PKMEM_BLOCK item;
    KMEM_MMAP_INFO_T info;
    size_t size;

    switch (cmd)
    {
    case IOCTL_KMEM_SET_MMAP_HANDLES:
        filp->private_data = (void *)arg;
        
        if (copy_from_user(&info, (void __user *)arg, sizeof(info)))
	    {
	        return -EFAULT;
	    }
	    
	    k_curr_handle = HANDLE_TO_ITEM(info.handles[0]);
	      
        break;

	case IOCTL_KMEM_GET_MMAP_HANDLES:
		if( copy_to_user((void __user *)arg, &k_curr_handle, sizeof(k_curr_handle)) )
		{
			return -EFAULT;
		}
        break;

    case IOCTL_KMEM_GET_SIZE:
        if (get_user(handle, (HANDLE_T __user *)(arg))) return -EFAULT;
        item = HANDLE_TO_ITEM(handle);
        size = (item->info.n ?: 1) * item->info.size + item->info.last_size;
        if (put_user(size, (size_t __user *)(arg))) return -EFAULT;
        break;

    case IOCTL_KMEM_GET_PAGE_OFFSET:
        if (get_user(handle, (HANDLE_T __user *)(arg))) return -EFAULT;
        item = HANDLE_TO_ITEM(handle);
        if (put_user((size_t)(item->addr[0]) & ~PAGE_MASK, (size_t __user *)(arg))) return -EFAULT;
        break;

    case IOCTL_KMEM_ALIGNED_FREE:
        x_kmem_block_aligned_free((HANDLE_T)(arg));
        break;

    case IOCTL_KMEM_ALIGNED_ALLOC:
        {
            KMEM_ALIGNED_ALLOC_T i;
            if (copy_from_user(&i.in, (void __user *)arg, sizeof(i.in)))
	        {
	            return -EFAULT;
	        }
            i.out.handle = x_kmem_block_aligned_alloc(i.in.z_size, i.in.u4Align);
            if (copy_to_user((void __user *)arg, &i.out, sizeof(i.out)))
    	    {
    	        return -EFAULT;
            }
        }
        break;

    case IOCTL_KMEM_SLICED_ALIGNED_ALLOC:
        {
            KMEM_SLICED_ALIGNED_ALLOC_T i;
            if (copy_from_user(&i.in, (void __user *)arg, sizeof(i.in)))
	        {
	            return -EFAULT;
	        }
            i.out.handle = x_kmem_block_sliced_aligned_alloc(i.in.z_size, i.in.z_size_element, i.in.u4Align);
            if (copy_to_user((void __user *)arg, &i.out, sizeof(i.out)))
    	    {
    	        return -EFAULT;
            }
        }
        break;
    }
    return 0;
}


static int kmem_mmap(struct file *filp, struct vm_area_struct *vma)
{
    KMEM_MMAP_INFO_T info;
    unsigned long start = 0;
    int i, ret;

    if (copy_from_user(&info, (void __user *)filp->private_data, sizeof(info)))
    {
        return -EFAULT;
    }

    for (i = 0; i < info.n; i++)
    {
        PKMEM_BLOCK item = HANDLE_TO_ITEM(info.handles[i]);
        unsigned long size = (((size_t)(item->addr[0]) & ~PAGE_MASK) + item->info.size + ~PAGE_MASK) & PAGE_MASK;
        int j, n = item->info.n ?: 1;

        for (j = 0; j < n; j++)
        {
            if (virt_addr_valid(item->addr[j]))
            {
                ret = remap_pfn_range(vma, vma->vm_start + start,
                    (__pa(item->addr[j]) >> PAGE_SHIFT) + vma->vm_pgoff,
                    size, vma->vm_page_prot);
                if (ret != 0) return -EAGAIN;
                start += size;
            }
            else
            {
                int offset;
                for (offset = 0; offset < size; offset += PAGE_SIZE)
                {
                    size_t addr = (size_t)(item->addr[j]) + offset;
                    unsigned long k;
                    if (!_addr_to_phys((void *)addr, &k))
                    {
                        _sync_pmd(addr);
                        if (!_addr_to_phys((void *)addr, &k)) return -EINVAL;
                    }
                    ret = remap_pfn_range(vma, vma->vm_start + start + offset,
                        (k >> PAGE_SHIFT) + vma->vm_pgoff,
                        PAGE_SIZE, vma->vm_page_prot);
                    if (ret != 0) return -EAGAIN;
                }
                start += size;
            }
        }
        size = (item->info.last_size + ~PAGE_MASK) & PAGE_MASK;
        if (size != 0)
        {
            if (virt_addr_valid(item->addr[j]))
            {
                ret = remap_pfn_range(vma, vma->vm_start + start,
                    (__pa(item->addr[j]) >> PAGE_SHIFT) + vma->vm_pgoff,
                    size, vma->vm_page_prot);
                if (ret != 0) return -EAGAIN;
            }
            else
            {
                int offset;
                for (offset = 0; offset < size; offset += PAGE_SIZE)
                {
                    size_t addr = (size_t)(item->addr[j]) + offset;
                    unsigned long k;
                    if (!_addr_to_phys((void *)addr, &k))
                    {
                        _sync_pmd(addr);
                        if (!_addr_to_phys((void *)addr, &k)) return -EINVAL;
                    }
                    ret = remap_pfn_range(vma, vma->vm_start + start + offset,
                        (k >> PAGE_SHIFT) + vma->vm_pgoff,
                        PAGE_SIZE, vma->vm_page_prot);
                    if (ret != 0) return -EAGAIN;
                }
            }
        }
    }
    return 0;
}


struct file_operations kmem_fops =
{
    .owner = THIS_MODULE,
    .unlocked_ioctl = kmem_ioctl,
    .mmap = kmem_mmap,
};

#if 0 //fix89 mkdev at mtk_mod.c
static struct cdev kmem_cdev;

static int __init kmem_init(void)
{
    dev_t dev = MKDEV(KMEM_MAJOR, 0);
    int ret;

    ret = register_chrdev_region(dev, 1, "kmem");
    if (ret != 0)
    {
        SYS_Printf(KERN_ERR "[KMEM] Unable to allocate region!\n");
        return ret;
    }
    cdev_init(&kmem_cdev, &kmem_fops);
    kmem_cdev.owner = THIS_MODULE;
    ret = cdev_add(&kmem_cdev, dev, 1);
    if (ret != 0)
    {
        SYS_Printf(KERN_ERR "[KMEM] Unable to add device!\n");
        return ret;
    }

    return ret;
}


static void __exit kmem_exit(void)
{
    dev_t dev = MKDEV(KMEM_MAJOR, 0);
    cdev_del(&kmem_cdev);
    unregister_chrdev_region(dev, 1);
}


DECLARE_MODULE(kmem);
#endif

#endif
#endif

