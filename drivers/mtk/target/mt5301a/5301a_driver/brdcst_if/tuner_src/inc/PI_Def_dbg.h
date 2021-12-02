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
/***********************************************************************/
/*                                                                     */
/*  FILE DESCRIPTION                                                   */
/*                                                                     */
/*  $RCSfile: PI_Def_dbg.h,v $                                         */
/*                                                                     */
/***********************************************************************/

#ifndef _PI_DEF_DBG_H_INCLUDED_
#define _PI_DEF_DBG_H_INCLUDED_

/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/
#include "PD_Def_dbg.h"

/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
/***********************************************************************/
/*              For EEPROM Config                                      */
/***********************************************************************/
#define cMAX_CFG_SIZE           4
//#define cMAX_CFG_SIZE           sizeof(EEPNIMCFG_T)
#define cNIM_CFG_TAG            0x5A
#define EEPDTV_NIMCFG_COPY      3

enum
{
    cOFST_CFG_TAG,
    cOFST_CFG_TUNER_TYPE,
    cOFST_CFG_RESERVED0,
    cOFST_CFG_CHECKSUM
};

/***********************************************************************/
/*              For Monitor Thread                                     */
/***********************************************************************/
/* move to dtd_pd_glue_if.h */
#if 0
typedef enum
{
    FE_EVT_NONE = 0,            /* Don't do anything. */
    FE_EVT_SUSPEND,             /* Suspend thread. */
    FE_EVT_KILL                 /* The tuner thread shall be killed. */
} FE_EVT_T;

#define cMAX_NUM_MSGS           8           /* Max number of messages. */
#endif

/***********************************************************************/
/*              For Analog Tuner, Tuner Independant                    */
/***********************************************************************/
/***********************************************************************/
/*              Common Macro                                           */
/***********************************************************************/
/************************ Bit Process *************************/
#define mcCLR_BIT_C(a)          ((~Bit_##a) & 0xFFFF)
//#define mcCLR_BIT_C(a)          ((-1 - (Bit_#a)) & 0xFFFF)
//#define mcCLR_SET_MASK(a, b, c)  ((a) = ((a) & (~(b))) | (c))
//          \_ same as mcCLR_MASK + mcSET_MASK at optimized code

/***********************************************************************/
#define mcSET_MASKS(par)                mcMASKS(cw##par, cp##par)
#define mcSET_FIELD0(var, val, msk, pos)    mcSET_MASK(var, mcFIELD(val, msk, pos))
#define mcSET_FIELD(var, value, mask, pos)  \
{                                           \
    mcCLR_MASK(var, mask);                  \
    mcSET_FIELD0(var, value, mask, pos);    \
}
#define mcFIELD_CMB(val, par)           mcFIELD(val, cm##par, cp##par)
#define mcGET_FIELD_CMB(var, par)       mcGET_FIELD(var, cm##par, cp##par)
#define mcSET_FIELD_CMB(var, val, par)  mcSET_FIELD(var, val, cm##par, cp##par)

/***********************************************************************/
/*              RISC-IF                                                */
/***********************************************************************/
#define mcSET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue)   \
{                                                           \
/*    mcRISC_Reg(pu4RegBase, u4RegOfst) = u4RegValue;   */  \
    IO_WRITE32(pu4RegBase, u4RegOfst, u4RegValue);          \
    mcSHOW_DBG_MSG(("w 0x%08"cFMT_XL" 0x%08"cFMT_XL"\n", ((UINT32) (pu4RegBase)) + u4RegOfst, (UINT32) (u4RegValue)));  \
}

#define mcGET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue)   \
{                                                           \
    u4RegValue = IO_READ32(pu4RegBase, u4RegOfst);          \
    mcSHOW_DBG_MSG(("r 0x%08"cFMT_XL" 0x%08"cFMT_XL"\n", ((UINT32) (pu4RegBase)) + u4RegOfst, (UINT32) (u4RegValue)));  \
}

#define mcSET_SHOW_REG_BIT(pu4RegBase, u4RegOfst, u1BitPos)   \
{                                                             \
      mcGET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue);        \
      mcSET_BIT(u4RegValue, u1BitPos);                          \
      mcSET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue);        \
}

#define mcCLR_SHOW_REG_BIT(pu4RegBase, u4RegOfst, u1BitPos)   \
{                                                             \
      mcGET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue);        \
      mcCLR_BIT(u4RegValue, u1BitPos);                          \
      mcSET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue);        \
}

#endif  // _PI_DEF_DBG_H_INCLUDED_
