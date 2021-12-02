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
 * MUSBStack-S Bulk-Only Transport (BOT) definitions.
 * $Revision: #1 $
 */

#ifndef __MUSB_MSD_BOT_H__
#define __MUSB_MSD_BOT_H__

#include "mu_tools.h"

/* BOT Protocol */
#define MGC_MSD_BOT_CBW_SIGNATURE      0x43425355
#define MGC_MSD_BOT_CSW_SIGNATURE      0x53425355
#define MGC_MSD_BOT_CBW_LENGTH          31
#define MGC_MSD_BOT_CSW_LENGTH          13
#define MGC_MSD_BOT_LUN_MASK            0xf0

/* Mass Storage Device BOT Command Execution State */
#define MGC_MSD_BOT_COMMAND_PASSED      0x00
#define MGC_MSD_BOT_COMMAND_FAILED      0x01
#define MGC_MSD_BOT_PHASE_ERROR         0x02

/* Mass Storage Device BOT States */
#define MGC_MSD_BOT_STATE_IDLE              0x00
#define MGC_MSD_BOT_STATE_DATA_IN           0x01
#define MGC_MSD_BOT_STATE_DATA_OUT          0x02
#define MGC_MSD_BOT_STATE_CSW               0x03
#define MGC_MSD_BOT_STATE_CBW               0x04
#define MGC_MSD_BOT_STATE_INIT              0x05

/* MSD BOT Functional Characteristics */
#define MGC_MSD_BOT_RESET               0xFF
#define MGC_MSD_BOT_GET_MAXLUN          0xFE

/* MSD BOT Host-Device Error Condtions */
#define MGC_MSD_BOT_CASEOK                       0
#define MGC_MSD_BOT_CASE_Hn_eq_Dn                1
#define MGC_MSD_BOT_CASE_Hn_lt_Di                2
#define MGC_MSD_BOT_CASE_Hn_lt_Do                3
#define MGC_MSD_BOT_CASE_Hi_gt_Dn                4
#define MGC_MSD_BOT_CASE_Hi_gt_Di                5
#define MGC_MSD_BOT_CASE_Hi_eq_Di                6
#define MGC_MSD_BOT_CASE_Hi_lt_Di                7
#define MGC_MSD_BOT_CASE_Hi_ne_Do                8
#define MGC_MSD_BOT_CASE_Ho_gt_Dn                9
#define MGC_MSD_BOT_CASE_Ho_ne_Di               10 
#define MGC_MSD_BOT_CASE_Ho_gt_Do               11
#define MGC_MSD_BOT_CASE_Ho_eq_Do               12
#define MGC_MSD_BOT_CASE_Ho_lt_Do               13
#define MGC_MSD_BOT_CASECBW                     14
#define MGC_MSD_BOT_CASE_ERROR                  15

/* Transfer Buffer Length for the Client : This value should not be 
 * greater than (64*1024-1) i.e. max value should fit in 16 Bits
 */
#define MGC_MSD_BOT_XFER_BUF_LEN                (16 * 1024)

/* Data Transfer Length for different commands */
#define MGC_MSD_BOT_FMT_CACP_LEN        12 /* Format Capacity */
#define MGC_MSD_BOT_MODE_SEN_LEN        4  /* Mode Sense */ 
#define MGC_MSD_BOT_INQ_DATA_LEN        36 /* Inquiry */
#define MGC_MSD_BOT_REQ_SENS_LEN        18 /* Request Sense */
#define MGC_MSD_BOT_RD_CAPCY_LEN        8  /* Read Capacity */

/* Total Number of Commands Supported */
#define MGC_MSD_BOT_SUPPORTED_COMMANDS 15

#include "mu_pkon.h"

/**
 * MGC_MsdCbw.
 * BOT Command Block Wrapper.
 * @field dCbwSignature must be MGC_MSD_BOT_CBW_SIGNATURE
 * @field dCbwTag tag created by host, echoed in CSW
 * @field dCbwDataTransferLength data transfer length (0 is valid)
 * @field bmCbwFlags bit 7 is USB direction
 * @field bCbwLun Logical Unit Number
 * @field bCbwCbLength length of embedded command block
 * @field aCbwCb embedded command block
 */
typedef struct
{
    uint32_t dCbwSignature;
    uint32_t dCbwTag;
    uint32_t dCbwDataTransferLength;
    uint8_t bmCbwFlags;
    uint8_t bCbwLun;
    uint8_t bCbwCbLength;
    uint8_t aCbwCb[16];
} MGC_MsdCbw;
#include "mu_pkoff.h"

/* Flags: */
#define MGC_MSD_BOT_DIR_OUT 0
#define MGC_MSD_BOT_DIR_IN  0x80

#include "mu_pkon.h"

/**
 * MGC_MsdCsw.
 * BOT Command Status Wrapper.
 * @field dCswSignature must be MGC_MSD_BOT_CSW_SIGNATURE
 * @field dCswTag echo of that from CBW
 * @field dCswDataResidue transfer length minus amount processed/sent
 * @field bCswStatus status code
 */
typedef struct
{
    uint32_t dCswSignature;
    uint32_t dCswTag;
    uint32_t dCswDataResidue;
    uint8_t bCswStatus;
} MGC_MsdCsw;
#include "mu_pkoff.h"

#endif	/* multiple inclusion protection */
