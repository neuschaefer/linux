/* Include */
/* ------- */
#include <linux/init.h>    /* Initiliasation support */
#include <linux/module.h>  /* Module support */
#include <linux/kernel.h>  /* Kernel support */
#include <linux/version.h> /* Kernel version */
#include <linux/fs.h>      /* File operations (fops) defines */
#include <linux/cdev.h>    /* Charactor device support */
#include <linux/netdevice.h> /* ??? SET_MODULE_OWNER ??? */
#include <linux/ioport.h>  /* Memory/device locking macros   */
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>

/* UTS_RELEASE             - Kernel version  (text) */
/* LINUX_VERSION_CODE      - Kernel version  (hex)  */

#include <linux/errno.h>   /* Defines standard error codes */
#include <linux/sched.h>   /* Defines pointer (current) to current task */

#include "switchctl_fops.h"
#include "switchctl_ioctl.h"

/* Linux compatibility */
/* ------------------- */
#if LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,12)
#define class_simple                                      class
#define class_simple_create(a,b)                          ((struct class *)1)
#define class_simple_destroy(a)
#define class_simple_device_add(a,b,c,d_args...)          ((struct class_device *)1)
#define class_simple_device_remove(a)
#define io_remap_page_range(vma, vaddr, pfn, size, prot)  io_remap_pfn_range(vma, vaddr, (pfn) >> PAGE_SHIFT, size, prot)
#endif

/* Local Prototypes */
/* ---------------- */
static int  switchctl_init_module(void);
static void switchctl_cleanup_module(void);
static int  switchctl_device_register(struct file_operations *stdevice_fops,unsigned int nbdevices,char *device_name,unsigned int *device_major,struct cdev **stdevice_cdev,struct class_simple **stdevice_class);
static int  switchctl_device_unregister(unsigned int nbdevices,char *device_name,unsigned int device_major,struct cdev *stdevice_cdev,struct class_simple *stdevice_class);

/* Module info */
/* ----------- */
MODULE_AUTHOR          ("Frederic Navarre");
MODULE_DESCRIPTION     ("Realtek Switch register access");
MODULE_SUPPORTED_DEVICE("Spear300");
MODULE_LICENSE         ("ST Microelectronics");
module_init            (switchctl_init_module);
module_exit            (switchctl_cleanup_module);

/* Constants */
/* --------- */
#define SWITCHCTL_LINUX_DEVICE_NAME "switchctl"

/* Global variables */
/* ---------------- */
unsigned int            switchctl_major;
struct semaphore        switchctl_lock; /* Global lock for the module */
struct cdev            *switchctl_cdev   = NULL;
struct class_simple    *switchctl_class  = NULL;

/* Definition for file operation */
/* ----------------------------- */
static struct file_operations switchctl_fops = 
{
 owner   : THIS_MODULE,
 open    : switchctl_open,
 ioctl   : switchctl_ioctl,
 //mmap    : switchctl_mmap, 
 release : switchctl_release, 
};

/* ========================================================================
   Name:        switchctl_init_module
   Description: Initialise the module and set the major number 
   ======================================================================== */

static int __init switchctl_init_module(void)
{
 /* For debug */
 /* --------- */
 printk(KERN_INFO "switchctl_init_module(): Initializing SWITCHCTL Module\n");
 switchctl_major=SWITCHCTL_MAJOR;
 
 /*** Set up the modules global lock ***/
 sema_init(&(switchctl_lock), 1);

 /* Register the device with the kernel */
 /* ----------------------------------- */
 if (switchctl_device_register(&switchctl_fops,1,SWITCHCTL_LINUX_DEVICE_NAME,&switchctl_major,&switchctl_cdev,&switchctl_class)!=0)
  {
   printk(KERN_ALERT "switchctl_init_module(): switchctldevice_register() error !!!\n");
   return(-1);
  }
  
  /* STFAE - Change debug trace */
 printk(KERN_ALERT "Load module switchctl [%d]\t\tby %s (pid %i)\n", switchctl_major,current->comm, current->pid);

 /* Return no errors */
 /* ---------------- */
 return(0);
}

/* ========================================================================
   Name:        switchctl_cleanup_module
   Description: Initialise the module and set the major number 
   ======================================================================== */

static void __exit switchctl_cleanup_module(void)
{
 /* For debug */
 /* --------- */
 printk(KERN_INFO "switchctl_cleanup_module(): Exiting SWITCHCTL kernel Module\n");

 /* Unregister the device with the kernel */
 /* ------------------------------------- */
 switchctl_device_unregister(1,SWITCHCTL_LINUX_DEVICE_NAME,switchctl_major,switchctl_cdev,switchctl_class);
}

/* ========================================================================
   Name:        switchctl_device_register
   Description: Register the device to the kernel 
   ======================================================================== */

static int switchctl_device_register(struct file_operations *stdevice_fops,unsigned int nbdevices,char *device_name,unsigned int *device_major,struct cdev **stdevice_cdev,struct class_simple **stdevice_class)
{
 struct class_device *class_err;
 int                  entries;
 dev_t                base_dev_no;
 char                *extended_name;

 /* Allocate a string device name */
 /* ----------------------------- */
 extended_name=(char *)vmalloc(strlen(device_name)+11); /* +11 equals max place for unsigned int number + \0 */
    
 /* Set module owner */
 /* ---------------- */
 SET_MODULE_OWNER(stdevice_fops);

 /* Register the major number of the device */
 /* --------------------------------------- */
 /* Register the major number. If major = 0 then a major number is auto */
 /* allocated. The allocated number is returned.                        */
 /* The major number can be seen in user space in '/proc/devices'       */
 if (*device_major==0)
  {
   if (alloc_chrdev_region(&base_dev_no,0,nbdevices, device_name))
    {
     printk(KERN_ALERT "%s(): No major numbers for %s by %s (pid %i)\n",__FUNCTION__,device_name,current->comm,current->pid);
     vfree(extended_name);
     return(-EBUSY);
    }
   *device_major=MAJOR(base_dev_no);
  }
 else
  {
   base_dev_no=MKDEV(*device_major,0);
   if (register_chrdev_region(base_dev_no,nbdevices,device_name))
    {
     printk(KERN_ALERT "%s(): No major numbers for %s by %s (pid %i)\n",__FUNCTION__,device_name,current->comm,current->pid);
     vfree(extended_name);
     return(-EBUSY);
    }
  }

 /* Allocate a device */
 /* ----------------- */
 if ((*stdevice_cdev=cdev_alloc())==NULL)
  {
   printk(KERN_ALERT "%s(): No major numbers for %s by %s (pid %i)\n",__FUNCTION__,device_name,current->comm,current->pid);
   unregister_chrdev_region(MKDEV(*device_major,0),nbdevices);
   vfree(extended_name);
   return(-EBUSY);
  }

 /* Set device name */
 /* --------------- */
 sprintf(extended_name,"%s%d",device_name,0);
 (*stdevice_cdev)->owner = THIS_MODULE;
 (*stdevice_cdev)->ops   = stdevice_fops;
 kobject_set_name(&((*stdevice_cdev)->kobj),extended_name);

 /* Add the char device structure for this module */
 /* --------------------------------------------- */
 if (cdev_add(*stdevice_cdev,base_dev_no,nbdevices))
 {
  printk(KERN_ALERT "%s(): Failed to add a charactor device for %s by %s (pid %i)\n",__FUNCTION__,device_name,current->comm,current->pid);
  kobject_put(&((*stdevice_cdev)->kobj));
  unregister_chrdev_region(MKDEV(*device_major,0),nbdevices);
  vfree(extended_name);
  return(-EBUSY);
 }

 /* Create a class for this module */
 /* ------------------------------ */
 *stdevice_class=class_simple_create(THIS_MODULE,device_name);
 if (IS_ERR(*stdevice_class))
  {
   printk(KERN_ERR "%s(): Error creating %s class\n",__FUNCTION__,device_name);
   cdev_del(*stdevice_cdev);
   unregister_chrdev_region(MKDEV(*device_major,0),nbdevices);
   vfree(extended_name);
   return(-EBUSY);
  }

 /* Add entries into /avmem for each minor number we use */
 /* ---------------------------------------------------- */
 for (entries=0;entries<nbdevices;entries++)
  {
   sprintf(extended_name,"%s%d",device_name,entries);
   class_err=class_simple_device_add(*stdevice_class,MKDEV(*device_major,entries),NULL,extended_name);
   if (IS_ERR(class_err))
    {
     printk(KERN_ERR "%s(): Error creating %s device %d\n",__FUNCTION__,device_name,entries);
     for (entries--;(entries>=0);entries--) class_simple_device_remove(MKDEV(*device_major,entries));
     class_simple_destroy(*stdevice_class);
     cdev_del(*stdevice_cdev);
     unregister_chrdev_region(MKDEV(*device_major,0),nbdevices);
     vfree(extended_name);
     return(PTR_ERR(class_err));
    }
  }

 /* For debug */
 /* --------- */
 printk(KERN_INFO "%s(): Load module %s by %s (pid %i)\n",__FUNCTION__,device_name,current->comm,current->pid);
 
 /* Return no errors */
 /* ---------------- */
 vfree(extended_name);
 return(0);
}

/* ========================================================================
   Name:        switchctl_device_unregister
   Description: Unregister the device to the kernel 
   ======================================================================== */

static int switchctl_device_unregister(unsigned int nbdevices,char *device_name,unsigned int device_major,struct cdev *stdevice_cdev,struct class_simple *stdevice_class)
{
 unsigned int i;

 /* Remove any devices */
 /* ------------------ */
 for (i=0;(i<nbdevices);i++)
  {
   class_simple_device_remove(MKDEV(device_major,i));
  }

 /* Remove the device class entry */
 /* ----------------------------- */
 class_simple_destroy(stdevice_class);

 /* Remove the char device structure (has been added) */
 /* ------------------------------------------------- */
 cdev_del(stdevice_cdev);

 /* Unregister the module */
 /* --------------------- */
 unregister_chrdev_region(MKDEV(device_major, 0), nbdevices);

 /* For debug */
 /* --------- */
 printk(KERN_INFO "%s(): Unload module %s by %s (pid %i)\n",__FUNCTION__,device_name,current->comm,current->pid);
 
 /* Return no errors */
 /* ---------------- */
 return(0);
}
