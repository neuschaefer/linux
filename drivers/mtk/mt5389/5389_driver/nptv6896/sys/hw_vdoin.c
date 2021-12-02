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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: hw_vdoin.c $ 
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 *   VDOIN module register access function
 *   
 * Author:
 * -------
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/05/17 6:55p $  
 *
 * $Revision: #1 $
 ****************************************************************************/

#include "general.h"
#include "hw_vdoin.h"
#include "x_assert.h"

#ifdef CC_MTK_LOADER
#undef x_crit_start
#undef x_crit_end
#define x_crit_start() 0
#define x_crit_end(x) UNUSED(x)
#define CRIT_STATE_T UINT8
#else
#include "x_os.h"
#include "sif_if.h"
#endif


/* Reg READ/WRITE function, for 82xx & 53xx
 *    provide for VDOON(16bit,2002xxxx)register
 *    82xx only has 16 bit address space.
 */
#if REG_ALL_FUNC
UINT8 u1RegRead1B(UINT16 reg16)
{
	return (*(volatile UINT8 *)(VDOIN_BASE+reg16));
}
UINT32 u4RegRead4B(UINT16 reg16)
{
	ASSERT((reg16&3)==0);
	return (*(volatile UINT32 *)(VDOIN_BASE+reg16));
}
void vRegWrite4B(UINT16 reg16, UINT32 val32)
{
	ASSERT((reg16&3)==0);
	(*(volatile UINT32 *)(VDOIN_BASE+reg16)=(val32));
}
void vRegWrite4B_S(UINT16 reg16, UINT8 bByte3, UINT8 bByte2, UINT8 bByte1, UINT8 bByte0)
{
	ASSERT((reg16&3)==0);
	(*(volatile UINT32 *)(VDOIN_BASE+reg16)=((bByte3<<24)|(bByte2<<16)|(bByte1<<8)|(bByte0)));
}
#endif

#if REG_2LAYER_IO32
UINT16 u2RegRead2B(UINT16 reg16)
{
	return u2IO32Read2B(VDOIN_BASE+reg16);
}
void vRegWrite1B(UINT16 reg16,/*UINT8*/UINT32 val8)
{
	vIO32Write1B(VDOIN_BASE+reg16,val8);
}
void vRegWrite1BMsk(UINT16 reg16,/*UINT8*/UINT32 val8,UINT8 msk8)
{
	vIO32Write1BMsk(VDOIN_BASE+reg16,val8,msk8);
}
void vRegWrite2B(UINT16 reg16, /*UINT16*/UINT32 val16)
{
	vIO32Write2B(VDOIN_BASE+reg16,val16);
}
void vRegWrite2BMsk(UINT16 reg16, /*UINT16*/UINT32 val16, UINT16 msk16)
{
	vIO32Write2BMsk(VDOIN_BASE+reg16,val16,msk16);
}

void vRegWrite4BMsk(UINT16 reg16, UINT32 val32, UINT32 msk32)
{
	vIO32Write4BMsk(VDOIN_BASE+reg16,val32,msk32);
}
#endif


#ifndef CC_MTK_LOADER
// ======================================================================
/* MT8297 READ/WRITE function, only for 539x later */

#ifdef CC_MT5391
UINT8 u1MT8297DirRead1B(UINT8 reg8)
{
	INT16 ret;
	VERIFY((ret=SIF_MT8297_DirectRead8(reg8))>=0);
	return (UINT8)(ret&0xff);
}

UINT8 u1MT8297InDRead1B(UINT8 reg8)
{
	INT16 ret;
	VERIFY((ret=SIF_MT8297_InDirectRead8(reg8))>=0);
	return (UINT8)(ret&0xff);
}

void vMT8297DirWrite1B(UINT8 reg8, UINT8 val8)
{
	VERIFY(SIF_MT8297_DirectWrite8(reg8, val8)==0);
}

void vMT8297InDWrite1B(UINT8 reg8,UINT8 val8)
{
	VERIFY(SIF_MT8297_InDirectWrite8(reg8, val8)==0);
}

void vMT8297DirWrite1BMsk(UINT8 reg8,UINT8 val8,UINT8 msk8)
{
	VERIFY(SIF_MT8297_DirectWrite8Msk(reg8, val8,msk8)==0);
}

void vMT8297InDWrite1BMsk(UINT8 reg8,UINT8 val8,UINT8 msk8)
{
	VERIFY(SIF_MT8297_InDirectWrite8Msk(reg8, val8,msk8)==0);
}
#endif
#endif
