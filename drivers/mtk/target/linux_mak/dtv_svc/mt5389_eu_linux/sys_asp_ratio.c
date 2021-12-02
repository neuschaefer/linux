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
 * $RCSfile: sys_asp_ratio.c,v $
 * $Revision: #3 $
 * $Date: 2012/05/31 $
 * $Author: jifeng.fan $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Chris Kuo $
 * $MD5HEX: 72ec35f463230d3f1747ea7bd18764a5 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "x_os.h"
#include "u_common.h"
#include "u_sm_video_hdlr.h"
#include "x_vid_plane.h"
#include "d_drv_cust.h"
#include "u_scc.h"
#ifdef SYS_MHEG5_SUPPORT
#include "x_mheg5.h"
#endif
#include "app_util/a_cfg.h"
#include "app_util/a_tv.h"

/*-----------------------------------------------------------------------------
 * Variable declarations
 *---------------------------------------------------------------------------*/
#define  REGION_RATIO_WIDTH      VSH_REGION_MAX_WIDTH / 10000
#define  REGION_RATIO_HEIGHT     VSH_REGION_MAX_HEIGHT / 10000

#if (VIEWPORT_SUPPORT == 1)
static APP_CFG_APP_STATUS_T _e_app_status = APP_CFG_APP_STATUS_LAST_ENTRY;
static APP_CFG_VIEWPORT_T _t_crnt_viewport;
static TV_MODE_T _e_crnt_tv_mode = TV_MODE_TYPE_NORMAL;
static BOOL _b_is_3d_mode_enabled = FALSE;
static BOOL _b_is_skip_viewport_func = FALSE;
#endif

#define _SET_REGION(pt_reg, x, y, w, h)                    \
    do {                                                   \
        (pt_reg)->ui4_x      = x;                          \
        (pt_reg)->ui4_y      = y;                          \
        (pt_reg)->ui4_width  = w;                          \
        (pt_reg)->ui4_height = h;                          \
    } while (0)

#define _COPY_REGION(pt_to_reg, pt_from_reg)                 \
    do {                                                     \
        (pt_to_reg)->ui4_x      = (pt_from_reg)->ui4_x;      \
        (pt_to_reg)->ui4_y      = (pt_from_reg)->ui4_y;      \
        (pt_to_reg)->ui4_width  = (pt_from_reg)->ui4_width;  \
        (pt_to_reg)->ui4_height = (pt_from_reg)->ui4_height; \
    } while (0)

#define _IS_REGION_EMPTY(pt_reg)                             \
(                                                            \
    (((pt_reg)->ui4_x == 0) &&                               \
     ((pt_reg)->ui4_y == 0) &&                               \
     ((pt_reg)->ui4_width == 0) &&                           \
     ((pt_reg)->ui4_height == 0)) ? TRUE : FALSE             \
)

/* Apect ratio modes */
typedef enum
{
    ASPECT_RATIO_MODE_NO_VIDEO   = 0,   /* unknown (fit) */
    ASPECT_RATIO_MODE_NORMAL,           /* normal (fit) */
    ASPECT_RATIO_MODE_LETTERBOX,        /* letterbox */
    ASPECT_RATIO_MODE_PAN_SCAN,         /* pan & scan (zoom) */
    ASPECT_RATIO_MODE_PANORAMA,         /* non-linear (panoramic) */
    ASPECT_RATIO_MODE_DOT_BY_DOT,       /* dot by dot */
    ASPECT_RATIO_MODE_AUTO              /* auto */
}   ASPECT_RATIO_MODE_T;

/* Aspect ratios */
typedef enum
{
    ASPECT_RATIO_4_3             = 0,   /* 4:3 */
    ASPECT_RATIO_16_10,                 /* 16:10 */
    ASPECT_RATIO_16_9,                  /* 16:9 */
    ASPECT_RATIO_2_21_1,                /* 2.21:1 */
    ASPECT_RATIO_UNKNOWN                /* unknown */
}   ASPECT_RATIO_T;

#ifdef SYS_MHEG5_SUPPORT
static BOOL _asp_ratio_mheg5_fct(
    const DRV_CUSTOM_RESOLUTION_INFO_T* pt_src_res,
    const DRV_CUSTOM_VID_SRC_INFO_T*    pt_vid_src_info,
    BOOL                                b_auto_mode,
    VID_PLA_VID_REGION_T*               pt_src_reg,
    VID_PLA_VID_REGION_T*               pt_dst_reg);

static BOOL _asp_ratio_mheg5_fct_ex(
       const DRV_CUSTOM_RESOLUTION_INFO_T* pt_src_res,
       const DRV_CUSTOM_VID_SRC_INFO_T*    pt_vid_src_info,
       const VID_PLA_VID_REGION_T*         pt_src_reg,
       const VID_PLA_VID_REGION_T*         pt_dst_reg);
#endif

/* Variables to record last WSS */
static UINT16 ui2_g_wss_main = 0, ui2_g_wss_sub = 0;
/* Variables to record last SCART pin 8 */
static DRV_CUSTOM_SCART_PIN_8_T e_g_scart_pin_8_main = DRV_CUSTOM_SCART_PIN_8_UNKNOWN;
static DRV_CUSTOM_SCART_PIN_8_T e_g_scart_pin_8_sub  = DRV_CUSTOM_SCART_PIN_8_UNKNOWN;

typedef enum
{
    ASP_SRC_TYPE_CVBS = 0,
    ASP_SRC_TYPE_COMPONENT,
    ASP_SRC_TYPE_HDMI,
    ASP_SRC_TYPE_DVI,
    ASP_SRC_TYPE_VGA,
    ASP_SRC_TYPE_MULTIMEDIA,          /* Multimedia is not stored in ACFG */
    ASP_SRC_TYPE_LAST_ENTRY
}   ASP_SRC_TYPE_T;

static VID_PLA_DISP_FMT_T at_disp_fmt_tbl [ASP_SRC_TYPE_LAST_ENTRY] =
{
    VID_PLA_DISP_FMT_NON_LINEAR_ZOOM, /* CVBS  */
    VID_PLA_DISP_FMT_NON_LINEAR_ZOOM, /* Component */
    VID_PLA_DISP_FMT_DOT_BY_DOT,      /* HDMI  */
    VID_PLA_DISP_FMT_DOT_BY_DOT,      /* DVI  */
    VID_PLA_DISP_FMT_DOT_BY_DOT,      /* VGA */
    VID_PLA_DISP_FMT_NORMAL           /* Multimedia */
};

/*-----------------------------------------------------------------------------
 * Funcation declarations
 *---------------------------------------------------------------------------*/
#if (VIEWPORT_SUPPORT == 1)
VOID sys_asp_ratio_set_skip_viewport_func (BOOL b_enable)
{
    _b_is_skip_viewport_func = b_enable;
}
BOOL sys_asp_ratio_get_skip_viewport_func (VOID)
{
    return _b_is_skip_viewport_func;
}
VOID sys_asp_ratio_set_is_3d_mode_enabled (BOOL b_enable)
{
    _b_is_3d_mode_enabled = b_enable;
}
BOOL sys_asp_ratio_get_is_3d_mode_enabled (VOID)
{
    return _b_is_3d_mode_enabled;
}
VOID sys_asp_ratio_set_app_status (APP_CFG_APP_STATUS_T e_status)
{
    _e_app_status = e_status;
}
VOID sys_asp_ratio_set_crnt_viewport (APP_CFG_VIEWPORT_T* pt_viewport)
{
    _t_crnt_viewport.e_type = pt_viewport->e_type;
    _t_crnt_viewport.i4_x = pt_viewport->i4_x;
    _t_crnt_viewport.i4_y = pt_viewport->i4_y;
    _t_crnt_viewport.i4_w = pt_viewport->i4_w;
    _t_crnt_viewport.i4_h = pt_viewport->i4_h;
}
APP_CFG_APP_STATUS_T sys_asp_ratio_get_app_status (VOID)
{
    return _e_app_status;
}
VOID sys_asp_ratio_set_crnt_tv_mode (TV_MODE_T e_mode)
{
    _e_crnt_tv_mode = e_mode;
}
TV_MODE_T sys_asp_ratio_get_crnt_tv_mode (VOID)
{
    return _e_crnt_tv_mode;
}
static BOOL _is_need_to_apply_viewport (VOID)
{
    if (sys_asp_ratio_get_crnt_tv_mode () != TV_MODE_TYPE_NORMAL)
    {
        return FALSE;
    }
    if (sys_asp_ratio_get_is_3d_mode_enabled ())
    {
        return FALSE;
    }
    if (sys_asp_ratio_get_skip_viewport_func ())
    {
        return FALSE;
    }

    return TRUE;
    /*
     *  Y!W changes behavior. It won't notify its status anymore.
     *  Hence, we don't rely on its status but only reference the coordinate.
     */
    /*switch (sys_asp_ratio_get_app_status ())
    {
    case APP_CFG_APP_STATUS_YH_SIDEBAR:
    case APP_CFG_APP_STATUS_MENU_NOT_WIDE_MODE:
    default:
        return TRUE;
    case APP_CFG_APP_STATUS_MENU_WIDE_MODE:
    case APP_CFG_APP_STATUS_LAST_ENTRY:
        return FALSE;
    }*/
}
#endif
static INT32 _get_src_type (const DRV_CUST_INPUT_SRC_TYPE_T e_inp_src_type,
                            const TV_DEC_TIMING_TYPE_T e_timing_type,
                            ASP_SRC_TYPE_T* pe_src_type)
{
    switch (e_inp_src_type)
    {
    case DRV_CUST_INPUT_SRC_TYPE_YPBPR:
        *pe_src_type = ASP_SRC_TYPE_COMPONENT;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_DTV:
    case DRV_CUST_INPUT_SRC_TYPE_ATV:
    case DRV_CUST_INPUT_SRC_TYPE_CVBS:
    case DRV_CUST_INPUT_SRC_TYPE_SCART:
    case DRV_CUST_INPUT_SRC_TYPE_TIMESHIFT:
        *pe_src_type = ASP_SRC_TYPE_CVBS;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_HDMI:
    {
        if (e_timing_type == TV_DEC_TIMING_GRAPHIC)
        {
            *pe_src_type = ASP_SRC_TYPE_DVI;
        }
        else
        {
            *pe_src_type = ASP_SRC_TYPE_HDMI;
        }
        break;
    }
    case DRV_CUST_INPUT_SRC_TYPE_DVI:
        *pe_src_type = ASP_SRC_TYPE_DVI;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_VGA:
        *pe_src_type = ASP_SRC_TYPE_VGA;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_MULTIMEDIA:
        *pe_src_type = ASP_SRC_TYPE_MULTIMEDIA;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_UNKNOWN:
    case DRV_CUST_INPUT_SRC_TYPE_WILDCARD:
    default:
        *pe_src_type = ASP_SRC_TYPE_CVBS;
        break;
    }
    
    return 0;
}

static VOID _update_disp_fmt_tbl (const DRV_CUSTOM_VID_SRC_INFO_T* pt_vid_src_info,
                                  const ASPECT_RATIO_MODE_T e_asp_mode)
{
    const DRV_CUSTOM_VID_SRC_INPUT_SOURCE_INFO_T* pt_inp_src_info;
    VID_PLA_DISP_FMT_T e_disp_fmt;
    ASP_SRC_TYPE_T e_src_type;
    
    if (pt_vid_src_info->e_type != DRV_CUSTOM_VID_SRC_INFO_TYPE_INPUT_SRC)
    {
        return;
    }
    pt_inp_src_info = &pt_vid_src_info->u.t_input_src;
    switch (e_asp_mode)
    {
    case ASPECT_RATIO_MODE_NORMAL:
        e_disp_fmt = VID_PLA_DISP_FMT_NORMAL;
        break;
    case ASPECT_RATIO_MODE_LETTERBOX:
        e_disp_fmt = VID_PLA_DISP_FMT_LETTERBOX;
        break;
    case ASPECT_RATIO_MODE_PAN_SCAN:
        e_disp_fmt = VID_PLA_DISP_FMT_PAN_SCAN;
        break;
    case ASPECT_RATIO_MODE_PANORAMA:
        e_disp_fmt = VID_PLA_DISP_FMT_NON_LINEAR_ZOOM;
        break;
    case ASPECT_RATIO_MODE_DOT_BY_DOT:
        e_disp_fmt = VID_PLA_DISP_FMT_DOT_BY_DOT;
        break;
    case ASPECT_RATIO_MODE_AUTO:
        e_disp_fmt = VID_PLA_DISP_FMT_CUSTOM_0;
        break;
    case ASPECT_RATIO_MODE_NO_VIDEO:
        e_disp_fmt = VID_PLA_DISP_FMT_UNKNOWN;
        break;        
    default:
        return;
    }

    _get_src_type (pt_inp_src_info->e_input_source, pt_inp_src_info->e_timing_type, &e_src_type);
    at_disp_fmt_tbl [(UINT8)e_src_type] = e_disp_fmt;
}


VOID vid_pla_get_current_disp_fmt_fct (const DRV_CUSTOM_RESOLUTION_INFO_T*     pt_src_res,
                                       const DRV_CUSTOM_RESOLUTION_INFO_T*     pt_disp_res,
                                       const DRV_CUSTOM_VID_SRC_INFO_T*        pt_vid_src_info,
                                       VID_PLA_DISP_FMT_T*                     pe_disp_fmt)
{
    const DRV_CUSTOM_VID_SRC_INPUT_SOURCE_INFO_T* pt_inp_src_info;
    ASP_SRC_TYPE_T e_src_type;
    
    if (pt_vid_src_info->e_type != DRV_CUSTOM_VID_SRC_INFO_TYPE_INPUT_SRC)
    {
        return;
    }
    pt_inp_src_info = &pt_vid_src_info->u.t_input_src;
    
    _get_src_type (pt_inp_src_info->e_input_source, pt_inp_src_info->e_timing_type, &e_src_type);
    *pe_disp_fmt = at_disp_fmt_tbl [(UINT8)e_src_type];

    return;
}

static ASPECT_RATIO_T _get_src_asp_ratio (const DRV_CUSTOM_RESOLUTION_INFO_T*    pt_res)
{
    UINT32            ui4_asp_ratio;
    ASPECT_RATIO_T    e_asp_ratio;

    /* set aspect ratio to unknown */
    e_asp_ratio = ASPECT_RATIO_UNKNOWN;

    if ((pt_res->ui2_width != 0) && (pt_res->ui2_height != 0))
    {
        /* calcuate aspect ratio according to the ratio between height and width */
        ui4_asp_ratio = pt_res->ui2_width * 100 / pt_res->ui2_height;

        if (ui4_asp_ratio <= 133) /* less than ASPECT_RATIO_4_3 */
        {
            e_asp_ratio = ASPECT_RATIO_4_3;
        }
        else if (ui4_asp_ratio >= 178) /* greater than ASPECT_RATIO_16_9 */
        {
            e_asp_ratio = ASPECT_RATIO_16_9;
        }
        else /* in the medium */
        {
            e_asp_ratio = (133 * 100 / ui4_asp_ratio) > (ui4_asp_ratio * 100 / 178) ?
                          ASPECT_RATIO_4_3 : ASPECT_RATIO_16_9;
        }
    }

    return  e_asp_ratio;
}

static ASPECT_RATIO_T _get_dst_asp_ratio (const DRV_CUSTOM_RESOLUTION_INFO_T*    pt_res)
{
    UINT32            ui4_asp_ratio;
    ASPECT_RATIO_T    e_asp_ratio;

    /* set aspect ratio to unknown */
    e_asp_ratio = ASPECT_RATIO_UNKNOWN;

    if ((pt_res->ui2_width != 0) && (pt_res->ui2_height != 0))
    {
        /* calcuate aspect ratio according to the ratio between height and width */
        ui4_asp_ratio = pt_res->ui2_width * 100 / pt_res->ui2_height;

        if (ui4_asp_ratio <= 133) /* less than ASPECT_RATIO_4_3 */
        {
            e_asp_ratio = ASPECT_RATIO_4_3;
        }
        else if ((ui4_asp_ratio > 133) && (ui4_asp_ratio <= 160)) /* in the medium of ASPECT_RATIO_4_3 and ASPECT_RATIO_16_10 */
        {

            e_asp_ratio = (133 * 100 / ui4_asp_ratio) > (ui4_asp_ratio * 100 / 160) ?
                          ASPECT_RATIO_4_3 : ASPECT_RATIO_16_10;
        }
        else if ((ui4_asp_ratio > 160) && (ui4_asp_ratio < 178)) /* in the medium of ASPECT_RATIO_16_10 and ASPECT_RATIO_16_9 */
        {

            e_asp_ratio = (160 * 100 / ui4_asp_ratio) > (ui4_asp_ratio * 100 / 178) ?
                          ASPECT_RATIO_16_10 : ASPECT_RATIO_16_9;
        }
        else /* greater than ASPECT_RATIO_16_9 */
        {
            e_asp_ratio = ASPECT_RATIO_16_9;
        }
    }

    return  e_asp_ratio;
}
static BOOL _is_center_overlay (APP_CFG_VIEWPORT_T* pt_crnt_viewport)
{
    if (pt_crnt_viewport->i4_x != 0 && pt_crnt_viewport->i4_y == 0 && pt_crnt_viewport->i4_w == 1920 && pt_crnt_viewport->i4_h == 1080)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
static BOOL _is_overlay (APP_CFG_VIEWPORT_T* pt_crnt_viewport)
{
    if (pt_crnt_viewport->i4_x == 0 && pt_crnt_viewport->i4_y == 0 && pt_crnt_viewport->i4_w == 1920 && pt_crnt_viewport->i4_h == 1080)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static VOID _get_regions (ASPECT_RATIO_MODE_T                      e_asp_ratio_mode,
                          const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                          const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                          const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                          VID_PLA_VID_REGION_T*                    pt_src_reg,
                          VID_PLA_VID_REGION_T*                    pt_dst_reg)
{
    ASPECT_RATIO_T    e_src_asp_ratio;
    ASPECT_RATIO_T    e_dst_asp_ratio;
    UINT16            ui2_pal_secam_wss_asp_ratio = 0;
    UINT16*           pui2_g_wss;
    DRV_CUSTOM_SCART_PIN_8_T* pe_g_scart_pin_8;

    if (pt_vid_src_info->ui2_vdp_id == 0)
    {
        pui2_g_wss = &ui2_g_wss_main;
        pe_g_scart_pin_8 = &e_g_scart_pin_8_main;
    }
    else
    {
        pui2_g_wss = &ui2_g_wss_sub;
        pe_g_scart_pin_8 = &e_g_scart_pin_8_sub;
    }


    /* get aspect ratios of source & destination */
    e_src_asp_ratio = _get_src_asp_ratio (pt_src_res);
    e_dst_asp_ratio = _get_dst_asp_ratio (pt_dst_res);

    /* Update disp format table. */
    if (DRV_CUSTOM_VID_SRC_INFO_TYPE_INPUT_SRC == pt_vid_src_info->e_type)
    {
        _update_disp_fmt_tbl (pt_vid_src_info, e_asp_ratio_mode);
        return;
    }
    else if (DRV_CUSTOM_VID_SRC_INFO_TYPE_UNKNOWN == pt_vid_src_info->e_type)
    {
        /* Reset WSS and SCART pin8 when change channel/source */
        *pui2_g_wss = 0;
        *pe_g_scart_pin_8 = DRV_CUSTOM_SCART_PIN_8_UNKNOWN;
    }
    else if (DRV_CUSTOM_VID_SRC_INFO_TYPE_PAL_SECAM_WSS_SCART == pt_vid_src_info->e_type)
    {
        ui2_pal_secam_wss_asp_ratio = 
            pt_vid_src_info->u.t_wss_scart.ui2_pal_secam_wss & 0x0F;

        if (ui2_pal_secam_wss_asp_ratio == 0)
        {
            if (DRV_CUSTOM_SCART_PIN_8_UNKNOWN == pt_vid_src_info->u.t_wss_scart.e_scart_pin_8)
            {
                /*It allow driver to reset last WSS when WSS value is absent*/
                *pui2_g_wss = 0;
            }
            else
            {
                *pe_g_scart_pin_8 = pt_vid_src_info->u.t_wss_scart.e_scart_pin_8;
            }
        }
        else
        {
            if (pt_src_res->ui2_height < 720)
            {
                /* keep current WSS */
                *pui2_g_wss = ui2_pal_secam_wss_asp_ratio; 
            }
            else
            {
                /* WSS is not supported for 720p/1080i/1080p */
                *pui2_g_wss = 0;
            }
        }
    }


    /* replace source aspect ratio with definition in MPEG header */
    if ((e_asp_ratio_mode == ASPECT_RATIO_MODE_AUTO) &&
        (pt_vid_src_info->e_type == DRV_CUSTOM_VID_SRC_INFO_TYPE_MPEG))
    {
        switch (pt_vid_src_info->u.t_mpeg.e_asp_ratio)
        {
            case VID_DEC_SRC_ASPECT_RATIO_4_3:
                e_src_asp_ratio = ASPECT_RATIO_4_3;
                break;
            case VID_DEC_SRC_ASPECT_RATIO_16_9:
            case VID_DEC_SRC_ASPECT_RATIO_2_21_1:
                e_src_asp_ratio = ASPECT_RATIO_16_9;
                break;
            default:
                break;
        }
    }
    
    /* When scanning, ignore the WSS. So we remap auto to normal */
    if(//a_cfg_av_is_condition(APP_CFG_AV_COND_MASK_SCANNING) &&
       e_asp_ratio_mode == ASPECT_RATIO_MODE_AUTO)
    {
        e_asp_ratio_mode = ASPECT_RATIO_MODE_NORMAL;
    }

    /* set default regions */
    _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
    _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);

    
    if (e_src_asp_ratio == ASPECT_RATIO_4_3) /* source 4:3 */
    {
        /* source 4:3, destination 16:9 (16:10) */
        if (e_dst_asp_ratio == ASPECT_RATIO_16_9 || e_dst_asp_ratio == ASPECT_RATIO_16_10)
        {
            switch (e_asp_ratio_mode)
            {
                case ASPECT_RATIO_MODE_NORMAL:          /* normal */
                case ASPECT_RATIO_MODE_PANORAMA:        /* panorama (non-linear) */
                case ASPECT_RATIO_MODE_DOT_BY_DOT:      /* dot by dot */
                {
                    _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                    _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);
                }
                break;

                case ASPECT_RATIO_MODE_LETTERBOX:       /* letterbox */
                {
                    _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);

                    if (e_dst_asp_ratio == ASPECT_RATIO_16_9) /* destination 16:9 */
                    {
                        _SET_REGION (pt_dst_reg, 1250, 0, 7500, 10000);
                    }
                    else /* destination 16:10 */
                    {
                        _SET_REGION (pt_dst_reg, 833, 0, 8334, 10000);
                    }
                }
                break;

                case ASPECT_RATIO_MODE_PAN_SCAN:        /* pan & scan */
                {
                    _SET_REGION (pt_src_reg, 0, 1250, 10000, 7500);
                    _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);
                }
                break;

                case ASPECT_RATIO_MODE_AUTO:            /* auto */
                {
                    switch (pt_vid_src_info->e_type)
                    {
                        case DRV_CUSTOM_VID_SRC_INFO_TYPE_MPEG:
                        {
                            switch (pt_vid_src_info->u.t_mpeg.ui1_afd & 0x07)
                            {
                                case 0: case 1: case 4: default:
                                {
                                    _SET_REGION (pt_src_reg, 1, 1, 9999, 9999);
                                    _SET_REGION (pt_dst_reg, 1251, 1, 7500, 9999);
                                }
                                break;

                                case 2: case 6: case 7:
                                {
                                    _SET_REGION (pt_src_reg, 0, 1250, 10000, 7500);
                                    _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);
                                }
                                break;

                                case 3: case 5:
                                {
                                    _SET_REGION (pt_src_reg, 0, 715, 10000, 8570);
                                    _SET_REGION (pt_dst_reg, 625, 0, 8750, 10000);
                                }
                                break;
                            }

                        }
                        break;

                        case DRV_CUSTOM_VID_SRC_INFO_TYPE_PAL_SECAM_WSS_SCART:  /* WSS */
                        {
                            switch (*pui2_g_wss)
                            {
                                case 4: case 7:
                                {
                                    _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                                    _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);
                                }
                                break;

                                case 8: default:
                                {
                                    _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                                    _SET_REGION (pt_dst_reg, 1250, 0, 7500, 10000);
                                }
                                break;

                                case 11: case 13:
                                {
                                    _SET_REGION (pt_src_reg, 0, 1250, 10000, 7500);
                                    _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);
                                }
                                break;

                                case 14: case 1:
                                {
                                    _SET_REGION (pt_src_reg, 0, 715, 10000, 8570);
                                    _SET_REGION (pt_dst_reg, 625, 0, 8750, 10000);
                                }
                                break;

                                case 2:
                                {
                                    _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                                    _SET_REGION (pt_dst_reg, 625, 0, 8750, 10000);
                                }
                                break;

                                case 0: /*SCART pin8*/
                                {                                  
                                    switch(*pe_g_scart_pin_8)
                                    {   
                                        case DRV_CUSTOM_SCART_PIN_8_ASPECT_16_9:
                                        default:
                                        {
                                            _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                                            _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);
                                        }
                                        break;

                                        case DRV_CUSTOM_SCART_PIN_8_ASPECT_4_3:
                                        {
                                            _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                                            _SET_REGION (pt_dst_reg, 1250, 0, 7500, 10000);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }

                        }
                        break;
                        
                        default:
                            break;
                    }

                }
                break;

                case ASPECT_RATIO_MODE_NO_VIDEO:
                default: /* unknown aspect ratio mode */
                {
                    _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                    _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);
                }
                break;
            }
#if (VIEWPORT_SUPPORT == 1)
            /* Override disp_region for 4:3 => 16:9 */
            if (_is_need_to_apply_viewport ())
            {
                VID_PLA_VID_REGION_T t_ori_dst_reg;
                UINT32 ui4_delta = 6666 * pt_dst_reg->ui4_x / 10000;
                
                x_memcpy (&t_ori_dst_reg, pt_dst_reg, sizeof (VID_PLA_VID_REGION_T));
#if 1
                /*
                 *  Y!W changes behavior. It won't notify its status anymore.
                 *  Hence, we don't rely on its status but only reference the coordinate.
                 */
                if (_is_center_overlay (&_t_crnt_viewport))
                {
                    pt_dst_reg->ui4_x += 1666;
                    if ((pt_dst_reg->ui4_width + pt_dst_reg->ui4_x) > 10000)
                    {
                        pt_dst_reg->ui4_width = 10000 - pt_dst_reg->ui4_x;
                    }
                    pt_src_reg->ui4_width = pt_src_reg->ui4_width * pt_dst_reg->ui4_width / t_ori_dst_reg.ui4_width;
				}
                else
                {
                    if (!_is_overlay (&_t_crnt_viewport))
                    {
                        pt_dst_reg->ui4_x = _t_crnt_viewport.i4_x*10000/1920 + ui4_delta;
                        pt_dst_reg->ui4_y = _t_crnt_viewport.i4_y*10000/1080;
                        pt_dst_reg->ui4_width= _t_crnt_viewport.i4_w*10000/1920 - ui4_delta * 2;
                        pt_dst_reg->ui4_height= _t_crnt_viewport.i4_h*10000/1080;
                    }
                }
#else                
                switch (sys_asp_ratio_get_app_status ())
                {
                case APP_CFG_APP_STATUS_MENU_NOT_WIDE_MODE:
                    if (_t_crnt_viewport.e_type == APP_CFG_VIEWPORT_TYPE_VIEWPORT)
                    {
                        pt_dst_reg->ui4_x = 638*10000/1920 + ui4_delta;
                        pt_dst_reg->ui4_y = 150*10000/1080;
                        pt_dst_reg->ui4_width = 1272*10000/1920 - ui4_delta * 2;
                        pt_dst_reg->ui4_height= 716*10000/1080;
                    }
                    else if (_t_crnt_viewport.e_type == APP_CFG_VIEWPORT_TYPE_CENTER_OVERLAY)
                    {
                        pt_dst_reg->ui4_x += 1666;
                        if ((pt_dst_reg->ui4_width + pt_dst_reg->ui4_x) > 10000)
                        {
                            pt_dst_reg->ui4_width = 10000 - pt_dst_reg->ui4_x;
                        }
                            pt_src_reg->ui4_width = pt_src_reg->ui4_width * pt_dst_reg->ui4_width / t_ori_dst_reg.ui4_width;
                    }
                    break;
                case APP_CFG_APP_STATUS_YH_SIDEBAR:
                case APP_CFG_APP_STATUS_YH_DOCK:
                    if (_is_center_overlay (&_t_crnt_viewport))
                    {
                        pt_dst_reg->ui4_x += 1666;
                        if ((pt_dst_reg->ui4_width + pt_dst_reg->ui4_x) > 10000)
                        {
                            pt_dst_reg->ui4_width = 10000 - pt_dst_reg->ui4_x;
                        }
                        pt_src_reg->ui4_width = pt_src_reg->ui4_width * pt_dst_reg->ui4_width / t_ori_dst_reg.ui4_width;
                    }
                    else
                    {                        
                        if (!_is_overlay (&_t_crnt_viewport))
                        {
                            pt_dst_reg->ui4_x = _t_crnt_viewport.i4_x*10000/1920 + ui4_delta;
                            pt_dst_reg->ui4_y = _t_crnt_viewport.i4_y*10000/1080;
                            pt_dst_reg->ui4_width= _t_crnt_viewport.i4_w*10000/1920 - ui4_delta * 2;
                            pt_dst_reg->ui4_height= _t_crnt_viewport.i4_h*10000/1080;
                        }
                    }
                    break;
                default:
                    break;
                }
#endif
            }
#endif            
        }
    }
    else /* source 16:9 */
    {
        /* source 16:9, destination 16:9 (16:10) */
        if (e_dst_asp_ratio == ASPECT_RATIO_16_9 || e_dst_asp_ratio == ASPECT_RATIO_16_10)
        {
            switch (e_asp_ratio_mode)
            {
                case ASPECT_RATIO_MODE_NORMAL:          /* normal */
                case ASPECT_RATIO_MODE_PANORAMA:        /* panorama (non-linear) */
                case ASPECT_RATIO_MODE_DOT_BY_DOT:      /* dot by dot */
                {
                    _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                    _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);
                }
                break;

                case ASPECT_RATIO_MODE_LETTERBOX:       /* letterbox */
                {
                    _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                    _SET_REGION (pt_dst_reg, 1250, 0, 7500, 10000);
                }
                break;

                case ASPECT_RATIO_MODE_PAN_SCAN:        /* pan & scan */
                {
                    _SET_REGION (pt_src_reg, 1250, 1250, 7500, 7500);
                    _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);
                }
                break;

                case ASPECT_RATIO_MODE_AUTO:            /* auto */
                {
                    switch (pt_vid_src_info->e_type)
                    {
                        case DRV_CUSTOM_VID_SRC_INFO_TYPE_MPEG:
                        {
                            switch (pt_vid_src_info->u.t_mpeg.ui1_afd & 0x07)
                            {
                                case 0: case 2: case 3: case 4: case 6: case 7: default:
                                {
                                    _SET_REGION (pt_src_reg, 1, 1, 9999, 9999);
                                    _SET_REGION (pt_dst_reg, 1, 1, 9999, 9999);
                                }
                                break;
                                
                                case 1:
                                {
                                    _SET_REGION (pt_src_reg, 1250, 0, 7500, 10000);
                                    _SET_REGION (pt_dst_reg, 1250, 0, 7500, 10000);
                                }
                                break;

                                case 5:
                                {
                                    _SET_REGION (pt_src_reg, 1250, 715, 7500, 8570);
                                    _SET_REGION (pt_dst_reg, 625, 0, 8750, 10000);
                                }
                                break;
                            }
                        }
                        break;

                        case DRV_CUSTOM_VID_SRC_INFO_TYPE_PAL_SECAM_WSS_SCART: /* WSS */
                        {
                            switch (*pui2_g_wss)
                            {
                                case 8:
                                {
                                    _SET_REGION (pt_src_reg, 1250, 0, 7500, 10000);
                                    _SET_REGION (pt_dst_reg, 1250, 0, 7500, 10000);
                                }
                                break;

                                case 14:
                                {
                                    _SET_REGION (pt_src_reg, 1250, 0, 7500, 10000);
                                    _SET_REGION (pt_dst_reg, 625, 0, 8750, 10000);
                                }
                                break;

                                case 7: default: 
                                {
                                    _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                                    _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);
                                }
                                break;

                                case 0: /*SCART pin8*/
                                {                                   
                                    switch(*pe_g_scart_pin_8)
                                    {   
                                        case DRV_CUSTOM_SCART_PIN_8_ASPECT_16_9:
                                        default:
                                        {
                                            _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                                            _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);
                                        }
                                        break;

                                        case DRV_CUSTOM_SCART_PIN_8_ASPECT_4_3:
                                        {
                                            _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                                            _SET_REGION (pt_dst_reg, 1250, 0, 7500, 10000);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        break;
                        
                        default:
                            break;
                    }
                }
                break;

                case ASPECT_RATIO_MODE_NO_VIDEO:
                default: /* unknown aspect ratio mode */
                {
                    _SET_REGION (pt_src_reg, 0, 0, 10000, 10000);
                    _SET_REGION (pt_dst_reg, 0, 0, 10000, 10000);
                }
                break;
            }
#if (VIEWPORT_SUPPORT == 1)
            /* Override disp_region for 16:9 => 16:9 */
            if (_is_need_to_apply_viewport ())
            {
                VID_PLA_VID_REGION_T t_ori_dst_reg;
                UINT32 ui4_delta = 6666 * pt_dst_reg->ui4_x / 10000;
                
                x_memcpy (&t_ori_dst_reg, pt_dst_reg, sizeof (VID_PLA_VID_REGION_T));
#if 1
                /*
                 *  Y!W changes behavior. It won't notify its status anymore.
                 *  Hence, we don't rely on its status but only reference the coordinate.
                 */
                if (_is_center_overlay (&_t_crnt_viewport))
                {
                    pt_dst_reg->ui4_x += 1666;
                    if ((pt_dst_reg->ui4_width + pt_dst_reg->ui4_x) > 10000)
                    {
                        pt_dst_reg->ui4_width = 10000 - pt_dst_reg->ui4_x;
                    }
                    pt_src_reg->ui4_width = pt_src_reg->ui4_width * pt_dst_reg->ui4_width / t_ori_dst_reg.ui4_width;
                }
                else
                {
                    if (!_is_overlay (&_t_crnt_viewport))
                    {
                        pt_dst_reg->ui4_x = _t_crnt_viewport.i4_x*10000/1920 + ui4_delta;
                        pt_dst_reg->ui4_y = _t_crnt_viewport.i4_y*10000/1080;
                        pt_dst_reg->ui4_width= _t_crnt_viewport.i4_w*10000/1920 - ui4_delta * 2;
                        pt_dst_reg->ui4_height= _t_crnt_viewport.i4_h*10000/1080;
                    }
                }
#else
                switch (sys_asp_ratio_get_app_status ())
                {
                case APP_CFG_APP_STATUS_MENU_NOT_WIDE_MODE:
                    if (_t_crnt_viewport.e_type == APP_CFG_VIEWPORT_TYPE_VIEWPORT)
                    {
                        pt_dst_reg->ui4_x = 638*10000/1920 + ui4_delta;
                        pt_dst_reg->ui4_y = 150*10000/1080;
                        pt_dst_reg->ui4_width = 1272*10000/1920 - ui4_delta * 2;
                        pt_dst_reg->ui4_height= 716*10000/1080;
                    }
                    else if (_t_crnt_viewport.e_type == APP_CFG_VIEWPORT_TYPE_CENTER_OVERLAY)
                    {
                        pt_dst_reg->ui4_x += 1666;
                        if ((pt_dst_reg->ui4_width + pt_dst_reg->ui4_x) > 10000)
                        {
                            pt_dst_reg->ui4_width = 10000 - pt_dst_reg->ui4_x;
                        }
                            pt_src_reg->ui4_width = pt_src_reg->ui4_width * pt_dst_reg->ui4_width / t_ori_dst_reg.ui4_width;
                    }
                    break;
                case APP_CFG_APP_STATUS_YH_SIDEBAR:
                case APP_CFG_APP_STATUS_YH_DOCK:
                    if (_is_center_overlay (&_t_crnt_viewport))
                    {
                        pt_dst_reg->ui4_x += 1666;
                        if ((pt_dst_reg->ui4_width + pt_dst_reg->ui4_x) > 10000)
                        {
                            pt_dst_reg->ui4_width = 10000 - pt_dst_reg->ui4_x;
                        }
                        pt_src_reg->ui4_width = pt_src_reg->ui4_width * pt_dst_reg->ui4_width / t_ori_dst_reg.ui4_width;
                    }
                    else
                    {
                        if (!_is_overlay (&_t_crnt_viewport))
                        {
                            pt_dst_reg->ui4_x = _t_crnt_viewport.i4_x*10000/1920 + ui4_delta;
                            pt_dst_reg->ui4_y = _t_crnt_viewport.i4_y*10000/1080;
                            pt_dst_reg->ui4_width= _t_crnt_viewport.i4_w*10000/1920 - ui4_delta * 2;
                            pt_dst_reg->ui4_height= _t_crnt_viewport.i4_h*10000/1080;
                        }
                    }
                    break;
                default:
                    _SET_REGION (pt_dst_reg, 3333, 1666, 6667, (VSH_REGION_MAX_WIDTH-1666*2));
                    break;
                }
#endif
            }
#endif            
        }
    }


    /* Normalize regions */
    pt_src_reg->ui4_x      = pt_src_reg->ui4_x      * REGION_RATIO_WIDTH;
    pt_src_reg->ui4_y      = pt_src_reg->ui4_y      * REGION_RATIO_HEIGHT;
    pt_src_reg->ui4_width  = pt_src_reg->ui4_width  * REGION_RATIO_WIDTH;
    pt_src_reg->ui4_height = pt_src_reg->ui4_height * REGION_RATIO_HEIGHT;

    pt_dst_reg->ui4_x      = pt_dst_reg->ui4_x      * REGION_RATIO_WIDTH;
    pt_dst_reg->ui4_y      = pt_dst_reg->ui4_y      * REGION_RATIO_HEIGHT;
    pt_dst_reg->ui4_width  = pt_dst_reg->ui4_width  * REGION_RATIO_WIDTH;
    pt_dst_reg->ui4_height = pt_dst_reg->ui4_height * REGION_RATIO_HEIGHT;
}

/*
   Callback functions for driver.
*/
VOID asp_ratio_normal_ex_fct     (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                  const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                  const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                  VID_PLA_VID_REGION_T*                    pt_src_reg,
                                  VID_PLA_VID_REGION_T*                    pt_dst_reg)
{
#ifdef SYS_MHEG5_SUPPORT
    BOOL b_auto_mode = FALSE;
    if((b_auto_mode = x_mheg5_query_display_b_auto_mode(0)) == TRUE)
    {
        if (_asp_ratio_mheg5_fct(
            pt_src_res,
            pt_vid_src_info, 
            FALSE,
            pt_src_reg, 
            pt_dst_reg))
        {
            return;
        }
    }
#endif

    _get_regions (ASPECT_RATIO_MODE_NORMAL,
                  pt_src_res,
                  pt_dst_res,
                  pt_vid_src_info,
                  pt_src_reg,
                  pt_dst_reg);

#ifdef SYS_MHEG5_SUPPORT
    if(b_auto_mode == FALSE)
    {
        _asp_ratio_mheg5_fct_ex(
                    pt_src_res,
                    pt_vid_src_info,
                    pt_src_reg,
                    pt_dst_reg);

    }
#endif
}

VOID asp_ratio_letterbox_ex_fct  (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                  const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                  const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                  VID_PLA_VID_REGION_T*                    pt_src_reg,
                                  VID_PLA_VID_REGION_T*                    pt_dst_reg)
{
#ifdef SYS_MHEG5_SUPPORT
    BOOL b_auto_mode = FALSE;
    if((b_auto_mode = x_mheg5_query_display_b_auto_mode(0)) == TRUE)
    {
        if (_asp_ratio_mheg5_fct(
            pt_src_res,
            pt_vid_src_info, 
            FALSE,
            pt_src_reg, 
            pt_dst_reg))
        {
            return;
        }
    }
#endif

    _get_regions (ASPECT_RATIO_MODE_LETTERBOX,
                  pt_src_res,
                  pt_dst_res,
                  pt_vid_src_info,
                  pt_src_reg,
                  pt_dst_reg);

#ifdef SYS_MHEG5_SUPPORT
    if(b_auto_mode == FALSE)
    {
        _asp_ratio_mheg5_fct_ex(
                    pt_src_res,
                    pt_vid_src_info,
                    pt_src_reg,
                    pt_dst_reg);

    }
#endif
}

VOID asp_ratio_pan_scan_ex_fct   (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                  const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                  const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                  VID_PLA_VID_REGION_T*                    pt_src_reg,
                                  VID_PLA_VID_REGION_T*                    pt_dst_reg)
{
#ifdef SYS_MHEG5_SUPPORT
    BOOL b_auto_mode = FALSE;
    if((b_auto_mode = x_mheg5_query_display_b_auto_mode(0)) == TRUE)
    {
        if (_asp_ratio_mheg5_fct(
            pt_src_res,
            pt_vid_src_info, 
            FALSE,
            pt_src_reg, 
            pt_dst_reg))
        {
            return;
        }
    }
#endif

    _get_regions (ASPECT_RATIO_MODE_PAN_SCAN,
                  pt_src_res,
                  pt_dst_res,
                  pt_vid_src_info,
                  pt_src_reg,
                  pt_dst_reg);

#ifdef SYS_MHEG5_SUPPORT
    if(b_auto_mode == FALSE)
    {
        _asp_ratio_mheg5_fct_ex(
                    pt_src_res,
                    pt_vid_src_info,
                    pt_src_reg,
                    pt_dst_reg);

    }
#endif
}

VOID asp_ratio_panorama_ex_fct   (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                  const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                  const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                  VID_PLA_VID_REGION_T*                    pt_src_reg,
                                  VID_PLA_VID_REGION_T*                    pt_dst_reg)
{
#ifdef SYS_MHEG5_SUPPORT
    BOOL b_auto_mode = FALSE;
    if((b_auto_mode = x_mheg5_query_display_b_auto_mode(0)) == TRUE)
    {
        if (_asp_ratio_mheg5_fct(
            pt_src_res,
            pt_vid_src_info, 
            FALSE,
            pt_src_reg, 
            pt_dst_reg))
        {
            return;
        }
    }
#endif

    _get_regions (ASPECT_RATIO_MODE_PANORAMA,
                  pt_src_res,
                  pt_dst_res,
                  pt_vid_src_info,
                  pt_src_reg,
                  pt_dst_reg);

#ifdef SYS_MHEG5_SUPPORT
    if(b_auto_mode == FALSE)
    {
        _asp_ratio_mheg5_fct_ex(
                    pt_src_res,
                    pt_vid_src_info,
                    pt_src_reg,
                    pt_dst_reg);

    }
#endif
}

VOID asp_ratio_dot_by_dot_ex_fct (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                  const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                  const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                  VID_PLA_VID_REGION_T*                    pt_src_reg,
                                  VID_PLA_VID_REGION_T*                    pt_dst_reg)
{
#ifdef SYS_MHEG5_SUPPORT
    BOOL b_auto_mode = FALSE;
    if((b_auto_mode = x_mheg5_query_display_b_auto_mode(0)) == TRUE)
    {
        if (_asp_ratio_mheg5_fct(
            pt_src_res,
            pt_vid_src_info, 
            FALSE,
            pt_src_reg, 
            pt_dst_reg))
        {
            return;
        }
    }
#endif

    _get_regions (ASPECT_RATIO_MODE_DOT_BY_DOT,
                  pt_src_res,
                  pt_dst_res,
                  pt_vid_src_info,
                  pt_src_reg,
                  pt_dst_reg);

#ifdef SYS_MHEG5_SUPPORT
    if(b_auto_mode == FALSE)
    {
        _asp_ratio_mheg5_fct_ex(
                    pt_src_res,
                    pt_vid_src_info,
                    pt_src_reg,
                    pt_dst_reg);

    }
#endif
}

VOID asp_ratio_auto_ex_fct       (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                                  const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                                  const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                                  VID_PLA_VID_REGION_T*                    pt_src_reg,
                                  VID_PLA_VID_REGION_T*                    pt_dst_reg)
{
    #ifdef SYS_MHEG5_SUPPORT
    if (_asp_ratio_mheg5_fct(
        pt_src_res,
        pt_vid_src_info, 
        TRUE,
        pt_src_reg, 
        pt_dst_reg))
    {
        return;
    }
    #endif

    _get_regions (ASPECT_RATIO_MODE_AUTO,
                  pt_src_res,
                  pt_dst_res,
                  pt_vid_src_info,
                  pt_src_reg,
                  pt_dst_reg);
}

VOID asp_ratio_no_video_fct   (const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_src_res,
                               const DRV_CUSTOM_RESOLUTION_INFO_T*      pt_dst_res,
                               const DRV_CUSTOM_VID_SRC_INFO_T*         pt_vid_src_info,
                               VID_PLA_VID_REGION_T*                    pt_src_reg,
                               VID_PLA_VID_REGION_T*                    pt_dst_reg)
{
#ifdef SYS_MHEG5_SUPPORT
    BOOL b_auto_mode = FALSE;
    if((b_auto_mode = x_mheg5_query_display_b_auto_mode(0)) == TRUE)
    {
        if (_asp_ratio_mheg5_fct(
            pt_src_res,
            pt_vid_src_info, 
            FALSE,
            pt_src_reg, 
            pt_dst_reg))
        {
            return;
        }
    }
#endif
    
    _get_regions (ASPECT_RATIO_MODE_NO_VIDEO,
                  pt_src_res,
                  pt_dst_res,
                  pt_vid_src_info,
                  pt_src_reg,
                  pt_dst_reg);

#ifdef SYS_MHEG5_SUPPORT
    if(b_auto_mode == FALSE)
    {
        _asp_ratio_mheg5_fct_ex(
                    pt_src_res,
                    pt_vid_src_info,
                    pt_src_reg,
                    pt_dst_reg);

    }
#endif
}

#ifdef SYS_MHEG5_SUPPORT
static BOOL _asp_ratio_mheg5_fct(
    const DRV_CUSTOM_RESOLUTION_INFO_T* pt_src_res,
    const DRV_CUSTOM_VID_SRC_INFO_T*    pt_vid_src_info,
    BOOL                                b_auto_mode,
    VID_PLA_VID_REGION_T*               pt_src_reg,
    VID_PLA_VID_REGION_T*               pt_dst_reg)
{
    if (DRV_CUSTOM_VID_SRC_INFO_TYPE_MPEG == pt_vid_src_info->e_type &&
        ((pt_vid_src_info->ui2_vdp_id == 0)
        #ifdef MHEG5_IC_SUPPORT
        || (pt_vid_src_info->ui2_vdp_id == 1)
        #endif
        ))
    {
        VSH_REGION_INFO_T       t_src_region;
        VSH_REGION_INFO_T       t_dst_region;
        MHEG5_DISPLAY_PARAM_T   t_dspy_param;

        SCC_VID_SCREEN_MODE_T   e_screen_mode;
        MHEG5_VIDEO_MODE_T      e_vid_mode;

        if (b_auto_mode)
        {
            e_screen_mode = x_mheg5_get_screen_auto_mode_enum();
            e_vid_mode    = MHEG5_VIDEO_MODE_AUTO;
        }
        else
        {
            e_screen_mode = SCC_VID_SCREEN_MODE_NORMAL;
            e_vid_mode    = MHEG5_VIDEO_MODE_FULLSCREEN;
        }
        
        t_dspy_param.ui1_afd = pt_vid_src_info->u.t_mpeg.ui1_afd;
        t_dspy_param.e_src_aspect_ratio = 
            ((VID_DEC_SRC_ASPECT_RATIO_4_3 == pt_vid_src_info->u.t_mpeg.e_asp_ratio) ?
                MHEG5_ASPECT_RATIO_4_3 : MHEG5_ASPECT_RATIO_16_9);
        t_dspy_param.e_screen_mode    = e_screen_mode;
        t_dspy_param.e_vid_mode       = e_vid_mode;
        t_dspy_param.ui2_video_width  = pt_src_res->ui2_width;
        t_dspy_param.ui2_video_height = pt_src_res->ui2_height;
        
        if (MHEG5R_OK == x_mheg5_query_display_adjustment(
            0,
            &t_dspy_param,
            &t_src_region, 
            &t_dst_region))
        {
            pt_src_reg->ui4_x      = t_src_region.ui4_x;
            pt_src_reg->ui4_y      = t_src_region.ui4_y;
            pt_src_reg->ui4_width  = t_src_region.ui4_width;
            pt_src_reg->ui4_height = t_src_region.ui4_height;

            pt_dst_reg->ui4_x      = t_dst_region.ui4_x;
            pt_dst_reg->ui4_y      = t_dst_region.ui4_y;
            pt_dst_reg->ui4_width  = t_dst_region.ui4_width;
            pt_dst_reg->ui4_height = t_dst_region.ui4_height;

            return TRUE;
        }
    }

    return FALSE;
}

static BOOL _asp_ratio_mheg5_fct_ex(
       const DRV_CUSTOM_RESOLUTION_INFO_T* pt_src_res,
       const DRV_CUSTOM_VID_SRC_INFO_T*    pt_vid_src_info,
       const VID_PLA_VID_REGION_T*         pt_src_reg,
       const VID_PLA_VID_REGION_T*         pt_dst_reg)
{
    if (DRV_CUSTOM_VID_SRC_INFO_TYPE_MPEG == pt_vid_src_info->e_type &&
    ((pt_vid_src_info->ui2_vdp_id == 0)
#ifdef MHEG5_IC_SUPPORT
    || (pt_vid_src_info->ui2_vdp_id == 1)
#endif
    ))
    {
        MHEG5_DISPLAY_PARAM_T   t_dspy_param;
        VSH_REGION_INFO_T       t_src_region;
        VSH_REGION_INFO_T       t_dst_region;
        
        SCC_VID_SCREEN_MODE_T   e_screen_mode;
        MHEG5_VIDEO_MODE_T      e_vid_mode;

        e_screen_mode = SCC_VID_SCREEN_MODE_NORMAL;
        e_vid_mode    = MHEG5_VIDEO_MODE_FULLSCREEN;
        
        t_dspy_param.ui1_afd = pt_vid_src_info->u.t_mpeg.ui1_afd;
        t_dspy_param.e_src_aspect_ratio = 
            ((VID_DEC_SRC_ASPECT_RATIO_4_3 == pt_vid_src_info->u.t_mpeg.e_asp_ratio) ?
                MHEG5_ASPECT_RATIO_4_3 : MHEG5_ASPECT_RATIO_16_9);
        t_dspy_param.e_screen_mode    = e_screen_mode;
        t_dspy_param.e_vid_mode       = e_vid_mode;
        t_src_region.ui4_x      = pt_src_reg->ui4_x;
        t_src_region.ui4_y      = pt_src_reg->ui4_y;
        t_src_region.ui4_width  = pt_src_reg->ui4_width;
        t_src_region.ui4_height = pt_src_reg->ui4_height;
        
        t_dst_region.ui4_x      = pt_dst_reg->ui4_x;
        t_dst_region.ui4_y      = pt_dst_reg->ui4_y;
        t_dst_region.ui4_width  = pt_dst_reg->ui4_width;
        t_dst_region.ui4_height = pt_dst_reg->ui4_height;
        
        if(x_mheg5_query_display_adjustment_ex(0,
                                            &t_dspy_param,
                                            &t_src_region,
                                            &t_dst_region) == MHEG5R_OK)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
}

#endif
/*-----------------------------------------------------------------------------
 * Name: sys_init_disp_fmt_tbl
 *
 * Description: This function is called by ACFG to initialize at_disp_fmt_tbl[]
 *              at booting time. This function MUST BE ONLY called one time
 *              during booting.
 *
 * Inputs:      pv_acfg_data    A pointer to ACFG init data.
 *              ui4_data_len    Data length of pv_acfg_data.
 *
 * Outputs:     -
 *
 * Returns:  0                  This function succeeded.
 *           Otherwiae          This function failed.
 *-----------------------------------------------------------------------------*/
INT32 sys_init_disp_fmt_tbl (VOID* pv_acfg_data, UINT32 ui4_data_len)
{
    UINT8 ui1_i;
    UINT8* aui1_scr_mode = (UINT8*) pv_acfg_data;

    if (ui4_data_len >= ASP_SRC_TYPE_LAST_ENTRY)
    {
        ui4_data_len = ASP_SRC_TYPE_LAST_ENTRY;
    }

    for (ui1_i = 0;ui1_i < ui4_data_len;ui1_i++)
    {
        SCC_VID_SCREEN_MODE_T e_scr_mode = (SCC_VID_SCREEN_MODE_T)aui1_scr_mode[ui1_i]; 
        VID_PLA_DISP_FMT_T e_disp_fmt;        

        switch (e_scr_mode)
        {
        case SCC_VID_SCREEN_MODE_NORMAL:      /* Normal. */
            e_disp_fmt = VID_PLA_DISP_FMT_NORMAL;
            break;
        case SCC_VID_SCREEN_MODE_LETTERBOX:      /* Letterbox. */
            e_disp_fmt = VID_PLA_DISP_FMT_LETTERBOX;
            break;
        case SCC_VID_SCREEN_MODE_PAN_SCAN:      /* Pan scan. */
            e_disp_fmt = VID_PLA_DISP_FMT_PAN_SCAN;
            break;
        case SCC_VID_SCREEN_MODE_NON_LINEAR_ZOOM:   /* Non linear. */
            e_disp_fmt = VID_PLA_DISP_FMT_NON_LINEAR_ZOOM;
            break;
        case SCC_VID_SCREEN_MODE_DOT_BY_DOT:        /* VGA normal. */
            e_disp_fmt = VID_PLA_DISP_FMT_DOT_BY_DOT;
            break;
        case SCC_VID_SCREEN_MODE_CUSTOM_DEF_0:      /* Auto. */
            e_disp_fmt = VID_PLA_DISP_FMT_CUSTOM_0;
            break;
        default:
            return -1;
        }
        at_disp_fmt_tbl[ui1_i] = e_disp_fmt;
    }
    
    return 0;
}
INT32 asp_ratio_get_disp_fmt_by_inp_src (const DRV_CUST_INPUT_SRC_TYPE_T e_inp_src_type,
                                         const DRV_CUSTOM_TIMING_INFO_T* pt_src_timing,
                                         VID_PLA_DISP_FMT_T* pe_disp_fmt)
{
    ASP_SRC_TYPE_T e_src_type;

    _get_src_type (e_inp_src_type, pt_src_timing->e_timing_type, &e_src_type);

    *pe_disp_fmt = at_disp_fmt_tbl [(UINT8)e_src_type];
    
    return 0;
}
