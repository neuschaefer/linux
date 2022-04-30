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
#ifndef SPEAR_JPEG_H
#define SPEAR_JPEG_H

#include "sprplus_jpeg_syn_hal.h"

/***********************************************************************************
 * PUBLIC Functions
 ***********************************************************************************/

/*This API is used to enable JPEG interrupt.*/
static int __init SPEAR_JPEG_Init(void);
static void __exit SPEAR_JPEG_Exit(void);

/*JPEG Interrupt Handler.*/
static irqreturn_t SPEAR_JPEG_IntHandler(int Irq, void *DeviceId, struct pt_regs *Reg);
#endif /* SPEAR_JPEG_H*/
