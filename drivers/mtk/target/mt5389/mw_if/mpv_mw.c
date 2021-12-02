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
 * $RCSfile: mpv_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file mpv_mw.c
 *  This file defines the interfaces for middleware to access MPV driver
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "drv_comp_id.h"
#include "drv_name.h"

#define DEFINE_IS_LOG    MPV_IsLog
#include "x_debug.h"
#include "x_assert.h"

#include "x_rm.h"
#ifndef LINUX_TURNKEY_SOLUTION
#include "x_rm_dev_types.h"
#else
#include "u_rm_dev_types.h"
#endif
#include "x_vid_dec.h"
#ifndef LINUX_TURNKEY_SOLUTION
#include "u_mm_common.h"
#endif
#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#endif
#include "vdp_drvif.h"
#include "vdp_if.h"
#include "b2r_drvif.h"
#include "b2r_if.h"
#include "vdec_if.h"
//#ifndef LINUX_TURNKEY_SOLUTION
#include "vdec_drvif.h"
//#endif
#include "stc_drvif.h"
#include "mpv_drvif.h"
#include "dmx_if.h"

#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_data_rm.h"
#include "cb_param.h"
#endif

#ifdef ENABLE_MULTIMEDIA
#include "swdmx_drvif.h"
#endif
#include "drvcust_if.h"

#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif

#ifdef CC_BOOT_VDEC_LOGO
#include "eepdef.h"
#include "api_eep.h"
#endif

#ifdef CC_3D_SUPPORT
#include "drv_tdtv_drvif.h"
#endif
#include "u_drv_cust.h"
//#define MPV_MW_GET_STATUS
#ifdef MPV_MW_GET_STATUS
#include "drv_dbase.h"
#endif

LINT_EXT_HEADER_END
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define COMP_ID_UNKNOWN    0xFF
#define ES_ID_UNKNOWN 0xFF
#define IFRAME_ES_ID    0x0
#if defined(CC_3D_MM_DS_SUPPORT) || defined(CC_FBM_TWO_FBP)
#define ES_ID_MAX 0x2
#else
#define ES_ID_MAX 0x1
#endif
#define LOGO_DATA_SIZE 0x9

#define DRV_SET_TIMESTAMP(module)     x_os_drv_set_timestamp(module)

typedef enum{
    DRIVER_CTRL_VDEC_PLAY = 0,
    DRIVER_CTRL_END
}DRIVER_CTRL_T;

#ifdef LINUX_TURNKEY_SOLUTION

MPV_SET_NFY_PARAM mpv_set_param;

static x_vid_dec_nfy_fct mpv_set_nfy_tbl[MPV_COMP_NS];

extern unsigned char adapt_fbm_is_fbm_usr_vir(unsigned int addr);
extern unsigned char adapt_fbm_is_fbm_knl_vir(unsigned int addr);
extern unsigned int adapt_fbm_knl_to_usr(unsigned int knl_addr);
extern unsigned int adapt_fbm_usr_to_knl(unsigned int usr_addr);
extern VOID rm_copy_from_user(VOID* pv_kernel,VOID *pv_user,UINT32 size);
extern INT32 _CB_PutEvent_Adpt(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

static VOID set_mpv_set_nfy(UINT32 ui4_id)
{
    mpv_set_param.pf_nfy = mpv_set_nfy_tbl[ui4_id];
}

VOID mpv_nfy_fct(
    VOID*               pv_nfy_tag,
    VID_DEC_COND_T      e_nfy_cond,
    UINT32              ui4_data_1,
    UINT32              ui4_data_2)
{
    mpv_set_param.pv_nfy_tag = pv_nfy_tag;
    mpv_set_param.e_nfy_cond = e_nfy_cond;
    mpv_set_param.ui4_data_1 = ui4_data_1;
    mpv_set_param.ui4_data_2 = ui4_data_2;
    _CB_PutEvent_Adpt(CB_DRV_MPV_SET_NFY, sizeof(MPV_SET_NFY_PARAM), &mpv_set_param);
}

MPV_FRAME_NFY_PARAM mpv_frame_param;

static x_vid_dec_nfy_fct mpv_frame_nfy_tbl[MPV_COMP_NS];

static VOID set_mpv_frame_nfy(UINT32 ui4_id)
{
    mpv_frame_param.pf_nfy = mpv_frame_nfy_tbl[ui4_id];
}

VOID mpv_frame_nfy_fct(
    VOID*               pv_nfy_tag,
    VID_DEC_COND_T      e_nfy_cond,
    UINT32              ui4_data_1,
    UINT32              ui4_data_2)
{
    mpv_frame_param.pv_nfy_tag = pv_nfy_tag;
    mpv_frame_param.e_nfy_cond = e_nfy_cond;
    mpv_frame_param.ui4_data_1 = ui4_data_1;
    mpv_frame_param.ui4_data_2 = ui4_data_2;
    _CB_PutEvent_Adpt(CB_DRV_MPV_FRAME_NFY, sizeof(MPV_FRAME_NFY_PARAM), &mpv_frame_param);
}

static x_vid_dec_cc_nfy_fct mpv_dec_cc_nfy = 0;

MPV_DEC_CC_NFY_PARAM mpv_dec_cc_nfy_param;

VOID mpv_cc_nfy_fct (
    VOID*                   pv_tag,
    VID_DEC_CC_COND_T       e_cc_cond,
    VOID*                   pv_arg
    )
{
    mpv_dec_cc_nfy_param.pv_tag = pv_tag;
    mpv_dec_cc_nfy_param.e_cc_cond = e_cc_cond;
    mpv_dec_cc_nfy_param.pv_arg = pv_arg;
    mpv_dec_cc_nfy_param.pf_nfy = mpv_dec_cc_nfy;
    _CB_PutEvent_Adpt(CB_DRV_MPV_DEC_CC_NFY, sizeof(MPV_DEC_CC_NFY_PARAM), &mpv_dec_cc_nfy_param);
}

static MPV_REACH_FCT_NFY_PARAM mpv_reach_nfy_param;

VOID mpv_reach_fct(
    VOID*                       pv_nfy_tag,
    PTS_T                       t_pts,
    BOOL                        b_out_of_sync,
    x_vid_dec_pts_reach_nfy_fct pf_reach_fct_nfy)
{
    mpv_reach_nfy_param.pv_nfy_tag = pv_nfy_tag;
    mpv_reach_nfy_param.t_pts = t_pts;
    mpv_reach_nfy_param.b_out_of_sync = b_out_of_sync;
    mpv_reach_nfy_param.pf_nfy = pf_reach_fct_nfy;
    _CB_PutEvent_Adpt(CB_DRV_MPV_REACH_NFY, sizeof(MPV_REACH_FCT_NFY_PARAM), &mpv_reach_nfy_param);
}
#endif
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
//lint --e{717} do ... while (0);
#define M_GET_VALIDATE(TYPE)             \
    do                                    \
    {                                    \
        if (sizeof(TYPE) > *pz_get_info_len)    \
        {                                \
            *pz_get_info_len = sizeof(TYPE);    \
            return RMR_DRV_NOT_ENOUGH_SPACE;\
        }                                \
        *pz_get_info_len = sizeof(TYPE);        \
    } while (0)

#define M_SET_VALIDATE(TYPE)             \
    do                                    \
    {                                    \
        if (sizeof(TYPE) > z_set_info_len)    \
        {                                \
            return RMR_DRV_INV_SET_INFO;\
        }                                \
    } while (0)

typedef struct _MW_MPV_T
{
    UCHAR            ucEsId;
    UCHAR          ucMode;
    UINT16            u2PcrId;
#ifdef CC_3D_MM_DS_SUPPORT
    BOOL            fgIs3DSubVid;
    BOOL            fgIsRealDEnhance;
    BOOL            fgIsMVCEnhance;
#endif
#ifdef MPV_MW_GET_STATUS
    PARAM_MPV_T     rMpv;
#endif
    BOOL            fgIsNotDisplay;
} MW_MPV_T;

/* add for CR 212789 */
typedef struct _MW_MPV_DISC_PARA_T
{
    DRV_COMP_ID_T    t_comp_id;
    DRV_DISC_TYPE_T  e_disc_type;
    DRV_COMP_ID_T    t_disc_info;
    SIZE_T           z_disc_info_len;
} MW_MPV_DISC_PARA_T;

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

#ifdef ENABLE_MULTIMEDIA
#ifdef CC_53XX_SWDMX_V2
EXTERN VOID MW_DMX_NotifyStepDone(UINT32 u4VdpId, UINT32 u4PTS, UINT32 u4Ret, UINT32 u4SwdmxSrcId);
EXTERN UINT8 _MW_DMX_MM_GetSwdmxSrcId(UINT16 u2CompId);
#else
EXTERN VOID MW_DMX_NotifyStepDone(UINT32 u4VdpId, UINT32 u4PTS, UINT32 u4Ret);
#endif
#endif

#ifdef CC_3D_MM_DS_SUPPORT
EXTERN void MW_VDP_Comp3DInfo(UCHAR ucCompId, BOOL fgFrameSeq);
#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static VID_DEC_NFY_INFO_T _arDecNfyInfo[MPV_COMP_NS];
static MW_MPV_T _arMwMpv[MPV_COMP_NS];

static x_vid_dec_pts_reach_nfy_fct _arPtsCb[MPV_COMP_NS];
static UINT32 _au4PtsTag[MPV_COMP_NS];

static HANDLE_T _hMutex = 0;			// Thread safe protection
static ENUM_VDEC_FMT_T _aeFmt[MPV_COMP_NS];
static UINT32 _u4CompType[MPV_COMP_NS]; //idx by esid

static UINT16 _au2UpStrmCompId[MPV_COMP_NS] = {0}; //idx by esid

static MM_SPEED_TYPE_T _ePreviousSpeedType[VDEC_MAX_ES];

static HANDLE_T _hVDPMWStepMutex = 0;

static BOOL _fgMpvSuperFreeze = FALSE;
static BOOL _fgMpvTimeShiftStatus = FALSE;   //flag if in time-shift mode
static BOOL _fgMpvPlaying = FALSE;
static UINT16 _u2CompId = MPV_COMP_NS;
static BOOL _fgMpvSuperFreezeDisc = FALSE;
static MW_MPV_DISC_PARA_T _rMpvDiscPara;
static INT32 _MpvMwDisconnect(
                            DRV_COMP_ID_T*   pt_comp_id,
                            DRV_DISC_TYPE_T  e_disc_type,
                            const VOID*      pv_disc_info,
                            SIZE_T           z_disc_info_len);

static CHAR _aszStr[][19] =
{
    "MW_VDEC_CTRL_PLAY"
};

#ifdef CC_BOOT_VDEC_LOGO
static BOOL _fgVdecBootLogoRead = FALSE;
static MPV_VDEC_LOGO_CTRL_UNION_T _rVdecBootLogo;
static UINT32 _u4Valid = 0;
static BOOL _fgVdecBootLogoAddrRead = FALSE;
static UINT8   _ucDataType;
static UINT32  _u4DataSize;
static UINT32  _u4DataAddr;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------



void MW_MPV_NOTIFY(UINT16 u2CompId);
#ifdef LINUX_TURNKEY_SOLUTION

static INT32 _MwDmx_data_from_usr_struct(VID_DEC_SEQUENCE_INFO_T* pv_set_info, const VID_DEC_SEQUENCE_INFO_T* prSeqInfo)
{
    if(pv_set_info == NULL || prSeqInfo == NULL)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    x_memcpy(pv_set_info,prSeqInfo,sizeof(VID_DEC_SEQUENCE_INFO_T));

    pv_set_info->u.t_seq_raw.pc_buff =  x_mem_alloc(pv_set_info->u.t_seq_raw.z_buff_size);
    
    if (pv_set_info->u.t_seq_raw.pc_buff== NULL)
    {
        return RMR_NOT_ENOUGH_MEM;
    }
    
    rm_copy_from_user((VOID*)pv_set_info->u.t_seq_raw.pc_buff,(VOID*)prSeqInfo->u.t_seq_raw.pc_buff,prSeqInfo->u.t_seq_raw.z_buff_size);
    return RMR_OK;
}
#endif


static UCHAR _CompIdtoEsId(UINT16 u2CompId)
{
    if(u2CompId >= (UINT16)MPV_COMP_NS)
    {
        LOG(1, "_CompIdtoEsId u2CompId(%d) known\n", u2CompId);
        u2CompId = (UINT16)(MPV_COMP_NS-1);
    }
    
    if ((_arMwMpv[u2CompId].ucEsId == (UCHAR)ES_ID_UNKNOWN) ||
        (_arMwMpv[u2CompId].ucEsId >= (UCHAR)ES_ID_MAX))
    {
        LOG(0, "%s %d, ucEsId(%d) error\n", __FUNCTION__, __LINE__, _arMwMpv[u2CompId].ucEsId);
        return ES0;
    }
    
    return (_arMwMpv[u2CompId].ucEsId);
}


static UINT16 _EsIdtoCompId(UCHAR ucEsId)
{
    UINT16 u2CompId;

    for (u2CompId = 0 ;u2CompId < (UINT16)MPV_COMP_NS; u2CompId++)
    {
        if (_arMwMpv[u2CompId].ucEsId == ucEsId)
        {
            return u2CompId;
        }
    }
    LOG(1, "_EsIdtoCompId ucEsId(%d) known\n", ucEsId);
    return COMP_ID_UNKNOWN;
}

#ifdef ENABLE_MULTIMEDIA
BOOL MPV_MW_SWDMX_SetInfo(UINT8 u1EsId, UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3)
{
#ifdef CC_53XX_SWDMX_V2
    UINT8 u1SrcId;
    if(u1EsId >= MPV_COMP_NS)
    {
        LOG(3, "(u1EsId(%d) >= MPV_COMP_NS)\n", u1EsId);
        return FALSE;        
    }
    if((_u4CompType[u1EsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES)&&(!_fgMpvTimeShiftStatus)) //DTV000316498 handle for time-shift

    {
        return TRUE;
    }

    u1SrcId = _MW_DMX_MM_GetSwdmxSrcId(_au2UpStrmCompId[u1EsId]);
    return SWDMX_SetInfo(u1SrcId, u1Type, u4Para1, u4Para2, u4Para3);	

#else
    UNUSED(u1EsId);
    return SWDMX_SetInfo(u1Type, u4Para1, u4Para2, u4Para3);
#endif
}

BOOL MPV_MW_SWDMX_GetInfo(UINT8 u1EsId, UINT8 u1Type,UINT32 *pu4Para1,UINT32 *pu4Para2,UINT32 *pu4Para3)
{
#ifdef CC_53XX_SWDMX_V2
    UINT8 u1SrcId;
    if(u1EsId >= MPV_COMP_NS)
    {
        LOG(3, "(u1EsId(%d) >= MPV_COMP_NS)\n", u1EsId);
        return FALSE;        
    }
    if((_u4CompType[u1EsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES)&&(!_fgMpvTimeShiftStatus)) //DTV000316498 handle for time-shift

    {
        return TRUE;
    }

    u1SrcId = _MW_DMX_MM_GetSwdmxSrcId(_au2UpStrmCompId[u1EsId]);
    return SWDMX_GetInfo(u1SrcId, u1Type, pu4Para1, pu4Para2, pu4Para3);	

#else
    UNUSED(u1EsId);
    return SWDMX_GetInfo(u1Type, pu4Para1, pu4Para2, pu4Para3);
#endif
}

UCHAR MPV_MW_GetEsId(UINT16 u2CompId)
{
    UINT16 u2CompCnt = 0;

    UNUSED(u2CompCnt);
    
    for (u2CompCnt = 0 ;u2CompCnt < (UINT16)MPV_COMP_NS; u2CompCnt++)
    {
        if (_au2UpStrmCompId[u2CompCnt] == u2CompId)
        {
            LOG(5, "u2CompId = %d, vdec ID = %d\n", u2CompId, u2CompCnt);
            return (UCHAR)u2CompCnt;
        }
    }

    LOG(5, "%s, u2CompId = %d, vdec ID = %d\n", __FUNCTION__, u2CompId, u2CompCnt);
    
    return 0xFF;
}

#endif

#ifdef CC_BOOT_VDEC_LOGO
static ENUM_VDEC_FMT_T _VidCodec2VdecFmt(VID_DEC_CODEC_T eCodecType)
{
    ENUM_VDEC_FMT_T eOrgFmt = VDEC_FMT_MAX;

    switch(eCodecType)
    {
        case VID_DEC_CODEC_MPEG_1:
        case VID_DEC_CODEC_MPEG_2:
            eOrgFmt = VDEC_FMT_MPV;
            break;
        case VID_DEC_CODEC_DIVX_311:
        case VID_DEC_CODEC_DIVX_4:
        case VID_DEC_CODEC_DIVX_5:
        case VID_DEC_CODEC_MPEG_4:
        case VID_DEC_CODEC_XVID:
        case VID_DEC_CODEC_H263:
            eOrgFmt = VDEC_FMT_MP4;
            break;
        case VID_DEC_CODEC_WMV1:
        case VID_DEC_CODEC_WMV2:
        case VID_DEC_CODEC_WMV3:
        case VID_DEC_CODEC_WVC1:
            eOrgFmt = VDEC_FMT_WMV;
            break;
        case VID_DEC_CODEC_H264:
            eOrgFmt = VDEC_FMT_H264;
            break;
        case VID_DEC_CODEC_MJPEG:
            eOrgFmt = VDEC_FMT_MJPEG;
            break;
        case VID_DEC_CODEC_VP8:
			eOrgFmt=  VDEC_FMT_VP8;
			break;
		case VID_DEC_CODEC_VP6:
			eOrgFmt=  VDEC_FMT_VP6;
			break;
        default:
            break;
    }

    return eOrgFmt;
}

static VID_DEC_CODEC_T  _VdecFmt2VidCodec(ENUM_VDEC_FMT_T eFmt)
{
    VID_DEC_CODEC_T eCodecType = VID_DEC_CODEC_UNKNOWN;

    switch(eFmt)
    {
        case VDEC_FMT_MPV:
            eCodecType = VID_DEC_CODEC_MPEG_2;
            break;
        case VDEC_FMT_MP4:
            eCodecType = VID_DEC_CODEC_MPEG_4;
            break;
        case VDEC_FMT_WMV:
            eCodecType = VID_DEC_CODEC_WVC1;
            break;
        case VDEC_FMT_H264:
            eCodecType = VID_DEC_CODEC_H264;
            break;
        case VDEC_FMT_MJPEG:
            eCodecType = VID_DEC_CODEC_MJPEG;
            break;
		case VDEC_FMT_VP8:
            eCodecType = VID_DEC_CODEC_VP8;
			break;
		case VDEC_FMT_VP6:
            eCodecType = VID_DEC_CODEC_VP6;
			break;
        default:
            break;
    }

    return eCodecType;
}
#endif


static void _PtsCbFunc(UINT32 u4VdpId, UINT32 u4Pts, UINT32 u4Arg)
{
    BOOL bOutOfSync = 0;
    UINT16 u2CompId;

    UNUSED(u4VdpId);

    if (u4Arg & VDP_CB_ERR)
    {
        u4Arg &= (~VDP_CB_ERR);
        bOutOfSync = 1;
    }

    u2CompId = _EsIdtoCompId((UCHAR)u4Arg);

    if(u2CompId >= (UINT16)MPV_COMP_NS)
    {
        return;
    }

    VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (_arPtsCb[u2CompId])
    {
#ifndef LINUX_TURNKEY_SOLUTION
        _arPtsCb[u2CompId]((VOID*)_au4PtsTag[u2CompId], (PTS_T)u4Pts, bOutOfSync);
#else
        mpv_reach_fct((VOID*)_au4PtsTag[u2CompId], (PTS_T)u4Pts, bOutOfSync, _arPtsCb[u2CompId]);
#endif
        _arPtsCb[u2CompId] = NULL;
    }

    VERIFY(x_sema_unlock(_hMutex) == OSR_OK);
}


static INT32 _GetCtrl(UINT16 u2CompId, VID_DEC_CTRL_T* peDecCtrl, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;

    ASSERT(peDecCtrl!=NULL);
    ASSERT(pz_get_info_len!=NULL);

    M_GET_VALIDATE(VID_DEC_CTRL_T);

    *peDecCtrl = (VID_DEC_CTRL_T)_arMwMpv[u2CompId].ucMode;

    return i4Ret;
}


static INT32 _GetStc(UCHAR ucEsId, PTS_T* ptPts, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;
    UINT16 u2CompId;
    STC_CLOCK rStcClk;

    ASSERT(ptPts!=NULL);
    ASSERT(pz_get_info_len!=NULL);

    M_GET_VALIDATE(PTS_T);

    u2CompId = _EsIdtoCompId(ucEsId);

    if(u2CompId >= (UINT16)MPV_COMP_NS)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if (STC_GetSrc((UCHAR)_arMwMpv[u2CompId].u2PcrId, &rStcClk) == STC_VALID)
    {
        *ptPts = (PTS_T)(rStcClk.u4Base);
    }

    return i4Ret;
}


static INT32 _GetResolution(UCHAR ucEsId, VID_DEC_RESOLUTION_INFO_T* peDecResolution, SIZE_T* pz_get_info_len)
{
    VDEC_HDR_INFO_T rHdrInfo;

    ASSERT(peDecResolution!=NULL);
    ASSERT(pz_get_info_len!=NULL);

    M_GET_VALIDATE(VID_DEC_RESOLUTION_INFO_T);

    x_memset(&rHdrInfo,0,sizeof(VDEC_HDR_INFO_T));
    VDEC_QueryInfo(ucEsId, &rHdrInfo);

    if(rHdrInfo.fgHdrInvalid)
    {
        return RMR_DRV_INV_GET_INFO;
    }
    else
    {
        peDecResolution->ui2_width = rHdrInfo.u2Width;
        peDecResolution->ui2_height = rHdrInfo.u2Height;
        peDecResolution->b_is_progressive = rHdrInfo.fgProgressiveSeq;

        if(rHdrInfo.fgPixelAspectRatio)
        {
            peDecResolution->b_is_display_aspect_ratio = FALSE;
        }
        else
        {
            peDecResolution->b_is_display_aspect_ratio = TRUE;
        }
        
        if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_4_3)
        {
            //peDecResolution->b_is_display_aspect_ratio = TRUE;
            peDecResolution->e_src_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_4_3;
        }
        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_16_9)
        {
            //peDecResolution->b_is_display_aspect_ratio = TRUE;
            peDecResolution->e_src_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_16_9;
        }
        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_221_1)
        {
            //peDecResolution->b_is_display_aspect_ratio = TRUE;
            peDecResolution->e_src_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_2_21_1;
        }
        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_10_11)
        {
            //peDecResolution->b_is_display_aspect_ratio = FALSE;
            peDecResolution->u4_aspect_ratio_w = 10;
            peDecResolution->u4_aspect_ratio_h = 11;
            peDecResolution->e_src_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_10_11;
        }
        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_40_33)
        {
            //peDecResolution->b_is_display_aspect_ratio = FALSE;
            peDecResolution->u4_aspect_ratio_w = 40;
            peDecResolution->u4_aspect_ratio_h = 33;            
            peDecResolution->e_src_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_40_33;
        }
        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_16_11)
        {
            //peDecResolution->b_is_display_aspect_ratio = FALSE;
            peDecResolution->u4_aspect_ratio_w = 16;
            peDecResolution->u4_aspect_ratio_h = 11;            
            peDecResolution->e_src_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_16_11;
        }
        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_12_11)
        {
            //peDecResolution->b_is_display_aspect_ratio = FALSE;
            peDecResolution->u4_aspect_ratio_w = 12;
            peDecResolution->u4_aspect_ratio_h = 11;            
            peDecResolution->e_src_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_12_11;
        }
        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_3_2)
        {
            //peDecResolution->b_is_display_aspect_ratio = FALSE;
            peDecResolution->u4_aspect_ratio_w = 3;
            peDecResolution->u4_aspect_ratio_h = 2;            
            peDecResolution->e_src_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_3_2;
        }
        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_1_1)
        {
            //peDecResolution->b_is_display_aspect_ratio = FALSE;
            peDecResolution->u4_aspect_ratio_w = 1;
            peDecResolution->u4_aspect_ratio_h = 1;            
            peDecResolution->e_src_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_1_1;
        }
        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_TRANSMIT)
        {
            //peDecResolution->b_is_display_aspect_ratio = FALSE;
            peDecResolution->u4_aspect_ratio_w = rHdrInfo.u4PixelAspectRatioWidth;
            peDecResolution->u4_aspect_ratio_h = rHdrInfo.u4PixelAspectRatioHeight;             
            peDecResolution->e_src_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_USR;
        }
        else
        {
            //peDecResolution->b_is_display_aspect_ratio = TRUE;
            peDecResolution->e_src_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_UNKNOWN;
        }
        peDecResolution->ui2_frame_rate = rHdrInfo.u2FrmRate;
        peDecResolution->ui2_afd = (UINT16)rHdrInfo.ucAfd;
        peDecResolution->b_overscan_not_applied = rHdrInfo.fgNoOverScan;

#ifdef CC_3D_SUPPORT
        switch(u4DrvTDTVGetDecTag3D(E_DEC_MOD_MM))
        {
            case TDTV_TAG3D_2D:
                peDecResolution->e_tag3d_type=VID_DEC_TAG3D_2D;
                break;              
            case TDTV_TAG3D_MVC:
                peDecResolution->e_tag3d_type=VID_DEC_TAG3D_MVC;
                break;  
            case TDTV_TAG3D_FP:
                peDecResolution->e_tag3d_type=VID_DEC_TAG3D_FP;
                break;
            case TDTV_TAG3D_FS:
            //Because LI not defined in MW interface, temporally catogorize LI to FS for AP support
            case TDTV_TAG3D_LI:    
                peDecResolution->e_tag3d_type=VID_DEC_TAG3D_FS;
                break;              
            case TDTV_TAG3D_TB:
                peDecResolution->e_tag3d_type=VID_DEC_TAG3D_TB;
                break;  
            case TDTV_TAG3D_SBS:
                peDecResolution->e_tag3d_type=VID_DEC_TAG3D_SBS;
                break;  
            case TDTV_TAG3D_REALD:
                peDecResolution->e_tag3d_type=VID_DEC_TAG3D_REALD;
                break;              
            case TDTV_TAG3D_SENSIO:
                peDecResolution->e_tag3d_type=VID_DEC_TAG3D_SENSIO;
                break;  
            case TDTV_TAG3D_TTDO:
                peDecResolution->e_tag3d_type=VID_DEC_TAG3D_TTDO;
                break;  
            case TDTV_TAG3D_NOT_SUPPORT:
            default:           
                peDecResolution->e_tag3d_type=VID_DEC_TAG3D_NOT_SUPPORT;
        }
#else
        peDecResolution->e_tag3d_type=VID_DEC_TAG3D_2D;
#endif
        LOG(7, "peDecResolution (%d)\n", peDecResolution->e_src_asp_ratio);
        return RMR_OK;
    }
}


static INT32 _GetCapability(UINT32 u2CompId, VID_DEC_CAPABILITY_INFO_T* pu4DecCapaInfo, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;
    UCHAR ucEsId = _CompIdtoEsId(u2CompId);

    ASSERT(pu4DecCapaInfo!=NULL);
    ASSERT(pz_get_info_len!=NULL);

    M_GET_VALIDATE(VID_DEC_CAPABILITY_INFO_T);

    // timing
    if (u2CompId == (UINT16)MPV_COMP_5)
    {
        pu4DecCapaInfo->ui4_fmt_timing = (VID_DEC_CAP_FMT_1080P | VID_DEC_CAP_FMT_1080I | VID_DEC_CAP_FMT_720P |
                                    VID_DEC_CAP_FMT_576P | VID_DEC_CAP_FMT_576I | VID_DEC_CAP_FMT_480P | VID_DEC_CAP_FMT_480I);
    }
    else
    {
        pu4DecCapaInfo->ui4_fmt_timing = (VID_DEC_CAP_FMT_576P | VID_DEC_CAP_FMT_576I | VID_DEC_CAP_FMT_480P | VID_DEC_CAP_FMT_480I);
    }

    if (MPV_SupportDbk())
    {
        pu4DecCapaInfo->ui4_fmt_timing |= (VID_DEC_CAP_DEBLOCK);
    }

    //codec type
    pu4DecCapaInfo->ui4_codec_type = (VID_DEC_CAP_CODEC_MPEG_1 | VID_DEC_CAP_CODEC_MPEG_2);

    #ifdef ENABLE_MULTIMEDIA
    pu4DecCapaInfo->ui4_codec_type |= (VID_DEC_CAP_CODEC_MPEG_4 | VID_DEC_CAP_CODEC_DIVX_311 |
                                    VID_DEC_CAP_CODEC_DIVX_4 | VID_DEC_CAP_CODEC_DIVX_5 | VID_DEC_CAP_CODEC_XVID |
                                    VID_DEC_CAP_CODEC_MJPEG);
    if(VDEC_ChkCodecCap(ucEsId, VDEC_FMT_WMV))
    {
        pu4DecCapaInfo->ui4_codec_type |= (VID_DEC_CAP_CODEC_WMV1 | VID_DEC_CAP_CODEC_WMV2 |
                                    VID_DEC_CAP_CODEC_WMV3 | VID_DEC_CAP_CODEC_WVC1);
    }
    if(VDEC_ChkCodecCap(ucEsId, VDEC_FMT_VP8))
    {
        pu4DecCapaInfo->ui4_codec_type |= VID_DEC_CAP_CODEC_VP8;
    }

    if(VDEC_ChkCodecCap(ucEsId, VDEC_FMT_VP6))
    {
        pu4DecCapaInfo->ui4_codec_type |= VID_DEC_CAP_CODEC_VP6;
    }
    #endif

    if(VDEC_ChkCodecCap(ucEsId, VDEC_FMT_H264))
    {
        pu4DecCapaInfo->ui4_codec_type |= VID_DEC_CAP_CODEC_H264;
    }

    return i4Ret;
}


static INT32 _GetPcrId(UINT32 u2CompId, UINT16* pu2PcrId, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;

    ASSERT(pu2PcrId!=NULL);
    ASSERT(pz_get_info_len!=NULL);

    M_GET_VALIDATE(UINT16);

    *pu2PcrId = _arMwMpv[u2CompId].u2PcrId;

    return i4Ret;
}


static INT32 _GetCc(UCHAR ucEsId, VID_DEC_CC_DATA* peDecCcData, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;

    ASSERT(pz_get_info_len!=NULL);

    M_GET_VALIDATE(VID_DEC_CC_DATA);

    i4Ret = MPV_GetCc(ucEsId, (MPV_CC_DATA*)(INT32)peDecCcData);

    return i4Ret;
}


static INT32 _GetDbkInfo(UCHAR ucEsId, VID_DEC_DEBLOCKING_INFO_T* prDbkInfo, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;

    ASSERT(pz_get_info_len!=NULL);

    M_GET_VALIDATE(VID_DEC_DEBLOCKING_INFO_T);

    MPV_GetDbkInfo(ucEsId, &prDbkInfo->b_is_on, &prDbkInfo->u.t_region.ui4_x,
        &prDbkInfo->u.t_region.ui4_y, &prDbkInfo->u.t_region.ui4_w, &prDbkInfo->u.t_region.ui4_h);

    return i4Ret;
}

void _GetThumbnailNotify(UINT32 u4CompId, UINT32 u4Condition, UINT32 u4Data1, UINT32 u4Data2)
{
    UINT16 u2CompId = (UINT16)u4CompId;
    UCHAR ucVdpId = (UCHAR)MW_VDP_Comp2Vdp((UCHAR)u2CompId);

    if(u4Condition == (UINT32)VID_DEC_COND_THUMBNAIL_DONE)
    {
        if(_arMwMpv[u2CompId].fgIsNotDisplay)
        {
        }
        else
        {
            VDP_SetPauseMm(ucVdpId, 0);
        }

        if (_arDecNfyInfo[u2CompId].pf_vid_dec_nfy)
        {
            /* not necessary
            #ifdef LINUX_TURNKEY_SOLUTION
            set_mpv_set_nfy(u2CompId);
            set_mpv_frame_nfy(u2CompId);
            #endif
            */
            _arDecNfyInfo[u2CompId].pf_vid_dec_nfy(_arDecNfyInfo[u2CompId].pv_tag, VID_DEC_COND_THUMBNAIL_DONE, 0, 0);
        }
    }
}

static INT32 _GetVideoThumbnail(UINT16 u2CompId, UCHAR ucEsId, VID_DEC_THUMBNAIL_INFO_T* prInfo, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;
    VDP_CAPTURE_INTO_T rCapInfo;
    VDP_THUMBNAIL_INFO_T rDstInfo;
    UCHAR ucVdpId = 0;
    BOOL bIsOnePass;

    ASSERT(pz_get_info_len!=NULL);

    M_GET_VALIDATE(VID_DEC_THUMBNAIL_INFO_T);

    //get ucVdpId by u2CompId
    ucVdpId = (UCHAR)MW_VDP_Comp2Vdp((UCHAR)u2CompId);

    if(_arMwMpv[u2CompId].fgIsNotDisplay)
    {
        VDEC_GetDisplayFrameBufferAddr(ucEsId, &rCapInfo);
    }
    else
    {
        VDP_SetPauseMm(ucVdpId, 1);
        VDP_GetLockFrameBufferAddr(ucVdpId, &rCapInfo);
    }
    if((rCapInfo.u4AddrY == 0) || (rCapInfo.u4AddrC== 0) || (rCapInfo.u4RRMode > 1))
    {
        return RMR_DRV_GET_FAILED;
    }

    bIsOnePass = (prInfo->e_canvas_colormode == VID_DEC_COLORMODE_AYUV_D8888) ? TRUE : FALSE;

    VDEC_RegEventNotifyFunc(_GetThumbnailNotify);

    rDstInfo.eCanvasColormode = (VDP_COLORMODE_T)prInfo->e_canvas_colormode;
    rDstInfo.pucCanvasBuffer = prInfo->pc_canvas_buffer;
    rDstInfo.u4CanvasHeight = prInfo->ui4_canvas_height;
    rDstInfo.u4CanvasPitch = prInfo->ui4_canvas_pitch;
    rDstInfo.u4CanvasWidth = prInfo->ui4_canvas_width;
    rDstInfo.u4ThumbnailHeight = prInfo->ui4_thumbnail_height;
    rDstInfo.u4ThumbnailWidth = prInfo->ui4_thumbnail_width;
    rDstInfo.u4ThumbnailX = prInfo->ui4_thumbnail_x;
    rDstInfo.u4ThumbnailY = prInfo->ui4_thumbnail_y;

    VDEC_GetVideoThumbnail(ucEsId, u2CompId, bIsOnePass, &rDstInfo, &rCapInfo,
        (_arMwMpv[u2CompId].fgIsNotDisplay ? FALSE : TRUE));

    return i4Ret;

}


#ifdef CC_BOOT_VDEC_LOGO
static INT32 _GetBootLogo(VID_DEC_BOOT_LOGO_GET_T* prInfo, SIZE_T* pz_get_info_len)
{
    BOOL fgLogoOn;
    INT32 i4Ret = RMR_OK;
    UINT8 u1Valid;
    UNUSED(pz_get_info_len);

    fgLogoOn = DRVCUST_OptGet(eDrvLogoVdecEnable);
    if(!fgLogoOn)
    {
        return RMR_INV_GET;
    }
    
    if(!_fgVdecBootLogoRead)
    {
        _fgVdecBootLogoRead = TRUE;        
        _rVdecBootLogo.aucReg[0] = bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_CTRL);
        u1Valid = bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_VALID_2);
        _u4Valid = (UINT32)u1Valid;
        _u4Valid <<= 8;
        u1Valid = bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_VALID_1);
        _u4Valid += (UINT32)u1Valid;
    }
    
    prInfo->b_default = (BOOL)_rVdecBootLogo.rField.ucDefault;
    prInfo->b_default_exist = (BOOL)DRVCUST_OptGet(eDrvLogoVdecDefault);
    prInfo->b_onoff = (BOOL)_rVdecBootLogo.rField.ucEnable;
    prInfo->c_data_select = _rVdecBootLogo.rField.ucSelect;
    prInfo->ui4_valid = _u4Valid;
    prInfo->c_slot_num = (UINT8)DRVCUST_OptGet(eDrvLogoVdecFlashSlotNum);
    prInfo->ui4_flash_addr = (UINT32)DRVCUST_OptGet(eDrvLogoVdecFlashAddr);
    prInfo->ui4_flash_size = (UINT32)DRVCUST_OptGet(eDrvLogoVdecFlashSize);
    
    return i4Ret;
}
#endif

#ifdef CC_BOOT_VDEC_LOGO
static INT32 _GetBootLogoAddr(VID_DEC_BOOT_LOGO_ADDR_GET_T* prInfo, SIZE_T* pz_get_info_len)
{
    BOOL fgLogoOn;
    INT32 i4Ret = RMR_OK;
    UNUSED(pz_get_info_len);

    fgLogoOn = DRVCUST_OptGet(eDrvLogoVdecEnable);
    if(!fgLogoOn)
    {
        return RMR_INV_GET;
    }
    
    
    if(!_fgVdecBootLogoAddrRead)
    {
        _fgVdecBootLogoAddrRead = TRUE;
        _ucDataType = bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_TYPE );

        _u4DataSize = (UINT32)bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_SIZE );
        _u4DataSize <<= 8;
        _u4DataSize += (UINT32)bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_SIZE + 1);
        _u4DataSize <<= 8;
        _u4DataSize += (UINT32)bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_SIZE + 2);
        _u4DataSize <<= 8;
        _u4DataSize += (UINT32)bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_SIZE + 3);

        _u4DataAddr = (UINT32)bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_ADDR);
        _u4DataAddr <<= 8;
        _u4DataAddr += (UINT32)bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_ADDR + 1);
        _u4DataAddr <<= 8;
        _u4DataAddr += (UINT32)bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_ADDR + 2);
        _u4DataAddr <<= 8;
        _u4DataAddr += (UINT32)bApiEepromReadByte(EEP_BOOT_VDEC_LOGO_DATA_ADDR + 3);
    }
    
    prInfo->ui4_addr = _u4DataAddr;
    prInfo->ui4_data_size = _u4DataSize;
    prInfo->c_data_type = (UCHAR)_VdecFmt2VidCodec((ENUM_VDEC_FMT_T)_ucDataType);
    
    return i4Ret;
}
#endif

#ifdef MPV_MW_GET_STATUS
static INT32 _GetStatus(UCHAR ucEsId, UINT16 u2CompId, UINT32* pu4Status, SIZE_T* pz_get_info_len)
{
    CRIT_STATE_T rCritState;
    static DRV_DBASE_T* prDbase = NULL;
    PARAM_MPV_T* prMpv = NULL;
    PARAM_MPV_T* prMpvKeep;
    INT32 i4Ret = RMR_OK;

    ASSERT(pz_get_info_len!=NULL);

    M_GET_VALIDATE(UINT32);

    if(prDbase == NULL)
    {
        prDbase = DBS_Lock(&rCritState);
        ASSERT(prDbase);
        VERIFY(DBS_Unlock(prDbase, rCritState));
    }
    
    *pu4Status = VID_DEC_STATUS_NORMAL;
    prMpv = &prDbase->rMpv;
    prMpv += ucEsId;
    prMpvKeep = &(_arMwMpv[u2CompId].rMpv);

    if((prMpv->u4RetrieveNs > 90) &&
       (prMpv->u4RetrieveNs > prMpvKeep->u4RetrieveNs) &&
       (prMpv->u4DecOkNs > 0))
    {
        if((prMpv->u4EsmSkipNs - prMpvKeep->u4EsmSkipNs) &&
           ((((prMpv->u4EsmSkipNs - prMpvKeep->u4EsmSkipNs) * 30) // 2 skips in 2 second
             / (prMpv->u4RetrieveNs - prMpvKeep->u4RetrieveNs)) > 0))
        {
            *pu4Status |= VID_DEC_STATUS_SKIP;
        }
        if((prMpv->u4VpsrDropNs - prMpvKeep->u4VpsrDropNs) &&
           ((((prMpv->u4VpsrDropNs - prMpvKeep->u4VpsrDropNs) * 30) // 2 drops in 2 second
            / (prMpv->u4RetrieveNs - prMpvKeep->u4RetrieveNs)) > 0))
        {
            *pu4Status |= VID_DEC_STATUS_DROP;
        }
        if((prMpv->u4TimeOutNs - prMpvKeep->u4TimeOutNs) &&
           ((((prMpv->u4TimeOutNs - prMpvKeep->u4TimeOutNs) * 60) // 1 timeout in 2 second
            / (prMpv->u4RetrieveNs - prMpvKeep->u4RetrieveNs)) > 0))
        {
            *pu4Status |= VID_DEC_STATUS_TIMEOUT;
        }
    }

    if((prMpv->u4RetrieveNs - prMpvKeep->u4RetrieveNs) > 60)
    {
        x_memcpy(prMpvKeep, prMpv, sizeof(PARAM_MPV_T));
    }

    return i4Ret;
}
#endif

static INT32 _SetCtrl(UINT16 u2CompId, VID_DEC_CTRL_T eDecCtrl, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
    UCHAR ucEsId;
    #ifdef CHANNEL_CHANGE_LOG
    HAL_TIME_T dt;
    #endif

    M_SET_VALIDATE(VID_DEC_CTRL_T);

    if(u2CompId >= (UINT16)MPV_COMP_NS)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    ucEsId = _CompIdtoEsId(u2CompId);

    if ((ucEsId == (UCHAR)ES_ID_UNKNOWN) ||
        (ucEsId >= (UCHAR)ES_ID_MAX))
    {
        ucEsId = 0;
    }

    switch (eDecCtrl)
    {
        case VID_DEC_CTRL_RESET:
            VDEC_Stop(ucEsId);
            _arMwMpv[u2CompId].ucMode = (UCHAR)VID_DEC_CTRL_STOP;
            break;

        case VID_DEC_CTRL_STOP:
            _aeFmt[ucEsId] = VDEC_FMT_MAX;
            if ((_arMwMpv[u2CompId].ucMode == (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME) &&
                (!_arMwMpv[u2CompId].fgIsNotDisplay))
            {
                ucEsId = IFRAME_ES_ID;
            }
            _arMwMpv[u2CompId].fgIsNotDisplay = FALSE;
            if (_arMwMpv[u2CompId].ucMode != (UCHAR)VID_DEC_CTRL_STOP)
            {
                VDEC_Stop(ucEsId);
                // Start -> Pause -> Stop: reset Pause status.
                VDP_SetPauseMm(B2R_1, FALSE);
                // Reset Step Forward status
                if(VDP_GetStepForward(B2R_1) != 0)
                {
                    VDP_SetFBNoTimeout(B2R_1, FALSE);
                    VDP_SetStepForward(B2R_1, VDP_PLAY_NORMAL);
                }
                // Reset Slow Forward status
                if(VDP_GetSlowForward(B2R_1) != 0)
                {
                    VDP_SetFBNoTimeout(B2R_1, FALSE);
                    VDP_SetSlowForward(B2R_1, VDP_PLAY_NORMAL);
                }
                // Reset trick mode status
                #ifdef CC_53XX_SWDMX_V2                
                VDP_SetTrickMode(B2R_1,VDP_PLAY_NORMAL);
                #else
                VDP_SetTrickMode(VDP_PLAY_NORMAL);
                #endif

                _arMwMpv[u2CompId].ucMode = (UCHAR)VID_DEC_CTRL_STOP;
            }
            break;

        case VID_DEC_CTRL_FREEZE:
            VDEC_Pause(ucEsId);
            _arMwMpv[u2CompId].ucMode = (UCHAR)VID_DEC_CTRL_FREEZE;
            break;

        case VID_DEC_CTRL_PLAY:
            #ifdef CHANNEL_CHANGE_LOG
            HAL_GetTime(&dt);
            LOG(0, " %u.%06u s [AV SYNC] 5 MW mpv play\n", dt.u4Seconds, dt.u4Micros);
            #endif

            #ifdef TIME_MEASUREMENT
            if(_arMwMpv[u2CompId].ucMode != (UCHAR)VID_DEC_CTRL_PLAY)
            {
                TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "SET_MPV_PLAY");
            }
            #endif

            /* record mw contrl play time stamp */
            if(_arMwMpv[u2CompId].ucMode != (UCHAR)VID_DEC_CTRL_PLAY)
            {
                DRV_SET_TIMESTAMP(_aszStr[DRIVER_CTRL_VDEC_PLAY]);
            }

            //set MM flag
            if(_u4CompType[ucEsId] == (UINT32)DRVT_DEMUX_CONTAINER_ES)
            {
                VDEC_PlayMM(ucEsId, TRUE);
            }
            else
            {
                VDEC_PlayMM(ucEsId, FALSE);
            }

#ifndef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
            VDEC_Play(ucEsId, _aeFmt[ucEsId]);
#else
            {
                BOOL fgIsBadInterleaved = FALSE;
                if (_u4CompType[ucEsId] == (UINT32)DRVT_DEMUX_CONTAINER_ES)
                {
                    MPV_MW_SWDMX_GetInfo(ucEsId, eSWDMX_GET_BAD_INTERLEAVED_INFO, (UINT32 *)&fgIsBadInterleaved, NULL, NULL);
                }
                if (fgIsBadInterleaved)
                {
                    // do not call VDEC_Play here since SWDMX will call it later
                    LOG(1, "Bad interleave modify, break here\n");
                }
                else
                {
                    VDEC_Play(ucEsId, _aeFmt[ucEsId]);
                }
            }
#endif
            _arMwMpv[u2CompId].ucMode = (UCHAR)VID_DEC_CTRL_PLAY;
            break;

        case VID_DEC_CTRL_PLAY_I_FRAME:
            if (_arMwMpv[u2CompId].ucMode != (UCHAR)VID_DEC_CTRL_STOP)
            {
                return RMR_DRV_SET_FAILED;
            }

            if(!_arMwMpv[u2CompId].fgIsNotDisplay)
            {
                ucEsId = IFRAME_ES_ID;
            }
            MW_VDP_Comp2EsInd((UCHAR)u2CompId, (UCHAR)ucEsId);
            VDEC_Play_I_Frame(ucEsId, _aeFmt[ucEsId]);
            
            _arMwMpv[u2CompId].ucMode = (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME;
            break;

        default:
            LOG(1, "_SetCtrl(): Invalid DecMode\n");
            return RMR_DRV_INV_SET_INFO;
    }

    //stop command will notify until vdec enter stop state
    if ((_arDecNfyInfo[u2CompId].pf_vid_dec_nfy) && (eDecCtrl != VID_DEC_CTRL_STOP))
    {
        /* not necessary
    #ifdef LINUX_TURNKEY_SOLUTION
        set_mpv_set_nfy(u2CompId);
        set_mpv_frame_nfy(u2CompId);
    #endif
        */
        _arDecNfyInfo[u2CompId].pf_vid_dec_nfy(_arDecNfyInfo[u2CompId].pv_tag,
                                    VID_DEC_COND_CTRL_DONE,
                                    (UINT32)eDecCtrl, 0);
    }

    return i4Ret;
}


static INT32 _SetNfyFct(UINT16 u2CompId, const VID_DEC_NFY_INFO_T* ptDecNfyInfo, SIZE_T z_set_info_len)
{
	INT32 i4Ret = RMR_OK;
	UCHAR ucEsId;
#ifdef LINUX_TURNKEY_SOLUTION
    VID_DEC_NFY_INFO_T tDecNfyInfoNew;
#endif

    if(u2CompId >= (UINT16)MPV_COMP_NS)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if(ptDecNfyInfo == NULL)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    M_SET_VALIDATE(VID_DEC_NFY_INFO_T);

#ifdef LINUX_TURNKEY_SOLUTION
    x_memcpy(&tDecNfyInfoNew, ptDecNfyInfo, sizeof(VID_DEC_NFY_INFO_T));
#endif

    _arDecNfyInfo[u2CompId].pv_tag = ptDecNfyInfo->pv_tag;

	ucEsId = _CompIdtoEsId(u2CompId);
    
#ifndef LINUX_TURNKEY_SOLUTION
    _arDecNfyInfo[u2CompId].pf_vid_dec_nfy = ptDecNfyInfo->pf_vid_dec_nfy;
	VDEC_SetDecNfy(ucEsId, (VDEC_DEC_NFY_INFO_T*)ptDecNfyInfo);
#else
    mpv_set_nfy_tbl[u2CompId] = ptDecNfyInfo->pf_vid_dec_nfy;
    set_mpv_set_nfy(u2CompId);
    _arDecNfyInfo[u2CompId].pf_vid_dec_nfy = mpv_nfy_fct;
    tDecNfyInfoNew.pf_vid_dec_nfy = mpv_nfy_fct;
    VDEC_SetDecNfy(ucEsId, (VDEC_DEC_NFY_INFO_T*)(void *)(&tDecNfyInfoNew));
#endif

	return i4Ret;
}


static INT32 _SetNfyFctPts(UINT16 u2CompId, const VID_DEC_NFY_PTS_INFO_T* ptPtsNfyInfo, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
    UCHAR ucEsId;

    if(u2CompId >= (UINT16)MPV_COMP_NS)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if ((ptPtsNfyInfo == NULL) ||
        (z_set_info_len != sizeof (VID_DEC_NFY_PTS_INFO_T)))
    {
        i4Ret = RMR_DRV_INV_SET_INFO;
    }
    else
    {
        VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

         _arPtsCb[u2CompId] = ptPtsNfyInfo->pf_vid_dec_pts_reach_nfy;
        _au4PtsTag[u2CompId] = (UINT32)(ptPtsNfyInfo->pv_tag);

        ucEsId = _CompIdtoEsId(u2CompId);

        if ((ucEsId == (UCHAR)ES_ID_UNKNOWN) ||
            (ucEsId >= (UCHAR)ES_ID_MAX))
        {
            ucEsId = 0;
        }

        VDP_RegCbFunc(VDP_CB_FUNC_PTS_IND, (UINT32)_PtsCbFunc, (UINT32)ucEsId, (UINT32)ptPtsNfyInfo->t_pts);
        i4Ret = RMR_OK;

        VERIFY(x_sema_unlock(_hMutex) == OSR_OK);
    }

    return i4Ret;
}


static INT32 _SetInpMode(UCHAR ucEsId, const VID_DEC_INP_MODE_INFO_T* ptInpInfo, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(VID_DEC_INP_MODE_INFO_T);

    // never used
    //i4Ret = MPV_SetInpNfyFct(ucEsId, (MPV_INP_NFY_INFO_T*)(void*)ptInpInfo);

    return i4Ret;
}


#ifndef LINUX_TURNKEY_SOLUTION
static INT32 _SetPlayCc(UCHAR ucEsId, const VID_DEC_CC_NFY_INFO_T* ptCcNfyInfo, SIZE_T z_set_info_len)
#else
static INT32 _SetPlayCc(UCHAR ucEsId, VID_DEC_CC_NFY_INFO_T* ptCcNfyInfo, SIZE_T z_set_info_len)
#endif
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(VID_DEC_CC_NFY_INFO_T);

#ifdef LINUX_TURNKEY_SOLUTION
    mpv_dec_cc_nfy = ptCcNfyInfo->pf_cc_nfy;
    ptCcNfyInfo->pf_cc_nfy = mpv_cc_nfy_fct;
#endif
    i4Ret = MPV_SetCcNfyFct(ucEsId, (MPV_CC_NFY_INFO_T*)(void*)ptCcNfyInfo);

    MPV_PlayCc(ucEsId);

    return i4Ret;
}


static INT32 _SetStopCc(UCHAR ucEsId)
{
    INT32 i4Ret = RMR_OK;

    MPV_StopCc(ucEsId);

    return i4Ret;
}


static INT32 _SetPcrId(UCHAR ucEsId, UINT16 u2PcrId, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
    UINT16 u2CompId;

    M_SET_VALIDATE(UINT16);

    u2CompId = _EsIdtoCompId(ucEsId);

    if(u2CompId >= (UINT16)MPV_COMP_NS)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }
    _arMwMpv[u2CompId].u2PcrId = u2PcrId;

    //VDEC_SyncStc(ucEsId, VID_SYNC_MODE_SLAVE, u2PcrId);
    /* PCR ID is mapped to STC_SRC_A1 */
    VDEC_SyncStc(ucEsId, VID_SYNC_MODE_SLAVE, STC_SRC_A1);

    return i4Ret;
}


static INT32 _SetCapture(UCHAR ucEsId, const VID_DEC_CAPTURE_INFO_T* ptCaptuerInfo, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

   ASSERT(ptCaptuerInfo != NULL);
    M_SET_VALIDATE(VID_DEC_CAPTURE_INFO_T);
    UNUSED(ucEsId);

    return i4Ret;
}


static INT32 _SetAllocCcBuf(UCHAR ucEsId, SIZE_T zCcBufSize, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(SIZE_T);

    i4Ret = MPV_AllocCcBuf(ucEsId, (UINT32)zCcBufSize);

    return i4Ret;
}


static INT32 _SetFreeCcBuf(UCHAR ucEsId)
{
    INT32 i4Ret = RMR_OK;

    i4Ret = MPV_FreeCcBuf(ucEsId);

    return i4Ret;
}


static INT32 _SetFlushCcBuf(UCHAR ucEsId)
{
    INT32 i4Ret = RMR_OK;

    i4Ret = MPV_FlushCcBuf(ucEsId);

    return i4Ret;
}


static INT32 _SetIFrameNfyFct(UINT16 u2CompId, const VID_DEC_NFY_INFO_T* ptDecNfyInfo, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    UCHAR ucEsId;
    
#ifdef LINUX_TURNKEY_SOLUTION
    VID_DEC_NFY_INFO_T tDecNfyInfoNew;
#endif

    ucEsId = _CompIdtoEsId(u2CompId);

    if ((ucEsId == (UCHAR)ES_ID_UNKNOWN) ||
        (ucEsId >= (UCHAR)ES_ID_MAX))
    {
        ucEsId = 0;
    }

    if(u2CompId >= (UINT16)MPV_COMP_NS)
    {
        LOG(0, "%s, incorrect u2CompId(%d)\n", __FUNCTION__, u2CompId);
        return (RMR_DRV_INV_CONN_INFO);
    }

    ASSERT(ptDecNfyInfo != NULL);
    M_SET_VALIDATE(VID_DEC_NFY_INFO_T);
    
#ifdef LINUX_TURNKEY_SOLUTION
    x_memcpy(&tDecNfyInfoNew, ptDecNfyInfo, sizeof(VID_DEC_NFY_INFO_T));
#endif

    _arDecNfyInfo[u2CompId].pv_tag = ptDecNfyInfo->pv_tag;

    if(!_arMwMpv[u2CompId].fgIsNotDisplay)
    {
        ucEsId = IFRAME_ES_ID;
    }
#ifndef LINUX_TURNKEY_SOLUTION
    _arDecNfyInfo[u2CompId].pf_vid_dec_nfy = ptDecNfyInfo->pf_vid_dec_nfy;
    i4Ret = VDEC_SetDecNfy(ucEsId, (VDEC_DEC_NFY_INFO_T*)(void*)ptDecNfyInfo);
#else
    mpv_frame_nfy_tbl[u2CompId] = ptDecNfyInfo->pf_vid_dec_nfy;
    set_mpv_frame_nfy(u2CompId);
    _arDecNfyInfo[u2CompId].pf_vid_dec_nfy = mpv_frame_nfy_fct;
    tDecNfyInfoNew.pf_vid_dec_nfy = mpv_frame_nfy_fct;
    i4Ret = VDEC_SetDecNfy(ucEsId, (VDEC_DEC_NFY_INFO_T*)(void*)(&tDecNfyInfoNew));
#endif

    return i4Ret;
}

static VOID _IframeKnl2Usr(VID_DEC_I_FRAME_BUFF_T* ptIFrameBuff)
{
    #ifdef LINUX_TURNKEY_SOLUTION
    if(ptIFrameBuff)
    {
        if((ptIFrameBuff->pc_frame_buff_sp) &&
           (adapt_fbm_is_fbm_knl_vir((UINT32)ptIFrameBuff->pc_frame_buff_sp)))
        {
            ptIFrameBuff->pc_frame_buff_sp = (UCHAR*)adapt_fbm_knl_to_usr((UINT32)ptIFrameBuff->pc_frame_buff_sp);
        }
    }
    #endif
    UNUSED(ptIFrameBuff);
}

static VOID _IframeUsr2Knl(VID_DEC_I_FRAME_BUFF_T* ptIFrameBuff)
{
    #ifdef LINUX_TURNKEY_SOLUTION
    if(ptIFrameBuff)
    {
        if((ptIFrameBuff->pc_frame_buff_sp) &&
           (adapt_fbm_is_fbm_usr_vir((UINT32)ptIFrameBuff->pc_frame_buff_sp)))
        {
            ptIFrameBuff->pc_frame_buff_sp = (UCHAR*)adapt_fbm_usr_to_knl((UINT32)ptIFrameBuff->pc_frame_buff_sp);
        }
    }
    #endif
    UNUSED(ptIFrameBuff);
}

static INT32 _SetAllocIFrameBuf(UCHAR ucEsId, const VID_DEC_I_FRAME_BUFF_T* ptIFrameBuff, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(VID_DEC_I_FRAME_BUFF_T);

    i4Ret = MPV_AllocIFrameBuf(ucEsId, (MPV_DEC_I_FRAME_BUFF_T*)(void*)ptIFrameBuff);

    return i4Ret;
}


static INT32 _SetFreeIFrameBuf(UCHAR ucEsId)
{
    INT32 i4Ret = RMR_OK;

    i4Ret = MPV_FreeIFrameBuf(ucEsId);

    return i4Ret;
}


static INT32 _SetIFrameBuf(UCHAR ucEsId, const VID_DEC_I_FRAME_BUFF_T* ptIFrameBuff, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(VID_DEC_I_FRAME_BUFF_T);

    i4Ret = MPV_SetIFrameBuf(ucEsId, (MPV_DEC_I_FRAME_BUFF_T*)(void*)ptIFrameBuff);

    return i4Ret;
}


static INT32 _SetIFrameDec(UCHAR ucEsId, const VID_DEC_I_FRAME_BUFF_T* ptIFrameBuff, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(VID_DEC_I_FRAME_BUFF_T);

    i4Ret = MPV_SetIFrameDec(ucEsId, (MPV_DEC_I_FRAME_BUFF_T*)(void*)ptIFrameBuff);

    return i4Ret;
}


static INT32 _SetDbkInfo(UCHAR ucEsId, const VID_DEC_DEBLOCKING_INFO_T* ptDbkInfo, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(VID_DEC_DEBLOCKING_INFO_T);

    MPV_SetDbkInfo(ucEsId, ptDbkInfo->b_is_on, ptDbkInfo->u.t_region.ui4_x,
        ptDbkInfo->u.t_region.ui4_y, ptDbkInfo->u.t_region.ui4_w, ptDbkInfo->u.t_region.ui4_h);

    return i4Ret;
}


static INT32 _SetVideoType(UCHAR ucEsId, VID_DEC_CODEC_T eCodecType, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
    ENUM_VDEC_FMT_T eOrgFmt = VDEC_FMT_MAX;
    UINT16 u2CompId;

    M_SET_VALIDATE(VID_DEC_CODEC_T);

    if(_fgMpvSuperFreeze)
    {
        eOrgFmt = _aeFmt[ucEsId];
    }

    u2CompId = _EsIdtoCompId(ucEsId);

    if(u2CompId >= (UINT16)MPV_COMP_NS)
    {
        LOG(0, "%s, incorrect u2CompId(%d)\n", __FUNCTION__, u2CompId);
        u2CompId =  (MPV_COMP_1);
    }

    switch(eCodecType)
    {
        case VID_DEC_CODEC_MPEG_1:
        case VID_DEC_CODEC_MPEG_2:
            // Currently, MW does not recognize MPEG2 or MPEG1 video.
            // MPV driver will do this notification.
            /*if(eCodecType == VID_DEC_CODEC_MPEG_2)
            {
                if(!DRVCUST_OpCodeQuery(eIsSupportMP2))
                {
                    i4Ret = RMR_DRV_INV_SET_INFO;
                    break;
                }
            }*/
            _aeFmt[ucEsId] = VDEC_FMT_MPV;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_MPEG);
            }
            #ifdef ENABLE_MULTIMEDIA
            MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_MPEG1_MPEG2,0,0);
            #endif
            break;

        case VID_DEC_CODEC_DIVX_311:
            if(!VDEC_ChkDivXBonding())
            {
                i4Ret = RMR_DRV_INV_SET_INFO;
                break;
            }
            if(!DRVCUST_OpCodeQuery(eIsSupportDivX))
            {
                i4Ret = RMR_DRV_INV_SET_INFO;
                break;
            }
            _aeFmt[ucEsId] = VDEC_FMT_MP4;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_MPEG4);
            }
            #ifdef ENABLE_MULTIMEDIA
            MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_DX311,0,0);
            #endif
            break;
        case VID_DEC_CODEC_DIVX_4:
			VDEC_SetMMParam(ucEsId, VDEC_MM_DIVX4_SIGNAL, 0, 0, 0);
        case VID_DEC_CODEC_DIVX_5:
            if(!VDEC_ChkDivXBonding())
            {
                i4Ret = RMR_DRV_INV_SET_INFO;
                break;
            }
        case VID_DEC_CODEC_MPEG_4:
        case VID_DEC_CODEC_XVID:
            if(eCodecType == VID_DEC_CODEC_MPEG_4)
            {
                if(!DRVCUST_OpCodeQuery(eIsSupportMPEG4))
                {
                    i4Ret = RMR_DRV_INV_SET_INFO;
                    break;
                }
            }
            else // DIVX and XVID
            {
                if(!DRVCUST_OpCodeQuery(eIsSupportDivX))
                {
                    i4Ret = RMR_DRV_INV_SET_INFO;
                    break;
                }
            }
            _aeFmt[ucEsId] = VDEC_FMT_MP4;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_MPEG4);
            }
            #ifdef ENABLE_MULTIMEDIA
            MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_MPEG4,0,0);
            #endif
            break;
        case VID_DEC_CODEC_H263:
            if(eCodecType == VID_DEC_CODEC_MPEG_4)
            {
                if(!DRVCUST_OpCodeQuery(eIsSupportMPEG4))
                {
                    i4Ret = RMR_DRV_INV_SET_INFO;
                    break;
                }
            }
            _aeFmt[ucEsId] = VDEC_FMT_MP4;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_MPEG4);
            }
            #ifdef ENABLE_MULTIMEDIA
            MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE, ENUM_SWDMX_VDEC_H263, 0, 0);
            #endif
            break;
        case VID_DEC_CODEC_WMV1:
            if(!VDEC_ChkCodecCap(ucEsId, VDEC_FMT_WMV))
            {
                i4Ret = RMR_DRV_INV_SET_INFO;
                break;
            }
            _aeFmt[ucEsId] = VDEC_FMT_WMV;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_WMV7);
            }
            #ifdef ENABLE_MULTIMEDIA
            MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_WMV7,0,0);
            #endif
            break;
        case VID_DEC_CODEC_WMV2:
            if(!VDEC_ChkCodecCap(ucEsId, VDEC_FMT_WMV))
            {
                i4Ret = RMR_DRV_INV_SET_INFO;
                break;
            }
            _aeFmt[ucEsId] = VDEC_FMT_WMV;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_WMV8);
            }
            #ifdef ENABLE_MULTIMEDIA
            MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_WMV8,0,0);
            #endif
            break;
        case VID_DEC_CODEC_WMV3:
            if(!VDEC_ChkCodecCap(ucEsId, VDEC_FMT_WMV))
            {
                i4Ret = RMR_DRV_INV_SET_INFO;
                break;
            }
            _aeFmt[ucEsId] = VDEC_FMT_WMV;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_WMV9);
            }
            #ifdef ENABLE_MULTIMEDIA
            #ifdef CC_3D_MM_DS_SUPPORT
            if(_arMwMpv[u2CompId].fgIs3DSubVid)
            {
                MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_SUBVIDEOTYPE,ENUM_SWDMX_VDEC_MULTITRACK,0,0);
                MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VDEC2_ID,(UINT32)ucEsId,0,0);
            }
            else
            #endif
            {
                MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_WMV9,0,0);
            }
            #endif
            break;
        case VID_DEC_CODEC_WVC1:
            if(!VDEC_ChkCodecCap(ucEsId, VDEC_FMT_WMV))
            {
                i4Ret = RMR_DRV_INV_SET_INFO;
                break;
            }
            _aeFmt[ucEsId] = VDEC_FMT_WMV;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_VC1);
            }
            #ifdef ENABLE_MULTIMEDIA
            #ifdef CC_3D_MM_DS_SUPPORT
            if(_arMwMpv[u2CompId].fgIs3DSubVid)
            {
                MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_SUBVIDEOTYPE,ENUM_SWDMX_VDEC_MULTITRACK,0,0);
                MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VDEC2_ID,(UINT32)ucEsId,0,0);
            }
            else
            #endif
            {
                MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_VC1,0,0);
            }
            #endif
            break;

        case VID_DEC_CODEC_H264:
            #ifdef CC_VDEC_FMT_DETECT
            //check bonding here if mmplay, else notify after format detection
            if(_u4CompType[ucEsId] == (UINT32)DRVT_DEMUX_CONTAINER_ES)
            {
            #endif
                if(!VDEC_ChkCodecCap(ucEsId, VDEC_FMT_H264))
                {
                    #ifdef FAKE_FMT_HANDLE
                        _aeFmt[ucEsId] = VDEC_FMT_MPV;
                        #ifdef CC_53XX_SWDMX_V2
                        if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
                           (!_arMwMpv[u2CompId].fgIsNotDisplay))
                        #endif
                        {
                            DMX_SetVideoType(DMX_VIDEO_MPEG);
                        }
                        #ifdef ENABLE_MULTIMEDIA
                        MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_MPEG1_MPEG2,0,0);
                        #endif
                    #else
                        i4Ret = RMR_DRV_INV_SET_INFO;
                    #endif

                    break;
                }
            #ifdef CC_VDEC_FMT_DETECT
            }
            #endif
            _aeFmt[ucEsId] = VDEC_FMT_H264;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_H264);
            }
            #ifdef ENABLE_MULTIMEDIA
            #ifdef CC_3D_MM_DS_SUPPORT
            if(_arMwMpv[u2CompId].fgIs3DSubVid)
            {
                if(_arMwMpv[u2CompId].fgIsMVCEnhance)
                {
                    VDEC_SetMMParam(0, VDEC_MM_3D_INFO, VDEC_3D_MVC, 0, 0);
                    VDEC_SetMMParam(ucEsId, VDEC_MM_3D_INFO, VDEC_3D_MVC, 0, 0);

                    MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_SUBVIDEOTYPE,ENUM_SWDMX_VDEC_MVC,0,0);
                }
                else if(_arMwMpv[u2CompId].fgIsRealDEnhance)
                {
                    VDEC_SetMMParam(0, VDEC_MM_3D_INFO, VDEC_3D_REALD_DUAL_STREAM, 0, 0);
                    VDEC_SetMMParam(ucEsId, VDEC_MM_3D_INFO, VDEC_3D_REALD_DUAL_STREAM, 0, 0);

                    MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_SUBVIDEOTYPE,ENUM_SWDMX_VDEC_MULTITRACK,0,0);
                }
                MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VDEC2_ID,(UINT32)ucEsId,0,0);
            }
            else
            #endif
            {
                MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_H264,0,0);
            }
            #endif
            break;

        case VID_DEC_CODEC_MJPEG:
            if(!VDEC_ChkCodecCap(ucEsId, VDEC_FMT_MJPEG))
            {
                i4Ret = RMR_DRV_INV_SET_INFO;
                break;
            }
            _aeFmt[ucEsId] = VDEC_FMT_MJPEG;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_MPEG4);
            }
            #ifdef ENABLE_MULTIMEDIA
            #ifdef CC_3D_MM_DS_SUPPORT
            if(_arMwMpv[u2CompId].fgIs3DSubVid)
            {
            MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_MJPEG,0,0);
                MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VDEC2_ID,(UINT32)ucEsId,0,0);
            }
             else
           #endif
            {
                MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_MJPEG,0,0);
            }           
            #endif
            break;

#if defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        case VID_DEC_CODEC_RV8:
        case VID_DEC_CODEC_RV9:
            if(!VDEC_ChkCodecCap(ucEsId, VDEC_FMT_RV))
            {
                i4Ret = RMR_DRV_INV_SET_INFO;
                break;
            }
            _aeFmt[ucEsId] = VDEC_FMT_RV;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_RV);
            }
#ifdef ENABLE_MULTIMEDIA
            MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_RV,0,0);
#endif
            break;
#endif

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        case VID_DEC_CODEC_NV12:
            _aeFmt[ucEsId] = VDEC_FMT_RAW;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_MPEG4);
            }
#ifdef ENABLE_MULTIMEDIA
            MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_RAW_NV12,0,0);
#endif
            break;
#endif
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        case VID_DEC_CODEC_AVS:
            _aeFmt[ucEsId] = VDEC_FMT_AVS;
            #ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
            #endif
            {
                DMX_SetVideoType(DMX_VIDEO_AVS);
            }
            #ifdef ENABLE_MULTIMEDIA
            MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_AVS,0,0);
            #endif

            break;
#endif

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) 
        case VID_DEC_CODEC_VP6:
            if(!VDEC_ChkCodecCap(ucEsId, VDEC_FMT_VP6))
            {
                i4Ret = RMR_DRV_INV_SET_INFO;
                break;
            }
            
            _aeFmt[ucEsId] = VDEC_FMT_VP6;
#ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
#endif
            {
                DMX_SetVideoType(DMX_VIDEO_VP6);
            }
#ifdef ENABLE_MULTIMEDIA
            MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_VP6,0,0);
#endif
            break;

        case VID_DEC_CODEC_VP8:
            if(!VDEC_ChkCodecCap(ucEsId, VDEC_FMT_VP8))
            {
                i4Ret = RMR_DRV_INV_SET_INFO;
                break;
            }
            
            _aeFmt[ucEsId] = VDEC_FMT_VP8;
#ifdef CC_53XX_SWDMX_V2
            if((_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES) &&
               (!_arMwMpv[u2CompId].fgIsNotDisplay))
#endif
            {
                DMX_SetVideoType(DMX_VIDEO_VP8);
            }
#ifdef ENABLE_MULTIMEDIA
            MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDEOTYPE,ENUM_SWDMX_VDEC_VP8,0,0);
#endif
            break;
#endif

        default:
            i4Ret = RMR_DRV_INV_SET_INFO;
            break;
    }

    if(_fgMpvSuperFreeze)
    {
        if(eOrgFmt != _aeFmt[ucEsId])
        {
            LOG(0, "Change format %d -> %d\n", (UINT32)eOrgFmt, (UINT32)_aeFmt[ucEsId]);
            VDEC_Stop(ucEsId);
            if(!VDEC_Play(ucEsId, _aeFmt[ucEsId]))
            {
                LOG(0, "VDEC Play fail %d\n", (UINT32)_aeFmt[ucEsId]);
                i4Ret = RMR_DRV_INV_SET_INFO;
            }
            VDEC_Pause(ucEsId);
            if(u2CompId < (UINT16)MPV_COMP_NS)
            {
                _arMwMpv[u2CompId].ucMode = (UCHAR)VID_DEC_CTRL_STOP;
            }
            else
            {
                LOG(0, "u2CompId unknow %d\n", u2CompId);
            }
        }
    }

    return i4Ret;
}

static STC_SPEED_TYPE_T _Speed_convert(MM_SPEED_TYPE_T e_mm_spd)
{

    switch (e_mm_spd)
    {
        case MM_SPEED_TYPE_FORWARD_1X:          /* Forward 1X */
            return STC_SPEED_TYPE_FORWARD_1X;
        case MM_SPEED_TYPE_FORWARD_2X:          /* Fast Forward 2X */
            return STC_SPEED_TYPE_FORWARD_2X;
        case MM_SPEED_TYPE_FORWARD_3X:          /* Fast Forward 3X */
            return STC_SPEED_TYPE_FORWARD_3X;
        case MM_SPEED_TYPE_FORWARD_4X:          /* Fast Forward 4X */
            return STC_SPEED_TYPE_FORWARD_4X;
        case MM_SPEED_TYPE_FORWARD_8X:          /* Fast Forward 8X */
            return STC_SPEED_TYPE_FORWARD_8X;
        case MM_SPEED_TYPE_FORWARD_16X:         /* Fast Forward 16X */
            return STC_SPEED_TYPE_FORWARD_16X;
        case MM_SPEED_TYPE_FORWARD_32X:        /* Fast Forward 32X */
            return STC_SPEED_TYPE_FORWARD_32X;
        case MM_SPEED_TYPE_FORWARD_64X:        /* Fast Forward 64X */
            return STC_SPEED_TYPE_FORWARD_64X;
        case MM_SPEED_TYPE_FORWARD_128X:        /* Fast Forward 128X */
            return STC_SPEED_TYPE_FORWARD_128X;
        case MM_SPEED_TYPE_FORWARD_256X:        /* Fast Forward 256X */
            return STC_SPEED_TYPE_FORWARD_256X;
        case MM_SPEED_TYPE_FORWARD_512X:        /* Fast Forward 512X */
            return STC_SPEED_TYPE_FORWARD_512X;
        case MM_SPEED_TYPE_FORWARD_1024X:        /* Fast Forward 1024X */
            return STC_SPEED_TYPE_FORWARD_1024X;
        case MM_SPEED_TYPE_REWIND_1X:          /* Rewind 1X */
            return STC_SPEED_TYPE_REWIND_1X;
        case MM_SPEED_TYPE_REWIND_2X:          /* Fast Rewind 2X */
            return STC_SPEED_TYPE_REWIND_2X;
        case MM_SPEED_TYPE_REWIND_3X:          /* Fast Rewind 3X */
            return STC_SPEED_TYPE_REWIND_3X;
        case MM_SPEED_TYPE_REWIND_4X:          /* Fast Rewind 4X */
            return STC_SPEED_TYPE_REWIND_4X;
        case MM_SPEED_TYPE_REWIND_8X:          /* Fast Rewind 8X */
            return STC_SPEED_TYPE_REWIND_8X;
        case MM_SPEED_TYPE_REWIND_16X:         /* Fast Rewind 16X */
            return STC_SPEED_TYPE_REWIND_16X;
        case MM_SPEED_TYPE_REWIND_32X:        /* Fast Rewind 32X */
            return STC_SPEED_TYPE_REWIND_32X;
        case MM_SPEED_TYPE_REWIND_64X:        /* Fast Rewind 64X */
            return STC_SPEED_TYPE_REWIND_64X;
        case MM_SPEED_TYPE_REWIND_128X:        /* Fast Rewind 128X */
            return STC_SPEED_TYPE_REWIND_128X;
        case MM_SPEED_TYPE_REWIND_256X:        /* Fast Rewind 256X */
            return STC_SPEED_TYPE_REWIND_256X;
        case MM_SPEED_TYPE_REWIND_512X:        /* Fast Rewind 512X */
            return STC_SPEED_TYPE_REWIND_512X;
        case MM_SPEED_TYPE_REWIND_1024X:        /* Fast Rewind 1024X */
            return STC_SPEED_TYPE_REWIND_1024X;
        case MM_SPEED_TYPE_FORWARD_1_DIV_2X:     /* Slow Rewind 1/2X */
            return STC_SPEED_TYPE_FORWARD_1_DIV_2X;
        case MM_SPEED_TYPE_FORWARD_1_DIV_3X:     /* Slow Rewind 1/3X */
            return STC_SPEED_TYPE_FORWARD_1_DIV_3X;
        case MM_SPEED_TYPE_FORWARD_1_DIV_4X:        /* Slow Rewind 1/4X */
            return STC_SPEED_TYPE_FORWARD_1_DIV_4X;
        case MM_SPEED_TYPE_FORWARD_1_DIV_8X:        /* Slow Rewind 1/8X */
            return STC_SPEED_TYPE_FORWARD_1_DIV_8X;
        case MM_SPEED_TYPE_FORWARD_1_DIV_16X:        /* Slow Rewind 1/16X */
            return STC_SPEED_TYPE_FORWARD_1_DIV_16X;
        case MM_SPEED_TYPE_FORWARD_1_DIV_32X:        /* Slow Rewind 1/32X */
            return STC_SPEED_TYPE_FORWARD_1_DIV_32X;
        default:
		    break;
    }
    return  STC_SPEED_TYPE_FORWARD_1X;
}

#ifdef CC_53XX_SWDMX_V2
static VOID _VDP_MW_StepFinNotify(UINT32 u4VdpId, UINT32 u4PTS, UINT32 u4Ret, UINT32 u4AttachedId)
#else
static VOID _VDP_MW_StepFinNotify(UINT32 u4VdpId, UINT32 u4PTS, UINT32 u4Ret)
#endif
{
    UNUSED(u4VdpId);
    UNUSED(u4PTS);
    LOG(0, "_VDP_MW_StepFinNotify\n");
    // block mode
    //VERIFY (x_sema_unlock(_hVDPMWStepMutex) == OSR_OK);
    // notify mw that we finish step.

    if((_fgMpvSuperFreeze) &&
       (_fgMpvPlaying))
    {
        if (_u2CompId >= (UINT16)MPV_COMP_NS)
        {
            LOG(1, "_VDP_MW_StepFinNotify(): error component id [%d]\n", _u2CompId);
        }
        else
        {
            //vdp should bypass _arMwVdp[ucVdpId].u2SrcCompId to us.
            MW_MPV_NOTIFY(_u2CompId);
        }
    }

#ifdef ENABLE_MULTIMEDIA
#ifndef CC_53XX_SWDMX_V2
    MW_DMX_NotifyStepDone(u4VdpId, u4PTS, u4Ret);
#else
    if (u4AttachedId <MPV_COMP_NS)
    {
        UINT8 u1SrcId = _MW_DMX_MM_GetSwdmxSrcId(_au2UpStrmCompId[u4AttachedId]);
        MW_DMX_NotifyStepDone(u4VdpId, u4PTS, u4Ret, u1SrcId);
    }
    else
    {
        LOG(1, "_VDP_MW_StepFinNotify(): error u4AttachedId id [%d]\n", u4AttachedId);
    }
#endif
#endif
}

static INT32 _SetSpeedType(UCHAR ucEsId, MM_SPEED_TYPE_T eSpeedType, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(MM_SPEED_TYPE_T);

    if(eSpeedType == MM_SPEED_TYPE_PAUSE)
    {
        VDEC_Pause(ucEsId);
        VDP_SetFBNoTimeout(B2R_1, TRUE);
        VDP_SetPauseMm(B2R_1, TRUE);
    }
    else if(eSpeedType == MM_SPEED_TYPE_FORWARD_1X)
    {
        if(VDP_GetStepForward(B2R_1) != 0)
        {
            VDP_SetFBNoTimeout(B2R_1, FALSE);
            VDP_SetStepForward(B2R_1, VDP_PLAY_NORMAL);
            VDP_SetPauseMm(B2R_1, FALSE);
        }
        if(VDP_GetSlowForward(B2R_1) != 0)
        {
            VDP_SetFBNoTimeout(B2R_1, FALSE);
            VDP_SetSlowForward(B2R_1, VDP_PLAY_NORMAL);
        }

        if(_ePreviousSpeedType[ucEsId] == MM_SPEED_TYPE_PAUSE || _ePreviousSpeedType[ucEsId] == MM_SPEED_TYPE_STEP)
        {
            VDP_SetPauseMm(0, FALSE);
            VDEC_Play(ucEsId, _aeFmt[ucEsId]);
        }

        VDEC_SetMMParam(ucEsId, VDEC_MM_PLAY_SPEED, _Speed_convert(eSpeedType), 0, 0);

        VDP_SetSpeed(0,_Speed_convert(eSpeedType));
        /* Trick Play at 20081001 */
        #ifdef CC_53XX_SWDMX_V2   
        VDP_SetTrickMode(B2R_1,VDP_PLAY_NORMAL);
        #else
        VDP_SetTrickMode(VDP_PLAY_NORMAL);
        #endif
    }
    else if((eSpeedType == MM_SPEED_TYPE_STEP) ||
        (eSpeedType == MM_SPEED_TYPE_STEP_REVERSE))
    {
        LOG(0, "-- MM_SPEED_TYPE_STEP\n");

        /* VDEC play for new frame decoding */
        VDEC_Play(ucEsId, _aeFmt[ucEsId]);
        VDEC_SetMMParam(ucEsId, VDEC_MM_PLAY_SPEED, _Speed_convert(MM_SPEED_TYPE_FORWARD_1X), 0, 0);
        VDP_SetSpeed(0,_Speed_convert(MM_SPEED_TYPE_FORWARD_1X));

        /* Pause VDP and just display 1 frame */
        VDP_SetFBNoTimeout(B2R_1, TRUE);
        VDP_SetPauseMm(B2R_1, TRUE);
        VDP_SetStepForward(B2R_1, VDP_PLAY_STEP_FORWARD);

        /* VDP will play one frame and then Pause if VDP_PLAY_STEP_FORWARD is set */
        VDP_SetPauseMm(B2R_1, FALSE);
#if 1
        VDP_SetCbAttachedSrcId(ucEsId, ucEsId);
        VDP_RegCbFunc(VDP_CB_FUNC_MM_STEP_FIN_IND, (UINT32)_VDP_MW_StepFinNotify, 0, 0);
        // block
        LOG(0, "-- MM_SPEED_TYPE_STEP lock\n");
        // change to notify MW directly, please see _VDP_MW_StepFinNotify
        //VERIFY (x_sema_lock(_hVDPMWStepMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif
    }
    else if((eSpeedType >= MM_SPEED_TYPE_FORWARD_1_DIV_32X) &&
            (eSpeedType <= MM_SPEED_TYPE_FORWARD_1_DIV_2X))
    {   
         // DTV0079404   move this code down
        //if(_ePreviousSpeedType == MM_SPEED_TYPE_PAUSE || _ePreviousSpeedType == MM_SPEED_TYPE_STEP)
        //{
        //    VDP_SetPauseMm(0, FALSE);
        //    VDEC_Play(ucEsId, _aeFmt[ucEsId]);
        //}

        VDP_SetTrickMode(B2R_1,VDP_PLAY_TRICK);
		
        VDEC_SetMMParam(ucEsId, VDEC_MM_PLAY_SPEED, _Speed_convert(eSpeedType), 0, 0);
        VDP_SetFBNoTimeout(B2R_1, TRUE);
        VDP_SetSpeed(B2R_1,_Speed_convert(eSpeedType));
        VDP_SetSlowForward(B2R_1, VDP_PLAY_SLOW_FORWARD);
		
        if(_ePreviousSpeedType[ucEsId] == MM_SPEED_TYPE_PAUSE || _ePreviousSpeedType[ucEsId] == MM_SPEED_TYPE_STEP)
        {
        	  if(VDP_GetStepForward(B2R_1)!=0)
        	  {
        	  	VDP_SetFBNoTimeout(B2R_1,FALSE);
        	  	VDP_SetStepForward(B2R_1,VDP_PLAY_NORMAL);
        	  }
            VDP_SetPauseMm(B2R_1, FALSE);
            VDEC_Play(ucEsId, _aeFmt[ucEsId]);
        }		
    }
    else
    {
        if(_ePreviousSpeedType[ucEsId]== MM_SPEED_TYPE_PAUSE || _ePreviousSpeedType[ucEsId] == MM_SPEED_TYPE_STEP)
        {
            VDP_SetPauseMm(B2R_1, FALSE);
            VDEC_Play(ucEsId, _aeFmt[ucEsId]);
        }

        if(VDP_GetStepForward(B2R_1) != 0)
        {
            VDP_SetFBNoTimeout(B2R_1, FALSE);
            VDP_SetStepForward(B2R_1, VDP_PLAY_NORMAL);
            VDP_SetPauseMm(B2R_1, FALSE);
        }
        if(VDP_GetSlowForward(B2R_1) != 0)
        {
            VDP_SetFBNoTimeout(B2R_1, FALSE);
            VDP_SetSlowForward(B2R_1, VDP_PLAY_NORMAL);
        }

        VDEC_SetMMParam(ucEsId, VDEC_MM_PLAY_SPEED, _Speed_convert(eSpeedType), 0, 0);
        VDP_SetSpeed(B2R_1,_Speed_convert(eSpeedType));
        /* Trick Play at 20081001 */
        #ifdef CC_53XX_SWDMX_V2
        VDP_SetTrickMode(B2R_1,VDP_PLAY_TRICK);
        #else
        VDP_SetTrickMode(VDP_PLAY_TRICK);
        #endif
    }
    _ePreviousSpeedType[ucEsId]= eSpeedType;

    return i4Ret;
}


static INT32 _SetSeqInfo(UCHAR ucEsId, VID_DEC_SEQUENCE_INFO_T* prSeqInfo, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
//    VDEC_PES_INFO_T rPesInfo;
//    VID_DEC_SEQUENCE_RAW_T r_seq_raw;
#ifdef LINUX_TURNKEY_SOLUTION
    VID_DEC_SEQUENCE_INFO_T t_seq_header_info;
#endif
    M_SET_VALIDATE(VID_DEC_SEQUENCE_INFO_T);

    if(prSeqInfo->b_is_raw)
    {
        /*r_seq_raw = prSeqInfo->u.t_seq_raw;
        x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
        rPesInfo.fgSeqHdr = TRUE;
        rPesInfo.u4FifoStart = (UINT32)r_seq_raw.pc_buff;
        rPesInfo.u4FifoEnd = (UINT32)r_seq_raw.pc_buff + (UINT32)r_seq_raw.pc_buff;
        rPesInfo.u4VldReadPtr = (UINT32)r_seq_raw.pc_buff;
        //rPesInfo.ucPicType
        VDEC_SendEs(&rPesInfo);*/
        #ifdef ENABLE_MULTIMEDIA
        
         #ifndef LINUX_TURNKEY_SOLUTION
         MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDSEQHEADER, (UINT32)prSeqInfo->u.t_seq_raw.pc_buff, (UINT32)prSeqInfo->u.t_seq_raw.z_buff_size, 0);
         #else
           if(_MwDmx_data_from_usr_struct(&t_seq_header_info,prSeqInfo) != 0)
           {
                return RMR_DRV_ERROR;
           }
           MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_VIDSEQHEADER, (UINT32)t_seq_header_info.u.t_seq_raw.pc_buff, (UINT32)t_seq_header_info.u.t_seq_raw.z_buff_size, 0);
           if(t_seq_header_info.u.t_seq_raw.pc_buff != NULL)
           {
               x_mem_free(t_seq_header_info.u.t_seq_raw.pc_buff);
               t_seq_header_info.u.t_seq_raw.pc_buff = NULL;
           }
        #endif
        #ifdef DIVX_PLUS_CER
            if(prSeqInfo->pt_next_scale)
            {
#ifndef LINUX_TURNKEY_SOLUTION
                MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_TPT_VIDSEQHEADER, (UINT32)prSeqInfo->pt_next_scale->u.t_seq_raw.pc_buff, (UINT32)prSeqInfo->pt_next_scale->u.t_seq_raw.z_buff_size, 0);
#else
                if(_MwDmx_data_from_usr_struct(&t_seq_header_info,prSeqInfo->pt_next_scale) != 0)
                {
                    return RMR_DRV_ERROR;
                }
                MPV_MW_SWDMX_SetInfo(ucEsId, eSWDMX_SET_TPT_VIDSEQHEADER, (UINT32)t_seq_header_info.u.t_seq_raw.pc_buff, (UINT32)t_seq_header_info.u.t_seq_raw.z_buff_size, 0);
                if(t_seq_header_info.u.t_seq_raw.pc_buff != NULL)
                {
                    x_mem_free(t_seq_header_info.u.t_seq_raw.pc_buff);
                    t_seq_header_info.u.t_seq_raw.pc_buff = NULL;
                }
#endif
            }
        #endif
       #endif
        //VDEC_SetMMParam(ucEsId,VDEC_MM_FRAME_RATE,(UINT32)prSeqInfo->u.t_seq_raw.e_frame_rate,0,0);
    }
    else
    {
        VDEC_SetMMParam(ucEsId, VDEC_MM_SEQ_INFO, (UINT32)&prSeqInfo->u.t_seq_data, 0, 0);
        #ifdef DIVX_PLUS_CER
        if(prSeqInfo->pt_next_scale)
        {   
            VDEC_SetMMParam(ucEsId, VDEC_MM_TPT_SEQ_INFO, (UINT32)&prSeqInfo->pt_next_scale->u.t_seq_data, 0, 0);            
        }
        #endif
    }

    return i4Ret;
}

static INT32 _SetSuperFreeze(UCHAR ucEsId, BOOL fgSuperFreeze)
{
    INT32 i4Ret = RMR_OK;

    LOG(5, "_SetSuperFreeze(%d), %d\n", (UINT32)ucEsId, (UINT32)fgSuperFreeze);
    UNUSED(VDEC_SetSuperFreeze(ucEsId, fgSuperFreeze));

    return i4Ret;
}

#ifdef CC_BOOT_VDEC_LOGO
static INT32 _SetBootLogo(VID_DEC_BOOT_LOGO_SET_T* prInfo, SIZE_T z_set_info_len, BOOL b_store)
{
    BOOL fgLogoOn;
    UINT8 u1Valid1 = 0;
    UINT8 u1Valid2 = 0;
    INT32 i4Ret = RMR_OK;
    UNUSED(z_set_info_len);

    fgLogoOn = DRVCUST_OptGet(eDrvLogoVdecEnable);
    if(!fgLogoOn)
    {
        return RMR_INV_SET;
    }

    _rVdecBootLogo.rField.ucDefault = (UCHAR)prInfo->b_default;
    _rVdecBootLogo.rField.ucEnable = (UCHAR)prInfo->b_onoff;
    _rVdecBootLogo.rField.ucSelect= (UCHAR)prInfo->c_data_select;
    _u4Valid = prInfo->ui4_valid;
    
    if(b_store)
    {
        _fgVdecBootLogoRead = TRUE;
        fgApiEepromWriteByte(EEP_BOOT_VDEC_LOGO_CTRL, _rVdecBootLogo.aucReg[0]);
        u1Valid1 = (UINT8)(_u4Valid & 0xFF);
        u1Valid2 = (UINT8)((_u4Valid >> 8) & 0xFF);
        fgApiEepromWriteByte(EEP_BOOT_VDEC_LOGO_VALID_1, u1Valid1);
        fgApiEepromWriteByte(EEP_BOOT_VDEC_LOGO_VALID_2, u1Valid2);        
    }

    return i4Ret;
}
#endif

#ifdef CC_BOOT_VDEC_LOGO
static INT32 _SetBootLogoAddr(VID_DEC_BOOT_LOGO_ADDR_SET_T* prInfo, SIZE_T z_set_info_len, BOOL b_store)
{
    BOOL fgLogoOn;
    UINT32 u4Tmp;
    INT32 i4Ret = RMR_OK;

    fgLogoOn = DRVCUST_OptGet(eDrvLogoVdecEnable);
    if(!fgLogoOn)
    {
        return RMR_INV_GET;
    }
    
    _u4DataAddr = prInfo->ui4_addr;
    _u4DataSize = prInfo->ui4_data_size;
    _ucDataType = (UCHAR)_VidCodec2VdecFmt((VID_DEC_CODEC_T)prInfo->c_data_type);
    
    if(b_store)
    {
        _fgVdecBootLogoAddrRead = TRUE;
        fgApiEepromWriteByte((EEP_BOOT_VDEC_LOGO_DATA_TYPE),
            _ucDataType);
        
        u4Tmp = _u4DataSize;
        fgApiEepromWriteByte((EEP_BOOT_VDEC_LOGO_DATA_SIZE + 3),
                                (UCHAR)(u4Tmp & 0xFF));
        u4Tmp >>= 8;
        fgApiEepromWriteByte((EEP_BOOT_VDEC_LOGO_DATA_SIZE + 2),
                                (UCHAR)(u4Tmp & 0xFF));
        u4Tmp >>= 8;
        fgApiEepromWriteByte((EEP_BOOT_VDEC_LOGO_DATA_SIZE + 1),
                                (UCHAR)(u4Tmp & 0xFF));
        u4Tmp >>= 8;
        fgApiEepromWriteByte((EEP_BOOT_VDEC_LOGO_DATA_SIZE),
                                (UCHAR)(u4Tmp & 0xFF));

        u4Tmp = _u4DataAddr;
        fgApiEepromWriteByte((EEP_BOOT_VDEC_LOGO_DATA_ADDR + 3),
                                (UCHAR)(u4Tmp & 0xFF));
        u4Tmp >>= 8;
        fgApiEepromWriteByte((EEP_BOOT_VDEC_LOGO_DATA_ADDR + 2),
                                (UCHAR)(u4Tmp & 0xFF));
        u4Tmp >>= 8;
        fgApiEepromWriteByte((EEP_BOOT_VDEC_LOGO_DATA_ADDR + 1),
                                (UCHAR)(u4Tmp & 0xFF));
        u4Tmp >>= 8;
        fgApiEepromWriteByte((EEP_BOOT_VDEC_LOGO_DATA_ADDR),
                                (UCHAR)(u4Tmp & 0xFF));
    }
    
    return i4Ret;
}
#endif

static INT32 _MpvMwConnect(
                            DRV_COMP_ID_T*   pt_comp_id,
                            DRV_CONN_TYPE_T  e_conn_type,
                            const VOID*      pv_conn_info,
                            SIZE_T           z_conn_info_len,
                            VOID*            pv_tag,
                            x_rm_nfy_fct     pf_nfy)
{
    INT32 i4RetCode = RMR_OK;
    UCHAR ucEsId;
    DRV_COMP_ID_T* prDmxCompReg;

    UNUSED(z_conn_info_len);
    UNUSED(pv_tag);
    UNUSED(pf_nfy);

    /* check time shift super freeze status */
    if (_fgMpvSuperFreeze)
    {
        _fgMpvSuperFreezeDisc = FALSE;
		_fgMpvTimeShiftStatus = TRUE;
		
		
		prDmxCompReg = (DRV_COMP_ID_T*)pv_conn_info;
		ucEsId = _CompIdtoEsId(pt_comp_id->ui2_id);
		if(ucEsId >= (UCHAR)MPV_COMP_NS)
		{
			ucEsId = (UCHAR)MPV_COMP_1;
		}
		//_u4CompType[ucEsId] = (UINT32)prDmxCompReg->e_type;
		_au2UpStrmCompId[ucEsId] = (UINT32)prDmxCompReg->ui2_id;
		
		LOG(5, "%s	ucEsId = %d, comp id = %d\n", 
			__FUNCTION__, 
			ucEsId,
			_au2UpStrmCompId[ucEsId]);
			
		UNUSED(_au2UpStrmCompId[ucEsId]);
		UNUSED(pt_comp_id);
		
        return (RMR_OK);
    }
	else
	{
		_fgMpvTimeShiftStatus = FALSE;

	}	

    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->e_type != DRVT_VID_DEC)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->ui2_id >= (UINT16)MPV_COMP_NS)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->b_sel_out_port)
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

    if (pv_conn_info == NULL)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }


    prDmxCompReg = (DRV_COMP_ID_T*)pv_conn_info;

    if ((prDmxCompReg->e_type != DRVT_DEMUX_TS_PES_PACKET) &&
        (prDmxCompReg->e_type != DRVT_DEMUX_CONTAINER_ES))
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (((prDmxCompReg->e_type == DRVT_DEMUX_TS_PES_PACKET) &&
        ((prDmxCompReg->ui2_id) > (UINT16)(DMX_PES_COMP_ID_END))) ||
        ((prDmxCompReg->e_type == DRVT_DEMUX_CONTAINER_ES) &&
        ((prDmxCompReg->ui2_id) > (UINT16)(DMX_MM_CONTAINER_ES_COMP_ID_END))))
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    //build comp/es related id
    if(prDmxCompReg->e_type == DRVT_DEMUX_CONTAINER_ES)
    {
        #if defined(CC_3D_MM_DS_SUPPORT) || defined(CC_FBM_TWO_FBP)
        if(pt_comp_id->ui2_id < MPV_COMP_5)
        {
            MW_MPV_CompIdtoVldId(pt_comp_id->ui2_id, ES1);
            #if defined(CC_3D_MM_DS_SUPPORT) && defined(ENABLE_MULTIMEDIA) && defined(CC_53XX_SWDMX_V2)
            if((_au2UpStrmCompId[ES0] > 0) &&
            	  (_MW_DMX_MM_GetSwdmxSrcId(_au2UpStrmCompId[ES0]) == 
                _MW_DMX_MM_GetSwdmxSrcId(prDmxCompReg->ui2_id)))
            {
                _arMwMpv[pt_comp_id->ui2_id].fgIs3DSubVid = TRUE;
            }
            #endif
        }
        else
        #endif
        {
            MW_MPV_CompIdtoVldId(pt_comp_id->ui2_id, ES0);
        }
    }
#if defined(CC_VOMX_TV_COEXIST) && defined(CC_FBM_TWO_FBP)
    else if(prDmxCompReg->e_type == DRVT_DEMUX_TS_PES_PACKET)
    {
        if(pt_comp_id->ui2_id < MPV_COMP_5)
        {
            MW_MPV_CompIdtoVldId(pt_comp_id->ui2_id, ES1);
        }
        else
        {
            MW_MPV_CompIdtoVldId(pt_comp_id->ui2_id, ES0);
        }
    }
#endif

#ifdef CC_3D_MM_DS_SUPPORT
    MW_VDP_Comp3DInfo(pt_comp_id->ui2_id,_arMwMpv[pt_comp_id->ui2_id].fgIs3DSubVid);
#endif

    if(pt_comp_id->ui2_id >= (UINT16)MPV_COMP_NS)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }
    ucEsId = _CompIdtoEsId(pt_comp_id->ui2_id);
    if(ucEsId >= (UCHAR)MPV_COMP_NS)
    {
        ucEsId = (UCHAR)MPV_COMP_1;
    }
    _u4CompType[ucEsId] = (UINT32)prDmxCompReg->e_type;
    _au2UpStrmCompId[ucEsId] = (UINT32)prDmxCompReg->ui2_id;

    LOG(5, "%s  ucEsId = %d, comp id = %d\n", 
        __FUNCTION__, 
        ucEsId,
        _au2UpStrmCompId[ucEsId]);
        
    UNUSED(_au2UpStrmCompId[ucEsId]);
    UNUSED(pt_comp_id);

    return i4RetCode;
}


static INT32 _MpvMwDisconnect(
                            DRV_COMP_ID_T*   pt_comp_id,
                            DRV_DISC_TYPE_T  e_disc_type,
                            const VOID*      pv_disc_info,
                            SIZE_T           z_disc_info_len)
{
    INT32 i4RetCode = RMR_OK;
	UCHAR ucEsId;
    DRV_COMP_ID_T* prDmxCompReg;

    UNUSED(e_disc_type);
    UNUSED(pv_disc_info);
    UNUSED(z_disc_info_len);
	UNUSED(ucEsId);

    /* check time shift super freeze status */
    if (_fgMpvSuperFreeze)
    {
        /* record disconnect parameter for CR 212789 */
        _fgMpvSuperFreezeDisc = TRUE;
	    _fgMpvTimeShiftStatus = FALSE;	
		
        _rMpvDiscPara.t_comp_id = *pt_comp_id;
        _rMpvDiscPara.e_disc_type = e_disc_type;
        _rMpvDiscPara.t_disc_info = *(DRV_COMP_ID_T*)pv_disc_info;
        _rMpvDiscPara.z_disc_info_len = z_disc_info_len;
        return (RMR_OK);
    }
    else
    {
        _fgMpvSuperFreezeDisc = FALSE;
    }

    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    if (pt_comp_id->e_type != DRVT_VID_DEC)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (pt_comp_id->ui2_id >= (UINT16)MPV_COMP_NS)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    ucEsId = _CompIdtoEsId(pt_comp_id->ui2_id);
    if(ucEsId >= (UCHAR)MPV_COMP_NS)
    {
        ucEsId = (UCHAR)MPV_COMP_1;
    }
    VDEC_ClrMMParam(ucEsId, 0);

    if (pt_comp_id->b_sel_out_port)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (pt_comp_id->u.ui1_inp_port > 1)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (e_disc_type != RM_CONN_TYPE_COMP_ID)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (pv_disc_info == NULL)
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    prDmxCompReg = (DRV_COMP_ID_T*)pv_disc_info;

    if (prDmxCompReg->e_type != DRVT_DEMUX_TS_PES_PACKET &&
        prDmxCompReg->e_type != DRVT_DEMUX_CONTAINER_ES)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }
    if (((prDmxCompReg->e_type == DRVT_DEMUX_TS_PES_PACKET) &&
        ((prDmxCompReg->ui2_id) > (UINT16)(DMX_PES_COMP_ID_END))) ||
        ((prDmxCompReg->e_type == DRVT_DEMUX_CONTAINER_ES) &&
        ((prDmxCompReg->ui2_id) > (UINT16)(DMX_MM_CONTAINER_ES_COMP_ID_END))))
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    // handle disconnect
    _u4CompType[ucEsId] = 0;
    _au2UpStrmCompId[ucEsId] = 0;
    
    VDEC_SetDecNfy(ucEsId, (VDEC_DEC_NFY_INFO_T*)(void *)(NULL));
    _arMwMpv[pt_comp_id->ui2_id].ucEsId = 0xFF;
    #ifdef CC_3D_MM_DS_SUPPORT
    _arMwMpv[pt_comp_id->ui2_id].fgIs3DSubVid = FALSE;
    _arMwMpv[pt_comp_id->ui2_id].fgIsRealDEnhance = FALSE;
    _arMwMpv[pt_comp_id->ui2_id].fgIsMVCEnhance = FALSE;
    #endif
    #ifdef MPV_MW_GET_STATUS
    x_memset(&(_arMwMpv[pt_comp_id->ui2_id].rMpv), 0, sizeof(PARAM_MPV_T));
    #endif
    _arMwMpv[pt_comp_id->ui2_id].fgIsNotDisplay = FALSE;
    _arDecNfyInfo[pt_comp_id->ui2_id].pv_tag = 0;
    _arDecNfyInfo[pt_comp_id->ui2_id].pf_vid_dec_nfy = 0;

    UNUSED(pt_comp_id);

    return i4RetCode;
}


static INT32 _MpvMwGet(
                        DRV_COMP_ID_T*        pt_comp_id,
                        DRV_GET_TYPE_T        e_get_type,
                        VOID*                pv_get_info,
                        SIZE_T*                pz_get_info_len)
{
    INT32 i4RetCode = RMR_DRV_INV_GET_INFO;
    UCHAR ucEsId;

    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    if (pt_comp_id->e_type != DRVT_VID_DEC)
    {
           LOG(1, "MW_MPV_Get(): error component type or id type\n");
           return (RMR_DRV_INV_GET_INFO);
    }

    if (pt_comp_id->ui2_id >= (UINT16)MPV_COMP_NS)
    {
        LOG(1, "MW_MPV_Get(): error component id [%d]\n", pt_comp_id->ui2_id);
        return (RMR_DRV_INV_GET_INFO);
    }

    if (pz_get_info_len == NULL)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    ucEsId = _CompIdtoEsId(pt_comp_id->ui2_id);

    if ((ucEsId == (UCHAR)ES_ID_UNKNOWN) ||
        (ucEsId >= (UCHAR)ES_ID_MAX))
    {
        ucEsId = 0;
    }

    switch (e_get_type)
    {
        case VID_DEC_GET_TYPE_CTRL:
            i4RetCode = _GetCtrl(pt_comp_id->ui2_id, (VID_DEC_CTRL_T*)pv_get_info, pz_get_info_len);
            break;

        case VID_DEC_GET_TYPE_STC:
            i4RetCode = _GetStc(ucEsId, (PTS_T*)pv_get_info, pz_get_info_len);
            break;

        case VID_DEC_GET_TYPE_RESOLUTION:
            if ((_arMwMpv[pt_comp_id->ui2_id].ucMode == (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME) &&
                (!_arMwMpv[pt_comp_id->ui2_id].fgIsNotDisplay))
            {
                ucEsId = IFRAME_ES_ID;
            }
            i4RetCode = _GetResolution(ucEsId, (VID_DEC_RESOLUTION_INFO_T*)pv_get_info, pz_get_info_len);
            break;

        case VID_DEC_GET_TYPE_CAPABILITY:
            i4RetCode = _GetCapability(pt_comp_id->ui2_id, (VID_DEC_CAPABILITY_INFO_T*)pv_get_info, pz_get_info_len);
            break;

        case VID_DEC_GET_TYPE_PCR_ID:
            i4RetCode = _GetPcrId(pt_comp_id->ui2_id, (UINT16*)pv_get_info, pz_get_info_len);
            break;

        case VID_DEC_GET_TYPE_CC_DATA:
            i4RetCode = _GetCc(ucEsId, (VID_DEC_CC_DATA*)pv_get_info, pz_get_info_len);
            break;

        case VID_DEC_GET_TYPE_DEBLOCKING:
            i4RetCode = _GetDbkInfo(ucEsId, (VID_DEC_DEBLOCKING_INFO_T*)pv_get_info, pz_get_info_len);
            break;

        case VID_DEC_GET_TYPE_THUMBNAIL:
            i4RetCode = _GetVideoThumbnail(pt_comp_id->ui2_id, ucEsId,
                (VID_DEC_THUMBNAIL_INFO_T*)pv_get_info, pz_get_info_len);
            break;

#ifdef VID_DEC_GET_TYPE_ALLOC_I_FRAME_BUFF
        case VID_DEC_GET_TYPE_ALLOC_I_FRAME_BUFF:
            if (_arMwMpv[pt_comp_id->ui2_id].ucMode != (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME)
            {
                return (RMR_DRV_INV_GET_INFO);
            }
            if(!_arMwMpv[pt_comp_id->ui2_id].fgIsNotDisplay)
            {
                ucEsId = IFRAME_ES_ID;
            }
            i4RetCode = _SetAllocIFrameBuf(ucEsId, (VID_DEC_I_FRAME_BUFF_T*)pv_get_info, *pz_get_info_len);
            _IframeKnl2Usr((VID_DEC_I_FRAME_BUFF_T*)(UINT32)pv_get_info);
            break;
#endif
        #ifdef MPV_MW_GET_STATUS
        case VID_DEC_GET_TYPE_STATUS:
            i4RetCode = _GetStatus(ucEsId, pt_comp_id->ui2_id,
                (UINT32*)pv_get_info, pz_get_info_len);
            break;
        #endif

        default:
            LOG(1, "MPV_MwGet(): Invalid get_type\n");
            return (RMR_DRV_INV_GET_INFO);

    }

    UNUSED(pt_comp_id);

    return i4RetCode;
}


static INT32 _MpvMwSet(
                    DRV_COMP_ID_T*        pt_comp_id,
                    DRV_SET_TYPE_T         e_set_type,
                    const VOID*            pv_set_info,
                    SIZE_T                z_set_info_len)
{
    INT32 i4RetCode = RMR_DRV_INV_SET_INFO;
    UCHAR ucEsId;


    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_SET_INFO);
    }

    if (pt_comp_id->e_type != DRVT_VID_DEC)
    {
        LOG(1, "MW_MPV_Set(): error component type or id type\n");
        return (RMR_DRV_INV_SET_INFO);
    }

    if (pt_comp_id->ui2_id >= (UINT16)MPV_COMP_NS)
    {
        LOG(1, "MW_MPV_Set(): error component id [%d]\n", pt_comp_id->ui2_id);
        return (RMR_DRV_INV_SET_INFO);
    }

    #if 0
    // Check set_info
    if (pv_set_info == NULL)
    {
        LOG(1, "MW_MPV_Set(): invalid set_info\n");
        return (RMR_DRV_INV_SET_INFO);
    }
    #endif

    /* check time shift super freeze status */
    if (_fgMpvSuperFreeze)
    {
        switch (e_set_type)
        {
            /*
            case VID_DEC_SET_TYPE_SUPER_FREEZE:
                break;

            case VID_DEC_SET_TYPE_NFY_FCT:
            case VID_DEC_SET_TYPE_CODEC:
            case VID_DEC_SET_TYPE_SPEED:
                break;
            */

            case VID_DEC_SET_TYPE_CTRL:
                if((VID_DEC_CTRL_T)(UINT32)pv_set_info == VID_DEC_CTRL_PLAY)
                {
                    _fgMpvPlaying = TRUE;
                    _u2CompId = pt_comp_id->ui2_id;
                }
                else if((VID_DEC_CTRL_T)(UINT32)pv_set_info == VID_DEC_CTRL_STOP)
                {
                    _fgMpvPlaying = FALSE;
                }
                else if((VID_DEC_CTRL_T)(UINT32)pv_set_info == VID_DEC_CTRL_RESET)
                {
                    _fgMpvPlaying = FALSE;
                }
                else
                {
                }
                //stop command will notify until vdec enter stop state
                if (_arDecNfyInfo[pt_comp_id->ui2_id].pf_vid_dec_nfy)
                {
                    /* not necessary
                    #ifdef LINUX_TURNKEY_SOLUTION
                    set_mpv_set_nfy(pt_comp_id->ui2_id);
                    set_mpv_frame_nfy(pt_comp_id->ui2_id);
                    #endif
                    */
                    _arDecNfyInfo[pt_comp_id->ui2_id].pf_vid_dec_nfy(_arDecNfyInfo[pt_comp_id->ui2_id].pv_tag,
                                                VID_DEC_COND_CTRL_DONE,
                                                (UINT32)pv_set_info, 0);
                }
                return RMR_OK;

            default:
                //return RMR_OK;
                break;
        }
    }

    ucEsId = _CompIdtoEsId(pt_comp_id->ui2_id);

    if ((ucEsId == (UCHAR)ES_ID_UNKNOWN) ||
        (ucEsId >= (UCHAR)ES_ID_MAX))
    {
        ucEsId = 0;
    }

    switch (e_set_type)
    {
        case VID_DEC_SET_TYPE_CTRL:
            i4RetCode = _SetCtrl(pt_comp_id->ui2_id, (VID_DEC_CTRL_T)(UINT32)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_NFY_FCT:
            i4RetCode = _SetNfyFct(pt_comp_id->ui2_id, (VID_DEC_NFY_INFO_T*)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_NFY_PTS:
            i4RetCode = _SetNfyFctPts(pt_comp_id->ui2_id, (VID_DEC_NFY_PTS_INFO_T*)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_INP_MODE:
            i4RetCode = _SetInpMode(ucEsId, (VID_DEC_INP_MODE_INFO_T*)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_PLAY_CC:
            i4RetCode = _SetPlayCc(ucEsId, (VID_DEC_CC_NFY_INFO_T*)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_STOP_CC:
            i4RetCode = _SetStopCc(ucEsId);
            break;

        case VID_DEC_SET_TYPE_PCR_ID:
            i4RetCode = _SetPcrId(ucEsId, (UINT16)(UINT32)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_CAPTURE:
            i4RetCode = _SetCapture(ucEsId, (VID_DEC_CAPTURE_INFO_T*)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_ALLOC_CC_BUFF:
            i4RetCode = _SetAllocCcBuf(ucEsId, (SIZE_T)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_FREE_CC_BUFF:
            i4RetCode = _SetFreeCcBuf(ucEsId);
            break;

        case VID_DEC_SET_TYPE_FLUSH_CC_BUFF:
            i4RetCode = _SetFlushCcBuf(ucEsId);
            break;

        case VID_DEC_SET_TYPE_I_FRAME_BUFF:
            if (_arMwMpv[pt_comp_id->ui2_id].ucMode != (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME)
            {
                return (RMR_DRV_SET_FAILED);
            }
            _IframeUsr2Knl((VID_DEC_I_FRAME_BUFF_T*)(UINT32)pv_set_info);
            if(!_arMwMpv[pt_comp_id->ui2_id].fgIsNotDisplay)
            {
                ucEsId = IFRAME_ES_ID;
            }
            i4RetCode = _SetIFrameBuf(ucEsId, (VID_DEC_I_FRAME_BUFF_T*)pv_set_info, z_set_info_len);
            _IframeKnl2Usr((VID_DEC_I_FRAME_BUFF_T*)(UINT32)pv_set_info);
            break;

        case VID_DEC_SET_TYPE_ALLOC_I_FRAME_BUFF:
            if (_arMwMpv[pt_comp_id->ui2_id].ucMode != (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME)
            {
                return (RMR_DRV_SET_FAILED);
            }
            _IframeUsr2Knl((VID_DEC_I_FRAME_BUFF_T*)(UINT32)pv_set_info);
            if(!_arMwMpv[pt_comp_id->ui2_id].fgIsNotDisplay)
            {
                ucEsId = IFRAME_ES_ID;
            }
            i4RetCode = _SetAllocIFrameBuf(ucEsId, (VID_DEC_I_FRAME_BUFF_T*)pv_set_info, z_set_info_len);
            _IframeKnl2Usr((VID_DEC_I_FRAME_BUFF_T*)(UINT32)pv_set_info);
            break;

        case VID_DEC_SET_TYPE_FREE_I_FRAME_BUFF:
            if(!_arMwMpv[pt_comp_id->ui2_id].fgIsNotDisplay)
            {
                ucEsId = IFRAME_ES_ID;
            }
            i4RetCode = _SetFreeIFrameBuf(ucEsId);
            break;

        case VID_DEC_SET_TYPE_I_FRAME_DECODE:
            if (_arMwMpv[pt_comp_id->ui2_id].ucMode != (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME)
            {
                return (RMR_DRV_SET_FAILED);
            }
            _IframeUsr2Knl((VID_DEC_I_FRAME_BUFF_T*)(UINT32)pv_set_info);
            if(!_arMwMpv[pt_comp_id->ui2_id].fgIsNotDisplay)
            {
                ucEsId = IFRAME_ES_ID;
            }
            i4RetCode = _SetIFrameDec(ucEsId, (VID_DEC_I_FRAME_BUFF_T*)pv_set_info, z_set_info_len);
            _IframeKnl2Usr((VID_DEC_I_FRAME_BUFF_T*)(UINT32)pv_set_info);
            break;

        case VID_DEC_SET_TYPE_I_FRAME_NFY_FCT:
            i4RetCode = _SetIFrameNfyFct(pt_comp_id->ui2_id, (VID_DEC_NFY_INFO_T*)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_DEBLOCKING:
            i4RetCode = _SetDbkInfo(ucEsId, (VID_DEC_DEBLOCKING_INFO_T*)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_CODEC:
            #ifdef CC_FORCE_DTV_MPEG2_DECODER
            if(_u4CompType[ucEsId] != (UINT32)DRVT_DEMUX_CONTAINER_ES)
            {
                // DTV only
                // some project only have mpeg2 video decoder, try mpeg2 decoder only
                i4RetCode = _SetVideoType(ucEsId, VID_DEC_CODEC_MPEG_2, z_set_info_len);
            }
            else
            #endif
            {
                i4RetCode = _SetVideoType(ucEsId, (VID_DEC_CODEC_T)((SIZE_T)pv_set_info), z_set_info_len);
            }
            break;

        case VID_DEC_SET_TYPE_SPEED:
            i4RetCode = _SetSpeedType(ucEsId, (MM_SPEED_TYPE_T)((SIZE_T)pv_set_info), z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_SEQUENCE_INFO:
            i4RetCode = _SetSeqInfo(ucEsId, (VID_DEC_SEQUENCE_INFO_T*)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_SUPER_FREEZE:
            {
                VID_DEC_SUPER_FREEZE_T* ptSuperFreeze;

                i4RetCode = RMR_OK;
                ptSuperFreeze = (VID_DEC_SUPER_FREEZE_T*)pv_set_info;
                if (ptSuperFreeze->e_type == VID_DEC_SUPER_FREEZE_TYPE_FORCE)
                {
                    /* for time shift super freeze */
                    if(ptSuperFreeze->b_enable)
                    {
                        _fgMpvSuperFreeze = TRUE;
                        _fgMpvPlaying = FALSE;
                    }
                    else
                    {
                        _fgMpvSuperFreeze = FALSE;
                        _fgMpvPlaying = FALSE;
                        /* disconnect first by recorded disconnect parameter for CR 212789 */
                        /* otherwise actual connect can't notify resolution change to mw */
                        if (_fgMpvSuperFreezeDisc)
                        {
                            _MpvMwDisconnect(&_rMpvDiscPara.t_comp_id,
                                             _rMpvDiscPara.e_disc_type,
                                             (const VOID*)&_rMpvDiscPara.t_disc_info,
                                             _rMpvDiscPara.z_disc_info_len);
                        }
                    }
                    i4RetCode = _SetSuperFreeze(ucEsId, _fgMpvSuperFreeze);
                }
                else
                {
                    i4RetCode = RMR_DRV_INV_SET_INFO;
                }
            }
            break;
            
        #ifdef CC_3D_SUPPORT
        case VID_DEC_SET_TYPE_RD3D_REG_DESC:
            {
                VID_DEC_RD3D_REG_DESC_T* ptRD3DInfo;

                ptRD3DInfo = (VID_DEC_RD3D_REG_DESC_T*)pv_set_info;

                LOG(1, "MPV_MwSet(): RD3D, type %d\n", ptRD3DInfo->e_layer_type);
                #ifdef CC_3D_MM_DS_SUPPORT
                if(ptRD3DInfo->e_layer_type == VD_REALD_ENHANCEMENT_LAYER)
                {
                    _arMwMpv[pt_comp_id->ui2_id].fgIsRealDEnhance = TRUE;
                }
                #endif
                i4RetCode = RMR_OK;
            }
            break;
        case VID_DEC_SET_TYPE_MVC_EXT_DESC:
            LOG(1, "MPV_MwSet(): MVC\n");
            #ifdef CC_3D_MM_DS_SUPPORT
            _arMwMpv[pt_comp_id->ui2_id].fgIs3DSubVid = TRUE;
            _arMwMpv[pt_comp_id->ui2_id].fgIsMVCEnhance = TRUE;
            #endif
            i4RetCode = RMR_OK;
            break;
        #endif

        case VID_DEC_SET_TYPE_NOT_DISPLAY:
            _arMwMpv[pt_comp_id->ui2_id].fgIsNotDisplay = TRUE;
            
            // solution for MHP, MW does not call connect,
            // so we don't know how to setup the relationship of compid and esid.
            // setup it here
	        #if defined(CC_FBM_TWO_FBP)
	        if(pt_comp_id->ui2_id < MPV_COMP_5)
	        {
	            MW_MPV_CompIdtoVldId(pt_comp_id->ui2_id, ES1);
	            ucEsId = ES1;
	        }
	        else
	        #endif
	        {
	            MW_MPV_CompIdtoVldId(pt_comp_id->ui2_id, ES0);
	            ucEsId = ES0;
	        }

            VDEC_PlayNotDisplay(ucEsId, (BOOL)(UINT32)pv_set_info);
            i4RetCode = RMR_OK;
            break;
         
        case VID_DEC_SET_TYPE_FLUSH_ES:
            LOG(1, "MPV_MwSet(): VID_DEC_SET_TYPE_FLUSH_ES\n");
            MPV_FlushEsmQ(ucEsId, FALSE);
            i4RetCode = RMR_OK;
            break;
		case VID_DEC_SET_TYPE_VDEC_REPLAY:
			LOG(1, "MPV_MwSet(): VID_DEC_SET_TYPE_H264_REPLAY\n");
			VDEC_RePlayVdec(ucEsId);
			i4RetCode = RMR_OK;
			break;

        default:
            LOG(1, "MPV_MwSet(): Invalid set_type\n");
            return (RMR_DRV_INV_SET_INFO);

    }

    UNUSED(pt_comp_id);

    return i4RetCode;
}


//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------

INT32 MW_MPV_Init(void)
{
    INT32 i4Return;
    DRV_COMP_REG_T tMpvCompReg;
    #if !defined(CC_3D_MM_DS_SUPPORT) && !defined(CC_FBM_TWO_FBP)
    DRV_COMP_REG_T tMpvCompExclReg;
    #endif
    //DRV_COMP_REG_T tDmxCompReg;
    DRV_COMP_FCT_TBL_T tMpvFctTbl;
    UINT32 u4Idx;
    UINT16 u2CompId;

    if (!_hMutex)
    {
        // create semaphore
        VERIFY(x_sema_create(&_hMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    if(!_hVDPMWStepMutex)
    {
        /* Create semaphore for VDP Step Forward */
        VERIFY (x_sema_create(&_hVDPMWStepMutex, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    }

    /* Init MPV */
    VDEC_Init();

    for (u4Idx = 0; u4Idx < (UINT32)MPV_COMP_NS; u4Idx++)
    {
        _arPtsCb[u4Idx] = NULL;
        _aeFmt[u4Idx] = VDEC_FMT_MAX;
    }

    /* register mpv component */
    for (u4Idx = 0; u4Idx < (UINT32)MPV_COMP_NS; u4Idx++)
    {
        UINT32 u4Flags;

        /* Setup component ID */
        tMpvCompReg.e_type = DRVT_VID_DEC;
        tMpvCompReg.e_id_type = ID_TYPE_IND;
        tMpvCompReg.u.t_ind.ui2_id = (UINT16)u4Idx;
        tMpvCompReg.u.t_ind.ui1_port = ALL_PORTS;
        tMpvCompReg.u.t_ind.pv_tag = NULL;

#ifdef CC_SUPPORT_TVE
        /* Registration Flags */
        if ((u4Idx != (UINT32)MPV_COMP_5))
        {
            u4Flags = VID_DEC_REG_FLAG_USED_NORMAL_MODE|VID_DEC_REG_FLAG_USED_IN_QUAD_MODE;
        }
        else
        {
            u4Flags = VID_DEC_REG_FLAG_USED_NORMAL_MODE;
        }
#else
        /* Registration Flags */
        if ((u4Idx != (UINT32)MPV_COMP_5))
        {
            u4Flags = VID_DEC_REG_FLAG_USED_NORMAL_MODE|
                        VID_DEC_REG_FLAG_USED_IN_QUAD_MODE|DRV_FLAG_SINGLE_CONN_ON_OUTPUT;
        }
        else
        {
            u4Flags = VID_DEC_REG_FLAG_USED_NORMAL_MODE|DRV_FLAG_SINGLE_CONN_ON_OUTPUT;
        }
#endif

        /* Setup component function table */
        tMpvFctTbl.pf_rm_connect = _MpvMwConnect;
        tMpvFctTbl.pf_rm_disconnect = _MpvMwDisconnect;
        tMpvFctTbl.pf_rm_get = _MpvMwGet;
        tMpvFctTbl.pf_rm_set = _MpvMwSet;

        /* Register component with the Resource Manager database */
        i4Return = x_rm_reg_comp(&tMpvCompReg,
                                1,
                                1,
                                NULL,
                                u4Flags,
                                &tMpvFctTbl,
                                NULL,
                                0);

        if (i4Return != RMR_OK)
        {
            LOG(1, "MW_MPV_Init(): x_rm_reg_comp failed [%d]\n", (INT32)i4Return);
            return i4Return;
        }

    }

    /* register connection list */
#if 0
    for (u4Idx = MPV_COMP_1; u4Idx < MPV_COMP_NS; u4Idx++ )
    {

        tMpvCompReg.e_type = DRVT_VID_DEC;
        tMpvCompReg.e_id_type = ID_TYPE_IND;
        tMpvCompReg.u.t_ind.ui2_id = u4Idx;
        tMpvCompReg.u.t_ind.ui1_port = ALL_PORTS;
        tMpvCompReg.u.t_ind.pv_tag = NULL;


        /* Setup Dmx component ID */
        tDmxCompReg.e_type = DRVT_DEMUX_TS_PES_PACKET;
        tDmxCompReg.e_id_type = ID_TYPE_RANGE;
        tDmxCompReg.u.t_range.ui2_first_id = DMX_PES_COMP_ID_START;
        tDmxCompReg.u.t_range.ui2_delta_id = 1;
        tDmxCompReg.u.t_range.ui2_num_of_ids = DMX_PES_COMP_NUM;
        tDmxCompReg.u.t_range.pv_first_tag = NULL;
        tDmxCompReg.u.t_range.pv_delta_tag = NULL;
        tDmxCompReg.u.t_range.ui1_port = ALL_PORTS;

        /* register connection list */
        i4Return = x_rm_reg_conn_list( &tMpvCompReg,
                                    &tDmxCompReg,
                                    1,
                                    CONN_DIR_INP_TO_OUT,
                                    FALSE );

        if ( i4Return != RMR_OK )
        {
            LOG(1, "MW_MPV_Init(): x_rm_reg_conn_list failed [%d]\n", (unsigned int)i4Return);
            return i4Return;
        }

    }
#endif

#if !defined(CC_3D_MM_DS_SUPPORT) && !defined(CC_FBM_TWO_FBP)
    // multi-track is able to open two comp at the same time
    /* Register component exclusive list */
    tMpvCompReg.e_type = DRVT_VID_DEC;
    tMpvCompReg.e_id_type = ID_TYPE_IND;
    tMpvCompReg.u.t_ind.ui2_id = (UINT16)MPV_COMP_5;
    tMpvCompReg.u.t_ind.ui1_port = ALL_PORTS;
    tMpvCompReg.u.t_ind.pv_tag = NULL;

    tMpvCompExclReg.e_type = DRVT_VID_DEC;
    tMpvCompExclReg.e_id_type = ID_TYPE_RANGE;
    tMpvCompExclReg.u.t_range.ui2_first_id = (UINT16)MPV_COMP_1;
    tMpvCompExclReg.u.t_range.ui2_delta_id = 1;
    tMpvCompExclReg.u.t_range.ui2_num_of_ids = (UINT16)MPV_COMP_SD_NS;
    tMpvCompExclReg.u.t_range.pv_first_tag = NULL;
    tMpvCompExclReg.u.t_range.pv_delta_tag = NULL;
    tMpvCompExclReg.u.t_range.ui1_port = ALL_PORTS;

    i4Return = x_rm_reg_comp_excl_list (&tMpvCompReg,
                                        &tMpvCompExclReg,
                                        1);

    if (i4Return != RMR_OK)
    {
        LOG(1, "MW_MPV_Init(): x_rm_reg_comp_excl_list failed [%d]\n", (INT32)i4Return);
        return i4Return;
    }
#endif
    for (u2CompId = 0 ;u2CompId < (UINT16)MPV_COMP_NS; u2CompId++)
    {
        _arMwMpv[u2CompId].ucEsId = (UCHAR)0xFF;
        _arMwMpv[u2CompId].ucMode= (UCHAR)VID_DEC_CTRL_STOP;
        #ifdef CC_3D_MM_DS_SUPPORT
        _arMwMpv[u2CompId].fgIs3DSubVid = FALSE;
        _arMwMpv[u2CompId].fgIsRealDEnhance = FALSE;
        _arMwMpv[u2CompId].fgIsMVCEnhance = FALSE;
        #endif
        _arMwMpv[u2CompId].fgIsNotDisplay = FALSE;
    }

    return (RMR_OK);
}


UCHAR MW_MPV_CompIdtoVldId(UINT16 u2CompId, UINT32 u4ChannelId)
{
    UCHAR ucVldId = 0xf;

    switch (u2CompId)
    {
        case MPV_COMP_1:
        case MPV_COMP_2:
        case MPV_COMP_3:
        case MPV_COMP_4:
        case MPV_COMP_5:
            ucVldId = VLD0;
            break;

        default:
            LOG(1, "Invalid CompId\n");
            ASSERT(0);
            break;
    }

    _arMwMpv[u2CompId].ucEsId= (UCHAR)u4ChannelId;

    MW_VDP_Comp2EsInd((UCHAR)u2CompId, (UCHAR)u4ChannelId);

    return (ucVldId);
}

#if defined(CC_VOMX_TV_COEXIST) && defined(CC_FBM_TWO_FBP)
UCHAR MW_MPV_CompIdtoEsId(UINT16 u2CompId)
{
    UCHAR ucEsId = 0xFF;

    switch (u2CompId)
    {
        case MPV_COMP_1:
        case MPV_COMP_2:
        case MPV_COMP_3:
        case MPV_COMP_4:
            ucEsId = ES1;
            break;

        case MPV_COMP_5:
            ucEsId = ES0;
            break;

        default:
            LOG(1, "Invalid CompId\n");
            ASSERT(0);
            break;
    }

    return (ucEsId);
}
#endif

void MW_MPV_PlayDone_NOTIFY(UINT16 u2CompId)
{
    if(_arDecNfyInfo[u2CompId].pf_vid_dec_nfy)
    {
        _arDecNfyInfo[u2CompId].pf_vid_dec_nfy(_arDecNfyInfo[u2CompId].pv_tag, VID_DEC_COND_PLAY_DONE, 0, 0);
    }
}

void MW_MPV_NOTIFY(UINT16 u2CompId)
{
    if(u2CompId >= (UINT16)MPV_COMP_NS)
    {
        LOG(0, "%s, incorrect u2CompId(%d)\n", __FUNCTION__, u2CompId);
        return;
    }

    if (_arDecNfyInfo[u2CompId].pf_vid_dec_nfy)
    {
    #ifdef LINUX_TURNKEY_SOLUTION
        set_mpv_set_nfy(u2CompId);
        set_mpv_frame_nfy(u2CompId);
    #endif
        _arDecNfyInfo[u2CompId].pf_vid_dec_nfy(_arDecNfyInfo[u2CompId].pv_tag, VID_DEC_COND_RESOLUTION_CHG, 0, 0);

        #ifdef CC_3D_MM_DS_SUPPORT
        if(_arMwMpv[u2CompId].fgIs3DSubVid)
        {
            //find main track
            UINT32 u4Idx;
            for (u4Idx = 0; u4Idx < (UINT32)MPV_COMP_NS; u4Idx++)
            {
                if((_arMwMpv[u4Idx].ucEsId != 0xFF) && (u4Idx != u2CompId))
                {
                    if(_arDecNfyInfo[u4Idx].pf_vid_dec_nfy)
                    {
                        _arDecNfyInfo[u4Idx].pf_vid_dec_nfy(_arDecNfyInfo[u4Idx].pv_tag, VID_DEC_COND_RESOLUTION_CHG, 0, 0);
                    }
                    break;
                }
            }
        }
        #endif
    }
}

#ifdef ENABLE_MULTIMEDIA
void MW_MPV_SET_SWDMXSrcId(UINT8 u1SwdmxSrcId)
{
    // remove it later
    UNUSED(u1SwdmxSrcId);
}
#endif

INT32 d_custom_vid_dec_set (
    DRV_CUSTOM_VID_DEC_TYPE_T   e_vdec_type,
    VOID*                       pv_set_info,
    SIZE_T                      z_size,
    BOOL                        b_store
)
{
    INT32 i4RetCode = RMR_OK;
    switch(e_vdec_type)
    {
#ifdef CC_BOOT_VDEC_LOGO
        case DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_CTRL:
            i4RetCode = _SetBootLogo(
                (VID_DEC_BOOT_LOGO_SET_T*)pv_set_info, z_size, b_store);
            break;
        case DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_ADDR:
            i4RetCode = _SetBootLogoAddr(
                (VID_DEC_BOOT_LOGO_ADDR_SET_T*)pv_set_info, z_size, b_store);
            break;
#endif
        default:
            break;
    }
    UNUSED(pv_set_info);
    UNUSED(z_size);
    UNUSED(b_store);
    return i4RetCode;
}

INT32 d_custom_vid_dec_get (
    DRV_CUSTOM_VID_DEC_TYPE_T   e_vdec_type,
    VOID*                       pv_get_info,
    SIZE_T*                     pz_size
)
{
    INT32 i4RetCode = RMR_OK;
    switch(e_vdec_type)
    {
#ifdef CC_BOOT_VDEC_LOGO
        case DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_CTRL:
            i4RetCode = _GetBootLogo(
                (VID_DEC_BOOT_LOGO_GET_T*)pv_get_info, pz_size);
            break;
        case DRV_CUSTOM_VID_DEC_TYPE_BOOT_LOGO_ADDR:
            i4RetCode = _GetBootLogoAddr(
                (VID_DEC_BOOT_LOGO_ADDR_GET_T*)pv_get_info, pz_size);
            break;
#endif
        default:
            break;
    }
    UNUSED(pv_get_info);
    UNUSED(pz_size);
    return i4RetCode;
}
#ifdef MULTIMEDIA_AUTOTEST_SUPPORT 
void MM_AUTOTEST_SET_MPV_CALLBACK(UCHAR ucEsId, UINT32 pfAtDecNfy,VOID *pv_Tag)
{	

	  ASSERT(ucEsId < VDEC_MAX_ES);
	  VDEC_SetAutoTestDecNfy(ucEsId, pfAtDecNfy, pv_Tag);

}
#ifdef VDEC_TIME_PROFILE
VOID MM_AUTOTEST_SET_CUR_FILE_DIR(UCHAR ucEsId, CHAR *ucAutoPlayCurFileDir)
{
    ASSERT(ucEsId < VDEC_MAX_ES);
    ASSERT(ucAutoPlayCurFileDir != NULL);
    VDEC_SetAutoTestCurFileDir(ucEsId, ucAutoPlayCurFileDir);
}
VOID MM_AUTOTEST_SET_CUR_FILE_NAME(UCHAR ucEsId, CHAR *ucAutoPlayCurFileName)
{
    ASSERT(ucEsId < VDEC_MAX_ES);
    ASSERT(ucAutoPlayCurFileName != NULL);
    VDEC_SetAutoTestCurFileName(ucEsId, ucAutoPlayCurFileName);
}
#endif
#endif

