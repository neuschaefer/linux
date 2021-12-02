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
 * MUSBStack-S buffer-overrun-safe micro-string library API.
 * $Revision: #1 $
 */

#ifndef __MUSB_STRING_H__
#define __MUSB_STRING_H__

#ifdef MUSB_DIAG

#include "mu_tools.h"

/**
 * Compare strings.
 * Lexically compare two strings.
 * @param pString1 first NUL-terminated ASCII string
 * @param pString2 second NUL-terminated ASCII string
 * @param bCaseSensitive TRUE for case-sensitive letter comparison;
 * FALSE to ignore case
 * @return 0 if strings are identical
 */
extern CHAR MUSB_StringCompare(const CHAR* pString1, 
				 const CHAR* pString2, uint8_t bCaseSensitive);

/**
 * strncat-analogue.
 * Concatenate a string to the end of another.
 * @param pResult the NUL-terminated ASCII string serving as the 
 * first string and result buffer
 * @param wResultLength length of result buffer
 * @param pConcat NUL-terminated ASCII string to append
 * @return TRUE on success
 * @return FALSE on failure (result would overflow buffer)
 */
extern uint8_t MUSB_StringConcat(CHAR* pResult, 
				 uint16_t wResultLength, const CHAR* pConcat);

/**
 * strstr-analogue.
 * Find the first occurence of a pattern in a string.
 * @param pString the NUL-terminated ASCII string in which to search
 * @param pPattern the NUL-terminated ASCII pattern for which to search
 * @param bCaseSensitive TRUE for case sensite letter comparison; 
 * FALSE to ignore case
 * @return pointer to first occurence if found; NULL if not found
 */
extern const CHAR* MUSB_StringFind(const CHAR* pString,
				   const CHAR* pPattern, uint8_t bCaseSensitive);

/**
 * Integer-to-string.
 * Convert an unsigned integer to a string.
 * @param pResult the result buffer
 * @param wResultLength length of result buffer
 * @param dwNumber the unsigned integer
 * @param bBase 2, 8, 10, or 16 for binary, octal, decimal, or hex
 * @param bJustification if non-zero, pad with leading 0s as necessary
 * to output this length
 * @return TRUE on success
 * @return FALSE on failure (result would overflow buffer)
 */
extern uint8_t MUSB_Stringize(CHAR* pResult, uint16_t wResultLength, 
			      uint32_t dwNumber, uint8_t bBase, 
			      uint8_t bJustification);
 
/**
 * strlen-analogue.
 * Return the length of a NUL-terminated ASCII string.
 * @param pString the NUL-terminated ASCII string
 * @return Length, in CHARacters, of the string
 */
extern uint16_t MUSB_StringLength(const CHAR* pString);

/**
 * strtol-analogue.
 * Parse a string in a given base (or any base using a limited set of specifiers).
 * @param pString the NUL-terminated ASCII string
 * @param pEnd filled with a pointer to the CHARacter immediately following
 * the last parsed CHARacter
 * @param bBase 2, 8, 10, or 16 for a specific base, or 0 to determine the base
 * from the string (i.e. leading 0x or 0X denotes base 16; 0 for octal, % for binary)
 * @return the number, or 0 on failure (on failure, pEnd==pString)
 */
extern int32_t MUSB_StringParse(const CHAR* pString, const CHAR** pEnd, 
				uint8_t bBase);

#endif  /* MUSB_DIAG > 0 */

#endif	/* multiple inclusion protection */
