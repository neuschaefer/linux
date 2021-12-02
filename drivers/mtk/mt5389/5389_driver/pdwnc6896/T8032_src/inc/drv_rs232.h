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
 *   $Workfile: drv_rs232.h $ 
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 *   RS232 Interrupt Service Routine and Driver
 *   
 * Author:
 * -------
 *   
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/03/15 5:07p $  
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _DRV_RS232_H_
#define _DRV_RS232_H_

#if SUPPORT_RS232_DEBUG
extern void vRs232Init(UINT8 b);
extern void vRs232ISR(void);
extern void vRs232PriorityProc(void);

#else
#define vRs232Init(x)
#define vRs232ISR()
#endif

#if (SUPPORT_RS232_DEBUG && ENABLE_RS232_LOG)
extern void LogS(const char *pcStr) reentrant;
extern void Log4B(UINT8 bV1, UINT8 bV2, UINT8 bV3, UINT8 bV4) reentrant;
extern void LogB(UINT8 bVal);
extern void LogW(UINT16 wVal);
extern void LogD(UINT32 dVal) reentrant;

extern void ISR_LogS(const char *pcStr) reentrant;
extern void ISR_Log4B(UINT8 bV1, UINT8 bV2, UINT8 bV3, UINT8 bV4) reentrant;
extern void ISR_LogB(UINT8 bVal);
extern void ISR_LogW(UINT16 wVal);
extern void ISR_LogD(UINT32 dVal)reentrant ;
    #if RW_REG_INTERFACE
    extern void _LogS(const char *pcStr) reentrant;
    extern void Print8bitsHex(UINT8 num)  reentrant;
    extern void _Print8bitsHex(UINT8 num)  reentrant;
    #endif
#else
#define LogS(pcStr)
#define Log4B(bV1, bV2, bV3, bV4)
#define LogB(val)
#define LogW(val)
#define LogD(val)

#define ISR_LogS_LogS(pcStr)
#define ISR_LogS_Log4B(bV1, bV2, bV3, bV4)
#define ISR_LogS_LogB(val)
#define ISR_LogS_LogW(val)
#define ISR_LogS_LogD(val)

#endif
extern void MTKPrintf(const CHAR * format, ...);
extern void vRs232Service(void);

#define LogSB(str, byte) \
        LogS(str); \
        LogB(byte);
        
#define LogSW(str, word) \
        LogS(str); \
        LogW(word);
        
#define LogSD(str, dword) \
        LogS(str); \
        LogD(dword);



#define ISR_LogSB(str, byte) \
        ISR_LogS(str); \
        ISR_LogB(byte);
        
#define ISR_LogSW(str, word) \
        ISR_LogS(str); \
        ISR_LogW(word);
        
#define ISR_LogSD(str, dword) \
        ISR_LogS(str); \
        ISR_LogD(dword);


#endif /* _DRV_RS232_H_ */
