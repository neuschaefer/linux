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
 * $RCSfile: pd_tuner_type.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pd_tuner_type.h
 *  This file list all tuner type
 */

#ifndef PD_TUNER_TYPE_H
#define PD_TUNER_TYPE_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define cDTD_TUNER_TYPE_START   0x0
#define cPHILIPS_TD1336O        0
#define cPHILIPS_FQD1236        1
#define cPHILIPS_FQD1236E       2
#define cSAMSUNG_DTVS203        3
#define cALPS_TDEU2             4
#define cMAXLINEAR_MXL5005      5
#define cSAMSUNG_DTVS203EH      6
#define cTHOMSON_DTT76830       7
#define cSAMSUNG_DTVS227CH      8
#define cALPS_TDQU4X524A        9
#define cALPS_TDQU8             10
#define cALPS_TDQU6             11
//#define cNXP_TD1636             12
#define cSHARP_VA1Y2UF2312      13
#define cALPS_TDQU6X521A        14
#define cALPS_TDAU              15
#define cALPS_TDQU8K01A         16
//#define cNXP_UV1336             17    // move to cITUNER_VALID_START below
#define cLG_TDVFH052            18
#define cSHARP_VA1Y2UF2446      19
/*You need to modify valid end when add a new type*/
#define cDTD_VALID_TYPE_END     19
//#define cTUNER_ALL 		        0x48
//#define cDTD_TUNER_ALL 		    0x49

#define cDVBT_TUNER_TYPE_START  0x50
#define cSAMSUNG_DTOS40A    	0x50
#define cPANASONIC_ENG37E   	0x51
#define cALPS_TDQG4         	0x52
#define cPHILIPS_FQD1116ME  	0x53
#define cPHILIPS_TDM1316    	0x54
#define cSAMSUNG_DTOS40FP    	0x55
#define cPHILIPS_TD1318AF       0x56
#define cPANASONIC_ET55DHR      0x57
#define cTCL_F07WP              0x58
#define cLG_TDTWS710D           0x59
#define cLG_TDMB_G211D_DVBC_NIM 0x5A
#define cSILICON_XC5000         0x5B
#define cTCL_F05WT              0x5C
#define cPANASONIC_ENV57S   	0x5D
#define cNXP_FT2101            0x5e
#define cLG_TDTWS810D           0x5f
#define cLG_TDTWS815D           0x60
#define cNuTune_FT2607          0x61
#define cTCL_DT70WI             0x62
#define cXUGUANG_DTT8D1C        0x63
#define cPANASONIC_ENV5SDF   	  0x64
#define cLG_TAFTZ716D   	  0x65
/*You need to modify valid end when add a new type*/
#define cDVBT_VALID_TYPE_END    0x65
//#define cDVBT_TUNER_ALL         0x69

#define cITUNER_TYPE_START     0x70
#define cNXP_UV1356E            0x70
#define cNXP_UV1336             0x71
#define cNXP_UV1316E            0x72
#define cSHARP_VA1G5BF          0x73
#define cLG_TDVRH751F    	    0x74
#define cLG_TDTGG902D           0x75
#define cALPS_TDAG4B03A         0x76
//5387
#define cALPS_TDAU4XB02A            0x77
#define cALPS_TDQU8X001A            0x78
#define cLG_TDVWH810F               0x79
#define cNXP_TD1636                 0x7A
#define cALPS_TEQE3                 0x7B
#define cALPS_TDAU4D01A             0x7C
#define cALPS_TEQH3L01A             0x7D
#define cALPS_TDAU4D02A             0x7E
#define cPANASONIC_ENV56S           0x7F
#define cALPS_TEQE3_SUNBOARD        0x80
#define cALPS_TEQH3L01A_SUNBOARD    0x81
#define  cTCL_DA58CT                0x82
#define cSAMSUNG_DTVA50CVH          0x83
#define cSHARP_VA1E1BF              0x84
#define cNUTUNE_FA2307A             0x85
#define cTCL_F20WT                  0x86
 
#define cITUNER_TYPE_END            0xDF
//
//#define cITUNER_NUM             (cITUNER_VALID_END - cITUNER_VALID_START + 1)
//#define cITUNER_ALL             0x8F

#define cITUNER_DONT_CARE       0xE0

#define cTUNER_ALL_START  0xE1
#define cITUNER_ALL             0xE1
#define cDTD_TUNER_ALL     0xE2
#define cDVBT_TUNER_ALL         0xE3
#define cEMCS_TUNERLST          0xE4  // For SONY: ATSC, GA, Latin single image.
#define cDVBT_2K10_TUNERLST 0xE5   //FOR TPV Philips 2K10 project...
#define cTTE_TUNERLST          0xE6   //For TCL Tuner DA58CT and ENV56S
#define cTPV2K10_TUNERLST   0xE7   //For TPV_SHARP_ATSC Tuner TDVWH810F and ENV56S
#define cTTE_I2CALLLST     0XE8  //For TCL tuner DT70WI and env57s

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#endif /* PD_TUNER_TYPE_H */

