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
 *   Ether CLI debug mode header file
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

#ifndef X_ETHER_CLI_H
#define X_ETHER_CLI_H

/****************************************************************************
** Constant definitions
****************************************************************************/
typedef enum 
{
    OID_CLI_OID_START        	            = 0,
    OID_CLI_IF_INDEX                        = 1,        /* param : INT32 */
    OID_CLI_IF_DESCR                        = 2,        /* param : CHAR[256] */
    OID_CLI_IF_TYPE                         = 3,        /* param : UINT32 */
    OID_CLI_IF_MTU                          = 4,        /* param : UINT32 */
    OID_CLI_IF_SPEED                        = 5,        /* param : UINT32 */
    OID_CLI_IF_PHYADDRESS                   = 6,        /* param : MAC_ADDRESS_T */
    OID_CLI_IF_MULTICASTADDRESSLIST         = 7,        /* param : NET_MAC_ADDRESS_LIST_T */
    OID_CLI_IF_MULTICASTADDRESS             = 8,        /* param : MAC_ADDRESS_T */
    OID_CLI_IF_ADMIN_STATUS                 = 9,        /* param : UINT32 */
    OID_CLI_IF_OPERSTATUS                   = 10,       /* param : UINT32 */
    OID_CLI_IF_LASTCHANGE                   = 11,       /* param : UINT32 */
    OID_CLI_IF_NAME                         = 12,       /* param : CHAR[256] */
    OID_CLI_IF_PROMISCUOUS_MODE             = 13,       /* param : UINT32 */
    /* the following info will record in net_dev */
    OID_CLI_IF_IN_OCTETS                    = 14,       /* param : UINT64 */
    OID_CLI_IF_IN_UCASTPKT                  = 15,       /* param : UINT64 */
    OID_CLI_IF_IN_DISCARDS                  = 16,       /* param : UINT64 */
    OID_CLI_IF_IN_ERRORS                    = 17,       /* param : UINT64 */
    OID_CLI_IF_IN_UNKNOWN_PROTOS            = 18,       /* param : UINT64 */
    OID_CLI_IF_OUT_OCTETS                   = 19,       /* param : UINT64 */
    OID_CLI_IF_OUT_UCASTPKTS                = 20,       /* param : UINT64 */
    OID_CLI_IF_OUT_DISCARDS                 = 21,       /* param : UINT64 */
    OID_CLI_IF_OUT_ERRORS                   = 22,       /* param : UINT64 */
    OID_CLI_IF_IN_MULTICAST_PKTS            = 23,       /* param : UINT64 */
    OID_CLI_IF_IN_BROADCAST_PKTS            = 24,       /* param : UINT64 */
    OID_CLI_IF_OUT_MULTICAST_PKTS           = 25,       /* param : UINT64 */
    OID_CLI_IF_OUT_BROADCAST_PKTS           = 26,       /* param : UINT64 */
    OID_CLI_IF_IP_ADDRESS                   = 27,       /* param : UINT32, IP address (get only) */
    OID_CLI_IF_MULTICASTADDRESS_DEL         = 28,       /* remove multicast addr */
    OID_CLI_IF_CONNECT_STATUS               = 29,       /* param : UINT32 */
    OID_CLI_IF_NIC_STATUS                   = 30,       /* param : UINT32 */
    OID_CLI_IF_CURRNET_SPEED_INFO           = 31,       /* param : UINT32 */
    /* -------------------------------------------------------------- */
    OID_CLI_ETHER_OID_START                 = 40,       
    OID_CLI_ETHER_ALIGNMENT_ERRORS          = 41,       /* param : UINT32 */
    OID_CLI_ETHER_FCSERRORS                 = 42,       /* param : UINT32 */
    OID_CLI_ETHER_SINGLECOLLISIONFRAMES     = 43,       /* param : UINT32 */
    OID_CLI_ETHER_MULTIPLECOLLISIONFRAMES   = 44,       /* param : UINT32 */
    OID_CLI_ETHER_DEFERREDTRANSMISSIONS     = 45,       /* param : UINT32 */
    OID_CLI_ETHER_LATECOLLISIONS            = 46,       /* param : UINT32 */
    OID_CLI_ETHER_EXCESSIVECOLLISIONS       = 47,       /* param : UINT32 */
    OID_CLI_ETHER_INTERNALMACTRANSMITERRORS = 48,       /* param : UINT32 */
    OID_CLI_ETHER_FRAMETOOLONGS             = 49,       /* param : UINT32 */
    OID_CLI_ETHER_INTERNALMACRECEIVEERRORS  = 50,       /* param : UINT32 */
    OID_CLI_ETHER_SYMBOLERRORS              = 51,       /* param : UINT32 */
    OID_CLI_ETHER_DUPLEXSTATUS              = 52,       /* param : UINT32 */
    /* -------------------------------------------------------------- */
	OID_CLI_ETHER_CLI_MAX
}   ETHER_CLI_OID_T;

/****************************************************************************
** Structure and Type definitions
****************************************************************************/

/****************************************************************************
** Export API
****************************************************************************/

/****************************************************************************
** External Network API
****************************************************************************/
INT32 _i4EtherInit(void);
INT32 _i4EtherDeInit(void);
INT32 _i4EtherStart(void);
INT32 _i4EtherStop(void);
INT32 _i4EtherReadPhy(UINT32 u4RegAddr,UINT32 u4Len);
INT32 _i4EtherWritePhy(UINT32 u4RegAddr, UINT32 u4RegData);
INT32 _i4EtherPhyANTest(void);
INT32 _i4EtherSetLoopBack(INT32 i4Enable);
INT32 _i4EtherSendFrame(INT32 i4Argc, const CHAR ** szArgv);
INT32 _i4EtherOidSet(INT32 i4Argc, const CHAR ** szArgv);
INT32 _i4EtherOidGet(INT32 i4Argc, const CHAR ** szArgv);
INT32 _i4CliEthMacAddrSet(void *pvArgv);
INT32 _i4CliEthMacAddrGet(void *pvArgv);
INT32 _i4EtherReadRegister(UINT32 u4Offset, UINT32 u4Len);
INT32 _i4EtherWriteRegister(UINT32 u4Offset, UINT32 u4Value);
INT32 _i4EtherGetBits(UINT32 u4Offset, UINT32 u4MSB, UINT32 u4LSB);
INT32 _i4EtherSetBits(UINT32 u4Offset, UINT32 u4MSB, UINT32 u4LSB, UINT32 u4Value);

INT32 _i4EtherDMAAutoPollingTest(INT32 i4Argc, const CHAR ** szArgv);

INT32 _i4EtherAdjPhySignal(INT32 i4Argc, const CHAR ** szArgv);
INT32 _i4EtherGetPhySignalSetting(INT32 i4Argc, const CHAR ** szArgv);
//INT32 i4EtherIndicateFrame(NET_DRV_IF_T *prDev, PKT_BUFF_T *prPktBuf);

INT32 i4ChkSumTestTaskCtrl(BOOL TxEnable, UINT32 TxCkSumSize, BOOL RxEnable, UINT32 RxCkSumSize);

#if CONFIG_CHKSUM
INT32 _i4CheckSumInit(void);
INT32 _i4EtherTxChkSumTest(INT32 i4Argc, const CHAR ** szArgv);
INT32 _i4EtherRxChkSumTest(INT32 i4Argc, const CHAR ** szArgv);
INT32 _i4EtherTxRxChkSumTest(INT32 i4Argc, const CHAR ** szArgv);
#endif

#endif /* X_ETHER_CLI_H */
