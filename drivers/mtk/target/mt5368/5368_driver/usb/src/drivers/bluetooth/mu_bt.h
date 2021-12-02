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

#ifndef _MUSB_BT_H_
#define _MUSB_BT_H_

#include "mu_dev.h"
#include "mu_bt_if.h"
#include "mu_hfi_if.h"

#define MUSB_BT_CTRL_LENGTH             ((uint8_t)0x20)     
#define MUSB_BT_HCI_EVENT_MAX_SIZE    ((uint8_t)0x10)     
#define MUSB_BT_ISO_TYPE_NUM       ((uint8_t)0x6)     

/** State of Bluetooth driver       */
#define MUSB_BT_STATE_FREE                      ((uint8_t)0x00)
#define MUSB_BT_STATE_CONNECT                   ((uint8_t)0x01)
#define MUSB_BT_STATE_CONFIGURED                ((uint8_t)0x02)
#define MUSB_BT_STATE_SET_IDLE                  ((uint8_t)0x03)

/* Macro to prepare setup packet for Hid Class driver*/
#define MGC_PREPARE_BT_SETUP_PACKET(pSetup,\
                                     bmActualRequestType,\
                                     bActualRequest,\
                                     wActualValue,\
                                     wActualIndex,\
                                     wActualLength)\
{\
    (pSetup)->bmRequestType = (uint8_t) bmActualRequestType;\
    (pSetup)->bRequest      = (uint8_t) bActualRequest;\
    (pSetup)->wValue        = (UINT16) MUSB_SWAP16(wActualValue);\
    (pSetup)->wIndex        = (UINT16) MUSB_SWAP16(wActualIndex);\
    (pSetup)->wLength       = (UINT16) MUSB_SWAP16(wActualLength);\
}

/* Macro to fill control Irp for Mass Storage Class driver */
#define MGC_FILL_BT_CONTROL_IRP(pTargetDevice,\
                                 pControlIrp,\
                                 pActualOutBuffer,\
                                 dwRequestedOutLength,\
                                 pActualInBuffer,\
                                 dwRequestedInLength,\
                                 pfControlIrpComplete)\
{\
    (pControlIrp)->pDevice           = pTargetDevice->pUsbDevice;\
    (pControlIrp)->pOutBuffer        = pActualOutBuffer;\
    (pControlIrp)->dwOutLength       = dwRequestedOutLength;\
    (pControlIrp)->pInBuffer         = pActualInBuffer;\
    (pControlIrp)->dwInLength        = dwRequestedInLength;\
    (pControlIrp)->dwStatus          = 0;\
    (pControlIrp)->dwActualOutLength = 0;\
    (pControlIrp)->dwActualInLength  = 0;\
    (pControlIrp)->pfIrpComplete     = pfControlIrpComplete;\
    (pControlIrp)->pCompleteParam    = (void *) pTargetDevice;\
}

#define BT_MAX_BULK_TX		4
#define BT_MAX_BULK_RX		4

#define BT_MAX_ISOC_RX		2
#define BT_MAX_ISOC_TX		2


typedef struct
{
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    UINT16 wMaxPacketSize;
} MGC_BtIsoChannelInfo; 

typedef struct _MGC_BtDevice
{
    uint8_t bDeviceIndex; /* Bt device index */
    uint8_t bState;

    /* USB hid device basic information object */
    MUSB_BusHandle hBus;   /* MGC_Port* pImplPort */
    MUSB_Device *pUsbDevice;
    MUSB_Port* pPort;
    MUSB_DeviceDriver *pDriver;
    MUSB_ConfigurationDescriptor *pConfig;
    
    /* HCI Commands */
    uint8_t aSetupTx[8+MUSB_BT_CTRL_LENGTH];
    MUSB_ControlIrp ControlIrp;
    MUSB_BT_IO_T *prBtControlData;

    /* HCI Events */    
    MUSB_Pipe hIntrInPipe;
    MUSB_Irp IntrInIrp;
    MUSB_BT_IO_T *prBtIntrInData;

    /* ACL Data */    
    MUSB_Pipe hBulkInPipe;
    MUSB_Pipe hBulkOutPipe;
    MUSB_Irp BulkInIrp;
    MUSB_Irp BulkOutIrp;
    MUSB_BT_IO_T *prBtBulkInData; /* Point to current bulk in io request. */
    MUSB_BT_IO_T *parBtBulkInQueue[BT_MAX_BULK_RX];
    MUSB_BT_IO_T *prBtBulkOutData; /* Point to current bulk out io request. */
    MUSB_BT_IO_T *parBtBulkOutQueue[BT_MAX_BULK_TX];

    /* Voice Channel */
    MGC_BtIsoChannelInfo rSocIface[MUSB_BT_ISO_TYPE_NUM];    
    MUSB_Pipe hIsoInPipe;
    MUSB_Pipe hIsoOutPipe;
    MUSB_IsochIrp IsoInIrp;
    MUSB_IsochIrp IsoOutIrp;    
    MUSB_BT_IO_T *prBtIsoInData;
    MUSB_BT_IO_T *parBtIsoInQueue[BT_MAX_ISOC_RX];
    MUSB_BT_IO_T *prBtIsoOutData;
    MUSB_BT_IO_T *parBtIsoOutQueue[BT_MAX_ISOC_TX];

    /* Device manager plug in/out handling*/
    MUSB_HOTSWAP_NFY_INFO_T *pt_socket_nfy_info; /* IN:  DM set hot swap notify function. */     
} MGC_BtDevice;

/**
 * Bluetooth external function.
 */

/**
 * Fill an array with the targetted peripheral list entry appropriate
 * for the mass-storage class driver, ending with the MUSB_TARGET_ACCEPT.
 * @param pList array
 * @param wListLength how many bytes are available in the array
 * @return how many bytes were filled in the array.
 * If this equals bListLength, the caller should assume there is insufficient space
 * in the array and the list entry is incomplete.
 */
extern UINT16 MUSB_FillBtClassPeripheralList(UINT8* pList, UINT16 wListLength);

/**
 * Get a pointer to the bluetooth connect/disconnect driver
 */
extern MUSB_DeviceDriver* MUSB_GetBtClassDriver(void);

#endif /* _MUSB_BT_H_ */
