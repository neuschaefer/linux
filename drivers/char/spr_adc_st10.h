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
#ifndef SPEAR_ADC_H
#define SPEAR_ADC_H

#include "spr_adc_st10_hal.h"


#define SPEAR_ADC_BASE 'A'
#define ADCIOC_CONFIG _IOWR(SPEAR_ADC_BASE,1,t_ADCConfig)

//Define the type used for storing current status of a ADC Device.
typedef struct
{
	void *CurrentOwner;
	uint DeviceNumber;
	t_bool IsConfigured;
	spinlock_t ADCSpinLock;	//To Take a lock when Current User is to be changed or verified.
	struct cdev Cdev;
	int FlagArrived;	//Related to WaitQueue.
	wait_queue_head_t WaitQueue;	//For Waiting in a system call.
}t_ADCStatus;

/***********************************************************************************
 * PUBLIC Functions
 ***********************************************************************************/
//This API is used to initialize the ADC.
static int __init SPEAR_ADC_Init(void);
static void __exit SPEAR_ADC_Exit(void);

//This API is used to Acquire a ADC Device.
t_ADCError SPEAR_ADC_Acquire(void *DeviceId);

//This API is used to Release a ADC Device.
t_ADCError SPEAR_ADC_Free(void *DeviceId);

//This API is used to Configure ADC.
t_ADCError SPEAR_ADC_Configure(t_ADCConfig Config, void *DeviceId);

//This API is used to Receive a number of characters.
t_ADCError SPEAR_ADC_Convert(uint *DigitalVolt, void *DeviceId);

//Interrupts handler.
static irqreturn_t SPEAR_ADC_IntHandler(int Irq, void *DeviceId, struct pt_regs *Reg);

#endif // SPEAR_ADC_H
