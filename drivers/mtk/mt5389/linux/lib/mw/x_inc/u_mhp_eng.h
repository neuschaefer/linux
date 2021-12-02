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
 * $RCSfile: u_mhp_eng.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains MHP engine specific definitions, which
 *         are exported.
 *------------------------------------------------------------------------*/

#ifndef _U_MHP_H_
#define _U_MHP_H_


/*--------------------------------------------------------------------------
                    include files
--------------------------------------------------------------------------*/

#include "u_handle_grp.h"
#include "u_common.h"

/*--------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 -------------------------------------------------------------------------*/

/* Default MHP engine name. */
#define MHP_DEFAULT_NAME  "mhp_engine"

/* MHP engine handle types. */
#define TMT_MHP_AIT      (TMT_MHP + ((HANDLE_TYPE_T)  0))
#define TMT_MHP_XAIT     (TMT_MHP + ((HANDLE_TYPE_T)  1))

/* MHP engine get operations. */
#define MHP_GET_TYPE_APP_ENTRY  (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 0))
#define MHP_GET_TYPE_APP_TYPE   (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 1))
#define MHP_GET_TYPE_TEST       (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 2))

/* MHP engine load types. */
#define MHP_LOAD_TYPE_PID_APP_TYPE_VER ((TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 0))|TM_LOAD_TYPE_SINGLE_SRC)
#define MHP_LOAD_TYPE_PID_APP_TYPE ((TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 1))|TM_LOAD_TYPE_SINGLE_SRC)

/* Application Types */
#define DVB_J_APP    1
#define DVB_HTML_APP 2

/* Application Control Codes */
#define APP_CC_AUTOSTART 1
#define APP_CC_PRESENT   2
#define APP_CC_DESTROY   3
#define APP_CC_KILL      4
#define APP_CC_PREFETCH  5
#define APP_CC_REMOTE    6

/* Holds a stream type and PID value. */
typedef struct _MHP_APP_INFO_T
{
    UINT32 ui4_org_id;
    UINT16 ui2_app_id;
    UINT8  ui1_control_code;
}   MHP_APP_INFO_T;

/* for loading AIT */
typedef struct _MHP_PID_APP_TYPE_VER_T
{
    MPEG_2_PID_T t_pid;
    TS_DESCR_T*  pt_ts_descr;
    UINT16       ui2_app_type;
    UINT8        ui1_version;
} MHP_PID_APP_TYPE_VER_T;

/* MHP engine configuration structure. */
typedef struct _MHP_CONFIG_T
{
    UINT8  ui1_num_ait_filters;

    SIZE_T  z_ait_buff_size;

    UINT16  ui2_num_ait_tbl_objs;
}   MHP_CONFIG_T;

#endif /* _U_MHP_H_ */
