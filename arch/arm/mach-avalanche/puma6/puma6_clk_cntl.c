/*
 *
 * puma6_clk_cntl.c
 * Description:
 * see below
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2011-2015 Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */


/** \file   puma6_clk_cntl.c
 *  \brief  Puma6 clock controller PAL API implementation
 *
 *
 *  \version
 *  (09-Aug-2011)   0.1     Amihay Tabul      For FPGA and Puma6
 */

#include <pal.h>

/*********************************************************************************************************
 * HELPER MACROS
 *********************************************************************************************************/
#define MEGA                    (1000000)
#define MHZ(val)                ( (val) * MEGA )

/****************************************************************************
 * DATA PURPOSE:    PRIVATE Variables
 **************************************************************************/
/* Puma6 INPUT clock */
#define PUMA6_FPLL_INPUT_CLK_VAL    (MHZ(450.0))
#define PUMA6_TDM_INPUT_CLK_VAL     (MHZ(2.048))

#ifndef CONFIG_MACH_PUMA6_FPGA
#define PUMA6_1X_CLK_VAL            (PUMA6_FPLL_INPUT_CLK_VAL / 4.0)    
#define PUMA6_2X_CLK_VAL            (PUMA6_FPLL_INPUT_CLK_VAL / 2.0)    
#define PUMA6_4X_CLK_VAL            (PUMA6_FPLL_INPUT_CLK_VAL)    
#else /* clock for the FPGA board */
#define PUMA6_1X_CLK_VAL            (MHZ(10))    
#define PUMA6_2X_CLK_VAL            (MHZ(20))
#define PUMA6_4X_CLK_VAL            (MHZ(40))
#endif

/* All freq are in MHz */
#define PUMA6_ARM_CLK_VAL         (PUMA6_4X_CLK_VAL)
#define PUMA6_C55_CLK_VAL         (PUMA6_4X_CLK_VAL)
#define PUMA6_BBU_CLK_VAL         (PUMA6_1X_CLK_VAL)
#define PUMA6_WDT_CLK_VAL         (PUMA6_1X_CLK_VAL)
#define PUMA6_RAM_CLK             (PUMA6_1X_CLK_VAL)
#define PUMA6_TIMER0_CLK_VAL      (PUMA6_1X_CLK_VAL)
#define PUMA6_TIMER1_CLK_VAL      (PUMA6_1X_CLK_VAL)
#define PUMA6_TIMER2_CLK_VAL      (PUMA6_1X_CLK_VAL)
#define PUMA6_UART0_CLK_VAL       (PUMA6_1X_CLK_VAL)
#define PUMA6_UART1_CLK_VAL       (PUMA6_1X_CLK_VAL)
#define PUMA6_UART2_CLK_VAL       (PUMA6_1X_CLK_VAL)
#define PUMA6_BOOT_CFG_CLK_VAL    (PUMA6_1X_CLK_VAL)
#define PUMA6_TDM_CLK_VAL         (PUMA6_TDM_INPUT_CLK_VAL) 
#define PUMA6_CODEC_SPI_CLK_VAL   (PUMA6_1X_CLK_VAL) 
#define PUMA6_TDM10_CLK_VAL       (PUMA6_1X_CLK_VAL) 
#define PUMA6_TDM11_CLK_VAL       (PUMA6_1X_CLK_VAL) 
#define PUMA6_I2C_CLK_VAL         (PUMA6_1X_CLK_VAL)
#define PUMA6_PERF_MON_CLK_VAL    (PUMA6_1X_CLK_VAL)    
#define PUMA6_C55_2_CLK_VAL       (PUMA6_4X_CLK_VAL)
#define PUMA6_INTC_CLK_VAL        (PUMA6_2X_CLK_VAL)
#define PUMA6_SSX_CLK_VAL         (PUMA6_2X_CLK_VAL)

/* Array holds the default freq (in Mhz) for all the different modules */
#define PUMA6_INIT_CLK_DB_ENTRY(clk_value, clk_index) clk_value,
static INT32 puma6_modules_freq[PAL_SYS_CLKC_MAX_ID]  = { PUMA6_INIT_CLK_DB_ENTRIES };
#undef PUMA6_INIT_CLK_DB_ENTRY

/******************************************************************************
 * HELPER FUNCTIONS/MACROS
 *****************************************************************************/

/* FIXME : uncomment line below to enable debug prints */
//#define DEBUG_PAL_PLL(fmt,arg...)  printk(KERN_EMERG fmt , ##arg);

#ifndef DEBUG_PAL_PLL
#define DEBUG_PAL_PLL(fmt,arg...)
#endif

/* generic get divider macro */
#define IS_CLK_ID_VALID(clk_id) \
	do { \
        if( (clk_id) < PAL_SYS_CLKC_ARM || (clk_id) >= PAL_SYS_CLKC_MAX_ID ) { \
            printk(KERN_ERR "Invalid argument clock ID (%d) passed to function %s \n",clk_id,__func__); \
            BUG(); \
            return (-1); \
        } \
	}while(0)


/*********************************************************************
 * 
 * SECTION : Exported PAL_SYS Clock APIs 
 * <START>
 *
 * *******************************************************************/

/** \func PAL_sysClkcInit
	\brief The routine initializes the internal variables depending on
	on the sources selected for different clocks.
*/
void PAL_sysClkcInit (void *param)
{
    DEBUG_PAL_PLL("%s : Puma6 Clock Controller Successfully Initialized\n", __func__);
    return;
}


/** \func PAL_sysClkcGetFreq
	\brief PAL API to read clock frequency of the specified module 
	\param clk_id	Module whose clock has to be read
	\return returns the clock frequency of the specified module,
			returns -1 on error.
*/
INT32 PAL_sysClkcGetFreq (PAL_SYS_CLKC_ID_T clk_id)
{
	/* sanity check */
	IS_CLK_ID_VALID(clk_id);

	/* Never touch hardware */
	return puma6_modules_freq[clk_id];
}


/** \func PAL_sysClkcSetFreq
 * \brief PAL System Set Clock Frequency
 * \note The current Puma6 clk control do not modify clock 
 * frequency. 
 * So it is only stub for now. 
 */
INT32 PAL_sysClkcSetFreq (PAL_SYS_CLKC_ID_T clk_id, UINT32 output_freq)
{
    /* sanity check */
	IS_CLK_ID_VALID(clk_id);

    /* Never touch hardware for now */
	return puma6_modules_freq[clk_id];
}

/*********************************************************************
 * 
 * SECTION : Exported PAL_SYS Clock APIs 
 * <END>
 *
 * *******************************************************************/

