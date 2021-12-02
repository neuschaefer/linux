/* Mmac PCI/Ethernet test chip driver for OS Linux
 * Linux netcard driver
 *
 *
 * 3 Jun 1999 - G.Kazakov.
 */
#if 0
#ifdef MODULE
#ifdef MODVERSION
#include <linux/modversion.h>
#endif
#include <linux/module.h>
#else
#define MOD_INC_USE_COUNT
#define MOD_DEC_USE_COUNT
#endif
#endif
#define MOD_INC_USE_COUNT
#define MOD_DEC_USE_COUNT

#include <linux/moduleparam.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/mii.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <asm/memory.h>

#include "mmac_dev.h"                   /* Mmac device definitions and functions */
#include "mmac_reg.h"
//#include "../../mtk/mt5391/5391_driver/drv_inc/c_model.h"


static u8 mac_addr[6] = { 0x00, 0x55, 0x7B, 0xB5, 0x7D, 0xF7 };
static int num=6;
module_param_array(mac_addr, byte, &num , S_IRUGO|S_IWUSR);

#ifdef MT539X_LAN8700
#define DEFAULT_PHY_ADDRESS 22
#else
#define DEFAULT_PHY_ADDRESS 1
#endif
#define DEFAULT_MAC_ADDRESS { 0x00, 0x55, 0x7B, 0xB5, 0x7D, 0xF7 }


#define TIMEOUT  (10*HZ)


#if LINUX_VERSION_CODE == 131589
    #define net_device device
    #define early_stop_netif_stop_queue(dev) test_and_set_bit(0, &dev->tbusy)
#else
    #define early_stop_netif_stop_queue(dev) 0 
#endif


static struct net_device *this_device;

static int io = 0x32000;
static int irq = ETH_IRQ;
static int dma = 0;
static int mem = 0;
u32 addr;

static int probe( struct net_device *dev, u32 Addr );
static int open( struct net_device *dev );
static int stop( struct net_device *dev );
static void receive( struct net_device *dev );       /* handle received packets */
static void finish_xmit( struct net_device *dev );   /* finish packet transmision started by start_xmit */
static irqreturn_t int_handler( int int_num, void *dev_id);  /* device interrupt handler */
static int hard_start_xmit( struct sk_buff *skb, struct net_device *dev );
static int do_ioctl( struct net_device *dev, struct ifreq *ifr, int cmd );
static struct net_device_stats *get_stats(struct net_device *dev);
static int phy_reset(Mmac *Dev);
static void check_link_timer(unsigned long data);
static void set_multicast_list(struct net_device *dev);


//***********************************************************

// int init_module(void)
static int __init mtkmac_init(void)
{
    struct net_device *dev;
//    struct netdev_private *np;
    Private *pr = NULL;

    int err;

    TR( KERN_NOTICE "init MTK MAC driver, mac %x, %x\n", mac_addr[0], mac_addr[5]);

    //5391 pinmux, gpio, ckgen setup++
    hwcfgsetup();

    //memcpy(this_device.name, devicename, 16);
	/* Copy the parameters from insmod into the device structure. */
    //    dev = alloc_etherdev(sizeof(struct Private));
    dev = alloc_etherdev(0);  //private date will be allocate DMA mem later,
                                          //!!! netdev_priv() can not be use in this driver.
    if (!dev)
    {
      err = -ENOMEM;
      return err;
    }	
//************************
//net dev info initial    
    dev ->base_addr = io;
    dev->irq       = irq;
    dev->dma       = dma;
    dev->mem_start = mem;
    dev->open = &open;
    dev->hard_start_xmit = &hard_start_xmit;
    dev->stop = &stop;
    dev->do_ioctl = &do_ioctl;
    dev->get_stats = &get_stats;
    dev->set_multicast_list = &set_multicast_list;
//  if(debug) dev->flags |= IFF_DEBUG;
//    dev->features |= NETIF_F_IP_CSUM;
//    dev->flags |= IFF_MULTICAST; //set in ether_setup().
//************************

      this_device = dev;
      err = probe(this_device, 0);
	if (err)
	{
		goto err_out_free_dev;
	}

      pr = dev->priv;

//************************
// privite data initial
    spin_lock_init(&pr->lock);

//mii info initial //remove it
//      pr->mii.phy_id = DEFAULT_PHY_ADDRESS;
//	pr->mii.dev = dev;
//	pr->mii.mdio_read = MmacMiiRead;
//	pr->mii.mdio_write = MmacMiiWrite;
//	pr->mii.phy_id_mask = 0x1f;
//	pr->mii.reg_num_mask = 0x1f;
  
	err = register_netdev(this_device);
	if (err)
	{
		goto err_out_free_priv;
	}
  return 0;
//free priv and dev
  err_out_free_priv:
    if (pr)
    {
      kfree(this_device->priv);
    }
//free dev
  err_out_free_dev:
    free_netdev(dev);
    return err;
}

//static void __exit cleanup_module(void)
static void __exit mtkmac_exit(void)	
{
	/* No need to check MOD_IN_USE, as sys_delete_module() checks. */
	unregister_netdev(this_device);
	/*
	 * If we don't do this, we can't re-insmod it later.
	 * Release irq/dma here, when you have jumpered versions and
	 * allocate them in net_probe1().
	 */
	/*
	   free_irq(this_device.irq, dev);
	   free_dma(this_device.dma);
	*/
	/*release_region(this_device.base_addr, NETCARD_IO_EXTENT);*/

	if (this_device->priv)
	{
        TR( KERN_DEBUG "Mmac::exit, free private data\n");
	  kfree(this_device->priv);
	}
	if (this_device)
	{
         TR( KERN_DEBUG "Mmac::exit, free dev\n");
         free_netdev(this_device);
	}
        /* unmap the mapped memory */ 
        //if(addr)
        //    iounmap((u32 *)addr); 
       
       TR( KERN_INFO "Mmac::exit\n");

        
}

static void receive( struct net_device *dev )       /* handle received packets */
{
  Private *pr = NULL;
  Mmac *tc = NULL;
  int r = 0;
  
  if(dev == NULL)
     return;
  
  TR( KERN_DEBUG "Mmac::receive(%s)\n", dev->name );
  
  pr = (Private *)dev->priv;
  if(pr == NULL)
        return;
   
  tc = &pr->mmac;
  if(tc == NULL)
      return;  

  MmacDmaDescSync(tc, tc->rx, tc->rxCount*sizeof(DmaDesc), DMA_FROM_DEVICE);  //invalidate cache from physical memory
  do                          /* handle recieve descriptors */
  {
    u32 data1;
    u32 status;
    struct sk_buff *skb;
    //int debug_count;
    r = MmacDmaRxGet( tc, &status, NULL, NULL, &data1 );   /* get rx descriptor content */
    if( r >= 0 && data1 != 0 )
    {

      /*printk("Mmac::receive(%s) - get rx descriptor %d for skb %08x, status = %08x\n",
                          dev->name, r, data1, status );*/

      TR( KERN_DEBUG "Mmac::receive(%s) - get rx descriptor %d for skb %08x, status = %08x\n", dev->name, r, data1, status );
      skb = (struct sk_buff *)data1;
      /* printk("mmac: Status: %08x",status); */

      if( MmacDmaRxValid(status) ) /* process good packets only */
      {
        /** Sync skb->data in cache with memory **/
        struct sk_buff *newskb;
        int len;
        len = MmacDmaRxLength( status ) - 4; /* ignore Ethernet CRC bytes */

        TR( KERN_DEBUG "Mmac::skb->data %08x, skb->len %08x, len %8x\n", (u32)skb->data, (u32)skb->len, len);

        MmacDmaDescSync(tc, skb->data, len, DMA_FROM_DEVICE);  //invalidate cache from physical memory
//        MmacDmaDescSync(tc, skb->data, len, DMA_BIDIRECTIONAL);  //invalidate cache from physical memory

        /* printk("mmac: Len : %0d, Status: %08x",len,status); */

        if( len <= ETHERNET_PACKET_COPY )
        {
          newskb = dev_alloc_skb( len );  /* allocate new skb */
          if( newskb == NULL )
          {
            printk( KERN_ERR "Mmac::receive(%s) - no memory for newskb\n", dev->name );
          }
          else
          {
/*
            printk("\n");
            for (debug_count = 0; debug_count < len; debug_count++)
            {
               if (debug_count % 4 == 0)
	          printk("rx_data[%0d] = ",debug_count/4);
	       printk("%02x ", *((skb->data)+debug_count));
               if (debug_count % 4 == 3)
	          printk("\n");
            }
            printk("\n");
*/
            memcpy( skb_put(newskb,len), skb->data, len );   /* copy data to the new skb */
          }
        }
        else
        {
          newskb = skb;          /* will send up old skb */
          skb_put( newskb, len );/* increase data pointers, data already is in the skb */

          skb = alloc_skb( dev->mtu + ETHERNET_PACKET_EXTRA, GFP_ATOMIC );   /* allocate new skb for descriptor ring */

          if( skb == NULL )
          {
            TR( KERN_ERR "Mmac::receive(%s) - no memory for skb\n", dev->name );
            tc->stats.rx_dropped++;
          }
        }
        newskb->dev = dev;
        newskb->protocol = eth_type_trans(newskb, dev);    /* set packet type */
#if 0
//test ipchecksum++
        newskb->ip_summed = CHECKSUM_HW;
        newskb->csum = 0;
//test ipchecksum--        
#endif
#if 0
        netif_rx(newskb);        /* send the packet up protocol stack */
#else
        if(netif_rx(newskb) == NET_RX_DROP)
        {
           tc->stats.rx_dropped++;
        }
#endif
        dev->last_rx = jiffies;                         /* set the time of the last receive */
        tc->stats.rx_packets++;
        tc->stats.rx_bytes += len;
      }
      else
      {
	/* printk("** rx sts err = 0x%x\n", status); */
        tc->stats.rx_errors++;
        tc->stats.collisions += MmacDmaRxCollisions(status);
        tc->stats.rx_crc_errors += MmacDmaRxCrc(status);
/* added by reyaz */
	tc->stats.rx_frame_errors += MmacDmaRxFrame(status);
	tc->stats.rx_length_errors += MmacDmaRxLengthError(status);
      }

      if( skb != NULL )
      {
        TR( KERN_DEBUG "Mmac::set desc skb->data %08x, len %08x\n", (u32)skb->tail, (u32)skb->data);
        r = MmacDmaRxSet( tc, virt_to_phys(skb->tail), skb_tailroom(skb), (u32)skb );  /* put the skb to the descriptor */        
        if( r < 0 )
        {
          TR( KERN_DEBUG "Mmac::receive(%s) - cannot set rx descriptor for skb %p\n", dev->name, skb );
//          dev_kfree_skb(skb);
          dev_kfree_skb_irq(skb);          
        }
      }
    }

  } while( r >= 0 );
  MmacDmaDescSync(tc, tc->rx, tc->rxCount*sizeof(DmaDesc), DMA_TO_DEVICE);  //invalidate cache from physical memory
//  MmacDmaDescSync(Dev, rx, sizeof(DmaDesc), DMA_TO_DEVICE);  //flush cache to physical memory

}

static void finish_xmit( struct net_device *dev )   /* finish packet transmision started by start_xmit */
{
  Private *pr;
  Mmac *tc;
  int r;
  
  if(dev == NULL)
     return;

  TR( KERN_DEBUG "Mmac::finish_xmit(%s)\n", dev->name );

  pr = (Private *)dev->priv;
  if(pr == NULL)
    return;

  tc = &pr->mmac;
  if(tc == NULL)
    return;

  MmacDmaDescSync(tc, tc->tx, tc->txCount*sizeof(DmaDesc), DMA_FROM_DEVICE);  //invalidate cache from physical memory

  do                          /* handle transmit descriptors */
  {
    u32 data1;
    u32 status;
    u32 len;

    r = MmacDmaTxGet( tc, &status, NULL, &len, &data1 );   /* get tx descriptor content */
    if(  r >= 0 && data1 != 0 )
    {
      TR( KERN_DEBUG "Mmac::finish_xmit(%s) - get tx descriptor %d for skb %08x, status = %08x\n",
                          dev->name, r, data1, status );
      dev_kfree_skb_irq((struct sk_buff *)data1);
      /*printk ("finish_xmit(%s) - get tx descriptor %d for skb %08x, status = %08x\n",dev->name, r, data1, status); */ 

      if( MmacDmaTxValid(status) )
      {
        tc->stats.tx_bytes += len;
        tc->stats.tx_packets++;
      }
      else
      {
        printk("** tx sts err = 0x%x\n", status);
        tc->stats.tx_errors++;
        tc->stats.tx_aborted_errors += MmacDmaTxAborted(status);
        tc->stats.tx_carrier_errors += MmacDmaTxCarrier(status);
      }
      tc->stats.collisions += MmacDmaTxCollisions(status);
      tc->txFreeCount++; 
    }

  } while( r >= 0 );     /* while we have transmitted descriptors */

  //dev->tbusy = 0;
//  netif_start_queue(dev);

  if(tc->txFreeCount > 0)
  {
    netif_wake_queue(dev);  
  }
}

static irqreturn_t int_handler( int int_num, void *dev_id)  /* device interrupt handler */
{
  struct net_device *dev = (struct net_device *)dev_id;
  Private *pr = NULL;
  Mmac *tc = NULL;
  u32 irq = 0;

  if( dev == NULL )
  {
    printk( KERN_ERR "Mmac::int_handler - unknown device\n" );
    return IRQ_NONE;
  }

  /*dev->interrupt = 1;*/

  TR( KERN_DEBUG "Mmac::int_handler(%s)\n", dev->name );

  pr = (Private *)dev->priv;
  if(pr == NULL)
  {
    return IRQ_NONE;
  }

  tc = &pr->mmac;
  if(tc == NULL)
  {
    return IRQ_NONE;
  }

  irq = MmacDmaIntType(tc);    /* get mask of current interrupt requests */
  while( irq != 0 )
  {
    TR( KERN_DEBUG "Mmac::int_handler(%s) - irq = %08x\n", dev->name, irq );

    if( irq & MmacDmaError )      /* fatal error condition (bus error) */
    {
//      printk("<1> \n Fatal Bus Error Interrupt Seen\n");
      TR( KERN_WARNING "Mmac::int_handler(%s) - fatal error\n", dev->name );
    }

    if( irq & MmacDmaRxNormal )   /* Normal receiver interrupt */
    {
      TR( KERN_DEBUG "Mmac::int_handler(%s) - rx normal\n", dev->name );
      receive(dev);   /* handle received packets */
      /* MmacDmaRxResume(tc); */  /* resume receiver */
    }

    if( irq & MmacDmaRxAbnormal ) /* Abnormal receiver interrupt */
    {
      TR( KERN_WARNING "Mmac::int_handler(%s) - rx abnormal\n", dev->name );
      receive(dev);   /* handle received packets */
      /* no room for received packet - ignore */
      tc->stats.rx_over_errors++;
//      MmacDmaRxResume(tc);  /* resume receiver */
    }

    if( irq & MmacDmaRxStopped )  /* Receiver went to stopped state */
    {
      TR( KERN_WARNING "Mmac::int_handler(%s) - rx stopped\n", dev->name );
      /* reset or stop command issued - ignore */
    }

    if( irq & MmacDmaTxNormal )   /* Normal transmitter interrupt */
    {
      TR( KERN_DEBUG "Mmac::int_handler(%s) - tx normal\n", dev->name );
      /* printk("<1> \n Normal Tx Interrupt Seen\n"); */
      finish_xmit(dev);   /* finish packet transmision started by start_xmit */
    }

    if( irq & MmacDmaTxAbnormal ) /* Abnormal transmitter interrupt */
    {
      TR( KERN_WARNING "Mmac::int_handler(%s) - tx abnormal\n", dev->name );
      finish_xmit(dev);
    }

    if( irq & MmacDmaTxStopped )  /* Transmitter went to stopped state */
    {
      TR( KERN_WARNING "Mmac::int_handler(%s) - tx stopped\n", dev->name );
      /* reset or stop command issued - ignore */
    }

    irq = MmacDmaIntType(tc);    /* read interrupt requests came during interrupt handling */
  }

  MmacDmaIntEnable(tc);         /* reenable interrupts */
  //TR( KERN_DEBUG "Mmac::int_handler(%s) - return\n", dev->name );
  return IRQ_HANDLED;
  /*dev->interrupt = 0;*/
}

static int open( struct net_device *dev )
{
  Private *pr = NULL;
  Mmac *tc = NULL;
  
  if(dev == NULL)
     return -1;

  TR( KERN_NOTICE "open(%s)\n", dev->name );

  MOD_INC_USE_COUNT;
  pr = (Private *)dev->priv;
  if(pr == NULL)
     return -1;
 
  tc = &pr->mmac;
  if(tc == NULL)
    return -1;

  /* reset phy here */
  phy_reset(&pr->mmac);
//init carrier as no link.
  netif_carrier_off(dev);

  check_media(dev,pr, tc);

  TR( KERN_DEBUG "Mmac::open(%s) - init DMA\n", dev->name );
  if( MmacDmaInit( tc, pr->desc, sizeof( pr->desc ) ) != 0 )        /* Init Mmac DMA Engine */
  {
    printk( KERN_ERR "Mmac::open(%s) - cannot init DMA\n", dev->name );
    MOD_DEC_USE_COUNT ;
    return -ENODEV;
  }
  RxDescInit(dev,pr, tc);

  TR( KERN_DEBUG "Mmac::open(%s) - init MAC\n", dev->name );
  if( MmacMacInit( tc, dev->dev_addr, dev->broadcast ) != 0 )       /* Init Mmac MAC module */
  {
    printk( KERN_ERR "Mmac::open(%s) - cannot init MAC\n", dev->name );
    return -ENODEV;
  }
  MmacMMCInit(tc);
  MmacDmaIntClear(tc);    /* clear interrupt requests */
  MmacDmaIntEnable(tc);   /* enable all interrupts */

  TR( KERN_DEBUG "Mmac::open(%s) - request interrupt %d\n", dev->name, dev->irq );
  if( request_irq(dev->irq, &int_handler, IRQF_SAMPLE_RANDOM , dev->name, dev) != 0 )     /* request non-shared interrupt */
  {
    printk( KERN_ERR "Mmac::open(%s) - interrupt %d request fail\n", dev->name, dev->irq );
    MOD_DEC_USE_COUNT ;
    return -ENODEV;
  }

  MmacDmaRxStart(tc);     /* start receiver */
  MmacDmaTxStart(tc);     /* start transmitter, it must go to suspend immediately */

  //dev->tbusy = 0;             /* transmitter free */
   netif_start_queue(dev);
  /*dev->start = 1;              device started */

  /* set and active a timer process */
  init_timer(&pr->timer);
  pr->timer.expires  = CHECK_LINK_TIMER_OUT; 
  pr->timer.data     = (unsigned long) dev;
  pr->timer.function = &check_link_timer;
  add_timer(&pr->timer);

  return 0;
}

static int stop( struct net_device *dev )
{
  Private *pr = NULL;
  Mmac *tc = NULL;
  int r = 0, i = 0;

  if(dev == NULL)
       return -1;

  TR( KERN_INFO "stop(%s)\n", dev->name );
  MOD_DEC_USE_COUNT;
  pr = (Private *)dev->priv;
  if(pr == NULL)
        return -1;

  tc = &pr->mmac;
  if(tc == NULL)
     return -1;

  del_timer(&pr->timer);

  //dev->tbusy = 1;             /* disable transmitter */
  netif_stop_queue(dev);
  //netif_start_queue(dev);
  /*dev->start = 0;              device stopped */
  free_irq(dev->irq, dev);    /* disconnect from irq */

  MmacDmaIntDisable(tc);  /* disable all interrupts */
  MmacDmaRxStop(tc);      /* stop receiver, take ownership of all rx descriptors */
  MmacDmaTxStop(tc);      /* stop transmitter, take ownership of all tx descriptors */
  MmacDmaDescSync(tc, tc->tx, sizeof( pr->desc ), DMA_FROM_DEVICE);  //validate before access

  for( i=0; i<(tc->txCount + tc->rxCount); i++ )
  {
    DescTake( tc->tx + i );
  }

  MmacDmaDescSync(tc, tc->tx, sizeof( pr->desc ), DMA_TO_DEVICE);  //flush cache to physical memory


  do                          /* free recieve descriptors */
  {
    u32 data1;

    r = MmacDmaRxGet( tc, NULL, NULL, NULL, &data1 );   /* clear rx descriptor content */
    if( r >= 0 && data1 != 0 )
    {
      TR( KERN_DEBUG "Mmac::stop(%s) - get rx descriptor %d for skb %08x\n", dev->name, r, data1 );
      dev_kfree_skb((struct sk_buff *)data1);
    }

  } while( r >= 0 );

  do                          /* free transmit descriptors */
  {
    u32 data1;

    r = MmacDmaTxGet( tc, NULL, NULL, NULL, &data1 );   /* clear tx descriptor content */
    if(  r >= 0 && data1 != 0 )
    {
      TR( KERN_DEBUG "Mmac::stop(%s) - get tx descriptor %d for skb %08x\n", dev->name, r, data1 );
      dev_kfree_skb((struct sk_buff *)data1);
    }

  } while( r >= 0 );     /* while we do not wrapped back to descriptor 0 */

  return 0;
}

static int hard_start_xmit( struct sk_buff *skb, struct net_device *dev )
{
  Private *pr = NULL;
  Mmac *tc = NULL;
  DmaDesc *pDesc=NULL;
  unsigned long flags;

  int r = 0;
  //int debug_count;
  //int byte_align_length;
  if(dev == NULL)
      return -EBUSY;
  if(skb == NULL)
     return 1;

  TR( KERN_DEBUG "Mmac::hard_start_xmit(%s)\n", dev->name );

  pr = (Private *)dev->priv;
  if(pr == NULL)
     return -EBUSY;

  tc = &pr->mmac;
  if(tc == NULL)
     return -EBUSY;

  if(0)
  {
    int n;
    for( n=0; n<0x60; n+=4 )
      TR( KERN_DEBUG "CSR%d = %08x\n", n/4, ioread32(tc->dmaBase+n) );
  }
  
/*if( test_and_set_bit( 0, &dev->tbusy ) != 0)*/
if (early_stop_netif_stop_queue(dev) != 0)
  {
    if( jiffies - dev->trans_start < TIMEOUT ) return -EBUSY;

    printk( KERN_ERR "Mmac::hard_start_xmit(%s) - tx timeout expired, restart\n", dev->name );
    stop(dev);
    open(dev);
    dev->trans_start = jiffies;
  }

/* Fix Up to DWord Align the Buffer1 Pointer */
/*
  byte_align_length = ( (skb->data) - (skb->head) );

  if (byte_align_length != 0)
  {
  for (debug_count = 0; debug_count < (skb->len); debug_count++)
  {
    *((skb->head)+debug_count) = *((skb->data)+debug_count);
  }
  }
*/
//critical section++
  spin_lock_irqsave(&pr->lock, flags);

  r = MmacDmaTxSet( tc, ( virt_to_phys( skb->data ) ), skb->len, (u32)skb );
  if( r < 0 )
  {
    TR( KERN_WARNING "Mmac::hard_start_xmit(%s) - no more free tx descriptors\n", dev->name );
    return -EBUSY;
  }

  tc->txFreeCount--;
  if(tc->txFreeCount<0)
  {
    TR( KERN_ERR "Mmac::hard_start_xmit - txFreeCount < 0");
  }
//  consistent_sync(skb->data,skb->len,PCI_DMA_TODEVICE);
  //flush cache to dram
  pDesc = tc->tx + r ;
  MmacDmaDescSync(tc, pDesc, sizeof(DmaDesc), DMA_TO_DEVICE);  //flush cache to physical memory
  MmacDmaDescSync(tc, skb->data, skb->len, DMA_TO_DEVICE);  //flush cache to physical memory
//critical section--
  MmacDmaTxResume(tc);      /* resume transmitter */
  dev->trans_start = jiffies;
  //dev->tbusy = 0;
  if(tc->txFreeCount == 0)
  {
    TR( KERN_WARNING "Mmac::hard_start_xmit - txFreeCount == 0");
    netif_stop_queue(dev);
  }
  spin_unlock_irqrestore(&pr->lock, flags);

  return 0;
}

static int do_ioctl( struct net_device *dev, struct ifreq *ifr, int cmd )
{
  Private *pr = NULL;
  Mmac *tc = NULL;
  int r = 0;
  struct ifr_data_struct
  {
    u32 unit;
    u32 addr;
    u32 data;
  } *req;
  
  if(ifr == NULL)
      return -1;

  req = (struct ifr_data_struct *)ifr->ifr_data;
  if(dev == NULL)
    return -1;

  TR( KERN_INFO "Mmac::do_ioctl(%s, cmd=%08x)\n", dev->name, cmd );

  pr = (Private *)dev->priv;
 
  if(pr == NULL)
    return -1;

  tc = &pr->mmac;
 
  if(tc == NULL)
     return -1;

  switch( cmd )
  {
#if 0
  case SIOCDEVPRIVATE:      /* invert IFF_DEBUG flag */
    if( dev->flags & IFF_DEBUG ) dev->flags &= ~IFF_DEBUG;
    else dev->flags |= IFF_DEBUG;
    debug = ( dev->flags & IFF_DEBUG ) != 0;
    break;
#endif
  case SIOCDEVPRIVATE+1:    /* Read registers */
    if( req->unit == 0 )    /* Read MAC register */
    {
      req->data = MmacReadMacReg( tc, req->addr );
    }
    else if( req->unit == 1 )    /* Read DMA register */
    {
      req->data = MmacReadDmaReg( tc, req->addr );
    }
    else if( req->unit == 2 )    /* Read MII register */
    {
      req->data = MmacMiiRead( tc, req->addr );
    }
    else r = -EOPNOTSUPP;
    break;

  case SIOCDEVPRIVATE+2:    /* Write registers */
    if( req->unit == 0 )    /* Write MAC register */
    {
      MmacWriteMacReg( tc, req->addr, req->data );
    }
    else if( req->unit == 1 )    /* Write DMA register */
    {
      MmacWriteDmaReg( tc, req->addr, req->data );
    }
    else if( req->unit == 2 )    /* Write MII register */
    {
      MmacMiiWrite( tc, req->addr, req->data );
    }
    else r = -EOPNOTSUPP;
    break;

  case SIOCDEVPRIVATE+3:    /* Read tc */
    memcpy( ifr->ifr_data, tc, sizeof(*tc) );
    break;

  case SIOCDEVPRIVATE+4:    /* Read rx descriptor */
    memcpy( ifr->ifr_data, tc->rx + ((DmaDesc*)ifr->ifr_data)->data1, sizeof(DmaDesc) );
    break;                                    /* descr number is passed thru data1 field */

  case SIOCDEVPRIVATE+5:    /* Read tx descriptor */
    memcpy( ifr->ifr_data, tc->tx + ((DmaDesc*)ifr->ifr_data)->data1, sizeof(DmaDesc) );
    break;                                    /* descr number is passed thru data1 field */

  case SIOCDEVPRIVATE+6:    /* control operation */

    if (req->unit == 7)
    {
	u16 phy  = MmacMiiRead(tc,  MII_BMCR);
	MmacMiiWrite(tc, MII_BMCR, phy|BMCR_LOOPBACK);
	printk("<1>loop\n");
    }	
    else if (req->unit == 8)
    {
	u16 phy  = MmacMiiRead(tc, MII_BMCR);
	MmacMiiWrite(tc, MII_BMCR, phy & ~(BMCR_LOOPBACK));

    }	
    else r = -EOPNOTSUPP;
    break;

  default:
    r = -EOPNOTSUPP;
  }

  return r;
}

static struct net_device_stats *get_stats(struct net_device *dev)
{
  Private *pr = NULL;
  Mmac *tc = NULL;
  
  if(dev == NULL)
     return NULL;

//  TR( KERN_INFO "Mmac::get_stats(%s) : 0x%08x\n", dev->name, dev);

  pr = (Private *)dev->priv;
  if( pr == NULL)
     return NULL;
  
  tc = &pr->mmac;
  if(tc == NULL)
     return NULL; 

  TR( KERN_DEBUG "Mmac::get_stats(%s), dev:0x%08x, priv:0x%08x, tc:0x%08x\n", dev->name, (u32)dev, (u32)pr, (u32)tc);

  return &tc->stats;
}



static int probe( struct net_device *dev, u32 Addr )  /* probe device at specified location or find the device */
{
  u32 configBase = 0;
  u32 macBase    = MAC_BASE;
  u32 dmaBase    = DMA_BASE;
  u32 phyAddr    = DEFAULT_PHY_ADDRESS;
  u8  macAddr[6] = DEFAULT_MAC_ADDRESS;
  /* u32 addr = 0;  */ /* commented by Sankar on 10.05.04 */
  Private *pr = NULL;

  if(dev == NULL)
     return -1;

  TR( KERN_DEBUG "Mmac::probe(%p, %08x)\n", dev, Addr );

  pr = kmalloc( sizeof(Private), GFP_KERNEL | GFP_DMA ); /* get memory for private data, descriptors will be DMA-safe  */
  if( pr == NULL ) 
  {
    return -ENOMEM;
  }
  dev->priv = pr;

  MmacInit( &pr->mmac, configBase, macBase, dmaBase, phyAddr ); /* init Mmac internal data */

  memcpy( dev->dev_addr, macAddr, ETH_ALEN );       /* Set MAC address */

  TR( KERN_NOTICE "Mmac::probe - addr = %08lx, irq=%d\n", dev->base_addr, dev->irq );

  /* phy reset is moved to open */
  //phy_reset(&pr->mmac);

  return 0;
}

static int phy_reset(Mmac *Dev)
{
	u16 miicontrol = BMCR_RESET;
	unsigned int tries = 0;

      //release HW reset pin first
      //reset low
      hw_reset();

      //MmacMiiWrite(Dev, MII_BMCR, BMCR_RESET);
	
	/* wait for 500ms */
	//msleep(500);
    msleep(50);
      /* PHY_ID detect automatically */
      for(Dev->phyAddr = 0; Dev->phyAddr<=31; Dev->phyAddr++)
      {
        if(MmacMiiRead(Dev, MII_BMCR) != 0xFFFF)
        {
          TR( KERN_DEBUG "Mmac::PHY ID = %d\n", Dev->phyAddr );
          break;
        }
      }

      MmacMiiWrite(Dev, MII_BMCR, BMCR_RESET);

	/* must wait till reset is deasserted */
	while (miicontrol & BMCR_RESET) 
	{
		msleep(10);
		miicontrol = MmacMiiRead(Dev, MII_BMCR);
		/* FIXME: 100 tries seem excessive */
		if (tries++ > 100)
			return -1;
	}
	return 0;
}

int check_link (struct net_device *dev, Mmac *mmac)
{
    int re_link = 0;
    int cur_link;
    int prev_link = netif_carrier_ok(dev);
	

	cur_link = (MmacMiiRead(mmac, MII_BMSR) & BMSR_LSTATUS) ? 1:0;

	if (cur_link && !prev_link)
	{
//	  netif_carrier_on(mii->dev);
        printk( KERN_DEBUG "eth check_link  - link up\n");  
        re_link = 1;
	}
	else if (prev_link && !cur_link)
	{
	  netif_carrier_off(dev);
        printk( KERN_ERR "eth check_link  - no link\n");
      }

	return re_link;
}


void check_media(struct net_device *dev, Private *pr, Mmac *mmac)
{
  int mii_lpa;
  int negotiated;
  int duplex;
  int i;

  if (check_link(dev, mmac) == 0)
  {
  	return;
  }

//
//restart AN
//  MmacMiiWrite(mmac, MII_BMCR, (MmacMiiRead(mmac,MII_BMCR) | BMCR_ANRESTART));
//wait AN complete
  i=100000;
  while( i-- > 0 )
  {
    if( (MmacMiiRead(mmac,MII_BMSR) & BMSR_ANEGCOMPLETE) != 0 )
    {
      TR( KERN_NOTICE "check_media(%s) - auto-negotiation complete, i=%d\n", dev->name ,i);
      break;   /* wait for autonegotiation complete */
    }
  }
  if(i ==0)
  {
    return;
    printk( KERN_NOTICE "check_media(%s) - auto-negotiation not finish\n", dev->name );
  }
//check AN result
  mii_lpa = MmacMiiRead(mmac, MII_LPA);
  mmac->advertising = MmacMiiRead(mmac, MII_ADVERTISE);
  negotiated = mii_lpa & (mmac->advertising);
  duplex = (negotiated &  LPA_100FULL) || ((negotiated &  (LPA_100HALF|LPA_10FULL)) == LPA_10FULL);

  if (mii_lpa == 0xffff)		/* Bogus read */
	return;
  if (mmac->full_duplex != duplex) 
  {
	mmac->full_duplex = duplex;
  }
 
  printk( KERN_NOTICE "check_media(%s) - %s-duplex mode, adv:%4.4x\n", dev->name, mmac->full_duplex? "Full" : "Half" , mii_lpa);

 if(negotiated & (LPA_100FULL| LPA_100HALF))
 {
  mmac->speed_100 = 1;
  printk( KERN_NOTICE "check_media(%s) - 100M\n", dev->name);
 }
 else
 {
  mmac->speed_10 = 1;
  printk( KERN_NOTICE "check_media(%s) - 10M\n", dev->name);
 }

#if 1
  MmacMacReset(mmac);
#else
  MmacDmaRxStop(mmac);
  MmacDmaTxStop(mmac); 

  MmacMacInit( mmac, dev->dev_addr, dev->broadcast );

  MmacDmaRxStart(mmac);
  MmacDmaTxStart(mmac); 
#endif  
  netif_carrier_on(dev);
}


static void check_link_timer(unsigned long data)
{
	struct net_device *dev = (struct net_device *) data;
	Private *pr = dev->priv;
      Mmac * mmac = &pr->mmac;

      check_media(dev, pr, mmac);
	/* Set timer again */
	pr->timer.expires = CHECK_LINK_TIMER_OUT;
	add_timer(&pr->timer);
}

	
static void set_multicast_list(struct net_device *dev)
{
      Private *pr = NULL;
      Mmac *tc = NULL;
      unsigned long flags;
	
      if(dev == NULL)
        return;

      TR( KERN_NOTICE "set_multicast_list(%s)\n", dev->name );

      pr = (Private *)dev->priv;
      if(pr == NULL)
         return;
 
      tc = &pr->mmac;
      if(tc == NULL)
        return;

      if (dev->flags & IFF_PROMISC) //promiscuous mode, pass all frame
      {
        TR(KERN_NOTICE "%s: RCR_PRMS\n", dev->name);
        spin_lock_irqsave(&pr->lock, flags);
        MmacPassAllPacket(tc);
        spin_unlock_irqrestore(&pr->lock, flags);
      }
	/*
	 * Here, setting this to accept all multicast packets.
	 * I don't need to zero the multicast table, because the flag is
	 * checked before the table is
	 */
      else if (dev->flags & IFF_ALLMULTI || dev->mc_count > 15) // pass all multicast frame
      {
         TR(KERN_NOTICE "%s: RCR_ALMUL\n", dev->name);
         spin_lock_irqsave(&pr->lock, flags);
         MmacPassAllMulticast(tc);
         spin_unlock_irqrestore(&pr->lock, flags);         
      }

	/*
	 * This sets the internal hardware table to filter out unwanted
	 * multicast packets before they take up memory.
	 *
	 * use MAC address filter to get precise filter result
	 * If HIGH bit[31] is 1, then the
	 * multicast packet is accepted.  Otherwise, it's dropped silently.
	 * set filter depends on dev->mc_count
	 */
      else //if (dev->mc_count)                           // pass multicast frames which match DA  filter, need to set filter here
      {
//         TR(KERN_NOTICE "%s: Not IFF_ALLMULTI & not IFF_PROMISC\n", dev->name);
         spin_lock_irqsave(&pr->lock, flags);
         MmacPassMatchPacket(dev, tc);
         spin_unlock_irqrestore(&pr->lock, flags);         
	} 
#if 0      
      else                                                //No multicast allow, clear multicast DA filter to block all multicast frame
      {
         TR(KERN_NOTICE, "%s: ~(GmacPromiscuousModeOn|GmacMulticastFilterOff)\n",
         	dev->name);
         //clear DA filter
         //Set register to accept matched packet only.
         MmacPassMatchPacket(tc);
	  /*
	    * since I'm disabling all multicast entirely, I need to
	    * clear the multicast list
	    */
	}
#endif      
}

	
module_init(mtkmac_init);
module_exit(mtkmac_exit);
//MODULE_AUTHOR(DRIVER_AUTHOR);
//MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

