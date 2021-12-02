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
 * $RCSfile: atd_feature.h $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef ATD_FEATURE_H
#define ATD_FEATURE_H

#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#define TVD_SUPPORT_SNOW_MODE 1
#else
#define TVD_SUPPORT_SNOW_MODE 0
#endif

#if (defined(CC_SY_GA) ||defined(CC_SUPPORT_ATV_SNOWSCREEN))
#if !TVD_SUPPORT_SNOW_MODE
#define ATD_SNOWSCREEN_MODE //20110718_Haibo:If not support TVD snow mode,use ATD snow mode
#else 
#define TVD_SNOWSCREEN_MODE
#endif
#endif


#if (defined(CC_SY_GA)||defined(CC_SUPPORT_PA_ALL_ATV))
#define ATD_NTSC443_DETECT
#endif
#if defined(CC_MT5396)
#define IS_MT5396MP_ATD 1//ES or MP ver
#endif

#if defined(CC_MT5368)
#define IS_MT5368MP_ATD 0//ES or MP ver
#endif

#ifndef IS_MT5396MP_ATD
#define IS_MT5396MP_ATD 1
#endif

#ifndef IS_MT5368MP_ATD
#define IS_MT5368MP_ATD 1
#endif

#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#if defined(DTD_INCLUDE)
#define ATD_SUPPORT_BATCH_RW 0//d110623_Haibo:cannot support batch read with DTD+MT5396/68/89
#if !IS_MT5396MP_ATD || !IS_MT5368MP_ATD
#define ATD_DRO_BYPASS_FIFO //d110701_Haibo:MT5396/68 ECO item
#endif
#endif
#endif



#ifndef ATD_SUPPORT_BATCH_RW
#define ATD_SUPPORT_BATCH_RW 1//d110623_Haibo:default support batch read
#endif

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT8223)
#define ATD_SUPPORT_DRO_SYNC_MODE 1
#define ATD_SUPPORT_ENLARGED_SCALING_GAIN 0
#endif

#if defined(CC_MT5396)
#if IS_MT5396MP_ATD
#define ATD_SUPPORT_OVM_HW_MAX_HOLD 1
#define ATD_SUPPORT_OVM_SOUND_INDEPENDENT 1
#define ATD_SUPPORT_OVM_ACCURATE_RANK 1
#define ATD_SUPPORT_PHASE_AVERAGE_MODE 1
#define ATD_SUPPORT_HW_AUTO_DISABLE_AVG 1
#define ATD_SUPPORT_CR_ENLARGED_FIFO 1
#define ATD_SUPPORT_AVERAGED_CMETRIC 1
#define ATD_SUPPORT_PJ_OVM_INDEPENDENT 1
#define ATD_SUPPORT_H_BLANK_MODE 1
#define ATD_SUPPORT_QUICK_MIDDLE_TERM 1
#endif
#endif

#if defined(CC_MT5368)
#if IS_MT5368MP_ATD
#define ATD_SUPPORT_OVM_HW_MAX_HOLD 1
#define ATD_SUPPORT_OVM_SOUND_INDEPENDENT 1
#define ATD_SUPPORT_OVM_ACCURATE_RANK 1
#define ATD_SUPPORT_PHASE_AVERAGE_MODE 1
#define ATD_SUPPORT_HW_AUTO_DISABLE_AVG 1
#define ATD_SUPPORT_CR_ENLARGED_FIFO 1
#define ATD_SUPPORT_AVERAGED_CMETRIC 1
#define ATD_SUPPORT_PJ_OVM_INDEPENDENT 1
#define ATD_SUPPORT_H_BLANK_MODE 1
#endif
#endif

#if defined(CC_MT5389)
#define ATD_SUPPORT_OVM_HW_MAX_HOLD 1
#define ATD_SUPPORT_PHASE_AVERAGE_MODE 1
#define ATD_SUPPORT_CR_ENLARGED_FIFO 1
#define ATD_SUPPORT_AVERAGED_CMETRIC 1
#endif


//default setting 
#ifndef ATD_SUPPORT_OVM_HW_MAX_HOLD
#define ATD_SUPPORT_OVM_HW_MAX_HOLD 0
#endif

#ifndef ATD_SUPPORT_OVM_SOUND_INDEPENDENT
#define ATD_SUPPORT_OVM_SOUND_INDEPENDENT 0
#endif

#ifndef ATD_SUPPORT_OVM_ACCURATE_RANK
#define ATD_SUPPORT_OVM_ACCURATE_RANK 0
#endif

#ifndef ATD_SUPPORT_PHASE_AVERAGE_MODE
#define ATD_SUPPORT_PHASE_AVERAGE_MODE 0
#endif

#ifndef ATD_SUPPORT_HW_AUTO_DISABLE_AVG
#define ATD_SUPPORT_HW_AUTO_DISABLE_AVG 0
#endif

#ifndef ATD_SUPPORT_CR_ENLARGED_FIFO
#define ATD_SUPPORT_CR_ENLARGED_FIFO 0
#endif

#ifndef ATD_SUPPORT_AVERAGED_CMETRIC
#define ATD_SUPPORT_AVERAGED_CMETRIC 0
#endif

#ifndef ATD_SUPPORT_PJ_OVM_INDEPENDENT
#define ATD_SUPPORT_PJ_OVM_INDEPENDENT 0
#endif

#ifndef ATD_SUPPORT_H_BLANK_MODE
#define ATD_SUPPORT_H_BLANK_MODE 0
#endif

#ifndef ATD_SUPPORT_DRO_SYNC_MODE
#define ATD_SUPPORT_DRO_SYNC_MODE 0
#endif

#if ATD_SUPPORT_PHASE_AVERAGE_MODE
#define fgEnable_CR_AVG_Mode 0 //default always disable CR phase average mode
#endif

#ifndef fgEnable_CR_AVG_Mode
#define fgEnable_CR_AVG_Mode 0
#endif

#ifndef ATD_SUPPORT_ENLARGED_SCALING_GAIN
#define ATD_SUPPORT_ENLARGED_SCALING_GAIN 1
#endif

#ifndef ATD_SUPPORT_QUICK_MIDDLE_TERM
#define ATD_SUPPORT_QUICK_MIDDLE_TERM 0
#endif

#endif

