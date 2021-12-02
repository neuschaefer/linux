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
                    include files
-----------------------------------------------------------------------------*/

#ifndef CC_USB_DISABLE
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "musb_if.h"
#include "mu_hfi_sicd_if.h"

#include "x_typedef.h"
#include "x_os.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_socket_dev.h"
#include "x_blk_dev.h"

#ifdef CC_USB_CSR_BLUETOOTH
#include "mu_bt_if.h"
#include "x_usb_bt.h"
#endif

LINT_EXT_HEADER_END

#define USB_PTR_CHECK(X)  if (!(X)) {ASSERT((X)); return;}

//#define USBSK_LOG(fmt...)   (VOID)Printf(fmt)
#define USBSK_LOG(fmt...)

// H/W USB port number.
#define USB_HW_SOCKET_NUM              (2)

// USB hub number per h/w usb port.
#define USB_SOCKET_HUB_NUM              (5)

// USB supported device number per h/w usb port.
#define USB_SOCKET_DEVICE_NUM     (2)

// USB supported device number per h/w usb port.
#define USB_BT_DEVICE_NUM     (1)

/*
* Device and/or Interface Class codes
*/
#define USB_CLASS_PER_INTERFACE	0	/* for DeviceClass */
#define USB_CLASS_AUDIO		1
#define USB_CLASS_COMM			2
#define USB_CLASS_HID			3
#define SB_CLASS_PHYSICAL		5
#define USB_CLASS_SICD		        6
#define USB_CLASS_PRINTER		7
#define USB_CLASS_MASS_STORAGE		8
#define USB_CLASS_HUB			9
#define USB_CLASS_DATA			10
#define USB_CLASS_DIAGNOSTIC		0xDC
#define USB_CLASS_APP_SPEC		0xfe
#define USB_CLASS_VENDOR_SPEC		0xff
#define USB_CLASS_WIRELESS		0xE0

typedef void (*USB_pfSocketNfy)(void *pvSocketInfo);

typedef struct _USB_SOCKET_NFY_INFO_T{
    UINT32 u4DeviceClass;   /* OUT: device class code */    
    UINT32 u4Status;            /* OUT: device status */
    void *pvParentDevice;    /* OUT: Driver parent of device pointer*/ 
    void *pvDevice;               /* OUT: Driver device pointer */  
    UINT32 u4SocketNo;        /* OUT:  the socket number which the child device is 
                                                            attached to, Start from 0 */
    UINT32 u4CompId;          /* IN: usb_socket_mw set u4CompId id */
    void *prDevStatusInfo;   /* IN: usb_socket_mw set USB_DEV_STATUS_INFO_T pointer */
    USB_pfSocketNfy pfSocketNfy;    /* IN: usb_socket_mw set call back */
} USB_SOCKET_NFY_INFO_T;

typedef struct _USB_DEV_ATTRIBUTE_T {
    UINT32 u4CompId;      /* IN: Componet id */
    UINT32 u4DevType;     /* IN: Componet type */
    SKTDEV_DEV_ST_NFY_T rDevStNfy; /* IN/OUT: Device manager (DM) call backfunction */
    void *pvDevStatuInfo; /* IN: Point to USB_DEV_STATUS_INFO_T structure */
    
    void *pvParentDevice; /* OUT: Point to parent device */
    void *pvDevice;            /* OUT: Point to device self */ 
    UINT32 u4PortCount;    /* OUT: Number of ports on Device */     
    UINT32 u4DeviceClass;   /* OUT: device class code */    
    SKTDEV_DEV_INFO_T rDevInfo;  /* OUT: Device information */ 
} USB_DEV_ATTRIBUTE_T;

typedef struct _USB_DEV_STATUS_INFO_T {
    UINT32 u4CompId;        /* IN: usb_socket_mw set u4CompId id  = USB h/w port number */
      
    USB_DEV_ATTRIBUTE_T rRoot;

    USB_DEV_ATTRIBUTE_T rHub[USB_SOCKET_HUB_NUM];

    USB_DEV_ATTRIBUTE_T rDevice[USB_SOCKET_DEVICE_NUM];
        
#ifdef CC_USB_CSR_BLUETOOTH
    USB_DEV_ATTRIBUTE_T rBluetooth[USB_BT_DEVICE_NUM];
#endif

} USB_DEV_STATUS_INFO_T;

static USB_DEV_STATUS_INFO_T _arUsbDevInfo[USB_HW_SOCKET_NUM];
static USB_SOCKET_NFY_INFO_T _arUsbSocketNfyInfo[USB_HW_SOCKET_NUM];
static SKTDEV_USB_ST_NFY_T _arUsbOCNfyInfo[USB_HW_SOCKET_NUM];
static MUSB_HOTSWAP_NFY_INFO_T _rUsbMediumInfo[MUSB_UCFS_UNIT_COUNT];
static MUSB_HOTSWAP_NFY_INFO_T _rUsbSicdInfo[MUSB_SICD_UNIT_COUNT];

#ifdef CC_USB_CSR_BLUETOOTH
static MUSB_HOTSWAP_NFY_INFO_T _rUsbBluetoothInfo[USB_BT_DEVICE_NUM];
#endif

static UINT8 _u1USBUnsupportCode = 0;
/*-----------------------------------------------------------------------------
                    data declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
-----------------------------------------------------------------------------*/
static void _Usb_HandleOverCurrent(UINT32 u4PortNum, UINT32 u4Status)
{
    SKTDEV_USB_ST_NFY_T *ptMWOCNfyInfo;
    SKTDEV_USB_STATUS_T e_nfy_st;

    if (u4PortNum >= MUSB_GetTotalPort())
    {
        return;
    }

    USBSK_LOG("\n[USB-5] u4PortNum=%d, u4Status=%d.\n", 
        u4PortNum, u4Status);
    if (u4PortNum >= USB_HW_SOCKET_NUM)
    {
        return;
    }
    ptMWOCNfyInfo = (SKTDEV_USB_ST_NFY_T *)&_arUsbOCNfyInfo[u4PortNum];

    if ((ptMWOCNfyInfo) && (ptMWOCNfyInfo->pf_sktdev_usb_st_nfy))
    {
        e_nfy_st = (u4Status) ? 
            SKTDEV_USB_STATUS_OVERCURRENT: SKTDEV_USB_STATUS_NORMAL;
        ptMWOCNfyInfo->pf_sktdev_usb_st_nfy(ptMWOCNfyInfo->pv_tag, e_nfy_st, NULL);
    }
}

static USB_DEV_ATTRIBUTE_T * _Usb_GetAttr(
    USB_DEV_STATUS_INFO_T *prDevStatusInfo, const void *pvDevice)
{
    UINT32 i;

    // search root.
    if (prDevStatusInfo->rRoot.pvDevice == pvDevice)
    {
        return (&prDevStatusInfo->rRoot);
    }

    // search hub.
    for (i=0; i<USB_SOCKET_HUB_NUM; i++)
    {
        if (prDevStatusInfo->rHub[i].pvDevice == pvDevice)
        {
            return (&prDevStatusInfo->rHub[i]);
        }
    }
    
    // search supported usb device.
    for (i=0; i<USB_SOCKET_DEVICE_NUM; i++)
    {
        if (prDevStatusInfo->rDevice[i].pvDevice == pvDevice)
        {
            return (&prDevStatusInfo->rDevice[i]);
        }
    }  

#ifdef CC_USB_CSR_BLUETOOTH
    // search csr bluetooth device.
    for (i=0; i<USB_BT_DEVICE_NUM; i++)
    {
        if (prDevStatusInfo->rBluetooth[i].pvDevice == pvDevice)
        {
            return (&prDevStatusInfo->rBluetooth[i]);
        }
    }  
#endif /* #ifdef CC_USB_CSR_BLUETOOTH */

    return NULL;
}

static USB_DEV_ATTRIBUTE_T * _Usb_GetFreeAttr(
    USB_DEV_STATUS_INFO_T *prDevStatusInfo, UINT32 u4DevType)
{
    UINT32 i;

    // search root.
    if (u4DevType == DRVT_HW_USB)
    {
        return (&prDevStatusInfo->rRoot);
    }
    else if (u4DevType == DRVT_HUB)
    {
        // search hub.
        for (i=0; i<USB_SOCKET_HUB_NUM; i++)
        {
            if (prDevStatusInfo->rHub[i].pvDevice == NULL)
            {
                return (&prDevStatusInfo->rHub[i]);
            }
        }
    }
    else if ((u4DevType == DRVT_UNKNOWN) ||
                (u4DevType == DRVT_MEM_CARD_READER))
    {    
        // search container for supported/unsupported device.
        for (i=0; i<USB_SOCKET_DEVICE_NUM; i++)
        {
            if (prDevStatusInfo->rDevice[i].pvDevice == NULL)
            {
                return (&prDevStatusInfo->rDevice[i]);
            }
        }
    }
#ifdef CC_USB_CSR_BLUETOOTH    
    else if (u4DevType == DRVT_USB_BT)
    {    
        // search container for supported/unsupported device.
        for (i=0; i<USB_BT_DEVICE_NUM; i++)
        {
            if (prDevStatusInfo->rBluetooth[i].pvDevice == NULL)
            {
                return (&prDevStatusInfo->rBluetooth[i]);
            }
        }
    }
#endif    

    return NULL;
}

static void _Usb_SocketNfy(void *pvSocketInfo)
{
    SKTDEV_DEV_INFO_T rDevInfo = {0};
    USB_SOCKET_NFY_INFO_T *prSocketInfo = NULL;
    USB_DEV_STATUS_INFO_T *prDevStatusInfo = NULL;
    USB_DEV_ATTRIBUTE_T *prAttr = NULL;
    SKTDEV_DEV_ST_NFY_T *prDevStNfy = NULL;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    UINT32 u4DevFlag = SKTDEV_FLAG_SOCKET_DEV;
    
    USB_PTR_CHECK(pvSocketInfo);
    prSocketInfo = (USB_SOCKET_NFY_INFO_T *)pvSocketInfo;
    USB_PTR_CHECK(prSocketInfo->prDevStatusInfo);
    prDevStatusInfo = (USB_DEV_STATUS_INFO_T *)prSocketInfo->prDevStatusInfo;  
       
    if (prSocketInfo->u4Status == (UINT32)USB_STATUS_DEV_READY)
    {           
        USBSK_LOG("\n[USB-2] Device insert.\n");      

        if (prSocketInfo->u4DeviceClass == USB_CLASS_MASS_STORAGE)
        {
            prAttr = _Usb_GetFreeAttr(prDevStatusInfo, DRVT_MEM_CARD_READER);       
            USB_PTR_CHECK(prAttr);

            pHfiDriver = MUSB_HfiDriverGet();
            // get total socket number by prAttr->pDevice.
            if (0 > pHfiDriver->pfIoCtl(0, (UINT32)MUSB_IOCTL_GET_SOCKET_NUM, 
                (UINT32)prSocketInfo->pvDevice, &prAttr->u4PortCount))
            {
                return;
            }             
            u4DevFlag = SKTDEV_FLAG_SOCKET_DEV;            
        }
        else if (prSocketInfo->u4DeviceClass == USB_CLASS_SICD)
        {
            prAttr = _Usb_GetFreeAttr(prDevStatusInfo, DRVT_MEM_CARD_READER);       
            USB_PTR_CHECK(prAttr);
        
            pHfiDriver = MUSB_HfiSicdDriverGet();
            // get total socket number by prAttr->pDevice.
            if (0 > pHfiDriver->pfIoCtl(0, (UINT32)MUSB_SICD_IOCTL_GET_SOCKET_NUM, 
                (UINT32)prSocketInfo->pvDevice, &prAttr->u4PortCount))
            {
                return;
            }
            u4DevFlag = SKTDEV_FLAG_SOCKET_DEV;            
        }
#ifdef CC_USB_CSR_BLUETOOTH            
        else if (prSocketInfo->u4DeviceClass == USB_CLASS_WIRELESS)
        {
            prAttr = _Usb_GetFreeAttr(prDevStatusInfo, DRVT_USB_BT);       
            USB_PTR_CHECK(prAttr);
        
            prAttr->u4PortCount = 1;
            u4DevFlag = SKTDEV_FLAG_MISC_DEV;            
        }
#endif        
        else
        {
            ASSERT((prSocketInfo->u4DeviceClass != USB_CLASS_SICD) ||
                   (prSocketInfo->u4DeviceClass != USB_CLASS_WIRELESS) ||
                   (prSocketInfo->u4DeviceClass != USB_CLASS_MASS_STORAGE));
            return;
        }
            
        USBSK_LOG("[USB-2] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
            prAttr->u4PortCount, prAttr->u4CompId, 
            prSocketInfo->u4SocketNo, prSocketInfo->pvParentDevice, prSocketInfo->pvDevice);
        
        rDevInfo.u4SocketNo = prSocketInfo->u4SocketNo;        /*u4SocketNo start from 0*/
        rDevInfo.eDevSt = SKTDEV_STATUS_DEV_ATTACH;
        rDevInfo.u_info.rSupDev.u4DevType = prAttr->u4DevType;
        rDevInfo.u_info.rSupDev.u4CompId = prAttr->u4CompId;
        rDevInfo.u_info.rSupDev.u4DevFlag = u4DevFlag;
        prAttr->pvParentDevice = prSocketInfo->pvParentDevice;
        prAttr->pvDevice = prSocketInfo->pvDevice;
        prAttr->u4DeviceClass = prSocketInfo->u4DeviceClass;
        (VOID)x_memcpy((VOID *)&prAttr->rDevInfo, 
            (const VOID *)&rDevInfo, (SIZE_T)sizeof(SKTDEV_DEV_INFO_T));            
    }    
    else if (prSocketInfo->u4Status == (UINT32)USB_STATUS_DEV_ABSENT)
    {               
        USBSK_LOG("\n[USB-2] Device remove.\n");

        prAttr = _Usb_GetAttr(prDevStatusInfo, prSocketInfo->pvDevice);
        USB_PTR_CHECK(prAttr);

        USBSK_LOG("[USB-2] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
            prAttr->u4PortCount, prAttr->u4CompId, 
            prSocketInfo->u4SocketNo, prSocketInfo->pvParentDevice, prSocketInfo->pvDevice);

        rDevInfo.u4SocketNo = prSocketInfo->u4SocketNo;        /*u4SocketNo start from 0*/
        rDevInfo.eDevSt = SKTDEV_STATUS_DEV_DETACH;
        rDevInfo.u_info.rSupDev.u4DevType = prAttr->u4DevType;
        rDevInfo.u_info.rSupDev.u4CompId = prAttr->u4CompId;
#ifdef CC_USB_CSR_BLUETOOTH           
        u4DevFlag = (prSocketInfo->u4DeviceClass == USB_CLASS_WIRELESS) ?
            SKTDEV_FLAG_MISC_DEV : SKTDEV_FLAG_SOCKET_DEV;        
#else
        u4DevFlag = SKTDEV_FLAG_SOCKET_DEV;
#endif            	            	
        rDevInfo.u_info.rSupDev.u4DevFlag = u4DevFlag;

        (VOID)x_memset((VOID *)&prAttr->rDevStNfy, 0, (SIZE_T)sizeof(SKTDEV_DEV_ST_NFY_T));
        prAttr->pvParentDevice = NULL;
        prAttr->pvDevice = NULL;
        prAttr->u4DeviceClass = 0;        
        (VOID)x_memset((VOID *)&prAttr->rDevInfo, 0, (SIZE_T)sizeof(SKTDEV_DEV_INFO_T));            
    }
    else if (prSocketInfo->u4Status == (UINT32)USB_STATUS_UNSUPPORT)
    {        
        USBSK_LOG("\n[USB-2] Unsupported Device insert.\n");

        // Unknown device type.
        prAttr = _Usb_GetFreeAttr(prDevStatusInfo, DRVT_UNKNOWN);       
        USB_PTR_CHECK(prAttr);

        USBSK_LOG("[USB-2] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
            prAttr->u4PortCount, prAttr->u4CompId, 
            prSocketInfo->u4SocketNo, prSocketInfo->pvParentDevice, prSocketInfo->pvDevice);

        rDevInfo.u4SocketNo = prSocketInfo->u4SocketNo;    
        rDevInfo.eDevSt = SKTDEV_STATUS_DEV_UNSUPPORT;
        _u1USBUnsupportCode = (UINT8)SKTDEV_UNSUPPORT_TYPE_DEV;
        rDevInfo.u_info.rUnsupDev.pvData = (VOID *)&_u1USBUnsupportCode;        
        rDevInfo.u_info.rUnsupDev.u4DataSz = sizeof(SKTDEV_UNSUPPORT_DESC_T);

        prAttr->pvParentDevice = prSocketInfo->pvParentDevice;
        prAttr->pvDevice = prSocketInfo->pvDevice;
        prAttr->u4DeviceClass = 0;        
        (VOID)x_memcpy((VOID *)&prAttr->rDevInfo, 
            (const VOID *)&rDevInfo, (SIZE_T)sizeof(SKTDEV_DEV_INFO_T));                    
    }
    else if (prSocketInfo->u4Status == (UINT32)USB_STATUS_UNSUPPORT_ABSENT)
    {              
        USBSK_LOG("\n[USB-2] Unsupported Device remove.\n");        

        prAttr = _Usb_GetAttr(prDevStatusInfo, prSocketInfo->pvDevice);
        USB_PTR_CHECK(prAttr);
        
        USBSK_LOG("[USB-2] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
            prAttr->u4PortCount, prAttr->u4CompId, 
            prSocketInfo->u4SocketNo, prSocketInfo->pvParentDevice, prSocketInfo->pvDevice);

        rDevInfo.u4SocketNo = prSocketInfo->u4SocketNo;    
        rDevInfo.eDevSt = SKTDEV_STATUS_DEV_DETACH;
        _u1USBUnsupportCode = (UINT8)SKTDEV_UNSUPPORT_TYPE_DEV;
        rDevInfo.u_info.rUnsupDev.pvData = (VOID *)&_u1USBUnsupportCode;                
        rDevInfo.u_info.rUnsupDev.u4DataSz = sizeof(SKTDEV_UNSUPPORT_DESC_T);

        (VOID)x_memset((VOID *)&prAttr->rDevStNfy, 0, (SIZE_T)sizeof(SKTDEV_DEV_ST_NFY_T));
        prAttr->pvParentDevice = NULL;
        prAttr->pvDevice = NULL;
        prAttr->u4DeviceClass = 0;
        (VOID)x_memset((VOID *)&prAttr->rDevInfo, 0, (SIZE_T)sizeof(SKTDEV_DEV_INFO_T));                    
    }
    else if (prSocketInfo->u4Status == (UINT32)USB_STATUS_HUB_READY)
    {           
        USBSK_LOG("\n[USB-2] Hub insert.\n");

        prAttr = _Usb_GetFreeAttr(prDevStatusInfo, DRVT_HUB);
        USB_PTR_CHECK(prAttr);

        pHfiDriver = MUSB_HfiDriverGet();

        // get total socket number by prAttr->pDevice.
        if (0 > pHfiDriver->pfIoCtl(0, (UINT32)MUSB_IOCTL_GET_SOCKET_NUM, 
            (UINT32)prSocketInfo->pvDevice, &prAttr->u4PortCount))
        {
            return;
        } 

        USBSK_LOG("[USB-2] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
            prAttr->u4PortCount, prAttr->u4CompId, 
            prSocketInfo->u4SocketNo, prSocketInfo->pvParentDevice, prSocketInfo->pvDevice);
        
        rDevInfo.u4SocketNo = prSocketInfo->u4SocketNo;        /*u4SocketNo start from 0*/
        rDevInfo.eDevSt = SKTDEV_STATUS_DEV_ATTACH;
        rDevInfo.u_info.rSupDev.u4DevType = prAttr->u4DevType;
        rDevInfo.u_info.rSupDev.u4CompId = prAttr->u4CompId;
        rDevInfo.u_info.rSupDev.u4DevFlag = SKTDEV_FLAG_SOCKET_DEV;

        prAttr->pvParentDevice = prSocketInfo->pvParentDevice;
        prAttr->pvDevice = prSocketInfo->pvDevice;
        prAttr->u4DeviceClass = prSocketInfo->u4DeviceClass;        
        (VOID)x_memcpy((VOID *)&prAttr->rDevInfo, 
            (const VOID *)&rDevInfo, (SIZE_T)sizeof(SKTDEV_DEV_INFO_T));            
    }    
    else if (prSocketInfo->u4Status == (UINT32)USB_STATUS_HUB_ABSENT)
    {               
        USBSK_LOG("\n[USB-2] Hub remove.\n");

        prAttr = _Usb_GetAttr(prDevStatusInfo, prSocketInfo->pvDevice);
        USB_PTR_CHECK(prAttr);

        USBSK_LOG("[USB-2] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
            prAttr->u4PortCount, prAttr->u4CompId, 
            prSocketInfo->u4SocketNo, prSocketInfo->pvParentDevice, prSocketInfo->pvDevice);
        
        rDevInfo.u4SocketNo = prSocketInfo->u4SocketNo;        /*u4SocketNo start from 0*/
        rDevInfo.eDevSt = SKTDEV_STATUS_DEV_DETACH;
        rDevInfo.u_info.rSupDev.u4DevType = prAttr->u4DevType;
        rDevInfo.u_info.rSupDev.u4CompId = prAttr->u4CompId;
        rDevInfo.u_info.rSupDev.u4DevFlag = SKTDEV_FLAG_SOCKET_DEV;

        (VOID)x_memset((VOID *)&prAttr->rDevStNfy, 0, (SIZE_T)sizeof(SKTDEV_DEV_ST_NFY_T));
        prAttr->pvParentDevice = NULL;
        prAttr->pvDevice = NULL;
        prAttr->u4DeviceClass = 0;
        (VOID)x_memset((VOID *)&prAttr->rDevInfo, 0, (SIZE_T)sizeof(SKTDEV_DEV_INFO_T));            
    }    
    else if (prSocketInfo->u4Status == (UINT32)USB_STATUS_UNSUPPORT_HUB) 
    {        
        USBSK_LOG("\n[USB-2] Unsupported Hub insert.\n");

        prAttr = _Usb_GetFreeAttr(prDevStatusInfo, DRVT_HUB);
        USB_PTR_CHECK(prAttr);
        
        USBSK_LOG("[USB-2] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
            prAttr->u4PortCount, prAttr->u4CompId, 
            prSocketInfo->u4SocketNo, prSocketInfo->pvParentDevice, prSocketInfo->pvDevice);
        
        rDevInfo.u4SocketNo = prSocketInfo->u4SocketNo;
        rDevInfo.eDevSt = SKTDEV_STATUS_DEV_UNSUPPORT;
        _u1USBUnsupportCode = (UINT8)SKTDEV_UNSUPPORT_TYPE_HUB;
        rDevInfo.u_info.rUnsupDev.pvData = (VOID *)&_u1USBUnsupportCode;
        rDevInfo.u_info.rUnsupDev.u4DataSz = sizeof(SKTDEV_UNSUPPORT_DESC_T);

        prAttr->pvParentDevice = prSocketInfo->pvParentDevice;
        prAttr->pvDevice = prSocketInfo->pvDevice;
        prAttr->u4DeviceClass = prSocketInfo->u4DeviceClass;                
        (VOID)x_memcpy((VOID *)&prAttr->rDevInfo, 
            (const VOID *)&rDevInfo, (SIZE_T)sizeof(SKTDEV_DEV_INFO_T));                    
    }
    else if (prSocketInfo->u4Status == (UINT32)USB_STATUS_UNSUPPORT_HUB_ABSENT)
    {               
        USBSK_LOG("\n[USB-2] Unsupported Hub remove.\n");

        prAttr = _Usb_GetAttr(prDevStatusInfo, prSocketInfo->pvDevice);
        USB_PTR_CHECK(prAttr);

        USBSK_LOG("[USB-2] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
            prAttr->u4PortCount, prAttr->u4CompId, 
            prSocketInfo->u4SocketNo, prSocketInfo->pvParentDevice, prSocketInfo->pvDevice);

        rDevInfo.u4SocketNo = prSocketInfo->u4SocketNo;
        rDevInfo.eDevSt = SKTDEV_STATUS_DEV_DETACH;
        _u1USBUnsupportCode = (UINT8)SKTDEV_UNSUPPORT_TYPE_HUB;
        rDevInfo.u_info.rUnsupDev.pvData = (VOID *)&_u1USBUnsupportCode;
        rDevInfo.u_info.rUnsupDev.u4DataSz = sizeof(SKTDEV_UNSUPPORT_DESC_T);

        (VOID)x_memset((VOID *)&prAttr->rDevStNfy, 0, (SIZE_T)sizeof(SKTDEV_DEV_ST_NFY_T));
        prAttr->pvParentDevice = NULL;
        prAttr->pvDevice = NULL;
        prAttr->u4DeviceClass = 0;
        (VOID)x_memset((VOID *)&prAttr->rDevInfo, 0, (SIZE_T)sizeof(SKTDEV_DEV_INFO_T));                    
    }

    // Get parent's notify function and call back to RM from parent's notify function.
    prAttr = _Usb_GetAttr(prDevStatusInfo, prSocketInfo->pvParentDevice);
    USB_PTR_CHECK(prAttr);    
    prDevStNfy = &prAttr->rDevStNfy;        

    USBSK_LOG("[USB-2] tag = 0x%08X, nfy = 0x%08X.\n",
        prDevStNfy->pv_tag, prDevStNfy->pf_sktdev_st_nfy);
    
    // check and execute call back function.
    if (prDevStNfy->pf_sktdev_st_nfy)
    {
        prDevStNfy->pf_sktdev_st_nfy(prDevStNfy->pv_tag, rDevInfo.eDevSt, &rDevInfo);
    }
}

static void _Usb_DevNfy(void *pvTag)
{
    MUSB_HOTSWAP_NFY_INFO_T *prNfyInfo = NULL;
    SKTDEV_DEV_INFO_T rDevInfo = {0};
    USB_DEV_ATTRIBUTE_T *prAttr = NULL;
    SKTDEV_DEV_ST_NFY_T *prDevStNfy = NULL;

    USB_PTR_CHECK(pvTag);

    prNfyInfo = (MUSB_HOTSWAP_NFY_INFO_T *)pvTag;
   
    rDevInfo.u4SocketNo = prNfyInfo->u4SocketNo;
    rDevInfo.u_info.rSupDev.u4DevType = prNfyInfo->u4DevType;
    rDevInfo.u_info.rSupDev.u4CompId = prNfyInfo->u4CompId;        

#ifdef CC_USB_CSR_BLUETOOTH    
    if (DRVT_USB_BT == prNfyInfo->u4DevType)
    {
        rDevInfo.u_info.rSupDev.u4DevFlag = SKTDEV_FLAG_MISC_DEV;        
    }
    else
#endif
    {
        rDevInfo.u_info.rSupDev.u4DevFlag = SKTDEV_FLAG_BLOCK_DEV;        
    }

    if (HFI_STATUS_ABSENT == prNfyInfo->u4Status)
    {
        USBSK_LOG("\n[USB-3] Medium remove.\n");

        rDevInfo.eDevSt = SKTDEV_STATUS_DEV_DETACH;
    }
    else if (HFI_STATUS_READY == prNfyInfo->u4Status)
    {
        USBSK_LOG("\n[USB-3] Medium insert.\n");
        
        rDevInfo.eDevSt = SKTDEV_STATUS_DEV_ATTACH;
    }
    else
    {
        USBSK_LOG("\n[USB-3] Medium init fail.\n");
        
        rDevInfo.eDevSt = SKTDEV_STATUS_DEV_FAIL;
    }

    // Get parent's notify function and call back to RM from parent's notify function.
    USB_PTR_CHECK(prNfyInfo->u4UsbPort < MUSB_GetTotalPort());    
    if(prNfyInfo->u4UsbPort >= USB_HW_SOCKET_NUM)
    {
       return ;
    }
    prAttr = _Usb_GetAttr(&_arUsbDevInfo[prNfyInfo->u4UsbPort], 
        prNfyInfo->pvDevice);
    if (prAttr)
    {
        prDevStNfy = &prAttr->rDevStNfy;        

        USBSK_LOG("[USB-3] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
            prAttr->u4PortCount, prAttr->u4CompId, 
            prNfyInfo->u4SocketNo, prAttr->pvParentDevice, prNfyInfo->pvDevice);
        USBSK_LOG("[USB-3] tag = 0x%08X, nfy = 0x%08X.\n",
            prDevStNfy->pv_tag, prDevStNfy->pf_sktdev_st_nfy);
        
        // check and execute call back function.
        if (prDevStNfy->pf_sktdev_st_nfy)
        {
            prDevStNfy->pf_sktdev_st_nfy(prDevStNfy->pv_tag, 
                rDevInfo.eDevSt, &rDevInfo);            
        }
        return;
    }        
}

/*-----------------------------------------------------------------------------
 * Name: _skt_get_fct
 *
 * Description: Storage device get function registered with Resource Manager.
 *
 * Inputs:  See Resource Manager specification.
 *
 * Outputs: -
 *
 * Returns: RMR_OK
 *              Success.
 *          RMR_DRV_NOT_ENOUGH_SPACE
 *              The given buffer does not have enough space.
 *          RMR_DRV_INV_GET_INFO
 *              Parameters are invalid or get type is not supported.
 *          RMR_DRV_GET_FAILED
 *              device is detached or get command is failed.
 ----------------------------------------------------------------------------*/
static INT32 _skt_get_fct(
    DRV_COMP_ID_T  *pt_comp_id,
    DRV_GET_TYPE_T  e_get_type,
    VOID           *pv_get_info,
    SIZE_T         *pz_get_info_len)
{
    INT32 i4_strlen;
    INT32 i4_ret = RMR_DRV_INV_GET_INFO;
    USB_DEV_ATTRIBUTE_T *prAttr = NULL;    
    USB_DEV_STATUS_INFO_T *prDevStatusInfo = NULL;    
    CHAR ps_name[32] = {0};
    
    if ((pt_comp_id == NULL)  ||
        (pv_get_info == NULL) ||
        (pz_get_info_len == NULL))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    USBSK_LOG("\n[USB-4] e_get_type=%d, e_type=%d, ui2_id=%d, pv_tag=0x%08X.\n", 
        e_get_type, pt_comp_id->e_type, pt_comp_id->ui2_id, pt_comp_id->pv_tag);

    switch (e_get_type)
    {
        case SKTDEV_GET_TYPE_SOCKET_NUM:
            if (*pz_get_info_len<sizeof(UINT32))
            {
                *pz_get_info_len = sizeof(UINT32);
                return RMR_NOT_ENOUGH_SPACE;
            }

            *pz_get_info_len = sizeof(UINT32);
        
            if (pt_comp_id->e_type == DRVT_HW_USB)
            {
                *(UINT32 *)pv_get_info = 1;            
            }
            else if ((pt_comp_id->e_type == DRVT_MEM_CARD_READER) ||
                        (pt_comp_id->e_type == DRVT_HUB))
            {
                prAttr = (USB_DEV_ATTRIBUTE_T *)pt_comp_id->pv_tag;
                if (!prAttr)
                {
                    return RMR_DRV_INV_GET_INFO; 
                }

                *(UINT32 *)pv_get_info = prAttr->u4PortCount; 
            }   
            else
            {
                return RMR_DRV_INV_GET_INFO;
            }

            i4_ret = RMR_OK;
            break;

        case SKTDEV_GET_TYPE_MODEL:

            prAttr = (USB_DEV_ATTRIBUTE_T *)pt_comp_id->pv_tag;
            if (!prAttr)
            {
                return RMR_DRV_INV_GET_INFO; 
            }
            prDevStatusInfo = (USB_DEV_STATUS_INFO_T *)prAttr->pvDevStatuInfo;
            if (!prDevStatusInfo)
            {
                return RMR_DRV_INV_GET_INFO; 
            }

            // check component id.
            if (pt_comp_id->ui2_id != (UINT16)prAttr->u4CompId)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if (pt_comp_id->e_type == DRVT_HW_USB)
            {           
                (VOID)x_sprintf(ps_name, "usb_hw_%d-%d", 
                    prDevStatusInfo->u4CompId, pt_comp_id->ui2_id);
            }
            else if (pt_comp_id->e_type == DRVT_MEM_CARD_READER)
            {           
                (VOID)x_sprintf(ps_name, "usb_device_%d-%d", 
                    prDevStatusInfo->u4CompId, pt_comp_id->ui2_id);
            }
            else if (pt_comp_id->e_type == DRVT_HUB)
            {
                (VOID)x_sprintf(ps_name, "usb_hub_%d-%d", 
                    prDevStatusInfo->u4CompId, pt_comp_id->ui2_id);
            }
            else
            {
                return RMR_DRV_INV_GET_INFO;
            }

            i4_strlen = (INT32)x_strlen((const CHAR *)ps_name) + 1;
            (VOID)x_strncpy((CHAR *)pv_get_info, (const CHAR *)ps_name, *pz_get_info_len);
            ((CHAR *)pv_get_info) [*pz_get_info_len - 1] = 0;
            *pz_get_info_len = (UINT32)i4_strlen;            
            i4_ret = RMR_OK;                        
            break;

        default:
                i4_ret = RMR_DRV_INV_GET_INFO;
            break;
    }

    // fix lint warning.
    UNUSED(pt_comp_id);
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _skt_set_fct
 *
 * Description: Storage device set function registered with Resource Manager.
 *
 * Inputs:  See Resource Manager specification.
 *
 * Outputs: -
 *
 * Returns: RMR_OK
 *              Success.
 *          RMR_DRV_INV_SET_INFO
 *              Other errors.
 ----------------------------------------------------------------------------*/
static INT32 _skt_set_fct(
     DRV_COMP_ID_T  *pt_comp_id,
     DRV_SET_TYPE_T  e_set_type,
     const VOID     *pv_set_info,
     SIZE_T          z_set_info_len)
{
    INT32 i4_ret = RMR_DRV_INV_SET_INFO;
    SKTDEV_DEV_ST_NFY_T *ptMWSocketNfyInfo;
    SKTDEV_USB_ST_NFY_T *ptMWOCNfyInfo;
    SKTDEV_DEV_INFO_T *prDevInfo = NULL;
    SKTDEV_DEV_ST_NFY_T *prDevStNfy = NULL;
    USB_DEV_ATTRIBUTE_T *prAttr = NULL;
    USB_DEV_STATUS_INFO_T *prDevStatusInfo = NULL;
    UINT32 i;
    
    if ((pt_comp_id == NULL)  ||
        (pv_set_info == NULL))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    switch (e_set_type)
    {
        case SKTDEV_SET_TYPE_DEV_ST_NFY:
            if (z_set_info_len < sizeof(SKTDEV_DEV_ST_NFY_T))
            {
                return RMR_NOT_ENOUGH_SPACE;
            }

            ptMWSocketNfyInfo = (SKTDEV_DEV_ST_NFY_T *)pv_set_info;

            USBSK_LOG("\n[USB-1] e_type = %d, tag = 0x%08X, nfy = 0x%08X.\n", 
                pt_comp_id->e_type, ptMWSocketNfyInfo->pv_tag, ptMWSocketNfyInfo->pf_sktdev_st_nfy);
            
            if (pt_comp_id->e_type == DRVT_HW_USB)
            {                          
                prAttr = (USB_DEV_ATTRIBUTE_T *)pt_comp_id->pv_tag;
                if (!prAttr)
                {
                    return RMR_DRV_INV_SET_INFO; 
                }
                prDevStatusInfo = (USB_DEV_STATUS_INFO_T *)prAttr->pvDevStatuInfo;
                if (!prDevStatusInfo)
                {
                    return RMR_DRV_INV_SET_INFO; 
                }

                // check component id.
                if (pt_comp_id->ui2_id != (UINT16)prAttr->u4CompId)
                {
                    return RMR_DRV_INV_SET_INFO;
                }

                prDevStNfy = &prAttr->rDevStNfy;

                // save call back function and tag.
                prDevStNfy->pv_tag = ptMWSocketNfyInfo->pv_tag;
                prDevStNfy->pf_sktdev_st_nfy = ptMWSocketNfyInfo->pf_sktdev_st_nfy;  

                // call back to user's function directly if device status already change.
                // check if root device insert.
                prDevInfo = &prAttr->rDevInfo;
                if ((prDevInfo->eDevSt == SKTDEV_STATUS_DEV_ATTACH) ||
                    (prDevInfo->eDevSt == SKTDEV_STATUS_DEV_UNSUPPORT))
                {
                    USBSK_LOG("[USB-1] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
                        prAttr->u4PortCount, prAttr->u4CompId, 
                        prDevInfo->u4SocketNo, prAttr->pvParentDevice, prAttr->pvDevice);
                    USBSK_LOG("[USB-1] tag = 0x%08X, nfy = 0x%08X.\n",
                        prDevStNfy->pv_tag, prDevStNfy->pf_sktdev_st_nfy);
                
                    prDevStNfy->pf_sktdev_st_nfy(prDevStNfy->pv_tag, 
                        prDevInfo->eDevSt, prDevInfo);

                    return RMR_OK;
                }

                // call back to user's function directly if device status already change.
                // check if first hub insert.
                prAttr = &prDevStatusInfo->rHub[0];
                prDevInfo = &prAttr->rDevInfo;
                if ((prDevInfo->eDevSt == SKTDEV_STATUS_DEV_ATTACH) ||
                    (prDevInfo->eDevSt == SKTDEV_STATUS_DEV_UNSUPPORT))
                {
                    USBSK_LOG("[USB-1] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
                        prAttr->u4PortCount, prAttr->u4CompId, 
                        prDevInfo->u4SocketNo, prAttr->pvParentDevice, prAttr->pvDevice);
                    USBSK_LOG("[USB-1] tag = 0x%08X, nfy = 0x%08X.\n",
                        prDevStNfy->pv_tag, prDevStNfy->pf_sktdev_st_nfy);
                
                    prDevStNfy->pf_sktdev_st_nfy(prDevStNfy->pv_tag, 
                        prDevInfo->eDevSt, prDevInfo);

                    return RMR_OK;
                }

                // check if first supported usb device insert.
                prAttr = &prDevStatusInfo->rDevice[0];
                prDevInfo = &prAttr->rDevInfo;
                if ((prDevInfo->eDevSt == SKTDEV_STATUS_DEV_ATTACH) ||
                    (prDevInfo->eDevSt == SKTDEV_STATUS_DEV_UNSUPPORT))
                {
                    USBSK_LOG("[USB-1] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
                        prAttr->u4PortCount, prAttr->u4CompId, 
                        prDevInfo->u4SocketNo, prAttr->pvParentDevice, prAttr->pvDevice);
                    USBSK_LOG("[USB-1] tag = 0x%08X, nfy = 0x%08X.\n",
                        prDevStNfy->pv_tag, prDevStNfy->pf_sktdev_st_nfy);
                        
                    prDevStNfy->pf_sktdev_st_nfy(prDevStNfy->pv_tag, 
                        prDevInfo->eDevSt, prDevInfo);
                }
                
                i4_ret = RMR_OK;
            }
            else if (pt_comp_id->e_type == DRVT_HUB)
            {                                  
                prAttr = (USB_DEV_ATTRIBUTE_T *)pt_comp_id->pv_tag;
                if (!prAttr)
                {
                    return RMR_DRV_INV_SET_INFO; 
                }

                prDevStatusInfo = (USB_DEV_STATUS_INFO_T *)prAttr->pvDevStatuInfo;
                if (!prDevStatusInfo)
                {
                    return RMR_DRV_INV_SET_INFO; 
                }

                // check component id.
                if (pt_comp_id->ui2_id != (UINT16)prAttr->u4CompId)
                {
                    return RMR_DRV_INV_SET_INFO;
                }
                
                prDevStNfy = &prAttr->rDevStNfy;

                // save call back function and tag.
                prDevStNfy->pv_tag = ptMWSocketNfyInfo->pv_tag;
                prDevStNfy->pf_sktdev_st_nfy = ptMWSocketNfyInfo->pf_sktdev_st_nfy;  

                // search supported usb device.
                for (i=0; i<USB_SOCKET_DEVICE_NUM; i++)
                {
                    prDevInfo = &prDevStatusInfo->rDevice[i].rDevInfo;
                    if ((prDevInfo->eDevSt == SKTDEV_STATUS_DEV_ATTACH) ||
                        (prDevInfo->eDevSt == SKTDEV_STATUS_DEV_UNSUPPORT))
                    {
                        USBSK_LOG("[USB-1] [%d:%d] S=%d, P=0x%08X, D=0x%08X.\n",
                            prAttr->u4PortCount, prAttr->u4CompId, 
                            prDevInfo->u4SocketNo, prAttr->pvParentDevice, prAttr->pvDevice);
                        USBSK_LOG("[USB-1] tag = 0x%08X, nfy = 0x%08X.\n",
                            prDevStNfy->pv_tag, prDevStNfy->pf_sktdev_st_nfy);
                            
                        prDevStNfy->pf_sktdev_st_nfy(prDevStNfy->pv_tag, 
                            prDevInfo->eDevSt, prDevInfo);
                    }                    
                }  

                i4_ret = RMR_OK;
            }                      
            else if (pt_comp_id->e_type == DRVT_MEM_CARD_READER)
            {                                  
                prAttr = (USB_DEV_ATTRIBUTE_T *)pt_comp_id->pv_tag;
                if (!prAttr)
                {
                    return RMR_DRV_INV_SET_INFO; 
                }

                // check component id.
                if (pt_comp_id->ui2_id != (UINT16)prAttr->u4CompId)
                {
                    return RMR_DRV_INV_SET_INFO;
                }
                
                prDevStNfy = &prAttr->rDevStNfy;

                // save call back function and tag.
                prDevStNfy->pv_tag = ptMWSocketNfyInfo->pv_tag;
                prDevStNfy->pf_sktdev_st_nfy = ptMWSocketNfyInfo->pf_sktdev_st_nfy;  

                // There is a child mass storage attaching on. Execute call back directly.
                if (prAttr->u4DeviceClass == USB_CLASS_MASS_STORAGE)
                {
                    for (i=0; i<MUSB_UCFS_UNIT_COUNT; i++)
                    {               
                        if ((_rUsbMediumInfo[i].pvDevice == prAttr->pvDevice) &&
                            (_rUsbMediumInfo[i].u4Status == HFI_STATUS_READY))
                        {                   
                            _Usb_DevNfy((void *)&_rUsbMediumInfo[i]);
                        }
                    }
                }
                else if (prAttr->u4DeviceClass == USB_CLASS_SICD)
                {
                    for (i=0; i<MUSB_SICD_UNIT_COUNT; i++)
                    {               
                        if ((_rUsbSicdInfo[i].pvDevice == prAttr->pvDevice) &&
                            (_rUsbSicdInfo[i].u4Status == HFI_STATUS_READY))
                        {                   
                            _Usb_DevNfy((void *)&_rUsbSicdInfo[i]);
                        }
                    }
                }                    
                
                i4_ret = RMR_OK;
            }                                   
#ifdef CC_USB_CSR_BLUETOOTH           
            else if (pt_comp_id->e_type == DRVT_USB_BT)
            {                                  
                prAttr = (USB_DEV_ATTRIBUTE_T *)pt_comp_id->pv_tag;
                if (!prAttr)
                {
                    return RMR_DRV_INV_SET_INFO; 
                }

                // check component id.
                if (pt_comp_id->ui2_id != (UINT16)prAttr->u4CompId)
                {
                    return RMR_DRV_INV_SET_INFO;
                }
                
                prDevStNfy = &prAttr->rDevStNfy;

                // save call back function and tag.
                prDevStNfy->pv_tag = ptMWSocketNfyInfo->pv_tag;
                prDevStNfy->pf_sktdev_st_nfy = ptMWSocketNfyInfo->pf_sktdev_st_nfy;  

                // There is a child mass storage attaching on. Execute call back directly.
                if (prAttr->u4DeviceClass == USB_CLASS_WIRELESS)
                {
                    for (i=0; i<USB_BT_DEVICE_NUM; i++)
                    {               
                        if ((_rUsbBluetoothInfo[i].pvDevice == prAttr->pvDevice) &&
                            (_rUsbBluetoothInfo[i].u4Status == HFI_STATUS_READY))
                        {                   
                            _Usb_DevNfy((void *)&_rUsbBluetoothInfo[i]);
                        }
                    }
                }
                
                i4_ret = RMR_OK;
            }
#endif /* #ifdef CC_USB_CSR_BLUETOOTH */

            break;
            
        case SKTDEV_SET_TYPE_USB_ST_NFY:
            if (z_set_info_len < sizeof(SKTDEV_USB_ST_NFY_T))
            {
                return RMR_NOT_ENOUGH_SPACE;
            }

            if ((pt_comp_id->ui2_id >= MUSB_GetTotalPort()) ||
                (pt_comp_id->e_type != DRVT_HW_USB))
            {
                return RMR_DRV_INV_SET_INFO;
            }
            ptMWOCNfyInfo = (SKTDEV_USB_ST_NFY_T *)pv_set_info;

            USBSK_LOG("\n[USB-1] e_type = %d, tag = 0x%08X, nfy = 0x%08X.\n", 
                pt_comp_id->e_type, ptMWOCNfyInfo->pv_tag, ptMWOCNfyInfo->pf_sktdev_usb_st_nfy);
            if(pt_comp_id->ui2_id >= USB_HW_SOCKET_NUM)
            {
            	 return RMR_DRV_NO_GET_INFO;
            }
            _arUsbOCNfyInfo[pt_comp_id->ui2_id].pv_tag = 
                ptMWOCNfyInfo->pv_tag;
            _arUsbOCNfyInfo[pt_comp_id->ui2_id].pf_sktdev_usb_st_nfy = 
                ptMWOCNfyInfo->pf_sktdev_usb_st_nfy;          

            i4_ret = RMR_OK;            
            break;
            
        default:
            i4_ret = RMR_DRV_INV_SET_INFO;
        break;
    }

    // fix lint warning.
    UNUSED(pt_comp_id);

    return i4_ret;
}

static INT32 _MW_SktDevReg(UINT32 u4Uid, void *pv_tag, DRV_TYPE_T drvType)
{
    DRV_COMP_REG_T      t_reg_comp      = {0};
    DRV_COMP_FCT_TBL_T  t_comp_fct_tbl  = {0};
    INT32               i4_ret          = RMR_OK;
    CHAR ps_name[32] = {0};

    (VOID)x_sprintf(ps_name, "usb_socket_%d-%d", drvType, u4Uid);

    USBSK_LOG("[USB-0] %s.\n", ps_name);
    
    t_reg_comp.e_type               = drvType;
    t_reg_comp.e_id_type            = ID_TYPE_IND;
    t_reg_comp.u.t_ind.ui2_id       = (UINT16)u4Uid;
    t_reg_comp.u.t_ind.ui1_port     = 0;
    t_reg_comp.u.t_ind.pv_tag       = pv_tag; /* storage device pointer. */

    t_comp_fct_tbl.pf_rm_connect    = NULL;
    t_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_comp_fct_tbl.pf_rm_get        = _skt_get_fct;
    t_comp_fct_tbl.pf_rm_set        = _skt_set_fct;

    i4_ret = x_rm_reg_comp(&t_reg_comp,
                           1,               /* num in port */
                           1,               /* num out port */
                           ps_name,
                           0, /* STORG_FLAG_SOCKET_DEV */
                           &t_comp_fct_tbl,
                           NULL,
                           0);

    if (i4_ret != RMR_OK)
    {
        return RMR_DRV_ERROR;
    }

    return RMR_OK;
}

#ifdef CC_USB_CSR_BLUETOOTH
static INT32 _bluetooth_get_fct(
    DRV_COMP_ID_T  *pt_comp_id,
    DRV_GET_TYPE_T  e_get_type,
    VOID           *pv_get_info,
    SIZE_T         *pz_get_info_len)
{
    INT32 i4_ret = RMR_DRV_INV_GET_INFO;
    MU_BT_DEVICE_TYPE* pBtDevType=NULL;
    
    if ((pt_comp_id == NULL)  ||
        (pv_get_info == NULL) ||
        (pz_get_info_len == NULL))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    pBtDevType = MUSB_BtDriverGet();
    pBtDevType->pfInitDevice(pt_comp_id->ui2_id);

    USBSK_LOG("\n[USB-7] e_get_type=%d, e_type=%d, ui2_id=%d, pv_tag=0x%08X.\n", 
        e_get_type, pt_comp_id->e_type, pt_comp_id->ui2_id, pt_comp_id->pv_tag);

    switch (e_get_type)
    {
        case USB_BT_GET_TYPE_IO_REQ_NUM:
            pBtDevType->pfIoCtl(pt_comp_id->ui2_id, MUSB_BT_IOCTL_GET_DEVICE_NUM, 
                                NULL, pv_get_info);
            i4_ret = RMR_OK;
            break;

        case USB_BT_GET_TYPE_DEV_NUM:
            pBtDevType->pfIoCtl(pt_comp_id->ui2_id, MUSB_BT_IOCTL_GET_DEVICE_NUM, 
                                NULL, pv_get_info);
            i4_ret = RMR_OK;
            break;
            
        case USB_BT_GET_TYPE_DEV_STATUS:
            pBtDevType->pfIoCtl(pt_comp_id->ui2_id, MUSB_BT_IOCTL_GET_DEVICE_STATUS, 
                                NULL, pv_get_info);
            i4_ret = RMR_OK;
            break;

        default:
                i4_ret = RMR_DRV_INV_GET_INFO;
            break;
    }

    return i4_ret;
}

static INT32 _bluetooth_set_fct(
     DRV_COMP_ID_T  *pt_comp_id,
     DRV_SET_TYPE_T  e_set_type,
     const VOID     *pv_set_info,
     SIZE_T          z_set_info_len)
{
    INT32 i4_ret = RMR_DRV_INV_SET_INFO;
    MUSB_BT_IO_T* pBtIoRequest=NULL;
    MU_BT_DEVICE_TYPE* pBtDevType=NULL;
    
    if ((pt_comp_id == NULL)  ||
        (pv_set_info == NULL))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    pBtIoRequest = (MUSB_BT_IO_T*)pv_set_info;
    pBtDevType = MUSB_BtDriverGet();
    pBtDevType->pfInitDevice(pt_comp_id->ui2_id);

    switch (e_set_type)
    {
        case USB_BT_SET_TYPE_SEND_IO_REQ:
        case USB_BT_SET_TYPE_SEND_IO_REQ_ISR:
            i4_ret = pBtDevType->pfBtIOTransfer(pt_comp_id->ui2_id, pBtIoRequest);
            if ( i4_ret != 0 )
            {
                i4_ret = RMR_DRV_SET_FAILED;
            }
            break;
            
        default:
            i4_ret = RMR_DRV_INV_SET_INFO;
        break;
    }

    return i4_ret;
}

static INT32 _MW_BluetoothDevReg(UINT32 u4Uid, void *pv_tag, DRV_TYPE_T drvType)
{
    DRV_COMP_REG_T      t_reg_comp      = {0};
    DRV_COMP_FCT_TBL_T  t_comp_fct_tbl  = {0};
    INT32               i4_ret          = RMR_OK;
    CHAR ps_name[32] = {0};

    (VOID)x_sprintf(ps_name, "usb_bt_%d-%d", drvType, u4Uid);

    USBSK_LOG("[USB-0] %s.\n", ps_name);
    
    t_reg_comp.e_type               = drvType;
    t_reg_comp.e_id_type            = ID_TYPE_IND;
    t_reg_comp.u.t_ind.ui2_id       = (UINT16)u4Uid;
    t_reg_comp.u.t_ind.ui1_port     = 0;
    t_reg_comp.u.t_ind.pv_tag       = pv_tag; /* storage device pointer. */

    t_comp_fct_tbl.pf_rm_connect    = NULL;
    t_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_comp_fct_tbl.pf_rm_get        = _bluetooth_get_fct;
    t_comp_fct_tbl.pf_rm_set        = _bluetooth_set_fct;

    i4_ret = x_rm_reg_comp(&t_reg_comp,
                           1,               /* num in port */
                           1,               /* num out port */
                           ps_name,
                           0, /* STORG_FLAG_SOCKET_DEV */
                           &t_comp_fct_tbl,
                           NULL,
                           0);

    if (i4_ret != RMR_OK)
    {
        return RMR_DRV_ERROR;
    }

    return RMR_OK;
}
#endif /* #ifdef CC_USB_CSR_BLUETOOTH */

INT32 MW_SOCKET_USB_Init(void)
{
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;    
    UINT32 i = 0;
    UINT32 j = 0;
    UINT32 u4CompId = 0;
    INT32 i4Ret;
    USB_DEV_STATUS_INFO_T *prDevStatusInfo = NULL;
#ifdef CC_USB_CSR_BLUETOOTH
    MU_BT_DEVICE_TYPE *prBtDriver;
#endif

    VERIFY(0 == MUSB_Init(MUSB_HOST_MODE));

    pHfiDriver = MUSB_HfiDriverGet();
    
    // register root h/w port: type = DRVT_HW_USB.
    for (i=0; i<MUSB_GetTotalPort(); i++)
    {            
        // register root h/w port for over current handler.
        if (0 > pHfiDriver->pfIoCtl(0, (UINT32)MUSB_IOCTL_SET_OCNFY, 
                i, (void *)_Usb_HandleOverCurrent))
        {
            return RMR_DRV_ERROR;
        }
        if(i >= USB_HW_SOCKET_NUM)
        {
      	    return RMR_DRV_ERROR;
        }
        prDevStatusInfo = &_arUsbDevInfo[i];
        prDevStatusInfo->u4CompId = i;

        // init usb root.
        prDevStatusInfo->rRoot.u4CompId = i;
        prDevStatusInfo->rRoot.u4DevType = DRVT_HW_USB;
        (VOID)x_memset((VOID *)&prDevStatusInfo->rRoot.rDevStNfy, 0, 
            (SIZE_T)sizeof(SKTDEV_DEV_ST_NFY_T));
        prDevStatusInfo->rRoot.pvDevStatuInfo = (void *)prDevStatusInfo;
        prDevStatusInfo->rRoot.pvParentDevice = NULL;
        prDevStatusInfo->rRoot.pvDevice = NULL;
        prDevStatusInfo->rRoot.u4PortCount = 1; /* Root always = 1 */         
        i4Ret = _MW_SktDevReg(i, (void *)&prDevStatusInfo->rRoot, DRVT_HW_USB);
        if (i4Ret)
        {
            return RMR_DRV_ERROR;
        }

        // init usb hub.
        for (j=0; j<USB_SOCKET_HUB_NUM; j++)
        {
            u4CompId = (i*USB_SOCKET_HUB_NUM) + j;
            prDevStatusInfo->rHub[j].u4CompId = u4CompId;
            prDevStatusInfo->rHub[j].u4DevType = DRVT_HUB;
            (VOID)x_memset((VOID *)&prDevStatusInfo->rHub[j].rDevStNfy, 0, 
                (SIZE_T)sizeof(SKTDEV_DEV_ST_NFY_T));            
            prDevStatusInfo->rHub[j].pvDevStatuInfo = (void *)prDevStatusInfo;            
            prDevStatusInfo->rHub[j].pvParentDevice = NULL;
            prDevStatusInfo->rHub[j].pvDevice = NULL;
            prDevStatusInfo->rHub[j].u4PortCount = 0; /* Run time determine */          
            i4Ret = _MW_SktDevReg(u4CompId, (void *)&prDevStatusInfo->rHub[j], 
                DRVT_HUB);
            if (i4Ret)
            {
                return RMR_DRV_ERROR;
            }        
        }    

        // register default socket port for supported usb device.
        for (j=0; j<USB_SOCKET_DEVICE_NUM; j++)
        {
            u4CompId = (i*USB_SOCKET_DEVICE_NUM) + j;
            prDevStatusInfo->rDevice[j].u4CompId = u4CompId;
            prDevStatusInfo->rDevice[j].u4DevType = DRVT_MEM_CARD_READER;
            (VOID)x_memset((VOID *)&prDevStatusInfo->rDevice[j].rDevStNfy, 0, 
                (SIZE_T)sizeof(SKTDEV_DEV_ST_NFY_T));                        
            prDevStatusInfo->rDevice[j].pvDevStatuInfo = (void *)prDevStatusInfo;                        
            prDevStatusInfo->rDevice[j].pvParentDevice = NULL;
            prDevStatusInfo->rDevice[j].pvDevice = NULL;
            prDevStatusInfo->rDevice[j].u4PortCount = 0; /* Run time determine */
            i4Ret = _MW_SktDevReg(u4CompId, (void *)&prDevStatusInfo->rDevice[j], 
                DRVT_MEM_CARD_READER);
            if (i4Ret)
            {
                return RMR_DRV_ERROR;
            }        
        }    

#ifdef CC_USB_CSR_BLUETOOTH
        // register default socket port for supported usb device.
        for (j=0; j<USB_BT_DEVICE_NUM; j++)
        {
            u4CompId = (i*USB_BT_DEVICE_NUM) + j;
            prDevStatusInfo->rBluetooth[j].u4CompId = u4CompId;
            prDevStatusInfo->rBluetooth[j].u4DevType = DRVT_USB_BT;
            (VOID)x_memset((VOID *)&prDevStatusInfo->rBluetooth[j].rDevStNfy, 0, 
                (SIZE_T)sizeof(SKTDEV_DEV_ST_NFY_T));                        
            prDevStatusInfo->rBluetooth[j].pvDevStatuInfo = (void *)prDevStatusInfo;                        
            prDevStatusInfo->rBluetooth[j].pvParentDevice = NULL;
            prDevStatusInfo->rBluetooth[j].pvDevice = NULL;
            prDevStatusInfo->rBluetooth[j].u4PortCount = 0; /* Run time determine */
            i4Ret = _MW_BluetoothDevReg(u4CompId, (void *)&prDevStatusInfo->rBluetooth[j], 
                DRVT_USB_BT);
            if (i4Ret)
            {
                return RMR_DRV_ERROR;
            }        
        }    
#endif

        // set socket call back funtion to driver.
        if(i >= USB_HW_SOCKET_NUM)
        {
      	    return RMR_DRV_ERROR;
        }
        _arUsbSocketNfyInfo[i].u4CompId = i; /* HW USB Port number */
        _arUsbSocketNfyInfo[i].prDevStatusInfo = prDevStatusInfo; 
                                                                        /* USB_DEV_STATUS_INFO_T pointer */        
        _arUsbSocketNfyInfo[i].pfSocketNfy = _Usb_SocketNfy;
        if (0 > pHfiDriver->pfIoCtl(0, (UINT32)MUSB_IOCTL_SET_SOCKET_NFY, 
                i, (void *)&_arUsbSocketNfyInfo[i]))
        {
            return RMR_DRV_ERROR;
        }
    }

    // init medium call back function.
    for (i=0; i<MUSB_UCFS_UNIT_COUNT; i++)
    {               
        _rUsbMediumInfo[i].u4DevType = DRVT_USB_MASS_STORAGE;
        _rUsbMediumInfo[i].pvTag = &_rUsbMediumInfo[i];
        _rUsbMediumInfo[i].pfNfy = _Usb_DevNfy;

        if (0 > pHfiDriver->pfIoCtl(i, 
            (UINT32)MUSB_IOCTL_SET_SOCKET_DEV_NFY, 0, (void *)&_rUsbMediumInfo[i]))
        {
            return RMR_DRV_ERROR;
        }       
    }

    pHfiDriver = MUSB_HfiSicdDriverGet();
    // init sicd call back function.
    for (i=0; i<MUSB_SICD_UNIT_COUNT; i++)
    {               
        _rUsbSicdInfo[i].u4DevType = DRVT_USB_PTP_MTP;
        _rUsbSicdInfo[i].pvTag = &_rUsbSicdInfo[i];
        _rUsbSicdInfo[i].pfNfy = _Usb_DevNfy;

        if (0 > pHfiDriver->pfIoCtl(i, 
            (UINT32)MUSB_SICD_IOCTL_SET_HOTSWAP_NFY
            , 0, (void *)&_rUsbSicdInfo[i]))
        {
            return RMR_DRV_ERROR;
        }       
    }

#ifdef CC_USB_CSR_BLUETOOTH

    prBtDriver = MUSB_BtDriverGet();
    // init bluetooth call back function.
    for (i=0; i<USB_BT_DEVICE_NUM; i++)
    {               
        _rUsbBluetoothInfo[i].u4DevType = DRVT_USB_BT;
        _rUsbBluetoothInfo[i].pvTag = &_rUsbBluetoothInfo[i];
        _rUsbBluetoothInfo[i].pfNfy = _Usb_DevNfy;

        if (0 > (INT32)prBtDriver->pfIoCtl(i, 
                                    (UINT32)MUSB_BT_IOCTL_SET_HOTSWAP_NFY,
                                    0, (void *)&_rUsbBluetoothInfo[i]))
        {
            return RMR_DRV_ERROR;
        }
    }

#endif /* #ifdef CC_USB_CSR_BLUETOOTH */

    return RMR_OK;
}

#else
#include "x_typedef.h"
#include "x_rm.h"
INT32 MW_SOCKET_USB_Init(void)
{
    return RMR_OK;
}

#endif


