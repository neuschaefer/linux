/********************************************************************************/
/*	 Copyright (C) STMicroelectronics 2008 by Deepak Sikri						*/
/*	 deepak.sikri@st.com														*/
/*																				*/
/*	 This program is free software;	you	can	redistribute it	and/or modify		*/
/*	 it	under the terms	of the GNU General Public License as published by		*/
/*	 the Free Software Foundation; either version 2	of the License,	or			*/
/*	 (at your option) any later	version.										*/
/*																				*/
/*	 This program is distributed in	the	hope that it will be useful,			*/
/*	 but WITHOUT ANY WARRANTY; without even	the	implied	warranty of				*/
/*	 MERCHANTABILITY or	FITNESS	FOR	A PARTICULAR PURPOSE.  See the				*/
/*	 GNU General Public	License	for	more details.								*/
/*																				*/
/*	 You should	have received a	copy of	the	GNU	General	Public License			*/
/*	 along with	this program; if not, write	to the								*/
/*	 Free Software Foundation, Inc.,											*/
/*	 59	Temple Place - Suite 330, Boston, MA  02111-1307, USA.					*/
/********************************************************************************/
#ifndef SPEAR_LOCK_HAL_H
#define SPEAR_LOCK_HAL_H

#include <linux/types.h>
#include <asm/arch-spearbasic/hardware.h>
#include <asm/arch-spearbasic/misc_regs.h>
#include <asm/arch-spearbasic/hcl_defs.h>

//Registers Base Address Mapping
#define PRC_LOCK_CTR  	(PRC1_LOCK_CTR + CoreId)

/******************************************************************************/
/* Types definitions */
/******************************************************************************/
//Max Number of Locks for a processor.
#define MAX_LOCKS 6

//Constants for configuring WDT Registers
typedef enum
{
	LOCK_RESET_SHIFT	=	4,
	LOCK_STATUS_MASK	=	0x10000	//i.e. 1<<15 to point to one bit left of base of the status bits.
}t_HWLockReg;

//Typedef for HWLockId
typedef enum
{	
	LOCK1 = 1,
	LOCK2,
	LOCK3,
	LOCK4,
	LOCK5,
	LOCK6,
	LOCK7,
	LOCK8,
	LOCK9,
	LOCK10,
	LOCK11,
	LOCK12,
	LOCK13,
	LOCK14,
	LOCK15
}t_HWLockId;

//Return values of the API's
typedef enum
{
	HWLOCK_OK,
	HWLOCK_INVALID_CORE_ID,
	HWLOCK_INVALID_LOCK_ID,
	HWLOCK_ALREADY_LOCKED
}t_HWLockError;

/***********************************************************************************
* PUBLIC Functions
***********************************************************************************/
//This API requests the hardware lock.
t_HWLockError HWLock_Request(t_CoreId CoreId, t_HWLockId LockId);

//This API releases a hardware lock.
t_HWLockError HWLock_Release(t_CoreId CoreId, t_HWLockId LockId);
#endif // SPEAR_LOCK_HAL_H
