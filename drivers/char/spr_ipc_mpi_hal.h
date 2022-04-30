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


#ifndef SPLUS_IPC_MPI_HAL_H
#define SPLUS_IPC_MPI_HAL_H

#include <asm/arch/hcl_defs.h>
#include "spr_ipc_if.h"
#include <asm/arch/hardware.h>

#define MAX_Q_1_2 30
#define MAX_CORES 2
#define MAX_PORTS 5

//#define SPLUS_IPC_DEBUG(x...) printk(x)
#define SPLUS_IPC_DEBUG(x...) ;

typedef enum irq_line
{
	MSG = 0,
	ACK
}t_irq_line;

typedef struct msg_node
{
	unsigned int msg_type;
	unsigned int * buff_ptr;
	unsigned int buff_size;
}msg;


typedef struct msg_qd
{
	unsigned int front;
	unsigned int rear;
	unsigned int msgq_id; 
	unsigned int msg_count;
	unsigned int max_msg;
	msg * msgq_ptr_base;
}msgqd;

//Error codes for IPC

//Define the type used to describe the possible errors of that can occurin.
typedef enum
{
	IPC_OK = 0,
	IPC_MSG_TIMEOUT,
	IPC_ACK_TIMEOUT,
	IPC_MSGQ_FULL,
	IPC_INVALID_PARAMETER,
	IPC_MSGQ_EMPTY
}t_IPCError;

void IPC_MPI_Init(t_CoreId);
void IPC_MPI_Qd_Init(t_CoreId,t_CoreId,int, msg *,msgqd*);
int  IPC_MPI_Send_Msg(t_CoreId core_id, msg * msg_ptr);
int  IPC_MPI_Recv_Msg(t_CoreId core_id,msg * msg_ptr);
int  IPC_MPI_Rmv_Msg(t_CoreId core_id);
int  IPC_MPI_Get_PendingMsgCount(t_CoreId core_id);
int Is_IPC_MPI_Prev_Msg_IrqClr(t_CoreId core_to);
int Is_IPC_MPI_Prev_Ack_IrqClr(t_CoreId core_from);
void IPC_MPI_Msg_ClrIrq(t_CoreId);
void IPC_MPI_Ack_ClrIrq(t_CoreId);
int  IPC_MPI_test(void);
void IPC_MPI_Msg_AssertIrq(t_CoreId core_to);
void IPC_MPI_Ack_AssertIrq(t_CoreId core_to);
void IPC_MPI_Ack_IrqHandler(t_CoreId);
void IPC_MPI_Msg_IrqHandler(t_CoreId);
t_CoreId IPC_MPI_Get_CoreId( int irq );
extern msgqd msgqd_1_2;
extern msgqd msgqd_2_1;

extern msg msg_area_1_2[MAX_Q_1_2];	
extern msg msg_area_2_1[MAX_Q_1_2];	

#endif
