/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2007 by viresh Kumar  			*/
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
#include "spr_adc_st10_hal.h"

/*------------------------------------------------------------------------
 * Global Variables 
 *----------------------------------------------------------------------*/

/****************************************************************************/
/* 			 ADC_Enable()	                        	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Enables the ADC.                                       */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_ADCError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_ADCError ADC_Enable(void)
{
	*ADCSTATUS = ADC_ENABLE;
	return ADC_OK;
}

/****************************************************************************/
/* 			 ADC_Disable()	                        	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Disables the ADC.                                       */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_ADCError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_ADCError ADC_Disable(void)
{
	*ADCSTATUS = *ADCSTATUS & ADC_DISABLE;
	return ADC_OK;
}


/****************************************************************************/
/* NAME: 		ADC_Configure	                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine configures the ADC before starting a conversion	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :	                                                            */
/*      - Config: Configuration of ADC		                            */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_ADCError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_ADCError ADC_Configure(t_ADCConfig Config)
{
	uint TotalClock = Config.ClkHigh + Config.ClkLow;

	//Check ChannelId.
	if((Config.ChannelId < CHANNEL0) | (Config.ChannelId > CHANNEL7))
		return ADC_INVALID_CHANNEL_ID;

	//Check AvgSamples.
	if((Config.AvgSamples < SAMPLE1) | (Config.AvgSamples > SAMPLE128))
		return ADC_INVALID_SAMPLE_SIZE;

	//Check Clock Hi and Low.
	if((Config.ClkHigh > 15) | (Config.ClkLow > 15) | (TotalClock > ADC_MAX_CLK) | (TotalClock < ADC_MIN_CLK))
		return ADC_INVALID_CLOCK_SCALING;

	*ADCSTATUS = ADC_ENABLE;

	//Check VoltRef.
	if(Config.VoltRef == INTERNAL_VOLT)
		*ADCSTATUS = *ADCSTATUS | REF_VOLT_INT;
	else if(Config.VoltRef == EXTERNAL_VOLT)
		*ADCSTATUS = *ADCSTATUS & REF_VOLT_EXT;
	else
		return ADC_INVALID_VOLT_REF;

	*ADCSTATUS = *ADCSTATUS | (Config.ChannelId << CHANNEL_SELECT_SHIFT);
	*ADCSTATUS = *ADCSTATUS | (Config.AvgSamples << SAMPLE_SIZE_SHIFT);

	*ADCCLK = Config.ClkLow | (Config.ClkHigh << CLK_HIGH_SHIFT);

	return ADC_OK;
}

/****************************************************************************/
/* NAME: 		ADC_StartConversion                                 */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine converts a analog voltage to digital voltage. 	    */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_ADCError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_ADCError ADC_StartConversion(void)
{
	uint Condition = *ADCSTATUS;
	if(!(Condition & CONVERSION_COMPLETE_MASK) && !(Condition & START_CONVERSION))
	{
		*ADCSTATUS = Condition | START_CONVERSION;
		return ADC_OK;
	}
	else
		return ADC_BUSY;
}

/****************************************************************************/
/* NAME: 		ADC_GetConvertedData                                */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine returns the digital data converted by ADC.        	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :	                                                            */
/* OUT : 	                                                            */
/*      - DigitalVolt: Output Digital Data 	                            */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_ADCError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_ADCError ADC_GetConvertedData(uint *DigitalVolt)
{
	uint AvgSamples, Val, Condition = *ADCSTATUS;

//	AvgSamples = (Condition >> SAMPLE_SIZE_SHIFT) & SAMPLE_SIZE_SELECT_MASK;

	//Get Data according a particular channel.
	if((Condition & CONVERSION_COMPLETE_MASK) && (Condition & START_CONVERSION))
	{
//		Val = *ADCAVERAGE;
//		*DigitalVolt = (Val >> AvgSamples) & DATA_MASK;	//This is used if we have to shift data register according to the document
		*DigitalVolt = *ADCAVERAGE & DATA_MASK;	//Currently we don't require any shift.

		return ADC_OK;
	}
	else
		return ADC_CONVERSION_ONGOING;
}
