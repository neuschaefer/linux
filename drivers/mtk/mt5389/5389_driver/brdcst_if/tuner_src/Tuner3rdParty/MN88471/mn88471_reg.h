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
/*==========================================================================================*/
/*  Panasonic System LSI Design Europe, a Division of Panasonic Europe Ltd.					*/
/*                                                                                          */
/*  Module Name:            MN88471.cpp														*/
/*  Author (Nationality):   Luke Grove (British)	                                        */
/*  Description:            Paired definition of register addresses and default values.		*/
/*																							*/
/*  Date First Created:     1st January 2010                                                */
/*  Related Specifications (PSDE document refs):                                            */
/*  Master file stored in CVS at: $Achive: $ */
/*                                                                                          */
/*  Status (choose one):    (*) under development           ( ) internal release            */
/*                          ( ) external release            ( ) example only                */
/*  Confidentiality:        ( ) Strictly Confidential       (*) Confidential                */
/*  (choose one)            ( ) Internal Use Only (IUO)     ( ) Open                        */
/*                                                                                          */
/*  Note: This code should only be viewed or otherwise used by customers of Panasonic after */
/*  an appropriate Non-Disclosure Agreement and/or Licence Agreement has been signed by.    */
/*  both Panasonic and the customer.  If in doubt, please check with your Panasonic         */
/*  representative. If you have reason to believe this code is NOT subject to the above,    */
/*  please delete all copies of it and related files from your systems immediately.         */
/*                                                                                          */
/*  (c) 2010 Panasonic Europe Limited.														*/
/*==========================================================================================*/

/*==========================================================================================*/
/*  THIS PROGRAM IS EXAMPLE SOFTWARE PROVIDED TO CUSTOMERS OF PANASONIC IN THE HOPE THAT IT */
/*  WILL BE USEFUL, BUT WITHOUT ANY WARRANTY EXPRESSED OR IMPLIED.  THE PROGRAM IS PROVIDED */
/*  ON AN "AS IS" BASIS.  THE ENTIRE RISK AS TO THE PROGRAM QUALITY AND PERFORMANCE IS WITH */
/*  THE USER. IN NO EVENT WILL PANASONIC BE LIABLE FOR DIRECT,  INDIRECT,  OR INCIDENTAL OR */
/*  CONSEQUENTIAL DAMAGES RESULTING FROM ANY DEFECT IN THE PROGRAM,   EVEN IF PANASONIC HAS */
/*  BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                                        */
/*                                                                                          */
/*  THIS PROGRAM  HAS BEEN DEVELOPED  OUTSIDE THE SCOPE OF  PANASONIC SEMICONDUCTOR COMPANY */
/*  DEVELOPMENT PROCEDURES,  AND IS PROVIDED ONLY AS EXAMPLE SOFTWARE.                      */
/*==========================================================================================*/

#ifndef __MN88471_REG__H__
#define __MN88471_REG__H__

#ifdef __cplusplus
extern "C"
{
#endif
#define PLLSET1     0x0
#define PLLSET2     0x1
#define PLLSET      0x2
#define RCVSET      0x3
#define PWDSET      0x4
#define TSSET1      0x5
#define TSSET2      0x6
#define PLDWSET     0x7
#define HIZSET1     0x0A
#define HIZSET2     0x0B
#define SYSSET      0xF
#define CLKSET1     0x10
#define SSEQSET     0x12
#define SSPRESET    0x14
#define NCOFREQU    0x23
#define NCOFREQM    0x24
#define NCOFREQL    0x25
#define FADU	    0x26
#define FADM	    0x27
#define FADL	    0x28
#define AGCREF      0x29
#define FECSET1     0x50
#define PLPID       0x52
#define IMPSET      0x57
#define ICISET      0x62
#define DETQCSET    0x6B
#define DETADSET    0x6C
#define SYN1RDSET   0x8B
#define DETRDSET1   0x8E
#define BERSET      0x91
#define BERLEN      0x92
#define SSEQFLG     0x94
#define SSEQRD1     0x95
#define AGCRDU		0x9C
#define AGCRDL		0x9D
#define P1RD        0x9E
#define DETRDU      0xA5
#define DETRDM      0xA6
#define DETRDL      0xA7
#define CNRDU       0xAF
#define CNRDL       0xB0
#define BERFLG		0xB1
#define BERRDU      0xB2
#define BERRDM      0xB3
#define BERRDL      0xB4
#define TPDSET1     0xB6
#define TPDSET2     0xB7
#define TPD1        0xB8
#define TPD2        0xB9
#define TPD3        0xBA
#define TPD4        0xBB
#define TPD5        0xBC
#define TPD6        0xBD
#define TPD7        0xBE
#define TPD8        0xBF
#define TCBSET      0xDE
#define TCBADR      0xE0
#define TCBCOM      0xE9
#define RSTSET1     0xEA
#define RSTSET2     0xEB
#define RSTSET3     0xEC
#define PSEQOP2     0xF2
#define PSEQOP4     0xF4
#define PSEQSET     0xF7
#define PSEQPRG     0xF8
#define PSEQFLG     0xFA

    /*DVB-T registers*/
#define MDSET_T      0x00
#define MDASET_T     0x01
#define RSDSET_T     0x16
#define FFILSET1_T   0x3A  
#define WPCSET_T     0x3D
#define BERSET1_T    0x62         
#define BERSTSET_T   0x65         
#define BERRDSET_T   0x67        
#define SSEQRD_T     0x69
#define CNRDU_T      0x73
#define CNRDL_T      0x74
#define BERRDU_T     0x76
#define BERRDM_T     0x77
#define BERRDL_T     0x78
#define BERLENRDU_T  0x79
#define BERLENRDL_T  0x7A  

    /*DVB-C registers*/
#define MDSET_C      0x80
#define MDDEF0_C     0x81
#define MDDEF1_C     0x82
#define MDRD_C       0xCA
#define SSEQRD_C     0xCB
#define RATEDETU_C	 0xCF
#define RATEDETM_C	 0xD0
#define RATEDETL_C	 0xD1
#define CNMON1_C     0xE3
#define CNMON2_C     0xE4
#define CNMON3_C     0xE5
#define CNMON4_C     0xE6

    /*recommended values*/
#define ICISET_rmd   0x40
#define TCBADR_rmd   0x0
#define MDSET_C_rmd  0x18


#ifdef __cplusplus
}
#endif

#endif

/*============================================================================================
    Revision History

    $Log: mn88471_reg.h,v $
    Revision 1.1  2010/09/16 17:30:37  bells
    Initial check-in to PSDE CVS repository

==============================================================================================*/
