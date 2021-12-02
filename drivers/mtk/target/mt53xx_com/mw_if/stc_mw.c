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
 * Description:
 *      Middleware driver - RTC
 *
 *---------------------------------------------------------------------------*/

//----------------------------------------------------------------------------
// Include files
//----------------------------------------------------------------------------
#include "x_stc_ctrl.h"
#include "x_rm_dev_types.h"
#include "x_assert.h"
#include "x_bim.h"
#include "drv_name.h"
#include "stc_drvif.h"

//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Static variables
//----------------------------------------------------------------------------

static STC_CTRL_TYPE_T _eStcCtrlType;
static STC_AV_SYNC_MODE_T _eStcAvSyncMode;
static BOOL _fgStcInvalid = FALSE;

EXTERN BOOL VDP_IsStartToPlay(UCHAR u1B2rId);
EXTERN BOOL VDEC_IsLock(UCHAR ucEsId);
//----------------------------------------------------------------------------
// Static functions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/** _StcCtrlGet() This is the STC Ctrl driver get function for middleware
 * Inputs:
 * Outputs:
 * Returns:
 */
//----------------------------------------------------------------------------
static INT32 _StcCtrlGet
(DRV_COMP_ID_T* ptCompId, DRV_GET_TYPE_T eType, VOID* pvData, SIZE_T* pzSize)
{
    INT32 i4Ret = RMR_OK;
    INT32 i4_value;

    if (ptCompId == NULL || ptCompId->e_type != DRVT_STC_CTRL ||
        ptCompId->ui2_id != 0 || pvData == NULL|| pzSize == NULL)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    switch (eType)
    {
    case STC_CTRL_GET_TYPE_CTRL:
        *(STC_CTRL_TYPE_T*)pvData = _eStcCtrlType;
        break;

    case STC_CTRL_GET_TYPE_STC:
#ifdef CC_53XX_SWDMX_V2
        *(UINT32*)pvData = STC_GetStcValue(ptCompId->ui2_id );
#else
        *(UINT32*)pvData = STC_GetStcValue();
#endif
        i4_value = *(UINT32*)pvData;
        if (i4_value<0)
        {
            *(UINT32*)pvData = 0;
        }
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)        
        if(VDEC_IsLock(0) && !VDP_IsStartToPlay(0))
        {
            *(UINT32*)pvData = 0;
        }
#endif        
        break;
    case STC_CTRL_GET_TYPE_STC_SIGNED:
#ifdef CC_53XX_SWDMX_V2
        *(UINT32*)pvData = STC_GetStcValue(ptCompId->ui2_id );
#else
        *(UINT32*)pvData = STC_GetStcValue();
#endif
        break;

    case STC_CTRL_GET_TYPE_AV_SYNC_MODE:        
        *(STC_AV_SYNC_MODE_T*)pvData = _eStcAvSyncMode;
        break;
        
    default:
        i4Ret = RMR_DRV_INV_GET_INFO;
        break;
    }

    return i4Ret;
}


//----------------------------------------------------------------------------
/** _RtcSet() This is the STC driver set function for middleware
 * Inputs:
 * Outputs:
 * Returns:
 */
//----------------------------------------------------------------------------
static INT32 _StcCtrlSet
(DRV_COMP_ID_T* ptCompId, DRV_SET_TYPE_T eType, const VOID* pvData, SIZE_T zSize)
{
    INT32 i4Ret = RMR_OK;

    if (ptCompId == NULL || ptCompId->e_type != DRVT_STC_CTRL ||
        ptCompId->ui2_id != 0 || zSize == 0 )
    {
        return RMR_DRV_INV_SET_INFO;
    }

    // workaround for Ginga
    if(eType == STC_CTRL_SET_TYPE_STC_INVALID)
    {
        _fgStcInvalid = (BOOL)(UINT32)(pvData);
        return i4Ret;
    }
    if(_fgStcInvalid)
    {
        return i4Ret;
    }

    switch (eType)
    {
    case STC_CTRL_SET_TYPE_CTRL:
        _eStcCtrlType = (STC_CTRL_TYPE_T)(UINT32)pvData;
        if (_eStcCtrlType == STC_CTRL_START)
        {
#ifdef CC_53XX_SWDMX_V2
            STC_StartStc((UINT8)ptCompId->ui2_id);
#else
            STC_StartStc();
#endif
        }
        else if (_eStcCtrlType == STC_CTRL_STOP)
        {
#ifdef CC_53XX_SWDMX_V2
            STC_StopStc((UINT8)ptCompId->ui2_id);
#else
            STC_StopStc();
#endif        
        }
        break;

    case STC_CTRL_SET_TYPE_STC:
#ifdef CC_53XX_SWDMX_V2
        STC_SetStcValue((UINT8)ptCompId->ui2_id, (UINT32)pvData);
#else
        STC_SetStcValue((UINT32)pvData);
#endif        
        break;

    case STC_CTRL_SET_TYPE_AV_SYNC_MODE:
        
        _eStcAvSyncMode = (STC_AV_SYNC_MODE_T)(UINT32)pvData;
        
        if (_eStcAvSyncMode == STC_AV_SYNC_MODE_AUDIO_MASTER)
        {
#ifdef CC_53XX_SWDMX_V2
            STC_SetSyncMode((UINT8)ptCompId->ui2_id, 0, 0, AV_SYNC_MODE_AUDIO_MASTER); 
#else
            STC_SetSyncMode(AV_SYNC_MODE_AUDIO_MASTER); 
#endif        
        }
        else if (_eStcAvSyncMode == STC_AV_SYNC_MODE_VIDEO_MASTER)
        {
#ifdef CC_53XX_SWDMX_V2
            STC_SetSyncMode((UINT8)ptCompId->ui2_id, 0, 0, AV_SYNC_MODE_VIDEO_MASTER); 
#else
            STC_SetSyncMode(AV_SYNC_MODE_VIDEO_MASTER); 
#endif
        }
        else if (_eStcAvSyncMode == STC_AV_SYNC_MODE_SYSTEM_MASTER)
        {
#ifdef CC_53XX_SWDMX_V2
            STC_SetSyncMode((UINT8)ptCompId->ui2_id, 0, 0, AV_SYNC_MODE_SYSTEM_MASTER); 
#else
            STC_SetSyncMode(AV_SYNC_MODE_SYSTEM_MASTER); 
#endif
        }
        else
        {
            _eStcAvSyncMode = STC_AV_SYNC_MODE_FREE_RUN;
#ifdef CC_53XX_SWDMX_V2
            STC_SetSyncMode((UINT8)ptCompId->ui2_id, 0, 0, AV_SYNC_MODE_NONE); 
#else
            STC_SetSyncMode(AV_SYNC_MODE_NONE); 
#endif
        }                         
        break;
        
    default:
        i4Ret = RMR_DRV_INV_SET_INFO;
        break;
    }

    return i4Ret;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

INT32 MW_StcCtrl_Init(void)
{
    INT32  i4Ret;
    UINT32 u4Flags = DRV_FLAG_SINGLE_CONN_ON_OUTPUT;
    // Component ID
    DRV_COMP_REG_T rComp;
    // Function table
    DRV_COMP_FCT_TBL_T rFctTbl =
    {
    	NULL,
    	NULL,
    	_StcCtrlGet,
    	_StcCtrlSet
    };

    rComp.e_type = DRVT_STC_CTRL;
    rComp.e_id_type = ID_TYPE_IND;

    rComp.u.t_ind.ui2_id = 0;
    rComp.u.t_ind.pv_tag = NULL;
    rComp.u.t_ind.ui1_port = 0;
        
    // Register component to Resource Manager database
    i4Ret = x_rm_reg_comp(&rComp, 1, 1, DRVN_STC_CTRL, u4Flags, &rFctTbl, NULL, 0);

    return i4Ret;
}


