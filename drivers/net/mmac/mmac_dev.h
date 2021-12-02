/* Mmac PCI/Ethernet test chip driver for OS Linux
 *
 * Device-dependent (mostly system-independent) functions
 *
 * 1 Jun 1999 - G.Kazakov.
 */

#ifndef MMAC_INCLUDED
#define MMAC_INCLUDED

#include <asm/types.h>
#include <asm/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#define ETHER_MAX_MULTICAST_NUM		15		/* Largest number of multicast list */
#define ETHERNET_PACKET_EXTRA 18    /* preallocated length for rx packets is MTU + ETHERNET_PACKET_EXTRA */
#define ETHERNET_PACKET_COPY  250   /* max length when received data is copied to new skb */
#define CHECK_LINK_TIMER_OUT  (jiffies+(HZ/2))	/* timer wakeup time : 0.5 second */

#define TR mmac_trace
#ifdef DEBUG_ETH_ON
  #define TRACE
#endif
#ifdef TRACE
  #define mmac_trace(fmt... )  printk(fmt)
#else
  #define mmac_trace(fmt... )
  //void mmac_trace() {}
#endif

typedef struct DmaDescStruct    /* The structure is common for both receive and transmit descriptors */
{                               /* put it here to allow the caller to know its size */
  u32   status;         /* Status */
  u32   length;         /* Buffer length */
  u32   buffer1;        /* Buffer 1 pointer */
  u32   buffer2;        /* Buffer 2 pointer or next descriptor pointer in chain structure */
  
  u32   data1;          /* driver data, are not used by DMA engine,                       */
  u32   data2;          /* for 128bit bus width, set DmaDescriptorSkip1 in DmaBusModeInit to skip these words */
  u32   data3;          /* for 128bit bus width, set DmaDescriptorSkip1 in DmaBusModeInit to skip these words */
  u32   data4;          /* for 128bit bus width, set DmaDescriptorSkip1 in DmaBusModeInit to skip these words */
} DmaDesc;

typedef struct MmacStruct       /* Mmac device data */
{
  u32 configBase;   /* base address of Config registers */
  u32 macBase;      /* base address of MAC registers */
  u32 mmcBase;      /* base address of MMC registers */
  u32 dmaBase;      /* base address of DMA registers */
  u32 phyAddr;      /* PHY device address on MII interface */
  DmaDesc *tx;      /* start of TX descriptors ring */
  DmaDesc *rx;      /* start of RX descriptors ring */
  u32  rxCount;     /* number of rx descriptors */
  u32  txCount;     /* number of tx descriptors */
  u32  txBusy;      /* first descriptor owned by DMA engine, moved by DmaTxGet */
  u32  txNext;      /* next available tx descriptor, moved by DmaTxSet */
  s32  txFreeCount;      /* tx desc free count */

  u32  rxBusy;      /* first descripror owned by DMA engine, moved by DmaRxGet */
  u32  rxNext;      /* next available rx descriptor, moved by DmaRxSet */
  int advertising;
  int full_duplex;          /* interface is in full-duplex mode */
  int speed_100;    /* interface operates on high speed (100Mb/s) */
  int speed_10;   /* interface operates on high speed (10Mb/s) */
  struct net_device_stats stats;
} Mmac;

typedef struct PrivateStruct  /* Driver private data */
{
//  Mmac mmac;            /* mmac device internal data */
//  DmaDesc  desc[512];         /* DMA descriptors - 128 rx and 128 tx */
  DmaDesc  desc[64];         /* DMA descriptors - 16 rx and 16 tx */
  Mmac mmac;            /* mmac device internal data */
  //struct mii_if_info mii;
  spinlock_t lock;
  struct timer_list	timer;
} Private;

#include "mmac_reg.h" /* System-dependent register access functions */

/**********************************************************
 * board cfg related functions
 **********************************************************/
void hwcfgsetup(void);
void hw_reset(void);


/**********************************************************
 * Common functions
 **********************************************************/

int MmacInit        /* Initialise device data */
(
  Mmac *Dev,          /* Device structure, must be allocated by caller */
  u32 ConfigBase,         /* Base address of Configuration registers */
  u32 MacBase,            /* Base address of MAC registers */
  u32 DmaBase,            /* Base address of DMA registers */
  u32 PhyAddr             /* PHY device address */
);

/**********************************************************
 * MAC module functions
 **********************************************************/
void MmacMMCInit( Mmac *Dev);
void MmacMacReset(Mmac *Dev);
int MmacMacInit   /* Initialize MAC module  - set MAC and broadcast adresses, set filtering mode */
(
  Mmac *Dev,        /* Device */
  u8 Addr[6],          /* MAC address */
  u8 Broadcast[6]      /* Broadcast address */
);
void MmacPassAllPacket( Mmac *Dev);
void MmacPassAllMulticast( Mmac *Dev);
void MmacPassMatchPacket(struct net_device *dev, Mmac *mmac);

u16 MmacMiiRead   /* Read MII register */
(
  Mmac *Dev,        /* Device */
  u8 Reg                /* MII register */
);

void MmacMiiWrite /* Write MII register */
(
  Mmac *Dev,        /* Device */
  u8 Reg,               /* MII register */
  u16 Data              /* Data to write */
);

/**********************************************************
 * MII functions
 **********************************************************/
int check_link (struct net_device *dev, Mmac *mmac);

 void check_media(struct net_device *dev, Private *pr, Mmac *mmac);

/**********************************************************
 * DMA engine functions
 **********************************************************/
void   RxDescInit(struct net_device *dev, Private *pr, Mmac *mmac );
void DescTake( DmaDesc *Desc );   /* Take ownership */

int MmacDmaInit   /* Initialize DMA engine  - setup descriptor rings and control registers */
(
  Mmac *Dev,        /* Device */
  void *Buffer,         /* Buffer for DMA descriptors */
  u32 Size              /* Buffer length */
);

int MmacDmaRxSet  /* Set rx descriptor Desc and pass ownership to DMA engine */
(                     /* returns descriptor number if ok, -1 if next descriptor is busy (owned by DMA) */
  Mmac *Dev,        /* Device */
  u32   Buffer1,        /* First buffer address to set to descriptor */
  u32   Length1,        /* First buffer length */
  u32   Data1           /* Driver's data to store into descriptor */
);

int MmacDmaTxSet  /* Set tx descriptor Desc and pass ownership to DMA engine */
(                     /* returns descriptor number if ok, -1 if next descriptor is busy (owned by DMA) */
  Mmac *Dev,        /* Device */
  u32   Buffer1,        /* First buffer address to set to descriptor */
  u32   Length1,        /* First buffer length */
  u32   Data1           /* Driver's data to store into descriptor */
);

int MmacDmaRxGet /* take ownership of the rx descriptor Desc and clear it, returns old data */
(                    /* returns descriptor number if ok, -1 if next descriptor is busy (owned by DMA) */
  Mmac *Dev,        /* Device */
  u32  *Status,         /* Descriptor status */
  u32  *Buffer1,        /* First buffer address to set to descriptor */
  u32  *Length1,        /* First buffer length */
  u32  *Data1           /* Driver's data to store into descriptor */
);

int MmacDmaTxGet  /* take ownership of the rx descriptor Desc and clear it, returns old data */
(                     /* returns descriptor number if ok, -1 if next descriptor is busy (owned by DMA) */
  Mmac *Dev,        /* Device */
  u32  *Status,         /* Descriptor status */
  u32  *Buffer1,        /* First buffer address to set to descriptor */
  u32  *Length1,        /* First buffer length */
  u32  *Data1           /* Driver's data to store into descriptor */
);

u32 MmacDmaRxLength   /* extracts length from the Dma descriptor status word */
(
  u32 Status
);

int MmacDmaRxValid   /* Test the status word if the descriptor is valid */
(
  u32 Status
);

int MmacDmaRxCollisions
(
  u32 Status
);

int MmacDmaRxCrc
(
  u32 Status
);

int MmacDmaTxValid   /* Test the status word if the descriptor is valid */
(
  u32 Status
);

int MmacDmaTxCollisions
(
  u32 Status
);

int MmacDmaTxAborted
(
  u32 Status
);

int MmacDmaTxCarrier
(
  u32 Status
);

void MmacDmaRxStart      /* Start DMA receiver */
(
  Mmac *Dev         /* Device */
);

void MmacDmaRxStop      /* Stop DMA receiver */
(
  Mmac *Dev         /* Device */
);

void MmacDmaRxResume    /* Resume DMA receiver after suspend */
(
  Mmac *Dev         /* Device */
);

void MmacDmaTxStart      /* Start DMA transmitter */
(
  Mmac *Dev         /* Device */
);

void MmacDmaTxStop      /* Stop DMA transmitter */
(
  Mmac *Dev         /* Device */
);

void MmacDmaTxResume    /* Resume DMA transmitter after suspend */
(
  Mmac *Dev         /* Device */
);

/* L&T */

int MmacDmaRxFrame    
(
  u32 Status
);

int MmacDmaRxLengthError    
(
  u32 Status
);

/**********************************************************
 * DMA engine interrupt handling functions
 **********************************************************/

enum MmacDmaIntEnum  /* Intrerrupt types */
{
  MmacDmaRxNormal   = 0x01,   /* normal receiver interrupt */
  MmacDmaRxAbnormal = 0x02,   /* abnormal receiver interrupt */
  MmacDmaRxStopped  = 0x04,   /* receiver stopped */
  MmacDmaTxNormal   = 0x08,   /* normal transmitter interrupt */
  MmacDmaTxAbnormal = 0x10,   /* abnormal transmitter interrupt */
  MmacDmaTxStopped  = 0x20,   /* transmitter stopped */
  MmacDmaError      = 0x80,   /* Dma engine error */
};

void MmacDmaIntEnable /* Enable DMA engine intrerrupts */
(
  Mmac *Dev         /* Device */
);

void MmacDmaIntDisable /* Disable DMA engine intrerrupts */
(
  Mmac *Dev         /* Device */
);

void MmacDmaIntClear /* Clear DMA engine interrupt requests */
(
  Mmac *Dev         /* Device */
);

u32 MmacDmaIntType
(
  Mmac *Dev         /* Device */
);

void MmacDmaDescSync( Mmac *Dev, void *vaddr, size_t size, int direction );


#endif /* MMAC_INCLUDED */

