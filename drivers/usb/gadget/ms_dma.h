#ifndef _MS_DMA_H_
#define _MS_DMA_H_


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#if defined( CONFIG_ARM )
#include <mach/hardware.h>
#include <asm/mach-types.h>
#endif
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>

#include <asm/byteorder.h>
#include <asm/dma.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/unaligned.h>

#include <linux/usb.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>

#include "msb250x_udc.h"
#include "msb250x_udc_reg.h"

//#define otgRegAddress OTG0_BASE_ADDR
//#define OffShift CPU_OFF_SHIFT


#include <mstar/mstar_chip.h>

#define _MSTAR_USB_BASEADR 0xfd200000

#define MIU0_BUS_BASE_ADDR 	MSTAR_MIU0_BUS_BASE
#define MIU1_BUS_BASE_ADDR	MSTAR_MIU1_BUS_BASE
#if defined(MSTAR_MIU2_BUS_BASE)
#define MIU2_BUS_BASE_ADDR	MSTAR_MIU2_BUS_BASE
#endif

#if defined(CONFIG_MSTAR_EAGLE)
#define MIU0_PHY_BASE_ADDR	((unsigned long)0x00000000)
/* MIU0 512M*/
#define MIU0_SIZE		((unsigned long)0x20000000)

#define MIU1_PHY_BASE_ADDR	((unsigned long)0x20000000)
/* MIU1 512M*/
#define MIU1_SIZE		((unsigned long)0x20000000)
#elif defined(CONFIG_MSTAR_AMBER3) || \
		defined(CONFIG_MSTAR_AGATE) || \
		defined(CONFIG_MSTAR_EDISON)
#define MIU0_PHY_BASE_ADDR	((unsigned long)0x00000000)
/* MIU0 1.5G*/
#define MIU0_SIZE		((unsigned long)0x60000000)

#define MIU1_PHY_BASE_ADDR	((unsigned long)0x80000000)
/* MIU1 1G*/
#define MIU1_SIZE		((unsigned long)0x40000000)
#elif defined(CONFIG_MSTAR_NIKE) || \
	defined(CONFIG_MSTAR_MADISON)
#define MIU0_PHY_BASE_ADDR	((unsigned long)0x00000000)
/* MIU0 1G*/
#define MIU0_SIZE		((unsigned long)0x40000000)

#define MIU1_PHY_BASE_ADDR	((unsigned long)0x40000000)
/* MIU1 1G*/
#define MIU1_SIZE		((unsigned long)0x40000000)
#elif defined(CONFIG_MSTAR_MONACO)
#define MIU0_PHY_BASE_ADDR	((unsigned long)0x00000000)
/* MIU0 2G*/
#define MIU0_SIZE		((unsigned long)0x80000000)

#define MIU1_PHY_BASE_ADDR	((unsigned long)0x80000000)
/* MIU1 1G*/
#define MIU1_SIZE		((unsigned long)0x40000000)

#define MIU2_PHY_BASE_ADDR	((unsigned long)0xC0000000)
/* MIU2 512M*/
#define MIU2_SIZE		((unsigned long)0x20000000)

#define ENABLE_USB_NEW_MIU_SLE	1
#define USB_MIU_SEL0	0x70	
#define USB_MIU_SEL1	0xB8
#define USB_MIU_SEL2	0xDC
#define USB_MIU_SEL3	0xFF
#elif defined(CONFIG_MSTAR_MIAMI)
#define MIU0_PHY_BASE_ADDR	((unsigned long)0x00000000)
/* MIU0 2G*/
#define MIU0_SIZE		((unsigned long)0x80000000)

#define MIU1_PHY_BASE_ADDR	((unsigned long)0x80000000)
/* MIU1 1G*/
#define MIU1_SIZE		((unsigned long)0x40000000)
		
#define ENABLE_USB_NEW_MIU_SLE	1
#define USB_MIU_SEL0	0x70	
#define USB_MIU_SEL1	0xB8
#define USB_MIU_SEL2	0xFF
#define USB_MIU_SEL3	0xFF	
#else
#define MIU0_PHY_BASE_ADDR	((unsigned long)0x00000000)
/* MIU0 2G*/
#define MIU0_SIZE		((unsigned long)0x80000000)

#define MIU1_PHY_BASE_ADDR	((unsigned long)0x80000000)
/* MIU1 1G*/
#define MIU1_SIZE		((unsigned long)0x40000000)
#endif

#if defined(CONFIG_MSTAR_MONACO) || \
    defined(CONFIG_MSTAR_MIAMI)
#define MIU_FAIL_ADDR 0xFFFFFFFF

#define IN_MIU_BUS(A, NUM)	\
	(((A)>=MIU##NUM##_BUS_BASE_ADDR)&&((A)<(MIU##NUM##_BUS_BASE_ADDR+MIU##NUM##_SIZE)))

#define IN_MIU_PA(A, NUM)	\
	(((A)>=MIU##NUM##_PHY_BASE_ADDR)&&((A)<(MIU##NUM##_PHY_BASE_ADDR+MIU##NUM##_SIZE)))

#define PA_MIU(A, NUM)	\
	((A)-MIU##NUM##_BUS_BASE_ADDR+MIU##NUM##_PHY_BASE_ADDR)

#define BUS_MIU(A, NUM)	\
	((A)-MIU##NUM##_PHY_BASE_ADDR+MIU##NUM##_BUS_BASE_ADDR)

#if defined(MIU2_BUS_BASE_ADDR)
#define BUS2PA(A)	\
	(IN_MIU_BUS(A, 0) ? PA_MIU(A, 0) : (IN_MIU_BUS(A, 1)? PA_MIU(A, 1): (IN_MIU_BUS(A, 2)? PA_MIU(A, 2) : MIU_FAIL_ADDR)))

#define PA2BUS(A)	\
	(IN_MIU_PA(A, 0) ? BUS_MIU(A, 0) : (IN_MIU_PA(A, 1)? BUS_MIU(A, 1): (IN_MIU_PA(A, 2)? BUS_MIU(A, 2) : MIU_FAIL_ADDR)))
#else
#define BUS2PA(A)	\
	(IN_MIU_BUS(A, 0) ? PA_MIU(A, 0) : (IN_MIU_BUS(A, 1)? PA_MIU(A, 1): MIU_FAIL_ADDR))

#define PA2BUS(A)	\
	(IN_MIU_PA(A, 0) ? BUS_MIU(A, 0) : (IN_MIU_PA(A, 1)? BUS_MIU(A, 1): MIU_FAIL_ADDR))
#endif
#else
#define BUS2PA(A)	\
	((((A)>=MIU0_BUS_BASE_ADDR)&&((A)<(MIU0_BUS_BASE_ADDR+MIU0_SIZE)))?	\
		((A)-MIU0_BUS_BASE_ADDR+MIU0_PHY_BASE_ADDR):	\
		((((A)>= MIU1_BUS_BASE_ADDR)&&((A)<MIU1_BUS_BASE_ADDR+MIU1_SIZE))?	\
			((A)-MIU1_BUS_BASE_ADDR+MIU1_PHY_BASE_ADDR):	\
			(0xFFFFFFFF)))

#define PA2BUS(A)	\
	((((A)>=MIU0_PHY_BASE_ADDR)&&((A)<(MIU0_PHY_BASE_ADDR+MIU0_SIZE)))?	\
		((A)-MIU0_PHY_BASE_ADDR+MIU0_BUS_BASE_ADDR):	\
		((((A)>= MIU1_PHY_BASE_ADDR)&&((A)<MIU1_PHY_BASE_ADDR+MIU1_SIZE))?	\
			((A)-MIU1_PHY_BASE_ADDR+MIU1_BUS_BASE_ADDR):	\
			(0xFFFFFFFF)))
#endif

#define DMA_ADDR_INVALID (~(dma_addr_t)0)
#define RX_DMA_CHANNEL  1
#define TX_DMA_CHANNEL  2

#define MIN_DMA_TRANSFER_BYTES       256

#define M_REG_DMA_INTR               /*IO_ADDRESS*/(OTG0_BASE_ADDR) + (0x200<<OffShift)

#define DMA_BASE_ADDRESS()           /*IO_ADDRESS*/(OTG0_BASE_ADDR) + (0x200<<OffShift)


#define DMA_CNTL_REGISTER(channel)  (u16 volatile*)(DMA_BASE_ADDRESS() + ((0x10 * (channel - 1) + 4)<<OffShift))
#define DMA_ADDR_REGISTER(channel)  (u32 volatile*)(DMA_BASE_ADDRESS() + ((0x10 * (channel - 1) + 8)<<OffShift))
#define DMA_COUNT_REGISTER(channel) (u32 volatile*)(DMA_BASE_ADDRESS() + ((0x10 * (channel - 1) + 0xc)<<OffShift))


#define DMA_TX                     0x2
#define DMA_RX                     0x0

#define DMA_MODE_ZERO              0x0
#define DMA_MODE_ONE               0x4

#define DMA_IRQ_ENABLE             0x8
#define DMA_IRQ_DISABLE            0x0

#define DMA_MODE_MASK              (DMA_TX | DMA_MODE_ONE)

#define DMA_TX_ZERO_IRQ       (DMA_TX | DMA_MODE_ZERO | DMA_IRQ_ENABLE)
#define DMA_RX_ZERO_IRQ       (DMA_RX | DMA_MODE_ZERO | DMA_IRQ_ENABLE)

#define DMA_TX_ONE_IRQ       (DMA_TX | DMA_MODE_ONE | DMA_IRQ_ENABLE)
#define DMA_RX_ONE_IRQ       (DMA_RX | DMA_MODE_ONE | DMA_IRQ_ENABLE)

#define DMA_BurstMode        0x03

#define RXCSR2_MODE1  (MSB250X_UDC_RXCSR2_AUTOCLR | MSB250X_UDC_RXCSR2_DMAREQEN | MSB250X_UDC_RXCSR2_DMAREQMD)
#define TXCSR2_MODE1  (MSB250X_UDC_TXCSR2_DMAREQENAB | MSB250X_UDC_TXCSR2_AUTOSET | MSB250X_UDC_TXCSR2_DMAREQMODE)

#define M_Mode1_P_BulkOut_EP   0x0002
#define M_Mode1_P_BulkOut_EP_4    0x0004
#define M_Mode1_P_OK2Rcv   0x8000
#define M_Mode1_P_AllowAck   0x4000
#define M_Mode1_P_Enable    0x2000
#define M_Model_P_NAK	0x2000

#define DMA_ENABLE_BIT          0x0001
#define DMA_BUSERROR_BIT        0x0100
#define DMA_ENDPOINT_SHIFT           4

#define EP_IRQ_ENABLE                1
#define EP_IRQ_DISABLE               0
#define EP_IRQ_RX                    0
#define EP_IRQ_TX                    2

#define Enable_TX_EP_Interrupt(endpoint) \
      Control_EP_Interrupt(endpoint, (EP_IRQ_ENABLE | EP_IRQ_TX))

#define Enable_RX_EP_Interrupt(endpoint) \
      Control_EP_Interrupt(endpoint, (EP_IRQ_ENABLE | EP_IRQ_RX))

#define Disable_TX_EP_Interrupt(endpoint) \
      Control_EP_Interrupt(endpoint, (EP_IRQ_DISABLE | EP_IRQ_TX))

#define Disable_RX_EP_Interrupt(endpoint) \
      Control_EP_Interrupt(endpoint, (EP_IRQ_DISABLE | EP_IRQ_RX))

#define DMA_BUSY 1
#define DMA_NOT_BUSY 0

enum DMA_RX_MODE_TYPE
{
    DMA_RX_MODE1,
    DMA_RX_MODE0,
    DMA_RX_MODE_NULL,
};

void USB_DMA_IRQ_Handler(u8 ch, struct msb250x_udc *dev);
s8 USB_Set_DMA(struct usb_ep *_ep, struct msb250x_request *req, u32 count, u32 mode);
void Control_EP_Interrupt(s8 ep, u32 mode);
void USB_EnableDMA(void);
void USB_DisableDMA(void);
void USB_Set_ClrRXMode1(void);
u16 USB_Read_DMA_Control(s8);
s8 check_dma_busy(void);

extern void msb250x_udc_done(struct msb250x_ep *ep, struct msb250x_request *req, int status);
enum DMA_RX_MODE_TYPE msb250_udc_set_dma_rx_by_name(const char * dev_name);
#endif
