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
 * $Author: muran.xiang $
 * $Date: 2012/08/07 $
 * $RCSfile: vout_mw.c,v $
 * $Revision: #2 $
 *
 * Description:
 *         Video Out middleware driver API
 *---------------------------------------------------------------------------*/
#ifdef CC_SUPPORT_TVE

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "general.h"
#include "video_def.h"
#include "vdo_misc.h"
#include "drv_vbi.h"
#include "drv_scart.h"
#include "drvcust_if.h"
#include "source_table.h"
#include "tve_if.h"

#include "x_rm_dev_types.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_video_out.h"

#include "x_debug.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_hamming_code.h"
#include "d_drv_cust_util.h"
#include "avc_combi_mw.h"
#include "x_tv_enc.h"
/* For Cache and DRAM consistence */
#include "x_hal_926.h"

/* For number of component */
#include "drv_comp_id.h"
/*-----------------------------------------------------------------------------
                    Macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* constants */
#define DRVN_VIDEO_OUT    "VOut"

#define VOUT_MSG_FLAG_DBG_INFO (1<<0)

/* macros */
#ifdef DEBUG

#define VDOOUT_DBG_MSG( m )  {if ( _u4VoutMsgFlags & VOUT_MSG_FLAG_DBG_INFO ) { x_dbg_stmt m; }}

#else

#define VDOOUT_DBG_MSG( m )

#endif

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Global Data declaration
 ----------------------------------------------------------------------------*/
static UINT32 _u4VoutMsgFlags = 0;
/*-----------------------------------------------------------------------------
                    constants declaration
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    function prototypes
 ----------------------------------------------------------------------------*/
static INT32 _VdoOutMwConnect(DRV_COMP_ID_T * pt_comp_id,
                           DRV_CONN_TYPE_T e_conn_type,
                           const VOID * pv_conn_info,
                           SIZE_T z_conn_info_len,
                           VOID * pv_tag, x_rm_nfy_fct pf_nfy);

static INT32 _VdoOutMwDisconnect(DRV_COMP_ID_T * pt_comp_id,
                              DRV_DISC_TYPE_T e_disc_type,
                              const VOID * pv_disc_info,
                              SIZE_T z_disc_info_len);

static INT32 _VdoOutMwGet(DRV_COMP_ID_T * pt_comp_id,
                       DRV_GET_TYPE_T e_get_type,
                       VOID * pv_get_info, SIZE_T * pz_get_info_len);

static INT32 _VdoOutMwSet(DRV_COMP_ID_T * pt_comp_id,
                       DRV_SET_TYPE_T e_set_type,
                       const VOID * pv_set_info, SIZE_T z_set_info_len);

#define VDO_OUT_CTRL_ENABLE_BIT_MASK (0x1)
#define VDO_OUT_MODE_NORMAL_BIT_MASK (0x1<<1)
static UINT8 _u1VdoOutFlag[VDO_OUT_COMP_NS];


UINT32 u4VoutMwGetDbgMsgFlags(void)
{
    return _u4VoutMsgFlags;
}

void vVoutMwSetDbgMsgFlags(UINT32 u4flags)
{
    _u4VoutMsgFlags = u4flags;  /* can be controlled by driver CLI read/write */
}

INT32 MW_VdoOutInit(void)
{
#ifndef CC_DRIVER_PROGRAM
    INT32 i4Ret;
    UINT32 u4Flags;
    DRV_COMP_REG_T tVdoOutCompReg;
    DRV_COMP_FCT_TBL_T tVdoOutFctTbl;
    UINT32 u4Idx;
    
    for(u4Idx=0;u4Idx<VDO_OUT_COMP_NS;u4Idx++)
    {
        CHAR s_name[11];

        /* Setup component ID */
        tVdoOutCompReg.e_type = DRVT_VIDEO_OUT;
        tVdoOutCompReg.e_id_type = ID_TYPE_IND;
        tVdoOutCompReg.u.t_ind.ui2_id = u4Idx;
        tVdoOutCompReg.u.t_ind.ui1_port = ALL_PORTS;
        tVdoOutCompReg.u.t_ind.pv_tag = NULL;
    
        /* Registration Flags */
        u4Flags = 0;
    
        /* Setup component function table */
        tVdoOutFctTbl.pf_rm_connect = _VdoOutMwConnect;
        tVdoOutFctTbl.pf_rm_disconnect = _VdoOutMwDisconnect;
        tVdoOutFctTbl.pf_rm_get = _VdoOutMwGet;
        tVdoOutFctTbl.pf_rm_set = _VdoOutMwSet;


        x_snprintf( s_name, sizeof(s_name), "%s%d", DRVN_VIDEO_OUT, u4Idx);

        
        /* Register component with the Resource Manager database */
        i4Ret = x_rm_reg_comp(&tVdoOutCompReg,
                                1, 
                                1, 
                                s_name,
                                u4Flags, 
                                &tVdoOutFctTbl, 
                                NULL, 
                                0);

        if (i4Ret != RMR_OK)
        {
            LOG(1, "MW_VdoOutInit(): x_rm_reg_comp failed [%d]\n", i4Ret);
            return i4Ret;
        }

        _u1VdoOutFlag[u4Idx] = 0;
        /* power down the DAC initialy */
  //      vApiTVESetDacMuteMask(u4Idx, TVE_DAC_MUTE_MASK_DRV, TRUE);
    }

    return (RMR_OK);
#else
    return 0;
#endif
}

static INT32 _VdoOutMwConnect(DRV_COMP_ID_T * pt_comp_id,
                              DRV_CONN_TYPE_T e_conn_type,
                              const VOID * pv_conn_info,
                              SIZE_T z_conn_info_len,
                              VOID * pv_tag, x_rm_nfy_fct pf_nfy)
{

#ifndef CC_DRIVER_PROGRAM

    DRV_COMP_ID_T *prVSrcCompReg;

    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->e_type != DRVT_VIDEO_OUT)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    LOG(8, "*** Connect VdoOut%d ***\r\n", pt_comp_id->ui2_id);

    // ????
    if (pt_comp_id->b_sel_out_port != FALSE)    // must select to input port
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->u.ui1_inp_port > 1)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if (e_conn_type != RM_CONN_TYPE_COMP_ID)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    prVSrcCompReg = (DRV_COMP_ID_T *) pv_conn_info;

    VDOOUT_DBG_MSG(("\r\n*** Connect VdoOut%d to %u tick=%u***\r\n",
                     pt_comp_id->ui2_id,
                     prVSrcCompReg->e_type,
                     x_os_get_sys_tick()));

    UNUSED(prVSrcCompReg);

    return (RMR_OK);
#else
    return 0;
#endif

}

static INT32 _VdoOutMwDisconnect(DRV_COMP_ID_T * pt_comp_id,
                              DRV_DISC_TYPE_T e_disc_type,
                              const VOID * pv_disc_info,
                              SIZE_T z_disc_info_len)
{

#ifndef CC_DRIVER_PROGRAM

    DRV_COMP_ID_T *prVSrcCompReg;
    UINT8 u1DacIdx;
    INT32 i4RetCode = RMR_OK;

    // Check component type and id
    if (!pt_comp_id)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (pt_comp_id->ui2_id == TV_ENC_SCART_1) 
    {
        u1DacIdx = 0; /* DAC_0 */
    }
    else if(pt_comp_id->ui2_id == TV_ENC_SCART_2)
    {
        u1DacIdx = 1; /* DAC_1 */
    }
    else
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    prVSrcCompReg = (DRV_COMP_ID_T *) pv_disc_info;
    if (!prVSrcCompReg)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (pt_comp_id->e_type != DRVT_VIDEO_OUT)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    LOG(8, "*** Disconnect VdoOut%d ***\r\n", pt_comp_id->ui2_id);
    VDOOUT_DBG_MSG(("*** Disconnect VdoOut%d tick=%u***\r\n",
                     pt_comp_id->ui2_id, x_os_get_sys_tick()));

    vApiTVESetDacMuteMask(u1DacIdx, TVE_DAC_MUTE_MASK_DRV, TRUE);
    
    if (pt_comp_id->b_sel_out_port != FALSE)
    {
        i4RetCode = RMR_DRV_INV_DISC_INFO;
    }

    return (i4RetCode);
#else
    return 0;
#endif
}

static INT32 _VdoOutMwGet(DRV_COMP_ID_T * pt_comp_id,
                       DRV_GET_TYPE_T e_get_type,
                       VOID * pv_get_info, SIZE_T * pz_get_info_len)
{
#ifndef CC_DRIVER_PROGRAM

    INT32 i4_return;
    UINT8 u1DacIdx;

    if (pt_comp_id->ui2_id == TV_ENC_SCART_1) 
    {
        u1DacIdx = 0; /* DAC_0 */
    }
    else if(pt_comp_id->ui2_id == TV_ENC_SCART_2)
    {
        u1DacIdx = 1; /* DAC_1 */
    }
    else
    {
        return RMR_DRV_INV_SET_INFO;
    }

    UNUSED(u1DacIdx);

    switch (e_get_type)
    {
    case VIDEO_OUT_GET_TYPE_CTRL:
        if (*pz_get_info_len >= sizeof(VIDEO_OUT_CTRL_T))
        {
            *pz_get_info_len = sizeof(VIDEO_OUT_CTRL_T);
            if( 0 != (_u1VdoOutFlag[pt_comp_id->ui2_id] & VDO_OUT_CTRL_ENABLE_BIT_MASK))
            {
                *(VIDEO_OUT_CTRL_T*)pv_get_info = VIDEO_OUT_CTRL_ENABLE;
            }
            else
            {
                *(VIDEO_OUT_CTRL_T*)pv_get_info = VIDEO_OUT_CTRL_DISABLE;
            }
            i4_return = RMR_OK;
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
        break;
    case VIDEO_OUT_GET_TYPE_MODE:
        if (*pz_get_info_len >= sizeof(VIDEO_OUT_MODE_T))
        {
            *pz_get_info_len = sizeof(VIDEO_OUT_MODE_T);
            if( 0 != (_u1VdoOutFlag[pt_comp_id->ui2_id] & VDO_OUT_MODE_NORMAL_BIT_MASK))
            {
                *(VIDEO_OUT_MODE_T*)pv_get_info = VIDEO_OUT_MODE_NORMAL;
            }
            else
            {
                *(VIDEO_OUT_MODE_T*)pv_get_info = VIDEO_OUT_MODE_BLANK;
            }
            i4_return = RMR_OK;
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
        break;
    default:
        i4_return = RMR_DRV_INV_GET_INFO;
    }

    return (i4_return);

#else /* !CC_DRIVER_PROGRAM */
    return 0;
#endif /* !CC_DRIVER_PROGRAM */
}

static INT32 _VdoOutMwSet(DRV_COMP_ID_T * pt_comp_id,
                       DRV_SET_TYPE_T e_set_type,
                       const VOID * pv_set_info, SIZE_T z_set_info_len)
{
#ifndef CC_DRIVER_PROGRAM

    INT32 i4RetCode = RMR_DRV_INV_SET_INFO;
    UINT8 u1DacIdx;

    if (pt_comp_id->ui2_id == TV_ENC_SCART_1) 
    {
        u1DacIdx = 0; /* DAC_1 */
    }
    else if(pt_comp_id->ui2_id == TV_ENC_SCART_2)
    {
        u1DacIdx = 1; /* DAC_2 */
    }
    else
    {
        return RMR_DRV_INV_SET_INFO;
    }

    /* temporarily always return OK, but doing nothing.... */
    i4RetCode = RMR_OK;

    switch (e_set_type)
    {
    case VIDEO_OUT_SET_TYPE_CTRL:
	if((VIDEO_OUT_CTRL_T)(UINT32)pv_set_info == VIDEO_OUT_CTRL_ENABLE)
	{
            _u1VdoOutFlag[pt_comp_id->ui2_id] |= VDO_OUT_CTRL_ENABLE_BIT_MASK;
	}
	else if((VIDEO_OUT_CTRL_T)(UINT32)pv_set_info == VIDEO_OUT_CTRL_DISABLE)
	{
            _u1VdoOutFlag[pt_comp_id->ui2_id] &= ~VDO_OUT_CTRL_ENABLE_BIT_MASK;
	}
	else
	{
            i4RetCode = RMR_DRV_INV_SET_INFO;
	}
        break;
    case VIDEO_OUT_SET_TYPE_MODE:
	if((VIDEO_OUT_MODE_T)(UINT32)pv_set_info == VIDEO_OUT_MODE_NORMAL)
        {
            _u1VdoOutFlag[pt_comp_id->ui2_id] |= VDO_OUT_MODE_NORMAL_BIT_MASK;
        }
        else if((VIDEO_OUT_MODE_T)(UINT32)pv_set_info == VIDEO_OUT_MODE_BLANK)
        {
            _u1VdoOutFlag[pt_comp_id->ui2_id] &= ~VDO_OUT_MODE_NORMAL_BIT_MASK;
        }
	else
	{
            i4RetCode = RMR_DRV_INV_SET_INFO;
	}

        break;
    default:
        i4RetCode = RMR_DRV_INV_SET_INFO;
        break;
    }

    if((_u1VdoOutFlag[pt_comp_id->ui2_id] & 
        (VDO_OUT_MODE_NORMAL_BIT_MASK|VDO_OUT_CTRL_ENABLE_BIT_MASK)) ==
        (VDO_OUT_MODE_NORMAL_BIT_MASK|VDO_OUT_CTRL_ENABLE_BIT_MASK))
    {
        vApiTVESetDacMuteMask(u1DacIdx, TVE_DAC_MUTE_MASK_MW, FALSE);
    }
    else
    {
        vApiTVESetDacMuteMask(u1DacIdx, TVE_DAC_MUTE_MASK_MW, TRUE);
    }

    return i4RetCode;
#else
    return 0;
#endif
}

#endif
