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

/*
 * MUSBStack-S HID (Human Interface Device) definitions.
 * $Revision: #1 $
 */

#ifndef __MUSB_HID_H__
#define __MUSB_HID_H__

#include "mu_dev.h"
#include "mu_tools.h"

//#define MUSB_HID_DEBUG

#define MUSB_DT_HID			(MUSB_TYPE_CLASS | 0x01)
#define MUSB_DT_REPORT			(MUSB_TYPE_CLASS | 0x02)
#define MUSB_DT_PHYSICAL		(MUSB_TYPE_CLASS | 0x03)
#define MUSB_DT_HUB			(MUSB_TYPE_CLASS | 0x09)

#define MUSB_DT_HID_SIZE		9

/*
* HID Class Descriptor Types
*/

#define MUSB_HID_DESCRIPTOR		0x21
#define MUSB_REPORT_DESCRIPTOR		0x22
#define MUSB_PHYSICAL_DESCRIPTOR	0x23

/*
* HID requests
*/

#define MUSB_REQ_GET_REPORT		0x01
#define MUSB_REQ_GET_IDLE		0x02
#define MUSB_REQ_GET_PROTOCOL		0x03
#define MUSB_REQ_SET_REPORT		0x09
#define MUSB_REQ_SET_IDLE		0x0A
#define MUSB_REQ_SET_PROTOCOL		0x0B

/*
*  HID Report Types
*/

#define MUSB_HID_INPUT_REPORT		0x00
#define MUSB_HID_OUTPUT_REPORT		0x01
#define MUSB_HID_FEATURE_REPORT		0x02

/* HID descriptor */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdHID;
    uint8_t bCountryCode;
    uint8_t bNumDescriptors;
    uint8_t bClassDescriptorType;
    uint16_t wDescriptorLength;
} MUSB_HidDescriptor;
#include "mu_pkoff.h"

/* HID optional descriptor */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bDescriptorType;
    uint16_t wDescriptorLength;
} MUSB_HidOptionalDescriptor;
#include "mu_pkoff.h"

/* Configuration option for HID  DRIVER */

/* This is used for Push/Pop Global Items of HID Report. If PUSH Commands
 * for Globals Items is coming n times before POP, then value of n
 * needs to be used for MAX_GST_STACK_SIZE
 */
#define MAX_GST_STACK_SIZE            0x03

/* This is used for Push/Pop Collection items of HID Report. If START
 * COLLECTION comes n times before END Collection, then value of n
 * needs to be used for MAX_COLLECTION_STACK_SIZE
 */
#define MAX_COLLECTION_ALLOWED        0x0A 

/* */
#define MAX_COLLECTION_CHILD_ALLOWED  0x03

/*   */
#define  MAX_ID_ALLOWED               0x03 
/*  */
#define  MAX_BASIC_REPORT_ALLOWED     0x5  

/*  */
#define  MAX_USAGE_ALLOWED     10  

/* Following is used for storing Keystrokes. If Application is not
 *  reading fast enough, this value can be changed. This buffer is used
 *  as Circular Buffer. If it is FULL, Old Data would be overwritten.
 */
#define MAX_KB_BUFFER_SIZE            0x20 

/*** End of Configuration Consants for HID Driver ****/

/* Mask used to get size, type and tag of an ITEM */
#define ITEM_SIZE_MASK                 0x03
#define ITEM_TYPE_MASK                 0x0C
#define ITEM_TAG_MASK                  0xF0

/*  Different types of ITEM  */
#define ITEM_TYPE_MAIN                 0x00
#define ITEM_TYPE_GLOBAL               0x01
#define ITEM_TYPE_LOCAL                0x02
#define ITEM_TYPE_RESERVED             0x03

/* ITEM Tag for MAIN ITEM */
#define MAIN_ITEM_TAG_INPUT            0x08
#define MAIN_ITEM_TAG_OUTPUT           0x09
#define MAIN_ITEM_TAG_COLLECTION       0x0A
#define MAIN_ITEM_TAG_FEATURE          0x0B
#define MAIN_ITEM_TAG_END_COLLECTION   0x0C

#define MAIN_COLLECTION_PHYSICAL           0x00
#define MAIN_COLLECTION_APPLICATION     0x01
#define MAIN_COLLECTION_LOGICAL             0x02
#define MAIN_COLLECTION_REPORT              0x03
#define MAIN_COLLECTION_NAMEDARRAY    0x04
#define MAIN_COLLECTION_USAGESWITCH   0x05
#define MAIN_COLLECTION_USAGEMODIFY   0x06

/* ITEM Tag for GLOBAL ITEM   */
#define GLOBAL_ITEM_TAG_USAGE_PAGE     0x00
#define GLOBAL_ITEM_TAG_LOGICAL_MIN    0x01  
#define GLOBAL_ITEM_TAG_LOGICAL_MAX    0x02
#define GLOBAL_ITEM_TAG_PHYSICAL_MIN   0x03
#define GLOBAL_ITEM_TAG_PHYSICAL_MAX   0x04
#define GLOBAL_ITEM_TAG_UNIT_EXPONENT  0x05
#define GLOBAL_ITEM_TAG_UNIT           0x06
#define GLOBAL_ITEM_TAG_REPORT_SIZE    0x07
#define GLOBAL_ITEM_TAG_REPORT_ID      0x08
#define GLOBAL_ITEM_TAG_REPORT_COUNT   0x09 
#define GLOBAL_ITEM_TAG_PUSH           0x0A 
#define GLOBAL_ITEM_TAG_POP            0x0B 

#define USAGE_PAGE_GENERIC_DESKTOP      0x01
#define USAGE_PAGE_BUTTON                          0x09

/* Generic Desktop Page (0x01) table */
#define GDP_USAGE_UNDEFINED 0x0
#define GDP_USAGE_POINTER 0x01
#define GDP_USAGE_MOUSE 0x02
/* 0x03 RESERVED*/
#define GDP_USAGE_JOYSTICK 0x04
#define GDP_USAGE_GAMEPAD 0x05
#define GDP_USAGE_KEYBOARD 0x06
#define GDP_USAGE_KEYPAD 0x07
#define GDP_USAGE_MULTIAXISCONTROLLER 0x08
#define GDP_USAGE_TABLETPCSYS 0x09
/* 0x0A ~ 0x2F RESERVED*/
#define GDP_USAGE_X 0x30
#define GDP_USAGE_Y 0x31
#define GDP_USAGE_Z 0x32
#define GDP_USAGE_RX 0x33
#define GDP_USAGE_RY 0x34
#define GDP_USAGE_RZ 0x35
#define GDP_USAGE_SLIDER 0x36
#define GDP_USAGE_DIAL 0x37
#define GDP_USAGE_WHEEL 0x38
#define GDP_USAGE_HATSWITCH  0x39
#define GDP_USAGE_COUNTEDBUFFER 0x3A
#define GDP_USAGE_BYTECOUNT 0x3B
#define GDP_USAGE_MOTIONWAKEUP 0x3C
#define GDP_USAGE_START 0x3D
#define GDP_USAGE_SELECT 0x3E
/* 0x3F RESERVED*/
#define GDP_USAGE_VX 0x40
#define GDP_USAGE_VY 0x41
#define GDP_USAGE_VZ 0x42
#define GDP_USAGE_VBRX 0x43
#define GDP_USAGE_VBRY 0x44
#define GDP_USAGE_VBRZ 0x45
#define GDP_USAGE_VNO 0x46
#define GDP_USAGE_FEATURENOTIFICATION 0x47

/* ITEM Tag for LOCAL ITEM   */
#define LOCAL_ITEM_TAG_USAGE           0x00
#define LOCAL_ITEM_TAG_USAGE_MIN       0x01
#define LOCAL_ITEM_TAG_USAGE_MAX       0x02    
#define LOCAL_ITEM_TAG_USAGE_DES_MIN   0x03
#define LOCAL_ITEM_TAG_USAGE_DES       0x04    
#define LOCAL_ITEM_TAG_USAGE_DELIMITER 0x0A                     

/* Descriptor  related to HID */
#define HID_DSCR_TYPE                  0x21
#define HID_REPORT_DSCR                0x22

/* HID report type  */
#define IN_REPORT                      0x00
#define OUT_REPORT                     0x01
#define FEATURE_REPORT                 0x02
#define COLLECTION                     0x03
#define END_COLLECTION                 0x04

#define HID_NUMBER_DSCR_OFFSET         0x05
#define USB_HID_DSCR_NOT_VALID           -2
#define USB_WRONG_ITEM_FOUND             -3


/* In USAGE TABLE document Presently maximum usage is availabel is 0x91 */
#define HID_MAX_USAGE                  0x91

#define LONG_ITEM_TAG                  0x0F

#define SHORT_ITEM                     0x00
#define LONG_ITEM                      0x01

#define USB_MAX_COLLECTION_CHILD_USED   -10
#define USB_STACK_FULL                  -11
#define USB_MAX_ID_REACHED              -12
#define USB_MAX_BASIC_REPORT_REACHED    -13

/*
 * Definitions for number of bits in byte, short and int
 */
#define     MAX_BITS_BYTE    (8)
#define     MAX_BITS_SHORT   (16)
#define     MAX_BITS_3BYTE   (24)
#define     MAX_BITS_INT     (32)

/*
 * Definions for position value of Byte, short and int
 */
#define     POSITION_VALUE_8    (0x100)
#define     POSITION_VALUE_16   (uint32_t)(0x10000)
#define     POSITION_VALUE_24   (uint32_t)(0x1000000)

/*
 * Definitions for MASKS
 */
#define     BIT_MASK(n)    ( ~ (((uint32_t)0xFFFFFFFF) <<n ) )

#define     GET_USAGEPAGE(x)    (((x) & 0xFFFF0000) >> 16)
#define     GET_USAGETAG(x)    ((x) & 0xFFFF)

typedef struct 
{
    uint8_t bDescriptorType;
    uint8_t unused;
    uint16_t wDescriptorLength;
}REPORT_PHY_DSCR_LIST_STRUCT;

/*   Structure for HID descriptor    */
typedef struct 
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bContryCode;
    uint8_t bNumDescriptor;
    uint16_t bcdHID;
    uint16_t unused;
    /* According to number of dscr allocate memory */
    REPORT_PHY_DSCR_LIST_STRUCT *dscr_list_P;
}HID_DSCR_STRUCT;

/* Structure for storing long ITEM information */
typedef struct  
{
    uint8_t bDataSize;
    uint8_t bLongItemTag;
    uint8_t *long_item_data_P;
}LONG_ITEM_INFO_STRUCT;

/* Structure for storing ITEM information */
typedef struct 
{
    uint8_t bPrefix;
    uint8_t      bSize;
    uint8_t      bType;
    uint8_t      bTag;
    uint32_t item_data;
}ITEM_INFO_STRUCT;

/* Structure for storing State of Global ITEM */
typedef struct 
{
    uint32_t report_id;
    uint32_t report_size;
    uint32_t report_count;
    uint32_t usage_page;
    uint32_t logical_min;
    uint32_t logical_max;
    uint32_t physical_min;
    uint32_t physical_max;
    uint32_t unit_exponent;
    uint32_t unit;
}GLOBAL_STATE_TABLE_STRUCT;

/* Structure for storing State of LOCAL ITEM */
typedef struct 
{
    uint32_t usage[MAX_USAGE_ALLOWED]; 
    uint32_t usage_num;    
    uint32_t usage_min;
    uint32_t usage_max;
    uint8_t bUsage;  /*TRUE: usage list is using. FALSE: min and max type is using.*/
}LOCAL_STATE_TABLE_STRUCT;

/* Structure for collection in tree structure  */
typedef struct COLLECTION_S
{
    uint8_t bCollectType;
    uint8_t bTotalChild; /* Should be initialized to 0 */
    struct COLLECTION_S *pParent;
    struct COLLECTION_S *pChild[MAX_COLLECTION_CHILD_ALLOWED];
}COLLECTION_STRUCT;
    
/* Structure for storing state of ITEM and used when MAIN ITEM encounter  */
typedef struct 
{   
    void *pHidDevice;
    uint32_t dwGlobalItem[10]; /* Global item with tag: 0 ~ 9 */
    LOCAL_STATE_TABLE_STRUCT LocalItem;
    COLLECTION_STRUCT   *pCollectionBuf[MAX_COLLECTION_ALLOWED];  
    uint8_t bCollectIndex;
}ITEM_STATE_TABLE_STRUCT;


/* Structure for storing actual reports. 
 * main_data  : represents DATA|CONST, ARRAY|VAR, ABSOLUTE|RELATIVE
 * collection : Pointer to collection related to this report.
 */

typedef struct 
{
    uint32_t usage_page;
    uint32_t logical_min;
    uint32_t logical_max;
    uint32_t physical_min;
    uint32_t physical_max;
    uint32_t unit_exponent;
    uint32_t unit;
    uint32_t report_size;
    uint32_t report_id;
    uint32_t report_count;    
    LOCAL_STATE_TABLE_STRUCT LocalItem;
    uint32_t main_data;  
    uint32_t report_offset;
    COLLECTION_STRUCT *collection; 
    struct id_report_struct_  *pIdReport;  
}BASIC_REPORT_STRUCT;

typedef struct id_report_struct_ 
{
    uint32_t report_id;
    BASIC_REPORT_STRUCT *pBasicReport[MAX_BASIC_REPORT_ALLOWED]; 
    uint32_t dwBasicReportNum;
    uint32_t dwTotalBasicReportSize;
}ID_REPORT_STRUCT;

/* Structure to store all the IN/OUT/FEATURE(one of them at a time)
 * reports belongs to one ID.
 */
typedef struct 
{
    uint8_t bTotalId;
    ID_REPORT_STRUCT *pIdReport[MAX_ID_ALLOWED]; 
}REPORT_STRUCT;


#define MUSB_HID_CTRL_LENGTH             0x0     

/** State of Hid driver       */
#define MUSB_HID_STATE_FREE                         ((uint8_t)0x00)
#define MUSB_HID_STATE_CONNECT                  ((uint8_t)0x01)
#define MUSB_HID_STATE_CONFIGURED           ((uint8_t)0x02)
#define MUSB_HID_STATE_SET_IDLE                  ((uint8_t)0x03)
#define MUSB_HID_STATE_REPORT_DESCR       ((uint8_t)0x04)
#define MUSB_HID_STATE_START_IN                 ((uint8_t)0x05)

/* Macro to prepare setup packet for Hid Class driver*/
#define MGC_PREPARE_SETUP_PACKET(pSetup,\
                                     bmActualRequestType,\
                                     bActualRequest,\
                                     wActualValue,\
                                     wActualIndex,\
                                     wActualLength)\
{\
    (pSetup)->bmRequestType = (uint8_t) bmActualRequestType;\
    (pSetup)->bRequest      = (uint8_t) bActualRequest;\
    (pSetup)->wValue        = (uint16_t) MUSB_SWAP16(wActualValue);\
    (pSetup)->wIndex        = (uint16_t) MUSB_SWAP16(wActualIndex);\
    (pSetup)->wLength       = (uint16_t) MUSB_SWAP16(wActualLength);\
}

/* Macro to fill control Irp for Hid driver */
#define MGC_FILL_CONTROL_IRP(pTargetDevice,\
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

#endif	/* multiple inclusion protection */
