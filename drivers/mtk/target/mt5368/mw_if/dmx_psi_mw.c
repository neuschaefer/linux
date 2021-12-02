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
#ifdef LINUX_TURNKEY_SOLUTION
#include "x_os.h"
#endif

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

#define DEFINE_IS_LOG   DMX_IsLog
#include "x_debug.h"

#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_data_rm.h"
#endif

LINT_EXT_HEADER_END

#include "dmx_mw.h"

#ifdef LINUX_TURNKEY_SOLUTION
extern int rm_copy_to_user(VOID *pv_user,VOID* pv_kernel,UINT32 size);
extern VOID rm_copy_from_user(VOID* pv_kernel,VOID *pv_user,UINT32 size);
#endif
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
/** _MwDmxSetFilter
 * Tricky for MM TS file
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxSetFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
    const DMX_FILTER_GENERIC_T* prFilter)
{
    BOOL fgRet = FALSE;

    if(u1FilterIndex >= DMX_NUM_FILTER_INDEX)
    {
        return FALSE;
    }

        fgRet = DMX_SetGenFilter(u1FilterIndex, u4Flags, prFilter);

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _MwDmxUnlockBuffer
 *  Unlock a PSI buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  prUnlockInfo    The unlock info
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
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
    u4FrameAddr = (u4SrcTag & 0x1fffffff);
#ifdef LINUX_TURNKEY_SOLUTION
    u4FrameAddr = VIRTUAL(u4FrameAddr);
#endif
    u1SerialNumber = (UINT8)((u4SrcTag >> 29) & 0x7);

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
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxReadBuffer(UINT8 u1Pidx,
    const DEMUX_READ_BUFFER_INFO_T* prReadInfo, UINT32* pu4Size)
{
    UINT32 u4SrcTag, u4FrameAddr;
    UINT8 u1SerialNumber;
    BOOL fgRet;
#ifdef LINUX_TURNKEY_SOLUTION
    static UINT8 *pui1_dest = NULL;
    static UINT32 u4_psi_maxlength = 0;
#endif

    ASSERT(prReadInfo != NULL);
    ASSERT(pu4Size != NULL);

    u4SrcTag = (UINT32)prReadInfo->pv_src_tag;
    u4FrameAddr = (u4SrcTag & 0x1fffffff);
    u1SerialNumber = (UINT8)((u4SrcTag >> 29) & 0x7);

#ifndef LINUX_TURNKEY_SOLUTION
    fgRet = DMX_PSI_GetBuffer(u1Pidx, u1SerialNumber, u4FrameAddr,
        (UINT32)prReadInfo->z_skip_len, (UINT32)prReadInfo->z_copy_len,
        prReadInfo->pui1_dest);
#else
    if (u4FrameAddr != 0)
    {
        u4FrameAddr = VIRTUAL(u4FrameAddr);
    }

    if (u4_psi_maxlength < ((UINT32)prReadInfo->z_copy_len))
    {
        if (pui1_dest != NULL)
        {
            x_mem_free(pui1_dest);
            pui1_dest = NULL;
        }
        u4_psi_maxlength = ((UINT32)prReadInfo->z_copy_len);
        pui1_dest = (UINT8 *)x_mem_alloc((UINT32)prReadInfo->z_copy_len);
    }
    if (pui1_dest == NULL)
    {
        u4_psi_maxlength = 0;
        return FALSE;
    }

    fgRet = DMX_PSI_GetBuffer(u1Pidx, u1SerialNumber, u4FrameAddr,
        (UINT32)prReadInfo->z_skip_len, (UINT32)prReadInfo->z_copy_len,
        pui1_dest);

    rm_copy_to_user(prReadInfo->pui1_dest, pui1_dest, (UINT32)prReadInfo->z_copy_len);
#endif

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
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxSetSectionFilter(UINT8 u1FilterIndex,
    const DEMUX_SECTION_FILTER_INFO_T* prInfo)
{
    DMX_FILTER_GENERIC_T rFilter;
    UINT32 u4Flags;
    UINT8 au1Data[16], au1Mask[16], au1Neg[16], au1PosNeg[16];
    BOOL fgRet;
    UINT8 i;
#ifdef LINUX_TURNKEY_SOLUTION
    UINT8 *pui1_pos_pattern = NULL;
    UINT8 *pui1_pos_mask = NULL;
    UINT8 *pui1_neg_pattern = NULL;
    UINT8 *pui1_neg_mask = NULL;
#endif

    ASSERT(prInfo != NULL);

    // Check pattern length
    if (prInfo->ui1_pattern_len > 15)        // fixme
    {
        fgRet = FALSE;
        goto _exit;
    }

    // Check pattern offset
    if (prInfo->ui1_pattern_offset > DMX_MAX_FILTER_OFFSET)
    {
        fgRet = FALSE;
        goto _exit;
    }

    if(u1FilterIndex >= DMX_NUM_FILTER_INDEX)
    {
        fgRet = FALSE;
        goto _exit;
    }

    x_memset(&rFilter, 0, sizeof(rFilter));

    if(prInfo->ui1_pattern_offset == 0)
    {
        LOG(5, "filter section offset is 0!!\n");
        rFilter.u1Offset = 1;
    }
    else
    {
        rFilter.u1Offset = prInfo->ui1_pattern_offset;
    }

    // Setup filter attributes
    rFilter.fgCheckCrc = prInfo->b_check_crc;
    u4Flags = DMX_FILTER_FLAG_OFFSET | DMX_FILTER_FLAG_CRC |
        DMX_FILTER_FLAG_MODE | DMX_FILTER_FLAG_PATTERN;

    // Clean all masks
    x_memset(au1Data, 0, sizeof(au1Data));
    x_memset(au1Mask, 0, sizeof(au1Mask));
    x_memset(au1Neg, 0, sizeof(au1Neg));
    x_memset(au1PosNeg, 0, sizeof(au1PosNeg));

    // Set table id to first byte
    au1Data[0] = prInfo->ui1_tbl_id_pattern;
    au1Mask[0] = prInfo->ui1_tbl_id_mask;
    au1Neg[0] = 0;
    au1PosNeg[0] = 0; // always fully match table id

    // Convert byte patterns to words
#ifndef LINUX_TURNKEY_SOLUTION
    x_memcpy(au1Data + 1, prInfo->pui1_pos_pattern, prInfo->ui1_pattern_len);
    x_memcpy(au1Mask + 1, prInfo->pui1_pos_mask, prInfo->ui1_pattern_len);
#else
    pui1_pos_pattern = (UINT8 *)x_mem_alloc(prInfo->ui1_pattern_len);
    pui1_pos_mask = (UINT8 *)x_mem_alloc(prInfo->ui1_pattern_len);

    if (pui1_pos_pattern == NULL || pui1_pos_mask == NULL)
    {
        fgRet = FALSE;
        goto _exit;
    }

    rm_copy_from_user(pui1_pos_pattern, prInfo->pui1_pos_pattern, prInfo->ui1_pattern_len);
    rm_copy_from_user(pui1_pos_mask, prInfo->pui1_pos_mask, prInfo->ui1_pattern_len);

    x_memcpy(au1Data + 1, pui1_pos_pattern, prInfo->ui1_pattern_len);
    x_memcpy(au1Mask + 1, pui1_pos_mask, prInfo->ui1_pattern_len);
#endif

    x_memcpy(rFilter.au1Data, au1Data, 16);
    x_memcpy(rFilter.au1Mask, au1Mask, 16);

    // Convert byte patterns to words
    if (prInfo->pui1_neg_pattern != NULL)
    {
#ifndef LINUX_TURNKEY_SOLUTION
        x_memcpy(au1Neg + 1, prInfo->pui1_neg_pattern, prInfo->ui1_pattern_len);
#else
        pui1_neg_pattern = (UINT8 *)x_mem_alloc(prInfo->ui1_pattern_len);

        if (pui1_neg_pattern == NULL)
        {
            fgRet = FALSE;
            goto _exit;
        }

        rm_copy_from_user(pui1_neg_pattern, prInfo->pui1_neg_pattern, prInfo->ui1_pattern_len);
        x_memcpy(au1Neg + 1, pui1_neg_pattern, prInfo->ui1_pattern_len);
#endif
    }

    if (prInfo->pui1_neg_mask != NULL)
    {
#ifndef LINUX_TURNKEY_SOLUTION
        x_memcpy(au1PosNeg + 1, prInfo->pui1_neg_mask, prInfo->ui1_pattern_len);
#else
        pui1_neg_mask = (UINT8 *)x_mem_alloc(prInfo->ui1_pattern_len);

        if (pui1_neg_mask == NULL)
        {
            fgRet = FALSE;
            goto _exit;
        }

        rm_copy_from_user(pui1_neg_mask, prInfo->pui1_neg_mask, prInfo->ui1_pattern_len);
        x_memcpy(au1PosNeg + 1, pui1_neg_mask, prInfo->ui1_pattern_len);
#endif
    }

    // If neg mask = 1, then | it to pattern mask
    // and force overwrite (neg pattern & neg mask) to pattern
    for(i = 1; i < 16; i++)
    {
        au1Mask[i] |= au1PosNeg[i];

        au1Data[i] &= (~au1PosNeg[i]);
        au1Data[i] |= (au1Neg[i] & au1PosNeg[i]);
    }

    x_memcpy(rFilter.au1PosNeg, au1PosNeg, 16);

    // Set pattern/mask/posneg
    if (!_MwDmxSetFilter(u1FilterIndex, u4Flags, &rFilter))
    {
        fgRet = FALSE;
        goto _exit;
    }
    else
    {
        fgRet = TRUE;
        goto _exit;
    }

_exit:
#ifdef LINUX_TURNKEY_SOLUTION
    if(pui1_pos_pattern != NULL)
    {
        x_mem_free(pui1_pos_pattern);
    }
    if(pui1_pos_mask != NULL)
    {
        x_mem_free(pui1_pos_mask);
    }
    if(pui1_neg_pattern != NULL)
    {
        x_mem_free(pui1_neg_pattern);
    }
    if(pui1_neg_mask != NULL)
    {
        x_mem_free(pui1_neg_mask);
    }
#endif

    return fgRet;
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
    DRV_COMP_ID_T* prConnectComp = 0;
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

    prConnectComp = (DRV_COMP_ID_T*)pv_conn_info;
    u1Pidx = _MW_DMX_ConnectInfo(u1Pidx, (UINT32)pt_comp_id->e_type, prConnectComp->e_type,
        prConnectComp->ui2_id);
    if(u1Pidx == 0xff)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    // Set PSI PID
    rPid.u1TsIndex = 0;
    rPid.u1KeyIndex = 0;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.fgPrimary = TRUE;

    // Set these flags only. Other flags are set by 'set' command
    u4Flags = DMX_PID_FLAG_PCR | DMX_PID_FLAG_STEER | DMX_PID_FLAG_DESC_MODE |
        DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_KEY_INDEX | DMX_PID_FLAG_PRIMARY;

    if (!_MW_DMX_SetPid(u1Pidx, u4Flags, &rPid))
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
        return (RMR_DRV_INV_DISC_INFO);
    }

    // 1.2 check id range
    u1Pidx = (UINT8)pt_comp_id->ui2_id;
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    u1Pidx = _MW_DMX_PhysicalPidx(u1Pidx);

    // Disconnect, free pid
    if (!DMX_FreePid(u1Pidx))
    {
        return RMR_DRV_DISC_FAILED;
    }

    // Reset notify info
    if (!_MW_DMX_ResetCallback(u1Pidx))
    {
        return RMR_DRV_DISC_FAILED;
    }

    _MW_DMX_DisConnectInfo((UINT8)pt_comp_id->ui2_id);

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
                    DRV_SET_TYPE_T          e_set_type,
                    const VOID*             pv_set_info,
                    SIZE_T                  z_set_info_len)
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

    u1Pidx = _MW_DMX_PhysicalPidx(u1Pidx);

#ifdef ENABLE_MULTIMEDIA
    if(!_MW_DMX_IsPidConnedted(u1Pidx))
    {
        return RMR_DRV_INV_SET_INFO;
    }
#endif  // ENABLE_MULTIMEDIA

    //set,
    switch (e_set_type)
    {
    case DEMUX_SET_TYPE_PID:
        rPid.u2Pid = (UINT16)(MPEG_2_PID_T)(UINT32)pv_set_info;;
        fgRet = _MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_PID, &rPid);
        break;

    case DEMUX_SET_TYPE_CTRL:
        rPid.fgEnable = (BOOL)(UINT32)pv_set_info;
        fgRet = _MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid);
		LOG(8, "[PSI PARSER]The PSI mw set enable to Pid Filter Pidx is %d,fgEnable is %d\n",u1Pidx,rPid.fgEnable);
        break;

    case DEMUX_SET_TYPE_NFY_FCT:
        fgRet = _MW_DMX_SetCallback(u1Pidx, (DEMUX_NFY_INFO_T*)pv_set_info);
        break;

    case DEMUX_SET_TYPE_ALLOC_BUFFER:
        rPid.u4BufAddr = 0;
        rPid.u4BufSize = (UINT32)(SIZE_T)pv_set_info;
        rPid.ePidType = DMX_PID_TYPE_PSI;
        rPid.fgAllocateBuffer = TRUE;
        fgRet = _MW_DMX_SetPid(u1Pidx, DMX_PID_FLAG_BUFFER, &rPid);
        //fgRet = DMX_PSI_AllocateBuffer(u1Pidx, (UINT32)(SIZE_T)pv_set_info);
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
                        DRV_GET_TYPE_T          e_get_type,
                        VOID*                   pv_get_info,
                        SIZE_T*                 pz_get_info_len)
{
    DMX_PID_T rPid;
    UINT32 u4Size = 0;
    UINT8 u1Pidx;
    BOOL fgRet = FALSE;

    x_memset(&rPid, 0, sizeof(rPid));

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

    u1Pidx = _MW_DMX_PhysicalPidx(u1Pidx);

#ifdef ENABLE_MULTIMEDIA
    if(!_MW_DMX_IsPidConnedted(u1Pidx))
    {
        return RMR_DRV_INV_GET_INFO;
    }
#endif  // ENABLE_MULTIMEDIA

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
    DMX_FILTER_GENERIC_T rFilter;

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

    u1Pidx = _MW_DMX_PhysicalPidx((UINT8)prConnectComp->ui2_id);
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    x_memset(&rFilter, 0, sizeof(rFilter));

    rFilter.u1Pidx = u1Pidx;
    rFilter.u1Offset = 1;
    rFilter.fgCheckCrc = TRUE;
    rFilter.fgEnable = FALSE;
    x_memset(rFilter.au1Data, 0, sizeof(rFilter.au1Data));
    x_memset(rFilter.au1Mask, 0, sizeof(rFilter.au1Mask));
    x_memset(rFilter.au1PosNeg, 0, sizeof(rFilter.au1PosNeg));

    if (!_MwDmxSetFilter(u1FilterIndex, DMX_FILTER_FLAG_ALL, &rFilter))
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
    DMX_FILTER_GENERIC_T rFilter;

    if ((NULL == pt_comp_id) || (NULL == pv_disc_info) )
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    if ( pt_comp_id->e_type != DRVT_DEMUX_SECTION_FILTER )
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    u1FilterIndex = (UINT8)pt_comp_id->ui2_id;
    if (u1FilterIndex >= DMX_NUM_FILTER_INDEX)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    prConnectComp = (DRV_COMP_ID_T*)pv_disc_info;

    if ( prConnectComp->e_type != DRVT_DEMUX_TS_SECTION_MEMORY )
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    u1Pidx = _MW_DMX_PhysicalPidx((UINT8)prConnectComp->ui2_id);
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    x_memset(&rFilter, 0, sizeof(rFilter));

    rFilter.fgEnable = FALSE;
    if (!_MwDmxSetFilter(u1FilterIndex, DMX_FILTER_FLAG_VALID, &rFilter))
    {
        return RMR_DRV_INV_DISC_INFO;
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
                    DRV_SET_TYPE_T          e_set_type,
                    const VOID*             pv_set_info,
                    SIZE_T                  z_set_info_len)
{
    DMX_FILTER_GENERIC_T rFilter;
    INT32 i4Ret = RMR_DRV_INV_SET_INFO;
    UINT8 u1FilterIndex;

    if ((NULL == pt_comp_id) )
    {
        return (RMR_DRV_INV_SET_INFO);
    }

    if ( pt_comp_id->e_type != DRVT_DEMUX_SECTION_FILTER )
    {
       return (RMR_DRV_INV_SET_INFO);
    }

    u1FilterIndex = (UINT8)pt_comp_id->ui2_id;
    if (u1FilterIndex >= DMX_NUM_FILTER_INDEX)
    {
       return (RMR_DRV_INV_SET_INFO);
    }

    x_memset(&rFilter, 0, sizeof(rFilter));

    switch (e_set_type)
    {
    case DEMUX_SET_TYPE_CTRL:
        rFilter.fgEnable = (BOOL)(UINT32)pv_set_info;
        if (_MwDmxSetFilter(u1FilterIndex, DMX_FILTER_FLAG_VALID, &rFilter))
        {
            i4Ret = RMR_OK;
        }
		LOG(8, "[PSI PARSER]The PSI mw set enable to Section FilterIndex is %d,fgEnable is %d\n",u1FilterIndex,rFilter.fgEnable);
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
                        DRV_GET_TYPE_T          e_get_type,
                        VOID*                   pv_get_info,
                        SIZE_T*                 pz_get_info_len)
{
    DMX_FILTER_GENERIC_T rFilter;
    INT32 i4Ret = RMR_DRV_INV_GET_INFO;
    UINT8 u1FilterIndex;

    x_memset(&rFilter, 0, sizeof(rFilter));

    if ((NULL == pt_comp_id) || (NULL == pv_get_info) ||
        (NULL == pz_get_info_len))
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    if ( pt_comp_id->e_type != DRVT_DEMUX_SECTION_FILTER )
    {
       return (RMR_DRV_INV_GET_INFO);
    }

    u1FilterIndex = (UINT8)pt_comp_id->ui2_id;
    if (u1FilterIndex >= DMX_NUM_FILTER_INDEX)
    {
       return (RMR_DRV_INV_GET_INFO);
    }

    switch (e_get_type)
    {
    case DEMUX_GET_TYPE_CTRL:
        if (DMX_GetGenFilter(u1FilterIndex, DMX_FILTER_FLAG_VALID, &rFilter))
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
                        DRVN_DEMUX_TS_SECTION_MEMORY,   /* name */
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
                         DRVN_DEMUX_SECTION_FILTER,     /* component name */
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

