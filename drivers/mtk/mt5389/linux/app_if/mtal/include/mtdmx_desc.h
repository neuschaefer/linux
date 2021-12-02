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
 * $RCSfile: mtdmx_desc.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/ 

/** @file 
 *  This header file declares exported APIs of DMX Descramble module.
 */

#ifndef MT_DMX_DESC_H
#define MT_DMX_DESC_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef UINT32      MTDMX_DESC_DESCRAMBLER_ID;

typedef struct
{
    UINT8 u1Descrambler_Max;        /* The number of descrambler instances available*/
    BOOL fgDVB_CSA_Supported;       /* TRUE if DVB Common Scrambling Algorithm is available(Should be)*/
    BOOL fgAES_128_CBC_Supported;   /* TRUE if AES 128 bits CBC descrambler is available.
                                       Mandatory for CI+ for HD channels."*/
    BOOL fgDES_56_ECB_Supported;    /* TRUE if DES 56 bits ECB descrambler is available. 
                                       Mandatory for CI+ stack for SD channels.*/
} MTDMX_DESC_CAPABILITY_T;

typedef enum
{
    MTDMX_DESC_NO_ERROR = 0,        /* OK returned */
    MTDMX_DESC_NOT_DONE,            /* operation not performed */
    MTDMX_DESC_ERROR                /* Bad parameter given to the function */
} MTDMX_DESC_ERROR_T;

typedef enum
{
    MTDMX_DESC_DVB_CSA = 0x00,
    MTDMX_DESC_AES_128_CBC = 0x01,
    MTDMX_DESC_DES_56_ECB = 0x02
} MTDMX_DESC_DESCTYPE_T;

typedef enum
{
    MTDMX_DESC_ODD_KEY = 0,
    MTDMX_DESC_EVEN_KEY,
    MTDMX_DESC_GENERIC_KEY          /*otherwise : e.g. initial vector*/
} MTDMX_DESC_KEYTYPE_T;

typedef enum
{               
    MTDMX_DESC_RESERVED = 0x00,             
    MTDMX_DESC_VIDEO = 0x01,                
    MTDMX_DESC_OTHER_VIDEO = 0x02,              // ignore
    MTDMX_DESC_AUDIO = 0x03,                
    MTDMX_DESC_OTHER_AUDIO = 0x04,              // ignore
    MTDMX_DESC_PRIVATE_SECTION = 0x05,              
    MTDMX_DESC_PES_PACKETS_CONTAINING_PRIVATE_DATA = 0x06,              
    MTDMX_DESC_MHEG = 0x07,             
    MTDMX_DESC_DSM_CC = 0x08
} MTDMX_DESC_STREAM_TYPE_T;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*
 *  Initialises the descrambler.                        
 *  This function must be called before any other functions of the MTDMX_DESC API.                      
 */
//-----------------------------------------------------------------------------
EXTERN MTDMX_DESC_ERROR_T MTDMX_DESC_Init(void);

//-----------------------------------------------------------------------------
/*
 *  Terminates the descrambler      
 */
//-----------------------------------------------------------------------------
EXTERN MTDMX_DESC_ERROR_T MTDMX_DESC_Terminate(void);

//-----------------------------------------------------------------------------
/*
 *  Open an instance of the descrambler
 *
 *  @param  eStreamType   Only for capability
 */
//-----------------------------------------------------------------------------
EXTERN MTDMX_DESC_DESCRAMBLER_ID MTDMX_DESC_Open_Descrambler(MTDMX_DESC_STREAM_TYPE_T eStreamType);

//-----------------------------------------------------------------------------
/*
 *  Close an instance of the descrambler
 *
 *  @param  eDescID   ID of the instance
 */
//-----------------------------------------------------------------------------
EXTERN MTDMX_DESC_ERROR_T MTDMX_DESC_Close_Descrambler(MTDMX_DESC_DESCRAMBLER_ID eDescID);

//-----------------------------------------------------------------------------
/*
 *  Enable to get platform capability
 *
 *  @param  prCap           Capability of the platform
 */
//-----------------------------------------------------------------------------
EXTERN MTDMX_DESC_ERROR_T MTDMX_DESC_GetCapability(MTDMX_DESC_CAPABILITY_T *prCap);

//-----------------------------------------------------------------------------
/*
 *  Affect a PID to a descrambler
 *
 *  @param  eDescID         Descrambler ID
 *  @param  u2PID           PID value
 */
//-----------------------------------------------------------------------------
EXTERN MTDMX_DESC_ERROR_T MTDMX_DESC_Set_Descrambler_Pid(MTDMX_DESC_DESCRAMBLER_ID eDescID, UINT16 u2Pid);

//-----------------------------------------------------------------------------
/*
 *  Set the control keys of a stream to descramble
 *   For DES, the keys are the first 8 bytes of key values
 *   For AES, the keys are the first 16 bytes of key values and the CIV are the second 16 bytes of key values
 *
 *  @param  eDescID         Descrambler ID
 *  @param  u2OddKeyLen     Key length
 *  @param  pu1OddKey       Key value
 *  @param  u2EvenKeyLen    Key length
 *  @param  pu1EvenKey      Key value
 */
//-----------------------------------------------------------------------------
EXTERN MTDMX_DESC_ERROR_T MTDMX_DESC_Set_Descrambler_Keys(MTDMX_DESC_DESCRAMBLER_ID eDescID,
                                UINT16 u2OddKeyLen, const UINT8* pu1OddKey,
                                UINT16 u2EvenKeyLen, const UINT8 *pu1EvenKey);

//-----------------------------------------------------------------------------
/*
 *  Set one key of a stream to descramble : odd, even or generic
 *   For DES, the keys are the first 8 bytes of key values
 *   For AES, the keys are the first 16 bytes of key values and the CIV are the second 16 bytes of key values
 *
 *  @param  eDescID         Descrambler ID
 *  @param  eKeyType        Type of the key
 *  @param  u2KeyLen        Key length
 *  @param  pu1Key          Key value
 */
//-----------------------------------------------------------------------------
EXTERN MTDMX_DESC_ERROR_T MTDMX_DESC_Set_Descrambler_Key(MTDMX_DESC_DESCRAMBLER_ID eDescID,
                                MTDMX_DESC_KEYTYPE_T eKeyType, UINT16 u2KeyLen, const UINT8* pu1Key);

//-----------------------------------------------------------------------------
/*
 *  Set the descrambler type of a descrambler
 *
 *  @param  eDescID         ID of descrambler
 *  @param  eKeyType        Type of the descrambler
 */
//-----------------------------------------------------------------------------
EXTERN MTDMX_DESC_ERROR_T MTDMX_DESC_Set_Descrambler_Type(MTDMX_DESC_DESCRAMBLER_ID eDescID, 
                                MTDMX_DESC_DESCTYPE_T eDescType);

//-----------------------------------------------------------------------------
/*
 *  Set the residual termination block(RTB) mode of a descrambler
 *
 *  @param  eDescID         ID of descrambler
 *  @param  eMode           RTB Mode of the descrambler
 */
//-----------------------------------------------------------------------------
EXTERN MTDMX_DESC_ERROR_T MTDMX_DESC_Set_Descrambler_RtbMode(MTDMX_DESC_DESCRAMBLER_ID eDescID, 
    MTDMX_DESC_RTB_MODE_T eMode);

#endif //MT_DMX_DESC_H

