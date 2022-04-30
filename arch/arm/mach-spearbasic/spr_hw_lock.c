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

/*------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------*/
#include "spr_hw_lock.h"

/*------------------------------------------------------------------------
 * Global Variables 
 *----------------------------------------------------------------------*/

/****************************************************************************/
/* 			 HWLock_Request()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This API requests the hardware lock.				    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - LockId: Id of the Requested Lock	                            */
/*      - CoreId: Id of the Requested Proccessor                            */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_HWLockError                                                     */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_HWLockError HWLock_Request(t_CoreId CoreId, t_HWLockId LockId)
{
	//Checks CoreId is valid or invalid.
	if((CoreId < ARM1)||(CoreId > RAS2))
		return HWLOCK_INVALID_CORE_ID;

	//Checks LockId is valid or invalid.
	if((LockId < LOCK1)||(LockId > MAX_LOCKS))
		return HWLOCK_INVALID_LOCK_ID;

	//Locks requested Lock.
	*PRC_LOCK_CTR = *PRC_LOCK_CTR | LockId;

	//Checks that Lock is Locked or Not.
	if((*PRC_LOCK_CTR) & (LOCK_STATUS_MASK << LockId))
		return HWLOCK_OK;
	else
	{
		return HWLOCK_ALREADY_LOCKED;
	}
}

/****************************************************************************/
/* 			 HWLock_Release()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This API releases the hardware lock.				    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - LockId: Id of the Requested Lock	                            */
/*      - CoreId: Id of the Requested Proccessor                            */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_HWLockError                                                     */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_HWLockError HWLock_Release(t_CoreId CoreId, t_HWLockId LockId)
{
	//Checks CoreId is valid or invalid.
	if((CoreId < ARM1)||(CoreId > RAS2))
		return HWLOCK_INVALID_CORE_ID;

	//Checks LockId is valid or invalid.
	if((LockId < LOCK1)||(LockId > MAX_LOCKS))
		return HWLOCK_INVALID_LOCK_ID;

	//Locks requested Lock.
	*PRC_LOCK_CTR = *PRC_LOCK_CTR | (LockId << LOCK_RESET_SHIFT);

	return HWLOCK_OK;
}
