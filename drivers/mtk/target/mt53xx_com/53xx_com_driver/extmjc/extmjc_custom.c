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
 * $RCSfile: extmjc_custom.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

#ifdef DRV_SUPPORT_EXTMJC

#include "extmjc_if.h"
//#include "u_drv_cust.h"

#include "extmjc_msg_hdlr.h"
//#include "extmjc_prc.h"
#include "extmjc_debug.h"
#include "drvcust_if.h"
#include "x_pdwnc.h"
#include "c_model.h"  // for SGP error code

extern UINT32 gui4_extmjc_ping_failed_cnt;
extern UINT32 gui4_extmjc_ping_okay_cnt;

#ifdef DRV_EXTMJC_CUST_SGP
#define _u4ModeAliveDisable     (*((UINT32 *)0x20028144))
#define GPIO1_MODE                  (1U << 0)
#define GPIO2_MODE                  (1U << 1)
#define KICKED_MODE                 (1U << 2)
#define SDM_MODE                    (1U << 3)
#define PANEL_MODE                  (1U << 4)
#define FACTORY_MODE                (1U << 5)
#define CSM_MODE                    (1U << 6)
#define SAM_MODE                    (1U << 7)
#define USB_MODE                    (1U << 8)
#define KICKED8032_MODE             (1U << 9)
#define KICKED8297_MODE             (1U << 10)
#define KICKED8281_MODE             (1U << 11)

#define MT8282_KICK_MODE (KICKED_MODE | KICKED8032_MODE | KICKED8297_MODE | KICKED8281_MODE)
#define MT8282_NON_KICK_MODE (GPIO1_MODE|GPIO2_MODE|SDM_MODE|PANEL_MODE|FACTORY_MODE|CSM_MODE|SAM_MODE|USB_MODE)

#define EXTMJC_PING_EXPIRE_TIMES 3 //15 seconds
#define EXTMJC_PING_OK_TIMES     100 //100x5 seconds

VOID extmjc_ping_fail_handler(VOID)
{
	Printf("EXTMJC_MSG_TYPE_PING_FAIL\n");
    if (!_u4ModeAliveDisable)
    {
        PDWNC_WriteErrorCode(ERR_I2C_MT8282);
        Printf("EXTMJC:reboot\n");
        PDWNC_Reboot();
    }
    else if ((_u4ModeAliveDisable & MT8282_KICK_MODE) &&
            ((_u4ModeAliveDisable & MT8282_NON_KICK_MODE) ==0) &&
            (gui4_extmjc_ping_okay_cnt >= EXTMJC_PING_OK_TIMES))
    {
        PDWNC_WriteErrorCode(ERR_I2C_MT8282);
        Printf("EXTMJC:reboot\n");
        PDWNC_Reboot();
    }
    else
    {
        gui4_extmjc_ping_okay_cnt=0;
    }
}

VOID extmjc_ping_handler(VOID)
{
    UINT8 u1MsgErr;
    if (d_custom_extmjc_get_alive_check() == TRUE)
    {
        drv_extmjc_set_ping(&u1MsgErr);
        if ((_u4ModeAliveDisable & MT8282_KICK_MODE) &&
            ((_u4ModeAliveDisable & MT8282_NON_KICK_MODE) ==0) &&
            (gui4_extmjc_ping_okay_cnt == EXTMJC_PING_OK_TIMES))
        {
            Printf("EXTMJC:ready for rekick\n");
        }

        if (gui4_extmjc_ping_failed_cnt > EXTMJC_PING_EXPIRE_TIMES)
        {
            extmjc_ping_fail_handler();
        }
        else
        {
            if (gui4_extmjc_ping_failed_cnt==0)
            {
                //Printf("Ping ok count =%d EXTMJC_PING_OK_TIMES=%d\n",gui4_extmjc_ping_okay_cnt,EXTMJC_PING_OK_TIMES);
                gui4_extmjc_ping_okay_cnt++;
            }
        }
    }
}


VOID extmjc_send_cmd_fail_handler(VOID)
{
    PDWNC_WriteErrorCode(ERR_I2C_MT8282);
}

#else
VOID extmjc_ping_fail_handler(VOID)
{
    return;
}
VOID extmjc_ping_handler(VOID)
{
}
VOID extmjc_send_cmd_fail_handler(VOID)
{
    return;
}

#endif

#endif /* DRV_SUPPORT_EXTMJC */

