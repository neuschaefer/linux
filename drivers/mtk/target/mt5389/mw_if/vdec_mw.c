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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_mw.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         Video Decoder middleware driver API
 *---------------------------------------------------------------------------*/
#if 1//!CC_DRIVER_PROGRAM

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "general.h"
#include "video_def.h"
#include "vdo_misc.h"
// #include "api_video.h"
#include "drv_vbi.h"
#include "drv_scart.h"
#include "drvcust_if.h"
#include "source_select.h"
#include "source_table.h"
#include "drv_comp_id.h"

#include "x_rm_dev_types.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_tv_dec.h"
#include "x_tv_enc.h"

#include "x_debug.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_hamming_code.h"
#include "d_drv_cust_util.h"
#include "tvsrc_mw.h"
#include "avc_combi_mw.h"

#include "vdec_mw.h"
#include "hw_hdmi.h"
#include "drv_hdmi.h"
#include "drv_dvi.h"
#include "hw_vdoin.h"
/* For Cache and DRAM consistence */
#include "x_hal_926.h"

#include "drv_vga.h"
//#include "drv_async.h"
//#include "drv_video.h"
#include "video_timing.h"
#include "feature.h"
#include "tve_if.h"

#ifdef CC_3D_SUPPORT
#include "drv_tdtv_drvif.h"
#endif

#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_data_rm.h"
#include "cb_param.h"
#endif

extern UINT16 wSP0IHSClock(UINT16 whtotal) ;
extern UINT8 bSP0IVSClock(UINT16 whtotal, UINT16 wvtotal) ;
extern UINT16 wASHLenMeasure(void) ;
extern UINT16 wASVtotalMeasure(void) ;



/* For number of component */
#include "drv_comp_id.h"
/*-----------------------------------------------------------------------------
                    Macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* constants */
#define DRVN_TV_DEC   "tvdec_ccc"


#define DAC_1 0
#define DAC_2 1


/* NTSC CC config */
#define TVD_CC_NRL_STATE_COUNTER_THRESHOLD      5

#define VDEC_MW_TVD_CC_NRL_THRESHOLD_BLOCK              ((UINT16) (_u1CCNRLBlock))     /* block if Noise Level beyong this, range: (0~50) */
#define VDEC_MW_TVD_CC_NRL_THRESHOLD_UNBLOCK            ((UINT16) (_u1CCNRLUnblock))   /* unblock if Noise Level beyong this, range: (0~VDEC_MW_TVD_CC_NRL_THRESHOLD_BLOCK) */

#define TVD_CC_PER_STATE_COUNTER_THRESHOLD      5
#define TVD_CC_PER_SAMPLE_WINDOW_SIZE_BY_32     4
#define TVD_CC_PER_SAMPLE_WINDOW_SIZE           (32 * TVD_CC_PER_SAMPLE_WINDOW_SIZE_BY_32)

#define VDEC_MW_TVD_CC_PER_THRESHOLD_BLOCK              ((UINT16) (_u1CCPERBlock))     /* block if Parity Error Rate beyong this, range: (0~TVD_CC_PER_SAMPLE_WINDOW_SIZE) */
#define VDEC_MW_TVD_CC_PER_THRESHOLD_UNBLOCK            ((UINT16) (_u1CCPERUnblock))   /* unblock if Parity Error Rate below this, range: (0~VDEC_MW_TVD_CC_PER_THRESHOLD_BLOCK) */

#ifndef TVD_CC_ECC_OFF_NRL
#define TVD_CC_ECC_OFF_NRL                      0x2A //ECC OFF Thres Hold
#endif
//#define TVD_CC_ECC_SWITCH                       1  // Enable Switch CC ECC dynamically




#define M_IS_TUNER_SRC( e_type )   ( e_type == DRVT_TUNER_SAT_ANA ||         \
                                     e_type == DRVT_TUNER_CAB_ANA ||         \
                                     e_type == DRVT_TUNER_TER_ANA )

#define TVD_COLOR_SYS_CAP        (TV_DEC_COLOR_SYS_CAP_NTSC|TV_DEC_COLOR_SYS_CAP_NTSC_443|TV_DEC_COLOR_SYS_CAP_PAL|TV_DEC_COLOR_SYS_CAP_PAL_N|TV_DEC_COLOR_SYS_CAP_PAL_M|TV_DEC_COLOR_SYS_CAP_PAL_60|TV_DEC_COLOR_SYS_CAP_SECAM)

#define GET_VDP_ID_FROM_PATH(_u1Path)  ((UCHAR) u1Path)

#define TVD_CC_IS_STATE_STABLE(_u2CntSt, _u2Threshold)    ((_u2CntSt) >= (_u2Threshold))


#define VGA_CLOCK_MIN         (-127)
#define VGA_CLOCK_MAX         (128)
#define VGA_PHASE_MIN         0
#define VGA_PHASE_MAX         31

UINT16 wDrvVGAHPositionMax(void);
UINT16 wDrvVGAHPositionMin(void);
UINT16 wDrvVGAVPositionMax(void);
UINT16 wDrvVGAVPositionMin(void);
UINT8 wDrvVgaGetClockMax(void);


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/* NTSC CC Status */
typedef struct _TVD_CC_STATUS_T
{
    /* noise level */
    BOOL        fgBlockedNRL;       /* if CC is blocked by NRL or not */
    
#ifdef TVD_CC_ECC_SWITCH
    BOOL        fgECCOnOff;         /* TRUE: CC ECC On / False: CC ECC Off */
    UINT16      u2CntECCNRL;        /* state counter for CC */
#endif 

    UINT16      u2CntStNRL;         /* state counter for NRL */
    
    /* parity error rate */
    BOOL        fgBlockedPER;       /* if CC is blocked by PER or not */
    
    UINT16      u2CntStPER;         /* state counter for PER */
    
    UINT16      u2ErrRate;          /* parity error rate: (0~TVD_CC_PER_SAMPLE_WINDOW_SIZE) */

    UINT32      au4Samples[TVD_CC_PER_SAMPLE_WINDOW_SIZE_BY_32];
                                    /* window samples, 0-bit:normal, 1-bit:error */

} TVD_CC_STATUS_T;

/* TTX related */
typedef DLIST_T(_TTX_BUF_NODE)          DLIST_TB_ND;

typedef DLIST_ENTRY_T(_TTX_BUF_NODE)    DLIST_ENTRY_TB_ND;

typedef struct _TTX_BUF_NODE
{
    UINT32                  u4Magic;
    UINT16                  u2NumPkt;
    DLIST_ENTRY_TB_ND       t_link;
    
    TV_DEC_TTX_PKT_T        atPkts[1];
    
    /* more packet data starts here */
    
} TTX_BUF_NODE ;

#define GET_NODE_SIZE( _u2NumPkt )              \
       ((((UINT32) sizeof(TTX_BUF_NODE)) +      \
        (((UINT32) sizeof(TV_DEC_TTX_PKT_T)) * ((UINT32) ((_u2NumPkt) - 1))) + 3 ) & (~((UINT32) 0x03)))

struct _TVD_TTX_BUF_CTX
{
    /* constants */
    BOOL                bInited;
    BYTE*               pbBuf;
    UINT32              u4SzBuf;
    
    /* variables */
    UINT32              u4Wtr;
    DLIST_TB_ND         tLstNodes;
    
};
 

#define TTX_BUF_NUM_PKT     512
#define TTX_BUF_NUM_NODE    (TTX_BUF_NUM_PKT / 17)

#define TTX_BUFR_OK                 ((INT32)   0)
#define TTX_BUFR_OUT_OF_MEMORY      ((INT32) -10)
#define TTX_BUFR_UNDERFLOW          ((INT32) -11)
#define TTX_BUFR_OVERFLOW           ((INT32) -12)
#define TTX_BUFR_INV_OPR            ((INT32) -13)
#define TTX_BUFR_INTERNAL_ERROR     ((INT32) -14)


#define TTX_BUF_NODE_MAGIC          ((UINT32) 0xF50AF50A)

/*-----------------------------------------------------------------------------
                    Global Data declaration
 ----------------------------------------------------------------------------*/

/* TVD common */
UINT32                      _u4MwIfDbgFlags = 0;      /* can be controlled by driver CLI read/write */
static TV_DEC_NFY_INFO_T           _arTVNfyInfo[TVD_COMP_NS];
static BOOL                        _afgTvdIsConnected[TVD_COMP_NS];

/* NTSC CC */
static UINT8                       _u1CCNRLBlock;           /* constant: VDEC_MW_TVD_CC_NRL_THRESHOLD_BLOCK   */
static UINT8                       _u1CCNRLUnblock;         /* constant: VDEC_MW_TVD_CC_NRL_THRESHOLD_UNBLOCK */
static UINT8                       _u1CCPERBlock;           /* constant: VDEC_MW_TVD_CC_PER_THRESHOLD_BLOCK   */
static UINT8                       _u1CCPERUnblock;         /* constant: VDEC_MW_TVD_CC_PER_THRESHOLD_UNBLOCK */
static BOOL                        _afgCCPlaying[TVD_COMP_NS];
static TVD_CC_STATUS_T             _arCCStatus[TVD_COMP_NS];
static TV_DEC_CC_NFY_INFO_T        _arCCNfyInfo[TVD_COMP_NS];

/* EU TTX */
typedef struct _TVD_TTX_BUF_CTX    TVD_TTX_BUF_CTX;
static TV_DEC_TTX_NFY_INFO_T       _arTtxNfyInfo[TVD_COMP_NS];
static BOOL                        _afgTtxEnabled[TVD_COMP_NS];
static BOOL                        _afgTtxBufErrPaused[TVD_COMP_NS];
static TVD_TTX_BUF_CTX             _arTtxBufCtx[TVD_COMP_NS];

/* EU VPS DATA */
static TV_DEC_VPS_DATA_NFY_INFO_T  _rVpsDataNfyInfo;
static UINT8                       _au1VpsDataNtfd[TV_DEC_VPS_DATA_SIZE];

/* EU WSS */
static UINT16                      _au2EuWssLastRcv[TVD_COMP_NS];
static BOOL                        _afgEuWssPlayed[TVD_COMP_NS];
static TV_DEC_EU_WSS_NFY_INFO_T    _arEuWssNfyInfo[TVD_COMP_NS];
static UINT16                      _au2EuWss625[TVD_COMP_NS];
static BOOL                        _afgEuWss625Coming[TVD_COMP_NS] = {FALSE};


/* US WSS525 */
static UINT16                      _au2UsWssLastRcv[TVD_COMP_NS];
static BOOL                        _afgUsWssPlayed[TVD_COMP_NS];
static TV_DEC_US_WSS_NFY_INFO_T    _arUsWssNfyInfo[TVD_COMP_NS];
static UINT32                      _au4UsWss525[TVD_COMP_NS];
static BOOL                        _afgUsWss525Coming[TVD_COMP_NS] = {FALSE};

#ifdef TVD_CC_ECC_SWITCH
UINT32 g_u4ECCThresHold = TVD_CC_ECC_OFF_NRL;
#endif

/* source auto detect*/
#ifdef CC_SOURCE_AUTO_DETECT
static TV_DEC_SRC_DETECT_NFY_INFO_T _arSRCDetectNfyInfo[TVD_COMP_NS];
extern VOID vDrvGetSrcDetectStatus(_TV_DEC_SOURCE_STATUS_T* pv_get_info);
#endif


//Signal Detect 
extern UINT32                      _u4SigDetDone;

#ifdef LINUX_TURNKEY_SOLUTION
static UINT32 ui4_ttx_kernel_virt_start[TVD_COMP_NS];
#endif

extern  UINT8 bDrvPorchTune[2];

/*-----------------------------------------------------------------------------
                    constants declaration
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    function prototypes
 ----------------------------------------------------------------------------*/
static INT32 _TvdMwConnect(
    DRV_COMP_ID_T*   pt_comp_id,
    DRV_CONN_TYPE_T  e_conn_type,
    const VOID*      pv_conn_info,
    SIZE_T           z_conn_info_len,
    VOID*            pv_tag,
    x_rm_nfy_fct     pf_nfy);

static INT32 _TvdMwDisconnect(
    DRV_COMP_ID_T*   pt_comp_id,
    DRV_DISC_TYPE_T  e_disc_type,
    const VOID*      pv_disc_info,
    SIZE_T           z_disc_info_len);

static INT32 _TvdMwGet(
    DRV_COMP_ID_T*  pt_comp_id,
    DRV_GET_TYPE_T  e_get_type,
    VOID*           pv_get_info,
    SIZE_T*         pz_get_info_len);

static INT32 _TvdMwSet(
    DRV_COMP_ID_T*  pt_comp_id,
    DRV_SET_TYPE_T  e_set_type,
    const VOID*     pv_set_info,
    SIZE_T          z_set_info_len);

static void _TvdProcessEuWssData(
    UINT8           u1Path, 
    UINT16          u2EuWss);

/* US WSS525 */
static void _TvdProcessUsWssData(
    UINT8           u1Path, 
    UINT16          u2EuWss);

static void _TvdProcessTtxData(
    UINT8 u1Path, 
    UINT16 u2NumPktAvail, 
    UINT32 _dVbiReadPtr);


extern BOOL VdoMLInit(void);

extern UINT16 wDrvVGAGetClock(void);
extern UINT16 wDrvVGAGetHPosition(void);
extern UINT16 wDrvVGAGetVPosition(void);
extern UINT8 u1DrvVGAGetPhase(void);
extern UINT8 bApiVideoGetInputTimingID(UINT8 bPath);
extern UINT8 bApiVideoGetInputPolarity(UINT8 bPath);
extern UINT8 bHDMIScanInfo(void);
extern UINT8 bHDMIAFD(void);
extern UINT8 bHDMIInputType(void);
extern UINT8 bHDMIAspectRatio(void);
extern UINT8 bTvdNRLevel(void);
extern UINT8  _bDviTiming;
extern UINT8 fgApiVideoVgaSetPhase(UINT8 bValue) ;

/*extern void _MW_VDP_HDMI_CB(UCHAR ucVdpId, UINT16 u2Data);*/
extern void _MW_VDP_SCART_PIN_8_CB(UCHAR ucVdpId, DRV_CUSTOM_SCART_PIN_8_T t_scart_pin_8);
extern void _MW_VDP_HDMI_AFD_CB(UCHAR ucVdpId, UINT16 u2Data);
#if 0 //SUPPORT_WSS
extern UINT8 bAPIWSSSwitch(UINT8 bPath);
#endif

extern void MW_VDP_CallbackTrigger(UCHAR ucVdpId);
extern void _MW_VDP_HDMI_AVI_INFOFRAME_CB(UCHAR ucVdpId, DRV_CUSTOM_VID_SRC_HDMI_AVI_INFOFRAME_INFO_T u2Data);
extern void _MW_VDP_WSS625_CB(UCHAR ucVdpId, UINT16 u2Data);
extern void _MW_VDP_WSS525_CB(UCHAR ucVdpId, UINT16 u2Data);

#ifdef LINUX_TURNKEY_SOLUTION
extern INT32 _CB_PutEvent_Adpt(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
extern void * _kernal_mem_alloc_pages(SIZE_T size);
extern UINT32 ttx_addr_user_to_kernel(UINT8 ui1_path, UINT32 ui4_addr);
extern UINT32 ttx_addr_kernel_to_user(UINT8 ui1_path, UINT32 ui4_addr);
#endif

/******************************************************************************
* Global Function definition
******************************************************************************/

// CINDY 2006/05/30 ADD FOR MIDDLEWARE DRIVER TEST, SHOULD FIGURE OUT SOME WAY WITH IOLO ABOUT
// GET/SET VGA/YPBPR RELATED PROPERTY
INT16     i2GetTypeClock;
UINT16    u2GetTypePhase;
UINT16    u2GetTypeHPos;
UINT16    u2GetTypeVPos;

INT8     bGetSrc;

static _NotifyStatus _tMainNotifyStatus;
static _NotifyStatus _tPipNotifyStatus;
static _NotifyStatus _tTveNotifyStatus;

#ifdef LINUX_TURNKEY_SOLUTION
static VOID set_tv_dec_nfy(UINT32           ui4_id,
                           void*            pv_nfy_tag,
                           TV_DEC_COND_T    e_nfy_cond,
                           UINT32           ui4_data_1,
                           UINT32           ui4_data_2)
{
    VOID * pv_buf;
    TV_DEC_NFY_PARAM tv_dec_param;
    tv_dec_param.pf_nfy = _arTVNfyInfo[ui4_id].pf_tv_dec_nfy;
    pv_buf = (VOID *)ui4_data_1;
    if (e_nfy_cond == TV_DEC_COND_RESOLUTION_CHG)
    {
#if 0
        lock_nfy_param_buf(PARAM_TVD_NFY);
        pv_buf = get_nfy_param_buf(PARAM_TVD_NFY,sizeof(TV_DEC_RESOLUTION_INFO_T));
        rm_copy_to_user(pv_buf,ui4_data_1,sizeof(TV_DEC_RESOLUTION_INFO_T));
#else
        tv_dec_param.t_data_1 = *((TV_DEC_RESOLUTION_INFO_T *)ui4_data_1);
#endif
    }
    tv_dec_param.pv_nfy_tag = pv_nfy_tag;
    tv_dec_param.e_nfy_cond = e_nfy_cond;
    tv_dec_param.ui4_data_1 = (UINT32)pv_buf;
    tv_dec_param.ui4_data_2 = ui4_data_2;
    _CB_PutEvent_Adpt(CB_DRV_TV_DEC_NFY, sizeof(TV_DEC_NFY_PARAM), &tv_dec_param);
#if 0
    if (e_nfy_cond == TV_DEC_COND_RESOLUTION_CHG)
    {
        unlock_nfy_param_buf(PARAM_TVD_NFY);
    }
#endif
}

static VOID set_cc_nfy(UINT32           ui4_id,
                       VOID*            pv_tag,
                       TV_DEC_CC_COND_T e_cc_cond,
                       VOID*            pv_arg)
{
#if 0 /* For fix in_interrupt case */
    CC_NFY_PARAM cc_param;
    cc_param.pf_nfy =  _arCCNfyInfo[ui4_id].pf_cc_nfy;
    cc_param.pv_tag = pv_tag;
    cc_param.e_cc_cond = e_cc_cond;
    lock_nfy_param_buf(PARAM_CC_NFY);
    cc_param.pv_arg = get_nfy_param_buf(PARAM_CC_NFY,sizeof(TV_DEC_CC_DATA_T));
    rm_copy_to_user(cc_param.pv_arg,pv_arg, sizeof(TV_DEC_CC_DATA_T));
    _CB_PutEvent_Adpt(CB_DRV_CC_NFY, sizeof(CC_NFY_PARAM), &cc_param);
    unlock_nfy_param_buf(PARAM_CC_NFY);
#else
    CC_NFY_PARAM cc_param;
    TV_DEC_CC_DATA_T* p_tmp;
    
    cc_param.pf_nfy =  _arCCNfyInfo[ui4_id].pf_cc_nfy;
    cc_param.pv_tag = pv_tag;
    cc_param.e_cc_cond = e_cc_cond;
    p_tmp = (TV_DEC_CC_DATA_T *)pv_arg;

    if(NULL != p_tmp)
    {
        cc_param.b_even_field = p_tmp->b_even_field;
        cc_param.ui1_byte1 = p_tmp->ui1_byte1;
        cc_param.ui1_byte2 = p_tmp->ui1_byte2;
    }

    _CB_PutEvent_Adpt(CB_DRV_CC_NFY, sizeof(CC_NFY_PARAM), &cc_param);
#endif
}

static VOID set_tv_dec_ttx_nfy(UINT32         ui4_id,
                           VOID*              pv_nfy_tag,
                           TV_DEC_TTX_COND_T  e_nfy_cond,
                           VOID*              pv_arg,
                           UINT32             pv_arg2)
{
    VOID * pv_buf;
    TV_DEC_TTX_NFY_PARAM tv_dec_ttx_nfy_param;
    tv_dec_ttx_nfy_param.pf_nfy = _arTtxNfyInfo[ui4_id].pf_ttx_nfy;
    pv_buf = (VOID *)pv_arg;    
    if (e_nfy_cond == TV_DEC_TTX_COND_DATA_ARRIVAL)
    {
        tv_dec_ttx_nfy_param.t_data_1= *((TV_DEC_TTX_DATA_T *)pv_arg);
    }   
    tv_dec_ttx_nfy_param.pv_nfy_tag = pv_nfy_tag;
    tv_dec_ttx_nfy_param.ui4_data_0 = ui4_id;
    tv_dec_ttx_nfy_param.ui4_data_1 = (UINT32)pv_buf;
    if (e_nfy_cond == TV_DEC_TTX_COND_MEM_MAP)
    {
        tv_dec_ttx_nfy_param.ui4_data_2 = pv_arg2;
    } 
    tv_dec_ttx_nfy_param.e_nfy_cond = e_nfy_cond;
    _CB_PutEvent_Adpt(CB_DRV_TV_DEC_TTX_NFY, sizeof(TV_DEC_TTX_NFY_PARAM), &tv_dec_ttx_nfy_param);
}

static VOID set_tv_dec_vps_data_nfy(VOID*          pv_nfy_tag,
                           TV_DEC_VPS_DATA_COND_T  e_nfy_cond,
                           VOID*                   pv_arg)
{    
    TV_DEC_VPS_DATA_NFY_PARAM tv_dec_vps_data_nfy_param;
    TV_DEC_VPS_DATA_T* p_tmp = (TV_DEC_VPS_DATA_T *)pv_arg;
    
    tv_dec_vps_data_nfy_param.pf_nfy = _rVpsDataNfyInfo.pf_vps_data_nfy;
    tv_dec_vps_data_nfy_param.pv_nfy_tag = pv_nfy_tag;
    tv_dec_vps_data_nfy_param.e_nfy_cond = e_nfy_cond;
    
    if(NULL != p_tmp)
    {
        x_memcpy(tv_dec_vps_data_nfy_param.aui1_vps_data, p_tmp->aui1_data, TV_DEC_VPS_DATA_SIZE);
    }
    _CB_PutEvent_Adpt(CB_DRV_TV_DEC_VPS_DATA_NFY, sizeof(TV_DEC_VPS_DATA_NFY_PARAM), &tv_dec_vps_data_nfy_param);
}

static VOID set_tv_dec_eu_wss_nfy(UINT32         ui4_id,
                           VOID*                 pv_nfy_tag,
                           TV_DEC_EU_WSS_COND_T  e_nfy_cond,
                           VOID*                 pv_arg)
{
    TV_DEC_EU_WSS_NFY_PARAM tv_dec_eu_wss_nfy_param;

    tv_dec_eu_wss_nfy_param.pf_nfy = _arEuWssNfyInfo[ui4_id].pf_eu_wss_nfy;
    tv_dec_eu_wss_nfy_param.t_data_1= *((TV_DEC_EU_WSS_DATA_T *)pv_arg);
    tv_dec_eu_wss_nfy_param.e_nfy_cond = e_nfy_cond;
    _CB_PutEvent_Adpt(CB_DRV_TV_DEC_EU_WSS_NFY, sizeof(TV_DEC_EU_WSS_NFY_PARAM), &tv_dec_eu_wss_nfy_param);
}

#ifdef CC_SOURCE_AUTO_DETECT
static VOID set_tv_dec_src_detect_nfy(UINT32 ui4_id,
									VOID*           pv_nfy_tag,
									TV_DEC_COND_T   e_nfy_cond,
									UINT32          ui4_data_1,
									UINT32          ui4_data_2)
{
	VOID* pv_buf;
	TV_DEC_SRC_DETECT_NFY_PARAM tv_dec_src_detect_param;
	tv_dec_src_detect_param.pf_nfy = _arSRCDetectNfyInfo[ui4_id].pf_src_detect_nfy;
	pv_buf = (VOID *)ui4_data_1;

	tv_dec_src_detect_param.pv_nfy_tag = pv_nfy_tag;
	tv_dec_src_detect_param.e_nfy_cond = e_nfy_cond;
	tv_dec_src_detect_param.ui4_data_1 = (UINT32)pv_buf;
	tv_dec_src_detect_param.ui4_data_2 = ui4_data_2;
	_CB_PutEvent_Adpt(CB_DRV_TV_DEC_SRC_DETECT_NFY,sizeof(TV_DEC_SRC_DETECT_NFY_PARAM),&tv_dec_src_detect_param);


}
#endif

#endif

#if 1

static VOID _TvdResetNfyStatus( UINT32  u4VidPath )
{
#ifndef CC_DRIVER_PROGRAM
    CRIT_STATE_T              t_cs;

    t_cs = x_crit_start();

    if ( u4VidPath == SV_VP_MAIN )
    {
        _tMainNotifyStatus.u1SigStatus = SV_VDO_UNKNOWN;
        _tMainNotifyStatus.u1ColSys = SV_CS_MAX;
        _tMainNotifyStatus.t_resolution.ui2_frame_rate = 0;
        _tMainNotifyStatus.t_resolution.ui2_width = 0;
        _tMainNotifyStatus.t_resolution.ui2_height = 0;
        _tMainNotifyStatus.t_resolution.b_is_progressive = FALSE;
        _tMainNotifyStatus.t_resolution.e_src_asp_ratio = TV_DEC_SRC_ASPECT_RATIO_UNKNOWN;
        _tMainNotifyStatus.t_resolution.e_timing_type = TV_DEC_TIMING_UNKNOWN;
        _tMainNotifyStatus.t_resolution.ui4_res_idx = 0;
        _tMainNotifyStatus.u1NeedNotify = FALSE;
    }
    else
    {
        _tPipNotifyStatus.u1SigStatus = SV_VDO_UNKNOWN;
        _tPipNotifyStatus.u1ColSys = SV_CS_MAX;
        _tPipNotifyStatus.t_resolution.ui2_frame_rate = 0;
        _tPipNotifyStatus.t_resolution.ui2_width = 0;
        _tPipNotifyStatus.t_resolution.ui2_height = 0;
        _tPipNotifyStatus.t_resolution.b_is_progressive = FALSE;
        _tPipNotifyStatus.t_resolution.e_src_asp_ratio = TV_DEC_SRC_ASPECT_RATIO_UNKNOWN;
        _tPipNotifyStatus.t_resolution.e_timing_type = TV_DEC_TIMING_UNKNOWN;
        _tPipNotifyStatus.t_resolution.ui4_res_idx = 0;
        _tPipNotifyStatus.u1NeedNotify = FALSE;
    }

    x_crit_end(t_cs);

#endif
}

/*-----------------------------------------------------------------------------
                    teletext buffer functions implementations
 ----------------------------------------------------------------------------*/

static VOID _TvdTtxBufSysInit(
    TVD_TTX_BUF_CTX*            ptCtx)
{
    x_memset( ptCtx, 0, sizeof( TVD_TTX_BUF_CTX ) );
    
    ptCtx->bInited = FALSE;
    ptCtx->pbBuf   = NULL;
    ptCtx->u4SzBuf = 0;
    ptCtx->u4Wtr   = 0;
    DLIST_INIT( & ptCtx->tLstNodes );
} 

#ifndef LINUX_TURNKEY_SOLUTION
static INT32 _TvdTtxBufInit(
    TVD_TTX_BUF_CTX*            ptCtx)
#else
static INT32 _TvdTtxBufInit(
    UINT8 ui1_path,
    TVD_TTX_BUF_CTX*            ptCtx)
#endif
{
    /* context: must be called from a thread context and no multithreads enter concurrently */
#ifdef LINUX_TURNKEY_SOLUTION
    if(ui1_path>=TVD_COMP_NS)
        return TTX_BUFR_INTERNAL_ERROR;
#endif

    if ( ptCtx->bInited )
    {
        return TTX_BUFR_INV_OPR;
    }
    
    ptCtx->u4SzBuf =  ( ( (UINT32) ( sizeof( TV_DEC_TTX_PKT_T ) * TTX_BUF_NUM_PKT + 
                                     sizeof( TTX_BUF_NODE ) * TTX_BUF_NUM_NODE + 3 ) )
                            & ( ~((UINT32) 0x03) ) );
#ifndef LINUX_TURNKEY_SOLUTION
    ptCtx->pbBuf = (BYTE *) x_mem_alloc( ptCtx->u4SzBuf );
#else
    /* the size should be page size*/        
    ui4_ttx_kernel_virt_start[ui1_path]  = (UINT32)_kernal_mem_alloc_pages( ptCtx->u4SzBuf);        
    ptCtx->pbBuf = (BYTE *)ui4_ttx_kernel_virt_start[ui1_path];

    set_tv_dec_ttx_nfy(ui1_path,
        NULL,
        TV_DEC_TTX_COND_MEM_MAP,
        (VOID *)ui4_ttx_kernel_virt_start[ui1_path],
        (UINT32)ptCtx->u4SzBuf);
#endif
    
    if ( ptCtx->pbBuf == NULL )
    {
        return TTX_BUFR_OUT_OF_MEMORY;
    }
    
    ptCtx->u4Wtr   = 0;
    DLIST_INIT( & ptCtx->tLstNodes );
    ptCtx->bInited = TRUE;
    
    return TTX_BUFR_OK;
}

static BOOL _TvdTtxBufChkInited(
    const TVD_TTX_BUF_CTX*      ptCtx)
{
    return ptCtx->bInited;
}        

static INT32 _TvdTtxBufAlloc( 
    TVD_TTX_BUF_CTX*            ptCtx,
    UINT16                      u2NumPkt,
    TV_DEC_TTX_PKT_T**          ppatPkts)
{
    UINT32          u4SzData;
    TTX_BUF_NODE    *ptNodeNew = NULL;
    CRIT_STATE_T    tCS;
    INT32           i4Ret;
    
    *ppatPkts = NULL;
    
    if ( u2NumPkt == 0 )
    {
        return TTX_BUFR_INV_OPR;
    }
    
    u4SzData = GET_NODE_SIZE( u2NumPkt );

    i4Ret = TTX_BUFR_OK;
    tCS = x_crit_start();
    
    if ( DLIST_IS_EMPTY( & ptCtx->tLstNodes ) )
    {
        if ( u4SzData <= ptCtx->u4SzBuf )
        {
            ptNodeNew    = (TTX_BUF_NODE *) (ptCtx->pbBuf);
            ptCtx->u4Wtr = u4SzData;
        }
        else
        {
            i4Ret = TTX_BUFR_OVERFLOW;
            goto _ERR_TvdTtxBufAlloc_1;
        }
    }
    else
    {
        UINT32    u4Rdr;
        
        u4Rdr = ((UINT32) DLIST_HEAD( & ptCtx->tLstNodes )) - ((UINT32) ptCtx->pbBuf);
        
        if (ptCtx->u4Wtr > u4Rdr)
        {
            if ( ptCtx->u4Wtr + u4SzData <= ptCtx->u4SzBuf )
            {
                ptNodeNew     = (TTX_BUF_NODE *) (ptCtx->pbBuf + ptCtx->u4Wtr);
                ptCtx->u4Wtr += u4SzData;
            }
            else
            {
                if ( u4SzData <= u4Rdr )
                {
                    ptNodeNew    = (TTX_BUF_NODE *) (ptCtx->pbBuf);
                    ptCtx->u4Wtr = u4SzData;
                }
                else
                {
                    i4Ret = TTX_BUFR_OVERFLOW;
                    goto _ERR_TvdTtxBufAlloc_1;
                }
            }
        }
        else
        {
            if (ptCtx->u4Wtr + u4SzData <= u4Rdr)
            {
                ptNodeNew     = (TTX_BUF_NODE *) (ptCtx->pbBuf + ptCtx->u4Wtr);
                ptCtx->u4Wtr += u4SzData;
            }
            else
            {
                i4Ret = TTX_BUFR_OVERFLOW;
                goto _ERR_TvdTtxBufAlloc_1;
            }
        }
    }
    
    DLIST_INSERT_TAIL( ptNodeNew, & ptCtx->tLstNodes, t_link );


_ERR_TvdTtxBufAlloc_1:

    x_crit_end( tCS );
    
    if ( i4Ret == TTX_BUFR_OK )
    {
        /* init ptNodeNew */
        ptNodeNew->u4Magic  = TTX_BUF_NODE_MAGIC;
        ptNodeNew->u2NumPkt = u2NumPkt;
        
        *ppatPkts           = ptNodeNew->atPkts;
    }
    
    return i4Ret;
}

static BOOL __TtxBufSrchNode(
    const DLIST_TB_ND*          ptLstNodes,
    UINT16                      u2NumPkt,
    const TV_DEC_TTX_PKT_T*     patPkts)
{
    TTX_BUF_NODE               *ptNode;
    
    ptNode  = DLIST_HEAD( ptLstNodes );
    while ( ptNode )
    {
        if ( ptNode->u4Magic  == TTX_BUF_NODE_MAGIC &&
             ptNode->u2NumPkt == u2NumPkt           &&
             ptNode->atPkts   == patPkts)
        {
            return TRUE;
        }
             
        ptNode = DLIST_NEXT( ptNode, t_link );
    } 
    
    return FALSE;
}    

static void __TtxBufFreeNodes(
    DLIST_TB_ND*                ptLstNodes,
    const TV_DEC_TTX_PKT_T*     patPktsTarget)
{
    TV_DEC_TTX_PKT_T            *patPktsNode;
    TTX_BUF_NODE                *ptNode;
    
    do 
    {
        ptNode      = DLIST_HEAD( ptLstNodes );
        patPktsNode = ptNode->atPkts;
        DLIST_REMOVE( ptNode, ptLstNodes, t_link );
        
    } while ( (patPktsNode != patPktsTarget) && (! DLIST_IS_EMPTY(ptLstNodes)) );
    
}

static INT32 _TvdTtxBufFree( 
    TVD_TTX_BUF_CTX*            ptCtx, 
    UINT16                      u2NumPkt,
    TV_DEC_TTX_PKT_T*           patPkts)
{
    CRIT_STATE_T    tCS;
    INT32           i4Ret;
    
    i4Ret = TTX_BUFR_OK;
    
    tCS = x_crit_start();
    
    if ( DLIST_IS_EMPTY ( & ptCtx->tLstNodes ) )
    {
        i4Ret = TTX_BUFR_UNDERFLOW;
        goto _ERR_TvdTtxBufFree_1;
    }
    
    /* search for the corresponding node */
    if ( __TtxBufSrchNode( & ptCtx->tLstNodes, u2NumPkt, patPkts ) )
    {
        __TtxBufFreeNodes( & ptCtx->tLstNodes, patPkts );
    }
    else
    {
        i4Ret = TTX_BUFR_INTERNAL_ERROR;
        goto _ERR_TvdTtxBufFree_1;
    }
    
_ERR_TvdTtxBufFree_1:

    x_crit_end( tCS );
        
        
    return i4Ret;
}

static void _TvdTtxBufFlush(
    TVD_TTX_BUF_CTX*            ptCtx )
{
    CRIT_STATE_T    tCS;
    
    tCS = x_crit_start();

    DLIST_INIT( & ptCtx->tLstNodes );
    ptCtx->u4Wtr = 0;
    
    x_crit_end( tCS );
}    

static void _TvdEuWssStartNotify (UINT8 u1Path)
{
    // reset some variables before playing
    _au2EuWssLastRcv[u1Path] = 0xFF;
    _au2EuWss625[u1Path]     = 0xFF;
    _afgEuWssPlayed[u1Path]  = FALSE;
    
    VBI_StartNotifyWSSOnOff(u1Path, SV_ON, NULL);
}

static void _TvdEuWssResetData (UINT8 u1Path)
{
    // reset some variables before playing
    if(u1Path < TVD_COMP_NS)
    {
        _au2EuWssLastRcv[u1Path] = 0xFF;
        _au2EuWss625[u1Path]     = 0xFF;
        _afgEuWssPlayed[u1Path]  = FALSE;
    }
}


static void _TvdEuWssStopNotify (UINT8 u1Path)
{
    VBI_StartNotifyWSSOnOff(u1Path, SV_OFF, NULL);
}


/* US WSS525 */
static void _TvdUsWssStartNotify (UINT8 u1Path)
{
    _au2UsWssLastRcv[u1Path] = 0xFF;
    _au4UsWss525[u1Path]     = 0xFF;
    _afgUsWssPlayed[u1Path]   = FALSE;
    
    VBI_StartNotifyUSWSSOnOff(u1Path, SV_ON, NULL);
}

static void _TvdUsWssResetData (UINT8 u1Path)
{
    if(u1Path < TVD_COMP_NS)
    {
        _au2UsWssLastRcv[u1Path] = 0xFF;
        _au4UsWss525[u1Path]     = 0xFF;
        _afgUsWssPlayed[u1Path]   = FALSE;
    }
}


/* US WSS525 */
static void _TvdUsWssStopNotify (UINT8 u1Path)
{
    VBI_StartNotifyUSWSSOnOff(u1Path, SV_OFF, NULL);
}

    
static TV_DEC_TV_SIG_T _drv_tv_sig_sts_2_mw( UINT8 u1SigStat )
{
    switch( u1SigStat )
    {
    case SV_VDO_NOSIGNAL:
        return TV_DEC_TV_SIG_NONE;
        
    case SV_VDO_NOSUPPORT:
    case SV_VDO_STABLE:
        return TV_DEC_TV_SIG_STABLE;
        
    case SV_VDO_UNKNOWN:
    default:
        return TV_DEC_TV_SIG_UNKNOWN;
    }
}


#include "x_os.h"
HANDLE_T hMLEvtBrg_Thread;
HANDLE_T hMLEvtBrg_MsgQueue;

#define MLEvtBrg_STACK_SIZE		2048
#define MLEvtBrg_THREAD_PRIORITY	70

typedef  struct {
	UINT8 cmd;
	UINT8 b1; /*option arg*/
	UINT8 b2; /*option arg*/
	UINT8 b3; /*option arg*/
	UINT32 d1;
	UINT32 d2;
	UINT32 d3;
} MLEvtBrg_MSG_T;

enum MLEvtBrg_Cmd{
    MLEvt_WSS_EU = 1,
    MLEvt_SCARTFS = 2,
    MLEvt_TTX = 3,
    MLEvt_WSS_US = 4,
    MLEvt_Max
};

typedef enum
{
    VDP_SCART_PIN_8_UNKNOWN = 0,
    VDP_SCART_PIN_8_NO_SIGNAL,    
    VDP_SCART_PIN_8_ASPECT_16_9,
    VDP_SCART_PIN_8_ASPECT_4_3
}   VDP_SCART_PIN_8_T;

void _MLEvtBridgeRoutine(void * args)
{
	MLEvtBrg_MSG_T msg;
	UINT16 idx;
	SIZE_T zMsgSize;

	zMsgSize = sizeof(MLEvtBrg_MSG_T);
	while(1)
	{
		VERIFY(x_msg_q_receive(&idx, (void *) (&msg), &zMsgSize,
		    &(hMLEvtBrg_MsgQueue), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
		ASSERT(zMsgSize == sizeof(MLEvtBrg_MSG_T));
		switch(msg.cmd)
		{
			case MLEvt_WSS_EU:	//(UINT8 u1Path, UINT16 u2Data)
            {
                _TvdProcessEuWssData ((UINT8) msg.d1, (UINT16) msg.d2);
                if (_u4SigDetDone)
                {
                    UINT8 u1Path = (UINT8)msg.d1;  
                    if (u1Path < TVD_COMP_NS)
                    {
                        _u4SigDetDone = 0;
                        if (_au2EuWssLastRcv[u1Path] != 0xFF)
                        {
                            _au2EuWssLastRcv[u1Path] = 0xFF;
                            _au2EuWss625[u1Path] = 0xFF;
                        }
                        _afgEuWssPlayed[u1Path]  = FALSE;
                        TVD_DBG_MSG("_MLEvtBridgeRoutine: Reset Eu wss variables.\n");
                    }
                }
                break;
            }

            case MLEvt_SCARTFS:
            {
                _MW_VDP_SCART_PIN_8_CB((UCHAR) msg.d1, (DRV_CUSTOM_SCART_PIN_8_T)(VDP_SCART_PIN_8_T)msg.d2);
                break;
            }

            case MLEvt_TTX: 
            {
                _TvdProcessTtxData((UINT8) msg.d1, (UINT16) msg.d2, (UINT32) msg.d3);
                break;
            }
            /* US WSS525 */
            case MLEvt_WSS_US:
            {
                _TvdProcessUsWssData ((UINT8) msg.d1, (UINT16) msg.d2);
                if (_u4SigDetDone)
                {
                    UINT8 u1Path = (UINT8)msg.d1; 
                    if (u1Path < TVD_COMP_NS)
                    {
                        _u4SigDetDone = 0;
                        _au2UsWssLastRcv[u1Path] = 0xFF;
                        _afgUsWssPlayed[u1Path]  = FALSE;
                        TVD_DBG_MSG("_MLEvtBridgeRoutine: Reset US wss variables.\n");
                    }
                }    
                break;
            }
			default: 
				TVD_DBG_MSG("MLEvtBrg recived unsupported cmd [%d]\n", (unsigned int)msg.cmd);
				break;
		}
	}
}


/* for 6 arg ,pass NULL if arg not used, for good code style & easy refernce*/
void vMLEvtBridge_MsgSend_fmt2(UINT8 cmd, UINT32 d1, UINT32 d2, UINT32 d3, UINT8 b1, UINT8 b2, UINT8 b3)
{
	MLEvtBrg_MSG_T msg; /* using stack is ok, becuase send will copy to queue */
	
	msg.cmd=cmd;
	msg.d1=d1;
	msg.d2=d2;
	msg.d3=d3;
	msg.b1=b1;
	msg.b2=b2;
	msg.b3=b3;
	/* call x_msg_q_send is ok for ISR, since only x_sema_unlock, no x_sema_lock */
	VERIFY (x_msg_q_send(hMLEvtBrg_MsgQueue, (void *) (&msg), sizeof(MLEvtBrg_MSG_T), 255) == OSR_OK);
}

/*  for 3 arg  ,pass NULL if arg not used, for good code style & easy refernce*/
void vMLEvtBridge_MsgSend(UINT8 cmd, UINT32 d1, UINT32 d2, UINT32 d3)
{
	vMLEvtBridge_MsgSend_fmt2(cmd,d1,d2,d3,0,0,0);
}

void vMLEvtBridge_Init(void)
{
        // create message Q
        VERIFY(x_msg_q_create(&hMLEvtBrg_MsgQueue, "MLEvtBrg_MsgQ", sizeof(MLEvtBrg_MSG_T), 64) == OSR_OK);
        // create thread
        VERIFY (x_thread_create(&hMLEvtBrg_Thread, "MLEvtBrg", MLEvtBrg_STACK_SIZE, MLEvtBrg_THREAD_PRIORITY,
            _MLEvtBridgeRoutine, 0, NULL) == OSR_OK);


}



INT32 MW_TvdInit( DRV_CUSTOM_DIR_T* pt_drv_cust_dir )
{
#ifndef CC_DRIVER_PROGRAM
    
    INT32 i4Ret;
    DRV_COMP_REG_T tTvdCompReg;
    //  DRV_COMP_REG_T tTvdCompExclReg;
    DRV_COMP_FCT_TBL_T tTvdFctTbl;
    UINT32 u4Idx, u4Val;
    
    
    i2GetTypeClock = 0;
    u2GetTypePhase = 0;
    u2GetTypeHPos = 0;
    u2GetTypeVPos = 0;
    
    // Nptv Init
    vMLEvtBridge_Init();
    VdoMLInit();



    /* register mpv component */
    for (u4Idx = 0; u4Idx < TVD_COMP_NS; u4Idx++)
    {
        UINT32 u4Flags;
        CHAR   s_name[17];


        // Reset Glue Layer Status
        _TvdResetNfyStatus(u4Idx);

        /* Setup component ID */
        tTvdCompReg.e_type = DRVT_TV_DEC;
        tTvdCompReg.e_id_type = ID_TYPE_IND;
        tTvdCompReg.u.t_ind.ui2_id = u4Idx;
        tTvdCompReg.u.t_ind.ui1_port = ALL_PORTS;
        tTvdCompReg.u.t_ind.pv_tag = NULL;
        
        /* Registration Flags */
        u4Flags = 0;
        
        /* Setup component function table */
        tTvdFctTbl.pf_rm_connect = _TvdMwConnect;
        tTvdFctTbl.pf_rm_disconnect = _TvdMwDisconnect;
        tTvdFctTbl.pf_rm_get = _TvdMwGet;
        tTvdFctTbl.pf_rm_set = _TvdMwSet;

        if(DRVN_TV_DEC != NULL)
        {
            x_snprintf( s_name, sizeof(s_name), "%s%d", DRVN_TV_DEC, u4Idx);
            
            /* Register component with the Resource Manager database */
            i4Ret = x_rm_reg_comp(&tTvdCompReg,
                                  1,
                                  1,
                                  /*(DRVN_TV_DEC == NULL) ? NULL : */s_name,
                                  u4Flags,
                                  &tTvdFctTbl,
                                  NULL,
                                  0);
        }
        else
        {
            i4Ret = x_rm_reg_comp(&tTvdCompReg,
                                  1,
                                  1,
                                  NULL,
                                  u4Flags,
                                  &tTvdFctTbl,
                                  NULL,
                                  0);
        }
    
        if (i4Ret != RMR_OK)
        {
            LOG(1, "MW_TVD_Init(): x_rm_reg_comp failed [%d]\n", (unsigned int)i4Ret);
            return i4Ret;
        }
    }
    
    /* init cc */
    
    /* VDEC_MW_TVD_CC_NRL_THRESHOLD_BLOCK */
    if(DRVCUST_OptQuery(eTVDCCNRLThresholdBlock, &u4Val) < 0)
    {
        _u1CCNRLBlock = 80;
    }
    else
    {
        _u1CCNRLBlock = (UINT8) u4Val;
    }
    
    /* VDEC_MW_TVD_CC_NRL_THRESHOLD_UNBLOCK */
    if(DRVCUST_OptQuery(eTVDCCNRLThresholdUnblock, &u4Val) < 0)
    {
        _u1CCNRLUnblock = 75;
    }
    else
    {
        _u1CCNRLUnblock = (UINT8) u4Val;
    }
    
    /* VDEC_MW_TVD_CC_PER_THRESHOLD_BLOCK */
    if(DRVCUST_OptQuery(eTVDCCPERThresholdBlock, &u4Val) < 0)
    {
        _u1CCPERBlock   = 100;
    }
    else
    {
        _u1CCPERBlock = (UINT8) u4Val;
    }
    
    /* VDEC_MW_TVD_CC_PER_THRESHOLD_UNBLOCK */
    if(DRVCUST_OptQuery(eTVDCCPERThresholdUnblock, &u4Val) < 0)
    {
        _u1CCPERUnblock = 90;
    }
    else
    {
        _u1CCPERUnblock = (UINT8) u4Val;
    }
        
    for (u4Idx = 0; u4Idx < TVD_COMP_NS; u4Idx ++)
    {
        _afgCCPlaying[u4Idx] = FALSE;
    }
    
    /* init teletext buffer, WOULD NOT allocate buffer in it */
    for (u4Idx = 0; u4Idx < TVD_COMP_NS; u4Idx ++)
    {
        _TvdTtxBufSysInit (&_arTtxBufCtx[u4Idx]);

        #ifdef LINUX_TURNKEY_SOLUTION
        ui4_ttx_kernel_virt_start[u4Idx] = 0;
        #endif
    }
    
    /* dump the address of TVD message control */
    Printf("\nTVD MsgCtrl:  NRL:%d/%d, PER:%d/%d\n", 
            VDEC_MW_TVD_CC_NRL_THRESHOLD_BLOCK,
            VDEC_MW_TVD_CC_NRL_THRESHOLD_UNBLOCK,
            VDEC_MW_TVD_CC_PER_THRESHOLD_BLOCK,
            VDEC_MW_TVD_CC_PER_THRESHOLD_UNBLOCK );
    
    return (RMR_OK);
    
#else
    return 0;
#endif
}

static INT32 _TvdMwConnect(DRV_COMP_ID_T*   pt_comp_id,
              DRV_CONN_TYPE_T  e_conn_type,
              const VOID*      pv_conn_info,
              SIZE_T           z_conn_info_len,
              VOID*            pv_tag,
              x_rm_nfy_fct     pf_nfy)
{

#ifndef CC_DRIVER_PROGRAM

    CRIT_STATE_T    t_cs;
    DRV_COMP_ID_T*  prVSrcCompReg;
    DRV_ID_T        rVCombiDftSrc;
    UINT8           u1Ret;
    RVChannel*      pt_rv_channel = NULL;
    _NotifyStatus*  pt_nfy_sts = NULL;
    UINT8           ui1_path = 0;
    DRV_TYPE_T      e_pre_comp_type;
    UINT16          ui2_pre_comp_id;
#ifdef CC_SUPPORT_TVE
    UINT8           u1SrcType;
#endif

    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }
    
    if (pt_comp_id->e_type != DRVT_TV_DEC)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->ui2_id >= TVD_COMP_NS)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

  
    // ????
    if (pt_comp_id->b_sel_out_port != FALSE) // must select to input port
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

    prVSrcCompReg = (DRV_COMP_ID_T*)pv_conn_info;

    TVD_DBG_MSG( "\r\n*** Connect TVD%d to %u tick=%u***\r\n", 
                pt_comp_id->ui2_id, 
                prVSrcCompReg->e_type, 
                x_os_get_sys_tick() );
    
    if ( pt_comp_id->ui2_id == 0 ) // main
    {
        pt_rv_channel = &_rMChannel;
        pt_nfy_sts = &_tMainNotifyStatus;
        ui1_path = SV_VP_MAIN;
    }
    else if ( pt_comp_id->ui2_id == 1 ) 
    {
        pt_rv_channel = &_rPChannel;
        pt_nfy_sts = &_tPipNotifyStatus;
        ui1_path = SV_VP_PIP;
    }
#ifdef CC_SUPPORT_TVE
    else if ( pt_comp_id->ui2_id < TVD_COMP_NS ) /* tv/mon-scart */
    {
        /* Set TVd to conencted */
        t_cs = x_crit_start();
        
        _afgTvdIsConnected[pt_comp_id->ui2_id] = TRUE;
        
        x_crit_end(t_cs);

        u1SrcType = SV_VS_MAX;
        if( prVSrcCompReg->e_type == DRVT_AVC_COMBI )
        {
            // Get default Combi component
            if( avc_combi_get_default_comp(
                    prVSrcCompReg->ui2_id, 
                    &rVCombiDftSrc) == 0 )
            {
                u1SrcType = rGetVSrcMap(rVCombiDftSrc.e_type, rVCombiDftSrc.ui2_id);
            }
        }
        else if( prVSrcCompReg->e_type == DRVT_AVC_SCART )    
        {
            AVC_SCART_INFO_T t_scart_info;
    
            rGetVScartInfo(&t_scart_info);
            if (t_scart_info.b_canal_plus_support)
            {
                u1SrcType = rGetVSrcMap(t_scart_info.e_tuner_type, t_scart_info.ui2_tuner_id);
            }                    
            else
            {
                u1SrcType = rGetVSrcMap(prVSrcCompReg->e_type, prVSrcCompReg->ui2_id);
            }
        }
        else
        {
            u1SrcType = rGetVSrcMap(prVSrcCompReg->e_type, prVSrcCompReg->ui2_id);
        }

        if(pt_comp_id->ui2_id == 3) /* tv-scart */
        {
            _TveSetScartOutSource(0, u1SrcType);
        }
        else if(pt_comp_id->ui2_id == 2) /* mon-scart */
        {
            _TveSetScartOutSource(1, u1SrcType);
        }
        return RMR_OK;
    }
#endif
    else
    {
        return RMR_DRV_INV_SET_INFO;
    }
    
    /* Get previous component type/ID */
    t_cs = x_crit_start();
    e_pre_comp_type = pt_rv_channel->u2_e_type;
    ui2_pre_comp_id = pt_rv_channel->u2_e_ui2_id;
    x_crit_end(t_cs);
    
    /* Reset Notify Status is only allowed for none-tuner input source.
       For tuner, this will be done in _TvdMwDisconnect(). */
    if ( !M_IS_TUNER_SRC( prVSrcCompReg->e_type ) )
    {
        BOOL        b_reset_nfy_sts = TRUE;
        
        /* If this is connect->connect process, and the old source the same 
           with new source, DO NOT reset signal status */
        if ( _afgTvdIsConnected[pt_comp_id->ui2_id] &&
             e_pre_comp_type == prVSrcCompReg->e_type &&
             ui2_pre_comp_id == prVSrcCompReg->ui2_id )
        {
            pt_nfy_sts->u1NeedNotify = TRUE;
            b_reset_nfy_sts = FALSE;
        }
        
        if ( b_reset_nfy_sts )
        {
            _TvdResetNfyStatus( ui1_path );
        }
    }

#if SUPPORT_S_DETECT
    // RESET NOTIFY FUNCTION POINTER HERE
    t_cs = x_crit_start();
    
    _arTVNfyInfo[ui1_path].pf_tv_dec_nfy = NULL;
    _arTVNfyInfo[ui1_path].pv_tag = NULL;
    pt_rv_channel->t_Tvd_Ctrl = TV_DEC_CTRL_STOP;

    x_crit_end(t_cs);

    if( prVSrcCompReg->e_type == DRVT_AVC_COMBI )
    {
        u1Ret = SV_FAIL;
        
        // Get default Combi component
        if( avc_combi_get_default_comp(
                prVSrcCompReg->ui2_id, 
                &rVCombiDftSrc) == 0 )
        {
            u1Ret = bApiVideoSetVideoSrc(
                ui1_path, 
                rGetVSrcMap(rVCombiDftSrc.e_type, rVCombiDftSrc.ui2_id) );
        }
    }
    else if( prVSrcCompReg->e_type == DRVT_AVC_SCART )    
    {
        AVC_SCART_INFO_T t_scart_info;
    
        rGetVScartInfo(&t_scart_info);
        if (t_scart_info.b_canal_plus_support)
        {
            switch (t_scart_info.e_scart_bypass)
            {
                case AVC_SCART_BYPASS_TRUE:
                    u1Ret = bApiVideoSetVideoSrc(
                        ui1_path, 
                        rGetVSrcMap(t_scart_info.e_tuner_type, t_scart_info.ui2_tuner_id));                    
                    break;
                    
                case AVC_SCART_BYPASS_FALSE:   
                    u1Ret = bApiVideoSetVideoSrc(
                        ui1_path, 
                        rGetVSrcMap(t_scart_info.e_scart_type, t_scart_info.ui2_scart_id));                                              
                    break;
                    
                default:
                    u1Ret = SV_FAIL;
            }                    
        }
        else
        {
            u1Ret = bApiVideoSetVideoSrc(
                ui1_path, 
                rGetVSrcMap(prVSrcCompReg->e_type, prVSrcCompReg->ui2_id));            
        }                                    
    }        
    else
    {
        u1Ret = bApiVideoSetVideoSrc(
            ui1_path, 
            rGetVSrcMap(prVSrcCompReg->e_type, prVSrcCompReg->ui2_id));
    }

    // Assign e_type and e_ui2_id
    if((u1Ret == SV_SUCCESS)||(u1Ret == SV_NO_CHANGE))
    {
        t_cs = x_crit_start();
        
        if( prVSrcCompReg->e_type == DRVT_AVC_COMBI )
        {
            pt_rv_channel->u2_e_type = rVCombiDftSrc.e_type;
            pt_rv_channel->u2_e_ui2_id = rVCombiDftSrc.ui2_id;
        }
        else
        {
            pt_rv_channel->u2_e_type = prVSrcCompReg->e_type;
            pt_rv_channel->u2_e_ui2_id = prVSrcCompReg->ui2_id;
        }
        
        // Set Capibility
        vSetCapibility(ui1_path);
        
        pt_rv_channel->t_comp_id.pv_tag = pt_comp_id->pv_tag;
        pt_rv_channel->t_comp_id.e_type = pt_comp_id->e_type;
        pt_rv_channel->t_comp_id.ui2_id = pt_comp_id->ui2_id;
        pt_rv_channel->t_comp_id.b_sel_out_port = pt_comp_id->b_sel_out_port;
        
        x_crit_end(t_cs);
    }
    else
    {
        // RESET NOTIFY FUNCTION POINTER HERE
        t_cs = x_crit_start();
        
        _arTVNfyInfo[ui1_path].pf_tv_dec_nfy = NULL;
        _arTVNfyInfo[ui1_path].pv_tag = NULL;
        pt_rv_channel->t_Tvd_Ctrl = TV_DEC_CTRL_STOP;
        
        pt_rv_channel->u4_Cap = 0;
        pt_rv_channel->u2_e_type = DRVT_UNKNOWN;
        pt_rv_channel->u2_e_ui2_id = 0;
        
        x_crit_end(t_cs);
        
        //Printf("Main Select Error, %d, %d\n\n", pt_rv_channel->bDecType, _rPChannel.bDecType);
        return(RMR_DRV_INV_CONN_INFO);
    }

#else  /* NOT SUPPORT_S_DETECT */
    u1Ret = bApiVideoSetVideoSrc(ui1_path, rGetVSrcMap(prVSrcCompReg->e_type, prVSrcCompReg->ui2_id));
#endif /* SUPPORT_S_DETECT */

    // check the valid or not of source component.
    // Call 8202 api to verify whether this works or not.
    
    /* Set TVd to conencted */
    t_cs = x_crit_start();
    
    _afgTvdIsConnected[pt_comp_id->ui2_id] = TRUE;
    
    x_crit_end(t_cs);
    
    /* Reset CC */
    _afgCCPlaying[ui1_path] = FALSE;
    
    if ( prVSrcCompReg->e_type == DRVT_AVC_COMBI )
    {
        avc_combi_tv_dec_connect( pt_comp_id->ui2_id, prVSrcCompReg->ui2_id );
    }
    
    return (RMR_OK);
#else
    return 0;
#endif

}


static INT32 _TvdMwDisconnect(
    DRV_COMP_ID_T*   pt_comp_id,
    DRV_DISC_TYPE_T  e_disc_type,
    const VOID*      pv_disc_info,
    SIZE_T           z_disc_info_len )
{
    DRV_COMP_ID_T*  prVSrcCompReg;
    CRIT_STATE_T    t_cs;
    RVChannel*      pt_rv_channel = NULL;
    UINT8           ui1_path = 0;

#ifndef CC_DRIVER_PROGRAM

    INT32 i4RetCode = RMR_OK;

    prVSrcCompReg = (DRV_COMP_ID_T*) pv_disc_info;
    if ( !prVSrcCompReg )
    {
        return RMR_DRV_INV_DISC_INFO;
    }
    
    // Check component type and id
    if ( !pt_comp_id )
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if ( pt_comp_id->e_type != DRVT_TV_DEC ||
         pt_comp_id->ui2_id >= TVD_COMP_NS )
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if ( pt_comp_id->ui2_id == 0 ) /* MAIN */
    {
        pt_rv_channel = &_rMChannel;
        ui1_path = SV_VP_MAIN;
    }
    else if ( pt_comp_id->ui2_id == 1 )  /* SUB */
    {
        pt_rv_channel = &_rPChannel;
        ui1_path = SV_VP_PIP;
    }
    else if ( pt_comp_id->ui2_id < TVD_COMP_NS)
    {
        /* Set TVd to disconencted */
        t_cs = x_crit_start();
        _afgTvdIsConnected[pt_comp_id->ui2_id] = FALSE;
        x_crit_end(t_cs);
        return RMR_OK;
    }
    else
    {
        return RMR_DRV_INV_SET_INFO;
    }

    TVD_DBG_MSG( "*** Disconnect TVD%d tick=%u***\r\n", 
                  pt_comp_id->ui2_id, x_os_get_sys_tick() );
  
    if (pt_comp_id->b_sel_out_port != FALSE)
    {
        i4RetCode = RMR_DRV_INV_DISC_INFO;
    }

    // RESET NOTIFY FUNCTION POINTER HERE
    t_cs = x_crit_start();
    _arTVNfyInfo[ui1_path].pf_tv_dec_nfy = NULL;
    _arTVNfyInfo[ui1_path].pv_tag = NULL;
    pt_rv_channel->t_Tvd_Ctrl = TV_DEC_CTRL_STOP;
    x_crit_end(t_cs);

    bApiVideoSetVideoSrc(ui1_path, SV_VS_MAX);

    
    t_cs = x_crit_start();
    
    pt_rv_channel->u2_e_type = DRVT_UNKNOWN;
    pt_rv_channel->u2_e_ui2_id = 0;
    _TvdResetNfyStatus( ui1_path );

    /* Set TVd to disconencted */
    _afgTvdIsConnected[pt_comp_id->ui2_id] = FALSE;
    
    x_crit_end(t_cs);

    if ( prVSrcCompReg->e_type == DRVT_AVC_COMBI )
    {
        avc_combi_tv_dec_disconnect( prVSrcCompReg->ui2_id );
    }
        
    return (i4RetCode);
#else
    return 0;
#endif
}


static INT32 _TvdMwGet(DRV_COMP_ID_T*  pt_comp_id,
          DRV_GET_TYPE_T  e_get_type,
          VOID*           pv_get_info,
          SIZE_T*         pz_get_info_len)
{
#ifndef CC_DRIVER_PROGRAM

    UINT8 u1ColSys;
    UINT8 u1Path = SV_VP_MAIN;
    UINT8 bDecType;
    INT32 i4_return;
    TV_DEC_MIN_MAX_INFO_T*  pt_tv_dec_min_max_info;
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT        
    TV_DEC_AMBIG_VGA_TIMING_INFO_T* pt_tv_dec_ambig_vga_timing_info;
#endif    
    CRIT_STATE_T            t_cs;
    _NotifyStatus*          pt_nfy_sts = NULL;
    
    if(pt_comp_id->ui2_id == 0 )// main
    {
        u1Path = SV_VP_MAIN;
        pt_nfy_sts = &_tMainNotifyStatus;
    }
    else if(pt_comp_id->ui2_id == 1)// sub
    {
        u1Path = SV_VP_PIP;
        pt_nfy_sts = &_tPipNotifyStatus;
    }
    else if(pt_comp_id->ui2_id < TVD_COMP_NS)
    {
        if ( e_get_type == TV_DEC_GET_TYPE_RESOLUTION ||
             e_get_type == TV_DEC_GET_TYPE_TV_SIGNAL ||
             e_get_type == TV_DEC_GET_TYPE_COLOR_SYS ||
             e_get_type == TV_DEC_GET_TYPE_CAPABILITY )
        {
            pt_nfy_sts = &_tMainNotifyStatus;
        }
        else
        {
            *pz_get_info_len = 0;
            return RMR_OK;
        }
    }
    else
    {
        return RMR_DRV_INV_GET_INFO;
    }

    switch (e_get_type)
    {
    case TV_DEC_GET_TYPE_CTRL:
        if (*pz_get_info_len >= sizeof(TV_DEC_CTRL_T))
        {
            if(u1Path == SV_VP_MAIN)// main
            {
                *(TV_DEC_CTRL_T*)pv_get_info = _rMChannel.t_Tvd_Ctrl;
                *pz_get_info_len = sizeof(TV_DEC_CTRL_T);
                i4_return = RMR_OK;

            }
            else if(u1Path == SV_VP_PIP)// sub
            {
                *(TV_DEC_CTRL_T*)pv_get_info = _rPChannel.t_Tvd_Ctrl;
                *pz_get_info_len = sizeof(TV_DEC_CTRL_T);
                i4_return = RMR_OK;

            }
            else
            {
                i4_return = RMR_DRV_INV_GET_INFO;
            }
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }

        break;
    case TV_DEC_GET_TYPE_RESOLUTION:
        if (*pz_get_info_len >= sizeof(TV_DEC_RESOLUTION_INFO_T))
        {
#ifdef CC_SUPPORT_TVE
            if( pt_comp_id->ui2_id > 1 && /* SCART out id */ 
                TVE_BYP_EXT_PATH == bApiTVEGetBypPath((pt_comp_id->ui2_id==3)?        /* hw byp */
                                         ((DAC_1==TV_ENC_SCART_1)?(DAC_1):(DAC_2)):
                                         ((DAC_2==TV_ENC_SCART_2)?(DAC_2):(DAC_1))) )



            {
                /* Hard-coded for PAL (576i) timing */
                ((TV_DEC_RESOLUTION_INFO_T*)pv_get_info)->ui2_frame_rate = 50;
                ((TV_DEC_RESOLUTION_INFO_T*)pv_get_info)->ui2_width = 720;
                ((TV_DEC_RESOLUTION_INFO_T*)pv_get_info)->ui2_height = 576;
                ((TV_DEC_RESOLUTION_INFO_T*)pv_get_info)->b_is_progressive = SV_FALSE;
                ((TV_DEC_RESOLUTION_INFO_T*)pv_get_info)->e_src_asp_ratio = TV_DEC_SRC_ASPECT_RATIO_UNKNOWN;
                ((TV_DEC_RESOLUTION_INFO_T*)pv_get_info)->e_timing_type = TV_DEC_TIMING_VIDEO;
                ((TV_DEC_RESOLUTION_INFO_T*)pv_get_info)->ui4_res_idx = 0; 
                ((TV_DEC_RESOLUTION_INFO_T*)pv_get_info)->ui4_ext = 0;
                ((TV_DEC_RESOLUTION_INFO_T*)pv_get_info)->e_color_fmt = TV_DEC_COLOR_FMT_YUV_4_2_2;
            }
            else
#endif
            {
                t_cs = x_crit_start();
    
                *((TV_DEC_RESOLUTION_INFO_T*)pv_get_info) = pt_nfy_sts->t_resolution;

                x_crit_end( t_cs );
            }

            *pz_get_info_len = sizeof(TV_DEC_RESOLUTION_INFO_T);
            i4_return = RMR_OK;
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
        break;

    case TV_DEC_GET_TYPE_TV_SIGNAL:

        if(*pz_get_info_len < sizeof(BOOL))
        {
            i4_return = RMR_DRV_INV_GET_INFO;
        }
        else
        {
            UINT8 u1SigState;

            *pz_get_info_len = sizeof(BOOL);
            i4_return = RMR_OK;

            if ( pt_comp_id->ui2_id == 0 )
            {
                u1SigState = _tMainNotifyStatus.u1SigStatus;
            }
            else if ( pt_comp_id->ui2_id == 1 )
            {
                u1SigState = _tPipNotifyStatus.u1SigStatus;
            }
#ifdef CC_SUPPORT_TVE
            else if ( pt_comp_id->ui2_id <= 3 )
            {
                switch(bApiTVEGetBypPath((pt_comp_id->ui2_id==3)?
                                         ((DAC_1==TV_ENC_SCART_1)?(DAC_1):(DAC_2)):
                                         ((DAC_2==TV_ENC_SCART_2)?(DAC_2):(DAC_1))) )
                {
                    default:
                    case TVE_BYP_EXT_PATH:
                        u1SigState = SV_VDO_STABLE;
                        break;
                    case TVE_BYP_AVMUX_PATH:
                        u1SigState = _tMainNotifyStatus.u1SigStatus;
                        break;
                    case TVE_BYP_PIP_PATH:
                        u1SigState = _tPipNotifyStatus.u1SigStatus;
                        break;
                }
            }
#endif
            else
            {
                return RMR_DRV_INV_GET_INFO;
            }
            
            *(TV_DEC_TV_SIG_T*)pv_get_info = _drv_tv_sig_sts_2_mw(u1SigState);
        }
        break;

    /* New TV_DEC_GET_TYPE_COLOR_SYS_CAP interface 2006/06/15 */
    case TV_DEC_GET_TYPE_COLOR_SYS_CAP:
        if (*pz_get_info_len >= sizeof(UINT32))
        {
            UINT32 u4ColCap;

            if( ((pt_comp_id->ui2_id == 0) && (fgIsMainTvd3d() == TRUE))  || 
                ((pt_comp_id->ui2_id == 1) && (fgIsPipTvd3d() == TRUE)))
            {
                *pz_get_info_len = sizeof(UINT32);

                u4ColCap = TVD_COLOR_SYS_CAP;
                *(UINT32*)pv_get_info = u4ColCap;
                *pz_get_info_len = sizeof(UINT32);
                i4_return = RMR_OK;
            }
            else
            {
                u4ColCap = TV_DEC_COLOR_SYS_CAP_NONE;
                i4_return = RMR_DRV_INV_GET_INFO;
            }
        }
        else
        {
            i4_return = RMR_DRV_INV_GET_INFO;
        }
        break;

    case TV_DEC_GET_TYPE_COLOR_SYS:

        if (*pz_get_info_len >= sizeof(TV_DEC_COLOR_SYS_T))
        {
            if(((pt_comp_id->ui2_id == 0 || pt_comp_id->ui2_id == 2 || pt_comp_id->ui2_id == 3) && (fgIsMainTvd3d() == TRUE))  || ((pt_comp_id->ui2_id == 1) && (fgIsPipTvd3d() == TRUE)))
            {
                u1ColSys =bApiVideoGetColorSystem();
                *pz_get_info_len = sizeof(TV_DEC_COLOR_SYS_T);

                switch(u1ColSys)
                {
                case SV_CS_PAL_N:
                    *(TV_DEC_COLOR_SYS_T*)pv_get_info = TV_DEC_COLOR_SYS_PAL_N;
                break;
                case SV_CS_PAL:
                    *(TV_DEC_COLOR_SYS_T*)pv_get_info = TV_DEC_COLOR_SYS_PAL;
                break;
                case SV_CS_PAL_M:
                    *(TV_DEC_COLOR_SYS_T*)pv_get_info = TV_DEC_COLOR_SYS_PAL_M;
                break;
                case SV_CS_NTSC358:
                    *(TV_DEC_COLOR_SYS_T*)pv_get_info = TV_DEC_COLOR_SYS_NTSC;
                break;
                case SV_CS_SECAM:
                    *(TV_DEC_COLOR_SYS_T*)pv_get_info = TV_DEC_COLOR_SYS_SECAM;
                break;
                case SV_CS_NTSC443:
                    *(TV_DEC_COLOR_SYS_T*)pv_get_info = TV_DEC_COLOR_SYS_NTSC_443;
                break;
                case SV_CS_PAL_60:
                    *(TV_DEC_COLOR_SYS_T*)pv_get_info = TV_DEC_COLOR_SYS_PAL_60;
                break;
                case SV_CS_UNSTABLE:
                    *(TV_DEC_COLOR_SYS_T*)pv_get_info = TV_DEC_COLOR_SYS_UNKNOWN;
                break;
                }

                i4_return = RMR_OK;
            }
            else
            {
                i4_return = RMR_DRV_INV_GET_INFO;
            }
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
        break;

    case TV_DEC_GET_TYPE_CLOCK:
        if (*pz_get_info_len >= sizeof(UINT8))
        {
            if(((pt_comp_id->ui2_id == 0) && fgIsMainVga()) || ((pt_comp_id->ui2_id == 1) && fgIsPipVga()))
            {
                *(INT16*)pv_get_info = wDrvVGAGetClock()-127;//i2GetTypeClock;
                *pz_get_info_len = sizeof(INT16);
                i4_return = RMR_OK;
            }
            else
            {
                i4_return = RMR_DRV_INV_GET_INFO;
            }
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }

        break;
    case TV_DEC_GET_TYPE_CLOCK_MAX:
        if (*pz_get_info_len >= sizeof(UINT8))
        {
            if(((pt_comp_id->ui2_id == 0) && fgIsMainVga()) || ((pt_comp_id->ui2_id == 1) && fgIsPipVga()))
            {
		    if(bDrvVideoIsSrcInterlace(pt_comp_id->ui2_id))
		    {
			    *(INT16*)pv_get_info=0;
		    }
		    else
		    {
			    //*(INT16*)pv_get_info = VGA_CLOCK_MAX;
			    *(INT16*)pv_get_info = wDrvVgaGetClockMax();
		    }  
                *pz_get_info_len = sizeof(UINT8);
                i4_return = RMR_OK;
            }
            else
            {
                i4_return = RMR_DRV_INV_GET_INFO;
            }
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
        break;

    case TV_DEC_GET_TYPE_CLOCK_MIN:
        if (*pz_get_info_len >= sizeof(UINT8))
        {
            if(((pt_comp_id->ui2_id == 0) && fgIsMainVga()) || ((pt_comp_id->ui2_id == 1) && fgIsPipVga()))
            {
            		if(bDrvVideoIsSrcInterlace(pt_comp_id->ui2_id))
            		{
            			*(INT16*)pv_get_info=0;
            		}
            		else
            		{
	            		*(INT16*)pv_get_info = VGA_CLOCK_MIN;
            		}          		
                *pz_get_info_len = sizeof(UINT8);
                i4_return = RMR_OK;
            }
            else
            {
                i4_return = RMR_DRV_INV_GET_INFO;
            }
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
        break;

    case TV_DEC_GET_TYPE_PHASE:
        if (*pz_get_info_len >= sizeof(UINT8))
        {
            if(((pt_comp_id->ui2_id == 0) && (fgIsMainVga() || fgIsMainYPbPr())) ||
                ((pt_comp_id->ui2_id == 1) && (fgIsPipVga() || fgIsPipYPbPr())))
            {
                *(UINT16*)pv_get_info = u1DrvVGAGetPhase();//u2GetTypePhase;//
                *pz_get_info_len = sizeof(UINT16);
                i4_return = RMR_OK;
            }
            else
            {
                i4_return = RMR_DRV_INV_GET_INFO;
            }
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
        break;

    case TV_DEC_GET_TYPE_H_POSITION:
	{
		 INT32 pos,min_off,max,min,def;
		 UINT8 ui1_path;
		 ui1_path=(UINT8)pt_comp_id->ui2_id;
		 bDecType = bGetVideoDecType(ui1_path); 

		 def=wDrvVideoGetPorch((UINT8)ui1_path, SV_HPORCH_DEFAULT);
		 max=wDrvVideoGetPorch((UINT8)ui1_path, SV_HPORCH_MAX);
		 min=wDrvVideoGetPorch((UINT8)ui1_path, SV_HPORCH_MIN);

		 pos=wDrvVideoGetPorch((UINT8)ui1_path,SV_HPORCH_CURRENT);


		 if (bDrvGetPorchTune((UINT8)ui1_path) == SV_PORCHTUNE_SCPOS)//for DTV
		 {
			*(UINT16*)pv_get_info=MAX(32+pos-def,0);
		 }
		 else
		 {
			 if (bDecType == SV_VD_VGA)
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
		 i4_return = RMR_OK;
	 }

        break;

    case TV_DEC_GET_TYPE_V_POSITION:

	    if (bDrvGetPorchTune((UINT8)pt_comp_id->ui2_id) == SV_PORCHTUNE_SCPOS)//for DTV
	     {
		    //force SCPOS HV
		    UINT16 def,pos;
		    def=wDrvVideoGetPorch((UINT8)pt_comp_id->ui2_id, SV_HPORCH_DEFAULT);
		    pos=wDrvVideoGetPorch((UINT8)pt_comp_id->ui2_id,SV_HPORCH_CURRENT);
		    bDrvPorchTune[(UINT8)pt_comp_id->ui2_id]=SV_PORCHTUNE_SCPOS;
		    *(UINT16*)pv_get_info =MAX(16+pos-def,0);
	     }
	     else
	     {
	     	*(UINT16*)pv_get_info =wDrvVideoGetPorch(pt_comp_id->ui2_id,SV_VPORCH_CURRENT);
	     }
	     *pz_get_info_len = sizeof(UINT16);
	     i4_return = RMR_OK;

        break;

    case TV_DEC_GET_TYPE_CAPABILITY:
        if (*pz_get_info_len >= sizeof (UINT32))
        {
            if(pt_comp_id->ui2_id == 0) // main
            {
                /* for main/sub TTX_HW flag is needed for middleware 0924 */
                *(UINT32*)pv_get_info = _rMChannel.u4_Cap | TV_DEC_CAP_TTX_HW;
            }
            else if (pt_comp_id->ui2_id == 1)
            {
                /* for main/sub TTX_HW flag is needed for middleware 0924 */
                *(UINT32*)pv_get_info = _rPChannel.u4_Cap | TV_DEC_CAP_TTX_HW;
            }
            else /* SCART-OUT, pseudo TVD */
            {
                *(UINT32*)pv_get_info = (_rMChannel.u4_Cap & (~TV_DEC_CAP_TTX_HW)) ;
            }

            *pz_get_info_len = sizeof(UINT32);
            i4_return = RMR_OK;
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
        break;

    case TV_DEC_GET_TYPE_MIN_MAX:
        if (*pz_get_info_len >= sizeof(TV_DEC_MIN_MAX_INFO_T))
        {
            pt_tv_dec_min_max_info = (TV_DEC_MIN_MAX_INFO_T*)pv_get_info;
            bDecType = bGetVideoDecType(u1Path); 
            if ((pt_tv_dec_min_max_info->e_get_type) == TV_DEC_GET_TYPE_PHASE)
            {
                        pt_tv_dec_min_max_info->ui4_min_value = (UINT32)VGA_PHASE_MIN;
                        pt_tv_dec_min_max_info->ui4_max_value = (UINT32)VGA_PHASE_MAX;
            }
		switch (pt_tv_dec_min_max_info->e_get_type)
		{
		    case TV_DEC_GET_TYPE_H_POSITION:
			if (bDecType == SV_VD_VGA)
			{
		                        pt_tv_dec_min_max_info->ui4_min_value = wDrvVideoGetPorch(u1Path,SV_HPORCH_MIN);
		                        pt_tv_dec_min_max_info->ui4_max_value = wDrvVideoGetPorch(u1Path,SV_HPORCH_MAX);
		             }
			else if (bDrvGetPorchTune(u1Path) == SV_PORCHTUNE_SCPOS)//for DTV
			{
				pt_tv_dec_min_max_info->ui4_min_value = 0;
				pt_tv_dec_min_max_info->ui4_max_value = 64;
			}
			else
			{
				UINT16 def=wDrvVideoGetPorch(u1Path,SV_HPORCH_DEFAULT);
	//			pt_tv_dec_min_max_info->ui4_min_value = wDrvVideoGetPorch(u1Path,SV_HPORCH_MIN);
	//			pt_tv_dec_min_max_info->ui4_max_value = wDrvVideoGetPorch(u1Path,SV_HPORCH_MAX);
				pt_tv_dec_min_max_info->ui4_min_value = (def>32)? def-32:0;
				pt_tv_dec_min_max_info->ui4_max_value = (def>32)? def+32:def*2;
			}
			break;
		    case TV_DEC_GET_TYPE_V_POSITION:
			if (bDecType == SV_VD_VGA)
			{
				pt_tv_dec_min_max_info->ui4_min_value = wDrvVideoGetPorch(u1Path,SV_VPORCH_MIN);
				pt_tv_dec_min_max_info->ui4_max_value = wDrvVideoGetPorch(u1Path,SV_VPORCH_MAX);
			}
			else  if (bDrvGetPorchTune(u1Path) == SV_PORCHTUNE_SCPOS)//for DTV
			{
				pt_tv_dec_min_max_info->ui4_min_value = 0;
				pt_tv_dec_min_max_info->ui4_max_value = 32;
			}
			else
			{
				UINT16 def=wDrvVideoGetPorch(u1Path,SV_VPORCH_DEFAULT);
	//			pt_tv_dec_min_max_info->ui4_min_value = wDrvVideoGetPorch(u1Path,SV_VPORCH_MIN);
	//			pt_tv_dec_min_max_info->ui4_max_value = wDrvVideoGetPorch(u1Path,SV_VPORCH_MAX);
				if (bDecType == SV_VD_DVI)
				{
					pt_tv_dec_min_max_info->ui4_min_value = (def>20)? def-20:0;
					pt_tv_dec_min_max_info->ui4_max_value = (def>20)? def+20:def*2;
				}
				else
				{
				pt_tv_dec_min_max_info->ui4_min_value = (def>16)? def-16:0;
				pt_tv_dec_min_max_info->ui4_max_value = (def>16)? def+16:def*2;
				}


			}
			break;
		    default:
			break;
		}
            *pz_get_info_len = sizeof(TV_DEC_MIN_MAX_INFO_T);
            i4_return = RMR_OK;
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
        break;

    case TV_DEC_GET_TYPE_SCART_INP_MODE:
        if (*pz_get_info_len >= sizeof (TV_DEC_SCART_MODE_T))
        {
            i4_return = RMR_DRV_NO_GET_INFO;
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }        
        break;      
                
    case TV_DEC_GET_TYPE_AMBIG_VGA_TIMING:
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT    
        pt_tv_dec_ambig_vga_timing_info = (TV_DEC_AMBIG_VGA_TIMING_INFO_T*)pv_get_info;
        if ((pt_tv_dec_ambig_vga_timing_info->e_type) == TV_DEC_AMBIG_VGA_TIMING_TYPE_NUM_IN_GRP)
        {
            pt_tv_dec_ambig_vga_timing_info->u.ui2_num = bVgaGetAmbiguousTimingNum();
            i4_return = RMR_OK;
        }
        else 
        {
            VGAAmbTimingInfo_t rAmbTiming;
            if((pt_tv_dec_ambig_vga_timing_info->e_type) == TV_DEC_AMBIG_VGA_TIMING_TYPE_OPTION_IN_GRP)
            {
                vVgaQueryAmbiguousTiming(pt_tv_dec_ambig_vga_timing_info->u.t_opt.ui2_opt_index, &rAmbTiming, 1);
            }
            else if((pt_tv_dec_ambig_vga_timing_info->e_type) == TV_DEC_AMBIG_VGA_TIMING_TYPE_CURR_OPT_IN_GRP)
        {
                vVgaQueryAmbiguousTiming(QUERY_CUR_AMBIGUOUS_TIMING, &rAmbTiming, 1);
            }

            pt_tv_dec_ambig_vga_timing_info->u.t_opt.ui2_opt_index = rAmbTiming.bCandiIdx;
            pt_tv_dec_ambig_vga_timing_info->u.t_opt.ui2_ver_frequncy = rAmbTiming.bVer_freq;
            pt_tv_dec_ambig_vga_timing_info->u.t_opt.ui4_hor_resolution = rAmbTiming.wHor_res;
            pt_tv_dec_ambig_vga_timing_info->u.t_opt.ui4_ver_resolution = rAmbTiming.wVer_res;
            
            if(rAmbTiming.bVer_freq != 0)
            {
                i4_return = RMR_OK;
            }
            else
            {
                i4_return = RMR_DRV_GET_FAILED;
            }
        }
#else
                i4_return = RMR_DRV_GET_FAILED;
#endif
       
        break;
        
    case TV_DEC_GET_TYPE_VGA_STATUS:
        i4_return = RMR_OK;
        switch(bDrvVideoSignalStatus(SV_VP_PIP))
        {
            case SV_VDO_STABLE:
                *((TV_DEC_VGA_STATUS_T*)pv_get_info) = TV_DEC_VGA_SIGNAL_STABLE;
                break;
            
            case SV_VDO_NOSIGNAL:
            case SV_VDO_UNKNOWN:
                *((TV_DEC_VGA_STATUS_T*)pv_get_info) = TV_DEC_VGA_SIGNAL_NO_SIGNAL;
                break;
            
            case SV_VDO_NOSUPPORT:
                *((TV_DEC_VGA_STATUS_T*)pv_get_info) = TV_DEC_VGA_SIGNAL_NOT_SUPPORT;
                break;
            default:
                i4_return = RMR_DRV_INV_GET_INFO;
                break;
        }
        break;
    case TV_DEC_GET_TYPE_SIGNAL_STRENGTH:
        if(*pz_get_info_len < sizeof(UINT8))
        {
            i4_return = RMR_DRV_INV_GET_INFO;
        }
        else
        {
            *pz_get_info_len = sizeof(UINT8);
            i4_return = RMR_OK;
            *(UINT8*)pv_get_info = bTvdSignalStrengthLevel(2);
        }
        break; 
	case TV_DEC_GET_TYPE_VID_FREQ:
		if (*pz_get_info_len >= sizeof(TV_DEC_VID_FREQ_T))
        {

		  if ((fgIsMainVga() || fgIsPipVga()) && fgIsValidTiming(_bVgaTiming))
            {
                ((TV_DEC_VID_FREQ_T*)pv_get_info)->b_valid = TRUE;
                ((TV_DEC_VID_FREQ_T*)pv_get_info)->ui2_v_freq =(UINT16) bSP0IVSClock(wASHLenMeasure(), wASVtotalMeasure());//bSP0Vclk;
                ((TV_DEC_VID_FREQ_T*)pv_get_info)->ui2_h_freq = wSP0IHSClock(wASHLenMeasure());//wSP0Hclk;
            }
            else

            {
                ((TV_DEC_VID_FREQ_T*)pv_get_info)->b_valid = FALSE;
                ((TV_DEC_VID_FREQ_T*)pv_get_info)->ui2_v_freq = 1;
                ((TV_DEC_VID_FREQ_T*)pv_get_info)->ui2_h_freq = 1;
            }

            *pz_get_info_len = sizeof(TV_DEC_VID_FREQ_T);
            i4_return = RMR_OK;
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
        break;
#ifdef CC_SOURCE_AUTO_DETECT
    case TV_DEC_GET_TYPE_SRC_DETECT_STATUS:
        vDrvGetSrcDetectStatus(((_TV_DEC_SOURCE_STATUS_T*)pv_get_info));
        i4_return = RMR_OK;
    break;
#endif		
    default:
        i4_return = RMR_DRV_INV_GET_INFO;
    }
    
    return (i4_return);
    
#else  /* !CC_DRIVER_PROGRAM */
    return 0;
#endif /* !CC_DRIVER_PROGRAM */
}


static INT32 _TvdMwSet(
    DRV_COMP_ID_T*  pt_comp_id,
    DRV_SET_TYPE_T  e_set_type,
    const VOID*     pv_set_info,
    SIZE_T          z_set_info_len)
{
#ifndef CC_DRIVER_PROGRAM
    
    INT32          i4RetCode = RMR_DRV_INV_SET_INFO;
    UINT8          bColSys;
    INT16          i2Tmp;
    UINT8          bDecType;
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT        
    TV_DEC_VGA_TIMING_OPTION_T* pt_tv_dec_vga_timing_option;
#endif    
    BOOL           fgIsCap = TRUE;
    RVChannel*     pt_rv_channel = NULL;
    _NotifyStatus* pt_nfy_sts = NULL;
    UINT8          ui1_path = 0;
    RVChannel      rTvechannel;
    x_signal_usr_nfy_fct      pf_signal_usr_nfy_fct = NULL;//singnal booster add by Hua 0609
    
    if ( pt_comp_id->ui2_id == 0 ) // main
    {
        pt_rv_channel = &_rMChannel;
        pt_nfy_sts = &_tMainNotifyStatus;
        ui1_path = SV_VP_MAIN;
    }
    else if ( pt_comp_id->ui2_id == 1 ) 
    {
        pt_rv_channel = &_rPChannel;
        pt_nfy_sts = &_tPipNotifyStatus;
        ui1_path = SV_VP_PIP;
    }
    else if ( pt_comp_id->ui2_id < TVD_COMP_NS)
    {
        TV_DEC_RESOLUTION_INFO_T  t_resolution;

        /* TVSCART & MonitorSCART, just ignore the set command */
        if ( e_set_type != TV_DEC_SET_TYPE_CTRL && 
	     e_set_type != TV_DEC_SET_TYPE_NFY_FCT )
        {
            return RMR_DRV_INV_SET_INFO;
        }

        if(e_set_type == TV_DEC_SET_TYPE_CTRL && 
           (TV_DEC_CTRL_T)(UINT32)pv_set_info == TV_DEC_CTRL_PLAY)
        {
            if(_arTVNfyInfo[pt_comp_id->ui2_id].pf_tv_dec_nfy != NULL)
            {
                // Call Back
            #ifndef LINUX_TURNKEY_SOLUTION
                _arTVNfyInfo[pt_comp_id->ui2_id].pf_tv_dec_nfy (
                    _arTVNfyInfo[pt_comp_id->ui2_id].pv_tag,
                    TV_DEC_COND_CTRL_DONE,
                    TV_DEC_SET_TYPE_CTRL,
                    (UINT32)pv_set_info );
           
                /* notification TVSig */
                _arTVNfyInfo[pt_comp_id->ui2_id].pf_tv_dec_nfy(
                   _arTVNfyInfo[pt_comp_id->ui2_id].pv_tag,
                   TV_DEC_COND_TV_SIGNAL,
                   _drv_tv_sig_sts_2_mw(SV_VDO_STABLE),
                   0);
            #else
                set_tv_dec_nfy(pt_comp_id->ui2_id,
                    _arTVNfyInfo[pt_comp_id->ui2_id].pv_tag,
                    TV_DEC_COND_CTRL_DONE,
                    TV_DEC_SET_TYPE_CTRL,
                    (UINT32)pv_set_info);
                /* notification TVSig */
                set_tv_dec_nfy(pt_comp_id->ui2_id,
                    _arTVNfyInfo[pt_comp_id->ui2_id].pv_tag,
                    TV_DEC_COND_TV_SIGNAL,
                    _drv_tv_sig_sts_2_mw(SV_VDO_STABLE),
                    0);
            #endif

                /* Hard-coded for PAL (576i) timing */
                t_resolution.ui2_frame_rate = 50;
                t_resolution.ui2_width = 720;
                t_resolution.ui2_height = 576;
                t_resolution.b_is_progressive = SV_FALSE;
                t_resolution.e_src_asp_ratio = TV_DEC_SRC_ASPECT_RATIO_UNKNOWN;
                t_resolution.e_timing_type = TV_DEC_TIMING_VIDEO;
                t_resolution.ui4_res_idx = 0; 
                t_resolution.ui4_ext = 0;
                t_resolution.e_color_fmt = TV_DEC_COLOR_FMT_YUV_4_2_2;

                /* notification ResolutionChg */
            #ifndef LINUX_TURNKEY_SOLUTION
                _arTVNfyInfo[pt_comp_id->ui2_id].pf_tv_dec_nfy (
                    _arTVNfyInfo[pt_comp_id->ui2_id].pv_tag,
                    TV_DEC_COND_RESOLUTION_CHG,
                    (UINT32)&t_resolution,
                    0);
            #else
                set_tv_dec_nfy(pt_comp_id->ui2_id,
                    _arTVNfyInfo[pt_comp_id->ui2_id].pv_tag,
                    TV_DEC_COND_RESOLUTION_CHG,
                    (UINT32)&t_resolution,
                    0);
            #endif
            }
        
            return RMR_OK;
        }
        pt_nfy_sts = &_tTveNotifyStatus;
        pt_nfy_sts->u1NeedNotify = FALSE;
        pt_rv_channel = &rTvechannel;
    }
    else
    {
        return RMR_DRV_INV_GET_INFO;
    }

    switch (e_set_type)
    {
    case TV_DEC_SET_TYPE_SIG_BOOSTER://singnal booster add by Hua 0609       
      	if (DRVCUST_InitQuery(eSetSignalUsrNotifyFunc, (UINT32 *)(void *)&(pf_signal_usr_nfy_fct))==0)
      	{
      		BOOL b_is_on;

			b_is_on = (BOOL)((UINT32)pv_set_info);
			if (pf_signal_usr_nfy_fct != NULL)
			{
				pf_signal_usr_nfy_fct(b_is_on);
			}//pf_irrc_usr_nfy_fct
            i4RetCode = RMR_OK;
      	}
	  	break;
	  	
    case TV_DEC_SET_TYPE_SWITCH_SIG_BOOSTER:
            i4RetCode = RMR_OK;
        break;
	   
    case TV_DEC_SET_TYPE_CTRL:
        if ( (z_set_info_len == sizeof(TV_DEC_CTRL_T)) &&
             (((TV_DEC_CTRL_T)(UINT32)pv_set_info == TV_DEC_CTRL_RESET) ||
             ((TV_DEC_CTRL_T)(UINT32)pv_set_info == TV_DEC_CTRL_STOP) ||
             ((TV_DEC_CTRL_T)(UINT32)pv_set_info == TV_DEC_CTRL_PLAY)) )
        {
            pt_rv_channel->t_Tvd_Ctrl = (TV_DEC_CTRL_T)(UINT32)pv_set_info;
            
            if( pt_rv_channel->t_Tvd_Ctrl == TV_DEC_CTRL_STOP )
            {
		/* do not touch nfy when scart bypass path */
                if (!(pt_comp_id->ui2_id > 1)) 
	        	{
                       _TvdEuWssStopNotify(ui1_path);
                       _TvdUsWssStopNotify(ui1_path);
                       _TvdResetNfyStatus( ui1_path );
				}
            }
            
            if( _arTVNfyInfo[pt_comp_id->ui2_id].pf_tv_dec_nfy )
            {
                // Call Back
            #ifndef LINUX_TURNKEY_SOLUTION
                _arTVNfyInfo[pt_comp_id->ui2_id].pf_tv_dec_nfy (
                    _arTVNfyInfo[pt_comp_id->ui2_id].pv_tag,
                    TV_DEC_COND_CTRL_DONE,
                    TV_DEC_SET_TYPE_CTRL,
                    (UINT32)pt_rv_channel->t_Tvd_Ctrl );
            #else
                set_tv_dec_nfy(pt_comp_id->ui2_id,
                    _arTVNfyInfo[pt_comp_id->ui2_id].pv_tag,
                    TV_DEC_COND_CTRL_DONE,
                    TV_DEC_SET_TYPE_CTRL,
                    (UINT32)pt_rv_channel->t_Tvd_Ctrl );
            #endif
            }
            
            // Notify happened before play arrives
            if( ((TV_DEC_CTRL_T)(UINT32)pv_set_info == TV_DEC_CTRL_PLAY) )
            {
                if (pt_nfy_sts->u1NeedNotify == TRUE)
                {
                    _Tvd_NotifyTVSig( ui1_path, pt_nfy_sts->u1SigStatus );
                    
                    if(pt_nfy_sts->u1SigStatus == SV_VDO_STABLE)
                    {
                        pt_nfy_sts->u1NeedNotify = TRUE;
                        _TvdNotifyColorSystem( ui1_path, pt_nfy_sts->u1SigStatus );
                        
                        pt_nfy_sts->u1NeedNotify = TRUE;
                        _TvdNotifyResolutionChg( ui1_path, pt_nfy_sts->u1SigStatus );
                    }
                }
                
                    _TvdEuWssStartNotify(ui1_path);
                    _TvdUsWssStartNotify(ui1_path);
                }

            i4RetCode = RMR_ASYNC_NFY;        
        }
        else
        {
            i4RetCode = RMR_DRV_INV_SET_INFO;
        }
        break;

    case TV_DEC_SET_TYPE_NFY_FCT:
        if ( (z_set_info_len == sizeof(TV_DEC_NFY_INFO_T)) &&
             (((TV_DEC_NFY_INFO_T*)pv_set_info)->pf_tv_dec_nfy != NULL))
        {
            _arTVNfyInfo[pt_comp_id->ui2_id].pv_tag = ((TV_DEC_NFY_INFO_T*)pv_set_info)->pv_tag;
            _arTVNfyInfo[pt_comp_id->ui2_id].pf_tv_dec_nfy = ((TV_DEC_NFY_INFO_T*)pv_set_info)->pf_tv_dec_nfy;
            i4RetCode = RMR_OK;
            
            // Notify happened before play arrives
            if( pt_nfy_sts->u1NeedNotify == TRUE )
            {
                _Tvd_NotifyTVSig(ui1_path, pt_nfy_sts->u1SigStatus);
            }
        }
        else
        {
            i4RetCode = RMR_DRV_INV_SET_INFO;
        }
        break;
#ifdef CC_SOURCE_AUTO_DETECT
		case TV_DEC_SET_TYPE_SRC_DETECT_NFY_FCT:
			if((z_set_info_len == sizeof(TV_DEC_SRC_DETECT_NFY_INFO_T)) &&
				(((TV_DEC_SRC_DETECT_NFY_INFO_T *)pv_set_info)->pf_src_detect_nfy != NULL))
			{
				_arSRCDetectNfyInfo[pt_comp_id->ui2_id].pv_tag = ((TV_DEC_SRC_DETECT_NFY_INFO_T *)pv_set_info)->pv_tag;
				_arSRCDetectNfyInfo[pt_comp_id->ui2_id].pf_src_detect_nfy = ((TV_DEC_SRC_DETECT_NFY_INFO_T *)pv_set_info)->pf_src_detect_nfy;
				i4RetCode = RMR_OK;
			}
			else
			{
				i4RetCode = RMR_DRV_INV_SET_INFO;
			}
			break;
	
#endif


    case TV_DEC_SET_TYPE_PLAY_CC:
        if ( (z_set_info_len == sizeof(TV_DEC_CC_NFY_INFO_T)) &&
             (((TV_DEC_CC_NFY_INFO_T*)pv_set_info)->pf_cc_nfy != NULL) )
        {
            TVD_DBG_MSG( "\r\n[TVD%d] PLAY CC\r\n",  pt_comp_id->ui2_id );
            
            _arCCNfyInfo[pt_comp_id->ui2_id].pv_tag    = ((TV_DEC_CC_NFY_INFO_T*)pv_set_info)->pv_tag;
            _arCCNfyInfo[pt_comp_id->ui2_id].pf_cc_nfy = ((TV_DEC_CC_NFY_INFO_T*)pv_set_info)->pf_cc_nfy;

            /* reset cc status */
            if(!_afgCCPlaying[ui1_path])
            {
                _arCCStatus[ui1_path].fgBlockedNRL = TRUE;
                _arCCStatus[ui1_path].u2CntStNRL   = 0;
                _arCCStatus[ui1_path].fgBlockedPER = TRUE;
                _arCCStatus[ui1_path].u2CntStPER   = 0;
                _arCCStatus[ui1_path].u2ErrRate    = 0;
            #ifdef TVD_CC_ECC_SWITCH 
                _arCCStatus[ui1_path].fgECCOnOff   = TRUE;
                _arCCStatus[ui1_path].u2CntECCNRL  = 0;
            #endif 
                x_memset(_arCCStatus[ui1_path].au4Samples, 0, sizeof(UINT32) * TVD_CC_PER_SAMPLE_WINDOW_SIZE_BY_32 );
                
                _afgCCPlaying[ui1_path] = TRUE;
            }
            
            VBI_StartNotifyCCOnOff(ui1_path, SV_ON, NULL);
            
            i4RetCode = RMR_OK;
        }
        else
        {
            i4RetCode = RMR_DRV_INV_SET_INFO;
        }
        break;
        
    case TV_DEC_SET_TYPE_STOP_CC:
        
        TVD_DBG_MSG( "\r\n[TVD%d] STOP CC\r\n",  pt_comp_id->ui2_id );
        
        VBI_StartNotifyCCOnOff(ui1_path, SV_OFF, NULL);
        
        if(_afgCCPlaying[ui1_path])
        {
            _afgCCPlaying[ui1_path] = FALSE;
        }
        
        i4RetCode = RMR_OK;
        
        break;
        
    case TV_DEC_SET_TYPE_COLOR_SYS:
        switch((TV_DEC_COLOR_SYS_T)(UINT32)pv_set_info)
        {
            case TV_DEC_COLOR_SYS_UNKNOWN:
                bColSys = SV_CS_AUTO;
                break;
            
            case TV_DEC_COLOR_SYS_NTSC:
                if(TVD_COLOR_SYS_CAP & TV_DEC_COLOR_SYS_CAP_NTSC)
                {
                    bColSys = SV_CS_NTSC358;
                }
                else
                {
                    fgIsCap = FALSE;
                }
                break;
            
            case TV_DEC_COLOR_SYS_NTSC_443:
                if(TVD_COLOR_SYS_CAP & TV_DEC_COLOR_SYS_CAP_NTSC_443)
                {
                    bColSys = SV_CS_NTSC443;
                }
                else
                {
                    fgIsCap = FALSE;
                }
                break;
            
            case TV_DEC_COLOR_SYS_PAL:
                if(TVD_COLOR_SYS_CAP & TV_DEC_COLOR_SYS_CAP_PAL)
                {
                    bColSys = SV_CS_PAL;
                }
                else
                {
                    fgIsCap = FALSE;
                }
                break;
            
            case TV_DEC_COLOR_SYS_PAL_N:
                if(TVD_COLOR_SYS_CAP & TV_DEC_COLOR_SYS_CAP_PAL_N)
                {
                    bColSys = SV_CS_PAL_N;
                }
                else
                {
                    fgIsCap = FALSE;
                }
                break;
            
            case TV_DEC_COLOR_SYS_PAL_M:
                if(TVD_COLOR_SYS_CAP & TV_DEC_COLOR_SYS_CAP_PAL_M)
                {
                    bColSys = SV_CS_PAL_M;
                }
                else
                {
                    fgIsCap = FALSE;
                }
                break;
            
            case TV_DEC_COLOR_SYS_PAL_60:
                if(TVD_COLOR_SYS_CAP & TV_DEC_COLOR_SYS_CAP_PAL_60)
                {
                    bColSys = SV_CS_PAL_60;
                }
                else
                {
                    fgIsCap = FALSE;
                }
                break;
            
            case TV_DEC_COLOR_SYS_SECAM:
                if(TVD_COLOR_SYS_CAP & TV_DEC_COLOR_SYS_CAP_SECAM)
                {
                    bColSys = SV_CS_SECAM;
                }
                else
                {
                    fgIsCap = FALSE;
                }
                break;
            
            default:
                ASSERT(0);
                return RMR_DRV_INV_SET_INFO;
        }
        
        if( (fgIsCap == FALSE) || 
            (fgApiVideoColorSystem(bColSys) == FALSE))
        {
               i4RetCode = RMR_DRV_INV_SET_INFO;
        }
        else
        {
            i4RetCode = RMR_OK;
        }
        break;

    case TV_DEC_SET_TYPE_CLOCK:
        i2Tmp = (UINT16)(UINT32)pv_set_info;
        
        if(fgApiVideoVgaSetClock((UINT16)(i2Tmp+127)))
        {
            i2GetTypeClock = i2Tmp;
            i4RetCode = RMR_OK;
        }
        else
        {
            i4RetCode = RMR_DRV_INV_SET_INFO;
        }
        break;

    case TV_DEC_SET_TYPE_PHASE:
        if(fgApiVideoVgaSetPhase((UINT16)(UINT32)pv_set_info))
        {
            u2GetTypePhase = (UINT16)(UINT32)pv_set_info;
            i4RetCode = RMR_OK;
        }
        else
        {
            i4RetCode = RMR_DRV_INV_SET_INFO;
        }
        break;
    case TV_DEC_SET_TYPE_H_POSITION:
    	{
		INT32 pos,min_off,max,min,def;
		bDecType = bGetVideoDecType(ui1_path);

		def=wDrvVideoGetPorch((UINT8)ui1_path, SV_HPORCH_DEFAULT);
		max=wDrvVideoGetPorch((UINT8)ui1_path, SV_HPORCH_MAX);
		min=wDrvVideoGetPorch((UINT8)ui1_path, SV_HPORCH_MIN);
		
		if (bDrvGetPorchTune((UINT8)ui1_path) == SV_PORCHTUNE_SCPOS)//for DTV
		{
		       pos=32-(UINT32)pv_set_info+def;
		       wDrvVideoSetPorch(ui1_path, SV_HPORCH_CURRENT,pos);
		}
		else
		{
			if (bDecType == SV_VD_VGA)
			{
				min_off=(INT32)pv_set_info-min;
				pos=max-min_off;
			}
			else
			{
				min_off=(INT32)pv_set_info-def;
				pos=def-min_off;
			}		
	    		wDrvVideoSetPorch(ui1_path, SV_HPORCH_CURRENT, pos);
	    	}
	}
	    u2GetTypeHPos =  (UINT16)(UINT32)pv_set_info;
	    i4RetCode = RMR_OK;
    	break;
    case TV_DEC_SET_TYPE_V_POSITION:
    	{
    		 bDecType = bGetVideoDecType(ui1_path);
		if (bDrvGetPorchTune((UINT8)ui1_path) == SV_PORCHTUNE_SCPOS)//for DTV
		{
			UINT16 def;
			vDrvSetPorchTune((UINT8)ui1_path,SV_PORCHTUNE_SCPOS);
			def=wDrvVideoGetPorch(ui1_path,SV_VPORCH_DEFAULT);
			wDrvVideoSetPorch(ui1_path, SV_VPORCH_CURRENT, (INT32)pv_set_info-16+def);
		}
		else
		{
			wDrvVideoSetPorch(ui1_path, SV_VPORCH_CURRENT, (UINT16)(UINT32)pv_set_info);
		}
	}
	    u2GetTypeHPos =  (UINT16)(UINT32)pv_set_info;
	    i4RetCode = RMR_OK;
    	break;
    case TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS:
        i4RetCode = RMR_ASYNC_NFY;
        if ( pt_rv_channel->u2_e_type == DRVT_AVC_VGA )
        {
            fgApiVideoVgaAuto();
        }
        else if ( pt_rv_channel->u2_e_type == DRVT_AVC_Y_PB_PR )
        {
            fgApiVideoYPbPrAuto();
        }
        else
        {
            i4RetCode = RMR_DRV_SET_FAILED;
        }
        break;

    case TV_DEC_SET_TYPE_AUTO_COLOR:
        if (ui1_path == SV_VP_MAIN)
        {
            bGetSrc=0;
        }
        else if (ui1_path == SV_VP_PIP)
        {
            bGetSrc=1;
        }
#if SUPPORT_SCART  //Gene Chang For the SCART Auto to remove the S first function 2006.05.12
	if (_bMainICIn == P_FB0 || _bMainICIn == P_FB1
#if SUPPORT_POP
			|| _bSubICIn == P_FB0 || _bSubICIn == P_FB1
#endif
	   )
	{
	   if(fgApiVideoAutoColor())
	   {
		   i4RetCode = RMR_ASYNC_NFY;
	   }
	else
	   {
		//_TvdNotifyAutoDone(pt_comp_id->ui2_id, FALSE);
		   i4RetCode = RMR_DRV_INV_SET_INFO;
	   }
	}
else
	{
#endif
 	       bDecType = bGetVideoDecType(ui1_path);
	      	 if ((bDecType == SV_VD_VGA) || (bDecType == SV_VD_YPBPR))
	        {
	            if(fgApiVideoAutoColor())
	            {
	                i4RetCode = RMR_ASYNC_NFY;
	            }
		     else
	            {
	             //_TvdNotifyAutoDone(pt_comp_id->ui2_id, FALSE);
	                i4RetCode = RMR_DRV_INV_SET_INFO;
	            }
	        }
	        else if(bDecType == SV_VD_TVD3D)
	        {
	            if(fgApiVideoCVBSAutoColor())
	            {
	                i4RetCode = RMR_ASYNC_NFY;
	            }
		     else
	            {
	             //_TvdNotifyAutoDone(pt_comp_id->ui2_id, FALSE);
	                i4RetCode = RMR_DRV_INV_SET_INFO;
	            }
	        }
	}
        break;
        
    case TV_DEC_SET_TYPE_SCART_INP_MODE:
        if ((z_set_info_len == sizeof(TV_DEC_SCART_MODE_T)) &&
            (pv_set_info != NULL))
        {
            //Gene Chang For the SCART driver
            vScartUIChangeInputMode((*(TV_DEC_SCART_MODE_T*)pv_set_info)-1);
            i4RetCode = RMR_OK;            
        }
        else
        {
            i4RetCode = RMR_DRV_INV_SET_INFO;
        }        
        break;        
                
    case TV_DEC_SET_TYPE_PLAY_TTX:
        if ( (z_set_info_len == sizeof(TV_DEC_TTX_NFY_INFO_T)) &&
              pv_set_info != NULL                              &&
             (((TV_DEC_TTX_NFY_INFO_T *)pv_set_info)->pf_ttx_nfy != NULL) )
        {
            BOOL            fgTtxAlreadyEnabled;
            CRIT_STATE_T    rCS;
            
            TVD_DBG_MSG( "\r\n[TVD%d] PLAY TTX\r\n",  pt_comp_id->ui2_id );
            
            rCS = x_crit_start ();
            _arTtxNfyInfo[ui1_path].pv_tag     = ((TV_DEC_TTX_NFY_INFO_T *)pv_set_info)->pv_tag;
            _arTtxNfyInfo[ui1_path].pf_ttx_nfy = ((TV_DEC_TTX_NFY_INFO_T *)pv_set_info)->pf_ttx_nfy;
            fgTtxAlreadyEnabled                = _afgTtxEnabled[ui1_path];
            _afgTtxEnabled[ui1_path]           = TRUE;
            x_crit_end (rCS);
            
            if ( ! _TvdTtxBufChkInited ( & _arTtxBufCtx[ui1_path] ) )
            {
                #ifndef LINUX_TURNKEY_SOLUTION
                _TvdTtxBufInit ( & _arTtxBufCtx[ui1_path] );
                #else
                _TvdTtxBufInit (ui1_path, & _arTtxBufCtx[ui1_path]);
                #endif
            }
            
            if (!fgTtxAlreadyEnabled)
            {
                VBI_StartNotifyTTXOnOff(ui1_path, SV_ON, NULL);
            }
            
            i4RetCode = RMR_OK;
        }
        else
        {
            i4RetCode = RMR_DRV_INV_SET_INFO;
        }
        break;
        
    case TV_DEC_SET_TYPE_STOP_TTX:
        {
            CRIT_STATE_T    rCS;
            
            TVD_DBG_MSG( "\r\n[TVD%d] STOP TTX\r\n",  pt_comp_id->ui2_id );
            
            if (_afgTtxEnabled[ui1_path])
            {
                VBI_StartNotifyTTXOnOff(ui1_path, SV_OFF, NULL);
            }

            rCS = x_crit_start ();
            _arTtxNfyInfo[ui1_path].pv_tag     = NULL;
            _arTtxNfyInfo[ui1_path].pf_ttx_nfy = NULL;
            _afgTtxEnabled[ui1_path]           = FALSE;
            x_crit_end (rCS);
                        
            i4RetCode = RMR_OK;
        }
        break;
    
    case TV_DEC_SET_TYPE_UNLOCK_TTX_BUFF:
        if ( (z_set_info_len == sizeof(TV_DEC_TTX_BUFF_INFO_T))           &&
              pv_set_info != NULL                                         &&
              (((TV_DEC_TTX_BUFF_INFO_T *)pv_set_info)->ui2_num_pkt > 0)  &&
              (((TV_DEC_TTX_BUFF_INFO_T *)pv_set_info)->at_pkts != NULL) )
        {
            TV_DEC_TTX_BUFF_INFO_T  *prBufInfo = (TV_DEC_TTX_BUFF_INFO_T *) pv_set_info;
            INT32                   i4Ret;
       #ifdef LINUX_TURNKEY_SOLUTION
            UINT32 ui4_kernel_addr = 0;
       #endif
       
            TVD_DBG_MSG( "\r\n[TVD%d] UNLOCK TTX BUF \r\n",  pt_comp_id->ui2_id );

        #ifndef LINUX_TURNKEY_SOLUTION
            i4Ret = _TvdTtxBufFree( 
                & _arTtxBufCtx[ui1_path],
                prBufInfo->ui2_num_pkt,
                prBufInfo->at_pkts );
        #else
            ui4_kernel_addr = ttx_addr_user_to_kernel(ui1_path, (UINT32)prBufInfo->at_pkts);
                i4Ret = _TvdTtxBufFree( 
                & _arTtxBufCtx[ui1_path],
                prBufInfo->ui2_num_pkt,
                (TV_DEC_TTX_PKT_T*) ui4_kernel_addr);
        #endif
            if ( i4Ret == TTX_BUFR_OK )
            {
                i4RetCode = RMR_OK;
            }
            else
            {
                /* Internal Error in TTX Buffer */
                
                TVD_DBG_MSG( "\r\n[TVD%d] TTX BUF FREE ERROR (%d) \r\n",  pt_comp_id->ui2_id, i4Ret );
                
                i4RetCode = RMR_DRV_SET_FAILED;
            }
        }
        else
        {
            i4RetCode = RMR_DRV_INV_SET_INFO;
        }
        break;
        
    case TV_DEC_SET_TYPE_FLUSH_TTX_BUFF:
        {
            CRIT_STATE_T    rCS;
            
            TVD_DBG_MSG( "\r\n[TVD%d] FLUSH TTX BUF \r\n", pt_comp_id->ui2_id );
            
            _TvdTtxBufFlush( & _arTtxBufCtx[ui1_path] );
            
            rCS = x_crit_start ();
            _afgTtxBufErrPaused[ui1_path] = FALSE;
            x_crit_end (rCS);
            
            i4RetCode = RMR_OK;
        }
        break;
    
    case TV_DEC_SET_TYPE_MONITOR_VPS_DATA:
        if ( ( ui1_path == SV_VP_MAIN && (_rMChannel.u4_Cap & TV_DEC_CAP_VPS) ) || 
             ( ui1_path == SV_VP_PIP  && (_rPChannel.u4_Cap & TV_DEC_CAP_VPS) )  ) 
        {
            if ( (z_set_info_len == sizeof(TV_DEC_VPS_DATA_NFY_INFO_T)) &&
                  pv_set_info != NULL                                   &&
                 (((TV_DEC_VPS_DATA_NFY_INFO_T *)pv_set_info)->pf_vps_data_nfy != NULL) )
            {
                TVD_DBG_MSG( "\r\n[TVD%d] PLAY VPS DATA \r\n", pt_comp_id->ui2_id );
                
                _rVpsDataNfyInfo.pv_tag          = ((TV_DEC_VPS_DATA_NFY_INFO_T *)pv_set_info)->pv_tag;
                _rVpsDataNfyInfo.pf_vps_data_nfy = ((TV_DEC_VPS_DATA_NFY_INFO_T *)pv_set_info)->pf_vps_data_nfy;
                
                x_memset (_au1VpsDataNtfd, 0, sizeof (TV_DEC_VPS_DATA_SIZE));
                
                /* When VBI_StartNotifyVPSOnOff (SV_ON) is called, */
                /* VBI slicer dedicated for component source will be automatically attached to the path which contain TV source */
                VBI_StartNotifyVPSOnOff(SV_ON, NULL);

                i4RetCode = RMR_OK;
            }
            else
            {
                i4RetCode = RMR_DRV_INV_SET_INFO;
            }
        }
        else
        {
            /* VPS is not supported in this input source */
            i4RetCode = RMR_OK;
        }
        break;
        
    case TV_DEC_SET_TYPE_STOP_VPS_DATA:
        if ( ( ui1_path == SV_VP_MAIN && (_rMChannel.u4_Cap & TV_DEC_CAP_VPS) ) || 
             ( ui1_path == SV_VP_PIP  && (_rPChannel.u4_Cap & TV_DEC_CAP_VPS) )  ) 
        {
            TVD_DBG_MSG( "\r\n[TVD%d] STOP VPS DATA\r\n",  pt_comp_id->ui2_id );
            
            /* after stopping VPS, VBI slicer dedicated for component will be resumed back attaching to the component source */
            VBI_StartNotifyVPSOnOff(SV_OFF, NULL);
            
            x_memset (&_rVpsDataNfyInfo, 0, sizeof (TV_DEC_VPS_DATA_NFY_INFO_T));
            
            i4RetCode = RMR_OK;
        }
        else
        {
            /* VPS is not supported in this input source */
            i4RetCode = RMR_OK;
        }
        break;
        
    case TV_DEC_SET_TYPE_PLAY_EU_WSS:
        if ( ( ui1_path == SV_VP_MAIN && (_rMChannel.u4_Cap & TV_DEC_CAP_EU_WSS) ) || 
             ( ui1_path == SV_VP_PIP  && (_rPChannel.u4_Cap & TV_DEC_CAP_EU_WSS) )  ) 
        {
            if ( (z_set_info_len == sizeof(TV_DEC_EU_WSS_NFY_INFO_T)) &&
                  pv_set_info != NULL                                  &&
                 (((TV_DEC_EU_WSS_NFY_INFO_T *)pv_set_info)->pf_eu_wss_nfy != NULL) )
            {
                TVD_DBG_MSG( "\r\n[TVD%d] PLAY EU WSS \r\n",  pt_comp_id->ui2_id );
                
                _arEuWssNfyInfo[ui1_path].pv_tag        = ((TV_DEC_EU_WSS_NFY_INFO_T *)pv_set_info)->pv_tag;
                _arEuWssNfyInfo[ui1_path].pf_eu_wss_nfy = ((TV_DEC_EU_WSS_NFY_INFO_T *)pv_set_info)->pf_eu_wss_nfy;
                
                /* start playing */
                _afgEuWssPlayed[ui1_path] = TRUE;
                
                i4RetCode = RMR_OK;
            }
            else
            {
                i4RetCode = RMR_DRV_INV_SET_INFO;
            }
        }
        else
        {
            /* EU WSS is not supported in this input source */
            i4RetCode = RMR_OK;
        }
        break;

    /* US WSS525 */
    case TV_DEC_SET_TYPE_PLAY_US_WSS:
        if ( ( ui1_path == SV_VP_MAIN && (_rMChannel.u4_Cap & TV_DEC_CAP_US_WSS) ) || 
             ( ui1_path == SV_VP_PIP  && (_rPChannel.u4_Cap & TV_DEC_CAP_US_WSS) )  ) 
        {
            if ( (z_set_info_len == sizeof(TV_DEC_US_WSS_NFY_INFO_T)) &&
                   pv_set_info != NULL                                  &&
                  (((TV_DEC_US_WSS_NFY_INFO_T *)pv_set_info)->pf_us_wss_nfy != NULL) )
            {
                TVD_DBG_MSG( "\r\n[TVD%d] PLAY US WSS \r\n",  pt_comp_id->ui2_id );
                    
                _arUsWssNfyInfo[ui1_path].pv_tag        = ((TV_DEC_US_WSS_NFY_INFO_T *)pv_set_info)->pv_tag;
                _arUsWssNfyInfo[ui1_path].pf_us_wss_nfy = ((TV_DEC_US_WSS_NFY_INFO_T *)pv_set_info)->pf_us_wss_nfy;
                    
                /* start playing */
                _afgUsWssPlayed[ui1_path] = TRUE;
                    
                i4RetCode = RMR_OK;
            }
            else
            {
                i4RetCode = RMR_DRV_INV_SET_INFO;
            }
        }
        else
        {
            /* US WSS is not supported in this input source */
            i4RetCode = RMR_OK;
        }
        break;

    case TV_DEC_SET_TYPE_STOP_EU_WSS:
        if ( ( ui1_path == SV_VP_MAIN && (_rMChannel.u4_Cap & TV_DEC_CAP_EU_WSS) ) || 
             ( ui1_path == SV_VP_PIP  && (_rPChannel.u4_Cap & TV_DEC_CAP_EU_WSS) )  ) 
        {
            TVD_DBG_MSG( "\r\n[TVD%d] STOP EU WSS\r\n",  pt_comp_id->ui2_id );

            /* stop playing */
            _afgEuWssPlayed[ui1_path] = FALSE;
            
            i4RetCode = RMR_OK;
            
        }
        else
        {
            /* EU WSS is not supported in this input source */
            i4RetCode = RMR_OK;
        }
        break;

    /* US WSS525 */        
    case TV_DEC_SET_TYPE_STOP_US_WSS:
        if ( ( ui1_path == SV_VP_MAIN && (_rMChannel.u4_Cap & TV_DEC_CAP_US_WSS) ) || 
             ( ui1_path == SV_VP_PIP  && (_rPChannel.u4_Cap & TV_DEC_CAP_US_WSS) )  ) 
        {
            TVD_DBG_MSG( "\r\n[TVD%d] STOP US WSS\r\n",  pt_comp_id->ui2_id );

            /* stop playing */
            _afgUsWssPlayed[ui1_path] = FALSE;
            
            i4RetCode = RMR_OK;
            
        }
        else
        {
            /* US WSS is not supported in this input source */
            i4RetCode = RMR_OK;
        }
        break;
    case TV_DEC_SET_TYPE_VGA_TIMING_OPTION:
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT  
        pt_tv_dec_vga_timing_option = (TV_DEC_VGA_TIMING_OPTION_T*)pv_set_info;
        vVgaSetAmbiguousTiming(pt_tv_dec_vga_timing_option->ui2_opt_index, pt_tv_dec_vga_timing_option->b_is_default);
        
        if( pt_tv_dec_vga_timing_option->ui2_opt_index == 0)
            i4RetCode = RMR_DRV_SET_FAILED;
        else
            i4RetCode = RMR_OK;
#else
			i4RetCode = RMR_DRV_SET_FAILED;	
#endif
        break;

    case TV_DEC_SET_TYPE_AMBIG_VGA_TIMING_CTRL:
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT         
        vVgaSetAmbiguousUserSel((BOOL)(UINT32)pv_set_info);
#endif        
        i4RetCode = RMR_OK;
        break;

   case TV_DEC_SET_TYPE_RESET_VGA_TIMING_OPTION:
   #ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT 
        if ( vVgaResetAmbiguousTimingEEPROM() )
        	i4RetCode = RMR_OK;
        else
        	i4RetCode = RMR_DRV_SET_FAILED;
    #else
            i4RetCode = RMR_DRV_SET_FAILED;
    #endif
        break;     
        
    case TV_DEC_SET_TYPE_VGA_DETECT:
        {
            TV_DEC_VGA_DETECT_T eDetectEnable;

            eDetectEnable = (TV_DEC_VGA_DETECT_T)(UINT32)pv_set_info; 
            if(eDetectEnable == TV_DEC_VGA_ENABLE_DETECTION)
            {
                bApiVideoSetVideoSrc(SV_VP_PIP, SV_VS_VGA1);
            }
            else
            {
                bApiVideoSetVideoSrc(SV_VP_PIP, SV_VS_MAX);
            }
       	    i4RetCode = RMR_OK;
        } 
        break;
    default:
        i4RetCode = RMR_DRV_INV_SET_INFO;
        break;
    }

    return i4RetCode;
#else
    return 0;
#endif
}


void _TvdNotifyAutoDone(UINT16 ui2_id, BOOL fgSuccess)
{
    if(_arTVNfyInfo[ui2_id].pf_tv_dec_nfy == NULL)
    {
        return;
    }

  if(ui2_id > 1) ui2_id = 1;

  if(fgSuccess)
  {
    if(_arTVNfyInfo[ui2_id].pf_tv_dec_nfy)
    {
      // Call Back
    #ifndef LINUX_TURNKEY_SOLUTION
      _arTVNfyInfo[ui2_id].pf_tv_dec_nfy (_arTVNfyInfo[ui2_id].pv_tag,
                                  TV_DEC_COND_CTRL_DONE,
                                  TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS,
                                  NULL);
    #else
      set_tv_dec_nfy(ui2_id,
                     _arTVNfyInfo[ui2_id].pv_tag,
                     TV_DEC_COND_CTRL_DONE,
                     TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS,
                     (UINT32)NULL);
    #endif
    }
  }
  else
  {
    if(_arTVNfyInfo[ui2_id].pf_tv_dec_nfy)
    {
      // Call Back
    #ifndef LINUX_TURNKEY_SOLUTION
      _arTVNfyInfo[ui2_id].pf_tv_dec_nfy (_arTVNfyInfo[ui2_id].pv_tag,
                                  TV_DEC_COND_ERROR,
                                  TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS,
                                  NULL);
    #else
      set_tv_dec_nfy(ui2_id,
                     _arTVNfyInfo[ui2_id].pv_tag,
                     TV_DEC_COND_ERROR,
                     TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS,
                     (UINT32)NULL);
    #endif
    }
  }
}

void _TvdNotifyAutoColorDone(UINT16 ui2_id, BOOL fgSuccess)
{
  if(_arTVNfyInfo[ui2_id].pf_tv_dec_nfy == NULL)
  {
      return;
  }

  if(ui2_id > 1) ui2_id = 1;

  if(fgSuccess)
  {
    if(_arTVNfyInfo[ui2_id].pf_tv_dec_nfy)
    {
      // Call Back
    #ifndef LINUX_TURNKEY_SOLUTION
      _arTVNfyInfo[ui2_id].pf_tv_dec_nfy (_arTVNfyInfo[ui2_id].pv_tag,
                                  TV_DEC_COND_CTRL_DONE,
                                  TV_DEC_SET_TYPE_AUTO_COLOR,
                                  NULL);
    #else
      set_tv_dec_nfy(ui2_id,
                     _arTVNfyInfo[ui2_id].pv_tag,
                     TV_DEC_COND_CTRL_DONE,
                     TV_DEC_SET_TYPE_AUTO_COLOR,
                     (UINT32)NULL);
    #endif
    }
  }
  else
  {
    if(_arTVNfyInfo[ui2_id].pf_tv_dec_nfy)
    {
      // Call Back
    #ifndef LINUX_TURNKEY_SOLUTION
      _arTVNfyInfo[ui2_id].pf_tv_dec_nfy (_arTVNfyInfo[ui2_id].pv_tag,
                                  TV_DEC_COND_ERROR,
                                  TV_DEC_SET_TYPE_AUTO_COLOR,
                                  NULL);
    #else
      set_tv_dec_nfy(ui2_id,
                     _arTVNfyInfo[ui2_id].pv_tag,
                     TV_DEC_COND_ERROR,
                     TV_DEC_SET_TYPE_AUTO_COLOR,
                     (UINT32)NULL);
    #endif
    }
  }
}

void _TvdNotifyMVChg(UINT8 u1Path, BOOL fgStatus)
{
  if(_arTVNfyInfo[u1Path].pf_tv_dec_nfy == NULL)
  {
      return;
  }

  if(u1Path > 1) u1Path = 1;

  // Call Back
#ifndef LINUX_TURNKEY_SOLUTION
  _arTVNfyInfo[u1Path].pf_tv_dec_nfy (_arTVNfyInfo[u1Path].pv_tag,
                                  TV_DEC_COND_MACROVISION,
                                  (UINT32)fgStatus,
                                  NULL);
#else
  set_tv_dec_nfy(u1Path,
                 _arTVNfyInfo[u1Path].pv_tag,
                 TV_DEC_COND_MACROVISION,
                 (UINT32)fgStatus,
                 0);
#endif
                                  
#ifdef CC_SUPPORT_TVE
            /* trigger the notification for MonitorScart*/
            if (u1Path == SV_VP_MAIN &&
                _arTVNfyInfo[2].pf_tv_dec_nfy != NULL)
            {
            #ifndef LINUX_TURNKEY_SOLUTION
                _arTVNfyInfo[2].pf_tv_dec_nfy(
                    _arTVNfyInfo[2].pv_tag,
                    TV_DEC_COND_MACROVISION,
                    (UINT32)fgStatus,
                    0);
            #else
                set_tv_dec_nfy(u1Path,
                               _arTVNfyInfo[2].pv_tag,
                               TV_DEC_COND_MACROVISION,
                               (UINT32)fgStatus,
                               0);
            #endif
            }
#endif
}

#ifdef CC_SOURCE_AUTO_DETECT
VOID _TvdNOtifyAutoDetectSource(UINT8 u1SrcIndex, UINT8 u1SrcStatus)
{
	if(_arSRCDetectNfyInfo[SV_VP_MAIN].pf_src_detect_nfy == NULL)
	{
		return;
	}
	#ifndef LINUX_TURNKEY_SOLUTION
	_arSRCDetectNfyInfo[SV_VP_MAIN].pf_src_detect_nfy(_arSRCDetectNfyInfo[SV_VP_MAIN].pv_tag,
								TV_DEC_COND_CTRL_DONE,
								(UINT32)u1SrcIndex,
								(UINT32)u1SrcStatus);
	#else
	set_tv_dec_src_detect_nfy(SV_VP_MAIN,
	                 		  _arSRCDetectNfyInfo[SV_VP_MAIN].pv_tag,
	                 		  TV_DEC_COND_CTRL_DONE,
	                 		  (UINT32)u1SrcIndex,
	                 		  (UINT32)u1SrcStatus);

	#endif
}


#endif



VOID _TvdNotifyResolutionChg(UINT8 u1Path, UINT8 u1SigState)
{
    CRIT_STATE_T              t_cs;
    RVChannel*                pt_rv_channel = NULL;
    TV_DEC_RESOLUTION_INFO_T  t_old_res, t_resolution;
    _NotifyStatus*            pt_nfy_sts = NULL;
    BOOL                      b_should_nfy = FALSE;
    DRV_TYPE_T                e_curr_src_type = DRVT_UNKNOWN;
    DRV_CUSTOM_VID_SRC_HDMI_AVI_INFOFRAME_INFO_T t_AVI_infoframe;
    UINT8 i;
    #if (defined CC_MT5360) || (defined CC_MT5363) ||(defined CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || (defined CC_MT5368) || (defined CC_MT5389)
    UINT8 bHDCPStatus;
    #endif
    #ifdef CC_AUD_HDMI_SPDIF_CFG
    UINT8 bAud_SPDIF_Status;
    #endif

    HDMI_INFOFRAME_DESCRIPTION *rAviInfo;

    x_memset(&t_resolution, 0, sizeof(TV_DEC_RESOLUTION_INFO_T));
    t_resolution.t_hdmi_infoframe.b_valid=0;
    for(i=0;i<TVD_HDMI_INFO_FRAME_LEN;i++)
    {
        t_resolution.t_hdmi_infoframe.aui1_info_frame_data[i]=0;
    }
    if( u1Path == SV_VP_MAIN )
    {
        pt_rv_channel = &_rMChannel;
        pt_nfy_sts = &_tMainNotifyStatus;
    }
    else if ( u1Path == SV_VP_PIP )
    {
        pt_rv_channel = &_rPChannel;
        pt_nfy_sts = &_tPipNotifyStatus;
    }
    else
    {
        return;
    }

    t_cs = x_crit_start();
    
    e_curr_src_type = pt_rv_channel->u2_e_type;
    
    x_crit_end(t_cs);
    
    if( (_arTVNfyInfo[u1Path].pf_tv_dec_nfy == NULL) || 
        (u1SigState == SV_VDO_NOSIGNAL) )
    {
        return;
    }
    if ( u1SigState == SV_VDO_UNKNOWN )
    {
        return;
    }
    
    t_resolution.ui2_frame_rate = bDrvVideoGetRefreshRate((UINT8)u1Path);
    t_resolution.ui2_width = wApiVideoGetInputWidth((UINT8)u1Path);
    t_resolution.ui2_height = wDrvVideoInputHeight((UINT8)u1Path);
    t_resolution.b_is_progressive = (SV_FALSE == bDrvVideoIsSrcInterlace((UINT8)u1Path));
    t_resolution.e_src_asp_ratio = TV_DEC_SRC_ASPECT_RATIO_UNKNOWN;

#ifdef CC_3D_SUPPORT
    switch(u4DrvTDTVGetTag3D((UINT8)u1Path))
    {
        case TDTV_TAG3D_2D:
            t_resolution.e_tag3d_type=TV_DEC_TAG3D_2D;
            break;              
        case TDTV_TAG3D_MVC:
            t_resolution.e_tag3d_type=TV_DEC_TAG3D_MVC;
            break;  
        case TDTV_TAG3D_FP:
            t_resolution.e_tag3d_type=TV_DEC_TAG3D_FP;
            break;
        case TDTV_TAG3D_FS:
            t_resolution.e_tag3d_type=TV_DEC_TAG3D_FS;
            break;              
        case TDTV_TAG3D_TB:
            t_resolution.e_tag3d_type=TV_DEC_TAG3D_TB;
            break;  
        case TDTV_TAG3D_SBS:
            t_resolution.e_tag3d_type=TV_DEC_TAG3D_SBS;
            break;  
        case TDTV_TAG3D_REALD:
            t_resolution.e_tag3d_type=TV_DEC_TAG3D_REALD;
            break;              
        case TDTV_TAG3D_SENSIO:
            t_resolution.e_tag3d_type=TV_DEC_TAG3D_SENSIO;
            break; 
        case TDTV_TAG3D_LI:            
            t_resolution.e_tag3d_type=TV_DEC_TAG3D_LA;
            break;             
        case TDTV_TAG3D_TTDO:
            t_resolution.e_tag3d_type=TV_DEC_TAG3D_TTDO;
            break;  
        case TDTV_TAG3D_NOT_SUPPORT:
        default:         
            t_resolution.e_tag3d_type=TV_DEC_TAG3D_NOT_SUPPORT;
    }
#else
    t_resolution.e_tag3d_type=TV_DEC_TAG3D_2D;
#endif

    if ( u1SigState == SV_VDO_NOSUPPORT )
    {
        t_resolution.e_timing_type = TV_DEC_TIMING_NOT_SUPPORT;
    }
    else
    {
        t_resolution.e_timing_type = ( (fgApiVideoIsVgaModeEx( u1Path, 0))?
                                       TV_DEC_TIMING_GRAPHIC:
                                       TV_DEC_TIMING_VIDEO );
    }
    t_resolution.ui4_res_idx = ( (((UINT32)bApiVideoGetInputPolarity(u1Path))<<24) |
                                 bApiVideoGetInputTimingID(u1Path) );
    
    t_resolution.ui4_ext = 0;
    if ( e_curr_src_type == DRVT_AVC_HDMI )
    {
    #if (defined CC_MT5360) || (defined CC_MT5363) ||(defined CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || (defined CC_MT5368) || (defined CC_MT5389)

    	                /* notification HDCP status */
    	                bHDCPStatus=bHDMIHDCPStatusGet();
    #ifndef LINUX_TURNKEY_SOLUTION
                _arTVNfyInfo[u1Path].pf_tv_dec_nfy (
                    _arTVNfyInfo[u1Path].pv_tag,
                    TV_DEC_COND_HDCP_CONTENT,
                    (UINT32)bHDCPStatus,
                    0);
    #else
        set_tv_dec_nfy(u1Path,
                       _arTVNfyInfo[u1Path].pv_tag,
                       TV_DEC_COND_HDCP_CONTENT,
                       (UINT32)bHDCPStatus,
                       0);
    #endif /* LINUX_TURNKEY_SOLUTION */

#ifdef CC_SUPPORT_TVE
    if(u1Path == SV_VP_MAIN &&_afgTvdIsConnected[2] != FALSE &&_arTVNfyInfo[2].pf_tv_dec_nfy != NULL )
    {    	                
        #ifndef LINUX_TURNKEY_SOLUTION
                _arTVNfyInfo[2].pf_tv_dec_nfy (
                _arTVNfyInfo[2].pv_tag,
                TV_DEC_COND_HDCP_CONTENT,
                (UINT32)bHDCPStatus,
                0);
        #else
            set_tv_dec_nfy(2,
                           _arTVNfyInfo[2].pv_tag,
                           TV_DEC_COND_HDCP_CONTENT,
                           (UINT32)bHDCPStatus,
                           0);
        #endif
    }        
#endif
    #endif
        TV_DEC_RES_EXT_SET_HDMI_SCAN_INFO( t_resolution.ui4_ext, bHDMIScanInfo() );
        TV_DEC_RES_EXT_SET_HDMI_PIC_ASP_RATIO( t_resolution.ui4_ext, bHDMIAspectRatio() );
        TV_DEC_RES_EXT_SET_HDMI_AFAR( t_resolution.ui4_ext, bHDMIAFD() );
        /*_MW_VDP_HDMI_CB(u1Path,(UINT16) bHDMIAspectRatio());*/
		
   //     _MW_VDP_HDMI_AFD_CB(u1Path,bHDMIAFD());

        #if 1
               rAviInfo = API_HDMI_Get_InfoFrame(HDMI_InfoFrame_AVI);

               if (rAviInfo->framesize == 0)
               {
			t_resolution.t_hdmi_infoframe.b_valid = 0;
			
			for(i=0;i<TVD_HDMI_INFO_FRAME_LEN;i++)
			{
				t_resolution.t_hdmi_infoframe.aui1_info_frame_data[i]=0;
			}
			
			 t_AVI_infoframe.ui1_byte_1 = 0;
			 t_AVI_infoframe.ui1_byte_2 = 0;
			 t_AVI_infoframe.ui1_byte_3 = 0;
			 
               }
               else
               {
			t_resolution.t_hdmi_infoframe.b_valid = 1;
			
			t_resolution.t_hdmi_infoframe.aui1_info_frame_data[0] = rAviInfo->framedata[1] & 0xff;
			t_resolution.t_hdmi_infoframe.aui1_info_frame_data[1] = (rAviInfo->framedata[1] >> 8) & 0xff;
			t_resolution.t_hdmi_infoframe.aui1_info_frame_data[2] = (rAviInfo->framedata[1] >> 16) & 0xff;
			t_resolution.t_hdmi_infoframe.aui1_info_frame_data[3] = (rAviInfo->framedata[1] >> 24 )& 0xff;
			t_resolution.t_hdmi_infoframe.aui1_info_frame_data[4] = rAviInfo->framedata[2] & 0xff;
			t_resolution.t_hdmi_infoframe.aui1_info_frame_data[5] = (rAviInfo->framedata[2] >> 8) & 0xff;
				
			t_AVI_infoframe.ui1_byte_1= rAviInfo->framedata[1] & 0xff;
			t_AVI_infoframe.ui1_byte_2= (rAviInfo->framedata[1] >> 8) & 0xff;
			t_AVI_infoframe.ui1_byte_3= (rAviInfo->framedata[1] >> 16) & 0xff;
               }
               
        #else
		   if ((u1RegRead1B(AVIRX0_0) != 0x82) ||(!_bHdmiMode))
		{
			t_resolution.t_hdmi_infoframe.b_valid=0;
			for(i=0;i<TVD_HDMI_INFO_FRAME_LEN;i++)
			{
				t_resolution.t_hdmi_infoframe.aui1_info_frame_data[i]=0;
			}
			 t_AVI_infoframe.ui1_byte_1= 0;
			 t_AVI_infoframe.ui1_byte_2= 0;
			 t_AVI_infoframe.ui1_byte_3= 0;
		}
			else
			{
				t_resolution.t_hdmi_infoframe.b_valid=1;
				t_resolution.t_hdmi_infoframe.aui1_info_frame_data[0]=u1RegRead1B(AVIRX1_0);
				t_resolution.t_hdmi_infoframe.aui1_info_frame_data[1]=u1RegRead1B(AVIRX1_1);
				t_resolution.t_hdmi_infoframe.aui1_info_frame_data[2]=u1RegRead1B(AVIRX1_2);
				t_resolution.t_hdmi_infoframe.aui1_info_frame_data[3]=u1RegRead1B(AVIRX1_3);
				t_resolution.t_hdmi_infoframe.aui1_info_frame_data[4]=u1RegRead1B(AVIRX2_0);
				t_resolution.t_hdmi_infoframe.aui1_info_frame_data[5]=u1RegRead1B(AVIRX2_1);
				
				t_AVI_infoframe.ui1_byte_1= u1RegRead1B(AVIRX1_0);
				t_AVI_infoframe.ui1_byte_2= u1RegRead1B(AVIRX1_1);
				t_AVI_infoframe.ui1_byte_3= u1RegRead1B(AVIRX1_2);
			}
	   #endif	
		
			_MW_VDP_HDMI_AVI_INFOFRAME_CB(u1Path,t_AVI_infoframe);

		
        t_resolution.e_color_fmt = ((bHDMIInputType()==0)? 
            TV_DEC_COLOR_FMT_YUV_4_2_2:TV_DEC_COLOR_FMT_VGA_4_4_4 );
        if ( u1SigState != SV_VDO_NOSUPPORT )
    	{
    	 
    	 #if 0 //DTV00073708
	        switch(bApiVideoFixColorCurState(u1Path)&FIXCOLORSPACE_MODEMASK)
		 {
		     	case SV_FIXCOLORSPACE_OFF: 
		     		t_resolution.e_timing_type = ((fgIsVideoTiming(_bDviTiming))? 
		              TV_DEC_TIMING_VIDEO:TV_DEC_TIMING_GRAPHIC);
		     	break;
			case SV_FIXCOLORSPACE_RGB_444: 
				t_resolution.e_timing_type =TV_DEC_TIMING_GRAPHIC; 
			break;
			case SV_FIXCOLORSPACE_RGB_422: 
			case SV_FIXCOLORSPACE_YCBCR: 
				t_resolution.e_timing_type =TV_DEC_TIMING_VIDEO; 
			break;
		  }
		  #endif
	}
    }
    else
    {
        /*US WSS525*/
        if (_afgUsWss525Coming[u1Path] == TRUE)
        {
            _afgUsWss525Coming[u1Path] = FALSE;            
            TV_DEC_RES_EXT_SET_NTSC_WSS_ASP_RATIO(t_resolution.ui4_ext, 
                                                  _au4UsWss525[u1Path]);
        }

        /*EU WSS625*/
        if (_afgEuWss625Coming[u1Path] == TRUE)
        {            
            _afgEuWss625Coming[u1Path] = FALSE;            
            TV_DEC_RES_EXT_SET_PAL_SECAM_WSS_ASP_RATIO(t_resolution.ui4_ext, 
                                                         _au2EuWss625[u1Path]);
        }
        
        switch( t_resolution.e_timing_type )
        {
        case TV_DEC_TIMING_GRAPHIC:
            t_resolution.e_color_fmt = TV_DEC_COLOR_FMT_VGA_4_4_4;
            break;
        case TV_DEC_TIMING_VIDEO:
            t_resolution.e_color_fmt = TV_DEC_COLOR_FMT_YUV_4_2_2;
            break;
        default:
            t_resolution.e_color_fmt = TV_DEC_COLOR_FMT_UNKNOWN;
            break;
        }
    }
    

    /* Get old resolution */
    t_cs = x_crit_start();

    t_old_res = pt_nfy_sts->t_resolution;
    b_should_nfy = (BOOL)pt_nfy_sts->u1NeedNotify;
    
    x_crit_end(t_cs);



    /* Do nothing if resolution has no change and b_should_nfy is FALSE */
    if( x_memcmp( &t_old_res, &t_resolution, sizeof(TV_DEC_RESOLUTION_INFO_T) ) == 0 && 
        !b_should_nfy )
    {
		TVD_DBG_MSG( "[TVD]  resolution check return\r\n");

        return;
    }

    /* Store new resolution */
    t_cs = x_crit_start();

    pt_nfy_sts->t_resolution = t_resolution;

    x_crit_end(t_cs);

    /* Call Back */
    if( _arTVNfyInfo[u1Path].pf_tv_dec_nfy && 
        (t_resolution.ui2_frame_rate != 0 || u1SigState == SV_VDO_NOSUPPORT) )
    {
        TVD_DBG_MSG( "[TVD] res_nfy(%ux%u, %uHz, idx=%u, ext=%u e_timing_type=%u)\r\n",
            t_resolution.ui2_width,
            t_resolution.ui2_height,
            t_resolution.ui2_frame_rate,
            t_resolution.ui4_res_idx,
            t_resolution.ui4_ext, t_resolution.e_timing_type );
        
    #ifndef LINUX_TURNKEY_SOLUTION
        _arTVNfyInfo[u1Path].pf_tv_dec_nfy (
            _arTVNfyInfo[u1Path].pv_tag,
            TV_DEC_COND_RESOLUTION_CHG,
            (UINT32)&t_resolution,
            0);
    #else
        set_tv_dec_nfy(u1Path,
            _arTVNfyInfo[u1Path].pv_tag,
            TV_DEC_COND_RESOLUTION_CHG,
            (UINT32)&t_resolution,
            0);
    #endif

    #ifdef CC_AUD_HDMI_SPDIF_CFG
     if ( e_curr_src_type == DRVT_AVC_HDMI )
    {
	bAud_SPDIF_Status=API_Get_Audio_SPDIF_STATUS();
    #ifndef LINUX_TURNKEY_SOLUTION
        _arTVNfyInfo[u1Path].pf_tv_dec_nfy (
            _arTVNfyInfo[u1Path].pv_tag,
            TV_DEC_COND_AUD_PROTECT_INFO,
            (UINT32)bAud_SPDIF_Status,
            0);
    #else
        set_tv_dec_nfy(u1Path,
            _arTVNfyInfo[u1Path].pv_tag,
            TV_DEC_COND_AUD_PROTECT_INFO,
            (UINT32)bAud_SPDIF_Status,
            0);
    #endif
     }
    #endif
    
#ifdef CC_SUPPORT_TVE
        /* notify for monitor scart */
        if(u1Path == SV_VP_MAIN &&
           _afgTvdIsConnected[2] != FALSE &&
           _arTVNfyInfo[2].pf_tv_dec_nfy != NULL )
        {
        #ifndef LINUX_TURNKEY_SOLUTION
            _arTVNfyInfo[2].pf_tv_dec_nfy (
                _arTVNfyInfo[2].pv_tag,
                TV_DEC_COND_RESOLUTION_CHG,
                (UINT32)&t_resolution,
                0);
        #else
            set_tv_dec_nfy(2,
               _arTVNfyInfo[2].pv_tag,
               TV_DEC_COND_RESOLUTION_CHG,
               (UINT32)&t_resolution,
               0);
        #endif
        }
#endif
        
        t_cs = x_crit_start();
        
        pt_nfy_sts->u1NeedNotify = FALSE;
        
        x_crit_end(t_cs);
    }

    if (pt_nfy_sts->ucVdpId < VDP_COMP_NS)
    {
        MW_VDP_CallbackTrigger(pt_nfy_sts->ucVdpId);
    }
}

static VOID _TvdCCUtilUpdatePER(
    TVD_CC_STATUS_T*        prCCStatus,
    UINT8                   u1Data)
{
#ifndef LINUX_TURNKEY_SOLUTION
    INT32               i4Idx;
    UINT32              u4IsErrPre8, u4IsErrNxt, u4IsErrData;
    register UINT32     u4Samples;
    
    u4IsErrData = (ODD_PARITY_DECODE_ERROR == x_lsb_odd_parity_check(u1Data)) ? TRUE : FALSE; 
    u4IsErrNxt  = u4IsErrData;
    
    for(i4Idx = 0; i4Idx < TVD_CC_PER_SAMPLE_WINDOW_SIZE_BY_32; i4Idx++)
    {
        u4Samples = prCCStatus->au4Samples[i4Idx];
        
        u4IsErrPre8 = u4Samples & ((UINT32) 0x80000000);
        
        u4Samples <<= 1;
        
        if (u4IsErrNxt)
        {
            u4Samples |= ((UINT32) 0x00000001);
        }
        
        prCCStatus->au4Samples[i4Idx] = u4Samples;
        
        u4IsErrNxt = u4IsErrPre8;
    }
    
    if(u4IsErrPre8)
    {
        if(prCCStatus->u2ErrRate > 0)
        {
            prCCStatus->u2ErrRate --;
        }
        else
        {
            TVD_DBG_CC_ERR("[CC] Internal error @ %s:%d\n", __FILE__, __LINE__);
        }
    }
    
    if(u4IsErrData)
    {
        if(prCCStatus->u2ErrRate < TVD_CC_PER_SAMPLE_WINDOW_SIZE)
        {
            prCCStatus->u2ErrRate ++;
        }
        else
        {
            TVD_DBG_CC_ERR("[CC] Internal error @ %s:%d\n", __FILE__, __LINE__);
        }
    }
#endif /* LINUX_TURNKEY_SOLUTION */
}    

static VOID _TvdCCUtilUpdateStatus(
    UINT8 u1Path,
    TVD_CC_STATUS_T*        prCCStatus,
    UINT8                   d1, 
    UINT8                   d2)
{
    UINT16   u2NRLCurr;

#ifdef TVD_CC_ECC_SWITCH 
    UINT8  u1i = 0;
    static UINT8 _u1NRLevel[16] = {0};
    static UINT8 _u1Count = 0;
    UINT16 uNR2Avg  = 0;
    BOOL bChkECCOnOff = FALSE;

    if (bGetInternalDec(u1Path)==SV_VD_TVD3D)
    {
    	u2NRLCurr = (UINT16) bTvdNRLevel();
        _u1NRLevel[ ((_u1Count++) & 0xF)] = u2NRLCurr;

        if ((_u1Count & 0xF) == 0)
        {
            for (u1i = 0; u1i <16; u1i ++)
            {
                uNR2Avg += _u1NRLevel[u1i];
            }
            uNR2Avg = uNR2Avg>>4;  //u2Avg/16
            //Printf("NR Avg = 0x%x\n", uNR2Avg);
            bChkECCOnOff = TRUE;
        }

    }
    /* Update CC ECC by Noise level */
    if (bChkECCOnOff)
    {
        if(prCCStatus->fgECCOnOff)
        {
            /* ECC ON detect Noice Level to turn off ECC */
            if(uNR2Avg >= g_u4ECCThresHold)
            {
                prCCStatus->u2CntECCNRL++;

                if(prCCStatus->u2CntECCNRL >  TVD_CC_NRL_STATE_COUNTER_THRESHOLD)
                {
                    /* the noise level is stable in high level, ECC OFF*/
                    prCCStatus->fgECCOnOff = FALSE;
                    
                    prCCStatus->u2CntECCNRL = 0;
                    
                    TVD_DBG_CC_ERR("[TVD] CC ECC is turned off by NRL\n"); //TVD_DBG_CC_ERR
                }
            }
            else
            {
                /* the noise level is not stable, reset the counter */
                prCCStatus->u2CntECCNRL = 0;
            }
        }

        else
        {
            /* ECC OFF detect Noice Level to turn on ECC */
            if(u2NRLCurr < g_u4ECCThresHold)
            {
                prCCStatus->u2CntECCNRL++;
                
                if(prCCStatus->u2CntECCNRL >  TVD_CC_NRL_STATE_COUNTER_THRESHOLD)
                {
                    /* the noise level is stable in low level, ECC ON*/
                    prCCStatus->fgECCOnOff = TRUE;
                    
                    prCCStatus->u2CntECCNRL = 0;
                    
                    TVD_DBG_CC_ERR("[TVD] CC ECC turned on by NRL\n");
                }
            }
            else
            {
                /* the noise level is not stable, reset the counter */
                prCCStatus->u2CntECCNRL = 0;
            }
        }
    }
#endif 

    
    if(bGetInternalDec(u1Path)==SV_VD_TVD3D)
    {
    	u2NRLCurr = (UINT16) bTvdNRLevel();
    }
    else
    {
    	u2NRLCurr = 0;
    }
    
    /* update cc status by noise level */
    if(prCCStatus->fgBlockedNRL)
    {
        /* check if need to unblock */
        
        if(u2NRLCurr <= VDEC_MW_TVD_CC_NRL_THRESHOLD_UNBLOCK)
        {
            prCCStatus->u2CntStNRL++;

            if(TVD_CC_IS_STATE_STABLE(prCCStatus->u2CntStNRL, TVD_CC_NRL_STATE_COUNTER_THRESHOLD))
            {
                /* the noise level is stable in low level, unnblock it */
                prCCStatus->fgBlockedNRL = FALSE;
                
                prCCStatus->u2CntStNRL = 0;
                
                TVD_DBG_CC_ERR("[TVD] CC is turned on by NRL\n");
            }
            else
            {
                /* do nothing, just increase the state counter */
            }
        }
        else
        {
            /* the noise level is not stable, reset the counter */
            prCCStatus->u2CntStNRL = 0;
        }
    }
    else
    {
        /* check if need to block */
        
        if(u2NRLCurr >= VDEC_MW_TVD_CC_NRL_THRESHOLD_BLOCK)
        {
            prCCStatus->u2CntStNRL++;
            
            if(TVD_CC_IS_STATE_STABLE(prCCStatus->u2CntStNRL, TVD_CC_NRL_STATE_COUNTER_THRESHOLD))
            {
                /* the noise level is stable in high level, block it */
                prCCStatus->fgBlockedNRL = TRUE;
                
                prCCStatus->u2CntStNRL = 0;
                
                TVD_DBG_CC_ERR("[TVD] CC is turned off by NRL\n");
            }
            else
            {
                /* do nothing, just increase the state counter */
            }
        }
        else
        {
            /* the noise level is not stable, reset the counter */
            prCCStatus->u2CntStNRL = 0;
        }
    }
    
    /* update cc parity error rate */
    _TvdCCUtilUpdatePER(prCCStatus, d1);
    _TvdCCUtilUpdatePER(prCCStatus, d2);
    
   if(prCCStatus->fgBlockedPER)
    {
        /* check if need to unblock */
        
        if(prCCStatus->u2ErrRate <= VDEC_MW_TVD_CC_PER_THRESHOLD_UNBLOCK)
        {
            prCCStatus->u2CntStPER ++;
            
            if(prCCStatus->u2CntStPER >= TVD_CC_PER_STATE_COUNTER_THRESHOLD)
            {
                 /* the PER is stable in low level, unblock it */
                prCCStatus->fgBlockedPER = FALSE;
                
                prCCStatus->u2CntStPER = 0;
            
                TVD_DBG_CC_ERR("[TVD] CC is turned on by PER\n");
            }
            else
            {
                /* do nothing, just increase the state counter */
            }
        }
        else
        {
            /* the PER is not stable, reset the counter */
            prCCStatus->u2CntStPER = 0;
        }
    }
    else
    {
        /* check if need to block */
        
        if(prCCStatus->u2ErrRate >= VDEC_MW_TVD_CC_PER_THRESHOLD_BLOCK)
        {
            prCCStatus->u2CntStPER ++;
            
            if(prCCStatus->u2CntStPER >= TVD_CC_PER_STATE_COUNTER_THRESHOLD)
            {
                 /* the PER is stable in high level, block it */
                prCCStatus->fgBlockedPER = TRUE;
                
                prCCStatus->u2CntStPER = 0;
                
                TVD_DBG_CC_ERR("[TVD] CC is turned off by PER\n");
            }
            else
            {
                /* do nothing, just increase the state counter */
            }
        }
        else
        {
            /* the PER is not stable, reset the counter */
            prCCStatus->u2CntStPER = 0;
        }
    }
    
    /* report */
    #if 0
    if(_u4TvdMsgFlags & TVD_MSG_FLAG_CC_DATA_STATUS)
    {
        Printf ("CC: [NRL] %2d [PER] %3d/%d\n", u2NRLCurr, prCCStatus->u2ErrRate, TVD_CC_PER_SAMPLE_WINDOW_SIZE);
    }
    #endif
}

void TvdNotifyCCData(UINT8 u1Path, UINT8 d1, UINT8 d2, UINT8 field)
{
    if(u1Path >= TVD_COMP_NS)
        return;
    
    if(_arCCNfyInfo[u1Path].pf_cc_nfy)
    {
        /* report data in debug mode */
        #ifdef DEBUG
        {
            static UINT32   aui4_cnt[2] = { 0, 0 };
            
            TVD_DBG_CC_MSG( "(%d)<%.2X %.2X> ", u1Path, d1, d2 );
            aui4_cnt[u1Path]++;
            if ( aui4_cnt[u1Path] % 6 == 0 )
            {
                TVD_DBG_CC_MSG("\n");
            }
        }
        #endif
        
        /* update cc status */
        _TvdCCUtilUpdateStatus(u1Path,&_arCCStatus[u1Path], d1, d2);

#ifdef TVD_CC_ECC_SWITCH 
        if (bGetInternalDec(u1Path)==SV_VD_TVD3D)
        {
            VBI_SetCCECC (_arCCStatus[u1Path].fgECCOnOff);
        }
#endif 
        
        /* notify data */
        if(!_arCCStatus[u1Path].fgBlockedNRL && 
           !_arCCStatus[u1Path].fgBlockedPER )
        {
            TV_DEC_CC_DATA_T cc_data;
                
            cc_data.ui1_byte1    = d1;
            cc_data.ui1_byte2    = d2;
            cc_data.b_even_field = field;
            
        #ifndef LINUX_TURNKEY_SOLUTION
            _arCCNfyInfo[u1Path].pf_cc_nfy (
                _arCCNfyInfo[u1Path].pv_tag,
                TV_DEC_CC_COND_DATA_ARRIVAL,
                &cc_data);
        #else
            set_cc_nfy(u1Path,
                _arCCNfyInfo[u1Path].pv_tag,
                TV_DEC_CC_COND_DATA_ARRIVAL,
                &cc_data);
        #endif
        }
    }
}

void TvdNotifyTtxData(UINT8 u1Path, UINT16 u2NumPktAvail, UINT32 _dVbiReadPtr)
{
	vMLEvtBridge_MsgSend(MLEvt_TTX, u1Path, u2NumPktAvail, _dVbiReadPtr);
}



static void _TvdProcessTtxData(UINT8 u1Path, UINT16 u2NumPktAvail, UINT32 _dVbiReadPtr)
{
#ifndef LINUX_TURNKEY_SOLUTION
    BYTE                    *pbRdr = (BYTE *) _dVbiReadPtr ;
#else
    BYTE                    *pbRdr = (BYTE *) VIRTUAL(_dVbiReadPtr);
#endif
    TV_DEC_TTX_PKT_T        *parPkts;
    INT32                   i4Ret;
    CRIT_STATE_T            rCS;
    TV_DEC_TTX_NFY_INFO_T   rTtxNfyInfo;
    BOOL                    fgTtxEnabled, fgTtxBufErrPaused;

    if(u1Path >= TVD_COMP_NS)
        return;
    
    rCS = x_crit_start ();

    fgTtxEnabled           = _afgTtxEnabled[u1Path];
    fgTtxBufErrPaused      = _afgTtxBufErrPaused[u1Path];
    rTtxNfyInfo.pv_tag     = _arTtxNfyInfo[u1Path].pv_tag;
    rTtxNfyInfo.pf_ttx_nfy = _arTtxNfyInfo[u1Path].pf_ttx_nfy;
    
    x_crit_end (rCS);
    
    if ( !fgTtxEnabled || fgTtxBufErrPaused || u2NumPktAvail == 0 )
    {
        return;
    }

    i4Ret = _TvdTtxBufAlloc( 
        & _arTtxBufCtx[u1Path],
        u2NumPktAvail,
        & parPkts );
    
    if ( i4Ret == TTX_BUFR_OK )
    {
        if ( rTtxNfyInfo.pf_ttx_nfy ) 
        {
            /* copy data */
            UINT16              u2i;
            TV_DEC_TTX_DATA_T   rTtxData;
            TV_DEC_TTX_PKT_T    *prPkt;
            
            prPkt = parPkts;
            HalFlushInvalidateDCache(); /* To make sure that Cache and DRAM are consistent */
            for ( u2i = 0; u2i < u2NumPktAvail; u2i ++ )
            {
                /* both parPkts[u2i].aui4_data and pbRdr should be 4-bytes alignment to make copy faster */
                x_memcpy( (VOID *) prPkt->aui1_data, (VOID *) pbRdr, 42);
                prPkt ++ ;
                pbRdr += 48 ;
            }

        #ifndef LINUX_TURNKEY_SOLUTION
            /* prepare notify data */
            rTtxData.t_data.ui2_num_pkt = u2NumPktAvail;
            rTtxData.t_data.at_pkts     = parPkts;
            
            /* notify data arrival */
            rTtxNfyInfo.pf_ttx_nfy(
                rTtxNfyInfo.pv_tag,
                TV_DEC_TTX_COND_DATA_ARRIVAL,
                (VOID *) & rTtxData );
        #else
            rTtxData.t_data.ui2_num_pkt = u2NumPktAvail;
            rTtxData.t_data.at_pkts= (TV_DEC_TTX_PKT_T* )ttx_addr_kernel_to_user(u1Path, (UINT32)parPkts);
            set_tv_dec_ttx_nfy(u1Path,
                rTtxNfyInfo.pv_tag,
                TV_DEC_TTX_COND_DATA_ARRIVAL,
                (VOID *) & rTtxData,
                (UINT32)0);
        #endif
        }
    }
    else
    {
        if ( i4Ret == TTX_BUFR_OVERFLOW )
        {
            rCS = x_crit_start ();
            _afgTtxBufErrPaused[u1Path] = TRUE;
            x_crit_end (rCS);
            
            /* notify data overflow */
            if ( rTtxNfyInfo.pf_ttx_nfy ) 
            {
        #ifndef LINUX_TURNKEY_SOLUTION
                rTtxNfyInfo.pf_ttx_nfy(
                    rTtxNfyInfo.pv_tag,
                    TV_DEC_TTX_COND_ERROR,
                    (VOID *) TV_DEC_TTX_ERR_BUFF_OVER_FLOW );

        #else
                set_tv_dec_ttx_nfy(u1Path,
                    rTtxNfyInfo.pv_tag,
                    TV_DEC_TTX_COND_ERROR,
                    (VOID *) TV_DEC_TTX_ERR_BUFF_OVER_FLOW,
                    (UINT32)0);
        #endif
            }
        }
        else 
        {
            rCS = x_crit_start ();
            _afgTtxBufErrPaused[u1Path] = TRUE;
            x_crit_end (rCS);
            
            /* notify buffer internal error */
            if ( rTtxNfyInfo.pf_ttx_nfy )
            {
        #ifndef LINUX_TURNKEY_SOLUTION
                rTtxNfyInfo.pf_ttx_nfy(
                    rTtxNfyInfo.pv_tag,
                    TV_DEC_TTX_COND_ERROR,
                    (VOID *) TV_DEC_TTX_ERR_BUFF_ERROR );
        #else
                set_tv_dec_ttx_nfy(u1Path,
                    rTtxNfyInfo.pv_tag,
                    TV_DEC_TTX_COND_ERROR,
                    (VOID *) TV_DEC_TTX_ERR_BUFF_ERROR,
                     (UINT32)0);
        #endif
            }
        }
    }
}    

/* size of au1Data[] is TV_DEC_VPS_DATA_SIZE */
void TvdNotifyVPSData(UINT8  au1Data[])
{
    if (x_memcmp (au1Data, _au1VpsDataNtfd, TV_DEC_VPS_DATA_SIZE))
    {
        if (_rVpsDataNfyInfo.pf_vps_data_nfy)
        {
            TV_DEC_VPS_DATA_T   t_vps_data;
            
            t_vps_data.aui1_data = au1Data;

        #ifndef LINUX_TURNKEY_SOLUTION            
            _rVpsDataNfyInfo.pf_vps_data_nfy (
                    _rVpsDataNfyInfo.pv_tag,
                    TV_DEC_VPS_DATA_COND_DATA_CHANGED,
                    (VOID *) & t_vps_data );
        #else
            set_tv_dec_vps_data_nfy(_rVpsDataNfyInfo.pv_tag,
                         TV_DEC_VPS_DATA_COND_DATA_CHANGED,
                         (VOID *) & t_vps_data);
        #endif
            TVD_DBG_MSG("[TVD] VPS Data Changed notify \n");

            x_memcpy (_au1VpsDataNtfd, au1Data, TV_DEC_VPS_DATA_SIZE);
        }
    }    
}     

void TvdNotifyEUWSSData(UINT8 u1Path, UINT16 u2Data)
{
	vMLEvtBridge_MsgSend(MLEvt_WSS_EU, u1Path, u2Data, (UINT32)NULL);
}    

/* US WSS525 */
void TvdNotifyUSWSSData(UINT8 u1Path, UINT16 u2Data)
{   
	vMLEvtBridge_MsgSend(MLEvt_WSS_US, u1Path, u2Data, (UINT32)NULL);
}


static void _TvdProcessEuWssData(UINT8 u1Path, UINT16 u2EuWss)
{
    UINT8 u1SigStatus;
    if(u1Path >= TVD_COMP_NS)
        return;
    
    /* notify VDP with error handling */
    if (u2EuWss == _au2EuWssLastRcv[u1Path])
    {
        /* notify VDP only when wss is changed */
        if (u2EuWss != _au2EuWss625[u1Path])
        {

            //LOG(3,"_TvdProcessEUWssData: upd <%x>\n", u2EuWss);
            /* notify VDP */
            _MW_VDP_WSS625_CB (GET_VDP_ID_FROM_PATH(u1Path), u2EuWss);
                    
            _au2EuWss625[u1Path] = u2EuWss;

            /* notify Resolution Changed */
            if (u1Path == SV_VP_MAIN)
                u1SigStatus = _tMainNotifyStatus.u1SigStatus;
            else
                u1SigStatus = _tPipNotifyStatus.u1SigStatus;

            _afgEuWss625Coming[u1Path] = TRUE;
            _TvdNotifyResolutionChg(u1Path, u1SigStatus);          
        }
    }

    _au2EuWssLastRcv[u1Path] = u2EuWss;

    
    /* notify middleware without error handling */
    if ( _afgEuWssPlayed[u1Path] && 
         _arEuWssNfyInfo[u1Path].pf_eu_wss_nfy ) 
    {
        TV_DEC_EU_WSS_DATA_T t_eu_wss;

        t_eu_wss.ui2_data = u2EuWss;
        TVD_DBG_MSG("[TVD] EU WSS nfy <%d>\n", u2EuWss);

    #ifndef LINUX_TURNKEY_SOLUTION
        _arEuWssNfyInfo[u1Path].pf_eu_wss_nfy (
                _arEuWssNfyInfo[u1Path].pv_tag,
                TV_DEC_EU_WSS_COND_DATA_ARRIVAL,
                & t_eu_wss );
    #else
        set_tv_dec_eu_wss_nfy(u1Path,
                _arEuWssNfyInfo[u1Path].pv_tag,
                TV_DEC_EU_WSS_COND_DATA_ARRIVAL,
                & t_eu_wss);
    #endif
    }
}

/* US WSS525 */
static void _TvdProcessUsWssData(UINT8 u1Path, UINT16 u2UsWss)
{  
    if(u1Path >= TVD_COMP_NS)
        return;
    /* Just get b1 b0 for aspect ratio */
    u2UsWss &= 0x03;
        
    /* notify middleware without error handling */
    if ( _afgUsWssPlayed[u1Path] && 
         _arUsWssNfyInfo[u1Path].pf_us_wss_nfy ) 
    {
        TV_DEC_US_WSS_DATA_T t_us_wss;

        t_us_wss.ui2_data = u2UsWss;
        TVD_DBG_MSG("[TVD] US WSS nfy <%d>\n", u2UsWss);

        _arUsWssNfyInfo[u1Path].pf_us_wss_nfy (
                _arUsWssNfyInfo[u1Path].pv_tag,
                TV_DEC_US_WSS_COND_DATA_ARRIVAL,
                & t_us_wss );
    }

    /* Notify stream manager the resolution change*/
    if (u2UsWss != _au2UsWssLastRcv[u1Path])
    {
        UINT8 u1SigStatus;

        //LOG(3, "_TvdProcessUsWssData: upd <%x>\n", u2UsWss);

        _MW_VDP_WSS525_CB (GET_VDP_ID_FROM_PATH(u1Path), u2UsWss);
        
        if (u1Path == SV_VP_MAIN)
            u1SigStatus = _tMainNotifyStatus.u1SigStatus;
        else
            u1SigStatus = _tPipNotifyStatus.u1SigStatus;

        _au4UsWss525[u1Path] = u2UsWss;
        
        _afgUsWss525Coming[u1Path] = TRUE;
        
        _TvdNotifyResolutionChg(u1Path, u1SigStatus);          
    }

    _au2UsWssLastRcv[u1Path] = u2UsWss;
}

#if 0    // To avoid warning by Pibben at 2006 7 3
static VOID _TvdNotifyVChipChange(DRV_COMP_ID_T* pt_comp_id,TV_DEC_CNT_ADV_INFO_T* vchip_data)
{
  if(_arTVNfyInfo[pt_comp_id->ui2_id].pf_tv_dec_nfy)
  {
    _arTVNfyInfo[pt_comp_id->ui2_id].pf_tv_dec_nfy (_arTVNfyInfo[pt_comp_id->ui2_id].pv_tag,
                                TV_DEC_COND_CONTENT_ADV_INFO,
                                (UINT32)(VOID*)vchip_data,
                                NULL);
  }
}
#endif

void vSetCapibility(UINT8 u1Path)
{
  UINT32 u4Cap;
  if(u1Path == SV_VP_MAIN)
  {
    if(fgIsMainTvd3d())
    {
      u4Cap = TVD_CAPABILITY;
    }
    else if(fgIsMainVga())
    {
      u4Cap = VGA_CAPABILITY;
    }
    else if(fgIsMainYPbPr())
    {
      u4Cap = YPBPR_CAPABILITY;
    }
    else
    {
      u4Cap = DVI_CAPABILITY;
    }

    _rMChannel.u4_Cap = u4Cap;
  }
  else if(u1Path == SV_VP_PIP)
  {
    if(fgIsPipTvd3d())
    {
      u4Cap = TVD_CAPABILITY;
    }
    else if(fgIsPipVga())
    {
      u4Cap = VGA_CAPABILITY;
    }
    else if(fgIsPipYPbPr())
    {
      u4Cap = (YPBPR_CAPABILITY & (~TV_DEC_CAP_AUTO_COLOR));
    }
    else
    {
      u4Cap = DVI_CAPABILITY;
    }

    _rPChannel.u4_Cap = u4Cap;
  }

}



#endif



/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

void _TvdNotifyColorSystem(UINT8 u1Path, UINT8 u1SigState)
{
    TV_DEC_COLOR_SYS_T t_col_sys;
    UINT8              u1ColSys;

    ASSERT(u1Path < SV_VP_MAX);

    if(u1SigState == SV_VDO_NOSIGNAL)
    {
        return;
    }
    
    _TvdEuWssResetData(u1Path);
    _TvdUsWssResetData(u1Path);    
    
    if ( !_arTVNfyInfo[u1Path].pf_tv_dec_nfy )
    {
        return;
    }
    
    if( ((u1Path == SV_VP_MAIN)&&(fgIsMainTvd3d())) ||
        ((u1Path == SV_VP_PIP)&&(fgIsPipTvd3d())) )
    {

        u1ColSys = bApiVideoGetColorSystem();

        if( ((u1Path == SV_VP_MAIN) && ((_tMainNotifyStatus.u1ColSys !=u1ColSys) || (_tMainNotifyStatus.u1NeedNotify == TRUE)))
            ||((u1Path == SV_VP_PIP) && ((_tPipNotifyStatus.u1ColSys !=u1ColSys) || (_tPipNotifyStatus.u1NeedNotify == TRUE))) )
        {

            if(u1Path == SV_VP_MAIN)
            {
                _tMainNotifyStatus.u1ColSys = u1ColSys;
                _tMainNotifyStatus.u1NeedNotify = FALSE;
            }
            else
            {
                _tPipNotifyStatus.u1ColSys = u1ColSys;
                _tPipNotifyStatus.u1NeedNotify = FALSE;
            }

            switch(u1ColSys)
            {
            case SV_CS_PAL_N:
                t_col_sys = TV_DEC_COLOR_SYS_PAL_N;
                break;
            case SV_CS_PAL:
                t_col_sys = TV_DEC_COLOR_SYS_PAL;
                break;
            case SV_CS_PAL_M:
                t_col_sys = TV_DEC_COLOR_SYS_PAL_M;
                break;
            case SV_CS_NTSC358:
                t_col_sys = TV_DEC_COLOR_SYS_NTSC;
                break;
            case SV_CS_SECAM:
                t_col_sys = TV_DEC_COLOR_SYS_SECAM;
                break;
            case SV_CS_NTSC443:
                t_col_sys = TV_DEC_COLOR_SYS_NTSC_443;
                break;
            case SV_CS_UNSTABLE:
            default:
                t_col_sys = TV_DEC_COLOR_SYS_UNKNOWN;
                break;
            }

            TVD_DBG_MSG("[ffr] _TvdNotifyColorSystem(%u) color_sys=%u, tick=%u\n",
                u1Path, t_col_sys, x_os_get_sys_tick() );

        #ifndef LINUX_TURNKEY_SOLUTION
            _arTVNfyInfo[u1Path].pf_tv_dec_nfy (_arTVNfyInfo[u1Path].pv_tag,
                                  TV_DEC_COND_COLOR_SYS,
                                  (UINT32)t_col_sys,
                                  0);
        #else
            set_tv_dec_nfy(u1Path,
                           _arTVNfyInfo[u1Path].pv_tag,
                           TV_DEC_COND_COLOR_SYS,
                           (UINT32)t_col_sys,
                           0);
        #endif
        }
    }
    else
    {
        return;
        //t_col_sys = TV_DEC_COLOR_SYS_UNKNOWN;
    }
}

VOID _Tvd_NotifyTVSig(UINT8 u1Path, UINT8 u1SigState)
{
#ifndef CC_DRIVER_PROGRAM

    RVChannel*                pt_rv_channel = NULL;
    _NotifyStatus*            pt_nfy_sts = NULL;
    BOOL                      b_is_connected = FALSE;
    UINT8                     ui1_comp_id = 0xFF;
#ifdef CC_SUPPORT_TVE
    AVC_SCART_INFO_T          t_scart_info;
#endif

    
    if( u1Path == SV_VP_MAIN )
    {
        ui1_comp_id    = 0;
        b_is_connected = _afgTvdIsConnected[0];
        pt_nfy_sts     = &_tMainNotifyStatus;
        pt_rv_channel  = &_rMChannel;
    }
    else if ( u1Path == SV_VP_PIP )
    {
        ui1_comp_id    = 1;
        b_is_connected = _afgTvdIsConnected[1];
        pt_nfy_sts     = &_tPipNotifyStatus;
        pt_rv_channel  = &_rPChannel;
    }
    else
    {
        TVD_DBG_MSG("[TVD] Ignore TV signal notify(path=%u, drv status=%u)\n", 
            u1Path, u1SigState);
        return;
    }

    if( !b_is_connected )
    {
        TVD_DBG_MSG("[TVD] Ignore NOT connected TV signal notify(path=%u, drv status=%u)\n", 
            u1Path, u1SigState);
    //    return;
    }

    if( ((pt_nfy_sts->u1SigStatus != u1SigState) || 
         (pt_nfy_sts->u1NeedNotify == TRUE)) )
    {
        if ( (u1SigState == SV_VDO_NOSIGNAL) || (u1SigState == SV_VDO_UNKNOWN))
        {
            _TvdResetNfyStatus( u1Path );
        }
        pt_nfy_sts->u1SigStatus = u1SigState;

        if( (_arTVNfyInfo[ui1_comp_id].pf_tv_dec_nfy != NULL) && 
            (pt_rv_channel->t_Tvd_Ctrl == TV_DEC_CTRL_PLAY) )
        {
            #ifdef DEBUG
            {
                static CHAR*    as_tv_sig_str[] = {
                    "TV_DEC_TV_SIG_UNKNOWN",
                    "TV_DEC_TV_SIG_NONE",
                    "TV_DEC_TV_SIG_STABLE" };
                
                TVD_DBG_MSG("[TVD] _Tvd_NotifyTVSig(%d) %s, tick=%u\n",
                    ui1_comp_id,
                    as_tv_sig_str[(UINT32)_drv_tv_sig_sts_2_mw(u1SigState)],
                    x_os_get_sys_tick() );
            }
            #endif
            
            //LOG(8, "*** TV signal ***\r\n" );
            
        #ifndef LINUX_TURNKEY_SOLUTION
            _arTVNfyInfo[ui1_comp_id].pf_tv_dec_nfy(
                _arTVNfyInfo[ui1_comp_id].pv_tag,
                TV_DEC_COND_TV_SIGNAL,
                _drv_tv_sig_sts_2_mw(u1SigState),
                0);
        #else
            set_tv_dec_nfy(ui1_comp_id,
                _arTVNfyInfo[ui1_comp_id].pv_tag,
                TV_DEC_COND_TV_SIGNAL,
                _drv_tv_sig_sts_2_mw(u1SigState),
                0);
        #endif

#ifdef CC_SUPPORT_TVE
            rGetVScartInfo(&t_scart_info);

            /* trigger the notification for TVSCART */
            if ((fgIsSrcAtv(u1Path) || t_scart_info.b_canal_plus_support != FALSE) &&
                bApiVideoGetSrcType(u1Path) == _TveGetScartOutSource(0) &&
                u1SigState == SV_VDO_STABLE && /* ignore no signal notify to mw */
                _arTVNfyInfo[3].pf_tv_dec_nfy != NULL)
            {
            #ifndef LINUX_TURNKEY_SOLUTION
                _arTVNfyInfo[3].pf_tv_dec_nfy(
                    _arTVNfyInfo[3].pv_tag,
                    TV_DEC_COND_TV_SIGNAL,
                    _drv_tv_sig_sts_2_mw(u1SigState),
                    0);
            #else
                set_tv_dec_nfy(3,
                    _arTVNfyInfo[3].pv_tag,
                    TV_DEC_COND_TV_SIGNAL,
                    _drv_tv_sig_sts_2_mw(u1SigState),
                    0);
            #endif
            }

            /* trigger the notification for MonitorScart*/
            if (u1Path == SV_VP_MAIN)
            {
                if(bApiVideoGetSrcType(u1Path) == _TveGetScartOutSource(1) &&
                   u1SigState == SV_VDO_STABLE && /* ignore no signal notify to mw */
                   _arTVNfyInfo[2].pf_tv_dec_nfy != NULL)
                {
                #ifndef LINUX_TURNKEY_SOLUTION
                    _arTVNfyInfo[2].pf_tv_dec_nfy(
                        _arTVNfyInfo[2].pv_tag,
                        TV_DEC_COND_TV_SIGNAL,
                        _drv_tv_sig_sts_2_mw(u1SigState),
                        0);
                #else
                    set_tv_dec_nfy(2,
                                   _arTVNfyInfo[2].pv_tag,
                                   TV_DEC_COND_TV_SIGNAL,
                                   _drv_tv_sig_sts_2_mw(u1SigState),
                                   0);
                #endif
                }

                if(bApiVideoGetSrcType(u1Path) == _TveGetScartOutSource(0) &&
                   u1SigState == SV_VDO_STABLE && /* ignore no signal notify to mw */
                   _arTVNfyInfo[3].pf_tv_dec_nfy != NULL)
                {
                #ifndef LINUX_TURNKEY_SOLUTION
                    _arTVNfyInfo[3].pf_tv_dec_nfy(
                        _arTVNfyInfo[3].pv_tag,
                        TV_DEC_COND_TV_SIGNAL,
                        _drv_tv_sig_sts_2_mw(u1SigState),
                        0);
                #else
                    set_tv_dec_nfy(3,
                                   _arTVNfyInfo[3].pv_tag,
                                   TV_DEC_COND_TV_SIGNAL,
                                   _drv_tv_sig_sts_2_mw(u1SigState),
                                   0);
                #endif
                }

            }
#endif
            
            if ( u1SigState == SV_VDO_NOSUPPORT )
            {
                _TvdNotifyResolutionChg( u1Path, u1SigState );
            }
            pt_nfy_sts->u1NeedNotify = FALSE;
        }
        else
        {
            pt_nfy_sts->u1NeedNotify = TRUE;
        }
    }

#endif /* !CC_DRIVER_PROGRAM */
}

VOID TvdConnectMainToVGA( VOID )
{
    DRV_COMP_ID_T     t_tvd_comp = { NULL, DRVT_TV_DEC, 0, FALSE, {0} };
    DRV_COMP_ID_T     t_vga_comp = { NULL, DRVT_AVC_VGA, 0, FALSE, {0} };
    
    _TvdMwConnect( 
        &t_tvd_comp,
        RM_CONN_TYPE_COMP_ID,
        (VOID*)&t_vga_comp,
        sizeof(DRV_COMP_ID_T),
        NULL,
        NULL );

    _TvdMwSet(
        &t_tvd_comp,
        TV_DEC_SET_TYPE_CTRL,
        (VOID*)(UINT32)TV_DEC_CTRL_PLAY,
        sizeof(TV_DEC_CTRL_T) );
}

BOOL TvdGetMainSigStatus( VOID )
{
    return ((_tMainNotifyStatus.u1SigStatus == SV_VDO_STABLE)? TRUE: FALSE);
}

VOID TvdGetCurrInputSrcInfo( UINT16                ui2_tvd_id,
                             DRV_TYPE_T*           pe_type, /* OUT */
                             UINT16*               pui2_id, /* OUT */
                             TV_DEC_TIMING_TYPE_T* pe_timing ) /* OUT */
{
    CRIT_STATE_T              t_cs;
    RVChannel*                pt_rv_channel = NULL;
    _NotifyStatus*            pt_nfy_sts = NULL;

    *pe_type = DRVT_UNKNOWN;

    if( ui2_tvd_id == 0 ) // main
    {
        pt_rv_channel = &_rMChannel;
        pt_nfy_sts = &_tMainNotifyStatus;
    }
    else if ( ui2_tvd_id == 1 )
    {
        pt_rv_channel = &_rPChannel;
        pt_nfy_sts = &_tPipNotifyStatus;
    }
    else
    {
        return;
    }

    t_cs = x_crit_start();
    
    *pe_type = pt_rv_channel->u2_e_type;
    *pui2_id = pt_rv_channel->u2_e_ui2_id;
    *pe_timing = pt_nfy_sts->t_resolution.e_timing_type;
    
    x_crit_end(t_cs);

    return;
}

VOID TvdNfyVdpResChgRegister( UINT16     ui2_tvd_id,
                              UCHAR      uc_vdp_id )
{
    CRIT_STATE_T              t_cs;

    t_cs = x_crit_start();

    if( ui2_tvd_id == 0 ) // main
    {
        _tMainNotifyStatus.ucVdpId = uc_vdp_id;
    }
    else if ( ui2_tvd_id == 1 )
    {
        _tPipNotifyStatus.ucVdpId = uc_vdp_id;
    }

    x_crit_end(t_cs);

}
#endif

