/*
 *
 * pals_sysMisc.c
 * Description:
 *
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

/** \file   pal_sysMisc.c
    \brief  PAL miscellaneous code file


    \author     Mansoor (mansoor.ahamed@ti.com)
    \version    0.1
 */
#include <asm-arm/arch-avalanche/generic/pal.h>

unsigned int PAL_sysGetChipVersionInfo(void)
{
    return (*(volatile unsigned int*)AVALANCHE_CVR);
}

/*******************************************************************************
 * FUNCTION:    avalanche_get_cpu_type
 *******************************************************************************
 * DESCRIPTION: Function to get the cpu type
 *
 * RETURNS:     cpu type 
 *
 ******************************************************************************/
AVALANCHE_CPU_TYPE_T avalanche_get_cpu_type(void)
{
    volatile UINT32 cvr;
    volatile UINT32 cvr2;
    AVALANCHE_CPU_TYPE_T cpu_type;
   
    cpu_type =  CPU_UNIDENT;
   
    /* Normal read */
    cvr =  PAL_sysGetChipVersionInfo() & 0xffff;
   
    /* 
     * Avalanche I errata; Try writing to the CVR, it changes on the Avalanche I
     * though it should not 
     */
    REG32_WRITE(AVALANCHE_CVR, 0xffff);
   
    cvr2 = (REG32_DATA(AVALANCHE_CVR) & 0xffff);
   
    if(cvr == cvr2)
    {
        switch(cvr)
        {
            case CPU_PUMA5:
                cpu_type = cvr;
            break;
         
            /* TODO: More CPUs here */
         
            default:
                cpu_type = CPU_UNIDENT;
        }
    }
    else
    {
        /* CVR can be written, this is the Avalanche I */
        cpu_type = CPU_AVALANCHE_I;
    }
   
    return cpu_type;    
}

/*******************************************************************************
 * FUNCTION:    avalanche_get_cpu_name
 *******************************************************************************
 * DESCRIPTION: Function to get the cpu name
 *
 * RETURNS:     cpu name
 *
 ******************************************************************************/
const char* avalanche_get_cpu_name(AVALANCHE_CPU_TYPE_T cpu_type)
{
    char* name = NULL;
    static char* cpu_name[]=
    {
       "Unknown"    ,   /* 0 */ 
       "PUMA5"     ,    /* 1 */
       
       /* TODO: Add more CPUs here */
       0
    };
    
    switch(cpu_type)
    {
        case CPU_PUMA5: 
            name = cpu_name[1];  
        break;
         
        case CPU_UNIDENT:
        default:        
            name = cpu_name[0];
    }
    
    return name;
}

/* Avalanche MDIX specific functionality */
SET_MDIX_ON_CHIP_FN_T p_set_mdix_on_chip_fn = NULL;

int avalanche_set_mdix_on_chip(unsigned int base_addr, unsigned int operation)
{
    if(p_set_mdix_on_chip_fn)
        return (p_set_mdix_on_chip_fn(base_addr, operation));
    else
        return (-1);
}

unsigned int avalanche_is_mdix_on_chip(void)
{
    return (p_set_mdix_on_chip_fn ? 1:0);
}
#if defined (CONFIG_MACH_PUMA5)
PAL_Result PAL_sysProbeAndPrep (Uint32 version, Uint32 base_addr, void *param)
{
        Int32 i = 0;
        BOARD_ID board_variant = avalanche_get_board_variant();
        MOD_INFO_T *p = &(soc[board_variant].modules[0]);

        for(i = 0; i < MAX_MODULES; i++, p++)
        {
            if ((version == p->version) && (base_addr == p->base_addr))
            {
                version = p->version;
                avalanche_device_prepare(version, base_addr, board_variant, param);
                return 0;
            }
        }

        return (-1);
}
#endif
