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
 * $RCSfile: dsp_slt.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_slt.h
 *  Brief of file dsp_slt.h.
 *  Details of file dsp_slt.h (optional).
 */
 
#ifndef DSP_SLT_H
#define DSP_SLT_H

#define ADSPSLT_THREAD_NAME         "ADSP_SLT"
#define ADSPSLT_STACK_SIZE          2048
#define ADSPSLT_THREAD1_PRIORITY    100

extern UINT8 u1ADSPSLTPatID;

extern void ADSP_SLT_Init(void);
extern void ADSP_SLT_SetPatID(UINT8 u1PID);
    
enum
{
    ADSP_TEST_PATTERNNULL           = 0,
    ADSP_TEST_PATTERN1              = 1,
    ADSP_TEST_PATTERN2              = 2,
    ADSP_TEST_PATTERN3              = 3,
    ADSP_TEST_PATTERN4              = 4,
    ADSP_TEST_PATTERN5              = 5,
    ADSP_TEST_PATTERN6              = 6,
    ADSP_PATTERN_CNT                = ADSP_TEST_PATTERN6
};


/******************************************************************************
* ADSP_SLT console API
******************************************************************************/
extern void ADSP_SLT_Init(void);

/******************************************************************************
* ADSP_SLT DSP Data
******************************************************************************/
// The Size and Start Address are define in UINT32 address
// If SA = X, Real SA in Byte is 4X, from first byte of header
// If SZ = Y, Real SZ in Byte is 4Y
enum
{
    DSPF_SLT_IMAGE_SA           =0,
    DSPF_SLT_IMAGE_SZ,

    DSPF_MEM_PATTERN_SA,
    DSPF_MEM_PATTERN_SZ,

    DSPF_INST_PATTERN_SA,
    DSPF_INST_PATTERN_SZ,

    DSPF_DRAM_PATTERN_SA,
    DSPF_DRAM_PATTERN_SZ,
    
    DSPF_GETBITS_PATTERN_SA,
    DSPF_GETBITS_PATTERN_SZ,
    
    DSPF_PUTBITS_PATTERN_SA,
    DSPF_PUTBITS_PATTERN_SZ,
    
    DSPF_CORDIC_PATTERN_SA,
    DSPF_CORDIC_PATTERN_SZ
};

/******************************************************************************
* ADSP_SLT DSP Share Memory: Located in Decoder #1 Page
******************************************************************************/
#define     ADSPSLT_PAT_1_BASE      (0x00000000) // 0x8000
#define     ADSPSLT_PAT_1_DONE      (ADSPSLT_PAT_1_BASE+ 0x00)
#define     ADSPSLT_PAT_1_RSLT      (ADSPSLT_PAT_1_BASE+ 0x01)
#define     ADSPSLT_PAT_1_CNTR      (ADSPSLT_PAT_1_BASE+ 0x02)

#define     ADSPSLT_PAT_2_BASE      (0x00000000) // 0x8000
#define     ADSPSLT_PAT_2_DONE      (ADSPSLT_PAT_2_BASE+ 0x00)
#define     ADSPSLT_PAT_2_RSLT      (ADSPSLT_PAT_2_BASE+ 0x01)
#define     ADSPSLT_PAT_2_CNTR      (ADSPSLT_PAT_2_BASE+ 0x02)

#define     ADSPSLT_PAT_3_BASE      (0x00000300) // 0x8300
#define     ADSPSLT_PAT_3_DONE      (ADSPSLT_PAT_3_BASE+ 0x00)
#define     ADSPSLT_PAT_3_RSLT      (ADSPSLT_PAT_3_BASE+ 0x01)
#define     ADSPSLT_PAT_3_CNTR      (ADSPSLT_PAT_3_BASE+ 0x02)

#define     ADSPSLT_PAT_4_BASE      (0x00000000) // 0x8000
#define     ADSPSLT_PAT_4_DONE      (ADSPSLT_PAT_4_BASE+ 0x00)
#define     ADSPSLT_PAT_4_RSLT      (ADSPSLT_PAT_4_BASE+ 0x01)
#define     ADSPSLT_PAT_4_CNTR      (ADSPSLT_PAT_4_BASE+ 0x02)

#define     ADSPSLT_PAT_5_BASE      (0x00000000) // 0x8000
#define     ADSPSLT_PAT_5_DONE      (ADSPSLT_PAT_5_BASE+ 0x00)
#define     ADSPSLT_PAT_5_RSLT      (ADSPSLT_PAT_5_BASE+ 0x01)
#define     ADSPSLT_PAT_5_CNTR      (ADSPSLT_PAT_5_BASE+ 0x02)

#define     ADSPSLT_PAT_6_BASE      (0x00000100) // 0x8100
#define     ADSPSLT_PAT_6_DONE      (ADSPSLT_PAT_6_BASE+ 0x00)
#define     ADSPSLT_PAT_6_RSLT      (ADSPSLT_PAT_6_BASE+ 0x01)
#define     ADSPSLT_PAT_6_CNTR      (ADSPSLT_PAT_6_BASE+ 0x02)

#endif  // DSP_SLT_H


