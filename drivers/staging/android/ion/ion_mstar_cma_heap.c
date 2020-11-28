/*
 * drivers/gpu/ion/ion_cma_heap.c
 *
 * Copyright (C) Linaro 2012
 * Author: <benjamin.gaignard@linaro.org> for ST-Ericsson.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/device.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/dma-mapping.h>
#include <linux/compaction.h>

#include <linux/swap.h>
#include <asm/dma-contiguous.h>
//#include <linux/dma-contiguous.h>
#include <linux/migrate.h>
#include <linux/freezer.h>
#include <linux/bug.h>
#include <linux/kthread.h>
#include <linux/page-isolation.h>
#include "../../../mm/internal.h"

#include "ion.h"
#include "ion_priv.h"
#include "ion_mstar_cma_heap.h"

struct mstar_cma{
	struct cma* 	cma;
	unsigned long   zero_bit_count;
	unsigned long  *pcache_low_threshold;			//point to cma_miuX_threshold[0]
	unsigned long  *pcache_high_threshold;		//point to cma_miuX_threshold[1]
	unsigned long  cache_batch;					//batch count to increase/decrease 

	struct mutex contig_rtylock;
	wait_queue_head_t cma_swap_wait;//page swap worker wait queue
	struct task_struct *swap_worker;
	
	spinlock_t      free_list_spinlock;//spin lock to protect free page list
	int  free_page_count;
	struct list_head      free_list;
	struct zone	*	zone;

	//these are for debug usage
	int fail_alloc_count;
	int total_alloc_count;
};

struct ion_mstar_cma_heap {
	struct ion_heap heap;
	struct device *dev;
	struct mstar_cma *mstar_cma;
};

struct ion_mstar_cma_page_info {
	struct page *page;
	unsigned long size;
	struct list_head list;
};

#define ION_MSTAR_CMA_ALLOCATE_FAILED -1
#define MSTAR_CMA_HEAP_DEBUG 0
#define CMA_CONTIG_RTYCNT 1

#define CMA_BUG_ON(condition, format...) ({						\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		__WARN_printf(format);					\
	if (unlikely(__ret_warn_on))					\
		BUG();					\
})

#define to_mstar_cma_heap(x) container_of(x, struct ion_mstar_cma_heap, heap)
#define to_mstar_cma(x)	container_of(x, struct mstar_cma, cma)


unsigned long cma_miu_threshold[3] = {256, 1024 , 64};


static unsigned long pfn_max_align_down(unsigned long pfn)
{
	return pfn & ~(max_t(unsigned long, MAX_ORDER_NR_PAGES,
			     pageblock_nr_pages) - 1);
}

static unsigned long pfn_max_align_up(unsigned long pfn)
{
	return ALIGN(pfn, max_t(unsigned long, MAX_ORDER_NR_PAGES,
				pageblock_nr_pages));
}

static unsigned long  get_free_bit_count(struct cma *cma, unsigned long start, unsigned long len)
{
    unsigned long count = 0;
	unsigned long offset_zero;
	unsigned long end = start+len;


    if(len==0 || start >= cma->count)
		return 0;
	end = start+len;
	end = min(end, cma->count);
	
	offset_zero = start;
	
    while(offset_zero < cma->count)
    {
        unsigned offset_bit;
	    offset_zero = find_next_zero_bit(cma->bitmap, end,
						 offset_zero);
	    if(offset_zero>= end)
	  	   break;
	    offset_bit = find_next_bit(cma->bitmap, end, offset_zero+1);
		count += offset_bit-offset_zero;
		offset_zero = offset_bit+1;
     }

   return count;
}

static int cma_cache_pages(struct compact_control *cc,unsigned long start, unsigned long end,
	        struct mstar_cma *mstar_cma, int target_count)
{
  int page_count = 0;
  unsigned long last_free_high;
  struct page *page;
  
  struct list_head freelist;
  INIT_LIST_HEAD(&freelist);
  lru_add_drain_all();
  drain_all_pages();
  isolate_descrete_freepages(cc,mstar_cma->zone, start, end, target_count, &freelist,&last_free_high);
  if(!list_empty(&freelist))
  {
        list_for_each_entry(page, &freelist, lru) 
	    {
	   	  unsigned long pfn_tmp;
	   	  pfn_tmp = page_to_pfn(page);
		  CMA_BUG_ON(pfn_tmp < mstar_cma->cma->base_pfn || pfn_tmp >= mstar_cma->cma->base_pfn + mstar_cma->cma->count,
		  	"pfn=%lx,cma[%lx,%lx] \n",pfn_tmp,mstar_cma->cma->base_pfn,mstar_cma->cma->count);
		  if(test_and_set_bit(pfn_tmp-mstar_cma->cma->base_pfn, mstar_cma->cma->bitmap))
		  {
		      BUG();//this bit should be zero before
		  }
#ifdef CONFIG_MP_CMA_PATCH_CMA_AGGRESSIVE_ALLOC
		  adjust_managed_cma_page_count(mstar_cma->zone, -1); 
#endif
		  mstar_cma->zero_bit_count--;
		  page_count++;
        }
		
		spin_lock(&mstar_cma->free_list_spinlock);
	    list_splice(&freelist, &mstar_cma->free_list);
		mstar_cma->free_page_count += page_count;
		spin_unlock(&mstar_cma->free_list_spinlock);
	
  }
  return page_count;
}

int cma_increase_pool_freelist_threshold(struct mstar_cma *mstar_cma)
{
	unsigned long pfn, last_pfn, end, nr_reclaimed;
	int ret;
	int total_page_count = 0;
	int tries = 0;

	unsigned long isolated_beg=(unsigned long)-1, isolated_end=(unsigned long)-1;
	unsigned long page_count;
	
	struct compact_control cc = {
		.nr_freepages = 0,
		.nr_migratepages = 0,
		.order = -1,
		.migratetype = MIGRATE_CMA,
		.zone = mstar_cma->zone,
		.sync = true,
		.ignore_skip_hint = true,
	};
	INIT_LIST_HEAD(&cc.freepages);
	INIT_LIST_HEAD(&cc.migratepages);

	mutex_lock(&mstar_cma->cma->lock);
 	if (mutex_is_locked(&mstar_cma->contig_rtylock))
 	{
 		//printk("Stop cma swap increase!\n");
 		mutex_unlock(&mstar_cma->cma->lock);
		return 0;
 	}
	
	migrate_prep();
	pfn = mstar_cma->cma->base_pfn;
	end = mstar_cma->cma->base_pfn+mstar_cma->cma->count;
	page_count = 0;
	last_pfn = (unsigned long)-1;
	
	while ( pfn < end|| !list_empty(&cc.migratepages)) {
		if (fatal_signal_pending(current)) {
			ret = -EINTR;
			break;
		}
		
		if (list_empty(&cc.migratepages)) {
			cc.nr_migratepages = 0;
							
			pfn = mstar_cma->cma->base_pfn+
				 find_next_zero_bit(mstar_cma->cma->bitmap, mstar_cma->cma->count, pfn-mstar_cma->cma->base_pfn);

next_block:
			if(pfn >= end)
				break;
			
			last_pfn = pfn;

			//no block isolated, isolated one
			if(isolated_beg == (unsigned long)-1){
				isolated_beg = pfn_max_align_down(pfn);
				isolated_end = pfn_max_align_up(pfn+1);
				ret = start_isolate_page_range(isolated_beg,
				                        isolated_end, MIGRATE_CMA,false);
#if MSTAR_CMA_HEAP_DEBUG == 1
				int g_flag = 0;
				if(mstar_cma->free_page_count == 0 && get_free_bit_count(mstar_cma->cma,isolated_beg-mstar_cma->cma->base_pfn,1024) > 512)
					g_flag = 1;
				else
					g_flag = 0;
#endif				
				
				if(ret)
				{ 
				    //isolate the block failed
					printk(KERN_ERR" isolate failed [%lx,%lx] \n",isolated_beg,isolated_end);
				    isolated_beg=(unsigned long)-1;
					pfn = isolated_end + 1;
					goto next_block;
				}
			}

			//next COMPACT_CLUSTER_MAX pages till isolated_end
			pfn = isolate_migratepages_range(cc.zone, &cc,
							 pfn, isolated_end,true);
			if (!pfn) {
				printk("!!!!!!!!!!!!!!!!!!!!! pfn = 0 \n");
				ret = -EINTR;
				break;
			}
#if MSTAR_CMA_HEAP_DEBUG == 1			
			if(g_flag == 1)
				printk("[%x,%x],%x ~ %x, cc.nr_migratepages=%d \n",isolated_beg, isolated_end,last_pfn,pfn,cc.nr_migratepages);
#endif
			tries = 0;
		} else if (++tries == 5) {
		     putback_lru_pages(&cc.migratepages);
			 CMA_BUG_ON(isolated_beg == (unsigned long)-1, "isolated_beg should not be -1 \n");

			 undo_isolate_page_range(isolated_beg, isolated_end, MIGRATE_CMA);
#if MSTAR_CMA_HEAP_DEBUG == 1				 
			 printk("migrate fail!!! [%x,%x],%x ~ %x,migrate pages=%d,zero_bit_count=%d,total_free_count=%d,block_free=%d \n",
			 	isolated_beg, isolated_end,last_pfn,pfn,cc.nr_migratepages,mstar_cma->zero_bit_count,
			 	mstar_cma->free_page_count,get_free_bit_count(mstar_cma->cma,isolated_beg-mstar_cma->cma->base_pfn,1024));
#endif
			 isolated_beg = -1;
			 continue;
		}

		if(!list_empty(&cc.migratepages)){
			nr_reclaimed = reclaim_clean_pages_from_list(cc.zone,
								&cc.migratepages);
			cc.nr_migratepages -= nr_reclaimed;

			ret = migrate_pages(&cc.migratepages, alloc_migrate_target,
						0, MIGRATE_SYNC, MR_CMA);
		}
		
		if(pfn >= isolated_end){	//end of a block
			int ret;
			//even though we get pages from [last_pfn, pfn), actually we get from the whole block
			total_page_count += ret = cma_cache_pages(&cc,last_pfn, pfn, mstar_cma, mstar_cma->cache_batch);
#if MSTAR_CMA_HEAP_DEBUG == 1			
			if(g_flag == 1)			
				printk("#######start %x,end %x, [%x,%x] getpages=%d,total_free_bit=%d ,total_free_page_count = %d,zerobit=%d, block_free=%d \n",last_pfn, pfn,isolated_beg, isolated_end,
					total_page_count,get_free_bit_count(mstar_cma->cma,0,mstar_cma->cma->count),
					mstar_cma->free_page_count,
					mstar_cma->zero_bit_count,
					get_free_bit_count(mstar_cma->cma,isolated_beg-mstar_cma->cma->base_pfn,1024));
#endif			
			if(total_page_count > mstar_cma->cache_batch)
				break;

			if(list_empty(&cc.migratepages)){	//empty, just goto next block
				//un-isolate the block
				if(isolated_beg != (unsigned long)-1)
					undo_isolate_page_range(isolated_beg, isolated_end, MIGRATE_CMA);
				isolated_beg = -1;			//isolated == -1 means no block isolated
				goto next_block;
			}
		}

	}

	 //clear migrate list
	 if(!list_empty(&cc.migratepages))
	 {
	     putback_lru_pages(&cc.migratepages);
	 }
	 if(isolated_beg !=(unsigned long)-1)
	 	  undo_isolate_page_range(isolated_beg, isolated_end, MIGRATE_CMA);
	
	mutex_unlock(&mstar_cma->cma->lock);
	
	if(total_page_count && waitqueue_active(&mstar_cma->cma_swap_wait))
		  wake_up_interruptible(&mstar_cma->cma_swap_wait);

	return total_page_count;
}

static int cma_swap_worker_thread(void *p)
{
	struct mstar_cma *mstar_cma = (struct mstar_cma *)p;

	set_freezable();

	while(1)
	{
	 DEFINE_WAIT(wait);

	 try_to_freeze();

	 mutex_lock(&mstar_cma->cma->lock);
 	if (mutex_is_locked(&mstar_cma->contig_rtylock))
 	{
 		//printk("Stop cma swap decrease!\n");
 		mutex_unlock(&mstar_cma->cma->lock);
		goto SLEEP;
 	}
	
	 if(mstar_cma->free_page_count>*mstar_cma->pcache_high_threshold)
	 {
	    unsigned long pfn;
		struct page *page;
		
	   //while(mstar_cma->free_page_count > *mstar_cma->pcache_high_threshold-mstar_cma->cache_batch)
		while(1)
		{
	         spin_lock(&mstar_cma->free_list_spinlock);
			 if(mstar_cma->free_page_count <= *mstar_cma->pcache_high_threshold-mstar_cma->cache_batch){
				spin_unlock(&mstar_cma->free_list_spinlock);
				break;
			 }
	         CMA_BUG_ON(list_empty(&mstar_cma->free_list),"cma free_list should not be empty! \n");
		     page = list_entry(mstar_cma->free_list.next,
							   struct page, lru);
		     list_del(&page->lru);
			 mstar_cma->free_page_count--;

			 spin_unlock(&mstar_cma->free_list_spinlock);

			 pfn = page_to_pfn(page);
			 
			 CMA_BUG_ON(pfn < mstar_cma->cma->base_pfn || pfn >= mstar_cma->cma->base_pfn + mstar_cma->cma->count,
			 	"pfn=%lx,cma[%lx,%lx] \n",pfn,mstar_cma->cma->base_pfn,mstar_cma->cma->count);
			 
			 if(!test_and_clear_bit(pfn - mstar_cma->cma->base_pfn, mstar_cma->cma->bitmap))
			 {
				BUG();
			 }
#ifdef CONFIG_MP_CMA_PATCH_CMA_AGGRESSIVE_ALLOC
			adjust_managed_cma_page_count(mstar_cma->zone, 1); 
#endif			 
			 mstar_cma->zero_bit_count++;
			 CMA_BUG_ON(page_count(page) != 1,"page_count=%d\n",page_count(page));
		     __free_page(page);

	    }
		
	 }
	 mutex_unlock(&mstar_cma->cma->lock);
	 if(mstar_cma->free_page_count<=*mstar_cma->pcache_low_threshold)
	 {
	     while(mstar_cma->free_page_count<*mstar_cma->pcache_low_threshold+mstar_cma->cache_batch)
	     {
	   	     if(!cma_increase_pool_freelist_threshold(mstar_cma))
			 	break;
	     }
	 }

SLEEP:
	 prepare_to_wait(&mstar_cma->cma_swap_wait, &wait, TASK_INTERRUPTIBLE);
	 schedule_timeout(HZ/10);
	 finish_wait(&mstar_cma->cma_swap_wait, &wait);
	}
	return 0;
}

//get one page 
struct page* __mstar_get_discrete(struct ion_heap *heap)
{
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(heap);
	struct mstar_cma *mstar_cma = mstar_cma_heap->mstar_cma;
	struct page *page;

	mstar_cma_heap->mstar_cma->total_alloc_count ++;

TRY_AGAIN:
	spin_lock(&mstar_cma->free_list_spinlock);
	if(!list_empty(&mstar_cma->free_list))
	{
		page = list_entry(mstar_cma->free_list.next,
							   struct page, lru);
	  	list_del(&page->lru);
		
		mstar_cma->free_page_count--;
		spin_unlock(&mstar_cma->free_list_spinlock);

		clear_highpage(page);

		return page;
	}

	spin_unlock(&mstar_cma->free_list_spinlock);
	//goto FALLBACK_KERNEL_PAGE;
	

	signed long timeout = HZ/5;
	DEFINE_WAIT(wait);
	if(mstar_cma->zero_bit_count <=100){//should refine condition here?	  
		goto FALLBACK_KERNEL_PAGE;
	}
	
	try_to_freeze();
	//printk("mstar_cma->zero_bit_count = %x , free count=%x \n",mstar_cma->zero_bit_count,mstar_cma->free_page_count);
	
	if(waitqueue_active(&mstar_cma->cma_swap_wait))
		wake_up_interruptible(&mstar_cma->cma_swap_wait);

	while(timeout>=0){
		timeout -= (wait_event_timeout(mstar_cma->cma_swap_wait,mstar_cma->free_page_count > 0,HZ/100) + 1);
		
		if(!list_empty(&mstar_cma->free_list))
		      goto TRY_AGAIN;	
		
		if(waitqueue_active(&mstar_cma->cma_swap_wait))
		  wake_up_interruptible(&mstar_cma->cma_swap_wait);
	}
		
FALLBACK_KERNEL_PAGE:
	mstar_cma_heap->mstar_cma->fail_alloc_count ++;
#if 0	
	printk("#### fail=%d,free_count=%d,zero_bit_count=%d \n",
		mstar_cma_heap->mstar_cma->fail_alloc_count,
		mstar_cma->free_page_count,
		mstar_cma->zero_bit_count);
#endif
	page =alloc_page(GFP_HIGHUSER| __GFP_ZERO | __GFP_REPEAT | __GFP_NOWARN);
	return page;
	
}


int __mstar_free_one_page(struct ion_heap *heap,struct page *page)
{
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(heap);
	struct mstar_cma *mstar_cma = mstar_cma_heap->mstar_cma;
	unsigned long pfn = page_to_pfn(page);

	if(!(pfn >= mstar_cma->cma->base_pfn && pfn < mstar_cma->cma->base_pfn + mstar_cma->cma->count)){		
		return -1;
	}
	
    CMA_BUG_ON(atomic_read(&page->_count)!= 1,"page_count=%d \n",page_count(page));


#if 1
//free to cache list
	spin_lock(&mstar_cma->free_list_spinlock);
	list_add(&page->lru,&mstar_cma->free_list);
	mstar_cma->free_page_count++;
	spin_unlock(&mstar_cma->free_list_spinlock);

	if(mstar_cma->free_page_count>*mstar_cma->pcache_high_threshold
			&& waitqueue_active(&mstar_cma->cma_swap_wait))
	{
		wake_up_interruptible(&mstar_cma->cma_swap_wait);
	}
#else
// free to buddy system directly
	//spin_lock(&cma->free_list_spinlock);
	//cma->free_page_count--;
	//spin_unlock(&cma->free_list_spinlock);

	pfn = page_to_pfn(page);

	CMA_BUG_ON(pfn<mstar_cma->cma->base_pfn || pfn>=mstar_cma->cma->base_pfn+mstar_cma->cma->count,"pfn=%x,cma[%x,%x] \n",pfn,mstar_cma->cma->base_pfn,mstar_cma->cma->count);

	if(!test_and_clear_bit(pfn - mstar_cma->cma->base_pfn, mstar_cma->cma->bitmap))
	{
		BUG();
	}
	mstar_cma->zero_bit_count++;
	CMA_BUG_ON(page_count(page) != 1,"page_count=%d\n",page_count(page));

	__free_page(page);
#endif
	return 0;
}


/*
 * Create scatter-list for the already allocated DMA buffer.
 * This function could be replaced by dma_common_get_sgtable
 * as soon as it will avalaible.
 */
static int ion_mstar_cma_get_sgtable(struct device *dev, struct sg_table *sgt,
			       struct page *page, dma_addr_t handle, size_t size)
{
	int ret;

	ret = sg_alloc_table(sgt, 1, GFP_KERNEL);
	if (unlikely(ret))
		return ret;

	sg_set_page(sgt->sgl, page, PAGE_ALIGN(size), 0);
	return 0;
}

static int ion_mstar_cma_alloc_discrete(struct ion_heap *heap, 
														struct ion_buffer *buffer,
														unsigned long len)
{
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(heap);
	struct ion_mstar_cma_page_info *info, *tmp_info;
	unsigned long size_remaining = PAGE_ALIGN(len);
	struct list_head page_info_list;
	struct ion_mstar_cma_buffer_info* buffer_info;	
	struct scatterlist *sg;
	int index = 0, ret = 0;

	buffer_info = kzalloc(sizeof(struct ion_mstar_cma_buffer_info), GFP_KERNEL);
	if (!buffer_info) {
		printk("Can't allocate buffer info\n");
		return ION_MSTAR_CMA_ALLOCATE_FAILED;
	}

	INIT_LIST_HEAD(&page_info_list);
	index = 0;
	while (size_remaining > 0) {
		info = kmalloc(sizeof(struct ion_mstar_cma_page_info), GFP_KERNEL);
		if (!info)
			goto err;

		info->page = __mstar_get_discrete(mstar_cma_heap);
		list_add_tail(&info->list, &page_info_list);
		info->size = PAGE_SIZE;
		size_remaining -= PAGE_SIZE;
		index++;
	}
	buffer_info->table = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!buffer_info->table)
		goto err;
	ret = sg_alloc_table(buffer_info->table, index, GFP_KERNEL);
	if (ret)
		goto err1;

	sg = buffer_info->table->sgl;
	list_for_each_entry_safe(info, tmp_info, &page_info_list, list) {
		struct page *page = info->page;
		sg_set_page(sg, page, info->size, 0);
		sg = sg_next(sg);
		list_del(&info->list);
		kfree(info);
	}
	buffer_info->flag |= ION_FLAG_DISCRETE;	
	buffer->priv_virt = buffer_info;
	return 0;

err1:
	kfree(buffer_info->table);

err:
	list_for_each_entry_safe(info, tmp_info, &page_info_list, list) {
		//free page to cache;
		__mstar_free_one_page(heap,info->page);
		kfree(info);
	}
	kfree(buffer_info);
	return -ENOMEM;
}

/* similar to ion_mstar_cma_shrink, but should get cma->lock */
static int ion_mstar_cma_shrink_lock(struct ion_heap *heap)
{
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(heap);
	struct mstar_cma *mstar_cma = mstar_cma_heap->mstar_cma;
	unsigned long pfn, freed = 0;
	struct page *page;

	mutex_lock(&mstar_cma->cma->lock);
		
	while(1)
	{
		spin_lock(&mstar_cma->free_list_spinlock);
		if(list_empty(&mstar_cma->free_list))
		{
		    spin_unlock(&mstar_cma->free_list_spinlock);
		    break;
		}
		page = list_entry(mstar_cma->free_list.next,
							  struct page, lru);
		list_del(&page->lru);
		mstar_cma->free_page_count--;
		
		spin_unlock(&mstar_cma->free_list_spinlock);
		pfn = page_to_pfn(page);
		
		CMA_BUG_ON(pfn<mstar_cma->cma->base_pfn || pfn>=mstar_cma->cma->base_pfn+mstar_cma->cma->count,"pfn=%lx,cma[%lx,%lx] \n",pfn,mstar_cma->cma->base_pfn,mstar_cma->cma->count);
		
		if(!test_and_clear_bit(pfn - mstar_cma->cma->base_pfn, mstar_cma->cma->bitmap))
		{
			BUG();
		}
#ifdef CONFIG_MP_CMA_PATCH_CMA_AGGRESSIVE_ALLOC
		adjust_managed_cma_page_count(mstar_cma->zone, 1); 
#endif			
		mstar_cma->zero_bit_count++;
		CMA_BUG_ON(page_count(page) != 1,"page count=%d \n",page_count(page));
		__free_page(page);
		freed++;
	}

	mutex_unlock(&mstar_cma->cma->lock);

    return freed;
}

static int ion_mstar_cma_alloc_contiguous(struct ion_heap *heap, 
														struct ion_buffer *buffer,
														unsigned long len)
{
	struct ion_mstar_cma_buffer_info* buffer_info;	
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(heap);
	struct device *dev = mstar_cma_heap->dev;
	struct mstar_cma *mstar_cma = mstar_cma_heap->mstar_cma;
	int freepg, retry = 0;

	buffer_info = kzalloc(sizeof(struct ion_mstar_cma_buffer_info), GFP_KERNEL);
	if (!buffer_info) {
		dev_err(dev, "Can't allocate buffer info\n");
		return ION_MSTAR_CMA_ALLOCATE_FAILED;
	}

ALLOC:
	buffer_info->page = dma_alloc_from_contiguous(dev,len>>PAGE_SHIFT,get_order(len));
	buffer_info->size = len;

	if (!buffer_info->page) {
		if (retry < CMA_CONTIG_RTYCNT)
		{
			if (!retry)
			{
				mutex_lock(&mstar_cma->contig_rtylock);
			}

			freepg = get_free_bit_count(mstar_cma->cma, 0, mstar_cma->cma->count)
					+ mstar_cma->free_page_count;
			// we can shrink cma cache and try again
			if (freepg >= len>>PAGE_SHIFT)
			{
				retry++;
				freepg = ion_mstar_cma_shrink_lock(heap);
				printk("CMA contig alloc retry by shrinking %d pages!\n", freepg);

				goto ALLOC;
			}
			else
			{
				mutex_unlock(&mstar_cma->contig_rtylock);
				printk("CMA contig alloc retry fail, lack of free pages! free:0x%x need:0x%lx retry:%d\n", 
					freepg, len>>PAGE_SHIFT, retry);
			}
		}
		else
		{
			mutex_unlock(&mstar_cma->contig_rtylock);
			printk("CMA contig alloc retry fail!\n");
		}

		dev_err(dev, "Fail to allocate buffer1\n");
		goto err;
	}

	if (retry && (retry <= CMA_CONTIG_RTYCNT))
	{
		mutex_unlock(&mstar_cma->contig_rtylock);
		printk("CMA contig alloc retry pass! retry %d\n", retry);
	}
	
	buffer_info->handle = pfn_to_dma(dev, page_to_pfn(buffer_info->page));

	buffer_info->table = kmalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!buffer_info->table) {
		dev_err(dev, "Fail to allocate sg table\n");
		goto free_mem;
	}

	if (ion_mstar_cma_get_sgtable
	    (dev, buffer_info->table, buffer_info->page, buffer_info->handle, len)){
			dma_release_from_contiguous(dev, buffer_info->page, len >> PAGE_SHIFT);
			goto err1;
	}

	buffer_info->flag |= ION_FLAG_CONTIGUOUS;
	buffer->priv_virt = buffer_info;
	dev_dbg(dev, "Allocate buffer %p\n", buffer);
	
	return 0;

err1:
	kfree(buffer_info->table);
free_mem:
	dma_release_from_contiguous(dev, buffer_info->page, len >> PAGE_SHIFT);
err:	
	kfree(buffer_info);
	return ION_MSTAR_CMA_ALLOCATE_FAILED;
}


static int ion_mstar_cma_alloc_from_addr(struct ion_heap *heap, 
														struct ion_buffer *buffer,
														unsigned long start,
														unsigned long len)
{
	struct ion_mstar_cma_buffer_info* buffer_info;
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(heap);
	struct device *dev = mstar_cma_heap->dev;
	struct mstar_cma *mstar_cma = mstar_cma_heap->mstar_cma;
	int freepg, retry = 0;

	buffer_info = kzalloc(sizeof(struct ion_mstar_cma_buffer_info), GFP_KERNEL);
	if (!buffer_info) {
		dev_err(dev, "Can't allocate buffer info\n");
		return ION_MSTAR_CMA_ALLOCATE_FAILED;
	}

ALLOC:
	buffer_info->page = dma_alloc_from_contiguous_addr(dev,start,len >> PAGE_SHIFT,1);
	buffer_info->size = len;

	if (!buffer_info->page) {
		if (retry < CMA_CONTIG_RTYCNT)
		{
			if (!retry)
			{
				mutex_lock(&mstar_cma->contig_rtylock);
			}

			freepg = get_free_bit_count(mstar_cma->cma, 0, mstar_cma->cma->count)
					+ mstar_cma->free_page_count;
			// we can shrink cma cache and try again
			if (freepg >= len>>PAGE_SHIFT)
			{
				retry++;
				freepg = ion_mstar_cma_shrink_lock(heap);
				printk("CMA contig alloc retry with addr by shrinking %d pages!\n", freepg);

				goto ALLOC;
			}
			else
			{
				mutex_unlock(&mstar_cma->contig_rtylock);
				printk("CMA contig alloc retry with addr fail, lack of free pages! free:0x%x need:0x%lx retry:%d\n", 
					freepg, len>>PAGE_SHIFT, retry);
			}
		}
		else
		{
			mutex_unlock(&mstar_cma->contig_rtylock);
			printk("CMA contig alloc retry with addr fail!\n");
		}

		dev_err(dev, "Fail to allocate buffer1\n");
		goto err;
	}

	if (retry && (retry <= CMA_CONTIG_RTYCNT))
	{
		mutex_unlock(&mstar_cma->contig_rtylock);
		printk("CMA contig alloc retry with addr pass! retry %d\n", retry);
	}
	
	buffer_info->handle = pfn_to_dma(dev, page_to_pfn(buffer_info->page));
	
	buffer_info->table = kmalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!buffer_info->table) {
		dev_err(dev, "Fail to allocate sg table\n");
		goto free_mem;
	}

	if (ion_mstar_cma_get_sgtable
	    (dev, buffer_info->table, buffer_info->page, buffer_info->handle, len)){
			dma_release_from_contiguous(dev, buffer_info->page, len >> PAGE_SHIFT);	
			goto err1;
	}

	buffer_info->flag |= ION_FLAG_CONTIGUOUS;
	buffer->priv_virt = buffer_info;
	dev_dbg(dev, "Allocate buffer %p\n", buffer);
	return 0;

err1:
	kfree(buffer_info->table);
free_mem:
	dma_release_from_contiguous(dev, buffer_info->page, len >> PAGE_SHIFT);
err:	
	kfree(buffer_info);
	return ION_MSTAR_CMA_ALLOCATE_FAILED;
}

/* ION CMA heap operations functions */
static int ion_mstar_cma_allocate(struct ion_heap *heap, struct ion_buffer *buffer,
			    unsigned long len, unsigned long align,
			    unsigned long flags)
{
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(heap);
	struct device *dev = mstar_cma_heap->dev;
	int ret;

	dev_dbg(dev, "Request buffer allocation len %ld\n", len);

	if (buffer->flags & ION_FLAG_CACHED)
		return -EINVAL;
	
	if(flags & ION_FLAG_CONTIGUOUS){
		ret = ion_mstar_cma_alloc_contiguous(heap, buffer, len);
		buffer->size = len;
	}
	else if(flags & ION_FLAG_STARTADDR){
		ret = ion_mstar_cma_alloc_from_addr(heap, buffer, align, len);
		buffer->size = len;
	}
	else{
		ret = ion_mstar_cma_alloc_discrete(heap, buffer, len);
	}

	return ret;

}

static void ion_mstar_cma_free(struct ion_buffer *buffer)
{
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(buffer->heap);
	struct device *dev = mstar_cma_heap->dev;
	struct ion_mstar_cma_buffer_info *buffer_info = buffer->priv_virt;
	
	int i;

	dev_dbg(dev, "Release buffer %p\n", buffer);
	/* release memory */

	if(buffer_info->flag & ION_FLAG_DISCRETE){		
		struct sg_table *table = buffer_info->table;
		struct scatterlist *sg;
		
		for_each_sg(table->sgl, sg, table->nents, i){
			CMA_BUG_ON(sg->length != PAGE_SIZE,"invalide sg length %d \n",sg->length);
			__mstar_free_one_page(buffer->heap,sg_page(sg));
		}
	}
	else if(buffer_info->flag & (ION_FLAG_CONTIGUOUS | ION_FLAG_STARTADDR)){	
		dma_release_from_contiguous(dev, buffer_info->page, buffer_info->size >> PAGE_SHIFT);
	}
	else
		CMA_BUG_ON(1,"BUG!!! \n");
	
	/* release sg table */
	sg_free_table(buffer_info->table);
	kfree(buffer_info->table);
	kfree(buffer_info);
}

/* return physical address in addr */
static int ion_mstar_cma_phys(struct ion_heap *heap, struct ion_buffer *buffer,
			ion_phys_addr_t *addr, size_t *len)
{
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(buffer->heap);
	struct device *dev = mstar_cma_heap->dev;
	struct ion_mstar_cma_buffer_info *info = buffer->priv_virt;


	if(info->flag & ION_FLAG_DISCRETE){
		return -ENODEV;
	}
	else if(info->flag & (ION_FLAG_CONTIGUOUS | ION_FLAG_STARTADDR)){	
		dev_dbg(dev, "Return buffer %p physical address 0x%pa\n", buffer,
			&info->handle);

		*addr = info->handle;
		*len = buffer->size;
	}
	else
		CMA_BUG_ON(1,"BUG!!! \n");
	return 0;
}

static struct sg_table *ion_mstar_cma_heap_map_dma(struct ion_heap *heap,
					     struct ion_buffer *buffer)
{
	struct ion_mstar_cma_buffer_info *info = buffer->priv_virt;

	return info->table;
}

static void ion_mstar_cma_heap_unmap_dma(struct ion_heap *heap,
				   struct ion_buffer *buffer)
{
	return;
}


static int ion_cma_heap_map_user(struct ion_heap *heap, struct ion_buffer *buffer,
		      struct vm_area_struct *vma)
{
	struct ion_mstar_cma_buffer_info *info = buffer->priv_virt;
	
	struct sg_table *table = info->table;
	unsigned long addr = vma->vm_start;
	unsigned long offset = vma->vm_pgoff * PAGE_SIZE;
	struct scatterlist *sg;
	int i;
	int ret;

	for_each_sg(table->sgl, sg, table->nents, i) {
		struct page *page = sg_page(sg);
		unsigned long remainder = vma->vm_end - addr;
		unsigned long len = sg->length;

		if (offset >= sg->length) {
			offset -= sg->length;
			continue;
		} else if (offset) {
			page += offset / PAGE_SIZE;
			len = sg->length - offset;
			offset = 0;
		}
		len = min(len, remainder);
		ret = remap_pfn_range(vma, addr, page_to_pfn(page), len,
				vma->vm_page_prot);
		if (ret)
			return ret;
		addr += len;
		if (addr >= vma->vm_end)
			return 0;
	}
	return 0;
}

static void *ion_mstar_cma_map_kernel(struct ion_heap *heap,
				struct ion_buffer *buffer)
{
	struct ion_mstar_cma_buffer_info *info = buffer->priv_virt;

	if(info->flag & ION_FLAG_DISCRETE)
		return ion_heap_map_kernel(heap,buffer);
	else if(info->flag & (ION_FLAG_CONTIGUOUS | ION_FLAG_STARTADDR)){
		return ion_heap_map_kernel(heap,buffer);
	}
	else
		CMA_BUG_ON(1,"BUG!!! \n");

	return 0;	
}

static void ion_mstar_cma_unmap_kernel(struct ion_heap *heap,
					struct ion_buffer *buffer)
{
	struct ion_mstar_cma_buffer_info *info = buffer->priv_virt;

	if(info->flag & ION_FLAG_DISCRETE)
		return ion_heap_unmap_kernel(heap,buffer);
	else if(info->flag & (ION_FLAG_CONTIGUOUS | ION_FLAG_STARTADDR)){
		return ion_heap_unmap_kernel(heap,buffer);
	}
	else
		CMA_BUG_ON(1,"BUG!!! \n");
}

static int ion_mstar_cma_shrink(struct ion_heap *heap, gfp_t gfp_mask,
					int nr_to_scan)
{
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(heap);
    struct mstar_cma *mstar_cma = mstar_cma_heap->mstar_cma;
	unsigned long pfn, freed = 0;
	struct page *page;

	if (allocflags_to_migratetype(gfp_mask) != MIGRATE_MOVABLE)
		return -1;

	if(!mutex_trylock(&mstar_cma->cma->lock))
		goto OUT_NO_LOCK;
		
	while(1)
	{
		spin_lock(&mstar_cma->free_list_spinlock);
		if(list_empty(&mstar_cma->free_list))
		{
		    spin_unlock(&mstar_cma->free_list_spinlock);
		    break;
		}
		page = list_entry(mstar_cma->free_list.next,
							  struct page, lru);
		list_del(&page->lru);
		mstar_cma->free_page_count--;
		
		spin_unlock(&mstar_cma->free_list_spinlock);
		pfn = page_to_pfn(page);
		
		CMA_BUG_ON(pfn<mstar_cma->cma->base_pfn || pfn>=mstar_cma->cma->base_pfn+mstar_cma->cma->count,"pfn=%lx,cma[%lx,%lx] \n",pfn,mstar_cma->cma->base_pfn,mstar_cma->cma->count);
		
		if(!test_and_clear_bit(pfn - mstar_cma->cma->base_pfn, mstar_cma->cma->bitmap))
		{
			BUG();
		}
#ifdef CONFIG_MP_CMA_PATCH_CMA_AGGRESSIVE_ALLOC
		adjust_managed_cma_page_count(mstar_cma->zone, 1); 
#endif			
		mstar_cma->zero_bit_count++;
		CMA_BUG_ON(page_count(page) != 1,"page count=%d \n",page_count(page));
		__free_page(page);
		freed++;
		if(freed >= nr_to_scan)
			break;
	}

	mutex_unlock(&mstar_cma->cma->lock);

OUT_NO_LOCK:
    return mstar_cma->free_page_count;

}

#ifdef CONFIG_MSTAR_CMAPOOL
struct device *ion_mstar_cma_get_dev(struct ion_heap *heap)
{
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(heap);
	struct device *dev = mstar_cma_heap->dev;
    return dev;
}
#endif

static struct ion_heap_ops ion_mstar_cma_ops = {
	.allocate = ion_mstar_cma_allocate,
	.free = ion_mstar_cma_free,
	.map_dma = ion_mstar_cma_heap_map_dma,
	.unmap_dma = ion_mstar_cma_heap_unmap_dma,
	.phys = ion_mstar_cma_phys,
	.map_user = ion_cma_heap_map_user,
	.map_kernel = ion_mstar_cma_map_kernel,
	.unmap_kernel = ion_mstar_cma_unmap_kernel,
	.shrink = ion_mstar_cma_shrink,
#ifdef CONFIG_MSTAR_CMAPOOL
	.get_dev = ion_mstar_cma_get_dev,
#endif		
};

struct ion_heap *ion_mstar_cma_heap_create(struct ion_platform_heap *data)
{
	struct ion_mstar_cma_heap *mstar_cma_heap;
	struct cma *cma;
	char name[64];
	struct mstar_cma_heap_private *mcma_private;

	mstar_cma_heap = kzalloc(sizeof(struct ion_mstar_cma_heap), GFP_KERNEL);

	if (!mstar_cma_heap)
		return ERR_PTR(-ENOMEM);

	mstar_cma_heap->heap.ops = &ion_mstar_cma_ops;
	/* get device from private heaps data, later it will be
	 * used to make the link with reserved CMA memory */

	mcma_private = (struct mstar_cma_heap_private *)data->priv;
	mstar_cma_heap->dev = mcma_private->cma_dev;		
	printk("\033[35mFunction = %s, Line = %d, dev = %p(%s), cma = %p\033[m\n",
		__PRETTY_FUNCTION__, __LINE__, mstar_cma_heap->dev, mstar_cma_heap->dev->init_name, mstar_cma_heap->dev->cma_area);
	BUG_ON(!mstar_cma_heap->dev);
	mstar_cma_heap->heap.type = ION_HEAP_TYPE_MSTAR_CMA;

	mstar_cma_heap->mstar_cma = kzalloc(sizeof(struct mstar_cma),GFP_KERNEL);
	if(!mstar_cma_heap->mstar_cma)
		goto err;

	//initialize the mstar cma private data
	cma = dev_get_cma_area(mstar_cma_heap->dev);
	if (!cma)
		goto err;	

	mstar_cma_heap->mstar_cma->cma = cma;
	mstar_cma_heap->mstar_cma->zone = page_zone(pfn_to_page(cma->base_pfn));
	//printk("################ cma[%x,%x] ,zone=%x \n",cma->base_pfn,cma->count,mstar_cma_heap->mstar_cma->zone);
	mstar_cma_heap->mstar_cma->zero_bit_count = cma->count;
	init_waitqueue_head(&mstar_cma_heap->mstar_cma->cma_swap_wait);
	if(cma->count/10 < cma_miu_threshold[0])
		cma_miu_threshold[0] = cma->count/10;
	mstar_cma_heap->mstar_cma->pcache_low_threshold = &cma_miu_threshold[0];	//max 1M
	if(cma->count/4 < cma_miu_threshold[1])
		cma_miu_threshold[1] = cma->count/4;
	mstar_cma_heap->mstar_cma->pcache_high_threshold = &cma_miu_threshold[1];	//max 32M
	mstar_cma_heap->mstar_cma->cache_batch = cma_miu_threshold[2];
	mstar_cma_heap->mstar_cma->free_page_count = 0;
	mstar_cma_heap->mstar_cma->total_alloc_count = 0;
	mstar_cma_heap->mstar_cma->fail_alloc_count = 0;
	mutex_init(&mstar_cma_heap->mstar_cma->contig_rtylock);
	INIT_LIST_HEAD(&mstar_cma_heap->mstar_cma->free_list);
	spin_lock_init(&mstar_cma_heap->mstar_cma->free_list_spinlock);

	//only descrete cma need worker thread
	if(mcma_private->flag == DESCRETE_CMA){
		sprintf(name,"cma_%s",dev_name(mstar_cma_heap->dev));
		mstar_cma_heap->mstar_cma->swap_worker = kthread_run(cma_swap_worker_thread, mstar_cma_heap->mstar_cma, name);
	    CMA_BUG_ON(IS_ERR(mstar_cma_heap->mstar_cma->swap_worker),"create swap worker thread error! \n");
	}
	//mstar_cma_heap->mstar_cma->debug_thread = kthread_run(cma_debug_thread,&mstar_cma_heap->heap,"debug_thread");
	return &mstar_cma_heap->heap;

err:
	printk("\033[35mFunction = %s, Line = %d,error! \033[m\n", __PRETTY_FUNCTION__, __LINE__);
	
	kfree(mstar_cma_heap);
	return ERR_PTR(-ENOMEM);
	
}

void ion_mstar_cma_heap_destroy(struct ion_heap *heap)
{
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(heap);
	
	kfree(mstar_cma_heap->mstar_cma);
	kfree(mstar_cma_heap);
}

//helper function 
static int get_cma_freebits(struct cma *cma)
{
	int total_free	= 0;
	int start	= 0;
	int pos		= 0;
	int next	= 0;

	while(1){
		pos = find_next_zero_bit(cma->bitmap, cma->count, start);

		if(pos >= cma->count)
			break;
		start = pos + 1;
		next = find_next_bit(cma->bitmap, cma->count, start);

		if(next >= cma->count){
			total_free += (cma->count - pos);
			break;
		}

		total_free += (next - pos);
		start = next + 1;
		
		if(start >= cma->count)
			break;
	}

	return total_free;
}

void get_cma_heap_info(struct ion_heap *heap, int *mali_heap_info[],char *name)
{
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(heap);
	mali_heap_info[0] = mstar_cma_heap->mstar_cma->total_alloc_count;
	mali_heap_info[1] = mstar_cma_heap->mstar_cma->free_page_count;
	mali_heap_info[2] = mstar_cma_heap->mstar_cma->fail_alloc_count;	
	mali_heap_info[3] = get_cma_freebits(mstar_cma_heap->mstar_cma->cma);
	strcpy(name,dev_name(mstar_cma_heap->dev));
}

int in_cma_range(struct ion_heap* heap, struct page* page)
{
	struct ion_mstar_cma_heap *mstar_cma_heap = to_mstar_cma_heap(heap);
	struct mstar_cma *mstar_cma = mstar_cma_heap->mstar_cma;
	unsigned long pfn = page_to_pfn(page);
	
	if(pfn >= mstar_cma->cma->base_pfn && pfn < mstar_cma->cma->base_pfn + mstar_cma->cma->count){
		return 1;
	}

	return 0;
}

