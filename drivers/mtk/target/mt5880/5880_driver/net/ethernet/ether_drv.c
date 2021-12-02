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
/********************************************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: ether_drv.c $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   Ethernet driver source file. 
 *
 * Author: Hs Huang
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author:
 *
 * $Modtime:
 *
 * $Revision:
****************************************************************************/
#include "ether_com.h"
#include "ether_drv.h"
#include "ether_if.h"
#include "drvcust_if.h"

//Hewitt
#define USE_CRIT 0
/****************************************************************************
** External function
****************************************************************************/
extern INT32 i4EtherIndicateFrame(NET_DRV_IF_T *prDev, PKT_BUFF_T *prPktBuf);
INT32  fgPrintDes=FALSE;
INT32  fgDMAPollingEn=FALSE;
INT32  _TxDesCtrlBitOnfgVTG=0;
INT32   _TxDesCtrlfgVTG=0;

INT32  _TxDesCtrlBitOnfgTCO=0;
INT32   _TxDesCtrlfgTCO=0;

INT32  _TxDesCtrlBitOnfgUCO=0;
INT32   _TxDesCtrlfgUCO=0;

INT32  _TxDesCtrlBitOnfgICO=0;
INT32   _TxDesCtrlfgICO=0;

INT32  _TxDesCtrlBitOnfgINSV=0;
INT32   _TxDesCtrlfgINSV=0;


INT32  _TxDesCtrlBitOnfgTxIntOn=0;
INT32   _TxDesCtrlfgTxIntOn=0;


INT32  _TxDesCtrlBitOnfgLastSeg=0;
INT32   _TxDesCtrlfgLastSeg=0;
 
INT32  _TxDesCtrlBitOnfgFirstSeg=0;
INT32   _TxDesCtrlfgFirstSeg=0;

INT32  _TxDesCtrlBitOnfgEndOfRing=0;
INT32   _TxDesCtrlfgEndOfRing=0;
BOOL _fgSupportTxChkSumOffload = FALSE;
// *********************************************************************
// Variable Declaration
// *********************************************************************
UINT32 u4HashTable[2][8]=
{
#if 1 //0x00~0xff , 0x100~0x1ff all be 1
 {
    0xffffffff,//bit 0~31	
    0xffffffff,//bit 32~63
    0xffffffff,//bit 64~95
    0xffffffff,//bit 96~127
    0xffffffff,//bit 128~159
    0xffffffff,//bit 160~191
    0xffffffff,//bit 192~223
    0xffffffff,//bit 224~255

 },
 {

     0xffffffff,//bit 256~287	
    0xffffffff,//bit 288~319
    0xffffffff,//bit 320~351
    0xffffffff,//bit 352~383
    0xffffffff,//bit 384~415
    0xffffffff,//bit 416~447
    0xffffffff,//bit 448~479
    0xffffffff,//bit 480~511

 }
  #endif  
 #if 0  //0x1CC(460) ==1
{
    0x00000000,//bit 0~31	
    0x00000000,//bit 32~63
    0x00000000,//bit 64~95
    0x00000000,//bit 96~127
    0x00000000,//bit 128~159
    0x00000000,//bit 160~191
    0x00000000,//bit 192~223
    0x00000000,//bit 224~255

 },
 {

     0x00000000,//bit 256~287	
    0x00000000,//bit 288~319
    0x00000000,//bit 320~351
    0x00000000,//bit 352~383
    0x00000000,//bit 384~415
    0x00000000,//bit 416~447
    0x00001000,//bit 448~479
    0x00000000,//bit 480~511

 }
 
   #endif

};

// *********************************************************************
// Local function prototype
// *********************************************************************

// *********************************************************************
// Constant Data Declaration
// *********************************************************************

//#define NONCACHE(addr)				(((addr) & 0x0fffffff) | 0x60000000)
#define CACHE(addr)					((addr) & 0x0fffffff)

#if 0
/* The function reply device status information */
BOOL GET_ETHER_INFO(PETHER_ADAPTER_T prMac, UINT32 Status)
{
    UINT32 u4Ret; 
    
    ETHER_SEMA_LOCK(prMac->hDevStatusSema);
    u4Ret = (prMac->u4DevStatus & Status);
    ETHER_SEMA_UNLOCK(prMac->hDevStatusSema);
        
    return((BOOL)u4Ret);            
}

/* The function set device status information */
void SET_ETHER_INFO(PETHER_ADAPTER_T prMac, UINT32 Status)
{
    ETHER_SEMA_LOCK(prMac->hDevStatusSema);
    prMac->u4DevStatus |= Status;
    ETHER_SEMA_UNLOCK(prMac->hDevStatusSema);
}

/* The function clear device status information */
void CLR_ETHER_INFO(PETHER_ADAPTER_T prMac, UINT32 Status)
{
    ETHER_SEMA_LOCK(prMac->hDevStatusSema);
    prMac->u4DevStatus &= ~(Status);
    ETHER_SEMA_UNLOCK(prMac->hDevStatusSema);
}
#endif
/*******************************************************************************
**  i4EtherRxPacketCreate
**
**  descriptions:
**      This routine is called to creat packet pool. It applies partial memory  
**      allocation API and it cannot be released in driver layer. 
**      So the memory pool can only be created once.  
**  parameters:
**  result:
**  return:
*******************************************************************************/
static INT32 i4EtherRxPacketCreate (PETHER_ADAPTER_T prMac)
{
    INT32 i4Ret = 0, i;
    UINT8 *pucPktBuf;
    PKT_BUFF_T *prPktBuf;
    UINT32 u4PktSize;

    ASSERT(prMac != NULL);

    /* each packet buffer size is 128-bit(16 byte) aligned */
    prMac->u4RxPktHdrSize = (sizeof(PKT_BUFF_T) + 16 - 1) & ~(16 - 1);
    u4PktSize = prMac->u4RxPktHdrSize + ETHER_PKT_BUF_SZ;

    #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    pucPktBuf = (UINT8 *)VIRTUAL(x_alloc_dma_mem((ETHER_RX_PKT_BUF_NUM * u4PktSize) + 16));
    #else    
    //pucPktBuf = (UINT8 *)x_mem_alloc((ETHER_RX_PKT_BUF_NUM * u4PktSize) + 16);
    pucPktBuf = (UINT8 *)BSP_AllocAlignedDmaMemory((ETHER_RX_PKT_BUF_NUM * u4PktSize), 16);
    #endif

    if(pucPktBuf == NULL)
    {
        ERR_ETHER("no memory for rx packet buffer\n");
        return (NET_DRV_ERROR);
    }
    prMac->pu4RxPktBuf = (UINT32 *)pucPktBuf;
    //pucPktBuf = (UINT8 *)ETHER_NONCACHE((UINT32)pucPktBuf);
    x_memset(pucPktBuf, 0, (ETHER_RX_PKT_BUF_NUM * u4PktSize));

//    prPktBuf = (PKT_BUFF_T *)pvNetAlignAddr(pucPktBuf, 128);
    prPktBuf = (PKT_BUFF_T *)pucPktBuf;
    for (i = 0; i < (ETHER_RX_PKT_BUF_NUM); i++)
    {
        prPktBuf->pui1_data = (UINT8 *)((UINT32)prPktBuf + prMac->u4RxPktHdrSize);
        MLIST_INSERT(&prMac->rRxPktList, &prPktBuf->t_link);
        prPktBuf = (PKT_BUFF_T *)((UINT32)prPktBuf + u4PktSize);
    }

    /* sync cache and non-cache memory content before using it */  
    //HalFlushInvalidateDCache();    

    /* Initialize ethernet packet list semaphhore */
    i4Ret = x_sema_create(&prMac->hRxListSema,
                           X_SEMA_TYPE_BINARY,
                           X_SEMA_STATE_UNLOCK);
    ASSERT(i4Ret == OSR_OK);
 
    return(i4Ret);
}

/*******************************************************************************
**  pucEtherRxPacketAlloc
**
**  descriptions:
**      This routine is called to allocate packet buffer
**  parameters:
**      prMac - pointer to adapter structure
**  result:
**  return:
**      Pointer tp packet data address if success 
*******************************************************************************/
static UINT8 *pucEtherRxPacketAlloc(PETHER_ADAPTER_T prMac)
{
    UCHAR *pucPktPtr;
    PKT_BUFF_T *prPktBuf = NULL;
	LIST_ENTRY_T *p_ent = (LIST_ENTRY_T *)prPktBuf;

#ifdef USE_CRIT
    CRIT_STATE_T  t_state;
#endif

    ASSERT(prMac != NULL);

    //MLIST_REMOVE(&prMac->rRxPktList, (LIST_ENTRY_T *)prPktBuf);
    MLIST_REMOVE(&prMac->rRxPktList, p_ent);
    prPktBuf = (PKT_BUFF_T *)p_ent;
    
    if(prPktBuf == NULL)
    {
#ifdef USE_CRIT    
        t_state = x_crit_start();
#else
        ETHER_SEMA_LOCK(prMac->hRxListSema);
#endif
        if(!GET_ETHER_INFO(prMac, RX_BUF_OVERRUN))
        {
            /* record the descriptor index to allocate buffer at next defer task service */
            prMac->u4RxDescNoBufIdx = prMac->u4RxIdx;
            prMac->u4RxNoBufCnt++;
            WARN_ETHER("rx buffer overrun %d times\n", prMac->u4RxNoBufCnt); 
            SET_ETHER_INFO(prMac, RX_BUF_OVERRUN);
        }
#ifdef USE_CRIT
        x_crit_end(t_state);
#else
        ETHER_SEMA_UNLOCK(prMac->hRxListSema);
#endif
        return (0);
    }

    /* Get free packet buffer address */
    pucPktPtr = (UINT8 *)(prPktBuf->pui1_data);
    ASSERT(pucPktPtr != NULL);
    pucPktPtr = (UINT8 *)pucPktPtr;
    prPktBuf->ui4_tag = ETHER_PKT_MASK;

    if(prMac->rRxPktList.ui4_count < NUM_RX_DESC)
    {
        /* do this every time when free packet buffer count is less than descriptor count */
        HalFlushInvalidateDCache();    
        DBG_ETHER("free rx buffer %d < descriptor count %d\n", prMac->rRxPktList.ui4_count, NUM_RX_DESC); 
    }
    
    return(pucPktPtr);
}

/*******************************************************************************
**  i4EtherRxPacketFree
**
**  descriptions:
**      This routine is called to free rx packet buffer allocated in ethernet driver
**  parameters:
**      prDev - pointer to net device structure
**      pvPktBuf - pointer to packet buffer
**  result:
**  return:
*******************************************************************************/
static INT32 i4EtherRxPacketFree(NET_DRV_IF_T *prDev, PKT_BUFF_T *prPktBuf)
{
    PETHER_ADAPTER_T prMac;
    volatile PMAC_RX_DESC_CTRL_T prCtrl;
    PMAC_RX_DESC_T prRxDesc;
#ifdef USE_CRIT    
    CRIT_STATE_T  t_state;
#endif

    ASSERT(prDev != NULL);
    ASSERT(prPktBuf != NULL);
    
    /* NIC and IP stack may be more than one */
    //Hewitt
    if (prPktBuf->ui2_refcnt == 0) 
    {
        INFO_ETHER(" double free \n");
        return NET_DRV_PKT_BUFF_ERROR;   
    }

    if (--prPktBuf->ui2_refcnt)
    {
        WARN_ETHER("reference count of rx free: %d\n", prPktBuf->ui2_refcnt);
        return NET_DRV_OK;   
    }
    prPktBuf->ui2_flag = 0;

    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
    ASSERT(prPktBuf->ui4_tag == ETHER_PKT_MASK);

    /* procedure when rx buffer is overrun */
    if(GET_ETHER_INFO(prMac, RX_BUF_OVERRUN))
    {
#ifdef USE_CRIT    
        t_state = x_crit_start();
#else    
        ETHER_SEMA_LOCK(prMac->hRxListSema);
#endif
        /* allocate buffer for the last descriptor that buffer is full */
        prRxDesc = prMac->prRxDesc + prMac->u4RxDescNoBufIdx;
        prCtrl = &prRxDesc->rRxCtrl;

        if((++prMac->u4RxDescNoBufIdx) >= NUM_RX_DESC)     
        {
            prMac->u4RxDescNoBufIdx = 0;
        }
        if(prMac->u4RxDescNoBufIdx == prMac->u4RxIdx)     
        {
            CLR_ETHER_INFO(prMac, RX_BUF_OVERRUN);
        }

        /* need to do this every packet in this case */
        HalFlushInvalidateDCache();           

        /* assign packet data buffer address and set own bit for DMA */
        #if CONFIG_DRV_LINUX_DATA_CONSISTENCY    //kenny   
        prRxDesc->u4BufAddr = PHYSICAL((UINT32)(prPktBuf->pui1_data));
        #else
        prRxDesc->u4BufAddr = (UINT32)(prPktBuf->pui1_data);        
        #endif

        INIT_RX_DESC(prCtrl);

        /* only resume here to make rx interrupt available again */
        vDmaRxResume();
#ifdef USE_CRIT
        x_crit_end(t_state);
#else
        ETHER_SEMA_UNLOCK(prMac->hRxListSema);
#endif
    }
    else
    {
        /* put it back to rx packet list */
        MLIST_INSERT(&prMac->rRxPktList, &prPktBuf->t_link);            
    }    

    return(0);
}

/*******************************************************************************
**  vEtherTxBufCreat
**
**  descriptions:
**      Creat tx descriptor chain buffer and init its value
**  parameters:
**      prMac - pointer to adapter struct
**  result:
**  return:
*******************************************************************************/
static INT32 i4EtherTxDescCreat(PETHER_ADAPTER_T prMac)
{
    UINT32 *pu4DescPtr, i;
    PMAC_TX_DESC_T prTxDesc, prTxDescPrev;

    ASSERT(prMac != NULL);

    #if 0
    /* Initialize ethernet packet list semaphhore */
    VERIFY(x_sema_create(&prMac->hTxListSema,
                         X_SEMA_TYPE_BINARY,
                         X_SEMA_STATE_UNLOCK) == OSR_OK);
    #endif

    /* Allocate tx descriptor, non-cache */
    #if CONFIG_DRV_LINUX_DATA_CONSISTENCY 
     //pu4DescPtr = (UINT32 *)x_alloc_aligned_dma_mem(NUM_TX_DESC * sizeof(MAC_TX_DESC_T), 128);
     pu4DescPtr = (UINT32 *)VIRTUAL(BSP_AllocAlignedDmaMemory((NUM_TX_DESC) * sizeof(MAC_TX_DESC_T), 16));
    #else    
    //pu4DescPtr = (UINT32 *)x_alloc_aligned_nc_mem(NUM_TX_DESC * sizeof(MAC_TX_DESC_T), 128);
    pu4DescPtr = (UINT32 *)BSP_AllocAlignedDmaMemory((NUM_TX_DESC) * sizeof(MAC_TX_DESC_T), 16);
    #endif
    if(pu4DescPtr == NULL)
    {
        //vEtherAdapterRemove(prMac);
        return(NET_DRV_ERROR);
    }
    #if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY )
    pu4DescPtr = (UINT32*)NONCACHE((UINT32)pu4DescPtr);                   
    #endif

    prMac->pu4TxDescBuf = pu4DescPtr;

    #if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY) 
	HalFlushInvalidateDCache();  //CM 20090326
    #endif
	
    x_memset(pu4DescPtr, 0x00, NUM_TX_DESC * sizeof(MAC_TX_DESC_T));

    /* record the first tx descriptor */
    prMac->prTxDesc = (PMAC_TX_DESC_T)pu4DescPtr;

    prTxDescPrev = NULL;
    prTxDesc = (PMAC_TX_DESC_T)pu4DescPtr;
    for(i = 0; i < NUM_TX_DESC; i++)
    {
        prTxDesc->rTxCtrl.Bits.fgFirstSeg = TRUE;   // each tx buffer is big enough for transmission
        prTxDesc->rTxCtrl.Bits.fgLastSeg = TRUE;    // 
        prTxDesc->rTxCtrl.Bits.fgTxIntOn = TRUE;
	 prTxDesc->rTxCtrl.Bits.fgOwn = OWNED_BY_CPU;

        /* assign packet data buffer address */
        prTxDesc->u4BufAddr = 0x00;
        prTxDescPrev = prTxDesc;
        prTxDesc = (PMAC_TX_DESC_T)prTxDesc + 1;
        
    }
    /* link next descriptor to first one to make as ring */
    prTxDescPrev->rTxCtrl.Bits.fgEndOfRing = TRUE;
    prMac->u4TxIdx = 0;

#if CONFIG_DRV_LINUX_DATA_CONSISTENCY        
    /* sync cache and DRAM memory content before receiving data */
    HalFlushInvalidateDCache();   
#endif

    return(NET_DRV_OK);
}

/*******************************************************************************
**  vEtherRxBufCreat
**
**  descriptions:
**      Creat rx descriptor and buffer pool and init descriptor
**  parameters:
**      prMac - pointer to adapter struct
**  result:
**  return:
*******************************************************************************/
static INT32 i4EtherRxDescCreat(PETHER_ADAPTER_T prMac)
{
    UCHAR *pucPktPtr;
    UINT32 *pu4DescPtr, i;
    PMAC_RX_DESC_T prRxDesc, prRxDescPrev;

    ASSERT(prMac != NULL);

    /* Create rx packet pool */
    if(i4EtherRxPacketCreate(prMac) != NET_DRV_OK)
    {
        //vEtherAdapterRemove(prMac);
        return(NET_DRV_ERROR);
    }

    /* Allocate rx descriptor non-cached buffer */
    #if CONFIG_DRV_LINUX_DATA_CONSISTENCY        
    //pu4DescPtr = (UINT32 *)x_alloc_aligned_dma_mem(NUM_RX_DESC * sizeof(MAC_RX_DESC_T), 128);
    pu4DescPtr = (UINT32 *)VIRTUAL(BSP_AllocAlignedDmaMemory((NUM_RX_DESC) * sizeof(MAC_RX_DESC_T), 16)); 
    #else   
    //pu4DescPtr = (UINT32 *)x_alloc_aligned_nc_mem(NUM_RX_DESC * sizeof(MAC_RX_DESC_T), 128);
    pu4DescPtr = (UINT32 *)BSP_AllocAlignedDmaMemory((NUM_RX_DESC) * sizeof(MAC_RX_DESC_T), 16);
    #endif 
    if(pu4DescPtr == NULL)
    {
        //vEtherAdapterRemove(prMac);
        return(NET_DRV_ERROR);
    }
    #if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY) 
    pu4DescPtr = (UINT32*)NONCACHE((UINT32)pu4DescPtr);
    #endif

    prMac->pu4RxDescBuf = pu4DescPtr;

    #if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY) 
	HalFlushInvalidateDCache();   //CM 20090326
    #endif
	
    x_memset(pu4DescPtr, 0x00, NUM_RX_DESC * sizeof(MAC_RX_DESC_T));

    /* record the first rx descriptor and rx buffer */
    prMac->prRxDesc = (PMAC_RX_DESC_T)pu4DescPtr;
    prRxDescPrev = NULL;
    prRxDesc = (PMAC_RX_DESC_T)pu4DescPtr;
    for(i = 0; i < NUM_RX_DESC; i++)
    {
        prMac->u4RxIdx = i;
        pucPktPtr = pucEtherRxPacketAlloc(prMac);
        ASSERT(pucPktPtr != NULL);

        prRxDesc->rRxCtrl.Bits.fgOwn = OWNED_BY_DMA;
        prRxDesc->rRxCtrl.Bits.u2BufSize = PKT_BUFFER_SIZE;

        /* assign packet data buffer address */
        #if CONFIG_DRV_LINUX_DATA_CONSISTENCY    //kenny   
        prRxDesc->u4BufAddr = PHYSICAL((UINT32)pucPktPtr);
        #else
        prRxDesc->u4BufAddr = (UINT32)pucPktPtr;
        #endif
        
        prRxDescPrev = prRxDesc;
        prRxDesc = (PMAC_RX_DESC_T)prRxDesc + 1;        
    }
    /* link next descriptor to first one to make as ring */
    prRxDescPrev->rRxCtrl.Bits.fgEndOfRing = TRUE;
    prMac->u4RxIdx = 0;

#if CONFIG_DRV_LINUX_DATA_CONSISTENCY        
    /* sync cache and DRAM memory content before receiving data */
    HalFlushInvalidateDCache();    
#endif

    return(NET_DRV_OK);
}

/*******************************************************************************
**  vEtherAdapterRemove
**
**  descriptions:
**      Remove ethernet adapter and free allocated memory.
**  parameters:
**      prMac - pointer to adapter struct
**  result:
**  return:
**      0 if success
**      negative if fail 
*******************************************************************************/
void vEtherAdapterRemove(PETHER_ADAPTER_T prMac)
{
	UINT32 pu4Ptr; 
		
    ASSERT(prMac != NULL);

    /* free rx resource */
    if(prMac->pu4RxPktBuf != NULL)
    {
       #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
        //x_mem_free(prMac->pu4RxPktBuf);
        BSP_FreeAlignedDmaMemory((UINT32)prMac->pu4RxPktBuf);
       #else
        //x_mem_free(prMac->pu4RxPktBuf);
        BSP_FreeAlignedDmaMemory((UINT32)prMac->pu4RxPktBuf); 
       #endif

    }   
    if(prMac->pu4RxDescBuf != NULL)
    {
       #if CONFIG_DRV_LINUX_DATA_CONSISTENCY   
        //x_mem_free(prMac->pu4RxDescBuf);
        pu4Ptr = CACHE((UINT32)(prMac->pu4RxDescBuf));
        BSP_FreeAlignedDmaMemory(pu4Ptr);
       #else
       //x_mem_free(prMac->pu4RxDescBuf);
        pu4Ptr = CACHE((UINT32)(prMac->pu4RxDescBuf));
        BSP_FreeAlignedDmaMemory(pu4Ptr);

       #endif  
    }   
    VERIFY(x_sema_delete(prMac->hRxListSema) == OSR_OK);

    /* free tx resource */
    /*
    if(prMac->pu4TxPktList != NULL)
    {
        //x_mem_free(prMac->pu4TxPktList);
        BSP_FreeAlignedDmaMemory(prMac->pu4TxPktList);
    } 
    */    
    if(prMac->pu4TxDescBuf != NULL)
    {
     #if CONFIG_DRV_LINUX_DATA_CONSISTENCY 
        //x_mem_free(prMac->pu4TxDescBuf);
        pu4Ptr = CACHE((UINT32)(prMac->pu4TxDescBuf));
        BSP_FreeAlignedDmaMemory(pu4Ptr);  
     #else
        //x_mem_free(prMac->pu4TxDescBuf);
        pu4Ptr = CACHE((UINT32)(prMac->pu4TxDescBuf));
        BSP_FreeAlignedDmaMemory(pu4Ptr);  

     #endif       
    }
    
    VERIFY(x_sema_delete(prMac->hDevStatusSema) == OSR_OK);
    
    /* free adapter structure */
    //if(prMac != NULL)			
    {
      #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)   
       x_mem_free(prMac);
      #else
       x_mem_free(prMac);
      #endif

    }
}

/*******************************************************************************
**  i4EtherAdapterCreat
**
**  descriptions:
**      Creat ethernet adapter.
**          1.MAC driver memory allocation and init
**          2.Enable MAC port, include MAC, DMA and PHY init 
**  parameters:
**      prDev - pointer to struct net_device
**  result:
**  return:
**      0 if success
**      negative if fail 
*******************************************************************************/
INT32 i4EtherAdapterCreat(NET_DRV_IF_T *prDev)
{
    PETHER_ADAPTER_T prMac;
	INT32 i4Ret;
	
    ASSERT(prDev != NULL);

    /* Init ethernet adapter variables */
   #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    prMac = (PETHER_ADAPTER_T)x_mem_alloc(sizeof(ETHER_ADAPTER_T));
    #else    
    prMac = (PETHER_ADAPTER_T)x_mem_alloc(sizeof(ETHER_ADAPTER_T));
    #endif
    
    
    if(prMac == NULL)
    {
        //vEtherAdapterRemove(prMac);
        return(NET_DRV_ERROR);
    }
    x_memset(prMac, 0x00, sizeof(ETHER_ADAPTER_T));
    prDev->pv_priv = (PETHER_ADAPTER_T)prMac;
    prMac->u4PhyAddr = PHY_DEV_ADDR;
    prMac->u4RxIdx = 0;

    /* creat device status semaphhore */
    i4Ret = x_sema_create(&prMac->hDevStatusSema,
                           X_SEMA_TYPE_BINARY,
                           X_SEMA_STATE_UNLOCK);
    ASSERT(i4Ret == OSR_OK);

    /* creat and init tx descriptor */
    i4Ret = i4EtherTxDescCreat(prMac);
    if(i4Ret != NET_DRV_OK)
    {
        ERR_ETHER("creat tx buffer fail\n");
        return(i4Ret);
    }
    
    /* creat and init rx descriptor and buffer pool */
    i4Ret = i4EtherRxDescCreat(prMac);
    if(i4Ret != NET_DRV_OK)
    {
        ERR_ETHER("creat rx buffer fail\n");
        return(i4Ret);
    }

    /* set device init status */
    SET_ETHER_INFO(prMac, ETHER_DEV_INIT);

    DBG_ETHER("Allocated memory for ethernet driver:\n");
    DBG_ETHER("ETHER_ADAPTER_T             = %d\n", sizeof(ETHER_ADAPTER_T));
    DBG_ETHER("RX packet link list         = %d\n", (ETHER_RX_PKT_BUF_NUM * sizeof(ETHER_RX_LIST_T)));
    DBG_ETHER("TX packet link list         = %d\n", (ETHER_TX_PKT_BUF_NUM * sizeof(ETHER_TX_LIST_T)));
    DBG_ETHER("RX packet buffer            = %d\n", ((prMac->u4RxPktHdrSize + ETHER_PKT_BUF_SZ) * ETHER_RX_PKT_BUF_NUM));
    DBG_ETHER("RX descriptor(non-cache)    = %d\n", (NUM_RX_DESC + 1) * sizeof(MAC_RX_DESC_T));
    DBG_ETHER("TX descriptor(non-cache)    = %d\n", (NUM_TX_DESC + 1) * sizeof(MAC_TX_DESC_T));

    return(NET_DRV_OK);
}

/*******************************************************************************
**  vEtherModuleInit
**
**  descriptions:
**      This routine is called reset and init ethernet module
**  parameters:
**      prDev - pointer to net device structure
**  result:
**  return:
*******************************************************************************/
void vEtherModuleInit(NET_DRV_IF_T *prDev)
{
    PETHER_ADAPTER_T prMac;

    ASSERT(prDev != NULL);
    prMac = prDev->pv_priv;

#if 0
    if(DRVCUST_InitGet(eFlagDDRQfp))//MT5365, QFP
    {
    /* clk source, pin setting !! */
    vRMiiInit(); 
    }
#endif

    /* DMA init, reset GMAC, must init first!! */
    vDmaInit(prMac);    

    /* MMC init, mask all MMC interrupt */
    vMmcInit();         

    /* MAC general register and PHY init */
    vMacInit(prMac);

    vDmaIntClearAll();  // clear all interrupt requests 
    //vDmaIntEnable(DMA_INT_ENABLE); // enable all interrupts 
    vDmaIntEnable((DMA_INT_ENABLE|PORT_STATUS_CFG)); //Kenny test
}

/*******************************************************************************
**  vEtherDrvStart
**
**  descriptions:
**      This routine is called to start ethernet module
**  parameters:
**      prMac - pointer to net adapter structure
**  result:
**  return:
*******************************************************************************/
void vEtherDrvStart(NET_DRV_IF_T *prDev)
{
    ASSERT(prDev != NULL);

    vHalSetMacPwrDown(FALSE);
    vDmaIntClearAll();  
   //  vDmaIntEnable(DMA_INT_ENABLE); 
    vDmaIntEnable((DMA_INT_ENABLE|PORT_STATUS_CFG)); //Kenny test
    
    vDmaRxStart();      // Start Rx
    vDmaTxStart();      // Start Tx

    vDmaRxResume();
    //vDmaTxResume();

}

/*******************************************************************************
**  vEtherDrvStop
**
**  descriptions:
**      This routine is called to stop ethernet module
**  parameters:
**      prMac - pointer to net adapter structure
**  result:
**  return:
*******************************************************************************/
void vEtherDrvStop(NET_DRV_IF_T *prDev)
{
    PETHER_ADAPTER_T prMac;
    
    ASSERT(prDev != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;

    /* disable and clear all interrupts */
    vDmaIntDisableAll();         
    vDmaIntClear(DMA_INT_CLEAR_ALL);

    /* Stop tx/rx engine */
    vDmaTxStop(prMac);           // stop receiver, take ownership of all rx descriptors          
    vDmaRxStop(prMac);           // stop transmitter, take ownership of all tx descriptors   

    //vHalSetMacPwrDown(TRUE);
}

/*******************************************************************************
**  vEtherDrvSwReset
**
**  descriptions:
**      This routine is called to reset all driver's status and parameters
**  parameters:
**      prDev - pointer to net device structure
**  return:
*******************************************************************************/
void vEtherDrvSwReset(NET_DRV_IF_T *prDev)
{
    PETHER_ADAPTER_T prMac;
    PMAC_RX_DESC_T prRxDesc;
    PMAC_TX_DESC_T prTxDesc;
    PKT_BUFF_T *prTxPktBuf = NULL;
    LIST_ENTRY_T *p_ent = (LIST_ENTRY_T *)prTxPktBuf;
    //INT32 i4Ret;
    UINT32 i;
        
    ASSERT(prDev != NULL);
    prMac = prDev->pv_priv;

    /* free tx packet buffer which is not processed */
    //MLIST_REMOVE(&prMac->rTxPktList, (LIST_ENTRY_T *)prTxPktBuf);
    MLIST_REMOVE(&prMac->rTxPktList, p_ent);
    prTxPktBuf = (PKT_BUFF_T *)p_ent;

    while(prTxPktBuf != NULL)
    {
        /* callback to free tx packet buffer */
        if(prTxPktBuf->pf_pkt_free != NULL)
        {
            /* callback to free tx packet */ 
            //i4Ret = prTxPktBuf->pf_pkt_free(prDev, prTxPktBuf);
            //ASSERT(i4Ret == NET_DRV_OK);
            VERIFY(prTxPktBuf->pf_pkt_free(prDev, prTxPktBuf) == NET_DRV_OK);
        }
        else
        {
            ERR_ETHER("tx callback free function is null\n"); 
            ASSERT(0);
        }
        //MLIST_REMOVE(&prMac->rTxPktList, (LIST_ENTRY_T *)prTxPktBuf);
        //p_ent = (LIST_ENTRY_T *)prTxPktBuf;
        MLIST_REMOVE(&prMac->rTxPktList, p_ent);
        prTxPktBuf = (PKT_BUFF_T *)p_ent;
    } 

    /* tx descriptor status reset */
    prTxDesc = (PMAC_TX_DESC_T)prMac->pu4TxDescBuf;
    for(i = 0; i < NUM_TX_DESC; i++)
    {
        prTxDesc->rTxCtrl.Bits.fgOwn = OWNED_BY_CPU; 
        prTxDesc->rTxCtrl.Bits.u2BufSize = 0; 
        prTxDesc->u4BufAddr = 0; 
        prTxDesc ++;
    }

    /* rx descriptor status reset */
    prRxDesc = (PMAC_RX_DESC_T)prMac->pu4RxDescBuf;
    for(i = 0; i < NUM_RX_DESC; i++)
    {
        prRxDesc->rRxCtrl.Bits.u2BufSize = PKT_BUFFER_SIZE;
        prRxDesc->rRxCtrl.Bits.fgOwn = OWNED_BY_DMA;
        prRxDesc ++;
    }
        
    /* others */
    prMac->u4IrqStatus = 0;
    //prMac->u4McListNum = 0;
    //prMac->u4DevStatus &= (ETHER_DEV_INIT | ETHER_DEV_START | ETHER_LINK_UP);
    //CLR_ETHER_INFO(prMac, (ETHER_DEV_RDY | RX_TO_RECV_PKT | RX_BUF_OVERRUN));
    CLR_ETHER_INFO(prMac, (RX_TO_RECV_PKT | RX_BUF_OVERRUN));
    //x_memset(&prMac->rStat, 0x00, sizeof(MAC_STAT_T));

    i4HalSyncDescIdx(prMac);
    
    //vEtherModuleInit(prDev);
}

/*******************************************************************************
**  vEtherLinkCheck
**
**  descriptions:
**      This routine is called to check ethernet link status
**  parameters:
**      prDev - pointer to net device structure
**  return:
*******************************************************************************/
void vEtherLinkCheck(NET_DRV_IF_T *prDev)
{
    INT32 i4Ret;
    UINT32 u4Cnt = 0;
    PETHER_ADAPTER_T prMac;
    PMAC_MII_T prMii;
    static BOOL _fgFullDup, _fgSpeed100;
    
    ASSERT(prDev != NULL);
    prMac = prDev->pv_priv;
    prMii = &prMac->rMii;

    _fgFullDup = prMii->fgFullDup;
    _fgSpeed100 = prMii->fgSpeed100;
    i4Ret = i4HalPhyStatusChk(prMac);
    if(i4Ret != NET_DRV_OK)
        return;

    if(!GET_ETHER_INFO(prMac, ETHER_DEV_RDY))
    {
        if(GET_ETHER_INFO(prMac, ETHER_LINK_UP))
        {
        	/* for UI= "OFF" , plug in*/
        	if (!GET_ETHER_INFO(prMac, ETHER_DEV_START))  //CM add 20090505
        	{
				return;
        	}
            /* continue to confirm status in 50ms, about 2-3 link pluse */
            do{
                x_thread_delay(10);
                i4Ret = i4HalPhyStatusChk(prMac);
                if((i4Ret == NET_DRV_OK) && (!GET_ETHER_INFO(prMac, ETHER_LINK_UP)))
                {
                    INFO_ETHER("link up is detected but status is unstable\n");
                    return;
                }
            } while(++u4Cnt < 5);
            INFO_ETHER("cable is connected\n");
            
            /* wait stable after plugging in, it takes 1000ms in HF/SZ but only 200ms in TW!? */
            x_thread_delay(500);
            while(!GET_ETHER_INFO(prMac, ETHER_DEV_START))
            {
                INFO_ETHER("wait driver start command\n");
                x_thread_delay(100);
            } 

            if(i4HalPhyStatusChk(prMac) != NET_DRV_OK)
            {
                INFO_ETHER("PHY status is unstable\n");
                return;
            }
            else
            if(!GET_ETHER_INFO(prMac, ETHER_LINK_UP))
            {
                INFO_ETHER("cable connection is unstable\n");
                return;
            }
            /* PHY is confirmed to link up */
            if(!prMii->fgLpNwAble)
                INFO_ETHER("Link partner does not support auto-negotiation\n");
            INFO_ETHER("PHY %s\n", prMii->fgLinkUp ? "link up" : "no link");
            INFO_ETHER("%s-duplex mode\n", prMii->fgFullDup ? "Full" : "Half");
            INFO_ETHER("%s Mbps speed\n", prMii->fgSpeed100 ? "100" : "10");
            _fgFullDup = prMii->fgFullDup;
            _fgSpeed100 = prMii->fgSpeed100;
            if (!fgIsDMATxEnable())
			{
            vEtherDrvStart(prDev);        
            }	
            SET_ETHER_INFO(prMac, ETHER_DEV_RDY);
            INFO_ETHER("driver is ready for tx/rx\n");

            if(prDev->pf_ni_event_notify)
            {
                i4Ret = prDev->pf_ni_event_notify(prDev, NI_DRV_EV_ETHERNET_PLUGIN);	
            } 
        }
    }
    else
    {
        if(!GET_ETHER_INFO(prMac, ETHER_LINK_UP))
        {
            /* continue to confirm status in 50ms, about 2-3 link pluse */
            do{
                x_thread_delay(10);
                i4Ret = i4HalPhyStatusChk(prMac);
                if((i4Ret == NET_DRV_OK) && (GET_ETHER_INFO(prMac, ETHER_LINK_UP)))
                {
                    INFO_ETHER("link down is detected but status is unstable\n");
                    return;
                }
            } while(++u4Cnt < 5);

            /* clear RDY flag fist to prohibit from transmitting by upper layer */
            CLR_ETHER_INFO(prMac, ETHER_DEV_RDY);
            Printf("!!!Not Link-UP Stop EtherDrv\n");
            vEtherDrvStop(prDev);
            INFO_ETHER("cable is disconnected and driver is stopped\n");

            if(prDev->pf_ni_event_notify)
            {
                i4Ret = prDev->pf_ni_event_notify(prDev, NI_DRV_EV_ETHERNET_UNPLUG);	
            } 
            x_thread_delay(5);
            vEtherDrvSwReset(prDev);
        }
        else
        {
            /* print log when mode is changed during link up */
            if((prMii->fgFullDup != _fgFullDup) ||
               (prMii->fgSpeed100 != _fgSpeed100))
            {
                INFO_ETHER("%sMbps/%s duplex is detected instead of %sMbps/%s duplex\n", 
                            prMii->fgSpeed100 ? "100" : "10",
                            prMii->fgFullDup ? "Full" : "Half",
                            _fgSpeed100 ? "100" : "10",
                            _fgFullDup ? "Full" : "Half");
                _fgFullDup = prMii->fgFullDup;
                _fgSpeed100 = prMii->fgSpeed100;
            }
        }
    }

}


#ifdef  SCATTER_TEST	//only for verification

INT32 i4EtherTxmitFrame(PETHER_ADAPTER_T prMac, PKT_BUFF_T *prPkt)
{
    PMAC_TX_DESC_T prTxDesc;
   volatile PMAC_TX_DESC_CTRL_T prCtrl; 
#if CONFIG_ETHER_TX_NO_COPY
    UINT32 u4TxDescStatus;
#endif
    CRIT_STATE_T rState;
    UINT32 u4WaitCnt;
#if CONFIG_ETHER_HW_CHKSUM
    BOOL fgChksumCompleted;
#endif
    UINT32 u4TxDespCount=0, u4PktSize;
    UINT8 u1FirstFs=1, u1LastFs=0, i;

    ASSERT(prMac != NULL);
    ASSERT(prPkt != NULL);

    if(prPkt->ui4_len > ETHER_VALID_PKT_LEN)
    {
        return(NET_DRV_PKT_TOO_BIG);
    }
 #if 0  //kenny mark it for 8555 test
    else
    if(prPkt->ui4_len < 60)
    {
        return(NET_DRV_PKT_TOO_SMALL);
    }
 #endif   
    else
    if(prPkt->pui1_data == NULL)
    {
        return(NET_DRV_PKT_ERROR);
    }

   //Cout how many decriptor is need
   u4PktSize = prPkt->ui4_len;
   
   while( u4PktSize > 0)
   {
     u4TxDespCount++;
     
    if(u4PktSize >=PKT_BUFFER_SIZE)
    u4PktSize -= 	PKT_BUFFER_SIZE;
    else
    u4PktSize =0;
   
   }

  

   
    ASSERT(prMac->rTxPktList.ui4_count < NUM_TX_DESC);
    MLIST_INSERT(&prMac->rTxPktList, (LIST_ENTRY_T *)&prPkt->t_link);


    u4PktSize = prPkt->ui4_len;
    u1FirstFs =1;
    u1LastFs =0;
    i=0;
   while(u4TxDespCount >0) 
   {
    
      prTxDesc = prMac->prTxDesc + prMac->u4TxIdx;
     prCtrl =  &prTxDesc->rTxCtrl;
     u4WaitCnt = 0;
    
    
        do{
#if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY)        
        PMAC_TX_DESC_T prPrevTxDesc;
#endif
        
        rState = x_crit_start();
        u4TxDescStatus = (UINT32)(prCtrl->Bits.fgOwn | prTxDesc->u4BufAddr);
        x_crit_end(rState);
        if(u4TxDescStatus == OWNED_BY_CPU)
        {
            /* current descriptor is available */
            break;
        }

        /* current descriptor is unavailable and check if previous one is also unavailable */
#if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY)        
        prPrevTxDesc = prTxDesc - 1;
        if((UINT32)prPrevTxDesc < (UINT32)prMac->pu4TxDescBuf)
        {
            prPrevTxDesc = prTxDesc + NUM_TX_DESC - 1;
        }
        if(!fgDescIsTxSent(prPrevTxDesc))
        {
            DBG_ETHER("tx descriptor count may be insufficient\n"); 
            x_thread_delay(1);
            if(++u4WaitCnt > 10)
            {
                WARN_ETHER("no descriptor for transmission\n"); 
                return(NET_DRV_ERROR);
            }
        }
#else
		DBG_ETHER("tx descriptor count may be insufficient\n"); 
		x_thread_delay(1);
		HalFlushInvalidateDCache();
       	if(++u4WaitCnt > 10)
        {
        	WARN_ETHER("no descriptor for transmission\n"); 
            return(NET_DRV_ERROR);
      	}
#endif
    } while(1);
        
    /* move index to next free descriptor */
    if((++prMac->u4TxIdx) >= NUM_TX_DESC)     
    {
        prMac->u4TxIdx = 0;
    }    
    
   
    rState = x_crit_start();
    #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
   // if(u4TxDespCount> 1)
    prTxDesc->u4BufAddr = PHYSICAL((UINT32)prPkt->pui1_data)+i*PKT_BUFFER_SIZE;//kenny
   // else
    //   prTxDesc->u4BufAddr = PHYSICAL((UINT32)prPkt->pui1_data)+i*PKT_BUFFER_SIZE+u4PktSize;//kenny	

//   Printf("prPkt->pui1_data =0x%x, prTxDesc->u4BufAddr =0x%x\n", PHYSICAL((UINT32)prPkt->pui1_data), prTxDesc->u4BufAddr);
    
    #else
     prTxDesc->u4BufAddr = (UINT32)prPkt->pui1_data;
    #endif

     
    if(u4TxDespCount> 1)
    prTxDesc->rTxCtrl.Bits.u2BufSize = PKT_BUFFER_SIZE;    
    else
    prTxDesc->rTxCtrl.Bits.u2BufSize = u4PktSize;    
    
    // Set TX descriptor for transmission   
    prCtrl->Bits.fgOwn = OWNED_BY_DMA;
     
   if(u4TxDespCount> 1)
   {

   }
   
    
    prCtrl->Bits.fgFirstSeg = u1FirstFs;
    
    prCtrl->Bits.fgLastSeg = u1LastFs;

    prCtrl->Bits.fgTCO =1;
    prCtrl->Bits.fgUCO =1;
    prCtrl->Bits.fgICO =1;
    
   x_crit_end(rState);
     #if CONFIG_ETHER_HW_CHKSUM
    if(!prPkt->t_hw_chksum.b_hw_chksum_en)
    {
        HalFlushInvalidateDCache();   // take more time than HalFlushDCache()
    }    
    else
    {
        /* do nothing. Cache was already flushed and invalidated while processing hw checksum */
    }    
#else
    HalFlushInvalidateDCache();   // take more time than HalFlushDCache()
#endif
       /* RISC write cache address(tx buffer) and HW read RAMBUF data */
    /* suppose nobody will modify the cache data while HW DMA is processed */
    //HalFlushDCache();
#if CONFIG_DRV_MT8520
    BIM_WAIT_WALE();
#endif


     u4TxDespCount--;
     if(u4PktSize>=PKT_BUFFER_SIZE)
     u4PktSize = u4PktSize - PKT_BUFFER_SIZE;
     else
     u4PktSize =0;	

     u1FirstFs =0;
     if(u4TxDespCount ==1)
     u1LastFs =1;	

     i++;
   }
    
 

    /* Trigger DMA engin to process TX */
    vDmaTxResume(); 

    return(NET_DRV_OK);
}

#else
/*******************************************************************************
**  i4EtherTxmitFrame
**
**  descriptions:
**      This routine is called to send ethernet frame
**  parameters:
**      prMac - pointer to adapter struct
**      prPkt - pointer to packet buffer struct
**  result:
**  return:
**      NET_DRV_OK
**      NET_DRV_ERROR, if try to transmit too long
*******************************************************************************/
INT32 i4EtherTxmitFrame(PETHER_ADAPTER_T prMac, PKT_BUFF_T *prPkt)
{
    PMAC_TX_DESC_T prTxDesc;
   volatile PMAC_TX_DESC_CTRL_T prCtrl; 
#if CONFIG_ETHER_TX_NO_COPY
    UINT32 u4TxDescStatus;
#endif
    CRIT_STATE_T rState;
    UINT32 u4WaitCnt;
#if CONFIG_ETHER_HW_CHKSUM
    BOOL fgChksumCompleted;
#endif
    
    ASSERT(prMac != NULL);
    ASSERT(prPkt != NULL);

    if(prPkt->ui4_len > PKT_BUFFER_SIZE)
    {
        return(NET_DRV_PKT_TOO_BIG);
    }
 #if 0  //kenny mark it for 8555 test
    else
    if(prPkt->ui4_len < 60)
    {
        return(NET_DRV_PKT_TOO_SMALL);
    }
 #endif   
    else
    if(prPkt->pui1_data == NULL)
    {
        return(NET_DRV_PKT_ERROR);
    }

    prTxDesc = prMac->prTxDesc + prMac->u4TxIdx;
    prCtrl =  &prTxDesc->rTxCtrl;
    u4WaitCnt = 0;
    
    do{
#if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY)        
        PMAC_TX_DESC_T prPrevTxDesc;
#endif

        rState = x_crit_start();
        u4TxDescStatus = (UINT32)(prCtrl->Bits.fgOwn | prTxDesc->u4BufAddr);
        x_crit_end(rState);
        if(u4TxDescStatus == OWNED_BY_CPU)
        {
            /* current descriptor is available */
            break;
        }

        /* current descriptor is unavailable and check if previous one is also unavailable */
#if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY)        
        prPrevTxDesc = prTxDesc - 1;
        if((UINT32)prPrevTxDesc < (UINT32)prMac->pu4TxDescBuf)
     	{
            prPrevTxDesc = prTxDesc + NUM_TX_DESC - 1;
     	}
        if(!fgDescIsTxSent(prPrevTxDesc))
        {
            DBG_ETHER("tx descriptor count may be insufficient\n"); 
        	x_thread_delay(1);
            if(++u4WaitCnt > 20)
            {
                DBG_ETHER("no descriptor for transmission\n"); 
                return(NET_DRV_ERROR);
            }
        }
#else
		DBG_ETHER("tx descriptor count may be insufficient\n"); 
		x_thread_delay(1);
		HalFlushInvalidateDCache();
       	
       	if(++u4WaitCnt > 10)
        {
        	WARN_ETHER("no descriptor for transmission\n"); 
            return(NET_DRV_ERROR);
      	}
#endif
    } while(1);

    /* move index to next free descriptor */
    if((++prMac->u4TxIdx) >= NUM_TX_DESC)     
    {
        prMac->u4TxIdx = 0;
    }    

#if CONFIG_ETHER_TX_NO_COPY
   // HalFlushInvalidateDCache();//kenny add test
    
    ASSERT(prMac->rTxPktList.ui4_count < NUM_TX_DESC);
    MLIST_INSERT(&prMac->rTxPktList, (LIST_ENTRY_T *)&prPkt->t_link);

    rState = x_crit_start();
    #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    prTxDesc->u4BufAddr = PHYSICAL((UINT32)prPkt->pui1_data);//kenny
    #else
    prTxDesc->u4BufAddr = (UINT32)prPkt->pui1_data;    
    #endif
    
	/* init tx descriptor */
    if (1)
	{
	   // prCtrl->u4Val &= 0x0000ffff;
		prCtrl->Bits.fgVTG = 0;   // each tx buffer is big enough for transmission
		prCtrl->Bits.fgINSV = 0;    // 
		//prCtrl->Bits.fgTxIntOn = TRUE;
		//prCtrl->Bits.fgOwn = OWNED_BY_CPU;
    }

    prTxDesc->rTxCtrl.Bits.u2BufSize = (UINT16)prPkt->ui4_len;    
    // Set TX descriptor for transmission   
    
    if(_fgSupportTxChkSumOffload)
    {
    prCtrl->Bits.fgTCO =1;
    prCtrl->Bits.fgUCO =1;
    prCtrl->Bits.fgICO =1;
    }
    
	
	if(_TxDesCtrlBitOnfgVTG) 
		prCtrl->Bits.fgVTG = _TxDesCtrlfgVTG;

	if(_TxDesCtrlBitOnfgTCO) 
		prCtrl->Bits.fgTCO = _TxDesCtrlfgTCO;

	if(_TxDesCtrlBitOnfgUCO) 
		prCtrl->Bits.fgUCO = _TxDesCtrlfgUCO;

	if(_TxDesCtrlBitOnfgICO) 
		prCtrl->Bits.fgICO = _TxDesCtrlfgICO;

	if(_TxDesCtrlBitOnfgINSV) 
		prCtrl->Bits.fgINSV = _TxDesCtrlfgINSV;


	if(_TxDesCtrlBitOnfgTxIntOn) 
		prCtrl->Bits.fgTxIntOn = _TxDesCtrlfgTxIntOn;


	 if(_TxDesCtrlBitOnfgLastSeg) 
		 prCtrl->Bits.fgLastSeg = _TxDesCtrlfgLastSeg;
	 
	 if(_TxDesCtrlBitOnfgFirstSeg) 
		 prCtrl->Bits.fgFirstSeg = _TxDesCtrlfgFirstSeg;
	 
	 if(_TxDesCtrlBitOnfgEndOfRing) 
		 prCtrl->Bits.fgEndOfRing = _TxDesCtrlfgEndOfRing;

	if (_TxDesCtrlBitOnfgVTG || _TxDesCtrlBitOnfgTCO ||
		_TxDesCtrlBitOnfgUCO || _TxDesCtrlBitOnfgICO ||
		_TxDesCtrlBitOnfgINSV || _TxDesCtrlBitOnfgTxIntOn ||
		_TxDesCtrlBitOnfgLastSeg || _TxDesCtrlBitOnfgFirstSeg ||
		_TxDesCtrlBitOnfgEndOfRing)
	{
		UTIL_Printf("Tx Desc: 0x%08X\n", prCtrl->u4Val);
		//HalFlushInvalidateDCache(); //Before Read Tx Descriptor //instead of map/unmap
	}

	 
	 //   UINT32   fgVTG        			: 1;    	// Tx packet is VLAN tagged
	  ///  UINT32   fgTCO       			: 1;    	// TCP checksum generation offload
	  ///  UINT32   fgUCO         			: 1;    	// UDP checksum generation offload   
	  //  UINT32   fgICO         			: 1;   		// IP checksum generation offload
	  //  UINT32   fgINSV         		: 1;    	// insert VLAN tag
	   // UINT32   fgTxIntOn       		: 1;    	// interrupt enable after sending out packet
	  //  UINT32   fgLastSeg          	: 1;   		// Tx - Last segment of the frame 
	  //  UINT32   fgFirstSeg           	: 1;    	// Tx - First segment of the frame  
	  //  UINT32   fgEndOfRing            : 1;    	// End of descriptors ring  

    prCtrl->Bits.fgOwn = OWNED_BY_DMA;
    
#if CONFIG_ETHER_HW_CHKSUM
    fgChksumCompleted = prPkt->t_hw_chksum.b_is_completed;
#endif
    x_crit_end(rState);

#if CONFIG_ETHER_HW_CHKSUM
    if(!prPkt->t_hw_chksum.b_hw_chksum_en)
    {
        HalFlushInvalidateDCache();   // take more time than HalFlushDCache()
    }    
    else
    {
        /* do nothing. Cache was already flushed and invalidated while processing hw checksum */
    }    
#else
    HalFlushInvalidateDCache();   // take more time than HalFlushDCache()
#endif

#else /* !CONFIG_ETHER_TX_NO_COPY */
    x_memcpy((UINT8*)prTxDesc->u4BufAddr, prPkt->pui1_data, prPkt->ui4_len);
    prTxDesc->rTxCtrl.Bits.u2BufSize = (UINT16)prPkt->ui4_len;    
    // Set TX descriptor for transmission   
    prCtrl->Bits.fgOwn = OWNED_BY_DMA;
#endif
    
#if CONFIG_ETHER_HW_CHKSUM
    if(prPkt->t_hw_chksum.b_hw_chksum_en)
    {
        UINT32 u4DbgCnt = 0;
        while(!fgChksumCompleted)
        {
            /* wait HW checksum completion */
            rState = x_crit_start();
            fgChksumCompleted = prPkt->t_hw_chksum.b_is_completed;
            x_crit_end(rState);
            u4DbgCnt ++;
        }
        if(u4DbgCnt)
        {
            /* flush cache again since checksum value of payload is updated */
            HalFlushInvalidateDCache();
            //#if CONFIG_DRV_MT8520
            //	BIM_WAIT_WALE();
            //#endif
            WARN_ETHER("HW checksum wait count = %d\n", u4DbgCnt); 
        }
    }
#endif

	if(!fgDMAPollingEn)
	{
    vDmaTxResume();                               
	}


    return(NET_DRV_OK);
}
#endif //#ifdef  SCATTER_TEST	

#ifdef  SCATTER_TEST	
void vEtherSendComplete(NET_DRV_IF_T *prDev)
{
    PETHER_ADAPTER_T prMac;
    PMAC_TX_DESC_T prTxDesc;
    PMAC_STAT_T prStat;
    INT32 i4Ret;
    PKT_BUFF_T *prTxPktBuf = NULL;
    LIST_ENTRY_T *p_ent = (LIST_ENTRY_T *)prTxPktBuf;
    BOOL fgIsTxSent, fgIsTxValid = 1, fgEnd=0;
    CRIT_STATE_T rState;
    
    ASSERT(prDev != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;

    prTxDesc = prMac->prTxDesc + prMac->u4TxCompIdx;   // Get TX descriptor address
    prStat = &prMac->rStat;                            // Get TX descriptor statistics

#if CONFIG_DRV_LINUX_DATA_CONSISTENCY 
	HalFlushInvalidateDCache(); //Before Read Tx Descriptor
#endif

    rState = x_crit_start();
    fgIsTxSent = (BOOL)fgDescIsTxSent(prTxDesc);
    x_crit_end(rState);

    /* reset packet and octect service count */
    prStat->u4TxFrameSrv = 0;
    prStat->u4TxOctetSrv = 0;

   
        

    while((fgIsTxSent == TRUE)&&(!fgEnd))
    {
        if(fgIsTxValid)                                 // check TX result ok or not
        {
            /* tx packet is valid */
            prStat->u4TxFrame ++;
            prStat->u4TxOctet += prTxDesc->rTxCtrl.Bits.u2BufSize;
        }
        else
        {
            DBG_ETHER("Txmit packet is invalid \n"); 
            prStat->u4TxErr ++;
            //prStat->u4TxCarErr += (prStatus->Bits.fgNoCarrier | prStatus->Bits.fgLostCarrier);
            //prStat->u4TxExcessColErr += prStatus->Bits.fgExcCol;
            //prStat->u4TxLateColErr += prStatus->Bits.fgLateCol;
            //prStat->u4TxExcessDefer += prStatus->Bits.fgExcDeferral; 
        }
        prStat->u4TxFrameSrv ++;
        prStat->u4TxOctetSrv += prTxDesc->rTxCtrl.Bits.u2BufSize;

         if(prTxDesc->rTxCtrl.Bits.fgLastSeg)
          fgEnd =1;	
        
        
                /* re-init Tx descriptor */
        rState = x_crit_start();
        prTxDesc->rTxCtrl.Bits.fgOwn = OWNED_BY_CPU; 
        prTxDesc->rTxCtrl.Bits.u2BufSize = 0;
        prTxDesc->u4BufAddr = 0; 
        prTxDesc->rTxCtrl.Bits.fgLastSeg = 0; 
        prTxDesc->rTxCtrl.Bits.fgFirstSeg = 0; 
        x_crit_end(rState);

        

        /* Continue to parse the next descriptor */
        if((++prMac->u4TxCompIdx) >= NUM_TX_DESC)     
        {
            prMac->u4TxCompIdx = 0;
        }
        prTxDesc = prMac->prTxDesc + prMac->u4TxCompIdx;   // Get TX descriptor address

       
        	
        rState = x_crit_start();
        fgIsTxSent = fgDescIsTxSent(prTxDesc);
        x_crit_end(rState);
    }

      //MLIST_REMOVE(&prMac->rTxPktList, (LIST_ENTRY_T *)prTxPktBuf);
      MLIST_REMOVE(&prMac->rTxPktList, p_ent);
      prTxPktBuf = (PKT_BUFF_T *)p_ent;
  //    ASSERT((UINT32)prTxPktBuf->pui1_data == VIRTUAL(prTxDesc->u4BufAddr));
      

      /* callback to free tx packet buffer */
        if(prTxPktBuf->pf_pkt_free != NULL)
        {
            /* callback to free tx packet */ 
            i4Ret = prTxPktBuf->pf_pkt_free(prDev, prTxPktBuf);
            ASSERT(i4Ret == NET_DRV_OK);
        }
        else
        {
            ERR_ETHER("tx callback free function is null\n"); 
            ASSERT(0);
        }


#if CONFIG_DRV_LINUX_DATA_CONSISTENCY 
	HalFlushInvalidateDCache(); //For fgDescIsTxSent(prTxDesc)
#endif
}

#else
/*******************************************************************************
**  vEtherSendComplete
**
**  descriptions:
**      This routine is called when tx completion interrupt is triggered.
**      It will gather packet transmission statistics information.
**  parameters:
**      prDev - pointer to struct net_device
**  result:
**      Service and free tx packets until no descriptor is owned by DMA
**  return:
**      none
*******************************************************************************/
void vEtherSendComplete(NET_DRV_IF_T *prDev)
{
    PETHER_ADAPTER_T prMac;
    PMAC_TX_DESC_T prTxDesc;    
    PMAC_STAT_T prStat;
    //INT32 i4Ret;
    PKT_BUFF_T *prTxPktBuf = NULL;
    LIST_ENTRY_T *p_ent = (LIST_ENTRY_T *)prTxPktBuf;
    BOOL fgIsTxSent, fgIsTxValid = 1;
    CRIT_STATE_T rState;

    ASSERT(prDev != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;

    prTxDesc = prMac->prTxDesc + prMac->u4TxCompIdx;   // Get TX descriptor address    
    prStat = &prMac->rStat;                            // Get TX descriptor statistics

#if CONFIG_DRV_LINUX_DATA_CONSISTENCY 
	HalFlushInvalidateDCache(); //Before Read Tx Descriptor
#endif

    rState = x_crit_start();
    fgIsTxSent = (BOOL)fgDescIsTxSent(prTxDesc);
    //fgIsTxValid = (BOOL)fgDescIsTxValid(prStatus);//cm mark
    x_crit_end(rState);

    /* reset packet and octect service count */
    prStat->u4TxFrameSrv = 0;
    prStat->u4TxOctetSrv = 0;
//  Printf("$");
    while((fgIsTxSent == TRUE)
          && (prStat->u4TxOctetSrv < MAX_TXMIT_BYTE_CNT)
          && (prStat->u4TxFrameSrv < NUM_TX_DESC))
    {
        if(fgIsTxValid)                                 // check TX result ok or not
        {
            /* tx packet is valid */
            prStat->u4TxFrame ++;
            prStat->u4TxOctet += prTxDesc->rTxCtrl.Bits.u2BufSize;
        }
        else
        {	
            DBG_ETHER("Txmit packet is invalid \n"); 
            prStat->u4TxErr ++;
            //prStat->u4TxCarErr += (prStatus->Bits.fgNoCarrier | prStatus->Bits.fgLostCarrier);
            //prStat->u4TxExcessColErr += prStatus->Bits.fgExcCol;
            //prStat->u4TxLateColErr += prStatus->Bits.fgLateCol;
            //prStat->u4TxExcessDefer += prStatus->Bits.fgExcDeferral; 
        }
        prStat->u4TxFrameSrv ++;
        prStat->u4TxOctetSrv += prTxDesc->rTxCtrl.Bits.u2BufSize;
        
        //MLIST_REMOVE(&prMac->rTxPktList, (LIST_ENTRY_T *)prTxPktBuf);
        MLIST_REMOVE(&prMac->rTxPktList, p_ent);
        prTxPktBuf = (PKT_BUFF_T *)p_ent;
        //ASSERT((UINT32)(prTxPktBuf->pui1_data) == VIRTUAL(prTxDesc->u4BufAddr));//Kenny temply mark
        
        /* re-init Tx descriptor */
        rState = x_crit_start();
        prTxDesc->rTxCtrl.Bits.fgOwn = OWNED_BY_CPU; 
        prTxDesc->rTxCtrl.Bits.u2BufSize = 0;
        prTxDesc->u4BufAddr = 0; 
        x_crit_end(rState);

        /* callback to free tx packet buffer */
        if(prTxPktBuf->pf_pkt_free != NULL)
        {
            /* callback to free tx packet */ 
            //i4Ret = prTxPktBuf->pf_pkt_free(prDev, prTxPktBuf);
            //ASSERT(i4Ret == NET_DRV_OK);
            VERIFY(prTxPktBuf->pf_pkt_free(prDev, prTxPktBuf) == NET_DRV_OK);
    	}
    	else
    	{
            ERR_ETHER("tx callback free function is null\n"); 
            ASSERT(0);
        }

        /* Continue to parse the next descriptor */
        if((++prMac->u4TxCompIdx) >= NUM_TX_DESC)     
        {
            prMac->u4TxCompIdx = 0;
        }
        prTxDesc = prMac->prTxDesc + prMac->u4TxCompIdx;   // Get TX descriptor address
        
        rState = x_crit_start();
        fgIsTxSent = fgDescIsTxSent(prTxDesc);
        //fgIsTxValid = fgDescIsTxValid(prStatus);//cm mark
        x_crit_end(rState);
    }
#if CONFIG_DRV_LINUX_DATA_CONSISTENCY 
	HalFlushInvalidateDCache(); //For fgDescIsTxSent(prTxDesc)
#endif
}

#endif//#ifdef  SCATTER_TEST	


#ifdef SCATTER_TEST  //only for verification
void vEtherRecvPacket(NET_DRV_IF_T *prDev)
{
    PETHER_ADAPTER_T prMac;
    PMAC_RX_DESC_T prRxDesc;
#if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY)  
    PMAC_RX_DESC_T prPrevRxDesc;
#endif
    volatile PMAC_RX_DESC_CTRL_T prCtrl;
    PMAC_STAT_T prStat;
    UINT32 u4Len, u4DescCnt;
    UCHAR *pucPktPtr;
    //UCHAR *puChkSum;
    PKT_BUFF_T *prPktBuf;    
    INT32 i4Ret;
    UINT32 u4Index, u4Index1;
    UINT8 *prData;
    
    ASSERT(prDev != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;

    prRxDesc = prMac->prRxDesc + prMac->u4RxIdx;
    prCtrl = &prRxDesc->rRxCtrl;
    prStat = &prMac->rStat;

#if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    /* check if previous descriptor is owned by host */
    prPrevRxDesc = prRxDesc - 1;
    if((UINT32)prPrevRxDesc < (UINT32)prMac->pu4RxDescBuf)
    {
        prPrevRxDesc = prRxDesc + NUM_RX_DESC - 1;
    }
    if((prPrevRxDesc->rRxCtrl.Bits.fgOwn == OWNED_BY_CPU) && (!GET_ETHER_INFO(prMac, RX_BUF_OVERRUN)))
    {
        DBG_ETHER("rx descriptor count may be insufficient\n"); 
    }
#endif
    /* reset packet and octect service count */
    prStat->u4RxFrameSrv = 0;
    prStat->u4RxOctetSrv = 0;
   
    while(prCtrl->Bits.fgOwn ==  OWNED_BY_CPU) 
    {
        if(GET_ETHER_INFO(prMac, RX_BUF_OVERRUN) && (prMac->u4RxIdx == prMac->u4RxDescNoBufIdx))
        {
            /* no more rx buffer for reception */
            ERR_ETHER("no more rx buffer for reception\n"); 
            CLR_ETHER_INFO(prMac, RX_TO_RECV_PKT);
            return;
        }        

        /* process one descriptor each time when input packet is valid */
        u4DescCnt = 1;

        if(!(prCtrl->Bits.fgOversizeErr) && (!(prCtrl->Bits.fgCrcErr)))// process good packets only 
        {
           // Printf("g.");
            /* get header of the packet buffer */
            #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
            pucPktPtr = (UINT8 *) VIRTUAL(prRxDesc->u4BufAddr);//kenny
            #else
            pucPktPtr = (UINT8 *)prRxDesc->u4BufAddr;
            #endif
            prPktBuf = (PKT_BUFF_T *)((UINT32)pucPktPtr - prMac->u4RxPktHdrSize); 

            ASSERT(prPktBuf->pui1_data == pucPktPtr);
            prPktBuf->t_chksum_info.e_chksum_flag = CHKSUM_NONE;
             

            if(prCtrl->Bits.fgFirstSeg ==1)
           {
              Printf("!!!Packet Start \n");
              Printf("!!!Packet Total len= %d \n", prCtrl->Bits.u2BufSize);
              Printf("==== First Segment======\n");
              u4Len = PKT_BUFFER_SIZE;
               
           }
           else
          {
           u4Len = prCtrl->Bits.u2BufSize;
           }

            if(prCtrl->Bits.fgLastSeg ==1)
           {
           
              Printf("==== Last Segment======\n");
              Printf("!!!Packet End \n");
           }
            
            /* prepare PKT_BUFF_T data to indicate upper layer */
            prPktBuf->ui2_refcnt = 1;
            prPktBuf->ui4_len = u4Len;
            prPktBuf->pf_pkt_free = (pkt_free_fnct) i4EtherRxPacketFree;
            prPktBuf->ui4_time_stamp = 0; //u4NetGetTimeTick();    

            
            Printf("Segmemt length =%d\n", prPktBuf->ui4_len);
            
            if(prDev->pf_ni_indicate != NULL)
            {
                i4Ret = prDev->pf_ni_indicate(prDev, prPktBuf);	
            } 
            else
            {
                /* indicate rx to ourself when callback function is null */
               // i4Ret = i4EtherIndicateFrame(prDev, prPktBuf);   
                prData = prPktBuf->pui1_data;
               for(u4Index =0; u4Index< u4Len ; u4Index+=16 )
             	{
             	   if((u4Index+15)< u4Len)
             	   {
             	      Printf("Data index %d= 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x \n", u4Index,  \
             	   	*(prData+u4Index),  \
             	   	*(prData+u4Index+1), \
             	   	*(prData+u4Index+2), \
             	   	*(prData+u4Index+3), \
             	   	*(prData+u4Index+4), \
             	   	*(prData+u4Index+5), \
             	   	*(prData+u4Index+6), \
             	   	*(prData+u4Index+7) ,\
                     *(prData+u4Index+8) ,\
                     *(prData+u4Index+9) ,\
                     *(prData+u4Index+10) ,\
                     *(prData+u4Index+11) ,\
                     *(prData+u4Index+12) ,\
                     *(prData+u4Index+13) ,\
                     *(prData+u4Index+14) ,\
                     *(prData+u4Index+15) 
             	   
             	   	);
             	   }
                 else
                 {
                    Printf("Data index %d= ", u4Index);
                    for(u4Index1 =0; u4Index1< (u4Len-u4Index) ; u4Index1++ )
                    	{
                       Printf("0x%x ", *(prData+u4Index+u4Index1) );
                       
                    	}
                    Printf("\n");
                    

                 }	
               }
               	
                 /* free rx buffer by ourself when callback function is null */
                VERIFY(prPktBuf->pf_pkt_free(prDev, (PKT_BUFF_T*)prPktBuf) == NET_DRV_OK); 
                i4Ret = NET_DRV_OK; 
            }
            
            
            if(i4Ret != NET_DRV_OK)
            {
                INFO_ETHER("indicate packet fail 0x%04x\n", i4Ret); 
            }    
            
            
            
            //NOTE_ETHER("."); 
            pucPktPtr = pucEtherRxPacketAlloc(prMac);   
            if(pucPktPtr != NULL)                      
            {
                /* assign packet data buffer address and set own bit for DMA */
                #if CONFIG_DRV_LINUX_DATA_CONSISTENCY    //kenny   
                prRxDesc->u4BufAddr = PHYSICAL((UINT32)pucPktPtr); 
                #else
                prRxDesc->u4BufAddr = (UINT32)pucPktPtr; 
                #endif
                
                INIT_RX_DESC(prCtrl);
            }   
            prStat->u8RxFrame ++;
            prStat->u8RxOctet += (UINT64)u4Len;
            
            prStat->u4RxFrameSrv ++;
            prStat->u4RxOctetSrv += u4Len;
        }
        /* rx descriptor is invalid */
        else
        {
            /* received packet is invalid */
            //DBG_ETHER("rx data is invalid 0x%04x\n", prCtrl->u4Val); 
            Printf("b.");
            Printf("rx data is invalid 0x%04x\n", prCtrl->u4Val); 
            prStat->u4RxErr ++;
            prStat->u4RxCrcErr += prCtrl->Bits.fgCrcErr;

            u4DescCnt = 0;
            do{
                /* set own bit for invalid descriptor */
                INIT_RX_DESC(prCtrl);
                Printf("C.");
                /* Continue to check next descriptor for giant(over 1518) frame */
                prRxDesc = (PMAC_RX_DESC_T)prRxDesc + 1; 
                prCtrl = (PMAC_RX_DESC_CTRL_T)&prRxDesc->rRxCtrl;
                u4DescCnt ++;
            } while((prCtrl->Bits.fgOwn) == OWNED_BY_CPU );    
        }
        prStat->u4RxLenErr += prCtrl->Bits.fgOversizeErr;

        /* Continue to parse the next descriptor */
        do{
            if((++prMac->u4RxIdx) >= NUM_RX_DESC)     
            {
                prMac->u4RxIdx = 0;
                /* only do this here once to get better performance */
                /* note: free packet buffer count must be greater or equal to descriptor count at this moment
                   so that every buffer hooked on descriptor next time will be flushed and invalidated.
                   Otherwise, it needs to flush and invalidate each time while allocating packet buffer */    
                HalFlushInvalidateDCache();           
            }    
        } while(--u4DescCnt > 0);
        
        if(prCtrl->Bits.fgLastSeg ==1)
        {
           prRxDesc = prMac->prRxDesc + prMac->u4RxIdx;   
           prCtrl= &prRxDesc->rRxCtrl;
          break;
        }
        else
       {
        prRxDesc = prMac->prRxDesc + prMac->u4RxIdx;   
        prCtrl= &prRxDesc->rRxCtrl;
        }
    }

    if(prCtrl->Bits.fgOwn == OWNED_BY_CPU)
    {
        /* have descriptors owned by HOST and need to receive it */
        SET_ETHER_INFO(prMac, RX_TO_RECV_PKT);  //For leave while loop when (prStat->u4RxOctetSrv >= MAX_RECV_BYTE_CNT)
    }
    else
    {
        /* no descriptor is owned by HOST */
        
        Printf("[NET]!!! CLR_ETHER_INFO RX_TO_RECV_PKT  \n");
        CLR_ETHER_INFO(prMac, RX_TO_RECV_PKT);
    }

#if CONFIG_DRV_LINUX_DATA_CONSISTENCY
   	HalFlushInvalidateDCache(); //For nxet time read prStatus->Bits.fgOwn and SET_RX_DESC_OWN(prStatus) 
#endif    
}

#else //Normal RecvPacket
/*******************************************************************************
**  vEtherRecvPacket
**
**  descriptions:
**      This routine is called service 'one' receiving packet.
**  parameters:
**      prDev - pointer to struct net_device
**  result:
**      It will gather packet receiving statistics information and upload packet to
**      upper layer. Next, allocate a new packet buffer to make the descriptor available.
**  return:
**      none
*******************************************************************************/
//static void vEtherRecvPacket(NET_DRV_IF_T *prDev)
void vEtherRecvPacket(NET_DRV_IF_T *prDev)
{
    PETHER_ADAPTER_T prMac;
    PMAC_RX_DESC_T prRxDesc;
#if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY)  
    PMAC_RX_DESC_T prPrevRxDesc;
#endif
    volatile PMAC_RX_DESC_CTRL_T prCtrl;
    volatile PMAC_RX_DESC_VLAN_T prVLAN;
    PMAC_STAT_T prStat;
    UINT32 u4Len, u4DescCnt;
    UCHAR *pucPktPtr;
    //UCHAR *puChkSum;
    PKT_BUFF_T *prPktBuf;    
    INT32 i4Ret;
    
    ASSERT(prDev != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;

    prRxDesc = prMac->prRxDesc + prMac->u4RxIdx;
    prCtrl = &prRxDesc->rRxCtrl;
	prVLAN =  &prRxDesc->rRxVlan;
    prStat = &prMac->rStat;
    	
#if (!CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    /* check if previous descriptor is owned by host */
    prPrevRxDesc = prRxDesc - 1;
    if((UINT32)prPrevRxDesc < (UINT32)prMac->pu4RxDescBuf)
    {
        prPrevRxDesc = prRxDesc + NUM_RX_DESC - 1;
    }
    if((prPrevRxDesc->rRxCtrl.Bits.fgOwn == OWNED_BY_CPU) && (!GET_ETHER_INFO(prMac, RX_BUF_OVERRUN)))
    {
        DBG_ETHER("rx descriptor count may be insufficient\n"); 
    }
#endif
    /* reset packet and octect service count */
    prStat->u4RxFrameSrv = 0;
    prStat->u4RxOctetSrv = 0;

    HalFlushInvalidateDCache(); //BD does not have this
   	
     while((prCtrl->Bits.fgOwn ==  OWNED_BY_CPU) 
           /*&& !(GET_ETHER_INFO(prMac, RX_BUF_OVERRUN) && (prMac->u4RxIdx == prMac->u4RxDescNoBufIdx))*/
           /*&& (!GET_ETHER_INFO(prMac, RX_BUF_OVERRUN))*/
          && (prStat->u4RxOctetSrv < MAX_RECV_BYTE_CNT) 
          /*&& (prStat->u4RxFrameSrv < MAX_RECV_PKT_CNT)*/) 
    {
      INT32  i4Show=0,j;

		if(fgPrintDes)
		{
		   i4Show = TRUE;
           UTIL_Printf("Ctrl word:\n\n");
		   
		   UTIL_Printf("u2BufSize      = %d ( Buffer size)\n",prCtrl->Bits.u2BufSize);
		   UTIL_Printf("fgL4ChecksumErr= %d ( TCP/UDP checksum error)\n",prCtrl->Bits.fgL4ChecksumErr);
		   UTIL_Printf("fgIPChecksumErr= %d ( IP checksum error )\n",prCtrl->Bits.fgIPChecksumErr);
		   UTIL_Printf("u1ProtocolSel  = %d ( Protocol select IP/TCP/UPD checksum select)\n",prCtrl->Bits.u1ProtocolSel);
		   UTIL_Printf("fgVlanPacket   = %d ( indicate VLAN tag packet )\n",prCtrl->Bits.fgVlanPacket);
		   UTIL_Printf("fgMyMacAddr    = %d ( Receive packet match My MAC addr.)\n",prCtrl->Bits.fgMyMacAddr); 
		   UTIL_Printf("fgHitHash      = %d ( Receive packet hit hash table)\n",prCtrl->Bits.fgHitHash);
           UTIL_Printf("fgRsvMultiAddr = %d ( Receive packet is reserved multicast addr.)\n",prCtrl->Bits.fgRsvMultiAddr);	 
           UTIL_Printf("fgCrcErr       = %d ( Receive packet is CRC error)\n",prCtrl->Bits.fgCrcErr);	
           UTIL_Printf("fgOversizeErr  = %d ( Receive packet is oversize error.)\n",prCtrl->Bits.fgOversizeErr);	
           UTIL_Printf("u1Reserved     = %d ( Reserved)\n",prCtrl->Bits.u1Reserved);	
           UTIL_Printf("fgLastSeg      = %d ( Rx - Last segment of the frame )\n",prCtrl->Bits.fgLastSeg);	
           UTIL_Printf("fgFirstSeg     = %d ( Rx - First segment of the frame )\n",prCtrl->Bits.fgFirstSeg);	
           UTIL_Printf("fgEndOfRing    = %d ( End of descriptors ring   )\n",prCtrl->Bits.fgEndOfRing);	
           UTIL_Printf("fgOwn          = %d ( Descriptor is owned by CPU engine   )\n",prCtrl->Bits.fgOwn);	

           UTIL_Printf("VLAN word:\n\n");

		   UTIL_Printf("u2VlanID      = %d ( VLAN tag ID)\n",prVLAN->u2VlanID);
		   UTIL_Printf("fgCFI      = %d ( VLAN tag CFI )\n",prVLAN->fgCFI);

		   UTIL_Printf("u1Priority      = %d ( VLAN tag priority)\n",prVLAN->u1Priority);
		   UTIL_Printf("u2EPID      = %d ( VLAN tag EPID )\n",prVLAN->u2EPID);
    
		   fgPrintDes-- ;
		}
        if(GET_ETHER_INFO(prMac, RX_BUF_OVERRUN) && (prMac->u4RxIdx == prMac->u4RxDescNoBufIdx))
        {
            /* no more rx buffer for reception */
            ERR_ETHER("no more rx buffer for reception\n"); 
            CLR_ETHER_INFO(prMac, RX_TO_RECV_PKT);
            return;
        }        

        /* process one descriptor each time when input packet is valid */
        u4DescCnt = 1;
        if(fgDescIsRxValid(prCtrl))           // process good packets only 
        {
            /* get header of the packet buffer */
            #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
            pucPktPtr = (UINT8 *) VIRTUAL(prRxDesc->u4BufAddr);//kenny
            #else
            pucPktPtr = (UINT8 *)prRxDesc->u4BufAddr;
            #endif
            prPktBuf = (PKT_BUFF_T *)((UINT32)pucPktPtr - prMac->u4RxPktHdrSize); 
             
            if(i4Show)
            {
                UTIL_Printf("RX Data: \n\n");
				for(j=0;j< prCtrl->Bits.u2BufSize;j++)
				{
				  if((j%16)==0)
                    UTIL_Printf("0x%04x ",j);
				 
				  UTIL_Printf("%02x ",*(pucPktPtr+j));
				  
				  if((j%16)==15)
				  	UTIL_Printf("\n");
				}
				UTIL_Printf("\n");
            }

            ASSERT(prPktBuf->pui1_data == pucPktPtr);
            if(IS_MAC_IP_CHKSUM())
            {
                u4Len = prCtrl->Bits.u2BufSize - MAC_IP_CHKSUM_LEN;
                
                prPktBuf->t_chksum_info.e_chksum_flag = CHKSUM_HW;
                 /* IP payload checksum is 16-bit located at the end of frame(after FCS) */
#if 0          //could be not in even boundary
                prPktBuf->t_chksum_info.ui2_ip_payload_chksum = \
                    *(UINT16*)((prRxDesc->u4BufAddr + prStatus->Bits.u2FrameLength) - MAC_IP_CHKSUM_LEN);
#else
                //puChkSum = (UINT8*)((prRxDesc->u4BufAddr + prCtrl->Bits.u2BufSize) - MAC_IP_CHKSUM_LEN);
                //prPktBuf->t_chksum_info.ui2_ip_payload_chksum = (*puChkSum << 8) | *(puChkSum+1);
#endif
            }    
            else
            {
                prPktBuf->t_chksum_info.e_chksum_flag = CHKSUM_NONE;
                u4Len = prCtrl->Bits.u2BufSize;
            }    
            
            /* prepare PKT_BUFF_T data to indicate upper layer */
            prPktBuf->ui2_refcnt = 1;
            prPktBuf->ui4_len = u4Len;
            prPktBuf->pf_pkt_free = (pkt_free_fnct) i4EtherRxPacketFree;
            prPktBuf->ui4_time_stamp = 0; //u4NetGetTimeTick();    

            if(prDev->pf_ni_indicate != NULL)
            {
                i4Ret = prDev->pf_ni_indicate(prDev, prPktBuf);	
            } 
            else
            {
                /* indicate rx to ourself when callback function is null */
                i4Ret = i4EtherIndicateFrame(prDev, prPktBuf);   
            } 
            if(i4Ret != NET_DRV_OK)
            {
                INFO_ETHER("indicate packet fail 0x%04x\n", i4Ret); 
            }    
            
            //NOTE_ETHER("."); 
            pucPktPtr = pucEtherRxPacketAlloc(prMac);   
            if(pucPktPtr != NULL)
            {
                /* assign packet data buffer address and set own bit for DMA */
                #if CONFIG_DRV_LINUX_DATA_CONSISTENCY    //kenny   
                prRxDesc->u4BufAddr = PHYSICAL((UINT32)pucPktPtr); 
                #else
                prRxDesc->u4BufAddr = (UINT32)pucPktPtr;
                #endif                
                INIT_RX_DESC(prCtrl);
            }   
            prStat->u8RxFrame ++;
            prStat->u8RxOctet += (UINT64)u4Len;
            
            prStat->u4RxFrameSrv ++;
            prStat->u4RxOctetSrv += u4Len;
        }
        /* rx descriptor is invalid */
        else
        {	
            /* received packet is invalid */
            //DBG_ETHER("rx data is invalid 0x%04x\n", prCtrl->u4Val); 
            //Printf("b.");
            //Printf("rx data is invalid 0x%04x\n", prCtrl->u4Val); 
            prStat->u4RxErr ++;
            prStat->u4RxCrcErr += prCtrl->Bits.fgCrcErr;

            u4DescCnt = 0;
            do{
                /* set own bit for invalid descriptor */
                INIT_RX_DESC(prCtrl);
                // Printf("C.");
                /* Continue to check next descriptor for giant(over 1518) frame */       	
                prRxDesc = (PMAC_RX_DESC_T)prRxDesc + 1; 
                prCtrl = (PMAC_RX_DESC_CTRL_T)&prRxDesc->rRxCtrl;
                u4DescCnt ++;
            } while(((prCtrl->Bits.fgOwn) == OWNED_BY_CPU) && 
                    (!(prCtrl->Bits.fgFirstSeg && prCtrl->Bits.fgLastSeg)));    
        }
        prStat->u4RxLenErr += prCtrl->Bits.fgOversizeErr;

        /* Continue to parse the next descriptor */       	
        do{
            if((++prMac->u4RxIdx) >= NUM_RX_DESC)     
        {
            prMac->u4RxIdx = 0;
                /* only do this here once to get better performance */
                /* note: free packet buffer count must be greater or equal to descriptor count at this moment
                   so that every buffer hooked on descriptor next time will be flushed and invalidated.
                   Otherwise, it needs to flush and invalidate each time while allocating packet buffer */    
                HalFlushInvalidateDCache();           
        }    
        } while(--u4DescCnt > 0);
        
        prRxDesc = prMac->prRxDesc + prMac->u4RxIdx;   
        prCtrl= &prRxDesc->rRxCtrl;
		prVLAN= &prRxDesc->rRxVlan;
    }

    if(prCtrl->Bits.fgOwn == OWNED_BY_CPU)
    {
        /* have descriptors owned by HOST and need to receive it */
        SET_ETHER_INFO(prMac, RX_TO_RECV_PKT);  //For leave while loop when (prStat->u4RxOctetSrv >= MAX_RECV_BYTE_CNT)
    }
    else
    {
        /* no descriptor is owned by HOST */
        CLR_ETHER_INFO(prMac, RX_TO_RECV_PKT);
    }

#if CONFIG_DRV_LINUX_DATA_CONSISTENCY
   	HalFlushInvalidateDCache(); //For nxet time read prStatus->Bits.fgOwn and SET_RX_DESC_OWN(prStatus) 
#endif    
}
#endif //#ifdef SCATTER_TEST //only for verification

/*******************************************************************************
**  vEtherDeferTask
**
**  descriptions:
**      This routine is rx defer task used to service serval rx interrupts
**  parameters:
**      prDev - pointer to struct net_device
**  result:
**      DMA rx interrupt is diabled in this routine to prevent rx interrupt 
**      is happened while handling rx receiving. The rx interrupt will be enabled 
**      before leaving this routine for next irq trigger.
**  return:
**      none
*******************************************************************************/
void vEtherDeferTask(NET_DRV_IF_T *prDev)
{
    PETHER_ADAPTER_T prMac;
    volatile UINT32 u4Irq;
    UINT32 u4LoopCnt;
    CRIT_STATE_T  t_state;

    ASSERT(prDev != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
    u4LoopCnt = 0;
    
    /* get latest interrupt status and clear it immediately to ensure no status is missed */
    t_state = x_crit_start();
    //u4Irq = u4DmaGetInt(DMA_INT_MASK) | prMac->u4IrqStatus;
    u4Irq = (u4DmaGetInt((DMA_INT_MASK|PORT_STATUS_CFG))| prMac->u4IrqStatus);
    vDmaIntClear(u4Irq);
    //Printf("[NET]!Defer Irq=0x%0x, IrqS =0x%x\n",u4Irq,  prMac->u4IrqStatus) ;
    x_crit_end(t_state);

    do{   
        /* rx service */
        if(u4Irq & DMA_RX_INT_MASK)
        {
           if (u4Irq & RX_FIFO_FULL)
            {
		 //NOTE_ETHER("Rx FIFO Full\n"); 
            }
           if (u4Irq & FNQF)
            {
		//NOTE_ETHER("Rx No Descriptor\n"); 
            }
           if((u4Irq & RX_FIFO_FULL)||(u4Irq & FNQF)) //kenny add
        {
                //Printf("Receive\n");
            vEtherRecvPacket(prDev);
                //vDmaRxResume();//kenny 2009/10/5 add it 
        } 
           else  if(u4Irq & FNRC)        
        {
                //Printf("FNRC\n");
            vEtherRecvPacket(prDev);
                //vDmaRxResume();//kenny 2009/10/5 add it 
            }
        }
        else
        if(GET_ETHER_INFO(prMac, RX_TO_RECV_PKT))
        {
            /* have descriptors owned by DMA and need to receive it */
             //Printf("[NET]!!Defer RX_TO_RECV_PKT\n");
            vEtherRecvPacket(prDev);
            //vDmaRxResume();//kenny 2009/10/5 add it 
            
        }

        /* tx service */
        if(u4Irq & DMA_TX_INT_MASK)
        {
            if (u4Irq & TX_FIFO_UNDER_RUN)
	    {
				NOTE_ETHER("Tx FIFO Under Run \n"); 
	    }
            if (u4Irq & TNQE)
	    {
				NOTE_ETHER("Tx No descriptor!!!\n"); 
	    }
             
            if(u4Irq & (TNTC | TX_FIFO_UNDER_RUN))	
            {
               vEtherSendComplete(prDev);
            }   
            else if (u4Irq & TNQE)
	    {
		  vEtherSendComplete(prDev);//kenny add
            }

        }

        if(u4Irq & PORT_STATUS_CFG)
        {
           vStopSoftPollingPhy(0);//enable Software Polling	
           Printf("[NET]Defer Link <-> down INT\n");	
        }
        if(++u4LoopCnt < MAX_INT_LOOP_CNT)
        //if(1)
        {
            /* get latest interrupt status and clear it immediately! */
            t_state = x_crit_start();
            u4Irq = u4DmaGetInt(DMA_INT_MASK);
            //Printf("[NET]Defer Last Irq= 0x%0x \n", u4Irq);
            vDmaIntClear(u4Irq);
            
            x_crit_end(t_state);
        }
        else
        {
            /* do not clear interrupt status and leave it to trigger BIM irq */
         
            break;
        }

    }while(u4Irq);      

    if(GET_ETHER_INFO(prMac, RX_TO_RECV_PKT))
    {
        /* send event to receive packet at next service */
        //Printf("[NET] Next RX_TO_RECV_PKT\n");
        ETHER_SET_EVENT(ETHER_EG_RECV_PKT);
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
        ETHER_SEMA_UNLOCK(_hEtherTaskSema);
#endif
    }
    	
    /* re-enable tx/rx interrupts */
    /* Normal and abnormal irq must also be cleared to make BIM interrupt available */
    //vDmaIntClear(DMA_INT_MASK);
    prMac->u4IrqStatus = 0;
    if(!fgDMAPollingEn)
        vDmaRxResume();//kenny 2009/10/5 add it 
    //vDmaIntEnable(DMA_INT_ENABLE);               
    vDmaIntEnable((DMA_INT_ENABLE|PORT_STATUS_CFG)); //Kenny test       
} 

void vWriteHashTable(void)
{
  int i, j;
  UINT8 u1Data;
  UINT32 u4Data;
  
  vWriteMAC(RW_HASH_CTRL, dReadMAC(RW_HASH_CTRL)|HASH_TABLE_BIST_START);
  x_thread_delay(1);
  //while(1)
 // {
  //   if((dReadMAC(RW_HASH_CTRL)&(HASH_TABLE_BIST_DONE|HASH_TABLE_BIST_OK)) == (HASH_TABLE_BIST_DONE|HASH_TABLE_BIST_OK))
    //   break;	
           	
 // }
        
  for(j=0; j<2; j++)
  {
     for(i=0; i < 256; i++)
    {
        if((u4HashTable[j][i/32]>>(i%32))&0x01)
    	   u1Data =1;
        else
          u1Data =0;	
        if(u1Data)
        u4Data=dReadMAC(RW_HASH_CTRL)|HASH_BIT_DATA|COMMAND_START|HASH_ACCESS_COMMAND|((j<<8)|i);
        else
        u4Data=dReadMAC(RW_HASH_CTRL)|COMMAND_START|HASH_ACCESS_COMMAND;

        u4Data= (u4Data&(~0x1ff))+((j<<8)|i);
        
        Printf("i =%d, j=%d, u4HashTable[%d][%d]=0x%x\n", i,j,j,i/32, u4HashTable[j][i/32]); 
        Printf("u1Data =%d, u4Data=0x%x\n", u1Data, u4Data);
        vWriteMAC(RW_HASH_CTRL, u4Data );        
     }
  }

  vWriteMAC(RW_HASH_CTRL, dReadMAC(RW_HASH_CTRL)& ~HASH_TABLE_BIST_START);
  
} 

void vEnableSupportTxCheckSumOffLoad(BOOL fgEnable)
{
  _fgSupportTxChkSumOffload = fgEnable; 	
}
