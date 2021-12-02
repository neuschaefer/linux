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

/** @file fpd_mw.c
 *  fpd_mw.c This file implements middleware char devices driver interface.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_os.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_fp_display.h"
#include "x_bim.h"
#include "x_assert.h"
#include "drv_common.h"
#include "drv_name.h"

#include "x_lint.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#if defined(CC_7SEG_HWCTRL)
#define SET_FPD_ENABLE       1
#else
#define SET_FPD_ENABLE       0
#endif

#define SZ_FPD_NAME             DRVN_FP_DISPLAY
#define FPD_BUF_SIZE            8

#define CHAR2DIGIT(ch)      (((ch >= '0') && (ch <= '9')) ? (ch - '0') :        \
                                (((ch >= 'a') && (ch <= 'f')) ? (ch - 'a') :    \
                                (((ch >= 'A') && (ch <= 'F')) ? (ch - 'A') : 0 )))

/*-----------------------------------------------------------------------------
                    global variables.
-----------------------------------------------------------------------------*/
#if SET_FPD_ENABLE
static UINT8 _afgFPD4Point[FPD_BUF_SIZE];
static UINT8 _au1FPD4Digit[FPD_BUF_SIZE];
#endif /* SET_FPD_ENABLE */


/*-----------------------------------------------------------------------------
                    function declaration
-----------------------------------------------------------------------------*/
#if SET_FPD_ENABLE
INT32 MW_FpdSetBuf(UINT32 u4Offset, UINT8 *pu1Digit);
INT32 FPD_DevInit(void);
#endif /* SET_FPD_ENABLE */

INT32 MW_FpdDevInit(void);

/*-----------------------------------------------------------------------------
                    function implementation
-----------------------------------------------------------------------------*/
#if SET_FPD_ENABLE
INT32 MW_FpdSetBuf(UINT32 u4Offset, UINT8 *pu1Digit)
{
    INT32 i;
    INT32 i4Dot, i4Digit, i4Gpio;
    UINT32 u4Val;

    ASSERT(pu1Digit!=NULL);
    ASSERT(u4Offset < 4);

    i4Dot = 0;
    i4Digit = u4Offset;

    x_memset(_au1FPD4Digit, 0, FPD_BUF_SIZE);
    _au1FPD4Digit[0] = '0'; /* for StrToInx heximal */
    _au1FPD4Digit[1] = 'x';
    i4Digit += 2;

    /* Change to lower case. */
    for (i=u4Offset; i4Digit<4; i++) {
        if ((pu1Digit[i] >= 'A') && (pu1Digit[i] <= 'F')) {
            pu1Digit[i] = 'a' + (pu1Digit[i] - 'A');
        }
        if ((pu1Digit[i] >= '0') && (pu1Digit[i] <= '9')) {
            _au1FPD4Digit[i4Digit] = pu1Digit[i];
            _afgFPD4Point[i4Digit] = 0;
            i4Digit++;
        } else if ((pu1Digit[i] >= 'a') && (pu1Digit[i] <= 'f')) {
            _au1FPD4Digit[i4Digit] = pu1Digit[i];
            _afgFPD4Point[i4Digit] = 0;
            i4Digit++;
        } else if (pu1Digit[i]=='.') {
            if (i4Digit > 0) {
                _afgFPD4Point[i4Digit-1] = '.';
                i4Dot++;
            }
        } else {
            _au1FPD4Digit[i4Digit] = ' ';
            _afgFPD4Point[i4Digit] = 0;
            i4Digit++;
        }
    }

    u4Val = StrToInt(_au1FPD4Digit);
    BIM_Set7Seg(u4Val);
    return 0;
}

INT32 FPD_DevInit()
{
    INT32 i;

    for (i=0; i<8; i++) {
        _afgFPD4Point[i] = 0;
        _au1FPD4Digit[i] = 0;
    }
    UNUSED(_afgFPD4Point);
    UNUSED(_au1FPD4Digit);
    return 0;
}
#endif /* SET_FPD_ENABLE */

//lint -save -e818
/*-----------------------------------------------------------------------------
 * Name: _fpd_get_fct
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
static INT32 _fpd_get_fct(
    DRV_COMP_ID_T  *pt_comp_id,
    DRV_GET_TYPE_T  e_get_type,
    VOID           *pv_get_info,
    SIZE_T         *pz_get_info_len)
{
#if SET_FPD_ENABLE
    INT32 i;
    FPD_CONTENT_CHAR_INFO_T *pr_content_info;
#endif
    FPD_MODE_INFO_T *pr_mode_info;
	INT32 i4_ret;
    FPD_CAPABILITY_INFO_T *pr_cap_info;

    UNUSED(pt_comp_id);
    ASSERT(pz_get_info_len!=NULL);
    i4_ret = RMR_DRV_INV_GET_INFO;

    switch(e_get_type) {
    case FPD_GET_TYPE_CAPABILITY:
        if (*pz_get_info_len < sizeof(FPD_CAPABILITY_INFO_T)) {
            *pz_get_info_len = sizeof(FPD_CAPABILITY_INFO_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        ASSERT(pv_get_info!=NULL);
        pr_cap_info = (FPD_CAPABILITY_INFO_T *)pv_get_info;
#if SET_FPD_ENABLE
        pr_cap_info->ui4_level = FPD_CAP_LEVEL_100;
        pr_cap_info->ui4_color = FPD_CAP_COLOR_SINGLE;
#else
        pr_cap_info->ui4_level = 0;
        pr_cap_info->ui4_color = 0;
#endif /* SET_FPD_ENABLE */
        pr_cap_info->ui4_blink_speed = 0;
        pr_cap_info->ui4_circle_right_speed = 0;
        pr_cap_info->ui4_circle_left_speed = 0;
        i4_ret = RMR_OK;
        break;
    case FPD_GET_TYPE_LEVEL:
        if (*pz_get_info_len < sizeof(FPD_LEVEL_T)) {
            *pz_get_info_len = sizeof(FPD_LEVEL_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        ASSERT(pv_get_info!=NULL);
#if SET_FPD_ENABLE
        *((FPD_LEVEL_T *)pv_get_info) = FPD_LEVEL_100;
#else
        *((FPD_LEVEL_T *)pv_get_info) = FPD_LEVEL_0;
#endif /* SET_FPD_ENABLE */
        i4_ret = RMR_OK;
        break;
    case FPD_GET_TYPE_MODE:
        if (*pz_get_info_len < sizeof(FPD_MODE_INFO_T)) {
            *pz_get_info_len = sizeof(FPD_MODE_INFO_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        ASSERT(pv_get_info!=NULL);
#if SET_FPD_ENABLE
        pr_mode_info = (FPD_MODE_INFO_T *)pv_get_info;
        pr_mode_info->e_mode = FPD_MODE_STEADY;
#else
        pr_mode_info = (FPD_MODE_INFO_T *)pv_get_info;
        pr_mode_info->e_mode = (FPD_MODE_T)0;
#endif /* SET_FPD_ENABLE */
        i4_ret = RMR_OK;
        break;
    case FPD_GET_TYPE_COLOR:
        if (*pz_get_info_len < sizeof(FPD_COLOR_T)) {
            *pz_get_info_len = sizeof(FPD_COLOR_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
#if SET_FPD_ENABLE
        ASSERT(pv_get_info!=NULL);
        *((FPD_COLOR_T *)pv_get_info) = FPD_COLOR_SINGLE;
        i4_ret = RMR_OK;
#endif /* SET_FPD_ENABLE */
        break;
    case FPD_GET_TYPE_NUM_OF_LINES:
        if (*pz_get_info_len < sizeof(UINT16)) {
            *pz_get_info_len = sizeof(UINT16);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        ASSERT(pv_get_info!=NULL);
#if SET_FPD_ENABLE
        *((UINT16 *)pv_get_info) = 1;
#else
        *((UINT16 *)pv_get_info) = 0;
#endif /* SET_FPD_ENABLE */
        i4_ret = RMR_OK;
        break;
    case FPD_GET_TYPE_LINE_LEN:
        /* Input is array of SIZE_T, so it should not be less then SIZE_T */
        if (*pz_get_info_len < sizeof(SIZE_T)) {
            *pz_get_info_len = sizeof(SIZE_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        ASSERT(pv_get_info!=NULL);
#if SET_FPD_ENABLE
        *((SIZE_T *)pv_get_info) = 4;
#else
        *((SIZE_T *)pv_get_info) = 0;
#endif /* SET_FPD_ENABLE */
        i4_ret = RMR_OK;
        break;
    case FPD_GET_TYPE_CONTENT:
        if (*pz_get_info_len < sizeof(FPD_CONTENT_CHAR_INFO_T)) {
            *pz_get_info_len = sizeof(FPD_CONTENT_CHAR_INFO_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        ASSERT(pv_get_info!=NULL);
#if SET_FPD_ENABLE
        pr_content_info = (FPD_CONTENT_CHAR_INFO_T *)pv_get_info;
        if (pr_content_info->ui2_line_num != 1) {
            return i4_ret;
        }
        if (pr_content_info->z_offset > 4) {
            return i4_ret;
        }
        for (i = 0; (i + pr_content_info->z_offset) < 4; i++) {
            pr_content_info->ps_content[i] = _au1FPD4Digit[i + pr_content_info->z_offset];
        }
        pr_content_info->ps_content[i] = 0;
        i4_ret = RMR_OK;
#endif /* SET_FPD_ENABLE */
        break;
    default:
        /* unknown set type */
        break;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _fpd_set_fct
 *
 * Description: FrontPanelDisplay set function registered with Resource Manager.
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
static INT32 _fpd_set_fct(
     DRV_COMP_ID_T  *pt_comp_id,
     DRV_SET_TYPE_T  e_set_type,
     const VOID     *pv_set_info,
     SIZE_T          z_set_info_len)
{
	INT32 i4_ret;
#if SET_FPD_ENABLE
    FPD_MODE_INFO_T *pr_mode_info;
    FPD_CLEAR_INFO_T *pr_clear_info;
    FPD_CONTENT_CHAR_INFO_T *pr_content_info;
#else
    UNUSED(pv_set_info);
#endif

    UNUSED(pt_comp_id);
    i4_ret = RMR_DRV_INV_SET_INFO;

    switch(e_set_type) {
    case FPD_SET_TYPE_LEVEL:
        if (z_set_info_len < sizeof(FPD_LEVEL_T)) {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
#if SET_FPD_ENABLE
        ASSERT(pv_set_info!=NULL);
        if (*((FPD_LEVEL_T *)pv_set_info) == FPD_LEVEL_100) {
            i4_ret = RMR_OK;
        }
#endif /* SET_FPD_ENABLE */
        break;
    case FPD_SET_TYPE_MODE:
        if (z_set_info_len < sizeof(FPD_MODE_INFO_T)) {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
#if SET_FPD_ENABLE
        ASSERT(pv_set_info!=NULL);
        pr_mode_info = (FPD_MODE_INFO_T *)pv_set_info;
        if (pr_mode_info->e_mode == FPD_MODE_STEADY) {
            i4_ret = RMR_OK;
        }
#endif /* SET_FPD_ENABLE */
        break;
    case FPD_SET_TYPE_COLOR:
        if (z_set_info_len < sizeof(FPD_COLOR_T)) {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
#if SET_FPD_ENABLE
        ASSERT(pv_set_info!=NULL);
        if (*((FPD_COLOR_T *)pv_set_info) == FPD_COLOR_SINGLE) {
            i4_ret = RMR_OK;
        }
#endif /* SET_FPD_ENABLE */
        break;
    case FPD_SET_TYPE_CONTENT:
        if (z_set_info_len < sizeof(FPD_CONTENT_CHAR_INFO_T)) {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
#if SET_FPD_ENABLE
        ASSERT(pv_set_info!=NULL);
        pr_content_info = (FPD_CONTENT_CHAR_INFO_T *)pv_set_info;
        if (pr_content_info->ui2_line_num != 1) {
            return i4_ret;
        }
        if (pr_content_info->z_offset >= 4) {
            return i4_ret;
        }

        if (0!=MW_FpdSetBuf(pr_content_info->z_offset, (UINT8 *)pr_content_info->ps_content)) {
            return i4_ret;
        }
        i4_ret = RMR_OK;
#endif /* SET_FPD_ENABLE */
        break;
    case FPD_SET_TYPE_CLEAR:
        if (z_set_info_len < sizeof(FPD_CLEAR_INFO_T)) {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
#if SET_FPD_ENABLE
        ASSERT(pv_set_info!=NULL);
        pr_clear_info = (FPD_CLEAR_INFO_T *)pv_set_info;
        if (pr_clear_info->ui2_line_num != 1) {
            return i4_ret;
        }
        if (pr_clear_info->z_offset > 4) {
            return i4_ret;
        }
        if (pr_clear_info->z_len > 4) {
            return i4_ret;
        }
        if (pr_clear_info->z_offset + pr_clear_info->z_len > 4) {
            return i4_ret;
        }
        if (0!=MW_FpdSetBuf(pr_clear_info->z_offset, "    ")) {
            return i4_ret;
        }
        i4_ret = RMR_OK;
#endif /* SET_FPD_ENABLE */
        break;
    default:
        /* unknown set type */
        break;
    }

    return i4_ret;
}
//lint -restore

/*-----------------------------------------------------------------------------
 * Name: MW_FpdDevInit()
 *
 * Description: Front Panel Display device initial function.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: RMR_OK
 *              Success.
 *          RMR_DRV_ERROR
 *              Errors.
 ----------------------------------------------------------------------------*/
INT32 MW_FpdDevInit()
{
    DRV_COMP_REG_T      t_reg_comp;
    DRV_COMP_FCT_TBL_T  t_comp_fct_tbl;
    CHAR ps_name[32];
    INT32 i4_ret;

#if SET_FPD_ENABLE
    i4_ret = FPD_DevInit();
    if (i4_ret != 0)
    {
        return RMR_DRV_ERROR;
    }
#endif /* SET_FPD_ENABLE */

//lint -save -e506 -e774
	if (SZ_FPD_NAME)
	{
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
	    x_snprintf(ps_name, sizeof(ps_name), "%s%d", SZ_FPD_NAME, 0);
	}
//lint -restore

    t_reg_comp.e_type = DRVT_FP_DISPLAY;
    t_reg_comp.e_id_type            = ID_TYPE_IND;
    t_reg_comp.u.t_ind.ui2_id       = 0;
    t_reg_comp.u.t_ind.ui1_port     = 0;
    t_reg_comp.u.t_ind.pv_tag       = 0;

    t_comp_fct_tbl.pf_rm_connect    = NULL;
    t_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_comp_fct_tbl.pf_rm_get        = _fpd_get_fct;
    t_comp_fct_tbl.pf_rm_set        = _fpd_set_fct;

//lint -save -e645 -e506
    i4_ret = x_rm_reg_comp(&t_reg_comp,
                           1,               /* num in port */
                           1,               /* num out port */
                           (DRVN_FP_DISPLAY == NULL) ? NULL : ps_name,	//ps_name,
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

