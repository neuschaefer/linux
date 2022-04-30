/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2006 by viresh Kumar  			*/
/*   viresh.kumar@st.com							*/
/*										*/
/*   This program is free software; you can redistribute it and/or modify	*/
/*   it under the terms of the GNU General Public License as published by	*/
/*   the Free Software Foundation; either version 2 of the License, or		*/
/*   (at your option) any later version.					*/
/*										*/
/*   This program is distributed in the hope that it will be useful,		*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 		*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*/
/*   GNU General Public License for more details.				*/
/*										*/
/*   You should have received a copy of the GNU General Public License		*/
/*   along with this program; if not, write to the				*/
/*   Free Software Foundation, Inc.,						*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.			*/
/********************************************************************************/

/*------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------*/
#include "spr_gpio_pl061_hal.h"

//Array of Interrupt Handlers.
volatile t_GPIOIntHandler IntHandlers[GPIO_NUM][GPIO_PINS] = {{NULL}};

//Array of Base Addresses.
#ifdef CONFIG_ARCH_SPEARPLUS
volatile uint BaseAddress[GPIO_NUM] = { VA_SPEARPLUS_START_APB_GPIO0, VA_SPEARPLUS_START_APB_GPIO1, VA_SPEARPLUS_START_APB_GPIO2 };
#endif

#ifdef CONFIG_ARCH_SPEARBASIC
volatile uint BaseAddress[GPIO_NUM] = {VA_SPEARBASIC_START_APB_GPIO0, VA_SPEARBASIC_START_APB_GPIO1 };
#endif



/*------------------------------------------------------------------------
 * a global structure representing the entire GPIO
 *----------------------------------------------------------------------*/

/****************************************************************************/
/* NAME: 		GetBaseAddress		                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Gets GPIO Base Address				    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - GPIOId: identify the GPIO Device	                            */
/*                                                                          */
/* OUT  :                                                                   */
/*                                                                          */
/* RETURN:                                                                  */
/*        Base address of the GPIO                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
uint *GetBaseAddress(t_GPIOId GPIOId)
{
    return &(BaseAddress[GPIOId]);
}

/****************************************************************************/
/* NAME: 		SetBaseAddress		                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Sets GPIO Base Address				    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - GPIOId: identify the GPIO Device	                            */
/*                                                                          */
/* OUT  :                                                                   */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_GPIOError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static t_GPIOError SetBaseAddress(volatile uint *SPEAR_GPIO_APB_BASE, t_GPIOId GPIOId)
{
	//Checking if GPIOId is correct.
	if((GPIOId < GPIO0) ||(GPIOId >= GPIO_NONE))
		return GPIO_INVALID_ID;

	//Setting base address of the GPIO
	*SPEAR_GPIO_APB_BASE = BaseAddress[GPIOId];

	return GPIO_OK;
}


/****************************************************************************/
/* NAME:           GPIO_Init                                                */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Initializes the GPIO to the Default Configurations	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : None                                                               */
/* OUT : None                                                               */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_GPIOError GPIO_Init()
{
	t_GPIOError Status;
	t_GPIOId GPIOId;

	for(GPIOId = GPIO0; GPIOId < GPIO_NONE; GPIOId += (t_GPIOId)1)
	{
		if((Status = GPIO_ConfigDefault(GPIOId)) != GPIO_OK)
			return Status;
	}

	return GPIO_OK;
}

/****************************************************************************/
/* NAME:           GPIO_ConfigDefault					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This API is used to configure the GPIO to the default settings.	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : None                                                               */
/* OUT : None                                                               */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_GPIOError GPIO_ConfigDefault(t_GPIOId GPIOId)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	t_GPIOError Status;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//All Registers are Cleared at Reset.
	*GPIODATA	= RESET;
	*GPIODIR 	= RESET;
	*GPIOIS  	= RESET;
	*GPIOIBE 	= RESET;
	*GPIOIEV 	= RESET;
	*GPIOIE	 	= RESET;
	*GPIORIS 	= RESET;
	*GPIOMIS 	= RESET;
	*GPIOIC	 	= RESET;

	return GPIO_OK;
}


/****************************************************************************/
/* NAME:           GPIO_GetMode					  	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This API is used to Get the Mode configuration of the GPIO.	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :  None								    */
/* OUT : 								    */
/*	Mode: Mode value required.				            */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_GPIOError GPIO_GetMode(t_GPIOId GPIOId, uchar *Mode)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	t_GPIOError Status;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//Returning Mode of Operation of GPIO.
	*Mode = (*GPIOAFSEL) & 0x000000FF;
	return GPIO_OK;
}


/****************************************************************************/
/* NAME:           GPIO_DirSet						    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This API is used to Set the direction of the Specified GPIO Pins.   */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : 								    */
/*	Dir: Dir of the pins to be set					    */
/*	Mask: Pins whose direction is required to be set 		    */
/* OUT : 								    */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

t_GPIOError GPIO_DirSet(t_GPIOId GPIOId, t_GPIODir Dir, uint Mask)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	t_GPIOError Status;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//If the Mask is invalid.
	if(Mask >= 0xFF)
	{
		return GPIO_INVALID_PIN_ID;
	}

	//If the specified pins are not in software mode then return error.
	if(((~ *GPIOAFSEL) & Mask) != Mask)
	{
		return GPIO_PIN_NOT_IN_SOFWARE_MODE;
	}


	//If pins are in sofware mode than set the required pins in IN or OUT Mode else return a error if Dir has Invalid Value.
	if(Dir == OUT)
		*GPIODIR |= Mask;
	else if(Dir == IN)
		*GPIODIR &= ~ Mask;
	else
		return GPIO_INVALID_PIN_DIRECTION;

	return GPIO_OK;
}

/****************************************************************************/
/* NAME:           GPIO_DirGet					  	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This API is used to Get the direction of the Specified GPIO Pins.   */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : 								    */
/*	Mask: Pin whose direction is required				    */
/* OUT : 								    */
/*	Dir: Direction of the pin					    */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_GPIOError GPIO_DirGet(t_GPIOId GPIOId, t_GPIODir *Dir,uint Mask)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	t_GPIOError Status;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//If the Mask is invalid.
	if(Mask >= 0x0FF)
	{
		return GPIO_INVALID_PIN_ID;
	}

	//If Mask is valid.
	else 
	{
		*Dir = (*GPIODIR) & Mask;
		return GPIO_OK;
	}
}


/****************************************************************************/
/* NAME:           GPIO_ValSet						    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This API is used to Set the value of the Specified GPIO Pins.       */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : 								    */
/*	Mask: Pin whose value is to be Set.				    */
/*	Val: Value of the pin to be set.				    */
/* OUT : 								    */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

t_GPIOError GPIO_ValSet(t_GPIOId GPIOId, t_GPIOValue Val, uint Mask)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	t_GPIOError Status;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//If the Mask is invalid.
	if(Mask >= 0xFF)
	{
		return GPIO_INVALID_PIN_ID;
	}

	//If the specified pins are not in software mode then return error.
	if(((~ *GPIOAFSEL) &Mask) != Mask)
	{
		return GPIO_PIN_NOT_IN_SOFWARE_MODE;
	}

	//If the Pins are not in Output Mode.
	if((*GPIODIR & Mask) != Mask)
	{
		return GPIO_NOT_AN_OUTPUT_PIN;
	}

	//If pins are in sofware mode than set the value of pins else return a error if Val is Invalid Value.
	if(Val == SET)
		*(GPIODATA + Mask) = 0xFF;
	else if(Val == RESET)
		*(GPIODATA + Mask) = 0x00;
	else
		return GPIO_INVALID_PIN_VALUE;

	return GPIO_OK;
}


/****************************************************************************/
/* NAME:           GPIO_ValGet						    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This API is used to Get the value at the Specified GPIO Pin.        */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : 								    */
/*	Mask: Pin whose Value is Required				    */
/* OUT : 								    */
/*	Val: Value at the pin.						    */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_GPIOError GPIO_ValGet(t_GPIOId GPIOId, t_GPIOValue *val, uint Mask)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	t_GPIOError Status;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//If the Mask is invalid.
	if(Mask >= 0x0FF)
	{
		return GPIO_INVALID_PIN_ID;
	}

	//If the Pins are not in Output Mode.
	if(((~ *GPIODIR) & Mask) != Mask)
	{
		return GPIO_NOT_AN_INPUT_PIN;
	}

	//Reading Value at Pin
	*val = *(GPIODATA + Mask);
	return GPIO_OK;
}


/********************************************************************************/
/* NAME:           GPIO_EdgeConfig					        */
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to configure Edge Interrupt on the specified GPIO Pins. 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/*	IntEdge: Type of interrupt to be set Single Edge or Both Edge.	    	*/
/*	IntSingleEdge : Rising or Falling Edge interrupt in case of Single Edge	*/
/*	Mask: Pin required to be set for Edge Interrupt			    	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_GPIOError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_GPIOError GPIO_EdgeConfig(t_GPIOId GPIOId, t_GPIOIntEdgeConfig IntEdge, t_GPIOIntSingleEdgeConfig IntSingleEdge, uint Mask)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	t_GPIOError Status;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//If the Mask is invalid.
	if(Mask >= 0xFF)
	{
		return GPIO_INVALID_PIN_ID;
	}

	//If the specified pins are not in software mode then return error.
	if(((~ *GPIOAFSEL) & Mask) != Mask)
	{
		return GPIO_PIN_NOT_IN_SOFWARE_MODE;
	}

	//If pins are in sofware mode than set them to detect Edge Interrupts or show an error if IntEdge or IntSingleEdge are Invalid.
	*GPIOIS &= ~Mask;
	switch(IntEdge)
	{
		case BOTH_EDGE: 
				*GPIOIBE |= Mask;
				break;
		case SINGLE_EDGE: 
				*GPIOIBE &= (~ Mask);
				if(IntSingleEdge == RISING_EDGE)
				{
					*GPIOIEV |= Mask;
				}
				else 
					if(IntSingleEdge == FALLING_EDGE)
						*GPIOIEV &= (~ Mask);
					else
						return GPIO_INVALID_INTERRUPT_TYPE;
				break;
		default:
				return GPIO_INVALID_INTERRUPT_TYPE;
				break;
	}
	return GPIO_OK;
}


/********************************************************************************/
/* NAME:           GPIO_LevelConfig						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to configure Level Interrupt on the specified GPIO Pins. 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/*	IntLevel: Type of interrupt to be set Low Level or High Level.	    	*/
/*	Mask: Pin required to be set for Level Interrupt			*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_GPIOError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_GPIOError GPIO_LevelConfig(t_GPIOId GPIOId, t_GPIOIntLevelConfig IntLevel, uint Mask)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	t_GPIOError Status;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//If the Mask is invalid.
	if(Mask >= 0x0FF)
	{
		return GPIO_INVALID_PIN_ID;
	}

	//If the specified pins are not in software mode then return error.
	if(((~ *GPIOAFSEL) & Mask) != Mask)
	{
		return GPIO_PIN_NOT_IN_SOFWARE_MODE;
	}


	//If pins are in sofware mode than set them to detect Level Interrupts or show an error if IntLevel is Invalid.
	*GPIOIS |= Mask;
	switch(IntLevel)
	{
		case LOW_LEVEL: 
				*GPIOIEV &= (~ Mask);
				break;
		case HIGH_LEVEL: 
				*GPIOIEV |= Mask;
				break;
		default: 
				return GPIO_INVALID_INTERRUPT_TYPE;
				break;
	}
	return GPIO_OK;
}


/********************************************************************************/
/* NAME:           GPIO_IntEnable						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Enable Interrupt on the specified GPIO Pins.	 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/*	Mask: Pin required to be set for Interrupts				*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_GPIOError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_GPIOError GPIO_IntEnable(t_GPIOId GPIOId, uint Mask)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	uint Index = 1;
	uint IndexPtr = 0;
	t_GPIOError Status;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//If the Mask is invalid.
	if(Mask >= 0x0FF)
	{
		return GPIO_INVALID_PIN_ID;
	}

	//If the specified pins are not in software mode then return error.
	if(((~ *GPIOAFSEL) & Mask) != Mask)
	{
		return GPIO_PIN_NOT_IN_SOFWARE_MODE;
	}

	//Enabling Interrupt on the Pin.
	while(Index <= Mask)
	{
		if(Mask & Index)
		{
			if(IntHandlers[GPIOId][IndexPtr] == NULL)
				return GPIO_PIN_NO_INT_HANDLER_REGISTERED;
		}
		Index <<= 1;
		IndexPtr++;
	}

	//If pins are in sofware mode than set them to detect Interrupt.
	*GPIOIE |= Mask;
	return GPIO_OK;
}

/********************************************************************************/
/* NAME:           GPIO_IntDisable						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Disable Interrupt on the specified GPIO Pins.	 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/*	Mask: Pin required to be Disabled for Interrupts			*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_GPIOError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_GPIOError GPIO_IntDisable(t_GPIOId GPIOId, uint Mask)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	t_GPIOError Status;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//If the Mask is invalid.
	if(Mask >= 0x0FF)
	{
		return GPIO_INVALID_PIN_ID;
	}

	//If the specified pins are not in software mode then return error.
	if(((~ *GPIOAFSEL) & Mask) != Mask)
	{
		return GPIO_PIN_NOT_IN_SOFWARE_MODE;
	}

	//If pins are in sofware mode than set them not to detect Interrupt.
	*GPIOIE &= ~Mask;
	return GPIO_OK;
}


/********************************************************************************/
/* NAME:           GPIO_IntClear						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to clear Interrupts on the specified GPIO Pins.	 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/*	Mask: Pin whose interrupts are required to be cleared.			*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_GPIOError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_GPIOError GPIO_IntClear(t_GPIOId GPIOId, uint Mask)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	t_GPIOError Status;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//If the Mask is invalid.
	if(Mask >= 0x0FF)
	{
		return GPIO_INVALID_PIN_ID;
	}

	//If the specified pins are not in software mode then return error.
	if(((~ *GPIOAFSEL) & Mask) != Mask)
	{
		return GPIO_PIN_NOT_IN_SOFWARE_MODE;
	}

	//If pins are in sofware mode than set them not to detect Interrupt.
	*GPIOIC |= Mask;
	return GPIO_OK;
}


/********************************************************************************/
/* NAME:           GPIO_BindIntHandler						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Bind Interrupt Handler with the specified GPIO Pins.	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/*	Mask: Pin required to be Cleared for Interrupts				*/
/*	HandlerPtr: Pointer to Interrupt handler to be registered		*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_GPIOError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_GPIOError GPIO_BindIntHandler(t_GPIOId GPIOId, uint Mask, t_GPIOIntHandler HandlerPtr)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	uint Index = 0;
	t_GPIOError Status;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//If the Mask is invalid.
	if(Mask >= 0x0FF)
	{
		return GPIO_INVALID_PIN_ID;
	}

	//If the specified pins are not in software mode then return error.
	if(((~ *GPIOAFSEL) & Mask) != Mask)
	{
		return GPIO_PIN_NOT_IN_SOFWARE_MODE;
	}

	//If HandlerPtr is NULL return error message.
	if(HandlerPtr == NULL)
		return GPIO_INVALID_INTERRUPT_HANDLER;

	//Set HandlerPtr as the pointer to interrupt handlers for all the specified Pins.
	while(Mask)
	{
		if(Mask & 1)
		{
			IntHandlers[GPIOId][Index] = HandlerPtr;
		}
		Index++;
		Mask >>= 1;
	}
	return GPIO_OK;
}



/********************************************************************************/
/* NAME:           GPIO_IntHandler						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This is Interrupt Handler function.						*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: 								    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_GPIOError GPIO_IntHandler(t_GPIOId GPIOId)
{
	volatile uint SPEAR_GPIO_APB_BASE;
	t_GPIOError Status;
	uint Index = 1;
	uint IndexPtr = 0;

	//Setting Base Address of GPIO.
	if((Status = SetBaseAddress(&SPEAR_GPIO_APB_BASE, GPIOId)) != GPIO_OK)
		return Status;

	//Handling all the active interrupts.
	while(Index < 256)
	{
		//If Interrupt is due to this Pin.
		if(*GPIOMIS & Index)
		{
			//Checking if a Interrupt handler is registered.
			if(IntHandlers[GPIOId][IndexPtr] != NULL)
				(*IntHandlers[GPIOId][IndexPtr])();

			//Clearing Interrupts.
			(*GPIOIC) |= Index;
		}
		//Shifting to next GPIO Line.
		Index <<= 1;
		IndexPtr++;
	}

	return GPIO_OK;
}

