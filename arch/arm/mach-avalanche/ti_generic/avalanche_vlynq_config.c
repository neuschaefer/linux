/*
 *
 * avalanche_vlynq_config.c
 * Description:
 * vlynq platform configuration file
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


#include "pal_vlynq.h"
#include "pal_vlynqIoctl.h"  
#include "vlynq_enum.h"
#include "avalanche_vlynq_config.h"

void vlynq_delay_wait(Uint32 count);

AV_VL_PRINTF vl_cfg_printf = NULL;
AV_VL_GETENV vl_cfg_getenv = NULL;

#define vl_printf if(vl_cfg_printf) vl_cfg_printf
#define vl_getenv vl_cfg_getenv
#ifdef CONFIG_ARM_AVALANCHE_SOC
#define KSEG1_ADDR(x)  IO_PHY2VIRT(x)
#endif

typedef struct
{
    VLYNQ_MODULE_T   *ustream_vlynq;
    VLYNQ_MODULE_T   *dstream_vlynq;

    Uint32           virt_dstream_vlynq_base;
    Uint32           virt_dstream_vlynq_portal;

    PAL_VLYNQ_HND    *vlynq_hnd;

    Uint32           reset_bit;
    Uint32           ustream_intr_vect;
    Uint32           dstream_intr_vect;
    Uint32           root;

    VLYNQ_SOC_T      *soc;
    Int32            instance;

    Bool             endian_swap;

    Uint32           dclk_div;                     /* down stream clock, local clock divisor */
    Uint32           dclk_dir;                     /* down stream clock, local clock direction */
    Uint32           uclk_div;                     /* up stream clock, peer clock divisor */
    Uint32           uclk_dir;                     /* up stream clock, peer clock direction */

} VLYNQ_ENUM_SOC_T;

#define MAX_VLYNQ_HOPS (MAX_VLYNQ_COUNT/MAX_ROOT_VLYNQ)
static VLYNQ_ENUM_SOC_T g_enum_soc[MAX_ROOT_VLYNQ][MAX_VLYNQ_HOPS + 1];
static unsigned int num_root_enumerated = 0;

static Uint32 host_endian;
static Uint32 error_enum_irq_id = 31;

static Int32 get_reset_bit(Uint32 root_base_addr, Uint32 hop,
		           void* handle, Uint32 *reset_bit);

extern unsigned int avalanche_mem_size;  /* Size of the memory available on the board. */

/*-----------------------------------------------------------------------------
 * 
 * vlynq_xxx_xxx( )                              - Enumeration functions. 
 * av_xxx_xxx( )/avalanche_xxx_xxx( )            - Configuration functions.
 * Other functions                               - Helper functions.
 * 
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Enumeration Routines.
 *----------------------------------------------------------------------------*/
static Int32 vlynq_adjust_portal_size(VLYNQ_MODULE_T *p_vlynq_mod,
		                      Uint32         *rem_window,
			              Bool           import)
{
    VLYNQ_SOC_T                 *p_soc = p_vlynq_mod->soc;
    VLYNQ_REGION_CFG_T *p_vlynq_region = p_vlynq_mod->region;
    VLYNQ_DEV_T           *p_vlynq_dev = p_vlynq_region->dev;
    VLYNQ_REGION_CFG_T       *p_region;
  
    for(p_region = p_soc->region_list; p_region->offset < 0xffffffff; p_region++)
    {
	VLYNQ_DEV_T* p_dev = p_region->dev;

	if((p_dev->import != import) || (p_dev == p_vlynq_dev))
	    continue;
	  
        if(*rem_window >= p_region->size)
            *rem_window -= p_region->size;
	else
	{
	    vl_printf("Error: Enumerating the portal for VLYNQ module=%s of %s.\n",
			 p_vlynq_dev->name, p_soc->name);
	    return (-1); /* We have issues. */
        }
    }

    p_vlynq_region->size = *rem_window;

    return (0);
}

static Int32 vlynq_enum_portal_size(VLYNQ_ENUM_SOC_T *p_enum_soc, 
		                    Uint32           *rem_dstream_window,
				    Uint32           *rem_ustream_window)
{
    VLYNQ_MODULE_T *p_vlynq = p_enum_soc->ustream_vlynq;

    if(p_vlynq)
    {
	/* ustream. */
        if(vlynq_adjust_portal_size(p_vlynq, rem_ustream_window, False))
	    return (-1);
    }

   p_vlynq = p_enum_soc->dstream_vlynq;
   if(p_vlynq)
   {
       /* dstream. */
       if(vlynq_adjust_portal_size(p_vlynq, rem_dstream_window, True))
           return (-1);
   }

   return (0);
}

static Int32 get_available_irq(Uint32 irq_map)
{
    Int32 ret_val = 0;

    while(irq_map & 0x1)
    {
        ret_val++;
	irq_map >>= 1;
    }

    return ((ret_val == 32) ? -1 : ret_val);
}

static Int32 vlynq_enum_irq_map(VLYNQ_ENUM_SOC_T *p_enum_soc,
			        Uint32           *irq_bit_map,
				Bool             import)
{
    VLYNQ_SOC_T      *p_soc = p_enum_soc->soc;
    Uint32          irq_map = *irq_bit_map;
    VLYNQ_IRQ_CFG_T  *p_irq = p_soc->irq_list;

    if(!p_irq)
    {
        vl_printf("No IRQ(s) associated with %s. No IRQ enum reqd.\n", 
		     p_soc->name);
	return (0);
    }

    for(p_irq = p_soc->irq_list; p_irq->hw_intr_line > -1; p_irq++)
    {
        Int32 new_irq = p_irq->irq;

        if(irq_map & (1 << new_irq))
	{
	    if((new_irq = get_available_irq(irq_map)) < 0)
	    {
	        *irq_bit_map = irq_map;
	        return (-1);	
 	    }

            p_irq->irq   = new_irq;                    
	}

        irq_map |= (1 << new_irq); 
    }

    *irq_bit_map = irq_map;

    return (0);
}

static Int32 av_map_irq_this_mod(PAL_VLYNQ_HND    *vlynq_hnd,
		                 VLYNQ_IRQ_CFG_T  *p_irq, 
				 Bool             import)
{
    while(p_irq->hw_intr_line > -1)
    {
        VLYNQ_DEV_T *p_dev = p_irq->dev;

	if(p_dev->import == import)
	    if(PAL_vlynqMapIrq(vlynq_hnd,  p_irq->hw_intr_line, 
			       p_irq->irq, p_dev->vlynq_dev_hnd))
	    {
	        vl_printf("Failed to map the vlynq hw irq:%d on chain irq:%d for %s\n", 
			     p_irq->hw_intr_line, p_irq->irq, p_dev->name);
                return (-1);
	    }

	p_irq++;
    }

    return (0);
}

static Int32 avalanche_map_irq(VLYNQ_ENUM_SOC_T      *p_enum_lsoc,
		               VLYNQ_ENUM_SOC_T      *p_enum_psoc)
{
    VLYNQ_SOC_T     *p_soc = p_enum_lsoc->soc;
    VLYNQ_IRQ_CFG_T *p_irq = p_soc->irq_list;

    if(p_irq)
        if(av_map_irq_this_mod(p_enum_lsoc->vlynq_hnd, p_irq, False))
	{
	    vl_printf("Failed to map dstream IRQ(s) for %s.\n", p_soc->name);
            return (-1);
        }

    p_soc = p_enum_psoc->soc;
    p_irq = p_soc->irq_list;

    if(p_irq)
        if(av_map_irq_this_mod(p_enum_lsoc->vlynq_hnd, p_irq, True))
        {
	    vl_printf("Failed to map ustream IRQ(s) for %s.\n", p_soc->name);
            return (-1);
	}

    return (0); 
}

static Int32 av_unmap_irq_this_mod(PAL_VLYNQ_HND    *vlynq_hnd,
		                   VLYNQ_IRQ_CFG_T  *p_irq,
				   Bool             import)
{
    while(p_irq->hw_intr_line > -1)
    {
        VLYNQ_DEV_T *p_dev = p_irq->dev;

        if(p_dev->import == import)
            if(PAL_vlynqUnMapIrq(vlynq_hnd, p_irq->irq,
			         p_dev->vlynq_dev_hnd))
	    {
                vl_printf("Failed to unmap chain irq:%d for %s.\n", 
			     p_irq->irq, p_dev->name);
	        return (-1);
            }

	p_irq++;
    }

    return (0);
}

static Int32 avalanche_unmap_irq(VLYNQ_ENUM_SOC_T  *p_enum_lsoc,
		                 VLYNQ_ENUM_SOC_T  *p_enum_psoc)
{
    VLYNQ_SOC_T     *p_soc = p_enum_lsoc->soc;
    VLYNQ_IRQ_CFG_T *p_irq = p_soc->irq_list;

    if(p_irq)
        if(av_unmap_irq_this_mod(p_enum_lsoc->vlynq_hnd, p_irq, False))
            return (-1);

    p_soc = p_enum_psoc->soc;
    p_irq = p_soc->irq_list;

    if(p_irq)
        if(av_unmap_irq_this_mod(p_enum_lsoc->vlynq_hnd, p_irq, True))
            return (-1);

    return (0); 
}

static PAL_VLYNQ_HND* avalanche_init_vlynq(VLYNQ_ENUM_SOC_T *p_enum_lsoc, 
		                           VLYNQ_ENUM_SOC_T *p_enum_psoc, 
				           Bool             enumerate,
				           Bool             *p_endian_swap)
{
    PAL_VLYNQ_CONFIG_T  vlynq_config;
    PAL_VLYNQ_HND       *p_vlynq_hnd;
    VLYNQ_MODULE_T      *p_lvlynq_mod = p_enum_lsoc->dstream_vlynq;
    VLYNQ_MODULE_T      *p_pvlynq_mod = p_enum_psoc->ustream_vlynq;
    VLYNQ_SOC_T         *p_lsoc       = p_enum_lsoc->soc;
    VLYNQ_SOC_T         *p_psoc       = p_enum_psoc->soc;

    /* Let's get done with the enumerations, if any. */
    if(enumerate)
    {
        p_enum_lsoc->dstream_intr_vect  = error_enum_irq_id--;
        p_enum_psoc->ustream_intr_vect  = error_enum_irq_id--;
    }

    /* Let the configuration begin. */
    vlynq_config.base_addr              = p_enum_lsoc->virt_dstream_vlynq_base;

    if(p_enum_lsoc->root)
    {
        vlynq_config.on_soc             = True;
        vlynq_config.local_int2cfg      = 1;
        vlynq_config.local_intr_pointer = 0x14;
    }
    else
    {
	VLYNQ_MODULE_T *p_sibling_vlynq = p_enum_lsoc->ustream_vlynq;

	vlynq_config.on_soc             = False;
        vlynq_config.local_int2cfg      = 0;
        vlynq_config.local_intr_pointer = p_sibling_vlynq->phy_addr + 0x14;
    }

    vlynq_config.init_swap_flag        = *p_endian_swap; 

    vlynq_config.local_clock_dir        = p_enum_lsoc->dclk_dir;
    vlynq_config.local_clock_div        = p_enum_lsoc->dclk_div;
    vlynq_config.local_intr_local       = 0x1;
    vlynq_config.local_intr_vector      = p_enum_lsoc->dstream_intr_vect;
    vlynq_config.local_intr_enable      = 1;
    vlynq_config.local_endianness       = pal_vlynq_little_en; /* default, check below. */
    vlynq_config.local_tx_addr          = p_lvlynq_mod->region->offset; 
    vlynq_config.local_rtm_cfg_type     = no_rtm_cfg;
    vlynq_config.local_rtm_sample_value = 0; /* Do not care as rtm_cfg_type is no_rtm_cfg. */
    vlynq_config.local_tx_fast_path     = False;	    

    vlynq_config.peer_clock_dir         = p_enum_psoc->uclk_dir;
    vlynq_config.peer_clock_div         = p_enum_psoc->uclk_div;
    vlynq_config.peer_intr_local        = 0x0;
    vlynq_config.peer_intr_vector       = p_enum_psoc->ustream_intr_vect;
    vlynq_config.peer_intr_enable       = 1;
    vlynq_config.peer_int2cfg           = 0;
    vlynq_config.peer_intr_pointer      = 0x14;    
    vlynq_config.peer_endianness        = pal_vlynq_little_en; /* default, check below. */
    vlynq_config.peer_tx_addr           = p_pvlynq_mod->region->offset;
    vlynq_config.peer_rtm_cfg_type      = no_rtm_cfg;
    vlynq_config.peer_rtm_sample_value  = 0; /* Do not care as rtm_cfg_type is no_rtm_cfg. */
    vlynq_config.peer_tx_fast_path      = False;
       
    /* Add logic for Endianness. */
   if((p_lsoc->endian_map & host_endian) != (p_psoc->endian_map & host_endian))
   {
	if(*p_endian_swap) 
	    vlynq_config.local_endianness = pal_vlynq_big_en; 
	else
	    vlynq_config.peer_endianness  = pal_vlynq_big_en;

    	*p_endian_swap = *p_endian_swap ? False : True;
    }

    p_vlynq_hnd = PAL_vlynqInit(&vlynq_config);

    if(!p_vlynq_hnd) 
        vl_printf("Vlynq init failed because of %s.\n", vlynq_config.error_msg);

    return (p_vlynq_hnd);
}

static Int32 av_map_region_this_vlynq_mod(PAL_VLYNQ_HND  *p_vlynq_hnd, 
                                          VLYNQ_MODULE_T *p_vlynq_mod, 
		                          Int32          reset_bit,
                                          Int32          instance,
		                          Bool           import)
{
    VLYNQ_SOC_T           *p_soc = p_vlynq_mod->soc;
    VLYNQ_REGION_CFG_T *p_region = p_soc->region_list;
    VLYNQ_DEV_T           *p_dev;
    Int32    region_index = 0;


    if(!p_region)
        return (-1);

    for(p_dev = p_soc->dev_list; p_dev->name; p_dev++)
    {
	if(p_dev->import != import)
	    continue;

	p_dev->vlynq_dev_hnd = PAL_vlynqDevCreate(p_vlynq_hnd, p_dev->name, 
			                          instance, 
						  reset_bit,  p_dev->import);
	
	if(!p_dev->vlynq_dev_hnd)
        {	
            vl_printf("Failed to create dev handle for %s on %s.\n", 
			 p_dev->name, p_soc->name);
            return (-1);
        }
	    
	if(PAL_vlynqAddDevice(p_vlynq_hnd, p_dev->vlynq_dev_hnd, p_dev->import))
	{
	    vl_printf("Failed to attach dev handle for %s to %s.\n",
                         p_dev->name, p_soc->name);
	    return (-1);
        }

	for(p_region = p_soc->region_list; 
	    (p_region->offset < 0xffffffff) && (region_index < 4);
	    p_region++) 
	{
            if(p_region->dev != p_dev)
                continue;

            /*HPVL*/
            vl_printf ("VLYNQ: Mapping region with offset %#x\n", p_region->offset);
            
            if(PAL_vlynqMapRegion(p_vlynq_hnd, True /* Always exporting regions, refer to implementation */, 
                                  region_index, p_region->offset, p_region->size, 
				  p_dev->vlynq_dev_hnd))
	    {
                vl_printf("Failed to map %s regions for %s.\n", 
			     import ? "dstream" : "ustream", p_dev->name);
                vl_printf("Failed to export regions of %s to %s.\n", 
			     p_soc->name, import ? "dstream" : "ustream"); 
                p_region->offset = 0xffffffff;
		return (-1);
	    }

	    region_index++;
        }
    } 

    return (0);
}

static Int32 av_unmap_region_this_vlynq_mod(PAL_VLYNQ_HND  *p_vlynq_hnd, 
                                            VLYNQ_MODULE_T *p_vlynq_mod, 
		                            Bool           import)
{
    VLYNQ_SOC_T           *p_soc = p_vlynq_mod->soc;
    VLYNQ_REGION_CFG_T *p_region = p_soc->region_list; 
    Int32    region_index = 0;
    VLYNQ_DEV_T   *p_dev  = p_soc->dev_list;

    if(!p_region) return (-1);

    for(p_dev = p_soc->dev_list;p_dev->name; p_dev++)
    {
        if(p_dev->import != import)
	    continue;

	for(p_region = p_soc->region_list; 
	    (p_region->offset < 0xffffffff) && (region_index < 4);
	    p_region++)
	{
            if(p_region->dev != p_dev)
                continue;

            if(PAL_vlynqUnMapRegion(p_vlynq_hnd, True /*p_dev->import*/, region_index,
				    p_dev->vlynq_dev_hnd))
	    {
                p_region->offset = 0xffffffff;
		vl_printf("Error in unmapping the region for %s on %s.\n", 
			     p_dev->name, p_soc->name);	
		return (-1);
	    }

	    region_index++;
        }
	
	if(PAL_vlynqRemoveDevice(p_vlynq_hnd, p_dev->vlynq_dev_hnd))
	{
	    vl_printf("Failed to remove the device %s from %s.\n", p_dev->name, p_soc->name);
	    return (-1);
	}

	if(PAL_vlynqDevDestroy(p_dev->vlynq_dev_hnd))
        {
            vl_printf("Failed to destroy the device %s.\n", p_dev->name);
            return (-1);
        }

    } 

    return (0);
}

static Int32 avalanche_map_vlynq_region(VLYNQ_ENUM_SOC_T *p_enum_lsoc,
		                        VLYNQ_ENUM_SOC_T *p_enum_psoc)
{
    if(av_map_region_this_vlynq_mod(p_enum_lsoc->vlynq_hnd,
			            p_enum_psoc->ustream_vlynq,
				    p_enum_psoc->reset_bit,
                                    p_enum_psoc->instance,
				    True))
        return (-1);

    if(av_map_region_this_vlynq_mod(p_enum_lsoc->vlynq_hnd,
                                    p_enum_lsoc->dstream_vlynq,
                                    p_enum_lsoc->reset_bit,
                                    p_enum_lsoc->instance,
                                    False))
        return (-1);

    return (0);
}

static Int32 avalanche_unmap_vlynq_region(VLYNQ_ENUM_SOC_T *p_enum_lsoc,
		                          VLYNQ_ENUM_SOC_T *p_enum_psoc)
{

    if(av_unmap_region_this_vlynq_mod(p_enum_lsoc->vlynq_hnd,
			              p_enum_lsoc->dstream_vlynq,
				      False))
        return (-1);


    if(av_unmap_region_this_vlynq_mod(p_enum_lsoc->vlynq_hnd,
			              p_enum_psoc->ustream_vlynq,
				      True))
        return (-1);

    return (0);
}

static Int32 avalanche_vlynq_setup(VLYNQ_ENUM_SOC_T *p_enum_lsoc, 
		                   VLYNQ_ENUM_SOC_T *p_enum_psoc, 
				   Bool             enumerate,
				   Bool             *p_endian_swap)
{
    VLYNQ_ENUM_SOC_T  *p_enum_soc_prev = NULL; 

    p_enum_lsoc->vlynq_hnd = avalanche_init_vlynq(p_enum_lsoc, p_enum_psoc,
						  enumerate, p_endian_swap);

    if(!p_enum_lsoc->vlynq_hnd)
    {
        vl_printf("Failed to initalize vlynq bridge for %s <---> %s.\n",
                     p_enum_lsoc->soc->name, p_enum_psoc->soc->name);
        return (-1);
    }

    if(!p_enum_lsoc->root)
        p_enum_soc_prev = p_enum_lsoc - 1;

    if(p_enum_soc_prev && PAL_vlynqChainAppend(p_enum_lsoc->vlynq_hnd, 
			                       p_enum_soc_prev->vlynq_hnd))
    {
        vl_printf("Failed to append %s to %s for the chaining.\n", 
		     p_enum_lsoc->soc->name, p_enum_soc_prev->soc->name);
        return (-1);
    }

    if(avalanche_map_vlynq_region(p_enum_lsoc, p_enum_psoc /*, enumerate*/))
    {
	vl_printf("Error in mapping VLYNQ regions across %s <---> %s.\n",
                     p_enum_lsoc->soc->name, p_enum_psoc->soc->name);
        return (-1);
    }
                                                        
    if(avalanche_map_irq(p_enum_lsoc, p_enum_psoc))
    {
        vl_printf("Error in mapping IRQ(s) across %s <---> %s.\n", 
                     p_enum_lsoc->soc->name, p_enum_psoc->soc->name);
	return (-1);
    }

    return (0); 
}

/* HPVL : Moved here for use in teardown sequence */
static Uint32 lo_reset_enum = 64;
#ifdef AVALANCHE_HIGH_VLYNQ_CONTROL_BASE
static Uint32 hi_reset_enum = 96;
#endif

static Int32 avalanche_vlynq_teardown(VLYNQ_ENUM_SOC_T *p_enum_lsoc, 
		                      VLYNQ_ENUM_SOC_T *p_enum_psoc)
{
    VLYNQ_ENUM_SOC_T  *p_enum_soc_prev = NULL; 

    
    if(!PAL_vlynqIsLast(p_enum_lsoc->vlynq_hnd))
    {
        vl_printf("Error: %s is NOT the last entity in the chain.\n", 
		     p_enum_psoc->soc->name);
        return (-1);
    }
	    
    if(avalanche_unmap_irq(p_enum_lsoc, p_enum_psoc))
    {
        vl_printf("Failed to unmap the IRQ(s) between %s <---> %s.\n",
                     p_enum_lsoc->soc->name, p_enum_psoc->soc->name);
        return (-1);
    }

    if(avalanche_unmap_vlynq_region(p_enum_lsoc, p_enum_psoc))
    {
        vl_printf("Failed to unmap the regions between %s <---> %s.\n",
		     p_enum_lsoc->soc->name, p_enum_psoc->soc->name);
        return (-1);
    }

    if(!p_enum_lsoc->root)
        p_enum_soc_prev = p_enum_lsoc - 1; 
    else
    {
        /* HPVL - Cleanup for root. This allows re-init in suspend-resume kind of
         * scenario
         */
        unsigned int base_addr;
        Uint32       *p_reset_enum = NULL;
        num_root_enumerated--;
        PAL_vlynqGetBaseAddr(p_enum_lsoc->vlynq_hnd, &base_addr);

        if(base_addr == AVALANCHE_LOW_VLYNQ_CONTROL_BASE)
            p_reset_enum = &lo_reset_enum;
#ifdef AVALANCHE_HIGH_VLYNQ_CONTROL_BASE
        else if (base_addr == AVALANCHE_HIGH_VLYNQ_CONTROL_BASE)
            p_reset_enum = &hi_reset_enum;
#endif
        /* NOTE : This forces the de-init of roots should be carried out in
         * reverse order of their initialization. 
         * TODO : Check if this works!
         */
        if (p_reset_enum)
            (*p_reset_enum)--;
        error_enum_irq_id += 2; /* CHK: Handle the case when peer not present */
    }		    
    
    if(p_enum_soc_prev && PAL_vlynqChainUnAppend(p_enum_lsoc->vlynq_hnd, 
			                         p_enum_soc_prev->vlynq_hnd))
    {
        vl_printf("Failed to detach %s from %s in the chain. \n", 
                     p_enum_lsoc->soc->name, p_enum_soc_prev->soc->name);
        return (-1); 
    }	

    if(PAL_vlynqCleanUp(p_enum_lsoc->vlynq_hnd))
    {
        vl_printf("Failed to clean up the VLYNQ instance for %s.\n",
		     p_enum_lsoc->soc->name);
        return (-1);
    }

    p_enum_lsoc->vlynq_hnd = NULL;

    return (0);
}

static long get_atol(const char *ptr)
{
    long val = 0;
    
    while(*ptr)
    {
        val = val*10 + (*ptr - '0');
	ptr++;
    }

    return (val);
}

static Uint32 parse_vlynq_config(char *cfg_str, VLYNQ_ENUM_SOC_T *p_enum_lsoc, 
                                 VLYNQ_ENUM_SOC_T *p_enum_psoc)
{
//    char  *substr = strtok(cfg_str, ":");
    char  *substr = (char *)strsep (&cfg_str, ":");
       
    int cfg_index = 0;

    if(!substr)
        return (-1);

    while(substr)
    {
	switch(cfg_index)
        {
            case 0:
                p_enum_psoc->uclk_dir = get_atol(substr);
		p_enum_lsoc->dclk_dir = !p_enum_psoc->uclk_dir;
                break;

            case 1:
		if(p_enum_psoc->uclk_dir)
                {
                    p_enum_psoc->uclk_div = get_atol(substr);
                    p_enum_lsoc->dclk_div = 1;
                }
                else
                {
                    p_enum_lsoc->dclk_div = get_atol(substr);
                    p_enum_psoc->uclk_div = 1;
                }
                break;
 
            default:
		break;
        }

 //       substr = strtok(NULL, ":");
        substr = (char *)strsep (&cfg_str, ":");
	cfg_index++;
    }

    return (0);
}

static Uint32 detect_host_endianness(void)
{
    union endian_detect
    {
        int  i_val;
	char c_val;
    };

    union endian_detect en;
    en.i_val = 0x1;

    if(en.c_val == 0x1) 
        return LITTLE_EN;
    else
	return BIG_EN;
}

typedef struct 
{
    char *name;
    int  count;

} CLASS_REF_COUNT;

static CLASS_REF_COUNT 
class_ref_cnt[ ] = {
                       { HOST_CLASS_NAME, 0},
                       { VDSP_CLASS_NAME, 0},
                       { WLAN_CLASS_NAME, 0},
                       { NULL,            0}
                   };

static Int32 vlynq_get_class_instance(char *name, Int32 *instance)
{
    CLASS_REF_COUNT *p_ref_cnt = &class_ref_cnt[0];
    Int32 ret = -1;

    while(p_ref_cnt->name)
    {
        if(!strcmp(p_ref_cnt->name, name))
        {
            *instance = p_ref_cnt->count;
            p_ref_cnt->count++;
            ret = 0;
            break;
        }

        p_ref_cnt++;
    }

    return (ret);
}

static Int32 vlynq_identify_next_portal(VLYNQ_ENUM_SOC_T  *p_enum_lsoc,
		                        VLYNQ_ENUM_SOC_T  *p_enum_psoc,
					Bool              endian_swap)
{
    VLYNQ_MODULE_T *p_peer_lvlynq    = p_enum_psoc->soc->lo_vlynq;
    VLYNQ_MODULE_T *p_peer_hvlynq    = p_enum_psoc->soc->hi_vlynq;
    VLYNQ_MODULE_T *p_dstream_vlynq  = p_enum_lsoc->dstream_vlynq;
    VLYNQ_REGION_CFG_T *p_region     = p_dstream_vlynq->region;
    Uint32         vlynq_phy_addr;

    if(PAL_sysVlynqDetectPeerPortal(p_enum_lsoc->virt_dstream_vlynq_base, 
                                    p_region->offset, p_enum_lsoc->virt_dstream_vlynq_portal,
			            p_peer_lvlynq->phy_addr, p_peer_hvlynq->phy_addr,
				    endian_swap,   &vlynq_phy_addr))
    {
        vl_printf("Could not identify the portal of %s to which %s is connected to. \
		     Aborting.\n", p_enum_lsoc->soc->name, p_enum_psoc->soc->name);
	// Suraj, consistent naming.??????
	return (-1);
    }

    if(vlynq_phy_addr == p_peer_hvlynq->phy_addr)
    {
        p_enum_psoc->ustream_vlynq = p_peer_hvlynq; 
	p_enum_psoc->dstream_vlynq = p_peer_lvlynq;
    }	
    else if (vlynq_phy_addr == p_peer_lvlynq->phy_addr)
    {
	p_enum_psoc->ustream_vlynq = p_peer_lvlynq;
	p_enum_psoc->dstream_vlynq = p_peer_hvlynq;
    }	
    else
    {
	vl_printf("Fatal: Unidentified portal. This should NOT happen. Aborting!!!.\n");
	return (-1);
    }

    p_enum_psoc->dstream_vlynq->region->dev->import = import_true; /*import_false; */ 
    p_enum_psoc->ustream_vlynq->region->dev->import = import_false;/* Suraj check*/ 

    return (0);
}

static VLYNQ_ENUM_SOC_T *vlynq_enumerate_next(VLYNQ_ENUM_SOC_T *p_enum_lsoc, 
		                              Uint32 *rem_ustream_window,
		                              Uint32 *rem_dstream_window, 
				              Uint32 *irq_bit_map,
			                      Bool   endian_swap)
{
    Uint32            peer_id,  cnt_word;
    VLYNQ_SOC_T*      p_lsoc = p_enum_lsoc->soc;
    VLYNQ_SOC_T*      p_psoc;
    VLYNQ_SOC_T**     pp_psoc;
    VLYNQ_ENUM_SOC_T* p_enum_psoc;

    vl_printf("Identifying the neighbour connected to %s <0x%x>.\n", 
		 p_lsoc->name, p_enum_lsoc->virt_dstream_vlynq_base);

    if(PAL_sysVlynqDetectPeer(p_enum_lsoc->virt_dstream_vlynq_base, 
			      endian_swap, &peer_id, &cnt_word))
    {
        vl_printf("Did not detect any SoC next to %s in the chain. \n", 
		    p_lsoc->name);
	return (NULL);
    }

    /* hack --> hemant HPVL */ 
    for(pp_psoc = &soc_list[1], p_psoc = *pp_psoc; p_psoc; pp_psoc++, p_psoc = *pp_psoc)
        if(p_psoc->soc_id == (peer_id & 0xFFFF))
	    break;

    if(!p_psoc)
    {
        vl_printf("Error: Info about VLYNQ SoC with id %u could NOT be found.\n", 
                     peer_id & 0xFFFF);
	return (NULL);
    }

    vl_printf("Detected the VLYNQ SoC with id 0x%04x and named %s.\n",
                 peer_id & 0xFFFF, p_psoc->name);

    p_enum_lsoc->dclk_dir          = ((cnt_word >> 15) & 0x1) ? 1:0; 
    p_enum_lsoc->dclk_div          = ((cnt_word >> 16) & 0x7) + 1; 

    p_enum_psoc                    = p_enum_lsoc + 1;
    p_enum_psoc->soc               = p_psoc;
    p_enum_psoc->root              = False;
    p_enum_psoc->uclk_dir          = !p_enum_lsoc->dclk_dir;
    p_enum_psoc->uclk_div          = 1;

    p_psoc->lo_vlynq->soc          = p_psoc;
    if(!p_psoc->hi_vlynq)
    {
        vl_printf("Detected SoC %s has just one VLYNQ module.\n", p_psoc->name);
	p_enum_psoc->ustream_vlynq = p_psoc->lo_vlynq;
	p_enum_psoc->dstream_vlynq = p_psoc->hi_vlynq;
        //HPVL *** p_enum_psoc->ustream_vlynq->region->dev->import = import_true;
        return (p_enum_psoc);
    }

    p_psoc->hi_vlynq->soc      = p_psoc;

    if(vlynq_identify_next_portal(p_enum_lsoc, p_enum_psoc, endian_swap))
    {
        vl_printf("Failed to identify the portal on %s to which %s is connected.\n",
                     p_psoc->name, p_lsoc->name);
        return (NULL); 
    }

    if(vlynq_enum_portal_size(p_enum_psoc, rem_dstream_window,
			      rem_ustream_window))
    {
	vl_printf("Failed to enumerate the portal(s) size for %s <---> %s.\n", 
		     p_lsoc->name, p_psoc->name);
        return (NULL);
    }

    if(vlynq_enum_irq_map(p_enum_psoc,irq_bit_map, True))
    {
	vl_printf("Failed to eumerate the IRQ(s) for %s <---> %s.\n",
		     p_lsoc->name, p_psoc->name);	
        return (NULL);
    }

    return (p_enum_psoc);
}

/* This is really a fixup. In general you do not get boards which have more than
 * 32MB of SDRAM, but once in a while you run into one of rich boards which has
 * more than 64MB likes of 128MB etc. 
 */
static Uint32 vlynq_fixup_host_sdram_val(VLYNQ_SOC_T *p_soc)
{
    Uint32                  ret_val = VL_PORTAL_SIZE;
    VLYNQ_REGION_CFG_T    *p_region = p_soc->region_list;

    while(p_region->offset < 0xffffffff)
    {
        if((p_region->offset == AVALANCHE_SDRAM_BASE) && 
           (avalanche_mem_size >  VL_PORTAL_SIZE))
        {
            p_region->size = avalanche_mem_size;
            ret_val        = avalanche_mem_size;
            vl_printf("Fixed memory available for %s as 0x%x.\n", p_soc->name,
                       avalanche_mem_size);
            break;
        }

        p_region++;
    }

    return (ret_val);
}

Int32 avalanche_vlynq_enumerate(Uint32 cvr, Uint32 root_vlynq_virt_base, 
		                Uint32 tx_virt_portal)
{
    VLYNQ_ENUM_SOC_T *p_enum_lsoc, *p_enum_psoc; 
    VLYNQ_SOC_T      *p_soc;
    VLYNQ_SOC_T      **pp_soc;
    VLYNQ_MODULE_T   *p_lvlynq, *p_hvlynq;
    Uint32            outstanding_dstream_window = VL_PORTAL_SIZE; 
    Uint32            outstanding_ustream_window = VL_PORTAL_SIZE; 
    Uint32            irq_enum_bit_map           = 0;
    Uint32            vlynq_virt_base            = root_vlynq_virt_base;
    Uint32            hop                        = 0;
    Bool              endian_swap                = False;
    char              class_name[50];
    char             *cfg_str;

    for(pp_soc = &soc_list[0], p_soc = *pp_soc; p_soc; pp_soc++, p_soc = *pp_soc)
        if(p_soc->soc_id == (cvr & 0xFFFF))
	    break;

    if(!p_soc)
    {
        vl_printf("Info about Root SoC with Id=%u could NOT be found. Aborting VLYNQ enumeration!!!.\n", 
		    cvr & 0xFFFF);	
        return (-1);
    }

    /* In case, we have more than VL_PORTAL_SIZE. */
    outstanding_dstream_window = vlynq_fixup_host_sdram_val(p_soc);

    vl_printf("Enumerating for the host SOC %s's VLYNQ module @ 0x%x.\n", 
		 p_soc->name, root_vlynq_virt_base);

    p_lvlynq = p_soc->lo_vlynq;
    p_hvlynq = p_soc->hi_vlynq;

    /* Add code to get the enum_soc. Needs update. */
    if(num_root_enumerated >= MAX_ROOT_VLYNQ)
        return (-1);

    p_enum_lsoc = &g_enum_soc[num_root_enumerated++][0];

    /* Now, identify the root vlynq module on which we have to work. */
    if(KSEG1_ADDR(p_lvlynq->phy_addr) == root_vlynq_virt_base)
        p_enum_lsoc->dstream_vlynq = p_soc->lo_vlynq;
    else if(p_hvlynq && KSEG1_ADDR(p_hvlynq->phy_addr) == root_vlynq_virt_base)
	p_enum_lsoc->dstream_vlynq = p_soc->hi_vlynq;
    else
    {
        vl_printf("Fatal Error, this should not have happened.\n");
	return (-1); /* We have serious issues. */
    }

    p_enum_lsoc->dstream_vlynq->soc = p_soc;
    p_enum_lsoc->ustream_vlynq      = NULL;
    p_enum_lsoc->root               = True;
    p_enum_lsoc->soc                = p_soc;
    p_enum_lsoc->endian_swap        = endian_swap;
    
    if(vlynq_get_class_instance(p_soc->class_name, &p_enum_lsoc->instance))
        return (-1);

    host_endian = detect_host_endianness( );

    if(vlynq_enum_portal_size(p_enum_lsoc, &outstanding_dstream_window, 
			      &outstanding_ustream_window))
    {
        vl_printf("Fail: Enumerating the portals. Aborting Enumeration !!!!\n");
	return (-1);
    }

    p_enum_lsoc->dstream_vlynq->region->dev->import = import_true;

    while(1)
    {
	VLYNQ_MODULE_T *p_dstream_vlynq;
        VLYNQ_DEV_T    *p_dstream_dev; 

	p_enum_lsoc->virt_dstream_vlynq_base   = vlynq_virt_base;
	p_enum_lsoc->virt_dstream_vlynq_portal = tx_virt_portal;

	/* Let us find and enumerate the next "peer" SoC in the chain. */
	p_enum_psoc = vlynq_enumerate_next(p_enum_lsoc, 
			                   &outstanding_dstream_window,
					   &outstanding_ustream_window, 
					   &irq_enum_bit_map, endian_swap);
        if(!p_enum_psoc)
	{
            /* HPVL - Cleanup for root. This allows re-init in suspend-resume kind of
             * scenario
             */
            unsigned int base_addr;
            Uint32       *p_reset_enum = NULL;
            num_root_enumerated--;
            PAL_vlynqGetBaseAddr(p_enum_lsoc->vlynq_hnd, &base_addr);

            if(base_addr == AVALANCHE_LOW_VLYNQ_CONTROL_BASE)
                p_reset_enum = &lo_reset_enum;
#ifdef AVALANCHE_HIGH_VLYNQ_CONTROL_BASE
            else if (base_addr == AVALANCHE_HIGH_VLYNQ_CONTROL_BASE)
                p_reset_enum = &hi_reset_enum;
#endif
            /* NOTE : This forces the de-init of roots should be carried out in
             * reverse order of their initialization. 
             * TODO : Check if this works!
             */
            if (p_reset_enum)
                (*p_reset_enum)--;
	    
            vl_printf("VLYNQ Enumeration ended on the chain for root@ 0x%08x for %s.\n",
		         vlynq_virt_base, p_soc->name); 
	    return (0);
	}

	vl_printf("Enumerated %s <---> %s, now configuring .....\n", 
		     p_enum_lsoc->soc->name, p_enum_psoc->soc->name);

        if(vlynq_get_class_instance(p_enum_psoc->soc->class_name, 
                                    &p_enum_psoc->instance))
            return (-1);

        sprintf(class_name, "vl_%s%d", p_enum_psoc->soc->class_name,
                p_enum_psoc->instance);

        cfg_str = vl_getenv(class_name);
	if(cfg_str)
        {
            vl_printf("Reading vlynq config from the env %s\n", cfg_str);
	    strcpy(class_name, cfg_str);
            parse_vlynq_config(class_name, p_enum_lsoc, p_enum_psoc); 
        }

        get_reset_bit(root_vlynq_virt_base, hop, p_enum_lsoc, 
		      &p_enum_psoc->reset_bit);

        /* Set up the VLYNQ bridge between this SoC and peer SoC. Updates endian_swap */
	if(avalanche_vlynq_setup(p_enum_lsoc, p_enum_psoc, True, &endian_swap))
	{
	// suraj    p_enum_lsoc->error = True;
	    vl_printf("Fail: config of the bridge for %s <---> %s Abort!!.\n",
			 p_enum_lsoc->soc->name, p_enum_psoc->soc->name);
            break;	
	}

	p_enum_psoc->endian_swap = endian_swap;
        p_dstream_vlynq = p_enum_psoc->dstream_vlynq;
	if(!p_dstream_vlynq)
	{
	    vl_printf("VLYNQ Enumeration ceasing as %s can not have further links.\n\n",
		         p_enum_psoc->soc->name); 
            return (0);
	}

        p_dstream_dev = p_dstream_vlynq->region->dev;

	if(PAL_vlynqGetDevBase(p_enum_lsoc->vlynq_hnd, 
                               p_dstream_vlynq->phy_addr,
			       &vlynq_virt_base, p_dstream_dev->vlynq_dev_hnd))
	{
	    /* Error, we do not have mapped phy addr of dstream VLYNQ mod. */
            vl_printf("Aborting: No addr for downstream vlynq of %s.\n", 
			 p_enum_psoc->soc->name);
	    break;
        }

	if(PAL_vlynqGetDevBase(p_enum_lsoc->vlynq_hnd, 
                               p_dstream_vlynq->region->offset,
			       &tx_virt_portal, p_dstream_dev->vlynq_dev_hnd))
        {
	    /* Error, we do not have mapped phy addr of dstream VLYNQ portal */
            vl_printf("Aborting: No addr for downstream portal of %s.\n", 
			 p_enum_psoc->soc->name);
	    break;
        }

	vl_printf("..... Configuration Done.\n");
	p_enum_lsoc      = p_enum_psoc;

	vlynq_virt_base  = KSEG1_ADDR(vlynq_virt_base);
        tx_virt_portal   = KSEG1_ADDR(tx_virt_portal);
	hop++;
    }

    vl_printf("Error: In enum/config of %s <--> %s. Aborting!!!.\n", 
	         p_enum_lsoc->soc->name, p_enum_psoc->soc->name);
    return (-1);
}

static Int32 dev_setup(void* handle)
{
    VLYNQ_ENUM_SOC_T *p_enum_lsoc = handle;
    VLYNQ_ENUM_SOC_T *p_enum_psoc = p_enum_lsoc + 1;
    Bool              endian_swap = p_enum_lsoc->endian_swap;

    vl_printf("%s <---> %s is being set up....\n", p_enum_lsoc->soc->name,
               p_enum_psoc->soc->name);
 
    if(avalanche_vlynq_setup(p_enum_lsoc, p_enum_psoc, 
                             False, &endian_swap))
    {
        vl_printf(".....Failed.\n");
        return (-1);
    }  

    p_enum_psoc->endian_swap = endian_swap;

    vl_printf(".....Complete.\n");
    return (0);
}

static Int32 dev_teardown(void *handle)
{
    VLYNQ_ENUM_SOC_T *p_enum_lsoc = handle;
    VLYNQ_ENUM_SOC_T *p_enum_psoc = p_enum_lsoc + 1;

    vl_printf("%s <---> %s is going down.....\n", p_enum_lsoc->soc->name,
               p_enum_psoc->soc->name);

    if(avalanche_vlynq_teardown(p_enum_lsoc, p_enum_psoc))
    {
        vl_printf("....Failed.\n");
        return (-1);
    }

    vl_printf("....Complete.\n");
    return (0);
}

typedef struct 
{
    unsigned int enum_bit;    // starts with a base of 64.
    unsigned int gpio_bit;   
    unsigned int gpio_pol;
    unsigned int reset_status;
    void         *enum_hnd;

} VL_BOARD_RESET_BITS;

static VL_BOARD_RESET_BITS lo_vlynq_reset_bits[MAX_VLYNQ_HOPS], hi_vlynq_reset_bits[MAX_VLYNQ_HOPS];
#if 0
/* HPVL : Moved before first use (in teardown func */
static Uint32 lo_reset_enum = 64;
#ifdef AVALANCHE_HIGH_VLYNQ_CONTROL_BASE
static Uint32 hi_reset_enum = 96;
#endif
#endif

extern REMOTE_VLYNQ_DEV_RESET_CTRL_FN p_remote_vlynq_dev_reset_ctrl;

/* This function is board specific and should be ported for each board. */
static void vlynq_remote_dev_reset_ctrl(unsigned int         module_reset_bit,
                                        PAL_SYS_RESET_CTRL_T reset_ctrl)
{
    VL_BOARD_RESET_BITS *p_reset_bits;
    int i = 0;

    if(module_reset_bit > 31)
        p_reset_bits       = &hi_vlynq_reset_bits[0];
    else
        p_reset_bits = &lo_vlynq_reset_bits[0];

    module_reset_bit  += 64;
 
    for(i = 0; i < MAX_VLYNQ_HOPS; i++, p_reset_bits++)
        if(p_reset_bits->enum_bit == module_reset_bit)
            break;
    
    if(i == MAX_VLYNQ_HOPS)
        return;

    if(!p_reset_bits->enum_hnd)     
        return;

    if((OUT_OF_RESET == reset_ctrl) && 
       (OUT_OF_RESET != p_reset_bits->reset_status))
    {
#if 0
        PAL_sysGpioOutBit(p_reset_bits->gpio_bit, !p_reset_bits->gpio_pol);
#else
        PAL_sysResetCtrl((INT32)p_reset_bits->gpio_bit, OUT_OF_RESET);
#endif
        p_reset_bits->reset_status = OUT_OF_RESET;

        vlynq_delay_wait(100000);
        dev_setup(p_reset_bits->enum_hnd);
        vl_printf("Out of reset.\n");
 
        return;
    }

    if((IN_RESET == reset_ctrl) && 
       (IN_RESET != p_reset_bits->reset_status))
    {
        vl_printf("Tearing down the VLYNQ bridge associated with gpio reset(enum) bit %d (%d).\n",
                   p_reset_bits->gpio_bit, p_reset_bits->enum_bit);

        dev_teardown(p_reset_bits->enum_hnd);

        /* HPVL */
#if 0
        PAL_sysGpioOutBit(p_reset_bits->gpio_bit, p_reset_bits->gpio_pol);
#else
        PAL_sysResetCtrl((INT32)p_reset_bits->gpio_bit, IN_RESET);
#endif
        p_reset_bits->reset_status = IN_RESET;
        return;
    }

    return;
}

Int32 avalanche_vlynq_set_hw_reset_info(Uint32 root_base_addr, Uint32 zero_based_hop,
	                                Uint32 gpio_bit,       Bool   gpio_pol)
{
    VL_BOARD_RESET_BITS *p_reset_bits;
    Uint32              *p_reset_enum;

    if(zero_based_hop >= MAX_VLYNQ_HOPS)
        return (-1);

    if(root_base_addr == AVALANCHE_LOW_VLYNQ_CONTROL_BASE)
    {
        p_reset_bits = &lo_vlynq_reset_bits[zero_based_hop];
        p_reset_enum = &lo_reset_enum;
    }
#ifdef AVALANCHE_HIGH_VLYNQ_CONTROL_BASE
    else if (root_base_addr == AVALANCHE_HIGH_VLYNQ_CONTROL_BASE)
    {
        p_reset_bits = &hi_vlynq_reset_bits[zero_based_hop];
        p_reset_enum = &hi_reset_enum;
    }
#endif
    else
        return (-1);

    p_reset_bits->enum_bit     = (*p_reset_enum)++;
    p_reset_bits->gpio_bit     = gpio_bit;
    p_reset_bits->gpio_pol     = gpio_pol;


    /* HPVL */
#if 0
    PAL_sysGpioCtrl(p_reset_bits->gpio_bit, GPIO_PIN, GPIO_OUTPUT_PIN);
    vlynq_delay_wait(50000);

    PAL_sysGpioOutBit(p_reset_bits->gpio_bit,  p_reset_bits->gpio_pol);
    vlynq_delay_wait(50000);

    PAL_sysGpioOutBit(p_reset_bits->gpio_bit, !p_reset_bits->gpio_pol);
    vlynq_delay_wait(50000);
#else
    PAL_sysResetCtrl((INT32)gpio_bit, OUT_OF_RESET);
#endif
    p_reset_bits->reset_status = OUT_OF_RESET;
    p_reset_bits->enum_hnd     = NULL;

    if(!p_remote_vlynq_dev_reset_ctrl) 
        p_remote_vlynq_dev_reset_ctrl = vlynq_remote_dev_reset_ctrl;
    
    return (0);
}

static Int32 get_reset_bit(Uint32 root_base_addr, Uint32 hop, 
		           void* handle, Uint32 *reset_bit)
{
    VL_BOARD_RESET_BITS *p_reset_bits;

    if(root_base_addr == AVALANCHE_LOW_VLYNQ_CONTROL_BASE)
        p_reset_bits = &lo_vlynq_reset_bits[0];
#ifdef AVALANCHE_HIGH_VLYNQ_CONTROL_BASE
    else if (root_base_addr == AVALANCHE_HIGH_VLYNQ_CONTROL_BASE)
	p_reset_bits = &hi_vlynq_reset_bits[0];
#endif
    else
        return (-1);

    p_reset_bits[hop].enum_hnd = handle;
    *reset_bit                 = p_reset_bits[hop].enum_bit;

    return (0);
}
