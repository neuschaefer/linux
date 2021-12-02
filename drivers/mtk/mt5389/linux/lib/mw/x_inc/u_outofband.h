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
 * $RCSfile: u_outofband.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/3 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 7073777ef840de31d560641fd88f5e7f $
 *
 * Description: 
 *         This header file contains the Ouf-of-band Connection Handler
 *         specific definitions that are exported.
 *---------------------------------------------------------------------------*/

#ifndef _U_OUTOFBAND_H_
#define _U_OUTOFBAND_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_cm.h"
#include "u_handle.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define OUTOFBAND_HANDLER_DEFAULT_NAME "outofband"

/* Connection command codes */
#define OUTOFBAND_CONN_OFFSET                  ((CM_COMMAND_CODE_T) 0x80010000)
#define OUTOFBAND_CONN_SIGNAL_LOST_DISCONNECT  (OUTOFBAND_CONN_OFFSET +  0)
#define OUTOFBAND_CONN_DELIVERY_SYS            (OUTOFBAND_CONN_OFFSET +  1)
#define OUTOFBAND_CONN_FREQUENCY               (OUTOFBAND_CONN_OFFSET +  2)
#define OUTOFBAND_CONN_MODULATION              (OUTOFBAND_CONN_OFFSET +  3)
#define OUTOFBAND_CONN_DATA_RATE               (OUTOFBAND_CONN_OFFSET +  4)
#define OUTOFBAND_CONN_OUTPUT_POWER            (OUTOFBAND_CONN_OFFSET +  5)
#define OUTOFBAND_CONN_SPECTRUM_INV            (OUTOFBAND_CONN_OFFSET +  6)
#define OUTOFBAND_CONN_DISC_IF_COMP_BUSY       (OUTOFBAND_CONN_OFFSET +  7)

/* Out-of-band-specific commands to use with x_cm_get */
#define OUTOFBAND_CTRL_GET_SIGNAL_LEVEL \
(CM_CTRL_HANDLER_PRIVATE_OFFSET + 0)
#define OUTOFBAND_CTRL_GET_DBM_SIGNAL_LEVEL \
(CM_CTRL_HANDLER_PRIVATE_OFFSET + 1)

/* Disconnection reasons */
#define OUTOFBAND_DISC_AS_REQUESTED                    0U
#define OUTOFBAND_DISC_OTHER_REASON                    1U
#define OUTOFBAND_DISC_NEW_CONNECTION                  2U
#define OUTOFBAND_DISC_NO_COMP_FOUND                   3U
#define OUTOFBAND_DISC_COMP_BUSY                       4U
#define OUTOFBAND_DISC_COULD_NOT_SET_COMP_PROPERTIES   5U
#define OUTOFBAND_DISC_COULD_NOT_CONNECT_COMP          6U
#define OUTOFBAND_DISC_TUNER_SIGNAL_LOST               7U
#define OUTOFBAND_DISC_TUNER_OUT_OF_RANGE              8U

#endif /*  _U_OUTOFBAND_H_ */
