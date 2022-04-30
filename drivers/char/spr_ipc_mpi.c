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

#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/arch/irqs.h>
#include <linux/socket.h>
#include <linux/net.h>
#include "spr_ipc_mpi.h"
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/arch/spr_major.h>
#include <../arch/arm/mach-spearplus/spr_hw_lock.h>

#define IPC_MSG_IRQ_NAME "IPC_MSG"
#define IPC_ACK_IRQ_NAME "IPC_ACK"

#define SPEAR_IPC_NAME   "arm2"
#define SPEAR_IPC_MAJOR  MAJOR_IPC
extern t_CoreId my_core_id;

t_HWLockId LockId_IPC = LOCK4;

static DECLARE_WAIT_QUEUE_HEAD(wq_msg_arrive);
static int flag_arrived = 0;

extern msg msg_area[MAX_Q_1_2];
extern msgqd msgqd_1_2;

msg msg_curr_arrived;

spinlock_t IPC_Send_lock = SPIN_LOCK_UNLOCKED;
spinlock_t IPC_Recv_lock = SPIN_LOCK_UNLOCKED;

unsigned int log_add_ipc_base;

extern struct splus_srv_sock_id splus_srv_sock_list;
extern  spinlock_t IPC_Sleep_lock;

void SPEAR_IPC_MPI_Msg_AssertIrq(t_CoreId core_to)
{
	//Raise the message line of the core_to
	IPC_MPI_Msg_AssertIrq(core_to);
}

int SPEAR_IPC_MPI_Send_Msg( t_CoreId core_to, msg * msg_ptr)
{
	int ret;
	unsigned long flags;
	// Check whether previous raised message line from my_core to core_id has been cleared
	spin_lock_irqsave(&IPC_Send_lock,flags);			
	ret = IPC_MPI_Send_Msg(core_to,msg_ptr);	
	//inform the correspdige core, for message additon 
	SPEAR_IPC_MPI_Msg_AssertIrq(core_to);
	spin_unlock_irqrestore(&IPC_Send_lock,flags);
	return ret;
}

int SPEAR_IPC_MPI_Recv_Msg( t_CoreId core_id,msg * msg_ptr)
{
	int ret;
	unsigned long flags;
	spin_lock_irqsave(&IPC_Recv_lock,flags);		
	ret = IPC_MPI_Recv_Msg(core_id,msg_ptr);
	// Convert the physical buff pointer to logical address
	if(!ret)
	{
		SPEAR_IPC_MPI_Ack_AssertIrq(core_id);
	}
	spin_unlock_irqrestore(&IPC_Recv_lock,flags);	
	return ret;
}

int SPEAR_IPC_MPI_Rmv_Msg( t_CoreId core_id)
{	
	int ret;
	ret = IPC_MPI_Rmv_Msg(core_id);
	return ret;
}

int SPEAR_IPC_MPI_Get_PendingMsgCount( t_CoreId core_id)
{
	int ret;
	ret = IPC_MPI_Get_PendingMsgCount(core_id);	
	return ret;
}



inline void SPEAR_IPC_MPI_Ack_ClrIrq(t_CoreId core_from)
{
	IPC_MPI_Ack_ClrIrq(core_from);	
}

void SPEAR_IPC_MPI_Ack_AssertIrq(t_CoreId core_to)
{
	IPC_MPI_Ack_AssertIrq(core_to);
}

inline void SPEAR_IPC_MPI_Msg_ClrIrq(t_CoreId core_from)
{
	IPC_MPI_Msg_ClrIrq(core_from);
}

/* Interrupt handler for Messages, A Message has come from  core_from */

static irqreturn_t SPEAR_IPC_MPI_Msg_IrqHandler(int irq, void *dev_id, struct pt_regs *regs)
{
	t_CoreId core_from;
	unsigned long flag;
	core_from = IPC_MPI_Get_CoreId( irq );
	spin_lock_irqsave(&IPC_Sleep_lock,flag);	
	flag_arrived = 1;
	wake_up_interruptible(&wq_msg_arrive);
	spin_unlock_irqrestore(&IPC_Sleep_lock,flag);
	IPC_MPI_Msg_IrqHandler(core_from);
	return IRQ_HANDLED;
}


/* Interrupt handler for Acknowledgements line, a interrupt is recieved from core_from*/ 
static irqreturn_t SPEAR_IPC_MPI_Ack_IrqHandler(int irq, void *dev_id, struct pt_regs *regs)
{
	t_CoreId core_from;
	core_from = IPC_MPI_Get_CoreId(irq);
	//core_from has processed the message
	IPC_MPI_Ack_IrqHandler(core_from);
	return IRQ_HANDLED;
}

//Message should be Non-Blocking
static int SPEAR_IPC_Write(struct file *file, const char __user *Buf, size_t len, loff_t *ppos) 
{
	//Message needs to be sent to corresponding core
	msg msg_to_queue;
	int err;
	copy_from_user(&msg_to_queue,(msg *)Buf,sizeof(msg));
	err = SPEAR_IPC_MPI_Send_Msg(ARM2,&msg_to_queue);
	if( err )
	{
		return err;
	}
	return (sizeof(msg));
}

static int SPEAR_IPC_Read(struct file *file, char __user *Buf, size_t len, loff_t *ppos)
{
	msg msg_recv;
	volatile int Pending_msgs;
	int err,flag;
	// Check if there are pending messages in queue from ARM2
	Pending_msgs = SPEAR_IPC_MPI_Get_PendingMsgCount(ARM2);


	if( Pending_msgs != 0) 
	{
		//Message is present in queue
		err = SPEAR_IPC_MPI_Recv_Msg(ARM2,&msg_recv);
		if( err)
		{
			SPLUS_IPC_DEBUG("Recv Message failed from ARM2 = %d \n",err);
			return err;
		}
	}
	else
	{
		// if user has passed NON blocking option the return with error ,else sleep untill a message arrives
		if (file->f_flags & O_NONBLOCK)
			return -EAGAIN;
		// goto sleep
		wait_event_interruptible(wq_msg_arrive,(flag_arrived != 0));
		// Check whether it is a Normal wakeup ??
		if( flag_arrived == 1)
		{
			//Message has arrived
			spin_lock_irqsave(&IPC_Sleep_lock,flag);
			flag_arrived = 	0;
		 	err = SPEAR_IPC_MPI_Recv_Msg(ARM2,&msg_recv);
			if( err )
			{
				printk(":Recv Message failed from ARM2 = %d \n",err);
				spin_unlock_irqrestore(&IPC_Sleep_lock,flag);
				return err;
			}
			spin_unlock_irqrestore(&IPC_Sleep_lock,flag);
		}
		else
		{
			return -1;
		}
	
	}
	// Copy Message to user space
	copy_to_user((char *)Buf,&msg_recv,sizeof(msg));	
	return (sizeof(msg));
}


static int SPEAR_IPC_IOCTL(struct inode *inode, struct file *file, uint cmd, unsigned long Buf)
{
}

static int SPEAR_IPC_Open(struct inode *inode, struct file *file) 
{
	return 0;

}

static int SPEAR_IPC_Release(struct inode *inode, struct file *file) 
{
	return 0;
}



//File oprations for SPEAR_IPC character device - only for sending/msgs from ARM2
static struct file_operations SPEAR_IPC_Fops = 
{
	.owner = THIS_MODULE,
	.write = SPEAR_IPC_Write,
	.read  = SPEAR_IPC_Read,
	.ioctl = SPEAR_IPC_IOCTL,
	.open  = SPEAR_IPC_Open,
	.release = SPEAR_IPC_Release
};


static int __exit SPEAR_IPC_MPI_Exit()
{
	t_HWLockError Status;
	//unmap the are returned by ioremap
	iounmap((void *)log_add_ipc_base);	
	//Release the HWlock
	Status = HWLock_Release(my_core_id, LockId_IPC);
	if(Status)
	{
		printk("Error in Releasing Lock %d.\n",LockId_IPC);
	}
	//unregister the irq's
	free_irq(IRQ_INTRCOMM_CPU_1,NULL);
	free_irq(IRQ_INTRCOMM_CPU_2,NULL);		
	//unregister the character device	
	unregister_chrdev(SPEAR_IPC_MAJOR,SPEAR_IPC_NAME);
	return 0;
}



int __init SPEAR_IPC_MPI_Init( t_CoreId my_core_id)
{
	int ret;
	unsigned int linux_ram_size,total_ram_size,remaining_ram_size;
	t_HWLockError Status;
	
	my_core_id = CONFIG_MY_CORE_ID;

	#if 0
	linux_ram_size = (unsigned int )__pa(high_memory);
	total_ram_size = SPEARPLUS_SIZE_SDRAM;
	remaining_ram_size = total_ram_size - linux_ram_size -1;

	//Create the MMU table entry for remaining portion of RAM after Linux is using for e.g. 32-63MB
	log_add_ipc_base = (unsigned int ) ioremap_nocache(MSGQD_BASE_ARM_1_2,remaining_ram_size);

	#endif

	log_add_ipc_base = (unsigned int ) ioremap_nocache(MSGQD_BASE_ARM_1_2,IPC_MEMORY_SIZE);
	
	if( log_add_ipc_base == NULL)
	{

		printk("Unable to ioremap IPC and ThreadX Area linux_ram_use = %x \n total_ram_size = %x\n",linux_ram_size,total_ram_size);
		return -1;
	}

	IPC_MPI_Init(my_core_id);
	// Acquire the HW Lock for Initialzing the Message QD, because only 1 Processor should be responsible for Initialzing it in 
	// shared memory
	Status = HWLock_Request(my_core_id, LockId_IPC);

	if(!Status)
	{
		IPC_MPI_Qd_Init(my_core_id,ARM2,MAX_Q_1_2, MSGQ_ARM_1_2 ,log_add_ipc_base);
		IPC_MPI_Qd_Init(ARM2,my_core_id,MAX_Q_1_2, MSGQ_ARM_2_1 ,log_add_ipc_base + MSGQD_ARM_2_1 - MSGQD_BASE_ARM_1_2 );
	}
	else
	{
		SPLUS_IPC_DEBUG("Falied to Acquire IPC Lock ");
	}
	//register the Msg and Ack Interrupts
	ret = request_irq(IRQ_INTRCOMM_CPU_1, (void *)SPEAR_IPC_MPI_Msg_IrqHandler,SA_INTERRUPT,IPC_MSG_IRQ_NAME,NULL);
	if(ret)
	{
		SPLUS_IPC_DEBUG("Unable to Register Interrupt Number = %d\n",IRQ_INTRCOMM_CPU_1);
		return ret;
	}
	ret = request_irq(IRQ_INTRCOMM_CPU_2, (void *)SPEAR_IPC_MPI_Ack_IrqHandler,SA_INTERRUPT,IPC_ACK_IRQ_NAME,NULL);
	if(ret)
	{
		SPLUS_IPC_DEBUG("Unable to Register Interrupt Number = %d\n",IRQ_INTRCOMM_CPU_1);
		return ret;
	}

	//register ipc as a character device
	ret = register_chrdev(SPEAR_IPC_MAJOR,SPEAR_IPC_NAME,&SPEAR_IPC_Fops);
	if( ret < 0 )
	{
		printk(KERN_ERR SPEAR_IPC_NAME":Unable to register character device\n");
		return ret;
	}

//	SPEAR_IPC_MPI_test();
	return 0;
}

module_init(SPEAR_IPC_MPI_Init);
module_exit(SPEAR_IPC_MPI_Exit);

#if 0

int SPEAR_IPC_MPI_test()
{
	msg temp_msg;
	int i,temp;
	SPEAR_IPC_MPI_Recv_Msg(ARM2,&temp_msg);	
//	IPC_MPI_Msg_IrqHandler(ARM1);
	for(i=0;i<30;i++)
	{
		temp_msg.msg_type = i*10;
		temp_msg.buff_ptr = 0xc2000320;
		temp_msg.buff_size = 10;
		temp_msg.port_num  = 0xCCCCCCCC;	
		SPLUS_IPC_DEBUG("Sending Message Number = %d \n");
		SPLUS_IPC_DEBUG(" msg.buff_ptr = %d  msg.buff_size = %d msg\n ",temp_msg.buff_ptr,temp_msg.buff_size,temp_msg.msg_type);
		SPEAR_IPC_MPI_Send_Msg(ARM2,&temp_msg);	
	}
}
#endif
