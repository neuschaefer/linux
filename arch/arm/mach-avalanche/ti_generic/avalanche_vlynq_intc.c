/*
 *
 * avalanche_vlynq_intc.c 
 * Description:
 * vlynq interrupt handling layer
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
 *
 */
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <linux/proc_fs.h>
#include <asm/irq.h>
#include <asm/arch/generic/avalanche_vlynq_intc.h>
#include <asm/arch/generic/avalanche_intc.h>

extern irq_desc_t irq_desc[];

typedef struct av_vlynq_isr_info
{
    void                             *dev_id;
    PAL_VLYNQ_DEV_ISR_PARAM_GRP_T    *vlynq_isr;
    struct av_vlynq_isr_info         *next;

} AV_VLYNQ_ISR_INFO_T;

int avalanche_vlynq_get_sys_irq_num(PAL_VLYNQ_HND *vlynq, int irq)
{
    PAL_VLYNQ_HND *p_root_vlynq;
    Uint32        base_addr;
    Int32         ret_irq = -1;

    if(!vlynq)
        return (ret_irq);
   
    p_root_vlynq = PAL_vlynqGetRootVlynq(vlynq);

    if(!p_root_vlynq)
        return (ret_irq);

    if(PAL_vlynqGetBaseAddr(p_root_vlynq, &base_addr))
        return (ret_irq);

    switch(base_addr)
    {
#ifdef AVALANCHE_HIGH_VLYNQ_CONTROL_BASE
        case AVALANCHE_HIGH_VLYNQ_CONTROL_BASE:
             ret_irq += irq + AVALANCHE_INT_END_LOW_VLYNQ;
             break;
#endif

        case AVALANCHE_LOW_VLYNQ_CONTROL_BASE:
             ret_irq += irq + AVALANCHE_INTC_END;
             break;

        default:
             break;
    }
    
    return (ret_irq);
}

#define VLYNQINTNUM(vlynq, irq) avalanche_vlynq_get_sys_irq_num(vlynq, irq)


/* Returns the handle to the VLYNQ which deals with the indicated irq and in the 
 * process adjusts the irq# to that suited for t he VLYNQ.
 */
static PAL_VLYNQ_HND* avalanche_vlynq_get_vlynq(unsigned int irq, unsigned int *adj_irq)
{
    PAL_VLYNQ_HND *p_vlynq = NULL;

    /* System Vlynq interrupts 32-63 */
    if(irq >= AVALANCHE_INT_END_LOW_VLYNQ)
    {
#ifdef AVALANCHE_HIGH_VLYNQ_CONTROL_BASE
        p_vlynq  = PAL_vlynqGetRootAtBase(AVALANCHE_HIGH_VLYNQ_CONTROL_BASE);
        *adj_irq = irq - AVALANCHE_INT_END_LOW_VLYNQ;
#endif
    }
    /* System Vlynq interrupts 0-31 */
    else
    {
        p_vlynq  = PAL_vlynqGetRootAtBase(AVALANCHE_LOW_VLYNQ_CONTROL_BASE);
        *adj_irq = irq - AVALANCHE_INTC_END;
    }

    if(p_vlynq)
        p_vlynq = PAL_vlynqGetForIrq(p_vlynq, *adj_irq);

    return (p_vlynq);
}

void avalanche_vlynq_disable_irq(unsigned int irq)
{
    unsigned int  adj_irq;
    PAL_VLYNQ_HND *p_vlynq = avalanche_vlynq_get_vlynq(irq, &adj_irq);

    if(p_vlynq)
        PAL_vlynqDisableIrq(p_vlynq, adj_irq);
}

void avalanche_vlynq_enable_irq(unsigned int irq)
{
    unsigned int adj_irq;
    PAL_VLYNQ_HND *p_vlynq = avalanche_vlynq_get_vlynq(irq, &adj_irq);

    if(p_vlynq)
        PAL_vlynqEnableIrq(p_vlynq, adj_irq);
}

static AV_VLYNQ_ISR_INFO_T *free_av_vlynq_isr_list = NULL;

int avalanche_vlynq_request_irq(unsigned int irq, irqreturn_t (*handler)(int, void *, struct pt_regs *),
		                unsigned long irqflags, const char * devname, void *dev_id)
{
    PAL_VLYNQ_DEV_ISR_PARAM_GRP_T *p_isr_param;
    AV_VLYNQ_ISR_INFO_T           *p_av_vlynq_isr;
    int                           adj_irq;
    PAL_VLYNQ_HND                 *p_vlynq        = avalanche_vlynq_get_vlynq(irq, &adj_irq);

    if(!p_vlynq)
        return (-1);    

    p_isr_param    = kmalloc(sizeof(PAL_VLYNQ_DEV_ISR_PARAM_GRP_T), GFP_KERNEL);
    if(!p_isr_param)
        return (-1);

    p_av_vlynq_isr = kmalloc(sizeof(AV_VLYNQ_ISR_INFO_T), GFP_KERNEL);

    if(!p_av_vlynq_isr)
    {
        kfree(p_isr_param);
        return (-1);
    }

    p_isr_param->arg1 = irq;
    p_isr_param->arg2 = dev_id;
    p_isr_param->arg3 = NULL;

    if(PAL_vlynqAddIsr(p_vlynq, adj_irq, handler, p_isr_param))
    {
        kfree(p_isr_param);
        return (-1);
    }

    p_av_vlynq_isr->dev_id    = dev_id;
    p_av_vlynq_isr->vlynq_isr = p_isr_param;

    p_av_vlynq_isr->next      = free_av_vlynq_isr_list;
    free_av_vlynq_isr_list    = p_av_vlynq_isr;

    return(0);
}

void avalanche_vlynq_free_irq(unsigned int irq, void *dev_id)
{
    AV_VLYNQ_ISR_INFO_T *p_last_isr, *p_isr;
    int adj_irq;
    PAL_VLYNQ_HND       *p_vlynq    = avalanche_vlynq_get_vlynq(irq, &adj_irq);
    
    for(p_isr  = free_av_vlynq_isr_list, p_last_isr = NULL; 
        p_isr != NULL; 
        p_last_isr = p_isr, p_isr = p_isr->next)
    {
        if(p_isr->dev_id == dev_id)
            break;
    }

    if(!p_isr)
        return;

    if(PAL_vlynqRemoveIsr(p_vlynq, adj_irq, p_isr->vlynq_isr))
        return;

    if(!p_last_isr)
        free_av_vlynq_isr_list = p_last_isr;
    else
        p_last_isr->next       = p_isr->next;

    kfree(p_isr->vlynq_isr);
    kfree(p_isr);
 
    return;
}

int avalanche_vlynq_set_irq_polarity(unsigned int irq, unsigned int pol)
{
    unsigned int adj_irq;
    PAL_VLYNQ_HND *p_vlynq = avalanche_vlynq_get_vlynq(irq, &adj_irq);
    PAL_Result    ret_val  = -1;

    if(p_vlynq)
        ret_val = PAL_vlynqSetIrqPol(p_vlynq, adj_irq, pol ? pal_vlynq_low_irq_pol : pal_vlynq_high_irq_pol);

    return (ret_val ? -1:0);
}

int avalanche_vlynq_get_irq_polarity(unsigned int irq)
{
    unsigned int adj_irq;
    PAL_VLYNQ_HND *p_vlynq = avalanche_vlynq_get_vlynq(irq, &adj_irq);
    PAL_Result    ret_val  = -1;
    PAL_VLYNQ_IRQ_POL_ENUM_T pol; 

    if(p_vlynq)
        ret_val = PAL_vlynqGetIrqPol(p_vlynq, adj_irq, &pol);
    
    if(ret_val)
        return (ret_val);
    else
        return ((pol == pal_vlynq_low_irq_pol) ? 1 : 0);
}

int avalanche_vlynq_set_irq_type(unsigned int irq, unsigned val)
{
    unsigned int adj_irq;
    PAL_VLYNQ_HND *p_vlynq = avalanche_vlynq_get_vlynq(irq, &adj_irq);
    PAL_Result    ret_val  = -1;
 
    if(p_vlynq)
        ret_val = PAL_vlynqSetIrqType(p_vlynq, adj_irq, val ? pal_vlynq_level_irq_type : pal_vlynq_edge_irq_type);

    return (ret_val ? -1:0);  
}

int avalanche_vlynq_get_irq_type(unsigned int irq)
{
    unsigned int adj_irq;
    PAL_VLYNQ_HND *p_vlynq = avalanche_vlynq_get_vlynq(irq, &adj_irq);
    PAL_Result    ret_val  = -1;
    PAL_VLYNQ_IRQ_TYPE_ENUM_T type; 

    if(p_vlynq)
        ret_val = PAL_vlynqGetIrqType(p_vlynq, adj_irq, &type);

    if(ret_val)
        return (ret_val);
    else    
        return ((type == pal_vlynq_edge_irq_type) ? 0 : 1);
}

int avalanche_vlynq_irq_list(char *buf)
{
    int i, len = 0;
    int num =  AVALANCHE_INTC_END;
    struct irqaction *action;

    /* vlynq interrupts */
    for (i = AVALANCHE_INTC_END; i < AVALANCHE_INT_END; i++,num++)
    {
        Uint32 count;
        PAL_VLYNQ_HND *p = NULL;
        int irq = 0;

        if(i >= AVALANCHE_INT_END_LOW_VLYNQ)
        {
#ifdef AVALANCHE_HIGH_VLYNQ_CONTROL_BASE
            irq  = i - AVALANCHE_INT_END_LOW_VLYNQ;
            p    = PAL_vlynqGetRootAtBase(AVALANCHE_HIGH_VLYNQ_CONTROL_BASE);
#endif
        }
        else
        {
            irq = i - AVALANCHE_INTC_END; 
            p   = PAL_vlynqGetRootAtBase(AVALANCHE_LOW_VLYNQ_CONTROL_BASE);
        } 

        if (!p)
            continue;
	            
        action = irq_desc[i].action;

        if (!action)
            continue;

        if(PAL_vlynqGetIrqCount(p, irq, &count))
            continue;

        len += sprintf(buf+len, "%3d: %8d %c %s", num, count, 
                       (action->flags & SA_INTERRUPT) ? '+' : ' ', action->name);

        for (action=action->next; action; action = action->next)
        {
            len += sprintf(buf+len, ",%s %s", (action->flags & SA_INTERRUPT) ? " +" : "",
                           action->name);
        }

        if(i < AVALANCHE_INT_END_LOW_VLYNQ)
            len += sprintf(buf+len, " [hw0 (Low Vlynq)]\n");
        else
            len += sprintf(buf+len, " [hw0 (High Vlynq)]\n");
    }

    return (len);
}

/* This assumes that irq value (i) passed is correct. No checking done here */
unsigned int avalanche_vlynq_get_irq_count(unsigned int i)
{
    PAL_VLYNQ_HND *p = NULL;
    int irq;
    unsigned int count;

    if(i >= AVALANCHE_INT_END_LOW_VLYNQ)
    {
#ifdef AVALANCHE_HIGH_VLYNQ_CONTROL_BASE
        irq  = i - AVALANCHE_INT_END_LOW_VLYNQ;
        p    = PAL_vlynqGetRootAtBase(AVALANCHE_HIGH_VLYNQ_CONTROL_BASE);
#endif
    }
    else
    {
        irq = i - AVALANCHE_INTC_END; 
        p   = PAL_vlynqGetRootAtBase(AVALANCHE_LOW_VLYNQ_CONTROL_BASE);
    }

    if (!p) return 0;

    if(PAL_vlynqGetIrqCount(p, irq, &count)) return 0;

    return count;
} 

// Exporting the symbols for the VLYNQ.

EXPORT_SYMBOL(PAL_vlynqAddDevice);
EXPORT_SYMBOL(PAL_vlynqRemoveDevice);
EXPORT_SYMBOL(PAL_vlynqMapRegion);
EXPORT_SYMBOL(PAL_vlynqUnMapRegion);
EXPORT_SYMBOL(PAL_vlynqGetDevBase);
EXPORT_SYMBOL(PAL_vlynqGetLinkStatus);
EXPORT_SYMBOL(PAL_vlynqGetNumRoot);
EXPORT_SYMBOL(PAL_vlynqGetRoot);
EXPORT_SYMBOL(PAL_vlynqGetNext);
EXPORT_SYMBOL(PAL_vlynqIsLast);
EXPORT_SYMBOL(PAL_vlynqGetChainLength);
EXPORT_SYMBOL(PAL_vlynqGetBaseAddr);
EXPORT_SYMBOL(PAL_vlynqGetRootAtBase);
EXPORT_SYMBOL(PAL_vlynqGetRootVlynq);
EXPORT_SYMBOL(PAL_vlynqChainAppend);
EXPORT_SYMBOL(PAL_vlynqChainUnAppend);
EXPORT_SYMBOL(PAL_vlynqMapIrq);
EXPORT_SYMBOL(PAL_vlynqUnMapIrq);
EXPORT_SYMBOL(PAL_vlynqAddIsr);
EXPORT_SYMBOL(PAL_vlynqRemoveIsr);
EXPORT_SYMBOL(PAL_vlynqRootIsr);
EXPORT_SYMBOL(PAL_vlynqGetIrqCount);
EXPORT_SYMBOL(PAL_vlynqGetForIrq);
EXPORT_SYMBOL(PAL_vlynqSetIrqPol);
EXPORT_SYMBOL(PAL_vlynqGetIrqPol);
EXPORT_SYMBOL(PAL_vlynqSetIrqType);
EXPORT_SYMBOL(PAL_vlynqGetIrqType);
EXPORT_SYMBOL(PAL_vlynqDisableIrq);
EXPORT_SYMBOL(PAL_vlynqEnableIrq);
EXPORT_SYMBOL(PAL_vlynqConfigClock);
EXPORT_SYMBOL(PAL_vlynqInit);
EXPORT_SYMBOL(PAL_vlynqCleanUp);
EXPORT_SYMBOL(PAL_vlynqDump);
EXPORT_SYMBOL(PAL_vlynqIoctl);


EXPORT_SYMBOL(PAL_vlynqDevCbRegister);
EXPORT_SYMBOL(PAL_vlynqDevCbUnregister);
EXPORT_SYMBOL(PAL_vlynqDevFind);
EXPORT_SYMBOL(PAL_vlynqDevGetVlynq);
EXPORT_SYMBOL(PAL_vlynqDevFindIrq);
EXPORT_SYMBOL(PAL_vlynqDevGetResetBit);
EXPORT_SYMBOL(PAL_vlynqDevCreate);
EXPORT_SYMBOL(PAL_vlynqDevDestroy);

/* This section registers the vlynq descriptors with the avalanche interrupt controller */

#define enable_avalanche_vlynq_irq              avalanche_vlynq_enable_irq 
#define disable_avalanche_vlynq_irq		avalanche_vlynq_disable_irq	

/* this API is assumed to get called from request_irq */ 
static unsigned int startup_avalanche_vlynq_irq(unsigned int irq)
{
	struct irqaction *action = irq_desc[irq].action;
	avalanche_vlynq_request_irq(irq, action->handler, action->flags,
		action->name, action->dev_id);

    enable_avalanche_vlynq_irq(irq);
    return 0; 
}

/* this API is assumed to get called from free_irq */
static void shutdown_avalanche_vlynq_irq(unsigned int irq)
{
    struct irqaction *action = irq_desc[irq].action;
	disable_avalanche_vlynq_irq(irq);
	avalanche_vlynq_free_irq(irq, action->dev_id);
}

static void end_avalanche_vlynq_irq(unsigned int irq) 
{  
	if (!(irq_desc[irq].status & (IRQ_DISABLED|IRQ_INPROGRESS)))
		enable_avalanche_vlynq_irq(irq);
}


#if 0
/*
   Sync between kstat and vlynq maintained ISR count.
   To be called before using kstat for vlynq interrupts
*/
void avalanche_update_vlynq_irq_counts(void)
{
    int i;
    /* go over all vlynq interrupts and update the counts in the kstat
       structure. This is being done as there is currently no way for
       linux to get informed whenever a vlynq interrupt occurs.
     */
    for (i = AVALANCHE_INTC_END; i < AVALANCHE_INT_END_VLYNQ; i++)
    {
        kstat_this_cpu.irqs[i] = avalanche_vlynq_get_irq_count(i);
    }
}

#endif

static struct irq_chip avalanche_low_vlynq_irq_type = {
    .name       = "[VLYNQ(Low)]",
    .startup    = startup_avalanche_vlynq_irq, /* invoked by request_irq()*/
    .shutdown   = shutdown_avalanche_vlynq_irq,/* inoked by free_irq() */ 
    .unmask     = enable_avalanche_vlynq_irq,
    .mask       = disable_avalanche_vlynq_irq,
//    .ack        = mask_and_ack_avalanche_vlynq_irq,
    .end        = end_avalanche_vlynq_irq,
};

#ifdef AVALANCHE_HIGH_VLYNQ_CONTROL_BASE
static struct irq_chip avalanche_high_vlynq_irq_type = {
    .name       = "[VLYNQ(High)]",
    .startup    = startup_avalanche_vlynq_irq,
    .shutdown   = shutdown_avalanche_vlynq_irq,
    .unmask     = enable_avalanche_vlynq_irq,
    .mask       = disable_avalanche_vlynq_irq,
 //   .ack        = mask_and_ack_avalanche_vlynq_irq,
    .end        = end_avalanche_vlynq_irq,
};
#endif

static int __init avalanche_vlynq_intc_init(void)
{
	int i = 0;

	for(i = AVALANCHE_INTC_END; i < AVALANCHE_INT_END_VLYNQ; i++)
	{
		if (i < AVALANCHE_INT_END_LOW_VLYNQ) {
			set_irq_chip(i, &avalanche_low_vlynq_irq_type);
		} 
		#ifdef AVALANCHE_HIGH_VLYNQ_CONTROL_BASE
		else  {
			set_irq_chip(i, &avalanche_high_vlynq_irq_type);
		}
		#endif /* AVALANCHE_HIGH_VLYNQ_CONTROL_BASE */
	}
	
	printk("Vlynq irq descriptors registered successfully.\n");
	
	return 0;
}

static void __exit avalanche_vlynq_intc_exit(void)
{
	printk("\nVlynq irq descriptors unregistered.\n");
}

module_init(avalanche_vlynq_intc_init);
module_exit(avalanche_vlynq_intc_exit);


