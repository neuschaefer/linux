/*
 * pal_vlynq.c
 * Description:
 * See below.
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */

/******************************************************************************
 * FILE PURPOSE:    PAL VLYNQ File
 ******************************************************************************
 * FILE NAME:       pal_vlynq.c
 *
 * DESCRIPTION:     PAL VLYNQ File
 *
 *******************************************************************************/

#include "pal_vlynq.h"
#include "pal_vlynqIoctl.h"

#define VLYNQ_IVEC_OFFSET(line)         (line*8)

#define VLYNQ_IVEC_POL_BIT_MAP          (1 << 5)
#define VLYNQ_IVEC_POL_MASK(line)       (VLYNQ_IVEC_POL_BIT_MAP << (line*8))
#define VLYNQ_IVEC_POL_VAL(val)         ((val & 0x1) << 5)
#define VLYNQ_IVEC_POL_DE_VAL(val)      ((val >>  5) & 1)

#define VLYNQ_IVEC_IRQ_BIT_MAP          (0x1f)
#define VLYNQ_IVEC_IRQ_MASK(line)       (VLYNQ_IVEC_IRQ_BIT_MAP << (line*8))
#define VLYNQ_IVEC_IRQ_VAL(val)         (val & 0x1f)
#define VLYNQ_IVEC_IRQ_DE_VAL(val)      (val & 0x1f)

#define VLYNQ_IVEC_TYPE_BIT_MAP         (1 << 6)
#define VLYNQ_IVEC_TYPE_MASK(line)      (VLYNQ_IVEC_TYPE_BIT_MAP << (line*8))
#define VLYNQ_IVEC_TYPE_VAL(val)        ((val & 0x1) << 6)
#define VLYNQ_IVEC_TYPE_DE_VAL(val)     ((val >>  6) & 1)

#define VLYNQ_IVEC_EN_BIT_MAP           (1 << 7)
#define VLYNQ_IVEC_EN_MASK(line)        (VLYNQ_IVEC_EN_BIT_MAP << (line*8))
#define VLYNQ_IVEC_EN_VAL(val)          ((val & 0x1) << 7)
#define VLYNQ_IVEC_EN_DE_VAL(val)       ((val >> 7) & 1)

#define VLYNQ_IVEC_HW_LINE_ADJUST(line) ((line > 3) ? line - 4: line)

/*-----------------------------------------------------------------------------
 * Register overlay.
 *----------------------------------------------------------------------------*/
typedef struct vlynq_rx_reg_t
{
    volatile Uint32 size;
    volatile Uint32 offset;

} VLYNQ_RX_REG_T;

/* Overlay of Vlynq 2.4 register set */
typedef struct vlynq_reg_t
{
    volatile Uint32         revision;    /* offset : 0x00 */
    volatile Uint32         control;     /* 0x04 */
    volatile Uint32         status;      /* 0x08 */
    volatile Uint32         intPriority; /* 0x0c */
    volatile Uint32         intStatus;   /* 0x10 */
    volatile Uint32         intPending;  /* 0x14 */
    volatile Uint32         intPtr;      /* 0x18 */
    volatile Uint32         txMap;       /* 0x1C */
             VLYNQ_RX_REG_T rx_set[4];
    volatile Uint32         chipVersion; /* 0x40 */
    volatile Uint32         autoNegn;    /* 0x44 */
    volatile Uint32         manualNegn;  /* 0x48 */
    volatile Uint32         negnStatus;  /* 0x4C */
    volatile Uint32         pad2[3];
    volatile Uint32         endian;      /* 0x5C */
    volatile Uint32         ivr30;       /* 0x60 */
    volatile Uint32         ivr74;       /* 0x64 */
    volatile Uint32         pad3[7];

} VLYNQ_REG_T;

/*------------------------------------------------------------------------------
 * Forward declaration of data structures.
 *----------------------------------------------------------------------------*/
typedef struct pal_vlynq_irq_map_t  PAL_VLYNQ_IRQ_MAP_T;
typedef struct pal_vlynq_irq_info_t PAL_VLYNQ_IRQ_INFO_T;
typedef struct pal_vlynq_isr_info_t PAL_VLYNQ_ISR_INFO_T;
typedef Bool                        IRQ_SWAP_FIXUP_FLAG_T;       /* A hack */

/*-----------------------------------------------------------------------------
 * The PAL VLYNQ Information. Priavate structures and API(s).
 *----------------------------------------------------------------------------*/

typedef struct pal_vlynq_region_info_t
{
    Int8               owner_dev_index;
    Bool               owner_dev_locale;

} PAL_VLYNQ_REGION_INFO_T;

typedef struct pal_vlynq_t
{
    Uint32                  base;
    Bool                    soc;
    Uint32                  vlynq_version;
    Uint32                  timeout_ms; 
    struct pal_vlynq_t      *next;
    struct pal_vlynq_t      *prev;
    PAL_VLYNQ_DEV_HND       *local_dev[MAX_DEV_PER_VLYNQ];
    PAL_VLYNQ_DEV_HND       *peer_dev[MAX_DEV_PER_VLYNQ];
    PAL_VLYNQ_REGION_INFO_T local_region_info[MAX_VLYNQ_REGION];
    PAL_VLYNQ_REGION_INFO_T remote_region_info[MAX_VLYNQ_REGION];
    PAL_VLYNQ_IRQ_MAP_T     *root_irq_map;
    PAL_VLYNQ_IRQ_INFO_T    *root_irq_info;
    PAL_VLYNQ_ISR_INFO_T    *local_isr_info;
    PAL_VLYNQ_ISR_INFO_T    *peer_isr_info;
    IRQ_SWAP_FIXUP_FLAG_T   *irq_swap_map;
    Uint32                  backup_local_cntl_word;
    Uint32                  backup_local_intr_ptr;
    Uint32                  backup_local_tx_map;
    Uint32                  backup_local_endian;
    Uint32                  backup_peer_cntl_word;
    Uint32                  backup_peer_intr_ptr;
    Uint32                  backup_peer_tx_map;
    Uint32                  backup_peer_endian;
    Int8                    local_irq;
    Int8                    peer_irq;
    Bool                    local_swap;
    Bool                    peer_swap;

} PAL_VLYNQ_T;

static PAL_VLYNQ_T pal_vlynq[MAX_VLYNQ_COUNT];
static PAL_VLYNQ_T *free_pal_vlynq_list = NULL;
static PAL_VLYNQ_T *root_pal_vlynq[MAX_ROOT_VLYNQ];

static PAL_VLYNQ_T *vlynq_get_free_vlynq(void);
static void         vlynq_free_vlynq(PAL_VLYNQ_T*);

#define LOCAL_REGS(p) (VLYNQ_REG_T*)(p_vlynq->base)
#define PEER_REGS(p)  (VLYNQ_REG_T*)(p_vlynq->base + 0x80)

static void vlynq_param_init(PAL_VLYNQ_T *p_vlynq)
{
    Int32 j;

    for(j = 0; j < MAX_DEV_PER_VLYNQ; j++)
    {
        p_vlynq->local_dev[j] = NULL;
	p_vlynq->peer_dev[j]  = NULL;
    }

    for(j = 0; j < MAX_VLYNQ_REGION; j++)
    {
	p_vlynq->local_region_info[j].owner_dev_index  = -1;
	p_vlynq->remote_region_info[j].owner_dev_index = -1;
    }

    p_vlynq->local_isr_info   = NULL;
    p_vlynq->peer_isr_info    = NULL;
    p_vlynq->root_irq_map     = NULL;
    p_vlynq->root_irq_info    = NULL;
    p_vlynq->irq_swap_map     = NULL;
    p_vlynq->prev             = NULL;
    p_vlynq->next             = NULL;
    p_vlynq->local_swap       = False;
    p_vlynq->peer_swap        = False;
}

static PAL_Result vlynq_init(void)
{
    PAL_VLYNQ_T *p_vlynq;
    Uint32 root_count       = 0, i;

    for(i = 0 ; i < MAX_VLYNQ_COUNT; i++)
    {
	p_vlynq             = &pal_vlynq[i];

        vlynq_param_init(p_vlynq);

        p_vlynq->next       = free_pal_vlynq_list;
	free_pal_vlynq_list = p_vlynq;
    }

    while(root_count < MAX_ROOT_VLYNQ)
        root_pal_vlynq[root_count++] = NULL;

    return (PAL_VLYNQ_OK);
}

static PAL_Result vlynq_add_root(PAL_VLYNQ_T *p_vlynq)
{
    Int32 i;
    PAL_Result ret = PAL_VLYNQ_INTERNAL_ERROR;

    for(i = 0; i < MAX_ROOT_VLYNQ; i++)
    {
        if(!root_pal_vlynq[i])
	{
	    root_pal_vlynq[i] = p_vlynq;
            ret = 0;
	    break;
	}
    }

    return (ret);
}

static PAL_Result vlynq_remove_root(PAL_VLYNQ_T *p_vlynq)
{
    Int32 i;
    PAL_Result ret = PAL_VLYNQ_INTERNAL_ERROR;

    for(i = 0; i < MAX_ROOT_VLYNQ; i++)
    {
        if(root_pal_vlynq[i])
	{
            if(root_pal_vlynq[i] == p_vlynq)
	    {
                ret = 0;
	        root_pal_vlynq[i] = NULL;
                break;
	    }
	}
    }

    return (ret);
}

static PAL_VLYNQ_T* vlynq_get_free_vlynq(void)
{
    PAL_VLYNQ_T *p_vlynq = free_pal_vlynq_list;

    if(p_vlynq)
    {
        free_pal_vlynq_list = free_pal_vlynq_list->next;
        p_vlynq->next = NULL;
    }

    return (p_vlynq);
}

static void vlynq_free_vlynq(PAL_VLYNQ_T* p_vlynq)
{
    vlynq_param_init(p_vlynq);

    p_vlynq->next       = free_pal_vlynq_list;
    free_pal_vlynq_list = p_vlynq;
}


static PAL_VLYNQ_T *vlynq_find_root_vlynq(PAL_VLYNQ_T *p_vlynq)
{
    while(p_vlynq->prev)
        p_vlynq = p_vlynq->prev;

    return (p_vlynq->soc ? p_vlynq : NULL); 
}

static Int32 vlynq_find_dev_index(PAL_VLYNQ_T *p_vlynq, PAL_VLYNQ_DEV_HND *vlynq_dev, 
		                  Bool *peer, Int32 *index)
{
    Int32           dev_index      = 0;
    Int32           ret_val        = PAL_VLYNQ_INTERNAL_ERROR;
    
    while(dev_index < MAX_DEV_PER_VLYNQ)
    {
        if(p_vlynq->peer_dev[dev_index] == vlynq_dev)
	{ 
	    ret_val = 0;
	    *peer   = 1; /* True */
	    break;
	}

	if(p_vlynq->local_dev[dev_index] == vlynq_dev)
	{
            ret_val = 0;
	    *peer   = 0; /* False */
	    break;
	}

	dev_index++;
    }

    *index = dev_index;

    return (ret_val);
}

/******************************************************************************
 * Interrupts galore: data structures and private functions.  
 ******************************************************************************/
#if (PAL_VLYNQ_DEV_ISR_PARM_NUM == 0)
#define PAL_VLYNQ_DEV_ISR_SIGNATURE (p)   
#elif (PAL_VLYNQ_DEV_ISR_PARM_NUM == 1)
#define PAL_VLYNQ_DEV_ISR_SIGNATURE(p) p->arg1
#elif (PAL_VLYNQ_DEV_ISR_PARM_NUM == 2)
#define PAL_VLYNQ_DEV_ISR_SIGNATURE(p) p->arg1, p->arg2
#elif (PAL_VLYNQ_DEV_ISR_PARM_NUM == 3)
#define PAL_VLYNQ_DEV_ISR_SIGNATURE(p) p->arg1, p->arg2, p->arg3
#elif (PAL_VLYNQ_DEV_ISR_PARM_NUM == 4)
#define PAL_VLYNQ_DEV_ISR_SIGNATURE(p) p->arg1, p->arg2, p->arg3, p->arg4
#elif (PAL_VLYNQ_DEV_ISR_PARM_NUM == 5)
#define PAL_VLYNQ_DEV_ISR_SIGNATURE(p) p->arg1, p->arg2, p->arg3, p->arg4, p->arg5
#elif (PAL_VLYNQ_DEV_ISR_PARM_NUM == 6)
#define PAL_VLYNQ_DEV_ISR_SIGNATURE(p) p->arg1, p->arg2, p->arg3, p->arg4, p->arg5, \
	                               p->arg6
#elif (PAL_VLYNQ_DEV_ISR_PARM_NUM == 7)
#define PAL_VLYNQ_DEV_ISR_SIGNATURE(p) p->arg1, p->arg2, p->arg3, p->arg4, p->arg5, \
	                               p->arg6, p->arg7
#elif (PAL_VLYNQ_DEV_ISR_PARM_NUM == 8)
#define PAL_VLYNQ_DEV_ISR_SIGNATURE(p) p->arg1, p->arg2, p->arg3, p->arg4, p->arg5, \
	                               p->arg6, p->arg7, p->arg8
#elif (PAL_VLYNQ_DEV_ISR_PARM_NUM == 9)
#define PAL_VLYNQ_DEV_ISR_SIGNATURE(p) p->arg1, p->arg2, p->arg3, p->arg4, p->arg5, \
	                               p->arg6, p->arg7, p->arg8, p->arg9
#elif (PAL_VLYNQ_DEV_ISR_PARM_NUM == 10)
#define PAL_VLYNQ_DEV_ISR_SIGNATURE(p) p->arg1, p->arg2, p->arg3, p->arg4, p->arg5, \
	                               p->arg6, p->arg7, p->arg8, p->arg9, p->arg10 
#else
#error "Please define the number of parameters in the ISR for the OS."
#endif

/*------------------------------------------------------------------------------
 * Endian Private functions.
 *
 * Following structure and the associated semantics with the structure is work
 * around to keep certain "endian specific" devices ticking on the VLYNQ chain.
 * Such devices can either work in "big endian" or "little endian" mode and are
 * not capable of the switching to the host specific endianness because of some
 * inherent shortcomings or design. Such devices are exceptions and are treated
 * with bare minimal support. 
 * 
 * One strong assumption that is made for a chain in which such devices are
 * present is that they should not share interrupts with any other devices in 
 * the chain.
 *----------------------------------------------------------------------------*/

/* 
 * Interprets the IRQ to the indented value as dispatched by the peer vlynq,
 * overcoming the endian mismatch. 
 */
Uint8 irq_val_swap[ ] = { 24, 25, 26, 27, 28, 29, 30, 31,
                          16, 17, 18, 19, 20, 21, 22, 23,
                           8,  9, 10, 11, 12, 13, 14, 15,  
                           0,  1,  2,  3,  4,  5,  6,  7 };

typedef struct irq_swap_fixup_flag_list_t
{
    /* Do not change the position of the first element in the structure. */
    IRQ_SWAP_FIXUP_FLAG_T             irq_swap_fixup_flag[MAX_IRQ_PER_CHAIN];
    struct irq_swap_fixup_flag_list_t *next;
    
} IRQ_SWAP_FLAG_FIXUP_LIST_T;

static IRQ_SWAP_FLAG_FIXUP_LIST_T irq_swap_flag_fixup_list[MAX_ROOT_VLYNQ];
static IRQ_SWAP_FLAG_FIXUP_LIST_T *free_irq_swap_flag_fixup_list = NULL;

static void vlynq_irq_swap_fixup_list_init(void)
{
    Int32 i = 0;
    IRQ_SWAP_FLAG_FIXUP_LIST_T *p_fixup_list;

    while(i < MAX_ROOT_VLYNQ)
    {
        Int j = 0;

        p_fixup_list                   = &irq_swap_flag_fixup_list[i];

        while(j < MAX_IRQ_PER_CHAIN)
            p_fixup_list->irq_swap_fixup_flag[j++] = False;

        p_fixup_list->next             = free_irq_swap_flag_fixup_list;
        free_irq_swap_flag_fixup_list  = p_fixup_list;
        i++;
    }
}

static IRQ_SWAP_FIXUP_FLAG_T* vlynq_get_free_irq_swap_fixup(void)
{
    IRQ_SWAP_FLAG_FIXUP_LIST_T *p_fixup_list = free_irq_swap_flag_fixup_list;

    if(p_fixup_list)
    {
        free_irq_swap_flag_fixup_list = free_irq_swap_flag_fixup_list->next;
        p_fixup_list->next            = NULL;
    }

    return ((IRQ_SWAP_FIXUP_FLAG_T*) p_fixup_list);
}

static void vlynq_free_irq_swap_fixup(IRQ_SWAP_FIXUP_FLAG_T *p_irq_swap)
{
    IRQ_SWAP_FLAG_FIXUP_LIST_T *p_fixup_list = (IRQ_SWAP_FLAG_FIXUP_LIST_T*) p_irq_swap;
    
    p_fixup_list->next = free_irq_swap_flag_fixup_list;
    free_irq_swap_flag_fixup_list = p_fixup_list;

    return;
}

static void vlynq_set_intern_irq_swap_fixup(PAL_VLYNQ_T *p_vlynq)
{
    Uint8                 irq = p_vlynq->peer_irq;
    PAL_VLYNQ_T *p_root_vlynq = vlynq_find_root_vlynq(p_vlynq);

    if (p_root_vlynq == NULL) return;

    if(p_vlynq->peer_swap == True) irq = irq_val_swap[irq];
    p_root_vlynq->irq_swap_map[irq] |= p_vlynq->peer_swap;

    irq = p_vlynq->local_irq;
    if(p_vlynq->local_swap == True) irq = irq_val_swap[irq];

    p_root_vlynq->irq_swap_map[irq] |= p_vlynq->local_swap;

    return;
}

static void vlynq_unset_intern_irq_swap_fixup(PAL_VLYNQ_T *p_vlynq)
{
    Uint8                 irq = p_vlynq->peer_irq;
    PAL_VLYNQ_T *p_root_vlynq = vlynq_find_root_vlynq(p_vlynq);

    if (p_root_vlynq == NULL) return;
    
    if(p_vlynq->peer_swap == True) irq = irq_val_swap[irq];
    p_root_vlynq->irq_swap_map[irq] = False;

    irq = p_vlynq->local_irq;
    if(p_vlynq->local_swap == True) irq = irq_val_swap[irq];
    
    p_root_vlynq->irq_swap_map[irq] = False;

    return;
}

static void vlynq_set_irq_swap_fixup(PAL_VLYNQ_T *p_vlynq, Bool peer, Uint8 irq,
                                     IRQ_SWAP_FIXUP_FLAG_T *p_irq_swap_map)
{
    switch(peer)
    {
       case True:
            if(p_vlynq->peer_swap == True) irq = irq_val_swap[irq];
            p_irq_swap_map[irq] |= p_vlynq->peer_swap;
            break;

        case False:
            if(p_vlynq->local_swap == True) irq = irq_val_swap[irq];
            p_irq_swap_map[irq] |= p_vlynq->local_swap;
	    break;

        default:
            break;
    }
    
    return;    
}

static void vlynq_unset_irq_swap_fixup(PAL_VLYNQ_T *p_vlynq, Bool peer, Uint8 irq,
                                       IRQ_SWAP_FIXUP_FLAG_T *p_irq_swap_map)
{
    switch(peer)
    {
        case True: 
            if(p_vlynq->peer_swap == True) irq = irq_val_swap[irq];
            p_irq_swap_map[irq] = False;
            break;

        case False:
            if(p_vlynq->local_swap == True) irq = irq_val_swap[irq];
            p_irq_swap_map[irq] = False;
	    break;

        default:
            break;
    }
    
    return;
}


        
Uint32 vlynq_endian_swap(Uint32 val)
{
    return ((val << 24) | (val >> 24) | ((val >> 8) & 0xff00) | ((val << 8) & 0xff0000));
}

static Bool need_swap(PAL_VLYNQ_T *p_vlynq, Bool peer)
{
    if((p_vlynq->peer_swap == True) && peer)
        return True;
    else if((p_vlynq->local_swap == True) && (!peer))
        return True;
    else
        return False;
}

static void vlynq_wr_reg(PAL_VLYNQ_T *p_vlynq, Bool peer, volatile Uint32 *reg_addr, Uint32 val)
{
    if(need_swap(p_vlynq, peer))
        val = vlynq_endian_swap(val); 

    *reg_addr = val;
}

static Uint32 vlynq_rd_reg(PAL_VLYNQ_T *p_vlynq, Bool peer, volatile Uint32 *reg_addr)
{
    Uint32 val = *reg_addr; 

    if(need_swap(p_vlynq, peer))
        val = vlynq_endian_swap(val);

    return (val); 
}

/*-----------------------------------------------------------------------------
 *   ISR Information: Takes care of user's drivers and error handling.
 *----------------------------------------------------------------------------*/
#define MAX_NUM_ISR             (TYPICAL_NUM_ISR_PER_IRQ*MAX_IRQ_PER_CHAIN)

typedef Int32 (*PAL_VLYNQ_INTERN_ISR_FN)(PAL_VLYNQ_T*);

typedef struct pal_vlynq_intern_isr_info_t
{
   /* Do not change the position of the first element in the structure. */
   PAL_VLYNQ_INTERN_ISR_FN            isr;
   PAL_VLYNQ_T                        *vlynq;
   
} PAL_VLYNQ_INTERN_ISR_INFO_T;

typedef struct pal_vlynq_dev_isr_info_t
{
    PAL_VLYNQ_DEV_ISR_FN             isr;
    PAL_VLYNQ_DEV_ISR_PARAM_GRP_T    *isr_param;
     
} PAL_VLYNQ_DEV_ISR_INFO_T;

struct pal_vlynq_isr_info_t 
{
    /* Let this be first function in this structure. */
    union one_of_isr_info_t
    {
        PAL_VLYNQ_DEV_ISR_INFO_T    dev_isr_info;
	PAL_VLYNQ_INTERN_ISR_INFO_T intern_isr_info;

    } one_of_isr_info;
    
    struct pal_vlynq_isr_info_t     *next;
    Int8                            irq_type;   /* device, internal. */

};

static PAL_VLYNQ_ISR_INFO_T vlynq_isr_info[MAX_NUM_ISR];
static PAL_VLYNQ_ISR_INFO_T *free_vlynq_isr_info_list = NULL;

static PAL_Result vlynq_isr_info_init(void)
{
    PAL_VLYNQ_ISR_INFO_T *p_isr_info; 
    Int32 i;
    
    for(i = 0; i < MAX_NUM_ISR; i++)
    {
	p_isr_info               = &vlynq_isr_info[i];
        p_isr_info->irq_type     = -1;
	p_isr_info->next         = free_vlynq_isr_info_list;
	free_vlynq_isr_info_list = p_isr_info;
    } 

    return (PAL_VLYNQ_OK);
}

static PAL_VLYNQ_ISR_INFO_T* vlynq_get_free_isr_info(void)
{
    PAL_VLYNQ_ISR_INFO_T *p_isr_info = free_vlynq_isr_info_list;

    if(p_isr_info)
    {
        free_vlynq_isr_info_list = free_vlynq_isr_info_list->next;
	p_isr_info->next         = NULL;
    }

    return (p_isr_info);
}

static void vlynq_free_isr_info(PAL_VLYNQ_ISR_INFO_T *p_isr_info)
{
/*    p_isr_info->irq          = -1; */
    p_isr_info->irq_type     = -1;
    p_isr_info->next         = free_vlynq_isr_info_list;
    free_vlynq_isr_info_list = p_isr_info;
}

/*-----------------------------------------------------------------------------
 * IRQ Information. House keeping and maintains information for multiple ISR(s).
 *---------------------------------------------------------------------------*/

struct pal_vlynq_irq_info_t
{
    PAL_VLYNQ_ISR_INFO_T  *isr_info;
    Uint32                count;
};

typedef struct pal_vlynq_irq_info_list_t
{
    /* Do not change the position of the first element in the structure. */
    PAL_VLYNQ_IRQ_INFO_T                 irq_info[MAX_IRQ_PER_CHAIN];
    struct pal_vlynq_irq_info_list_t     *next; 

} PAL_VLYNQ_IRQ_INFO_LIST_T;

static PAL_VLYNQ_IRQ_INFO_LIST_T  vlynq_irq_info_list[MAX_ROOT_VLYNQ];
static PAL_VLYNQ_IRQ_INFO_LIST_T *free_vlynq_irq_info_list = NULL;

static PAL_Result vlynq_irq_info_init(void)
{
    PAL_VLYNQ_IRQ_INFO_LIST_T *p_irq_info_list;
    PAL_VLYNQ_IRQ_INFO_T      *p_irq_info;
    Int32 i, j;

    for(i = 0; i < MAX_ROOT_VLYNQ; i++)
    {
	p_irq_info_list  = &vlynq_irq_info_list[i];
	p_irq_info       = (PAL_VLYNQ_IRQ_INFO_T*) p_irq_info_list;

        for(j = 0; j < MAX_IRQ_PER_CHAIN; j++)
        {
	    p_irq_info->isr_info = NULL;
	    p_irq_info->count    = 0; /* SSI: what about irq. ? */
	    p_irq_info++;
        }

	p_irq_info_list->next    = free_vlynq_irq_info_list;
	free_vlynq_irq_info_list = p_irq_info_list;
    }

    return (PAL_VLYNQ_OK);
}

static PAL_VLYNQ_IRQ_INFO_T *vlynq_get_free_irq_info(void)
{
    PAL_VLYNQ_IRQ_INFO_LIST_T *p_irq_info_list = free_vlynq_irq_info_list;

    if(p_irq_info_list)
    {
        free_vlynq_irq_info_list = free_vlynq_irq_info_list->next;
        p_irq_info_list->next    = NULL;
    }

    return ((PAL_VLYNQ_IRQ_INFO_T*)p_irq_info_list);
}

static void vlynq_free_irq_info(PAL_VLYNQ_IRQ_INFO_T *p_irq_info)
{
    PAL_VLYNQ_IRQ_INFO_LIST_T *p_irq_info_list = 
                             (PAL_VLYNQ_IRQ_INFO_LIST_T*)p_irq_info;
    Int32 i;

    for(i = 0; i < MAX_IRQ_PER_CHAIN; i++)
    {
        p_irq_info->isr_info = NULL;
	p_irq_info->count    = 0;
	p_irq_info++;
    }

    p_irq_info_list->next    = free_vlynq_irq_info_list;
    free_vlynq_irq_info_list = p_irq_info_list;

    return;
}

/*-----------------------------------------------------------------------------
 * IRQ Mapping: maps hw line to irq.
 *---------------------------------------------------------------------------*/
struct pal_vlynq_irq_map_t
{
    PAL_VLYNQ_T             *vlynq;
    Int8                    hw_intr_line;
    Bool                    peer; 

};

typedef struct pal_vlynq_irq_map_list_t
{ 
    /* Do not change the position of the first element in the structure. */
    PAL_VLYNQ_IRQ_MAP_T                  irq_map[MAX_IRQ_PER_CHAIN];
    struct pal_vlynq_irq_map_list_t      *next;

} PAL_VLYNQ_IRQ_MAP_LIST_T;

static PAL_VLYNQ_IRQ_MAP_LIST_T  vlynq_irq_map_list[MAX_ROOT_VLYNQ];
static PAL_VLYNQ_IRQ_MAP_LIST_T  *free_vlynq_irq_map_list = NULL;

static PAL_Result vlynq_irq_map_init(void)
{
    PAL_VLYNQ_IRQ_MAP_LIST_T *p_irq_map_list;
    PAL_VLYNQ_IRQ_MAP_T      *p_irq_map;
    Int32 i,j;

    for(i = 0; i < MAX_ROOT_VLYNQ; i++)
    {
	p_irq_map_list  = &vlynq_irq_map_list[i];
	p_irq_map       = (PAL_VLYNQ_IRQ_MAP_T*) p_irq_map_list;

        for(j = 0; j < MAX_IRQ_PER_CHAIN; j++)
        {
	    p_irq_map->vlynq        = NULL;
	    p_irq_map->hw_intr_line = -1;
	    p_irq_map++;
        }

	p_irq_map_list->next    = free_vlynq_irq_map_list;
	free_vlynq_irq_map_list = p_irq_map_list;
    }

    return (PAL_VLYNQ_OK); 
}

static PAL_VLYNQ_IRQ_MAP_T* vlynq_get_free_irq_map(void)
{
    PAL_VLYNQ_IRQ_MAP_LIST_T *p_irq_map_list = free_vlynq_irq_map_list;

    if(p_irq_map_list)
    {
        free_vlynq_irq_map_list = free_vlynq_irq_map_list->next;
        p_irq_map_list->next    = NULL;
    }

    return ((PAL_VLYNQ_IRQ_MAP_T*)p_irq_map_list); 
}

static void vlynq_free_irq_map(PAL_VLYNQ_IRQ_MAP_T *p_irq_map)
{
    PAL_VLYNQ_IRQ_MAP_LIST_T *p_irq_map_list = 
                            (PAL_VLYNQ_IRQ_MAP_LIST_T*)p_irq_map;
    Int32 i;

    for(i = 0; i < MAX_IRQ_PER_CHAIN; i++)
    {
        p_irq_map->vlynq         = NULL;
	p_irq_map->hw_intr_line  = -1;
	p_irq_map++;
    }

    p_irq_map_list->next    = free_vlynq_irq_map_list;
    free_vlynq_irq_map_list = p_irq_map_list;

    return;
}

static PAL_VLYNQ_IRQ_MAP_T* vlynq_get_map_for_irq(PAL_VLYNQ_T *p_vlynq, Uint8 irq)
{
    PAL_VLYNQ_IRQ_MAP_T *p_irq_map;
    PAL_VLYNQ_T         *p_root_vlynq = vlynq_find_root_vlynq(p_vlynq);

    if(!p_root_vlynq || !p_root_vlynq->root_irq_map)
	return (NULL);

    p_irq_map = p_root_vlynq->root_irq_map + irq;

    if(p_irq_map->hw_intr_line < 0)
        return (NULL);

    return (p_irq_map);
}

/*------------------------------------------------------------------------------------------
 * VLYNQ portal Detect Logic Begins.
 *
 * These detection logic has been implemented to take care of cascaded chain of same SoC(s).
 * Since, this SW is a generic one and target to cater all types of cascaded needs, we have
 * a little bit complex logic. Simpler logic would work as well but for configurations where
 * you may not have more than 2 instances of the same SoC(s) cascaded (pretty much next to 
 * each other.
 *----------------------------------------------------------------------------------------*/
#define CLK_DIV_TO_CTRL_REG_MAP(reg, clk) ((reg & ~(0x7 << 16)) | ((clk & 0x7) << 16)) 
#define CTRL_REG_MAP_TO_CLK_DIV(reg)      ((reg >> 16) & 0x7)

static void vlynq_sys_wr_reg(Bool swap, volatile Uint32 *reg_addr, Uint32 val)
{
    if(swap)
        val = vlynq_endian_swap(val); 

    *reg_addr = val;
}

static Uint32 vlynq_sys_rd_reg(Bool swap, volatile Uint32 *reg_addr)
{
    volatile Uint32 val = *reg_addr; 

    if(swap)
        val = vlynq_endian_swap(val);

    return (val); 
}

volatile int vlynq_delay_value_new = 0;

void vlynq_delay_wait(Uint32 count)
{
    /* Code adopted from original vlynq driver */
    int i = 0;
    volatile int    *ptr = &vlynq_delay_value_new;
    *ptr = 0;

    /* We are assuming that the each cycle takes about
     * 23 assembly instructions. */
    for(i = 0; i < (count + 23)/23; i++)
    {
        *ptr = *ptr + 1;
    }

    return;

}

static PAL_Result vlynq_detect_mapped_clk_div_equal(Uint32         peer_base,
			                            Uint32         clk_div,
					            Bool           endian_swap)
{
    /* We need to have shadow of the VLYNQ register in which clk div was set. */
    VLYNQ_REG_T *p_reg   = (VLYNQ_REG_T*) (peer_base); 
    Uint32      stat_val = vlynq_sys_rd_reg(endian_swap, &p_reg->status);
    Uint32      ctrl_val;

    if(!(stat_val & 0x1))
        return (PAL_VLYNQ_INTERNAL_ERROR);

    p_reg    = (VLYNQ_REG_T*) (peer_base + 0x80);
    ctrl_val = vlynq_sys_rd_reg(endian_swap, &p_reg->control);

    if((CTRL_REG_MAP_TO_CLK_DIV(ctrl_val) != clk_div))
        return (PAL_VLYNQ_INTERNAL_ERROR);

    return (PAL_VLYNQ_OK);
}

static PAL_Result vlynq_detect_peer_portal_when_sinking_clk(Uint32      base_addr, 
			                                    Uint32      peer_hvlynq_virt_addr, 
			                                    Uint32      peer_lvlynq_virt_addr,
						            Bool        endian_swap,
						            Uint32      *got_vlynq_virt_addr)
{
    Bool        first_selection = True;
    Uint32             clk_div  = 0x4;
    Uint32          peer_base   = peer_hvlynq_virt_addr;

    VLYNQ_REG_T        *p_reg   = (VLYNQ_REG_T*) base_addr;

    /* The algorithm:
     * [a] Set the clock div of local VLYNQ to a known value.
     * [b] Try reading the value of the clock div in the remote VLYNQ of the mapped
     *     of one of the two VLYNQ module of the peer VLYNQ SoC. 
     * [c] If the values in [a] and [b] match, then, chances are that local VLYNQ is 
     *     connected to that mapped VLYNQ module. If not try doing [b] with the other
     *     mapped VLYNQ module.
     * [d] After the first match try out [a] & [b] two more times, with unique values of 
     *     clock div between the local VLYNQ and the selected peer mapped VLYNQ module.
     */
    do
    {
	Uint32 rd_val  = vlynq_sys_rd_reg(endian_swap, &p_reg->control);
	rd_val         = CLK_DIV_TO_CTRL_REG_MAP(rd_val, clk_div);
        vlynq_sys_wr_reg(endian_swap, &p_reg->control, rd_val);
	vlynq_delay_wait(0xfff);

	rd_val         = vlynq_sys_rd_reg(endian_swap, &p_reg->status);
	if(!(rd_val & 0x1))
	    return (PAL_VLYNQ_INTERNAL_ERROR); /* No Link. */
	 
	if(first_selection)
	{
	    first_selection = 0;
	    if(!vlynq_detect_mapped_clk_div_equal(peer_base, clk_div, endian_swap))
                continue;

	    peer_base = peer_lvlynq_virt_addr;
	}

	if(vlynq_detect_mapped_clk_div_equal(peer_base, clk_div, endian_swap))
	   return (PAL_VLYNQ_INTERNAL_ERROR);

    } while (clk_div >>= 1);  

    *got_vlynq_virt_addr = peer_base;
    return (PAL_VLYNQ_OK);
}

static PAL_Result vlynq_detect_set_mapped_clk_div(Uint32          base,
					          Uint32          clk_div,
					          Bool            endian_swap)
{
    VLYNQ_REG_T *p_reg  = (VLYNQ_REG_T*) base;
    Uint32         val  = vlynq_sys_rd_reg(endian_swap, &p_reg->control);

    val = CLK_DIV_TO_CTRL_REG_MAP(val, clk_div); 
    vlynq_sys_wr_reg(endian_swap, &p_reg->control, val);
    vlynq_delay_wait(0xfff);

    return (PAL_VLYNQ_OK);
}
		   
static PAL_Result vlynq_detect_peer_portal_when_sourcing_clk(Uint32      base_addr,
			                                     Uint32      peer_hvlynq_virt_addr, 
			                                     Uint32      peer_lvlynq_virt_addr,
		                                             Bool        endian_swap,
						             Uint32      *got_vlynq_phy_addr) 
{
    Bool first_selection = True;
    Uint32 clk_div       = 0x4;
    Uint32 peer_base     = peer_hvlynq_virt_addr;

    /* The algorithm:
     * [a] Set the clock div of one of the mapped VLYNQ module to a known value.
     * [b] Try reading the value of the clock div in the local VLYNQ.
     * [c] If the values in [a] and [b] match, then, chances are that local VLYNQ is 
     *     connected to that mapped VLYNQ module. If not try doing [a] with the other
     *     mapped VLYNQ module.
     * [d] After the first match try out [a] & [b] two more times, with unique values of 
     *     clock div between the local VLYNQ and the selected peer mapped VLYNQ module.
     */
    do  
    {
        VLYNQ_REG_T *p_reg = (VLYNQ_REG_T *)base_addr;
	Uint32      rd_val;

	if(!(vlynq_sys_rd_reg(endian_swap, &p_reg->status) & 0x1))
	    return (PAL_VLYNQ_INTERNAL_ERROR);   /* No Link. */
	   
	if(vlynq_detect_set_mapped_clk_div(peer_base, clk_div, endian_swap))
	    return (PAL_VLYNQ_INTERNAL_ERROR);

        p_reg   = (VLYNQ_REG_T*)(base_addr + 0x80);

	if(first_selection)
	{
	    first_selection   = False;
	    rd_val = vlynq_sys_rd_reg(endian_swap, &p_reg->control);
	    if(CTRL_REG_MAP_TO_CLK_DIV(rd_val) == clk_div)
		continue;

	    peer_base = peer_lvlynq_virt_addr;
	    if(vlynq_detect_set_mapped_clk_div(peer_base, clk_div, endian_swap))
	        return (PAL_VLYNQ_INTERNAL_ERROR);
	}

	rd_val = vlynq_sys_rd_reg(endian_swap, &p_reg->control);
	if(CTRL_REG_MAP_TO_CLK_DIV(rd_val) != clk_div)
	    return (PAL_VLYNQ_INTERNAL_ERROR);

    } while (clk_div >>= 1);

    *got_vlynq_phy_addr = peer_base; 
    return (PAL_VLYNQ_OK);
}

PAL_Result PAL_sysVlynqDetectPeerPortal(Uint32 local_vlynq_base,      
                                        Uint32 local_tx_phy_addr,    Uint32 local_tx_virt_addr,    
				        Uint32 peer_lvlynq_phy_addr, Uint32 peer_hvlynq_phy_addr,
					Bool   endian_swap,          Uint32 *got_vlynq_addr)
{
    VLYNQ_REG_T           *p_reg = (VLYNQ_REG_T*) (local_vlynq_base);
    volatile Uint32       rd_val = vlynq_sys_rd_reg(endian_swap, &p_reg->status);  
    Bool                 clk_dir = (vlynq_sys_rd_reg(endian_swap, &p_reg->control) >> 15) & 0x1;
    Uint32    vlynq_reg_map_size = 0x100; /* 256 bytes. */
    PAL_Result           ret_val = PAL_VLYNQ_OK;
    Uint32 peer_lvlynq_virt_addr = local_tx_virt_addr;
    Uint32 peer_hvlynq_virt_addr = peer_lvlynq_virt_addr + vlynq_reg_map_size;
    Uint32 lvlynq_id, hvlynq_id; 
  
    if(!(rd_val & 0x1)) 
        return (PAL_VLYNQ_INTERNAL_ERROR); /* Could not detect any Link. */

    vlynq_sys_wr_reg(endian_swap,  &p_reg->status,          rd_val | 0x180); /* clear spur intr. */
    vlynq_sys_wr_reg(endian_swap,  &p_reg->txMap,           local_tx_phy_addr);
    
    p_reg = (VLYNQ_REG_T*) (local_vlynq_base + 0x80);

    /* vlynq_sys_wr_reg(endian_swap,  &p_reg->txMap,           0x00000000); */
    vlynq_sys_wr_reg(endian_swap,  &p_reg->rx_set[0].offset, peer_lvlynq_phy_addr);
    vlynq_sys_wr_reg(endian_swap,  &p_reg->rx_set[0].size,   vlynq_reg_map_size);
    vlynq_sys_wr_reg(endian_swap,  &p_reg->rx_set[1].offset, peer_hvlynq_phy_addr);
    vlynq_sys_wr_reg(endian_swap,  &p_reg->rx_set[1].size,   vlynq_reg_map_size);

    lvlynq_id = (vlynq_sys_rd_reg(endian_swap, (Uint32*)peer_lvlynq_virt_addr) & 0xFFFF0000) >> 16; 
    hvlynq_id = (vlynq_sys_rd_reg(endian_swap, (Uint32*)peer_hvlynq_virt_addr) & 0xFFFF0000) >> 16; 

 #define VLYNQ_MODULE_ID 0x1

    /* On many devices, associated portals do NOT master (cannot access) control modules. */ 
    if((lvlynq_id == VLYNQ_MODULE_ID) && (hvlynq_id != VLYNQ_MODULE_ID))
    {
        /* High portal mastering low control module, we are connected to high portal. */	
        *got_vlynq_addr = peer_hvlynq_phy_addr;
    }
    else if((lvlynq_id != VLYNQ_MODULE_ID) && (hvlynq_id == VLYNQ_MODULE_ID))
    {
        /* Low portal mastering high control module, we are connected to low portal. */	
	*got_vlynq_addr = peer_lvlynq_phy_addr;
    }
    else if(clk_dir) /* Since, both the controls are visible to us, let us detect the connected portal. */
    {
        ret_val = vlynq_detect_peer_portal_when_sourcing_clk(local_vlynq_base, 
			                                    peer_hvlynq_virt_addr, 
			                                    peer_lvlynq_virt_addr,
							    endian_swap,
                                                            got_vlynq_addr);

        *got_vlynq_addr = (*got_vlynq_addr == peer_lvlynq_virt_addr) ?
	                   peer_lvlynq_phy_addr : peer_hvlynq_phy_addr;
    }
    else
    {
	ret_val = vlynq_detect_peer_portal_when_sinking_clk(local_vlynq_base, 
			                                    peer_hvlynq_virt_addr,
			                                    peer_lvlynq_virt_addr,
							    endian_swap,
							    got_vlynq_addr);

        *got_vlynq_addr = (*got_vlynq_addr == peer_lvlynq_virt_addr) ?
	                   peer_lvlynq_phy_addr : peer_hvlynq_phy_addr;
    }

    vlynq_sys_wr_reg(endian_swap,  &p_reg->rx_set[0].offset, 0x00000000);
    vlynq_sys_wr_reg(endian_swap,  &p_reg->rx_set[0].size,   0x00000000);
    vlynq_sys_wr_reg(endian_swap,  &p_reg->rx_set[1].offset, 0x00000000);
    vlynq_sys_wr_reg(endian_swap,  &p_reg->rx_set[1].size,   0x00000000);

    return (ret_val);
}

static PAL_Result vlynq_sys_soft_reset(Uint32 base_addr, Bool local_swap)
{
    VLYNQ_REG_T *p_vlynq_reg = (VLYNQ_REG_T*)base_addr;
    volatile Uint32      val = vlynq_sys_rd_reg(local_swap, &p_vlynq_reg->revision);

    if(val >= 0x00010200)
    { 
	val = vlynq_sys_rd_reg(local_swap, &p_vlynq_reg->control);
        vlynq_sys_wr_reg(local_swap, &p_vlynq_reg->control, val | 0x1); 
			        
	/* Provide sufficient time for reset. Refer 2.4.2 of 2.6 VLYNQ specifications. */
	vlynq_delay_wait(0xffffff);

	val = vlynq_sys_rd_reg(local_swap, &p_vlynq_reg->control);
        vlynq_sys_wr_reg(local_swap, &p_vlynq_reg->control, val & (~0x1));
        vlynq_delay_wait(0xffffff);
    }

    return (PAL_VLYNQ_OK);
}

static PAL_Result vlynq_enable_HW_default_clock(Uint32 base_addr, Bool local_swap, Bool peer_swap)
{
    VLYNQ_REG_T *p_vlynq_reg = (VLYNQ_REG_T*)base_addr;
    int              num_try = 3;
    int            num_reset = 2;
    Uint32           clk_div;
    volatile Uint32      val;

    /* 
     * The intent here is to find the default clock directions. The qualifying
     * factor for the default clock directions would be the link detection. 
     */  
reset_sink_default_link:
    vlynq_sys_soft_reset(base_addr, local_swap);

try_sink_default_link:
    /* Lets see if have link on reset. We are sinking the clock. */     
    val  = vlynq_sys_rd_reg(local_swap, &p_vlynq_reg->status) & 0x1;
    if(val)
    {
	if(--num_try) 
	{
	    vlynq_sys_wr_reg(local_swap, &p_vlynq_reg->status, val | 0x180);
	    vlynq_delay_wait(0xffffff);
	    goto try_sink_default_link;
        }
  
	num_try = 3;
	if(--num_reset)
	    goto reset_sink_default_link;

        return(PAL_VLYNQ_OK); /* Yes, we have the link. */
    }

    /* So, we did not find default clock direction as yet. Let us set the clock
     * in the source mode.
     */
    for(clk_div = 4; clk_div < 9; clk_div++)
    {
	num_try   = 3;
	num_reset = 2;

reset_source_default_link:
	vlynq_sys_soft_reset(base_addr, local_swap);

	/* Set up a reducing frequency to detect the link in each iteration. */
        val  = vlynq_sys_rd_reg(local_swap, &p_vlynq_reg->control);
        val |=  (0x1 << 15);
        val &= ~(0x7 << 16);
        val |=  ((clk_div - 1) << 16);
        vlynq_sys_wr_reg(local_swap, &p_vlynq_reg->control, val);  

try_source_default_link:
	vlynq_delay_wait(0xffffff);

        val  = vlynq_sys_rd_reg(local_swap, &p_vlynq_reg->status) & 0x1;
        if(val)
	{
            if(--num_try)
	    {
		vlynq_sys_wr_reg(local_swap, &p_vlynq_reg->status, val | 0x180);
                goto try_source_default_link;
            }

	    num_try = 3;
	    if(--num_reset)
                goto reset_source_default_link;

            return(PAL_VLYNQ_OK); /* Yes, we have the link. */
	} 
        /* Not yet, let us try getting the link at a lower frequency. */
    }

    return (PAL_VLYNQ_INTERNAL_ERROR);
}

PAL_Result PAL_sysVlynqDetectPeer(Uint32 vlynq_base_addr, Bool endian_swap, 
		                  Uint32 *rev_word, Uint32 *cnt_word)
{
    VLYNQ_REG_T *p_reg = (VLYNQ_REG_T*)(vlynq_base_addr);

    if(vlynq_enable_HW_default_clock(vlynq_base_addr, endian_swap, endian_swap))
        return (PAL_VLYNQ_INTERNAL_ERROR);
    
    *cnt_word = vlynq_sys_rd_reg(endian_swap, &p_reg->control);

    p_reg = (VLYNQ_REG_T*)(vlynq_base_addr + 0x80);
    *rev_word = vlynq_sys_rd_reg(endian_swap, &p_reg->chipVersion);

    return (PAL_VLYNQ_OK); 
}

/*------------------------------------------------------------------------------------------
 * VLYNQ portal Detect Logic Ends.
 *----------------------------------------------------------------------------------------*/

PAL_Result PAL_sysVlynqInit(void)
{
    /* SSI, need to fill up. */
    vlynq_isr_info_init();
    vlynq_irq_info_init();
    vlynq_irq_map_init();
    vlynq_irq_swap_fixup_list_init();
    vlynq_init();

    pal_vlynq_dev_init();

    return (PAL_VLYNQ_OK);
}


static PAL_Result vlynq_add_intern_isr(PAL_VLYNQ_T *p_root_vlynq, Uint8 irq, 
	   	                       PAL_VLYNQ_ISR_INFO_T *p_isr_info)
{
    PAL_VLYNQ_IRQ_INFO_T *p_irq_info  = p_root_vlynq->root_irq_info + irq;

    p_isr_info->next     = p_irq_info->isr_info;
    p_irq_info->isr_info = p_isr_info;

    return (PAL_VLYNQ_OK);
}

static PAL_Result vlynq_remove_intern_isr(PAL_VLYNQ_T *p_root_vlynq, Uint8 irq,
		                          PAL_VLYNQ_ISR_INFO_T *isr_info)
{
    PAL_VLYNQ_IRQ_INFO_T *p_irq_info      = p_root_vlynq->root_irq_info + irq;
    PAL_VLYNQ_ISR_INFO_T *p_isr_info      = p_irq_info->isr_info;
    PAL_VLYNQ_ISR_INFO_T *p_last_isr_info = NULL;
    PAL_Result           ret_val          = PAL_VLYNQ_INTERNAL_ERROR;

    for(p_isr_info      = p_irq_info->isr_info;
        p_isr_info     != NULL;
	p_last_isr_info = p_isr_info, p_isr_info = p_isr_info->next)
    {
        if(p_isr_info == isr_info)
	{
	    if(!p_last_isr_info)
	        p_irq_info->isr_info  = p_last_isr_info;
	    else
	      p_last_isr_info->next = p_isr_info->next;

	    ret_val = PAL_VLYNQ_OK;
	    break;
	}
    }

    return (ret_val);
}

PAL_Result PAL_vlynqAddDevice(PAL_VLYNQ_HND *vlynq, PAL_VLYNQ_DEV_HND *vlynq_dev,
	   			       Bool peer)
{
    PAL_VLYNQ_T     *p_vlynq       = (PAL_VLYNQ_T*) vlynq;
    Int32           dev_count      = MAX_DEV_PER_VLYNQ;
    PAL_Result      ret_val        = PAL_VLYNQ_INTERNAL_ERROR;
    PAL_VLYNQ_DEV_HND **pp_vlynq_dev;

    if(!vlynq || !vlynq_dev)
        return  (PAL_VLYNQ_INVALID_PARAM);

    pp_vlynq_dev = (PAL_VLYNQ_DEV_HND**)
                   (peer ? &p_vlynq->peer_dev : &p_vlynq->local_dev);

    while(dev_count--)
    {
        if (!(*pp_vlynq_dev))
	{
            *pp_vlynq_dev = (PAL_VLYNQ_DEV_HND*) vlynq_dev;
	    ret_val       = PAL_VLYNQ_OK;
	    break; 
	}
	else
	    pp_vlynq_dev++;
    }

    return (ret_val);
}

PAL_Result PAL_vlynqRemoveDevice(PAL_VLYNQ_HND *vlynq, PAL_VLYNQ_DEV_HND *vlynq_dev)
{
    PAL_VLYNQ_T     *p_vlynq       = (PAL_VLYNQ_T*) vlynq;
    PAL_Result      ret_val        = PAL_VLYNQ_OK;
    Bool            peer;
    Int32           index;
    
    PAL_VLYNQ_DEV_HND **pp_vlynq_dev;

    if(!vlynq || !vlynq_dev)
        return  (PAL_VLYNQ_INVALID_PARAM);

    if(vlynq_find_dev_index(p_vlynq, vlynq_dev, &peer, &index))
        return (PAL_VLYNQ_INTERNAL_ERROR);

    pp_vlynq_dev = (PAL_VLYNQ_DEV_HND**)
                   (peer ? &p_vlynq->peer_dev : &p_vlynq->local_dev);

    pp_vlynq_dev[index] = NULL;

    return (ret_val);
}

static Bool vlynq_detect_link_status(PAL_VLYNQ_T *p_vlynq)
{
    VLYNQ_REG_T *p_reg = LOCAL_REGS(p_vlynq);

    return ((vlynq_rd_reg(p_vlynq, False, &p_reg->status) & 0x1) ? True: False);
}

PAL_Result PAL_vlynqUnMapRegion(PAL_VLYNQ_HND *vlynq, Bool remote, Uint32 region_id, 
				   PAL_VLYNQ_DEV_HND *vlynq_dev)
{
    PAL_VLYNQ_DEV_HND *p_vlynq_dev = vlynq_dev;
    PAL_VLYNQ_T       *p_vlynq     = vlynq;

    Bool                    peer;
    Uint32                  index;
    PAL_VLYNQ_REGION_INFO_T *p_region, *p_local_region, *p_remote_region;
    VLYNQ_RX_REG_T          *p_rx_reg;
    VLYNQ_REG_T             *p_remote_reg, *p_local_reg;

    if(!p_vlynq_dev || !p_vlynq)
        return  (PAL_VLYNQ_INVALID_PARAM);

    if(vlynq_find_dev_index(p_vlynq, p_vlynq_dev, &peer, &index))
        return (PAL_VLYNQ_INTERNAL_ERROR);

    if(region_id > 3)
	return  (PAL_VLYNQ_INVALID_PARAM);

    p_local_region  = p_vlynq->local_region_info  + region_id;
    p_remote_region = p_vlynq->remote_region_info + region_id;
    
    /* HPVL - To take care of scenario where peer is powered down before
     * powering down root, check the link during cleanup before accessing remote
     * registers. TODO: There may be lot of such places to handle!
     * Note: Still return success as it doesn't matter and it ensures complete
     * de-init sequence is followed to resume software state.
     */
    if(!vlynq_detect_link_status(p_vlynq))
        return (PAL_VLYNQ_OK);
    
    p_local_reg     = LOCAL_REGS(p_vlynq);
    p_remote_reg    = PEER_REGS(p_vlynq);

    /* 
     * local vlynq <----------------bus/remoteness--------------> peer vlynq
     *
     * Interpretation has to be made in the perspective of the device and its
     * location i.e. association of the device with the local vlynq or peer 
     * vlynq.
     *
     * A true value for 'remote' means that a device wants to export the 
     * local memory to other devices, otherwise, the device intends to import 
     * portion of external memory into it.
     *
     * A device connected to 'peer' vlynq exports memory region by programming 
     * peer vlynq rx registers and imports memory region by programming the 
     * rx registers of local vlynq.  
     *
     * A device connected to local vlynq exports memory region by programming
     * local vlynq rx registers and imports memory region by programming the
     * rx registers of peer vlynq.
     *
     * Complex eh ? Yep, seems so.
     */ 
    if(peer)
    {
        p_region   = remote ? p_remote_region : p_local_region;
	p_rx_reg   = remote ? p_remote_reg->rx_set : p_local_reg->rx_set; 
	
    }
    else
    {
	p_region   = remote ? p_local_region : p_remote_region;
	p_rx_reg   = remote ? p_local_reg->rx_set : p_remote_reg->rx_set; 
	
    }

    if(p_region->owner_dev_index == -1)
        return (PAL_VLYNQ_INTERNAL_ERROR); /* Region Not allocated. */

    p_rx_reg = p_rx_reg + region_id;
    p_rx_reg->size   = 0;
    p_rx_reg->offset = 0;

    p_region->owner_dev_locale = peer;
    p_region->owner_dev_index  = -1;

    return (PAL_VLYNQ_OK); 
}

PAL_Result PAL_vlynqMapRegion(PAL_VLYNQ_HND *vlynq, Bool remote, Uint32 region_id, 
		                Uint32 region_offset, Uint32 region_size, 
				PAL_VLYNQ_DEV_HND *vlynq_dev)
{
    PAL_VLYNQ_DEV_HND *p_vlynq_dev = vlynq_dev;
    PAL_VLYNQ_T       *p_vlynq     = vlynq;

    Bool                    peer;
    Uint32                  index;
    PAL_VLYNQ_REGION_INFO_T *p_region, *p_local_region, *p_remote_region;
    VLYNQ_RX_REG_T          *p_rx_reg;
    VLYNQ_REG_T             *p_remote_reg, *p_local_reg;
    Bool                     peer_reg_flag;

    if(!p_vlynq_dev || !p_vlynq)
        return  (PAL_VLYNQ_INVALID_PARAM);

    if(vlynq_find_dev_index(p_vlynq, p_vlynq_dev, &peer, &index))
        return (PAL_VLYNQ_INTERNAL_ERROR);

    if(region_id > 3)
	return  (PAL_VLYNQ_INVALID_PARAM);

    p_local_region  = p_vlynq->local_region_info  + region_id;
    p_remote_region = p_vlynq->remote_region_info + region_id;
    
    p_local_reg     = LOCAL_REGS(p_vlynq);
    p_remote_reg    = PEER_REGS(p_vlynq);

    /* 
     * local vlynq <----------------bus/remoteness--------------> peer vlynq
     *
     * Interpretation has to be made in the perspective of the device and its
     * location i.e. association of the device with the local vlynq or peer 
     * vlynq.
     *
     * A true value for 'remote' means that a device wants to export the 
     * local memory to other devices, otherwise, the device intends to import 
     * portion of external memory into it.
     *
     * A device connected to 'peer' vlynq exports memory region by programming 
     * peer vlynq rx registers and imports memory region by programming the 
     * rx registers of local vlynq.  
     *
     * A device connected to local vlynq exports memory region by programming
     * local vlynq rx registers and imports memory region by programming the
     * rx registers of peer vlynq.
     *
     * Complex eh ? Yep, seems so.
     */ 
    if(peer)
    {
        p_region   = remote ? p_remote_region : p_local_region;
	p_rx_reg   = remote ? p_remote_reg->rx_set : p_local_reg->rx_set; 
	peer_reg_flag = remote ? True : False;
    }
    else
    {
	p_region   = remote ? p_local_region : p_remote_region;
	p_rx_reg   = remote ? p_local_reg->rx_set : p_remote_reg->rx_set; 
	peer_reg_flag = remote ? False: True;
    }

    if(p_region->owner_dev_index != -1)
    {
        return (PAL_VLYNQ_INTERNAL_ERROR); /* Region already allocated. */
    }

    p_rx_reg = p_rx_reg + region_id;
    vlynq_wr_reg(p_vlynq, peer_reg_flag, &p_rx_reg->size,   region_size);
    vlynq_wr_reg(p_vlynq, peer_reg_flag, &p_rx_reg->offset, region_offset);

    p_region->owner_dev_locale = peer;
    p_region->owner_dev_index  = index;

    return (PAL_VLYNQ_OK); 
}


static PAL_Result vlynq_calculate_base_addr(PAL_VLYNQ_T *p_vlynq, Uint32 offset, 
                                            Uint32 *base_addr, Uint32 index)
{
    VLYNQ_REG_T    *p_local_reg = LOCAL_REGS(p_vlynq);
    VLYNQ_REG_T    *p_vlynq_reg = PEER_REGS(p_vlynq);
    VLYNQ_RX_REG_T *p_rx_reg    = p_vlynq_reg->rx_set;
    Uint32 i, offset_diff, size = 0;
    PAL_VLYNQ_REGION_INFO_T *p_remote_region = p_vlynq->remote_region_info;

    for(i = 0; i < MAX_VLYNQ_REGION; i++)
    {
#if 0
        if(p_remote_region->owner_dev_index != index)
        {
            return (PAL_VLYNQ_INTERNAL_ERROR);
        }
#endif
 
        offset_diff = offset - vlynq_rd_reg(p_vlynq, True, &p_rx_reg[i].offset);

	if(offset_diff < 0)
            return (PAL_VLYNQ_INTERNAL_ERROR);

	if(offset_diff <= vlynq_rd_reg(p_vlynq, True, &p_rx_reg[i].size))
	{
            *base_addr = offset_diff + size + vlynq_rd_reg(p_vlynq, False, &p_local_reg->txMap);
	    break; 
	}

	size += vlynq_rd_reg(p_vlynq, True, &p_rx_reg[i].size);

        p_remote_region++;
    }

    return (PAL_VLYNQ_OK);
}


PAL_Result PAL_vlynqGetDevBase(PAL_VLYNQ_HND     *vlynq,
		               Uint32            offset,
			       Uint32            *base_addr,
			       PAL_VLYNQ_DEV_HND *vlynq_dev)
{
    PAL_VLYNQ_DEV_HND        *p_vlynq_dev = vlynq_dev;
    PAL_VLYNQ_T              *p_vlynq     = vlynq;
    Uint32                   index;
    Bool                     peer;
    Int32                    ret_val = 0;

    if(!p_vlynq || !p_vlynq_dev)
        return  (PAL_VLYNQ_INVALID_PARAM);

    if(vlynq_find_dev_index(p_vlynq, p_vlynq_dev, &peer, &index))
	return (PAL_VLYNQ_INTERNAL_ERROR);

    /* 
     * Not sure why would one require to calculate base address for the 
     * device which is not associated with the peer VLYNQ.
     */
#if 1
    if(!peer)
	return (PAL_VLYNQ_INTERNAL_ERROR);
#endif

    /* Let us now calculate the base address back up to the root. */
    while(p_vlynq)
    {
        ret_val = vlynq_calculate_base_addr(p_vlynq, offset, base_addr, index);
	if(ret_val)
	    break;

        offset  = *base_addr;
	p_vlynq = p_vlynq->prev;
    } 

    return (ret_val);
}

/******************************************************************************
 * VLYNQ Configuration API(s): Private ones.
 *****************************************************************************/
static PAL_Result vlynq_validate_param_and_find_ivr_info(PAL_VLYNQ_T *p_vlynq, 
	  	                                         Uint32 irq, volatile Uint32 **p_reg,
						         Uint8  *p_hw_intr_line)
{
    PAL_VLYNQ_IRQ_MAP_T   *p_irq_map;
    VLYNQ_REG_T           *p_vlynq_reg;

    if(!p_vlynq)
        return (PAL_VLYNQ_INVALID_PARAM);

    if(irq > 31)
	return (PAL_VLYNQ_INVALID_PARAM);

    p_irq_map = vlynq_get_map_for_irq(p_vlynq, irq);
    if(!p_irq_map)
	return (PAL_VLYNQ_INTERNAL_ERROR);

    p_vlynq_reg    = (p_irq_map->peer == True) ? PEER_REGS(p_vlynq): LOCAL_REGS(p_vlynq);
    *p_hw_intr_line = p_irq_map->hw_intr_line;

    *p_reg = (p_irq_map->hw_intr_line < 4) ? &p_vlynq_reg->ivr30 : &p_vlynq_reg->ivr74; 

    return (PAL_VLYNQ_OK);
}

static PAL_Result vlynq_root_isr_local_error(PAL_VLYNQ_T *p_vlynq)
{
    VLYNQ_REG_T *p_reg          = LOCAL_REGS(p_vlynq);
    PAL_VLYNQ_DEV_HND **pp_dev  = p_vlynq->local_dev;
    volatile Uint32       val   = vlynq_rd_reg(p_vlynq, False, &p_reg->status);
    Int32                    i  = 0;
 
    val |= 0x80;
    vlynq_wr_reg(p_vlynq, False, &p_reg->status, val); 

    while(i++ < MAX_DEV_PER_VLYNQ)
        pal_vlynq_dev_handle_event(*pp_dev++, PAL_VLYNQ_EVENT_LOCAL_ERROR, val); 

    return (PAL_VLYNQ_OK); 
}

static PAL_Result vlynq_root_isr_peer_error(PAL_VLYNQ_T *p_vlynq)
{
    VLYNQ_REG_T     *p_reg     = LOCAL_REGS(p_vlynq);
    PAL_VLYNQ_DEV_HND **pp_dev = p_vlynq->peer_dev;
    volatile Uint32        val = vlynq_rd_reg(p_vlynq, False, &p_reg->status);
    Int32                    i = 0;

    val |= 0x100;
    vlynq_wr_reg(p_vlynq, False, &p_reg->status, val); 

    while(i++ < MAX_DEV_PER_VLYNQ)
        pal_vlynq_dev_handle_event(*pp_dev++, PAL_VLYNQ_EVENT_PEER_ERROR, val);

    return (PAL_VLYNQ_OK);
}

static PAL_Result vlynq_validate_init_params(PAL_VLYNQ_CONFIG_T *pal_vlynq_config)
{
   if(!pal_vlynq_config)
        return (PAL_VLYNQ_INVALID_PARAM);

    /* Both peers of the VLYNQ bridge can not be sourcing. */
    if(pal_vlynq_config->local_clock_dir  == pal_vlynq_clk_out &&
       pal_vlynq_config->peer_clock_dir   == pal_vlynq_clk_out)
        return (PAL_VLYNQ_INVALID_PARAM);

    /* We can not accept zero divisors. */
    if(pal_vlynq_config->local_clock_div == 0 ||
       pal_vlynq_config->peer_clock_div  == 0)
        return (PAL_VLYNQ_INVALID_PARAM);

    /* This vlynq has to handle irqs locally and peer should 
     * be sending the irq to the local VLYNQ. */
    if(!pal_vlynq_config->local_intr_local  ||
        pal_vlynq_config->peer_intr_local)
        return (PAL_VLYNQ_INVALID_PARAM);

    /* Non-on chip VLYNQ(s) have to write to a specific
     * register to get the interrupts to the root vlynq. */ 
    if(pal_vlynq_config->on_soc          && 
       !pal_vlynq_config->local_int2cfg)
        return (PAL_VLYNQ_INVALID_PARAM); 

    /* Validate the clock divisor values. */
    if(pal_vlynq_config->local_clock_div > 8 ||
       pal_vlynq_config->peer_clock_div  > 8)
        return (PAL_VLYNQ_INVALID_PARAM);

    /* Validate the interrupt vector values. */
    if(pal_vlynq_config->local_intr_vector > 31 ||
       pal_vlynq_config->peer_intr_vector  > 31)
        return (PAL_VLYNQ_INVALID_PARAM); 

    return (PAL_VLYNQ_OK);
}

static Bool vlynq_touch_vlynq_through_chain(PAL_VLYNQ_T *p_vlynq)
{
    /* 
     * We need to go back to the root vlynq to ensure that the link exists from 
     * the root to the vlynq under consideration i.e. p_vlynq.
     * 
     * If we find any of the intermediate links as failed, then we cannot 
     * access the p_vlynq and we return that the link is down. 
     *
     * This shall avoid a possible system hang.
     */

     PAL_VLYNQ_T *p_temp_vlynq   = vlynq_find_root_vlynq(p_vlynq);
     Bool link_status = 0;
     
     while(p_temp_vlynq)
     {
         if(p_temp_vlynq == p_vlynq)
         {
             link_status = vlynq_detect_link_status(p_vlynq);
             break;
         }
 
         p_temp_vlynq = p_temp_vlynq->next;
     }

     return (link_status);
}

Bool PAL_vlynqGetLinkStatus(PAL_VLYNQ_HND *vlynq)
{
    Bool ret_val;

    ret_val = vlynq_touch_vlynq_through_chain((PAL_VLYNQ_T*)vlynq);

    return (ret_val);
}

static PAL_Result vlynq_config_bridge_endianness(PAL_VLYNQ_T *p_vlynq,
                                                 PAL_VLYNQ_ENDIAN_ENUM_T local_endianness,
                                                 PAL_VLYNQ_ENDIAN_ENUM_T peer_endianness)
{
    /* The following implementation has been hacked for 1350, the correct state
     * ment should be (... < 0x00010205) instead (... <= 0x00010206). */

    VLYNQ_REG_T *p_reg = LOCAL_REGS(p_vlynq);
     
    if((vlynq_rd_reg(p_vlynq, False, &p_reg->revision) >= 0x00010200) && 
       (vlynq_rd_reg(p_vlynq, False, &p_reg->revision) <= 0x00010206) &&  
       local_endianness)
    {
        p_reg->endian = local_endianness;
    }

    p_reg = PEER_REGS(p_vlynq);

    if((vlynq_rd_reg(p_vlynq, True, &p_reg->revision) >= 0x00010200) && 
       (vlynq_rd_reg(p_vlynq, True, &p_reg->revision) <= 0x00010206) && 
       peer_endianness)
    {
        p_reg->endian = peer_endianness;
    }

    if(local_endianness != peer_endianness)
	p_vlynq->peer_swap = (p_vlynq->peer_swap == True) ? False : True;

    return (PAL_VLYNQ_OK);
}

static void vlynq_wr_config_clock(PAL_VLYNQ_T                *p_vlynq,
                                  VLYNQ_REG_T                *p_vlynq_reg,
                                  Bool                       flag,
	  	                  PAL_VLYNQ_CLOCK_DIR_ENUM_T clock_dir,
                                  Uint8                      clock_div)
{
    volatile Uint32 val;

    val  = vlynq_rd_reg(p_vlynq, flag, &p_vlynq_reg->control);
    val &= ~(0x1 << 15);
    val &= ~(0x7 << 16);
    val |=  ((clock_div - 1) << 16) | (clock_dir << 15);
    vlynq_wr_reg(p_vlynq, flag, &p_vlynq_reg->control, val);
 
    vlynq_delay_wait(0xffffff);
}
                                     
static PAL_Result vlynq_config_bridge_clock(PAL_VLYNQ_T                *p_vlynq,
	  	                            PAL_VLYNQ_CLOCK_DIR_ENUM_T local_clock_dir,
		                            PAL_VLYNQ_CLOCK_DIR_ENUM_T peer_clock_dir,
				            Uint8                      local_clock_div,
				            Uint8                      peer_clock_div)
{
    VLYNQ_REG_T *p_vlynq_reg;

    /* Clock configuration is hardware driven. We do some basic configuration
     * to aid the hardware setup. Lest, the software configuration is aligned
     * with the hardware setup, expect the ultimate, "the hang". Read on.....
     */

    if(local_clock_dir == pal_vlynq_clk_out)
    {
        p_vlynq_reg = LOCAL_REGS(p_vlynq);
        vlynq_wr_config_clock(p_vlynq, p_vlynq_reg, False, 
                              local_clock_dir, local_clock_div);

        /* We must have the Link at this stage. We have to go over the link for
         * peer clock.*/
        if(!vlynq_detect_link_status(p_vlynq))
            return (PAL_VLYNQ_INTERNAL_ERROR);

        p_vlynq_reg = PEER_REGS(p_vlynq);
        vlynq_wr_config_clock(p_vlynq, p_vlynq_reg, True, 
                              peer_clock_dir, peer_clock_div);
    } 
    else
    {
        /* We must have the Link at this stage. We have to go over the link for
         * peer clock.*/
        if(!vlynq_detect_link_status(p_vlynq))
            return (PAL_VLYNQ_INTERNAL_ERROR);

        p_vlynq_reg = PEER_REGS(p_vlynq);
        vlynq_wr_config_clock(p_vlynq, p_vlynq_reg, True, 
                              peer_clock_dir, peer_clock_div);

        p_vlynq_reg = LOCAL_REGS(p_vlynq);
        vlynq_wr_config_clock(p_vlynq, p_vlynq_reg, False, 
                              local_clock_dir, local_clock_div);
    }

    /* We must have the Link at this stage */
    if(!vlynq_detect_link_status(p_vlynq))
       return (PAL_VLYNQ_INTERNAL_ERROR);

    return (PAL_VLYNQ_OK);
}
		                     
static PAL_Result vlynq_uninstall_intern_isr(PAL_VLYNQ_T *p_vlynq)
{
    PAL_VLYNQ_ISR_INFO_T *p_isr_info;
    VLYNQ_REG_T *p_reg = PEER_REGS(p_vlynq);

    /* HPVL - To take care of scenario where peer is powered down before
     * powering down root, check the link during cleanup before accessing remote
     * registers. TODO: There may be lot of such places to handle!
     */
    if(vlynq_detect_link_status(p_vlynq))
    {
        vlynq_wr_reg(p_vlynq, True, &p_reg->control,
                vlynq_rd_reg(p_vlynq, True, &p_reg->control) & (~0x7f80));
    }
   
    if(p_vlynq->peer_isr_info)
    {
        p_isr_info = p_vlynq->peer_isr_info;
        p_isr_info->one_of_isr_info.intern_isr_info.isr   = NULL;
        p_isr_info->one_of_isr_info.intern_isr_info.vlynq = NULL;
    
	vlynq_free_isr_info(p_isr_info);
    }

    /* HPVL - To take care of scenario where peer is powered down before
     * powering down root, check the link during cleanup before accessing remote
     * registers. TODO: There may be lot of such places to handle!
     * TODO: Check if this really should be PEER_REGS below? Either of the above
     * or below should be LOCAL_REGS.
     */
    if(vlynq_detect_link_status(p_vlynq))
    {
        p_reg = PEER_REGS(p_vlynq);

        vlynq_wr_reg(p_vlynq, False, &p_reg->control,
                vlynq_rd_reg(p_vlynq, False, &p_reg->control) & (~0x7f80));
    }
    
    if(p_vlynq->local_isr_info)
    {
        p_isr_info = p_vlynq->local_isr_info;
        p_isr_info->one_of_isr_info.intern_isr_info.isr   = NULL;
        p_isr_info->one_of_isr_info.intern_isr_info.vlynq = NULL;

        vlynq_free_isr_info(p_isr_info);
    }

    return (PAL_VLYNQ_OK);
}

static PAL_VLYNQ_ISR_INFO_T *vlynq_config_intern_error_intr(PAL_VLYNQ_T *p_vlynq,
	                                                    Uint8       intr_local,
					                    Uint8       intr_enable,
					                    Uint8       intr_vec,
					                    Uint8       int2cfg,
						            Uint32      intr_pointer,
					                    Bool        peer)
{
    PAL_VLYNQ_ISR_INFO_T *p_isr_info;
    volatile Uint32 val = 0;
    VLYNQ_REG_T  *p_reg = (peer == True)? PEER_REGS(p_vlynq) : LOCAL_REGS(p_vlynq);

    p_isr_info = vlynq_get_free_isr_info();
    if(!p_isr_info)
        return (NULL);

    if(peer)
        p_isr_info->one_of_isr_info.intern_isr_info.isr = vlynq_root_isr_peer_error;
    else
        p_isr_info->one_of_isr_info.intern_isr_info.isr = vlynq_root_isr_local_error;

    p_isr_info->one_of_isr_info.intern_isr_info.vlynq = p_vlynq;
    p_isr_info->irq_type                              = 0x1;

    if(int2cfg)
        vlynq_wr_reg(p_vlynq, peer, &p_reg->intPtr, 0x14);
    else
        vlynq_wr_reg(p_vlynq, peer, &p_reg->intPtr, intr_pointer);

    val  = vlynq_rd_reg(p_vlynq, peer, &p_reg->control);
    val &= ~(0x7f80); 
    val |= intr_local  << 14;
    val |= intr_enable << 13;
    val |= intr_vec    <<  8;
    val |= int2cfg     <<  7;
    vlynq_wr_reg(p_vlynq, peer, &p_reg->control, val);

    return (p_isr_info);
}

static PAL_Result vlynq_install_intern_isr(PAL_VLYNQ_T *p_vlynq, 
	  	                           PAL_VLYNQ_CONFIG_T* p_vlynq_config)
{
    PAL_VLYNQ_ISR_INFO_T        *p_intern_isr   = NULL;
    PAL_Result                        ret_val   = 0;

    p_intern_isr = vlynq_config_intern_error_intr(p_vlynq,
	                                          p_vlynq_config->local_intr_local,
					          p_vlynq_config->local_intr_enable,
					          p_vlynq_config->local_intr_vector,
					          p_vlynq_config->local_int2cfg,
					          p_vlynq_config->local_intr_pointer,
					          0);
    if(!p_intern_isr)
        goto vlynq_install_intern_isr_local_error;

    p_vlynq->local_isr_info = p_intern_isr;
    p_vlynq->local_irq      = p_vlynq_config->local_intr_vector;

    p_intern_isr = vlynq_config_intern_error_intr(p_vlynq,
	                                          p_vlynq_config->peer_intr_local,
					          p_vlynq_config->peer_intr_enable,
					          p_vlynq_config->peer_intr_vector,
					          p_vlynq_config->peer_int2cfg,
					          p_vlynq_config->peer_intr_pointer,
					          1);
    if(!p_intern_isr)
        goto vlynq_install_intern_isr_peer_error;

    p_vlynq->peer_isr_info = p_intern_isr;
    p_vlynq->peer_irq      = p_vlynq_config->peer_intr_vector;

    return (ret_val);

vlynq_install_intern_isr_peer_error:
    vlynq_free_isr_info(p_vlynq->local_isr_info);
    p_vlynq->local_isr_info = NULL;

vlynq_install_intern_isr_local_error:
    ret_val = PAL_VLYNQ_INTERNAL_ERROR;

    return (ret_val);
}

Int32 PAL_vlynqGetNumRoot(void)
{
    Int32 root_count = 0, index = 0;

    while(index < MAX_ROOT_VLYNQ)
        if(root_pal_vlynq[index++])
	    root_count++;
    
    return (root_count);
}

PAL_VLYNQ_HND* PAL_vlynqGetRoot(Int32 index)
{
    PAL_VLYNQ_T *p_vlynq = NULL;
    
    if(index < MAX_ROOT_VLYNQ)
        p_vlynq = root_pal_vlynq[index];

    return ((PAL_VLYNQ_HND*) p_vlynq);
}

PAL_VLYNQ_HND* PAL_vlynqGetNext(PAL_VLYNQ_HND *this)
{
    PAL_VLYNQ_T *p_vlynq = (PAL_VLYNQ_T*) this;

    if(p_vlynq)
        p_vlynq = p_vlynq->next;

    return ((PAL_VLYNQ_HND*)p_vlynq);
}

Int32 PAL_vlynqIsLast(PAL_VLYNQ_HND *this)
{
    PAL_VLYNQ_T *p_vlynq = (PAL_VLYNQ_T*) this;
    Int32 ret_val = PAL_VLYNQ_INTERNAL_ERROR;

    if(p_vlynq)
        ret_val = p_vlynq->next ? 0 : 1;

    return (ret_val);
}

Int32 PAL_vlynqGetChainLength(PAL_VLYNQ_HND *this)
{
    PAL_VLYNQ_T *p_vlynq;
    Int32 length;
    
    for(p_vlynq  = (PAL_VLYNQ_T*)this, length = 0; 
	p_vlynq != NULL; 
	p_vlynq  = p_vlynq->next)
        length++;

    return (this ? length : -1);
}

PAL_Result PAL_vlynqGetBaseAddr(PAL_VLYNQ_HND *vlynq, Uint32 *base)
{
    PAL_VLYNQ_T *p_vlynq = vlynq;

    if(!p_vlynq)
        return  (PAL_VLYNQ_INVALID_PARAM);

    *base = p_vlynq->base; 
   
    return (PAL_VLYNQ_OK);
}

PAL_VLYNQ_HND *PAL_vlynqGetRootAtBase(Uint32 base_addr)
{
    PAL_VLYNQ_T *p_ret_vlynq = NULL;
    Int32 i;

    for(i = 0; i < MAX_ROOT_VLYNQ; i++)
    {
        if(root_pal_vlynq[i])
        {
            if(root_pal_vlynq[i]->base == base_addr)
            {
                p_ret_vlynq = root_pal_vlynq[i];
                break;
            }
        }
    }

    return (p_ret_vlynq);
}

PAL_VLYNQ_HND *PAL_vlynqGetRootVlynq(PAL_VLYNQ_HND *vlynq)
{
    if(vlynq)
        return (vlynq_find_root_vlynq(vlynq));

    return (NULL);
}

PAL_Result PAL_vlynqChainAppend(PAL_VLYNQ_HND *this, PAL_VLYNQ_HND *to)
{
    PAL_VLYNQ_T *this_vlynq = (PAL_VLYNQ_T*) this;
    PAL_VLYNQ_T *to_vlynq   = (PAL_VLYNQ_T*) to;
    PAL_VLYNQ_T *root_vlynq = NULL;
/*   PAL_VLYNQ_IRQ_MAP_T *p_irq_map; */

    if(!this_vlynq || !to_vlynq)
        return (PAL_VLYNQ_INVALID_PARAM);

    if(to_vlynq->next != NULL)
        return (PAL_VLYNQ_INVALID_PARAM);

    root_vlynq = vlynq_find_root_vlynq(to_vlynq);
    if(!root_vlynq || !root_vlynq->root_irq_info)
        return (PAL_VLYNQ_INTERNAL_ERROR);

    /* Load the local isr information into the root vlynq. */
    vlynq_add_intern_isr(root_vlynq, this_vlynq->local_irq, 
		         this_vlynq->local_isr_info);

    /* Load the peer isr information into the root vlynq. */
    vlynq_add_intern_isr(root_vlynq, this_vlynq->peer_irq, 
		         this_vlynq->peer_isr_info);

    to_vlynq->next   = this_vlynq;
    this_vlynq->prev = to_vlynq;

    if(to_vlynq->peer_swap == True) this_vlynq->local_swap = True;

    vlynq_set_intern_irq_swap_fixup(this_vlynq);

    return (PAL_VLYNQ_OK); 
}

PAL_Result PAL_vlynqChainUnAppend(PAL_VLYNQ_HND *this, PAL_VLYNQ_HND *from)
{
    PAL_VLYNQ_T *this_vlynq = (PAL_VLYNQ_T*) this;
    PAL_VLYNQ_T *from_vlynq = (PAL_VLYNQ_T*) from;
    PAL_VLYNQ_T *root_vlynq;

    if(!this_vlynq || !from_vlynq)
        return  (PAL_VLYNQ_INVALID_PARAM);    /* NULL input pointers */

    if(this_vlynq->next)
        return (PAL_VLYNQ_INVALID_PARAM);

    if(from_vlynq->next != this_vlynq)
        return (PAL_VLYNQ_INTERNAL_ERROR);

    root_vlynq = vlynq_find_root_vlynq(from_vlynq);
    if(!root_vlynq || !root_vlynq->root_irq_info)
        return (PAL_VLYNQ_INTERNAL_ERROR);

    vlynq_unset_intern_irq_swap_fixup(this_vlynq);

    vlynq_remove_intern_isr(root_vlynq, this_vlynq->local_irq,
		            this_vlynq->local_isr_info);

    vlynq_remove_intern_isr(root_vlynq, this_vlynq->peer_irq,
		            this_vlynq->peer_isr_info);

    from_vlynq->next       = this_vlynq->next;
    this_vlynq->prev       = NULL; 
 
    return (PAL_VLYNQ_OK); 
}

PAL_Result PAL_vlynqMapIrq(PAL_VLYNQ_HND *vlynq, Uint32 hw_intr_line,
		             Uint32 irq, PAL_VLYNQ_DEV_HND *vlynq_dev)
{
    PAL_VLYNQ_DEV_HND    *p_vlynq_dev = (PAL_VLYNQ_DEV_HND*) vlynq_dev;
    PAL_VLYNQ_T          *p_vlynq     = vlynq, *p_root_vlynq;
    PAL_VLYNQ_IRQ_MAP_T  *p_irq_map;
    volatile Uint32      *p_reg, val;
    VLYNQ_REG_T          *p_vlynq_reg;
    Int32    index;
    Bool     peer;

    if(!p_vlynq_dev || !p_vlynq)
        return  (PAL_VLYNQ_INVALID_PARAM);

    if(irq > 31 || hw_intr_line > 7)
        return  (PAL_VLYNQ_INVALID_PARAM);

    /* Has this irq been already mapped. */
    if(vlynq_get_map_for_irq((PAL_VLYNQ_T*)vlynq, irq))
        return (PAL_VLYNQ_INTERNAL_ERROR);

    p_root_vlynq = vlynq_find_root_vlynq((PAL_VLYNQ_T*)vlynq);
    if(!p_root_vlynq || !p_root_vlynq->root_irq_map)
        return (PAL_VLYNQ_INTERNAL_ERROR);

    p_irq_map = p_root_vlynq->root_irq_map + irq;

    if(vlynq_find_dev_index(p_vlynq, p_vlynq_dev, &peer, &index))
        return (PAL_VLYNQ_INTERNAL_ERROR);

    /* Add information into the device. */
    if(pal_vlynq_dev_ioctl(p_vlynq_dev, PAL_VLYNQ_DEV_ADD_IRQ, irq))
	return (PAL_VLYNQ_INTERNAL_ERROR);

    /* Update the VLYNQ hardware. */
    p_vlynq_reg = (peer == True) ? PEER_REGS(p_vlynq): LOCAL_REGS(p_vlynq);
    
    p_reg = (hw_intr_line < 4) ? &p_vlynq_reg->ivr30 : &p_vlynq_reg->ivr74; 

    p_irq_map->hw_intr_line = hw_intr_line;
    p_irq_map->vlynq        = p_vlynq;
    p_irq_map->peer         = peer;

    hw_intr_line  =  VLYNQ_IVEC_HW_LINE_ADJUST(hw_intr_line);

    val  = vlynq_rd_reg(p_vlynq, peer, p_reg);
    val &= ~VLYNQ_IVEC_IRQ_MASK(hw_intr_line);
    val |= VLYNQ_IVEC_IRQ_VAL(irq) << VLYNQ_IVEC_OFFSET(hw_intr_line);    
    vlynq_wr_reg(p_vlynq, peer, p_reg, val);

    vlynq_set_irq_swap_fixup(p_vlynq, peer, irq, p_root_vlynq->irq_swap_map);

    return (PAL_VLYNQ_OK);
}

PAL_Result PAL_vlynqUnMapIrq(PAL_VLYNQ_HND *vlynq, Uint32 irq, 
		             PAL_VLYNQ_DEV_HND *vlynq_dev)
{
    PAL_VLYNQ_DEV_HND     *p_vlynq_dev  = (PAL_VLYNQ_DEV_HND*) vlynq_dev;
    PAL_VLYNQ_T           *p_vlynq      = (PAL_VLYNQ_T*) vlynq;
    PAL_VLYNQ_IRQ_MAP_T   *p_irq_map;
    PAL_VLYNQ_T           *p_root_vlynq = NULL;
    Int32 index;
    Bool  peer;

    if(!p_vlynq_dev || !p_vlynq)
        return  (PAL_VLYNQ_INVALID_PARAM);

    if(irq > 31 )
        return (PAL_VLYNQ_INVALID_PARAM);

    p_irq_map = vlynq_get_map_for_irq(p_vlynq, irq);
    if(!p_irq_map)
        return (PAL_VLYNQ_INTERNAL_ERROR);

    /* Was it mapped by this VLYNQ. */
    if(p_irq_map->vlynq != vlynq)
	return (PAL_VLYNQ_INTERNAL_ERROR);

    p_root_vlynq = vlynq_find_root_vlynq(p_vlynq);
    if(!p_root_vlynq)
        return (PAL_VLYNQ_INTERNAL_ERROR);

    /* Still have ISR attached. */
    if((p_root_vlynq->root_irq_info[irq].isr_info))
        return (PAL_VLYNQ_INTERNAL_ERROR);

    if(vlynq_find_dev_index(p_vlynq, p_vlynq_dev, &peer, &index))
        return (PAL_VLYNQ_INTERNAL_ERROR);

    /* Add information into the device. */
    if(pal_vlynq_dev_ioctl(p_vlynq_dev, PAL_VLYNQ_DEV_REMOVE_IRQ, irq))
	return (PAL_VLYNQ_INTERNAL_ERROR);

    p_irq_map->hw_intr_line = -1;    
    p_irq_map->vlynq       = NULL;

    vlynq_unset_irq_swap_fixup(p_vlynq, peer, irq, p_root_vlynq->irq_swap_map);    

    return (PAL_VLYNQ_OK); 
}

PAL_Result PAL_vlynqAddIsr(PAL_VLYNQ_HND *vlynq, Uint32 irq,
		             PAL_VLYNQ_DEV_ISR_FN  dev_isr,
			     PAL_VLYNQ_DEV_ISR_PARAM_GRP_T* isr_param)
{
    PAL_VLYNQ_T            *p_root_vlynq;
    PAL_VLYNQ_ISR_INFO_T   *p_isr_info;
    Uint32                 cookie;

    if(!vlynq || !dev_isr || !isr_param)
        return  (PAL_VLYNQ_INVALID_PARAM);

    if(irq > 31)
        return (PAL_VLYNQ_INVALID_PARAM);

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);

    /* Ascertain whether the IRQ has been already mapped. */
    if(!vlynq_get_map_for_irq((PAL_VLYNQ_T*)vlynq, irq))
    {
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
        return (PAL_VLYNQ_INTERNAL_ERROR);
    }

    p_root_vlynq = vlynq_find_root_vlynq((PAL_VLYNQ_T*)vlynq);
    if(!p_root_vlynq || !p_root_vlynq->root_irq_info)
    {
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
        return (PAL_VLYNQ_INTERNAL_ERROR);
    }
    
    /* Get a place holder for ISR information. */
    p_isr_info = vlynq_get_free_isr_info();
    if(!p_isr_info)
    {
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
        return (PAL_VLYNQ_INTERNAL_ERROR);
    }

    /* House keeping; you see. */
    p_isr_info->one_of_isr_info.dev_isr_info.isr       = dev_isr;
    p_isr_info->one_of_isr_info.dev_isr_info.isr_param = isr_param;
    p_isr_info->irq_type                               = 0x0;

    p_isr_info->next = p_root_vlynq->root_irq_info[irq].isr_info;
    p_root_vlynq->root_irq_info[irq].isr_info = p_isr_info;

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return (PAL_VLYNQ_OK);
}

PAL_Result PAL_vlynqRemoveIsr(PAL_VLYNQ_HND *vlynq, Uint32 irq,
		              PAL_VLYNQ_DEV_ISR_PARAM_GRP_T *isr_param)
{
    PAL_VLYNQ_T              *p_root_vlynq;
    PAL_VLYNQ_ISR_INFO_T     *p_isr_info, *p_last_isr_info;
    Uint32                   cookie;

    if(!vlynq || !isr_param)
        return  (PAL_VLYNQ_INVALID_PARAM);

    if(irq > 31)
	return (PAL_VLYNQ_INVALID_PARAM);

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);

    p_root_vlynq = vlynq_find_root_vlynq((PAL_VLYNQ_T*)vlynq);
    if(!p_root_vlynq || !p_root_vlynq->root_irq_info)
    {
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
	return (PAL_VLYNQ_INTERNAL_ERROR);
    }

    p_isr_info = p_root_vlynq->root_irq_info[irq].isr_info;

    for(p_last_isr_info = NULL;
	p_isr_info != NULL;
        p_last_isr_info = p_isr_info, p_isr_info = p_isr_info->next)
    {
        PAL_VLYNQ_DEV_ISR_INFO_T *p_dev_isr_info = 
                                 (PAL_VLYNQ_DEV_ISR_INFO_T*)p_isr_info;

        if(p_dev_isr_info->isr_param == isr_param)
	{
            if(!p_last_isr_info)
	        p_root_vlynq->root_irq_info[irq].isr_info = NULL;
	    else
		p_last_isr_info->next = p_isr_info->next;

	    vlynq_free_isr_info(p_isr_info);

            PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

	    return (PAL_VLYNQ_OK);
	}	
    }	    

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return (PAL_VLYNQ_INTERNAL_ERROR);
}



PAL_Result PAL_vlynqRootIsr(PAL_VLYNQ_HND *vlynq)
{
    Int32 count, irq = 0; 

    PAL_VLYNQ_T *p_vlynq = (PAL_VLYNQ_T*)vlynq;

    VLYNQ_REG_T *p_vlynq_reg = LOCAL_REGS(p_vlynq);
    register volatile Uint32 irq_status_map    = p_vlynq_reg->intStatus;
    PAL_VLYNQ_ISR_INFO_T        *p_vlynq_isr_info;
    PAL_VLYNQ_IRQ_INFO_T        *p_vlynq_irq_info;
    PAL_VLYNQ_DEV_ISR_INFO_T    *p_dev_isr_info;
    PAL_VLYNQ_INTERN_ISR_INFO_T *p_intern_isr_info;

    /* No more than 32 consecutive interrupts for VLYNQ; lest it thrashes
     * the system; safety measures you see. 
     */
    for(count = 0; count < 32; count++)
    {
        if(p_vlynq_reg->revision >= 0x00010200)                
        {                                        
            Uint32 val = p_vlynq_reg->intPriority;          

            irq        = val & 0x1f;

	    if(val & 0x80000000)
                break;	
        }	    
        else                                                    
        { 
	    Uint8 val = irq_status_map & 1;
            if(!irq_status_map)
            {
	        break;
            }
            
	    irq_status_map >>= 1;
	    if(val)
            {
	        irq = count;
            }
	    else
		continue;
	}

	p_vlynq_reg->intStatus |= (1 << irq);

	if(p_vlynq->irq_swap_map[irq] == True)
	    irq = irq_val_swap[irq];

        p_vlynq_irq_info  = p_vlynq->root_irq_info + irq;
        p_vlynq_isr_info  = p_vlynq_irq_info->isr_info;

        p_vlynq_irq_info->count++;

        while(p_vlynq_isr_info)
        {
            p_dev_isr_info    = (PAL_VLYNQ_DEV_ISR_INFO_T*)p_vlynq_isr_info;
            p_intern_isr_info = (PAL_VLYNQ_INTERN_ISR_INFO_T*)p_vlynq_isr_info;

            if(!p_vlynq_isr_info->irq_type)
                p_dev_isr_info->isr(PAL_VLYNQ_DEV_ISR_SIGNATURE(p_dev_isr_info->isr_param));
            else
	        p_intern_isr_info->isr(p_intern_isr_info->vlynq);

	    p_vlynq_isr_info = p_vlynq_isr_info->next;
        }
    }

    return (PAL_VLYNQ_OK);
}

PAL_Result PAL_vlynqGetIrqCount(PAL_VLYNQ_HND *vlynq, Uint32 irq, Uint32 *count)
{
    PAL_VLYNQ_T           *p_root_vlynq;
    PAL_VLYNQ_IRQ_INFO_T  *p_irq_info; 

    if(!vlynq)
        return (PAL_VLYNQ_INVALID_PARAM);

    if(irq > 31)
	return (PAL_VLYNQ_INVALID_PARAM);

    p_root_vlynq = vlynq_find_root_vlynq((PAL_VLYNQ_T*)vlynq);
    if(!p_root_vlynq || !p_root_vlynq->root_irq_info)
      return (PAL_VLYNQ_INTERNAL_ERROR);

    p_irq_info = p_root_vlynq->root_irq_info + irq;
    *count     = p_irq_info->count;

    return (PAL_VLYNQ_OK); 
}

PAL_VLYNQ_HND* PAL_vlynqGetForIrq(PAL_VLYNQ_HND *root, Uint32 irq)
{ 
    PAL_VLYNQ_IRQ_MAP_T *p_irq_map = NULL;

    if(!root || irq > 31)
	return (NULL);

    p_irq_map = vlynq_get_map_for_irq(root, irq);
    
    return (p_irq_map ? p_irq_map->vlynq : NULL); 
}

PAL_Result PAL_vlynqSetIrqPol(PAL_VLYNQ_HND *vlynq, Uint32 irq,
		              PAL_VLYNQ_IRQ_POL_ENUM_T pol)
{
    Int8             hw_intr_line;
    volatile Uint32  *p_reg;
    PAL_Result       ret_val;
    volatile Uint32  val;
   
    ret_val = vlynq_validate_param_and_find_ivr_info((PAL_VLYNQ_T*) vlynq,
		                                     irq, &p_reg, 
						     &hw_intr_line);
    if(ret_val)
        return (ret_val);

    hw_intr_line  = VLYNQ_IVEC_HW_LINE_ADJUST(hw_intr_line);

    val  = vlynq_rd_reg((PAL_VLYNQ_T*)vlynq, True, p_reg);
    val &= ~VLYNQ_IVEC_POL_MASK(hw_intr_line);
    val |= VLYNQ_IVEC_POL_VAL(pol) << VLYNQ_IVEC_OFFSET(hw_intr_line);
    vlynq_wr_reg((PAL_VLYNQ_T*)vlynq, True, p_reg, val);

    return (PAL_VLYNQ_OK);
}

PAL_Result PAL_vlynqGetIrqPol(PAL_VLYNQ_HND *vlynq, Uint32 irq,
		              PAL_VLYNQ_IRQ_POL_ENUM_T *pol)
{
    Int8            hw_intr_line;
    volatile Uint32 *p_reg;
    PAL_Result      ret_val;
    volatile Uint32  val;
   
    ret_val = vlynq_validate_param_and_find_ivr_info((PAL_VLYNQ_T*) vlynq,
		                                     irq, &p_reg, 
						     &hw_intr_line);
    if(ret_val)
        return (ret_val); 

    hw_intr_line = VLYNQ_IVEC_HW_LINE_ADJUST(hw_intr_line);
 
    val  = vlynq_rd_reg((PAL_VLYNQ_T*) vlynq, True, p_reg);
    *pol = val & VLYNQ_IVEC_POL_MASK(hw_intr_line);
    *pol = (*pol) >> VLYNQ_IVEC_OFFSET(hw_intr_line);
    *pol = VLYNQ_IVEC_POL_DE_VAL(*pol);

    return (PAL_VLYNQ_OK);
}

PAL_Result PAL_vlynqSetIrqType(PAL_VLYNQ_HND *vlynq, Uint32 irq,
		                  PAL_VLYNQ_IRQ_TYPE_ENUM_T type)
{
    Int8                 hw_intr_line;
    volatile Uint32      *p_reg;
    PAL_Result           ret_val;
    volatile Uint32      val;
   
    ret_val = vlynq_validate_param_and_find_ivr_info((PAL_VLYNQ_T*) vlynq,
		                                     irq, &p_reg, 
						     &hw_intr_line);
    if(ret_val)
        return (ret_val);

    hw_intr_line = VLYNQ_IVEC_HW_LINE_ADJUST(hw_intr_line);

    val  = vlynq_rd_reg((PAL_VLYNQ_T*) vlynq, True, p_reg);
    val &= ~(VLYNQ_IVEC_TYPE_MASK(hw_intr_line));
    val |= VLYNQ_IVEC_TYPE_VAL(type) << VLYNQ_IVEC_OFFSET(hw_intr_line);
    vlynq_wr_reg((PAL_VLYNQ_T*) vlynq, True, p_reg, val);

    return (PAL_VLYNQ_OK);
}

PAL_Result PAL_vlynqGetIrqType(PAL_VLYNQ_HND *vlynq, Uint32 irq,
		               PAL_VLYNQ_IRQ_TYPE_ENUM_T *type)
{
    Int8                 hw_intr_line;
    volatile Uint32      *p_reg;
    PAL_Result           ret_val;
    volatile Uint32      val;
   
    ret_val = vlynq_validate_param_and_find_ivr_info((PAL_VLYNQ_T*) vlynq,
		                                     irq, &p_reg, 
						     &hw_intr_line);
    if(ret_val)
        return (ret_val);
    
    hw_intr_line = VLYNQ_IVEC_HW_LINE_ADJUST(hw_intr_line);

    val   = vlynq_rd_reg((PAL_VLYNQ_T*) vlynq, True, p_reg);
    *type = val & VLYNQ_IVEC_TYPE_MASK(hw_intr_line);
    *type = (*type) >> VLYNQ_IVEC_OFFSET(hw_intr_line);
    *type = VLYNQ_IVEC_TYPE_DE_VAL(*type);

    return (PAL_VLYNQ_OK);
}

PAL_Result PAL_vlynqDisableIrq(PAL_VLYNQ_HND *vlynq, Uint32 irq)
{
    Int8                 hw_intr_line;
    volatile Uint32      *p_reg;
    PAL_Result           ret_val;
    volatile Uint32      val; 
    
    ret_val = vlynq_validate_param_and_find_ivr_info((PAL_VLYNQ_T*) vlynq,
		                                     irq, &p_reg, 
						     &hw_intr_line);
    if(ret_val)
        return (ret_val);

    hw_intr_line = VLYNQ_IVEC_HW_LINE_ADJUST(hw_intr_line);

    val  = vlynq_rd_reg((PAL_VLYNQ_T*) vlynq, True, p_reg);
    val &= ~(VLYNQ_IVEC_EN_MASK(hw_intr_line));
    vlynq_wr_reg((PAL_VLYNQ_T*) vlynq, True, p_reg, val);
    
    return (PAL_VLYNQ_OK);
}

PAL_Result PAL_vlynqEnableIrq(PAL_VLYNQ_HND *vlynq, Uint32 irq)
{
    Int8                 hw_intr_line;
    volatile Uint32      *p_reg;
    PAL_Result           ret_val;
    volatile Uint32      val;
   
    ret_val = vlynq_validate_param_and_find_ivr_info((PAL_VLYNQ_T*) vlynq,
		                                     irq, &p_reg, 
						     &hw_intr_line);
    if(ret_val)
        return (ret_val);

    hw_intr_line = VLYNQ_IVEC_HW_LINE_ADJUST(hw_intr_line);

    val  = vlynq_rd_reg((PAL_VLYNQ_T*) vlynq, True, p_reg);
    val |= VLYNQ_IVEC_EN_VAL(1) << VLYNQ_IVEC_OFFSET(hw_intr_line);
    vlynq_wr_reg((PAL_VLYNQ_T*) vlynq, True, p_reg, val);

    return (PAL_VLYNQ_OK);
}

static void vlynq_soft_reset(PAL_VLYNQ_T *p_vlynq)
{
    VLYNQ_REG_T *p_reg = LOCAL_REGS(p_vlynq);

    if(vlynq_rd_reg(p_vlynq, False, &p_reg->revision) >= 0x00010200)
    { 
	vlynq_wr_reg(p_vlynq, False, &p_reg->control, 
		     vlynq_rd_reg(p_vlynq, False, &p_reg->control) | 0x1);
       
        /* Provide sufficient time for reset. Refer 2.4.2 of 2.6 VLYNQ specifications. */
        vlynq_delay_wait(0xffffff);	

	vlynq_wr_reg(p_vlynq, False, &p_reg->control,
		     vlynq_rd_reg(p_vlynq, False, &p_reg->control) & (~0x1));
    }
}

static PAL_Result vlynq_config_RTM(PAL_VLYNQ_T               *p_vlynq,
				   Bool                      peer,
                                   PAL_VLYNQ_RTM_CFG_ENUM_T  rtm_cfg_type,
                                   Uint8                     rtm_sample_value,
                                   Bool                      tx_fast_path)
{
    /* Let us get on with the RTM configuration first. We will follow it up with the 
       TX Fast path configuration. */

    /* Refer to Section 2.4.2 and 4.3.4 of 2.6 VLYNQ specifications for clarifications on 
     * the configuration aspects of the RTM and Fast Path. It seems there is a window in 
     * the hardware state machine, under which the configuration of the RTM and Fast Path
     * can cause hang. It is recommended in the specifications not to set the values when
     * link or auto negotiation is going on the VLYNQ bridge. 
     *
     * We should be here once, the link has been established after sufficient reset time,
     * It is still ok to configure the local VLYNQ because we can configure it and check
     * for the link. For the peer VLYNQ, we must have the link to configure on the peer
     * RTM and if link goes down because of the RTM configuration on the peer VLYNQ, then
     * be ready to emberace the ultimate "the hang".
     */
    volatile Uint32 val;
    VLYNQ_REG_T   *p_reg = (peer == True) ? LOCAL_REGS(p_vlynq) : PEER_REGS(p_vlynq); 
    
#define RTM_ENABLE_BIT         0x1

    if(rtm_cfg_type == rtm_auto_select_sample_val)
    {
	val = vlynq_rd_reg(p_vlynq, peer, &p_reg->control);
        val |= (RTM_ENABLE_BIT << 22);
	vlynq_wr_reg(p_vlynq, peer, &p_reg->control, val);
    }
    else if(rtm_cfg_type == rtm_force_sample_val)
    {   
        if(rtm_sample_value > 0x7)
	    return (PAL_VLYNQ_INVALID_PARAM);
 
#define RTM_SAMPLE_VAL_MASK    0x7

        /* Clear RTM enable bit & RTM sample value */    
        val = vlynq_rd_reg(p_vlynq, peer, &p_reg->control);	
        val &= ~((RTM_ENABLE_BIT << 22) | (RTM_SAMPLE_VAL_MASK << 24)); 

#define RTM_FORCE_VAL_WR       0x1

        val |= (RTM_FORCE_VAL_WR << 23) | (rtm_sample_value << 24); /* Populate the new sample value. */
	vlynq_wr_reg(p_vlynq, peer, &p_reg->control, val);
    }
    else
    {
        ; /* Nothing for now. We did not mess the RTM configuration. */
    }

#define TX_FAST_PATH       0x1

    val = vlynq_rd_reg(p_vlynq, peer, &p_reg->control);
    val &= ~(TX_FAST_PATH << 21);    /* Clear the value of the fast path */
    val |= (tx_fast_path << 21);     /* Set the new value. */
    vlynq_wr_reg(p_vlynq, peer, &p_reg->control, val);

    return (PAL_VLYNQ_OK);
}

static PAL_Result vlynq_config_bridge_RTM(PAL_VLYNQ_T                    *p_vlynq,
                                          PAL_VLYNQ_RTM_CFG_ENUM_T       local_rtm_cfg_type,
                                          Uint8                          local_sample_value,
                                          Bool                           local_tx_fast_path,
                                          PAL_VLYNQ_RTM_CFG_ENUM_T       peer_rtm_cfg_type,
                                          Uint8                          peer_sample_value,
                                          Bool                           peer_tx_fast_path)
{
    PAL_Result    ret_val = PAL_VLYNQ_OK;
    VLYNQ_REG_T   *p_reg  = LOCAL_REGS(p_vlynq);

#define RTM_STATUS_BIT    0x1

    if((p_reg->revision >= 0x00010205) && (p_reg->status & (RTM_STATUS_BIT << 11)))
    {
        ret_val = vlynq_config_RTM(p_vlynq, False, local_rtm_cfg_type, local_sample_value, local_tx_fast_path);

        if(ret_val != PAL_VLYNQ_OK)
            return (ret_val);

        /* Provide time for the bus to settle down. */
        vlynq_delay_wait(0xffffff);
    }

    /* We must have the Link at this stage after we played with the RTM. */
    if(!vlynq_detect_link_status(p_vlynq))
        return (PAL_VLYNQ_INTERNAL_ERROR);
        
    p_reg = PEER_REGS(p_vlynq);

    if((p_reg->revision >= 0x00010205) && (p_reg->status & (RTM_STATUS_BIT << 11)))
    {
        ret_val = vlynq_config_RTM(p_vlynq, True, peer_rtm_cfg_type, peer_sample_value, peer_tx_fast_path);

        /* Provide time for the bus to settle down. */
        vlynq_delay_wait(0xffffff);
    }
    
    return (ret_val);
}                              
                         
PAL_Result PAL_vlynqConfigClock(PAL_VLYNQ_HND                *p_vlynq,
	  	                PAL_VLYNQ_CLOCK_DIR_ENUM_T   local_clock_dir,
		                PAL_VLYNQ_CLOCK_DIR_ENUM_T   peer_clock_dir,
				Uint8                        local_clock_div,
				Uint8                        peer_clock_div)
{
    PAL_Result ret_val; 

    if(!p_vlynq)
        return (PAL_VLYNQ_INVALID_PARAM);

    /* Valid clock directions: Either of the peers to source the clock or both
     * of the peers to sink in external board clock.
     */
    if(local_clock_dir == pal_vlynq_clk_out &&
       peer_clock_dir  == pal_vlynq_clk_out)
        return  (PAL_VLYNQ_INVALID_PARAM);
   
    if((local_clock_div == 0) ||
       (peer_clock_div  == 0))
        return (PAL_VLYNQ_INVALID_PARAM);

    if((local_clock_div > 8) ||
       (peer_clock_div  > 8))
        return (PAL_VLYNQ_INVALID_PARAM);

    ret_val = vlynq_config_bridge_clock(p_vlynq, local_clock_dir, peer_clock_dir,
                                        local_clock_div, peer_clock_div);

    return (ret_val);
}


PAL_VLYNQ_HND *PAL_vlynqInit(PAL_VLYNQ_CONFIG_T *pal_vlynq_config)
{
    PAL_VLYNQ_T                 *p_vlynq        = NULL;
    VLYNQ_REG_T                 *p_reg          = NULL;
    PAL_VLYNQ_IRQ_MAP_T         *p_root_irq_map = NULL;
    PAL_VLYNQ_IRQ_INFO_T        *p_root_irq_info= NULL;
    IRQ_SWAP_FIXUP_FLAG_T       *p_irq_swap_map = NULL;
    Uint32                      val             = 0;

    if(vlynq_validate_init_params(pal_vlynq_config))
    {
        pal_vlynq_config->error_status = pal_vlynq_init_invalid_param;
        strcpy(pal_vlynq_config->error_msg, "Invalid init params.");
        return (NULL); 
    }
  
    /* We are done with the param validation and are
     * set for configuring the hardware. Here we go...*/
    p_vlynq = vlynq_get_free_vlynq();
    if(!p_vlynq)
    {
        pal_vlynq_config->error_status = pal_vlynq_init_no_mem;
        strcpy(pal_vlynq_config->error_msg, "Mem alloc failed for VLYNQ.");
        return (NULL);
    }

    /* Let us give some more time for the VLYNQ to settle down after the 
     * hardware reset. 
     */
    vlynq_delay_wait(0xffff);

    p_vlynq->base         = pal_vlynq_config->base_addr;

    if(pal_vlynq_config->init_swap_flag)
    {
        p_vlynq->local_swap = True;
        p_vlynq->peer_swap  = True;
    }

    /* Put the vlynq in reset, if the version is more than 
     * 2.0 for more than 256 serial cycles. Finally bring it out 
     * of reset. */
    vlynq_soft_reset(p_vlynq);

    /* Provide sufficient time for reset to settle down. Refer 2.4.2 of 2.6 VLYNQ specifications. */
    vlynq_delay_wait(0xffffff);	
    
    /* Do we have the link on the VLYNQ bridge on reset. We must have the
     * link on reset. It indicates that the hw state machine has  settled
     * down and hw clocks are working fine. This is must when we are sink
     * ing the clock. */
    if((pal_vlynq_config->local_clock_dir == pal_vlynq_clk_in) && 
       !vlynq_detect_link_status(p_vlynq))
    {
        pal_vlynq_config->error_status = pal_vlynq_init_no_link_on_reset;
        strcpy(pal_vlynq_config->error_msg, "No link detected on reset.");
	goto pal_vlynq_init_link_error_1;
    }

    if(vlynq_config_bridge_clock(p_vlynq, 
			         pal_vlynq_config->local_clock_dir,
				 pal_vlynq_config->peer_clock_dir,
				 pal_vlynq_config->local_clock_div,
				 pal_vlynq_config->peer_clock_div))
    {
	    
	pal_vlynq_config->error_status =  pal_vlynq_init_clk_cfg;
	strcpy(pal_vlynq_config->error_msg, "Error in configuring clocks.");
        goto pal_vlynq_init_clock_config_error;
    }

    /* Let us wait for the Link to settle down. */
    vlynq_delay_wait(0xffffff);

    /* We have just updated the clock, let us check the health of the link.
     * We must have the link, if not it is over for us. */
    if(!vlynq_detect_link_status(p_vlynq))
    {
	pal_vlynq_config->error_status = pal_vlynq_init_no_link_on_clk_cfg;
	strcpy(pal_vlynq_config->error_msg, "No link detected post clk config.");
	goto pal_vlynq_init_link_error_2;
    }

    /* We must clear the spur interrupts. */
    p_reg            = PEER_REGS(p_vlynq);
    vlynq_wr_reg(p_vlynq, True, &p_reg->status, 
		 vlynq_rd_reg(p_vlynq, True, &p_reg->status) | 0x180);

        
    p_reg            = LOCAL_REGS(p_vlynq);
    vlynq_wr_reg(p_vlynq, False, &p_reg->status, 
                 vlynq_rd_reg(p_vlynq, False, &p_reg->status) | 0x180);

    /* Are we equipped enough to support the VLYNQ bridge ? */
    p_reg         = PEER_REGS(p_vlynq);
    if(vlynq_rd_reg(p_vlynq, True, &p_reg->revision) > LATEST_VLYNQ_REV)
    {    
        pal_vlynq_config->error_status = pal_vlynq_init_peer_high_rev;
        strcpy(pal_vlynq_config->error_msg, "Peer VLYNQ revision higher than SW.");
        goto pal_vlynq_init_peer_rev_error;
    }

    p_reg         = LOCAL_REGS(p_vlynq);
    if(vlynq_rd_reg(p_vlynq, False, &p_reg->revision) > LATEST_VLYNQ_REV)
    {    
        pal_vlynq_config->error_status = pal_vlynq_init_local_high_rev;
        strcpy(pal_vlynq_config->error_msg, "Local VLYNQ revision higher than SW.");
        goto pal_vlynq_init_local_rev_error;
    }

    /* Let us configure the RTM for VLYNQ 2.5 and higher. */
    if(vlynq_config_bridge_RTM(p_vlynq, 
                               pal_vlynq_config->local_rtm_cfg_type, 
                               pal_vlynq_config->local_rtm_sample_value,
                               pal_vlynq_config->local_tx_fast_path,
                               pal_vlynq_config->peer_rtm_cfg_type,
                               pal_vlynq_config->peer_rtm_sample_value,
                               pal_vlynq_config->peer_tx_fast_path))
    {
        pal_vlynq_config->error_status = pal_vlynq_init_rtm_cfg;
        strcpy(pal_vlynq_config->error_msg, "Error in configuring RTM.");
        goto pal_vlynq_init_rtm_config_error;
    }

    /* We have just updated the RTM, let us check the health of the link.
     * We must have the link, if not it is over for us. */
    if(!vlynq_detect_link_status(p_vlynq))
    {
	pal_vlynq_config->error_status = pal_vlynq_init_no_link_on_rtm_cfg;
	strcpy(pal_vlynq_config->error_msg, "No link detected post RTM config.");
	goto pal_vlynq_init_link_error_3;
    }

    /* Again, We must clear the spur interrupts. */
    p_reg            = PEER_REGS(p_vlynq);
    vlynq_wr_reg(p_vlynq, True, &p_reg->status, 
		 vlynq_rd_reg(p_vlynq, True, &p_reg->status) | 0x180);

    p_reg            = LOCAL_REGS(p_vlynq);
    vlynq_wr_reg(p_vlynq, False, &p_reg->status, 
                 vlynq_rd_reg(p_vlynq, False, &p_reg->status) | 0x180);

    /* Let us go ahead and initialize the interal interrupts mechanisms
     * for the local VLYNQ. */
    if(pal_vlynq_config->on_soc)
    { 
        p_root_irq_map  = vlynq_get_free_irq_map();
        p_root_irq_info = vlynq_get_free_irq_info();
        p_irq_swap_map  = vlynq_get_free_irq_swap_fixup();
       
        if(!p_root_irq_map || !p_root_irq_info || !p_irq_swap_map)
        {
	    pal_vlynq_config->error_status = pal_vlynq_init_internal_problem;
	    strcpy(pal_vlynq_config->error_msg, "Could not allocate root irq structs.");
            goto pal_vlynq_init_root_irq_map_error;
        }
    }
       
    p_vlynq->soc           = pal_vlynq_config->on_soc;
    p_vlynq->root_irq_map  = p_root_irq_map;
    p_vlynq->root_irq_info = p_root_irq_info;
    p_vlynq->irq_swap_map  = p_irq_swap_map;

    if(pal_vlynq_config->on_soc && vlynq_add_root(p_vlynq))
    {
       	 pal_vlynq_config->error_status = pal_vlynq_init_internal_problem;
	 strcpy(pal_vlynq_config->error_msg, "Could not add root vlynq.");
         goto pal_vlynq_init_root_add_error;
    }

    if(vlynq_install_intern_isr(p_vlynq, pal_vlynq_config))
    {
	pal_vlynq_config->error_status = pal_vlynq_init_internal_problem;
	strcpy(pal_vlynq_config->error_msg, "Could not install internal isr.");
        goto pal_vlynq_init_install_intern_isr_error;
    }

    if(p_vlynq->soc)
    {
        if(vlynq_add_intern_isr(p_vlynq, pal_vlynq_config->local_intr_vector,
                                p_vlynq->local_isr_info))
	{
	    pal_vlynq_config->error_status = pal_vlynq_init_internal_problem;
	    strcpy(pal_vlynq_config->error_msg, "Could not add local isr.");
            goto pal_vlynq_init_add_intern_local_isr_error;
	}

        if(vlynq_add_intern_isr(p_vlynq, pal_vlynq_config->peer_intr_vector,
                                p_vlynq->peer_isr_info))
	{
	    pal_vlynq_config->error_status = pal_vlynq_init_internal_problem;
	    strcpy(pal_vlynq_config->error_msg, "Could not add peer isr.");
            goto pal_vlynq_init_add_intern_peer_isr_error;
	}
    }

    p_reg            = PEER_REGS(p_vlynq);
    vlynq_wr_reg(p_vlynq, True, &p_reg->txMap, pal_vlynq_config->peer_tx_addr);
    vlynq_wr_reg(p_vlynq, True, &p_reg->status, 
                 vlynq_rd_reg(p_vlynq, True, &p_reg->status) | 0x180);

    val              = vlynq_rd_reg(p_vlynq, True, &p_reg->intStatus);
    vlynq_wr_reg(p_vlynq, True, &p_reg->intStatus, val);

    p_reg            = LOCAL_REGS(p_vlynq);
    vlynq_wr_reg(p_vlynq, False, &p_reg->txMap, pal_vlynq_config->local_tx_addr);
    vlynq_wr_reg(p_vlynq, False, &p_reg->status, 
                 vlynq_rd_reg(p_vlynq, True, &p_reg->status) | 0x180);

    val              = vlynq_rd_reg(p_vlynq, False, &p_reg->intStatus);
    vlynq_wr_reg(p_vlynq, False, &p_reg->intStatus, val);

    vlynq_config_bridge_endianness(p_vlynq, 
			           pal_vlynq_config->local_endianness,
				   pal_vlynq_config->peer_endianness);

    /* Lets fixup the effect of endinaness madness on the intern ISR(s). */
    if(p_vlynq->soc)
        vlynq_set_intern_irq_swap_fixup(p_vlynq); 

    pal_vlynq_config->error_status = pal_vlynq_init_success;
    strcpy(pal_vlynq_config->error_msg, "Success");

    return (p_vlynq);

pal_vlynq_init_add_intern_peer_isr_error:

    vlynq_remove_intern_isr(p_vlynq, pal_vlynq_config->local_intr_vector, 
		            p_vlynq->local_isr_info);

pal_vlynq_init_add_intern_local_isr_error:

    vlynq_uninstall_intern_isr(p_vlynq);

pal_vlynq_init_install_intern_isr_error:

    if(pal_vlynq_config->on_soc)
        vlynq_remove_root(p_vlynq);

pal_vlynq_init_root_add_error:

    if(pal_vlynq_config->on_soc && p_vlynq->root_irq_info)
        vlynq_free_irq_info(p_vlynq->root_irq_info);

    if(pal_vlynq_config->on_soc && p_vlynq->root_irq_info)
        vlynq_free_irq_map(p_vlynq->root_irq_map);	

pal_vlynq_init_root_irq_map_error:

pal_vlynq_init_link_error_3:
pal_vlynq_init_rtm_config_error:

pal_vlynq_init_local_rev_error:
pal_vlynq_init_peer_rev_error:

pal_vlynq_init_link_error_2:
pal_vlynq_init_clock_config_error:

pal_vlynq_init_link_error_1:
    vlynq_free_vlynq(p_vlynq);
    p_vlynq = NULL;

    return (p_vlynq); 
}


PAL_Result PAL_vlynqCleanUp(PAL_VLYNQ_HND *vlynq)
{
    PAL_VLYNQ_T     *p_vlynq = (PAL_VLYNQ_T*)vlynq;
    volatile Uint32 val; 
    VLYNQ_REG_T     *p_reg;
    PAL_Result      ret_val = PAL_VLYNQ_OK;

    if(!p_vlynq)
        return  (PAL_VLYNQ_INVALID_PARAM);

    /* HPVL - To take care of scenario where peer is powered down before
     * powering down root, check the link during cleanup before accessing remote
     * registers. TODO: There may be lot of such places to handle!
     */
    if(vlynq_detect_link_status(p_vlynq))
    {
        p_reg               = PEER_REGS(p_vlynq);

        val                 = vlynq_rd_reg(p_vlynq, True, &p_reg->intStatus);
        vlynq_wr_reg(p_vlynq, True, &p_reg->intStatus, val);

        val                 = vlynq_rd_reg(p_vlynq, True, &p_reg->status);
        val                |= 0x180;
        vlynq_wr_reg(p_vlynq, True, &p_reg->status, val);

        p_reg->txMap        = 0;
    }

    p_reg               = LOCAL_REGS(p_vlynq);
    val                 = vlynq_rd_reg(p_vlynq, False, &p_reg->intStatus);
    vlynq_wr_reg(p_vlynq, False, &p_reg->intStatus, val);

    val                 = vlynq_rd_reg(p_vlynq, False, &p_reg->status);
    val                |= (0x180);
    vlynq_wr_reg(p_vlynq, False, &p_reg->status, val);
    p_reg->txMap        = 0;


    /* 
     * Here, we can not do much about the errors. We simply keep moving on 
     * and return the error status. Thats all we can do.
     */ 
    if(p_vlynq->soc)
    {
        /* Unset the endianness */
        vlynq_unset_intern_irq_swap_fixup(p_vlynq);

        ret_val = vlynq_remove_intern_isr(p_vlynq, p_vlynq->local_irq,
                                          p_vlynq->local_isr_info);
   
        ret_val = vlynq_remove_intern_isr(p_vlynq, p_vlynq->peer_irq,
                                               p_vlynq->peer_isr_info);
    } 
   
    ret_val = vlynq_uninstall_intern_isr(p_vlynq);

    if(p_vlynq->soc)
        vlynq_remove_root(p_vlynq);

    if(p_vlynq->soc)
    {
        vlynq_free_irq_map(p_vlynq->root_irq_map);
        vlynq_free_irq_info(p_vlynq->root_irq_info);
	vlynq_free_irq_swap_fixup(p_vlynq->irq_swap_map);
    }

    p_reg = LOCAL_REGS(p_vlynq);

    /* Put the the local device in reset. */
    if(vlynq_rd_reg(p_vlynq, False, &p_reg->revision) >= 0x00010200)
    {
        vlynq_wr_reg(p_vlynq, False, &p_reg->control, 
		     vlynq_rd_reg(p_vlynq, False, &p_reg->control) | 0x1);    
    }

    vlynq_free_vlynq(p_vlynq);

    return (PAL_VLYNQ_OK);
}

char* vlynq_get_popular_name(Uint32 id, char *soc_name)
{
    const char *name = NULL;

    switch(id)
    {
        case 0x0001: 
	    name = "avalanche-i";        
            break;

        case 0x0002:
            name = "avalanche-d";
            break;

        case 0x0003:
            name = "taos";
            break;

        case 0x0104:
            name = "puma-s";
            break;

        case 0x0204:
            name = "puma-III";
            break;

        case 0x0005:
            name = "sangam";
            break;

        case 0x0006:
            name = "vdsp";
            break;

        case 0x0007: 
            name = "titan";
            break;
      
        case 0x0008:
            name = "vlynq2pci";
            break;

        case 0x0009:
            name = "acx111";
            break;

        case 0x000b:
            name = "apex";
            break;

        case 0x000c:
            name = "vlynq2usb";
            break;

        case 0x000d:
            name = "vlynq2ipsec";
            break;

        case 0x000e:
            name = "omap730";
            break;

        case 0x000f:
            name = "omap1611";
            break;

        case 0x0010:
            name = "omap1710";
            break;

        case 0x0011:
            name = "tnetw1150";
            break;

        case 0x0012:
            name = "omap331";
            break;

        case 0x0013:
            name = "DM320";
            break;

        case 0x0014:
            name = "omap2410";
            break;
       
        case 0x0015:
            name = "omap2420";
            break;

        case 0x0016:
            name = "huawei-SD705";
            break;

        case 0x0017:
            name = "trinity";
            break;

        case 0x0018:
            name = "ohio";
            break;

        case 0x0019:
            name = "sierra";
            break;
 
        case 0x0020:
            name = "Psyloke";
            break;

        case 0x0021:
            name = "tnetw1251";
            break;

        case 0x0022:
            name = "himalaya";
            break;
 
        case 0x0023:
            name = "monticello";
            break;

        case 0x0024:
            name = "DDP3020";
            break;

        case 0x0025:
            name = "Davinci";
            break;

        case 0x0026:
            name = "TUSB6010";
            break;

        case 0x0027:
            name = "tnetw1160";
            break;
 
        case 0x0028:
            name = "peta";
            break;

        case 0x0029:
            name = "1150v";
            break;

        case 0x002a:
            name = "triveni";
            break;
            
        case 0x002b:
	    name = "ohio250";
            break;

        case 0x002c:
	    name = "tomahawk";
            break;

        case 0x002d: 
            name = "DM64LC";
            break;

        case 0x002e:
            name = "Jacinto";
            break;

        case 0x002f: 
            name = "Aries";
            break;

        case 0x0030:
            name = "Yamuna"; 
            break;

        case 0x0031:
            name = "Kailash";
            break;

        case 0x0032: 
            name = "Passave";
            break;

        case 0x0033: 
       	    name = "Davinci-HD/DM700";
	    break;

        default:
            name = "unknown";
            break;
	}

        strcpy(soc_name, name);

        return (soc_name);
}

static Int32 vlynq_dump_raw(PAL_VLYNQ_T *p_vlynq, char *buf, 
                            Uint32 limit, Uint32 *eof, 
                            Bool peer)
{
    Uint8 *p_addr = (peer == True) ? (Uint8 *)PEER_REGS(p_vlynq): (Uint8*)LOCAL_REGS(p_vlynq);
    Int32 i, j, len = 0;

    for(i=0; i < 16; i++)
    {
        if(len < limit)
            len += sprintf(buf + len, "%08x:", (Uint32)p_addr);
        else
            break;

        for(j=0; j < 8; j++)
        {
            if((j%4==0) && (len < limit))
                len += sprintf(buf + len, " ");
                
            if(len  < limit)
                len += sprintf(buf + len, "%02x ", (Uint32)*p_addr++);
            else
                break;
     
        }

        if(len < limit)
            len += sprintf(buf + len, "\n");
        else
	    break;
    }

    return (len);

}

static Int32 vlynq_dump_raw_reg(PAL_VLYNQ_HND *vlynq, char *buf, 
                                Uint32 limit, Uint32 *eof)
{
    PAL_VLYNQ_T *p_vlynq = (PAL_VLYNQ_T*)vlynq;
    Int32            len = 0;

    if(!p_vlynq->soc)
    {
        if(!vlynq_touch_vlynq_through_chain(p_vlynq))
        {
            len += sprintf(buf + len, "Link failure.\n");
            return (len);
        }
    }
 
    len = vlynq_dump_raw(p_vlynq, buf, limit, eof, False);

    if(!vlynq_touch_vlynq_through_chain(p_vlynq))
    {
        len += sprintf(buf + len, "Link to peer failed.\n");
        return (len);
    } 

    return (len + vlynq_dump_raw(p_vlynq, buf + len, limit - len, eof, True));
}

static Int32 vlynq_get_hop_in_chain(PAL_VLYNQ_T *p_root_vlynq, PAL_VLYNQ_T *this_vlynq)
{
    PAL_VLYNQ_T *p_vlynq = p_root_vlynq;
    Int32       hop = -1, found = 0;

    do
    {
        hop++;
 
        if(p_vlynq == this_vlynq)
        {
            found = 1;
            break;
        }
 
        p_vlynq = p_vlynq->next;
      
    } while(p_vlynq);

    return ( found ? hop : PAL_VLYNQ_INTERNAL_ERROR);
}


static Int32 vlynq_dump_root_vlynq(PAL_VLYNQ_T *p_root_vlynq, Int32 root, char *buf, 
		                   Int32 limit, Int32 *eof)
{
    PAL_VLYNQ_IRQ_MAP_T *p_irq_map;
    Int32 irq, hop, len = 0;

    if(len < limit)
        len += sprintf(buf + len, "\nRoot Vlynq %d @ 0x%08x\n  IRQ Mapping:\n    ", 
                       root, p_root_vlynq->base);

    for(irq = 0, p_irq_map = p_root_vlynq->root_irq_map;
        irq < MAX_IRQ_PER_CHAIN; 
        irq++, p_irq_map++)
    {
        int disp_count = 0;

        if(p_irq_map->hw_intr_line == -1)
            continue;

        hop = vlynq_get_hop_in_chain(p_root_vlynq, p_irq_map->vlynq);

        if(hop < 0)
            continue;
        
        if(len < limit)
        {
            len += sprintf(buf + len, "%d for Bridge %d%d %s hw-line %d, ",
                           irq, root, hop, 
                           p_irq_map->peer ? "remote" : "local", 
                           p_irq_map->hw_intr_line);
            disp_count++;

            if(!(disp_count % 8))
                if(len < limit) len += sprintf(buf + len, "\n");

            if(!(disp_count % 2))
                if(len < limit) len += sprintf(buf + len, "\n    ");
        }
    }

    if(len < limit)
        len += sprintf(buf + len, "\n");

    return (len);
}

extern Int32 vlynq_dev_dump_dev(PAL_VLYNQ_DEV_HND *vlynq_dev, char *buf, Int32 limit, 
                                Int32 *eof);

static Int32 vlynq_dump_vlynq(PAL_VLYNQ_T *p_vlynq, char *buf, Int32 limit, Int32 *eof)
{
    Int32 index, len = 0;
    VLYNQ_REG_T *p_local_reg = LOCAL_REGS(p_vlynq);
    VLYNQ_REG_T *p_peer_reg  = PEER_REGS(p_vlynq);
    PAL_VLYNQ_DEV_HND *p_vlynq_dev_hnd;

    if(len < limit)
        len += sprintf(buf + len, "  Tx Mapping:\n");

    if(len < limit)
        len += sprintf(buf + len, "    %22s 0x%08x <-> 0x%08x\n", " ", 
		       vlynq_rd_reg(p_vlynq, False, &p_local_reg->txMap),
		       vlynq_rd_reg(p_vlynq, True,  &p_peer_reg->txMap));

    if(len < limit)
        len += sprintf(buf + len, "  Rx Mapping:\n");

    for(index = 0; index < MAX_VLYNQ_REGION; index++)
    {
        PAL_VLYNQ_REGION_INFO_T *p_lregion = &p_vlynq->local_region_info[index];
        PAL_VLYNQ_REGION_INFO_T *p_rregion = &p_vlynq->remote_region_info[index];

        if(len < limit)
            len += sprintf(buf + len, "    Rx[%d]: ", index);

        if(p_lregion->owner_dev_index != -1)
        {
            if(len < limit)
            {
                len += sprintf(buf + len, "S:0x%08x, O:0x%08x <-> ",
                               vlynq_rd_reg(p_vlynq, False, &p_local_reg->rx_set[index].size), 
			       vlynq_rd_reg(p_vlynq, False, &p_local_reg->rx_set[index].offset));
            }
        }
        else
        {
            if(len < limit)
            {
                len += sprintf(buf + len, "                           <-> ");
            }
        }

        if(p_rregion->owner_dev_index != -1)
        {
            if(len < limit)
            {
                len += sprintf(buf + len, "S:0x%08x, O:0x%08x",
                               vlynq_rd_reg(p_vlynq, True, &p_peer_reg->rx_set[index].size), 
			       vlynq_rd_reg(p_vlynq, True, &p_peer_reg->rx_set[index].offset));
            }
        }

        if(len < limit)
            len += sprintf(buf + len, "\n");
    }

    if(len < limit)
        len += sprintf(buf + len, "  Local devices:\n"); 

    for( index = 0; index < MAX_DEV_PER_VLYNQ; index++)
    {
        p_vlynq_dev_hnd = p_vlynq->local_dev[index];
         
        if(p_vlynq_dev_hnd)
        {
            if(len < limit) 
                len += vlynq_dev_dump_dev(p_vlynq_dev_hnd, buf+len, limit-len, eof);
        }                
    }

    if(len < limit)
        len += sprintf(buf + len, "\n  Remote devices:\n"); 

    for( index = 0; index < MAX_DEV_PER_VLYNQ; index++)
    {
        p_vlynq_dev_hnd = p_vlynq->peer_dev[index];
         
        if(p_vlynq_dev_hnd)
        {
            if(len < limit) 
                len += vlynq_dev_dump_dev(p_vlynq_dev_hnd, buf+len, limit-len, eof);
        }                
    }

    return (len);
}

static Int32 vlynq_dump_global(char *buf, Int32 limit, Int32 *eof)
{
    Int32 root, len = 0, hop;

    for(root = 0; root < MAX_ROOT_VLYNQ; root++)
    {
        PAL_VLYNQ_T *p_root_vlynq = root_pal_vlynq[root];
        PAL_VLYNQ_T *p_vlynq      = p_root_vlynq;

        if(!p_root_vlynq)
            continue;

        if(len < limit)
            len += vlynq_dump_root_vlynq(p_root_vlynq, root, buf+len, limit-len, eof);

        if(!vlynq_detect_link_status(p_root_vlynq))
        {
            if(len < limit)
                len += sprintf(buf + len, "Link failure for Bridge %d0.\n", root);
               
            continue;
        }

        hop = 0;

        while(p_vlynq)
        {
            VLYNQ_REG_T *p_local_reg = LOCAL_REGS(p_vlynq);
            VLYNQ_REG_T *p_peer_reg  = PEER_REGS(p_vlynq);
            char lname[30], pname[30];

            if(len < limit)
                len += sprintf(buf + len, "\nVlynq Bridge %d%d :", root, hop);

            if(!vlynq_detect_link_status(p_vlynq))
            {
                if(len < limit) 
                    len += sprintf(buf + len, "Link failure for Bridge %d%d\n",
                                    root, hop); 
                break;
            }

            if(len < limit)
                len += sprintf(buf + len, "%20s <-> %s\n",
                               vlynq_get_popular_name(vlynq_rd_reg(p_vlynq, False, &p_local_reg->chipVersion) & 0xffff, lname),
                               vlynq_get_popular_name(vlynq_rd_reg(p_vlynq, True, &p_peer_reg->chipVersion) & 0xffff, pname));
            if(len < limit)
                 len += vlynq_dump_vlynq(p_vlynq, buf+len, limit-len, eof);

            p_vlynq = p_vlynq->next;
            hop++;
        }
    }

    *eof = 1;
    return (len);

}

Bool vlynq_swap_hack;

extern Int32 vlynq_dump_ioctl (volatile Uint32 *start_reg, Uint32 dump_type, 
                               char *buf, Int32 limit, Uint32 vlynq_rev);

Int32 PAL_vlynqDump(PAL_VLYNQ_HND *vlynq, Uint32 dump_type, 
                     char *buf, Int32 limit, Int32 *eof)   
{
    Int32 len = 0, valid = 0;
    volatile Uint32 *local_addr = NULL, *peer_addr = NULL;
    VLYNQ_REG_T *p_reg;
    PAL_VLYNQ_T *p_vlynq = vlynq;

    if(dump_type == PAL_VLYNQ_DUMP_RAW_DATA)
        return (vlynq_dump_raw_reg(p_vlynq, buf, limit, eof));

    if(dump_type == PAL_VLYNQ_DUMP_ALL_ROOT)
        return (vlynq_dump_global(buf, limit, eof));
 
    if(!p_vlynq)
        return (len);

    p_reg   = LOCAL_REGS(p_vlynq);

    switch(dump_type)
    {

        case PAL_VLYNQ_DUMP_STS_REG: 
        
            local_addr = (volatile Uint32 *)&p_reg->status;
            p_reg = PEER_REGS(p_vlynq);
            peer_addr  = (volatile Uint32 *)&p_reg->status;
            break;
        
        case PAL_VLYNQ_DUMP_CNTL_REG:

            local_addr = (volatile Uint32 *)&p_reg->control;
            p_reg = PEER_REGS(p_vlynq);
            peer_addr  = (volatile Uint32 *)&p_reg->control;
            break;

        case PAL_VLYNQ_DUMP_ALL_REGS:

            local_addr = (volatile Uint32 *)p_reg;
            p_reg = PEER_REGS(p_vlynq);
            peer_addr  = (volatile Uint32 *)p_reg;
            break;
        
        default:
            valid = -1;
            break;
     }

     if(valid == -1)
         return (len);

     p_reg = LOCAL_REGS(p_vlynq);

     if(len < limit)
         len += sprintf(buf + len, "\nLocal ver#: 0x%08x\n\n", 
			vlynq_rd_reg(p_vlynq, False, &p_reg->revision));

     vlynq_swap_hack = p_vlynq->local_swap;

     if(len < limit)
         len += vlynq_dump_ioctl (local_addr, dump_type, buf + len, limit - len, 
				  vlynq_rd_reg(p_vlynq, False, &p_reg->revision));

     if(!vlynq_touch_vlynq_through_chain(p_vlynq))
     {
         len += sprintf(buf + len, "Link to peer failed.\n");
         return (len);
     } 

     p_reg = PEER_REGS(p_vlynq);

     if(len < limit)
         len += sprintf(buf + len, "\nPeer/Remote ver#: 0x%08x\n\n", 
			vlynq_rd_reg(p_vlynq, True, &p_reg->revision));

     vlynq_swap_hack = p_vlynq->peer_swap;

     if(len < limit)
         len += vlynq_dump_ioctl (peer_addr, dump_type, buf + len, limit - len, 
				  vlynq_rd_reg(p_vlynq, True, &p_reg->revision));
     *eof = 1;
     
     return (len);
}


extern PAL_Result vlynq_read_write_ioctl(volatile Uint32 *p_start_reg, Uint32 cmd, 
                                         Uint32 val, Uint32 vlynq_rev);

/* Cmd Code:
 *
 * -------------------------------------------------------------------------------------------------------------------------
 * |31: Bitop | 30: R/W* |29: Remote | 28-24:reserved | 23-16 major id of entity | 15-8 reserved|7 - 0 minor id of entity  |
 * ------------------------------------------------------------------------------------------------------------------------- 
 */

PAL_Result vlynq_non_bit_op_ioctl(PAL_VLYNQ_T *p_vlynq, Uint32 cmd, Uint32 cmd_val)
{
    VLYNQ_REG_T *p_vlynq_reg = LOCAL_REGS(p_vlynq);
    volatile Uint32      *p_start_reg = NULL;

    PAL_Result           ret_val     =  0;
    Int32                valid       =  0;
    Uint32               vlynq_rev;
    volatile Uint32      val;

    switch(PAL_VLYNQ_IOCTL_MAJOR_DE_VAL(cmd))
    {
        case PAL_VLYNQ_IOCTL_REG_CMD:

            if(cmd & PAL_VLYNQ_IOCTL_REMOTE_CMD)
                p_vlynq_reg = PEER_REGS(p_vlynq);

            p_start_reg = (volatile Uint32*)p_vlynq_reg;
            valid       = 1;
            break;

        case PAL_VLYNQ_IOCTL_PREP_LINK_DOWN:
          
            p_vlynq->backup_local_cntl_word = p_vlynq_reg->control;
            p_vlynq->backup_local_intr_ptr  = p_vlynq_reg->intPtr;
            p_vlynq->backup_local_tx_map    = p_vlynq_reg->txMap;
            if((p_vlynq_reg->revision >= 0x00010200) && 
               (p_vlynq_reg->revision <= 0x00010206))/* Hack for 1350A */
                p_vlynq->backup_local_endian = p_vlynq_reg->endian;
            else
                p_vlynq->backup_local_endian = pal_vlynq_little_en;


            /* Let us disable the interrupts, spur interrupts are not invited. */
	    val = vlynq_rd_reg(p_vlynq, False, &p_vlynq_reg->control) & (~(1 << 13));
	    vlynq_wr_reg(p_vlynq, False, &p_vlynq_reg->control, val); 

            p_vlynq_reg = PEER_REGS(p_vlynq);

            p_vlynq->backup_peer_cntl_word = p_vlynq_reg->control;
            p_vlynq->backup_peer_intr_ptr  = p_vlynq_reg->intPtr;
            p_vlynq->backup_peer_tx_map    = p_vlynq_reg->txMap;
            if((vlynq_rd_reg(p_vlynq, True, &p_vlynq_reg->revision) >= 0x00010200) && 
               (vlynq_rd_reg(p_vlynq, True, &p_vlynq_reg->revision) <= 0x00010206)) /* Hack for 1350A */
                p_vlynq->backup_peer_endian = p_vlynq_reg->endian;
            else
                p_vlynq->backup_peer_endian = pal_vlynq_little_en;

            break;

        case PAL_VLYNQ_IOCTL_PREP_LINK_UP:

            /* Clearing the spur interrupts, lest it locks up the bus. */
	    val = vlynq_rd_reg(p_vlynq, False, &p_vlynq_reg->status) | 0x180;
	    vlynq_wr_reg(p_vlynq, False, &p_vlynq_reg->status, val);

            p_vlynq_reg->control = p_vlynq->backup_local_cntl_word;
            p_vlynq_reg->intPtr  = p_vlynq->backup_local_intr_ptr;
            p_vlynq_reg->txMap   = p_vlynq->backup_local_tx_map;

            if((p_vlynq_reg->revision >= 0x00010200) && 
               (p_vlynq_reg->revision <= 0x00010206)) /* Hack for 1350A */
                p_vlynq_reg->endian = p_vlynq->backup_local_endian;

            p_vlynq_reg = PEER_REGS(p_vlynq);

	    p_vlynq->peer_swap = p_vlynq->local_swap;

            /* Clearing the spur interrupts, lest it locks up the bus. */
	    val = vlynq_rd_reg(p_vlynq, True, &p_vlynq_reg->status) | 0x180;
	    vlynq_wr_reg(p_vlynq, True, &p_vlynq_reg->status, val);

            if((vlynq_rd_reg(p_vlynq, True, &p_vlynq_reg->revision) >= 0x00010200) && 
               (vlynq_rd_reg(p_vlynq, True, &p_vlynq_reg->revision) <= 0x00010206)) /* Hack for 1350A */
                vlynq_wr_reg(p_vlynq, True, &p_vlynq_reg->endian, p_vlynq->backup_peer_endian);

	    if(p_vlynq->backup_local_endian != p_vlynq->backup_peer_endian)
	        p_vlynq->peer_swap = p_vlynq->peer_swap ? False : True;

            p_vlynq_reg->control = p_vlynq->backup_peer_cntl_word;
            p_vlynq_reg->intPtr  = p_vlynq->backup_peer_intr_ptr;
            p_vlynq_reg->txMap   = p_vlynq->backup_peer_tx_map;
            break;

        case PAL_VLYNQ_IOCTL_CLEAR_INTERN_ERR:

            p_vlynq_reg = LOCAL_REGS(p_vlynq);
	    val  = vlynq_rd_reg(p_vlynq, False, &p_vlynq_reg->status) | 0x180;
	    vlynq_wr_reg(p_vlynq, False, &p_vlynq_reg->status, val);

            p_vlynq_reg = PEER_REGS(p_vlynq);

            vlynq_delay_wait(0x100);

	    val = vlynq_rd_reg(p_vlynq, True, &p_vlynq_reg->status) | 0x180;
	    vlynq_wr_reg(p_vlynq, True, &p_vlynq_reg->status, val);

            break;

        default:
            ret_val = -1;
            break;         
    }

    vlynq_rev = vlynq_rd_reg(p_vlynq, 
		             (cmd & PAL_VLYNQ_IOCTL_REMOTE_CMD) ? True : False,
		             &p_vlynq_reg->revision); 

    if(valid)
    {
	vlynq_swap_hack = (cmd & PAL_VLYNQ_IOCTL_REMOTE_CMD) ? p_vlynq->peer_swap : p_vlynq->local_swap;
        ret_val = vlynq_read_write_ioctl(p_start_reg, cmd, cmd_val, 
                                         vlynq_rev);
    }

    return (PAL_VLYNQ_OK);
}

PAL_Result PAL_vlynqIoctl(PAL_VLYNQ_HND *vlynq_hnd, Uint32 cmd, Uint32 cmd_val)
{
    VLYNQ_REG_T *p_vlynq_reg;
    volatile Uint32      *p_start_reg = NULL;
    
    PAL_Result           ret_val     = -1;
    PAL_VLYNQ_T          *p_vlynq    = vlynq_hnd;
    Int32                valid       = 1;
    Uint32               vlynq_rev;
    Bool                 peer        = True;

    if(!p_vlynq)
        return (PAL_VLYNQ_INVALID_PARAM);

    if(!vlynq_touch_vlynq_through_chain(p_vlynq))
        return (PAL_VLYNQ_INTERNAL_ERROR);

    if(cmd & PAL_VLYNQ_IOCTL_REMOTE_CMD)
        p_vlynq_reg = PEER_REGS(p_vlynq);
    else
    {
        p_vlynq_reg = LOCAL_REGS(p_vlynq);
	peer        = False;
    }

    /* Hack for now as we support just one major non bit cmd. */
    if(!(cmd & PAL_VLYNQ_IOCTL_BIT_CMD))
    {
        ret_val = vlynq_non_bit_op_ioctl(p_vlynq, cmd, cmd_val);
        valid   = 0; /* We do not require register access. */
    }
    else
    {
        switch(PAL_VLYNQ_IOCTL_MAJOR_DE_VAL(cmd))
        {
            case  PAL_VLYNQ_IOCTL_STATUS_REG:
        
                p_start_reg = (volatile Uint32*)&p_vlynq_reg->status;
                break;

            case PAL_VLYNQ_IOCTL_CONTROL_REG:
 
                p_start_reg = (volatile Uint32*)&p_vlynq_reg->control;
                break;

            default:
                valid = 0;
                break;
    
        }
    }

    vlynq_rev = vlynq_rd_reg(p_vlynq, peer, &p_vlynq_reg->revision);

    if(valid)
    {
	vlynq_swap_hack = peer ? p_vlynq->peer_swap : p_vlynq->local_swap;
        ret_val = vlynq_read_write_ioctl(p_start_reg, cmd, cmd_val, 
                                         vlynq_rev);
    }

    return (ret_val);
}
