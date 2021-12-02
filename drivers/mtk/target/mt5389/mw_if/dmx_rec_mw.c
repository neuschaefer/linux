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
 * $RCSfile: dmx_rec_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_mw.c
 *  Rec Demux driver - middleware interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


#include "dmx_if.h"
#include "fvr_if.h"
#include "dmx_mw.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "drv_comp_id.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_rec_demux.h"
#include "x_rm_dev_types.h"
#include "x_os.h"

#define DEFINE_IS_LOG	DMX_IsLog
#include "x_debug.h"

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


//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static DMX_MW_PVRPID_INFO_T _arDmxRecPid[REC_DMX_COMP_NUM + REC_DMX_PCR_COMP_NUM];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static BOOL _MwDmxRecResetPidInfo(UINT8 u1Pidx)
{
    if(u1Pidx >= (REC_DMX_COMP_NUM + REC_DMX_PCR_COMP_NUM))
    {
        return FALSE;
    }
    _arDmxRecPid[u1Pidx].fgValid = FALSE;
    _arDmxRecPid[u1Pidx].u2PidNum = 0x1FFF;
    _arDmxRecPid[u1Pidx].rNfyInfo.pf_recdemux_nfy = NULL;
    _arDmxRecPid[u1Pidx].rNfyInfo.pv_tag = NULL;
    _arDmxRecPid[u1Pidx].u4TypeMask = 0;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwDmxRec_Connect
 *  Middleware interface - connect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxRec_Connect(
							const DRV_COMP_ID_T*   pt_comp_id,
							DRV_CONN_TYPE_T  e_conn_type,
							const VOID*      pv_conn_info,
							SIZE_T           z_conn_info_len,
							VOID*            pv_tag,
							x_rm_nfy_fct     pf_nfy)
{
    UINT8 u1Pidx;

    if ((NULL == pt_comp_id) || (NULL == pv_conn_info) )
    {
       	return RMR_DRV_INV_CONN_INFO;
    }

    // Check index range
    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= REC_DMX_COMP_NUM)
    {
        return RMR_DRV_INV_CONN_INFO;
    }
    
    _arDmxRecPid[u1Pidx].fgValid = TRUE;
    _arDmxRecPid[u1Pidx].u4TypeMask = 0;

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmxRec_Disconnect
 *  Middleware interface - disconnect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxRec_Disconnect(
							const DRV_COMP_ID_T*    pt_comp_id,
							DRV_DISC_TYPE_T         e_disc_type,
							const VOID*             pv_disc_info,
							SIZE_T                  z_disc_info_len)
{
    UINT8 u1Pidx;
    
	// Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_disc_info) )
    {
       	return (RMR_DRV_INV_DISC_INFO);
    }

    // Check index range
    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= REC_DMX_COMP_NUM)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if(!_MwDmxRecResetPidInfo(u1Pidx))
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmxRec_Get
 *  Middleware interface - get
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxRec_Get(
						const DRV_COMP_ID_T*    pt_comp_id,
						DRV_GET_TYPE_T		    e_get_type,
						VOID*				    pv_get_info,
						SIZE_T*				    pz_get_info_len)
{
    UINT8 u1Pidx;

	// Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_get_info) )
    {
       	return (RMR_DRV_INV_GET_INFO);
    }

    // Check index range
    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= REC_DMX_COMP_NUM)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    switch (e_get_type)
    {
    case RECDEMUX_GET_TYPE_PID:
        if(!_arDmxRecPid[u1Pidx].fgValid)
        {
            return RMR_DRV_INV_GET_INFO;
        }
        *(UINT16*)pv_get_info = _arDmxRecPid[u1Pidx].u2PidNum;
        *pz_get_info_len = sizeof(UINT16);
        break;

    default:
        break;
    }

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmxRec_Set
 *  Middleware interface - set
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxRec_Set(
					DRV_COMP_ID_T*		pt_comp_id,
					DRV_SET_TYPE_T		e_set_type,
					const VOID*			pv_set_info,
					SIZE_T				z_set_info_len)
{
    RECDEMUX_NFY_INFO_T *prNfyInfo;
    UINT8 u1Pidx;

    if ((NULL == pt_comp_id))
    {
       	return RMR_DRV_INV_SET_INFO;
    }

  	u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= REC_DMX_COMP_NUM)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    if(!_arDmxRecPid[u1Pidx].fgValid)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    switch (e_set_type)
    {
    case RECDEMUX_SET_TYPE_PID:
        _arDmxRecPid[u1Pidx].u2PidNum = (UINT16)(UINT32)pv_set_info;
        break;

    case RECDEMUX_SET_TYPE_DEST_ID:
        break;

    case RECDEMUX_SET_TYPE_NFY_FCT:
        break;

    case RECDEMUX_SET_TYPE_CTRL:
        break;

    case RECDEMUX_SET_TYPE_SCRS_NFY_FCT:
        prNfyInfo = (RECDEMUX_NFY_INFO_T*)pv_set_info;
        _arDmxRecPid[u1Pidx].rNfyInfo.pf_recdemux_nfy = prNfyInfo->pf_recdemux_nfy;
        _arDmxRecPid[u1Pidx].rNfyInfo.pv_tag = prNfyInfo->pv_tag;
        break;

    case RECDEMUX_SET_TYPE_STREAM_TYPE_MASK:
        _arDmxRecPid[u1Pidx].u4TypeMask |= (UINT32)pv_set_info;
        break;

    default:
        break;
    }

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmxRecPCR_Connect
 *  Middleware interface - connect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxRecPCR_Connect(
                            const DRV_COMP_ID_T*   pt_comp_id,
                            DRV_CONN_TYPE_T  e_conn_type,
                            const VOID*      pv_conn_info,
                            SIZE_T           z_conn_info_len,
                            VOID*            pv_tag,
                            x_rm_nfy_fct     pf_nfy)
{
    UINT8 u1Pidx;
    
    if ((NULL == pt_comp_id) || (NULL == pv_conn_info) )
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    // Check index range
    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= REC_DMX_PCR_COMP_NUM)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    u1Pidx += REC_DMX_COMP_NUM;
    
    _arDmxRecPid[u1Pidx].fgValid = TRUE;
    _arDmxRecPid[u1Pidx].u4TypeMask = 0;
    
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmxRecPCR_Disconnect
 *  Middleware interface - disconnect
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxRecPCR_Disconnect(
                            const DRV_COMP_ID_T*    pt_comp_id,
                            DRV_DISC_TYPE_T         e_disc_type,
                            const VOID*             pv_disc_info,
                            SIZE_T                  z_disc_info_len)
{
    UINT8 u1Pidx;
    
    // Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_disc_info) )
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    // Check index range
    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= REC_DMX_PCR_COMP_NUM)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    u1Pidx += REC_DMX_COMP_NUM;

    if(!_MwDmxRecResetPidInfo(u1Pidx))
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmxRecPCR_Get
 *  Middleware interface - get
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxRecPCR_Get(
                        const DRV_COMP_ID_T*    pt_comp_id,
                        DRV_GET_TYPE_T          e_get_type,
                        VOID*                   pv_get_info,
                        SIZE_T*                 pz_get_info_len)
{
    UINT8 u1Pidx;

    // Check parameters
    if ((NULL == pt_comp_id) || (NULL == pv_get_info) )
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    // Check index range
    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= REC_DMX_PCR_COMP_NUM)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    u1Pidx += REC_DMX_COMP_NUM;

    switch (e_get_type)
    {
    case RECDEMUX_GET_TYPE_PID:
        if(!_arDmxRecPid[u1Pidx].fgValid)
        {
            return RMR_DRV_INV_GET_INFO;
        }
        *(UINT16*)pv_get_info = _arDmxRecPid[u1Pidx].u2PidNum;
        *pz_get_info_len = sizeof(UINT16);
        break;

    default:
        break;
    }
    
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmxRecPCR_Set
 *  Middleware interface - set
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxRecPCR_Set(
                    DRV_COMP_ID_T*      pt_comp_id,
                    DRV_SET_TYPE_T      e_set_type,
                    const VOID*         pv_set_info,
                    SIZE_T              z_set_info_len)
{
    //RECDEMUX_NFY_INFO_T *prNfyInfo;
    UINT8 u1Pidx;

    if ((NULL == pt_comp_id))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= REC_DMX_PCR_COMP_NUM)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    u1Pidx += REC_DMX_COMP_NUM;

    if(!_arDmxRecPid[u1Pidx].fgValid)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    switch (e_set_type)
    {
    case RECDEMUX_SET_TYPE_PID:
        _arDmxRecPid[u1Pidx].u2PidNum = (UINT16)(UINT32)pv_set_info;
        break;

    case RECDEMUX_SET_TYPE_DEST_ID:
        break;

    case RECDEMUX_SET_TYPE_NFY_FCT:
        break;

    case RECDEMUX_SET_TYPE_CTRL:
        //prNfyInfo = (RECDEMUX_NFY_INFO_T*)pv_set_info;
        //_arDmxRecPid[u1Pidx].rNfyInfo.pf_recdemux_nfy = prNfyInfo->pf_recdemux_nfy;
        //_arDmxRecPid[u1Pidx].rNfyInfo.pv_tag = prNfyInfo->pv_tag;        
        break;

    default:
        break;
    }

    return RMR_OK;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** MW_Rec_DMX_Init
 *  Initialize demux record driver for middleware interface
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
DMX_MW_PVRPID_INFO_T* _MW_Rec_GetPidInfo(UINT8 u1Pidx)
{
    if (u1Pidx >= (REC_DMX_COMP_NUM + REC_DMX_PCR_COMP_NUM))
    {
        return NULL;
    }

    return &_arDmxRecPid[u1Pidx];
}


//-----------------------------------------------------------------------------
/** MW_Rec_DMX_Init
 *  Initialize demux record driver for middleware interface
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL MW_Rec_DMX_Init(void)
{
    ID_IND_T at_id_ind[REC_DMX_COMP_NUM];
	DRV_COMP_REG_T t_comp_id;
	DRV_COMP_FCT_TBL_T t_comp_fct_tbl;
    UINT32 i, u4Flags;
	INT32 i4Ret;

    for(i=0; i<REC_DMX_COMP_NUM; i++)
    {
        if(!_MwDmxRecResetPidInfo(i))
        {
            return FALSE;
        }	    
    }

    //---------------------------------------------------------
	// Register Record components
    //---------------------------------------------------------
    t_comp_fct_tbl.pf_rm_connect = (x_rm_connect_fct)_MwDmxRec_Connect;
    t_comp_fct_tbl.pf_rm_disconnect = (x_rm_disconnect_fct)_MwDmxRec_Disconnect;
    t_comp_fct_tbl.pf_rm_get = (x_rm_get_fct)_MwDmxRec_Get;
    t_comp_fct_tbl.pf_rm_set = (x_rm_set_fct)_MwDmxRec_Set;
    t_comp_id.e_type = DRVT_DEMUX_REC;
    t_comp_id.e_id_type = ID_TYPE_LIST;
    t_comp_id.u.t_list.ui2_num_of_ids = REC_DMX_COMP_NUM;
    t_comp_id.u.t_list.pt_list = at_id_ind;

    for (i = 0; i < REC_DMX_COMP_NUM; i++)
    {
        t_comp_id.u.t_list.pt_list[i].ui2_id = i;
        t_comp_id.u.t_list.pt_list[i].pv_tag = NULL;
        t_comp_id.u.t_list.pt_list[i].ui1_port = ALL_PORTS;
    }

    u4Flags = DRV_FLAG_SINGLE_CONN_ON_OUTPUT;

    i4Ret = x_rm_reg_comp(&t_comp_id,
                         1,                     // in port num
                         1,                     // out port num
                         NULL,                  // component name
                         u4Flags,               // flags
                         &t_comp_fct_tbl,
                         NULL,
                         0);
    if (i4Ret != RMR_OK)
    {
        return FALSE;
    }

    //---------------------------------------------------------
    // Register Record PCR components
    //---------------------------------------------------------
    t_comp_fct_tbl.pf_rm_connect = (x_rm_connect_fct)_MwDmxRecPCR_Connect;
    t_comp_fct_tbl.pf_rm_disconnect = (x_rm_disconnect_fct)_MwDmxRecPCR_Disconnect;
    t_comp_fct_tbl.pf_rm_get = (x_rm_get_fct)_MwDmxRecPCR_Get;
    t_comp_fct_tbl.pf_rm_set = (x_rm_set_fct)_MwDmxRecPCR_Set;
    t_comp_id.e_type = DRVT_DEMUX_REC_PCR;
    t_comp_id.e_id_type = ID_TYPE_LIST;
    t_comp_id.u.t_list.ui2_num_of_ids = REC_DMX_PCR_COMP_NUM;
    t_comp_id.u.t_list.pt_list = at_id_ind;

    for (i = 0; i < REC_DMX_PCR_COMP_NUM; i++)
    {
        t_comp_id.u.t_list.pt_list[i].ui2_id = i;
        t_comp_id.u.t_list.pt_list[i].pv_tag = NULL;
        t_comp_id.u.t_list.pt_list[i].ui1_port = ALL_PORTS;
    }

    u4Flags = DRV_FLAG_SINGLE_CONN_ON_OUTPUT;

    i4Ret = x_rm_reg_comp(&t_comp_id,
                         1,                     // in port num
                         1,                     // out port num
                         NULL,                  // component name
                         u4Flags,               // flags
                         &t_comp_fct_tbl,
                         NULL,
                         0);
    if (i4Ret != RMR_OK)
    {
        return FALSE;
    }
    

    return TRUE;
}

