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
 * $RCSfile: pd_type.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pd_type.h
 *  common type definition
 */

#ifndef PD_TYPE_H
#define PD_TYPE_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#if 0  //LH , no-one use this file
typedef UINT16          WORD;
typedef UINT32          DWORD;

typedef UINT8           Data8;
//typedef UINT16          Data16;
typedef INT16           Data16;
typedef UINT32          Data32;
typedef UINT32          Data64;
typedef BOOL            Bool;

typedef UINT8           U8;
typedef UINT16          U16;
typedef UINT32          U32;
//typedef unsigned int    UINT;
typedef INT8            S8;
typedef INT16           S16;
typedef INT32           S32;


/************************ Constant *************************/
#ifndef EXTERN
#define EXTERN          extern
#endif
#ifndef STATIC
#define STATIC          static
#endif
#ifndef VOID
#define VOID            void
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
// 
#define mcPRINTF                Printf


//-----------------------------------------------------------------------------
// Bit Process
//-----------------------------------------------------------------------------
#define mcBITL(b)               (1UL << (b))
//#define mcBIT(b)                (1 << (b))
#define mcBIT(b)                mcBITL(b)
#define mcMASK(w)               (mcBIT(w) - 1)
#define mcMASKS(w, b)           (mcMASK(w) << (b))
#define mcFIELD(val, msk, pos)  (((val) << (pos)) & (msk))
#define mcGET_FIELD(var, mask, pos)     (((var) & (mask)) >> (pos))

#define mcSET_MASK(a, b)        ((a) |= (b))
#define mcCLR_MASK(a, b)        ((a) &= (~(b)))
#define mcCHK_MASK(a, b)        ((a) & (b))
#define mcSET_BIT(a, b)         mcSET_MASK(a, mcBIT(b))
#define mcCLR_BIT(a, b)         mcCLR_MASK(a, mcBIT(b))
#define mcCHK_BIT1(a, b)        ((a) & mcBIT(b))
#define mcCHK_BITM(a, b, m)     (((a) >> (b)) & (m))
#define mcCHK_BITS(a, b, w)     mcCHK_BITM(a, b, mcMASK(w))
#define mcTEST_BIT(a, b)        mcCHK_BIT1(a, b)

#define mcFIELD(val, msk, pos)  (((val) << (pos)) & (msk))
#define mcSET_FIELD0(var, val, msk, pos)    mcSET_MASK(var, mcFIELD(val, msk, pos))

#define mcSET_FIELD(var, value, mask, pos)  \
{                                           \
    mcCLR_MASK(var, mask);                  \
    mcSET_FIELD0(var, value, mask, pos);    \
}
/* Ex. Set var[15:8] = 8. => msSET_FIELD(var, 0x08, MASk) */
#define mcSET_FIELD_CMB(var, val, par)  mcSET_FIELD(var, val, cm##par, cp##par)


#ifndef BIT0
#define BIT0                    mcBIT(0)
#define BIT1                    mcBIT(1)
#define BIT2                    mcBIT(2)
#define BIT3                    mcBIT(3)
#define BIT4                    mcBIT(4)
#define BIT5                    mcBIT(5)
#define BIT6                    mcBIT(6)
#define BIT7                    mcBIT(7)
#endif

#define mcLOW_MASK(VarW)        ((UINT8) ((VarW) & mcMASK(8)))
#define mcHIGH_MASK(VarW)       ((UINT8) ((VarW) & ~mcMASK(8)))
#define mcLOW2HIGH(VarW)        (((UINT16) (VarW)) << 8)
/* (1234, 5678) -> 7834 */
#define mc2BYTE(ch1L, ch2H)     (mcLOW_MASK(ch1L) | mcLOW2HIGH(ch2H))
/* (1234) -> 0034 */
#define mcLOW_BYTE(VarW)        mcLOW_MASK(VarW)
/* (1234) -> 0012 */
#define mcHIGH_BYTE(VarW)       ((UINT8) (((UINT16) (VarW)) >> 8))


//-----------------------------------------------------------------------------
// debug definitions
//-----------------------------------------------------------------------------

EXTERN  UINT8       u1DbgLevel;
#ifndef cMULTI_MSG_MSK
#define cMULTI_MSG_MSK          4 //remove this?
#endif
#ifndef cMAX_STR_LEN
#define cMAX_STR_LEN            40
#endif

#define mcCHECK_DBG_LVL(Lvl)            (mcCHK_MASK(u1DbgLevel, mcMASK(cMULTI_MSG_MSK)) >= Lvl)
#define mcCHECK_HW_MSG                  mcCHECK_DBG_LVL(1)
#define mcCHECK_DBG_MSG1                mcCHECK_DBG_LVL(1)
#define mcCHECK_DBG_MSG                 mcCHECK_DBG_LVL(2)
#define mcCHECK_DBG_MSG2                mcCHECK_DBG_LVL(3)
#define mcCHECK_DBG_MSG3                mcCHECK_DBG_LVL(4)
//#define mcCHECK_USER_MSG                mcCHECK_DBG_LVL(0)

#define mcDBG_LVL_MSG(Lvl, _x_ )        \
{                                       \
    if (mcCHECK_DBG_LVL(Lvl))           \
        mcPRINTF _x_;                   \
}

#define mcSHOW_POP_MSG                  mcSHOW_HW_MSG
#define mcSHOW_HW_MSG(_x_)              mcDBG_LVL_MSG(1, _x_)
#define mcSHOW_DBG_MSG1(_x_)            mcDBG_LVL_MSG(1, _x_)
#define mcSHOW_DBG_MSG(_x_)             mcDBG_LVL_MSG(2, _x_)
#define mcSHOW_DBG_MSG2(_x_)            mcDBG_LVL_MSG(3, _x_)
#define mcSHOW_DBG_MSG3(_x_)            mcDBG_LVL_MSG(4, _x_)
//#define mcSHOW_USER_MSG(_x_)            mcDBG_LVL_MSG(1, _x_)
#define mcSHOW_USER_MSG(_x_)            mcPRINTF _x_

#define mcSHOW_DRVERR_MSG(_x_)          mcDBG_LVL_MSG(1, _x_)
#define mcSHOW_OSAI_MSG(_x_)            mcDBG_LVL_MSG(2, _x_)
#define mcSHOW_OSAIOK_MSG(_x_)          mcDBG_LVL_MSG(4, _x_)
#define mcSHOW_DRVAPI_MSG(_x_)          mcDBG_LVL_MSG(2, _x_)
#define mcSHOW_DRVAPIOK_MSG(_x_)        mcDBG_LVL_MSG(4, _x_)
#define mcSHOW_DRVLOCK_MSG(_x_)         mcDBG_LVL_MSG(4, _x_)



#endif 
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#endif /* PD_TYPE_H */

