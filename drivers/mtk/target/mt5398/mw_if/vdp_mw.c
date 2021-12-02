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

#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_data_rm.h"
#include "cb_param.h"
#include "x_debug.h"
#endif

#include "x_lint.h"
#include "vdp_drvif.h"
#include "b2r_drvif.h"
#include "fbm_drvif.h"
#include "pmx_drvif.h"
LINT_EXT_HEADER_BEGIN
#include "drv_comp_id.h"
#include "drv_name.h"
#include "drv_hdmi.h"
// #include "api_video.h"  // nptv
#include "pe_if.h"
#include "source_table.h"  // nptv
#include "video_def.h"  // nptv
#include "sv_const.h"  // nptv
#include "vdo_misc.h"  // nptv
#include "drv_video.h" // nptv
#include "drv_scaler.h"
#include "drv_ycproc.h"
#include "nptv_if.h"
#include "mute_if.h"
#include "drvcust_if.h"
#include "d_drv_cust_util.h"
#include "u_drv_cust.h"
#include "x_vid_plane.h"
#include "x_assert.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_avc.h"
#include "x_debug.h"
#include "vdec_mw.h"
#include "vdec_if.h"
#include "mpv_if.h"
#include "mpv_drvif.h"
#include "srm_drvif.h"
#include "mute_if.h"
#include "vdp_if.h"
#include "vdp_drvif.h"
#include "drv_tdtv_drvif.h"

#ifdef ENABLE_MULTIMEDIA
#include "swdmx_if.h"
#endif
#ifdef CC_FLIP_MIRROR_SUPPORT
#include "eeprom_if.h"
#endif
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

#if 0
#ifndef LINUX_TURNKEY_SOLUTION
#define CC_DISP_FMT_ENUM_SIZE      1
#else
#define CC_DISP_FMT_ENUM_SIZE      18
#endif
#else
#define CC_DISP_FMT_ENUM_SIZE (VID_PLA_DISP_FMT_MAX)
#endif

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
    UCHAR ucBlank;

    /* for Super Freeze */
    UCHAR ucMemoMode;
    UCHAR ucPmxCompId;
    UCHAR ucLocalId;

    UCHAR ucLbEnable;
    UINT16 u2DynamicScale;

    UCHAR ucSrcChgNfy;

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
    UINT16 u2SrcCompId;
    UINT16 u2TvdCompId;
    UINT32 u4FrameRate;
    UINT32 u4Interlace;
    VID_PLA_QV_INFO_T rQvInfo;
    VID_PLA_LB_DETECT_CONFIG_T rLBConfig;

    VID_PLA_SPLIT_SCRN_DEMO_T eDemo;

    VOID* pvEosTag;
    x_vid_pla_nfy_fct pfEosNfy;
#ifdef MULTIMEDIA_AUTOTEST_SUPPORT
	VOID* pvATEosTag;
	x_vid_pla_nfy_fct pfAtEosNfy;	 
#endif

#ifdef CC_3D_MM_DS_SUPPORT
    BOOL fgFrameSeq;
#endif    
} MW_VDP_T;

/* add for CR 212789 */
typedef struct _MW_VDP_DISC_PARA_T
{
    DRV_COMP_ID_T    t_comp_id;
    DRV_DISC_TYPE_T  e_disc_type;
    DRV_COMP_ID_T    t_disc_info;
    SIZE_T           z_disc_info_len;
} MW_VDP_DISC_PARA_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define VID_QA_TBL_IS_END( pt_vid_qa_tbl )                 \
    ( pt_vid_qa_tbl->e_src_type == DRVT_UNKNOWN &&         \
      pt_vid_qa_tbl->pt_min_max_dft_lst == NULL )

#define VID_QA_TBL_EX_IS_END( pt_vid_qa_tbl )                     \
    ( pt_vid_qa_tbl->e_src_type == DRVT_UNKNOWN &&                \
      pt_vid_qa_tbl->e_pic_mode == VID_PLA_PIC_MODE_DEFAULT &&    \
      pt_vid_qa_tbl->pt_min_max_dft_lst == NULL )

#define VID_MIN_MAX_DFT_IS_END( pt_min_max_dft )                 \
    ( pt_min_max_dft->e_vid_qa_type == VID_QA_TYPE_UNKNOWN &&    \
      pt_min_max_dft->i4_min == 0 &&                             \
      pt_min_max_dft->i4_max == 0 &&                             \
      pt_min_max_dft->i4_default == 0 )

#define VERIFY_VDP_ID(id)                                   \
          do {                                              \
            if (id >= VDP_NS) { return; }     \
          } while (0)



#define MAX_ATOMIC_CMD_NUM 20
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
EXTERN void MW_MPV_PlayDone_NOTIFY(UINT16 u2CompId);
EXTERN void MW_MPV_NOTIFY(UINT16 u2CompId);
EXTERN UINT32 VDP_CheckScalerDisplayMode(void);
EXTERN UINT32 VDP_IsSupportDotByDot(void);
EXTERN void MW_PMX_PORT_DISCONNECT(UCHAR ucPmxCompId, UCHAR ucLocalId);
EXTERN int rm_copy_to_user(VOID *pv_user,VOID* pv_kernel,UINT32 size);
EXTERN VOID lock_nfy_param_buf(int i4_id);
EXTERN VOID rm_copy_from_user(VOID* pv_kernel,VOID *pv_user,UINT32 size);
EXTERN VOID unlock_nfy_param_buf(int i4_id);
EXTERN INT32 _CB_PutEvent_Adpt(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
EXTERN INT32 _CB_PutEvent_Adpt_Ext(CB_FCT_ID_T e_FctId, INT32 i4_TagSize, void* pv_Tag, INT32 i4_OutSize, void* pv_Output);
EXTERN unsigned long adapt_fbm_knl_to_usr(unsigned long addr);

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

static PE_ARG_TYPE _VdpMwGetType2ArgType(UINT32 u4GetType);

static PE_ARG_TYPE _VdpMwSetType2ArgType(UINT32 u4SetType);

static void _VdpPlayDoneNotify(UINT32 u4VdpId, UINT32 u4Arg1, UINT32 u4Arg2);

static void _VdpAfdChangeNotify(UINT32 u4VdpId, UINT32 u4Afd,FBM_ASPECT_RATIO_T* prAspectRatio);

static UINT32 _VdpSourceChangeNotify(UCHAR ucVdpId, VDP_SOURCE_INFO_T rVdpSrcInfo);

static UINT32 _VdpXlatDispFmtCallBackMw(UCHAR ucVdpId);

static void _VdpCalculateOutputRegion(UCHAR ucVdpId);

static void _VdpCalculateSourceRegion(UCHAR ucVdpId);

static DRVP_TYPE_T _VdpDrvCustParse(const DRV_CUSTOM_T* pt_drv_cust, VOID* pv_tag);

static DRV_CUSTOM_COLOR_SYS_TYPE_T _VdpGetDrvCustColorSys(UCHAR ucVdpId);

static DRV_CUST_INPUT_SRC_TYPE_T _VdpGetDrvCustInputSrcType(UCHAR ucVdpId, DRV_TYPE_T e_type);

static BOOL _VdpUpdateOverscan(UCHAR ucVdpId, UINT32* pui4_top, UINT32* pui4_bottom, UINT32* pui4_left, UINT32* pui4_right);

static void _VdpUpdateDispFmt(UCHAR ucVdpId);

static void _VdpEosCallback(UINT32 u4VdpId, VDP_COND_T eCond, BOOL fgFastForward);

static BOOL _VdpParseMinMaxDftTbl(UINT8 ui1_src_idx, VID_QA_MIN_MAX_DFT_T* pt_min_max_dft_tbl );

static INT32   _VdpSetDispRegion(UCHAR ucVdpId, const VOID* pv_set_info);
static INT32   _VdpSetDispFormat(UCHAR ucVdpId, const VOID* pv_set_info);
static INT32   _VdpSetOverScan(UCHAR ucVdpId, const VOID* pv_set_info);

void _MW_VDP_WSS625_CB(UCHAR ucVdpId, UINT16 u2Data);
void _MW_VDP_WSS525_CB(UCHAR ucVdpId, UINT16 u2Data);
void _MW_VDP_SCART_PIN_8_CB(UCHAR ucVdpId, DRV_CUSTOM_SCART_PIN_8_T t_scart_pin_8);
extern UINT8 MWGetScartPin8Status(UINT8 u1Path);

void MW_VDP_CallbackTrigger(UCHAR ucVdpId);
UCHAR MW_VDP_MEMO_MODE_GET(UCHAR ucVdpId);
void MW_VDP_MEMO_MODE_SET_PMX_INFO(UCHAR ucVdpId, UCHAR ucPmxCompId, UCHAR ucLocalId);

extern UINT8 bHDMIInputType(void);

UINT8 bApiFlashPqUpdateQtyDft(void);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static MW_VDP_T _rMwVdpDefault =
{
    0, 0, 0, 0,
    0, PMX_COMP_NS, PMX_MAX_INPORT_NS,
    0, 0,
    0,
    VID_PLA_NORMAL,
    VID_PLA_DISP_FMT_NORMAL,
    DRVT_UNKNOWN,
    TV_DEC_TIMING_UNKNOWN,
    { 0, 0 },
    { 0, 0 },
    {0, 0, 0, VDP_MAX_REGION_WIDTH, VDP_MAX_REGION_HEIGHT},
    {0, 0, 0, VDP_MAX_REGION_WIDTH, VDP_MAX_REGION_HEIGHT},
    {0, 0, 0, VDP_MAX_REGION_WIDTH, VDP_MAX_REGION_HEIGHT},
    {0, 0, 0, VDP_MAX_REGION_WIDTH, VDP_MAX_REGION_HEIGHT},
    {0, 0, 0, VDP_MAX_REGION_WIDTH, VDP_MAX_REGION_HEIGHT},
    {0, 0, 0, VDP_MAX_REGION_WIDTH, VDP_MAX_REGION_HEIGHT},

    255,
    (UINT16)MPV_COMP_NS,
    (UINT16)TVD_COMP_NS,
     0, 0,
    {VID_PLA_QV_INP_NULL, VID_PLA_QV_INP_NULL, VID_PLA_QV_INP_NULL, VID_PLA_QV_INP_NULL},

    VID_PLA_SPLIT_SCRN_DEMO_OFF
};

MW_VDP_T _arMwVdp[VDP_COMP_NS];

static DRV_CUSTOM_VID_SRC_INFO_T _arMwVidSrcInfo[VDP_COMP_NS];

UCHAR _arMwComp2EsId[MPV_COMP_NS];

#ifdef CC_3D_MM_DS_SUPPORT
UCHAR _arMwComp3dInfo[MPV_COMP_NS];
#endif

static VDP_CONF_T* _prVdpConf[VDP_NS];

#if 0
#ifndef LINUX_TURNKEY_SOLUTION
static d_vid_pla_xlat_disp_fmt_fct _pfXlatDispFmtFct[(1 << (8 * CC_DISP_FMT_ENUM_SIZE))];

static d_vid_pla_xlat_disp_fmt_ex_fct _pfXlatDispFmtExFct[(1 << (8 * CC_DISP_FMT_ENUM_SIZE))];
#else
static d_vid_pla_xlat_disp_fmt_fct _pfXlatDispFmtFct[CC_DISP_FMT_ENUM_SIZE];

static d_vid_pla_xlat_disp_fmt_ex_fct _pfXlatDispFmtExFct[CC_DISP_FMT_ENUM_SIZE];
#endif
#else
static d_vid_pla_xlat_disp_fmt_ex_fct _pfXlatDispFmtExFct[CC_DISP_FMT_ENUM_SIZE];
#endif

static d_vid_pla_get_current_disp_fmt_fct _pfGetCurrentDispFmtFct;

static d_vid_pla_overscan_fct _pfOverscanFct;

static d_vid_pla_update_overscan_tbl_fct _pfUpdateOverscanTbl;

static d_vid_pla_vid_qa_tbl_query_fct _pfVidQaTblQueryFct;

static HANDLE_T _hMutex = 0;            // Thread safe protection

//#define CC_SCPOS_FORCE_UNMUTE
//#ifdef CC_SCPOS_FORCE_UNMUTE
static BOOL fgVDPForceUnmute = FALSE;
//#endif

static BOOL fgVDPForceFreeze = FALSE;
static BOOL _fgVDPSuperFreezeDisc = FALSE;
static MW_VDP_DISC_PARA_T _rVDPDiscPara;

extern UINT8  u1HdmiColorMode  ;
extern void vApiHdmiColorModeChg(UINT8 bPath) ;

static UINT16 _ui2_pal_secam_wss = 0;

static BOOL fgEnableMwCb = TRUE;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef LINUX_TURNKEY_SOLUTION
//static VDP_PLA_NFY_PARAM vdp_pla_param;

//static x_vid_pla_nfy_fct vdp_pla_nfy_tbl[VDP_COMP_NS];
/*
static VOID set_vdp_pla_nfy(UINT32 ui4_id)
{
    vdp_pla_param.pf_nfy = vdp_pla_nfy_tbl[ui4_id];
}
*/
static VOID vdp_pla_nfy_fct(UINT32 ui4_id,
                        VOID*                  pv_nfy_tag,
                        VID_PLA_COND_T          e_nfy_cond,
                        UINT32                 ui4_data_1,
                        UINT32                 ui4_data_2)
{
    VDP_PLA_NFY_PARAM vdp_pla_param;

    if (ui4_id >= VDP_COMP_NS)
    {
        return;
    }

    vdp_pla_param.pf_nfy = _arMwVdp[ui4_id].pfEosNfy;
    vdp_pla_param.pv_nfy_tag = pv_nfy_tag;
    vdp_pla_param.e_nfy_cond = e_nfy_cond;
    vdp_pla_param.ui4_data_1 = ui4_data_1;
    vdp_pla_param.ui4_data_2 = ui4_data_2;
    _CB_PutEvent_Adpt(CB_DRV_VDP_PLA_NFY, sizeof(VDP_PLA_NFY_PARAM), &vdp_pla_param);
}

static VOID set_vdp_ex_nfy(UINT32                               ucVdpId,
                           const DRV_CUSTOM_RESOLUTION_INFO_T*  pt_src_res,
                           const DRV_CUSTOM_RESOLUTION_INFO_T*  pt_disp_res,
                           const DRV_CUSTOM_VID_SRC_INFO_T*     pt_vid_src_info,
                           VID_PLA_VID_REGION_T*                pt_src_region,     /* OUT */
                           VID_PLA_VID_REGION_T*                pt_disp_region)    /* OUT */
{
    #if 1
    #else
    VDP_EX_NFY_PARAM * pv_dst;
    UINT32 addr;
    UINT32 fct;
    #endif
    VDP_EX_NFY_PARAM   dst;
    #if 1
    //VDP_NFY_OUT_PARAM tOutData;

    /*if ( in_interrupt( ) )
    {
        ASSERT(0);
    }*/

    dst.pf_nfy = _pfXlatDispFmtExFct[(UINT32)_arMwVdp[ucVdpId].eFmt];
    dst.t_src_res = *pt_src_res;
    dst.t_disp_res = *pt_disp_res;
    dst.t_vid_src_info = *pt_vid_src_info;
    #if 1
    dst.tOut.t_src_region = *pt_src_region;
    dst.tOut.t_disp_region = *pt_disp_region;
    #else
    dst.t_src_region = *pt_src_region;
    dst.t_disp_region = *pt_disp_region;
    #endif

    _CB_PutEvent_Adpt_Ext(CB_DRV_VDP_EX_NFY, sizeof(VDP_EX_NFY_PARAM), &dst, sizeof(VDP_NFY_OUT_PARAM), &(dst.tOut));  

    *pt_src_region = dst.tOut.t_src_region;
    *pt_disp_region = dst.tOut.t_disp_region;
    #else    
    lock_nfy_param_buf(PARAM_VDP_EX_NFY);

    pv_dst = (VDP_EX_NFY_PARAM *)get_nfy_param_buf(PARAM_VDP_EX_NFY,512);

    dst.pf_nfy = _pfXlatDispFmtExFct[(UINT32)_arMwVdp[ucVdpId].eFmt];
    dst.t_src_res = *pt_src_res;
    dst.t_disp_res = *pt_disp_res;
    dst.t_vid_src_info = *pt_vid_src_info;
    dst.t_src_region = *pt_src_region;
    dst.t_disp_region = *pt_disp_region;
    rm_copy_to_user(pv_dst,&dst,sizeof(VDP_EX_NFY_PARAM));
    addr = pv_dst;
    _CB_PutEvent_Adpt(CB_DRV_VDP_EX_NFY, sizeof(UINT32), &addr);

    rm_copy_from_user(&dst,pv_dst,sizeof(VDP_EX_NFY_PARAM));

    unlock_nfy_param_buf(PARAM_VDP_EX_NFY);

    *pt_src_region = dst.t_src_region;
    *pt_disp_region = dst.t_disp_region;
    #endif
}

static VOID vid_pla_get_current_disp_fmt_fct (
            DRV_CUSTOM_RESOLUTION_INFO_T*       pt_src_res,
            DRV_CUSTOM_RESOLUTION_INFO_T*       pt_disp_res,
            DRV_CUSTOM_VID_SRC_INFO_T*          pt_vid_src_info,
            VID_PLA_DISP_FMT_T*                 pe_disp_fmt )
{
    #if 1
    #else
    PARMA_GET_CUR_DISP_FMT *pv_dst;
    UINT32 addr;
    #endif
    PARMA_GET_CUR_DISP_FMT dst;
    #if 1
    VID_PLA_DISP_FMT_T tDispFmt;

    /*if ( in_interrupt( ) )
    {
        ASSERT(0);
    }*/
    #endif
    dst.t_src_res = *pt_src_res;
    dst.t_disp_res = *pt_disp_res;
    dst.t_vid_src_info = *pt_vid_src_info;
    dst.e_disp_fmt = *pe_disp_fmt;
    dst.pf_nfy = _pfGetCurrentDispFmtFct;

    #if 1
    _CB_PutEvent_Adpt_Ext(CB_DRV_VDP_GET_CUR_DISP_FMT, sizeof(PARMA_GET_CUR_DISP_FMT), &dst, sizeof(VID_PLA_DISP_FMT_T), &tDispFmt);  
    *pe_disp_fmt  = tDispFmt;
    #else    
    lock_nfy_param_buf(PARMA_GET_CUR_DISP_FMT_NFY);
    pv_dst = (PARMA_GET_CUR_DISP_FMT *)get_nfy_param_buf(PARMA_GET_CUR_DISP_FMT_NFY, sizeof(PARMA_GET_CUR_DISP_FMT));
    rm_copy_to_user(pv_dst,&dst,sizeof(PARMA_GET_CUR_DISP_FMT));
    addr = pv_dst;
    _CB_PutEvent_Adpt(CB_DRV_VDP_GET_CUR_DISP_FMT, sizeof(UINT32), &addr);
    rm_copy_from_user(&dst,pv_dst,sizeof(PARMA_GET_CUR_DISP_FMT));
    unlock_nfy_param_buf(PARMA_GET_CUR_DISP_FMT_NFY);
    *pe_disp_fmt = dst.e_disp_fmt;
    #endif
}

static VOID vid_pla_overscan_fct (
            UINT16                          ui2_vdp_id,
            DRV_CUST_INPUT_SRC_TYPE_T       e_src_type,
            DRV_CUSTOM_TIMING_INFO_T*       pt_src_timing,
            UINT16                          *pui2_up,       /* OUT */
            UINT16                          *pui2_down,     /* OUT */
            UINT16                          *pui2_left,     /* OUT */
            UINT16                          *pui2_right)    /* OUT */
{
    #if 1
    #else
    PARAM_VDP_OVERSCAN *pv_dst;
    UINT32 addr;
    int c;
    #endif
    PARAM_VDP_OVERSCAN dst;
    #if 1
  //  VDP_OVERSCAN_OUT_PARAM tOutOverscan;

    /*if ( in_interrupt( ) )
    {
        ASSERT(0);
    }*/
    #endif

    dst.ui2_vdp_id = ui2_vdp_id;
    dst.e_src_type = e_src_type;
    dst.t_src_timing = *pt_src_timing;
    dst.pf_nfy = _pfOverscanFct;

    #if 1
    dst.tOut.ui2_up = 0;
    dst.tOut.ui2_down = 0;
    dst.tOut.ui2_left = 0;
    dst.tOut.ui2_right = 0;    
    
    _CB_PutEvent_Adpt_Ext(CB_DRV_VDP_OVERSCAN, sizeof(PARAM_VDP_OVERSCAN), &dst, sizeof(VDP_OVERSCAN_OUT_PARAM), &(dst.tOut));  

    *pui2_up = dst.tOut.ui2_up;
    *pui2_down = dst.tOut.ui2_down;
    *pui2_left = dst.tOut.ui2_left;
    *pui2_right = dst.tOut.ui2_right;    
    #else
    lock_nfy_param_buf(PARAM_OVERSCAN_NFY);

    pv_dst = (PARAM_VDP_OVERSCAN *)get_nfy_param_buf(PARAM_OVERSCAN_NFY,sizeof(PARAM_VDP_OVERSCAN));
    c = rm_copy_to_user(pv_dst,&dst,sizeof(PARAM_VDP_OVERSCAN));
    addr = (UINT32)pv_dst;

    _CB_PutEvent_Adpt(CB_DRV_VDP_OVERSCAN, sizeof(UINT32), &addr);
    rm_copy_from_user(&dst,pv_dst,sizeof(PARAM_VDP_OVERSCAN));
    unlock_nfy_param_buf(PARAM_OVERSCAN_NFY);
    *pui2_up = dst.ui2_up;
    *pui2_down = dst.ui2_down;
    *pui2_left = dst.ui2_left;
    *pui2_right = dst.ui2_right;
    #endif
}

static VOID vid_pla_update_overscan_tbl (
            UINT16                          ui2_vdp_id,
            DRV_CUST_INPUT_SRC_TYPE_T       e_src_type,
            DRV_CUSTOM_TIMING_INFO_T*       pt_src_timing,
            UINT16                          ui2_up,
            UINT16                          ui2_down,
            UINT16                          ui2_left,
            UINT16                          ui2_right )
{
    #if 1
    #else
    PARAM_UPDATE_OVERSCAN *pv_dst;
    UINT32 addr;
    #endif

    PARAM_UPDATE_OVERSCAN dst;

    dst.ui2_vdp_id =  ui2_vdp_id;
    dst.e_src_type = e_src_type;
    dst.t_src_timing = *pt_src_timing;
    dst.ui2_up = ui2_up;
    dst.ui2_down = ui2_down;
    dst.ui2_left = ui2_left;
    dst.ui2_right = ui2_right;
    dst.pf_nfy = _pfUpdateOverscanTbl;

    #if 1
    _CB_PutEvent_Adpt(CB_DRV_VDP_UPDATE_OVERSCAN, sizeof(PARAM_UPDATE_OVERSCAN), &dst);
    #else
    lock_nfy_param_buf(PARAM_UPDATE_OVERSCAN_NFY);

    pv_dst = (PARAM_UPDATE_OVERSCAN *)get_nfy_param_buf(PARAM_UPDATE_OVERSCAN_NFY,sizeof(PARAM_UPDATE_OVERSCAN));
    rm_copy_to_user(pv_dst,&dst,sizeof(PARAM_UPDATE_OVERSCAN));
    addr = (UINT32)pv_dst;
    _CB_PutEvent_Adpt(CB_DRV_VDP_UPDATE_OVERSCAN, sizeof(UINT32), &addr);
    unlock_nfy_param_buf(PARAM_UPDATE_OVERSCAN_NFY);
    #endif
}
#endif

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

    /* check time shift super freeze status */
    if (fgVDPForceFreeze)
    {
        _fgVDPSuperFreezeDisc = FALSE;
        return (RMR_OK);
    }

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

    if (ucVdpId >= VDP_NS)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    if (_arMwVdp[ucVdpId].ucMemoMode)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

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
        _arMwVdp[ucVdpId].u2SrcCompId = prMpvCompReg->ui2_id;
        ucEsId = _arMwComp2EsId[_arMwVdp[ucVdpId].u2SrcCompId];
        _arMwVdp[ucVdpId].ucEsId = ucEsId;        
        
        if (_SetInput(ucVdpId) != VDP_SET_OK)
        {
           return (RMR_DRV_INV_CONN_INFO);
        }

#if 0
        // HD B2R

        if (bApiVideoSetVideoSrc(ucVdpId, SV_VS_DT1) == 0)
        {
            return (RMR_DRV_INV_CONN_INFO);
        }
#else
#ifdef CC_B2R_SKYPE_SUPPORT
        if(ucVdpId == 0)
        {
            if (bApiVideoSetVideoSrc(ucVdpId, SV_VS_DT1) == 0)
            {
                return (RMR_DRV_INV_CONN_INFO);
            }            
        }
        else
        {
            if (bApiVideoSetVideoSrc(ucVdpId, SV_VS_DT2) == 0)
            {
                return (RMR_DRV_INV_CONN_INFO);
            }        
        }
#else
        //u4DmxComponentType = _GetDmxComponentType(_arMwVdp[ucVdpId].ucEsId);

        if(_arMwVdp[ucVdpId].ucEsId == ES0)
        {
            if (bApiVideoSetVideoSrc(ucVdpId, SV_VS_DT1) == 0)
            {
                return (RMR_DRV_INV_CONN_INFO);
            }            
        }
        else
        {
            if (MPV_HD((MPV_COMP_ID_T)_arMwVdp[ucVdpId].u2SrcCompId))
            {
                // HD B2R
                if (bApiVideoSetVideoSrc(ucVdpId, SV_VS_DT1) == 0)
                {
                    return (RMR_DRV_INV_CONN_INFO);
                }
            }
            else
            {
                if(ucVdpId == VDP_2)
                {
                    if (bApiVideoSetVideoSrc(ucVdpId, SV_VS_DT2) == 0)
                    {
                        return (RMR_DRV_INV_CONN_INFO);
                    }
                }
            }
        }
#endif        
#endif
        _arMwVdp[ucVdpId].ucDtv = 1;
    }
    else
    if (prMpvCompReg->e_type == DRVT_TV_DEC)
    {
        DRV_TYPE_T e_type, e_src_type;
        UINT16 ui2_id, ui2_src_id;
        TV_DEC_TIMING_TYPE_T e_timing;
        extern BOOL avc_scart_is_from_tuner_src(
            DRV_TYPE_T e_type, UINT16 ui2_comp_id,
            DRV_TYPE_T* pe_src_type, UINT16* pui2_src_comp_id );

        _arMwVdp[ucVdpId].u2TvdCompId = prMpvCompReg->ui2_id;
        TvdNfyVdpResChgRegister(prMpvCompReg->ui2_id, ucVdpId);

        TvdGetCurrInputSrcInfo(prMpvCompReg->ui2_id, &e_type, &ui2_id, &e_timing);

        /* If source is SCART, should check if the real source is TUNER */
        if ( e_type == DRVT_AVC_SCART &&
             avc_scart_is_from_tuner_src( e_type, ui2_id, &e_src_type, &ui2_src_id ) )
        {
            _arMwVdp[ucVdpId].e_type = e_src_type;
            _arMwVdp[ucVdpId].e_timing = e_timing;
        }
        else
        {
            _arMwVdp[ucVdpId].e_type = e_type;
            _arMwVdp[ucVdpId].e_timing = e_timing;
        }

        _arMwVdp[ucVdpId].ucDtv = 0;

        // According to Alec, LH, Martin, 20070423
        DRV_CUSTOM_SCART_PIN_8_T t_scart_pin_8;

        _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_UNKNOWN;
        _VdpXlatDispFmtCallBackMw(ucVdpId);

        t_scart_pin_8 = (DRV_CUSTOM_SCART_PIN_8_T) MWGetScartPin8Status(ucVdpId);
	if((UINT32)t_scart_pin_8 != (UINT32)AVC_SCART_PIN_8_UNKNOWN)
        {
            _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_PAL_SECAM_WSS_SCART;
            _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.e_scart_pin_8 = t_scart_pin_8;
            _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.ui2_pal_secam_wss = 0;
            _ui2_pal_secam_wss = 0;
            _MW_VDP_SCART_PIN_8_CB(ucVdpId, t_scart_pin_8);
        }

    }
    else
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    MW_VDP_CallbackTrigger(ucVdpId);

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

    /* check time shift super freeze status */
    if (fgVDPForceFreeze)
    {
        /* record disconnect parameter for CR 212789 */
        _fgVDPSuperFreezeDisc = TRUE;
        _rVDPDiscPara.t_comp_id = *pt_comp_id;
        _rVDPDiscPara.e_disc_type = e_disc_type;
        _rVDPDiscPara.t_disc_info = *(DRV_COMP_ID_T*)pv_disc_info;
        _rVDPDiscPara.z_disc_info_len = z_disc_info_len;
        return (RMR_OK);
    }
    else
    {
        _fgVDPSuperFreezeDisc = FALSE;
    }

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
    if (_arMwVdp[ucVdpId].u2TvdCompId < TVD_COMP_NS)
    {
        TvdNfyVdpResChgRegister(_arMwVdp[ucVdpId].u2TvdCompId, VDP_COMP_NS);
    }
    _arMwVdp[ucVdpId].u2TvdCompId = (UINT16) TVD_COMP_NS;
    _arMwVdp[ucVdpId].u2SrcCompId = (UINT16) MPV_COMP_NS;
    _arMwVdp[ucVdpId].ucEsId = 255;
    _arMwVdp[ucVdpId].e_type = DRVT_UNKNOWN;
    _arMwVdp[ucVdpId].ucSrcChgNfy = 0;

    _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_UNKNOWN;
    _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.ui2_pal_secam_wss = 0;
    _ui2_pal_secam_wss = 0;
    _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.e_scart_pin_8 = DRV_CUSTOM_SCART_PIN_8_UNKNOWN;

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
        case VID_PLA_GET_TYPE_3D_MJC_ENABLE:
        {
            UINT8* peArg;
            peArg = (UINT8*) pv_get_info;

            if (*pz_get_info_len < sizeof(UINT8))
            {
                i4ReturnValue = RMR_DRV_INV_GET_INFO;
            }
            else
            {
                *pz_get_info_len = sizeof(UINT8);
                *peArg = u1DrvTDTVMJCEnableQuery();
            }
		}
		break;
		
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
                        prArg->ui1_r = (UINT8)((u4Bg >> 16) & 0xFF);
                        prArg->ui1_g = (UINT8)((u4Bg >> 8) & 0xFF);
                        prArg->ui1_b = (UINT8)(u4Bg & 0xFF);
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
                    VDP_DBG_MSG("get final disp region\n");
                    PrintfOutRegion(_arMwVdp[ucVdpId].rFinalOutput);
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
			
		case VID_PLA_GET_TYPE_OVERSCAN_ENABLE:
		{
			BOOL* pfgArg;
		
			pfgArg = (BOOL*) pv_get_info;
		
			if (*pz_get_info_len < sizeof(BOOL))
			{
				i4ReturnValue = RMR_DRV_INV_GET_INFO;
			}
			else
			{
				*pz_get_info_len = sizeof(BOOL);
                *pfgArg = VDP_IsSupportOverscan(ucVdpId);
			}
		}
		break;

        case VID_PLA_GET_TYPE_NON_LINEAR_SCALING:
            {
                BOOL* pfgArg;
        
                pfgArg = (BOOL*) pv_get_info;
        
                if (*pz_get_info_len < sizeof(BOOL))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(BOOL);
        
                    *pfgArg = VDP_IsSupportNonlinear(ucVdpId);
                }
            }
            break;



        case VID_PLA_GET_TYPE_3D_PANEL_TYPE:
        {
            VID_PLA_3D_PANEL_TYPE_T* prArg;
            prArg = (VID_PLA_3D_PANEL_TYPE_T*) pv_get_info;

            if (*pz_get_info_len < sizeof(VID_PLA_3D_PANEL_TYPE_T))
            {
                i4ReturnValue = RMR_DRV_INV_GET_INFO;
            }
            else
            {
                *pz_get_info_len = sizeof(VID_PLA_3D_PANEL_TYPE_T);
                *prArg = ((VID_PLA_3D_PANEL_TYPE_T) (VDP_GetPanelType()));
            }
        }
        break;

        case VID_PLA_GET_TYPE_3D_FMT_CAPABILITY:
        {
            UINT32* prArg;
            prArg = (UINT32*) pv_get_info;

            if (*pz_get_info_len < sizeof(UINT32))
            {
                i4ReturnValue = RMR_DRV_INV_GET_INFO;
            }
            else
            {
                *pz_get_info_len = sizeof(UINT32);
                *prArg = ((UINT32) (u4DrvTDTV3DFMTCapQuery(ucVdpId)));
            }
        }
        break;

        case VID_PLA_GET_TYPE_3D_CTRL_CAPABILITY:
        {
            UINT32* prArg;
            prArg = (UINT32*) pv_get_info;

            if (*pz_get_info_len < sizeof(UINT32))
            {
                i4ReturnValue = RMR_DRV_INV_GET_INFO;
            }
            else
            {
                *pz_get_info_len = sizeof(UINT32);
                *prArg = ((UINT32) (u4DrvTDTV3DCtrlCapQuery(ucVdpId)));
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
                //#ifdef CC_SCPOS_FORCE_UNMUTE
                        VID_PLA_CAP_FORCE_UNMUTE_ON_NO_SIG |
                //#endif
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
        case VID_PLA_GET_TYPE_WHITE_STRETCH:
        case VID_PLA_GET_TYPE_BLACK_STRETCH:
        case VID_PLA_GET_TYPE_LCDIM:    
        case VID_PLA_GET_TYPE_3D_MODE:
        case  VID_PLA_GET_TYPE_3D_LR_SWITCH:     
        case VID_PLA_GET_TYPE_3D_PARALLAX:       
        case VID_PLA_GET_TYPE_3D_FLD_DEPTH:      
        case VID_PLA_GET_TYPE_3D_TO_2D: 
        case VID_PLA_GET_TYPE_3D_FPR: 
        case VID_PLA_GET_TYPE_3D_PROTRUDE_SCR:                       
        case VID_PLA_GET_TYPE_3D_DISTANCE_TV:                       
        case VID_PLA_GET_TYPE_3D_OSD_DEPTH:           
            {
                PE_ARG_TYPE ucArgType;
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

                    if (PE_GetArg(ucVdpId, ucArgType, &i2Val) != VDP_SET_OK)
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

        case VID_PLA_GET_TYPE_REGION_CAPABILITY:
            {
                VID_PLA_VID_REGION_CAPABILITY_T* prArg;
                prArg = (VID_PLA_VID_REGION_CAPABILITY_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_VID_REGION_CAPABILITY_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_VID_REGION_CAPABILITY_T);

                    switch (prArg->e_get_type)
                    {
                        case VID_PLA_GET_TYPE_DISP_REGION:
                            if (VDP_IsSupportDotByDot() && (ucVdpId == VDP_1))
                            {
                                prArg->b_is_enable = 1;
                            }
                            else
                            {
                                prArg->b_is_enable = 0;
                            }

                            prArg->ui4_height_max = VDP_MAX_REGION_HEIGHT;
                            prArg->ui4_height_min = 0;
                            prArg->ui4_width_max = VDP_MAX_REGION_WIDTH;
                            prArg->ui4_width_min = 0;
                            prArg->ui4_x_max = 0;
                            prArg->ui4_x_min = 0;
                            prArg->ui4_y_max = 0;
                            prArg->ui4_y_min = 0;
                            break;

                        case VID_PLA_GET_TYPE_SRC_REGION:
                            if (VDP_IsSupportDotByDot() && (ucVdpId == VDP_1))
                            {
                                prArg->b_is_enable = 1;
                            }
                            else
                            {
                                prArg->b_is_enable = 0;
                            }
                            
                            if (VDP_CheckScalerDisplayMode() == SV_FALSE) // non-scaler dispmode
                            {
                                prArg->ui4_height_max = VDP_MAX_REGION_HEIGHT;
                                prArg->ui4_height_min = 0;
                                prArg->ui4_width_max = VDP_MAX_REGION_WIDTH;
                                if (_arMwVdp[ucVdpId].rXlatSource.ui4_width)
                                {
                                    prArg->ui4_width_min = VDP_GetMinSrcRegionWidth() * VDP_MAX_REGION_WIDTH/_arMwVdp[ucVdpId].rXlatSource.ui4_width;
                                }
                                else
                                {
                                    prArg->ui4_width_min = VDP_GetMinSrcRegionWidth();
                                }
                                if (prArg->ui4_width_min >= VDP_MAX_REGION_WIDTH)
                                {
                                //    prArg->b_is_enable = 0;
                                    prArg->ui4_width_min = VDP_MAX_REGION_WIDTH;
                                }
                                prArg->ui4_x_max = 0;
                                prArg->ui4_x_min = 0;
                                prArg->ui4_y_max = 0;
                                prArg->ui4_y_min = 0;
                            }
                            else // Scaler display mode
                            {
                                prArg->ui4_height_max = VDP_MAX_REGION_HEIGHT;
                                prArg->ui4_height_min = VDP_MAX_REGION_HEIGHT;
                                prArg->ui4_width_max = VDP_MAX_REGION_WIDTH;
                                prArg->ui4_width_min = VDP_MAX_REGION_WIDTH;
                                prArg->ui4_x_max = 0;
                                prArg->ui4_x_min = 0;
                                prArg->ui4_y_max = 0;
                                prArg->ui4_y_min = 0;
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_MIN_MAX:
            {
                INT16 i2Min, i2Max;
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
                i2Min= (INT16)wDrvVideoGetPorch(ucVdpId,SV_HPORCH_MIN);
                i2Max = (INT16)wDrvVideoGetPorch(ucVdpId,SV_HPORCH_MAX);
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
				i2Min = (INT16)wDrvVideoGetPorch(ucVdpId,SV_VPORCH_MIN);
				i2Max = (INT16)wDrvVideoGetPorch(ucVdpId,SV_VPORCH_MAX);
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
		    default:
					    {
					    	PE_UI_RANGE_T PEuiRange;					    	
							PE_ARG_TYPE ucArgType = _VdpMwGetType2ArgType(prArg->e_get_type);
							bRet = PE_GetUiRange(ucVdpId, ucArgType, &PEuiRange);
							i2Min = PEuiRange.i4Min;
							i2Max = PEuiRange.i4Max;							
			break;
		}
					}

                    if (bRet != TRUE)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui4_min_value = (UINT32)i2Min;
                        prArg->ui4_max_value = (UINT32)i2Max;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_MAX:
            {
                PE_ARG_TYPE ucArgType;
                BOOL bRet;
                VID_PLA_MAX_INFO_T* prArg;

                prArg = (VID_PLA_MAX_INFO_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_MAX_INFO_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
					PE_UI_RANGE_T PEUiRange;                
                    *pz_get_info_len = sizeof(VID_PLA_MAX_INFO_T);

                    ucArgType = _VdpMwGetType2ArgType(prArg->e_get_type);

                    bRet = PE_GetUiRange(ucVdpId, ucArgType, &PEUiRange);

                    if (bRet != TRUE)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui4_max_value = (UINT32)PEUiRange.i4Max;
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
                    if (PE_GetArg(ucVdpId, PE_ARG_R_GAIN, &i2Gain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui2_r_gain = (UINT16)i2Gain;
                    }

                    // Get G Gain
                    if (PE_GetArg(ucVdpId, PE_ARG_G_GAIN, &i2Gain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui2_g_gain = (UINT16)i2Gain;
                    }

                    // Get B Gain
                    if (PE_GetArg(ucVdpId, PE_ARG_B_GAIN, &i2Gain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui2_b_gain = (UINT16)i2Gain;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_COLOR_GAIN_MAX:
        case VID_PLA_GET_TYPE_COLOR_OFFSET_MAX:
            {
            	PE_UI_RANGE_T PEUiRange;
                PE_ARG_TYPE ucArgType;
                BOOL bRet;
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

                    bRet = PE_GetUiRange(ucVdpId, ucArgType, &PEUiRange);

                    if (bRet != TRUE)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui2_r_gain = (UINT16)PEUiRange.i4Max;
                        prArg->ui2_g_gain = (UINT16)PEUiRange.i4Max;
                        prArg->ui2_b_gain = (UINT16)PEUiRange.i4Max;
                    }
                }
            }
            break;

        case VID_PLA_GET_TYPE_COLOR_GAIN_MIN:
        case VID_PLA_GET_TYPE_COLOR_OFFSET_MIN:
            {
				PE_UI_RANGE_T PEUiRange;
                PE_ARG_TYPE ucArgType;
                BOOL bRet;
                INT16 i2Min;
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
                    bRet = PE_GetUiRange(ucVdpId, ucArgType, &PEUiRange);
					i2Min = (INT16)PEUiRange.i4Min;

                    if (bRet != TRUE)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui2_r_gain = (UINT16)i2Min;
                        prArg->ui2_g_gain = (UINT16)i2Min;
                        prArg->ui2_b_gain = (UINT16)i2Min;
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
                    if (PE_GetArg(ucVdpId, PE_ARG_R_OFFSET, &i2Offset) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui2_r_gain = (UINT16)i2Offset;
                    }

                    // Get G Offset
                    if (PE_GetArg(ucVdpId, PE_ARG_G_OFFSET, &i2Offset) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui2_g_gain = (UINT16)i2Offset;
                    }

                    // Get B Offset
                    if (PE_GetArg(ucVdpId, PE_ARG_B_OFFSET, &i2Offset) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                    else
                    {
                        prArg->ui2_b_gain = (UINT16)i2Offset;
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
                INT16 u1FilmOnOff;

                pucArg = (UCHAR*) pv_get_info;
                PE_GetArg(ucVdpId ,PE_ARG_DI_FILM_MODE, &u1FilmOnOff);

                if (*pz_get_info_len < sizeof(BOOL))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(BOOL);
                    *pucArg = u1FilmOnOff ;
                }
            }
            break;

	case VID_PLA_GET_TYPE_H_POSITION:
	{
		 INT32 pos,min_off,max,min,def;

		 def=(INT32)wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_DEFAULT);
		 max=(INT32)wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_MAX);
		 min=(INT32)wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_MIN);
		 pos=(INT32)wDrvVideoGetPorch((UINT8)ucVdpId,SV_HPORCH_CURRENT);

		 if (bDrvGetPorchTune((UINT8)ucVdpId) == SV_PORCHTUNE_SCPOS)//for DTV
		 {
			*(UINT16*)pv_get_info=(UINT16)MAX(32+pos-def,0);
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
			 *(UINT16*)pv_get_info = (UINT16)pos;
		 }
		 *pz_get_info_len = sizeof(UINT16);
	 }
	    break;	   
	case VID_PLA_GET_TYPE_V_POSITION:
	    if (bDrvGetPorchTune((UINT8)ucVdpId) == SV_PORCHTUNE_SCPOS)//for DTV
	     {
		    UINT16 def,pos;
		    def=wDrvVideoGetPorch((UINT8)ucVdpId, SV_VPORCH_DEFAULT);
		    pos=wDrvVideoGetPorch((UINT8)ucVdpId, SV_VPORCH_CURRENT);
		    *(UINT16*)pv_get_info = (UINT16)MAX(16+pos-def,0);
	     }
	     else
	     {
	     	*(UINT16*)pv_get_info =wDrvVideoGetPorch(ucVdpId,SV_VPORCH_CURRENT);
	     }
	     *pz_get_info_len = sizeof(UINT16);
	    break;

        case VID_PLA_GET_TYPE_FORCE_UNMUTE_ON_NO_SIG:
        //#ifdef CC_SCPOS_FORCE_UNMUTE
            if ( !pz_get_info_len || *pz_get_info_len < sizeof(BOOL) ||
                 !pv_get_info )
            {
                i4ReturnValue = RMR_DRV_INV_SET_INFO;
                break;
            }
            *(BOOL*)pv_get_info = fgVDPForceUnmute;
        //#else
        //    i4ReturnValue = RMR_DRV_INV_SET_INFO;
        //#endif
            break;

	case VID_PLA_GET_TYPE_HDMI_MODE:

		 switch(u1HdmiColorMode)
	     {
			case SV_HDMI_MODE_GRAPHIC:
				*(UINT8*)pv_get_info = VID_PLA_HDMI_MODE_GRAPHIC ;
			break;
			case SV_HDMI_MODE_VIDEO:
				*(UINT8*)pv_get_info = VID_PLA_HDMI_MODE_VIDEO ;
			break;
			case SV_HDMI_MODE_AUTO:
			default:
				*(UINT8*)pv_get_info = VID_PLA_HDMI_MODE_AUTO ;
			break;
	     }
	     *pz_get_info_len = sizeof(UINT8);
	    break;	    
    case VID_PLA_GET_TYPE_ENHANCE:
    case VID_PLA_GET_TYPE_QV_INP:
    case VID_PLA_GET_TYPE_LB_DETECT_CONFIG:
        {
            VID_PLA_LB_DETECT_CONFIG_T* peArg;
            peArg = (VID_PLA_LB_DETECT_CONFIG_T*) pv_get_info;

            if (*pz_get_info_len < sizeof(VID_PLA_LB_DETECT_CONFIG_T))
            {
                i4ReturnValue = RMR_DRV_INV_GET_INFO;
            }
            else
            {
                *pz_get_info_len = sizeof(VID_PLA_LB_DETECT_CONFIG_T);
                *peArg = _arMwVdp[ucVdpId].rLBConfig;
            }
        }
        break;    
    case VID_PLA_GET_TYPE_MODE_CAPABILITY:
        {
            UCHAR ucArg;
            switch( *(VID_PLA_MODE_T*)pv_get_info )
            {
                case VID_PLA_NORMAL:
                case VID_PLA_BLANK:
                case VID_PLA_QUAD_VIDEO:
                case VID_PLA_DEINT:
                default:
                    break;
                case VID_PLA_FREEZE:
                    VDP_GetFreezable(ucVdpId, &ucArg);
                    if(ucArg == SV_FALSE)
                    {
                        i4ReturnValue = RMR_DRV_GET_FAILED;
                    }
                    break;
            }
        }
        break;

        case VID_PLA_GET_TYPE_PICTURE_INFO:
            {
                VID_PLA_PICTURE_INFO_T* prArg;
                VDP_PIC_INFO_T rVdpPicInfo;
                #ifdef ENABLE_MULTIMEDIA
                UINT32 u4Tmp;
                UINT64 u8Tmp;
                #endif

                prArg = (VID_PLA_PICTURE_INFO_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_PICTURE_INFO_T))
                {
                    i4ReturnValue = RMR_DRV_NOT_ENOUGH_SPACE;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_PICTURE_INFO_T);

                    if (VDP_GetPicInfo(ucVdpId, &rVdpPicInfo) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                        #ifdef ENABLE_MULTIMEDIA
#ifdef CC_53XX_SWDMX_V2
                        if(SWDMX_GetInfo(SWDMX_PRIMARY_SOURCE, eSWDMX_CONTAINER_TYPE, &u4Tmp, 0, 0))
#else
                        if(SWDMX_GetInfo(eSWDMX_CONTAINER_TYPE, &u4Tmp, 0, 0))
#endif
                        {
                            if((u4Tmp == (UINT32)SWDMX_FMT_H264_VIDEO_ES) ||
                               (u4Tmp == (UINT32)SWDMX_FMT_MPEG2_VIDEO_ES) ||
                               (u4Tmp == (UINT32)SWDMX_FMT_MPEG4_VIDEO_ES) ||
                               (u4Tmp == (UINT32)SWDMX_FMT_VC1_ES))
                            {
#ifdef CC_53XX_SWDMX_V2
                                if(SWDMX_GetInfo(SWDMX_PRIMARY_SOURCE, eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8Tmp, 0, 0))
#else
                                if(SWDMX_GetInfo(eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8Tmp, 0, 0))
#endif
                                {
                                    prArg->ui8_offset = u8Tmp;
                                    i4ReturnValue = RMR_OK;
                                }
                            }
                        }
                        #endif
                    }
                    else
                    {
                        prArg->ui8_offset = rVdpPicInfo.u8OffsetDisp;
                    }

                    if (prArg->e_type == VID_PLA_PIC_INFO_TYPE_PICTURE)
                    {
                        prArg->u.ui8_pts = (UINT64)rVdpPicInfo.u4Pts;
                        #ifdef ENABLE_MULTIMEDIA
#ifdef CC_53XX_SWDMX_V2
                        if(SWDMX_GetInfo(SWDMX_PRIMARY_SOURCE, eSWDMX_CONTAINER_TYPE, &u4Tmp, 0, 0))
#else
                        if(SWDMX_GetInfo(eSWDMX_CONTAINER_TYPE, &u4Tmp, 0, 0))
#endif
                        {
                            if(u4Tmp == (UINT32)SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                            {
                                prArg->u.ui8_pts = (UINT64)rVdpPicInfo.u4TickNum;
                            }
                        }
                        #endif
                    }
                    else if (prArg->e_type == VID_PLA_PIC_INFO_TYPE_GOP)
                    {
                        prArg->u.t_time_code.ui1_hours = rVdpPicInfo.ucHours;
                        prArg->u.t_time_code.ui1_minutes = rVdpPicInfo.ucMinutes;
                        prArg->u.t_time_code.ui1_seconds = rVdpPicInfo.ucSeconds;
                        prArg->u.t_time_code.ui1_pictures = rVdpPicInfo.ucPictures;
                        prArg->u.t_time_code.i4_total_dur_adj = (INT32)rVdpPicInfo.u4TotlaTimeOffset;
                    }
                    else
                    {
                        i4ReturnValue = RMR_DRV_INV_GET_INFO;
                    }
                }
            }
            break;

#ifdef CC_MT5391
        case VID_PLA_GET_TYPE_SUPER_FREEZE:
            {
                VID_PLA_SUPER_FREEZE_MODE_T* peArg;
                peArg = (VID_PLA_SUPER_FREEZE_MODE_T*) pv_get_info;

                if (*pz_get_info_len < sizeof(VID_PLA_SUPER_FREEZE_MODE_T))
                {
                    i4ReturnValue = RMR_DRV_INV_GET_INFO;
                }
                else
                {
                    *pz_get_info_len = sizeof(VID_PLA_SUPER_FREEZE_MODE_T);

                    if (_arMwVdp[ucVdpId].ucMemoMode == 0)
                    {
                        *peArg = VID_PLA_SUPER_FREEZE_MODE_DISABLE;
                    }
                    else if (VDP_GetMemoMode())
                    {
                        *peArg = VID_PLA_SUPER_FREEZE_MODE_ENABLE;
                    }
                    else
                    {
                        *peArg = VID_PLA_SUPER_FREEZE_MODE_FREEZING;
                    }
                }
            }
            break;
#endif
#ifdef CC_MT5360
	 case VID_PLA_GET_TYPE_HDCP_KEY_EXIST:
            {
                BOOL* peArg;
                peArg = (BOOL*) pv_get_info;
		  if(bHDMIHDCPKeyCheck())
		  {
			*peArg = 1;
		  }
		  else
		  {
		  	 *peArg = 0;
		  }
            }
            break;
#endif
#ifdef CC_FLIP_MIRROR_SUPPORT
		case VID_PLA_GET_TYPE_FLIP:
		{
			DTVCFG_T rDtvCfg;

			if(EEPDTV_GetCfg(&rDtvCfg))
			{
				i4ReturnValue = RMR_DRV_INV_SET_INFO;
			}
                    if(rDtvCfg.u1Flags2 &DTVCFG_FLAG2_FLIP_ON)
                    {
                        *(BOOL*)pv_get_info = 1 ;
                    }
                    else
                    {
                        *(BOOL*)pv_get_info = 0 ;
                    }
		}
			break;

		case VID_PLA_GET_TYPE_MIRROR:
		{
			DTVCFG_T rDtvCfg;

			if(EEPDTV_GetCfg(&rDtvCfg))
			{
				i4ReturnValue = RMR_DRV_INV_SET_INFO;
			}
                    if(rDtvCfg.u1Flags2 &DTVCFG_FLAG2_MIRROR_ON)
                    {
                        *(BOOL*)pv_get_info = 1 ;
                    }
                    else
                    {
                        *(BOOL*)pv_get_info = 0 ;
                    }
               }
			break;
#endif
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5363)
				case VID_PLA_GET_TYPE_DRAMINF:
					{
						CAP_INPUT_T* peArg;
						DRAM_INFO_T draminf ;
						peArg = (CAP_INPUT_T*) pv_get_info;
		
						if (*pz_get_info_len < sizeof(CAP_INPUT_T))
						{
							i4ReturnValue = RMR_DRV_INV_GET_INFO;
						}
						else
						{
							*pz_get_info_len = sizeof(CAP_INPUT_T);
							vScpipGetDramInf(0,&draminf);
							peArg->ui1_data_src = draminf.scaler_inf.dram_from;
							if(draminf.scaler_inf.dram_from == 3)               //data from scaler
							{
								peArg->puac_src_buffer = (UCHAR*) draminf.scaler_inf.startAddr;
								peArg->e_color_format = (CAP_COLOR_FORMAT_T)draminf.scaler_inf.format;
								peArg->ui4_buffer_length = draminf.scaler_inf.dramSize;
								peArg->ui4_src_height = draminf.scaler_inf.src_height;
								peArg->ui4_src_width = draminf.scaler_inf.src_width;
								peArg->ui4_src_pitch = draminf.scaler_inf.data_length;
							}
							else if(draminf.scaler_inf.dram_from == 2)          //data from DI
							{
								peArg->ui1_block_mode = draminf.DI_inf.u1BlockMode;
								peArg->ui1_out_mode = draminf.DI_inf.u1OutMode;
								peArg->ui1_bit_res = draminf.DI_inf.u1BitRes;
								peArg->ui4_src_height = draminf.DI_inf.u2Height;
								peArg->ui4_src_width = draminf.DI_inf.u2Width;
								peArg->ui4_y_start_msb_add = draminf.DI_inf.u4YStartMSBAddr;
								peArg->ui4_y_start_lsb_add = draminf.DI_inf.u4YStartLSBAddr;
								peArg->ui4_y_field_size_msb = draminf.DI_inf.u4YFieldSizeMSB;
								peArg->ui4_y_field_size_lsb = draminf.DI_inf.u4YFieldSizeLSB;
								peArg->ui4_u_start_msb_add = draminf.DI_inf.u4UStartLSBAddr;
								peArg->ui4_u_start_lsb_add = draminf.DI_inf.u4YStartLSBAddr;
								peArg->ui4_u_field_size_msb = draminf.DI_inf.u4UFieldSizeMSB;
								peArg->ui4_u_field_size_lsb = draminf.DI_inf.u4UFieldSizeLSB;
							}
							else if(draminf.scaler_inf.dram_from == 1)          //data from B2R
							{
								peArg->ui1_block_mode = draminf.B2R_inf.u1BlockMode;
								peArg->ui1_out_mode = draminf.B2R_inf.u1OutMode;
								peArg->ui4_src_pitch = draminf.B2R_inf.u4Pitch;
								peArg->ui4_u_start_msb_add = draminf.B2R_inf.u4CStart;
								peArg->ui4_src_height = draminf.B2R_inf.u4Height;
								peArg->ui4_src_width = draminf.B2R_inf.u4With;
								peArg->ui4_y_start_msb_add = draminf.B2R_inf.u4YStart;
							}
							                                                   //else draminf.scaler_inf.dram_from ==0xF  , get fail !
						}
					}
					break;
#endif
#if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389) || defined(CC_MT5398)
				case VID_PLA_GET_TYPE_DRAMINF:
					{
							CAP_INPUT_T* peArg;
							DRAM_INFO_T draminf ;
							peArg = (CAP_INPUT_T*) pv_get_info;
		
							if (*pz_get_info_len < sizeof(CAP_INPUT_T))
							{
								i4ReturnValue = RMR_DRV_INV_GET_INFO;
							}
							else
							{
								*pz_get_info_len = sizeof(CAP_INPUT_T);
								 VDP_GetDramInf(peArg->ui1_video_path,&draminf);
								peArg->ui1_CfgFlipMirror = (draminf.scaler_inf.dram_from >> 4);
								peArg->ui1_data_src = draminf.scaler_inf.dram_from & 0x0F;
								if(peArg->ui1_data_src == 3)			   //data from scaler
								{
									peArg->ui1_out_mode = 1; //only progressive
									peArg->ui4_src_width = draminf.scaler_inf.PictureWidth;
									peArg->ui4_src_height = draminf.scaler_inf.PictureHeight;
									peArg->ui4_src_pitch = draminf.scaler_inf.DramPitch;							
									#ifdef LINUX_TURNKEY_SOLUTION
									peArg->ui4_u_start_lsb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.scaler_inf.ULsbAddr));
									peArg->ui4_u_start_msb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.scaler_inf.UMsbAddr));
									peArg->ui4_y_start_lsb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.scaler_inf.YLsbAddr));
									peArg->ui4_y_start_msb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.scaler_inf.YMsbAddr));
									peArg->ui4_v_start_lsb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.scaler_inf.VLsbAddr));
									peArg->ui4_v_start_msb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.scaler_inf.VMsbAddr));
									#else
									peArg->ui4_u_start_lsb_add = draminf.scaler_inf.ULsbAddr;
									peArg->ui4_u_start_msb_add = draminf.scaler_inf.UMsbAddr;
									peArg->ui4_y_start_lsb_add = draminf.scaler_inf.YLsbAddr;
									peArg->ui4_y_start_msb_add = draminf.scaler_inf.YMsbAddr;
									peArg->ui4_v_start_lsb_add = draminf.scaler_inf.VLsbAddr;
									peArg->ui4_v_start_msb_add = draminf.scaler_inf.VMsbAddr;
									#endif
									peArg->ui1_block_mode = draminf.scaler_inf.BlockMode;
									peArg->ui1_bit_res = draminf.scaler_inf.u1BitRes;
									peArg->ui1_color_mode = draminf.scaler_inf.u1ColorMode;
									//peArg->ui1_AF_Bottom = draminf.scaler_inf.u1AFBottom;
									//peArg->ui1_AF_YUVIndex = draminf.scaler_inf.u1AFYUVIndex;
									peArg->ui1_y_frame_num = draminf.scaler_inf.FrameNum;
									peArg->ui1_c_frame_num = draminf.scaler_inf.FrameNum;
									//peArg->ui1_CfgFlipMirror = draminf.scaler_inf.u1CfgFlipMirror;
									//peArg->ui1_DramFlipMirror = draminf.scaler_inf.u1DramFlipMirror;
								}
								else if(peArg->ui1_data_src == 1)		   //data from B2R
								{
									peArg->ui1_out_mode = draminf.B2R_inf.u1OutMode;
									peArg->ui4_src_pitch = draminf.B2R_inf.u4Pitch;
									peArg->ui4_src_height = draminf.B2R_inf.u4Height;
									peArg->ui4_src_width = draminf.B2R_inf.u4With;
									#ifdef LINUX_TURNKEY_SOLUTION
									peArg->ui4_y_start_msb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.B2R_inf.u4YStart));
									peArg->ui4_u_start_msb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.B2R_inf.u4CStart));
									#else
									peArg->ui4_y_start_msb_add = draminf.B2R_inf.u4YStart;
									peArg->ui4_u_start_msb_add = draminf.B2R_inf.u4CStart;
									#endif
								}
								else if (peArg->ui1_data_src == 2)	   //data from DI
								{
									peArg->ui1_out_mode = draminf.DI_inf.u1OutMode;
									peArg->ui4_src_width = draminf.DI_inf.u2Width;
									peArg->ui4_src_height = draminf.DI_inf.u2Height;
									peArg->ui4_src_pitch = draminf.DI_inf.u2DramPitch;
									#ifdef LINUX_TURNKEY_SOLUTION
									peArg->ui4_u_start_lsb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.DI_inf.u4UStartLSBAddr));
									peArg->ui4_u_start_msb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.DI_inf.u4UStartMSBAddr));
									peArg->ui4_y_start_lsb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.DI_inf.u4YStartLSBAddr));
									peArg->ui4_y_start_msb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.DI_inf.u4YStartMSBAddr));
									peArg->ui4_v_start_lsb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.DI_inf.u4VStartLSBAddr));
									peArg->ui4_v_start_msb_add = (UINT32)adapt_fbm_knl_to_usr(VIRTUAL(draminf.DI_inf.u4VStartMSBAddr));
									#else
									peArg->ui4_u_start_lsb_add = draminf.DI_inf.u4UStartLSBAddr;
									peArg->ui4_u_start_msb_add = draminf.DI_inf.u4UStartMSBAddr;
									peArg->ui4_y_start_lsb_add = draminf.DI_inf.u4YStartLSBAddr;
									peArg->ui4_y_start_msb_add = draminf.DI_inf.u4YStartMSBAddr;
									peArg->ui4_v_start_lsb_add = draminf.DI_inf.u4VStartLSBAddr;
									peArg->ui4_v_start_msb_add = draminf.DI_inf.u4VStartMSBAddr;
									#endif
									peArg->ui1_block_mode = draminf.DI_inf.u1BlockMode;
									peArg->ui1_bit_res = draminf.DI_inf.u1BitRes;
									peArg->ui1_color_mode = draminf.DI_inf.u1ColorMode;
									peArg->ui1_AF_Bottom = draminf.DI_inf.u1AFBottom;
									peArg->ui1_AF_YUVIndex = draminf.DI_inf.u1AFYUVIndex;
									peArg->ui1_y_frame_num = draminf.DI_inf.u1YFrameNum;
									peArg->ui1_c_frame_num = draminf.DI_inf.u1CFrameNUm;
									peArg->ui1_CfgFlipMirror = draminf.DI_inf.u1CfgFlipMirror;
									peArg->ui1_DramFlipMirror = draminf.DI_inf.u1DramFlipMirror;
								}
							}
						}
					break;
					
#endif
        case VID_PLA_GET_TYPE_DELAY_TIME:
        {
            UINT32 u4Delay;
            #ifdef CC_MT5395
            u4Delay = VDP_GetCmdDelay();
            #else
            u4Delay = 0;
            #endif
            *(UINT32*)pv_get_info = u4Delay;
        }
        break;

    default:
        i4ReturnValue = RMR_DRV_INV_GET_INFO;
        break;
    }

    VERIFY(x_sema_unlock(_hMutex) == OSR_OK);
    return (i4ReturnValue);
}

static INT32   _VdpSetDispRegion(UCHAR ucVdpId, const VOID* pv_set_info)
{
                VID_PLA_VID_REGION_T* prArg;
                VDP_REGION_T rOutRegion;
                INT32 i4ReturnValue = RMR_OK;

					if (ucVdpId >= VDP_COMP_NS)
					{
					   return (RMR_DRV_INV_SET_INFO);
					}
#if 1
                    VDP_DBG_MSG("vdp set disp region\n");
                    prArg = (VID_PLA_VID_REGION_T*) pv_set_info;

                    _arMwVdp[ucVdpId].rFullOutput = *prArg;

                    // Final = Translate + Full
                    _VdpCalculateOutputRegion(ucVdpId);

                    rOutRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_x;
                    rOutRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_y;
                    rOutRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_width;
                    rOutRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_height;
                    PrintfOutRegion(_arMwVdp[ucVdpId].rFinalOutput);

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
                    return i4ReturnValue ;


}

static INT32 _VdpSetDispFormat(UCHAR ucVdpId, const VOID* pv_set_info)
{
LINT_SUPPRESS_BRACE(549)
#if 1
      static UINT8 bOldFmt[VDP_COMP_NS] ={0xff, 0xff};
    UINT8 bMuteTime =0;
	VID_PLA_DISP_FMT_T eArg;
	eArg = (VID_PLA_DISP_FMT_T) (UINT32) pv_set_info;
	INT32 i4ReturnValue = (INT32)RMR_OK;

	if ((ucVdpId >= VDP_COMP_NS) || (!fgEnableMwCb))
	{
	   return (RMR_DRV_INV_SET_INFO);
	}

    _arMwVdp[ucVdpId].eFmt = eArg;
	VDP_DBG_MSG("_VdpSetDispFormat %d\n", _arMwVdp[ucVdpId].eFmt);

    bMuteTime = DRVCUST_OptGet(eFlagMuteDispFmtChg);
    if (bMuteTime &&(bOldFmt[ucVdpId] != _arMwVdp[ucVdpId].eFmt))
    {
            _vDrvVideoSetMute(MUTE_MODULE_MW_DISP_FMT, ucVdpId, bMuteTime, TRUE);
            bOldFmt[ucVdpId] = _arMwVdp[ucVdpId].eFmt;

            #ifdef CC_MT5395
            UNUSED(VDP_UpdateCmdDelay((UINT32)ucVdpId));
            #endif
            //Printf("==============\n _VdpSetDispFormat mute\n============\n");
    }
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

                if (_pfXlatDispFmtExFct[(UINT32)_arMwVdp[ucVdpId].eFmt] != NULL)
                {
                    VID_PLA_VID_REGION_T rSrcReg;
                    VID_PLA_VID_REGION_T rDispReg;
                    DRV_CUSTOM_VID_SRC_INFO_T rVidSrcInfo;

                    rVidSrcInfo.ui2_vdp_id = (UINT16)ucVdpId;
                    rVidSrcInfo.e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_INPUT_SRC;
                    rVidSrcInfo.u.t_input_src.e_input_source = _VdpGetDrvCustInputSrcType(ucVdpId, _arMwVdp[ucVdpId].e_type);
                    if (rVidSrcInfo.u.t_input_src.e_input_source == DRV_CUST_INPUT_SRC_TYPE_MULTIMEDIA)
                    {
                        // sys not ready for MM input source.
                        // once sys is ready for this input source, you can remove this.
                        rVidSrcInfo.u.t_input_src.e_input_source = DRV_CUST_INPUT_SRC_TYPE_DTV;
                    }
                    rVidSrcInfo.u.t_input_src.ui2_id = _arMwVdp[ucVdpId].u2SrcCompId;
                    rVidSrcInfo.u.t_input_src.e_timing_type = _arMwVdp[ucVdpId].e_timing;

                    if (rVidSrcInfo.u.t_input_src.e_input_source != DRV_CUST_INPUT_SRC_TYPE_UNKNOWN)
                    {
                        #ifndef LINUX_TURNKEY_SOLUTION
                        _pfXlatDispFmtExFct[(UINT32)_arMwVdp[ucVdpId].eFmt](
                            &_arMwVdp[ucVdpId].rSrcRes,
                            &_arMwVdp[ucVdpId].rOutRes,
                            &rVidSrcInfo,
                            &rSrcReg,
                            &rDispReg);
                        #else
                        set_vdp_ex_nfy(ucVdpId,
                            &_arMwVdp[ucVdpId].rSrcRes,
                            &_arMwVdp[ucVdpId].rOutRes,
                            &rVidSrcInfo,
                            &rSrcReg,
                            &rDispReg);
                        #endif
                    }
                }
#else
				VID_PLA_DISP_FMT_T eArg;
				eArg = (VID_PLA_DISP_FMT_T) (UINT32) pv_set_info;

                _arMwVdp[ucVdpId].eFmt = eArg;
#endif
                return  i4ReturnValue ;

}


static INT32 _VdpSetOverScan(UCHAR ucVdpId, const VOID* pv_set_info)
{
    VID_PLA_OVER_SCAN_CLIPPER_T* prArg;
    VDP_OVERSCAN_REGION_T rOverScan;
    INT32 i4ReturnValue = RMR_OK;


	if (ucVdpId >= VDP_COMP_NS)
	{
	   return (RMR_DRV_INV_SET_INFO);
	}

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

            eSrcType = _VdpGetDrvCustInputSrcType(ucVdpId, _arMwVdp[ucVdpId].e_type);
            /*check MM mode*/
#if 0
//AP could set overscan for MM
            if (eSrcType == DRV_CUST_INPUT_SRC_TYPE_MULTIMEDIA)
            {
                // someone don't want to use overscan.
                i4ReturnValue = RMR_OK;
                break;
            }
#endif
                  
            rTimingInfo.ui2_width = _arMwVdp[ucVdpId].rSrcRes.ui2_width;
            rTimingInfo.ui2_height = _arMwVdp[ucVdpId].rSrcRes.ui2_height;
            rTimingInfo.ui4_fram_rate = _arMwVdp[ucVdpId].u4FrameRate;
            rTimingInfo.e_color_sys = _VdpGetDrvCustColorSys(ucVdpId);
            rTimingInfo.e_timing_type = _arMwVdp[ucVdpId].e_timing;

            if (_arMwVdp[ucVdpId].u4Interlace)
            {
                rTimingInfo.e_scan_mode = DRV_CUSTOM_TIMING_SCAN_MODE_INTERLACE;
            }
            else
            {
                rTimingInfo.e_scan_mode = DRV_CUSTOM_TIMING_SCAN_MODE_PROGRESSIVE;
            }

            #ifndef LINUX_TURNKEY_SOLUTION
            _pfUpdateOverscanTbl(
                ucVdpId,
                eSrcType,
                &rTimingInfo,
                (UINT16)rOverScan.u4Top,
                (UINT16)rOverScan.u4Bottom,
                (UINT16)rOverScan.u4Left,
                (UINT16)rOverScan.u4Right
                );
            #else
            vid_pla_update_overscan_tbl(
                ucVdpId,
                eSrcType,
                &rTimingInfo,
                (UINT16)rOverScan.u4Top,
                (UINT16)rOverScan.u4Bottom,
                (UINT16)rOverScan.u4Left,
                (UINT16)rOverScan.u4Right
                );
            #endif
        }

        if (VDP_SetOverScan(ucVdpId,
            rOverScan) != VDP_SET_OK)
        {
            // if video plane is NOT connect (switch), it will return fail.
            // ignore it!
            // i4ReturnValue = RMR_DRV_INV_SET_INFO;
        }
    }
    return i4ReturnValue;
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

    /* check time shift super freeze status
    if (fgVDPForceFreeze)
    {
        switch (e_set_type)
        {
            case VID_PLA_SET_TYPE_H_POSITION:
            case VID_PLA_SET_TYPE_V_POSITION:
            case VID_PLA_SET_TYPE_H_SIZE:
            case VID_PLA_SET_TYPE_V_SIZE:
            case VID_PLA_SET_TYPE_DISP_REGION:
            case VID_PLA_SET_TYPE_SUPER_FREEZE:
                break;

            default:
                return RMR_OK;
        }
    }
    */

    /* This is better one to allow PQ settings */
    if (fgVDPForceFreeze)
    {
        switch (e_set_type)
        {
            case VID_PLA_SET_TYPE_CTRL:
            case VID_PLA_SET_TYPE_MODE:
                return RMR_OK;

            default:
                break;
        }
    }

    i4ReturnValue = RMR_OK;
    VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    VDP_DBG_MSG("VDP(%d) SET(%d) ARG(%x)\n",ucVdpId,  (UINT32) (e_set_type - VID_PLA_SET_TYPE_CTRL), (UINT32) pv_set_info);

    // Perform set opertion
    switch (e_set_type)
    {

        case VID_PLA_SET_TYPE_BRIGHTNESS:
        case VID_PLA_SET_TYPE_CONTRAST:
        case VID_PLA_SET_TYPE_HUE:
        case VID_PLA_SET_TYPE_SATURATION:
        case VID_PLA_SET_TYPE_SHARPNESS:
        case VID_PLA_SET_TYPE_BLENDING:
        case VID_PLA_SET_TYPE_CTI:
        case VID_PLA_SET_TYPE_ETI:
        case VID_PLA_SET_TYPE_COLOR_SUPPRESS:
        case VID_PLA_SET_TYPE_NR:
        case VID_PLA_SET_TYPE_BLACK_LVL_EXT:
        case VID_PLA_SET_TYPE_WHITE_PEAK_LMT:
        case VID_PLA_SET_TYPE_FLESH_TONE:
        case VID_PLA_SET_TYPE_LUMA:
        case VID_PLA_SET_TYPE_3D_NR:
        // new UI
        case VID_PLA_SET_TYPE_DI_FILM_MODE:
        case VID_PLA_SET_TYPE_DI_MA:
        case VID_PLA_SET_TYPE_DI_EDGE_PRESERVE:
        case VID_PLA_SET_TYPE_MJC_EFFECT:
        case VID_PLA_SET_TYPE_MJC_MODE:
        case VID_PLA_SET_TYPE_MJC_DEMO:
        case VID_PLA_SET_TYPE_XVYCC:
        case VID_PLA_SET_TYPE_WCG:
        case VID_PLA_SET_TYPE_BLUE_STRETCH:
        case VID_PLA_SET_TYPE_WHITE_STRETCH:
        case VID_PLA_SET_TYPE_BLACK_STRETCH:
        case VID_PLA_SET_TYPE_MPEG_NR:
        case VID_PLA_SET_TYPE_LCDIM:
        case VID_PLA_SET_TYPE_3D_MODE:
        case VID_PLA_SET_TYPE_3D_LR_SWITCH:
        case VID_PLA_SET_TYPE_3D_FLD_DEPTH:
        case VID_PLA_SET_TYPE_3D_TO_2D:                
        case VID_PLA_SET_TYPE_3D_FPR:                
        case VID_PLA_SET_TYPE_3D_PROTRUDE_SCR:           
        case VID_PLA_SET_TYPE_3D_DISTANCE_TV:           
        case VID_PLA_SET_TYPE_3D_OSD_DEPTH:            
	    {
LINT_SUPPRESS_BRACE(507)
            PE_ARG_TYPE ucArgType;
            UCHAR ucArg;

            ucArg = (UCHAR) (UINT32) pv_set_info;
            ucArgType = _VdpMwSetType2ArgType(e_set_type);

            if (PE_SetArg(ucVdpId, ucArgType, (INT16)ucArg) != VDP_SET_OK)
            {
                i4ReturnValue = RMR_DRV_INV_SET_INFO;
            }
        }
        break;
        case VID_PLA_SET_TYPE_GAME_MODE:
        {
            UINT8 u1OnOff;
            u1OnOff = (UINT8) (UINT32) pv_set_info;
            VDP_SetGameMode((UINT32)ucVdpId, u1OnOff);
            break;
        }
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
                    _ui2_pal_secam_wss = 0;
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

                // FIXME!
                // Mode change event is already blocking in driver layer under memo mode.
                // We have also to block this event in glue layer to keep glue layer and driver status sync.
                // In the future, this blocking checking should move to middleware, so that middleware status can sync with driver.
                // FIXME!
                if ((_arMwVdp[ucVdpId].ucMemoMode) && (ucVdpId == 0) && (eArg == VID_PLA_NORMAL || eArg == VID_PLA_DEINT  || eArg == VID_PLA_BLANK))
                {
                    break;
                }

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
        case VID_PLA_SET_TYPE_SRM_MODE:
            {
                VID_PLA_SRM_MODE_T eSrmMode;
                {
                    eSrmMode = (VID_PLA_SRM_MODE_T)(UINT32)(pv_set_info);
#if 1
                    if(eSrmMode == VID_PLA_SRM_MODE_OFF)
                    {
                       // VDP_ForceWriteOff(ucVdpId,0);
                        VDP_SetThumbnailMode(ucVdpId, 0);
                        SRM_SetMmMode(SRM_MM_MODE_TYPE_OFF);
                    }
                    else if(eSrmMode == VID_PLA_SRM_MODE_VIDEO)
                    {
                       // VDP_ForceWriteOff(ucVdpId,0);
                        VDP_SetThumbnailMode(ucVdpId, 0);
                        SRM_SetMmMode(SRM_MM_MODE_TYPE_VIDEO);
                    }
                    else if(eSrmMode == VID_PLA_SRM_MODE_THUMBNAIL)
                    {
						#ifdef MULTIMEDIA_AUTOTEST_SUPPORT 
							// VDP_ForceWriteOff(ucVdpId,0);
							VDP_SetThumbnailMode(ucVdpId, 0);
							SRM_SetMmMode(SRM_MM_MODE_TYPE_VIDEO);
						#else
						   // VDP_ForceWriteOff(ucVdpId,1);
							VDP_SetThumbnailMode(ucVdpId, 1);
							SRM_SetMmMode(SRM_MM_MODE_TYPE_THUMBNAIL);	
						#endif
                    }
#else
                    if (!b_Arg)
                    {
                        VDP_ForceWriteOff(ucVdpId,0);
                        VDP_SetThumbnailMode(ucVdpId, 0);
                    }
                    else
                    {
                        VDP_ForceWriteOff(ucVdpId,1);
                        VDP_SetThumbnailMode(ucVdpId, 1);
                    }
#endif
                 }
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
                if (pv_set_info == NULL)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
                else
                {
                    i4ReturnValue = _VdpSetDispRegion(ucVdpId, pv_set_info);
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
                    VDP_DBG_MSG("vdp set src region\n");
                    prArg = (VID_PLA_VID_REGION_T*) pv_set_info;

                    _arMwVdp[ucVdpId].rFullSource = *prArg;

                    // Final = Translate + Full
                    _VdpCalculateSourceRegion(ucVdpId);

                    rSrcRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_x;
                    rSrcRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_y;
                    rSrcRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_width;
                    rSrcRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalSource.ui4_height;

                    PrintfSrcRegion(_arMwVdp[ucVdpId].rFinalSource);

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

                if (pv_set_info == NULL)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
                else
                {
                    i4ReturnValue = _VdpSetOverScan(ucVdpId, pv_set_info);
                }
            }
            break;

        case VID_PLA_SET_TYPE_PIC_MODE:
        {
            if(DRVCUST_OptGet(eFlagFlashPqEnableQtySmartPic))
            {
                VID_PLA_PIC_MODE_T ePicMode = (VID_PLA_PIC_MODE_T)(UINT32)pv_set_info;
                VID_QA_MIN_MAX_DFT_T*  pt_min_max_dft_lst = NULL;
                INT32 i4Ret;
                UINT32 i4UiIndex;

                UNUSED(pv_set_info);
                UNUSED(pt_min_max_dft_lst);
                UNUSED(i4Ret);
                UNUSED(i4UiIndex);

//                vDrvSetSWGammaFlg(SV_ON);

                VDP_DBG_MSG("VID_PLA_SET_TYPE_PIC_MODE %d\n", ePicMode);

                /* sample code, wait for driver implementation */
                if (_pfVidQaTblQueryFct != NULL)
                {
					if(DRVCUST_OptGet(eFlagFlashPqQtySmartPicNewMode))
					{
						vApiFlashPqSetSmartPicId((UINT8)ePicMode);
						bApiFlashPqUpdateQtyDft();
						PE_SetArg(SV_VP_MAIN, PE_ARG_GAMMA,0);
					}
                    // FIXME
                    // FIXME
                    #ifndef LINUX_TURNKEY_SOLUTION
                    i4Ret = _pfVidQaTblQueryFct(
                        (UINT16)ucVdpId,
                        _arMwVdp[ucVdpId].e_type,
                        ePicMode,
                        &pt_min_max_dft_lst );

                    if (i4Ret == DCR_OK)
                    {
                    
						if(DRVCUST_OptGet(eFlagFlashPqQtySmartPicNewMode) == SV_FALSE)
						{
                        vApiFlashPqSetSmartPicId((UINT8)ePicMode);
                        bApiFlashPqUpdateQtyDft() ;
	                    }					

                        // update all source UI default table, since the table is same
                        for (i4UiIndex = 0; i4UiIndex < (UINT8)EVDP_VIS_MAX; i4UiIndex++)
                        {
                            _VdpParseMinMaxDftTbl(i4UiIndex, pt_min_max_dft_lst );
                        }
						if(DRVCUST_OptGet(eFlagFlashPqQtySmartPicNewMode) == SV_FALSE)
						{						
                        PE_SetArg(SV_VP_MAIN, PE_ARG_GAMMA,0);
						}

                        while (pt_min_max_dft_lst->e_vid_qa_type != VID_QA_TYPE_UNKNOWN)
                        {

                            VDP_DBG_MSG("VID TYPE %d\n", pt_min_max_dft_lst->e_vid_qa_type);
                            VDP_DBG_MSG("VID MIN %d\n", pt_min_max_dft_lst->i4_min);
                            VDP_DBG_MSG( "VID MAX %d\n", pt_min_max_dft_lst->i4_max);
                            VDP_DBG_MSG("VID DEFAULT %d\n\n", pt_min_max_dft_lst->i4_default);
                            pt_min_max_dft_lst++;
                        }
                    }
                    #endif

                }
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

                    rGain.ucRgain = (UINT16) prArg->ui2_r_gain;
                    rGain.ucGgain = (UINT16) prArg->ui2_g_gain;
                    rGain.ucBgain = (UINT16) prArg->ui2_b_gain;

                    // R Gain
                    if (PE_SetArg(ucVdpId, PE_ARG_R_GAIN, (INT16)rGain.ucRgain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    // G Gain
                    if (PE_SetArg(ucVdpId, PE_ARG_G_GAIN, (INT16)rGain.ucGgain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    // B Gain
                    if (PE_SetArg(ucVdpId, PE_ARG_B_GAIN, (INT16)rGain.ucBgain) != VDP_SET_OK)
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

                    rOffset.ucRgain = (UINT16) prArg->ui2_r_gain;
                    rOffset.ucGgain = (UINT16) prArg->ui2_g_gain;
                    rOffset.ucBgain = (UINT16) prArg->ui2_b_gain;

                    // R Offset
                    if (PE_SetArg(ucVdpId, PE_ARG_R_OFFSET, (INT16)rOffset.ucRgain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    // G Offset
                    if (PE_SetArg(ucVdpId, PE_ARG_G_OFFSET, (INT16)rOffset.ucGgain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }

                    // B Offset
                    if (PE_SetArg(ucVdpId, PE_ARG_B_OFFSET, (INT16)rOffset.ucBgain) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }
            }
            break;

		case VID_PLA_SET_TYPE_DISP_FMT:
			{
                        i4ReturnValue = _VdpSetDispFormat(ucVdpId, pv_set_info);
			}
			break;


		case VID_PLA_SET_TYPE_ATOMIC_CMD_GRP:
		{

		     if (pv_set_info == NULL)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                    else
                    {
                         VID_PLA_ATOMIC_CMD_T *prAtmoicCmd;
                         UINT8 u1CmdNum=0 ;

                         prAtmoicCmd = (VID_PLA_ATOMIC_CMD_T *) pv_set_info ;
                        do
                        {
                            switch(prAtmoicCmd-> e_drv_set_type)
                            {
                                case VID_PLA_SET_TYPE_DISP_FMT:
                                {
                                    i4ReturnValue = _VdpSetDispFormat(ucVdpId, prAtmoicCmd->u.pv_data);
                                }
                                break;

                                case VID_PLA_SET_TYPE_DISP_REGION:
                                {
                                    i4ReturnValue = _VdpSetDispRegion(ucVdpId, prAtmoicCmd->u.pv_data);
                                }
                                break;

                                case VID_PLA_SET_TYPE_OVER_SCAN_CLIPPER:
                                {
                                    i4ReturnValue = _VdpSetOverScan(ucVdpId, prAtmoicCmd->u.pv_data);
                                }
                                break;

                                case  VID_PLA_ATOMIC_CMD_CODE_END:
                                //command end
                                break;

                                default:
                                    i4ReturnValue = RMR_DRV_INV_SET_INFO;

                            }
                            prAtmoicCmd++ ;
                            if(++u1CmdNum > MAX_ATOMIC_CMD_NUM)
                            {
                                 i4ReturnValue = RMR_DRV_INV_SET_INFO;
                                break;
                            }
                        }while(prAtmoicCmd-> e_drv_set_type != VID_PLA_ATOMIC_CMD_CODE_END);
                    }
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

                PE_ARG_TYPE ucArgType;
                UCHAR ucArg;

                _arMwVdp[ucVdpId].eDemo = (VID_PLA_SPLIT_SCRN_DEMO_T) (UINT32) pv_set_info;

                ucArg = (UCHAR) (UINT32) pv_set_info;
                ucArgType = _VdpMwSetType2ArgType(e_set_type);

                if (PE_SetArg(ucVdpId, ucArgType, (INT16)ucArg) != VDP_SET_OK)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }

             }
            break;

  	    case VID_PLA_SET_TYPE_FILM_MODE:
             {

                BOOL b32OnOff;

                b32OnOff = (BOOL) (UINT32) pv_set_info;

                if (PE_SetArg(ucVdpId,PE_ARG_DI_FILM_MODE,(UINT8)b32OnOff) != VDP_SET_OK)
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
             }
            break;

        case VID_PLA_SET_TYPE_H_POSITION:
    	{
		INT32 pos,min_off,max,min,def;

		def=(INT32)wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_DEFAULT);
		max=(INT32)wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_MAX);
		min=(INT32)wDrvVideoGetPorch((UINT8)ucVdpId, SV_HPORCH_MIN);

		if (bDrvGetPorchTune((UINT8)ucVdpId) == SV_PORCHTUNE_SCPOS)//for DTV
		{
		       pos=(32-(UINT32)pv_set_info)*2+def;
			   if(pos > max)
				   pos = max ;
			   if(pos < min)
				   pos = min ;
		       wDrvVideoSetPorch(ucVdpId, SV_HPORCH_CURRENT, (UINT16)pos);
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
	    		wDrvVideoSetPorch(ucVdpId, SV_HPORCH_CURRENT, (UINT16)pos);
	    	//	_VDP_ScposPorchUpdate(ucVdpId,0); // reset scpos porch
			VDP_SetScposPorch(ucVdpId, SV_HPORCH_CURRENT, 1000);// reset scpos porch
	    	}
	}
            break;
        case VID_PLA_SET_TYPE_V_POSITION:
		if (bDrvGetPorchTune((UINT8)ucVdpId) == SV_PORCHTUNE_SCPOS)//for DTV
		{
			UINT16 def=wDrvVideoGetPorch(ucVdpId,SV_VPORCH_DEFAULT);
			wDrvVideoSetPorch(ucVdpId, SV_VPORCH_CURRENT, (UINT16)(INT32)pv_set_info-16+def);
		}
		else
		{
			wDrvVideoSetPorch(ucVdpId, SV_VPORCH_CURRENT, (UINT16)(UINT32)pv_set_info);
			//_VDP_ScposPorchUpdate(ucVdpId,0); // reset scpos porch
			VDP_SetScposPorch(ucVdpId, SV_VPORCH_CURRENT, 1000);// reset scpos porch
		}
            break;

        case VID_PLA_SET_TYPE_FORCE_UNMUTE_ON_NO_SIG:
        //#ifdef CC_SCPOS_FORCE_UNMUTE
            if ( z_set_info_len != sizeof(BOOL) )
            {
                i4ReturnValue = RMR_DRV_INV_SET_INFO;
                break;
            }
            if ( (BOOL)(UINT32)pv_set_info != TRUE &&
                 (BOOL)(UINT32)pv_set_info != FALSE )
            {
                i4ReturnValue = RMR_DRV_INV_SET_INFO;
                break;
            }
            //_vDrvVideoForceUnMute(ucVdpId, (BOOL)(UINT32)pv_set_info);
			vDrvSetForceUmute(ucVdpId, SV_FORCE_UNMUTE_TYPE_ON_NO_SIG, (BOOL)(UINT32)pv_set_info);
            fgVDPForceUnmute = (BOOL)(UINT32)pv_set_info;
            //Printf("fgVDPForceUnmute %d\n", (BOOL)(UINT32)pv_set_info);
        //#else
        //    i4ReturnValue = RMR_DRV_INV_SET_INFO;
        //#endif
            break;

        case VID_PLA_SET_TYPE_HDMI_MODE:
            {
			 UINT8 u1NewHdmiColorMode;
		     //VERIFY(x_sema_unlock(_hMutex) == OSR_OK); /* fix lock sequence of VDP & NPTV src switch */
			 switch((UINT32)pv_set_info)
		     {
				case VID_PLA_HDMI_MODE_GRAPHIC:
					u1NewHdmiColorMode = SV_HDMI_MODE_GRAPHIC ;
				break;
				case VID_PLA_HDMI_MODE_VIDEO:
					u1NewHdmiColorMode = SV_HDMI_MODE_VIDEO ;
				break;
				case VID_PLA_HDMI_MODE_AUTO:
				default:
					u1NewHdmiColorMode = SV_HDMI_MODE_AUTO ;
				break;
		     }
			if(u1NewHdmiColorMode != u1HdmiColorMode)
			{
				u1HdmiColorMode = u1NewHdmiColorMode ;
				vApiHdmiColorModeChg(ucVdpId);
			}
         }
	     //VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK); /* fix lock sequence of VDP & NPTV src switch */
	        break;
        case VID_PLA_SET_TYPE_ENHANCE:
        case VID_PLA_SET_TYPE_QV_INP:
        case VID_PLA_SET_TYPE_LB_DETECT_CONFIG:
        {
            VID_PLA_LB_DETECT_CONFIG_T* prArg;

            UINT32 u4Time;
            UINT32 u4Bound;
            prArg = (VID_PLA_LB_DETECT_CONFIG_T*) pv_set_info;
            u4Time = (UINT32) prArg->ui4_time_threshold;
            u4Bound = (UINT32) prArg->ui4_bounds_left;
            _arMwVdp[ucVdpId].rLBConfig.ui4_bounds_left = u4Bound;
            _arMwVdp[ucVdpId].rLBConfig.ui4_bounds_right = u4Bound;
            _arMwVdp[ucVdpId].rLBConfig.ui4_bounds_top= u4Bound;
            _arMwVdp[ucVdpId].rLBConfig.ui4_bounds_bottom= u4Bound;
            _arMwVdp[ucVdpId].rLBConfig.ui4_time_threshold = u4Time;

            if (VDP_LBoxDetectMode(ucVdpId, u4Time, u4Bound) != VDP_SET_OK)
            {
                i4ReturnValue = RMR_DRV_INV_SET_INFO;
            }
        }
            break;

        case VID_PLA_SET_TYPE_NFY_FCT:
            {
                VID_PLA_NFY_INFO_T* prArg;

                prArg = (VID_PLA_NFY_INFO_T*) pv_set_info;

         //   #ifndef LINUX_TURNKEY_SOLUTION
                _arMwVdp[ucVdpId].pfEosNfy = prArg->pf_vid_pla_nfy;
//            #else
    //            vdp_pla_nfy_tbl[ucVdpId] = prArg->pf_vid_pla_nfy;
//                _arMwVdp[ucVdpId].pfEosNfy = vdp_pla_nfy_fct;
//            #endif
                _arMwVdp[ucVdpId].pvEosTag = prArg->pv_tag;

                VDP_RegCbFunc(VDP_CB_FUNC_MM_COND_IND, (UINT32)_VdpEosCallback, 0, 0);
            }
            break;

        case VID_PLA_SET_TYPE_SUPER_BLANK:
            {
                VID_PLA_SUPER_BLANK_T *rSuperBlank;
                VDP_REGION_T rOutRegion;
                UINT32 u4Bg;

                rSuperBlank = (VID_PLA_SUPER_BLANK_T*)pv_set_info;

                if(rSuperBlank->b_enable != FALSE)
                {
                    _arMwVdp[ucVdpId].ucBlank = 1;
                    if (_VdpSetEnable(ucVdpId) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                    else
                    {
                        VDP_SetMode(ucVdpId, VDP_MODE_BG);
                        if(ucVdpId == VDP_1)
                        {
                            _rMChannel.bIsChannelOn = SV_TRUE;
                        }
                        else if(ucVdpId == VDP_2)
                        {
                            _rPChannel.bIsChannelOn = SV_TRUE;
                        }

                        _arMwVdp[ucVdpId].rFullOutput.ui4_x = rSuperBlank->t_disp_region.ui4_x;
                        _arMwVdp[ucVdpId].rFullOutput.ui4_y = rSuperBlank->t_disp_region.ui4_y;
                        _arMwVdp[ucVdpId].rFullOutput.ui4_width = rSuperBlank->t_disp_region.ui4_width;
                        _arMwVdp[ucVdpId].rFullOutput.ui4_height = rSuperBlank->t_disp_region.ui4_height;
                        _VdpCalculateOutputRegion(ucVdpId);
                        rOutRegion.u4X = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_x;
                        rOutRegion.u4Y = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_y;
                        rOutRegion.u4Width = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_width;
                        rOutRegion.u4Height = (UINT32) _arMwVdp[ucVdpId].rFinalOutput.ui4_height;
                        VDP_SetOutRegion(ucVdpId, 0, rOutRegion);

                        u4Bg = ((((UINT32)rSuperBlank->t_bg_color.ui1_r) << 16) + (((UINT32)rSuperBlank->t_bg_color.ui1_g) << 8) + ((UINT32)rSuperBlank->t_bg_color.ui1_b));
                        VDP_SetBg(ucVdpId, u4Bg);
                    }
                }
                else
                {
                    VDP_SetMode(ucVdpId, VDP_MODE_NORMAL);
                    _arMwVdp[ucVdpId].ucBlank = 0;
                    if (_VdpSetEnable(ucVdpId) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_INV_SET_INFO;
                    }
                }
            }
            break;
        case VID_PLA_SET_TYPE_COLORSPACE:
        {
            switch((UINT32)pv_set_info)
            {
                case VID_PLA_COLORSPACE_FORCE_RGB:
                    if(u1GetVGA422En() != SV_FALSE)
                    {
                        vVgaTriggerModeChange();
                        vSetVGA422En(SV_FALSE);
                    }
                    break;
                case VID_PLA_COLORSPACE_FORCE_YCBCR:
                    if(u1GetVGA422En() != SV_TRUE)
                    {
                        vVgaTriggerModeChange();
                        vSetVGA422En(SV_TRUE);
                    }
                    break;
                case VID_PLA_COLORSPACE_AUTO:
                default:
                    LOG(2, "Valid color space param for VGA mode \n");
                break;
            }
        }
        break;
#ifdef CC_FLIP_MIRROR_SUPPORT
		case VID_PLA_SET_TYPE_FLIP:
		{
			DTVCFG_T rDtvCfg;

			x_memset(&rDtvCfg, 0, sizeof(DTVCFG_T));

			if(EEPDTV_GetCfg(&rDtvCfg))
			{
				return RMR_DRV_INV_SET_INFO;
			}
			rDtvCfg.u1Flags2 &= ~(DTVCFG_FLAG2_FLIP_ON);

                    if((BOOL)(UINT32)pv_set_info)
                    {
                        rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_FLIP_ON);
                    }

			if(EEPDTV_SetCfg(&rDtvCfg))
			{
				i4ReturnValue = RMR_DRV_INV_SET_INFO;
			}
		}
			break;

		case VID_PLA_SET_TYPE_MIRROR:
		{
			DTVCFG_T rDtvCfg;

			x_memset(&rDtvCfg, 0, sizeof(DTVCFG_T));

			if(EEPDTV_GetCfg(&rDtvCfg))
			{
				return RMR_DRV_INV_SET_INFO;
			}
			rDtvCfg.u1Flags2 &= ~(DTVCFG_FLAG2_MIRROR_ON);

                    if((BOOL)(UINT32)pv_set_info)
                    {
                        rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_MIRROR_ON);
                    }

			if(EEPDTV_SetCfg(&rDtvCfg))
			{
				i4ReturnValue = RMR_DRV_INV_SET_INFO;
			}
		}
			break;
#endif

#ifdef DIVX_PLUS_CER
#ifdef CC_B2R_CHAPTER_CHG
        case VID_PLA_SET_TYPE_CHAP_INFO:
        {
            VDP_CHAPTER_T       rVdpChapter;
            VID_PLA_VID_CHAP_T  *prChapInfo = NULL ;

            x_memset(&rVdpChapter,0,sizeof(VDP_CHAPTER_T));
            
            if(NULL == pv_set_info)
            {
                i4ReturnValue = RMR_DRV_INV_SET_INFO;
                break;
            }
            prChapInfo = (VID_PLA_VID_CHAP_T *)pv_set_info;

            LOG(0," PLA_SET_TYPE_CHAP_INFO \n");
            
            rVdpChapter.fgIsPending     = prChapInfo->b_seek;
            rVdpChapter.u8StartPts      = prChapInfo->ui8_start_time;
            rVdpChapter.u8EndPts        = prChapInfo->ui8_end_time;
            rVdpChapter.fgFirstSetChapter = prChapInfo->b_first;
            if( TRUE != VDP_SetChpterEnable(&rVdpChapter))
            {
                i4ReturnValue = RMR_DRV_INV_SET_INFO;
            }
            break;
        }  
#endif
#endif

        case VID_PLA_SET_TYPE_SUPER_FREEZE:
            {
                VID_PLA_SUPER_FREEZE_T* ptSuperFreeze;

                ptSuperFreeze = (VID_PLA_SUPER_FREEZE_T*)pv_set_info;
                if (ptSuperFreeze->e_type == VID_PLA_SUPER_FREEZE_TYPE_MEMO)
                {
#ifdef CC_MT5391
                    /* SetMemoMode */
                    BOOL bMemoMode;

                    bMemoMode = ptSuperFreeze->b_enable;

                    if (VDP_SetMemoMode(bMemoMode) != VDP_SET_OK)
                    {
                        i4ReturnValue = RMR_DRV_CONN_FAILED;
                    }
                    else
                    {
                        _arMwVdp[ucVdpId].ucMemoMode = bMemoMode;

                        if (!bMemoMode)
                        {
                            MW_PMX_PORT_DISCONNECT(_arMwVdp[ucVdpId].ucPmxCompId, _arMwVdp[ucVdpId].ucLocalId);
                            _arMwVdp[ucVdpId].ucPmxCompId = PMX_COMP_NS;
                            _arMwVdp[ucVdpId].ucLocalId = PMX_MAX_INPORT_NS;
                        }
                    }
#endif
                }
                else if (ptSuperFreeze->e_type == VID_PLA_SUPER_FREEZE_TYPE_FORCE)
                {
                    /* for time shift super freeze */
                    if(ptSuperFreeze->b_enable)
                    {
                        fgVDPForceFreeze = TRUE;
                    }
                    else
                    {
                        fgVDPForceFreeze = FALSE;

                        /* disconnect first by recorded disconnect parameter for CR 212789 */
                        /* otherwise actual connect can't notify resolution change to mw */
                        if (_fgVDPSuperFreezeDisc)
                        {
                            VERIFY(x_sema_unlock(_hMutex) == OSR_OK);
                            _VdpDisconnect(&_rVDPDiscPara.t_comp_id,
                                           _rVDPDiscPara.e_disc_type,
                                           (const VOID*)&_rVDPDiscPara.t_disc_info,
                                           _rVDPDiscPara.z_disc_info_len);
                            return (i4ReturnValue);
                        }
                    }
                    /*todo: need set to vdp driver */
                }
                else
                {
                    i4ReturnValue = RMR_DRV_INV_SET_INFO;
                }
            }
            break;
		case VID_PLA_SET_TYPE_FORCE_UNMUTE:
			{
				VID_PLA_FORCE_UNMUTE_T *ptForceUmute;
				UINT8 u1UnmuteType=0xff ;

				ptForceUmute = (VID_PLA_FORCE_UNMUTE_T*)pv_set_info;
				 switch(ptForceUmute->e_force_unmute_type)
				 {
					case VID_PLA_FORCE_UNMUTE_TYPE_ON_NO_SIG:
						u1UnmuteType = SV_FORCE_UNMUTE_TYPE_ON_NO_SIG;
					break;
					case VID_PLA_FORCE_UNMUTE_TYPE_ATV_CHG_CH:
						u1UnmuteType = SV_FORCE_UNMUTE_TYPE_ATV_CHG_CH ;
					break;
					default:
						i4ReturnValue = RMR_DRV_INV_SET_INFO;
					break;
				 }
				if(i4ReturnValue != RMR_DRV_INV_SET_INFO)
				{
					vDrvSetForceUmute(ucVdpId, u1UnmuteType, ptForceUmute->b_enable);
				}
			}
			break;
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

#ifdef CC_3D_MM_DS_SUPPORT    
    if(_arMwComp3dInfo[_arMwVdp[0].u2SrcCompId] || _arMwComp3dInfo[_arMwVdp[1].u2SrcCompId])
    {
        ucEsId =0;
    }
    else
#endif    
    {
        ucEsId = _arMwVdp[ucVdpId].ucEsId;
    }        

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
    if (_arMwVdp[ucVdpId].ucConnect &&
        _arMwVdp[ucVdpId].ucEnable)
    {
        return VDP_SetEnable(ucVdpId, 1);
    }
    else if ((_arMwVdp[ucVdpId].ucConnect == 0) ||
             (_arMwVdp[ucVdpId].ucEnable == 0) )
    {
        return VDP_SetEnable(ucVdpId, 0);
    }

    return VDP_SET_OK;
}

//-----------------------------------------------------------------------------
/** Brief of _VdpMwGetType2ArgType.
 */
//-----------------------------------------------------------------------------
static PE_ARG_TYPE _VdpMwGetType2ArgType(UINT32 u4GetType)
{
    switch (u4GetType)
    {
        case VID_PLA_GET_TYPE_BRIGHTNESS:   // brightness
            return PE_ARG_BRIGHTNESS;

        case VID_PLA_GET_TYPE_CONTRAST:     // contrast
            return PE_ARG_CONTRAST;

        case VID_PLA_GET_TYPE_HUE:          // hue
            return PE_ARG_HUE;

        case VID_PLA_GET_TYPE_SATURATION:   // saturation
            return PE_ARG_SATURATION;

        case VID_PLA_GET_TYPE_CTI:          // cti
            return PE_ARG_CTI;

        case VID_PLA_GET_TYPE_SHARPNESS:    // sharpness (v, h)
            return PE_ARG_SHARPNESS;

        case VID_PLA_GET_TYPE_COLOR_GAIN:   // color gain
        case VID_PLA_GET_TYPE_COLOR_GAIN_MIN:   // color gain
        case VID_PLA_GET_TYPE_COLOR_GAIN_MAX:   // color gain
            return PE_ARG_R_GAIN;

        case VID_PLA_GET_TYPE_COLOR_OFFSET: // color offset
        case VID_PLA_GET_TYPE_COLOR_OFFSET_MIN: // color offset
        case VID_PLA_GET_TYPE_COLOR_OFFSET_MAX: // color offset
            return PE_ARG_R_OFFSET;

        case VID_PLA_GET_TYPE_NR:           // noise reduction
            return PE_ARG_NR;

        case VID_PLA_GET_TYPE_WHITE_PEAK_LMT:   // white peak limit
            return PE_ARG_WHITE_PEAK_LMT;

        case VID_PLA_GET_TYPE_FLESH_TONE:   // second color enhance
            return PE_ARG_SCE;

        case VID_PLA_GET_TYPE_LUMA:         // adaptive luma
            return PE_ARG_ADAPTIVE_LUMA;
        case VID_PLA_GET_TYPE_BLACK_STRETCH:
            return PE_ARG_BLACK_STRETCH;
        case VID_PLA_GET_TYPE_WHITE_STRETCH:
            return PE_ARG_WHITE_STRETCH;

        case VID_PLA_GET_TYPE_3D_NR:        // 3D noise reduction
            return PE_ARG_3D_NR;

        case VID_PLA_GET_TYPE_SPLIT_SCRN_DEMO:
            return PE_ARG_DEMO;

        case VID_PLA_GET_TYPE_LCDIM:
            return PE_ARG_LCDIM;                

        case VID_PLA_GET_TYPE_3D_MODE:
            return PE_ARG_TDTV_PACKING;       
            
        case VID_PLA_GET_TYPE_3D_LR_SWITCH:
            return PE_ARG_TDTV_INVERSE;
            
        case VID_PLA_GET_TYPE_3D_FLD_DEPTH:
            return PE_ARG_TDTV_DEPTH;        
            
        case VID_PLA_GET_TYPE_3D_TO_2D: 	        
            return PE_ARG_TDTV_3D_TO_2D;

        case VID_PLA_GET_TYPE_3D_FPR: 	        
            return PE_ARG_TDTV_FPR;
        
        case VID_PLA_GET_TYPE_3D_PROTRUDE_SCR:           
            return PE_ARG_TDTV_PROTRUDE;
            
        case VID_PLA_GET_TYPE_3D_DISTANCE_TV:           
            return PE_ARG_TDTV_DISTANCE;
            
        case VID_PLA_GET_TYPE_3D_OSD_DEPTH:           
            return PE_ARG_TDTV_OSD_DEPTH;

        
        default:
            return PE_ARG_NS;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VdpMwSetType2ArgType.
 */
//-----------------------------------------------------------------------------
static PE_ARG_TYPE _VdpMwSetType2ArgType(UINT32 u4SetType)
{
    switch (u4SetType)
    {
        case VID_PLA_SET_TYPE_BRIGHTNESS:   // brightness
            return PE_ARG_BRIGHTNESS;

        case VID_PLA_SET_TYPE_CONTRAST:     // contrast
            return PE_ARG_CONTRAST;

        case VID_PLA_SET_TYPE_HUE:          // hue
            return PE_ARG_HUE;

        case VID_PLA_SET_TYPE_SATURATION:   // saturation
            return PE_ARG_SATURATION;

        case VID_PLA_SET_TYPE_CTI:          // cti
            return PE_ARG_CTI;

        case VID_PLA_SET_TYPE_SHARPNESS:    // sharpness (v, h)
            return PE_ARG_SHARPNESS;

        case VID_PLA_SET_TYPE_NR:           // noise reduction
            return PE_ARG_NR;

        case VID_PLA_SET_TYPE_FLESH_TONE:   // second color enhance
            return PE_ARG_SCE;

        case VID_PLA_SET_TYPE_LUMA:         // adaptive luma
            return PE_ARG_ADAPTIVE_LUMA;
        case VID_PLA_SET_TYPE_BLACK_STRETCH:
            return PE_ARG_BLACK_STRETCH;
        case VID_PLA_SET_TYPE_WHITE_STRETCH:
            return PE_ARG_WHITE_STRETCH;

        case VID_PLA_SET_TYPE_WHITE_PEAK_LMT:   // white peak limit
            return PE_ARG_WHITE_PEAK_LMT;

        case VID_PLA_SET_TYPE_3D_NR:        // 3D noise reduction
            return PE_ARG_3D_NR;

        case VID_PLA_SET_TYPE_SPLIT_SCRN_DEMO:        // DEMO MODE
            return PE_ARG_DEMO;

	    case VID_PLA_SET_TYPE_DI_FILM_MODE:
	        return PE_ARG_DI_FILM_MODE;

	    case VID_PLA_SET_TYPE_DI_MA:
	        return PE_ARG_DI_MA;

	    case VID_PLA_SET_TYPE_DI_EDGE_PRESERVE:
	        return PE_ARG_DI_EDGE_PRESERVE;

	    case VID_PLA_SET_TYPE_MJC_MODE:
	        return PE_ARG_MJC_MODE;

	    case VID_PLA_SET_TYPE_MJC_EFFECT:
	        return PE_ARG_MJC_EFFECT;

	    case VID_PLA_SET_TYPE_MJC_DEMO:
	        return PE_ARG_MJC_DEMO;

	    case VID_PLA_SET_TYPE_XVYCC:
	        return PE_ARG_XVYCC;

	    case VID_PLA_SET_TYPE_WCG:
	        return PE_ARG_WCG;

	    case VID_PLA_SET_TYPE_GAME_MODE:
	        return PE_ARG_GAME_MODE;

	    case VID_PLA_SET_TYPE_BLUE_STRETCH:
	        return PE_ARG_BLUE_STRETCH;

	    case VID_PLA_SET_TYPE_MPEG_NR:
	        return PE_ARG_MPEG_NR;

	    case VID_PLA_SET_TYPE_LCDIM:
	        return PE_ARG_LCDIM;

        case VID_PLA_SET_TYPE_3D_MODE:
            return PE_ARG_TDTV_PACKING;       
            
        case VID_PLA_SET_TYPE_3D_LR_SWITCH:
            return PE_ARG_TDTV_INVERSE;
            
        case VID_PLA_SET_TYPE_3D_FLD_DEPTH:
            return PE_ARG_TDTV_DEPTH;        
            
        case VID_PLA_SET_TYPE_3D_TO_2D: 	        
            return PE_ARG_TDTV_3D_TO_2D;

        case VID_PLA_SET_TYPE_3D_FPR: 	        
            return PE_ARG_TDTV_FPR;	            
           
       case VID_PLA_SET_TYPE_3D_PROTRUDE_SCR:           
           return PE_ARG_TDTV_PROTRUDE;
           
       case VID_PLA_SET_TYPE_3D_DISTANCE_TV:           
           return PE_ARG_TDTV_DISTANCE;
           
       case VID_PLA_SET_TYPE_3D_OSD_DEPTH:           
           return PE_ARG_TDTV_OSD_DEPTH;
           
        default:
            return PE_ARG_NS;
    }
}


//-----------------------------------------------------------------------------
/** Brief of _VdpMwVidQaType2UiVqItem.
 */
//-----------------------------------------------------------------------------
//static PE_ARG_TYPE _VdpMwVidQaType2UiVqItem(VID_QA_TYPE_T eVidQaType)
PE_ARG_TYPE _VdpMwVidQaType2UiVqItem(VID_QA_TYPE_T eVidQaType)
{
    switch (eVidQaType)
    {
        case VID_QA_TYPE_BRIGHTNESS:    // brightness
            return PE_ARG_BRIGHTNESS;

        case VID_QA_TYPE_CONTRAST:      // contrast
            return PE_ARG_CONTRAST;

        case VID_QA_TYPE_HUE:           // hue
            return PE_ARG_HUE;

        case VID_QA_TYPE_SATURATION:    // saturation
            return PE_ARG_SATURATION;

        case VID_QA_TYPE_CTI:           // cti
            return PE_ARG_CTI;

        case VID_QA_TYPE_SHARPNESS:     // sharpness (v, h)
            return PE_ARG_SHARPNESS;

        case VID_QA_TYPE_LTI:     // LTI
            return PE_ARG_LTI;

		case VID_QA_TYPE_COLOR_SUPPRESS:
			return PE_ARG_BRIGHTNESS;	// No Used
			
        case VID_QA_TYPE_COLOR_GAIN:    // color gain
            return PE_ARG_R_GAIN;

        case VID_QA_TYPE_COLOR_OFFSET:    // color offset
            return PE_ARG_R_OFFSET;

        case VID_QA_TYPE_NR:            // noise reduction
            return PE_ARG_NR;

        case VID_QA_TYPE_GAMMA:    // GAMMA
            return PE_ARG_GAMMA;

        case VID_QA_TYPE_WHITE_PEAK_LMT:// white peak limit
            return PE_ARG_WHITE_PEAK_LMT;

        case VID_QA_TYPE_FLESH_TONE:    // second color enhance
            return PE_ARG_SCE;

        case VID_QA_TYPE_LUMA:          // adaptive luma
            return PE_ARG_ADAPTIVE_LUMA;

        case VID_QA_TYPE_3D_NR:          // 3D NR
            return PE_ARG_3D_NR;

        case VID_QA_TYPE_DEMO:
            return PE_ARG_DEMO;

        case VID_QA_TYPE_DI_FILM_MODE:
            return PE_ARG_DI_FILM_MODE;

        case VID_QA_TYPE_DI_MA:
            return PE_ARG_DI_MA;

        case VID_QA_TYPE_DI_EDGE_PRESERVE:
            return PE_ARG_DI_EDGE_PRESERVE;

        case VID_QA_TYPE_MJC_MODE:
            return PE_ARG_MJC_MODE;

        case VID_QA_TYPE_MJC_EFFECT:
            return PE_ARG_MJC_EFFECT;

        case VID_QA_TYPE_MJC_DEMO:
            return PE_ARG_MJC_DEMO;

        case VID_QA_TYPE_XVYCC:
            return PE_ARG_XVYCC;

        case VID_QA_TYPE_WCG:
            return PE_ARG_WCG;

        case VID_QA_TYPE_GAME_MODE:
            return PE_ARG_GAME_MODE;

        case VID_QA_TYPE_BLUE_STRETCH:   // blue stretch
            return PE_ARG_BLUE_STRETCH;

        case VID_QA_TYPE_MPEG_NR:        // mpeg nr
            return PE_ARG_MPEG_NR;

        case VID_QA_TYPE_BLACK_STRETCH:   // black stretch
            return PE_ARG_BLACK_STRETCH;

        case VID_QA_TYPE_WHITE_STRETCH:   // white stretch
            return PE_ARG_WHITE_STRETCH;

        default:
        	Printf("VID_QA_TYPE Unknow %d\n", eVidQaType);
        	ASSERT(0);
        	return PE_ARG_NS;
    }
}
static BOOL _VdpUpdateOverscan(UCHAR ucVdpId, UINT32* pui4_top, UINT32* pui4_bottom, UINT32* pui4_left, UINT32* pui4_right)
{
    if ((_pfOverscanFct != NULL) && fgEnableMwCb)
    {
        UINT16 u2Up;
        UINT16 u2Down;
        UINT16 u2Left;
        UINT16 u2Right;
        DRV_CUST_INPUT_SRC_TYPE_T eSrcType;
        DRV_CUSTOM_TIMING_INFO_T rTimingInfo;

        if (ucVdpId >= VDP_NS)
        {
            return FALSE;
        }

        eSrcType = _VdpGetDrvCustInputSrcType(ucVdpId, _arMwVdp[ucVdpId].e_type);

        rTimingInfo.ui2_width = _arMwVdp[ucVdpId].rSrcRes.ui2_width;
        rTimingInfo.ui2_height = _arMwVdp[ucVdpId].rSrcRes.ui2_height;
        rTimingInfo.ui4_fram_rate = _arMwVdp[ucVdpId].u4FrameRate;
        rTimingInfo.e_color_sys = _VdpGetDrvCustColorSys(ucVdpId);
        rTimingInfo.e_timing_type = _arMwVdp[ucVdpId].e_timing;

        if (_arMwVdp[ucVdpId].u4Interlace)
        {
            rTimingInfo.e_scan_mode = DRV_CUSTOM_TIMING_SCAN_MODE_INTERLACE;
        }
        else
        {
            rTimingInfo.e_scan_mode = DRV_CUSTOM_TIMING_SCAN_MODE_PROGRESSIVE;
        }

        u2Up = (UINT16)*pui4_top;
        u2Down = (UINT16)*pui4_bottom;
        u2Left = (UINT16)*pui4_left;
        u2Right = (UINT16)*pui4_right;

        #ifndef LINUX_TURNKEY_SOLUTION
        _pfOverscanFct(
            ucVdpId,
            eSrcType,
            &rTimingInfo,
            &u2Up,
            &u2Down,
            &u2Left,
            &u2Right
        );
        #else
        vid_pla_overscan_fct(
            ucVdpId,
            eSrcType,
            &rTimingInfo,
            &u2Up,
            &u2Down,
            &u2Left,
            &u2Right
        );
        #endif

        if (*pui4_top != u2Up ||
            *pui4_bottom != u2Down ||
            *pui4_left != u2Left ||
            *pui4_right != u2Right)
        {
            *pui4_top = u2Up;
            *pui4_bottom = u2Down;
            *pui4_left = u2Left;
            *pui4_right = u2Right;

            return TRUE;
        }
    }

    return FALSE;
}

static void _VdpUpdateDispFmt(UCHAR ucVdpId)
{
    if ((_pfGetCurrentDispFmtFct != NULL) && fgEnableMwCb)
    {
        VID_PLA_DISP_FMT_T e_fmt;
        DRV_CUSTOM_VID_SRC_INFO_T rVidSrcInfo;

        rVidSrcInfo.ui2_vdp_id = (UINT16)ucVdpId;
        rVidSrcInfo.e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_INPUT_SRC;
        rVidSrcInfo.u.t_input_src.e_input_source = _VdpGetDrvCustInputSrcType(ucVdpId, _arMwVdp[ucVdpId].e_type);
        if (rVidSrcInfo.u.t_input_src.e_input_source == DRV_CUST_INPUT_SRC_TYPE_MULTIMEDIA)
        {
            // sys not ready for MM input source.
            // once sys is ready for this input source, you can remove this.
            rVidSrcInfo.u.t_input_src.e_input_source = DRV_CUST_INPUT_SRC_TYPE_DTV;
        }
        rVidSrcInfo.u.t_input_src.ui2_id = _arMwVdp[ucVdpId].u2SrcCompId;  // ucPortId=?
        rVidSrcInfo.u.t_input_src.e_timing_type = _arMwVdp[ucVdpId].e_timing;

        e_fmt = _arMwVdp[ucVdpId].eFmt;

        #ifndef LINUX_TURNKEY_SOLUTION
        _pfGetCurrentDispFmtFct(
            &_arMwVdp[ucVdpId].rSrcRes,
            &_arMwVdp[ucVdpId].rOutRes,
            &rVidSrcInfo,
            &e_fmt);
        #else
        vid_pla_get_current_disp_fmt_fct(
            &_arMwVdp[ucVdpId].rSrcRes,
            &_arMwVdp[ucVdpId].rOutRes,
            &rVidSrcInfo,
            &e_fmt);
        #endif

        if (_arMwVdp[ucVdpId].eFmt != e_fmt)
        {
            VDP_DBG_MSG("FMT CHANGE (%d) to (%d) VidSrcInfo (%d, %d, %d)\n",
                _arMwVdp[ucVdpId].eFmt,
                e_fmt,
                rVidSrcInfo.u.t_input_src.e_input_source,
                rVidSrcInfo.u.t_input_src.ui2_id,
                rVidSrcInfo.u.t_input_src.e_timing_type);

            _arMwVdp[ucVdpId].eFmt = e_fmt;
        }
    }
}


static TV_DEC_TIMING_TYPE_T  _bVDPGetTimingType(UINT8 u1Path)
{
    RVChannel*                pt_rv_channel = NULL;
	TV_DEC_TIMING_TYPE_T      e_timing_type ;
	DRV_TYPE_T                e_curr_src_type = DRVT_UNKNOWN;

    if( u1Path == SV_VP_MAIN )
    {
        pt_rv_channel = &_rMChannel;
    }
    else if ( u1Path == SV_VP_PIP )
    {
        pt_rv_channel = &_rPChannel;
    }
    else
    {
        return TV_DEC_TIMING_UNKNOWN;
    }

    e_curr_src_type = pt_rv_channel->u2_e_type;

    if ( bDrvVideoSignalStatus(u1Path) == SV_VDO_NOSUPPORT )
    {
        e_timing_type = TV_DEC_TIMING_NOT_SUPPORT;
    }
    else
    {
        e_timing_type = ( (fgApiVideoIsVgaModeEx( u1Path, 0))?
                                       TV_DEC_TIMING_GRAPHIC:
                                       TV_DEC_TIMING_VIDEO );
    }

    if ( e_curr_src_type == DRVT_AVC_HDMI )
    {
        switch(bApiVideoGetFixColorSpaceMode(u1Path))
        {
            case SV_FIXCOLORSPACE_OFF:
                e_timing_type = ((fgIsVideoTiming(_bDviTiming))?
                TV_DEC_TIMING_VIDEO:TV_DEC_TIMING_GRAPHIC);
                break;
            case SV_FIXCOLORSPACE_RGB_444:
                e_timing_type =TV_DEC_TIMING_GRAPHIC;
                break;
            case SV_FIXCOLORSPACE_RGB_422:
            case SV_FIXCOLORSPACE_YCBCR:
                e_timing_type =TV_DEC_TIMING_VIDEO;
                break;
            default:
                break;
        }
    }
	return e_timing_type ;
}


//-----------------------------------------------------------------------------
/** Brief of _VdpSourceChangeNotify.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpSourceChangeNotify(UCHAR ucVdpId, VDP_SOURCE_INFO_T rVdpSrcInfo)
{
    if (ucVdpId >= VDP_NS)
    {
        return 0;
    }
    VDP_DBG_MSG("_VdpSourceChangeNotify\n");
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
    BOOL fgRetTrue = FALSE;

    VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    _arMwVdp[ucVdpId].rSrcRes.ui2_width = (UINT16)rVdpSrcInfo.u4SrcWidth;
    _arMwVdp[ucVdpId].rSrcRes.ui2_height = (UINT16)rVdpSrcInfo.u4SrcHeight;
    _arMwVdp[ucVdpId].rOutRes.ui2_width = (UINT16)rVdpSrcInfo.u4OutWidth;
    _arMwVdp[ucVdpId].rOutRes.ui2_height = (UINT16)rVdpSrcInfo.u4OutHeight;
    _arMwVdp[ucVdpId].u4FrameRate = rVdpSrcInfo.u4FrameRate;
    _arMwVdp[ucVdpId].u4Interlace = rVdpSrcInfo.u4Interlace;
    _arMwVdp[ucVdpId].e_timing = _bVDPGetTimingType(ucVdpId);

    if (_arMwVdp[ucVdpId].e_type == DRVT_UNKNOWN ||
        (_arMwVdp[ucVdpId].e_timing != TV_DEC_TIMING_VIDEO &&
         _arMwVdp[ucVdpId].e_timing != TV_DEC_TIMING_GRAPHIC))
    {
        _arMwVdp[ucVdpId].ucSrcChgNfy = 1;
    }
    else
    {
        _arMwVdp[ucVdpId].ucSrcChgNfy = 0;

        fgRetTrue |= _VdpUpdateOverscan(ucVdpId,
                                        &(_prVdpConf[ucVdpId]->rOverScan.u4Top),
                                        &(_prVdpConf[ucVdpId]->rOverScan.u4Bottom),
                                        &(_prVdpConf[ucVdpId]->rOverScan.u4Left),
                                        &(_prVdpConf[ucVdpId]->rOverScan.u4Right));

        _VdpUpdateDispFmt(ucVdpId);
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

        fgRetTrue = TRUE;
    }

    VERIFY(x_sema_unlock(_hMutex) == OSR_OK);

    if (_arMwVdp[ucVdpId].e_type == DRVT_VID_DEC &&
        _arMwVdp[ucVdpId].u2SrcCompId != (UINT16)MPV_COMP_NS)
    {
    	VDP_DBG_MSG("MPV notify in _VdpSourceChangeNotify\n");
        MW_MPV_NOTIFY(_arMwVdp[ucVdpId].u2SrcCompId);
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

static void _VdpPlayDoneNotify(UINT32 u4VdpId, UINT32 u4Arg1, UINT32 u4Arg2)
{
    MW_MPV_PlayDone_NOTIFY(_arMwVdp[u4VdpId].u2SrcCompId);
}

//-----------------------------------------------------------------------------
/** Brief of _VdpAfdChangeNotify.
 */
//-----------------------------------------------------------------------------
static void _VdpAfdChangeNotify(UINT32 u4VdpId, UINT32 u4Afd, FBM_ASPECT_RATIO_T* prAspectRatio)
{
    UNUSED(u4Afd);
    UNUSED(prAspectRatio);

    VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
    VDP_DBG_MSG("_VdpAfdChangeNotify\n");
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

    if (_arMwVdp[u4VdpId].e_type == DRVT_VID_DEC &&
        _arMwVdp[u4VdpId].u2SrcCompId != (UINT16)MPV_COMP_NS)
    {
        MW_MPV_NOTIFY(_arMwVdp[u4VdpId].u2SrcCompId);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VdpXlatDispFmtCallBackMw.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpXlatDispFmtCallBackMw(UCHAR ucVdpId)
{
    if (ucVdpId >= VDP_NS)
    {
        return 0;
    }

#ifndef CC_VDP_FORCE_NO_MW_CB
    if ((_pfXlatDispFmtExFct[(UINT32)_arMwVdp[ucVdpId].eFmt] != NULL) && fgEnableMwCb)
    {
        VID_PLA_VID_REGION_T rSrcReg;
        VID_PLA_VID_REGION_T rDispReg;
        DRV_CUSTOM_VID_SRC_INFO_T rVidSrcInfo;

#if 1        
		static VDEC_HDR_INFO_T rHdrInfo;
		x_memset(&rHdrInfo,0,sizeof(VDEC_HDR_INFO_T));
		VDEC_QueryInfo(ES0, &rHdrInfo);
		if (_arMwVdp[ucVdpId].ucDtv != 0)
		{
            rVidSrcInfo.ui2_vdp_id = (UINT16)ucVdpId;
            rVidSrcInfo.e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_MPEG;
            rVidSrcInfo.u.t_mpeg.ui1_afd = (UINT8) _prVdpConf[ucVdpId]->u4Afd;
            rVidSrcInfo.ui4_panel_width = PANEL_GetPanelWidth();
            rVidSrcInfo.ui4_panel_height = PANEL_GetPanelHeight();
            
            if(rHdrInfo.fgPixelAspectRatio)
            {
            	rVidSrcInfo.u.t_mpeg.b_is_display_aspect_ratio = FALSE;
            }
            else
            {
            	rVidSrcInfo.u.t_mpeg.b_is_display_aspect_ratio = TRUE;
            }

			if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_4_3)
	        {
	            //peDecResolution->b_is_display_aspect_ratio = TRUE;
	            rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_4_3;
	        }
	        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_16_9)
	        {
	            //peDecResolution->b_is_display_aspect_ratio = TRUE;
	            rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_16_9;
	        }
	        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_221_1)
	        {
	            //peDecResolution->b_is_display_aspect_ratio = TRUE;
	            rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_2_21_1;
	        }
	        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_10_11)
	        {
	            //peDecResolution->b_is_display_aspect_ratio = FALSE;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_w = 10*rHdrInfo.u2Width;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_h = 11*rHdrInfo.u2Height;
	            rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_10_11;
	        }
	        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_40_33)
	        {
	            //peDecResolution->b_is_display_aspect_ratio = FALSE;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_w = 40*rHdrInfo.u2Width;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_h = 33*rHdrInfo.u2Height;            
	            rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_40_33;
	        }
	        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_16_11)
	        {
	            //peDecResolution->b_is_display_aspect_ratio = FALSE;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_w = 16*rHdrInfo.u2Width;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_h = 11*rHdrInfo.u2Height;            
	            rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_16_11;
	        }
	        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_12_11)
	        {
	            //peDecResolution->b_is_display_aspect_ratio = FALSE;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_w = 12*rHdrInfo.u2Width;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_h = 11*rHdrInfo.u2Height;            
	            rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_12_11;
	        }
	        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_3_2)
	        {
	            //peDecResolution->b_is_display_aspect_ratio = FALSE;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_w = 3*rHdrInfo.u2Width;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_h = 2*rHdrInfo.u2Height;            
	            rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_3_2;
	        }
	        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_1_1)
	        {
	            //peDecResolution->b_is_display_aspect_ratio = FALSE;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_w = 1*rHdrInfo.u2Width;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_h = 1*rHdrInfo.u2Height;            
	            rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_1_1;
	        }
	        else if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_TRANSMIT)
	        {
	            //peDecResolution->b_is_display_aspect_ratio = FALSE;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_w = rHdrInfo.u4PixelAspectRatioWidth*rHdrInfo.u2Width;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_h = rHdrInfo.u4PixelAspectRatioHeight*rHdrInfo.u2Height;             
	            rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_USR;
	        }
	        else
	        {
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_w = 1*rHdrInfo.u2Width;
	            rVidSrcInfo.u.t_mpeg.u4_aspect_ratio_h = 1*rHdrInfo.u2Height;            
	            rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_1_1;
	        }	
        }
#else
        if (_arMwVdp[ucVdpId].ucDtv != 0)
        {
            rVidSrcInfo.ui2_vdp_id = (UINT16)ucVdpId;
            rVidSrcInfo.e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_MPEG;
            rVidSrcInfo.u.t_mpeg.ui1_afd = (UINT8) _prVdpConf[ucVdpId]->u4Afd;

            switch (_prVdpConf[ucVdpId]->u4AspectRatio)
            {
                case 1:  // MPEG_ASPECT_RATIO_1_1
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_1_1;
                break;

                case 2: // MPEG_ASPECT_RATIO_4_3
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_4_3;
                break;

                case 3: // MPEG_ASPECT_RATIO_16_9
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_16_9;
                break;

                case 4: // MPEG_ASPECT_RATIO_221_1
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_2_21_1;
                break;

                case 5: // AVI_ASPECT_RATIO_10_11
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_10_11;
                break;

                case 6: // AVI_ASPECT_RATIO_40_33
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_40_33;
                break;

                case 7: // AVI_ASPECT_RATIO_16_11
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_16_11;
                break;

                case 8: // AVI_ASPECT_RATIO_12_11
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_12_11;
                break;

                case 9: // AVI_ASPECT_RATIO_3_2
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_PXL_3_2;
                break;

                default:
                rVidSrcInfo.u.t_mpeg.e_asp_ratio = VID_DEC_SRC_ASPECT_RATIO_UNKNOWN;
                break;
            }
        }
#endif        
        else
        {
            rVidSrcInfo = _arMwVidSrcInfo[ucVdpId];
        }

        #ifndef LINUX_TURNKEY_SOLUTION
        _pfXlatDispFmtExFct[(UINT32)_arMwVdp[ucVdpId].eFmt](
            &_arMwVdp[ucVdpId].rSrcRes,
            &_arMwVdp[ucVdpId].rOutRes,
            &rVidSrcInfo,
            &rSrcReg,
            &rDispReg);
        #else
        set_vdp_ex_nfy(ucVdpId,
            &_arMwVdp[ucVdpId].rSrcRes,
            &_arMwVdp[ucVdpId].rOutRes,
            &rVidSrcInfo,
            &rSrcReg,
            &rDispReg);
        #endif

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

            PrintfSrcRegion(_arMwVdp[ucVdpId].rFinalSource);
            PrintfOutRegion(_arMwVdp[ucVdpId].rFinalOutput);

            return 1;
        }
    }
    #if 0
    else
    if (_pfXlatDispFmtFct[(UINT32)_arMwVdp[ucVdpId].eFmt] != NULL)
    {
        VID_PLA_VID_REGION_T rSrcReg;
        VID_PLA_VID_REGION_T rDispReg;

    #ifndef LINUX_TURNKEY_SOLUTION
        _pfXlatDispFmtFct[(UINT32)_arMwVdp[ucVdpId].eFmt](
            &_arMwVdp[ucVdpId].rSrcRes,
            &_arMwVdp[ucVdpId].rOutRes,
            (UINT8) _prVdpConf[ucVdpId]->u4Afd,
            &rSrcReg,
            &rDispReg);
    #endif

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
    _arMwVdp[ucVdpId].rFinalOutput.ui4_x = _arMwVdp[ucVdpId].rFullOutput.ui4_x + ((_arMwVdp[ucVdpId].rXlatOutput.ui4_x * _arMwVdp[ucVdpId].rFullOutput.ui4_width) / VDP_MAX_REGION_WIDTH);

    // Y
    _arMwVdp[ucVdpId].rFinalOutput.ui4_y = _arMwVdp[ucVdpId].rFullOutput.ui4_y + ((_arMwVdp[ucVdpId].rXlatOutput.ui4_y * _arMwVdp[ucVdpId].rFullOutput.ui4_height) / VDP_MAX_REGION_HEIGHT);

    // Width
    _arMwVdp[ucVdpId].rFinalOutput.ui4_width = (_arMwVdp[ucVdpId].rXlatOutput.ui4_width * _arMwVdp[ucVdpId].rFullOutput.ui4_width) / VDP_MAX_REGION_WIDTH;

    // Height
    _arMwVdp[ucVdpId].rFinalOutput.ui4_height = (_arMwVdp[ucVdpId].rXlatOutput.ui4_height * _arMwVdp[ucVdpId].rFullOutput.ui4_height) / VDP_MAX_REGION_HEIGHT;
}

//-----------------------------------------------------------------------------
/** Brief of _VdpCalculateSourceRegion.
 */
//-----------------------------------------------------------------------------
static void _VdpCalculateSourceRegion(UCHAR ucVdpId)
{
    // X
    _arMwVdp[ucVdpId].rFinalSource.ui4_x = _arMwVdp[ucVdpId].rFullSource.ui4_x + ((_arMwVdp[ucVdpId].rXlatSource.ui4_x * _arMwVdp[ucVdpId].rFullSource.ui4_width) / VDP_MAX_REGION_WIDTH);

    // Y
    _arMwVdp[ucVdpId].rFinalSource.ui4_y = _arMwVdp[ucVdpId].rFullSource.ui4_y + ((_arMwVdp[ucVdpId].rXlatSource.ui4_y * _arMwVdp[ucVdpId].rFullSource.ui4_height) / VDP_MAX_REGION_HEIGHT);

    // Width
    _arMwVdp[ucVdpId].rFinalSource.ui4_width = (_arMwVdp[ucVdpId].rXlatSource.ui4_width * _arMwVdp[ucVdpId].rFullSource.ui4_width) / VDP_MAX_REGION_WIDTH;

    // Height
    _arMwVdp[ucVdpId].rFinalSource.ui4_height = (_arMwVdp[ucVdpId].rXlatSource.ui4_height * _arMwVdp[ucVdpId].rFullSource.ui4_height) / VDP_MAX_REGION_HEIGHT;
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

    PE_ARG_TYPE eUiVqItem;
    PE_UI_RANGE_T rUiVqMinMaxDft;
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
        if (eUiVqItem >= PE_ARG_NS)
        {
            return FALSE;
        }

        rUiVqMinMaxDft.i4Min = pt_iter->i4_min;
        rUiVqMinMaxDft.i4Max = pt_iter->i4_max;
        rUiVqMinMaxDft.i4Dft = pt_iter->i4_default;

		// TODO: Remove RGB Gain/Offset Special case
		switch (pt_iter->e_vid_qa_type)
        {
			case VID_QA_TYPE_COLOR_GAIN:
		        PE_SetUiRange(ui1_src_idx, PE_ARG_R_GAIN, &rUiVqMinMaxDft);						
		        PE_SetUiRange(ui1_src_idx, PE_ARG_G_GAIN, &rUiVqMinMaxDft);		
		        PE_SetUiRange(ui1_src_idx, PE_ARG_B_GAIN, &rUiVqMinMaxDft);				        
				break;
			case VID_QA_TYPE_COLOR_OFFSET:
		        PE_SetUiRange(ui1_src_idx, PE_ARG_R_OFFSET, &rUiVqMinMaxDft);						
		        PE_SetUiRange(ui1_src_idx, PE_ARG_G_OFFSET, &rUiVqMinMaxDft);		
		        PE_SetUiRange(ui1_src_idx, PE_ARG_B_OFFSET, &rUiVqMinMaxDft);						
				break;

			default:
		        PE_SetUiRange(ui1_src_idx, eUiVqItem, &rUiVqMinMaxDft);				
        }

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

    PE_SetUiMinMaxDft();

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

		if(((UINT32) prFmtInfo->e_disp_fmt) >= CC_DISP_FMT_ENUM_SIZE)
		{
            return DRVP_ERROR;
		}
        _pfXlatDispFmtExFct[(UINT32) prFmtInfo->e_disp_fmt] = prFmtInfo->pf_xlat_disp_fmt;
    }
    else
    if (pt_drv_cust->e_custom_type == DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT)
    {
        #if 0
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
        #else
        return DRVP_ERROR;
        #endif
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
    else if (pt_drv_cust->e_custom_type == DRV_CUSTOM_TYPE_VID_PLA_VID_QA_TBL_QUERY_CB)
    {
        _pfVidQaTblQueryFct = (d_vid_pla_vid_qa_tbl_query_fct) pt_drv_cust->pv_arg;
    }
    else
    {
        return DRVP_ERROR;
    }

    return DRVP_NEXT;
LINT_EXT_HEADER_END
}

extern UINT8 bApiVideoGetColorSystem(void);

static DRV_CUSTOM_COLOR_SYS_TYPE_T _VdpGetDrvCustColorSys(UCHAR ucVdpId)
{
    UINT8 u1ColSys;

    if ( _arMwVdp[ucVdpId].e_type == DRVT_UNKNOWN ||
         _arMwVdp[ucVdpId].ucDtv == 1 )
    {
        return DRV_CUSTOM_COLOR_SYS_UNKNOWN;
    }

    u1ColSys = bApiVideoGetColorSystem();

    switch (u1ColSys)
    {
    case SV_CS_PAL_N:
        return DRV_CUSTOM_COLOR_SYS_PAL_N;
    case SV_CS_PAL:
        return DRV_CUSTOM_COLOR_SYS_PAL;
    case SV_CS_PAL_M:
        return DRV_CUSTOM_COLOR_SYS_PAL_M;
    case SV_CS_NTSC358:
        return DRV_CUSTOM_COLOR_SYS_NTSC;
    case SV_CS_SECAM:
        return DRV_CUSTOM_COLOR_SYS_SECAM;
    case SV_CS_NTSC443:
        return DRV_CUSTOM_COLOR_SYS_NTSC_443;
    case SV_CS_PAL_60:
        return DRV_CUSTOM_COLOR_SYS_PAL_60;
    case SV_CS_UNSTABLE:
    default:
        return DRV_CUSTOM_COLOR_SYS_UNKNOWN;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VdpGetDrvCustInputSrcType.
 */
//-----------------------------------------------------------------------------
static DRV_CUST_INPUT_SRC_TYPE_T _VdpGetDrvCustInputSrcType(UCHAR ucVdpId, DRV_TYPE_T e_type)
{
    switch (e_type)
    {
        case DRVT_AVC_COMP_VIDEO:
        case DRVT_AVC_S_VIDEO:
            return DRV_CUST_INPUT_SRC_TYPE_CVBS;

        case DRVT_AVC_Y_PB_PR:
            return DRV_CUST_INPUT_SRC_TYPE_YPBPR;

        case DRVT_AVC_VGA:
            return DRV_CUST_INPUT_SRC_TYPE_VGA;

        case DRVT_AVC_SCART:
            return DRV_CUST_INPUT_SRC_TYPE_SCART;

        case DRVT_AVC_DVI:
            return DRV_CUST_INPUT_SRC_TYPE_DVI;

        case DRVT_AVC_HDMI:
            return DRV_CUST_INPUT_SRC_TYPE_HDMI;

        case DRVT_VID_DEC:
#ifdef ENABLE_MULTIMEDIA
            /*check MM mode*/
            if (VDP_GetPlayMode(ucVdpId) == FBM_FBG_MM_MODE)
            {
                DRV_CUST_INPUT_SRC_TYPE_T eSrcType = DRV_CUST_INPUT_SRC_TYPE_MULTIMEDIA;

#ifdef TIME_SHIFT_SUPPORT
                UINT32 u4Tmp;

#ifdef CC_53XX_SWDMX_V2
                if(SWDMX_GetInfo(SWDMX_PRIMARY_SOURCE, eSWDMX_CONTAINER_TYPE, &u4Tmp, 0, 0))
#else
                if(SWDMX_GetInfo(eSWDMX_CONTAINER_TYPE, &u4Tmp, 0, 0))
#endif
                {
                    if((u4Tmp == (UINT32)SWDMX_FMT_MPEG2_TS_TIME_SHIFT)||          //DTV00145200
						(u4Tmp == (UINT32)SWDMX_FMT_MPEG2_TS_ENCRYPT_192))         //when pvr overscan is set as dtv

                    {

                        eSrcType = DRV_CUST_INPUT_SRC_TYPE_TIMESHIFT;
                    }
                }
#endif
                return eSrcType;
            }
#endif
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
void _MW_VDP_HDMI_AVI_INFOFRAME_CB(UCHAR ucVdpId, DRV_CUSTOM_VID_SRC_HDMI_AVI_INFOFRAME_INFO_T u2Data)
{
    if (ucVdpId < VDP_COMP_NS)
    {
        VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        if ((_arMwVidSrcInfo[ucVdpId].e_type != DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_AVI_INFOFRAME)
            ||(_arMwVidSrcInfo[ucVdpId].u.t_hdmi_avi_infoframe.ui1_byte_1 != u2Data.ui1_byte_1)
            ||(_arMwVidSrcInfo[ucVdpId].u.t_hdmi_avi_infoframe.ui1_byte_2 != u2Data.ui1_byte_2)
            ||(_arMwVidSrcInfo[ucVdpId].u.t_hdmi_avi_infoframe.ui1_byte_3 != u2Data.ui1_byte_3))
        {
            _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_AVI_INFOFRAME;
            _arMwVidSrcInfo[ucVdpId].u.t_hdmi_avi_infoframe.ui1_byte_1 = u2Data.ui1_byte_1;
            _arMwVidSrcInfo[ucVdpId].u.t_hdmi_avi_infoframe.ui1_byte_2 = u2Data.ui1_byte_2;
            _arMwVidSrcInfo[ucVdpId].u.t_hdmi_avi_infoframe.ui1_byte_3 = u2Data.ui1_byte_3;

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

void _VdpDispSupportCallback(UINT32 u4VdpId, UINT32 u4Status)
{
    #ifdef LINUX_TURNKEY_SOLUTION
    //set_vdp_pla_nfy(u4VdpId);
    vdp_pla_nfy_fct(u4VdpId, _arMwVdp[u4VdpId].pvEosTag, VID_PLA_COND_DISP_MODE, u4Status, 0);
    #else
    _arMwVdp[u4VdpId].pfEosNfy(_arMwVdp[u4VdpId].pvEosTag, VID_PLA_COND_DISP_MODE, u4Status, 0);
    #endif
}

void _VdpEosCallback(UINT32 u4VdpId, VDP_COND_T eCond, BOOL fgFastForward)
{
    #ifdef LINUX_TURNKEY_SOLUTION
    //set_vdp_pla_nfy(u4VdpId);
     #ifndef MULTIMEDIA_AUTOTEST_SUPPORT
		 vdp_pla_nfy_fct(u4VdpId, _arMwVdp[u4VdpId].pvEosTag, (VID_PLA_COND_T)eCond, (UINT32)fgFastForward, 0);
	 #else
		 if(fgFastForward)	   //only send eos notify to mw for  forward not for backward
		 {
			 vdp_pla_nfy_fct(u4VdpId, _arMwVdp[u4VdpId].pvEosTag, (VID_PLA_COND_T)eCond, (UINT32)fgFastForward, 0);
		 }
	 #endif
    #else
     #ifndef MULTIMEDIA_AUTOTEST_SUPPORT
		 _arMwVdp[u4VdpId].pfEosNfy(_arMwVdp[u4VdpId].pvEosTag, (VID_PLA_COND_T)eCond, (UINT32)fgFastForward, 0);
     #else
		 if(fgFastForward)	   //only send eos notify to mw for  forward not for backward
		 {
			_arMwVdp[u4VdpId].pfEosNfy(_arMwVdp[u4VdpId].pvEosTag, (VID_PLA_COND_T)eCond, (UINT32)fgFastForward, 0);
		 }
     #endif 
    #endif
#ifdef MULTIMEDIA_AUTOTEST_SUPPORT 
	if(_arMwVdp[u4VdpId].pfAtEosNfy)
	{
		_arMwVdp[u4VdpId].pfAtEosNfy(_arMwVdp[u4VdpId].pvATEosTag,(VID_PLA_COND_T)eCond,1,0);
	}
#endif

}


void _MW_VDP_WSS525_CB(UCHAR ucVdpId, UINT16 u2Data)
{
    if (ucVdpId < VDP_COMP_NS)
    {
      VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        if ((_arMwVidSrcInfo[ucVdpId].e_type != DRV_CUSTOM_VID_SRC_INFO_TYPE_NTSC_WSS) ||
            (_arMwVidSrcInfo[ucVdpId].u.ui4_ntsc_wss != u2Data))
        {
            _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_NTSC_WSS;
            _arMwVidSrcInfo[ucVdpId].u.ui4_ntsc_wss = u2Data;
            _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.e_scart_pin_8 = DRV_CUSTOM_SCART_PIN_8_UNKNOWN;
            VDP_DBG_MSG("_MW_VDP_WSS525_CB\n");

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


void _MW_VDP_WSS625_CB(UCHAR ucVdpId, UINT16 u2Data)
{
    if (ucVdpId < VDP_COMP_NS)
    {
      VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        if ((_arMwVidSrcInfo[ucVdpId].e_type != DRV_CUSTOM_VID_SRC_INFO_TYPE_PAL_SECAM_WSS_SCART) ||
            (_ui2_pal_secam_wss != u2Data))
        {
            _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_PAL_SECAM_WSS_SCART;
            _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.ui2_pal_secam_wss = u2Data;
            _ui2_pal_secam_wss = u2Data;
            _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.e_scart_pin_8 = DRV_CUSTOM_SCART_PIN_8_UNKNOWN;
            VDP_DBG_MSG("_MW_VDP_WSS625_CB\n");

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

        if(t_scart_pin_8 != DRV_CUSTOM_SCART_PIN_8_UNKNOWN)
        {
            _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_PAL_SECAM_WSS_SCART;
        }
        else
        {
            // notify unknown for canal+ tuner case
            _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_UNKNOWN;
        }
        _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.e_scart_pin_8 = t_scart_pin_8;
        _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.ui2_pal_secam_wss = 0;
        VDP_DBG_MSG("_MW_VDP_SCART_PIN_8_CB\n");
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
    #if 0
    ASSERT((UINT32) sizeof(VID_PLA_DISP_FMT_T) <= CC_DISP_FMT_ENUM_SIZE);
    #else
    ASSERT((UINT32) VID_PLA_DISP_FMT_MAX <= CC_DISP_FMT_ENUM_SIZE );
    #endif

    /* Init Local Variable */
    for (ucVdpId = 0; ucVdpId < VDP_COMP_NS; ucVdpId++)
    {
        _arMwVdp[ucVdpId] = _rMwVdpDefault;

        _arMwVidSrcInfo[ucVdpId].ui2_vdp_id = (UINT16)ucVdpId;
        _arMwVidSrcInfo[ucVdpId].e_type = DRV_CUSTOM_VID_SRC_INFO_TYPE_UNKNOWN;
        _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.ui2_pal_secam_wss = 0;
        _ui2_pal_secam_wss = 0;
        _arMwVidSrcInfo[ucVdpId].u.t_wss_scart.e_scart_pin_8 = DRV_CUSTOM_SCART_PIN_8_UNKNOWN;
    }

    for (ucVdpId = 0; ucVdpId < (UCHAR)MPV_COMP_NS; ucVdpId++)
    {
        _arMwComp2EsId[ucVdpId] = 0xFF;
#ifdef CC_3D_MM_DS_SUPPORT        
        _arMwComp3dInfo[ucVdpId] = FALSE;
#endif        
    }

    /* Init */
    VDP_Init();

    // get semaphore instead of create semaphore, Upper Layer, DTV, & NPTV share one mutex to prevent deadlock
    _hMutex = VDP_GetMutex();
//    VERIFY(x_sema_create(&_hMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

    /* Source Change Notification */
    VDP_RegCbFunc(VDP_CB_FUNC_SRC_CHG_IND, (UINT32)_VdpSourceChangeNotify, 0, 0);
//    VDP_SetSourceChangeCb(_VdpSourceChangeNotify);
    VDP_RegCbFunc(VDP_CB_FUNC_DISP_SUPPORT_IND, (UINT32)_VdpDispSupportCallback, 0, 0);

    /* Afd Change Notification */
    VDP_RegCbFunc(VDP_CB_FUNC_AFD_IND, (UINT32)_VdpAfdChangeNotify, 0, 0);
//    VDP_SetAfdChangeCb(_VdpAfdChangeNotify);

    /* Play Done Notification */
    VDP_RegCbFunc(VDP_CB_FUNC_PLAY_DONE_IND, (UINT32)_VdpPlayDoneNotify, 0, 0);

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
#if 0
#ifndef LINUX_TURNKEY_SOLUTION
    for (u4Idx = 0; u4Idx < (UINT32)(1 << (8 * CC_DISP_FMT_ENUM_SIZE)); u4Idx++)
#else
    for (u4Idx = 0; u4Idx < CC_DISP_FMT_ENUM_SIZE; u4Idx++)
#endif
#else
    for (u4Idx = 0; u4Idx < CC_DISP_FMT_ENUM_SIZE; u4Idx++)
#endif
    {
        #if 0
        _pfXlatDispFmtFct[u4Idx] = NULL;
        #endif

        _pfXlatDispFmtExFct[u4Idx] = NULL;
    }
    _pfGetCurrentDispFmtFct = NULL;
    _pfOverscanFct = NULL;
    _pfUpdateOverscanTbl = NULL;
    _pfVidQaTblQueryFct = NULL;

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
    PE_SetAllUiVqItemDftValues();

    return (RMR_OK);
}

//-----------------------------------------------------------------------------
/** Brief of MW_VDP_SetConnect.
 */
//-----------------------------------------------------------------------------
void MW_VDP_SetConnect(UCHAR ucVdpId, UCHAR ucConnect)
{
    VERIFY_VDP_ID(ucVdpId);

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
            if (_arMwVdp[u4VdpId].u2SrcCompId == (UINT16)ucCompId)
            {
                _arMwVdp[u4VdpId].ucEsId = ucEsId;
                    VERIFY(_SetInput((UCHAR)u4VdpId) == VDP_SET_OK);
            }
        }
    }
}

#ifdef CC_3D_MM_DS_SUPPORT
void MW_VDP_Comp3DInfo(UCHAR ucCompId, BOOL fgFrameSeq)
{
    if (ucCompId >= (UCHAR)MPV_COMP_NS)
    {
        ASSERT(ucCompId >= (UCHAR)MPV_COMP_NS);
        return;
    }

    if (_arMwComp3dInfo[ucCompId] != fgFrameSeq)
    {
        _arMwComp3dInfo[ucCompId] = fgFrameSeq;
    }

    if(fgFrameSeq)
    {
        VDP_Set3DInfo(0,B2R_3D_FRAME_SEQUENTIAL,TRUE);
        VDP_Set3DInfo(1,B2R_3D_FRAME_SEQUENTIAL,TRUE);
        VDP_SetInput(0,0,0);
    }
    /*else
    {
        VDP_Set3DInfo(0,B2R_3D_NONE,FALSE);
        VDP_Set3DInfo(1,B2R_3D_NONE,FALSE);
    }*/
}
#endif

//-----------------------------------------------------------------------------
/** Brief of MW_VDP_Comp2Vdp.
 */
//-----------------------------------------------------------------------------
UINT32 MW_VDP_Comp2Vdp(UCHAR ucCompId)
{
    UINT32 u4VdpId = VDP_NS;

    if (ucCompId >= (UCHAR)MPV_COMP_NS)
    {
        ASSERT(ucCompId >= (UCHAR)MPV_COMP_NS);
        return VDP_NS;
    }

    for (u4VdpId = 0; u4VdpId < VDP_COMP_NS; u4VdpId++)
    {
        if (_arMwVdp[u4VdpId].u2SrcCompId == (UINT16)ucCompId)
        {
            return u4VdpId;
        }
    }

    return VDP_NS;

}


void MW_VDP_CallbackTrigger(UCHAR ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId);

    if (_arMwVdp[ucVdpId].e_type != DRVT_VID_DEC &&
        _arMwVdp[ucVdpId].u2TvdCompId < TVD_COMP_NS ) /* the source must be TVD */
    {
        DRV_TYPE_T e_type, e_src_type;
        UINT16 ui2_id, ui2_src_id;
        TV_DEC_TIMING_TYPE_T e_timing;
        extern BOOL avc_scart_is_from_tuner_src(
            DRV_TYPE_T e_type, UINT16 ui2_comp_id,
            DRV_TYPE_T* pe_src_type, UINT16* pui2_src_comp_id );

        TvdGetCurrInputSrcInfo(_arMwVdp[ucVdpId].u2TvdCompId, &e_type, &ui2_id, &e_timing);

        /* If source is SCART, should check if the real source is TUNER */
        if ( e_type == DRVT_AVC_SCART &&
             avc_scart_is_from_tuner_src( e_type, ui2_id, &e_src_type, &ui2_src_id ) )
        {
            _arMwVdp[ucVdpId].e_type = e_src_type;
            _arMwVdp[ucVdpId].e_timing = e_timing;
        }
        else
        {
            _arMwVdp[ucVdpId].e_type = e_type;
            _arMwVdp[ucVdpId].e_timing = e_timing;
        }
    }

    if (_arMwVdp[ucVdpId].e_type == DRVT_UNKNOWN ||
        (_arMwVdp[ucVdpId].e_timing != TV_DEC_TIMING_VIDEO &&
         _arMwVdp[ucVdpId].e_timing != TV_DEC_TIMING_GRAPHIC))
    {
        return;
    }

    if (_arMwVdp[ucVdpId].ucSrcChgNfy)
    {
        VDP_OVERSCAN_REGION_T rOverScan;

        VDP_DBG_MSG("MW_VDP_CallbackTrigger\n");
        VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        rOverScan.u4Top = _prVdpConf[ucVdpId]->rOverScan.u4Top;
        rOverScan.u4Bottom = _prVdpConf[ucVdpId]->rOverScan.u4Bottom;
        rOverScan.u4Left = _prVdpConf[ucVdpId]->rOverScan.u4Left;
        rOverScan.u4Right = _prVdpConf[ucVdpId]->rOverScan.u4Right;

        if (_VdpUpdateOverscan(ucVdpId, &(rOverScan.u4Top), &(rOverScan.u4Bottom), &(rOverScan.u4Left), &(rOverScan.u4Right)))
        {
            VERIFY(x_sema_unlock(_hMutex) == OSR_OK);

            VDP_SetOverScan(ucVdpId, rOverScan);
        }
        else
        {
            VERIFY(x_sema_unlock(_hMutex) == OSR_OK);
        }

        _VdpUpdateDispFmt(ucVdpId);
        if (_VdpXlatDispFmtCallBackMw(ucVdpId) != 0)
        {
            VDP_REGION_T rSrcRegion;
            VDP_REGION_T rOutRegion;

            rSrcRegion.u4X = _arMwVdp[ucVdpId].rFinalSource.ui4_x;
            rSrcRegion.u4Y = _arMwVdp[ucVdpId].rFinalSource.ui4_y;
            rSrcRegion.u4Width = _arMwVdp[ucVdpId].rFinalSource.ui4_width;
            rSrcRegion.u4Height = _arMwVdp[ucVdpId].rFinalSource.ui4_height;
            VDP_SetSrcRegion(ucVdpId, 0, rSrcRegion);

            rOutRegion.u4X = _arMwVdp[ucVdpId].rFinalOutput.ui4_x;
            rOutRegion.u4Y = _arMwVdp[ucVdpId].rFinalOutput.ui4_y;
            rOutRegion.u4Width = _arMwVdp[ucVdpId].rFinalOutput.ui4_width;
            rOutRegion.u4Height = _arMwVdp[ucVdpId].rFinalOutput.ui4_height;
            VDP_SetOutRegion(ucVdpId, 0, rOutRegion);
        }

        if (_arMwVdp[ucVdpId].e_type == DRVT_VID_DEC &&
            _arMwVdp[ucVdpId].u2SrcCompId != (UINT16)MPV_COMP_NS)
        {
            MW_MPV_NOTIFY(_arMwVdp[ucVdpId].u2SrcCompId);
        }
    }
    _arMwVdp[ucVdpId].ucSrcChgNfy = 0;
}

UCHAR MW_VDP_MEMO_MODE_GET(UCHAR ucVdpId)
{
    return (_arMwVdp[ucVdpId].ucMemoMode);
}

void MW_VDP_MEMO_MODE_SET_PMX_INFO(UCHAR ucVdpId, UCHAR ucPmxCompId, UCHAR ucLocalId)
{
    _arMwVdp[ucVdpId].ucPmxCompId = ucPmxCompId;
    _arMwVdp[ucVdpId].ucLocalId = ucLocalId;
}
void MW_VDP_ENABLE_CB(BOOL fEnable)
{ 
	fgEnableMwCb = fEnable;
}
#ifdef MULTIMEDIA_AUTOTEST_SUPPORT
VOID MM_AUTOTEST_SET_VDP_CALLBACK(UCHAR ucVdpId,UINT32 pfAtEosNfy,VOID *pv_Tag)
{
    _arMwVdp[ucVdpId].pfAtEosNfy=(x_vid_pla_nfy_fct)pfAtEosNfy;
    _arMwVdp[ucVdpId].pvATEosTag=pv_Tag;
}
#endif

