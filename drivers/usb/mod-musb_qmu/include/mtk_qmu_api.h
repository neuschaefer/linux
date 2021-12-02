/*! @file Mtk_qmu_api.h
*
*  @par Modification Notice: 
*
*  This software is modified by MediaTek Inc. and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
* $Workfile: Mtk_qmu_api.h$
*
* @par Project: 
*    Mtk usb swip
*
* @par Description: 
*    Mtk DMAQ module interface header file
*    Define DMAQ Atomic Function
*
* @par Author_Name: 
*    tianhao.fei 
*
* @par Last_Changed: add comment for Doxygen
* $Author: dtvbm11 $
* $Modtime: 2010-5-7$
* $Revision: #1 $
*
*/
#ifndef MTK_QMU_API_H
#define MTK_QMU_API_H

#ifdef CONFIG_USB_QMU_SUPPORT 

#include <linux/slab.h>


//#include <linux/dma-mapping.h>

#define LOG_QMU 4

/// @brief Define RxQ & TxQ number
/// .
/// @Author_Name:tianhao.fei 4/29/2010
/// @{
#define RXQ_NUM 4
#define TXQ_NUM 4
/// @}

/// @brief RxQ & TxQ for level 1 index of Mtk_USB_Result
/// .
/// @Author_Name:tianhao.fei 4/29/2010
/// @{
#define TXQ	0
#define RXQ	1
/// @}

#define mtk_printk(level, facility, format, args...) do { \
	if (/*mtk_dbg_level(level)*/1) { \
		printk(facility "%s %d: " format , \
				__func__, __LINE__ , ## args); \
	} } while (0)

extern unsigned mtk_usb_debug;

static inline int mtk_dbg_level(unsigned l)
{
	return mtk_usb_debug >= l;
}

#define MTK_DBG(level, fmt, args...) mtk_printk(level, KERN_INFO, fmt, ## args)

//void* musb_malloc(unsigned int);
//void  musb_free(void*);

#define MUSB_MemAlloc(size)				kmalloc(size, GFP_ATOMIC)
#define MUSB_GPD_MemAlloc(dmaAddr,size)			dma_alloc_coherent(NULL, size,dmaAddr, GFP_KERNEL)
#define MUSB_GPD_MemFree(dmaAddr,size)				dma_free_coherent(NUll,size, dmaAddr)
#define MUSB_MemFree(x)				kfree(x)
#define MUSB_MemFlush(x, size)			dmac_flush_range(x, size)
#define MUSB_MemInvalidate(x, size)		dmac_inv_range(x, size)
#define MUSB_MemCopy					memcpy
#define MUSB_MemSet					memset

#if 1
/// @brief Define DMAQ Error Status
/// .
/// @Author_Name:tianhao.fei 4/29/2010
//CC: maybe we need to redefine this
enum QStatus
{
	MTK_NO_ERROR,			///No error
	MTK_MODULE_ERROR,		///checksum error
	MTK_LENGTH_ERROR,		///length error (for Rx, receive exceed length data)
	MTK_BUS_ERROR			///Bus error
};

/// @brief Define DMAQ Error Status
/// .
/// @param buf: pointer of done GPD
/// @param buf_length: buffer length of GPD
/// @param actual_length: if Tx, this field is zero; if Rx, this field feedback the received length
/// @param next: the next SDU result pointer. The field point to NULL means that it's the last done SDU.
/// @Author_Name:tianhao.fei 5/25/2010
typedef struct Mtk_SDU_Result
{
	u8*	buf;
	u16	buf_length;
	u16 actual_length;
	struct Mtk_SDU_Result*	next;
}__attribute__ ((packed))Mtk_SDU_Result, *PMtk_SDU_Result;


/// @brief Define DMAQ communication result board
/// .
/// @Author_Name:tianhao.fei 4/29/2010
/// @param isEmpty: whether Queue is empty
/// @param status: Queue Error status
/// @number_of_sdu: number of complete SDU
/// @param link_header: SDU result link header pointer. The field point to NULL means that there is no SDU done.
typedef struct Mtk_USB_Result
{
	u8	isEmpty;		
	enum QStatus	status;	
	u16	number_of_sdu;
	PMtk_SDU_Result	link_header;
}__attribute__ ((packed))Mtk_USB_Result, *PMtk_USB_Result;
#endif
#if 1
//extern int mtk_usb_result;

/// @brief Enable Queue
/// .
/// @param EP_Num: Endpoint number mapping Queue 
/// @param isRx: RxQ or TxQ
/// @param isZLP: if open ZLP feature
/// @param isCSCheck: if open checksum check
/// @param isEmptyCheck: if open empty notification
/// @return status.
extern int musb_qmu_enable(
    struct musb* musb,
	u8 EP_Num, 			
	u8 isRx, 				
	u8 isZLP, 			
	u8 isCSCheck, 		
	u8 isEmptyCheck
);

/// @brief Disable Queue
/// .
/// @param EP_Num: Endpoint number mapping Queue 
/// @param isRx: RxQ or TxQ
/// @return status.
extern void musb_qmu_disable(
    struct musb* musb,
	u8 EP_Num, 			
	u8 isRx				
);

/// @brief Insert a transfer task into Queue
/// .
/// @param EP_Num: Endpoint number mapping Queue  
/// @param isRx: RxQ or TxQ
/// @param buf: Data buffer pointer
/// @param length: if Tx this is transmit data length, else this is allow length
/// @param isIOC: whether to set IOC flag on this GPD
extern int musb_qmu_insert_task(
    struct musb* musb,
	u8 EP_Num, 			
	u8 isRx, 				
	u8* buf, 			
	u32 length,
	u8 isIOC
);

/// @brief Remove a transfer task from Queue
/// .
/// @param EP_Num: Endpoint number mapping Queue  
/// @param isRx: RxQ or TxQ
/// @param buf: Data buffer pointer, use to find GPD
/// @param length: if Tx this is transmit data length, else this is allow length
/// @return status.
extern int musb_qmu_remove_task(
struct musb* musb,u8 EP_Num, u8 isRx, u8* buf, u16 length, u8* irq_cb, u8* do_stop); 

/// @brief Clean up the selected Queue
/// .
/// @param EP_Num: Endpoint number mapping Queue  
/// @param isRx: RxQ or TxQ
/// @return status.
extern int musb_qmu_cleanup(
    struct musb* musb,
	u8 EP_Num, 			
	u8 isRx				
);

/// @brief Stop the selected Queue
/// .
/// @param EP_Num: Endpoint number mapping Queue  
/// @param isRx: RxQ or TxQ
extern void musb_qmu_stop(
    struct musb* musb,
	u8 EP_Num, 			
	u8 isRx				
);

/// @brief Start the selected Queue
/// .
/// @param EP_Num: Endpoint number mapping Queue 
/// @param isRx: RxQ or TxQ
/// @return status.
extern int musb_qmu_restart(
    struct musb* musb,
	u8 EP_Num, 			
	u8 isRx				
);

/// @brief DMAQ interrupt handling, record the interrupt status in MTK_USB_Result
/// .
/// @param qisar: qisar value
/// @param pResult: Record interrupt status  
/// @return status.
extern int musb_qmu_irq(
    struct musb* musb,
	u32 qisar,
	Mtk_USB_Result pResult[][8]	
);
//extern char mtk_is_qmu_enabled(u8 EP_Num, u8 isRx);
extern bool musb_is_qmu_enabled(struct musb* musb, u8 EP_Num, u8 isRx);
#endif
#endif
#endif
