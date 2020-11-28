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
/// @file   ion_customer.h
/// @brief  ion customer command driver IO interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LINUX_ION_CUSTOMER_H
#define _LINUX_ION_CUSTOMER_H
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "../ion.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MTLB_CUST_START_SHIFT  2
#define MTLB_CUST_START_SET  0x1UL
#define MTLB_CUST_START_MASK  (~0x3UL)

#define ION_BUG_ON(cond)  \
do { \
      if(cond) \
      	printk(KERN_ERR "ION_BUG in %s @ %d \n", __FUNCTION__, __LINE__); \
	  BUG_ON(cond); \
   } while(0)


#define ION_CUST_ERR(fmt, args...) printk(KERN_ERR "error %s:%d " fmt,__FUNCTION__,__LINE__,## args)
#define ION_CUST_WARNING(fmt, args...) printk(KERN_ERR "warning %s:%d " fmt,__FUNCTION__,__LINE__,## args)

//#define DEBUG
#ifdef DEBUG
#define ION_CUST_DEBUG(fmt, args...) printk(KERN_ERR "%s:%d " fmt,__FUNCTION__,__LINE__,## args)
#else 
#define ION_CUST_DEBUG(fmt, args...) do {} while(0)
#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum {
    E_MTLB_RBT_SIZE = 1,
    E_MTLB_RBT_START
} EN_MTLB_RBTType;


struct mtlb_address_space_node {
    struct rb_node node_size;
    struct rb_node node_start;
    long start;
    long entry_size; //unit: one page 4K bytes
};


struct mtlb_address_space {
    char miu;
    struct rb_root mtlb_size_rb_root;
    struct rb_root mtlb_start_rb_root;
    struct list_head mtlb_unmap_list_head;
    atomic_t mtlb_unmap_list_count;
};

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void ion_cust_reserve(void);
int mstar_cust_init (void);
int ion_cust_mtlb_unmap(struct mtlb_address_space * space_item, long start, long length, bool check_unmaplist);
long mstar_custom_ioctl (struct ion_client *client, unsigned int cmd, unsigned long arg);
#endif

