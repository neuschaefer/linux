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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: musb_82xx_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file musb_82xx_if.c
 *  MUSB module for 82xx series api function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef MUSB_8226

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"

#include "mu_hfi_if.h"
#include "mu_cdi.h"
#include "mu_impl.h"
#include "mu_mapi.h"
#include "mu_hapi.h"
#include "mu_none.h"

#include "mu_scsi.h"

#ifdef MUSB_HUB
#include "mu_hapi.h"
#endif

#include "mu_hid_if.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
/** Function-only port */
#define USB_PORT_TYPE_FUNCTION   (1)
/** Host-only port */
#define USB_PORT_TYPE_HOST     (2)

#define MUSB_MAX_DRV_NUM ((UINT8)5)   // Debounce loop.

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef enum
{
    MUSB_82_INIT = 0,

    // MSD test item.
    MUSB_82_MSD = 10,
    MUSB_82_MSD_END,
    MUSB_82_MSD_WRITE_PROTECT,
    MUSB_82_MSD_WRITE,
    MUSB_82_MSD_READ,

    // CDC test item.
    MUSB_82_CDC = 20,
    MUSB_82_CDC_END,
    MUSB_82_CDC_TX,
    MUSB_82_CDC_RX,
    MUSB_82_CDC_CHECK,

    MUSB_82_IDLE = 50
}MUSB_NoneOsEvent;

typedef void (* pfnUsbAsyncCb)(int iRet, void* pvArg);

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------
#ifdef MUSB_FUNCTION

extern MUSB_FunctionClient MGC_MsdFunctionClient;

extern MUSB_FunctionClient MGC_CdcFunctionClient;
#endif

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
//#define USB_UNIT_TEST
#ifdef USB_UNIT_TEST
void MUSB_NoneOsMain(void);
extern void vSetStc(UINT32 dwNewStc);
extern UINT32 dwReadStc(void);
#endif
//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static MUSB_NoneOsEvent _MUSB_82Event = MUSB_82_INIT;

static MUSB_NFY_INFO_T _rt_82nfy_info;
static MUSB_IO_DATA_T _rt_82data;

static INT32 _MUSB_i4Init = 0;

static MUSB_Port *MGC_pUsbPort[MUSB_MAX_CONTROLLERS];
static MGC_Port *MGC_pImplPort[MUSB_MAX_CONTROLLERS];

static UINT8 MGC_aUsbPeripheralList[100];

/*
Class evice driver entry.
*/
static MUSB_DeviceDriver MGC_aDeviceDriver[MUSB_MAX_DRV_NUM];

static MUSB_HostClient MGC_UsbHostClient =
{
    MGC_aUsbPeripheralList, /* peripheral list */
    0,                      /* filled in main */
    MGC_aDeviceDriver,
    0,                      /* filled in main */
    0,                      /* dwHostError */
    NULL,                /* void* pPrivateData; may be NULL in callback function */  
    NULL                 /* MUSB_pfHostError pfHostError */    
};

static pfnUsbAsyncCb MGC_pfnUsbAsyncCb = NULL;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** _MUSB_SetPortMode
 *  Set USB port to host or device mode function.
 *  @param  u4PortIndex   port index 0 ~ (MUSB_CountPorts() - 1).
 *  @param  u4Type	    
            1: USB_PORT_TYPE_FUNCTION
            2: USB_PORT_TYPE_HOST            
 *  @retval  0 	SUCCESS.
 *  @retval  -1	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSB_SetPortMode(UINT32 u4PortIndex, UINT32 u4Type)
{
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;

    if ((!_MUSB_i4Init) || (u4PortIndex >= MUSB_CountPorts()))
    {
        return -1;
    }

    pUsbPort = MUSB_GetPort(u4PortIndex);
    MUSB_ASSERT(pUsbPort);        
    pUsbPort->Type = (MUSB_PortType)u4Type;
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);

    // disable suspend before reset port.
    pPort->bWantSuspend = FALSE;
    pPort->pfProgramBusState(pPort);
    
    pPort->pfResetPort(pPort);
    
    if (u4Type == USB_PORT_TYPE_HOST)
    {
        // Turn on suspend mode in host mode.
        pPort->bWantSuspend = TRUE;
    
        pPort->bOtgState = (uint8_t)MUSB_A_WAIT_BCON;
    }
    else
    {
        pPort->bOtgState = (uint8_t)MUSB_AB_IDLE;
    }        

    pPort->bWantSession = TRUE;
    pPort->pfProgramBusState(pPort);

    LOG(0, "Set port-%d to %s mode.\n", 
        u4PortIndex, ((u4Type == USB_PORT_TYPE_HOST) ? "Host" : "Device"));        

    return 0;
}

//-------------------------------------------------------------------------
/** _MUSB_Init
 *  Set USB host or device mode function.
 *  @param  u4Mode		1: host, 0: device.
 *  @retval  0 	SUCCESS.
 *  @retval  -1	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSB_Init(UINT32 u4Mode)
{
    UINT8 *pList;

    uint16_t wCount, wSize, wRemain;
    UINT8 bDriver;
    MUSB_DeviceDriver *pDriver = NULL;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_Port *pUsbPort = NULL;
    uint16_t i = 0;
    MUSB_BusHandle hUsbSession;
    
    if (!_MUSB_i4Init)    
    {
        _MUSB_i4Init = TRUE;        

        if (!MUSB_InitSystem(5))
        {
            return -1;
        }

        /* fill driver table */
        bDriver = 0;
        wSize = 0;
        wCount = 0;
        wRemain = (uint16_t)sizeof(MGC_aUsbPeripheralList);
        pList = MGC_aUsbPeripheralList;
        wSize = MGC_FillStorageClassPeripheralList(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MGC_GetStorageClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }                                        
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }
        
#ifdef MUSB_HUB
        wSize = MGC_FillHubClassPeripheralList(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MGC_GetHubClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }                                                    
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }
#endif

#ifdef MUSB_HID
        wSize = MUSB_FillHidClassPeripheralList(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MUSB_GetHidClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }                                                    
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }
#endif

#ifdef MUSB_AUD
        wSize = MUSB_FillAudClassPeripheralList(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MUSB_GetAudClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }                                                    
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }
#endif

#ifdef MUSB_SICD
        // Note this should put in the last driver to hook on system.
        // Because it will get Microsoft OS descriptor in SICD driver for
        // device of class code = 0xFF or 0x00.
        for (i=0; i<MUSB_SICD_LIST_NUM; i++)
        {
            wSize = MUSB_FillSicdPeripheralList(pList, wRemain, i);
            MUSB_ASSERT(wSize < wRemain);
            pDriver = MUSB_GetSicdDriver();
            MUSB_ASSERT(pDriver);
            if (bDriver >= MUSB_MAX_DRV_NUM)
            {
                LOG(0, "Out of max driver size.\n", NULL);
                return -1;
            }                                                    
            MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), 
                pDriver, sizeof(MUSB_DeviceDriver));
            pList += wSize;
            wCount += wSize + 1;
            if (wRemain < (wSize + 1))
            {
                LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                return -1;
            }
            wRemain -= (wSize + 1);
            (*pList) = bDriver;
            pList ++;
        }
        bDriver ++;
#endif
    
        MGC_UsbHostClient.wPeripheralListLength = wCount;
        MGC_UsbHostClient.bDeviceDriverListLength = bDriver;

        for (i=0; i<MUSB_CountPorts(); i++)
        {
            pUsbPort = MUSB_GetPort(i);
            if (!pUsbPort)
            {
                return -1;
            }

            MGC_pUsbPort[i] = pUsbPort;

            /* start session */
            hUsbSession = MUSB_RegisterHostClient(pUsbPort, &MGC_UsbHostClient);
            if (!hUsbSession)
            {
                return -1;
            }
            MGC_pImplPort[i] = (MGC_Port *)hUsbSession;

            // fix warning.
            UNUSED(MGC_pUsbPort[i]);
            UNUSED(MGC_pImplPort[i]);
            
#ifdef MUSB_FUNCTION
            hUsbSession = MUSB_RegisterFunctionClient(pUsbPort, &MGC_GenericUsbFunctionClient);
            if (!hUsbSession)
            {
                return -1;
            }

            if (u4Mode == 0)
            {
                MUSB_GenericDevInit();
            }
#endif /* #ifdef MUSB_FUNCTION */

            // Set port mode.
            VERIFY(0 == _MUSB_SetPortMode(i, 
                ((u4Mode) ? USB_PORT_TYPE_HOST : USB_PORT_TYPE_FUNCTION)));
        }

        // get Hfi driver structure.
        pHfiDriver = MUSB_HfiDriverGet();
        if (!pHfiDriver)
        {
            return -1;
        }

        // init device.
        if (0 > pHfiDriver->pfInitDevice(0))
        {
            return -1;
        }

#ifdef MUSB_SICD
        pHfiDriver = MUSB_HfiSicdDriverGet();
        if (!pHfiDriver)
        {
            return -1;
        }

        // init device.
        if (0 > pHfiDriver->pfInitDevice(0))
        {
            return -1;
        }
#endif /* #ifdef MUSB_SICD */

        return 0;
    }

    if (u4Mode)
    {
        for (i=0; i<MUSB_CountPorts(); i++)
        {
            VERIFY(0 == _MUSB_SetPortMode(i, USB_PORT_TYPE_HOST));
        }
    }

    return 0;
}
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** _MUSB_82WPComplete
 *  USB MSD checking write protect complete call back function.
 *  @param  pvHfiNfyTag: call back notify tag.
 *  @param  e_nfy_cond: read write operation condition.
 *  @param  pv_data: io data structure.
 *  @retval  void.
 */
//-------------------------------------------------------------------------
static void _MUSB_82WPComplete(void *pvHfiNfyTag, int32_t e_nfy_cond, void *pv_data)
{
    MUSB_HfiMediumInfo *pMediumInfo;
    MUSB_IO_DATA_T *prMUSBData;    
    MGC_ScsiCmdSetData* pScsi;
    uint8_t *pModeSenseData;
    MUSB_HfiAccessType AccessType = MUSB_HFI_ACCESS_RANDOM;

    MUSB_ASSERT(pvHfiNfyTag);
    MUSB_ASSERT(pv_data);
    
    pMediumInfo = (MUSB_HfiMediumInfo *)pvHfiNfyTag;
    prMUSBData = (MUSB_IO_DATA_T *)pv_data;
    MUSB_ASSERT(prMUSBData->pvData);
    pScsi = (MGC_ScsiCmdSetData *)prMUSBData->pvData;
    pModeSenseData = pScsi->aModeSenseData;
    MUSB_ASSERT(pModeSenseData);
    
    if (pScsi->bModeSenseType == MGC_SCSI_MODE_SENSE10)
    {
        AccessType = (pModeSenseData[3] & 0x80)
                         ? MUSB_HFI_ACCESS_RANDOM_READ : MUSB_HFI_ACCESS_RANDOM;
    }
    else if (pScsi->bModeSenseType == MGC_SCSI_MODE_SENSE)
    {
        AccessType = (pModeSenseData[2] & 0x80)
                         ? MUSB_HFI_ACCESS_RANDOM_READ : MUSB_HFI_ACCESS_RANDOM;
    }
    else
    {
        AccessType = MUSB_HFI_ACCESS_RANDOM_READ;
    }
    
    pMediumInfo->AccessType = AccessType;
   
    _MUSB_82Event = MUSB_82_MSD_WRITE;

    UNUSED(e_nfy_cond);
}
//-------------------------------------------------------------------------
/** _MUSB_82RWComplete
 *  8226 porting layer: USB MSD read/write complete call back function in single thread environment.
 *  @param  pvHfiNfyTag: call back notify tag.
 *  @param  e_nfy_cond: read write operation condition.
 *  @param  pv_data: io data structure.
 *  @retval  void.
 */
//-------------------------------------------------------------------------
static void _MUSB_82RWComplete(void *pvHfiNfyTag, int32_t e_nfy_cond, void *pv_data)
{
#ifndef NDEBUG
    if ((e_nfy_cond != MUSB_HFI_COND_WRITE_OK) && (e_nfy_cond != MUSB_HFI_COND_READ_OK))
    {
        LOG(1, "RW complete error = %d.\n", e_nfy_cond);
    }
#endif    
    _MUSB_82Event = MUSB_82_MSD_END;
}

//-------------------------------------------------------------------------
/** fgUsbSetMediaInUse
 *  8226 porting layer: check lun in device is in used function.
 *  @param  bMediaNum lun number.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
BOOL fgUsbSetMediaInUse(BYTE bMediaNum)
{
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    UINT32 u4Unit = bMediaNum;
    INT32 i4Status;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return 0;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Unit, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return 0;
    }

    // medium absent or medium is in used.
    if (HFI_STATUS_READY != i4Status)
    {
        return 0;
    }

    return 1;
}

//-------------------------------------------------------------------------
/** fgUsbSetMediaInUse
 *  8226 porting layer: check lun in device is in used function.
 *  @param  bMediaNum lun number.
 *  @retval  0 	USBM_MEDIA_STS_ABSENT.
 *  @retval  1 	USBM_MEDIA_STS_PRESENT.
 *  @retval  2	USBM_MEDIA_STS_WR_PROTECT.
 *  @retval  3   USBM_MEDIA_STS_ERROR. 
 */
//-------------------------------------------------------------------------
INT32 eUsbMediaStatus(BYTE bMediaNum)
{
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    UINT32 u4Unit = bMediaNum;
    INT32 i4Status;
    MUSB_HfiMediumInfo *pMediumInfo;

    if (!_MUSB_i4Init)
    {
        return 0; //USBM_MEDIA_STS_ABSENT
    }

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return 0; //USBM_MEDIA_STS_ABSENT
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Unit, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return 0; //USBM_MEDIA_STS_ABSENT
    }

    if (HFI_STATUS_READY != i4Status)
    {    
        return 0; //USBM_MEDIA_STS_ABSENT
    }

    // get medium information.
    if (0 > pHfiDriver->pfIoCtl(u4Unit, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {    
        return 3; //USBM_MEDIA_STS_ABSENT
    }

    if (pMediumInfo->AccessType == MUSB_HFI_ACCESS_RANDOM_READ)
    {
        return 2; //USBM_MEDIA_STS_WR_PROTECT
    }

    return 1; //USBM_MEDIA_STS_PRESENT;
}

//-------------------------------------------------------------------------
/** fgUsbMediaInfo
 *  8226 porting layer: get lun in device information.
 *  @param  bMediaNum lun number.
 *  @param  pDeviceInfoBuff  device informaiton buffer.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
#ifdef SS_WISELINK
BOOL fgUsbMediaInfo(BYTE bMediaNum, MUSB_HfiDeviceInfo *pDeviceInfoBuff)
{
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    UINT32 u4Unit = bMediaNum;
    INT32 i4Status;
    MUSB_HfiDeviceInfo *pDeviceInfo;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return 0; 
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Unit, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return 0; 
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return 0; 
    }

    // get device information.
    if (0 > pHfiDriver->pfIoCtl(u4Unit, (uint32_t)MUSB_IOCTL_GET_DEVICE_INFO, 0, &pDeviceInfo))
    {
        return NULL;
    }

    MUSB_MemCopy((void *)pDeviceInfoBuff, (void *)pDeviceInfo,
                 sizeof(MUSB_HfiDeviceInfo));

    return 1; 
}
#endif

//-------------------------------------------------------------------------
/** fgUsbIoChk
 *  8226 porting layer: check lun device status.
 *  @param  driveno lun number.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
BOOL fgUsbIoChk(int driveno)
{
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    UINT32 u4Unit = driveno;
    INT32 i4Status;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return 0;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Unit, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return 0;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return 0;
    }

    return 1;
}

//-------------------------------------------------------------------------
/** fgUsbIoRead
 *  8226 porting layer: read data from lun device.
 *  @param  driveno lun number.
 *  @param  sector  start offset.
 *  @param  buffer   data buffer pointer.
 *  @param  count   length of data size.
 *  @param  fd        file system depend variable(UNUSED).
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
BOOL fgUsbIoRead(int driveno, UINT32 sector, void  *buffer, UINT32 count, int fd)
{
    UINT32 lun = driveno;
    UINT32 block_size = 0;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;
    UINT32 u4RetryCount = 0;
    UINT32 u4MaxRetryCount = 0;
    MUSB_HfiDeviceInfo *pDeviceInfo;
    
    UNUSED(fd);

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return 0;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return 0;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return 0;
    }

    // get device block size.
    if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return 0;
    }

    // get device information.
    if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_INFO, 0, &pDeviceInfo))
    {
        return NULL;
    }

    // black list for retry handle.
    if ((pDeviceInfo->wVendorId == 0x058F) && (pDeviceInfo->wProductId == 0x9361))
    {
        u4MaxRetryCount = 20;
    }

    block_size = pMediumInfo->dwBlockSize;

    if (_MUSB_82Event != MUSB_82_IDLE)
    {
        LOG(0, "Sync read _MUSB_82Event error =%d.\n", _MUSB_82Event);
    }

    _MUSB_82Event = MUSB_82_MSD_READ;

    /*
    Printf("[%d] fgUsbIoRead sector=%d, count=%d.\n", 
        u4RetryCount, sector, count);                    
    */        

    while (1)
    {
        switch (_MUSB_82Event)
        {
        case MUSB_82_MSD_READ:
            _MUSB_82Event = MUSB_82_IDLE;
            /*
             * Read the data from the same sector which has written already
             */

            // Set read call back function.
            _rt_82nfy_info.pvHfiNfyTag = NULL;
            _rt_82nfy_info.pfHfiNfy = _MUSB_82RWComplete;
            if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_SET_TYPENFY, 0, &_rt_82nfy_info))
            {
                return 0;
            }

            // read operation.
            _rt_82data.u8BlkNum = sector;
            _rt_82data.u4Count = count * block_size;
            _rt_82data.u4TransCount = 0;
            _rt_82data.pvData = buffer;
            if (0 > pHfiDriver->pfRWBurst(lun, MUSB_HFI_OP_READ, (void *)&_rt_82data))
            {
                return 0;
            }
            break;

        // exit MSD testing loop.
        case MUSB_82_MSD_END:            
            if (_rt_82data.u4Count != _rt_82data.u4TransCount)
            {               
                /*
                Printf("[%d] fgUsbIoRead error: sector=%d, count=%d, TransCount=%d.\n", 
                    u4RetryCount, sector, count, _rt_82data.u4TransCount);                    
                */
                if (u4RetryCount++ < u4MaxRetryCount)
                {
                    _MUSB_82Event = MUSB_82_MSD_READ;
                    break;
                }
            }
            _MUSB_82Event = MUSB_82_IDLE;
            return (_rt_82data.u4Count == _rt_82data.u4TransCount);

        default:
            break;
        }

        // backgroup thread should alway be executed in every while(1).
        MUSB_NoneRunBackground();
    }
}

//-------------------------------------------------------------------------
/** fgUsbIoWrite
 *  8226 porting layer: write data to lun device.
 *  @param  driveno lun number.
 *  @param  sector  start offset.
 *  @param  buffer   data buffer pointer.
 *  @param  count   length of data size.
 *  @param  fd        file system depend variable(UNUSED).
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
BOOL fgUsbIoWrite(int driveno, UINT32 sector, void  *buffer, UINT32 count, int fd)
{
    UINT32 lun = driveno;
    UINT32 block_size = 0;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;

    UNUSED(fd);
    
    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return 0;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return 0;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return 0;
    }

    // get device block size.
    if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return 0;
    }

    block_size = pMediumInfo->dwBlockSize;

    if (_MUSB_82Event != MUSB_82_IDLE)
    {
        LOG(0, "Sync read _MUSB_82Event error =%d.\n", _MUSB_82Event);
    }

    _MUSB_82Event = MUSB_82_MSD_WRITE_PROTECT;

    //Printf("fgUsbIoWrite sector=%d, count=%d.\n", sector, count);                    

    while (1)
    {
        switch (_MUSB_82Event)
        {
        case MUSB_82_MSD_WRITE_PROTECT:
            _MUSB_82Event = MUSB_82_IDLE;

            // Set write protect call back function.
            _rt_82nfy_info.pvHfiNfyTag = pMediumInfo;
            _rt_82nfy_info.pfHfiNfy = _MUSB_82WPComplete;
            if (0 > pHfiDriver->pfIoCtl(lun, 
                    (uint32_t)MUSB_IOCTL_SET_TYPENFY, 0, &_rt_82nfy_info))
            {
                return 0;
            }

           /*
            * Check write protect of this device
            */
            MUSB_MemSet(&_rt_82data, 0, sizeof(MUSB_IO_DATA_T));
            
            if (0 > pHfiDriver->pfWriteProtect(lun, (void *)&_rt_82data))
            {
                return 0;
            }
            break;

        case MUSB_82_MSD_WRITE:
            _MUSB_82Event = MUSB_82_IDLE;
            if (pMediumInfo->AccessType != MUSB_HFI_ACCESS_RANDOM)
            {
                return 0;
            }

            /*
             * Read the data from the same sector which has written already
             */
            // Set read call back function.
            _rt_82nfy_info.pvHfiNfyTag = NULL;
            _rt_82nfy_info.pfHfiNfy = _MUSB_82RWComplete;
            if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_SET_TYPENFY, 0, &_rt_82nfy_info))
            {
                return 0;
            }

            // read operation.
            _rt_82data.u8BlkNum = sector;
            _rt_82data.u4Count = count * block_size;
            _rt_82data.u4TransCount = 0;
            _rt_82data.pvData = buffer;
            if (0 > pHfiDriver->pfRWBurst(lun, MUSB_HFI_OP_WRITE, (void *)&_rt_82data))
            {
                return 0;
            }
            break;

        // exit MSD testing loop.
        case MUSB_82_MSD_END: 
            /*            
            if (_rt_82data.u4Count != _rt_82data.u4TransCount)
            {               
                Printf("fgUsbIoWrite error: sector=%d, count=%d, TransCount=%d.\n", 
                    sector, count, _rt_82data.u4TransCount);                    
            }
            */
            
            _MUSB_82Event = MUSB_82_IDLE;
            return (_rt_82data.u4Count == _rt_82data.u4TransCount);

        default:
            break;
        }

        // backgroup thread should alway be executed in every while(1).
        MUSB_NoneRunBackground();
    }
}

static void _MUSB_82AsyncRWComplete(void *pvHfiNfyTag, int32_t e_nfy_cond, void *pv_data)
{
    if ((e_nfy_cond != MUSB_HFI_COND_WRITE_OK) && (e_nfy_cond != MUSB_HFI_COND_READ_OK))
    {
         _MUSB_82Event = MUSB_82_IDLE;
    
        LOG(0, "RW complete error = %d.\n", e_nfy_cond);

        // error
        if (MGC_pfnUsbAsyncCb)
        {
            MGC_pfnUsbAsyncCb(-1, NULL);
        }
        return;        
    }

    if (!MGC_pfnUsbAsyncCb)
    {
        LOG(0, "MGC_pfnUsbAsyncCb is NULL.\n", NULL);    

        _MUSB_82Event = MUSB_82_IDLE;

        // No call back is hooked.
        return;        
    }

    if ((_MUSB_82Event != MUSB_82_MSD_READ) && (_MUSB_82Event != MUSB_82_MSD_WRITE))
    {
        LOG(0, "Async call back _MUSB_82Event error =%d.\n", _MUSB_82Event);
        LOG(0, "RW count=%d != TransCount=%d.\n", 
            _rt_82data.u4Count, _rt_82data.u4TransCount);
    }
        
    if ((_MUSB_82Event == MUSB_82_MSD_READ) || (_MUSB_82Event == MUSB_82_MSD_WRITE))
    {
         _MUSB_82Event = MUSB_82_IDLE;

        if (_rt_82data.u4Count != _rt_82data.u4TransCount)
        {                     
            LOG(0, "RW count=%d != TransCount=%d.\n", 
                _rt_82data.u4Count, _rt_82data.u4TransCount);    
            
            MGC_pfnUsbAsyncCb(-1, NULL);
        }
        else
        {
            MGC_pfnUsbAsyncCb(_rt_82data.u4TransCount, NULL);
        }        
    }

    return;
}

UINT8 fgUsbIoAsyncRead(INT32 driveno, UINT32 sector, void  *buffer, UINT32 count, pfnUsbAsyncCb pfCb)
{
    UINT32 lun = driveno;
    UINT32 block_size = 0;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;
    MUSB_HfiDeviceInfo *pDeviceInfo;
    
    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return FALSE;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return FALSE;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return FALSE;
    }

    // get device block size.
    if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return FALSE;
    }

    // get device information.
    if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_INFO, 0, &pDeviceInfo))
    {
        return FALSE;
    }

    block_size = pMediumInfo->dwBlockSize;

    // Set read call back function.
    _rt_82nfy_info.pvHfiNfyTag = NULL;
    _rt_82nfy_info.pfHfiNfy = _MUSB_82AsyncRWComplete;
    if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_SET_TYPENFY, 0, &_rt_82nfy_info))
    {
        return FALSE;
    }

    if (_MUSB_82Event != MUSB_82_IDLE)
    {
        LOG(0, "Async read _MUSB_82Event error =%d.\n", _MUSB_82Event);
    }

    _MUSB_82Event = MUSB_82_MSD_READ;
    MGC_pfnUsbAsyncCb = pfCb;

    // read operation.
    _rt_82data.u8BlkNum = sector;
    _rt_82data.u4Count = count * block_size;
    _rt_82data.u4TransCount = 0;
    _rt_82data.pvData = buffer;
    if (0 > pHfiDriver->pfRWBurst(lun, MUSB_HFI_OP_READ, (void *)&_rt_82data))
    {
        _MUSB_82Event = MUSB_82_IDLE;
        MGC_pfnUsbAsyncCb = NULL;

        return FALSE;
    }

    return TRUE;
}

UINT8 fgUsbIoAsyncWrite(INT32 driveno, UINT32 sector, void  *buffer, UINT32 count, pfnUsbAsyncCb pfCb)
{
    UINT32 lun = driveno;
    UINT32 block_size = 0;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;
   
    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return FALSE;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return FALSE;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return FALSE;
    }

    // get device block size.
    if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return FALSE;
    }

    block_size = pMediumInfo->dwBlockSize;

    _rt_82nfy_info.pvHfiNfyTag = NULL;
    _rt_82nfy_info.pfHfiNfy = _MUSB_82AsyncRWComplete;
    if (0 > pHfiDriver->pfIoCtl(lun, (uint32_t)MUSB_IOCTL_SET_TYPENFY, 0, &_rt_82nfy_info))
    {
        return FALSE;
    }

    if (_MUSB_82Event != MUSB_82_IDLE)
    {
        LOG(0, "Async write _MUSB_82Event error =%d.\n", _MUSB_82Event);
    }
    
    _MUSB_82Event = MUSB_82_MSD_WRITE;
    MGC_pfnUsbAsyncCb = pfCb;

    // write operation.
    _rt_82data.u8BlkNum = sector;
    _rt_82data.u4Count = count * block_size;
    _rt_82data.u4TransCount = 0;
    _rt_82data.pvData = buffer;
    if (0 > pHfiDriver->pfRWBurst(lun, MUSB_HFI_OP_WRITE, (void *)&_rt_82data))
    {
        _MUSB_82Event = MUSB_82_IDLE;
        MGC_pfnUsbAsyncCb = NULL;
    
        return FALSE;
    }
    
    return TRUE;
}

UINT8 fgUsbIoAsyncAbort(void)
{
  MGC_pfnUsbAsyncCb = NULL;
  while (_MUSB_82Event != MUSB_82_IDLE && _MUSB_82Event != MUSB_82_INIT)
  {
    MUSB_NoneRunBackground();
  }

  return TRUE;
}
//-------------------------------------------------------------------------
/** dwUsbGetMediaSectSz
 *  8226 porting layer: get media sector size.
 *  @param  driveno lun number.
 *  @retval  sector size in bytes.
 */
//-------------------------------------------------------------------------
UINT32 dwUsbGetMediaSectSz(BYTE bLun)
{
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return 0;
    }

    // get device block size.
    if (0 > pHfiDriver->pfIoCtl(bLun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return 0;
    }

    return pMediumInfo->dwBlockSize;
}

//-------------------------------------------------------------------------
/** dwUsbGetTotalSocketNum
 *  8226 porting layer: get total number inserted socket.
 *  @param  driveno lun number.
 *  @retval  total socket number in system.
 */
//-------------------------------------------------------------------------
#ifdef SS_WISELINK
UINT32 dwUsbGetTotalSocketNum(void)
{
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    UINT32 u4Status;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return 0;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(0, (UINT32)MUSB_IOCTL_GET_TOTAL_MSD_SOCKET_NUM, 0, &u4Status))
    {
        return 0;
    }
    
    return u4Status;
}
#endif

//-------------------------------------------------------------------------
/** vUsbDevState
 *  8226 porting layer: usb background polling task.
 *  @param  void
 *  @retval  void.
 */
//-------------------------------------------------------------------------
void vUsbDevState(void)
{
    MUSB_NoneRunBackground();

#ifdef USB_UNIT_TEST
    MUSB_NoneOsMain();
#endif
}

//-------------------------------------------------------------------------
/** fgUsbSetPortMode
 *  Set USB port to host or device mode function.
 *  @param  u4PortIndex   port index 0 ~ (MUSB_CountPorts() - 1).
 *  @param  u4Type	    
            1: USB_PORT_TYPE_FUNCTION
            2: USB_PORT_TYPE_HOST            
 *  @retval  1 success.
 *  @retval  0 fail.
 */
//-------------------------------------------------------------------------
#ifdef SS_WISELINK
BOOL fgUsbSetPortMode(UINT32 u4PortIndex, UINT32 u4Type)
{
    if (0 == _MUSB_SetPortMode(u4PortIndex, u4Type))
    {
        return 1;
    }
    return 0;
}
#endif
//-------------------------------------------------------------------------
/** fgUsbInit
 *  8226 porting layer: usb system initial function.
 *  @param  void
 *  @retval  1 success.
 *  @retval  0 fail.
 */
//-------------------------------------------------------------------------
BOOL fgUsbInit(const MUSB_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info, 
    const MUSB_SOCKET_NFY_INFO_T *pt_socket_nfy_info)
{
    INT32 i4Ret;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    uint32_t i;

    i4Ret = _MUSB_Init(1);
    //i4Ret = _MUSB_Init(0);

    if (i4Ret  != 0)
    {
        return 0;
    }
    
    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return 0;
    }

    // init medium call back function.
    for (i=0; i<MUSB_UCFS_UNIT_COUNT; i++)
    {               
        if (0 > pHfiDriver->pfIoCtl(i, 
            (UINT32)MUSB_IOCTL_SET_SOCKET_DEV_NFY, 0, (void *)pt_hotswap_nfy_info))
        {
            return 0;
        }       
    }

    // set to report unsupport device status.
    for (i=0; i<MUSB_CountPorts(); i++)
    {
        if (0 > pHfiDriver->pfIoCtl(0, (UINT32)MUSB_IOCTL_SET_SOCKET_NFY, 
                i, (void *)pt_socket_nfy_info))
        {
            return 0;
        }
        pt_socket_nfy_info++;
    }

    _MUSB_82Event = MUSB_82_IDLE;
    return 1;    
}

//-------------------------------------------------------------------------
/** fgUsbDisable
 *  8226 porting layer: usb system shutdown function.
 *  @param  void
 *  @retval  1 success.
 *  @retval  0 fail.
 */
//-------------------------------------------------------------------------
BOOL fgUsbDisable(void)
{
    MUSB_Port *pUsbPort = NULL;
    MGC_Port *pPort = NULL;
    uint32_t i;

    for (i=0; i<MUSB_CountPorts(); i++)
    {
        LOG(5, "HS_HOST_SUSPEND Port%d.\n", i);    

        pUsbPort = MUSB_GetPort(i);
        MUSB_ASSERT(pUsbPort);        
        pPort = (MGC_Port *)pUsbPort->pPrivateData;
        MUSB_ASSERT(pPort);

        MUSB_SuspendBus((MUSB_BusHandle)pPort);
    }

    // Max Liao, 2008/05/06.
    // this delay must be added here to wait usb module handle suspend. 
    MUSB_Sleep(100);  

    return 1;
}

//-------------------------------------------------------------------------
/** fgUsbHidSetJoyStickInsertNfy
 *  8226 porting layer: usb HID set Joystick insert notify function.
 *  @param  pfHidJoystck Joystick notify function
 *  @retval  1 success.
 *  @retval  0 fail.
 */
//-------------------------------------------------------------------------
BOOL fgUsbHidSetJoyStickInsertNfy(MUSB_pfHidJoystickInsertNfy pfHidJoystck)
{
#ifdef MUSB_HID

    INT32 i4Return;

    // Set function test.
    i4Return = MUSB_HidIoctl(0, MUSB_HID_IOCTL_SET_JOYSTICK_INSERT_NOTIFY, 
        0, (void *)pfHidJoystck);
    if (i4Return < 0)
    {
        return 0;
    }        

#endif

    return 1;
}

//-------------------------------------------------------------------------
/** fgUsbHidSetJoyStickDataNfy
 *  8226 porting layer: usb HID set Joystick data notify function.
 *  @param  pfHidJoystck Joystick notify function
 *  @retval  1 success.
 *  @retval  0 fail.
 */
//-------------------------------------------------------------------------
BOOL fgUsbHidSetJoyStickDataNfy(MUSB_pfHidJoystickDataNfy pfHidJoystck)
{
#ifdef MUSB_HID

    INT32 i4Return;

    // Set function test.
    i4Return = MUSB_HidIoctl(0, MUSB_HID_IOCTL_SET_JOYSTICK_DATA_NOTIFY, 
        0, (void *)pfHidJoystck);
    if (i4Return < 0)
    {
        return 0;
    }     
    
#endif

    return 1;
}


#ifdef USB_UNIT_TEST
extern void *pvUSBMemPtr1;

//-------------------------------------------------------------------------
/** fgUsbIoUnitTest
 *  USB MSD unit test in single thread environment.
 *  @param  void.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
BOOL fgUsbIoUnitTest(int driveno, UINT32 sector_start_offset, UINT32 sector_num)
{
    UINT32 sector_offset = 0;
    UINT32 readonly = 0;
    UINT32 stress = 0;
    UINT32 lun = driveno;
    UINT32 test_size = 0x10000;
    UINT32 test_total_size = 0;
    INT32 block_size = 0;
    UINT8 *SrcBuf = (UINT8 *)pvUSBMemPtr1;
    UINT8 *RxBuf = (UINT8 *)((UINT32)pvUSBMemPtr1 + (UINT32)0x20000);
    UINT32 offset = 64;
    UINT32 i;
    UINT64 u8WriteRate = 0;
    UINT64 u8ReadRate = 0;
    INT32 i4Status;
    UINT32 start_time = 0;
    UINT32 stop_time = 0;

    LOG(1, "SrcBuf = 0x%08X.\n", (UINT32)SrcBuf);
    LOG(1, "RxBuf = 0x%08X.\n", (UINT32)RxBuf);

    sector_start_offset = (sector_start_offset < 0x80) ? 0x80 : sector_start_offset;

    if ((sector_start_offset + 0x80) < sector_num)
    {
        sector_num -= 0x80;
    }
    else
    {
        return 0;
    }

    //initial Src data.
    for (i = 0; i < (0x10000 + 64); i++)
    {
        *(SrcBuf + i) = i & 0xFF;
        //*(SrcBuf + i) = 0xFF;
        *(RxBuf + i) = 0;
    }

    // check device status.
    i4Status = eUsbMediaStatus(lun);   
    if ((i4Status == 0) || (i4Status == 3))
    {
        LOG(1, "[%d] status error = %d.\n", lun, i4Status);    
        return 0;
    }

    // check if write protect.
    readonly = (i4Status == 2);

    LOG(1, "[%d] %s.\n", lun, (readonly) ? ("WP enable"):("WP disable"));

    // get device block size.
    block_size = dwUsbGetMediaSectSz(lun);

    LOG(1, "[%d] block_size = %d.\n", lun, block_size);

    sector_offset = sector_start_offset;

    test_size = block_size;

    // clear total time stamp.
    vSetStc(0);    

    /* 
     * check write speed.
     */
    test_total_size = 0;
    test_size = block_size;
    start_time = dwReadStc();
    if (!readonly)
    {
        while (1)
        {
            test_total_size += test_size;     
            if (1 != fgUsbIoWrite(driveno, sector_start_offset, (void *)SrcBuf, (test_size/block_size), 0))
            {
                LOG(1, "Write error !");
                return 0;
            }
           
            // update test size.
            test_size += block_size;

            if (test_size > 65536)
            {
                stop_time = dwReadStc();                               
                u8WriteRate = ((UINT64)(test_total_size * 90))/((stop_time - start_time));
                LOG(0, "Write rate = %d KB/s.\n", u8WriteRate);

                break;                
            }            
        }
    }     

    // clear total time stamp.
    vSetStc(0);    

    /* 
     * check read speed.
     */
    test_total_size = 0;
    test_size = block_size;
    start_time = dwReadStc();
    while (1)
    {
        test_total_size += test_size;
        if (1 != fgUsbIoRead(driveno, sector_start_offset, (void *)SrcBuf, (test_size/block_size), 0))
        {
            LOG(1, "Read error !");
            return 0;
        }
           
        // update test size.
        test_size += block_size;

        if (test_size > 65536)
        {
            stop_time = dwReadStc();                                  
            u8ReadRate = ((UINT64)(test_total_size * 90))/((stop_time - start_time));
            LOG(0, "Read rate = %d KB/s.\n", u8ReadRate);

            break;                
        }            
    }

    LOG(1, "Start Write/Read data check...\n");
    //initial Src data.
    for (i = 0; i < (0x10000 + 64); i++)
    {
        *(SrcBuf + i) = i & 0xFF;
        //*(SrcBuf + i) = 0xFF;
        *(RxBuf + i) = 0;
    }

    test_size = block_size;
    while (1)
    {
        /*
         * Write a blcok of date into a sector
         */
        if (stress)
        {
            LOG(1, "[%d] offset = 0x%X, test_size = 0x%X.\n", lun, sector_offset, test_size);
        }
        
        if (!readonly)
        {
            if (1 != fgUsbIoWrite(driveno, sector_offset, (void *)(SrcBuf + offset), (test_size/block_size), 0))
            {
                LOG(1, "Write error !");
                return 0;
            }
            
            x_memset((void *)(RxBuf + offset), 0, test_size);
        }

        /*
         * Read the data from the same sector which has written already
         */
        if (1 != fgUsbIoRead(driveno, sector_offset, (void *)(RxBuf + offset), (test_size/block_size), 0))
        {
            LOG(1, "Read error !");
            return 0;
        }

        /* 
         * compare this data with what was written to find
         * out if there is an error in reading 
         */
        if (!readonly)
        {
            for (i = 0; i < test_size; i++)
            {
                if (SrcBuf[i + offset] != RxBuf[i + offset])
                {
                    LOG(1, "\nerror: test_size = 0x%X, i = 0x%X, src = 0x%X, rx = 0x%X.\n",
                        test_size, i,
                        SrcBuf[i + offset], RxBuf[i + offset]);
                    return 0;
                }
            }
        }      
        
        sector_offset += (test_size /block_size);

        if (sector_offset >= sector_num)
        {
            sector_offset = sector_start_offset;
        }

        // update test size.
        test_size += block_size;

        if (test_size > 65536)
        {
            if (!stress)
            {
                LOG(1, "Write/Read data check ok !\n");
                break;                
            }
            test_size = block_size;
        }

        offset++;
        if (offset > 64)
        {
            offset = 1;
        }
    }
    return 0;
}

//#ifdef MUSB_FUNCTION
#if 0
//-------------------------------------------------------------------------
/** _MUSB_NoneOsTxComplete
 *  USB CDC tx complete call back function in single thread environment.
 *  @param  void.
 *  @retval  void.
 */
//-------------------------------------------------------------------------

static void _MUSB_NoneOsTxComplete(uint32_t dwActualLength)
{
    _MUSB_Event = MUSB_82_CDC_RX;
}

//-------------------------------------------------------------------------
/** _MUSB_NoneOsRxComplete
 *  USB CDC rx complete call back function in single thread environment.
 *  @param  void.
 *  @retval  void.
 */
//-------------------------------------------------------------------------
static void _MUSB_NoneOsRxComplete(uint32_t dwActualLength)
{
    _MUSB_Event = MUSB_82_CDC_TX;
}

//-------------------------------------------------------------------------
/** _MUSB_NoneOsCdcTest
 *  USB CDC test in single thread environment.
 *  @param  void.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSB_NoneOsCdcTest(void)
{
    UINT32 status;

    UINT32 count = 1;
    UINT32 test_size = 65536;
    UINT32 xfercount = 1;
    UINT32 txcount = 0;
    UINT8 *RxBuf = (UINT8 *)0x03040000;
    UINT32 offset = 0;

    _MUSB_Event = MUSB_82_CDC_RX;

    while (1)
    {
        switch (_MUSB_Event)
        {
        case MUSB_82_CDC_RX:
            _MUSB_Event = MUSB_82_IDLE;            
            // check data by PC.
            //Printf("\nRX = %d, offset = %d.\n", count, offset);

            /*
            status = MGC_CdcRxData((UINT8 *)(RxBuf + offset), count, &xfercount,
                                   _MUSB_NoneOsRxComplete);
            */
            status = MGC_CdcRxNonBufferData((UINT8 *)(RxBuf + offset), count, &xfercount, _MUSB_NoneOsRxComplete);
            MUSB_ASSERT(status == 0);
            break;

        case MUSB_82_CDC_TX:
            _MUSB_Event = MUSB_82_IDLE;

            txcount = xfercount;
            /*        
                if (!(xfercount % maxpktsize))
                {
                    txcount++;
                }
            */
            // check data by PC.
            //Printf("\nTX = %d, offset = %d.\n", txcount, offset);

            status = MGC_CdcTxData((UINT8 *)(RxBuf + offset), txcount, &xfercount, _MUSB_NoneOsTxComplete);
            MUSB_ASSERT(status == 0);

            count++;
            count = (count > test_size) ? 1 : count;

            // set addres to round 0~16.
            offset++;
            if (offset > 32)
            {
                offset = 0;
            }
            break;

        // exit CDC testing loop.
        case MUSB_82_CDC_END:
            return 0;

        default:
            break;
        }

        // backgroud thread should alway be executed in every while(1).
        MUSB_NoneRunBackground();
    }
}
#endif

//-------------------------------------------------------------------------
/** MUSB_NoneOsMain
 *  USB entry point in single thread environment.
 *  @param  void.
 *  @retval  void.
 */
//-------------------------------------------------------------------------
void MUSB_NoneOsMain(void)
{
    int i;
    extern UINT32 _fgUSBTestLun[];

    for (i=0; i<12; i++)
    {
        if (_fgUSBTestLun[i])
        {
            LOG(1, "\nMUSB_NoneOsMain: test lun = %d.\n", i);
            _fgUSBTestLun[i] = FALSE;
            fgUsbIoUnitTest(i, 0x100, 0x500);
        }
    }
}
#endif

#endif

