/* Mmac PCI/Ethernet test chip driver for OS Linux
 *
 * System-dependent register access functions
 *
 * 1 Jun 1999 - G.Kazakov.
 */

#ifndef MMAC_REG_INCLUDED
#define MMAC_REG_INCLUDED

#include <asm/io.h>
#if defined (CONFIG_ARCH_MT5391)
#include <mach/mt5391.h>
#elif defined (CONFIG_ARCH_MT5363)
#include <mach/mt5363.h>
#elif defined (CONFIG_ARCH_MT5395)
#include <mach/mt5395.h>
#else /* other platform */
#error "Error. No ARCH_MTXXXX been defined!"
#endif

//mtkmac add
//#define IO_BASE_MAC                        (0x20000000)
#define ETHERNET_BASE                     (ETHER_VIRT)//(IO_BASE_MAC + 0x32000) 
#define MAC_REG_OFFSET                  (0x0000)  
#define MMC_REG_OFFSET                  (0x0100)  
#define DMA_REG_OFFSET                  (0x0200)  
#define MAC_BASE                             (ETHERNET_BASE + MAC_REG_OFFSET)  //0x20032000
#define MMC_BASE                             (ETHERNET_BASE + MMC_REG_OFFSET) //0x20032100
#define DMA_BASE                             (ETHERNET_BASE + DMA_REG_OFFSET) //0x20032200

#define ETHERNET_LENGTH     				(0x0300)
#define MAC_LENGTH     					(0x0100)
#define DMA_LENGTH     					(0x0100)
#define ETH_IRQ                                        (8)

static u32 __inline__ MmacReadMacReg( Mmac *Dev, u32 Reg )
{
  u32 data = ioread32(Dev->macBase+Reg);
  //TR( KERN_DEBUG "MmacReadMacReg(%02x)=%08x\n", Reg, data );
  return data;
}

static void __inline__ MmacWriteMacReg( Mmac *Dev, u32 Reg, u32 Data )
{
  //TR( KERN_DEBUG "MmacWriteMacReg(%02x,%08x)\n", Reg, Data );
  iowrite32(Data,Dev->macBase+Reg);
}

static void __inline__ MmacSetMacReg( Mmac *Dev, u32 Reg, u32 Data )
{
  u32 addr = Dev->macBase+Reg;
  u32 data = ioread32(addr);
  data |= Data;
  //TR( KERN_DEBUG "MmacSetMacReg(%02x,%08x)=%08x\n", Reg, Data, data );
  iowrite32(data,addr);
}

static void __inline__ MmacClearMacReg( Mmac *Dev, u32 Reg, u32 Data )
{
  u32 addr = Dev->macBase+Reg;
  u32 data = ioread32(addr);
  data &= ~Data;
  //TR( KERN_DEBUG "MmacClearMacReg(%02x,%08x)=%08x\n", Reg, Data, data );
  iowrite32(data,addr);
}

static u32 __inline__ MmacReadMmcReg( Mmac *Dev, u32 Reg )
{
  u32 data = ioread32(Dev->mmcBase+Reg);
  //TR( KERN_DEBUG "MmacReadMmcReg(%02x)=%08x\n", Reg, data );
  return data;
}

static void __inline__ MmacWriteMmcReg( Mmac *Dev, u32 Reg, u32 Data )
{
  //TR( KERN_DEBUG "MmacWriteMmcReg(%02x,%08x)\n", Reg, Data );
  iowrite32(Data,Dev->mmcBase+Reg);
}

static u32 __inline__ MmacReadDmaReg( Mmac *Dev, u32 Reg )
{
  u32 data = ioread32(Dev->dmaBase+Reg);
  //TR( KERN_DEBUG "MmacReadDmaReg(%02x)=%08x\n", Reg, data );
  return data;
}

static void __inline__ MmacWriteDmaReg( Mmac *Dev, u32 Reg, u32 Data )
{
  //TR( KERN_DEBUG "MmacWriteDmaReg(%02x,%08x)\n", Reg, Data );
  iowrite32(Data,Dev->dmaBase+Reg);
}

static void __inline__ MmacSetDmaReg( Mmac *Dev, u32 Reg, u32 Data )
{
  u32 addr = Dev->dmaBase+Reg;
  u32 data = ioread32(addr);
  data |= Data;
  //TR( KERN_DEBUG "MmacSetDmaReg(%02x,%08x)=%08x\n", Reg, Data, data );
  iowrite32(data,addr);
}

static void __inline__ MmacClearDmaReg( Mmac *Dev, u32 Reg, u32 Data )
{
  u32 addr = Dev->dmaBase+Reg;
  u32 data = ioread32(addr);
  data &= ~Data;
  //TR( KERN_DEBUG "MmacClearDmaReg(%02x,%08x)=%08x\n", Reg, Data, data );
  iowrite32(data,addr);
}


#endif /* MMAC_REG_INCLUDED */

