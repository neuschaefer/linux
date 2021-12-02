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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: smc_if.h,v $
 * $Revision: #1 $
 *
 * Description: 
 *         This header file contains Interfaces of SmartCard.
 *---------------------------------------------------------------------------*/

/** @file smc_if.h
 *  smc_if.h is the header of SmartCard Interface.
 */


#ifndef SMC_IF_H
#define SMC_IF_H

#include "x_typedef.h"

typedef struct _SMC_CMD_SEND_T {
    UINT8 block_type;      /* telling the type of the block( info,receive,supervisory ) */
    UINT8 ins_code;        /* instruction code */
    UINT8 ns;              /* send sequence number */
} SMC_CMD_SEND_T;

typedef  struct _SMC_CMD_RESPONSE_T {
    UINT8          protocol_number;                /* Version of IC Card */
    UINT8          unit_length;                    /* Length of payload */
    UINT16         card_instruction;               /* Instruction from Smart Card */
    UINT16         return_code;                    /* Process result of Smart card */
} SMC_CMD_RESPONSE_T;

typedef enum {
    SMC_CARD_PRESENT,                  /* Card Present                       */
    SMC_CARD_ABSENT,                   /* Card Absent                        */
    SMC_CARD_INIT_FAIL,                  /* Card Invalid                       */
    SMC_CARD_ACTIVATED,                  /* Card Present                       */    
    SMC_CARD_DEACTIVATED,                  /* Card Present                       */        
    SMC_CARD_ERROR                     /* Card invalid                         */
} SMC_CARD_STATUS_T;

typedef struct {
    UINT16 ecm_pid;
    UINT8  odd_key[8];
    UINT8  even_key[8];
    UINT8  smc_slot_nbr;
    UINT32 session_id;
    UINT8  card_id[6];
} SMC_BCAS_SCRAMBLE_KEYS_T;

typedef struct {
    UINT8 maker_identification;            /* Maker identification               */
    UINT8 version;                         /* Version                            */
    UINT8 id_identification;               /* Identification                     */
    UINT8 group_id[6];          /* Group id                           */
    UINT16 check_code;                     /* Check code                         */
} SMC_BCAS_GROUP_ID_T;

typedef struct {
    UINT8 maker_identification;            /* Maker identification               */
    UINT8 version;                         /* Version                            */
    UINT8 id_identification;               /* Identification                     */
    UINT8 ind_card_id[6];    /* Individual Card id                 */
    UINT16 check_code;                     /* Check code                         */
} SMC_BCAS_IND_CARD_ID_T;

typedef enum {
    GENERAL_CARD,                       /* General Card                       */
    PREPAID_CARD                        /* Prepaid Card                       */
} SMC_BCAS_CARD_TYPE_T;

typedef struct {
    UINT8 smc_slot_nbr;                    /* Smartcard slot number              */
    UINT8 card_id[6];         /* Card id of smartcard               */
    UINT16 ca_system_id;                   /* CA System ID                       */
    UINT8 no_of_grp_id;                    /* Number of Group id                 */
    UINT8 system_key[32];         /* Card id of smartcard               */    
    UINT8 cbc_initial_vector[8];         /* Card id of smartcard               */        
    SMC_BCAS_IND_CARD_ID_T ind_id;          /* Individual Card ID                 */
    SMC_BCAS_GROUP_ID_T grp_id[6]; /* Group ID                  */
    SMC_BCAS_CARD_TYPE_T card_type;
    SMC_CARD_STATUS_T card_status;    
} SMC_BCAS_CARD_INFO_T;

#define SMCR_OK             ((INT32)  0)
#define SMCR_INV_ARG        ((INT32) -1)
#define SMCR_FAIL           ((INT32) -2)
#define SMCR_CARD_WRITE_ERROR     ((INT32) -3)
#define SMCR_CARD_READ_ERROR     ((INT32) -4)
#define SMCR_LRC_ERROR     ((INT32) -5)
#define SMCR_CARD_ERROR     ((INT32) -6)
#define SMCR_NO_CARD        ((INT32) -7)

typedef void (* PFN_SMC_HOTPLUG_CALLBACK)(UINT8 u1HotPlugStatus);

EXTERN INT32 SMC_Init (void);

EXTERN INT32 SMC_Reset (void);

EXTERN INT32 SMC_Activate (void);

EXTERN INT32 SMC_Deactivate (void);
	
EXTERN void SMC_SetHotPlugCb (PFN_SMC_HOTPLUG_CALLBACK pfnCb);

EXTERN INT32 SMC_SendReceiveAPDU (UINT8 u1SlotNbr, 
    UINT8 *pu1SendData, UINT8 u1SendLength, UINT8 *pu1ResponseData, UINT8 *pu1ResponseLength);

EXTERN INT32 SMC_RequestIFS(UINT8 u1Len);

EXTERN INT32 SMC_BCAS_GetCardId(UINT8* pu1CardId);

EXTERN INT32 SMC_BCAS_InitialSettings(SMC_BCAS_CARD_INFO_T* prCardInfo);

EXTERN INT32 SMC_BCAS_SendECM (UINT8	smc_slot_nbr, UINT8 *ecm_data, UINT16 length,
                                                        SMC_BCAS_SCRAMBLE_KEYS_T *keys);

#endif //SMC_IF_H

