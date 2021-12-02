/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*********************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY
* ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS
* DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED FROM MEDIATEK AND/OR
* ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY.
* MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
* WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH
* MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE
* RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED
* HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR
* SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE
* AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN
* ACCORDANCE WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING
* ITS CONFLICT OF LAWS PRINCIPLES.
*********************************************************************/
/*********************************************************************
*  @file           ts_transfer.c
*  @brief          unit test register default value, read/write
*  @author
*  @description    test cases to test unified USB registers
*  @date           2009.10.12
*********************************************************************/

/*******************************************************************************


 * Include files 


******************************************************************************/
#if 0 //!PORTING
#include <base/utest.h>
#include <stdlib.h>
#include <time.h>
#include <bsp/ddr_uarti.h>
#include <bsp/ddr_misc.h>
#include <bsp/ddr_ahbmon.h>
#include <bsp/config.h>
#include <bsp/ddr_pmu.h>
#if CACHE_ENABLE
#include <bsp/ddr_mpu.h>
#endif
#include <packetgen.h>
#include <debug.h>
#include <bsp/ddr_gpt.h>
#include <ddr_usbhost.h>
#include <musbdefs.h>
#include <utest-usb.h>
#include <utest_uniusb.h>
#include <dvt_common.h>
#include <utest_enumeration.h>
#include <bsp/ddr_cirq.h>
#include <bsp/ddr_cirq_common.h>
#include <utest_transfer.h>
#endif
 
#include "dt_debug.h"
//#include "x_rtos.h"
//#include "brd_mem.h"
#include "mu_mem.h"

/*******************************************************************
*        externed function                                                                                   *
*******************************************************************/
void usb_ep_enable(int ep_num, USB_DIR dir, USB_TRANSFER_TYPE type, int maxp, int dpb, int dev_ep_num);

#if 0 // mhzhang
struct USB_REQ *usb_get_req(int ep_num, USB_DIR dir);
struct USB_EP_SETTING *usb_get_ep_setting(int ep_num, USB_DIR dir);
#else
USB_REQ *usb_get_req(int ep_num, USB_DIR dir);
USB_EP_SETTING *usb_get_ep_setting(int ep_num, USB_DIR dir);
#endif


#if 0 //!PORTING
cisr_status_t HdrcIsr(unsigned int irq_no, void* pParam);
#endif
#if 0 //!PORTING
void HdrcBsr(unsigned int irq_no, unsigned int count, void* pParam);
#else
void HdrcBsr(unsigned short wInterruptNumber);
#endif
void dma_handler(int channel);
void tx_handler(int ep_num);
void rx_handler(int ep_num);
void proc_epx_tx(int ep_num);
void proc_epx_rx(int ep_num, unsigned int dwCount);
void checkRxBuf(int ep_num);
void clearRxBuf(int ep_num);
void prepareTxBuf(int ep_num);
void ep0_handler(void);
#if 0 //!PORTING
static int QueueBackgroundItem(hdrc_t *pCtrl, BsrItem *pItem);
static int DequeueBackgroundItem(hdrc_t *pCtrl, BsrItem *pItem);
#endif
void usb_config_ep0_req(int address, USB_EP0_DATASTAGE data_stage, int count, MUSB_DeviceRequest *pDevReq);
unsigned char usb_check_ep0_complete(void);
#if 0 // mhzhang
struct USB_EP0_REQ *usb_get_ep0_req(void);
#else
USB_EP0_REQ *usb_get_ep0_req(void);
#endif

static unsigned int g_FIFOadd = USB_FIFO_START_ADDRESS;
USB_TEST_SETTING g_usb_setting;
USB_REQ g_usb_req[TX_FIFO_NUM + RX_FIFO_NUM -1]; // a request for each endpoint
USB_DMA_REQ g_usb_dma[DMA_CHANNEL_NUM];
USB_EP0_REQ g_usb_ep0_req = {0};
#define _HAL_DELAY_MS_GPTIDX    1
#if 0 //!PORTING
static ddriver_t *gpt_p = NULL;
#endif
extern unsigned char isQMUmode;

void usb_ep_enable(int ep_num, USB_DIR dir, USB_TRANSFER_TYPE type, int maxp, int dpb, int dev_ep_num){
  //  int index;
    int ep_index = 0; // 0 is added by mhzhang
    int used_before;
    unsigned char fifosz;
    // int maxpreg;
    int multipkt;
    USB_EP_SETTING *ep_setting;
    Printf("usb_ep_enable:ep_num = %d, dir = %d\n", ep_num, dir);
   // index = MGC_QREG_Read8(MGC_O_HDRC_INDEX);
    MGC_QMU_Write8(MGC_O_HDRC_INDEX, ep_num);
    if(dir == USB_TX){
        ep_index = ep_num;
    }
    else if(dir == USB_RX){
        ep_index = ep_num + TX_FIFO_NUM - 1;
    }
    else{
        Printf("Error, dir invalid");
    }

    ep_setting = &g_usb_setting.ep_setting[ep_index];
    used_before = ep_setting->fifoaddr;

    if((g_FIFOadd + maxp * (dpb + 1) > USB_RAM_SIZE) && (!used_before)){
        Printf("usb_ep_enable, FAILED: ram exhausted\n");
    }

    ep_setting->dpb = dpb;
    ep_setting->transfer_type = type;
    ep_setting->fifoaddr = g_FIFOadd;    
    ep_setting->fifosz = maxp;
    ep_setting->maxp = maxp;
    ep_setting->dir = dir;
    ep_setting->enabled = 1;
    fifosz = getFIFOsz(maxp, dpb);
    if(dir == USB_TX){
        //set func addr
        //set index
        MGC_QMU_Write8(MGC_O_HDRC_INDEX, ep_num);
        //set txFIFOsz
        MGC_QMU_Write8 (MGC_O_HDRC_TXFIFOSZ, fifosz);
        //set txFIFOadd
        MGC_QMU_Write16 (MGC_O_HDRC_TXFIFOADD, g_FIFOadd/8);
        //set txType, interval
        if(type == USB_INTR){
            MGC_QWriteCsr8 (MGC_O_HDRC_TXTYPE, ep_num, MGC_TYPE_INTERRUPT | dev_ep_num);
            MGC_QWriteCsr8 (MGC_O_HDRC_TXINTERVAL, ep_num, 1);
    	}
        else if(type == USB_BULK){
            MGC_QWriteCsr8 (MGC_O_HDRC_TXTYPE, ep_num, 0x40 | MGC_TYPE_BULK | dev_ep_num);
            MGC_QWriteCsr8 (MGC_O_HDRC_TXINTERVAL, ep_num, 0);
        }
        else if(type == USB_ISO){
            MGC_QWriteCsr8 (MGC_O_HDRC_TXTYPE, ep_num, MGC_TYPE_ISO | dev_ep_num|0x40);
            MGC_QWriteCsr8 (MGC_O_HDRC_TXINTERVAL, ep_num, 0x07); //QMU  1);           
        }
        //set TxMaxP
        if(maxp<=1024){
        	MGC_QWriteCsr16(MGC_O_HDRC_TXMAXP, ep_num, maxp);
        }else{
		multipkt=maxp/1024;
		MGC_QWriteCsr16(MGC_O_HDRC_TXMAXP, ep_num, ((multipkt-1)<<11) |0x400);
        }
        //flush TxFIFO
        MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, ep_num) | MGC_M_TXCSR_FLUSHFIFO);
        MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, ep_num) | MGC_M_TXCSR_FLUSHFIFO);
        //turn on tx interrupt enable
        if(!isQMUmode){
        	MGC_QMU_Write16 (MGC_O_HDRC_INTRTXE, MGC_QREG_Read16 (MGC_O_HDRC_INTRTXE) | (1 << ep_num));
        }
    }
    else if(dir == USB_RX){
        //set index
        MGC_QMU_Write8(MGC_O_HDRC_INDEX, ep_num);
        //set rxFIFOsz
        MGC_QMU_Write8 (MGC_O_HDRC_RXFIFOSZ, fifosz);
        //set rxFIFOadd
        MGC_QMU_Write16 (MGC_O_HDRC_RXFIFOADD, g_FIFOadd/8);
        //set rxType, interval
        if(type == USB_INTR){
            MGC_QWriteCsr8 (MGC_O_HDRC_RXTYPE, ep_num, MGC_TYPE_INTERRUPT | dev_ep_num);
            MGC_QWriteCsr8 (MGC_O_HDRC_RXINTERVAL, ep_num, 1);
        }
        else if(type == USB_BULK){
            MGC_QWriteCsr8 (MGC_O_HDRC_RXTYPE, ep_num, 0x40 | MGC_TYPE_BULK | dev_ep_num);
            MGC_QWriteCsr8 (MGC_O_HDRC_RXINTERVAL, ep_num, 0);
        }
        else if(type == USB_ISO){
            MGC_QWriteCsr8 (MGC_O_HDRC_RXTYPE, ep_num, MGC_TYPE_ISO | dev_ep_num);
            MGC_QWriteCsr8 (MGC_O_HDRC_RXINTERVAL, ep_num, 0x07); // QMU 0x01;
            Printf("RxType:0x%x\r\n", MGC_TYPE_ISO | dev_ep_num);
        }
        //set RxMaxP
        if(maxp<=1024){
        	MGC_QWriteCsr16(MGC_O_HDRC_RXMAXP, ep_num, maxp);
        }else{
		multipkt=maxp/1024;
		MGC_QWriteCsr16(MGC_O_HDRC_RXMAXP, ep_num, ((multipkt-1)<<11) |0x400);
        }
        //flush RxFIFO
        MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num) | MGC_M_RXCSR_FLUSHFIFO);
        MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num) | MGC_M_RXCSR_FLUSHFIFO);
        //turn on rx interrupt enable
        if(!isQMUmode){
        	MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, MGC_QREG_Read16 (MGC_O_HDRC_INTRRXE) | (1 << ep_num));
        }
    }
    if(dpb){
        g_FIFOadd += (maxp*2);
    }
    else{
        g_FIFOadd += (maxp);
    }
}

void minusGlobalFIFOadd(unsigned int num){
    g_FIFOadd = g_FIFOadd - num;
}

void addGlobalFIFOadd(unsigned int num){
    g_FIFOadd = g_FIFOadd + num;
}

USB_DMA_REQ *usb_get_dma_req(int channel){
    return &g_usb_dma[channel-1];
}

USB_REQ *usb_get_req(int ep_num, USB_DIR dir){
    int ep_index = 0; // 0 is added by mhzhang ??

    if(dir == USB_TX){
        ep_index = ep_num;
    }
    else if(dir == USB_RX){
        ep_index = ep_num + TX_FIFO_NUM -1;
    }
    else{
        Printf("Error, dir invalid\n");
    }

    //ben
    if(!g_usb_req[ep_index].buf)
    {
        g_usb_req[ep_index].buf = (unsigned char *)MUSB_MemAlloc(USB_BUF_SIZE);//, 32);
        Printf("g_usb_req[%d].buf@0x%08X\n", ep_index, (unsigned int)g_usb_req[ep_index].buf);
    }

    return &g_usb_req[ep_index];
}

USB_EP_SETTING *usb_get_ep_setting(int ep_num, USB_DIR dir){
    int ep_index = 0; // 0 is added by mhzhang
    if(dir == USB_TX){
        ep_index = ep_num;
    }
    else if(dir == USB_RX){
        ep_index = ep_num + TX_FIFO_NUM -1;
    }
    else{
        Printf("Error, dir invalid\n");
    }
    return &g_usb_setting.ep_setting[ep_index];
}

#if 0 //!PORTING
irqreturn_t HdrcIsr(int irq_no, void* pParam){

    hdrc_t* pCtrl = (hdrc_t*)pParam;
    BsrItem *pQItem;
    unsigned char bIntrUsbValue;
    unsigned int dwDmaIntrValue;
    unsigned short wIntrTxValue;
    unsigned short wIntrRxValue;
    CIRQ_Mask(irq_no);
    pQItem = x_alloc_aligned_mem(sizeof(BsrItem), 32);
    pQItem->bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
    pQItem->dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
    pQItem->wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    pQItem->wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
    MGC_QMU_Write8(MGC_O_HDRC_INTRUSB, (pQItem->bIntrUsbValue));
    MGC_QREG_Write32(MGC_O_HSDMA_INTR, (pQItem->dwDmaIntrValue));
    MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (pQItem->wIntrTxValue));
    MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (pQItem->wIntrRxValue));
    #if 0
    //MGC_QMU_Write16(MGC_O_HDRC_INTRTX, 0);
    pQItem->bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
    pQItem->dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
    pQItem->wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    pQItem->wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
    //write 0 clear, will be modified after design change
    bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
    dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
    //wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
    #if 0
    bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
    dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
    wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
    
    if(bIntrUsbValue != pQItem->bIntrUsbValue){
        Printf("Intr USB different after read: orig[%x] current[%x]", pQItem->bIntrUsbValue, bIntrUsbValue);
    }
    if(dwDmaIntrValue != pQItem->dwDmaIntrValue){
        Printf("Intr DMA different after read: orig[%x] current[%x]", pQItem->dwDmaIntrValue, dwDmaIntrValue);
    }
    if(wIntrTxValue != pQItem->wIntrTxValue){
        Printf("Intr Tx different after read: orig[%x] current[%x]", pQItem->wIntrTxValue, wIntrTxValue);
    }
    if(wIntrRxValue != pQItem->wIntrRxValue){
        Printf("Intr Tx different after read: orig[%x] current[%x]", pQItem->wIntrRxValue, wIntrRxValue);
    }
    #endif
    MGC_QMU_Write8(MGC_O_HDRC_INTRUSB, pQItem->bIntrUsbValue);
    MGC_QREG_Write32(MGC_O_HSDMA_INTR, pQItem->dwDmaIntrValue);
    //MGC_QMU_Write16(MGC_O_HDRC_INTRTX, pQItem->wIntrTxValue);
    MGC_QMU_Write16(MGC_O_HDRC_INTRRX, pQItem->wIntrRxValue);
    bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
    dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
    //wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
    if(bIntrUsbValue != 0){
        Printf("Intr USB doesn't clear after write 1: current[%x]", bIntrUsbValue);
    }
    bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
    dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
    //wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
#if 0
    bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
    dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
    wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
    
    if(bIntrUsbValue != 0){
        Printf("Intr USB doesn't clear after write 1: current[%x]", bIntrUsbValue);
    }
    if(dwDmaIntrValue != 0){
        Printf("Intr DMA doesn't clear after write 1: current[%x]", dwDmaIntrValue);
    }
    if(wIntrTxValue != 0){
        Printf("Intr Tx doesn't clear after write 1: current[%x]", wIntrTxValue);
    }
    if(wIntrRxValue != 0){
        Printf("Intr Rx doesn't clear after write 1: current[%x]", wIntrRxValue);
    }
    #endif
    MGC_QMU_Write8(MGC_O_HDRC_INTRUSB, ~(pQItem->bIntrUsbValue));
    //MGC_QREG_Write32(MGC_O_HSDMA_INTR, ~(pQItem->dwDmaIntrValue));
    //MGC_QMU_Write16(MGC_O_HDRC_INTRTX, ~(pQItem->wIntrTxValue));
    MGC_QMU_Write16(MGC_O_HDRC_INTRRX, ~(pQItem->wIntrRxValue));
    #endif
    CIRQ_ACK(irq_no);
    CIRQ_Unmask(irq_no);
    QueueBackgroundItem(pCtrl, pQItem);
    free(pQItem);
    return CISR_CALL_DSR;
}
#endif

void usb_config_ep0_req(int address, USB_EP0_DATASTAGE data_stage, int count, MUSB_DeviceRequest *pDevReq){
  //  int i;
    USB_EP0_REQ *req = usb_get_ep0_req();
    req->complete = FALSE;
    req->address = address;
    req->txActual = 0;
    req->rxActual = 0;
    req->txCount = count;
    req->rxCount = count;
    req->state = USB_EP0_SETUP;
    req->data_stage = data_stage;

    //config setup buffer data
    req->setupBuf[0] = pDevReq->bmRequestType;
    req->setupBuf[1] = pDevReq->bRequest;
    req->setupBuf[2] = pDevReq->wValue & 0xff;
    req->setupBuf[3] = pDevReq->wValue >> 8;
    req->setupBuf[4] = pDevReq->wIndex & 0xff;
    req->setupBuf[5] = pDevReq->wIndex >> 8;
    req->setupBuf[6] = pDevReq->wLength & 0xff;
    req->setupBuf[7] = pDevReq->wLength >> 8;
}

unsigned char usb_check_ep0_complete(void){
    USB_EP0_REQ *req = usb_get_ep0_req();
    return req->complete;
}

USB_EP0_REQ *usb_get_ep0_req(void){	
    //ben
    if(!g_usb_ep0_req.txBuf)
    {
        g_usb_ep0_req.txBuf = (unsigned char *)MUSB_MemAlloc(USB_BUF_SIZE);//, 32);
    }

    if(!g_usb_ep0_req.rxBuf)
    {
        g_usb_ep0_req.rxBuf = (unsigned char *)MUSB_MemAlloc(USB_BUF_SIZE);//, 32);
    }
    //~ben

    return &g_usb_ep0_req;
}

void usb_start_ep0_transfer(void){
    USB_EP0_REQ *req = usb_get_ep0_req();
    //SetAddress(req->address);
    SetHubAddress(0, req->address, USB_TX);
    //turn off ep0 interrupt
    MGC_QMU_Write16 (MGC_O_HDRC_INTRTXE, MGC_QREG_Read16(MGC_O_HDRC_INTRTXE) & 0x1E);
    MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
    MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
    MGC_QWriteCsr8 (MGC_O_HDRC_NAKLIMIT0, 0, bTxInterval);
    //turn on ep0 interrupt
    MGC_QMU_Write16 (MGC_O_HDRC_INTRTXE, MGC_QREG_Read16(MGC_O_HDRC_INTRTXE) | 0x1);
    QMU_HdrcLoadFifo (0, 8, req->setupBuf);
    MGC_QWriteCsr16(MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_H_NO_PING | MGC_M_CSR0_H_SETUPPKT | MGC_M_CSR0_TXPKTRDY);
}

void usb_start_transfer(int ep_num, USB_DIR dir){
    USB_REQ *req = usb_get_req(ep_num, dir);
    USB_EP_SETTING *setting = usb_get_ep_setting(ep_num, dir);
    USB_DMA_REQ *dma_req;
    unsigned int dataCount;
    if(dir == USB_TX){
		MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_M_TXCSR_FLUSHFIFO);
		MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_M_TXCSR_FLUSHFIFO);
        if(req->isDma){
            dma_req = usb_get_dma_req(req->dmaChannel);
            if(dma_req->dma_mode == 0){
                //set DMAReqEnab=0
                MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, ep_num) & (~MGC_M_TXCSR_DMAENAB));
                //Set DMA Ctrl: , direction=1, interrupt_enable=1, dma_mode=0, burst_mode=3
                MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), 0);
                MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), ((MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel))) | (1<<MGC_S_HSDMA_IRQENABLE)
                    | (1<<MGC_S_HSDMA_TRANSMIT) ) & ((~(1<<MGC_S_HSDMA_MODE1 )) | (dma_req->burstmode<<MGC_S_HSDMA_BURSTMODE)
                    | (ep_num<<MGC_S_HSDMA_ENDPOINT)));
                //dma address                
                MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), (unsigned int)(req->buf));

                //dma count
                if(req->count - req->actual >= setting->maxp){
                    MGC_QREG_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), setting->maxp);
                }
                else{
                    MGC_QREG_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), (req->count - req->actual));
                }
                //enable DMA
                req->actual = req->actual + setting->maxp;
                MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_ENABLE));
            }
            else{
                //set AutoSet=1, DMAReqEnab=1, DMAReqMode=1
                MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, ep_num) | MGC_M_TXCSR_DMAENAB
                    | MGC_M_TXCSR_DMAMODE | MGC_M_TXCSR_AUTOSET);
                //Set DMA addr, count
                MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), (unsigned int)(req->buf));

                MGC_QREG_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), req->count);
                //Set DMA CTRL regs: Direction=1, DMA Mode=1, Interrupt Enable=1, Burst Mode
                MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), 0);
                MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_TRANSMIT)
                    | (1<<MGC_S_HSDMA_MODE1) | (1<<MGC_S_HSDMA_IRQENABLE) | (dma_req->burstmode<<MGC_S_HSDMA_BURSTMODE)
                    | (ep_num<<MGC_S_HSDMA_ENDPOINT));
                //Enable DMA
                MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_ENABLE));
            }
        }
        else{
            if(req->count - req->actual >= setting->maxp){
                dataCount = setting->maxp;
            }
            else{
                dataCount = req->count - req->actual;
                req->complete = TRUE;
            }
            QMU_HdrcLoadFifo(ep_num, dataCount, req->buf + req->actual);
            req->actual = req->actual + dataCount;
//            req->curr = req->curr + setting->maxp;
            req->nextTx = TRUE;
            MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, ep_num, MGC_M_TXCSR_TXPKTRDY);
//            proc_epx_tx(ep_num);
//            MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, ep_num, MGC_M_TXCSR_TXPKTRDY);
        }
    }
    else if(dir == USB_RX){
		MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_M_RXCSR_FLUSHFIFO);
		MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_M_RXCSR_FLUSHFIFO);
        if(req->isDma){
            dma_req = usb_get_dma_req(req->dmaChannel);
            if(dma_req->dma_mode == 0){
                //set DMA Ctrl: Direction=0, Interrupt Enable=1, DMA Mode=0, Burst Mode
                MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), 0);
                MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), ((MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel))) | (1<<MGC_S_HSDMA_IRQENABLE))
                    & (~(1<<MGC_S_HSDMA_TRANSMIT)) & (((~(1<<MGC_S_HSDMA_MODE1 )) | (dma_req->burstmode<<MGC_S_HSDMA_BURSTMODE))
                    | (ep_num<<MGC_S_HSDMA_ENDPOINT)));
                MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_M_RXCSR_H_REQPKT);
            }
            else if(dma_req->dma_mode == 1){
                if(dma_req->dma_reqmode == 0){
                    /*   known size   */
                    //turn off rx interrupt enable
                   MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, MGC_QREG_Read16 (MGC_O_HDRC_INTRRXE) & (~(1 << ep_num)));
                    //set RxCSR: AutoClear=1, DMAReqEnab=1, DMAReqMode=0, AutoReq=1
                   MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, (((MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num)) | MGC_M_RXCSR_AUTOCLEAR)
                       | MGC_M_RXCSR_DMAENAB) & ((~MGC_M_RXCSR_DMAMODE) | MGC_M_RXCSR_H_AUTOREQ));
                    //Set DMA addr, count

#if 0 // mhzhang  ??? 
                   MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), req->buf);
#else
                   MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), (unsigned int)(req->buf));										
#endif

                   if(req->count > 0){
                       MGC_QREG_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), req->count);
                   }
                   else{ 
                       MGC_QREG_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), USB_BUF_SIZE);
                   }
                   //set DMA Ctrl: Direction=0, Interrupt Enable=1, DMA Mode=1, Burst Mode
                   MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), 0);
                   MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), (((MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel))) | (1<<MGC_S_HSDMA_IRQENABLE))
                       & ((~(1<<MGC_S_HSDMA_TRANSMIT)) | (1<<MGC_S_HSDMA_MODE1) | (dma_req->burstmode<<MGC_S_HSDMA_BURSTMODE)
                       | (ep_num<<MGC_S_HSDMA_ENDPOINT))));
                   //enable DMA
                   MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_ENABLE));
                   //request packet
                   MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num) | MGC_M_RXCSR_H_REQPKT);
                }
                else if(dma_req->dma_reqmode == 1){
                    //unknown size
                    //turn on rx interrupt enable
                    MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, MGC_QREG_Read16 (MGC_O_HDRC_INTRRXE) | (1 << ep_num));
                    //set RxCSR: AutoClear=1, DMAReqEnab=1, DMAReqMode=1, AutoReq=1
                    MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num) | MGC_M_RXCSR_AUTOCLEAR
                        | MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE | MGC_M_RXCSR_H_AUTOREQ);
                    //Set DMA addr, count
#if 0 // mhzhang ???                    
                    MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), req->buf);
#else										
                    MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), (unsigned int)(req->buf));										
#endif										
                    if(req->count > 0){
                        MGC_QREG_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), req->count);
                    }
                    else{
                        MGC_QREG_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), USB_BUF_SIZE);
                    }
                    //set DMA Ctrl: Direction=0, Interrupt Enable=1, DMA Mode=1, Burst Mode
                    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), 0);
                    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), (MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_IRQENABLE))
                        & ((((~(1<<MGC_S_HSDMA_TRANSMIT)) | (1<<MGC_S_HSDMA_MODE1)) | (dma_req->burstmode<<MGC_S_HSDMA_BURSTMODE))
                        | (ep_num<<MGC_S_HSDMA_ENDPOINT)));
                    //enable DMA
                    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_ENABLE));
                    //request packet
                    MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num) | MGC_M_RXCSR_H_REQPKT);
                }
            }
        }
        else{
            MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_M_RXCSR_H_REQPKT);
        }
    }
}
void usb_config_req(int ep_num, USB_DIR dir, int count, char isDma, int channel, int burstmode, int dma_mode, int dma_reqmode){
    USB_REQ *req = usb_get_req(ep_num, dir);
//    USB_EP_SETTING *setting = usb_get_ep_setting(ep_num, dir);
    USB_DMA_REQ *dma_req;
    req->actual = 0;
    req->complete = FALSE;
    req->curr = 0;
    req->count = count;
    req->isDma = FALSE;
    req->nextDma = FALSE;
    req->nextReq = FALSE;
    req->nextTx = FALSE;
    if(isDma){
        dma_req = usb_get_dma_req(channel);
        req->dmaChannel = channel;
        req->isDma = TRUE;
        dma_req->burstmode = burstmode;
        dma_req->channel = channel;
        dma_req->dir = dir;
        dma_req->dma_mode = dma_mode;
        dma_req->dma_reqmode = dma_reqmode;
        dma_req->ep_num = ep_num;
    }
}

void HdrcBsr(unsigned short wInterruptNumber){
	// hdrc_t* pCtrl = (hdrc_t*)pParam;
	//BsrItem item;
	unsigned char bIntrUsbValue = 0;
	unsigned int dwDmaIntrValue;
	unsigned short wIntrTxValue = 0; // 0 is added by mhzhang ???
	unsigned short wIntrRxValue = 0;
	// unsigned int orgMask;
	unsigned int ep_num, channel;
	int frameNum, newFrameNum;		 // mhzhang

	#if 0 //!PORTING
	orgMask = ARM_DisableInt();
	int Ok;
	Ok = DequeueBackgroundItem(pCtrl, &item);
	ARM_RestoreInt(orgMask); 
	while(Ok){
	#endif		
	{
		#if 0 // mhzhang for compile error ????
		item.bIntrUsbValue = 0;
		item.dwDmaIntrValue =0;
		item.wIntrTxValue = 0;
		item.wIntrRxValue = 0;
		#endif

		#if 0 //!PORTING
		bIntrUsbValue = item.bIntrUsbValue;
		dwDmaIntrValue = item.dwDmaIntrValue;
		wIntrTxValue = item.wIntrTxValue;
		wIntrRxValue = item.wIntrRxValue;
		#else
		bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
		dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
		wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
		wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
		
		MGC_QMU_Write8(MGC_O_HDRC_INTRUSB, (bIntrUsbValue));
		MGC_QREG_Write32(MGC_O_HSDMA_INTR, (dwDmaIntrValue));
		MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (wIntrTxValue));
		MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (wIntrRxValue));
		#endif
		
		/* handle CONN interrupt */
		if(bIntrUsbValue & MGC_M_INTR_CONNECT){
			MGC_ResetPort();
			return;
		}
		if(bIntrUsbValue & MGC_M_INTR_SESSREQ){
			MGC_ResetPort();
			bSession = TRUE;
			return;
		}
		if(bIntrUsbValue & MGC_M_INTR_RESET){
			Printf("Reset Interrupt \r\n");
			isReset = 1;
		}
		if(bIntrUsbValue & MGC_M_INTR_DISCONNECT){
			Printf("Disconnect \r\n");
			return;
		}
		if(bIntrUsbValue & MGC_M_INTR_BABBLE){
			nBabble++;
			Printf("Babble (%d)\r\n", nBabble);
		}
		if(bIntrUsbValue & MGC_M_INTR_VBUSERROR){
			bError = TRUE;
			Printf("VBus Error \r\n");
		}
		if(bIntrUsbValue & MGC_M_INTR_SUSPEND){
			Printf("suspend interrupt\r\n");
			isSuspend = TRUE;
		}
		if(bIntrUsbValue & MGC_M_INTR_RESUME){
			Printf("resume interrupt\r\n");
			#if 0
			Printf("do resume\r\n");
			MGC_QMU_Write8(MGC_O_HDRC_POWER, (MGC_QREG_Read8(MGC_O_HDRC_POWER) | MGC_M_POWER_RESUME));
			USB_hal_delay_ms(12);
			MGC_QMU_Write8(MGC_O_HDRC_POWER, (MGC_QREG_Read8(MGC_O_HDRC_POWER) & ~MGC_M_POWER_RESUME));
			isSuspend = FALSE;
			Printf("resume done\r\n");
			#endif
			if(MGC_QREG_Read8(MGC_O_HDRC_POWER) & MGC_M_POWER_RESUME){
				Printf("resume up\r\n");
				USB_hal_delay_ms(10);
				MGC_QMU_Write8(MGC_O_HDRC_POWER, (MGC_QREG_Read8(MGC_O_HDRC_POWER) & ~MGC_M_POWER_RESUME));
			}
			else{
				Printf("resume not up\r\n");
			}

			#if 0 // mhzhang			
			int frameNum, newFrameNum;
			#endif			
			frameNum = MGC_QREG_Read16(MGC_O_HDRC_FRAME);
			//wait 3ms
			USB_hal_delay_ms(3);
			//get new frame number
			newFrameNum = MGC_QREG_Read16(MGC_O_HDRC_FRAME);
			//check frame number
			if(frameNum == newFrameNum){
				Printf("Failed: frame number doesn't increase after resume\r\n");
			}
			else{
				Printf("Pass: frame number increase after resume\r\n");
			}
			isSuspend = FALSE;
		}
		if(dwDmaIntrValue){
			for(channel = 0; channel < DMA_CHANNEL_NUM; channel++){
				if(dwDmaIntrValue & (1<<channel)){
					dma_handler(channel+1);
				}
			}
		}
		if(wIntrTxValue & 1){
			ep0_handler();
		}
		if(wIntrTxValue){
			for(ep_num = 1; ep_num <= TX_FIFO_NUM; ep_num++){
				if(wIntrTxValue & (1<<ep_num)){
					tx_handler(ep_num);
				}
			}
		}
		if(wIntrRxValue){
			for(ep_num = 1; ep_num <= RX_FIFO_NUM; ep_num++){
				if(wIntrRxValue & (1<<ep_num)){
					rx_handler(ep_num);
				}
			}
		}
		//free(&item);

		#if 0 //!PORTING
		orgMask = ARM_DisableInt();
		Ok = DequeueBackgroundItem(pCtrl, &item);
		ARM_RestoreInt(orgMask);
		#endif
	}
}

void dma_handler(int channel){
    USB_DMA_REQ *dma_req = usb_get_dma_req(channel);
    USB_REQ *req = usb_get_req(dma_req->ep_num, dma_req->dir);
    USB_EP_SETTING *setting = usb_get_ep_setting(dma_req->ep_num, dma_req->dir);
    unsigned char txType;
    int i;
    int dwDmaRealCnt;  // int tmp;
    if(dma_req->dir == USB_TX){
        if(dma_req->dma_mode == 0){
            txType = MGC_QReadCsr8(MGC_O_HDRC_TXTYPE, dma_req->ep_num);
            if(txType & MGC_TYPE_ISO){
                i=0;
                while(i < 100000){
                    i++;
                }
            }
            req->nextTx = TRUE;
//            Printf("set txpktrdy\r\n");
            dwDmaRealCnt = MGC_QREG_Read32(MGC_O_HSDMA_REALCNT(dma_req->channel));
            if(dwDmaRealCnt < setting->maxp){
                req->complete = TRUE;
            }
            MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num) | MGC_M_TXCSR_TXPKTRDY);
        }
        else{
            //clear DMA enab            
            MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num) & (~MGC_M_TXCSR_DMAENAB));
            //clear DMA ReqMode
            MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, bEndpoint, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, bEndpoint) & (~MGC_M_TXCSR_DMAMODE));
            MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_M_TXCSR_TXPKTRDY);
        }
    }
    else if(dma_req->dir == USB_RX){
        dwDmaRealCnt = MGC_QREG_Read32(MGC_O_HSDMA_REALCNT(dma_req->channel));
        if(dma_req->dma_mode == 0){
            //check if last packet
            req->actual = req->actual + dwDmaRealCnt;
            if(req->count == 0){
                if(dwDmaRealCnt < setting->maxp){
                    req->complete = TRUE;
//                    Printf("rx ep[%d] complete, get %d bytes.\r\n", dma_req->ep_num, req->actual);
                }
                else{
                    MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, dma_req->ep_num, MGC_M_RXCSR_H_REQPKT);
                }
            }
            else{
                if(req->actual < req->count){
                    txType = MGC_QReadCsr8(MGC_O_HDRC_TXTYPE, dma_req->ep_num);
                    if(txType & MGC_TYPE_ISO){
                        i=0;
                        while(i < 100000){
                            i++;
                        }
                    }
                    
                    MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, dma_req->ep_num, MGC_M_RXCSR_H_REQPKT);
                }
                else{
                    req->complete = TRUE;
//                    Printf("rx ep[%d] complete\r\n", dma_req->ep_num);
                }
            }
        }
        else{
            if(dma_req->dma_reqmode == 0){
                if(MGC_QReadCsr16(MGC_O_HDRC_RXCSR, dma_req->ep_num) & MGC_M_RXCSR_RXPKTRDY){
                    MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, dma_req->ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, dma_req->ep_num) & (~MGC_M_RXCSR_RXPKTRDY));
                }
                req->actual = req->actual + dwDmaRealCnt;
                req->complete = TRUE;
//                Printf("rx ep[%d] complete\r\n", dma_req->ep_num);
            }
        }
    }
}

void ep0_handler(void){
    unsigned short wCsrVal;
    unsigned int dwCount;
    unsigned char bVal, bOutVal = 0;
    USB_EP0_REQ *req =usb_get_ep0_req();
    wCsrVal = MGC_QReadCsr16(MGC_O_HDRC_CSR0, 0);
    dwCount = MGC_QReadCsr8(MGC_O_HDRC_COUNT0, 0);
    
    bVal = (unsigned char)wCsrVal;
    if((req->state == USB_EP0_SETUP) && (req->data_stage != USB_EP0_NODATA)
         && (!dwCount) && (wCsrVal & MGC_M_CSR0_RXPKTRDY)){
        Printf("Error: missing data\r\n");
        bError = TRUE;
        MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
        MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
    }
    if (bVal & MGC_M_CSR0_H_RXSTALL) {
        Printf("Stalling Endpoint 0\r\n");
        bError = TRUE;		
    } 
    else if (bVal & MGC_M_CSR0_H_ERROR) {
        Printf("ep0 no response (error)\r\n");
        bError = TRUE;
    } 
    else if (bVal & MGC_M_CSR0_H_NAKTIMEOUT) {
        Printf("ep0 NAK timeoutpEnd->bRetries=%d\r\n", bRetries); 
        if (++bRetries < MUSB_MAX_RETRIES) {
            /* cover it up if retries not exhausted */
            MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, 0);
        } 
        else {
            Printf("no response (NAK timeout)\r\n");
            bRetries = 0;
            bError = TRUE;
        }
    }
    if (bError == TRUE) {
        Printf("Failed: EP0 aborting\r\n");
        /* use the proper sequence to abort the transfer */
        if (bVal & MGC_M_CSR0_H_REQPKT) {
            bVal &= ~MGC_M_CSR0_H_REQPKT;
            MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, bVal);
            bVal &= ~MGC_M_CSR0_H_NAKTIMEOUT;
            MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, bVal);
        } 
        else {
            bVal |= MGC_M_CSR0_FLUSHFIFO;
            MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, bVal);
            MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, bVal);
            bVal &= ~MGC_M_CSR0_H_NAKTIMEOUT;
            MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, bVal);
        }
        MGC_QWriteCsr8 (MGC_O_HDRC_NAKLIMIT0, 0, 0);
        return;
    }
    /* STATUS STAGE */
    if(req->state == USB_EP0_STATUS){
        req->complete = TRUE;
        MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, 0);
    }
    /* DATAIN & DATA STAGE */
    if(req->state == USB_EP0_DATA && req->data_stage == USB_EP0_DATAIN){
        QMU_HdrcUnloadFifo (0, dwCount, req->rxBuf + req->rxActual);
        req->rxActual += dwCount;
        if(req->rxActual < req->rxCount || ((dwCount == dwEP0MaxP) && (req->rxCount == 0))){
            bOutVal = MGC_M_CSR0_H_REQPKT;
        }
        else{
            req->state = USB_EP0_STATUS;
            bOutVal = MGC_M_CSR0_H_STATUSPKT | MGC_M_CSR0_TXPKTRDY;
        }
    }
    /* DATAOUT & DATA STAGE */
    else if(req->state == USB_EP0_DATA && req->data_stage == USB_EP0_DATAOUT){
        if(req->txCount - req->txActual > dwEP0MaxP){
            QMU_HdrcLoadFifo(0, dwEP0MaxP, req->txBuf);
            req->txActual += dwEP0MaxP;
            bOutVal = MGC_M_CSR0_TXPKTRDY;
        }
        else if(req->txCount > req->txActual){
            QMU_HdrcLoadFifo(0, req->txCount - req->txActual, req->txBuf);
            req->txActual = req->txCount;
            bOutVal = MGC_M_CSR0_TXPKTRDY;
        }
        else{
			req->state = USB_EP0_STATUS;
            bOutVal = MGC_M_CSR0_H_STATUSPKT | MGC_M_CSR0_H_REQPKT;
        }
   }
   else if(req->state == USB_EP0_SETUP){
       /* DATAIN & SETUP STAGE */
       if(req->data_stage == USB_EP0_DATAIN){
           req->state = USB_EP0_DATA;
           bOutVal = MGC_M_CSR0_H_REQPKT;
       }
       /* DATAOUT & SETUP STAGE */
       else if(req->data_stage == USB_EP0_DATAOUT){
           if(req->txCount > dwEP0MaxP){
               QMU_HdrcLoadFifo(0, dwEP0MaxP, req->txBuf);
               req->txActual += dwEP0MaxP;
           }
           else{
               QMU_HdrcLoadFifo(0, req->txCount, req->txBuf);
               req->txActual = req->txCount;
           }
           req->state = USB_EP0_DATA;
           bOutVal = MGC_M_CSR0_TXPKTRDY;
       }
       /* NODATA & SETUP STAGE */
       else if(req->data_stage == USB_EP0_NODATA){
           req->state = USB_EP0_STATUS;
           bOutVal = MGC_M_CSR0_H_STATUSPKT | MGC_M_CSR0_H_REQPKT;
       }
    }
    if(bOutVal) {
        MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, bOutVal);
    }
}

void tx_handler(int ep_num){
    unsigned short wCsrVal;
    unsigned char bVal;
    //readTxCSR
    wCsrVal = MGC_QReadCsr16(MGC_O_HDRC_TXCSR, ep_num);
    bVal = (unsigned char)wCsrVal;
    //check error
    if (bVal & MGC_M_TXCSR_H_RXSTALL) {
        Printf("Stalling Endpoint (%d)\r\n", ep_num);
        bError = TRUE;		
    }
    else if (bVal & MGC_M_TXCSR_H_NAKTIMEOUT) {
       Printf("ep(%d) NAK timeoutpEnd\r\n",ep_num);
       bError = TRUE;
    }

    else if(bVal & MGC_M_TXCSR_H_ERROR){
       Printf("ep(%d) Error\r\n",ep_num);
    }

    if (bError == TRUE) {
       Printf("Failed: EP(%d) aborting\r\n", bEndpoint);
       return ;
    }
    if(!bStop){
        proc_epx_tx(ep_num);
    }
}

void rx_handler(int ep_num){
    unsigned short wCsrVal;
    unsigned int dwCount;
    unsigned char bVal;
    //read RxCSR
    wCsrVal = MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num);
    //read RxCount
    dwCount = MGC_QReadCsr16(MGC_O_HDRC_RXCOUNT, ep_num);
    bVal = (unsigned char)wCsrVal;
    if (bVal & MGC_M_RXCSR_H_RXSTALL) {
        Printf("Stalling Endpoint (%d)\r\n", ep_num);
		MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num) & ~MGC_M_RXCSR_H_RXSTALL);
		bStall = TRUE;
    } 
    else if (bVal & MGC_M_RXCSR_H_ERROR) {
        Printf("ep(%d) no response (error)\r\n", ep_num);
		bError = TRUE;
    } 
    else if (bVal & MGC_M_RXCSR_DATAERR) {
        Printf("ep(%d) NAK timeoutpEnd\r\n", ep_num); 
        bError = TRUE;
    }
    if (bError == TRUE) {
        Printf("Failed: EP(%d) aborting\r\n", ep_num);
        return ;
    }
    if(!bStop){
        proc_epx_rx(ep_num, dwCount);
    }
}

void proc_epx_rx(int ep_num, unsigned int dwCount){
    unsigned int dwDmaRealCnt;
    USB_REQ *req = usb_get_req(ep_num, USB_RX);
    USB_EP_SETTING *setting = usb_get_ep_setting(ep_num, USB_RX);
    USB_DMA_REQ *dma_req;
    // int i;
     if(!req->isDma){
        QMU_HdrcUnloadFifo (ep_num, dwCount, req->buf + req->actual);
        //req->curr+= dwCount;
        req->actual+= dwCount;
        if(req->count == 0){
            if(dwCount < setting->maxp){
                req->complete = TRUE;
//                Printf("rx ep[%d] complete, receive %d bytes\r\n", ep_num, req->actual);
            }
            else{
                req->nextReq = TRUE;
                MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_M_RXCSR_H_REQPKT);
            }
        }
        else{
            if (req->actual < req->count) {
                req->nextReq = TRUE;
                MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_M_RXCSR_H_REQPKT);
            }
            else{
                req->complete = TRUE;
//                Printf("rx ep[%d] complete\r\n", ep_num);
            }
        }
    }
    else if(req->isDma){
        dma_req = usb_get_dma_req(req->dmaChannel);
        if(dma_req->dma_mode == 0){
            //Set DMA addr, count
#if 0 // mhzhang            
            MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), req->buf + req->actual);
#else
            MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), (unsigned int)((unsigned int)req->buf + req->actual));
#endif						
            MGC_QREG_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), dwCount);
//            req->curr = req->curr + dwCount;
//            req->actual = req->actual + dwCount;
            //enable DMA
            MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_ENABLE));
        }
        else{
            if(dma_req->dma_reqmode == 1){
                //unload last packet
                dwDmaRealCnt = MGC_QREG_Read32(MGC_O_HSDMA_REALCNT(dma_req->channel));
//                req->curr = dwDmaRealCnt;
                req->actual = dwDmaRealCnt;
                QMU_HdrcUnloadFifo (ep_num, dwCount, req->buf + req->actual);
                MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, 0);
//                req->curr = req->curr + dwCount;
                req->actual = req->actual + dwCount;
                req->complete = TRUE;
//                Printf("rx ep[%d] complete\r\n", ep_num);
            }
        }
    }
}
void proc_epx_tx(int ep_num){
    USB_REQ *req = usb_get_req(ep_num, USB_TX);
    USB_EP_SETTING *setting = usb_get_ep_setting(ep_num, USB_TX);
    USB_DMA_REQ *dma_req;
    unsigned int dataCount;
   // int tmp;
    if(req->isDma){
        dma_req = usb_get_dma_req(req->dmaChannel);
        if(dma_req->dma_mode == 0){
            if(req->count > req->actual){
                if((req->count - req->actual) >= setting->maxp){
                    dataCount = setting->maxp;
                }
                else{
                    dataCount = (req->count - req->actual);
                }

#if 0 // mhzhang	???? 
                MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), req->buf + req->actual);
#else
                MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), (unsigned int)((unsigned int)(req->buf) + req->actual));
#endif

                MGC_QREG_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), dataCount);
//                req->curr = req->curr + setting->maxp;
                req->actual = req->actual + dataCount;
                //enable DMA
                req->nextDma = TRUE;
                MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_ENABLE));
            }
            else if(req->count == req->actual && !req->complete){
                 //Printf("do zlp\r\n");
                //send ZLP
//                req->curr = req->curr + setting->maxp;
                req->complete = TRUE;
//                MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_ENABLE));
                MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, ep_num, MGC_M_TXCSR_TXPKTRDY);
            }
            else{
                //Printf("do complete\r\n");
                req->complete = TRUE;
                req->transferCount++;
//                req->curr = 0;
                if(req->transferCount % 100==0){
//                    Printf("tx ep[%d] complete, count[%d]\r\n", ep_num, req->transferCount);
                }
                if(bNonStop){
                    req->actual = 0;
                }
                //proc_epx_tx(ep_num);
            }
        }
        else if(dma_req->dma_mode == 1){
            
            req->complete = TRUE;
            req->transferCount++;
            if(bNonStop){
//                req->curr = 0;
                req->actual = 0;
                if(req->transferCount % 500== 0){
//                    Printf("tx ep[%d] complete, count[%d]\r\n", ep_num, req->transferCount);
                }
                usb_start_transfer(ep_num, USB_TX);
            }
            else{
//                Printf("tx ep[%d] complete, count[%d]\r\n", ep_num, req->transferCount);
            }
        }
    }
    else if(!req->isDma){
        if(req->count > req->actual){
            if(req->count - req->actual >= setting->maxp){
                dataCount = setting->maxp;
            }
            else{
                dataCount = req->count - req->actual;
            }
            QMU_HdrcLoadFifo(ep_num, dataCount, req->buf + req->actual);
            req->actual = req->actual + dataCount;
            //req->curr = req->curr + setting->maxp;
            req->nextTx = TRUE;
            MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, ep_num, MGC_M_TXCSR_TXPKTRDY);
        }
        else if(req->count == req->actual && !req->complete) {
            req->actual = req->actual + setting->maxp;
            req->complete = TRUE;
            MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, ep_num, MGC_M_TXCSR_TXPKTRDY);
        }
        else{
            req->complete = TRUE;
            req->transferCount++;
           //req->curr = 0;
//            Printf("tx ep[%d] complete, count[%d]\r\n", ep_num, req->transferCount);
//            proc_epx_tx(ep_num);
        }
    }
}

void clearRxBuf(int ep_num){
    int i;
    USB_REQ *req = usb_get_req(ep_num, USB_RX);
    for(i=0; i<req->count; i++){
        req->buf[i] = 0;
    }
}
void checkRxBuf(int ep_num){
   // int index=0; 
    int toggle=0;
    int maxValue=0;
    int indexValue=0;
    int i, j;
    USB_REQ *req = usb_get_req(ep_num, USB_RX);
    USB_EP_SETTING *setting = usb_get_ep_setting(ep_num, USB_RX);
    maxValue = setting->maxp -1;
    if(maxValue >= 256){
        maxValue = 255;
    }
    for(i=0, j=1; i<req->count; i++, j++){
        if(req->buf[i] != indexValue){
            Printf("Failed: Data buffer[%d] is not match expected[%d] value[%d]\r\n", i, indexValue, req->buf[i]);
            bError = TRUE;
        }
        if(toggle == 0){
            indexValue++;
            if(indexValue > maxValue){
                indexValue = 0;
            }
        }
        else{
            indexValue--;
            if(indexValue < 0){
                indexValue = maxValue;
            }
        }
        if(j == setting->maxp){
            toggle = toggle ^ 0x1;
            j=0;
            if(toggle == 0){
                indexValue = 0;
            }
            else{
                indexValue = maxValue;
            }
            
        }
    }
}
void randomTxBuf(int ep_num){   
    int i, value;  // int seed;
    // unsigned max = 0xfffff;
    USB_REQ *req = usb_get_req(ep_num, USB_TX);
    #if 0 //!PORTING
    if ( gpt_p == NULL )
    {
        gpt_p = GPT_GetInstance();
        if ( gpt_p )
        {
            	gpt_p->ioctl(	gpt_p, GPT_IOCTL_START,
            				_HAL_DELAY_MS_GPTIDX,
            				GPT_16K, /* Source : 16KHz */
            				max,
            				1,          /* Auto-repeat mode */
            				NULL,
            				NULL);
        }
    }
    seed = GPTM_DATA(_HAL_DELAY_MS_GPTIDX);
    srand(seed);    
    #endif
    for(i=0; i<req->count; i++){
        #if 0 //!PORTING
        value = rand()%0xff;
        #else
        value = 0x10;
        //value = random32()%0xff;        
        #endif
        if(value == 0xff){
            value = 0xfe;
        }
        req->buf[i] = value;
    }    
}
int USB_get_random(void){
    #if 0//!PORTING
    int i, seed, value;
    unsigned max = 0xfffff;

    if ( gpt_p == NULL )
    {
        gpt_p = GPT_GetInstance();
        if ( gpt_p )
        {
                 	gpt_p->ioctl(	gpt_p, GPT_IOCTL_START,
            				_HAL_DELAY_MS_GPTIDX,
            				GPT_16K, /* Source : 16KHz */
            				max,
            				1,          /* Auto-repeat mode */
            				NULL,
            				NULL);
        }
    }
    seed = GPTM_DATA(_HAL_DELAY_MS_GPTIDX);
//    Printf("seed: [%d]", seed);
    srand(seed);
    return rand();
    #else
    return 0x10;
    //return random32();
    #endif
}
void prepareTxBuf(int ep_num){
    // int index=0; 
    int toggle=0;
    int maxValue=0;
    int indexValue=0;
    int i, j;
    USB_REQ *req = usb_get_req(ep_num, USB_TX);
    USB_EP_SETTING *setting = usb_get_ep_setting(ep_num, USB_TX);
    maxValue = setting->maxp -1;
    if(setting->maxp >= 256){
        maxValue = 255;
    }
    
    for(i=0, j=1; i<req->count; i++, j++){
        req->buf[i] = indexValue;
        if(toggle == 0){
            indexValue++;
            if(indexValue > maxValue){
                indexValue = 0;
            }
        }
        else{
            indexValue--;
            if(indexValue < 0){
                indexValue = maxValue;
            }
        }
        if(j == setting->maxp){
            toggle = toggle ^ 0x1;
            j=0;
            if(toggle == 0){
                indexValue = 0;
            }
            else{
                indexValue = maxValue;
            }
        }
    }
}
unsigned char checkComplete(int ep_num, USB_DIR dir){
    USB_REQ *req = usb_get_req(ep_num, dir);
    if(req->complete){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

void resetFIFO(void){
    g_FIFOadd = USB_FIFO_START_ADDRESS;
}

#if 0//!PORTING
static int QueueBackgroundItem(hdrc_t *pCtrl, BsrItem *pItem)
{   
    BsrItem *temp;
    if (LIST_Empty(&pCtrl->BsrPool)) {
        DBG (3, "QueueBackgroundItem: cannot allocate free BsrItem\r\n" );
        Printf("QueueBackgroundItem: cannot allocate free BsrItem\r\n");
        return 0;
    }
    
    temp=LIST_Entry(pCtrl->BsrPool.next, BsrItem, link);
    LIST_Del(&(temp->link));
    MUSB_MemCopy(temp, pItem, sizeof(BsrItem)-sizeof(llist_t));
    LIST_AddTail(&(temp->link), &pCtrl->BsrList);
    return 1;
}

static int DequeueBackgroundItem(hdrc_t *pCtrl, BsrItem *pItem)
{   
    BsrItem *temp;  
    if (LIST_Empty(&pCtrl->BsrList)) {   
        memset(pItem,0,sizeof(BsrItem)-sizeof(llist_t));
        return 0;
    }
    
    temp=LIST_Entry(pCtrl->BsrList.next, BsrItem, link);
    LIST_Del(&(temp->link));
    MUSB_MemCopy(pItem, temp, sizeof(BsrItem)-sizeof(llist_t));
    LIST_AddTail(&(temp->link), &pCtrl->BsrPool);
    return 1;
}
#endif

