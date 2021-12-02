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

#ifndef _MUSB_SICD_UTIL_H_
#define _MUSB_SICD_UTIL_H_

#include "mu_sicderrorcode.h"
#include "mu_sicd.h"
#include "mu_hfi_sicd_if.h"
#include "mu_sicdapi.h"
#include "mu_sicdstate.h"

/*
SICD-->class specific requests
*/
#define MUSB_SICD_CANCEL_REQUEST_CODE           0x64
#define MUSB_SICD_GET_EXT_EVENT_REQUEST_CODE    0x65
#define MUSB_SICD_RESET_REQUEST_CODE            0x66
#define MUSB_SICD_GET_STATUS_REQUEST_CODE       0x67

#define MUSB_SICD_DEVICE_STATUS_OK                   0x2001
#define MUSB_SICD_DEVICE_STATUS_BUSY                 0x2019
#define MUSB_SICD_DEVICE_TRANSACTION_CANCELLED       0x201F

#define MUSB_SICD_CANCEL_REQUEST_LENGTH    8
#define MUSB_SICD_CANCEL_REQUEST_DATA_LENGTH    6
#define MUSB_SICD_CANELLATION_CODE       0x4001

/**
 * MUSB_SICD_CONTAINER.
 * Still Image Class Driver Packet Container
 *
 * @field dwContainerLength Total data length in the container
 * @field wContainerType 0:Undefined, 1:Command, 2:Data, 3:Response, 4:Event
 * @field wOperationCode Operation code defined in PIMA 15740
 * @field dwTransactionId Transaction Id
 * @field abPayload payload buffer
 */
#include "mu_pkon.h"
typedef struct
{
    uint32_t dwContainerLength;
    uint16_t wContainerType;
    uint16_t wOperationCode;
    uint32_t dwTransactionId;
    uint8_t abPayload[20];
}MUSB_SICD_CONTAINER;
#include "mu_pkoff.h"

extern MUSB_SicdResult MUSB_SicdOperationAbort(MUSB_SicdProtocol* pSicdProtocol);
extern MUSB_SicdResult MUSB_SicdDeviceReset(MUSB_SicdProtocol* pSicdProtocol);
extern MUSB_SicdResult MUSB_SicdGetExtEventData(MUSB_SicdProtocol* pSicdProtocol);
#if MGC_SICD_OPEN_INTERRUPT_PIPE
extern MUSB_SicdResult MUSB_SicdStartInterruptPipeListening(MUSB_SicdProtocol* pSicdProtocol);
#endif
extern MUSB_SicdResult MUSB_SicdSilentBusReset(MUSB_SicdProtocol* pSicdProtocol);
extern MUSB_SicdResult MUSB_SicdGetNextData(MUSB_SicdProtocol* pSicdProtocol);


extern MUSB_SicdResult MUSB_SicdTransferCommandBlock(MUSB_SicdProtocol* pSicdProtocol);
extern MUSB_SicdResult MUSB_SicdTransferDataBlock(MUSB_SicdProtocol* pSicdProtocol);
extern MUSB_SicdResult MUSB_SicdTransferResponseBlock(MUSB_SicdProtocol* pSicdProtocol);

extern MUSB_SicdResult MUSB_SicdTransferGetDeviceStatus(MUSB_SicdProtocol* pSicdProtocol);
extern MUSB_SicdResult MUSB_SicdTransferCancelRequest(MUSB_SicdProtocol* pSicdProtocol);
extern MUSB_SicdResult MUSB_SicdTransferClearFeatureRequest(MUSB_SicdProtocol* pSicdProtocol);
extern MUSB_SicdResult MUSB_SicdTransferResetDeviceRequest(MUSB_SicdProtocol* pSicdProtocol);
extern MUSB_SicdResult MUSB_SicdTransferGetExtEventDataRequest(MUSB_SicdProtocol* pSicdProtocol);



#endif  //_MUSB_SICD_UTIL_H_
