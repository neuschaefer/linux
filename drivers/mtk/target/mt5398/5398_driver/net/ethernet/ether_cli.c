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
/********************************************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
* 
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: ether_cli.c $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   Ethernet CLI unit test and debug source file
 *
 * Author:
 * -------
 *   Hs Huang
 *
 * Last changed:
 * -------------
 * $Author:
 *
 * $Modtime:
 *
 * $Revision:
****************************************************************************/

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "x_stl_lib.h"

#include "ether_com.h"
#include "ether_oid.h"
#include "ether_cli.h"
#include "ether_if.h"
#include "ether_drv.h"
#include "chksum.h"
#include "ether_reg.h"
//#ifdef __linux__
//#include "net_cmd_native.h"
//#endif

LINT_EXT_HEADER_END

BOOL fgDrvCliInit= FALSE;
UINT8 _TxPacketType=0;

//THe following define is for _TxPacketType
#define DEFAULT_PACKET 0
#define UDP_WRONG_CHKSUM_PACKET 1
#define TCP_WRONG_CHKSUM_PACKET 2
#define UDP_NORMAL_PACKET 3
#define TCP_NORMAL_PACKET 4
#define CUSTOM_PACKET 5

/****************************************************************************
** Local definitions
****************************************************************************/

/****************************************************************************
** Local structures and enumerations
****************************************************************************/

/****************************************************************************
** Constant Data Declaration
****************************************************************************/
/* Ping:    rc(192.168.1.231),         Dst(192.168.1.230) 
            rc Mac(00:08:a1:93:73:6e), Dst Mac(00:0a:79:5c:08:5c)	*/
#if 1            
static const UINT8 ETH_PING_COREGA[] = {
//0x00, 0x0a, 0x79, 0x5c, 0x08, 0x5c, 0x00, 0x08, 0xa1, 0x93, 0x73, 0x6e, 0x08, 0x00, 0x45, 0x00,
0x00, 0x0a, 0x79, 0x5c, 0x08, 0x5c, 0x00, 0x08, 0xa1, 0x93, 0x73, 0x6e, 0x00, 0x4a, 0x45, 0x00,
0x00, 0x3c, 0x1d, 0x4d, 0x00, 0x00, 0x80, 0x01, 0x98, 0x56, 0xc0, 0xa8, 0x01, 0xe7, 0xc0, 0xa8,
0x01, 0xe6, 0x08, 0x00, 0x43, 0x5c, 0x02, 0x00, 0x08, 0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69
};
#endif

#if 0            
/* IP packet with correct ip checksum. This pattern can be used to test the Auto TX SA learning. */
static const UINT8 ETH_PING_COREGA[] = {
0x00, 0x11, 0x00, 0x33, 0x65, 0x84, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x08, 0x00, 0x45, 0x00,
0x00, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x40, 0x72, 0xEC, 0x34, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13,
0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

#if 0            
//VLAN tag 0x81 0x00 0x51 0x01
static const UINT8 ETH_PING_COREGA[] = {
//0x00, 0x0a, 0x79, 0x5c, 0x08, 0x5c, 0x00, 0x08, 0xa1, 0x93, 0x73, 0x6e, 0x08, 0x00, 0x45, 0x00,
0x00, 0x0a, 0x79, 0x5c, 0x08, 0x5c, 0x00, 0x08, 0xa1, 0x93, 0x73, 0x6e, 0x81, 0x00, 0x51, 0x01, 0x00, 0x4a, 0x45, 0x00,
0x00, 0x3c, 0x1d, 0x4d, 0x00, 0x00, 0x80, 0x01, 0x98, 0x56, 0xc0, 0xa8, 0x01, 0xe7, 0xc0, 0xa8,
0x01, 0xe6, 0x08, 0x00, 0x43, 0x5c, 0x02, 0x00, 0x08, 0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69
};
#endif

#if 0
//TCP VLAN packet, IP header checksum = 0xEC9C, TCP checksum = 0x9313 , length = 72 bytes
static const UINT8 ETH_PING_COREGA[] = {
0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x01, 0x23,0x08, 0x00, 0x45, 0x00,
0x00, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x40, 0x06, 0xEC, 0x94, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13, 
0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x00, 0x01, 0xE2, 0x40, 0x00, 0x03, 0x94, 0x47, 0x50, 0x10, 
0x10, 0x00, 0x93, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

#if 0
//TCP packet, IP header checksum = 0xEC9C, TCP checksum = 0x9313 , length = 72 bytes
static const UINT8 ETH_PING_COREGA[] = {
0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x45, 0x00,
0x00, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x40, 0x06, 0xEC, 0x94, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13, 
0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x00, 0x01, 0xE2, 0x40, 0x00, 0x03, 0x94, 0x47, 0x50, 0x10, 
0x10, 0x00, 0x93, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

#if 0
//(SNAP + TCP) packet, IP header checksum = 0xEC9C, TCP checksum = 0x9313 , length = 72 bytes
static const UINT8 ETH_PING_COREGA[] = {
0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3A, 0xAA, 0xAA, 
0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x45, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x00, 0x40, 0x06, 
0xEC, 0x9C, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13, 0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x00, 0x01, 
0xE2, 0x40, 0x00, 0x03, 0x94, 0x47, 0x50, 0x10, 0x10, 0x00, 0x93, 0x13, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif
#if 0
//UDP packet, IP header checksum = 0xEC6D, TCP checksum = 0x5800 , length = 100 bytes
static const UINT8 ETH_PING_COREGA[] = {
0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x08, 0x00, 0x45, 0x00,
0x00, 0x56, 0x00, 0x00, 0x00, 0x00, 0x40, 0x11, 0xEC, 0x6D, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13, 
0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x00, 0x42, 0x58, 0x00, 0x00, 0x01, 0x00, 0x12, 0x00, 0x23,
0x00, 0x09, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00
};
#endif

//UDP packet, wrong checksum IP header checksum = 0xE8e9(correct),  length = 1000 bytes, IP length=0x03da, UDP checksum=0x6237(correct)
//it can be for verrify Iem46
#if 0
static const UINT8 ETH_PING_COREGA[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x45, 0x00,
0x03, 0xda/*Ip len*/, 0x00, 0x00, 0x00, 0x00, 0x40, 0x11, 0xE8, 0x00/*IP Chksum e8e9*/, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13,
0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x03, 0xc6, 0x62, 0x00/*chksum 6237*/, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif


//UDP packet, IP header checksum = 0xE8e9,  length = 1000 bytes, IP length=0x03da, UDP checksum=0x6237
//it can be for verrify Iem46
#if 0
static const UINT8 ETH_PING_COREGA[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x45, 0x00,
0x03, 0xda/*Ip len*/, 0x00, 0x00, 0x00, 0x00, 0x40, 0x11, 0xE8, 0xe9/*IP Chksum e8e9*/, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13,
0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x03, 0xc6, 0x62, 0x37/*chksum*/, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif


//TCP packet, IP header checksum = 0xE8F4(correct),  length = 1000 bytes, IP length=0x03da, UDP checksum=0x8f6b(correct)
//for verrify Iem46
#if 0//fill wrong checksum
static const UINT8 ETH_PING_COREGA[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x45, 0x00,
0x03, 0xda/*Ip len*/, 0x00, 0x00, 0x00, 0x00, 0x40, 0x06, 0xE8, 0x00/*IP* Chksum E8F4*/, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13,
0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x00, 0x01, 0xe2, 0x40, 0x00, 0x03, 0x94, 0x47, 0x50, 0x10,
0x10, 0x00, 0x8f, 0x00/*TCP chksum 0x8f6b*/, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif


//UDP packet VLAN=81000003 (VID=03), IP header checksum = 0xE8e9,  length = 1000 bytes, IP length=0x03da, UDP checksum=0x6237
//for verrify Iem46
#if 0 //fill wrong checksum
static const UINT8 ETH_PING_COREGA[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81, 0x00, 0x00, 0x03, 
0x08, 0x00, 0x45, 0x00, 0x03, 0xda/*Ip len*/, 0x00, 0x00, 0x00, 0x00, 0x40, 0x11, 0xE8, 0x00/*IP Chksum e8e9*/, 0xC6, 0x13, 
0x01, 0x02, 0xC6, 0x13, 0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x03, 0xc6, 0x62, 0x00/*UDP chksum 6237*/, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

#if 0//Pause Frame
static const UINT8 ETH_PING_COREGA[] = {
0x01, 0x80, 0xc2, 0x00, 0x00, 0x01, 0x00, 0x0c, 0xe7, 0x06, 0x00, 0x00, 0x88, 0x08, 0x00, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

//UDP packet, IP header checksum = 0xEC89, UDP checksum = 0x6977 , length = 72 bytes
//for verrify Iem24
//kenny 2010/3/1
static const UINT8 ETH_UDP_WRONG_CHKSUM[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x45, 0x00,
0x00, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x40, 0x11/*UDP*/, 0xEC, 0x95/*0x89*/, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13,
0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x00, 0x26, 0x69, /*0x77,*/ 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};



//TCP packet, IP header checksum = 0xEC94, TCP checksum = 0x930b , length = 72 bytes
//for verrify Iem24
//kenny 2010/3/1
static const UINT8 ETH_TCP_WRONG_CHKSUM[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x45, 0x00,
0x00, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x40, 0x06/*TCP*/, 0xEC, 0x95/*0x94*/, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13,
0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x00, 0x01, 0xe2, 0x40, 0x00, 0x03, 0x94, 0x47, 0x50, 0x10, 
0x10, 0x00, 0x93, 0x0A/*0x0B*/, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


//UDP packet, IP header checksum = 0xEC89, UDP checksum = 0x6977 , length = 72 bytes
//for verrify Iem24
//kenny 2010/3/1
static const UINT8 ETH_UDP_CORRECT_CHKSUM[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x45, 0x00,
0x00, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x40, 0x11/*UDP*/, 0xEC, 0x89, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13,
0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x00, 0x26, 0x69, 0x77,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};



//TCP packet, IP header checksum = 0xEC94, TCP checksum = 0x930b , length = 72 bytes
//for verrify Iem24
//kenny 2010/3/1
static const UINT8 ETH_TCP_CORRECT_CHKSUM[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x45, 0x00,
0x00, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x40, 0x06/*TCP*/, 0xEC, 0x94, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13,
0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x00, 0x01, 0xe2, 0x40, 0x00, 0x03, 0x94, 0x47, 0x50, 0x10, 
0x10, 0x00, 0x93, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


#if 0
//(SNAP + UDP) packet, IP header checksum = 0xEC75, TCP checksum = 0x5810 , length = 100 bytes
static const UINT8 ETH_PING_COREGA[] = {
0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x56, 0xAA, 0xAA, 
0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x45, 0x00, 0x00, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x40, 0x11,
0xEC, 0x75, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13, 0x01, 0x01, 0x04, 0x00, 0x04, 0x01, 0x00, 0x3A,
0x58, 0x10, 0x00, 0x01, 0x00, 0x12, 0x00, 0x23, 0x00, 0x09, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00
};
#endif

#if 0
/* for item 24: Tx IP/TCP/UDP checksum offload Test (AT)
    IP header len=40, checksum=0xe734 (correct), total len = 60
*/
static const UINT8 ETH_PING_COREGA[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x4A, 0x00, 
0x00, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x40, 0x72, 0xE7, /*0x34*/0x00, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13,
0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

#if 0
/* for item 24: Tx IP/TCP/UDP checksum offload Test (AT)
    IP header len=40, checksum=0xe72c (correct), total len = 68
*/
static const UINT8 ETH_PING_COREGA[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x4A, 0x00,
0x00, 0x36, 0x00, 0x00, 0x00, 0x00, 0x40, 0x72, 0xE7, /*0x2C*/0x00, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13,
0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00
};
#endif

#if 0
/* for item 24: Tx IP/TCP/UDP checksum offload Test (AT)
    IP header len=40, checksum=0xe728 (correct), total len = 72
*/
static const UINT8 ETH_PING_COREGA[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x4A, 0x00,
0x00, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x40, 0x72, 0xE7, /*0x28*/0x00, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13,
0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif


#if 0
/* for item 24: Tx IP/TCP/UDP checksum offload Test (AT)
    IP header len=40, checksum=0xe720 (correct), total len = 80
*/
static const UINT8 ETH_PING_COREGA[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x4A, 0x00,
0x00, 0x42, 0x00, 0x00, 0x00, 0x00, 0x40, 0x72, 0xE7, /*0x20*/0x00, 0xC6, 0x13, 0x01, 0x02, 0xC6, 0x13,
0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif


#if 0
static const MAC_ADDRESS_T _aEtherMacAddr[] = {
  {0x00, 0x0c, 0xe7, 0x06, 0x00, 0x0b},     /* 0, this mac can only be used for HS */
  {0x00, 0x0c, 0xe7, 0x00, 0x00, 0x28},		/* 1 */
  {0x00, 0x08, 0xa1, 0x93, 0x73, 0x6e},		/* 2 */
  {0x66, 0x55, 0x44, 0x33, 0x22, 0x11},		/* 3 */
  {0x11, 0x22, 0x33, 0x44, 0x55, 0x66}    	/* 4 */
};	
#endif

static UINT8 _eth_custom_packet[PKT_BUFFER_SIZE] = { 0 };
static UINT32 _eth_custom_packet_len = 60;
    
/* Wakeup frame filter testing sample:
   1.SmartBit pattern generator sends packet with payload (00:01:02:03:04:05) repeatly 20 times 
   2.Length type(LT) field must <= 0x0600, otherwise, wake-up frame bit will not be triggered */ 
static const ETHER_WAKEUP_FRAME_T _aWakeupFilterReg = {
  0x7fc0fdf8,       /* reg0, filter 0 byte mask */
  0x71f5fcf8,       /* reg1, filter 1 byte mask */
  0x76ff4f1f,       /* reg2, filter 2 byte mask */
  0x7fffffff,       /* reg3, filter 3 byte mask */
  0x01010101,       /* reg4, command */
  0x0e11130f,       /* reg5, offset */
  0x2328170f,       /* reg6, filter 0,1 CRC-16 */
  0xd72af8a8        /* reg7, filter 2,3 CRC-16 */
};	


static const UINT32 _u4RandomPacketSize[] = {
1024,
1085,
64,
1498,
1000,
100,
987,
1403,
70,
1301,
1107,
90
};

/****************************************************************************
** Function prototypes
****************************************************************************/

/****************************************************************************
** Local variable
****************************************************************************/
static NET_DRV_IF_T _rEtherCliDev;
static UINT32 _u4TxCount, _u4SuccessCnt, _u4LossRate , _u4FailCnt;
static HAL_TIME_T _rEtherTimeSum;
static BOOL _fgSelfTestEnd, _fgCompare = 0;
static UINT8 *_pucTxBuf;
static UINT32 _u4RxCnt = 0;
static BOOL _fgSltInit = 0;
#ifdef DRV_LOOP_BACK_TEST
  //PKT_BUFF_T *_prLoopTxPktBuff[64];
  //UINT8 *_pucLoopTxBuf[64];
  static PKT_BUFF_T *_prLoopTxPktBuff;
  static UINT8 *_pucLoopTxBuf;
  
  static UINT8 _u1LoopTxWtPktInd = 0;
  static UINT8 _u1LoopTxRdPktInd = 0;
  
#endif
static INT32 _fgEthBurnInStarted = 0;
static HANDLE_T _hEthBurnInTask = 0;

/****************************************************************************
** Global variable
****************************************************************************/
HAL_TIME_T _rEtherTimeStart;
HAL_TIME_T _rEtherTimeEnd;
HAL_TIME_T _rEtherTimeDelta;

/****************************************************************************
** External functions
****************************************************************************/
#ifdef DRV_LOOP_BACK_TEST
void vCreateLoopTxBuff(void)
{
   #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
        _prLoopTxPktBuff = (PKT_BUFF_T *)VIRTUAL(x_alloc_dma_mem(sizeof(PKT_BUFF_T)*NUM_RX_DESC));
   #else
       _prLoopTxPktBuff = (PKT_BUFF_T *)x_mem_alloc(sizeof(PKT_BUFF_T)*NUM_RX_DESC);
   #endif

   if(_prLoopTxPktBuff == NULL)
   {
     ERR_ETHER("no memory for Tx packet buffer !\n");
     return (NET_DRV_ERROR);
   }

   #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    _pucLoopTxBuf = (UINT8 *)VIRTUAL(x_alloc_dma_mem(PKT_BUFFER_SIZE*NUM_RX_DESC));
   #else
   _pucLoopTxBuf = (UINT8 *)x_mem_alloc(PKT_BUFFER_SIZE*NUM_RX_DESC);
   #endif

    if(_pucLoopTxBuf== NULL)
    {
        ERR_ETHER("no memory for Tx buffer !\n");
        return (NET_DRV_ERROR);
    }


}

#endif
/*******************************************************************************
**  _i4EtherInit
**
**  descriptions:
**      This rotinue is called to initialize ethernet driver 
**  parameters:
**      none
**  result:
**  return:
**      0 if success,
**      negative value if fail
**  note:
*******************************************************************************/
INT32 _i4EtherInit(void)
{
    NET_DRV_IF_T *prDev;
    INT32 i4Ret;

#ifdef __KERNEL__
    /* Before starting Generic driver, we may need to check if
       ethernet driver is started in native kernel. If yes, we
       will need to shutdown the interface first.
     */
    volatile UINT32 u4Data = 0;
    
    if(fgDrvCliInit == TRUE)
    return 0;
    
    i4Native_EthIfUpDown(0);
    IO_WRITE32(0xf000d000, 0x270, 0x9FFFFFFF);
    x_thread_delay(50);
    IO_WRITE32(0xf000d000, 0x270, 0xFFFFFFFF);
    /* also try to reset DMA */
    //u4Data = dReadMAC(RW_SW_RESET_CONTROL);
    //u4Data &= ~HRST;
    //vWriteMAC(RW_SW_RESET_CONTROL, u4Data);
    //u4Data |= HRST;
    //vWriteMAC(RW_SW_RESET_CONTROL, u4Data);
#endif /* __linux__ */

    prDev = (NET_DRV_IF_T *)&_rEtherCliDev;
    i4Ret = i4EtherDrv_Init(prDev);
    if(i4Ret != NET_DRV_OK)
    {
        ERR_ETHER("ethernet driver init fail\n");
    }

    /* start directly */
    i4Ret = i4EtherDrv_Start(prDev);

    /* fill wakeup frame filter register */
    vHalWakeupFrameFilterSet((ETHER_WAKEUP_FRAME_T *)_aWakeupFilterReg);
#ifdef DRV_LOOP_BACK_TEST   
    vCreateLoopTxBuff();
#endif
    fgDrvCliInit = TRUE;
    return(i4Ret);
}

/*******************************************************************************
**  _i4EtherDeInit
**
**  descriptions:
**      This rotinue is called to de-initialize ethernet driver 
**  parameters:
**      none
**  result:
**  return:
**      0 if success,
**      negative value if fail
**  note:
*******************************************************************************/
INT32 _i4EtherDeInit(void)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    INT32 i4Ret;
	
    i4Ret = i4EtherDrv_DeInit(prDev);
    if(i4Ret != NET_DRV_OK)
    {
        ERR_ETHER("ethernet driver deinit fail\n");
    }
    return(i4Ret);
}

/*******************************************************************************
**  _i4EtherStart
**
**  descriptions:
**      This rotinue is called to start(resume) ethernet driver 
**  parameters:
**      none
**  result:
**  return:
**      0 if success,
**      negative value if fail
**  note:
*******************************************************************************/
INT32 _i4EtherStart(void)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    INT32 i4Ret;
	
    i4Ret = i4EtherDrv_Start(prDev);
    if(i4Ret != NET_DRV_OK)
    {
        ERR_ETHER("ethernet driver start(resume) fail\n");
    }
    return(i4Ret);
}

/*******************************************************************************
**  _i4EtherStop
**
**  descriptions:
**      This rotinue is called to stop(suspend) ethernet driver 
**  parameters:
**      none
**  result:
**  return:
**      0 if success,
**      negative value if fail
**  note:
*******************************************************************************/
INT32 _i4EtherStop(void)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    INT32 i4Ret;
	
    i4Ret = i4EtherDrv_Stop(prDev);
    if(i4Ret != NET_DRV_OK)
    {
        ERR_ETHER("ethernet driver stop(suspend) fail\n");
    }
    return(i4Ret);
}

/*******************************************************************************
**  _i4EtherReadPhy
**
**  descriptions:
**      This rotinue is called to read PHY register through MII
**  parameters:
**      u4RegAddr - PHY register offset
**  result:
**  return:
**      0 if success,
**      negative value if fail
*******************************************************************************/
INT32 _i4EtherReadPhy(UINT32 u4RegAddr,UINT32 u4Len)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    PETHER_ADAPTER_T prMac = prDev->pv_priv;
    INT32 i4Ret = NULL;
    UINT32 u4Data;
    UINT32 i;
    
    for(i=0;i<u4Len;i++)
    {
        i4Ret = i4MacPhyRead(prMac->u4PhyAddr, u4RegAddr+i, &u4Data);
        if(i4Ret == NET_DRV_OK)
        {
            NOTE_ETHER("0x%04x ", u4Data);
        }
        else
        {
            NOTE_ETHER("\n");
            break;
        }
        if((i%4)==3) NOTE_ETHER("\n");
    }
    return(i4Ret);
}

/*******************************************************************************
**  _i4EtherWritePhy
**
**  descriptions:
**      This rotinue is called to write PHY register through MII
**  parameters:
**      u4RegAddr - PHY register offset
**      u4RegData - register value wrote to PHY
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
INT32 _i4EtherWritePhy(UINT32 u4RegAddr, UINT32 u4RegData)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    PETHER_ADAPTER_T prMac = prDev->pv_priv;
    INT32 i4Ret;

    i4Ret = i4MacPhyWrite(prMac->u4PhyAddr, u4RegAddr, u4RegData);
	
    return(i4Ret);
}

/*******************************************************************************
**  _i4EtherPhyANTest
**
**  descriptions:
**      This rotinue is called to test PHY AN 
**  parameters:
**      N/A
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
INT32 _i4EtherPhyANTest(void)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    PETHER_ADAPTER_T prMac = prDev->pv_priv;
    INT32 i4Ret = 0,i, j;
    UINT32 u4Val;
    CHAR aArgv[100] = {0}, **pArgv;
    UINT16 advTable[15]={
    	0x0021, 0x0121, 0x00A1, 0x01a1,
	0x0061, 0x0161, 0x00e1, 0x01e1,
	0x0101, 0x0081, 0x0181, 0x0041, 
	0x0141, 0x00c1, 0x01c1
	};

     for(i = 0; i <= 14; i++)
     {
        j = 0;
        i4MacPhyWrite(prMac->u4PhyAddr, MII_PHY_ANAR_REG, advTable[i]); //change adv capa
        i4MacPhyRead(prMac->u4PhyAddr, MII_PHY_CTRL_REG, &u4Val); //AN restart
        u4Val |= RESTART_AN | AN_ENABLE;
        i4MacPhyWrite(prMac->u4PhyAddr, MII_PHY_CTRL_REG, u4Val);

	 //wait until link up
        x_thread_delay(5000);
	 while (!GET_ETHER_INFO(prMac, ETHER_LINK_UP))
   	{
            x_thread_delay(50);
            j++;
            if (j >= 100)
            	{
           	   INFO_ETHER("no link after 5 sec !\n");
                break;
            	}
 	}
	 if (GET_ETHER_INFO(prMac, ETHER_LINK_UP))
	 {
           INFO_ETHER("send pkt when adv = %x\n", advTable[i]);
           pArgv = (char**)&aArgv;
           pArgv[1] = &aArgv[50];
           pArgv[2] = &aArgv[60];
           pArgv[3] = &aArgv[70];
           pArgv[4] = &aArgv[80];
    
           x_strncpy(pArgv[1], "1500", 4);    // data size
           x_strncpy(pArgv[2], "1", 3);     // count
           x_strncpy(pArgv[3], "1", 1);       // delay(ms)
           x_strncpy(pArgv[4], "0", 1);       // compare or not
           i4Ret = _i4EtherSendFrame(5, (const char**)pArgv);
           x_thread_delay(1000);
	 }
     }

    return(i4Ret);
}



/*******************************************************************************
**  
*******************************************************************************/
INT32 _i4EtherTxFreeCallback(NET_DRV_IF_T *prDev, PKT_BUFF_T *prPkt)
{
 

#ifdef DRV_LOOP_BACK_TEST

  _u1LoopTxRdPktInd++;
if(_u1LoopTxRdPktInd > NUM_RX_DESC)
_u1LoopTxRdPktInd=0;

#else
    ASSERT(prPkt != NULL);    
    #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    x_free_dma_mem(prPkt);
    #else
    x_mem_free(prPkt);
    #endif 

#endif

    return(0);
}

/*******************************************************************************
**  _i4EtherSetLoopBack
**
**  descriptions:
**      This rotinue is called set ethernet as loopback mode
**  parameters:
**      i4Argc - user input string count
**      szArgv - user input string  
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
INT32 _i4EtherSetLoopBack(INT32 i4Enable)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    PETHER_ADAPTER_T prMac = prDev->pv_priv;
    INT32 i4Ret;

    i4Ret = i4MacPhyLoopback(prMac->u4PhyAddr, i4Enable);
    return(i4Ret);
}

/*******************************************************************************
**  _i4EtherPkgCustom
**
**  descriptions:
**      This rotinue is called set custom packet
**  parameters:
**      i4Argc - user input string count
**      szArgv - user input string  
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
INT32 _i4EtherPktAdd(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i, u4Offset;
    UINT32 u4PktCount;

    do {
        if (i4Argc < 2)
        {
            NOTE_ETHER("pkta offset byte_array\n");
            break;
        }
        u4PktCount = i4Argc-2;
        u4Offset = StrToInt(szArgv[1]);
        if (u4Offset + u4PktCount > PKT_BUFFER_SIZE)
        {
            NOTE_ETHER("offset + data_length is large than MAX_PKT_SIZE %d\n", PKT_BUFFER_SIZE);
            break;
        }
        for (i=0; i<u4PktCount && i<PKT_BUFFER_SIZE; i++)
        {
            _eth_custom_packet[i+u4Offset] = (UINT8)StrToInt(szArgv[2+i]);
        }
        if (_eth_custom_packet_len < i+u4Offset)
            _eth_custom_packet_len = i+u4Offset;
    } while (0);
    NOTE_ETHER("Custom Packet:\n");
    NOTE_ETHER("Addr:   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    NOTE_ETHER("======================================================\n");
    for (i=0; i<_eth_custom_packet_len; i++)
    {
        if ((i%16) == 0)
        {
            NOTE_ETHER("%04X: ", i);
        }
        NOTE_ETHER(" %02X", _eth_custom_packet[i]);
        if ((i%16) == 0x0F)
        {
            NOTE_ETHER("\n");
        }
    }
    return NET_DRV_OK;
}
/*******************************************************************************
**  _i4EtherSendFrame
**
**  descriptions:
**      This rotinue is called send ethernet frame
**  parameters:
**      i4Argc - user input string count
**      szArgv - user input string  
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
INT32 _i4EtherSendFrame(INT32 i4Argc, const CHAR ** szArgv)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    PKT_BUFF_T *prPktBuff;
    UINT32 u4Period = 0, i, j=0;
    UINT32 u4Len = 0;
    INT32 i4Ret = 0;
    UINT8 fgRandomSize=0;
        
    ASSERT(szArgv != NULL);

    x_memset(&_rEtherTimeSum, 0x00, sizeof(HAL_TIME_T));

   #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
   
   #if (ALIGNMENT_16BYTES_TX_BUFF_TEST)
     _pucTxBuf = (UINT8 *)VIRTUAL(x_alloc_aligned_dma_mem(PKT_BUFFER_SIZE, 128));//kenny test 
   #else
     _pucTxBuf = (UINT8 *)VIRTUAL(x_alloc_dma_mem(PKT_BUFFER_SIZE));
   #endif 
   
        
   #else
    _pucTxBuf = (UINT8 *)x_mem_alloc(PKT_BUFFER_SIZE);
   #endif
   
    if(_pucTxBuf == NULL)
    {
        ERR_ETHER("no memory for Tx buffer !\n");
        return (NET_DRV_ERROR);
    }
    x_memset(_pucTxBuf, 0x00, PKT_BUFFER_SIZE);
    
    #if (ALIGNMENT_16BYTES_TX_BUFF_TEST)
    for(i =0; i< PKT_BUFFER_SIZE ; i++) //kenny add for test
    {
      _pucTxBuf[i] = i % 16;
      
    }
    #endif
    
   
    if(_TxPacketType == UDP_WRONG_CHKSUM_PACKET)	
    x_memcpy(_pucTxBuf, (UINT8 *)ETH_UDP_WRONG_CHKSUM, sizeof(ETH_UDP_WRONG_CHKSUM));
    else if(_TxPacketType == TCP_WRONG_CHKSUM_PACKET)	
    x_memcpy(_pucTxBuf, (UINT8 *)ETH_TCP_WRONG_CHKSUM, sizeof(ETH_TCP_WRONG_CHKSUM));
    else if(_TxPacketType == UDP_NORMAL_PACKET)	
    x_memcpy(_pucTxBuf, (UINT8 *)ETH_UDP_CORRECT_CHKSUM, sizeof(ETH_UDP_CORRECT_CHKSUM));
    else if(_TxPacketType == TCP_NORMAL_PACKET)	
    x_memcpy(_pucTxBuf, (UINT8 *)ETH_TCP_CORRECT_CHKSUM, sizeof(ETH_TCP_CORRECT_CHKSUM));
    else if(_TxPacketType == CUSTOM_PACKET)	
    x_memcpy(_pucTxBuf, (UINT8 *)_eth_custom_packet, _eth_custom_packet_len);
    else
    x_memcpy(_pucTxBuf, (UINT8 *)ETH_PING_COREGA, sizeof(ETH_PING_COREGA));


    if(i4Argc < 2)
    {
        _u4TxCount = 1;
        u4Period = 0;
        u4Len = sizeof(ETH_PING_COREGA);
        NOTE_ETHER("tx one packet @ %d byte\n", u4Len);
        NOTE_ETHER("Arg: byte, count, delay, compare\n");
    }
    else
    if(i4Argc == 2)
    {
        _u4TxCount = 0;
        if(x_strncmp(szArgv[1], "random", 6) == 0)
        {
            u4Len = sizeof(ETH_PING_COREGA);
            for(i = 0; i < (PKT_BUFFER_SIZE - u4Len); i+=u4Len)
            {
                x_memcpy((_pucTxBuf + i), (UINT8 *)ETH_PING_COREGA, u4Len);
            }
            u4Len = 1500;
            _u4TxCount = 10000;
            u4Period = 0;
            _fgCompare = 0;
        }
        else
        if(x_strncmp(szArgv[1], "ones", 4) == 0)
        {
            x_memset(_pucTxBuf, 0xff, PKT_BUFFER_SIZE);
            u4Len = 1500;
            _u4TxCount = 10000;
            u4Period = 0;
            _fgCompare = 0;
        }
        else
        {
            NOTE_ETHER("tx random: transmit random data\n");
            NOTE_ETHER("tx ones: transmit all ones data(0xff)\n");
        }
    }
    else
    if(i4Argc >= 4)
    {
        u4Len = (UINT32) StrToInt(szArgv[1]);
        _u4TxCount = (UINT32) StrToInt(szArgv[2]);
        u4Period = (UINT32) StrToInt(szArgv[3]);
        if(i4Argc == 5)
        {
            _fgCompare = (UINT32) StrToInt(szArgv[4]);
        }    
        else if(i4Argc == 6)
        {
            _fgCompare = (UINT32) StrToInt(szArgv[4]);
            fgRandomSize = (UINT32) StrToInt(szArgv[5]);
        }    
        else
        {
            _fgCompare = 0;
        }    
    }
    else
    {
        NOTE_ETHER("Arg: byte, count, delay, compare\n");
        _u4TxCount = 0;
    }

    _fgSelfTestEnd = 0;
    _u4SuccessCnt = 0;
    _u4LossRate = 100;
    _u4FailCnt = 0;
    for(i = 0; i < _u4TxCount; i++)
    {
       #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
        prPktBuff = (PKT_BUFF_T *)VIRTUAL(x_alloc_dma_mem(sizeof(PKT_BUFF_T)));
       #else
        prPktBuff = (PKT_BUFF_T *)x_mem_alloc(sizeof(PKT_BUFF_T));
       #endif
       
        if(prPktBuff == NULL)
    	{
        	ERR_ETHER("no memory for Tx packet buffer !\n");
        	return (NET_DRV_ERROR);
    	}
        prPktBuff->pui1_data = (UINT8 *)_pucTxBuf;
        if(fgRandomSize)
        {
            u4Len =  _u4RandomPacketSize[j]+(i%16);
            if(u4Len > 1514)
            u4Len = 1514;
            j++;

            if(j > sizeof(_u4RandomPacketSize)/4)
            j=0;
        }	
        
        prPktBuff->ui4_len = u4Len;
        prPktBuff->pf_pkt_free = (pkt_free_fnct)_i4EtherTxFreeCallback;

        if(i == (_u4TxCount - 1))
        {
            _fgSelfTestEnd = (BOOL)TRUE;
        }    
        x_thread_delay(u4Period);
        HAL_GetTime(&_rEtherTimeStart);
        //NOTE_ETHER("*");
        i4Ret = i4EtherDrv_Txmit(prDev, prPktBuff);
    }
     
    x_thread_delay(10);
    #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
      #if (ALIGNMENT_16BYTES_TX_BUFF_TEST)
        x_free_aligned_dma_mem(_pucTxBuf);
      #else
        x_free_dma_mem(_pucTxBuf);
      #endif
    #else
    x_mem_free(_pucTxBuf);
    #endif
    
    return(i4Ret);
}

/*******************************************************************************
**  _i4CliEthMacAddrSet
**
**  descriptions:
**      This rotinue is called to set MAC address and save it to flash
**  parameters:
**      pvArgv - 6 byte MAC address
**  return:
**      0 on success; others on failure
*******************************************************************************/
INT32 _i4CliEthMacAddrSet(void *pvArgv)
{
    INT32 i4Ret;

    ASSERT(pvArgv != NULL);

    //i4Ret = i4EtherDrv_Set(prDev, OID_CMN_IF_PHYADDRESS, (MAC_ADDRESS_T*)pvArgv);
    i4Ret = i4MiniCliSetMacAddr((MAC_ADDRESS_T*)pvArgv);
    return (i4Ret);
}

/*******************************************************************************
**  _i4CliEthMacAddrGet
**
**  descriptions:
**      This rotinue is called to get MAC address from flash
**  parameters:
**      pvArgv - 6 byte MAC address
**  return:
**      0 on success; others on failure
*******************************************************************************/
INT32 _i4CliEthMacAddrGet(void *pvArgv)
{
    INT32 i4Ret;

    ASSERT(pvArgv != NULL);

    //i4Ret = i4EtherDrv_Get(prDev, OID_CMN_IF_PHYADDRESS, (MAC_ADDRESS_T*)pvArgv);
    i4Ret = i4MiniCliGetMacAddr((MAC_ADDRESS_T*)pvArgv);
    return (i4Ret);
}

/*******************************************************************************
**  i4EtherIndicateFrame
**
**  descriptions:
**      This rotinue is called indicate received frame and calculate average throughput
**  parameters:
**      prDev - pointer to struct net_device
**      pvPktBuf - pointer to packet buffer
**  result:
**  return:
**      0 if success,
**      negative if free packet buffer fail
*******************************************************************************/
INT32 i4RxPacketCounter=0;
INT32 i4EtherIndicateFrame(NET_DRV_IF_T *prDev, PKT_BUFF_T *prPktBuf)
{
    UINT8 *pucBuf, *pucCmpPtr;
    BOOL fgCompareRet;
#ifdef DRV_LOOP_BACK_TEST
 INT32 i4Ret = 0;
UINT32 i;
UINT32 u4Len;
PKT_BUFF_T *prwTxPktBuf;
#endif
    
    ASSERT(prDev != NULL);
    ASSERT(prPktBuf != NULL);

	i4RxPacketCounter++;
	if(!(i4RxPacketCounter & 0xFFFFFF) )
	{
		NOTE_ETHER("RX counter = 0x%x \n",i4RxPacketCounter);
	}

    prPktBuf->ui2_refcnt = 1;
    if(_fgCompare)
    {
        /* compare tx with loopback rx data */
        pucCmpPtr = (UINT8 *)_pucTxBuf;
        pucBuf = prPktBuf->pui1_data;
        if(x_memcmp(pucCmpPtr, pucBuf, (UINT32)prPktBuf->ui4_len) == OSR_OK) 
        {
            fgCompareRet = TRUE;
        }
        else
        {
            fgCompareRet = FALSE;
        }
    }
    else
    {
        /* do not compare to show the real throughput */
        fgCompareRet = TRUE;
    }

#ifdef DRV_LOOP_BACK_TEST
   
     pucBuf = prPktBuf->pui1_data;
     
    x_memset(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE) , 0x00, PKT_BUFFER_SIZE);
    x_memcpy(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE) , (UINT8 *)pucBuf, (UINT32)prPktBuf->ui4_len); 
  #if 0
    Printf("Tx data = \n");  
    for(i=0; i< prPktBuf->ui4_len;i+=16)
    {
       Printf("0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x  0x%x\n",  
               
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE) +i), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE) +i+1), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+2), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+3), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+4), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+5), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+6), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+7), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+8), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+9), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+10), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+11), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+12), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+13), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+14), \
                *(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE )+i+15));
      
       

    }
   #endif
     u4Len=    (UINT32)prPktBuf->ui4_len;
     prwTxPktBuf =  (PKT_BUFF_T *)(_prLoopTxPktBuff+_u1LoopTxWtPktInd);
     prwTxPktBuf->pui1_data = (UINT8 *)(_pucLoopTxBuf+(_u1LoopTxWtPktInd*PKT_BUFFER_SIZE ));
     prwTxPktBuf->ui4_len = u4Len;
     prwTxPktBuf->pf_pkt_free = (pkt_free_fnct)_i4EtherTxFreeCallback;

       //Printf(" _prLoopTxPktBuff[%d]->pui1_data= 0x%x\n",  _u1LoopTxPktInd,  (UINT8 *)(_prLoopTxPktBuff->pui1_data));
       //Printf(" _prLoopTxPktBuff[%d]->ui4_len= 0x%x\n",  _u1LoopTxPktInd,  (UINT32)_prLoopTxPktBuff->ui4_len);
       //Printf(" _prLoopTxPktBuff[%d]->pui1_data= 0x%x\n",  _u1LoopTxPktInd,  _prLoopTxPktBuff[_u1LoopTxPktInd]->pui1_data);
           
       // HAL_GetTime(&_rEtherTimeStart);
        i4Ret = i4EtherDrv_Txmit(prDev, prwTxPktBuf);
        _u1LoopTxWtPktInd++;

        if(_u1LoopTxWtPktInd > NUM_RX_DESC)
        _u1LoopTxWtPktInd =0;
        

#endif//#ifdef DRV_LOOP_BACK_TEST


    /* free rx buffer by ourself when callback function is null */
    VERIFY(prPktBuf->pf_pkt_free(prDev, (PKT_BUFF_T*)prPktBuf) == NET_DRV_OK); 

    /* no transmission request */
    if (_u4TxCount == 0)
    {
        /* receive packet from outside instead of loopback */
        if((_u4RxCnt ++) > 10000)
        {
            _u4RxCnt = 0;
            //NOTE_ETHER("*");
        }    
        return(NET_DRV_OK);
    }
    
    if(fgCompareRet)
    {
        /* calculate the interval from send to receive packet */
        HAL_GetTime(&_rEtherTimeEnd);
        HAL_GetDeltaTime(&_rEtherTimeDelta, &_rEtherTimeStart, &_rEtherTimeEnd);
        _rEtherTimeSum.u4Seconds += _rEtherTimeDelta.u4Seconds;
        _rEtherTimeSum.u4Micros += _rEtherTimeDelta.u4Micros;
        if(_rEtherTimeSum.u4Micros >= 1000000)
        {
            _rEtherTimeSum.u4Micros -= 1000000;
            _rEtherTimeSum.u4Seconds ++;
        }
        _u4SuccessCnt ++;
        
        /* tx and rx frame loopback test is correct */
        //if (!_fgEthBurnInStarted) /* do not print notation '.' when burn-in test */
        //NOTE_ETHER(".");
    }
    else
    {
        /* tx and rx frame loopback test is in-correct */
        NOTE_ETHER("!");
        _u4FailCnt++;
    }

//    if(_fgSelfTestEnd && (_u4SuccessCnt != 0))
    if(_fgSelfTestEnd && (_u4SuccessCnt + _u4FailCnt == _u4TxCount)&& (_u4SuccessCnt != 0))
    {
        _rEtherTimeSum.u4Micros = (_rEtherTimeSum.u4Micros / _u4SuccessCnt);
        if(_rEtherTimeSum.u4Seconds > 0)
        {
            _rEtherTimeSum.u4Seconds = (_rEtherTimeSum.u4Seconds * 1000000) / _u4SuccessCnt;
            _rEtherTimeSum.u4Micros += (_rEtherTimeSum.u4Seconds % 1000000);
            _rEtherTimeSum.u4Seconds = _rEtherTimeSum.u4Seconds / 1000000;
            if(_rEtherTimeSum.u4Micros >= 1000000)
            {
                _rEtherTimeSum.u4Micros -= 1000000;
                _rEtherTimeSum.u4Seconds ++;
            }
        }

        /* loopback test is completed, show result and clear tx count */  
        // NOTE_ETHER("loopback throughput: %d.%06dsec @%d byte => %d.%02dMbps (%d\%% loss)\n", 
        NOTE_ETHER("\n");
        _u4LossRate = 100 * (_u4TxCount - _u4SuccessCnt) / _u4TxCount;
        NOTE_ETHER("loopback throughput: %d.%06dsec @%d byte => %d.%02dMbps (%d precent loss)\n", 
                    _rEtherTimeSum.u4Seconds, 
                    _rEtherTimeSum.u4Micros, 
                    prPktBuf->ui4_len, 
                    ((prPktBuf->ui4_len * 8) / _rEtherTimeSum.u4Micros),
                    ((prPktBuf->ui4_len * 8 * 100) / _rEtherTimeSum.u4Micros) % 100,
                    _u4LossRate);
        _u4TxCount = 0;
    }

    return(NET_DRV_OK);
}

/*******************************************************************************
**  _i4EtherMacAddr
**
**  descriptions:
**      This rotinue is called to set/get MAC address
**  parameters:
**      i4Argc - user input string count
**      szArgv - user input string  
**  result:
**  return:
**      0 on success, else on failure
**  note:
*******************************************************************************/
INT32 _i4EtherMacAddr(INT32 i4Argc, const CHAR ** szArgv)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    UCHAR aMac[6];  
//    MAC_ADDRESS_T aMac;
    INT32 i4Ret;
    NET_PHY_ADDRESS_T PhyAddr;
    
    ASSERT(szArgv != NULL);

    if(i4Argc > 1)
    {
        if(i4Argc != 2)
        {
            NOTE_ETHER("usage: mac xx:xx:xx:xx:xx:xx\n");
            return (0);
        }
        x_sscanf(szArgv[1], "%x:%x:%x:%x:%x:%x", &aMac[0],&aMac[1],&aMac[2],&aMac[3],&aMac[4],&aMac[5]);
        //i4Ret = i4EtherDrv_Set(prDev, OID_CMN_IF_PHYADDRESS, (MAC_ADDRESS_T*)aMac);
        i4Ret = i4MiniCliSetMacAddr((MAC_ADDRESS_T*)aMac);
    }
		
     i4Ret = i4EtherDrv_Get(prDev, OID_CMN_IF_PHYADDRESS, (NET_PHY_ADDRESS_T*)&PhyAddr);    

	return (i4Ret);
}

/*******************************************************************************
**  _i4EtherPromiscuous
**
**  descriptions:
**      This rotinue is called to set/get promiscuous mode
**  parameters:
**      i4Argc - user input string count
**      szArgv - user input string  
**  result:
**  return:
**      0 on success, else on failure
**  note:
*******************************************************************************/
INT32 _i4EtherPromiscuous(INT32 i4Argc, const CHAR ** szArgv)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    UINT32 u4Parm;
    INT32 i4Ret;

    ASSERT(szArgv != NULL);

    if(i4Argc > 1)
    {
        if(i4Argc != 2)
        {
            NOTE_ETHER("unknown command\n");
            return (0);
        }
        u4Parm = (UINT32)StrToInt(szArgv[1]);
        i4Ret = i4EtherDrv_Set(prDev, OID_CMN_IF_PROMISCUOUS_MODE, &u4Parm);
    }

    i4Ret = i4EtherDrv_Get(prDev, OID_CMN_IF_PROMISCUOUS_MODE, &u4Parm);
    if(u4Parm)
    {
        NOTE_ETHER("promiscuous mode is on\n");
    }
    else
    {
        NOTE_ETHER("promiscuous mode is off\n");
    }

    return (i4Ret);
}

/*******************************************************************************
**  _i4EtherMulticastList
**
**  descriptions:
**      This rotinue is called to set/get multicast address list
**  parameters:
**      i4Argc - user input string count
**      szArgv - user input string  
**  result:
**  return:
**      0 on success, else on failure
**  note:
*******************************************************************************/
INT32 _i4EtherMulticastList(INT32 i4Argc, const CHAR ** szArgv)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    NET_MAC_ADDRESS_LIST_T *prMcList;
    UCHAR aMac[6];   
    UINT32 u4ListSize, i;
    INT32 i4Ret;

    ASSERT(szArgv != NULL);

    /* allocate MAC_ADDRESS_T for the multicast list */
    u4ListSize = sizeof(NET_MAC_ADDRESS_LIST_T) +
    	         ((ETHER_MAX_MULTICAST_NUM - 1) * sizeof(MAC_ADDRESS_T));
    #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    prMcList = VIRTUAL(x_alloc_dma_mem(u4ListSize));
    #else
    prMcList = x_mem_alloc(u4ListSize);
    #endif
    
    if(prMcList == NULL)
    {
        ERR_ETHER("no memory for multicast address list\n");
        return (NET_DRV_ERROR);
    }	
    x_memset(prMcList, 0x00, u4ListSize);

    if(i4Argc > 1)
    {
        //NOTE_ETHER("usage: mc xx:xx:xx:xx:xx:xx xx:xx...\n");
    
        /* copy multicast list */
        prMcList->ui4_NumberOfItems = 0;
        for(i = i4Argc; i >= 2; i--)
    {
            x_sscanf(szArgv[prMcList->ui4_NumberOfItems + 1],
                     "%x:%x:%x:%x:%x:%x", 
                     &aMac[0],&aMac[1],&aMac[2],&aMac[3],&aMac[4],&aMac[5]);
            x_memcpy(&prMcList->t_MulticastList[prMcList->ui4_NumberOfItems],
                     aMac, 
                     sizeof(MAC_ADDRESS_T));
            prMcList->ui4_NumberOfItems ++;
        }

        //x_memcpy(prMcList->t_MulticastList, _aEtherMacAddr, (u4ListNum * sizeof(MAC_ADDRESS_T)));
        i4Ret = i4EtherDrv_Set(prDev, OID_CMN_IF_MULTICASTADDRESSLIST, (NET_MAC_ADDRESS_LIST_T*)prMcList);
        if(i4Ret != NET_DRV_OK)
        {
            ERR_ETHER("multicast address set fail\n");
        }
    }

    i4Ret = i4EtherDrv_Get(prDev, OID_CMN_IF_MULTICASTADDRESSLIST, (NET_MAC_ADDRESS_LIST_T*)prMcList);
        if(i4Ret != NET_DRV_OK)
        {
            ERR_ETHER("multicast address get fail\n");
        }
        
    if(prMcList->ui4_NumberOfItems == 0)
    {
        NOTE_ETHER("no multicast list\n");
    }
    else
    {
    	for(i = 0; i < prMcList->ui4_NumberOfItems; i++)
        {
            NOTE_ETHER("(%02x:%02x:%02x:%02x:%02x:%02x)\n", 
            	        ETH_MAC2STR(prMcList->t_MulticastList[i]));
        }
    }

 #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    x_free_dma_mem(prMcList);
 #else
    x_mem_free(prMcList);
 #endif
 
    return(i4Ret);
}

/*******************************************************************************
**  i4CliEthMulticastDelList
**
**  descriptions:
**      This rotinue is called to set/get multicast address list
**  parameters:
**      i4IoCmd - ioctl(OID) command
**      fgSet - TRUE for set and FALSE for get 
**      i4Argc - user input string count
**      szArgv - user input string  
**  result:
**  return:
**      0 if success,
**      negative value if fail
**  note:
*******************************************************************************/
static INT32 i4CliEthMulticastDelList(INT32 i4IoCmd, BOOL fgSet, INT32 i4Argc, const CHAR ** szArgv)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    NET_MAC_ADDRESS_LIST_T *prMcList;
    UCHAR aMac[6];   
    UINT32 u4ListSize, i;
    INT32 i4Ret=0;

    ASSERT(szArgv != NULL);

    /* allocate MAC_ADDRESS_T for the multicast list */
    u4ListSize = sizeof(NET_MAC_ADDRESS_LIST_T) +
                ((ETHER_MAX_MULTICAST_NUM - 1) * sizeof(MAC_ADDRESS_T));
    #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    prMcList = VIRTUAL(x_alloc_dma_mem(u4ListSize));
    #else
    prMcList = x_mem_alloc(u4ListSize);
    #endif
    
    if(prMcList == NULL)
    {
        ERR_ETHER("no memory for multicast address list\n");
        return (NET_DRV_ERROR);
    }
    x_memset(prMcList, 0x00, u4ListSize);

    if(fgSet)
    {
        if (i4Argc < 3)
        {
            NOTE_ETHER("usage: s 28 xx:xx:xx:xx:xx:xx\n");
            return (0);
        }
        
        /* copy multicast list */
        prMcList->ui4_NumberOfItems = 0;
        for(i = i4Argc; i >= 3; i--)
        {
            x_sscanf(szArgv[prMcList->ui4_NumberOfItems + 2],
                     "%x:%x:%x:%x:%x:%x", 
                     &aMac[0],&aMac[1],&aMac[2],&aMac[3],&aMac[4],&aMac[5]);
            x_memcpy(&prMcList->t_MulticastList[prMcList->ui4_NumberOfItems],
                     aMac, 
                     sizeof(MAC_ADDRESS_T));
            prMcList->ui4_NumberOfItems ++;
        }
    
        //x_memcpy(prMcList->t_MulticastList, _aEtherMacAddr[u4ListNum], (sizeof(MAC_ADDRESS_T)));
        i4Ret = i4EtherDrv_Set(prDev, i4IoCmd, (NET_MAC_ADDRESS_LIST_T*)prMcList);
        if(i4Ret != NET_DRV_OK)
        {
            ERR_ETHER("multicast address set fail\n");
        }
    }

#if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    x_free_dma_mem(prMcList);
#else
    x_mem_free(prMcList);
#endif 
    return(i4Ret);
}

/*******************************************************************************
**  _i4EtherStatInfo
**
**  descriptions:
**      This rotinue is called to get statistics information 
**  parameters:
**      i4Argc - user input string count
**      szArgv - user input string  
**  result:
**  return:
**      0 on success, else on failure
**  note:
*******************************************************************************/
INT32 _i4EtherStatInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    PETHER_ADAPTER_T prMac;
    PMAC_STAT_T prStat;

    ASSERT(szArgv != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
    prStat = (PMAC_STAT_T)&prMac->rStat;

    if(i4Argc > 1)
    {
        //if(x_strcmp("reset", szArgv[1]) == 0)
        if(StrToInt(szArgv[1]) == 0)
        {
            x_memset(prStat, 0x00, sizeof(MAC_STAT_T));
            NOTE_ETHER("all statistics are reset to 0\n");
        }
        else
        {
            NOTE_ETHER("unknown command\n");
        }
        return(0);
    }

    /* rx statistics */
    NOTE_ETHER("rx packet:                  %u\n", (UINT32)prStat->u8RxFrame);
    NOTE_ETHER("rx octet: (w/o CRC)         %u\n", (UINT32)prStat->u8RxOctet);
    NOTE_ETHER("rx pkt buffer: (free/total) %d/%d\n", prMac->rRxPktList.ui4_count, ETHER_RX_PKT_BUF_NUM);
    NOTE_ETHER("rx total error:             %d\n", prStat->u4RxErr);
    NOTE_ETHER("rx bit rate:                %d Kbps\n", prMac->rEthBitRate.u4RxBitRate);
    if(prStat->u4RxErr)
    {
    NOTE_ETHER("rx crc error:               %d\n", prStat->u4RxCrcErr);
    NOTE_ETHER("rx length error:            %d\n", prStat->u4RxLenErr);
    NOTE_ETHER("rx dribbled error:          %d\n", prStat->u4RxDribErr);
    NOTE_ETHER("rx late col error:          %d\n", prStat->u4RxLateColErr);
    }
    
    /* tx statistics */
    NOTE_ETHER("\n");
    NOTE_ETHER("tx packet:                  %d\n", prStat->u4TxFrame);
    NOTE_ETHER("tx octet: (w/o CRC)         %d\n", prStat->u4TxOctet);
    NOTE_ETHER("tx collision:               %d\n", prStat->u4TxColCnt);
    NOTE_ETHER("tx pkt needed to transmit:  %d\n", prMac->rTxPktList.ui4_count);
    NOTE_ETHER("tx total error:             %d\n", prStat->u4TxErr);
    NOTE_ETHER("tx bit rate:                %d Kbps\n", prMac->rEthBitRate.u4TxBitRate);
    if(prStat->u4TxErr)
    {
    NOTE_ETHER("tx carrier error:           %d\n", prStat->u4TxCarErr);
    NOTE_ETHER("tx excessive defer error:   %d\n", prStat->u4TxExcessDefer);
    NOTE_ETHER("tx excessive col error:     %d\n", prStat->u4TxExcessColErr);
    NOTE_ETHER("tx late col error:          %d\n", prStat->u4TxLateColErr);
    }

    return(0);
}

INT32 _i4EtherBitRateGet(UINT32 *pu4Parm)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    PETHER_ADAPTER_T prMac;

    ASSERT(pu4Parm != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;

    *pu4Parm = prMac->rEthBitRate.u4RxBitRate;
    return 0;
}

/*******************************************************************************
**  _i4EtherRunTimeTest
**
**  descriptions:
**      This rotinue is called to do NIC loopback test and reply result 
**  parameters:
**  return:
**      0
*******************************************************************************/
INT32 _i4EtherRunTimeTest(NET_DRV_IF_T *prDev, INT32 *pu4Result)
{
    PETHER_ADAPTER_T prMac = NULL;
    ni_indicate_fnct pf_ni_ind_tmp;
    CHAR aArgv[100] = {0}, **pArgv;
    UINT32 u4Data;
    INT32 i4Ret;
    
    ASSERT(prDev != NULL);
    ASSERT(pu4Result != NULL);

	prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
		
    /* enable promiscuous, enable PHY loopback */
    i4MacPhyRead(prMac->u4PhyAddr, MII_PHY_CTRL_REG, &u4Data);
    u4Data |= LOOKBACK_EN;
    i4Ret = i4MacPhyWrite(prMac->u4PhyAddr, MII_PHY_CTRL_REG, u4Data);
    if(i4Ret != NET_DRV_OK)
    {
        *pu4Result = NET_DRV_ERROR;
        ERR_ETHER("MII r/w fail\n");
        return (0);
    }
    /* delay for PHY stable */
    x_thread_delay(50);

    pf_ni_ind_tmp = prDev->pf_ni_indicate;
    prDev->pf_ni_indicate = NULL; 
    u4Data = TRUE;
    vHalSetPromiseMode(&u4Data);
   
    INFO_ETHER("NIC loopback test start\n");
    pArgv = (char**)&aArgv;
    pArgv[1] = &aArgv[40];
    pArgv[2] = &aArgv[50];
    pArgv[3] = &aArgv[60];
    pArgv[4] = &aArgv[70];
    x_strncpy(pArgv[1], "1500", 4);    // data size
    x_strncpy(pArgv[2], "500", 3);     // count
    x_strncpy(pArgv[3], "0", 1);       // delay(ms)
    x_strncpy(pArgv[4], "1", 1);       // compare or not
    _i4EtherSendFrame(5, (const char**)pArgv);
    /* SW tx speeed is faster than Ethernet engine, normally lost 3-5 packets */
    if(_u4LossRate <= 2)
    {
        *pu4Result = NET_DRV_OK;
        INFO_ETHER("NIC OK, received %d packets\n", _u4SuccessCnt);
    }
    else
    {
        *pu4Result = NET_DRV_ERROR;
        INFO_ETHER("NIC NG, %d percent loss\n", _u4LossRate);
    }

    /* disable promiscuous, disable PHY loopback, restart AN*/
    u4Data = FALSE;
    vHalSetPromiseMode(&u4Data);
    prDev->pf_ni_indicate = pf_ni_ind_tmp; 

    i4MacPhyRead(prMac->u4PhyAddr, MII_PHY_CTRL_REG, &u4Data);
    u4Data = (u4Data & ~LOOKBACK_EN) | AN_ENABLE | RESTART_AN;
    i4Ret = i4MacPhyWrite(prMac->u4PhyAddr, MII_PHY_CTRL_REG, u4Data);
    if(i4Ret != NET_DRV_OK)
    {
        *pu4Result = NET_DRV_ERROR;
        ERR_ETHER("MII r/w fail\n");
    }

    return (0);
}

/*******************************************************************************
**  _i4EtherSltLoopbackTest
**
**  descriptions:
**      This rotinue is called to set/get debug level 
**  parameters:
**  return:
*******************************************************************************/
INT32 _i4EtherSltLoopbackTest(INT32 i4Argc, const CHAR ** szArgv)
{
    NET_DRV_IF_T *prDev;
    PETHER_ADAPTER_T prMac;
    CHAR aArgv[100] = {0}, **pArgv;
    UINT32 u4Data;
    volatile UINT32 u4Val;
    INT32 i4Ret;
    INT32 maxDelayCnt = 500;
    PMAC_MII_T prMii;
    
    ASSERT(szArgv != NULL);

    _u4EtherDbgLevel = 0;
    if(!_fgSltInit)
    {
        _i4EtherInit();
        _fgSltInit = TRUE;
    }
    prDev = prEtherGetDev();
    prMac = prDev->pv_priv;
    prMii = &prMac->rMii;

    ERR_ETHER("Ethernet PHY addr = %d\n", (INT32)prMac->u4PhyAddr);
    
    
    /* delay for PHY stable */
    x_thread_delay(50);
    #if 0
    /* Enable auto crossover if internal PHY is used */
    i4MacPhyWrite(prMac->u4PhyAddr, 0x1f, 0x1); //change phy register switch
    i4MacPhyRead(prMac->u4PhyAddr, 0x1b, &u4Data); //read 0x1b register
    u4Data &= (UINT32)(~(0x1 << 15));
    i4MacPhyWrite(prMac->u4PhyAddr, 0x1b, u4Data); //enable auto crossover
    i4MacPhyWrite(prMac->u4PhyAddr, 0x1f, 0x0); //switch back phy register bank
    
    /* ---------- NOTE ----------- 
        When internal PHY is used (MT8550 and newer), we disable 
        PHY auto negotiation when running SLT. Since auto negotiation
        may take some time to complete (2~3 sec) and should be
        ignored. We force it as 100-full duplex mode.
     */
    #endif
    u4Val = dReadMAC(RW_PHY_CTRL1);
    u4Val &= ~AN_EN;
    u4Val |= (FORCE_DUPLEX_FULL|FORCE_SPEED_100M);
    vWriteMAC(RW_PHY_CTRL1, u4Val);
    

#if 0 /* modify this to change PHY loopback mode */
    /* set PHY as 100Mbps@full and enable loopback mode */
    /* we do not use PHY loopback when internal PHY is used, since
       the internal PHY has to be verified as well */
    i4Ret = i4MacPhyWrite(prMac->u4PhyAddr, 0x00, 0x6100);
    if(i4Ret != NET_DRV_OK)
    {
        ERR_ETHER("SLT NG: MII r/w fail\n");
    }
    Printf("Ethernet PHY loopback is enabled.\n");
#else
    i4Ret = i4MacPhyWrite(prMac->u4PhyAddr, 0x00, 0x2100);
    //i4Ret = i4MacPhyWrite(prMac->u4PhyAddr, 0x00, 0x3100);
    if(i4Ret != NET_DRV_OK)
    {
        ERR_ETHER("SLT NG: MII r/w fail\n");
    }
    Printf("Ethernet PHY loopback is NOT enabled.\n");
#endif
    /* wait until link is up */
    Printf("Checking for link status...\n");
    while (--maxDelayCnt > 0)
    {
        i4MacPhyRead(prMac->u4PhyAddr, 0x01, &u4Data);
        if (u4Data & 0x4)
            break;
        x_thread_delay(10);
    }
    if (!(u4Data & 0x4))
    {
        Printf("Error! Ethernet link is down.\n");
        return -1;
    }
    else
    {
        i4MacPhyRead(prMac->u4PhyAddr, 0x00, &u4Data);
        Printf("Ethernet link is up (%s duplex-%sMbps)\n",
              (u4Data&0x100 ? "Full":"Half"),
              ((u4Data&0x2000)? "100":"10"));
    }
    
    SET_ETHER_INFO(prMac, ETHER_LINK_UP);
    SET_ETHER_INFO(prMac, ETHER_DEV_RDY);
    prMii->fgSpeed100 = 1;
    prMii->fgFullDup = 1;

    u4Data = TRUE;
    vHalSetPromiseMode(&u4Data);
    
    _u4EtherDbgLevel = 9;
    Printf("Ethernet SLT start\n");
    pArgv = (char**)&aArgv;
    pArgv[1] = &aArgv[50];
    pArgv[2] = &aArgv[60];
    pArgv[3] = &aArgv[70];
    pArgv[4] = &aArgv[80];
    
#define SLT_PKT_SIZE    "1000"//"1500"
#define SLT_PKT_CNT     "200" //"200"
#define SLT_PKT_DELAY   "1"//"5"
    x_strncpy(pArgv[1], SLT_PKT_SIZE, x_strlen(SLT_PKT_SIZE));    // data size
    x_strncpy(pArgv[2], SLT_PKT_CNT, x_strlen(SLT_PKT_CNT));     // count
    x_strncpy(pArgv[3], SLT_PKT_DELAY, x_strlen(SLT_PKT_DELAY));       // delay(ms)
    x_strncpy(pArgv[4], "1", 1);       // compare or not
    i4Ret = _i4EtherSendFrame(5, (const char**)pArgv);

    x_thread_delay(10);
    /* SW tx speeed is faster than Ethernet engine, normally lost 3-5 packets */
    if(_u4LossRate == 0)
    {
        i4Ret = NET_DRV_OK;
        Printf("Ethernet SLT pass\n");
    }
    else
    {
        i4Ret = NET_DRV_ERROR;
        Printf("Ethernet SLT fail\n");
    }
    #if SLT_TEST_ETHERNET_EN
    if(i4Ret == NET_DRV_OK)
    {
        SLTSendResult(SLT_ITEMS_ETHERNET,SLT_STATUS_PASS);
    }
    else
    {
        SLTSendResult(SLT_ITEMS_ETHERNET,SLT_STATUS_FAIL);
    }
    #endif

    //_u4EtherDbgLevel = 1;
    _u4EtherDbgLevel = 9;
    return (i4Ret);
}



/*******************************************************************************
**  _i4EtherBurnInTask
**
**  descriptions:
**      Ethernet Burn-in PHY Loopback test main task.
**  parameters:
**  return:
*******************************************************************************/
static void _i4EtherBurnInTask(void *pv_arg)
{
    NET_DRV_IF_T *prDev;
    PETHER_ADAPTER_T prMac;
    CHAR aArgv[100] = {0}, **pArgv;
    UINT32 u4Data;
    volatile UINT32 u4Val;
    INT32 i4Ret;
    INT32 maxDelayCnt = 1000;
    PMAC_MII_T prMii;
    

    _fgEthBurnInStarted = 1;
    _u4EtherDbgLevel = 0;
    if(!_fgSltInit)
    {
        _i4EtherInit();
        _fgSltInit = TRUE;
    }
    prDev = prEtherGetDev();
    prMac = prDev->pv_priv;
    prMii = &prMac->rMii;

    /* delay for PHY stable */
    x_thread_delay(50);
    Printf("Ethernet PHY addr = %d\n", (INT32)prMac->u4PhyAddr);
#if 0
    Printf("Ethernet PHY loopback is enabled\n");
    while (1)
    {
        /* set PHY as 100Mbps@full and enable loopback mode */
        i4Ret = i4MacPhyWrite(prMac->u4PhyAddr, 0x00, 0x7100);
        if(i4Ret != NET_DRV_OK)
        {
            ERR_ETHER("ETH_NG: MII write fail\n");
        }
        /* confirm if the PHY loopback was set */
        if (i4MacPhyRead(prMac->u4PhyAddr, 0x00, &u4Data) != 0)
        {
            ERR_ETHER("ETH_NG: MII read fail\n");
        }
        if ((u4Data & 0x7100))
        {
            break;
        }
        x_thread_delay(10);
    }
#else
    Printf("Ethernet PHY loopback is disabled\n");
#endif
    /* ---------- NOTE ----------- 
        When internal PHY is used (MT8550 and newer), we disable 
        PHY auto negotiation when running SLT. Since auto negotiation
        may take some time to complete (2~3 sec) and should be
        ignored. We force it as 100-full duplex mode.
     */
    /*
    u4Val = dReadMAC(RW_PHY_CTRL1);
    u4Val &= ~AN_EN;
    u4Val |= (FORCE_DUPLEX_FULL|FORCE_SPEED_100M);
    vWriteMAC(RW_PHY_CTRL1, u4Val);
    */
    
    /* wait until link is up */
    Printf("Checking for link status...\n");
    while (!(dReadMAC(RW_PHY_CTRL1) & LINK_ST) && (--maxDelayCnt > 0))
    {
        x_thread_delay(10);
    }
    u4Val = dReadMAC(RW_PHY_CTRL1);
    if (!(u4Val & LINK_ST))
    {
        Printf("ETH_NG:Error! Ethernet link is down.\n");
        goto out;
    }
    else
    {
        Printf("Ethernet link is up (%s duplex-%sMbps)\n",
              (u4Val&DULPLEX_ST ? "Full":"Half"),
              ((u4Val&SPEED_ST) == SPEED_ST_100M? "100":"10"));
    }


    SET_ETHER_INFO(prMac, ETHER_LINK_UP);
    SET_ETHER_INFO(prMac, ETHER_DEV_RDY);
    prMii->fgSpeed100 = 1;
    prMii->fgFullDup = 1;

    u4Data = TRUE;
    vHalSetPromiseMode(&u4Data);
    
    _u4EtherDbgLevel = 9;
    Printf("Ethernet Burn-in start\n");
    pArgv = (char**)&aArgv;
    pArgv[1] = &aArgv[50];
    pArgv[2] = &aArgv[60];
    pArgv[3] = &aArgv[70];
    pArgv[4] = &aArgv[80];

    /* Note: Modify BURN_IN_PKT_CNT to adjust the test time.
                12,000,000 pkts with 10ms interval 
                = total test time 40 hours 
     */
#define BURN_IN_PKT_SIZE    "1000"
#define BURN_IN_PKT_CNT     "12000000"
#define BURN_IN_PKT_DELAY   "10"

    x_strncpy(pArgv[1], BURN_IN_PKT_SIZE, x_strlen(BURN_IN_PKT_SIZE)); // data size
    x_strncpy(pArgv[2], BURN_IN_PKT_CNT, x_strlen(BURN_IN_PKT_CNT)); // count
    x_strncpy(pArgv[3], BURN_IN_PKT_DELAY, x_strlen(BURN_IN_PKT_DELAY)); // delay(ms)
    x_strncpy(pArgv[4], "1", 1);       // compare or not
    
    i4Ret = _i4EtherSendFrame(5, (const char**)pArgv);
    if(i4Ret != NET_DRV_OK)
    {
      ERR_ETHER("ETH_NG: _i4EtherSendFrame fail\n");
    }
    
    x_thread_delay(10);
    /* SW tx speeed is faster than Ethernet engine, normally lost 3-5 packets */
    if(_u4LossRate == 0)
    {
        i4Ret = NET_DRV_OK;
        Printf("ETH_OK: Ethernet Burn-in pass (pkt_len:%s, pkt_cnt:%s, pkt_delay:%s)\n",
                BURN_IN_PKT_SIZE, BURN_IN_PKT_CNT, BURN_IN_PKT_DELAY);
    }
    else
    {
        i4Ret = NET_DRV_ERROR;
        Printf("ETH_NG: Ethernet Burn-in fail\n");
    }

    _u4EtherDbgLevel = 1;

out :
    _fgEthBurnInStarted = 0;
}


/*******************************************************************************
**  _i4EtherBurnInLBTest
**
**  descriptions:
**      Ethernet Burn-in PHY Loopback test.
**  parameters:
**  return:
*******************************************************************************/
INT32 _i4EtherBurnInLBTest(INT32 i4Argc, const CHAR ** szArgv)
{
    if (_fgEthBurnInStarted)
    {
        Printf("Ethernet Burn-in test already running...\n");
        return -1;
    }
    else
    {
        Printf("Creating Ethernet Burn-in task...\n");
    }
    if (x_thread_create(&_hEthBurnInTask,
                     "EthBurnIn",
                     4096,
                     120, 
                     (x_os_thread_main_fct) _i4EtherBurnInTask,
                     0,
                     NULL) != 0)
    {
        Printf("ETH_NG! Fail to create burn-in task\n");
        return -1;
    }

    return 0;
}


/*******************************************************************************
**  _i4EtherSltTest
**
**  descriptions:
**      This rotinue is called to set/get debug level 
**  parameters:
**      i4Argc - user input string count
**      szArgv - user input string  
**  return:
*******************************************************************************/
INT32 _i4EtherSltTest(INT32 i4Argc, const CHAR ** szArgv)
{
    NET_DRV_IF_T *prDev;// = prEtherGetDev();
    INT32 i4Ret = 0;

    if((i4Argc == 2) &&
       (x_strncmp(szArgv[1], "1", 1) == 0))
    {
        prDev = prEtherGetDev();
        _i4EtherRunTimeTest(prDev, &i4Ret);
    }
    else
    {
        i4Ret = _i4EtherSltLoopbackTest(i4Argc, szArgv);
    }
    
    return (i4Ret);
      }

/*******************************************************************************
**  _i4EtherBurnInTest
**
**  descriptions:
**      This rotinue is to perform the Ethernet Burn-in Test
**  parameters:
**      i4Argc - user input string count
**      szArgv - user input string  
**  return:
*******************************************************************************/
INT32 _i4EtherBurnInTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    
    i4Ret = _i4EtherBurnInLBTest(i4Argc, szArgv);
    
    return (i4Ret);
}

#if CONFIG_CHKSUM
/*******************************************************************************
**  _i4CheckSumInit
**
**  descriptions:
**      This rotinue is called to initialize Chksum driver by cli command
**  parameters:
**      none
**  result:
**  return:
**      0 if success,
**      negative value if fail
*******************************************************************************/
INT32 _i4CheckSumInit(void)
{
  vCheckSumInit();
  return 0;
}

/*******************************************************************************
**  _i4EtherTxChkSumTest
**
**  descriptions:
**      The function is called to test checksum module
**  parameters:
**      none
**  result:
**  return: M6S56_DRV_OK
*******************************************************************************/
INT32 _i4EtherTxChkSumTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size, u4Count, u4SingleSizeTest;

    if(i4Argc != 4)
    {
    	NOTE_ETHER("unknown command\n");
    	NOTE_ETHER("cks u4size u4Count fgSingleSizeTest\n");
        return (0);
    }

    u4Size = (UINT32) StrToInt(szArgv[1]);
    u4Count = (UINT32) StrToInt(szArgv[2]);
    u4SingleSizeTest = (UINT32) StrToInt(szArgv[3]);
    _i4EtherTxChkSumVerify(u4Size, u4Count, u4SingleSizeTest);

    return(0);
}

/*******************************************************************************
**  _i4EtherRxChkSumTest
**
**  descriptions:
**      The function is called to test checksum module
**  parameters:
**      none
**  result:
**  return: M6S56_DRV_OK
*******************************************************************************/
INT32 _i4EtherRxChkSumTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Size, u4Count, u4SingleSizeTest;

    if(i4Argc != 4)
        {
    	NOTE_ETHER("unknown command\n");
    	NOTE_ETHER("cks u4size u4Count fgSingleSizeTest\n");
        return (0);
    }

    u4Size = (UINT32) StrToInt(szArgv[1]);
    u4Count = (UINT32) StrToInt(szArgv[2]);
    u4SingleSizeTest = (UINT32) StrToInt(szArgv[3]);
    _i4EtherRxChkSumVerify(u4Size, u4Count, u4SingleSizeTest);

    return(0);
}



/*******************************************************************************
**  _i4EtherRxChkSumTest
**
**  descriptions:
**      The function is called to test checksum module
**  parameters:
**      none
**  result:
**  return: M6S56_DRV_OK
*******************************************************************************/
INT32 _i4EtherTxRxChkSumTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4TxSize, u4RxSize;
    UINT32 u4TxEnable, u4RxEnable;
    if(i4Argc != 5)
    {
        NOTE_ETHER("unknown command\n");
        NOTE_ETHER("cks TxEnable u4TxSize RxEnable u4RxSize\n");
        NOTE_ETHER("TxEnable: 0: disable TX task\n");
        NOTE_ETHER("TxEnable: 1: Enable TX task\n");
        NOTE_ETHER("RxEnable: 0: disable RX task\n");
        NOTE_ETHER("RxEnable: 1: enable RX task\n");
        return (0);
    }

    u4TxEnable = (UINT32) StrToInt(szArgv[1]);
    u4TxSize = (UINT32) StrToInt(szArgv[2]);
    u4RxEnable = (UINT32) StrToInt(szArgv[3]);
    u4RxSize = (UINT32) StrToInt(szArgv[4]);

    i4ChkSumTestTaskCtrl(u4TxEnable, u4TxSize, u4RxEnable, u4RxSize);
    return(0);
}
#endif

/*******************************************************************************
**  _i4EtherRxChkSumTest
**
**  descriptions:
**      The function is called to test checksum module
**  parameters:
**      none
**  result:
**  return: M6S56_DRV_OK
*******************************************************************************/
INT32 _i4EtherWriteHashTable(INT32 i4Argc, const CHAR ** szArgv)
    {
//  UINT32 u4TxSize, u4RxSize;	
//  UINT32 u4TxEnable, u4RxEnable;	

 NOTE_ETHER("Write Hash Table\n"); 	
  vWriteHashTable();
	
    return(0);
}

/*******************************************************************************
**  i4CmnEthGetNicTest
**
**  descriptions:
**      This rotinue is called to check Synopsys MAC core version 
**  parameters:
**  return:
**      0
*******************************************************************************/
INT32 i4CmnEthGetNicTest(NET_DRV_IF_T *prDev, INT32 *pu4Result)
      {
    UNUSED(prDev);
    ASSERT(pu4Result != NULL);

    *pu4Result = NET_DRV_OK;
    INFO_ETHER("NIC OK\n");   
    return (0);
}

/*******************************************************************************
**  _i4EtherDbgLevel
**
**  descriptions:
**      This rotinue is called to set/get debug level 
**  parameters:
**      i4Argc - user input string count
**      szArgv - user input string  
**  return:
**      0
*******************************************************************************/
INT32 _i4EtherDbgLevel(INT32 i4Argc, const CHAR ** szArgv)
{
    ASSERT(szArgv != NULL);

    if(i4Argc > 1)
    {
        if(i4Argc != 2)
        {
            NOTE_ETHER("unknown command\n");
            return (0);
        }
        _u4EtherDbgLevel = (UINT32)StrToInt(szArgv[1]);
    }

    Printf("debug level is %d\n", _u4EtherDbgLevel);

    return (0);
}

/******************************************************************************
* Function      : _i4WlanOidSet
* Description   : CLI test API, ioctl set OID
* Parameter     : szArgv
* Return        : i4Ret
******************************************************************************/
INT32 _i4EtherOidSet(INT32 i4Argc, const CHAR ** szArgv)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    UINT8 ucCliCmd;
    INT32 i4Ret = 0, i4IoCmd;
    UINT32 u4Parm;
    UINT64 u8Parm;
    	
    ASSERT(szArgv != NULL);
	
    ucCliCmd = (UCHAR) StrToInt(szArgv[1]);
    if((ucCliCmd > OID_CLI_OID_START) &&
       (ucCliCmd < OID_CLI_ETHER_OID_START))
    {
       i4IoCmd = (INT32)(ucCliCmd - OID_CLI_OID_START) | OID_GROUP_CMN_IF;
    }
    else
    if((ucCliCmd > OID_CLI_ETHER_OID_START) &&
       (ucCliCmd < OID_CLI_ETHER_CLI_MAX))
    {
       i4IoCmd = (INT32)(ucCliCmd - OID_CLI_ETHER_OID_START) | OID_GROUP_ETHER;
    }
    else
    {
       ERR_ETHER("OID command error\n");
       return(i4Ret);
    }

    u4Parm = (UINT32) StrToInt(szArgv[2]);
    u8Parm = (UINT32) StrToInt(szArgv[2]);
	
    switch(ucCliCmd)
    {
        case OID_CLI_OID_START:        			
        case OID_CLI_IF_DESCR:        			
        case OID_CLI_IF_TYPE:        			    
        case OID_CLI_IF_MTU:        			   
            break;
        case OID_CLI_IF_SPEED:        			
            i4Ret = i4EtherDrv_Set(prDev, i4IoCmd, &u4Parm);
            break;
        case OID_CLI_IF_PHYADDRESS:  
        case OID_CLI_IF_MULTICASTADDRESSLIST:    
        case OID_CLI_IF_MULTICASTADDRESS:        	
        case OID_CLI_IF_ADMIN_STATUS:        	
        case OID_CLI_IF_OPERSTATUS:        	
        case OID_CLI_IF_LASTCHANGE:        	
        case OID_CLI_IF_NAME:        		
            break;
        case OID_CLI_IF_PROMISCUOUS_MODE:  
            i4Ret = i4EtherDrv_Set(prDev, i4IoCmd, &u4Parm);
            break;
        /* the following info will record in net_dev */
        case OID_CLI_IF_IN_OCTETS:
        case OID_CLI_IF_IN_UCASTPKT:
        case OID_CLI_IF_IN_DISCARDS:
        case OID_CLI_IF_IN_ERRORS:
        case OID_CLI_IF_IN_UNKNOWN_PROTOS:        
        case OID_CLI_IF_OUT_OCTETS:
        case OID_CLI_IF_OUT_UCASTPKTS:
        case OID_CLI_IF_OUT_DISCARDS:
        case OID_CLI_IF_OUT_ERRORS:
        case OID_CLI_IF_IN_MULTICAST_PKTS:        
        case OID_CLI_IF_IN_BROADCAST_PKTS:        
        case OID_CLI_IF_OUT_MULTICAST_PKTS:       
        case OID_CLI_IF_OUT_BROADCAST_PKTS:  
            i4Ret = i4EtherDrv_Set(prDev, i4IoCmd, &u8Parm);
            NOTE_ETHER("set stat 0x%04x = %d\n", i4IoCmd, u8Parm);
            break;
        case OID_CLI_IF_MULTICASTADDRESS_DEL: 
            i4Ret = i4CliEthMulticastDelList(i4IoCmd, 1, i4Argc, szArgv);
            break;
        /* -------------------------------------------------------------- */
        case OID_CLI_ETHER_OID_START:                
        case OID_CLI_ETHER_ALIGNMENT_ERRORS:          
        case OID_CLI_ETHER_FCSERRORS:                 
        case OID_CLI_ETHER_SINGLECOLLISIONFRAMES:     
        case OID_CLI_ETHER_MULTIPLECOLLISIONFRAMES:   
        case OID_CLI_ETHER_DEFERREDTRANSMISSIONS:     
        case OID_CLI_ETHER_LATECOLLISIONS:            
        case OID_CLI_ETHER_EXCESSIVECOLLISIONS:       
        case OID_CLI_ETHER_INTERNALMACTRANSMITERRORS:
        case OID_CLI_ETHER_FRAMETOOLONGS:             
        case OID_CLI_ETHER_INTERNALMACRECEIVEERRORS:  
        case OID_CLI_ETHER_SYMBOLERRORS:              
        case OID_CLI_ETHER_DUPLEXSTATUS:              
            i4Ret = i4EtherDrv_Set(prDev, i4IoCmd, &u4Parm);
            NOTE_ETHER("set error stat 0x%04x = %d\n", i4IoCmd, u4Parm);
            break;
        default:
            i4Ret = i4EtherDrv_Set(prDev, i4IoCmd, &u4Parm);
            break;        	
    }

	return(i4Ret);
}

/******************************************************************************
* Function      : _i4WlanOidGet
* Description   : CLI test API, ioctl get OID
* Parameter     : szArgv
* Return        : i4Ret
******************************************************************************/
INT32 _i4EtherOidGet(INT32 i4Argc, const CHAR ** szArgv)
{
    NET_DRV_IF_T *prDev = prEtherGetDev();
    UINT8 ucCliCmd;
    INT32 i4Ret = 0, i4IoCmd;
    UINT32 u4Parm;
    UINT64 u8Parm;
	
    ASSERT(szArgv != NULL);
	
    ucCliCmd = (UCHAR) StrToInt(szArgv[1]);
    if((ucCliCmd > OID_CLI_OID_START) &&
      (ucCliCmd < OID_CLI_ETHER_OID_START))
    {
       i4IoCmd = (INT32)(ucCliCmd - OID_CLI_OID_START) | OID_GROUP_CMN_IF;
    }
    else
    if((ucCliCmd > OID_CLI_ETHER_OID_START) &&
       (ucCliCmd < OID_CLI_ETHER_CLI_MAX))
    {
       i4IoCmd = (INT32)(ucCliCmd - OID_CLI_ETHER_OID_START) | OID_GROUP_ETHER;
    }
    else
    {
       ERR_ETHER("OID command error\n");
       return(i4Ret);
    }
	
    switch(ucCliCmd)
    {
        case OID_CLI_OID_START:        			
            break;
        case OID_CLI_IF_MTU:        			   
            i4Ret = i4EtherDrv_Get(prDev, i4IoCmd, &u4Parm);
            NOTE_ETHER("MTU is %d byte\n", u4Parm);
            break;
        case OID_CLI_IF_SPEED:        			
            i4Ret = i4EtherDrv_Get(prDev, i4IoCmd, &u4Parm);
            NOTE_ETHER("link speed is %dMbps\n", u4Parm);
            break;
        case OID_CLI_IF_PHYADDRESS:  
        case OID_CLI_IF_MULTICASTADDRESSLIST:    
        case OID_CLI_IF_MULTICASTADDRESS:        	
        case OID_CLI_IF_ADMIN_STATUS:        	
        case OID_CLI_IF_OPERSTATUS:        	
        case OID_CLI_IF_LASTCHANGE:        	
        case OID_CLI_IF_NAME:        		
            break;
        case OID_CLI_IF_PROMISCUOUS_MODE:        	
            i4Ret = i4EtherDrv_Get(prDev, i4IoCmd, &u4Parm);
            if(u4Parm)
            {
                NOTE_ETHER("promiscuous mode\n");
            }
            else
            {
                NOTE_ETHER("not promiscuous mode\n");
            }
            break;
        /* the following info will record in net_dev */
        case OID_CLI_IF_IN_OCTETS:        	
        case OID_CLI_IF_IN_UCASTPKT:        	
        case OID_CLI_IF_IN_DISCARDS:        	
        case OID_CLI_IF_IN_ERRORS:        	
        case OID_CLI_IF_IN_UNKNOWN_PROTOS:        
        case OID_CLI_IF_OUT_OCTETS:        	
        case OID_CLI_IF_OUT_UCASTPKTS:        	
        case OID_CLI_IF_OUT_DISCARDS:        	
        case OID_CLI_IF_OUT_ERRORS:        	
        case OID_CLI_IF_IN_MULTICAST_PKTS:        
        case OID_CLI_IF_IN_BROADCAST_PKTS:        
        case OID_CLI_IF_OUT_MULTICAST_PKTS:       
        case OID_CLI_IF_OUT_BROADCAST_PKTS:  
            i4Ret = i4EtherDrv_Get(prDev, i4IoCmd, &u8Parm);
            NOTE_ETHER("get stat 0x%04x = %d\n", i4IoCmd, u8Parm);
            break;
        /* -------------------------------------------------------------- */
        case OID_CLI_ETHER_OID_START:                
        case OID_CLI_ETHER_ALIGNMENT_ERRORS:          
        case OID_CLI_ETHER_FCSERRORS:                 
        case OID_CLI_ETHER_SINGLECOLLISIONFRAMES:     
        case OID_CLI_ETHER_MULTIPLECOLLISIONFRAMES:   
        case OID_CLI_ETHER_DEFERREDTRANSMISSIONS:     
        case OID_CLI_ETHER_LATECOLLISIONS:            
        case OID_CLI_ETHER_EXCESSIVECOLLISIONS:       
        case OID_CLI_ETHER_INTERNALMACTRANSMITERRORS:
        case OID_CLI_ETHER_FRAMETOOLONGS:             
        case OID_CLI_ETHER_INTERNALMACRECEIVEERRORS:  
        case OID_CLI_ETHER_SYMBOLERRORS:              
        case OID_CLI_ETHER_DUPLEXSTATUS:              
            i4Ret = i4EtherDrv_Get(prDev, i4IoCmd, &u4Parm);
            NOTE_ETHER("get error stat 0x%04x = %d\n", i4IoCmd, u4Parm);
            break;
        default:
            i4Ret = i4EtherDrv_Get(prDev, i4IoCmd, &u4Parm);
            break;	
    }

    return(i4Ret);
}

/******************************************************************************
* Function      : _i4EtherDiag_AdjPhySignal
* Description   : Ethernet PHY adjust CLI
* Parameter     : szArgv
* Return        : i4Ret
******************************************************************************/
INT32 _i4EtherAdjPhySignal(INT32 i4Argc, const CHAR ** szArgv)
{
  UINT16 chipId;
  UINT16 type = 0;
  UINT16 val;

    if (i4Argc != 4)
    {
        Printf("Argument error!\n");
        Printf("adjp [chip ID] [adjtype] [val]\n");
        Printf("[chip ID]: 8550 or 8555 ..\n");
        Printf("[adjtype]: 1: 100M Amplitude, [val]: 0~15\n");
        Printf("[adjtype]: 2: Slew Rate, [val]: 0x0~0x6\n");
        Printf("[adjtype]: 3: BW In Bias, [val]: 0~3\n");
        Printf("[adjtype]: 4: BW Out Bias, [val]: 0~3\n");
        Printf("[adjtype]: 5: BW 50 percent off, [val]: 0:0 percent off 1:50 percent off\n");
        Printf("[adjtype]: 6: BW Feedback Cap, [val]: 0~3\n");
        Printf("[adjtype]: 7: Eye Open, [val]: 0~7\n");
        Printf("[adjtype]: 8: 10M Amplitude, [val]: 0~3\n");
        Printf("[adjtype]: 9: Phy Loop On, [val]: 0: Loop off, 1: Loop On\n");
        Printf("[adjtype]: 10: Force MDI/MDIX, [val]: 0: Not force, 1: Force MDI, 2:Force MDIX\n");
        Printf("[adjtype]: 11: Set Speed, [val]: 0: 10M FULL, 1: 100M FULL\n");
        Printf("[adjtype]: 12: Set Pattern Gen, [val]: 0: None, 1: Random 2:All Zero 3:All one\n");
        
        return 0;
    }
    chipId = (UINT16)StrToInt(szArgv[1]);
    type = (UINT16)StrToInt(szArgv[2]);
    val = (UINT16)StrToInt(szArgv[3]);
    
    switch(type)
    {
      case 1:
      vSet100MAmp(chipId, val);      
      break;
      
      case 2:
      vSetSlewRate(chipId, val);
      break;
      
      case 3:
      vSetInputBias(chipId, val);
      break;
      
      case 4:
      vSetOutputBias(chipId, val);
      break;
      
      case 5:
      vSet50percentBW(chipId, val);
      break;
      
      case 6:
      vSetFeedbackCap(chipId, val);
      break;
      
      case 7:
      vSetEyeOpen(chipId, val);
      break;
      
      case 8:
      vSet10MAmp(chipId, val);
      break;	
      
      case 9:
      vSetPhyLoopOn(chipId, val);
      break;
      
      case 10:
      vSetForceMdiCrossover(chipId, val);
      break;
     
      case 11:
      vSetSpeed(chipId, val);
      break;
      
      case 12:
      vSetPhyEPG(chipId, val);
      break;
    
      default:
      break;
      	
    }	
    
    return 0;
}   


INT32 _i4EtherGetPhySignalSetting(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT16 chipId;
    UINT16 type = 0;
    

    if (i4Argc != 3)
    {
        Printf("Argument error!\n");
        Printf("gp [chip ID] [gettype]\n");
        Printf("[chip ID]: 8550 or 8555 ..\n");
        Printf("[gettype]: 1: 100M Amplitude setting: 0~15\n");
        Printf("[gettype]: 2: Slew Rate setting: 0~6\n");
        Printf("[gettype]: 3: BW In Bias setting: 0~3\n");
        Printf("[gettype]: 4: BW Out Bias setting: 0~3\n");
        Printf("[gettype]: 5: BW 50 percent off setting: 0:0 percent off 1:50 percent off\n");
        Printf("[gettype]: 6: BW Feedback Cap setting: 0~3\n");
        Printf("[gettype]: 7: Eye Open setting: 0~7\n");
        Printf("[gettype]: 8: 10M Amplitude setting: 0~3\n");
        return 0;
    }
    chipId = (UINT16)StrToInt(szArgv[1]);
    type = (UINT16)StrToInt(szArgv[2]);
    
    
    switch(type)
    {
      case 1:
      u1Get100MAmp(chipId);      
      break;
      
      case 2:
      u1GetSlewRate(chipId);
      break;
      
      case 3:
      u1GetInputBias(chipId);
      break;
      
      case 4:
      u1GetOutputBias(chipId);
      break;
      
      case 5:
      u1Get50percentBW(chipId);
      break;
      
      case 6:
      u1GetFeedbackCap(chipId);
      break;
      
      case 7:
      u1GetEyeOpen(chipId);
      break;
      
      case 8:
      u1Get10MAmp(chipId);
      break;	
    
      default:
      break;
      	
    }	
    
    
    return 0;
}

INT32 _i4EtherSetTxPacketType(INT32 i4Argc, const CHAR ** szArgv)
{
	
    UINT16 type = 0;
    

    if (i4Argc != 2)
    {
        Printf("Argument error!\n");
        Printf("pa [type]\n");
        Printf("[type]: 0: default Packet\n");
        Printf("[type]: 1: UDP wrong CheckSum Packet\n");
        Printf("[type]: 2: TCP wrong CheckSum Packet\n");
        Printf("[type]: 3: UDP Normal CheckSum Packet\n");
        Printf("[type]: 4: TCP Normal CheckSum Packet\n");
        Printf("[type]: 5: Custom Packet defined by pktcustom (default: 00..FF)\n");
        return 0;
    }
    
    type = (UINT8)StrToInt(szArgv[1]);
    
    _TxPacketType = type;
    
    return 0;
}

INT32 _i4EtherSetTxCheckSumOffLoad(INT32 i4Argc, const CHAR ** szArgv)
{
  UINT16 type = 0;
    

    if (i4Argc != 2)
    {
        Printf("Argument error!\n");
        Printf("txchk [enable]\n");
        Printf("[enable]: 0: Disable TX checksum offload\n");
        Printf("[enable]: 1: Enable TX checksum offload\n");
        
        return 0;
    }
    
    type = (UINT8)StrToInt(szArgv[1]);
    
   if(type == 1) 	
     vEnableSupportTxCheckSumOffLoad(TRUE);
   else  
     vEnableSupportTxCheckSumOffLoad(FALSE);
  return 0;     
}

extern INT32  fgDMAPollingEn;
INT32 _i4EtherDMAAutoPollingTest(INT32 i4Argc, const CHAR ** szArgv)
{

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		UTIL_Printf("[0:disable 1:enable] \n");
		return 0;
	}
	
	fgDMAPollingEn = (UINT8)StrToInt(szArgv[1]); 
	
	if(fgDMAPollingEn)
	{
		UTIL_Printf(" Polling test enable %d\n");
	}
	else
	{
		UTIL_Printf("Polling test disable %d\n");
	}
	
	if(fgDMAPollingEn)
	{
	  UTIL_Printf("Set bit TX/RX_POLL_EN\n");
	  SetBitMAC(RW_DMA_CFG, TX_POLL_EN);		 // Start Tx
	  SetBitMAC(RW_DMA_CFG, RX_POLL_EN);
	}
	else
	{
	  UTIL_Printf("Clear bit TX/RX_POLL_EN\n");
	  ClrBitMAC(RW_DMA_CFG, TX_POLL_EN);		 // Start Tx
	  ClrBitMAC(RW_DMA_CFG, RX_POLL_EN);
	}

  return 0;
}


INT32 _i4EtherINTTest(INT32 i4Argc, const CHAR ** szArgv)
{
    extern INT32 fgIntTest;
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		UTIL_Printf("[0:disable 1:enable] \n");
		return 0;
	}
	
	fgIntTest = (INT32)StrToInt(szArgv[1]); 
	//fgSetPollingEn = (UINT8)StrToInt(szArgv[2]); 
	//fgSetDesOwnBit = (UINT8)StrToInt(szArgv[3]); 
	
	
	  if(fgIntTest)
	  {
	    UTIL_Printf("enable Interrupt test , counter = %d \n",fgIntTest);
        //vWriteMAC(RW_INT_MASK,0x00);
		vWriteMAC(RW_INT_MASK,0x00);  ///enable all interrupt
	  }
	  else
	  {
	    UTIL_Printf("disable Interrupt test \n");
		vDmaIntEnable((DMA_INT_ENABLE|PORT_STATUS_CFG|MIB_COUNTER_TH));
	  	
	  }
	


  return 0;
}


INT32 _i4EtherPauseFrmDecTest(INT32 i4Argc, const CHAR ** szArgv)
{
    extern INT32 PauseFrameDecodeTest;
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		UTIL_Printf("[0:disable 1:enable] \n");
		return 0;
	}


	PauseFrameDecodeTest = (INT32)StrToInt(szArgv[1]); 

	if(PauseFrameDecodeTest)
	{
       ClrBitMAC(RW_INT_MASK,TNTC);
	   UTIL_Printf("enable test \n");
	}
	else
	{
      // SetBitMAC(RW_INT_MASK,TNTC);
	   UTIL_Printf("disable test\n");
	}
		

   return 0;
}


extern void vWriteEthPhy(UINT32 u4RegAddr, UINT32 u4RegData);
extern UINT32 u4ReadEthPhy(UINT32 u4RegAddr);

static INT32 _Ether_Wait(INT32 n)
{
    volatile UINT32 i, j;
    for (; n != 0; n--);
        for (i=0; i<10000; i++)
            for (j=0; j<10000; j++)
                ;
    return 0;
}

INT32  _i4EtherResetPhyTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i;
    vWriteEthPhy(0x1f, 0x00);
    UTIL_Printf("Please wait!\n");
    for(i=0;i<100;i++)
    {
        vWriteEthPhy(0x00, 0x4fff);
        _Ether_Wait(1);
        vWriteEthPhy(0x00, 0xCfff);
        _Ether_Wait(10);
        if(u4ReadEthPhy(0x00)!=0x3000) 
        {
            UTIL_Printf("fail!  %d ; %d \n",i,u4ReadEthPhy(0x00));
	    	break;
        }
    }
    UTIL_Printf("Test Pass!\n");
    return 0;
}



extern INT32  _TxDesCtrlBitOnfgVTG;
extern INT32   _TxDesCtrlfgVTG;

extern INT32  _TxDesCtrlBitOnfgTCO;
extern INT32   _TxDesCtrlfgTCO;

extern INT32  _TxDesCtrlBitOnfgUCO;
extern INT32   _TxDesCtrlfgUCO;

extern INT32  _TxDesCtrlBitOnfgICO;
extern INT32   _TxDesCtrlfgICO;

extern INT32  _TxDesCtrlBitOnfgINSV;
extern INT32   _TxDesCtrlfgINSV;


extern INT32  _TxDesCtrlBitOnfgTxIntOn;
extern INT32   _TxDesCtrlfgTxIntOn;


extern INT32  _TxDesCtrlBitOnfgLastSeg;
extern INT32   _TxDesCtrlfgLastSeg;
 
extern INT32  _TxDesCtrlBitOnfgFirstSeg;
extern INT32   _TxDesCtrlfgFirstSeg;
 
extern INT32  _TxDesCtrlBitOnfgEndOfRing;
extern INT32   _TxDesCtrlfgEndOfRing;


INT32 _i4EtherTxDesSet (INT32 i4Argc, const CHAR ** szArgv)
{

	INT32 i4Ret=0;
	INT32 fgOn=0;
	INT32 FlagValue=0;
			
		UNUSED(i4Argc);
		UNUSED(szArgv);
	
		if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL)||(szArgv[2] == NULL)||(szArgv[3] == NULL))
		{
			UTIL_Printf("[0~8: Tx descriptor flag]  [0/1: disable/enable] [0/1 value]\n");
			UTIL_Printf("0 fgVTG\n");
			UTIL_Printf("1 fgTCO\n");
			UTIL_Printf("2 fgUCO\n");
			UTIL_Printf("3 fgICO\n");
			UTIL_Printf("4 fgINSV\n");
			UTIL_Printf("5 fgTxIntOn\n");
			UTIL_Printf("6 fgLastSeg\n");
			UTIL_Printf("7 fgFirstSeg\n");
			UTIL_Printf("8 fgEndOfRing\n");
			return 0;
		}

	
		i4Ret = (UINT8)StrToInt(szArgv[1]); 
		fgOn = (UINT8)StrToInt(szArgv[2]); 
		FlagValue = (UINT8)StrToInt(szArgv[3]); 

		if(i4Ret<0 || i4Ret>8)
		{
			UTIL_Printf("[0~8: Tx descriptor flag]  [0/1: disable/enable] [0/1 value]\n");
			UTIL_Printf("0 fgVTG\n");
			UTIL_Printf("1 fgTCO\n");
			UTIL_Printf("2 fgUCO\n");
			UTIL_Printf("3 fgICO\n");
			UTIL_Printf("4 fgINSV\n");
			UTIL_Printf("5 fgTxIntOn\n");
			UTIL_Printf("6 fgLastSeg\n");
			UTIL_Printf("7 fgFirstSeg\n");
			UTIL_Printf("8 fgEndOfRing\n");
			return 0;
		}

		
        if(fgOn)
        {
		    fgOn = 1;
		    UTIL_Printf("Flag enable  \n");
        }
		else
		{
		    fgOn = 0;
		    UTIL_Printf("Flag Disable  \n");
		}

        if(FlagValue)
        {
		    FlagValue = 1;
        }
		else
		{
		    FlagValue = 0 ;
		}

		switch(i4Ret)
		{
			case 0: _TxDesCtrlBitOnfgVTG=fgOn; _TxDesCtrlfgVTG=FlagValue;  UTIL_Printf("0 fgVTG=%x\n",FlagValue ); break;
			case 1: _TxDesCtrlBitOnfgTCO=fgOn; _TxDesCtrlfgTCO=FlagValue; UTIL_Printf("1 fgTCO=%x\n",FlagValue);break;
			case 2: _TxDesCtrlBitOnfgUCO=fgOn; _TxDesCtrlfgUCO=FlagValue; UTIL_Printf("2 fgUCO=%x\n",FlagValue);break;
			case 3: _TxDesCtrlBitOnfgICO=fgOn; _TxDesCtrlfgICO=FlagValue; UTIL_Printf("3 fgICO=%x\n",FlagValue);break;
			case 4: _TxDesCtrlBitOnfgINSV=fgOn; _TxDesCtrlfgINSV=FlagValue; UTIL_Printf("4 fgINSV=%x\n",FlagValue);break;
			case 5: _TxDesCtrlBitOnfgTxIntOn=fgOn; _TxDesCtrlfgTxIntOn=FlagValue; UTIL_Printf("5 fgTxIntOn=%x\n",FlagValue);break;
			case 6: _TxDesCtrlBitOnfgLastSeg=fgOn; _TxDesCtrlfgLastSeg=FlagValue; UTIL_Printf("6 fgLastSeg=%x\n",FlagValue);break;
			case 7: _TxDesCtrlBitOnfgFirstSeg=fgOn; _TxDesCtrlfgFirstSeg=FlagValue; UTIL_Printf("7 fgFirstSeg=%x\n",FlagValue);break;
			case 8: _TxDesCtrlBitOnfgEndOfRing=fgOn; _TxDesCtrlfgEndOfRing=FlagValue; UTIL_Printf("8 fgEndOfRing=%x\n",FlagValue);break;
			
		}

		return (0);
}

INT32 _i4EtherSetInterPHYCancelDCOffset(INT32 i4Argc, const CHAR ** szArgv)
{
//    i4InternalPhyOffsetCancle();
    return 0;

}

#if 1 // MT5396_ETHER_LOWLEVEL_CLI
INT32 _i4EtherReadRegister(UINT32 u4Offset, UINT32 u4Len)
{
    UINT32 i;
    
    for (i=u4Offset; i<u4Offset+u4Len; i+=4)
    {
        if ((i-u4Offset) % 16 == 0)
        {
            Printf("%04X: ", i);
        }
        Printf(" %08X", dReadMAC(i));
        if ((i-u4Offset) % 16 == 12)
        {
            Printf("\n");
        }
    }
    Printf("\n");
    return 0;
}

INT32 _i4EtherWriteRegister(UINT32 u4Offset, UINT32 u4Value)
{
    vWriteMAC(u4Offset, u4Value);
    return 0;
}

INT32 _i4EtherGetBits(UINT32 u4Offset, UINT32 u4MSB, UINT32 u4LSB)
{
    UINT32 u4Data, u4Mask;
    
    if (u4MSB == 31 && u4LSB == 0)
        u4Mask = 0xFFFFFFFF;
    else
        u4Mask = (1 << (u4MSB-u4LSB+1)) -1;
    u4Data = dReadMAC(u4Offset);
    u4Data = (u4Data >> u4LSB) & u4Mask;
    Printf("Get %04X[%d:%d] = 0x%08X\n", u4Offset, u4MSB, u4LSB, u4Data);
    return 0;
}

INT32 _i4EtherSetBits(UINT32 u4Offset, UINT32 u4MSB, UINT32 u4LSB, UINT32 u4Value)
{
    UINT32 u4Data, u4Mask;
    
    if (u4MSB == 31 && u4LSB == 0)
        u4Mask = 0xFFFFFFFF;
    else
        u4Mask = (1 << (u4MSB-u4LSB+1)) -1;
    u4Value = (u4Value & u4Mask);
    u4Data = dReadMAC(u4Offset);
    u4Data = (u4Data & ~(u4Mask<<u4LSB)) | (u4Value << u4LSB);
    vWriteMAC(u4Offset, u4Data);

    Printf("Set %04X[%d:%d] = 0x%08X\n", u4Offset, u4MSB, u4LSB, u4Value);
    _i4EtherGetBits(u4Offset, u4MSB, u4LSB);
    return 0;
}

INT32 _i4EtherPktInit(UINT32 u4Len)
{
    if (u4Len >= PKT_BUFFER_SIZE)
        u4Len = PKT_BUFFER_SIZE;
    x_memset(_eth_custom_packet, 0, u4Len);
    _eth_custom_packet_len = u4Len;

    return 0;
}

INT32 _i4EtherPktSet(UINT32 u4Offset, INT32 i4Len, const CHAR **szData)
{
    INT32 i;
    UINT32 u4Ch;

    if (u4Offset + i4Len > _eth_custom_packet_len)
    {
        i4Len = _eth_custom_packet_len - u4Offset;
    }
    for (i=0; i<i4Len; i++)
    {
        x_sscanf(szData[i], "%x", &u4Ch);
        _eth_custom_packet[u4Offset+i] = (UINT8)u4Ch;
    }
    return 0;
}

INT32 _i4EtherPktDump(void)
{
    UINT32 i;

    UTIL_Printf("ADDR |  0  1  2  3  4  5  6  7| 8  9  A  B  C  D  E  F  (Packet Length: %d)\n"
                "-----+------------------------+-----------------------\n", _eth_custom_packet_len);
    for (i=0; i<_eth_custom_packet_len; i++)
    {
        if ((i % 16) == 0)
            UTIL_Printf("\n%04X | ", i);
        UTIL_Printf(" %02X", _eth_custom_packet[i]);
    }
    UTIL_Printf("\n");

    return 0;
}

#endif
