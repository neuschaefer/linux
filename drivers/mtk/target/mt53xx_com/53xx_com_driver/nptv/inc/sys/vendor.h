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
 * $Date  $
 * $RCSfile: vendor.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _VENDOR_H
#define _VENDOR_H

#include "panel.h"

/*
 * This file contains prototype customization settings for vendor's system.
   Description, value range and note are also attached.

 * There're following setting levels for customization:
   + Basic
     1. Can be customized freely if there isn't special request.
     2. All comments are outlined with '********'.

   + Advanced
     1. Should be customized carefully after proper understanding comments.
     2. All comments are outlined with '--------'.

   + Constraint
     1. Needn't be customized usually to meet some limitation or regulation.
     2. All comments are outlined with '========'.

   + Notes:
     1. The //(X) sign after #define means not customizable.
     2. The //(NO USE NOW) sign after #define means it's no use now.
 */

////////////////////////////////////////////////////////////////////////////////
// Module On/Off Setting
////////////////////////////////////////////////////////////////////////////////

	#define SUPPORT_DISPLAY 1
	#define SUPPORT_BACKLT 1// 5371 !CONFIG_MT8211
	#define SUPPORT_VIDEO 1
	#define SUPPORT_DSP 0// 5371!CONFIG_MT8211
	#define SUPPORT_EEP 0//1
	#define SUPPORT_TUNER 1
	#define SUPPORT_RADIO 0	
	#define SUPPORT_OSD 0//1
	#define SUPPORT_HW_INIT 1	
	#define SUPPORT_POWER 0//1
	
////////////////////////////////////////////////////////////////////////////////
// Tuner and correspoding UI Part
////////////////////////////////////////////////////////////////////////////////

	/****************************************************************************
	SUPPORT_MULTI_SYSTEM: Define if support multi-system is supported.
		1 - Indicates multi-system is supported, uni-system is not supported
		0 - Indicates multi-system is not supported, uni-system is supported

	SUPPORT_NTSC_TV:
		1 - Indicates TV signal is in NTSC mode
		0 - Indicates TV signal is in PAL  mode
	It's constraint definition that Multi System doen't support NTSTC TV signal
	and Uni System doen't support PAL TV signal.

	SUPPORT_JAPAN_TV:
		Japan TV interface can be active only under NTSC_TV
		Use (1 &&SUPPORT_NTSC_TV) to enable Japan TV functions when SUPPORT_NTSC_TV is 1
	****************************************************************************/
	#ifndef SUPPORT_MULTI_SYSTEM
	#define SUPPORT_MULTI_SYSTEM 0
	#else
	#undef SUPPORT_MULTI_SYSTEM
	#define SUPPORT_MULTI_SYSTEM 1
	#endif

	#define SUPPORT_NTSC_TV	(!SUPPORT_MULTI_SYSTEM)
	#define SUPPORT_JAPAN_TV 	(0 &&SUPPORT_NTSC_TV)	


	/****************************************************************************
	Define if PAL tuner is used and set the TVD AUTO detection default to 50Hz.
	****************************************************************************/
	#define TVD_DFT_PAL SUPPORT_MULTI_SYSTEM


	
////////////////////////////////////////////////////////////////////////////////
// Teletext/V-Chip/Closed-Caption/Auto-Naming/WSS Part
////////////////////////////////////////////////////////////////////////////////

	/****************************************************************************
	Define whether Teletext Decoder or Close Caption Decoder is supported.
	Teletext Decoder and Close Caption Decoder can't be supported simultaneously.
	****************************************************************************/
	#define SUPPORT_CLOSE_CAPTION_VCHIP 1 /* Close Caption Decoder */
	#define SUPPORT_TELETEXT 1 /* Teletext Decoder */		
	#define SUPPORT_AUTO_NAMING 1 /* VPS */	
	#define SUPPORT_WSS 1                 /* WSS */

	#define SUPPORT_VBI (SUPPORT_CLOSE_CAPTION_VCHIP|SUPPORT_WSS|SUPPORT_TELETEXT|SUPPORT_AUTO_NAMING)

////////////////////////////////////////////////////////////////////////////////
// Input Source Part
////////////////////////////////////////////////////////////////////////////////

	#include "source_table.h"

	/*---------------------------------------------------------------------------
	Scart auto source switch support (depend on pin 8 and pin 16 change).
	---------------------------------------------------------------------------*/
	#define SUPPORT_SCART  (SUPPORT_SCART1_INPUT || SUPPORT_SCART2_INPUT || SUPPORT_SCART3_INPUT)
	#define SUPPORT_PIP_NOSCARTOUT 0
	/*---------------------------------------------------------------------------
	Disable or Enable scart pin8 functions (scart plug detection & picture size auto)
	---------------------------------------------------------------------------*/
	#define ENABLE_SCART_PIN8	SUPPORT_SCART     
     
	//efine SUPPORT_TTX25                            0

#endif /* _VENDOR_H */
