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

#ifndef _MUSB_AUD_API_H_
#define _MUSB_AUD_API_H_

/*
 * Prototypes for Audio class driver API
 * $Revision: #1 $
 */

#define MUSB_AUD_CTRL_LENGTH             0x4     

#define MUSB_AUD_INTERFACE                  0x2     

#define MUSB_AUD_CHANNEL                     0x3     

/* Debug message values for success and error */
#define MUSB_AUD_DIAG_ERROR                     0x1     
#define MUSB_AUD_DIAG_SUCCESS                   0x2

/* Format type codes */
#define MUSB_AUD_FORMAT_TYPE_UNDEF               0x00
#define MUSB_AUD_FORMAT_TYPE_I                   0x01
#define MUSB_AUD_FORMAT_TYPE_II                  0x02
#define MUSB_AUD_FORMAT_TYPE_III                 0x03

/* Audio Data Format Type I Codes */
#define MUSB_AUD_TYPE_I_UNDEFINED 0x0000
#define MUSB_AUD_PCM 0x0001
#define MUSB_AUD_PCM8 0x0002
#define MUSB_AUD_IEEE_FLOAT 0x0003
#define MUSB_AUD_ALAW 0x0004
#define MUSB_AUD_MULAW 0x0005

/* Audio class specific descriptor types */
#define MUSB_CS_UNDEFINED                            0x20
#define MUSB_CS_DEVICE                               0x21
#define MUSB_CS_CONFIGURATION                        0x22
#define MUSB_CS_STRING                               0x23
#define MUSB_CS_INTERFACE                            0x24
#define MUSB_CS_ENDPOINT                             0x25

/* ASI descriptor sub types */
#define MUSB_AS_GENERAL                              0x01
#define MUSB_FORMAT_TYPE                             0x02
#define MUSB_FORMAT_SPECIFIC                         0x03

/* Audio Interface Subclass Codes */
#define MUSB_SUBCLASS_UNDEFINED 0x00
#define MUSB_AUDIOCONTROL 0x01
#define MUSB_AUDIOSTREAMING 0x02
#define MUSB_MIDISTREAMING 0x03

/* USB terminal type */
#define MUSB_USB_TERMINAL                   0x0101

/* Aduio class specific end point descriptor subtypes */
#define MUSB_EP_GENERAL                              0x01

/* currently ASSOC interface descriptor values is not known. */
#define MUSB_ASSOC_INTERFACE                         0xFF

/** State of Audio       */
#define MUSB_AUD_STATE_FREE                         ((uint8_t)0x00)
#define MUSB_AUD_STATE_CONNECT                  ((uint8_t)0x01)
#define MUSB_AUD_STATE_CONFIGURED           ((uint8_t)0x02)
#define MUSB_AUD_STATE_SETOUTINF              ((uint8_t)0x03)
#define MUSB_AUD_STATE_UNMUTE                   ((uint8_t)0x05)
#define MUSB_AUD_STATE_GETMIN                   ((uint8_t)0x06)
#define MUSB_AUD_STATE_GETMAX                   ((uint8_t)0x07)
#define MUSB_AUD_STATE_GETRES                   ((uint8_t)0x08)
#define MUSB_AUD_STATE_GETCUR                  ((uint8_t)0x09)
#define MUSB_AUD_STATE_SETCUR                   ((uint8_t)0x0A)
#define MUSB_AUD_STATE_INT_END                   ((uint8_t)0x0B)
#define MUSB_AUD_STATE_READY                   ((uint8_t)0x0C)
#define MUSB_AUD_STATE_SETOUTFREQ             ((uint8_t)0x0D)
#define MUSB_AUD_STATE_SETINFREQ                ((uint8_t)0x0E)
#define MUSB_AUD_STATE_PLAYING                ((uint8_t)0x0F)
#define MUSB_AUD_STATE_RECORDING               ((uint8_t)0x10)
#define MUSB_AUD_STATE_SETOUTVOL                ((uint8_t)0x11)
#define MUSB_AUD_STATE_SETINVOL                   ((uint8_t)0x12)

/** Audio Class-Specific AS Interface Descriptor Subtypes */
#define MUSB_AS_DESCRIPTOR_UNDEFINED 0x00
#define MUSB_AS_GENERAL 0x01
#define MUSB_AS_FORMAT_TYPE 0x02
#define MUSB_AS_FORMAT_SPECIFIC 0x03

/** Audio Class-Specific AC Interface Descriptor Subtypes */
#define MUSB_AC_DESCRIPTOR_UNDEFINED 0x00
#define MUSB_AC_HEADER 0x01
#define MUSB_AC_INPUT_TERMINAL 0x02
#define MUSB_AC_OUTPUT_TERMINAL 0x03
#define MUSB_AC_MIXER_UNIT 0x04
#define MUSB_AC_SELECTOR_UNIT 0x05
#define MUSB_AC_FEATURE_UNIT 0x06
#define MUSB_AC_PROCESSING_UNIT 0x07
#define MUSB_AC_EXTENSION_UNIT 0x08

/** Audio Class-Specific Request Codes */
#define MUSB_AC_REQUEST_CODE_UNDEFINED 0x00
#define MUSB_AC_SET_CUR 0x01
#define MUSB_AC_GET_CUR 0x81
#define MUSB_AC_SET_MIN 0x02
#define MUSB_AC_GET_MIN 0x82
#define MUSB_AC_SET_MAX 0x03
#define MUSB_AC_GET_MAX 0x83
#define MUSB_AC_SET_RES 0x04
#define MUSB_AC_GET_RES 0x84
#define MUSB_AC_SET_MEM 0x05
#define MUSB_AC_GET_MEM 0x85
#define MUSB_AC_GET_STAT 0xFF

/** Feature Unit Control Selectors */
#define MUSB_AC_FU_CONTROL_UNDEFINED 0x00
#define MUSB_AC_MUTE_CONTROL 0x01
#define MUSB_AC_VOLUME_CONTROL 0x02
#define MUSB_AC_BASS_CONTROL 0x03
#define MUSB_AC_MID_CONTROL 0x04
#define MUSB_AC_TREBLE_CONTROL 0x05
#define MUSB_AC_GRAPHIC_EQUALIZER_CONTROL 0x06
#define MUSB_AC_AUTOMATIC_GAIN_CONTROL 0x07
#define MUSB_AC_DELAY_CONTROL 0x08
#define MUSB_AC_BASS_BOOST_CONTROL 0x09
#define MUSB_AC_LOUDNESS_CONTROL 0x0A

#define MUSB_AC_MUTE_LENGTH   0x01
#define MUSB_AC_VOLUME_LENGTH   0x02

#define MUSB_AC_MAX_VOLUME  0x7FFF
#define MUSB_AC_MID_VOLUME  0
#define MUSB_AC_MIN_VOLUME  0x8000

/** Endpoint Control Selectors */
#define MUSB_AC_EP_CONTROL_UNDEFINED 0x00
#define MUSB_AC_SAMPLING_FREQ_CONTROL 0x01
#define MUSB_AC_PITCH_CONTROL 0x02

/** Class-Specific AS Isochronous Audio Data Endpoint Descriptor */
#define MUSB_BM_SAMPLING_FREQ_MASK   (1<<0x00)
#define MUSB_BM_PITCH_MASK                      (1<<0x01)
#define MUSB_BM_MAXPKTSIZE_MASK           (1<<0x07)

#define MUSB_AC_SAMPLING_FREQ_LENGTH   0x03

/* Macro to prepare setup packet for Mass Storage Class driver*/
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

/* Macro to fill control Irp for Mass Storage Class driver */
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

typedef struct
{
    uint8_t bMute;
    uint16_t wMin;
    uint16_t wMax;
    uint16_t wRes;
    uint16_t wCur;
    uint32_t dwValueField; /* Total volume field vale */
    uint16_t wUsrCur;  /* Use normalized 0 ~ 100 */
} MUSB_ACVolume;

typedef struct
{
    uint16_t wFormatTag;
    uint8_t bNrChannels;
    uint8_t bBitResolution;
    uint32_t dwSamFreq;
} MUSB_AUDParam;

/**
 * Bulk/Interrupt IRP completion handler.
 * A completion handler for bulk/interrupt transfers.
 * @param 1 pCompleteParam from IRP
 * @param 2 IRP pointer
 */
struct _MUSB_AudIrp;
 
typedef void (*MUSB_pfAudIrpComplete)(void*, const struct _MUSB_AudIrp*);

typedef struct _MUSB_AudIrp
{
    uint8_t* pBuffer;
    uint32_t dwLength;
    MUSB_pfAudIrpComplete pfIrpComplete;
    void* pCompleteParam;
} MUSB_AUDIrp;

struct _MUSB_AudStreamIrp;

/// Callback handler of capture buffer
typedef void (*MUSB_pfAudStreamIrpComplete)(void*, const struct _MUSB_AudStreamIrp*);

typedef struct _MUSB_AudIsoPtr
{
    uint32_t dwRp;
    uint32_t dwWp;
    uint32_t dwWEp;  /// bottom of write pointer before wrapped.
    uint8_t bEmpty;  /// After read pointer increase, read = write pointer.
} MUSB_AudIsoPtr;

typedef struct _MUSB_AudStreamIrp
{
    uint8_t* pBuffer;                
    uint32_t dwBufSize;
    uint32_t dwThreshold;        
    MUSB_AudIsoPtr *prAudIsoPtr;
    MUSB_pfAudStreamIrpComplete pfIrpComplete;
    void* pCompleteParam;
} MUSB_AudStreamIrp;

/* All Audio standard descriptors have these 3 fields in common */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;    
} MUSB_AUDescriptorHeader;
#include "mu_pkoff.h"

/* Class-Specific AC Interface Header Descriptor (audio class 4.5.2) */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint16_t bcdADC;
    uint16_t wTotalLength;
    uint8_t bInCollection;
 } MUSB_ACInterfaceHeaderDescriptor; 
#include "mu_pkoff.h"

/* Class-Specific AC Input Terminal Descriptor (audio class 4.5.2) */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bTerminalID;
    uint16_t wTerminalType;
    uint8_t bAssocTerminal;
    uint8_t bNrChannels;
    uint16_t wChannelConfig;
    uint8_t iChannelNames;
    uint8_t iTerminal;
} MUSB_ACInputTerminalDescriptor; 
#include "mu_pkoff.h"

/* Class-Specific AC Output Terminal Descriptor (audio class 4.5.2) */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bTerminalID;
    uint16_t wTerminalType;
    uint8_t bAssocTerminal;
    uint8_t bSourceID;
    uint8_t iTerminal;
} MUSB_ACOutputTerminalDescriptor; 
#include "mu_pkoff.h"

/* Class-Specific AC Feature Unit Descriptor (audio class 4.5.2) */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bUnitID;
    uint8_t bSourceID;
    uint8_t bControlSize;
    uint8_t bmaControls[MUSB_AUD_CHANNEL];
} MUSB_ACFeatureUnitDescriptor; 
#include "mu_pkoff.h"

/* Class-Specific AS Interface Descriptor (audio class 4.5.2) */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bTerminalLink;
    uint8_t bDelay;
    uint8_t wFormatTag;
} MUSB_ASInterfaceDescriptor; 
#include "mu_pkoff.h"

/* Class-Specific AS Format Type Descriptor. 
     Type I Format Type Descriptor (audio class 4.5.2) */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bFormatType;
    uint8_t bNrChannels;
    uint8_t bSubframeSize;
    uint8_t bBitResolution;
    uint8_t bSamFreqType;

} MUSB_ASFormatTypeDescriptor; 
#include "mu_pkoff.h"

/* Audio Streaming Class Specific Audio Data Endpoint Descriptor */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubType;
    uint8_t bmAttributes;
    uint8_t bLockDelayUnits;
    uint16_t wLockDelay;
} MUSB_ASDataEndpointDescriptor;
#include "mu_pkoff.h"

typedef struct 
{
    MUSB_BusHandle             hBus;   /* MGC_Port* pImplPort */
    MUSB_Device                   *pUsbDevice;
    MUSB_DeviceDriver           *pDriver;
    uint8_t aSetupTx[8+MUSB_AUD_CTRL_LENGTH];
    MUSB_ControlIrp               ControlIrp;

    /* Audio driver control state */
    uint8_t                             bState;
    uint8_t                             bNextState;    

    MUSB_ConfigurationDescriptor *pConfig;
    MUSB_InterfaceDescriptor *pControlIfaceHead;
    MUSB_InterfaceDescriptor *pASOutIfaceHead;
    MUSB_InterfaceDescriptor *pASInIfaceHead;

    /* Feature control */
    MUSB_ACFeatureUnitDescriptor *pItFeature;
    MUSB_ACFeatureUnitDescriptor *pOtFeature;
    uint8_t bItCtrlNum;    /*Include Master channel.*/
    uint8_t bOtCtrlNum;   /*Include Master channel.*/
    uint8_t bItCtrlIdx;      
    uint8_t bOtCtrlIdx;      
    
    /* Volume control */    
    MUSB_ACVolume rItVolume[MUSB_AUD_CHANNEL];    
    MUSB_ACVolume rOtVolume[MUSB_AUD_CHANNEL];    
    
    /* Selected interface */
    MUSB_InterfaceDescriptor *pASOutIface;
    MUSB_InterfaceDescriptor *pASInIface;

    MUSB_EndpointDescriptor *pIsoOutEnd; /* Working Out endpoint */
    MUSB_EndpointDescriptor *pIsoInEnd;   /* Working In endpoint */   

    MUSB_ASDataEndpointDescriptor *pIsoASOutEnd; /* Working AS Out endpoint */
    MUSB_ASDataEndpointDescriptor *pIsoASInEnd;   /* Working AS In endpoint */
        
    MUSB_Pipe hIsoInPipe;
    MUSB_Pipe hIsoOutPipe;

    MUSB_IsochIrp InIsochIrp;
    MUSB_IsochIrp OutIsochIrp;
    
    MUSB_AUDParam *prIsoOutParm;  /* Play out data structure */
    MUSB_AUDParam *prIsoInParm;  /*  Record in data structure */    
} MGC_AudDevice;

/**
 * Fill an array with the targetted peripheral list entry appropriate
 * for the mass-storage class driver, ending with the MUSB_TARGET_ACCEPT.
 * @param pList array
 * @param wListLength how many bytes are available in the array
 * @return how many bytes were filled in the array.
 * If this equals bListLength, the caller should assume there is insufficient space
 * in the array and the list entry is incomplete.
 */
extern uint16_t MUSB_FillAudClassPeripheralList(uint8_t* pList, uint16_t wListLength);

/**
 * Get a pointer to the mass-storage class driver
 */
extern MUSB_DeviceDriver* MUSB_GetAudClassDriver(void);

/** Connect handler for mass-storage class driver */
extern uint8_t MUSB_AudConnect(void *pPrivateData, 
                                 MUSB_BusHandle     hBus,
                                 MUSB_Device       *pUsbDevice,
                                 const uint8_t     *pPeripheralList);

/** Disconnect Handler for Mass Storage Device Driver */
extern uint8_t MUSB_AudDisconnect (void *pPrivateData, 
                                MUSB_BusHandle  hBus,
                                MUSB_Device    *pUsbDevice);

extern uint32_t MUSB_AudVolumeSet(uint32_t dwIndex, 
    uint8_t bIsPlay, uint16_t wVolume);

extern uint32_t MUSB_AudStreamSet(uint32_t dwIndex, 
    MUSB_AUDParam *prPlayParm, MUSB_AudStreamIrp *prAudStreamIrp);

extern uint32_t MUSB_AudStreamGet(uint32_t dwIndex, 
    MUSB_AUDParam *prPlayParm, MUSB_AudStreamIrp *prAudStreamIrp);

#endif /* _MUSB_AUD_API_H_ */
