
/* Requires   MODULE   defined on the command line */
/* Requires __KERNEL__ defined on the command line */
/* Requires __SMP__    defined on the command line for SMP systems */

#define _NO_VERSION_
#include <linux/module.h>  /* Module support */
#include <linux/version.h> /* Kernel version */
#include <linux/fs.h>      /* File operations (fops) defines */
#include <linux/ioport.h>  /* Memory/device locking macros   */

#include <linux/errno.h>   /* Defines standard error codes */
#include <linux/slab.h>    /* Memory allocation */
#include <linux/slab.h>    /* Memory allocation */

/*** PROTOTYPES **************************************************************/

#include "switchctl_fops.h"

extern struct semaphore        switchctl_lock; /* Global lock for the module */

#define DATA_BUFFER_SIZE 64 /* The size of the internal buffer for the node */



typedef struct {

    struct semaphore      lock;        /* Lock for the device        */
    wait_queue_head_t     read_queue;  /* Wait for data to read      */
    wait_queue_head_t     write_queue; /* Wait for space to write to */

    char                  buffer[DATA_BUFFER_SIZE];    /* Internal  data buffer          */
    int                   read_idx;                    /* The read  index for the buffer */
    int                   write_idx;                   /* The write index for the buffer */

}
switchctl_dev_t;

switchctl_dev_t *switchctl_NewNode(unsigned int id);

/*** PRIVATE VARIABLES (static) ************************************************/
static switchctl_dev_t *table[16];

/*=============================================================================

   switchctl_open

   This function can deschedule and MUST be reentrant.

  ===========================================================================*/
int switchctl_open(struct inode *node, struct file *filp)
{

    int err   = 0;                      /* No error */
    int minor = iminor(node);           /* The major number */

    /*** Modify the private data if needed - could be set by devfs/sysfs ***/
    if (NULL == filp->private_data) 
    {
        /* switchctl_NewNode() may deshedule - call must be reentrant */
        filp->private_data = switchctl_NewNode(minor);

        if (NULL == filp->private_data) 
        {
            /* Memory allocation error */
            err = -ENOMEM;
            goto fail;
        }
    }

    /*** Check the access mode ***/
    printk(KERN_ALERT "ACCESS %08X\n", filp->f_flags);
    switch((filp->f_flags & O_ACCMODE))
    {
        case O_RDONLY : 
            printk(KERN_INFO "%s(): Open Read only\n",__FUNCTION__);
            break;

        case O_WRONLY :
            printk(KERN_INFO "%s(): Open Write only\n",__FUNCTION__);
            break;
            
        case O_RDWR   :
            printk(KERN_INFO "%s(): Open Read/Write only\n",__FUNCTION__);
            break;
            
        default :
            printk(KERN_ERR "%s(): Don't know how this is open !\n",__FUNCTION__);
            break;
    }
    
    if (filp->f_flags & O_CREAT   )
    {
        printk(KERN_INFO "%s(): Create if it does not exist\n",__FUNCTION__);
    }
    if (filp->f_flags & O_EXCL    )
    {
        printk(KERN_INFO "%s(): Provide exclusive access\n",__FUNCTION__);
    }
    if (filp->f_flags & O_NOCTTY  )
    {
        printk(KERN_INFO "%s(): ???? O_NOCTTY\n",__FUNCTION__);
    }
    if (filp->f_flags & O_TRUNC   )
    {
        printk(KERN_INFO "%s(): Truncate the file to zero size first\n",__FUNCTION__);
    }
    if (filp->f_flags & O_APPEND  )
    {
        printk(KERN_INFO "%s(): Append to the file (don't overwrite)\n",__FUNCTION__);
    }
    if (filp->f_flags & O_NONBLOCK)
    {
        printk(KERN_INFO "%s(): Access methods are non-blocking\n",__FUNCTION__);
    }
    if (filp->f_flags & O_SYNC    )
    {
        printk(KERN_INFO "%s(): ???? O_SYNC\n",__FUNCTION__);
    }

    return (0);  /* If we get here then we have succeeded */

    /*** Clean up on error ***/
fail :
    return (err);
}


/*=============================================================================

   switchctl_release   (close)

   This function can deschedule and MUST be reentrant.

  ===========================================================================*/
int switchctl_release(struct inode *node, struct file *filp)  /* close */
{

    int err = 0;  /* No error */

    return (0);  /* If we get here then we have succeeded */

    /*** Clean up on error ***/
    return (err);
}

/*=============================================================================

   switchctl_NewNode

   Create a new data structure for this 'device' (minor number)

   This function can deschedule and MUST be reentrant.

  ===========================================================================*/
switchctl_dev_t *switchctl_NewNode(unsigned int id)
{
    /* Check valid id */

    if (id >= (sizeof(table)/sizeof(*table))) {

        /* Invalid id */
        return (NULL);
    }

    down(&(switchctl_lock)); /* We can deshedule here !!! */

    if (NULL == table[id]) {

        /* Initialise the data structure */

        /* We can deshedule here !!! */
        table[id] = (switchctl_dev_t*)kmalloc(sizeof(switchctl_dev_t), GFP_KERNEL);

        if (NULL != table[id])
        {
            sema_init          (&(table[id]->lock), 1);
            init_waitqueue_head(&(table[id]->read_queue));
            init_waitqueue_head(&(table[id]->write_queue));
            table[id]->read_idx  = 0;
            table[id]->write_idx = 0;
        }
    }

    up(&(switchctl_lock));  /* Release the global lock */

    return (table[id]);
}


/*=============================================================================

   switchctl_GetNode

   Get an existing data structure for this 'device' (minor number)

  ===========================================================================*/
switchctl_dev_t *switchctl_GetNode(unsigned int id)
{
    /* Check valid id */

    if (id >=(sizeof(table)/sizeof(*table))) {

        /* Invalid id */
        return (NULL);
    }

    return (table[id]);
}

/*=============================================================================

   switchctl_CleanUp

   Free up all the allocated 'device' structures.

   This function can deschedule and MUST be reentrant.

  ===========================================================================*/
void switchctl_CleanUp(void)
{
    int i;

    down(&(switchctl_lock)); /* We can deshedule here !!! */

    for (i = 0; (i < (sizeof(table)/sizeof(*table))); i++) {

        if (NULL != table[i]) {

            kfree(table[i]);
        }

        table[i] = NULL;
    }

    up(&(switchctl_lock));
}
