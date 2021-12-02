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

/*
 * MUSBStack-S diagnostic messaging macros.
 * $Revision: #1 $
 */

#ifndef __MUSB_DIAG_H__
#define __MUSB_DIAG_H__

#include "mu_stdio.h"
#include "mu_strng.h"

/*
 * Diagnostics
 */
#ifdef MUSB_DIAG
extern uint8_t MGC_bDiagLevel;

/**
 * Print diagnostic string.
 * Use this to eliminate diagnostic code when not compiled with diagnostics.
 * @param _bLevel diagnostic level
 * @param _pMsg message to print
 */
#define MUSB_DIAG_STRING(_bLevel, _pMsg) if(MGC_bDiagLevel >= _bLevel) \
    { LOG(6, "%s\r\n", _pMsg); }

/**
 * Print a diagnostic with a string and number.
 * Use this to eliminate diagnostic code when not compiled with diagnostics.
 * @param _bLevel diagnostic level
 * @param _pText the text portion of the message; printed first
 * @param _dwNum the numeric portion of the message; appended to the text portion
 * @param _bBase numeric base (2, 8, 10 or 16)
 * @param _bJust the justification (0 for none); this is for leading-zero padding
 */
#define MUSB_DIAG1(_bLevel, _pText, _dwNum, _bBase, _bJust) \
if (MGC_bDiagLevel >= _bLevel) { \
    LOG(6, "%s 0x%X\r\n", _pText, _dwNum);}    
/**
 * Print a diagnostic with a two strings and two numbers.
 * Use this to eliminate diagnostic code when not compiled with diagnostics.
 * Print a diagnostic with a string followed by a number, followed by another set.
 * @param _bLevel diagnostic level
 * @param _pText1 the first text portion of the message; printed first
 * @param _dwNum1 the first numeric portion of the message; appended to the above
 * @param _pText2 the second text portion of the message; appended to the above
 * @param _dwNum2 the second numeric portion of the message; appended to the above
 * @param _bBase numeric base for both numbers (2, 8, 10 or 16)
 * @param _bJust the justification (0 for none); this is for leading-zero padding
 */
#define MUSB_DIAG2(_bLevel, _pText1, _dwNum1, _pText2, _dwNum2, _bBase, _bJust) \
if (MGC_bDiagLevel >= _bLevel) { \
    LOG(6, "%s 0x%X, %s 0x%X\r\n", _pText1, _dwNum1, _pText2, _dwNum2);}

#else
/* Print diagnostic */
#define MUSB_DIAG_STRING(_bLevel, _pMsg)    USBLOG_##_bLevel(_pMsg)

#define USBLOG_1(_pMsg)    LOG(5, "%s\r\n", _pMsg)
#define USBLOG_2(_pMsg)
#define USBLOG_3(_pMsg)

#define MUSB_DIAG1(_bLevel, _pText1, _dwNum1, _bBase, _bJust)
#define MUSB_DIAG2(_bLevel, _pText1, _dwNum1, _pText2, _dwNum2, _bBase, _bJust)

#endif

#endif	/* multiple inclusion protection */
