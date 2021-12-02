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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdp_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_mw.c
 *  Brief of file vdp_mw.c.
 *  Details of file vdp_mw.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "vdp_drvif.h"
LINT_EXT_HEADER_BEGIN
#include "drv_comp_id.h"
#include "drv_name.h"
#include "api_video.h"  // nptv
#include "source_table.h"  // nptv
#include "video_def.h"  // nptv
#include "sv_const.h"  // nptv
#include "vdo_misc.h"  // nptv
#include "drvcust_if.h"
#include "d_drv_cust_util.h"
#include "u_drv_cust.h"
#include "x_vid_plane.h"
#include "x_assert.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "../5391_driver/vdp/vdp_debug.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

// Test Case 1
//#define CC_VDP_MW_T1

// Do not call back middleware (display format customization)
//#define CC_VDP_FORCE_NO_MW_CB

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define CC_DISP_FMT_ENUM_SIZE      1

#define VID_QA_MAX_SRC_TYPES       16

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** Brief of MW_VDP_T
 *  Details of MW_VDP_T (optional).
 */
typedef struct
{
    UCHAR ucConnect;
    UCHAR ucEnable;
    UCHAR ucDtv;

    UCHAR ucLbEnable;
    UINT16 u2DynamicScale;

    VID_PLA_MODE_T eMode;
    VID_PLA_DISP_FMT_T eFmt;
    DRV_TYPE_T  e_type;
    TV_DEC_TIMING_TYPE_T e_timing;

    DRV_CUSTOM_RESOLUTION_INFO_T rSrcRes;
    DRV_CUSTOM_RESOLUTION_INFO_T rOutRes;

    VID_PLA_VID_REGION_T rXlatOutput;
    VID_PLA_VID_REGION_T rFullOutput;
    VID_PLA_VID_REGION_T rFinalOutput;

    VID_PLA_VID_REGION_T rXlatSource;
    VID_PLA_VID_REGION_T rFullSource;
    VID_PLA_VID_REGION_T rFinalSource;

    UCHAR ucEsId;
    UCHAR ucMpvCompId;
    UINT32 u4FrameRate;
    UINT32 u4Interlace;
    VID_PLA_QV_INFO_T rQvInfo;

    VID_PLA_SPLIT_SCRN_DEMO_T eDemo;
} MW_VDP_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define VID_QA_TBL_IS_END( pt_vid_qa_tbl )                 \
    ( pt_vid_qa_tbl->e_src_type == DRVT_UNKNOWN &&         \
      pt_vid_qa_tbl->pt_min_max_dft_lst == NULL )

#define VID_MIN_MAX_DFT_IS_END( pt_min_max_dft )                 \
    ( pt_min_max_dft->e_vid_qa_type == VID_QA_TYPE_UNKNOWN &&    \
      pt_min_max_dft->i4_min == 0 &&                             \
      pt_min_max_dft->i4_max == 0 &&                             \
      pt_min_max_dft->i4_default == 0 )



//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

EXTERN void MW_MPV_NOTIFY(UINT16 u2CompId);

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static INT32 _VdpConnect(DRV_COMP_ID_T*   pt_comp_id,
                        DRV_CONN_TYPE_T  e_conn_type,
                        const VOID*      pv_conn_info,
                        SIZE_T           z_conn_info_len,
                        VOID*            pv_tag,
                        x_rm_nfy_fct     pf_nfy);

static INT32 _VdpDisconnect(DRV_COMP_ID_T*   pt_comp_id,
                        DRV_DISC_TYPE_T  e_disc_type,
                        const VOID*      pv_disc_info,
                        SIZE_T           z_disc_info_len);

static INT32 _VdpGet(DRV_COMP_ID_T*  pt_comp_id,
                    DRV_GET_TYPE_T  e_get_type,
                    VOID*           pv_get_info,
                    SIZE_T*         pz_get_info_len);

static INT32 _VdpSet(DRV_COMP_ID_T*  pt_comp_id,
                    DRV_SET_TYPE_T  e_set_type,
                    const VOID*     pv_set_info,
                    SIZE_T          z_set_info_len);

static UINT32 _SetInput(UCHAR ucVdpId);

static UINT32 _VdpSetEnable(UCHAR ucVdpId);

static UCHAR _VdpMwGetType2ArgType(UINT32 u4GetType);

static UCHAR _VdpMwSetType2ArgType(UINT32 u4SetType);

static void _VdpAfdChangeNotify(UINT32 u4VdpId, UINT32 u4Afd, UINT32 u4AspectRatio);

static UINT32 _VdpSourceChangeNotify(UCHAR ucVdpId, VDP_SOURCE_INFO_T rVdpSrcInfo);

static UINT32 _VdpXlatDispFmtCallBackMw(UCHAR ucVdpId);

static void _VdpCalculateOutputRegion(UCHAR ucVdpId);

static void _VdpCalculateSourceRegion(UCHAR ucVdpId);

static DRVP_TYPE_T _VdpDrvCustParse(const DRV_CUSTOM_T* pt_drv_cust, VOID* pv_tag);

static EVDP_UI_VIDEO_QUALITY_ITEM_T _VdpMwVidQaType2UiVqItem(VID_QA_TYPE_T eVidQaType);

static DRV_CUST_INPUT_SRC_TYPE_T _VdpGetDrvCustInputSrcType(DRV_TYPE_T e_type);

#ifndef CC_CQAM
void _MW_VDP_WSS_CB(UCHAR ucVdpId, UINT16 u2Data);
void _MW_VDP_SCART_PIN_8_CB(UCHAR ucVdpId, DRV_CUSTOM_SCART_PIN_8_T t_scart_pin_8);
extern UINT8 MWGetScartPin8Status(UINT8 u1Path);
#endif

extern VOID TvdGetCurrInputSrcInfo( UINT16                ui2_tvd_id,
                                    DRV_TYPE_T*           pe_type,
                                    UINT16*               pui2_id,
                                    TV_DEC_TIMING_TYPE_T* pe_timing );

extern UINT8 bHDMIInputType(void);

extern UINT32 VDP_CheckZoom(UCHAR ucVdpId);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static MW_VDP_T _rMwVdpDefault =
{
    0, 0, 0, 0, 0,
    VID_PLA_NORMAL,
    VID_PLA_DISP_FMT_NORMAL,
    DRVT_UNKNOWN,
    TV_DEC_TIMING_UNKNOWN,
    { 0, 0 },
    { 0, 0 },
    {0, 0, 0, 1000, 1000},
    {0, 0, 0, 1000, 1000},
    {0, 0, 0, 1000, 1000},
    {0, 0, 0, 1000, 1000},
    {0, 0, 0, 1000, 1000},
    {0, 0, 0, 1000, 1000},

    255,
    (UCHAR)MPV_COMP_NS,
     0, 0,
    {VID_PLA_QV_INP_NULL, VID_PLA_QV_INP_NULL, VID_PLA_QV_INP_NULL, VID_PLA_QV_INP_NULL},

    VID_PLA_SPLIT_SCRN_DEMO_OFF
};

MW_VDP_T _arMwVdp[VDP_COMP_NS];

static DRV_CUSTOM_VID_SRC_INFO_T _arMwVidSrcInfo[VDP_COMP_NS];

UCHAR _arMwComp2EsId[MPV_COMP_NS];

static VDP_CONF_T* _prVdpConf[VDP_NS];

static d_vid_pla_xlat_disp_fmt_fct _pfXlatDispFmtFct[(1 << (8 * CC_DISP_FMT_ENUM_SIZE))];

static d_vid_pla_xlat_disp_fmt_ex_fct _pfXlatDispFmtExFct[(1 << (8 * CC_DISP_FMT_ENUM_SIZE))];

static d_vid_pla_get_current_disp_fmt_fct _pfGetCurrentDispFmtFct;

static d_vid_pla_overscan_fct _pfOverscanFct;

static d_vid_pla_update_overscan_tbl_fct _pfUpdateOverscanTbl;

static HANDLE_T _hMutex = NULL;            // Thread safe protection

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of _VdpConnect.
 */
//-----------------------------------------------------------------------------
static INT32 _VdpConnect(DRV_COMP_ID_T*   pt_comp_id,
                        DRV_CONN_TYPE_T  e_conn_type,
                        const VOID*      pv_conn_info,
                        SIZE_T           z_conn_info_len,
                        VOID*            pv_tag,
                        x_rm_nfy_fct     pf_nfy)
{
LINT_SUPPRESS_BRACE(818)

    DRV_COMP_ID_T* prMpvCompReg;
    UCHAR ucVdpId;
    UCHAR ucPortId;
    UCHAR ucEsId;

    UNUSED(z_conn_info_len);
    UNUSED(pv_tag);
    UNUSED(pf_nfy);

    // Check component type and id
    if (pt_comp_id == NULL)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->e_type != DRVT_VID_PLANE)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    ucVdpId = (UCHAR) pt_comp_id->ui2_id;
    if (ucVdpId > (VDP_COMP_NS + VDP_COMP_1))
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (ucVdpId >= VDP_COMP_NS)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->b_sel_out_port)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    ucPortId = (UCHAR) pt_comp_id->u.ui1_inp_port;

    if (ucPortId >= VDP_MAX_INPORT_NS)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (e_conn_type != RM_CONN_TYPE_COMP_ID)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    prMpvCompReg = (DRV_COMP_ID_T*)pv_conn_info;

    if (prMpvCompReg == NULL)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (prMpvCompReg->e_type == DRVT_VID_DEC)
    {
        if (prMpvCompReg->ui2_id > (UCHAR)MPV_COMP_NS)
        {
           return (RMR_DRV_INV_CONN_INFO);
        }

        // handle connect
    //    ucEsId = MW_MpvCompIdtoEsId(prMpvCompReg->ui2_id);        // prMpvCompReg->ui2_id to ucEsId

        _arMwVdp[ucVdpId].e_type = prMpvCompReg->e_type;
        _arMwVdp[ucVdpId].e_timing = TV_DEC_TIMING_VIDEO;
        _arMwVdp[ucVdpId].ucMpvCompId = (UCHAR)prMpvCompReg->ui2_id;
        ucEsId = _arMwComp2EsId[_arMwVdp[ucVdpId].ucMpvCompId];
        _arMwVdp[ucVdpId].ucEsId = ucEsId;

        if (_SetInput(ucVdpId) != VDP_SET_OK)
        {
           return (RMR_DRV_INV_CONN_INFO);
        }

        if (MPV_HD((MPV_COMP_ID_T)_arMwVdp[ucVdpId].ucMpvCompId))
        {
            // HD B2R

            if (bApiVideoSetVideoSrc(ucVdpId, SV_VS_DT1) == 0)
            {
                return (RMR_DRV_INV_CONN_INFO);
            }
        }
        else
        {
            // SD B2R

            if (bApiVideoSetVideoSrc(ucVdpId, SV_VS_DT2) == 0)
            {
                return (RMR_DRV_INV_CONN_INFO);
            }
        }

        _arMwVdp[ucVdpId].ucDtv = 1;
    }
    else
    if (prMpvCompReg->e_type == DRVT_TV_DEC)
    {
        DRV_TYPE_T e_type;
        UINT16 ui2_id;
        TV_DEC_TIMING_TYPE_T e_timing;

        TvdGetCurrInputSrcInfo(prMpvCompReg->ui2_id, &e_type, &ui2_id, &e_timing);
        _arMwVdp[ucVdpId].e_type = e_type;
        _arMwVdp[ucVdpId].e_timing = e_timing;
        _arMwVdp[ucVdpId].ucMpvCompId = (UCHAR)ui2_id;

#ifndef CC_CQAM
        // According to Alec, LH, Martin, 20070423
        DRV_CUSTOM_SCART_PIN_8_T t_scart_pin_8;

        _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_UNKNOWN;
        _VdpXlatDispFmtCallBackMw(ucVdpId);

        t_scart_pin_8 = (DRV_CUSTOM_SCART_PIN_8_T) MWGetScartPin8Status(ucVdpId);
        _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_PAL_SECAM_WSS_SCART;
        _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.e_scart_pin_8 = t_scart_pin_8;
        _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.ui2_pal_secam_wss = 0;
#endif

        _arMwVdp[ucVdpId].ucDtv = 0;

#ifndef CC_CQAM
        _MW_VDP_SCART_PIN_8_CB(ucVdpId, t_scart_pin_8);
#endif
    }
    else
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    return (RMR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of _VdpDisconnect.
 */
//-----------------------------------------------------------------------------
static INT32 _VdpDisconnect(DRV_COMP_ID_T*   pt_comp_id,
                        DRV_DISC_TYPE_T  e_disc_type,
                        const VOID*      pv_disc_info,
                        SIZE_T           z_disc_info_len)
{
LINT_SUPPRESS_BRACE(818)

    DRV_COMP_ID_T* prMpvCompReg;
    UCHAR ucVdpId;
    UCHAR ucPortId;

    UNUSED(e_disc_type);
    UNUSED(pv_disc_info);
    UNUSED(z_disc_info_len);

    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    if (pt_comp_id->e_type != DRVT_VID_PLANE)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    ucVdpId = (UCHAR) pt_comp_id->ui2_id;

    if (ucVdpId > (VDP_COMP_NS + VDP_COMP_1))
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (ucVdpId >= VDP_COMP_NS)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (pt_comp_id->b_sel_out_port)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    ucPortId = (UCHAR) pt_comp_id->u.ui1_inp_port;

    if (ucPortId >= VDP_MAX_INPORT_NS)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    // handle disconnect
    _arMwVdp[ucVdpId].ucMpvCompId = (UCHAR) MPV_COMP_NS;
    _arMwVdp[ucVdpId].ucEsId = 255;

#ifndef CC_CQAM
    _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_UNKNOWN;
    _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.ui2_pal_secam_wss = 0;
    _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.e_scart_pin_8 = DRV_CUSTOM_SCART_PIN_8_UNKNOWN;
#endif

    if (_SetInput(ucVdpId) != VDP_SET_OK)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    prMpvCompReg = (DRV_COMP_ID_T*)pv_disc_info;

    if (prMpvCompReg == NULL)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (prMpvCompReg->e_type == DRVT_VID_DEC)
    {
        if (bApiVideoSetVideoSrc(ucVdpId, SV_VS_MAX) == 0)
        {
            return (RMR_DRV_INV_CONN_INFO);
        }
    }

    return (RMR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of _VdpGet.
 */
//-----------------------------------------------------------------------------
static INT32 _VdpGet(DRV_COMP_ID_T*  pt_comp_id,
                    DRV_GET_TYPE_T  e_get_type,
                    VOID*           pv_get_info,
                    SIZE_T*         pz_get_info_len)
{
LINT_SUPPRESS_BRACE(818)

    UCHAR ucVdpId;
    INT32 i4ReturnValue;

    // Check component type and id
    if (pt_comp_id == NULL)
    {
       return (RMR_DRV_INV_GET_INFO);
    }

    if (pt_comp_id->e_type != DRVT_VID_PLANE)
    {
       return (RMR_DRV_INV_GET_INFO);
    }

    ucVdpId = (UCHAR) pt_comp_id->ui2_id;

    if (ucVdpId > (VDP_COMP_NS + VDP_COMP_1))
    {
       return (RMR_DRV_INV_GET_INFO);
    }

    if (ucVdpId >= VDP_COMP_NS)
    {
       return (RMR_DRV_INV_GET_INFO);
    }

    // Check get_info
    if (pv_get_info == NULL)
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    if (pz_get_info_len == NULL)
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    i4ReturnValue = RMR_OK;
    VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    // Perform get opertion
    switch (e_get_type)
    {
        case VID_PLA_GET_TYPE_CTRL:
            {
                VID_PLA_CTRL_T* peArg;
                peArg = (VID_PLA_CTRL_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_CTRL_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_CTRL_T);

                    if (_arMwVdp[ucVdpId].ucEnable)
                    {
                        *peArg = VID_PLA_CTRL_ENABLE;
                    }
                    else
                    {
                        *peArg = VID_PLA_CTRL_DISABLE;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_MODE:
            {
                VID_PLA_MODE_T* peArg;
                peArg = (VID_PLA_MODE_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_MODE_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_MODE_T);

                    *peArg = (VID_PLA_MODE_T) _arMwVdp[ucVdpId].eMode;
                }
            }
            break;

        case VID_PLA_GET_TYPE_BG:
            {
                UINT32 u4Bg;
                VID_PLA_BG_COLOR_T* prArg;
                prArg = (VID_PLA_BG_COLOR_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_BG_COLOR_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_BG_COLOR_T);

                    if (VDP_GetBg(ucVdpId, &u4Bg) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui1_r = (u4Bg >> 16) & 0xFF;
                        prArg->ui1_g = (u4Bg >> 8) & 0xFF;
                        prArg->ui1_b = u4Bg & 0xFF;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_DISP_REGION:
            {
                VID_PLA_VID_REGION_T* prArg;

                prArg = (VID_PLA_VID_REGION_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_VID_REGION_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_VID_REGION_T);

                    *prArg = _arMwVdp[ucVdpId].rFullOutput;
                }
            }
            break;

        case VID_PLA_GET_TYPE_SRC_REGION:
            {
                VID_PLA_VID_REGION_T* prArg;

                prArg = (VID_PLA_VID_REGION_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_VID_REGION_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_VID_REGION_T);

                    *prArg = _arMwVdp[ucVdpId].rFullSource;
                }
            }
            break;

        case VID_PLA_GET_TYPE_FINAL_DISP_REGION:
            {
                VID_PLA_VID_REGION_T* prArg;

                prArg = (VID_PLA_VID_REGION_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_VID_REGION_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_VID_REGION_T);

                    *prArg = _arMwVdp[ucVdpId].rFinalOutput;
                }
            }
            break;

        case VID_PLA_GET_TYPE_FINAL_SRC_REGION:
            {
                VID_PLA_VID_REGION_T* prArg;

                prArg = (VID_PLA_VID_REGION_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_VID_REGION_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_VID_REGION_T);

                    *prArg = _arMwVdp[ucVdpId].rFinalSource;
                }
            }
            break;

        case VID_PLA_GET_TYPE_OVER_SCAN_CLIPPER:
            {
                VID_PLA_OVER_SCAN_CLIPPER_T* prArg;
                VDP_OVERSCAN_REGION_T rOverScan;

                prArg = (VID_PLA_OVER_SCAN_CLIPPER_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_OVER_SCAN_CLIPPER_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_OVER_SCAN_CLIPPER_T);

                    if (VDP_GetOverScan(ucVdpId,
                        &rOverScan) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui4_top = (UINT32) rOverScan.u4Top;
                        prArg->ui4_bottom = (UINT32) rOverScan.u4Bottom;
                        prArg->ui4_left = (UINT32) rOverScan.u4Left;
                        prArg->ui4_right = (UINT32) rOverScan.u4Right;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_CAPABILITY:
            {
                UINT32* pu4Arg;
                pu4Arg = (UINT32*) pv_get_info;

                if (*pz_get_info_len < sizeof(UINT32))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(UINT32);

                    *pu4Arg = (VID_PLA_CAP_MODE_NORMAL | VID_PLA_CAP_MODE_BLANK |
                        VID_PLA_CAP_BG | VID_PLA_CAP_DISP_REGION |
                        VID_PLA_CAP_SRC_REGION | VID_PLA_CAP_BLENDING |
                        VID_PLA_CAP_BRIGHTNESS | VID_PLA_CAP_CONTRAST |
                        VID_PLA_CAP_HUE | VID_PLA_CAP_SATURATION |
                        VID_PLA_CAP_CTI | VID_PLA_CAP_SHARPNESS |
                        VID_PLA_CAP_ETI | VID_PLA_CAP_NR | VID_PLA_CAP_BLACK_LVL_EXT |
                        VID_PLA_CAP_FLESH_TONE | VID_PLA_CAP_LUMA |
                        VID_PLA_CAP_WHITE_PEAK_LMT |
                        VID_PLA_CAP_OVER_SCAN_CLIPPER | VID_PLA_CAP_COLOR_GAIN |
                        VID_PLA_CAP_COLOR_OFFSET | VID_PLA_CAP_DISP_FMT |
			VID_PLA_CAP_H_POS | VID_PLA_CAP_V_POS );
                }
            }
            break;

        case VID_PLA_GET_TYPE_BLENDING:
        case VID_PLA_GET_TYPE_BRIGHTNESS:
        case VID_PLA_GET_TYPE_CONTRAST:
        case VID_PLA_GET_TYPE_HUE:
        case VID_PLA_GET_TYPE_SATURATION:
        case VID_PLA_GET_TYPE_CTI:
        case VID_PLA_GET_TYPE_ETI:
        case VID_PLA_GET_TYPE_SHARPNESS:
        case VID_PLA_GET_TYPE_COLOR_SUPPRESS:
        case VID_PLA_GET_TYPE_NR:
        case VID_PLA_GET_TYPE_BLACK_LVL_EXT:
        case VID_PLA_GET_TYPE_WHITE_PEAK_LMT:
        case VID_PLA_GET_TYPE_FLESH_TONE:
        case VID_PLA_GET_TYPE_LUMA:
        case VID_PLA_GET_TYPE_3D_NR:
            {
                UCHAR ucArgType;
                INT16 i2Val;
                UCHAR* pucArg;
                pucArg = (UCHAR*) pv_get_info;

                if (*pz_get_info_len < sizeof(UCHAR))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(UCHAR);

                    ucArgType = _VdpMwGetType2ArgType(e_get_type);

                    if (VDP_GetArg(ucVdpId, ucArgType, &i2Val) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        *pucArg = (UCHAR)i2Val;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_MIN_MAX:
            {
                UCHAR ucArgType;
                INT16 i2Min, i2Max, i2Dft, i2Cur;
                VID_PLA_MIN_MAX_INFO_T* prArg;
                BOOL bRet = TRUE;

                prArg = (VID_PLA_MIN_MAX_INFO_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_MIN_MAX_INFO_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_MIN_MAX_INFO_T);


		switch (prArg->e_get_type)
		{
		    case VID_PLA_GET_TYPE_BLENDING:
                        i2Min = 0;
                        i2Max = 255;
                        bRet = TRUE;
			break;
		    case VID_PLA_GET_TYPE_H_POSITION:
			if (bGetVideoDecType(ucVdpId) == SV_VD_VGA)
			{
	                        i2Min= wDrvVideoGetPorch(ucVdpId,SV_HPORCH_MIN);
	                        i2Max = wDrvVideoGetPorch(ucVdpId,SV_HPORCH_MAX);
		        }
			else if (bDrvGetPorchTune(ucVdpId) == SV_PORCHTUNE_SCPOS)//for DTV
			{
				i2Min = 0;
				i2Max = 64;
			}
			else
			{
				UINT16 def=wDrvVideoGetPorch(ucVdpId,SV_HPORCH_DEFAULT);
				i2Min = (def>32)? def-32:0;
				i2Max = (def>32)? def+32:def*2;
			}
			bRet = TRUE;
			break;
		    case VID_PLA_GET_TYPE_V_POSITION:
			if (bGetVideoDecType(ucVdpId) == SV_VD_VGA)
			{
				i2Min = wDrvVideoGetPorch(ucVdpId,SV_VPORCH_MIN);
				i2Max = wDrvVideoGetPorch(ucVdpId,SV_VPORCH_MAX);
			}
			else  if (bDrvGetPorchTune(ucVdpId) == SV_PORCHTUNE_SCPOS)//for DTV
			{
				i2Min = 0;
				i2Max = 32;
			}
			else
			{
				UINT16 def=wDrvVideoGetPorch(ucVdpId,SV_VPORCH_DEFAULT);
				i2Min = (def>16)? def-16:0;
				i2Max = (def>16)? def+16:def*2;

			}
			bRet = TRUE;
			break;

		    // [DTV00111632] [5381P]When timing=DMT1660 and aspect=ZOOM,main window(HDMI1) displays black mute.
		    // added by peggy 20080116
		    case VID_PLA_GET_TYPE_DISP_REGION:
		    	// Not Display Mode
		    	if (VDP_CheckZoom(ucVdpId) == SV_TRUE)
		    	{
		    	    i2Min = 0;
		    	    i2Max = 1000;
		    	}
		    	else
		    	{
		    	    i2Min = 1000;
		    	    i2Max = 1000;
		    	}
		    	break;

		    case VID_PLA_GET_TYPE_SRC_REGION:
		    	// Not Display Mode
		    	if (VDP_CheckZoom(ucVdpId) == SV_TRUE)
		    	{
		    	    i2Min = 0;
		    	    i2Max = 1000;
		    	}
		    	else
		    	{
		    	    i2Min = 1000;
		    	    i2Max = 1000;
		    	}
		    	break;

		    default:
                        ucArgType = _VdpMwGetType2ArgType(prArg->e_get_type);
                        bRet = VDP_GetUiVqItemMinMaxDftCur(ucVdpId, ucArgType,
                                &i2Min, &i2Max, &i2Dft, &i2Cur);
			break;
		}


                    UNUSED(i2Dft);
                    UNUSED(i2Cur);

                    if (bRet != TRUE)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui4_min_value = i2Min;
                        prArg->ui4_max_value = i2Max;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_MAX:
            {
                UCHAR ucArgType;
                BOOL bRet;
                INT16 i2Min, i2Max, i2Dft, i2Cur;
                VID_PLA_MAX_INFO_T* prArg;

                prArg = (VID_PLA_MAX_INFO_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_MAX_INFO_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_MAX_INFO_T);

                    ucArgType = _VdpMwGetType2ArgType(prArg->e_get_type);

                    bRet = VDP_GetUiVqItemMinMaxDftCur(ucVdpId, ucArgType,
                                &i2Min, &i2Max, &i2Dft, &i2Cur);

                    UNUSED(i2Min);
                    UNUSED(i2Dft);
                    UNUSED(i2Cur);

                    if (bRet != TRUE)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui4_max_value = i2Max;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_COLOR_GAIN:
            {
                VID_PLA_COLOR_GAIN_T* prArg;
                INT16 i2Gain;

                prArg = (VID_PLA_COLOR_GAIN_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_COLOR_GAIN_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_COLOR_GAIN_T);

                    // Get R Gain
                    if (VDP_GetArg(ucVdpId, VDP_ARG_R_GAIN, &i2Gain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui1_r_gain = (UINT8)i2Gain;
                    }

                    // Get G Gain
                    if (VDP_GetArg(ucVdpId, VDP_ARG_G_GAIN, &i2Gain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui1_g_gain = (UINT8)i2Gain;
                    }

                    // Get B Gain
                    if (VDP_GetArg(ucVdpId, VDP_ARG_B_GAIN, &i2Gain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui1_b_gain = (UINT8)i2Gain;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_COLOR_GAIN_MAX:
        case VID_PLA_GET_TYPE_COLOR_OFFSET_MAX:
            {
                UCHAR ucArgType;
                BOOL bRet;
                INT16 i2Min, i2Max, i2Dft, i2Cur;
                VID_PLA_COLOR_GAIN_T* prArg;

                prArg = (VID_PLA_COLOR_GAIN_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_COLOR_GAIN_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_COLOR_GAIN_T);

                    ucArgType = _VdpMwGetType2ArgType(e_get_type);

                    bRet = VDP_GetUiVqItemMinMaxDftCur(ucVdpId, ucArgType,
                                &i2Min, &i2Max, &i2Dft, &i2Cur);

                    UNUSED(i2Min);
                    UNUSED(i2Dft);
                    UNUSED(i2Cur);

                    if (bRet != TRUE)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui1_r_gain = i2Max;
                        prArg->ui1_g_gain = i2Max;
                        prArg->ui1_b_gain = i2Max;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_COLOR_GAIN_MIN:
        case VID_PLA_GET_TYPE_COLOR_OFFSET_MIN:
            {
                UCHAR ucArgType;
                BOOL bRet;
                INT16 i2Min, i2Max, i2Dft, i2Cur;
                VID_PLA_COLOR_GAIN_T* prArg;

                prArg = (VID_PLA_COLOR_GAIN_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_COLOR_GAIN_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_COLOR_GAIN_T);

                    ucArgType = _VdpMwGetType2ArgType(e_get_type);

                    bRet = VDP_GetUiVqItemMinMaxDftCur(ucVdpId, ucArgType,
                                &i2Min, &i2Max, &i2Dft, &i2Cur);

                    UNUSED(i2Max);
                    UNUSED(i2Dft);
                    UNUSED(i2Cur);

                    if (bRet != TRUE)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui1_r_gain = i2Min;
                        prArg->ui1_g_gain = i2Min;
                        prArg->ui1_b_gain = i2Min;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_COLOR_OFFSET:
            {
                VID_PLA_COLOR_GAIN_T* prArg;
                INT16 i2Offset;

                prArg = (VID_PLA_COLOR_GAIN_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_COLOR_GAIN_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_COLOR_GAIN_T);

                    // Get R Offset
                    if (VDP_GetArg(ucVdpId, VDP_ARG_R_OFFSET, &i2Offset) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui1_r_gain = (UINT8)i2Offset;
                    }

                    // Get G Offset
                    if (VDP_GetArg(ucVdpId, VDP_ARG_G_OFFSET, &i2Offset) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui1_g_gain = (UINT8)i2Offset;
                    }

                    // Get B Offset
                    if (VDP_GetArg(ucVdpId, VDP_ARG_B_OFFSET, &i2Offset) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui1_b_gain = (UINT8)i2Offset;
                    }
                }
            }
            break;

		case VID_PLA_GET_TYPE_DISP_FMT:
			{
				VID_PLA_DISP_FMT_T* peArg;
				peArg = (VID_PLA_DISP_FMT_T*) pv_get_info;

			    if (*pz_get_info_len < sizeof(VID_PLA_DISP_FMT_T))
			    {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
			    }
			    else
			    {
    			    *pz_get_info_len = sizeof(VID_PLA_DISP_FMT_T);

                    *peArg = _arMwVdp[ucVdpId].eFmt;
			    }
			}
			break;

        case VID_PLA_GET_TYPE_LETTER_BOX_DETECT:
             {
                UCHAR* pucArg;
                pucArg = (UCHAR*) pv_get_info;

                if (*pz_get_info_len < sizeof(UCHAR))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(UCHAR);

                    *pucArg = _arMwVdp[ucVdpId].ucLbEnable;
                }
            }
            break;

        case VID_PLA_GET_TYPE_DYNAMIC_SCALING:
             {
                UINT16* pu2Arg;
                pu2Arg = (UINT16*) pv_get_info;

                if (*pz_get_info_len < sizeof(UINT16))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(UINT16);

                    *pu2Arg = _arMwVdp[ucVdpId].u2DynamicScale;
                }
            }
            break;

        case VID_PLA_GET_TYPE_SPLIT_SCRN_DEMO:
            {
                VID_PLA_SPLIT_SCRN_DEMO_T* peArg;
                peArg = (VID_PLA_SPLIT_SCRN_DEMO_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_SPLIT_SCRN_DEMO_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_SPLIT_SCRN_DEMO_T);

                    *peArg = _arMwVdp[ucVdpId].eDemo;
                }
             }
            break;

  	    case VID_PLA_GET_TYPE_FILM_MODE:
             {
                UCHAR* pucArg;
                BOOL b32OnOff;

                pucArg = (UCHAR*) pv_get_info;
		  b32OnOff = (BOOL)VDP_GetDIFilm32Enable(ucVdpId) ;

                if (*pz_get_info_len < sizeof(BOOL))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(BOOL);

                    *pucArg = b32OnOff ;
                }
            }
            break;

	case VID_PLA_GET_TYPE_H_POSITION:
	{
		 INT32 pos,min_off,max,min,def;

		 def=wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_DEFAULT);
		 max=wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_MAX);
		 min=wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_MIN);
		 pos=wDrvVideoGetPorch((UINT8)ucVdpId,SV_HPORCH_CURRENT);

		 if (bDrvGetPorchTune((UINT8)ucVdpId) == SV_PORCHTUNE_SCPOS)//for DTV
		 {
			*(UINT16*)pv_get_info=MAX(32+pos-def,0);
		 }
		 else
		 {
			 if (bGetVideoDecType(ucVdpId) == SV_VD_VGA)
			 {
				 min_off=pos-min;
				 pos=max-min_off;
			 }
			 else
			 {
				 min_off=pos-def;
				 pos=def-min_off;
			 }
			 if (pos<min)
			 {
				 pos=min;
			 }
			 if (pos>max)
			 {
				 pos=max;
			 }
			 *(UINT16*)pv_get_info =pos;
		 }
		 *pz_get_info_len = sizeof(UINT16);
	 }
	    break;
	case VID_PLA_GET_TYPE_V_POSITION:
	    if (bDrvGetPorchTune((UINT8)ucVdpId) == SV_PORCHTUNE_SCPOS)//for DTV
	     {
		    UINT16 def,pos;
		    def=wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_DEFAULT);
		    pos=wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_CURRENT);
		    *(UINT16*)pv_get_info = MAX(16+pos-def,0);
	     }
	     else
	     {
	     	*(UINT16*)pv_get_info =wDrvVideoGetPorch(ucVdpId,SV_VPORCH_CURRENT);
	     }
	     *pz_get_info_len = sizeof(UINT16);
	    break;
	case VID_PLA_GET_TYPE_HDMI_MODE:
	     switch(bApiVideoFixColorCurState(ucVdpId)&FIXCOLORSPACE_MODEMASK)
	     {
	     	case SV_FIXCOLORSPACE_OFF: *(UINT8*)pv_get_info=VID_PLA_HDMI_MODE_AUTO; break;
		case SV_FIXCOLORSPACE_RGB_444: *(UINT8*)pv_get_info=VID_PLA_HDMI_MODE_GRAPHIC; break;
		case SV_FIXCOLORSPACE_RGB_422: *(UINT8*)pv_get_info=VID_PLA_HDMI_MODE_VIDEO; break;
		case SV_FIXCOLORSPACE_YCBCR: *(UINT8*)pv_get_info=VID_PLA_HDMI_MODE_VIDEO; break;
	     }
	     *pz_get_info_len = sizeof(UINT8);
	    break;
        case VID_PLA_GET_TYPE_ENHANCE:
        case VID_PLA_GET_TYPE_QV_INP:
        default:
            i4ReturnValue = RMR_DRV_INV_GET_INFO;
            break;
    }

    VERIFY(x_sema_unlock(_hMutex) == OSR_OK);

    return (i4ReturnValue);
}

//-----------------------------------------------------------------------------
/** Brief of _VdpSet.
 */
//-----------------------------------------------------------------------------
static INT32 _VdpSet(DRV_COMP_ID_T*  pt_comp_id,
                    DRV_SET_TYPE_T  e_set_type,
                    const VOID*     pv_set_info,
                    SIZE_T          z_set_info_len)
{
LINT_SUPPRESS_BRACE(818)

    UCHAR ucVdpId;
    INT32 i4ReturnValue;

    // Check component type and id
    if (pt_comp_id == NULL)
    {
       return (RMR_DRV_INV_SET_INFO);
    }

    if (pt_comp_id->e_type != DRVT_VID_PLANE)
    {
       return (RMR_DRV_INV_SET_INFO);
    }

    ucVdpId = (UCHAR) pt_comp_id->ui2_id;
    if (ucVdpId > (VDP_COMP_NS + VDP_COMP_1))
    {
       return (RMR_DRV_INV_SET_INFO);
    }

    if (ucVdpId >= VDP_COMP_NS)
    {
       return (RMR_DRV_INV_SET_INFO);
    }

    // Check set_info
    if (z_set_info_len == 0)
    {
        return (RMR_DRV_INV_SET_INFO);
    }

    i4ReturnValue = RMR_OK;
    VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    LOG(3, "VDP SET(%d) ARG(%x)\n", (UINT32) (e_set_type - VID_PLA_SET_TYPE_CTRL), (UINT32) pv_set_info);

    // Perform set opertion
    switch (e_set_type)
    {
        case VID_PLA_SET_TYPE_CTRL:
            {
LINT_SUPPRESS_BRACE(549)

                VID_PLA_CTRL_T eArg;
                eArg = (VID_PLA_CTRL_T) (UINT32) pv_set_info;

                if (eArg == VID_PLA_CTRL_ENABLE)
                {
                    _arMwVdp[ucVdpId].ucEnable = 1;
                    if (_VdpSetEnable(ucVdpId) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }
                else
                if (eArg == VID_PLA_CTRL_DISABLE)
                {
                    _arMwVdp[ucVdpId].ucEnable = 0;

                    _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_UNKNOWN;
                    _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.ui2_pal_secam_wss = 0;
                    _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.e_scart_pin_8 = DRV_CUSTOM_SCART_PIN_8_UNKNOWN;

                    if (_VdpSetEnable(ucVdpId) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }
                else
                {
                    UCHAR ucConnect;

                    ucConnect = _arMwVdp[ucVdpId].ucConnect;
                    _arMwVdp[ucVdpId] = _rMwVdpDefault;
                    _arMwVdp[ucVdpId].ucConnect = ucConnect;
                    VDP_Reset(ucVdpId);
                }
            }
            break;

        case VID_PLA_SET_TYPE_MODE:
            {
LINT_SUPPRESS_BRACE(549)

                VID_PLA_MODE_T eArg;
                eArg = (VID_PLA_MODE_T) (UINT32) pv_set_info;

                if ((_arMwVdp[ucVdpId].eMode == VID_PLA_FREEZE) &&
                    (eArg != VID_PLA_FREEZE))
                {
                    if (VDP_SetFreeze(ucVdpId, 0) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }

                if (eArg == VID_PLA_FREEZE)
                {
                    if (VDP_SetFreeze(ucVdpId, 1) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    if (VDP_SetMode(ucVdpId, VDP_MODE_DEINT) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }
                else
                if (VDP_SetMode(ucVdpId, (UCHAR)eArg) != VDP_SET_OK)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
                else
                {
                    if (_SetInput(ucVdpId) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }

                _arMwVdp[ucVdpId].eMode = eArg;
            }
            break;

        case VID_PLA_SET_TYPE_BG:
            {
                UINT32 u4Bg;
                VID_PLA_BG_COLOR_T* prArg;

                if (pv_set_info == NULL)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
                else
                {
                    prArg = (VID_PLA_BG_COLOR_T*) pv_set_info;

                    u4Bg = ((((UINT32)prArg->ui1_r) << 16) + (((UINT32)prArg->ui1_g) << 8) + ((UINT32)prArg->ui1_b));

                    if (VDP_SetBg(ucVdpId, u4Bg) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }
            }
            break;

        case VID_PLA_SET_TYPE_DISP_REGION:
            {
                VID_PLA_VID_REGION_T* prArg;
                VDP_REGION_T rOutRegion;

                if (pv_set_info == NULL)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
                else
                {
#if 1
                    prArg = (VID_PLA_VID_REGION_T*) pv_set_info;

                    _arMwVdp[ucVdpId].rFullOutput = *prArg;

                    // Final = Translate + Full
                    _VdpCalculateOutputRegion(ucVdpId);

                    rOutRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_x;
                    rOutRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_y;
                    rOutRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_width;
                    rOutRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_height;

                    if (VDP_SetOutRegion(ucVdpId, 0, rOutRegion) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
#else
                    prArg = (VID_PLA_VID_REGION_T*) pv_set_info;

                    rOutRegion.u4X = (UINT32) prArg->ui4_x;
                    rOutRegion.u4Y = (UINT32) prArg->ui4_y;
                    rOutRegion.u4Width = (UINT32) prArg->ui4_width;
                    rOutRegion.u4Height = (UINT32) prArg->ui4_height;

                    if (VDP_SetOutRegion(ucVdpId,
                        (UCHAR)prArg->b_set_to_full_scr,
                        rOutRegion) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    _arMwVdp[ucVdpId].rFullOutput = *prArg;
#endif
                }
            }
            break;

        case VID_PLA_SET_TYPE_SRC_REGION:
            {
                VID_PLA_VID_REGION_T* prArg;
                VDP_REGION_T rSrcRegion;

                if (pv_set_info == NULL)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
                else
                {
#if 1
                    prArg = (VID_PLA_VID_REGION_T*) pv_set_info;

                    _arMwVdp[ucVdpId].rFullSource = *prArg;

                    // Final = Translate + Full
                    _VdpCalculateSourceRegion(ucVdpId);

                    rSrcRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_x;
                    rSrcRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_y;
                    rSrcRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_width;
                    rSrcRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_height;

                    if (VDP_SetSrcRegion(ucVdpId, 0, rSrcRegion) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
#else
                    prArg = (VID_PLA_VID_REGION_T*) pv_set_info;

                    rSrcRegion.u4X = (UINT32) prArg->ui4_x;
                    rSrcRegion.u4Y = (UINT32) prArg->ui4_y;
                    rSrcRegion.u4Width = (UINT32) prArg->ui4_width;
                    rSrcRegion.u4Height = (UINT32) prArg->ui4_height;

                    if (VDP_SetSrcRegion(ucVdpId,
                        (UCHAR)prArg->b_set_to_full_scr,
                        rSrcRegion) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
#endif
                }
            }
            break;

        case VID_PLA_SET_TYPE_OVER_SCAN_CLIPPER:
            {
                VID_PLA_OVER_SCAN_CLIPPER_T* prArg;
                VDP_OVERSCAN_REGION_T rOverScan;

                if (pv_set_info == NULL)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
                else
                {
                    prArg = (VID_PLA_OVER_SCAN_CLIPPER_T*) pv_set_info;

                    rOverScan.u4Top = (UINT32) prArg->ui4_top;
                    rOverScan.u4Bottom = (UINT32) prArg->ui4_bottom;
                    rOverScan.u4Left = (UINT32) prArg->ui4_left;
                    rOverScan.u4Right = (UINT32) prArg->ui4_right;

                    if (_pfUpdateOverscanTbl != NULL)
                    {
                        DRV_CUST_INPUT_SRC_TYPE_T eSrcType;
                        DRV_CUSTOM_TIMING_INFO_T rTimingInfo;

                        eSrcType = _VdpGetDrvCustInputSrcType(_arMwVdp[ucVdpId].e_type);

                        rTimingInfo.ui2_width = _arMwVdp[ucVdpId].rSrcRes.ui2_width;
                        rTimingInfo.ui2_height = _arMwVdp[ucVdpId].rSrcRes.ui2_height;
                        rTimingInfo.ui4_fram_rate = _arMwVdp[ucVdpId].u4FrameRate;

                        if (_arMwVdp[ucVdpId].u4Interlace)
                        {
                            rTimingInfo.e_scan_mode = DRV_CUSTOM_TIMING_SCAN_MODE_INTERLACE;
                        }
                        else
                        {
                            rTimingInfo.e_scan_mode = DRV_CUSTOM_TIMING_SCAN_MODE_PROGRESSIVE;
                        }

                        _pfUpdateOverscanTbl(
                            ucVdpId,
                            eSrcType,
                            &rTimingInfo,
                            rOverScan.u4Top,
                            rOverScan.u4Bottom,
                            rOverScan.u4Left,
                            rOverScan.u4Right
                            );
                    }

                    if (VDP_SetOverScan(ucVdpId,
                        rOverScan) != VDP_SET_OK)
                    {
                        // if video plane is NOT connect (switch), it will return fail.
                        // ignore it!
                        // i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }
            }
            break;

        case VID_PLA_SET_TYPE_BLENDING:
        case VID_PLA_SET_TYPE_BRIGHTNESS:
        case VID_PLA_SET_TYPE_CONTRAST:
        case VID_PLA_SET_TYPE_HUE:
        case VID_PLA_SET_TYPE_SATURATION:
        case VID_PLA_SET_TYPE_CTI:
        case VID_PLA_SET_TYPE_ETI:
        case VID_PLA_SET_TYPE_SHARPNESS:
        case VID_PLA_SET_TYPE_COLOR_SUPPRESS:
        case VID_PLA_SET_TYPE_NR:
        case VID_PLA_SET_TYPE_BLACK_LVL_EXT:
        case VID_PLA_SET_TYPE_WHITE_PEAK_LMT:
        case VID_PLA_SET_TYPE_FLESH_TONE:
        case VID_PLA_SET_TYPE_LUMA:
        case VID_PLA_SET_TYPE_3D_NR:
            {
LINT_SUPPRESS_BRACE(507)

                UCHAR ucArgType;
                UCHAR ucArg;

                ucArg = (UCHAR) (UINT32) pv_set_info;
                ucArgType = _VdpMwSetType2ArgType(e_set_type);

                if (VDP_SetArg(ucVdpId, ucArgType, (INT16)ucArg) != VDP_SET_OK)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
             }
            break;

        case VID_PLA_SET_TYPE_COLOR_GAIN:
            {
                VID_PLA_COLOR_GAIN_T* prArg;
                VDP_COLOR_GAIN_T rGain;

                if (pv_set_info == NULL)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
                else
                {
                    prArg = (VID_PLA_COLOR_GAIN_T*) pv_set_info;

                    rGain.ucRgain = (UINT8) prArg->ui1_r_gain;
                    rGain.ucGgain = (UINT8) prArg->ui1_g_gain;
                    rGain.ucBgain = (UINT8) prArg->ui1_b_gain;

                    // R Gain
                    if (VDP_SetArg(ucVdpId, VDP_ARG_R_GAIN, (INT16)rGain.ucRgain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    // G Gain
                    if (VDP_SetArg(ucVdpId, VDP_ARG_G_GAIN, (INT16)rGain.ucGgain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    // B Gain
                    if (VDP_SetArg(ucVdpId, VDP_ARG_B_GAIN, (INT16)rGain.ucBgain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                }
            }
            break;

        case VID_PLA_SET_TYPE_COLOR_OFFSET:
            {
                VID_PLA_COLOR_GAIN_T* prArg;
                VDP_COLOR_GAIN_T rOffset;

                if (pv_set_info == NULL)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
                else
                {
                    prArg = (VID_PLA_COLOR_GAIN_T*) pv_set_info;

                    rOffset.ucRgain = (UINT8) prArg->ui1_r_gain;
                    rOffset.ucGgain = (UINT8) prArg->ui1_g_gain;
                    rOffset.ucBgain = (UINT8) prArg->ui1_b_gain;

                    // R Offset
                    if (VDP_SetArg(ucVdpId, VDP_ARG_R_OFFSET, (INT16)rOffset.ucRgain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    // G Offset
                    if (VDP_SetArg(ucVdpId, VDP_ARG_G_OFFSET, (INT16)rOffset.ucGgain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    // B Offset
                    if (VDP_SetArg(ucVdpId, VDP_ARG_B_OFFSET, (INT16)rOffset.ucBgain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }
            }
            break;

		case VID_PLA_SET_TYPE_DISP_FMT:
			{
LINT_SUPPRESS_BRACE(549)

#if 1
				VID_PLA_DISP_FMT_T eArg;
				eArg = (VID_PLA_DISP_FMT_T) (UINT32) pv_set_info;

                _arMwVdp[ucVdpId].eFmt = eArg;

                if ((eArg == VID_PLA_DISP_FMT_NON_LINEAR_ZOOM) ||
                    (eArg == VID_PLA_DISP_FMT_NLZ_CUSTOM_0) ||
                    (eArg == VID_PLA_DISP_FMT_NLZ_CUSTOM_1) ||
                    (eArg == VID_PLA_DISP_FMT_NLZ_CUSTOM_2) ||
                    (eArg == VID_PLA_DISP_FMT_NLZ_CUSTOM_3))
                {
                    // turn off 121
                    if (VDP_Set121Map(ucVdpId, 0) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                    // turn on non-linear
                    if (VDP_SetNonlinear(ucVdpId, 1) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }
                else
                if (eArg == VID_PLA_DISP_FMT_DOT_BY_DOT)
                {
                    // turn off non-linear
                    if (VDP_SetNonlinear(ucVdpId, 0) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    // turn on 121
                    if (VDP_Set121Map(ucVdpId, 1) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }
                else
                {
                    // turn off 121
                    if (VDP_Set121Map(ucVdpId, 0) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    // turn off non-linear
                    if (VDP_SetNonlinear(ucVdpId, 0) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }

                if (_VdpXlatDispFmtCallBackMw(ucVdpId) != 0)
                {
                    VDP_REGION_T rSrcRegion;
                    VDP_REGION_T rOutRegion;

                    rSrcRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_x;
                    rSrcRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_y;
                    rSrcRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_width;
                    rSrcRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_height;

                    if (VDP_SetSrcRegion(ucVdpId, 0, rSrcRegion) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    rOutRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_x;
                    rOutRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_y;
                    rOutRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_width;
                    rOutRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_height;

                    if (VDP_SetOutRegion(ucVdpId, 0, rOutRegion) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }

                {
                    VID_PLA_VID_REGION_T rSrcReg;
                    VID_PLA_VID_REGION_T rDispReg;
                    DRV_CUSTOM_VID_SRC_INFO_T rVidSrcInfo;

                    rVidSrcInfo.e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_INPUT_SRC;
                    rVidSrcInfo.u.t_input_src.e_input_source = _VdpGetDrvCustInputSrcType(_arMwVdp[ucVdpId].e_type);
                    rVidSrcInfo.u.t_input_src.ui2_id = _arMwVdp[ucVdpId].ucMpvCompId;
                    rVidSrcInfo.u.t_input_src.e_timing_type = _arMwVdp[ucVdpId].e_timing;

                    _pfXlatDispFmtExFct[(UINT32)_arMwVdp[ucVdpId].eFmt](
                        &_arMwVdp[ucVdpId].rSrcRes,
                        &_arMwVdp[ucVdpId].rOutRes,
                        &rVidSrcInfo,
                        &rSrcReg,
                        &rDispReg);
                }
#else
				VID_PLA_DISP_FMT_T eArg;
				eArg = (VID_PLA_DISP_FMT_T) (UINT32) pv_set_info;

                _arMwVdp[ucVdpId].eFmt = eArg;
#endif
			}
			break;

        case VID_PLA_SET_TYPE_LETTER_BOX_DETECT:
            {
LINT_SUPPRESS_BRACE(507)

                UCHAR ucLb;

                ucLb = (UCHAR) (UINT32) pv_set_info;

                _arMwVdp[ucVdpId].ucLbEnable = ucLb;

                if (VDP_SetLBoxDetection(ucVdpId, ucLb, 0) != VDP_SET_OK)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
             }
            break;

        case VID_PLA_SET_TYPE_DYNAMIC_SCALING:
            {
LINT_SUPPRESS_BRACE(507)

                UINT16 u2Delay;

                u2Delay = (UINT16) (UINT32) pv_set_info;

                _arMwVdp[ucVdpId].u2DynamicScale = u2Delay;

                if (u2Delay != 0)
                {
                    if (VDP_SetDynamiceScaler(ucVdpId, 1, (UINT32)u2Delay) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }
                else
                {
                    if (VDP_SetDynamiceScaler(ucVdpId, 0, 250) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }
             }
            break;

        case VID_PLA_SET_TYPE_SPLIT_SCRN_DEMO:
            {
LINT_SUPPRESS_BRACE(507)

                UCHAR ucArgType;
                UCHAR ucArg;

                _arMwVdp[ucVdpId].eDemo = (VID_PLA_SPLIT_SCRN_DEMO_T) (UINT32) pv_set_info;

                ucArg = (UCHAR) (UINT32) pv_set_info;
                ucArgType = _VdpMwSetType2ArgType(e_set_type);

                if (VDP_SetArg(ucVdpId, ucArgType, (INT16)ucArg) != VDP_SET_OK)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }

             }
            break;

  	    case VID_PLA_SET_TYPE_FILM_MODE:
             {

                BOOL b32OnOff;

                b32OnOff = (BOOL) (UINT32) pv_set_info;

                if (VDP_SetDIFilm32Enable(ucVdpId, (UINT8)b32OnOff) != VDP_SET_OK)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
             }
            break;

        case VID_PLA_SET_TYPE_H_POSITION:
    	{
		INT32 pos,min_off,max,min,def;

		def=wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_DEFAULT);
		max=wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_MAX);
		min=wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_MIN);

		if (bDrvGetPorchTune((UINT8)ucVdpId) == SV_PORCHTUNE_SCPOS)//for DTV
		{
		       pos=32-(UINT32)pv_set_info+def;
		       wDrvVideoSetPorch(ucVdpId, SV_HPORCH_CURRENT,pos);
		}
		else
		{
			if (bGetVideoDecType(ucVdpId) == SV_VD_VGA)
			{
				min_off=(INT32)pv_set_info-min;
				pos=max-min_off;
			}
			else
			{
				min_off=(INT32)pv_set_info-def;
				pos=def-min_off;
			}
	    		wDrvVideoSetPorch(ucVdpId, SV_HPORCH_CURRENT, pos);
	    	}
	}
            break;
        case VID_PLA_SET_TYPE_V_POSITION:
		if (bDrvGetPorchTune((UINT8)ucVdpId) == SV_PORCHTUNE_SCPOS)//for DTV
		{
			UINT16 def=wDrvVideoGetPorch(ucVdpId,SV_VPORCH_DEFAULT);
			wDrvVideoSetPorch(ucVdpId, SV_VPORCH_CURRENT, (INT32)pv_set_info-16+def);
		}
		else
		{
			wDrvVideoSetPorch(ucVdpId, SV_VPORCH_CURRENT, (UINT16)(UINT32)pv_set_info);
		}
            break;
	case VID_PLA_SET_TYPE_HDMI_MODE:
	     VERIFY(x_sema_unlock(_hMutex) == OSR_OK); /* fix lock sequence of VDP & NPTV src switch */
	     switch((UINT32)pv_set_info)
	     {
	     	case VID_PLA_HDMI_MODE_AUTO:
			vApiVideoFixColorSpace(ucVdpId,SV_FIXCOLORSPACE_OFF);
			break;
		case VID_PLA_HDMI_MODE_GRAPHIC:
			vApiVideoFixColorSpace(ucVdpId, bHDMIInputType()? SV_FIXCOLORSPACE_RBG444_ONCE : SV_FIXCOLORSPACE_OFF);
			break;
		case VID_PLA_HDMI_MODE_VIDEO:
			vApiVideoFixColorSpace(ucVdpId, bHDMIInputType()? SV_FIXCOLORSPACE_RBG422_ONCE : SV_FIXCOLORSPACE_YCBCR_ONCE);
		 	break;
	     }
	     VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK); /* fix lock sequence of VDP & NPTV src switch */
	    break;
        case VID_PLA_SET_TYPE_ENHANCE:
        case VID_PLA_SET_TYPE_QV_INP:
        default:
            i4ReturnValue = RMR_DRV_INV_SET_INFO;
            break;
    }

    VERIFY(x_sema_unlock(_hMutex) == OSR_OK);

    return (i4ReturnValue);
}

//-----------------------------------------------------------------------------
/** Brief of _SetInput.
 */
//-----------------------------------------------------------------------------
static UINT32 _SetInput(UCHAR ucVdpId)
{
    UCHAR ucEsId;

    ucEsId = _arMwVdp[ucVdpId].ucEsId;

    if (VDP_SetInput(ucVdpId, ucEsId, 0) != VDP_SET_OK)
    {
        return VDP_SET_ERROR;
    }

	return VDP_SET_OK;
}

//-----------------------------------------------------------------------------
/** Brief of _VdpSetEnable.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpSetEnable(UCHAR ucVdpId)
{
    if (_arMwVdp[ucVdpId].ucConnect && _arMwVdp[ucVdpId].ucEnable)
    {
        return VDP_SetEnable(ucVdpId, 1);
    }
    else
    {
        return VDP_SetEnable(ucVdpId, 0);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VdpMwGetType2ArgType.
 */
//-----------------------------------------------------------------------------
static UCHAR _VdpMwGetType2ArgType(UINT32 u4GetType)
{
    switch (u4GetType)
    {
        case VID_PLA_GET_TYPE_BLENDING:     // blending
            return VDP_ARG_BLENDING;

        case VID_PLA_GET_TYPE_BRIGHTNESS:   // brightness
            return VDP_ARG_BRIGHTNESS;

        case VID_PLA_GET_TYPE_CONTRAST:     // contrast
            return VDP_ARG_CONTRAST;

        case VID_PLA_GET_TYPE_HUE:          // hue
            return VDP_ARG_HUE;

        case VID_PLA_GET_TYPE_SATURATION:   // saturation
            return VDP_ARG_SATURATION;

        case VID_PLA_GET_TYPE_CTI:          // cti
            return VDP_ARG_CTI;

        case VID_PLA_GET_TYPE_ETI:          // eti
            return VDP_ARG_ETI;

        case VID_PLA_GET_TYPE_SHARPNESS:    // sharpness (v, h)
            return VDP_ARG_SHARPNESS;

        case VID_PLA_GET_TYPE_COLOR_GAIN:   // color gain
        case VID_PLA_GET_TYPE_COLOR_GAIN_MIN:   // color gain
        case VID_PLA_GET_TYPE_COLOR_GAIN_MAX:   // color gain
            return VDP_ARG_R_GAIN;

        case VID_PLA_GET_TYPE_COLOR_OFFSET: // color offset
        case VID_PLA_GET_TYPE_COLOR_OFFSET_MIN: // color offset
        case VID_PLA_GET_TYPE_COLOR_OFFSET_MAX: // color offset
            return VDP_ARG_R_OFFSET;

        case VID_PLA_GET_TYPE_COLOR_SUPPRESS:
            return VDP_ARG_COLOR_SUPPRESS;

        case VID_PLA_GET_TYPE_NR:           // noise reduction
            return VDP_ARG_NR;

        case VID_PLA_GET_TYPE_BLACK_LVL_EXT:// black level extension
            return VDP_ARG_BLE;

        case VID_PLA_GET_TYPE_WHITE_PEAK_LMT:   // white peak limit
            return VDP_ARG_WHITE_PEAK_LMT;

        case VID_PLA_GET_TYPE_FLESH_TONE:   // second color enhance
            return VDP_ARG_SCE;

        case VID_PLA_GET_TYPE_LUMA:         // adaptive luma
            return VDP_ARG_ADAPTIVE_LUMA;

        case VID_PLA_GET_TYPE_3D_NR:        // 3D noise reduction
            return VDP_ARG_3D_NR;

        case VID_PLA_GET_TYPE_SPLIT_SCRN_DEMO:
            return VDP_ARG_DEMO;

        default:
            return VDP_ARG_NS;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VdpMwSetType2ArgType.
 */
//-----------------------------------------------------------------------------
static UCHAR _VdpMwSetType2ArgType(UINT32 u4SetType)
{
    switch (u4SetType)
    {
        case VID_PLA_SET_TYPE_BLENDING:     // blending
            return VDP_ARG_BLENDING;

        case VID_PLA_SET_TYPE_BRIGHTNESS:   // brightness
            return VDP_ARG_BRIGHTNESS;

        case VID_PLA_SET_TYPE_CONTRAST:     // contrast
            return VDP_ARG_CONTRAST;

        case VID_PLA_SET_TYPE_HUE:          // hue
            return VDP_ARG_HUE;

        case VID_PLA_SET_TYPE_SATURATION:   // saturation
            return VDP_ARG_SATURATION;

        case VID_PLA_SET_TYPE_CTI:          // cti
            return VDP_ARG_CTI;

        case VID_PLA_SET_TYPE_ETI:          // eti
            return VDP_ARG_ETI;

        case VID_PLA_SET_TYPE_SHARPNESS:    // sharpness (v, h)
            return VDP_ARG_SHARPNESS;

        case VID_PLA_SET_TYPE_COLOR_SUPPRESS:
            return VDP_ARG_COLOR_SUPPRESS;

        case VID_PLA_SET_TYPE_NR:           // noise reduction
            return VDP_ARG_NR;

        case VID_PLA_SET_TYPE_BLACK_LVL_EXT:// black level extension
            return VDP_ARG_BLE;

        case VID_PLA_SET_TYPE_FLESH_TONE:   // second color enhance
            return VDP_ARG_SCE;

        case VID_PLA_SET_TYPE_LUMA:         // adaptive luma
            return VDP_ARG_ADAPTIVE_LUMA;

        case VID_PLA_SET_TYPE_WHITE_PEAK_LMT:   // white peak limit
            return VDP_ARG_WHITE_PEAK_LMT;

        case VID_PLA_SET_TYPE_3D_NR:        // 3D noise reduction
            return VDP_ARG_3D_NR;

        case VID_PLA_SET_TYPE_SPLIT_SCRN_DEMO:        // 3D noise reduction
            return VDP_ARG_DEMO;

        default:
            return VDP_ARG_NS;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VdpSourceChangeNotify.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpSourceChangeNotify(UCHAR ucVdpId, VDP_SOURCE_INFO_T rVdpSrcInfo)
{
#ifdef CC_VDP_MW_T1

    static UINT32 u4State = 0;

    if (u4State == 0)
    {
        _prVdpConf[ucVdpId]->rOutRegion.u4X = 0;
        _prVdpConf[ucVdpId]->rOutRegion.u4Y = 0;
        _prVdpConf[ucVdpId]->rOutRegion.u4Width = 500;
        _prVdpConf[ucVdpId]->rOutRegion.u4Height = 500;
    }
    else
    if (u4State == 1)
    {
        _prVdpConf[ucVdpId]->rOutRegion.u4X = 500;
        _prVdpConf[ucVdpId]->rOutRegion.u4Y = 0;
        _prVdpConf[ucVdpId]->rOutRegion.u4Width = 500;
        _prVdpConf[ucVdpId]->rOutRegion.u4Height = 500;
    }
    else
    if (u4State == 2)
    {
        _prVdpConf[ucVdpId]->rOutRegion.u4X = 0;
        _prVdpConf[ucVdpId]->rOutRegion.u4Y = 500;
        _prVdpConf[ucVdpId]->rOutRegion.u4Width = 500;
        _prVdpConf[ucVdpId]->rOutRegion.u4Height = 500;
    }
    else
    if (u4State >= 3)
    {
        _prVdpConf[ucVdpId]->rOutRegion.u4X = 500;
        _prVdpConf[ucVdpId]->rOutRegion.u4Y = 500;
        _prVdpConf[ucVdpId]->rOutRegion.u4Width = 500;
        _prVdpConf[ucVdpId]->rOutRegion.u4Height = 500;
    }

    if (++u4State > 3)
    {
        u4State = 0;
    }

    return 1;
#else
    BOOL fgRetTrue = 0;

    VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    _arMwVdp[ucVdpId].rSrcRes.ui2_width = (UINT16)rVdpSrcInfo.u4SrcWidth;
    _arMwVdp[ucVdpId].rSrcRes.ui2_height = (UINT16)rVdpSrcInfo.u4SrcHeight;
    _arMwVdp[ucVdpId].rOutRes.ui2_width = (UINT16)rVdpSrcInfo.u4OutWidth;
    _arMwVdp[ucVdpId].rOutRes.ui2_height = (UINT16)rVdpSrcInfo.u4OutHeight;
    _arMwVdp[ucVdpId].u4FrameRate = rVdpSrcInfo.u4FrameRate;
    _arMwVdp[ucVdpId].u4Interlace = rVdpSrcInfo.u4Interlace;

    if (_pfOverscanFct != NULL)
    {
        UINT16 u2Up;
        UINT16 u2Down;
        UINT16 u2Left;
        UINT16 u2Right;
        DRV_CUST_INPUT_SRC_TYPE_T eSrcType;
        DRV_CUSTOM_TIMING_INFO_T rTimingInfo;

        eSrcType = _VdpGetDrvCustInputSrcType(_arMwVdp[ucVdpId].e_type);

        rTimingInfo.ui2_width = (UINT16)rVdpSrcInfo.u4SrcWidth;
        rTimingInfo.ui2_height = (UINT16)rVdpSrcInfo.u4SrcHeight;
        rTimingInfo.ui4_fram_rate = rVdpSrcInfo.u4FrameRate;

        if (rVdpSrcInfo.u4Interlace)
        {
            rTimingInfo.e_scan_mode = DRV_CUSTOM_TIMING_SCAN_MODE_INTERLACE;
        }
        else
        {
            rTimingInfo.e_scan_mode = DRV_CUSTOM_TIMING_SCAN_MODE_PROGRESSIVE;
        }

        _pfOverscanFct(
            ucVdpId,
            eSrcType,
            &rTimingInfo,
            &u2Up,
            &u2Down,
            &u2Left,
            &u2Right
        );

        if (_prVdpConf[ucVdpId]->rOverScan.u4Top != u2Up ||
            _prVdpConf[ucVdpId]->rOverScan.u4Bottom != u2Down ||
            _prVdpConf[ucVdpId]->rOverScan.u4Left != u2Left ||
            _prVdpConf[ucVdpId]->rOverScan.u4Right != u2Right)
        {
            _prVdpConf[ucVdpId]->rOverScan.u4Top = u2Up;
            _prVdpConf[ucVdpId]->rOverScan.u4Bottom = u2Down;
            _prVdpConf[ucVdpId]->rOverScan.u4Left = u2Left;
            _prVdpConf[ucVdpId]->rOverScan.u4Right = u2Right;

            fgRetTrue = TRUE;
        }
    }

    if (_pfGetCurrentDispFmtFct != NULL)
    {
        VID_PLA_DISP_FMT_T e_fmt;
        DRV_CUSTOM_VID_SRC_INFO_T rVidSrcInfo;

        rVidSrcInfo.e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_INPUT_SRC;
        rVidSrcInfo.u.t_input_src.e_input_source = _VdpGetDrvCustInputSrcType(_arMwVdp[ucVdpId].e_type);
        rVidSrcInfo.u.t_input_src.ui2_id = _arMwVdp[ucVdpId].ucMpvCompId;  // ucPortId=?
        rVidSrcInfo.u.t_input_src.e_timing_type = _arMwVdp[ucVdpId].e_timing;

        e_fmt = _arMwVdp[ucVdpId].eFmt;

        _pfGetCurrentDispFmtFct(
            &_arMwVdp[ucVdpId].rSrcRes,
            &_arMwVdp[ucVdpId].rOutRes,
            &rVidSrcInfo,
            &e_fmt);

        if (_arMwVdp[ucVdpId].eFmt != e_fmt)
        {
            LOG(1, "FMT CHANGE (%d) to (%d) VidSrcInfo (%d, %d, %d)\n",
                _arMwVdp[ucVdpId].eFmt,
                e_fmt,
                rVidSrcInfo.u.t_input_src.e_input_source,
                rVidSrcInfo.u.t_input_src.ui2_id,
                rVidSrcInfo.u.t_input_src.e_timing_type);

            _arMwVdp[ucVdpId].eFmt = e_fmt;
        }
    }

    if (_VdpXlatDispFmtCallBackMw(ucVdpId) != 0)
    {
        _prVdpConf[ucVdpId]->rSrcRegion.u4X = _arMwVdp[ucVdpId].rFinalSource.ui4_x;
        _prVdpConf[ucVdpId]->rSrcRegion.u4Y = _arMwVdp[ucVdpId].rFinalSource.ui4_y;
        _prVdpConf[ucVdpId]->rSrcRegion.u4Width = _arMwVdp[ucVdpId].rFinalSource.ui4_width;
        _prVdpConf[ucVdpId]->rSrcRegion.u4Height = _arMwVdp[ucVdpId].rFinalSource.ui4_height;

        _prVdpConf[ucVdpId]->rOutRegion.u4X = _arMwVdp[ucVdpId].rFinalOutput.ui4_x;
        _prVdpConf[ucVdpId]->rOutRegion.u4Y = _arMwVdp[ucVdpId].rFinalOutput.ui4_y;
        _prVdpConf[ucVdpId]->rOutRegion.u4Width = _arMwVdp[ucVdpId].rFinalOutput.ui4_width;
        _prVdpConf[ucVdpId]->rOutRegion.u4Height = _arMwVdp[ucVdpId].rFinalOutput.ui4_height;

        VERIFY(x_sema_unlock(_hMutex) == OSR_OK);

        if (_arMwVdp[ucVdpId].ucMpvCompId != (UCHAR)MPV_COMP_NS)
        {
            MW_MPV_NOTIFY(_arMwVdp[ucVdpId].ucMpvCompId);
        }

        fgRetTrue = TRUE;
    }
    else
    {
        VERIFY(x_sema_unlock(_hMutex) == OSR_OK);

        if (_arMwVdp[ucVdpId].ucMpvCompId != (UCHAR)MPV_COMP_NS)
        {
            MW_MPV_NOTIFY(_arMwVdp[ucVdpId].ucMpvCompId);
        }
        }

    if (fgRetTrue)
    {
        return 1;
    }
    else
    {
        return 0;
    }

#endif
}

//-----------------------------------------------------------------------------
/** Brief of _VdpAfdChangeNotify.
 */
//-----------------------------------------------------------------------------
static void _VdpAfdChangeNotify(UINT32 u4VdpId, UINT32 u4Afd, UINT32 u4AspectRatio)
{
    UNUSED(u4Afd);
    UNUSED(u4AspectRatio);

    VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (_VdpXlatDispFmtCallBackMw(u4VdpId) != 0)
    {
        VDP_REGION_T rOutRegion;

        rOutRegion.u4X = (UINT32) _arMwVdp[u4VdpId].rFinalOutput.ui4_x;
        rOutRegion.u4Y = (UINT32) _arMwVdp[u4VdpId].rFinalOutput.ui4_y;
        rOutRegion.u4Width = (UINT32) _arMwVdp[u4VdpId].rFinalOutput.ui4_width;
        rOutRegion.u4Height = (UINT32) _arMwVdp[u4VdpId].rFinalOutput.ui4_height;

        if (VDP_SetOutRegion(u4VdpId, 0, rOutRegion) != VDP_SET_OK)
        {
        }

        rOutRegion.u4X = (UINT32) _arMwVdp[u4VdpId].rFinalSource.ui4_x;
        rOutRegion.u4Y = (UINT32) _arMwVdp[u4VdpId].rFinalSource.ui4_y;
        rOutRegion.u4Width = (UINT32) _arMwVdp[u4VdpId].rFinalSource.ui4_width;
        rOutRegion.u4Height = (UINT32) _arMwVdp[u4VdpId].rFinalSource.ui4_height;

        if (VDP_SetSrcRegion(u4VdpId, 0, rOutRegion) != VDP_SET_OK)
        {
        }
    }

    VERIFY(x_sema_unlock(_hMutex) == OSR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of _VdpXlatDispFmtCallBackMw.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpXlatDispFmtCallBackMw(UCHAR ucVdpId)
{
#ifndef CC_VDP_FORCE_NO_MW_CB
    if (_pfXlatDispFmtExFct[(UINT32)_arMwVdp[ucVdpId].eFmt] != NULL)
    {
        VID_PLA_VID_REGION_T rSrcReg;
        VID_PLA_VID_REGION_T rDispReg;
        DRV_CUSTOM_VID_SRC_INFO_T rVidSrcInfo;

        if (_arMwVdp[ucVdpId].ucDtv != 0)
        {
            rVidSrcInfo.e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_MPEG;
            rVidSrcInfo.u.t_mpeg.ui1_afd = (UINT8) _prVdpConf[ucVdpId]->u4Afd;

            switch (_prVdpConf[ucVdpId]->u4AspectRatio)
            {
                case 2:
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_4_3;
                break;

                case 3:
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_16_9;
                break;

                case 4:
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_2_21_1;
                break;

                default:
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_UNKNOWN;
                break;
            }
        }
        else
        {
            rVidSrcInfo = _arMwVidSrcInfo[ucVdpId];
        }

        _pfXlatDispFmtExFct[(UINT32)_arMwVdp[ucVdpId].eFmt](
            &_arMwVdp[ucVdpId].rSrcRes,
            &_arMwVdp[ucVdpId].rOutRes,
            &rVidSrcInfo,
            &rSrcReg,
            &rDispReg);

        if ((rSrcReg.ui4_x != _arMwVdp[ucVdpId].rXlatSource.ui4_x) ||
            (rSrcReg.ui4_y != _arMwVdp[ucVdpId].rXlatSource.ui4_y) ||
            (rSrcReg.ui4_width != _arMwVdp[ucVdpId].rXlatSource.ui4_width) ||
            (rSrcReg.ui4_height != _arMwVdp[ucVdpId].rXlatSource.ui4_height) ||
            (rDispReg.ui4_x != _arMwVdp[ucVdpId].rXlatOutput.ui4_x) ||
            (rDispReg.ui4_y != _arMwVdp[ucVdpId].rXlatOutput.ui4_y) ||
            (rDispReg.ui4_width != _arMwVdp[ucVdpId].rXlatOutput.ui4_width) ||
            (rDispReg.ui4_height != _arMwVdp[ucVdpId].rXlatOutput.ui4_height))
        {
            // New Source/Output Region

            _arMwVdp[ucVdpId].rXlatSource = rSrcReg;
            _arMwVdp[ucVdpId].rXlatOutput = rDispReg;

            _VdpCalculateOutputRegion(ucVdpId);

            _VdpCalculateSourceRegion(ucVdpId);

            return 1;
        }
    }
    else
    if (_pfXlatDispFmtFct[(UINT32)_arMwVdp[ucVdpId].eFmt] != NULL)
    {
        VID_PLA_VID_REGION_T rSrcReg;
        VID_PLA_VID_REGION_T rDispReg;

        _pfXlatDispFmtFct[(UINT32)_arMwVdp[ucVdpId].eFmt](
            &_arMwVdp[ucVdpId].rSrcRes,
            &_arMwVdp[ucVdpId].rOutRes,
            (UINT8) _prVdpConf[ucVdpId]->u4Afd,
            &rSrcReg,
            &rDispReg);

        if ((rSrcReg.ui4_x != _arMwVdp[ucVdpId].rXlatSource.ui4_x) ||
            (rSrcReg.ui4_y != _arMwVdp[ucVdpId].rXlatSource.ui4_y) ||
            (rSrcReg.ui4_width != _arMwVdp[ucVdpId].rXlatSource.ui4_width) ||
            (rSrcReg.ui4_height != _arMwVdp[ucVdpId].rXlatSource.ui4_height) ||
            (rDispReg.ui4_x != _arMwVdp[ucVdpId].rXlatOutput.ui4_x) ||
            (rDispReg.ui4_y != _arMwVdp[ucVdpId].rXlatOutput.ui4_y) ||
            (rDispReg.ui4_width != _arMwVdp[ucVdpId].rXlatOutput.ui4_width) ||
            (rDispReg.ui4_height != _arMwVdp[ucVdpId].rXlatOutput.ui4_height))
        {
            // New Source/Output Region

            _arMwVdp[ucVdpId].rXlatSource = rSrcReg;
            _arMwVdp[ucVdpId].rXlatOutput = rDispReg;

            _VdpCalculateOutputRegion(ucVdpId);

            _VdpCalculateSourceRegion(ucVdpId);

            return 1;
        }
    }
#endif

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of _VdpCalculateOutputRegion.
 */
//-----------------------------------------------------------------------------
static void _VdpCalculateOutputRegion(UCHAR ucVdpId)
{
    // X
    _arMwVdp[ucVdpId].rFinalOutput.ui4_x = _arMwVdp[ucVdpId].rFullOutput.ui4_x + ((_arMwVdp[ucVdpId].rXlatOutput.ui4_x * _arMwVdp[ucVdpId].rFullOutput.ui4_width) / 1000);

    // Y
    _arMwVdp[ucVdpId].rFinalOutput.ui4_y = _arMwVdp[ucVdpId].rFullOutput.ui4_y + ((_arMwVdp[ucVdpId].rXlatOutput.ui4_y * _arMwVdp[ucVdpId].rFullOutput.ui4_height) / 1000);

    // Width
    _arMwVdp[ucVdpId].rFinalOutput.ui4_width = (_arMwVdp[ucVdpId].rXlatOutput.ui4_width * _arMwVdp[ucVdpId].rFullOutput.ui4_width) / 1000;

    // Height
    _arMwVdp[ucVdpId].rFinalOutput.ui4_height = (_arMwVdp[ucVdpId].rXlatOutput.ui4_height * _arMwVdp[ucVdpId].rFullOutput.ui4_height) / 1000;
}

//-----------------------------------------------------------------------------
/** Brief of _VdpCalculateSourceRegion.
 */
//-----------------------------------------------------------------------------
static void _VdpCalculateSourceRegion(UCHAR ucVdpId)
{
    // X
    _arMwVdp[ucVdpId].rFinalSource.ui4_x = _arMwVdp[ucVdpId].rFullSource.ui4_x + ((_arMwVdp[ucVdpId].rXlatSource.ui4_x * _arMwVdp[ucVdpId].rFullSource.ui4_width) / 1000);

    // Y
    _arMwVdp[ucVdpId].rFinalSource.ui4_y = _arMwVdp[ucVdpId].rFullSource.ui4_y + ((_arMwVdp[ucVdpId].rXlatSource.ui4_y * _arMwVdp[ucVdpId].rFullSource.ui4_height) / 1000);

    // Width
    _arMwVdp[ucVdpId].rFinalSource.ui4_width = (_arMwVdp[ucVdpId].rXlatSource.ui4_width * _arMwVdp[ucVdpId].rFullSource.ui4_width) / 1000;

    // Height
    _arMwVdp[ucVdpId].rFinalSource.ui4_height = (_arMwVdp[ucVdpId].rXlatSource.ui4_height * _arMwVdp[ucVdpId].rFullSource.ui4_height) / 1000;
}

//-----------------------------------------------------------------------------
/** Brief of _VdpGetSrcTypeIdx.
 */
//-----------------------------------------------------------------------------
static UINT8 _VdpGetSrcTypeIdx( DRV_TYPE_T  e_drv_type )
{
    switch( e_drv_type )
    {
    case DRVT_AVC_COMP_VIDEO:
        return (UINT8)EVDP_VIS_AVC_COMP_VIDEO;

    case DRVT_AVC_S_VIDEO:
        return (UINT8)EVDP_VIS_AVC_S_VIDEO;

    case DRVT_AVC_Y_PB_PR:
        return (UINT8)EVDP_VIS_AVC_Y_PB_PR;

    case DRVT_AVC_VGA:
        return (UINT8)EVDP_VIS_AVC_VGA;

    case DRVT_AVC_SCART:
        return (UINT8)EVDP_VIS_AVC_SCART;

    case DRVT_AVC_DVI:
        return (UINT8)EVDP_VIS_AVC_DVI;

    case DRVT_AVC_HDMI:
        return (UINT8)EVDP_VIS_AVC_HDMI;

    case DRVT_TUNER_SAT_DIG:
    case DRVT_TUNER_CAB_DIG:
    case DRVT_TUNER_TER_DIG:
        return (UINT8)EVDP_VIS_TUNER_DIGITAL;

    case DRVT_TUNER_SAT_ANA:
    case DRVT_TUNER_CAB_ANA:
    case DRVT_TUNER_TER_ANA:
        return (UINT8)EVDP_VIS_TUNER_ANALOG;

    default:
        return (UINT8)EVDP_VIS_MAX;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VdpParseMinMaxDftTbl.
 */
//-----------------------------------------------------------------------------
static BOOL _VdpParseMinMaxDftTbl(
    UINT8                    ui1_src_idx,
    VID_QA_MIN_MAX_DFT_T*    pt_min_max_dft_tbl )
{
LINT_EXT_HEADER_BEGIN
    UINT8 u1UiVqItem;
    EVDP_UI_VIDEO_QUALITY_ITEM_T eUiVqItem;
    VDP_UI_VQ_MIN_MAX_DFT_T rUiVqMinMaxDft;
    VID_QA_MIN_MAX_DFT_T *pt_iter = pt_min_max_dft_tbl;

    if ( !pt_min_max_dft_tbl )
    {
        return FALSE;
    }

    while( !VID_MIN_MAX_DFT_IS_END(pt_iter) )
    {
        if ( pt_iter->e_vid_qa_type >= VID_QA_TYPE_MAX ||
             pt_iter->e_vid_qa_type <= VID_QA_TYPE_UNKNOWN )
        {
            return FALSE;
        }

        // set UI min, max and dft values to driver
        eUiVqItem = _VdpMwVidQaType2UiVqItem(pt_iter->e_vid_qa_type);
        if (eUiVqItem >= EVDP_UVQI_MAX)
        {
            return FALSE;
        }

        rUiVqMinMaxDft.i4Min = pt_iter->i4_min;
        rUiVqMinMaxDft.i4Max = pt_iter->i4_max;
        rUiVqMinMaxDft.i4Dft = pt_iter->i4_default;
        u1UiVqItem = ((UINT8)eUiVqItem);

        VDP_SetUiVqMinMaxDft(ui1_src_idx, u1UiVqItem, &rUiVqMinMaxDft);

        // next
        pt_iter++;
    }

    return TRUE;
LINT_EXT_HEADER_END
}

//-----------------------------------------------------------------------------
/** Brief of _VdpParseVidQaTbl.
 */
//-----------------------------------------------------------------------------
static BOOL _VdpParseVidQaTbl( VID_QA_TBL_T*    pt_vid_qa_tbl )
{
LINT_EXT_HEADER_BEGIN
    BOOL             b_success = FALSE;
    VID_QA_TBL_T*    pt_iter = pt_vid_qa_tbl;

    if ( !pt_vid_qa_tbl )
    {
        return FALSE;
    }

    while( !VID_QA_TBL_IS_END(pt_iter) )
    {
        UINT8  ui1_idx = _VdpGetSrcTypeIdx( pt_iter->e_src_type );

        if ( ui1_idx >= (UINT8)EVDP_VIS_MAX )
        {
            return FALSE;
        }

        b_success = _VdpParseMinMaxDftTbl(
            ui1_idx,
            pt_vid_qa_tbl->pt_min_max_dft_lst );
        if ( !b_success )
        {
            return FALSE;
        }

        pt_iter++;
    }

    return TRUE;
LINT_EXT_HEADER_END
}


//-----------------------------------------------------------------------------
/** Brief of _VdpDrvCustParse.
 */
//-----------------------------------------------------------------------------
static DRVP_TYPE_T _VdpDrvCustParse(
    const DRV_CUSTOM_T*     pt_drv_cust,
    VOID*                   pv_tag )
{
LINT_EXT_HEADER_BEGIN
    UNUSED(pv_tag);

    if (pt_drv_cust == NULL)
    {
        return DRVP_ERROR;
    }

    if (pt_drv_cust->e_custom_type == DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT_EX)
    {
        DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_EX_INFO_T* prFmtInfo;

        if (pt_drv_cust->ui2_id != MAX_COMP_ID)
        {
            return DRVP_ERROR;
        }

        if (pt_drv_cust->pv_arg == NULL)
        {
            return DRVP_ERROR;
        }

        prFmtInfo = (DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_EX_INFO_T*) (pt_drv_cust->pv_arg);

        _pfXlatDispFmtExFct[(UINT32) prFmtInfo->e_disp_fmt] = prFmtInfo->pf_xlat_disp_fmt;
    }
    else
    if (pt_drv_cust->e_custom_type == DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT)
    {
        DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_INFO_T* prFmtInfo;

        if (pt_drv_cust->ui2_id != MAX_COMP_ID)
        {
            return DRVP_ERROR;
        }

        if (pt_drv_cust->pv_arg == NULL)
        {
            return DRVP_ERROR;
        }

        prFmtInfo = (DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_INFO_T*) (pt_drv_cust->pv_arg);

        _pfXlatDispFmtFct[(UINT32) prFmtInfo->e_disp_fmt] = prFmtInfo->pf_xlat_disp_fmt;
    }
    else if (pt_drv_cust->e_custom_type == DRV_CUSTOM_TYPE_VID_PLA_VID_QA_MIN_MAX_DFT)
    {
        BOOL    b_success;

        b_success = _VdpParseVidQaTbl( (VID_QA_TBL_T*)(pt_drv_cust->pv_arg) );
        if ( !b_success )
        {
            return DRVP_ERROR;
        }
    }
    else if (pt_drv_cust->e_custom_type == DRV_CUSTOM_TYPE_VID_PLA_GET_CURRENT_DISP_FMT)
    {
        _pfGetCurrentDispFmtFct = (d_vid_pla_get_current_disp_fmt_fct) pt_drv_cust->pv_arg;
    }
    else if (pt_drv_cust->e_custom_type == DRV_CUSTOM_TYPE_VID_PLA_OVERSCAN_CB)
    {
        _pfOverscanFct = (d_vid_pla_overscan_fct) pt_drv_cust->pv_arg;
    }
    else if (pt_drv_cust->e_custom_type == DRV_CUSTOM_TYPE_VID_PLA_UPDATE_OVERSCAN_TBL)
    {
        _pfUpdateOverscanTbl = (d_vid_pla_update_overscan_tbl_fct) pt_drv_cust->pv_arg;
    }
    else
    {
        return DRVP_ERROR;
    }

    return DRVP_NEXT;
LINT_EXT_HEADER_END
}


//-----------------------------------------------------------------------------
/** Brief of _VdpMwVidQaType2UiVqItem.
 */
//-----------------------------------------------------------------------------
static EVDP_UI_VIDEO_QUALITY_ITEM_T _VdpMwVidQaType2UiVqItem(VID_QA_TYPE_T eVidQaType)
{
    switch (eVidQaType)
    {
        case VID_QA_TYPE_BRIGHTNESS:    // brightness
            return EVDP_UVQI_BRIGHTNESS;

        case VID_QA_TYPE_CONTRAST:      // contrast
            return EVDP_UVQI_CONTRAST;

        case VID_QA_TYPE_HUE:           // hue
            return EVDP_UVQI_HUE;

        case VID_QA_TYPE_SATURATION:    // saturation
            return EVDP_UVQI_SATURATION;

        case VID_QA_TYPE_CTI:           // cti
            return EVDP_UVQI_CTI;

        case VID_QA_TYPE_ETI:           // eti
            return EVDP_UVQI_ETI;

        case VID_QA_TYPE_SHARPNESS:     // sharpness (v, h)
            return EVDP_UVQI_SHARPNESS;

        case VID_QA_TYPE_COLOR_GAIN:    // color gain
            return EVDP_UVQI_COLOR_GAIN;

        case VID_QA_TYPE_COLOR_SUPPRESS:
            return EVDP_UVQI_COLOR_SUPPRESS;

        case VID_QA_TYPE_COLOR_OFFSET:    // color offset
            return EVDP_UVQI_COLOR_OFFSET;

        case VID_QA_TYPE_NR:            // noise reduction
            return EVDP_UVQI_NR;

        case VID_QA_TYPE_BLACK_LVL_EXT: // black level extension
            return EVDP_UVQI_BLACK_LVL_EXT;

        case VID_QA_TYPE_WHITE_PEAK_LMT:// white peak limit
            return EVDP_UVQI_WHITE_PEAK_LMT;

        case VID_QA_TYPE_FLESH_TONE:    // second color enhance
            return EVDP_UVQI_FLESH_TONE;

        case VID_QA_TYPE_LUMA:          // adaptive luma
            return EVDP_UVQI_LUMA;

        case VID_QA_TYPE_3D_NR:          // 3D NR
            return EVDP_UVQI_3D_NR;

        default:
            return EVDP_UVQI_MAX;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VdpGetDrvCustInputSrcType.
 */
//-----------------------------------------------------------------------------
static DRV_CUST_INPUT_SRC_TYPE_T _VdpGetDrvCustInputSrcType(DRV_TYPE_T e_type)
{
    switch(e_type)
    {
        case DRVT_AVC_COMP_VIDEO:
        case DRVT_AVC_Y_PB_PR:
            return DRV_CUST_INPUT_SRC_TYPE_YPBPR;

        case DRVT_AVC_S_VIDEO:
            return DRV_CUST_INPUT_SRC_TYPE_CVBS;

        case DRVT_AVC_VGA:
            return DRV_CUST_INPUT_SRC_TYPE_VGA;

        case DRVT_AVC_SCART:
            return DRV_CUST_INPUT_SRC_TYPE_SCART;

        case DRVT_AVC_DVI:
            return DRV_CUST_INPUT_SRC_TYPE_DVI;

        case DRVT_AVC_HDMI:
            return DRV_CUST_INPUT_SRC_TYPE_HDMI;

        case DRVT_VID_DEC:
        case DRVT_TUNER_SAT_DIG:
        case DRVT_TUNER_CAB_DIG:
        case DRVT_TUNER_TER_DIG:
            return DRV_CUST_INPUT_SRC_TYPE_DTV;

        case DRVT_TUNER_SAT_ANA:
        case DRVT_TUNER_CAB_ANA:
        case DRVT_TUNER_TER_ANA:
            return DRV_CUST_INPUT_SRC_TYPE_ATV;

        default:
            return DRV_CUST_INPUT_SRC_TYPE_UNKNOWN;
    }
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
void _MW_VDP_HDMI_AFD_CB(UCHAR ucVdpId, UINT16 u2Data)
{
    if (ucVdpId < VDP_COMP_NS)
    {
        VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        if ((_arMwVidSrcInfo[ucVdpId].e_type != DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_AFD) ||
            (_arMwVidSrcInfo[ucVdpId].u.ui1_hdmi_afd != (UINT8)u2Data))
        {
            _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_AFD;
            _arMwVidSrcInfo[ucVdpId].u.ui1_hdmi_afd = (UINT8)u2Data;

            if (_VdpXlatDispFmtCallBackMw(ucVdpId) != 0)
            {
                VDP_REGION_T rSrcRegion;
                VDP_REGION_T rOutRegion;

                rSrcRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_x;
                rSrcRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_y;
                rSrcRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_width;
                rSrcRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_height;

                if (VDP_SetSrcRegion(ucVdpId, 0, rSrcRegion) != VDP_SET_OK)
                {
                }

                rOutRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_x;
                rOutRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_y;
                rOutRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_width;
                rOutRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_height;

                if (VDP_SetOutRegion(ucVdpId, 0, rOutRegion) != VDP_SET_OK)
                {
                }
            }
        }

        VERIFY(x_sema_unlock(_hMutex) == OSR_OK);
    }
    else
    {
        ASSERT(0);
    }
}
void _MW_VDP_HDMI_CB(UCHAR ucVdpId, UINT16 u2Data)
{
    if (ucVdpId < VDP_COMP_NS)
    {
        VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        if ((_arMwVidSrcInfo[ucVdpId].e_type != DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_ASP_RATIO) ||
            (_arMwVidSrcInfo[ucVdpId].u.e_hdmi_asp != (DRV_CUSTOM_HDMI_ASP_RATIO_T)u2Data))
        {
            _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_ASP_RATIO;
            _arMwVidSrcInfo[ucVdpId].u.e_hdmi_asp = (DRV_CUSTOM_HDMI_ASP_RATIO_T)u2Data;

            if (_VdpXlatDispFmtCallBackMw(ucVdpId) != 0)
            {
                VDP_REGION_T rSrcRegion;
                VDP_REGION_T rOutRegion;

                rSrcRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_x;
                rSrcRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_y;
                rSrcRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_width;
                rSrcRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_height;

                if (VDP_SetSrcRegion(ucVdpId, 0, rSrcRegion) != VDP_SET_OK)
                {
                }

                rOutRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_x;
                rOutRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_y;
                rOutRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_width;
                rOutRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_height;

                if (VDP_SetOutRegion(ucVdpId, 0, rOutRegion) != VDP_SET_OK)
                {
                }
            }
        }

        VERIFY(x_sema_unlock(_hMutex) == OSR_OK);
    }
    else
    {
        ASSERT(0);
    }
}

#ifndef CC_CQAM
void _MW_VDP_WSS_CB(UCHAR ucVdpId, UINT16 u2Data)
{
    if (ucVdpId < VDP_COMP_NS)
    {
      VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        if ((_arMwVidSrcInfo[ucVdpId].e_type != DRV_CUSTOM_VID_SRC_INFO_TYPE_PAL_SECAM_WSS_SCART) ||
            (_arMwVidSrcInfo[ucVdpId].u.t_wss_scart.ui2_pal_secam_wss != u2Data))
        {
            _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_PAL_SECAM_WSS_SCART;
            _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.ui2_pal_secam_wss = u2Data;
            _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.e_scart_pin_8 = DRV_CUSTOM_SCART_PIN_8_UNKNOWN;

            if (_VdpXlatDispFmtCallBackMw(ucVdpId) != 0)
            {
                VDP_REGION_T rSrcRegion;
                VDP_REGION_T rOutRegion;

                rSrcRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_x;
                rSrcRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_y;
                rSrcRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_width;
                rSrcRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_height;

                if (VDP_SetSrcRegion(ucVdpId, 0, rSrcRegion) != VDP_SET_OK)
                {
                }

                rOutRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_x;
                rOutRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_y;
                rOutRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_width;
                rOutRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_height;

                if (VDP_SetOutRegion(ucVdpId, 0, rOutRegion) != VDP_SET_OK)
                {
                }
            }
        }

      VERIFY(x_sema_unlock(_hMutex) == OSR_OK);
    }
    else
    {
        ASSERT(0);
    }
}

void _MW_VDP_SCART_PIN_8_CB(UCHAR ucVdpId, DRV_CUSTOM_SCART_PIN_8_T t_scart_pin_8)
{
    if (ucVdpId < VDP_COMP_NS)
    {
        VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_PAL_SECAM_WSS_SCART;
        _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.e_scart_pin_8 = t_scart_pin_8;
        _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.ui2_pal_secam_wss = 0;

        if (_VdpXlatDispFmtCallBackMw(ucVdpId) != 0)
        {
            VDP_REGION_T rSrcRegion;
            VDP_REGION_T rOutRegion;

            rSrcRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_x;
            rSrcRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_y;
            rSrcRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_width;
            rSrcRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_height;

            if (VDP_SetSrcRegion(ucVdpId, 0, rSrcRegion) != VDP_SET_OK)
            {
            }

            rOutRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_x;
            rOutRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_y;
            rOutRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_width;
            rOutRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_height;

            if (VDP_SetOutRegion(ucVdpId, 0, rOutRegion) != VDP_SET_OK)
            {
            }
        }

        VERIFY(x_sema_unlock(_hMutex) == OSR_OK);
    }
    else
    {
        ASSERT(0);
    }
}

#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of MW_VDP_Init.
 */
//-----------------------------------------------------------------------------
INT32 MW_VDP_Init( DRV_CUSTOM_DIR_T* pt_drv_cust_dir )
{
LINT_SUPPRESS_BRACE(818)

    UCHAR ucVdpId;
    UINT32 u4Idx;
    INT32 i4_return;
    DRV_COMP_REG_T tVdpCompReg;
    DRV_COMP_FCT_TBL_T tVdpFctTbl;

    /* Check enum value */
    ASSERT((UINT32)VID_PLA_NORMAL == VDP_MODE_NORMAL);
    ASSERT((UINT32)VID_PLA_BLANK == VDP_MODE_BG);
    ASSERT((UINT32)VID_PLA_QUAD_VIDEO == VDP_MODE_QV);
    ASSERT((UINT32)VID_PLA_DEINT == VDP_MODE_DEINT);
    ASSERT((UINT32) sizeof(VID_PLA_DISP_FMT_T) <= CC_DISP_FMT_ENUM_SIZE);

    /* Init Local Variable */
    for (ucVdpId = 0; ucVdpId < VDP_COMP_NS; ucVdpId++)
    {
        _arMwVdp[ucVdpId] = _rMwVdpDefault;

        _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_UNKNOWN;
        _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.ui2_pal_secam_wss = 0;
        _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.e_scart_pin_8 = DRV_CUSTOM_SCART_PIN_8_UNKNOWN;
    }

    for (ucVdpId = 0; ucVdpId < (UCHAR)MPV_COMP_NS; ucVdpId++)
    {
        _arMwComp2EsId[ucVdpId] = 0xFF;
    }

    /* Init */
    VDP_Init();

    // get semaphore instead of create semaphore, Upper Layer, DTV, & NPTV share one mutex to prevent deadlock
    _hMutex = VDP_GetMutex();
//    VERIFY(x_sema_create(&_hMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

    /* Source Change Notification */
    VDP_RegCbFunc(VDP_CB_FUNC_SRC_CHG_IND, (UINT32)_VdpSourceChangeNotify, 0, 0);
//    VDP_SetSourceChangeCb(_VdpSourceChangeNotify);

    /* Afd Change Notification */
    VDP_RegCbFunc(VDP_CB_FUNC_AFD_IND, (UINT32)_VdpAfdChangeNotify, 0, 0);
//    VDP_SetAfdChangeCb(_VdpAfdChangeNotify);

    /* Get  */
    for (ucVdpId = 0; ucVdpId < VDP_NS; ucVdpId++)
    {
        _prVdpConf[ucVdpId] = VDP_GetConf(ucVdpId);
    }

    /* Setup component ID */
    tVdpCompReg.e_type = DRVT_VID_PLANE;
    tVdpCompReg.e_id_type = ID_TYPE_RANGE;
    tVdpCompReg.u.t_range.ui2_first_id = VDP_COMP_1;
    tVdpCompReg.u.t_range.ui2_delta_id = 1;
    tVdpCompReg.u.t_range.ui2_num_of_ids = VDP_COMP_NS;
    tVdpCompReg.u.t_range.pv_first_tag = NULL;
    tVdpCompReg.u.t_range.pv_delta_tag = NULL;
    tVdpCompReg.u.t_range.ui1_port = VDP_MAX_INPORT_NS;

    /* Setup component function table */
    tVdpFctTbl.pf_rm_connect = _VdpConnect;
    tVdpFctTbl.pf_rm_disconnect = _VdpDisconnect;
    tVdpFctTbl.pf_rm_get = _VdpGet;
    tVdpFctTbl.pf_rm_set = _VdpSet;

    /* Register component with the Resource Manager database */
    i4_return = x_rm_reg_comp(&tVdpCompReg,
                                VDP_MAX_INPORT_NS,
                                1,
                                DRVN_VID_PLANE,            //"VDP",
                                DRV_FLAG_SINGLE_CONN_ON_OUTPUT,
                                &tVdpFctTbl,
                                NULL,
                                0);
    if (i4_return != RMR_OK)
    {
        return i4_return;
    }

    /* Clean custom table */
    for (u4Idx = 0; u4Idx < (1 << (8 * CC_DISP_FMT_ENUM_SIZE)); u4Idx++)
    {
        _pfXlatDispFmtFct[u4Idx] = NULL;

        _pfXlatDispFmtExFct[u4Idx] = NULL;
    }
    _pfGetCurrentDispFmtFct = NULL;
    _pfOverscanFct = NULL;
    _pfUpdateOverscanTbl = NULL;

    /* Parse custom table */
    if (pt_drv_cust_dir != NULL)
    {
        if (d_drv_cust_parse(
            pt_drv_cust_dir->pt_custom,
            _VdpDrvCustParse,
            NULL ) != 0)
        {
            return (RMR_DRV_ERROR);
        }
    }

    // Set all UI video quality items' current to default value.
    VDP_SetAllUiVqItemDftValues();

    return (RMR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of MW_VDP_SetConnect.
 */
//-----------------------------------------------------------------------------
void MW_VDP_SetConnect(UCHAR ucVdpId, UCHAR ucConnect)
{
    _arMwVdp[ucVdpId].ucConnect = ucConnect;

    VERIFY(_VdpSetEnable(ucVdpId) == VDP_SET_OK);
}

//-----------------------------------------------------------------------------
/** Brief of MW_VDP_Comp2EsInd.
 */
//-----------------------------------------------------------------------------
void MW_VDP_Comp2EsInd(UCHAR ucCompId, UCHAR ucEsId)
{
    if (ucCompId >= (UCHAR)MPV_COMP_NS)
    {
        ASSERT(ucCompId >= (UCHAR)MPV_COMP_NS);
        return;
    }

    if (_arMwComp2EsId[ucCompId] != ucEsId)
    {
        UINT32 u4VdpId;

        _arMwComp2EsId[ucCompId] = ucEsId;

        for (u4VdpId = 0; u4VdpId < VDP_COMP_NS; u4VdpId++)
        {
            if (_arMwVdp[u4VdpId].ucMpvCompId == ucCompId)
            {
                _arMwVdp[u4VdpId].ucEsId = ucEsId;
                    VERIFY(_SetInput((UCHAR)u4VdpId) == VDP_SET_OK);
            }
        }
    }
}



