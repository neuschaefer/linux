/*****************************************************************************
* Copyright 2010 - 2011 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>

#define  PROC_CMD_NAME     "dma-cmd"
#define  PROC_CMD_BUF_LEN  256

typedef struct
{
   struct list_head  list;
   int               id;
   void             *virt_addr;
   dma_addr_t        phys_addr;
   size_t            size;

} dma_alloc_t;

static   struct proc_dir_entry  *proc_cmd = NULL;
static   LIST_HEAD( dma_alloc_list );
static   DEFINE_MUTEX( dma_alloc_lock );

/****************************************************************************
*
*  Allocates a piece of DMA memory.
*
****************************************************************************/

static void dma_test_alloc( int id, size_t size )
{
   dma_alloc_t          *alloc;
   struct list_head     *iter;

   if (( alloc = kzalloc( sizeof( *alloc ), GFP_KERNEL )) == NULL )
   {
      printk( KERN_ERR "%s: failed to kmalloc %d bytes\n", __func__, sizeof( *alloc ));
      return;
   }

   alloc->id = id;
   alloc->size = size;

   if (( alloc->virt_addr = dma_alloc_coherent( NULL, size, &alloc->phys_addr, GFP_KERNEL )) == NULL ) {
      printk( KERN_ERR "%s: failed to dma_alloc_coherent %d bytes\n", __func__, size );
      kfree( alloc );
      return;
   }

   mutex_lock( &dma_alloc_lock );
   list_for_each( iter, &dma_alloc_list ) {
      dma_alloc_t *item = list_entry( iter, dma_alloc_t, list );

      if ( item->id == alloc->id ) {
         dma_free_coherent( NULL, item->size, item->virt_addr, item->phys_addr );
         list_replace( &item->list, &alloc->list );
         kfree( item );
         alloc = NULL;
         break;
      }
   }
   if (alloc != NULL) {
      list_add_tail( &alloc->list, &dma_alloc_list );
   }
   mutex_unlock( &dma_alloc_lock );
}

/****************************************************************************
*
*  Lists the currently allocated pieces of DMA memory.
*
****************************************************************************/

static void dma_test_alloc_show( void )
{
   struct list_head     *iter;

   mutex_lock( &dma_alloc_lock );

   printk( KERN_INFO "       ID  Virt Addr  Phys Addr     Size\n" );
   printk( KERN_INFO "--------- ---------- ---------- --------\n" );

   list_for_each( iter, &dma_alloc_list ) {
      dma_alloc_t *item = list_entry( iter, dma_alloc_t, list );

      printk( KERN_INFO "%9d 0x%p 0x%08x %8d\n",
              item->id, item->virt_addr, item->phys_addr, item->size );
   }
   mutex_unlock( &dma_alloc_lock );
}

/****************************************************************************
*
*  Frees a previously allocated piece of DMA memory
*
****************************************************************************/

static void dma_test_free( int id )
{
   struct list_head     *iter;

   mutex_lock( &dma_alloc_lock );
   list_for_each( iter, &dma_alloc_list )
   {
      dma_alloc_t *item = list_entry( iter, dma_alloc_t, list );

      if ( item->id == id ) {
         dma_free_coherent( NULL, item->size, item->virt_addr, item->phys_addr );
         list_del( &item->list );
         kfree( item );
         break;
      }
   }
   mutex_unlock( &dma_alloc_lock );
}

/****************************************************************************
*
*  Frees all of previously allocated DMA memory
*
****************************************************************************/

static void dma_test_free_all( void )
{
   struct list_head     *iter;
   struct list_head     *tmp;

   mutex_lock( &dma_alloc_lock );

   list_for_each_safe( iter, tmp, &dma_alloc_list ) {
      dma_alloc_t *item = list_entry( iter, dma_alloc_t, list );

      dma_free_coherent( NULL, item->size, item->virt_addr, item->phys_addr );
      list_del( &item->list );
      kfree( item );
   }

   mutex_unlock( &dma_alloc_lock );
}

/****************************************************************************
*
*  Called to process new commands
*
****************************************************************************/

static int proc_cmd_write(struct file *file, const char __user *buffer,
                          unsigned long count, void *data)
{
   int      rc;
   char    *s;
   char     cmd[ PROC_CMD_BUF_LEN ];
   int      id;
   size_t   size;
   char     size_str[ 20 ];

   if ( count > sizeof( cmd )) {
      count = sizeof( cmd );
   }
   if ((rc = copy_from_user(cmd, buffer, count)) != 0) {
      printk( KERN_ERR "%s: copy_from_user failed, rc = %d\n", __func__, rc );
      return -EFAULT;
   }

   /* Remove the trailing newline */

   if (( s = strchr( cmd, '\n')) != NULL ) {
      *s = '\0';
   }

   /*printk( KERN_DEBUG "%s: cmd = '%s'\n", __func__, cmd );*/

   if ( sscanf( cmd, "alloc %d %20s", &id, size_str ) == 2 ) {
      size = memparse( size_str, NULL );
      if ( size == 0 ) {
         printk( KERN_ERR "alloc needs a non-zero size\n" );
      }
      else
      {
         dma_test_alloc( id, size );
      }
   }
   else
   if ( strcmp( cmd, "show" ) == 0 ) {
      dma_test_alloc_show();
   }
   else
   if ( sscanf( cmd, "free %d", &id ) == 1 ) {
      dma_test_free( id );
   }
   else
   if ( strcmp( cmd, "free-all" ) == 0 ) {
      dma_test_free_all();
   }
   else
   {
      printk( KERN_ERR "%s: Unrecognized command '%s'\n", __func__, cmd );
   }

   return count;
}

/****************************************************************************
*
*  Called when the module loads.
*
****************************************************************************/

static int __init dma_test_init(void)
{
   printk( KERN_NOTICE "%s: DMA Test Module loading...\n", __func__ );

   if (( proc_cmd = create_proc_entry( PROC_CMD_NAME, 0200, NULL )) == NULL ) {
      printk( KERN_ERR "create_proc_entry( '%s' ) failed\n", PROC_CMD_NAME );
      return -ENOMEM;
   }
   proc_cmd->write_proc = proc_cmd_write;

   return 0;
}

/****************************************************************************
*
*  Called when the module unloads.
*
****************************************************************************/

static void __exit dma_test_exit(void)
{
   printk( KERN_NOTICE "%s: DMA Test Module unloading...\n", __func__ );

   dma_test_free_all();

   if (proc_cmd != NULL) {
      remove_proc_entry( PROC_CMD_NAME, NULL );
   }
}

module_init(dma_test_init);
module_exit(dma_test_exit);

MODULE_AUTHOR("Broadcom");
MODULE_DESCRIPTION("Broadcom DMA Test");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
