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
 * Description: This file implements middleware char devices driver interface.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_typedef.h"
#include "drvcust_if.h"
#include "x_os.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_indicator.h"
#include "x_pinmux.h"
#include "drv_name.h"
#include "x_lint.h"
#include "x_assert.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define SZ_POWER_IND_NAME            DRVN_IND_POWER
#define SZ_PLAYBACK_IND_NAME         DRVN_IND_PLAYBACK
#define SZ_RECORD_IND_NAME           DRVN_IND_RECORD
#define SZ_FORWARD_IND_NAME          DRVN_IND_FORWARD
#define SZ_REWIND_IND_NAME           DRVN_IND_REWIND
#define SZ_PAUSE_IND_NAME            DRVN_IND_PAUSE
#define SZ_MAIL_IND_NAME             DRVN_IND_MAIL
#define SZ_REMINDER_IND_NAME         DRVN_IND_REMINDER

#define IND_POWER_ID                    1
#define IND_PLAYBACK_ID                 2
#define IND_RECORD_ID                   3
#define IND_FORWARD_ID                  4
#define IND_REWIND_ID                   5
#define IND_PAUSE_ID                    6
#define IND_MAIL_ID                     7
#define IND_REMINDER_ID                 8

// tentatively, change to OPCTRL(0) soon
#define GPIO_DONE                       DRVCUST_OptGet(ePowerLedGpio)

/*-----------------------------------------------------------------------------
                        function declaration
-----------------------------------------------------------------------------*/
INT32 MW_IndctrDevInit(void);

//lint -save -e641 -e818
/*-----------------------------------------------------------------------------
 * Name: _indctr_get_fct
 *
 * Description: indicator get function registered with Resource Manager.
 *
 * Inputs:  See Resource Manager specification.
 *
 * Outputs: -
 *
 * Returns: RMR_OK
 *              Success.
 *          RMR_DRV_NOT_ENOUGH_SPACE
 *              The given buffer does not have enough space.
 *          RMR_DRV_INV_GET_INFO
 *              Other errors.
 ----------------------------------------------------------------------------*/
static INT32 _indctr_get_fct(
    DRV_COMP_ID_T  *pt_comp_id,
    DRV_GET_TYPE_T  e_get_type,
    VOID           *pv_get_info,
    SIZE_T         *pz_get_info_len)
{
    INT32 i4_ret;
    IND_CAPABILITY_INFO_T *pr_cap_info;
    IND_MODE_INFO_T *pr_mode_info;

    ASSERT(pt_comp_id!=NULL);
    ASSERT(pz_get_info_len!=NULL);
    i4_ret = RMR_DRV_INV_GET_INFO;

    switch(e_get_type) {
    case IND_GET_TYPE_CAPABILITY:
        if (*pz_get_info_len < sizeof(IND_CAPABILITY_INFO_T)) {
            *pz_get_info_len = sizeof(IND_CAPABILITY_INFO_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        ASSERT(pv_get_info!=NULL);
        pr_cap_info = (IND_CAPABILITY_INFO_T *)pv_get_info;
        pr_cap_info->ui4_state = 0;
        pr_cap_info->ui4_color = 0;
        pr_cap_info->ui4_blink_speed = 0;
        pr_cap_info->ui4_circle_fwd_speed = 0;
        pr_cap_info->ui4_circle_rev_speed = 0;
        if (pt_comp_id->ui2_id == IND_POWER_ID) {
            pr_cap_info->ui4_state = (UINT32)(IND_CAP_STATE_DISABLE | IND_CAP_STATE_ENABLE);
            pr_cap_info->ui4_color = (UINT32)(IND_CAP_COLOR_SINGLE);
        }
        i4_ret = RMR_OK;
        break;
    case IND_GET_TYPE_STATE:
        if (*pz_get_info_len < sizeof(IND_STATE_T)) {
            *pz_get_info_len = sizeof(IND_STATE_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        if (pt_comp_id->ui2_id == IND_POWER_ID) {
            ASSERT(pv_get_info!=NULL);
            if (DRVCUST_OptGet(ePowerLedGpio) == 0)
            {
                *((IND_STATE_T *)pv_get_info) =
                    (GPIO_Output(GPIO_DONE, NULL)==(DRVCUST_OptGet(ePowerLedOnVal))) ? IND_STATE_ENABLE : IND_STATE_DISABLE;
            }
            else
            {
                *((IND_STATE_T *)pv_get_info) = IND_STATE_DISABLE;
            }
            i4_ret = RMR_OK;
        }
        break;
    case IND_GET_TYPE_MODE:
        if (*pz_get_info_len < sizeof(IND_MODE_INFO_T)) {
            *pz_get_info_len = sizeof(IND_MODE_INFO_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        ASSERT(pv_get_info!=NULL);
        pr_mode_info = (IND_MODE_INFO_T *)pv_get_info;
        if (pt_comp_id->ui2_id == IND_POWER_ID) {
            pr_mode_info->e_mode = IND_MODE_STEADY;
            i4_ret = RMR_OK;
        }
        break;
    case IND_GET_TYPE_COLOR:
        if (*pz_get_info_len < sizeof(IND_COLOR_T)) {
            *pz_get_info_len = sizeof(IND_COLOR_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        if (pt_comp_id->ui2_id == IND_POWER_ID) {
            ASSERT(pv_get_info!=NULL);
            *((IND_COLOR_T *)pv_get_info) = IND_COLOR_SINGLE;
            i4_ret = RMR_OK;
        }
        break;
    default:
        /* unknown set type */
        break;
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _indctr_set_fct
 *
 * Description: indicator set function registered with Resource Manager.
 *
 * Inputs:  See Resource Manager specification.
 *
 * Outputs: -
 *
 * Returns: RMR_OK
 *              Success.
 *          RMR_DRV_NOT_ENOUGH_SPACE
 *              The given buffer does not have enough space.
 *          RMR_DRV_INV_GET_INFO
 *              Other errors.
 ----------------------------------------------------------------------------*/
static INT32 _indctr_set_fct(
     DRV_COMP_ID_T  *pt_comp_id,
     DRV_SET_TYPE_T  e_set_type,
     const VOID     *pv_set_info,
     SIZE_T          z_set_info_len)
{
    INT32 i4_ret, i4_state;
    IND_MODE_INFO_T *pr_mode_info;

    ASSERT(pt_comp_id!=NULL);
    i4_ret = RMR_DRV_INV_SET_INFO;

    switch(e_set_type) {
    case IND_SET_TYPE_STATE:
        if (z_set_info_len < sizeof(IND_STATE_T)) {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        if (pt_comp_id->ui2_id == IND_POWER_ID) {
            ASSERT(pv_set_info!=NULL);
            switch (*((IND_STATE_T *)pv_set_info)) {
            case IND_STATE_DISABLE:
                i4_state = !DRVCUST_OptGet(ePowerLedOnVal);
                LINT_SUPPRESS_NEXT_EXPRESSION(534);
                if (DRVCUST_OptGet(ePowerLedGpio) == 0)
                {
                    GPIO_Output(GPIO_DONE, &i4_state);
                }
                i4_ret = RMR_OK;
                break;
            case IND_STATE_ENABLE:
                i4_state = DRVCUST_OptGet(ePowerLedOnVal);
                LINT_SUPPRESS_NEXT_EXPRESSION(534);
                if (DRVCUST_OptGet(ePowerLedGpio) == 0)
                {
                    GPIO_Output(GPIO_DONE, &i4_state);
                }
                i4_ret = RMR_OK;
                break;
            default:
                break;
            }
        }
        break;
    case IND_SET_TYPE_MODE:
        if (z_set_info_len < sizeof(IND_MODE_INFO_T)) {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        ASSERT(pv_set_info!=NULL);
        pr_mode_info = (IND_MODE_INFO_T *)pv_set_info;
        if (pt_comp_id->ui2_id == IND_POWER_ID) {
            switch (pr_mode_info->e_mode) {
            case IND_MODE_STEADY:
                i4_ret = RMR_OK;
                break;
            default:
                break;
            }
        }
        break;
    case IND_SET_TYPE_COLOR:
        if (z_set_info_len < sizeof(IND_COLOR_T)) {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        ASSERT(pv_set_info!=NULL);
        if ((pt_comp_id->ui2_id == IND_POWER_ID) &&
            ((*((IND_COLOR_T *)pv_set_info)) == IND_COLOR_SINGLE)) {
            i4_ret = RMR_OK;
        }
        break;
    default:
        /* unknown set type */
        break;
    }

    return i4_ret;
}
//lint -restore

static INT32 _MW_IndctrDevRegister(CHAR *szIndName, DRV_TYPE_T drvType, UINT16 ui2Id)
{
    DRV_COMP_REG_T      t_reg_comp;
    DRV_COMP_FCT_TBL_T  t_comp_fct_tbl;
    CHAR ps_name[32];
    INT32 i4_ret;

    if (ui2Id == IND_POWER_ID) {
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
    }

	if (szIndName)
	{
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
	    x_snprintf(ps_name, sizeof(ps_name), "%s%d", szIndName, ui2Id);
	}

    t_reg_comp.e_type = drvType;
    t_reg_comp.e_id_type            = ID_TYPE_IND;
    t_reg_comp.u.t_ind.ui2_id       = ui2Id;
    t_reg_comp.u.t_ind.ui1_port     = 0;
    t_reg_comp.u.t_ind.pv_tag       = (void *)((UINT32)drvType);

    t_comp_fct_tbl.pf_rm_connect    = NULL;
    t_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_comp_fct_tbl.pf_rm_get        = _indctr_get_fct;
    t_comp_fct_tbl.pf_rm_set        = _indctr_set_fct;

//lint -save -e645
    i4_ret = x_rm_reg_comp(&t_reg_comp,
                           1,               /* num in port */
                           1,               /* num out port */
                           (szIndName == NULL) ? NULL : ps_name,
                           0,               /* ui4_flags */
                           &t_comp_fct_tbl,
                           NULL,
                           0);
//lint -restore
    if (i4_ret != RMR_OK)
    {
        return RMR_DRV_ERROR;
    }

    return RMR_OK;
}

INT32 MW_IndctrDevInit()
{
    INT32 i4_ret, i4Ret;

    i4Ret = RMR_OK;
    i4_ret = _MW_IndctrDevRegister(SZ_POWER_IND_NAME, DRVT_IND_POWER, IND_POWER_ID);
//lint -save -e774
    if (i4_ret && (i4Ret==RMR_OK)) { i4Ret = i4_ret; }
//lint -restore
    i4_ret = _MW_IndctrDevRegister(SZ_PLAYBACK_IND_NAME, DRVT_IND_PLAYBACK, IND_PLAYBACK_ID);
    if (i4_ret && (i4Ret==RMR_OK)) { i4Ret = i4_ret; }
    i4_ret = _MW_IndctrDevRegister(SZ_RECORD_IND_NAME, DRVT_IND_RECORD, IND_RECORD_ID);
    if (i4_ret && (i4Ret==RMR_OK)) { i4Ret = i4_ret; }
    i4_ret = _MW_IndctrDevRegister(SZ_FORWARD_IND_NAME, DRVT_IND_FORWARD, IND_FORWARD_ID);
    if (i4_ret && (i4Ret==RMR_OK)) { i4Ret = i4_ret; }
    i4_ret = _MW_IndctrDevRegister(SZ_REWIND_IND_NAME, DRVT_IND_REWIND, IND_REWIND_ID);
    if (i4_ret && (i4Ret==RMR_OK)) { i4Ret = i4_ret; }
    i4_ret = _MW_IndctrDevRegister(SZ_PAUSE_IND_NAME, DRVT_IND_PAUSE, IND_PAUSE_ID);
    if (i4_ret && (i4Ret==RMR_OK)) { i4Ret = i4_ret; }
    i4_ret = _MW_IndctrDevRegister(SZ_MAIL_IND_NAME, DRVT_IND_MAIL, IND_MAIL_ID);
    if (i4_ret && (i4Ret==RMR_OK)) { i4Ret = i4_ret; }
    i4_ret = _MW_IndctrDevRegister(SZ_REMINDER_IND_NAME, DRVT_IND_REMINDER, IND_REMINDER_ID);
    if (i4_ret && (i4Ret==RMR_OK)) { i4Ret = i4_ret; }
    return i4Ret;
}


