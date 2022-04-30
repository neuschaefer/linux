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
#include "spr_ssp_pl022_hal.h"

/*------------------------------------------------------------------------
 * Global Variables 
 *----------------------------------------------------------------------*/

//Storing the Latest Configurations.
extern t_SSPConfigure SSPCurrentConfig[];

/****************************************************************************/
/* NAME: 		GetFreqAvailable	            	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine calculates the bit rate and Prescale Divisor for       */
/*      configuring SSP to the desired frequency. In case of incorrect      */
/*      frequency passed it returns -1 as error. It also returns the closest*/
/*      attainable frequency.                                               */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*     - RequiredFrequency:- It tells the required frequency to which SSP   */
/*			     should be configured.                          */
/*     - PrescaleVal:-       Value of Prescale Factor.                      */
/*     - SerialClkRate:-     Value of Serial Clock Rate.                    */
/*     - AttainedFrequency:- It tells the Attained frequency to which SSP   */
/*			     is be configured.                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        - Attained frequency                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static uint GetFreqAvailable(uint RequiredFrequency, uint *PrescaleDiv, uint *SerialClkRate, uint *AttainedFrequency)
{
	uint FreqMax, FreqMin, BestAF = 0, BestPD = 0, BestSCR = 0, TempFreq = 0;

	//Check that the pointers are valid or not.
	if ((PrescaleDiv == NULL) || (SerialClkRate == NULL) || (AttainedFrequency == NULL))
	{
		return SSP_NULL_POINTER;
	}

	//Initilizing Division factors.
	*PrescaleDiv = MIN_PSDVSR;
	*SerialClkRate = MIN_SCR;

	// Calculate effective frequency only in master mode
	FreqMax = SPEAR_APB_CLK / (MIN_PSDVSR * (1 + MIN_SCR)); // psdvsr = 2 & scr = 0
	FreqMin = SPEAR_APB_CLK / (MAX_PSDVSR * (1 + MAX_SCR)); // psdvsr = 254 & scr = 255

	//If Required Frequency is in the range of valid frequencies.
	if((RequiredFrequency <= FreqMax) && (RequiredFrequency >= FreqMin))
	{
		while (*PrescaleDiv <= MAX_PSDVSR)
		{
			while(*SerialClkRate <= MAX_SCR)
			{
				TempFreq = SPEAR_APB_CLK / ((*PrescaleDiv) * (1 + *SerialClkRate));

				// This bool is made TRUE when effective frequency > target frequency
				if (TempFreq > RequiredFrequency)
					(*SerialClkRate)++;
				else if(TempFreq == RequiredFrequency)
				{
					*AttainedFrequency = TempFreq;
					return SSP_OK;
				}
				else
				{
					if(!BestAF || ((RequiredFrequency - BestAF) > (RequiredFrequency - TempFreq)))
					{
						BestAF = TempFreq;
						BestPD = *PrescaleDiv;
						BestSCR = *SerialClkRate;
					}
					break;
				}
			}
			*PrescaleDiv += 2;
			*SerialClkRate = MIN_SCR;
		}

		//Calculate the Final Frequency.
		*AttainedFrequency = BestAF;
		*PrescaleDiv = BestPD;
		*SerialClkRate = BestSCR;
	}
	else
		return SSP_INVALID_FREQUENCY;

	return SSP_OK;
}

/****************************************************************************/
/* 			 SSP_Reset()	                	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Resets a SSP Device, disables it and clears all 	    */
/*	interrupts.							    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: Identify the SSP Device	                            */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        void	                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SSP_Reset(t_SSPId SSPId)
{
	volatile uint SPEAr_SSP_APB_BASE;

	//Setting Base address of the device.
	SELECT_DEVICE;

	//Clearing the Receive FIFO.
	while(!SSP_IsRxFIFOEmpty)
	{
		*SSPDR;
	}

	//Clearing the Interrupts and Control Registers.
	*SSPCR1 = 0;
	*SSPCR0 = 0;
	*SSPCPSR = 0;
	*SSPIMSC = 0;
	*SSPDMACR = 0;
	*SSPICR = RECEIVE_TIMEOUT_CLEAR | RECEIVE_OVERRUN_CLEAR;
}

/****************************************************************************/
/* NAME: 		SSP_Init	            		            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine initializes all SSP with default settings		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_SSPError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_SSPError SSP_Init()
{
	//Both the SSP's are initialized with default settings configured.
	return(SSP_Configure(SSP_ALL, &(SSPCurrentConfig[SSP0])));
}

/****************************************************************************/
/* 			 SSP_Configure()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to configure a SSP Device. It first disables    */
/*      the SSP, then configures it and finally enables it again.           */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: Identify the SSP Device	                            */
/*      - Config: Pointer to Configuration Information.          	    */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_SSPError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_SSPError SSP_Configure(t_SSPId SSPId, t_SSPConfigure *Config)
{
	uint PrescaleDiv = 0, SerialClkRate = 0, CtrlReg0 = 0, CtrlReg1 = 0, SSPCount = 0;
	volatile uint SPEAr_SSP_APB_BASE;
	t_SSPError Status = 0;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId > SSP_ALL))
		return SSP_INVALID_ID;

	//Configuring CtrlReg0**********************************
	if(Config->Mode == SSP_SLAVE)
	{
		CtrlReg1 |= SSP_SLAVE;

		//Setting Slave Mode Output Disable Bit.
		if(Config->SlaveOutput == SLAVE_OUTPUT_DISABLE)
			CtrlReg1 |= SLAVE_OUTPUT_DISABLE;
		else if(Config->SlaveOutput != SLAVE_OUTPUT_ENABLE)
			return SSP_INVALID_SLAVE_OUTPUT;
	}
	else if(Config->Mode != SSP_MASTER)
		return SSP_INVALID_MODE;

	//Setting Interface Type
	if((Config->FrameFormat.Interface < MOTOROLA_SPI) || (Config->FrameFormat.Interface > NATIONAL_MICROWIRE))
		return SSP_INVALID_INTERFACE;
	CtrlReg0 |= Config->FrameFormat.Interface << FRAME_FORMAT_SHIFT;

	//Setting Polarity and Phase for Motorola SPI
	if(Config->FrameFormat.Interface == MOTOROLA_SPI)
	{
		if(Config->FrameFormat.MotoClkPolarity == CLK_POL_HIGH)
			CtrlReg0 |= CLK_POL_HIGH;
		else if(Config->FrameFormat.MotoClkPolarity != CLK_POL_LOW)
			return SSP_INVALID_CLK_POLARITY;

		if(Config->FrameFormat.MotoClkPhase == CLK_RISING_EDGE)
			CtrlReg0 |= CLK_RISING_EDGE;
		else if(Config->FrameFormat.MotoClkPhase != CLK_FALLING_EDGE)
			return SSP_INVALID_CLK_PHASE;
	}

	//Setting Data Size.
	if((Config->DataSize < DATA_SIZE_4) || (Config->DataSize >DATA_SIZE_16))
		return SSP_INVALID_DATA_SIZE;
	CtrlReg0 |= Config->DataSize;

	//Setting SSP Mode.
	//Setting Bit Rate if mode selected is Master mode.
	if((Status = GetFreqAvailable(Config->RequiredFrequency, &PrescaleDiv, &SerialClkRate, &(Config->AttainedFrequency)))!=SSP_OK)
		return Status;

	//Setting Serial Clock Rate.
	CtrlReg0 |= ((SerialClkRate << CLK_RATE_SHIFT) & CLK_RATE_MASK);

	//Checking how many Devices are required to be configured.
	if(SSPId == SSP_ALL)
	{
		SSPCount = 3;
		SSPId = SSP0;
	}
	else
	{
		SSPCount = 1;
		//Reset SSP.
		SSP_Reset(SSPId);
	}

	//While the all the SSP's required to be configured are configured.
	while(SSPCount > 0)
	{
		//Decrementing count.
		SSPCount--;

		//Select Present Device Base Address.
		SELECT_DEVICE;

		//Configuring the Registers.
		//Setting SSPCPSR.
		*SSPCPSR = PrescaleDiv & PRESCALE_MASK;

		//Setting Control Register0.
		*SSPCR0 = CtrlReg0;

		//Setting Control Register1.
		*SSPCR1 = CtrlReg1;

		//Enabling the SSP.
		*SSPCR1 |= SSP_ENABLE;

		//Setting SSPId to the Next SSP in case more than one SSP is required to be configured.
		SSPId += 1;
	}

	return SSP_OK;
}


/****************************************************************************/
/* NAME: 		SSP_TransmitDummyData	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine starts transmititng Dummy Data			    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: identify the SSP Device	                            */
/*      - NumOfCharToTx: This is the number of characters to be transmitted.*/
/*                                                                          */
/* RETURN:                                                                  */
/*        t_SSPError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
inline void SSP_TransmitDummyData(t_SSPId SSPId, uint NumOfCharToTx)
{
	volatile uint SPEAr_SSP_APB_BASE;
	uint NumOfCharTx = 0;

	//Select Present Device Base Address.
	SELECT_DEVICE;

	//Transmit data.
	while(NumOfCharTx < NumOfCharToTx)
	{
		//Send Data if Tx is FIFO is not full.
		if(!SSP_IsTxFIFOFull)
		{
			*SSPDR = (short)DUMMY_DATA;
			NumOfCharTx += 2;
		}
		else
			break;
	}
}

/****************************************************************************/
/* NAME: 		SSP_TransmitMaxChar	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine starts transmititng max number of bytes		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: identify the SSP Device	                            */
/*      - NumOfCharToTx: This is the number of characters to be transmitted.*/
/*                                                                          */
/* OUT  :                                                                   */
/*      - Buffer: This is the pointer to the characters to be transmitted.  */
/*      - NumOfCharTx: This is the number of characters actually transmitted*/
/*                                                                          */
/* RETURN:                                                                  */
/*        t_SSPError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_SSPError SSP_TransmitMaxChar(t_SSPId SSPId, uint NumOfCharToTx, char *Buffer, uint *NumOfCharTx)
{
	volatile uint SPEAr_SSP_APB_BASE;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	//If Number of Characters to be transmitted is zero.
	if(!NumOfCharToTx)
		return SSP_NO_DATA_TO_WRITE;

	//If Pointer Sent is null.
	if((Buffer == NULL) || (NumOfCharTx == NULL))
		return SSP_NULL_POINTER;

	//Resetting Num of Char actually Transmitted.
	*NumOfCharTx = 0;

	//Select Present Device Base Address.
	SELECT_DEVICE;

	//Transmit data.
	while(*NumOfCharTx < NumOfCharToTx)
	{
		//Send Data if Tx is FIFO is not full.
		if(!SSP_IsTxFIFOFull)
		{
			switch(NumOfCharToTx - *NumOfCharTx)
			{
				case 1:
					*SSPDR = *(Buffer + *NumOfCharTx);
					(*NumOfCharTx)++;
					break;
				default:
					*SSPDR = *(short *)(Buffer + *NumOfCharTx);
					(*NumOfCharTx) += sizeof(short);
					break;
			}
		}
		else
			return SSP_OK;
	}
	return SSP_OK;
}

/****************************************************************************/
/* NAME: 		SSP_ReceiveMaxChar	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine starts receiving max number of bytes		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: identify the SSP Device	                            */
/*      - NumOfCharToRx: This is the number of characters to be Received.   */
/*                                                                          */
/* OUT  :                                                                   */
/*      - Buffer: This is the pointer to the buffer where the data is stored*/
/*      - NumOfCharRx: This is the number of characters Received before     */
/*		       error is encountered.   				    */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_SSPError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_SSPError SSP_ReceiveMaxChar(t_SSPId SSPId, uint NumOfCharToRx, char *Buffer, uint *NumOfCharRx)
{
	volatile uint SPEAr_SSP_APB_BASE;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	//If Number of Characters to be transmitted is zero.
	if(!NumOfCharToRx)
		return SSP_NO_DATA_TO_READ;

	//If Pointer Sent is null.
	if((Buffer == NULL) || (NumOfCharRx == NULL))
		return SSP_NULL_POINTER;

	//Resetting Num of Char actually Received.
	*NumOfCharRx = 0;

	//Select Present Device Base Address.
	SELECT_DEVICE;

	//Receive data.
	while(*NumOfCharRx < NumOfCharToRx)
	{
		//Send Data if Rx is FIFO is not empty.
		if(!SSP_IsRxFIFOEmpty)
		{
			switch(NumOfCharToRx - *NumOfCharRx)
			{
				case 1:
					*(Buffer + *NumOfCharRx) = *SSPDR;
					(*NumOfCharRx)++;
					break;
				default:
					*(short *)(Buffer + *NumOfCharRx) = *SSPDR;
					(*NumOfCharRx) += sizeof(short);
					break;
			}
		}
		else
			return SSP_OK;
	}
	return SSP_OK;
}

/****************************************************************************/
/* NAME: 		SSP_FlushRxFIFO		                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine empties the Rx FIFO.				    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: identify the SSP Device	                            */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_SSPError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
inline void SSP_FlushRxFIFO(t_SSPId SSPId)
{
	volatile uint SPEAr_SSP_APB_BASE;
	int Data;

	//Select Present Device Base Address.
	SELECT_DEVICE;

	//Receive data.
	while(!SSP_IsRxFIFOEmpty)
		Data = *SSPDR;
}


/********************************************************************************/
/* NAME:           SSP_DMAEnable						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Enable DMA on the specified SSP.	 			*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - SSPId: identify the SSP Device	                            	*/
/*                                                                          	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_SSPError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_SSPError SSP_DMAEnable(t_SSPId SSPId)
{
	volatile uint SPEAr_SSP_APB_BASE;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	//Select Present Device Base Address.
	SELECT_DEVICE;

	//Enabling DMA
	*SSPDMACR = SSP_DMA_ENABLE;

	return SSP_OK;
}

/********************************************************************************/
/* NAME:           SSP_DMADisable						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Disable DMA on the specified SSP.	 		*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - SSPId: identify the SSP Device	                            	*/
/*                                                                          	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_SSPError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_SSPError SSP_DMADisable(t_SSPId SSPId)
{
	volatile uint SPEAr_SSP_APB_BASE;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	//Select Present Device Base Address.
	SELECT_DEVICE;

	//Disabling DMA
	*SSPDMACR = SSP_DMA_DISABLE;

	return SSP_OK;
}

/********************************************************************************/
/* NAME:           SSP_IntEnable						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Enable Interrupt on the specified SSP.	 		*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - SSPId: identify the SSP Device	                            	*/
/*      - IntType: identify the Interrupt Type. All the interrupts required   	*/
/*      	   to be enabled should be ORed first and then passed into this */
/*      	   as argument							*/
/*                                                                          	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_SSPError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_SSPError SSP_IntEnable(t_SSPId SSPId, t_SSPIntType IntType)
{
	volatile uint SPEAr_SSP_APB_BASE;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	//Checking whether the IntType is correct or not.
	if((IntType < SSP_RX_OR_INT) || (IntType > INT_ALL))
		return SSP_INVALID_INT_TYPE;

	//Select Present Device Base Address.
	SELECT_DEVICE;

	//Enabling Interrupts
	*SSPIMSC = *SSPIMSC | IntType;

	return SSP_OK;
}

/********************************************************************************/
/* NAME:           SSP_IntDisable						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Disable Interrupt on the specified SSP.	 		*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - SSPId: identify the SSP Device	                            	*/
/*      - IntType: identify the Interrupt Type. All the interrupts required   	*/
/*      	   to be disabled should be ORed first and then passed into this*/
/*      	   argument							*/
/*                                                                          	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_SSPError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_SSPError SSP_IntDisable(t_SSPId SSPId, t_SSPIntType IntType)
{
	volatile uint SPEAr_SSP_APB_BASE;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	//Checking whether the IntType is correct or not.
	if((IntType < SSP_RX_OR_INT) || (IntType > INT_ALL))
		return SSP_INVALID_INT_TYPE;

	//Select Present Device Base Address.
	SELECT_DEVICE;

	//Disabling Interrupts
	*SSPIMSC = *SSPIMSC & (~IntType);

	return SSP_OK;
}

/********************************************************************************/
/* NAME:           SSP_IntClear						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Clear Interrupt of the specified SSP.	 		*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - SSPId: identify the SSP Device	                            	*/
/*      - IntType: identify the Interrupt Type. All the interrupts required   	*/
/*      	   to be cleared should be ORed first and then passed into this */
/*      	   argument							*/
/*                                                                          	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_SSPError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_SSPError SSP_IntClear(t_SSPId SSPId, t_SSPIntType IntType)
{
	volatile uint SPEAr_SSP_APB_BASE;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	//Checking whether the IntType is correct or not.
	if((IntType < SSP_RX_OR_INT) || (IntType > INT_ALL))
		return SSP_INVALID_INT_TYPE;

	//Select Present Device Base Address.
	SELECT_DEVICE;

	//Clearing Interrupts
	*SSPICR = *SSPICR | IntType;

	return SSP_OK;
}

/********************************************************************************/
/* NAME:           SSP_GetIntSrc						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Get the source of Interrupt.		 		*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - SSPId: identify the SSP Device	                            	*/
/*                                                                          	*/
/* OUT : 				    					*/
/*									    	*/
/* RETURN: uint representing the interrupts				    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
uint SSP_GetIntSrc(t_SSPId SSPId)
{
	volatile uint SPEAr_SSP_APB_BASE;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	//Select Present Device Base Address.
	SELECT_DEVICE;

	//Return the Active Interrupts.
	return((*SSPMIS) & INT_ALL);
}
