/*
 *
 * pal_sysGpioCtrl.c
 * Description:
 * GPIO control APIs
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

/** \file   pal_sysGpioCtrl.c
 *  \brief  PAL GPIO control APIs
 *    
 *  \author     PSP, TI
 *
 *  \note       Set tabstop to 4 (:se ts=4) while viewing this file in an
 *              editor
 *
 *  \version    
 *  				0.1     Ajay Sing   		Created
 *  (11-Jul-2007)	0.2     Mansoor Ahamed		For Avalanche3 architecture
 */

#include <asm-arm/arch-avalanche/generic/pal.h>

/*! \fn void PAL_sysGpioInit(void)
    \brief This API returns Initializes the GPIO module
*/
void PAL_sysGpioInit(void)
{
    UINT32 cookie;

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
	/* Put the module in reset state */
	PAL_sysResetCtrl(AVALANCHE_GPIO_RESET_BIT, IN_RESET);

	PAL_osWaitMsecs(1);

	/* Bring the module out of reset */
	PAL_sysResetCtrl(AVALANCHE_GPIO_RESET_BIT, OUT_OF_RESET);

    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
}

/*! \fn INT32 PAL_sysGpioCtrl(UINT32 gpio_pin,
                    PAL_SYS_GPIO_PIN_MODE_T pin_mode,
                    PAL_SYS_GPIO_PIN_DIRECTION_T pin_direction)
    \brief This API initializes the mode of a specific GPIO pin
	\param gpio_pin GPIO pin number (auxiliary GPIOs start after primary GPIOs)
    \param pin_mode	GPIO pin mode (GPIO_PIN, FUNCTIONAL_PIN)
	\param pin_direction Direction of the gpio pin in GPIO_PIN mode
	\return Returns (0) on success and (-1) on failure
*/
INT32 PAL_sysGpioCtrl(UINT32 gpio_pin,
                    PAL_SYS_GPIO_PIN_MODE_T pin_mode,
                    PAL_SYS_GPIO_PIN_DIRECTION_T pin_direction)
{
    UINT32 cookie;
    volatile UINT32 *gpio_ctrl;
	UINT32 offset = 0; 

    if(gpio_pin >= AVALANCHE_MAX_GPIOS)
	{
        return(-1);
	}

	/* adjust offset and gpio_pin if auxiliary GPIOs */
	if(gpio_pin >= AVALANCHE_MAX_PRIMARY_GPIOS)
	{
		offset = AVALANCHE_AUX_GPIO_OFFSET;
		gpio_pin -= AVALANCHE_MAX_PRIMARY_GPIOS;
	}


    gpio_ctrl = (UINT32*)(AVALANCHE_GPIO_ENBL + offset);

	/* Critical_Start */
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);

    if(pin_mode == GPIO_PIN)
    {
        *gpio_ctrl |= (1 << gpio_pin);

	    gpio_ctrl = (UINT32*)(AVALANCHE_GPIO_DIR + offset);
        
        if(pin_direction == GPIO_INPUT_PIN)
		{
            *gpio_ctrl |=  (1 << gpio_pin);
		}
        else
		{
            *gpio_ctrl &= ~(1 << gpio_pin);
		}
    }
    else /* FUNCTIONAL PIN */
    {
        *gpio_ctrl &= ~(1 << gpio_pin);
    }

	/* Critical_End */
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return (0);
}

EXPORT_SYMBOL(PAL_sysGpioCtrl);

/*! \fn INT32 PAL_sysGpioOutBit(UINT32 gpio_pin, INT32 value) 
    \brief This API outputs the specified value on the gpio pin
	\param gpio_pin GPIO pin number (auxiliary GPIOs start after primary GPIOs)
    \param value 0/1 (TRUE/FALSE)
	\return Returns (0) on success and (-1) on failure
*/
INT32 PAL_sysGpioOutBit(UINT32 gpio_pin, INT32 value)
{
    UINT32 cookie;
    volatile UINT32 *gpio_out;
 	UINT32 offset = 0; 

    if(gpio_pin >= AVALANCHE_MAX_GPIOS)
	{
        return(-1);
	}

	/* adjust offset and gpio_pin if auxiliary GPIOs */
	if(gpio_pin >= AVALANCHE_MAX_PRIMARY_GPIOS)
	{
		offset = AVALANCHE_AUX_GPIO_OFFSET;
		gpio_pin -= AVALANCHE_MAX_PRIMARY_GPIOS;
	}

    gpio_out = (UINT32*) (AVALANCHE_GPIO_DATA_OUT + offset);
  
  	/* Critical_Start */	
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie); 

    if(value == TRUE)
	{
        *gpio_out |=   1 << gpio_pin;
	}
    else
	{
	    *gpio_out &= ~(1 << gpio_pin);
	}

  	/* Critical_End */	
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);   

    return(0);
}
EXPORT_SYMBOL(PAL_sysGpioOutBit);

/*! \fn INT32 PAL_sysGpioInBit(UINT32 gpio_pin)
    \brief This API reads the specified gpio_pin and returns the value
	\param gpio_pin GPIO pin number (auxiliary GPIOs start after primary GPIOs)
	\return Returns gpio status 0/1 (TRUE/FALSE), on failure returns (-1)
*/
INT32 PAL_sysGpioInBit(UINT32 gpio_pin)
{
    UINT32 cookie;
    volatile UINT32 *gpio_in;
    INT32 ret_val = 0;
  	UINT32 offset = 0; 

    if(gpio_pin >= AVALANCHE_MAX_GPIOS)
	{
        return(-1);
	}

	/* adjust offset and gpio_pin if auxiliary GPIOs */
	if(gpio_pin >= AVALANCHE_MAX_PRIMARY_GPIOS)
	{
		offset = AVALANCHE_AUX_GPIO_OFFSET;
		gpio_pin -= AVALANCHE_MAX_PRIMARY_GPIOS;
	}


    gpio_in = (UINT32*) (AVALANCHE_GPIO_DATA_IN + offset);

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie); 
    ret_val = ((*gpio_in) & (1 << gpio_pin)) >> gpio_pin;
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
 
    return (ret_val);
}

EXPORT_SYMBOL(PAL_sysGpioInBit);

/*! \fn INT32 PAL_sysGpioOutValue(UINT32 out_val, UINT32 out_mask,UINT32 reg_index)
    \brief This API outputs the specified value on the GPIOs (after masking) in the specified register
	\param out_val value to be output on the GPIO pins
	\param out_mask mask for the gpio ins to be used
	\param reg_index which gpio register should be used
	\return Returns (0) on success and (-1) on error
*/
INT32 PAL_sysGpioOutValue(UINT32 out_val, UINT32 out_mask,UINT32 reg_index)
{
    UINT32 cookie;
    volatile UINT32 *gpio_out;
  	UINT32 offset = 0; 

    if(reg_index >= AVALANCHE_MAX_GPIO_REGS)
	{
        return(-1);
	}

	if(reg_index >= AVALANCHE_MAX_PRIMARY_GPIO_REGS)
	{
		offset = AVALANCHE_AUX_GPIO_OFFSET;
	}

    gpio_out = (UINT32*) (AVALANCHE_GPIO_DATA_OUT + offset);

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie); 
    *gpio_out &= ~out_mask;
    *gpio_out |= out_val;
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return(0);
}
EXPORT_SYMBOL(PAL_sysGpioOutValue);

/*! \fn INT32 PAL_sysGpioInValue(UINT32* in_val, UINT32 reg_index)
    \brief This API reads the specified GPIO register
	\param in_val value read from the gpio register
	\param reg_index which gpio register should be used
	\return Returns (0) on success and (-1) on error
*/
INT32 PAL_sysGpioInValue(UINT32* in_val, UINT32 reg_index)
{
    UINT32 cookie;
    volatile UINT32 *gpio_in;
   	UINT32 offset = 0; 

    if(reg_index >= AVALANCHE_MAX_GPIO_REGS)
	{
        return(-1);
	}

	if(reg_index >= AVALANCHE_MAX_PRIMARY_GPIO_REGS)
	{
		offset = AVALANCHE_AUX_GPIO_OFFSET;
	}

    gpio_in = (UINT32*) (AVALANCHE_GPIO_DATA_IN + offset);

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie); 
    *in_val = *gpio_in;
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie); 

    return (0);
}

EXPORT_SYMBOL(PAL_sysGpioInValue);
