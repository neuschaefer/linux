/*-----------------------------------------------------------------------------
 *
 * Licensed under the GPL
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html#TOC1
 *
 *---------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: mtk_hcd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file mtk_hcd.c
 *  This C file implements the mtk53xx USB host controller driver.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/usb.h>
#include <linux/kthread.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/unaligned.h>
#include <asm/uaccess.h>
#include <linux/dma-mapping.h>
#include <linux/usb/hcd.h>

#include "mtk_hcd.h"
#include "mtk_qmu_api.h"

MODULE_DESCRIPTION("MTK 5396 USB Host Controller Driver");
MODULE_LICENSE("GPL");

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#define DRIVER_VERSION    "01 Sep 2011"

//#define MUC_DMA_DISABLE   // Disable DMA support.
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define EPRX (0)
#define EPTX (1)
#define EP0 (0)
#define EPMSD (0)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static void MGC_ServiceDefaultEnd(MGC_LinuxCd *pThis);
static void MGC_ServiceTxAvail(MGC_LinuxCd *pThis, uint8_t bEnd);
static void MGC_ServiceRxReady(MGC_LinuxCd *pThis, uint8_t bEnd);

static irqreturn_t MUC_Irq(struct usb_hcd *hcd);
static int MUC_start(struct usb_hcd *hcd);
static void MUC_stop(struct usb_hcd *hcd);
static int MUC_urb_enqueue(struct usb_hcd *hcd, struct urb *pUrb, gfp_t mem_flags);
static int MUC_urb_dequeue(struct usb_hcd *hcd, struct urb *pUrb, int status);
static void MUC_endpoint_disable(struct usb_hcd *hcd, struct usb_host_endpoint *hep);
static int MUC_get_frame(struct usb_hcd *hcd);
static int MUC_hub_status_data(struct usb_hcd *hcd, char *buf);
static int MUC_hub_control(struct usb_hcd *hcd, uint16_t typeReq, uint16_t wValue, uint16_t wIndex, char *buf, uint16_t wLength);
static int MUC_bus_suspend(struct usb_hcd *hcd);
static int MUC_bus_resume(struct usb_hcd *hcd);
static void MUC_hub_descriptor(MGC_LinuxCd *pThis, struct usb_hub_descriptor *desc);

static int MUC_hcd_probe(struct platform_device *pdev);
static int MUC_hcd_remove(struct platform_device *pdev);
static int MUC_hcd_suspend(struct platform_device *pdev, pm_message_t state);
static int MUC_hcd_resume(struct platform_device *pdev);

#ifdef MUSB_DEBUG
    int MGC_DebugLevel = 3;
    int MGC_DebugDisable = 0;
#endif 

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static const char MUC_HcdName[] = "MtkUsbHcd";

static struct platform_driver MUC_hcd_driver = 
{
    .probe = MUC_hcd_probe, 
    .remove = MUC_hcd_remove, 
    .suspend = MUC_hcd_suspend, 
    .resume = MUC_hcd_resume, 
    .driver = 
    {
        .name = (char*)MUC_HcdName, 
        .owner = THIS_MODULE, 
    } , 
};

static struct hc_driver MUC_hc_driver = 
{
    .description = MUC_HcdName, 
    .hcd_priv_size = sizeof(MGC_LinuxCd), 

    /*
     * generic hardware linkage
     */
    .irq = MUC_Irq, 
    .flags = HCD_USB2 | HCD_MEMORY, 

    /* Basic lifecycle operations */
    .start = MUC_start, 
    .stop = MUC_stop, 

    /*
     * managing i/o requests and associated device resources
     */
    .urb_enqueue = MUC_urb_enqueue, 
    .urb_dequeue = MUC_urb_dequeue, 
    .endpoint_disable = MUC_endpoint_disable, 

    /*
     * periodic schedule support
     */
    .get_frame_number = MUC_get_frame, 

    /*
     * root hub support
     */
    .hub_status_data = MUC_hub_status_data, 
    .hub_control = MUC_hub_control, 
    .bus_suspend = MUC_bus_suspend, 
    .bus_resume = MUC_bus_resume, 
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static inline MGC_LinuxCd *hcd_to_musbstruct(struct usb_hcd *hcd)
{
    return (MGC_LinuxCd*)(hcd->hcd_priv);
} 

static inline struct usb_hcd *musbstruct_to_hcd(const MGC_LinuxCd *pThis)
{
    return container_of((void*)pThis, struct usb_hcd, hcd_priv);
} 

static void MGC_VbusControl(unsigned long pParam)
{
    MGC_LinuxCd *pThis = (MGC_LinuxCd*)pParam;

    // Turn on vbus.
    pThis->MGC_pfVbusControl(pThis->bPortNum, 1);
}

static char *MGC_DecodeUrbProtocol(struct urb *pUrb)
{
    static char buffer[8];

    if (!pUrb)
    {
        strcpy(&buffer[0], "NULL");

        return buffer;
    } 

    buffer[0] = usb_pipein(pUrb->pipe) ? 'I' : 'O';

    if (usb_pipeint(pUrb->pipe))
    {
        strcpy(&buffer[1], " int");
    }
    else if (usb_pipeisoc(pUrb->pipe))
    {
        strcpy(&buffer[1], " isoc");
    }
    else if (usb_pipebulk(pUrb->pipe))
    {
        strcpy(&buffer[1], " bulk");
    }
    else if (usb_pipecontrol(pUrb->pipe))
    {
        strcpy(&buffer[0], " ctl");
    }

    return buffer;
}

int MGC_CheckTxCsrValidForWrite(const void *pBase, uint8_t bEnd)
{
    uint32_t dwCtrl;
    uint8_t bChannel;

    // Assert when this condition happens :  Write EP TxCSR when the EP is doing DMA. 
    if (bEnd)
    {
        for (bChannel = 0; bChannel < MGC_HSDMA_CHANNELS; bChannel++)
        {
            dwCtrl = MGC_Read32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL));
            if ((((dwCtrl &0xf0) >> 4) == bEnd) && ((dwCtrl&0x3) == 0x3))
            {
                printk("[USB] MGC_CheckTxCsrValidForWrite check error ! bEnd=%x, TxCSR=0x%x !\n", bEnd, dwCtrl);
                while(1);
                return 0;
            }
        }
    }

    return 1;
}

int MGC_IsEndIdle(MGC_LinuxLocalEnd *pEnd)
{
    if (pEnd->pCurrentUrb)
    {
        return FALSE;
    }

    return list_empty(&pEnd->list);
}

struct urb *MGC_GetNextUrb(MGC_LinuxLocalEnd *pEnd)
{
    MGC_LinuxUrbList *pUrbList;
    struct urb *pUrb = NULL;

    if (pEnd->pCurrentUrb)
    {
        //leon 20110607 cam9q issue
        //MUSB_ASSERT(list_empty(&pEnd->list) == TRUE);
        pUrb = pEnd->pCurrentUrb;
    } 
    else
    {
        MUSB_ASSERT(list_empty(&pEnd->list) == FALSE);
        pUrbList = list_entry(pEnd->list.next, MGC_LinuxUrbList, list);
        if (pUrbList)
        {            
            pUrb = pUrbList->pUrb;
            if (pUrb)
            {                
                pEnd->pCurrentUrb = pUrb;                        
            }        
            list_del(&pUrbList->list);
            kfree (pUrbList);            
        }
    }

    /*
    if (pUrb)
    {
        printk("[USB]Next pUrb=0x%08X, size=%d.\n", (uint32_t)pUrb, pUrb->transfer_buffer_length);
    }
    else
    {
        printk("[USB]Next pUrb=NULL.\n");
    }
     */

    return pUrb;
}

struct urb *MGC_GetCurrentUrb(MGC_LinuxLocalEnd *pEnd)
{
    return pEnd->pCurrentUrb;
} 

static void MGC_ClearEnd(MGC_LinuxLocalEnd *pEnd)
{
    pEnd->dwOffset = 0;
    pEnd->dwRequestSize = 0;
    pEnd->dwIsoPacket = 0;
    pEnd->bRetries = 0;
    pEnd->bTrafficType = 0;
}

static int MGC_EnqueueEndUrb(MGC_LinuxCd *pThis, MGC_LinuxLocalEnd *pEnd, struct urb *pUrb)
{
    MGC_LinuxUrbList *pUrbList;
    int rc;

    rc = usb_hcd_link_urb_to_ep(musbstruct_to_hcd(pThis), pUrb);
    if (rc) 
    {
        return rc;
    }

    //printk("Enqueue: pUrb=0x%08X.\n", (uint32_t)pUrb);
   
    if ((pEnd->pCurrentUrb == NULL) && (list_empty(&pEnd->list) == TRUE))
    {
        // Only put pUrb to pCurrentUrb when pCurrentUrb and list are both empty.
        pEnd->pCurrentUrb = pUrb;
    } 
    else
    {
        pUrbList = kmalloc(sizeof(MGC_LinuxUrbList), GFP_ATOMIC); 
        if (!pUrbList)
        {        
            return  - ENOMEM;
        }

        pUrbList->pUrb = pUrb;
        list_add_tail(&pUrbList->list, &pEnd->list);
    }
    // Add pUrb to pEnd structure.
    pUrb->hcpriv = pEnd;

    return 0;
}

void MGC_DequeueEndurb(MGC_LinuxCd *pThis, MGC_LinuxLocalEnd *pEnd, struct urb *pUrb)
{
    MGC_LinuxUrbList *pUrbList;
    MGC_LinuxUrbList *pNextUrbList;

    //printk("Dequeue: pUrb=0x%08X.\n", (uint32_t)pUrb);

    // Remove pUrb to pEnd structure.
    pUrb->hcpriv = NULL;

    // Dequeue urb from it's endpoint list.
    usb_hcd_unlink_urb_from_ep(musbstruct_to_hcd(pThis), pUrb);   

    if (pEnd->pCurrentUrb == pUrb)
    {
        pEnd->pCurrentUrb = NULL;
        MGC_ClearEnd(pEnd);
    } 
    else
    {
        list_for_each_entry_safe(pUrbList, pNextUrbList, &pEnd->list, list)
        {
            if (pUrbList->pUrb == pUrb)
            {
                list_del(&pUrbList->list);
                kfree(pUrbList);
                break;
            }
        }
    }
}

static void MGC_InitEnd(MGC_LinuxCd *pThis, uint8_t bEndpoint_num)
{
    uint8_t bEnd;
    uint8_t bTx;
    MGC_LinuxLocalEnd *pEnd;
    void *pBase = pThis->pRegs;
   
    /* use the defined end points */
    pThis->bEndCount = bEndpoint_num;
    if (pThis->bEndCount > MUSB_C_NUM_EPS)
    {
        printk("Endpoint array out of boundary.\n");
        return;
    }

    for (bEnd=0; bEnd<pThis->bEndCount; bEnd++)
    {        
        MGC_SelectEnd(pBase, bEnd);
        MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 3);  // 64 bytes.
        MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 3);                
        MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, 0);
        MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, 0);

        pThis->wEndMask |= (1 << bEnd);
        for (bTx = EPRX; bTx <= EPTX; bTx++)
        {
            pEnd = &(pThis->aLocalEnd[bTx][bEnd]);
            pEnd->bEnd = bEnd;
            pEnd->bIsTx = bTx;
            pEnd->wPacketSize = 0;
            
            if (bEnd==0)
            {
                pEnd->wFifoAddress = 0;
                pEnd->wMaxPacketSize = MGC_END0_FIFOSIZE;
                pEnd->bDoublePacketBuffer = FALSE;
            }
            else if (bEnd == MGC_END_MSD)
            {
                /* Reserve EP for mass storage with 1024 bytes double packet fifo size.*/
                pEnd->wFifoAddress = MGC_END_MSD_FIFOADDR; 
                pEnd->wMaxPacketSize = 512;
                pEnd->bDoublePacketBuffer = TRUE;
                pEnd->bStateGated = TRUE;
            }
            else
            {
                pEnd->wFifoAddress = MGC_END0_FIFOSIZE; 
                pEnd->wMaxPacketSize = 0;  /* EPx is not occupied */
                pEnd->bDoublePacketBuffer = FALSE;                
            }
            
            pEnd->pCurrentUrb = NULL;
            INIT_LIST_HEAD(&pEnd->list);
            pEnd->wPacketSize = 0;
            pEnd->bRemoteAddress = 0;
            pEnd->bRemoteEnd = 0;
            pEnd->bTrafficType = 0;
        }        
    }
}

static void MGC_DelTimer(MGC_LinuxCd *pThis)
{
    if ( timer_pending(&pThis->Timer) )
        del_timer_sync((&pThis->Timer)); /* make sure another timer is not running */
}

static void MGC_SetTimer(MGC_LinuxCd *pThis, void(*pfFunc)(unsigned long), 
                         uint32_t pParam, uint32_t millisecs)
{
    if ( timer_pending(&pThis->Timer) )
        del_timer_sync((&pThis->Timer)); /* make sure another timer is not running */

    pThis->Timer.function = pfFunc;
    pThis->Timer.data = (unsigned long)pParam;
    pThis->Timer.expires = jiffies + (HZ *millisecs) / 1000;
    add_timer(&(pThis->Timer));
}

static void MGC_StartTx(MGC_LinuxCd *pThis, uint8_t bEnd)
{
    uint8_t *pBase = (uint8_t*)pThis->pRegs;

    MGC_CheckTxCsrValidForWrite(pBase, bEnd);

    if (bEnd)
    {
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_TXPKTRDY);
    }
    else
    {
        if (MGC_END0_START == pThis->bEnd0Stage)
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, 
                MGC_M_CSR0_H_NO_PING | MGC_M_CSR0_H_SETUPPKT | MGC_M_CSR0_TXPKTRDY);
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_TXPKTRDY);
        }        
    }
}

static uint32_t MGC_Log2(uint32_t x)
{
    uint32_t i;

    for (i = 0; x > 1; i++)
    {
        x = x / 2;
    }

    return i;
}

static uint16_t MGC_SelectFifoSize(uint16_t wPacketSize)
{
    /* Setting    Fifo size
        4'b0000: 8
        4'b0001: 16
        4'b0010: 32
        4'b0011: 64
        4'b0100: 128
        4'b0101: 256
        4'b0110: 512
        4'b0111: 1024
        4'b1000: 2048 (Single-packet buffering only)
        4'b1001: 4096 (Single-packet buffering only)
    */
    if (wPacketSize <= 8)
    {
        return 0;
    }
    else if ((wPacketSize > 8) && (wPacketSize <= 16))
    {
        return 1;
    }
    else if ((wPacketSize > 16) && (wPacketSize <= 32))
    {
        return 2;
    }
    else if ((wPacketSize > 32) && (wPacketSize <= 64))
    {
        return 3;
    }
    else if ((wPacketSize > 64) && (wPacketSize <= 128))
    {
        return 4;
    }
    else if ((wPacketSize > 128) && (wPacketSize <= 256))
    {
        return 5;
    }
    else if ((wPacketSize > 256) && (wPacketSize <= 512))
    {
        return 6;
    }
    else if ((wPacketSize > 512) && (wPacketSize <= 1024))
    {
        return 7;
    }
    else if ((wPacketSize > 1024) && (wPacketSize <= 2048))
    {
        return 8;
    }
    else // if (wPacketSize > 2048)
    {
        return 9;
    }
}

static MGC_LinuxLocalEnd *MGC_FindEnd(MGC_LinuxCd *pThis, struct urb *pUrb)
{
    void *pBase = pThis->pRegs;
    MGC_LinuxLocalEnd *pEnd = NULL;    
    //MGC_LinuxLocalEnd *pDbgEnd = NULL;        
    unsigned int nOut = usb_pipeout(pUrb->pipe);
    uint16_t wPacketSize = usb_maxpacket(pUrb->dev, pUrb->pipe, nOut);
    struct usb_device *dev = pUrb->dev;
    struct usb_host_endpoint *ep = pUrb->ep;
    struct usb_device_descriptor *pDescriptor = &dev->descriptor;
    struct usb_interface_descriptor *d;
    uint8_t bAddTransaction;   
    uint16_t wStartAddr = MGC_END0_FIFOSIZE;
    uint16_t wEndAddr = 0;
    uint16_t wSize = 0;
    uint16_t i, j;
    uint8_t bIndex;
    uint8_t bEnd = 0;
        
    DBG(2, "[USB]<== pUrb=%p\n", pUrb);

    /* control is always EP0, and can always be queued */
    if (usb_pipecontrol(pUrb->pipe))
    {
        DBG(2, "[USB]==> is a control pipe use ep0\n");

        pEnd = &(pThis->aLocalEnd[EP0][0]); /*0: Rx, 1: Tx*/
        return pEnd;
    } 

    if (ep->hcpriv)
    {
        pEnd = (MGC_LinuxLocalEnd*)ep->hcpriv;
        return pEnd;
    }

    // Handle high speed isochronous transfer, multiple packet part.
    bAddTransaction = (wPacketSize >> 11) & 0x3;
    wPacketSize &= 0x7ff;
    if (bAddTransaction == 1)
    {
        wPacketSize = 2*wPacketSize;
    }
    else if (bAddTransaction == 2)
    {
        wPacketSize = 3*wPacketSize;
    }

    wPacketSize = (wPacketSize < 8) ? 8: wPacketSize;

    printk("[USB] Port-%d: New Dev=0x%X, proto=%s, wPacketSize=%d.\n", 
        pThis->bPortNum, (uint32_t)dev, MGC_DecodeUrbProtocol(pUrb), wPacketSize);
    printk("[USB] Port-%d: idVendor=0x%04X, idProduct=0x%04X, bcdDevice=0x%04X.\n", 
        pThis->bPortNum, (uint32_t)pDescriptor->idVendor, 
        (uint32_t)pDescriptor->idProduct, (uint32_t)pDescriptor->bcdDevice);

    if (usb_pipeisoc(pUrb->pipe) && (!pThis->bSupportCmdQ))
    {
        printk("[USB] Port-%d: This port do not support isochronous transfer !\n", pThis->bPortNum);
        ASSERT(usb_pipeisoc(pUrb->pipe) && (!pThis->bSupportCmdQ));
    }

    if (MUSB_C_NUM_EPS < pThis->bEndCount)
    {
        printk("[USB] port endpoint out of boundary =%d.\n", pThis->bEndCount);        
        return NULL;
    }
    
    // Check free fifo. EP1 is for mass storage and share the same fifo with 1024 bytes.
    wSize = 0;
    for (i=1; i<pThis->bEndCount; i++)
    {
        for (j=0; j<2; j++)
        {
            pEnd = &(pThis->aLocalEnd[j][i]); /*0: Rx, 1: Tx*/ 
            wSize += pEnd->wMaxPacketSize;                      
        }
    }
    
    if ((wSize + wPacketSize) > MGC_ENDX_FIFOSIZE)
    {
        printk("[USB] Fifo not enough !.\n");
        return NULL;
    }
    else
    {
        printk("[USB] Ep request size = %d bytes, Fifo left = %d bytes.\n", 
            wPacketSize, (MGC_ENDX_FIFOSIZE - wSize));
    }

    d = &dev->actconfig->interface[0]->cur_altsetting->desc;

    /* use a reserved one for bulk if any */
    if (usb_pipebulk(pUrb->pipe))
    {        
        if ( (pDescriptor->bDeviceClass == USB_CLASS_MASS_STORAGE) || 
             (d->bInterfaceClass == USB_CLASS_MASS_STORAGE) )
        {
            /*
                Endpoint use for mass storage device.
                We will turn on double packet buffer to enhance performance.
            */
            bEnd = MGC_END_MSD;            
            
            /*
                NOTICE:
                Because we use shared double packet buffer for mass storage.
                It can not let tx/rx traffic happend at the same time.
                So we need to queue all urb (tx/rx) on the same list.
                It will prevent tx/rx data corruption when tx/rx transfer at the same time.
                Always use EPMSD direction of pEnd for mass storage.
            */
            pEnd = &(pThis->aLocalEnd[EPMSD][bEnd]);
            
            ep->hcpriv = (void*)pEnd;                                
            // Mass storage can use the same tx/rx fifo address.
            wStartAddr = pEnd->wFifoAddress;
            goto SetFifo;   
        }
    }

    // No endpoint left for this. Possible interrupt endpoint. Search again without check command queue.
    for (i=1; ((i<pThis->bEndCount) && (bEnd == 0)); i++)  /* EP1 is reserve for mass storage */
    {
        if (i == MGC_END_MSD)
        {
            continue;
        }
        pEnd = &(pThis->aLocalEnd[nOut][i]); /*0: Rx, 1: Tx*/ 

        if (pEnd->wMaxPacketSize ==  0)
        {        
            bEnd = i;
            //printk("[USB] %s Ep %d is free for use.\n", ((nOut) ? "Tx" : "Rx"), bEnd);            
            break;
        }
    }

    if (bEnd == 0)
    {
        printk("[USB] No endpoint left.\n");
        return NULL;
    }

    // Search the start address of free fifo.
    wStartAddr = MGC_END0_FIFOSIZE;
    
search_again:    
    
    for (i=1; i<pThis->bEndCount; i++)
    {
        if (i == MGC_END_MSD)
        {
            continue;
        }

        for (j=0; j<2; j++)
        {
            pEnd = &(pThis->aLocalEnd[j][i]); /*0: Rx, 1: Tx*/ 

            // Avoid to use the pEnd->wFifoAddress.
            if ((pEnd->wMaxPacketSize >  0) && (wStartAddr == pEnd->wFifoAddress))
            {
                wStartAddr = pEnd->wFifoAddress + pEnd->wMaxPacketSize;
                /* 
                    We need to check again if (wStartAddr == pEnd->wFifoAddress), 
                    because we allocate fifo not in sequence.
                */
                goto search_again;
            }            
        }
    }

    printk("[USB] %s Ep %d prepare to use fifo at %d.\n", ((nOut) ? "Tx" : "Rx"), bEnd, wStartAddr);
    
    // Check if free fifo start address + wPacketSize do not overlap other fifo.
    wEndAddr = wStartAddr + wPacketSize -1;
    
    for (i=1; i<pThis->bEndCount; i++)
    {
        if (i == MGC_END_MSD)
        {
            continue;
        }
    
        for (j=0; j<2; j++)
        {
            pEnd = &(pThis->aLocalEnd[j][i]); /*0: Rx, 1: Tx*/ 
            
            if ((pEnd->wFifoAddress <= wEndAddr) && 
                ((pEnd->wFifoAddress +pEnd->wMaxPacketSize) > wEndAddr))
            {
                printk("[USB] %s Ep %d  use fifo %d ~ %d, overlay fifo at %d ~ %d.\n", 
                    ((nOut) ? "Tx" : "Rx"), bEnd, wStartAddr, wEndAddr, 
                    pEnd->wFifoAddress, (pEnd->wFifoAddress +pEnd->wMaxPacketSize-1));            

                wStartAddr = pEnd->wFifoAddress + pEnd->wMaxPacketSize;
                goto search_again;                
            }            
        }
    }

    // Check if wStartAddr and wEndAddr is reasonable ?
    if (wEndAddr >= MGC_END_MSD_FIFOADDR)
    {
        printk("[USB] Out of fifo boundary.\n");
        return NULL;
    }
    
    // Occupy this fifo address.
    pEnd = &(pThis->aLocalEnd[nOut][bEnd]); /*0: Rx, 1: Tx*/ 
    pEnd->wFifoAddress = wStartAddr;
    pEnd->wMaxPacketSize = wPacketSize;

    ep->hcpriv = (void*)pEnd;

SetFifo:
    wSize = MGC_SelectFifoSize(wPacketSize);
            
    /* save index */
    bIndex = MGC_Read8(pBase, MGC_O_HDRC_INDEX);
    MGC_SelectEnd(pBase, bEnd);
    if (nOut)
    {
        wSize = (pEnd->bDoublePacketBuffer) ? (wSize | 0x10) : wSize;
        MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, wSize);
        MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, wStartAddr >> 3);
    }
    else
    {
        wSize = (pEnd->bDoublePacketBuffer) ? (wSize | 0x10) : wSize;
        MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, wSize);
        MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, wStartAddr >> 3);        
    }

    /* restore index */
    MGC_Write8(pBase, MGC_O_HDRC_INDEX, bIndex);

#if 0
    // Debug information for all fifo allocation.    
    for (i=1; i<pThis->bEndCount; i++)
    {
        for (j=0; j<2; j++)
        {
            if (i < MUSB_C_NUM_EPS)
            {
                pDbgEnd = &(pThis->aLocalEnd[j][i]); /*0: Rx, 1: Tx*/ 
                printk("[USB] Port-%d: %s Ep %d use fifo at %d, size=%d, DB=%d.\n", 
                    pThis->bPortNum, ((j) ? "Tx" : "Rx"), i, 
                    pDbgEnd->wFifoAddress, pDbgEnd->wMaxPacketSize, 
                    pDbgEnd->bDoublePacketBuffer);
            }        
        }
    }
#endif
    return pEnd;
}

void MGC_CallbackUrb(MGC_LinuxCd *pThis, struct urb *pUrb)
{
    if (pUrb->status)
    {
        DBG(1, "[USB]Err: urb=0x%p,status=%d\n", pUrb, pUrb->status);
    } 

#ifdef MUC_DMA_DISABLE
    if (usb_pipein(pUrb->pipe) && usb_pipetype(pUrb ->pipe) != PIPE_CONTROL) {
        void *ptr;
        for (ptr = pUrb ->transfer_buffer;
             ptr < pUrb ->transfer_buffer + pUrb ->transfer_buffer_length;
             ptr += PAGE_SIZE)
                flush_dcache_page(virt_to_page(ptr));
    }
#endif

    usb_hcd_giveback_urb(musbstruct_to_hcd(pThis), pUrb, pUrb->status);

    if (pUrb->status)
    {
        DBG(1, "[USB]done completing pUrb=%p\n", pUrb);
    }
}

void MGC_CompleteUrb(MGC_LinuxCd *pThis, MGC_LinuxLocalEnd *pEnd, struct urb *pUrb)
{
    DBG(2, "[USB]Completing URB=0x%p, on pEnd->bEnd=%d status=%d, proto=%s\n", pUrb, pEnd->bEnd, pUrb->status, MGC_DecodeUrbProtocol(pUrb));

    //printk("[USB] Complete pUrb=0x%X.\n", (uint32_t)pUrb);
    if (pUrb->status)
    {
        DBG(1, "[USB]completing URB=%p, status=%d, len=%x\n", pUrb, pUrb->status, pUrb->actual_length);
    }

    MGC_CallbackUrb(pThis, pUrb);
} 

static inline uint8_t MGC_GetTransferType(struct urb *pUrb)
{
    uint8_t bStdType = 0;

    const unsigned int nPipe = pUrb->pipe;

    if (usb_pipeisoc(nPipe))
    {
        bStdType = 1;
    } 
    else if (usb_pipeint(nPipe))
    {
        bStdType = 3;
    }
    else if (usb_pipebulk(nPipe))
    {
        bStdType = 2;
    }

    return bStdType;
}

static void MGC_SetProtocol(MGC_LinuxCd *pThis, struct urb *pUrb, uint8_t bEnd, unsigned int bXmt)
{
    uint8_t reg;

    uint8_t bStdType = MGC_GetTransferType(pUrb);
    unsigned int nPipe = pUrb->pipe;
    uint8_t *pBase = (uint8_t*)pThis->pRegs;

    reg = (bStdType << 4) | (((uint8_t)usb_pipeendpoint(nPipe)) &0xf);

    switch (((uint8_t)pUrb->dev->speed))
    {
        case USB_SPEED_LOW:
            reg |= 0xc0;
            break;

        case USB_SPEED_FULL:
            reg |= 0x80;
            break;

        default:
            reg |= 0x40;
    }

    if (bXmt)
    {
        if (bEnd)
        {
            MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, reg);
        }
        else
        {
            MGC_WriteCsr8(pBase, MGC_O_HDRC_TYPE0, 0, reg &0xc0);
        }
    }
    else
    {
        if (bEnd)
        {
            // Note that although we invoke WriteCsr8, but it actually write 32bits.
            // Writing to MGC_O_HDRC_RXTYPE will actually affect MGC_O_HDRC_RXINTERVAL.
            // Therefore, here we have a check to avoid iso transfer interval be affected.
            if (usb_pipeisoc(nPipe))
            {
                uint8_t regvalue;
                regvalue = MGC_ReadCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd);
                if ((regvalue | 0xC0) != (reg | 0xC0))
                {
                    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, reg);
                }
            }
            else
            {
                MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, reg);
            }
        }
        else
        {
            MGC_WriteCsr8(pBase, MGC_O_HDRC_TYPE0, 0, reg &0xc0);
        }
    }
}

static void MGC_SetAddress(MGC_LinuxCd *pThis, struct urb *pUrb, uint8_t bEnd, unsigned int bXmt)
{
    uint8_t *pBase = (uint8_t*)pThis->pRegs;

    uint8_t bAddress = (uint8_t)usb_pipedevice(pUrb->pipe);
    uint8_t bHubAddr = 0, bHubPort = 0;

    /* NOTE: there is always a parent due to the virtual root hub */
    if (pUrb->dev->parent)
    {
        bHubAddr |= (uint8_t)pUrb->dev->parent->devnum;
    } 
	
    /*  HS MTT Hub + Low/Full Speed Device */
    if (pUrb->dev->tt)
    {
        if(pUrb->dev->tt->multi)
        {
            bHubAddr |= 0x80;
            bHubPort = (uint8_t)pUrb->dev->ttport;
        }
    }
    
    /* target addr & hub addr/port */
    if (bXmt)
    {
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXFUNCADDR), bAddress);
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBADDR), bHubAddr);
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBPORT), bHubPort);
    }

    /* also, try Rx (this is a bug ion the core: I always need to to do 
     * both (at least for ep0), needs to be changed when the core is
     * fixed */
    if ((bEnd == 0) || (!bXmt))
    {
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXFUNCADDR), bAddress);
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBADDR), bHubAddr);
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBPORT), bHubPort);
    }

    DBG(2, "[USB]end %d, device %d, parent %d, port %d, speed:%d\n", bEnd, pUrb->dev->devnum, bHubAddr, bHubPort, pUrb->dev->speed);
}

static void MGC_LoadFifo(const uint8_t *pBase, uint8_t bEnd, uint16_t wCount, const uint8_t *pSource)
{    
    uint32_t dwDatum = 0;

    uint32_t dwCount = wCount;
    uint8_t bFifoOffset = MGC_FIFO_OFFSET(bEnd);
    uint32_t dwBufSize = 4;

    //printk("[USB]pBase=%p, bEnd=%d, wCount=0x%04x, pSrc=%p\n", pBase, bEnd, wCount, pSource);

    //  do not handle zero length data.
    if (dwCount == 0)
    {
        return ;
    }

    /* byte access for unaligned */
    if ((dwCount > 0) && ((uint32_t)pSource &3))
    {
        while (dwCount)
        {
            if (3 == dwCount || 2 == dwCount)
            {
                dwBufSize = 2;             
                // set FIFO byte count.
                MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 1);
            }
            else if(1 == dwCount)
            {
                dwBufSize = 1;             
                // set FIFO byte count.
                MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 0);               
            }

            memcpy((void *)&dwDatum, (const void *)pSource, dwBufSize);

            MGC_Write32(pBase, bFifoOffset, dwDatum);

            dwCount -= dwBufSize;
            pSource += dwBufSize;
        }
    }
    else /* word access if aligned */
    {
        while ((dwCount > 3) && !((uint32_t)pSource &3))
        {
            MGC_Write32(pBase, bFifoOffset, *((uint32_t*)((void*)pSource)));

            pSource += 4;
            dwCount -= 4;
        }
        if (3 == dwCount || 2 == dwCount)
        {
            MUSB_ASSERT(dwCount < 4);
    
            // set FIFO byte count.
            MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 1);
    
            MGC_Write32(pBase, bFifoOffset, *((uint32_t *)((void *)pSource)));
            dwCount -= 2;
            pSource += 2;
        }
        
        if(1 == dwCount)
        {
            MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 0);
            if((uint32_t)pSource & 3)
            {
            
                memcpy((void *)&dwDatum, (const void *)pSource, 1);
                
                MGC_Write32(pBase, bFifoOffset, dwDatum);            
            }
            else
            {
                MGC_Write32(pBase, bFifoOffset, *((uint32_t *)((void *)pSource)));            
            }
        }
    }
    MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 2);
    return ;
}

void MGC_UnloadFifo(const uint8_t *pBase, uint8_t bEnd, uint16_t wCount, uint8_t *pDest)
{   
    uint32_t dwDatum = 0;

    uint32_t dwCount = wCount;
    uint8_t bFifoOffset = MGC_FIFO_OFFSET(bEnd);
    uint32_t i;

    // printk("[USB]pBase=%p, bEnd=%d, wCount=0x%04x, pDest=%p\n", pBase, bEnd, wCount, pDest);

    //  do not handle zero length data.
    if (dwCount == 0)
    {
        return ;
    }

    if (((uint32_t)pDest) &3)
    {
        /* byte access for unaligned */
        while (dwCount > 0)
        {
            if (dwCount < 4)
            {            
                if(3 == dwCount || 2 == dwCount)
                {
                   MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 1);
                   dwDatum = MGC_Read32(pBase, bFifoOffset);
                   
                   for (i = 0; i < 2; i++)
                   {
                       *pDest++ = ((dwDatum >> (i *8)) & 0xFF);
                   }                   
                   dwCount -=2;
                }
                if(1 == dwCount) 
                {
                    MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 0);
                    dwDatum = MGC_Read32(pBase, bFifoOffset);
                    *pDest++ = (dwDatum  & 0xFF);
                    dwCount -= 1;
                }    
                
                // set FIFO byte count = 4 bytes.
                MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 2);
                dwCount = 0;
            }
            else
            {
                dwDatum = MGC_Read32(pBase, bFifoOffset);

                for (i = 0; i < 4; i++)
                {
                    *pDest++ = ((dwDatum >> (i *8)) &0xFF);
                }

                dwCount -= 4;
            }
        }
    }
    else
    {
        /* word access if aligned */
        while (dwCount >= 4)
        {
            *((uint32_t*)((void*)pDest)) = MGC_Read32(pBase, bFifoOffset);

            pDest += 4;
            dwCount -= 4;
        }

        if (dwCount > 0)
        {    
            if(3 == dwCount ||2 == dwCount )
            {
                MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 1);
                dwDatum = MGC_Read32(pBase, bFifoOffset);
                for (i = 0; i < 2; i++)
                 {
                     *pDest++ = ((dwDatum >> (i *8)) & 0xFF);
                 }
                dwCount -= 2;
            }

            if(1 == dwCount)
            {
               MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT,0);
               dwDatum = MGC_Read32(pBase, bFifoOffset);
               
                *pDest++ = (dwDatum & 0xFF);
                dwCount -= 1;               
            }
                
            // set FIFO byte count = 4 bytes.
            MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 2);
        }
    }

    return ;
}

void MGC_CleanDcache(struct urb* urb,uint32_t start,uint32_t len)
{
    uint32_t phy;
    
    if(!(urb->transfer_flags & URB_NO_TRANSFER_DMA_MAP) && virt_addr_valid(start))
    {  
        phy = dma_map_single(urb->dev->bus->controller,(void *)start, len, DMA_TO_DEVICE);
        dma_unmap_single(urb->dev->bus->controller,(dma_addr_t)phy, len, DMA_TO_DEVICE);             
    }
}

static void MGC_SetInterval(MGC_LinuxCd *pThis, struct urb *pUrb, uint8_t bEnd)
{
    uint8_t *pBase = (uint8_t*)pThis->pRegs;

    unsigned int nPipe = pUrb->pipe;
    uint8_t bInterval = 0;

    if (usb_pipeint(nPipe))
    {
        if (pThis->bRootSpeed == 1) /* high speed */
        {
            bInterval = MGC_Log2(pUrb->interval) + 1;
        } 
        else /* full or low speed */
        {
            bInterval = pUrb->interval;
        }
    }
    else if (usb_pipeisoc(nPipe))
    {
        bInterval = MGC_Log2(pUrb->interval) + 1;
    }
    else if (usb_pipebulk(nPipe) && (pUrb->interval > 0))
    {
        if (pThis->bRootSpeed == 1) /* high speed */
        {
            if (pUrb->interval > 4096)
            {
                bInterval = 16;
            }
            else
            {
                bInterval = MGC_Log2(pUrb->interval) + 1;
            }
        }
        else if (pThis->bRootSpeed == 2) /* full speed */
        {
            if (pUrb->interval > 32768)
            {
                bInterval = 16;
            }
            else
            {
                bInterval = MGC_Log2(pUrb->interval) + 1;
            }
        }
    }

    if (usb_pipeout(nPipe))
    {    
        if(MGC_ReadCsr8(pBase, MGC_O_HDRC_TXINTERVAL, bEnd) != bInterval)
        {
            MGC_WriteCsr8(pBase, MGC_O_HDRC_TXINTERVAL, bEnd, bInterval);
        }
    }
    else
    {
        // Everytime when we write to MGC_O_HDRC_RXINTERVAL, there would be 1 frame/micro-frame delay.
        // MGC_O_HDRC_RXINTERVAL should be written only when we want to change interval.
        if (MGC_ReadCsr8(pBase, MGC_O_HDRC_RXINTERVAL, bEnd) != bInterval)
        {
            MGC_WriteCsr8(pBase, MGC_O_HDRC_RXINTERVAL, bEnd, bInterval);
        }
    }
}

/**
 * used ONLY in host mode, I'll be moved to musb_host
 * @param pPrivateData
 * @param bLocalEnd
 * @param bTransmit
 */
static uint8_t MGC_HsDmaChannelStatusChanged(MGC_LinuxCd *pThis, 
    uint8_t bLocalEnd, uint8_t bTransmit)
{
    if (!bLocalEnd)
    {
        MGC_ServiceDefaultEnd(pThis);
    }
    else
    {
        /* endpoints 1..15 */
        if (bTransmit)
        {
            MGC_ServiceTxAvail(pThis, bLocalEnd);
        }
        else
        {
            /* receive */
            MGC_ServiceRxReady(pThis, bLocalEnd);
        }
    }

    return TRUE;
}

static MGC_HsDmaChannel *MGC_HsDmaAllocateChannel(
    MGC_HsDmaController *pController, uint8_t bLocalEnd, uint8_t bTransmit, 
    uint32_t dwLength, uint8_t bProtocol, uint16_t wMaxPacketSize)
{
    MGC_HsDmaChannel *pImplChannel = NULL;
#ifndef MUC_DMA_DISABLE       
    uint8_t bBit;
    MGC_LinuxCd *pThis = (MGC_LinuxCd *)pController->pThis;

/*
    Note 5396 limitation problem:
    (1) Channel B do not support DMA. 
    (2) Tx command queue h/w bug. 
    If channel B is used, we need to use cpu read/write fifo.
    And must turn off Tx command queue support.
    After 5398, This condition can be removed.
*/
#if defined(CONFIG_ARCH_MT5396) && defined(CONFIG_MT53XX_USE_CHANNELB_DRAM)       
    if (dwLength <= MGC_HSDMA_MIN_DMA_LEN)
    {
        return NULL;
    }       
#else
    /*
        If command queue use, all transfer must use DMA to workaround Tx commad queue bug.
        If this port do not support command queue, we only use DMA at size > MGC_HSDMA_MIN_DMA_LEN.
    */    
    if ((!pThis->bSupportCmdQ) && (dwLength <= MGC_HSDMA_MIN_DMA_LEN))
    {
        return NULL;
    }       
#endif

    for (bBit = 0; bBit < pController->bChannelCount; bBit++)
    {
        if (!(pController->bmUsedChannels & (1 << bBit)))
        {
            pController->bmUsedChannels |= (1 << bBit);

            pImplChannel = &(pController->aChannel[bBit]);
            pImplChannel->pController = (void *)pController;
            pImplChannel->dwStartAddress = 0;
            pImplChannel->dwCount = 0;
            pImplChannel->dwActualLength = 0;
            pImplChannel->wMaxPacketSize = wMaxPacketSize;
            pImplChannel->bStatus = MGC_DMA_STATUS_FREE;
            pImplChannel->bDesiredMode = 0;
            pImplChannel->bIndex = bBit;
            pImplChannel->bEnd = bLocalEnd;
            pImplChannel->bProtocol = bProtocol;
            pImplChannel->bTransmit = bTransmit;
            break;
        }
    }

    if ((!pImplChannel) && (pThis->bSupportCmdQ))
    {
        printk("[USB] No DMA. bEnd=%d, bProtocol=%d, bTransmit=%d.\n", 
            bLocalEnd, bProtocol, bTransmit);
        while(1);
    }
#endif

    return pImplChannel;
}

static void MGC_HsDmaReleaseChannel(MGC_HsDmaChannel *pChannel)
{
    MGC_HsDmaController *pController = (MGC_HsDmaController *)pChannel->pController;
    MGC_LinuxCd *pThis = (MGC_LinuxCd *)pController->pThis;
    uint8_t *pBase = (uint8_t*)pThis->pRegs;
    uint8_t bEnd = pChannel->bEnd;
    uint8_t bChannel = pChannel->bIndex;
    uint16_t wCsr = 0; 
    uint32_t bIntr;
    
    bIntr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);
    if (bIntr & (1<< bChannel))
    {
        //  write clear interrupt register value.
        MGC_Write8(pBase, MGC_O_HSDMA_INTR, (1 << bChannel));
    }

    MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL), 0);

    // clear TX/RX CSR register.
    if (pChannel->bTransmit)
    {
        wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);

        if (wCsr &(MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE))
        {
            wCsr &= ~(MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE);

            MGC_CheckTxCsrValidForWrite(pBase, bEnd);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr);
        }
    }
    else
    {
        wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);

        if (wCsr &(MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_H_AUTOREQ | 
                MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE))
        {
            wCsr &= ~(MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_H_AUTOREQ | 
                MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE);

            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wCsr);
        }
    }

    pController->bmUsedChannels &= ~(1 << bChannel);
    pChannel->bStatus = MGC_DMA_STATUS_FREE;
}

static uint8_t MGC_HsDmaProgramChannel(MGC_HsDmaChannel *pChannel)
{
    MGC_HsDmaController *pController = (MGC_HsDmaController *)pChannel->pController;
    MGC_LinuxCd *pThis = (MGC_LinuxCd *)pController->pThis;
    uint8_t *pBase = (uint8_t*)pThis->pRegs;
    uint8_t bEnd = pChannel->bEnd;
    uint8_t bChannel = pChannel->bIndex;
    uint16_t wMaxPacketSize = pChannel->wMaxPacketSize;
    uint8_t bMode = pChannel->bDesiredMode;
    uint8_t *pBuffer = (uint8_t *)pChannel->dwStartAddress;
    uint32_t dwLength = pChannel->dwCount;
    uint32_t dwCsr = (bEnd << MGC_S_HSDMA_ENDPOINT) | (1 << MGC_S_HSDMA_ENABLE);
    uint16_t wCsr = 0;

    if ((uint32_t)(pBuffer)+ dwLength >= 0x20000000)
    {
        printk("[USB]MGC_HsDmaProgramChannel error, incorrect address 0x%p %d\n", pBuffer, dwLength);
    }

    if (bMode)
    {
        dwCsr |= 1 << MGC_S_HSDMA_MODE1;
    }

    dwCsr |= MGC_M_HSDMA_BURSTMODE;
    
    if (pChannel->bTransmit)
    {   
        //  prevent client task and USB HISR race condition, set csr in MGC_HsDmaProgramChannel().
        wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);

        if (bMode)
        {            
            wCsr |= (MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE);
        }
        else
        {
            wCsr &= ~(MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE);
        }

        MGC_CheckTxCsrValidForWrite(pBase, bEnd);
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr);
        dwCsr |= 1 << MGC_S_HSDMA_TRANSMIT;
    }
    else
    {
        if (bMode)
        {
            wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);

            wCsr |= (MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_DMAENAB);
            //  Request the actual number of packet to be received.
            MGC_DMA_Write32(pBase, M_REG_REQPKT(bEnd), (((dwLength + wMaxPacketSize) - 1) / wMaxPacketSize));

            wCsr &= ~MGC_M_RXCSR_RXPKTRDY;
            // host use MGC_M_RXCSR_DMAMODE.
            wCsr |= (MGC_M_RXCSR_H_AUTOREQ | MGC_M_RXCSR_DMAMODE | MGC_M_RXCSR_H_REQPKT);
        }
    }

    dwCsr |= 1 << MGC_S_HSDMA_IRQENABLE;

    /* address/count */
    MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_ADDRESS), (uint32_t)pBuffer);
    MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_COUNT), dwLength);

    /* control (this should start things) */
    MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL), dwCsr);

    if (!pChannel->bTransmit)
    {
        /*
            Note: 
            RxCSR should be set after DMA is configured. 
            This can prevent race condition between setuping DMA and data entering fifo.
             */
        if (bMode)
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wCsr);
        }
    }

    return TRUE;
}

static MGC_DmaChannelStatus MGC_HsDmaGetChannelStatus(
    MGC_HsDmaChannel *pChannel)
{
    MGC_HsDmaController *pController = (MGC_HsDmaController *)pChannel->pController;
    MGC_LinuxCd *pThis = (MGC_LinuxCd *)pController->pThis;
    uint8_t *pBase = (uint8_t*)pThis->pRegs;
    uint8_t bChannel = pChannel->bIndex;
    uint32_t dwAddress;
    uint32_t dwCsr = MGC_Read32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL));
    uint32_t dwCsrMask;

    if (dwCsr &(1 << MGC_S_HSDMA_BUSERROR))
    {
        printk("[USB]MGC_HsDmaGetChannelStatus : MGC_S_HSDMA_BUSERROR !\n");
        return MGC_DMA_STATUS_BUS_ABORT;
    }

    //  handle last short packet in multiple packet DMA RX mode 1.  
    dwCsrMask = (1 << MGC_S_HSDMA_ENABLE) | (1 << MGC_S_HSDMA_MODE1) | (1 << MGC_S_HSDMA_IRQENABLE);

    if ((dwCsr &0xf) == dwCsrMask)
    {
        /* most DMA controllers would update the count register for simplicity... */
        dwAddress = MGC_Read32(pBase, 
            MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_ADDRESS));

        pChannel->dwActualLength = dwAddress - pChannel->dwStartAddress;

        MGC_Write32(pBase, 
            MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL), 0);
        
        return MGC_DMA_STATUS_MODE1_SHORTPKT;
    }

    /* most DMA controllers would update the count register for simplicity... */
    dwAddress = MGC_Read32(pBase, 
        MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_ADDRESS));

    if (dwAddress < (pChannel->dwStartAddress + pChannel->dwCount))
    {
        printk("[USB]MGC_HsDmaGetChannelStatus : MGC_DMA_STATUS_BUSY !\n");
        printk("[USB] 0x%08X, 0x%08X, 0x%08X.\n", 
            dwAddress, pChannel->dwStartAddress, pChannel->dwCount);
        
        return MGC_DMA_STATUS_BUSY;
    }

    return MGC_DMA_STATUS_FREE;
}

static uint8_t MGC_HsDmaControllerIsr(MGC_HsDmaController *pController)
{
    MGC_LinuxCd *pThis = (MGC_LinuxCd *)pController->pThis;
    uint8_t *pBase = (uint8_t*)pThis->pRegs;
    MGC_HsDmaChannel *pChannel;
    uint8_t bChannel;
    uint32_t dwCsr;
    uint32_t dwAddress;
    uint8_t bIntr;
    uint8_t bIntrMask;
    uint8_t bEnd;    
    uint16_t wVal;
    MGC_LinuxLocalEnd *pEnd; 

    bIntr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);
    bIntrMask = MGC_Read8(pBase, MGC_O_HSDMA_INTR_MASK);
    bIntr = (bIntr & bIntrMask);
    if (!bIntr)
    {
        return FALSE;
    }
    
    MGC_Write8(pBase, MGC_O_HSDMA_INTR, bIntr);  

    for (bChannel = 0; bChannel < pController->bChannelCount; bChannel++)
    {    
        if ((bIntr &(1 << bChannel)) == 0)
        {        
            continue;        
        }
        
        pChannel = (MGC_HsDmaChannel*) &(pController->aChannel[bChannel]);
        bEnd = pChannel->bEnd;
        
        dwCsr = MGC_Read32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL));

        if (dwCsr &(1 << MGC_S_HSDMA_BUSERROR))
        {
            printk("[USB]MGC_HsDmaControllerIsr Bus ERROR !\n");
            pChannel->bStatus = MGC_DMA_STATUS_BUS_ABORT;
        }
        else
        {
            /* most DMA controllers would update the count register for simplicity... */
            dwAddress = MGC_Read32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_ADDRESS));

            pChannel->bStatus = MGC_DMA_STATUS_FREE;
            pChannel->dwActualLength = dwAddress - pChannel->dwStartAddress;

            if (pChannel->bTransmit)
            {
                /*
                    NOTICE:
                    Because we use shared double packet buffer for mass storage.
                    It can not let tx/rx traffic happend at the same time.
                    So we need to queue all urb (tx/rx) on the same list.
                    It will prevent tx/rx data corruption when tx/rx transfer at the same time.
                    Always use EPMSD direction of pEnd for mass storage.
                */
                pEnd = (MGC_END_MSD == bEnd) ? (&(pThis->aLocalEnd[EPMSD][bEnd])) : 
                    ((0 == bEnd) ? (&(pThis->aLocalEnd[EP0][0])) :  (&(pThis->aLocalEnd[EPTX][bEnd])));
                
                if (pEnd->bDoublePacketBuffer)
                {                
                    /* Notice: double packet buffer fifo need to check fifo empty */  
                    do
                    {                        
                        // Check again.
                        wVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);                   
                    }while (wVal & (MGC_M_TXCSR_FIFONOTEMPTY|MGC_M_TXCSR_TXPKTRDY));                    
                }
                       
                //  send last short packet in multiple packet transfer and tx single packet.
                if ( (pChannel->dwCount % pChannel->wMaxPacketSize) || 
                     (pChannel->dwCount <= pChannel->wMaxPacketSize) )
                {
                    MGC_StartTx(pThis, bEnd);
                    continue;
                }
            }

            MGC_HsDmaChannelStatusChanged(pThis, bEnd, pChannel->bTransmit);
        }
    }

    return TRUE;
}

static uint8_t MGC_TxPacket(MGC_LinuxCd *pThis, MGC_LinuxLocalEnd *pEnd)
{
    uint16_t wLength = 0;
    uint8_t bDone = FALSE;
    uint8_t *pBase;
    struct urb *pUrb;
    uint8_t *pBuffer;
    int nPipe;
    uint8_t bEnd;
    int i;
    struct usb_iso_packet_descriptor *packet;
    MGC_HsDmaChannel *pDmaChannel;
    uint8_t *pDmaBuffer;
    uint8_t bDmaOk;
    MGC_DmaChannelStatus DmaChannelStatus = MGC_DMA_STATUS_UNKNOWN;

    if ((!pThis) || (!pEnd))
    {
        return TRUE;
    } 

    pBase = (uint8_t*)pThis->pRegs;
    pUrb = MGC_GetCurrentUrb(pEnd);
    nPipe = pUrb->pipe;
    bEnd = pEnd->bEnd;
    pEnd->dwOffset += pEnd->dwRequestSize;

    DBG(2, "[USB]<== bEnd=%d\n", bEnd);

    /* see if more transactions are needed */
    if (pEnd->pDmaChannel)
    {
        pDmaChannel = pEnd->pDmaChannel;    
        DmaChannelStatus = MGC_HsDmaGetChannelStatus(pDmaChannel);
        MUSB_ASSERT(MGC_DMA_STATUS_FREE == DmaChannelStatus);

        if (usb_pipeisoc(nPipe))
        {
            packet = &pUrb->iso_frame_desc[pEnd->dwIsoPacket];                
            MUSB_ASSERT(pEnd->dwRequestSize >= packet->length);               
            packet->status = 0;
            packet->actual_length = packet->length;
            pEnd->dwIsoPacket++;
            packet ++;

            if (pEnd->dwIsoPacket < pUrb->number_of_packets)
            {
                pDmaBuffer = (uint8_t*)pUrb->transfer_dma;
                pDmaBuffer += packet->offset;
                wLength = packet->length;

                // Iso is always single packet to send.
                pDmaChannel->dwStartAddress = (uint32_t)pDmaBuffer;
                pDmaChannel->dwCount = (uint32_t)wLength;
                pDmaChannel->dwActualLength = 0;
                pDmaChannel->bDesiredMode = 0;
                pEnd->dwRequestSize = wLength;
                bDmaOk = MGC_HsDmaProgramChannel(pDmaChannel);
                if (!bDmaOk)
                {
                    MGC_HsDmaReleaseChannel(pDmaChannel);
                    pEnd->pDmaChannel = NULL;
                    pEnd->dwRequestSize = 0;
                }
                else
                {
                    // Wait DMA finish and set TXPKTRDY at DMA ISR.
                    return FALSE;
                }
            }
            else
            {
                // Urb finish.
                pUrb->status = 0;
                return TRUE;
            }
        }
    }

    if (usb_pipeisoc(nPipe))
    {
        /* isoch case */
        if (pEnd->dwIsoPacket >= pUrb->number_of_packets)
        {
            for (i = 0; i < pUrb->number_of_packets; i++)
            {
                packet = &pUrb->iso_frame_desc[i];
                packet->status = 0;
                packet->actual_length = packet->length;
            }

            bDone = TRUE;
        }
        else
        {
            pBuffer = pUrb->transfer_buffer;
            packet = &pUrb->iso_frame_desc[pEnd->dwIsoPacket];
            pBuffer += packet->offset;
            wLength = packet->length;
            pEnd->dwIsoPacket++;
        }
    }
    else
    {
        pBuffer = pUrb->transfer_buffer;    
        pBuffer += pEnd->dwOffset;

        wLength = min((int)pEnd->wPacketSize, (int)(pUrb->transfer_buffer_length - pEnd->dwOffset));

        if (pEnd->dwOffset >= pUrb->transfer_buffer_length)
        {
            /* sent everything; see if we need to send a null */
            if ((pUrb->transfer_flags &USB_ZERO_PACKET) && 
                 (pEnd->dwRequestSize > 0) && 
                 ((pEnd->dwRequestSize % pEnd->wPacketSize) == 0))
            {
                // send null packet.
                pEnd->dwRequestSize = 0;
                bDone = FALSE;
                printk("[USB]MGC_TxPacket: Send NULL Packet !\n");
            }
            else
            {
                bDone = TRUE;
            }
        }
    }

    if (bDone)
    {
        pUrb->status = 0;
    }
    else if (wLength)
    {
         /* @assert bDone && !wLength */
        MGC_LoadFifo(pBase, bEnd, wLength, pBuffer);
        pEnd->dwRequestSize = wLength;

        MGC_CheckTxCsrValidForWrite(pBase, bEnd);
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, (MGC_M_TXCSR_TXPKTRDY));        
    }
    else
    {
        printk("[USB]MGC_TxPacket error : 0x%x, 0x%x, 0x%x, 0x%x !\n", pEnd->wPacketSize, pUrb->transfer_buffer_length, pEnd->dwOffset, pEnd->dwRequestSize);
    }

    return bDone;
}

static uint8_t MGC_RxPacket(MGC_LinuxCd *pThis, MGC_LinuxLocalEnd *pEnd, uint16_t wRxCount)
{
    uint16_t wLength;
    uint8_t bDone = FALSE;
    uint8_t *pBase;
    uint8_t bEnd;
    struct urb *pUrb;
    uint8_t *pBuffer;
    int nPipe;
    uint16_t wPacketSize;
    struct usb_iso_packet_descriptor *packet;

    if ((!pThis) || (!pEnd))
    {
        return TRUE;
    } 

    pBase = (uint8_t*)pThis->pRegs;
    pUrb = MGC_GetCurrentUrb(pEnd);
    
    nPipe = pUrb->pipe;
    bEnd = pEnd->bEnd;
    pBuffer = pUrb->transfer_buffer;
    
    DBG(2, "[USB]<== end %d RxCount=%04x\n", bEnd, wRxCount);
    DBG(3, "[USB]bEnd=%d, pUrb->transfer_flags=0x%x pUrb->transfer_buffer=%p\n", bEnd, pUrb->transfer_flags, pUrb->transfer_buffer);
    DBG(3, "[USB]pUrb->transfer_buffer_length=%d, pEnd->dwOffset=%d, wRxCount=%d\n", pUrb->transfer_buffer_length, pEnd->dwOffset, wRxCount);

    /* unload FIFO */
    if (usb_pipeisoc(nPipe))
    {
        /* isoch case */
        packet = &pUrb->iso_frame_desc[pEnd->dwIsoPacket];
        pBuffer += packet->offset;
        wLength = min((unsigned int)wRxCount, packet->length);
        pUrb->actual_length += wLength;

        /* update actual & status */
        packet->actual_length = wLength;
        packet->status = USB_ST_NOERROR;

        /* see if we are done */
        bDone = (++pEnd->dwIsoPacket >= pUrb->number_of_packets);

        DBG(3, "[USB]pEnd->dwIsoPacket=%d, pUrb->number_of_packets=%d, wLength=%d\n", pEnd->dwIsoPacket, pUrb->number_of_packets, wLength);

        if (wLength)
        {
            MGC_UnloadFifo(pBase, bEnd, wLength, pBuffer);
            MGC_CleanDcache(pUrb,(uint32_t)pBuffer, (uint32_t)wLength);
        }

        if (bEnd && bDone)
        {
            pUrb->status = 0;
        }
    }
    else
    {
        DBG(3, "[USB](bEnd=%d), wRxCount=%d, pUrb->transfer_buffer_length=%d, pEnd->dwOffset=%d, pEnd->wPacketSize=%d\n", bEnd, wRxCount, pUrb->transfer_buffer_length, pEnd->dwOffset, pEnd->wPacketSize);

        /* non-isoch */
        pBuffer += pEnd->dwOffset;

        wLength = min((unsigned int)wRxCount, pUrb->transfer_buffer_length - pEnd->dwOffset);

        wPacketSize = usb_maxpacket(pUrb->dev, pUrb->pipe, FALSE);

        if (wLength > 0)
        {
            pUrb->actual_length += wLength;
            pEnd->dwOffset += wLength;

            MGC_UnloadFifo(pBase, bEnd, wLength, pBuffer);
	    MGC_CleanDcache(pUrb,(uint32_t)pBuffer, (uint32_t)wLength);
        }

        /* see if we are done */
        bDone = (pEnd->dwOffset >= pUrb->transfer_buffer_length) || (wRxCount < pEnd->wPacketSize);
    }

    MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 0);

    if (!bDone)
    {
         /* test for short packet */
        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_H_REQPKT);
    }

    if (bEnd && bDone)
    {
        pUrb->status = 0;
    }

    DBG(2, "[USB]==> bDone=%d\n", bDone);
    return bDone;
}

#ifdef CONFIG_USB_QMU_SUPPORT
static uint32_t MGC_IsCmdQSupport(MGC_LinuxCd *pThis, MGC_LinuxLocalEnd *pEnd, struct urb *pUrb)
{
    uint8_t bEnd;
    uint32_t nPipe;
    uint8_t bXmt;
    
    if ((!pThis)||(!pEnd) ||(!pUrb))
    {
        return 0;
    }

    bEnd = pEnd->bEnd;
    nPipe = pUrb->pipe;
    bXmt = usb_pipeout(nPipe);

    if (usb_pipecontrol(nPipe))
    {
        return 0;
    }    

/*
    Note 5396 limitation problem:
    (1) Channel B do not support DMA. 
    (2) Tx command queue h/w bug. 
    If channel B is used, we need to use cpu read/write fifo.
    And must turn off Tx command queue support.
    After 5398, This condition can be removed.
*/
#if defined(CONFIG_ARCH_MT5396) && defined(CONFIG_MT53XX_USE_CHANNELB_DRAM)
    if ((pThis->bSupportCmdQ) 
        && ((!bXmt) && (bEnd <= RXQ_NUM)))
    {
        return 1;
    }
#else             
    if ((pThis->bSupportCmdQ) 
        && ((bXmt) ? (bEnd <= TXQ_NUM): (bEnd <= RXQ_NUM)))
    {
        return 1;
    }
#endif

    return 0;
}

static uint32_t MGC_IsCmdQEnable(MGC_LinuxCd *pThis, MGC_LinuxLocalEnd *pEnd, struct urb *pUrb)
{
    uint8_t *pBase;
    uint8_t bEnd;
    uint32_t nPipe;
    uint8_t bXmt;
    uint8_t bRemoteAddress;

    if ((!pThis)||(!pEnd) ||(!pUrb))
    {
        return 0;
    }

    bEnd = pEnd->bEnd;
    nPipe = pUrb->pipe;
    bXmt = usb_pipeout(nPipe);

    if (!QMU_is_enabled(pThis, bEnd, !bXmt))
    {
        return 0;
    }

    pBase = (uint8_t*)pThis->pRegs;
    bRemoteAddress = (bXmt) ? 
        MGC_Read8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXFUNCADDR)) :
        MGC_Read8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXFUNCADDR));

    /* Make sure device attribute need to reset or not */
    if (pEnd->bRemoteAddress != bRemoteAddress)
    {
        return 0;
    }

    return 1;
}

static void MGC_KickCmdQ(MGC_LinuxCd *pThis, MGC_LinuxLocalEnd *pEnd, struct urb *pUrb)
{
    uint16_t wIntr;
    uint32_t dwIntr;
    uint8_t *pBase = (uint8_t*)pThis->pRegs;
    unsigned int nPipe = pUrb->pipe;
    uint16_t wPacketSize = usb_maxpacket(pUrb->dev, nPipe, usb_pipeout(nPipe));
    uint8_t bEnd = pEnd->bEnd;
    uint8_t bXmt = pEnd->bIsTx;
    uint8_t bIsZlp;
    int i;
    unsigned int offset;
    uint8_t bIsIoc;
    uint8_t *pBuffer;
    uint32_t dwLength;

    /* insert GPD! */
    if (!MGC_IsCmdQEnable(pThis, pEnd, pUrb))
    {
        /* remember software state - find_end() will use this - */
        pEnd->bRemoteAddress = (uint8_t)usb_pipedevice(nPipe);;
        pEnd->bRemoteEnd = (uint8_t)usb_pipeendpoint(nPipe);;
        pEnd->bTrafficType = (uint8_t)usb_pipetype(nPipe);

        /* init urb */
        pEnd->dwOffset = 0;
        pEnd->dwRequestSize = 0;
        pEnd->dwIsoPacket = 0;
        pEnd->bRetries = 0;
        pEnd->wPacketSize = wPacketSize;
        pEnd->bIsTx = bXmt;
    
        printk("[USB] CmdQ: ep%d, dir=%d is enabled.\n", bEnd, bXmt);
        
        MGC_SetProtocol(pThis, pUrb, bEnd, bXmt);
        MGC_SetAddress(pThis, pUrb, bEnd, bXmt);

        if (bXmt) /* transmit */
        {
            /*
                IntrTxE, and IntrRx are the same 32 bits group.
                Tricky: Set 0 in all write clear field in IntrRx field. Prevent to clear IntrRx.
                Because our 32 bits register access mode.
            */
            wIntr = MGC_Read16(pBase, MGC_O_HDRC_INTRTXE);
            dwIntr = (uint32_t)((wIntr & (~(uint16_t)(1 << bEnd))) << 16);
            dwIntr &= 0xFFFF0000;
            MGC_Write32(pBase, MGC_O_HDRC_INTRRX, dwIntr);

            /* twice in case of double packet buffering */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, 
                MGC_M_TXCSR_FLUSHFIFO |MGC_M_TXCSR_CLRDATATOG);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, 
                MGC_M_TXCSR_FLUSHFIFO |MGC_M_TXCSR_CLRDATATOG);

            /* protocol/endpoint/interval/NAKlimit */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, wPacketSize);

            //  Set interrupt polling interval must depend on high, or low/full speed.
            MGC_SetInterval(pThis, pUrb, bEnd);

            // Clean tx interrupt.
            wIntr = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
            MGC_Write16(pBase, MGC_O_HDRC_INTRTX, wIntr);

            // Cmd Queue: need to turn on DMAENAB.
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_DMAENAB);
        }
        else
        {
            // Disable endpoint interrupt when using command queue. 
            /*
                IntrRxE, IntrUSB, and IntrUSBE are the same 32 bits group.
                Tricky: Set 0 in all write clear field in IntrUSB field. Prevent to clear IntrUSB.
                Because our 32 bits register access mode.            
            */
            dwIntr = MGC_Read32(pBase, MGC_O_HDRC_INTRRXE);
            dwIntr &= ~((uint32_t)(1 << bEnd));
            dwIntr &= 0xFF00FFFE;
            MGC_Write32(pBase, MGC_O_HDRC_INTRRXE, (uint32_t)dwIntr);

            /* twice in case of double packet buffering */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 
                MGC_M_RXCSR_FLUSHFIFO |MGC_M_RXCSR_CLRDATATOG);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 
                MGC_M_RXCSR_FLUSHFIFO |MGC_M_RXCSR_CLRDATATOG);

            /* protocol/endpoint/interval/NAKlimit */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, wPacketSize);

            //  Set interrupt polling interval must depend on high, or low/full speed.
            MGC_SetInterval(pThis, pUrb, bEnd);

            // Clear rx endpoint interrupt.
            MGC_Write16(pBase, MGC_O_HDRC_INTRRX, (1 << bEnd));

            // Cmd Queue: need to turn on DMAENAB.
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_DMAENAB);            
        }

        bIsZlp = ((usb_pipeisoc(nPipe)) ? FALSE: (pUrb->transfer_flags & URB_ZERO_PACKET));

        if (QMU_enable(pThis, bEnd, !bXmt, bIsZlp, TRUE, FALSE) < 0)
        {
            printk("[USB] Failed to enable queue, ISOC\n");
            return;
        }

        pEnd->bCmdQEnable = TRUE;
    }

    if ((uint32_t)(pUrb->transfer_dma) >= 0x20000000)
    {
        printk("[USB]MGC_KickCmdQ error, incorrect address 0x%p.\n", (uint32_t)(pUrb->transfer_dma));
    }

    if (usb_pipeisoc(nPipe))
    {
        pBuffer = (uint8_t *)pUrb->transfer_dma;
            
        for(i=0; i<pUrb->number_of_packets; i++)
        {
            offset = pUrb->iso_frame_desc[i].offset;
            dwLength = pUrb->iso_frame_desc[i].length;
            /* If interrupt on complete ? */
            bIsIoc = (i == (pUrb->number_of_packets-1)) ? TRUE : FALSE;  
            
            if (QMU_insert_task(pThis, bEnd, !bXmt, 
                    pBuffer+offset, dwLength, bIsIoc) < 0)
            {
                printk("[USB] Insert Task Error !\n");
                return;
            }		        
        }
    }
    else
    {
        /* Must be the bulk transfer type */
        pBuffer = (uint8_t *)pUrb->transfer_dma;
    
        /* 
        Note current GPD only support 16 bits transferred data length.
        Currently no software workaround this problem.
        */        
        ASSERT(pUrb->transfer_buffer_length < 65536);
        dwLength = pUrb->transfer_buffer_length;
        bIsIoc = TRUE;
 
        if (QMU_insert_task(pThis, bEnd, !bXmt, pBuffer, dwLength, bIsIoc) < 0)
        {
            printk("[USB] Insert Task Error !\n");
            return;
        }		        
    }    
}

#endif /* #ifdef CONFIG_USB_QMU_SUPPORT */

static void MGC_ProgramEnd(MGC_LinuxCd *pThis, MGC_LinuxLocalEnd *pEnd, struct urb *pUrb)
{
    uint16_t wIntrTxE;
    uint32_t dwIntrTxE;
    uint8_t *pBase = (uint8_t*)pThis->pRegs;
    unsigned int nPipe = pUrb->pipe;
    uint16_t wPacketSize = pEnd->wPacketSize;
    uint8_t bInterval;
    uint8_t bEnd = pEnd->bEnd;
    uint8_t bXmt = pEnd->bIsTx;
    uint8_t *pBuffer;
    uint32_t dwLength;
    uint8_t bDmaOk = FALSE;    
    MGC_HsDmaChannel* pDmaChannel;    
    uint16_t wCount = 0;
    uint16_t wCsr = 0;
        
    MGC_SelectEnd(pBase, bEnd);
    MGC_SetProtocol(pThis, pUrb, bEnd, bXmt);
    MGC_SetAddress(pThis, pUrb, bEnd, bXmt);

    if (usb_pipecontrol(nPipe))
    {
        pThis->bEnd0Stage = MGC_END0_START;
        dwLength = 8;       

/*
    Note 5396 limitation problem:
    (1) Channel B do not support DMA. 
    (2) Tx command queue h/w bug. 
    If channel B is used, we need to use cpu read/write fifo.
    And must turn off Tx command queue support.
    After 5398, This condition can be removed.
*/
#if defined(CONFIG_ARCH_MT5396) && defined(CONFIG_MT53XX_USE_CHANNELB_DRAM)       
        pBuffer =  (uint8_t*)((uint32_t)pUrb->setup_packet);
#else
        pBuffer = 
            (uint8_t*)((pThis->bSupportCmdQ || (dwLength > MGC_HSDMA_MIN_DMA_LEN)) ? 
            ((uint32_t)pUrb->setup_dma) : 
            ((uint32_t)pUrb->setup_packet));
#endif
    }
    else if (usb_pipeisoc(nPipe))
    {
        dwLength = pUrb->iso_frame_desc[0].length;

/*
    Note 5396 limitation problem:
    (1) Channel B do not support DMA. 
    (2) Tx command queue h/w bug. 
    If channel B is used, we need to use cpu read/write fifo.
    And must turn off Tx command queue support.
    After 5398, This condition can be removed.
*/
#if defined(CONFIG_ARCH_MT5396) && defined(CONFIG_MT53XX_USE_CHANNELB_DRAM)       
        pBuffer = 
            (uint8_t*)((dwLength > MGC_HSDMA_MIN_DMA_LEN) ? 
            ((uint32_t)pUrb->transfer_dma + pUrb->iso_frame_desc[0].offset) : 
            ((uint32_t)pUrb->transfer_buffer + pUrb->iso_frame_desc[0].offset));
#else
        pBuffer = 
            (uint8_t*)((pThis->bSupportCmdQ || (dwLength > MGC_HSDMA_MIN_DMA_LEN)) ? 
            ((uint32_t)pUrb->transfer_dma + pUrb->iso_frame_desc[0].offset) : 
            ((uint32_t)pUrb->transfer_buffer + pUrb->iso_frame_desc[0].offset));
#endif            
    }
    else
    {
        dwLength = pUrb->transfer_buffer_length;

/*
    Note 5396 limitation problem:
    (1) Channel B do not support DMA. 
    (2) Tx command queue h/w bug. 
    If channel B is used, we need to use cpu read/write fifo.
    And must turn off Tx command queue support.
    After 5398, This condition can be removed.
*/
#if defined(CONFIG_ARCH_MT5396) && defined(CONFIG_MT53XX_USE_CHANNELB_DRAM)
        pBuffer = 
            (uint8_t*)((dwLength > MGC_HSDMA_MIN_DMA_LEN) ? 
            ((uint32_t)pUrb->transfer_dma) : 
            ((uint32_t)pUrb->transfer_buffer));
#else
        pBuffer = 
            (uint8_t*)((pThis->bSupportCmdQ || (dwLength > MGC_HSDMA_MIN_DMA_LEN)) ? 
            ((uint32_t)pUrb->transfer_dma) : 
            ((uint32_t)pUrb->transfer_buffer));
#endif            
     }

    if (bXmt) /* transmit */
    {
        /*
            IntrTxE, and IntrRx are the same 32 bits group.
            Tricky: Set 0 in all write clear field in IntrRx field. Prevent to clear IntrRx.
            Because our 32 bits register access mode.
        */
        wIntrTxE = MGC_Read16(pBase, MGC_O_HDRC_INTRTXE);
        dwIntrTxE = (uint32_t)((wIntrTxE & (~(uint16_t)(1 << bEnd))) << 16);
        dwIntrTxE &= 0xFFFF0000;
        MGC_Write32(pBase, MGC_O_HDRC_INTRRX, dwIntrTxE);

        if (bEnd)
        {
            MGC_CheckTxCsrValidForWrite(pBase, bEnd);            
            /* twice in case of double packet buffering */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_CLRDATATOG);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_CLRDATATOG);

            // get data toggle bit from logical ep.
            if (usb_gettoggle(pUrb->dev, pEnd->bRemoteEnd, 1))
            {
                // set data toggle bit to physical ep.
                MGC_Write32(pBase, M_REG_TXDATATOG, M_REG_SET_DATATOG(bEnd, 1));
                //unified usb patch, dexiao
                MGC_Write32(pBase, M_REG_TXDATATOG,(1 << bEnd));
            }

            /* protocol/endpoint/interval/NAKlimit */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, wPacketSize);

            //  Set interrupt polling interval must depend on high, or low/full speed.
            MGC_SetInterval(pThis, pUrb, bEnd);
        }
        else
        {
            /* endpoint 0: just flush */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, bEnd, MGC_M_CSR0_FLUSHFIFO);

            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, bEnd, MGC_M_CSR0_FLUSHFIFO);

            bInterval = MGC_Log2(pUrb->interval) + 1;
            bInterval = (bInterval > 16) ? 16 : ((bInterval <= 1) ? 0 : bInterval);

            /* protocol/endpoint/interval/NAKlimit */
            MGC_WriteCsr8(pBase, MGC_O_HDRC_NAKLIMIT0, 0, bInterval);
        }

        // clear tx interrupt.
        MGC_Write16(pBase, MGC_O_HDRC_INTRTX, (1 << bEnd));
        
        /* re-enable interrupt and write CSR to transmit */
        dwIntrTxE = (wIntrTxE) << 16;
        dwIntrTxE &= 0xFFFF0000;        
        MGC_Write32(pBase, MGC_O_HDRC_INTRRX, dwIntrTxE);

        if (bEnd)
        {
            MGC_CheckTxCsrValidForWrite(pBase, bEnd);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr);
        }

        if (!pEnd->pDmaChannel)
        {
            pEnd->pDmaChannel = 
                MGC_HsDmaAllocateChannel(&pThis->rDma, bEnd, TRUE, 
                dwLength, (uint8_t)usb_pipetype(nPipe), wPacketSize);
        }

        if (pEnd->pDmaChannel)
        {
            pDmaChannel = pEnd->pDmaChannel;

            //  set DMAReqMode by sending data size.
            pDmaChannel->dwStartAddress = (uint32_t)pBuffer;
            pDmaChannel->dwCount = (uint32_t)dwLength;
            pDmaChannel->dwActualLength = 0;
            pDmaChannel->bDesiredMode = (dwLength > wPacketSize) ? 1 : 0;               
            pEnd->dwRequestSize = dwLength;
            bDmaOk = MGC_HsDmaProgramChannel(pDmaChannel);

            if (!bDmaOk)
            {
                MGC_HsDmaReleaseChannel(pDmaChannel);

                pEnd->pDmaChannel = NULL;
                pEnd->dwRequestSize = 0;
            }
        }

        if (!bDmaOk)
        {
            wCount = min((uint32_t)wPacketSize, dwLength);

            if (bEnd)
            {
                MGC_TxPacket(pThis, pEnd);
            }
            else
            {
                if (wCount)
                {
                    pEnd->dwRequestSize = wCount;

                    MGC_LoadFifo(pBase, bEnd, wCount, pBuffer);
                }
                MGC_StartTx(pThis, bEnd);                
            }            
        }
    }
    else
    {
        /* protocol/endpoint/interval/NAKlimit */
        if (bEnd)
        {
            /* grab Rx residual if any */
            wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);

            if (wCsr &MGC_M_RXCSR_RXPKTRDY)
            {
                printk("[USB]RXPKTRDY when kicking urb !!");
                wCount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);
                MGC_RxPacket(pThis, pEnd, wCount);
                return ;
            }

            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, wPacketSize);

            //  Set interrupt polling interval must depend on high, or low/full speed.
            MGC_SetInterval(pThis, pUrb, bEnd);

            /* first time or re-program and shared FIFO, flush & clear toggle */
            /* twice in case of double packet buffering */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_CLRDATATOG);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_CLRDATATOG);

            // HDRC will use this method.
            if (usb_gettoggle(pUrb->dev, pEnd->bRemoteEnd, 0))
            {
                MGC_Write32(pBase, M_REG_RXDATATOG, M_REG_SET_DATATOG(bEnd, 1));
                //unified usb patch, dexiao
                MGC_Write32(pBase, M_REG_RXDATATOG,(1 << bEnd));
            }

            if ((usb_pipebulk(nPipe)) && (dwLength >= wPacketSize))
            {
                pEnd->pDmaChannel = 
                    MGC_HsDmaAllocateChannel(&pThis->rDma, bEnd, FALSE, 
                    dwLength, (uint8_t)usb_pipetype(nPipe), wPacketSize);

                if (pEnd->pDmaChannel)
                {
                    pDmaChannel = pEnd->pDmaChannel;
                    /*
                        Note: 
                        Use "multiple packet RX, if size of data block not know" to handle rx data by dma.
                    */
                    pDmaChannel->dwStartAddress = (uint32_t)pBuffer;
                    pDmaChannel->dwCount = (uint32_t)dwLength;
                    pDmaChannel->dwActualLength = 0;
                    pDmaChannel->bDesiredMode = 1;
                    pEnd->dwRequestSize = dwLength;
                    bDmaOk = MGC_HsDmaProgramChannel(pDmaChannel);
                    if (!bDmaOk)
                    {
                        MGC_HsDmaReleaseChannel(pDmaChannel);

                        pEnd->pDmaChannel = NULL;
                        pEnd->dwRequestSize = 0;
                    }
                    else
                    {
                        // DMA ok.
                        return ;
                    }
                }
            }

            /* kick things off */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_H_REQPKT);
         }
    }
}

void MGC_KickUrb(MGC_LinuxCd *pThis, struct urb *pUrb, MGC_LinuxLocalEnd *pEnd)
{
    uint32_t nPipe;
    uint8_t bXmt;
    uint16_t wPacketSize;

    if (!pUrb)
    {
        ERR("[USB]***> bEnd=%d, pUrb = NULL!\n", pEnd->bEnd);
        return ;
    } 

    nPipe = pUrb->pipe;
    bXmt = (usb_pipecontrol(nPipe)) ? TRUE: usb_pipeout(nPipe);
    wPacketSize = usb_maxpacket(pUrb->dev, nPipe, usb_pipeout(nPipe));

    DBG(2, "[USB]<== pUrb=%p, bEnd=%d, wPacketSize=%d, bRemoteAddress=%d, bRemoteEnd=%d, bXmt=%d\n", pUrb, pEnd->bEnd, wPacketSize, bRemoteAddress, bRemoteEnd, bXmt);

    //printk("[USB]<== pUrb=%p, bEnd=%d, transfer_buffer_length=%d, wPacketSize=%d, bRemoteAddress=%d, bRemoteEnd=%d, bXmt=%d\n", pUrb, pEnd->bEnd, pUrb->transfer_buffer_length, wPacketSize, bRemoteAddress, bRemoteEnd, bXmt);

    /* indicate in progress */
    pUrb->actual_length = 0;
    pUrb->error_count = 0;

    DBG(3, "[USB](%p): dir=%s, type=%d, wPacketSize=%d, bRemoteAddress=%d, bRemoteEnd=%d.\n", pUrb, (bXmt) ? "out" : "in", usb_pipetype(nPipe), wPacketSize, bRemoteAddress, bRemoteEnd);

#ifdef CONFIG_USB_QMU_SUPPORT
    if (MGC_IsCmdQSupport(pThis, pEnd, pUrb))
    {
        MGC_KickCmdQ(pThis, pEnd, pUrb);
    }
    else
#endif  /* CONFIG_USB_QMU_SUPPORT */ 
    {
        /* remember software state - find_end() will use this - */
        pEnd->bRemoteAddress = (uint8_t)usb_pipedevice(nPipe);;
        pEnd->bRemoteEnd = (uint8_t)usb_pipeendpoint(nPipe);;
        pEnd->bTrafficType = (uint8_t)usb_pipetype(nPipe);

        /* init urb */
        pEnd->dwOffset = 0;
        pEnd->dwRequestSize = 0;
        pEnd->dwIsoPacket = 0;
        pEnd->bRetries = 0;
        pEnd->wPacketSize = wPacketSize;
        pEnd->bIsTx = bXmt;

        /* Configure endpoint */
        MGC_ProgramEnd(pThis, pEnd, pUrb);
    }
}

static int MGC_ScheduleUrb(MGC_LinuxCd *pThis, MGC_LinuxLocalEnd *pEnd, struct urb *pUrb)
{
    int idle;
    int rc;

    DBG(3, "[USB]queued URB %p (current %p) to end %d (bRemoteAddress=%d, bRemoteEnd=%d proto=%d) (idle=%d) pEnd->bBusyCompleting=%d\n", pUrb, MGC_GetCurrentUrb(pEnd), pEnd->bEnd, (uint8_t)usb_pipedevice(pUrb->pipe), (uint8_t)usb_pipeendpoint(pUrb->pipe), usb_pipetype(pUrb->pipe), idle, pEnd->bBusyCompleting);

    idle = MGC_IsEndIdle(pEnd);

    rc = MGC_EnqueueEndUrb(pThis, pEnd, pUrb);
    if (rc)
    {
        return rc;
    }

#ifdef CONFIG_USB_QMU_SUPPORT
    if (MGC_IsCmdQSupport(pThis, pEnd, pUrb) 
        && (!pEnd->bStateGated))
    {
        MGC_KickUrb(pThis, pUrb, pEnd);
    }
    else
#endif        
    {       
        //  check pEnd->bBusyCompleting to prevent double kickstart the same urb.           
        if (idle && (pEnd->bBusyCompleting == 0))
        {
            MGC_KickUrb(pThis, MGC_GetCurrentUrb(pEnd), pEnd);
        }
    }

    return 0;
}

static uint8_t MGC_IsNoiseStillSof(uint8_t *pBase)
{
    uint32_t reg;
    reg = MGC_Read8(pBase,M_REG_PERFORMANCE3);
    return (uint8_t)(reg & 0x80);/* Check if Host set SOF_FORCE bit */

}

static int MGC_ServiceUSBIntr(MGC_LinuxCd *pThis, uint8_t bIntrUSB)
{
    int handled = 0;
    uint8_t bSpeed = 1;
    uint8_t devctl;
    uint8_t *pBase = (uint8_t*)pThis->pRegs;
    uint32_t dwVirtualHubPortStatus = 0;
    uint8_t power;

    devctl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);
    power = MGC_Read8(pBase, MGC_O_HDRC_POWER);

    if (bIntrUSB &MGC_M_INTR_RESUME)
    {
        handled++;

        DBG(2, "[USB]RESUME\n");
    }

    if (bIntrUSB &MGC_M_INTR_SESSREQ)
    {
        handled++;

        DBG(2, "[USB]SESSION_REQUEST\n");
    }

    if (bIntrUSB &MGC_M_INTR_VBUSERROR)
    {
        handled++;  

        DBG(2, "[USB]V_BUS ERROR??? this is bad (TM)\n");
    }

    if (bIntrUSB &MGC_M_INTR_CONNECT)
    {
        handled++;
 
        DBG(2, "[USB]CONNECT\n");

        pThis->bEnd0Stage = MGC_END0_START;

        if (devctl &MGC_M_DEVCTL_LSDEV)
        {
            bSpeed = MGC_SPEED_LS;

            dwVirtualHubPortStatus = (1 << USB_PORT_FEAT_C_CONNECTION) | (1 << USB_PORT_FEAT_LOWSPEED) | (1 << USB_PORT_FEAT_POWER) | (1 << USB_PORT_FEAT_CONNECTION);
        }
        else if (devctl &MGC_M_DEVCTL_FSDEV)
        {
            /* NOTE: full-speed is "speculative" until reset */
            //bSpeed = 2;
            bSpeed = (power &MGC_M_POWER_HSMODE) ? MGC_SPEED_HS : MGC_SPEED_FS;

            dwVirtualHubPortStatus = (1 << USB_PORT_FEAT_C_CONNECTION) | (1 << USB_PORT_FEAT_POWER) | (1 << USB_PORT_FEAT_CONNECTION);
        }

        pThis->bRootSpeed = bSpeed;
        pThis->dwVirtualHubPortStatus = dwVirtualHubPortStatus;
        usb_hcd_poll_rh_status(musbstruct_to_hcd(pThis));
    }

    /* saved one bit: bus reset and babble share the same bit;
     * If I am host is a babble! i must be the only one allowed
     * to reset the bus; when in otg mode it means that I have 
     * to switch to device
     */
    if (bIntrUSB &MGC_M_INTR_RESET)
    {
        handled++;

        DBG(1, "[USB]BUS babble\n");

        //  Solve the busy usb babble interrupt caused by babble device
        //  if device is attached
        if (devctl &(MGC_M_DEVCTL_FSDEV | MGC_M_DEVCTL_LSDEV))
        {
            DBG(1, "[USB]MT539x ");

            if (MGC_IsNoiseStillSof(pBase))
            {
                DBG(1,"[USB]SOF is still transmit during babble.\n", NULL);
                // Ignore this babble.
                bIntrUSB &= ~MGC_M_INTR_RESET;
            }
        }

        handled++;
    }

    if (bIntrUSB &MGC_M_INTR_SOF)
    {
        DBG(2, "[USB]START_OF_FRAME\n");

        handled++;
    }

    /* p35 MUSBHDRC manual for the order of the tests */
    if (bIntrUSB &MGC_M_INTR_DISCONNECT)
    {
        DBG(2, "[USB]DISCONNECT\n");

        handled++;

        pThis->dwVirtualHubPortStatus = (1 << USB_PORT_FEAT_C_CONNECTION) | (1 << USB_PORT_FEAT_POWER);

        // Set UTMI+PHY to low power mode and stop XCLK.
        power = MGC_Read8(pBase, MGC_O_HDRC_POWER);
        power |= (MGC_M_POWER_SUSPENDM | MGC_M_POWER_ENSUSPEND);
        MGC_Write8(pBase, MGC_O_HDRC_POWER, power);
    }

    /* I cannot get suspend while in host mode! go to error mode and ignore 
     * the other signals; need to be last (see manual p35)s  */
    if (bIntrUSB &MGC_M_INTR_SUSPEND)
    {
        DBG(2, "[USB]RECEIVED SUSPEND\n");

        handled++;
    }

    return handled;
}

static void MGC_CheckConnect(unsigned long pParam)
{
    MGC_LinuxCd *pThis = (MGC_LinuxCd*)pParam;
    void *pPhyBase = pThis->pPhyBase;
    uint32_t dwLineState;

    dwLineState = MGC_PHY_Read32(pPhyBase, M_REG_U2PHYDMON1);
    dwLineState &= M_REG_LINESTATE_MASK;
    if (dwLineState == LINESTATE_DISCONNECT)
    {
        printk("[USB]Line State Err : 0x%x\n", dwLineState);
        return ;
    }

    if (dwLineState == LINESTATE_HWERROR)
    {
        //  Move MGC_dwCheckInsert inside MGC_LinuxCd port structure
        pThis->bCheckInsert++;

        if (pThis->bCheckInsert > 5)
        {
            DBG(2, "[USB]LINESTATE_HWERROR !!!\n");

            pThis->bCheckInsert = 0;
            return ;
        }

        MGC_SetTimer(pThis, MGC_CheckConnect, (unsigned long)pThis, MGC_CHECK_INSERT_DEBOUNCE);
        printk("[USB]Line State Err : 0x%x\n", dwLineState);
        return ;
    }

    pThis->bInsert = TRUE;

    DBG(2, "[USB]Connect interrupt !!!\n");

    // handle device connect.
    MGC_ServiceUSBIntr(pThis, MGC_M_INTR_CONNECT);
}

static MGC_DmaControlStatus MGC_ServiceHostDefault(MGC_LinuxCd *pThis, uint16_t wCount, struct urb *pUrb)
{
    MGC_DmaControlStatus bMore = MGC_DMA_FIFO_DATAEND;
    uint8_t *pFifoDest = NULL;
    uint16_t wFifoCount = 0;
    uint8_t *pBase = (uint8_t*)pThis->pRegs;
    MGC_LinuxLocalEnd *pEnd = &(pThis->aLocalEnd[EP0][0]);
    MUSB_DeviceRequest *pRequest = (MUSB_DeviceRequest*)pUrb->setup_packet;
    uint8_t bDmaOk = FALSE;    
    MGC_HsDmaChannel* pDmaChannel;    

    DBG(2, "[USB]<== (wCount=%04x, pUrb=%lx, bStage=%02x)\n", wCount, (unsigned long)pUrb, pThis->bEnd0Stage);

    switch (pThis->bEnd0Stage)
    {
        case MGC_END0_START:
            if (pRequest->bmRequestType & USB_DIR_IN)
            {
                DBG(3, "[USB]just did setup, switching to IN\n");

                /* this means we just did setup; switch to IN */
                pThis->bEnd0Stage = MGC_END0_IN;
                bMore = MGC_DMA_FIFO_DATAMORE;                
            }
            else if (pRequest->wLength > 0)
            {
                if (pEnd->pDmaChannel)
                {
                    pDmaChannel = pEnd->pDmaChannel;
                    pFifoDest = (uint8_t *)((uint32_t)pUrb->transfer_dma + pUrb->actual_length);
                    wFifoCount = min(pEnd->wPacketSize, ((uint16_t)(pUrb->transfer_buffer_length - pUrb->actual_length)));
                    pDmaChannel->bTransmit = TRUE;  /* Change to Out data */
                    pDmaChannel->dwStartAddress = (uint32_t)pFifoDest;
                    pDmaChannel->dwCount = (uint32_t)wFifoCount;
                    pDmaChannel->dwActualLength = 0;
                    pDmaChannel->bDesiredMode = 0;
                    pEnd->dwRequestSize = wFifoCount;
                    bDmaOk = MGC_HsDmaProgramChannel(pDmaChannel);
                    if (!bDmaOk)
                    {
                        MGC_HsDmaReleaseChannel(pDmaChannel);
                        pEnd->pDmaChannel = NULL;
                        pEnd->dwRequestSize = 0;
                    }
                    else
                    {            
                        // DMA ok.                
                        pThis->bEnd0Stage = MGC_END0_DMA_OUT;                
                        return MGC_DMA_OUT;
                    }
                }

                pThis->bEnd0Stage = MGC_END0_OUT;
            
                pFifoDest = (uint8_t*)(pUrb->transfer_buffer + pUrb->actual_length);
                wFifoCount = min(pEnd->wPacketSize, ((uint16_t)(pUrb->transfer_buffer_length - pUrb->actual_length)));
                DBG(3, "[USB]Sending %d bytes to %p\n", wFifoCount, pFifoDest);
                MGC_LoadFifo(pBase, 0, wFifoCount, pFifoDest);

                pEnd->dwRequestSize = wFifoCount;
                pUrb->actual_length += wFifoCount;
                if (wFifoCount)
                {
                    bMore = MGC_DMA_FIFO_DATAMORE;
                }
            }
            break;
        case MGC_END0_IN:
            if (pEnd->pDmaChannel)
            {
                pDmaChannel = pEnd->pDmaChannel;           
                pFifoDest = (uint8_t *)((uint32_t)pUrb->transfer_dma + pUrb->actual_length);
                wFifoCount = min(wCount, ((uint16_t)(pUrb->transfer_buffer_length - pUrb->actual_length)));
                pDmaChannel->bTransmit = FALSE;  /* Change to In data */
                pDmaChannel->dwStartAddress = (uint32_t)pFifoDest;
                pDmaChannel->dwCount = (uint32_t)wFifoCount;
                pDmaChannel->dwActualLength = 0;
                pDmaChannel->bDesiredMode = 0;
                pEnd->dwRequestSize = wFifoCount;
                bDmaOk = MGC_HsDmaProgramChannel(pDmaChannel);
                if (!bDmaOk)
                {
                    MGC_HsDmaReleaseChannel(pDmaChannel);
                    pEnd->pDmaChannel = NULL;
                    pEnd->dwRequestSize = 0;
                }
                else
                {            
                    // DMA ok.                
                    pThis->bEnd0Stage = MGC_END0_DMA_IN;
                    return MGC_DMA_IN;
                }
            }
            
            /* we are receiving from peripheral */
            pFifoDest = pUrb->transfer_buffer + pUrb->actual_length;
            wFifoCount = min(wCount, ((uint16_t)(pUrb->transfer_buffer_length - pUrb->actual_length)));

            DBG(3, "[USB]Receiving %d bytes in &%p[%d] (pUrb->actual_length=%u)\n", wFifoCount, pUrb->transfer_buffer, (unsigned int)pUrb->actual_length, pUrb->actual_length);

            MGC_UnloadFifo(pBase, 0, wFifoCount, pFifoDest);
            MGC_CleanDcache(pUrb,(uint32_t)pFifoDest, (uint32_t)wFifoCount);
            pUrb->actual_length += wFifoCount;
            if ((pUrb->actual_length < pUrb->transfer_buffer_length) && (wCount == pEnd->wPacketSize))
            {
                bMore = MGC_DMA_FIFO_DATAMORE;
            }             
            break;
        case MGC_END0_DMA_IN:
            MUSB_ASSERT(pEnd->pDmaChannel);
            pDmaChannel = pEnd->pDmaChannel;
            wCount = pDmaChannel->dwActualLength;
            pEnd->dwOffset += wCount;
            pUrb->actual_length += wCount;
            if ((pUrb->actual_length < pUrb->transfer_buffer_length) && 
                 (wCount == pEnd->wPacketSize))
            {
                pThis->bEnd0Stage = MGC_END0_IN;
                bMore = MGC_DMA_FIFO_DATAMORE;
            }
            break;
        case MGC_END0_OUT:
            pFifoDest = (uint8_t*)(pUrb->transfer_buffer + pUrb->actual_length);
            wFifoCount = min(pEnd->wPacketSize, ((uint16_t)(pUrb->transfer_buffer_length - pUrb->actual_length)));
            DBG(3, "[USB]Sending %d bytes to %p\n", wFifoCount, pFifoDest);
            MGC_LoadFifo(pBase, 0, wFifoCount, pFifoDest);

            pEnd->dwRequestSize = wFifoCount;
            pUrb->actual_length += wFifoCount;
            if (wFifoCount)
            {
                bMore = MGC_DMA_FIFO_DATAMORE;
            }            
            break;
        case MGC_END0_DMA_OUT:
            MUSB_ASSERT(pEnd->pDmaChannel);
            pDmaChannel = pEnd->pDmaChannel;
            pEnd->dwOffset += pDmaChannel->dwActualLength;
            pUrb->actual_length += pDmaChannel->dwActualLength;
            if (pUrb->actual_length < pUrb->transfer_buffer_length)
            {
                pFifoDest = (uint8_t *)((uint32_t)pUrb->transfer_dma + pUrb->actual_length);
                wFifoCount = min(pEnd->wPacketSize, ((uint16_t)(pUrb->transfer_buffer_length - pUrb->actual_length)));
                pDmaChannel->bTransmit = TRUE;  /* Change to Out data */
                pDmaChannel->dwStartAddress = (uint32_t)pFifoDest;
                pDmaChannel->dwCount = (uint32_t)wFifoCount;
                pDmaChannel->dwActualLength = 0;
                pDmaChannel->bDesiredMode = 0;
                pEnd->dwRequestSize = wFifoCount;
                bDmaOk = MGC_HsDmaProgramChannel(pDmaChannel);
                if (!bDmaOk)
                {
                    MGC_HsDmaReleaseChannel(pDmaChannel);
                    pEnd->pDmaChannel = NULL;
                    pEnd->dwRequestSize = 0;
                }
                else
                {            
                    return MGC_DMA_OUT;
                }
            }            
            break;
        default:
            break;
    }

    return bMore;
}

static void MGC_ServiceDefaultEnd(MGC_LinuxCd *pThis)
{
    struct urb *pUrb;
    uint16_t wCsrVal, wCount;
    int status = USB_ST_NOERROR;
    uint8_t *pBase = (uint8_t*)pThis->pRegs;
    MGC_LinuxLocalEnd *pEnd = &(pThis->aLocalEnd[EP0][0]);
    uint8_t bVal, bOutVal = 0, bComplete = FALSE, bError = FALSE;
    MGC_DmaControlStatus bMore;
    
    pUrb = MGC_GetCurrentUrb(pEnd);
    MGC_SelectEnd(pBase, 0);

    wCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_CSR0, 0);
    wCount = MGC_ReadCsr8(pBase, MGC_O_HDRC_COUNT0, 0);
    bVal = (uint8_t)wCsrVal;

    DBG(2, "[USB]<== CSR0=%04x, wCount=%04x\n", wCsrVal, wCount);

    /* if we just did status stage, we are done */
    if (MGC_END0_STATUS == pThis->bEnd0Stage)
    {
        bComplete = TRUE;
    }

    /* prepare status */
    if ((MGC_END0_START == pThis->bEnd0Stage) && !wCount && (wCsrVal &MGC_M_CSR0_RXPKTRDY))
    {
        printk("[USB]EP0 missed data, Flush FIFO !\n");
        /* just started and got Rx with no data, so probably missed data */
        status = USB_ST_SHORT_PACKET;
        bError = TRUE;

        //bComplete = TRUE;
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
    }

    if (bVal &MGC_M_CSR0_H_RXSTALL)
    {
        printk("[USB]STALLING ENDPOINT 0\n");
        status = USB_ST_STALL;
        bError = TRUE;
    }
    else if (bVal &MGC_M_CSR0_H_ERROR)
    {
        printk("[USB]ep0 no response (error)\n");

        status = USB_ST_NORESPONSE;
        bError = TRUE;
    }
    else if (bVal &MGC_M_CSR0_H_NAKTIMEOUT)
    {
        printk("[USB]ep0 NAK timeout pEnd->bRetries=%d\n", pEnd->bRetries);

        if (++pEnd->bRetries < MUSB_MAX_RETRIES)
        {
            /* cover it up if retries not exhausted */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, 0);
        }
        else
        {
            printk("[USB]no response (NAK timeout)\n");
            pEnd->bRetries = 0;
            status = USB_ST_NORESPONSE;
            bError = TRUE;
        }
    }

    if (USB_ST_NORESPONSE == status)
    {
        printk("[USB]ep0 aborting\n");

        /* use the proper sequence to abort the transfer */
        if (bVal &MGC_M_CSR0_H_REQPKT)
        {
            bVal &= ~MGC_M_CSR0_H_REQPKT;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, bVal);
            bVal &= ~MGC_M_CSR0_H_NAKTIMEOUT;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, bVal);
        }
        else
        {
            printk("[USB]end 0 no response, Flush FIFO !\n");
            bVal |= MGC_M_CSR0_FLUSHFIFO;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, bVal);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, bVal);
            bVal &= ~MGC_M_CSR0_H_NAKTIMEOUT;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, bVal);
        }

        MGC_WriteCsr8(pBase, MGC_O_HDRC_NAKLIMIT0, 0, 0);
    }

    if (bError)
    {
        DBG(3, "[USB]ep0 handling error\n");

        /* clear it */
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, 0);
    }

    if (!pUrb)
    {
        /* stop endpoint since we have no place for its data, this 
         * SHOULD NEVER HAPPEN! */
        printk("[USB]no URB for end 0, Flush FIFO !\n");
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, 0);

        /* start next URB that might be queued for it */
        return ;
    }

    if (!bComplete && !bError)
    {
        /* call common logic and prepare response */
        bMore = MGC_ServiceHostDefault(pThis, wCount, pUrb);
        if (bMore == MGC_DMA_FIFO_DATAMORE)
        {
            /* more packets required */
            bOutVal = (MGC_END0_IN == pThis->bEnd0Stage) ? MGC_M_CSR0_H_REQPKT: MGC_M_CSR0_TXPKTRDY;
            DBG(3, "[USB]Need more bytes bOutVal=%04x\n", bOutVal);
        }
        else if (bMore == MGC_DMA_FIFO_DATAEND)
        {
            /* data transfer complete; perform status phase */
            bOutVal = MGC_M_CSR0_H_STATUSPKT | (usb_pipeout(pUrb->pipe) ? MGC_M_CSR0_H_REQPKT : MGC_M_CSR0_TXPKTRDY);

            /* flag status stage */
            pThis->bEnd0Stage = MGC_END0_STATUS;
            DBG(3, "[USB]Data transfer complete, status phase bOutVal=%04x\n", bOutVal);
        }

        /* write CSR0 if needed */
        if (bOutVal)
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, bOutVal);
        }
    }

    /* call completion handler if done */
    if (bComplete || bError)
    {
        DBG(3, "[USB]completing cntrl URB %p, status=%d, len=%x\n", pUrb, status, pUrb->actual_length);

        // release dma channel.
        if (pEnd->pDmaChannel)
        {
            /* release previously-allocated channel */
            MGC_HsDmaReleaseChannel(pEnd->pDmaChannel);
            pEnd->pDmaChannel = NULL;
        }

        MGC_DequeueEndurb(pThis, pEnd, pUrb);
        pUrb->status = status;

        pEnd->bBusyCompleting = 1;
        spin_unlock(&pThis->SelectEndLock); 
        MGC_CompleteUrb(pThis, pEnd, pUrb);
        spin_lock(&pThis->SelectEndLock);
        pEnd->bBusyCompleting = 0;

        if (!MGC_IsEndIdle(pEnd))
        {
            MGC_KickUrb(pThis, MGC_GetNextUrb(pEnd), pEnd);
        }
    }
}

static void MGC_ServiceTxAvail(MGC_LinuxCd *pThis, uint8_t bEnd)
{
    int skip = 0;
    struct urb *pUrb;
    uint16_t wTxCsrVal;
    uint16_t wVal = 0;
    MGC_LinuxLocalEnd *pEnd; 
    uint8_t *pBase = (uint8_t*)pThis->pRegs;
    int toggle;
    uint32_t status = 0;

    if (bEnd >= pThis->bEndCount)
    {
        printk("Tx Error: bEnd >= pThis->bEndCount !.\n");
	return;
    }

    if (bEnd >= MUSB_C_NUM_EPS)
    {
        return;
    }
    
    /*
        NOTICE:
        Because we use shared double packet buffer for mass storage.
        It can not let tx/rx traffic happend at the same time.
        So we need to queue all urb (tx/rx) on the same list.
        It will prevent tx/rx data corruption when tx/rx transfer at the same time.
        Always use EPMSD direction of pEnd for mass storage.
    */
    pEnd = (MGC_END_MSD == bEnd) ? 
        (&(pThis->aLocalEnd[EPMSD][bEnd])) : (&(pThis->aLocalEnd[EPTX][bEnd]));
    
    MGC_SelectEnd(pBase, bEnd);

    wTxCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
    wVal = wTxCsrVal;

    if (pEnd->bDoublePacketBuffer)
    {
        if (wTxCsrVal & (MGC_M_TXCSR_FIFONOTEMPTY|MGC_M_TXCSR_TXPKTRDY))
        {
            /* whopps, dbould buffering better be enabled. */
            /* skip this interrrupt and wait FIFONOTEMPTY to be clear by h/w */
            ASSERT(pEnd->bDoublePacketBuffer);    
            return;
        }
        else
        {
            // Clear double packet buffer overhead interrupt.    
            if (MGC_Read16(pBase, MGC_O_HDRC_INTRTX) & (1 << bEnd))
            {            
                MGC_Write16(pBase, MGC_O_HDRC_INTRTX, (1 << bEnd));            
            }
        }
    }
        
    pUrb = MGC_GetCurrentUrb(pEnd);
    if (!pUrb)
    {
        printk("[USB]Urb has been freed before packet comeback. Tx bEnd =%d.\n", bEnd);    
        MGC_SelectEnd(pBase, bEnd);
        MGC_CheckTxCsrValidForWrite(pBase, bEnd);
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, 0);
        return ;
    } 

    /* check for errors */
    if (wTxCsrVal &MGC_M_TXCSR_H_RXSTALL)
    {
        printk("[USB]TX end %d STALL, Flush FIFO (%d:%d) !\n", bEnd, pEnd->bRemoteAddress, pEnd->bEnd);
        status = USB_ST_STALL;
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_TXPKTRDY);
    }
    else if (wTxCsrVal &MGC_M_TXCSR_H_ERROR)
    {
        printk("[USB]TX data error on ep=%d, Flush FIFO (%d:%d) !\n", bEnd, pEnd->bRemoteAddress, pEnd->bEnd);

        status = USB_ST_NORESPONSE;

        /* do the proper sequence to abort the transfer */
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_TXPKTRDY);
    }
    else if (wTxCsrVal &MGC_M_TXCSR_H_NAKTIMEOUT)
    {
        /* cover it up if retries not exhausted */
        if (pUrb->status == ( - EINPROGRESS) && ++pEnd->bRetries < MUSB_MAX_RETRIES)
        {

            wVal |= MGC_M_TXCSR_TXPKTRDY;
            wVal &= ~MGC_M_TXCSR_H_NAKTIMEOUT;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wVal);

            printk("[USB]TX NAK Timeout (%d:%d), Retry %d !\n", pEnd->bRemoteAddress, pEnd->bEnd, pEnd->bRetries);
            return ;
        }

        if (pUrb->status == ( - EINPROGRESS))
        {
            printk("[USB]TX NAK Timeout !\n");
            status = USB_ST_NORESPONSE;
        }

        printk("[USB]device Tx not responding on ep=%d, Flush FIFO (%d:%d) !\n", bEnd, pEnd->bRemoteAddress, pEnd->bEnd);

        /* do the proper sequence to abort the transfer */
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_TXPKTRDY);
        pEnd->bRetries = 0;
    }   
    else if (wTxCsrVal &MGC_M_TXCSR_FIFONOTEMPTY)
    {
        printk("[USB]FIFO not empty when Tx done on ep=%d (%d:%d) !\n", bEnd, pEnd->bRemoteAddress, pEnd->bEnd);
        /* whopps, dbould buffering better be enabled */
        skip = TRUE;
    }

    if (status)
    {
        pUrb->status = status;

        /* reset error bits */
        wVal &= ~(MGC_M_TXCSR_H_ERROR | MGC_M_TXCSR_H_RXSTALL | MGC_M_TXCSR_H_NAKTIMEOUT);
        wVal |= MGC_M_TXCSR_FRCDATATOG;
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wVal);
    }

    if (!skip && pUrb->status == ( - EINPROGRESS))
    {   
        MGC_TxPacket(pThis, pEnd);
    }

    /* complete the current request or start next tx transaction */
    if (pUrb->status != ( - EINPROGRESS))
    {
        //  set data toggle in device basis for supporting Hub.
        // get data toggle bit from physical ep.
        toggle = ((MGC_Read32(pBase, M_REG_TXDATATOG)) >> bEnd) &0x01;
            
        // release dma channel.
        if (pEnd->pDmaChannel)
        {
            /* release previously-allocated channel */
            MGC_HsDmaReleaseChannel(pEnd->pDmaChannel);
            pEnd->pDmaChannel = NULL;
        }
        pUrb->actual_length = pEnd->dwOffset;

        if (pUrb->status == USB_ST_STALL)
        {
            toggle = 0;
        } 

            /* save data toggle */
        usb_settoggle(pUrb->dev, pEnd->bRemoteEnd, TRUE, toggle);

        MGC_DequeueEndurb(pThis, pEnd, pUrb);

        pEnd->bBusyCompleting = 1;
        spin_unlock(&pThis->SelectEndLock); 
        MGC_CompleteUrb(pThis, pEnd, pUrb);
        spin_lock(&pThis->SelectEndLock);
        pEnd->bBusyCompleting = 0;

        if (!MGC_IsEndIdle(pEnd))
        {
            MGC_KickUrb(pThis, MGC_GetNextUrb(pEnd), pEnd);
        }
    }
}

static void MGC_ServiceRxReady(MGC_LinuxCd *pThis, uint8_t bEnd)
{
    struct urb *pUrb;
    uint16_t wRxCount, wRxCsrVal, wVal = 0;
    uint8_t bIsochError = FALSE;
    MGC_LinuxLocalEnd *pEnd;
    uint8_t *pBase = (uint8_t*)pThis->pRegs;
    uint8_t bDone = FALSE;
    uint32_t dwLength;
    MGC_DmaChannelStatus bStatus;
    MGC_HsDmaChannel* pDmaChannel;
    uint8_t bDmaOk = FALSE;
    uint8_t *pDmaBuffer;
    uint32_t dwIntr;    
    int toggle;
    uint32_t status = 0;
    struct usb_iso_packet_descriptor *packet;

    DBG(2, "[USB]<== end%d\n", bEnd);
    DBG(3, "[USB]locked end%d, pUrb=%p\n", bEnd, MGC_GetCurrentUrb(pEnd));
    
    if(bEnd >= pThis->bEndCount)
    {
        printk("Rx Error: bEnd >= pThis->bEndCount !.\n");
	return;
    }

    if (bEnd >= MUSB_C_NUM_EPS)
    {
        return;
    }    
    
    pEnd = &(pThis->aLocalEnd[EPRX][bEnd]); /*0: Rx*/
    
    pUrb = MGC_GetCurrentUrb(pEnd);
    if (!pUrb)
    {
        printk("[USB]Urb has been freed before packet comeback. Rx bEnd =%d.\n", bEnd);
        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 0);
        return ;
    } 

    MGC_SelectEnd(pBase, bEnd);

    wRxCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
    wVal = wRxCsrVal;
    wRxCount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);

    DBG(3, "[USB]end %d wRxCsrVal=%04x, wRxCount=%d, pUrb->actual_length=%d\n", bEnd, wRxCsrVal, wRxCount, pUrb->actual_length);

    /* check for errors, concurrent stall & unlink is not really
     * handled yet! */
    if (wRxCsrVal &MGC_M_RXCSR_H_RXSTALL)
    {
        DBG(1, "[USB]RX end %d STALL (%d:%d) !\n", bEnd, pEnd->bRemoteAddress, pEnd->bEnd);
        status = USB_ST_STALL;
    }
    else if (wRxCsrVal &MGC_M_RXCSR_H_ERROR)
    {
        DBG(1, "[USB]end %d Rx error (%d:%d) !\n", bEnd, pEnd->bRemoteAddress, pEnd->bEnd);
        status = USB_ST_NORESPONSE;

        /* do the proper sequence to abort the transfer */
        wVal &= ~MGC_M_RXCSR_H_REQPKT;
        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wVal);
        MGC_WriteCsr8(pBase, MGC_O_HDRC_RXINTERVAL, bEnd, 0);
    }
    else if (wRxCsrVal &MGC_M_RXCSR_DATAERR)
    {
        if (PIPE_BULK == pEnd->bTrafficType)
        {
            /* cover it up if retries not exhausted, slow devices might  
             * not answer quickly enough: I was expecting a packet but the 
             * packet didn't come. The interrupt is generated after 3 failed
             * attempts, it make MUSB_MAX_RETRIESx3 attempts total..
             */
            if (pUrb->status == ( - EINPROGRESS) && ++pEnd->bRetries < MUSB_MAX_RETRIES)
            {
                /* silently ignore it */
                wRxCsrVal &= ~MGC_M_RXCSR_DATAERR;
                wRxCsrVal &= ~MGC_M_RXCSR_RXPKTRDY;
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wRxCsrVal | MGC_M_RXCSR_H_REQPKT);

                DBG(1, "[USB]Rx NAK Timeout (%d:%d), Retry %d !\n", pEnd->bRemoteAddress, pEnd->bEnd, pEnd->bRetries);
                return ;
            }

            if (pUrb->status == ( - EINPROGRESS))
            {
                DBG(1, "[USB]RX NAK Timeout !\n");
                status = USB_ST_NORESPONSE;
            }

            wVal &= ~MGC_M_RXCSR_H_REQPKT;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wVal);
            MGC_WriteCsr8(pBase, MGC_O_HDRC_RXINTERVAL, bEnd, 0);
            pEnd->bRetries = 0;

            /* do the proper sequence to abort the transfer; 
             * am I dealing with a slow device maybe? */
            printk("[USB]end=%d device Rx not responding (%d:%d) !\n", bEnd, pEnd->bRemoteAddress, pEnd->bEnd);

        }
        else if (PIPE_ISOCHRONOUS == pEnd->bTrafficType)
        {
            DBG(3, "[USB]bEnd=%d Isochronous error\n", bEnd);
            bIsochError = TRUE;
        }
    }

    /* an error won't process the data */
    if (status)
    {
        pUrb->status = status;

        /* data errors are signaled */
        if (USB_ST_STALL != status)
        {
            DBG(1, "[USB]end %d Rx error, status=%d !\n", bEnd, status);
        }
        else
        {
            //printk("[USB]MGC_ServiceRxReady : Flush FIFO !\n");
            /* twice in case of double packet buffering */
            MGC_WriteCsr16((uint8_t*)pThis->pRegs, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);
            MGC_WriteCsr16((uint8_t*)pThis->pRegs, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);
        }

        DBG(3, "[USB]clearing all error bits, right away\n");
        wVal &= ~(MGC_M_RXCSR_H_ERROR | MGC_M_RXCSR_DATAERR | MGC_M_RXCSR_H_RXSTALL);
        wVal &= ~MGC_M_RXCSR_RXPKTRDY;
        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wVal);
    }

    /* no errors, unload... */
    if (pUrb->status == ( - EINPROGRESS))
    {
        /* be sure a packet is ready for unloading */
        if (!wRxCsrVal &MGC_M_RXCSR_RXPKTRDY)
        {
            pUrb->status = USB_ST_INTERNALERROR;

            /* do the proper sequence to abort the transfer */
            wVal &= ~MGC_M_RXCSR_H_REQPKT;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wVal);
            DBG(3, "[USB]Rx interrupt with no errors or packet!\n");
        }
        else
        {        
            /* we are expecting traffic */
            if (pEnd->pDmaChannel)
            {
                pDmaChannel = pEnd->pDmaChannel;
                
                if (usb_pipeisoc(pUrb->pipe))
                {                    
                    packet = &pUrb->iso_frame_desc[pEnd->dwIsoPacket];                        
                    /* update actual & status */
                    packet->actual_length = pDmaChannel->dwActualLength;                    
                    packet->status = USB_ST_NOERROR;
                    pUrb->actual_length += pDmaChannel->dwActualLength;

                    // Free DMA.
                    MGC_HsDmaReleaseChannel(pDmaChannel);
                    pEnd->pDmaChannel = NULL;
                    pEnd->dwRequestSize = 0;
                    pEnd->dwIsoPacket ++;

                    if (pEnd->dwIsoPacket < pUrb->number_of_packets)
                    {
                        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_H_REQPKT);                        
                        return;
                    }
                    else
                    {
                        //Finish iso transfer.
                        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 0);
                        pUrb->status = 0;
                        goto rx_urb_end;
                    }
                }

                bStatus = MGC_HsDmaGetChannelStatus(pDmaChannel);
                pEnd->dwOffset += pDmaChannel->dwActualLength;
                pUrb->actual_length += pDmaChannel->dwActualLength;

                if (MGC_DMA_STATUS_FREE == bStatus)
                {                
                    /* see if we are done */
                    bDone = (pEnd->dwOffset >= pUrb->transfer_buffer_length) || (pDmaChannel->dwActualLength < pEnd->wPacketSize);

                    if (bEnd && bDone)
                    {
                        // clear rxpktrdy.
                        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 0);
                        pUrb->status = 0;
                    }
                    else
                    {
                        dwLength = pUrb->transfer_buffer_length - pEnd->dwOffset;
                        if (dwLength > pEnd->wPacketSize)
                        {
                            printk("[USB]MGC_ServiceRxReady CASE1 %d %d %d\n", dwLength, pUrb->transfer_buffer_length, pEnd->dwOffset);
                            pDmaBuffer = (uint8_t*)pUrb->transfer_dma;
                            pDmaBuffer += pEnd->dwOffset;                          
                            pDmaChannel->dwStartAddress = (uint32_t)pDmaBuffer;
                            pDmaChannel->dwCount = (uint32_t)dwLength;
                            pDmaChannel->dwActualLength = 0;
                            pDmaChannel->bDesiredMode = 1;
                            pEnd->dwRequestSize = dwLength;
                            bDmaOk = MGC_HsDmaProgramChannel(pDmaChannel);
                            if (!bDmaOk)
                            {
                                MGC_HsDmaReleaseChannel(pDmaChannel);
                                pEnd->pDmaChannel = NULL;
                                pEnd->dwRequestSize = 0;
                            }
                        }
                        else
                        {
                            printk("[USB]MGC_ServiceRxReady CASE2 ! %d %d %d\n", dwLength, pUrb->transfer_buffer_length, pEnd->dwOffset);
                            /* release previously-allocated channel */
                            MGC_HsDmaReleaseChannel(pDmaChannel);
                            pEnd->pDmaChannel = NULL;

                            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_H_REQPKT);
                        }
                    }
                }
                else if (MGC_DMA_STATUS_MODE1_SHORTPKT == bStatus)
                {
                    /*
                    This case must be vary careful.
                    Short packet which is received by MGC_RxPacket() will cause bug.
                    When unmap_urb_for_dma() to invalidate cache, MGC_RxPacket() data will be trashed.
                    We use DMA to receive this short packet.
                    */
                    dwLength = min((uint32_t)wRxCount, 
                        (uint32_t)(pUrb->transfer_buffer_length - pUrb->actual_length));

                    /*
                        IntrRxE, IntrUSB, and IntrUSBE are the same 32 bits group.
                        Tricky: Set 0 in all write clear field in IntrUSB field. Prevent to clear IntrUSB.
                        Because our 32 bits register access mode.            
                    */
                    dwIntr = MGC_Read32(pBase, MGC_O_HDRC_INTRRXE);
                    dwIntr &= ~((uint32_t)(1 << bEnd));
                    dwIntr &= 0xFF00FFFE;
                    MGC_Write32(pBase, MGC_O_HDRC_INTRRXE, (uint32_t)dwIntr);

                    // Clear RXCSR will trigger one more endpoint interrupt.
                    wRxCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
                    wRxCsrVal &= ~(MGC_M_RXCSR_AUTOCLEAR |MGC_M_RXCSR_DMAENAB|MGC_M_RXCSR_DMAMODE);
                    MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wRxCsrVal);

                    // Clear rx endpoint interrupt.
                    MGC_Write16(pBase, MGC_O_HDRC_INTRRX, (1 << bEnd));

                    // Enable endpoint interrupt after flush fifo.
                    /*
                        IntrRxE, IntrUSB, and IntrUSBE are the same 32 bits group.
                        Tricky: Set 0 in all write clear field in IntrUSB field. Prevent to clear IntrUSB.
                        Because our 32 bits register access mode.            
                    */
                    dwIntr = MGC_Read32(pBase, MGC_O_HDRC_INTRRXE);
                    dwIntr |= ((uint32_t)(1 << bEnd));
                    dwIntr &= 0xFF00FFFE;
                    MGC_Write32(pBase, MGC_O_HDRC_INTRRXE, (uint32_t)dwIntr);
                    
                    // Use mode 0 to receive short packet.
                    pDmaBuffer = (uint8_t*)pUrb->transfer_dma;
                    pDmaBuffer += pEnd->dwOffset;
                    pDmaChannel->dwStartAddress = (uint32_t)pDmaBuffer;
                    pDmaChannel->dwCount = (uint32_t)dwLength;
                    pDmaChannel->dwActualLength = 0;
                    pDmaChannel->bDesiredMode = 0;
                    pEnd->dwRequestSize = dwLength;
                    bDmaOk = MGC_HsDmaProgramChannel(pDmaChannel);
                    if (!bDmaOk)
                    {
                        MGC_HsDmaReleaseChannel(pDmaChannel);
                        pEnd->pDmaChannel = NULL;
                        pEnd->dwRequestSize = 0;
                    }
                }
            }
            else
            {
                dwLength = min((uint32_t)wRxCount, 
                    (uint32_t)(pUrb->transfer_buffer_length - pUrb->actual_length));

                pEnd->pDmaChannel = 
                    MGC_HsDmaAllocateChannel(&pThis->rDma, bEnd, FALSE, 
                        dwLength, (uint8_t)usb_pipetype(pUrb->pipe), pEnd->wPacketSize);

                if (pEnd->pDmaChannel)
                {
                    pDmaChannel = pEnd->pDmaChannel;

                    // Use mode 0 to receive short packet.
                    pDmaBuffer = (uint8_t*)pUrb->transfer_dma;
                    
                    if (usb_pipeisoc(pUrb->pipe))
                    {
                        packet = &pUrb->iso_frame_desc[pEnd->dwIsoPacket];
                        pDmaBuffer += packet->offset;
                    }
                    else
                    {
                        pDmaBuffer += pEnd->dwOffset;
                    }                    
                    
                    pDmaChannel->dwStartAddress = (uint32_t)pDmaBuffer;
                    pDmaChannel->dwCount = dwLength;
                    pDmaChannel->dwActualLength = 0;
                    pDmaChannel->bDesiredMode = 0;               
                    pEnd->dwRequestSize = dwLength;
                    bDmaOk = MGC_HsDmaProgramChannel(pDmaChannel);

                    if (!bDmaOk)
                    {
                        MGC_HsDmaReleaseChannel(pDmaChannel);

                        pEnd->pDmaChannel = NULL;
                        pEnd->dwRequestSize = 0;

                        (void)MGC_RxPacket(pThis, pEnd, (uint16_t)dwLength);
                    }
                }
                else
                {
                    (void)MGC_RxPacket(pThis, pEnd, (uint16_t)dwLength);
                }
            }
        }
    }

rx_urb_end:
    /* complete the current request or start next one clearing RxPktRdy 
     * and setting ReqPkt */
    if (pUrb->status != ( - EINPROGRESS))
    {
        //  set data toggle in device basis for supporting Hub.
        // get data toggle bit from physical ep.
        toggle = ((MGC_Read32(pBase, M_REG_RXDATATOG)) >> bEnd) &0x01;
        // release dma channel.
        if (pEnd->pDmaChannel)
        {
            /* release previously-allocated channel */
            MGC_HsDmaReleaseChannel(pEnd->pDmaChannel);
            pEnd->pDmaChannel = NULL;
        }

        if (pUrb->status == USB_ST_STALL)
        {
            toggle = 0;
        } 

            /* save data toggle */
        usb_settoggle(pUrb->dev, pEnd->bRemoteEnd, FALSE, toggle);

        MGC_DequeueEndurb(pThis, pEnd, pUrb);

        pEnd->bBusyCompleting = 1;
        spin_unlock(&pThis->SelectEndLock); 
        MGC_CompleteUrb(pThis, pEnd, pUrb);
        spin_lock(&pThis->SelectEndLock);
        pEnd->bBusyCompleting = 0;
        
        if (!MGC_IsEndIdle(pEnd))
        {
            MGC_KickUrb(pThis, MGC_GetNextUrb(pEnd), pEnd);
        }
    }
}

static void MGC_ResetOff(unsigned long param)
{
    uint8_t power;
    MGC_LinuxCd *pThis = (MGC_LinuxCd*)param;
    void *pBase = pThis->pRegs;
    unsigned long flags = 0;

    spin_lock_irqsave(&pThis->SelectEndLock, flags);

    power = MGC_Read8(pBase, MGC_O_HDRC_POWER);
    MGC_Write8(pBase, MGC_O_HDRC_POWER, power &~MGC_M_POWER_RESET);

    /* check for high-speed and set in root device if so */
    power = MGC_Read8(pBase, MGC_O_HDRC_POWER);
    if (power &MGC_M_POWER_HSMODE)
    {
        DBG(3, "[USB]high-speed device connected\n");
        pThis->bRootSpeed = MGC_SPEED_HS;
    }

    // disable reset status.
    pThis->dwVirtualHubPortStatus &= ~(1 << USB_PORT_FEAT_RESET);

    // reset complete. device still connect, set port enable.
    pThis->dwVirtualHubPortStatus |= (1 << USB_PORT_FEAT_C_RESET) | (1 << USB_PORT_FEAT_ENABLE);

    if (pThis->bRootSpeed == MGC_SPEED_HS)
    {
        pThis->dwVirtualHubPortStatus &= ~(1 << USB_PORT_FEAT_LOWSPEED);
        pThis->dwVirtualHubPortStatus |= USB_PORT_STAT_HIGH_SPEED;
    }
    else if (pThis->bRootSpeed == MGC_SPEED_FS)
    {
        pThis->dwVirtualHubPortStatus &= ~(1 << USB_PORT_FEAT_LOWSPEED);
    }
    else
    {
        pThis->dwVirtualHubPortStatus |= (1 << USB_PORT_FEAT_LOWSPEED);
    }

    spin_unlock_irqrestore(&pThis->SelectEndLock, flags);    
}

static void MGC_UnlinkUrb(MGC_LinuxCd *pThis, struct urb *pUrb)
{
    MGC_LinuxLocalEnd *pEnd;
    const void *pBase = pThis->pRegs;
    uint16_t wIntr;

    pEnd = (MGC_LinuxLocalEnd*)pUrb->hcpriv;
    
    if (MGC_GetCurrentUrb(pEnd)== pUrb)
    {
        //printk("[USB]MGC_UnlinkUrb pUrb=%p, bEnd=%d, bIsTx=%d.\n", pUrb, pEnd->bEnd, pEnd->bIsTx);

#ifdef CONFIG_USB_QMU_SUPPORT
        if (pEnd->bCmdQEnable)
        {
            QMU_disable_q(pThis, pEnd->bEnd, !pEnd->bIsTx);
            pEnd->bCmdQEnable = FALSE;
        }        
#endif

        // release dma channel.
        if (pEnd->pDmaChannel)
        {
            /* release previously-allocated channel */
            MGC_HsDmaReleaseChannel(pEnd->pDmaChannel);
            pEnd->pDmaChannel = NULL;
        }
        
        if (pEnd->bEnd)
        {                
            if (!pEnd->bIsTx)                    
            {
                //printk("[USB]MGC_UnlinkUrb: Flush Rx FIFO !\n");
                /* twice in case of double packet buffering */
                MGC_WriteCsr16((uint8_t*)pThis->pRegs, MGC_O_HDRC_RXCSR, pEnd->bEnd, MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);
                MGC_WriteCsr16((uint8_t*)pThis->pRegs, MGC_O_HDRC_RXCSR, pEnd->bEnd, MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);

                // Clear interrupt.
                wIntr = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);
                if (wIntr &(1 << pEnd->bEnd))
                {
                    MGC_Write16(pBase, MGC_O_HDRC_INTRRX, (1 << pEnd->bEnd));
                }
            }
            else
            {                
                //printk("[USB]MGC_UnlinkUrb: Flush Tx FIFO !\n");
                MGC_CheckTxCsrValidForWrite(pBase, pEnd->bEnd);
                /* twice in case of double packet buffering */
                MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, pEnd->bEnd, MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);
                MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, pEnd->bEnd, MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);

                // Clear interrupt.
                wIntr = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
                if (wIntr &(1 << pEnd->bEnd))
                {
                    MGC_Write16(pBase, MGC_O_HDRC_INTRTX, (1 << pEnd->bEnd));
                }                    
            }
        }
        else
        {
            //printk("[USB]MGC_UnlinkUrb: Flush Control FIFO  !\n");
            /* endpoint 0: just flush */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, pEnd->bEnd, MGC_M_CSR0_FLUSHFIFO);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, pEnd->bEnd, MGC_M_CSR0_FLUSHFIFO);

            wIntr = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
            if (wIntr & 1)
            {
                MGC_Write16(pBase, MGC_O_HDRC_INTRTX, 1);
            }
        }
    }

    MGC_DequeueEndurb(pThis, pEnd, pUrb);
}

//-------------------------------------------------------------------------
/** MUC_Irq
 *  system usb irq entry point.
 *  @param   irq                interrupt number.
 *  @param   *_prHcd       point to mtkhcd data structure.
 *  @param   *regs           cpu register before interrupt.
 *  @return  irqreturn_t    irq status: IRQ_HANDLED means successfully irq handled.
 */
//-------------------------------------------------------------------------
static irqreturn_t MUC_Irq(struct usb_hcd *hcd)
{
    MGC_LinuxCd *pThis = hcd_to_musbstruct(hcd);

    const void *pBase = pThis->pRegs;
    uint32_t nSource = 0;
    uint8_t bIntrUsbValue;
    uint16_t wIntrTxValue;
    uint16_t wIntrRxValue;
    uint32_t dwLineState;
    uint8_t bIndex;
    uint8_t bShift = 0;
    uint32_t reg;
#ifdef CONFIG_USB_QMU_SUPPORT
    uint32_t dwIntrQisar;
#endif 

    spin_lock(&pThis->SelectEndLock);

    /* save index */
    bIndex = MGC_Read8(pBase, MGC_O_HDRC_INDEX);

    if ((pThis == NULL) || (hcd->state != HC_STATE_RUNNING))
    {
        printk("[USB]IRQ happens after hcd removed !\n");
        goto exit_muc_irq;
    }

    /* ### DMA intr handler added */
    if (MGC_HsDmaControllerIsr(&pThis->rDma))
    {
        nSource |= 1;
    }

    bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);
    wIntrTxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
    wIntrRxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);
#ifdef CONFIG_USB_QMU_SUPPORT
    dwIntrQisar = MGC_Read32(pBase, M_REG_QISAR);
#endif

    if (bIntrUsbValue)
    {
        MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, bIntrUsbValue);

        bIntrUsbValue &= MGC_Read8(pBase, MGC_O_HDRC_INTRUSBE);
    }

    if (wIntrTxValue)
    {
        MGC_Write16(pBase, MGC_O_HDRC_INTRTX, wIntrTxValue);

        wIntrTxValue &= MGC_Read16(pBase, MGC_O_HDRC_INTRTXE);
    }

    if (wIntrRxValue)
    {
        MGC_Write16(pBase, MGC_O_HDRC_INTRRX, wIntrRxValue);

        wIntrRxValue &= MGC_Read16(pBase, MGC_O_HDRC_INTRRXE);
    }

#ifdef CONFIG_USB_QMU_SUPPORT
    if (dwIntrQisar)
    {
        MGC_Write32(pBase, M_REG_QISAR, dwIntrQisar);
        dwIntrQisar &= ~(MGC_Read32(pBase, M_REG_QIMR));   
    }
#endif

    if (bIntrUsbValue == (MGC_M_INTR_CONNECT | MGC_M_INTR_DISCONNECT))
    {
        DBG(2, "[USB]Connect/Disconnet interrupt = 0x%X.\n", bIntrUsbValue);

        // treat it as disconnect interrupt only.
        bIntrUsbValue &= ~MGC_M_INTR_CONNECT;
    }

    if (bIntrUsbValue &MGC_M_INTR_CONNECT)
    {
        dwLineState = LINESTATE_FS_SPEED;
        DBG(2, "[USB]Connect interrupt  = 0x%X.\n", bIntrUsbValue);

        //kill timer.
        MGC_DelTimer(pThis);

        if ((!pThis->bInsert) && (dwLineState != LINESTATE_DISCONNECT))
        {
            pThis->bCheckInsert = 0;
            MGC_SetTimer(pThis, MGC_CheckConnect, (unsigned long)pThis, MGC_CHECK_INSERT_DEBOUNCE);
        }

        // When connect and disconnet happen at the same time,
        // we only handle connect event. This will happen at Vbus not stable when cable is  inserted.
        bIntrUsbValue &= ~(MGC_M_INTR_CONNECT);
    }
    else if (bIntrUsbValue &MGC_M_INTR_DISCONNECT)
    // disconnect event.   
    {
        DBG(2, "[USB]Disconnect interrupt  = 0x%X.\n", bIntrUsbValue);

        /* <lawrance.liu@mediatek.com> 20100516 */
        /* Patched from BDP, To avoid card reader plug/unplug problem */
        pThis->dwVirtualHubPortStatus = (1 << USB_PORT_FEAT_POWER) | (1 << USB_PORT_FEAT_C_CONNECTION);
        pThis->dwVirtualHubPortStatus &= ~(1 << USB_PORT_FEAT_CONNECTION);
        usb_hcd_poll_rh_status(hcd);

        //kill timer.
        MGC_DelTimer(pThis);

        if (!pThis->bInsert)
        {
            // no need to handle this disconnect event.
            bIntrUsbValue &= ~(MGC_M_INTR_DISCONNECT);
        }
        else
        {
            pThis->bInsert = FALSE;
        }
    }

#ifdef CONFIG_USB_QMU_SUPPORT
    nSource |= bIntrUsbValue | wIntrTxValue | wIntrRxValue |dwIntrQisar;
#else
    nSource |= bIntrUsbValue | wIntrTxValue | wIntrRxValue;
#endif

    if (!nSource)
    {
        goto exit_muc_irq;
    }

    if (bIntrUsbValue)
    {
        MGC_ServiceUSBIntr(pThis, bIntrUsbValue);
    }

#ifdef CONFIG_USB_QMU_SUPPORT
    /* process generic queue interrupt*/
    if (dwIntrQisar)
    {
        (void)QMU_irq(pThis, dwIntrQisar);
    }	
#endif

    /* handle tx/rx on endpoints; each bit of wIntrTxValue is an endpoint, 
     * endpoint 0 first (p35 of the manual) bc is "SPECIAL" treatment; 
     * WARNING: when operating as device you might start receving traffic 
     * to ep0 before anything else happens so be ready for it */

    reg = wIntrTxValue;
    if (reg &1)
    {
        /* EP0 */
        MGC_ServiceDefaultEnd(pThis);
    }

    /* TX on endpoints 1-15 */
    bShift = 1;
    reg >>= 1;

    while (reg)
    {
        if (reg &1)
        {
            MGC_ServiceTxAvail(pThis, bShift);
        }

        reg >>= 1;
        bShift++;
    }

    /* RX on endpoints 1-15 */
    reg = wIntrRxValue;
    bShift = 1;
    reg >>= 1;

    while (reg)
    {
        if (reg &1)
        {
            MGC_ServiceRxReady(pThis, bShift);
        }

        reg >>= 1;
        bShift++;
    }

exit_muc_irq:
    
    /* restore index */
    MGC_Write8(pBase, MGC_O_HDRC_INDEX, bIndex);
    spin_unlock(&pThis->SelectEndLock);
    
    return IRQ_HANDLED;
}

//-------------------------------------------------------------------------
/** MUC_urb_enqueue
 *  queue and process user's urb sequentially.
 *  @param   *hcd           system usb structure.
 *  @param   *urb             user's request block. 
 *  @param   mem_flags    malloc flag. 
 *  @retval   0        Success
 *  @retval  others Fail
 */
//-------------------------------------------------------------------------
static int MUC_urb_enqueue(struct usb_hcd *hcd, struct urb *pUrb, gfp_t mem_flags)
{
    MGC_LinuxCd *pThis = hcd_to_musbstruct(hcd);
    MGC_LinuxLocalEnd *pEnd = NULL;
    unsigned int pipe = pUrb->pipe;
    unsigned long flags = 0;
    int rc;

    spin_lock_irqsave(&pThis->SelectEndLock, flags);

    DBG(2, "[USB]pUrb=0x%p, pUrb->hcpriv=%p proto=%s\n", pUrb, pUrb->hcpriv, MGC_DecodeUrbProtocol(pUrb));

    /* find appropriate local endpoint to do it */
    pEnd = MGC_FindEnd(pThis, pUrb);
    if (pEnd == NULL)
    {
        printk("[USB]==> no resource for proto=%d, addr=%d, end=%d\n", 
            usb_pipetype(pipe), usb_pipedevice(pipe), usb_pipeendpoint(pipe));
        spin_unlock_irqrestore(&pThis->SelectEndLock, flags);
        return USB_ST_URB_REQUEST_ERROR;
    }

    rc = MGC_ScheduleUrb(pThis, pEnd, pUrb);
    if (rc) 
    {
        usb_hcd_unlink_urb_from_ep(hcd, pUrb);
    }
    spin_unlock_irqrestore(&pThis->SelectEndLock, flags);

    return rc;
}

//-------------------------------------------------------------------------
/** MUC_urb_dequeue
 *  kill user's urb.
 *  @param   *hcd           system usb structure.
 *  @param   *urb             user's request block. 
 *  @retval   0        Success
 *  @retval  others Fail
 */
//-------------------------------------------------------------------------
static int MUC_urb_dequeue(struct usb_hcd *hcd, struct urb *pUrb, int status)
{
    MGC_LinuxCd *pThis = hcd_to_musbstruct(hcd);
    int result;
    unsigned long flags;
    
    if (!pUrb)
    {
        DBG(2, "[USB]==> invalid urb%p, pUrb->hcpriv=%p\n", pUrb, (pUrb) ? pUrb->hcpriv: NULL);
        return  - EINVAL;
    } 

    if (!pThis)
    {
        ERR("[USB]==> pThis is null: stopping before unlink\n");
        return  - ENODEV;
    }

    spin_lock_irqsave(&pThis->SelectEndLock, flags);
    result = usb_hcd_check_unlink_urb(hcd, pUrb, status);
    if (result || (!pUrb->hcpriv))
    {
        spin_unlock_irqrestore(&pThis->SelectEndLock, flags);
        return result;
    }

    pUrb->status = status;   
    MGC_UnlinkUrb(pThis, pUrb);

    spin_unlock_irqrestore(&pThis->SelectEndLock, flags);
    
    MGC_CallbackUrb(pThis, pUrb);
    
    return result;
}

//-------------------------------------------------------------------------
/** MUC_endpoint_disable
 *  disable user's ep.
 *  @param   *hcd             system usb hcd structure.
 *  @param   *hdev           system hcd device structure.
 *  @param   epnum          ep number.
 *  @return   void
 */
//-------------------------------------------------------------------------
static void MUC_endpoint_disable(struct usb_hcd *hcd, struct usb_host_endpoint *hep)
{
    MGC_LinuxCd *pThis = hcd_to_musbstruct(hcd);
    unsigned long flags = 0;
    MGC_LinuxLocalEnd *pEnd;

    spin_lock_irqsave(&pThis->SelectEndLock, flags);
    
    if (hep && hep->hcpriv)
    {
        pEnd = (MGC_LinuxLocalEnd*)hep->hcpriv;
        if (pEnd)
        {
            if ((pEnd->bEnd != 0) && (pEnd->bEnd != MGC_END_MSD))
            {
                pEnd->wFifoAddress = MGC_END0_FIFOSIZE; 
                pEnd->wMaxPacketSize = 0;  /* EPx is not occupied */
            }
        
            //printk("[USB]MUC_endpoint_disable: h/w %s ep%d.\n", (pEnd->bIsTx ? "Out": "In"), pEnd->bEnd);           

            if (!MGC_IsEndIdle(pEnd))
            {
                printk("[USB]MUC_endpoint_disable: list not empty.\n");
            }
            
            hep->hcpriv = NULL;           
        }
    }

    spin_unlock_irqrestore(&pThis->SelectEndLock, flags);
}
//-------------------------------------------------------------------------
/** MUC_get_frame
 *  Get sof frame variable.
 *  @param   *hcd             system usb hcd structure.
 *  @return   frame number
 */
//-------------------------------------------------------------------------
static int MUC_get_frame(struct usb_hcd *hcd)
{
    MGC_LinuxCd *pThis = hcd_to_musbstruct(hcd);

    /* wrong except while periodic transfers are scheduled;
     * never matches the on-the-wire frame;
     * subject to overruns.
     */
    //return pThis->frame;
    (void)pThis;
    return 0;
} 

//-------------------------------------------------------------------------
/** MUC_hub_status_data
 *  Virtual root hub status api.
 *  @param   *hcd             system usb hcd structure.
 *  @param   *buf             point to status buffer.
 *  @retval   0        No change hub status.
 *  @retval   1      Change hub status.
 */
//-------------------------------------------------------------------------
static int MUC_hub_status_data(struct usb_hcd *hcd, char *buf)
{
    MGC_LinuxCd *pThis = hcd_to_musbstruct(hcd);
    int32_t retval;
  
    if (!(pThis->dwVirtualHubPortStatus &MTK_PORT_C_MASK))
    {
        retval = 0;
    } 
    else
    {
        // Hub port status change. Port 1 change detected.
        *buf = (1 << 1);

        DBG(3, "[USB]port status 0x%08X has changes\n", pThis->dwVirtualHubPortStatus);
        retval = 1;
    }

    return retval;
}

//-------------------------------------------------------------------------
/** MUC_hub_control
 *  Virtual root hub control api.
 *  @param   *hcd             system usb hcd structure.
 *  @param   typeReq        request code.
 *  @param   wValue         request value.
 *  @param   wIndex         request index.
 *  @param   *buf             point to status buffer.
 *  @param   wLength       data length.
 *  @retval   0        Success.
 *  @retval   others   Fail.
 */
//-------------------------------------------------------------------------
static int MUC_hub_control(struct usb_hcd *hcd, uint16_t typeReq, uint16_t wValue, uint16_t wIndex, char *buf, uint16_t wLength)
{
    MGC_LinuxCd *pThis = hcd_to_musbstruct(hcd);
    void *pBase = pThis->pRegs;
    int32_t retval = 0;
    unsigned long flags;
    uint8_t power;

    DBG(3, "[USB]= 0x%X, 0x%X.\n", typeReq, wValue);

    spin_lock_irqsave(&pThis->SelectEndLock, flags);
  
    switch (typeReq)
    {
        case ClearHubFeature:
        case SetHubFeature:
            switch (wValue)
            {
            case C_HUB_OVER_CURRENT:
            case C_HUB_LOCAL_POWER:
                break;

            default:
                goto error;
            }

            break;

        case ClearPortFeature:
            if (wIndex != 1 || wLength != 0)
            {
                goto error;
            }

            switch (wValue)
            {
            case USB_PORT_FEAT_ENABLE:
                pThis->dwVirtualHubPortStatus &= (1 << USB_PORT_FEAT_POWER);
                break;

            case USB_PORT_FEAT_SUSPEND:
                if (!(pThis->dwVirtualHubPortStatus &(1 << USB_PORT_FEAT_SUSPEND)))
                {
                    break;
                }

                /* 20 msec of resume/K signaling, other irqs blocked */
                DBG(3, "[USB]start resume...\n");
                break;

            case USB_PORT_FEAT_POWER:
                break;

            case USB_PORT_FEAT_C_ENABLE:
            case USB_PORT_FEAT_C_SUSPEND:
            case USB_PORT_FEAT_C_CONNECTION:
            case USB_PORT_FEAT_C_OVER_CURRENT:
            case USB_PORT_FEAT_C_RESET:
                break;

            default:
                goto error;
            }

            pThis->dwVirtualHubPortStatus &= ~(1 << wValue);
            break;

        case GetHubDescriptor:
            MUC_hub_descriptor(pThis, (struct usb_hub_descriptor*)buf);
            break;

        case GetHubStatus:
            *(__le32*)buf = cpu_to_le32(0);
            break;

        case GetPortStatus:
            if (wIndex != 1)
            {
                goto error;
            }

            *(__le32*)buf = cpu_to_le32(pThis->dwVirtualHubPortStatus);

            #ifndef VERBOSE

                if (*(uint16_t*)(buf + 2))
                 /* only if wPortChange is interesting */
            #endif 

            {
                DBG(3, "[USB]GetPortStatus = 0x%08X.\n", pThis->dwVirtualHubPortStatus);
            }

            break;

        case SetPortFeature:
            if (wIndex != 1 || wLength != 0)
            {
                goto error;
            }

            switch (wValue)
            {
            case USB_PORT_FEAT_ENABLE:
                break;

            case USB_PORT_FEAT_SUSPEND:
                if (pThis->dwVirtualHubPortStatus &(1 << USB_PORT_FEAT_RESET))
                {
                    goto error;
                }

                if (!(pThis->dwVirtualHubPortStatus &(1 << USB_PORT_FEAT_ENABLE)))
                {
                    goto error;
                }

                DBG(3, "[USB]suspend...\n");
                break;

            case USB_PORT_FEAT_POWER:
                break;

            case USB_PORT_FEAT_RESET:
                if (pThis->dwVirtualHubPortStatus &(1 << USB_PORT_FEAT_SUSPEND))
                {
                    goto error;
                }

                if (!(pThis->dwVirtualHubPortStatus &(1 << USB_PORT_FEAT_POWER)))
                {
                    break;
                }

                // reset port.
                DBG(3, "[USB]Reset Port \n");
                power = MGC_Read8(pBase, MGC_O_HDRC_POWER);
                power &= ~(MGC_M_POWER_SUSPENDM | MGC_M_POWER_ENSUSPEND);
                MGC_Write8(pBase, MGC_O_HDRC_POWER, power | MGC_M_POWER_RESET);
                MGC_SetTimer(pThis, MGC_ResetOff, (unsigned long)pThis, 80);

                break;

            default:
                goto error;
            }

            pThis->dwVirtualHubPortStatus |= 1 << wValue;
            break;

        default:
            error: 
            /* "protocol stall" on error */
            retval =  - EPIPE;
    }

    spin_unlock_irqrestore(&pThis->SelectEndLock, flags);
    return retval;
}

//-------------------------------------------------------------------------
/** MUC_bus_suspend
 *  Virtual root hub suspend.
 *  @param   *hcd             system usb hcd structure.
 *  @retval   0       Success.
 *  @retval   1      Fail.
 */
//-------------------------------------------------------------------------
static int MUC_bus_suspend(struct usb_hcd *hcd)
{
    // SOFs off
    return 0;
} 

//-------------------------------------------------------------------------
/** MUC_bus_resume
 *  Virtual root hub resume.
 *  @param   *hcd             system usb hcd structure.
 *  @retval   0       Success.
 *  @retval   1      Fail.
 */
//-------------------------------------------------------------------------
static int MUC_bus_resume(struct usb_hcd *hcd)
{
    // SOFs on
    return 0;
} 

//-------------------------------------------------------------------------
/** MUC_hub_descriptor
 *  get root hub descriptor.
 *  @param  *pHcd           mtkhcd pointer.
 *  @param   *desc             point to hub descriptor buffer.
 *  @return   void
 */
//-------------------------------------------------------------------------
static void MUC_hub_descriptor(MGC_LinuxCd *pThis, struct usb_hub_descriptor *desc)
{
    uint16_t temp = 0;

    desc->bDescriptorType = 0x29;
    desc->bHubContrCurrent = 0;

    desc->bNbrPorts = 1;
    desc->bDescLength = 9;

    /* per-port power switching (gang of one!), or none */
    desc->bPwrOn2PwrGood = 10;

    /* no overcurrent errors detection/handling */
    temp = 0x0011;

    desc->wHubCharacteristics = (__force __u16)cpu_to_le16(temp);

    /* two bitmaps:  ports removable, and legacy PortPwrCtrlMask */
    desc->u.hs.DeviceRemovable[0] = 0 << 1;	
    desc->u.hs.DeviceRemovable[1] = ~0;    
} 

/*-------------------------------------------------------------------------*/
static int MUC_start(struct usb_hcd *hcd)
{
    DBG(3, "[USB]hcd = 0x%08X.\n", (uint32_t)hcd);

    hcd->state = HC_STATE_RUNNING;

    return 0;
} 

/*-------------------------------------------------------------------------*/
static void MUC_stop(struct usb_hcd *hcd)
{
    DBG(3, "[USB]hcd = 0x%08X.\n", (uint32_t)hcd);
} 

/*-------------------------------------------------------------------------*/
static int MUC_hcd_probe(struct platform_device *pdev)
{
    uint32_t id = 0;
    int32_t retval;
    MUSB_LinuxController *pController;
    struct usb_hcd *hcd;
    MGC_LinuxCd *pThis;
    uint8_t *pBase;
    uint32_t u4Reg = 0;
    uint8_t intrmask = 0;
    uint8_t i;
    MGC_HsDmaController *pDmaController;

    DBG(3, "[USB]pdev = 0x%08X.\n", (uint32_t)pdev);

    if (!pdev)
    {
        return  - ENODEV;
    } 

    id = pdev->id;
    if (id >= MUCUST_GetControllerNum())
    {
        return  - ENODEV;
    }
    
    pController = MUCUST_GetControllerDev(id);    
    if (!pController)
    {
        return  - ENODEV;
    }

    /* allocate and initialize hcd */
    hcd = usb_create_hcd(&MUC_hc_driver, &pdev->dev, dev_name(&pdev->dev));
    if (!hcd)
    {
        return  - ENOMEM;
    }

    hcd->rsrc_start = (uint32_t)pController->pBase;
    pThis = hcd_to_musbstruct(hcd);
    pThis->bPortNum = id;
    pThis->pRegs = pController->pBase;
    pThis->pPhyBase = pController->pPhyBase;
    pThis->nIrq = pController->dwIrq;
    pThis->bSupportCmdQ = pController->bSupportCmdQ;    
    pThis->dev = &pdev->dev;

    pThis->MGC_pfVbusControl = pController->MGC_pfVbusControl;
    pThis->MGC_pfPhyReset = pController->MGC_pfPhyReset;

    init_timer(&(pThis->Timer));

    // turn off vbus.
    if (pThis->MGC_pfVbusControl)
    {
        pThis->MGC_pfVbusControl(pThis->bPortNum, 0);

        // Wait 5000 ms to turn on vbus.
        MGC_SetTimer(pThis, MGC_VbusControl, (unsigned long)pThis, 5000);
    }
    
    spin_lock_init(&pThis->SelectEndLock);

    // check Phy reset is ok ?
    if ((!pThis->MGC_pfPhyReset) ||
         (pThis->MGC_pfPhyReset(pController->pBase, pController->pPhyBase) < 0))
    {
        return  - ENODEV;            
    }    

    pBase = (uint8_t*)pThis->pRegs;

    // init endpoint, fifo.
    MGC_InitEnd(pThis, pController->bEndpoint_num);
    
    MGC_Write16(pBase, MGC_O_HDRC_INTRTXE, pThis->wEndMask);

    /*
        IntrRxE, IntrUSB, and IntrUSBE are the same 32 bits group.
        Tricky: Set 0 in all write clear field in IntrUSB field. Prevent to clear IntrUSB.
    */
    u4Reg = MGC_M_INTR_SUSPEND | MGC_M_INTR_RESUME | MGC_M_INTR_BABBLE | 
    /* MGC_M_INTR_SOF | */
    MGC_M_INTR_CONNECT | MGC_M_INTR_DISCONNECT | MGC_M_INTR_SESSREQ | MGC_M_INTR_VBUSERROR;
    u4Reg = (u4Reg << 24) | (pThis->wEndMask &0xfffe);
    MGC_Write32(pBase, MGC_O_HDRC_INTRRXE, u4Reg);

    u4Reg = MGC_Read32(pBase, M_REG_INTRLEVEL1EN);
    #ifdef CONFIG_USB_QMU_SUPPORT
    u4Reg |= 0x2f; 
    #else
    u4Reg |= 0x0f;
    #endif
    MGC_Write32(pBase, M_REG_INTRLEVEL1EN, u4Reg);
    DBG(1, "[USB]LEVEL1En reg:0x0x*X \n", MGC_Read8(pBase, M_REG_INTRLEVEL1EN));
    DBG(1, "[USB]INTRUSBE reg:0x0x%X \n", MGC_Read8(pBase, MGC_O_HDRC_INTRUSBE));
    DBG(1, "[USB]INTRTXE  reg:0x0x%X \n", MGC_Read8(pBase, MGC_O_HDRC_INTRTXE));
    DBG(1, "[USB]INTRRXE  reg:0x0x%X \n", MGC_Read8(pBase, MGC_O_HDRC_INTRRXE));

    /* enable high-speed/low-power and start session */
    MGC_Write8(pBase, MGC_O_HDRC_POWER, MGC_M_POWER_SOFTCONN | MGC_M_POWER_HSENAB | MGC_M_POWER_SUSPENDM | MGC_M_POWER_ENSUSPEND);

    /* enable high-speed/low-power and start session & suspend IM host */
    MGC_Write8(pBase, MGC_O_HDRC_DEVCTL, MGC_M_DEVCTL_SESSION);

    // DMA controller init.
    pDmaController = &pThis->rDma;
    memset(pDmaController, 0, sizeof(MGC_HsDmaController));
    pDmaController->pThis = (void *)pThis;
    //reserve the last dma channel for command queue.
    pDmaController->bChannelCount = (pThis->bSupportCmdQ) ? 
        (MGC_HSDMA_CHANNELS-1) : (MGC_HSDMA_CHANNELS);
    for(i=0; i<pDmaController->bChannelCount; i++)
    {
        intrmask |= (1<<i); 
    }	
    MGC_Write8(pBase, MGC_O_HSDMA_INTR_SET, intrmask);
        
#ifdef CONFIG_USB_QMU_SUPPORT
    if (pThis->bSupportCmdQ)
    {
        QMU_init(pThis);
        //reserve the last dma channel for command queue.
        QMU_select_dma_ch(pThis, pDmaController->bChannelCount, 3);
    }
#endif  

    retval = usb_add_hcd(hcd, pController->dwIrq, IRQF_SHARED | IRQF_DISABLED);
    if (retval != 0)
    {
        return  - ENOMEM;
    }

    return 0;
}

/*-------------------------------------------------------------------------*/
static int MUC_hcd_remove(struct platform_device *pdev)
{
    struct usb_hcd *hcd;
#ifdef CONFIG_USB_QMU_SUPPORT
    MGC_LinuxCd *pThis;
#endif

    DBG(3, "[USB]pdev = 0x%08X.\n", (uint32_t)pdev);

    hcd = platform_get_drvdata(pdev);
#ifdef CONFIG_USB_QMU_SUPPORT
    pThis = hcd_to_musbstruct(hcd);
    QMU_disable_all_q(pThis);
    QMU_clean(pThis);
#endif		  
   
    usb_remove_hcd(hcd);
    usb_put_hcd(hcd);
   
    return 0;
} 

/*-------------------------------------------------------------------------*/
static int MUC_hcd_suspend(struct platform_device *pdev, pm_message_t state)
{
    DBG(3, "[USB]pdev = 0x%08X, state.event=%d.\n", (uint32_t)pdev, state.event);
    return 0;
} 

/*-------------------------------------------------------------------------*/
static int MUC_hcd_resume(struct platform_device *pdev)
{
    DBG(3, "[USB]pdev = 0x%08X.\n", (uint32_t)pdev);
    return 0;
} 

/*-------------------------------------------------------------------------*/
static void MUC_hcd_release(struct device *dev)
{
    DBG(3, "[USB]dev = 0x%08X.\n", (uint32_t)dev);
} 

#if defined(CONFIG_MT53XX_MAP_CHANNELB_DRAM) || defined(CONFIG_MT53XX_USE_CHANNELB_DRAM)
#define MTK_HCD_DMA_MASK 0x1fffffff
#else
#define MTK_HCD_DMA_MASK 0xffffffff
#endif

/*-------------------------------------------------------------------------*/
static int __init MUC_init(void)
{
    int32_t retval = 0;
    uint32_t nCount;
    uint32_t i;
    struct platform_device *pPlatformDev;

    if (usb_disabled())
    {
        return  - ENODEV;
    } 

    retval = platform_driver_register(&MUC_hcd_driver);
    if (retval < 0)
    {
        return retval;
    }

#ifdef MUC_DMA_DISABLE
    printk("[USB]Controller in CPU mode\n");
#else
    printk("[USB]Controller in DMA mode\n");
#endif
    nCount = MUCUST_GetControllerNum();
    for (i = 0; i < nCount; i++)
    {
        pPlatformDev = MUCUST_GetPlatformDev(i);
        pPlatformDev->name = MUC_HcdName;
        pPlatformDev->id = i;
        DBG(3, "[USB]pdev = 0x%08X, id = %d.\n", (uint32_t)pPlatformDev, i);
        //  This controller has DMA capability.        
#ifdef MUC_DMA_DISABLE
        pPlatformDev->dev.dma_mask = 0;
        pPlatformDev->dev.coherent_dma_mask = 0;
#else
        pPlatformDev->dev.coherent_dma_mask = MTK_HCD_DMA_MASK;
        pPlatformDev->dev.dma_mask = &pPlatformDev->dev.coherent_dma_mask;
#endif
        pPlatformDev->dev.release = MUC_hcd_release;

        retval = platform_device_register(pPlatformDev);
        if (retval < 0)
        {
            platform_device_unregister(pPlatformDev);
            break;
        } 
    }

    if (retval < 0)
    {
        platform_driver_unregister(&MUC_hcd_driver);
    }

    return retval;
}

/*-------------------------------------------------------------------------*/
static void __exit MUC_cleanup(void)
{
    uint32_t nCount;
    uint32_t i;
    struct platform_device *pPlatformDev;

    nCount = MUCUST_GetControllerNum();
    for (i = 0; i < nCount; i++)
    {
        pPlatformDev = MUCUST_GetPlatformDev(i);
        DBG(3, "[USB]pdev = 0x%08X, id = %d.\n", (uint32_t)pPlatformDev, i);
        platform_device_unregister(pPlatformDev);
    } 

    platform_driver_unregister(&MUC_hcd_driver);

    return ;
}

/*-------------------------------------------------------------------------*/
module_init(MUC_init);
module_exit(MUC_cleanup);
