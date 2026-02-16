/*
 *
 * puma5_emu_clk_cntl.c 
 * Description:
 * puma5 emulation platform (mostly volcano) clock (set/get) APIs
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

#include <asm/arch/update_tag.h>
#include <asm/arch/generic/pal.h>

#define PUMA5_CLOCK_UNKNOWN	1
extern unsigned int cpu_freq;
int arm1176_puma5_cpu_clock=PUMA5_CLOCK_UNKNOWN;
int arm1176_puma5_sys_clock=PUMA5_CLOCK_UNKNOWN;
int arm1176_puma5_vbus_clock=PUMA5_CLOCK_UNKNOWN;

/****************************************************************************
 * FUNCTION: PAL_sysClkcInit
 ****************************************************************************
 * Description: The routine initializes the internal variables depending on
 *              on the sources selected for different clocks.
 ***************************************************************************/
void PAL_sysClkcInit (void *param)
{
    /* No init on emulation platform */        
	cpu_freq = PAL_sysClkcGetFreq(CLKC_ARM); 
}

/**
 * \brief PAL System Set Clock Frequency
 * \note this function may not be able to set the frequency requested if it
 * finds that the hardware does not allow it and/or the output desired is 
 * unfeasible. In all cases, the function just returns the clock frequency 
 * before reprogramming. It is the responsibility of the caller to call its 
 * 'get' counterpart after the 'set' to determine if the change actually did 
 * take place. 
 * \note This function waits for the PLLs to lock in a tight loop. Think twice
 * before calling this function in an interrupt context (for example)
 */
int PAL_sysClkcSetFreq (PAL_SYS_CLKC_ID_T clk_id, unsigned int output_freq)
{
    /* Cant set frequencies on emulation platform */        
    return -1;        
}

int PAL_sysClkcGetFreq (PAL_SYS_CLKC_ID_T clk_id)
{
    unsigned int freq = 0;

    switch(clk_id) {
        case CLKC_ARM:
             freq = readl(AVALANCHE_ARM_CLKC_BASE);
             if(freq <= PUMA5_CLOCK_UNKNOWN) {
                printk("Can't read cpufrequency environment from bootloader. defaulting to %u Hz\n", 
							AVALANCHE_ARM_FREQ_DEFAULT);
                freq = AVALANCHE_ARM_FREQ_DEFAULT;
            } 
    	    break;
        case CLKC_SYS:
             freq = readl(AVALANCHE_SYS_CLKC_BASE);
             if(freq <= PUMA5_CLOCK_UNKNOWN) {
                printk("Can't read sbusfrequency environment from bootloader. defaulting to %u Hz\n", 
							AVALANCHE_SBUS_FREQ_DEFAULT);
                freq = AVALANCHE_SBUS_FREQ_DEFAULT;
            }
        	break;
        case CLKC_VBUS:
             freq =  readl(AVALANCHE_VBUS_CLKC_BASE);
             if(freq <= PUMA5_CLOCK_UNKNOWN) {
                printk("Can't read pbusfrequency environment from bootloader. defaulting to %u Hz\n", 
							AVALANCHE_PBUS_FREQ_DEFAULT);
                freq = AVALANCHE_PBUS_FREQ_DEFAULT;
            }
			break;
        default:
            freq = 0;
    }
    return freq;
}
