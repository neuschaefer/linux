/*
 *
 * puma5_clk_cntl.h 
 * Description:
 * see below
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


#ifndef _PUMA5_CLK_CNTL_H_
#define _PUMA5_CLK_CNTL_H_


/** \struct PAL_SYS_ARM_PLL_STRUCT_T
    \brief This type defines the hardware overlay of ARM PLL 
			and control registers with respect to each clock.
*/
typedef struct PAL_SYS_ARM_PLL_STRUCT_tag
{
	UINT32 pid;				/**< peripheral identification register */
	UINT32 reserved1[55];	/**< reserved from 0x004 to 0x0df */
	UINT32 fuserr;			/**< FuseFarm error register */
	UINT32 rstype;			/**< Reset type status register */
	UINT32 reserved2[6];	/**< reserved from 0x0ef to 0x0ff */
	UINT32 pllctl;			/**< PLL controol register */
	UINT32 ocsel;			/**< OBSCLK select register */
	UINT32 secctl;			/**< PLL secondary control register */
	UINT32 reserved3;		/**< reserved from 0x10c to 0x10f */
	UINT32 pllm;			/**< PLL multiplier control register */
	UINT32 prediv;			/**< PLL pre-divider control register */
	UINT32 plldiv_group1[3];/**< PLL controller divider regiter - from sysclk1 to sysclk3 */	
	UINT32 oscdiv1;			/**< Oscillator divider register */
	UINT32 postdiv;			/**< PLL pos-divider control register */
	UINT32 bpdiv;			/**< bypass diver register */
	UINT32 wakeup;			/**< wakeup register */
	UINT32 reserved4;		/**< reserved for wakeup expansion - from 0x134 to 0x13f */
	UINT32 pllcmd;			/**< PLL controller command register */
	UINT32 pllstat;			/**< PLL controller status register */
	UINT32 alnctl;			/**< PLL controller clock align register */
	UINT32 dchange;			/**< PLL controller ratio change status register */
	UINT32 cken;			/**< clock enable control register */
	UINT32 ckstat;			/**< clock status register */
	UINT32 systat;			/**< SYSCLK status register */
	UINT32 ckctl;			/**< Miscellaneous clock control register */
	UINT32 reserved5[2];	/**< reserved for future control - from 0x158 to 0x15f */
	UINT32 plldiv_group2[13];/**< PLL controller divider regiter - from sysclk4 to sysclk16 */ 
} PAL_SYS_ARM_PLL_STRUCT_T;

/** \enum PAL_SYS_CLKC_ID_T
	\brief Enum used to identify the module whose frequency has to be set/get

*/
typedef enum PAL_SYS_CLKC_ID_tag 
{
	PAL_SYS_CLKC_ARM,
	PAL_SYS_CLKC_SYSCLK1,
	PAL_SYS_CLKC_SYSCLK2,
	PAL_SYS_CLKC_SYSCLK3,
	PAL_SYS_CLKC_SYSCLK4,
	PAL_SYS_CLKC_SYSCLK5,
	PAL_SYS_CLKC_SYSCLK6,
	PAL_SYS_CLKC_SYSCLK7,
	PAL_SYS_CLKC_SYSCLK8,
	PAL_SYS_CLKC_SYSCLK9,
	PAL_SYS_CLKC_SYSCLK10,
	PAL_SYS_CLKC_SYSCLK11,
	PAL_SYS_CLKC_SYSCLK12,
	PAL_SYS_CLKC_SYSCLK13,
	PAL_SYS_CLKC_SYSCLK14,
	PAL_SYS_CLKC_SYSCLK15,
	PAL_SYS_CLKC_SYSCLKBP,
	PAL_SYS_CLKC_AUXCLK,
	PAL_SYS_CLKC_OBSCLK,
	PAL_SYS_CLKC_GMII,
	PAL_SYS_CLKC_USB,
	PAL_SYS_CLKC_VLYNQ,
	PAL_SYS_CLKC_TDM,
	PAL_SYS_CLKC_DOCSIS,	
	PAL_SYS_CLKC_MAX_ID	/* this is not a real clock. Used to find clock id end */
}PAL_SYS_CLKC_ID_T;


/* for backward compatibility */
#define CLKC_ARM		  PAL_SYS_CLKC_ARM
#define CLKC_VBUS 		  PAL_SYS_CLKC_SYSCLK3
#define CLKC_VLYNQ		  PAL_SYS_CLKC_VLYNQ
#define CLKC_SYS 		  PAL_SYS_CLKC_SYSCLK2
#define PAL_SYS_CLKC_WDT  CLKC_VBUS
 
/** \struct PAL_SYS_AUX_PLL_STRUCT_T
	\brief This type defines the hardware overlay of GMII PLL.
*/
typedef struct PAL_SYS_AUX_PLL_STRUCT_tag
{
	UINT32 pllmult;	/**< pll multiplier register */
	UINT32 plldiv;		/**< pll divider register */
	UINT32 pllctrl;	/**< pll controll register */
}PAL_SYS_AUX_PLL_STRUCT_T;


/** \enum PAL_SYS_VLYNQ_SOURCE_T
	\brief Source clock selection for vlynq
*/
typedef enum PAL_SYS_VLYNQ_SOURCE_tag
{
	PAL_SYS_CLKC_VLYNQ_SRC_GMII = 0,
	PAL_SYS_CLKC_VLYNQ_SRC_SYSCLK
}PAL_SYS_VLYNQ_SOURCE_T;


/** \struct PAL_SYS_Puma5Init
	\brief	This structure the initialization data for Puma5 clock controller
*/
typedef struct PAL_SYS_Puma5_tag
{
	INT32 refclk_inp;						 /**< Reference Input clock, could be from
												  the oscillator (25Mhz) */
	PAL_SYS_VLYNQ_SOURCE_T vlynq_source; 	/**< Vlyq could either source SYSCLK1 or GMII PLL o/p 
							  PAL_SYS_CLKC_VLYNQ_SRC_GMII,
							  PAL_SYS_CLKC_VLYNQ_SRC_SYSCLK */
} PAL_SYS_Puma5Init;


typedef INT32 (*GET_CALLS)(PAL_SYS_CLKC_ID_T);
typedef INT32 (*SET_CALLS)(PAL_SYS_CLKC_ID_T, UINT32);

/** \struct PAL_SYS_CLKC_HANDLE_T
    \brief Placeholder for set_freq/get_freq/min_freq/max_freq interfaces
   			for each module	
*/
typedef struct PAL_SYS_CLKC_HANDLE_tag
{
	GET_CALLS get_freq; /**< callback to get frequency */
	SET_CALLS set_freq; /**< callback to set frequency */
	INT32 pll_id;		/**< reall pll_id for the virutal clk_id */
/* FIXME : no need for range validation */
#if 0	
	UINT32 min_freq;	/**< supported minimum frequency */
	UINT32 max_freq;	/**< supported maximum frequency */
#endif
}PAL_SYS_CLKC_HANDLE_T;


#define PAL_SYS_CLKC_DIV_ENABLE 		(0x8000)
#define PAL_SYS_CLKC_TDM_DEF			(2048*1000)
#define PAL_SYS_CLKC_MAX_DIV			(32)
#define PAL_SYS_CLKC_MAX_MUL			(32)
#define PAL_SYS_CLKC_GO_BIT				(0x1)
#define PAL_SYS_CLKC_AUXCLK_ENABLE 		(0x1)
#define PAL_SYS_CLKC_OBSCLK_ENABLE 		(0x2)

#define PAL_SYS_CLKC_MULT_MASK			(0x1F) /* multiplier uses only bits 4:0 */
#define PAL_SYS_CLKC_DIV_MASK			(0x1F) /* divider uses only bits 4:0 */


#define PAL_SYS_CLKC_AUXPLL_DOWN		(0x0) /* reset deasserted, bypassed, 
												 powered up, disabled */
#define PAL_SYS_CLKC_AUXPLL_UP			(0xC) /* reset deeasserted, not bypassed, 
												 powered up, enabled */
#define PAL_SYS_USBPLL_DISABLE			(1<<0)
#define PAL_SYS_USBPLL_POWERDOWN		(1<<1)
#define PAL_SYS_USBPLL_BYPASS_N			(1<<2)
#define PAL_SYS_USBPLL_RESET_N			(1<<3)
#define PAL_SYS_USBPLL_DIV_DISABLE		(1<<4)
#define PAL_SYS_USBPLL_OUTPUT_CLK_SEL		(0x1<<5)



/** \struct PAL_SYS_CLKC_SET_CMD_T
 	\brief Command structure passed to low level set freuency API 
*/
typedef struct PAL_SYS_CLKC_SET_CMD_tag
{
	PAL_SYS_CLKC_ID_T pll_id;
	UINT32 output_freq;
	UINT32 base_freq;
	INT32 max_div;
	volatile UINT32 *pll_div_ptr;
	UINT32 div_mask;
	volatile UINT32 *pll_enb_ptr;
	UINT32 enable_mask;
}PAL_SYS_CLKC_SET_CMD_T;


#endif
