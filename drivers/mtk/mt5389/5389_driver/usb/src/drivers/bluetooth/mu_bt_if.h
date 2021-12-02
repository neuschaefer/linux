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

#ifndef _MUSB_BT_IF_H_
#define _MUSB_BT_IF_H_

#define MUSB_BT_DEVICE_NUM   (1)

/** Successful status */
#define BT_STATUS_OK			(0)
/** error status */
#define BT_STATUS_ERROR		(0x80)
/** error: NACK limit reached */
#define BT_STATUS_NACK_LIMIT		(BT_STATUS_ERROR | 0x02)
/** error: general transmit error */
#define BT_STATUS_TRANSMIT_ERROR (BT_STATUS_ERROR | 0x08)
/** error: general receive error */
#define BT_STATUS_RECEIVE_ERROR	(BT_STATUS_ERROR | 0x09)
/** error: received USB stall handshake */
#define BT_STATUS_STALLED		(BT_STATUS_ERROR | 0x17)
/** error: transfer aborted because peer was removed from bus */
#define BT_STATUS_PEER_REMOVED	(BT_STATUS_ERROR | 0x20)

/** 
 * MUSB_BtIoctlCmdType.
 * HFI Ioctl command Type.
 */
typedef enum
{
    MUSB_BT_IOCTL_GET_DEVICE_NUM,     // total number of Bluetooth devices on our system.
    MUSB_BT_IOCTL_GET_DEVICE_STATUS,
    MUSB_BT_IOCTL_SET_HOTSWAP_NFY,
    MUSB_BT_IOCTL_GET_IO_REQ_NUM,
    
    MUSB_BT_IOCTL_END
} MUSB_BtIoctlCmdType;

/**
 * Bluetooth Control/Bulk/Interrupt/Isochronous IRP completion handler.
 * A completion handler for control transfers.
 * @param 1 pCompleteParam from user's define.
 * @param 2 IRP pointer
 */
struct _MUSB_BT_IO_T;

typedef void (*MUSB_pfBtIOComplete)(void* pBtIOCompleteParam, 
    struct _MUSB_BT_IO_T *prBtIOData);

/* Traffic types in bmAttributes */
#define MUSB_BT_XFER_CONTROL	0
#define MUSB_BT_XFER_ISOC		1
#define MUSB_BT_XFER_BULK		2
#define MUSB_BT_XFER_INT		3

typedef struct _MUSB_BT_ISO_T 
{
    UINT32 dwDataLength;		/* expected length */
    UINT32 dwActualDataLength;
    UINT32 dwStatus;
}MUSB_BT_ISO_T;

typedef struct _MUSB_BT_IO_REQ_NUM_T
{
    BOOL                    b_is_tx;
    UINT8                   ui1_traffic_type;           /*I: 0 ~ 3: select transfer type.*/
    UINT32                  ui4_io_request_in_queue;    /*O: num of uncompleted request in driver queue*/
}MUSB_BT_IO_REQ_NUM_T;

typedef struct _MUSB_BT_IO_T
{
    /*0 ~ 3: select transfer type.*/
    UINT8 bTrafficType;           

    /*1: Tx, 0: Rx */
    BOOL  bIsTx;                     

    /* Only for bTrafficType == MUSB_BT_XFER_CONTROL */
    UINT8* pSetupPacket;      

    /* For all transfer type, including ISO. */
    UINT8* pDataBuffer;

    /* For Bulk/Interrupt/Control transfer.
        If bTrafficType == MUSB_BT_XFER_CONTROL, 
        dwDataLength = the size of data at data phase.
        It does not include setup packet.
        Do not use by ISO transfer.
    */
    UINT32 dwDataLength;     
    UINT32 dwActualDataLength;
    UINT32 dwStatus;              /*0: ok, others: fail.*/

    /* number of ISO packets */
    UINT32 dwNofPackets;        
    /* assign every ISO packets transfer information */
    MUSB_BT_ISO_T *prISOpacket; 
    
    MUSB_pfBtIOComplete pfBtIOComplete;
    void* pBtIOCompleteParam;
    
} MUSB_BT_IO_T;

/**
 * Element Meaning
 * 
 * @param MaxUnits Number of maximum units the driver can handle.
 * @param pfIoCtl Pointer to the device IoCtl function.
 * @param pfRWBurst Pointer to the device read/write multiple sector function. (optional)
 * @param pfWriteProtect Pointer to the device write protect checking function. (optional)
 * @param pfInitDevice Pointer to the device driver initialization function. (optional)
*/
typedef struct 
{
    UINT32 MaxUnits;
    UINT32 (*pfIoCtl) (UINT32 u4Index, UINT32 u4Cmd, UINT32 u4Aux,void *pBuffer);
    UINT32 (*pfBtIOTransfer) (UINT32 u4Index, MUSB_BT_IO_T *prBtIOData);
    UINT32 (*pfInitDevice) (UINT32 u4Index);
} MU_BT_DEVICE_TYPE;

/**
 * Get a pointer to the bluetooth handling driver
 */
extern MU_BT_DEVICE_TYPE *MUSB_BtDriverGet(void);

#endif /* _MUSB_BT_IF_H_ */
