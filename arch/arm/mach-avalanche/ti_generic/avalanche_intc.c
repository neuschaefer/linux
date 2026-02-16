/*
 *
 * avalanche_intc.c
 * Description:
 * avalanche interrupt controller implementation
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
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/
#include <asm/arch/update_tag.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/dma-mapping.h>


#include <asm/irq.h>
#include <asm/atomic.h>
#include <asm/arch/hardware.h>
#include <asm-arm/arch-avalanche/generic/pal.h>

#ifdef CONFIG_AVALANCHE_INTC_PACING
extern unsigned int avalanche_get_intc_input_freq( void );
#endif

#if defined(CONFIG_ICRTL_DEBUG)
#define DEBUG(fmt,arg...)  printk(KERN_EMERG fmt , ##arg);
#else
#define DEBUG(fmt,arg...)
#endif

#define ENTER DEBUG("[Ei %s-%d] \n", __FUNCTION__, __LINE__);
#define EXIT  DEBUG("[Ex %s-%d] \n", __FUNCTION__, __LINE__);
#define INTERRUPT_PANIC(irq) \
        printk("whee, invalid irq_nr %d at %s line %d\n", \
                                (irq), __FILE__, __LINE__);\
        panic("IRQ, you lose...");

#define INTC_SET_BIT_0                ( 0x00000001 )
#define INTC_CLEAR_BIT_0              ( 0xFFFFFFFE )

/*****************************************************************************
 *  General and Global Regsiters APIs
 ****************************************************************************/
/* Revision Register */
static inline unsigned int avalanche_intc_read_revision_register( void );

/* Control Register */
#define INTC_CNRTL_WAKEUP_MODE          ( 0x00000002 )
#define INTC_CNRTL_NORMAL_MODE          ( 0x00000000 )
#define INTC_CNRTL_NO_NESTING           ( 0x00000000 )
#define INTC_CNRTL_MANUAL_NESTING       ( 0x0000000C )
#define INTC_CNRTL_AUTO_GLB_NESTING     ( 0x00000008 )
#define INTC_CNRTL_AUTO_INDL_NESTING    ( 0x00000004 )

static unsigned int avalanche_intc_get_cntrl_mode( void );

/* Global Nesting Level Register */
#ifdef CONFIG_INTC_INTERRUPT_NESTING
#define OVERRIDE_AUTO_NESTING          ( 0x10000000 )
static int avalanche_intc_set_glb_nesting_lvl ( unsigned int nesting_level );
#endif /*ifdef CONFIG_INTC_INTERRUPT_NESTING */

/* Global Prioritized Index Register  */
#define INTC_PRIORITY_INT_FLAG         ( 0x1FF )


/*****************************************************************************
 *  Index Register APIs
 ****************************************************************************/
/* Interrupt Status Index Register */
static int avalanche_intc_clear_status( unsigned int irq );

/* Secure Interrupt  Prioritized Index Register */
#ifdef CONFIG_AVALANCHE_INTC_SECURE_INTS
#define INTC_PRIORITY_INTERRUPT_FLAG    ( 0x00EF )
static unsigned int avalanche_intc_get_secure_priority_interrupt (void);
#endif /*CONFIG_AVALANCHE_INTC_SECURE_INTS */

/****************************************************************************
 * Interrupt Mapping APIs
 ****************************************************************************/
/* Channel Map Registers */
static int avalanche_intc_map_system_int_to_channel( unsigned int sys_irq,
                                                     unsigned int channel_num);
/* Host  Interrupt  Map Registers */
static int avalanche_intc_map_channel_to_host_int( unsigned int channel_num,
                                                   unsigned int host_irq);

/*****************************************************************************
 * Security Enable and Disable regs  per interrupt
 ****************************************************************************/
#ifdef CONFIG_AVALANCHE_INTC_SECURE_INTS
static int avalanche_intc_enable_secure_interrupt( unsigned int irq );
static int avalanche_intc_disable_secure_interrupt( unsigned int irq );
#endif

#ifdef CONFIG_AVALANCHE_INTC_PACING

#define ENABLING_PACING_BIT_POS            ( 16 )
#define INTC_PACER_COUNT_BIT_POS           ( 16 )
#define INTC_MAX_COUNT_VAL                 ( 0xFF )
#define INTC_MAX_PRESCALE_COUNT            ( 0xFFF )
#define INTC_PACER_MAX_VAL                 ( 0xFFFF )
#define INTC_GLOBAL_PACER_MODE_FLAG        ( 0xA0000000 )
#define INTC_PRESCALE_COUNT_FLAG           ( 0x00000FFF )
#define INTC_PACER_COUNT_FLAG              ( 0x00FF0000 )
#define INTC_PACER_MAX_FLAG                ( 0x0000FFFF )
#define INTC_ENABLE_PACER_RESTART          ( 0x08000000 )
#define INTC_DISABLE_PACER_RESTART         ( 0xF7FFFFFF )
#define INTC_ENABLE_PACER_BYPASS           ( 0x80000000 )
#define INTC_DISABLE_PACER_BYPASS          ( 0x7FFFFFFF )

#endif /*CONFIG_AVALANCHE_INTC_PACING */

/*****************************************************************************
 * Global Data types
 ****************************************************************************/
extern struct irq_desc irq_desc[NR_IRQS];
struct avalanche_ictrl_regs  *avalanche_hw0_icregs;
extern asmlinkage void armIRQ(void);

/*****************************************************************************
 * Utility Functions
 ****************************************************************************/
inline unsigned char  get_bit_position ( unsigned int irq )
{
    return((unsigned char)( irq % NUM_INTS_PER_REG ));
}

inline unsigned char  get_reg_index( unsigned int irq )
{
    return((unsigned char )( irq / NUM_INTS_PER_REG ));
}
/*****************************************************************************
 * Function Definitions
 ****************************************************************************/
static inline unsigned int avalanche_intc_read_revision_register( void )
{
    return(avalanche_hw0_icregs->icrevr);
}


inline static unsigned int avalanche_intc_get_cntrl_mode( void )
{
    return((unsigned  int)avalanche_hw0_icregs->iccntlr);
}

#ifdef CONFIG_INTC_INTERRUPT_NESTING
static int avalanche_intc_set_glb_nesting_lvl( unsigned int nesting_level )
{
    avalanche_hw0_icregs->ichcntlr = (OVERRIDE_AUTO_NESTING |
                                          nesting_level);
    return(0);
}
#endif /* CONFIG_INTC_INTERRUPT_NESTING */

static int avalanche_intc_clear_status( unsigned int irq )
{
    avalanche_hw0_icregs->icsicr = irq;
    return(0);
}

/* Channel Map Registers */
static int avalanche_intc_map_system_int_to_channel( unsigned int sys_irq,
                                                     unsigned int channel_num )
{
    unsigned int chnl_reg_indx = sys_irq/NUM_SYS_INTS_PER_REG;
    /* There is one register per 4 system Interrupts, so get the proper
     * register base for the given interrupt.
     */
    avalanche_hw0_icregs->icchnlmpr[chnl_reg_indx] |=
    (channel_num << ((sys_irq % NUM_SYS_INTS_PER_REG) * 8));
    return(0);
}

/* Host  Interrupt  Map Registers */
static int avalanche_intc_map_channel_to_host_int( unsigned int channel_num,
                                                   unsigned int host_irq )
{
    unsigned int hst_int_reg_indx = channel_num /NUM_CHANNELS_PER_REG;
    /* There is one register per 4 channels so get the proper
     * register base for the given channel.
     */
    avalanche_hw0_icregs->ichmpr[hst_int_reg_indx] |=
    (host_irq << ((channel_num % NUM_CHANNELS_PER_REG)* 8 ));
    return(0);
}

/* Linux irq_chip framework hook for set type */
/* check include/linux/irq.h for type flags */
int avalanche_intc_set_type(struct irq_data *data, unsigned int flow_type)
{
    /* identify interrupt type and set the same */
    if((flow_type & IRQ_TYPE_EDGE_RISING) ||
            (flow_type & IRQ_TYPE_EDGE_FALLING) ||
                (flow_type & IRQ_TYPE_EDGE_BOTH))
    {
        avalanche_intc_set_interrupt_type(data->irq, 1);
        set_irq_handler(data->irq, do_edge_IRQ);
    }
    else
    {
        avalanche_intc_set_interrupt_type(data->irq, 0);
        set_irq_handler(data->irq, do_level_IRQ);
    }

    /* identify polarity and set the same */
    if((flow_type & IRQ_TYPE_EDGE_FALLING) ||
            (flow_type & IRQ_TYPE_LEVEL_LOW))
    {
        avalanche_intc_set_interrupt_polarity(data->irq, 0);
    }
    else
    {
        avalanche_intc_set_interrupt_polarity(data->irq, 1);
    }
    return(0);
}

/* low level INTC type set */
int avalanche_intc_set_interrupt_type( unsigned int irq,
                                       unsigned char  int_type)
{
    unsigned int int_reg_indx = get_reg_index( irq );
    if (int_type)
    {
       avalanche_hw0_icregs->ictypr[int_reg_indx] |=
           (1 << get_bit_position( irq ));
    }
    else
    {
       avalanche_hw0_icregs->ictypr[int_reg_indx] &=
           ~(1 << get_bit_position( irq ));
    }
    return(0);
}

/* low level INTC type get */
int avalanche_intc_get_interrupt_type( unsigned int irq )
{
    unsigned int int_reg_indx = get_reg_index( irq );
    return(unsigned char)((avalanche_hw0_icregs->ictypr[int_reg_indx] >>
                                get_bit_position( irq ))&0x1);
}

/* low level INTC pol set */
int avalanche_intc_set_interrupt_polarity( unsigned int irq,
                                           unsigned char int_polarity)
{
    unsigned int int_reg_indx =  get_reg_index( irq );
    if(int_polarity)
    {
        avalanche_hw0_icregs->icpolr[int_reg_indx] |=
                        (1 << get_bit_position( irq ));
    }
    else
    {
        avalanche_hw0_icregs->icpolr[int_reg_indx] &=
                        ~(1 << get_bit_position( irq ));
    }
    return(0);
}

/* low level INTC pol get */
int avalanche_intc_get_interrupt_polarity ( unsigned int irq )
{
    unsigned int int_reg_indx = get_reg_index( irq );
    return(unsigned char)((avalanche_hw0_icregs->icpolr[int_reg_indx] >>
                            get_bit_position( irq ))&0x1);
}

#ifdef CONFIG_AVALANCHE_INTC_LEGACY_SUPPORT
EXPORT_SYMBOL(avalanche_intc_set_interrupt_type);
EXPORT_SYMBOL(avalanche_intc_get_interrupt_type);
EXPORT_SYMBOL(avalanche_intc_set_interrupt_polarity);
EXPORT_SYMBOL(avalanche_intc_get_interrupt_polarity);
#endif /* CONFIG_AVALANCHE_INTC_LEGACY_SUPPORT */

/* Secure Interrupt  Prioritized Index Register */
#ifdef CONFIG_AVALANCHE_INTC_SECURE_INTS
inline static unsigned int avalanche_intc_get_secure_priority_interrupt( void )
{
    return((unsigned int)(INTC_PRIORITY_INTERRUPT_FLAG &
                           avalanche_hw0_icregs->icsecpir ) );
}
#endif /* CONFIG_AVALANCHE_INTC_SECURE_INTS */

/* Security Enable and Disable regs  per interrupt */
#ifdef CONFIG_AVALANCHE_INTC_SECURE_INTS
static int avalanche_intc_enable_secure_interrupt( unsigned int irq )
{
    unsigned int int_reg_indx =  get_reg_index( irq );
    avalanche_hw0_icregs->icsecer[int_reg_indx]  |=
                     (1 << get_bit_position( irq ));
    return(0);
}


static int avalanche_intc_disable_secure_interrupt( unsigned int irq )
{
    unsigned int int_reg_indx =  get_reg_index( irq );
    avalanche_hw0_icregs->icsecer[int_reg_indx] &=
                    ~(1 << get_bit_position( irq ));
    return(0);
}
#endif /* CONFIG_AVALANCHE_INTC_SECURE_INTS */

/*****************************************************************************/

static inline void avalanche_intc_enable_irq( struct irq_data *data )
{
    avalanche_hw0_icregs->iceisr = data->irq;
}

static inline void avalanche_intc_disable_irq( struct irq_data *data )
{
    avalanche_hw0_icregs->ichinteicr = HOST_ARM_IRQ_NUM;
    avalanche_hw0_icregs->iceicr = data->irq;
    avalanche_hw0_icregs->ichinteisr = HOST_ARM_IRQ_NUM;
}

static unsigned int avalanche_intc_startup_irq( struct irq_data *data )
{
    /* clear already latched interrupts */
    avalanche_intc_clear_status(data->irq);

    /* Enable IRQ */
    avalanche_hw0_icregs->iceisr = data->irq;
    return(0);
}

static void avalanche_intc_end_irq( struct irq_data *data )
{
    if (! ( irq_desc[ data->irq ].status_use_accessors & ( IRQD_IRQ_DISABLED|IRQD_IRQ_INPROGRESS ) ) )
       avalanche_hw0_icregs->iceisr = data->irq; /* enable IRQ */
}

static void avalanche_intc_ack_irq( struct irq_data *data )
{
    /* clear status */
    avalanche_intc_clear_status(data->irq);
}


#define startup_avalanche_primary_irq         avalanche_intc_startup_irq
#define shutdown_avalanche_primary_irq        avalanche_intc_disable_irq
#define enable_avalanche_primary_irq          avalanche_intc_enable_irq
#define disable_avalanche_primary_irq         avalanche_intc_disable_irq
#define ack_avalanche_primary_irq             avalanche_intc_ack_irq
#define mask_avalanche_primary_irq            avalanche_intc_disable_irq
#define unmask_avalanche_primary_irq          avalanche_intc_enable_irq
#define end_avalanche_primary_irq             avalanche_intc_end_irq

static struct irq_chip avalanche_primary_irq_type = {
    .name           = "[INTC Primary]",
    .irq_startup        = startup_avalanche_primary_irq,
    .irq_shutdown       = shutdown_avalanche_primary_irq,
    .irq_disable        = disable_avalanche_primary_irq,
    .irq_unmask     = enable_avalanche_primary_irq,
    .irq_mask       = disable_avalanche_primary_irq,
    .irq_ack        = ack_avalanche_primary_irq,
    .irq_set_type       = avalanche_intc_set_type,
};

#define startup_avalanche_secondary_irq         avalanche_intc_startup_irq
#define shutdown_avalanche_secondary_irq        avalanche_intc_disable_irq
#define enable_avalanche_secondary_irq          avalanche_intc_enable_irq
#define disable_avalanche_secondary_irq         avalanche_intc_disable_irq
#define ack_avalanche_secondary_irq             avalanche_intc_ack_irq
#define mask_avalanche_secondary_irq            avalanche_intc_disable_irq
#define unmask_avalanche_secondary_irq          avalanche_intc_enable_irq
#define end_avalanche_secondary_irq             avalanche_intc_end_irq

static struct irq_chip avalanche_secondary_irq_type = {
    .name       = "[INTC Secondary]",
    .irq_startup    = startup_avalanche_secondary_irq,
    .irq_shutdown   = shutdown_avalanche_secondary_irq,
    .irq_disable    = disable_avalanche_secondary_irq,
    .irq_unmask     = enable_avalanche_secondary_irq,
    .irq_mask       = disable_avalanche_secondary_irq,
    .irq_ack        = ack_avalanche_secondary_irq,
    .irq_set_type   = avalanche_intc_set_type,
};

#ifdef CONFIG_AVALANCHE_INTC_PACING
int avalanche_intc_get_glb_pacer_mode ( void )
{
    ENTER
    EXIT
    return( INTC_GLOBAL_PACER_MODE_FLAG & avalanche_hw0_icregs->icpprer );
}
EXPORT_SYMBOL(avalanche_intc_get_glb_pacer_mode);

int avalanche_intc_set_glb_pacer_mode ( unsigned int glb_pacer_mode )
{
    ENTER
    if( glb_pacer_mode == INTC_GLOBAL_PACER_TEST_MODE  )
    {
        avalanche_hw0_icregs->icpprer |= INTC_GLOBAL_PACER_TEST_MODE;
    }
    else if( glb_pacer_mode == INTC_GLOBAL_PACER_BYPASSMODE )
    {
        avalanche_hw0_icregs->icpprer |= INTC_GLOBAL_PACER_BYPASSMODE;
    }
    else if( glb_pacer_mode == INTC_GLOBAL_PACER_NORMALMODE )
    {
        avalanche_hw0_icregs->icpprer &= INTC_GLOBAL_PACER_NORMALMODE ;
    }
    else
    {
        EXIT
        return(-1);
    }
    EXIT
    return(0);
}
EXPORT_SYMBOL(avalanche_intc_set_glb_pacer_mode);

int avalanche_intc_set_glb_pacer_prescale_count( unsigned int prescale_cnt )
{
    ENTER
    if( prescale_cnt > 0 && prescale_cnt < INTC_MAX_PRESCALE_COUNT &&
        !( avalanche_hw0_icregs->icpprer & INTC_GLOBAL_PACER_BYPASSMODE) )
    {
        avalanche_hw0_icregs->icpprer |= prescale_cnt;
    }
    else
    {
        EXIT
        return(-1);
    }
    EXIT
    return(0);
}
EXPORT_SYMBOL(avalanche_intc_set_glb_pacer_prescale_count);

int avalanche_intc_get_glb_pacer_prescale_count( void )
{
    ENTER
    EXIT
    return(INTC_PRESCALE_COUNT_FLAG &avalanche_hw0_icregs->icpprer);
}
EXPORT_SYMBOL(avalanche_intc_get_glb_pacer_prescale_count);

int avalanche_intc_enable_pacer_bypass( unsigned int pacer_num )
{
    ENTER
    if(  pacer_num >= 0  && pacer_num < INTC_MAX_NUMBER_PACERS )
    {
        avalanche_hw0_icregs->icpacer[pacer_num].icpparamr |=
                                      INTC_ENABLE_PACER_BYPASS;
    }
    else
    {
        EXIT
        return(-1);
    }
    EXIT
    return(0);
}
EXPORT_SYMBOL(avalanche_intc_enable_pacer_bypass);

int avalanche_intc_disable_pacer_bypass( unsigned int pacer_num )
{
    ENTER
    if(  pacer_num >= 0  && pacer_num < INTC_MAX_NUMBER_PACERS )
    {
        avalanche_hw0_icregs->icpacer[pacer_num].icpparamr &=
                                    INTC_DISABLE_PACER_BYPASS;
    }
    else
    {
       EXIT
       return(-1);
    }
    EXIT
    return(0);
}
EXPORT_SYMBOL(avalanche_intc_disable_pacer_bypass);

int avalanche_intc_set_pacer_mode ( unsigned int pacer_num,
                                    unsigned int  mode )
{
    ENTER
    if( pacer_num >=0 && pacer_num < INTC_MAX_NUMBER_PACERS &&
        ( mode == INTC_PACER_FREQ_BASED ||
          mode == INTC_PACER_TIME_BASED ||
          mode == INTC_PACER_COUNT_BASED ||
          mode == INTC_PACER_COUNT_AND_TIME_BASED ) )
    {
        avalanche_hw0_icregs->icpacer[pacer_num].icpparamr &= INTC_PACER_FREQ_BASED;
        if ( mode != INTC_PACER_FREQ_BASED )
        {
            avalanche_hw0_icregs->icpacer[pacer_num].icpparamr |= mode;
        }
    }
    else
    {
        EXIT
        return(-1);
    }
    EXIT
    return(0);
}
EXPORT_SYMBOL(avalanche_intc_set_pacer_mode);

int avalanche_intc_set_pacer_count( unsigned int pacer_num,
                                     unsigned int cnt_val )
{
    ENTER
    if( pacer_num >= 0  && pacer_num < INTC_MAX_NUMBER_PACERS &&
        cnt_val > 0 && cnt_val <= INTC_MAX_COUNT_VAL )
    {
       /* cleanup bits */
       avalanche_hw0_icregs->icpacer[pacer_num].icpparamr &=
                         (0xFF00FFFF);

       avalanche_hw0_icregs->icpacer[pacer_num].icpparamr |=
                         cnt_val << INTC_PACER_COUNT_BIT_POS;
    }
    else
   {
        EXIT
        return(-1);
   }
   EXIT
   return(0);
}
EXPORT_SYMBOL(avalanche_intc_set_pacer_count);

int avalanche_intc_get_pacer_count( unsigned int pacer_num )
{
    ENTER
    if( pacer_num >= 0  && pacer_num < INTC_MAX_NUMBER_PACERS )
    {
        EXIT
        return(( INTC_PACER_COUNT_FLAG &
                avalanche_hw0_icregs->icpacer[pacer_num].icpparamr) >>
                    INTC_PACER_COUNT_BIT_POS);
    }
    else
    {
        EXIT
        return(-1);
    }
}
EXPORT_SYMBOL(avalanche_intc_get_pacer_count);

int  avalanche_intc_set_pacer_restart_mode( unsigned int pacer_num  )
{
    ENTER
    if( pacer_num >= 0  && pacer_num < INTC_MAX_NUMBER_PACERS )
    {
        avalanche_hw0_icregs->icpacer[pacer_num].icpparamr |=
                                    INTC_ENABLE_PACER_RESTART;
    }
    else
    {
        EXIT
        return(-1);
    }
    EXIT
    return(0);
}
EXPORT_SYMBOL(avalanche_intc_set_pacer_restart_mode);

int avalanche_intc_clear_pacer_restart_mode( unsigned int pacer_num )
 {
    ENTER
    if( pacer_num >= 0  && pacer_num < INTC_MAX_NUMBER_PACERS )
    {
        avalanche_hw0_icregs->icpacer[pacer_num].icpparamr &=
                               INTC_DISABLE_PACER_RESTART;
    }
    else
    {
        EXIT
        return(-1);
    }
    EXIT
    return(0);
}
EXPORT_SYMBOL(avalanche_intc_clear_pacer_restart_mode);

int avalanche_intc_get_serviced_int_count( unsigned int pacer_num )
{
    ENTER
    if( pacer_num >= 0 && pacer_num < INTC_MAX_NUMBER_PACERS )
    {
        EXIT
        return(avalanche_hw0_icregs->icpacer[pacer_num].icpdecr);
    }
    else
    {
        EXIT
        return(-1);
    }
}
EXPORT_SYMBOL(avalanche_intc_get_serviced_int_count);

int avalanche_intc_set_serviced_int_count( unsigned int pacer_num,
                                            unsigned int cnt_val )
{
    ENTER
    if( pacer_num >=0 && pacer_num < INTC_MAX_NUMBER_PACERS &&
        cnt_val >=0 && cnt_val < 0xFF )
    {
    avalanche_hw0_icregs->icpacer[pacer_num].icpdecr = cnt_val;
    }
    else
    {
        EXIT
        return(-1);
    }
    EXIT
    return(0);
}
EXPORT_SYMBOL(avalanche_intc_set_serviced_int_count);

int avalanche_intc_set_pacer_max_val( unsigned int pacer_num,
                                      unsigned int max_value )
{
    ENTER
    if( pacer_num >= 0  && pacer_num < INTC_MAX_NUMBER_PACERS  &&
        max_value > 0 && max_value <= INTC_PACER_MAX_VAL )
    {
        /* cleanup bits */
        avalanche_hw0_icregs->icpacer[pacer_num].icpparamr &= 0xFFFFFF00;

        avalanche_hw0_icregs->icpacer[pacer_num].icpparamr |= max_value;
    }
    else
    {
        EXIT
        return(-1);
    }
    EXIT
    return(0);
}
EXPORT_SYMBOL(avalanche_intc_set_pacer_max_val);

int avalanche_intc_get_pacer_max_val ( unsigned int pacer_num )
{
    ENTER
    if(  pacer_num >= 0  && pacer_num < INTC_MAX_NUMBER_PACERS )
    {
        EXIT
        return( INTC_PACER_MAX_FLAG &
                avalanche_hw0_icregs->icpacer[pacer_num].icpparamr);
    }
    else
    {
        EXIT
        return(-1);
    }

 }
EXPORT_SYMBOL(avalanche_intc_get_pacer_max_val);

/* Pacer Map Register */
int avalanche_intc_map_interrupt_to_pacer( unsigned int irq,
                                           unsigned int pacer_num )
{
    ENTER
    if( irq >= 0 && irq < NUM_MAX_SYSTEM_INTS &&
        pacer_num >=0 && pacer_num < INTC_MAX_NUMBER_PACERS )
    {
        /* the reg may have override interrupt val, reset the reg */
         avalanche_hw0_icregs->icpacer[pacer_num].icpmap &= 0;
         avalanche_hw0_icregs->icpacer[pacer_num].icpmap =
                  irq  << ENABLING_PACING_BIT_POS;
    }
    else
    {
        EXIT
        return(-1);
    }
    EXIT
    return(0);
}
EXPORT_SYMBOL(avalanche_intc_map_interrupt_to_pacer);

int avalanche_intc_unmap_interrupt_to_pacer( unsigned int irq,
                                              unsigned int pacer_num )
{
    ENTER
    if( irq >= 0 && irq < NUM_MAX_SYSTEM_INTS &&
        pacer_num >=0 && pacer_num < INTC_MAX_NUMBER_PACERS )
    {
        /* if the reg may have enabled interrupt val,  reset the reg */
        avalanche_hw0_icregs->icpacer[pacer_num].icpmap &= 0;
        avalanche_hw0_icregs->icpacer[pacer_num].icpmap |= irq;
    }
    else
    {
       EXIT
       return(-1);
    }
    EXIT
    return(0);
}
EXPORT_SYMBOL(avalanche_intc_unmap_interrupt_to_pacer);
#endif
/*****************************************************************************/


/* Initializes the avalanche interrupt controller hardware to a sane
 * state.
 */
static void avalanche_init_intc_hw( void )
{
    unsigned int i,cntrl_ver;
    avalanche_hw0_icregs =
    (struct avalanche_ictrl_regs *)AVALANCHE_INTC_BASE;

    /* read revision */
    cntrl_ver = *(volatile unsigned int*)(AVALANCHE_INTC_BASE);
    printk("Interrupt controller revision : %x\n", cntrl_ver);

    /* Globally disable all hosts ints */
    avalanche_hw0_icregs->icglber = 0;

    /* disable host interrupts */
    for(i=0; i < NUM_ENABLE_HINT_REGS; i++ )
    {
        avalanche_hw0_icregs->ichinter[i] = 0x0;
    }
    /* disable system interrupts */
    for(i=0; i < NUM_ENABLE_CLEAR_REGS; i++ )
    {
        avalanche_hw0_icregs->icenbcr[i] = 0xFFFFFFFF;
    }

    /* set all s/m interrupt polarity to active high*/
    for(i=0; i < NUM_POLARITY_REGS; i++ )
    {
        avalanche_hw0_icregs->icpolr[i] = 0xFFFFFFFF;
    }
    /* set all s/m interrupt type to level*/
    for(i=0; i < NUM_TYPE_REGS; i++ )
    {
        avalanche_hw0_icregs->ictypr[i] =0x0;
    }

    /* disable nesting and wakeup mode */
    avalanche_hw0_icregs->iccntlr =( INTC_CNRTL_NO_NESTING |
                                     INTC_CNRTL_NORMAL_MODE ) ;

    /* disable vic */
    avalanche_hw0_icregs->ichcntlr = 0;

    /* enable only timer interrupt */
    avalanche_hw0_icregs->icenbsr[0] = 0x10;

    /* clear system interrupts */
    for(i=0; i < NUM_RAW_STATUS_REGS; i++ )
    {
        avalanche_hw0_icregs->icestar[i] = 0xFFFFFFFF;
    }
}



void __init arch_init_irq( void )
{
    int irq,chnl_num;
    unsigned int sys_to_chan_map[NUM_MAX_SYSTEM_INTS];
    ENTER

    /* Initialize the INTC hardware */
    avalanche_init_intc_hw();

    /* extern unsigned int sys_to_chan_map[NUM_MAX_SYSTEM_INTS];
     * Initilize the system int to channel map arry
     * This should be done outside this file
     * Currently all the int will be mapped to channel 0
     */
    memset(sys_to_chan_map, 0, (NUM_MAX_SYSTEM_INTS * sizeof(unsigned int)));

    /* map all system interrupts to channel 0 */
    for( irq = 0; irq < NUM_MAX_SYSTEM_INTS; irq++ )
    {
        avalanche_intc_map_system_int_to_channel(irq, sys_to_chan_map[irq]);
    }

    /* As VIC is disabled, All the channel will map to to either nFIQ or nIRQ.
     * ARM11 have only one IRQ line,so map all the channels to this single host
     * ARM11 IRQ line.
     */
    for( chnl_num = 0; chnl_num < (NUM_HOST_INT_MAP_REGS * NUM_CHANNELS_PER_REG); chnl_num++ )
    {
        avalanche_intc_map_channel_to_host_int(chnl_num, HOST_ARM_IRQ_NUM);
    }

    /* enable host interrupt */
    avalanche_hw0_icregs->ichinteisr = HOST_ARM_IRQ_NUM;

    /* setup the IRQ description array. */
    for (irq = 0; irq < AVALANCHE_INT_END; irq++)
    {
        if(irq < AVALANCHE_INT_END_PRIMARY)
            irq_set_chip(irq, &avalanche_primary_irq_type);
        else if (irq < AVALANCHE_INT_END_SECONDARY)
            irq_set_chip(irq, &avalanche_secondary_irq_type);
        else
            irq_desc[irq].irq_data.chip= NULL;

        set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
        irq_set_handler(irq, do_level_IRQ);
    }
    /* set global enable */
    avalanche_hw0_icregs->icglber = INTC_SET_BIT_0;

    /* Set Pacer global parameters */
    /* --------------------------- */
    /* Set globally to normal function (no bypass) */
    avalanche_intc_set_glb_pacer_mode(INTC_GLOBAL_PACER_NORMALMODE);

    /* Set interrupt counter prescaler, i.e. the number of VBUSP clocks in 4us. (VBUSP = 225M clocks per second --> in 15us we get 3375 clocks) */
    avalanche_intc_set_glb_pacer_prescale_count( (avalanche_get_intc_input_freq()/1000000) * 15 );

    EXIT
}

int do_print_developers_show(struct seq_file *m, void *v)
{
    seq_printf(m, "\n" \
    "\n" \
    "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" \
    "India Kernel Development Team (2007-)\n" \
    "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" \
    "Mansoor            - BasePort\n" \
    "Srinath/Mansoor    - Intc/Intd\n" \
    "Hemant             - Ethernet\n" \
    "Sekhar             - CPPI\n" \
    "Srinath            - I2C\n" \
    "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" \
    "\n" \
    "\n");
    return 0;
}

static int do_print_developers_open(struct inode *inode, struct file *file)
{
    return single_open(file, do_print_developers_show, NULL); 
}

static const struct file_operations proc_depelopers_ops = {
    .open           = do_print_developers_open,
    .read           = seq_read,
    .llseek         = seq_lseek,
    .release        = single_release,
};

void avalanche_proc_entries(void)
{
    proc_create("avalanche/developers", 0, NULL, &proc_depelopers_ops);
}



