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
/*-----------------------------------------------------------------------------
 * $RCSfile: x_usb_bt.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains sicd device related definitions.
 *---------------------------------------------------------------------------*/

#ifndef _X_USB_BT_H_
#define _X_USB_BT_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "res_mngr/x_rm.h"

/************************************************************************/
/* Error code                                                           */
/************************************************************************/
#define USB_BT_OK                       ((INT32)    0)
#define USB_BT_DEVICE_BUSY              ((INT32)   -1)
#define USB_BT_MEDIA_REMOVED            ((INT32)   -2)
#define USB_BT_ALIGNMENT_ERROR          ((INT32)   -3)
#define USB_BT_USB_ERROR                ((INT32)   -4)
#define USB_BT_UNKNOWN_ERROR            ((INT32)   -5)

/*-----------------------------------------------------------------------------
 *  Constants and type definitions
 *---------------------------------------------------------------------------*/
/* Traffic types in ui1_traffic_type */
#define USB_BT_XFER_CONTROL	    0
#define USB_BT_XFER_ISOC	    1
#define USB_BT_XFER_BULK	    2
#define USB_BT_XFER_INT        	3

#define USB_BT_STATE_FREE                      ((uint8_t)0x00)
#define USB_BT_STATE_CONNECT                   ((uint8_t)0x01)
#define USB_BT_STATE_CONFIGURED                ((uint8_t)0x02)
#define USB_BT_STATE_SET_IDLE                  ((uint8_t)0x03)


typedef struct _USB_BT_IO_REQ_T USB_BT_IO_REQ_T;

typedef void (*usb_bt_io_complete_fnc)(VOID*                pv_tag, 
                                       USB_BT_IO_REQ_T*     pt_bt_iO);

typedef struct _USB_BT_ISO_T 
{
    UINT32      ui4_data_length;
    UINT32      ui4_actual_data_length;
    INT32       i4_status;
}USB_BT_ISO_T;

struct _USB_BT_IO_REQ_T
{
    UINT8                   ui1_traffic_type;           /*I: 0 ~ 3: select transfer type.*/
    BOOL                    b_is_tx;                    /*I: 1: Tx, 0: Rx */

    UINT8*                  aui1_setup_packet;          /*I: Only for ui1_traffic_type == USB_BT_XFER_CONTROL 8 bytes*/

    UINT8*                  aui1_data_buffer;           /*I/O: data phase data buffer; Tx:data to dev, Rx:dev to client */

    UINT32                  ui4_data_length;            /*I: If ui1_traffic_type == MUSB_BT_XFER_CONTROL, 
                                                             ui4_data_length = the size of data at data phase. It does not include setup packet. */

    UINT32                  ui4_actual_data_length;     /*O: actual data length gotton*/
    UINT32                  ui4_status;                  /*O: 0: ok, others: fail.*/

    UINT32                  ui4_packets_number;
    USB_BT_ISO_T*           at_iso_packet; 
    
    usb_bt_io_complete_fnc  pf_bt_io_complete;          /*callback function*/
    VOID*                   pv_tag;
};

typedef struct _USB_BT_IO_REQ_NUM_T
{
    BOOL                    b_is_tx;
    UINT8                   ui1_traffic_type;           /*I: 0 ~ 3: select transfer type.*/
    UINT32                  ui4_io_request_in_queue;    /*O: num of uncompleted request in driver queue*/
}USB_BT_IO_REQ_NUM_T;

/*-----------------------------------------------------------------------------
 * Get types
 *---------------------------------------------------------------------------*/
#define USB_BT_GET_TYPE_IO_REQ_NUM (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))  // USB_BT_IO_REQ_NUM_T
#define USB_BT_GET_TYPE_DEV_NUM (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))     // UINT32* 
#define USB_BT_GET_TYPE_DEV_STATUS (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))  // UINT32* (USB_BT_STATE)


/*-----------------------------------------------------------------------------
 * Set types
 *---------------------------------------------------------------------------*/
#define USB_BT_SET_TYPE_SEND_IO_REQ (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0)) // CSR_BT_IO_REQ_T: send CSR BT request to dev
#define USB_BT_SET_TYPE_SEND_IO_REQ_ISR  (RM_SET_TYPE_ISR_CONTEXT | USB_BT_SET_TYPE_SEND_IO_REQ)

#endif /* _X_CSR_BT_H_ */
