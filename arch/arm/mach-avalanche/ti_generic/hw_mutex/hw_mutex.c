/*
 * kernel/hw_mutex.c
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2011-2014 Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */
 
#include <linux/interrupt.h>
#include <linux/init.h>

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <asm-generic/bug.h>

#include <hw_mutex_ctrl.h> /* API declarations */
#include "hw_mutex.h"      /* HW Mutex Internal declarations */


struct hw_master *hw_master_glob;

#define to_hw_mutex_ops() (hw_master_glob->ops)
#define to_hw_mutex(mutex) (&hw_master_glob->hw_mutexes[mutex])
static DEFINE_MUTEX(mmc_access_lock);

/* hw_mutex_is_locked - check whether the current master owns the mutex or not
 * @mutex: the mutex number to be checked
 *
 * Return 1, if current master owns the mutex
 * Return 0, if not
 * Return Negative for errors
*/
int hw_mutex_is_locked(uint8_t mutex)
{
	struct hw_mutex  * hmutex = NULL;
	struct hw_mutex_operations *hmutex_ops = to_hw_mutex_ops();
	DEBUG_PRINTK("func %s mutex number: %x\n", __FUNCTION__,mutex);
	if (WARN_ON(mutex >= HW_MUTEX_TOTAL)) return -EINVAL;
	hmutex = to_hw_mutex(mutex);
	
	return hmutex_ops->is_locked(hmutex);
}

EXPORT_SYMBOL(hw_mutex_is_locked);

/*
 * hw_mutex_isr - interrupt handler
 * 
 *  Sequence in case of FIFO interrupt mode:
     1, Are we the one requesting for it? If not, then go away
     2, Check whether we own the MUTEX
     3, Is there a valid waiter waiting for the MUTEX? If not, then release the MUTEX
     4, Wake up the waiter process
 *
 * 
 *  Sequence in case of NULL interrupt mode: 
     1, Are we the one wairing for it? If not, then go away
     2, Is there a valid waiter waiting for the MUTEX? If not, then release the MUTEX
     3, Try to lock the HW mutex again, if failure, wait for the next interrupt
 */
static irqreturn_t hw_mutex_isr(int irq, void *dev_id)
{
	struct hw_master *pmaster = (struct hw_master *)dev_id;
	struct hw_mutex_operations *hmutex_ops = pmaster->ops;
	struct hw_mutex *hmutex;
	irqreturn_t	ret = IRQ_NONE;
	int i;

	switch(pmaster->mode){
		case MUTEX_FIFO_SCHE:	
			hmutex_ops->clr_intr(pmaster);
			ret = IRQ_HANDLED;
			for (i = 0; i< HW_MUTEX_TOTAL; i++) {
				hmutex = &pmaster->hw_mutexes[i]; 
				spin_lock(&hmutex->irq_lock);
				if (HW_MUTEX_REQUESTING == atomic_read(&hmutex->status)){ 
					if (hmutex_ops->is_locked(hmutex)) {
						atomic_set(&hmutex->status, HW_MUTEX_LOCKED);
						if (likely(hw_mutex_get_owner(hmutex)))
							wake_up_process(hmutex->owner->task);
						else
						/* Nobody need the MUTEX, just unlock it to avoid deadlock */
							hmutex_ops->unlock(hmutex);				
					}
				}
			spin_unlock(&hmutex->irq_lock);
			}
			break;
		case MUTEX_NULL_SCHE: 		
			for (i = 0; i< HW_MUTEX_TOTAL; i++){	
				hmutex = &pmaster->hw_mutexes[i]; 
				if (hmutex_ops->is_waiting(hmutex)) {
					hmutex_ops->clr_intr(pmaster);
					spin_lock(&hmutex->irq_lock);				
					 if (likely(hw_mutex_get_owner(hmutex))){
						 /* Forcibly request it again */
						 if (hmutex_ops->lock(hmutex,1))
							 wake_up_process(hmutex->owner->task);
					}
					 spin_unlock(&hmutex->irq_lock);
					 ret = IRQ_HANDLED;
				}
			}
			break;
		case MUTEX_POLLING:
			return ret;
	}	
	/* clear interrupt status */
	return ret;
}

static inline long __sched
do_wait_common(struct hw_mutex_operations *hmutex_ops, struct hw_mutex *hmutex, long timeout, int state, unsigned long flags)
{

	DEBUG_PRINT;
	do {
		if (hmutex_ops->is_locked(hmutex))
			break;
		if (unlikely(signal_pending_state(state, current))) {
			printk(KERN_ERR "interrupt by signal\n");
			return -EINTR;
		}
		__set_current_state(state);
		spin_unlock_irqrestore(&hmutex->irq_lock,flags);	
		timeout = schedule_timeout(timeout);
		/* timeout:0, wake up for timer timeout, positive value: wake up by others */
		spin_lock_irqsave(&hmutex->irq_lock,flags);	
	} while (timeout);
	
	DEBUG_PRINTK("exit loop timeout 0x%x\n",(unsigned int)timeout);
	if (likely(hmutex_ops->is_locked(hmutex))) 
		return 0;
	else
    {
        printk(KERN_ERR "HW_Mutex-ERROR: timeout while waiting to HW mutex id=%d\n",hmutex->lock_name);
        return -EINTR;
    }
}


/*
  * hw_mutex_lock - acquire the mutex
  * @mutex: the mutex to be acquired
  *
  * Lock the mutex exclusively for this task. If the mutex is not
  * available right now, it will sleep until we can get it.
  *
  * The function is non interruptible
  */
void hw_mutex_lock(uint8_t mutex)
{
	struct hw_mutex  * hmutex = NULL;
	struct hw_mutex_operations *hmutex_ops = to_hw_mutex_ops();
	unsigned long int flags;
	long timeout;	
	
	BUG_ON(mutex >= HW_MUTEX_TOTAL);
	hmutex = to_hw_mutex(mutex);
	
	DEBUG_PRINTK("hmutex 0x%x, number %d\n",(unsigned int)hmutex,hmutex->lock_name);
	might_sleep();
		
	mutex_lock(&hmutex->lock);
	spin_lock_irqsave(&hmutex->irq_lock,flags);	
	hw_mutex_set_owner(hmutex);
	if (hmutex_ops->lock(hmutex,0)) {
	        spin_unlock_irqrestore(&hmutex->irq_lock,flags);	
		return ;
	}
	timeout = do_wait_common(hmutex_ops,hmutex,MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE, flags);
	if (timeout == -EINTR) 
		hw_mutex_clear_owner(hmutex);
	spin_unlock_irqrestore(&hmutex->irq_lock,flags);	

	/* Lock failure */
	if (timeout) 
		mutex_unlock(&hmutex->lock);
	return ;
}

EXPORT_SYMBOL(hw_mutex_lock);

/*
 * hw_mutex_lock_interruptible - acquire the mutex
 * @mutex: the mutex to be acquired
 *
 * Lock the mutex exclusively for this task. If the mutex is not
 * available right now, it will sleep until it can get it.
 * It can be interruptibed by signal, or exit when timeout
 *
 * Returns 0 if success, negative if interrupted or timeout
 */

long __sched hw_mutex_lock_interruptible(uint8_t mutex)
{
	struct hw_mutex  * hmutex = NULL;
	struct hw_mutex_operations *hmutex_ops = to_hw_mutex_ops();
	unsigned long int flags;
	long timeout;	
	
	if (WARN_ON( mutex >= HW_MUTEX_TOTAL)) return -EINVAL;
	hmutex = to_hw_mutex(mutex);
	
	DEBUG_PRINTK("hmutex 0x%x, number %d\n",(uint32_t)hmutex,hmutex->lock_name);
	might_sleep();

	if (mutex_lock_interruptible(&hmutex->lock))
		return -EINTR;
	
	spin_lock_irqsave(&hmutex->irq_lock,flags);	
	hw_mutex_set_owner(hmutex);
	if (hmutex_ops->lock(hmutex,0)) {
		spin_unlock_irqrestore(&hmutex->irq_lock,flags);	
		return 0;
	}
	timeout = do_wait_common(hmutex_ops,hmutex,IRQ_HW_MUTEX_TIME_OUT, TASK_INTERRUPTIBLE, flags);
	if (timeout == -EINTR)
		hw_mutex_clear_owner(hmutex);
	spin_unlock_irqrestore(&hmutex->irq_lock,flags);	

	/* Lock failure */
	if (timeout) 
		mutex_unlock(&hmutex->lock);
	return timeout;

}
EXPORT_SYMBOL(hw_mutex_lock_interruptible);

/*
 * hw_mutex_unlock - release the mutex
 * @mutex: the mutex to be released
 */
void hw_mutex_unlock(uint8_t mutex)
{
	struct hw_mutex  * hmutex = NULL;	
	struct hw_mutex_operations *hmutex_ops = to_hw_mutex_ops();
	unsigned long flags;
	BUG_ON( mutex >= HW_MUTEX_TOTAL);
	
	hmutex = to_hw_mutex(mutex);
	DEBUG_PRINTK("hmutex 0x%x, number %d\n",(uint32_t)hmutex,hmutex->lock_name);

	if (hw_mutex_get_owner(hmutex) != current_thread_info()){	
		printk(KERN_DEBUG "WARN: HW MUTEX should be released by the same owner %x\n", mutex);
		return;
	}
	else{		
		DEBUG_PRINTK("hmutex 0x%x, number %d, owner 0x%x, released by 0x%x\n",(uint32_t)hmutex,hmutex->lock_name,(uint32_t)hw_mutex_get_owner(hmutex),(uint32_t)current_thread_info());
	        spin_lock_irqsave(&hmutex->irq_lock,flags);	
		hmutex_ops->unlock(hmutex);

		hw_mutex_clear_owner(hmutex);
	        spin_unlock_irqrestore(&hmutex->irq_lock,flags);	
		mutex_unlock(&hmutex->lock);
	}
}
EXPORT_SYMBOL(hw_mutex_unlock);


int hw_mutex_register (struct hw_master *pmaster)
{
	if (WARN_ON(pmaster == NULL)) return -EINVAL;
	hw_master_glob = pmaster;
	if (pmaster->mode != MUTEX_POLLING){
		if (request_irq(pmaster->irq_num, hw_mutex_isr, IRQF_SHARED,HW_MUTEX_IRQ_NAME, (void *)pmaster)){
			printk(KERN_ERR "HW Mutex: Unable to allocate IRQ\n");
			return -ENODEV;
		}
	}
	return 0;
}
EXPORT_SYMBOL(hw_mutex_register);

void hw_mutex_unregister(struct hw_master *pmaster)
{
	if (WARN_ON(pmaster == NULL)) return;
	if (pmaster->mode != MUTEX_POLLING){
		free_irq(pmaster->irq_num,(void *)pmaster);
	}
	hw_master_glob = NULL;
}
EXPORT_SYMBOL(hw_mutex_unregister);

int try_lock_emmc_access(void)
{
    might_sleep();
    return mutex_trylock(&mmc_access_lock);
}
EXPORT_SYMBOL(try_lock_emmc_access);

void lock_emmc_access(void)
{
    might_sleep();
    mutex_lock(&mmc_access_lock);
}
EXPORT_SYMBOL(lock_emmc_access);

void unlock_emmc_access(void)
{
    mutex_unlock(&mmc_access_lock);
}
EXPORT_SYMBOL(unlock_emmc_access);



