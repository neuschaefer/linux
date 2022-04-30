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
#include <linux/module.h>
#include "asm/arch/spr_misc_config.h"

/****************************************************************************/
/* 			 Misc_DMAConfigScheme()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This API Configures a DMA Request Line/Channel for a particular     */
/*	assignment scheme.  						    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - DMAScheme: Id of the Requested Scheme                             */
/*      - RequestId: Id of the Request Line	                            */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_HWLockError                                                     */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_MiscError Misc_DMAConfigScheme(t_MiscDMAScheme DMAScheme, t_DMARequestId RequestId)
{
	//Checks DMAScheme is valid or invalid.
	if((DMAScheme < DMA_SCH_O)||(DMAScheme >= DMA_SCH_NONE))
		return MISC_INVALID_DMA_SCHEME;

	//Checks RequestId is valid or invalid.
	if(RequestId > REQUEST_NONE)
		return MISC_INVALID_REQUEST_ID;

	//Configures a DMA Request Line.
	*DMA_CHN_CFG = *DMA_CHN_CFG | (DMAScheme<<(RequestId * DMA_CHN_CFG_SHIFT_FACTOR));

	return MISC_OK;
}

EXPORT_SYMBOL(Misc_DMAConfigScheme);
