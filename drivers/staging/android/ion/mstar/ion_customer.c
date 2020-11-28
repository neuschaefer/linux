////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   ion_customer.c
/// @brief  ion customer command driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include <linux/list.h>
#include <linux/rbtree.h>
#include <linux/err.h>
#include <linux/scatterlist.h>
#include <linux/highmem.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <linux/sched.h> 
#include <linux/fdtable.h>
#include <linux/pid.h>
#include <linux/file.h>
#include <asm/uaccess.h>
#include "ion_customer.h"
#include "../../uapi/ion_customer.h"
#include "../ion.h"
#include "../ion_priv.h"
#include "mdrv_mtlb.h"

//-------------------------------------------------------------------------------------------------
// Macros and struct
//-------------------------------------------------------------------------------------------------

struct zero_page_info { 
    unsigned long zero_page_phy[E_MTLB_MIU_NUM];
};

struct unmap_info {
    long entry_size; //unit: one page 4K bytes
    unsigned long mapaddr;    
    struct ion_buffer * buffer;
    struct list_head list;
};

#define UNMAPALL  (-1U)
static struct mutex mtlb_rbtree_mutex;
static struct mutex mtlb_ion_cust_mutex;
static DEFINE_SPINLOCK(unmap_list_spinlock);
static struct zero_page_info zero_page;
static const unsigned int unmapsize[] = {1, 2, 4, 8, 16, 32, UNMAPALL};
static const int num_unmapsize = ARRAY_SIZE(unmapsize);
static mtlb_hardware_info hardware_info;
struct mtlb_address_space  mtlb_addr_space[E_MTLB_MIU_NUM];


static int ion_cust_deferred_unmap(struct mtlb_address_space * space_item, int times);

#ifdef DEBUG
static int gettime_diff(struct timeval *start_time, struct timeval *end_time)
{
   int diff_time = 0; //us

   do_gettimeofday(end_time);

   if(start_time)
   {
      diff_time = ((end_time->tv_sec-start_time->tv_sec)*1000000 + 
         (end_time->tv_usec-start_time->tv_usec));
   }

   return diff_time;
}
#endif

static void _mtlb_dump_tree(struct rb_root *root, EN_MTLB_RBTType rbttyep)
{
    struct rb_node *node;
    struct mtlb_address_space_node * data = NULL;	

    //need double check  below code
    if (RB_EMPTY_NODE(root->rb_node))
        return;

    if(rbttyep == E_MTLB_RBT_SIZE)
    {
        printk(KERN_ERR "dump size rbtree ====== start =====\n");
    }
    else
    {        
        printk(KERN_ERR "dump start rbtree ===== start =====\n");
    }

    for (node = rb_first(root); node != NULL; node = rb_next(node))
    {
        if(rbttyep == E_MTLB_RBT_SIZE)
        {
            data = container_of(node, struct mtlb_address_space_node, node_size);
        }
        else
        {
            data = container_of(node, struct mtlb_address_space_node, node_start);
        }
        
        printk(KERN_ERR "start %ld, size %ld\n", data->start, data->entry_size);
    }


    if(rbttyep == E_MTLB_RBT_SIZE)
    {
        printk(KERN_ERR "dump size rbtree ====== end =====\n");
    }
    else
    {        
        printk(KERN_ERR "dump start rbtree ===== end =====\n");
    }
}

static int _mtlb_node_insert(struct rb_root *root, struct mtlb_address_space_node * mtlb_node, 
    EN_MTLB_RBTType rbttyep)
{
    struct rb_node **node = &(root->rb_node),  *parent = NULL;
    struct mtlb_address_space_node * this = NULL;
    long result =  0;

    while(*node)
    {
        if(rbttyep == E_MTLB_RBT_SIZE)
        {            
            this = container_of(*node, struct mtlb_address_space_node, node_size);
            result =  mtlb_node->entry_size - this->entry_size;
        }
        else
        {            
            this = container_of(*node, struct mtlb_address_space_node, node_start);
            result =  mtlb_node->start - this->start;
        }

        parent = *node;

        if(result > 0)
            node = &((*node)->rb_right);
        else if(result < 0)
            node = &((*node)->rb_left);
        else
        {
            if(rbttyep == E_MTLB_RBT_START)
            {
                ION_CUST_ERR("node exist in start tree, start %ld size %ld\n", 
                    mtlb_node->start, mtlb_node->entry_size);
                return -EEXIST;
            }
                

            //in size rb tree,  if size key equals, compare start
            result = mtlb_node->start - this->start;

            if(result > 0)
                node = &((*node)->rb_right);
            else if(result < 0)
                node = &((*node)->rb_left);
            else
            {
                ION_CUST_ERR("node exist in size tree, start %ld size %ld\n", 
                    mtlb_node->start, mtlb_node->entry_size);
                return -EEXIST;
            }            
        }
            
    }

    if(rbttyep == E_MTLB_RBT_SIZE)
    {
        rb_link_node(&mtlb_node->node_size, parent, node);
        rb_insert_color(&mtlb_node->node_size, root);
    }
    else
    {
        rb_link_node(&mtlb_node->node_start, parent, node);
        rb_insert_color(&mtlb_node->node_start, root);    
    }

    return 0;
}

/* 
  * search for one chunk its entry size is the smallest of  the node >= value 
  */
static struct mtlb_address_space_node * _mtlb_nodesize_search(struct rb_root *root, long value, 
    EN_MTLB_RBTType rbttyep)
{
    struct rb_node *node = root->rb_node, *find = NULL;
    struct mtlb_address_space_node *data = NULL;
    long result = 0;

    //need double check if below code
    if (RB_EMPTY_NODE(root->rb_node))
        return NULL;
    ION_BUG_ON(rbttyep != E_MTLB_RBT_SIZE);

    while (node)
    {        
        data = container_of(node, struct mtlb_address_space_node, node_size);
        result = data->entry_size - value;


        if (result < 0)
        {
            node = node->rb_right;
        }
        else if (result > 0)
        {
            find = node;
            node = node->rb_left;
        }
        else
            return data;
    }

    if(!find)
    {
        ION_CUST_WARNING("rb tree has no enough size, need size %ld, dump size tree\n", value);
        _mtlb_dump_tree(root, E_MTLB_RBT_SIZE);
        return NULL;
    }

    data = container_of(find, struct mtlb_address_space_node, node_size);
    ION_BUG_ON(data->entry_size < value);
    
    return data;
}

/*  this function only for debug internal
  *
  */
static void _find_node_neighbor_debug(struct rb_root *root, long start)
{
    struct rb_node *node = root->rb_node;
    struct mtlb_address_space_node * this = NULL, *find = NULL;
    long result = 0;   


    if (rb_first(root) == NULL)
    {   
        printk(KERN_ERR "empty start rb tree\n");
        return;
    }

    this = container_of(node, struct mtlb_address_space_node, node_start);
    printk(KERN_ERR "rootnode start %ld, size %ld\n", this->start, this->entry_size);

 
    while(node && !find)
    {            
        this = container_of(node, struct mtlb_address_space_node, node_start);
        result = start - this->start;

        if(result > 0)
        {
            node = node->rb_right;
            if(node)
            {
                this = container_of(node, struct mtlb_address_space_node, node_start);
                printk(KERN_ERR "right start %ld, size %ld\n", this->start, this->entry_size);
            }
        }
        else if(result < 0)
        {
            node = node->rb_left;
            if(node)
            {
                this = container_of(node, struct mtlb_address_space_node, node_start);
                printk(KERN_ERR "left start %ld, size %ld\n", this->start, this->entry_size);
            }
        }
        else
        {
            find = this;
            printk(KERN_ERR "find it\n");
        }
    }
}


/*  this function only been callen with start rb tree
  *
  *  find the prev and next mtlb node
  *  if the start already exits in tree, return this node
  *  else return value follow the rule: 
  *  prev < start < next
  */
static struct mtlb_address_space_node * _find_node_neighbor(struct rb_root *root, long start, 
    struct mtlb_address_space_node ** prev, struct mtlb_address_space_node ** next)
{
    struct rb_node *node = root->rb_node,  *parent = NULL;
    struct mtlb_address_space_node * this = NULL, *find = NULL;
    long result = 0;
    
    //todo: need check 
    if (RB_EMPTY_NODE(root->rb_node))
    {   
        ION_CUST_WARNING("empty start rb tree\n");
        *prev = *next = NULL;
        return NULL;
    }
    
    while(node && !find)
    {            
        this = container_of(node, struct mtlb_address_space_node, node_start);
        result = start - this->start;
       
        parent = node;
        if(result > 0)
            node = node->rb_right;
        else if(result < 0)
            node = node->rb_left;
        else
            find = this;
    }

    if(find)
    {
        *prev = *next = NULL;
        return find;
    }

    this = container_of(parent, struct mtlb_address_space_node, node_start);
    if(this->start > start)
    {
        *next = this;
        parent = rb_prev(parent);
        if(parent)
        {
            this = container_of(parent, struct mtlb_address_space_node, node_start);
            *prev = this;
        }       
    }
    else
    {
        *prev = this;
        parent = rb_next(parent);
        if(parent)
        {
            this = container_of(parent, struct mtlb_address_space_node, node_start);
            *next = this;
        }      
    }
    
    if((*prev == NULL) && (*next == NULL))
    {
       _find_node_neighbor_debug(root, start);
    }

    //already freed by upper layer,  start hit in  prev node rage[start, start+entrysize)
    if(*prev)
    {
        if(((*prev)->start <= start)
            && (start < ((*prev)->start + (*prev)->entry_size)))
        {
            ION_CUST_ERR("should not been here, start %ld, prev_start %ld, prev_size %ld, dump\n", 
                start, (*prev)->start, (*prev)->entry_size);
            _find_node_neighbor_debug(root, start);

            find = *prev;
            *prev = *next = NULL;
        }
    }

    return find;
}

static int _init_MTLB_AddrSpace(int TLB_MAPTO_MIU, struct mtlb_address_space *mtlb_addr_space)
{
   int count = 0, index = 0;

   ION_BUG_ON(mtlb_addr_space == NULL);
   if(TLB_MAPTO_MIU <= 0)
      return 0;

   for(index = 0; index < E_MTLB_MIU_NUM; ++index)
   {
      mtlb_addr_space[index].miu = E_MTLB_MIU_INVALID;
      mtlb_addr_space[index].mtlb_size_rb_root = RB_ROOT;
      mtlb_addr_space[index].mtlb_start_rb_root = RB_ROOT;
      INIT_LIST_HEAD(&mtlb_addr_space[index].mtlb_unmap_list_head);
      atomic_set(&mtlb_addr_space[index].mtlb_unmap_list_count, 0);   
   }

   if(TLB_MAPTO_MIU & E_MTLB_MIU_0)
   {
      count++;
      mtlb_addr_space[0].miu = 0;
   }


   if(TLB_MAPTO_MIU & E_MTLB_MIU_1)
   {
      count++;
      mtlb_addr_space[1].miu = 1;
   }
   
   if(TLB_MAPTO_MIU & E_MTLB_MIU_2)
   {
      count++;
      mtlb_addr_space[2].miu = 2;
   }

   return count;   
}


static struct mtlb_address_space * _getAddrSpaceByMiu(unsigned char miu_sel, unsigned long *tableaddress)
{
   struct mtlb_address_space * item = NULL;
   
   switch (miu_sel) 
   {
    case 0:
       item = &mtlb_addr_space[0];      
       *tableaddress = hardware_info.tlb_table_addr_miu0; 
       break;
    case 1:
       item = &mtlb_addr_space[1];
       *tableaddress = hardware_info.tlb_table_addr_miu1; 
       break;
    case 2:
       item = &mtlb_addr_space[2];
       *tableaddress = hardware_info.tlb_table_addr_miu2; 
       break;
    default:
       item = NULL;
   }

   if(item && item->miu == E_MTLB_MIU_INVALID)
   {
      item = NULL;
   }
   
   return item;
}

int mstar_cust_init (void)
{
    int miu_count = 0, index;
    long entry_size = 0;
    struct mtlb_address_space_node * node = NULL; 

    MDrv_MTLB_Get_HWInfo(&hardware_info);
    miu_count = _init_MTLB_AddrSpace(hardware_info.TLB_MAPTO_MIU, mtlb_addr_space);
    
    //if (ge&gop has dedicated tlb)
    if(hardware_info.GE_Has_TLB && hardware_info.GOP_Has_TLB)
    {
        ION_BUG_ON(miu_count <= 0);
    }

    mutex_init(&mtlb_rbtree_mutex);
    mutex_init(&mtlb_ion_cust_mutex);

    entry_size = MDrv_MTLB_TableSize();
    ION_BUG_ON(entry_size <= 0);

    mutex_lock(&mtlb_rbtree_mutex);
    for(index = 0; index < E_MTLB_MIU_NUM; ++index)
    {
        if(mtlb_addr_space[index].miu == E_MTLB_MIU_INVALID)
            continue;
        
        node = kzalloc(sizeof(struct mtlb_address_space_node), GFP_KERNEL);
        ION_BUG_ON(!node);
        
        node->entry_size = entry_size;
        node->start = 0;     
        _mtlb_node_insert(&mtlb_addr_space[index].mtlb_start_rb_root, node, E_MTLB_RBT_START);
        _mtlb_node_insert(&mtlb_addr_space[index].mtlb_size_rb_root, node, E_MTLB_RBT_SIZE);
    }
    mutex_unlock(&mtlb_rbtree_mutex);

    return 0;
}

int ion_cust_mtlb_unmap(struct mtlb_address_space * space_item, long start, 
    long length, bool check_unmaplist)
{
    int ret = 0;
    unsigned long flag = 0, pa = 0;
    struct mtlb_address_space_node * node;
    struct mtlb_address_space_node * prev = NULL;
    struct mtlb_address_space_node * next = NULL;
    struct unmap_info * unmap_cur = NULL, * unmap_next = NULL;
    bool front_merge = false, back_merge = false;

    if(length <= 0)
        return -EINVAL;

    if(check_unmaplist)
    {
        spin_lock(&unmap_list_spinlock);
        list_for_each_entry_safe(unmap_cur, unmap_next, &space_item->mtlb_unmap_list_head, list) 
        {
            if (unmap_cur->entry_size == length && unmap_cur->mapaddr == (start<<PAGE_SHIFT))
            {
                list_del(&unmap_cur->list);
                atomic_dec(&space_item->mtlb_unmap_list_count);
                kfree(unmap_cur);
            }
        }
        spin_unlock(&unmap_list_spinlock);
    }

    mutex_lock(&mtlb_rbtree_mutex);
    node = _find_node_neighbor(&space_item->mtlb_start_rb_root, start, &prev, &next);
    ION_BUG_ON(node);
    
    if(prev && (prev->start+prev->entry_size == start))
        front_merge = true;

    if(next && (start+length == next->start))
        back_merge = true;

    if(front_merge && back_merge)
    {
        //rb size tree modify: all size changed, delete prev and next and insert new
        rb_erase(&prev->node_size, &space_item->mtlb_size_rb_root);
        rb_erase(&next->node_size, &space_item->mtlb_size_rb_root);
        prev->entry_size = prev->entry_size + length + next->entry_size;
        ret = _mtlb_node_insert(&space_item->mtlb_size_rb_root, prev, E_MTLB_RBT_SIZE);
        ION_BUG_ON(ret != 0);

        //rb start tree modify: prev's start not change, and delete next        
        rb_erase(&next->node_start, &space_item->mtlb_start_rb_root);
  
        //free node of next
        kfree(next);
    }
    else if(front_merge)
    {
        //rb size tree modify: prev size changed, delete prev and insert new
        rb_erase(&prev->node_size, &space_item->mtlb_size_rb_root);
        prev->entry_size = prev->entry_size + length;
        ret = _mtlb_node_insert(&space_item->mtlb_size_rb_root, prev, E_MTLB_RBT_SIZE);
        ION_BUG_ON(ret != 0);

        //rb start tree modify: prev's start not change, do nothing 
    }
    else if(back_merge)
    {
        //rb size tree modify: next size changed, delete next and insert new
        rb_erase(&next->node_size, &space_item->mtlb_size_rb_root);
        next->entry_size = next->entry_size + length;
        ret = _mtlb_node_insert(&space_item->mtlb_size_rb_root, next, E_MTLB_RBT_SIZE);
        ION_BUG_ON(ret != 0);

        //rb start tree modify: next's start change, delete and insert 
        rb_erase(&next->node_start, &space_item->mtlb_start_rb_root);
        next->start = start;
        ret = _mtlb_node_insert(&space_item->mtlb_start_rb_root, next, E_MTLB_RBT_START);
        ION_BUG_ON(ret != 0);        
    }
    else
    {
        //insert a new node
        node = kzalloc(sizeof(struct mtlb_address_space_node), GFP_KERNEL);
        if(!node)
        {
            ret = -ENOMEM;
            mutex_unlock(&mtlb_rbtree_mutex);
            return ret;
        }
        
        //INIT_LIST_HEAD(&node->list);
        node->entry_size = length;
        node->start = start;
        
        ret = _mtlb_node_insert(&space_item->mtlb_start_rb_root, node, E_MTLB_RBT_START);
        ION_BUG_ON(ret != 0);
        ret = _mtlb_node_insert(&space_item->mtlb_size_rb_root, node, E_MTLB_RBT_SIZE);
        ION_BUG_ON(ret != 0);
    }

    //clear mtlb table
    pa = zero_page.zero_page_phy[(int)space_item->miu];
    MDrv_MTLB_Mapping_Start(&flag);
    ret = MDrv_MTLB_Mapping(space_item->miu, start<<PAGE_SHIFT, pa, false, length<<PAGE_SHIFT);
    MDrv_MTLB_Mapping_End(&flag, space_item->miu, start, length);
    ION_BUG_ON(ret);
   
    mutex_unlock(&mtlb_rbtree_mutex);
    return ret;
}


static int ion_cust_lock(struct ion_handle *handle, ion_cust_alloc_data * cust_alloc_data)
{
    int ret = 0, i = 0;
    unsigned char miu_sel = 0;
    unsigned long length = 0, flag = 0;
    unsigned long * paarray = NULL;
    unsigned long tableaddress = 0;
    struct scatterlist *sg;
    struct ion_buffer * buffer = handle->buffer;
    struct sg_table *table = buffer->sg_table;
    struct mtlb_address_space_node * node;
    struct unmap_info * unmap_cur = NULL, * unmap_next = NULL;
    struct mtlb_address_space * space_item = NULL;
    unsigned long u32mtlbva, u32pa;
    int searchtimes = 0;

    ION_BUG_ON(table->nents <= 0);
    ION_BUG_ON(table->sgl[0].offset != 0);
    cust_alloc_data->tlb_table_change = 0;

    //only one sg, one physical section.
    if (cust_alloc_data->addrType == E_ADDR_PHY)
    {
        if (table->nents == 1)
        {           
           cust_alloc_data->mapaddr = table->sgl[0].dma_address;
           return 0;        
        }
        else
        {
           ION_CUST_ERR("table->nents > 0, is %d\n", table->nents);
           return -EINVAL;
        }
    }
    
    //more than one sg, more than one physical section
    length = buffer->size/PAGE_SIZE;
    
    paarray = kzalloc(sizeof(paarray) * table->nents, GFP_KERNEL);
    if (!paarray)
    {
        ION_CUST_ERR("kzalloc fail\n");
        return -ENOMEM;
    }

    for_each_sg(table->sgl, sg, table->nents, i)
    {
        paarray[i] = sg_dma_address(sg);
    }
    
    if (MDrv_MTLB_Mapping_Address_Support(paarray, table->nents, &miu_sel))
    {
        kfree(paarray);
        
        ION_CUST_ERR("buffer allocated not in same miu\n");
        return -EINVAL;
    }
    kfree(paarray);

    u32pa = table->sgl[0].dma_address;

    space_item = _getAddrSpaceByMiu(miu_sel, &tableaddress);    
    
    if(!space_item)
    {
        ION_CUST_ERR("not find space item\n");
        return -EINVAL;
    }
    
    spin_lock(&unmap_list_spinlock);
    list_for_each_entry_safe_reverse(unmap_cur, unmap_next, &space_item->mtlb_unmap_list_head, list)
    {
        if (unmap_cur->entry_size == length && unmap_cur->buffer == buffer)
        {
            list_del(&unmap_cur->list);
            atomic_dec(&space_item->mtlb_unmap_list_count);
            cust_alloc_data->mapaddr = unmap_cur->mapaddr;
            cust_alloc_data->miu = miu_sel;
            cust_alloc_data->miu_offset = tableaddress;
            kfree(unmap_cur);
            spin_unlock(&unmap_list_spinlock);
            return 0;
        }
    }
    spin_unlock(&unmap_list_spinlock);

    mutex_lock(&mtlb_rbtree_mutex);
SEARCH_AGAIN:
    node = _mtlb_nodesize_search(&space_item->mtlb_size_rb_root, length, E_MTLB_RBT_SIZE);
    ++searchtimes;

    if(!node)
    {
        if(searchtimes >= num_unmapsize)
        {
            ret = -EAGAIN;
            goto ION_CUST_LOCK_DONE;
        }
        //unmap sereval nodes
        #if 0
        spin_lock(&unmap_list_spinlock);
        list_for_each_entry(unmap_cur, &space_item->mtlb_unmap_list_head, list)
        {
            unmap_count += unmap_cur->entry_size;
        }
        spin_unlock(&unmap_list_spinlock);
        ION_CUST_WARNING("ion_cust_deferred_unmap searchtimes %d, mtlb_unmap list count %d, entry count %d\n", 
        searchtimes, atomic_read(&space_item->mtlb_unmap_list_count), unmap_count);
        #endif

        mutex_unlock(&mtlb_rbtree_mutex);
        ion_cust_deferred_unmap(space_item, searchtimes);
        mutex_lock(&mtlb_rbtree_mutex);

        goto SEARCH_AGAIN;
    }

    u32mtlbva = node->start << PAGE_SHIFT;
    cust_alloc_data->mapaddr = u32mtlbva;
    cust_alloc_data->miu = miu_sel;
    cust_alloc_data->miu_offset = tableaddress;
    cust_alloc_data->tlb_table_change = 1;

    MDrv_MTLB_Mapping_Start(&flag);
    for_each_sg(table->sgl, sg, table->nents, i)
    {
        unsigned long u32size = PAGE_ALIGN(sg->length);

        ret = MDrv_MTLB_Mapping(miu_sel, u32mtlbva, sg->dma_address, true, u32size);
        ION_BUG_ON(ret);
        u32mtlbva += u32size;
    }
    MDrv_MTLB_Mapping_End(&flag, miu_sel, node->start, length);

    buffer->node_start = (unsigned long)((node->start << MTLB_CUST_START_SHIFT) | MTLB_CUST_START_SET);    
    buffer->priv = (void *)space_item;

    //now  entry_size >= length
    rb_erase(&node->node_size, &space_item->mtlb_size_rb_root);
    rb_erase(&node->node_start, &space_item->mtlb_start_rb_root);
    if(node->entry_size == length)
    {
       kfree(node);
    }
    else
    {
       node->entry_size -= length;
       node->start += length;
       ret = _mtlb_node_insert(&space_item->mtlb_size_rb_root, node, E_MTLB_RBT_SIZE);
       ION_BUG_ON(ret != 0);
       ret = _mtlb_node_insert(&space_item->mtlb_start_rb_root, node, E_MTLB_RBT_START);
       ION_BUG_ON(ret != 0);
    }
  
ION_CUST_LOCK_DONE:
    mutex_unlock(&mtlb_rbtree_mutex);
  
    return 0;
}


static int ion_cust_deferred_unmap(struct mtlb_address_space * space_item, int times)
{
    int ret = 0;
    unsigned int unmapnum = 1, unmap_count = 0;
    struct unmap_info * unmapinfo = NULL;
    struct ion_buffer * buffer = NULL;
    long start, length;

    if(times > num_unmapsize)
        unmapnum = UNMAPALL;
    else
        unmapnum = unmapsize[times];

    ION_BUG_ON(space_item == NULL);

    while(unmap_count <= unmapnum)
    {
        spin_lock(&unmap_list_spinlock);
        if(list_empty(&space_item->mtlb_unmap_list_head))
        {
            spin_unlock(&unmap_list_spinlock);
            return 0;
        }
        
        unmapinfo = list_entry(space_item->mtlb_unmap_list_head.next, struct unmap_info, list);
        list_del(&unmapinfo->list);    
        spin_unlock(&unmap_list_spinlock);
        
        start = unmapinfo->mapaddr >> PAGE_SHIFT;
        length = unmapinfo->entry_size;

        ret = ion_cust_mtlb_unmap(space_item, start, length, false);
        if(ret)
        {
            ION_CUST_ERR("ion_cust_mtlb_unmap fail, start %ld length %ld\n", start, length);
            goto ION_CUST_DEFERRED_UNMAP_DONE;
        }
        else
        {
            buffer = unmapinfo->buffer;
            mutex_lock(&buffer->lock);
            buffer->node_start = 0;
            mutex_unlock(&buffer->lock);         
        }
        kfree(unmapinfo);
        unmap_count++;
    }

ION_CUST_DEFERRED_UNMAP_DONE:  
    return ret;
}


static int ion_cust_unlock(struct ion_handle *handle, ion_cust_alloc_data * cust_alloc_data)
{
    struct sg_table *table = handle->buffer->sg_table;
    struct mtlb_address_space_node * node = NULL;
    struct mtlb_address_space_node * prev = NULL;
    struct mtlb_address_space_node * next = NULL;
    struct unmap_info * unmapinfo = NULL;
    struct mtlb_address_space * space_item = NULL;
    unsigned long tableaddr = 0;
    unsigned long length = 0;
    long start = 0;


    ION_BUG_ON(table->nents <= 0);
    ION_BUG_ON(table->sgl[0].offset != 0);

    //only one sg, one physical section    
    if (cust_alloc_data->addrType == E_ADDR_PHY)
    {
       if (table->nents > 1)
       {
          ION_CUST_ERR("sg table nents %d, why phy address ???\n", table->nents);
          return -EINVAL;
       }
       
       return 0;
    }    

    //more than one physical sections
    length = handle->buffer->size/PAGE_SIZE;
    start = cust_alloc_data->mapaddr >> PAGE_SHIFT;

    space_item = _getAddrSpaceByMiu(cust_alloc_data->miu, &tableaddr);
    if(!space_item)
        return -EINVAL;

    mutex_lock(&mtlb_rbtree_mutex);
    node = _find_node_neighbor(&space_item->mtlb_start_rb_root, start, &prev, &next);
    mutex_unlock(&mtlb_rbtree_mutex);

    if(node)
    {
        ION_CUST_ERR("unlock node exit in rb tree: start %ld, length %lu\n", start, length);
        ION_BUG_ON(node != NULL);  
    }

    unmapinfo = kzalloc(sizeof(struct unmap_info), GFP_KERNEL);
    if(!unmapinfo)
    {
        return -ENOMEM;
    }
    INIT_LIST_HEAD(&unmapinfo->list);
    
    unmapinfo->mapaddr = cust_alloc_data->mapaddr;
    unmapinfo->entry_size = length;    
    unmapinfo->buffer = handle->buffer;    

    spin_lock(&unmap_list_spinlock);
    //in unmap list, the more close to tail, the more active
    list_add_tail(&unmapinfo->list, &space_item->mtlb_unmap_list_head);
    atomic_inc(&space_item->mtlb_unmap_list_count);    
    spin_unlock(&unmap_list_spinlock);

    return 0;
}

//support dfb: use this API to share memory by different process
static int task_fd_install(pid_t task_id, int fd_source)
{
    int fd = -1, error = 0;
    struct file *file;   
    struct task_struct *task_source = NULL;
    struct files_struct * files = NULL;

    if(task_pid_nr(current->group_leader) == task_id)
        return -EFAULT;

    fd = get_unused_fd_flags(O_CLOEXEC);
    if (fd < 0)
    {
        return -EFAULT;
    }   

    rcu_read_lock();
    task_source = find_task_by_vpid(task_id);
    if (task_source)
        get_task_struct(task_source);
    rcu_read_unlock();

    if (!task_source)
    {
        error = -ESRCH;
        goto err_put_unused_fd;
    }
    
    files = get_files_struct(task_source);
    put_task_struct(task_source);
    if(files == NULL)
    {
        error = -EFAULT;
        goto err_put_unused_fd;
    }

    spin_lock(&files->file_lock);
    file = fcheck_files(files, fd_source);
    if (file) 
    {
        /* File object ref couldn't be taken */
        if (file->f_mode & FMODE_PATH ||
            !atomic_long_inc_not_zero(&file->f_count))
            file = NULL;      
    }
    spin_unlock(&files->file_lock);
    put_files_struct(files);

    if(file == NULL)
    {
        error = -EFAULT;
        goto err_put_unused_fd;
    }

    fd_install(fd, file);    
    return fd;

err_put_unused_fd:
    put_unused_fd(fd);
    return error;    
}



/* clear the mtlb table on all mius
  * if the zero_page_phy == 0, the table not exist
  */
static int _mtlb_init_allMapping(void)
{
    int tablesize = 0, ret = 0, miu = 0;
    unsigned long zero_page_phy = 0, flag = 0;
    
    MDrv_MTLB_Get_ZeroPage(&zero_page.zero_page_phy[0], &zero_page.zero_page_phy[1], 
        &zero_page.zero_page_phy[2]);
    tablesize = MDrv_MTLB_TableSize();

    for(miu = 0; miu < E_MTLB_MIU_NUM; ++miu)
    {
        zero_page_phy = zero_page.zero_page_phy[miu];
        if(zero_page_phy == 0)
            continue;
        
        MDrv_MTLB_Mapping_Start(&flag);
        ret = MDrv_MTLB_Mapping(miu, 0, zero_page_phy, false, tablesize<<PAGE_SHIFT);
        MDrv_MTLB_Mapping_End(&flag, miu, 0, tablesize);       
        ION_BUG_ON(ret);
    }  

    return 0;
}


long mstar_custom_ioctl (struct ion_client *client, unsigned int cmd, unsigned long arg)
{
    int ret = 0, err = 0;

    if (_IOC_DIR(cmd) & _IOC_READ)
    {
        err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
        if(err)
            return -EFAULT;
    }

    if (_IOC_DIR(cmd) & _IOC_WRITE)
    {
        err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
        if(err)
            return -EFAULT;
    }

    mutex_lock(&mtlb_ion_cust_mutex);
    switch (cmd)
    {
        case MTLB_CUST_IOC_GET_HWINO:
        {
            mtlb_hardware_info hw_info;

            MDrv_MTLB_Get_HWInfo(&hw_info);
            if (copy_to_user((void __user *)arg, &hw_info, _IOC_SIZE(cmd)))
                ret = -EFAULT;

            break;
        }
        case MTLB_CUST_IOC_GET_HWSTATUS:
        {
            mtlb_hardware_status hw_status;

            MDrv_MTLB_Get_HWStatus(&hw_status);
            if (copy_to_user((void __user *)arg, &hw_status, _IOC_SIZE(cmd)))
                ret = -EFAULT;

            break;
        }
        case MTLB_CUST_IOC_GET_TABLESIZE:
        {
            int tablesize = 0;
            tablesize = MDrv_MTLB_TableSize();
            if (copy_to_user((void __user *)arg, &tablesize, _IOC_SIZE(cmd)))
                ret = -EFAULT;

            break;
        }
        case MTLB_CUST_IOC_INIT:
        {
            if(MDrv_MTLB_Init())
                ret = -ENODEV;

            break;
        }
        case MTLB_CUST_IOC_ENABLE:
        {
            mtlb_tlbclient_enable tlbclient_enable;

            if (copy_from_user(&tlbclient_enable, (void __user *)arg, _IOC_SIZE(cmd)))
            {
                ret = -EFAULT;
                goto ION_CUST_IOCTL_DONE;
            }

            if(MDrv_MTLB_Enable(&tlbclient_enable))
                ret = -EINVAL;
            else
            { 
                //clear mtlb table
                _mtlb_init_allMapping();            
            }          

            break;
        }
        case MTLB_CUST_IOC_LOCK:
        {
            struct ion_handle *handle;
            ion_cust_alloc_data alloction_data;

            if (copy_from_user(&alloction_data, (void __user *)arg, _IOC_SIZE(cmd)))
            {
                ret = -EFAULT;
                goto ION_CUST_IOCTL_DONE;
            }

            handle = ion_handle_get_by_id(client, alloction_data.handle);
            if (IS_ERR(handle))
            {
                ret = -EINVAL;
                goto ION_CUST_IOCTL_DONE;
            }           

            ret = ion_cust_lock(handle, &alloction_data);
            ion_handle_put(handle);
            if(ret)
                goto ION_CUST_IOCTL_DONE;

            if (copy_to_user((void __user *)arg, &alloction_data, _IOC_SIZE(cmd)))
                ret = -EFAULT;          

            break;
        }
        case MTLB_CUST_IOC_UNLOCK:
        {
            struct ion_handle *handle;
            ion_cust_alloc_data alloction_data;

            if (copy_from_user(&alloction_data, (void __user *)arg, _IOC_SIZE(cmd)))
            {
                ret = -EFAULT;
                goto ION_CUST_IOCTL_DONE;
            }

            handle = ion_handle_get_by_id(client, alloction_data.handle);
            if (IS_ERR(handle))
            {
                ret = -EINVAL;
                goto ION_CUST_IOCTL_DONE;
            }

            ret = ion_cust_unlock(handle, &alloction_data);
            ion_handle_put(handle);
            break;
        }

        //support dfb: use this API to share memory by different process
        case MTLB_CUST_IOC_IMPORT:
        {
            int fd;            
            ion_cust_import import_info;

            if (copy_from_user(&import_info, (void __user *)arg, _IOC_SIZE(cmd)))
            {
                ret = -EFAULT;
                goto ION_CUST_IOCTL_DONE;
            }            	

            fd = task_fd_install(import_info.pid, import_info.fd);
            if(fd < 0)
            {
                ret = fd;
                goto ION_CUST_IOCTL_DONE;       
            }
            
            import_info.newfd = fd;
            if (copy_to_user((void __user *)arg, &import_info, _IOC_SIZE(cmd)))
                ret = -EFAULT;

            break;
        }
        default:
        return -ENOTTY;
    }

ION_CUST_IOCTL_DONE:
    mutex_unlock(&mtlb_ion_cust_mutex);
    return ret;
}

void __init ion_cust_reserve()
{
    MDrv_MTLB_Reserve_Memory();
}

