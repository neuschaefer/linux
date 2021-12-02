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
 * $RCSfile: pmx_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pmx_mw.c
 *  Brief of file pmx_mw.c.
 *  Details of file pmx_mw.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "pmx_drvif.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "vdp_if.h"
#include "panel.h"
#include "srm_drvif.h"
#include "drvcust_if.h"
#include "drv_comp_id.h"
#include "drv_name.h"
#include "d_drv_cust_util.h"
#include "x_plane_mxr.h"
#include "x_pinmux.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "vdp_drvif.h"
#include "nptv_if.h"
#include "sv_const.h"
#define DEFINE_IS_LOG PMX_IsLog
#include "x_debug.h"
#include "api_backlt.h"
// #include "api_ycproc.h"
#ifdef MMP_ARGB_MODE
#include "drv_ycproc.h"
#endif
#include "pe_if.h"


LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define DISP_QA_MIN_MAX_DFT_IS_END( pt_min_max_dft )                           \
    ( (pt_min_max_dft->e_disp_qa_type == DISP_QA_TYPE_UNKNOWN) &&              \
      (pt_min_max_dft->i4_min == 0) &&                                         \
      (pt_min_max_dft->i4_max == 0) &&                                         \
      (pt_min_max_dft->i4_default == 0) )

#define DISP_QA_TBL_IS_END( pt_qa_tbl )                                        \
    ( (pt_qa_tbl->e_src_type == DRVT_UNKNOWN) &&                               \
      (pt_qa_tbl->pt_min_max_dft_lst == NULL) )

#define PLA_MXR_Z_ORDER_OSD_VIDEO_CAP                                       \
    (PLA_MXR_Z_ORDER_FLAG_0 +PLA_MXR_Z_ORDER_FLAG_1+                        \
    PLA_MXR_Z_ORDER_FLAG_2+PLA_MXR_Z_ORDER_FLAG_3+PLA_MXR_Z_ORDER_FLAG_4)


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

extern INT32 MW_PMX_Init(DRV_CUSTOM_DIR_T* pt_drv_cust_dir );
extern void MW_OsdConnect(INT32 hOsdPlane, INT32 fgEnable);
extern void MW_PMX_PORT_DISCONNECT(UCHAR ucPmxCompId, UCHAR ucLocalId);

EXTERN UCHAR MW_VDP_MEMO_MODE_GET(UCHAR ucVdpId);
EXTERN void MW_VDP_MEMO_MODE_SET_PMX_INFO(UCHAR ucVdpId, UCHAR ucPmxCompId, UCHAR ucPmxPortId);


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static UCHAR _PmxCompId2LocalId(const DRV_COMP_ID_T* prCompReg);

static void _PmxLocalId2CompId(DRV_COMP_ID_T* prCompReg, UCHAR ucLocalId);

static void _PmxSetPlaneOrder(UCHAR ucPmxId);

static PE_ARG_TYPE _PmxMwGetType2ArgType(UINT32 u4GetType);

static PE_ARG_TYPE _PmxMwSetType2ArgType(UINT32 u4SetType);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

// dynamic port
static UCHAR _aucPort[PMX_COMP_NS][PMX_MAX_INPORT_NS];

// static port: VDP1/VDP2/OSD1/OSD2/OSD3
static UINT32 _au4Zorder[PMX_COMP_NS][PMX_MAX_INPORT_NS];

// static port: VDP1/VDP2/OSD1/OSD2/OSD3
static UINT32 _au4ZorderCap[PMX_COMP_NS][PMX_MAX_INPORT_NS];

// static port: VDP1/VDP2/OSD1/OSD2/OSD3
static UINT32 _au4LastZorder[PMX_COMP_NS][PMX_MAX_INPORT_NS];

// Background Color
static PLA_MXR_BG_COLOR_T _arBg[PMX_COMP_NS] =
{
    {0, 0, 0}
};

// Output Port Configuration  (TODO)
static UINT32 _arOutputPort[PMX_COMP_NS];
static UCHAR _ucHwAdpBacklightEnable = FALSE;
static BOOL _fgPatternEnable = FALSE;
static BOOL _fgBacklightOnOff = FALSE;
static BOOL _fgPanelOnOff = FALSE;

#ifdef LINUX_TURNKEY_SOLUTION
void
pmx_mw_get_zorder(UCHAR ucPmxId, UINT32 u4Orders[], size_t *ptCount)
{
    UCHAR ucPort;
    *ptCount = 4;
    for (ucPort = 0; ucPort < PMX_MAX_INPORT_NS; ucPort++)
    {
        if (_aucPort[ucPmxId][ucPort] >= PMX_MAX_INPORT_NS)
        {
            u4Orders[ucPort] = PLA_MXR_Z_ORDER_UNKNOWN;
        }
        else
        {
            u4Orders[ucPort] = _au4Zorder[ucPmxId][_aucPort[ucPmxId][ucPort]];
        }
    }
}

void
pmx_mw_set_zorder(UCHAR ucPmxId, UINT32 u4Orders[])
{
    UINT32 u4Port;

    for (u4Port = 0; u4Port < PMX_MAX_INPORT_NS; u4Port++)
    {
        if (_aucPort[ucPmxId][u4Port] < PMX_MAX_INPORT_NS)        // connected port
        {
            _au4Zorder[ucPmxId][_aucPort[ucPmxId][u4Port]] = u4Orders[u4Port];
        }
    }

    _PmxSetPlaneOrder(ucPmxId);
}

void _sync_fb()
{
    UINT32 u4HwOrderPlanes[PMX_MAX_INPORT_NS];
    UINT32 u4Port;

    _mt53fb_get_plane_order_HW_array(u4HwOrderPlanes);

    for (u4Port = 0; u4Port < PMX_MAX_INPORT_NS; u4Port++)
    {
        if(u4HwOrderPlanes[u4Port] < PMX_MAX_INPORT_NS)
        {
            _au4Zorder[0][u4HwOrderPlanes[u4Port]] = u4Port;
        }
    }
}
#endif /* LINUX_TURNKEY_SOLUTION */
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of _PmxConnect.
 */
//-----------------------------------------------------------------------------
static INT32 _PmxConnect(DRV_COMP_ID_T*   pt_comp_id,
                        DRV_CONN_TYPE_T  e_conn_type,
                        const VOID*      pv_conn_info,
                        SIZE_T           z_conn_info_len,
                        VOID*            pv_tag,
                        x_rm_nfy_fct     pf_nfy)
{
    DRV_COMP_ID_T* prCompReg;
    UCHAR ucPortId;
    UCHAR ucPmxId;
    UCHAR ucLocalId;
    UINT32 u4ZorderIdx;
    UINT32 u4Idx;

    UNUSED(pf_nfy);
    UNUSED(pv_tag);
    UNUSED(z_conn_info_len);

    // Check component type and id
    if (pt_comp_id == NULL)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->e_type != DRVT_PLA_MXR)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

#ifdef LINUX_TURNKEY_SOLUTION
    _sync_fb();
#endif

    ucPmxId = (UCHAR) pt_comp_id->ui2_id;

    // subset of the following checking, useless?
    if (ucPmxId > (PMX_COMP_NS + PMX_COMP_1))
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (ucPmxId >= PMX_COMP_NS)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    // handle input port ID
    if (pt_comp_id->b_sel_out_port)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    ucPortId = (UCHAR) pt_comp_id->u.ui1_inp_port;

    if (ucPortId >= PMX_MAX_INPORT_NS)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (e_conn_type != RM_CONN_TYPE_COMP_ID)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    // handle Component ID
    prCompReg = (DRV_COMP_ID_T*)pv_conn_info;
    ucLocalId = _PmxCompId2LocalId(prCompReg);

    if ((ucLocalId>=PMX_MAX_INPORT_NS))
    {
        return (RMR_DRV_INV_CONN_INFO);                
    }
 
    // protection, if plane is connected, return fail
    for (u4Idx = 0; u4Idx < PMX_MAX_INPORT_NS; u4Idx++)
    {
        if ((u4Idx != ucPortId) &&
            (_aucPort[ucPmxId][u4Idx] == ucLocalId))
        {
           return (RMR_DRV_INV_CONN_INFO);
        }
    }

    if (prCompReg == NULL)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (prCompReg->e_type == DRVT_VID_PLANE)
    {
        UCHAR ucVdpId;

        ucVdpId = (UCHAR) prCompReg->ui2_id;
        if (ucVdpId > (VDP_COMP_NS + VDP_COMP_1))
        {
           return (RMR_DRV_INV_CONN_INFO);
        }

        // handle connect
        MW_VDP_SetConnect(ucVdpId, 1);

        _aucPort[ucPmxId][ucPortId] = ucLocalId; //_PmxCompId2LocalId(prCompReg);
    }
    else
    if (prCompReg->e_type == DRVT_OSD_PLANE)
    {
        UCHAR ucOsdId;

        ucOsdId = (UCHAR) prCompReg->ui2_id;
        if (ucOsdId > (OSD_COMP_NS + (UCHAR)OSD_COMP_1))
        {
           return (RMR_DRV_INV_CONN_INFO);
        }

        // handle connect
        MW_OsdConnect(ucOsdId, 1);

        _aucPort[ucPmxId][ucPortId] = ucLocalId; //_PmxCompId2LocalId(prCompReg);
    }
    else
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    // try last Z-order first
    if(_au4LastZorder[ucPmxId][ucLocalId] != PLA_MXR_Z_ORDER_UNKNOWN)
    {
        UINT32 u4Occupy;
        UINT32 u4PortIdx;

        u4ZorderIdx = _au4LastZorder[ucPmxId][ucLocalId];

        // check if this z-order is occupied
        u4Occupy = 0;
        for (u4PortIdx = 0; u4PortIdx < PMX_MAX_INPORT_NS; u4PortIdx++)
        {
            if (_au4Zorder[ucPmxId][u4PortIdx] == u4ZorderIdx)
            {
                u4Occupy = 1;
                u4PortIdx = PMX_MAX_INPORT_NS;
            }
        }

        if (u4Occupy == 0)
        {
            // NOT occupy, we can use this z-order
            _au4Zorder[ucPmxId][ucLocalId] = u4ZorderIdx;
        }
    }

    if(_au4Zorder[ucPmxId][ucLocalId] == PLA_MXR_Z_ORDER_UNKNOWN)
    {
        // principle, set Z-order = the 1st Z-order after connect
        for (u4ZorderIdx = PLA_MXR_Z_ORDER_0; u4ZorderIdx <=PLA_MXR_Z_ORDER_31; u4ZorderIdx++)
        {
            UINT32 u4Cap;
            u4Cap = MAKE_BIT_MASK_32(u4ZorderIdx);

            if (_au4ZorderCap[ucPmxId][ucLocalId] & u4Cap)  // capable
            {
                UINT32 u4Occupy;
                UINT32 u4PortIdx;

                // check if this z-order is occupied
                u4Occupy = 0;
                for (u4PortIdx = 0; u4PortIdx < PMX_MAX_INPORT_NS; u4PortIdx++)
                {
                    if (_au4Zorder[ucPmxId][u4PortIdx] == u4ZorderIdx)
                    {
                        u4Occupy = 1;
                        u4PortIdx = PMX_MAX_INPORT_NS;
                    }
                }

                if (u4Occupy == 0)
                {
                    // NOT occupy, we can use this z-order
                    _au4Zorder[ucPmxId][ucLocalId] = u4ZorderIdx;
                    u4ZorderIdx = PLA_MXR_Z_ORDER_31;
                }
            }
        }
    }

    ASSERT(_au4Zorder[ucPmxId][ucLocalId] != PLA_MXR_Z_ORDER_UNKNOWN);
    LOG(5, "_PmxConnect: port%d=VDP%d, z-order=%d\n",
        ucPortId, ucLocalId, _au4Zorder[ucPmxId][ucLocalId]);
    _PmxSetPlaneOrder(ucPmxId);

    UNUSED(pt_comp_id);
    return (RMR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of _PmxDisconnect.
 */
//-----------------------------------------------------------------------------
static INT32 _PmxDisconnect(DRV_COMP_ID_T*   pt_comp_id,
                        DRV_DISC_TYPE_T  e_disc_type,
                        const VOID*      pv_disc_info,
                        SIZE_T           z_disc_info_len)
{
    UCHAR ucPortId;
    UCHAR ucPmxId;
    DRV_COMP_ID_T rCompId;

    UNUSED(e_disc_type);
    UNUSED(pv_disc_info);
    UNUSED(z_disc_info_len);

    // Check component type and id
    if (pt_comp_id == NULL)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (pt_comp_id->e_type != DRVT_PLA_MXR)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

#ifdef LINUX_TURNKEY_SOLUTION
    _sync_fb();
#endif

    ucPmxId = (UCHAR) pt_comp_id->ui2_id;

    // subset of the following checking, useless?
    if (ucPmxId > (PMX_COMP_NS + PMX_COMP_1))
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (ucPmxId >= PMX_COMP_NS)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    // handle input port ID
    if (pt_comp_id->b_sel_out_port)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    ucPortId = (UCHAR)pt_comp_id->u.ui1_inp_port;

    if (ucPortId >= PMX_MAX_INPORT_NS)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    _PmxLocalId2CompId(&rCompId, _aucPort[ucPmxId][ucPortId]);

    if (rCompId.e_type == DRVT_VID_PLANE)
    {
        UCHAR ucVdpId;

        ucVdpId = (UCHAR)rCompId.ui2_id;

        if (MW_VDP_MEMO_MODE_GET(ucVdpId))
        {
            MW_VDP_MEMO_MODE_SET_PMX_INFO(ucVdpId, ucPmxId, _aucPort[ucPmxId][ucPortId]);
            return (RMR_OK);
        }
        
        // handle disconnect
        MW_VDP_SetConnect(ucVdpId, 0);
    }
    else
    if (rCompId.e_type == DRVT_OSD_PLANE)
    {
        UCHAR ucOsdId;

        ucOsdId = (UCHAR)rCompId.ui2_id;

        // handle disconnect
        MW_OsdConnect(ucOsdId, 0);
    }
    else
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    LOG(5, "_PmxDisconnect: port%d=VDP%d, z-order=%d\n",
        ucPortId,
        _aucPort[ucPmxId][ucPortId],
        _au4Zorder[ucPmxId][_aucPort[ucPmxId][ucPortId]]);

    // backup Z-order
    _au4LastZorder[ucPmxId][_aucPort[ucPmxId][ucPortId]] = _au4Zorder[ucPmxId][_aucPort[ucPmxId][ucPortId]];

    // principle, set Z-order = PLA_MXR_Z_ORDER_UNKNOWN when disconnect
    _au4Zorder[ucPmxId][_aucPort[ucPmxId][ucPortId]] = PLA_MXR_Z_ORDER_UNKNOWN;
#ifndef LINUX_TURNKEY_SOLUTION
//    _PmxSetPlaneOrder(ucPmxId);        // NO need, port will be disable
#else
    _PmxSetPlaneOrder(ucPmxId);        // NO: Needed to sync with FB, port will be disable
#endif
    _aucPort[ucPmxId][ucPortId] = PMX_MAX_INPORT_NS;

    UNUSED(pt_comp_id);
    return (RMR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of _PmxGet.
 */
//-----------------------------------------------------------------------------
static INT32 _PmxGet(DRV_COMP_ID_T*  pt_comp_id,
                    DRV_GET_TYPE_T  e_get_type,
                    VOID*           pv_get_info,
                    SIZE_T*         pz_get_info_len)
{
    UCHAR ucPmxId;

    // Check component type and id
    if (pt_comp_id == NULL)
    {
       return (RMR_DRV_INV_GET_INFO);
    }

    if (pt_comp_id->e_type != DRVT_PLA_MXR)
    {
       return (RMR_DRV_INV_GET_INFO);
    }

    ucPmxId = (UCHAR) pt_comp_id->ui2_id;

    if (ucPmxId > (PMX_COMP_NS + PMX_COMP_1))
    {
       return (RMR_DRV_INV_GET_INFO);
    }

    if (ucPmxId >= (PMX_COMP_NS))
    {
       return (RMR_DRV_INV_GET_INFO);
    }

    if (pv_get_info == NULL)
    {
       return (RMR_DRV_INV_GET_INFO);
    }

    if (pz_get_info_len == NULL)
    {
       return (RMR_DRV_INV_GET_INFO);
    }

#ifdef LINUX_TURNKEY_SOLUTION
    _sync_fb();
#endif

    // Perform get opertion
    switch (e_get_type)
    {
        case PLA_MXR_GET_TYPE_CTRL:
            {
                PLA_MXR_CTRL_T* peArg;
                peArg = (PLA_MXR_CTRL_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(PLA_MXR_CTRL_T))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *pz_get_info_len = sizeof(PLA_MXR_CTRL_T);

                if (_fgPanelOnOff)
                {
                    *peArg = PLA_MXR_CTRL_ENABLE;
                }
                else
                {
                    *peArg = PLA_MXR_CTRL_DISABLE;
                }
            }
            break;

        case PLA_MXR_GET_TYPE_CAPABILITY:
            {
                PLA_MXR_CAPABILITY_INFO_T* peArg;
                peArg = (PLA_MXR_CAPABILITY_INFO_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(PLA_MXR_CAPABILITY_INFO_T))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *pz_get_info_len = sizeof(PLA_MXR_CAPABILITY_INFO_T);

                *peArg = (PLA_MXR_CAP_GAMMA |
                          PLA_MXR_CAP_ADJ_PLA |
                          PLA_MXR_CAP_PLA_ORDER |
                          PLA_MXR_CAP_RESOLUTION |
                          PLA_MXR_CAP_BG |
                          PLA_MXR_CAP_BACK_LIGHT |
                          PLA_MXR_CAP_ADP_BACK_LIGHT |
                          PLA_MXR_CAP_BACK_LIGHT_CTRL );
             }
            break;

        case PLA_MXR_GET_TYPE_ADJ_PLA_BY_PORT:
            {
                PLA_MXR_ADJ_PLA_BY_PORT_INFO_T* prArg;
                prArg = (PLA_MXR_ADJ_PLA_BY_PORT_INFO_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(PLA_MXR_ADJ_PLA_BY_PORT_INFO_T))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *pz_get_info_len = sizeof(PLA_MXR_ADJ_PLA_BY_PORT_INFO_T);

                // invalid port id
                if (prArg->ui1_port >= PMX_MAX_INPORT_NS)
                {
                    return (RMR_DRV_INV_GET_INFO);
                }

                // corrosponding port NOT exist
                if (_aucPort[ucPmxId][prArg->ui1_port] == PMX_MAX_INPORT_NS)
                {
                    prArg->ui4_adj_zorder = PLA_MXR_Z_ORDER_FLAG_UNKNOWN;
                }
                else
                {
                    prArg->ui4_adj_zorder = _au4ZorderCap[ucPmxId][_aucPort[ucPmxId][prArg->ui1_port]];
                }
            }
            break;

        case PLA_MXR_GET_TYPE_ADJ_PLA_BY_ID_TYPE:
            {
                UCHAR ucLocalId;
                DRV_COMP_ID_T rCompId;
                PLA_MXR_ADJ_PLA_BY_ID_TYPE_INFO_T* prArg;
                prArg = (PLA_MXR_ADJ_PLA_BY_ID_TYPE_INFO_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(PLA_MXR_ADJ_PLA_BY_ID_TYPE_INFO_T))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *pz_get_info_len = sizeof(PLA_MXR_ADJ_PLA_BY_ID_TYPE_INFO_T);

                rCompId.e_type = prArg->e_plane_type;
                rCompId.ui2_id = prArg->ui2_comp_id;
                ucLocalId = _PmxCompId2LocalId(&rCompId);
                if ((ucPmxId >= PMX_COMP_NS)||(ucLocalId>=PMX_MAX_INPORT_NS))
                {
                    return (RMR_DRV_INV_GET_INFO);                
                }
                prArg->ui4_adj_zorder = _au4ZorderCap[ucPmxId][ucLocalId];
             }
            break;

        case PLA_MXR_GET_TYPE_PLA_ORDER:
            {
                UCHAR ucPort;
                UINT32* pu4PlaneOrder;
                pu4PlaneOrder = (UINT32*) pv_get_info;

                if (*pz_get_info_len < (sizeof(UINT32) * PMX_MAX_INPORT_NS))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *pz_get_info_len = sizeof(UINT32) * PMX_MAX_INPORT_NS;

                for (ucPort = 0; ucPort < PMX_MAX_INPORT_NS; ucPort++)
                {
                    if (_aucPort[ucPmxId][ucPort] >= PMX_MAX_INPORT_NS)
                    {
                        pu4PlaneOrder[ucPort] = PLA_MXR_Z_ORDER_UNKNOWN;
                    }
                    else
                    {
                        pu4PlaneOrder[ucPort] = _au4Zorder[ucPmxId][_aucPort[ucPmxId][ucPort]];
                    }
                }
            }
            break;

        case PLA_MXR_GET_TYPE_BG:
            {
                PLA_MXR_BG_COLOR_T* prArg;
                prArg = (PLA_MXR_BG_COLOR_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(PLA_MXR_BG_COLOR_T))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *pz_get_info_len = sizeof(PLA_MXR_BG_COLOR_T);

                *prArg = _arBg[ucPmxId];
            }
            break;

        // back light control
        case PLA_MXR_GET_TYPE_BACK_LIGHT:
            {
                PE_ARG_TYPE ucArgType;
                INT16 i2Val;
                UCHAR* pucArg;
                pucArg = (UCHAR*) pv_get_info;

                if (*pz_get_info_len < sizeof(UCHAR))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }

                *pz_get_info_len = sizeof(UCHAR);
                ucArgType = _PmxMwGetType2ArgType(e_get_type);

                if (PE_GetArg(VDP_1, ucArgType, &i2Val) != VDP_SET_OK)
                {
                    return (RMR_DRV_INV_GET_INFO);
                }

                *pucArg = (UCHAR)i2Val;
            }
            break;
        case PLA_MXR_GET_TYPE_ADP_BACK_LIGHT:
            {
                PE_ARG_TYPE ucArgType;
                INT16 i2Val;
                UCHAR* pucArg;
                UINT32 u4Gpio;
                pucArg = (UCHAR*) pv_get_info;

                if (*pz_get_info_len < sizeof(UCHAR))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }

                *pz_get_info_len = sizeof(UCHAR);

                if (DRVCUST_OptQuery(ePanelDCRGpio, &u4Gpio) == -1)
                {
                    ucArgType = _PmxMwGetType2ArgType(e_get_type);

                    if (PE_GetArg(VDP_1, ucArgType, &i2Val) != VDP_SET_OK)
                    {
                        return (RMR_DRV_INV_GET_INFO);
                    }

                    *pucArg = (UCHAR)i2Val;
                }
                else
                {
                    *pucArg = _ucHwAdpBacklightEnable;
                }
            }
            break;

        case PLA_MXR_GET_TYPE_MIN_MAX:
            {
            	PE_UI_RANGE_T PEUiRange;
                PE_ARG_TYPE ucArgType;
                PLA_MXR_MIN_MAX_INFO_T* prArg;

                prArg = (PLA_MXR_MIN_MAX_INFO_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(PLA_MXR_MIN_MAX_INFO_T))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *pz_get_info_len = sizeof(PLA_MXR_MIN_MAX_INFO_T);

                ucArgType = _PmxMwGetType2ArgType(prArg->e_get_type);

                if ((ucArgType == PE_ARG_BACK_LIGHT_LVL) ||
                    (ucArgType == PE_ARG_ADAPTIVE_BACK_LIGHT))
                {
                    BOOL bRet = PE_GetUiRange(VDP_1, ucArgType, &PEUiRange);

                    if (!bRet)
                    {
                        return (RMR_DRV_INV_GET_INFO);
                    }

                    prArg->ui4_min_value = (UINT16)PEUiRange.i4Min;
                    prArg->ui4_max_value = (UINT16)PEUiRange.i4Max;
                }
                else
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
            }
            break;

        case PLA_MXR_GET_TYPE_RESOLUTION:
            {
                PLA_MXR_RESOLUTION_INFO_T* prArg;
                prArg = (PLA_MXR_RESOLUTION_INFO_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(PLA_MXR_RESOLUTION_INFO_T))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *pz_get_info_len = sizeof(PLA_MXR_RESOLUTION_INFO_T);

                prArg->b_is_progressive = 1;
                prArg->ui2_width = PANEL_GetPanelWidth();
                prArg->ui2_height = PANEL_GetPanelHeight();
                prArg->ui2_curr_v_clk = PANEL_GetPixelClk60Hz();
                prArg->ui2_v_clk_max = PANEL_GetVClkMax();
                prArg->ui2_v_clk_min = PANEL_GetVClkMin();
                prArg->ui2_panel_width = PANEL_GetPanelPhysicalWidth();
                prArg->ui2_panel_height = PANEL_GetPanelPhysicalHeight();
            }
            break;

        case PLA_MXR_GET_TYPE_OUTPUT_PORT:
            {
                UINT32* pu4Arg;
                pu4Arg = (UINT32*) pv_get_info;

                if (*pz_get_info_len < sizeof(UINT32))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *pz_get_info_len = sizeof(UINT32);

                *pu4Arg = _arOutputPort[ucPmxId];
            }
            break;

        case PLA_MXR_GET_TYPE_TV_MODE:
            {
                SRM_TV_MODE_T rTvMode = SRM_GetTvMode();
                PLA_MXR_TV_MODE_T *prArg = (PLA_MXR_TV_MODE_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(PLA_MXR_TV_MODE_T))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *pz_get_info_len = sizeof(PLA_MXR_TV_MODE_T);

                if (rTvMode == SRM_TV_MODE_TYPE_NORMAL)
                {
                    *prArg = PLA_MXR_TV_MODE_NORMAL;
                }
                else if (rTvMode == SRM_TV_MODE_TYPE_PIP)
                {
                    *prArg = PLA_MXR_TV_MODE_PIP;
                }
                else if (rTvMode == SRM_TV_MODE_TYPE_POP)
                {
                    *prArg = PLA_MXR_TV_MODE_POP;
                }
                else
                {
                    ASSERT(0);
                }
            }
            break;

        case PLA_MXR_GET_TYPE_MJC:
            {
                if (*pz_get_info_len < sizeof(BOOL))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *(BOOL*)pv_get_info = TRUE;
            }
            break;

        case PLA_MXR_GET_TYPE_TEST_PATTERN:
            {
                if (*pz_get_info_len < sizeof(BOOL))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *(BOOL*)pv_get_info = _fgPatternEnable;
            }
            break;

        case PLA_MXR_GET_TYPE_BACK_LIGHT_CTRL:
            {
                if (*pz_get_info_len < sizeof(BOOL)||!pv_get_info)
                {
                    return (RMR_DRV_INV_GET_INFO);
                }

                *((BOOL*)(UINT32)pv_get_info) = _fgBacklightOnOff;
            }
            break;

        case PLA_MXR_GET_TYPE_MAX_GAMMA_CURVE:
            {
                UCHAR* pucArg;
                pucArg = (UCHAR*) pv_get_info;

                if (*pz_get_info_len < sizeof(UCHAR))
                {
                    return (RMR_DRV_INV_GET_INFO);
                }
                *pz_get_info_len = sizeof(UCHAR);
		        *pucArg = 3;
            }
            break;
        case PLA_MXR_GET_TYPE_DITHER:
        case PLA_MXR_GET_TYPE_DITHER_CAP:
        case PLA_MXR_GET_TYPE_ASPECT_RATIO:
        case PLA_MXR_GET_TYPE_DIGITAL_OUTPUT_FMT:
        default:
            return (RMR_DRV_INV_GET_INFO);
    }

    UNUSED(pt_comp_id);

    return (RMR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of _PmxSet.
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSet(DRV_COMP_ID_T*  pt_comp_id,
                    DRV_SET_TYPE_T  e_set_type,
                    const VOID*     pv_set_info,
                    SIZE_T          z_set_info_len)
{
    UCHAR ucPmxId;

    // Check component type and id
    if (pt_comp_id == NULL)
    {
       return (RMR_DRV_INV_SET_INFO);
    }

    if (pt_comp_id->e_type != DRVT_PLA_MXR)
    {
       return (RMR_DRV_INV_SET_INFO);
    }

    ucPmxId = (UCHAR) pt_comp_id->ui2_id;

    if (ucPmxId > (PMX_COMP_NS + PMX_COMP_1))
    {
       return (RMR_DRV_INV_SET_INFO);
    }

    if (ucPmxId >= (PMX_COMP_NS))
    {
       return (RMR_DRV_INV_GET_INFO);
    }

    // Check set_info
    if (z_set_info_len == 0)
    {
        return (RMR_DRV_INV_SET_INFO);
    }

#ifdef LINUX_TURNKEY_SOLUTION
    _sync_fb();
#endif

    // Perform set opertion
    switch (e_set_type)
    {
        case PLA_MXR_SET_TYPE_CTRL:
            {
                PLA_MXR_CTRL_T eArg;
                eArg = (PLA_MXR_CTRL_T) (UINT32) pv_set_info;

                if (eArg == PLA_MXR_CTRL_ENABLE)
                {
                    vApiPanelPowerSequence(TRUE);
                    _fgPanelOnOff = TRUE;
                    //return (RMR_DRV_INV_SET_INFO);
                }
                else if (eArg == PLA_MXR_CTRL_DISABLE)
                {
                    vApiPanelPowerSequence(FALSE);
                    _fgPanelOnOff = FALSE;
                    //return (RMR_DRV_INV_SET_INFO);
                }
                else
                {
                }
            }
            break;

        case PLA_MXR_SET_TYPE_PLA_ORDER:
            {
                UINT32 u4Port;
                UINT32* pu4PlaneOrder;

                if (pv_set_info == NULL)
                {
                    return (RMR_DRV_INV_SET_INFO);
                }

                pu4PlaneOrder = (UINT32*) pv_set_info;

                LOG(5, "_PmxSet: [port,zo]=([0,%d] [1,%d] [2,%d] [3,%d] [4,%d])\n"
                    , pu4PlaneOrder[0]
                    , pu4PlaneOrder[1]
                    , pu4PlaneOrder[2]
                    , pu4PlaneOrder[3]
                    , pu4PlaneOrder[4]);

                // protection
                for (u4Port = 0; u4Port < PMX_MAX_INPORT_NS; u4Port++)
                {
                    if (_aucPort[ucPmxId][u4Port] < PMX_MAX_INPORT_NS)        // connected port
                    {
                        UINT32 u4ZoderCap;

                        if (pu4PlaneOrder[u4Port] > PLA_MXR_Z_ORDER_31)
                        {
                            return (RMR_DRV_INV_SET_INFO);
                        }

                        u4ZoderCap = _au4ZorderCap[ucPmxId][_aucPort[ucPmxId][u4Port]];

                        if ((u4ZoderCap & MAKE_BIT_MASK_32(pu4PlaneOrder[u4Port])) == 0)
                        {
                            // NOT capable
                            return (RMR_DRV_INV_SET_INFO);
                        }
                    }
                    else                                                    // disconnected port
                    {
                        if (pu4PlaneOrder[u4Port] != PLA_MXR_Z_ORDER_UNKNOWN)
                        {
                            return (RMR_DRV_INV_SET_INFO);
                        }
                    }
                }

                // set new z-order
                for (u4Port = 0; u4Port < PMX_MAX_INPORT_NS; u4Port++)
                {
                    if (_aucPort[ucPmxId][u4Port] < PMX_MAX_INPORT_NS)        // connected port
                    {
                        _au4Zorder[ucPmxId][_aucPort[ucPmxId][u4Port]] = pu4PlaneOrder[u4Port];
                    }
                }

                _PmxSetPlaneOrder(ucPmxId);
            }
            break;

        case PLA_MXR_SET_TYPE_BG:
            {
                UINT32 u4Bg;
                PLA_MXR_BG_COLOR_T* prArg;

                if (pv_set_info == NULL)
                {
                   return (RMR_DRV_INV_SET_INFO);
                }

                prArg = (PLA_MXR_BG_COLOR_T*) pv_set_info;
                _arBg[ucPmxId] = *prArg;

                u4Bg = (prArg->ui1_r << 16) | (prArg->ui1_g << 8) |
                       (prArg->ui1_b);

                if (PMX_SetBg(u4Bg, TRUE) != VDP_SET_OK)
                {
                   return (RMR_DRV_INV_SET_INFO);
                }
            }
            break;

        case PLA_MXR_SET_TYPE_GAMMA:
            {
// TODO: REMOVE    	
                UCHAR ucArg;

                ucArg = (UCHAR) (UINT32) pv_set_info;
                #ifdef MMP_ARGB_MODE
//                vDrvSetSWGammaFlg(SV_ON);
		#endif
                //UNUSED(bApiVideoProc(SV_VP_MAIN , (INT32)PE_ARG_GAMMA));
                //vApiVideoGammaLoadTable(ucArg);
                //vApiRegisterVideoEvent(PE_ARG_GAMMA, ucArg, SV_ON);
		        PE_SetArg(VDP_1, PE_ARG_GAMMA, ucArg);
	        
             }
            break;

        // back light control
        case PLA_MXR_SET_TYPE_BACK_LIGHT:
            {
// TODO: REMOVE          	
                UCHAR ucArg;
                PE_ARG_TYPE ucArgType;

                // when DCR is on, don't set backlight
                if (_ucHwAdpBacklightEnable)
                {
                    return (RMR_DRV_INV_SET_INFO);
                }

                ucArg = (UCHAR) (UINT32) pv_set_info;
                ucArgType = _PmxMwSetType2ArgType(e_set_type);
                if (PE_SetArg(VDP_1, ucArgType, (INT16)ucArg) != VDP_SET_OK)
                {
                    return (RMR_DRV_INV_SET_INFO);
                }
            }
            break;
        case PLA_MXR_SET_TYPE_ADP_BACK_LIGHT:
            {
// TODO: REMOVE           	
                UCHAR ucArg;
                UINT32 u4Gpio;

                ucArg = (UCHAR) (UINT32) pv_set_info;
                if (DRVCUST_OptQuery(ePanelDCRGpio, &u4Gpio) == -1)
                {
                    PE_ARG_TYPE ucArgType;
                    ucArgType = _PmxMwSetType2ArgType(e_set_type);
                    if (PE_SetArg(VDP_1, ucArgType, (INT16)ucArg) != VDP_SET_OK)
                    {
                        return (RMR_DRV_INV_SET_INFO);
                    }
                }
                else
                {
                    UINT32 u4Value = DRVCUST_OptGet(ePanelDCROnVal);
                    _ucHwAdpBacklightEnable = ucArg;
                    if (!ucArg)
                    {
                        u4Value = !u4Value;
                    }
                    IGNORE_RET(GPIO_Output((INT32)u4Gpio, (INT32*)&u4Value));
                }
            }
            break;

        case PLA_MXR_SET_TYPE_OUTPUT_PORT:
            {
                UINT32 u4Cap;
                UINT32 u4Arg;
                u4Arg = (UINT32) pv_set_info;

                u4Cap = (PLA_MXR_OUTPUT_FLAG_DIGITAL |
                    PLA_MXR_OUTPUT_FLAG_LVDS |
                    PLA_MXR_OUTPUT_FLAG_CRT);

                if (u4Arg & ~u4Cap)
                {
                    return (RMR_DRV_INV_SET_INFO);
                }

                _arOutputPort[ucPmxId] = u4Arg;
            }
            break;

        case PLA_MXR_SET_TYPE_TV_MODE:
            {
                PLA_MXR_TV_MODE_T rTvMode;

                rTvMode = (PLA_MXR_TV_MODE_T)(UINT32)pv_set_info;
                if (rTvMode == PLA_MXR_TV_MODE_NORMAL)
                {
                    SRM_SetTvMode(SRM_TV_MODE_TYPE_NORMAL);
                }
                else if (rTvMode == PLA_MXR_TV_MODE_PIP)
                {
                    SRM_SetTvMode(SRM_TV_MODE_TYPE_PIP);
                }
                else if (rTvMode == PLA_MXR_TV_MODE_POP)
                {
                    SRM_SetTvMode(SRM_TV_MODE_TYPE_POP);
                }
                else
                {
                    return (RMR_DRV_INV_SET_INFO);
                }
            }
            break;

        case PLA_MXR_SET_TYPE_MJC:
            {
            }
            break;

        case PLA_MXR_SET_TYPE_TEST_PATTERN:
            {
                if (pv_set_info == PLA_MXR_TEST_PATTERN_DISABLE)
                {
                    _fgPatternEnable = FALSE;
                    PMX_ShowPattern(_fgPatternEnable);
                }
                else
                {
                    _fgPatternEnable = TRUE;
                    PMX_ShowPattern(_fgPatternEnable);
                }
            }
            break;

        case PLA_MXR_SET_TYPE_BACK_LIGHT_CTRL:
            {
                if ((BOOL)(UINT32)pv_set_info == FALSE)
                {
                    vApiBackltONOFF(FALSE);
                }
                else
                {
                    vApiBackltONOFF(TRUE);
                }
                _fgBacklightOnOff = (BOOL)(UINT32)pv_set_info;
            }
            break;

        case PLA_MXR_SET_TYPE_BRIGHTNESS:
        case PLA_MXR_SET_TYPE_CONTRAST:
        case PLA_MXR_SET_TYPE_HUE:
        case PLA_MXR_SET_TYPE_SATURATION:
        case PLA_MXR_SET_TYPE_DITHER:
        case PLA_MXR_SET_TYPE_RESOLUTION:
        case PLA_MXR_SET_TYPE_ASPECT_RATIO:
        case PLA_MXR_SET_TYPE_DIGITAL_OUTPUT_FMT:
        default:
            return (RMR_DRV_INV_SET_INFO);
    }

    UNUSED(pt_comp_id);

    return (RMR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of _PmxCompId2LocalId.
 */
//-----------------------------------------------------------------------------
static UCHAR _PmxCompId2LocalId(const DRV_COMP_ID_T* prCompReg)
{
    if (prCompReg == NULL)
    {
        return PMX_MAX_INPORT_NS;
    }

    if (prCompReg->e_type == DRVT_VID_PLANE)
    {
        UCHAR ucVdpId;

        ucVdpId = (UCHAR) prCompReg->ui2_id;
        if (ucVdpId > (VDP_COMP_NS + VDP_COMP_1))
        {
           return (PMX_MAX_INPORT_NS);
        }

        return ucVdpId;
    }
    else
    if (prCompReg->e_type == DRVT_OSD_PLANE)
    {
        UCHAR ucOsdId;

        ucOsdId = (UCHAR) prCompReg->ui2_id;
        if (ucOsdId > OSD_COMP_NS)
        {
           return (PMX_MAX_INPORT_NS);
        }

        return (VDP_COMP_NS + ucOsdId);
    }

    return PMX_MAX_INPORT_NS;
}

//-----------------------------------------------------------------------------
/** Brief of _PmxLocalId2CompId.
 */
//-----------------------------------------------------------------------------
static void _PmxLocalId2CompId(DRV_COMP_ID_T* prCompReg, UCHAR ucLocalId)
{
    if (prCompReg == NULL)
    {
        return;
    }

    if (ucLocalId == PMX_MAX_INPORT_NS)
    {
        prCompReg->e_type = DRVT_UNKNOWN;
        prCompReg->ui2_id = 0;
    }
    else
    if (ucLocalId < VDP_COMP_NS)
    {
        prCompReg->e_type = DRVT_VID_PLANE;
        prCompReg->ui2_id = VDP_COMP_1 + ucLocalId;
    }
    else
    {
        prCompReg->e_type = DRVT_OSD_PLANE;
        prCompReg->ui2_id = ucLocalId - VDP_COMP_NS;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _PmxSetPlaneOrder.
 */
//-----------------------------------------------------------------------------
static void _PmxSetPlaneOrder(UCHAR ucPmxId)
{
    // do conversion, middleware 2 hardware

    // middleware
    // PMXMW Port(0, 2, 3, 4, 5) Zo(1, -1, 2, 3, 0)
    // VDP 0 Z-order X
    // VDP 1, Z-order X
    // VDP 2, Z-order X
    // ..

    // hardware
    // Z-order 0, VDP X
    // Z-order 1, VDP X
    // Z-order 2, VDP X
    // ..

    if (ucPmxId == PMX_COMP_1)
    {
        UINT32 u4PortIdx;
        UINT32 u4ZorderIdx;
        UINT32 au4HwZorder[PMX_MAX_INPORT_NS];

        LOG(5, "_PmxSetPlaneOrder: [port,vdp,zo]=([0,%d,%d] [1,%d,%d] [2,%d,%d] [3,%d,%d] [4,%d,%d])\n",
            _aucPort[ucPmxId][0], _au4Zorder[ucPmxId][_aucPort[ucPmxId][0]],
            _aucPort[ucPmxId][1], _au4Zorder[ucPmxId][_aucPort[ucPmxId][1]],
            _aucPort[ucPmxId][2], _au4Zorder[ucPmxId][_aucPort[ucPmxId][2]],
            _aucPort[ucPmxId][3], _au4Zorder[ucPmxId][_aucPort[ucPmxId][3]],
            _aucPort[ucPmxId][4], _au4Zorder[ucPmxId][_aucPort[ucPmxId][4]]);

         for (u4PortIdx = 0; u4PortIdx < PMX_MAX_INPORT_NS; u4PortIdx++)
         {
             au4HwZorder[u4PortIdx] = PMX_MAX_INPORT_NS;
         }


         for (u4ZorderIdx = 0; u4ZorderIdx < PMX_MAX_INPORT_NS; u4ZorderIdx++)
         {
             for (u4PortIdx = 0; u4PortIdx < PMX_MAX_INPORT_NS; u4PortIdx++)
             {
                 if (_au4Zorder[ucPmxId][u4PortIdx] == u4ZorderIdx)
                 {
                     au4HwZorder[u4ZorderIdx] = u4PortIdx;
                     u4PortIdx = PMX_MAX_INPORT_NS;
                 }
             }
         }

        LOG(5, "_PmxSetPlaneOrder: hw zo=(VDP%d, VDP%d, VDP%d, VDP%d, VDP%d)\n",
            au4HwZorder[0],
            au4HwZorder[1],
            au4HwZorder[2],
            au4HwZorder[3],
            au4HwZorder[4]);

    #ifndef LINUX_TURNKEY_SOLUTION
        VERIFY(PMX_SetPlaneOrderArray(au4HwZorder) == PMX_SET_OK);
    #else
        _mt53fb_set_plane_order_HW_array(au4HwZorder);
    #endif
    }
}


//-----------------------------------------------------------------------------
/** Brief of _PmxMwGetType2ArgType.
 */
//-----------------------------------------------------------------------------
static PE_ARG_TYPE _PmxMwGetType2ArgType(UINT32 u4GetType)
{
    switch (u4GetType)
    {
        // VDP Back Light Args
        case PLA_MXR_GET_TYPE_BACK_LIGHT:
            return PE_ARG_BACK_LIGHT_LVL;

        case PLA_MXR_GET_TYPE_ADP_BACK_LIGHT:
            return PE_ARG_ADAPTIVE_BACK_LIGHT;

        default:
            return (PE_ARG_TYPE)0;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _PmxMwSetType2ArgType.
 */
//-----------------------------------------------------------------------------
static PE_ARG_TYPE _PmxMwSetType2ArgType(UINT32 u4SetType)
{
    switch (u4SetType)
    {
        // VDP Back Light Args
        case PLA_MXR_SET_TYPE_BACK_LIGHT:
            return PE_ARG_BACK_LIGHT_LVL;

        case PLA_MXR_SET_TYPE_ADP_BACK_LIGHT:
            return PE_ARG_ADAPTIVE_BACK_LIGHT;

        default:
            return (PE_ARG_TYPE)0;
    }
}

static EVDP_VIDEO_INPUT_SOURCE_T _convert_mw_src_type_2_drv_src_type(
    DRV_TYPE_T                e_src_type )
{
    EVDP_VIDEO_INPUT_SOURCE_T    e_drv_src_type;

    switch( e_src_type )
    {
    case DRVT_AVC_COMP_VIDEO:
        e_drv_src_type = EVDP_VIS_AVC_COMP_VIDEO;
        break;

    case DRVT_AVC_S_VIDEO:
        e_drv_src_type = EVDP_VIS_AVC_S_VIDEO;
        break;

    case DRVT_AVC_Y_PB_PR:
        e_drv_src_type = EVDP_VIS_AVC_Y_PB_PR;
        break;

    case DRVT_AVC_VGA:
        e_drv_src_type = EVDP_VIS_AVC_VGA;
        break;

    case DRVT_AVC_SCART:
        e_drv_src_type = EVDP_VIS_AVC_SCART;
        break;

    case DRVT_AVC_DVI:
        e_drv_src_type = EVDP_VIS_AVC_DVI;
        break;

    case DRVT_AVC_HDMI:
        e_drv_src_type = EVDP_VIS_AVC_HDMI;
        break;

    case DRVT_TUNER_SAT_DIG:
    case DRVT_TUNER_CAB_DIG:
    case DRVT_TUNER_TER_DIG:
        e_drv_src_type = EVDP_VIS_TUNER_DIGITAL;
        break;

    case DRVT_TUNER_SAT_ANA:
    case DRVT_TUNER_CAB_ANA:
    case DRVT_TUNER_TER_ANA:
        e_drv_src_type = EVDP_VIS_TUNER_ANALOG;
        break;

    default:
        e_drv_src_type = EVDP_VIS_MAX;
    }

    return e_drv_src_type;
}

//-----------------------------------------------------------------------------
/** Brief of _PmxParseMinMaxDftTbl.
 */
//-----------------------------------------------------------------------------
static BOOL _PmxParseMinMaxDftTbl(
    DRV_TYPE_T                e_src_type,
    DISP_QA_MIN_MAX_DFT_T*    pt_min_max_dft_tbl )
{
    PE_ARG_TYPE eUiVqItem;
    PE_UI_RANGE_T      rUiVqMinMaxDft;
    DISP_QA_MIN_MAX_DFT_T        *pt_iter = pt_min_max_dft_tbl;
    EVDP_VIDEO_INPUT_SOURCE_T    e_drv_src_type;

    if ( !pt_min_max_dft_tbl )
    {
        return FALSE;
    }

    e_drv_src_type = _convert_mw_src_type_2_drv_src_type( e_src_type );
    if ( e_drv_src_type == EVDP_VIS_MAX )
    {
        return FALSE;
    }

    if (pt_iter == NULL)
    {
        return FALSE;
    }

    while( !DISP_QA_MIN_MAX_DFT_IS_END(pt_iter) )
    {
        if ( (pt_iter->e_disp_qa_type >= DISP_QA_TYPE_MAX) ||
             (pt_iter->e_disp_qa_type <= DISP_QA_TYPE_UNKNOWN) )
        {
            return FALSE;
        }

        // set UI min, max and dft values to driver
        switch( pt_iter->e_disp_qa_type )
        {
        case DISP_QA_TYPE_BACK_LIGHT:
            eUiVqItem = PE_ARG_BACK_LIGHT_LVL;
            break;

        case DISP_QA_TYPE_ADP_BACK_LIGHT:
            eUiVqItem = PE_ARG_ADAPTIVE_BACK_LIGHT;
            break;

        default:
            return FALSE;
        }

        rUiVqMinMaxDft.i4Min = pt_iter->i4_min;
        rUiVqMinMaxDft.i4Max = pt_iter->i4_max;
        rUiVqMinMaxDft.i4Dft = pt_iter->i4_default;

        PE_SetUiRange((UINT8)e_drv_src_type, eUiVqItem, &rUiVqMinMaxDft);

        // next
        pt_iter++;
    }

    return TRUE;
}

static BOOL _PmxParseQaTbl(
    DISP_QA_TBL_T*    pt_qa_tbl )
{
    BOOL              b_success;
    DISP_QA_TBL_T*    pt_iter = pt_qa_tbl;

    if ( !pt_qa_tbl )
    {
        return FALSE;
    }

    if (pt_iter == NULL)
    {
        return FALSE;
    }

    while( !DISP_QA_TBL_IS_END(pt_iter) )
    {
        b_success = _PmxParseMinMaxDftTbl(
            pt_iter->e_src_type,
            (DISP_QA_MIN_MAX_DFT_T*)(pt_iter->pt_min_max_dft_lst) );
        if ( !b_success )
        {
            return b_success;
        }

        pt_iter++;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** Brief of _PmxDrvCustParse.
 */
//-----------------------------------------------------------------------------
static DRVP_TYPE_T _PmxDrvCustParse(
    const DRV_CUSTOM_T*     pt_drv_cust,
    VOID*                   pv_tag )
{
    UNUSED(pv_tag);

    if (pt_drv_cust == NULL)
    {
        return DRVP_ERROR;
    }

    if (pt_drv_cust->e_custom_type == DRV_CUSTOM_TYPE_PLA_MXR_DISP_QA_MIN_MAX_DFT )
    {
        BOOL    b_success;

        b_success = _PmxParseQaTbl( (DISP_QA_TBL_T*)(pt_drv_cust->pv_arg) );
        if ( !b_success )
        {
            return DRVP_ERROR;
        }
    }
    else
    {
        return DRVP_ERROR;
    }

    return DRVP_NEXT;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of MW_PMX_Init.
 */
//-----------------------------------------------------------------------------
INT32 MW_PMX_Init(DRV_CUSTOM_DIR_T* pt_drv_cust_dir)
{
    UCHAR ucPmxId;
    UCHAR ucPort;
    INT32 i4_return;
    DRV_COMP_REG_T tPmxCompReg;
    DRV_COMP_FCT_TBL_T tPmxFctTbl;

    for (ucPmxId = 0; ucPmxId < PMX_COMP_NS; ucPmxId++)
    {
        for (ucPort = 0; ucPort < PMX_MAX_INPORT_NS; ucPort++)
        {
            _aucPort[ucPmxId][ucPort] = PMX_MAX_INPORT_NS;
            _au4Zorder[ucPmxId][ucPort] = PLA_MXR_Z_ORDER_UNKNOWN;
            _au4LastZorder[ucPmxId][ucPort] = PLA_MXR_Z_ORDER_UNKNOWN;
            _au4ZorderCap[ucPmxId][ucPort] = PLA_MXR_Z_ORDER_FLAG_UNKNOWN;
        }

        _arOutputPort[ucPmxId] = 0;
    }

    _au4ZorderCap[PMX_COMP_1][VDP_COMP_1] = PLA_MXR_Z_ORDER_OSD_VIDEO_CAP;
    _au4ZorderCap[PMX_COMP_1][VDP_COMP_2] = PLA_MXR_Z_ORDER_OSD_VIDEO_CAP;
    _au4ZorderCap[PMX_COMP_1][VDP_COMP_3] = PLA_MXR_Z_ORDER_OSD_VIDEO_CAP;
    _au4ZorderCap[PMX_COMP_1][VDP_COMP_4] = PLA_MXR_Z_ORDER_OSD_VIDEO_CAP;
    _au4ZorderCap[PMX_COMP_1][VDP_COMP_5] = PLA_MXR_Z_ORDER_OSD_VIDEO_CAP;

#ifdef LINUX_TURNKEY_SOLUTION
#if defined(CC_MT5363) ||defined(CC_MT5365) || defined(CC_MT5395)||defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5880) || defined(CC_MT5860)
    _aucPort[0][3] = 3;
    _au4Zorder[0][3] = 0;
    _au4LastZorder[0][3] = PLA_MXR_Z_ORDER_UNKNOWN;
    
    _aucPort[0][4] = 4;
    _au4Zorder[0][4] = 1;
    _au4LastZorder[0][4] = PLA_MXR_Z_ORDER_UNKNOWN;
	#else //CC_MT5387
		_aucPort[0][3] = 3;
    _au4Zorder[0][3] = 0;
    _au4LastZorder[0][3] = PLA_MXR_Z_ORDER_UNKNOWN;

    _aucPort[0][2] = 2;
    _au4Zorder[0][2] = 1;
    _au4LastZorder[0][2] = PLA_MXR_Z_ORDER_UNKNOWN;
	#endif
    _sync_fb();
#endif
    /* Init */
    PMX_Init();

    /* Setup component ID */
    tPmxCompReg.e_type = DRVT_PLA_MXR;
    tPmxCompReg.e_id_type = ID_TYPE_RANGE;
    tPmxCompReg.u.t_range.ui2_first_id = PMX_COMP_1;
    tPmxCompReg.u.t_range.ui2_delta_id = 1;
    tPmxCompReg.u.t_range.ui2_num_of_ids = PMX_COMP_NS;
    tPmxCompReg.u.t_range.pv_first_tag = NULL;
    tPmxCompReg.u.t_range.pv_delta_tag = NULL;
    tPmxCompReg.u.t_range.ui1_port = PMX_MAX_INPORT_NS;

    /* Setup component function table */
    tPmxFctTbl.pf_rm_connect = _PmxConnect;
    tPmxFctTbl.pf_rm_disconnect = _PmxDisconnect;
    tPmxFctTbl.pf_rm_get = _PmxGet;
    tPmxFctTbl.pf_rm_set = _PmxSet;

    /* Register component with the Resource Manager database */
    i4_return = x_rm_reg_comp(&tPmxCompReg,
                                PMX_MAX_INPORT_NS,
                                1,
                                DRVN_PLA_MXR,            //"PMX",
                                DRV_FLAG_SINGLE_CONN_ON_OUTPUT,
                                &tPmxFctTbl,
                                NULL,
                                0);
    if (i4_return != RMR_OK)
    {
        return i4_return;
    }

    /* Parse custom table */
    if (pt_drv_cust_dir != NULL)
    {
        if (d_drv_cust_parse(
            pt_drv_cust_dir->pt_custom,
            _PmxDrvCustParse,
            NULL ) != 0)
        {
            return (RMR_DRV_ERROR);
        }
    }

    UNUSED(pt_drv_cust_dir);
    return (RMR_OK);
}

void MW_PMX_PORT_DISCONNECT(UCHAR ucPmxCompId, UCHAR ucLocalId)
{
    UCHAR ucPort;
    
    if ((ucPmxCompId >= PMX_COMP_NS) || (ucLocalId >= PMX_MAX_INPORT_NS))
    {
        return;
    }
    
    LOG(5, "_PmxDisconnect: local_id=%d, z-order=%d\n",
        ucLocalId,
        _au4Zorder[ucPmxCompId][ucLocalId]);

    // backup Z-order
    _au4LastZorder[ucPmxCompId][ucLocalId] = _au4Zorder[ucPmxCompId][ucLocalId];

    // principle, set Z-order = PLA_MXR_Z_ORDER_UNKNOWN when disconnect
    _au4Zorder[ucPmxCompId][ucLocalId] = PLA_MXR_Z_ORDER_UNKNOWN;
//    _PmxSetPlaneOrder(ucPmxId);        // NO need, port will be disable

    for (ucPort = 0; ucPort < PMX_MAX_INPORT_NS; ucPort++)
    {
        if (_aucPort[ucPmxCompId][ucPort] == ucLocalId)
        {
            _aucPort[ucPmxCompId][ucPort] = PMX_MAX_INPORT_NS;
        }
    }
}

