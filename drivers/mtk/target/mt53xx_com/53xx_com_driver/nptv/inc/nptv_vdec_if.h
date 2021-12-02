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
 * $RCSfile: nptv_vdec_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nptv_vdec_if.h
 *  NPTV Video interface for decoder, including VGA, YPbpr, and DVI
 */

#ifndef _NPTV_VDEC_IF_H_
#define _NPTV_VDEC_IF_H_

#include "x_typedef.h"
#include "sv_const.h"
#include "drv_vbi_if.h"

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------



///-----------------------------------------------------------------------------
/// Public functions
///-----------------------------------------------------------------------------
/**
 * @brief vApiVideoVgaSetPhase(UINT8 bPath, UINT8 bValue) 
 * Set Vga phase
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @param bValue  : Phase value (0~31)
 * @retval void
 * @example vApiVideoVgaSetPhase(SV_VP_MAIN, 5);
 */
EXTERN UINT8 fgApiVideoVgaSetPhase(UINT8 bValue);

/**
 * @brief vApiVideoVgaSetClock(UINT8 bPath, UINT16 wValue) 
 * Set VGA Clock value
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @param wValue  : clock  value (not UI value)
 * @retval void
 * @example vApiVideoVgaSetClock(SV_VP_MAIN, 1024);
 */
EXTERN UINT8 fgApiVideoVgaSetClock(UINT16 wValue); 

/**
 * @brief fgApiVideoGeoHPosition(UINT8 bPath, UINT16 wValue) 
 * Set VGA H Position
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @param wValue  : H Position (not UI value) (H porch)
 * @retval void
 * @example fgApiVideoGeoHPosition(SV_VP_MAIN, 100);
 */
EXTERN UINT8 fgApiVideoGeoHPosition(UINT8 bPath, UINT16 wValue); 

/**
 * @brief fgApiVideoGeoVPosition(UINT8 bPath, UINT16 wValue) 
 * Set VGA V Position
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @param wValue  : V Position (not UI value) 
 * @retval void
 * @example fgApiVideoGeoVPosition(SV_VP_MAIN, 30);
 */
EXTERN UINT8 fgApiVideoGeoVPosition(UINT8 bPath, UINT16 wValue);

/**
 * @brief fgApiVideoVgaAuto(void) 
 * Trigger Auto state machine entry,Do it when path is VGA or DVI+ext AD
 * @param void
 * @retval void
 * @example vApiVideoVgaAuto(void);
 */
EXTERN UINT8 fgApiVideoVgaAuto(void);

/**
 * 	StopVGA AutoClock, AutoPhase, and AutoPosition
 *
 * @return NONE
 * @retval void
 */
EXTERN void vApiVgaAutoStop(void);

/**
 * 	VGA call-back function for vVdoMainState, vVdoPipState polling update video status to UI
 *
 *  @retval SV_VDO_UNKNOWN / SV_VDO_NOSIGNAL / SV_VDO_NOSUPPORT / SV_VDO_STABLE
 */
EXTERN UINT8 bApiVgaSigStatus(void);

/**
 * @brief fgApiVideoVgaAuto(void) 
 * Trigger Auto state machine entry,Do it when path is VGA or DVI+ext AD
 * @param void
 * @retval void
 * @example vApiVideoVgaAuto(void);
 */
EXTERN UINT8 fgApiVideoYPbPrAuto(void);

/**
 * @brief fgApiVideoAutoColor(void) 
 * Trigger Auto Color state machine (VGA or YPbPr)
 * @param void
 * @retval UINT8
 * @example fgApiVideoAutoColor(void);
 */
EXTERN UINT8 fgApiVideoAutoColor(void);

/**
 * @brief fgApiVideoCVBSAutoColor(void) 
 * Trigger CVBS Auto Color state machine 
 * @param void
 * @retval UINT8
 * @example fgApiVideoCVBSAutoColor(void);
 */
EXTERN UINT8 fgApiVideoCVBSAutoColor(void); 

/**
 * @brief wApiVideoGetVgaStdHtotal(void)
 * Get  Standard Htotal value of current VGA timing
 * @param void
 * @retval :Htotal value
 */
EXTERN UINT16 wApiVideoGetVgaStdHtotal(void);

/**
 * @brief wApiVideoGetVgaStdHPos(void)
 * Get  Standard H position value of current VGA timing
 * @param void
 * @retval :H position value
 */
EXTERN UINT16 wApiVideoGetVgaStdHPos(void);

/**
 * @brief wApiVideoGetVgaStdVPos(void)
 * Get  Standard V position value of current VGA timing
 * @param void
 * @retval :V position value
 */
EXTERN UINT16 wApiVideoGetVgaStdVPos(void);

#endif // _NPTV_VDEC_IF_H_
 
