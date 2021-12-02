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
 * $RCSfile: dmx_psi_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_psi_mw.c
 *  Demux driver - middleware interface of PSI
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mw.h"
#include "dmx_if.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_common.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_demux.h"
#include "x_assert.h"

#define __DMX_MW_API__  // avoid duplicate define for PSI MW and Driver
#include "x_printf.h"
#include "drv_comp_id.h"
#include "drv_name.h"

#define DEFINE_IS_LOG	DMX_IsLog
#include "x_debug.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// Section filter flag:
//  - Support negtive pattern match
//  - Support including section length
//  - Support filter offset
#define _MW_SECTION_FILTER_REG_FLAG \
    (DEMUX_FLAG_NEG_PATTERN_MATCH | \
    DEMUX_FLAG_INCL_SECTION_LENGTH | \
    DEMUX_FLAG_ALLOW_FILTER_OFFSET)


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _MwDmxUnlockBuffer
 *  Unlock a PSI buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  prUnlockInfo    The unlock info
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxUnlockBuffer(UINT8 u1Pidx,
    const DEMUX_UNLOCK_BUFFER_INFO_T* prUnlockInfo)
{
    UINT32 u4Size, u4SrcTag, u4FrameAddr;
    UINT8 u1SerialNumber;

    ASSERT(prUnlockInfo != NULL);

    u4Size = (UINT32)prUnlockInfo->z_src_len;
    u4SrcTag = (UINT32)prUnlockInfo->pv_src_tag;
    u4FrameAddr = (u4SrcTag & 0x0fffffff);
    u1SerialNumber = (UINT8)((u4SrcTag >> 28) & 0xf);

    return DMX_PSI_UnlockBuffer(u1Pidx, u1SerialNumber, u4Size, u4FrameAddr);
}


//-----------------------------------------------------------------------------
/** _MwDmxReadBuffer
 *  Read a PSI buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  prReadInfo      The read info
 *  @param  pu4Size[output] The size actually read
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxReadBuffer(UINT8 u1Pidx,
    const DEMUX_READ_BUFFER_INFO_T* prReadInfo, UINT32* pu4Size)
{
    UINT32 u4SrcTag, u4FrameAddr;
    UINT8 u1SerialNumber;
    BOOL fgRet;

    ASSERT(prReadInfo != NULL);
    ASSERT(pu4Size != NULL);

    u4SrcTag = (UINT32)prReadInfo->pv_src_tag;
    u4FrameAddr = (u4SrcTag & 0x0fffffff);
    u1SerialNumber = (UINT8)((u4SrcTag >> 28) & 0xf);

    fgRet = DMX_PSI_GetBuffer(u1Pidx, u1SerialNumber, u4FrameAddr,
        (UINT32)prReadInfo->z_skip_len, (UINT32)prReadInfo->z_copy_len,
        prReadInfo->pui1_dest);

    if (fgRet)
    {
        *pu4Size = (UINT32)prReadInfo->z_copy_len;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _MwDmxSetSectionFilter
 *  Set section filter
 *
 *  @param  u1FilterIndex   The filter index
 *  @param  prInfo          The filter info
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxSetSectionFilter(UINT8 u1FilterIndex,
    const DEMUX_SECTION_FILTER_INFO_T* prInfo)
{
    DMX_FILTER_T rFilter;
    UINT32 u4Flags;
    UINT8 au1Data[8], au1Mask[8];

    ASSERT(prInfo != NULL);

    // Check pattern length
    if (prInfo->ui1_pattern_len > 7)        // fixme
    {
        return FALSE;
    }

    // Check pattern offset
    if (prInfo->ui1_pattern_offset > DMX_MAX_FILTER_OFFSET)
    {
        return FALSE;
    }

    rFilter.u1Offset = prInfo->ui1_pattern_offset;

    // Setup filter attributes
    rFilter.fgCheckCrc = prInfo->b_check_crc;
    rFilter.eMode = DMX_FILTER_MODE_POS_NEG;
    u4Flags = DMX_FILTER_FLAG_OFFSET | DMX_FILTER_FLAG_CRC |
        DMX_FILTER_FLAG_MODE | DMX_FILTER_FLAG_PATTERN;

    // Clean all masks
    x_memset(au1Data, 0, 8);
    x_memset(au1Mask, 0, 8);

    // Set table id to first byte
    au1Data[0] = prInfo->ui1_tbl_id_pattern;
    au1Mask[0] = prInfo->ui1_tbl_id_mask;

    // Convert byte patterns to words
    x_memcpy(au1Data + 1, prInfo->pui1_pos_pattern, prInfo->ui1_pattern_len);
    x_memcpy(au1Mask + 1, prInfo->pui1_pos_mask, prInfo->ui1_pattern_len);

	rFilter.au4Data[0] = (au1Data[0] << 24) | (au1Data[1] << 16) |
	    (au1Data[2] << 8) | au1Data[3];
	rFilter.au4Data[1] = (au1Data[4] << 24) | (au1Data[5] << 16) |
		(au1Data[6] << 8) | au1Data[7];

	rFilter.au4Mask[0] = (au1Mask[0] << 24) | (au1Mask[1] << 16) |
	    (au1Mask[2] << 8) | au1Mask[3];
	rFilter.au4Mask[1] = (au1Mask[4] << 24) | (au1Mask[5] << 16) |
		(au1Mask[6] << 8) | au1Mask[7];

    // Set positive pattern to bank 0
    if (!DMX_SetFilter(u1FilterIndex, 0, u4Flags, &rFilter))
    {
        return FALSE;
    }

    // Convert byte patterns to words
    if (prInfo->pui1_neg_pattern != NULL)
    {
        x_memcpy(au1Data + 1, prInfo->pui1_neg_pattern, prInfo->ui1_pattern_len);

    	rFilter.au4Data[0] = (au1Data[0] << 24) | (au1Data[1] << 16) |
    	    (au1Data[2] << 8) | au1Data[3];
    	rFilter.au4Data[1] = (au1Data[4] << 24) | (au1Data[5] << 16) |
    		(au1Data[6] << 8) | au1Data[7];
    }
    else
    {
        rFilter.au4Data[0] = 0;
        rFilter.au4Data[1] = 0;
    }

    if (prInfo->pui1_neg_mask != NULL)
    {
        x_memcpy(au1Mask + 1, prInfo->pui1_neg_mask, prInfo->ui1_pattern_len);

    	rFilter.au4Mask[0] = (au1Mask[0] << 24) | (au1Mask[1] << 16) |
    	    (au1Mask[2] << 8) | au1Mask[3];
    	rFilter.au4Mask[1] = (au1Mask[4] << 24) | (au1Mask[5] << 16) |
    		(au1Mask[6] << 8) | au1Mask[7];
    }
    else
    {
        rFilter.au4Mask[0] = 0;
        rFilter.au4Mask[1] = 0;
    }

    // Set negtive pattern to bank 1
    if (!DMX_SetFilter(u1FilterIndex, 1, u4Flags, &rFilter))
    {
        return FALSE;
    }

    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Notes: record and set something on hardware(src, dest, type...)
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxPsi_Connect(
							const DRV_COMP_ID_T*    pt_comp_id,
							DRV_CONN_TYPE_T         e_conn_type,
							const VOID*             pv_conn_info,
							SIZE_T                  z_conn_info_len,
							VOID*                   pv_tag,
							x_rm_nfy_fct            pf_nfy)
{
    DMX_PID_T rPid;
    UINT32 u4Flags;
    UINT8 u1Pidx;

	// 1. Check para valid
    if ((NULL == pt_comp_id) || (NULL == pv_conn_info) )
    {
       	return (RMR_DRV_INV_CONN_INFO);
    }

    // 1.2 check id range
  	u1Pidx = (UINT8)pt_comp_id->ui2_id;
  	if (u1Pidx >= DMX_NUM_PID_INDEX)
  	{
  	    return RMR_DRV_INV_CONN_INFO;
  	}

	if (e_conn_type != RM_CONN_TYPE_COMP_ID)
	{
       return RMR_DRV_INV_CONN_INFO;
	}

	// Set PSI PID
    rPid.u1TsIndex = 0;
    rPid.u1KeyIndex = 0;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
	rPid.u1SteerMode = DMX_STEER_TO_FTUP;
	rPid.ePcrMode = DMX_PCR_MODE_NONE;

    // Set these flags only. Other flags are set by 'set' command
    u4Flags = DMX_PID_FLAG_PCR | DMX_PID_FLAG_STEER | DMX_PID_FLAG_DESC_MODE |
        DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_KEY_INDEX;

    if (!DMX_SetPid(u1Pidx, u4Flags, &rPid))
    {
        return RMR_DRV_CONN_FAILED;
    }

    UNUSED(z_conn_info_len);
    UNUSED(pv_tag);
    UNUSED(pf_nfy);

    return RMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:  just record, since it's hardwire
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxPsi_Disconnect(
							const DRV_COMP_ID_T*    pt_comp_id,
							DRV_DISC_TYPE_T         e_disc_type,
							const VOID*             pv_disc_info,
							SIZE_T                  z_disc_info_len)
{
    UINT8 u1Pidx;

	// 1. Check para valid
    if ((NULL == pt_comp_id) || (NULL == pv_disc_info) )
    {
       	return (RMR_DRV_INV_CONN_INFO);
    }

    // 1.2 check id range
  	u1Pidx = (UINT8)pt_comp_id->ui2_id;
  	if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

	// Disconnect, free pid
	if (!DMX_FreePid(u1Pidx))
	{
	    return RMR_DRV_INV_CONN_INFO;
	}

    // Reset notify info
    if (!_MW_DMX_ResetCallback(u1Pidx))
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    UNUSED(e_disc_type);
    UNUSED(z_disc_info_len);

    return RMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxPsi_Set(
					const DRV_COMP_ID_T*    pt_comp_id,
					DRV_SET_TYPE_T		    e_set_type,
					const VOID*			    pv_set_info,
					SIZE_T				    z_set_info_len)
{
    DMX_PID_T rPid;
    UINT8 u1Pidx;
    BOOL fgRet = FALSE;

	//1. Check para valid
    if ((NULL == pt_comp_id))
    {
       	return RMR_DRV_INV_SET_INFO;
    }

    //1.2 check id range
  	u1Pidx = (UINT8)pt_comp_id->ui2_id;
  	if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
       return RMR_DRV_INV_SET_INFO;
    }

    //set,
    switch (e_set_type)
    {
    case DEMUX_SET_TYPE_PID:
        rPid.u2Pid = (UINT16)(MPEG_2_PID_T)(UINT32)pv_set_info;;
        fgRet = DMX_SetPid(u1Pidx, DMX_PID_FLAG_PID, &rPid);
        break;

    case DEMUX_SET_TYPE_CTRL:
        rPid.fgEnable = (BOOL)(UINT32)pv_set_info;
        fgRet = DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid);
        break;

    case DEMUX_SET_TYPE_NFY_FCT:
        fgRet = _MW_DMX_SetCallback(u1Pidx, (DEMUX_NFY_INFO_T*)pv_set_info);
        break;

    case DEMUX_SET_TYPE_ALLOC_BUFFER:
        fgRet = DMX_PSI_AllocateBuffer(u1Pidx, (UINT32)(SIZE_T)pv_set_info);
        break;

    case DEMUX_SET_TYPE_FREE_BUFFER:
        fgRet = DMX_PSI_FreeBuffer(u1Pidx);
        break;

    case DEMUX_SET_TYPE_UNLOCK_BUFFER:
        fgRet = _MwDmxUnlockBuffer(u1Pidx,
            (DEMUX_UNLOCK_BUFFER_INFO_T*)pv_set_info);
        break;

    case DEMUX_SET_TYPE_FILTER_LEN:
        fgRet = TRUE;   // FIXME
        break;

    case DEMUX_SET_TYPE_FLUSH_BUFFER:
        fgRet = DMX_PSI_FlushBuffer(u1Pidx);
        break;

    default:
        break;
    }

    UNUSED(z_set_info_len);

    return (fgRet ? RMR_OK : RMR_DRV_INV_SET_INFO);
}


/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxPsi_Get(
						const DRV_COMP_ID_T*    pt_comp_id,
						DRV_GET_TYPE_T		    e_get_type,
						VOID*				    pv_get_info,
						SIZE_T*				    pz_get_info_len)
{
    DMX_PID_T rPid;
    UINT32 u4Size = 0;
    UINT8 u1Pidx;
    BOOL fgRet = FALSE;

	// 1. Check para valid
    if ((NULL == pt_comp_id) || (NULL == pv_get_info) ||
        (NULL == pz_get_info_len))
    {
       	return RMR_DRV_INV_GET_INFO;
    }

    // 1.2 check id range
  	u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
       return RMR_DRV_INV_GET_INFO;
    }

    //get
    switch (e_get_type)
    {
    case DEMUX_GET_TYPE_PID:
        // Get PID value
        if (DMX_GetPid(u1Pidx, DMX_PID_FLAG_PID, &rPid))
        {
            *(MPEG_2_PID_T*)pv_get_info = rPid.u2Pid;
            *pz_get_info_len = sizeof(MPEG_2_PID_T);
            fgRet = TRUE;
        }
        break;

    case DEMUX_GET_TYPE_CTRL:
        // Query if a PID is enabled or not
        if (DMX_GetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            *(BOOL*)pv_get_info = rPid.fgEnable;
            *pz_get_info_len = sizeof(BOOL);
            fgRet = TRUE;
        }
        break;

    case DEMUX_GET_TYPE_READ_BUFFER_NFY:
    case DEMUX_GET_TYPE_READ_BUFFER:
        if (_MwDmxReadBuffer(u1Pidx, (DEMUX_READ_BUFFER_INFO_T*)pv_get_info,
            &u4Size))
        {
            *pz_get_info_len = (SIZE_T)u4Size;
            fgRet = TRUE;
        }
        break;

    case DEMUX_GET_TYPE_FILTER_LEN:
        *(UINT8*) pv_get_info = 8;          // FIXME
        *pz_get_info_len = sizeof(UINT8);   // FIXME
        fgRet = TRUE;
        break;

    default:
        break;
    }

    return (fgRet ? RMR_OK : RMR_DRV_INV_GET_INFO);
}


/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Notes: record and set something on hardware(src, dest, type...)
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxFilter_Connect(
							const DRV_COMP_ID_T*    pt_comp_id,
							DRV_CONN_TYPE_T         e_conn_type,
							const VOID*             pv_conn_info,
							SIZE_T                  z_conn_info_len,
							VOID*                   pv_tag,
							x_rm_nfy_fct            pf_nfy)
{
    UINT8 u1Pidx, u1FilterIndex;
	DRV_COMP_ID_T* prConnectComp = 0;
	DMX_FILTER_T rFilter;

    if ((NULL == pt_comp_id) || (NULL == pv_conn_info) )
    {
       	return (RMR_DRV_INV_CONN_INFO);
    }

    if ( pt_comp_id->e_type != DRVT_DEMUX_SECTION_FILTER )
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

  	u1FilterIndex =  (UINT8)pt_comp_id->ui2_id;
    if (u1FilterIndex >= DMX_NUM_FILTER_INDEX)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

	if (e_conn_type != RM_CONN_TYPE_COMP_ID)
	{
       return (RMR_DRV_INV_CONN_INFO);
	}

	prConnectComp = (DRV_COMP_ID_T*)pv_conn_info;

    if ( prConnectComp->e_type != DRVT_DEMUX_TS_SECTION_MEMORY )
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    u1Pidx = (UINT8)prConnectComp->ui2_id ;
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    rFilter.u1Pidx = u1Pidx;
    rFilter.u1Offset = 0;
    rFilter.fgCheckCrc = TRUE;
    rFilter.fgEnable = FALSE;
    rFilter.eMode = DMX_FILTER_MODE_POS_NEG;
    rFilter.au4Data[0] = 0;
    rFilter.au4Data[1] = 0;
    rFilter.au4Mask[0] = 0;
    rFilter.au4Mask[1] = 0;

    if (!DMX_SetFilter(u1FilterIndex, 0, DMX_FILTER_FLAG_ALL, &rFilter))
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    if (!DMX_SetFilter(u1FilterIndex, 1, DMX_FILTER_FLAG_ALL, &rFilter))
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    UNUSED(z_conn_info_len);
    UNUSED(pv_tag);
    UNUSED(pf_nfy);

    return RMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:  just record, since it's hardwire
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxFilter_Disconnect(
							const DRV_COMP_ID_T*    pt_comp_id,
							DRV_DISC_TYPE_T         e_disc_type,
							const VOID*             pv_disc_info,
							SIZE_T                  z_disc_info_len)
{
    UINT8 u1Pidx, u1FilterIndex;
	DRV_COMP_ID_T* prConnectComp = 0;
	DMX_FILTER_T rFilter;

    if ((NULL == pt_comp_id) || (NULL == pv_disc_info) )
    {
       	return (RMR_DRV_INV_CONN_INFO);
    }

    if ( pt_comp_id->e_type != DRVT_DEMUX_SECTION_FILTER )
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

  	u1FilterIndex = (UINT8)pt_comp_id->ui2_id;
    if (u1FilterIndex >= DMX_NUM_FILTER_INDEX)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

	prConnectComp = (DRV_COMP_ID_T*)pv_disc_info;

    if ( prConnectComp->e_type != DRVT_DEMUX_TS_SECTION_MEMORY )
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    u1Pidx = (UINT8)prConnectComp->ui2_id;
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    rFilter.fgEnable = FALSE;
    if (!DMX_SetFilter(u1FilterIndex, 0, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        return RMR_DRV_INV_CONN_INFO;
    }
    if (!DMX_SetFilter(u1FilterIndex, 1, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    UNUSED(e_disc_type);
    UNUSED(z_disc_info_len);

    return RMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxFilter_Set(
					const DRV_COMP_ID_T*    pt_comp_id,
					DRV_SET_TYPE_T		    e_set_type,
					const VOID*			    pv_set_info,
					SIZE_T				    z_set_info_len)
{
    DMX_FILTER_T rFilter;
	INT32 i4Ret = RMR_DRV_INV_SET_INFO;
    UINT8 u1FilterIndex;

    if ((NULL == pt_comp_id) )
    {
       	return (RMR_DRV_INV_SET_INFO);
    }

    if ( pt_comp_id->e_type != DRVT_DEMUX_SECTION_FILTER )
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

  	u1FilterIndex = (UINT8)pt_comp_id->ui2_id;
    if (u1FilterIndex >= DMX_NUM_FILTER_INDEX)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    switch (e_set_type)
    {
    case DEMUX_SET_TYPE_CTRL:
        rFilter.fgEnable = (BOOL)(UINT32)pv_set_info;
        if (DMX_SetFilter(u1FilterIndex, 0, DMX_FILTER_FLAG_VALID, &rFilter))
        {
            i4Ret = RMR_OK;
        }
        if (DMX_SetFilter(u1FilterIndex, 1, DMX_FILTER_FLAG_VALID, &rFilter))
        {
            i4Ret = RMR_OK;
        }
        break;

    case DEMUX_SET_TYPE_SECTION_FILTER:
        if (_MwDmxSetSectionFilter(u1FilterIndex,
            (DEMUX_SECTION_FILTER_INFO_T*)pv_set_info))
        {
            i4Ret = RMR_OK;
        }
        break;

    default:
        break;
    }

    UNUSED(z_set_info_len);

    return i4Ret;
}


/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxFilter_Get(
						const DRV_COMP_ID_T*    pt_comp_id,
						DRV_GET_TYPE_T		    e_get_type,
						VOID*				    pv_get_info,
						SIZE_T*				    pz_get_info_len)
{
    DMX_FILTER_T rFilter;
	INT32 i4Ret = RMR_DRV_INV_GET_INFO;
    UINT8 u1FilterIndex;

    if ((NULL == pt_comp_id) || (NULL == pv_get_info) ||
        (NULL == pz_get_info_len))
    {
       	return (RMR_DRV_INV_GET_INFO);
    }

    if ( pt_comp_id->e_type != DRVT_DEMUX_SECTION_FILTER )
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

  	u1FilterIndex = (UINT8)pt_comp_id->ui2_id;
    if (u1FilterIndex >= DMX_NUM_FILTER_INDEX)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    switch (e_get_type)
    {
    case DEMUX_GET_TYPE_CTRL:
        if (DMX_GetFilter(u1FilterIndex, 0, DMX_FILTER_FLAG_VALID, &rFilter))
        {
            *pz_get_info_len = sizeof (BOOL);
            *(BOOL*)pv_get_info = rFilter.fgEnable;
            i4Ret = RMR_OK;
        }
        break;

    default:
        break;
    }

    return i4Ret;
}


 /*-----------------------------------------------------------------------------
 * Name:
 *
 * Description: Registration
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:  #define DMX_SECTION_MEM_COMP_NUM, DMX_SECTION_MEM_COMP_ID_START
 ----------------------------------------------------------------------------*/
BOOL _MW_DMX_PSI_Init(void)
{
    UINT32 u4i = 0, u4Flags;
	INT32                ret;
	DRV_COMP_REG_T       t_comp_id;
	DRV_COMP_FCT_TBL_T   t_comp_fct_tbl;
	ID_IND_T             at_id_ind_sec[DMX_SECTION_MEM_COMP_NUM];
	ID_IND_T             at_id_ind_fil[DMX_SECTION_FILTER_COMP_NUM];

	/* register Section Memory component */
	t_comp_fct_tbl.pf_rm_connect = (x_rm_connect_fct)_MwDmxPsi_Connect;
	t_comp_fct_tbl.pf_rm_disconnect = (x_rm_disconnect_fct)_MwDmxPsi_Disconnect;
	t_comp_fct_tbl.pf_rm_get = (x_rm_get_fct)_MwDmxPsi_Get;
	t_comp_fct_tbl.pf_rm_set = (x_rm_set_fct)_MwDmxPsi_Set;

	t_comp_id.e_type = DRVT_DEMUX_TS_SECTION_MEMORY;
	t_comp_id.e_id_type = ID_TYPE_LIST;
	t_comp_id.u.t_list.ui2_num_of_ids = DMX_SECTION_MEM_COMP_NUM;
	t_comp_id.u.t_list.pt_list = at_id_ind_sec;

	for ( u4i = 0; u4i < DMX_SECTION_MEM_COMP_NUM; u4i++ )
	{
		at_id_ind_sec[u4i].ui2_id = DMX_SECTION_MEM_COMP_ID_START + u4i;
		at_id_ind_sec[u4i].pv_tag = NULL;
		at_id_ind_sec[u4i].ui1_port = ALL_PORTS;
	}

    u4Flags = DRV_FLAG_SINGLE_CONN_ON_OUTPUT;

	ret = x_rm_reg_comp(&t_comp_id,
	                    1,                              /* num in port */
	                    1,                              /* num out port */
	                    DRVN_DEMUX_TS_SECTION_MEMORY,	/* name */
	                    u4Flags,                        /* flag */
	                    &t_comp_fct_tbl,
	                    NULL,
	                    0);
	if ( ret != RMR_OK )
	{
		return FALSE;
	}

	/*======================================================================*/
	/* register Section Filter component */
	t_comp_fct_tbl.pf_rm_connect    = (x_rm_connect_fct)_MwDmxFilter_Connect;
	t_comp_fct_tbl.pf_rm_disconnect = (x_rm_disconnect_fct)_MwDmxFilter_Disconnect;
	t_comp_fct_tbl.pf_rm_get        = (x_rm_get_fct)_MwDmxFilter_Get;
	t_comp_fct_tbl.pf_rm_set        = (x_rm_set_fct)_MwDmxFilter_Set;

	t_comp_id.e_type                   = DRVT_DEMUX_SECTION_FILTER;
	t_comp_id.e_id_type                = ID_TYPE_LIST;
	t_comp_id.u.t_list.ui2_num_of_ids  = DMX_SECTION_FILTER_COMP_NUM;
	t_comp_id.u.t_list.pt_list         = at_id_ind_fil;

	for ( u4i = 0; u4i < DMX_SECTION_FILTER_COMP_NUM; u4i++ )
	{
		at_id_ind_fil[u4i].ui2_id = DMX_SECTION_FILTER_COMP_ID_START + u4i;
		at_id_ind_fil[u4i].pv_tag = NULL;
		at_id_ind_fil[u4i].ui1_port = ALL_PORTS;
	}

    u4Flags |= _MW_SECTION_FILTER_REG_FLAG;

	ret = x_rm_reg_comp( &t_comp_id,
	                     1,                             /* num in port */
	                     1,                             /* num out port */
	                     DRVN_DEMUX_SECTION_FILTER,		/* component name */
	                     u4Flags,                       /* flag */
	                     &t_comp_fct_tbl,
	                     NULL,
	                     0 );
	if ( ret != RMR_OK )
	{
		return FALSE;
	}

    // Connection lists and exclusive lists are moved to mw_post_init.c

    return TRUE;
}
