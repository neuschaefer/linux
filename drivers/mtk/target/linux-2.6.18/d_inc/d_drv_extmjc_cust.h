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
 * $RCSfile: d_drv_extmjc_cust.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *---------------------------------------------------------------------------*/

#ifndef _D_DRV_EXTMJC_CUST_H_
#define _D_DRV_EXTMJC_CUST_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define DRV_CUSTOM_EXTMJCR_OK                       ((INT32)     0)
#define DRV_CUSTOM_EXTMJCR_INV_ARG                  ((INT32)     -1)
#define DRV_CUSTOM_EXTMJCR_TX_FAIL                  ((INT32)     -2)
#define DRV_CUSTOM_EXTMJCR_INV_RESP                 ((INT32)     -3)
#define DRV_CUSTOM_EXTMJCR_REG_FAIL                 ((INT32)     -4)
#define DRV_CUSTOM_EXTMJCR_CONNECT_NOT_READY        ((INT32)     -5)

/* Version */
typedef struct DRV_CUSTOM_EXTMJC_VERSION
{
    UINT8       ui1_size;
    UINT8       ui1_len;
    UINT8*      pui1_version;
} DRV_CUSTOM_EXTMJC_VERSION_T;

typedef struct DRV_CUSTOM_EXTMJC_WINDOW_POSITION
{
    UINT16      ui2_left_top_x;
    UINT16      ui2_left_top_y;
    UINT16      ui2_right_bottom_x;
    UINT16      ui2_right_bottom_y;
} DRV_CUSTOM_EXTMJC_WINDOW_POSITION_T;

typedef enum DRV_CUSTOM_EXTMJC_COLOR_PATTERN
{
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_RED = 0,
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_GREEN = 1,
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_BLUE = 2,
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_WHITE = 3,
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_NONE = 0xFF
} DRV_CUSTOM_EXTMJC_COLOR_PATTERN_T;

#define DRV_CUSTOM_EXTMJC_OSD_MODE_NORMAL       0
#define DRV_CUSTOM_EXTMJC_OSD_MODE_DMA          1
#define DRV_CUSTOM_EXTMJC_OSD_MODE_GRAPHIC      2

#define DRV_CUSTOM_EXTMJC_DEMO_MODE_NORMAL      0
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_LEFT_MJC    1
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_RIGHT_MJC   2
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_BOTH_MJC    3
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_LEFT_DNM    4
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_RIGHT_DNM   5
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_BOTH_DNM    6
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_DISABLE     7

#define DRV_CUSTOM_EXTMJC_SETTING_EFFECT_OFF    0
#define DRV_CUSTOM_EXTMJC_SETTING_EFFECT_LOW    1
#define DRV_CUSTOM_EXTMJC_SETTING_EFFECT_MIDDLE 2
#define DRV_CUSTOM_EXTMJC_SETTING_EFFECT_HIGH   3

#define DRV_CUSTOM_EXTMJC_SETTING_60            0
#define DRV_CUSTOM_EXTMJC_SETTING_120           1

#define DRV_CUSTOM_EXTMJC_SETTING_DEMO_OFF      0
#define DRV_CUSTOM_EXTMJC_SETTING_DEMO_RIGHT    1
#define DRV_CUSTOM_EXTMJC_SETTING_DEMO_LEFT     2

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

extern BOOL d_custom_extmjc_is_enable(VOID);

extern VOID d_custom_extmjc_enable_alive_check(BOOL b_enable);

extern BOOL d_custom_extmjc_get_alive_check(VOID);


extern INT32 d_custom_extmjc_get_version(
    UINT8*                                  pui1_msg_error,
    DRV_CUSTOM_EXTMJC_VERSION_T*            pt_extmjc_version
    );

extern INT32 d_custom_extmjc_set_mjc_setting(
                                    UINT8       ui1_mjc_effect,
                                    UINT8       ui1_60_120,
                                    UINT8       ui1_demo_mode,
                                    UINT8*      pui1_msg_error
                                    );

extern INT32 d_custom_extmjc_set_osd_mode(
    UINT8               ui1_osd_mode,
    UINT8*                                  pui1_msg_error
    );

extern INT32 d_custom_extmjc_set_window_position(
    DRV_CUSTOM_EXTMJC_WINDOW_POSITION_T*    pat_window_position,
    UINT8                                   ui1_window_num,
    UINT8*                                  pui1_msg_error
    );

extern INT32 d_custom_extmjc_set_color_pattern(
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_T       t_color_pattern,
    UINT8*                                  pui1_msg_error
    );

extern INT32 d_custom_extmjc_get_gpio(
    UINT8                                   ui1_gpio_id,
    BOOL*                                   pb_onoff,
    UINT8*                                  pui1_msg_error
    );

extern INT32 d_custom_extmjc_set_gpio(
    UINT8                                   ui1_gpio_id,
    BOOL                                    b_onoff,
    UINT8*                                  pui1_msg_error
    );

extern INT32 d_custom_extmjc_get_dram(
    UINT32                                  ui4_addr,
    UINT32*                                 pui4_data,
    UINT8*                                  pui1_msg_error
    );

extern INT32 d_custom_extmjc_set_dram(
    UINT32                                  ui4_addr,
    UINT32                                  ui4_data,
    UINT8*                                  pui1_msg_error
    );

extern INT32 d_custom_extmjc_set_demo_mode(
    UINT8                                   ui1_demo_mode,
    UINT16                                  ui2_middle_line_pos,
    UINT8*                                  pui1_msg_error
    );

extern INT32 d_custom_extmjc_upgrade_open(VOID);


extern INT32 d_custom_extmjc_upgrade_erase(VOID);

/* The maximun data size is 128 */
extern INT32 d_custom_extmjc_upgrade_write(
    UINT32                                  ui4_offset,
    UINT8*                                  pui1_data,
    UINT32                                  ui4_data_size
    );

extern INT32 d_custom_extmjc_upgrade_close(VOID);

#endif /* _D_DRV_EXTMJC_CUST_H_ */
