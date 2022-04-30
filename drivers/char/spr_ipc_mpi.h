/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2006 by Ashwini Pahuja  			*/
/*   ashwini.pahuja@st.com							*/
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
#ifndef SPLUS_IPC_MPI_H
#define SPLUS_IPC_MPI_H

#include "spr_ipc_mpi_hal.h"
#include<linux/module.h>
#include<asm/arch/hardware.h>

#define MEGABYTE_1 1024*1024

int SPEAR_IPC_MPI_Init(t_CoreId);
int SPEAR_IPC_MPI_Send_Msg(t_CoreId, msg * );
int SPEAR_IPC_MPI_Recv_Msg(t_CoreId,msg *);
int SPEAR_IPC_MPI_Rmv_Msg(t_CoreId);
int SPEAR_IPC_MPI_Get_PendingMsgCount(t_CoreId);
static int SPEAR_IPC_MPI_Exit(void);
void SPEAR_IPC_MPI_Ack_AssertIrq(t_CoreId core_to);
#endif
