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
 * $RCSfile: x_sicd_dev.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains sicd device related definitions.
 *---------------------------------------------------------------------------*/

#ifndef _X_SICD_DEV_H_
#define _X_SICD_DEV_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "inc/x_common.h"


/************************************************************************/
/* Error code                                                           */
/************************************************************************/
/* refer to mu_hfi_sicd_if.h */
#define SICD_OK                        ((INT32)    0)
#define SICD_DEVICE_BUSY               ((INT32)   -1)
#define SICD_MEDIA_REMOVED             ((INT32)   -2)
#define SICD_ALIGNMENT_ERROR           ((INT32)   -3)
#define SICD_BUS_RESET_FAILED          ((INT32)   -4)
#define SICD_NOT_IN_DATA_PHASE         ((INT32)   -5)
#define SICD_NOT_IN_READY_PHASE        ((INT32)   -6)
#define SICD_USB_ERROR                 ((INT32)   -7)
#define SICD_UNKNOWN_ERROR             ((INT32)   -8)

/*-----------------------------------------------------------------------------
 *  Constants and type definitions
 *---------------------------------------------------------------------------*/
/* mapping to MUSB_HFI_NFY_TYPE_E */
#define USB_CAPABILITY_NONE 0x00
#define USB_CAPABILITY_SICD 0x01
#define USB_CAPABILITY_MSD  0x02

/* mapping to MUSB_HFI_NFY_TYPE_E */
typedef enum
{                                                   /* The structure of pv_data */
    SICDDEV_COND_READ_READY                 = 1,    /* SICDDEV_BLOCK_T */
    SICDDEV_COND_WRITE_READY                = 2,
    SICDDEV_COND_PHASE_CMD_COMPLETE         = 3,    /* SICDDEV_BLOCK_T (CMD) */
    SICDDEV_COND_PHASE_DATA_COMPLETE        = 4,    /* SICDDEV_BLOCK_T (DATA) */
    SICDDEV_COND_PHASE_ERSP_COMPLETE        = 5,    /* SICDDEV_BLOCK_T (RESP) */
    SICDDEV_COND_GOT_EVENT                  = 6,    /* SICDDEV_BLOCK_T (EVENT) */
    SICDDEV_COND_GET_EXT_EVENT_COMPLETE     = 7,    /* SICDDEV_EXT_EVENT_BLOCK_T (EXT EVENT DATA) */
    SICDDEV_COND_OPERATION_ERROR            = 8,    /* NULL */
    SICDDEV_COND_RESET_SUCCESS              = 9,    /* NULL */
    SICDDEV_COND_RESET_FAIL                 = 10,   /* NULL */
    SICDDEV_COND_ABORT_SUCCESS              = 11,   /* NULL */
    SICDDEV_COND_ABORT_FAIL                 = 12,   /* NULL */
    SICDDEV_COND_SKIP_COMPLETE              = 13,   /* NULL */
    SICDDEV_COND_SILENT_BUS_RESET           = 14    /* NULL */
} SICDDEV_COND_T;

/* refer to mu_hfi_if.h */
typedef enum
{
    SICDDEV_STATUS_ABSENT = 0,
    SICDDEV_STATUS_FAIL   = 1,
    SICDDEV_STATUS_READY  = 2
} SICDDEV_STATUS_T;

/* mapping to MUSB_SICD_DATA_TYPE_E */
typedef enum{
    SICDDEV_CMD_BLOCK = 1,
    SICDDEV_DATA_BLOCK,
    SICDDEV_RESPONSE_BLOCK, 
    SICDDEV_EVENT_BLOCK
}SICDDEV_DATA_TYPE_T;

/* mapping to MUSB_SICD_DATA_DIRECTION_E */
typedef enum{
    SICDDEV_DATA_OUT,
    SICDDEV_DATA_IN,
    SICDDEV_NO_DATA
}SICDDEV_DATA_DIRECTION_T;


/**
 * SICDDEV_BLOCK_T.
 * Command, Data, Response, Event Block Structure.
 * mapping to MUSB_SICD_BLOCK_T
 */
typedef struct
{
    SICDDEV_DATA_TYPE_T e_data_type;
    SICDDEV_DATA_DIRECTION_T e_data_direction;
    UINT16 ui2_operation_code;
    UINT32 ui4_payload_buflen;
    UINT32 ui4_transcation_id;
    UINT8* pui1_payload_buf;
} SICDDEV_BLOCK_T;

/**
 * SICDDEV_EXT_EVENT_BLOCK_T.
 * Extended Event Block Structure.
 * mapping to MUSB_SICD_EXT_EVENT_BLOCK_T
 */
typedef struct
{
    UINT16 ui2_event_code;
    UINT32 ui4_transaction_id;
    UINT16 ui1_num_parameter;
    UINT8* pui1_payload_buf;
}SICDDEV_EXT_EVENT_BLOCK_T;


/*-----------------------------------------------------------------------------
 * Notify function
 *---------------------------------------------------------------------------*/

/* mapping to MUSB_pfHfiNfy */
typedef VOID (*x_sicddev_nfy_fct)(VOID           *pv_nfy_tag,
                                  SICDDEV_COND_T e_nfy_cond,
                                  VOID           *pv_data);

/* mapping to MUSB_pfHfiHotSwapNfy */
typedef VOID (*x_sicddev_hotswap_nfy_fct)(void *pv_tag);


/* mapping to MUSB_NFY_INFO_T */
typedef struct _SICDDEV_NFY_INFO_T
{
    VOID             *pv_tag;
    x_sicddev_nfy_fct  pf_sicddev_nfy;
} SICDDEV_NFY_INFO_T;


/* mapping to MUSB_HOTSWAP_NFY_INFO_T */
typedef struct _SICDDEV_HOTSWAP_NFY_INFO_T
{
    UINT32  ui4_comp_id;                  /* OUT: Driver set u4CompId id */
    VOID    *pv_device;                   /* OUT: Driver device pointer */  
    UINT32  ui4_status;                   /* OUT: device status */
    UINT32  ui4_socket_no;                /* OUT: the socket number which the child device is 
                                                  attached to, Start from 0 */   
    UINT32  ui4_usb_port;                 /* OUT: Driver set h/w usb port number */
    UINT32  ui4_dev_type;                 /* IN:  usb_socket_mw set device type at init call back */    
    VOID    *pv_tag;                      /* IN:  usb_socket_mw set notify tag */
    x_sicddev_hotswap_nfy_fct pf_nfy;     /* IN:  usb_socket_mw set call back function */
} SICDDEV_HOTSWAP_NFY_INFO_T;


/*-----------------------------------------------------------------------------
 * Parameter definitions of get/set types
 *---------------------------------------------------------------------------*/
 /* mapping to MUSB_SICD_IO_DATA_T */
typedef struct 
{
    UINT16    ui2_operation_code;
    UINT16    ui2_param_num;
    UINT32    ui4_transaction_id;
    UINT32    aui4_param[5];
    UINT32    ui4_data_buf_len; /* Size of pvDataBuf. Should not be changed during data phase running.*/
    UINT8     *pui1_data_buf;
    UINT32    ui4_total_data_size; /* used in direction out operation */
    SICDDEV_DATA_DIRECTION_T e_data_direction; /* Data direction in Data Phase */
}SICDDEV_IO_DATA_T;

/* mapping to MUSB_HfiDeviceInfo */
typedef struct
{
    UINT16 ui2_vendor_id;
    UINT16 ui2_product_id;
    UINT16 ui2_bcd_device;
    UINT8  ui1_bus_address;
    UINT8  ui1_lun_count;    
    UINT8  *pui1_disk_vendor;
    UINT8  *pui1_disk_product;
    UINT8  *pui1_disk_revision;    
} SICDDEV_DEVICEINFO_T;


typedef struct _SICD_REQUEST_T
{
    UINT32 ui4_type;
    union
    {
        /*Get Type*/
        SICDDEV_STATUS_T e_device_status;
        SICDDEV_DEVICEINFO_T t_device_info;
        UINT32 ui4_alignment;
        UINT32 ui4_capability;

        /*Set Type*/
        SICDDEV_IO_DATA_T t_io_data;
        SICDDEV_NFY_INFO_T t_nfy_info;
        SICDDEV_HOTSWAP_NFY_INFO_T t_hotswap_nfy_info;
        UINT32 ui4_offset;

        /*Future extend*/
        VOID *pv_req;
    }u_container;

    INT32 i4_return;
}SICD_REQUEST_T;

/*-----------------------------------------------------------------------------
 * Get types
 *---------------------------------------------------------------------------*/
/*The structure of get function parameter*/
#define SICDDEV_GET_TYPE_ALIGNMENT (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))

#define SICDDEV_GET_TYPE_DEVICE_STATUS (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1)) /*SICDDEV_STATUS_T*/

#define SICDDEV_GET_TYPE_DEVICE_INFO (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2)) /*SICDDEV_DEVICEINFO_T*/

#define SICDDEV_GET_TYPE_DEVICE_CAPABILITY (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3)) /*UINT32*/
/*-----------------------------------------------------------------------------
 * Set types
 *---------------------------------------------------------------------------*/
#define SICDDEV_SET_TYPE_SEND_OPERATION (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0)) /*SICDDEV_IO_DATA_T*/

#define SICDDEV_SET_TYPE_REGISTER_NFY_FCT (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1)) /*SICDDEV_NFY_INFO_T*/

#define SICDDEV_SET_TYPE_CLEAR_NFY_FCT (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2)) /*NULL*/

#define SICDDEV_SET_TYPE_HOTSWAP_NFY_FCT (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3)) /*SICDDEV_HOTSWAP_NFY_INFO_T*/

#define SICDDEV_SET_TYPE_DEVICE_RESET (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4)) /*NULL*/

#define SICDDEV_SET_TYPE_OPERATION_ABORT (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5)) /*NULL*/

#define SICDDEV_SET_TYPE_EXTENDED_EVENT_DATA (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6)) /*NULL*/

/*for NonGetPartialObject Device*/
#define SICDDEV_SET_TYPE_GET_NEXT_DATA (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7)) /*SICDDEV_IO_DATA_T*/

#define SICDDEV_SET_TYPE_DATA_PHASE_CONTROLLED_BY_MIDDLEWARE (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 8)) /*NULL*/

#define SICDDEV_SET_TYPE_DATA_PHASE_CONTROLLED_BY_SICD (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 9)) /*NULL*/

/*for SendObject Operation*/
#define SICDDEV_SET_TYPE_SEND_NEXT_DATA (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 10)) /*SICDDEV_IO_DATA_T*/

#define SICDDEV_SET_TYPE_DATA_SKIP (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 11)) /*UINT32 ui4_offset*/

#define SICDDEV_SET_TYPE_SWITCH_TO_MSD (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 12)) /*NULL*/
#endif /* _X_SICD_DEV_H_ */
