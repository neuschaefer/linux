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
/// @file   ion_mstar_cma_heap.h
/// @brief  mstar ion heap interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LINUX_ION_MSATR_CMA_HEAP_H
#define _LINUX_ION_MSATR_CMA_HEAP_H
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DEBUG
#define ION_CUST_ERR(fmt, args...) printk(KERN_ERR "error %s:%d " fmt,__FUNCTION__,__LINE__,## args)
#ifdef DEBUG
#define ION_CUST_DEBUG(fmt, args...) printk(KERN_ERR "%s:%d " fmt,__FUNCTION__,__LINE__,## args)
#else 
#define ION_CUST_DEBUG(fmt, args...) do {} while(0)
#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
enum cma_heap_flag{
	DESCRETE_CMA,
	CONTINUOUS_ONLY_CMA	
};

struct mstar_cma_heap_private {
    struct dev* cma_dev;
	enum cma_heap_flag flag;	//flag for cma type
};

struct ion_mstar_cma_buffer_info {
	unsigned long flag;
	struct page* page;
	unsigned long size;
	dma_addr_t handle;
	struct sg_table *table;
};

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
struct page* __mstar_get_discrete(struct ion_heap *heap);
int __mstar_free_one_page(struct ion_heap *heap,struct page *page);
void get_cma_heap_info(struct ion_heap *heap, int *mali_heap_info[],char *name);
int in_cma_range(struct ion_heap* heap, struct page* page);

#endif	//_LINUX_ION_MSATR_CMA_HEAP_H

