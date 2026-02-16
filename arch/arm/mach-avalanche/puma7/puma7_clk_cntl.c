/*
 *
 * puma7_clk_cntl.c
 * Description:
 * see below
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2015 Intel Corporation. All rights reserved.
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


/** \file   puma7_clk_cntl.c
 *  \brief  Puma7 clock controller PAL API implementation
 *
 *
 *  \version
 *  (09-Aug-2011)   0.1     Amihay Tabul      For FPGA and Puma6
 *  (Feb-2013)      0.2     Ben Saar          For Puma6/7
 *  
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
/* Puma7 INPUT clock */
#define PUMA7_FPLL_INPUT_CLK_VAL    (MHZ(480.0))
#define PUMA7_TDM_INPUT_CLK_VAL     (MHZ(2.048))

#ifndef CONFIG_MACH_PUMA7_FPGA
#define PUMA7_1X_CLK_VAL            (PUMA7_FPLL_INPUT_CLK_VAL / 4.0)    
#define PUMA7_2X_CLK_VAL            (PUMA7_FPLL_INPUT_CLK_VAL / 2.0)    
#define PUMA7_4X_CLK_VAL            (PUMA7_FPLL_INPUT_CLK_VAL)    
#else /* clock for the Puma7 FPGA board */
#define PUMA7_1X_CLK_VAL            (CONFIG_MACH_PUMA7_FPGA_X1_CLK_FREQ_HZ)
#define PUMA7_2X_CLK_VAL            (PUMA7_1X_CLK_VAL * 2)
#define PUMA7_4X_CLK_VAL            (PUMA7_1X_CLK_VAL * 4)
#endif

/* All freq are in MHz */
#define PUMA7_ARM_CLK_VAL         (PUMA7_4X_CLK_VAL)
#define PUMA7_C55_CLK_VAL         (PUMA7_4X_CLK_VAL)
#define PUMA7_BBU_CLK_VAL         (PUMA7_1X_CLK_VAL)
#define PUMA7_WDT_CLK_VAL         (PUMA7_1X_CLK_VAL)
#define PUMA7_RAM_CLK             (PUMA7_1X_CLK_VAL)
#define PUMA7_TIMER0_CLK_VAL      (PUMA7_1X_CLK_VAL)
#define PUMA7_TIMER1_CLK_VAL      (PUMA7_1X_CLK_VAL)
#define PUMA7_TIMER2_CLK_VAL      (PUMA7_1X_CLK_VAL)
#define PUMA7_UART0_CLK_VAL       (PUMA7_1X_CLK_VAL)
#define PUMA7_UART1_CLK_VAL       (PUMA7_1X_CLK_VAL)
#define PUMA7_UART2_CLK_VAL       (PUMA7_1X_CLK_VAL)
#define PUMA7_BOOT_CFG_CLK_VAL    (PUMA7_1X_CLK_VAL)
#define PUMA7_TDM_CLK_VAL         (PUMA7_TDM_INPUT_CLK_VAL) 
#define PUMA7_CODEC_SPI_CLK_VAL   (PUMA7_1X_CLK_VAL) 
#define PUMA7_TDM10_CLK_VAL       (PUMA7_1X_CLK_VAL) 
#define PUMA7_TDM11_CLK_VAL       (PUMA7_1X_CLK_VAL) 
#define PUMA7_I2C_CLK_VAL         (PUMA7_1X_CLK_VAL)
#define PUMA7_PERF_MON_CLK_VAL    (PUMA7_1X_CLK_VAL)    
#define PUMA7_C55_2_CLK_VAL       (PUMA7_4X_CLK_VAL)
#define PUMA7_INTC_CLK_VAL        (PUMA7_2X_CLK_VAL)
#define PUMA7_SSX_CLK_VAL         (PUMA7_2X_CLK_VAL)

/* Array holds the default freq (in Mhz) for all the different modules */
#define PUMA7_INIT_CLK_DB_ENTRY(clk_value, clk_index) clk_value,
static INT32 puma7_modules_freq[PAL_SYS_CLKC_MAX_ID]  = { PUMA7_INIT_CLK_DB_ENTRIES };
#undef PUMA7_INIT_CLK_DB_ENTRY


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
    DEBUG_PAL_PLL("%s : Puma7 Clock Controller Successfully Initialized\n", __func__);
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
	return puma7_modules_freq[clk_id];
}


/** \func PAL_sysClkcSetFreq
 * \brief PAL System Set Clock Frequency
 * \note The current Puma7 clk control do not modify clock 
 * frequency. 
 * So it is only stub for now. 
 */
INT32 PAL_sysClkcSetFreq (PAL_SYS_CLKC_ID_T clk_id, UINT32 output_freq)
{
    /* sanity check */
	IS_CLK_ID_VALID(clk_id);

    /* Never touch hardware for now */
	return puma7_modules_freq[clk_id];
}

/*********************************************************************
 * 
 * SECTION : Exported PAL_SYS Clock APIs 
 * <END>
 *
 * *******************************************************************/

