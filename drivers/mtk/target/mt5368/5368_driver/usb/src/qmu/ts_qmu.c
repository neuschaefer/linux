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

//#include "x_os.h"
//#include "x_rtos.h"
#include "x_bim.h"
#include "plat_arc.h"
#include "mu_arch.h"

#include "dt_debug.h"
//#include "x_hal_1176.h"
//#include "x_drv_cli.h"
#include "utest_enumeration.h"
#include "mu_hdrdf.h"
#include "x_stl_lib.h"
//#include "brd_mem.h"
#include "mu_mem.h"


#define TX_Test
//#include <assert.h>

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
#include <utest-usb-qmu.h>

#include <Ddr_qmu.h>
#endif

//#include "x_rtos.h"
#include "dt_debug.h"
//#include "drv_qmu.c"

/*******************************************************************************


 * LOCAL CONST DEFINATION


******************************************************************************/
static int usecs;
static int i;
static int isPass;

PGPD Rx_gpd_end[15] = {0};
PGPD Tx_gpd_end[15] = {0};
PGPD Rx_gpd_last[15] = {0};
PGPD Tx_gpd_last[15] = {0};
#if 1 //ben
UINT8P ixia_buf[4][GPD_LIST_LEN]; 
#endif
//unsigned char *ixia_buf[4][5000]; 

unsigned char isFreeGPD = TRUE;

unsigned char is_bdp;
unsigned int gpd_num;
unsigned char isQMUmode;
unsigned char _fgTxErrChkSumTestCase = 0;
unsigned char _fgRxErrChkSumTestCase = 0;
/*******************************************************************************


 * LOCAL TYPE DEFINATION


******************************************************************************/
/*******************************************************************************


 * EXTERN DECLARATION


******************************************************************************/
static unsigned int g_FIFOadd = USB_FIFO_START_ADDRESS;
extern USB_TEST_SETTING g_usb_setting;
extern unsigned char isQMUmode;

#if 0 //!PORTING
extern cisr_status_t HdrcIsrQmu(unsigned int irq_no, void* pParam); 
extern void HdrcBsrQmu(unsigned int irq_no, unsigned int count, void* pParam);
extern void HdrcBsrDevQmu(unsigned int irq_no, unsigned int count, void* pParam);
#else
void HdrcBsrQmu(unsigned short wInterruptNumber);
void HdrcBsrDevQmu(unsigned short wInterruptNumber);
#endif
#if 0 //!PORTING
extern cisr_status_t HdrcIsrMultiIxiaQMU(unsigned int irq_no, void* pParam);
#else
extern void HdrcIsrMultiIxiaQMU(unsigned short wInterruptNumber);
#endif
#if 0 //!PORTING
extern cisr_status_t HdrcIsr(unsigned int irq_no, void* pParam);
#endif
#if 0 //!PORTING
extern void HdrcBsr(unsigned int irq_no, unsigned int count, void* pParam);
#else
void HdrcBsr(unsigned short wInterruptNumber);
#endif
extern void SetAddress(int addr);
extern void resetDevFIFO(void);
extern void usb_dev_ep_enable(int ep_num, USB_DIR dir, USB_TRANSFER_TYPE type, int maxp, int dpb);

#if 0 //!PORTING
DVT_STATUS_T TS_USB_QMU_ShowUsage(UINT32 u4TestCaseId);
DVT_STATUS_T TestQMUBulkLoopback(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
DVT_STATUS_T TestQMUBulkMultiGPD(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
DVT_STATUS_T TestQMUNGPD(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
DVT_STATUS_T TestMultiQueue(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
DVT_STATUS_T TestMultiEndpoint(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
DVT_STATUS_T TestQMURxStop(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
DVT_STATUS_T TestQMUTxStop(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
DVT_STATUS_T TestQMUIsoLoopback(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
DVT_STATUS_T TestQMUDevIsoLoopback(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
#endif
void initialQMUTestcase(unsigned char isHS);
void initialDevQMUTestcase(unsigned char isHS);
void initialQMU(unsigned char isHS);
#if 0 //!PORTING
#else
void endTestcase(void);
#endif
void endQMUTestcase(void);

#if 0 //!PORTING
DVT_STATUS_T TestMultiIxiaLoopbackQMU(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput);
#else
int TestMultiIxiaLoopbackQMU(int i4Argc, const char **szArgv);
#endif
extern void SetMultiETHEPConfig(unsigned int port);
#if 0 //!PORTING
extern cisr_status_t HdrcIsrIxia(unsigned int irq_no, void* pParam);
extern cisr_status_t HdrcIsrMultiIxia(unsigned int irq_no, void* pParam);
#endif
extern void ixia_dma_handler(int channel);
extern void ixia_tx_handler(int ep_num);
extern void ixia_rx_handler(int ep_num);
extern void multi_ixia_dma_handler(int channel);
extern void multi_ixia_tx_handler(int ep_num);
extern void multi_ixia_rx_handler(int ep_num);

/*******************************************************************************


 * GLOBAL VARIABLE DEFINATIONS


 ******************************************************************************/
#if 0 //!PORTING
 DVT_TEST_CASE_T g_asTS_USB_QMU_TestCases[] = {
    {DVT_AUTO, "QMU Loopback Random", "QMU random data loopback test using bulk transfer", TestQMUBulkLoopback, NULL},
    //{DVT_AUTO, "QMU multiple GPD loopback", "QMU random data loopback for multi GPD", TestQMUBulkMultiGPD, NULL},
    {DVT_AUTO, "QMU multiple GPD loopback", "QMU data loopback for multi GPD(10T10R)", TestQMUNGPD, NULL},
    {DVT_AUTO, "QMU multiple Queue", "QMU  random data for multiple Queue", TestMultiQueue, NULL},
    {DVT_AUTO, "QMU multiple Endpoint 4T&4R", "QMU  random data for multiple endpoint 4T & 4R", TestMultiEndpoint, NULL},
    {DVT_AUTO, "QMU Random stop", "QMU Tx random stop", TestQMUTxStop, NULL},
    {DVT_AUTO, "QMU Random stop", "QMU Rx random stop", TestQMURxStop, NULL},
    {DVT_AUTO, "MultiIxiaLoopbackQMU", "Multi Ixia LoopBack with QMU", TestMultiIxiaLoopbackQMU, NULL},    
    {DVT_AUTO, "MultiIxiaLoopbackQMU", "Multi Ixia LoopBack with QMU", TestMultiIxiaLoopbackQMU, NULL},    
    {DVT_AUTO, "Iso LoopbackQMU", "Iso LoopBack with QMU", TestQMUIsoLoopback, NULL},    
    {DVT_AUTO, "Iso Device LoopbackQMU", "Iso Device LoopBack with QMU", TestQMUDevIsoLoopback, NULL},    
    {0, NULL, NULL, NULL, NULL}
};
#endif
unsigned int _u4UsbQDebugLevel = 0;
/*******************************************************************************


 * FUNCTION DEFINATIONS


******************************************************************************/
#if 0  //!PORTING
DVT_STATUS_T TS_USB_QMU_TEST_Init(void **ppvInitStruct){
    DVT_TEST_SUITE_T sTestSuite;
    sTestSuite.pzCommand = "TS_USB_QMU_TEST";
    sTestSuite.pzDescription = "Test USB qmu";
    sTestSuite.psTestCase = g_asTS_USB_QMU_TestCases;
    sTestSuite.pfUsageFunc = TS_USB_QMU_ShowUsage;
    DVT_RegisterTestSuite(&sTestSuite);
    *ppvInitStruct = (void *)"TS_USB_QMU_TEST Init";
    return DVT_SUCCESS;
}

DVT_STATUS_T TS_USB_QMU_ShowUsage(UINT32 u4TestCaseId){
    switch (u4TestCaseId){
        default:
		        break;
   }
   return DVT_SUCCESS;
}
#endif
char ep_ioc_dis = FALSE;
char gpd_tx_ioc_en = TRUE;
char gpd_rx_ioc_en = TRUE;
char ioc_test = FALSE;

int TestIOCflagSetting(int i4Argc, const char **szArgv)
{
     if (i4Argc < 4)
     {
       Printf("[ep ioc dis][tx ioc dis][rx ioc dis]\n");
	   return 0;
     }
	 ioc_test = TRUE;
     ep_ioc_dis = StrToInt(szArgv[1]);     
	 gpd_tx_ioc_en = StrToInt(szArgv[2]); 
	 gpd_rx_ioc_en = StrToInt(szArgv[3]);
	 Printf("ioc setting - ep_ioc_dis %d gpd_tx_ioc_en %d gpd_rx_ioc_en %d\n",ep_ioc_dis,
	 gpd_tx_ioc_en,gpd_rx_ioc_en);

	 return 0;
}


void QMU_HdrcLoadFifo(unsigned char nEnd, unsigned int dwCount, const unsigned char* pSource)
{
#if 0
    unsigned int  dwIndex, dwIndex32;
    unsigned int  dwDatum;
    unsigned int  dwCount32 = ((unsigned int )pSource & 3) ? 0 : (dwCount >> 2);
    unsigned char bFifoOffset = MGC_FIFO_OFFSET(nEnd);

//    DBG (5, "LoadFifo end %d, count=%d, pSrc=%p\r\n", nEnd, dwCount, pSource);

    /* doublewords when possible */
    for(dwIndex = dwIndex32 = 0; dwIndex32 < dwCount32; dwIndex32++, dwIndex += 4)
    {
        dwDatum = *((unsigned int*)&(pSource[dwIndex]));
        MGC_Write32(bFifoOffset, dwDatum);
    }
    while(dwIndex < dwCount)
    {
        MGC_Write8(bFifoOffset, pSource[dwIndex]);
        dwIndex += 1;
    }
#else

#ifdef CC_MT5368
    uint32_t dwDatum = 0;

    uint32_t pBase;
    uint8_t bFifoOffset = MGC_FIFO_OFFSET(nEnd);
    uint32_t dwBufSize = 4;

    pBase = USB_HW_BASE_ADDR;

    //  MTK Notice: Max Liao, 2006/10/31.
    //  do not handle zero length data.
    if (dwCount == 0)
    {
        return;
    }

    //LOG(5, "[USB QFIFO]@0x%08X Load   %d\n", (UINT32)pSource, dwCount);    

    //assert(pSource);    

    /* byte access for unaligned */
    if ((dwCount > 0) && ((uint32_t)pSource & 3))
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
     		
     		MUSB_MemCopy((VOID *)&dwDatum, (const VOID *)pSource, dwBufSize);
     		
     		MGC_QREG_Write32(bFifoOffset, dwDatum);
     		
     		dwCount -= dwBufSize;
     		pSource += dwBufSize;
        }
    }
    else /* word access if aligned */
    {
        while ((dwCount > 3) && !((uint32_t)pSource & 3))
        {
            MGC_QREG_Write32(bFifoOffset, *((uint32_t *)((void *)pSource)));

            pSource += 4;
            dwCount -= 4;
        }

        if (3 == dwCount || 2 == dwCount)
        {
            //assert(dwCount < 4);

            // set FIFO byte count.
            MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 1);

            MGC_QREG_Write32(bFifoOffset, *((uint32_t *)((void *)pSource)));
			dwCount -= 2;
			pSource += 2;
        }
		
		if(1 == dwCount)
		{
		  MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 0);
		  if((uint32_t)pSource & 3)
		  {
     		
     		MUSB_MemCopy((VOID *)&dwDatum, (const VOID *)pSource, 1);
     		
     		MGC_QREG_Write32(bFifoOffset, dwDatum);		    
		  }
		  else
		  {
            MGC_QREG_Write32(bFifoOffset, *((uint32_t *)((void *)pSource)));		    
		  }
		}
    }
    MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 2);
#endif

    return;

#endif

}

void QMU_HdrcUnloadFifo(unsigned char nEnd, unsigned int dwCount, unsigned char* pDest)
{
#if 0
    unsigned int  dwIndex, dwIndex32;
    unsigned int  dwCount32 = ((unsigned int )pDest & 3) ? 0 : (dwCount >> 2);
    unsigned char bFifoOffset = MGC_FIFO_OFFSET(nEnd);

    Printf("UnLoadFifo end %d, count=%d, pDest=%p\r\n", nEnd, dwCount, pDest);

    /* doublewords when possible */
    for(dwIndex = dwIndex32 = 0; dwIndex32 < dwCount32; dwIndex32++, dwIndex += 4)
    {
        Printf("------------------------------------------\n");

        Printf("@[0x%08X], 0x%08X\n", (unsigned int)&(pDest[dwIndex]), (*((unsigned int*)(&(pDest[dwIndex])))));
        *((unsigned int*)&(pDest[dwIndex])) = MGC_QREG_Read32(bFifoOffset);
        Printf("0x%08X\n", *((unsigned int*)&(pDest[dwIndex])));
    }
    while(dwIndex < dwCount)
    {
        MGC_QREG_Write32(M_REG_FIFOBYTECNT, 0);
        pDest[dwIndex++] = MGC_QREG_Read8(bFifoOffset);
    }

    MGC_QREG_Write32(M_REG_FIFOBYTECNT, 2);
#else
    uint32_t dwDatum = 0;

    uint8_t bFifoOffset = MGC_FIFO_OFFSET(nEnd);
    uint32_t i;

    //  MTK Notice: Max Liao, 2006/10/31.
    //  do not handle zero length data.
    if (dwCount == 0)
    {
        return;
    }

    if (pDest == NULL)
    {
        Printf("*** WARNING ***, QMU_HdrcUnloadFifo: pDest is NULL\n");
        return;
    }

    //LOG(5, "[USB QFIFO]@0x%08X Unload %d\n", (UINT32)pDest, dwCount);    

    //assert(pDest);

    if (((uint32_t)pDest) & 3)
    {
        /* byte access for unaligned */
        while (dwCount > 0)
        {
            if (dwCount < 4)
            {
               if(3 == dwCount || 2 == dwCount)
               {
				   MGC_QREG_Write32(M_REG_FIFOBYTECNT, 1);
				   dwDatum = MGC_QREG_Read32(bFifoOffset);
				   
				   for (i = 0; i < 2; i++)
				   {
					   *pDest++ = ((dwDatum >> (i *8)) & 0xFF);
				   }
				   
                   dwCount -=2;
              }

			  if(1 == dwCount) 
			  {
			    MGC_QREG_Write32(M_REG_FIFOBYTECNT, 0);
                dwDatum = MGC_QREG_Read32(bFifoOffset);
				*pDest++ = (dwDatum  & 0xFF);
				 dwCount -= 1;
			  }
			   
			   
			   // set FIFO byte count = 4 bytes.
			   MGC_QREG_Write32(M_REG_FIFOBYTECNT, 2);
			   dwCount = 0;
            }
            else
            {
                dwDatum = MGC_QREG_Read32(bFifoOffset);

                for (i = 0; i < 4; i++)
                {
                    *pDest++ = ((dwDatum >> (i *8)) & 0xFF);
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
            *((uint32_t *)(pDest)) = MGC_QREG_Read32(bFifoOffset);
            pDest += 4;
            dwCount -= 4;
        }
        if (dwCount > 0)
        {
		    if(3 == dwCount ||2 == dwCount )
		    {
               MGC_QREG_Write32(M_REG_FIFOBYTECNT, 1);
			   dwDatum = MGC_QREG_Read32(bFifoOffset);
			    for (i = 0; i < 2; i++)
         		{
         			*pDest++ = ((dwDatum >> (i *8)) & 0xFF);
         		}
				dwCount -= 2;
			}

			if(1 == dwCount)
			{
               MGC_QREG_Write32(M_REG_FIFOBYTECNT,0);
			   dwDatum = MGC_QREG_Read32(bFifoOffset);
			   
			    *pDest++ = (dwDatum & 0xFF);
				dwCount -= 1;			   
			}
			
    		// set FIFO byte count = 4 bytes.
    		MGC_QREG_Write32(M_REG_FIFOBYTECNT, 2);
        }
    }

    return;

#endif
}

int _MUSB_QmuInit(int i4Agc, const char **szArgv)
{

    Printf("Arg: QMU init \n");

    initialQMUTestcase(TRUE);
    return 0;
}

#if 0  //!PORTING
DVT_STATUS_T TestQMUBulkLoopback(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput){
#else
int TestQMUBulkLoopback(int i4Argc, const char **szArgv){
#endif
    unsigned int j,k;
    //unsigned int istx;
#if 0  // mhzhang
   unsigned char keepGoing = TRUE;
   unsigned int userDecision;
   unsigned int randomValue;
#endif	 
    USB_REQ *req1 = NULL;
  // USB_REQ *req2 = NULL;
    unsigned int u4Type = 0;

	k=0;

    //vBdpDramBusy(TRUE);

    if (i4Argc < 1)
    {
        Printf("Arg: [Transfer Type] [TX Check Sum Printf] [RX Check Sum Printf]\n");
        Printf("[Transfer Type] 0:CTRL , 1:ISO , 2:BULK, 3:INTR \n");
      //  Printf("[TX Check Sum Printf] 0:Normal Case 1:TX Check Sum Printf\n");
       // Printf("[RX Check Sum Printf] 0:Normal Case 1:RX Check Sum Printf\n");
        //Printf("[Tx or Rx]1: Tx, 0 Rx\n");
        return 0;
    }

    u4Type = StrToInt(szArgv[1]);
    u4Type = 0x02;
    _fgTxErrChkSumTestCase = 0;//StrToInt(szArgv[2]);
    _fgRxErrChkSumTestCase = 0;//StrToInt(szArgv[3]);
    //istx = 1;//StrToInt(szArgv[4]);
#if 0
    if(_fgTxErrChkSumTestCase)
    {
        Printf("*** NOTE ***: Perform QMU TX Check SUM Printf test Now!\n");
        //Ummark TX Queue Printf Indication Mask.
        MGC_WriteQIRQ32(MGC_O_QIRQ_TQEIMR, 0);         
    }

    if(_fgRxErrChkSumTestCase)
    {
        Printf("*** NOTE ***: Perform QMU RX Check SUM Printf test Now!\n");
        //Ummark RX Queue Printf Indication Mask.
        MGC_WriteQIRQ32(MGC_O_QIRQ_RQEIMR, 0);                 
    }
    #endif
    //psOutput->eResult = DVT_RESULT_PASS;
    bError = FALSE;
    initialQMUTestcase(TRUE);
    //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
    if(isPass == -1){
        endQMUTestcase();
	 //CISR_Detach(hdrc_cisr, USB_HW_IRQ_NO);
        //CISR_Dest(hdrc_cisr);
        return DVT_FAIL;
    }
    //set address
    //MGC_QMU_Write8(MGC_O_HDRC_FADDR, 0x1);
    SetAddress(0x1);
    resetFIFO();
    //CISR_Detach(hdrc_cisr, USB_HW_IRQ_NO);
    //CISR_Dest(hdrc_cisr);
    if(u4Type == 2)
    {
        #ifdef TX_Test
            usb_ep_enable(1, USB_TX, USB_BULK, 512, 0, 1);
        #else
            usb_ep_enable(2, USB_RX, USB_BULK, 512, 0, 2);
        #endif
    }
    else if(u4Type == 1)
    {
        #ifdef TX_Test
            usb_ep_enable(1, USB_TX, USB_ISO, 1024, 0, 1);
        #else
            usb_ep_enable(2, USB_RX, USB_ISO, 1024, 0, 2);
        #endif
    }
    
    //usb_config_req(1, USB_TX, 1023, TRUE, 1, 3, 0, 0);
    //usb_config_req(2, USB_RX, 1023, TRUE, 2, 3, 0, 0);

    #if 0 //!PORTING      
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf("- Select QMU loopback test                                     -\r\n"); 
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf(" (0) BD (1) Data Buffer \r\n");
    Printf("\r\n");
    while(DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision, 0, (1)));
    if(userDecision == 0){
        is_bdp = TRUE;
    }else{
	 is_bdp = FALSE;
    }
    #else
    is_bdp = FALSE;
    #endif
    //while (keepGoing && !bError){   
    gpd_num=1;

    Printf("----------before start qmu\n");
   #ifdef TX_Test
        usb_start_QMU(1, USB_TX, 0);
    #else
        usb_start_QMU(2, USB_RX, 0);
    #endif
    Printf(">>>>>>>>>after start qmu>>>>>>>>>>\n");

	#ifdef Leon
    while(1)
        #endif
        {
	k++;
    
    for(j=24;j>4&& (!bError); j--)
    {   
    #ifdef TX_Test
        {
        //usb_start_QMU(1, USB_TX, 0);
        usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        }
    #else
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
    #endif
    #ifdef TX_Test
    {
        randomTxBuf(1);       
        req1 = usb_get_req(1, USB_TX);              
        usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
        Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
        usb_resume_QMU(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
    }
    #else
   {
      //Printf("2:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE); 
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        //Printf("3:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
        Printf("rx complete...\r\n");
    }
    #endif
    #ifdef Leon
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
     #endif
        Printf("length %d Done!\r\n", j);
		/*
        Printf("---------------------------------------------------------------------------\r\n"); 
        Printf("- Running loopback test                                     -\r\n"); 
        Printf("---------------------------------------------------------------------------\r\n"); 
        Printf(" (0) END (1) NEXT \r\n");
        Printf("\r\n");
        */
        /*
        while(DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision, 0, (1)));
        if(userDecision == 0){
            keepGoing = FALSE;            
        }
        */
    }
#if 0//def Leon
    for(j=510; j<520&& (!bError); j++){   
    //for(j=1; j<1025&& (!bError); j++){   
        usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
        randomTxBuf(1);       
        req1 = usb_get_req(1, USB_TX);
        usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
        usb_resume_QMU(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE);  
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        Printf("rx complete...\r\n");
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
        Printf("length %d Done!\r\n", j);
	
    }   
    Printf("end of 519\r\n");
    
    for(j=1022; j<1027&&(!bError); j++){   
	 usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
        randomTxBuf(1);       
        req1 = usb_get_req(1, USB_TX);
        usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
        usb_resume_QMU(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE);  
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        Printf("rx complete...\r\n");
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
        Printf("length %d Done!\r\n", j);
    }
    Printf("end of 1029\r\n");
    for(j=2045; j<2050&&( !bError); j++){   
	 usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
        randomTxBuf(1);       
        req1 = usb_get_req(1, USB_TX);
        usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
        usb_resume_QMU(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE);  
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        Printf("rx complete...\r\n");
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
        Printf("length %d Done!\r\n", j);
    }	
    Printf("end of 2049\r\n");

    
    for(j=9215; j<9218&& (!bError); j++){   
	 usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
        randomTxBuf(1);       
        req1 = usb_get_req(1, USB_TX);
        usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
        usb_resume_QMU(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE); 
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        Printf("rx complete...\r\n");
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
        Printf("length %d Done!\r\n", j);
    }	
    
    for(j=65534; j<65536&& (!bError); j++){   
	 usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
        randomTxBuf(1);       
        req1 = usb_get_req(1, USB_TX);
        usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
        usb_resume_QMU(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE); 
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        Printf("rx complete...\r\n");
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
        Printf("length %d Done!\r\n", j);
    }	
#endif

    Printf("QMU basic success\r\n");

}   
        #ifdef Leon
 usb_config_req(1, USB_TX, 512, TRUE, 1, 3, 0, 0);
    for(i=0; i<512; i++){
        req1->buf[i] = 0xff;
    }
    usb_insert_transfer_QMU(1, 3, USB_TX, req1->buf, req1->count, TRUE);
    usb_resume_QMU(1, USB_TX);
    while(!checkComplete(1, USB_TX));
    endQMUTestcase();
    #endif
    #if 0
    if(bError){
	psOutput->eResult = DVT_RESULT_FAIL;
    }else{
	psOutput->eType = DVT_OUTPUT_RESULT; 
    }
    #endif
    return DVT_SUCCESS;
}

#if 0
DVT_STATUS_T TestQMUBulkMultiGPD(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput){
#else
int TestQMUBulkMultiGPD(int i4Argc, const char **szArgv){
#endif
    unsigned int i,j;
#if 0 // mhzhang
  unsigned char keepGoing = TRUE;
    unsigned int userDecision;
    unsigned int randomValue;
#endif		

    USB_REQ *req1 = NULL;
    USB_REQ *req2 = NULL;
    //psOutput->eResult = DVT_RESULT_PASS;
    bError = FALSE;
    initialQMUTestcase(TRUE);
    if(isPass == -1){
        endQMUTestcase();
	 //CISR_Detach(hdrc_cisr, USB_HW_IRQ_NO);
        //CISR_Dest(hdrc_cisr);
        return DVT_FAIL;
    }
    //set address
    //MGC_QMU_Write8(MGC_O_HDRC_FADDR, 0x1);
    SetAddress(0x1);
    resetFIFO();
    //CISR_Detach(hdrc_cisr, USB_HW_IRQ_NO);
    //CISR_Dest(hdrc_cisr);
    usb_ep_enable(1, USB_TX, USB_BULK, 512, 0, 1);
    usb_ep_enable(2, USB_RX, USB_BULK, 512, 0, 2);
    //usb_config_req(1, USB_TX, 1023, TRUE, 1, 3, 0, 0);
    //usb_config_req(2, USB_RX, 1023, TRUE, 2, 3, 0, 0);
    gpd_num=6;
    is_bdp=TRUE;
    for(j=65525; j<65535&& (!bError); j++){   
	 usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);	
        randomTxBuf(1);
        usb_start_transfer_QMU(1, 1, USB_TX);
        req1 = usb_get_req(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
        usb_start_transfer_QMU(2, 2, USB_RX);
        req2 = usb_get_req(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        Printf("rx complete...\r\n");
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
        Printf("length %d Done!\r\n", j);	
    }	
    Printf("QMU multi GPD success\r\n");
    usb_config_req(1, USB_TX, 512, TRUE, 1, 3, 0, 0);
    for(i=0; i<512; i++){
        req1->buf[i] = 0xff;
    }
    usb_start_transfer_QMU(1, 1, USB_TX);
    endQMUTestcase();
    //psOutput->eType = DVT_OUTPUT_RESULT;    
    return DVT_SUCCESS;
}

int TestQMUNGPD(int i4Argc, const char **szArgv)
{
    unsigned int i,j;
    //unsigned char keepGoing = TRUE;
    //unsigned int userDecision;
    //unsigned int randomValue;
    USB_REQ *req1, *req2;
    unsigned int sum=0;
    unsigned int buflen[10]={511,512,514,515,516,2040,2048,2050,4096,9000};
    unsigned char* buf1, *buf2;  // mhzhang
    TGPD* nextgpd; // mhzhang
    //psOutput->eResult = DVT_RESULT_PASS;
    bError = FALSE;
    initialQMUTestcase(TRUE);
    //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);

    if(isPass == -1)
    {
        endQMUTestcase();
        //CISR_Detach(hdrc_cisr, USB_HW_IRQ_NO);
        //CISR_Dest(hdrc_cisr);
        return DVT_FAIL;
    }
    //set address
    //MGC_QMU_Write8(MGC_O_HDRC_FADDR, 0x1);
    SetAddress(0x1);
    resetFIFO();
    //CISR_Detach(hdrc_cisr, USB_HW_IRQ_NO);
    //CISR_Dest(hdrc_cisr);
    usb_ep_enable(1, USB_TX, USB_BULK, 512, 0, 1);
    usb_ep_enable(2, USB_RX, USB_BULK, 512, 0, 2);
    //usb_config_req(1, USB_TX, 1023, TRUE, 1, 3, 0, 0);
    //usb_config_req(2, USB_RX, 1023, TRUE, 2, 3, 0, 0);

    #if 0 //!PORTING      
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf("- Select QMU loopback test                                     -\r\n"); 
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf(" (0) BD (1) Data Buffer \r\n");
    Printf("\r\n");
    while(DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision, 0, (1)));
    if(userDecision == 0){
        is_bdp = TRUE;
    }else{
	 is_bdp = FALSE;
    }
    #else
    is_bdp = FALSE;
    #endif
    //while (keepGoing && !bError){   
    gpd_num=1;
    usb_start_QMU(1, USB_TX, 0);
    usb_start_QMU(2, USB_RX, 0);
    req1 = usb_get_req(1, USB_TX);  
    req2 = usb_get_req(2, USB_RX);
    usb_config_req(1, USB_TX, USB_BUF_SIZE, TRUE, 1, 3, 0, 0);
    usb_config_req(2, USB_RX, USB_BUF_SIZE, TRUE, 2, 3, 0, 0);     
    randomTxBuf(1); 
    clearRxBuf(2);
    buf1=req1->buf;
    buf2=req2->buf;
 		
    for(j=0;j<10; j++)
    {                      
        usb_insert_transfer_QMU(1, 1, USB_TX, buf1, buflen[j], FALSE); 
        usb_insert_transfer_QMU(2, 2,  USB_RX, buf2, buflen[j], TRUE); 
        //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
        buf1+=buflen[j];
        buf2+=buflen[j];
        sum+=buflen[j];
    }    
    nextgpd=TGPD_GET_NEXT(Rx_gpd_last[2]);
 		
    TGPD_SET_FLAGS_HWO(Tx_gpd_last[1]);
    usb_resume_QMU(2, USB_RX);
    usb_resume_QMU(1, USB_TX);    
    //while(!checkComplete(1, USB_TX));
    while(!checkComplete(2, USB_RX))
    {
        if(nextgpd != Rx_gpd_last[2])
        {
            continue;
        }
    	nextgpd=TGPD_GET_NEXT(Rx_gpd_last[2]);
    	Printf(">>>>>>Set Tx GPD: 0x%08X\r\n", (unsigned int)Tx_gpd_last[1]);
    	TGPD_SET_FLAGS_HWO(Tx_gpd_last[1]);
    	usb_resume_QMU(1, USB_TX);    		
        if(nextgpd == Rx_gpd_end[2])
        {
            break;
        }
    }
    //while(!checkComplete(2, USB_RX));

    //Printf("3:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
    for(i=0; i<sum; i++)
    {
        if(req2->buf[i] != req1->buf[i])
        {
            Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
            bError = TRUE;
            //psOutput->eResult = DVT_RESULT_FAIL;
            break;
        }
    }     
    Printf("QMU MultiGPD success\r\n");
    usb_config_req(1, USB_TX, 512, TRUE, 1, 3, 0, 0);
    for(i=0; i<512; i++)
    {
        req1->buf[i] = 0xff;
    }
    usb_insert_transfer_QMU(1, 3, USB_TX, req1->buf, req1->count, TRUE);
    usb_resume_QMU(1, USB_TX);
    while(!checkComplete(1, USB_TX));
    endQMUTestcase();
    #if 0
     if(bError){
	psOutput->eResult = DVT_RESULT_FAIL;
    }else{
	psOutput->eType = DVT_OUTPUT_RESULT; 
    }  
    #endif
    return DVT_SUCCESS;
}

void QMU_prepareTxBuf(unsigned char * buf, unsigned int count, unsigned int maxp){
//    int index=0; 
    int toggle=0;
    int maxValue=0;
    int indexValue=0;
    int i, j;
    maxValue = maxp -1;
    if(maxValue >= 256){
        maxValue = 255;
    }
      
    for(i=0, j=1; i<count; i++, j++){
        buf[i] = indexValue;
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
        if(j == maxp){
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

void QMU_clearRxBuf(unsigned char * buf, unsigned int count){
    int i;
    for(i=0; i<count; i++){
        buf[i] = 0;
    }
}

void QMU_checkRxBuf(unsigned char * buf, unsigned int count, unsigned int maxp){
//    int index=0; 
    int toggle=0;
    int maxValue=0;
    int indexValue=0;
    int i, j;
    //USB_REQ *req = usb_get_req(ep_num, USB_RX);
    //USB_EP_SETTING *setting = usb_get_ep_setting(ep_num, USB_RX);
    maxValue = maxp -1;
    if(maxValue >= 256){
        maxValue = 255;
    }
    for(i=0, j=1; i<count; i++, j++){
        if(buf[i] != indexValue){
            Printf("Failed: Data buffer[%d] is not match expected[%d] value[%d]\r\n", i, indexValue, buf[i]);
            bError = TRUE;
            break;
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
        if(j == maxp){
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

#if 0
DVT_STATUS_T TestMultiEndpoint(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput){
#else
int TestMultiEndpoint(int i4Argc, const char **szArgv){    
#endif
   //  int userDecision;
    int isHS = 0;  // 0 is added by mhzhang
    int status = DVT_FAIL;

// mhzhang
    unsigned char *buf1 = (unsigned char *)MUSB_MemAlloc(USB_BUF_SIZE);
    unsigned char *buf2 = (unsigned char *)MUSB_MemAlloc(USB_BUF_SIZE);//, 32);
    unsigned char *buf3 = (unsigned char *)MUSB_MemAlloc(USB_BUF_SIZE);//, 32);
    unsigned char *buf4 = (unsigned char *)MUSB_MemAlloc(USB_BUF_SIZE);//, 32);
    unsigned char *buf5 = (unsigned char *)MUSB_MemAlloc(USB_BUF_SIZE);//, 32);
    unsigned char *buf6 = (unsigned char *)MUSB_MemAlloc(USB_BUF_SIZE);//, 32);
    unsigned char *buf7 = (unsigned char *)MUSB_MemAlloc(USB_BUF_SIZE);//, 32);
    unsigned char *buf8 = (unsigned char *)MUSB_MemAlloc(USB_BUF_SIZE);//, 32);
    USB_REQ *req1;
    USB_REQ *req8;
		
    //user input speed
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf("- Select speed                                     -\r\n"); 
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf(" (0) RETURN \r\n");
    Printf(" Please select speed: (1)High speed (2)Full speed");
    Printf("\r\n");
    #if !PORTING //make isHS as CLI parameter
    while (DVT_SUCCESS != DVT_GetUserNumCommand(&isHS, 0, (2)));
    #endif
    if(0 == isHS){ // who give isHS value ?
        //psOutput->eResult = DVT_RESULT_IGNORE;
        goto TEST_DVT_SUCCESS;
    }    
    if(isHS == 1){
        initialQMUTestcase(TRUE);
    }
    else{
        initialQMUTestcase(FALSE);
    }

    #if 0 //!PORTING
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf("- Select QMU loopback test                                     -\r\n"); 
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf(" (0) BD (1) Data Buffer \r\n");
    Printf("\r\n");
    while(DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision, 0, (1)));

    if(userDecision == 0){
        is_bdp = TRUE;
    }else{
	 is_bdp = FALSE;
    }
    #else
    is_bdp = FALSE;
    #endif    
    //while (keepGoing && !bError){   
    gpd_num=1;
    
    
    if(isPass == -1){
        endTestcase();
        goto TEST_DVT_FAIL;
    }
    //set address
    //MGC_QMU_Write8(MGC_O_HDRC_FADDR, 0x1);
    SetAddress(0x1);
    resetFIFO();
    //CISR_Detach(hdrc_cisr, USB_HW_IRQ_NO);
    //CISR_Dest(hdrc_cisr);
    if(isHS == 1){
        usb_ep_enable(1, USB_TX, USB_BULK, 256, 0, 1);
        //usb_ep_enable(2, USB_TX, USB_BULK, 512, 0, 2);
        
        //usb_ep_enable(1, USB_RX, USB_BULK, 512, 0, 3);
        usb_ep_enable(2, USB_RX, USB_BULK, 256, 0, 4);
    }
    else{
        usb_ep_enable(1, USB_TX, USB_BULK, 64, 0, 1);
        //usb_ep_enable(2, USB_TX, USB_BULK, 64, 0, 2);
        
        //usb_ep_enable(1, USB_RX, USB_BULK, 64, 0, 3);
        usb_ep_enable(2, USB_RX, USB_BULK, 64, 0, 4);        
    }

#if 0		
    unsigned char buf1[USB_BUF_SIZE];
    unsigned char buf2[USB_BUF_SIZE];
    unsigned char buf3[USB_BUF_SIZE];
    unsigned char buf4[USB_BUF_SIZE];
    unsigned char buf5[USB_BUF_SIZE];
    unsigned char buf6[USB_BUF_SIZE];
    unsigned char buf7[USB_BUF_SIZE];
    unsigned char buf8[USB_BUF_SIZE];
#endif
		
    QMU_prepareTxBuf(buf1, 9216, 256);
    QMU_prepareTxBuf(buf2, 9216, 256);
    QMU_prepareTxBuf(buf3, 9216, 256);
    QMU_prepareTxBuf(buf4, 9216, 256);
    
    QMU_clearRxBuf(buf1, 9216);
    QMU_clearRxBuf(buf2, 9216);
    QMU_clearRxBuf(buf3, 9216);
    QMU_clearRxBuf(buf4, 9216);
    
    //usb_config_req(1, USB_TX, 9216, TRUE, 1, 3, 0, 0);    
    usb_insert_transfer_QMU(1, 1, USB_TX, buf1, 9216, TRUE); 
    usb_insert_transfer_QMU(1, 2, USB_TX, buf2, 9216, TRUE); 
    usb_insert_transfer_QMU(1, 3, USB_TX, buf3, 9216, TRUE); 
    usb_insert_transfer_QMU(1, 4, USB_TX, buf4, 9216, TRUE); 
   
    usb_insert_transfer_QMU(2, 1, USB_RX, buf5, 9216, TRUE); 
    usb_insert_transfer_QMU(2, 2, USB_RX, buf6, 9216, TRUE); 
    usb_insert_transfer_QMU(2, 3, USB_RX, buf7, 9216, TRUE); 
    usb_insert_transfer_QMU(2, 4, USB_RX, buf8, 9216, TRUE); 
    
    
    
#if 0 // mhzhang
    USB_REQ *req1 = usb_get_req(1, USB_TX);   
    USB_REQ *req8 = usb_get_req(2, USB_RX);
#else
    req1 = usb_get_req(1, USB_TX);   
    req8 = usb_get_req(2, USB_RX);
#endif

    req1->complete=FALSE;
    req8->complete=FALSE;
    
    usb_start_QMU(1, USB_TX, 1);
    usb_start_QMU(2, USB_RX, 1);
    
    //usb_start_transfer_QMU(2, 4, USB_RX);
    Printf("req1->complete : %d\r\n", req1->complete);
    i=0;
    //req1->complete && req2->complete &&   && req4->complete
    while(i<usecs && !(req1->complete  && req8->complete) ){
        USB_hal_delay_ms(1000);
        i++;
    }
    Printf("req1->complete : %d\r\n", req1->complete);
    if(i >= usecs){
        Printf("Failed: EP(%d) Data in not done after timeout (%d) secs", 1,usecs);
        isPass = -1;
    }
    if(bError){
        isPass = -1;
    }
    if(isPass == 0){
    	 Printf("Check buf 5\r\n");
        QMU_checkRxBuf(buf5, 9216, 256);
        Printf("Check buf 6\r\n");
        QMU_checkRxBuf(buf6, 9216, 256);
        Printf("Check buf 7\r\n");
        QMU_checkRxBuf(buf7, 9216, 256);
        Printf("Check buf 8\r\n");
        QMU_checkRxBuf(buf8, 9216, 256);
//        checkRxBuf(3);
//        checkRxBuf(4);
    }
    if(bError){
        isPass = -1;
    }
    endQMUTestcase();
    if(isPass == 0){
        #if 0
        psOutput->eType = DVT_OUTPUT_RESULT;
        psOutput->eResult = DVT_RESULT_PASS;
        #endif
        goto TEST_DVT_SUCCESS;
    }
    else{
        #if 0
        psOutput->eType = DVT_OUTPUT_RESULT;
        psOutput->eResult = DVT_RESULT_FAIL;
        #endif
        goto  TEST_DVT_FAIL;
    }

TEST_DVT_SUCCESS:
    status = DVT_SUCCESS;    
TEST_DVT_FAIL:
    #if 0
    //x_free_aligned_mem(buf1);
    //x_free_aligned_mem(buf2);
    //x_free_aligned_mem(buf3);
    //x_free_aligned_mem(buf4);
    //x_free_aligned_mem(buf5);
    //x_free_aligned_mem(buf6);
    //x_free_aligned_mem(buf7);
    //x_free_aligned_mem(buf8);
#endif
    buf1 = NULL;
    buf2 = NULL;
    buf3 = NULL;
    buf4 = NULL;
    buf5 = NULL;
    buf6 = NULL;
    buf7 = NULL;
    buf8 = NULL;

    return status;
}

#if 0
DVT_STATUS_T TestMultiQueue(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput){
#else
int TestMultiQueue(int i4Argc, const char **szArgv){
#endif
//    int userDecision;
    int isHS = 0; // 0 is added by mhzhang
		
    // mhzhang
    USB_REQ *req1 = usb_get_req(1, USB_TX);
    USB_REQ *req2 = usb_get_req(2, USB_TX);
    USB_REQ *req3 = usb_get_req(3, USB_TX);
    USB_REQ *req4 = usb_get_req(4, USB_TX);
    USB_REQ *req5 = usb_get_req(1, USB_RX);
    USB_REQ *req6 = usb_get_req(2, USB_RX);
    USB_REQ *req7 = usb_get_req(3, USB_RX);
    USB_REQ *req8 = usb_get_req(4, USB_RX);
		
    //user input speed
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf("- Select speed                                     -\r\n"); 
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf(" (0) RETURN \r\n");
    Printf(" Please select speed: (1)High speed (2)Full speed");
    Printf("\r\n");
    #if !PORTING //add CLI parameter
    while (DVT_SUCCESS != DVT_GetUserNumCommand(&isHS, 0, (2)));
    #endif
    if(0 == isHS){   // who give isHS value ?????     
        //psOutput->eResult = DVT_RESULT_IGNORE;
        return DVT_SUCCESS;
    }    
    if(isHS == 1){
        initialQMUTestcase(TRUE);
    }
    else{
        initialQMUTestcase(FALSE);
    }

    #if 0// !PORTING
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf("- Select QMU loopback test                                     -\r\n"); 
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf(" (0) BD (1) Data Buffer \r\n");
    Printf("\r\n");

    while(DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision, 0, (1)));
    if(userDecision == 0){
        is_bdp = TRUE;
    }else{
	 is_bdp = FALSE;
    }
    #else
    is_bdp = FALSE;
    #endif
    //while (keepGoing && !bError){   
    gpd_num=1;
    
    
    if(isPass == -1){
        endTestcase();
        return DVT_FAIL;
    }
    //set address
    //MGC_QMU_Write8(MGC_O_HDRC_FADDR, 0x1);
    SetAddress(0x1);
    resetFIFO();
    //CISR_Detach(hdrc_cisr, USB_HW_IRQ_NO);
    //CISR_Dest(hdrc_cisr);
    if(isHS == 1){
        usb_ep_enable(1, USB_TX, USB_BULK, 256, 0, 1);
        usb_ep_enable(2, USB_TX, USB_BULK, 256, 0, 2);
        usb_ep_enable(3, USB_TX, USB_BULK, 256, 0, 3);
        usb_ep_enable(4, USB_TX, USB_BULK, 256, 0, 4);
        
        usb_ep_enable(1, USB_RX, USB_BULK, 256, 0, 1);
        usb_ep_enable(2, USB_RX, USB_BULK, 256, 0, 2);
        usb_ep_enable(3, USB_RX, USB_BULK, 256, 0, 3);
        usb_ep_enable(4, USB_RX, USB_BULK, 256, 0, 4);
    }
    else{
        usb_ep_enable(1, USB_TX, USB_BULK, 64, 0, 1);
        usb_ep_enable(2, USB_TX, USB_BULK, 64, 0, 2);
        
        usb_ep_enable(1, USB_RX, USB_BULK, 64, 0, 3);
        usb_ep_enable(2, USB_RX, USB_BULK, 64, 0, 4);        
    }
    usb_config_req(1, USB_TX, 9216, TRUE, 1, 3, 0, 0);
    usb_config_req(2, USB_TX, 9216, TRUE, 2, 3, 0, 0);    
    usb_config_req(3, USB_TX, 9216, TRUE, 3, 3, 0, 0);
    usb_config_req(4, USB_TX, 9216, TRUE, 4, 3, 0, 0);    
   
    usb_config_req(1, USB_RX, 9216, TRUE, 1, 3, 0, 0);
    usb_config_req(2, USB_RX, 9216, TRUE, 2, 3, 0, 0);   
    usb_config_req(3, USB_RX, 9216, TRUE, 3, 3, 0, 0);
    usb_config_req(4, USB_RX, 9216, TRUE, 4, 3, 0, 0);    
    
    prepareTxBuf(1);
    prepareTxBuf(2);
    prepareTxBuf(3);
    prepareTxBuf(4);
    
    clearRxBuf(1);
    clearRxBuf(2);
    clearRxBuf(3);
    clearRxBuf(4);

#if 0 // mhzhang
    USB_REQ *req1 = usb_get_req(1, USB_TX);
    USB_REQ *req2 = usb_get_req(2, USB_TX);
    USB_REQ *req3 = usb_get_req(3, USB_TX);
    USB_REQ *req4 = usb_get_req(4, USB_TX);
    USB_REQ *req5 = usb_get_req(1, USB_RX);
    USB_REQ *req6 = usb_get_req(2, USB_RX);
    USB_REQ *req7 = usb_get_req(3, USB_RX);
    USB_REQ *req8 = usb_get_req(4, USB_RX);
#endif

    
    usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
    usb_insert_transfer_QMU(2, 2, USB_TX, req2->buf, req2->count, TRUE); 
    usb_insert_transfer_QMU(3, 3, USB_TX, req3->buf, req3->count, TRUE); 
    usb_insert_transfer_QMU(4, 4, USB_TX, req4->buf, req4->count, TRUE); 
    usb_insert_transfer_QMU(1, 1, USB_RX, req5->buf, req5->count, TRUE); 
    usb_insert_transfer_QMU(2, 2, USB_RX, req6->buf, req6->count, TRUE); 
    usb_insert_transfer_QMU(3, 3, USB_RX, req7->buf, req7->count, TRUE); 
    usb_insert_transfer_QMU(4, 4, USB_RX, req8->buf, req8->count, TRUE); 
    //usb_start_transfer_QMU(2, 4, USB_RX);

    usb_start_QMU(1, USB_TX, 0);
    usb_start_QMU(2, USB_TX, 0);
    usb_start_QMU(3, USB_TX, 0);
    usb_start_QMU(4, USB_TX, 0);
    usb_start_QMU(1, USB_RX, 0);
    usb_start_QMU(2, USB_RX, 0);
    usb_start_QMU(3, USB_RX, 0);
    usb_start_QMU(4, USB_RX, 0);
    
    i=0;
    //req1->complete && req2->complete &&   && req4->complete
    while(i<usecs && 
    	!(req1->complete && req2->complete && req3->complete && req4->complete && req5->complete && req6->complete &&  req7->complete && req8->complete) ){
        USB_hal_delay_ms(1000);
        i++;
    }
    if(i >= usecs){
        Printf("Failed: EP(%d) Data in not done after timeout (%d) secs", 1,usecs);
        isPass = -1;
    }
    if(bError){
        isPass = -1;
    }
    if(isPass == 0){
        checkRxBuf(1);
        checkRxBuf(2);
        checkRxBuf(3);
        checkRxBuf(4);
    }
    if(bError){
        isPass = -1;
    }
    endQMUTestcase();
    
    if(isPass == 0){
        #if 0
        psOutput->eType = DVT_OUTPUT_RESULT;
        psOutput->eResult = DVT_RESULT_PASS;
        #endif
        return DVT_SUCCESS;
    }
    else{
        #if 0
        psOutput->eType = DVT_OUTPUT_RESULT;
        psOutput->eResult = DVT_RESULT_FAIL;
        #endif
        return DVT_FAIL;
    }
}


#define INTR_TXEP   3
#define INTR_RXEP   4

#define BULK_TXEP   1
#define BULK_RXEP   2

#if 0
DVT_STATUS_T TestQMURxStop(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput){
#else
int TestQMURxStop(int i4Argc, const char **szArgv)
{
#endif
   // int userDecision, userDecision1;
    unsigned int data_count = 2000;
    unsigned char bStart;
    USB_REQ * req1, *req2, *req3, *req4;
    unsigned short wIntrRxValue;
    unsigned short wIntrTxValue;		
    unsigned short wIntrTxEValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    unsigned short wIntrRxEValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
	int rand ;
	
    bStop = FALSE;
    bStart = TRUE;
    initialQMUTestcase(TRUE);
    
    MGC_QMU_Write16(MGC_O_HDRC_INTRTXE, (0x1<<INTR_TXEP)|wIntrTxEValue);
    MGC_QMU_Write16(MGC_O_HDRC_INTRRXE, (0x1<<INTR_RXEP)|wIntrRxEValue);
    
    SetAddress(1);
    usb_ep_enable(INTR_TXEP, USB_TX, USB_INTR, 16, 0, 1);
    usb_ep_enable(INTR_RXEP, USB_RX, USB_INTR, 16, 0, 2);
    usb_ep_enable(BULK_TXEP, USB_TX, USB_BULK, 512, 0, 3);
    usb_ep_enable(BULK_RXEP, USB_RX, USB_BULK, 512, 0, 4);

    usb_config_req(INTR_TXEP, USB_TX, 8, FALSE, 1, 3, 0, 0);
    usb_config_req(INTR_RXEP, USB_RX, 8, FALSE, 1, 3, 0, 0);
    usb_config_req(BULK_TXEP, USB_TX, 65535, TRUE, 1, 3, 0, 0);
    usb_config_req(BULK_RXEP, USB_RX, 0, TRUE, 2, 3, 0, 0);
    
    
    bStart = TRUE;

#if 0 // mhzhang		
    USB_REQ *req1 = usb_get_req(1, USB_TX);
    USB_REQ *req2 = usb_get_req(2, USB_RX);
    USB_REQ *req3 = usb_get_req(3, USB_TX);
    USB_REQ *req4 = usb_get_req(4, USB_RX);
#else 	
    req1 = usb_get_req(INTR_TXEP, USB_TX);
    req2 = usb_get_req(INTR_RXEP, USB_RX);
    req3 = usb_get_req(BULK_TXEP, USB_TX);
    req4 = usb_get_req(BULK_RXEP, USB_RX);
#endif


#if 0
    QMU_prepareTxBuf(req3->buf, 65535, 512);
    USB_hal_delay_ms(500);
    req3->count = data_count++;
    req3->complete=FALSE;
    usb_insert_transfer_QMU(BULK_TXEP, 3, USB_TX, req3->buf, req3->count, FALSE);    
    for(i=1; i<100; i++){
    	     req3->count = data_count++;            
	     usb_insert_transfer_QMU(BULK_TXEP, 3, USB_TX, req3->buf, req3->count, TRUE);
    }
    Printf("Tx_gpd_last[3]: 0x%x!!\r\n", Tx_gpd_last[3]);
    TGPD_SET_FLAGS_HWO(Tx_gpd_last[3]);
    usb_start_QMU(BULK_TXEP, USB_TX, FALSE);
#endif

#if 1
    req4->actual = 0;
    req4->complete=FALSE;
    QMU_clearRxBuf(req4->buf, 65535);
    USB_hal_delay_ms(500);
    usb_insert_transfer_QMU(BULK_RXEP, 4, USB_RX, req4->buf, 65535,  FALSE);    
    for(i=1; i<100; i++){       
	     usb_insert_transfer_QMU(BULK_RXEP, 4, USB_RX, req4->buf, 65535,  TRUE);   
    }
//    Printf("Rx_gpd_last[4]: 0x%x!!\r\n", Rx_gpd_last[4]);
    TGPD_SET_FLAGS_HWO(Rx_gpd_last[BULK_RXEP]);
    usb_start_QMU(BULK_RXEP, USB_RX, FALSE);
#endif
    
    //usb_insert_transfer_QMU(4, 4, USB_RX, req4->buf, req4->count, TRUE);
//    usb_start_transfer(4, USB_RX);

    rand = USB_get_random();
    USB_hal_delay_ms(10);
    while(1){
         rand = USB_get_random();
         if(rand % 20 == 0){
            if(bStart){
                bStop = TRUE;
                //usb_stop_QMU(3, USB_TX);
                usb_stop_QMU(BULK_RXEP, USB_RX);
                Printf("stop!!\r\n");
                USB_hal_delay_ms(50);
                req1->buf[0] = 0xff;
                req1->complete = FALSE;
                req1->actual = 0;
                req2->actual = 0;
                req2->complete = FALSE;
                req3->complete = FALSE;
                req3->actual = 0;
                req4->complete = FALSE;
                req4->actual = 0;                
                /*
                MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, 3, MGC_M_TXCSR_FLUSHFIFO);
                MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, 4, MGC_M_TXCSR_FLUSHFIFO);
                */
#if 0 // mhzhang                
                unsigned short wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
                unsigned short wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
#else								
                wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
                wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);						
#endif								
                MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (wIntrTxValue));
                MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (wIntrRxValue));
                bStop = FALSE;
                usb_start_transfer(INTR_TXEP, USB_TX);
                usb_start_transfer(INTR_RXEP, USB_RX);
                Printf("wait done!!\r\n");
                while(!checkComplete(INTR_TXEP, USB_TX));
                while(!checkComplete(INTR_RXEP, USB_RX));
                Printf("stop done!!\r\n");
                //Flush_QMU(3, USB_TX);
                Flush_QMU(BULK_RXEP, USB_RX);
                USB_hal_delay_ms(300);
                bStart = FALSE;
            }
            else{
                Printf("start!!\r\n");
                req1->buf[0] = 0x0;
                req1->actual = 0;
                req1->complete = FALSE;
                req2->actual = 0;
                req2->complete = FALSE;
                /*
                MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, 3, MGC_M_TXCSR_FLUSHFIFO);
                MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, 4, MGC_M_TXCSR_FLUSHFIFO);
                */
#if 0 // mhzhang                
                unsigned short wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
                unsigned short wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
#else
                wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
                wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);

#endif

                MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (wIntrTxValue));
                MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (wIntrRxValue));
                usb_start_transfer(INTR_TXEP, USB_TX);
                usb_start_transfer(INTR_RXEP, USB_RX);
                while(!checkComplete(INTR_TXEP, USB_TX));
                while(!checkComplete(INTR_RXEP, USB_RX));
                //usb_resume_QMU(3, USB_TX);
                //dexiao update from tianhao! should use restart! 
                //usb_resume_QMU(BULK_RXEP, USB_RX);
                usb_restart_QMU(BULK_RXEP, USB_RX);
                Printf("start done!!\r\n");
                bStart = TRUE;    
                USB_hal_delay_ms(30);
//                usb_start_transfer(4, USB_RX);
            }
        }
        if(checkComplete(BULK_TXEP, USB_TX)){
            Printf("Tx complete, len[%d]\r\n", req3->count);
            req3->complete = FALSE;
            req3->actual = 0;     
            req3->count = data_count++;
            if(data_count >= 65536){
                data_count = 9216;
            }
            usb_insert_transfer_QMU(BULK_TXEP, 3, USB_TX, req3->buf, req3->count, FALSE);    
 	     for(i=1;i<100;i++){
 	    	     req3->count = data_count++;
 	    	     if(data_count >= 65536){
                		data_count = 9216;
            	     }
 	            usb_insert_transfer_QMU(BULK_TXEP, 3, USB_TX, req3->buf, req3->count, TRUE);
 	     }
 	     TGPD_SET_FLAGS_HWO(Tx_gpd_last[BULK_TXEP]);
 	     if(bStart){
 	     		usb_resume_QMU(BULK_TXEP, USB_TX);
 	     }
        }
        
        if(checkComplete(BULK_RXEP, USB_RX)){
            Printf("Rx complete...\r\n");
            req4->actual = 0;
            req4->complete = FALSE;
            usb_insert_transfer_QMU(BULK_RXEP, 4, USB_RX, req4->buf, 65535,  FALSE);    
    	     for(i=1; i<100; i++){       
    		     usb_insert_transfer_QMU(BULK_RXEP, 4, USB_RX, req4->buf, 65535,  TRUE);   
    	     }    	     
    	     TGPD_SET_FLAGS_HWO(Rx_gpd_last[BULK_RXEP]);
    	     if(bStart){
 	     	     usb_resume_QMU(BULK_RXEP, USB_RX);
 	     }
        }       
    }    
}

int TestQMUTxStop(int i4Argc, const char **szArgv)
{
    //int userDecision, userDecision1;
    unsigned int data_count = 2000;
    unsigned char bStart;
    int rand;
    unsigned short wIntrTxValue;		
    unsigned short wIntrRxValue;
    unsigned short  wIntrTxEValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    unsigned short wIntrRxEValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);

    USB_REQ *req1,*req2, *req3, *req4;

    bStop = FALSE;
    bStart = TRUE;
    initialQMUTestcase(TRUE);		
    
    MGC_QMU_Write16(MGC_O_HDRC_INTRTXE, (0x1 << INTR_TXEP)|wIntrTxEValue);
    MGC_QMU_Write16(MGC_O_HDRC_INTRRXE, (0x1 << INTR_RXEP)|wIntrRxEValue);
    
    SetAddress(1);
    usb_ep_enable(INTR_TXEP, USB_TX, USB_INTR, 16, 0, 1);
    usb_ep_enable(INTR_RXEP, USB_RX, USB_INTR, 16, 0, 2);
    usb_ep_enable(BULK_TXEP, USB_TX, USB_BULK, 512, 0, 3);
    usb_ep_enable(BULK_RXEP, USB_RX, USB_BULK, 512, 0, 4);

    usb_config_req(INTR_TXEP, USB_TX, 8, FALSE, 1, 3, 0, 0);
    usb_config_req(INTR_RXEP, USB_RX, 8, FALSE, 1, 3, 0, 0);
    usb_config_req(BULK_TXEP, USB_TX, 65535, TRUE, 1, 3, 0, 0);
    usb_config_req(BULK_RXEP, USB_RX, 0, TRUE, 2, 3, 0, 0);    
    
    bStart = TRUE;

    req1 = usb_get_req(INTR_TXEP, USB_TX);
    req2 = usb_get_req(INTR_RXEP, USB_RX);
    req3 = usb_get_req(BULK_TXEP, USB_TX);
    req4 = usb_get_req(BULK_RXEP, USB_RX);

#if 1
    QMU_prepareTxBuf(req3->buf, 65535, 512);
    USB_hal_delay_ms(500);
    req3->count = data_count++;
    req3->complete=FALSE;
    usb_insert_transfer_QMU(BULK_TXEP, 3, USB_TX, req3->buf, req3->count, FALSE);    
    for(i=1; i<100; i++)
    {
        req3->count = data_count++;            
        usb_insert_transfer_QMU(BULK_TXEP, 3, USB_TX, req3->buf, req3->count, TRUE);
    }
    Printf("Tx_gpd_last[%d]: 0x%x!!\r\n", BULK_TXEP, (unsigned int)Tx_gpd_last[BULK_TXEP]);
    TGPD_SET_FLAGS_HWO(Tx_gpd_last[BULK_TXEP]);
    usb_start_QMU(BULK_TXEP, USB_TX, FALSE);
#endif

#if 0
    req4->actual = 0;
    req4->complete=FALSE;
    QMU_clearRxBuf(req4->buf, 65535);
    USB_hal_delay_ms(500);
    usb_insert_transfer_QMU(BULK_RXEP, 4, USB_RX, req4->buf, 65535,  FALSE);    
    for(i=1; i<100; i++){       
	     usb_insert_transfer_QMU(BULK_RXEP, 4, USB_RX, req4->buf, 65535,  TRUE);   
    }
    Printf("Rx_gpd_last[4]: 0x%x!!\r\n", Rx_gpd_last[4]);
    TGPD_SET_FLAGS_HWO(Rx_gpd_last[BULK_RXEP]);
    usb_start_QMU(BULK_RXEP, USB_RX, FALSE);
#endif
    
    //usb_insert_transfer_QMU(4, 4, USB_RX, req4->buf, req4->count, TRUE);
    //usb_start_transfer(4, USB_RX);

    rand = USB_get_random();
    USB_hal_delay_ms(10);
    while(1)
    {
         rand = USB_get_random();
         if(rand % 20 == 0)
         {
            if(bStart)
            {
                bStop = TRUE;
                usb_stop_QMU(BULK_TXEP, USB_TX);
                //usb_stop_QMU(4, USB_RX);
                Printf("stop!!\r\n");
                USB_hal_delay_ms(50);
                req1->buf[0] = 0xff;
                req1->complete = FALSE;
                req1->actual = 0;
                req2->actual = 0;
                req2->complete = FALSE;
                req3->complete = FALSE;
                req3->actual = 0;
                req4->complete = FALSE;
                req4->actual = 0;                
                /*
                MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, 3, MGC_M_TXCSR_FLUSHFIFO);
                MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, 4, MGC_M_TXCSR_FLUSHFIFO);
                */
                wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
                wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);

                MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (wIntrTxValue));
                MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (wIntrRxValue));
                bStop = FALSE;
                usb_start_transfer(INTR_TXEP, USB_TX);
                usb_start_transfer(INTR_RXEP, USB_RX);
                Printf("wait done!!\r\n");
                while(!checkComplete(INTR_TXEP, USB_TX));
                while(!checkComplete(INTR_RXEP, USB_RX));
                Flush_QMU(BULK_TXEP, USB_TX);
                //Flush_QMU(4, USB_RX);
                Printf("stop done!!\r\n");
                USB_hal_delay_ms(300);
                bStart = FALSE;
            }
            else{
                Printf("start!!\r\n");
                req1->buf[0] = 0x0;
                req1->actual = 0;
                req1->complete = FALSE;
                req2->actual = 0;
                req2->complete = FALSE;
                /*
                MGC_WriteCsr16(MGC_O_HDRC_TXCSR, 3, MGC_M_TXCSR_FLUSHFIFO);
                MGC_WriteCsr16(MGC_O_HDRC_TXCSR, 4, MGC_M_TXCSR_FLUSHFIFO);
                */
                wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
                wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);

                MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (wIntrTxValue));
                MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (wIntrRxValue));
                usb_start_transfer(INTR_TXEP, USB_TX);
                usb_start_transfer(INTR_RXEP, USB_RX);
                while(!checkComplete(INTR_TXEP, USB_TX));
                while(!checkComplete(INTR_RXEP, USB_RX));
                usb_resume_QMU(BULK_TXEP, USB_TX);
                //usb_resume_QMU(4, USB_RX);
                Printf("start done!!\r\n");
                bStart = TRUE;    
                USB_hal_delay_ms(30);
                //usb_start_transfer(4, USB_RX);
            }
        }
         
        if(checkComplete(BULK_TXEP, USB_TX))
        {
            Printf("Tx complete, len[%d]\r\n", req3->count);
            req3->complete = FALSE;
            req3->actual = 0;     
            req3->count = data_count++;

            if(data_count >= 65536)
            {
                data_count = 9216;
            }

            usb_insert_transfer_QMU(BULK_TXEP, 3, USB_TX, req3->buf, req3->count, FALSE);    

            for(i=1;i<100;i++)
            {
                req3->count = data_count++;
                if(data_count >= 65536)
                {
                    data_count = 9216;
                }
                usb_insert_transfer_QMU(BULK_TXEP, 3, USB_TX, req3->buf, req3->count, TRUE);
            }

            TGPD_SET_FLAGS_HWO(Tx_gpd_last[BULK_TXEP]);

            if(bStart)
            {
                usb_resume_QMU(BULK_TXEP, USB_TX);
            }
        }
        
        if(checkComplete(BULK_RXEP, USB_RX))
        {
            Printf("Rx complete...\r\n");
            req4->actual = 0;
            req4->complete = FALSE;
            usb_insert_transfer_QMU(BULK_RXEP, 4, USB_RX, req4->buf, 65535,  FALSE);    
    	     for(i=1; i<100; i++)
    	     {       
    		     usb_insert_transfer_QMU(BULK_RXEP, 4, USB_RX, req4->buf, 65535,  TRUE);   
    	     }    	     
    	     TGPD_SET_FLAGS_HWO(Rx_gpd_last[BULK_RXEP]);
    	     if(bStart)
    	     {
 	     	     usb_resume_QMU(BULK_RXEP, USB_RX);
 	     	 }
        }
    }    
}

int TestQMU2TxStop(int i4Argc, const char **szArgv)
{
    //int userDecision, userDecision1;
    int rand;
    unsigned int data_count = 2000;
    unsigned char bStart1, bStart2;
    PGPD gpd = NULL;
    PGPD gpd_tmp = NULL;
    USB_REQ *req1 = NULL;
    USB_REQ *req2 = NULL;
    USB_REQ *req3 = NULL;
    USB_REQ *req4 = NULL;
    unsigned short wIntrTxValue = 0;
    unsigned short wIntrRxValue = 0;

    isFreeGPD = FALSE;
    bStop = FALSE;
    bStart1 = TRUE;
    bStart2 = TRUE;    
    initialQMUTestcase(TRUE);

    MGC_QMU_Write16(MGC_O_HDRC_INTRTXE, (0x1 << INTR_TXEP)|MGC_QREG_Read16(MGC_O_HDRC_INTRTXE));
    MGC_QMU_Write16(MGC_O_HDRC_INTRRXE, (0x1 << INTR_RXEP)|MGC_QREG_Read16(MGC_O_HDRC_INTRRXE));


    SetAddress(1);

    usb_ep_enable(INTR_TXEP, USB_TX, USB_INTR, 16, 0, 1);
    usb_ep_enable(INTR_RXEP, USB_RX, USB_INTR, 16, 0, 2);
    usb_ep_enable(BULK_TXEP, USB_TX, USB_BULK, 512, 0, 3);
    usb_ep_enable(BULK_RXEP, USB_TX, USB_BULK, 512, 0, 4);

    usb_config_req(INTR_TXEP, USB_TX, 8, FALSE, 1, 3, 0, 0);
    usb_config_req(INTR_RXEP, USB_RX, 8, FALSE, 1, 3, 0, 0);
    usb_config_req(BULK_TXEP, USB_TX, 65535, TRUE, 1, 3, 0, 0);
    usb_config_req(BULK_RXEP, USB_TX, 65535, TRUE, 2, 3, 0, 0);

    req1 = usb_get_req(INTR_TXEP, USB_TX);
    req2 = usb_get_req(INTR_RXEP, USB_RX);
    req3 = usb_get_req(BULK_TXEP, USB_TX);
    req4 = usb_get_req(BULK_RXEP, USB_TX);

    QMU_prepareTxBuf(req3->buf, 65535, 512);
    QMU_prepareTxBuf(req4->buf, 65535, 512);

    req3->count=data_count++;
    req3->complete=FALSE;    
    usb_insert_transfer_QMU(BULK_TXEP, 3, USB_TX, req3->buf, req3->count, FALSE);    

    for(i=1; i<100; i++)
    {
        gpd_tmp = Tx_gpd_end[BULK_TXEP];
        req3->count=data_count++;            
        usb_insert_transfer_QMU(BULK_TXEP, 3, USB_TX, req3->buf, req3->count, TRUE);
    }    

    gpd = Tx_gpd_end[BULK_TXEP];
    Tx_gpd_end[BULK_TXEP] = Tx_gpd_last[BULK_TXEP];
    //x_free_aligned_mem(gpd);
    gpd = NULL;

    TGPD_SET_NEXT(gpd_tmp, Tx_gpd_last[BULK_TXEP]);
    TGPD_SET_CHKSUM(gpd_tmp , 16);    
    Printf("Tx_gpd_last[3]: 0x%x!!\r\n", (unsigned int)Tx_gpd_last[BULK_TXEP]);
    
    #if 0
    gpd = Tx_gpd_end[3];    
    for(i=1; i<105;i++)
    {
        Printf("Tx 3 [%d] : %x, %x %x %x %x, GPD %x\r\n", i, gpd, 
            *(((UINT32 *)(gpd))+0), 
            *(((UINT32 *)(gpd))+1), 
            *(((UINT32 *)(gpd))+2), 
            *(((UINT32 *)(gpd))+3), 
            TGPD_GET_CHKSUM(gpd));
        gpd = TGPD_GET_NEXT(gpd);
    }
    #endif
    
    req4->count=data_count++;
    req4->complete=FALSE;
    usb_insert_transfer_QMU(BULK_RXEP, 4, USB_TX, req4->buf, req4->count, FALSE);    
    for(i=1; i<100; i++){
        gpd_tmp = Tx_gpd_end[BULK_RXEP];
        
        req4->count = data_count++;            
        usb_insert_transfer_QMU(BULK_RXEP, 4, USB_TX, req4->buf, req4->count, TRUE);
    }
    gpd = Tx_gpd_end[BULK_RXEP];
    Tx_gpd_end[BULK_RXEP] = Tx_gpd_last[BULK_RXEP];
    //x_free_aligned_mem(gpd);    
    TGPD_SET_NEXT(gpd_tmp, Tx_gpd_last[BULK_RXEP]);
    TGPD_SET_CHKSUM(gpd_tmp, 16);    
    Printf("Tx_gpd_last[4]: 0x%x!!\r\n", (unsigned int)Tx_gpd_last[BULK_RXEP]);    

    #if 0
    gpd = Tx_gpd_end[4];    
    for(i=1; i<105;i++)
    {
        Printf("Tx 4 [%d] : %x, %x %x %x %x, GPD %x\r\n", i, gpd, 
            *(((UINT32 *)(gpd))+0), 
            *(((UINT32 *)(gpd))+1), 
            *(((UINT32 *)(gpd))+2), 
            *(((UINT32 *)(gpd))+3), 
            TGPD_GET_CHKSUM(gpd));
        gpd = TGPD_GET_NEXT(gpd);
    }
    #endif
    
    TGPD_SET_FLAGS_HWO(Tx_gpd_last[BULK_TXEP]);
    usb_start_QMU(BULK_TXEP, USB_TX, FALSE);
    TGPD_SET_FLAGS_HWO(Tx_gpd_last[BULK_RXEP]);
    usb_start_QMU(BULK_RXEP, USB_TX, FALSE);

    USB_hal_delay_ms(10);
    while(1){
        rand = USB_get_random();

        if(rand % 10 == 1){            
            if(bStart1){
                bStop = TRUE;
                usb_stop_QMU(BULK_TXEP, USB_TX);
                Printf("stop 3\r\n");

                USB_hal_delay_ms(50);
                req1->buf[0] = 0xf3;
                req1->complete = FALSE;
                req1->actual = 0;
                req2->complete = FALSE;
                req2->actual = 0;
                req3->complete = FALSE;
                req3->actual = 0;
                wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
                wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
                MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (wIntrTxValue));
                MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (wIntrRxValue));
                bStop = FALSE;
                usb_start_transfer(INTR_TXEP, USB_TX);
                usb_start_transfer(INTR_RXEP, USB_RX);
                Printf("wait 3 done\r\n");

                while(!checkComplete(INTR_TXEP, USB_TX));
                while(!checkComplete(INTR_RXEP, USB_RX));                
                Flush_QMU(BULK_TXEP, USB_TX);
                Printf("stop 3 done\r\n");
                USB_hal_delay_ms(300);
                bStart1 = FALSE;
            }
            else{
                Printf("start 3\r\n");
                req1->buf[0] = 0x3;
                req1->actual = 0;
                req1->complete = FALSE;
                req2->actual = 0;
                req2->complete = FALSE;
                wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
                wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
                MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (wIntrTxValue));
                MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (wIntrRxValue));
                usb_start_transfer(INTR_TXEP, USB_TX);
                usb_start_transfer(INTR_RXEP, USB_RX);

                while(!checkComplete(INTR_TXEP, USB_TX));
                while(!checkComplete(INTR_RXEP, USB_RX));
                usb_restart_QMU(BULK_TXEP, USB_TX);
                Printf("start 3 done\r\n");
                bStart1 = TRUE;    
                USB_hal_delay_ms(30);
            }
        }

        else if(rand % 10 == 2){
            if(bStart2){
                bStop = TRUE;
                usb_stop_QMU(BULK_RXEP, USB_TX);
                Printf("stop 4\r\n");

                USB_hal_delay_ms(50);
                req1->buf[0] = 0xf4;
                req1->actual = 0;
                req1->complete = FALSE;                
                req2->actual = 0;
                req2->complete = FALSE;
                req4->complete = FALSE;
                req4->actual = 0;                
                wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
                wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
                MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (wIntrTxValue));
                MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (wIntrRxValue));
                bStop = FALSE;
                usb_start_transfer(INTR_TXEP, USB_TX);
                usb_start_transfer(INTR_RXEP, USB_RX);
                Printf("wait 4 done\r\n");

                while(!checkComplete(INTR_TXEP, USB_TX));
                while(!checkComplete(INTR_RXEP, USB_RX));
                Flush_QMU(BULK_RXEP, USB_TX);
                Printf("stop 4 done\r\n");
                USB_hal_delay_ms(300);
                bStart2 = FALSE;
            }
            else{
                Printf("start 4\r\n");
                req1->buf[0] = 0x4;
                req1->actual = 0;
                req1->complete = FALSE;
                req2->actual = 0;
                req2->complete = FALSE;
                wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
                wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
                MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (wIntrTxValue));
                MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (wIntrRxValue));
                usb_start_transfer(INTR_TXEP, USB_TX);
                usb_start_transfer(INTR_RXEP, USB_RX);

                while(!checkComplete(INTR_TXEP, USB_TX));
                while(!checkComplete(INTR_RXEP, USB_RX));
                usb_restart_QMU(BULK_RXEP, USB_TX);
                Printf("start 4 done\r\n");
                bStart2 = TRUE;    
                USB_hal_delay_ms(30);
            }
        }

        if(checkComplete(BULK_TXEP, USB_TX)){
            req3->complete = FALSE;
            req3->actual = 0;                 

            Printf("check complete 3\r\n");

            #if 0
            gpd = Tx_gpd_end[3];            
            for(i=1; i<105;i++)
            {
                Printf("Tx 3 [%d] : %x, %x %x %x %x, GPD %x\r\n", i, gpd, 
                    *(((UINT32 *)(gpd))+0), 
                    *(((UINT32 *)(gpd))+1), 
                    *(((UINT32 *)(gpd))+2), 
                    *(((UINT32 *)(gpd))+3), 
                    TGPD_GET_CHKSUM(gpd));
                gpd = TGPD_GET_NEXT(gpd);
            }
            #endif
            
            gpd = Tx_gpd_last[BULK_TXEP];
            do {
                TGPD_SET_FLAGS_HWO(gpd);
                gpd = TGPD_GET_NEXT(gpd);
            } while (gpd != Tx_gpd_last[BULK_TXEP]);            
        }


        if(checkComplete(BULK_RXEP, USB_TX)){
            req4->complete = FALSE;
            req4->actual = 0;                 

            Printf("check complete 4\r\n");

            #if 0
            gpd = Tx_gpd_end[4];            
            for(i=1; i<105;i++)
            {
                Printf("Tx 4 [%d] : %x, %x %x %x %x, GPD %x\r\n", i, gpd, 
                    *(((UINT32 *)(gpd))+0), 
                    *(((UINT32 *)(gpd))+1), 
                    *(((UINT32 *)(gpd))+2), 
                    *(((UINT32 *)(gpd))+3), 
                    TGPD_GET_CHKSUM(gpd));
                gpd = TGPD_GET_NEXT(gpd);
            }
            #endif
            
            gpd = Tx_gpd_last[BULK_RXEP];
            do {
                TGPD_SET_FLAGS_HWO(gpd);
                gpd = TGPD_GET_NEXT(gpd);
            } while (gpd != Tx_gpd_last[BULK_RXEP]);
        }


        if(bStart1)
        {
            usb_resume_QMU(BULK_TXEP, USB_TX);
        }         
        if(bStart2)
        {
            usb_resume_QMU(BULK_RXEP, USB_TX);
        }
    }    

   // isFreeGPD = TRUE;
}


#define MGC_O_DMA_CONTROL 4
#define MGC_O_DMA_ADDRESS 8
#define MGC_O_DMA_COUNT   0xc
#define MGC_DMA_CHANNEL_OFFSET(_bChannel, _bOffset) (MGC_O_HSDMA_BASE + (_bChannel << 4) + _bOffset)
#define FIFOTEST_MAX_LENGTH (64)
int TestRiscDramFifoAccess(int i4Argc, const char **szArgv)
{

    static unsigned int u4Count=0;
    static const unsigned int ep_num = 0;
    static const unsigned char bEpNum=0;

    unsigned char i=0;
    unsigned char *pSource = NULL;
    unsigned char *pDestination = NULL;
    unsigned int j = 0;
    unsigned char fgIsUseDma = 0;
    //USB_DMA_REQ rDmaReq;
    //USB_DMA_REQ *dma_req = &rDmaReq;
    unsigned int u4UsbVector = VECTOR_USB;
    unsigned int u4DmaChannela = 0;
    unsigned int u4DmaChannelb = 2;
    //unsigned char index = 0;
    //unsigned char intr = 0;
    uint32_t   rxcount = 0, addr = 0;

    //dma_req->channel = 0;
    //dma_req->burstmode = 3;

    if (i4Argc < 1)
    {
        Printf("Arg: [Use DMA]\n");
        Printf("[Use DMA]: 0 for disable; 1 for enable DMA.\n");
        return 0;
    }

    pSource = MUSB_MemAlloc(64);//, 32);
    pDestination = MUSB_MemAlloc(64);//, 32);

    if(!pSource || !pDestination)
    {
        Printf("Alloc Mem Fail.\n");
    }

    Printf("pSource     : 0x%08X\n", (unsigned int)pSource);
    Printf("pDestination: 0x%08X\n", (unsigned int)pDestination);    

    fgIsUseDma = (unsigned char) StrToInt(szArgv[1]);

    if(fgIsUseDma)
    {
        Printf("DMA Mode\n");
    }
    else
    {
        Printf("PIO Mode\n");
    }
    

#if 0
    MGC_QMU_Write8(MGC_O_HDRC_TXFIFOSZ, 0x3); //64Bytes
    MGC_QMU_Write8(MGC_O_HDRC_TXFIFOADD, 0);

    MGC_QMU_Write8(MGC_O_HDRC_RXFIFOSZ, 0x3); //64Bytes
    MGC_QMU_Write8(MGC_O_HDRC_RXFIFOADD, 0);
    MGC_WriteCsr16(MGC_O_HDRC_TXMAXP, ep_num, u4Count);        
    MGC_WriteCsr16(MGC_O_HDRC_RXMAXP, ep_num, u4Count);        
#endif

  	resetUSB();

    //mdelay(5);
    
    //turn on ep0 interrupt
    //MGC_QMU_Write16 (MGC_O_HDRC_INTRTXE, MGC_QREG_Read16(MGC_O_HDRC_INTRTXE) | 0x1);

    while(1)
    {        
        //j++;
		u4Count++;

		if(u4Count > FIFOTEST_MAX_LENGTH)
		{
		  u4Count = 1;
        j++;
		}

        for(i = 0; i<u4Count ; i++)
        {
            *(unsigned char *)((unsigned int)pSource+i) = 0xa5;
            *(unsigned char *)((unsigned int)pDestination+i) = 0x0;
        }
    	
        //enableUsbIntr();
        // disable USB interrupt.   
        BIM_DisableIrq(u4UsbVector); 

		//MGC_WriteCsr16(MGC_O_HDRC_CSR0, ep_num, MGC_M_CSR0_FLUSHFIFO);
		//MGC_WriteCsr16(MGC_O_HDRC_CSR0, ep_num, MGC_M_CSR0_FLUSHFIFO);

        //index = MGC_QREG_Read8(MGC_O_HDRC_INDEX);
        MGC_QMU_Write8(MGC_O_HDRC_INDEX, ep_num);

        if(fgIsUseDma)
        {
            //DMA for TX

            //Disable Interrupt.
            //Printf("Todo: Disable Interrupt.\n");
            //remove mask.
            //Printf("Todo: remove mask.\n");
            //Printf("DMA for TX\n");

            MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, ep_num) & (~MGC_M_TXCSR_DMAENAB));
            // set dma address.
            MGC_QREG_Write32(MGC_DMA_CHANNEL_OFFSET(u4DmaChannela, MGC_O_DMA_ADDRESS), (uint32_t)pSource);
            // set data size.
            MGC_QREG_Write32(MGC_DMA_CHANNEL_OFFSET(u4DmaChannela, MGC_O_DMA_COUNT), u4Count);            
            // trigger dma
            //MGC_QREG_Write32(MGC_DMA_CHANNEL_OFFSET(u4DmaChannela, MGC_O_DMA_CONTROL), (0xB |(ep_num << 4)));
            MGC_QREG_Write32(MGC_DMA_CHANNEL_OFFSET(u4DmaChannela, MGC_O_DMA_CONTROL), (0x60B |(ep_num << 4)));                


            //Printf("Waiting for DMA INTR\n");
                        
            // wait irq.    
            while (!MGC_QREG_Read8(MGC_O_HSDMA_INTR));    

            //Printf("Clean IRQ\n");

            MGC_QMU_Write8(MGC_O_HSDMA_INTR, MGC_QREG_Read8(MGC_O_HSDMA_INTR));  

            BIM_ClearIrq(u4UsbVector);

            addr = (0x200+((8+u4DmaChannela)<<4));
            rxcount = MGC_QREG_Read32(addr); //DMA_REALCNT_0

            if(j%0x100 == 0)
                Printf("[0x%04X] TX: %d Bytes\n", addr, rxcount);

            //Printf("Clean IRQ OK [0x%08X]\n", MGC_QREG_Read8(MGC_O_HSDMA_INTR));
        }
        else
        {
            if(j%0x100 == 0)
            Printf("[FIFO] TX: %d Bytes\n",u4Count);
			
            QMU_HdrcLoadFifo(bEpNum, u4Count, pSource);
        }

        //mdelay(1);

        MGC_QWriteCsr16(MGC_O_HDRC_CSR0, bEpNum, MGC_M_CSR0_TXPKTRDY);

        //mdelay(1);

        MGC_QMU_Write16(MGC_O_HDRC_TESTMODE, MGC_M_TEST_FIFO_ACCESS);

        //mdelay(1);

        //rxcount = MGC_QREG_Read8(MGC_O_HDRC_COUNT0);

        if(fgIsUseDma)
        {
            //DMA for RX
            //Disable Interrupt.
            //Printf("Todo: Disable Interrupt.\n");
            //remove mask.
            //mdelay(1);
            
            //Printf("DMA for RX\n");

            MGC_QREG_Write32(MGC_DMA_CHANNEL_OFFSET(u4DmaChannelb, MGC_O_DMA_ADDRESS), (uint32_t)pDestination);

            // set data size.   //2010.05.18
            MGC_QREG_Write32(MGC_DMA_CHANNEL_OFFSET(u4DmaChannelb, MGC_O_DMA_COUNT), u4Count);
            // trigger dma    //2010.05.18
            MGC_QREG_Write32(MGC_DMA_CHANNEL_OFFSET(u4DmaChannelb, MGC_O_DMA_CONTROL), (0x609 |(ep_num << 4)));
            //MGC_QREG_Write32(MGC_DMA_CHANNEL_OFFSET(2, MGC_O_DMA_CONTROL), (0x9 |(ep_num << 4)));

            //Printf("MGC_O_HSDMA_INTR [0x%08X]\n", MGC_QREG_Read8(MGC_O_HSDMA_INTR));

            // wait irq.    
            while (!MGC_QREG_Read8(MGC_O_HSDMA_INTR));    

            //Printf("Clean IRQ\n");

            MGC_QMU_Write8(MGC_O_HSDMA_INTR, MGC_QREG_Read8(MGC_O_HSDMA_INTR));  

            BIM_ClearIrq(u4UsbVector);

            addr = (0x200+((8+u4DmaChannelb)<<4));
            rxcount = MGC_QREG_Read32(addr); //DMA_REALCNT_2

            if(j%0x1000 == 0)
                Printf("[0x%04X] RX: %d Bytes\n", addr, rxcount);

            //Printf("Clean IRQ OK [0x%08X]\n", MGC_QREG_Read8(MGC_O_HSDMA_INTR));
        }
        else
        {
            if(j%0x100 == 0)
            Printf("[FIFO] RX: %d Bytes\n",u4Count);
		
            QMU_HdrcUnloadFifo(bEpNum, u4Count, pDestination);
        }

        //mdelay(1);

        MGC_QWriteCsr16(MGC_O_HDRC_CSR0, bEpNum, MGC_M_CSR0_P_SVDRXPKTRDY);    
        
        //mdelay(1);

        for(i=0; i<u4Count; i++)
        {
            if(pSource[i] != pDestination[i])
            {
                Printf("[%d]*** FIFO ACCESS Printf!\n", j);
                Printf("Pos:%d, Src(0x%02X); Des(0x%02X)\n", i, pSource[i], pDestination[i]);
            }
        }

        if(j%0x100 == 0 && u4Count == FIFOTEST_MAX_LENGTH)
            Printf("[0x%08X]FIFO access Done---OK!\n", j);

    }
    
    //x_free_aligned_mem((void *)pSource);
    //x_free_aligned_mem((void *)pDestination);

//    return TRUE;
    
}

int TestQMUIsoLoopback(int i4Argc, const char **szArgv){
    unsigned int i,j;
    //unsigned char keepGoing = TRUE;
    //unsigned int userDecision;
    //unsigned int randomValue;
    USB_REQ *req1, *req2;
	unsigned char* buf1;
	unsigned char* buf2;
    unsigned int sum=0;
    unsigned int buflen[10]={511,512,1023,1024,1030,2040,2048,2050,3070,3072};
      
    #if 0 //!PORTING
    while(DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision, 0, (1)));
    if(userDecision == 0){
        is_bdp = TRUE;
    }else{
	 is_bdp = FALSE;
    }
    #else
    is_bdp = 0;
    #endif    
    //psOutput->eResult = DVT_RESULT_PASS;
    bError = FALSE;
    initialQMUTestcase(TRUE);    
    if(isPass == -1){
        endQMUTestcase();	 
        return DVT_FAIL;
    }
    //set address    
    SetAddress(0x1);
    resetFIFO();    
    usb_ep_enable(1, USB_TX, USB_ISO, 3072, 0, 1);
    resetFIFO();    
    usb_ep_enable(2, USB_RX, USB_ISO, 3072, 0, 2);
    //usb_config_req(1, USB_TX, 1023, TRUE, 1, 3, 0, 0);
    //usb_config_req(2, USB_RX, 1023, TRUE, 2, 3, 0, 0);
    
    //while (keepGoing && !bError){   
    usb_start_QMU(1, USB_TX, 0);
    usb_start_QMU(2, USB_RX, 0);
    MGC_WriteQMU32(MGC_O_QMU_QCR2, 0);
    MGC_WriteQMU32(MGC_O_QMU_QCR3, 0);
    req1 = usb_get_req(1, USB_TX);  
    req2 = usb_get_req(2, USB_RX);
    usb_config_req(1, USB_TX, USB_BUF_SIZE, TRUE, 1, 3, 0, 0);
    usb_config_req(2, USB_RX, USB_BUF_SIZE, TRUE, 2, 3, 0, 0);     
    randomTxBuf(1); 
    clearRxBuf(2);

    buf1=req1->buf;
    buf2=req2->buf;

    for(j=0;j<10; j++)
    {
        usb_insert_transfer_QMU(1, 1, USB_TX, buf1, buflen[j],TRUE); 
        //usb_insert_transfer_QMU(2, 2, USB_RX, buf2, buflen[j], TRUE); 
        //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
        //Printf("Rx_gpd_end[2]:%x\r\n", Rx_gpd_end[2]);
        buf1+=buflen[j];
        //buf2+=buflen[j];
        sum+=buflen[j];
    }    
    //TGPD* nextgpd=TGPD_GET_NEXT(Rx_gpd_last[2]);
    
    usb_resume_QMU(1, USB_TX);    
    while(!checkComplete(1, USB_TX));
    //USB_hal_delay_ms(3000000);
    USB_hal_delay_ms(30000);

    //Prepare RX QMU
    for(j=0;j<10; j++)
    {                      
        //usb_insert_transfer_QMU(1, 1, USB_TX, buf1, buflen[j],TRUE); 
        usb_insert_transfer_QMU(2, 2, USB_RX, buf2, 3072, TRUE); 
        //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
        // Printf("Rx_gpd_end[2]:%x\r\n", Rx_gpd_end[2]);
        //buf1+=buflen[j];
        buf2+=buflen[j];        
    }    
    usb_resume_QMU(2, USB_RX);
    while(!checkComplete(2, USB_RX));

        //Printf("3:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
    for(i=0; i<sum; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
    }     
    Printf("QMU iso loopback success\r\n");
        
    endQMUTestcase();
    #if 0
     if(bError){
	psOutput->eResult = DVT_RESULT_FAIL;
    }else{
	psOutput->eType = DVT_OUTPUT_RESULT; 
    }  
    #endif
    return DVT_SUCCESS;
}

void resetDevFIFO()
{
    g_FIFOadd = USB_FIFO_START_ADDRESS;
}

void usb_dev_ep_enable(int ep_num, USB_DIR dir, USB_TRANSFER_TYPE type, int maxp, int dpb)
{
    //int index;
    int ep_index = 0;
    int used_before;
    unsigned char fifosz;
    //int maxpreg;
	unsigned int multiPacket;
	unsigned short wMaxP;
    USB_EP_SETTING *ep_setting;
    //index = MGC_QREG_Read8(MGC_O_HDRC_INDEX);
    MGC_QMU_Write8(MGC_O_HDRC_INDEX, ep_num);

    if(dir == USB_TX)
    {
        ep_index = ep_num;
    }
    else if(dir == USB_RX)
    {
        ep_index = ep_num + TX_FIFO_NUM - 1;
    }
    else
    {
        Printf("Printf, dir invalid");
    }
    
    ep_setting = &g_usb_setting.ep_setting[ep_index];
    used_before = ep_setting->fifoaddr;

    if((g_FIFOadd + maxp * (dpb + 1) > USB_RAM_SIZE) && (!used_before))
    {
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
        //set index
        MGC_QMU_Write8(MGC_O_HDRC_INDEX, ep_num);
        //set txFIFOsz
        MGC_QMU_Write8 (MGC_O_HDRC_TXFIFOSZ, fifosz);
        //set txFIFOadd
        MGC_QMU_Write16 (MGC_O_HDRC_TXFIFOADD, g_FIFOadd/8);
        //set txType, interval
        //set TxMaxP
        multiPacket = 1;
		wMaxP = maxp;
		if(maxp > 1024){
			wMaxP = 1024;
			multiPacket = maxp/1024;
		}
		wMaxP = ((multiPacket-1) << 11) | wMaxP;
        MGC_QWriteCsr16(MGC_O_HDRC_TXMAXP, ep_num, wMaxP);
		if(type == USB_ISO){
			MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR,ep_num) | MGC_M_TXCSR_ISO|0x0200);
		}
		else{
			MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR,ep_num) & ~MGC_M_TXCSR_ISO);
		}
        //flush TxFIFO
        MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR,ep_num) | MGC_M_TXCSR_FLUSHFIFO);
        MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR,ep_num) | MGC_M_TXCSR_FLUSHFIFO);
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
        //set RxMaxP
        multiPacket = 1;
		wMaxP = maxp;
		if(maxp > 1024){
			wMaxP = 1024;
			multiPacket = maxp/1024;
		}
		wMaxP = ((multiPacket-1) << 11) | wMaxP;
        MGC_QWriteCsr16(MGC_O_HDRC_RXMAXP, ep_num, wMaxP);
		if(type == USB_ISO){
			MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR,ep_num) | MGC_M_RXCSR_P_ISO|0x0200);
		}
		else{
			MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR,ep_num) & ~MGC_M_RXCSR_P_ISO);
		}
        //flush RxFIFO
        MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR,ep_num) | MGC_M_RXCSR_FLUSHFIFO);
        MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR,ep_num) | MGC_M_RXCSR_FLUSHFIFO);
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

int TestQMUDevIsoLoopback(int i4Argc, const char **szArgv)
{
#if 1
    unsigned int i,j;
    //unsigned int j;
    //unsigned char keepGoing = TRUE;
    //unsigned int userDecision;
    //unsigned int randomValue;
    USB_REQ *req1;//, *req2;
    unsigned int sum=0;
    unsigned int buflen[10]={0};
    //unsigned char buf[10][3072];
    unsigned char* buf[10];
    unsigned char* buf1;
//    unsigned char* buf2;

    for(i = 0; i<10; i++)
    {
        buf[i] = (unsigned char *)MUSB_MemAlloc(3072);//, 32);
    }
    		
    #if 0 //!PORTING make BDP = 1
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf("- Select QMU loopback test                                     -\r\n"); 
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf(" (0) BD (1) Data Buffer \r\n");
    Printf("\r\n");
    while(DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision, 0, (1)));
    if(userDecision == 0){
        is_bdp = TRUE;
    }else{
	 is_bdp = FALSE;
    }    
    #else
    is_bdp = TRUE;
    #endif
    
    //psOutput->eResult = DVT_RESULT_PASS;
    bError = FALSE;
    initialDevQMUTestcase(TRUE);    
    if(isPass == -1){
        endQMUTestcase();	 
        return DVT_FAIL;
    }
    //set address    
    MGC_QMU_Write8(MGC_O_HDRC_FADDR, 0x1);
    resetDevFIFO();  
    usb_dev_ep_enable(1, USB_RX, USB_ISO, 3072, 0);
    resetDevFIFO();    
    usb_dev_ep_enable(2, USB_TX, USB_ISO, 3072, 0);
          
    //while (keepGoing && !bError){   
    usb_start_QMU(1, USB_RX, 0);
    
    MGC_WriteQMU32(MGC_O_QMU_QCR2, 0);
    MGC_WriteQMU32(MGC_O_QMU_QCR3, 0);
    req1 = usb_get_req(1, USB_RX);  
    //req2 = usb_get_req(2, USB_TX);
    usb_config_req(1, USB_RX, USB_BUF_SIZE, TRUE, 1, 3, 0, 0);
    usb_config_req(2, USB_TX, USB_BUF_SIZE, TRUE, 2, 3, 0, 0);     
    //randomTxBuf(1); 
    //clearRxBuf(2);
    buf1=req1->buf;
    //buf2=req1->buf;

    for(j=0;j<10; j++){                      
        usb_insert_transfer_QMU(1, 1, USB_RX, buf[j], 3072,TRUE); 
        //usb_insert_transfer_QMU(2, 2, USB_TX, buf2, buflen[j], TRUE); 
        Printf("Rx_gpd_end[1]:%x\r\n", (unsigned int)Rx_gpd_end[1]);
        buf1+=buflen[j];
        //buf2+=buflen[j];
        sum+=buflen[j];
    }    

    usb_resume_QMU(1, USB_RX);    
    while(!checkComplete(1, USB_RX));
    TGPD_SET_FLAGS_HWO(Tx_gpd_last[2]);
    usb_start_QMU(2, USB_TX, 0);
    //usb_resume_QMU(2, USB_TX);    
    while(!checkComplete(2, USB_TX));

    Printf("QMU iso loopback success\r\n");
        
    endQMUTestcase();
    #if 0
    if(bError){
	psOutput->eResult = DVT_RESULT_FAIL;
    }else{
	psOutput->eType = DVT_OUTPUT_RESULT; 
    }  
    #endif
#endif
    return DVT_SUCCESS;
}

void initialTestcase(unsigned char isHS){
	unsigned char state;
	void (*pfnOldIsr)(unsigned short);
	
	bError = FALSE;
	isReset = 0;
	isPass = 0;

	resetUSB();
	
	//disable and flush interrupt
	disableIntr();
	//Attach USB CISR
	#if 0 //!PORTING
	CISR_Attach(hdrc_cisr, USB_HW_IRQ_NO);   
	#else
	if(OSR_OK != x_reg_isr((unsigned short)USB_IRQ_NO, HdrcBsr, &pfnOldIsr))
	{
		Printf("Roll: Can't register IRQ %d\n", USB_IRQ_NO);
		return;
	}
	else
	{
		Printf("Register IRQ %d\n", USB_IRQ_NO);
	}

	Printf("USB Disable IRQ: %d\n", USB_IRQ_NO);
	BIM_DisableIrq(USB_IRQ_NO);	
	#endif

	//Start Session
	MGC_QMU_Write8(MGC_O_HDRC_DEVCTL, MGC_M_DEVCTL_SESSION);
	if(isHS){
		MGC_QMU_Write8(MGC_O_HDRC_POWER, (MGC_QREG_Read8(MGC_O_HDRC_POWER) | (MGC_M_POWER_HSENAB)));
	}
	else{
		MGC_QMU_Write8(MGC_O_HDRC_POWER, (MGC_QREG_Read8(MGC_O_HDRC_POWER) & (~MGC_M_POWER_HSENAB)));
	}
	
	//Enable ConnIntr
	state = MGC_M_INTR_CONNECT | MGC_M_INTR_BABBLE | MGC_M_INTR_VBUSERROR | MGC_M_INTR_DISCONNECT;
	MGC_QMU_Write8(MGC_O_HDRC_INTRUSBE, state);
	enableUsbIntr();

	i=0;
	usecs = 15;
	Printf("Test out transaction please attach device in (%d) secs...\r\n", usecs);
	while(i<usecs && isReset == 0){
		USB_hal_delay_ms(1000);
		i++;
	}
	if(i >= usecs){
		Printf("Failed: no device attached after timeout (%d) secs\r\n", usecs);
		isPass = -1;
	}
	
	Printf("Device attached and reset done\r\n");
}


unsigned short oldwIntrTxEValue ;
unsigned short oldwIntrRxEValue ;

void initialQMUTestcase(unsigned char isHS){    
    bError = FALSE;
    isReset = 0;
    isPass = 0;
    #if 1//def Leon
    resetUSB();
    #endif
    initialQMU(isHS);
    i=0;
    usecs = 15;
    Printf("Test out transaction please attach device in (%d) secs...\r\n", usecs);
    while(i<usecs && isReset == 0){
        USB_hal_delay_ms(1000);
        i++;
    }
    if(i >= usecs){
        Printf("Failed: no device attached after timeout (%d) secs\r\n", usecs);
        isPass = -1;
    }
    Printf("Device attached and reset done\r\n");
}

void *UsbIrqFoo = NULL;
void initialQMU(unsigned char isHS){    
    //register isr and bsr
    unsigned int QCR = 0;
    //unsigned long irqflags = IRQF_SHARED;
    void (*pfnOldIsr)(unsigned short);

    #if 0 //!PORTING    
    LIST_Init(&(hqmu.BsrList));
    LIST_Init(&(hqmu.BsrPool));
    for (i=0; i<NUM_BSR_ITEMS; i++) {
        QMUBsrItemPool[i].link.prev=  &QMUBsrItemPool[i].link;
        QMUBsrItemPool[i].link.next=  &QMUBsrItemPool[i].link;
        LIST_Add(&(QMUBsrItemPool[i].link), &(hqmu.BsrPool));
    }
    //Init CISR
    CISR_Init(hqmu_cisr, HdrcIsrQmu, 1, &hqmu);
    CISR_SetDSR(hqmu_cisr, HdrcBsrQmu);
    #endif
#if 0
    LIST_Init(&(hdrc.BsrList));
    LIST_Init(&(hdrc.BsrPool));
    for (i=0; i<NUM_BSR_ITEMS; i++) {
        BsrItemPool[i].link.prev=  &BsrItemPool[i].link;
        BsrItemPool[i].link.next=  &BsrItemPool[i].link;
        LIST_Add(&(BsrItemPool[i].link), &(hdrc.BsrPool));
    }
    //Init CISR
    CISR_Init(hdrc_cisr, HdrcIsr, 1, &hdrc);
    CISR_SetDSR(hdrc_cisr, HdrcBsr);
#endif	
    //disable and flush interrupt
    disableIntr();

    //Attach USB CISR
    #if 0 //!PORTING
    CISR_Attach(hqmu_cisr, USB_HW_IRQ_NO);  
    CISR_Attach(hqmu_cisr, QMU_HW_IRQ_NO);  
    #else
        #if 0
		if (irqflags & IRQF_SHARED)
			irqflags &= ~IRQF_DISABLED;
        #endif
#if 0 //ben
    if (request_irq(USB_IRQ_NO, &HdrcBsrQmu, irqflags ,"BsrQmu", UsbIrqFoo)) 
    {
          Printf(KERN_ERR "Roll: Can't register IRQ %d\n", USB_IRQ_NO);
          return;
    }
#else    
    if(OSR_OK != x_reg_isr((unsigned short)USB_IRQ_NO, HdrcBsrQmu, &pfnOldIsr))
    {
      Printf("Roll: Can't register IRQ %d\n", USB_IRQ_NO);
      return;
    }
    else
    {
      Printf("Register IRQ %d\n", USB_IRQ_NO);
    }

  //  Printf("USB Disable IRQ: %d\n", USB_IRQ_NO);
    BIM_DisableIrq(USB_IRQ_NO);
#endif
		

    #if 1//def Leon
    //Start Session
    MGC_QMU_Write8(MGC_O_HDRC_DEVCTL, MGC_M_DEVCTL_SESSION);
    if(isHS){
        MGC_QMU_Write8(MGC_O_HDRC_POWER, (MGC_QREG_Read8(MGC_O_HDRC_POWER) | (MGC_M_POWER_HSENAB)));//|MGC_M_POWER_ISOUPDATE));
    }
    else{
        MGC_QMU_Write8(MGC_O_HDRC_POWER, (((MGC_QREG_Read8(MGC_O_HDRC_POWER)) & (~MGC_M_POWER_HSENAB)) |MGC_M_POWER_ISOUPDATE));
    }
    //Enable ConnIntr
    MGC_QMU_Write8(MGC_O_HDRC_INTRUSBE, MGC_M_INTR_CONNECT);
    #endif
    #if 1// leon test
    for(i=1; i<5; i++){
	    Rx_gpd_end[i]= (TGPD*)MUSB_MemAlloc(sizeof(TGPD));//, 32);
	    Printf("ALLOC RX GPD End [%d] Mem@0x%08X\n", i, (unsigned int)Rx_gpd_end[i]);
	    MUSB_MemSet(Rx_gpd_end[i], 0 , sizeof(TGPD));
	    TGPD_CLR_FLAGS_HWO(Rx_gpd_end[i]);    

#if 0// mhzhang			
	    MGC_WriteQMU32(MGC_O_QMU_RQSAR(i), Rx_gpd_end[i]);
#else
	    MGC_WriteQMU32(MGC_O_QMU_RQSAR(i), (int)(Rx_gpd_end[i]));
#endif			
	    Rx_gpd_last[i]=Rx_gpd_end[i];
	    QCR|=USB_QMU_Rx_EN(i);
	    Tx_gpd_end[i]= (TGPD*)MUSB_MemAlloc(sizeof(TGPD));//, 32);
	    Printf("ALLOC TX GPD End [%d] Mem@0x%08X\n", i, (unsigned int)Tx_gpd_end[i]);
	    MUSB_MemSet(Tx_gpd_end[i], 0 , sizeof(TGPD)+AT_GPD_EXT_LEN);
	    TGPD_CLR_FLAGS_HWO(Tx_gpd_end[i]);    
#if 0 // mhzhang			
	    MGC_WriteQMU32(MGC_O_QMU_TQSAR(i), Tx_gpd_end[i]);
#else
	    MGC_WriteQMU32(MGC_O_QMU_TQSAR(i), (int)(Tx_gpd_end[i]));
#endif			
	    Tx_gpd_last[i]=Tx_gpd_end[i];
	    QCR|=USB_QMU_Tx_EN(i);
    }  
    MGC_WriteQUCS32(MGC_O_QUCS_USBGCSR,  QCR);
    MGC_WriteQIRQ32(MGC_O_QIRQ_QIMR, 0x00000000);

    oldwIntrTxEValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    oldwIntrRxEValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
    isQMUmode=TRUE;

    MGC_QMU_Write16(MGC_O_HDRC_INTRTXE, 1);
    MGC_QMU_Write16(MGC_O_HDRC_INTRRXE, 0);
    MGC_QMU_Write16(MGC_O_HSDMA_CONFIG, MGC_QREG_Read16(MGC_O_HSDMA_CONFIG)|0x30);    
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(4), 0x600);
#endif
#endif
    //ben
    enableUsbIntr();
    //~ben
}

void initialDevQMUTestcase(unsigned char isHS){    
    unsigned char state;
    int i;
//    int isPass = 0;
    unsigned int QCR=0;
   // unsigned long irqflags = IRQF_SHARED;		
    void (*pfnOldIsr)(unsigned short);
    bError = FALSE;
    isReset = 0;
		
    resetUSB();
    #if 0 //!PORTING    
    //register isr and bsr
    LIST_Init(&(hdrc.BsrList));
    LIST_Init(&(hdrc.BsrPool));
    for (i=0; i<NUM_BSR_ITEMS; i++) {
        BsrItemPool[i].link.prev=  &BsrItemPool[i].link;
        BsrItemPool[i].link.next=  &BsrItemPool[i].link;
        LIST_Add(&(BsrItemPool[i].link), &(hdrc.BsrPool)); 
    }
    //Init CISR
    CISR_Init(hdrc_cisr, HdrcIsrQmu, 1, &hdrc);
    CISR_SetDSR(hdrc_cisr, HdrcBsrDevQmu);
    #endif
    //disable and flush interrupt
    disableIntr();
    //Attach USB CISR
    #if 0 //!PORTING
    CISR_Attach(hdrc_cisr, USB_HW_IRQ_NO);   
    CISR_Attach(hdrc_cisr, QMU_HW_IRQ_NO);  
    #else

	//	if (irqflags & IRQF_SHARED)
	//		irqflags &= ~IRQF_DISABLED;

#if 0//ben
    if (request_irq(USB_IRQ_NO, &HdrcBsrDevQmu, irqflags ,"BsrDevQmu", UsbIrqFoo)) 
    {
      Printf(KERN_ERR "Roll: Can't register IRQ %d\n", USB_IRQ_NO);
      return;
    }
#else
    if(OSR_OK != x_reg_isr((unsigned short)USB_IRQ_NO, HdrcBsrDevQmu, &pfnOldIsr))
    {
      Printf("Can't register IRQ %d\n", USB_IRQ_NO);
      return;
    }
    else
    {
      Printf("Register IRQ %d\n", USB_IRQ_NO);
    }
    //(void *)pfnOldIsr;
#endif

    #endif

    
	/* software reset */
	
    /* enable interrupt */
    state = MGC_M_INTR_RESET | MGC_M_INTR_DISCONNECT | MGC_M_INTR_SUSPEND | MGC_M_INTR_RESUME;
	MGC_QMU_Write8(MGC_O_HDRC_INTRUSBE, state);
    /* usb connect */
    MGC_QMU_Write8(MGC_O_HDRC_POWER, MGC_QREG_Read8(MGC_O_HDRC_POWER) | MGC_M_POWER_SOFTCONN|MGC_M_POWER_ISOUPDATE);
    Printf("Waiting for connect\r\n");
    while(isReset == 0);

    for(i=1; i<5; i++){
	    Rx_gpd_end[i]= (TGPD*)MUSB_MemAlloc(sizeof(TGPD));//, 32);
	    MUSB_MemSet(Rx_gpd_end[i], 0 , sizeof(TGPD));
	    TGPD_CLR_FLAGS_HWO(Rx_gpd_end[i]);    
	    MGC_WriteQMU32(MGC_O_QMU_RQSAR(i), (unsigned int)Rx_gpd_end[i]);
	    Rx_gpd_last[i]=Rx_gpd_end[i];
	    QCR|=USB_QMU_Rx_EN(i);
	    Tx_gpd_end[i]= (TGPD*)MUSB_MemAlloc(sizeof(TGPD)+AT_GPD_EXT_LEN);//, 32);
	    MUSB_MemSet(Tx_gpd_end[i], 0 , sizeof(TGPD)+AT_GPD_EXT_LEN);
	    TGPD_CLR_FLAGS_HWO(Tx_gpd_end[i]);    
	    MGC_WriteQMU32(MGC_O_QMU_TQSAR(i), (unsigned int)Tx_gpd_end[i]);
	    Tx_gpd_last[i]=Tx_gpd_end[i];
	    QCR|=USB_QMU_Tx_EN(i);
    }  
    MGC_WriteQUCS32(MGC_O_QUCS_USBGCSR,  QCR);
    MGC_WriteQIRQ32(MGC_O_QIRQ_QIMR, 0x00000000);

    oldwIntrTxEValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    oldwIntrRxEValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
    isQMUmode=TRUE;

    MGC_QMU_Write16(MGC_O_HDRC_INTRTXE, 1);
    MGC_QMU_Write16(MGC_O_HDRC_INTRRXE, 0);
    MGC_QMU_Write16(MGC_O_HSDMA_CONFIG, MGC_QREG_Read16(MGC_O_HSDMA_CONFIG)|0x30);    
    MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(4), 0x600);    

    //ben
    enableUsbIntr();
    //~ben
}

#if 0 //!PORTING 
#else
void endTestcase(void){
   // free_irq(USB_IRQ_NO, NULL);
}
#endif

void endQMUTestcase(void)
{
    //unregister ISR
    for(i=1; i<5; i++)
    {
	    //x_free_aligned_mem(Rx_gpd_end[i]);
	    Rx_gpd_end[i]=NULL;
	    Rx_gpd_last[i]=NULL;
#if 0 // mhzhang			
	    MGC_WriteQMU32(MGC_O_QMU_RQSAR(i), Rx_gpd_end[i]);
#else
	    MGC_WriteQMU32(MGC_O_QMU_RQSAR(i), (int)(Rx_gpd_end[i]));
#endif

	    //x_free_aligned_mem(Tx_gpd_end[i]);
	    Tx_gpd_end[i]=NULL;
	    Tx_gpd_last[i]=NULL;
#if 0 // mhzhang			
	    MGC_WriteQMU32(MGC_O_QMU_TQSAR(i), Tx_gpd_end[i]);
#else
	    MGC_WriteQMU32(MGC_O_QMU_TQSAR(i), (int)(Tx_gpd_end[i]));			
#endif
    }  
    isQMUmode=FALSE;
    MGC_QMU_Write16(MGC_O_HDRC_INTRTX, MGC_QREG_Read16(MGC_O_HDRC_INTRTX));
    MGC_QMU_Write16(MGC_O_HDRC_INTRRX, MGC_QREG_Read16(MGC_O_HDRC_INTRRX));
    MGC_QMU_Write16(MGC_O_HDRC_INTRTXE, (oldwIntrTxEValue));
    MGC_QMU_Write16(MGC_O_HDRC_INTRRXE, (oldwIntrRxEValue));
    MGC_WriteQUCS32(MGC_O_QUCS_USBGCSR,  0);
    MGC_WriteQIRQ32(MGC_O_QIRQ_QIMR, 0xFFFFFFFF);

    #if 0 //!PORTING
    CISR_Detach(hqmu_cisr, QMU_HW_IRQ_NO);
    CISR_Detach(hqmu_cisr, USB_HW_IRQ_NO);
    CISR_Dest(hqmu_cisr);
    #else
   // free_irq(USB_IRQ_NO, NULL);
    #endif
}

#if 0
void vQmuDebugPrint(unsigned int line)
{
    unsigned int u4DebugReg = 0;
    MGC_WriteQDBGQ32(MGC_O_QMU_DEBUG_FLAG, 0x05056162);
    u4DebugReg = MGC_ReadQDBGQ32(MGC_O_QMU_DEBUG_MONITOR);
    Printf("[%d] RX0 QMU DBG: 0x%08X\n", line, u4DebugReg);

    MGC_WriteQDBGQ32(MGC_O_QMU_DEBUG_FLAG, 0x05056364);
    u4DebugReg = MGC_ReadQDBGQ32(MGC_O_QMU_DEBUG_MONITOR);
    Printf("[%d] RX1 QMU DBG: 0x%08X\n", line, u4DebugReg);
}
#endif

void vBdpDramBusy(unsigned char fgBool)
{   
    static unsigned char *pcDramBusyMemAddr = NULL;
    const unsigned int u4BufLength = 64*1024;
    unsigned int u4Reg = 0;
    
    if(fgBool)
    {
        if(!pcDramBusyMemAddr)
        {
            pcDramBusyMemAddr = (unsigned char*)MUSB_MemAlloc(u4BufLength);//, 32); 
        }

        if(pcDramBusyMemAddr)
        {
            Printf("Start DRAM Test Agent.\n");
            //High priority¡G
            //RISCWrite 0x007104 0x0xxxxxxx
        	u4Reg= Reg_Read32(IO_BASE+0x7104);
            u4Reg &= 0x0FFFFFFF;
            Reg_Write32((IO_BASE+0x7104), u4Reg);
            
            //007210 (PHY Addr)
            Reg_Write32((IO_BASE+0x7210), (unsigned int)pcDramBusyMemAddr);
            
            //007214 (Length, must be greater than 0x1000)
            Reg_Write32((IO_BASE+0x7214), u4BufLength);           
            
            //007218 0x840F110D (once)
            //       0x860F110D (repeat)
            Reg_Write32((IO_BASE+0x7218), 0x860F110D);           
        }
    }
    else
    {
        //Disable
        //007218 0x060F110D (once)
    	u4Reg= Reg_Read32(IO_BASE+0x7218);
        u4Reg &= 0x0FFFFFFF;
        Reg_Write32((IO_BASE+0x7218), u4Reg);        
    }
}

#if 0 //!PORTING
DVT_STATUS_T TestMultiIxiaLoopbackQMU(DVT_INPUT_DATA_T *psInput, DVT_OUTPUT_DATA_T *psOutput){
#else
int TestMultiIxiaLoopbackQMU(int i4Argc, const char **szArgv){
#endif
    void (*pfnOldIsr)(unsigned short);
    int nEnumStep = 0;
    int isPass = 0;
    //int usecs = 30;
    //int i = 0, j=0;;
    int i;
    //int i4EpNum = 1;
    int wAddress = 5;
    int assignedAddress = 0;
    int count = 0;
    #if 0 //!PORTING
    int devNum = 3;
    #else
    int devNum = 2;
    #endif
    USB_EP0_DATASTAGE data_stage =USB_EP0_DATAIN;
    MUSB_DeviceRequest devReq;
    USB_EP0_REQ *req, *req1;

  //  TGPD* gpd=NULL;
    TGPD* rx_gpd=NULL;
    TGPD* tx_gpd=NULL;

    bError = FALSE;
    dwEP0MaxP = 8;
    resetUSB();

    //initialQMUTestcase();

    vBdpDramBusy(TRUE);

    initialTestcase(TRUE);
    if(isPass == -1){
        endTestcase();
        return DVT_FAIL;
    }
    resetFIFO();

    //MUSB_DeviceRequest devReq;
    devReq.bmRequestType = MUSB_DIR_IN | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE;
    devReq.bRequest = MUSB_REQ_GET_DESCRIPTOR;
    devReq.wValue = MUSB_DT_DEVICE << 8;
    devReq.wIndex = 0;
    devReq.wLength = 0x40;
    usb_config_ep0_req(0, USB_EP0_DATAIN, 0, &devReq);
    usb_start_ep0_transfer();
    while(!usb_check_ep0_complete() && !bError);
    if(bError){
        #if 0
        psOutput->eType = DVT_OUTPUT_RESULT;
        psOutput->eResult = DVT_RESULT_FAIL;
        #endif
        return DVT_FAIL;
    }
    Printf("GetDescriptor done\r\n");
    
    #if 1    
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
            #if 0
            psOutput->eType = DVT_OUTPUT_RESULT;
            psOutput->eResult = DVT_RESULT_FAIL;
            #endif
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
            #if 0
            psOutput->eType = DVT_OUTPUT_RESULT;
            psOutput->eResult = DVT_RESULT_FAIL;
            #endif
            return DVT_FAIL;
        }
        Printf("DEV RESERT step(%d) done: %s\r\n", nEnumStep, hubdevenumeration_step[nEnumStep].pDesciptor);
        nEnumStep++;
    }
    Printf("Hub Device Reset done\r\n");    
    #endif
    
    //USB_EP0_REQ *req;
    //i = i4EpNum;
	for(i=1; i<=devNum; i++)
	{
		MGC_QMU_Write8(MGC_O_HDRC_TXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_RXHUBADDR(0), 5);        
		MGC_QMU_Write8(MGC_O_HDRC_TXPORTADDR(0), i);        
		MGC_QMU_Write8(MGC_O_HDRC_RXPORTADDR(0), i);
		MGC_QMU_Write8(0x1a, 0x40);

	#if 1
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
	#endif
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

    //i = i4EpNum;
    for(i=1; i<=devNum; i++)
    {
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
		//SetMultiETHEPConfig(i);
    }
    
    //i = i4EpNum;
    for(i=1; i<=devNum; i++)
    {
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
                    //USB_EP0_REQ *req = usb_get_ep0_req();
                    req1 = usb_get_ep0_req();
                    req1->txBuf[0] = eth_enum_write_value[eth_enum_write_index][0];
                    req1->txBuf[1] = eth_enum_write_value[eth_enum_write_index++][1];
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
                    //USB_EP0_REQ *req = usb_get_ep0_req();
                    req = usb_get_ep0_req();
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
                    Printf ("case %d is not support.........\r\n\r\n\r\n", pDevReq->bRequest);
                    break;
               }
            }
    		
            usb_config_ep0_req(i, data_stage, count, pDevReq);
            usb_start_ep0_transfer();
            while(!usb_check_ep0_complete() && !bError);
            if(bError){
                #if 0
                psOutput->eType = DVT_OUTPUT_RESULT;
                psOutput->eResult = DVT_RESULT_FAIL;
                #endif
                return DVT_FAIL;
            }
            Printf("ETHENUM port (%d) step(%d) done: %s\r\n", i, nEnumStep, ethenumeration_step[nEnumStep].pDesciptor);
            nEnumStep++;
        }
    }

	Printf("ETH enumeration done\r\n");

    //vQmuDebugPrint(__LINE__);
	
	endTestcase();
    //Init CISR
    bError = FALSE;    
    isReset = 0;    
    initialQMU(TRUE);
    //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
    if(isPass == -1){
        endQMUTestcase();	 
        return DVT_FAIL;
    }
	#if 0 //!PORTING
	CISR_Detach(hqmu_cisr, QMU_HW_IRQ_NO);
	CISR_Dest(hqmu_cisr);

	CISR_Init(hdrc_cisr,HdrcIsrMultiIxiaQMU, 1, &hdrc);
	//Attach USB CISR
	CISR_Attach(hdrc_cisr, QMU_HW_IRQ_NO);   
	#else
	if(OSR_OK != x_reg_isr((unsigned short)USB_IRQ_NO, HdrcIsrMultiIxiaQMU, &pfnOldIsr))
	{
		Printf("Roll: Can't register IRQ %d\n", USB_IRQ_NO);
		return 0;
	}
	else
	{
		Printf("Register IRQ %d\n", USB_IRQ_NO);
	}

	Printf("USB Disable IRQ: %d\n", USB_IRQ_NO);
	BIM_DisableIrq(USB_IRQ_NO);
	#endif
    
    MGC_QMU_Write8(MGC_O_HDRC_INTRUSBE, MGC_M_INTR_CONNECT | MGC_M_INTR_BABBLE | MGC_M_INTR_VBUSERROR);
    Printf("[QMU] IXIA port number: %d\r\n", devNum);	
    is_bdp=FALSE;
    #if  !PORTING
    CIRQ_Mask(QMU_HW_IRQ_NO);
    #endif
    //i = i4EpNum;
    for(i=1; i<=devNum; i++)
    {
    	usb_start_QMU(i, USB_RX, FALSE);
    	usb_start_QMU(i, USB_TX, FALSE);
        //vQmuDebugPrint(__LINE__);
    }

#if 0 // mhzhang		
    TGPD* gpd=NULL;
    TGPD* rx_gpd=NULL;
    TGPD* tx_gpd=NULL;
#endif
		
    //i = i4EpNum;
    for(i=1; i<=devNum; i++)
    { 
        int j;	  
        Printf("[QMU] Config IXIA port number: %d\r\n", i);	
        //for(j=0; j<4000; j++){
        for(j=0; j<GPD_LIST_LEN; j++){
        	//Printf(">>>RX_gpd_end[%d]:%x\r\n", j, Rx_gpd_end[i]);
        	ixia_buf[i-1][j] = (unsigned char*)MUSB_MemAlloc(2048);//, 32);    
        	if(!ixia_buf[i-1][j])
        	{
                Printf("Alloc Mem Fail\n");
        	}
        	rx_gpd=Rx_gpd_end[i];
        	tx_gpd=Tx_gpd_end[i];
        #if 1 //ben
        	usb_insert_transfer_QMU(i, 3, USB_RX, (unsigned char* )ixia_buf[i-1][j], 2048, TRUE);
        	usb_insert_transfer_QMU(i, 2, USB_TX, (unsigned char* )ixia_buf[i-1][j], 1514, FALSE);
        #endif
        	//Printf("RX_gpd_end[%d]:%x\r\n", j, Rx_gpd_end[i]);
        }
       // gpd=Tx_gpd_end[i];
        Tx_gpd_end[i]=Tx_gpd_last[i];
        TGPD_SET_NEXT(tx_gpd, Tx_gpd_end[i]);
        TGPD_SET_CHKSUM_HWO(tx_gpd, 16);
        //x_free_aligned_mem(gpd);
       // gpd=Rx_gpd_end[i];
        Rx_gpd_end[i]=Rx_gpd_last[i];
        TGPD_SET_NEXT(rx_gpd, Rx_gpd_end[i]);
        TGPD_SET_CHKSUM(rx_gpd, 16);
        //x_free_aligned_mem(gpd);
//        gpd = NULL;
    }     
    //i = i4EpNum;
    for(i=1; i<=devNum; i++)
    {
        Printf("Resume Q#%d\n", i);
    	usb_resume_QMU(i, USB_RX);
        //Printf("Ixia test Q#%d\n", i);
        //Printf("Ixia test RQCSR 0x%08X\n", MGC_ReadQMU16(MGC_O_QMU_RQCSR(i)));
        //Printf("Ixia test RQSAR 0x%08X\n", MGC_ReadQMU16(MGC_O_QMU_RQSAR(i)));
        //Printf("Ixia test RQCPR 0x%08X\n", MGC_ReadQMU16(MGC_O_QMU_RQCPR(i)));
        //vQmuDebugPrint(__LINE__);
    }

    enableUsbIntr();

    #if 0 //!PORTING
    CIRQ_Unmask(QMU_HW_IRQ_NO); 
    #endif

    while(1);
}



int QMUStressLoopback(int i4Argc, const char **szArgv)
{

    unsigned int i,j;
#if 0  // mhzhang
   unsigned char keepGoing = TRUE;
   unsigned int userDecision;
   unsigned int randomValue;
#endif	 
    USB_REQ *req1 = NULL;
    USB_REQ *req2 = NULL;
    unsigned int u4Type = 0;

    vBdpDramBusy(TRUE);

    if (i4Argc < 4)
    {
        Printf("Arg: [Transfer Type] [TX Check Sum Printf] [RX Check Sum Printf]\n");
        Printf("[Transfer Type] 0:CTRL , 1:ISO , 2:BULK, 3:INTR \n");
        Printf("[TX Check Sum Printf] 0:Normal Case 1:TX Check Sum Printf\n");
        Printf("[RX Check Sum Printf] 0:Normal Case 1:RX Check Sum Printf\n");
        return 0;
    }

    u4Type = StrToInt(szArgv[1]);
    _fgTxErrChkSumTestCase = StrToInt(szArgv[2]);
    _fgRxErrChkSumTestCase = StrToInt(szArgv[3]);

    if(_fgTxErrChkSumTestCase)
    {
        Printf("*** NOTE ***: Perform QMU TX Check SUM Printf test Now!\n");
        //Ummark TX Queue Printf Indication Mask.
        MGC_WriteQIRQ32(MGC_O_QIRQ_TQEIMR, 0);         
    }

    if(_fgRxErrChkSumTestCase)
    {
        Printf("*** NOTE ***: Perform QMU RX Check SUM Printf test Now!\n");
        //Ummark RX Queue Printf Indication Mask.
        MGC_WriteQIRQ32(MGC_O_QIRQ_RQEIMR, 0);                 
    }
    
    //psOutput->eResult = DVT_RESULT_PASS;
    bError = FALSE;
    initialQMUTestcase(TRUE);
    //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
    if(isPass == -1){
        endQMUTestcase();
	 //CISR_Detach(hdrc_cisr, USB_HW_IRQ_NO);
        //CISR_Dest(hdrc_cisr);
        return DVT_FAIL;
    }
    //set address
    //MGC_QMU_Write8(MGC_O_HDRC_FADDR, 0x1);
    SetAddress(0x1);
    resetFIFO();
    //CISR_Detach(hdrc_cisr, USB_HW_IRQ_NO);
    //CISR_Dest(hdrc_cisr);
    if(u4Type == 2)
    {
        usb_ep_enable(1, USB_TX, USB_BULK, 512, 0, 1);
        usb_ep_enable(2, USB_RX, USB_BULK, 512, 0, 2);
    }
    else if(u4Type == 1)
    {
        usb_ep_enable(1, USB_TX, USB_ISO, 1024, 0, 1);
        usb_ep_enable(2, USB_RX, USB_ISO, 1024, 0, 2);
    }
    
    //usb_config_req(1, USB_TX, 1023, TRUE, 1, 3, 0, 0);
    //usb_config_req(2, USB_RX, 1023, TRUE, 2, 3, 0, 0);

    #if 0 //!PORTING      
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf("- Select QMU loopback test                                     -\r\n"); 
    Printf("---------------------------------------------------------------------------\r\n"); 
    Printf(" (0) BD (1) Data Buffer \r\n");
    Printf("\r\n");
    while(DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision, 0, (1)));
    if(userDecision == 0){
        is_bdp = TRUE;
    }else{
	 is_bdp = FALSE;
    }
    #else
    is_bdp = FALSE;
    #endif
    //while (keepGoing && !bError){   
    gpd_num=1;
    usb_start_QMU(1, USB_TX, 0);
    usb_start_QMU(2, USB_RX, 0);
    
    for(j=24;j>4&& (!bError); j--)
    {   
        usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
        randomTxBuf(1);       
        req1 = usb_get_req(1, USB_TX);              
        usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
        //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
        usb_resume_QMU(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
        //Printf("2:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE); 
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        //Printf("3:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
        Printf("rx complete...\r\n");
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
        Printf("length %d Done!\r\n", j);
		/*
        Printf("---------------------------------------------------------------------------\r\n"); 
        Printf("- Running loopback test                                     -\r\n"); 
        Printf("---------------------------------------------------------------------------\r\n"); 
        Printf(" (0) END (1) NEXT \r\n");
        Printf("\r\n");
        */
        /*
        while(DVT_SUCCESS != DVT_GetUserNumCommand(&userDecision, 0, (1)));
        if(userDecision == 0){
            keepGoing = FALSE;            
        }
        */
    }
#if 1
    for(j=510; j<520&& (!bError); j++){   
    //for(j=1; j<1025&& (!bError); j++){   
        usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
        randomTxBuf(1);       
        req1 = usb_get_req(1, USB_TX);
        usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
        usb_resume_QMU(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE);  
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        Printf("rx complete...\r\n");
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
        Printf("length %d Done!\r\n", j);
	
    }   
    Printf("end of 519\r\n");
    
    for(j=1022; j<1027&&(!bError); j++){   
	 usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
        randomTxBuf(1);       
        req1 = usb_get_req(1, USB_TX);
        usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
        usb_resume_QMU(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE);  
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        Printf("rx complete...\r\n");
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
        Printf("length %d Done!\r\n", j);
    }
    Printf("end of 1029\r\n");
    for(j=2045; j<2050&&( !bError); j++){   
	 usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
        randomTxBuf(1);       
        req1 = usb_get_req(1, USB_TX);
        usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
        usb_resume_QMU(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE);  
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        Printf("rx complete...\r\n");
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
        Printf("length %d Done!\r\n", j);
    }	
    Printf("end of 2049\r\n");

    
    for(j=9215; j<9218&& (!bError); j++){   
	 usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
        randomTxBuf(1);       
        req1 = usb_get_req(1, USB_TX);
        usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
        usb_resume_QMU(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE); 
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        Printf("rx complete...\r\n");
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
        Printf("length %d Done!\r\n", j);
    }	
    
    for(j=65534; j<65536&& (!bError); j++){   
	 usb_config_req(1, USB_TX, j, TRUE, 1, 3, 0, 0);
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
        randomTxBuf(1);       
        req1 = usb_get_req(1, USB_TX);
        usb_insert_transfer_QMU(1, 1, USB_TX, req1->buf, req1->count, TRUE); 
        usb_resume_QMU(1, USB_TX);
        while(!checkComplete(1, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE); 
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        Printf("rx complete...\r\n");
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }
        clearRxBuf(2);
        Printf("length %d Done!\r\n", j);
    }	
#endif

    Printf("QMU basic success\r\n");
    usb_config_req(1, USB_TX, 512, TRUE, 1, 3, 0, 0);
    for(i=0; i<512; i++){
        req1->buf[i] = 0xff;
    }
    usb_insert_transfer_QMU(1, 3, USB_TX, req1->buf, req1->count, TRUE);
    usb_resume_QMU(1, USB_TX);
    while(!checkComplete(1, USB_TX));
    endQMUTestcase();
    #if 0
    if(bError){
	psOutput->eResult = DVT_RESULT_FAIL;
    }else{
	psOutput->eType = DVT_OUTPUT_RESULT; 
    }
    #endif
    return DVT_SUCCESS;
}

#ifdef MUSB_QMU
void _MUSB_QMU_Init(void)
{
    unsigned int QCR = 0;

     //disable and flush interrupt
    disableIntr();
#ifdef Leon
    //Start Session
    MGC_QMU_Write8(MGC_O_HDRC_DEVCTL, MGC_M_DEVCTL_SESSION);
   // if(isHS){
        MGC_QMU_Write8(MGC_O_HDRC_POWER, (MGC_QREG_Read8(MGC_O_HDRC_POWER) | (MGC_M_POWER_HSENAB)|MGC_M_POWER_ISOUPDATE));
    //}
    //else{
        //MGC_QMU_Write8(MGC_O_HDRC_POWER, (((MGC_QREG_Read8(MGC_O_HDRC_POWER)) & (~MGC_M_POWER_HSENAB)) |MGC_M_POWER_ISOUPDATE));
    //}
    //Enable ConnIntr
    MGC_QMU_Write8(MGC_O_HDRC_INTRUSBE, MGC_M_INTR_CONNECT);
#endif

        for(i=1; i<5; i++){
            Rx_gpd_end[i]= (TGPD*)MUSB_MemAlloc(sizeof(TGPD));//, 32);
            Printf("ALLOC RX GPD End [%d] Mem@0x%08X\n", i, (unsigned int)Rx_gpd_end[i]);
            MUSB_MemSet(Rx_gpd_end[i], 0 , sizeof(TGPD));
            TGPD_CLR_FLAGS_HWO(Rx_gpd_end[i]);    
    
#if 0// mhzhang			
            MGC_WriteQMU32(MGC_O_QMU_RQSAR(i), Rx_gpd_end[i]);
#else
            MGC_WriteQMU32(MGC_O_QMU_RQSAR(i), (int)(Rx_gpd_end[i]));
#endif			
            Rx_gpd_last[i]=Rx_gpd_end[i];
            QCR|=USB_QMU_Rx_EN(i);
            Tx_gpd_end[i]= (TGPD*)MUSB_MemAlloc(sizeof(TGPD));//, 32);
            Printf("ALLOC TX GPD End [%d] Mem@0x%08X\n", i, (unsigned int)Tx_gpd_end[i]);
            MUSB_MemSet(Tx_gpd_end[i], 0 , sizeof(TGPD)+AT_GPD_EXT_LEN);
            TGPD_CLR_FLAGS_HWO(Tx_gpd_end[i]);    
#if 0 // mhzhang			
            MGC_WriteQMU32(MGC_O_QMU_TQSAR(i), Tx_gpd_end[i]);
#else
            MGC_WriteQMU32(MGC_O_QMU_TQSAR(i), (int)(Tx_gpd_end[i]));
#endif			
            Tx_gpd_last[i]=Tx_gpd_end[i];
            QCR|=USB_QMU_Tx_EN(i);
        }  
        Printf("QMU DMA enable is %x\n", QCR);
        MGC_WriteQUCS32(MGC_O_QUCS_USBGCSR,  QCR);
        MGC_WriteQIRQ32(MGC_O_QIRQ_QIMR, 0x00000000);
    
        oldwIntrTxEValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
        oldwIntrRxEValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
        isQMUmode=TRUE;
    
       // MGC_QMU_Write16(MGC_O_HDRC_INTRTXE, 1);
       // MGC_QMU_Write16(MGC_O_HDRC_INTRRXE, 0);
        MGC_QMU_Write16(MGC_O_HSDMA_CONFIG, MGC_QREG_Read16(MGC_O_HSDMA_CONFIG)|0x30);    
         MGC_QMU_Write16(MGC_O_HSDMA_CONTROLLER(4), 0x600);
         MGC_QREG_Write32(M_REG_INTRLEVEL1EN, 0x2F);


    enableUsbIntr();
    return;
}

void _MUSB_QMU_Transfer(uint8_t end_num, uint32_t u4Type, uint32_t framenum, uint8_t isHost)
{
    unsigned int j,k;
    USB_REQ *req1 = NULL;
    USB_REQ *req2 = NULL;
   // _MUSB_QMU_Init();
    MGC_QMU_Write8 (MGC_O_HDRC_INTRUSBE, 0);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRTXE, 0x01);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, 0);
    // u4Type = 0x01;
  //  isHost = TRUE;
    
    SetAddress(100);
    resetFIFO();
    
    if(u4Type == 2)
    {
        if(isHost)
            usb_ep_enable(end_num, USB_TX, USB_BULK, 512, 0, end_num);
        else
            usb_ep_enable(2, USB_RX, USB_BULK, 512, 0, 2);    
    }
    else if(u4Type == 1)
    {
        if(isHost)
            usb_ep_enable(end_num, USB_TX, USB_ISO, 1024, 0, end_num);
        else
            usb_ep_enable(2, USB_RX, USB_ISO, 1024, 0, 2);
    }

    gpd_num=1;

    Printf("before start qmu\n");
    if(isHost)
        usb_start_QMU(end_num, USB_TX, 0);
    else
        usb_start_QMU(2, USB_RX, 0);

    Printf("after start qmu\n");

    k++;

if(isHost)
{
    for(j = framenum;j >  4 ; j --)
    {   
    if(isHost)
        usb_config_req(end_num, USB_TX, j, TRUE, end_num, 3, 0, 0);
    else
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
    if(isHost)
    {
        randomTxBuf(end_num);       
        req1 = usb_get_req(end_num, USB_TX);   
        if((req1->count%0x04) == 1)
            gpd_tx_ioc_en = TRUE;
        else
            gpd_tx_ioc_en = TRUE;
        usb_insert_transfer_QMU(end_num, end_num, USB_TX, req1->buf, req1->count, TRUE); 
        //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
        if(gpd_tx_ioc_en == TRUE)
        {
            usb_resume_QMU(end_num, USB_TX);
            while(!checkComplete(end_num, USB_TX));
            Printf("tx complete...\r\n");
        }
        USB_hal_delay_ms (500);
    }
    else
    {
        //Printf("2:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE); 
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        //Printf("3:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
        Printf("rx complete...\r\n");
    }
    #if 0
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }

        clearRxBuf(2);
        #endif
        Printf("length %d Done!\r\n", j);
    }  
}
else
{
    for(j= 5; j < (framenum + 4); j++)
    {   
    if(isHost)
        usb_config_req(end_num, USB_TX, j, TRUE, end_num, 3, 0, 0);
    else
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
    if(isHost)
    {
        randomTxBuf(end_num);       
        req1 = usb_get_req(end_num, USB_TX);              
        usb_insert_transfer_QMU(end_num, end_num, USB_TX, req1->buf, req1->count, TRUE); 
        //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
        usb_resume_QMU(end_num, USB_TX);
        while(!checkComplete(end_num, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
    }
    else
    {
        //Printf("2:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
        req2 = usb_get_req(2, USB_RX);
        if((req2->count%0x04) == 1)
            gpd_rx_ioc_en = TRUE;
        else
            gpd_rx_ioc_en = TRUE;

        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE); 
        if(gpd_rx_ioc_en == TRUE)
        {
            usb_resume_QMU(2, USB_RX);
            while(!checkComplete(2, USB_RX));
        }
        //Printf("3:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
        Printf("rx complete...\r\n");
    }
#if 0
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }

        clearRxBuf(2);
    #endif
        Printf("length %d Done!\r\n", j);
    }  
}

}

#if 0
void _MUSB_QMU_Transfer_2(uint8_t end_num, uint8_t end_num2, uint32_t u4Type, uint32_t framenum, uint8_t isHost)
{
    unsigned int j,k;
    USB_REQ *req1 = NULL;
    USB_REQ *req2 = NULL;
   // _MUSB_QMU_Init();
    MGC_QMU_Write8 (MGC_O_HDRC_INTRUSBE, 0);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRTXE, 0x01);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, 0);
    // u4Type = 0x01;
  //  isHost = TRUE;
    
    SetAddress(100);
    resetFIFO();
    
    if(u4Type == 2)
    {
        if(isHost)
        {
            usb_ep_enable(end_num, USB_TX, USB_BULK, 512, 0, end_num);
            
            usb_ep_enable(end_num2, USB_TX, USB_BULK, 512, 0, end_num2);
        }
        else
            usb_ep_enable(2, USB_RX, USB_BULK, 512, 0, 2);    
    }
    else if(u4Type == 1)
    {
        if(isHost)
        {
            usb_ep_enable(end_num, USB_TX, USB_ISO, 1024, 0, end_num);
            usb_ep_enable(end_num2, USB_TX, USB_ISO, 1024, 0 end_num2);
        }
        else
        {
            usb_ep_enable(2, USB_RX, USB_ISO, 1024, 0, 2);
        }
    }

    gpd_num=1;

    Printf("before start qmu\n");
    if(isHost)
    {
        usb_start_QMU(end_num, USB_TX, 0);
        usb_start_QMU(end_num2, USB_TX, 0);
    }
    else
        usb_start_QMU(2, USB_RX, 0);

    Printf("after start qmu\n");

    k++;

if(isHost)
{
    for(j = framenum;j >  4 ; j --)
    {   
    if(isHost)
    {
        usb_config_req(end_num, USB_TX, j, TRUE, end_num, 3, 0, 0);
        usb_config_req(end_num2, USB_TX, j, TRUE, end_num2, 3, 0, 0);
   }        
    else
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
    if(isHost)
    {
        randomTxBuf(end_num);       
        req1 = usb_get_req(end_num, USB_TX);   
        if((req1->count%0x04) == 1)
            gpd_tx_ioc_en = TRUE;
        else
            gpd_tx_ioc_en = TRUE;
        usb_insert_transfer_QMU(end_num, end_num, USB_TX, req1->buf, req1->count, TRUE); 
        //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
        if(gpd_tx_ioc_en == TRUE)
        {
            usb_resume_QMU(end_num, USB_TX);
            while(!checkComplete(end_num, USB_TX));
            Printf("tx1 complete...\r\n");
        }
        USB_hal_delay_ms (500);
        
        randomTxBuf(end_num2);       
        req1 = usb_get_req(end_num2, USB_TX);   
        if((req1->count%0x04) == 1)
            gpd_tx_ioc_en = TRUE;
        else
            gpd_tx_ioc_en = TRUE;
        usb_insert_transfer_QMU(end_num2, end_num2, USB_TX, req1->buf, req1->count, TRUE); 
        //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
        if(gpd_tx_ioc_en == TRUE)
        {
            usb_resume_QMU(end_num2, USB_TX);
            while(!checkComplete(end_num2, USB_TX));
            Printf("tx2 complete...\r\n");
        }
        USB_hal_delay_ms (500);
    }
    else
    {
        //Printf("2:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
        req2 = usb_get_req(2, USB_RX);
        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE); 
        usb_resume_QMU(2, USB_RX);
        while(!checkComplete(2, USB_RX));
        //Printf("3:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
        Printf("rx complete...\r\n");
    }
    #if 0
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }

        clearRxBuf(2);
        #endif
        Printf("length %d Done!\r\n", j);
    }  
}
else
{
    for(j=4;j< (framenum + 4); j++)
    {   
    if(isHost)
        usb_config_req(end_num, USB_TX, j, TRUE, end_num, 3, 0, 0);
    else
        usb_config_req(2, USB_RX, j, TRUE, 2, 3, 0, 0);        
    if(isHost)
    {
        randomTxBuf(end_num);       
        req1 = usb_get_req(end_num, USB_TX);              
        usb_insert_transfer_QMU(end_num, end_num, USB_TX, req1->buf, req1->count, TRUE); 
        //Printf("Tx_gpd_end[1]:%x\r\n", Tx_gpd_end[1]);
        usb_resume_QMU(end_num, USB_TX);
        while(!checkComplete(end_num, USB_TX));
        Printf("tx complete...\r\n");
        USB_hal_delay_ms (500);
    }
    else
    {
        //Printf("2:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
        req2 = usb_get_req(2, USB_RX);
        if((req2->count%0x04) == 1)
            gpd_rx_ioc_en = TRUE;
        else
            gpd_rx_ioc_en = FALSE;

        usb_insert_transfer_QMU(2, 2,  USB_RX, req2->buf, req2->count, TRUE); 
        if(gpd_rx_ioc_en == TRUE)
        {
            usb_resume_QMU(2, USB_RX);
            while(!checkComplete(2, USB_RX));
        }
        //Printf("3:Tx_gpd_current[1]:%x\r\n", MGC_ReadQMU32(MGC_O_QMU_TQCPR(1)));
        Printf("rx complete...\r\n");
    }
#if 0
        for(i=0; i<req1->count; i++){
            if(req2->buf[i] != req1->buf[i]){
                Printf("buffer %d not match, expected[%d], value[%d]....\r\n", i, req1->buf[i], req2->buf[i]);
                bError = TRUE;
                //psOutput->eResult = DVT_RESULT_FAIL;
                break;
            }
        }

        clearRxBuf(2);
    #endif
        Printf("length %d Done!\r\n", j);
    }  
}

}
#endif

#endif

