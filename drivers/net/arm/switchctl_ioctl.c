
/* Requires   MODULE   defined on the command line */
/* Requires __KERNEL__ defined on the command line */
/* Requires __SMP__    defined on the command line for SMP systems */

#define _NO_VERSION_
#include <linux/module.h>  /* Module support */
#include <linux/version.h> /* Kernel version */
#include <linux/fs.h>            /* File operations (fops) defines */
#include <linux/ioport.h>        /* Memory/device locking macros   */

#include <linux/errno.h>         /* Defines standard error codes */

#include <asm/uaccess.h>         /* User space access routines */

#include <linux/sched.h>         /* User privilages (capabilities) */
#include "switchctl_fops.h"  
#include "switchctl_ioctl.h" /* Defines ioctl numbers */


s8 rtl8363_setAsicReg(u8 phyad, u8 regad, u8 pagtype,  u8 npage, u16 value);
s8 rtl8363_getAsicReg(u8 phyad, u8 regad, u8 pagtype,  u8 npage, u16 *pvalue);


/* ========================================================================
   Name:        switchctl_ioctl
   Description: Handle any device specific calls 
   ======================================================================== */

int switchctl_ioctl(struct inode *node,struct file *filp,unsigned int cmd,unsigned long arg)
{
    int err = 0;
   
    /*** Check the access permittions ***/

    if ((_IOC_DIR(cmd) & _IOC_READ) && !(filp->f_mode & FMODE_READ)) 
    {
        /* No Read permittions */
        err = -ENOTTY;
        goto fail;
    }

    if ((_IOC_DIR(cmd) & _IOC_WRITE) && !(filp->f_mode & FMODE_WRITE)) 
    {
        /* No Write permittions */
        err = -ENOTTY;
        goto fail;
    }

    /*** Execute the command ***/
     switch (cmd)
    {
         case SWITCHCTL_IOC_READREG:
        {
            int error;
            switchreg_t sr;
            if ((error = copy_from_user(&sr, (unsigned int *)arg, sizeof(switchreg_t)) ))
            {
                goto fail;
            }           
            error = rtl8363_getAsicReg(sr.phyad, sr.regad, sr.pagtype,  sr.npage, &sr.value); 
            //printk("read :%d,%d,%d,%d,%x\n",sr.phyad, sr.regad, sr.pagtype,  sr.npage, sr.value);
            if (error) 
	        {
		        printk("SWITCHCTL_IOC_READREG : rtl8363_getAsicReg failed\n");
                goto fail;
	        }
     	    if((err = copy_to_user((int *)arg, &sr, sizeof(switchreg_t))))
            {
                goto fail;
            }           
            break;  
        }            
        
         case SWITCHCTL_IOC_WRITEREG:
        {
            switchreg_t sr;
            int error;
            if ((error = copy_from_user(&sr, (unsigned int *)arg, sizeof(switchreg_t)) ))
            {
                goto fail;
            }            
        	error = rtl8363_setAsicReg(sr.phyad, sr.regad, sr.pagtype,  sr.npage, sr.value); 
        	//printk("write :%d,%d,%d,%d,%x\n",sr.phyad, sr.regad, sr.pagtype,  sr.npage, sr.value);
        	
	        if (error) 
	        {
		        printk("SWITCHCTL_IOC_WRITEREG : rtl8363_setAsicReg failed \n");
                goto fail;
	        }
            break;  
        }            
        
        
        
        default :
            /*** Inappropriate ioctl for the device ***/
            err = -ENOTTY;
            goto fail;
    }


    return (0);

    /**************************************************************************/

    /*** Clean up on error ***/

fail:
    return (err);
}
