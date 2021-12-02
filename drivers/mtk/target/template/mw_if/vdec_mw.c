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
#include "api_video.h"
#include "drv_vbi.h"
#include "drv_scart.h"
#include "drvcust_if.h"
#include "source_table.h"

#include "x_rm_dev_types.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_tv_dec.h"

#include "x_debug.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_hamming_code.h"
#include "d_drv_cust_util.h"
#include "tvsrc_mw.h"
#include "avc_combi_mw.h"

#include "vdec_mw.h"

/* For Cache and DRAM consistence */
#include "x_hal_926.h"

#include "drv_vga.h"

/*-----------------------------------------------------------------------------
                    Macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* constants */
#ifdef CC_MT5382P
#define TVD_COMP_NS   2
#else
#define TVD_COMP_NS   1
#endif

#define DRVN_TV_DEC   "tvdec_ccc"

#define  DXX_HV_POS 1

/* TVD Message Flags used in _u4TvdMsgFlags */
#define TVD_MSG_FLAG_CC_DATA_STATUS         (((UINT32) 1) << 0)
#define TVD_MSG_FLAG_CC_DBG_ERROR           (((UINT32) 1) << 1)
#define TVD_MSG_FLAG_CC_DBG_INFO            (((UINT32) 1) << 2)
#define TVD_MSG_FLAG_TVD_DBG_INFO           (((UINT32) 1) << 3)

/* NTSC CC config */
#define TVD_CC_NRL_STATE_COUNTER_THRESHOLD      5

#define TVD_CC_NRL_THRESHOLD_BLOCK              ((UINT16) (_u1CCNRLBlock))     /* block if Noise Level beyong this, range: (0~50) */
#define TVD_CC_NRL_THRESHOLD_UNBLOCK            ((UINT16) (_u1CCNRLUnblock))   /* unblock if Noise Level beyong this, range: (0~TVD_CC_NRL_THRESHOLD_BLOCK) */

#define TVD_CC_PER_STATE_COUNTER_THRESHOLD      5
#define TVD_CC_PER_SAMPLE_WINDOW_SIZE_BY_32     4
#define TVD_CC_PER_SAMPLE_WINDOW_SIZE           (32 * TVD_CC_PER_SAMPLE_WINDOW_SIZE_BY_32)

#define TVD_CC_PER_THRESHOLD_BLOCK              ((UINT16) (_u1CCPERBlock))     /* block if Parity Error Rate beyong this, range: (0~TVD_CC_PER_SAMPLE_WINDOW_SIZE) */
#define TVD_CC_PER_THRESHOLD_UNBLOCK            ((UINT16) (_u1CCPERUnblock))   /* unblock if Parity Error Rate below this, range: (0~TVD_CC_PER_THRESHOLD_BLOCK) */

/* macros */
#ifdef DEBUG

#define TVD_DBG_MSG( m )  {if ( _u4TvdMsgFlags & TVD_MSG_FLAG_TVD_DBG_INFO ) { x_dbg_stmt m; }}

#define TVD_DBG_CC_MSG( m )   {if ( _u4TvdMsgFlags & TVD_MSG_FLAG_CC_DBG_INFO ) { x_dbg_stmt m; }}

#define TVD_DBG_CC_ERR( m )   {if ( _u4TvdMsgFlags & TVD_MSG_FLAG_CC_DBG_ERROR ) { x_dbg_stmt m; }}

#else

#define TVD_DBG_MSG( m )  

#define TVD_DBG_CC_MSG( m )   

#define TVD_DBG_CC_ERR( m )     

#endif


#define M_IS_TUNER_SRC( e_type )   ( e_type == DRVT_TUNER_SAT_ANA ||         \
                                     e_type == DRVT_TUNER_CAB_ANA ||         \
                                     e_type == DRVT_TUNER_TER_ANA )

#define TVD_COLOR_SYS_CAP        (TV_DEC_COLOR_SYS_CAP_NTSC|TV_DEC_COLOR_SYS_CAP_NTSC_443|TV_DEC_COLOR_SYS_CAP_PAL|TV_DEC_COLOR_SYS_CAP_PAL_N|TV_DEC_COLOR_SYS_CAP_PAL_M|TV_DEC_COLOR_SYS_CAP_SECAM)

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

UINT16 vMwGetHPosition(void);
UINT16 vMwGetVPosition(void);
UINT8  vMwSetHPosition(UINT8 path, UINT16 wValue) ;
UINT8  vMwSetVPosition(UINT8 path, UINT16 wValue) ;
#if DXX_HV_POS
INT16 wDrvAdjHPos(UINT8 bPath, UINT8 dir, UINT16 value);
INT16 wDrvAdjVPos(UINT8 bPath, UINT8 dir, UINT16 value);
extern UINT16 _wAdjHPos[2];
extern UINT16 _wAdjVPos[2];
#endif

#define VGA_HPOS_MIN          wDrvVGAHPositionMin()
#define VGA_HPOS_MAX          wDrvVGAHPositionMax()
#define VGA_VPOS_MIN          wDrvVGAVPositionMin()
#define VGA_VPOS_MAX          wDrvVGAVPositionMax()


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/* NTSC CC Status */
typedef struct _TVD_CC_STATUS_T
{
    /* noise level */
    BOOL        fgBlockedNRL;       /* if CC is blocked by NRL or not */
    
    UINT16      u2CntStNRL;         /* state counter for NRL */
    
    
    /* parity error rate */
    BOOL        fgBlockedPER;       /* if CC is blocked by PER or not */
    
    UINT16      u2CntStPER;         /* state counter for PER */
    
    UINT16      u2ErrRate;          /* parity error rate: (0~TVD_CC_PER_SAMPLE_WINDOW_SIZE) */

    UINT32      au4Samples[TVD_CC_PER_SAMPLE_WINDOW_SIZE_BY_32];
                                    /* window samples, 0-bit:normal, 1-bit:error */

} TVD_CC_STATUS_T;


/*-----------------------------------------------------------------------------
                    Global Data declaration
 ----------------------------------------------------------------------------*/

/* TVD common */
static UINT32                      _u4TvdMsgFlags = 0;      /* can be controlled by driver CLI read/write */
static TV_DEC_NFY_INFO_T           _arTVNfyInfo[TVD_COMP_NS];
static BOOL                        _afgTvdIsConnected[TVD_COMP_NS];

/* NTSC CC */
static UINT8                       _u1CCNRLBlock;           /* constant: TVD_CC_NRL_THRESHOLD_BLOCK   */
static UINT8                       _u1CCNRLUnblock;         /* constant: TVD_CC_NRL_THRESHOLD_UNBLOCK */
static UINT8                       _u1CCPERBlock;           /* constant: TVD_CC_PER_THRESHOLD_BLOCK   */
static UINT8                       _u1CCPERUnblock;         /* constant: TVD_CC_PER_THRESHOLD_UNBLOCK */
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
#ifndef CC_CQAM
static UINT16                      _au2EuWssLastRcv[TVD_COMP_NS];
static UINT16                      _au2EuWssCurr[TVD_COMP_NS];
#endif
static BOOL                        _afgEuWssPlayed[TVD_COMP_NS];
static TV_DEC_EU_WSS_NFY_INFO_T    _arEuWssNfyInfo[TVD_COMP_NS];



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
/*extern void _MW_VDP_HDMI_CB(UCHAR ucVdpId, UINT16 u2Data);*/
#ifndef CC_CQAM
extern void _MW_VDP_SCART_PIN_8_CB(UCHAR ucVdpId, DRV_CUSTOM_SCART_PIN_8_T t_scart_pin_8);
#endif
extern void _MW_VDP_HDMI_AFD_CB(UCHAR ucVdpId, UINT16 u2Data);
#if 0 //SUPPORT_WSS
extern UINT8 bAPIWSSSwitch(UINT8 bPath);
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

#if 1

static VOID _TvdResetNfyStatus( UINT32  u4VidPath )
{
#if !CC_DRIVER_PROGRAM
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

static INT32 _TvdTtxBufInit(
    TVD_TTX_BUF_CTX*            ptCtx)
{
    /* context: must be called from a thread context and no multithreads enter concurrently */
    
    if ( ptCtx->bInited )
    {
        return TTX_BUFR_INV_OPR;
    }
    
    ptCtx->u4SzBuf =  ( ( (UINT32) ( sizeof( TV_DEC_TTX_PKT_T ) * TTX_BUF_NUM_PKT + 
                                     sizeof( TTX_BUF_NODE ) * TTX_BUF_NUM_NODE + 3 ) )
                            & ( ~((UINT32) 0x03) ) );
    
    ptCtx->pbBuf = (BYTE *) x_mem_alloc( ptCtx->u4SzBuf );
    
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

#ifndef CC_CQAM

static void _TvdEuWssStartNotify (UINT8 u1Path)
{
    // reset some variables before playing
    _au2EuWssLastRcv[u1Path] = 0;
    _au2EuWssCurr[u1Path]    = 0;
    _afgEuWssPlayed[u1Path]   = FALSE;
    
    VBI_StartNotifyWSSOnOff(u1Path, SV_ON, NULL);
}

static void _TvdEuWssStopNotify (UINT8 u1Path)
{
    VBI_StartNotifyWSSOnOff(u1Path, SV_OFF, NULL);
}

#endif
    
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
    MLEvt_WSS = 1,
    MLEvt_SCARTFS = 2
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
			case MLEvt_WSS:	//(UINT8 u1Path, UINT16 u2Data)
            {
                _TvdProcessEuWssData ((UINT8) msg.d1, (UINT16) msg.d2);
                break;
            }

            case MLEvt_SCARTFS:
            {
#ifndef CC_CQAM
                _MW_VDP_SCART_PIN_8_CB((UCHAR) msg.d1, (DRV_CUSTOM_SCART_PIN_8_T)(VDP_SCART_PIN_8_T)msg.d2);
#endif
                break;
            }
			default: 
				LOG(1, "MLEvtBrg recived unsupported cmd [%d]\n", (unsigned int)msg.cmd);
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
#if !CC_DRIVER_PROGRAM
    
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
    VdoMLInit();
    vMLEvtBridge_Init();


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
            x_sprintf( s_name, "%s%d", DRVN_TV_DEC, u4Idx);
            
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
    
    /* TVD_CC_NRL_THRESHOLD_BLOCK */
    if(DRVCUST_OptQuery(eTVDCCNRLThresholdBlock, &u4Val) < 0)
    {
        _u1CCNRLBlock = 80;
    }
    else
    {
        _u1CCNRLBlock = (UINT8) u4Val;
    }
    
    /* TVD_CC_NRL_THRESHOLD_UNBLOCK */
    if(DRVCUST_OptQuery(eTVDCCNRLThresholdUnblock, &u4Val) < 0)
    {
        _u1CCNRLUnblock = 75;
    }
    else
    {
        _u1CCNRLUnblock = (UINT8) u4Val;
    }
    
    /* TVD_CC_PER_THRESHOLD_BLOCK */
    if(DRVCUST_OptQuery(eTVDCCPERThresholdBlock, &u4Val) < 0)
    {
        _u1CCPERBlock   = 100;
    }
    else
    {
        _u1CCPERBlock = (UINT8) u4Val;
    }
    
    /* TVD_CC_PER_THRESHOLD_UNBLOCK */
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
    }
    
    /* dump the address of TVD message control */
    Printf("\nTVD MsgCtrl: 0x%X, NRL:%d/%d, PER:%d/%d\n", 
            ((UINT32) &_u4TvdMsgFlags),
            TVD_CC_NRL_THRESHOLD_BLOCK,
            TVD_CC_NRL_THRESHOLD_UNBLOCK,
            TVD_CC_PER_THRESHOLD_BLOCK,
            TVD_CC_PER_THRESHOLD_UNBLOCK );
    
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

#if !CC_DRIVER_PROGRAM

    CRIT_STATE_T    t_cs;
    DRV_COMP_ID_T*  prVSrcCompReg;
    DRV_ID_T        rVCombiDftSrc;
    UINT8           u1Ret;
    RVChannel*      pt_rv_channel = NULL;
    _NotifyStatus*  pt_nfy_sts = NULL;
    UINT8           ui1_path = 0;
    DRV_TYPE_T      e_pre_comp_type;
    UINT16          ui2_pre_comp_id;

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

    LOG(8, "*** Connect TVD%d ***\r\n", pt_comp_id->ui2_id );
  
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

    TVD_DBG_MSG(( "\r\n*** Connect TVD%d to %u tick=%u***\r\n", 
                pt_comp_id->ui2_id, 
                prVSrcCompReg->e_type, 
                x_os_get_sys_tick() ));
    
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

#if !CC_DRIVER_PROGRAM

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
    else
    {
        return RMR_DRV_INV_SET_INFO;
    }

    LOG(8, "*** Disconnect TVD%d ***\r\n", pt_comp_id->ui2_id );
    TVD_DBG_MSG(( "*** Disconnect TVD%d tick=%u***\r\n", 
                  pt_comp_id->ui2_id, x_os_get_sys_tick() ));
  
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
#if !CC_DRIVER_PROGRAM

    UINT8 u1ColSys;
    UINT8 u1Path;
    UINT8 bDecType;
    INT32 i4_return;
    TV_DEC_MIN_MAX_INFO_T*  pt_tv_dec_min_max_info;
#if SUPPORT_VGA_AMBIGUOUS_USER_SEL
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
            t_cs = x_crit_start();
    
            *((TV_DEC_RESOLUTION_INFO_T*)pv_get_info) = pt_nfy_sts->t_resolution;

            x_crit_end( t_cs );

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
            if(((pt_comp_id->ui2_id == 0) && (fgIsMainTvd3d() == TRUE))  || ((pt_comp_id->ui2_id == 1) && (fgIsPipTvd3d() == TRUE)))
            {
                u1ColSys = u1ApiVideoGetColorSystem((UINT8)pt_comp_id->ui2_id);
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
                LOG(9, "Not VGA Decoder\n");
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
			    *(INT16*)pv_get_info = VGA_CLOCK_MAX;
		    }  
                *pz_get_info_len = sizeof(UINT8);
                i4_return = RMR_OK;
            }
            else
            {
                LOG(9, "Not VGA Decoder\n");
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
                LOG(9, "Not VGA Decoder\n");
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
                LOG(9, "Not VGA Decoder\n");
                i4_return = RMR_DRV_INV_GET_INFO;
            }
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
        break;
#define NEW_HV_POS 1
extern  UINT8 bDrvPorchTune[2];

    case TV_DEC_GET_TYPE_H_POSITION:
#if NEW_HV_POS
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
#else
        if (*pz_get_info_len >= sizeof(UINT8))
        {
            if(((pt_comp_id->ui2_id == 0) && fgIsMainVga()) || ((pt_comp_id->ui2_id == 1) && fgIsPipVga()))
            {
                *(UINT16*)pv_get_info =vMwGetHPosition();//u2GetTypeHPos;
                *pz_get_info_len = sizeof(UINT16);
                i4_return = RMR_OK;
            }
            else
            {
                LOG(9, "Not VGA Decoder\n");
                i4_return = RMR_DRV_INV_GET_INFO;
            }
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
#endif
        break;

    case TV_DEC_GET_TYPE_V_POSITION:
#if NEW_HV_POS
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
#else
        if (*pz_get_info_len >= sizeof(UINT8))
        {
            if(((pt_comp_id->ui2_id == 0) && fgIsMainVga()) || ((pt_comp_id->ui2_id == 1) && fgIsPipVga()))
            {
                *(UINT16*)pv_get_info = vMwGetVPosition();//u2GetTypeVPos;
                *pz_get_info_len = sizeof(UINT16);
                i4_return = RMR_OK;
            }
            else
            {
                LOG(9, "Not VGA Decoder\n");
                i4_return = RMR_DRV_INV_GET_INFO;
            }
        }
        else
        {
            i4_return = RMR_DRV_NOT_ENOUGH_SPACE;
        }
#endif
        break;

    case TV_DEC_GET_TYPE_CAPABILITY:
        if (*pz_get_info_len >= sizeof (UINT32))
        {
            if(pt_comp_id->ui2_id == 0) // main
            {
                *(UINT32*)pv_get_info = _rMChannel.u4_Cap;
            }
            else
            {
                *(UINT32*)pv_get_info = _rPChannel.u4_Cap;
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
#if NEW_HV_POS
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
				pt_tv_dec_min_max_info->ui4_min_value = (def>16)? def-16:0;
				pt_tv_dec_min_max_info->ui4_max_value = (def>16)? def+16:def*2;

			}
			break;
		    default:
			break;
		}

#else
            else if (bDecType == SV_VD_VGA)
            {
                switch (pt_tv_dec_min_max_info->e_get_type)
                {
                    case TV_DEC_GET_TYPE_H_POSITION:
                        pt_tv_dec_min_max_info->ui4_min_value = (UINT32)VGA_HPOS_MIN;
                        pt_tv_dec_min_max_info->ui4_max_value = (UINT32)VGA_HPOS_MAX;
                        break;
                    case TV_DEC_GET_TYPE_V_POSITION:
                        pt_tv_dec_min_max_info->ui4_min_value = (UINT32)VGA_VPOS_MIN;
                        pt_tv_dec_min_max_info->ui4_max_value = (UINT32)VGA_VPOS_MAX;
                        break;
                    default:
                        break;
                }
            }
            else
            {
#if DXX_HV_POS
                switch (pt_tv_dec_min_max_info->e_get_type)
                {
                    case TV_DEC_GET_TYPE_H_POSITION:
                        pt_tv_dec_min_max_info->ui4_min_value = (UINT32)0;
                        pt_tv_dec_min_max_info->ui4_max_value = (UINT32)64;
                        break;
                    case TV_DEC_GET_TYPE_V_POSITION:
                        pt_tv_dec_min_max_info->ui4_min_value = (UINT32)0;
                        pt_tv_dec_min_max_info->ui4_max_value = (UINT32)32;
                        break;
                    default:
                        break;
                }
#else  /* Undefined DXX_HV_POS */
                return RMR_DRV_INV_GET_INFO;
#endif /* DXX_HV_POS */

            }
#endif

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
                
#if SUPPORT_VGA_AMBIGUOUS_USER_SEL
    case TV_DEC_GET_TYPE_AMBIG_VGA_TIMING:
        pt_tv_dec_ambig_vga_timing_info = (TV_DEC_AMBIG_VGA_TIMING_INFO_T*)pv_get_info;
        if ((pt_tv_dec_ambig_vga_timing_info->e_type) == TV_DEC_AMBIG_VGA_TIMING_TYPE_NUM_IN_GRP)
        {
            pt_tv_dec_ambig_vga_timing_info->u.ui2_num = vVgaGetAmbiguousTimingNum();
            i4_return = RMR_OK;
        }
        else // The case of TV_DEC_AMBIG_VGA_TIMING_TYPE_OPTION_IN_GRP
             //          && TV_DEC_AMBIG_VGA_TIMING_TYPE_CURR_OPT_IN_GRP
        {
            vVgaQueryAmbiguousTiming(pt_tv_dec_ambig_vga_timing_info);
            
            if (pt_tv_dec_ambig_vga_timing_info->u.t_opt.ui2_ver_frequncy != 0)
            {
                i4_return = RMR_OK;
            }
            else
            {
                i4_return = RMR_DRV_GET_FAILED;
            }
        }
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


extern void vScartChangeMode(UINT8 bICIn);

static INT32 _TvdMwSet(
    DRV_COMP_ID_T*  pt_comp_id,
    DRV_SET_TYPE_T  e_set_type,
    const VOID*     pv_set_info,
    SIZE_T          z_set_info_len)
{
#if !CC_DRIVER_PROGRAM
    
    INT32          i4RetCode = RMR_DRV_INV_SET_INFO;
    UINT8          bColSys;
    INT16          i2Tmp;
    UINT8          bDecType;
#if SUPPORT_VGA_AMBIGUOUS_USER_SEL
    TV_DEC_VGA_TIMING_OPTION_T* pt_tv_dec_vga_timing_option;
#endif
    BOOL           fgIsCap = TRUE;
    RVChannel*     pt_rv_channel = NULL;
    _NotifyStatus* pt_nfy_sts = NULL;
    UINT8          ui1_path = 0;
    
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
    else
    {
        return RMR_DRV_INV_SET_INFO;
    }

    switch (e_set_type)
    {
    case TV_DEC_SET_TYPE_CTRL:
        if ( (z_set_info_len == sizeof(TV_DEC_CTRL_T)) &&
             (((TV_DEC_CTRL_T)(UINT32)pv_set_info == TV_DEC_CTRL_RESET) ||
             ((TV_DEC_CTRL_T)(UINT32)pv_set_info == TV_DEC_CTRL_STOP) ||
             ((TV_DEC_CTRL_T)(UINT32)pv_set_info == TV_DEC_CTRL_PLAY)) )
        {
            pt_rv_channel->t_Tvd_Ctrl = (TV_DEC_CTRL_T)(UINT32)pv_set_info;
            
            if( pt_rv_channel->t_Tvd_Ctrl == TV_DEC_CTRL_STOP )
            {
                #ifndef CC_CQAM
                {
                    _TvdEuWssStopNotify(ui1_path);
                }
                #endif
                
                _TvdResetNfyStatus( ui1_path );
            }
            
            if( _arTVNfyInfo[pt_comp_id->ui2_id].pf_tv_dec_nfy )
            {
                // Call Back
                _arTVNfyInfo[pt_comp_id->ui2_id].pf_tv_dec_nfy (
                    _arTVNfyInfo[pt_comp_id->ui2_id].pv_tag,
                    TV_DEC_COND_CTRL_DONE,
                    TV_DEC_SET_TYPE_CTRL,
                    (UINT32)pt_rv_channel->t_Tvd_Ctrl );
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
                
                #ifndef CC_CQAM
                {
                    _TvdEuWssStartNotify(ui1_path);
                }
                #endif
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

    case TV_DEC_SET_TYPE_PLAY_CC:
        if ( (z_set_info_len == sizeof(TV_DEC_CC_NFY_INFO_T)) &&
             (((TV_DEC_CC_NFY_INFO_T*)pv_set_info)->pf_cc_nfy != NULL) )
        {
            TVD_DBG_MSG(( "\r\n[TVD%d] PLAY CC\r\n",  pt_comp_id->ui2_id ));
            
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
        
        TVD_DBG_MSG(( "\r\n[TVD%d] STOP CC\r\n",  pt_comp_id->ui2_id ));
        
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
            (fgApiVideoColorSystem((UINT8)ui1_path, bColSys) == FALSE))
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
        
        if(fgApiVideoVgaSetClock((UINT8)ui1_path, (UINT16)(i2Tmp+127)))
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
        if(fgApiVideoVgaSetPhase((UINT8)ui1_path, (UINT16)(UINT32)pv_set_info))
        {
            u2GetTypePhase = (UINT16)(UINT32)pv_set_info;
            i4RetCode = RMR_OK;
        }
        else
        {
            i4RetCode = RMR_DRV_INV_SET_INFO;
        }
        break;
#if NEW_HV_POS
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
			vDrvSetPorchTune((UINT8)ui1_path,SV_PORCHTUNE_SCPOS);
			UINT16 def=wDrvVideoGetPorch(ui1_path,SV_VPORCH_DEFAULT);
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
#else
    case TV_DEC_SET_TYPE_H_POSITION:
        bDecType = bGetVideoDecType(ui1_path);
        if (bDecType == SV_VD_VGA)
        {
            if(vMwSetHPosition((UINT8)ui1_path, (UINT16)(UINT32)pv_set_info))
                //      if(fgApiVideoGeoHPosition((UINT8)pt_comp_id->ui2_id, (UINT16)(UINT32)pv_set_info))
            {
                u2GetTypeHPos =  (UINT16)(UINT32)pv_set_info;
                i4RetCode = RMR_OK;
            }
            else
            {
                i4RetCode = RMR_DRV_INV_SET_INFO;
            }
        }
        else
        {
            #if DXX_HV_POS
            u2GetTypeHPos =  (UINT16)(UINT32)pv_set_info;
            i4RetCode = RMR_OK;
            if (u2GetTypeHPos > 32)
            {
        	  _wAdjHPos[ui1_path] = u2GetTypeHPos-32;
            _wAdjHPos[ui1_path] &= 0x7FFF;
                wDrvAdjHPos((UINT8)ui1_path, 0, u2GetTypeHPos-32);
            }
            else
            {
                _wAdjHPos[ui1_path] = 32-u2GetTypeHPos;
                _wAdjHPos[ui1_path] |= 0x8000;
            wDrvAdjHPos((UINT8)ui1_path, 1, 32-u2GetTypeHPos);
            }
            #endif
        }
        break;

    case TV_DEC_SET_TYPE_V_POSITION:
        bDecType = bGetVideoDecType(ui1_path);
        if (bDecType == SV_VD_VGA)
        {
            if(vMwSetVPosition((UINT8)ui1_path,(UINT16)(UINT32)pv_set_info))
                //if(fgApiVideoGeoVPosition((UINT8)pt_comp_id->ui2_id, (UINT16)(UINT32)pv_set_info))
            {
                u2GetTypeVPos =  (UINT16)(UINT32)pv_set_info;
                i4RetCode = RMR_OK;
            }
            else
            {
                i4RetCode = RMR_DRV_INV_SET_INFO;
            }
        }
        else
        {
            #if DXX_HV_POS
            u2GetTypeVPos =  (UINT16)(UINT32)pv_set_info;
            i4RetCode = RMR_OK;
            if (u2GetTypeVPos > 16)
            {
        		_wAdjVPos[ui1_path] = u2GetTypeVPos-16;
        		_wAdjVPos[ui1_path] &= 0x7FFF;
                wDrvAdjVPos((UINT8)ui1_path, 0, u2GetTypeVPos-16);
            }
            else
            {
                _wAdjVPos[ui1_path] = 16-u2GetTypeVPos;
                _wAdjVPos[ui1_path] |= 0x8000;
            wDrvAdjVPos((UINT8)ui1_path, 1, 16-u2GetTypeVPos);
            }
            #endif
        }
        break;
#endif
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
        if(fgApiVideoAutoColor())
        {
            i4RetCode = RMR_ASYNC_NFY;
        }
        else
        {
            //_TvdNotifyAutoDone(pt_comp_id->ui2_id, FALSE);
            i4RetCode = RMR_DRV_INV_SET_INFO;
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
            
            TVD_DBG_MSG(( "\r\n[TVD%d] PLAY TTX\r\n",  pt_comp_id->ui2_id ));
            
            rCS = x_crit_start ();
            _arTtxNfyInfo[ui1_path].pv_tag     = ((TV_DEC_TTX_NFY_INFO_T *)pv_set_info)->pv_tag;
            _arTtxNfyInfo[ui1_path].pf_ttx_nfy = ((TV_DEC_TTX_NFY_INFO_T *)pv_set_info)->pf_ttx_nfy;
            fgTtxAlreadyEnabled                = _afgTtxEnabled[ui1_path];
            _afgTtxEnabled[ui1_path]           = TRUE;
            x_crit_end (rCS);
            
            if ( ! _TvdTtxBufChkInited ( & _arTtxBufCtx[ui1_path] ) )
            {
                _TvdTtxBufInit ( & _arTtxBufCtx[ui1_path] );
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
            
            TVD_DBG_MSG(( "\r\n[TVD%d] STOP TTX\r\n",  pt_comp_id->ui2_id ));
            
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
            
            TVD_DBG_MSG(( "\r\n[TVD%d] UNLOCK TTX BUF \r\n",  pt_comp_id->ui2_id ));
            
            i4Ret = _TvdTtxBufFree( 
                & _arTtxBufCtx[ui1_path],
                prBufInfo->ui2_num_pkt,
                prBufInfo->at_pkts );
                
            if ( i4Ret == TTX_BUFR_OK )
            {
                i4RetCode = RMR_OK;
            }
            else
            {
                /* Internal Error in TTX Buffer */
                
                TVD_DBG_MSG(( "\r\n[TVD%d] TTX BUF FREE ERROR (%d) \r\n",  pt_comp_id->ui2_id, i4Ret ));
                
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
            
            TVD_DBG_MSG(( "\r\n[TVD%d] FLUSH TTX BUF \r\n", pt_comp_id->ui2_id ));
            
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
                TVD_DBG_MSG(( "\r\n[TVD%d] PLAY VPS DATA \r\n", pt_comp_id->ui2_id ));
                
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
            TVD_DBG_MSG(( "\r\n[TVD%d] STOP VPS DATA\r\n",  pt_comp_id->ui2_id ));
            
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
                TVD_DBG_MSG(( "\r\n[TVD%d] PLAY EU WSS \r\n",  pt_comp_id->ui2_id ));
                
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
        
    case TV_DEC_SET_TYPE_STOP_EU_WSS:
        if ( ( ui1_path == SV_VP_MAIN && (_rMChannel.u4_Cap & TV_DEC_CAP_EU_WSS) ) || 
             ( ui1_path == SV_VP_PIP  && (_rPChannel.u4_Cap & TV_DEC_CAP_EU_WSS) )  ) 
        {
            TVD_DBG_MSG(( "\r\n[TVD%d] STOP EU WSS\r\n",  pt_comp_id->ui2_id ));

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

#if SUPPORT_VGA_AMBIGUOUS_USER_SEL        
    case TV_DEC_SET_TYPE_VGA_TIMING_OPTION:
        pt_tv_dec_vga_timing_option = (TV_DEC_VGA_TIMING_OPTION_T*)pv_set_info;
        vVgaSetAmbiguousTiming(pt_tv_dec_vga_timing_option);
        
        if( pt_tv_dec_vga_timing_option->ui2_opt_index == 0)
            i4RetCode = RMR_DRV_SET_FAILED;
        else
            i4RetCode = RMR_OK;
        break;
#endif

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
      _arTVNfyInfo[ui2_id].pf_tv_dec_nfy (_arTVNfyInfo[ui2_id].pv_tag,
                                  TV_DEC_COND_CTRL_DONE,
                                  TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS,
                                  NULL);
    }
  }
  else
  {
    if(_arTVNfyInfo[ui2_id].pf_tv_dec_nfy)
    {
      // Call Back
      _arTVNfyInfo[ui2_id].pf_tv_dec_nfy (_arTVNfyInfo[ui2_id].pv_tag,
                                  TV_DEC_COND_ERROR,
                                  TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS,
                                  NULL);
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
      _arTVNfyInfo[ui2_id].pf_tv_dec_nfy (_arTVNfyInfo[ui2_id].pv_tag,
                                  TV_DEC_COND_CTRL_DONE,
                                  TV_DEC_SET_TYPE_AUTO_COLOR,
                                  NULL);
    }
  }
  else
  {
    if(_arTVNfyInfo[ui2_id].pf_tv_dec_nfy)
    {
      // Call Back
      _arTVNfyInfo[ui2_id].pf_tv_dec_nfy (_arTVNfyInfo[ui2_id].pv_tag,
                                  TV_DEC_COND_ERROR,
                                  TV_DEC_SET_TYPE_AUTO_COLOR,
                                  NULL);
    }
  }
}


VOID _TvdNotifyResolutionChg(UINT8 u1Path, UINT8 u1SigState)
{
    CRIT_STATE_T              t_cs;
    RVChannel*                pt_rv_channel = NULL;
    TV_DEC_RESOLUTION_INFO_T  t_old_res, t_resolution;
    _NotifyStatus*            pt_nfy_sts = NULL;
    BOOL                      b_should_nfy = FALSE;
    DRV_TYPE_T                e_curr_src_type = DRVT_UNKNOWN;

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
        TV_DEC_RES_EXT_SET_HDMI_SCAN_INFO( t_resolution.ui4_ext, bHDMIScanInfo() );
        TV_DEC_RES_EXT_SET_HDMI_PIC_ASP_RATIO( t_resolution.ui4_ext, bHDMIAspectRatio() );
        TV_DEC_RES_EXT_SET_HDMI_AFAR( t_resolution.ui4_ext, bHDMIAFD() );
        /*_MW_VDP_HDMI_CB(u1Path,(UINT16) bHDMIAspectRatio());*/
        _MW_VDP_HDMI_AFD_CB(u1Path,bHDMIAFD());
        t_resolution.e_color_fmt = ((bHDMIInputType()==0)? 
            TV_DEC_COLOR_FMT_YUV_4_2_2:TV_DEC_COLOR_FMT_VGA_4_4_4 );
    }
    else
    {
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
    
#if 0 //SUPPORT_WSS
    switch (bAPIWSSSwitch(u1Path))
    {
    case SV_WS_4W_3H:
        t_resolution.e_src_asp_ratio = TV_DEC_SRC_ASPECT_RATIO_4_3;
        break;
    case SV_WS_16W_9H:
        t_resolution.e_src_asp_ratio = TV_DEC_SRC_ASPECT_RATIO_16_9;
        break;
    default:
        t_resolution.e_src_asp_ratio = TV_DEC_SRC_ASPECT_RATIO_UNKNOWN;
        break;
    }
#endif

    /* Get old resolution */
    t_cs = x_crit_start();

    t_old_res = pt_nfy_sts->t_resolution;
    b_should_nfy = (BOOL)pt_nfy_sts->u1NeedNotify;
    
    x_crit_end(t_cs);
    
    /* Do nothing if resolution has no change and b_should_nfy is FALSE */
    if( x_memcmp( &t_old_res, &t_resolution, sizeof(TV_DEC_RESOLUTION_INFO_T) ) == 0 && 
        !b_should_nfy )
    {
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
        TVD_DBG_MSG(( "[TVD] res_nfy(%ux%u, %uHz, idx=%u, ext=%u)\r\n",
            t_resolution.ui2_width,
            t_resolution.ui2_height,
            t_resolution.ui2_frame_rate,
            t_resolution.ui4_res_idx,
            t_resolution.ui4_ext ));
        
        _arTVNfyInfo[u1Path].pf_tv_dec_nfy (
            _arTVNfyInfo[u1Path].pv_tag,
            TV_DEC_COND_RESOLUTION_CHG,
            (UINT32)&t_resolution,
            0);
        
        t_cs = x_crit_start();
        
        pt_nfy_sts->u1NeedNotify = FALSE;
        
        x_crit_end(t_cs);
    }
}

static VOID _TvdCCUtilUpdatePER(
    TVD_CC_STATUS_T*        prCCStatus,
    UINT8                   u1Data)
{

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
            TVD_DBG_CC_ERR(("[CC] Internal error @ %s:%d\n", __FILE__, __LINE__));
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
            TVD_DBG_CC_ERR(("[CC] Internal error @ %s:%d\n", __FILE__, __LINE__));
        }
    }
}    

static VOID _TvdCCUtilUpdateStatus(
    TVD_CC_STATUS_T*        prCCStatus,
    UINT8                   d1, 
    UINT8                   d2)
{
    UINT16   u2NRLCurr;
    
    u2NRLCurr = (UINT16) bTvdNRLevel();
    
    /* update cc status by noise level */
    if(prCCStatus->fgBlockedNRL)
    {
        /* check if need to unblock */
        
        if(u2NRLCurr <= TVD_CC_NRL_THRESHOLD_UNBLOCK)
        {
            prCCStatus->u2CntStNRL++;

            if(TVD_CC_IS_STATE_STABLE(prCCStatus->u2CntStNRL, TVD_CC_NRL_STATE_COUNTER_THRESHOLD))
            {
                /* the noise level is stable in low level, unnblock it */
                prCCStatus->fgBlockedNRL = FALSE;
                
                prCCStatus->u2CntStNRL = 0;
                
                TVD_DBG_CC_ERR(("[TVD] CC is turned on by NRL\n"));
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
        
        if(u2NRLCurr >= TVD_CC_NRL_THRESHOLD_BLOCK)
        {
            prCCStatus->u2CntStNRL++;
            
            if(TVD_CC_IS_STATE_STABLE(prCCStatus->u2CntStNRL, TVD_CC_NRL_STATE_COUNTER_THRESHOLD))
            {
                /* the noise level is stable in high level, block it */
                prCCStatus->fgBlockedNRL = TRUE;
                
                prCCStatus->u2CntStNRL = 0;
                
                TVD_DBG_CC_ERR(("[TVD] CC is turned off by NRL\n"));
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
        
        if(prCCStatus->u2ErrRate <= TVD_CC_PER_THRESHOLD_UNBLOCK)
        {
            prCCStatus->u2CntStPER ++;
            
            if(prCCStatus->u2CntStPER >= TVD_CC_PER_STATE_COUNTER_THRESHOLD)
            {
                 /* the PER is stable in low level, unblock it */
                prCCStatus->fgBlockedPER = FALSE;
                
                prCCStatus->u2CntStPER = 0;
            
                TVD_DBG_CC_ERR(("[TVD] CC is turned on by PER\n"));
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
        
        if(prCCStatus->u2ErrRate >= TVD_CC_PER_THRESHOLD_BLOCK)
        {
            prCCStatus->u2CntStPER ++;
            
            if(prCCStatus->u2CntStPER >= TVD_CC_PER_STATE_COUNTER_THRESHOLD)
            {
                 /* the PER is stable in high level, block it */
                prCCStatus->fgBlockedPER = TRUE;
                
                prCCStatus->u2CntStPER = 0;
                
                TVD_DBG_CC_ERR(("[TVD] CC is turned off by PER\n"));
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
    if(_u4TvdMsgFlags & TVD_MSG_FLAG_CC_DATA_STATUS)
    {
        Printf ("CC: [NRL] %2d [PER] %3d/%d\n", u2NRLCurr, prCCStatus->u2ErrRate, TVD_CC_PER_SAMPLE_WINDOW_SIZE);
    }
}

void _TvdNotifyCCData(UINT8 u1Path, UINT8 d1, UINT8 d2, UINT8 field)
{
    if(_arCCNfyInfo[u1Path].pf_cc_nfy)
    {
        /* report data in debug mode */
        #ifdef DEBUG
        {
            static UINT32   aui4_cnt[2] = { 0, 0 };
            
            TVD_DBG_CC_MSG(( "(%d)<%.2X %.2X> ", u1Path, d1, d2 ));
            aui4_cnt[u1Path]++;
            if ( aui4_cnt[u1Path] % 6 == 0 )
            {
                TVD_DBG_CC_MSG(("\n"));
            }
        }
        #endif
        
        /* update cc status */
        _TvdCCUtilUpdateStatus(&_arCCStatus[u1Path], d1, d2);
        
        /* notify data */
        if(!_arCCStatus[u1Path].fgBlockedNRL && 
           !_arCCStatus[u1Path].fgBlockedPER )
        {
            TV_DEC_CC_DATA_T cc_data;
                
            cc_data.ui1_byte1    = d1;
            cc_data.ui1_byte2    = d2;
            cc_data.b_even_field = field;
            
            _arCCNfyInfo[u1Path].pf_cc_nfy (
                _arCCNfyInfo[u1Path].pv_tag,
                TV_DEC_CC_COND_DATA_ARRIVAL,
                &cc_data);
        }
    }
}

void _TvdNotifyTtxData(UINT8 u1Path, UINT16 u2NumPktAvail, UINT32 _dVbiReadPtr)
{
    BYTE                    *pbRdr = (BYTE *) _dVbiReadPtr ;
    TV_DEC_TTX_PKT_T        *parPkts;
    INT32                   i4Ret;
    CRIT_STATE_T            rCS;
    TV_DEC_TTX_NFY_INFO_T   rTtxNfyInfo;
    BOOL                    fgTtxEnabled, fgTtxBufErrPaused;
    
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
            
            /* prepare notify data */
            rTtxData.t_data.ui2_num_pkt = u2NumPktAvail;
            rTtxData.t_data.at_pkts     = parPkts;
            
            /* notify data arrival */
            rTtxNfyInfo.pf_ttx_nfy(
                rTtxNfyInfo.pv_tag,
                TV_DEC_TTX_COND_DATA_ARRIVAL,
                (VOID *) & rTtxData );
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
                rTtxNfyInfo.pf_ttx_nfy(
                    rTtxNfyInfo.pv_tag,
                    TV_DEC_TTX_COND_ERROR,
                    (VOID *) TV_DEC_TTX_ERR_BUFF_OVER_FLOW );
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
                rTtxNfyInfo.pf_ttx_nfy(
                    rTtxNfyInfo.pv_tag,
                    TV_DEC_TTX_COND_ERROR,
                    (VOID *) TV_DEC_TTX_ERR_BUFF_ERROR );
            }
        }
    }
}    

/* size of au1Data[] is TV_DEC_VPS_DATA_SIZE */
void _TvdNotifyVPSData(UINT8  au1Data[])
{
    if (x_memcmp (au1Data, _au1VpsDataNtfd, TV_DEC_VPS_DATA_SIZE))
    {
        if (_rVpsDataNfyInfo.pf_vps_data_nfy)
        {
            TV_DEC_VPS_DATA_T   t_vps_data;
            
            t_vps_data.aui1_data = au1Data;
           
            _rVpsDataNfyInfo.pf_vps_data_nfy (
                    _rVpsDataNfyInfo.pv_tag,
                    TV_DEC_VPS_DATA_COND_DATA_CHANGED,
                    (VOID *) & t_vps_data );

            TVD_DBG_MSG(("[TVD] VPS Data Changed notify \n"));

            x_memcpy (_au1VpsDataNtfd, au1Data, TV_DEC_VPS_DATA_SIZE);
        }
    }    
}     

void _TvdNotifyEUWSSData(UINT8 u1Path, UINT16 u2Data)
{
	vMLEvtBridge_MsgSend(MLEvt_WSS, u1Path, u2Data, NULL);
}    

#ifndef CC_CQAM
extern void _MW_VDP_WSS_CB(UCHAR ucVdpId, UINT16 u2Data);
#endif

static void _TvdProcessEuWssData(UINT8 u1Path, UINT16 u2EuWss)
{
    #ifndef CC_CQAM
    {
        /* notify VDP with error handling */
        if (u2EuWss == _au2EuWssLastRcv[u1Path])
        {
            /* notify VDP only when wss is changed */
            if (u2EuWss != _au2EuWssCurr[u1Path])
            {
                /* notify VDP */
                _MW_VDP_WSS_CB (GET_VDP_ID_FROM_PATH(u1Path), u2EuWss);
                        
                _au2EuWssCurr[u1Path] = u2EuWss;
            }
        }

        _au2EuWssLastRcv[u1Path] = u2EuWss;
    }
    #endif
    
    
    /* notify middleware without error handling */
    if ( _afgEuWssPlayed[u1Path] && 
         _arEuWssNfyInfo[u1Path].pf_eu_wss_nfy ) 
    {
        TV_DEC_EU_WSS_DATA_T t_eu_wss;

        t_eu_wss.ui2_data = u2EuWss;
        TVD_DBG_MSG(("[TVD] EU WSS nfy <%d>\n", u2EuWss));

        _arEuWssNfyInfo[u1Path].pf_eu_wss_nfy (
                _arEuWssNfyInfo[u1Path].pv_tag,
                TV_DEC_EU_WSS_COND_DATA_ARRIVAL,
                & t_eu_wss );
    }
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

    if(u1Path != SV_VP_MAIN)
    {
        ASSERT(u1Path == SV_VP_PIP);
        u1Path = SV_VP_PIP;
    }

    if(u1SigState == SV_VDO_NOSIGNAL)
    {
        return;
    }
    
    if ( !_arTVNfyInfo[u1Path].pf_tv_dec_nfy )
    {
        return;
    }
    
    if( ((u1Path == SV_VP_MAIN)&&(fgIsMainTvd3d())) ||
        ((u1Path == SV_VP_PIP)&&(fgIsPipTvd3d())) )
    {

        u1ColSys = u1ApiVideoGetColorSystem(u1Path);

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

            TVD_DBG_MSG(("[ffr] _TvdNotifyColorSystem(%u) color_sys=%u, tick=%u\n",
                u1Path, t_col_sys, x_os_get_sys_tick() ));

            _arTVNfyInfo[u1Path].pf_tv_dec_nfy (_arTVNfyInfo[u1Path].pv_tag,
                                  TV_DEC_COND_COLOR_SYS,
                                  (UINT32)t_col_sys,
                                  0);
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
#if !CC_DRIVER_PROGRAM

    RVChannel*                pt_rv_channel = NULL;
    _NotifyStatus*            pt_nfy_sts = NULL;
    BOOL                      b_is_connected = FALSE;
    UINT8                     ui1_comp_id = 0xFF;
    
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
        TVD_DBG_MSG(("[TVD] Ignore TV signal notify(path=%u, drv status=%u)\n", 
            u1Path, u1SigState));
        return;
    }

    if( !b_is_connected )
    {
        TVD_DBG_MSG(("[TVD] Ignore NOT connected TV signal notify(path=%u, drv status=%u)\n", 
            u1Path, u1SigState));
        return;
    }
    if ( u1SigState == SV_VDO_UNKNOWN )
    {
        TVD_DBG_MSG(("[TVD] Ignore unknown TV signal notify(path=%u, drv status=%u)\n", 
            u1Path, u1SigState));
        return;
    }

    if( ((pt_nfy_sts->u1SigStatus != u1SigState) || 
         (pt_nfy_sts->u1NeedNotify == TRUE)) )
    {
        if ( u1SigState == SV_VDO_NOSIGNAL )
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
                
                TVD_DBG_MSG(("[TVD] _Tvd_NotifyTVSig(%d) %s, tick=%u\n",
                    ui1_comp_id,
                    as_tv_sig_str[(UINT32)_drv_tv_sig_sts_2_mw(u1SigState)],
                    x_os_get_sys_tick() ));
            }
            #endif
            
            LOG(8, "*** TV signal ***\r\n" );
            
            _arTVNfyInfo[ui1_comp_id].pf_tv_dec_nfy(
                _arTVNfyInfo[ui1_comp_id].pv_tag,
                TV_DEC_COND_TV_SIGNAL,
                _drv_tv_sig_sts_2_mw(u1SigState),
                0);
            
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
#endif
