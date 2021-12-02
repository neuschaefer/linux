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

#ifndef _MUSB_HID_IF_H_
#define _MUSB_HID_IF_H_

#include "mu_hid.h"

#define MGC_HID_JOYSTICK_NUM   (4)

/** 
 * MUSB_HidIoctlCmdType.
 * HFI Ioctl command Type.
 */
typedef enum
{
    MUSB_HID_IOCTL_GET_DEVICE_NUM,     // total number of HID devices on our system.
    MUSB_HID_IOCTL_GET_DEVICE_PORT,   // number of ports on the each HID device.
    MUSB_HID_IOCTL_GET_DEVICE_STATUS,

    /* JoyStick related data */
    MUSB_HID_IOCTL_GET_JOYSTICK_DATA,
    MUSB_HID_IOCTL_GET_JOYSTICK_NUM,
    MUSB_HID_IOCTL_SET_JOYSTICK_INSERT_NOTIFY,
    MUSB_HID_IOCTL_SET_JOYSTICK_DATA_NOTIFY,
    
    MUSB_HID_IOCTL_END
} MUSB_HidIoctlCmdType;

typedef struct 
{
    uint32_t dwX;
    uint32_t dwY;
    uint32_t dwButton; /* Max 32 buttons in bit field to get 1: press, 0: release */

    /* Value range */
    uint8_t   bXYMin; 
    uint8_t   bXYMax; 
    uint8_t   bButtonBitsNum; 

    /* Count the change index */    
    uint32_t  Index;  
}JOYSTICK_INFO_STRUCT;

typedef void (*MUSB_pfHidJoystickInsertNfy)(uint8_t bJoystickId, 
    uint8_t bInsert);

typedef void (*MUSB_pfHidJoystickDataNfy)(uint8_t bJoystickId, 
    JOYSTICK_INFO_STRUCT *pJoyStick);

struct _MGC_HidDevice;

typedef struct 
{
    /* Joystick related object */
    uint8_t bIndex;
    uint8_t bHidDevId; /* The id index in total id */
    JOYSTICK_INFO_STRUCT rJoyStick;
    MUSB_pfHidJoystickDataNfy pfHidJoystck;
    struct _MGC_HidDevice *pHidDevice;
} MGC_HidJoystick;

typedef struct _MGC_HidDevice
{
    /* USB hid device basic information object */
    uint8_t bDeviceIndex;
    MUSB_BusHandle hBus;   /* MGC_Port* pImplPort */
    MUSB_Device *pUsbDevice;
    MUSB_DeviceDriver *pDriver;
    MUSB_ConfigurationDescriptor *pConfig;
    MUSB_InterfaceDescriptor *pIface;
    MUSB_HidDescriptor *pHidDescr;
    MUSB_EndpointDescriptor *pEnd;
    uint8_t *pReportDescr;

    /* USB transfer related object */
    uint8_t aSetupTx[8+MUSB_HID_CTRL_LENGTH];
    MUSB_ControlIrp ControlIrp;
    MUSB_Irp intrInIrp;
    MUSB_Pipe hIntInPipe;
    uint8_t aIntInBuf[8];

    /* Common HID driver object */
    const CHAR *HidTypes;
    uint8_t bHidType;
    REPORT_STRUCT rReport[3]; /* 0 FOR IN, 1 for OUT, 2 for Feature */
    COLLECTION_STRUCT aCollection[MAX_COLLECTION_ALLOWED];
    uint8_t bCollectionNum;
    uint8_t bState;
    uint8_t bNextState;        

    /* Joystick, Keyboard related object */
    uint8_t bHidDriverNum;  /*Num of driver on the same hid device.*/
    void *pHidDriverTag;
} MGC_HidDevice;

/**
 * Fill an array with the targetted peripheral list entry appropriate
 * for the mass-storage class driver, ending with the MUSB_TARGET_ACCEPT.
 * @param pList array
 * @param wListLength how many bytes are available in the array
 * @return how many bytes were filled in the array.
 * If this equals bListLength, the caller should assume there is insufficient space
 * in the array and the list entry is incomplete.
 */
extern uint16_t MUSB_FillHidClassPeripheralList(uint8_t* pList, uint16_t wListLength);

/**
 * Get a pointer to the mass-storage class driver
 */
extern MUSB_DeviceDriver* MUSB_GetHidClassDriver(void);

extern int32_t MUSB_HidIoctl(uint32_t u4Index, uint32_t u4Cmd, uint32_t u4Aux, void *pBuffer);

/**
 * Joystick internal function.
 */
extern void MUSB_JoystickInit(void); 
extern void MUSB_JoystickRegister(MGC_HidDevice *pHidDevice);
extern void MUSB_JoystickRelease(MGC_HidDevice *pHidDevice);
extern uint32_t MUSB_JoystickEvent(MGC_HidDevice *pHidDevice);
/**
 * Joystick external function.
 */
extern uint32_t MUSB_JoystickCount(void);
extern void *MUSB_JoystickGetData(uint8_t bIndex, void *pBuf);
extern void MUSB_JoystickSetInsertNotify(void *pBuf);
extern void MUSB_JoystickSetDataNotify(void *pBuf);

 
#endif /* _MUSB_HID_IF_H_ */
