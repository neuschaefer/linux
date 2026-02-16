/*
 *
 * puma5_clk_cntl.c
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
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/

/** \file   puma5_clk_cntl.c
 *  \brief  Puma5 clock controller PAL API implementation
 *
 *  \author     PSP, TI
 *
 *  \note       Set tabstop to 4 (:se ts=4) while viewing this file in an
 *              editor. The diagram given below uses more than 80 column, 
 *              so you need an editor which can display more than 80 columns
 *              in a single line otherwise ignore the diagram and use (multiple) specs
 *              for reference
 *
 *  \version
 *                  0.1     Sekhar Nori        For Yamuna
 *  (23-Jul-2007)   0.2     Mansoor Ahamed     For Puma5
 */

/********************************************************************************************************
	PUMA5 Clock Architecture block diagram
	--------------------------------------							GMII(125MHz)----->|
																					  |
                                +---------------------------------------------------+ |
                                |                                     /4            | |
CLK_INP --------+               |                                   |----| 100MHz   | v Vlynq
                |               |   ARM                         +---|SYS1|----------+--->
                |               |   PLL                         |   |----|          | 
    |-------|   | |------|      |                               |     /2            |
    | OSC   |--|>-| PRE  |      |                               |   |----| 200MHz   | Sys-Full
    |25MHZ  |    || DIV  |------+----|                          +---|SYS2|----------+--> (session router)
    |-------|    ||------| 25MHz|  |------| 400MHz|------|      |   |----|          |
                 |  /1          |  | MUL  |-------| POST |400MHz|                   |
                 |              |  |------|       | DIV  |------|     /4            |
                 |              |    *16          |------|      |   |----| 100MHz   | Sys-Half (uart,
                 |              |                    /1         +---|SYS3|----------+-->        gpio,
                 |              |                               |   |----|          |           timers,
                 |              |                               |     /1            |			etc..)
                 |              |                               |   |----| 400MHz   | DDR
                 |              |                               +---|SYS4|----------+-->
                 |              |                               |   |----|          |
                 |              |                               |     /1            |
                 |              |                               |   |----| 400MHz   | DOCSIS MAC
                 |              |                               +---|SYS5|----------+-->
                 |              |                                   |----|          |  TDM (2.048 MHz)
                 |              |                                                   |
                 |              +---------------------------------------------------+
                 |                     
                 |                  |------|     125MHz
                 |    25MHz         | GMII |------------------------------------------>GMIITX
                 |------------------| *5   |
 				 |					|------|
 				 |                                     
 				 |                  |-----------|     24MHz   					
                 |    25MHz         | USB  		|------------------------------------->USB2.0
                 |------------------|  *24 /25  |              
                 | 					|-----------|                           
				 |                                                                
				 |                  |-----------|     300MHz   					                       
				 |    25MHz         | DOCSIS	|------------------------------------->DOCSIS PLL
				 +------------------| *24 /2	|                                            
                   					|-----------|                                                     

NOTE: 	Apart from clock control registers some system (soc level global) registers should
		be programmed. This information is available in Puma5 chip level document
		Sysclock0 is not shown in the above diagram, it is used by ARM SS
		Auxiliary, Backup, Observe clocks are not shown in the diagram
***************************************************************************************************************/

#include <asm-arm/arch-avalanche/generic/pal.h>

/*********************************************************************************************************
 * HELPER MACROS
 *********************************************************************************************************/
#define MIN(x,y)               ( ((x) <  (y)) ? (x) : (y) )
#define MAX(x,y)               ( ((x) >  (y)) ? (x) : (y) )
#define ABS(x)                 ( ((signed)(x) > 0) ? (x) : (-(x)) )
#define QUOTIENT(x,y)          ( ((x) + (y) / 2) / (y) )
#define MHZ(val)        (val * 1000000)
#define RETURN_IF_ZERO(x) if(x==0) return 0

/****************************************************************************
 * DATA PURPOSE:    PRIVATE Variables
 **************************************************************************/

static PAL_SYS_Puma5Init clkc_init;					 /**< clock controller initialization data */
static volatile PAL_SYS_ARM_PLL_STRUCT_T *apll_regs; /**< Pointer to ARM PLL register map */
static volatile PAL_SYS_AUX_PLL_STRUCT_T *upll_regs; /**< Pointer to USB PLL register map */
static volatile PAL_SYS_AUX_PLL_STRUCT_T *gpll_regs; /**< Pointer to GMII PLL register map */
static volatile PAL_SYS_AUX_PLL_STRUCT_T *dpll_regs; /**< Pointer to docsis PLL register mapp */
static volatile UINT32 *vlynqcr_reg;				 /**< system level vlynq configuration register */
static volatile UINT32 *tdmcr_reg;					 /**< system level tdma configuration register */



/*  Define this macro if you want to the APIs to return/set default values.
    The APIs will not program the hardware regiater if you define this macro.
    Please check the silicon spec for the default values and update the default
    macros defined below (stubs for backup)
*/
//#define PAL_SYS_CLKC_USE_STUBS 1


/* Default Stubs */
#ifdef PAL_SYS_CLKC_USE_STUBS

#if 0
INT32 freq_stubs[PAL_SYS_CLKC_MAX_ID] = {
    MHZ(300), /* ARM Clock */
    MHZ(300)/4,	/* Vlynq */
    MHZ(300)/2,
    MHZ(300)/4,
    MHZ(300)/1, 
	MHZ(300)/8,  /* SFI */
	MHZ(300)/1, MHZ(300)/1, MHZ(300)/1,
    MHZ(300)/1, MHZ(300)/1, MHZ(300)/1, MHZ(300)/1, MHZ(300)/1,
    MHZ(300)/1, MHZ(300)/1, /* sys clock ends here */
    MHZ(25),    /* PAL_SYS_CLKC_SYSCLKBP */
    MHZ(25),    /* PAL_SYS_CLKC_AUXCLK */
    MHZ(25),    /* PAL_SYS_CLKC_OBSCLK */
    MHZ(125),   /* PAL_SYS_CLKC_GMII, */
    MHZ(24),    /* PAL_SYS_CLKC_USB */
    MHZ(133),   /* PAL_SYS_CLKC_VLYNQ */
    (2048000),    /* PAL_SYS_CLKC_TDM - 2.048 MHz */
    MHZ(300),   /* PAL_SYS_CLKC_DOCSIS */
    };

#else
INT32 freq_stubs[PAL_SYS_CLKC_MAX_ID] = {
    MHZ(400), /* ARM Clock */
    MHZ(400)/4,	/* Vlynq */
    MHZ(400)/2,
    MHZ(400)/4,
    MHZ(400)/1, 
	MHZ(400)/8,  /* SFI */
	MHZ(400)/1, MHZ(400)/1, MHZ(400)/1,
    MHZ(400)/1, MHZ(400)/1, MHZ(400)/1, MHZ(400)/1, MHZ(400)/1,
    MHZ(400)/1, MHZ(400)/1, /* sys clock ends here */
    MHZ(25),    /* PAL_SYS_CLKC_SYSCLKBP */
    MHZ(25),    /* PAL_SYS_CLKC_AUXCLK */
    MHZ(25),    /* PAL_SYS_CLKC_OBSCLK */
    MHZ(125),   /* PAL_SYS_CLKC_GMII, */
    MHZ(24),    /* PAL_SYS_CLKC_USB */
    MHZ(133),   /* PAL_SYS_CLKC_VLYNQ */
    (2048*1000),    /* PAL_SYS_CLKC_TDM - 2.048 MHz */
    MHZ(300),   /* PAL_SYS_CLKC_DOCSIS */
    };
#endif

#endif


/* Array holds the function pointers to GetFreq() function of different modules */
PAL_SYS_CLKC_HANDLE_T puma5_handles[PAL_SYS_CLKC_MAX_ID];

/******************************************************************************
 * HELPER FUNCTIONS/MACROS
 *****************************************************************************/

/* FIXME : uncomment line below to enable debug prints */
//#define DEBUG_PAL_PLL(fmt,arg...)  printk(KERN_EMERG fmt , ##arg);

#ifndef DEBUG_PAL_PLL
#define DEBUG_PAL_PLL(fmt,arg...)
#endif

/* generic get divider macro */
#define GET_DIV(div, reg) \
	do { \
		div = 1; \
		/* update div variable if divider was enabled */ \
		if(reg & PAL_SYS_CLKC_DIV_ENABLE) \
			div = ((reg & PAL_SYS_CLKC_DIV_MASK) + 1); /* divider = div + 1 */ \
	}while(0);

/* returns pre-divider, if disabled returns 0 */
#define GET_PREDIV(pre_div) GET_DIV(pre_div, apll_regs->prediv);
/* returns post-divider, if disabled returns 0 */
#define GET_POSTDIV(post_div) GET_DIV(post_div, apll_regs->postdiv);
/* returns multiplier */
#define GET_MULT(mult) mult = (apll_regs->pllm & 0x3F) + 1; /* bits 5:0 gives the 
														 multiplier */ 
	
/* calculate base frequency */
/* sys clock base_freq = ref_clk / pre_divider * multiplier / post divider */
/* e.g base_freq = (((MHZ(25) / 1) * 16) / 1)) */
/*				 = MHZ(400) */
#define CALC_SYS_BASE_FREQ(ref, pre, mult, post) (((ref / pre) * mult) / post);

/* returns the address of the sysclock divder register based on clock id */
#define GET_SYSCLK_DIV_PTR(clk_id, ptr) \
	do{ \
		if((clk_id >= PAL_SYS_CLKC_ARM) && (clk_id <= PAL_SYS_CLKC_SYSCLK2)) \
		{ \
			ptr = &(apll_regs->plldiv_group1[clk_id]);	\
		} \
		else if((clk_id >= PAL_SYS_CLKC_SYSCLK3) && (clk_id <= PAL_SYS_CLKC_SYSCLK15)) \
		{ \
			ptr = &(apll_regs->plldiv_group2[clk_id - PAL_SYS_CLKC_SYSCLK3]); \
		} \
		else \
			ptr = 0x0; /* if clk id is out of range*/  \
	}while(0);


/* reads and returns sysclock diveder value */
/* returns -1 on error */
#define GET_SYSCLK_DIV(divder, clk_id) \
	do{	\
		volatile UINT32 *d = 0;	\
		divider = 1;	\
		GET_SYSCLK_DIV_PTR(clk_id, d); \
		if(d != 0) \
		{	\
			if(*d & PAL_SYS_CLKC_DIV_ENABLE) \
				divider = (*d & PAL_SYS_CLKC_DIV_MASK) + 1; /* bits 4:0 gives the divider */ \
		}\
	}while(0);
		

/* this mamcro is used by GMII, USB and DOCSIS PLLS */
#define GET_AUX_PLLS_FREQ(freq, ctrl, mult, div) \
do{	\
	freq = clkc_init.refclk_inp; \
	/* if in reset then return 0 */ \
	if(!(ctrl & 0x8))	\
		freq = 0;\
	else if(!(ctrl & 0x4)) /* if bypassed then return ref clock */\
		freq = freq;\
	else if(ctrl & 0x2) /* if powered down return 0 */\
		freq = 0;\
	else if(ctrl & 0x1) /* if disabled then return 0 */\
		freq = 0;\
	else\
	{\
		/* only bits 4:0 are used in div and mul registers */\
		freq = (freq * ((mult & PAL_SYS_CLKC_MULT_MASK) + 1)) / \
						((div & PAL_SYS_CLKC_DIV_MASK) + 1);\
	}	\
}while(0);


/* Returns vlynq source */
static inline PAL_SYS_VLYNQ_SOURCE_T get_vlynq_source(void)
{
	/* FIXME : Register information not available
				check with CP&IP team */
	return PAL_SYS_CLKC_VLYNQ_SRC_SYSCLK;
}

/* sets vlynq source */
static inline void set_vlynq_source(PAL_SYS_VLYNQ_SOURCE_T source)
{
	/* FIXME : Register information not available
				check with CP&IP team */
	return ;
}

/* returns base frequency for sysclocks */
static inline INT32 get_sysclk_base_freq(void)
{
	INT32 pre_div;
	INT32 mult;
	INT32 post_div;

	/* get pre-divider */
	GET_PREDIV(pre_div);

	/* get multiplier */
	GET_MULT(mult);

	/* get post-divider */
	GET_POSTDIV(post_div);

	/* if either pre or post divider was disabled */
	if((pre_div == 0) || (post_div == 0))
		return 0;

	/* return sysclock base frequency (mostly 400MHz) */
	return CALC_SYS_BASE_FREQ(clkc_init.refclk_inp, pre_div, mult, post_div);
}


/*********************************************************************
 *
 * SECTION : CLOCK GET APIS
 * <START>
 *
 * *******************************************************************/

/* read and return sysclock0 ... sysclock15 clock */
static INT32 get_sysclk_freq(PAL_SYS_CLKC_ID_T clk_id)
{
	INT32 divider = 0;
	INT32 freq = 0;
	GET_SYSCLK_DIV(divider, puma5_handles[clk_id].pll_id);
	RETURN_IF_ZERO(divider);
	freq = (get_sysclk_base_freq() / divider);
	return freq;
}

/* read and return sys bypass clock */
static INT32 get_sysbp_freq(PAL_SYS_CLKC_ID_T clk_id)
{
	INT32 divider = 0;
	INT32 freq;
	GET_DIV(divider, apll_regs->bpdiv);
	RETURN_IF_ZERO(divider);
	/* bypass clock sources reference clock input */
	freq = (clkc_init.refclk_inp / divider);
	return freq;
}

/* read and return aux clock */
/* auxclk = refclk */
static INT32 get_auxclk_freq(PAL_SYS_CLKC_ID_T clk_id)
{
    INT32 freq = clkc_init.refclk_inp;
	/* if auxclk not enabled then return 0 */
	if(!(apll_regs->cken & 0x1))
		freq = 0;
	return freq;
}


/* read and return observe clock */
static INT32 get_obs_freq(PAL_SYS_CLKC_ID_T clk_id)
{
    INT32 divider = 0;
    INT32 freq = clkc_init.refclk_inp;
	/* if observe clock and OSC divider are enabled */
	if((apll_regs->cken & 0x2)) /* bit 2 represents OBS enable */
	{
		GET_DIV(divider, apll_regs->oscdiv1);
		RETURN_IF_ZERO(divider);
		freq /= divider;
	}
	else
		freq = 0;
	return freq;
}
/* read and return GMII clock */
static INT32 get_gmii_freq(PAL_SYS_CLKC_ID_T clk_id)
{
	INT32 freq = 0;

	GET_AUX_PLLS_FREQ(freq, gpll_regs->pllctrl, 
						gpll_regs->pllmult, gpll_regs->plldiv);

	return freq;	
}

/* read and return usb clock */
static INT32 get_usb_freq(PAL_SYS_CLKC_ID_T clk_id)
{
	INT32 freq = 0;

	GET_AUX_PLLS_FREQ(freq, upll_regs->pllctrl, 
						upll_regs->pllmult, upll_regs->plldiv);

	return freq;	
}


/* read and return vlynq clock */
/* vlynq can either source from sysclk1 or from GMII */
static INT32 get_vlynq_freq(PAL_SYS_CLKC_ID_T clk_id)
{
	if(get_vlynq_source() == PAL_SYS_CLKC_VLYNQ_SRC_SYSCLK)
		return PAL_sysClkcGetFreq(PAL_SYS_CLKC_SYSCLK1);
	else
		return PAL_sysClkcGetFreq(PAL_SYS_CLKC_GMII);
}


/* read and return TDM clock */
static INT32 get_tdm_freq(PAL_SYS_CLKC_ID_T clk_id)
{
    if(*tdmcr_reg & 0x1) /* use docsis mac NCO */
        return PAL_SYS_CLKC_TDM_DEF; /* FIXME : don't have info on docsis PLL */
    else /* else use sysclock3 NCO */
        return PAL_sysClkcGetFreq(PAL_SYS_CLKC_SYSCLK3);
}

/* read and return docsis clock */
static INT32 get_docsis_freq(PAL_SYS_CLKC_ID_T clk_id)
{
	return MHZ(300); /* FIXME : don't have info on docsis PLL */
}


/*********************************************************************
 *
 * SECTION : CLOCK GET APIS
 * <END>
 *
 * *******************************************************************/


/*********************************************************************
 *
 * SECTION : CLOCK SET APIS
 * <START>
 *
 * *******************************************************************/

/* Macro checks the GO status in PLL status register
 * and returns -1 if busy
 */ 
#define RETURN_ON_BUSY(pll_id) \
	do { \
		/* if PLL is already in transition then return */ \
		if(apll_regs->pllstat & PAL_SYS_CLKC_GO_BIT) \
		{ \
			DEBUG_PAL_PLL("%s : PLL Busy (module=%d)\n", __FUNCTION__, pll_id); \
			return (-1); \
		} \
	}while(0);

/* This macro triggers the GO command */
#define ARM_PLL_GO() \
	do { \
		/* GO command for loading new values  */ \
		apll_regs->pllcmd &= (~PAL_SYS_CLKC_GO_BIT);	/* clear the bit */	\
		apll_regs->pllcmd |= (PAL_SYS_CLKC_GO_BIT); /* GO..... */ \
	}while(0);


/* This macro sets the align bit for the specified module */
#define ARM_PLL_ALIGN(pll_id) \
	do { \
		apll_regs->alnctl &= (~(1 << pll_id));	/* clear the bit */	\
		apll_regs->alnctl |= (1 << pll_id); 	/* set the align bit */ \
	}while(0);


/* wait in a tight loop while PLL is busy (wait for end of transition */
#define WAIT_FOR_PLL_UPDATE() \
	do { \
		DEBUG_PAL_PLL("%s : Syncing...", __FUNCTION__); \
		while(apll_regs->pllstat & PAL_SYS_CLKC_GO_BIT) \
		{ \
			pll_delay(4); \
			DEBUG_PAL_PLL(".");\
		} \
		DEBUG_PAL_PLL("\n"); \
	}while(0);


/* Rough cycles delay routine */
INT32 pll_delay(INT32 cycles)
{
	INT32 dummy=1;
	/* compiler should not optimize so do something.
	 * Multiplication is a best way to introduce delay
	 * divied by 2 is just a random number
	 */
	for(cycles=cycles/2;cycles;cycles--)
		dummy *= cycles;				
	
	return dummy;
}

/* This function validates the requested frequency based on the base 
 * frequency and max divider and returns the divider
 */
static INT32 validate_freq_and_get_divider(PAL_SYS_CLKC_ID_T clk_id, 
								UINT32 output_freq,
								UINT32 base_freq,
								INT32 max_div,
								INT32 *divider)

{
	UINT32 calc_freq;

	*divider = base_freq / output_freq;
	
	/* check if requested freq can be set */	
	if((output_freq > base_freq) || (*divider > max_div)) {
		DEBUG_PAL_PLL("%s : Requested Frequency (%dHz) is out of range for module %d \n",
						__FUNCTION__, output_freq, clk_id);
		return (-1);
	}

	/* closest match - inform user */
	calc_freq = base_freq / *divider;
	if(calc_freq != output_freq)
	{
		DEBUG_PAL_PLL("PAL_sysClock : Closest match (%dHz) found for Requested frequency (%dHz) for module %d\n",
						calc_freq, output_freq, clk_id);
#if 0 /* we are allowed to program closest match */
		return (base_freq / *divider);
#endif
	}

	*divider = *divider - 1; /* divider is always less one */
	return 0;

}



/* Low level set frequency API */
static INT32 __set_freq(PAL_SYS_CLKC_SET_CMD_T cmd)
{
	INT32 no_match;
	INT32 divider;
	
	/* check if PLL is busy doing some transition */
	RETURN_ON_BUSY(cmd.pll_id);

	no_match = validate_freq_and_get_divider(cmd.pll_id, cmd.output_freq, 
					cmd.base_freq, cmd.max_div, &divider);
	/* can't set */
	if(no_match != 0)
		return no_match;

	/* FIXME : spec says bypass is not required for ARM PLL dividers 
	 * (except pre-div)
	 * .......put it in bypass mode ......*/

	/* check enable register availability */
	if(cmd.pll_enb_ptr)
		/* disable divider */
		*(cmd.pll_enb_ptr) &= (~cmd.enable_mask);

	/* check divider register availability */
	if(	cmd.pll_div_ptr) 
	{
		/* update new divider */
		*(cmd.pll_div_ptr) = 	(divider & cmd.div_mask); 
		/* we need a delay for locking */
		pll_delay(200);
	}

	if(cmd.pll_enb_ptr)
		/* enable divider */
   		*(cmd.pll_enb_ptr) |= cmd.enable_mask; 
	
	/* enable Align register before GO */
	ARM_PLL_ALIGN(cmd.pll_id);

	/* Trigger GO cmd: FIXME : Is this required??? */
	ARM_PLL_GO();

	/* wait for GOSTAT to become zero */
	WAIT_FOR_PLL_UPDATE();

	/* we need a delay for locking */
	pll_delay(200);

	return 0;
}




/* set sysclock0 ... sysclock15 clock */
static INT32 set_sysclk_freq(PAL_SYS_CLKC_ID_T clk_id, UINT32 output_freq)
{
	volatile UINT32 *pll_div = 0;
	PAL_SYS_CLKC_SET_CMD_T cmd;

	/* populate set cmd */
	cmd.pll_id 			= puma5_handles[clk_id].pll_id;
	cmd.output_freq 	= output_freq;
	cmd.base_freq 		= get_sysclk_base_freq();
	cmd.max_div			= PAL_SYS_CLKC_MAX_DIV;
	/* get the pointer to the sysclock divider */
	GET_SYSCLK_DIV_PTR(cmd.pll_id, pll_div);
	if(pll_div == 0) {
		DEBUG_PAL_PLL("%s : pll_id out of range (%d)\n", __FUNCTION__, cmd.pll_id);
		return (-1);
	}	
	cmd.pll_div_ptr		= pll_div;
	cmd.div_mask		= PAL_SYS_CLKC_DIV_MASK; /* divisor is from bits 4:0 */
	cmd.pll_enb_ptr		= pll_div; /* for sysclock div and enable are in the same register */
	cmd.enable_mask		= PAL_SYS_CLKC_DIV_ENABLE;

	return __set_freq(cmd);
}

/* set sys bypass clock */
static INT32 set_sysbp_freq(PAL_SYS_CLKC_ID_T clk_id, UINT32 output_freq)
{
	PAL_SYS_CLKC_SET_CMD_T cmd;

	/* populate set cmd */
	cmd.pll_id 			= puma5_handles[clk_id].pll_id;
	cmd.output_freq 	= output_freq;
	cmd.base_freq 		= clkc_init.refclk_inp;
	cmd.max_div			= PAL_SYS_CLKC_MAX_DIV;
	cmd.pll_div_ptr		= &(apll_regs->bpdiv);
	cmd.div_mask		= PAL_SYS_CLKC_DIV_MASK; /* divisor is from bits 4:0 */
	cmd.pll_enb_ptr		= &(apll_regs->bpdiv);
	cmd.enable_mask		= PAL_SYS_CLKC_DIV_ENABLE;

	return __set_freq(cmd);	
}

/* set aux clock */
/* auxclk = refclk */
static INT32 set_auxclk_freq(PAL_SYS_CLKC_ID_T clk_id, UINT32 output_freq)
{
	PAL_SYS_CLKC_SET_CMD_T cmd;

	/* populate set cmd */
	cmd.pll_id 			= puma5_handles[clk_id].pll_id;
	cmd.output_freq 	= output_freq;
	cmd.base_freq 		= clkc_init.refclk_inp;
	cmd.max_div			= 1; /* auxclk = refclk */
	cmd.pll_div_ptr		= NULL;
	cmd.div_mask		= 0; /* no divider */ 
	cmd.pll_enb_ptr		= &(apll_regs->cken);
	cmd.enable_mask		= PAL_SYS_CLKC_AUXCLK_ENABLE;

	return __set_freq(cmd);	
}


/* set observe clock */
static INT32 set_obs_freq(PAL_SYS_CLKC_ID_T clk_id, UINT32 output_freq)
{
	PAL_SYS_CLKC_SET_CMD_T cmd;

	/* populate set cmd */
	cmd.pll_id 			= puma5_handles[clk_id].pll_id;
	cmd.output_freq 	= output_freq;
	cmd.base_freq 		= clkc_init.refclk_inp;
	cmd.max_div			= PAL_SYS_CLKC_MAX_DIV; 
	cmd.pll_div_ptr		= &(apll_regs->oscdiv1);
	cmd.div_mask		= PAL_SYS_CLKC_DIV_MASK; /* divisor is from bits 4:0 */ 
	cmd.pll_enb_ptr		= &(apll_regs->cken);
	cmd.enable_mask		= PAL_SYS_CLKC_OBSCLK_ENABLE;

	return __set_freq(cmd);	
}

/* find gcd of 2 numbers */
static UINT32 find_gcd (UINT32 min, UINT32 max)
{
	if (max % min == 0)
		return min;
	else
		return find_gcd (max % min, min);
}

/* get values of divider and multiplier based on base_freq and output_freq */
static INT32 get_val (UINT32 output_freq, UINT32 base_freq,
     UINT32 *multiplier, UINT32 *divider)
{
	UINT32 temp_mul;
	UINT32 temp_div;
	UINT32 gcd;
	UINT32 min_freq;
	UINT32 max_freq;

	/* find gcd of base_freq, output_freq */
	min_freq = (base_freq < output_freq) ? base_freq : output_freq;
	max_freq = (base_freq > output_freq) ? base_freq : output_freq;
	gcd = find_gcd (min_freq, max_freq);

	if (gcd == 0)
	{
		DEBUG_PAL_PLL("%s : Invalid GCD\n", __FUNCTION__);
		return (-1);         /* ERROR */
	}

	/* compute values of multiplier and divider */
	temp_mul = output_freq / gcd;
	temp_div = base_freq / gcd;

	/* we don't support this */
	if ((temp_mul <= 0) || (temp_mul > PAL_SYS_CLKC_MAX_MUL))
	{
		DEBUG_PAL_PLL("%s : pal_sysClock -> Calculated muliplier (%d) out of range\n",
					__FUNCTION__, temp_mul); 
		return (-1);
	}
	if ((temp_div <= 0) || (temp_div > PAL_SYS_CLKC_MAX_DIV))
	{
		DEBUG_PAL_PLL("%s : pal_sysClock -> Calculated divider (%d) out of range\n",
					__FUNCTION__, temp_div); 
		return (-1);
	}

	*multiplier = temp_mul - 1;
	*divider = temp_div - 1;

	return 0;
}


/* low level set routine for USB and GMII PLLs */
static INT32 __set_aux_pll(char *module, UINT32 output_freq, 
							volatile UINT32 *ctrl, volatile UINT32 *mult_ptr,
							volatile UINT32 *div_ptr)
{
	UINT32 mult 	= 0;
	UINT32 divider 	= 0;

	/* Bypass PLL */
	*ctrl = PAL_SYS_CLKC_AUXPLL_DOWN;

	/* get multiplier and divider */
	if(get_val(output_freq, clkc_init.refclk_inp,
						   &mult, &divider) != 0)
	{
		DEBUG_PAL_PLL("%s : Could not scale reference clock to requested %s freq\n", 
						__FUNCTION__, module);
		return (-1);
	}


	if(  strcmp (module, "USB") == 0 ) 
	{
		mult = mult & PAL_SYS_CLKC_MULT_MASK;
		/* program multiplier */
		*mult_ptr = mult ;

		divider = (divider & PAL_SYS_CLKC_DIV_MASK) | ( PAL_SYS_CLKC_DIV_ENABLE ) ;
		/* program divider */
		*div_ptr = divider ;

		pll_delay(200);

		/* Enable PLL */
		*ctrl = PAL_SYS_USBPLL_OUTPUT_CLK_SEL;
		*ctrl = (PAL_SYS_USBPLL_OUTPUT_CLK_SEL |PAL_SYS_USBPLL_RESET_N);
		*ctrl = (PAL_SYS_CLKC_AUXPLL_UP | PAL_SYS_USBPLL_DIV_DISABLE 
				| PAL_SYS_USBPLL_OUTPUT_CLK_SEL);
		udelay(200);
		*ctrl = (PAL_SYS_CLKC_AUXPLL_UP | PAL_SYS_USBPLL_DIV_DISABLE); 

	}
	else 
	{
		/* program multiplier */
		*mult_ptr = mult & PAL_SYS_CLKC_MULT_MASK;

		/* program divider */
		*div_ptr = divider & PAL_SYS_CLKC_DIV_MASK;

		pll_delay(200);

		/* Enable PLL */
		*ctrl = PAL_SYS_CLKC_AUXPLL_UP;
	}

	return 0;
}



/* set GMII clock */
/* FIXME : Exact sequence for programming GMII PLL is not available */
static INT32 set_gmii_freq(PAL_SYS_CLKC_ID_T clk_id, UINT32 output_freq)
{
	return __set_aux_pll("GMII", output_freq, &(gpll_regs->pllctrl),
				   	&(gpll_regs->pllmult), &(gpll_regs->plldiv));
}

/* set usb clock */
static INT32 set_usb_freq(PAL_SYS_CLKC_ID_T clk_id, UINT32 output_freq)
{
	return __set_aux_pll("USB", output_freq, &(upll_regs->pllctrl),
				   	&(upll_regs->pllmult), &(upll_regs->plldiv));
}


/* set vlynq clock */
/* vlynq can either source from sysclk1 or from GMII */
static INT32 set_vlynq_freq(PAL_SYS_CLKC_ID_T clk_id, UINT32 output_freq)
{
	/* source from GMII if requested is same as GMII frequency */	
	if(output_freq == PAL_sysClkcGetFreq(PAL_SYS_CLKC_GMII))
			set_vlynq_source(PAL_SYS_CLKC_VLYNQ_SRC_GMII);
	else /* vlynq uses sysclock1 */
		return PAL_sysClkcSetFreq(PAL_SYS_CLKC_SYSCLK1, output_freq);

	return 0;
}


/* set TDM clock */
/* FIXME : don't have info on docsis PLL */
static INT32 set_tdm_freq(PAL_SYS_CLKC_ID_T clk_id, UINT32 output_freq)
{
    if(*tdmcr_reg & 0x1) /* use docsis mac NCO */
        return PAL_SYS_CLKC_TDM_DEF; 
    else /* else use sysclock3 NCO */
        return 0;
}

/* set docsis clock */
/* FIXME : don't have info on docsis PLL */
static INT32 set_docsis_freq(PAL_SYS_CLKC_ID_T clk_id, UINT32 output_freq)
{
	return 0; 
}




/*********************************************************************
 *
 * SECTION : CLOCK SET APIS
 * <END>
 *
 * *******************************************************************/


typedef struct freq_table {
   INT32 freq;
   INT32 div;
} FREQ_TABLE_T;

FREQ_TABLE_T armpll_freq_table [] = {
    {400000000,     0 /* Populated during init */},
    {200000000,     0},
    {-1,            0}
};


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
	PAL_SYS_CLKC_ID_T clk_id;	
	if(param == NULL) {
		DEBUG_PAL_PLL("Null parameter\n");
		return;
	}

	/* Initialize clockc data structure */
	memcpy(&clkc_init, param, sizeof(clkc_init));

	/* never touch hardware */
	#ifdef PAL_SYS_CLKC_USE_STUBS
	return;
	#endif

	/* Initialize pointers to clock controller registers */
	apll_regs = (PAL_SYS_ARM_PLL_STRUCT_T*)AVALANCHE_ARM_PLL_BASE;
	upll_regs = (PAL_SYS_AUX_PLL_STRUCT_T*)AVALANCHE_USB_PLL_BASE;
	gpll_regs = (PAL_SYS_AUX_PLL_STRUCT_T*)AVALANCHE_GMII_PLL_BASE;
	dpll_regs = (PAL_SYS_AUX_PLL_STRUCT_T*)AVALANCHE_DOCSIS_PLL_BASE;
	vlynqcr_reg = (volatile UINT32*)AVALANCHE_VLYNQ_CR;
	tdmcr_reg 	= (volatile UINT32*)AVALANCHE_TDM_CR;

	/* vlynq can source wither from sysclk or from GMII */
	set_vlynq_source(clkc_init.vlynq_source);

	/* Initialize sys clock callbacks */
	for(clk_id = PAL_SYS_CLKC_ARM; clk_id <= PAL_SYS_CLKC_SYSCLK15; clk_id++) {
		puma5_handles[clk_id].set_freq 	= set_sysclk_freq;
		puma5_handles[clk_id].get_freq 	= get_sysclk_freq;
		puma5_handles[clk_id].pll_id 	= clk_id; /* sysclocks have 1:1 mapping */
	}

	/* Initialize other handles */
    puma5_handles[PAL_SYS_CLKC_SYSCLKBP].get_freq 	= get_sysbp_freq;
    puma5_handles[PAL_SYS_CLKC_SYSCLKBP].set_freq 	= set_sysbp_freq;
    puma5_handles[PAL_SYS_CLKC_SYSCLKBP].pll_id		= PAL_SYS_CLKC_SYSCLKBP;

    puma5_handles[PAL_SYS_CLKC_AUXCLK].set_freq		= set_auxclk_freq;
    puma5_handles[PAL_SYS_CLKC_AUXCLK].get_freq		= get_auxclk_freq;
    puma5_handles[PAL_SYS_CLKC_AUXCLK].pll_id		= PAL_SYS_CLKC_AUXCLK;
    
	puma5_handles[PAL_SYS_CLKC_OBSCLK].set_freq		= set_obs_freq;
	puma5_handles[PAL_SYS_CLKC_OBSCLK].get_freq		= get_obs_freq;
    puma5_handles[PAL_SYS_CLKC_OBSCLK].pll_id		= PAL_SYS_CLKC_OBSCLK;
    
	puma5_handles[PAL_SYS_CLKC_GMII].set_freq		= set_gmii_freq;
	puma5_handles[PAL_SYS_CLKC_GMII].get_freq		= get_gmii_freq;
    puma5_handles[PAL_SYS_CLKC_GMII].pll_id			= PAL_SYS_CLKC_GMII;
    
	puma5_handles[PAL_SYS_CLKC_USB].set_freq		= set_usb_freq;
	puma5_handles[PAL_SYS_CLKC_USB].get_freq		= get_usb_freq;
    puma5_handles[PAL_SYS_CLKC_USB].pll_id			= PAL_SYS_CLKC_USB;
    
	puma5_handles[PAL_SYS_CLKC_VLYNQ].set_freq		= set_vlynq_freq;
	puma5_handles[PAL_SYS_CLKC_VLYNQ].get_freq		= get_vlynq_freq;
    puma5_handles[PAL_SYS_CLKC_VLYNQ].pll_id		= PAL_SYS_CLKC_VLYNQ;
    
	puma5_handles[PAL_SYS_CLKC_TDM].set_freq		= set_tdm_freq;
	puma5_handles[PAL_SYS_CLKC_TDM].get_freq		= get_tdm_freq;
    puma5_handles[PAL_SYS_CLKC_TDM].pll_id			= PAL_SYS_CLKC_TDM;
    
	puma5_handles[PAL_SYS_CLKC_DOCSIS].set_freq		= set_docsis_freq;
	puma5_handles[PAL_SYS_CLKC_DOCSIS].get_freq		= get_docsis_freq;
    puma5_handles[PAL_SYS_CLKC_DOCSIS].pll_id		= PAL_SYS_CLKC_DOCSIS;

	/* Enable wakeup control for all sys clocks */
	apll_regs->wakeup = 0xFFFF;
	

	/* FIXME BOOTCR initialization should happen in board init code and not here */

        /* Populate the ARM frequency table SYSCLK1 dividers according to base
         * frequency
         */
        {
            int i;
            INT32 base_freq = get_sysclk_base_freq();
            for (i = 0; i < (sizeof (armpll_freq_table)/sizeof (armpll_freq_table[0])) 
                        && (armpll_freq_table[i].freq != -1); i++)
            {
                armpll_freq_table[i].div = base_freq / armpll_freq_table[i].freq;
                //printk ("ARM Freq table entry %d: %d MHz: %d\n", i, armpll_freq_table[i].freq, armpll_freq_table[i].div);
            }
        }

	DEBUG_PAL_PLL("%s : Puma5 Clock Controller Successfully Initialized\n", __FUNCTION__);
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
	if((clk_id < PAL_SYS_CLKC_ARM) || (clk_id >= PAL_SYS_CLKC_MAX_ID)) 
	{
		DEBUG_PAL_PLL("Invalid Argument passed to function %s param = %d\n", 
						__FUNCTION__, clk_id);
		return (-1);
	}

	/* never touch hardware */
	#ifdef PAL_SYS_CLKC_USE_STUBS
	return freq_stubs[clk_id];
	#endif

	return (puma5_handles[clk_id]).get_freq(clk_id);
}


/* ARM freq change is only supported when NOT using high res timers */
#ifndef CONFIG_HIGH_RES_TIMERS
static int set_armclk_freq (int div)
{
    /* As per the h/w design, changing ARM clock (SYSCLK1) requires
     * CLK2 (fast), 3(slow) and 5(arm9) to be updated to maintain the
     * same ratio.
     * DDR : Either set clock ratio or verify it is in ASYNCH mode
     *
     * Note: The code below assumes all the mentioned SYSCLKs fall under same
     * PLL.
     */
    UINT32 cookie;
    int div2, ddr_only = 0, arm_only = 0;

    int timer_clk;

    INT32 pll_id_arm = puma5_handles[PAL_SYS_CLKC_ARM].pll_id;
    INT32 pll_id_fasts = puma5_handles[PAL_SYS_CLKC_SYSCLK2].pll_id;
    INT32 pll_id_slows = puma5_handles[PAL_SYS_CLKC_SYSCLK3].pll_id;
    INT32 pll_id_ddr = puma5_handles[PAL_SYS_CLKC_SYSCLK4].pll_id;
    INT32 pll_id_arm9 = puma5_handles[PAL_SYS_CLKC_SYSCLK5].pll_id;

    volatile UINT32 *div_arm = 0, *div_fasts = 0, *div_slows = 0, *div_ddr = 0, *div_arm9 = 0;
    extern void serial8250_suspend_port(int);
    extern void serial8250_resume_port(int);
    extern unsigned int cpu_freq;

    ddr_only = 0;
    arm_only = 1;

    GET_SYSCLK_DIV_PTR(pll_id_arm, div_arm); 
    GET_SYSCLK_DIV_PTR(pll_id_fasts, div_fasts); 
    GET_SYSCLK_DIV_PTR(pll_id_slows, div_slows); 
    GET_SYSCLK_DIV_PTR(pll_id_arm9, div_arm9); 

    if (!div_arm || !div_fasts || !div_slows || !div_arm9)
        return -1;

    if (arm_only)
    {
        /* Verify the DDR is in ASYNCH mode */
        if (!IS_DDR_ASYNCH)
            return -1;
    }

    /* check if PLL is busy doing some transition */
    RETURN_ON_BUSY(pll_id);

    if (!ddr_only)
		serial8250_suspend_port (0);

    /* Stop timer */
    PAL_sysTimer16Ctrl(AVALANCHE_TIMER0_BASE, TIMER16_CTRL_STOP);

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);

    if (!ddr_only) {
        /* Update divider */
        *div_arm = PAL_SYS_CLKC_DIV_ENABLE | (div-1);
        *div_fasts = PAL_SYS_CLKC_DIV_ENABLE | ((div*2)-1);  //CLK3
        *div_slows = PAL_SYS_CLKC_DIV_ENABLE | ((div*4)-1);  //CLK4
        *div_arm9 = PAL_SYS_CLKC_DIV_ENABLE | ((div*2)-1);   //CLK5

        /* enable Align register before GO */
        ARM_PLL_ALIGN(pll_id_arm);
        ARM_PLL_ALIGN(pll_id_fasts);
        ARM_PLL_ALIGN(pll_id_slows);
        ARM_PLL_ALIGN(pll_id_arm9);

        div2 = div;
    }
    else
        div2 = div;

    if (!arm_only) {
        GET_SYSCLK_DIV_PTR(pll_id_ddr, div_ddr); 
        *div_ddr = PAL_SYS_CLKC_DIV_ENABLE | (div2-1);   //CLK5
        ARM_PLL_ALIGN(pll_id_ddr);
    }

    /* Program the GOSET bit to take new divier values */
    ARM_PLL_GO();

    /* Wait for PLL to Reset Properly */
    pll_delay(2000000);

    /* Wait for Done */ 
    WAIT_FOR_PLL_UPDATE();

    avalanche_set_vbus_freq (get_sysclk_base_freq()/(div*4));
    timer_clk = avalanche_get_vbus_freq ();
    PAL_sysTimer16SetParams(AVALANCHE_TIMER0_BASE, timer_clk, 
            TIMER16_CNTRL_AUTOLOAD, (int)((1.0/(float)(HZ)) * 1000000.0), NULL);

    cpu_freq = PAL_sysClkcGetFreq(CLKC_ARM);  

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    /* Start timer */
    PAL_sysTimer16Ctrl(AVALANCHE_TIMER0_BASE, TIMER16_CTRL_START);
    if (!ddr_only) {
        serial8250_resume_port (0);
    }
    return 0;
}
#endif /* !CONFIG_HIGH_RES_TIMERS */

/** \func PAL_sysClkcSetFreq
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
INT32 PAL_sysClkcSetFreq (PAL_SYS_CLKC_ID_T clk_id, UINT32 output_freq)
{
	/* sanity check */
	if((clk_id < PAL_SYS_CLKC_ARM) || (clk_id >= PAL_SYS_CLKC_MAX_ID)) 
	{
		DEBUG_PAL_PLL("Invalid Argument passed to function %s param = %d\n", 
						__FUNCTION__, clk_id);
		return (-1);
	}

	/* never touch hardware */
	#ifdef PAL_SYS_CLKC_USE_STUBS
	freq_stubs[clk_id] = output_freq;
	return 0;
	#endif

	/* clock setting is not allowed only for ARM (sysclock 0) */
	if(clk_id == PAL_SYS_CLKC_ARM)
	{
/* ARM freq change is only supported when NOT using high res timers */
#ifndef CONFIG_HIGH_RES_TIMERS
            int i;

            //printk ("Checking if desired ARM freq is supported...\n");
            for (i = 0; i < (sizeof (armpll_freq_table)/sizeof (armpll_freq_table[0]))
                        && (armpll_freq_table[i].freq != -1); i++)
            {
                if (output_freq == armpll_freq_table[i].freq) {
                    //printk ("ARM Freq table entry %d: %d MHz: %d matched\n", i, armpll_freq_table[i].freq, armpll_freq_table[i].div);
                    break;
                }
            }

            if (i >= (sizeof (armpll_freq_table)/sizeof (armpll_freq_table[0]))
                    || (armpll_freq_table[i].freq == -1))
            {
                printk ("%s : Unsupported ARM clock frequency %d\n", __FUNCTION__, output_freq);
                printk ("Supported values are: ");
                for (i = 0; i < (sizeof (armpll_freq_table)/sizeof (armpll_freq_table[0])); i++)
                    printk ("%d ", armpll_freq_table[i].freq);
                printk ("\n");
                return -1;
            }

            return set_armclk_freq (armpll_freq_table[i].div);
#else /* !CONFIG_HIGH_RES_TIMERS */
            printk ("%s : Can't set ARM clock (%d) from OS when CONFIG_HIGH_RES_TIMERS is enabled\n", 
                    __FUNCTION__, clk_id);
            return -1;
#endif /* !CONFIG_HIGH_RES_TIMERS */
	}

/* FIXME do we really need to validate range? 
 * Function which invokes this APIshould worry about the frequency range. 
 * If not possible to set requested freq then let module "set function" return -1.
 */
#if 0
	if(puma5_handles[clk_id].max_freq != 0) /* any freq is possible */
	{
		if((output_freq < puma5_handles[clk_id].min_freq) || 
				(output_freq > puma5_handles[clk_id].max_freq))
		{
			DEBUG_PAL_PLL("%s : Set frequency (%d) out of range for module %d, \
						allowed range is %dHz to %dHz\n",
						__FUNCTION__, output_freq, clk_id,
						puma5_handles[clk_id].min_freq,
						puma5_handles[clk_id].max_freq);
			return (-1);
		}
	}
#endif

	return (puma5_handles[clk_id]).set_freq(clk_id, output_freq);
}
/*********************************************************************
 * 
 * SECTION : Exported PAL_SYS Clock APIs 
 * <END>
 *
 * *******************************************************************/

