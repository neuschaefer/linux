/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dmx.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file tz.h
 *  Demux driver - common definitions
 */


#ifndef TZ_H
#define TZ_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "tz_if.h"
#include "x_hal_5381.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define TZ_SUCCESS                  ((UINT32)0x00)
#define TZ_PENDING                  ((UINT32)0x01)
#define TZ_ERROR_ACCESS_DENIED      ((UINT32)0x02)
#define TZ_ERROR_BUSY               ((UINT32)0x03)
#define TZ_ERROR_CANCEL             ((UINT32)0x04)
#define TZ_ERROR_COMMUNICATION      ((UINT32)0x05)
#define TZ_ERROR_DECODE_NO_DATA     ((UINT32)0x06)
#define TZ_ERROR_DECODE_TYPE        ((UINT32)0x07)
#define TZ_ERROR_ENCODE_FORMAT      ((UINT32)0x08)
#define TZ_ERROR_ENCODE_MEMORY      ((UINT32)0x09)
#define TZ_ERROR_GENERIC            ((UINT32)0x0a)
#define TZ_ERROR_ILLEGAL_ARGUMENT   ((UINT32)0x0b)
#define TZ_ERROR_ILLEGAL_STATE      ((UINT32)0x0c)
#define TZ_ERROR_MEMORY             ((UINT32)0x0d)
#define TZ_ERROR_NOT_IMPLEMENTED    ((UINT32)0x0e)
#define TZ_ERROR_SECURITY           ((UINT32)0x0f)
#define TZ_ERROR_SERVICE            ((UINT32)0x10)
#define TZ_ERROR_SHORT_BUFFER       ((UINT32)0x11)
#define TZ_ERROR_UNDEFINED          ((UINT32)0x12)
#define TZ_ERROR_ITEM_NOT_FOUND     ((UINT32)0x13)

#define TZCTL_SYSTEM_ECHO           ((UINT32)0x0)
#define TZCTL_SYSTEM_ENABLE_DEBUG   ((UINT32)0x1)
#define TZCTL_SYSTEM_REVERSE_ARRAY  ((UINT32)0x2)

#define TZCTL_MARLIN_TEST           ((UINT32)0x1000)

#define TZCTL_HDCP_TEST             ((UINT32)0x2000)



/* Define macros which are used to interpret the SMC ID field.
 * The SMC ID format is as follows:
 *  Bits 31-28:     SMC Type.
 *  Bits 27-16:     Service ID.
 *  Bits 15- 0:     Function ID.
 */
#define SMC_TP_MASK                 ((UINT32)0xF0000000)
#define SMC_TP_SHIFT                28

#define SVC_ID_MASK                 ((UINT32)0x0FFF0000)
#define SVC_ID_SHIFT                16

#define FUN_ID_MASK                 ((UINT32)0x0000FFFF)
#define FUN_ID_SHIFT                0

/* Define the SMC Type field. */
#define SMC_TP_SYSTEM               ((UINT32)0x8)
#define SMC_TP_TZAPI                ((UINT32)0xE)

/* Macros which extract fields from an SMC ID. */
#define GET_SMC_TP(smc_id) \
    (((UINT32)(smc_id) & SMC_TP_MASK) >> SMC_TP_SHIFT)

#define GET_SVC_ID(smc_id) \
    (((UINT32)(smc_id) & SVC_ID_MASK) >> SVC_ID_SHIFT)

#define GET_FUN_ID(smc_id) \
    (((UINT32)(smc_id) & FUN_ID_MASK) >> FUN_ID_SHIFT)

/* A simple macro for constructing an SMC ID. */
#define SMC_ID(typ,svc,fun)                     \
    (UINT32)(                                 \
        ((typ << SMC_TP_SHIFT) & SMC_TP_MASK) | \
        ((svc << SVC_ID_SHIFT) & SVC_ID_MASK) | \
        ((fun << FUN_ID_SHIFT) & FUN_ID_MASK)   \
    )
    
/* The service's Service ID. */
#define SVC_MTK_ID                      ((UINT32)0x235)

#define SMC_MTK_INIT_ID                 SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x0)
#define SMC_MTK_INVALIDATEICACHE_ID     SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x1) 
#define SMC_MTK_INVALIDATEDCACHE_ID     SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x2) 
#define SMC_MTK_MEMORY_ACCESS_EX        SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x3) 
#define SMC_MTK_ENABLE_L2               SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x4)
#define SMC_MTK_FLUSH_L2                SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x5) 

#define SMC_MTK_DMX_INIT                SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x100)
#define SMC_MTK_DMX_SETDESIV            SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x101)
#define SMC_MTK_DMX_SETDESIVEX          SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x102)
#define SMC_MTK_DMX_SETDESKEY           SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x103)
#define SMC_MTK_DMX_SETAESIVEX          SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x104)
#define SMC_MTK_DMX_SETAESIV            SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x105)
#define SMC_MTK_DMX_SETAESKEYLEN        SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x106)
#define SMC_MTK_DMX_SETAESKEY           SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x107)
#define SMC_MTK_DMX_SETDMEMAESIV        SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x108)
#define SMC_MTK_DMX_SETMULTI2KEY        SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x109)
#define SMC_MTK_DMX_SETMULTI2CBCIV      SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x110)
#define SMC_MTK_DMX_SetMULTI2OFBIV      SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x111)
#define SMC_MTK_DMX_SetMULTI2SYSKEY     SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x112)
#define SMC_MTK_DMX_SetMULTI2CONFIG     SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x113)
#define SMC_MTK_DMX_CPUDESC_KEYIV       SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x114)
#define SMC_MTK_DMX_CPUDESC_INTRCLEAR   SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x115)

#define SMC_MTK_DRM_WV_SET_ASSET_KEY    SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x200)
#define SMC_MTK_DRM_WV_DERIVE_CW        SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x201)
#define SMC_MTK_DRM_WV_PASS_KEYBOX      SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x202)
#define SMC_MTK_DRM_GETROMBYTES         SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x203)

#define SMC_MTK_TZCTL                   SMC_ID(SMC_TP_SYSTEM, SVC_MTK_ID, 0x300)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct
{
    UINT32 u4SMCID;
    UINT32 u4R1;
    UINT32 u4R2;
    UINT32 u4R3;
    UINT32 u4R4;
    UINT32 u4R5;
    UINT32 u4R6;
    UINT32 u4R7;
} TZ_SMC_ARG_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------




#endif  // TZ_H

