/*
 *
 *  Copyright (C) ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General PGublic License as published by
 * the Free Software Foundation; either version 2 of the License , or
 * ( at your option )any later version.
 *
 * This program is distributed in the hope that it will be useful , 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not , write to the Free Software
 * Foundation , Inc. , 59 Temple Place , Suite 330 , Boston , MA  02111-1307 USA
 * AUTHOR :   <xx.yyy@st.com> */

#ifndef __GCFG_H__
#define __GCFG_H__
#include <asm/arch/hcl_defs.h>

typedef struct gdcf_struct
{
    t_uint32 __fill0;		  /* Base + 0 */
    t_uint32 __fill1;		  /* Base + 4 */
    t_uint32 __fill2;		  /* Base + 8 */
    t_uint32 pll_status_register; /* Base + C */
    t_uint32 __fill3;		  /* Base + 10 */
    t_uint32 __fill4;		  /* Base + 14 */
    t_uint32 __fill5;		  /* Base + 18 */
    t_uint32 __fill6;		  /* Base + 1c */
    t_uint32 __fill7;		  /* Base + 20 */
    t_uint32 __fill8;		  /* Base + 24 */
    t_uint32 __fill9;		  /* Base + 28 */

    /*
     * eASIC_PROTECTION_PORT_REGISTER --
     * bit 0 : Enable eASIC Slave Port 1
     * bit 1 : Enable eASIC Slave Port 2
     * bit 2 : Enable eASIC Master Lite Port
     * bit 3 : Enable eASIC Master PL175 Port
     * bit 4 : Enable eASIC Master Full Port
     * bit 5 : Enable eASIC Full Slave Port
     * bit 6 : Enable eASIC Interrupts Port
     * bit 7 : Enable DTCM Port 
     * bit 8 : Enable Coprocessor Port
     * bit 9 : Enable GPIO Port 
     */

    t_uint32 easic_protection_port_register;	/* Base + 2c */

    /*
     * bit   7-0: FREQ. REFRESH DIVIDER FOR SDRAM (Default: 61)
     * bit  15-8: M VALUE FOR PLL91  (Default:  1) }
     * bit 23-16: N VALUE FOR PLL91  (Default: 16)  } Default Multiplication x16
     * bit 26-24: P VALUE FOR PLL91  (Default:  1) }
     * bit    31: CHANGE FREQUENCY BIT FOR PLL91
     */
    t_uint32 reset_clock_control_register;	        /* Base + 30 */

    /* PRESCALER */                                                      
    t_uint32 prescaler_register_clock_pi_register;	/* Base + 34 */
    t_uint32 prescaler_register_clock_48_register;	/* Base + 38 */
    t_uint32 __fill10;		                        /* Base + 3c */

    /* ENABLE REGISTER */                                                           t_uint32 clock_enable_register;	/* Base + 40 */

    /* DISABLE REGISTER */                                                
    t_uint32 clock_disable_register;	/* Base + 44 */
    t_uint32 __fill11;		        /* Base + 48 */
    t_uint32 __fill12;		        /* Base + 4c */
    t_uint32 sdram_output_clock_delay;	/* Base + 50 */
    t_uint32 sdram_input_clock_delay;	/* Base + 54 */
    t_uint32 sdram_fb_clock_source;	/* Base + 58 */
} gcfg_struct_t;

/* Define Gestione Clock */
#define GCFG_ENABLE_CLK_PI		0x00000001U
#define GCFG_ENABLE_HCLK_PI		0x00000002U
#define GCFG_ENABLE_CLK_48		0x00000004U
#define GCFG_DISABLE_CLK_PI		0xFFFFFFFEU
#define GCFG_DISABLE_HCLK_PI		0xFFFFFFFDU
#define GCFG_DISABLE_CLK_48		0xFFFFFFFBU
#define GCFG_PLL_STATUS_BIT		0x00000080U
#define GCFG_PLL_VALUE_M_MASK		0x0000FF00U
#define GCFG_PLL_VALUE_N_MASK		0x00FF0000U
#define GCFG_PLL_VALUE_P_MASK		0x07000000U
#define GCFG_PLL_UPD_FRQ_BIT		0x80000000U
#define GCFG_PLL_VALUE_M_SHIFT		8
#define GCFG_PLL_VALUE_N_SHIFT		16
#define GCFG_PLL_VALUE_P_SHIFT		24

/* Defines For eASIC Protection Register */
#define GCFG_ENABLE_eASIC_SLAVE_PORT_1      0x00000001U
#define GCFG_ENABLE_eASIC_SLAVE_PORT_2      0x00000002U
#define GCFG_ENABLE_eASIC_MASTER_LITE_PORT  0x00000004U
#define GCFG_ENABLE_eASIC_MASTER_PL175_PORT 0x00000008U
#define GCFG_ENABLE_eASIC_MASTER_FULL_PORT  0x00000010U
#define GCFG_ENABLE_eASIC_SLAVE_FULL_PORT   0x00000020U
#define GCFG_ENABLE_eASIC_INTERRUPT_PORT    0x00000040U
#define GCFG_ENABLE_eASIC_DCTM_PORT         0x00000080U
#define GCFG_ENABLE_COPROC_PORT             0x00000100U
#define GCFG_ENABLE_eASIC_GPIOs             0x00000200U
#define GCFG_ENABLE_DMA_PORT                0x00000400U

#endif

/* End of file - gcfg.h */

