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
//#include <base/utest.h>
#include <stdlib.h>
#include <time.h>
//#include <bsp/ddr_uarti.h>
//#include <bsp/ddr_misc.h>
//#include <bsp/ddr_ahbmon.h>
//#include <bsp/config.h>
//#include <bsp/ddr_pmu.h>
//#if CACHE_ENABLE
//#include <bsp/ddr_mpu.h>
//#endif
//#include <packetgen.h>
#include <debug.h>
//#include <bsp/ddr_gpt.h>
#include <ddr_usbhost.h>
#include <musbdefs.h>
#include <utest-usb.h>
#include <utest_uniusb.h>
//#include <dvt_common.h>
//#include <bsp/ddr_cirq.h>
//#include <bsp/ddr_cirq_common.h>
#include <utest_transfer.h>

void dev_rx_handler(int ep_num);
void dev_tx_handler(int ep_num);
void dev_dma_handler(int channel);
extern void resetDevFIFO(void);

static unsigned int g_FIFOadd = USB_FIFO_START_ADDRESS;
extern USB_TEST_SETTING g_usb_setting;
extern unsigned char isQMUmode;
extern USB_REQ g_usb_req[TX_FIFO_NUM + RX_FIFO_NUM -1]; // a request for each endpoint
extern USB_DMA_REQ g_usb_dma[DMA_CHANNEL_NUM];
extern void usb_dev_ep_enable(int ep_num, USB_DIR dir, USB_TRANSFER_TYPE type, int maxp, int dpb);

struct USB_DMA_REQ *usb_dev_get_dma_req(int channel){
    return &g_usb_dma[channel-1];
}

struct USB_REQ *usb_dev_get_req(int ep_num, USB_DIR dir){
    int ep_index;

    if(dir == USB_TX){
        ep_index = ep_num;
    }
    else if(dir == USB_RX){
        ep_index = ep_num + TX_FIFO_NUM -1;
    }
    else{
        printf("Error, dir invalid!\r\n");
    }

    return &g_usb_req[ep_index];
}

struct USB_EP_SETTING *usb_dev_get_ep_setting(int ep_num, USB_DIR dir){
    int ep_index;
    if(dir == USB_TX){
        ep_index = ep_num;
    }
    else if(dir == USB_RX){
        ep_index = ep_num + TX_FIFO_NUM -1;
    }
    else{
        printf("Error, dir invalid\n");
    }
    return &g_usb_setting.ep_setting[ep_index];
}

cisr_status_t HdrcDevIsr(unsigned int irq_no, void* pParam){
    hdrc_t* pCtrl = (hdrc_t*)pParam;
    BsrItem *pQItem;
    unsigned char bIntrUsbValue;
    unsigned int dwDmaIntrValue;
    unsigned short wIntrTxValue;
    unsigned short wIntrRxValue;
//	printf("enter dev isr\r\n");
    CIRQ_Mask(irq_no);
    pQItem = malloc(sizeof(BsrItem));
    pQItem->bIntrUsbValue = MGC_Read8(MGC_O_HDRC_INTRUSB) & MGC_Read8(MGC_O_HDRC_INTRUSBE);
    pQItem->dwDmaIntrValue = MGC_Read32(MGC_O_HSDMA_INTR);
    pQItem->wIntrTxValue = MGC_Read16(MGC_O_HDRC_INTRTX) & MGC_Read16(MGC_O_HDRC_INTRTXE);
    pQItem->wIntrRxValue = MGC_Read16(MGC_O_HDRC_INTRRX) & MGC_Read16(MGC_O_HDRC_INTRRXE);
    MGC_Write8(MGC_O_HDRC_INTRUSB, (pQItem->bIntrUsbValue));
    MGC_Write32(MGC_O_HSDMA_INTR, (pQItem->dwDmaIntrValue));
    MGC_Write16(MGC_O_HDRC_INTRTX, (pQItem->wIntrTxValue));
    MGC_Write16(MGC_O_HDRC_INTRRX, (pQItem->wIntrRxValue));
#if 0
	printf("INTRUSB: %d\r\n", pQItem->bIntrUsbValue);
	printf("INTRDMA: %d\r\n", pQItem->dwDmaIntrValue);
	printf("INTRTX: %d\r\n", pQItem->wIntrTxValue);
	printf("INTRRX: %d\r\n", pQItem->wIntrRxValue);
#endif
    CIRQ_ACK(irq_no);
    CIRQ_Unmask(irq_no);
    QueueBackgroundItem(pCtrl, pQItem);
    free(pQItem);
    return CISR_CALL_DSR;
}

void usb_dev_start_transfer(int ep_num, USB_DIR dir){
    USB_REQ *req = usb_dev_get_req(ep_num, dir);
    USB_EP_SETTING *setting = usb_dev_get_ep_setting(ep_num, dir);
    USB_DMA_REQ *dma_req;
    unsigned int dataCount;
    if(dir == USB_TX){
        if(!req->isDma){
            if(req->count - req->actual >= setting->maxp){
                dataCount = setting->maxp;
            }
            else{
                dataCount = req->count - req->actual;
            }
            QMU_HdrcLoadFifo(ep_num, dataCount, req->buf + req->actual);
            req->actual = req->actual + dataCount;
            req->nextTx = TRUE;
            MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR,ep_num) | MGC_M_TXCSR_TXPKTRDY);
        }
        else if(req->isDma){
            dma_req = usb_dev_get_dma_req(req->dmaChannel);
            if(dma_req->dma_mode == 0){
                //set DMAReqEnab=0
                MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, ep_num) & (~MGC_M_TXCSR_DMAENAB));
                //Set DMA Ctrl: , direction=1, interrupt_enable=1, dma_mode=0, burst_mode=3
                MGC_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_IRQENABLE)
                    | (1<<MGC_S_HSDMA_TRANSMIT) & (~(1<<MGC_S_HSDMA_MODE1 )) | (dma_req->burstmode<<MGC_S_HSDMA_BURSTMODE)
                    | (ep_num<<MGC_S_HSDMA_ENDPOINT));
                //dma address
                MGC_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), req->buf);
                //dma count
                if(req->count - req->actual >= setting->maxp){
                    MGC_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), setting->maxp);
                }
                else{
                    MGC_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), (req->count - req->actual));
                }
                //enable DMA
                MGC_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_ENABLE));
            }
            else if(dma_req->dma_mode == 1){
                //set AutoSet=1, DMAReqEnab=1, DMAReqMode=1
                MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, ep_num) | MGC_M_TXCSR_DMAENAB
                    | MGC_M_TXCSR_DMAMODE | MGC_M_TXCSR_AUTOSET);
                //Set DMA addr, count
                MGC_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), req->buf);
                MGC_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), req->count);
                //Set DMA CTRL regs: Direction=1, DMA Mode=1, Interrupt Enable=1, Burst Mode
                MGC_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_TRANSMIT)
                    | (1<<MGC_S_HSDMA_MODE1) | (1<<MGC_S_HSDMA_IRQENABLE) | (dma_req->burstmode<<MGC_S_HSDMA_BURSTMODE)
                    | (ep_num<<MGC_S_HSDMA_ENDPOINT));
                //Enable DMA
                MGC_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_ENABLE));
            }
        }
    }
    else if(dir = USB_RX){
        if(!req->isDma){
            MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num) & ~MGC_M_RXCSR_RXPKTRDY);
        }
        else if(req->isDma){
            dma_req = usb_dev_get_dma_req(req->dmaChannel);
            if(dma_req->dma_mode == 0){
				//set DMA Ctrl: Direction=0, Interrupt Enable=1, DMA Mode=0, Burst Mode
                MGC_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), 0);
                MGC_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_IRQENABLE)
                    & (~(1<<MGC_S_HSDMA_TRANSMIT)) & (~(1<<MGC_S_HSDMA_MODE1 )) | (dma_req->burstmode<<MGC_S_HSDMA_BURSTMODE)
                    | (ep_num<<MGC_S_HSDMA_ENDPOINT));
                MGC_Write16(MGC_O_HDRC_RXCSR, MGC_Read16(MGC_O_HDRC_RXCSR) & ~MGC_M_RXCSR_RXPKTRDY);
            }
            else if(dma_req->dma_mode == 1){
                unsigned int usb_dma_cntl;
                MGC_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), req->buf);
                MGC_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), req->count);
                usb_dma_cntl = (1<<MGC_S_HSDMA_IRQENABLE) & (~(1<<MGC_S_HSDMA_TRANSMIT)) | (1<<MGC_S_HSDMA_MODE1) 
                	| MGC_M_HSDMA_BURSTMODE | (ep_num<<MGC_S_HSDMA_ENDPOINT) | (1<<MGC_S_HSDMA_ENABLE);
                if(dma_req->dma_reqmode == 0){
                    //turn off rx interrupt enable
                    MGC_Write16 (MGC_O_HDRC_INTRRXE, MGC_Read16 (MGC_O_HDRC_INTRRXE) & (~(1 << ep_num)));
                    MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num) | MGC_M_RXCSR_AUTOCLEAR
                       | MGC_M_RXCSR_DMAENAB & (~MGC_M_RXCSR_DMAMODE));
                }
                else if(dma_req->dma_reqmode == 1){
                    //turn on rx interrupt enable
                    MGC_Write16 (MGC_O_HDRC_INTRRXE, MGC_Read16 (MGC_O_HDRC_INTRRXE) | (1 << ep_num));
                    MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num) | MGC_M_RXCSR_AUTOCLEAR
                        | MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE);
                }
                MGC_Write16(MGC_O_HDRC_RXCSR, MGC_Read16(MGC_O_HDRC_RXCSR) & ~MGC_M_RXCSR_RXPKTRDY);
                MGC_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), usb_dma_cntl);
            }
        }
    }
}

void usb_dev_config_req(int ep_num, USB_DIR dir, int count, char isDma, int channel, int burstmode, int dma_mode, int dma_reqmode){
    USB_REQ *req = usb_dev_get_req(ep_num, dir);
    USB_EP_SETTING *setting = usb_dev_get_ep_setting(ep_num, dir);
    USB_DMA_REQ *dma_req;
    req->actual = 0;
    req->complete = FALSE;
    req->current = 0;
    req->count = count;
    req->isDma = FALSE;
    req->nextDma = FALSE;
    req->nextReq = FALSE;
    req->nextTx = FALSE;
    if(isDma){
        dma_req = usb_dev_get_dma_req(channel);
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

void HdrcDevBsr(unsigned int irq_no, unsigned int count, void* pParam){
    hdrc_t* pCtrl = (hdrc_t*)pParam;
    BsrItem item;
    unsigned char bIntrUsbValue;
    unsigned int dwDmaIntrValue;
    unsigned short wIntrTxValue;
    unsigned short wIntrRxValue;
    unsigned int orgMask;
    unsigned int ep_num, channel;
	unsigned char bPower;
    orgMask = ARM_DisableInt();
    int Ok;
    Ok = DequeueBackgroundItem(pCtrl, &item);
    ARM_RestoreInt(orgMask); 
    while(Ok){
        bIntrUsbValue = item.bIntrUsbValue;
        dwDmaIntrValue = item.dwDmaIntrValue;
        wIntrTxValue = item.wIntrTxValue;
        wIntrRxValue = item.wIntrRxValue;
//		printf("enter dev bsr\r\n");
        if(bIntrUsbValue & MGC_M_INTR_RESET){
            printf("Reset interrupt\r\n");
            if(MGC_Read8(MGC_O_HDRC_POWER) & MGC_M_POWER_HSMODE){
                printf("Device: high-speed mode\r\n");
            }
            else{
                printf("Device: full-speed mode\r\n");
            }
			SetAddress(1);
			usb_dev_ep_enable(1,USB_RX,USB_BULK,512,0);
			usb_dev_ep_enable(2,USB_TX,USB_BULK,512,0);
			usb_dev_config_req(1,USB_RX,0,FALSE,1,3,0,0);
			usb_dev_start_transfer(1, USB_RX);
			isReset = 1;
        }
		if(bIntrUsbValue & MGC_M_INTR_SUSPEND){
			printf("suspend interrupt\r\n");
			isSuspend = TRUE;
			//MGC_Write8(MGC_O_HDRC_INTRUSBE, MGC_Read8(MGC_O_HDRC_INTRUSBE) & ~MGC_M_INTR_SUSPEND);
		}
		if(bIntrUsbValue & MGC_M_INTR_RESUME){
			printf("resume interrupt\r\n");
			isSuspend = FALSE;
		}
		if(bIntrUsbValue & MGC_M_INTR_CONNECT){
			printf("connect interrupt\r\n");
			bPower = MGC_Read8(MGC_O_HDRC_POWER);
			printf("Power: 0x%x\r\n", bPower);
			USB_hal_delay_ms(20);
			MGC_Write8(MGC_O_HDRC_POWER, MGC_Read8(MGC_O_HDRC_POWER) & ~MGC_M_POWER_RESET);
			isReset = 1;
		}
		if(bIntrUsbValue & MGC_M_INTR_DISCONNECT){
			printf("disconnect\r\n");
		}
//		printf("wIntrTxValue: %d\r\n", wIntrTxValue);
        for(ep_num = 1; ep_num <= TX_FIFO_NUM; ep_num++)	
        {
            if (wIntrTxValue & (0x01 <<ep_num))
            {
//                printf("enter dev_tx_handler\r\n");
                dev_tx_handler(ep_num);
            }
        }
       for(ep_num = 1; ep_num <= RX_FIFO_NUM; ep_num++){
           if(wIntrRxValue & (0x1 << ep_num)){
               dev_rx_handler(ep_num);
           }
       }
       for(channel = 0; channel < DMA_CHANNEL_NUM; channel++){
            if(dwDmaIntrValue & (1<<channel)){
                dev_dma_handler(channel+1);
            }
        }
        orgMask = ARM_DisableInt();
        Ok = DequeueBackgroundItem(pCtrl, &item);
	  ARM_RestoreInt(orgMask);
    }
}

void dev_dma_handler(int channel){
    USB_DMA_REQ *dma_req = usb_dev_get_dma_req(channel);
    USB_REQ *req = usb_dev_get_req(dma_req->ep_num, dma_req->dir);
    USB_EP_SETTING *setting = usb_dev_get_ep_setting(dma_req->ep_num, dma_req->dir);
    unsigned char txType;
    int i, data_count;
    int tmp, dwDmaRealCnt;
    if(dma_req->dir == USB_TX){
        if(dma_req->dma_mode == 0){
			dwDmaRealCnt = MGC_Read32(MGC_O_HSDMA_REALCNT(dma_req->channel));
			req->actual = req->actual + dwDmaRealCnt;

			if(dwDmaRealCnt < setting->maxp){
			    req->complete = TRUE;
				printf("Dma0: tx[%d] complete\r\n", dma_req->ep_num);
			}
            MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num) | MGC_M_TXCSR_TXPKTRDY);
        }
        else if(dma_req->dma_mode == 1){
			dwDmaRealCnt = MGC_Read32(MGC_O_HSDMA_REALCNT(dma_req->channel));
			req->actual = req->actual + dwDmaRealCnt;
			req->complete = TRUE;
			printf("Dma1: tx[%d] complete\r\n", dma_req->ep_num);
            //clear DMA enab            
            MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num) & (~MGC_M_TXCSR_DMAENAB));
            //clear DMA ReqMode
            MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, bEndpoint, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, bEndpoint) & (~MGC_M_TXCSR_DMAMODE));
			MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num) | MGC_M_TXCSR_TXPKTRDY);
        }
    }
    else if(dma_req->dir == USB_RX){
        dwDmaRealCnt = MGC_Read32(MGC_O_HSDMA_REALCNT(dma_req->channel));
        if(dma_req->dma_mode == 0){
            req->actual = req->actual + dwDmaRealCnt;
            if(dwDmaRealCnt < setting->maxp){
                req->complete = TRUE;
                printf("rx ep[%d] complete, get %d bytes.\r\n", dma_req->ep_num, req->actual);
            }
            else{
            }
            MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, dma_req->ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, dma_req->ep_num) & (~MGC_M_RXCSR_RXPKTRDY));
        }
        else if(dma_req->dma_mode == 1){
            if(dma_req->dma_reqmode == 0){
                if(MGC_QReadCsr16(MGC_O_HDRC_RXCSR, dma_req->ep_num) & MGC_M_RXCSR_RXPKTRDY){
                    MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, dma_req->ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, dma_req->ep_num) & (~MGC_M_RXCSR_RXPKTRDY));
                }
                req->actual = req->actual + dwDmaRealCnt;
                req->complete = TRUE;
                printf("rx ep[%d] complete\r\n", dma_req->ep_num);
            }
            else if(dma_req->dma_reqmode == 1){
            }
        }
    }
    	
}

void dev_tx_handler(int ep_num){
    USB_EP_SETTING *setting = usb_dev_get_ep_setting(ep_num, USB_TX);
    USB_REQ *req = usb_dev_get_req(ep_num, USB_TX);
    USB_DMA_REQ *dma_req;
    unsigned int dataCount;
    if(!req->isDma){
		if(!req->complete){
//			printf("tx actual: %d\r\n", req->actual);
            if(req->count > req->actual){
                if(req->count - req->actual >= setting->maxp){
                    dataCount = setting->maxp;
                }
                else{
                    dataCount = req->count - req->actual;
    				req->complete = TRUE;
                }
                QMU_HdrcLoadFifo(ep_num, dataCount, req->buf + req->actual);
                req->actual = req->actual + dataCount;
                MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR,ep_num) | MGC_M_TXCSR_TXPKTRDY);
            }
            else if(req->count == req->actual && !req->complete) {
                req->complete = TRUE;
                MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR,ep_num) | MGC_M_TXCSR_TXPKTRDY);
    			printf("tx ep[%d] complete!!\r\n", ep_num);
            }
		}
    }
    else if(req->isDma){
        dma_req = usb_dev_get_dma_req(req->dmaChannel);
		if(!req->complete){
	        if(dma_req->dma_mode == 0){
	            if(req->count > req->actual){
	                if(req->count - req->actual >= setting->maxp){
	                    dataCount = setting->maxp;
	                }
	                else{
	                    dataCount = req->count - req->actual;
	                }
	                MGC_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), req->buf + req->actual);
	                MGC_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), dataCount);
	                //enable DMA
	                req->nextDma = TRUE;
	                MGC_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_ENABLE));
	            }
	            else if(req->count == req->actual){
	                req->complete = TRUE;
	                MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, ep_num, MGC_M_TXCSR_TXPKTRDY);
	            }
	        }
	        else if(dma_req->dma_mode == 1){
	        }
        }
    }
}

void dev_rx_handler(int ep_num){
    USB_EP_SETTING *setting = usb_dev_get_ep_setting(ep_num, USB_RX);
    USB_REQ *req = usb_dev_get_req(ep_num, USB_RX);
    USB_DMA_REQ *dma_req;
    unsigned int dwDmaRealCnt;
    unsigned int dwCount = MGC_QReadCsr16(MGC_O_HDRC_RXCOUNT, ep_num);
	unsigned short wCsrVal = MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num);
	if(wCsrVal & MGC_M_RXCSR_INCOMPRX){
		printf("Incomplete Rx[%d]\r\n", nIncompRx);
        	bError = TRUE;
	}
	if(wCsrVal & MGC_M_RXCSR_PIDERROR){
		printf("PID Error[%d]\r\n", nPIDError);		
		bError = TRUE;
	}
	printf("RxCount: %d\r\n", dwCount);
	if(bError){
		return;	
	}
	
    if(!req->isDma){
        QMU_HdrcUnloadFifo (ep_num, dwCount, req->buf + req->actual);
        req->actual += dwCount;
		printf("rx actual: %d\r\n", req->actual);
		if(req->count > 0){
			if(req->actual >= req->count){
				req->complete = TRUE;
				printf("rx[%d] complete\r\n", ep_num);
			}
		}
        else if(dwCount < setting->maxp){
            req->complete = TRUE;
			printf("rx[%d] complete\r\n", ep_num);
        }
		MGC_QWriteCsr16(MGC_O_HDRC_RXCSR,ep_num,MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num) & ~MGC_M_RXCSR_RXPKTRDY);
    }
    else if(req->isDma){
        dma_req = usb_dev_get_dma_req(req->dmaChannel);
        if(dma_req->dma_mode == 0){
            MGC_Write32(MGC_O_HSDMA_ADDRESS1(dma_req->channel), req->buf + req->actual);
            MGC_Write32(MGC_O_HSDMA_COUNT1(dma_req->channel), dwCount);
            MGC_Write16(MGC_O_HSDMA_CONTROLLER(dma_req->channel), MGC_Read16(MGC_O_HSDMA_CONTROLLER(dma_req->channel)) | (1<<MGC_S_HSDMA_ENABLE));
        }
        else if(dma_req->dma_mode == 1){
            if(dma_req->dma_reqmode == 0){
            }
            else if(dma_req->dma_reqmode == 1){
                dwDmaRealCnt = MGC_Read32(MGC_O_HSDMA_REALCNT(dma_req->channel));
                req->actual = dwDmaRealCnt;
                QMU_HdrcUnloadFifo (ep_num, dwCount, req->buf + req->actual);
                MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, 0);
                req->actual = req->actual + dwCount;
                req->complete = TRUE;
            }
        }
    }
}

unsigned char checkDevComplete(int ep_num, USB_DIR dir){
    USB_REQ *req = usb_dev_get_req(ep_num, dir);
    if(req->complete){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

static int QueueBackgroundItem(hdrc_t *pCtrl, BsrItem *pItem)
{   
    BsrItem *temp;
    if (LIST_Empty(&pCtrl->BsrPool)) {
        DBG (3, "QueueBackgroundItem: cannot allocate free BsrItem\r\n" );
        printf("QueueBackgroundItem: cannot allocate free BsrItem\r\n");
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

