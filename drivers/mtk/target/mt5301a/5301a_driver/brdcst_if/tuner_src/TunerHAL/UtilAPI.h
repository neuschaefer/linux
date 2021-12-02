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
/*    FILE DESCRIPTION                           UtilAPI.h             */
/*      This file is the header file of UtilAPI.c.                     */
/*                                                                     */
/*    LEGAL DISCLAIMER                                                 */
/*                                                                     */
/*  BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY          */
/*  ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS         */
/*  DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED FROM MEDIATEK AND/OR */
/*  ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS      */
/*  ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS */
/*  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF */
/*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR               */
/*  NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY        */
/*  WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH   */
/*  MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK     */
/*  SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR    */
/*  ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE    */
/*  RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S     */
/*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN       */
/*  FORUM.                                                             */
/*                                                                     */
/*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND        */
/*  CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE         */
/*  RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR     */
/*  REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE     */
/*  LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH  */
/*  MEDIATEK SOFTWARE AT ISSUE.                                        */
/*                                                                     */
/*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN       */
/*  ACCORDANCE WITH THE LAWS OF THE STATE OF CALIFORNIA, USA,          */
/*  EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.                         */
/*                                                                     */
/*    Creator                                                          */
/*      Ja-Shun Wan      MediaTek Inc.   2005.09                       */
/*                                                                     */
/***********************************************************************/

#ifndef _UTILAPI_H_
#define _UTILAPI_H_

#include "queue.h"

S32 Util_TunerModSize(TUNER_MODULATION_T tuner_mod);

INT32 Util_TranslateConnectInfo(DRV_CONN_TYPE_T  e_conn_type, 
                           VOID *pv_conn_info, SIZE_T z_conn_info_len,
                           UINT32 *pFreq1, UINT32 *pSymbolRate,
                           TUNER_MODULATION_T *pe_mode,
                           BOOL *pSpecInv);

INT32 Util_InterlockedIncrement(SEMA_T RefSema, long* pRefCnt);
INT32 Util_InterlockedDecrement(SEMA_T RefSema, long* pRefCnt);

#endif /* _UTILAPI_H_ */
