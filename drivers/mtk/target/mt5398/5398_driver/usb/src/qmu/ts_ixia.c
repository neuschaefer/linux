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
*  @file           ts_ixia.c
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
#endif

#include "dt_debug.h"
#include "utest_ixia.h"

/*******************************************************************************


 * LOCAL CONST DEFINATION


******************************************************************************/
//static int usecs;
//static int i;
//static int isPass;
//static int nEnumStep;
//static int nTxEP;
//static int nRxEP;
//static int nCounter;
#if 0 //!PORTING
static mon_statistics_t stat04;	
static mon_analysis_t analy04;    
#endif

/*******************************************************************************


 * LOCAL TYPE DEFINATION


******************************************************************************/
/*******************************************************************************


 * EXTERN DECLARATION


******************************************************************************/
#if 0 //!PORTING
DVT_STATUS_T TS_USB_IXIA_ShowUsage(UINT32 u4TestCaseId);
DVT_STATUS_T TestIxiaLoopback(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
DVT_STATUS_T TestMultiIxiaLoopback(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
DVT_STATUS_T TestIxiaLoopbackSpeedy(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
DVT_STATUS_T TestMultiIxiaLoopbackSpeedy(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
#endif

#if 0 //!PORTING
void SetETHEPConfig();
#else
void SetETHEPConfig(void); 
#endif
void SetMultiETHEPConfig(unsigned int port);
#if 0 //!PORTING
cisr_status_t HdrcIsrIxia(unsigned int irq_no, void* pParam);
cisr_status_t HdrcIsrMultiIxia(unsigned int irq_no, void* pParam);
#endif
void ixia_dma_handler(int channel);
void ixia_tx_handler(int ep_num);
void ixia_rx_handler(int ep_num);
void multi_ixia_dma_handler(int channel);
void multi_ixia_tx_handler(int ep_num);
void multi_ixia_rx_handler(int ep_num);

#if 0
extern cisr_status_t IxiaIsr(unsigned int irq_no, void* pParam);
extern void IxiaBsr(unsigned int irq_no, unsigned int count, void* pParam);
#endif
/*******************************************************************************


 * GLOBAL VARIABLE DEFINATIONS


 ******************************************************************************/
#if 0 //!PORTING
DVT_TEST_CASE_T g_asTS_USB_IXIA_TestCases[] = {
    {DVT_AUTO, "IxiaLoopback", "Ixia LoopBack", TestIxiaLoopback, NULL},
    {DVT_AUTO, "4PortIxiaLoopback", "Ixia LoopBack 4 port", TestMultiIxiaLoopback, NULL},
    {DVT_AUTO, "IxiaLoopbackSpeedy", "Ixia LoopBack speedy", TestIxiaLoopbackSpeedy, NULL},
    {DVT_AUTO, "MultiIxiaLoopbackSpeedy", "Multi Ixia LoopBack speedy", TestMultiIxiaLoopbackSpeedy, NULL},    
    {0, NULL, NULL, NULL, NULL}    
};
#endif

/*******************************************************************************


 * FUNCTION DEFINATIONS


******************************************************************************/
#if 0 //!PORTING
DVT_STATUS_T TS_USB_IXIA_TEST_Init(void **ppvInitStruct){
    DVT_TEST_SUITE_T sTestSuite;
    sTestSuite.pzCommand = "TS_USB_IXIA_TEST";
    sTestSuite.pzDescription = "Test USB ixia stress";
    sTestSuite.psTestCase = g_asTS_USB_IXIA_TestCases;
    sTestSuite.pfUsageFunc = TS_USB_IXIA_ShowUsage;
    DVT_RegisterTestSuite(&sTestSuite);
    *ppvInitStruct = (void *)"TS_USB_IXIA_TEST Init";
    return DVT_SUCCESS;
}

DVT_STATUS_T TS_USB_IXIA_ShowUsage(UINT32 u4TestCaseId){
    switch (u4TestCaseId){
        default:
	     break;
   }
   return DVT_SUCCESS;
}

DVT_STATUS_T TestIxiaLoopback(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput){
    bError = false;
    nEnumStep = 0;
    dwEP0MaxP = 8;
    int isPass = 0;
    int usecs = 30;
    int i = 0;
    int wAddress = 2;
    int assignedAddress = 0;
    int count = 0;
    USB_EP0_DATASTAGE data_stage;
    resetUSB();
    initialTestcase(true);
    if(isPass == -1){
        endTestcase();
        return DVT_FAIL;
    }
    resetFIFO();

    MUSB_DeviceRequest devReq;
    devReq.bmRequestType = MUSB_DIR_IN | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE;
    devReq.bRequest = MUSB_REQ_GET_DESCRIPTOR;
    devReq.wValue = MUSB_DT_DEVICE << 8;
    devReq.wIndex = 0;
    devReq.wLength = 0x40;
    usb_config_ep0_req(0, USB_EP0_DATAIN, 0, &devReq);
    usb_start_ep0_transfer();
    while(!usb_check_ep0_complete() && !bError);
    if(bError){
        psOutput->eType = DVT_OUTPUT_RESULT;
        psOutput->eResult = DVT_RESULT_FAIL;
        return DVT_FAIL;
    }
    Printf("GetDescriptor done\r\n");
    while(enumeration_step[nEnumStep].pDesciptor != NULL){
        MUSB_DeviceRequest *pDevReq = &(enumeration_step[nEnumStep].sDevReq);
        switch (pDevReq->bRequest) {
            case MUSB_REQ_GET_STATUS :
            {
                data_stage = USB_EP0_DATAIN;
                break;
            }
         
            case MUSB_REQ_CLEAR_FEATURE :
            
            {
                data_stage = USB_EP0_DATAOUT;
         
                break;
            }
         
            case MUSB_REQ_GET_DESCRIPTOR :
            
            {

                data_stage = USB_EP0_DATAIN;
                break;
            }          
            case MUSB_REQ_SET_ADDRESS :
            
            {
                 //assignedAddress = 2;
                 
                 pDevReq->bmRequestType = MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE;
                 pDevReq->bRequest = MUSB_REQ_SET_ADDRESS;
                 pDevReq->wValue = wAddress;
                 pDevReq->wIndex = 0x0;
                 pDevReq->wLength = 0x0;
                 data_stage = USB_EP0_NODATA;
                 break;
            }
           
            case MUSB_REQ_GET_CONFIGURATION :
            
           {
               data_stage = USB_EP0_DATAIN;
               break;
           }
 
           case MUSB_REQ_SET_CONFIGURATION :
            
           {
         
                data_stage = USB_EP0_NODATA;
               break;
           }
         
           case 0xFE :

         {

               data_stage = USB_EP0_NODATA;
               break;
          }
         
          default :
 
          {
         
               break;
           }
        
        }

        usb_config_ep0_req(assignedAddress, data_stage, 0, pDevReq);
        usb_start_ep0_transfer();
        while(!usb_check_ep0_complete() && !bError);
        if(bError){
            psOutput->eType = DVT_OUTPUT_RESULT;
            psOutput->eResult = DVT_RESULT_FAIL;
            return DVT_FAIL;
        }
        Printf("ENUM step(%d) done: %s\r\n", nEnumStep, enumeration_step[nEnumStep].pDesciptor);
        if(pDevReq->bRequest == MUSB_REQ_SET_ADDRESS){
            assignedAddress = wAddress;
        }
        if(pDevReq->bRequest==MUSB_REQ_GET_DESCRIPTOR && pDevReq->wValue==(MUSB_DT_CONFIG << 8)){
            SetETHEPConfig();
        }
        nEnumStep++;
    }
    Printf("Enumeration done\r\n");
	#if 0
	nEnumStep = 0;
	while(ethenumeration_step[nEnumStep].pDesciptor != NULL){
	    MUSB_DeviceRequest *pDevReq = &(ethenumeration_step[nEnumStep].sDevReq);
		count = 0;
		if(pDevReq->bmRequestType & MUSB_DIR_IN){
		    data_stage = USB_EP0_DATAIN;
		}
		else{
		    data_stage = USB_EP0_DATAOUT;
		}
		switch(pDevReq->wValue){
		    case AX_CMD_GUSBKR3_12e: 
			{
			    if(i == 0){
				    count = 1;
					USB_EP0_REQ *req = usb_get_ep0_req();
					req->txBuf[0] = 0x11;
			    }
				else if(i == 9){
				    count = 1;
					USB_EP0_REQ *req = usb_get_ep0_req();
					req->txBuf[0] = 0x0c;
				}
				break;
		    }
			case AX_CMD_GUSBKR3_120: 
			{
			    if(pDevReq->bmRequestType& MUSB_DIR_IN){
					count = 0;
			    }
				else{
				    count = 6;
					USB_EP0_REQ *req = usb_get_ep0_req();
					req->txBuf[0] = 0x0;
					req->txBuf[1] = 0x10;
					req->txBuf[2] = 0x60;
					req->txBuf[3] = 0xdb;
					req->txBuf[4] = 0x6c;
					req->txBuf[5] = 0xe6;
				}
				break;
			}
			case AX_CMD_GUSBKR3_126:
			{
				if(pDevReq->bmRequestType& MUSB_DIR_IN){
					count = 0;
			    }
				else{
				    count = 8;
					USB_EP0_REQ *req = usb_get_ep0_req();
					req->txBuf[0] = 0xff;
					req->txBuf[1] = 0xff;
					req->txBuf[2] = 0xff;
					req->txBuf[3] = 0xff;
					req->txBuf[4] = 0xff;
					req->txBuf[5] = 0xff;
					req->txBuf[6] = 0xff;
					req->txBuf[7] = 0xff;
				}
				break;
			}
			case AX_CMD_GUSBKR3_134:
			{
				if(pDevReq->bmRequestType& MUSB_DIR_IN){
					count = 0;
			    }
				else{
				    count = 1;
					USB_EP0_REQ *req = usb_get_ep0_req();
					req->txBuf[0] = 0x80;
				}
				break;
			}
			case AX_CMD_GUSBKR3_12f:
			{
				if(pDevReq->bmRequestType& MUSB_DIR_IN){
					count = 0;
			    }
				else{
				    count = 1;
					USB_EP0_REQ *req = usb_get_ep0_req();
					req->txBuf[0] = 0x18;
				}
				break;
			}
			case AX_CMD_GUSBKR3_130:
			{
				if(pDevReq->bmRequestType& MUSB_DIR_IN){
					count = 0;
			    }
				else{
				    count = 2;
					USB_EP0_REQ *req = usb_get_ep0_req();
					req->txBuf[0] = 0x9e;
					req->txBuf[1] = 0x0;
				}
				break;
			}
			case AX_CMD_GUSBKR3_137:
			{
				if(pDevReq->bmRequestType& MUSB_DIR_IN){
					count = 0;
			    }
				break;
			}
			case AX_CMD_GUSBKR3_02:
			{
				if(pDevReq->bmRequestType& MUSB_DIR_IN){
					count = 0;
			    }
				else{
				    count = 1;
					USB_EP0_REQ *req = usb_get_ep0_req();
					req->txBuf[0] = 0x38;
				}
				break;
			}
			case AX_CMD_GUSBKR3_13e:
			{
				if(pDevReq->bmRequestType& MUSB_DIR_IN){
					count = 0;
			    }
				else{
				    count = 1;
					USB_EP0_REQ *req = usb_get_ep0_req();
					req->txBuf[0] = 0x0;
				}
				break;
			}
		}
		usb_config_ep0_req(assignedAddress, data_stage, count, pDevReq);
        usb_start_ep0_transfer();
        while(!usb_check_ep0_complete() && !bError);
        if(bError){
            psOutput->eType = DVT_OUTPUT_RESULT;
            psOutput->eResult = DVT_RESULT_FAIL;
            return DVT_FAIL;
        }
        Printf("ETHENUM step(%d) done: %s\r\n", nEnumStep, ethenumeration_step[nEnumStep].pDesciptor);
        nEnumStep++;
	}
	#endif
	#if 1
    nEnumStep = 0;
    while(ethenumeration_step[nEnumStep].pDesciptor != NULL){
        MUSB_DeviceRequest *pDevReq = &(ethenumeration_step[nEnumStep].sDevReq);
		count = 0;
        switch(pDevReq->bRequest){
            case AX_CMD_SET_SW_MII:        //0X06
            {
                data_stage = USB_EP0_NODATA;
                break; 
            }
            case AX_CMD_READ_MII_REG:        //0x07
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }
            case AX_CMD_WRITE_MII_REG:        //0x08
            {
                data_stage = USB_EP0_DATAOUT;
                count = 2;
                USB_EP0_REQ *req = usb_get_ep0_req();
                req->txBuf[0] = eth_enum_write_value[eth_enum_write_index][0];
                req->txBuf[1] = eth_enum_write_value[eth_enum_write_index++][1];
                break;
            }
            case AX_CMD_READ_MII_OPERATION_MODE:        //0x09
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }        
            case AX_CMD_SET_HW_MII:        //0x0a
            {  
                data_stage = USB_EP0_NODATA;
                break;
            }
            case AX_CMD_READ_RX_CONTROL_REG:        //0x0f
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }        
            case AX_CMD_WRITE_RX_CTL:        //0x10
            {  
                data_stage = USB_EP0_NODATA;
                break;
            }
            case AX_CMD_READ_IPG012:        //0x11
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }        
            case AX_CMD_WRITE_IPG0:        //0x12    - FullSpeed Used Only
            {  
                data_stage = USB_EP0_NODATA;
                break;
            }
            case AX_CMD_WRITE_IPG1:        //0x13    - FullSpeed Used Only
            {  
                data_stage = USB_EP0_NODATA;
                break;
            }
            case AX_CMD_WRITE_IPG2:        //0x14    - FullSpeed Used Only
            {  
                data_stage = USB_EP0_NODATA;
                break;
            }
            case AX_CMD_READ_MULTIFILTER_ARRAY:        //0x15
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }        
            case AX_CMD_WRITE_MULTI_FILTER: //0x16
            {
                data_stage = USB_EP0_DATAOUT;
                count = 8;
                USB_EP0_REQ *req = usb_get_ep0_req();
                req->txBuf[0] = eth_enum_write_value[eth_enum_write_index][0];
                req->txBuf[1] = eth_enum_write_value[eth_enum_write_index][1];
                req->txBuf[2] = eth_enum_write_value[eth_enum_write_index][2];                
                req->txBuf[3] = eth_enum_write_value[eth_enum_write_index][3];
                req->txBuf[4] = eth_enum_write_value[eth_enum_write_index][4];                
                req->txBuf[5] = eth_enum_write_value[eth_enum_write_index][5];
                req->txBuf[6] = eth_enum_write_value[eth_enum_write_index][6];
                req->txBuf[7] = eth_enum_write_value[eth_enum_write_index++][7];                
                break;
            }
            case AX_CMD_READ_NODE_ID:        //0x17
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }
            case AX_CMD_READ_PHY_ID:        //0x19
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }
            case AX_CMD_READ_MEDIUM_MODE:         // 0x1a
            {
                data_stage = USB_EP0_DATAIN;
                break;    
            }
            case AX_CMD_WRITE_MEDIUM_MODE:        //0x1b    - FullSpeed Used Only
            {  
                data_stage = USB_EP0_NODATA;
                break;
            }
            case AX_CMD_READ_MONITOR_MODE:        //0x1c
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }    
            case AX_CMD_READ_GPIOS:        //0x1e
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }        
            case AX_CMD_WRITE_GPIOS:        //0X1f
            {
                data_stage = USB_EP0_NODATA;
                break;
            } 
            default :
           {    
                Printf (3, "case %d is not support.........\r\n\r\n\r\n", pDevReq->bRequest);
                break;
           }
        }
		
        usb_config_ep0_req(assignedAddress, data_stage, count, pDevReq);
        usb_start_ep0_transfer();
        while(!usb_check_ep0_complete() && !bError);
        if(bError){
            psOutput->eType = DVT_OUTPUT_RESULT;
            psOutput->eResult = DVT_RESULT_FAIL;
            return DVT_FAIL;
        }
        Printf("ETHENUM step(%d) done: %s\r\n", nEnumStep, ethenumeration_step[nEnumStep].pDesciptor);
        nEnumStep++;
    }
	#endif
    Printf("ETH enumeration done\r\n");
    /* options ********************************************************************************/
    int userDecision, userDecision2, userDecision3, userDecision4, userDecision5;
    unsigned char bTxDma, bRxDma;
    unsigned int txDmaMode, rxDmaMode, rxDmaReqMode;
    //RX DMA
    Printf("---------------------------------------------------------------------------\r\n");     
    Printf("- Select Rx DMA                                     -\r\n");     
    Printf("---------------------------------------------------------------------------\r\n");     
    Printf(" (0) RETURN\r\n");    
    Printf(" Rx use DMA? (1)Yes (2)No");
    Printf("\r\n");
    while (DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision, 0, (2)));    
    if(0 == userDecision){        
        psOutput->eResult = DVT_RESULT_IGNORE;        
        return DVT_SUCCESS;    
    }
    if(userDecision == 1){
        bRxDma = true;
    }
    else{
        bRxDma = false;
    }
    //TX DMA
    Printf("---------------------------------------------------------------------------\r\n");     
    Printf("- Select Tx DMA                                     -\r\n");     
    Printf("---------------------------------------------------------------------------\r\n");     
    Printf(" (0) RETURN\r\n");    
    Printf(" Tx use DMA? (1)Yes (2)No");
    Printf("\r\n");
    while (DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision2, 0, (2)));    
    if(0 == userDecision2){        
        psOutput->eResult = DVT_RESULT_IGNORE;        
        return DVT_SUCCESS;    
    }
    if(userDecision2 == 1){
        bTxDma = true;
    }
    else{
        bTxDma = false;
    }
    if(bRxDma){
        //Rx DMA Mode
        Printf("---------------------------------------------------------------------------\r\n");     
        Printf("- Select Rx DMA Mode                                     -\r\n");     
        Printf("---------------------------------------------------------------------------\r\n");     
        Printf(" Rx DMA Mode? 0~1");
        Printf("\r\n");
        while (DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision3, 0, (1)));    
        rxDmaMode = userDecision3;
        //RxDma Req Mode
        Printf("---------------------------------------------------------------------------\r\n");     
        Printf("- Select Rx DMAReq Mode                                     -\r\n");     
        Printf("---------------------------------------------------------------------------\r\n");     
        Printf(" Rx DMAReq Mode? 0~1");
        Printf("\r\n");
        while (DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision4, 0, (1)));    
        rxDmaReqMode = userDecision4;
    }
    if(bTxDma){
        //Tx DMA Mode
        Printf("---------------------------------------------------------------------------\r\n");     
        Printf("- Select Tx DMA Mode                                     -\r\n");     
        Printf("---------------------------------------------------------------------------\r\n");     
        Printf(" Tx DMA Mode? 0~1");
        Printf("\r\n");
        while (DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision5, 0, (1)));    
        txDmaMode = userDecision5;
    }
    /*********************************************************************************************/
    USB_REQ *reqRx = usb_get_req(nRxEP, USB_RX);
    USB_REQ *reqTx = usb_get_req(nTxEP, USB_TX);
    *reqTx->buf = *reqRx->buf;
	SetAddress(assignedAddress);
    int k;
    while(1){
        usb_config_req(nRxEP, USB_RX, 0, bRxDma, 1, 3, rxDmaMode, rxDmaReqMode);
        usb_start_transfer(nRxEP, USB_RX);
        while(!checkComplete(nRxEP, USB_RX));
        usb_config_req(nTxEP, USB_TX, reqRx->actual, bTxDma, 2,3,txDmaMode,0);
        usb_start_transfer(nTxEP, USB_TX);
        while(!checkComplete(nTxEP, USB_TX));
        #if 0
        Printf(" received data: \r\n");
        for(k=0; k<reqRx->actual; k++){
            Printf("%x ", reqRx->buf[k]);
        }
        Printf("\r\n");
        #endif
        #if 0        
        USB_REQ *reqTx = usb_get_req(nTxEP, USB_TX);
        for(k=0; k<reqRx->actual; k++){
            reqTx->buf[k] = reqRx->buf[k];
        }
        #endif
//        Printf("tx complete\r\n");
    }
    
}

DVT_STATUS_T TestMultiIxiaLoopback(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput){
    bError = false;
    nEnumStep = 0;
    dwEP0MaxP = 8;
    int isPass = 0;
    int usecs = 30;
    int i = 0, j=0;;
    int wAddress = 5;
    int assignedAddress = 0;
    int count = 0;
	int devNum = 1;
    USB_EP0_DATASTAGE data_stage;
    resetUSB();
    initialTestcase(true);
    if(isPass == -1){
        endTestcase();
        return DVT_FAIL;
    }
    resetFIFO();

    MUSB_DeviceRequest devReq;
    devReq.bmRequestType = MUSB_DIR_IN | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE;
    devReq.bRequest = MUSB_REQ_GET_DESCRIPTOR;
    devReq.wValue = MUSB_DT_DEVICE << 8;
    devReq.wIndex = 0;
    devReq.wLength = 0x40;
    usb_config_ep0_req(0, USB_EP0_DATAIN, 0, &devReq);
    usb_start_ep0_transfer();
    while(!usb_check_ep0_complete() && !bError);
    if(bError){
        psOutput->eType = DVT_OUTPUT_RESULT;
        psOutput->eResult = DVT_RESULT_FAIL;
        return DVT_FAIL;
    }
    Printf("GetDescriptor done\r\n");
    while(enumeration_step[nEnumStep].pDesciptor != NULL){
        MUSB_DeviceRequest *pDevReq = &(enumeration_step[nEnumStep].sDevReq);
        switch (pDevReq->bRequest) {
            case MUSB_REQ_GET_STATUS :
            {
                data_stage = USB_EP0_DATAIN;
                break;
            }
         
            case MUSB_REQ_CLEAR_FEATURE :
            
            {
                data_stage = USB_EP0_DATAOUT;
         
                break;
            }
         
            case MUSB_REQ_GET_DESCRIPTOR :
            
            {

                data_stage = USB_EP0_DATAIN;
                break;
            }          
            case MUSB_REQ_SET_ADDRESS :
            
            {
                 //assignedAddress = 2;
                 
                 pDevReq->bmRequestType = MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE;
                 pDevReq->bRequest = MUSB_REQ_SET_ADDRESS;
                 pDevReq->wValue = wAddress;
                 pDevReq->wIndex = 0x0;
                 pDevReq->wLength = 0x0;
                 data_stage = USB_EP0_NODATA;
                 break;
            }
           
            case MUSB_REQ_GET_CONFIGURATION :
            
           {
               data_stage = USB_EP0_DATAIN;
               break;
           }
 
           case MUSB_REQ_SET_CONFIGURATION :
            
           {
         
                data_stage = USB_EP0_NODATA;
               break;
           }
         
           case 0xFE :

         {

               data_stage = USB_EP0_NODATA;
               break;
          }
         
          default :
 
          {
         
               break;
           }
        
        }

        usb_config_ep0_req(assignedAddress, data_stage, 0, pDevReq);
        usb_start_ep0_transfer();
        while(!usb_check_ep0_complete() && !bError);
        if(bError){
            psOutput->eType = DVT_OUTPUT_RESULT;
            psOutput->eResult = DVT_RESULT_FAIL;
            return DVT_FAIL;
        }
        Printf("ENUM step(%d) done: %s\r\n", nEnumStep, enumeration_step[nEnumStep].pDesciptor);
        if(pDevReq->bRequest == MUSB_REQ_SET_ADDRESS){
            assignedAddress = wAddress;
        }
        nEnumStep++;
    }
    Printf("Enumeration done\r\n");

    nEnumStep = 0;
    while(hubdevenumeration_step[nEnumStep].pDesciptor != NULL){
        MUSB_DeviceRequest *pDevReq = &(hubdevenumeration_step[nEnumStep].sDevReq);
        switch (pDevReq->bRequest) {
            case MUSB_REQ_GET_STATUS :
            {
                data_stage = USB_EP0_DATAIN;
                break;
            }
			case MUSB_REQ_SET_FEATURE:        
			{            
				data_stage = USB_EP0_NODATA;               
				if ((pDevReq->wValue & 0xff)  == 0x4)            
				{                
				    Printf("delay\r\n");                
					USB_hal_delay_ms(100);            
				}            
				break;        
			}
            case MUSB_REQ_CLEAR_FEATURE :
            {
                data_stage = USB_EP0_NODATA;
				if (pDevReq->bRequest == MUSB_REQ_CLEAR_FEATURE){                
					Printf("delay\r\n");                
					USB_hal_delay_ms(100);            
				}
                break;
            }
            default :
            {
               break;
            }
        }
        usb_config_ep0_req(assignedAddress, data_stage, 0, pDevReq);
        usb_start_ep0_transfer();
        while(!usb_check_ep0_complete() && !bError);
        if(bError){
            psOutput->eType = DVT_OUTPUT_RESULT;
            psOutput->eResult = DVT_RESULT_FAIL;
            return DVT_FAIL;
        }
        Printf("DEV RESERT step(%d) done: %s\r\n", nEnumStep, hubdevenumeration_step[nEnumStep].pDesciptor);
        nEnumStep++;
    }
    Printf("Hub Device Reset done\r\n");
    USB_EP0_REQ *req;
	for(i=1; i<=devNum; i++){
		MGC_QMU_Write8(MGC_O_HDRC_TXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_RXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_TXPORTADDR(0), i);        
		MGC_QMU_Write8(MGC_O_HDRC_RXPORTADDR(0), i);
		MGC_QMU_Write8(0x1a, 0x40);
		
        Printf("set feature reset port [%d]\r\n", i);

		devReq.bmRequestType = MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER;        
		devReq.bRequest = MUSB_REQ_SET_FEATURE;        
		devReq.wValue = 0x4;        
		devReq.wIndex = i;        
		devReq.wLength = 0;

        usb_config_ep0_req(assignedAddress, USB_EP0_NODATA, 0, &devReq);
    
    	usb_start_ep0_transfer();                
    	while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}
		
	    devReq.bmRequestType = MUSB_DIR_IN|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER;        
		devReq.bRequest = MUSB_REQ_GET_STATUS;        
		devReq.wValue = 0;        
		devReq.wIndex = i;        
		devReq.wLength = 0x4;
    	Printf("get status port [%d]\r\n", i);        
    
    	usb_config_ep0_req(assignedAddress, USB_EP0_DATAIN, 0, &devReq);
    
    	usb_start_ep0_transfer();                
    	while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}
    	
    	req = usb_get_ep0_req();
    	if (req->rxBuf[0] & 0x3){            
    	    Printf("port [%d]: ", i);            
    		switch ((req->rxBuf[1] & 0x6) >> 1)            
    		{                
    		    case 0x0:                    
    				Printf("full speed\r\n");                    
    				MGC_QMU_Write8(0x1a, 0x80);                    //fs                    
    				break;                
    			case 0x1:                    
    				Printf("low speed\r\n");                                        
    				MGC_QMU_Write8(0x1a, 0xc0);                                        //ls                    
    				break;                
    			case 0x2:                    
    				Printf("high speed\r\n");                                                            
    				MGC_QMU_Write8(0x1a, 0x40);                                        //hs                    
    				break;            
    		}        
    	}        
    	else{
    		Printf("No device, next!\r\n");
    	    continue;        
    	}

        Printf(" clear feature reset port [%d]\r\n", i);
        
        devReq.bmRequestType = MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER;
		devReq.bRequest = MUSB_REQ_CLEAR_FEATURE;
		devReq.wValue = 20;
		devReq.wIndex = i;
		devReq.wLength = 0;

        usb_config_ep0_req(assignedAddress, USB_EP0_NODATA, 0, &devReq);        
    	usb_start_ep0_transfer();                
    	while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}

		devReq.bmRequestType = MUSB_DIR_IN|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER;        
		devReq.bRequest = MUSB_REQ_GET_STATUS;        
		devReq.wValue = 0;        
		devReq.wIndex = i;        
		devReq.wLength = 0x4;
		MGC_QMU_Write8(MGC_O_HDRC_TXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_RXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_TXPORTADDR(0), i);        
		MGC_QMU_Write8(MGC_O_HDRC_RXPORTADDR(0), i);

    	Printf("get status port [%d]\r\n", i);        
    
    	usb_config_ep0_req(assignedAddress, USB_EP0_DATAIN, 0, &devReq);
    	usb_start_ep0_transfer();                
    	while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}
		req = usb_get_ep0_req();
		Printf("0x%x 0x%x 0x%x 0x%x \r\n"
			, req->rxBuf[0], req->rxBuf[1], req->rxBuf[2], req->rxBuf[3]);
			
        Printf("set address port[%d]\r\n", i);
    	devReq.bmRequestType = MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE;        
    	devReq.bRequest = MUSB_REQ_SET_ADDRESS;        
    	devReq.wValue = i;        
    	devReq.wIndex = 0;        
		devReq.wLength = 0;
    
    	usb_config_ep0_req(0, USB_EP0_NODATA, 0, &devReq);        
    	usb_start_ep0_transfer();                
    	while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}
		Printf("set configure 1 port [%d]\r\n", i);
		devReq.bmRequestType = MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE;        
    	devReq.bRequest = MUSB_REQ_SET_CONFIGURATION;        
    	devReq.wValue = 0x01;        
    	devReq.wIndex = 0;        
		devReq.wLength = 0;
    
    	usb_config_ep0_req(i, USB_EP0_NODATA, 0, &devReq);        
    	usb_start_ep0_transfer();                
    	while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}

	}

    for(i=1; i<=devNum; i++){
		Printf("config port[%d] EP config\r\n", i);
	    devReq.bmRequestType = MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE;        
    	devReq.bRequest = MUSB_REQ_GET_DESCRIPTOR;        
    	devReq.wValue = MUSB_DT_CONFIG << 8;        
    	devReq.wIndex = 0;        
		devReq.wLength = 0x39;

		MGC_QMU_Write8(MGC_O_HDRC_TXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_RXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_TXPORTADDR(0), i);        
		MGC_QMU_Write8(MGC_O_HDRC_RXPORTADDR(0), i);
         
        usb_config_ep0_req(i, USB_EP0_DATAIN, 0, &devReq);
		usb_start_ep0_transfer();
		while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}
		Printf("Get config done!!\r\n");
		SetMultiETHEPConfig(i);
    }

    for(i=1; i<=devNum; i++){
	    nEnumStep = 0;
		eth_enum_write_index = 0;
        while(ethenumeration_step[nEnumStep].pDesciptor != NULL){
            MUSB_DeviceRequest *pDevReq = &(ethenumeration_step[nEnumStep].sDevReq);
    		count = 0;
            switch(pDevReq->bRequest){
                case AX_CMD_SET_SW_MII:        //0X06
                {
                    data_stage = USB_EP0_NODATA;
                    break; 
                }
                case AX_CMD_READ_MII_REG:        //0x07
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }
                case AX_CMD_WRITE_MII_REG:        //0x08
                {
                    data_stage = USB_EP0_DATAOUT;
                    count = 2;
                    USB_EP0_REQ *req = usb_get_ep0_req();
                    req->txBuf[0] = eth_enum_write_value[eth_enum_write_index][0];
                    req->txBuf[1] = eth_enum_write_value[eth_enum_write_index++][1];
                    break;
                }
                case AX_CMD_READ_MII_OPERATION_MODE:        //0x09
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }        
                case AX_CMD_SET_HW_MII:        //0x0a
                {  
                    data_stage = USB_EP0_NODATA;
                    break;
                }
                case AX_CMD_READ_RX_CONTROL_REG:        //0x0f
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }        
                case AX_CMD_WRITE_RX_CTL:        //0x10
                {  
                    data_stage = USB_EP0_NODATA;
                    break;
                }
                case AX_CMD_READ_IPG012:        //0x11
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }        
                case AX_CMD_WRITE_IPG0:        //0x12    - FullSpeed Used Only
                {  
                    data_stage = USB_EP0_NODATA;
                    break;
                }
                case AX_CMD_WRITE_IPG1:        //0x13    - FullSpeed Used Only
                {  
                    data_stage = USB_EP0_NODATA;
                    break;
                }
                case AX_CMD_WRITE_IPG2:        //0x14    - FullSpeed Used Only
                {  
                    data_stage = USB_EP0_NODATA;
                    break;
                }
                case AX_CMD_READ_MULTIFILTER_ARRAY:        //0x15
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }        
                case AX_CMD_WRITE_MULTI_FILTER: //0x16
                {
                    data_stage = USB_EP0_DATAOUT;
                    count = 8;
                    USB_EP0_REQ *req = usb_get_ep0_req();
                    req->txBuf[0] = eth_enum_write_value[eth_enum_write_index][0];
                    req->txBuf[1] = eth_enum_write_value[eth_enum_write_index][1];
                    req->txBuf[2] = eth_enum_write_value[eth_enum_write_index][2];                
                    req->txBuf[3] = eth_enum_write_value[eth_enum_write_index][3];
                    req->txBuf[4] = eth_enum_write_value[eth_enum_write_index][4];                
                    req->txBuf[5] = eth_enum_write_value[eth_enum_write_index][5];
                    req->txBuf[6] = eth_enum_write_value[eth_enum_write_index][6];
                    req->txBuf[7] = eth_enum_write_value[eth_enum_write_index++][7];                
                    break;
                }
                case AX_CMD_READ_NODE_ID:        //0x17
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }
                case AX_CMD_READ_PHY_ID:        //0x19
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }
                case AX_CMD_READ_MEDIUM_MODE:         // 0x1a
                {
                    data_stage = USB_EP0_DATAIN;
                    break;    
                }
                case AX_CMD_WRITE_MEDIUM_MODE:        //0x1b    - FullSpeed Used Only
                {  
                    data_stage = USB_EP0_NODATA;
                    break;
                }
                case AX_CMD_READ_MONITOR_MODE:        //0x1c
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }    
                case AX_CMD_READ_GPIOS:        //0x1e
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }        
                case AX_CMD_WRITE_GPIOS:        //0X1f
                {
                    data_stage = USB_EP0_NODATA;
                    break;
                } 
                default :
               {    
                    Printf (3, "case %d is not support.........\r\n\r\n\r\n", pDevReq->bRequest);
                    break;
               }
            }
    		
            usb_config_ep0_req(i, data_stage, count, pDevReq);
            usb_start_ep0_transfer();
            while(!usb_check_ep0_complete() && !bError);
            if(bError){
                psOutput->eType = DVT_OUTPUT_RESULT;
                psOutput->eResult = DVT_RESULT_FAIL;
                return DVT_FAIL;
            }
            Printf("ETHENUM port (%d) step(%d) done: %s\r\n", i, nEnumStep, ethenumeration_step[nEnumStep].pDesciptor);
            nEnumStep++;
        }
    }
	for(i=1; i<=devNum; i++){
	    USB_REQ *reqRx = usb_get_req(i, USB_RX);
	    USB_REQ *reqTx = usb_get_req(i, USB_TX);
		*reqTx->buf = *reqRx->buf;
		reqTx->complete = true;
	}

	ahbmon_init();    
    mon_statistics_t stat04;    
    mon_analysis_t analy04;    
    // For Traffic Monitor    
    // Specify monitored master and monitored transaction type    
    mon_attr_t attr04 = {            
        .masterID  = MASTER_APS_USB20_ID,            
        .transType  = TRANS_ALL_TYPE,            
        .monAddr   = 0x00000000,//0x6EDFB, //SetupTxDataBuffer,           
        .monAddrMask    = 0xFFFFFFFF // bits set to '1' to indicate DON'T COMPARE    
    };    
    void *mon_mdmcu_param = 0;    
    /** Step 1. Register MDMCU ISR */    
    //ahbmon_register_intr(MON_APMCU_ID, mon_mdmcu_isr, mon_mdmcu_param);    
    /** Step 2: Specify Which Monitor System (MD, AP or WXP) and Its Working Mode */    
    ahbmon_set_sys(MON_APMCU_ID, MON_TRAFFIC_MODE);    
    /** Step 3: Specify Target Bus of the Monitor System */    
    ahbmon_set_bus(BUS_APS_ID);    
    /** Step 4: Set Related Attributes */    
    ahbmon_set_attributes(&attr04);    
//	Printf("press anykey\r\n");
	int userCmd;
//	DVT_GetUserCommand(&userCmd);
    /** Step 5: Kick Off Monitor 1st Time */
    ahbmon_set_op(MON_START_OP);
    USB_hal_delay_ms(1000);
	j=0;
	while(1){
		for(i=1; i<=devNum; i++){
		    if(checkComplete(i, USB_TX)){
			    USB_REQ *reqTx = usb_get_req(i, USB_TX);
				USB_REQ *reqRx = usb_get_req(i, USB_RX);
				reqTx->complete = false;
				usb_config_req(i, USB_RX, 0, 1, i, 3, 1, 1);
				usb_start_transfer(i, USB_RX);
		    }
			if(checkComplete(i, USB_RX)){
				USB_REQ *reqTx = usb_get_req(i, USB_TX);
				USB_REQ *reqRx = usb_get_req(i, USB_RX);
				reqRx->complete = false;
				usb_config_req(i, USB_TX, reqRx->actual, 1, i,3,1,0);
                usb_start_transfer(i, USB_TX);
			}
		}
//		j++;
    }

	
    /** Step 6: Pause Monitor  */
	ahbmon_set_op(MON_PAUSE_OP);
	/** Step 7: Collect Monitor Data */
    ahbmon_get_statistics(&stat04);
    /** Step 8: Generate/Output Statistics Report  */
    ahbmon_dump_statistics("Example 04 Statistics", &stat04);
	/** Step 12: Convert Statistics to Analysis*/
    ahbmon_get_analysis(&stat04, &analy04);    
    /** Step 13: Generate/Output Analysis Report*/    
    ahbmon_dump_analysis("Example 04 Analysis", &analy04);
	#if 0
	/* GDMA */
	Reg_Write32(0x80000008,Reg_Read32(0x80000008) | 2);
	Reg_Write32(0x80050000,0x00000200);
	Reg_Write32(0x80050014,SetupRxDataBuffer);
	Reg_Write32(0x80050018,SetupTxDataBuffer);
	Reg_Write32(0x80050400,0x1);
	Reg_Write32(0x80050010,0xFFFFC0A1);
	#endif
	#if 0
	/* GDMA */
	if(!Reg_Read32(0x80050000) & 1){
	    Reg_Write32(0x80000008,Reg_Read32(0x80000008) | 2);
    	Reg_Write32(0x80050000,0x00000200);
    	Reg_Write32(0x80050014,SetupRxDataBuffer);
    	Reg_Write32(0x80050018,SetupTxDataBuffer);
    	Reg_Write32(0x80050400,0x1);
    	Reg_Write32(0x80050010,0xFFFFC0A1);
	}
	#endif
	
}

DVT_STATUS_T TestMultiIxiaLoopbackSpeedy(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput){
    bError = false;
    nEnumStep = 0;
    dwEP0MaxP = 8;
    int isPass = 0;
    int usecs = 30;
    int i = 0, j=0;;
    int wAddress = 5;
    int assignedAddress = 0;
    int count = 0;
	int devNum = 3;
    USB_EP0_DATASTAGE data_stage;
    resetUSB();
    initialTestcase(true);
    if(isPass == -1){
        endTestcase();
        return DVT_FAIL;
    }
    resetFIFO();

    MUSB_DeviceRequest devReq;
    devReq.bmRequestType = MUSB_DIR_IN | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE;
    devReq.bRequest = MUSB_REQ_GET_DESCRIPTOR;
    devReq.wValue = MUSB_DT_DEVICE << 8;
    devReq.wIndex = 0;
    devReq.wLength = 0x40;
    usb_config_ep0_req(0, USB_EP0_DATAIN, 0, &devReq);
    usb_start_ep0_transfer();
    while(!usb_check_ep0_complete() && !bError);
    if(bError){
        psOutput->eType = DVT_OUTPUT_RESULT;
        psOutput->eResult = DVT_RESULT_FAIL;
        return DVT_FAIL;
    }
    Printf("GetDescriptor done\r\n");
    while(enumeration_step[nEnumStep].pDesciptor != NULL){
        MUSB_DeviceRequest *pDevReq = &(enumeration_step[nEnumStep].sDevReq);
        switch (pDevReq->bRequest) {
            case MUSB_REQ_GET_STATUS :
            {
                data_stage = USB_EP0_DATAIN;
                break;
            }
         
            case MUSB_REQ_CLEAR_FEATURE :
            
            {
                data_stage = USB_EP0_DATAOUT;
         
                break;
            }
         
            case MUSB_REQ_GET_DESCRIPTOR :
            
            {

                data_stage = USB_EP0_DATAIN;
                break;
            }          
            case MUSB_REQ_SET_ADDRESS :
            
            {
                 //assignedAddress = 2;
                 
                 pDevReq->bmRequestType = MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE;
                 pDevReq->bRequest = MUSB_REQ_SET_ADDRESS;
                 pDevReq->wValue = wAddress;
                 pDevReq->wIndex = 0x0;
                 pDevReq->wLength = 0x0;
                 data_stage = USB_EP0_NODATA;
                 break;
            }
           
            case MUSB_REQ_GET_CONFIGURATION :
            
           {
               data_stage = USB_EP0_DATAIN;
               break;
           }
 
           case MUSB_REQ_SET_CONFIGURATION :
            
           {
         
                data_stage = USB_EP0_NODATA;
               break;
           }
         
           case 0xFE :

         {

               data_stage = USB_EP0_NODATA;
               break;
          }
         
          default :
 
          {
         
               break;
           }
        
        }

        usb_config_ep0_req(assignedAddress, data_stage, 0, pDevReq);
        usb_start_ep0_transfer();
        while(!usb_check_ep0_complete() && !bError);
        if(bError){
            psOutput->eType = DVT_OUTPUT_RESULT;
            psOutput->eResult = DVT_RESULT_FAIL;
            return DVT_FAIL;
        }
        Printf("ENUM step(%d) done: %s\r\n", nEnumStep, enumeration_step[nEnumStep].pDesciptor);
        if(pDevReq->bRequest == MUSB_REQ_SET_ADDRESS){
            assignedAddress = wAddress;
        }
        nEnumStep++;
    }
    Printf("Enumeration done\r\n");

    nEnumStep = 0;
    while(hubdevenumeration_step[nEnumStep].pDesciptor != NULL){
        MUSB_DeviceRequest *pDevReq = &(hubdevenumeration_step[nEnumStep].sDevReq);
        switch (pDevReq->bRequest) {
            case MUSB_REQ_GET_STATUS :
            {
                data_stage = USB_EP0_DATAIN;
                break;
            }
			case MUSB_REQ_SET_FEATURE:        
			{            
				data_stage = USB_EP0_NODATA;               
				if ((pDevReq->wValue & 0xff)  == 0x4)            
				{                
				    Printf("delay\r\n");                
					USB_hal_delay_ms(100);            
				}            
				break;        
			}
            case MUSB_REQ_CLEAR_FEATURE :
            {
                data_stage = USB_EP0_NODATA;
				if (pDevReq->bRequest == MUSB_REQ_CLEAR_FEATURE){                
					Printf("delay\r\n");                
					USB_hal_delay_ms(100);            
				}
                break;
            }
            default :
            {
               break;
            }
        }
        usb_config_ep0_req(assignedAddress, data_stage, 0, pDevReq);
        usb_start_ep0_transfer();
        while(!usb_check_ep0_complete() && !bError);
        if(bError){
            psOutput->eType = DVT_OUTPUT_RESULT;
            psOutput->eResult = DVT_RESULT_FAIL;
            return DVT_FAIL;
        }
        Printf("DEV RESERT step(%d) done: %s\r\n", nEnumStep, hubdevenumeration_step[nEnumStep].pDesciptor);
        nEnumStep++;
    }
    Printf("Hub Device Reset done\r\n");
    USB_EP0_REQ *req;
	for(i=1; i<=devNum; i++){
		MGC_QMU_Write8(MGC_O_HDRC_TXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_RXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_TXPORTADDR(0), i);        
		MGC_QMU_Write8(MGC_O_HDRC_RXPORTADDR(0), i);
		MGC_QMU_Write8(0x1a, 0x40);
		
        Printf("set feature reset port [%d]\r\n", i);

		devReq.bmRequestType = MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER;        
		devReq.bRequest = MUSB_REQ_SET_FEATURE;        
		devReq.wValue = 0x4;        
		devReq.wIndex = i;        
		devReq.wLength = 0;

        usb_config_ep0_req(assignedAddress, USB_EP0_NODATA, 0, &devReq);
    
    	usb_start_ep0_transfer();                
    	while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}
		
	    devReq.bmRequestType = MUSB_DIR_IN|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER;        
		devReq.bRequest = MUSB_REQ_GET_STATUS;        
		devReq.wValue = 0;        
		devReq.wIndex = i;        
		devReq.wLength = 0x4;
    	Printf("get status port [%d]\r\n", i);        
    
    	usb_config_ep0_req(assignedAddress, USB_EP0_DATAIN, 0, &devReq);
    
    	usb_start_ep0_transfer();                
    	while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}
    	
    	req = usb_get_ep0_req();
    	if (req->rxBuf[0] & 0x3){            
    	    Printf("port [%d]: ", i);            
    		switch ((req->rxBuf[1] & 0x6) >> 1)            
    		{                
    		    case 0x0:                    
    				Printf("full speed\r\n");                    
    				MGC_QMU_Write8(0x1a, 0x80);                    //fs                    
    				break;                
    			case 0x1:                    
    				Printf("low speed\r\n");                                        
    				MGC_QMU_Write8(0x1a, 0xc0);                                        //ls                    
    				break;                
    			case 0x2:                    
    				Printf("high speed\r\n");                                                            
    				MGC_QMU_Write8(0x1a, 0x40);                                        //hs                    
    				break;            
    		}        
    	}        
    	else{
    		Printf("No device, next!\r\n");
    	    continue;        
    	}

        Printf(" clear feature reset port [%d]\r\n", i);
        
        devReq.bmRequestType = MUSB_DIR_OUT|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER;
		devReq.bRequest = MUSB_REQ_CLEAR_FEATURE;
		devReq.wValue = 20;
		devReq.wIndex = i;
		devReq.wLength = 0;

        usb_config_ep0_req(assignedAddress, USB_EP0_NODATA, 0, &devReq);        
    	usb_start_ep0_transfer();                
    	while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}

		devReq.bmRequestType = MUSB_DIR_IN|MUSB_TYPE_CLASS|MUSB_RECIP_OTHER;        
		devReq.bRequest = MUSB_REQ_GET_STATUS;        
		devReq.wValue = 0;        
		devReq.wIndex = i;        
		devReq.wLength = 0x4;
		MGC_QMU_Write8(MGC_O_HDRC_TXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_RXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_TXPORTADDR(0), i);        
		MGC_QMU_Write8(MGC_O_HDRC_RXPORTADDR(0), i);

    	Printf("get status port [%d]\r\n", i);        
    
    	usb_config_ep0_req(assignedAddress, USB_EP0_DATAIN, 0, &devReq);
    	usb_start_ep0_transfer();                
    	while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}
		req = usb_get_ep0_req();
		Printf("0x%x 0x%x 0x%x 0x%x \r\n"
			, req->rxBuf[0], req->rxBuf[1], req->rxBuf[2], req->rxBuf[3]);
			
        Printf("set address port[%d]\r\n", i);
    	devReq.bmRequestType = MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE;        
    	devReq.bRequest = MUSB_REQ_SET_ADDRESS;        
    	devReq.wValue = i;        
    	devReq.wIndex = 0;        
		devReq.wLength = 0;
    
    	usb_config_ep0_req(0, USB_EP0_NODATA, 0, &devReq);        
    	usb_start_ep0_transfer();                
    	while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}
		Printf("set configure 1 port [%d]\r\n", i);
		devReq.bmRequestType = MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE;        
    	devReq.bRequest = MUSB_REQ_SET_CONFIGURATION;        
    	devReq.wValue = 0x01;        
    	devReq.wIndex = 0;        
		devReq.wLength = 0;
    
    	usb_config_ep0_req(i, USB_EP0_NODATA, 0, &devReq);        
    	usb_start_ep0_transfer();                
    	while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}

	}

    for(i=1; i<=devNum; i++){
		Printf("config port[%d] EP config\r\n", i);
	    devReq.bmRequestType = MUSB_DIR_IN|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE;        
    	devReq.bRequest = MUSB_REQ_GET_DESCRIPTOR;        
    	devReq.wValue = MUSB_DT_CONFIG << 8;        
    	devReq.wIndex = 0;        
		devReq.wLength = 0x39;

		MGC_QMU_Write8(MGC_O_HDRC_TXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_RXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_TXPORTADDR(0), i);        
		MGC_QMU_Write8(MGC_O_HDRC_RXPORTADDR(0), i);
         
        usb_config_ep0_req(i, USB_EP0_DATAIN, 0, &devReq);
		usb_start_ep0_transfer();
		while(!usb_check_ep0_complete() && !bError);
		if(bError){
		    return DVT_FAIL;
		}
		Printf("Get config done!!\r\n");
		SetMultiETHEPConfig(i);
    }

    for(i=1; i<=devNum; i++){
	    nEnumStep = 0;
		eth_enum_write_index = 0;
        while(ethenumeration_step[nEnumStep].pDesciptor != NULL){
            MUSB_DeviceRequest *pDevReq = &(ethenumeration_step[nEnumStep].sDevReq);
    		count = 0;
            switch(pDevReq->bRequest){
                case AX_CMD_SET_SW_MII:        //0X06
                {
                    data_stage = USB_EP0_NODATA;
                    break; 
                }
                case AX_CMD_READ_MII_REG:        //0x07
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }
                case AX_CMD_WRITE_MII_REG:        //0x08
                {
                    data_stage = USB_EP0_DATAOUT;
                    count = 2;
                    USB_EP0_REQ *req = usb_get_ep0_req();
                    req->txBuf[0] = eth_enum_write_value[eth_enum_write_index][0];
                    req->txBuf[1] = eth_enum_write_value[eth_enum_write_index++][1];
                    break;
                }
                case AX_CMD_READ_MII_OPERATION_MODE:        //0x09
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }        
                case AX_CMD_SET_HW_MII:        //0x0a
                {  
                    data_stage = USB_EP0_NODATA;
                    break;
                }
                case AX_CMD_READ_RX_CONTROL_REG:        //0x0f
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }        
                case AX_CMD_WRITE_RX_CTL:        //0x10
                {  
                    data_stage = USB_EP0_NODATA;
                    break;
                }
                case AX_CMD_READ_IPG012:        //0x11
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }        
                case AX_CMD_WRITE_IPG0:        //0x12    - FullSpeed Used Only
                {  
                    data_stage = USB_EP0_NODATA;
                    break;
                }
                case AX_CMD_WRITE_IPG1:        //0x13    - FullSpeed Used Only
                {  
                    data_stage = USB_EP0_NODATA;
                    break;
                }
                case AX_CMD_WRITE_IPG2:        //0x14    - FullSpeed Used Only
                {  
                    data_stage = USB_EP0_NODATA;
                    break;
                }
                case AX_CMD_READ_MULTIFILTER_ARRAY:        //0x15
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }        
                case AX_CMD_WRITE_MULTI_FILTER: //0x16
                {
                    data_stage = USB_EP0_DATAOUT;
                    count = 8;
                    USB_EP0_REQ *req = usb_get_ep0_req();
                    req->txBuf[0] = eth_enum_write_value[eth_enum_write_index][0];
                    req->txBuf[1] = eth_enum_write_value[eth_enum_write_index][1];
                    req->txBuf[2] = eth_enum_write_value[eth_enum_write_index][2];                
                    req->txBuf[3] = eth_enum_write_value[eth_enum_write_index][3];
                    req->txBuf[4] = eth_enum_write_value[eth_enum_write_index][4];                
                    req->txBuf[5] = eth_enum_write_value[eth_enum_write_index][5];
                    req->txBuf[6] = eth_enum_write_value[eth_enum_write_index][6];
                    req->txBuf[7] = eth_enum_write_value[eth_enum_write_index++][7];                
                    break;
                }
                case AX_CMD_READ_NODE_ID:        //0x17
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }
                case AX_CMD_READ_PHY_ID:        //0x19
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }
                case AX_CMD_READ_MEDIUM_MODE:         // 0x1a
                {
                    data_stage = USB_EP0_DATAIN;
                    break;    
                }
                case AX_CMD_WRITE_MEDIUM_MODE:        //0x1b    - FullSpeed Used Only
                {  
                    data_stage = USB_EP0_NODATA;
                    break;
                }
                case AX_CMD_READ_MONITOR_MODE:        //0x1c
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }    
                case AX_CMD_READ_GPIOS:        //0x1e
                {  
                    data_stage = USB_EP0_DATAIN;
                    break;
                }        
                case AX_CMD_WRITE_GPIOS:        //0X1f
                {
                    data_stage = USB_EP0_NODATA;
                    break;
                } 
                default :
               {    
                    Printf (3, "case %d is not support.........\r\n\r\n\r\n", pDevReq->bRequest);
                    break;
               }
            }
    		
            usb_config_ep0_req(i, data_stage, count, pDevReq);
            usb_start_ep0_transfer();
            while(!usb_check_ep0_complete() && !bError);
            if(bError){
                psOutput->eType = DVT_OUTPUT_RESULT;
                psOutput->eResult = DVT_RESULT_FAIL;
                return DVT_FAIL;
            }
            Printf("ETHENUM port (%d) step(%d) done: %s\r\n", i, nEnumStep, ethenumeration_step[nEnumStep].pDesciptor);
            nEnumStep++;
        }
    }

	Printf("ETH enumeration done\r\n");
	endTestcase();
    //Init CISR
    CISR_Init(hdrc_cisr, HdrcIsrMultiIxia, 1, &hdrc);
    //Attach USB CISR
    CISR_Attach(hdrc_cisr, USB_HW_IRQ_NO);   
    MGC_QMU_Write8(MGC_O_HDRC_INTRUSBE, MGC_M_INTR_CONNECT | MGC_M_INTR_BABBLE | MGC_M_INTR_VBUSERROR);
	
	for(i=1; i<=devNum; i++){
		usb_config_req(i, USB_RX, 0, 1, i, 3, 1, 1);
		usb_config_req(i, USB_TX, 0, 1, i, 3, 1, 1);
	}
	nCounter = 0;
#if 1
	ahbmon_init();	    
	// For Traffic Monitor	  
	// Specify monitored master and monitored transaction type	  
	mon_attr_t attr04 = {			 
		.masterID  = MASTER_APS_USB20_ID,			 
		.transType	= TRANS_ALL_TYPE,			 
		.monAddr   = 0x00000000,//0x6EDFB, //SetupTxDataBuffer, 		  
		.monAddrMask	= 0xFFFFFFFF // bits set to '1' to indicate DON'T COMPARE	 
	};	  
	void *mon_mdmcu_param = 0;	  
	/** Step 1. Register MDMCU ISR */	 
	//ahbmon_register_intr(MON_APMCU_ID, mon_mdmcu_isr, mon_mdmcu_param);	 
	/** Step 2: Specify Which Monitor System (MD, AP or WXP) and Its Working Mode */	
	ahbmon_set_sys(MON_APMCU_ID, MON_TRAFFIC_MODE);    
	/** Step 3: Specify Target Bus of the Monitor System */    
	ahbmon_set_bus(BUS_APS_ID);    
	/** Step 4: Set Related Attributes */	 
	ahbmon_set_attributes(&attr04);    
//	Printf("press anykey\r\n");
	int userCmd;
//	DVT_GetUserCommand(&userCmd);
#endif
	for(i=1; i<=devNum; i++){
		usb_start_transfer(i, USB_RX);
	}
#if 0
	USB_hal_delay_ms(2000);
    /** Step 5: Kick Off Monitor 1st Time */
    ahbmon_set_op(MON_START_OP);

	USB_hal_delay_ms(5000);

	/** Step 6: Pause Monitor  */
	ahbmon_set_op(MON_PAUSE_OP);
	/** Step 7: Collect Monitor Data */
    ahbmon_get_statistics(&stat04);
    /** Step 8: Generate/Output Statistics Report  */
    ahbmon_dump_statistics("Example 04 Statistics", &stat04);
	/** Step 12: Convert Statistics to Analysis*/
    ahbmon_get_analysis(&stat04, &analy04);    
    /** Step 13: Generate/Output Analysis Report*/    
    ahbmon_dump_analysis("Example 04 Analysis", &analy04);
#endif
	while(1);
	
}


DVT_STATUS_T TestIxiaLoopbackSpeedy(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput){
	bError = false;
    nEnumStep = 0;
    dwEP0MaxP = 8;
    int isPass = 0;
    int usecs = 30;
    int i = 0;
    int wAddress = 2;
    int assignedAddress = 0;
    int count = 0;
    USB_EP0_DATASTAGE data_stage;
    resetUSB();
    unsigned char state;
    bError = FALSE;
    isReset = 0;
    isPass = 0;
    resetUSB();
    
	initialTestcase(true);
	if(isPass == -1){
		endTestcase();
		return DVT_FAIL;
	}
	resetFIFO();
    

    MUSB_DeviceRequest devReq;
    devReq.bmRequestType = MUSB_DIR_IN | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE;
    devReq.bRequest = MUSB_REQ_GET_DESCRIPTOR;
    devReq.wValue = MUSB_DT_DEVICE << 8;
    devReq.wIndex = 0;
    devReq.wLength = 0x40;
    usb_config_ep0_req(0, USB_EP0_DATAIN, 0, &devReq);
    usb_start_ep0_transfer();
    while(!usb_check_ep0_complete() && !bError);
    if(bError){
        psOutput->eType = DVT_OUTPUT_RESULT;
        psOutput->eResult = DVT_RESULT_FAIL;
        return DVT_FAIL;
    }
    Printf("GetDescriptor done\r\n");
    while(enumeration_step[nEnumStep].pDesciptor != NULL){
        MUSB_DeviceRequest *pDevReq = &(enumeration_step[nEnumStep].sDevReq);
        switch (pDevReq->bRequest) {
            case MUSB_REQ_GET_STATUS :
            {
                data_stage = USB_EP0_DATAIN;
                break;
            }
         
            case MUSB_REQ_CLEAR_FEATURE :
            
            {
                data_stage = USB_EP0_DATAOUT;
         
                break;
            }
         
            case MUSB_REQ_GET_DESCRIPTOR :
            
            {

                data_stage = USB_EP0_DATAIN;
                break;
            }          
            case MUSB_REQ_SET_ADDRESS :
            
            {
                 //assignedAddress = 2;
                 
                 pDevReq->bmRequestType = MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE;
                 pDevReq->bRequest = MUSB_REQ_SET_ADDRESS;
                 pDevReq->wValue = wAddress;
                 pDevReq->wIndex = 0x0;
                 pDevReq->wLength = 0x0;
                 data_stage = USB_EP0_NODATA;
                 break;
            }
           
            case MUSB_REQ_GET_CONFIGURATION :
            
           {
               data_stage = USB_EP0_DATAIN;
               break;
           }
 
           case MUSB_REQ_SET_CONFIGURATION :
            
           {
         
                data_stage = USB_EP0_NODATA;
               break;
           }
         
           case 0xFE :

         {

               data_stage = USB_EP0_NODATA;
               break;
          }
         
          default :
 
          {
         
               break;
           }
        
        }

        usb_config_ep0_req(assignedAddress, data_stage, 0, pDevReq);
        usb_start_ep0_transfer();
        while(!usb_check_ep0_complete() && !bError);
        if(bError){
            psOutput->eType = DVT_OUTPUT_RESULT;
            psOutput->eResult = DVT_RESULT_FAIL;
            return DVT_FAIL;
        }
        Printf("ENUM step(%d) done: %s\r\n", nEnumStep, enumeration_step[nEnumStep].pDesciptor);
        if(pDevReq->bRequest == MUSB_REQ_SET_ADDRESS){
            assignedAddress = wAddress;
        }
        if(pDevReq->bRequest==MUSB_REQ_GET_DESCRIPTOR && pDevReq->wValue==(MUSB_DT_CONFIG << 8)){
            SetETHEPConfig();
        }
        nEnumStep++;
    }
    Printf("Enumeration done\r\n");
	#if 1
    nEnumStep = 0;
    while(ethenumeration_step[nEnumStep].pDesciptor != NULL){
        MUSB_DeviceRequest *pDevReq = &(ethenumeration_step[nEnumStep].sDevReq);
		count = 0;
        switch(pDevReq->bRequest){
            case AX_CMD_SET_SW_MII:        //0X06
            {
                data_stage = USB_EP0_NODATA;
                break; 
            }
            case AX_CMD_READ_MII_REG:        //0x07
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }
            case AX_CMD_WRITE_MII_REG:        //0x08
            {
                data_stage = USB_EP0_DATAOUT;
                count = 2;
                USB_EP0_REQ *req = usb_get_ep0_req();
                req->txBuf[0] = eth_enum_write_value[eth_enum_write_index][0];
                req->txBuf[1] = eth_enum_write_value[eth_enum_write_index++][1];
                break;
            }
            case AX_CMD_READ_MII_OPERATION_MODE:        //0x09
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }        
            case AX_CMD_SET_HW_MII:        //0x0a
            {  
                data_stage = USB_EP0_NODATA;
                break;
            }
            case AX_CMD_READ_RX_CONTROL_REG:        //0x0f
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }        
            case AX_CMD_WRITE_RX_CTL:        //0x10
            {  
                data_stage = USB_EP0_NODATA;
                break;
            }
            case AX_CMD_READ_IPG012:        //0x11
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }        
            case AX_CMD_WRITE_IPG0:        //0x12    - FullSpeed Used Only
            {  
                data_stage = USB_EP0_NODATA;
                break;
            }
            case AX_CMD_WRITE_IPG1:        //0x13    - FullSpeed Used Only
            {  
                data_stage = USB_EP0_NODATA;
                break;
            }
            case AX_CMD_WRITE_IPG2:        //0x14    - FullSpeed Used Only
            {  
                data_stage = USB_EP0_NODATA;
                break;
            }
            case AX_CMD_READ_MULTIFILTER_ARRAY:        //0x15
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }        
            case AX_CMD_WRITE_MULTI_FILTER: //0x16
            {
                data_stage = USB_EP0_DATAOUT;
                count = 8;
                USB_EP0_REQ *req = usb_get_ep0_req();
                req->txBuf[0] = eth_enum_write_value[eth_enum_write_index][0];
                req->txBuf[1] = eth_enum_write_value[eth_enum_write_index][1];
                req->txBuf[2] = eth_enum_write_value[eth_enum_write_index][2];                
                req->txBuf[3] = eth_enum_write_value[eth_enum_write_index][3];
                req->txBuf[4] = eth_enum_write_value[eth_enum_write_index][4];                
                req->txBuf[5] = eth_enum_write_value[eth_enum_write_index][5];
                req->txBuf[6] = eth_enum_write_value[eth_enum_write_index][6];
                req->txBuf[7] = eth_enum_write_value[eth_enum_write_index++][7];                
                break;
            }
            case AX_CMD_READ_NODE_ID:        //0x17
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }
            case AX_CMD_READ_PHY_ID:        //0x19
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }
            case AX_CMD_READ_MEDIUM_MODE:         // 0x1a
            {
                data_stage = USB_EP0_DATAIN;
                break;    
            }
            case AX_CMD_WRITE_MEDIUM_MODE:        //0x1b    - FullSpeed Used Only
            {  
                data_stage = USB_EP0_NODATA;
                break;
            }
            case AX_CMD_READ_MONITOR_MODE:        //0x1c
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }    
            case AX_CMD_READ_GPIOS:        //0x1e
            {  
                data_stage = USB_EP0_DATAIN;
                break;
            }        
            case AX_CMD_WRITE_GPIOS:        //0X1f
            {
                data_stage = USB_EP0_NODATA;
                break;
            } 
            default :
           {    
                Printf (3, "case %d is not support.........\r\n\r\n\r\n", pDevReq->bRequest);
                break;
           }
        }
		
        usb_config_ep0_req(assignedAddress, data_stage, count, pDevReq);
        usb_start_ep0_transfer();
        while(!usb_check_ep0_complete() && !bError);
        if(bError){
            psOutput->eType = DVT_OUTPUT_RESULT;
            psOutput->eResult = DVT_RESULT_FAIL;
            return DVT_FAIL;
        }
        Printf("ETHENUM step(%d) done: %s\r\n", nEnumStep, ethenumeration_step[nEnumStep].pDesciptor);
        nEnumStep++;
    }
	#endif
    Printf("ETH enumeration done\r\n");
	endTestcase();
    //Init CISR
    CISR_Init(hdrc_cisr, HdrcIsrIxia, 1, &hdrc);
    //disable and flush interrupt
//    disableIntr();
    //Attach USB CISR
    CISR_Attach(hdrc_cisr, USB_HW_IRQ_NO);   
	state = MGC_M_INTR_CONNECT | MGC_M_INTR_BABBLE | MGC_M_INTR_VBUSERROR;
    MGC_QMU_Write8(MGC_O_HDRC_INTRUSBE, state);
    USB_REQ *reqRx = usb_get_req(nRxEP, USB_RX);
    USB_REQ *reqTx = usb_get_req(nTxEP, USB_TX);
    *reqTx->buf = *reqRx->buf;
	SetAddress(assignedAddress);
	usb_config_req(nRxEP, USB_RX, 0, true, 1, 3, 1, 1);
	usb_config_req(nTxEP, USB_TX, 0, true, 2, 3, 1, 0);
	usb_start_transfer(nRxEP, USB_RX);
	while(1);
}
#endif

extern void minusGlobalFIFOadd(unsigned int num);
extern void addGlobalFIFOadd(unsigned int num);


void SetMultiETHEPConfig(unsigned int port){
#if 0	
    unsigned int ep_num;
    USB_DIR dir;
    USB_TRANSFER_TYPE type;
    USB_EP0_REQ *req = usb_get_ep0_req();
    MUSB_ConfigurationDescriptor *pConfDes;
    MUSB_InterfaceDescriptor *pInterDes;
    MUSB_EndpointDescriptor *pEndDes;
    //MUSB_DeviceDescriptor *pDevDes;
    //LocalEnd *pEnd;
    void *pBuf;
    int i, j;
    pBuf = pConfDes = (MUSB_ConfigurationDescriptor *) &(req->rxBuf);
    pBuf += sizeof (MUSB_ConfigurationDescriptor);
	for (i=0; i<pConfDes->bNumInterfaces; i++) {
        pInterDes = (MUSB_InterfaceDescriptor *) pBuf;
        pBuf += sizeof (MUSB_InterfaceDescriptor);
        bEndCount = pInterDes->bNumEndpoints;
        for (j=0; j<pInterDes->bNumEndpoints; j++) {
            pEndDes = (MUSB_EndpointDescriptor *) pBuf;
            ep_num = pEndDes->bEndpointAddress & 0xf;
            dir = (MUSB_DIR_OUT == (pEndDes->bEndpointAddress & 0x80)) ? USB_TX : USB_RX;
            type = pEndDes->bmAttributes & 0x3;
			if(type == USB_BULK){
                unsigned int dwMaxP = pEndDes->wMaxPacketSize;
                usb_ep_enable(port, dir, type, dwMaxP, 1, ep_num);
				minusGlobalFIFOadd(dwMaxP);
                Printf("config endpoint: port[%d] ep[%d], dir[%d], type[%d], maxp[%d] \r\n", port, ep_num, dir, type, dwMaxP);
			}
			pBuf += sizeof (MUSB_EndpointDescriptor);
        }
    }
	addGlobalFIFOadd(512);
	MGC_QMU_Write8(MGC_O_HDRC_TXHUBADDR(port), 5);        
	MGC_QMU_Write8(MGC_O_HDRC_RXHUBADDR(port), 5);        
	MGC_QMU_Write8(MGC_O_HDRC_TXPORTADDR(port), port);        
	MGC_QMU_Write8(MGC_O_HDRC_RXPORTADDR(port), port);
	SetHubAddress(port, port, USB_TX);
	SetHubAddress(port, port, USB_RX);
#else
	unsigned int ep_num;
	unsigned int dwMaxP;
	USB_DIR dir;
	USB_TRANSFER_TYPE type;

	dir = USB_RX;
	type = USB_BULK;
	dwMaxP = 512;
	ep_num = 3;
	usb_ep_enable(port, dir, type, dwMaxP, 1, ep_num);
	//ben 20100513
	//minusGlobalFIFOadd(dwMaxP);
	Printf("config endpoint: port[%d] ep[%d], dir[%d], type[%d], maxp[%d] \r\n", port, ep_num, dir, type, dwMaxP);

	dir = USB_TX;
	type = USB_BULK;
	dwMaxP = 512;
	ep_num = 2;
	usb_ep_enable(port, dir, type, dwMaxP, 0, ep_num);
	//ben 20100513
	//minusGlobalFIFOadd(dwMaxP);
	Printf("config endpoint: port[%d] ep[%d], dir[%d], type[%d], maxp[%d] \r\n", port, ep_num, dir, type, dwMaxP);	

	MGC_QMU_Write8(MGC_O_HDRC_TXHUBADDR(port), 5);        
	MGC_QMU_Write8(MGC_O_HDRC_RXHUBADDR(port), 5);        
	MGC_QMU_Write8(MGC_O_HDRC_TXPORTADDR(port), port);        
	MGC_QMU_Write8(MGC_O_HDRC_RXPORTADDR(port), port);
	SetHubAddress(port, port, USB_TX);
	SetHubAddress(port, port, USB_RX);	
#endif
	
}

#if 0 //!PORTING
void SetETHEPConfig(){
    unsigned int ep_num;	
    USB_DIR dir;
    USB_TRANSFER_TYPE type;
    unsigned int dwMaxP;
    USB_EP0_REQ *req = usb_get_ep0_req();
    MUSB_ConfigurationDescriptor *pConfDes;
    MUSB_InterfaceDescriptor *pInterDes;
    MUSB_EndpointDescriptor *pEndDes;
    MUSB_DeviceDescriptor *pDevDes;
//    LocalEnd *pEnd;
//    void *pBuf;
    int i, j;
    pBuf = pConfDes = (MUSB_ConfigurationDescriptor *) &(req->rxBuf);
    pBuf += sizeof (MUSB_ConfigurationDescriptor);
    for (i=0; i<pConfDes->bNumInterfaces; i++) {
        pInterDes = (MUSB_InterfaceDescriptor *) pBuf;
        pBuf += sizeof (MUSB_InterfaceDescriptor);
        bEndCount = pInterDes->bNumEndpoints;
        for (j=0; j<pInterDes->bNumEndpoints; j++) {
            pEndDes = (MUSB_EndpointDescriptor *) pBuf;
            ep_num = pEndDes->bEndpointAddress & 0xf;
            dir = (MUSB_DIR_OUT == (pEndDes->bEndpointAddress & 0x80)) ? USB_TX : USB_RX;
            if(dir == USB_TX){
                nTxEP = ep_num;
            }
            else if(dir == USB_RX){
                nRxEP = ep_num;
            }
            type = pEndDes->bmAttributes & 0x3;
            dwMaxP = pEndDes->wMaxPacketSize;
            usb_ep_enable(ep_num, dir, type, dwMaxP, 1, ep_num);
            Printf("config endpoint: ep[%d], dir[%d], type[%d], maxp[%d] \r\n", ep_num, dir, type, dwMaxP);
            pBuf += sizeof (MUSB_EndpointDescriptor);
        }
    }
    
}
#endif

#if 0 //!PORTING
cisr_status_t HdrcIsrMultiIxia(unsigned int irq_no, void* pParam){
//	Printf("hdrcisrmultiixia\r\n");
    hdrc_t* pCtrl = (hdrc_t*)pParam;
    unsigned char bIntrUsbValue;
    unsigned int dwDmaIntrValue;
    unsigned short wIntrTxValue;
    unsigned short wIntrRxValue;
	unsigned int ep_num, channel;
    CIRQ_Mask(irq_no);
    bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
    dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
    wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
    MGC_QMU_Write8(MGC_O_HDRC_INTRUSB, (bIntrUsbValue));
    MGC_QREG_Write32(MGC_O_HSDMA_INTR, (dwDmaIntrValue));
    MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (wIntrTxValue));
    MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (wIntrRxValue));
//	Printf("intrusb[0x%x], intrdma[0x%x], intrtx[0x%x], intrrx[0x%x]\r\n", bIntrUsbValue, dwDmaIntrValue, wIntrTxValue, wIntrRxValue);
    if(bIntrUsbValue & MGC_M_INTR_CONNECT){
        MGC_ResetPort();
        return;
    }
	if(dwDmaIntrValue){
        for(channel = 0; channel < DMA_CHANNEL_NUM; channel++){
            if(dwDmaIntrValue & (1<<channel)){
                multi_ixia_dma_handler(channel+1);
            }
        }
    }
	if(wIntrTxValue){
        for(ep_num = 1; ep_num <= TX_FIFO_NUM; ep_num++){
            if(wIntrTxValue & (1<<ep_num)){
                multi_ixia_tx_handler(ep_num);
            }
        }
    }
    if(wIntrRxValue){
        for(ep_num = 1; ep_num <= RX_FIFO_NUM; ep_num++){
            if(wIntrRxValue & (1<<ep_num)){
                multi_ixia_rx_handler(ep_num);
            }
        }
    }
    CIRQ_Unmask(irq_no);
    return CISR_DONE;
}
#endif

#if 0 //!PORTING
cisr_status_t HdrcIsrIxia(unsigned int irq_no, void* pParam){
    hdrc_t* pCtrl = (hdrc_t*)pParam;
    unsigned char bIntrUsbValue;
    unsigned int dwDmaIntrValue;
    unsigned short wIntrTxValue;
    unsigned short wIntrRxValue;
	unsigned int ep_num, channel;
    CIRQ_Mask(irq_no);
    bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
    dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
    wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
//	Printf("dmaintr[0x%x], txintr[0x%x], rxintr[0x%x]\r\n", dwDmaIntrValue, wIntrTxValue, wIntrRxValue);
    MGC_QMU_Write8(MGC_O_HDRC_INTRUSB, (bIntrUsbValue));
    MGC_QREG_Write32(MGC_O_HSDMA_INTR, (dwDmaIntrValue));
    MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (wIntrTxValue));
    MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (wIntrRxValue));
//	Printf("dmaintr[0x%x], txintr[0x%x], rxintr[0x%x]\r\n", MGC_QREG_Read32(MGC_O_HSDMA_INTR), MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE), MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE));
    if(bIntrUsbValue & MGC_M_INTR_CONNECT){
        MGC_ResetPort();
        return;
    }
	if(dwDmaIntrValue){
        for(channel = 0; channel < DMA_CHANNEL_NUM; channel++){
            if(dwDmaIntrValue & (1<<channel)){
                ixia_dma_handler(channel+1);
            }
        }
    }
	if(wIntrTxValue){
        for(ep_num = 1; ep_num <= TX_FIFO_NUM; ep_num++){
            if(wIntrTxValue & (1<<ep_num)){
                ixia_tx_handler(ep_num);
            }
        }
    }
    if(wIntrRxValue){
        for(ep_num = 1; ep_num <= RX_FIFO_NUM; ep_num++){
            if(wIntrRxValue & (1<<ep_num)){
                ixia_rx_handler(ep_num);
            }
        }
    }
    CIRQ_Unmask(irq_no);
    return CISR_DONE;
}
#endif

#if 0 //!PORTING
void multi_ixia_dma_handler(int channel){
	USB_DMA_REQ *dma_req = usb_get_dma_req(channel);
    int tmp, dwDmaRealCnt;
    unsigned short wDmaCtrl;
	wDmaCtrl = MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(channel));
#if 0
	if(nCounter == 0){
		ahbmon_set_op(MON_START_OP);
	}
	nCounter++;
	if(nCounter >= 10000){
		/** Step 6: Pause Monitor  */
		ahbmon_set_op(MON_PAUSE_OP);
		/** Step 7: Collect Monitor Data */
	    ahbmon_get_statistics(&stat04);
	    /** Step 8: Generate/Output Statistics Report  */
	    ahbmon_dump_statistics("Example 04 Statistics", &stat04);
		/** Step 12: Convert Statistics to Analysis*/
	    ahbmon_get_analysis(&stat04, &analy04);    
	    /** Step 13: Generate/Output Analysis Report*/    
	    ahbmon_dump_analysis("Example 04 Analysis", &analy04);
		while(1);
	}
#endif
	if(wDmaCtrl & 1<<MGC_S_HSDMA_TRANSMIT){	//Tx
//		Printf("dma hand tx channel[%d], ep[%d]\r\n", channel, dma_req->ep_num);
		//clear DMA enab            
        MGC_WriteCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_ReadCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num) & (~MGC_M_TXCSR_DMAENAB));
        //clear DMA ReqMode
        MGC_WriteCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_ReadCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num) & (~MGC_M_TXCSR_DMAMODE));
        MGC_WriteCsr16 (MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_M_TXCSR_TXPKTRDY);
	}
	else{	//Rx
//		Printf("dma hand rx channel[%d], ep[%d]\r\n", channel, dma_req->ep_num);
		USB_REQ *reqRx = usb_get_req(dma_req->ep_num, USB_RX);
		dwDmaRealCnt = MGC_QREG_Read32(MGC_O_HSDMA_REALCNT(channel));
		reqRx->actual = reqRx->actual + dwDmaRealCnt;
		MGC_WriteCsr16 (MGC_O_HDRC_RXCSR, dma_req->ep_num, 0);
		//start next Tx transfer
		MGC_WriteCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE | MGC_M_TXCSR_AUTOSET);
	    //Set DMA addr, count
	    MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(channel), reqRx->buf);
	    MGC_QREG_Write32(MGC_O_HSDMA_COUNT(channel), reqRx->actual);
		MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(channel), 0);
	    //Set DMA CTRL regs: Direction=1, DMA Mode=1, Interrupt Enable=1, Burst Mode
	    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(channel), (1<<MGC_S_HSDMA_TRANSMIT)
	        | (1<<MGC_S_HSDMA_MODE1) | (1<<MGC_S_HSDMA_IRQENABLE) | (3<<MGC_S_HSDMA_BURSTMODE)
	        | (dma_req->ep_num<<MGC_S_HSDMA_ENDPOINT));
	    //Enable DMA
	    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(channel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(channel)) | (1<<MGC_S_HSDMA_ENABLE));
	}
}
#endif

#if 0 //!PORTING
void multi_ixia_tx_handler(int ep_num){
//	Printf("tx hand ep[%d]\r\n", ep_num);
	USB_REQ *reqRx = usb_get_req(ep_num, USB_RX);
	USB_REQ *reqTx = usb_get_req(ep_num, USB_TX);
    unsigned int dataCount;
//    int tmp;
	//start next Rx transfer
	reqRx->actual = 0;
	//turn on rx interrupt enable
    MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, MGC_QREG_Read16 (MGC_O_HDRC_INTRRXE) | (1 << ep_num));
    //set RxCSR: AutoClear=1, DMAReqEnab=1, DMAReqMode=1, AutoReq=1
    MGC_WriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_M_RXCSR_AUTOCLEAR
        | MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE | MGC_M_RXCSR_H_AUTOREQ);
    //Set DMA addr, count
    MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(reqRx->dmaChannel), reqRx->buf);
    MGC_QREG_Write32(MGC_O_HSDMA_COUNT(reqRx->dmaChannel), USB_BUF_SIZE);
    //set DMA Ctrl: Direction=0, Interrupt Enable=1, DMA Mode=1, Burst Mode
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel), 0);
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel), (1<<MGC_S_HSDMA_IRQENABLE)
        & (~(1<<MGC_S_HSDMA_TRANSMIT)) | (1<<MGC_S_HSDMA_MODE1) | (3<<MGC_S_HSDMA_BURSTMODE)
        | (ep_num<<MGC_S_HSDMA_ENDPOINT));
    //enable DMA
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel)) | (1<<MGC_S_HSDMA_ENABLE));
    //request packet
    MGC_WriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_ReadCsr16(MGC_O_HDRC_RXCSR, ep_num) | MGC_M_RXCSR_H_REQPKT);
}

void multi_ixia_rx_handler(int ep_num){
//	Printf("rx hand ep[%d]\r\n", ep_num);
	unsigned int dwDmaRealCnt;
	unsigned int dwCount;
    USB_REQ *reqRx = usb_get_req(ep_num, USB_RX);
	USB_REQ *reqTx = usb_get_req(ep_num, USB_TX);
	dwCount = MGC_ReadCsr16(MGC_O_HDRC_RXCOUNT, ep_num);
	dwDmaRealCnt = MGC_QREG_Read32(MGC_O_HSDMA_REALCNT(reqRx->dmaChannel));
    reqRx->actual = dwDmaRealCnt;
	MGC_WriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_M_RXCSR_RXPKTRDY);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, MGC_QREG_Read16 (MGC_O_HDRC_INTRRXE) & (~(1 << ep_num)));
	//set DMA Ctrl: Direction=0, Interrupt Enable=1, DMA Mode=0, Burst Mode
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel), 0);
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel)) | (1<<MGC_S_HSDMA_IRQENABLE)
        & (~(1<<MGC_S_HSDMA_TRANSMIT)) & (~(1<<MGC_S_HSDMA_MODE1 )) | (3<<MGC_S_HSDMA_BURSTMODE)
        | (ep_num<<MGC_S_HSDMA_ENDPOINT));
    //Set DMA addr, count
    MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(reqRx->dmaChannel), reqRx->buf + reqRx->actual);
    MGC_QREG_Write32(MGC_O_HSDMA_COUNT(reqRx->dmaChannel), dwCount);
    //enable DMA
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel)) | (1<<MGC_S_HSDMA_ENABLE));
}
#endif

#if 0 //!PORTING
void ixia_dma_handler(int channel){
	USB_DMA_REQ *dma_req = usb_get_dma_req(channel);
    USB_REQ *reqRx = usb_get_req(nRxEP, USB_RX);
	USB_REQ *reqTx = usb_get_req(nTxEP, USB_TX);
    unsigned char txType;
    int i;
    int tmp, dwDmaRealCnt;
	
	if(channel == reqRx->dmaChannel){
//		Printf("rx dma handler\r\n");
		
		dwDmaRealCnt = MGC_QREG_Read32(MGC_O_HSDMA_REALCNT(reqRx->dmaChannel));
		//dwDmaRealCnt = MGC_QREG_Read32(MGC_O_HSDMA_COUNT(reqRx->dmaChannel));
		reqRx->actual = reqRx->actual + dwDmaRealCnt;
//		Printf("dma handler, rx actual [%d]\r\n", reqRx->actual);
		MGC_WriteCsr16 (MGC_O_HDRC_RXCSR, nRxEP, 0);
		//start next Tx transfer
		MGC_WriteCsr16(MGC_O_HDRC_TXCSR, nTxEP, MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE | MGC_M_TXCSR_AUTOSET);
	    //Set DMA addr, count
	    MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(reqTx->dmaChannel), reqRx->buf);
	    MGC_QREG_Write32(MGC_O_HSDMA_COUNT(reqTx->dmaChannel), reqRx->actual);
		MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqTx->dmaChannel), 0);
	    //Set DMA CTRL regs: Direction=1, DMA Mode=1, Interrupt Enable=1, Burst Mode
	    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqTx->dmaChannel), (1<<MGC_S_HSDMA_TRANSMIT)
	        | (1<<MGC_S_HSDMA_MODE1) | (1<<MGC_S_HSDMA_IRQENABLE) | (3<<MGC_S_HSDMA_BURSTMODE)
	        | (nTxEP<<MGC_S_HSDMA_ENDPOINT));
	    //Enable DMA
	    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqTx->dmaChannel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(reqTx->dmaChannel)) | (1<<MGC_S_HSDMA_ENABLE));
	}
	else if(channel == reqTx->dmaChannel){
//		Printf("tx dma handler\r\n");
		//clear DMA enab            
        MGC_WriteCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_ReadCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num) & (~MGC_M_TXCSR_DMAENAB));
        //clear DMA ReqMode
        MGC_WriteCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_ReadCsr16(MGC_O_HDRC_TXCSR, dma_req->ep_num) & (~MGC_M_TXCSR_DMAMODE));
        MGC_WriteCsr16 (MGC_O_HDRC_TXCSR, dma_req->ep_num, MGC_M_TXCSR_TXPKTRDY);
	}
}
#endif

#if 0 //!PORTING
void ixia_tx_handler(int ep_num){
//	Printf("tx handler\r\n");
	USB_REQ *reqRx = usb_get_req(nRxEP, USB_RX);
	USB_REQ *reqTx = usb_get_req(nTxEP, USB_TX);
    unsigned int dataCount;
    int tmp;
	//start next Rx transfer
	reqRx->actual = 0;
	//turn on rx interrupt enable
    MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, MGC_QREG_Read16 (MGC_O_HDRC_INTRRXE) | (1 << nRxEP));
    //set RxCSR: AutoClear=1, DMAReqEnab=1, DMAReqMode=1, AutoReq=1
    MGC_WriteCsr16 (MGC_O_HDRC_RXCSR, nRxEP, MGC_M_RXCSR_AUTOCLEAR
        | MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE | MGC_M_RXCSR_H_AUTOREQ);
    //Set DMA addr, count
    MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(reqRx->dmaChannel), reqRx->buf);
    MGC_QREG_Write32(MGC_O_HSDMA_COUNT(reqRx->dmaChannel), USB_BUF_SIZE);
    //set DMA Ctrl: Direction=0, Interrupt Enable=1, DMA Mode=1, Burst Mode
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel), 0);
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel), (1<<MGC_S_HSDMA_IRQENABLE)
        & (~(1<<MGC_S_HSDMA_TRANSMIT)) | (1<<MGC_S_HSDMA_MODE1) | (3<<MGC_S_HSDMA_BURSTMODE)
        | (nRxEP<<MGC_S_HSDMA_ENDPOINT));
    //enable DMA
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel)) | (1<<MGC_S_HSDMA_ENABLE));
    //request packet
    MGC_WriteCsr16 (MGC_O_HDRC_RXCSR, nRxEP, MGC_ReadCsr16(MGC_O_HDRC_RXCSR, nRxEP) | MGC_M_RXCSR_H_REQPKT);
}
#endif

#if 0 //!PORTING
void ixia_rx_handler(int ep_num){
	
//	Printf("rx handler [%d]\r\n", ep_num);
	unsigned int dwDmaRealCnt;
	unsigned int dwCount;
    USB_REQ *reqRx = usb_get_req(nRxEP, USB_RX);
	USB_REQ *reqTx = usb_get_req(nTxEP, USB_TX);
	dwCount = MGC_ReadCsr16(MGC_O_HDRC_RXCOUNT, nRxEP);
//	Printf("dwcount: [%d]\r\n", dwCount);
	dwDmaRealCnt = MGC_QREG_Read32(MGC_O_HSDMA_REALCNT(reqRx->dmaChannel));
    reqRx->actual = dwDmaRealCnt;
//	Printf("rx handler, rx actual [%d]\r\n", reqRx->actual);
	MGC_WriteCsr16 (MGC_O_HDRC_RXCSR, nRxEP, 1);
	//Printf("RX hand dmaintr[0x%x], txintr[0x%x], rxintr[0x%x]\r\n", MGC_QREG_Read32(MGC_O_HSDMA_INTR), MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE), MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE));
    //turn off intrRx
    MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, MGC_QREG_Read16 (MGC_O_HDRC_INTRRXE) & (~(1 << nRxEP)));
	//set DMA Ctrl: Direction=0, Interrupt Enable=1, DMA Mode=0, Burst Mode
//    Printf("rxcsr: 0x%x\r\n", MGC_ReadCsr16(MGC_O_HDRC_RXCSR,nRxEP));
//	Printf("rxcount: %d\r\n", MGC_ReadCsr16(MGC_O_HDRC_RXCOUNT,nRxEP));
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel), 0);
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel)) | (1<<MGC_S_HSDMA_IRQENABLE)
        & (~(1<<MGC_S_HSDMA_TRANSMIT)) & (~(1<<MGC_S_HSDMA_MODE1 )) | (3<<MGC_S_HSDMA_BURSTMODE)
        | (nRxEP<<MGC_S_HSDMA_ENDPOINT));
    //Set DMA addr, count1
    MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(reqRx->dmaChannel), reqRx->buf + reqRx->actual);
    MGC_QREG_Write32(MGC_O_HSDMA_COUNT(reqRx->dmaChannel), dwCount);
    //enable DMA
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(reqRx->dmaChannel)) | (1<<MGC_S_HSDMA_ENABLE));
	#if 0
    QMU_HdrcUnloadFifo (ep_num, dwCount, reqRx->buf + reqRx->actual);
    MGC_WriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, 0);
    reqRx->actual = reqRx->actual + dwCount;
    
	//start next Tx transfer
	MGC_WriteCsr16(MGC_O_HDRC_TXCSR, nTxEP, MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE | MGC_M_TXCSR_AUTOSET);
    //Set DMA addr, count
    MGC_QREG_Write32(MGC_O_HSDMA_ADDRESS1(reqTx->dmaChannel), reqRx->buf);
    MGC_QREG_Write32(MGC_O_HSDMA_COUNT(reqTx->dmaChannel), reqRx->actual);
	MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqTx->dmaChannel), 0);
    //Set DMA CTRL regs: Direction=1, DMA Mode=1, Interrupt Enable=1, Burst Mode
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqTx->dmaChannel), (1<<MGC_S_HSDMA_TRANSMIT)
        | (1<<MGC_S_HSDMA_MODE1) | (1<<MGC_S_HSDMA_IRQENABLE) | (3<<MGC_S_HSDMA_BURSTMODE)
        | (nTxEP<<MGC_S_HSDMA_ENDPOINT));
    //Enable DMA
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(reqTx->dmaChannel), MGC_QREG_Read16(MGC_O_HSDMA_CONTROLLER(reqTx->dmaChannel)) | (1<<MGC_S_HSDMA_ENABLE));
	#endif
}
#endif

