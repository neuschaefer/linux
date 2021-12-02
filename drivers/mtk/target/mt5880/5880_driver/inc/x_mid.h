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
 * $RCSfile: x_mid.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file x_mid.h
 *  x_mid.h Memory intrusion detection module
 */

#ifndef X_MID_H
#define X_MID_H

#include "x_typedef.h"
#include "x_hal_5381.h"

//=====================================================================
// Constant definitions
#define MID_MAX_REGIONS                 (5)

#define MID_ADDR_ALIGN_SHIFT            2
#define MID_ADDR_ALIGNMENT              (1 << MID_ADDR_ALIGN_SHIFT)
#define GROUP2_FIRST_AGENT_ID           16
#define GROUP3_FIRST_AGENT_ID           23

#define MID_BASE            0x400
#define REGION                0x10

#define UPADDR               0x0
#define LOWADDR            0x4
#define AGENT                   0x8

#define MID_ENABLE          (((UINT32)1) << 31)
#define MID_MODE             (((UINT32)1) << 30)

#define MID_WRITEABLE        (((UINT32)1) << 31)
#define MID_READPROTECT   (((UINT32)1) << 30)

#define MID_GROUP2_SHIFT  (16)
#define MID_GROUP3_SHIFT  (24)

#define MID_CLEAR_STATUS  0x70
#define MID_CLEAR          (1)

#define MID_INTRUD_INFO  0x80
#define MID_INTRUD_INFO_OFFSET  0x4
#define MID_INTRUD_DETECTED  (((UINT32)1) << 31)

#define MID_INTRUD_AGENT_INFO 0xA0
#define MID_INTRUD_AGENT_INFO_OFFSET  0x4

//=====================================================================
// Type definitions
typedef enum
{
    // Group 1
    MID_AGENT_AUDIO = 0,
    MID_AGENT_DEMUX = 1,
    MID_AGENT_VDOIN = 2,
    MID_AGENT_OSD = 3,
    MID_AGENT_PSCAN = 4,
    MID_AGENT_B2R = 5,
    MID_AGENT_CPU = 6,
    MID_AGENT_SCPOS = 7,
    MID_AGENT_VDECMC = 8,
    MID_AGENT_VLD = 9,
    MID_AGENT_3DGFX = 10,
    MID_AGENT_JPEG = 11,
    MID_AGENT_VENC = 12,
    MID_AGENT_MJCIN = 13,
    MID_AGENT_MJCOUT = 14,
    MID_AGENT_TEST0 = 15,

    // Group 2
    MID_AGENT_USB = GROUP2_FIRST_AGENT_ID,
    MID_AGENT_ETHERNET = 17,
    MID_AGENT_DDI = 18,
    MID_AGENT_PCIE = 19,
    MID_AGENT_DSP0 = 20,
    MID_AGENT_FCI = 21,
    MID_AGENT_TEST1 = 22,
    MID_AGENT_GDMA = 30,
    MID_AGENT_GDMA_BIT = 23,
    MID_AGENT_NONE = 31,        // None, for read-only region

    // Group 3
    MID_AGENT_GFXLO = GROUP3_FIRST_AGENT_ID,
    MID_AGENT_TCM = 24,
    MID_AGENT_ETHERNET_CHECKSUM = 25,
    MID_AGENT_RS232 = 26,
    MID_AGENT_TEST2 = 29,

    MID_AGENT_GFXLO_BIT = 24,
    MID_AGENT_TCM_BIT = 25,
    MID_AGENT_ETHERNET_CHECKSUM_BIT = 26,
    MID_AGENT_RS232_BIT = 27,
    MID_AGENT_TEST2_BIT = 30,

    // Special IDs
    MID_AGENT_MAX = 32,
    MID_AGENT_INVALID = 33
} MID_AGENT_ID_T;

#define MID_FLAG_INVERSE    (1U << 0)
#define MID_FLAG_CPUACCESS  (1U << 1)
#define MID_FLAG_WRITEABLE  (1U << 2)
 
typedef void (*PFN_MID_CALLBACK)(UINT32 u4Region, MID_AGENT_ID_T eAgentId,
    UINT32 u4Addr);

//=====================================================================
// Porting issue. Define.
#define MID_AGENT_MPEG1 MID_AGENT_VDECMC

//=====================================================================
// Function declarations

EXTERN BOOL MID_Init(void);

EXTERN BOOL MID_Exit(void);

EXTERN BOOL MID_Reset(void);

EXTERN VOID MID_ResetStatus(UINT32 u4Vector);

EXTERN VOID MID_QueryIntrusionStatus(void);

EXTERN VOID MID_SetDRAMChannel(BOOL fgChannelA);

EXTERN PFN_MID_CALLBACK MID_RegisterHandler(PFN_MID_CALLBACK pfnHandler);

EXTERN BOOL MID_SetRegion(UINT32 u4Region, MID_AGENT_ID_T eAllowedAgentId,
    UINT32 u4LowAddr, UINT32 u4HighAddr);

EXTERN BOOL MID_SetRegionEx(UINT32 u4Region, MID_AGENT_ID_T eAllowedAgentId,
    UINT32 u4LowAddr, UINT32 u4HighAddr, UINT32 u4Flags);

EXTERN BOOL MID_SetRegionEx2(UINT32 u4Region, MID_AGENT_ID_T eAllowedAgentId,
    UINT32 u4LowAddr, UINT32 u4HighAddr, UINT32 u4Flags, UINT32 u4AgentList);

EXTERN BOOL MID_ResetRegion(UINT32 u4Region);

EXTERN BOOL MID_AllocateFreeRegion(BOOL fgReadOnly, UINT32* pu4Region);

EXTERN BOOL MID_EnableRegionProtect(UINT32 u4Region);

EXTERN BOOL MID_DisableRegionProtect(UINT32 u4Region);

EXTERN void MID_ShowAgentList(void);

EXTERN const CHAR* MID_AgentIdToString(MID_AGENT_ID_T eAgentId);

EXTERN MID_AGENT_ID_T MID_StringToAgentId(const CHAR* szAgent);

/*
EXTERN UINT32 MID_CntBandWidthSet(UINT32 fgEnable, const CHAR *szAgentName,
                UINT32 u4Clk);

EXTERN UINT32 MID_CntBandWidthGet(UINT32 u4GrpId, CHAR *szAgentName,
                UINT32 *pu4AgentId, UINT32 *pu4Enable);
*/                
//---------------------------------------------------------------------

#endif  // X_MID_H

