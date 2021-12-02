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

// === INCLUDE =============================================================================
#include "x_os.h"	
#include "x_rand.h"
#include "x_assert.h"
#include "sv_const.h"
#include "hw_vdoin.h"
#include "nptv_debug.h"
#include "drv_pq_cust.h"
#include "drv_upscaler.h"
#include "drv_lcdim.h"
#include "drv_od.h"

// === HW INCLUDE =============================================================================
#include "hw_ycproc.h"
#include "hw_sw.h"
#include "hw_ospe.h"
#include "hw_pdwnc.h"
// === DEFINE =============================================================================
// === FUNCTION PROTOTYPE =================================================================
// === EXTERN FUNCTION ====================================================================
// === GLOBAL VARIABLE ====================================================================
BOOL _fgGammaStressTest = SV_FALSE;

#ifndef NDEBUG    // PQ SRAM Stress Test Only Take Effect in Debug Build

UINT32 u4ScalerCoefMask[1161] = 
{
// HORI_Y
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
// HORI_C
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
0x00FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
//VER
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF,
0x00000000, 0x03FFFFFF, 0xFFFFFFFF
};

UINT8 OD_Table_33[33*33];
UINT8 OD_Table_41[41*41];
UINT8 FB_Table_17[17*17];

// === EXTERN VARIABLE ====================================================================
extern UINT32 u4FilterCoeff[1161];

//extern UINT8 TDS_LC_H[65];
//extern UINT8 TDS_YLEV[65];

extern UINT8 GAMMA_256[3][257];
extern UINT16 u2GammaDecodedTable[3][257];
extern UINT8 DEFAULT_SCE_TBL[960];
extern UINT8 OD_Table[33*33];
extern UINT8 FB_Table[17*17];
// === STATIC VARIABLE ====================================================================
// === END OF STATIC VARIABLE =============================================================
// === BODY ===============================================================================


/*****************************************************************************************/
/************************************** TABLE-GEN ****************************************/
/*****************************************************************************************/
extern int random(int mod);

static void vArryGetComplementData(UINT8 *u1arry, UINT32 u4length)
{
    UINT32 i;
    UINT8 pat, pat1, pat2;
    static UINT8 SeedArry[11] = { 0xAA,                      // 1-bit complement
                                  0xCC, 0x99,                // 2-bit complement
                                  0xF0, 0xE1, 0xD2, 0xC3, 0xB4, 0xA5, 0x96, 0x87,    // 4-bit complement
                           };                                // 8-bit complement reserved in pat = ~pat.
    static UINT8 SeedIndex;

    SeedIndex = (SeedIndex+17)%11;      //Randomize SeedIndex
    pat1 = SeedArry[SeedIndex];
    pat2 = SeedArry[(SeedIndex+1)%11];

    pat = pat1;
    for(i=0;i<u4length;i++)
    {
        *(u1arry+i) = pat;
        
        switch(i%4)                 //   0x0000 = (~pat2, pat2, ~pat1, pat1)
        {                           //   0x0004 = (pat2, ~pat2, pat1, ~pat1)
            case 0:
                pat = ~pat1;
                break;
            case 1:
                pat = pat2;
                break;
            case 2:
                pat = ~pat2;
                break;
            case 3:
                pat1 = ~pat1;
                pat2 = ~pat2;
                pat = pat1;
                break;
            default:
                break;
        }
    }
}


static void vArryGetSequentialData(UINT8 *u1arry, UINT32 u4length)
{
    UINT32 i;
   
    for(i=0;i<u4length;i++)
    {
        *(u1arry+i) = (UINT8)(i);
    }
}

static void vArryGetRandomData(UINT8 *u1arry, UINT32 u4length)
{
    UINT32 i;
   
    for(i=0;i<u4length;i++)
    {
        *(u1arry+i) = random(256);
    }
}

void vDrvGenSRAMTestTable(UINT8 *u1SRAMTable, UINT32 u4TableSize, UINT8 u1TablePattern)
{
    switch(u1TablePattern)
    {
        case 0:         //Complement data
            vArryGetComplementData(u1SRAMTable, u4TableSize);
            break;
        case 1:         //Sequential data
            vArryGetSequentialData(u1SRAMTable, u4TableSize);
            break;
        default:         //Random data and else
            vArryGetRandomData(u1SRAMTable, u4TableSize);
            break;
    }
}
 
BOOL bGammaStressTest(UINT8 u1TablePattern)
{
    BOOL bRet = SV_TRUE;
    UINT32 i;

    // 1. write test data into default table
    vDrvGenSRAMTestTable((UINT8*)u2GammaDecodedTable, 1542, u1TablePattern);    // 2 x 257 x 3

    for(i = 0; i < 257; i++)
    {
        u2GammaDecodedTable[0][i] = u2GammaDecodedTable[0][i] & 0xFFF;
        u2GammaDecodedTable[1][i] = u2GammaDecodedTable[1][i] & 0xFFF;
        u2GammaDecodedTable[2][i] = u2GammaDecodedTable[2][i] & 0xFFF;
    }

    // 2. load data from table into sram
    vDrvGammaWriteTable();

    // 3. read data back and compare
    if (bDrvGammaVerifyTable() == SV_FALSE)
    {
        bRet = SV_FALSE;
    }
    vDrvGammaKickPingPong();
    HAL_Delay_us(30000);

    // 4. return result
    return bRet;
}

BOOL bSCEStressTest(UINT8 u1TablePattern)
{
    BOOL bRet = SV_TRUE;

    // 1. write test data into default table
    vDrvGenSRAMTestTable((UINT8*)DEFAULT_SCE_TBL, 960, u1TablePattern);

     // 2. load data from table into sram
    vDrvSCELoadTable();

    // 3. read data back and compare
    if(bDrvSCEVerifyTable() == SV_FALSE)
    {
        bRet = SV_FALSE;
    }

    // 4. return result
    return bRet;    
}
/*
BOOL bYLEVSharpnessStressTest(UINT8 u1TablePattern)
{
    BOOL bRet = SV_TRUE;

    // 1. write test data into default table
    vDrvGenSRAMTestTable(TDS_YLEV, 65, u1TablePattern);

    // 2. load data from table into sram
    vDrvLoadYLevTable();

    // 3. read data back and compare
    if(bDrvReadYLevTable() == SV_FALSE)
    {
        bRet = SV_FALSE;
    }

    // 4. return result
    return bRet;
}

BOOL bLCSharpnessStressTest(UINT8 u1TablePattern)
{
    BOOL bRet = SV_TRUE;

    // 1. write test data into default table
    vDrvGenSRAMTestTable(TDS_LC_H, 65, u1TablePattern);

    // 2. load data from table into sram
    vDrvLoadLocalTable();

    // 3. read data back and compare
    bDrvReadLocalTable();
    
    if(bDrvReadLocalTable() == SV_FALSE)
    {
        bRet = SV_FALSE;
    }

    // 4. return result
    return bRet;    
}
*/
BOOL bScalerStressTest(UINT8 u1TablePattern)
{
    BOOL bRet = SV_TRUE;
    UINT32 i;
	
    // 1. write test data into default table
    vDrvGenSRAMTestTable((UINT8*)u4FilterCoeff, 4644, u1TablePattern);

    for(i = 0; i < 1161; i++)
    {
        u4FilterCoeff[i] &= u4ScalerCoefMask[i];
    }

    // 2. load data from table into sram
    vDrvSetUpScalerFilter(SV_VP_MAIN, UPSCALER_HORI, 0);
    vDrvSetUpScalerFilter(SV_VP_MAIN, UPSCALER_VERT, 0);    
    // 3. read data back and compare
    
    if (vDrvCheckUpScalerFilter(SV_VP_MAIN, UPSCALER_HORI, 0) == SV_FALSE)
    {
        bRet = SV_FALSE;
    }
    else if (vDrvCheckUpScalerFilter(SV_VP_MAIN, UPSCALER_VERT, 0) == SV_FALSE)
    {
        bRet = SV_FALSE;
    }
    else
    {
    	bRet = SV_TRUE;
    }
    // 4. return result
    return bRet;     
}
#ifdef CC_MT5396 
BOOL bPostScalerStressTest(UINT8 u1TablePattern)
{
    BOOL bRet = SV_TRUE;
    UINT32 i;
	
    // 1. write test data into default table
    vDrvGenSRAMTestTable((UINT8*)u4FilterCoeff, 4644, u1TablePattern);

    for(i = 0; i < 1161; i++)
    {
        u4FilterCoeff[i] &= u4ScalerCoefMask[i];
    }

    // 2. load data from table into sram
    vDrvSetPostUpScalerFilter(SV_VP_MAIN, UPSCALER_HORI, 0);
    vDrvSetPostUpScalerFilter(SV_VP_MAIN, UPSCALER_VERT, 0);    
    
    // 3. read data back and compare    
    if (vDrvCheckPostUpScalerFilter(SV_VP_MAIN, UPSCALER_HORI, 0) == SV_FALSE)
    {
        bRet = SV_FALSE;
    }
    else
    {
    	bRet = SV_TRUE;
    }
    // 4. return result
    return bRet;     
}
#endif// CC_MT5396 

BOOL bLcDimStressTest(UINT8 u1TablePattern)
{
    BOOL bRet = SV_TRUE;
    UINT8 bLcDimMappingCurve[256];

    // 1. write test data into default table
    vDrvGenSRAMTestTable(bLcDimMappingCurve, 256, u1TablePattern);

    // 2. load data from table into sram
    vDrvSetLcDimMappingCurve(bLcDimMappingCurve);

    // 3. read data back and compare
    if(vDrvVerifyLcDimMappingCurve(bLcDimMappingCurve) == SV_FALSE)
    {
        bRet = SV_FALSE;
    }
    vDrvLcDimKickPingPong();
    HAL_Delay_us(30000);

    // 4. return result
    return bRet;    
}

BOOL bODStressTest(UINT8 u1TablePattern)
{
    BOOL bRet = SV_TRUE;

    // 1. write test data into default table
    vDrvGenSRAMTestTable(OD_Table_33, 1089, u1TablePattern);

    // 2. load data from table into sram
    vDrvODWriteTable(OD_TABLE_33, OD_ALL, OD_Table_33);

    // 3. read data back and compare
    if(u1DrvODReadTable(OD_TABLE_33, OD_RED, OD_Table_33) == 1)
    {
        bRet = SV_FALSE;
    }

    if(u1DrvODReadTable(OD_TABLE_33, OD_GREEN, OD_Table_33) == 1)
    {
        bRet = SV_FALSE;
    }

    if(u1DrvODReadTable(OD_TABLE_33, OD_BLUE, OD_Table_33) == 1)
    {
        bRet = SV_FALSE;
    }
    
    // 1. write test data into default table
    vDrvGenSRAMTestTable(OD_Table_41, 1681, u1TablePattern);

    // 2. load data from table into sram
    vDrvODWriteTable(OD_TABLE_41_MODE1, OD_ALL, OD_Table_41);

    // 3. read data back and compare
    if(u1DrvODReadTable(OD_TABLE_41_MODE1, OD_RED, OD_Table_41) == 1)
    {
        bRet = SV_FALSE;
    }

    if(u1DrvODReadTable(OD_TABLE_41_MODE1, OD_GREEN, OD_Table_41) == 1)
    {
        bRet = SV_FALSE;
    }

    if(u1DrvODReadTable(OD_TABLE_41_MODE1, OD_BLUE, OD_Table_41) == 1)
    {
        bRet = SV_FALSE;
    }    


    // 1. write test data into default table
    vDrvGenSRAMTestTable(FB_Table_17, 289, u1TablePattern);

    // 2. load data from table into sram
    vDrvODWriteTable(FB_TABLE_17, OD_ALL, FB_Table_17);

    // 3. read data back and compare
    if(u1DrvODReadTable(FB_TABLE_17, OD_RED, FB_Table_17) == 1)
    {
        bRet = SV_FALSE;
    }
    if(u1DrvODReadTable(FB_TABLE_17, OD_GREEN, FB_Table_17) == 1)
    {
        bRet = SV_FALSE;
    }
    if(u1DrvODReadTable(FB_TABLE_17, OD_BLUE, FB_Table_17) == 1)
    {
        bRet = SV_FALSE;
    }    

    // 4. return result
    return bRet;    
}

void vDrvPQSLTSetParam(UINT8 bEnable, UINT8 u1Module, UINT8 u1TablePattern, UINT32 u4TestRound)
{
    vIO32WriteFldAlign(PQSLT_01, bEnable, PQSLT_EN);
    vIO32WriteFldAlign(PQSLT_01, u1Module, PQSLT_MODULE_SEL);
    vIO32WriteFldAlign(PQSLT_01, u1TablePattern, PQSLT_PATTERN_SEL);
    vIO32WriteFldAlign(PQSLT_02, u4TestRound, PQSLT_ROUND);    
}

BOOL bDrvPeSramStressTest(UINT8 u1Module, UINT8 u1TablePattern, UINT32 u4TestRound)
{
    BOOL bRet = SV_FALSE;
    UINT32 u4CurRound;

    for(u4CurRound = 0; u4CurRound < u4TestRound; u4CurRound ++)
    {    
        switch(u1Module)
        {        
            case 0:        
                bRet = bGammaStressTest(u1TablePattern);
                break;
            case 1:        
                bRet = bSCEStressTest(u1TablePattern);
                break;
            case 2:        
                //bRet = bYLEVSharpnessStressTest(u1TablePattern);
                break;
            case 3:        
                bRet = bScalerStressTest(u1TablePattern);
                break;
            case 4:   
#ifdef CC_MT5396 
                bRet = bPostScalerStressTest(u1TablePattern);
#endif
                break;
            case 5:        
                bRet = bLcDimStressTest(u1TablePattern);
                break;
            case 6:
                bRet = bODStressTest(u1TablePattern);                
                break;
            default:              
                break;
        }

        if(bRet == SV_FALSE)
        {
            Printf("fail the test at round {%d}\n", u4CurRound);
            return SV_FALSE;
        }
        else
        {
            if(u4CurRound % 1000 == 0)
            {
                Printf("pass the test for {%d} rounds\n ", u4CurRound);
            }
        }
    }

    return bRet;
}

void vDrvPQStressProc()
{
    UINT8 u1PQSLTModule;
    UINT8 u1PQSLTPattern;
    UINT32 u4PQSLTRound;
    BOOL bPQSLTResult;    

    if(IO32ReadFldAlign(PQSLT_01, PQSLT_EN) == SV_TRUE)
    {
		// disable watchdog
        vIO32Write4B(PDWNC_WDTCTL, 0);     

        u1PQSLTModule = IO32ReadFldAlign(PQSLT_01, PQSLT_MODULE_SEL);
        u1PQSLTPattern = IO32ReadFldAlign(PQSLT_01, PQSLT_PATTERN_SEL);
        u4PQSLTRound = IO32ReadFldAlign(PQSLT_02, PQSLT_ROUND);
        
        vIO32WriteFldAlign(PQSLT_01, SV_FALSE, PQSLT_DONE);
        vIO32WriteFldAlign(PQSLT_01, SV_FALSE, PQSLT_RESULT);

        bPQSLTResult = bDrvPeSramStressTest(u1PQSLTModule, u1PQSLTPattern, u4PQSLTRound);
        
        vIO32WriteFldAlign(PQSLT_01, SV_FALSE, PQSLT_EN);

        Printf("PQ SLT on module {%d} for {%d} rounds in pattern {%d}, result :", u1PQSLTModule, u4PQSLTRound, u1PQSLTPattern);

        if(bPQSLTResult == SV_TRUE)
        {
            Printf(" pass.\n");
        }
        else
        {
            Printf(" fail.\n");            
        }
        // enable watchdog
        vIO32Write4B(PDWNC_WDTCTL, 1);             
    }
}
#endif //#ifndef NDEBUG 

