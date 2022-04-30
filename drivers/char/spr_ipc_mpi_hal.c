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

#include "spr_ipc_mpi_hal.h"
#include <linux/kernel.h>
#include <asm/arch/misc_regs.h>
#include <linux/dma-mapping.h>

msgqd msgqd_1_2;
msgqd msgqd_2_1;

msg msg_area_1_2[MAX_Q_1_2];	
msg msg_area_2_1[MAX_Q_1_2];	

msgqd * msgqd_cores[MAX_CORES][MAX_CORES];

t_CoreId my_core_id;
extern unsigned int log_add_ipc_base;

// Array for pending messages from respective cores

void show_msgqd(t_CoreId core_to)
{
	msgqd * msgqd_ptr =  msgqd_cores[my_core_id][core_to];
	SPLUS_IPC_DEBUG("\n\n############################################################################################\n");
	SPLUS_IPC_DEBUG("PRINTING MESSAGE QUEUE DESCRIPOR OF my_core_id = %d and core_to = %d",my_core_id,core_to);
	SPLUS_IPC_DEBUG(" MSG_COUNT  = %d \n, FRONT = %d\n, REAR = %d\n,MSG_COUNT =%d\n MAX_MSG = %d\n,MSGQ_PTR = %x  \n",msgqd_ptr->msg_count,msgqd_ptr->front,msgqd_ptr->rear,msgqd_ptr->msg_count,msgqd_ptr->max_msg,msgqd_ptr->msgq_ptr_base);
	SPLUS_IPC_DEBUG("\n\n############################################################################################\n");
}

/* Number of Messages pending for processesing from Corresponding queue*/
void IPC_MPI_Init(t_CoreId core_id)
{
	my_core_id = core_id;
}

void IPC_MPI_Qd_Init( t_CoreId core_to, t_CoreId core_from,int max_msgs, msg *msg_area,msgqd* msgqd_ptr)
{
	msgqd_cores[core_to][core_from] =  msgqd_ptr;
	msgqd_ptr->msg_count = 0;
	msgqd_ptr->front = 0;
	msgqd_ptr->rear = -1;
	msgqd_ptr->max_msg = max_msgs;
	msgqd_ptr->msgq_ptr_base = msg_area;
}

int IPC_MPI_Send_Msg(t_CoreId core_to, msg * msg_ptr)
{
  	msgqd * msgqd_ptr = msgqd_cores[my_core_id][core_to];
	//In case of sending we need to convert the Physical msgq_ptr_base to Logical address
	msg * msgq_ptr_log;	
	int flag_prev_msg;

	if(msgqd_ptr->msg_count >= msgqd_ptr->max_msg)
	{
		SPLUS_IPC_DEBUG("Queue is full \n");
		return -IPC_MSGQ_FULL;
	}
	if( msg_ptr == NULL)
	{
		SPLUS_IPC_DEBUG("Null Message cannot be added \n");		
		return -IPC_INVALID_PARAMETER;
	}
	else 
	{
		//try to check whether previous message interrupt  has been serviced or not
		flag_prev_msg = Is_IPC_MPI_Prev_Msg_IrqClr(core_to);
		if( flag_prev_msg)
		{
			return 	flag_prev_msg;
		}			
		msgqd_ptr->msg_count++;
		msgqd_ptr->rear  = (msgqd_ptr->rear + 1) % msgqd_ptr->max_msg;
		msgq_ptr_log     = log_add_ipc_base + (unsigned int)msgqd_ptr->msgq_ptr_base - MSGQD_BASE_ARM_1_2;
		( msgq_ptr_log  + msgqd_ptr->rear)->buff_ptr  =   msg_ptr->buff_ptr;		
		( msgq_ptr_log  + msgqd_ptr->rear)->buff_size =   msg_ptr->buff_size;		
		( msgq_ptr_log  + msgqd_ptr->rear)->msg_type  =   msg_ptr->msg_type;
	 	SPLUS_IPC_DEBUG("Message Put at Address %x\n", msgqd_ptr->rear);
	}
	return 0;
}

int IPC_MPI_Recv_Msg(t_CoreId core_from,msg * msg_ptr)
{
	msgqd * msgqd_ptr = msgqd_cores[core_from][my_core_id];
	msg * msg_ptr_log;	
	int flag_prev_ack;
	//Patch for cache coherency for DEMO Purpose , but it has to be removed after ioremap_nocache
//	consistent_sync(msgqd_ptr,sizeof( msgqd),DMA_BIDIRECTIONAL);
	if( core_from == my_core_id)
	{
		SPLUS_IPC_DEBUG("Cannot recieve message from own queue \n");
		return -IPC_INVALID_PARAMETER;
	}
	if( msgqd_ptr->msg_count <= 0 )
	{
		SPLUS_IPC_DEBUG("MSGQ Empty \n");
		return -IPC_MSGQ_EMPTY;
	}
	flag_prev_ack =  Is_IPC_MPI_Prev_Ack_IrqClr(core_from);
	if( flag_prev_ack )
	{
		return flag_prev_ack;
	}
	msg_ptr_log = log_add_ipc_base + (unsigned int)msgqd_ptr->msgq_ptr_base - MSGQD_BASE_ARM_1_2 ;

	//consistent_sync(msg_ptr_log,sizeof( msg),DMA_BIDIRECTIONAL);
	SPLUS_IPC_DEBUG("##########msg_ptr_log######3 = %x",msg_ptr_log);
	msg_ptr->buff_ptr =  ( 	msg_ptr_log   + msgqd_ptr->front)->buff_ptr ;
        msg_ptr->buff_size = (	msg_ptr_log  + msgqd_ptr->front)->buff_size ;
        msg_ptr->msg_type =  ( 	msg_ptr_log + msgqd_ptr->front)->msg_type;

	return 0;
}


int IPC_MPI_Rmv_Msg(t_CoreId core_id)
{	
	msgqd * msgqd_ptr = msgqd_cores[my_core_id][core_id];

	if( msgqd_ptr->msg_count <= 0 )
	{
		SPLUS_IPC_DEBUG("MSGQ Empty \n");
		return -IPC_MSGQ_EMPTY;
	}
	else
	{
		msgqd_ptr->msg_count--;
		msgqd_ptr->front = (msgqd_ptr->front+1)%msgqd_ptr->max_msg;
		SPLUS_IPC_DEBUG("** Deleting Node \n"); 
	}
	show_msgqd(core_id);
	return 0;
}

int IPC_MPI_Get_PendingMsgCount(t_CoreId core_id)
{
	msgqd * msgqd_ptr = msgqd_cores[core_id][my_core_id];
	return msgqd_ptr->msg_count;
}

/* Clear the Interrupt line asserted from core_from */
inline void IPC_MPI_Msg_ClrIrq(t_CoreId core_from)
{
	//Clear the irq line of core_from ,depending upon irq_line
	*PRC1_IRQ_CTR = 0x00010000 ;		
}



/* Interrupt handler for Messages, A Message has come from  core_from */

void IPC_MPI_Msg_IrqHandler(t_CoreId core_from)
{
//	msgqd * msgqd_ptr = msgqd_cores[core_from][my_core_id];
	// Clear the Interrupt in Misc Register
	// Invoke the process that is waiting for a message on this port
	IPC_MPI_Msg_ClrIrq(core_from);
	SPLUS_IPC_DEBUG("Inside IPC_MPI_Msg_IrqHand \n");

}

/* Clear the Interrupt line asserted from core_from */

inline void IPC_MPI_Ack_ClrIrq(t_CoreId core_from)
{
	//Clear the irq line of core_from ,depending upon irq_line
	*PRC1_IRQ_CTR = 0x00020000 ;		
}

/* Interrupt handler for Acknowledgements line, a interrupt is recieved from core_from*/ 
void IPC_MPI_Ack_IrqHandler(t_CoreId core_from)
{
	//Inside Ack _Irq Handler
	SPLUS_IPC_DEBUG("Inside Ack Irq Handler\n");
	//delete the node from corresponding queue
	IPC_MPI_Rmv_Msg(core_from);
	//Clear the Interrupt in Misc Register
	IPC_MPI_Ack_ClrIrq(core_from);
}

/* Generate a Interrupt to core_to core */
inline void IPC_MPI_Ack_AssertIrq(t_CoreId core_to)
{
	//Raise an irq_line line to core_to 	
	*PRC1_IRQ_CTR = 0x00000002 ;		
}

/* Generate a Interrupt to core_to core */
inline void IPC_MPI_Msg_AssertIrq(t_CoreId core_to)
{
	//Raise an irq_line line to core_to 	
	*PRC1_IRQ_CTR = 0x00000001 ;		
}

/* Check whether previous generated msg irq line is cleared*/

int Is_IPC_MPI_Prev_Msg_IrqClr(t_CoreId core_to)
{
	volatile unsigned int msg_time_out = 0;
	volatile unsigned int prc1_irq_ctr;

	prc1_irq_ctr = *PRC1_IRQ_CTR;
	//Misc register
	while((( prc1_irq_ctr & 0x00000001) != 0x00000000) && msg_time_out != MAX_PREV_MSGIRQ_TIMEOUT   )
	{
		prc1_irq_ctr = *PRC1_IRQ_CTR;
		msg_time_out++;
	}
	
	if(msg_time_out == MAX_PREV_MSGIRQ_TIMEOUT )
	{
		SPLUS_IPC_DEBUG("Previous Interrupt not serviced \n ThreadX might be down or Busy \n");
		return -IPC_MSG_TIMEOUT;
	}
	return 0;
}

int Is_IPC_MPI_Prev_Ack_IrqClr(t_CoreId core_from)
{
	unsigned int ack_time_out = 0;
	volatile unsigned int prc1_irq_ctr;

	prc1_irq_ctr = *PRC1_IRQ_CTR;
	//Misc register
	while((( prc1_irq_ctr & 0x00000002) != 0x00000000) && ack_time_out != MAX_PREV_ACKIRQ_TIMEOUT  )
	{
		prc1_irq_ctr = *PRC1_IRQ_CTR;
		ack_time_out++;
	}
	
	if( ack_time_out != MAX_PREV_ACKIRQ_TIMEOUT )
	{
		return 0;
	}
	SPLUS_IPC_DEBUG("Previous Ack Interrupt not serviced \n ThreadX might be down or Busy\n");
	return -IPC_ACK_TIMEOUT;
}


t_CoreId IPC_MPI_Get_CoreId(int irq)
{
	t_CoreId core_from = -1;
	switch(irq)
	{
		case 1 : case 2:
			 if(my_core_id == ARM1)
				core_from = ARM2;		
			 else
				core_from = ARM1;
			 break;
		case 3 : case 4:
			 core_from = RAS1;
			 break;
		case 5 : case 6:
			 core_from = RAS2;
			 break;		
	}
	return core_from;
}

#if 0

int IPC_MPI_test()
{
	msg temp_msg;
	int i,temp;
	IPC_MPI_Init(MY_CORE_ID);
	IPC_MPI_Qd_Init(MY_CORE_ID,ARM1,MAX_Q_1_2,(msg *)MSGQ_BASE_ARM_1_2,MSGQD_BASE_ARM_1_2);
	IPC_MPI_Qd_Init(ARM1,MY_CORE_ID,MAX_Q_1_2,(msg *)MSGQ_BASE_ARM_2_1,MSGQD_BASE_ARM_2_1);
	for(i=0;i<30;i++)
	{
		temp_msg.msg_type = i*10;
		SPLUS_IPC_DEBUG("Sending Message Number = %d \n");
		SPLUS_IPC_DEBUG(" msg.buff_ptr = %d  msg.buff_size = %d msg\n ",temp_msg.buff_ptr,temp_msg.buff_size,temp_msg.msg_type);
		IPC_MPI_Send_Msg(ARM2,&temp_msg);	
	}
}


int main(int argc, char* argv[])
{
	

	msg temp_msg;
	int i,temp;

	IPC_MPI_Init(MY_CORE_ID);
	IPC_MPI_Qd_Init(MY_CORE_ID,ARM2,MAX_Q_1_2,(msg *)MSGQ_BASE_ARM_1_2,MSGQD_BASE_ARM_1_2);
	IPC_MPI_Qd_Init(ARM2,MY_CORE_ID,MAX_Q_1_2,(msg *)MSGQ_BASE_ARM_2_1,MSGQD_BASE_ARM_2_1);

	SPLUS_IPC_DEBUG("Sending 30 messages to ARM2 from ARM1 \n");

	for(i=0;i<30;i++)
	{
		temp_msg.msg_type = i*10;
		SPLUS_IPC_DEBUG(" msg.buff_ptr = %d  msg.buff_size = %d msg\n ",temp_msg.buff_ptr,temp_msg.buff_size,temp_msg.msg_type);
		IPC_MPI_Send_Msg(ARM2,&temp_msg);	
	}

	SPLUS_IPC_DEBUG("Number of elements in list = %d \n",IPC_MPI_GetMsgCount(ARM2));
	SPLUS_IPC_DEBUG("Removing 7 messages from ARM1_2 \n");

	for(i=0;i<7;i++)
	{
		temp = IPC_MPI_Rmv_Msg(ARM2);		
		SPLUS_IPC_DEBUG("Number of elements in list = %d \n",IPC_MPI_GetMsgCount(ARM2));
		/*if(!temp)
		{
			SPLUS_IPC_DEBUG(" msg_area %d  .buff_ptr = %d, buff_size = %d, msg_type =%d \n \n ",i,temp_msg.buff_ptr,temp_msg.buff_size,temp_msg.msg_type);
		}*/
		
	}		
	SPLUS_IPC_DEBUG("Number of elements in list = %d \n",IPC_MPI_GetMsgCount(ARM2));

	SPLUS_IPC_DEBUG("Sending 2 messages to ARM2 from ARM1 \n");

	for(i=0;i<2;i++)
	{
		temp_msg.msg_type = i*10;
		SPLUS_IPC_DEBUG(" msg.buff_ptr = %d  msg.buff_size = %d msg\n ",temp_msg.buff_ptr,temp_msg.buff_size,temp_msg.msg_type);
		IPC_MPI_Send_Msg(ARM2,&temp_msg);	
	}

	SPLUS_IPC_DEBUG("Number of elements in list = %d \n",IPC_MPI_GetMsgCount(ARM2));

	SPLUS_IPC_DEBUG("Removing 7 messages from ARM1_2 \n");

	for(i=0;i<7;i++)
	{
		temp = IPC_MPI_Rmv_Msg(ARM2);		
		SPLUS_IPC_DEBUG("Number of elements in list = %d \n",IPC_MPI_GetMsgCount(ARM2));
		/*if(!temp)
		{
			SPLUS_IPC_DEBUG(" msg_area %d  .buff_ptr = %d, buff_size = %d, msg_type =%d \n \n ",i,temp_msg.buff_ptr,temp_msg.buff_size,temp_msg.msg_type);
		}*/
		
	}		

	SPLUS_IPC_DEBUG("Sending 2 messages to ARM2 from ARM1 \n");

	for(i=0;i<2;i++)
	{
		temp_msg.msg_type = i*10;
		SPLUS_IPC_DEBUG(" msg.buff_ptr = %d  msg.buff_size = %d msg\n ",temp_msg.buff_ptr,temp_msg.buff_size,temp_msg.msg_type);
		IPC_MPI_Send_Msg(ARM2,&temp_msg);	
	}

	SPLUS_IPC_DEBUG("Number of elements in list = %d \n",IPC_MPI_GetMsgCount(ARM2));



	SPLUS_IPC_DEBUG("Sending 1 messages to ARM2 from ARM1 \n");

	for(i=0;i<1;i++)
	{
		temp_msg.msg_type = i*10;
		SPLUS_IPC_DEBUG(" msg.buff_ptr = %d  msg.buff_size = %d msg\n ",temp_msg.buff_ptr,temp_msg.buff_size,temp_msg.msg_type);
		IPC_MPI_Send_Msg(ARM2,&temp_msg);	
	}

	SPLUS_IPC_DEBUG("Number of elements in list = %d \n",IPC_MPI_GetMsgCount(ARM2));

	SPLUS_IPC_DEBUG("Removing 40 messages from ARM1_2 \n");

	for(i=0;i<40;i++)
	{
		temp = IPC_MPI_Rmv_Msg(ARM2);		
		SPLUS_IPC_DEBUG("Number of elements in list = %d \n",IPC_MPI_GetMsgCount(ARM2));
		/*if(!temp)
		{
			SPLUS_IPC_DEBUG(" msg_area %d  .buff_ptr = %d, buff_size = %d, msg_type =%d \n \n ",i,temp_msg.buff_ptr,temp_msg.buff_size,temp_msg.msg_type);
		}*/
		
	}		


	for(i=0;i<15;i++)
	{
		temp_msg.msg_type = i*10;
		IPC_MPI_Send_Msg(ARM1,&temp_msg);	
	}
	
	
	for(i=0;i<50;i++)
	{
		temp = IPC_MPI_Recv_Msg(ARM1,&temp_msg);
		if(!temp)
		{
			SPLUS_IPC_DEBUG(" msg_area %d  .buff_ptr = %d, buff_size = %d, msg_type =%d \n \n ",i,temp_msg.buff_ptr,temp_msg.buff_size,temp_msg.msg_type);
		}
		IPC_MPI_Rmv_Msg(ARM2);
		
	}
	SPLUS_IPC_DEBUG("Number of elements in list = %d \n",msgqd_1_2.msg_count);

	return 0;
}
#endif

